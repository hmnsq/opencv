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

نامخلا cv { نامخلا ew {

// ============================ op metadata (was ew_op.cpp) ============================

ثابت حرف* opName(چوٹی op)
{
    انتخاب (op)
    {
    حالت OP_NOP:           واپس "nop";
    حالت OP_NEG:           واپس "neg";
    حالت OP_ABS:           واپس "abs";
    حالت OP_NOT:           واپس "not";
    حالت OP_SQRT:          واپس "sqrt";
    حالت OP_EXP:           واپس "exp";
    حالت OP_LOG:           واپس "log";
    حالت OP_SIN:           واپس "sin";
    حالت OP_COS:           واپس "cos";
    حالت OP_TANH:          واپس "tanh";
    حالت OP_ERF:           واپس "erf";
    حالت OP_RELU:          واپس "relu";
    حالت OP_CAST:          واپس "cast";
    حالت OP_جوڑو:           واپس "add";
    حالت OP_SUB:           واپس "sub";
    حالت OP_MUL:           واپس "mul";
    حالت OP_DIV:           واپس "div";
    حالت OP_POW:           واپس "pow";
    حالت OP_MIN:           واپس "min";
    حالت OP_MAX:           واپس "max";
    حالت OP_ABSDIFF:       واپس "absdiff";
    حالت OP_ADDW:          واپس "addWeighted";
    حالت OP_HYPOT:         واپس "hypot";
    حالت OP_ATAN2:         واپس "atan2";
    حالت OP_AND:           واپس "and";
    حالت OP_OR:            واپس "or";
    حالت OP_XOR:           واپس "xor";
    حالت OP_CMP_EQ:        واپس "cmp_eq";
    حالت OP_CMP_NE:        واپس "cmp_ne";
    حالت OP_CMP_LT:        واپس "cmp_lt";
    حالت OP_CMP_LE:        واپس "cmp_le";
    حالت OP_CMP_GT:        واپس "cmp_gt";
    حالت OP_CMP_GE:        واپس "cmp_ge";
    حالت OP_CLAMP:         واپس "clamp";
    حالت OP_SELECT:        واپس "select";
    حالت OP_CONVERT_SCALE: واپس "convert_scale";
    طےشدہ:               واپس "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(چوٹی op)
{
    انتخاب (op)
    {
    حالت OP_AND: حالت OP_OR: حالت OP_XOR: حالت OP_NOT:
        واپس CAT_BITWISE;
    حالت OP_CMP_EQ: حالت OP_CMP_NE: حالت OP_CMP_LT:
    حالت OP_CMP_LE: حالت OP_CMP_GT: حالت OP_CMP_GE:
        واپس CAT_COMPARE;
    حالت OP_SQRT: حالت OP_EXP: حالت OP_LOG:
    حالت OP_SIN: حالت OP_COS: حالت OP_TANH: حالت OP_ERF: حالت OP_ATAN2: حالت OP_RELU:
        واپس CAT_MATH;
    حالت OP_CAST: حالت OP_CONVERT_SCALE:
        واپس CAT_CAST;
    حالت OP_SELECT:
        واپس CAT_SELECT;
    طےشدہ:
        واپس CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
ساکن منطقی is_Float_گہرائی(صحیح d)
{
    واپس d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
ساکن منطقی گہرائی_Represents(دوہرا v, صحیح گہرائی)
{
    اگر (is_Float_گہرائی(گہرائی)) واپس سچ;
    اگر (v != std::floor(v)) واپس جھوٹ;
    انتخاب (گہرائی)
    {
    حالت CV_8U:  واپس v >= 0            && v <= 255;
    حالت CV_8S:  واپس v >= -128         && v <= 127;
    حالت CV_16U: واپس v >= 0            && v <= 65535;
    حالت CV_16S: واپس v >= -32768       && v <= 32767;
    حالت CV_32U: واپس v >= 0            && v <= 4294967295.0;
    حالت CV_32S: واپس v >= -2147483648.0 && v <= 2147483647.0;
    حالت CV_64U: واپس v >= 0            && v <= 18446744073709551615.0;
    حالت CV_64S: واپس v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    طےشدہ:     واپس جھوٹ;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
ساکن خالی intRange(صحیح گہرائی, دوہرا& lo, دوہرا& hi)
{
    انتخاب (گہرائی)
    {
    حالت CV_8U:  lo = 0;                     hi = 255; توڑو;
    حالت CV_8S:  lo = -128;                  hi = 127; توڑو;
    حالت CV_16U: lo = 0;                     hi = 65535; توڑو;
    حالت CV_16S: lo = -32768;                hi = 32767; توڑو;
    حالت CV_32U: lo = 0;                     hi = 4294967295.0; توڑو;
    حالت CV_32S: lo = -2147483648.0;         hi = 2147483647.0; توڑو;
    حالت CV_64U: lo = 0;                     hi = 18446744073709551615.0; توڑو;
    حالت CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; توڑو;
    طےشدہ:     lo = 0;                     hi = 0; توڑو;
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
    constexpr غیرعلامتی signlut = 0b1001111110100u;
    constexpr غیرعلامتی fltlut  = 0b1111000000u;

    اگر (a == b) واپس a;

    صحیح wa = صحیح((leszlut >> (a+1)*3) & 7u), wb = صحیح((leszlut >> (b+1)*3) & 7u);
    صحیح fa = صحیح((fltlut >> (a+1)) & 1u),    fb = صحیح((fltlut >> (b+1)) & 1u);

    اگر (fa + fb == 0)                          // both integer
    {
        صحیح sa = صحیح((signlut >> (a+1)) & 1u), sb = صحیح((signlut >> (b+1)) & 1u);
        اگر (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            واپس wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        صحیح rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr صحیح ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        واپس (ilut >> rw*5) & CV_MAT_گہرائی_نقاب;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    صحیح maxw = std::max(wa, wb);
    اگر (maxw == 2 && fa + fb == 1)
        واپس fa*a + (1 - fa)*b;
    واپس CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
صحیح absdiff_نتیجہ_گہرائی(صحیح گہرائی)
{
    انتخاب (گہرائی)
    {
    حالت CV_8S:  واپس CV_8U;
    حالت CV_16S: واپس CV_16U;
    حالت CV_32S: واپس CV_32U;
    حالت CV_64S: واپس CV_64U;
    طےشدہ:     واپس گہرائی;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
ساکن صحیح promote2(صحیح a, صحیح b)
{
    اگر (a == b) واپس a;

    constexpr غیرعلامتی lbits = 3, lmask = (1u << lbits) - 1u;
    ثابت uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    غیرعلامتی pr_a = غیرعلامتی((typelut >> (a*lbits)) & lmask);
    غیرعلامتی pr_b = غیرعلامتی((typelut >> (b*lbits)) & lmask);
    غیرعلامتی max_pr = std::max(pr_a, pr_b);
    constexpr غیرعلامتی dbits = CV_CN_سرکاؤ, dmask = (1u << dbits) - 1u;
    ثابت غیرعلامتی ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    واپس صحیح((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
ساکن صحیح promoteBitwise(صحیح a, صحیح b)
{
    اگر (a == EW_گہرائی_NONE) واپس b;
    اگر (b == EW_گہرائی_NONE) واپس a;
    اگر (a == b) واپس a;
    واپس CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
ساکن صحیح محفوظ_Wide(صحیح گہرائی)
{
    constexpr غیرعلامتی lbits = 3, lmask = (1u << lbits) - 1u;
    ثابت uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    غیرعلامتی pr_گہرائی = غیرعلامتی((typelut >> (گہرائی*lbits)) & lmask);
    constexpr غیرعلامتی dbits = CV_CN_سرکاؤ, dmask = (1u << dbits) - 1u;
    ثابت غیرعلامتی ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    واپس صحیح((ctypelut >> (pr_گہرائی*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
شمار { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
ساکن size_t appendConstBuf(Auto_بفر<uint64_t, 16>& constbuf, صحیح srcdepth, ثابت خالی* ڈیٹا, صحیح channels)
{
    ثابت صحیح cn = std::max(1, channels);
    ثابت size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ثابت size_t nu64 = (nbytes + حجم(uint64_t) - 1) / حجم(uint64_t);
    ثابت size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.ڈیٹا() + ofs);
    اگر (ڈیٹا) حافظہ_نقل(dst, ڈیٹا, nbytes); ورنہ memset(dst, 0, nbytes);
    واپس ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
ساکن صحیح constDoubles(ثابت TExpr& e, صحیح s, Auto_بفر<دوہرا, MAX_LOCAL_CN>& buf)
{
    ثابت TExpr::Arg& a = e.arginfo[s];
    ثابت صحیح cn = std::max(1, a.channels);
    buf.resize(cn);
    ثابت uchar* src = (ثابت uchar*)(e.constbuf.ڈیٹا() + a.constofs);
    اگر (a.srcdepth == CV_64F)
        حافظہ_نقل(buf.ڈیٹا(), src, (size_t)cn * حجم(دوہرا));   // common (Scalar / parsed literal)
    ورنہ
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, خالی_اشاریہ, 0, (uchar*)buf.ڈیٹا(), 0, سائز(cn, 1), خالی_اشاریہ);
    واپس cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
ساکن اندرونی منطقی isFlexConst(ثابت TExpr& e, صحیح s)
{
    واپس e.arginfo[s].قسم == TExpr::CONST && e.arginfo[s].گہرائی == EW_گہرائی_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
ساکن منطقی scalar_Const_قدر(ثابت TExpr& e, صحیح s, دوہرا& v)
{
    ثابت TExpr::Arg& a = e.arginfo[s];
    اگر (a.قسم != TExpr::CONST || std::max(1, a.channels) != 1) واپس جھوٹ;
    Auto_بفر<دوہرا, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    واپس سچ;
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
ساکن منطقی مطابقت_Scaled_Chain(ثابت TExpr& e, صحیح end, صحیح t,
                             صحیح& arr_خانہ, دوہرا& scale, صحیح& آغاز_Idx)
{
    اگر (end < 0) واپس جھوٹ;
    صحیح j = end, mul_نتیجہ = t;

    // optional trailing narrowing cast
    اگر (e.prog[j].op == OP_CAST && e.prog[j].نتیجہ == t)
    {
        mul_نتیجہ = e.prog[j].arg0;
        اگر (--j < 0) واپس جھوٹ;
    }

    ثابت TExpr::Insn& mul = e.prog[j];
    اگر (mul.op != OP_MUL || mul.نتیجہ != mul_نتیجہ || mul.params[0] != 1.0) واپس جھوٹ;

    صحیح arr;
    اگر      (scalar_Const_قدر(e, mul.arg1, scale)) arr = mul.arg0;
    ورنہ اگر (scalar_Const_قدر(e, mul.arg0, scale)) arr = mul.arg1;
    ورنہ واپس جھوٹ;                                  // array*array is not an addWeighted term
    آغاز_Idx = j;

    // optional leading widening cast feeding the multiply
    اگر (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].نتیجہ == arr &&
        e.arginfo[arr].قسم == TExpr::عارضی)
    {
        arr = e.prog[j-1].arg0;
        آغاز_Idx = j - 1;
    }

    اگر (e.arginfo[arr].قسم == TExpr::CONST) واپس جھوٹ;   // scalar*scalar is folded elsewhere
    arr_خانہ = arr;
    واپس سچ;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
ساکن منطقی const_سماتا(ثابت TExpr& e, صحیح s, صحیح d)
{
    اگر (!isFlexConst(e, s)) واپس سچ;
    Auto_بفر<دوہرا, MAX_LOCAL_CN> v;
    صحیح cn = constDoubles(e, s, v);
    برائے (صحیح ch = 0; ch < cn; ch++)
        اگر (!گہرائی_Represents(v[ch], d)) واپس جھوٹ;
    واپس سچ;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
صحیح TExpr::emit_ثنائی(چوٹی op, صحیح a, صحیح b, صحیح rdepth, ثابت Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    اگر (op == OP_ADDW)
    {
        صحیح Tt = arginfo[a].گہرائی;
        اگر (arginfo[a].گہرائی != arginfo[b].گہرائی)
        {
            Tt = promoteArith(arginfo[a].گہرائی, arginfo[b].گہرائی);
            a = maybe_جوڑو_Cast(a, Tt); b = maybe_جوڑو_Cast(b, Tt);
        }
        اگر (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        اگر (rdepth == EW_گہرائی_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_گہرائی_NONE, rdepth);
        صحیح برآمد_D = rdepth;
        اگر (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            برآمد_D = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_گہرائی_NONE, برآمد_D);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ثابت صحیح out = جوڑو_عارضی(برآمد_D);
        جوڑو_Insn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        اگر (برآمد_D == rdepth) واپس out;
        ثابت صحیح out2 = جوڑو_عارضی(rdepth);
        جوڑو_Insn(OP_CAST, out, 0, 0, out2);
        واپس out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    اگر (op == OP_جوڑو)
    {
        ثابت صحیح n = (صحیح)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        صحیح x = 0, y = 0, آغاز_A = 0, آغاز_B = 0;
        دوہرا alpha = 0, beta = 0;
        اگر (n >= 2 && a != b &&
            arginfo[a].قسم == عارضی && !arginfo[a].pinned &&
            arginfo[b].قسم == عارضی && !arginfo[b].pinned &&
            مطابقت_Scaled_Chain(*یہ, n - 1, b, y, beta, آغاز_B) &&
            مطابقت_Scaled_Chain(*یہ, آغاز_B - 1, a, x, alpha, آغاز_A) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].گہرائی != CV_Bool && arginfo[y].گہرائی != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            صحیح retired = 0, lowest = زیادہ_سے_زیادہ_عدد;
            برائے (صحیح i = آغاز_A; i < n; i++)
            {
                ثابت صحیح r = prog[i].نتیجہ;
                اگر (arginfo[r].قسم != عارضی || arginfo[r].pinned) { retired = -1; توڑو; }
                lowest = std::min(lowest, arginfo[r].اشاریہ);
                retired++;
            }
            اگر (retired > 0 && lowest == ntemps - retired)
            {
                برائے (صحیح i = آغاز_A; i < n; i++) arginfo[prog[i].نتیجہ].قسم = NONE;
                prog.resize(آغاز_A);
                ntemps -= retired;
                واپس emit_ثنائی(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            صحیح acc = 0; دوہرا gamma = 0;
            اگر      (scalar_Const_قدر(*یہ, b, gamma)) acc = a;
            ورنہ اگر (scalar_Const_قدر(*یہ, a, gamma)) acc = b;
            اگر (acc != 0 && n >= 1 &&
                arginfo[acc].قسم == عارضی && !arginfo[acc].pinned &&
                arginfo[acc].اشاریہ == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].نتیجہ == acc &&
                (rdepth == EW_گہرائی_NONE || rdepth == arginfo[acc].گہرائی))
            {
                prog[n-1].params[2] += gamma;
                واپس acc;
            }
        }
    }

    ثابت صحیح nd0 = isFlexConst(*یہ, a) ? EW_گہرائی_NONE : arginfo[a].گہرائی;
    ثابت صحیح nd1 = isFlexConst(*یہ, b) ? EW_گہرائی_NONE : arginfo[b].گہرائی;
    ثابت ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    صحیح نتیجہ = rdepth;
    اگر (نتیجہ == EW_گہرائی_NONE)
    {
        نتیجہ = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        اگر (نتیجہ == EW_گہرائی_NONE) نتیجہ = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    صحیح بنیاد;
    اگر (nd0 == EW_گہرائی_NONE && nd1 == EW_گہرائی_NONE) بنیاد = نتیجہ;
    ورنہ اگر (nd0 == EW_گہرائی_NONE) بنیاد = nd1;
    ورنہ اگر (nd1 == EW_گہرائی_NONE) بنیاد = nd0;
    ورنہ بنیاد = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    اگر (cat == CAT_COMPARE && nd0 != EW_گہرائی_NONE && !is_Float_گہرائی(nd0) &&
        isFlexConst(*یہ, b) && !const_سماتا(*یہ, b, بنیاد))
    {
        دوہرا lo, hi; intRange(nd0, lo, hi);
        اگر (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            شمار { REAL, CFALSE, CTRUE };
            Auto_بفر<دوہرا, MAX_LOCAL_CN> tv; صحیح cn = constDoubles(*یہ, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            صحیح قسم[4] = {}; دوہرا bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ثابت چوٹی fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            برائے (صحیح c = 0; c < cn; c++)
            {
                ثابت دوہرا t = tv[c]; صحیح k; دوہرا B = t;
                انتخاب (op)
                {
                حالت OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; توڑو;
                حالت OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; توڑو;
                حالت OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; توڑو;
                حالت OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; توڑو;
                حالت OP_CMP_EQ: k = گہرائی_Represents(t, nd0) ? REAL : CFALSE; توڑو;   // a==non-rep -> false
                طےشدہ:        k = گہرائی_Represents(t, nd0) ? REAL : CTRUE;  توڑو;   // NE: a!=non-rep -> true
                }
                قسم[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            خودکار ucOp  = [&](صحیح c){ واپس قسم[c]==REAL ? fam : قسم[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            خودکار ucThr = [&](صحیح c){ واپس قسم[c]==REAL ? bound[c]
                                          : (قسم[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ثابت چوٹی u0 = ucOp(0);
            منطقی یکساں = سچ;
            برائے (صحیح c = 1; c < cn && یکساں; c++) یکساں = (ucOp(c) == u0);
            اگر (یکساں)
            {
                Scalar s; برائے (صحیح c = 0; c < cn; c++) s[c] = ucThr(c);
                b = جوڑو_Const(EW_گہرائی_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            ورنہ
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; صحیح patchFlags = 0;
                برائے (صحیح c = 0; c < cn; c++)
                {
                    ts[c] = قسم[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ثابت صحیح mbits = قسم[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ثابت صحیح vbits = قسم[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_سرکاؤ + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, بنیاد, بنیاد, EW_گہرائی_NONE, نتیجہ);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ثابت صحیح out = جوڑو_عارضی(نتیجہ);
                جوڑو_Insn(fam, a, جوڑو_Const(بنیاد, ts, cn), 0, out, kern);
                واپس out;
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
    صحیح گہرائی, wide;
    منطقی divGuard = جھوٹ;
    انتخاب (op)
    {
    حالت OP_MIN: حالت OP_MAX:
    حالت OP_AND: حالت OP_OR: حالت OP_XOR:
        گہرائی = نتیجہ; wide = نتیجہ; توڑو;
    حالت OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        صحیح common = (nd0 == EW_گہرائی_NONE) ? nd1
                   : (nd1 == EW_گہرائی_NONE) ? nd0 : promoteArith(nd0, nd1);
        اگر (common == EW_گہرائی_NONE) common = نتیجہ;          // both operands flexible consts
        گہرائی = common;
        wide  = absdiff_نتیجہ_گہرائی(common);
        اگر (rdepth == EW_گہرائی_NONE) نتیجہ = wide;            // auto result: unsigned same width
        توڑو;
    }
    حالت OP_CMP_EQ: حالت OP_CMP_NE: حالت OP_CMP_LT:
    حالت OP_CMP_LE: حالت OP_CMP_GT: حالت OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        گہرائی = بنیاد; wide = CV_64F; توڑو;
    حالت OP_MUL: حالت OP_DIV:
    {
        گہرائی = بنیاد;
        ثابت منطقی w = (گہرائی==CV_32U || گہرائی==CV_32S || گہرائی==CV_64U ||
                        گہرائی==CV_64S || گہرائی==CV_64F);
        wide = w ? CV_64F : CV_32F;
        اگر (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            خودکار int_عملیہ = [&](صحیح s, صحیح nd) {
                اگر (nd != EW_گہرائی_NONE) واپس !is_Float_گہرائی(nd);
                Auto_بفر<دوہرا, MAX_LOCAL_CN> v; constDoubles(*یہ, s, v);
                واپس v[0] == std::floor(v[0]); };
            divGuard = int_عملیہ(a, nd0) && int_عملیہ(b, nd1);
        }
        توڑو;
    }
    حالت OP_POW: حالت OP_HYPOT: حالت OP_ATAN2:
        گہرائی = (بنیاد == CV_64F) ? CV_64F : CV_32F; wide = گہرائی; توڑو;
    طےشدہ:        // OP_ADD, OP_SUB
        گہرائی = بنیاد; wide = محفوظ_Wide(گہرائی); توڑو;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    خودکار fracConst = [&](صحیح s) {
        اگر (!isFlexConst(*یہ, s)) واپس جھوٹ;
        Auto_بفر<دوہرا, MAX_LOCAL_CN> v; صحیح n = constDoubles(*یہ, s, v);
        برائے (صحیح ch = 0; ch < n; ch++)
            اگر (v[ch] != std::floor(v[ch])) واپس سچ;
        واپس جھوٹ;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    اگر (cat != CAT_BITWISE && (!const_سماتا(*یہ, a, گہرائی) || !const_سماتا(*یہ, b, گہرائی)))
    {
        گہرائی = wide;
        ثابت منطقی frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        اگر (frac && !is_Float_گہرائی(گہرائی) && op != OP_MIN && op != OP_MAX)
            گہرائی = wide = (نتیجہ == CV_64F || بنیاد == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        اگر (frac && (op == OP_MUL || op == OP_DIV) && گہرائی == CV_32F)
            گہرائی = wide = CV_64F;
    }

    صحیح s0 = isFlexConst(*یہ, a) ? typedConstFrom(a, گہرائی) : a;
    صحیح s1 = isFlexConst(*یہ, b) ? typedConstFrom(b, گہرائی) : b;
    صحیح c0 = maybe_جوڑو_Cast(s0, گہرائی), c1 = maybe_جوڑو_Cast(s1, گہرائی);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    خودکار resolve = [&](صحیح rd) {
        واپس op == OP_DIV ? getDivFunc(گہرائی, rd, divGuard)
                            : getElemwiseFunc(op, گہرائی, گہرائی, EW_گہرائی_NONE, rd);
    };
    TKernel k = resolve(نتیجہ);
    اگر (k.fptr)
    {
        صحیح res = جوڑو_عارضی(نتیجہ);
        جوڑو_Insn(op, c0, c1, 0, res, k, params);
        واپس res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحیح w = جوڑو_عارضی(wide);
    جوڑو_Insn(op, c0, c1, 0, w, k, params);
    واپس maybe_جوڑو_Cast(w, نتیجہ);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
صحیح TExpr::emitUnary(چوٹی op, صحیح a, صحیح rdepth, ثابت Scalar& params)
{
    ثابت صحیح nd = isFlexConst(*یہ, a) ? EW_گہرائی_NONE : arginfo[a].گہرائی;
    ثابت ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    اگر (op == OP_NEG)
        واپس emit_ثنائی(OP_SUB, جوڑو_Const(EW_گہرائی_NONE, Scalar(0.), 1), a, rdepth, params);
    اگر (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        اگر (!prog.empty() && arginfo[a].قسم == عارضی && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().نتیجہ == a &&
            arginfo[a].اشاریہ == ntemps - 1)
        {
            ثابت صحیح x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].قسم = NONE;
            ntemps--;
            واپس emit_ثنائی(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        واپس emit_ثنائی(OP_ABSDIFF, a, جوڑو_Const(EW_گہرائی_NONE, Scalar(0.), 1), rdepth, params);
    }

    صحیح نتیجہ = rdepth;
    اگر (نتیجہ == EW_گہرائی_NONE)
    {
        انتخاب (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        حالت CAT_MATH:    نتیجہ = is_Float_گہرائی(nd) ? nd : CV_32F; توڑو;
        حالت CAT_BITWISE: نتیجہ = (nd == EW_گہرائی_NONE) ? CV_32S : nd; توڑو;  // NOT
        طےشدہ:          نتیجہ = (nd == EW_گہرائی_NONE) ? CV_32F : nd; توڑو;  // NEG, ABS
        }
    }

    صحیح گہرائی, wide;
    انتخاب (cat)
    {
    حالت CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        گہرائی = (is_Float_گہرائی(nd) && nd == نتیجہ) ? nd
              : (nd == CV_64F || نتیجہ == CV_64F) ? CV_64F : CV_32F;
        wide = گہرائی;
        توڑو;
    حالت CAT_BITWISE: گہرائی = نتیجہ; wide = نتیجہ; توڑو;
    طےشدہ:          گہرائی = (nd == EW_گہرائی_NONE) ? نتیجہ : nd; wide = نتیجہ; توڑو;  // NEG/ABS
    }

    اگر (!const_سماتا(*یہ, a, گہرائی)) گہرائی = (cat == CAT_MATH) ? گہرائی : CV_32F;
    صحیح s0 = isFlexConst(*یہ, a) ? typedConstFrom(a, گہرائی) : a;
    صحیح c0 = maybe_جوڑو_Cast(s0, گہرائی);

    TKernel k = getElemwiseFunc(op, گہرائی, EW_گہرائی_NONE, EW_گہرائی_NONE, نتیجہ);
    اگر (k.fptr)
    {
        صحیح res = جوڑو_عارضی(نتیجہ);
        جوڑو_Insn(op, c0, 0, 0, res, k, params);
        واپس res;
    }
    k = getElemwiseFunc(op, گہرائی, EW_گہرائی_NONE, EW_گہرائی_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحیح w = جوڑو_عارضی(wide);
    جوڑو_Insn(op, c0, 0, 0, w, k, params);
    واپس maybe_جوڑو_Cast(w, نتیجہ);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
صحیح TExpr::emitTernary(چوٹی op, صحیح a, صحیح b, صحیح c, صحیح rdepth)
{
    خودکار dep = [&](صحیح s) { واپس isFlexConst(*یہ, s) ? EW_گہرائی_NONE : arginfo[s].گہرائی; };

    اگر (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        اگر (isFlexConst(*یہ, a) || CV_ELEM_SIZE1(arginfo[a].گہرائی) != 1)
            a = emit_ثنائی(OP_CMP_NE, a, جوڑو_Const(EW_گہرائی_NONE, Scalar(0.), 1), CV_8U, Scalar());
        صحیح نتیجہ = rdepth != EW_گہرائی_NONE ? rdepth : promoteArith(dep(b), dep(c));
        اگر (نتیجہ == EW_گہرائی_NONE) نتیجہ = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        صحیح sb = isFlexConst(*یہ, b) ? typedConstFrom(b, نتیجہ) : maybe_جوڑو_Cast(b, نتیجہ);
        صحیح sc = isFlexConst(*یہ, c) ? typedConstFrom(c, نتیجہ) : maybe_جوڑو_Cast(c, نتیجہ);
        صحیح res = جوڑو_عارضی(نتیجہ);
        جوڑو_Insn(OP_SELECT, a, sb, sc, res);
        واپس res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    صحیح نتیجہ = rdepth != EW_گہرائی_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    اگر (نتیجہ == EW_گہرائی_NONE) نتیجہ = CV_32F;
    خودکار typed = [&](صحیح s) {
        واپس isFlexConst(*یہ, s) ? typedConstFrom(s, نتیجہ) : maybe_جوڑو_Cast(s, نتیجہ);
    };
    صحیح c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, نتیجہ, نتیجہ, نتیجہ, نتیجہ);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحیح res = جوڑو_عارضی(نتیجہ);
    جوڑو_Insn(op, c0, c1, c2, res, k);
    واپس res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
صحیح TExpr::move_مقصد_Output(صحیح عارضی_2, صحیح out)
{
    ثابت صحیح ninsn = (صحیح)prog.size();
    صحیح پیداکار = -1;
    منطقی مستعمل_As_Arg = جھوٹ;
    برائے (صحیح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        اگر (ins.نتیجہ == عارضی_2) پیداکار = i;
        اگر (ins.arg0 == عارضی_2 || ins.arg1 == عارضی_2 || ins.arg2 == عارضی_2) مستعمل_As_Arg = سچ;
    }
    اگر (arginfo[عارضی_2].قسم == عارضی && پیداکار >= 0 && !مستعمل_As_Arg && !arginfo[عارضی_2].pinned &&
        arginfo[عارضی_2].گہرائی == arginfo[out].گہرائی)
    {
        // MOVE semantics: redirect `temp`'s single producer to write `out` directly, then leave the
        // source slot EMPTY - reclassify it to NONE. A NONE slot gets no physical buffer (compile's
        // liveness only walks TEMP slots) and is skipped everywhere in exec, so no stale dead-TEMP slot
        // remains (which, buffer or not, would still cost per-tile setup in the general path) - and no
        // slot removal / index renumbering is needed. This keeps single-op programs at zero temps =>
        // compile()'s no-temp early-out (zero heap traffic) still fires. Every caller moves the result
        // it JUST emitted, so `temp` is always the last-created temp - decrement ntemps to retire its
        // index and keep the temp indices dense (0..ntemps-1) for compile()'s liveness arrays.
        CV_Assert(arginfo[عارضی_2].اشاریہ == ntemps - 1);
        prog[پیداکار].نتیجہ = out;
        arginfo[عارضی_2].قسم = NONE;
        ntemps--;
        واپس out;
    }
    جوڑو_Insn(OP_CAST, عارضی_2, 0, 0, out);                     // same/different depth copy into `out`
    واپس out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
صحیح TExpr::output(صحیح جڑ_خانہ)
{
    واپس move_مقصد_Output(جڑ_خانہ, جوڑو_Output(arginfo[جڑ_خانہ].گہرائی));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
خالی TExpr::compile()
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
    برائے (صحیح s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        اگر (a.قسم != CONST || a.گہرائی == EW_گہرائی_NONE) جاری;
        ثابت صحیح cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.گہرائی;
        ثابت size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        Auto_بفر<uint64_t, حجم(uint64_t)> srcbuf(div_اوپر((size_t)cn * sesz, (غیرعلامتی)حجم(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.ڈیٹا();
        حافظہ_نقل(srcbytes, (ثابت uchar*)(constbuf.ڈیٹا() + a.constofs), (size_t)cn * sesz);
        ثابت size_t ofs = appendConstBuf(constbuf, dd, خالی_اشاریہ, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.ڈیٹا() + ofs);
        اگر (sd == dd) حافظہ_نقل(dst, srcbytes, (size_t)cn * desz);
        ورنہ getConvertFunc(sd, dd)(srcbytes, 0, خالی_اشاریہ, 0, dst, 0, سائز(cn, 1), خالی_اشاریہ);
        a.constofs = ofs; a.srcdepth = dd;
        nconsts++;
    }

    // ---- liveness: pack temps into a minimal set of reusable physical buffers. A program with no
    //      temps (single-op add/sub/mul/...) needs none of this - early out with ZERO heap traffic,
    //      so building such a program (the dominant cv::add-style call) allocates nothing. The temp
    //      case uses stack-backed AutoBuffers (inline for typical small expressions). ----
    nbuffers = 0;
    buf_Esz_سابقہ.resize(1); buf_Esz_سابقہ[0] = 0;
    capElems = زیادہ_سے_زیادہ_عدد;                       // no temps => exec runs each tile in one block, no scratch
    اگر (ntemps == 0) واپس;

    Auto_بفر<صحیح, 32> عارضی_کا_خانہ(nslots);
    برائے (صحیح s = 0; s < nslots; s++) عارضی_کا_خانہ[s] = -1;
    برائے (صحیح s = 1; s < nslots; s++)
        اگر (arginfo[s].قسم == عارضی) عارضی_کا_خانہ[s] = arginfo[s].اشاریہ;

    Auto_بفر<صحیح, 16> آخری_Use(ntemps);
    برائے (صحیح t = 0; t < ntemps; t++) آخری_Use[t] = -1;
    برائے (صحیح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        صحیح as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        برائے (صحیح k = 0; k < 3; k++)
            اگر (عارضی_کا_خانہ[as[k]] >= 0) آخری_Use[عارضی_کا_خانہ[as[k]]] = i;
    }

    بفر_کا_عارضی.resize(ntemps);
    برائے (صحیح t = 0; t < ntemps; t++) بفر_کا_عارضی[t] = -1;
    Auto_بفر<صحیح, 16> freeBufs(ntemps);
    صحیح nfree = 0, nbuf = 0;
    برائے (صحیح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        صحیح rt = عارضی_کا_خانہ[ins.نتیجہ];
        اگر (rt >= 0 && بفر_کا_عارضی[rt] < 0)
            بفر_کا_عارضی[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        صحیح as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        برائے (صحیح k = 0; k < 3; k++)
        {
            صحیح t = عارضی_کا_خانہ[as[k]];
            اگر (t >= 0 && آخری_Use[t] == i)
            {
                freeBufs[nfree++] = بفر_کا_عارضی[t];
                آخری_Use[t] = -1;   // the same temp may be several args of this insn ("d*d") -
                                   // release its buffer once, or freeBufs overflows/double-frees
            }
        }
    }

    nbuffers = nbuf;

    // Byte layout of the physical temp buffers, per output element: prefix sums of each buffer's
    // max element size. bufEszPrefix[b]*region = buffer b's byte offset in the scratch; [nbuffers] =
    // total temp bytes/element. Precomputed here (depths are build-time) so exec()/its fast path
    // never recompute it per call, and the fast path can size its scratch in O(1).
    buf_Esz_سابقہ.resize(nbuffers + 1);
    برائے (صحیح b = 0; b <= nbuffers; b++) buf_Esz_سابقہ[b] = 0;
    برائے (صحیح s = 1; s < nslots; s++)
        اگر (arginfo[s].قسم == عارضی)
        {
            صحیح b = بفر_کا_عارضی[arginfo[s].اشاریہ], e = (صحیح)CV_ELEM_SIZE1(arginfo[s].گہرائی);
            اگر (e > buf_Esz_سابقہ[b + 1]) buf_Esz_سابقہ[b + 1] = e;     // max elem size in buffer b
        }
    برائے (صحیح b = 0; b < nbuffers; b++) buf_Esz_سابقہ[b + 1] += buf_Esz_سابقہ[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ثابت صحیح کل_Esz = buf_Esz_سابقہ[nbuffers];
    capElems = کل_Esz > 0 ? std::max(64, (4 * 1024) / کل_Esz) : زیادہ_سے_زیادہ_عدد;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
ساکن خالی matLogical(ثابت Mat& m, MatShape& shp, Ew_اقدام& قدم, صحیح& esz1)
{
    esz1 = (صحیح)m.elemSize1();
    صحیح nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    برائے (صحیح i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        قدم[i] = m.قدم[i] / esz1;
    }
    shp[nd] = cn;
    قدم[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
ساکن منطقی نشر_Shape(ثابت MatShape* shps, صحیح K, MatShape& out)
{
    صحیح nd = 0;
    برائے (صحیح k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    برائے (صحیح k = 0; k < K; k++)
    {
        ثابت MatShape& s = shps[k];
        صحیح off = nd - s.dims;
        برائے (صحیح i = 0; i < s.dims; i++)
        {
            صحیح d = s[i], &o = out[off + i];
            اگر (o == 1) o = d;
            ورنہ اگر (d != 1 && d != o) واپس جھوٹ;
        }
    }
    واپس سچ;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
ساکن صحیح op_لاگت(چوٹی op)
{
    انتخاب (op)
    {
    حالت OP_جوڑو: حالت OP_SUB: حالت OP_MUL: حالت OP_MIN: حالت OP_MAX:
    حالت OP_ABSDIFF: حالت OP_AND: حالت OP_OR: حالت OP_XOR: حالت OP_NOT:
    حالت OP_NEG: حالت OP_ABS: حالت OP_CAST: حالت OP_RELU: حالت OP_SELECT:
    حالت OP_CMP_EQ: حالت OP_CMP_NE: حالت OP_CMP_LT:
    حالت OP_CMP_LE: حالت OP_CMP_GT: حالت OP_CMP_GE: واپس 1;
    حالت OP_DIV: حالت OP_SQRT: حالت OP_HYPOT: حالت OP_CONVERT_SCALE: واپس 3;
    حالت OP_SIN: حالت OP_COS: حالت OP_TANH: حالت OP_ERF: حالت OP_ATAN2:
    حالت OP_EXP: حالت OP_LOG: حالت OP_POW:           واپس 6;
    طےشدہ:                                         واپس 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
خالی TExpr::clear()
{
    prog.allocate(0);
    arginfo.allocate(0);
    بفر_کا_عارضی.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
خالی TExpr::dump(std::ostream& os) ثابت
{
    خودکار dn = [](صحیح d) -> ثابت حرف* {
        انتخاب (d) {
        حالت EW_گہرائی_NONE: واپس "flex";
        حالت CV_8U:  واپس "u8";  حالت CV_8S:  واپس "s8";
        حالت CV_16U: واپس "u16"; حالت CV_16S: واپس "s16";
        حالت CV_32U: واپس "u32"; حالت CV_32S: واپس "s32";
        حالت CV_64U: واپس "u64"; حالت CV_64S: واپس "s64";
        حالت CV_16F: واپس "f16"; حالت CV_16BF:واپس "bf16";
        حالت CV_32F: واپس "f32"; حالت CV_64F: واپس "f64";
        حالت CV_Bool:واپس "bool";
        طےشدہ:     واپس "?"; }
    };
    خودکار kn = [](Arg_قسم k) -> ثابت حرف* {
        انتخاب (k) { حالت NONE: واپس "none"; حالت داخلہ: واپس "in"; حالت CONST: واپس "const";
                     حالت عارضی: واپس "temp"; حالت OUTPUT: واپس "out"; } واپس "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (صحیح)prog.size() << "\n";
    os << "  slots:\n";
    برائے (صحیح s = 0; s < (صحیح)arginfo.size(); s++) {
        ثابت Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.قسم);
        اگر (a.قسم != NONE) os << " " << dn(a.گہرائی);
        اگر (a.قسم == داخلہ || a.قسم == OUTPUT || a.قسم == عارضی) os << " idx=" << a.اشاریہ;
        اگر (a.قسم == CONST) {
            ثابت صحیح cn = std::max(1, a.channels);
            Auto_بفر<دوہرا, MAX_LOCAL_CN> v(cn);
            ثابت uchar* src = (ثابت uchar*)(constbuf.ڈیٹا() + a.constofs);
            اگر (a.srcdepth == CV_64F) حافظہ_نقل(v.ڈیٹا(), src, (size_t)cn * حجم(دوہرا));
            ورنہ getConvertFunc(a.srcdepth, CV_64F)(src, 0, خالی_اشاریہ, 0, (uchar*)v.ڈیٹا(), 0, سائز(cn, 1), خالی_اشاریہ);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            برائے (صحیح c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    برائے (صحیح i = 0; i < (صحیح)prog.size(); i++) {
        ثابت Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        اگر (ins.arg1) os << ", " << ins.arg1;
        اگر (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.نتیجہ;
        اگر (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        اگر (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        اگر (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
صحیح TExpr::جوڑو_داخلہ(صحیح گہرائی)
{
    Arg a; a.قسم = داخلہ; a.گہرائی = گہرائی; a.اشاریہ = ninputs++;
    arginfo.push_back(a); واپس (صحیح)arginfo.size() - 1;
}

صحیح TExpr::جوڑو_Output(صحیح گہرائی)
{
    Arg a; a.قسم = OUTPUT; a.گہرائی = گہرائی; a.اشاریہ = noutputs++;
    arginfo.push_back(a); واپس (صحیح)arginfo.size() - 1;
}

صحیح TExpr::جوڑو_عارضی(صحیح گہرائی)
{
    Arg a; a.قسم = عارضی; a.گہرائی = گہرائی; a.اشاریہ = ntemps++;
    arginfo.push_back(a); واپس (صحیح)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
صحیح TExpr::جوڑو_Const(صحیح گہرائی, ثابت Scalar& v, صحیح channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.قسم = CONST; a.گہرائی = گہرائی; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); واپس (صحیح)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
صحیح TExpr::جوڑو_Const(صحیح گہرائی, صحیح srcdepth, ثابت خالی* ڈیٹا, صحیح channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.قسم = CONST; a.گہرائی = گہرائی; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, ڈیٹا, channels);
    arginfo.push_back(a); واپس (صحیح)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
صحیح TExpr::typedConstFrom(صحیح src_خانہ, صحیح گہرائی)
{
    ثابت Arg& src = arginfo[src_خانہ];
    Arg a; a.قسم = CONST; a.گہرائی = گہرائی; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); واپس (صحیح)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
صحیح TExpr::جوڑو_Insn(چوٹی op, صحیح a0, صحیح a1, صحیح a2, صحیح نتیجہ, ثابت TKernel& kernel, ثابت Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.نتیجہ = نتیجہ;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    واپس (صحیح)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
صحیح TExpr::جوڑو_Insn(چوٹی op, صحیح a0, صحیح a1, صحیح a2, صحیح نتیجہ, ثابت Scalar& params)
{
    صحیح d0 = a0 ? arginfo[a0].گہرائی : EW_گہرائی_NONE;
    صحیح d1 = a1 ? arginfo[a1].گہرائی : EW_گہرائی_NONE;
    صحیح d2 = a2 ? arginfo[a2].گہرائی : EW_گہرائی_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[نتیجہ].گہرائی);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    واپس جوڑو_Insn(op, a0, a1, a2, نتیجہ, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
صحیح TExpr::maybe_جوڑو_Cast(صحیح arg, صحیح گہرائی)
{
    اگر (arginfo[arg].گہرائی == گہرائی) واپس arg;
    صحیح t = جوڑو_عارضی(گہرائی);
    جوڑو_Insn(OP_CAST, arg, 0, 0, t);
    واپس t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
ساکن اندرونی خالی تسلسل_Insn(ثابت TExpr::Insn& ins,
                           ثابت خالی* p0, size_t y0, size_t x0,
                           ثابت خالی* p1, size_t y1, size_t x1,
                           ثابت خالی* p2, size_t y2, size_t x2,
                           خالی* pr, size_t yr, صحیح w, صحیح h)
{
    صحیح رمز = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(رمز >= 0);
}

ساخت EwBody {
    ثابت TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ثابت صحیح* خانہ_Map;
    ثابت علامتی حرف* خانہ_قسم; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ثابت صحیح* buf_Esz_سابقہ;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    صحیح ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

خالی TExpr::outputShape(ثابت Mat* ثابت* inputs, MatShape& spatial, صحیح& channels) ثابت
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ثابت صحیح rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    منطقی sameShape = ninputs >= 1;
    برائے (صحیح i = 1; sameShape && i < ninputs; i++)
    {
        ثابت Mat& a = *inputs[i];
        اگر (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = جھوٹ;
        ورنہ برائے (صحیح d = 0; d < a.dims; d++) اگر (a.size[d] != inputs[0]->size[d]) { sameShape = جھوٹ; توڑو; }
    }
    اگر (sameShape) { spatial = inputs[0]->size; channels = rcn0; واپس; }

    Auto_بفر<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; Ew_اقدام قدم; صحیح esz1;
    برائے (صحیح i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, قدم, esz1); bshapes[i] = shp; }
    MatShape بھرا;
    CV_Assert(نشر_Shape(bshapes.ڈیٹا(), ninputs, بھرا) && "ew: inputs not broadcast-compatible");
    ثابت صحیح nd_بھرا = (صحیح)بھرا.size();
    channels = بھرا[nd_بھرا - 1];
    spatial = بھرا; spatial.resize(nd_بھرا - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
خالی TExpr::outputShape(ثابت Mat* inputs, MatShape& spatial, صحیح& channels) ثابت
{
    Auto_بفر<ثابت Mat*, 16> ptrs(std::max(ninputs, 1));
    برائے (صحیح i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.ڈیٹا(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ساکن constexpr صحیح LOCAL_HDRS = 3;
ساکن constexpr صحیح LOCAL_CONSTS = 4;
ساکن constexpr صحیح LOCAL_OPS = 16;

خالی TExpr::exec(ثابت Mat* ثابت* inputs, Mat* outputs)
{
    استعمال BrTile = نشر_Op::Tile;
    استعمال Br_قاش = نشر_Op::قاش;
    ثابت صحیح nslots = (صحیح)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].قسم == TExpr::NONE);

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
        برائے (صحیح i = 1; ok && i < ninputs; i++)
        {
            ثابت Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        برائے (صحیح s = 1; ok && s < nslots; s++)
            اگر (arginfo[s].قسم == OUTPUT) ok = ok && outputs[arginfo[s].اشاریہ].isContinuous();
        ثابت size_t کل = ok ? r.کل() * rcn : 0;
        اگر (ok && کل == 0)
        {
            ثابت MatShape rshape = r.size;
            برائے (صحیح s = 1; s < nslots; s++)
                اگر (arginfo[s].قسم == OUTPUT)
                    outputs[arginfo[s].اشاریہ].create(rshape, CV_MAKETYPE(arginfo[s].گہرائی, rcn));
            واپس;
        }
        اگر (ok && کل <= EW_FASTPATH_MAX)
        {
            ثابت MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            برائے (صحیح s = 1; s < nslots; s++)
                اگر (arginfo[s].قسم == OUTPUT)
                    outputs[arginfo[s].اشاریہ].create(rshape, CV_MAKETYPE(arginfo[s].گہرائی, rcn));
            ثابت صحیح ninsn = (صحیح)prog.size();

            اگر (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                خودکار ptr_کا = [&](صحیح s) -> خالی* {
                    اگر (s <= 0) واپس خالی_اشاریہ;
                    ثابت Arg& ai = arginfo[s];
                    اگر (ai.قسم == داخلہ)  واپس (خالی*)inputs[ai.اشاریہ]->ڈیٹا;
                    اگر (ai.قسم == OUTPUT) واپس (خالی*)outputs[ai.اشاریہ].ڈیٹا;
                    واپس خالی_اشاریہ;   // NONE
                };
                ثابت صحیح w = (صحیح)کل;
                برائے (صحیح n = 0; n < ninsn; n++)
                {
                    ثابت Insn& ins = prog[n];
                    تسلسل_Insn(ins, ptr_کا(ins.arg0), 0, 1, ptr_کا(ins.arg1), 0, 1,
                            ptr_کا(ins.arg2), 0, 1, ptr_کا(ins.نتیجہ), 0, w, 1);
                }
                واپس;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ثابت صحیح کل_Esz = buf_Esz_سابقہ[nbuffers];
            ثابت صحیح wf0 = std::min((صحیح)کل, capElems);

            ثابت size_t region = align_سائز((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            Auto_بفر<uint64_t, (4*1024 + 256)/حجم(uint64_t)> scratchBuf(
                div_اوپر((size_t)کل_Esz * region, (غیرعلامتی)حجم(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.ڈیٹا();
            برائے (صحیح x0 = 0; x0 < (صحیح)کل; x0 += wf0)
            {
                ثابت صحیح wf = std::min(wf0, (صحیح)کل - x0);
                خودکار ptr_کا = [&](صحیح s) -> خالی* {
                    اگر (s <= 0) واپس خالی_اشاریہ;
                    ثابت Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.گہرائی);
                    اگر (ai.قسم == داخلہ)  واپس (uchar*)inputs[ai.اشاریہ]->ڈیٹا  + (size_t)x0 * esz;
                    اگر (ai.قسم == OUTPUT) واپس (uchar*)outputs[ai.اشاریہ].ڈیٹا + (size_t)x0 * esz;
                    اگر (ai.قسم != عارضی)   واپس خالی_اشاریہ;                                    // NONE: moved-from
                    ثابت صحیح b = بفر_کا_عارضی.empty() ? ai.اشاریہ : بفر_کا_عارضی[ai.اشاریہ];  //عارضی
                    واپس scratch + (size_t)buf_Esz_سابقہ[b] * region;                        // fragment-local
                };
                برائے (صحیح n = 0; n < ninsn; n++)
                {
                    ثابت Insn& ins = prog[n];
                    تسلسل_Insn(ins, ptr_کا(ins.arg0), 0, 1, ptr_کا(ins.arg1), 0, 1,
                            ptr_کا(ins.arg2), 0, 1, ptr_کا(ins.نتیجہ), 0, wf, 1);
                }
            }
            واپس;
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
    منطقی inplace = جھوٹ;
    برائے (صحیح i = 0; i < ninputs && !inplace; i++)
        برائے (صحیح j = 0; j < noutputs; j++)
            اگر (inputs[i]->ڈیٹا && inputs[i]->ڈیٹا == outputs[j].ڈیٹا) { inplace = سچ; توڑو; }
    صحیح nhdrs = inplace ? ninputs : 0;

    Auto_بفر<ثابت Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    Auto_بفر<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    Auto_بفر<ثابت Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    Auto_بفر<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    Auto_بفر<صحیح, LOCAL_OPS> خانہ_Map(nslots);
    Auto_بفر<علامتی حرف, LOCAL_OPS> خانہ_قسم(nslots);
    صحیح narr = 0, nc = 0;
    اگر (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        برائے (صحیح j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.ڈیٹا();
    }
    خانہ_قسم[0] = (علامتی حرف)TExpr::NONE;
    برائے (صحیح s = 1; s < nslots; s++)
    {
        ثابت TExpr::Arg& ai = arginfo[s];
        خانہ_قسم[s] = (علامتی حرف)ai.قسم;
        اگر (ai.قسم == TExpr::داخلہ) {
            خانہ_Map[s] = narr;
            arr[narr++] = inputs[ai.اشاریہ];
        }
        ورنہ اگر (ai.قسم == TExpr::OUTPUT) {
            outputs[ai.اشاریہ].create(spatial, CV_MAKETYPE(ai.گہرائی, rchannels));
            خانہ_Map[s] = narr; arr[narr++] = &outputs[ai.اشاریہ];
        }
        ورنہ اگر (ai.قسم == TExpr::CONST && ai.گہرائی == EW_گہرائی_NONE) {
            خانہ_قسم[s] = (علامتی حرف)TExpr::NONE;   // dead flexible literal: never referenced
            خانہ_Map[s] = 0;
        }
        ورنہ اگر (ai.قسم == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ثابت صحیح c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.گہرائی, c),
                                  (خالی*)(constbuf.ڈیٹا() + ai.constofs));
            خانہ_قسم[s] = (علامتی حرف)TExpr::داخلہ;   // to the body it is just an array operand
            خانہ_Map[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        ورنہ اگر (ai.قسم == TExpr::عارضی) {
            خانہ_Map[s] = بفر_کا_عارضی.empty() ? ai.اشاریہ : بفر_کا_عارضی[ai.اشاریہ];  // physical buffer id
        }
        ورنہ {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            خانہ_Map[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ثابت صحیح ninsn = (صحیح)prog.size();
    طویل طویل لاگت_فی_Elem = 0;
    برائے (صحیح n = 0; n < ninsn; n++)
        لاگت_فی_Elem += op_لاگت(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    طویل طویل otot = (طویل طویل)rchannels;
    برائے (صحیح d = 0; d < (صحیح)spatial.size(); d++) otot *= spatial[d];
    اگر (otot == 0)
        واپس;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ثابت دوہرا T = (دوہرا)std::max(get_عدد_دھاگے(), 1);
    ثابت دوہرا nstripes = std::min(
        (دوہرا)otot * (دوہرا)std::max<طویل طویل>(لاگت_فی_Elem, 1) * (1./ (دوہرا)(1 << 16)),
        std::min(4.*T, std::max(32., 3.*T)));

    EwBody body;
    body.prog = prog.ڈیٹا();
    body.ninsn = ninsn;
    body.خانہ_Map = خانہ_Map.ڈیٹا();

    // ---- 5. drive: broadcastOp does geometry + 2D tiling + parallelism; the body runs the
    //         frozen program on each tile (temps tile-local, re-pointed from the tile slices).
    //         expandChannels=true => the body always sees single-channel data.
    //
    //         All state the body needs is packed into one POD (EwBody) so the lambda captures a
    //         SINGLE reference: the closure is then one pointer, fits std::function's small-buffer
    //         and never heap-allocates. Inside, the hot fields are copied into locals so the
    //         per-tile/per-insn loops read them from registers, not through the captured pointer. -

    body.buf_Esz_سابقہ = buf_Esz_سابقہ.ڈیٹا();
    body.خانہ_قسم = خانہ_قسم.ڈیٹا();
    body.nslots = nslots;
    body.nbuffers = nbuffers;
    body.capElems = capElems;

    نشر_Op_2(arr.ڈیٹا(), narr, [&](ثابت BrTile& tile)
    {
        EwBody& bc = body;
        ثابت TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ثابت صحیح* ثابت smap  = bc.خانہ_Map;
        ثابت علامتی حرف* ثابت skind = bc.خانہ_قسم;
        ثابت صحیح nin = bc.ninsn, nsl = bc.nslots;
        ثابت صحیح w = tile.چوڑائی, h = tile.اونچائی;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ثابت صحیح* ثابت esz_سابقہ = bc.buf_Esz_سابقہ;
        ثابت صحیح bw = std::min(w, bc.capElems);
        ثابت صحیح bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ثابت size_t region = align_سائز((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        Auto_بفر<uint64_t, (4*1024 + 256)/حجم(uint64_t)> tstoreBuf(
            div_اوپر((size_t)esz_سابقہ[bc.nbuffers] * region, (غیرعلامتی)حجم(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.ڈیٹا();

        Auto_بفر<Br_قاش, LOCAL_OPS> دلائل(nsl);
        برائے (صحیح y0 = 0; y0 < h; y0 += bh)
        {
            ثابت صحیح hf = std::min(bh, h - y0);
            برائے (صحیح x0 = 0; x0 < w; x0 += bw)
            {
                ثابت صحیح wf = std::min(bw, w - x0);
                برائے (صحیح s = 1; s < nsl; s++)
                {
                    Br_قاش& a = دلائل[s];
                    ثابت صحیح k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].گہرائی);
                    اگر (k == TExpr::عارضی)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)esz_سابقہ[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    ورنہ                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ثابت Br_قاش& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                برائے (صحیح n = 0; n < nin; n++)
                {
                    ثابت TExpr::Insn& ins = insns[n];
                    ثابت Br_قاش& a0 = دلائل[ins.arg0]; ثابت Br_قاش& a1 = دلائل[ins.arg1];
                    ثابت Br_قاش& a2 = دلائل[ins.arg2]; ثابت Br_قاش& rr = دلائل[ins.نتیجہ];
                    تسلسل_Insn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (خالی*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, سچ, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
خالی TExpr::exec(ثابت Mat* inputs, Mat* outputs)
{
    Auto_بفر<ثابت Mat*, 16> ptrs(std::max(ninputs, 1));
    برائے (صحیح i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    exec(ptrs.ڈیٹا(), outputs);
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
خالی make_ثنائی_Arith_پروگرام(TExpr& p, چوٹی op, صحیح depth0, صحیح depth1, صحیح rdepth,
                            صحیح نقاب_گہرائی, دوہرا scale)
{
    p.clear();
    اگر (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        اگر (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        ورنہ
        {
            rdepth = promoteArith(depth0, depth1);
            اگر (op == OP_ABSDIFF) rdepth = absdiff_نتیجہ_گہرائی(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ثابت منطقی masked = نقاب_گہرائی != EW_گہرائی_NONE;
    صحیح sIn0  = p.جوڑو_داخلہ(depth0);
    صحیح sIn1  = p.جوڑو_داخلہ(depth1);
    صحیح s_نقاب = masked ? p.جوڑو_داخلہ(نقاب_گہرائی) : 0;
    صحیح s_برآمد  = p.جوڑو_Output(rdepth);

    صحیح r = p.emit_ثنائی(op, sIn0, sIn1, rdepth, Scalar(scale));
    اگر (masked)
        p.جوڑو_Insn(OP_SELECT, s_نقاب, r, s_برآمد, s_برآمد);          // dst = mask ? r : dst
    ورنہ
        p.move_مقصد_Output(r, s_برآمد);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
خالی make_جوڑو_Weighted_پروگرام(TExpr& p, صحیح depth0, صحیح depth1, صحیح rdepth,
                            دوہرا alpha, دوہرا beta, دوہرا gamma)
{
    p.clear();
    صحیح sA   = p.جوڑو_داخلہ(depth0);
    صحیح sB   = p.جوڑو_داخلہ(depth1);
    صحیح s_برآمد = p.جوڑو_Output(rdepth);
    صحیح r = p.emit_ثنائی(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.move_مقصد_Output(r, s_برآمد);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

نامخلا {

// --- tokens ------------------------------------------------------------------------------
شمار TokType { T_عدد, T_داخلہ, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_تفویض, T_انتہا };

ساخت Token
{
    TokType type = T_انتہا;
    دوہرا num = 0;
    صحیح داخلہ_2 = 0;
    std::string متن;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
ساخت Lexer
{
    std::string_view s;
    size_t pos = 0;

    صریح Lexer(std::string_view src) : s(src) {}

    ساکن منطقی is_Ident_آغاز(حرف c) { واپس std::حرف_ہے((غیرعلامتی حرف)c) || c == '_'; }
    ساکن منطقی isIdentChar(حرف c)  { واپس std::حرف_یا_ہندسہ((غیرعلامتی حرف)c) || c == '_'; }

    Token next()
    {
        جب (pos < s.size() && std::خلا_ہے((غیرعلامتی حرف)s[pos])) pos++;
        Token t;
        اگر (pos >= s.size()) { t.type = T_انتہا; واپس t; }

        حرف c = s[pos];

        اگر (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t آغاز = pos;
            جب (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_داخلہ;
            t.داخلہ_2 = std::atoi(std::string(s.substr(آغاز, pos - آغاز)).c_str());
            pos++;                          // consume '}'
            واپس t;
        }
        اگر (std::ہندسہ_ہے((غیرعلامتی حرف)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::ہندسہ_ہے((غیرعلامتی حرف)s[pos + 1])))
        {
            حرف* end = خالی_اشاریہ;
            std::string num(s.substr(pos));
            t.type = T_عدد;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            واپس t;
        }
        اگر (is_Ident_آغاز(c))
        {
            size_t آغاز = pos;
            جب (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.متن.assign(s.substr(آغاز, pos - آغاز));
            واپس t;
        }
        انتخاب (c)
        {
        حالت '(': pos++; t.type = T_LPAREN; واپس t;
        حالت ')': pos++; t.type = T_RPAREN; واپس t;
        حالت ',': pos++; t.type = T_COMMA;  واپس t;
        حالت ';': pos++; t.type = T_SEMI;   واپس t;
        }
        // multi/!single-char operators
        خودکار two = [&](ثابت حرف* op) {
            واپس pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        اگر (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.متن.assign(s.substr(pos, 2)); pos += 2; واپس t; }
        اگر (c == '=') { pos++; t.type = T_تفویض; واپس t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.متن.assign(1, c); pos++;
        واپس t;
    }
};

// --- operator / function tables ----------------------------------------------------------
ساکن صحیح binPrec(ثابت std::string& op)
{
    اگر (op == "**") واپس 8;              // power binds tighter than '*'; RIGHT-associative
    اگر (op == "*" || op == "/") واپس 7;
    اگر (op == "+" || op == "-") واپس 6;
    اگر (op == "<" || op == "<=" || op == ">" || op == ">=") واپس 5;
    اگر (op == "==" || op == "!=") واپس 4;
    اگر (op == "&") واپس 3;
    اگر (op == "^") واپس 2;
    اگر (op == "|") واپس 1;
    واپس -1;
}

ساکن چوٹی binOp(ثابت std::string& op)
{
    اگر (op == "**") واپس OP_POW;
    اگر (op == "+")  واپس OP_جوڑو;
    اگر (op == "-")  واپس OP_SUB;
    اگر (op == "*")  واپس OP_MUL;
    اگر (op == "/")  واپس OP_DIV;
    اگر (op == "<")  واپس OP_CMP_LT;
    اگر (op == "<=") واپس OP_CMP_LE;
    اگر (op == ">")  واپس OP_CMP_GT;
    اگر (op == ">=") واپس OP_CMP_GE;
    اگر (op == "==") واپس OP_CMP_EQ;
    اگر (op == "!=") واپس OP_CMP_NE;
    اگر (op == "&")  واپس OP_AND;
    اگر (op == "|")  واپس OP_OR;
    اگر (op == "^")  واپس OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
ساکن صحیح type_گہرائی(ثابت std::string& name)
{
    اگر (name == "float")    واپس CV_32F;
    اگر (name == "double")   واپس CV_64F;
    اگر (name == "half" || name == "float16") واپس CV_16F;
    اگر (name == "bfloat16") واپس CV_16BF;
    اگر (name == "uint8")    واپس CV_8U;
    اگر (name == "int8")     واپس CV_8S;
    اگر (name == "uint16")   واپس CV_16U;
    اگر (name == "int16")    واپس CV_16S;
    اگر (name == "uint32")   واپس CV_32U;
    اگر (name == "int32")    واپس CV_32S;
    اگر (name == "uint64")   واپس CV_64U;
    اگر (name == "int64")    واپس CV_64S;
    واپس -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
ساکن چوٹی fnOp(ثابت std::string& name, صحیح& arity)
{
    ساخت E { ثابت حرف* n; چوٹی op; };
    ساکن ثابت E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    ساکن ثابت E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    ساکن ثابت E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    برائے (ثابت E& e : unary)  اگر (name == e.n) { arity = 1; واپس e.op; }
    برائے (ثابت E& e : binary) اگر (name == e.n) { arity = 2; واپس e.op; }
    برائے (ثابت E& e : tern)   اگر (name == e.n) { arity = 3; واپس e.op; }
    arity = 0; واپس OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
ساخت Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ثابت صحیح* داخلہ_خانہ;                 // slot id of each external input (precreated)
    صحیح ninputs;
    std::map<std::string, صحیح> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& اظہاریہ, ثابت صحیح* islot, صحیح nin)
        : lex(src), e(اظہاریہ), داخلہ_خانہ(islot), ninputs(nin) { cur = lex.next(); }

    خالی advance() { cur = lex.next(); }
    منطقی isOp(ثابت حرف* op) ثابت { واپس cur.type == T_OP && cur.متن == op; }

    خالی expect(TokType t, ثابت حرف* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    منطقی isFlexConst(صحیح خانہ) ثابت
    {
        واپس e.arginfo[خانہ].قسم == TExpr::CONST && e.arginfo[خانہ].گہرائی == EW_گہرائی_NONE;
    }

    صحیح parsePrimary()
    {
        اگر (cur.type == T_عدد)   { صحیح s = e.جوڑو_Const(EW_گہرائی_NONE, Scalar(cur.num), 1); advance(); واپس s; }
        اگر (cur.type == T_داخلہ) { صحیح idx = cur.داخلہ_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   واپس داخلہ_خانہ[idx]; }
        اگر (cur.type == T_LPAREN){ advance(); صحیح x = parseTernary(); expect(T_RPAREN, "expected ')'"); واپس x; }
        اگر (cur.type == T_IDENT)
        {
            std::string name = cur.متن; advance();
            اگر (cur.type != T_LPAREN)             // variable reference
            {
                خودکار it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                واپس it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<صحیح, 8> دلائل = {}; صحیح nargs = 0;
            اگر (cur.type != T_RPAREN)
            {
                دلائل[nargs++] = parseTernary();
                جب (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (صحیح)دلائل.size() && "ew::expression: too many arguments");
                    دلائل[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            صحیح td = type_گہرائی(name);
            اگر (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                صحیح s = دلائل[0];
                واپس isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybe_جوڑو_Cast(s, td);
            }

            صحیح arity = 0; چوٹی op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            اگر (arity == 1) واپس e.emitUnary(op, دلائل[0]);
            اگر (arity == 2) واپس e.emit_ثنائی(op, دلائل[0], دلائل[1]);
            واپس e.emitTernary(op, دلائل[0], دلائل[1], دلائل[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    صحیح parseUnary()
    {
        اگر (isOp("-") || isOp("!"))
        {
            std::string op = cur.متن; advance();
            صحیح عملیہ = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            اگر (op == "-" && isFlexConst(عملیہ))
            {
                Auto_بفر<دوہرا, MAX_LOCAL_CN> v; صحیح cn = constDoubles(e, عملیہ, v);
                Scalar neg; برائے (صحیح i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                واپس e.جوڑو_Const(EW_گہرائی_NONE, neg, std::min(cn, 4));
            }
            واپس e.emitUnary(op == "-" ? OP_NEG : OP_NOT, عملیہ);
        }
        واپس parsePrimary();
    }

    صحیح parse_اظہاریہ(صحیح minPrec)
    {
        صحیح بائیں = parseUnary();
        جب (cur.type == T_OP)
        {
            صحیح p = binPrec(cur.متن);
            اگر (p < minPrec) توڑو;
            std::string op = cur.متن; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            صحیح دائیں = parse_اظہاریہ(op == "**" ? p : p + 1);
            بائیں = e.emit_ثنائی(binOp(op), بائیں, دائیں);
        }
        واپس بائیں;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    صحیح parseTernary()
    {
        صحیح cond = parse_اظہاریہ(0);
        اگر (!isOp("?")) واپس cond;
        advance();
        صحیح thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        صحیح elsev = parseTernary();               // right-associative chain
        واپس e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    خالی parse_نتیجہ()
    {
        اگر (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); صحیح n_عارضی = e.ntemps;
            advance();
            صحیح e0 = parseTernary();
            اگر (cur.type == T_COMMA)
            {
                e.output(e0);
                جب (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                واپس;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = n_عارضی;
        }
        e.output(parseTernary());
    }

    خالی parse()
    {
        جب (سچ)
        {
            اگر (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.متن; advance();
                اگر (cur.type == T_تفویض)
                {
                    advance();
                    ثابت صحیح خانہ = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[خانہ].pinned = سچ;
                    env[name] = خانہ;
                    expect(T_SEMI, "expected ';' after assignment");
                    جاری;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parse_نتیجہ();
            اگر (cur.type == T_SEMI) advance();
            توڑو;
        }
        CV_Assert(cur.type == T_انتہا && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

نامخلا cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
خالی texpr(ثابت std::string& اظہاریہ, داخلہ_Array_کا_Arrays _inputs, Output_Array_کا_Arrays _outputs)
{
    استعمال نامخلا cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.قسم() == داخلہ_Array_2::STD_سمتیہ_MAT);
    ثابت std::vector<Mat>& inps = *(ثابت std::vector<Mat>*)_inputs.getObj();
    ثابت صحیح ninputs = (صحیح)inps.size();
    برائے (صحیح i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    Auto_بفر<صحیح> islot(std::max(ninputs, 1));
    برائے (صحیح i = 0; i < ninputs; i++) islot[i] = e.جوڑو_داخلہ(inps[i].گہرائی());
    Parser(اظہاریہ, e, islot.ڈیٹا(), ninputs).parse();
    e.compile();

    خودکار قسم = _outputs.قسم();
    اگر (قسم == داخلہ_Array_2::STD_سمتیہ_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.ڈیٹا(), outs.ڈیٹا());
    } ورنہ {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
