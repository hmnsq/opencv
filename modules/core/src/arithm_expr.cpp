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

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* opName(ತುದಿ op)
{
    ಆಯ್ಕೆ (op)
    {
    ಸ್ಥಿತಿ OP_NOP:           ಹಿಂತಿರುಗಿ "nop";
    ಸ್ಥಿತಿ OP_NEG:           ಹಿಂತಿರುಗಿ "neg";
    ಸ್ಥಿತಿ OP_ABS:           ಹಿಂತಿರುಗಿ "abs";
    ಸ್ಥಿತಿ OP_NOT:           ಹಿಂತಿರುಗಿ "not";
    ಸ್ಥಿತಿ OP_SQRT:          ಹಿಂತಿರುಗಿ "sqrt";
    ಸ್ಥಿತಿ OP_EXP:           ಹಿಂತಿರುಗಿ "exp";
    ಸ್ಥಿತಿ OP_LOG:           ಹಿಂತಿರುಗಿ "log";
    ಸ್ಥಿತಿ OP_SIN:           ಹಿಂತಿರುಗಿ "sin";
    ಸ್ಥಿತಿ OP_COS:           ಹಿಂತಿರುಗಿ "cos";
    ಸ್ಥಿತಿ OP_TANH:          ಹಿಂತಿರುಗಿ "tanh";
    ಸ್ಥಿತಿ OP_ERF:           ಹಿಂತಿರುಗಿ "erf";
    ಸ್ಥಿತಿ OP_RELU:          ಹಿಂತಿರುಗಿ "relu";
    ಸ್ಥಿತಿ OP_CAST:          ಹಿಂತಿರುಗಿ "cast";
    ಸ್ಥಿತಿ OP_ಕೂಡಿಸು:           ಹಿಂತಿರುಗಿ "add";
    ಸ್ಥಿತಿ OP_SUB:           ಹಿಂತಿರುಗಿ "sub";
    ಸ್ಥಿತಿ OP_MUL:           ಹಿಂತಿರುಗಿ "mul";
    ಸ್ಥಿತಿ OP_DIV:           ಹಿಂತಿರುಗಿ "div";
    ಸ್ಥಿತಿ OP_POW:           ಹಿಂತಿರುಗಿ "pow";
    ಸ್ಥಿತಿ OP_MIN:           ಹಿಂತಿರುಗಿ "min";
    ಸ್ಥಿತಿ OP_MAX:           ಹಿಂತಿರುಗಿ "max";
    ಸ್ಥಿತಿ OP_ABSDIFF:       ಹಿಂತಿರುಗಿ "absdiff";
    ಸ್ಥಿತಿ OP_ADDW:          ಹಿಂತಿರುಗಿ "addWeighted";
    ಸ್ಥಿತಿ OP_HYPOT:         ಹಿಂತಿರುಗಿ "hypot";
    ಸ್ಥಿತಿ OP_ATAN2:         ಹಿಂತಿರುಗಿ "atan2";
    ಸ್ಥಿತಿ OP_AND:           ಹಿಂತಿರುಗಿ "and";
    ಸ್ಥಿತಿ OP_OR:            ಹಿಂತಿರುಗಿ "or";
    ಸ್ಥಿತಿ OP_XOR:           ಹಿಂತಿರುಗಿ "xor";
    ಸ್ಥಿತಿ OP_CMP_EQ:        ಹಿಂತಿರುಗಿ "cmp_eq";
    ಸ್ಥಿತಿ OP_CMP_NE:        ಹಿಂತಿರುಗಿ "cmp_ne";
    ಸ್ಥಿತಿ OP_CMP_LT:        ಹಿಂತಿರುಗಿ "cmp_lt";
    ಸ್ಥಿತಿ OP_CMP_LE:        ಹಿಂತಿರುಗಿ "cmp_le";
    ಸ್ಥಿತಿ OP_CMP_GT:        ಹಿಂತಿರುಗಿ "cmp_gt";
    ಸ್ಥಿತಿ OP_CMP_GE:        ಹಿಂತಿರುಗಿ "cmp_ge";
    ಸ್ಥಿತಿ OP_CLAMP:         ಹಿಂತಿರುಗಿ "clamp";
    ಸ್ಥಿತಿ OP_SELECT:        ಹಿಂತಿರುಗಿ "select";
    ಸ್ಥಿತಿ OP_CONVERT_SCALE: ಹಿಂತಿರುಗಿ "convert_scale";
    ಪೂರ್ವನಿಯೋಜಿತ:               ಹಿಂತಿರುಗಿ "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(ತುದಿ op)
{
    ಆಯ್ಕೆ (op)
    {
    ಸ್ಥಿತಿ OP_AND: ಸ್ಥಿತಿ OP_OR: ಸ್ಥಿತಿ OP_XOR: ಸ್ಥಿತಿ OP_NOT:
        ಹಿಂತಿರುಗಿ CAT_BITWISE;
    ಸ್ಥಿತಿ OP_CMP_EQ: ಸ್ಥಿತಿ OP_CMP_NE: ಸ್ಥಿತಿ OP_CMP_LT:
    ಸ್ಥಿತಿ OP_CMP_LE: ಸ್ಥಿತಿ OP_CMP_GT: ಸ್ಥಿತಿ OP_CMP_GE:
        ಹಿಂತಿರುಗಿ CAT_COMPARE;
    ಸ್ಥಿತಿ OP_SQRT: ಸ್ಥಿತಿ OP_EXP: ಸ್ಥಿತಿ OP_LOG:
    ಸ್ಥಿತಿ OP_SIN: ಸ್ಥಿತಿ OP_COS: ಸ್ಥಿತಿ OP_TANH: ಸ್ಥಿತಿ OP_ERF: ಸ್ಥಿತಿ OP_ATAN2: ಸ್ಥಿತಿ OP_RELU:
        ಹಿಂತಿರುಗಿ CAT_MATH;
    ಸ್ಥಿತಿ OP_CAST: ಸ್ಥಿತಿ OP_CONVERT_SCALE:
        ಹಿಂತಿರುಗಿ CAT_CAST;
    ಸ್ಥಿತಿ OP_SELECT:
        ಹಿಂತಿರುಗಿ CAT_SELECT;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
ಸ್ಥಿರ ತಾರ್ಕಿಕ is_Float_ಆಳ(ಪೂರ್ಣಾಂಕ d)
{
    ಹಿಂತಿರುಗಿ d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
ಸ್ಥಿರ ತಾರ್ಕಿಕ ಆಳ_Represents(ದ್ವಿಗುಣ v, ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಆದರೆ (is_Float_ಆಳ(ಆಳ)) ಹಿಂತಿರುಗಿ ನಿಜ;
    ಆದರೆ (v != std::floor(v)) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಆಯ್ಕೆ (ಆಳ)
    {
    ಸ್ಥಿತಿ CV_8U:  ಹಿಂತಿರುಗಿ v >= 0            && v <= 255;
    ಸ್ಥಿತಿ CV_8S:  ಹಿಂತಿರುಗಿ v >= -128         && v <= 127;
    ಸ್ಥಿತಿ CV_16U: ಹಿಂತಿರುಗಿ v >= 0            && v <= 65535;
    ಸ್ಥಿತಿ CV_16S: ಹಿಂತಿರುಗಿ v >= -32768       && v <= 32767;
    ಸ್ಥಿತಿ CV_32U: ಹಿಂತಿರುಗಿ v >= 0            && v <= 4294967295.0;
    ಸ್ಥಿತಿ CV_32S: ಹಿಂತಿರುಗಿ v >= -2147483648.0 && v <= 2147483647.0;
    ಸ್ಥಿತಿ CV_64U: ಹಿಂತಿರುಗಿ v >= 0            && v <= 18446744073709551615.0;
    ಸ್ಥಿತಿ CV_64S: ಹಿಂತಿರುಗಿ v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    ಪೂರ್ವನಿಯೋಜಿತ:     ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
ಸ್ಥಿರ ಶೂನ್ಯ intRange(ಪೂರ್ಣಾಂಕ ಆಳ, ದ್ವಿಗುಣ& lo, ದ್ವಿಗುಣ& hi)
{
    ಆಯ್ಕೆ (ಆಳ)
    {
    ಸ್ಥಿತಿ CV_8U:  lo = 0;                     hi = 255; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:  lo = -128;                  hi = 127; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U: lo = 0;                     hi = 65535; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S: lo = -32768;                hi = 32767; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U: lo = 0;                     hi = 4294967295.0; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S: lo = -2147483648.0;         hi = 2147483647.0; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U: lo = 0;                     hi = 18446744073709551615.0; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:     lo = 0;                     hi = 0; ನಿಲ್ಲಿಸು;
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
ಪೂರ್ಣಾಂಕ promoteArith(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr ಚಿಹ್ನೆರಹಿತ signlut = 0b1001111110100u;
    constexpr ಚಿಹ್ನೆರಹಿತ fltlut  = 0b1111000000u;

    ಆದರೆ (a == b) ಹಿಂತಿರುಗಿ a;

    ಪೂರ್ಣಾಂಕ wa = ಪೂರ್ಣಾಂಕ((leszlut >> (a+1)*3) & 7u), wb = ಪೂರ್ಣಾಂಕ((leszlut >> (b+1)*3) & 7u);
    ಪೂರ್ಣಾಂಕ fa = ಪೂರ್ಣಾಂಕ((fltlut >> (a+1)) & 1u),    fb = ಪೂರ್ಣಾಂಕ((fltlut >> (b+1)) & 1u);

    ಆದರೆ (fa + fb == 0)                          // both integer
    {
        ಪೂರ್ಣಾಂಕ sa = ಪೂರ್ಣಾಂಕ((signlut >> (a+1)) & 1u), sb = ಪೂರ್ಣಾಂಕ((signlut >> (b+1)) & 1u);
        ಆದರೆ (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            ಹಿಂತಿರುಗಿ wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        ಪೂರ್ಣಾಂಕ rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr ಪೂರ್ಣಾಂಕ ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        ಹಿಂತಿರುಗಿ (ilut >> rw*5) & CV_MAT_ಆಳ_ಮುಖವಾಡ;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    ಪೂರ್ಣಾಂಕ maxw = std::max(wa, wb);
    ಆದರೆ (maxw == 2 && fa + fb == 1)
        ಹಿಂತಿರುಗಿ fa*a + (1 - fa)*b;
    ಹಿಂತಿರುಗಿ CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
ಪೂರ್ಣಾಂಕ absdiff_ಫಲಿತಾಂಶ_ಆಳ(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಆಯ್ಕೆ (ಆಳ)
    {
    ಸ್ಥಿತಿ CV_8S:  ಹಿಂತಿರುಗಿ CV_8U;
    ಸ್ಥಿತಿ CV_16S: ಹಿಂತಿರುಗಿ CV_16U;
    ಸ್ಥಿತಿ CV_32S: ಹಿಂತಿರುಗಿ CV_32U;
    ಸ್ಥಿತಿ CV_64S: ಹಿಂತಿರುಗಿ CV_64U;
    ಪೂರ್ವನಿಯೋಜಿತ:     ಹಿಂತಿರುಗಿ ಆಳ;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ promote2(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b)
{
    ಆದರೆ (a == b) ಹಿಂತಿರುಗಿ a;

    constexpr ಚಿಹ್ನೆರಹಿತ lbits = 3, lmask = (1u << lbits) - 1u;
    ಸ್ಥಿರಾಂಕ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    ಚಿಹ್ನೆರಹಿತ pr_a = ಚಿಹ್ನೆರಹಿತ((typelut >> (a*lbits)) & lmask);
    ಚಿಹ್ನೆರಹಿತ pr_b = ಚಿಹ್ನೆರಹಿತ((typelut >> (b*lbits)) & lmask);
    ಚಿಹ್ನೆರಹಿತ max_pr = std::max(pr_a, pr_b);
    constexpr ಚಿಹ್ನೆರಹಿತ dbits = CV_CN_ಸರಿಸು, dmask = (1u << dbits) - 1u;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ಹಿಂತಿರುಗಿ ಪೂರ್ಣಾಂಕ((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ promoteBitwise(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b)
{
    ಆದರೆ (a == EW_ಆಳ_NONE) ಹಿಂತಿರುಗಿ b;
    ಆದರೆ (b == EW_ಆಳ_NONE) ಹಿಂತಿರುಗಿ a;
    ಆದರೆ (a == b) ಹಿಂತಿರುಗಿ a;
    ಹಿಂತಿರುಗಿ CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸುರಕ್ಷಿತ_Wide(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    constexpr ಚಿಹ್ನೆರಹಿತ lbits = 3, lmask = (1u << lbits) - 1u;
    ಸ್ಥಿರಾಂಕ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    ಚಿಹ್ನೆರಹಿತ pr_ಆಳ = ಚಿಹ್ನೆರಹಿತ((typelut >> (ಆಳ*lbits)) & lmask);
    constexpr ಚಿಹ್ನೆರಹಿತ dbits = CV_CN_ಸರಿಸು, dmask = (1u << dbits) - 1u;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ಹಿಂತಿರುಗಿ ಪೂರ್ಣಾಂಕ((ctypelut >> (pr_ಆಳ*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
ಎಣಿಕೆ { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
ಸ್ಥಿರ size_t appendConstBuf(Auto_ಬಫರ್<uint64_t, 16>& constbuf, ಪೂರ್ಣಾಂಕ srcdepth, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ channels)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = std::max(1, channels);
    ಸ್ಥಿರಾಂಕ size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ಸ್ಥಿರಾಂಕ size_t nu64 = (nbytes + ಗಾತ್ರ(uint64_t) - 1) / ಗಾತ್ರ(uint64_t);
    ಸ್ಥಿರಾಂಕ size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.ದತ್ತಾಂಶ() + ofs);
    ಆದರೆ (ದತ್ತಾಂಶ) ಸ್ಮರಣೆ_ನಕಲು(dst, ದತ್ತಾಂಶ, nbytes); ಇಲ್ಲದಿದ್ದರೆ memset(dst, 0, nbytes);
    ಹಿಂತಿರುಗಿ ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ constDoubles(ಸ್ಥಿರಾಂಕ TExpr& e, ಪೂರ್ಣಾಂಕ s, Auto_ಬಫರ್<ದ್ವಿಗುಣ, MAX_LOCAL_CN>& buf)
{
    ಸ್ಥಿರಾಂಕ TExpr::Arg& a = e.arginfo[s];
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = std::max(1, a.channels);
    buf.resize(cn);
    ಸ್ಥಿರಾಂಕ uchar* src = (ಸ್ಥಿರಾಂಕ uchar*)(e.constbuf.ದತ್ತಾಂಶ() + a.constofs);
    ಆದರೆ (a.srcdepth == CV_64F)
        ಸ್ಮರಣೆ_ನಕಲು(buf.ದತ್ತಾಂಶ(), src, (size_t)cn * ಗಾತ್ರ(ದ್ವಿಗುಣ));   // common (Scalar / parsed literal)
    ಇಲ್ಲದಿದ್ದರೆ
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, ಶೂನ್ಯಸೂಚಕ, 0, (uchar*)buf.ದತ್ತಾಂಶ(), 0, ಗಾತ್ರ_2(cn, 1), ಶೂನ್ಯಸೂಚಕ);
    ಹಿಂತಿರುಗಿ cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isFlexConst(ಸ್ಥಿರಾಂಕ TExpr& e, ಪೂರ್ಣಾಂಕ s)
{
    ಹಿಂತಿರುಗಿ e.arginfo[s].ಬಗೆ == TExpr::CONST && e.arginfo[s].ಆಳ == EW_ಆಳ_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
ಸ್ಥಿರ ತಾರ್ಕಿಕ scalar_Const_ಮೌಲ್ಯ(ಸ್ಥಿರಾಂಕ TExpr& e, ಪೂರ್ಣಾಂಕ s, ದ್ವಿಗುಣ& v)
{
    ಸ್ಥಿರಾಂಕ TExpr::Arg& a = e.arginfo[s];
    ಆದರೆ (a.ಬಗೆ != TExpr::CONST || std::max(1, a.channels) != 1) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    Auto_ಬಫರ್<ದ್ವಿಗುಣ, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    ಹಿಂತಿರುಗಿ ನಿಜ;
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
ಸ್ಥಿರ ತಾರ್ಕಿಕ ಹೊಂದಿಕೆ_Scaled_Chain(ಸ್ಥಿರಾಂಕ TExpr& e, ಪೂರ್ಣಾಂಕ end, ಪೂರ್ಣಾಂಕ t,
                             ಪೂರ್ಣಾಂಕ& arr_ಸ್ಥಾನ, ದ್ವಿಗುಣ& scale, ಪೂರ್ಣಾಂಕ& ಆರಂಭ_Idx)
{
    ಆದರೆ (end < 0) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ j = end, mul_ಫಲಿತಾಂಶ = t;

    // optional trailing narrowing cast
    ಆದರೆ (e.prog[j].op == OP_CAST && e.prog[j].ಫಲಿತಾಂಶ == t)
    {
        mul_ಫಲಿತಾಂಶ = e.prog[j].arg0;
        ಆದರೆ (--j < 0) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಸ್ಥಿರಾಂಕ TExpr::Insn& mul = e.prog[j];
    ಆದರೆ (mul.op != OP_MUL || mul.ಫಲಿತಾಂಶ != mul_ಫಲಿತಾಂಶ || mul.params[0] != 1.0) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ arr;
    ಆದರೆ      (scalar_Const_ಮೌಲ್ಯ(e, mul.arg1, scale)) arr = mul.arg0;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scalar_Const_ಮೌಲ್ಯ(e, mul.arg0, scale)) arr = mul.arg1;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;                                  // array*array is not an addWeighted term
    ಆರಂಭ_Idx = j;

    // optional leading widening cast feeding the multiply
    ಆದರೆ (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].ಫಲಿತಾಂಶ == arr &&
        e.arginfo[arr].ಬಗೆ == TExpr::ತಾತ್ಕಾಲಿಕ)
    {
        arr = e.prog[j-1].arg0;
        ಆರಂಭ_Idx = j - 1;
    }

    ಆದರೆ (e.arginfo[arr].ಬಗೆ == TExpr::CONST) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;   // scalar*scalar is folded elsewhere
    arr_ಸ್ಥಾನ = arr;
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
ಸ್ಥಿರ ತಾರ್ಕಿಕ const_ಹೊಂದುತ್ತದೆ(ಸ್ಥಿರಾಂಕ TExpr& e, ಪೂರ್ಣಾಂಕ s, ಪೂರ್ಣಾಂಕ d)
{
    ಆದರೆ (!isFlexConst(e, s)) ಹಿಂತಿರುಗಿ ನಿಜ;
    Auto_ಬಫರ್<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v;
    ಪೂರ್ಣಾಂಕ cn = constDoubles(e, s, v);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ch = 0; ch < cn; ch++)
        ಆದರೆ (!ಆಳ_Represents(v[ch], d)) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಂಕ TExpr::emit_ದ್ವಿಮಾನ(ತುದಿ op, ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ rdepth, ಸ್ಥಿರಾಂಕ Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    ಆದರೆ (op == OP_ADDW)
    {
        ಪೂರ್ಣಾಂಕ Tt = arginfo[a].ಆಳ;
        ಆದರೆ (arginfo[a].ಆಳ != arginfo[b].ಆಳ)
        {
            Tt = promoteArith(arginfo[a].ಆಳ, arginfo[b].ಆಳ);
            a = maybe_ಕೂಡಿಸು_Cast(a, Tt); b = maybe_ಕೂಡಿಸು_Cast(b, Tt);
        }
        ಆದರೆ (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        ಆದರೆ (rdepth == EW_ಆಳ_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_ಆಳ_NONE, rdepth);
        ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_D = rdepth;
        ಆದರೆ (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            ಹೊರಹೋಗು_D = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_ಆಳ_NONE, ಹೊರಹೋಗು_D);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ out = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(ಹೊರಹೋಗು_D);
        ಕೂಡಿಸು_Insn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        ಆದರೆ (ಹೊರಹೋಗು_D == rdepth) ಹಿಂತಿರುಗಿ out;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ out2 = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(rdepth);
        ಕೂಡಿಸು_Insn(OP_CAST, out, 0, 0, out2);
        ಹಿಂತಿರುಗಿ out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    ಆದರೆ (op == OP_ಕೂಡಿಸು)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = (ಪೂರ್ಣಾಂಕ)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        ಪೂರ್ಣಾಂಕ x = 0, y = 0, ಆರಂಭ_A = 0, ಆರಂಭ_B = 0;
        ದ್ವಿಗುಣ alpha = 0, beta = 0;
        ಆದರೆ (n >= 2 && a != b &&
            arginfo[a].ಬಗೆ == ತಾತ್ಕಾಲಿಕ && !arginfo[a].pinned &&
            arginfo[b].ಬಗೆ == ತಾತ್ಕಾಲಿಕ && !arginfo[b].pinned &&
            ಹೊಂದಿಕೆ_Scaled_Chain(*ಇದು, n - 1, b, y, beta, ಆರಂಭ_B) &&
            ಹೊಂದಿಕೆ_Scaled_Chain(*ಇದು, ಆರಂಭ_B - 1, a, x, alpha, ಆರಂಭ_A) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].ಆಳ != CV_Bool && arginfo[y].ಆಳ != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            ಪೂರ್ಣಾಂಕ retired = 0, lowest = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಆರಂಭ_A; i < n; i++)
            {
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ r = prog[i].ಫಲಿತಾಂಶ;
                ಆದರೆ (arginfo[r].ಬಗೆ != ತಾತ್ಕಾಲಿಕ || arginfo[r].pinned) { retired = -1; ನಿಲ್ಲಿಸು; }
                lowest = std::min(lowest, arginfo[r].ಸೂಚ್ಯಂಕ);
                retired++;
            }
            ಆದರೆ (retired > 0 && lowest == ntemps - retired)
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಆರಂಭ_A; i < n; i++) arginfo[prog[i].ಫಲಿತಾಂಶ].ಬಗೆ = NONE;
                prog.resize(ಆರಂಭ_A);
                ntemps -= retired;
                ಹಿಂತಿರುಗಿ emit_ದ್ವಿಮಾನ(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            ಪೂರ್ಣಾಂಕ acc = 0; ದ್ವಿಗುಣ gamma = 0;
            ಆದರೆ      (scalar_Const_ಮೌಲ್ಯ(*ಇದು, b, gamma)) acc = a;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scalar_Const_ಮೌಲ್ಯ(*ಇದು, a, gamma)) acc = b;
            ಆದರೆ (acc != 0 && n >= 1 &&
                arginfo[acc].ಬಗೆ == ತಾತ್ಕಾಲಿಕ && !arginfo[acc].pinned &&
                arginfo[acc].ಸೂಚ್ಯಂಕ == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].ಫಲಿತಾಂಶ == acc &&
                (rdepth == EW_ಆಳ_NONE || rdepth == arginfo[acc].ಆಳ))
            {
                prog[n-1].params[2] += gamma;
                ಹಿಂತಿರುಗಿ acc;
            }
        }
    }

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nd0 = isFlexConst(*ಇದು, a) ? EW_ಆಳ_NONE : arginfo[a].ಆಳ;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nd1 = isFlexConst(*ಇದು, b) ? EW_ಆಳ_NONE : arginfo[b].ಆಳ;
    ಸ್ಥಿರಾಂಕ ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = rdepth;
    ಆದರೆ (ಫಲಿತಾಂಶ == EW_ಆಳ_NONE)
    {
        ಫಲಿತಾಂಶ = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        ಆದರೆ (ಫಲಿತಾಂಶ == EW_ಆಳ_NONE) ಫಲಿತಾಂಶ = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    ಪೂರ್ಣಾಂಕ ತಳ;
    ಆದರೆ (nd0 == EW_ಆಳ_NONE && nd1 == EW_ಆಳ_NONE) ತಳ = ಫಲಿತಾಂಶ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nd0 == EW_ಆಳ_NONE) ತಳ = nd1;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nd1 == EW_ಆಳ_NONE) ತಳ = nd0;
    ಇಲ್ಲದಿದ್ದರೆ ತಳ = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    ಆದರೆ (cat == CAT_COMPARE && nd0 != EW_ಆಳ_NONE && !is_Float_ಆಳ(nd0) &&
        isFlexConst(*ಇದು, b) && !const_ಹೊಂದುತ್ತದೆ(*ಇದು, b, ತಳ))
    {
        ದ್ವಿಗುಣ lo, hi; intRange(nd0, lo, hi);
        ಆದರೆ (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            ಎಣಿಕೆ { REAL, CFALSE, CTRUE };
            Auto_ಬಫರ್<ದ್ವಿಗುಣ, MAX_LOCAL_CN> tv; ಪೂರ್ಣಾಂಕ cn = constDoubles(*ಇದು, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            ಪೂರ್ಣಾಂಕ ಬಗೆ[4] = {}; ದ್ವಿಗುಣ bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ಸ್ಥಿರಾಂಕ ತುದಿ fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
            {
                ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ t = tv[c]; ಪೂರ್ಣಾಂಕ k; ದ್ವಿಗುಣ B = t;
                ಆಯ್ಕೆ (op)
                {
                ಸ್ಥಿತಿ OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ OP_CMP_EQ: k = ಆಳ_Represents(t, nd0) ? REAL : CFALSE; ನಿಲ್ಲಿಸು;   // a==non-rep -> false
                ಪೂರ್ವನಿಯೋಜಿತ:        k = ಆಳ_Represents(t, nd0) ? REAL : CTRUE;  ನಿಲ್ಲಿಸು;   // NE: a!=non-rep -> true
                }
                ಬಗೆ[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            ಸ್ವಯಂಚಾಲಿತ ucOp  = [&](ಪೂರ್ಣಾಂಕ c){ ಹಿಂತಿರುಗಿ ಬಗೆ[c]==REAL ? fam : ಬಗೆ[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            ಸ್ವಯಂಚಾಲಿತ ucThr = [&](ಪೂರ್ಣಾಂಕ c){ ಹಿಂತಿರುಗಿ ಬಗೆ[c]==REAL ? bound[c]
                                          : (ಬಗೆ[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ಸ್ಥಿರಾಂಕ ತುದಿ u0 = ucOp(0);
            ತಾರ್ಕಿಕ ಏಕರೂಪ = ನಿಜ;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 1; c < cn && ಏಕರೂಪ; c++) ಏಕರೂಪ = (ucOp(c) == u0);
            ಆದರೆ (ಏಕರೂಪ)
            {
                Scalar s; ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++) s[c] = ucThr(c);
                b = ಕೂಡಿಸು_Const(EW_ಆಳ_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; ಪೂರ್ಣಾಂಕ patchFlags = 0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                {
                    ts[c] = ಬಗೆ[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ mbits = ಬಗೆ[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vbits = ಬಗೆ[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_ಸರಿಸು + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, ತಳ, ತಳ, EW_ಆಳ_NONE, ಫಲಿತಾಂಶ);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ out = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(ಫಲಿತಾಂಶ);
                ಕೂಡಿಸು_Insn(fam, a, ಕೂಡಿಸು_Const(ತಳ, ts, cn), 0, out, kern);
                ಹಿಂತಿರುಗಿ out;
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
    ಪೂರ್ಣಾಂಕ ಆಳ, wide;
    ತಾರ್ಕಿಕ divGuard = ಸುಳ್ಳು;
    ಆಯ್ಕೆ (op)
    {
    ಸ್ಥಿತಿ OP_MIN: ಸ್ಥಿತಿ OP_MAX:
    ಸ್ಥಿತಿ OP_AND: ಸ್ಥಿತಿ OP_OR: ಸ್ಥಿತಿ OP_XOR:
        ಆಳ = ಫಲಿತಾಂಶ; wide = ಫಲಿತಾಂಶ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        ಪೂರ್ಣಾಂಕ common = (nd0 == EW_ಆಳ_NONE) ? nd1
                   : (nd1 == EW_ಆಳ_NONE) ? nd0 : promoteArith(nd0, nd1);
        ಆದರೆ (common == EW_ಆಳ_NONE) common = ಫಲಿತಾಂಶ;          // both operands flexible consts
        ಆಳ = common;
        wide  = absdiff_ಫಲಿತಾಂಶ_ಆಳ(common);
        ಆದರೆ (rdepth == EW_ಆಳ_NONE) ಫಲಿತಾಂಶ = wide;            // auto result: unsigned same width
        ನಿಲ್ಲಿಸು;
    }
    ಸ್ಥಿತಿ OP_CMP_EQ: ಸ್ಥಿತಿ OP_CMP_NE: ಸ್ಥಿತಿ OP_CMP_LT:
    ಸ್ಥಿತಿ OP_CMP_LE: ಸ್ಥಿತಿ OP_CMP_GT: ಸ್ಥಿತಿ OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        ಆಳ = ತಳ; wide = CV_64F; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_MUL: ಸ್ಥಿತಿ OP_DIV:
    {
        ಆಳ = ತಳ;
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ w = (ಆಳ==CV_32U || ಆಳ==CV_32S || ಆಳ==CV_64U ||
                        ಆಳ==CV_64S || ಆಳ==CV_64F);
        wide = w ? CV_64F : CV_32F;
        ಆದರೆ (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            ಸ್ವಯಂಚಾಲಿತ int_ಕಾರ್ಯಾಂಶ = [&](ಪೂರ್ಣಾಂಕ s, ಪೂರ್ಣಾಂಕ nd) {
                ಆದರೆ (nd != EW_ಆಳ_NONE) ಹಿಂತಿರುಗಿ !is_Float_ಆಳ(nd);
                Auto_ಬಫರ್<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v; constDoubles(*ಇದು, s, v);
                ಹಿಂತಿರುಗಿ v[0] == std::floor(v[0]); };
            divGuard = int_ಕಾರ್ಯಾಂಶ(a, nd0) && int_ಕಾರ್ಯಾಂಶ(b, nd1);
        }
        ನಿಲ್ಲಿಸು;
    }
    ಸ್ಥಿತಿ OP_POW: ಸ್ಥಿತಿ OP_HYPOT: ಸ್ಥಿತಿ OP_ATAN2:
        ಆಳ = (ತಳ == CV_64F) ? CV_64F : CV_32F; wide = ಆಳ; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:        // OP_ADD, OP_SUB
        ಆಳ = ತಳ; wide = ಸುರಕ್ಷಿತ_Wide(ಆಳ); ನಿಲ್ಲಿಸು;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    ಸ್ವಯಂಚಾಲಿತ fracConst = [&](ಪೂರ್ಣಾಂಕ s) {
        ಆದರೆ (!isFlexConst(*ಇದು, s)) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        Auto_ಬಫರ್<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v; ಪೂರ್ಣಾಂಕ n = constDoubles(*ಇದು, s, v);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ch = 0; ch < n; ch++)
            ಆದರೆ (v[ch] != std::floor(v[ch])) ಹಿಂತಿರುಗಿ ನಿಜ;
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    ಆದರೆ (cat != CAT_BITWISE && (!const_ಹೊಂದುತ್ತದೆ(*ಇದು, a, ಆಳ) || !const_ಹೊಂದುತ್ತದೆ(*ಇದು, b, ಆಳ)))
    {
        ಆಳ = wide;
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        ಆದರೆ (frac && !is_Float_ಆಳ(ಆಳ) && op != OP_MIN && op != OP_MAX)
            ಆಳ = wide = (ಫಲಿತಾಂಶ == CV_64F || ತಳ == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        ಆದರೆ (frac && (op == OP_MUL || op == OP_DIV) && ಆಳ == CV_32F)
            ಆಳ = wide = CV_64F;
    }

    ಪೂರ್ಣಾಂಕ s0 = isFlexConst(*ಇದು, a) ? typedConstFrom(a, ಆಳ) : a;
    ಪೂರ್ಣಾಂಕ s1 = isFlexConst(*ಇದು, b) ? typedConstFrom(b, ಆಳ) : b;
    ಪೂರ್ಣಾಂಕ c0 = maybe_ಕೂಡಿಸು_Cast(s0, ಆಳ), c1 = maybe_ಕೂಡಿಸು_Cast(s1, ಆಳ);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    ಸ್ವಯಂಚಾಲಿತ resolve = [&](ಪೂರ್ಣಾಂಕ rd) {
        ಹಿಂತಿರುಗಿ op == OP_DIV ? getDivFunc(ಆಳ, rd, divGuard)
                            : getElemwiseFunc(op, ಆಳ, ಆಳ, EW_ಆಳ_NONE, rd);
    };
    TKernel k = resolve(ಫಲಿತಾಂಶ);
    ಆದರೆ (k.fptr)
    {
        ಪೂರ್ಣಾಂಕ res = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(ಫಲಿತಾಂಶ);
        ಕೂಡಿಸು_Insn(op, c0, c1, 0, res, k, params);
        ಹಿಂತಿರುಗಿ res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ಪೂರ್ಣಾಂಕ w = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(wide);
    ಕೂಡಿಸು_Insn(op, c0, c1, 0, w, k, params);
    ಹಿಂತಿರುಗಿ maybe_ಕೂಡಿಸು_Cast(w, ಫಲಿತಾಂಶ);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಂಕ TExpr::emitUnary(ತುದಿ op, ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ rdepth, ಸ್ಥಿರಾಂಕ Scalar& params)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nd = isFlexConst(*ಇದು, a) ? EW_ಆಳ_NONE : arginfo[a].ಆಳ;
    ಸ್ಥಿರಾಂಕ ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    ಆದರೆ (op == OP_NEG)
        ಹಿಂತಿರುಗಿ emit_ದ್ವಿಮಾನ(OP_SUB, ಕೂಡಿಸು_Const(EW_ಆಳ_NONE, Scalar(0.), 1), a, rdepth, params);
    ಆದರೆ (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        ಆದರೆ (!prog.empty() && arginfo[a].ಬಗೆ == ತಾತ್ಕಾಲಿಕ && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().ಫಲಿತಾಂಶ == a &&
            arginfo[a].ಸೂಚ್ಯಂಕ == ntemps - 1)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].ಬಗೆ = NONE;
            ntemps--;
            ಹಿಂತಿರುಗಿ emit_ದ್ವಿಮಾನ(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        ಹಿಂತಿರುಗಿ emit_ದ್ವಿಮಾನ(OP_ABSDIFF, a, ಕೂಡಿಸು_Const(EW_ಆಳ_NONE, Scalar(0.), 1), rdepth, params);
    }

    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = rdepth;
    ಆದರೆ (ಫಲಿತಾಂಶ == EW_ಆಳ_NONE)
    {
        ಆಯ್ಕೆ (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        ಸ್ಥಿತಿ CAT_MATH:    ಫಲಿತಾಂಶ = is_Float_ಆಳ(nd) ? nd : CV_32F; ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ CAT_BITWISE: ಫಲಿತಾಂಶ = (nd == EW_ಆಳ_NONE) ? CV_32S : nd; ನಿಲ್ಲಿಸು;  // NOT
        ಪೂರ್ವನಿಯೋಜಿತ:          ಫಲಿತಾಂಶ = (nd == EW_ಆಳ_NONE) ? CV_32F : nd; ನಿಲ್ಲಿಸು;  // NEG, ABS
        }
    }

    ಪೂರ್ಣಾಂಕ ಆಳ, wide;
    ಆಯ್ಕೆ (cat)
    {
    ಸ್ಥಿತಿ CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        ಆಳ = (is_Float_ಆಳ(nd) && nd == ಫಲಿತಾಂಶ) ? nd
              : (nd == CV_64F || ಫಲಿತಾಂಶ == CV_64F) ? CV_64F : CV_32F;
        wide = ಆಳ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CAT_BITWISE: ಆಳ = ಫಲಿತಾಂಶ; wide = ಫಲಿತಾಂಶ; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:          ಆಳ = (nd == EW_ಆಳ_NONE) ? ಫಲಿತಾಂಶ : nd; wide = ಫಲಿತಾಂಶ; ನಿಲ್ಲಿಸು;  // NEG/ABS
    }

    ಆದರೆ (!const_ಹೊಂದುತ್ತದೆ(*ಇದು, a, ಆಳ)) ಆಳ = (cat == CAT_MATH) ? ಆಳ : CV_32F;
    ಪೂರ್ಣಾಂಕ s0 = isFlexConst(*ಇದು, a) ? typedConstFrom(a, ಆಳ) : a;
    ಪೂರ್ಣಾಂಕ c0 = maybe_ಕೂಡಿಸು_Cast(s0, ಆಳ);

    TKernel k = getElemwiseFunc(op, ಆಳ, EW_ಆಳ_NONE, EW_ಆಳ_NONE, ಫಲಿತಾಂಶ);
    ಆದರೆ (k.fptr)
    {
        ಪೂರ್ಣಾಂಕ res = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(ಫಲಿತಾಂಶ);
        ಕೂಡಿಸು_Insn(op, c0, 0, 0, res, k, params);
        ಹಿಂತಿರುಗಿ res;
    }
    k = getElemwiseFunc(op, ಆಳ, EW_ಆಳ_NONE, EW_ಆಳ_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ಪೂರ್ಣಾಂಕ w = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(wide);
    ಕೂಡಿಸು_Insn(op, c0, 0, 0, w, k, params);
    ಹಿಂತಿರುಗಿ maybe_ಕೂಡಿಸು_Cast(w, ಫಲಿತಾಂಶ);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಂಕ TExpr::emitTernary(ತುದಿ op, ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ rdepth)
{
    ಸ್ವಯಂಚಾಲಿತ dep = [&](ಪೂರ್ಣಾಂಕ s) { ಹಿಂತಿರುಗಿ isFlexConst(*ಇದು, s) ? EW_ಆಳ_NONE : arginfo[s].ಆಳ; };

    ಆದರೆ (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        ಆದರೆ (isFlexConst(*ಇದು, a) || CV_ELEM_SIZE1(arginfo[a].ಆಳ) != 1)
            a = emit_ದ್ವಿಮಾನ(OP_CMP_NE, a, ಕೂಡಿಸು_Const(EW_ಆಳ_NONE, Scalar(0.), 1), CV_8U, Scalar());
        ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = rdepth != EW_ಆಳ_NONE ? rdepth : promoteArith(dep(b), dep(c));
        ಆದರೆ (ಫಲಿತಾಂಶ == EW_ಆಳ_NONE) ಫಲಿತಾಂಶ = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        ಪೂರ್ಣಾಂಕ sb = isFlexConst(*ಇದು, b) ? typedConstFrom(b, ಫಲಿತಾಂಶ) : maybe_ಕೂಡಿಸು_Cast(b, ಫಲಿತಾಂಶ);
        ಪೂರ್ಣಾಂಕ sc = isFlexConst(*ಇದು, c) ? typedConstFrom(c, ಫಲಿತಾಂಶ) : maybe_ಕೂಡಿಸು_Cast(c, ಫಲಿತಾಂಶ);
        ಪೂರ್ಣಾಂಕ res = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(ಫಲಿತಾಂಶ);
        ಕೂಡಿಸು_Insn(OP_SELECT, a, sb, sc, res);
        ಹಿಂತಿರುಗಿ res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = rdepth != EW_ಆಳ_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    ಆದರೆ (ಫಲಿತಾಂಶ == EW_ಆಳ_NONE) ಫಲಿತಾಂಶ = CV_32F;
    ಸ್ವಯಂಚಾಲಿತ typed = [&](ಪೂರ್ಣಾಂಕ s) {
        ಹಿಂತಿರುಗಿ isFlexConst(*ಇದು, s) ? typedConstFrom(s, ಫಲಿತಾಂಶ) : maybe_ಕೂಡಿಸು_Cast(s, ಫಲಿತಾಂಶ);
    };
    ಪೂರ್ಣಾಂಕ c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, ಫಲಿತಾಂಶ, ಫಲಿತಾಂಶ, ಫಲಿತಾಂಶ, ಫಲಿತಾಂಶ);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ಪೂರ್ಣಾಂಕ res = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(ಫಲಿತಾಂಶ);
    ಕೂಡಿಸು_Insn(op, c0, c1, c2, res, k);
    ಹಿಂತಿರುಗಿ res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಂಕ TExpr::move_ಗಮ್ಯ_Output(ಪೂರ್ಣಾಂಕ ತಾತ್ಕಾಲಿಕ_2, ಪೂರ್ಣಾಂಕ out)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ninsn = (ಪೂರ್ಣಾಂಕ)prog.size();
    ಪೂರ್ಣಾಂಕ ಉತ್ಪಾದಕ = -1;
    ತಾರ್ಕಿಕ ಬಳಸಿದ_As_Arg = ಸುಳ್ಳು;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninsn; i++)
    {
        ಸ್ಥಿರಾಂಕ TExpr::Insn& ins = prog[i];
        ಆದರೆ (ins.ಫಲಿತಾಂಶ == ತಾತ್ಕಾಲಿಕ_2) ಉತ್ಪಾದಕ = i;
        ಆದರೆ (ins.arg0 == ತಾತ್ಕಾಲಿಕ_2 || ins.arg1 == ತಾತ್ಕಾಲಿಕ_2 || ins.arg2 == ತಾತ್ಕಾಲಿಕ_2) ಬಳಸಿದ_As_Arg = ನಿಜ;
    }
    ಆದರೆ (arginfo[ತಾತ್ಕಾಲಿಕ_2].ಬಗೆ == ತಾತ್ಕಾಲಿಕ && ಉತ್ಪಾದಕ >= 0 && !ಬಳಸಿದ_As_Arg && !arginfo[ತಾತ್ಕಾಲಿಕ_2].pinned &&
        arginfo[ತಾತ್ಕಾಲಿಕ_2].ಆಳ == arginfo[out].ಆಳ)
    {
        // MOVE semantics: redirect `temp`'s single producer to write `out` directly, then leave the
        // source slot EMPTY - reclassify it to NONE. A NONE slot gets no physical buffer (compile's
        // liveness only walks TEMP slots) and is skipped everywhere in exec, so no stale dead-TEMP slot
        // remains (which, buffer or not, would still cost per-tile setup in the general path) - and no
        // slot removal / index renumbering is needed. This keeps single-op programs at zero temps =>
        // compile()'s no-temp early-out (zero heap traffic) still fires. Every caller moves the result
        // it JUST emitted, so `temp` is always the last-created temp - decrement ntemps to retire its
        // index and keep the temp indices dense (0..ntemps-1) for compile()'s liveness arrays.
        CV_Assert(arginfo[ತಾತ್ಕಾಲಿಕ_2].ಸೂಚ್ಯಂಕ == ntemps - 1);
        prog[ಉತ್ಪಾದಕ].ಫಲಿತಾಂಶ = out;
        arginfo[ತಾತ್ಕಾಲಿಕ_2].ಬಗೆ = NONE;
        ntemps--;
        ಹಿಂತಿರುಗಿ out;
    }
    ಕೂಡಿಸು_Insn(OP_CAST, ತಾತ್ಕಾಲಿಕ_2, 0, 0, out);                     // same/different depth copy into `out`
    ಹಿಂತಿರುಗಿ out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
ಪೂರ್ಣಾಂಕ TExpr::output(ಪೂರ್ಣಾಂಕ ಬೇರು_ಸ್ಥಾನ)
{
    ಹಿಂತಿರುಗಿ move_ಗಮ್ಯ_Output(ಬೇರು_ಸ್ಥಾನ, ಕೂಡಿಸು_Output(arginfo[ಬೇರು_ಸ್ಥಾನ].ಆಳ));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
ಶೂನ್ಯ TExpr::compile()
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ninsn = (ಪೂರ್ಣಾಂಕ)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nslots = (ಪೂರ್ಣಾಂಕ)arginfo.size();
    nconsts = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        ಆದರೆ (a.ಬಗೆ != CONST || a.ಆಳ == EW_ಆಳ_NONE) ಮುಂದುವರಿ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.ಆಳ;
        ಸ್ಥಿರಾಂಕ size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        Auto_ಬಫರ್<uint64_t, ಗಾತ್ರ(uint64_t)> srcbuf(div_ಮೇಲೆ((size_t)cn * sesz, (ಚಿಹ್ನೆರಹಿತ)ಗಾತ್ರ(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.ದತ್ತಾಂಶ();
        ಸ್ಮರಣೆ_ನಕಲು(srcbytes, (ಸ್ಥಿರಾಂಕ uchar*)(constbuf.ದತ್ತಾಂಶ() + a.constofs), (size_t)cn * sesz);
        ಸ್ಥಿರಾಂಕ size_t ofs = appendConstBuf(constbuf, dd, ಶೂನ್ಯಸೂಚಕ, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.ದತ್ತಾಂಶ() + ofs);
        ಆದರೆ (sd == dd) ಸ್ಮರಣೆ_ನಕಲು(dst, srcbytes, (size_t)cn * desz);
        ಇಲ್ಲದಿದ್ದರೆ getConvertFunc(sd, dd)(srcbytes, 0, ಶೂನ್ಯಸೂಚಕ, 0, dst, 0, ಗಾತ್ರ_2(cn, 1), ಶೂನ್ಯಸೂಚಕ);
        a.constofs = ofs; a.srcdepth = dd;
        nconsts++;
    }

    // ---- liveness: pack temps into a minimal set of reusable physical buffers. A program with no
    //      temps (single-op add/sub/mul/...) needs none of this - early out with ZERO heap traffic,
    //      so building such a program (the dominant cv::add-style call) allocates nothing. The temp
    //      case uses stack-backed AutoBuffers (inline for typical small expressions). ----
    nbuffers = 0;
    buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ.resize(1); buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[0] = 0;
    capElems = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;                       // no temps => exec runs each tile in one block, no scratch
    ಆದರೆ (ntemps == 0) ಹಿಂತಿರುಗಿ;

    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, 32> ತಾತ್ಕಾಲಿಕ_ದ_ಸ್ಥಾನ(nslots);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < nslots; s++) ತಾತ್ಕಾಲಿಕ_ದ_ಸ್ಥಾನ[s] = -1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 1; s < nslots; s++)
        ಆದರೆ (arginfo[s].ಬಗೆ == ತಾತ್ಕಾಲಿಕ) ತಾತ್ಕಾಲಿಕ_ದ_ಸ್ಥಾನ[s] = arginfo[s].ಸೂಚ್ಯಂಕ;

    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, 16> ಕೊನೆಯ_Use(ntemps);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ t = 0; t < ntemps; t++) ಕೊನೆಯ_Use[t] = -1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninsn; i++)
    {
        ಸ್ಥಿರಾಂಕ TExpr::Insn& ins = prog[i];
        ಪೂರ್ಣಾಂಕ as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 3; k++)
            ಆದರೆ (ತಾತ್ಕಾಲಿಕ_ದ_ಸ್ಥಾನ[as[k]] >= 0) ಕೊನೆಯ_Use[ತಾತ್ಕಾಲಿಕ_ದ_ಸ್ಥಾನ[as[k]]] = i;
    }

    ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ.resize(ntemps);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ t = 0; t < ntemps; t++) ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ[t] = -1;
    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, 16> freeBufs(ntemps);
    ಪೂರ್ಣಾಂಕ nfree = 0, nbuf = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninsn; i++)
    {
        ಸ್ಥಿರಾಂಕ TExpr::Insn& ins = prog[i];
        ಪೂರ್ಣಾಂಕ rt = ತಾತ್ಕಾಲಿಕ_ದ_ಸ್ಥಾನ[ins.ಫಲಿತಾಂಶ];
        ಆದರೆ (rt >= 0 && ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ[rt] < 0)
            ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        ಪೂರ್ಣಾಂಕ as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < 3; k++)
        {
            ಪೂರ್ಣಾಂಕ t = ತಾತ್ಕಾಲಿಕ_ದ_ಸ್ಥಾನ[as[k]];
            ಆದರೆ (t >= 0 && ಕೊನೆಯ_Use[t] == i)
            {
                freeBufs[nfree++] = ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ[t];
                ಕೊನೆಯ_Use[t] = -1;   // the same temp may be several args of this insn ("d*d") -
                                   // release its buffer once, or freeBufs overflows/double-frees
            }
        }
    }

    nbuffers = nbuf;

    // Byte layout of the physical temp buffers, per output element: prefix sums of each buffer's
    // max element size. bufEszPrefix[b]*region = buffer b's byte offset in the scratch; [nbuffers] =
    // total temp bytes/element. Precomputed here (depths are build-time) so exec()/its fast path
    // never recompute it per call, and the fast path can size its scratch in O(1).
    buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ.resize(nbuffers + 1);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ b = 0; b <= nbuffers; b++) buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[b] = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 1; s < nslots; s++)
        ಆದರೆ (arginfo[s].ಬಗೆ == ತಾತ್ಕಾಲಿಕ)
        {
            ಪೂರ್ಣಾಂಕ b = ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ[arginfo[s].ಸೂಚ್ಯಂಕ], e = (ಪೂರ್ಣಾಂಕ)CV_ELEM_SIZE1(arginfo[s].ಆಳ);
            ಆದರೆ (e > buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[b + 1]) buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[b + 1] = e;     // max elem size in buffer b
        }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ b = 0; b < nbuffers; b++) buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[b + 1] += buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_Esz = buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[nbuffers];
    capElems = ಒಟ್ಟು_Esz > 0 ? std::max(64, (4 * 1024) / ಒಟ್ಟು_Esz) : ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
ಸ್ಥಿರ ಶೂನ್ಯ matLogical(ಸ್ಥಿರಾಂಕ Mat& m, MatShape& shp, Ew_ಹೆಜ್ಜೆಗಳು& ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ& esz1)
{
    esz1 = (ಪೂರ್ಣಾಂಕ)m.elemSize1();
    ಪೂರ್ಣಾಂಕ nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        ಹೆಜ್ಜೆ[i] = m.ಹೆಜ್ಜೆ[i] / esz1;
    }
    shp[nd] = cn;
    ಹೆಜ್ಜೆ[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
ಸ್ಥಿರ ತಾರ್ಕಿಕ ಪ್ರಸಾರ_Shape(ಸ್ಥಿರಾಂಕ MatShape* shps, ಪೂರ್ಣಾಂಕ K, MatShape& out)
{
    ಪೂರ್ಣಾಂಕ nd = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
    {
        ಸ್ಥಿರಾಂಕ MatShape& s = shps[k];
        ಪೂರ್ಣಾಂಕ off = nd - s.dims;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < s.dims; i++)
        {
            ಪೂರ್ಣಾಂಕ d = s[i], &o = out[off + i];
            ಆದರೆ (o == 1) o = d;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (d != 1 && d != o) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ op_ವೆಚ್ಚ(ತುದಿ op)
{
    ಆಯ್ಕೆ (op)
    {
    ಸ್ಥಿತಿ OP_ಕೂಡಿಸು: ಸ್ಥಿತಿ OP_SUB: ಸ್ಥಿತಿ OP_MUL: ಸ್ಥಿತಿ OP_MIN: ಸ್ಥಿತಿ OP_MAX:
    ಸ್ಥಿತಿ OP_ABSDIFF: ಸ್ಥಿತಿ OP_AND: ಸ್ಥಿತಿ OP_OR: ಸ್ಥಿತಿ OP_XOR: ಸ್ಥಿತಿ OP_NOT:
    ಸ್ಥಿತಿ OP_NEG: ಸ್ಥಿತಿ OP_ABS: ಸ್ಥಿತಿ OP_CAST: ಸ್ಥಿತಿ OP_RELU: ಸ್ಥಿತಿ OP_SELECT:
    ಸ್ಥಿತಿ OP_CMP_EQ: ಸ್ಥಿತಿ OP_CMP_NE: ಸ್ಥಿತಿ OP_CMP_LT:
    ಸ್ಥಿತಿ OP_CMP_LE: ಸ್ಥಿತಿ OP_CMP_GT: ಸ್ಥಿತಿ OP_CMP_GE: ಹಿಂತಿರುಗಿ 1;
    ಸ್ಥಿತಿ OP_DIV: ಸ್ಥಿತಿ OP_SQRT: ಸ್ಥಿತಿ OP_HYPOT: ಸ್ಥಿತಿ OP_CONVERT_SCALE: ಹಿಂತಿರುಗಿ 3;
    ಸ್ಥಿತಿ OP_SIN: ಸ್ಥಿತಿ OP_COS: ಸ್ಥಿತಿ OP_TANH: ಸ್ಥಿತಿ OP_ERF: ಸ್ಥಿತಿ OP_ATAN2:
    ಸ್ಥಿತಿ OP_EXP: ಸ್ಥಿತಿ OP_LOG: ಸ್ಥಿತಿ OP_POW:           ಹಿಂತಿರುಗಿ 6;
    ಪೂರ್ವನಿಯೋಜಿತ:                                         ಹಿಂತಿರುಗಿ 3;
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
    ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
ಶೂನ್ಯ TExpr::dump(std::ostream& os) ಸ್ಥಿರಾಂಕ
{
    ಸ್ವಯಂಚಾಲಿತ dn = [](ಪೂರ್ಣಾಂಕ d) -> ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* {
        ಆಯ್ಕೆ (d) {
        ಸ್ಥಿತಿ EW_ಆಳ_NONE: ಹಿಂತಿರುಗಿ "flex";
        ಸ್ಥಿತಿ CV_8U:  ಹಿಂತಿರುಗಿ "u8";  ಸ್ಥಿತಿ CV_8S:  ಹಿಂತಿರುಗಿ "s8";
        ಸ್ಥಿತಿ CV_16U: ಹಿಂತಿರುಗಿ "u16"; ಸ್ಥಿತಿ CV_16S: ಹಿಂತಿರುಗಿ "s16";
        ಸ್ಥಿತಿ CV_32U: ಹಿಂತಿರುಗಿ "u32"; ಸ್ಥಿತಿ CV_32S: ಹಿಂತಿರುಗಿ "s32";
        ಸ್ಥಿತಿ CV_64U: ಹಿಂತಿರುಗಿ "u64"; ಸ್ಥಿತಿ CV_64S: ಹಿಂತಿರುಗಿ "s64";
        ಸ್ಥಿತಿ CV_16F: ಹಿಂತಿರುಗಿ "f16"; ಸ್ಥಿತಿ CV_16BF:ಹಿಂತಿರುಗಿ "bf16";
        ಸ್ಥಿತಿ CV_32F: ಹಿಂತಿರುಗಿ "f32"; ಸ್ಥಿತಿ CV_64F: ಹಿಂತಿರುಗಿ "f64";
        ಸ್ಥಿತಿ CV_Bool:ಹಿಂತಿರುಗಿ "bool";
        ಪೂರ್ವನಿಯೋಜಿತ:     ಹಿಂತಿರುಗಿ "?"; }
    };
    ಸ್ವಯಂಚಾಲಿತ kn = [](Arg_ಬಗೆ k) -> ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* {
        ಆಯ್ಕೆ (k) { ಸ್ಥಿತಿ NONE: ಹಿಂತಿರುಗಿ "none"; ಸ್ಥಿತಿ ಒಳಹರಿವು: ಹಿಂತಿರುಗಿ "in"; ಸ್ಥಿತಿ CONST: ಹಿಂತಿರುಗಿ "const";
                     ಸ್ಥಿತಿ ತಾತ್ಕಾಲಿಕ: ಹಿಂತಿರುಗಿ "temp"; ಸ್ಥಿತಿ OUTPUT: ಹಿಂತಿರುಗಿ "out"; } ಹಿಂತಿರುಗಿ "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (ಪೂರ್ಣಾಂಕ)prog.size() << "\n";
    os << "  slots:\n";
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < (ಪೂರ್ಣಾಂಕ)arginfo.size(); s++) {
        ಸ್ಥಿರಾಂಕ Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.ಬಗೆ);
        ಆದರೆ (a.ಬಗೆ != NONE) os << " " << dn(a.ಆಳ);
        ಆದರೆ (a.ಬಗೆ == ಒಳಹರಿವು || a.ಬಗೆ == OUTPUT || a.ಬಗೆ == ತಾತ್ಕಾಲಿಕ) os << " idx=" << a.ಸೂಚ್ಯಂಕ;
        ಆದರೆ (a.ಬಗೆ == CONST) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = std::max(1, a.channels);
            Auto_ಬಫರ್<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v(cn);
            ಸ್ಥಿರಾಂಕ uchar* src = (ಸ್ಥಿರಾಂಕ uchar*)(constbuf.ದತ್ತಾಂಶ() + a.constofs);
            ಆದರೆ (a.srcdepth == CV_64F) ಸ್ಮರಣೆ_ನಕಲು(v.ದತ್ತಾಂಶ(), src, (size_t)cn * ಗಾತ್ರ(ದ್ವಿಗುಣ));
            ಇಲ್ಲದಿದ್ದರೆ getConvertFunc(a.srcdepth, CV_64F)(src, 0, ಶೂನ್ಯಸೂಚಕ, 0, (uchar*)v.ದತ್ತಾಂಶ(), 0, ಗಾತ್ರ_2(cn, 1), ಶೂನ್ಯಸೂಚಕ);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)prog.size(); i++) {
        ಸ್ಥಿರಾಂಕ Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        ಆದರೆ (ins.arg1) os << ", " << ins.arg1;
        ಆದರೆ (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.ಫಲಿತಾಂಶ;
        ಆದರೆ (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        ಆದರೆ (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        ಆದರೆ (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
ಪೂರ್ಣಾಂಕ TExpr::ಕೂಡಿಸು_ಒಳಹರಿವು(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    Arg a; a.ಬಗೆ = ಒಳಹರಿವು; a.ಆಳ = ಆಳ; a.ಸೂಚ್ಯಂಕ = ninputs++;
    arginfo.push_back(a); ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)arginfo.size() - 1;
}

ಪೂರ್ಣಾಂಕ TExpr::ಕೂಡಿಸು_Output(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    Arg a; a.ಬಗೆ = OUTPUT; a.ಆಳ = ಆಳ; a.ಸೂಚ್ಯಂಕ = noutputs++;
    arginfo.push_back(a); ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)arginfo.size() - 1;
}

ಪೂರ್ಣಾಂಕ TExpr::ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    Arg a; a.ಬಗೆ = ತಾತ್ಕಾಲಿಕ; a.ಆಳ = ಆಳ; a.ಸೂಚ್ಯಂಕ = ntemps++;
    arginfo.push_back(a); ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
ಪೂರ್ಣಾಂಕ TExpr::ಕೂಡಿಸು_Const(ಪೂರ್ಣಾಂಕ ಆಳ, ಸ್ಥಿರಾಂಕ Scalar& v, ಪೂರ್ಣಾಂಕ channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.ಬಗೆ = CONST; a.ಆಳ = ಆಳ; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
ಪೂರ್ಣಾಂಕ TExpr::ಕೂಡಿಸು_Const(ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ srcdepth, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.ಬಗೆ = CONST; a.ಆಳ = ಆಳ; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, ದತ್ತಾಂಶ, channels);
    arginfo.push_back(a); ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
ಪೂರ್ಣಾಂಕ TExpr::typedConstFrom(ಪೂರ್ಣಾಂಕ src_ಸ್ಥಾನ, ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರಾಂಕ Arg& src = arginfo[src_ಸ್ಥಾನ];
    Arg a; a.ಬಗೆ = CONST; a.ಆಳ = ಆಳ; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
ಪೂರ್ಣಾಂಕ TExpr::ಕೂಡಿಸು_Insn(ತುದಿ op, ಪೂರ್ಣಾಂಕ a0, ಪೂರ್ಣಾಂಕ a1, ಪೂರ್ಣಾಂಕ a2, ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ, ಸ್ಥಿರಾಂಕ TKernel& kernel, ಸ್ಥಿರಾಂಕ Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.ಫಲಿತಾಂಶ = ಫಲಿತಾಂಶ;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
ಪೂರ್ಣಾಂಕ TExpr::ಕೂಡಿಸು_Insn(ತುದಿ op, ಪೂರ್ಣಾಂಕ a0, ಪೂರ್ಣಾಂಕ a1, ಪೂರ್ಣಾಂಕ a2, ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ, ಸ್ಥಿರಾಂಕ Scalar& params)
{
    ಪೂರ್ಣಾಂಕ d0 = a0 ? arginfo[a0].ಆಳ : EW_ಆಳ_NONE;
    ಪೂರ್ಣಾಂಕ d1 = a1 ? arginfo[a1].ಆಳ : EW_ಆಳ_NONE;
    ಪೂರ್ಣಾಂಕ d2 = a2 ? arginfo[a2].ಆಳ : EW_ಆಳ_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[ಫಲಿತಾಂಶ].ಆಳ);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_Insn(op, a0, a1, a2, ಫಲಿತಾಂಶ, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
ಪೂರ್ಣಾಂಕ TExpr::maybe_ಕೂಡಿಸು_Cast(ಪೂರ್ಣಾಂಕ arg, ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಆದರೆ (arginfo[arg].ಆಳ == ಆಳ) ಹಿಂತಿರುಗಿ arg;
    ಪೂರ್ಣಾಂಕ t = ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ(ಆಳ);
    ಕೂಡಿಸು_Insn(OP_CAST, arg, 0, 0, t);
    ಹಿಂತಿರುಗಿ t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಸತತ_Insn(ಸ್ಥಿರಾಂಕ TExpr::Insn& ins,
                           ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* p0, size_t y0, size_t x0,
                           ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* p1, size_t y1, size_t x1,
                           ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* p2, size_t y2, size_t x2,
                           ಶೂನ್ಯ* pr, size_t yr, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2 = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(ಸಂಕೇತ_2 >= 0);
}

ರಚನೆ EwBody {
    ಸ್ಥಿರಾಂಕ TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ಸ್ಥಾನ_Map;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ* ಸ್ಥಾನ_ಬಗೆ; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    ಪೂರ್ಣಾಂಕ ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

ಶೂನ್ಯ TExpr::outputShape(ಸ್ಥಿರಾಂಕ Mat* ಸ್ಥಿರಾಂಕ* inputs, MatShape& spatial, ಪೂರ್ಣಾಂಕ& channels) ಸ್ಥಿರಾಂಕ
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    ತಾರ್ಕಿಕ sameShape = ninputs >= 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; sameShape && i < ninputs; i++)
    {
        ಸ್ಥಿರಾಂಕ Mat& a = *inputs[i];
        ಆದರೆ (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = ಸುಳ್ಳು;
        ಇಲ್ಲದಿದ್ದರೆ ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d = 0; d < a.dims; d++) ಆದರೆ (a.size[d] != inputs[0]->size[d]) { sameShape = ಸುಳ್ಳು; ನಿಲ್ಲಿಸು; }
    }
    ಆದರೆ (sameShape) { spatial = inputs[0]->size; channels = rcn0; ಹಿಂತಿರುಗಿ; }

    Auto_ಬಫರ್<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; Ew_ಹೆಜ್ಜೆಗಳು ಹೆಜ್ಜೆ; ಪೂರ್ಣಾಂಕ esz1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, ಹೆಜ್ಜೆ, esz1); bshapes[i] = shp; }
    MatShape ತುಂಬಿದ;
    CV_Assert(ಪ್ರಸಾರ_Shape(bshapes.ದತ್ತಾಂಶ(), ninputs, ತುಂಬಿದ) && "ew: inputs not broadcast-compatible");
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nd_ತುಂಬಿದ = (ಪೂರ್ಣಾಂಕ)ತುಂಬಿದ.size();
    channels = ತುಂಬಿದ[nd_ತುಂಬಿದ - 1];
    spatial = ತುಂಬಿದ; spatial.resize(nd_ತುಂಬಿದ - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ಶೂನ್ಯ TExpr::outputShape(ಸ್ಥಿರಾಂಕ Mat* inputs, MatShape& spatial, ಪೂರ್ಣಾಂಕ& channels) ಸ್ಥಿರಾಂಕ
{
    Auto_ಬಫರ್<ಸ್ಥಿರಾಂಕ Mat*, 16> ptrs(std::max(ninputs, 1));
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.ದತ್ತಾಂಶ(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಂಕ LOCAL_HDRS = 3;
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಂಕ LOCAL_CONSTS = 4;
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಂಕ LOCAL_OPS = 16;

ಶೂನ್ಯ TExpr::exec(ಸ್ಥಿರಾಂಕ Mat* ಸ್ಥಿರಾಂಕ* inputs, Mat* outputs)
{
    ಬಳಸಿ BrTile = ಪ್ರಸಾರ_Op::Tile;
    ಬಳಸಿ Br_ತುಂಡು = ಪ್ರಸಾರ_Op::ತುಂಡು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nslots = (ಪೂರ್ಣಾಂಕ)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].ಬಗೆ == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    ಆದರೆ (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        ಸ್ಥಿರಾಂಕ Mat& r = *inputs[0];
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ rcn = r.channels();
        ತಾರ್ಕಿಕ ok = r.isContinuous();
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; ok && i < ninputs; i++)
        {
            ಸ್ಥಿರಾಂಕ Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 1; ok && s < nslots; s++)
            ಆದರೆ (arginfo[s].ಬಗೆ == OUTPUT) ok = ok && outputs[arginfo[s].ಸೂಚ್ಯಂಕ].isContinuous();
        ಸ್ಥಿರಾಂಕ size_t ಒಟ್ಟು = ok ? r.ಒಟ್ಟು() * rcn : 0;
        ಆದರೆ (ok && ಒಟ್ಟು == 0)
        {
            ಸ್ಥಿರಾಂಕ MatShape rshape = r.size;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 1; s < nslots; s++)
                ಆದರೆ (arginfo[s].ಬಗೆ == OUTPUT)
                    outputs[arginfo[s].ಸೂಚ್ಯಂಕ].create(rshape, CV_MAKETYPE(arginfo[s].ಆಳ, rcn));
            ಹಿಂತಿರುಗಿ;
        }
        ಆದರೆ (ok && ಒಟ್ಟು <= EW_FASTPATH_MAX)
        {
            ಸ್ಥಿರಾಂಕ MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 1; s < nslots; s++)
                ಆದರೆ (arginfo[s].ಬಗೆ == OUTPUT)
                    outputs[arginfo[s].ಸೂಚ್ಯಂಕ].create(rshape, CV_MAKETYPE(arginfo[s].ಆಳ, rcn));
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ninsn = (ಪೂರ್ಣಾಂಕ)prog.size();

            ಆದರೆ (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                ಸ್ವಯಂಚಾಲಿತ ptr_ದ = [&](ಪೂರ್ಣಾಂಕ s) -> ಶೂನ್ಯ* {
                    ಆದರೆ (s <= 0) ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
                    ಸ್ಥಿರಾಂಕ Arg& ai = arginfo[s];
                    ಆದರೆ (ai.ಬಗೆ == ಒಳಹರಿವು)  ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ*)inputs[ai.ಸೂಚ್ಯಂಕ]->ದತ್ತಾಂಶ;
                    ಆದರೆ (ai.ಬಗೆ == OUTPUT) ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ*)outputs[ai.ಸೂಚ್ಯಂಕ].ದತ್ತಾಂಶ;
                    ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;   // NONE
                };
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ w = (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ n = 0; n < ninsn; n++)
                {
                    ಸ್ಥಿರಾಂಕ Insn& ins = prog[n];
                    ಸತತ_Insn(ins, ptr_ದ(ins.arg0), 0, 1, ptr_ದ(ins.arg1), 0, 1,
                            ptr_ದ(ins.arg2), 0, 1, ptr_ದ(ins.ಫಲಿತಾಂಶ), 0, w, 1);
                }
                ಹಿಂತಿರುಗಿ;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_Esz = buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[nbuffers];
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ wf0 = std::min((ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು, capElems);

            ಸ್ಥಿರಾಂಕ size_t region = align_ಗಾತ್ರ((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            Auto_ಬಫರ್<uint64_t, (4*1024 + 256)/ಗಾತ್ರ(uint64_t)> scratchBuf(
                div_ಮೇಲೆ((size_t)ಒಟ್ಟು_Esz * region, (ಚಿಹ್ನೆರಹಿತ)ಗಾತ್ರ(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.ದತ್ತಾಂಶ();
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x0 = 0; x0 < (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು; x0 += wf0)
            {
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ wf = std::min(wf0, (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು - x0);
                ಸ್ವಯಂಚಾಲಿತ ptr_ದ = [&](ಪೂರ್ಣಾಂಕ s) -> ಶೂನ್ಯ* {
                    ಆದರೆ (s <= 0) ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
                    ಸ್ಥಿರಾಂಕ Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.ಆಳ);
                    ಆದರೆ (ai.ಬಗೆ == ಒಳಹರಿವು)  ಹಿಂತಿರುಗಿ (uchar*)inputs[ai.ಸೂಚ್ಯಂಕ]->ದತ್ತಾಂಶ  + (size_t)x0 * esz;
                    ಆದರೆ (ai.ಬಗೆ == OUTPUT) ಹಿಂತಿರುಗಿ (uchar*)outputs[ai.ಸೂಚ್ಯಂಕ].ದತ್ತಾಂಶ + (size_t)x0 * esz;
                    ಆದರೆ (ai.ಬಗೆ != ತಾತ್ಕಾಲಿಕ)   ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;                                    // NONE: moved-from
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ b = ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ.empty() ? ai.ಸೂಚ್ಯಂಕ : ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ[ai.ಸೂಚ್ಯಂಕ];  //ತಾತ್ಕಾಲಿಕ
                    ಹಿಂತಿರುಗಿ scratch + (size_t)buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ[b] * region;                        // fragment-local
                };
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ n = 0; n < ninsn; n++)
                {
                    ಸ್ಥಿರಾಂಕ Insn& ins = prog[n];
                    ಸತತ_Insn(ins, ptr_ದ(ins.arg0), 0, 1, ptr_ದ(ins.arg1), 0, 1,
                            ptr_ದ(ins.arg2), 0, 1, ptr_ದ(ins.ಫಲಿತಾಂಶ), 0, wf, 1);
                }
            }
            ಹಿಂತಿರುಗಿ;
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
    ಪೂರ್ಣಾಂಕ rchannels;
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
    ತಾರ್ಕಿಕ inplace = ಸುಳ್ಳು;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninputs && !inplace; i++)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < noutputs; j++)
            ಆದರೆ (inputs[i]->ದತ್ತಾಂಶ && inputs[i]->ದತ್ತಾಂಶ == outputs[j].ದತ್ತಾಂಶ) { inplace = ನಿಜ; ನಿಲ್ಲಿಸು; }
    ಪೂರ್ಣಾಂಕ nhdrs = inplace ? ninputs : 0;

    Auto_ಬಫರ್<ಸ್ಥಿರಾಂಕ Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    Auto_ಬಫರ್<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    Auto_ಬಫರ್<ಸ್ಥಿರಾಂಕ Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    Auto_ಬಫರ್<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, LOCAL_OPS> ಸ್ಥಾನ_Map(nslots);
    Auto_ಬಫರ್<ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ, LOCAL_OPS> ಸ್ಥಾನ_ಬಗೆ(nslots);
    ಪೂರ್ಣಾಂಕ narr = 0, nc = 0;
    ಆದರೆ (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.ದತ್ತಾಂಶ();
    }
    ಸ್ಥಾನ_ಬಗೆ[0] = (ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)TExpr::NONE;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 1; s < nslots; s++)
    {
        ಸ್ಥಿರಾಂಕ TExpr::Arg& ai = arginfo[s];
        ಸ್ಥಾನ_ಬಗೆ[s] = (ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)ai.ಬಗೆ;
        ಆದರೆ (ai.ಬಗೆ == TExpr::ಒಳಹರಿವು) {
            ಸ್ಥಾನ_Map[s] = narr;
            arr[narr++] = inputs[ai.ಸೂಚ್ಯಂಕ];
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ai.ಬಗೆ == TExpr::OUTPUT) {
            outputs[ai.ಸೂಚ್ಯಂಕ].create(spatial, CV_MAKETYPE(ai.ಆಳ, rchannels));
            ಸ್ಥಾನ_Map[s] = narr; arr[narr++] = &outputs[ai.ಸೂಚ್ಯಂಕ];
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ai.ಬಗೆ == TExpr::CONST && ai.ಆಳ == EW_ಆಳ_NONE) {
            ಸ್ಥಾನ_ಬಗೆ[s] = (ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)TExpr::NONE;   // dead flexible literal: never referenced
            ಸ್ಥಾನ_Map[s] = 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ai.ಬಗೆ == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.ಆಳ, c),
                                  (ಶೂನ್ಯ*)(constbuf.ದತ್ತಾಂಶ() + ai.constofs));
            ಸ್ಥಾನ_ಬಗೆ[s] = (ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ)TExpr::ಒಳಹರಿವು;   // to the body it is just an array operand
            ಸ್ಥಾನ_Map[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ai.ಬಗೆ == TExpr::ತಾತ್ಕಾಲಿಕ) {
            ಸ್ಥಾನ_Map[s] = ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ.empty() ? ai.ಸೂಚ್ಯಂಕ : ಬಫರ್_ದ_ತಾತ್ಕಾಲಿಕ[ai.ಸೂಚ್ಯಂಕ];  // physical buffer id
        }
        ಇಲ್ಲದಿದ್ದರೆ {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            ಸ್ಥಾನ_Map[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ninsn = (ಪೂರ್ಣಾಂಕ)prog.size();
    ದೀರ್ಘ ದೀರ್ಘ ವೆಚ್ಚ_ಪ್ರತಿ_Elem = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ n = 0; n < ninsn; n++)
        ವೆಚ್ಚ_ಪ್ರತಿ_Elem += op_ವೆಚ್ಚ(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    ದೀರ್ಘ ದೀರ್ಘ otot = (ದೀರ್ಘ ದೀರ್ಘ)rchannels;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d = 0; d < (ಪೂರ್ಣಾಂಕ)spatial.size(); d++) otot *= spatial[d];
    ಆದರೆ (otot == 0)
        ಹಿಂತಿರುಗಿ;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ T = (ದ್ವಿಗುಣ)std::max(get_ಅಂಕೆ_ಎಳೆಗಳು(), 1);
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ nstripes = std::min(
        (ದ್ವಿಗುಣ)otot * (ದ್ವಿಗುಣ)std::max<ದೀರ್ಘ ದೀರ್ಘ>(ವೆಚ್ಚ_ಪ್ರತಿ_Elem, 1) * (1./ (ದ್ವಿಗುಣ)(1 << 16)),
        std::min(4.*T, std::max(32., 3.*T)));

    EwBody body;
    body.prog = prog.ದತ್ತಾಂಶ();
    body.ninsn = ninsn;
    body.ಸ್ಥಾನ_Map = ಸ್ಥಾನ_Map.ದತ್ತಾಂಶ();

    // ---- 5. drive: broadcastOp does geometry + 2D tiling + parallelism; the body runs the
    //         frozen program on each tile (temps tile-local, re-pointed from the tile slices).
    //         expandChannels=true => the body always sees single-channel data.
    //
    //         All state the body needs is packed into one POD (EwBody) so the lambda captures a
    //         SINGLE reference: the closure is then one pointer, fits std::function's small-buffer
    //         and never heap-allocates. Inside, the hot fields are copied into locals so the
    //         per-tile/per-insn loops read them from registers, not through the captured pointer. -

    body.buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ = buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ.ದತ್ತಾಂಶ();
    body.ಸ್ಥಾನ_ಬಗೆ = ಸ್ಥಾನ_ಬಗೆ.ದತ್ತಾಂಶ();
    body.nslots = nslots;
    body.nbuffers = nbuffers;
    body.capElems = capElems;

    ಪ್ರಸಾರ_Op_2(arr.ದತ್ತಾಂಶ(), narr, [&](ಸ್ಥಿರಾಂಕ BrTile& tile)
    {
        EwBody& bc = body;
        ಸ್ಥಿರಾಂಕ TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ಸ್ಥಿರಾಂಕ smap  = bc.ಸ್ಥಾನ_Map;
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ skind = bc.ಸ್ಥಾನ_ಬಗೆ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nin = bc.ninsn, nsl = bc.nslots;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ w = tile.ಅಗಲ, h = tile.ಎತ್ತರ;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ಸ್ಥಿರಾಂಕ esz_ಪೂರ್ವಪ್ರತ್ಯಯ = bc.buf_Esz_ಪೂರ್ವಪ್ರತ್ಯಯ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ bw = std::min(w, bc.capElems);
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ಸ್ಥಿರಾಂಕ size_t region = align_ಗಾತ್ರ((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        Auto_ಬಫರ್<uint64_t, (4*1024 + 256)/ಗಾತ್ರ(uint64_t)> tstoreBuf(
            div_ಮೇಲೆ((size_t)esz_ಪೂರ್ವಪ್ರತ್ಯಯ[bc.nbuffers] * region, (ಚಿಹ್ನೆರಹಿತ)ಗಾತ್ರ(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.ದತ್ತಾಂಶ();

        Auto_ಬಫರ್<Br_ತುಂಡು, LOCAL_OPS> ವಾದಗಳು(nsl);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y0 = 0; y0 < h; y0 += bh)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ hf = std::min(bh, h - y0);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x0 = 0; x0 < w; x0 += bw)
            {
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ wf = std::min(bw, w - x0);
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 1; s < nsl; s++)
                {
                    Br_ತುಂಡು& a = ವಾದಗಳು[s];
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].ಆಳ);
                    ಆದರೆ (k == TExpr::ತಾತ್ಕಾಲಿಕ)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)esz_ಪೂರ್ವಪ್ರತ್ಯಯ[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ಸ್ಥಿರಾಂಕ Br_ತುಂಡು& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ n = 0; n < nin; n++)
                {
                    ಸ್ಥಿರಾಂಕ TExpr::Insn& ins = insns[n];
                    ಸ್ಥಿರಾಂಕ Br_ತುಂಡು& a0 = ವಾದಗಳು[ins.arg0]; ಸ್ಥಿರಾಂಕ Br_ತುಂಡು& a1 = ವಾದಗಳು[ins.arg1];
                    ಸ್ಥಿರಾಂಕ Br_ತುಂಡು& a2 = ವಾದಗಳು[ins.arg2]; ಸ್ಥಿರಾಂಕ Br_ತುಂಡು& rr = ವಾದಗಳು[ins.ಫಲಿತಾಂಶ];
                    ಸತತ_Insn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (ಶೂನ್ಯ*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, ನಿಜ, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ಶೂನ್ಯ TExpr::exec(ಸ್ಥಿರಾಂಕ Mat* inputs, Mat* outputs)
{
    Auto_ಬಫರ್<ಸ್ಥಿರಾಂಕ Mat*, 16> ptrs(std::max(ninputs, 1));
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    exec(ptrs.ದತ್ತಾಂಶ(), outputs);
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
ಶೂನ್ಯ make_ದ್ವಿಮಾನ_Arith_ಕಾರ್ಯಕ್ರಮ(TExpr& p, ತುದಿ op, ಪೂರ್ಣಾಂಕ depth0, ಪೂರ್ಣಾಂಕ depth1, ಪೂರ್ಣಾಂಕ rdepth,
                            ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ_ಆಳ, ದ್ವಿಗುಣ scale)
{
    p.clear();
    ಆದರೆ (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        ಆದರೆ (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        ಇಲ್ಲದಿದ್ದರೆ
        {
            rdepth = promoteArith(depth0, depth1);
            ಆದರೆ (op == OP_ABSDIFF) rdepth = absdiff_ಫಲಿತಾಂಶ_ಆಳ(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ masked = ಮುಖವಾಡ_ಆಳ != EW_ಆಳ_NONE;
    ಪೂರ್ಣಾಂಕ sIn0  = p.ಕೂಡಿಸು_ಒಳಹರಿವು(depth0);
    ಪೂರ್ಣಾಂಕ sIn1  = p.ಕೂಡಿಸು_ಒಳಹರಿವು(depth1);
    ಪೂರ್ಣಾಂಕ s_ಮುಖವಾಡ = masked ? p.ಕೂಡಿಸು_ಒಳಹರಿವು(ಮುಖವಾಡ_ಆಳ) : 0;
    ಪೂರ್ಣಾಂಕ s_ಹೊರಹೋಗು  = p.ಕೂಡಿಸು_Output(rdepth);

    ಪೂರ್ಣಾಂಕ r = p.emit_ದ್ವಿಮಾನ(op, sIn0, sIn1, rdepth, Scalar(scale));
    ಆದರೆ (masked)
        p.ಕೂಡಿಸು_Insn(OP_SELECT, s_ಮುಖವಾಡ, r, s_ಹೊರಹೋಗು, s_ಹೊರಹೋಗು);          // dst = mask ? r : dst
    ಇಲ್ಲದಿದ್ದರೆ
        p.move_ಗಮ್ಯ_Output(r, s_ಹೊರಹೋಗು);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
ಶೂನ್ಯ make_ಕೂಡಿಸು_Weighted_ಕಾರ್ಯಕ್ರಮ(TExpr& p, ಪೂರ್ಣಾಂಕ depth0, ಪೂರ್ಣಾಂಕ depth1, ಪೂರ್ಣಾಂಕ rdepth,
                            ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ gamma)
{
    p.clear();
    ಪೂರ್ಣಾಂಕ sA   = p.ಕೂಡಿಸು_ಒಳಹರಿವು(depth0);
    ಪೂರ್ಣಾಂಕ sB   = p.ಕೂಡಿಸು_ಒಳಹರಿವು(depth1);
    ಪೂರ್ಣಾಂಕ s_ಹೊರಹೋಗು = p.ಕೂಡಿಸು_Output(rdepth);
    ಪೂರ್ಣಾಂಕ r = p.emit_ದ್ವಿಮಾನ(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.move_ಗಮ್ಯ_Output(r, s_ಹೊರಹೋಗು);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

ನಾಮಾಕಾಶ {

// --- tokens ------------------------------------------------------------------------------
ಎಣಿಕೆ TokType { T_ಅಂಕೆ, T_ಒಳಹರಿವು, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ನಿಯೋಜಿಸು, T_ಕೊನೆ };

ರಚನೆ Token
{
    TokType type = T_ಕೊನೆ;
    ದ್ವಿಗುಣ num = 0;
    ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_2 = 0;
    std::string ಪಠ್ಯ;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
ರಚನೆ Lexer
{
    std::string_view s;
    size_t pos = 0;

    ಸ್ಪಷ್ಟ Lexer(std::string_view src) : s(src) {}

    ಸ್ಥಿರ ತಾರ್ಕಿಕ is_Ident_ಆರಂಭ(ಅಕ್ಷರ c) { ಹಿಂತಿರುಗಿ std::ಅಕ್ಷರವೇ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)c) || c == '_'; }
    ಸ್ಥಿರ ತಾರ್ಕಿಕ isIdentChar(ಅಕ್ಷರ c)  { ಹಿಂತಿರುಗಿ std::ಅಕ್ಷರವೇ_ಅಂಕಿಯೇ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)c) || c == '_'; }

    Token next()
    {
        ಆಗ (pos < s.size() && std::ಖಾಲಿಯೇ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)s[pos])) pos++;
        Token t;
        ಆದರೆ (pos >= s.size()) { t.type = T_ಕೊನೆ; ಹಿಂತಿರುಗಿ t; }

        ಅಕ್ಷರ c = s[pos];

        ಆದರೆ (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t ಆರಂಭ = pos;
            ಆಗ (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_ಒಳಹರಿವು;
            t.ಒಳಹರಿವು_2 = std::atoi(std::string(s.substr(ಆರಂಭ, pos - ಆರಂಭ)).c_str());
            pos++;                          // consume '}'
            ಹಿಂತಿರುಗಿ t;
        }
        ಆದರೆ (std::ಅಂಕಿಯೇ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::ಅಂಕಿಯೇ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)s[pos + 1])))
        {
            ಅಕ್ಷರ* end = ಶೂನ್ಯಸೂಚಕ;
            std::string num(s.substr(pos));
            t.type = T_ಅಂಕೆ;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            ಹಿಂತಿರುಗಿ t;
        }
        ಆದರೆ (is_Ident_ಆರಂಭ(c))
        {
            size_t ಆರಂಭ = pos;
            ಆಗ (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.ಪಠ್ಯ.assign(s.substr(ಆರಂಭ, pos - ಆರಂಭ));
            ಹಿಂತಿರುಗಿ t;
        }
        ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ '(': pos++; t.type = T_LPAREN; ಹಿಂತಿರುಗಿ t;
        ಸ್ಥಿತಿ ')': pos++; t.type = T_RPAREN; ಹಿಂತಿರುಗಿ t;
        ಸ್ಥಿತಿ ',': pos++; t.type = T_COMMA;  ಹಿಂತಿರುಗಿ t;
        ಸ್ಥಿತಿ ';': pos++; t.type = T_SEMI;   ಹಿಂತಿರುಗಿ t;
        }
        // multi/!single-char operators
        ಸ್ವಯಂಚಾಲಿತ two = [&](ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* op) {
            ಹಿಂತಿರುಗಿ pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        ಆದರೆ (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.ಪಠ್ಯ.assign(s.substr(pos, 2)); pos += 2; ಹಿಂತಿರುಗಿ t; }
        ಆದರೆ (c == '=') { pos++; t.type = T_ನಿಯೋಜಿಸು; ಹಿಂತಿರುಗಿ t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.ಪಠ್ಯ.assign(1, c); pos++;
        ಹಿಂತಿರುಗಿ t;
    }
};

// --- operator / function tables ----------------------------------------------------------
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ binPrec(ಸ್ಥಿರಾಂಕ std::string& op)
{
    ಆದರೆ (op == "**") ಹಿಂತಿರುಗಿ 8;              // power binds tighter than '*'; RIGHT-associative
    ಆದರೆ (op == "*" || op == "/") ಹಿಂತಿರುಗಿ 7;
    ಆದರೆ (op == "+" || op == "-") ಹಿಂತಿರುಗಿ 6;
    ಆದರೆ (op == "<" || op == "<=" || op == ">" || op == ">=") ಹಿಂತಿರುಗಿ 5;
    ಆದರೆ (op == "==" || op == "!=") ಹಿಂತಿರುಗಿ 4;
    ಆದರೆ (op == "&") ಹಿಂತಿರುಗಿ 3;
    ಆದರೆ (op == "^") ಹಿಂತಿರುಗಿ 2;
    ಆದರೆ (op == "|") ಹಿಂತಿರುಗಿ 1;
    ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ತುದಿ binOp(ಸ್ಥಿರಾಂಕ std::string& op)
{
    ಆದರೆ (op == "**") ಹಿಂತಿರುಗಿ OP_POW;
    ಆದರೆ (op == "+")  ಹಿಂತಿರುಗಿ OP_ಕೂಡಿಸು;
    ಆದರೆ (op == "-")  ಹಿಂತಿರುಗಿ OP_SUB;
    ಆದರೆ (op == "*")  ಹಿಂತಿರುಗಿ OP_MUL;
    ಆದರೆ (op == "/")  ಹಿಂತಿರುಗಿ OP_DIV;
    ಆದರೆ (op == "<")  ಹಿಂತಿರುಗಿ OP_CMP_LT;
    ಆದರೆ (op == "<=") ಹಿಂತಿರುಗಿ OP_CMP_LE;
    ಆದರೆ (op == ">")  ಹಿಂತಿರುಗಿ OP_CMP_GT;
    ಆದರೆ (op == ">=") ಹಿಂತಿರುಗಿ OP_CMP_GE;
    ಆದರೆ (op == "==") ಹಿಂತಿರುಗಿ OP_CMP_EQ;
    ಆದರೆ (op == "!=") ಹಿಂತಿರುಗಿ OP_CMP_NE;
    ಆದರೆ (op == "&")  ಹಿಂತಿರುಗಿ OP_AND;
    ಆದರೆ (op == "|")  ಹಿಂತಿರುಗಿ OP_OR;
    ಆದರೆ (op == "^")  ಹಿಂತಿರುಗಿ OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ type_ಆಳ(ಸ್ಥಿರಾಂಕ std::string& name)
{
    ಆದರೆ (name == "float")    ಹಿಂತಿರುಗಿ CV_32F;
    ಆದರೆ (name == "double")   ಹಿಂತಿರುಗಿ CV_64F;
    ಆದರೆ (name == "half" || name == "float16") ಹಿಂತಿರುಗಿ CV_16F;
    ಆದರೆ (name == "bfloat16") ಹಿಂತಿರುಗಿ CV_16BF;
    ಆದರೆ (name == "uint8")    ಹಿಂತಿರುಗಿ CV_8U;
    ಆದರೆ (name == "int8")     ಹಿಂತಿರುಗಿ CV_8S;
    ಆದರೆ (name == "uint16")   ಹಿಂತಿರುಗಿ CV_16U;
    ಆದರೆ (name == "int16")    ಹಿಂತಿರುಗಿ CV_16S;
    ಆದರೆ (name == "uint32")   ಹಿಂತಿರುಗಿ CV_32U;
    ಆದರೆ (name == "int32")    ಹಿಂತಿರುಗಿ CV_32S;
    ಆದರೆ (name == "uint64")   ಹಿಂತಿರುಗಿ CV_64U;
    ಆದರೆ (name == "int64")    ಹಿಂತಿರುಗಿ CV_64S;
    ಹಿಂತಿರುಗಿ -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
ಸ್ಥಿರ ತುದಿ fnOp(ಸ್ಥಿರಾಂಕ std::string& name, ಪೂರ್ಣಾಂಕ& arity)
{
    ರಚನೆ E { ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* n; ತುದಿ op; };
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ E& e : unary)  ಆದರೆ (name == e.n) { arity = 1; ಹಿಂತಿರುಗಿ e.op; }
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ E& e : binary) ಆದರೆ (name == e.n) { arity = 2; ಹಿಂತಿರುಗಿ e.op; }
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ E& e : tern)   ಆದರೆ (name == e.n) { arity = 3; ಹಿಂತಿರುಗಿ e.op; }
    arity = 0; ಹಿಂತಿರುಗಿ OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
ರಚನೆ Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ಒಳಹರಿವು_ಸ್ಥಾನ;                 // slot id of each external input (precreated)
    ಪೂರ್ಣಾಂಕ ninputs;
    std::map<std::string, ಪೂರ್ಣಾಂಕ> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& ಅಭಿವ್ಯಕ್ತಿ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* islot, ಪೂರ್ಣಾಂಕ nin)
        : lex(src), e(ಅಭಿವ್ಯಕ್ತಿ), ಒಳಹರಿವು_ಸ್ಥಾನ(islot), ninputs(nin) { cur = lex.next(); }

    ಶೂನ್ಯ advance() { cur = lex.next(); }
    ತಾರ್ಕಿಕ isOp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* op) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ cur.type == T_OP && cur.ಪಠ್ಯ == op; }

    ಶೂನ್ಯ expect(TokType t, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    ತಾರ್ಕಿಕ isFlexConst(ಪೂರ್ಣಾಂಕ ಸ್ಥಾನ) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ e.arginfo[ಸ್ಥಾನ].ಬಗೆ == TExpr::CONST && e.arginfo[ಸ್ಥಾನ].ಆಳ == EW_ಆಳ_NONE;
    }

    ಪೂರ್ಣಾಂಕ parsePrimary()
    {
        ಆದರೆ (cur.type == T_ಅಂಕೆ)   { ಪೂರ್ಣಾಂಕ s = e.ಕೂಡಿಸು_Const(EW_ಆಳ_NONE, Scalar(cur.num), 1); advance(); ಹಿಂತಿರುಗಿ s; }
        ಆದರೆ (cur.type == T_ಒಳಹರಿವು) { ಪೂರ್ಣಾಂಕ idx = cur.ಒಳಹರಿವು_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   ಹಿಂತಿರುಗಿ ಒಳಹರಿವು_ಸ್ಥಾನ[idx]; }
        ಆದರೆ (cur.type == T_LPAREN){ advance(); ಪೂರ್ಣಾಂಕ x = parseTernary(); expect(T_RPAREN, "expected ')'"); ಹಿಂತಿರುಗಿ x; }
        ಆದರೆ (cur.type == T_IDENT)
        {
            std::string name = cur.ಪಠ್ಯ; advance();
            ಆದರೆ (cur.type != T_LPAREN)             // variable reference
            {
                ಸ್ವಯಂಚಾಲಿತ it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                ಹಿಂತಿರುಗಿ it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<ಪೂರ್ಣಾಂಕ, 8> ವಾದಗಳು = {}; ಪೂರ್ಣಾಂಕ nargs = 0;
            ಆದರೆ (cur.type != T_RPAREN)
            {
                ವಾದಗಳು[nargs++] = parseTernary();
                ಆಗ (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (ಪೂರ್ಣಾಂಕ)ವಾದಗಳು.size() && "ew::expression: too many arguments");
                    ವಾದಗಳು[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            ಪೂರ್ಣಾಂಕ td = type_ಆಳ(name);
            ಆದರೆ (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                ಪೂರ್ಣಾಂಕ s = ವಾದಗಳು[0];
                ಹಿಂತಿರುಗಿ isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybe_ಕೂಡಿಸು_Cast(s, td);
            }

            ಪೂರ್ಣಾಂಕ arity = 0; ತುದಿ op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            ಆದರೆ (arity == 1) ಹಿಂತಿರುಗಿ e.emitUnary(op, ವಾದಗಳು[0]);
            ಆದರೆ (arity == 2) ಹಿಂತಿರುಗಿ e.emit_ದ್ವಿಮಾನ(op, ವಾದಗಳು[0], ವಾದಗಳು[1]);
            ಹಿಂತಿರುಗಿ e.emitTernary(op, ವಾದಗಳು[0], ವಾದಗಳು[1], ವಾದಗಳು[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    ಪೂರ್ಣಾಂಕ parseUnary()
    {
        ಆದರೆ (isOp("-") || isOp("!"))
        {
            std::string op = cur.ಪಠ್ಯ; advance();
            ಪೂರ್ಣಾಂಕ ಕಾರ್ಯಾಂಶ = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            ಆದರೆ (op == "-" && isFlexConst(ಕಾರ್ಯಾಂಶ))
            {
                Auto_ಬಫರ್<ದ್ವಿಗುಣ, MAX_LOCAL_CN> v; ಪೂರ್ಣಾಂಕ cn = constDoubles(e, ಕಾರ್ಯಾಂಶ, v);
                Scalar neg; ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                ಹಿಂತಿರುಗಿ e.ಕೂಡಿಸು_Const(EW_ಆಳ_NONE, neg, std::min(cn, 4));
            }
            ಹಿಂತಿರುಗಿ e.emitUnary(op == "-" ? OP_NEG : OP_NOT, ಕಾರ್ಯಾಂಶ);
        }
        ಹಿಂತಿರುಗಿ parsePrimary();
    }

    ಪೂರ್ಣಾಂಕ parse_ಅಭಿವ್ಯಕ್ತಿ(ಪೂರ್ಣಾಂಕ minPrec)
    {
        ಪೂರ್ಣಾಂಕ ಎಡ = parseUnary();
        ಆಗ (cur.type == T_OP)
        {
            ಪೂರ್ಣಾಂಕ p = binPrec(cur.ಪಠ್ಯ);
            ಆದರೆ (p < minPrec) ನಿಲ್ಲಿಸು;
            std::string op = cur.ಪಠ್ಯ; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            ಪೂರ್ಣಾಂಕ ಬಲ = parse_ಅಭಿವ್ಯಕ್ತಿ(op == "**" ? p : p + 1);
            ಎಡ = e.emit_ದ್ವಿಮಾನ(binOp(op), ಎಡ, ಬಲ);
        }
        ಹಿಂತಿರುಗಿ ಎಡ;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    ಪೂರ್ಣಾಂಕ parseTernary()
    {
        ಪೂರ್ಣಾಂಕ cond = parse_ಅಭಿವ್ಯಕ್ತಿ(0);
        ಆದರೆ (!isOp("?")) ಹಿಂತಿರುಗಿ cond;
        advance();
        ಪೂರ್ಣಾಂಕ thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        ಪೂರ್ಣಾಂಕ elsev = parseTernary();               // right-associative chain
        ಹಿಂತಿರುಗಿ e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    ಶೂನ್ಯ parse_ಫಲಿತಾಂಶ()
    {
        ಆದರೆ (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); ಪೂರ್ಣಾಂಕ n_ತಾತ್ಕಾಲಿಕ = e.ntemps;
            advance();
            ಪೂರ್ಣಾಂಕ e0 = parseTernary();
            ಆದರೆ (cur.type == T_COMMA)
            {
                e.output(e0);
                ಆಗ (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                ಹಿಂತಿರುಗಿ;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = n_ತಾತ್ಕಾಲಿಕ;
        }
        e.output(parseTernary());
    }

    ಶೂನ್ಯ parse()
    {
        ಆಗ (ನಿಜ)
        {
            ಆದರೆ (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.ಪಠ್ಯ; advance();
                ಆದರೆ (cur.type == T_ನಿಯೋಜಿಸು)
                {
                    advance();
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನ = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[ಸ್ಥಾನ].pinned = ನಿಜ;
                    env[name] = ಸ್ಥಾನ;
                    expect(T_SEMI, "expected ';' after assignment");
                    ಮುಂದುವರಿ;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parse_ಫಲಿತಾಂಶ();
            ಆದರೆ (cur.type == T_SEMI) advance();
            ನಿಲ್ಲಿಸು;
        }
        CV_Assert(cur.type == T_ಕೊನೆ && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

ನಾಮಾಕಾಶ cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
ಶೂನ್ಯ texpr(ಸ್ಥಿರಾಂಕ std::string& ಅಭಿವ್ಯಕ್ತಿ, ಒಳಹರಿವು_Array_ದ_Arrays _inputs, Output_Array_ದ_Arrays _outputs)
{
    ಬಳಸಿ ನಾಮಾಕಾಶ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.ಬಗೆ() == ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT);
    ಸ್ಥಿರಾಂಕ std::vector<Mat>& inps = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)_inputs.getObj();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ninputs = (ಪೂರ್ಣಾಂಕ)inps.size();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> islot(std::max(ninputs, 1));
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ninputs; i++) islot[i] = e.ಕೂಡಿಸು_ಒಳಹರಿವು(inps[i].ಆಳ());
    Parser(ಅಭಿವ್ಯಕ್ತಿ, e, islot.ದತ್ತಾಂಶ(), ninputs).parse();
    e.compile();

    ಸ್ವಯಂಚಾಲಿತ ಬಗೆ = _outputs.ಬಗೆ();
    ಆದರೆ (ಬಗೆ == ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.ದತ್ತಾಂಶ(), outs.ದತ್ತಾಂಶ());
    } ಇಲ್ಲದಿದ್ದರೆ {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
