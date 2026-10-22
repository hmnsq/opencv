// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#include "precomp.hpp"
#include "arithm_expr.hpp"
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <map>
#include <ostream>
#include <string>

namespace cv { namespace ew {

// ============================ op metadata (was ew_op.cpp) ============================

const char* opName(TOp op)
{
    switch (op)
    {
    case OP_NOP:           повернути "nop";
    case OP_NEG:           повернути "neg";
    case OP_ABS:           повернути "abs";
    case OP_NOT:           повернути "not";
    case OP_SQRT:          повернути "sqrt";
    case OP_EXP:           повернути "exp";
    case OP_LOG:           повернути "log";
    case OP_SIN:           повернути "sin";
    case OP_COS:           повернути "cos";
    case OP_TANH:          повернути "tanh";
    case OP_ERF:           повернути "erf";
    case OP_RELU:          повернути "relu";
    case OP_CAST:          повернути "cast";
    case OP_ADD:           повернути "add";
    case OP_SUB:           повернути "sub";
    case OP_MUL:           повернути "mul";
    case OP_DIV:           повернути "div";
    case OP_POW:           повернути "pow";
    case OP_MIN:           повернути "min";
    case OP_MAX:           повернути "max";
    case OP_ABSDIFF:       повернути "absdiff";
    case OP_ADDW:          повернути "addWeighted";
    case OP_HYPOT:         повернути "hypot";
    case OP_ATAN2:         повернути "atan2";
    case OP_AND:           повернути "and";
    case OP_OR:            повернути "or";
    case OP_XOR:           повернути "xor";
    case OP_CMP_EQ:        повернути "cmp_eq";
    case OP_CMP_NE:        повернути "cmp_ne";
    case OP_CMP_LT:        повернути "cmp_lt";
    case OP_CMP_LE:        повернути "cmp_le";
    case OP_CMP_GT:        повернути "cmp_gt";
    case OP_CMP_GE:        повернути "cmp_ge";
    case OP_CLAMP:         повернути "clamp";
    case OP_SELECT:        повернути "select";
    case OP_CONVERT_SCALE: повернути "convert_scale";
    default:               повернути "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(TOp op)
{
    switch (op)
    {
    case OP_AND: case OP_OR: case OP_XOR: case OP_NOT:
        повернути CAT_BITWISE;
    case OP_CMP_EQ: case OP_CMP_NE: case OP_CMP_LT:
    case OP_CMP_LE: case OP_CMP_GT: case OP_CMP_GE:
        повернути CAT_COMPARE;
    case OP_SQRT: case OP_EXP: case OP_LOG:
    case OP_SIN: case OP_COS: case OP_TANH: case OP_ERF: case OP_ATAN2: case OP_RELU:
        повернути CAT_MATH;
    case OP_CAST: case OP_CONVERT_SCALE:
        повернути CAT_CAST;
    case OP_SELECT:
        повернути CAT_SELECT;
    default:
        повернути CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
static bool isFloatDepth(ціле d)
{
    повернути d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
static bool depthRepresents(double v, ціле depth)
{
    якщо (isFloatDepth(depth)) повернути true;
    якщо (v != std::floor(v)) повернути false;
    switch (depth)
    {
    case CV_8U:  повернути v >= 0            && v <= 255;
    case CV_8S:  повернути v >= -128         && v <= 127;
    case CV_16U: повернути v >= 0            && v <= 65535;
    case CV_16S: повернути v >= -32768       && v <= 32767;
    case CV_32U: повернути v >= 0            && v <= 4294967295.0;
    case CV_32S: повернути v >= -2147483648.0 && v <= 2147483647.0;
    case CV_64U: повернути v >= 0            && v <= 18446744073709551615.0;
    case CV_64S: повернути v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    default:     повернути false;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
static void intRange(ціле depth, double& lo, double& hi)
{
    switch (depth)
    {
    case CV_8U:  lo = 0;                     hi = 255; break;
    case CV_8S:  lo = -128;                  hi = 127; break;
    case CV_16U: lo = 0;                     hi = 65535; break;
    case CV_16S: lo = -32768;                hi = 32767; break;
    case CV_32U: lo = 0;                     hi = 4294967295.0; break;
    case CV_32S: lo = -2147483648.0;         hi = 2147483647.0; break;
    case CV_64U: lo = 0;                     hi = 18446744073709551615.0; break;
    case CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; break;
    default:     lo = 0;                     hi = 0; break;
    }
}

// numpy-style arithmetic promotion - INTEGER-PRESERVING and COMMUTATIVE. Same signedness -> the wider
// integer (keeps the sign). Mixed sign -> a SIGNED result wide enough to hold the unsigned operand's
// range (8u+8s -> 16s, 16u+16s -> 32s, 32u+32s -> 64s; 64-bit mixed -> 64F, no 128-bit int exists).
// Any float -> the smallest float covering both operands (8-bit int / 16F / 16BF -> a 16-bit float,
// 16-bit int / 32F -> 32F, 32/64-bit int / 64F -> 64F; two DISTINCT 16-floats widen to 32F). A flexible
// operand (EW_DEPTH_NONE == -1) lands in the reserved slot 0 of every LUT (size 0, unsigned, non-float),
// so the integer path just returns the other operand - no explicit NONE guard needed. Declared in
// ew_op.hpp.
//
// Packed lookup tables, one field per depth at slot (depth+1):
//   leszlut - 3-bit size class: 8-bit -> 1, 16-bit -> 2, 32-bit -> 3, 64-bit -> 4 (NONE/Bool -> 0)
//   signlut - 1 bit, set for signed integer depths
//   fltlut  - 1 bit, set for float depths
ціле promoteArith(ціле a, ціле b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    якщо (a == b) повернути a;

    ціле wa = ціле((leszlut >> (a+1)*3) & 7u), wb = ціле((leszlut >> (b+1)*3) & 7u);
    ціле fa = ціле((fltlut >> (a+1)) & 1u),    fb = ціле((fltlut >> (b+1)) & 1u);

    якщо (fa + fb == 0)                          // both integer
    {
        ціле sa = ціле((signlut >> (a+1)) & 1u), sb = ціле((signlut >> (b+1)) & 1u);
        якщо (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            повернути wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        ціле rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr ціле ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        повернути (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    ціле maxw = std::max(wa, wb);
    якщо (maxw == 2 && fa + fb == 1)
        повернути fa*a + (1 - fa)*b;
    повернути CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
ціле absdiffResultDepth(ціле depth)
{
    switch (depth)
    {
    case CV_8S:  повернути CV_8U;
    case CV_16S: повернути CV_16U;
    case CV_32S: повернути CV_32U;
    case CV_64S: повернути CV_64U;
    default:     повернути depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
static ціле promote2(ціле a, ціле b)
{
    якщо (a == b) повернути a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    const uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    const unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    повернути ціле((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
static ціле promoteBitwise(ціле a, ціле b)
{
    якщо (a == EW_DEPTH_NONE) повернути b;
    якщо (b == EW_DEPTH_NONE) повернути a;
    якщо (a == b) повернути a;
    повернути CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
static ціле safeWide(ціле depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    const uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    const unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    повернути ціле((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
enum { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
static size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, ціле srcdepth, const void* data, ціле channels)
{
    const ціле cn = std::max(1, channels);
    const size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    const size_t nu64 = (nbytes + sizeof(uint64_t) - 1) / sizeof(uint64_t);
    const size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    якщо (data) memcpy(dst, data, nbytes); інакше memset(dst, 0, nbytes);
    повернути ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
static ціле constDoubles(const TExpr& e, ціле s, AutoBuffer<double, MAX_LOCAL_CN>& buf)
{
    const TExpr::Arg& a = e.arginfo[s];
    const ціле cn = std::max(1, a.channels);
    buf.resize(cn);
    const uchar* src = (const uchar*)(e.constbuf.data() + a.constofs);
    якщо (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * sizeof(double));   // common (Scalar / parsed literal)
    інакше
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    повернути cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
static inline bool isFlexConst(const TExpr& e, ціле s)
{
    повернути e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
static bool scalarConstValue(const TExpr& e, ціле s, double& v)
{
    const TExpr::Arg& a = e.arginfo[s];
    якщо (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) повернути false;
    AutoBuffer<double, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    повернути true;
}

// Does the run of instructions ENDING at `end` compute `array * scalar` into slot `t`?
//
// emitBinary() may wrap the multiply in casts: an integer array times a fractional scalar computes
// in the float domain and lands back in the array's own type, so the run is
//     [cast widen] -> mul -> [cast narrow]
// with the multiply always present and the two casts optional. All three are contiguous and sit at
// the tail of the program (the operand was emitted immediately before the pending add), so matching
// backwards from `end` identifies the whole run. Fills the ORIGINAL array operand, the scalar, and
// the index of the run's first instruction.
static bool matchScaledChain(const TExpr& e, ціле end, ціле t,
                             ціле& arrSlot, double& scale, ціле& startIdx)
{
    якщо (end < 0) повернути false;
    ціле j = end, mulResult = t;

    // optional trailing narrowing cast
    якщо (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        якщо (--j < 0) повернути false;
    }

    const TExpr::Insn& mul = e.prog[j];
    якщо (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) повернути false;

    ціле arr;
    якщо      (scalarConstValue(e, mul.arg1, scale)) arr = mul.arg0;
    інакше якщо (scalarConstValue(e, mul.arg0, scale)) arr = mul.arg1;
    інакше повернути false;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    якщо (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    якщо (e.arginfo[arr].kind == TExpr::CONST) повернути false;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    повернути true;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
static bool constFits(const TExpr& e, ціле s, ціле d)
{
    якщо (!isFlexConst(e, s)) повернути true;
    AutoBuffer<double, MAX_LOCAL_CN> v;
    ціле cn = constDoubles(e, s, v);
    для (ціле ch = 0; ch < cn; ch++)
        якщо (!depthRepresents(v[ch], d)) повернути false;
    повернути true;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
ціле TExpr::emitBinary(TOp op, ціле a, ціле b, ціле rdepth, const Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    якщо (op == OP_ADDW)
    {
        ціле Tt = arginfo[a].depth;
        якщо (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        якщо (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        якщо (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        ціле outD = rdepth;
        якщо (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        const ціле out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        якщо (outD == rdepth) повернути out;
        const ціле out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        повернути out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    якщо (op == OP_ADD)
    {
        const ціле n = (ціле)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        ціле x = 0, y = 0, startA = 0, startB = 0;
        double alpha = 0, beta = 0;
        якщо (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*this, n - 1, b, y, beta, startB) &&
            matchScaledChain(*this, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            ціле retired = 0, lowest = INT_MAX;
            для (ціле i = startA; i < n; i++)
            {
                const ціле r = prog[i].result;
                якщо (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; break; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            якщо (retired > 0 && lowest == ntemps - retired)
            {
                для (ціле i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                повернути emitBinary(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            ціле acc = 0; double gamma = 0;
            якщо      (scalarConstValue(*this, b, gamma)) acc = a;
            інакше якщо (scalarConstValue(*this, a, gamma)) acc = b;
            якщо (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                повернути acc;
            }
        }
    }

    const ціле nd0 = isFlexConst(*this, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    const ціле nd1 = isFlexConst(*this, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    const ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    ціле result = rdepth;
    якщо (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        якщо (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    ціле base;
    якщо (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    інакше якщо (nd0 == EW_DEPTH_NONE) base = nd1;
    інакше якщо (nd1 == EW_DEPTH_NONE) base = nd0;
    інакше base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    якщо (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*this, b) && !constFits(*this, b, base))
    {
        double lo, hi; intRange(nd0, lo, hi);
        якщо (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            enum { REAL, CFALSE, CTRUE };
            AutoBuffer<double, MAX_LOCAL_CN> tv; ціле cn = constDoubles(*this, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            ціле kind[4] = {}; double bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            const TOp fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            для (ціле c = 0; c < cn; c++)
            {
                const double t = tv[c]; ціле k; double B = t;
                switch (op)
                {
                case OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; break;
                case OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; break;
                case OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; break;
                case OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; break;
                case OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; break;   // a==non-rep -> false
                default:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  break;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](ціле c){ повернути kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](ціле c){ повернути kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            const TOp u0 = ucOp(0);
            bool uniform = true;
            для (ціле c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            якщо (uniform)
            {
                Scalar s; для (ціле c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            інакше
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; ціле patchFlags = 0;
                для (ціле c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    const ціле mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    const ціле vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                const ціле out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                повернути out;
            }
        }
    }

    // compute depth + wide fallback per family:
    //   MIN/MAX, AND/OR/XOR         : T x T -> T, never widen   (depth = result)
    //   ABSDIFF                     : T x T -> unsigned same width for signed ints (8s->8u, ...)
    //   COMPARE                     : common type -> mask        (depth = base)
    //   MUL/DIV                     : float work (f64 if wide)   (matches cv::multiply/divide)
    //   POW                         : float work
    //   ADD/SUB                     : common type, signed wide fallback
    ціле depth, wide;
    bool divGuard = false;
    switch (op)
    {
    case OP_MIN: case OP_MAX:
    case OP_AND: case OP_OR: case OP_XOR:
        depth = result; wide = result; break;
    case OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        ціле common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        якщо (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        якщо (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        break;
    }
    case OP_CMP_EQ: case OP_CMP_NE: case OP_CMP_LT:
    case OP_CMP_LE: case OP_CMP_GT: case OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; break;
    case OP_MUL: case OP_DIV:
    {
        depth = base;
        const bool w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        якщо (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](ціле s, ціле nd) {
                якщо (nd != EW_DEPTH_NONE) повернути !isFloatDepth(nd);
                AutoBuffer<double, MAX_LOCAL_CN> v; constDoubles(*this, s, v);
                повернути v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        break;
    }
    case OP_POW: case OP_HYPOT: case OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; break;
    default:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); break;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](ціле s) {
        якщо (!isFlexConst(*this, s)) повернути false;
        AutoBuffer<double, MAX_LOCAL_CN> v; ціле n = constDoubles(*this, s, v);
        для (ціле ch = 0; ch < n; ch++)
            якщо (v[ch] != std::floor(v[ch])) повернути true;
        повернути false;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    якщо (cat != CAT_BITWISE && (!constFits(*this, a, depth) || !constFits(*this, b, depth)))
    {
        depth = wide;
        const bool frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        якщо (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        якщо (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    ціле s0 = isFlexConst(*this, a) ? typedConstFrom(a, depth) : a;
    ціле s1 = isFlexConst(*this, b) ? typedConstFrom(b, depth) : b;
    ціле c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](ціле rd) {
        повернути op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    якщо (k.fptr)
    {
        ціле res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        повернути res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ціле w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    повернути maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
ціле TExpr::emitUnary(TOp op, ціле a, ціле rdepth, const Scalar& params)
{
    const ціле nd = isFlexConst(*this, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    const ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    якщо (op == OP_NEG)
        повернути emitBinary(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    якщо (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        якщо (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            const ціле x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            повернути emitBinary(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        повернути emitBinary(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    ціле result = rdepth;
    якщо (result == EW_DEPTH_NONE)
    {
        switch (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        case CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; break;
        case CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; break;  // NOT
        default:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; break;  // NEG, ABS
        }
    }

    ціле depth, wide;
    switch (cat)
    {
    case CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        break;
    case CAT_BITWISE: depth = result; wide = result; break;
    default:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; break;  // NEG/ABS
    }

    якщо (!constFits(*this, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    ціле s0 = isFlexConst(*this, a) ? typedConstFrom(a, depth) : a;
    ціле c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    якщо (k.fptr)
    {
        ціле res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        повернути res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ціле w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    повернути maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
ціле TExpr::emitTernary(TOp op, ціле a, ціле b, ціле c, ціле rdepth)
{
    auto dep = [&](ціле s) { повернути isFlexConst(*this, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    якщо (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        якщо (isFlexConst(*this, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emitBinary(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        ціле result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        якщо (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        ціле sb = isFlexConst(*this, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        ціле sc = isFlexConst(*this, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        ціле res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        повернути res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    ціле result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    якщо (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](ціле s) {
        повернути isFlexConst(*this, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    ціле c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ціле res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    повернути res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
ціле TExpr::moveToOutput(ціле temp, ціле out)
{
    const ціле ninsn = (ціле)prog.size();
    ціле producer = -1;
    bool usedAsArg = false;
    для (ціле i = 0; i < ninsn; i++)
    {
        const TExpr::Insn& ins = prog[i];
        якщо (ins.result == temp) producer = i;
        якщо (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = true;
    }
    якщо (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
        arginfo[temp].depth == arginfo[out].depth)
    {
        // MOVE semantics: redirect `temp`'s single producer to write `out` directly, then leave the
        // source slot EMPTY - reclassify it to NONE. A NONE slot gets no physical buffer (compile's
        // liveness only walks TEMP slots) and is skipped everywhere in exec, so no stale dead-TEMP slot
        // remains (which, buffer or not, would still cost per-tile setup in the general path) - and no
        // slot removal / index renumbering is needed. This keeps single-op programs at zero temps =>
        // compile()'s no-temp early-out (zero heap traffic) still fires. Every caller moves the result
        // it JUST emitted, so `temp` is always the last-created temp - decrement ntemps to retire its
        // index and keep the temp indices dense (0..ntemps-1) for compile()'s liveness arrays.
        CV_Assert(arginfo[temp].index == ntemps - 1);
        prog[producer].result = out;
        arginfo[temp].kind = NONE;
        ntemps--;
        повернути out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    повернути out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
ціле TExpr::output(ціле rootSlot)
{
    повернути moveToOutput(rootSlot, addOutput(arginfo[rootSlot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
void TExpr::compile()
{
    const ціле ninsn = (ціле)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    const ціле nslots = (ціле)arginfo.size();
    nconsts = 0;
    для (ціле s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        якщо (a.kind != CONST || a.depth == EW_DEPTH_NONE) continue;
        const ціле cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        const size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, sizeof(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)sizeof(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (const uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        const size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        якщо (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        інакше getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
        a.constofs = ofs; a.srcdepth = dd;
        nconsts++;
    }

    // ---- liveness: pack temps into a minimal set of reusable physical buffers. A program with no
    //      temps (single-op add/sub/mul/...) needs none of this - early out with ZERO heap traffic,
    //      so building such a program (the dominant cv::add-style call) allocates nothing. The temp
    //      case uses stack-backed AutoBuffers (inline for typical small expressions). ----
    nbuffers = 0;
    bufEszPrefix.resize(1); bufEszPrefix[0] = 0;
    capElems = INT_MAX;                       // no temps => exec runs each tile in one block, no scratch
    якщо (ntemps == 0) повернути;

    AutoBuffer<ціле, 32> tempOfSlot(nslots);
    для (ціле s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    для (ціле s = 1; s < nslots; s++)
        якщо (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<ціле, 16> lastUse(ntemps);
    для (ціле t = 0; t < ntemps; t++) lastUse[t] = -1;
    для (ціле i = 0; i < ninsn; i++)
    {
        const TExpr::Insn& ins = prog[i];
        ціле as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        для (ціле k = 0; k < 3; k++)
            якщо (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    для (ціле t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<ціле, 16> freeBufs(ntemps);
    ціле nfree = 0, nbuf = 0;
    для (ціле i = 0; i < ninsn; i++)
    {
        const TExpr::Insn& ins = prog[i];
        ціле rt = tempOfSlot[ins.result];
        якщо (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        ціле as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        для (ціле k = 0; k < 3; k++)
        {
            ціле t = tempOfSlot[as[k]];
            якщо (t >= 0 && lastUse[t] == i)
            {
                freeBufs[nfree++] = bufferOfTemp[t];
                lastUse[t] = -1;   // the same temp may be several args of this insn ("d*d") -
                                   // release its buffer once, or freeBufs overflows/double-frees
            }
        }
    }

    nbuffers = nbuf;

    // Byte layout of the physical temp buffers, per output element: prefix sums of each buffer's
    // max element size. bufEszPrefix[b]*region = buffer b's byte offset in the scratch; [nbuffers] =
    // total temp bytes/element. Precomputed here (depths are build-time) so exec()/its fast path
    // never recompute it per call, and the fast path can size its scratch in O(1).
    bufEszPrefix.resize(nbuffers + 1);
    для (ціле b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    для (ціле s = 1; s < nslots; s++)
        якщо (arginfo[s].kind == TEMP)
        {
            ціле b = bufferOfTemp[arginfo[s].index], e = (ціле)CV_ELEM_SIZE1(arginfo[s].depth);
            якщо (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    для (ціле b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    const ціле totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
static void matLogical(const Mat& m, MatShape& shp, EwSteps& step, ціле& esz1)
{
    esz1 = (ціле)m.elemSize1();
    ціле nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    для (ціле i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
static bool broadcastShape(const MatShape* shps, ціле K, MatShape& out)
{
    ціле nd = 0;
    для (ціле k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    для (ціле k = 0; k < K; k++)
    {
        const MatShape& s = shps[k];
        ціле off = nd - s.dims;
        для (ціле i = 0; i < s.dims; i++)
        {
            ціле d = s[i], &o = out[off + i];
            якщо (o == 1) o = d;
            інакше якщо (d != 1 && d != o) повернути false;
        }
    }
    повернути true;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
static ціле opCost(TOp op)
{
    switch (op)
    {
    case OP_ADD: case OP_SUB: case OP_MUL: case OP_MIN: case OP_MAX:
    case OP_ABSDIFF: case OP_AND: case OP_OR: case OP_XOR: case OP_NOT:
    case OP_NEG: case OP_ABS: case OP_CAST: case OP_RELU: case OP_SELECT:
    case OP_CMP_EQ: case OP_CMP_NE: case OP_CMP_LT:
    case OP_CMP_LE: case OP_CMP_GT: case OP_CMP_GE: повернути 1;
    case OP_DIV: case OP_SQRT: case OP_HYPOT: case OP_CONVERT_SCALE: повернути 3;
    case OP_SIN: case OP_COS: case OP_TANH: case OP_ERF: case OP_ATAN2:
    case OP_EXP: case OP_LOG: case OP_POW:           повернути 6;
    default:                                         повернути 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
void TExpr::clear()
{
    prog.allocate(0);
    arginfo.allocate(0);
    bufferOfTemp.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
void TExpr::dump(std::ostream& os) const
{
    auto dn = [](ціле d) -> const char* {
        switch (d) {
        case EW_DEPTH_NONE: повернути "flex";
        case CV_8U:  повернути "u8";  case CV_8S:  повернути "s8";
        case CV_16U: повернути "u16"; case CV_16S: повернути "s16";
        case CV_32U: повернути "u32"; case CV_32S: повернути "s32";
        case CV_64U: повернути "u64"; case CV_64S: повернути "s64";
        case CV_16F: повернути "f16"; case CV_16BF:повернути "bf16";
        case CV_32F: повернути "f32"; case CV_64F: повернути "f64";
        case CV_Bool:повернути "bool";
        default:     повернути "?"; }
    };
    auto kn = [](ArgKind k) -> const char* {
        switch (k) { case NONE: повернути "none"; case INPUT: повернути "in"; case CONST: повернути "const";
                     case TEMP: повернути "temp"; case OUTPUT: повернути "out"; } повернути "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (ціле)prog.size() << "\n";
    os << "  slots:\n";
    для (ціле s = 0; s < (ціле)arginfo.size(); s++) {
        const Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        якщо (a.kind != NONE) os << " " << dn(a.depth);
        якщо (a.kind == INPUT || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        якщо (a.kind == CONST) {
            const ціле cn = std::max(1, a.channels);
            AutoBuffer<double, MAX_LOCAL_CN> v(cn);
            const uchar* src = (const uchar*)(constbuf.data() + a.constofs);
            якщо (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * sizeof(double));
            інакше getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            для (ціле c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    для (ціле i = 0; i < (ціле)prog.size(); i++) {
        const Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        якщо (ins.arg1) os << ", " << ins.arg1;
        якщо (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        якщо (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        якщо (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        якщо (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
ціле TExpr::addInput(ціле depth)
{
    Arg a; a.kind = INPUT; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); повернути (ціле)arginfo.size() - 1;
}

ціле TExpr::addOutput(ціле depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); повернути (ціле)arginfo.size() - 1;
}

ціле TExpr::addTemp(ціле depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); повернути (ціле)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
ціле TExpr::addConst(ціле depth, const Scalar& v, ціле channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); повернути (ціле)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
ціле TExpr::addConst(ціле depth, ціле srcdepth, const void* data, ціле channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); повернути (ціле)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
ціле TExpr::typedConstFrom(ціле srcSlot, ціле depth)
{
    const Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); повернути (ціле)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
ціле TExpr::addInsn(TOp op, ціле a0, ціле a1, ціле a2, ціле result, const TKernel& kernel, const Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    повернути (ціле)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
ціле TExpr::addInsn(TOp op, ціле a0, ціле a1, ціле a2, ціле result, const Scalar& params)
{
    ціле d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    ціле d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    ціле d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    повернути addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
ціле TExpr::maybeAddCast(ціле arg, ціле depth)
{
    якщо (arginfo[arg].depth == depth) повернути arg;
    ціле t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    повернути t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
static inline void runInsn(const TExpr::Insn& ins,
                           const void* p0, size_t y0, size_t x0,
                           const void* p1, size_t y1, size_t x1,
                           const void* p2, size_t y2, size_t x2,
                           void* pr, size_t yr, ціле w, ціле h)
{
    ціле code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

struct EwBody {
    const TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    const ціле* slotMap;
    const signed char* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    const ціле* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    ціле ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

void TExpr::outputShape(const Mat* const* inputs, MatShape& spatial, ціле& channels) const
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    const ціле rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    bool sameShape = ninputs >= 1;
    для (ціле i = 1; sameShape && i < ninputs; i++)
    {
        const Mat& a = *inputs[i];
        якщо (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = false;
        інакше для (ціле d = 0; d < a.dims; d++) якщо (a.size[d] != inputs[0]->size[d]) { sameShape = false; break; }
    }
    якщо (sameShape) { spatial = inputs[0]->size; channels = rcn0; повернути; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; ціле esz1;
    для (ціле i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    const ціле ndFull = (ціле)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
void TExpr::outputShape(const Mat* inputs, MatShape& spatial, ціле& channels) const
{
    AutoBuffer<const Mat*, 16> ptrs(std::max(ninputs, 1));
    для (ціле i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
static constexpr ціле LOCAL_HDRS = 3;
static constexpr ціле LOCAL_CONSTS = 4;
static constexpr ціле LOCAL_OPS = 16;

void TExpr::exec(const Mat* const* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    const ціле nslots = (ціле)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    якщо (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        const Mat& r = *inputs[0];
        const ціле rcn = r.channels();
        bool ok = r.isContinuous();
        для (ціле i = 1; ok && i < ninputs; i++)
        {
            const Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        для (ціле s = 1; ok && s < nslots; s++)
            якщо (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        const size_t total = ok ? r.total() * rcn : 0;
        якщо (ok && total == 0)
        {
            const MatShape rshape = r.size;
            для (ціле s = 1; s < nslots; s++)
                якщо (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            повернути;
        }
        якщо (ok && total <= EW_FASTPATH_MAX)
        {
            const MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            для (ціле s = 1; s < nslots; s++)
                якщо (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            const ціле ninsn = (ціле)prog.size();

            якщо (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](ціле s) -> void* {
                    якщо (s <= 0) повернути nullptr;
                    const Arg& ai = arginfo[s];
                    якщо (ai.kind == INPUT)  повернути (void*)inputs[ai.index]->data;
                    якщо (ai.kind == OUTPUT) повернути (void*)outputs[ai.index].data;
                    повернути nullptr;   // NONE
                };
                const ціле w = (ціле)total;
                для (ціле n = 0; n < ninsn; n++)
                {
                    const Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                повернути;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            const ціле totalEsz = bufEszPrefix[nbuffers];
            const ціле wf0 = std::min((ціле)total, capElems);

            const size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/sizeof(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)sizeof(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            для (ціле x0 = 0; x0 < (ціле)total; x0 += wf0)
            {
                const ціле wf = std::min(wf0, (ціле)total - x0);
                auto ptrOf = [&](ціле s) -> void* {
                    якщо (s <= 0) повернути nullptr;
                    const Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    якщо (ai.kind == INPUT)  повернути (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    якщо (ai.kind == OUTPUT) повернути (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    якщо (ai.kind != TEMP)   повернути nullptr;                                    // NONE: moved-from
                    const ціле b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    повернути scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                для (ціле n = 0; n < ninsn; n++)
                {
                    const Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            повернути;
        }
    }

    // Scalars (TExpr::CONST) never influence the result shape or channel count - the output geometry
    // comes from the real array inputs alone (a scalar broadcasts into whatever they produce).
    // A flexible CONST (depth == EW_DEPTH_NONE) is a leftover literal: the emit* layers materialize a
    // typed copy at each use, so the original is dead - skipped entirely (no header built for it).

    // ---- 1. result shape (spatial dims + channel count), channels innermost ----
    // Fast path: every input shares one shape+channels => the result IS inputs[0]'s shape. Skips
    // the matLogical + broadcastShape rebuild/re-broadcast done only to size the output. Consts are
    // irrelevant here (they don't add dims/channels), so they never break the fast path.
    MatShape spatial;
    ціле rchannels;
    outputShape(inputs, spatial, rchannels);

    // ---- 2. ONE pass over slots: allocate outputs, build the operand pointer list + slot map,
    //         per-slot element size, and per-temp-buffer element size. No Mat copies (arr[] holds
    //         pointers); only real inputs + outputs become broadcast operands. TExpr::CONST scalars are
    //         NOT broadcast operands and get NO Mat header: each is materialized once into a typed
    //         scratch (constStore) and exposed to the body as a fixed slice {ptr=&value, 0, 0} - a
    //         scalar the kernels read by broadcast. slotMap[s] stays a plain non-negative index;
    //         its meaning (arr / temp / const) is recovered from arginfo[s].kind (see EwBody).
    // In-place: an input shares its data buffer with an output. An output create() may realloc that
    // buffer while an input still needs the old contents, so we incref (header-copy) every input and
    // read through the copies. Cheap data-pointer aliasing test (nullptr data => never matches).
    bool inplace = false;
    для (ціле i = 0; i < ninputs && !inplace; i++)
        для (ціле j = 0; j < noutputs; j++)
            якщо (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = true; break; }
    ціле nhdrs = inplace ? ninputs : 0;

    AutoBuffer<const Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<const Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<ціле, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed char, LOCAL_OPS> slotKind(nslots);
    ціле narr = 0, nc = 0;
    якщо (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        для (ціле j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed char)TExpr::NONE;
    для (ціле s = 1; s < nslots; s++)
    {
        const TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed char)ai.kind;
        якщо (ai.kind == TExpr::INPUT) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        інакше якщо (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        інакше якщо (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed char)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        інакше якщо (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            const ціле c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (void*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed char)TExpr::INPUT;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        інакше якщо (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
        }
        інакше {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    const ціле ninsn = (ціле)prog.size();
    long long costPerElem = 0;
    для (ціле n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    для (ціле d = 0; d < (ціле)spatial.size(); d++) otot *= spatial[d];
    якщо (otot == 0)
        повернути;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    const double T = (double)std::max(getNumThreads(), 1);
    const double nstripes = std::min(
        (double)otot * (double)std::max<long long>(costPerElem, 1) * (1./ (double)(1 << 16)),
        std::min(4.*T, std::max(32., 3.*T)));

    EwBody body;
    body.prog = prog.data();
    body.ninsn = ninsn;
    body.slotMap = slotMap.data();

    // ---- 5. drive: broadcastOp does geometry + 2D tiling + parallelism; the body runs the
    //         frozen program on each tile (temps tile-local, re-pointed from the tile slices).
    //         expandChannels=true => the body always sees single-channel data.
    //
    //         All state the body needs is packed into one POD (EwBody) so the lambda captures a
    //         SINGLE reference: the closure is then one pointer, fits std::function's small-buffer
    //         and never heap-allocates. Inside, the hot fields are copied into locals so the
    //         per-tile/per-insn loops read them from registers, not through the captured pointer. -

    body.bufEszPrefix = bufEszPrefix.data();
    body.slotKind = slotKind.data();
    body.nslots = nslots;
    body.nbuffers = nbuffers;
    body.capElems = capElems;

    broadcastOp(arr.data(), narr, [&](const BrTile& tile)
    {
        EwBody& bc = body;
        const TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        const ціле* const smap  = bc.slotMap;
        const signed char* const skind = bc.slotKind;
        const ціле nin = bc.ninsn, nsl = bc.nslots;
        const ціле w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        const ціле* const eszPrefix = bc.bufEszPrefix;
        const ціле bw = std::min(w, bc.capElems);
        const ціле bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        const size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/sizeof(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)sizeof(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        для (ціле y0 = 0; y0 < h; y0 += bh)
        {
            const ціле hf = std::min(bh, h - y0);
            для (ціле x0 = 0; x0 < w; x0 += bw)
            {
                const ціле wf = std::min(bw, w - x0);
                для (ціле s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    const ціле k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    якщо (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    інакше                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        const BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                для (ціле n = 0; n < nin; n++)
                {
                    const TExpr::Insn& ins = insns[n];
                    const BrSlice& a0 = args[ins.arg0]; const BrSlice& a1 = args[ins.arg1];
                    const BrSlice& a2 = args[ins.arg2]; const BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (void*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, true, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
void TExpr::exec(const Mat* inputs, Mat* outputs)
{
    AutoBuffer<const Mat*, 16> ptrs(std::max(ninputs, 1));
    для (ціле i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    exec(ptrs.data(), outputs);
}

// ---------------------------------------------------------------------------
// Manual program builders (stand-ins for the future engine-backed cv::add etc.): they skip the
// source graph and emit the program directly through TExpr's addInput/addTemp/addOutput/addInsn.
// ---------------------------------------------------------------------------

// Compose a binary op (ADD/SUB/MUL/DIV/MIN/MAX/ABSDIFF/CMP_*) for any (depth0, depth1, rdepth): just
// the operand/output plumbing around emitBinary, which owns the whole type policy (compute type, wide
// fallback, div's /0 guard, mul/div `scale` in params[0]). emitBinary returns the result slot;
// moveToOutput lands it in the output with no dead temp (so a single-op program keeps zero temps).
//
// maskDepth != EW_DEPTH_NONE adds a write-mask (input #2): the arithmetic result lands in a temp and a
// final select(mask, r, dst) -> dst overwrites only the masked subset of the (pre-existing) output,
// leaving the rest UNCHANGED (matching cv::add/... with a mask); the output slot rides as both the
// select's arg2 and its result (the kernel is alias-safe). select is always the LAST instruction.
// The mask is a single-channel 1-byte array (bool/u8/s8) the size of the output spatial shape; it
// rides the normal broadcast machinery, so nothing special is needed in the executor.
void makeBinaryArithProgram(TExpr& p, TOp op, ціле depth0, ціле depth1, ціле rdepth,
                            ціле maskDepth, double scale)
{
    p.clear();
    якщо (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        якщо (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        інакше
        {
            rdepth = promoteArith(depth0, depth1);
            якщо (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    const bool masked = maskDepth != EW_DEPTH_NONE;
    ціле sIn0  = p.addInput(depth0);
    ціле sIn1  = p.addInput(depth1);
    ціле sMask = masked ? p.addInput(maskDepth) : 0;
    ціле sOut  = p.addOutput(rdepth);

    ціле r = p.emitBinary(op, sIn0, sIn1, rdepth, Scalar(scale));
    якщо (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    інакше
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
void makeAddWeightedProgram(TExpr& p, ціле depth0, ціле depth1, ціле rdepth,
                            double alpha, double beta, double gamma)
{
    p.clear();
    ціле sA   = p.addInput(depth0);
    ціле sB   = p.addInput(depth1);
    ціле sOut = p.addOutput(rdepth);
    ціле r = p.emitBinary(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

namespace {

// --- tokens ------------------------------------------------------------------------------
enum TokType { T_NUM, T_INPUT, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

struct Token
{
    TokType type = T_END;
    double num = 0;
    ціле input = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
struct Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    static bool isIdentStart(char c) { повернути std::isalpha((unsigned char)c) || c == '_'; }
    static bool isIdentChar(char c)  { повернути std::isalnum((unsigned char)c) || c == '_'; }

    Token next()
    {
        поки (pos < s.size() && std::isspace((unsigned char)s[pos])) pos++;
        Token t;
        якщо (pos >= s.size()) { t.type = T_END; повернути t; }

        char c = s[pos];

        якщо (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            поки (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_INPUT;
            t.input = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            повернути t;
        }
        якщо (std::isdigit((unsigned char)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned char)s[pos + 1])))
        {
            char* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_NUM;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            повернути t;
        }
        якщо (isIdentStart(c))
        {
            size_t start = pos;
            поки (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            повернути t;
        }
        switch (c)
        {
        case '(': pos++; t.type = T_LPAREN; повернути t;
        case ')': pos++; t.type = T_RPAREN; повернути t;
        case ',': pos++; t.type = T_COMMA;  повернути t;
        case ';': pos++; t.type = T_SEMI;   повернути t;
        }
        // multi/!single-char operators
        auto two = [&](const char* op) {
            повернути pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        якщо (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; повернути t; }
        якщо (c == '=') { pos++; t.type = T_ASSIGN; повернути t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        повернути t;
    }
};

// --- operator / function tables ----------------------------------------------------------
static ціле binPrec(const std::string& op)
{
    якщо (op == "**") повернути 8;              // power binds tighter than '*'; RIGHT-associative
    якщо (op == "*" || op == "/") повернути 7;
    якщо (op == "+" || op == "-") повернути 6;
    якщо (op == "<" || op == "<=" || op == ">" || op == ">=") повернути 5;
    якщо (op == "==" || op == "!=") повернути 4;
    якщо (op == "&") повернути 3;
    якщо (op == "^") повернути 2;
    якщо (op == "|") повернути 1;
    повернути -1;
}

static TOp binOp(const std::string& op)
{
    якщо (op == "**") повернути OP_POW;
    якщо (op == "+")  повернути OP_ADD;
    якщо (op == "-")  повернути OP_SUB;
    якщо (op == "*")  повернути OP_MUL;
    якщо (op == "/")  повернути OP_DIV;
    якщо (op == "<")  повернути OP_CMP_LT;
    якщо (op == "<=") повернути OP_CMP_LE;
    якщо (op == ">")  повернути OP_CMP_GT;
    якщо (op == ">=") повернути OP_CMP_GE;
    якщо (op == "==") повернути OP_CMP_EQ;
    якщо (op == "!=") повернути OP_CMP_NE;
    якщо (op == "&")  повернути OP_AND;
    якщо (op == "|")  повернути OP_OR;
    якщо (op == "^")  повернути OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
static ціле typeDepth(const std::string& name)
{
    якщо (name == "float")    повернути CV_32F;
    якщо (name == "double")   повернути CV_64F;
    якщо (name == "half" || name == "float16") повернути CV_16F;
    якщо (name == "bfloat16") повернути CV_16BF;
    якщо (name == "uint8")    повернути CV_8U;
    якщо (name == "int8")     повернути CV_8S;
    якщо (name == "uint16")   повернути CV_16U;
    якщо (name == "int16")    повернути CV_16S;
    якщо (name == "uint32")   повернути CV_32U;
    якщо (name == "int32")    повернути CV_32S;
    якщо (name == "uint64")   повернути CV_64U;
    якщо (name == "int64")    повернути CV_64S;
    повернути -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
static TOp fnOp(const std::string& name, ціле& arity)
{
    struct E { const char* n; TOp op; };
    static const E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    static const E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    static const E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    для (const E& e : unary)  якщо (name == e.n) { arity = 1; повернути e.op; }
    для (const E& e : binary) якщо (name == e.n) { arity = 2; повернути e.op; }
    для (const E& e : tern)   якщо (name == e.n) { arity = 3; повернути e.op; }
    arity = 0; повернути OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
struct Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    const ціле* inputSlot;                 // slot id of each external input (precreated)
    ціле ninputs;
    std::map<std::string, ціле> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& expr, const ціле* islot, ціле nin)
        : lex(src), e(expr), inputSlot(islot), ninputs(nin) { cur = lex.next(); }

    void advance() { cur = lex.next(); }
    bool isOp(const char* op) const { повернути cur.type == T_OP && cur.text == op; }

    void expect(TokType t, const char* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    bool isFlexConst(ціле slot) const
    {
        повернути e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    ціле parsePrimary()
    {
        якщо (cur.type == T_NUM)   { ціле s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); повернути s; }
        якщо (cur.type == T_INPUT) { ціле idx = cur.input; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   повернути inputSlot[idx]; }
        якщо (cur.type == T_LPAREN){ advance(); ціле x = parseTernary(); expect(T_RPAREN, "expected ')'"); повернути x; }
        якщо (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            якщо (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                повернути it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<ціле, 8> args = {}; ціле nargs = 0;
            якщо (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                поки (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (ціле)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            ціле td = typeDepth(name);
            якщо (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                ціле s = args[0];
                повернути isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            ціле arity = 0; TOp op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            якщо (arity == 1) повернути e.emitUnary(op, args[0]);
            якщо (arity == 2) повернути e.emitBinary(op, args[0], args[1]);
            повернути e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    ціле parseUnary()
    {
        якщо (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            ціле operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            якщо (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<double, MAX_LOCAL_CN> v; ціле cn = constDoubles(e, operand, v);
                Scalar neg; для (ціле i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                повернути e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            повернути e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        повернути parsePrimary();
    }

    ціле parseExpr(ціле minPrec)
    {
        ціле left = parseUnary();
        поки (cur.type == T_OP)
        {
            ціле p = binPrec(cur.text);
            якщо (p < minPrec) break;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            ціле right = parseExpr(op == "**" ? p : p + 1);
            left = e.emitBinary(binOp(op), left, right);
        }
        повернути left;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    ціле parseTernary()
    {
        ціле cond = parseExpr(0);
        якщо (!isOp("?")) повернути cond;
        advance();
        ціле thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        ціле elsev = parseTernary();               // right-associative chain
        повернути e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    void parseResult()
    {
        якщо (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); ціле nTemp = e.ntemps;
            advance();
            ціле e0 = parseTernary();
            якщо (cur.type == T_COMMA)
            {
                e.output(e0);
                поки (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                повернути;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    void parse()
    {
        поки (true)
        {
            якщо (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                якщо (cur.type == T_ASSIGN)
                {
                    advance();
                    const ціле slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = true;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    continue;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            якщо (cur.type == T_SEMI) advance();
            break;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

namespace cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
void texpr(const std::string& expr, InputArrayOfArrays _inputs, OutputArrayOfArrays _outputs)
{
    using namespace cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == _InputArray::STD_VECTOR_MAT);
    const std::vector<Mat>& inps = *(const std::vector<Mat>*)_inputs.getObj();
    const ціле ninputs = (ціле)inps.size();
    для (ціле i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<ціле> islot(std::max(ninputs, 1));
    для (ціле i = 0; i < ninputs; i++) islot[i] = e.addInput(inps[i].depth());
    Parser(expr, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    якщо (kind == _InputArray::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } інакше {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
