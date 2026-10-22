// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "arithm_expr.hpp"
#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <climits>
#অন্তর্ভাব <cmath>
#অন্তর্ভাব <cstdlib>
#অন্তর্ভাব <cstring>
#অন্তর্ভাব <map>
#অন্তর্ভাব <ostream>
#অন্তর্ভাব <string>

নামাকাশ cv { নামাকাশ ew {

// ============================ op metadata (was ew_op.cpp) ============================

ধ্রুবক অক্ষর* opName(শীর্ষ op)
{
    নির্বাচন (op)
    {
    ক্ষেত্র OP_NOP:           ফেরত "nop";
    ক্ষেত্র OP_NEG:           ফেরত "neg";
    ক্ষেত্র OP_ABS:           ফেরত "abs";
    ক্ষেত্র OP_NOT:           ফেরত "not";
    ক্ষেত্র OP_SQRT:          ফেরত "sqrt";
    ক্ষেত্র OP_EXP:           ফেরত "exp";
    ক্ষেত্র OP_LOG:           ফেরত "log";
    ক্ষেত্র OP_SIN:           ফেরত "sin";
    ক্ষেত্র OP_COS:           ফেরত "cos";
    ক্ষেত্র OP_TANH:          ফেরত "tanh";
    ক্ষেত্র OP_ERF:           ফেরত "erf";
    ক্ষেত্র OP_RELU:          ফেরত "relu";
    ক্ষেত্র OP_CAST:          ফেরত "cast";
    ক্ষেত্র OP_ADD:           ফেরত "add";
    ক্ষেত্র OP_SUB:           ফেরত "sub";
    ক্ষেত্র OP_MUL:           ফেরত "mul";
    ক্ষেত্র OP_DIV:           ফেরত "div";
    ক্ষেত্র OP_POW:           ফেরত "pow";
    ক্ষেত্র OP_MIN:           ফেরত "min";
    ক্ষেত্র OP_MAX:           ফেরত "max";
    ক্ষেত্র OP_ABSDIFF:       ফেরত "absdiff";
    ক্ষেত্র OP_ADDW:          ফেরত "addWeighted";
    ক্ষেত্র OP_HYPOT:         ফেরত "hypot";
    ক্ষেত্র OP_ATAN2:         ফেরত "atan2";
    ক্ষেত্র OP_AND:           ফেরত "and";
    ক্ষেত্র OP_OR:            ফেরত "or";
    ক্ষেত্র OP_XOR:           ফেরত "xor";
    ক্ষেত্র OP_CMP_EQ:        ফেরত "cmp_eq";
    ক্ষেত্র OP_CMP_NE:        ফেরত "cmp_ne";
    ক্ষেত্র OP_CMP_LT:        ফেরত "cmp_lt";
    ক্ষেত্র OP_CMP_LE:        ফেরত "cmp_le";
    ক্ষেত্র OP_CMP_GT:        ফেরত "cmp_gt";
    ক্ষেত্র OP_CMP_GE:        ফেরত "cmp_ge";
    ক্ষেত্র OP_CLAMP:         ফেরত "clamp";
    ক্ষেত্র OP_SELECT:        ফেরত "select";
    ক্ষেত্র OP_CONVERT_SCALE: ফেরত "convert_scale";
    পূর্বনির্ধারিত:               ফেরত "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(শীর্ষ op)
{
    নির্বাচন (op)
    {
    ক্ষেত্র OP_AND: ক্ষেত্র OP_OR: ক্ষেত্র OP_XOR: ক্ষেত্র OP_NOT:
        ফেরত CAT_BITWISE;
    ক্ষেত্র OP_CMP_EQ: ক্ষেত্র OP_CMP_NE: ক্ষেত্র OP_CMP_LT:
    ক্ষেত্র OP_CMP_LE: ক্ষেত্র OP_CMP_GT: ক্ষেত্র OP_CMP_GE:
        ফেরত CAT_COMPARE;
    ক্ষেত্র OP_SQRT: ক্ষেত্র OP_EXP: ক্ষেত্র OP_LOG:
    ক্ষেত্র OP_SIN: ক্ষেত্র OP_COS: ক্ষেত্র OP_TANH: ক্ষেত্র OP_ERF: ক্ষেত্র OP_ATAN2: ক্ষেত্র OP_RELU:
        ফেরত CAT_MATH;
    ক্ষেত্র OP_CAST: ক্ষেত্র OP_CONVERT_SCALE:
        ফেরত CAT_CAST;
    ক্ষেত্র OP_SELECT:
        ফেরত CAT_SELECT;
    পূর্বনির্ধারিত:
        ফেরত CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
স্থির যৌক্তিক isFloatDepth(পূর্ণসংখ্যা d)
{
    ফেরত d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
স্থির যৌক্তিক depthRepresents(দ্বিগুণ v, পূর্ণসংখ্যা depth)
{
    যদি (isFloatDepth(depth)) ফেরত সত্য;
    যদি (v != std::floor(v)) ফেরত মিথ্যা;
    নির্বাচন (depth)
    {
    ক্ষেত্র CV_8U:  ফেরত v >= 0            && v <= 255;
    ক্ষেত্র CV_8S:  ফেরত v >= -128         && v <= 127;
    ক্ষেত্র CV_16U: ফেরত v >= 0            && v <= 65535;
    ক্ষেত্র CV_16S: ফেরত v >= -32768       && v <= 32767;
    ক্ষেত্র CV_32U: ফেরত v >= 0            && v <= 4294967295.0;
    ক্ষেত্র CV_32S: ফেরত v >= -2147483648.0 && v <= 2147483647.0;
    ক্ষেত্র CV_64U: ফেরত v >= 0            && v <= 18446744073709551615.0;
    ক্ষেত্র CV_64S: ফেরত v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    পূর্বনির্ধারিত:     ফেরত মিথ্যা;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
স্থির শূন্য intRange(পূর্ণসংখ্যা depth, দ্বিগুণ& lo, দ্বিগুণ& hi)
{
    নির্বাচন (depth)
    {
    ক্ষেত্র CV_8U:  lo = 0;                     hi = 255; বিরতি;
    ক্ষেত্র CV_8S:  lo = -128;                  hi = 127; বিরতি;
    ক্ষেত্র CV_16U: lo = 0;                     hi = 65535; বিরতি;
    ক্ষেত্র CV_16S: lo = -32768;                hi = 32767; বিরতি;
    ক্ষেত্র CV_32U: lo = 0;                     hi = 4294967295.0; বিরতি;
    ক্ষেত্র CV_32S: lo = -2147483648.0;         hi = 2147483647.0; বিরতি;
    ক্ষেত্র CV_64U: lo = 0;                     hi = 18446744073709551615.0; বিরতি;
    ক্ষেত্র CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; বিরতি;
    পূর্বনির্ধারিত:     lo = 0;                     hi = 0; বিরতি;
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
পূর্ণসংখ্যা promoteArith(পূর্ণসংখ্যা a, পূর্ণসংখ্যা b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    যদি (a == b) ফেরত a;

    পূর্ণসংখ্যা wa = পূর্ণসংখ্যা((leszlut >> (a+1)*3) & 7u), wb = পূর্ণসংখ্যা((leszlut >> (b+1)*3) & 7u);
    পূর্ণসংখ্যা fa = পূর্ণসংখ্যা((fltlut >> (a+1)) & 1u),    fb = পূর্ণসংখ্যা((fltlut >> (b+1)) & 1u);

    যদি (fa + fb == 0)                          // both integer
    {
        পূর্ণসংখ্যা sa = পূর্ণসংখ্যা((signlut >> (a+1)) & 1u), sb = পূর্ণসংখ্যা((signlut >> (b+1)) & 1u);
        যদি (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            ফেরত wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        পূর্ণসংখ্যা rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr পূর্ণসংখ্যা ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        ফেরত (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    পূর্ণসংখ্যা maxw = std::max(wa, wb);
    যদি (maxw == 2 && fa + fb == 1)
        ফেরত fa*a + (1 - fa)*b;
    ফেরত CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
পূর্ণসংখ্যা absdiff_ফলাফল_Depth(পূর্ণসংখ্যা depth)
{
    নির্বাচন (depth)
    {
    ক্ষেত্র CV_8S:  ফেরত CV_8U;
    ক্ষেত্র CV_16S: ফেরত CV_16U;
    ক্ষেত্র CV_32S: ফেরত CV_32U;
    ক্ষেত্র CV_64S: ফেরত CV_64U;
    পূর্বনির্ধারিত:     ফেরত depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
স্থির পূর্ণসংখ্যা promote2(পূর্ণসংখ্যা a, পূর্ণসংখ্যা b)
{
    যদি (a == b) ফেরত a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ধ্রুবক uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ধ্রুবক unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ফেরত পূর্ণসংখ্যা((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
স্থির পূর্ণসংখ্যা promoteBitwise(পূর্ণসংখ্যা a, পূর্ণসংখ্যা b)
{
    যদি (a == EW_DEPTH_NONE) ফেরত b;
    যদি (b == EW_DEPTH_NONE) ফেরত a;
    যদি (a == b) ফেরত a;
    ফেরত CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
স্থির পূর্ণসংখ্যা safeWide(পূর্ণসংখ্যা depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ধ্রুবক uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ধ্রুবক unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ফেরত পূর্ণসংখ্যা((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
গণনা { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
স্থির size_t appendConstBuf(Auto_বাফার<uint64_t, 16>& constbuf, পূর্ণসংখ্যা srcdepth, ধ্রুবক শূন্য* তথ্য, পূর্ণসংখ্যা channels)
{
    ধ্রুবক পূর্ণসংখ্যা cn = std::max(1, channels);
    ধ্রুবক size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ধ্রুবক size_t nu64 = (nbytes + আকার(uint64_t) - 1) / আকার(uint64_t);
    ধ্রুবক size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.তথ্য() + ofs);
    যদি (তথ্য) memcpy(dst, তথ্য, nbytes); অন্যথা memset(dst, 0, nbytes);
    ফেরত ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
স্থির পূর্ণসংখ্যা constDoubles(ধ্রুবক TExpr& e, পূর্ণসংখ্যা s, Auto_বাফার<দ্বিগুণ, MAX_LOCAL_CN>& buf)
{
    ধ্রুবক TExpr::Arg& a = e.arginfo[s];
    ধ্রুবক পূর্ণসংখ্যা cn = std::max(1, a.channels);
    buf.resize(cn);
    ধ্রুবক uchar* src = (ধ্রুবক uchar*)(e.constbuf.তথ্য() + a.constofs);
    যদি (a.srcdepth == CV_64F)
        memcpy(buf.তথ্য(), src, (size_t)cn * আকার(দ্বিগুণ));   // common (Scalar / parsed literal)
    অন্যথা
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.তথ্য(), 0, আকার_2(cn, 1), nullptr);
    ফেরত cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
স্থির অন্তঃরেখ যৌক্তিক isFlexConst(ধ্রুবক TExpr& e, পূর্ণসংখ্যা s)
{
    ফেরত e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
স্থির যৌক্তিক scalar_Const_মান(ধ্রুবক TExpr& e, পূর্ণসংখ্যা s, দ্বিগুণ& v)
{
    ধ্রুবক TExpr::Arg& a = e.arginfo[s];
    যদি (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) ফেরত মিথ্যা;
    Auto_বাফার<দ্বিগুণ, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    ফেরত সত্য;
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
স্থির যৌক্তিক matchScaledChain(ধ্রুবক TExpr& e, পূর্ণসংখ্যা end, পূর্ণসংখ্যা t,
                             পূর্ণসংখ্যা& arrSlot, দ্বিগুণ& scale, পূর্ণসংখ্যা& startIdx)
{
    যদি (end < 0) ফেরত মিথ্যা;
    পূর্ণসংখ্যা j = end, mul_ফলাফল = t;

    // optional trailing narrowing cast
    যদি (e.prog[j].op == OP_CAST && e.prog[j].ফলাফল == t)
    {
        mul_ফলাফল = e.prog[j].arg0;
        যদি (--j < 0) ফেরত মিথ্যা;
    }

    ধ্রুবক TExpr::Insn& mul = e.prog[j];
    যদি (mul.op != OP_MUL || mul.ফলাফল != mul_ফলাফল || mul.params[0] != 1.0) ফেরত মিথ্যা;

    পূর্ণসংখ্যা arr;
    যদি      (scalar_Const_মান(e, mul.arg1, scale)) arr = mul.arg0;
    অন্যথা যদি (scalar_Const_মান(e, mul.arg0, scale)) arr = mul.arg1;
    অন্যথা ফেরত মিথ্যা;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    যদি (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].ফলাফল == arr &&
        e.arginfo[arr].kind == TExpr::অস্থায়ী)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    যদি (e.arginfo[arr].kind == TExpr::CONST) ফেরত মিথ্যা;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    ফেরত সত্য;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
স্থির যৌক্তিক constFits(ধ্রুবক TExpr& e, পূর্ণসংখ্যা s, পূর্ণসংখ্যা d)
{
    যদি (!isFlexConst(e, s)) ফেরত সত্য;
    Auto_বাফার<দ্বিগুণ, MAX_LOCAL_CN> v;
    পূর্ণসংখ্যা cn = constDoubles(e, s, v);
    জন্য (পূর্ণসংখ্যা ch = 0; ch < cn; ch++)
        যদি (!depthRepresents(v[ch], d)) ফেরত মিথ্যা;
    ফেরত সত্য;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
পূর্ণসংখ্যা TExpr::emit_দ্বিমিক(শীর্ষ op, পূর্ণসংখ্যা a, পূর্ণসংখ্যা b, পূর্ণসংখ্যা rdepth, ধ্রুবক Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    যদি (op == OP_ADDW)
    {
        পূর্ণসংখ্যা Tt = arginfo[a].depth;
        যদি (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        যদি (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        যদি (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        পূর্ণসংখ্যা outD = rdepth;
        যদি (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ধ্রুবক পূর্ণসংখ্যা out = add_অস্থায়ী(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        যদি (outD == rdepth) ফেরত out;
        ধ্রুবক পূর্ণসংখ্যা out2 = add_অস্থায়ী(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        ফেরত out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    যদি (op == OP_ADD)
    {
        ধ্রুবক পূর্ণসংখ্যা n = (পূর্ণসংখ্যা)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        পূর্ণসংখ্যা x = 0, y = 0, startA = 0, startB = 0;
        দ্বিগুণ alpha = 0, beta = 0;
        যদি (n >= 2 && a != b &&
            arginfo[a].kind == অস্থায়ী && !arginfo[a].pinned &&
            arginfo[b].kind == অস্থায়ী && !arginfo[b].pinned &&
            matchScaledChain(*এই, n - 1, b, y, beta, startB) &&
            matchScaledChain(*এই, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            পূর্ণসংখ্যা retired = 0, lowest = INT_MAX;
            জন্য (পূর্ণসংখ্যা i = startA; i < n; i++)
            {
                ধ্রুবক পূর্ণসংখ্যা r = prog[i].ফলাফল;
                যদি (arginfo[r].kind != অস্থায়ী || arginfo[r].pinned) { retired = -1; বিরতি; }
                lowest = std::min(lowest, arginfo[r].সূচক);
                retired++;
            }
            যদি (retired > 0 && lowest == ntemps - retired)
            {
                জন্য (পূর্ণসংখ্যা i = startA; i < n; i++) arginfo[prog[i].ফলাফল].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                ফেরত emit_দ্বিমিক(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            পূর্ণসংখ্যা acc = 0; দ্বিগুণ gamma = 0;
            যদি      (scalar_Const_মান(*এই, b, gamma)) acc = a;
            অন্যথা যদি (scalar_Const_মান(*এই, a, gamma)) acc = b;
            যদি (acc != 0 && n >= 1 &&
                arginfo[acc].kind == অস্থায়ী && !arginfo[acc].pinned &&
                arginfo[acc].সূচক == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].ফলাফল == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                ফেরত acc;
            }
        }
    }

    ধ্রুবক পূর্ণসংখ্যা nd0 = isFlexConst(*এই, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ধ্রুবক পূর্ণসংখ্যা nd1 = isFlexConst(*এই, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    ধ্রুবক ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    পূর্ণসংখ্যা ফলাফল = rdepth;
    যদি (ফলাফল == EW_DEPTH_NONE)
    {
        ফলাফল = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        যদি (ফলাফল == EW_DEPTH_NONE) ফলাফল = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    পূর্ণসংখ্যা base;
    যদি (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = ফলাফল;
    অন্যথা যদি (nd0 == EW_DEPTH_NONE) base = nd1;
    অন্যথা যদি (nd1 == EW_DEPTH_NONE) base = nd0;
    অন্যথা base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    যদি (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*এই, b) && !constFits(*এই, b, base))
    {
        দ্বিগুণ lo, hi; intRange(nd0, lo, hi);
        যদি (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            গণনা { REAL, CFALSE, CTRUE };
            Auto_বাফার<দ্বিগুণ, MAX_LOCAL_CN> tv; পূর্ণসংখ্যা cn = constDoubles(*এই, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            পূর্ণসংখ্যা kind[4] = {}; দ্বিগুণ bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ধ্রুবক শীর্ষ fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            জন্য (পূর্ণসংখ্যা c = 0; c < cn; c++)
            {
                ধ্রুবক দ্বিগুণ t = tv[c]; পূর্ণসংখ্যা k; দ্বিগুণ B = t;
                নির্বাচন (op)
                {
                ক্ষেত্র OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; বিরতি;
                ক্ষেত্র OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; বিরতি;
                ক্ষেত্র OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; বিরতি;
                ক্ষেত্র OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; বিরতি;
                ক্ষেত্র OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; বিরতি;   // a==non-rep -> false
                পূর্বনির্ধারিত:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  বিরতি;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](পূর্ণসংখ্যা c){ ফেরত kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](পূর্ণসংখ্যা c){ ফেরত kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ধ্রুবক শীর্ষ u0 = ucOp(0);
            যৌক্তিক uniform = সত্য;
            জন্য (পূর্ণসংখ্যা c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            যদি (uniform)
            {
                Scalar s; জন্য (পূর্ণসংখ্যা c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            অন্যথা
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; পূর্ণসংখ্যা patchFlags = 0;
                জন্য (পূর্ণসংখ্যা c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ধ্রুবক পূর্ণসংখ্যা mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ধ্রুবক পূর্ণসংখ্যা vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, ফলাফল);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ধ্রুবক পূর্ণসংখ্যা out = add_অস্থায়ী(ফলাফল);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                ফেরত out;
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
    পূর্ণসংখ্যা depth, wide;
    যৌক্তিক divGuard = মিথ্যা;
    নির্বাচন (op)
    {
    ক্ষেত্র OP_MIN: ক্ষেত্র OP_MAX:
    ক্ষেত্র OP_AND: ক্ষেত্র OP_OR: ক্ষেত্র OP_XOR:
        depth = ফলাফল; wide = ফলাফল; বিরতি;
    ক্ষেত্র OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        পূর্ণসংখ্যা common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        যদি (common == EW_DEPTH_NONE) common = ফলাফল;          // both operands flexible consts
        depth = common;
        wide  = absdiff_ফলাফল_Depth(common);
        যদি (rdepth == EW_DEPTH_NONE) ফলাফল = wide;            // auto result: unsigned same width
        বিরতি;
    }
    ক্ষেত্র OP_CMP_EQ: ক্ষেত্র OP_CMP_NE: ক্ষেত্র OP_CMP_LT:
    ক্ষেত্র OP_CMP_LE: ক্ষেত্র OP_CMP_GT: ক্ষেত্র OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; বিরতি;
    ক্ষেত্র OP_MUL: ক্ষেত্র OP_DIV:
    {
        depth = base;
        ধ্রুবক যৌক্তিক w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        যদি (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](পূর্ণসংখ্যা s, পূর্ণসংখ্যা nd) {
                যদি (nd != EW_DEPTH_NONE) ফেরত !isFloatDepth(nd);
                Auto_বাফার<দ্বিগুণ, MAX_LOCAL_CN> v; constDoubles(*এই, s, v);
                ফেরত v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        বিরতি;
    }
    ক্ষেত্র OP_POW: ক্ষেত্র OP_HYPOT: ক্ষেত্র OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; বিরতি;
    পূর্বনির্ধারিত:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); বিরতি;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](পূর্ণসংখ্যা s) {
        যদি (!isFlexConst(*এই, s)) ফেরত মিথ্যা;
        Auto_বাফার<দ্বিগুণ, MAX_LOCAL_CN> v; পূর্ণসংখ্যা n = constDoubles(*এই, s, v);
        জন্য (পূর্ণসংখ্যা ch = 0; ch < n; ch++)
            যদি (v[ch] != std::floor(v[ch])) ফেরত সত্য;
        ফেরত মিথ্যা;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    যদি (cat != CAT_BITWISE && (!constFits(*এই, a, depth) || !constFits(*এই, b, depth)))
    {
        depth = wide;
        ধ্রুবক যৌক্তিক frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        যদি (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (ফলাফল == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        যদি (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    পূর্ণসংখ্যা s0 = isFlexConst(*এই, a) ? typedConstFrom(a, depth) : a;
    পূর্ণসংখ্যা s1 = isFlexConst(*এই, b) ? typedConstFrom(b, depth) : b;
    পূর্ণসংখ্যা c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](পূর্ণসংখ্যা rd) {
        ফেরত op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(ফলাফল);
    যদি (k.fptr)
    {
        পূর্ণসংখ্যা res = add_অস্থায়ী(ফলাফল);
        addInsn(op, c0, c1, 0, res, k, params);
        ফেরত res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    পূর্ণসংখ্যা w = add_অস্থায়ী(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    ফেরত maybeAddCast(w, ফলাফল);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
পূর্ণসংখ্যা TExpr::emitUnary(শীর্ষ op, পূর্ণসংখ্যা a, পূর্ণসংখ্যা rdepth, ধ্রুবক Scalar& params)
{
    ধ্রুবক পূর্ণসংখ্যা nd = isFlexConst(*এই, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ধ্রুবক ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    যদি (op == OP_NEG)
        ফেরত emit_দ্বিমিক(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    যদি (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        যদি (!prog.empty() && arginfo[a].kind == অস্থায়ী && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().ফলাফল == a &&
            arginfo[a].সূচক == ntemps - 1)
        {
            ধ্রুবক পূর্ণসংখ্যা x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            ফেরত emit_দ্বিমিক(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        ফেরত emit_দ্বিমিক(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    পূর্ণসংখ্যা ফলাফল = rdepth;
    যদি (ফলাফল == EW_DEPTH_NONE)
    {
        নির্বাচন (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        ক্ষেত্র CAT_MATH:    ফলাফল = isFloatDepth(nd) ? nd : CV_32F; বিরতি;
        ক্ষেত্র CAT_BITWISE: ফলাফল = (nd == EW_DEPTH_NONE) ? CV_32S : nd; বিরতি;  // NOT
        পূর্বনির্ধারিত:          ফলাফল = (nd == EW_DEPTH_NONE) ? CV_32F : nd; বিরতি;  // NEG, ABS
        }
    }

    পূর্ণসংখ্যা depth, wide;
    নির্বাচন (cat)
    {
    ক্ষেত্র CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == ফলাফল) ? nd
              : (nd == CV_64F || ফলাফল == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        বিরতি;
    ক্ষেত্র CAT_BITWISE: depth = ফলাফল; wide = ফলাফল; বিরতি;
    পূর্বনির্ধারিত:          depth = (nd == EW_DEPTH_NONE) ? ফলাফল : nd; wide = ফলাফল; বিরতি;  // NEG/ABS
    }

    যদি (!constFits(*এই, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    পূর্ণসংখ্যা s0 = isFlexConst(*এই, a) ? typedConstFrom(a, depth) : a;
    পূর্ণসংখ্যা c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, ফলাফল);
    যদি (k.fptr)
    {
        পূর্ণসংখ্যা res = add_অস্থায়ী(ফলাফল);
        addInsn(op, c0, 0, 0, res, k, params);
        ফেরত res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    পূর্ণসংখ্যা w = add_অস্থায়ী(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    ফেরত maybeAddCast(w, ফলাফল);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
পূর্ণসংখ্যা TExpr::emitTernary(শীর্ষ op, পূর্ণসংখ্যা a, পূর্ণসংখ্যা b, পূর্ণসংখ্যা c, পূর্ণসংখ্যা rdepth)
{
    auto dep = [&](পূর্ণসংখ্যা s) { ফেরত isFlexConst(*এই, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    যদি (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        যদি (isFlexConst(*এই, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_দ্বিমিক(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        পূর্ণসংখ্যা ফলাফল = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        যদি (ফলাফল == EW_DEPTH_NONE) ফলাফল = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        পূর্ণসংখ্যা sb = isFlexConst(*এই, b) ? typedConstFrom(b, ফলাফল) : maybeAddCast(b, ফলাফল);
        পূর্ণসংখ্যা sc = isFlexConst(*এই, c) ? typedConstFrom(c, ফলাফল) : maybeAddCast(c, ফলাফল);
        পূর্ণসংখ্যা res = add_অস্থায়ী(ফলাফল);
        addInsn(OP_SELECT, a, sb, sc, res);
        ফেরত res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    পূর্ণসংখ্যা ফলাফল = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    যদি (ফলাফল == EW_DEPTH_NONE) ফলাফল = CV_32F;
    auto typed = [&](পূর্ণসংখ্যা s) {
        ফেরত isFlexConst(*এই, s) ? typedConstFrom(s, ফলাফল) : maybeAddCast(s, ফলাফল);
    };
    পূর্ণসংখ্যা c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, ফলাফল, ফলাফল, ফলাফল, ফলাফল);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    পূর্ণসংখ্যা res = add_অস্থায়ী(ফলাফল);
    addInsn(op, c0, c1, c2, res, k);
    ফেরত res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
পূর্ণসংখ্যা TExpr::moveToOutput(পূর্ণসংখ্যা অস্থায়ী_2, পূর্ণসংখ্যা out)
{
    ধ্রুবক পূর্ণসংখ্যা ninsn = (পূর্ণসংখ্যা)prog.size();
    পূর্ণসংখ্যা producer = -1;
    যৌক্তিক usedAsArg = মিথ্যা;
    জন্য (পূর্ণসংখ্যা i = 0; i < ninsn; i++)
    {
        ধ্রুবক TExpr::Insn& ins = prog[i];
        যদি (ins.ফলাফল == অস্থায়ী_2) producer = i;
        যদি (ins.arg0 == অস্থায়ী_2 || ins.arg1 == অস্থায়ী_2 || ins.arg2 == অস্থায়ী_2) usedAsArg = সত্য;
    }
    যদি (arginfo[অস্থায়ী_2].kind == অস্থায়ী && producer >= 0 && !usedAsArg && !arginfo[অস্থায়ী_2].pinned &&
        arginfo[অস্থায়ী_2].depth == arginfo[out].depth)
    {
        // MOVE semantics: redirect `temp`'s single producer to write `out` directly, then leave the
        // source slot EMPTY - reclassify it to NONE. A NONE slot gets no physical buffer (compile's
        // liveness only walks TEMP slots) and is skipped everywhere in exec, so no stale dead-TEMP slot
        // remains (which, buffer or not, would still cost per-tile setup in the general path) - and no
        // slot removal / index renumbering is needed. This keeps single-op programs at zero temps =>
        // compile()'s no-temp early-out (zero heap traffic) still fires. Every caller moves the result
        // it JUST emitted, so `temp` is always the last-created temp - decrement ntemps to retire its
        // index and keep the temp indices dense (0..ntemps-1) for compile()'s liveness arrays.
        CV_Assert(arginfo[অস্থায়ী_2].সূচক == ntemps - 1);
        prog[producer].ফলাফল = out;
        arginfo[অস্থায়ী_2].kind = NONE;
        ntemps--;
        ফেরত out;
    }
    addInsn(OP_CAST, অস্থায়ী_2, 0, 0, out);                     // same/different depth copy into `out`
    ফেরত out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
পূর্ণসংখ্যা TExpr::output(পূর্ণসংখ্যা মূল_Slot)
{
    ফেরত moveToOutput(মূল_Slot, addOutput(arginfo[মূল_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
শূন্য TExpr::compile()
{
    ধ্রুবক পূর্ণসংখ্যা ninsn = (পূর্ণসংখ্যা)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ধ্রুবক পূর্ণসংখ্যা nslots = (পূর্ণসংখ্যা)arginfo.size();
    nconsts = 0;
    জন্য (পূর্ণসংখ্যা s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        যদি (a.kind != CONST || a.depth == EW_DEPTH_NONE) চালু;
        ধ্রুবক পূর্ণসংখ্যা cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        ধ্রুবক size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        Auto_বাফার<uint64_t, আকার(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)আকার(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.তথ্য();
        memcpy(srcbytes, (ধ্রুবক uchar*)(constbuf.তথ্য() + a.constofs), (size_t)cn * sesz);
        ধ্রুবক size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.তথ্য() + ofs);
        যদি (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        অন্যথা getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, আকার_2(cn, 1), nullptr);
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
    যদি (ntemps == 0) ফেরত;

    Auto_বাফার<পূর্ণসংখ্যা, 32> অস্থায়ী_Of_Slot(nslots);
    জন্য (পূর্ণসংখ্যা s = 0; s < nslots; s++) অস্থায়ী_Of_Slot[s] = -1;
    জন্য (পূর্ণসংখ্যা s = 1; s < nslots; s++)
        যদি (arginfo[s].kind == অস্থায়ী) অস্থায়ী_Of_Slot[s] = arginfo[s].সূচক;

    Auto_বাফার<পূর্ণসংখ্যা, 16> lastUse(ntemps);
    জন্য (পূর্ণসংখ্যা t = 0; t < ntemps; t++) lastUse[t] = -1;
    জন্য (পূর্ণসংখ্যা i = 0; i < ninsn; i++)
    {
        ধ্রুবক TExpr::Insn& ins = prog[i];
        পূর্ণসংখ্যা as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        জন্য (পূর্ণসংখ্যা k = 0; k < 3; k++)
            যদি (অস্থায়ী_Of_Slot[as[k]] >= 0) lastUse[অস্থায়ী_Of_Slot[as[k]]] = i;
    }

    বাফার_Of_অস্থায়ী.resize(ntemps);
    জন্য (পূর্ণসংখ্যা t = 0; t < ntemps; t++) বাফার_Of_অস্থায়ী[t] = -1;
    Auto_বাফার<পূর্ণসংখ্যা, 16> freeBufs(ntemps);
    পূর্ণসংখ্যা nfree = 0, nbuf = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < ninsn; i++)
    {
        ধ্রুবক TExpr::Insn& ins = prog[i];
        পূর্ণসংখ্যা rt = অস্থায়ী_Of_Slot[ins.ফলাফল];
        যদি (rt >= 0 && বাফার_Of_অস্থায়ী[rt] < 0)
            বাফার_Of_অস্থায়ী[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        পূর্ণসংখ্যা as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        জন্য (পূর্ণসংখ্যা k = 0; k < 3; k++)
        {
            পূর্ণসংখ্যা t = অস্থায়ী_Of_Slot[as[k]];
            যদি (t >= 0 && lastUse[t] == i)
            {
                freeBufs[nfree++] = বাফার_Of_অস্থায়ী[t];
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
    জন্য (পূর্ণসংখ্যা b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    জন্য (পূর্ণসংখ্যা s = 1; s < nslots; s++)
        যদি (arginfo[s].kind == অস্থায়ী)
        {
            পূর্ণসংখ্যা b = বাফার_Of_অস্থায়ী[arginfo[s].সূচক], e = (পূর্ণসংখ্যা)CV_ELEM_SIZE1(arginfo[s].depth);
            যদি (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    জন্য (পূর্ণসংখ্যা b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ধ্রুবক পূর্ণসংখ্যা যোগ_Esz = bufEszPrefix[nbuffers];
    capElems = যোগ_Esz > 0 ? std::max(64, (4 * 1024) / যোগ_Esz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
স্থির শূন্য matLogical(ধ্রুবক Mat& m, MatShape& shp, EwSteps& step, পূর্ণসংখ্যা& esz1)
{
    esz1 = (পূর্ণসংখ্যা)m.elemSize1();
    পূর্ণসংখ্যা nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    জন্য (পূর্ণসংখ্যা i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
স্থির যৌক্তিক broadcastShape(ধ্রুবক MatShape* shps, পূর্ণসংখ্যা K, MatShape& out)
{
    পূর্ণসংখ্যা nd = 0;
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
    {
        ধ্রুবক MatShape& s = shps[k];
        পূর্ণসংখ্যা off = nd - s.dims;
        জন্য (পূর্ণসংখ্যা i = 0; i < s.dims; i++)
        {
            পূর্ণসংখ্যা d = s[i], &o = out[off + i];
            যদি (o == 1) o = d;
            অন্যথা যদি (d != 1 && d != o) ফেরত মিথ্যা;
        }
    }
    ফেরত সত্য;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
স্থির পূর্ণসংখ্যা opCost(শীর্ষ op)
{
    নির্বাচন (op)
    {
    ক্ষেত্র OP_ADD: ক্ষেত্র OP_SUB: ক্ষেত্র OP_MUL: ক্ষেত্র OP_MIN: ক্ষেত্র OP_MAX:
    ক্ষেত্র OP_ABSDIFF: ক্ষেত্র OP_AND: ক্ষেত্র OP_OR: ক্ষেত্র OP_XOR: ক্ষেত্র OP_NOT:
    ক্ষেত্র OP_NEG: ক্ষেত্র OP_ABS: ক্ষেত্র OP_CAST: ক্ষেত্র OP_RELU: ক্ষেত্র OP_SELECT:
    ক্ষেত্র OP_CMP_EQ: ক্ষেত্র OP_CMP_NE: ক্ষেত্র OP_CMP_LT:
    ক্ষেত্র OP_CMP_LE: ক্ষেত্র OP_CMP_GT: ক্ষেত্র OP_CMP_GE: ফেরত 1;
    ক্ষেত্র OP_DIV: ক্ষেত্র OP_SQRT: ক্ষেত্র OP_HYPOT: ক্ষেত্র OP_CONVERT_SCALE: ফেরত 3;
    ক্ষেত্র OP_SIN: ক্ষেত্র OP_COS: ক্ষেত্র OP_TANH: ক্ষেত্র OP_ERF: ক্ষেত্র OP_ATAN2:
    ক্ষেত্র OP_EXP: ক্ষেত্র OP_LOG: ক্ষেত্র OP_POW:           ফেরত 6;
    পূর্বনির্ধারিত:                                         ফেরত 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
শূন্য TExpr::clear()
{
    prog.allocate(0);
    arginfo.allocate(0);
    বাফার_Of_অস্থায়ী.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
শূন্য TExpr::dump(std::ostream& os) ধ্রুবক
{
    auto dn = [](পূর্ণসংখ্যা d) -> ধ্রুবক অক্ষর* {
        নির্বাচন (d) {
        ক্ষেত্র EW_DEPTH_NONE: ফেরত "flex";
        ক্ষেত্র CV_8U:  ফেরত "u8";  ক্ষেত্র CV_8S:  ফেরত "s8";
        ক্ষেত্র CV_16U: ফেরত "u16"; ক্ষেত্র CV_16S: ফেরত "s16";
        ক্ষেত্র CV_32U: ফেরত "u32"; ক্ষেত্র CV_32S: ফেরত "s32";
        ক্ষেত্র CV_64U: ফেরত "u64"; ক্ষেত্র CV_64S: ফেরত "s64";
        ক্ষেত্র CV_16F: ফেরত "f16"; ক্ষেত্র CV_16BF:ফেরত "bf16";
        ক্ষেত্র CV_32F: ফেরত "f32"; ক্ষেত্র CV_64F: ফেরত "f64";
        ক্ষেত্র CV_Bool:ফেরত "bool";
        পূর্বনির্ধারিত:     ফেরত "?"; }
    };
    auto kn = [](ArgKind k) -> ধ্রুবক অক্ষর* {
        নির্বাচন (k) { ক্ষেত্র NONE: ফেরত "none"; ক্ষেত্র প্রবেশ: ফেরত "in"; ক্ষেত্র CONST: ফেরত "const";
                     ক্ষেত্র অস্থায়ী: ফেরত "temp"; ক্ষেত্র OUTPUT: ফেরত "out"; } ফেরত "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (পূর্ণসংখ্যা)prog.size() << "\n";
    os << "  slots:\n";
    জন্য (পূর্ণসংখ্যা s = 0; s < (পূর্ণসংখ্যা)arginfo.size(); s++) {
        ধ্রুবক Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        যদি (a.kind != NONE) os << " " << dn(a.depth);
        যদি (a.kind == প্রবেশ || a.kind == OUTPUT || a.kind == অস্থায়ী) os << " idx=" << a.সূচক;
        যদি (a.kind == CONST) {
            ধ্রুবক পূর্ণসংখ্যা cn = std::max(1, a.channels);
            Auto_বাফার<দ্বিগুণ, MAX_LOCAL_CN> v(cn);
            ধ্রুবক uchar* src = (ধ্রুবক uchar*)(constbuf.তথ্য() + a.constofs);
            যদি (a.srcdepth == CV_64F) memcpy(v.তথ্য(), src, (size_t)cn * আকার(দ্বিগুণ));
            অন্যথা getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.তথ্য(), 0, আকার_2(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            জন্য (পূর্ণসংখ্যা c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    জন্য (পূর্ণসংখ্যা i = 0; i < (পূর্ণসংখ্যা)prog.size(); i++) {
        ধ্রুবক Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        যদি (ins.arg1) os << ", " << ins.arg1;
        যদি (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.ফলাফল;
        যদি (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        যদি (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        যদি (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
পূর্ণসংখ্যা TExpr::add_প্রবেশ(পূর্ণসংখ্যা depth)
{
    Arg a; a.kind = প্রবেশ; a.depth = depth; a.সূচক = ninputs++;
    arginfo.push_back(a); ফেরত (পূর্ণসংখ্যা)arginfo.size() - 1;
}

পূর্ণসংখ্যা TExpr::addOutput(পূর্ণসংখ্যা depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.সূচক = noutputs++;
    arginfo.push_back(a); ফেরত (পূর্ণসংখ্যা)arginfo.size() - 1;
}

পূর্ণসংখ্যা TExpr::add_অস্থায়ী(পূর্ণসংখ্যা depth)
{
    Arg a; a.kind = অস্থায়ী; a.depth = depth; a.সূচক = ntemps++;
    arginfo.push_back(a); ফেরত (পূর্ণসংখ্যা)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
পূর্ণসংখ্যা TExpr::addConst(পূর্ণসংখ্যা depth, ধ্রুবক Scalar& v, পূর্ণসংখ্যা channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); ফেরত (পূর্ণসংখ্যা)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
পূর্ণসংখ্যা TExpr::addConst(পূর্ণসংখ্যা depth, পূর্ণসংখ্যা srcdepth, ধ্রুবক শূন্য* তথ্য, পূর্ণসংখ্যা channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, তথ্য, channels);
    arginfo.push_back(a); ফেরত (পূর্ণসংখ্যা)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
পূর্ণসংখ্যা TExpr::typedConstFrom(পূর্ণসংখ্যা srcSlot, পূর্ণসংখ্যা depth)
{
    ধ্রুবক Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); ফেরত (পূর্ণসংখ্যা)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
পূর্ণসংখ্যা TExpr::addInsn(শীর্ষ op, পূর্ণসংখ্যা a0, পূর্ণসংখ্যা a1, পূর্ণসংখ্যা a2, পূর্ণসংখ্যা ফলাফল, ধ্রুবক TKernel& kernel, ধ্রুবক Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.ফলাফল = ফলাফল;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    ফেরত (পূর্ণসংখ্যা)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
পূর্ণসংখ্যা TExpr::addInsn(শীর্ষ op, পূর্ণসংখ্যা a0, পূর্ণসংখ্যা a1, পূর্ণসংখ্যা a2, পূর্ণসংখ্যা ফলাফল, ধ্রুবক Scalar& params)
{
    পূর্ণসংখ্যা d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    পূর্ণসংখ্যা d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    পূর্ণসংখ্যা d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[ফলাফল].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ফেরত addInsn(op, a0, a1, a2, ফলাফল, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
পূর্ণসংখ্যা TExpr::maybeAddCast(পূর্ণসংখ্যা arg, পূর্ণসংখ্যা depth)
{
    যদি (arginfo[arg].depth == depth) ফেরত arg;
    পূর্ণসংখ্যা t = add_অস্থায়ী(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    ফেরত t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
স্থির অন্তঃরেখ শূন্য runInsn(ধ্রুবক TExpr::Insn& ins,
                           ধ্রুবক শূন্য* p0, size_t y0, size_t x0,
                           ধ্রুবক শূন্য* p1, size_t y1, size_t x1,
                           ধ্রুবক শূন্য* p2, size_t y2, size_t x2,
                           শূন্য* pr, size_t yr, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    পূর্ণসংখ্যা code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

কাঠামো EwBody {
    ধ্রুবক TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ধ্রুবক পূর্ণসংখ্যা* slotMap;
    ধ্রুবক signed অক্ষর* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ধ্রুবক পূর্ণসংখ্যা* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    পূর্ণসংখ্যা ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

শূন্য TExpr::outputShape(ধ্রুবক Mat* ধ্রুবক* inputs, MatShape& spatial, পূর্ণসংখ্যা& channels) ধ্রুবক
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ধ্রুবক পূর্ণসংখ্যা rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    যৌক্তিক sameShape = ninputs >= 1;
    জন্য (পূর্ণসংখ্যা i = 1; sameShape && i < ninputs; i++)
    {
        ধ্রুবক Mat& a = *inputs[i];
        যদি (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = মিথ্যা;
        অন্যথা জন্য (পূর্ণসংখ্যা d = 0; d < a.dims; d++) যদি (a.size[d] != inputs[0]->size[d]) { sameShape = মিথ্যা; বিরতি; }
    }
    যদি (sameShape) { spatial = inputs[0]->size; channels = rcn0; ফেরত; }

    Auto_বাফার<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; পূর্ণসংখ্যা esz1;
    জন্য (পূর্ণসংখ্যা i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.তথ্য(), ninputs, full) && "ew: inputs not broadcast-compatible");
    ধ্রুবক পূর্ণসংখ্যা ndFull = (পূর্ণসংখ্যা)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
শূন্য TExpr::outputShape(ধ্রুবক Mat* inputs, MatShape& spatial, পূর্ণসংখ্যা& channels) ধ্রুবক
{
    Auto_বাফার<ধ্রুবক Mat*, 16> ptrs(std::max(ninputs, 1));
    জন্য (পূর্ণসংখ্যা i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.তথ্য(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
স্থির constexpr পূর্ণসংখ্যা LOCAL_HDRS = 3;
স্থির constexpr পূর্ণসংখ্যা LOCAL_CONSTS = 4;
স্থির constexpr পূর্ণসংখ্যা LOCAL_OPS = 16;

শূন্য TExpr::exec(ধ্রুবক Mat* ধ্রুবক* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    ধ্রুবক পূর্ণসংখ্যা nslots = (পূর্ণসংখ্যা)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    যদি (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        ধ্রুবক Mat& r = *inputs[0];
        ধ্রুবক পূর্ণসংখ্যা rcn = r.channels();
        যৌক্তিক ok = r.isContinuous();
        জন্য (পূর্ণসংখ্যা i = 1; ok && i < ninputs; i++)
        {
            ধ্রুবক Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        জন্য (পূর্ণসংখ্যা s = 1; ok && s < nslots; s++)
            যদি (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].সূচক].isContinuous();
        ধ্রুবক size_t যোগ = ok ? r.যোগ() * rcn : 0;
        যদি (ok && যোগ == 0)
        {
            ধ্রুবক MatShape rshape = r.size;
            জন্য (পূর্ণসংখ্যা s = 1; s < nslots; s++)
                যদি (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].সূচক].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ফেরত;
        }
        যদি (ok && যোগ <= EW_FASTPATH_MAX)
        {
            ধ্রুবক MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            জন্য (পূর্ণসংখ্যা s = 1; s < nslots; s++)
                যদি (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].সূচক].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ধ্রুবক পূর্ণসংখ্যা ninsn = (পূর্ণসংখ্যা)prog.size();

            যদি (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](পূর্ণসংখ্যা s) -> শূন্য* {
                    যদি (s <= 0) ফেরত nullptr;
                    ধ্রুবক Arg& ai = arginfo[s];
                    যদি (ai.kind == প্রবেশ)  ফেরত (শূন্য*)inputs[ai.সূচক]->তথ্য;
                    যদি (ai.kind == OUTPUT) ফেরত (শূন্য*)outputs[ai.সূচক].তথ্য;
                    ফেরত nullptr;   // NONE
                };
                ধ্রুবক পূর্ণসংখ্যা w = (পূর্ণসংখ্যা)যোগ;
                জন্য (পূর্ণসংখ্যা n = 0; n < ninsn; n++)
                {
                    ধ্রুবক Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.ফলাফল), 0, w, 1);
                }
                ফেরত;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ধ্রুবক পূর্ণসংখ্যা যোগ_Esz = bufEszPrefix[nbuffers];
            ধ্রুবক পূর্ণসংখ্যা wf0 = std::min((পূর্ণসংখ্যা)যোগ, capElems);

            ধ্রুবক size_t region = align_আকার((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            Auto_বাফার<uint64_t, (4*1024 + 256)/আকার(uint64_t)> scratchBuf(
                divUp((size_t)যোগ_Esz * region, (unsigned)আকার(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.তথ্য();
            জন্য (পূর্ণসংখ্যা x0 = 0; x0 < (পূর্ণসংখ্যা)যোগ; x0 += wf0)
            {
                ধ্রুবক পূর্ণসংখ্যা wf = std::min(wf0, (পূর্ণসংখ্যা)যোগ - x0);
                auto ptrOf = [&](পূর্ণসংখ্যা s) -> শূন্য* {
                    যদি (s <= 0) ফেরত nullptr;
                    ধ্রুবক Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    যদি (ai.kind == প্রবেশ)  ফেরত (uchar*)inputs[ai.সূচক]->তথ্য  + (size_t)x0 * esz;
                    যদি (ai.kind == OUTPUT) ফেরত (uchar*)outputs[ai.সূচক].তথ্য + (size_t)x0 * esz;
                    যদি (ai.kind != অস্থায়ী)   ফেরত nullptr;                                    // NONE: moved-from
                    ধ্রুবক পূর্ণসংখ্যা b = বাফার_Of_অস্থায়ী.empty() ? ai.সূচক : বাফার_Of_অস্থায়ী[ai.সূচক];  //অস্থায়ী
                    ফেরত scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                জন্য (পূর্ণসংখ্যা n = 0; n < ninsn; n++)
                {
                    ধ্রুবক Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.ফলাফল), 0, wf, 1);
                }
            }
            ফেরত;
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
    পূর্ণসংখ্যা rchannels;
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
    যৌক্তিক inplace = মিথ্যা;
    জন্য (পূর্ণসংখ্যা i = 0; i < ninputs && !inplace; i++)
        জন্য (পূর্ণসংখ্যা j = 0; j < noutputs; j++)
            যদি (inputs[i]->তথ্য && inputs[i]->তথ্য == outputs[j].তথ্য) { inplace = সত্য; বিরতি; }
    পূর্ণসংখ্যা nhdrs = inplace ? ninputs : 0;

    Auto_বাফার<ধ্রুবক Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    Auto_বাফার<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    Auto_বাফার<ধ্রুবক Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    Auto_বাফার<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    Auto_বাফার<পূর্ণসংখ্যা, LOCAL_OPS> slotMap(nslots);
    Auto_বাফার<signed অক্ষর, LOCAL_OPS> slotKind(nslots);
    পূর্ণসংখ্যা narr = 0, nc = 0;
    যদি (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        জন্য (পূর্ণসংখ্যা j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.তথ্য();
    }
    slotKind[0] = (signed অক্ষর)TExpr::NONE;
    জন্য (পূর্ণসংখ্যা s = 1; s < nslots; s++)
    {
        ধ্রুবক TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed অক্ষর)ai.kind;
        যদি (ai.kind == TExpr::প্রবেশ) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.সূচক];
        }
        অন্যথা যদি (ai.kind == TExpr::OUTPUT) {
            outputs[ai.সূচক].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.সূচক];
        }
        অন্যথা যদি (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed অক্ষর)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        অন্যথা যদি (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ধ্রুবক পূর্ণসংখ্যা c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (শূন্য*)(constbuf.তথ্য() + ai.constofs));
            slotKind[s] = (signed অক্ষর)TExpr::প্রবেশ;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        অন্যথা যদি (ai.kind == TExpr::অস্থায়ী) {
            slotMap[s] = বাফার_Of_অস্থায়ী.empty() ? ai.সূচক : বাফার_Of_অস্থায়ী[ai.সূচক];  // physical buffer id
        }
        অন্যথা {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ধ্রুবক পূর্ণসংখ্যা ninsn = (পূর্ণসংখ্যা)prog.size();
    long long costPerElem = 0;
    জন্য (পূর্ণসংখ্যা n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    জন্য (পূর্ণসংখ্যা d = 0; d < (পূর্ণসংখ্যা)spatial.size(); d++) otot *= spatial[d];
    যদি (otot == 0)
        ফেরত;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ধ্রুবক দ্বিগুণ T = (দ্বিগুণ)std::max(get_অঙ্ক_Threads(), 1);
    ধ্রুবক দ্বিগুণ nstripes = std::min(
        (দ্বিগুণ)otot * (দ্বিগুণ)std::max<long long>(costPerElem, 1) * (1./ (দ্বিগুণ)(1 << 16)),
        std::min(4.*T, std::max(32., 3.*T)));

    EwBody body;
    body.prog = prog.তথ্য();
    body.ninsn = ninsn;
    body.slotMap = slotMap.তথ্য();

    // ---- 5. drive: broadcastOp does geometry + 2D tiling + parallelism; the body runs the
    //         frozen program on each tile (temps tile-local, re-pointed from the tile slices).
    //         expandChannels=true => the body always sees single-channel data.
    //
    //         All state the body needs is packed into one POD (EwBody) so the lambda captures a
    //         SINGLE reference: the closure is then one pointer, fits std::function's small-buffer
    //         and never heap-allocates. Inside, the hot fields are copied into locals so the
    //         per-tile/per-insn loops read them from registers, not through the captured pointer. -

    body.bufEszPrefix = bufEszPrefix.তথ্য();
    body.slotKind = slotKind.তথ্য();
    body.nslots = nslots;
    body.nbuffers = nbuffers;
    body.capElems = capElems;

    broadcastOp(arr.তথ্য(), narr, [&](ধ্রুবক BrTile& tile)
    {
        EwBody& bc = body;
        ধ্রুবক TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ধ্রুবক পূর্ণসংখ্যা* ধ্রুবক smap  = bc.slotMap;
        ধ্রুবক signed অক্ষর* ধ্রুবক skind = bc.slotKind;
        ধ্রুবক পূর্ণসংখ্যা nin = bc.ninsn, nsl = bc.nslots;
        ধ্রুবক পূর্ণসংখ্যা w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ধ্রুবক পূর্ণসংখ্যা* ধ্রুবক eszPrefix = bc.bufEszPrefix;
        ধ্রুবক পূর্ণসংখ্যা bw = std::min(w, bc.capElems);
        ধ্রুবক পূর্ণসংখ্যা bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ধ্রুবক size_t region = align_আকার((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        Auto_বাফার<uint64_t, (4*1024 + 256)/আকার(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)আকার(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.তথ্য();

        Auto_বাফার<BrSlice, LOCAL_OPS> args(nsl);
        জন্য (পূর্ণসংখ্যা y0 = 0; y0 < h; y0 += bh)
        {
            ধ্রুবক পূর্ণসংখ্যা hf = std::min(bh, h - y0);
            জন্য (পূর্ণসংখ্যা x0 = 0; x0 < w; x0 += bw)
            {
                ধ্রুবক পূর্ণসংখ্যা wf = std::min(bw, w - x0);
                জন্য (পূর্ণসংখ্যা s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    ধ্রুবক পূর্ণসংখ্যা k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    যদি (k == TExpr::অস্থায়ী)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    অন্যথা                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ধ্রুবক BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                জন্য (পূর্ণসংখ্যা n = 0; n < nin; n++)
                {
                    ধ্রুবক TExpr::Insn& ins = insns[n];
                    ধ্রুবক BrSlice& a0 = args[ins.arg0]; ধ্রুবক BrSlice& a1 = args[ins.arg1];
                    ধ্রুবক BrSlice& a2 = args[ins.arg2]; ধ্রুবক BrSlice& rr = args[ins.ফলাফল];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (শূন্য*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, সত্য, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
শূন্য TExpr::exec(ধ্রুবক Mat* inputs, Mat* outputs)
{
    Auto_বাফার<ধ্রুবক Mat*, 16> ptrs(std::max(ninputs, 1));
    জন্য (পূর্ণসংখ্যা i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    exec(ptrs.তথ্য(), outputs);
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
শূন্য make_দ্বিমিক_Arith_Program(TExpr& p, শীর্ষ op, পূর্ণসংখ্যা depth0, পূর্ণসংখ্যা depth1, পূর্ণসংখ্যা rdepth,
                            পূর্ণসংখ্যা maskDepth, দ্বিগুণ scale)
{
    p.clear();
    যদি (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        যদি (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        অন্যথা
        {
            rdepth = promoteArith(depth0, depth1);
            যদি (op == OP_ABSDIFF) rdepth = absdiff_ফলাফল_Depth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ধ্রুবক যৌক্তিক masked = maskDepth != EW_DEPTH_NONE;
    পূর্ণসংখ্যা sIn0  = p.add_প্রবেশ(depth0);
    পূর্ণসংখ্যা sIn1  = p.add_প্রবেশ(depth1);
    পূর্ণসংখ্যা sMask = masked ? p.add_প্রবেশ(maskDepth) : 0;
    পূর্ণসংখ্যা sOut  = p.addOutput(rdepth);

    পূর্ণসংখ্যা r = p.emit_দ্বিমিক(op, sIn0, sIn1, rdepth, Scalar(scale));
    যদি (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    অন্যথা
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
শূন্য makeAddWeightedProgram(TExpr& p, পূর্ণসংখ্যা depth0, পূর্ণসংখ্যা depth1, পূর্ণসংখ্যা rdepth,
                            দ্বিগুণ alpha, দ্বিগুণ beta, দ্বিগুণ gamma)
{
    p.clear();
    পূর্ণসংখ্যা sA   = p.add_প্রবেশ(depth0);
    পূর্ণসংখ্যা sB   = p.add_প্রবেশ(depth1);
    পূর্ণসংখ্যা sOut = p.addOutput(rdepth);
    পূর্ণসংখ্যা r = p.emit_দ্বিমিক(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

নামাকাশ {

// --- tokens ------------------------------------------------------------------------------
গণনা TokType { T_অঙ্ক, T_প্রবেশ, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

কাঠামো Token
{
    TokType type = T_END;
    দ্বিগুণ num = 0;
    পূর্ণসংখ্যা প্রবেশ_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
কাঠামো Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    স্থির যৌক্তিক isIdentStart(অক্ষর c) { ফেরত std::isalpha((unsigned অক্ষর)c) || c == '_'; }
    স্থির যৌক্তিক isIdentChar(অক্ষর c)  { ফেরত std::isalnum((unsigned অক্ষর)c) || c == '_'; }

    Token next()
    {
        যখন (pos < s.size() && std::isspace((unsigned অক্ষর)s[pos])) pos++;
        Token t;
        যদি (pos >= s.size()) { t.type = T_END; ফেরত t; }

        অক্ষর c = s[pos];

        যদি (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            যখন (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_প্রবেশ;
            t.প্রবেশ_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            ফেরত t;
        }
        যদি (std::isdigit((unsigned অক্ষর)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned অক্ষর)s[pos + 1])))
        {
            অক্ষর* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_অঙ্ক;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            ফেরত t;
        }
        যদি (isIdentStart(c))
        {
            size_t start = pos;
            যখন (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            ফেরত t;
        }
        নির্বাচন (c)
        {
        ক্ষেত্র '(': pos++; t.type = T_LPAREN; ফেরত t;
        ক্ষেত্র ')': pos++; t.type = T_RPAREN; ফেরত t;
        ক্ষেত্র ',': pos++; t.type = T_COMMA;  ফেরত t;
        ক্ষেত্র ';': pos++; t.type = T_SEMI;   ফেরত t;
        }
        // multi/!single-char operators
        auto two = [&](ধ্রুবক অক্ষর* op) {
            ফেরত pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        যদি (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; ফেরত t; }
        যদি (c == '=') { pos++; t.type = T_ASSIGN; ফেরত t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        ফেরত t;
    }
};

// --- operator / function tables ----------------------------------------------------------
স্থির পূর্ণসংখ্যা binPrec(ধ্রুবক std::string& op)
{
    যদি (op == "**") ফেরত 8;              // power binds tighter than '*'; RIGHT-associative
    যদি (op == "*" || op == "/") ফেরত 7;
    যদি (op == "+" || op == "-") ফেরত 6;
    যদি (op == "<" || op == "<=" || op == ">" || op == ">=") ফেরত 5;
    যদি (op == "==" || op == "!=") ফেরত 4;
    যদি (op == "&") ফেরত 3;
    যদি (op == "^") ফেরত 2;
    যদি (op == "|") ফেরত 1;
    ফেরত -1;
}

স্থির শীর্ষ binOp(ধ্রুবক std::string& op)
{
    যদি (op == "**") ফেরত OP_POW;
    যদি (op == "+")  ফেরত OP_ADD;
    যদি (op == "-")  ফেরত OP_SUB;
    যদি (op == "*")  ফেরত OP_MUL;
    যদি (op == "/")  ফেরত OP_DIV;
    যদি (op == "<")  ফেরত OP_CMP_LT;
    যদি (op == "<=") ফেরত OP_CMP_LE;
    যদি (op == ">")  ফেরত OP_CMP_GT;
    যদি (op == ">=") ফেরত OP_CMP_GE;
    যদি (op == "==") ফেরত OP_CMP_EQ;
    যদি (op == "!=") ফেরত OP_CMP_NE;
    যদি (op == "&")  ফেরত OP_AND;
    যদি (op == "|")  ফেরত OP_OR;
    যদি (op == "^")  ফেরত OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
স্থির পূর্ণসংখ্যা typeDepth(ধ্রুবক std::string& name)
{
    যদি (name == "float")    ফেরত CV_32F;
    যদি (name == "double")   ফেরত CV_64F;
    যদি (name == "half" || name == "float16") ফেরত CV_16F;
    যদি (name == "bfloat16") ফেরত CV_16BF;
    যদি (name == "uint8")    ফেরত CV_8U;
    যদি (name == "int8")     ফেরত CV_8S;
    যদি (name == "uint16")   ফেরত CV_16U;
    যদি (name == "int16")    ফেরত CV_16S;
    যদি (name == "uint32")   ফেরত CV_32U;
    যদি (name == "int32")    ফেরত CV_32S;
    যদি (name == "uint64")   ফেরত CV_64U;
    যদি (name == "int64")    ফেরত CV_64S;
    ফেরত -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
স্থির শীর্ষ fnOp(ধ্রুবক std::string& name, পূর্ণসংখ্যা& arity)
{
    কাঠামো E { ধ্রুবক অক্ষর* n; শীর্ষ op; };
    স্থির ধ্রুবক E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    স্থির ধ্রুবক E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    স্থির ধ্রুবক E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    জন্য (ধ্রুবক E& e : unary)  যদি (name == e.n) { arity = 1; ফেরত e.op; }
    জন্য (ধ্রুবক E& e : binary) যদি (name == e.n) { arity = 2; ফেরত e.op; }
    জন্য (ধ্রুবক E& e : tern)   যদি (name == e.n) { arity = 3; ফেরত e.op; }
    arity = 0; ফেরত OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
কাঠামো Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ধ্রুবক পূর্ণসংখ্যা* প্রবেশ_Slot;                 // slot id of each external input (precreated)
    পূর্ণসংখ্যা ninputs;
    std::map<std::string, পূর্ণসংখ্যা> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& রাশিমালা, ধ্রুবক পূর্ণসংখ্যা* islot, পূর্ণসংখ্যা nin)
        : lex(src), e(রাশিমালা), প্রবেশ_Slot(islot), ninputs(nin) { cur = lex.next(); }

    শূন্য advance() { cur = lex.next(); }
    যৌক্তিক isOp(ধ্রুবক অক্ষর* op) ধ্রুবক { ফেরত cur.type == T_OP && cur.text == op; }

    শূন্য expect(TokType t, ধ্রুবক অক্ষর* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    যৌক্তিক isFlexConst(পূর্ণসংখ্যা slot) ধ্রুবক
    {
        ফেরত e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    পূর্ণসংখ্যা parsePrimary()
    {
        যদি (cur.type == T_অঙ্ক)   { পূর্ণসংখ্যা s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); ফেরত s; }
        যদি (cur.type == T_প্রবেশ) { পূর্ণসংখ্যা idx = cur.প্রবেশ_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   ফেরত প্রবেশ_Slot[idx]; }
        যদি (cur.type == T_LPAREN){ advance(); পূর্ণসংখ্যা x = parseTernary(); expect(T_RPAREN, "expected ')'"); ফেরত x; }
        যদি (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            যদি (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                ফেরত it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<পূর্ণসংখ্যা, 8> args = {}; পূর্ণসংখ্যা nargs = 0;
            যদি (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                যখন (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (পূর্ণসংখ্যা)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            পূর্ণসংখ্যা td = typeDepth(name);
            যদি (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                পূর্ণসংখ্যা s = args[0];
                ফেরত isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            পূর্ণসংখ্যা arity = 0; শীর্ষ op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            যদি (arity == 1) ফেরত e.emitUnary(op, args[0]);
            যদি (arity == 2) ফেরত e.emit_দ্বিমিক(op, args[0], args[1]);
            ফেরত e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    পূর্ণসংখ্যা parseUnary()
    {
        যদি (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            পূর্ণসংখ্যা operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            যদি (op == "-" && isFlexConst(operand))
            {
                Auto_বাফার<দ্বিগুণ, MAX_LOCAL_CN> v; পূর্ণসংখ্যা cn = constDoubles(e, operand, v);
                Scalar neg; জন্য (পূর্ণসংখ্যা i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                ফেরত e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            ফেরত e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        ফেরত parsePrimary();
    }

    পূর্ণসংখ্যা parse_রাশিমালা(পূর্ণসংখ্যা minPrec)
    {
        পূর্ণসংখ্যা বাম = parseUnary();
        যখন (cur.type == T_OP)
        {
            পূর্ণসংখ্যা p = binPrec(cur.text);
            যদি (p < minPrec) বিরতি;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            পূর্ণসংখ্যা ডান = parse_রাশিমালা(op == "**" ? p : p + 1);
            বাম = e.emit_দ্বিমিক(binOp(op), বাম, ডান);
        }
        ফেরত বাম;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    পূর্ণসংখ্যা parseTernary()
    {
        পূর্ণসংখ্যা cond = parse_রাশিমালা(0);
        যদি (!isOp("?")) ফেরত cond;
        advance();
        পূর্ণসংখ্যা thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        পূর্ণসংখ্যা elsev = parseTernary();               // right-associative chain
        ফেরত e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    শূন্য parse_ফলাফল()
    {
        যদি (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); পূর্ণসংখ্যা n_অস্থায়ী = e.ntemps;
            advance();
            পূর্ণসংখ্যা e0 = parseTernary();
            যদি (cur.type == T_COMMA)
            {
                e.output(e0);
                যখন (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                ফেরত;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = n_অস্থায়ী;
        }
        e.output(parseTernary());
    }

    শূন্য parse()
    {
        যখন (সত্য)
        {
            যদি (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                যদি (cur.type == T_ASSIGN)
                {
                    advance();
                    ধ্রুবক পূর্ণসংখ্যা slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = সত্য;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    চালু;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parse_ফলাফল();
            যদি (cur.type == T_SEMI) advance();
            বিরতি;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

নামাকাশ cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
শূন্য texpr(ধ্রুবক std::string& রাশিমালা, প্রবেশ_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using নামাকাশ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == প্রবেশ_Array_2::STD_VECTOR_MAT);
    ধ্রুবক std::vector<Mat>& inps = *(ধ্রুবক std::vector<Mat>*)_inputs.getObj();
    ধ্রুবক পূর্ণসংখ্যা ninputs = (পূর্ণসংখ্যা)inps.size();
    জন্য (পূর্ণসংখ্যা i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    Auto_বাফার<পূর্ণসংখ্যা> islot(std::max(ninputs, 1));
    জন্য (পূর্ণসংখ্যা i = 0; i < ninputs; i++) islot[i] = e.add_প্রবেশ(inps[i].depth());
    Parser(রাশিমালা, e, islot.তথ্য(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    যদি (kind == প্রবেশ_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.তথ্য(), outs.তথ্য());
    } অন্যথা {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
