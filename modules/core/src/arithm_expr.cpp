// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#ضمن "precomp.hpp"
#ضمن "arithm_expr.hpp"
#ضمن <algorithm>
#ضمن <climits>
#ضمن <cmath>
#ضمن <cstdlib>
#ضمن <cstring>
#ضمن <map>
#ضمن <ostream>
#ضمن <string>

نطاق cv { نطاق ew {

// ============================ op metadata (was ew_op.cpp) ============================

ثابت حرف* opName(چوققا op)
{
    بدل (op)
    {
    حالة OP_NOP:           أرجع "nop";
    حالة OP_NEG:           أرجع "neg";
    حالة OP_ABS:           أرجع "abs";
    حالة OP_NOT:           أرجع "not";
    حالة OP_SQRT:          أرجع "sqrt";
    حالة OP_EXP:           أرجع "exp";
    حالة OP_LOG:           أرجع "log";
    حالة OP_SIN:           أرجع "sin";
    حالة OP_COS:           أرجع "cos";
    حالة OP_TANH:          أرجع "tanh";
    حالة OP_ERF:           أرجع "erf";
    حالة OP_RELU:          أرجع "relu";
    حالة OP_CAST:          أرجع "cast";
    حالة OP_ADD:           أرجع "add";
    حالة OP_SUB:           أرجع "sub";
    حالة OP_MUL:           أرجع "mul";
    حالة OP_DIV:           أرجع "div";
    حالة OP_POW:           أرجع "pow";
    حالة OP_MIN:           أرجع "min";
    حالة OP_MAX:           أرجع "max";
    حالة OP_ABSDIFF:       أرجع "absdiff";
    حالة OP_ADDW:          أرجع "addWeighted";
    حالة OP_HYPOT:         أرجع "hypot";
    حالة OP_ATAN2:         أرجع "atan2";
    حالة OP_AND:           أرجع "and";
    حالة OP_OR:            أرجع "or";
    حالة OP_XOR:           أرجع "xor";
    حالة OP_CMP_EQ:        أرجع "cmp_eq";
    حالة OP_CMP_NE:        أرجع "cmp_ne";
    حالة OP_CMP_LT:        أرجع "cmp_lt";
    حالة OP_CMP_LE:        أرجع "cmp_le";
    حالة OP_CMP_GT:        أرجع "cmp_gt";
    حالة OP_CMP_GE:        أرجع "cmp_ge";
    حالة OP_CLAMP:         أرجع "clamp";
    حالة OP_SELECT:        أرجع "select";
    حالة OP_CONVERT_SCALE: أرجع "convert_scale";
    افتراضي:               أرجع "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(چوققا op)
{
    بدل (op)
    {
    حالة OP_AND: حالة OP_OR: حالة OP_XOR: حالة OP_NOT:
        أرجع CAT_BITWISE;
    حالة OP_CMP_EQ: حالة OP_CMP_NE: حالة OP_CMP_LT:
    حالة OP_CMP_LE: حالة OP_CMP_GT: حالة OP_CMP_GE:
        أرجع CAT_COMPARE;
    حالة OP_SQRT: حالة OP_EXP: حالة OP_LOG:
    حالة OP_SIN: حالة OP_COS: حالة OP_TANH: حالة OP_ERF: حالة OP_ATAN2: حالة OP_RELU:
        أرجع CAT_MATH;
    حالة OP_CAST: حالة OP_CONVERT_SCALE:
        أرجع CAT_CAST;
    حالة OP_SELECT:
        أرجع CAT_SELECT;
    افتراضي:
        أرجع CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
ساكن منطقي isFloatDepth(صحيح d)
{
    أرجع d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
ساكن منطقي depthRepresents(مزدوج v, صحيح depth)
{
    إذا (isFloatDepth(depth)) أرجع true;
    إذا (v != std::floor(v)) أرجع خطأ;
    بدل (depth)
    {
    حالة CV_8U:  أرجع v >= 0            && v <= 255;
    حالة CV_8S:  أرجع v >= -128         && v <= 127;
    حالة CV_16U: أرجع v >= 0            && v <= 65535;
    حالة CV_16S: أرجع v >= -32768       && v <= 32767;
    حالة CV_32U: أرجع v >= 0            && v <= 4294967295.0;
    حالة CV_32S: أرجع v >= -2147483648.0 && v <= 2147483647.0;
    حالة CV_64U: أرجع v >= 0            && v <= 18446744073709551615.0;
    حالة CV_64S: أرجع v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    افتراضي:     أرجع خطأ;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
ساكن فراغ intRange(صحيح depth, مزدوج& lo, مزدوج& hi)
{
    بدل (depth)
    {
    حالة CV_8U:  lo = 0;                     hi = 255; اكسر;
    حالة CV_8S:  lo = -128;                  hi = 127; اكسر;
    حالة CV_16U: lo = 0;                     hi = 65535; اكسر;
    حالة CV_16S: lo = -32768;                hi = 32767; اكسر;
    حالة CV_32U: lo = 0;                     hi = 4294967295.0; اكسر;
    حالة CV_32S: lo = -2147483648.0;         hi = 2147483647.0; اكسر;
    حالة CV_64U: lo = 0;                     hi = 18446744073709551615.0; اكسر;
    حالة CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; اكسر;
    افتراضي:     lo = 0;                     hi = 0; اكسر;
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
صحيح promoteArith(صحيح a, صحيح b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    إذا (a == b) أرجع a;

    صحيح wa = صحيح((leszlut >> (a+1)*3) & 7u), wb = صحيح((leszlut >> (b+1)*3) & 7u);
    صحيح fa = صحيح((fltlut >> (a+1)) & 1u),    fb = صحيح((fltlut >> (b+1)) & 1u);

    إذا (fa + fb == 0)                          // both integer
    {
        صحيح sa = صحيح((signlut >> (a+1)) & 1u), sb = صحيح((signlut >> (b+1)) & 1u);
        إذا (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            أرجع wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        صحيح rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr صحيح ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        أرجع (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    صحيح maxw = std::max(wa, wb);
    إذا (maxw == 2 && fa + fb == 1)
        أرجع fa*a + (1 - fa)*b;
    أرجع CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
صحيح absdiffResultDepth(صحيح depth)
{
    بدل (depth)
    {
    حالة CV_8S:  أرجع CV_8U;
    حالة CV_16S: أرجع CV_16U;
    حالة CV_32S: أرجع CV_32U;
    حالة CV_64S: أرجع CV_64U;
    افتراضي:     أرجع depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
ساكن صحيح promote2(صحيح a, صحيح b)
{
    إذا (a == b) أرجع a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ثابت uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ثابت unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    أرجع صحيح((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
ساكن صحيح promoteBitwise(صحيح a, صحيح b)
{
    إذا (a == EW_DEPTH_NONE) أرجع b;
    إذا (b == EW_DEPTH_NONE) أرجع a;
    إذا (a == b) أرجع a;
    أرجع CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
ساكن صحيح safeWide(صحيح depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ثابت uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ثابت unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    أرجع صحيح((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
تعداد { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
ساكن size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, صحيح srcdepth, ثابت فراغ* data, صحيح channels)
{
    ثابت صحيح cn = std::max(1, channels);
    ثابت size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ثابت size_t nu64 = (nbytes + حجم(uint64_t) - 1) / حجم(uint64_t);
    ثابت size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    إذا (data) memcpy(dst, data, nbytes); وإلا memset(dst, 0, nbytes);
    أرجع ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
ساكن صحيح constDoubles(ثابت TExpr& e, صحيح s, AutoBuffer<مزدوج, MAX_LOCAL_CN>& buf)
{
    ثابت TExpr::Arg& a = e.arginfo[s];
    ثابت صحيح cn = std::max(1, a.channels);
    buf.resize(cn);
    ثابت uchar* src = (ثابت uchar*)(e.constbuf.data() + a.constofs);
    إذا (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * حجم(مزدوج));   // common (Scalar / parsed literal)
    وإلا
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    أرجع cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
ساكن مضمن منطقي isFlexConst(ثابت TExpr& e, صحيح s)
{
    أرجع e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
ساكن منطقي scalar_Const_قىممەت(ثابت TExpr& e, صحيح s, مزدوج& v)
{
    ثابت TExpr::Arg& a = e.arginfo[s];
    إذا (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) أرجع خطأ;
    AutoBuffer<مزدوج, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    أرجع true;
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
ساكن منطقي matchScaledChain(ثابت TExpr& e, صحيح end, صحيح t,
                             صحيح& arrSlot, مزدوج& scale, صحيح& startIdx)
{
    إذا (end < 0) أرجع خطأ;
    صحيح j = end, mulResult = t;

    // optional trailing narrowing cast
    إذا (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        إذا (--j < 0) أرجع خطأ;
    }

    ثابت TExpr::Insn& mul = e.prog[j];
    إذا (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) أرجع خطأ;

    صحيح arr;
    إذا      (scalar_Const_قىممەت(e, mul.arg1, scale)) arr = mul.arg0;
    وإلا إذا (scalar_Const_قىممەت(e, mul.arg0, scale)) arr = mul.arg1;
    وإلا أرجع خطأ;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    إذا (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    إذا (e.arginfo[arr].kind == TExpr::CONST) أرجع خطأ;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    أرجع true;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
ساكن منطقي constFits(ثابت TExpr& e, صحيح s, صحيح d)
{
    إذا (!isFlexConst(e, s)) أرجع true;
    AutoBuffer<مزدوج, MAX_LOCAL_CN> v;
    صحيح cn = constDoubles(e, s, v);
    لكل (صحيح ch = 0; ch < cn; ch++)
        إذا (!depthRepresents(v[ch], d)) أرجع خطأ;
    أرجع true;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
صحيح TExpr::emit_ئىككىلىك(چوققا op, صحيح a, صحيح b, صحيح rdepth, ثابت Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    إذا (op == OP_ADDW)
    {
        صحيح Tt = arginfo[a].depth;
        إذا (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        إذا (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        إذا (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        صحيح outD = rdepth;
        إذا (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ثابت صحيح out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        إذا (outD == rdepth) أرجع out;
        ثابت صحيح out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        أرجع out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    إذا (op == OP_ADD)
    {
        ثابت صحيح n = (صحيح)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        صحيح x = 0, y = 0, startA = 0, startB = 0;
        مزدوج alpha = 0, beta = 0;
        إذا (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*هذا, n - 1, b, y, beta, startB) &&
            matchScaledChain(*هذا, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            صحيح retired = 0, lowest = INT_MAX;
            لكل (صحيح i = startA; i < n; i++)
            {
                ثابت صحيح r = prog[i].result;
                إذا (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; اكسر; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            إذا (retired > 0 && lowest == ntemps - retired)
            {
                لكل (صحيح i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                أرجع emit_ئىككىلىك(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            صحيح acc = 0; مزدوج gamma = 0;
            إذا      (scalar_Const_قىممەت(*هذا, b, gamma)) acc = a;
            وإلا إذا (scalar_Const_قىممەت(*هذا, a, gamma)) acc = b;
            إذا (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                أرجع acc;
            }
        }
    }

    ثابت صحيح nd0 = isFlexConst(*هذا, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ثابت صحيح nd1 = isFlexConst(*هذا, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    ثابت ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    صحيح result = rdepth;
    إذا (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        إذا (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    صحيح base;
    إذا (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    وإلا إذا (nd0 == EW_DEPTH_NONE) base = nd1;
    وإلا إذا (nd1 == EW_DEPTH_NONE) base = nd0;
    وإلا base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    إذا (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*هذا, b) && !constFits(*هذا, b, base))
    {
        مزدوج lo, hi; intRange(nd0, lo, hi);
        إذا (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            تعداد { REAL, CFALSE, CTRUE };
            AutoBuffer<مزدوج, MAX_LOCAL_CN> tv; صحيح cn = constDoubles(*هذا, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            صحيح kind[4] = {}; مزدوج bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ثابت چوققا fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            لكل (صحيح c = 0; c < cn; c++)
            {
                ثابت مزدوج t = tv[c]; صحيح k; مزدوج B = t;
                بدل (op)
                {
                حالة OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; اكسر;
                حالة OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; اكسر;
                حالة OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; اكسر;
                حالة OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; اكسر;
                حالة OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; اكسر;   // a==non-rep -> false
                افتراضي:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  اكسر;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](صحيح c){ أرجع kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](صحيح c){ أرجع kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ثابت چوققا u0 = ucOp(0);
            منطقي uniform = true;
            لكل (صحيح c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            إذا (uniform)
            {
                Scalar s; لكل (صحيح c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            وإلا
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; صحيح patchFlags = 0;
                لكل (صحيح c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ثابت صحيح mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ثابت صحيح vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ثابت صحيح out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                أرجع out;
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
    صحيح depth, wide;
    منطقي divGuard = خطأ;
    بدل (op)
    {
    حالة OP_MIN: حالة OP_MAX:
    حالة OP_AND: حالة OP_OR: حالة OP_XOR:
        depth = result; wide = result; اكسر;
    حالة OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        صحيح common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        إذا (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        إذا (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        اكسر;
    }
    حالة OP_CMP_EQ: حالة OP_CMP_NE: حالة OP_CMP_LT:
    حالة OP_CMP_LE: حالة OP_CMP_GT: حالة OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; اكسر;
    حالة OP_MUL: حالة OP_DIV:
    {
        depth = base;
        ثابت منطقي w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        إذا (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](صحيح s, صحيح nd) {
                إذا (nd != EW_DEPTH_NONE) أرجع !isFloatDepth(nd);
                AutoBuffer<مزدوج, MAX_LOCAL_CN> v; constDoubles(*هذا, s, v);
                أرجع v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        اكسر;
    }
    حالة OP_POW: حالة OP_HYPOT: حالة OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; اكسر;
    افتراضي:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); اكسر;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](صحيح s) {
        إذا (!isFlexConst(*هذا, s)) أرجع خطأ;
        AutoBuffer<مزدوج, MAX_LOCAL_CN> v; صحيح n = constDoubles(*هذا, s, v);
        لكل (صحيح ch = 0; ch < n; ch++)
            إذا (v[ch] != std::floor(v[ch])) أرجع true;
        أرجع خطأ;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    إذا (cat != CAT_BITWISE && (!constFits(*هذا, a, depth) || !constFits(*هذا, b, depth)))
    {
        depth = wide;
        ثابت منطقي frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        إذا (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        إذا (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    صحيح s0 = isFlexConst(*هذا, a) ? typedConstFrom(a, depth) : a;
    صحيح s1 = isFlexConst(*هذا, b) ? typedConstFrom(b, depth) : b;
    صحيح c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](صحيح rd) {
        أرجع op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    إذا (k.fptr)
    {
        صحيح res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        أرجع res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحيح w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    أرجع maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
صحيح TExpr::emitUnary(چوققا op, صحيح a, صحيح rdepth, ثابت Scalar& params)
{
    ثابت صحيح nd = isFlexConst(*هذا, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ثابت ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    إذا (op == OP_NEG)
        أرجع emit_ئىككىلىك(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    إذا (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        إذا (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            ثابت صحيح x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            أرجع emit_ئىككىلىك(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        أرجع emit_ئىككىلىك(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    صحيح result = rdepth;
    إذا (result == EW_DEPTH_NONE)
    {
        بدل (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        حالة CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; اكسر;
        حالة CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; اكسر;  // NOT
        افتراضي:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; اكسر;  // NEG, ABS
        }
    }

    صحيح depth, wide;
    بدل (cat)
    {
    حالة CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        اكسر;
    حالة CAT_BITWISE: depth = result; wide = result; اكسر;
    افتراضي:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; اكسر;  // NEG/ABS
    }

    إذا (!constFits(*هذا, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    صحيح s0 = isFlexConst(*هذا, a) ? typedConstFrom(a, depth) : a;
    صحيح c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    إذا (k.fptr)
    {
        صحيح res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        أرجع res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحيح w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    أرجع maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
صحيح TExpr::emitTernary(چوققا op, صحيح a, صحيح b, صحيح c, صحيح rdepth)
{
    auto dep = [&](صحيح s) { أرجع isFlexConst(*هذا, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    إذا (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        إذا (isFlexConst(*هذا, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_ئىككىلىك(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        صحيح result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        إذا (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        صحيح sb = isFlexConst(*هذا, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        صحيح sc = isFlexConst(*هذا, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        صحيح res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        أرجع res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    صحيح result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    إذا (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](صحيح s) {
        أرجع isFlexConst(*هذا, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    صحيح c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحيح res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    أرجع res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
صحيح TExpr::moveToOutput(صحيح temp, صحيح out)
{
    ثابت صحيح ninsn = (صحيح)prog.size();
    صحيح producer = -1;
    منطقي usedAsArg = خطأ;
    لكل (صحيح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        إذا (ins.result == temp) producer = i;
        إذا (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = true;
    }
    إذا (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        أرجع out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    أرجع out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
صحيح TExpr::output(صحيح يىلتىز_Slot)
{
    أرجع moveToOutput(يىلتىز_Slot, addOutput(arginfo[يىلتىز_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
فراغ TExpr::compile()
{
    ثابت صحيح ninsn = (صحيح)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ثابت صحيح nslots = (صحيح)arginfo.size();
    nconsts = 0;
    لكل (صحيح s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        إذا (a.kind != CONST || a.depth == EW_DEPTH_NONE) استمر;
        ثابت صحيح cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        ثابت size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, حجم(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)حجم(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (ثابت uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        ثابت size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        إذا (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        وإلا getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
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
    إذا (ntemps == 0) أرجع;

    AutoBuffer<صحيح, 32> tempOfSlot(nslots);
    لكل (صحيح s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    لكل (صحيح s = 1; s < nslots; s++)
        إذا (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<صحيح, 16> lastUse(ntemps);
    لكل (صحيح t = 0; t < ntemps; t++) lastUse[t] = -1;
    لكل (صحيح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        صحيح as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        لكل (صحيح k = 0; k < 3; k++)
            إذا (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    لكل (صحيح t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<صحيح, 16> freeBufs(ntemps);
    صحيح nfree = 0, nbuf = 0;
    لكل (صحيح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        صحيح rt = tempOfSlot[ins.result];
        إذا (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        صحيح as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        لكل (صحيح k = 0; k < 3; k++)
        {
            صحيح t = tempOfSlot[as[k]];
            إذا (t >= 0 && lastUse[t] == i)
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
    لكل (صحيح b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    لكل (صحيح s = 1; s < nslots; s++)
        إذا (arginfo[s].kind == TEMP)
        {
            صحيح b = bufferOfTemp[arginfo[s].index], e = (صحيح)CV_ELEM_SIZE1(arginfo[s].depth);
            إذا (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    لكل (صحيح b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ثابت صحيح totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
ساكن فراغ matLogical(ثابت Mat& m, MatShape& shp, EwSteps& step, صحيح& esz1)
{
    esz1 = (صحيح)m.elemSize1();
    صحيح nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    لكل (صحيح i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
ساكن منطقي broadcastShape(ثابت MatShape* shps, صحيح K, MatShape& out)
{
    صحيح nd = 0;
    لكل (صحيح k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    لكل (صحيح k = 0; k < K; k++)
    {
        ثابت MatShape& s = shps[k];
        صحيح off = nd - s.dims;
        لكل (صحيح i = 0; i < s.dims; i++)
        {
            صحيح d = s[i], &o = out[off + i];
            إذا (o == 1) o = d;
            وإلا إذا (d != 1 && d != o) أرجع خطأ;
        }
    }
    أرجع true;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
ساكن صحيح opCost(چوققا op)
{
    بدل (op)
    {
    حالة OP_ADD: حالة OP_SUB: حالة OP_MUL: حالة OP_MIN: حالة OP_MAX:
    حالة OP_ABSDIFF: حالة OP_AND: حالة OP_OR: حالة OP_XOR: حالة OP_NOT:
    حالة OP_NEG: حالة OP_ABS: حالة OP_CAST: حالة OP_RELU: حالة OP_SELECT:
    حالة OP_CMP_EQ: حالة OP_CMP_NE: حالة OP_CMP_LT:
    حالة OP_CMP_LE: حالة OP_CMP_GT: حالة OP_CMP_GE: أرجع 1;
    حالة OP_DIV: حالة OP_SQRT: حالة OP_HYPOT: حالة OP_CONVERT_SCALE: أرجع 3;
    حالة OP_SIN: حالة OP_COS: حالة OP_TANH: حالة OP_ERF: حالة OP_ATAN2:
    حالة OP_EXP: حالة OP_LOG: حالة OP_POW:           أرجع 6;
    افتراضي:                                         أرجع 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
فراغ TExpr::clear()
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
فراغ TExpr::dump(std::ostream& os) ثابت
{
    auto dn = [](صحيح d) -> ثابت حرف* {
        بدل (d) {
        حالة EW_DEPTH_NONE: أرجع "flex";
        حالة CV_8U:  أرجع "u8";  حالة CV_8S:  أرجع "s8";
        حالة CV_16U: أرجع "u16"; حالة CV_16S: أرجع "s16";
        حالة CV_32U: أرجع "u32"; حالة CV_32S: أرجع "s32";
        حالة CV_64U: أرجع "u64"; حالة CV_64S: أرجع "s64";
        حالة CV_16F: أرجع "f16"; حالة CV_16BF:أرجع "bf16";
        حالة CV_32F: أرجع "f32"; حالة CV_64F: أرجع "f64";
        حالة CV_Bool:أرجع "bool";
        افتراضي:     أرجع "?"; }
    };
    auto kn = [](ArgKind k) -> ثابت حرف* {
        بدل (k) { حالة NONE: أرجع "none"; حالة كىرگۈزۈش: أرجع "in"; حالة CONST: أرجع "const";
                     حالة TEMP: أرجع "temp"; حالة OUTPUT: أرجع "out"; } أرجع "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (صحيح)prog.size() << "\n";
    os << "  slots:\n";
    لكل (صحيح s = 0; s < (صحيح)arginfo.size(); s++) {
        ثابت Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        إذا (a.kind != NONE) os << " " << dn(a.depth);
        إذا (a.kind == كىرگۈزۈش || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        إذا (a.kind == CONST) {
            ثابت صحيح cn = std::max(1, a.channels);
            AutoBuffer<مزدوج, MAX_LOCAL_CN> v(cn);
            ثابت uchar* src = (ثابت uchar*)(constbuf.data() + a.constofs);
            إذا (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * حجم(مزدوج));
            وإلا getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            لكل (صحيح c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    لكل (صحيح i = 0; i < (صحيح)prog.size(); i++) {
        ثابت Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        إذا (ins.arg1) os << ", " << ins.arg1;
        إذا (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        إذا (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        إذا (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        إذا (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
صحيح TExpr::add_كىرگۈزۈش(صحيح depth)
{
    Arg a; a.kind = كىرگۈزۈش; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

صحيح TExpr::addOutput(صحيح depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

صحيح TExpr::addTemp(صحيح depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
صحيح TExpr::addConst(صحيح depth, ثابت Scalar& v, صحيح channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
صحيح TExpr::addConst(صحيح depth, صحيح srcdepth, ثابت فراغ* data, صحيح channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
صحيح TExpr::typedConstFrom(صحيح srcSlot, صحيح depth)
{
    ثابت Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
صحيح TExpr::addInsn(چوققا op, صحيح a0, صحيح a1, صحيح a2, صحيح result, ثابت TKernel& kernel, ثابت Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    أرجع (صحيح)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
صحيح TExpr::addInsn(چوققا op, صحيح a0, صحيح a1, صحيح a2, صحيح result, ثابت Scalar& params)
{
    صحيح d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    صحيح d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    صحيح d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    أرجع addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
صحيح TExpr::maybeAddCast(صحيح arg, صحيح depth)
{
    إذا (arginfo[arg].depth == depth) أرجع arg;
    صحيح t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    أرجع t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
ساكن مضمن فراغ runInsn(ثابت TExpr::Insn& ins,
                           ثابت فراغ* p0, size_t y0, size_t x0,
                           ثابت فراغ* p1, size_t y1, size_t x1,
                           ثابت فراغ* p2, size_t y2, size_t x2,
                           فراغ* pr, size_t yr, صحيح w, صحيح h)
{
    صحيح code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

بنية EwBody {
    ثابت TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ثابت صحيح* slotMap;
    ثابت signed حرف* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ثابت صحيح* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    صحيح ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

فراغ TExpr::outputShape(ثابت Mat* ثابت* inputs, MatShape& spatial, صحيح& channels) ثابت
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ثابت صحيح rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    منطقي sameShape = ninputs >= 1;
    لكل (صحيح i = 1; sameShape && i < ninputs; i++)
    {
        ثابت Mat& a = *inputs[i];
        إذا (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = خطأ;
        وإلا لكل (صحيح d = 0; d < a.dims; d++) إذا (a.size[d] != inputs[0]->size[d]) { sameShape = خطأ; اكسر; }
    }
    إذا (sameShape) { spatial = inputs[0]->size; channels = rcn0; أرجع; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; صحيح esz1;
    لكل (صحيح i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    ثابت صحيح ndFull = (صحيح)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
فراغ TExpr::outputShape(ثابت Mat* inputs, MatShape& spatial, صحيح& channels) ثابت
{
    AutoBuffer<ثابت Mat*, 16> ptrs(std::max(ninputs, 1));
    لكل (صحيح i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ساكن constexpr صحيح LOCAL_HDRS = 3;
ساكن constexpr صحيح LOCAL_CONSTS = 4;
ساكن constexpr صحيح LOCAL_OPS = 16;

فراغ TExpr::exec(ثابت Mat* ثابت* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    ثابت صحيح nslots = (صحيح)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    إذا (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        ثابت Mat& r = *inputs[0];
        ثابت صحيح rcn = r.channels();
        منطقي ok = r.isContinuous();
        لكل (صحيح i = 1; ok && i < ninputs; i++)
        {
            ثابت Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        لكل (صحيح s = 1; ok && s < nslots; s++)
            إذا (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        ثابت size_t total = ok ? r.total() * rcn : 0;
        إذا (ok && total == 0)
        {
            ثابت MatShape rshape = r.size;
            لكل (صحيح s = 1; s < nslots; s++)
                إذا (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            أرجع;
        }
        إذا (ok && total <= EW_FASTPATH_MAX)
        {
            ثابت MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            لكل (صحيح s = 1; s < nslots; s++)
                إذا (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ثابت صحيح ninsn = (صحيح)prog.size();

            إذا (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](صحيح s) -> فراغ* {
                    إذا (s <= 0) أرجع nullptr;
                    ثابت Arg& ai = arginfo[s];
                    إذا (ai.kind == كىرگۈزۈش)  أرجع (فراغ*)inputs[ai.index]->data;
                    إذا (ai.kind == OUTPUT) أرجع (فراغ*)outputs[ai.index].data;
                    أرجع nullptr;   // NONE
                };
                ثابت صحيح w = (صحيح)total;
                لكل (صحيح n = 0; n < ninsn; n++)
                {
                    ثابت Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                أرجع;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ثابت صحيح totalEsz = bufEszPrefix[nbuffers];
            ثابت صحيح wf0 = std::min((صحيح)total, capElems);

            ثابت size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/حجم(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)حجم(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            لكل (صحيح x0 = 0; x0 < (صحيح)total; x0 += wf0)
            {
                ثابت صحيح wf = std::min(wf0, (صحيح)total - x0);
                auto ptrOf = [&](صحيح s) -> فراغ* {
                    إذا (s <= 0) أرجع nullptr;
                    ثابت Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    إذا (ai.kind == كىرگۈزۈش)  أرجع (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    إذا (ai.kind == OUTPUT) أرجع (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    إذا (ai.kind != TEMP)   أرجع nullptr;                                    // NONE: moved-from
                    ثابت صحيح b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    أرجع scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                لكل (صحيح n = 0; n < ninsn; n++)
                {
                    ثابت Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            أرجع;
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
    صحيح rchannels;
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
    منطقي inplace = خطأ;
    لكل (صحيح i = 0; i < ninputs && !inplace; i++)
        لكل (صحيح j = 0; j < noutputs; j++)
            إذا (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = true; اكسر; }
    صحيح nhdrs = inplace ? ninputs : 0;

    AutoBuffer<ثابت Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<ثابت Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<صحيح, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed حرف, LOCAL_OPS> slotKind(nslots);
    صحيح narr = 0, nc = 0;
    إذا (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        لكل (صحيح j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed حرف)TExpr::NONE;
    لكل (صحيح s = 1; s < nslots; s++)
    {
        ثابت TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed حرف)ai.kind;
        إذا (ai.kind == TExpr::كىرگۈزۈش) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        وإلا إذا (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        وإلا إذا (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed حرف)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        وإلا إذا (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ثابت صحيح c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (فراغ*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed حرف)TExpr::كىرگۈزۈش;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        وإلا إذا (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
        }
        وإلا {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ثابت صحيح ninsn = (صحيح)prog.size();
    long long costPerElem = 0;
    لكل (صحيح n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    لكل (صحيح d = 0; d < (صحيح)spatial.size(); d++) otot *= spatial[d];
    إذا (otot == 0)
        أرجع;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ثابت مزدوج T = (مزدوج)std::max(get_رەقەم_Threads(), 1);
    ثابت مزدوج nstripes = std::min(
        (مزدوج)otot * (مزدوج)std::max<long long>(costPerElem, 1) * (1./ (مزدوج)(1 << 16)),
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

    broadcastOp(arr.data(), narr, [&](ثابت BrTile& tile)
    {
        EwBody& bc = body;
        ثابت TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ثابت صحيح* ثابت smap  = bc.slotMap;
        ثابت signed حرف* ثابت skind = bc.slotKind;
        ثابت صحيح nin = bc.ninsn, nsl = bc.nslots;
        ثابت صحيح w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ثابت صحيح* ثابت eszPrefix = bc.bufEszPrefix;
        ثابت صحيح bw = std::min(w, bc.capElems);
        ثابت صحيح bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ثابت size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/حجم(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)حجم(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        لكل (صحيح y0 = 0; y0 < h; y0 += bh)
        {
            ثابت صحيح hf = std::min(bh, h - y0);
            لكل (صحيح x0 = 0; x0 < w; x0 += bw)
            {
                ثابت صحيح wf = std::min(bw, w - x0);
                لكل (صحيح s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    ثابت صحيح k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    إذا (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    وإلا                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ثابت BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                لكل (صحيح n = 0; n < nin; n++)
                {
                    ثابت TExpr::Insn& ins = insns[n];
                    ثابت BrSlice& a0 = args[ins.arg0]; ثابت BrSlice& a1 = args[ins.arg1];
                    ثابت BrSlice& a2 = args[ins.arg2]; ثابت BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (فراغ*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, true, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
فراغ TExpr::exec(ثابت Mat* inputs, Mat* outputs)
{
    AutoBuffer<ثابت Mat*, 16> ptrs(std::max(ninputs, 1));
    لكل (صحيح i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
فراغ make_ئىككىلىك_Arith_Program(TExpr& p, چوققا op, صحيح depth0, صحيح depth1, صحيح rdepth,
                            صحيح maskDepth, مزدوج scale)
{
    p.clear();
    إذا (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        إذا (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        وإلا
        {
            rdepth = promoteArith(depth0, depth1);
            إذا (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ثابت منطقي masked = maskDepth != EW_DEPTH_NONE;
    صحيح sIn0  = p.add_كىرگۈزۈش(depth0);
    صحيح sIn1  = p.add_كىرگۈزۈش(depth1);
    صحيح sMask = masked ? p.add_كىرگۈزۈش(maskDepth) : 0;
    صحيح sOut  = p.addOutput(rdepth);

    صحيح r = p.emit_ئىككىلىك(op, sIn0, sIn1, rdepth, Scalar(scale));
    إذا (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    وإلا
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
فراغ makeAddWeightedProgram(TExpr& p, صحيح depth0, صحيح depth1, صحيح rdepth,
                            مزدوج alpha, مزدوج beta, مزدوج gamma)
{
    p.clear();
    صحيح sA   = p.add_كىرگۈزۈش(depth0);
    صحيح sB   = p.add_كىرگۈزۈش(depth1);
    صحيح sOut = p.addOutput(rdepth);
    صحيح r = p.emit_ئىككىلىك(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

نطاق {

// --- tokens ------------------------------------------------------------------------------
تعداد TokType { T_رەقەم, T_كىرگۈزۈش, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

بنية Token
{
    TokType type = T_END;
    مزدوج num = 0;
    صحيح كىرگۈزۈش_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
بنية Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    ساكن منطقي isIdentStart(حرف c) { أرجع std::isalpha((unsigned حرف)c) || c == '_'; }
    ساكن منطقي isIdentChar(حرف c)  { أرجع std::isalnum((unsigned حرف)c) || c == '_'; }

    Token next()
    {
        طالما (pos < s.size() && std::isspace((unsigned حرف)s[pos])) pos++;
        Token t;
        إذا (pos >= s.size()) { t.type = T_END; أرجع t; }

        حرف c = s[pos];

        إذا (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            طالما (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_كىرگۈزۈش;
            t.كىرگۈزۈش_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            أرجع t;
        }
        إذا (std::isdigit((unsigned حرف)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned حرف)s[pos + 1])))
        {
            حرف* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_رەقەم;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            أرجع t;
        }
        إذا (isIdentStart(c))
        {
            size_t start = pos;
            طالما (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            أرجع t;
        }
        بدل (c)
        {
        حالة '(': pos++; t.type = T_LPAREN; أرجع t;
        حالة ')': pos++; t.type = T_RPAREN; أرجع t;
        حالة ',': pos++; t.type = T_COMMA;  أرجع t;
        حالة ';': pos++; t.type = T_SEMI;   أرجع t;
        }
        // multi/!single-char operators
        auto two = [&](ثابت حرف* op) {
            أرجع pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        إذا (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; أرجع t; }
        إذا (c == '=') { pos++; t.type = T_ASSIGN; أرجع t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        أرجع t;
    }
};

// --- operator / function tables ----------------------------------------------------------
ساكن صحيح binPrec(ثابت std::string& op)
{
    إذا (op == "**") أرجع 8;              // power binds tighter than '*'; RIGHT-associative
    إذا (op == "*" || op == "/") أرجع 7;
    إذا (op == "+" || op == "-") أرجع 6;
    إذا (op == "<" || op == "<=" || op == ">" || op == ">=") أرجع 5;
    إذا (op == "==" || op == "!=") أرجع 4;
    إذا (op == "&") أرجع 3;
    إذا (op == "^") أرجع 2;
    إذا (op == "|") أرجع 1;
    أرجع -1;
}

ساكن چوققا binOp(ثابت std::string& op)
{
    إذا (op == "**") أرجع OP_POW;
    إذا (op == "+")  أرجع OP_ADD;
    إذا (op == "-")  أرجع OP_SUB;
    إذا (op == "*")  أرجع OP_MUL;
    إذا (op == "/")  أرجع OP_DIV;
    إذا (op == "<")  أرجع OP_CMP_LT;
    إذا (op == "<=") أرجع OP_CMP_LE;
    إذا (op == ">")  أرجع OP_CMP_GT;
    إذا (op == ">=") أرجع OP_CMP_GE;
    إذا (op == "==") أرجع OP_CMP_EQ;
    إذا (op == "!=") أرجع OP_CMP_NE;
    إذا (op == "&")  أرجع OP_AND;
    إذا (op == "|")  أرجع OP_OR;
    إذا (op == "^")  أرجع OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
ساكن صحيح typeDepth(ثابت std::string& name)
{
    إذا (name == "float")    أرجع CV_32F;
    إذا (name == "double")   أرجع CV_64F;
    إذا (name == "half" || name == "float16") أرجع CV_16F;
    إذا (name == "bfloat16") أرجع CV_16BF;
    إذا (name == "uint8")    أرجع CV_8U;
    إذا (name == "int8")     أرجع CV_8S;
    إذا (name == "uint16")   أرجع CV_16U;
    إذا (name == "int16")    أرجع CV_16S;
    إذا (name == "uint32")   أرجع CV_32U;
    إذا (name == "int32")    أرجع CV_32S;
    إذا (name == "uint64")   أرجع CV_64U;
    إذا (name == "int64")    أرجع CV_64S;
    أرجع -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
ساكن چوققا fnOp(ثابت std::string& name, صحيح& arity)
{
    بنية E { ثابت حرف* n; چوققا op; };
    ساكن ثابت E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    ساكن ثابت E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    ساكن ثابت E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    لكل (ثابت E& e : unary)  إذا (name == e.n) { arity = 1; أرجع e.op; }
    لكل (ثابت E& e : binary) إذا (name == e.n) { arity = 2; أرجع e.op; }
    لكل (ثابت E& e : tern)   إذا (name == e.n) { arity = 3; أرجع e.op; }
    arity = 0; أرجع OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
بنية Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ثابت صحيح* كىرگۈزۈش_Slot;                 // slot id of each external input (precreated)
    صحيح ninputs;
    std::map<std::string, صحيح> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& ئىپادە, ثابت صحيح* islot, صحيح nin)
        : lex(src), e(ئىپادە), كىرگۈزۈش_Slot(islot), ninputs(nin) { cur = lex.next(); }

    فراغ advance() { cur = lex.next(); }
    منطقي isOp(ثابت حرف* op) ثابت { أرجع cur.type == T_OP && cur.text == op; }

    فراغ expect(TokType t, ثابت حرف* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    منطقي isFlexConst(صحيح slot) ثابت
    {
        أرجع e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    صحيح parsePrimary()
    {
        إذا (cur.type == T_رەقەم)   { صحيح s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); أرجع s; }
        إذا (cur.type == T_كىرگۈزۈش) { صحيح idx = cur.كىرگۈزۈش_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   أرجع كىرگۈزۈش_Slot[idx]; }
        إذا (cur.type == T_LPAREN){ advance(); صحيح x = parseTernary(); expect(T_RPAREN, "expected ')'"); أرجع x; }
        إذا (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            إذا (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                أرجع it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<صحيح, 8> args = {}; صحيح nargs = 0;
            إذا (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                طالما (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (صحيح)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            صحيح td = typeDepth(name);
            إذا (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                صحيح s = args[0];
                أرجع isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            صحيح arity = 0; چوققا op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            إذا (arity == 1) أرجع e.emitUnary(op, args[0]);
            إذا (arity == 2) أرجع e.emit_ئىككىلىك(op, args[0], args[1]);
            أرجع e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    صحيح parseUnary()
    {
        إذا (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            صحيح operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            إذا (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<مزدوج, MAX_LOCAL_CN> v; صحيح cn = constDoubles(e, operand, v);
                Scalar neg; لكل (صحيح i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                أرجع e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            أرجع e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        أرجع parsePrimary();
    }

    صحيح parse_ئىپادە(صحيح minPrec)
    {
        صحيح سول = parseUnary();
        طالما (cur.type == T_OP)
        {
            صحيح p = binPrec(cur.text);
            إذا (p < minPrec) اكسر;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            صحيح ئوڭ = parse_ئىپادە(op == "**" ? p : p + 1);
            سول = e.emit_ئىككىلىك(binOp(op), سول, ئوڭ);
        }
        أرجع سول;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    صحيح parseTernary()
    {
        صحيح cond = parse_ئىپادە(0);
        إذا (!isOp("?")) أرجع cond;
        advance();
        صحيح thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        صحيح elsev = parseTernary();               // right-associative chain
        أرجع e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    فراغ parseResult()
    {
        إذا (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); صحيح nTemp = e.ntemps;
            advance();
            صحيح e0 = parseTernary();
            إذا (cur.type == T_COMMA)
            {
                e.output(e0);
                طالما (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                أرجع;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    فراغ parse()
    {
        طالما (true)
        {
            إذا (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                إذا (cur.type == T_ASSIGN)
                {
                    advance();
                    ثابت صحيح slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = true;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    استمر;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            إذا (cur.type == T_SEMI) advance();
            اكسر;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

نطاق cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
فراغ texpr(ثابت std::string& ئىپادە, كىرگۈزۈش_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using نطاق cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == كىرگۈزۈش_Array_2::STD_VECTOR_MAT);
    ثابت std::vector<Mat>& inps = *(ثابت std::vector<Mat>*)_inputs.getObj();
    ثابت صحيح ninputs = (صحيح)inps.size();
    لكل (صحيح i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<صحيح> islot(std::max(ninputs, 1));
    لكل (صحيح i = 0; i < ninputs; i++) islot[i] = e.add_كىرگۈزۈش(inps[i].depth());
    Parser(ئىپادە, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    إذا (kind == كىرگۈزۈش_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } وإلا {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
