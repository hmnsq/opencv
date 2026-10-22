// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <climits>
#ಅನ್ತರ್ಭಾವ <cmath>
#ಅನ್ತರ್ಭಾವ <cstdlib>
#ಅನ್ತರ್ಭಾವ <cstring>
#ಅನ್ತರ್ಭಾವ <map>
#ಅನ್ತರ್ಭಾವ <ostream>
#ಅನ್ತರ್ಭಾವ <string>

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ew {

// ============================ op metadata (was ew_op.cpp) ============================

ಧ್ರುವ ವರ್ಣ* opName(ತುದಿ op)
{
    ವಿಕಲ್ಪ (op)
    {
    ಪಕ್ಷ OP_NOP:           ಪ್ರತ್ಯಾಗಮ "nop";
    ಪಕ್ಷ OP_NEG:           ಪ್ರತ್ಯಾಗಮ "neg";
    ಪಕ್ಷ OP_ABS:           ಪ್ರತ್ಯಾಗಮ "abs";
    ಪಕ್ಷ OP_NOT:           ಪ್ರತ್ಯಾಗಮ "not";
    ಪಕ್ಷ OP_SQRT:          ಪ್ರತ್ಯಾಗಮ "sqrt";
    ಪಕ್ಷ OP_EXP:           ಪ್ರತ್ಯಾಗಮ "exp";
    ಪಕ್ಷ OP_LOG:           ಪ್ರತ್ಯಾಗಮ "log";
    ಪಕ್ಷ OP_SIN:           ಪ್ರತ್ಯಾಗಮ "sin";
    ಪಕ್ಷ OP_COS:           ಪ್ರತ್ಯಾಗಮ "cos";
    ಪಕ್ಷ OP_TANH:          ಪ್ರತ್ಯಾಗಮ "tanh";
    ಪಕ್ಷ OP_ERF:           ಪ್ರತ್ಯಾಗಮ "erf";
    ಪಕ್ಷ OP_RELU:          ಪ್ರತ್ಯಾಗಮ "relu";
    ಪಕ್ಷ OP_CAST:          ಪ್ರತ್ಯಾಗಮ "cast";
    ಪಕ್ಷ OP_ADD:           ಪ್ರತ್ಯಾಗಮ "add";
    ಪಕ್ಷ OP_SUB:           ಪ್ರತ್ಯಾಗಮ "sub";
    ಪಕ್ಷ OP_MUL:           ಪ್ರತ್ಯಾಗಮ "mul";
    ಪಕ್ಷ OP_DIV:           ಪ್ರತ್ಯಾಗಮ "div";
    ಪಕ್ಷ OP_POW:           ಪ್ರತ್ಯಾಗಮ "pow";
    ಪಕ್ಷ OP_MIN:           ಪ್ರತ್ಯಾಗಮ "min";
    ಪಕ್ಷ OP_MAX:           ಪ್ರತ್ಯಾಗಮ "max";
    ಪಕ್ಷ OP_ABSDIFF:       ಪ್ರತ್ಯಾಗಮ "absdiff";
    ಪಕ್ಷ OP_ADDW:          ಪ್ರತ್ಯಾಗಮ "addWeighted";
    ಪಕ್ಷ OP_HYPOT:         ಪ್ರತ್ಯಾಗಮ "hypot";
    ಪಕ್ಷ OP_ATAN2:         ಪ್ರತ್ಯಾಗಮ "atan2";
    ಪಕ್ಷ OP_AND:           ಪ್ರತ್ಯಾಗಮ "and";
    ಪಕ್ಷ OP_OR:            ಪ್ರತ್ಯಾಗಮ "or";
    ಪಕ್ಷ OP_XOR:           ಪ್ರತ್ಯಾಗಮ "xor";
    ಪಕ್ಷ OP_CMP_EQ:        ಪ್ರತ್ಯಾಗಮ "cmp_eq";
    ಪಕ್ಷ OP_CMP_NE:        ಪ್ರತ್ಯಾಗಮ "cmp_ne";
    ಪಕ್ಷ OP_CMP_LT:        ಪ್ರತ್ಯಾಗಮ "cmp_lt";
    ಪಕ್ಷ OP_CMP_LE:        ಪ್ರತ್ಯಾಗಮ "cmp_le";
    ಪಕ್ಷ OP_CMP_GT:        ಪ್ರತ್ಯಾಗಮ "cmp_gt";
    ಪಕ್ಷ OP_CMP_GE:        ಪ್ರತ್ಯಾಗಮ "cmp_ge";
    ಪಕ್ಷ OP_CLAMP:         ಪ್ರತ್ಯಾಗಮ "clamp";
    ಪಕ್ಷ OP_SELECT:        ಪ್ರತ್ಯಾಗಮ "select";
    ಪಕ್ಷ OP_CONVERT_SCALE: ಪ್ರತ್ಯಾಗಮ "convert_scale";
    ಸ್ವಭಾವ:               ಪ್ರತ್ಯಾಗಮ "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(ತುದಿ op)
{
    ವಿಕಲ್ಪ (op)
    {
    ಪಕ್ಷ OP_AND: ಪಕ್ಷ OP_OR: ಪಕ್ಷ OP_XOR: ಪಕ್ಷ OP_NOT:
        ಪ್ರತ್ಯಾಗಮ CAT_BITWISE;
    ಪಕ್ಷ OP_CMP_EQ: ಪಕ್ಷ OP_CMP_NE: ಪಕ್ಷ OP_CMP_LT:
    ಪಕ್ಷ OP_CMP_LE: ಪಕ್ಷ OP_CMP_GT: ಪಕ್ಷ OP_CMP_GE:
        ಪ್ರತ್ಯಾಗಮ CAT_COMPARE;
    ಪಕ್ಷ OP_SQRT: ಪಕ್ಷ OP_EXP: ಪಕ್ಷ OP_LOG:
    ಪಕ್ಷ OP_SIN: ಪಕ್ಷ OP_COS: ಪಕ್ಷ OP_TANH: ಪಕ್ಷ OP_ERF: ಪಕ್ಷ OP_ATAN2: ಪಕ್ಷ OP_RELU:
        ಪ್ರತ್ಯಾಗಮ CAT_MATH;
    ಪಕ್ಷ OP_CAST: ಪಕ್ಷ OP_CONVERT_SCALE:
        ಪ್ರತ್ಯಾಗಮ CAT_CAST;
    ಪಕ್ಷ OP_SELECT:
        ಪ್ರತ್ಯಾಗಮ CAT_SELECT;
    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
ಸ್ಥಿರ ತಾರ್ಕಿಕ isFloatDepth(ಪೂರ್ಣಾಙ್ಕ d)
{
    ಪ್ರತ್ಯಾಗಮ d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
ಸ್ಥಿರ ತಾರ್ಕಿಕ depthRepresents(ದ್ವಿಗುಣ v, ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಯದಿ (isFloatDepth(depth)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಯದಿ (v != std::floor(v)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ವಿಕಲ್ಪ (depth)
    {
    ಪಕ್ಷ CV_8U:  ಪ್ರತ್ಯಾಗಮ v >= 0            && v <= 255;
    ಪಕ್ಷ CV_8S:  ಪ್ರತ್ಯಾಗಮ v >= -128         && v <= 127;
    ಪಕ್ಷ CV_16U: ಪ್ರತ್ಯಾಗಮ v >= 0            && v <= 65535;
    ಪಕ್ಷ CV_16S: ಪ್ರತ್ಯಾಗಮ v >= -32768       && v <= 32767;
    ಪಕ್ಷ CV_32U: ಪ್ರತ್ಯಾಗಮ v >= 0            && v <= 4294967295.0;
    ಪಕ್ಷ CV_32S: ಪ್ರತ್ಯಾಗಮ v >= -2147483648.0 && v <= 2147483647.0;
    ಪಕ್ಷ CV_64U: ಪ್ರತ್ಯಾಗಮ v >= 0            && v <= 18446744073709551615.0;
    ಪಕ್ಷ CV_64S: ಪ್ರತ್ಯಾಗಮ v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    ಸ್ವಭಾವ:     ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
ಸ್ಥಿರ ಶೂನ್ಯ intRange(ಪೂರ್ಣಾಙ್ಕ depth, ದ್ವಿಗುಣ& lo, ದ್ವಿಗುಣ& hi)
{
    ವಿಕಲ್ಪ (depth)
    {
    ಪಕ್ಷ CV_8U:  lo = 0;                     hi = 255; ಭಙ್ಗ;
    ಪಕ್ಷ CV_8S:  lo = -128;                  hi = 127; ಭಙ್ಗ;
    ಪಕ್ಷ CV_16U: lo = 0;                     hi = 65535; ಭಙ್ಗ;
    ಪಕ್ಷ CV_16S: lo = -32768;                hi = 32767; ಭಙ್ಗ;
    ಪಕ್ಷ CV_32U: lo = 0;                     hi = 4294967295.0; ಭಙ್ಗ;
    ಪಕ್ಷ CV_32S: lo = -2147483648.0;         hi = 2147483647.0; ಭಙ್ಗ;
    ಪಕ್ಷ CV_64U: lo = 0;                     hi = 18446744073709551615.0; ಭಙ್ಗ;
    ಪಕ್ಷ CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; ಭಙ್ಗ;
    ಸ್ವಭಾವ:     lo = 0;                     hi = 0; ಭಙ್ಗ;
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
ಪೂರ್ಣಾಙ್ಕ promoteArith(ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    ಯದಿ (a == b) ಪ್ರತ್ಯಾಗಮ a;

    ಪೂರ್ಣಾಙ್ಕ wa = ಪೂರ್ಣಾಙ್ಕ((leszlut >> (a+1)*3) & 7u), wb = ಪೂರ್ಣಾಙ್ಕ((leszlut >> (b+1)*3) & 7u);
    ಪೂರ್ಣಾಙ್ಕ fa = ಪೂರ್ಣಾಙ್ಕ((fltlut >> (a+1)) & 1u),    fb = ಪೂರ್ಣಾಙ್ಕ((fltlut >> (b+1)) & 1u);

    ಯದಿ (fa + fb == 0)                          // both integer
    {
        ಪೂರ್ಣಾಙ್ಕ sa = ಪೂರ್ಣಾಙ್ಕ((signlut >> (a+1)) & 1u), sb = ಪೂರ್ಣಾಙ್ಕ((signlut >> (b+1)) & 1u);
        ಯದಿ (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            ಪ್ರತ್ಯಾಗಮ wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        ಪೂರ್ಣಾಙ್ಕ rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr ಪೂರ್ಣಾಙ್ಕ ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        ಪ್ರತ್ಯಾಗಮ (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    ಪೂರ್ಣಾಙ್ಕ maxw = std::max(wa, wb);
    ಯದಿ (maxw == 2 && fa + fb == 1)
        ಪ್ರತ್ಯಾಗಮ fa*a + (1 - fa)*b;
    ಪ್ರತ್ಯಾಗಮ CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
ಪೂರ್ಣಾಙ್ಕ absdiffResultDepth(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ವಿಕಲ್ಪ (depth)
    {
    ಪಕ್ಷ CV_8S:  ಪ್ರತ್ಯಾಗಮ CV_8U;
    ಪಕ್ಷ CV_16S: ಪ್ರತ್ಯಾಗಮ CV_16U;
    ಪಕ್ಷ CV_32S: ಪ್ರತ್ಯಾಗಮ CV_32U;
    ಪಕ್ಷ CV_64S: ಪ್ರತ್ಯಾಗಮ CV_64U;
    ಸ್ವಭಾವ:     ಪ್ರತ್ಯಾಗಮ depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ promote2(ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ b)
{
    ಯದಿ (a == b) ಪ್ರತ್ಯಾಗಮ a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ಧ್ರುವ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ಧ್ರುವ unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ಪ್ರತ್ಯಾಗಮ ಪೂರ್ಣಾಙ್ಕ((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ promoteBitwise(ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ b)
{
    ಯದಿ (a == EW_DEPTH_NONE) ಪ್ರತ್ಯಾಗಮ b;
    ಯದಿ (b == EW_DEPTH_NONE) ಪ್ರತ್ಯಾಗಮ a;
    ಯದಿ (a == b) ಪ್ರತ್ಯಾಗಮ a;
    ಪ್ರತ್ಯಾಗಮ CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ safeWide(ಪೂರ್ಣಾಙ್ಕ depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ಧ್ರುವ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ಧ್ರುವ unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ಪ್ರತ್ಯಾಗಮ ಪೂರ್ಣಾಙ್ಕ((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
ಗಣನಾ { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
ಸ್ಥಿರ size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, ಪೂರ್ಣಾಙ್ಕ srcdepth, ಧ್ರುವ ಶೂನ್ಯ* data, ಪೂರ್ಣಾಙ್ಕ channels)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = std::max(1, channels);
    ಧ್ರುವ size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ಧ್ರುವ size_t nu64 = (nbytes + ಪರಿಮಾಣ(uint64_t) - 1) / ಪರಿಮಾಣ(uint64_t);
    ಧ್ರುವ size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    ಯದಿ (data) memcpy(dst, data, nbytes); ಅನ್ಯಥಾ memset(dst, 0, nbytes);
    ಪ್ರತ್ಯಾಗಮ ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ constDoubles(ಧ್ರುವ TExpr& e, ಪೂರ್ಣಾಙ್ಕ s, AutoBuffer<ದ್ವಿಗುಣ, MAX_LOCAL_CN>& buf)
{
    ಧ್ರುವ TExpr::Arg& a = e.arginfo[s];
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = std::max(1, a.channels);
    buf.resize(cn);
    ಧ್ರುವ uchar* src = (ಧ್ರುವ uchar*)(e.constbuf.data() + a.constofs);
    ಯದಿ (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * ಪರಿಮಾಣ(ದ್ವಿಗುಣ));   // common (Scalar / parsed literal)
    ಅನ್ಯಥಾ
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    ಪ್ರತ್ಯಾಗಮ cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isFlexConst(ಧ್ರುವ TExpr& e, ಪೂರ್ಣಾಙ್ಕ s)
{
    ಪ್ರತ್ಯಾಗಮ e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
ಸ್ಥಿರ ತಾರ್ಕಿಕ scalar_Const_ಮೌಲ್ಯ(ಧ್ರುವ TExpr& e, ಪೂರ್ಣಾಙ್ಕ s, ದ್ವಿಗುಣ& v)
{
    ಧ್ರುವ TExpr::Arg& a = e.arginfo[s];
    ಯದಿ (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    AutoBuffer<ದ್ವಿಗುಣ, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
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
ಸ್ಥಿರ ತಾರ್ಕಿಕ matchScaledChain(ಧ್ರುವ TExpr& e, ಪೂರ್ಣಾಙ್ಕ end, ಪೂರ್ಣಾಙ್ಕ t,
                             ಪೂರ್ಣಾಙ್ಕ& arrSlot, ದ್ವಿಗುಣ& scale, ಪೂರ್ಣಾಙ್ಕ& startIdx)
{
    ಯದಿ (end < 0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ j = end, mulResult = t;

    // optional trailing narrowing cast
    ಯದಿ (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        ಯದಿ (--j < 0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಧ್ರುವ TExpr::Insn& mul = e.prog[j];
    ಯದಿ (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ arr;
    ಯದಿ      (scalar_Const_ಮೌಲ್ಯ(e, mul.arg1, scale)) arr = mul.arg0;
    ಅನ್ಯಥಾ ಯದಿ (scalar_Const_ಮೌಲ್ಯ(e, mul.arg0, scale)) arr = mul.arg1;
    ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    ಯದಿ (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    ಯದಿ (e.arginfo[arr].kind == TExpr::CONST) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
ಸ್ಥಿರ ತಾರ್ಕಿಕ constFits(ಧ್ರುವ TExpr& e, ಪೂರ್ಣಾಙ್ಕ s, ಪೂರ್ಣಾಙ್ಕ d)
{
    ಯದಿ (!isFlexConst(e, s)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    AutoBuffer<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v;
    ಪೂರ್ಣಾಙ್ಕ cn = constDoubles(e, s, v);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ ch = 0; ch < cn; ch++)
        ಯದಿ (!depthRepresents(v[ch], d)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಙ್ಕ TExpr::emit_ದ್ವಿಮಾನ(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ b, ಪೂರ್ಣಾಙ್ಕ rdepth, ಧ್ರುವ Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    ಯದಿ (op == OP_ADDW)
    {
        ಪೂರ್ಣಾಙ್ಕ Tt = arginfo[a].depth;
        ಯದಿ (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        ಯದಿ (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        ಯದಿ (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        ಪೂರ್ಣಾಙ್ಕ outD = rdepth;
        ಯದಿ (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        ಯದಿ (outD == rdepth) ಪ್ರತ್ಯಾಗಮ out;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        ಪ್ರತ್ಯಾಗಮ out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    ಯದಿ (op == OP_ADD)
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n = (ಪೂರ್ಣಾಙ್ಕ)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        ಪೂರ್ಣಾಙ್ಕ x = 0, y = 0, startA = 0, startB = 0;
        ದ್ವಿಗುಣ alpha = 0, beta = 0;
        ಯದಿ (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*ಏತತ್, n - 1, b, y, beta, startB) &&
            matchScaledChain(*ಏತತ್, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            ಪೂರ್ಣಾಙ್ಕ retired = 0, lowest = INT_MAX;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = startA; i < n; i++)
            {
                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ r = prog[i].result;
                ಯದಿ (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; ಭಙ್ಗ; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            ಯದಿ (retired > 0 && lowest == ntemps - retired)
            {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                ಪ್ರತ್ಯಾಗಮ emit_ದ್ವಿಮಾನ(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            ಪೂರ್ಣಾಙ್ಕ acc = 0; ದ್ವಿಗುಣ gamma = 0;
            ಯದಿ      (scalar_Const_ಮೌಲ್ಯ(*ಏತತ್, b, gamma)) acc = a;
            ಅನ್ಯಥಾ ಯದಿ (scalar_Const_ಮೌಲ್ಯ(*ಏತತ್, a, gamma)) acc = b;
            ಯದಿ (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                ಪ್ರತ್ಯಾಗಮ acc;
            }
        }
    }

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nd0 = isFlexConst(*ಏತತ್, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nd1 = isFlexConst(*ಏತತ್, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    ಧ್ರುವ ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    ಪೂರ್ಣಾಙ್ಕ result = rdepth;
    ಯದಿ (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        ಯದಿ (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    ಪೂರ್ಣಾಙ್ಕ base;
    ಯದಿ (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    ಅನ್ಯಥಾ ಯದಿ (nd0 == EW_DEPTH_NONE) base = nd1;
    ಅನ್ಯಥಾ ಯದಿ (nd1 == EW_DEPTH_NONE) base = nd0;
    ಅನ್ಯಥಾ base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    ಯದಿ (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*ಏತತ್, b) && !constFits(*ಏತತ್, b, base))
    {
        ದ್ವಿಗುಣ lo, hi; intRange(nd0, lo, hi);
        ಯದಿ (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            ಗಣನಾ { REAL, CFALSE, CTRUE };
            AutoBuffer<ದ್ವಿಗುಣ, MAX_LOCAL_CN> tv; ಪೂರ್ಣಾಙ್ಕ cn = constDoubles(*ಏತತ್, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            ಪೂರ್ಣಾಙ್ಕ kind[4] = {}; ದ್ವಿಗುಣ bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ಧ್ರುವ ತುದಿ fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < cn; c++)
            {
                ಧ್ರುವ ದ್ವಿಗುಣ t = tv[c]; ಪೂರ್ಣಾಙ್ಕ k; ದ್ವಿಗುಣ B = t;
                ವಿಕಲ್ಪ (op)
                {
                ಪಕ್ಷ OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ಭಙ್ಗ;
                ಪಕ್ಷ OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ಭಙ್ಗ;
                ಪಕ್ಷ OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ಭಙ್ಗ;
                ಪಕ್ಷ OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ಭಙ್ಗ;
                ಪಕ್ಷ OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; ಭಙ್ಗ;   // a==non-rep -> false
                ಸ್ವಭಾವ:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  ಭಙ್ಗ;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](ಪೂರ್ಣಾಙ್ಕ c){ ಪ್ರತ್ಯಾಗಮ kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](ಪೂರ್ಣಾಙ್ಕ c){ ಪ್ರತ್ಯಾಗಮ kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ಧ್ರುವ ತುದಿ u0 = ucOp(0);
            ತಾರ್ಕಿಕ uniform = ಸತ್ಯ;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            ಯದಿ (uniform)
            {
                Scalar s; ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            ಅನ್ಯಥಾ
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; ಪೂರ್ಣಾಙ್ಕ patchFlags = 0;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                ಪ್ರತ್ಯಾಗಮ out;
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
    ಪೂರ್ಣಾಙ್ಕ depth, wide;
    ತಾರ್ಕಿಕ divGuard = ಅಸತ್ಯ;
    ವಿಕಲ್ಪ (op)
    {
    ಪಕ್ಷ OP_MIN: ಪಕ್ಷ OP_MAX:
    ಪಕ್ಷ OP_AND: ಪಕ್ಷ OP_OR: ಪಕ್ಷ OP_XOR:
        depth = result; wide = result; ಭಙ್ಗ;
    ಪಕ್ಷ OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        ಪೂರ್ಣಾಙ್ಕ common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        ಯದಿ (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        ಯದಿ (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        ಭಙ್ಗ;
    }
    ಪಕ್ಷ OP_CMP_EQ: ಪಕ್ಷ OP_CMP_NE: ಪಕ್ಷ OP_CMP_LT:
    ಪಕ್ಷ OP_CMP_LE: ಪಕ್ಷ OP_CMP_GT: ಪಕ್ಷ OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; ಭಙ್ಗ;
    ಪಕ್ಷ OP_MUL: ಪಕ್ಷ OP_DIV:
    {
        depth = base;
        ಧ್ರುವ ತಾರ್ಕಿಕ w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        ಯದಿ (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](ಪೂರ್ಣಾಙ್ಕ s, ಪೂರ್ಣಾಙ್ಕ nd) {
                ಯದಿ (nd != EW_DEPTH_NONE) ಪ್ರತ್ಯಾಗಮ !isFloatDepth(nd);
                AutoBuffer<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v; constDoubles(*ಏತತ್, s, v);
                ಪ್ರತ್ಯಾಗಮ v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        ಭಙ್ಗ;
    }
    ಪಕ್ಷ OP_POW: ಪಕ್ಷ OP_HYPOT: ಪಕ್ಷ OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; ಭಙ್ಗ;
    ಸ್ವಭಾವ:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); ಭಙ್ಗ;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](ಪೂರ್ಣಾಙ್ಕ s) {
        ಯದಿ (!isFlexConst(*ಏತತ್, s)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        AutoBuffer<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v; ಪೂರ್ಣಾಙ್ಕ n = constDoubles(*ಏತತ್, s, v);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ ch = 0; ch < n; ch++)
            ಯದಿ (v[ch] != std::floor(v[ch])) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    ಯದಿ (cat != CAT_BITWISE && (!constFits(*ಏತತ್, a, depth) || !constFits(*ಏತತ್, b, depth)))
    {
        depth = wide;
        ಧ್ರುವ ತಾರ್ಕಿಕ frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        ಯದಿ (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        ಯದಿ (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    ಪೂರ್ಣಾಙ್ಕ s0 = isFlexConst(*ಏತತ್, a) ? typedConstFrom(a, depth) : a;
    ಪೂರ್ಣಾಙ್ಕ s1 = isFlexConst(*ಏತತ್, b) ? typedConstFrom(b, depth) : b;
    ಪೂರ್ಣಾಙ್ಕ c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](ಪೂರ್ಣಾಙ್ಕ rd) {
        ಪ್ರತ್ಯಾಗಮ op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    ಯದಿ (k.fptr)
    {
        ಪೂರ್ಣಾಙ್ಕ res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        ಪ್ರತ್ಯಾಗಮ res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ಪೂರ್ಣಾಙ್ಕ w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    ಪ್ರತ್ಯಾಗಮ maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಙ್ಕ TExpr::emitUnary(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ rdepth, ಧ್ರುವ Scalar& params)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nd = isFlexConst(*ಏತತ್, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ಧ್ರುವ ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    ಯದಿ (op == OP_NEG)
        ಪ್ರತ್ಯಾಗಮ emit_ದ್ವಿಮಾನ(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    ಯದಿ (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        ಯದಿ (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            ಪ್ರತ್ಯಾಗಮ emit_ದ್ವಿಮಾನ(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        ಪ್ರತ್ಯಾಗಮ emit_ದ್ವಿಮಾನ(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    ಪೂರ್ಣಾಙ್ಕ result = rdepth;
    ಯದಿ (result == EW_DEPTH_NONE)
    {
        ವಿಕಲ್ಪ (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        ಪಕ್ಷ CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; ಭಙ್ಗ;
        ಪಕ್ಷ CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; ಭಙ್ಗ;  // NOT
        ಸ್ವಭಾವ:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; ಭಙ್ಗ;  // NEG, ABS
        }
    }

    ಪೂರ್ಣಾಙ್ಕ depth, wide;
    ವಿಕಲ್ಪ (cat)
    {
    ಪಕ್ಷ CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        ಭಙ್ಗ;
    ಪಕ್ಷ CAT_BITWISE: depth = result; wide = result; ಭಙ್ಗ;
    ಸ್ವಭಾವ:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; ಭಙ್ಗ;  // NEG/ABS
    }

    ಯದಿ (!constFits(*ಏತತ್, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    ಪೂರ್ಣಾಙ್ಕ s0 = isFlexConst(*ಏತತ್, a) ? typedConstFrom(a, depth) : a;
    ಪೂರ್ಣಾಙ್ಕ c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    ಯದಿ (k.fptr)
    {
        ಪೂರ್ಣಾಙ್ಕ res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        ಪ್ರತ್ಯಾಗಮ res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ಪೂರ್ಣಾಙ್ಕ w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    ಪ್ರತ್ಯಾಗಮ maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಙ್ಕ TExpr::emitTernary(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ b, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ rdepth)
{
    auto dep = [&](ಪೂರ್ಣಾಙ್ಕ s) { ಪ್ರತ್ಯಾಗಮ isFlexConst(*ಏತತ್, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    ಯದಿ (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        ಯದಿ (isFlexConst(*ಏತತ್, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_ದ್ವಿಮಾನ(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        ಪೂರ್ಣಾಙ್ಕ result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        ಯದಿ (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        ಪೂರ್ಣಾಙ್ಕ sb = isFlexConst(*ಏತತ್, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        ಪೂರ್ಣಾಙ್ಕ sc = isFlexConst(*ಏತತ್, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        ಪೂರ್ಣಾಙ್ಕ res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        ಪ್ರತ್ಯಾಗಮ res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    ಪೂರ್ಣಾಙ್ಕ result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    ಯದಿ (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](ಪೂರ್ಣಾಙ್ಕ s) {
        ಪ್ರತ್ಯಾಗಮ isFlexConst(*ಏತತ್, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    ಪೂರ್ಣಾಙ್ಕ c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ಪೂರ್ಣಾಙ್ಕ res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    ಪ್ರತ್ಯಾಗಮ res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಙ್ಕ TExpr::moveToOutput(ಪೂರ್ಣಾಙ್ಕ temp, ಪೂರ್ಣಾಙ್ಕ out)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ninsn = (ಪೂರ್ಣಾಙ್ಕ)prog.size();
    ಪೂರ್ಣಾಙ್ಕ producer = -1;
    ತಾರ್ಕಿಕ usedAsArg = ಅಸತ್ಯ;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninsn; i++)
    {
        ಧ್ರುವ TExpr::Insn& ins = prog[i];
        ಯದಿ (ins.result == temp) producer = i;
        ಯದಿ (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = ಸತ್ಯ;
    }
    ಯದಿ (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        ಪ್ರತ್ಯಾಗಮ out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    ಪ್ರತ್ಯಾಗಮ out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಙ್ಕ TExpr::output(ಪೂರ್ಣಾಙ್ಕ ಬೇರು_Slot)
{
    ಪ್ರತ್ಯಾಗಮ moveToOutput(ಬೇರು_Slot, addOutput(arginfo[ಬೇರು_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
ಶೂನ್ಯ TExpr::compile()
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ninsn = (ಪೂರ್ಣಾಙ್ಕ)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nslots = (ಪೂರ್ಣಾಙ್ಕ)arginfo.size();
    nconsts = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        ಯದಿ (a.kind != CONST || a.depth == EW_DEPTH_NONE) ಅನುವರ್ತ;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        ಧ್ರುವ size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, ಪರಿಮಾಣ(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)ಪರಿಮಾಣ(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (ಧ್ರುವ uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        ಧ್ರುವ size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        ಯದಿ (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        ಅನ್ಯಥಾ getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
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
    ಯದಿ (ntemps == 0) ಪ್ರತ್ಯಾಗಮ;

    AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, 32> tempOfSlot(nslots);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 1; s < nslots; s++)
        ಯದಿ (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, 16> lastUse(ntemps);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ t = 0; t < ntemps; t++) lastUse[t] = -1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninsn; i++)
    {
        ಧ್ರುವ TExpr::Insn& ins = prog[i];
        ಪೂರ್ಣಾಙ್ಕ as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < 3; k++)
            ಯದಿ (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, 16> freeBufs(ntemps);
    ಪೂರ್ಣಾಙ್ಕ nfree = 0, nbuf = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninsn; i++)
    {
        ಧ್ರುವ TExpr::Insn& ins = prog[i];
        ಪೂರ್ಣಾಙ್ಕ rt = tempOfSlot[ins.result];
        ಯದಿ (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        ಪೂರ್ಣಾಙ್ಕ as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < 3; k++)
        {
            ಪೂರ್ಣಾಙ್ಕ t = tempOfSlot[as[k]];
            ಯದಿ (t >= 0 && lastUse[t] == i)
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
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 1; s < nslots; s++)
        ಯದಿ (arginfo[s].kind == TEMP)
        {
            ಪೂರ್ಣಾಙ್ಕ b = bufferOfTemp[arginfo[s].index], e = (ಪೂರ್ಣಾಙ್ಕ)CV_ELEM_SIZE1(arginfo[s].depth);
            ಯದಿ (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
ಸ್ಥಿರ ಶೂನ್ಯ matLogical(ಧ್ರುವ Mat& m, MatShape& shp, EwSteps& step, ಪೂರ್ಣಾಙ್ಕ& esz1)
{
    esz1 = (ಪೂರ್ಣಾಙ್ಕ)m.elemSize1();
    ಪೂರ್ಣಾಙ್ಕ nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
ಸ್ಥಿರ ತಾರ್ಕಿಕ broadcastShape(ಧ್ರುವ MatShape* shps, ಪೂರ್ಣಾಙ್ಕ K, MatShape& out)
{
    ಪೂರ್ಣಾಙ್ಕ nd = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
    {
        ಧ್ರುವ MatShape& s = shps[k];
        ಪೂರ್ಣಾಙ್ಕ off = nd - s.dims;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < s.dims; i++)
        {
            ಪೂರ್ಣಾಙ್ಕ d = s[i], &o = out[off + i];
            ಯದಿ (o == 1) o = d;
            ಅನ್ಯಥಾ ಯದಿ (d != 1 && d != o) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opCost(ತುದಿ op)
{
    ವಿಕಲ್ಪ (op)
    {
    ಪಕ್ಷ OP_ADD: ಪಕ್ಷ OP_SUB: ಪಕ್ಷ OP_MUL: ಪಕ್ಷ OP_MIN: ಪಕ್ಷ OP_MAX:
    ಪಕ್ಷ OP_ABSDIFF: ಪಕ್ಷ OP_AND: ಪಕ್ಷ OP_OR: ಪಕ್ಷ OP_XOR: ಪಕ್ಷ OP_NOT:
    ಪಕ್ಷ OP_NEG: ಪಕ್ಷ OP_ABS: ಪಕ್ಷ OP_CAST: ಪಕ್ಷ OP_RELU: ಪಕ್ಷ OP_SELECT:
    ಪಕ್ಷ OP_CMP_EQ: ಪಕ್ಷ OP_CMP_NE: ಪಕ್ಷ OP_CMP_LT:
    ಪಕ್ಷ OP_CMP_LE: ಪಕ್ಷ OP_CMP_GT: ಪಕ್ಷ OP_CMP_GE: ಪ್ರತ್ಯಾಗಮ 1;
    ಪಕ್ಷ OP_DIV: ಪಕ್ಷ OP_SQRT: ಪಕ್ಷ OP_HYPOT: ಪಕ್ಷ OP_CONVERT_SCALE: ಪ್ರತ್ಯಾಗಮ 3;
    ಪಕ್ಷ OP_SIN: ಪಕ್ಷ OP_COS: ಪಕ್ಷ OP_TANH: ಪಕ್ಷ OP_ERF: ಪಕ್ಷ OP_ATAN2:
    ಪಕ್ಷ OP_EXP: ಪಕ್ಷ OP_LOG: ಪಕ್ಷ OP_POW:           ಪ್ರತ್ಯಾಗಮ 6;
    ಸ್ವಭಾವ:                                         ಪ್ರತ್ಯಾಗಮ 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
ಶೂನ್ಯ TExpr::clear()
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
ಶೂನ್ಯ TExpr::dump(std::ostream& os) ಧ್ರುವ
{
    auto dn = [](ಪೂರ್ಣಾಙ್ಕ d) -> ಧ್ರುವ ವರ್ಣ* {
        ವಿಕಲ್ಪ (d) {
        ಪಕ್ಷ EW_DEPTH_NONE: ಪ್ರತ್ಯಾಗಮ "flex";
        ಪಕ್ಷ CV_8U:  ಪ್ರತ್ಯಾಗಮ "u8";  ಪಕ್ಷ CV_8S:  ಪ್ರತ್ಯಾಗಮ "s8";
        ಪಕ್ಷ CV_16U: ಪ್ರತ್ಯಾಗಮ "u16"; ಪಕ್ಷ CV_16S: ಪ್ರತ್ಯಾಗಮ "s16";
        ಪಕ್ಷ CV_32U: ಪ್ರತ್ಯಾಗಮ "u32"; ಪಕ್ಷ CV_32S: ಪ್ರತ್ಯಾಗಮ "s32";
        ಪಕ್ಷ CV_64U: ಪ್ರತ್ಯಾಗಮ "u64"; ಪಕ್ಷ CV_64S: ಪ್ರತ್ಯಾಗಮ "s64";
        ಪಕ್ಷ CV_16F: ಪ್ರತ್ಯಾಗಮ "f16"; ಪಕ್ಷ CV_16BF:ಪ್ರತ್ಯಾಗಮ "bf16";
        ಪಕ್ಷ CV_32F: ಪ್ರತ್ಯಾಗಮ "f32"; ಪಕ್ಷ CV_64F: ಪ್ರತ್ಯಾಗಮ "f64";
        ಪಕ್ಷ CV_Bool:ಪ್ರತ್ಯಾಗಮ "bool";
        ಸ್ವಭಾವ:     ಪ್ರತ್ಯಾಗಮ "?"; }
    };
    auto kn = [](ArgKind k) -> ಧ್ರುವ ವರ್ಣ* {
        ವಿಕಲ್ಪ (k) { ಪಕ್ಷ NONE: ಪ್ರತ್ಯಾಗಮ "none"; ಪಕ್ಷ ಒಳಹರಿವು: ಪ್ರತ್ಯಾಗಮ "in"; ಪಕ್ಷ CONST: ಪ್ರತ್ಯಾಗಮ "const";
                     ಪಕ್ಷ TEMP: ಪ್ರತ್ಯಾಗಮ "temp"; ಪಕ್ಷ OUTPUT: ಪ್ರತ್ಯಾಗಮ "out"; } ಪ್ರತ್ಯಾಗಮ "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (ಪೂರ್ಣಾಙ್ಕ)prog.size() << "\n";
    os << "  slots:\n";
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 0; s < (ಪೂರ್ಣಾಙ್ಕ)arginfo.size(); s++) {
        ಧ್ರುವ Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        ಯದಿ (a.kind != NONE) os << " " << dn(a.depth);
        ಯದಿ (a.kind == ಒಳಹರಿವು || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        ಯದಿ (a.kind == CONST) {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = std::max(1, a.channels);
            AutoBuffer<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v(cn);
            ಧ್ರುವ uchar* src = (ಧ್ರುವ uchar*)(constbuf.data() + a.constofs);
            ಯದಿ (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * ಪರಿಮಾಣ(ದ್ವಿಗುಣ));
            ಅನ್ಯಥಾ getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)prog.size(); i++) {
        ಧ್ರುವ Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        ಯದಿ (ins.arg1) os << ", " << ins.arg1;
        ಯದಿ (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        ಯದಿ (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        ಯದಿ (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        ಯದಿ (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
ಪೂರ್ಣಾಙ್ಕ TExpr::add_ಒಳಹರಿವು(ಪೂರ್ಣಾಙ್ಕ depth)
{
    Arg a; a.kind = ಒಳಹರಿವು; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)arginfo.size() - 1;
}

ಪೂರ್ಣಾಙ್ಕ TExpr::addOutput(ಪೂರ್ಣಾಙ್ಕ depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)arginfo.size() - 1;
}

ಪೂರ್ಣಾಙ್ಕ TExpr::addTemp(ಪೂರ್ಣಾಙ್ಕ depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
ಪೂರ್ಣಾಙ್ಕ TExpr::addConst(ಪೂರ್ಣಾಙ್ಕ depth, ಧ್ರುವ Scalar& v, ಪೂರ್ಣಾಙ್ಕ channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
ಪೂರ್ಣಾಙ್ಕ TExpr::addConst(ಪೂರ್ಣಾಙ್ಕ depth, ಪೂರ್ಣಾಙ್ಕ srcdepth, ಧ್ರುವ ಶೂನ್ಯ* data, ಪೂರ್ಣಾಙ್ಕ channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
ಪೂರ್ಣಾಙ್ಕ TExpr::typedConstFrom(ಪೂರ್ಣಾಙ್ಕ srcSlot, ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಧ್ರುವ Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
ಪೂರ್ಣಾಙ್ಕ TExpr::addInsn(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ a0, ಪೂರ್ಣಾಙ್ಕ a1, ಪೂರ್ಣಾಙ್ಕ a2, ಪೂರ್ಣಾಙ್ಕ result, ಧ್ರುವ TKernel& kernel, ಧ್ರುವ Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
ಪೂರ್ಣಾಙ್ಕ TExpr::addInsn(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ a0, ಪೂರ್ಣಾಙ್ಕ a1, ಪೂರ್ಣಾಙ್ಕ a2, ಪೂರ್ಣಾಙ್ಕ result, ಧ್ರುವ Scalar& params)
{
    ಪೂರ್ಣಾಙ್ಕ d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    ಪೂರ್ಣಾಙ್ಕ d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    ಪೂರ್ಣಾಙ್ಕ d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ಪ್ರತ್ಯಾಗಮ addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
ಪೂರ್ಣಾಙ್ಕ TExpr::maybeAddCast(ಪೂರ್ಣಾಙ್ಕ arg, ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಯದಿ (arginfo[arg].depth == depth) ಪ್ರತ್ಯಾಗಮ arg;
    ಪೂರ್ಣಾಙ್ಕ t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    ಪ್ರತ್ಯಾಗಮ t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ runInsn(ಧ್ರುವ TExpr::Insn& ins,
                           ಧ್ರುವ ಶೂನ್ಯ* p0, size_t y0, size_t x0,
                           ಧ್ರುವ ಶೂನ್ಯ* p1, size_t y1, size_t x1,
                           ಧ್ರುವ ಶೂನ್ಯ* p2, size_t y2, size_t x2,
                           ಶೂನ್ಯ* pr, size_t yr, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪೂರ್ಣಾಙ್ಕ code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

ಸಂರಚನಾ EwBody {
    ಧ್ರುವ TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* slotMap;
    ಧ್ರುವ signed ವರ್ಣ* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    ಪೂರ್ಣಾಙ್ಕ ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

ಶೂನ್ಯ TExpr::outputShape(ಧ್ರುವ Mat* ಧ್ರುವ* inputs, MatShape& spatial, ಪೂರ್ಣಾಙ್ಕ& channels) ಧ್ರುವ
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    ತಾರ್ಕಿಕ sameShape = ninputs >= 1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; sameShape && i < ninputs; i++)
    {
        ಧ್ರುವ Mat& a = *inputs[i];
        ಯದಿ (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = ಅಸತ್ಯ;
        ಅನ್ಯಥಾ ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ d = 0; d < a.dims; d++) ಯದಿ (a.size[d] != inputs[0]->size[d]) { sameShape = ಅಸತ್ಯ; ಭಙ್ಗ; }
    }
    ಯದಿ (sameShape) { spatial = inputs[0]->size; channels = rcn0; ಪ್ರತ್ಯಾಗಮ; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; ಪೂರ್ಣಾಙ್ಕ esz1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ndFull = (ಪೂರ್ಣಾಙ್ಕ)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ಶೂನ್ಯ TExpr::outputShape(ಧ್ರುವ Mat* inputs, MatShape& spatial, ಪೂರ್ಣಾಙ್ಕ& channels) ಧ್ರುವ
{
    AutoBuffer<ಧ್ರುವ Mat*, 16> ptrs(std::max(ninputs, 1));
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ LOCAL_HDRS = 3;
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ LOCAL_CONSTS = 4;
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ LOCAL_OPS = 16;

ಶೂನ್ಯ TExpr::exec(ಧ್ರುವ Mat* ಧ್ರುವ* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nslots = (ಪೂರ್ಣಾಙ್ಕ)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    ಯದಿ (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        ಧ್ರುವ Mat& r = *inputs[0];
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ rcn = r.channels();
        ತಾರ್ಕಿಕ ok = r.isContinuous();
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; ok && i < ninputs; i++)
        {
            ಧ್ರುವ Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 1; ok && s < nslots; s++)
            ಯದಿ (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        ಧ್ರುವ size_t total = ok ? r.total() * rcn : 0;
        ಯದಿ (ok && total == 0)
        {
            ಧ್ರುವ MatShape rshape = r.size;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 1; s < nslots; s++)
                ಯದಿ (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ಪ್ರತ್ಯಾಗಮ;
        }
        ಯದಿ (ok && total <= EW_FASTPATH_MAX)
        {
            ಧ್ರುವ MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 1; s < nslots; s++)
                ಯದಿ (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ninsn = (ಪೂರ್ಣಾಙ್ಕ)prog.size();

            ಯದಿ (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](ಪೂರ್ಣಾಙ್ಕ s) -> ಶೂನ್ಯ* {
                    ಯದಿ (s <= 0) ಪ್ರತ್ಯಾಗಮ nullptr;
                    ಧ್ರುವ Arg& ai = arginfo[s];
                    ಯದಿ (ai.kind == ಒಳಹರಿವು)  ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ*)inputs[ai.index]->data;
                    ಯದಿ (ai.kind == OUTPUT) ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ*)outputs[ai.index].data;
                    ಪ್ರತ್ಯಾಗಮ nullptr;   // NONE
                };
                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ w = (ಪೂರ್ಣಾಙ್ಕ)total;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ n = 0; n < ninsn; n++)
                {
                    ಧ್ರುವ Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                ಪ್ರತ್ಯಾಗಮ;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ totalEsz = bufEszPrefix[nbuffers];
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ wf0 = std::min((ಪೂರ್ಣಾಙ್ಕ)total, capElems);

            ಧ್ರುವ size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/ಪರಿಮಾಣ(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)ಪರಿಮಾಣ(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x0 = 0; x0 < (ಪೂರ್ಣಾಙ್ಕ)total; x0 += wf0)
            {
                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ wf = std::min(wf0, (ಪೂರ್ಣಾಙ್ಕ)total - x0);
                auto ptrOf = [&](ಪೂರ್ಣಾಙ್ಕ s) -> ಶೂನ್ಯ* {
                    ಯದಿ (s <= 0) ಪ್ರತ್ಯಾಗಮ nullptr;
                    ಧ್ರುವ Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    ಯದಿ (ai.kind == ಒಳಹರಿವು)  ಪ್ರತ್ಯಾಗಮ (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    ಯದಿ (ai.kind == OUTPUT) ಪ್ರತ್ಯಾಗಮ (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    ಯದಿ (ai.kind != TEMP)   ಪ್ರತ್ಯಾಗಮ nullptr;                                    // NONE: moved-from
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    ಪ್ರತ್ಯಾಗಮ scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ n = 0; n < ninsn; n++)
                {
                    ಧ್ರುವ Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            ಪ್ರತ್ಯಾಗಮ;
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
    ಪೂರ್ಣಾಙ್ಕ rchannels;
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
    ತಾರ್ಕಿಕ inplace = ಅಸತ್ಯ;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninputs && !inplace; i++)
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < noutputs; j++)
            ಯದಿ (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = ಸತ್ಯ; ಭಙ್ಗ; }
    ಪೂರ್ಣಾಙ್ಕ nhdrs = inplace ? ninputs : 0;

    AutoBuffer<ಧ್ರುವ Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<ಧ್ರುವ Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed ವರ್ಣ, LOCAL_OPS> slotKind(nslots);
    ಪೂರ್ಣಾಙ್ಕ narr = 0, nc = 0;
    ಯದಿ (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed ವರ್ಣ)TExpr::NONE;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 1; s < nslots; s++)
    {
        ಧ್ರುವ TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed ವರ್ಣ)ai.kind;
        ಯದಿ (ai.kind == TExpr::ಒಳಹರಿವು) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        ಅನ್ಯಥಾ ಯದಿ (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        ಅನ್ಯಥಾ ಯದಿ (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed ವರ್ಣ)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        ಅನ್ಯಥಾ ಯದಿ (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (ಶೂನ್ಯ*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed ವರ್ಣ)TExpr::ಒಳಹರಿವು;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        ಅನ್ಯಥಾ ಯದಿ (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
        }
        ಅನ್ಯಥಾ {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ninsn = (ಪೂರ್ಣಾಙ್ಕ)prog.size();
    long long costPerElem = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ d = 0; d < (ಪೂರ್ಣಾಙ್ಕ)spatial.size(); d++) otot *= spatial[d];
    ಯದಿ (otot == 0)
        ಪ್ರತ್ಯಾಗಮ;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ಧ್ರುವ ದ್ವಿಗುಣ T = (ದ್ವಿಗುಣ)std::max(get_ಅಂಕೆ_Threads(), 1);
    ಧ್ರುವ ದ್ವಿಗುಣ nstripes = std::min(
        (ದ್ವಿಗುಣ)otot * (ದ್ವಿಗುಣ)std::max<long long>(costPerElem, 1) * (1./ (ದ್ವಿಗುಣ)(1 << 16)),
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

    broadcastOp(arr.data(), narr, [&](ಧ್ರುವ BrTile& tile)
    {
        EwBody& bc = body;
        ಧ್ರುವ TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ಧ್ರುವ smap  = bc.slotMap;
        ಧ್ರುವ signed ವರ್ಣ* ಧ್ರುವ skind = bc.slotKind;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nin = bc.ninsn, nsl = bc.nslots;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ಧ್ರುವ eszPrefix = bc.bufEszPrefix;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ bw = std::min(w, bc.capElems);
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ಧ್ರುವ size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/ಪರಿಮಾಣ(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)ಪರಿಮಾಣ(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y0 = 0; y0 < h; y0 += bh)
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ hf = std::min(bh, h - y0);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x0 = 0; x0 < w; x0 += bw)
            {
                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ wf = std::min(bw, w - x0);
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    ಯದಿ (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    ಅನ್ಯಥಾ                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ಧ್ರುವ BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ n = 0; n < nin; n++)
                {
                    ಧ್ರುವ TExpr::Insn& ins = insns[n];
                    ಧ್ರುವ BrSlice& a0 = args[ins.arg0]; ಧ್ರುವ BrSlice& a1 = args[ins.arg1];
                    ಧ್ರುವ BrSlice& a2 = args[ins.arg2]; ಧ್ರುವ BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (ಶೂನ್ಯ*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, ಸತ್ಯ, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ಶೂನ್ಯ TExpr::exec(ಧ್ರುವ Mat* inputs, Mat* outputs)
{
    AutoBuffer<ಧ್ರುವ Mat*, 16> ptrs(std::max(ninputs, 1));
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
ಶೂನ್ಯ make_ದ್ವಿಮಾನ_Arith_Program(TExpr& p, ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ depth0, ಪೂರ್ಣಾಙ್ಕ depth1, ಪೂರ್ಣಾಙ್ಕ rdepth,
                            ಪೂರ್ಣಾಙ್ಕ maskDepth, ದ್ವಿಗುಣ scale)
{
    p.clear();
    ಯದಿ (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        ಯದಿ (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        ಅನ್ಯಥಾ
        {
            rdepth = promoteArith(depth0, depth1);
            ಯದಿ (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ಧ್ರುವ ತಾರ್ಕಿಕ masked = maskDepth != EW_DEPTH_NONE;
    ಪೂರ್ಣಾಙ್ಕ sIn0  = p.add_ಒಳಹರಿವು(depth0);
    ಪೂರ್ಣಾಙ್ಕ sIn1  = p.add_ಒಳಹರಿವು(depth1);
    ಪೂರ್ಣಾಙ್ಕ sMask = masked ? p.add_ಒಳಹರಿವು(maskDepth) : 0;
    ಪೂರ್ಣಾಙ್ಕ sOut  = p.addOutput(rdepth);

    ಪೂರ್ಣಾಙ್ಕ r = p.emit_ದ್ವಿಮಾನ(op, sIn0, sIn1, rdepth, Scalar(scale));
    ಯದಿ (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    ಅನ್ಯಥಾ
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
ಶೂನ್ಯ makeAddWeightedProgram(TExpr& p, ಪೂರ್ಣಾಙ್ಕ depth0, ಪೂರ್ಣಾಙ್ಕ depth1, ಪೂರ್ಣಾಙ್ಕ rdepth,
                            ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ gamma)
{
    p.clear();
    ಪೂರ್ಣಾಙ್ಕ sA   = p.add_ಒಳಹರಿವು(depth0);
    ಪೂರ್ಣಾಙ್ಕ sB   = p.add_ಒಳಹರಿವು(depth1);
    ಪೂರ್ಣಾಙ್ಕ sOut = p.addOutput(rdepth);
    ಪೂರ್ಣಾಙ್ಕ r = p.emit_ದ್ವಿಮಾನ(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

ನಾಮಾಕಾಶ {

// --- tokens ------------------------------------------------------------------------------
ಗಣನಾ TokType { T_ಅಂಕೆ, T_ಒಳಹರಿವು, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

ಸಂರಚನಾ Token
{
    TokType type = T_END;
    ದ್ವಿಗುಣ num = 0;
    ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
ಸಂರಚನಾ Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    ಸ್ಥಿರ ತಾರ್ಕಿಕ isIdentStart(ವರ್ಣ c) { ಪ್ರತ್ಯಾಗಮ std::isalpha((unsigned ವರ್ಣ)c) || c == '_'; }
    ಸ್ಥಿರ ತಾರ್ಕಿಕ isIdentChar(ವರ್ಣ c)  { ಪ್ರತ್ಯಾಗಮ std::isalnum((unsigned ವರ್ಣ)c) || c == '_'; }

    Token next()
    {
        ಯಾವತ್ (pos < s.size() && std::isspace((unsigned ವರ್ಣ)s[pos])) pos++;
        Token t;
        ಯದಿ (pos >= s.size()) { t.type = T_END; ಪ್ರತ್ಯಾಗಮ t; }

        ವರ್ಣ c = s[pos];

        ಯದಿ (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            ಯಾವತ್ (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_ಒಳಹರಿವು;
            t.ಒಳಹರಿವು_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            ಪ್ರತ್ಯಾಗಮ t;
        }
        ಯದಿ (std::isdigit((unsigned ವರ್ಣ)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned ವರ್ಣ)s[pos + 1])))
        {
            ವರ್ಣ* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_ಅಂಕೆ;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            ಪ್ರತ್ಯಾಗಮ t;
        }
        ಯದಿ (isIdentStart(c))
        {
            size_t start = pos;
            ಯಾವತ್ (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            ಪ್ರತ್ಯಾಗಮ t;
        }
        ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ '(': pos++; t.type = T_LPAREN; ಪ್ರತ್ಯಾಗಮ t;
        ಪಕ್ಷ ')': pos++; t.type = T_RPAREN; ಪ್ರತ್ಯಾಗಮ t;
        ಪಕ್ಷ ',': pos++; t.type = T_COMMA;  ಪ್ರತ್ಯಾಗಮ t;
        ಪಕ್ಷ ';': pos++; t.type = T_SEMI;   ಪ್ರತ್ಯಾಗಮ t;
        }
        // multi/!single-char operators
        auto two = [&](ಧ್ರುವ ವರ್ಣ* op) {
            ಪ್ರತ್ಯಾಗಮ pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        ಯದಿ (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; ಪ್ರತ್ಯಾಗಮ t; }
        ಯದಿ (c == '=') { pos++; t.type = T_ASSIGN; ಪ್ರತ್ಯಾಗಮ t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        ಪ್ರತ್ಯಾಗಮ t;
    }
};

// --- operator / function tables ----------------------------------------------------------
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ binPrec(ಧ್ರುವ std::string& op)
{
    ಯದಿ (op == "**") ಪ್ರತ್ಯಾಗಮ 8;              // power binds tighter than '*'; RIGHT-associative
    ಯದಿ (op == "*" || op == "/") ಪ್ರತ್ಯಾಗಮ 7;
    ಯದಿ (op == "+" || op == "-") ಪ್ರತ್ಯಾಗಮ 6;
    ಯದಿ (op == "<" || op == "<=" || op == ">" || op == ">=") ಪ್ರತ್ಯಾಗಮ 5;
    ಯದಿ (op == "==" || op == "!=") ಪ್ರತ್ಯಾಗಮ 4;
    ಯದಿ (op == "&") ಪ್ರತ್ಯಾಗಮ 3;
    ಯದಿ (op == "^") ಪ್ರತ್ಯಾಗಮ 2;
    ಯದಿ (op == "|") ಪ್ರತ್ಯಾಗಮ 1;
    ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ತುದಿ binOp(ಧ್ರುವ std::string& op)
{
    ಯದಿ (op == "**") ಪ್ರತ್ಯಾಗಮ OP_POW;
    ಯದಿ (op == "+")  ಪ್ರತ್ಯಾಗಮ OP_ADD;
    ಯದಿ (op == "-")  ಪ್ರತ್ಯಾಗಮ OP_SUB;
    ಯದಿ (op == "*")  ಪ್ರತ್ಯಾಗಮ OP_MUL;
    ಯದಿ (op == "/")  ಪ್ರತ್ಯಾಗಮ OP_DIV;
    ಯದಿ (op == "<")  ಪ್ರತ್ಯಾಗಮ OP_CMP_LT;
    ಯದಿ (op == "<=") ಪ್ರತ್ಯಾಗಮ OP_CMP_LE;
    ಯದಿ (op == ">")  ಪ್ರತ್ಯಾಗಮ OP_CMP_GT;
    ಯದಿ (op == ">=") ಪ್ರತ್ಯಾಗಮ OP_CMP_GE;
    ಯದಿ (op == "==") ಪ್ರತ್ಯಾಗಮ OP_CMP_EQ;
    ಯದಿ (op == "!=") ಪ್ರತ್ಯಾಗಮ OP_CMP_NE;
    ಯದಿ (op == "&")  ಪ್ರತ್ಯಾಗಮ OP_AND;
    ಯದಿ (op == "|")  ಪ್ರತ್ಯಾಗಮ OP_OR;
    ಯದಿ (op == "^")  ಪ್ರತ್ಯಾಗಮ OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ typeDepth(ಧ್ರುವ std::string& name)
{
    ಯದಿ (name == "float")    ಪ್ರತ್ಯಾಗಮ CV_32F;
    ಯದಿ (name == "double")   ಪ್ರತ್ಯಾಗಮ CV_64F;
    ಯದಿ (name == "half" || name == "float16") ಪ್ರತ್ಯಾಗಮ CV_16F;
    ಯದಿ (name == "bfloat16") ಪ್ರತ್ಯಾಗಮ CV_16BF;
    ಯದಿ (name == "uint8")    ಪ್ರತ್ಯಾಗಮ CV_8U;
    ಯದಿ (name == "int8")     ಪ್ರತ್ಯಾಗಮ CV_8S;
    ಯದಿ (name == "uint16")   ಪ್ರತ್ಯಾಗಮ CV_16U;
    ಯದಿ (name == "int16")    ಪ್ರತ್ಯಾಗಮ CV_16S;
    ಯದಿ (name == "uint32")   ಪ್ರತ್ಯಾಗಮ CV_32U;
    ಯದಿ (name == "int32")    ಪ್ರತ್ಯಾಗಮ CV_32S;
    ಯದಿ (name == "uint64")   ಪ್ರತ್ಯಾಗಮ CV_64U;
    ಯದಿ (name == "int64")    ಪ್ರತ್ಯಾಗಮ CV_64S;
    ಪ್ರತ್ಯಾಗಮ -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
ಸ್ಥಿರ ತುದಿ fnOp(ಧ್ರುವ std::string& name, ಪೂರ್ಣಾಙ್ಕ& arity)
{
    ಸಂರಚನಾ E { ಧ್ರುವ ವರ್ಣ* n; ತುದಿ op; };
    ಸ್ಥಿರ ಧ್ರುವ E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    ಸ್ಥಿರ ಧ್ರುವ E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    ಸ್ಥಿರ ಧ್ರುವ E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    ಪ್ರತಿ (ಧ್ರುವ E& e : unary)  ಯದಿ (name == e.n) { arity = 1; ಪ್ರತ್ಯಾಗಮ e.op; }
    ಪ್ರತಿ (ಧ್ರುವ E& e : binary) ಯದಿ (name == e.n) { arity = 2; ಪ್ರತ್ಯಾಗಮ e.op; }
    ಪ್ರತಿ (ಧ್ರುವ E& e : tern)   ಯದಿ (name == e.n) { arity = 3; ಪ್ರತ್ಯಾಗಮ e.op; }
    arity = 0; ಪ್ರತ್ಯಾಗಮ OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
ಸಂರಚನಾ Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ಒಳಹರಿವು_Slot;                 // slot id of each external input (precreated)
    ಪೂರ್ಣಾಙ್ಕ ninputs;
    std::map<std::string, ಪೂರ್ಣಾಙ್ಕ> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& ಅಭಿವ್ಯಕ್ತಿ, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* islot, ಪೂರ್ಣಾಙ್ಕ nin)
        : lex(src), e(ಅಭಿವ್ಯಕ್ತಿ), ಒಳಹರಿವು_Slot(islot), ninputs(nin) { cur = lex.next(); }

    ಶೂನ್ಯ advance() { cur = lex.next(); }
    ತಾರ್ಕಿಕ isOp(ಧ್ರುವ ವರ್ಣ* op) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ cur.type == T_OP && cur.text == op; }

    ಶೂನ್ಯ expect(TokType t, ಧ್ರುವ ವರ್ಣ* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    ತಾರ್ಕಿಕ isFlexConst(ಪೂರ್ಣಾಙ್ಕ slot) ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    ಪೂರ್ಣಾಙ್ಕ parsePrimary()
    {
        ಯದಿ (cur.type == T_ಅಂಕೆ)   { ಪೂರ್ಣಾಙ್ಕ s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); ಪ್ರತ್ಯಾಗಮ s; }
        ಯದಿ (cur.type == T_ಒಳಹರಿವು) { ಪೂರ್ಣಾಙ್ಕ idx = cur.ಒಳಹರಿವು_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   ಪ್ರತ್ಯಾಗಮ ಒಳಹರಿವು_Slot[idx]; }
        ಯದಿ (cur.type == T_LPAREN){ advance(); ಪೂರ್ಣಾಙ್ಕ x = parseTernary(); expect(T_RPAREN, "expected ')'"); ಪ್ರತ್ಯಾಗಮ x; }
        ಯದಿ (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            ಯದಿ (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                ಪ್ರತ್ಯಾಗಮ it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<ಪೂರ್ಣಾಙ್ಕ, 8> args = {}; ಪೂರ್ಣಾಙ್ಕ nargs = 0;
            ಯದಿ (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                ಯಾವತ್ (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (ಪೂರ್ಣಾಙ್ಕ)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            ಪೂರ್ಣಾಙ್ಕ td = typeDepth(name);
            ಯದಿ (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                ಪೂರ್ಣಾಙ್ಕ s = args[0];
                ಪ್ರತ್ಯಾಗಮ isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            ಪೂರ್ಣಾಙ್ಕ arity = 0; ತುದಿ op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            ಯದಿ (arity == 1) ಪ್ರತ್ಯಾಗಮ e.emitUnary(op, args[0]);
            ಯದಿ (arity == 2) ಪ್ರತ್ಯಾಗಮ e.emit_ದ್ವಿಮಾನ(op, args[0], args[1]);
            ಪ್ರತ್ಯಾಗಮ e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    ಪೂರ್ಣಾಙ್ಕ parseUnary()
    {
        ಯದಿ (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            ಪೂರ್ಣಾಙ್ಕ operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            ಯದಿ (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v; ಪೂರ್ಣಾಙ್ಕ cn = constDoubles(e, operand, v);
                Scalar neg; ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                ಪ್ರತ್ಯಾಗಮ e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            ಪ್ರತ್ಯಾಗಮ e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        ಪ್ರತ್ಯಾಗಮ parsePrimary();
    }

    ಪೂರ್ಣಾಙ್ಕ parse_ಅಭಿವ್ಯಕ್ತಿ(ಪೂರ್ಣಾಙ್ಕ minPrec)
    {
        ಪೂರ್ಣಾಙ್ಕ ಎಡ = parseUnary();
        ಯಾವತ್ (cur.type == T_OP)
        {
            ಪೂರ್ಣಾಙ್ಕ p = binPrec(cur.text);
            ಯದಿ (p < minPrec) ಭಙ್ಗ;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            ಪೂರ್ಣಾಙ್ಕ ಬಲ = parse_ಅಭಿವ್ಯಕ್ತಿ(op == "**" ? p : p + 1);
            ಎಡ = e.emit_ದ್ವಿಮಾನ(binOp(op), ಎಡ, ಬಲ);
        }
        ಪ್ರತ್ಯಾಗಮ ಎಡ;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    ಪೂರ್ಣಾಙ್ಕ parseTernary()
    {
        ಪೂರ್ಣಾಙ್ಕ cond = parse_ಅಭಿವ್ಯಕ್ತಿ(0);
        ಯದಿ (!isOp("?")) ಪ್ರತ್ಯಾಗಮ cond;
        advance();
        ಪೂರ್ಣಾಙ್ಕ thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        ಪೂರ್ಣಾಙ್ಕ elsev = parseTernary();               // right-associative chain
        ಪ್ರತ್ಯಾಗಮ e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    ಶೂನ್ಯ parseResult()
    {
        ಯದಿ (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); ಪೂರ್ಣಾಙ್ಕ nTemp = e.ntemps;
            advance();
            ಪೂರ್ಣಾಙ್ಕ e0 = parseTernary();
            ಯದಿ (cur.type == T_COMMA)
            {
                e.output(e0);
                ಯಾವತ್ (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                ಪ್ರತ್ಯಾಗಮ;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    ಶೂನ್ಯ parse()
    {
        ಯಾವತ್ (ಸತ್ಯ)
        {
            ಯದಿ (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                ಯದಿ (cur.type == T_ASSIGN)
                {
                    advance();
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = ಸತ್ಯ;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    ಅನುವರ್ತ;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            ಯದಿ (cur.type == T_SEMI) advance();
            ಭಙ್ಗ;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

ನಾಮಾಕಾಶ cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
ಶೂನ್ಯ texpr(ಧ್ರುವ std::string& ಅಭಿವ್ಯಕ್ತಿ, ಒಳಹರಿವು_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using ನಾಮಾಕಾಶ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT);
    ಧ್ರುವ std::vector<Mat>& inps = *(ಧ್ರುವ std::vector<Mat>*)_inputs.getObj();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ninputs = (ಪೂರ್ಣಾಙ್ಕ)inps.size();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<ಪೂರ್ಣಾಙ್ಕ> islot(std::max(ninputs, 1));
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ninputs; i++) islot[i] = e.add_ಒಳಹರಿವು(inps[i].depth());
    Parser(ಅಭಿವ್ಯಕ್ತಿ, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    ಯದಿ (kind == ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } ಅನ್ಯಥಾ {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
