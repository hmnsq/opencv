// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "arithm_expr.hpp"
#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <climits>
#ଅନ୍ତର୍ଭାଵ <cmath>
#ଅନ୍ତର୍ଭାଵ <cstdlib>
#ଅନ୍ତର୍ଭାଵ <cstring>
#ଅନ୍ତର୍ଭାଵ <map>
#ଅନ୍ତର୍ଭାଵ <ostream>
#ଅନ୍ତର୍ଭାଵ <string>

ନାମାକାଶ cv { ନାମାକାଶ ew {

// ============================ op metadata (was ew_op.cpp) ============================

ଅଚଳ ଅକ୍ଷର* opName(ଶୀର୍ଷ op)
{
    ବାଛ (op)
    {
    ସ୍ଥିତି OP_NOP:           ଫେରନ୍ତୁ "nop";
    ସ୍ଥିତି OP_NEG:           ଫେରନ୍ତୁ "neg";
    ସ୍ଥିତି OP_ABS:           ଫେରନ୍ତୁ "abs";
    ସ୍ଥିତି OP_NOT:           ଫେରନ୍ତୁ "not";
    ସ୍ଥିତି OP_SQRT:          ଫେରନ୍ତୁ "sqrt";
    ସ୍ଥିତି OP_EXP:           ଫେରନ୍ତୁ "exp";
    ସ୍ଥିତି OP_LOG:           ଫେରନ୍ତୁ "log";
    ସ୍ଥିତି OP_SIN:           ଫେରନ୍ତୁ "sin";
    ସ୍ଥିତି OP_COS:           ଫେରନ୍ତୁ "cos";
    ସ୍ଥିତି OP_TANH:          ଫେରନ୍ତୁ "tanh";
    ସ୍ଥିତି OP_ERF:           ଫେରନ୍ତୁ "erf";
    ସ୍ଥିତି OP_RELU:          ଫେରନ୍ତୁ "relu";
    ସ୍ଥିତି OP_CAST:          ଫେରନ୍ତୁ "cast";
    ସ୍ଥିତି OP_ADD:           ଫେରନ୍ତୁ "add";
    ସ୍ଥିତି OP_SUB:           ଫେରନ୍ତୁ "sub";
    ସ୍ଥିତି OP_MUL:           ଫେରନ୍ତୁ "mul";
    ସ୍ଥିତି OP_DIV:           ଫେରନ୍ତୁ "div";
    ସ୍ଥିତି OP_POW:           ଫେରନ୍ତୁ "pow";
    ସ୍ଥିତି OP_MIN:           ଫେରନ୍ତୁ "min";
    ସ୍ଥିତି OP_MAX:           ଫେରନ୍ତୁ "max";
    ସ୍ଥିତି OP_ABSDIFF:       ଫେରନ୍ତୁ "absdiff";
    ସ୍ଥିତି OP_ADDW:          ଫେରନ୍ତୁ "addWeighted";
    ସ୍ଥିତି OP_HYPOT:         ଫେରନ୍ତୁ "hypot";
    ସ୍ଥିତି OP_ATAN2:         ଫେରନ୍ତୁ "atan2";
    ସ୍ଥିତି OP_AND:           ଫେରନ୍ତୁ "and";
    ସ୍ଥିତି OP_OR:            ଫେରନ୍ତୁ "or";
    ସ୍ଥିତି OP_XOR:           ଫେରନ୍ତୁ "xor";
    ସ୍ଥିତି OP_CMP_EQ:        ଫେରନ୍ତୁ "cmp_eq";
    ସ୍ଥିତି OP_CMP_NE:        ଫେରନ୍ତୁ "cmp_ne";
    ସ୍ଥିତି OP_CMP_LT:        ଫେରନ୍ତୁ "cmp_lt";
    ସ୍ଥିତି OP_CMP_LE:        ଫେରନ୍ତୁ "cmp_le";
    ସ୍ଥିତି OP_CMP_GT:        ଫେରନ୍ତୁ "cmp_gt";
    ସ୍ଥିତି OP_CMP_GE:        ଫେରନ୍ତୁ "cmp_ge";
    ସ୍ଥିତି OP_CLAMP:         ଫେରନ୍ତୁ "clamp";
    ସ୍ଥିତି OP_SELECT:        ଫେରନ୍ତୁ "select";
    ସ୍ଥିତି OP_CONVERT_SCALE: ଫେରନ୍ତୁ "convert_scale";
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:               ଫେରନ୍ତୁ "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(ଶୀର୍ଷ op)
{
    ବାଛ (op)
    {
    ସ୍ଥିତି OP_AND: ସ୍ଥିତି OP_OR: ସ୍ଥିତି OP_XOR: ସ୍ଥିତି OP_NOT:
        ଫେରନ୍ତୁ CAT_BITWISE;
    ସ୍ଥିତି OP_CMP_EQ: ସ୍ଥିତି OP_CMP_NE: ସ୍ଥିତି OP_CMP_LT:
    ସ୍ଥିତି OP_CMP_LE: ସ୍ଥିତି OP_CMP_GT: ସ୍ଥିତି OP_CMP_GE:
        ଫେରନ୍ତୁ CAT_COMPARE;
    ସ୍ଥିତି OP_SQRT: ସ୍ଥିତି OP_EXP: ସ୍ଥିତି OP_LOG:
    ସ୍ଥିତି OP_SIN: ସ୍ଥିତି OP_COS: ସ୍ଥିତି OP_TANH: ସ୍ଥିତି OP_ERF: ସ୍ଥିତି OP_ATAN2: ସ୍ଥିତି OP_RELU:
        ଫେରନ୍ତୁ CAT_MATH;
    ସ୍ଥିତି OP_CAST: ସ୍ଥିତି OP_CONVERT_SCALE:
        ଫେରନ୍ତୁ CAT_CAST;
    ସ୍ଥିତି OP_SELECT:
        ଫେରନ୍ତୁ CAT_SELECT;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
ସ୍ଥିର ତାର୍କିକ isFloatDepth(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d)
{
    ଫେରନ୍ତୁ d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
ସ୍ଥିର ତାର୍କିକ depthRepresents(ଦ୍ୱିଗୁଣ v, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଯଦି (isFloatDepth(depth)) ଫେରନ୍ତୁ ସତ୍ୟ;
    ଯଦି (v != std::floor(v)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ବାଛ (depth)
    {
    ସ୍ଥିତି CV_8U:  ଫେରନ୍ତୁ v >= 0            && v <= 255;
    ସ୍ଥିତି CV_8S:  ଫେରନ୍ତୁ v >= -128         && v <= 127;
    ସ୍ଥିତି CV_16U: ଫେରନ୍ତୁ v >= 0            && v <= 65535;
    ସ୍ଥିତି CV_16S: ଫେରନ୍ତୁ v >= -32768       && v <= 32767;
    ସ୍ଥିତି CV_32U: ଫେରନ୍ତୁ v >= 0            && v <= 4294967295.0;
    ସ୍ଥିତି CV_32S: ଫେରନ୍ତୁ v >= -2147483648.0 && v <= 2147483647.0;
    ସ୍ଥିତି CV_64U: ଫେରନ୍ତୁ v >= 0            && v <= 18446744073709551615.0;
    ସ୍ଥିତି CV_64S: ଫେରନ୍ତୁ v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:     ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
ସ୍ଥିର ଶୂନ୍ୟ intRange(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ଦ୍ୱିଗୁଣ& lo, ଦ୍ୱିଗୁଣ& hi)
{
    ବାଛ (depth)
    {
    ସ୍ଥିତି CV_8U:  lo = 0;                     hi = 255; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_8S:  lo = -128;                  hi = 127; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16U: lo = 0;                     hi = 65535; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16S: lo = -32768;                hi = 32767; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_32U: lo = 0;                     hi = 4294967295.0; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_32S: lo = -2147483648.0;         hi = 2147483647.0; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_64U: lo = 0;                     hi = 18446744073709551615.0; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:     lo = 0;                     hi = 0; ଭାଙ୍ଗ;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା promoteArith(ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    ଯଦି (a == b) ଫେରନ୍ତୁ a;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wa = ପୂର୍ଣ୍ଣସଂଖ୍ୟା((leszlut >> (a+1)*3) & 7u), wb = ପୂର୍ଣ୍ଣସଂଖ୍ୟା((leszlut >> (b+1)*3) & 7u);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fa = ପୂର୍ଣ୍ଣସଂଖ୍ୟା((fltlut >> (a+1)) & 1u),    fb = ପୂର୍ଣ୍ଣସଂଖ୍ୟା((fltlut >> (b+1)) & 1u);

    ଯଦି (fa + fb == 0)                          // both integer
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sa = ପୂର୍ଣ୍ଣସଂଖ୍ୟା((signlut >> (a+1)) & 1u), sb = ପୂର୍ଣ୍ଣସଂଖ୍ୟା((signlut >> (b+1)) & 1u);
        ଯଦି (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            ଫେରନ୍ତୁ wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        ଫେରନ୍ତୁ (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxw = std::max(wa, wb);
    ଯଦି (maxw == 2 && fa + fb == 1)
        ଫେରନ୍ତୁ fa*a + (1 - fa)*b;
    ଫେରନ୍ତୁ CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା absdiffResultDepth(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ବାଛ (depth)
    {
    ସ୍ଥିତି CV_8S:  ଫେରନ୍ତୁ CV_8U;
    ସ୍ଥିତି CV_16S: ଫେରନ୍ତୁ CV_16U;
    ସ୍ଥିତି CV_32S: ଫେରନ୍ତୁ CV_32U;
    ସ୍ଥିତି CV_64S: ଫେରନ୍ତୁ CV_64U;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:     ଫେରନ୍ତୁ depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା promote2(ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b)
{
    ଯଦି (a == b) ଫେରନ୍ତୁ a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ଅଚଳ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ଅଚଳ unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ଫେରନ୍ତୁ ପୂର୍ଣ୍ଣସଂଖ୍ୟା((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା promoteBitwise(ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b)
{
    ଯଦି (a == EW_DEPTH_NONE) ଫେରନ୍ତୁ b;
    ଯଦି (b == EW_DEPTH_NONE) ଫେରନ୍ତୁ a;
    ଯଦି (a == b) ଫେରନ୍ତୁ a;
    ଫେରନ୍ତୁ CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା safeWide(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ଅଚଳ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ଅଚଳ unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ଫେରନ୍ତୁ ପୂର୍ଣ୍ଣସଂଖ୍ୟା((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
ଗଣନା { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
ସ୍ଥିର size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcdepth, ଅଚଳ ଶୂନ୍ୟ* data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = std::max(1, channels);
    ଅଚଳ size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ଅଚଳ size_t nu64 = (nbytes + ଆକାର(uint64_t) - 1) / ଆକାର(uint64_t);
    ଅଚଳ size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    ଯଦି (data) memcpy(dst, data, nbytes); ଅନ୍ୟଥା memset(dst, 0, nbytes);
    ଫେରନ୍ତୁ ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା constDoubles(ଅଚଳ TExpr& e, ପୂର୍ଣ୍ଣସଂଖ୍ୟା s, AutoBuffer<ଦ୍ୱିଗୁଣ, MAX_LOCAL_CN>& buf)
{
    ଅଚଳ TExpr::Arg& a = e.arginfo[s];
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = std::max(1, a.channels);
    buf.resize(cn);
    ଅଚଳ uchar* src = (ଅଚଳ uchar*)(e.constbuf.data() + a.constofs);
    ଯଦି (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * ଆକାର(ଦ୍ୱିଗୁଣ));   // common (Scalar / parsed literal)
    ଅନ୍ୟଥା
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    ଫେରନ୍ତୁ cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isFlexConst(ଅଚଳ TExpr& e, ପୂର୍ଣ୍ଣସଂଖ୍ୟା s)
{
    ଫେରନ୍ତୁ e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
ସ୍ଥିର ତାର୍କିକ scalar_Const_ମୂଲ୍ୟ(ଅଚଳ TExpr& e, ପୂର୍ଣ୍ଣସଂଖ୍ୟା s, ଦ୍ୱିଗୁଣ& v)
{
    ଅଚଳ TExpr::Arg& a = e.arginfo[s];
    ଯଦି (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    AutoBuffer<ଦ୍ୱିଗୁଣ, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    ଫେରନ୍ତୁ ସତ୍ୟ;
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
ସ୍ଥିର ତାର୍କିକ matchScaledChain(ଅଚଳ TExpr& e, ପୂର୍ଣ୍ଣସଂଖ୍ୟା end, ପୂର୍ଣ୍ଣସଂଖ୍ୟା t,
                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା& arrSlot, ଦ୍ୱିଗୁଣ& scale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& startIdx)
{
    ଯଦି (end < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = end, mulResult = t;

    // optional trailing narrowing cast
    ଯଦି (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        ଯଦି (--j < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଅଚଳ TExpr::Insn& mul = e.prog[j];
    ଯଦି (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା arr;
    ଯଦି      (scalar_Const_ମୂଲ୍ୟ(e, mul.arg1, scale)) arr = mul.arg0;
    ଅନ୍ୟଥା ଯଦି (scalar_Const_ମୂଲ୍ୟ(e, mul.arg0, scale)) arr = mul.arg1;
    ଅନ୍ୟଥା ଫେରନ୍ତୁ ମିଥ୍ୟା;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    ଯଦି (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    ଯଦି (e.arginfo[arr].kind == TExpr::CONST) ଫେରନ୍ତୁ ମିଥ୍ୟା;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
ସ୍ଥିର ତାର୍କିକ constFits(ଅଚଳ TExpr& e, ପୂର୍ଣ୍ଣସଂଖ୍ୟା s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା d)
{
    ଯଦି (!isFlexConst(e, s)) ଫେରନ୍ତୁ ସତ୍ୟ;
    AutoBuffer<ଦ୍ୱିଗୁଣ, MAX_LOCAL_CN> v;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = constDoubles(e, s, v);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ch = 0; ch < cn; ch++)
        ଯଦି (!depthRepresents(v[ch], d)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::emit_ଦ୍ୱିମିକ(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdepth, ଅଚଳ Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    ଯଦି (op == OP_ADDW)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା Tt = arginfo[a].depth;
        ଯଦି (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        ଯଦି (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        ଯଦି (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା outD = rdepth;
        ଯଦି (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        ଯଦି (outD == rdepth) ଫେରନ୍ତୁ out;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        ଫେରନ୍ତୁ out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    ଯଦି (op == OP_ADD)
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0, y = 0, startA = 0, startB = 0;
        ଦ୍ୱିଗୁଣ alpha = 0, beta = 0;
        ଯଦି (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*ଏହି, n - 1, b, y, beta, startB) &&
            matchScaledChain(*ଏହି, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା retired = 0, lowest = INT_MAX;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = startA; i < n; i++)
            {
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = prog[i].result;
                ଯଦି (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; ଭାଙ୍ଗ; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            ଯଦି (retired > 0 && lowest == ntemps - retired)
            {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                ଫେରନ୍ତୁ emit_ଦ୍ୱିମିକ(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା acc = 0; ଦ୍ୱିଗୁଣ gamma = 0;
            ଯଦି      (scalar_Const_ମୂଲ୍ୟ(*ଏହି, b, gamma)) acc = a;
            ଅନ୍ୟଥା ଯଦି (scalar_Const_ମୂଲ୍ୟ(*ଏହି, a, gamma)) acc = b;
            ଯଦି (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                ଫେରନ୍ତୁ acc;
            }
        }
    }

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd0 = isFlexConst(*ଏହି, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd1 = isFlexConst(*ଏହି, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    ଅଚଳ ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = rdepth;
    ଯଦି (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        ଯଦି (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା base;
    ଯଦି (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    ଅନ୍ୟଥା ଯଦି (nd0 == EW_DEPTH_NONE) base = nd1;
    ଅନ୍ୟଥା ଯଦି (nd1 == EW_DEPTH_NONE) base = nd0;
    ଅନ୍ୟଥା base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    ଯଦି (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*ଏହି, b) && !constFits(*ଏହି, b, base))
    {
        ଦ୍ୱିଗୁଣ lo, hi; intRange(nd0, lo, hi);
        ଯଦି (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            ଗଣନା { REAL, CFALSE, CTRUE };
            AutoBuffer<ଦ୍ୱିଗୁଣ, MAX_LOCAL_CN> tv; ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = constDoubles(*ଏହି, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind[4] = {}; ଦ୍ୱିଗୁଣ bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ଅଚଳ ଶୀର୍ଷ fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0; c < cn; c++)
            {
                ଅଚଳ ଦ୍ୱିଗୁଣ t = tv[c]; ପୂର୍ଣ୍ଣସଂଖ୍ୟା k; ଦ୍ୱିଗୁଣ B = t;
                ବାଛ (op)
                {
                ସ୍ଥିତି OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ଭାଙ୍ଗ;
                ସ୍ଥିତି OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ଭାଙ୍ଗ;
                ସ୍ଥିତି OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ଭାଙ୍ଗ;
                ସ୍ଥିତି OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ଭାଙ୍ଗ;
                ସ୍ଥିତି OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; ଭାଙ୍ଗ;   // a==non-rep -> false
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  ଭାଙ୍ଗ;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା c){ ଫେରନ୍ତୁ kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା c){ ଫେରନ୍ତୁ kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ଅଚଳ ଶୀର୍ଷ u0 = ucOp(0);
            ତାର୍କିକ uniform = ସତ୍ୟ;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            ଯଦି (uniform)
            {
                Scalar s; ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            ଅନ୍ୟଥା
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; ପୂର୍ଣ୍ଣସଂଖ୍ୟା patchFlags = 0;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                ଫେରନ୍ତୁ out;
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, wide;
    ତାର୍କିକ divGuard = ମିଥ୍ୟା;
    ବାଛ (op)
    {
    ସ୍ଥିତି OP_MIN: ସ୍ଥିତି OP_MAX:
    ସ୍ଥିତି OP_AND: ସ୍ଥିତି OP_OR: ସ୍ଥିତି OP_XOR:
        depth = result; wide = result; ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        ଯଦି (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        ଯଦି (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        ଭାଙ୍ଗ;
    }
    ସ୍ଥିତି OP_CMP_EQ: ସ୍ଥିତି OP_CMP_NE: ସ୍ଥିତି OP_CMP_LT:
    ସ୍ଥିତି OP_CMP_LE: ସ୍ଥିତି OP_CMP_GT: ସ୍ଥିତି OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_MUL: ସ୍ଥିତି OP_DIV:
    {
        depth = base;
        ଅଚଳ ତାର୍କିକ w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        ଯଦି (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd) {
                ଯଦି (nd != EW_DEPTH_NONE) ଫେରନ୍ତୁ !isFloatDepth(nd);
                AutoBuffer<ଦ୍ୱିଗୁଣ, MAX_LOCAL_CN> v; constDoubles(*ଏହି, s, v);
                ଫେରନ୍ତୁ v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        ଭାଙ୍ଗ;
    }
    ସ୍ଥିତି OP_POW: ସ୍ଥିତି OP_HYPOT: ସ୍ଥିତି OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); ଭାଙ୍ଗ;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା s) {
        ଯଦି (!isFlexConst(*ଏହି, s)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        AutoBuffer<ଦ୍ୱିଗୁଣ, MAX_LOCAL_CN> v; ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = constDoubles(*ଏହି, s, v);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ch = 0; ch < n; ch++)
            ଯଦି (v[ch] != std::floor(v[ch])) ଫେରନ୍ତୁ ସତ୍ୟ;
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    ଯଦି (cat != CAT_BITWISE && (!constFits(*ଏହି, a, depth) || !constFits(*ଏହି, b, depth)))
    {
        depth = wide;
        ଅଚଳ ତାର୍କିକ frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        ଯଦି (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        ଯଦି (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା s0 = isFlexConst(*ଏହି, a) ? typedConstFrom(a, depth) : a;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା s1 = isFlexConst(*ଏହି, b) ? typedConstFrom(b, depth) : b;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା rd) {
        ଫେରନ୍ତୁ op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    ଯଦି (k.fptr)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        ଫେରନ୍ତୁ res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    ଫେରନ୍ତୁ maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::emitUnary(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdepth, ଅଚଳ Scalar& params)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd = isFlexConst(*ଏହି, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ଅଚଳ ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    ଯଦି (op == OP_NEG)
        ଫେରନ୍ତୁ emit_ଦ୍ୱିମିକ(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    ଯଦି (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        ଯଦି (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            ଫେରନ୍ତୁ emit_ଦ୍ୱିମିକ(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        ଫେରନ୍ତୁ emit_ଦ୍ୱିମିକ(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = rdepth;
    ଯଦି (result == EW_DEPTH_NONE)
    {
        ବାଛ (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        ସ୍ଥିତି CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; ଭାଙ୍ଗ;
        ସ୍ଥିତି CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; ଭାଙ୍ଗ;  // NOT
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; ଭାଙ୍ଗ;  // NEG, ABS
        }
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, wide;
    ବାଛ (cat)
    {
    ସ୍ଥିତି CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CAT_BITWISE: depth = result; wide = result; ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; ଭାଙ୍ଗ;  // NEG/ABS
    }

    ଯଦି (!constFits(*ଏହି, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା s0 = isFlexConst(*ଏହି, a) ? typedConstFrom(a, depth) : a;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    ଯଦି (k.fptr)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        ଫେରନ୍ତୁ res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    ଫେରନ୍ତୁ maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::emitTernary(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdepth)
{
    auto dep = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା s) { ଫେରନ୍ତୁ isFlexConst(*ଏହି, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    ଯଦି (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        ଯଦି (isFlexConst(*ଏହି, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_ଦ୍ୱିମିକ(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        ଯଦି (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sb = isFlexConst(*ଏହି, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sc = isFlexConst(*ଏହି, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        ଫେରନ୍ତୁ res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    ଯଦି (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା s) {
        ଫେରନ୍ତୁ isFlexConst(*ଏହି, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    ଫେରନ୍ତୁ res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::moveToOutput(ପୂର୍ଣ୍ଣସଂଖ୍ୟା temp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ninsn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prog.size();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା producer = -1;
    ତାର୍କିକ usedAsArg = ମିଥ୍ୟା;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninsn; i++)
    {
        ଅଚଳ TExpr::Insn& ins = prog[i];
        ଯଦି (ins.result == temp) producer = i;
        ଯଦି (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = ସତ୍ୟ;
    }
    ଯଦି (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        ଫେରନ୍ତୁ out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    ଫେରନ୍ତୁ out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::output(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଳ_Slot)
{
    ଫେରନ୍ତୁ moveToOutput(ମୂଳ_Slot, addOutput(arginfo[ମୂଳ_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
ଶୂନ୍ୟ TExpr::compile()
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ninsn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nslots = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size();
    nconsts = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        ଯଦି (a.kind != CONST || a.depth == EW_DEPTH_NONE) ଜାରି;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        ଅଚଳ size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, ଆକାର(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)ଆକାର(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (ଅଚଳ uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        ଅଚଳ size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        ଯଦି (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        ଅନ୍ୟଥା getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
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
    ଯଦି (ntemps == 0) ଫେରନ୍ତୁ;

    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 32> tempOfSlot(nslots);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 1; s < nslots; s++)
        ଯଦି (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 16> lastUse(ntemps);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = 0; t < ntemps; t++) lastUse[t] = -1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninsn; i++)
    {
        ଅଚଳ TExpr::Insn& ins = prog[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < 3; k++)
            ଯଦି (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 16> freeBufs(ntemps);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfree = 0, nbuf = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninsn; i++)
    {
        ଅଚଳ TExpr::Insn& ins = prog[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt = tempOfSlot[ins.result];
        ଯଦି (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < 3; k++)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = tempOfSlot[as[k]];
            ଯଦି (t >= 0 && lastUse[t] == i)
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
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 1; s < nslots; s++)
        ଯଦି (arginfo[s].kind == TEMP)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = bufferOfTemp[arginfo[s].index], e = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_ELEM_SIZE1(arginfo[s].depth);
            ଯଦି (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
ସ୍ଥିର ଶୂନ୍ୟ matLogical(ଅଚଳ Mat& m, MatShape& shp, EwSteps& step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& esz1)
{
    esz1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.elemSize1();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
ସ୍ଥିର ତାର୍କିକ broadcastShape(ଅଚଳ MatShape* shps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, MatShape& out)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
    {
        ଅଚଳ MatShape& s = shps[k];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା off = nd - s.dims;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < s.dims; i++)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = s[i], &o = out[off + i];
            ଯଦି (o == 1) o = d;
            ଅନ୍ୟଥା ଯଦି (d != 1 && d != o) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opCost(ଶୀର୍ଷ op)
{
    ବାଛ (op)
    {
    ସ୍ଥିତି OP_ADD: ସ୍ଥିତି OP_SUB: ସ୍ଥିତି OP_MUL: ସ୍ଥିତି OP_MIN: ସ୍ଥିତି OP_MAX:
    ସ୍ଥିତି OP_ABSDIFF: ସ୍ଥିତି OP_AND: ସ୍ଥିତି OP_OR: ସ୍ଥିତି OP_XOR: ସ୍ଥିତି OP_NOT:
    ସ୍ଥିତି OP_NEG: ସ୍ଥିତି OP_ABS: ସ୍ଥିତି OP_CAST: ସ୍ଥିତି OP_RELU: ସ୍ଥିତି OP_SELECT:
    ସ୍ଥିତି OP_CMP_EQ: ସ୍ଥିତି OP_CMP_NE: ସ୍ଥିତି OP_CMP_LT:
    ସ୍ଥିତି OP_CMP_LE: ସ୍ଥିତି OP_CMP_GT: ସ୍ଥିତି OP_CMP_GE: ଫେରନ୍ତୁ 1;
    ସ୍ଥିତି OP_DIV: ସ୍ଥିତି OP_SQRT: ସ୍ଥିତି OP_HYPOT: ସ୍ଥିତି OP_CONVERT_SCALE: ଫେରନ୍ତୁ 3;
    ସ୍ଥିତି OP_SIN: ସ୍ଥିତି OP_COS: ସ୍ଥିତି OP_TANH: ସ୍ଥିତି OP_ERF: ସ୍ଥିତି OP_ATAN2:
    ସ୍ଥିତି OP_EXP: ସ୍ଥିତି OP_LOG: ସ୍ଥିତି OP_POW:           ଫେରନ୍ତୁ 6;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                                         ଫେରନ୍ତୁ 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
ଶୂନ୍ୟ TExpr::clear()
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
ଶୂନ୍ୟ TExpr::dump(std::ostream& os) ଅଚଳ
{
    auto dn = [](ପୂର୍ଣ୍ଣସଂଖ୍ୟା d) -> ଅଚଳ ଅକ୍ଷର* {
        ବାଛ (d) {
        ସ୍ଥିତି EW_DEPTH_NONE: ଫେରନ୍ତୁ "flex";
        ସ୍ଥିତି CV_8U:  ଫେରନ୍ତୁ "u8";  ସ୍ଥିତି CV_8S:  ଫେରନ୍ତୁ "s8";
        ସ୍ଥିତି CV_16U: ଫେରନ୍ତୁ "u16"; ସ୍ଥିତି CV_16S: ଫେରନ୍ତୁ "s16";
        ସ୍ଥିତି CV_32U: ଫେରନ୍ତୁ "u32"; ସ୍ଥିତି CV_32S: ଫେରନ୍ତୁ "s32";
        ସ୍ଥିତି CV_64U: ଫେରନ୍ତୁ "u64"; ସ୍ଥିତି CV_64S: ଫେରନ୍ତୁ "s64";
        ସ୍ଥିତି CV_16F: ଫେରନ୍ତୁ "f16"; ସ୍ଥିତି CV_16BF:ଫେରନ୍ତୁ "bf16";
        ସ୍ଥିତି CV_32F: ଫେରନ୍ତୁ "f32"; ସ୍ଥିତି CV_64F: ଫେରନ୍ତୁ "f64";
        ସ୍ଥିତି CV_Bool:ଫେରନ୍ତୁ "bool";
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:     ଫେରନ୍ତୁ "?"; }
    };
    auto kn = [](ArgKind k) -> ଅଚଳ ଅକ୍ଷର* {
        ବାଛ (k) { ସ୍ଥିତି NONE: ଫେରନ୍ତୁ "none"; ସ୍ଥିତି ନିବେଶ: ଫେରନ୍ତୁ "in"; ସ୍ଥିତି CONST: ଫେରନ୍ତୁ "const";
                     ସ୍ଥିତି TEMP: ଫେରନ୍ତୁ "temp"; ସ୍ଥିତି OUTPUT: ଫେରନ୍ତୁ "out"; } ଫେରନ୍ତୁ "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prog.size() << "\n";
    os << "  slots:\n";
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0; s < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size(); s++) {
        ଅଚଳ Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        ଯଦି (a.kind != NONE) os << " " << dn(a.depth);
        ଯଦି (a.kind == ନିବେଶ || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        ଯଦି (a.kind == CONST) {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = std::max(1, a.channels);
            AutoBuffer<ଦ୍ୱିଗୁଣ, MAX_LOCAL_CN> v(cn);
            ଅଚଳ uchar* src = (ଅଚଳ uchar*)(constbuf.data() + a.constofs);
            ଯଦି (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * ଆକାର(ଦ୍ୱିଗୁଣ));
            ଅନ୍ୟଥା getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prog.size(); i++) {
        ଅଚଳ Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        ଯଦି (ins.arg1) os << ", " << ins.arg1;
        ଯଦି (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        ଯଦି (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        ଯଦି (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        ଯଦି (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::add_ନିବେଶ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    Arg a; a.kind = ନିବେଶ; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size() - 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::addOutput(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size() - 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::addTemp(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::addConst(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ଅଚଳ Scalar& v, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::addConst(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcdepth, ଅଚଳ ଶୂନ୍ୟ* data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::typedConstFrom(ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcSlot, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଅଚଳ Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::addInsn(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା result, ଅଚଳ TKernel& kernel, ଅଚଳ Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::addInsn(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା result, ଅଚଳ Scalar& params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ଫେରନ୍ତୁ addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା TExpr::maybeAddCast(ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଯଦି (arginfo[arg].depth == depth) ଫେରନ୍ତୁ arg;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    ଫେରନ୍ତୁ t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ runInsn(ଅଚଳ TExpr::Insn& ins,
                           ଅଚଳ ଶୂନ୍ୟ* p0, size_t y0, size_t x0,
                           ଅଚଳ ଶୂନ୍ୟ* p1, size_t y1, size_t x1,
                           ଅଚଳ ଶୂନ୍ୟ* p2, size_t y2, size_t x2,
                           ଶୂନ୍ୟ* pr, size_t yr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

ସଂରଚନା EwBody {
    ଅଚଳ TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* slotMap;
    ଅଚଳ signed ଅକ୍ଷର* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

ଶୂନ୍ୟ TExpr::outputShape(ଅଚଳ Mat* ଅଚଳ* inputs, MatShape& spatial, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& channels) ଅଚଳ
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    ତାର୍କିକ sameShape = ninputs >= 1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; sameShape && i < ninputs; i++)
    {
        ଅଚଳ Mat& a = *inputs[i];
        ଯଦି (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = ମିଥ୍ୟା;
        ଅନ୍ୟଥା ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = 0; d < a.dims; d++) ଯଦି (a.size[d] != inputs[0]->size[d]) { sameShape = ମିଥ୍ୟା; ଭାଙ୍ଗ; }
    }
    ଯଦି (sameShape) { spatial = inputs[0]->size; channels = rcn0; ଫେରନ୍ତୁ; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndFull = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ଶୂନ୍ୟ TExpr::outputShape(ଅଚଳ Mat* inputs, MatShape& spatial, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& channels) ଅଚଳ
{
    AutoBuffer<ଅଚଳ Mat*, 16> ptrs(std::max(ninputs, 1));
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା LOCAL_HDRS = 3;
ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା LOCAL_CONSTS = 4;
ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା LOCAL_OPS = 16;

ଶୂନ୍ୟ TExpr::exec(ଅଚଳ Mat* ଅଚଳ* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nslots = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    ଯଦି (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        ଅଚଳ Mat& r = *inputs[0];
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rcn = r.channels();
        ତାର୍କିକ ok = r.isContinuous();
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; ok && i < ninputs; i++)
        {
            ଅଚଳ Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 1; ok && s < nslots; s++)
            ଯଦି (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        ଅଚଳ size_t total = ok ? r.total() * rcn : 0;
        ଯଦି (ok && total == 0)
        {
            ଅଚଳ MatShape rshape = r.size;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 1; s < nslots; s++)
                ଯଦି (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ଫେରନ୍ତୁ;
        }
        ଯଦି (ok && total <= EW_FASTPATH_MAX)
        {
            ଅଚଳ MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 1; s < nslots; s++)
                ଯଦି (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ninsn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prog.size();

            ଯଦି (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା s) -> ଶୂନ୍ୟ* {
                    ଯଦି (s <= 0) ଫେରନ୍ତୁ nullptr;
                    ଅଚଳ Arg& ai = arginfo[s];
                    ଯଦି (ai.kind == ନିବେଶ)  ଫେରନ୍ତୁ (ଶୂନ୍ୟ*)inputs[ai.index]->data;
                    ଯଦି (ai.kind == OUTPUT) ଫେରନ୍ତୁ (ଶୂନ୍ୟ*)outputs[ai.index].data;
                    ଫେରନ୍ତୁ nullptr;   // NONE
                };
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0; n < ninsn; n++)
                {
                    ଅଚଳ Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                ଫେରନ୍ତୁ;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା totalEsz = bufEszPrefix[nbuffers];
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wf0 = std::min((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total, capElems);

            ଅଚଳ size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/ଆକାର(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)ଆକାର(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x0 = 0; x0 < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total; x0 += wf0)
            {
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wf = std::min(wf0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total - x0);
                auto ptrOf = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା s) -> ଶୂନ୍ୟ* {
                    ଯଦି (s <= 0) ଫେରନ୍ତୁ nullptr;
                    ଅଚଳ Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    ଯଦି (ai.kind == ନିବେଶ)  ଫେରନ୍ତୁ (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    ଯଦି (ai.kind == OUTPUT) ଫେରନ୍ତୁ (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    ଯଦି (ai.kind != TEMP)   ଫେରନ୍ତୁ nullptr;                                    // NONE: moved-from
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    ଫେରନ୍ତୁ scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0; n < ninsn; n++)
                {
                    ଅଚଳ Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            ଫେରନ୍ତୁ;
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rchannels;
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
    ତାର୍କିକ inplace = ମିଥ୍ୟା;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninputs && !inplace; i++)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < noutputs; j++)
            ଯଦି (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = ସତ୍ୟ; ଭାଙ୍ଗ; }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nhdrs = inplace ? ninputs : 0;

    AutoBuffer<ଅଚଳ Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<ଅଚଳ Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed ଅକ୍ଷର, LOCAL_OPS> slotKind(nslots);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା narr = 0, nc = 0;
    ଯଦି (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed ଅକ୍ଷର)TExpr::NONE;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 1; s < nslots; s++)
    {
        ଅଚଳ TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed ଅକ୍ଷର)ai.kind;
        ଯଦି (ai.kind == TExpr::ନିବେଶ) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        ଅନ୍ୟଥା ଯଦି (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        ଅନ୍ୟଥା ଯଦି (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed ଅକ୍ଷର)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        ଅନ୍ୟଥା ଯଦି (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (ଶୂନ୍ୟ*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed ଅକ୍ଷର)TExpr::ନିବେଶ;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        ଅନ୍ୟଥା ଯଦି (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
        }
        ଅନ୍ୟଥା {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ninsn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prog.size();
    long long costPerElem = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = 0; d < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)spatial.size(); d++) otot *= spatial[d];
    ଯଦି (otot == 0)
        ଫେରନ୍ତୁ;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ଅଚଳ ଦ୍ୱିଗୁଣ T = (ଦ୍ୱିଗୁଣ)std::max(get_ଅଙ୍କ_Threads(), 1);
    ଅଚଳ ଦ୍ୱିଗୁଣ nstripes = std::min(
        (ଦ୍ୱିଗୁଣ)otot * (ଦ୍ୱିଗୁଣ)std::max<long long>(costPerElem, 1) * (1./ (ଦ୍ୱିଗୁଣ)(1 << 16)),
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

    broadcastOp(arr.data(), narr, [&](ଅଚଳ BrTile& tile)
    {
        EwBody& bc = body;
        ଅଚଳ TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ଅଚଳ smap  = bc.slotMap;
        ଅଚଳ signed ଅକ୍ଷର* ଅଚଳ skind = bc.slotKind;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nin = bc.ninsn, nsl = bc.nslots;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ଅଚଳ eszPrefix = bc.bufEszPrefix;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା bw = std::min(w, bc.capElems);
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ଅଚଳ size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/ଆକାର(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)ଆକାର(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y0 = 0; y0 < h; y0 += bh)
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା hf = std::min(bh, h - y0);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x0 = 0; x0 < w; x0 += bw)
            {
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wf = std::min(bw, w - x0);
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    ଯଦି (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    ଅନ୍ୟଥା                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ଅଚଳ BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0; n < nin; n++)
                {
                    ଅଚଳ TExpr::Insn& ins = insns[n];
                    ଅଚଳ BrSlice& a0 = args[ins.arg0]; ଅଚଳ BrSlice& a1 = args[ins.arg1];
                    ଅଚଳ BrSlice& a2 = args[ins.arg2]; ଅଚଳ BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (ଶୂନ୍ୟ*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, ସତ୍ୟ, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ଶୂନ୍ୟ TExpr::exec(ଅଚଳ Mat* inputs, Mat* outputs)
{
    AutoBuffer<ଅଚଳ Mat*, 16> ptrs(std::max(ninputs, 1));
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
ଶୂନ୍ୟ make_ଦ୍ୱିମିକ_Arith_Program(TExpr& p, ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdepth,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା maskDepth, ଦ୍ୱିଗୁଣ scale)
{
    p.clear();
    ଯଦି (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        ଯଦି (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        ଅନ୍ୟଥା
        {
            rdepth = promoteArith(depth0, depth1);
            ଯଦି (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ଅଚଳ ତାର୍କିକ masked = maskDepth != EW_DEPTH_NONE;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sIn0  = p.add_ନିବେଶ(depth0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sIn1  = p.add_ନିବେଶ(depth1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sMask = masked ? p.add_ନିବେଶ(maskDepth) : 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sOut  = p.addOutput(rdepth);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = p.emit_ଦ୍ୱିମିକ(op, sIn0, sIn1, rdepth, Scalar(scale));
    ଯଦି (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    ଅନ୍ୟଥା
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
ଶୂନ୍ୟ makeAddWeightedProgram(TExpr& p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdepth,
                            ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ gamma)
{
    p.clear();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sA   = p.add_ନିବେଶ(depth0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sB   = p.add_ନିବେଶ(depth1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sOut = p.addOutput(rdepth);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = p.emit_ଦ୍ୱିମିକ(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

ନାମାକାଶ {

// --- tokens ------------------------------------------------------------------------------
ଗଣନା TokType { T_ଅଙ୍କ, T_ନିବେଶ, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

ସଂରଚନା Token
{
    TokType type = T_END;
    ଦ୍ୱିଗୁଣ num = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
ସଂରଚନା Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    ସ୍ଥିର ତାର୍କିକ isIdentStart(ଅକ୍ଷର c) { ଫେରନ୍ତୁ std::isalpha((unsigned ଅକ୍ଷର)c) || c == '_'; }
    ସ୍ଥିର ତାର୍କିକ isIdentChar(ଅକ୍ଷର c)  { ଫେରନ୍ତୁ std::isalnum((unsigned ଅକ୍ଷର)c) || c == '_'; }

    Token next()
    {
        ଯେତେବେଳେ (pos < s.size() && std::isspace((unsigned ଅକ୍ଷର)s[pos])) pos++;
        Token t;
        ଯଦି (pos >= s.size()) { t.type = T_END; ଫେରନ୍ତୁ t; }

        ଅକ୍ଷର c = s[pos];

        ଯଦି (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            ଯେତେବେଳେ (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_ନିବେଶ;
            t.ନିବେଶ_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            ଫେରନ୍ତୁ t;
        }
        ଯଦି (std::isdigit((unsigned ଅକ୍ଷର)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned ଅକ୍ଷର)s[pos + 1])))
        {
            ଅକ୍ଷର* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_ଅଙ୍କ;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            ଫେରନ୍ତୁ t;
        }
        ଯଦି (isIdentStart(c))
        {
            size_t start = pos;
            ଯେତେବେଳେ (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            ଫେରନ୍ତୁ t;
        }
        ବାଛ (c)
        {
        ସ୍ଥିତି '(': pos++; t.type = T_LPAREN; ଫେରନ୍ତୁ t;
        ସ୍ଥିତି ')': pos++; t.type = T_RPAREN; ଫେରନ୍ତୁ t;
        ସ୍ଥିତି ',': pos++; t.type = T_COMMA;  ଫେରନ୍ତୁ t;
        ସ୍ଥିତି ';': pos++; t.type = T_SEMI;   ଫେରନ୍ତୁ t;
        }
        // multi/!single-char operators
        auto two = [&](ଅଚଳ ଅକ୍ଷର* op) {
            ଫେରନ୍ତୁ pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        ଯଦି (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; ଫେରନ୍ତୁ t; }
        ଯଦି (c == '=') { pos++; t.type = T_ASSIGN; ଫେରନ୍ତୁ t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        ଫେରନ୍ତୁ t;
    }
};

// --- operator / function tables ----------------------------------------------------------
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା binPrec(ଅଚଳ std::string& op)
{
    ଯଦି (op == "**") ଫେରନ୍ତୁ 8;              // power binds tighter than '*'; RIGHT-associative
    ଯଦି (op == "*" || op == "/") ଫେରନ୍ତୁ 7;
    ଯଦି (op == "+" || op == "-") ଫେରନ୍ତୁ 6;
    ଯଦି (op == "<" || op == "<=" || op == ">" || op == ">=") ଫେରନ୍ତୁ 5;
    ଯଦି (op == "==" || op == "!=") ଫେରନ୍ତୁ 4;
    ଯଦି (op == "&") ଫେରନ୍ତୁ 3;
    ଯଦି (op == "^") ଫେରନ୍ତୁ 2;
    ଯଦି (op == "|") ଫେରନ୍ତୁ 1;
    ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ଶୀର୍ଷ binOp(ଅଚଳ std::string& op)
{
    ଯଦି (op == "**") ଫେରନ୍ତୁ OP_POW;
    ଯଦି (op == "+")  ଫେରନ୍ତୁ OP_ADD;
    ଯଦି (op == "-")  ଫେରନ୍ତୁ OP_SUB;
    ଯଦି (op == "*")  ଫେରନ୍ତୁ OP_MUL;
    ଯଦି (op == "/")  ଫେରନ୍ତୁ OP_DIV;
    ଯଦି (op == "<")  ଫେରନ୍ତୁ OP_CMP_LT;
    ଯଦି (op == "<=") ଫେରନ୍ତୁ OP_CMP_LE;
    ଯଦି (op == ">")  ଫେରନ୍ତୁ OP_CMP_GT;
    ଯଦି (op == ">=") ଫେରନ୍ତୁ OP_CMP_GE;
    ଯଦି (op == "==") ଫେରନ୍ତୁ OP_CMP_EQ;
    ଯଦି (op == "!=") ଫେରନ୍ତୁ OP_CMP_NE;
    ଯଦି (op == "&")  ଫେରନ୍ତୁ OP_AND;
    ଯଦି (op == "|")  ଫେରନ୍ତୁ OP_OR;
    ଯଦି (op == "^")  ଫେରନ୍ତୁ OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା typeDepth(ଅଚଳ std::string& name)
{
    ଯଦି (name == "float")    ଫେରନ୍ତୁ CV_32F;
    ଯଦି (name == "double")   ଫେରନ୍ତୁ CV_64F;
    ଯଦି (name == "half" || name == "float16") ଫେରନ୍ତୁ CV_16F;
    ଯଦି (name == "bfloat16") ଫେରନ୍ତୁ CV_16BF;
    ଯଦି (name == "uint8")    ଫେରନ୍ତୁ CV_8U;
    ଯଦି (name == "int8")     ଫେରନ୍ତୁ CV_8S;
    ଯଦି (name == "uint16")   ଫେରନ୍ତୁ CV_16U;
    ଯଦି (name == "int16")    ଫେରନ୍ତୁ CV_16S;
    ଯଦି (name == "uint32")   ଫେରନ୍ତୁ CV_32U;
    ଯଦି (name == "int32")    ଫେରନ୍ତୁ CV_32S;
    ଯଦି (name == "uint64")   ଫେରନ୍ତୁ CV_64U;
    ଯଦି (name == "int64")    ଫେରନ୍ତୁ CV_64S;
    ଫେରନ୍ତୁ -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
ସ୍ଥିର ଶୀର୍ଷ fnOp(ଅଚଳ std::string& name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& arity)
{
    ସଂରଚନା E { ଅଚଳ ଅକ୍ଷର* n; ଶୀର୍ଷ op; };
    ସ୍ଥିର ଅଚଳ E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    ସ୍ଥିର ଅଚଳ E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    ସ୍ଥିର ଅଚଳ E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    ପାଇଁ (ଅଚଳ E& e : unary)  ଯଦି (name == e.n) { arity = 1; ଫେରନ୍ତୁ e.op; }
    ପାଇଁ (ଅଚଳ E& e : binary) ଯଦି (name == e.n) { arity = 2; ଫେରନ୍ତୁ e.op; }
    ପାଇଁ (ଅଚଳ E& e : tern)   ଯଦି (name == e.n) { arity = 3; ଫେରନ୍ତୁ e.op; }
    arity = 0; ଫେରନ୍ତୁ OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
ସଂରଚନା Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ନିବେଶ_Slot;                 // slot id of each external input (precreated)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ninputs;
    std::map<std::string, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& ପଦସମଷ୍ଟି, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* islot, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nin)
        : lex(src), e(ପଦସମଷ୍ଟି), ନିବେଶ_Slot(islot), ninputs(nin) { cur = lex.next(); }

    ଶୂନ୍ୟ advance() { cur = lex.next(); }
    ତାର୍କିକ isOp(ଅଚଳ ଅକ୍ଷର* op) ଅଚଳ { ଫେରନ୍ତୁ cur.type == T_OP && cur.text == op; }

    ଶୂନ୍ୟ expect(TokType t, ଅଚଳ ଅକ୍ଷର* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    ତାର୍କିକ isFlexConst(ପୂର୍ଣ୍ଣସଂଖ୍ୟା slot) ଅଚଳ
    {
        ଫେରନ୍ତୁ e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା parsePrimary()
    {
        ଯଦି (cur.type == T_ଅଙ୍କ)   { ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); ଫେରନ୍ତୁ s; }
        ଯଦି (cur.type == T_ନିବେଶ) { ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = cur.ନିବେଶ_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   ଫେରନ୍ତୁ ନିବେଶ_Slot[idx]; }
        ଯଦି (cur.type == T_LPAREN){ advance(); ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = parseTernary(); expect(T_RPAREN, "expected ')'"); ଫେରନ୍ତୁ x; }
        ଯଦି (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            ଯଦି (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                ଫେରନ୍ତୁ it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 8> args = {}; ପୂର୍ଣ୍ଣସଂଖ୍ୟା nargs = 0;
            ଯଦି (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                ଯେତେବେଳେ (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା td = typeDepth(name);
            ଯଦି (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = args[0];
                ଫେରନ୍ତୁ isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା arity = 0; ଶୀର୍ଷ op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            ଯଦି (arity == 1) ଫେରନ୍ତୁ e.emitUnary(op, args[0]);
            ଯଦି (arity == 2) ଫେରନ୍ତୁ e.emit_ଦ୍ୱିମିକ(op, args[0], args[1]);
            ଫେରନ୍ତୁ e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା parseUnary()
    {
        ଯଦି (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            ଯଦି (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<ଦ୍ୱିଗୁଣ, MAX_LOCAL_CN> v; ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = constDoubles(e, operand, v);
                Scalar neg; ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                ଫେରନ୍ତୁ e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            ଫେରନ୍ତୁ e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        ଫେରନ୍ତୁ parsePrimary();
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_ପଦସମଷ୍ଟି(ପୂର୍ଣ୍ଣସଂଖ୍ୟା minPrec)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ = parseUnary();
        ଯେତେବେଳେ (cur.type == T_OP)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = binPrec(cur.text);
            ଯଦି (p < minPrec) ଭାଙ୍ଗ;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ = parse_ପଦସମଷ୍ଟି(op == "**" ? p : p + 1);
            ବାମ = e.emit_ଦ୍ୱିମିକ(binOp(op), ବାମ, ଡାହାଣ);
        }
        ଫେରନ୍ତୁ ବାମ;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା parseTernary()
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cond = parse_ପଦସମଷ୍ଟି(0);
        ଯଦି (!isOp("?")) ଫେରନ୍ତୁ cond;
        advance();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା elsev = parseTernary();               // right-associative chain
        ଫେରନ୍ତୁ e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    ଶୂନ୍ୟ parseResult()
    {
        ଯଦି (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); ପୂର୍ଣ୍ଣସଂଖ୍ୟା nTemp = e.ntemps;
            advance();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା e0 = parseTernary();
            ଯଦି (cur.type == T_COMMA)
            {
                e.output(e0);
                ଯେତେବେଳେ (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                ଫେରନ୍ତୁ;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    ଶୂନ୍ୟ parse()
    {
        ଯେତେବେଳେ (ସତ୍ୟ)
        {
            ଯଦି (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                ଯଦି (cur.type == T_ASSIGN)
                {
                    advance();
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = ସତ୍ୟ;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    ଜାରି;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            ଯଦି (cur.type == T_SEMI) advance();
            ଭାଙ୍ଗ;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

ନାମାକାଶ cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
ଶୂନ୍ୟ texpr(ଅଚଳ std::string& ପଦସମଷ୍ଟି, ନିବେଶ_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using ନାମାକାଶ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == ନିବେଶ_Array_2::STD_VECTOR_MAT);
    ଅଚଳ std::vector<Mat>& inps = *(ଅଚଳ std::vector<Mat>*)_inputs.getObj();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ninputs = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)inps.size();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> islot(std::max(ninputs, 1));
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ninputs; i++) islot[i] = e.add_ନିବେଶ(inps[i].depth());
    Parser(ପଦସମଷ୍ଟି, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    ଯଦି (kind == ନିବେଶ_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } ଅନ୍ୟଥା {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
