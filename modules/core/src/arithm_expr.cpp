// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "arithm_expr.hpp"
#অন্তর্ভাৱ <algorithm>
#অন্তর্ভাৱ <climits>
#অন্তর্ভাৱ <cmath>
#অন্তর্ভাৱ <cstdlib>
#অন্তর্ভাৱ <cstring>
#অন্তর্ভাৱ <map>
#অন্তর্ভাৱ <ostream>
#অন্তর্ভাৱ <string>

নামাকাশ cv { নামাকাশ ew {

// ============================ op metadata (was ew_op.cpp) ============================

ধ্রুৱ ৱর্ণ* opName(শীৰ্ষ op)
{
    ৱিকল্প (op)
    {
    পক্ষ OP_NOP:           ঘূৰাই "nop";
    পক্ষ OP_NEG:           ঘূৰাই "neg";
    পক্ষ OP_ABS:           ঘূৰাই "abs";
    পক্ষ OP_NOT:           ঘূৰাই "not";
    পক্ষ OP_SQRT:          ঘূৰাই "sqrt";
    পক্ষ OP_EXP:           ঘূৰাই "exp";
    পক্ষ OP_LOG:           ঘূৰাই "log";
    পক্ষ OP_SIN:           ঘূৰাই "sin";
    পক্ষ OP_COS:           ঘূৰাই "cos";
    পক্ষ OP_TANH:          ঘূৰাই "tanh";
    পক্ষ OP_ERF:           ঘূৰাই "erf";
    পক্ষ OP_RELU:          ঘূৰাই "relu";
    পক্ষ OP_CAST:          ঘূৰাই "cast";
    পক্ষ OP_ADD:           ঘূৰাই "add";
    পক্ষ OP_SUB:           ঘূৰাই "sub";
    পক্ষ OP_MUL:           ঘূৰাই "mul";
    পক্ষ OP_DIV:           ঘূৰাই "div";
    পক্ষ OP_POW:           ঘূৰাই "pow";
    পক্ষ OP_MIN:           ঘূৰাই "min";
    পক্ষ OP_MAX:           ঘূৰাই "max";
    পক্ষ OP_ABSDIFF:       ঘূৰাই "absdiff";
    পক্ষ OP_ADDW:          ঘূৰাই "addWeighted";
    পক্ষ OP_HYPOT:         ঘূৰাই "hypot";
    পক্ষ OP_ATAN2:         ঘূৰাই "atan2";
    পক্ষ OP_AND:           ঘূৰাই "and";
    পক্ষ OP_OR:            ঘূৰাই "or";
    পক্ষ OP_XOR:           ঘূৰাই "xor";
    পক্ষ OP_CMP_EQ:        ঘূৰাই "cmp_eq";
    পক্ষ OP_CMP_NE:        ঘূৰাই "cmp_ne";
    পক্ষ OP_CMP_LT:        ঘূৰাই "cmp_lt";
    পক্ষ OP_CMP_LE:        ঘূৰাই "cmp_le";
    পক্ষ OP_CMP_GT:        ঘূৰাই "cmp_gt";
    পক্ষ OP_CMP_GE:        ঘূৰাই "cmp_ge";
    পক্ষ OP_CLAMP:         ঘূৰাই "clamp";
    পক্ষ OP_SELECT:        ঘূৰাই "select";
    পক্ষ OP_CONVERT_SCALE: ঘূৰাই "convert_scale";
    স্ৱভাৱ:               ঘূৰাই "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(শীৰ্ষ op)
{
    ৱিকল্প (op)
    {
    পক্ষ OP_AND: পক্ষ OP_OR: পক্ষ OP_XOR: পক্ষ OP_NOT:
        ঘূৰাই CAT_BITWISE;
    পক্ষ OP_CMP_EQ: পক্ষ OP_CMP_NE: পক্ষ OP_CMP_LT:
    পক্ষ OP_CMP_LE: পক্ষ OP_CMP_GT: পক্ষ OP_CMP_GE:
        ঘূৰাই CAT_COMPARE;
    পক্ষ OP_SQRT: পক্ষ OP_EXP: পক্ষ OP_LOG:
    পক্ষ OP_SIN: পক্ষ OP_COS: পক্ষ OP_TANH: পক্ষ OP_ERF: পক্ষ OP_ATAN2: পক্ষ OP_RELU:
        ঘূৰাই CAT_MATH;
    পক্ষ OP_CAST: পক্ষ OP_CONVERT_SCALE:
        ঘূৰাই CAT_CAST;
    পক্ষ OP_SELECT:
        ঘূৰাই CAT_SELECT;
    স্ৱভাৱ:
        ঘূৰাই CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
স্থির তার্কিক isFloatDepth(পূর্ণাঙ্ক d)
{
    ঘূৰাই d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
স্থির তার্কিক depthRepresents(দ্ৱিগুণ v, পূর্ণাঙ্ক depth)
{
    যদি (isFloatDepth(depth)) ঘূৰাই সঁচা;
    যদি (v != std::floor(v)) ঘূৰাই মিছা;
    ৱিকল্প (depth)
    {
    পক্ষ CV_8U:  ঘূৰাই v >= 0            && v <= 255;
    পক্ষ CV_8S:  ঘূৰাই v >= -128         && v <= 127;
    পক্ষ CV_16U: ঘূৰাই v >= 0            && v <= 65535;
    পক্ষ CV_16S: ঘূৰাই v >= -32768       && v <= 32767;
    পক্ষ CV_32U: ঘূৰাই v >= 0            && v <= 4294967295.0;
    পক্ষ CV_32S: ঘূৰাই v >= -2147483648.0 && v <= 2147483647.0;
    পক্ষ CV_64U: ঘূৰাই v >= 0            && v <= 18446744073709551615.0;
    পক্ষ CV_64S: ঘূৰাই v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    স্ৱভাৱ:     ঘূৰাই মিছা;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
স্থির শূন্য intRange(পূর্ণাঙ্ক depth, দ্ৱিগুণ& lo, দ্ৱিগুণ& hi)
{
    ৱিকল্প (depth)
    {
    পক্ষ CV_8U:  lo = 0;                     hi = 255; ভঙ্গ;
    পক্ষ CV_8S:  lo = -128;                  hi = 127; ভঙ্গ;
    পক্ষ CV_16U: lo = 0;                     hi = 65535; ভঙ্গ;
    পক্ষ CV_16S: lo = -32768;                hi = 32767; ভঙ্গ;
    পক্ষ CV_32U: lo = 0;                     hi = 4294967295.0; ভঙ্গ;
    পক্ষ CV_32S: lo = -2147483648.0;         hi = 2147483647.0; ভঙ্গ;
    পক্ষ CV_64U: lo = 0;                     hi = 18446744073709551615.0; ভঙ্গ;
    পক্ষ CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; ভঙ্গ;
    স্ৱভাৱ:     lo = 0;                     hi = 0; ভঙ্গ;
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
পূর্ণাঙ্ক promoteArith(পূর্ণাঙ্ক a, পূর্ণাঙ্ক b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    যদি (a == b) ঘূৰাই a;

    পূর্ণাঙ্ক wa = পূর্ণাঙ্ক((leszlut >> (a+1)*3) & 7u), wb = পূর্ণাঙ্ক((leszlut >> (b+1)*3) & 7u);
    পূর্ণাঙ্ক fa = পূর্ণাঙ্ক((fltlut >> (a+1)) & 1u),    fb = পূর্ণাঙ্ক((fltlut >> (b+1)) & 1u);

    যদি (fa + fb == 0)                          // both integer
    {
        পূর্ণাঙ্ক sa = পূর্ণাঙ্ক((signlut >> (a+1)) & 1u), sb = পূর্ণাঙ্ক((signlut >> (b+1)) & 1u);
        যদি (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            ঘূৰাই wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        পূর্ণাঙ্ক rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr পূর্ণাঙ্ক ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        ঘূৰাই (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    পূর্ণাঙ্ক maxw = std::max(wa, wb);
    যদি (maxw == 2 && fa + fb == 1)
        ঘূৰাই fa*a + (1 - fa)*b;
    ঘূৰাই CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
পূর্ণাঙ্ক absdiffResultDepth(পূর্ণাঙ্ক depth)
{
    ৱিকল্প (depth)
    {
    পক্ষ CV_8S:  ঘূৰাই CV_8U;
    পক্ষ CV_16S: ঘূৰাই CV_16U;
    পক্ষ CV_32S: ঘূৰাই CV_32U;
    পক্ষ CV_64S: ঘূৰাই CV_64U;
    স্ৱভাৱ:     ঘূৰাই depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
স্থির পূর্ণাঙ্ক promote2(পূর্ণাঙ্ক a, পূর্ণাঙ্ক b)
{
    যদি (a == b) ঘূৰাই a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ধ্রুৱ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ধ্রুৱ unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ঘূৰাই পূর্ণাঙ্ক((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
স্থির পূর্ণাঙ্ক promoteBitwise(পূর্ণাঙ্ক a, পূর্ণাঙ্ক b)
{
    যদি (a == EW_DEPTH_NONE) ঘূৰাই b;
    যদি (b == EW_DEPTH_NONE) ঘূৰাই a;
    যদি (a == b) ঘূৰাই a;
    ঘূৰাই CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
স্থির পূর্ণাঙ্ক safeWide(পূর্ণাঙ্ক depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ধ্রুৱ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ধ্রুৱ unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ঘূৰাই পূর্ণাঙ্ক((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
গণনা { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
স্থির size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, পূর্ণাঙ্ক srcdepth, ধ্রুৱ শূন্য* data, পূর্ণাঙ্ক channels)
{
    ধ্রুৱ পূর্ণাঙ্ক cn = std::max(1, channels);
    ধ্রুৱ size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ধ্রুৱ size_t nu64 = (nbytes + পরিমাণ(uint64_t) - 1) / পরিমাণ(uint64_t);
    ধ্রুৱ size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    যদি (data) memcpy(dst, data, nbytes); অন্যথা memset(dst, 0, nbytes);
    ঘূৰাই ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
স্থির পূর্ণাঙ্ক constDoubles(ধ্রুৱ TExpr& e, পূর্ণাঙ্ক s, AutoBuffer<দ্ৱিগুণ, MAX_LOCAL_CN>& buf)
{
    ধ্রুৱ TExpr::Arg& a = e.arginfo[s];
    ধ্রুৱ পূর্ণাঙ্ক cn = std::max(1, a.channels);
    buf.resize(cn);
    ধ্রুৱ uchar* src = (ধ্রুৱ uchar*)(e.constbuf.data() + a.constofs);
    যদি (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * পরিমাণ(দ্ৱিগুণ));   // common (Scalar / parsed literal)
    অন্যথা
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    ঘূৰাই cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
স্থির অন্তঃস্থ তার্কিক isFlexConst(ধ্রুৱ TExpr& e, পূর্ণাঙ্ক s)
{
    ঘূৰাই e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
স্থির তার্কিক scalar_Const_মান(ধ্রুৱ TExpr& e, পূর্ণাঙ্ক s, দ্ৱিগুণ& v)
{
    ধ্রুৱ TExpr::Arg& a = e.arginfo[s];
    যদি (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) ঘূৰাই মিছা;
    AutoBuffer<দ্ৱিগুণ, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    ঘূৰাই সঁচা;
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
স্থির তার্কিক matchScaledChain(ধ্রুৱ TExpr& e, পূর্ণাঙ্ক end, পূর্ণাঙ্ক t,
                             পূর্ণাঙ্ক& arrSlot, দ্ৱিগুণ& scale, পূর্ণাঙ্ক& startIdx)
{
    যদি (end < 0) ঘূৰাই মিছা;
    পূর্ণাঙ্ক j = end, mulResult = t;

    // optional trailing narrowing cast
    যদি (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        যদি (--j < 0) ঘূৰাই মিছা;
    }

    ধ্রুৱ TExpr::Insn& mul = e.prog[j];
    যদি (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) ঘূৰাই মিছা;

    পূর্ণাঙ্ক arr;
    যদি      (scalar_Const_মান(e, mul.arg1, scale)) arr = mul.arg0;
    অন্যথা যদি (scalar_Const_মান(e, mul.arg0, scale)) arr = mul.arg1;
    অন্যথা ঘূৰাই মিছা;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    যদি (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    যদি (e.arginfo[arr].kind == TExpr::CONST) ঘূৰাই মিছা;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    ঘূৰাই সঁচা;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
স্থির তার্কিক constFits(ধ্রুৱ TExpr& e, পূর্ণাঙ্ক s, পূর্ণাঙ্ক d)
{
    যদি (!isFlexConst(e, s)) ঘূৰাই সঁচা;
    AutoBuffer<দ্ৱিগুণ, MAX_LOCAL_CN> v;
    পূর্ণাঙ্ক cn = constDoubles(e, s, v);
    বাবে (পূর্ণাঙ্ক ch = 0; ch < cn; ch++)
        যদি (!depthRepresents(v[ch], d)) ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
পূর্ণাঙ্ক TExpr::emit_দ্বিমিক(শীৰ্ষ op, পূর্ণাঙ্ক a, পূর্ণাঙ্ক b, পূর্ণাঙ্ক rdepth, ধ্রুৱ Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    যদি (op == OP_ADDW)
    {
        পূর্ণাঙ্ক Tt = arginfo[a].depth;
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
        পূর্ণাঙ্ক outD = rdepth;
        যদি (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ধ্রুৱ পূর্ণাঙ্ক out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        যদি (outD == rdepth) ঘূৰাই out;
        ধ্রুৱ পূর্ণাঙ্ক out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        ঘূৰাই out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    যদি (op == OP_ADD)
    {
        ধ্রুৱ পূর্ণাঙ্ক n = (পূর্ণাঙ্ক)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        পূর্ণাঙ্ক x = 0, y = 0, startA = 0, startB = 0;
        দ্ৱিগুণ alpha = 0, beta = 0;
        যদি (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*এতত্, n - 1, b, y, beta, startB) &&
            matchScaledChain(*এতত্, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            পূর্ণাঙ্ক retired = 0, lowest = INT_MAX;
            বাবে (পূর্ণাঙ্ক i = startA; i < n; i++)
            {
                ধ্রুৱ পূর্ণাঙ্ক r = prog[i].result;
                যদি (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; ভঙ্গ; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            যদি (retired > 0 && lowest == ntemps - retired)
            {
                বাবে (পূর্ণাঙ্ক i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                ঘূৰাই emit_দ্বিমিক(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            পূর্ণাঙ্ক acc = 0; দ্ৱিগুণ gamma = 0;
            যদি      (scalar_Const_মান(*এতত্, b, gamma)) acc = a;
            অন্যথা যদি (scalar_Const_মান(*এতত্, a, gamma)) acc = b;
            যদি (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                ঘূৰাই acc;
            }
        }
    }

    ধ্রুৱ পূর্ণাঙ্ক nd0 = isFlexConst(*এতত্, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ধ্রুৱ পূর্ণাঙ্ক nd1 = isFlexConst(*এতত্, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    ধ্রুৱ ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    পূর্ণাঙ্ক result = rdepth;
    যদি (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        যদি (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    পূর্ণাঙ্ক base;
    যদি (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
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
        isFlexConst(*এতত্, b) && !constFits(*এতত্, b, base))
    {
        দ্ৱিগুণ lo, hi; intRange(nd0, lo, hi);
        যদি (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            গণনা { REAL, CFALSE, CTRUE };
            AutoBuffer<দ্ৱিগুণ, MAX_LOCAL_CN> tv; পূর্ণাঙ্ক cn = constDoubles(*এতত্, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            পূর্ণাঙ্ক kind[4] = {}; দ্ৱিগুণ bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ধ্রুৱ শীৰ্ষ fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            বাবে (পূর্ণাঙ্ক c = 0; c < cn; c++)
            {
                ধ্রুৱ দ্ৱিগুণ t = tv[c]; পূর্ণাঙ্ক k; দ্ৱিগুণ B = t;
                ৱিকল্প (op)
                {
                পক্ষ OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ভঙ্গ;
                পক্ষ OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ভঙ্গ;
                পক্ষ OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ভঙ্গ;
                পক্ষ OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ভঙ্গ;
                পক্ষ OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; ভঙ্গ;   // a==non-rep -> false
                স্ৱভাৱ:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  ভঙ্গ;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](পূর্ণাঙ্ক c){ ঘূৰাই kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](পূর্ণাঙ্ক c){ ঘূৰাই kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ধ্রুৱ শীৰ্ষ u0 = ucOp(0);
            তার্কিক uniform = সঁচা;
            বাবে (পূর্ণাঙ্ক c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            যদি (uniform)
            {
                Scalar s; বাবে (পূর্ণাঙ্ক c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            অন্যথা
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; পূর্ণাঙ্ক patchFlags = 0;
                বাবে (পূর্ণাঙ্ক c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ধ্রুৱ পূর্ণাঙ্ক mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ধ্রুৱ পূর্ণাঙ্ক vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ধ্রুৱ পূর্ণাঙ্ক out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                ঘূৰাই out;
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
    পূর্ণাঙ্ক depth, wide;
    তার্কিক divGuard = মিছা;
    ৱিকল্প (op)
    {
    পক্ষ OP_MIN: পক্ষ OP_MAX:
    পক্ষ OP_AND: পক্ষ OP_OR: পক্ষ OP_XOR:
        depth = result; wide = result; ভঙ্গ;
    পক্ষ OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        পূর্ণাঙ্ক common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        যদি (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        যদি (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        ভঙ্গ;
    }
    পক্ষ OP_CMP_EQ: পক্ষ OP_CMP_NE: পক্ষ OP_CMP_LT:
    পক্ষ OP_CMP_LE: পক্ষ OP_CMP_GT: পক্ষ OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; ভঙ্গ;
    পক্ষ OP_MUL: পক্ষ OP_DIV:
    {
        depth = base;
        ধ্রুৱ তার্কিক w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        যদি (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](পূর্ণাঙ্ক s, পূর্ণাঙ্ক nd) {
                যদি (nd != EW_DEPTH_NONE) ঘূৰাই !isFloatDepth(nd);
                AutoBuffer<দ্ৱিগুণ, MAX_LOCAL_CN> v; constDoubles(*এতত্, s, v);
                ঘূৰাই v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        ভঙ্গ;
    }
    পক্ষ OP_POW: পক্ষ OP_HYPOT: পক্ষ OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; ভঙ্গ;
    স্ৱভাৱ:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); ভঙ্গ;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](পূর্ণাঙ্ক s) {
        যদি (!isFlexConst(*এতত্, s)) ঘূৰাই মিছা;
        AutoBuffer<দ্ৱিগুণ, MAX_LOCAL_CN> v; পূর্ণাঙ্ক n = constDoubles(*এতত্, s, v);
        বাবে (পূর্ণাঙ্ক ch = 0; ch < n; ch++)
            যদি (v[ch] != std::floor(v[ch])) ঘূৰাই সঁচা;
        ঘূৰাই মিছা;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    যদি (cat != CAT_BITWISE && (!constFits(*এতত্, a, depth) || !constFits(*এতত্, b, depth)))
    {
        depth = wide;
        ধ্রুৱ তার্কিক frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        যদি (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        যদি (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    পূর্ণাঙ্ক s0 = isFlexConst(*এতত্, a) ? typedConstFrom(a, depth) : a;
    পূর্ণাঙ্ক s1 = isFlexConst(*এতত্, b) ? typedConstFrom(b, depth) : b;
    পূর্ণাঙ্ক c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](পূর্ণাঙ্ক rd) {
        ঘূৰাই op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    যদি (k.fptr)
    {
        পূর্ণাঙ্ক res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        ঘূৰাই res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    পূর্ণাঙ্ক w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    ঘূৰাই maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
পূর্ণাঙ্ক TExpr::emitUnary(শীৰ্ষ op, পূর্ণাঙ্ক a, পূর্ণাঙ্ক rdepth, ধ্রুৱ Scalar& params)
{
    ধ্রুৱ পূর্ণাঙ্ক nd = isFlexConst(*এতত্, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ধ্রুৱ ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    যদি (op == OP_NEG)
        ঘূৰাই emit_দ্বিমিক(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    যদি (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        যদি (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            ধ্রুৱ পূর্ণাঙ্ক x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            ঘূৰাই emit_দ্বিমিক(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        ঘূৰাই emit_দ্বিমিক(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    পূর্ণাঙ্ক result = rdepth;
    যদি (result == EW_DEPTH_NONE)
    {
        ৱিকল্প (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        পক্ষ CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; ভঙ্গ;
        পক্ষ CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; ভঙ্গ;  // NOT
        স্ৱভাৱ:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; ভঙ্গ;  // NEG, ABS
        }
    }

    পূর্ণাঙ্ক depth, wide;
    ৱিকল্প (cat)
    {
    পক্ষ CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        ভঙ্গ;
    পক্ষ CAT_BITWISE: depth = result; wide = result; ভঙ্গ;
    স্ৱভাৱ:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; ভঙ্গ;  // NEG/ABS
    }

    যদি (!constFits(*এতত্, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    পূর্ণাঙ্ক s0 = isFlexConst(*এতত্, a) ? typedConstFrom(a, depth) : a;
    পূর্ণাঙ্ক c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    যদি (k.fptr)
    {
        পূর্ণাঙ্ক res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        ঘূৰাই res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    পূর্ণাঙ্ক w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    ঘূৰাই maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
পূর্ণাঙ্ক TExpr::emitTernary(শীৰ্ষ op, পূর্ণাঙ্ক a, পূর্ণাঙ্ক b, পূর্ণাঙ্ক c, পূর্ণাঙ্ক rdepth)
{
    auto dep = [&](পূর্ণাঙ্ক s) { ঘূৰাই isFlexConst(*এতত্, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    যদি (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        যদি (isFlexConst(*এতত্, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_দ্বিমিক(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        পূর্ণাঙ্ক result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        যদি (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        পূর্ণাঙ্ক sb = isFlexConst(*এতত্, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        পূর্ণাঙ্ক sc = isFlexConst(*এতত্, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        পূর্ণাঙ্ক res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        ঘূৰাই res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    পূর্ণাঙ্ক result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    যদি (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](পূর্ণাঙ্ক s) {
        ঘূৰাই isFlexConst(*এতত্, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    পূর্ণাঙ্ক c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    পূর্ণাঙ্ক res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    ঘূৰাই res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
পূর্ণাঙ্ক TExpr::moveToOutput(পূর্ণাঙ্ক temp, পূর্ণাঙ্ক out)
{
    ধ্রুৱ পূর্ণাঙ্ক ninsn = (পূর্ণাঙ্ক)prog.size();
    পূর্ণাঙ্ক producer = -1;
    তার্কিক usedAsArg = মিছা;
    বাবে (পূর্ণাঙ্ক i = 0; i < ninsn; i++)
    {
        ধ্রুৱ TExpr::Insn& ins = prog[i];
        যদি (ins.result == temp) producer = i;
        যদি (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = সঁচা;
    }
    যদি (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        ঘূৰাই out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    ঘূৰাই out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
পূর্ণাঙ্ক TExpr::output(পূর্ণাঙ্ক মূল_Slot)
{
    ঘূৰাই moveToOutput(মূল_Slot, addOutput(arginfo[মূল_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
শূন্য TExpr::compile()
{
    ধ্রুৱ পূর্ণাঙ্ক ninsn = (পূর্ণাঙ্ক)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ধ্রুৱ পূর্ণাঙ্ক nslots = (পূর্ণাঙ্ক)arginfo.size();
    nconsts = 0;
    বাবে (পূর্ণাঙ্ক s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        যদি (a.kind != CONST || a.depth == EW_DEPTH_NONE) অনুৱর্ত;
        ধ্রুৱ পূর্ণাঙ্ক cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        ধ্রুৱ size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, পরিমাণ(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)পরিমাণ(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (ধ্রুৱ uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        ধ্রুৱ size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        যদি (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        অন্যথা getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
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
    যদি (ntemps == 0) ঘূৰাই;

    AutoBuffer<পূর্ণাঙ্ক, 32> tempOfSlot(nslots);
    বাবে (পূর্ণাঙ্ক s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    বাবে (পূর্ণাঙ্ক s = 1; s < nslots; s++)
        যদি (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<পূর্ণাঙ্ক, 16> lastUse(ntemps);
    বাবে (পূর্ণাঙ্ক t = 0; t < ntemps; t++) lastUse[t] = -1;
    বাবে (পূর্ণাঙ্ক i = 0; i < ninsn; i++)
    {
        ধ্রুৱ TExpr::Insn& ins = prog[i];
        পূর্ণাঙ্ক as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        বাবে (পূর্ণাঙ্ক k = 0; k < 3; k++)
            যদি (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    বাবে (পূর্ণাঙ্ক t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<পূর্ণাঙ্ক, 16> freeBufs(ntemps);
    পূর্ণাঙ্ক nfree = 0, nbuf = 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < ninsn; i++)
    {
        ধ্রুৱ TExpr::Insn& ins = prog[i];
        পূর্ণাঙ্ক rt = tempOfSlot[ins.result];
        যদি (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        পূর্ণাঙ্ক as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        বাবে (পূর্ণাঙ্ক k = 0; k < 3; k++)
        {
            পূর্ণাঙ্ক t = tempOfSlot[as[k]];
            যদি (t >= 0 && lastUse[t] == i)
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
    বাবে (পূর্ণাঙ্ক b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    বাবে (পূর্ণাঙ্ক s = 1; s < nslots; s++)
        যদি (arginfo[s].kind == TEMP)
        {
            পূর্ণাঙ্ক b = bufferOfTemp[arginfo[s].index], e = (পূর্ণাঙ্ক)CV_ELEM_SIZE1(arginfo[s].depth);
            যদি (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    বাবে (পূর্ণাঙ্ক b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ধ্রুৱ পূর্ণাঙ্ক totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
স্থির শূন্য matLogical(ধ্রুৱ Mat& m, MatShape& shp, EwSteps& step, পূর্ণাঙ্ক& esz1)
{
    esz1 = (পূর্ণাঙ্ক)m.elemSize1();
    পূর্ণাঙ্ক nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    বাবে (পূর্ণাঙ্ক i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
স্থির তার্কিক broadcastShape(ধ্রুৱ MatShape* shps, পূর্ণাঙ্ক K, MatShape& out)
{
    পূর্ণাঙ্ক nd = 0;
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
    {
        ধ্রুৱ MatShape& s = shps[k];
        পূর্ণাঙ্ক off = nd - s.dims;
        বাবে (পূর্ণাঙ্ক i = 0; i < s.dims; i++)
        {
            পূর্ণাঙ্ক d = s[i], &o = out[off + i];
            যদি (o == 1) o = d;
            অন্যথা যদি (d != 1 && d != o) ঘূৰাই মিছা;
        }
    }
    ঘূৰাই সঁচা;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
স্থির পূর্ণাঙ্ক opCost(শীৰ্ষ op)
{
    ৱিকল্প (op)
    {
    পক্ষ OP_ADD: পক্ষ OP_SUB: পক্ষ OP_MUL: পক্ষ OP_MIN: পক্ষ OP_MAX:
    পক্ষ OP_ABSDIFF: পক্ষ OP_AND: পক্ষ OP_OR: পক্ষ OP_XOR: পক্ষ OP_NOT:
    পক্ষ OP_NEG: পক্ষ OP_ABS: পক্ষ OP_CAST: পক্ষ OP_RELU: পক্ষ OP_SELECT:
    পক্ষ OP_CMP_EQ: পক্ষ OP_CMP_NE: পক্ষ OP_CMP_LT:
    পক্ষ OP_CMP_LE: পক্ষ OP_CMP_GT: পক্ষ OP_CMP_GE: ঘূৰাই 1;
    পক্ষ OP_DIV: পক্ষ OP_SQRT: পক্ষ OP_HYPOT: পক্ষ OP_CONVERT_SCALE: ঘূৰাই 3;
    পক্ষ OP_SIN: পক্ষ OP_COS: পক্ষ OP_TANH: পক্ষ OP_ERF: পক্ষ OP_ATAN2:
    পক্ষ OP_EXP: পক্ষ OP_LOG: পক্ষ OP_POW:           ঘূৰাই 6;
    স্ৱভাৱ:                                         ঘূৰাই 3;
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
    bufferOfTemp.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
শূন্য TExpr::dump(std::ostream& os) ধ্রুৱ
{
    auto dn = [](পূর্ণাঙ্ক d) -> ধ্রুৱ ৱর্ণ* {
        ৱিকল্প (d) {
        পক্ষ EW_DEPTH_NONE: ঘূৰাই "flex";
        পক্ষ CV_8U:  ঘূৰাই "u8";  পক্ষ CV_8S:  ঘূৰাই "s8";
        পক্ষ CV_16U: ঘূৰাই "u16"; পক্ষ CV_16S: ঘূৰাই "s16";
        পক্ষ CV_32U: ঘূৰাই "u32"; পক্ষ CV_32S: ঘূৰাই "s32";
        পক্ষ CV_64U: ঘূৰাই "u64"; পক্ষ CV_64S: ঘূৰাই "s64";
        পক্ষ CV_16F: ঘূৰাই "f16"; পক্ষ CV_16BF:ঘূৰাই "bf16";
        পক্ষ CV_32F: ঘূৰাই "f32"; পক্ষ CV_64F: ঘূৰাই "f64";
        পক্ষ CV_Bool:ঘূৰাই "bool";
        স্ৱভাৱ:     ঘূৰাই "?"; }
    };
    auto kn = [](ArgKind k) -> ধ্রুৱ ৱর্ণ* {
        ৱিকল্প (k) { পক্ষ NONE: ঘূৰাই "none"; পক্ষ প্ৰবেশ: ঘূৰাই "in"; পক্ষ CONST: ঘূৰাই "const";
                     পক্ষ TEMP: ঘূৰাই "temp"; পক্ষ OUTPUT: ঘূৰাই "out"; } ঘূৰাই "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (পূর্ণাঙ্ক)prog.size() << "\n";
    os << "  slots:\n";
    বাবে (পূর্ণাঙ্ক s = 0; s < (পূর্ণাঙ্ক)arginfo.size(); s++) {
        ধ্রুৱ Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        যদি (a.kind != NONE) os << " " << dn(a.depth);
        যদি (a.kind == প্ৰবেশ || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        যদি (a.kind == CONST) {
            ধ্রুৱ পূর্ণাঙ্ক cn = std::max(1, a.channels);
            AutoBuffer<দ্ৱিগুণ, MAX_LOCAL_CN> v(cn);
            ধ্রুৱ uchar* src = (ধ্রুৱ uchar*)(constbuf.data() + a.constofs);
            যদি (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * পরিমাণ(দ্ৱিগুণ));
            অন্যথা getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            বাবে (পূর্ণাঙ্ক c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    বাবে (পূর্ণাঙ্ক i = 0; i < (পূর্ণাঙ্ক)prog.size(); i++) {
        ধ্রুৱ Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        যদি (ins.arg1) os << ", " << ins.arg1;
        যদি (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        যদি (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        যদি (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        যদি (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
পূর্ণাঙ্ক TExpr::add_প্ৰবেশ(পূর্ণাঙ্ক depth)
{
    Arg a; a.kind = প্ৰবেশ; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); ঘূৰাই (পূর্ণাঙ্ক)arginfo.size() - 1;
}

পূর্ণাঙ্ক TExpr::addOutput(পূর্ণাঙ্ক depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); ঘূৰাই (পূর্ণাঙ্ক)arginfo.size() - 1;
}

পূর্ণাঙ্ক TExpr::addTemp(পূর্ণাঙ্ক depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); ঘূৰাই (পূর্ণাঙ্ক)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
পূর্ণাঙ্ক TExpr::addConst(পূর্ণাঙ্ক depth, ধ্রুৱ Scalar& v, পূর্ণাঙ্ক channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); ঘূৰাই (পূর্ণাঙ্ক)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
পূর্ণাঙ্ক TExpr::addConst(পূর্ণাঙ্ক depth, পূর্ণাঙ্ক srcdepth, ধ্রুৱ শূন্য* data, পূর্ণাঙ্ক channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); ঘূৰাই (পূর্ণাঙ্ক)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
পূর্ণাঙ্ক TExpr::typedConstFrom(পূর্ণাঙ্ক srcSlot, পূর্ণাঙ্ক depth)
{
    ধ্রুৱ Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); ঘূৰাই (পূর্ণাঙ্ক)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
পূর্ণাঙ্ক TExpr::addInsn(শীৰ্ষ op, পূর্ণাঙ্ক a0, পূর্ণাঙ্ক a1, পূর্ণাঙ্ক a2, পূর্ণাঙ্ক result, ধ্রুৱ TKernel& kernel, ধ্রুৱ Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    ঘূৰাই (পূর্ণাঙ্ক)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
পূর্ণাঙ্ক TExpr::addInsn(শীৰ্ষ op, পূর্ণাঙ্ক a0, পূর্ণাঙ্ক a1, পূর্ণাঙ্ক a2, পূর্ণাঙ্ক result, ধ্রুৱ Scalar& params)
{
    পূর্ণাঙ্ক d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    পূর্ণাঙ্ক d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    পূর্ণাঙ্ক d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ঘূৰাই addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
পূর্ণাঙ্ক TExpr::maybeAddCast(পূর্ণাঙ্ক arg, পূর্ণাঙ্ক depth)
{
    যদি (arginfo[arg].depth == depth) ঘূৰাই arg;
    পূর্ণাঙ্ক t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    ঘূৰাই t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
স্থির অন্তঃস্থ শূন্য runInsn(ধ্রুৱ TExpr::Insn& ins,
                           ধ্রুৱ শূন্য* p0, size_t y0, size_t x0,
                           ধ্রুৱ শূন্য* p1, size_t y1, size_t x1,
                           ধ্রুৱ শূন্য* p2, size_t y2, size_t x2,
                           শূন্য* pr, size_t yr, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    পূর্ণাঙ্ক code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

সংরচনা EwBody {
    ধ্রুৱ TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ধ্রুৱ পূর্ণাঙ্ক* slotMap;
    ধ্রুৱ signed ৱর্ণ* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ধ্রুৱ পূর্ণাঙ্ক* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    পূর্ণাঙ্ক ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

শূন্য TExpr::outputShape(ধ্রুৱ Mat* ধ্রুৱ* inputs, MatShape& spatial, পূর্ণাঙ্ক& channels) ধ্রুৱ
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ধ্রুৱ পূর্ণাঙ্ক rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    তার্কিক sameShape = ninputs >= 1;
    বাবে (পূর্ণাঙ্ক i = 1; sameShape && i < ninputs; i++)
    {
        ধ্রুৱ Mat& a = *inputs[i];
        যদি (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = মিছা;
        অন্যথা বাবে (পূর্ণাঙ্ক d = 0; d < a.dims; d++) যদি (a.size[d] != inputs[0]->size[d]) { sameShape = মিছা; ভঙ্গ; }
    }
    যদি (sameShape) { spatial = inputs[0]->size; channels = rcn0; ঘূৰাই; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; পূর্ণাঙ্ক esz1;
    বাবে (পূর্ণাঙ্ক i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    ধ্রুৱ পূর্ণাঙ্ক ndFull = (পূর্ণাঙ্ক)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
শূন্য TExpr::outputShape(ধ্রুৱ Mat* inputs, MatShape& spatial, পূর্ণাঙ্ক& channels) ধ্রুৱ
{
    AutoBuffer<ধ্রুৱ Mat*, 16> ptrs(std::max(ninputs, 1));
    বাবে (পূর্ণাঙ্ক i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
স্থির constexpr পূর্ণাঙ্ক LOCAL_HDRS = 3;
স্থির constexpr পূর্ণাঙ্ক LOCAL_CONSTS = 4;
স্থির constexpr পূর্ণাঙ্ক LOCAL_OPS = 16;

শূন্য TExpr::exec(ধ্রুৱ Mat* ধ্রুৱ* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    ধ্রুৱ পূর্ণাঙ্ক nslots = (পূর্ণাঙ্ক)arginfo.size();
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
        ধ্রুৱ Mat& r = *inputs[0];
        ধ্রুৱ পূর্ণাঙ্ক rcn = r.channels();
        তার্কিক ok = r.isContinuous();
        বাবে (পূর্ণাঙ্ক i = 1; ok && i < ninputs; i++)
        {
            ধ্রুৱ Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        বাবে (পূর্ণাঙ্ক s = 1; ok && s < nslots; s++)
            যদি (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        ধ্রুৱ size_t total = ok ? r.total() * rcn : 0;
        যদি (ok && total == 0)
        {
            ধ্রুৱ MatShape rshape = r.size;
            বাবে (পূর্ণাঙ্ক s = 1; s < nslots; s++)
                যদি (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ঘূৰাই;
        }
        যদি (ok && total <= EW_FASTPATH_MAX)
        {
            ধ্রুৱ MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            বাবে (পূর্ণাঙ্ক s = 1; s < nslots; s++)
                যদি (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ধ্রুৱ পূর্ণাঙ্ক ninsn = (পূর্ণাঙ্ক)prog.size();

            যদি (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](পূর্ণাঙ্ক s) -> শূন্য* {
                    যদি (s <= 0) ঘূৰাই nullptr;
                    ধ্রুৱ Arg& ai = arginfo[s];
                    যদি (ai.kind == প্ৰবেশ)  ঘূৰাই (শূন্য*)inputs[ai.index]->data;
                    যদি (ai.kind == OUTPUT) ঘূৰাই (শূন্য*)outputs[ai.index].data;
                    ঘূৰাই nullptr;   // NONE
                };
                ধ্রুৱ পূর্ণাঙ্ক w = (পূর্ণাঙ্ক)total;
                বাবে (পূর্ণাঙ্ক n = 0; n < ninsn; n++)
                {
                    ধ্রুৱ Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                ঘূৰাই;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ধ্রুৱ পূর্ণাঙ্ক totalEsz = bufEszPrefix[nbuffers];
            ধ্রুৱ পূর্ণাঙ্ক wf0 = std::min((পূর্ণাঙ্ক)total, capElems);

            ধ্রুৱ size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/পরিমাণ(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)পরিমাণ(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            বাবে (পূর্ণাঙ্ক x0 = 0; x0 < (পূর্ণাঙ্ক)total; x0 += wf0)
            {
                ধ্রুৱ পূর্ণাঙ্ক wf = std::min(wf0, (পূর্ণাঙ্ক)total - x0);
                auto ptrOf = [&](পূর্ণাঙ্ক s) -> শূন্য* {
                    যদি (s <= 0) ঘূৰাই nullptr;
                    ধ্রুৱ Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    যদি (ai.kind == প্ৰবেশ)  ঘূৰাই (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    যদি (ai.kind == OUTPUT) ঘূৰাই (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    যদি (ai.kind != TEMP)   ঘূৰাই nullptr;                                    // NONE: moved-from
                    ধ্রুৱ পূর্ণাঙ্ক b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    ঘূৰাই scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                বাবে (পূর্ণাঙ্ক n = 0; n < ninsn; n++)
                {
                    ধ্রুৱ Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            ঘূৰাই;
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
    পূর্ণাঙ্ক rchannels;
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
    তার্কিক inplace = মিছা;
    বাবে (পূর্ণাঙ্ক i = 0; i < ninputs && !inplace; i++)
        বাবে (পূর্ণাঙ্ক j = 0; j < noutputs; j++)
            যদি (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = সঁচা; ভঙ্গ; }
    পূর্ণাঙ্ক nhdrs = inplace ? ninputs : 0;

    AutoBuffer<ধ্রুৱ Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<ধ্রুৱ Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<পূর্ণাঙ্ক, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed ৱর্ণ, LOCAL_OPS> slotKind(nslots);
    পূর্ণাঙ্ক narr = 0, nc = 0;
    যদি (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        বাবে (পূর্ণাঙ্ক j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed ৱর্ণ)TExpr::NONE;
    বাবে (পূর্ণাঙ্ক s = 1; s < nslots; s++)
    {
        ধ্রুৱ TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed ৱর্ণ)ai.kind;
        যদি (ai.kind == TExpr::প্ৰবেশ) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        অন্যথা যদি (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        অন্যথা যদি (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed ৱর্ণ)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        অন্যথা যদি (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ধ্রুৱ পূর্ণাঙ্ক c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (শূন্য*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed ৱর্ণ)TExpr::প্ৰবেশ;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        অন্যথা যদি (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
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
    ধ্রুৱ পূর্ণাঙ্ক ninsn = (পূর্ণাঙ্ক)prog.size();
    long long costPerElem = 0;
    বাবে (পূর্ণাঙ্ক n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    বাবে (পূর্ণাঙ্ক d = 0; d < (পূর্ণাঙ্ক)spatial.size(); d++) otot *= spatial[d];
    যদি (otot == 0)
        ঘূৰাই;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ধ্রুৱ দ্ৱিগুণ T = (দ্ৱিগুণ)std::max(get_অঙ্ক_Threads(), 1);
    ধ্রুৱ দ্ৱিগুণ nstripes = std::min(
        (দ্ৱিগুণ)otot * (দ্ৱিগুণ)std::max<long long>(costPerElem, 1) * (1./ (দ্ৱিগুণ)(1 << 16)),
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

    broadcastOp(arr.data(), narr, [&](ধ্রুৱ BrTile& tile)
    {
        EwBody& bc = body;
        ধ্রুৱ TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ধ্রুৱ পূর্ণাঙ্ক* ধ্রুৱ smap  = bc.slotMap;
        ধ্রুৱ signed ৱর্ণ* ধ্রুৱ skind = bc.slotKind;
        ধ্রুৱ পূর্ণাঙ্ক nin = bc.ninsn, nsl = bc.nslots;
        ধ্রুৱ পূর্ণাঙ্ক w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ধ্রুৱ পূর্ণাঙ্ক* ধ্রুৱ eszPrefix = bc.bufEszPrefix;
        ধ্রুৱ পূর্ণাঙ্ক bw = std::min(w, bc.capElems);
        ধ্রুৱ পূর্ণাঙ্ক bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ধ্রুৱ size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/পরিমাণ(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)পরিমাণ(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        বাবে (পূর্ণাঙ্ক y0 = 0; y0 < h; y0 += bh)
        {
            ধ্রুৱ পূর্ণাঙ্ক hf = std::min(bh, h - y0);
            বাবে (পূর্ণাঙ্ক x0 = 0; x0 < w; x0 += bw)
            {
                ধ্রুৱ পূর্ণাঙ্ক wf = std::min(bw, w - x0);
                বাবে (পূর্ণাঙ্ক s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    ধ্রুৱ পূর্ণাঙ্ক k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    যদি (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    অন্যথা                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ধ্রুৱ BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                বাবে (পূর্ণাঙ্ক n = 0; n < nin; n++)
                {
                    ধ্রুৱ TExpr::Insn& ins = insns[n];
                    ধ্রুৱ BrSlice& a0 = args[ins.arg0]; ধ্রুৱ BrSlice& a1 = args[ins.arg1];
                    ধ্রুৱ BrSlice& a2 = args[ins.arg2]; ধ্রুৱ BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (শূন্য*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, সঁচা, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
শূন্য TExpr::exec(ধ্রুৱ Mat* inputs, Mat* outputs)
{
    AutoBuffer<ধ্রুৱ Mat*, 16> ptrs(std::max(ninputs, 1));
    বাবে (পূর্ণাঙ্ক i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
শূন্য make_দ্বিমিক_Arith_Program(TExpr& p, শীৰ্ষ op, পূর্ণাঙ্ক depth0, পূর্ণাঙ্ক depth1, পূর্ণাঙ্ক rdepth,
                            পূর্ণাঙ্ক maskDepth, দ্ৱিগুণ scale)
{
    p.clear();
    যদি (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        যদি (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        অন্যথা
        {
            rdepth = promoteArith(depth0, depth1);
            যদি (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ধ্রুৱ তার্কিক masked = maskDepth != EW_DEPTH_NONE;
    পূর্ণাঙ্ক sIn0  = p.add_প্ৰবেশ(depth0);
    পূর্ণাঙ্ক sIn1  = p.add_প্ৰবেশ(depth1);
    পূর্ণাঙ্ক sMask = masked ? p.add_প্ৰবেশ(maskDepth) : 0;
    পূর্ণাঙ্ক sOut  = p.addOutput(rdepth);

    পূর্ণাঙ্ক r = p.emit_দ্বিমিক(op, sIn0, sIn1, rdepth, Scalar(scale));
    যদি (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    অন্যথা
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
শূন্য makeAddWeightedProgram(TExpr& p, পূর্ণাঙ্ক depth0, পূর্ণাঙ্ক depth1, পূর্ণাঙ্ক rdepth,
                            দ্ৱিগুণ alpha, দ্ৱিগুণ beta, দ্ৱিগুণ gamma)
{
    p.clear();
    পূর্ণাঙ্ক sA   = p.add_প্ৰবেশ(depth0);
    পূর্ণাঙ্ক sB   = p.add_প্ৰবেশ(depth1);
    পূর্ণাঙ্ক sOut = p.addOutput(rdepth);
    পূর্ণাঙ্ক r = p.emit_দ্বিমিক(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

নামাকাশ {

// --- tokens ------------------------------------------------------------------------------
গণনা TokType { T_অঙ্ক, T_প্ৰবেশ, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

সংরচনা Token
{
    TokType type = T_END;
    দ্ৱিগুণ num = 0;
    পূর্ণাঙ্ক প্ৰবেশ_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
সংরচনা Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    স্থির তার্কিক isIdentStart(ৱর্ণ c) { ঘূৰাই std::isalpha((unsigned ৱর্ণ)c) || c == '_'; }
    স্থির তার্কিক isIdentChar(ৱর্ণ c)  { ঘূৰাই std::isalnum((unsigned ৱর্ণ)c) || c == '_'; }

    Token next()
    {
        যেতিয়ালৈ (pos < s.size() && std::isspace((unsigned ৱর্ণ)s[pos])) pos++;
        Token t;
        যদি (pos >= s.size()) { t.type = T_END; ঘূৰাই t; }

        ৱর্ণ c = s[pos];

        যদি (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            যেতিয়ালৈ (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_প্ৰবেশ;
            t.প্ৰবেশ_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            ঘূৰাই t;
        }
        যদি (std::isdigit((unsigned ৱর্ণ)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned ৱর্ণ)s[pos + 1])))
        {
            ৱর্ণ* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_অঙ্ক;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            ঘূৰাই t;
        }
        যদি (isIdentStart(c))
        {
            size_t start = pos;
            যেতিয়ালৈ (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            ঘূৰাই t;
        }
        ৱিকল্প (c)
        {
        পক্ষ '(': pos++; t.type = T_LPAREN; ঘূৰাই t;
        পক্ষ ')': pos++; t.type = T_RPAREN; ঘূৰাই t;
        পক্ষ ',': pos++; t.type = T_COMMA;  ঘূৰাই t;
        পক্ষ ';': pos++; t.type = T_SEMI;   ঘূৰাই t;
        }
        // multi/!single-char operators
        auto two = [&](ধ্রুৱ ৱর্ণ* op) {
            ঘূৰাই pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        যদি (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; ঘূৰাই t; }
        যদি (c == '=') { pos++; t.type = T_ASSIGN; ঘূৰাই t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        ঘূৰাই t;
    }
};

// --- operator / function tables ----------------------------------------------------------
স্থির পূর্ণাঙ্ক binPrec(ধ্রুৱ std::string& op)
{
    যদি (op == "**") ঘূৰাই 8;              // power binds tighter than '*'; RIGHT-associative
    যদি (op == "*" || op == "/") ঘূৰাই 7;
    যদি (op == "+" || op == "-") ঘূৰাই 6;
    যদি (op == "<" || op == "<=" || op == ">" || op == ">=") ঘূৰাই 5;
    যদি (op == "==" || op == "!=") ঘূৰাই 4;
    যদি (op == "&") ঘূৰাই 3;
    যদি (op == "^") ঘূৰাই 2;
    যদি (op == "|") ঘূৰাই 1;
    ঘূৰাই -1;
}

স্থির শীৰ্ষ binOp(ধ্রুৱ std::string& op)
{
    যদি (op == "**") ঘূৰাই OP_POW;
    যদি (op == "+")  ঘূৰাই OP_ADD;
    যদি (op == "-")  ঘূৰাই OP_SUB;
    যদি (op == "*")  ঘূৰাই OP_MUL;
    যদি (op == "/")  ঘূৰাই OP_DIV;
    যদি (op == "<")  ঘূৰাই OP_CMP_LT;
    যদি (op == "<=") ঘূৰাই OP_CMP_LE;
    যদি (op == ">")  ঘূৰাই OP_CMP_GT;
    যদি (op == ">=") ঘূৰাই OP_CMP_GE;
    যদি (op == "==") ঘূৰাই OP_CMP_EQ;
    যদি (op == "!=") ঘূৰাই OP_CMP_NE;
    যদি (op == "&")  ঘূৰাই OP_AND;
    যদি (op == "|")  ঘূৰাই OP_OR;
    যদি (op == "^")  ঘূৰাই OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
স্থির পূর্ণাঙ্ক typeDepth(ধ্রুৱ std::string& name)
{
    যদি (name == "float")    ঘূৰাই CV_32F;
    যদি (name == "double")   ঘূৰাই CV_64F;
    যদি (name == "half" || name == "float16") ঘূৰাই CV_16F;
    যদি (name == "bfloat16") ঘূৰাই CV_16BF;
    যদি (name == "uint8")    ঘূৰাই CV_8U;
    যদি (name == "int8")     ঘূৰাই CV_8S;
    যদি (name == "uint16")   ঘূৰাই CV_16U;
    যদি (name == "int16")    ঘূৰাই CV_16S;
    যদি (name == "uint32")   ঘূৰাই CV_32U;
    যদি (name == "int32")    ঘূৰাই CV_32S;
    যদি (name == "uint64")   ঘূৰাই CV_64U;
    যদি (name == "int64")    ঘূৰাই CV_64S;
    ঘূৰাই -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
স্থির শীৰ্ষ fnOp(ধ্রুৱ std::string& name, পূর্ণাঙ্ক& arity)
{
    সংরচনা E { ধ্রুৱ ৱর্ণ* n; শীৰ্ষ op; };
    স্থির ধ্রুৱ E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    স্থির ধ্রুৱ E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    স্থির ধ্রুৱ E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    বাবে (ধ্রুৱ E& e : unary)  যদি (name == e.n) { arity = 1; ঘূৰাই e.op; }
    বাবে (ধ্রুৱ E& e : binary) যদি (name == e.n) { arity = 2; ঘূৰাই e.op; }
    বাবে (ধ্রুৱ E& e : tern)   যদি (name == e.n) { arity = 3; ঘূৰাই e.op; }
    arity = 0; ঘূৰাই OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
সংরচনা Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ধ্রুৱ পূর্ণাঙ্ক* প্ৰবেশ_Slot;                 // slot id of each external input (precreated)
    পূর্ণাঙ্ক ninputs;
    std::map<std::string, পূর্ণাঙ্ক> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& ৰাশিমালা, ধ্রুৱ পূর্ণাঙ্ক* islot, পূর্ণাঙ্ক nin)
        : lex(src), e(ৰাশিমালা), প্ৰবেশ_Slot(islot), ninputs(nin) { cur = lex.next(); }

    শূন্য advance() { cur = lex.next(); }
    তার্কিক isOp(ধ্রুৱ ৱর্ণ* op) ধ্রুৱ { ঘূৰাই cur.type == T_OP && cur.text == op; }

    শূন্য expect(TokType t, ধ্রুৱ ৱর্ণ* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    তার্কিক isFlexConst(পূর্ণাঙ্ক slot) ধ্রুৱ
    {
        ঘূৰাই e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    পূর্ণাঙ্ক parsePrimary()
    {
        যদি (cur.type == T_অঙ্ক)   { পূর্ণাঙ্ক s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); ঘূৰাই s; }
        যদি (cur.type == T_প্ৰবেশ) { পূর্ণাঙ্ক idx = cur.প্ৰবেশ_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   ঘূৰাই প্ৰবেশ_Slot[idx]; }
        যদি (cur.type == T_LPAREN){ advance(); পূর্ণাঙ্ক x = parseTernary(); expect(T_RPAREN, "expected ')'"); ঘূৰাই x; }
        যদি (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            যদি (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                ঘূৰাই it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<পূর্ণাঙ্ক, 8> args = {}; পূর্ণাঙ্ক nargs = 0;
            যদি (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                যেতিয়ালৈ (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (পূর্ণাঙ্ক)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            পূর্ণাঙ্ক td = typeDepth(name);
            যদি (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                পূর্ণাঙ্ক s = args[0];
                ঘূৰাই isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            পূর্ণাঙ্ক arity = 0; শীৰ্ষ op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            যদি (arity == 1) ঘূৰাই e.emitUnary(op, args[0]);
            যদি (arity == 2) ঘূৰাই e.emit_দ্বিমিক(op, args[0], args[1]);
            ঘূৰাই e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    পূর্ণাঙ্ক parseUnary()
    {
        যদি (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            পূর্ণাঙ্ক operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            যদি (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<দ্ৱিগুণ, MAX_LOCAL_CN> v; পূর্ণাঙ্ক cn = constDoubles(e, operand, v);
                Scalar neg; বাবে (পূর্ণাঙ্ক i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                ঘূৰাই e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            ঘূৰাই e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        ঘূৰাই parsePrimary();
    }

    পূর্ণাঙ্ক parse_ৰাশিমালা(পূর্ণাঙ্ক minPrec)
    {
        পূর্ণাঙ্ক বাম = parseUnary();
        যেতিয়ালৈ (cur.type == T_OP)
        {
            পূর্ণাঙ্ক p = binPrec(cur.text);
            যদি (p < minPrec) ভঙ্গ;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            পূর্ণাঙ্ক ডান = parse_ৰাশিমালা(op == "**" ? p : p + 1);
            বাম = e.emit_দ্বিমিক(binOp(op), বাম, ডান);
        }
        ঘূৰাই বাম;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    পূর্ণাঙ্ক parseTernary()
    {
        পূর্ণাঙ্ক cond = parse_ৰাশিমালা(0);
        যদি (!isOp("?")) ঘূৰাই cond;
        advance();
        পূর্ণাঙ্ক thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        পূর্ণাঙ্ক elsev = parseTernary();               // right-associative chain
        ঘূৰাই e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    শূন্য parseResult()
    {
        যদি (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); পূর্ণাঙ্ক nTemp = e.ntemps;
            advance();
            পূর্ণাঙ্ক e0 = parseTernary();
            যদি (cur.type == T_COMMA)
            {
                e.output(e0);
                যেতিয়ালৈ (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                ঘূৰাই;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    শূন্য parse()
    {
        যেতিয়ালৈ (সঁচা)
        {
            যদি (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                যদি (cur.type == T_ASSIGN)
                {
                    advance();
                    ধ্রুৱ পূর্ণাঙ্ক slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = সঁচা;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    অনুৱর্ত;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            যদি (cur.type == T_SEMI) advance();
            ভঙ্গ;
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
শূন্য texpr(ধ্রুৱ std::string& ৰাশিমালা, প্ৰবেশ_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using নামাকাশ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == প্ৰবেশ_Array_2::STD_VECTOR_MAT);
    ধ্রুৱ std::vector<Mat>& inps = *(ধ্রুৱ std::vector<Mat>*)_inputs.getObj();
    ধ্রুৱ পূর্ণাঙ্ক ninputs = (পূর্ণাঙ্ক)inps.size();
    বাবে (পূর্ণাঙ্ক i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<পূর্ণাঙ্ক> islot(std::max(ninputs, 1));
    বাবে (পূর্ণাঙ্ক i = 0; i < ninputs; i++) islot[i] = e.add_প্ৰবেশ(inps[i].depth());
    Parser(ৰাশিমালা, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    যদি (kind == প্ৰবেশ_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } অন্যথা {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
