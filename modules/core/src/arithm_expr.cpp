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

ثابت حرف* opName(چوٹی op)
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
    حالة OP_جوڑو:           أرجع "add";
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
ElemwiseCategory opCategory(چوٹی op)
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
ساكن منطقي is_Float_ڈونگھائی(صحيح d)
{
    أرجع d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
ساكن منطقي ڈونگھائی_Represents(مزدوج v, صحيح ڈونگھائی)
{
    إذا (is_Float_ڈونگھائی(ڈونگھائی)) أرجع سچ;
    إذا (v != std::floor(v)) أرجع خطأ;
    بدل (ڈونگھائی)
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
ساكن فراغ intRange(صحيح ڈونگھائی, مزدوج& lo, مزدوج& hi)
{
    بدل (ڈونگھائی)
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
    constexpr بنانشانی signlut = 0b1001111110100u;
    constexpr بنانشانی fltlut  = 0b1111000000u;

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
        أرجع (ilut >> rw*5) & CV_MAT_ڈونگھائی_نقاب;
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
صحيح absdiff_نتیجہ_ڈونگھائی(صحيح ڈونگھائی)
{
    بدل (ڈونگھائی)
    {
    حالة CV_8S:  أرجع CV_8U;
    حالة CV_16S: أرجع CV_16U;
    حالة CV_32S: أرجع CV_32U;
    حالة CV_64S: أرجع CV_64U;
    افتراضي:     أرجع ڈونگھائی;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
ساكن صحيح promote2(صحيح a, صحيح b)
{
    إذا (a == b) أرجع a;

    constexpr بنانشانی lbits = 3, lmask = (1u << lbits) - 1u;
    ثابت uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    بنانشانی pr_a = بنانشانی((typelut >> (a*lbits)) & lmask);
    بنانشانی pr_b = بنانشانی((typelut >> (b*lbits)) & lmask);
    بنانشانی max_pr = std::max(pr_a, pr_b);
    constexpr بنانشانی dbits = CV_CN_کھسکاؤ, dmask = (1u << dbits) - 1u;
    ثابت بنانشانی ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
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
    إذا (a == EW_ڈونگھائی_NONE) أرجع b;
    إذا (b == EW_ڈونگھائی_NONE) أرجع a;
    إذا (a == b) أرجع a;
    أرجع CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
ساكن صحيح محفوظ_Wide(صحيح ڈونگھائی)
{
    constexpr بنانشانی lbits = 3, lmask = (1u << lbits) - 1u;
    ثابت uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    بنانشانی pr_ڈونگھائی = بنانشانی((typelut >> (ڈونگھائی*lbits)) & lmask);
    constexpr بنانشانی dbits = CV_CN_کھسکاؤ, dmask = (1u << dbits) - 1u;
    ثابت بنانشانی ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    أرجع صحيح((ctypelut >> (pr_ڈونگھائی*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
تعداد { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
ساكن size_t appendConstBuf(Auto_بفر<uint64_t, 16>& constbuf, صحيح srcdepth, ثابت فراغ* انکڑے, صحيح channels)
{
    ثابت صحيح cn = std::max(1, channels);
    ثابت size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ثابت size_t nu64 = (nbytes + حجم(uint64_t) - 1) / حجم(uint64_t);
    ثابت size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.انکڑے() + ofs);
    إذا (انکڑے) حافظہ_نقل(dst, انکڑے, nbytes); وإلا memset(dst, 0, nbytes);
    أرجع ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
ساكن صحيح constDoubles(ثابت TExpr& e, صحيح s, Auto_بفر<مزدوج, MAX_LOCAL_CN>& buf)
{
    ثابت TExpr::Arg& a = e.arginfo[s];
    ثابت صحيح cn = std::max(1, a.channels);
    buf.resize(cn);
    ثابت uchar* src = (ثابت uchar*)(e.constbuf.انکڑے() + a.constofs);
    إذا (a.srcdepth == CV_64F)
        حافظہ_نقل(buf.انکڑے(), src, (size_t)cn * حجم(مزدوج));   // common (Scalar / parsed literal)
    وإلا
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, خالی_سوچک, 0, (uchar*)buf.انکڑے(), 0, ناپ(cn, 1), خالی_سوچک);
    أرجع cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
ساكن مضمن منطقي isFlexConst(ثابت TExpr& e, صحيح s)
{
    أرجع e.arginfo[s].قسم == TExpr::CONST && e.arginfo[s].ڈونگھائی == EW_ڈونگھائی_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
ساكن منطقي scalar_Const_قدر(ثابت TExpr& e, صحيح s, مزدوج& v)
{
    ثابت TExpr::Arg& a = e.arginfo[s];
    إذا (a.قسم != TExpr::CONST || std::max(1, a.channels) != 1) أرجع خطأ;
    Auto_بفر<مزدوج, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    أرجع سچ;
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
ساكن منطقي میل_Scaled_Chain(ثابت TExpr& e, صحيح end, صحيح t,
                             صحيح& arr_تھاں, مزدوج& scale, صحيح& شروع_Idx)
{
    إذا (end < 0) أرجع خطأ;
    صحيح j = end, mul_نتیجہ = t;

    // optional trailing narrowing cast
    إذا (e.prog[j].op == OP_CAST && e.prog[j].نتیجہ == t)
    {
        mul_نتیجہ = e.prog[j].arg0;
        إذا (--j < 0) أرجع خطأ;
    }

    ثابت TExpr::Insn& mul = e.prog[j];
    إذا (mul.op != OP_MUL || mul.نتیجہ != mul_نتیجہ || mul.params[0] != 1.0) أرجع خطأ;

    صحيح arr;
    إذا      (scalar_Const_قدر(e, mul.arg1, scale)) arr = mul.arg0;
    وإلا إذا (scalar_Const_قدر(e, mul.arg0, scale)) arr = mul.arg1;
    وإلا أرجع خطأ;                                  // array*array is not an addWeighted term
    شروع_Idx = j;

    // optional leading widening cast feeding the multiply
    إذا (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].نتیجہ == arr &&
        e.arginfo[arr].قسم == TExpr::عارضی)
    {
        arr = e.prog[j-1].arg0;
        شروع_Idx = j - 1;
    }

    إذا (e.arginfo[arr].قسم == TExpr::CONST) أرجع خطأ;   // scalar*scalar is folded elsewhere
    arr_تھاں = arr;
    أرجع سچ;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
ساكن منطقي const_سماندا(ثابت TExpr& e, صحيح s, صحيح d)
{
    إذا (!isFlexConst(e, s)) أرجع سچ;
    Auto_بفر<مزدوج, MAX_LOCAL_CN> v;
    صحيح cn = constDoubles(e, s, v);
    لكل (صحيح ch = 0; ch < cn; ch++)
        إذا (!ڈونگھائی_Represents(v[ch], d)) أرجع خطأ;
    أرجع سچ;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
صحيح TExpr::emit_ثنائی(چوٹی op, صحيح a, صحيح b, صحيح rdepth, ثابت Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    إذا (op == OP_ADDW)
    {
        صحيح Tt = arginfo[a].ڈونگھائی;
        إذا (arginfo[a].ڈونگھائی != arginfo[b].ڈونگھائی)
        {
            Tt = promoteArith(arginfo[a].ڈونگھائی, arginfo[b].ڈونگھائی);
            a = maybe_جوڑو_Cast(a, Tt); b = maybe_جوڑو_Cast(b, Tt);
        }
        إذا (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        إذا (rdepth == EW_ڈونگھائی_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_ڈونگھائی_NONE, rdepth);
        صحيح باہرلا_D = rdepth;
        إذا (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            باہرلا_D = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_ڈونگھائی_NONE, باہرلا_D);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ثابت صحيح out = جوڑو_عارضی(باہرلا_D);
        جوڑو_Insn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        إذا (باہرلا_D == rdepth) أرجع out;
        ثابت صحيح out2 = جوڑو_عارضی(rdepth);
        جوڑو_Insn(OP_CAST, out, 0, 0, out2);
        أرجع out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    إذا (op == OP_جوڑو)
    {
        ثابت صحيح n = (صحيح)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        صحيح x = 0, y = 0, شروع_A = 0, شروع_B = 0;
        مزدوج alpha = 0, beta = 0;
        إذا (n >= 2 && a != b &&
            arginfo[a].قسم == عارضی && !arginfo[a].pinned &&
            arginfo[b].قسم == عارضی && !arginfo[b].pinned &&
            میل_Scaled_Chain(*هذا, n - 1, b, y, beta, شروع_B) &&
            میل_Scaled_Chain(*هذا, شروع_B - 1, a, x, alpha, شروع_A) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].ڈونگھائی != CV_Bool && arginfo[y].ڈونگھائی != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            صحيح retired = 0, lowest = ودھ_توں_ودھ_عدد;
            لكل (صحيح i = شروع_A; i < n; i++)
            {
                ثابت صحيح r = prog[i].نتیجہ;
                إذا (arginfo[r].قسم != عارضی || arginfo[r].pinned) { retired = -1; اكسر; }
                lowest = std::min(lowest, arginfo[r].اشاریہ);
                retired++;
            }
            إذا (retired > 0 && lowest == ntemps - retired)
            {
                لكل (صحيح i = شروع_A; i < n; i++) arginfo[prog[i].نتیجہ].قسم = NONE;
                prog.resize(شروع_A);
                ntemps -= retired;
                أرجع emit_ثنائی(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            صحيح acc = 0; مزدوج gamma = 0;
            إذا      (scalar_Const_قدر(*هذا, b, gamma)) acc = a;
            وإلا إذا (scalar_Const_قدر(*هذا, a, gamma)) acc = b;
            إذا (acc != 0 && n >= 1 &&
                arginfo[acc].قسم == عارضی && !arginfo[acc].pinned &&
                arginfo[acc].اشاریہ == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].نتیجہ == acc &&
                (rdepth == EW_ڈونگھائی_NONE || rdepth == arginfo[acc].ڈونگھائی))
            {
                prog[n-1].params[2] += gamma;
                أرجع acc;
            }
        }
    }

    ثابت صحيح nd0 = isFlexConst(*هذا, a) ? EW_ڈونگھائی_NONE : arginfo[a].ڈونگھائی;
    ثابت صحيح nd1 = isFlexConst(*هذا, b) ? EW_ڈونگھائی_NONE : arginfo[b].ڈونگھائی;
    ثابت ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    صحيح نتیجہ = rdepth;
    إذا (نتیجہ == EW_ڈونگھائی_NONE)
    {
        نتیجہ = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        إذا (نتیجہ == EW_ڈونگھائی_NONE) نتیجہ = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    صحيح ادھار;
    إذا (nd0 == EW_ڈونگھائی_NONE && nd1 == EW_ڈونگھائی_NONE) ادھار = نتیجہ;
    وإلا إذا (nd0 == EW_ڈونگھائی_NONE) ادھار = nd1;
    وإلا إذا (nd1 == EW_ڈونگھائی_NONE) ادھار = nd0;
    وإلا ادھار = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    إذا (cat == CAT_COMPARE && nd0 != EW_ڈونگھائی_NONE && !is_Float_ڈونگھائی(nd0) &&
        isFlexConst(*هذا, b) && !const_سماندا(*هذا, b, ادھار))
    {
        مزدوج lo, hi; intRange(nd0, lo, hi);
        إذا (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            تعداد { REAL, CFALSE, CTRUE };
            Auto_بفر<مزدوج, MAX_LOCAL_CN> tv; صحيح cn = constDoubles(*هذا, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            صحيح قسم[4] = {}; مزدوج bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ثابت چوٹی fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
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
                حالة OP_CMP_EQ: k = ڈونگھائی_Represents(t, nd0) ? REAL : CFALSE; اكسر;   // a==non-rep -> false
                افتراضي:        k = ڈونگھائی_Represents(t, nd0) ? REAL : CTRUE;  اكسر;   // NE: a!=non-rep -> true
                }
                قسم[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            خودکار ucOp  = [&](صحيح c){ أرجع قسم[c]==REAL ? fam : قسم[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            خودکار ucThr = [&](صحيح c){ أرجع قسم[c]==REAL ? bound[c]
                                          : (قسم[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ثابت چوٹی u0 = ucOp(0);
            منطقي اکسار = سچ;
            لكل (صحيح c = 1; c < cn && اکسار; c++) اکسار = (ucOp(c) == u0);
            إذا (اکسار)
            {
                Scalar s; لكل (صحيح c = 0; c < cn; c++) s[c] = ucThr(c);
                b = جوڑو_Const(EW_ڈونگھائی_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
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
                    ts[c] = قسم[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ثابت صحيح mbits = قسم[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ثابت صحيح vbits = قسم[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_کھسکاؤ + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, ادھار, ادھار, EW_ڈونگھائی_NONE, نتیجہ);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ثابت صحيح out = جوڑو_عارضی(نتیجہ);
                جوڑو_Insn(fam, a, جوڑو_Const(ادھار, ts, cn), 0, out, kern);
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
    صحيح ڈونگھائی, wide;
    منطقي divGuard = خطأ;
    بدل (op)
    {
    حالة OP_MIN: حالة OP_MAX:
    حالة OP_AND: حالة OP_OR: حالة OP_XOR:
        ڈونگھائی = نتیجہ; wide = نتیجہ; اكسر;
    حالة OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        صحيح common = (nd0 == EW_ڈونگھائی_NONE) ? nd1
                   : (nd1 == EW_ڈونگھائی_NONE) ? nd0 : promoteArith(nd0, nd1);
        إذا (common == EW_ڈونگھائی_NONE) common = نتیجہ;          // both operands flexible consts
        ڈونگھائی = common;
        wide  = absdiff_نتیجہ_ڈونگھائی(common);
        إذا (rdepth == EW_ڈونگھائی_NONE) نتیجہ = wide;            // auto result: unsigned same width
        اكسر;
    }
    حالة OP_CMP_EQ: حالة OP_CMP_NE: حالة OP_CMP_LT:
    حالة OP_CMP_LE: حالة OP_CMP_GT: حالة OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        ڈونگھائی = ادھار; wide = CV_64F; اكسر;
    حالة OP_MUL: حالة OP_DIV:
    {
        ڈونگھائی = ادھار;
        ثابت منطقي w = (ڈونگھائی==CV_32U || ڈونگھائی==CV_32S || ڈونگھائی==CV_64U ||
                        ڈونگھائی==CV_64S || ڈونگھائی==CV_64F);
        wide = w ? CV_64F : CV_32F;
        إذا (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            خودکار int_عملیہ = [&](صحيح s, صحيح nd) {
                إذا (nd != EW_ڈونگھائی_NONE) أرجع !is_Float_ڈونگھائی(nd);
                Auto_بفر<مزدوج, MAX_LOCAL_CN> v; constDoubles(*هذا, s, v);
                أرجع v[0] == std::floor(v[0]); };
            divGuard = int_عملیہ(a, nd0) && int_عملیہ(b, nd1);
        }
        اكسر;
    }
    حالة OP_POW: حالة OP_HYPOT: حالة OP_ATAN2:
        ڈونگھائی = (ادھار == CV_64F) ? CV_64F : CV_32F; wide = ڈونگھائی; اكسر;
    افتراضي:        // OP_ADD, OP_SUB
        ڈونگھائی = ادھار; wide = محفوظ_Wide(ڈونگھائی); اكسر;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    خودکار fracConst = [&](صحيح s) {
        إذا (!isFlexConst(*هذا, s)) أرجع خطأ;
        Auto_بفر<مزدوج, MAX_LOCAL_CN> v; صحيح n = constDoubles(*هذا, s, v);
        لكل (صحيح ch = 0; ch < n; ch++)
            إذا (v[ch] != std::floor(v[ch])) أرجع سچ;
        أرجع خطأ;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    إذا (cat != CAT_BITWISE && (!const_سماندا(*هذا, a, ڈونگھائی) || !const_سماندا(*هذا, b, ڈونگھائی)))
    {
        ڈونگھائی = wide;
        ثابت منطقي frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        إذا (frac && !is_Float_ڈونگھائی(ڈونگھائی) && op != OP_MIN && op != OP_MAX)
            ڈونگھائی = wide = (نتیجہ == CV_64F || ادھار == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        إذا (frac && (op == OP_MUL || op == OP_DIV) && ڈونگھائی == CV_32F)
            ڈونگھائی = wide = CV_64F;
    }

    صحيح s0 = isFlexConst(*هذا, a) ? typedConstFrom(a, ڈونگھائی) : a;
    صحيح s1 = isFlexConst(*هذا, b) ? typedConstFrom(b, ڈونگھائی) : b;
    صحيح c0 = maybe_جوڑو_Cast(s0, ڈونگھائی), c1 = maybe_جوڑو_Cast(s1, ڈونگھائی);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    خودکار resolve = [&](صحيح rd) {
        أرجع op == OP_DIV ? getDivFunc(ڈونگھائی, rd, divGuard)
                            : getElemwiseFunc(op, ڈونگھائی, ڈونگھائی, EW_ڈونگھائی_NONE, rd);
    };
    TKernel k = resolve(نتیجہ);
    إذا (k.fptr)
    {
        صحيح res = جوڑو_عارضی(نتیجہ);
        جوڑو_Insn(op, c0, c1, 0, res, k, params);
        أرجع res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحيح w = جوڑو_عارضی(wide);
    جوڑو_Insn(op, c0, c1, 0, w, k, params);
    أرجع maybe_جوڑو_Cast(w, نتیجہ);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
صحيح TExpr::emitUnary(چوٹی op, صحيح a, صحيح rdepth, ثابت Scalar& params)
{
    ثابت صحيح nd = isFlexConst(*هذا, a) ? EW_ڈونگھائی_NONE : arginfo[a].ڈونگھائی;
    ثابت ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    إذا (op == OP_NEG)
        أرجع emit_ثنائی(OP_SUB, جوڑو_Const(EW_ڈونگھائی_NONE, Scalar(0.), 1), a, rdepth, params);
    إذا (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        إذا (!prog.empty() && arginfo[a].قسم == عارضی && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().نتیجہ == a &&
            arginfo[a].اشاریہ == ntemps - 1)
        {
            ثابت صحيح x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].قسم = NONE;
            ntemps--;
            أرجع emit_ثنائی(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        أرجع emit_ثنائی(OP_ABSDIFF, a, جوڑو_Const(EW_ڈونگھائی_NONE, Scalar(0.), 1), rdepth, params);
    }

    صحيح نتیجہ = rdepth;
    إذا (نتیجہ == EW_ڈونگھائی_NONE)
    {
        بدل (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        حالة CAT_MATH:    نتیجہ = is_Float_ڈونگھائی(nd) ? nd : CV_32F; اكسر;
        حالة CAT_BITWISE: نتیجہ = (nd == EW_ڈونگھائی_NONE) ? CV_32S : nd; اكسر;  // NOT
        افتراضي:          نتیجہ = (nd == EW_ڈونگھائی_NONE) ? CV_32F : nd; اكسر;  // NEG, ABS
        }
    }

    صحيح ڈونگھائی, wide;
    بدل (cat)
    {
    حالة CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        ڈونگھائی = (is_Float_ڈونگھائی(nd) && nd == نتیجہ) ? nd
              : (nd == CV_64F || نتیجہ == CV_64F) ? CV_64F : CV_32F;
        wide = ڈونگھائی;
        اكسر;
    حالة CAT_BITWISE: ڈونگھائی = نتیجہ; wide = نتیجہ; اكسر;
    افتراضي:          ڈونگھائی = (nd == EW_ڈونگھائی_NONE) ? نتیجہ : nd; wide = نتیجہ; اكسر;  // NEG/ABS
    }

    إذا (!const_سماندا(*هذا, a, ڈونگھائی)) ڈونگھائی = (cat == CAT_MATH) ? ڈونگھائی : CV_32F;
    صحيح s0 = isFlexConst(*هذا, a) ? typedConstFrom(a, ڈونگھائی) : a;
    صحيح c0 = maybe_جوڑو_Cast(s0, ڈونگھائی);

    TKernel k = getElemwiseFunc(op, ڈونگھائی, EW_ڈونگھائی_NONE, EW_ڈونگھائی_NONE, نتیجہ);
    إذا (k.fptr)
    {
        صحيح res = جوڑو_عارضی(نتیجہ);
        جوڑو_Insn(op, c0, 0, 0, res, k, params);
        أرجع res;
    }
    k = getElemwiseFunc(op, ڈونگھائی, EW_ڈونگھائی_NONE, EW_ڈونگھائی_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحيح w = جوڑو_عارضی(wide);
    جوڑو_Insn(op, c0, 0, 0, w, k, params);
    أرجع maybe_جوڑو_Cast(w, نتیجہ);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
صحيح TExpr::emitTernary(چوٹی op, صحيح a, صحيح b, صحيح c, صحيح rdepth)
{
    خودکار dep = [&](صحيح s) { أرجع isFlexConst(*هذا, s) ? EW_ڈونگھائی_NONE : arginfo[s].ڈونگھائی; };

    إذا (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        إذا (isFlexConst(*هذا, a) || CV_ELEM_SIZE1(arginfo[a].ڈونگھائی) != 1)
            a = emit_ثنائی(OP_CMP_NE, a, جوڑو_Const(EW_ڈونگھائی_NONE, Scalar(0.), 1), CV_8U, Scalar());
        صحيح نتیجہ = rdepth != EW_ڈونگھائی_NONE ? rdepth : promoteArith(dep(b), dep(c));
        إذا (نتیجہ == EW_ڈونگھائی_NONE) نتیجہ = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        صحيح sb = isFlexConst(*هذا, b) ? typedConstFrom(b, نتیجہ) : maybe_جوڑو_Cast(b, نتیجہ);
        صحيح sc = isFlexConst(*هذا, c) ? typedConstFrom(c, نتیجہ) : maybe_جوڑو_Cast(c, نتیجہ);
        صحيح res = جوڑو_عارضی(نتیجہ);
        جوڑو_Insn(OP_SELECT, a, sb, sc, res);
        أرجع res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    صحيح نتیجہ = rdepth != EW_ڈونگھائی_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    إذا (نتیجہ == EW_ڈونگھائی_NONE) نتیجہ = CV_32F;
    خودکار typed = [&](صحيح s) {
        أرجع isFlexConst(*هذا, s) ? typedConstFrom(s, نتیجہ) : maybe_جوڑو_Cast(s, نتیجہ);
    };
    صحيح c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, نتیجہ, نتیجہ, نتیجہ, نتیجہ);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    صحيح res = جوڑو_عارضی(نتیجہ);
    جوڑو_Insn(op, c0, c1, c2, res, k);
    أرجع res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
صحيح TExpr::move_منزل_Output(صحيح عارضی_2, صحيح out)
{
    ثابت صحيح ninsn = (صحيح)prog.size();
    صحيح پیداکار = -1;
    منطقي ورتیا_As_Arg = خطأ;
    لكل (صحيح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        إذا (ins.نتیجہ == عارضی_2) پیداکار = i;
        إذا (ins.arg0 == عارضی_2 || ins.arg1 == عارضی_2 || ins.arg2 == عارضی_2) ورتیا_As_Arg = سچ;
    }
    إذا (arginfo[عارضی_2].قسم == عارضی && پیداکار >= 0 && !ورتیا_As_Arg && !arginfo[عارضی_2].pinned &&
        arginfo[عارضی_2].ڈونگھائی == arginfo[out].ڈونگھائی)
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
        أرجع out;
    }
    جوڑو_Insn(OP_CAST, عارضی_2, 0, 0, out);                     // same/different depth copy into `out`
    أرجع out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
صحيح TExpr::output(صحيح جڑ_تھاں)
{
    أرجع move_منزل_Output(جڑ_تھاں, جوڑو_Output(arginfo[جڑ_تھاں].ڈونگھائی));
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
        إذا (a.قسم != CONST || a.ڈونگھائی == EW_ڈونگھائی_NONE) استمر;
        ثابت صحيح cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.ڈونگھائی;
        ثابت size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        Auto_بفر<uint64_t, حجم(uint64_t)> srcbuf(div_اتے((size_t)cn * sesz, (بنانشانی)حجم(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.انکڑے();
        حافظہ_نقل(srcbytes, (ثابت uchar*)(constbuf.انکڑے() + a.constofs), (size_t)cn * sesz);
        ثابت size_t ofs = appendConstBuf(constbuf, dd, خالی_سوچک, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.انکڑے() + ofs);
        إذا (sd == dd) حافظہ_نقل(dst, srcbytes, (size_t)cn * desz);
        وإلا getConvertFunc(sd, dd)(srcbytes, 0, خالی_سوچک, 0, dst, 0, ناپ(cn, 1), خالی_سوچک);
        a.constofs = ofs; a.srcdepth = dd;
        nconsts++;
    }

    // ---- liveness: pack temps into a minimal set of reusable physical buffers. A program with no
    //      temps (single-op add/sub/mul/...) needs none of this - early out with ZERO heap traffic,
    //      so building such a program (the dominant cv::add-style call) allocates nothing. The temp
    //      case uses stack-backed AutoBuffers (inline for typical small expressions). ----
    nbuffers = 0;
    buf_Esz_اگیتر.resize(1); buf_Esz_اگیتر[0] = 0;
    capElems = ودھ_توں_ودھ_عدد;                       // no temps => exec runs each tile in one block, no scratch
    إذا (ntemps == 0) أرجع;

    Auto_بفر<صحيح, 32> عارضی_دا_تھاں(nslots);
    لكل (صحيح s = 0; s < nslots; s++) عارضی_دا_تھاں[s] = -1;
    لكل (صحيح s = 1; s < nslots; s++)
        إذا (arginfo[s].قسم == عارضی) عارضی_دا_تھاں[s] = arginfo[s].اشاریہ;

    Auto_بفر<صحيح, 16> آخری_Use(ntemps);
    لكل (صحيح t = 0; t < ntemps; t++) آخری_Use[t] = -1;
    لكل (صحيح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        صحيح as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        لكل (صحيح k = 0; k < 3; k++)
            إذا (عارضی_دا_تھاں[as[k]] >= 0) آخری_Use[عارضی_دا_تھاں[as[k]]] = i;
    }

    بفر_دا_عارضی.resize(ntemps);
    لكل (صحيح t = 0; t < ntemps; t++) بفر_دا_عارضی[t] = -1;
    Auto_بفر<صحيح, 16> freeBufs(ntemps);
    صحيح nfree = 0, nbuf = 0;
    لكل (صحيح i = 0; i < ninsn; i++)
    {
        ثابت TExpr::Insn& ins = prog[i];
        صحيح rt = عارضی_دا_تھاں[ins.نتیجہ];
        إذا (rt >= 0 && بفر_دا_عارضی[rt] < 0)
            بفر_دا_عارضی[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        صحيح as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        لكل (صحيح k = 0; k < 3; k++)
        {
            صحيح t = عارضی_دا_تھاں[as[k]];
            إذا (t >= 0 && آخری_Use[t] == i)
            {
                freeBufs[nfree++] = بفر_دا_عارضی[t];
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
    buf_Esz_اگیتر.resize(nbuffers + 1);
    لكل (صحيح b = 0; b <= nbuffers; b++) buf_Esz_اگیتر[b] = 0;
    لكل (صحيح s = 1; s < nslots; s++)
        إذا (arginfo[s].قسم == عارضی)
        {
            صحيح b = بفر_دا_عارضی[arginfo[s].اشاریہ], e = (صحيح)CV_ELEM_SIZE1(arginfo[s].ڈونگھائی);
            إذا (e > buf_Esz_اگیتر[b + 1]) buf_Esz_اگیتر[b + 1] = e;     // max elem size in buffer b
        }
    لكل (صحيح b = 0; b < nbuffers; b++) buf_Esz_اگیتر[b + 1] += buf_Esz_اگیتر[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ثابت صحيح کل_Esz = buf_Esz_اگیتر[nbuffers];
    capElems = کل_Esz > 0 ? std::max(64, (4 * 1024) / کل_Esz) : ودھ_توں_ودھ_عدد;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
ساكن فراغ matLogical(ثابت Mat& m, MatShape& shp, Ew_قدماں& قدم, صحيح& esz1)
{
    esz1 = (صحيح)m.elemSize1();
    صحيح nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    لكل (صحيح i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        قدم[i] = m.قدم[i] / esz1;
    }
    shp[nd] = cn;
    قدم[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
ساكن منطقي نشر_Shape(ثابت MatShape* shps, صحيح K, MatShape& out)
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
    أرجع سچ;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
ساكن صحيح op_لاگت(چوٹی op)
{
    بدل (op)
    {
    حالة OP_جوڑو: حالة OP_SUB: حالة OP_MUL: حالة OP_MIN: حالة OP_MAX:
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
    بفر_دا_عارضی.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
فراغ TExpr::dump(std::ostream& os) ثابت
{
    خودکار dn = [](صحيح d) -> ثابت حرف* {
        بدل (d) {
        حالة EW_ڈونگھائی_NONE: أرجع "flex";
        حالة CV_8U:  أرجع "u8";  حالة CV_8S:  أرجع "s8";
        حالة CV_16U: أرجع "u16"; حالة CV_16S: أرجع "s16";
        حالة CV_32U: أرجع "u32"; حالة CV_32S: أرجع "s32";
        حالة CV_64U: أرجع "u64"; حالة CV_64S: أرجع "s64";
        حالة CV_16F: أرجع "f16"; حالة CV_16BF:أرجع "bf16";
        حالة CV_32F: أرجع "f32"; حالة CV_64F: أرجع "f64";
        حالة CV_Bool:أرجع "bool";
        افتراضي:     أرجع "?"; }
    };
    خودکار kn = [](Arg_قسم k) -> ثابت حرف* {
        بدل (k) { حالة NONE: أرجع "none"; حالة داخلہ: أرجع "in"; حالة CONST: أرجع "const";
                     حالة عارضی: أرجع "temp"; حالة OUTPUT: أرجع "out"; } أرجع "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (صحيح)prog.size() << "\n";
    os << "  slots:\n";
    لكل (صحيح s = 0; s < (صحيح)arginfo.size(); s++) {
        ثابت Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.قسم);
        إذا (a.قسم != NONE) os << " " << dn(a.ڈونگھائی);
        إذا (a.قسم == داخلہ || a.قسم == OUTPUT || a.قسم == عارضی) os << " idx=" << a.اشاریہ;
        إذا (a.قسم == CONST) {
            ثابت صحيح cn = std::max(1, a.channels);
            Auto_بفر<مزدوج, MAX_LOCAL_CN> v(cn);
            ثابت uchar* src = (ثابت uchar*)(constbuf.انکڑے() + a.constofs);
            إذا (a.srcdepth == CV_64F) حافظہ_نقل(v.انکڑے(), src, (size_t)cn * حجم(مزدوج));
            وإلا getConvertFunc(a.srcdepth, CV_64F)(src, 0, خالی_سوچک, 0, (uchar*)v.انکڑے(), 0, ناپ(cn, 1), خالی_سوچک);
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
        os << ") -> " << ins.نتیجہ;
        إذا (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        إذا (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        إذا (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
صحيح TExpr::جوڑو_داخلہ(صحيح ڈونگھائی)
{
    Arg a; a.قسم = داخلہ; a.ڈونگھائی = ڈونگھائی; a.اشاریہ = ninputs++;
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

صحيح TExpr::جوڑو_Output(صحيح ڈونگھائی)
{
    Arg a; a.قسم = OUTPUT; a.ڈونگھائی = ڈونگھائی; a.اشاریہ = noutputs++;
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

صحيح TExpr::جوڑو_عارضی(صحيح ڈونگھائی)
{
    Arg a; a.قسم = عارضی; a.ڈونگھائی = ڈونگھائی; a.اشاریہ = ntemps++;
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
صحيح TExpr::جوڑو_Const(صحيح ڈونگھائی, ثابت Scalar& v, صحيح channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.قسم = CONST; a.ڈونگھائی = ڈونگھائی; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
صحيح TExpr::جوڑو_Const(صحيح ڈونگھائی, صحيح srcdepth, ثابت فراغ* انکڑے, صحيح channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.قسم = CONST; a.ڈونگھائی = ڈونگھائی; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, انکڑے, channels);
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
صحيح TExpr::typedConstFrom(صحيح src_تھاں, صحيح ڈونگھائی)
{
    ثابت Arg& src = arginfo[src_تھاں];
    Arg a; a.قسم = CONST; a.ڈونگھائی = ڈونگھائی; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); أرجع (صحيح)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
صحيح TExpr::جوڑو_Insn(چوٹی op, صحيح a0, صحيح a1, صحيح a2, صحيح نتیجہ, ثابت TKernel& kernel, ثابت Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.نتیجہ = نتیجہ;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    أرجع (صحيح)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
صحيح TExpr::جوڑو_Insn(چوٹی op, صحيح a0, صحيح a1, صحيح a2, صحيح نتیجہ, ثابت Scalar& params)
{
    صحيح d0 = a0 ? arginfo[a0].ڈونگھائی : EW_ڈونگھائی_NONE;
    صحيح d1 = a1 ? arginfo[a1].ڈونگھائی : EW_ڈونگھائی_NONE;
    صحيح d2 = a2 ? arginfo[a2].ڈونگھائی : EW_ڈونگھائی_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[نتیجہ].ڈونگھائی);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    أرجع جوڑو_Insn(op, a0, a1, a2, نتیجہ, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
صحيح TExpr::maybe_جوڑو_Cast(صحيح arg, صحيح ڈونگھائی)
{
    إذا (arginfo[arg].ڈونگھائی == ڈونگھائی) أرجع arg;
    صحيح t = جوڑو_عارضی(ڈونگھائی);
    جوڑو_Insn(OP_CAST, arg, 0, 0, t);
    أرجع t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
ساكن مضمن فراغ لگاتار_Insn(ثابت TExpr::Insn& ins,
                           ثابت فراغ* p0, size_t y0, size_t x0,
                           ثابت فراغ* p1, size_t y1, size_t x1,
                           ثابت فراغ* p2, size_t y2, size_t x2,
                           فراغ* pr, size_t yr, صحيح w, صحيح h)
{
    صحيح کوڈ = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(کوڈ >= 0);
}

بنية EwBody {
    ثابت TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ثابت صحيح* تھاں_Map;
    ثابت نشانیوالا حرف* تھاں_قسم; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ثابت صحيح* buf_Esz_اگیتر;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
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

    Auto_بفر<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; Ew_قدماں قدم; صحيح esz1;
    لكل (صحيح i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, قدم, esz1); bshapes[i] = shp; }
    MatShape بھریا;
    CV_Assert(نشر_Shape(bshapes.انکڑے(), ninputs, بھریا) && "ew: inputs not broadcast-compatible");
    ثابت صحيح nd_بھریا = (صحيح)بھریا.size();
    channels = بھریا[nd_بھریا - 1];
    spatial = بھریا; spatial.resize(nd_بھریا - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
فراغ TExpr::outputShape(ثابت Mat* inputs, MatShape& spatial, صحيح& channels) ثابت
{
    Auto_بفر<ثابت Mat*, 16> ptrs(std::max(ninputs, 1));
    لكل (صحيح i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.انکڑے(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ساكن constexpr صحيح LOCAL_HDRS = 3;
ساكن constexpr صحيح LOCAL_CONSTS = 4;
ساكن constexpr صحيح LOCAL_OPS = 16;

فراغ TExpr::exec(ثابت Mat* ثابت* inputs, Mat* outputs)
{
    ورتوں BrTile = نشر_Op::Tile;
    ورتوں Br_ٹکڑا = نشر_Op::ٹکڑا;
    ثابت صحيح nslots = (صحيح)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].قسم == TExpr::NONE);

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
            إذا (arginfo[s].قسم == OUTPUT) ok = ok && outputs[arginfo[s].اشاریہ].isContinuous();
        ثابت size_t کل = ok ? r.کل() * rcn : 0;
        إذا (ok && کل == 0)
        {
            ثابت MatShape rshape = r.size;
            لكل (صحيح s = 1; s < nslots; s++)
                إذا (arginfo[s].قسم == OUTPUT)
                    outputs[arginfo[s].اشاریہ].create(rshape, CV_MAKETYPE(arginfo[s].ڈونگھائی, rcn));
            أرجع;
        }
        إذا (ok && کل <= EW_FASTPATH_MAX)
        {
            ثابت MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            لكل (صحيح s = 1; s < nslots; s++)
                إذا (arginfo[s].قسم == OUTPUT)
                    outputs[arginfo[s].اشاریہ].create(rshape, CV_MAKETYPE(arginfo[s].ڈونگھائی, rcn));
            ثابت صحيح ninsn = (صحيح)prog.size();

            إذا (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                خودکار ptr_دا = [&](صحيح s) -> فراغ* {
                    إذا (s <= 0) أرجع خالی_سوچک;
                    ثابت Arg& ai = arginfo[s];
                    إذا (ai.قسم == داخلہ)  أرجع (فراغ*)inputs[ai.اشاریہ]->انکڑے;
                    إذا (ai.قسم == OUTPUT) أرجع (فراغ*)outputs[ai.اشاریہ].انکڑے;
                    أرجع خالی_سوچک;   // NONE
                };
                ثابت صحيح w = (صحيح)کل;
                لكل (صحيح n = 0; n < ninsn; n++)
                {
                    ثابت Insn& ins = prog[n];
                    لگاتار_Insn(ins, ptr_دا(ins.arg0), 0, 1, ptr_دا(ins.arg1), 0, 1,
                            ptr_دا(ins.arg2), 0, 1, ptr_دا(ins.نتیجہ), 0, w, 1);
                }
                أرجع;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ثابت صحيح کل_Esz = buf_Esz_اگیتر[nbuffers];
            ثابت صحيح wf0 = std::min((صحيح)کل, capElems);

            ثابت size_t region = align_ناپ((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            Auto_بفر<uint64_t, (4*1024 + 256)/حجم(uint64_t)> scratchBuf(
                div_اتے((size_t)کل_Esz * region, (بنانشانی)حجم(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.انکڑے();
            لكل (صحيح x0 = 0; x0 < (صحيح)کل; x0 += wf0)
            {
                ثابت صحيح wf = std::min(wf0, (صحيح)کل - x0);
                خودکار ptr_دا = [&](صحيح s) -> فراغ* {
                    إذا (s <= 0) أرجع خالی_سوچک;
                    ثابت Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.ڈونگھائی);
                    إذا (ai.قسم == داخلہ)  أرجع (uchar*)inputs[ai.اشاریہ]->انکڑے  + (size_t)x0 * esz;
                    إذا (ai.قسم == OUTPUT) أرجع (uchar*)outputs[ai.اشاریہ].انکڑے + (size_t)x0 * esz;
                    إذا (ai.قسم != عارضی)   أرجع خالی_سوچک;                                    // NONE: moved-from
                    ثابت صحيح b = بفر_دا_عارضی.empty() ? ai.اشاریہ : بفر_دا_عارضی[ai.اشاریہ];  //عارضی
                    أرجع scratch + (size_t)buf_Esz_اگیتر[b] * region;                        // fragment-local
                };
                لكل (صحيح n = 0; n < ninsn; n++)
                {
                    ثابت Insn& ins = prog[n];
                    لگاتار_Insn(ins, ptr_دا(ins.arg0), 0, 1, ptr_دا(ins.arg1), 0, 1,
                            ptr_دا(ins.arg2), 0, 1, ptr_دا(ins.نتیجہ), 0, wf, 1);
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
            إذا (inputs[i]->انکڑے && inputs[i]->انکڑے == outputs[j].انکڑے) { inplace = سچ; اكسر; }
    صحيح nhdrs = inplace ? ninputs : 0;

    Auto_بفر<ثابت Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    Auto_بفر<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    Auto_بفر<ثابت Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    Auto_بفر<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    Auto_بفر<صحيح, LOCAL_OPS> تھاں_Map(nslots);
    Auto_بفر<نشانیوالا حرف, LOCAL_OPS> تھاں_قسم(nslots);
    صحيح narr = 0, nc = 0;
    إذا (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        لكل (صحيح j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.انکڑے();
    }
    تھاں_قسم[0] = (نشانیوالا حرف)TExpr::NONE;
    لكل (صحيح s = 1; s < nslots; s++)
    {
        ثابت TExpr::Arg& ai = arginfo[s];
        تھاں_قسم[s] = (نشانیوالا حرف)ai.قسم;
        إذا (ai.قسم == TExpr::داخلہ) {
            تھاں_Map[s] = narr;
            arr[narr++] = inputs[ai.اشاریہ];
        }
        وإلا إذا (ai.قسم == TExpr::OUTPUT) {
            outputs[ai.اشاریہ].create(spatial, CV_MAKETYPE(ai.ڈونگھائی, rchannels));
            تھاں_Map[s] = narr; arr[narr++] = &outputs[ai.اشاریہ];
        }
        وإلا إذا (ai.قسم == TExpr::CONST && ai.ڈونگھائی == EW_ڈونگھائی_NONE) {
            تھاں_قسم[s] = (نشانیوالا حرف)TExpr::NONE;   // dead flexible literal: never referenced
            تھاں_Map[s] = 0;
        }
        وإلا إذا (ai.قسم == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ثابت صحيح c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.ڈونگھائی, c),
                                  (فراغ*)(constbuf.انکڑے() + ai.constofs));
            تھاں_قسم[s] = (نشانیوالا حرف)TExpr::داخلہ;   // to the body it is just an array operand
            تھاں_Map[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        وإلا إذا (ai.قسم == TExpr::عارضی) {
            تھاں_Map[s] = بفر_دا_عارضی.empty() ? ai.اشاریہ : بفر_دا_عارضی[ai.اشاریہ];  // physical buffer id
        }
        وإلا {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            تھاں_Map[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ثابت صحيح ninsn = (صحيح)prog.size();
    لما لما لاگت_فی_Elem = 0;
    لكل (صحيح n = 0; n < ninsn; n++)
        لاگت_فی_Elem += op_لاگت(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    لما لما otot = (لما لما)rchannels;
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
    ثابت مزدوج T = (مزدوج)std::max(get_عدد_دھاگے(), 1);
    ثابت مزدوج nstripes = std::min(
        (مزدوج)otot * (مزدوج)std::max<لما لما>(لاگت_فی_Elem, 1) * (1./ (مزدوج)(1 << 16)),
        std::min(4.*T, std::max(32., 3.*T)));

    EwBody body;
    body.prog = prog.انکڑے();
    body.ninsn = ninsn;
    body.تھاں_Map = تھاں_Map.انکڑے();

    // ---- 5. drive: broadcastOp does geometry + 2D tiling + parallelism; the body runs the
    //         frozen program on each tile (temps tile-local, re-pointed from the tile slices).
    //         expandChannels=true => the body always sees single-channel data.
    //
    //         All state the body needs is packed into one POD (EwBody) so the lambda captures a
    //         SINGLE reference: the closure is then one pointer, fits std::function's small-buffer
    //         and never heap-allocates. Inside, the hot fields are copied into locals so the
    //         per-tile/per-insn loops read them from registers, not through the captured pointer. -

    body.buf_Esz_اگیتر = buf_Esz_اگیتر.انکڑے();
    body.تھاں_قسم = تھاں_قسم.انکڑے();
    body.nslots = nslots;
    body.nbuffers = nbuffers;
    body.capElems = capElems;

    نشر_Op_2(arr.انکڑے(), narr, [&](ثابت BrTile& tile)
    {
        EwBody& bc = body;
        ثابت TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ثابت صحيح* ثابت smap  = bc.تھاں_Map;
        ثابت نشانیوالا حرف* ثابت skind = bc.تھاں_قسم;
        ثابت صحيح nin = bc.ninsn, nsl = bc.nslots;
        ثابت صحيح w = tile.چوڑائی, h = tile.اچائی;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ثابت صحيح* ثابت esz_اگیتر = bc.buf_Esz_اگیتر;
        ثابت صحيح bw = std::min(w, bc.capElems);
        ثابت صحيح bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ثابت size_t region = align_ناپ((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        Auto_بفر<uint64_t, (4*1024 + 256)/حجم(uint64_t)> tstoreBuf(
            div_اتے((size_t)esz_اگیتر[bc.nbuffers] * region, (بنانشانی)حجم(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.انکڑے();

        Auto_بفر<Br_ٹکڑا, LOCAL_OPS> دلیلاں(nsl);
        لكل (صحيح y0 = 0; y0 < h; y0 += bh)
        {
            ثابت صحيح hf = std::min(bh, h - y0);
            لكل (صحيح x0 = 0; x0 < w; x0 += bw)
            {
                ثابت صحيح wf = std::min(bw, w - x0);
                لكل (صحيح s = 1; s < nsl; s++)
                {
                    Br_ٹکڑا& a = دلیلاں[s];
                    ثابت صحيح k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].ڈونگھائی);
                    إذا (k == TExpr::عارضی)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)esz_اگیتر[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    وإلا                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ثابت Br_ٹکڑا& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                لكل (صحيح n = 0; n < nin; n++)
                {
                    ثابت TExpr::Insn& ins = insns[n];
                    ثابت Br_ٹکڑا& a0 = دلیلاں[ins.arg0]; ثابت Br_ٹکڑا& a1 = دلیلاں[ins.arg1];
                    ثابت Br_ٹکڑا& a2 = دلیلاں[ins.arg2]; ثابت Br_ٹکڑا& rr = دلیلاں[ins.نتیجہ];
                    لگاتار_Insn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (فراغ*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, سچ, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
فراغ TExpr::exec(ثابت Mat* inputs, Mat* outputs)
{
    Auto_بفر<ثابت Mat*, 16> ptrs(std::max(ninputs, 1));
    لكل (صحيح i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    exec(ptrs.انکڑے(), outputs);
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
فراغ make_ثنائی_Arith_پروگرام(TExpr& p, چوٹی op, صحيح depth0, صحيح depth1, صحيح rdepth,
                            صحيح نقاب_ڈونگھائی, مزدوج scale)
{
    p.clear();
    إذا (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        إذا (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        وإلا
        {
            rdepth = promoteArith(depth0, depth1);
            إذا (op == OP_ABSDIFF) rdepth = absdiff_نتیجہ_ڈونگھائی(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ثابت منطقي masked = نقاب_ڈونگھائی != EW_ڈونگھائی_NONE;
    صحيح sIn0  = p.جوڑو_داخلہ(depth0);
    صحيح sIn1  = p.جوڑو_داخلہ(depth1);
    صحيح s_نقاب = masked ? p.جوڑو_داخلہ(نقاب_ڈونگھائی) : 0;
    صحيح s_باہرلا  = p.جوڑو_Output(rdepth);

    صحيح r = p.emit_ثنائی(op, sIn0, sIn1, rdepth, Scalar(scale));
    إذا (masked)
        p.جوڑو_Insn(OP_SELECT, s_نقاب, r, s_باہرلا, s_باہرلا);          // dst = mask ? r : dst
    وإلا
        p.move_منزل_Output(r, s_باہرلا);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
فراغ make_جوڑو_Weighted_پروگرام(TExpr& p, صحيح depth0, صحيح depth1, صحيح rdepth,
                            مزدوج alpha, مزدوج beta, مزدوج gamma)
{
    p.clear();
    صحيح sA   = p.جوڑو_داخلہ(depth0);
    صحيح sB   = p.جوڑو_داخلہ(depth1);
    صحيح s_باہرلا = p.جوڑو_Output(rdepth);
    صحيح r = p.emit_ثنائی(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.move_منزل_Output(r, s_باہرلا);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

نطاق {

// --- tokens ------------------------------------------------------------------------------
تعداد TokType { T_عدد, T_داخلہ, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_سونپو, T_سرا };

بنية Token
{
    TokType type = T_سرا;
    مزدوج num = 0;
    صحيح داخلہ_2 = 0;
    std::string متن;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
بنية Lexer
{
    std::string_view s;
    size_t pos = 0;

    واضح Lexer(std::string_view src) : s(src) {}

    ساكن منطقي is_Ident_شروع(حرف c) { أرجع std::حرف_اے((بنانشانی حرف)c) || c == '_'; }
    ساكن منطقي isIdentChar(حرف c)  { أرجع std::حرف_یا_ہندسہ((بنانشانی حرف)c) || c == '_'; }

    Token next()
    {
        طالما (pos < s.size() && std::خلا_اے((بنانشانی حرف)s[pos])) pos++;
        Token t;
        إذا (pos >= s.size()) { t.type = T_سرا; أرجع t; }

        حرف c = s[pos];

        إذا (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t شروع = pos;
            طالما (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_داخلہ;
            t.داخلہ_2 = std::atoi(std::string(s.substr(شروع, pos - شروع)).c_str());
            pos++;                          // consume '}'
            أرجع t;
        }
        إذا (std::ہندسہ_اے((بنانشانی حرف)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::ہندسہ_اے((بنانشانی حرف)s[pos + 1])))
        {
            حرف* end = خالی_سوچک;
            std::string num(s.substr(pos));
            t.type = T_عدد;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            أرجع t;
        }
        إذا (is_Ident_شروع(c))
        {
            size_t شروع = pos;
            طالما (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.متن.assign(s.substr(شروع, pos - شروع));
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
        خودکار two = [&](ثابت حرف* op) {
            أرجع pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        إذا (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.متن.assign(s.substr(pos, 2)); pos += 2; أرجع t; }
        إذا (c == '=') { pos++; t.type = T_سونپو; أرجع t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.متن.assign(1, c); pos++;
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

ساكن چوٹی binOp(ثابت std::string& op)
{
    إذا (op == "**") أرجع OP_POW;
    إذا (op == "+")  أرجع OP_جوڑو;
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
ساكن صحيح type_ڈونگھائی(ثابت std::string& name)
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
ساكن چوٹی fnOp(ثابت std::string& name, صحيح& arity)
{
    بنية E { ثابت حرف* n; چوٹی op; };
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
    ثابت صحيح* داخلہ_تھاں;                 // slot id of each external input (precreated)
    صحيح ninputs;
    std::map<std::string, صحيح> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& اظہاریہ, ثابت صحيح* islot, صحيح nin)
        : lex(src), e(اظہاریہ), داخلہ_تھاں(islot), ninputs(nin) { cur = lex.next(); }

    فراغ advance() { cur = lex.next(); }
    منطقي isOp(ثابت حرف* op) ثابت { أرجع cur.type == T_OP && cur.متن == op; }

    فراغ expect(TokType t, ثابت حرف* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    منطقي isFlexConst(صحيح تھاں) ثابت
    {
        أرجع e.arginfo[تھاں].قسم == TExpr::CONST && e.arginfo[تھاں].ڈونگھائی == EW_ڈونگھائی_NONE;
    }

    صحيح parsePrimary()
    {
        إذا (cur.type == T_عدد)   { صحيح s = e.جوڑو_Const(EW_ڈونگھائی_NONE, Scalar(cur.num), 1); advance(); أرجع s; }
        إذا (cur.type == T_داخلہ) { صحيح idx = cur.داخلہ_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   أرجع داخلہ_تھاں[idx]; }
        إذا (cur.type == T_LPAREN){ advance(); صحيح x = parseTernary(); expect(T_RPAREN, "expected ')'"); أرجع x; }
        إذا (cur.type == T_IDENT)
        {
            std::string name = cur.متن; advance();
            إذا (cur.type != T_LPAREN)             // variable reference
            {
                خودکار it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                أرجع it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<صحيح, 8> دلیلاں = {}; صحيح nargs = 0;
            إذا (cur.type != T_RPAREN)
            {
                دلیلاں[nargs++] = parseTernary();
                طالما (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (صحيح)دلیلاں.size() && "ew::expression: too many arguments");
                    دلیلاں[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            صحيح td = type_ڈونگھائی(name);
            إذا (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                صحيح s = دلیلاں[0];
                أرجع isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybe_جوڑو_Cast(s, td);
            }

            صحيح arity = 0; چوٹی op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            إذا (arity == 1) أرجع e.emitUnary(op, دلیلاں[0]);
            إذا (arity == 2) أرجع e.emit_ثنائی(op, دلیلاں[0], دلیلاں[1]);
            أرجع e.emitTernary(op, دلیلاں[0], دلیلاں[1], دلیلاں[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    صحيح parseUnary()
    {
        إذا (isOp("-") || isOp("!"))
        {
            std::string op = cur.متن; advance();
            صحيح عملیہ = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            إذا (op == "-" && isFlexConst(عملیہ))
            {
                Auto_بفر<مزدوج, MAX_LOCAL_CN> v; صحيح cn = constDoubles(e, عملیہ, v);
                Scalar neg; لكل (صحيح i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                أرجع e.جوڑو_Const(EW_ڈونگھائی_NONE, neg, std::min(cn, 4));
            }
            أرجع e.emitUnary(op == "-" ? OP_NEG : OP_NOT, عملیہ);
        }
        أرجع parsePrimary();
    }

    صحيح parse_اظہاریہ(صحيح minPrec)
    {
        صحيح کھبے = parseUnary();
        طالما (cur.type == T_OP)
        {
            صحيح p = binPrec(cur.متن);
            إذا (p < minPrec) اكسر;
            std::string op = cur.متن; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            صحيح سجے = parse_اظہاریہ(op == "**" ? p : p + 1);
            کھبے = e.emit_ثنائی(binOp(op), کھبے, سجے);
        }
        أرجع کھبے;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    صحيح parseTernary()
    {
        صحيح cond = parse_اظہاریہ(0);
        إذا (!isOp("?")) أرجع cond;
        advance();
        صحيح thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        صحيح elsev = parseTernary();               // right-associative chain
        أرجع e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    فراغ parse_نتیجہ()
    {
        إذا (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); صحيح n_عارضی = e.ntemps;
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
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = n_عارضی;
        }
        e.output(parseTernary());
    }

    فراغ parse()
    {
        طالما (سچ)
        {
            إذا (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.متن; advance();
                إذا (cur.type == T_سونپو)
                {
                    advance();
                    ثابت صحيح تھاں = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[تھاں].pinned = سچ;
                    env[name] = تھاں;
                    expect(T_SEMI, "expected ';' after assignment");
                    استمر;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parse_نتیجہ();
            إذا (cur.type == T_SEMI) advance();
            اكسر;
        }
        CV_Assert(cur.type == T_سرا && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

نطاق cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
فراغ texpr(ثابت std::string& اظہاریہ, داخلہ_Array_دا_Arrays _inputs, Output_Array_دا_Arrays _outputs)
{
    ورتوں نطاق cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.قسم() == داخلہ_Array_2::STD_سمتیہ_MAT);
    ثابت std::vector<Mat>& inps = *(ثابت std::vector<Mat>*)_inputs.getObj();
    ثابت صحيح ninputs = (صحيح)inps.size();
    لكل (صحيح i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    Auto_بفر<صحيح> islot(std::max(ninputs, 1));
    لكل (صحيح i = 0; i < ninputs; i++) islot[i] = e.جوڑو_داخلہ(inps[i].ڈونگھائی());
    Parser(اظہاریہ, e, islot.انکڑے(), ninputs).parse();
    e.compile();

    خودکار قسم = _outputs.قسم();
    إذا (قسم == داخلہ_Array_2::STD_سمتیہ_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.انکڑے(), outs.انکڑے());
    } وإلا {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
