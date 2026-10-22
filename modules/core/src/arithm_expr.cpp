// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#شامل "precomp.hpp"
#شامل "arithm_expr.hpp"
#شامل <algorithm>
#شامل <climits>
#شامل <cmath>
#شامل <cstdlib>
#شامل <cstring>
#شامل <map>
#شامل <ostream>
#شامل <string>

فضاینام cv { فضاینام ew {

// ============================ op metadata (was ew_op.cpp) ============================

ثابت نویسه* opName(بالاترین op)
{
    انتخاب (op)
    {
    حالت OP_NOP:           بازگشت "nop";
    حالت OP_NEG:           بازگشت "neg";
    حالت OP_ABS:           بازگشت "abs";
    حالت OP_NOT:           بازگشت "not";
    حالت OP_SQRT:          بازگشت "sqrt";
    حالت OP_EXP:           بازگشت "exp";
    حالت OP_LOG:           بازگشت "log";
    حالت OP_SIN:           بازگشت "sin";
    حالت OP_COS:           بازگشت "cos";
    حالت OP_TANH:          بازگشت "tanh";
    حالت OP_ERF:           بازگشت "erf";
    حالت OP_RELU:          بازگشت "relu";
    حالت OP_CAST:          بازگشت "cast";
    حالت OP_ADD:           بازگشت "add";
    حالت OP_SUB:           بازگشت "sub";
    حالت OP_MUL:           بازگشت "mul";
    حالت OP_DIV:           بازگشت "div";
    حالت OP_POW:           بازگشت "pow";
    حالت OP_MIN:           بازگشت "min";
    حالت OP_MAX:           بازگشت "max";
    حالت OP_ABSDIFF:       بازگشت "absdiff";
    حالت OP_ADDW:          بازگشت "addWeighted";
    حالت OP_HYPOT:         بازگشت "hypot";
    حالت OP_ATAN2:         بازگشت "atan2";
    حالت OP_AND:           بازگشت "and";
    حالت OP_OR:            بازگشت "or";
    حالت OP_XOR:           بازگشت "xor";
    حالت OP_CMP_EQ:        بازگشت "cmp_eq";
    حالت OP_CMP_NE:        بازگشت "cmp_ne";
    حالت OP_CMP_LT:        بازگشت "cmp_lt";
    حالت OP_CMP_LE:        بازگشت "cmp_le";
    حالت OP_CMP_GT:        بازگشت "cmp_gt";
    حالت OP_CMP_GE:        بازگشت "cmp_ge";
    حالت OP_CLAMP:         بازگشت "clamp";
    حالت OP_SELECT:        بازگشت "select";
    حالت OP_CONVERT_SCALE: بازگشت "convert_scale";
    پیشفرض:               بازگشت "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(بالاترین op)
{
    انتخاب (op)
    {
    حالت OP_AND: حالت OP_OR: حالت OP_XOR: حالت OP_NOT:
        بازگشت CAT_BITWISE;
    حالت OP_CMP_EQ: حالت OP_CMP_NE: حالت OP_CMP_LT:
    حالت OP_CMP_LE: حالت OP_CMP_GT: حالت OP_CMP_GE:
        بازگشت CAT_COMPARE;
    حالت OP_SQRT: حالت OP_EXP: حالت OP_LOG:
    حالت OP_SIN: حالت OP_COS: حالت OP_TANH: حالت OP_ERF: حالت OP_ATAN2: حالت OP_RELU:
        بازگشت CAT_MATH;
    حالت OP_CAST: حالت OP_CONVERT_SCALE:
        بازگشت CAT_CAST;
    حالت OP_SELECT:
        بازگشت CAT_SELECT;
    پیشفرض:
        بازگشت CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
ایستا منطقی isFloatDepth(صحیح d)
{
    بازگشت d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
ایستا منطقی depthRepresents(دوگانه v, صحیح depth)
{
    اگر (isFloatDepth(depth)) بازگشت درست;
    اگر (v != std::floor(v)) بازگشت نادرست;
    انتخاب (depth)
    {
    حالت CV_8U:  بازگشت v >= 0            && v <= 255;
    حالت CV_8S:  بازگشت v >= -128         && v <= 127;
    حالت CV_16U: بازگشت v >= 0            && v <= 65535;
    حالت CV_16S: بازگشت v >= -32768       && v <= 32767;
    حالت CV_32U: بازگشت v >= 0            && v <= 4294967295.0;
    حالت CV_32S: بازگشت v >= -2147483648.0 && v <= 2147483647.0;
    حالت CV_64U: بازگشت v >= 0            && v <= 18446744073709551615.0;
    حالت CV_64S: بازگشت v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    پیشفرض:     بازگشت نادرست;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
ایستا تهی intRange(صحیح depth, دوگانه& lo, دوگانه& hi)
{
    انتخاب (depth)
    {
    حالت CV_8U:  lo = 0;                     hi = 255; بشکن;
    حالت CV_8S:  lo = -128;                  hi = 127; بشکن;
    حالت CV_16U: lo = 0;                     hi = 65535; بشکن;
    حالت CV_16S: lo = -32768;                hi = 32767; بشکن;
    حالت CV_32U: lo = 0;                     hi = 4294967295.0; بشکن;
    حالت CV_32S: lo = -2147483648.0;         hi = 2147483647.0; بشکن;
    حالت CV_64U: lo = 0;                     hi = 18446744073709551615.0; بشکن;
    حالت CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; بشکن;
    پیشفرض:     lo = 0;                     hi = 0; بشکن;
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
صحیح promoteArith(صحیح a, صحیح b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    اگر (a == b) بازگشت a;

    صحیح wa = صحیح((leszlut >> (a+1)*3) & 7u), wb = صحیح((leszlut >> (b+1)*3) & 7u);
    صحیح fa = صحیح((fltlut >> (a+1)) & 1u),    fb = صحیح((fltlut >> (b+1)) & 1u);

    اگر (fa + fb == 0)                          // both integer
    {
        صحیح sa = صحیح((signlut >> (a+1)) & 1u), sb = صحیح((signlut >> (b+1)) & 1u);
        اگر (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            بازگشت wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        صحیح rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr صحیح ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        بازگشت (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    صحیح maxw = std::max(wa, wb);
    اگر (maxw == 2 && fa + fb == 1)
        بازگشت fa*a + (1 - fa)*b;
    بازگشت CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
صحیح absdiffResultDepth(صحیح depth)
{
    انتخاب (depth)
    {
    حالت CV_8S:  بازگشت CV_8U;
    حالت CV_16S: بازگشت CV_16U;
    حالت CV_32S: بازگشت CV_32U;
    حالت CV_64S: بازگشت CV_64U;
    پیشفرض:     بازگشت depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
ایستا صحیح promote2(صحیح a, صحیح b)
{
    اگر (a == b) بازگشت a;

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
    بازگشت صحیح((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
ایستا صحیح promoteBitwise(صحیح a, صحیح b)
{
    اگر (a == EW_DEPTH_NONE) بازگشت b;
    اگر (b == EW_DEPTH_NONE) بازگشت a;
    اگر (a == b) بازگشت a;
    بازگشت CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
ایستا صحیح safeWide(صحیح depth)
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
    بازگشت صحیح((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
شمارش { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
ایستا size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, صحیح srcdepth, ثابت تهی* data, صحیح channels)
{
    ثابت صحیح cn = std::max(1, channels);
    ثابت size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ثابت size_t nu64 = (nbytes + اندازه(uint64_t) - 1) / اندازه(uint64_t);
    ثابت size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    اگر (data) memcpy(dst, data, nbytes); وگرنه memset(dst, 0, nbytes);
    بازگشت ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
ایستا صحیح constDoubles(ثابت TExpr& e, صحیح s, AutoBuffer<دوگانه, MAX_LOCAL_CN>& buf)
{
    ثابت TExpr::Arg& a = e.arginfo[s];
    ثابت صحیح cn = std::max(1, a.channels);
    buf.resize(cn);
    ثابت uchar* src = (ثابت uchar*)(e.constbuf.data() + a.constofs);
    اگر (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * اندازه(دوگانه));   // common (Scalar / parsed literal)
    وگرنه
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    بازگشت cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
ایستا درونخطی منطقی isFlexConst(ثابت TExpr& e, صحیح s)
{
    بازگشت e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
ایستا منطقی scalar_Const_مقدار(ثابت TExpr& e, صحیح s, دوگانه& v)
{
    ثابت TExpr::Arg& a = e.arginfo[s];
    اگر (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) بازگشت نادرست;
    AutoBuffer<دوگانه, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    بازگشت درست;
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
ایستا منطقی matchScaledChain(ثابت TExpr& e, صحیح end, صحیح t,
                             صحیح& arrSlot, دوگانه& scale, صحیح& startIdx)
{
    اگر (end < 0) بازگشت نادرست;
    صحیح j = end, mulResult = t;

    // optional trailing narrowing cast
    اگر (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        اگر (--j < 0) بازگشت نادرست;
    }

    ثابت TExpr::Insn& mul = e.prog[j];
    اگر (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) بازگشت نادرست;

    صحیح arr;
    اگر      (scalar_Const_مقدار(e, mul.arg1, scale)) arr = mul.arg0;
    وگرنه اگر (scalar_Const_مقدار(e, mul.arg0, scale)) arr = mul.arg1;
    وگرنه بازگشت نادرست;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    اگر (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    اگر (e.arginfo[arr].kind == TExpr::CONST) بازگشت نادرست;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    بازگشت درست;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
ایستا منطقی constFits(ثابت TExpr& e, صحیح s, صحیح d)
{
    اگر (!isFlexConst(e, s)) بازگشت درست;
    AutoBuffer<دوگانه, MAX_LOCAL_CN> v;
    صحیح cn = constDoubles(e, s, v);
    برای (صحیح ch = 0; ch < cn; ch++)
        اگر (!depthRepresents(v[ch], d)) بازگشت نادرست;
    بازگشت درست;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
صحیح TExpr::emit_دودویی(بالاترین op, صحیح a, صحیح b, صحیح rdepth, ثابت Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    اگر (op == OP_ADDW)
    {
        صحیح Tt = arginfo[a].depth;
        اگر (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        اگر (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        اگر (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        صحیح outD = rdepth;
        اگر (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ثابت صحیح out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        اگر (outD == rdepth) بازگشت out;
        ثابت صحیح out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        بازگشت out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    اگر (op == OP_ADD)
    {
        ثابت صحیح n = (صحیح)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        صحیح x = 0, y = 0, startA = 0, startB = 0;
        دوگانه alpha = 0, beta = 0;
        اگر (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*این, n - 1, b, y, beta, startB) &&
            matchScaledChain(*این, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            صحیح retired = 0, lowest = INT_MAX;
            برای (صحیح i = startA; i < n; i++)
            {
                ثابت صحیح r = prog[i].result;
                اگر (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; بشکن; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            اگر (retired > 0 && lowest == ntemps - retired)
            {
                برای (صحیح i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                بازگشت emit_دودویی(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            صحیح acc = 0; دوگانه gamma = 0;
            اگر      (scalar_Const_مقدار(*این, b, gamma)) acc = a;
            وگرنه اگر (scalar_Const_مقدار(*این, a, gamma)) acc = b;
            اگر (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                بازگشت acc;
            }
        }
    }

    ثابت صحیح nd0 = isFlexConst(*این, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ثابت صحیح nd1 = isFlexConst(*این, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    ثابت ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    صحیح result = rdepth;
    اگر (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        اگر (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    صحیح base;
    اگر (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    وگرنه اگر (nd0 == EW_DEPTH_NONE) base = nd1;
    وگرنه اگر (nd1 == EW_DEPTH_NONE) base = nd0;
    وگرنه base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    اگر (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*این, b) && !constFits(*این, b, base))
    {
        دوگانه lo, hi; intRange(nd0, lo, hi);
        اگر (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            شمارش { REAL, CFALSE, CTRUE };
            AutoBuffer<دوگانه, MAX_LOCAL_CN> tv; صحیح cn = constDoubles(*این, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            صحیح kind[4] = {}; دوگانه bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ثابت بالاترین fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            برای (صحیح c = 0; c < cn; c++)
            {
                ثابت دوگانه t = tv[c]; صحیح k; دوگانه B = t;
                انتخاب (op)
                {
                حالت OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; بشکن;
                حالت OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; بشکن;
                حالت OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; بشکن;
                حالت OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; بشکن;
                حالت OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; بشکن;   // a==non-rep -> false
                پیشفرض:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  بشکن;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](صحیح c){ بازگشت kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](صحیح c){ بازگشت kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ثابت بالاترین u0 = ucOp(0);
            منطقی uniform = درست;
            برای (صحیح c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            اگر (uniform)
            {
                Scalar s; برای (صحیح c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            وگرنه
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; صحیح patchFlags = 0;
                برای (صحیح c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ثابت صحیح mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ثابت صحیح vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ثابت صحیح out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                بازگشت out;
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
    صحیح depth, wide;
    منطقی divGuard = نادرست;
    انتخاب (op)
    {
    حالت OP_MIN: حالت OP_MAX:
    حالت OP_AND: حالت OP_OR: حالت OP_XOR:
        depth = result; wide = result; بشکن;
    حالت OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        صحیح common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        اگر (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        اگر (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        بشکن;
    }
    حالت OP_CMP_EQ: حالت OP_CMP_NE: حالت OP_CMP_LT:
    حالت OP_CMP_LE: حالت OP_CMP_GT: حالت OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; بشکن;
    حالت OP_MUL: حالت OP_DIV:
    {
        depth = base;
        ثابت منطقی w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        اگر (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](صحیح s, صحیح nd) {
                اگر (nd != EW_DEPTH_NONE) بازگشت !isFloatDepth(nd);
                AutoBuffer<دوگانه, MAX_LOCAL_CN> v; constDoubles(*این, s, v);
                بازگشت v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        بشکن;
    }
    حالت OP_POW: حالت OP_HYPOT: حالت OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; بشکن;
    پیشفرض:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); بشکن;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](صحیح s) {
        اگر (!isFlexConst(*این, s)) بازگشت نادرست;
        AutoBuffer<دوگانه, MAX_LOCAL_CN> v; صحیح n = constDoubles(*این, s, v);
        برای (صحیح ch = 0; ch < n; ch++)
            اگر (v[ch] != std::floor(v[ch])) بازگشت درست;
        بازگشت نادرست;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    اگر (cat != CAT_BITWISE && (!constFits(*این, a, depth) || !constFits(*این, b, depth)))
    {
        depth = wide;
        ثابت منطقی frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        اگر (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        اگر (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    صحیح s0 = isFlexConst(*این, a) ? typedConstFrom(a, depth) : a;
    صحیح s1 = isFlexConst(*این, b) ? typedConstFrom(b, depth) : b;
    صحیح c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](صحیح rd) {
        بازگشت op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    اگر (k.fptr)
    {
        صحیح res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        بازگشت res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحیح w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    بازگشت maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
صحیح TExpr::emitUnary(بالاترین op, صحیح a, صحیح rdepth, ثابت Scalar& params)
{
    ثابت صحیح nd = isFlexConst(*این, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ثابت ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    اگر (op == OP_NEG)
        بازگشت emit_دودویی(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    اگر (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        اگر (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            ثابت صحیح x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            بازگشت emit_دودویی(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        بازگشت emit_دودویی(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    صحیح result = rdepth;
    اگر (result == EW_DEPTH_NONE)
    {
        انتخاب (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        حالت CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; بشکن;
        حالت CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; بشکن;  // NOT
        پیشفرض:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; بشکن;  // NEG, ABS
        }
    }

    صحیح depth, wide;
    انتخاب (cat)
    {
    حالت CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        بشکن;
    حالت CAT_BITWISE: depth = result; wide = result; بشکن;
    پیشفرض:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; بشکن;  // NEG/ABS
    }

    اگر (!constFits(*این, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    صحیح s0 = isFlexConst(*این, a) ? typedConstFrom(a, depth) : a;
    صحیح c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    اگر (k.fptr)
    {
        صحیح res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        بازگشت res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحیح w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    بازگشت maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
صحیح TExpr::emitTernary(بالاترین op, صحیح a, صحیح b, صحیح c, صحیح rdepth)
{
    auto dep = [&](صحیح s) { بازگشت isFlexConst(*این, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    اگر (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        اگر (isFlexConst(*این, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_دودویی(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        صحیح result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        اگر (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        صحیح sb = isFlexConst(*این, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        صحیح sc = isFlexConst(*این, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        صحیح res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        بازگشت res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    صحیح result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    اگر (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](صحیح s) {
        بازگشت isFlexConst(*این, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    صحیح c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحیح res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    بازگشت res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
صحیح TExpr::moveToOutput(صحیح temp, صحیح out)
{
    ثابت صحیح ninsn = (صحیح)prog.size();
    صحیح producer = -1;
    منطقی usedAsArg = نادرست;
    برای (صحیح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        اگر (ins.result == temp) producer = i;
        اگر (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = درست;
    }
    اگر (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        بازگشت out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    بازگشت out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
صحیح TExpr::output(صحیح ریشه_Slot)
{
    بازگشت moveToOutput(ریشه_Slot, addOutput(arginfo[ریشه_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
تهی TExpr::compile()
{
    ثابت صحیح ninsn = (صحیح)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ثابت صحیح nslots = (صحیح)arginfo.size();
    nconsts = 0;
    برای (صحیح s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        اگر (a.kind != CONST || a.depth == EW_DEPTH_NONE) ادامه;
        ثابت صحیح cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        ثابت size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, اندازه(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)اندازه(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (ثابت uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        ثابت size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        اگر (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        وگرنه getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
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
    اگر (ntemps == 0) بازگشت;

    AutoBuffer<صحیح, 32> tempOfSlot(nslots);
    برای (صحیح s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    برای (صحیح s = 1; s < nslots; s++)
        اگر (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<صحیح, 16> lastUse(ntemps);
    برای (صحیح t = 0; t < ntemps; t++) lastUse[t] = -1;
    برای (صحیح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        صحیح as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        برای (صحیح k = 0; k < 3; k++)
            اگر (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    برای (صحیح t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<صحیح, 16> freeBufs(ntemps);
    صحیح nfree = 0, nbuf = 0;
    برای (صحیح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        صحیح rt = tempOfSlot[ins.result];
        اگر (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        صحیح as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        برای (صحیح k = 0; k < 3; k++)
        {
            صحیح t = tempOfSlot[as[k]];
            اگر (t >= 0 && lastUse[t] == i)
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
    برای (صحیح b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    برای (صحیح s = 1; s < nslots; s++)
        اگر (arginfo[s].kind == TEMP)
        {
            صحیح b = bufferOfTemp[arginfo[s].index], e = (صحیح)CV_ELEM_SIZE1(arginfo[s].depth);
            اگر (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    برای (صحیح b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ثابت صحیح totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
ایستا تهی matLogical(ثابت Mat& m, MatShape& shp, EwSteps& step, صحیح& esz1)
{
    esz1 = (صحیح)m.elemSize1();
    صحیح nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    برای (صحیح i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
ایستا منطقی broadcastShape(ثابت MatShape* shps, صحیح K, MatShape& out)
{
    صحیح nd = 0;
    برای (صحیح k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    برای (صحیح k = 0; k < K; k++)
    {
        ثابت MatShape& s = shps[k];
        صحیح off = nd - s.dims;
        برای (صحیح i = 0; i < s.dims; i++)
        {
            صحیح d = s[i], &o = out[off + i];
            اگر (o == 1) o = d;
            وگرنه اگر (d != 1 && d != o) بازگشت نادرست;
        }
    }
    بازگشت درست;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
ایستا صحیح opCost(بالاترین op)
{
    انتخاب (op)
    {
    حالت OP_ADD: حالت OP_SUB: حالت OP_MUL: حالت OP_MIN: حالت OP_MAX:
    حالت OP_ABSDIFF: حالت OP_AND: حالت OP_OR: حالت OP_XOR: حالت OP_NOT:
    حالت OP_NEG: حالت OP_ABS: حالت OP_CAST: حالت OP_RELU: حالت OP_SELECT:
    حالت OP_CMP_EQ: حالت OP_CMP_NE: حالت OP_CMP_LT:
    حالت OP_CMP_LE: حالت OP_CMP_GT: حالت OP_CMP_GE: بازگشت 1;
    حالت OP_DIV: حالت OP_SQRT: حالت OP_HYPOT: حالت OP_CONVERT_SCALE: بازگشت 3;
    حالت OP_SIN: حالت OP_COS: حالت OP_TANH: حالت OP_ERF: حالت OP_ATAN2:
    حالت OP_EXP: حالت OP_LOG: حالت OP_POW:           بازگشت 6;
    پیشفرض:                                         بازگشت 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
تهی TExpr::clear()
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
تهی TExpr::dump(std::ostream& os) ثابت
{
    auto dn = [](صحیح d) -> ثابت نویسه* {
        انتخاب (d) {
        حالت EW_DEPTH_NONE: بازگشت "flex";
        حالت CV_8U:  بازگشت "u8";  حالت CV_8S:  بازگشت "s8";
        حالت CV_16U: بازگشت "u16"; حالت CV_16S: بازگشت "s16";
        حالت CV_32U: بازگشت "u32"; حالت CV_32S: بازگشت "s32";
        حالت CV_64U: بازگشت "u64"; حالت CV_64S: بازگشت "s64";
        حالت CV_16F: بازگشت "f16"; حالت CV_16BF:بازگشت "bf16";
        حالت CV_32F: بازگشت "f32"; حالت CV_64F: بازگشت "f64";
        حالت CV_Bool:بازگشت "bool";
        پیشفرض:     بازگشت "?"; }
    };
    auto kn = [](ArgKind k) -> ثابت نویسه* {
        انتخاب (k) { حالت NONE: بازگشت "none"; حالت درون_داد: بازگشت "in"; حالت CONST: بازگشت "const";
                     حالت TEMP: بازگشت "temp"; حالت OUTPUT: بازگشت "out"; } بازگشت "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (صحیح)prog.size() << "\n";
    os << "  slots:\n";
    برای (صحیح s = 0; s < (صحیح)arginfo.size(); s++) {
        ثابت Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        اگر (a.kind != NONE) os << " " << dn(a.depth);
        اگر (a.kind == درون_داد || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        اگر (a.kind == CONST) {
            ثابت صحیح cn = std::max(1, a.channels);
            AutoBuffer<دوگانه, MAX_LOCAL_CN> v(cn);
            ثابت uchar* src = (ثابت uchar*)(constbuf.data() + a.constofs);
            اگر (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * اندازه(دوگانه));
            وگرنه getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            برای (صحیح c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    برای (صحیح i = 0; i < (صحیح)prog.size(); i++) {
        ثابت Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        اگر (ins.arg1) os << ", " << ins.arg1;
        اگر (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        اگر (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        اگر (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        اگر (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
صحیح TExpr::add_درون_داد(صحیح depth)
{
    Arg a; a.kind = درون_داد; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); بازگشت (صحیح)arginfo.size() - 1;
}

صحیح TExpr::addOutput(صحیح depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); بازگشت (صحیح)arginfo.size() - 1;
}

صحیح TExpr::addTemp(صحیح depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); بازگشت (صحیح)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
صحیح TExpr::addConst(صحیح depth, ثابت Scalar& v, صحیح channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); بازگشت (صحیح)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
صحیح TExpr::addConst(صحیح depth, صحیح srcdepth, ثابت تهی* data, صحیح channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); بازگشت (صحیح)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
صحیح TExpr::typedConstFrom(صحیح srcSlot, صحیح depth)
{
    ثابت Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); بازگشت (صحیح)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
صحیح TExpr::addInsn(بالاترین op, صحیح a0, صحیح a1, صحیح a2, صحیح result, ثابت TKernel& kernel, ثابت Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    بازگشت (صحیح)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
صحیح TExpr::addInsn(بالاترین op, صحیح a0, صحیح a1, صحیح a2, صحیح result, ثابت Scalar& params)
{
    صحیح d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    صحیح d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    صحیح d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    بازگشت addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
صحیح TExpr::maybeAddCast(صحیح arg, صحیح depth)
{
    اگر (arginfo[arg].depth == depth) بازگشت arg;
    صحیح t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    بازگشت t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
ایستا درونخطی تهی runInsn(ثابت TExpr::Insn& ins,
                           ثابت تهی* p0, size_t y0, size_t x0,
                           ثابت تهی* p1, size_t y1, size_t x1,
                           ثابت تهی* p2, size_t y2, size_t x2,
                           تهی* pr, size_t yr, صحیح w, صحیح h)
{
    صحیح code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

ساختار EwBody {
    ثابت TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ثابت صحیح* slotMap;
    ثابت signed نویسه* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ثابت صحیح* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    صحیح ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

تهی TExpr::outputShape(ثابت Mat* ثابت* inputs, MatShape& spatial, صحیح& channels) ثابت
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ثابت صحیح rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    منطقی sameShape = ninputs >= 1;
    برای (صحیح i = 1; sameShape && i < ninputs; i++)
    {
        ثابت Mat& a = *inputs[i];
        اگر (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = نادرست;
        وگرنه برای (صحیح d = 0; d < a.dims; d++) اگر (a.size[d] != inputs[0]->size[d]) { sameShape = نادرست; بشکن; }
    }
    اگر (sameShape) { spatial = inputs[0]->size; channels = rcn0; بازگشت; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; صحیح esz1;
    برای (صحیح i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    ثابت صحیح ndFull = (صحیح)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
تهی TExpr::outputShape(ثابت Mat* inputs, MatShape& spatial, صحیح& channels) ثابت
{
    AutoBuffer<ثابت Mat*, 16> ptrs(std::max(ninputs, 1));
    برای (صحیح i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ایستا constexpr صحیح LOCAL_HDRS = 3;
ایستا constexpr صحیح LOCAL_CONSTS = 4;
ایستا constexpr صحیح LOCAL_OPS = 16;

تهی TExpr::exec(ثابت Mat* ثابت* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    ثابت صحیح nslots = (صحیح)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    اگر (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        ثابت Mat& r = *inputs[0];
        ثابت صحیح rcn = r.channels();
        منطقی ok = r.isContinuous();
        برای (صحیح i = 1; ok && i < ninputs; i++)
        {
            ثابت Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        برای (صحیح s = 1; ok && s < nslots; s++)
            اگر (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        ثابت size_t total = ok ? r.total() * rcn : 0;
        اگر (ok && total == 0)
        {
            ثابت MatShape rshape = r.size;
            برای (صحیح s = 1; s < nslots; s++)
                اگر (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            بازگشت;
        }
        اگر (ok && total <= EW_FASTPATH_MAX)
        {
            ثابت MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            برای (صحیح s = 1; s < nslots; s++)
                اگر (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ثابت صحیح ninsn = (صحیح)prog.size();

            اگر (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](صحیح s) -> تهی* {
                    اگر (s <= 0) بازگشت nullptr;
                    ثابت Arg& ai = arginfo[s];
                    اگر (ai.kind == درون_داد)  بازگشت (تهی*)inputs[ai.index]->data;
                    اگر (ai.kind == OUTPUT) بازگشت (تهی*)outputs[ai.index].data;
                    بازگشت nullptr;   // NONE
                };
                ثابت صحیح w = (صحیح)total;
                برای (صحیح n = 0; n < ninsn; n++)
                {
                    ثابت Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                بازگشت;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ثابت صحیح totalEsz = bufEszPrefix[nbuffers];
            ثابت صحیح wf0 = std::min((صحیح)total, capElems);

            ثابت size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/اندازه(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)اندازه(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            برای (صحیح x0 = 0; x0 < (صحیح)total; x0 += wf0)
            {
                ثابت صحیح wf = std::min(wf0, (صحیح)total - x0);
                auto ptrOf = [&](صحیح s) -> تهی* {
                    اگر (s <= 0) بازگشت nullptr;
                    ثابت Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    اگر (ai.kind == درون_داد)  بازگشت (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    اگر (ai.kind == OUTPUT) بازگشت (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    اگر (ai.kind != TEMP)   بازگشت nullptr;                                    // NONE: moved-from
                    ثابت صحیح b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    بازگشت scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                برای (صحیح n = 0; n < ninsn; n++)
                {
                    ثابت Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            بازگشت;
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
    صحیح rchannels;
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
    منطقی inplace = نادرست;
    برای (صحیح i = 0; i < ninputs && !inplace; i++)
        برای (صحیح j = 0; j < noutputs; j++)
            اگر (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = درست; بشکن; }
    صحیح nhdrs = inplace ? ninputs : 0;

    AutoBuffer<ثابت Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<ثابت Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<صحیح, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed نویسه, LOCAL_OPS> slotKind(nslots);
    صحیح narr = 0, nc = 0;
    اگر (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        برای (صحیح j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed نویسه)TExpr::NONE;
    برای (صحیح s = 1; s < nslots; s++)
    {
        ثابت TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed نویسه)ai.kind;
        اگر (ai.kind == TExpr::درون_داد) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        وگرنه اگر (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        وگرنه اگر (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed نویسه)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        وگرنه اگر (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ثابت صحیح c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (تهی*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed نویسه)TExpr::درون_داد;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        وگرنه اگر (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
        }
        وگرنه {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ثابت صحیح ninsn = (صحیح)prog.size();
    long long costPerElem = 0;
    برای (صحیح n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    برای (صحیح d = 0; d < (صحیح)spatial.size(); d++) otot *= spatial[d];
    اگر (otot == 0)
        بازگشت;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ثابت دوگانه T = (دوگانه)std::max(get_رقم_Threads(), 1);
    ثابت دوگانه nstripes = std::min(
        (دوگانه)otot * (دوگانه)std::max<long long>(costPerElem, 1) * (1./ (دوگانه)(1 << 16)),
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
        ثابت صحیح* ثابت smap  = bc.slotMap;
        ثابت signed نویسه* ثابت skind = bc.slotKind;
        ثابت صحیح nin = bc.ninsn, nsl = bc.nslots;
        ثابت صحیح w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ثابت صحیح* ثابت eszPrefix = bc.bufEszPrefix;
        ثابت صحیح bw = std::min(w, bc.capElems);
        ثابت صحیح bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ثابت size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/اندازه(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)اندازه(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        برای (صحیح y0 = 0; y0 < h; y0 += bh)
        {
            ثابت صحیح hf = std::min(bh, h - y0);
            برای (صحیح x0 = 0; x0 < w; x0 += bw)
            {
                ثابت صحیح wf = std::min(bw, w - x0);
                برای (صحیح s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    ثابت صحیح k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    اگر (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    وگرنه                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ثابت BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                برای (صحیح n = 0; n < nin; n++)
                {
                    ثابت TExpr::Insn& ins = insns[n];
                    ثابت BrSlice& a0 = args[ins.arg0]; ثابت BrSlice& a1 = args[ins.arg1];
                    ثابت BrSlice& a2 = args[ins.arg2]; ثابت BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (تهی*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, درست, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
تهی TExpr::exec(ثابت Mat* inputs, Mat* outputs)
{
    AutoBuffer<ثابت Mat*, 16> ptrs(std::max(ninputs, 1));
    برای (صحیح i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
تهی make_دودویی_Arith_Program(TExpr& p, بالاترین op, صحیح depth0, صحیح depth1, صحیح rdepth,
                            صحیح maskDepth, دوگانه scale)
{
    p.clear();
    اگر (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        اگر (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        وگرنه
        {
            rdepth = promoteArith(depth0, depth1);
            اگر (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ثابت منطقی masked = maskDepth != EW_DEPTH_NONE;
    صحیح sIn0  = p.add_درون_داد(depth0);
    صحیح sIn1  = p.add_درون_داد(depth1);
    صحیح sMask = masked ? p.add_درون_داد(maskDepth) : 0;
    صحیح sOut  = p.addOutput(rdepth);

    صحیح r = p.emit_دودویی(op, sIn0, sIn1, rdepth, Scalar(scale));
    اگر (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    وگرنه
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
تهی makeAddWeightedProgram(TExpr& p, صحیح depth0, صحیح depth1, صحیح rdepth,
                            دوگانه alpha, دوگانه beta, دوگانه gamma)
{
    p.clear();
    صحیح sA   = p.add_درون_داد(depth0);
    صحیح sB   = p.add_درون_داد(depth1);
    صحیح sOut = p.addOutput(rdepth);
    صحیح r = p.emit_دودویی(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

فضاینام {

// --- tokens ------------------------------------------------------------------------------
شمارش TokType { T_رقم, T_درون_داد, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

ساختار Token
{
    TokType type = T_END;
    دوگانه num = 0;
    صحیح درون_داد_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
ساختار Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    ایستا منطقی isIdentStart(نویسه c) { بازگشت std::isalpha((unsigned نویسه)c) || c == '_'; }
    ایستا منطقی isIdentChar(نویسه c)  { بازگشت std::isalnum((unsigned نویسه)c) || c == '_'; }

    Token next()
    {
        وقتی (pos < s.size() && std::isspace((unsigned نویسه)s[pos])) pos++;
        Token t;
        اگر (pos >= s.size()) { t.type = T_END; بازگشت t; }

        نویسه c = s[pos];

        اگر (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            وقتی (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_درون_داد;
            t.درون_داد_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            بازگشت t;
        }
        اگر (std::isdigit((unsigned نویسه)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned نویسه)s[pos + 1])))
        {
            نویسه* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_رقم;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            بازگشت t;
        }
        اگر (isIdentStart(c))
        {
            size_t start = pos;
            وقتی (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            بازگشت t;
        }
        انتخاب (c)
        {
        حالت '(': pos++; t.type = T_LPAREN; بازگشت t;
        حالت ')': pos++; t.type = T_RPAREN; بازگشت t;
        حالت ',': pos++; t.type = T_COMMA;  بازگشت t;
        حالت ';': pos++; t.type = T_SEMI;   بازگشت t;
        }
        // multi/!single-char operators
        auto two = [&](ثابت نویسه* op) {
            بازگشت pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        اگر (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; بازگشت t; }
        اگر (c == '=') { pos++; t.type = T_ASSIGN; بازگشت t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        بازگشت t;
    }
};

// --- operator / function tables ----------------------------------------------------------
ایستا صحیح binPrec(ثابت std::string& op)
{
    اگر (op == "**") بازگشت 8;              // power binds tighter than '*'; RIGHT-associative
    اگر (op == "*" || op == "/") بازگشت 7;
    اگر (op == "+" || op == "-") بازگشت 6;
    اگر (op == "<" || op == "<=" || op == ">" || op == ">=") بازگشت 5;
    اگر (op == "==" || op == "!=") بازگشت 4;
    اگر (op == "&") بازگشت 3;
    اگر (op == "^") بازگشت 2;
    اگر (op == "|") بازگشت 1;
    بازگشت -1;
}

ایستا بالاترین binOp(ثابت std::string& op)
{
    اگر (op == "**") بازگشت OP_POW;
    اگر (op == "+")  بازگشت OP_ADD;
    اگر (op == "-")  بازگشت OP_SUB;
    اگر (op == "*")  بازگشت OP_MUL;
    اگر (op == "/")  بازگشت OP_DIV;
    اگر (op == "<")  بازگشت OP_CMP_LT;
    اگر (op == "<=") بازگشت OP_CMP_LE;
    اگر (op == ">")  بازگشت OP_CMP_GT;
    اگر (op == ">=") بازگشت OP_CMP_GE;
    اگر (op == "==") بازگشت OP_CMP_EQ;
    اگر (op == "!=") بازگشت OP_CMP_NE;
    اگر (op == "&")  بازگشت OP_AND;
    اگر (op == "|")  بازگشت OP_OR;
    اگر (op == "^")  بازگشت OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
ایستا صحیح typeDepth(ثابت std::string& name)
{
    اگر (name == "float")    بازگشت CV_32F;
    اگر (name == "double")   بازگشت CV_64F;
    اگر (name == "half" || name == "float16") بازگشت CV_16F;
    اگر (name == "bfloat16") بازگشت CV_16BF;
    اگر (name == "uint8")    بازگشت CV_8U;
    اگر (name == "int8")     بازگشت CV_8S;
    اگر (name == "uint16")   بازگشت CV_16U;
    اگر (name == "int16")    بازگشت CV_16S;
    اگر (name == "uint32")   بازگشت CV_32U;
    اگر (name == "int32")    بازگشت CV_32S;
    اگر (name == "uint64")   بازگشت CV_64U;
    اگر (name == "int64")    بازگشت CV_64S;
    بازگشت -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
ایستا بالاترین fnOp(ثابت std::string& name, صحیح& arity)
{
    ساختار E { ثابت نویسه* n; بالاترین op; };
    ایستا ثابت E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    ایستا ثابت E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    ایستا ثابت E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    برای (ثابت E& e : unary)  اگر (name == e.n) { arity = 1; بازگشت e.op; }
    برای (ثابت E& e : binary) اگر (name == e.n) { arity = 2; بازگشت e.op; }
    برای (ثابت E& e : tern)   اگر (name == e.n) { arity = 3; بازگشت e.op; }
    arity = 0; بازگشت OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
ساختار Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ثابت صحیح* درون_داد_Slot;                 // slot id of each external input (precreated)
    صحیح ninputs;
    std::map<std::string, صحیح> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& عبارت, ثابت صحیح* islot, صحیح nin)
        : lex(src), e(عبارت), درون_داد_Slot(islot), ninputs(nin) { cur = lex.next(); }

    تهی advance() { cur = lex.next(); }
    منطقی isOp(ثابت نویسه* op) ثابت { بازگشت cur.type == T_OP && cur.text == op; }

    تهی expect(TokType t, ثابت نویسه* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    منطقی isFlexConst(صحیح slot) ثابت
    {
        بازگشت e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    صحیح parsePrimary()
    {
        اگر (cur.type == T_رقم)   { صحیح s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); بازگشت s; }
        اگر (cur.type == T_درون_داد) { صحیح idx = cur.درون_داد_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   بازگشت درون_داد_Slot[idx]; }
        اگر (cur.type == T_LPAREN){ advance(); صحیح x = parseTernary(); expect(T_RPAREN, "expected ')'"); بازگشت x; }
        اگر (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            اگر (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                بازگشت it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<صحیح, 8> args = {}; صحیح nargs = 0;
            اگر (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                وقتی (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (صحیح)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            صحیح td = typeDepth(name);
            اگر (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                صحیح s = args[0];
                بازگشت isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            صحیح arity = 0; بالاترین op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            اگر (arity == 1) بازگشت e.emitUnary(op, args[0]);
            اگر (arity == 2) بازگشت e.emit_دودویی(op, args[0], args[1]);
            بازگشت e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    صحیح parseUnary()
    {
        اگر (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            صحیح operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            اگر (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<دوگانه, MAX_LOCAL_CN> v; صحیح cn = constDoubles(e, operand, v);
                Scalar neg; برای (صحیح i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                بازگشت e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            بازگشت e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        بازگشت parsePrimary();
    }

    صحیح parse_عبارت(صحیح minPrec)
    {
        صحیح چپ = parseUnary();
        وقتی (cur.type == T_OP)
        {
            صحیح p = binPrec(cur.text);
            اگر (p < minPrec) بشکن;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            صحیح راست = parse_عبارت(op == "**" ? p : p + 1);
            چپ = e.emit_دودویی(binOp(op), چپ, راست);
        }
        بازگشت چپ;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    صحیح parseTernary()
    {
        صحیح cond = parse_عبارت(0);
        اگر (!isOp("?")) بازگشت cond;
        advance();
        صحیح thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        صحیح elsev = parseTernary();               // right-associative chain
        بازگشت e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    تهی parseResult()
    {
        اگر (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); صحیح nTemp = e.ntemps;
            advance();
            صحیح e0 = parseTernary();
            اگر (cur.type == T_COMMA)
            {
                e.output(e0);
                وقتی (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                بازگشت;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    تهی parse()
    {
        وقتی (درست)
        {
            اگر (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                اگر (cur.type == T_ASSIGN)
                {
                    advance();
                    ثابت صحیح slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = درست;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    ادامه;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            اگر (cur.type == T_SEMI) advance();
            بشکن;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

فضاینام cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
تهی texpr(ثابت std::string& عبارت, درون_داد_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using فضاینام cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == درون_داد_Array_2::STD_VECTOR_MAT);
    ثابت std::vector<Mat>& inps = *(ثابت std::vector<Mat>*)_inputs.getObj();
    ثابت صحیح ninputs = (صحیح)inps.size();
    برای (صحیح i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<صحیح> islot(std::max(ninputs, 1));
    برای (صحیح i = 0; i < ninputs; i++) islot[i] = e.add_درون_داد(inps[i].depth());
    Parser(عبارت, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    اگر (kind == درون_داد_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } وگرنه {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
