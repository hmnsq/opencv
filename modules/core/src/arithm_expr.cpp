// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm_expr.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <climits>
#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdlib>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstring>
#ਅਨ੍ਤਰ੍ਭਾਵ <map>
#ਅਨ੍ਤਰ੍ਭਾਵ <ostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ ew {

// ============================ op metadata (was ew_op.cpp) ============================

ਅਚਲ ਅੱਖਰ* opName(ਸਿਖਰ op)
{
    ਚੋਣ (op)
    {
    ਹਾਲਤ OP_NOP:           ਵਾਪਸ "nop";
    ਹਾਲਤ OP_NEG:           ਵਾਪਸ "neg";
    ਹਾਲਤ OP_ABS:           ਵਾਪਸ "abs";
    ਹਾਲਤ OP_NOT:           ਵਾਪਸ "not";
    ਹਾਲਤ OP_SQRT:          ਵਾਪਸ "sqrt";
    ਹਾਲਤ OP_EXP:           ਵਾਪਸ "exp";
    ਹਾਲਤ OP_LOG:           ਵਾਪਸ "log";
    ਹਾਲਤ OP_SIN:           ਵਾਪਸ "sin";
    ਹਾਲਤ OP_COS:           ਵਾਪਸ "cos";
    ਹਾਲਤ OP_TANH:          ਵਾਪਸ "tanh";
    ਹਾਲਤ OP_ERF:           ਵਾਪਸ "erf";
    ਹਾਲਤ OP_RELU:          ਵਾਪਸ "relu";
    ਹਾਲਤ OP_CAST:          ਵਾਪਸ "cast";
    ਹਾਲਤ OP_ਜੋੜੋ:           ਵਾਪਸ "add";
    ਹਾਲਤ OP_SUB:           ਵਾਪਸ "sub";
    ਹਾਲਤ OP_MUL:           ਵਾਪਸ "mul";
    ਹਾਲਤ OP_DIV:           ਵਾਪਸ "div";
    ਹਾਲਤ OP_POW:           ਵਾਪਸ "pow";
    ਹਾਲਤ OP_MIN:           ਵਾਪਸ "min";
    ਹਾਲਤ OP_MAX:           ਵਾਪਸ "max";
    ਹਾਲਤ OP_ABSDIFF:       ਵਾਪਸ "absdiff";
    ਹਾਲਤ OP_ADDW:          ਵਾਪਸ "addWeighted";
    ਹਾਲਤ OP_HYPOT:         ਵਾਪਸ "hypot";
    ਹਾਲਤ OP_ATAN2:         ਵਾਪਸ "atan2";
    ਹਾਲਤ OP_AND:           ਵਾਪਸ "and";
    ਹਾਲਤ OP_OR:            ਵਾਪਸ "or";
    ਹਾਲਤ OP_XOR:           ਵਾਪਸ "xor";
    ਹਾਲਤ OP_CMP_EQ:        ਵਾਪਸ "cmp_eq";
    ਹਾਲਤ OP_CMP_NE:        ਵਾਪਸ "cmp_ne";
    ਹਾਲਤ OP_CMP_LT:        ਵਾਪਸ "cmp_lt";
    ਹਾਲਤ OP_CMP_LE:        ਵਾਪਸ "cmp_le";
    ਹਾਲਤ OP_CMP_GT:        ਵਾਪਸ "cmp_gt";
    ਹਾਲਤ OP_CMP_GE:        ਵਾਪਸ "cmp_ge";
    ਹਾਲਤ OP_CLAMP:         ਵਾਪਸ "clamp";
    ਹਾਲਤ OP_SELECT:        ਵਾਪਸ "select";
    ਹਾਲਤ OP_CONVERT_SCALE: ਵਾਪਸ "convert_scale";
    ਮੂਲ:               ਵਾਪਸ "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(ਸਿਖਰ op)
{
    ਚੋਣ (op)
    {
    ਹਾਲਤ OP_AND: ਹਾਲਤ OP_OR: ਹਾਲਤ OP_XOR: ਹਾਲਤ OP_NOT:
        ਵਾਪਸ CAT_BITWISE;
    ਹਾਲਤ OP_CMP_EQ: ਹਾਲਤ OP_CMP_NE: ਹਾਲਤ OP_CMP_LT:
    ਹਾਲਤ OP_CMP_LE: ਹਾਲਤ OP_CMP_GT: ਹਾਲਤ OP_CMP_GE:
        ਵਾਪਸ CAT_COMPARE;
    ਹਾਲਤ OP_SQRT: ਹਾਲਤ OP_EXP: ਹਾਲਤ OP_LOG:
    ਹਾਲਤ OP_SIN: ਹਾਲਤ OP_COS: ਹਾਲਤ OP_TANH: ਹਾਲਤ OP_ERF: ਹਾਲਤ OP_ATAN2: ਹਾਲਤ OP_RELU:
        ਵਾਪਸ CAT_MATH;
    ਹਾਲਤ OP_CAST: ਹਾਲਤ OP_CONVERT_SCALE:
        ਵਾਪਸ CAT_CAST;
    ਹਾਲਤ OP_SELECT:
        ਵਾਪਸ CAT_SELECT;
    ਮੂਲ:
        ਵਾਪਸ CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
ਸਥਿਰ ਤਾਰਕਿਕ is_Float_ਡੂੰਘਾਈ(ਪੂਰਨ d)
{
    ਵਾਪਸ d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
ਸਥਿਰ ਤਾਰਕਿਕ ਡੂੰਘਾਈ_Represents(ਦੁੱਗਣਾ v, ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਜੇ (is_Float_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ)) ਵਾਪਸ ਸੱਚ;
    ਜੇ (v != std::floor(v)) ਵਾਪਸ ਝੂਠ;
    ਚੋਣ (ਡੂੰਘਾਈ)
    {
    ਹਾਲਤ CV_8U:  ਵਾਪਸ v >= 0            && v <= 255;
    ਹਾਲਤ CV_8S:  ਵਾਪਸ v >= -128         && v <= 127;
    ਹਾਲਤ CV_16U: ਵਾਪਸ v >= 0            && v <= 65535;
    ਹਾਲਤ CV_16S: ਵਾਪਸ v >= -32768       && v <= 32767;
    ਹਾਲਤ CV_32U: ਵਾਪਸ v >= 0            && v <= 4294967295.0;
    ਹਾਲਤ CV_32S: ਵਾਪਸ v >= -2147483648.0 && v <= 2147483647.0;
    ਹਾਲਤ CV_64U: ਵਾਪਸ v >= 0            && v <= 18446744073709551615.0;
    ਹਾਲਤ CV_64S: ਵਾਪਸ v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    ਮੂਲ:     ਵਾਪਸ ਝੂਠ;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
ਸਥਿਰ ਖਾਲੀ intRange(ਪੂਰਨ ਡੂੰਘਾਈ, ਦੁੱਗਣਾ& lo, ਦੁੱਗਣਾ& hi)
{
    ਚੋਣ (ਡੂੰਘਾਈ)
    {
    ਹਾਲਤ CV_8U:  lo = 0;                     hi = 255; ਰੋਕੋ;
    ਹਾਲਤ CV_8S:  lo = -128;                  hi = 127; ਰੋਕੋ;
    ਹਾਲਤ CV_16U: lo = 0;                     hi = 65535; ਰੋਕੋ;
    ਹਾਲਤ CV_16S: lo = -32768;                hi = 32767; ਰੋਕੋ;
    ਹਾਲਤ CV_32U: lo = 0;                     hi = 4294967295.0; ਰੋਕੋ;
    ਹਾਲਤ CV_32S: lo = -2147483648.0;         hi = 2147483647.0; ਰੋਕੋ;
    ਹਾਲਤ CV_64U: lo = 0;                     hi = 18446744073709551615.0; ਰੋਕੋ;
    ਹਾਲਤ CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; ਰੋਕੋ;
    ਮੂਲ:     lo = 0;                     hi = 0; ਰੋਕੋ;
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
ਪੂਰਨ promoteArith(ਪੂਰਨ a, ਪੂਰਨ b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ signlut = 0b1001111110100u;
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ fltlut  = 0b1111000000u;

    ਜੇ (a == b) ਵਾਪਸ a;

    ਪੂਰਨ wa = ਪੂਰਨ((leszlut >> (a+1)*3) & 7u), wb = ਪੂਰਨ((leszlut >> (b+1)*3) & 7u);
    ਪੂਰਨ fa = ਪੂਰਨ((fltlut >> (a+1)) & 1u),    fb = ਪੂਰਨ((fltlut >> (b+1)) & 1u);

    ਜੇ (fa + fb == 0)                          // both integer
    {
        ਪੂਰਨ sa = ਪੂਰਨ((signlut >> (a+1)) & 1u), sb = ਪੂਰਨ((signlut >> (b+1)) & 1u);
        ਜੇ (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            ਵਾਪਸ wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        ਪੂਰਨ rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr ਪੂਰਨ ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        ਵਾਪਸ (ilut >> rw*5) & CV_MAT_ਡੂੰਘਾਈ_ਨਕਾਬ;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    ਪੂਰਨ maxw = std::max(wa, wb);
    ਜੇ (maxw == 2 && fa + fb == 1)
        ਵਾਪਸ fa*a + (1 - fa)*b;
    ਵਾਪਸ CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
ਪੂਰਨ absdiff_ਨਤੀਜਾ_ਡੂੰਘਾਈ(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਚੋਣ (ਡੂੰਘਾਈ)
    {
    ਹਾਲਤ CV_8S:  ਵਾਪਸ CV_8U;
    ਹਾਲਤ CV_16S: ਵਾਪਸ CV_16U;
    ਹਾਲਤ CV_32S: ਵਾਪਸ CV_32U;
    ਹਾਲਤ CV_64S: ਵਾਪਸ CV_64U;
    ਮੂਲ:     ਵਾਪਸ ਡੂੰਘਾਈ;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
ਸਥਿਰ ਪੂਰਨ promote2(ਪੂਰਨ a, ਪੂਰਨ b)
{
    ਜੇ (a == b) ਵਾਪਸ a;

    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ lbits = 3, lmask = (1u << lbits) - 1u;
    ਅਚਲ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    ਬਿਨਾਨਿਸ਼ਾਨੀ pr_a = ਬਿਨਾਨਿਸ਼ਾਨੀ((typelut >> (a*lbits)) & lmask);
    ਬਿਨਾਨਿਸ਼ਾਨੀ pr_b = ਬਿਨਾਨਿਸ਼ਾਨੀ((typelut >> (b*lbits)) & lmask);
    ਬਿਨਾਨਿਸ਼ਾਨੀ max_pr = std::max(pr_a, pr_b);
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ dbits = CV_CN_ਖਿਸਕਾਓ, dmask = (1u << dbits) - 1u;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ਵਾਪਸ ਪੂਰਨ((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
ਸਥਿਰ ਪੂਰਨ promoteBitwise(ਪੂਰਨ a, ਪੂਰਨ b)
{
    ਜੇ (a == EW_ਡੂੰਘਾਈ_NONE) ਵਾਪਸ b;
    ਜੇ (b == EW_ਡੂੰਘਾਈ_NONE) ਵਾਪਸ a;
    ਜੇ (a == b) ਵਾਪਸ a;
    ਵਾਪਸ CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
ਸਥਿਰ ਪੂਰਨ ਸੁਰੱਖਿਅਤ_Wide(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ lbits = 3, lmask = (1u << lbits) - 1u;
    ਅਚਲ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    ਬਿਨਾਨਿਸ਼ਾਨੀ pr_ਡੂੰਘਾਈ = ਬਿਨਾਨਿਸ਼ਾਨੀ((typelut >> (ਡੂੰਘਾਈ*lbits)) & lmask);
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ dbits = CV_CN_ਖਿਸਕਾਓ, dmask = (1u << dbits) - 1u;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    ਵਾਪਸ ਪੂਰਨ((ctypelut >> (pr_ਡੂੰਘਾਈ*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
ਗਿਣਤੀ { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
ਸਥਿਰ size_t appendConstBuf(Auto_ਬਫਰ<uint64_t, 16>& constbuf, ਪੂਰਨ srcdepth, ਅਚਲ ਖਾਲੀ* ਅੰਕੜੇ, ਪੂਰਨ channels)
{
    ਅਚਲ ਪੂਰਨ cn = std::max(1, channels);
    ਅਚਲ size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ਅਚਲ size_t nu64 = (nbytes + ਆਕਾਰ(uint64_t) - 1) / ਆਕਾਰ(uint64_t);
    ਅਚਲ size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.ਅੰਕੜੇ() + ofs);
    ਜੇ (ਅੰਕੜੇ) ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, ਅੰਕੜੇ, nbytes); ਨਹੀਂਤਾਂ memset(dst, 0, nbytes);
    ਵਾਪਸ ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
ਸਥਿਰ ਪੂਰਨ constDoubles(ਅਚਲ TExpr& e, ਪੂਰਨ s, Auto_ਬਫਰ<ਦੁੱਗਣਾ, MAX_LOCAL_CN>& buf)
{
    ਅਚਲ TExpr::Arg& a = e.arginfo[s];
    ਅਚਲ ਪੂਰਨ cn = std::max(1, a.channels);
    buf.resize(cn);
    ਅਚਲ uchar* src = (ਅਚਲ uchar*)(e.constbuf.ਅੰਕੜੇ() + a.constofs);
    ਜੇ (a.srcdepth == CV_64F)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf.ਅੰਕੜੇ(), src, (size_t)cn * ਆਕਾਰ(ਦੁੱਗਣਾ));   // common (Scalar / parsed literal)
    ਨਹੀਂਤਾਂ
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, ਖਾਲੀ_ਸੂਚਕ, 0, (uchar*)buf.ਅੰਕੜੇ(), 0, ਮਾਪ(cn, 1), ਖਾਲੀ_ਸੂਚਕ);
    ਵਾਪਸ cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isFlexConst(ਅਚਲ TExpr& e, ਪੂਰਨ s)
{
    ਵਾਪਸ e.arginfo[s].ਕਿਸਮ == TExpr::CONST && e.arginfo[s].ਡੂੰਘਾਈ == EW_ਡੂੰਘਾਈ_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
ਸਥਿਰ ਤਾਰਕਿਕ scalar_Const_ਮੁੱਲ(ਅਚਲ TExpr& e, ਪੂਰਨ s, ਦੁੱਗਣਾ& v)
{
    ਅਚਲ TExpr::Arg& a = e.arginfo[s];
    ਜੇ (a.ਕਿਸਮ != TExpr::CONST || std::max(1, a.channels) != 1) ਵਾਪਸ ਝੂਠ;
    Auto_ਬਫਰ<ਦੁੱਗਣਾ, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    ਵਾਪਸ ਸੱਚ;
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
ਸਥਿਰ ਤਾਰਕਿਕ ਮੇਲ_Scaled_Chain(ਅਚਲ TExpr& e, ਪੂਰਨ end, ਪੂਰਨ t,
                             ਪੂਰਨ& arr_ਥਾਂ, ਦੁੱਗਣਾ& scale, ਪੂਰਨ& ਸ਼ੁਰੂ_Idx)
{
    ਜੇ (end < 0) ਵਾਪਸ ਝੂਠ;
    ਪੂਰਨ j = end, mul_ਨਤੀਜਾ = t;

    // optional trailing narrowing cast
    ਜੇ (e.prog[j].op == OP_CAST && e.prog[j].ਨਤੀਜਾ == t)
    {
        mul_ਨਤੀਜਾ = e.prog[j].arg0;
        ਜੇ (--j < 0) ਵਾਪਸ ਝੂਠ;
    }

    ਅਚਲ TExpr::Insn& mul = e.prog[j];
    ਜੇ (mul.op != OP_MUL || mul.ਨਤੀਜਾ != mul_ਨਤੀਜਾ || mul.params[0] != 1.0) ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ arr;
    ਜੇ      (scalar_Const_ਮੁੱਲ(e, mul.arg1, scale)) arr = mul.arg0;
    ਨਹੀਂਤਾਂ ਜੇ (scalar_Const_ਮੁੱਲ(e, mul.arg0, scale)) arr = mul.arg1;
    ਨਹੀਂਤਾਂ ਵਾਪਸ ਝੂਠ;                                  // array*array is not an addWeighted term
    ਸ਼ੁਰੂ_Idx = j;

    // optional leading widening cast feeding the multiply
    ਜੇ (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].ਨਤੀਜਾ == arr &&
        e.arginfo[arr].ਕਿਸਮ == TExpr::ਆਰਜ਼ੀ)
    {
        arr = e.prog[j-1].arg0;
        ਸ਼ੁਰੂ_Idx = j - 1;
    }

    ਜੇ (e.arginfo[arr].ਕਿਸਮ == TExpr::CONST) ਵਾਪਸ ਝੂਠ;   // scalar*scalar is folded elsewhere
    arr_ਥਾਂ = arr;
    ਵਾਪਸ ਸੱਚ;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
ਸਥਿਰ ਤਾਰਕਿਕ const_ਸਮਾਉਂਦਾ(ਅਚਲ TExpr& e, ਪੂਰਨ s, ਪੂਰਨ d)
{
    ਜੇ (!isFlexConst(e, s)) ਵਾਪਸ ਸੱਚ;
    Auto_ਬਫਰ<ਦੁੱਗਣਾ, MAX_LOCAL_CN> v;
    ਪੂਰਨ cn = constDoubles(e, s, v);
    ਲਈ (ਪੂਰਨ ch = 0; ch < cn; ch++)
        ਜੇ (!ਡੂੰਘਾਈ_Represents(v[ch], d)) ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
ਪੂਰਨ TExpr::emit_ਦੁਆਧਾਰੀ(ਸਿਖਰ op, ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ rdepth, ਅਚਲ Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    ਜੇ (op == OP_ADDW)
    {
        ਪੂਰਨ Tt = arginfo[a].ਡੂੰਘਾਈ;
        ਜੇ (arginfo[a].ਡੂੰਘਾਈ != arginfo[b].ਡੂੰਘਾਈ)
        {
            Tt = promoteArith(arginfo[a].ਡੂੰਘਾਈ, arginfo[b].ਡੂੰਘਾਈ);
            a = maybe_ਜੋੜੋ_Cast(a, Tt); b = maybe_ਜੋੜੋ_Cast(b, Tt);
        }
        ਜੇ (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        ਜੇ (rdepth == EW_ਡੂੰਘਾਈ_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_ਡੂੰਘਾਈ_NONE, rdepth);
        ਪੂਰਨ ਬਾਹਰਲਾ_D = rdepth;
        ਜੇ (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            ਬਾਹਰਲਾ_D = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_ਡੂੰਘਾਈ_NONE, ਬਾਹਰਲਾ_D);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ਅਚਲ ਪੂਰਨ out = ਜੋੜੋ_ਆਰਜ਼ੀ(ਬਾਹਰਲਾ_D);
        ਜੋੜੋ_Insn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        ਜੇ (ਬਾਹਰਲਾ_D == rdepth) ਵਾਪਸ out;
        ਅਚਲ ਪੂਰਨ out2 = ਜੋੜੋ_ਆਰਜ਼ੀ(rdepth);
        ਜੋੜੋ_Insn(OP_CAST, out, 0, 0, out2);
        ਵਾਪਸ out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    ਜੇ (op == OP_ਜੋੜੋ)
    {
        ਅਚਲ ਪੂਰਨ n = (ਪੂਰਨ)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        ਪੂਰਨ x = 0, y = 0, ਸ਼ੁਰੂ_A = 0, ਸ਼ੁਰੂ_B = 0;
        ਦੁੱਗਣਾ alpha = 0, beta = 0;
        ਜੇ (n >= 2 && a != b &&
            arginfo[a].ਕਿਸਮ == ਆਰਜ਼ੀ && !arginfo[a].pinned &&
            arginfo[b].ਕਿਸਮ == ਆਰਜ਼ੀ && !arginfo[b].pinned &&
            ਮੇਲ_Scaled_Chain(*ਇਹ, n - 1, b, y, beta, ਸ਼ੁਰੂ_B) &&
            ਮੇਲ_Scaled_Chain(*ਇਹ, ਸ਼ੁਰੂ_B - 1, a, x, alpha, ਸ਼ੁਰੂ_A) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].ਡੂੰਘਾਈ != CV_Bool && arginfo[y].ਡੂੰਘਾਈ != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            ਪੂਰਨ retired = 0, lowest = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
            ਲਈ (ਪੂਰਨ i = ਸ਼ੁਰੂ_A; i < n; i++)
            {
                ਅਚਲ ਪੂਰਨ r = prog[i].ਨਤੀਜਾ;
                ਜੇ (arginfo[r].ਕਿਸਮ != ਆਰਜ਼ੀ || arginfo[r].pinned) { retired = -1; ਰੋਕੋ; }
                lowest = std::min(lowest, arginfo[r].ਸੂਚਕਾਂਕ);
                retired++;
            }
            ਜੇ (retired > 0 && lowest == ntemps - retired)
            {
                ਲਈ (ਪੂਰਨ i = ਸ਼ੁਰੂ_A; i < n; i++) arginfo[prog[i].ਨਤੀਜਾ].ਕਿਸਮ = NONE;
                prog.resize(ਸ਼ੁਰੂ_A);
                ntemps -= retired;
                ਵਾਪਸ emit_ਦੁਆਧਾਰੀ(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            ਪੂਰਨ acc = 0; ਦੁੱਗਣਾ gamma = 0;
            ਜੇ      (scalar_Const_ਮੁੱਲ(*ਇਹ, b, gamma)) acc = a;
            ਨਹੀਂਤਾਂ ਜੇ (scalar_Const_ਮੁੱਲ(*ਇਹ, a, gamma)) acc = b;
            ਜੇ (acc != 0 && n >= 1 &&
                arginfo[acc].ਕਿਸਮ == ਆਰਜ਼ੀ && !arginfo[acc].pinned &&
                arginfo[acc].ਸੂਚਕਾਂਕ == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].ਨਤੀਜਾ == acc &&
                (rdepth == EW_ਡੂੰਘਾਈ_NONE || rdepth == arginfo[acc].ਡੂੰਘਾਈ))
            {
                prog[n-1].params[2] += gamma;
                ਵਾਪਸ acc;
            }
        }
    }

    ਅਚਲ ਪੂਰਨ nd0 = isFlexConst(*ਇਹ, a) ? EW_ਡੂੰਘਾਈ_NONE : arginfo[a].ਡੂੰਘਾਈ;
    ਅਚਲ ਪੂਰਨ nd1 = isFlexConst(*ਇਹ, b) ? EW_ਡੂੰਘਾਈ_NONE : arginfo[b].ਡੂੰਘਾਈ;
    ਅਚਲ ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    ਪੂਰਨ ਨਤੀਜਾ = rdepth;
    ਜੇ (ਨਤੀਜਾ == EW_ਡੂੰਘਾਈ_NONE)
    {
        ਨਤੀਜਾ = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        ਜੇ (ਨਤੀਜਾ == EW_ਡੂੰਘਾਈ_NONE) ਨਤੀਜਾ = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    ਪੂਰਨ ਅਧਾਰ;
    ਜੇ (nd0 == EW_ਡੂੰਘਾਈ_NONE && nd1 == EW_ਡੂੰਘਾਈ_NONE) ਅਧਾਰ = ਨਤੀਜਾ;
    ਨਹੀਂਤਾਂ ਜੇ (nd0 == EW_ਡੂੰਘਾਈ_NONE) ਅਧਾਰ = nd1;
    ਨਹੀਂਤਾਂ ਜੇ (nd1 == EW_ਡੂੰਘਾਈ_NONE) ਅਧਾਰ = nd0;
    ਨਹੀਂਤਾਂ ਅਧਾਰ = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    ਜੇ (cat == CAT_COMPARE && nd0 != EW_ਡੂੰਘਾਈ_NONE && !is_Float_ਡੂੰਘਾਈ(nd0) &&
        isFlexConst(*ਇਹ, b) && !const_ਸਮਾਉਂਦਾ(*ਇਹ, b, ਅਧਾਰ))
    {
        ਦੁੱਗਣਾ lo, hi; intRange(nd0, lo, hi);
        ਜੇ (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            ਗਿਣਤੀ { REAL, CFALSE, CTRUE };
            Auto_ਬਫਰ<ਦੁੱਗਣਾ, MAX_LOCAL_CN> tv; ਪੂਰਨ cn = constDoubles(*ਇਹ, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            ਪੂਰਨ ਕਿਸਮ[4] = {}; ਦੁੱਗਣਾ bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ਅਚਲ ਸਿਖਰ fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
            {
                ਅਚਲ ਦੁੱਗਣਾ t = tv[c]; ਪੂਰਨ k; ਦੁੱਗਣਾ B = t;
                ਚੋਣ (op)
                {
                ਹਾਲਤ OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ਰੋਕੋ;
                ਹਾਲਤ OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ਰੋਕੋ;
                ਹਾਲਤ OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ਰੋਕੋ;
                ਹਾਲਤ OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ਰੋਕੋ;
                ਹਾਲਤ OP_CMP_EQ: k = ਡੂੰਘਾਈ_Represents(t, nd0) ? REAL : CFALSE; ਰੋਕੋ;   // a==non-rep -> false
                ਮੂਲ:        k = ਡੂੰਘਾਈ_Represents(t, nd0) ? REAL : CTRUE;  ਰੋਕੋ;   // NE: a!=non-rep -> true
                }
                ਕਿਸਮ[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            ਸਵੈਚਾਲਿਤ ucOp  = [&](ਪੂਰਨ c){ ਵਾਪਸ ਕਿਸਮ[c]==REAL ? fam : ਕਿਸਮ[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            ਸਵੈਚਾਲਿਤ ucThr = [&](ਪੂਰਨ c){ ਵਾਪਸ ਕਿਸਮ[c]==REAL ? bound[c]
                                          : (ਕਿਸਮ[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ਅਚਲ ਸਿਖਰ u0 = ucOp(0);
            ਤਾਰਕਿਕ ਇਕਸਾਰ = ਸੱਚ;
            ਲਈ (ਪੂਰਨ c = 1; c < cn && ਇਕਸਾਰ; c++) ਇਕਸਾਰ = (ucOp(c) == u0);
            ਜੇ (ਇਕਸਾਰ)
            {
                Scalar s; ਲਈ (ਪੂਰਨ c = 0; c < cn; c++) s[c] = ucThr(c);
                b = ਜੋੜੋ_Const(EW_ਡੂੰਘਾਈ_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            ਨਹੀਂਤਾਂ
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; ਪੂਰਨ patchFlags = 0;
                ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                {
                    ts[c] = ਕਿਸਮ[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ਅਚਲ ਪੂਰਨ mbits = ਕਿਸਮ[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ਅਚਲ ਪੂਰਨ vbits = ਕਿਸਮ[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_ਖਿਸਕਾਓ + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, ਅਧਾਰ, ਅਧਾਰ, EW_ਡੂੰਘਾਈ_NONE, ਨਤੀਜਾ);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ਅਚਲ ਪੂਰਨ out = ਜੋੜੋ_ਆਰਜ਼ੀ(ਨਤੀਜਾ);
                ਜੋੜੋ_Insn(fam, a, ਜੋੜੋ_Const(ਅਧਾਰ, ts, cn), 0, out, kern);
                ਵਾਪਸ out;
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
    ਪੂਰਨ ਡੂੰਘਾਈ, wide;
    ਤਾਰਕਿਕ divGuard = ਝੂਠ;
    ਚੋਣ (op)
    {
    ਹਾਲਤ OP_MIN: ਹਾਲਤ OP_MAX:
    ਹਾਲਤ OP_AND: ਹਾਲਤ OP_OR: ਹਾਲਤ OP_XOR:
        ਡੂੰਘਾਈ = ਨਤੀਜਾ; wide = ਨਤੀਜਾ; ਰੋਕੋ;
    ਹਾਲਤ OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        ਪੂਰਨ common = (nd0 == EW_ਡੂੰਘਾਈ_NONE) ? nd1
                   : (nd1 == EW_ਡੂੰਘਾਈ_NONE) ? nd0 : promoteArith(nd0, nd1);
        ਜੇ (common == EW_ਡੂੰਘਾਈ_NONE) common = ਨਤੀਜਾ;          // both operands flexible consts
        ਡੂੰਘਾਈ = common;
        wide  = absdiff_ਨਤੀਜਾ_ਡੂੰਘਾਈ(common);
        ਜੇ (rdepth == EW_ਡੂੰਘਾਈ_NONE) ਨਤੀਜਾ = wide;            // auto result: unsigned same width
        ਰੋਕੋ;
    }
    ਹਾਲਤ OP_CMP_EQ: ਹਾਲਤ OP_CMP_NE: ਹਾਲਤ OP_CMP_LT:
    ਹਾਲਤ OP_CMP_LE: ਹਾਲਤ OP_CMP_GT: ਹਾਲਤ OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        ਡੂੰਘਾਈ = ਅਧਾਰ; wide = CV_64F; ਰੋਕੋ;
    ਹਾਲਤ OP_MUL: ਹਾਲਤ OP_DIV:
    {
        ਡੂੰਘਾਈ = ਅਧਾਰ;
        ਅਚਲ ਤਾਰਕਿਕ w = (ਡੂੰਘਾਈ==CV_32U || ਡੂੰਘਾਈ==CV_32S || ਡੂੰਘਾਈ==CV_64U ||
                        ਡੂੰਘਾਈ==CV_64S || ਡੂੰਘਾਈ==CV_64F);
        wide = w ? CV_64F : CV_32F;
        ਜੇ (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            ਸਵੈਚਾਲਿਤ int_ਸੰਕਾਰਜ = [&](ਪੂਰਨ s, ਪੂਰਨ nd) {
                ਜੇ (nd != EW_ਡੂੰਘਾਈ_NONE) ਵਾਪਸ !is_Float_ਡੂੰਘਾਈ(nd);
                Auto_ਬਫਰ<ਦੁੱਗਣਾ, MAX_LOCAL_CN> v; constDoubles(*ਇਹ, s, v);
                ਵਾਪਸ v[0] == std::floor(v[0]); };
            divGuard = int_ਸੰਕਾਰਜ(a, nd0) && int_ਸੰਕਾਰਜ(b, nd1);
        }
        ਰੋਕੋ;
    }
    ਹਾਲਤ OP_POW: ਹਾਲਤ OP_HYPOT: ਹਾਲਤ OP_ATAN2:
        ਡੂੰਘਾਈ = (ਅਧਾਰ == CV_64F) ? CV_64F : CV_32F; wide = ਡੂੰਘਾਈ; ਰੋਕੋ;
    ਮੂਲ:        // OP_ADD, OP_SUB
        ਡੂੰਘਾਈ = ਅਧਾਰ; wide = ਸੁਰੱਖਿਅਤ_Wide(ਡੂੰਘਾਈ); ਰੋਕੋ;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    ਸਵੈਚਾਲਿਤ fracConst = [&](ਪੂਰਨ s) {
        ਜੇ (!isFlexConst(*ਇਹ, s)) ਵਾਪਸ ਝੂਠ;
        Auto_ਬਫਰ<ਦੁੱਗਣਾ, MAX_LOCAL_CN> v; ਪੂਰਨ n = constDoubles(*ਇਹ, s, v);
        ਲਈ (ਪੂਰਨ ch = 0; ch < n; ch++)
            ਜੇ (v[ch] != std::floor(v[ch])) ਵਾਪਸ ਸੱਚ;
        ਵਾਪਸ ਝੂਠ;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    ਜੇ (cat != CAT_BITWISE && (!const_ਸਮਾਉਂਦਾ(*ਇਹ, a, ਡੂੰਘਾਈ) || !const_ਸਮਾਉਂਦਾ(*ਇਹ, b, ਡੂੰਘਾਈ)))
    {
        ਡੂੰਘਾਈ = wide;
        ਅਚਲ ਤਾਰਕਿਕ frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        ਜੇ (frac && !is_Float_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ) && op != OP_MIN && op != OP_MAX)
            ਡੂੰਘਾਈ = wide = (ਨਤੀਜਾ == CV_64F || ਅਧਾਰ == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        ਜੇ (frac && (op == OP_MUL || op == OP_DIV) && ਡੂੰਘਾਈ == CV_32F)
            ਡੂੰਘਾਈ = wide = CV_64F;
    }

    ਪੂਰਨ s0 = isFlexConst(*ਇਹ, a) ? typedConstFrom(a, ਡੂੰਘਾਈ) : a;
    ਪੂਰਨ s1 = isFlexConst(*ਇਹ, b) ? typedConstFrom(b, ਡੂੰਘਾਈ) : b;
    ਪੂਰਨ c0 = maybe_ਜੋੜੋ_Cast(s0, ਡੂੰਘਾਈ), c1 = maybe_ਜੋੜੋ_Cast(s1, ਡੂੰਘਾਈ);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    ਸਵੈਚਾਲਿਤ resolve = [&](ਪੂਰਨ rd) {
        ਵਾਪਸ op == OP_DIV ? getDivFunc(ਡੂੰਘਾਈ, rd, divGuard)
                            : getElemwiseFunc(op, ਡੂੰਘਾਈ, ਡੂੰਘਾਈ, EW_ਡੂੰਘਾਈ_NONE, rd);
    };
    TKernel k = resolve(ਨਤੀਜਾ);
    ਜੇ (k.fptr)
    {
        ਪੂਰਨ res = ਜੋੜੋ_ਆਰਜ਼ੀ(ਨਤੀਜਾ);
        ਜੋੜੋ_Insn(op, c0, c1, 0, res, k, params);
        ਵਾਪਸ res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ਪੂਰਨ w = ਜੋੜੋ_ਆਰਜ਼ੀ(wide);
    ਜੋੜੋ_Insn(op, c0, c1, 0, w, k, params);
    ਵਾਪਸ maybe_ਜੋੜੋ_Cast(w, ਨਤੀਜਾ);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
ਪੂਰਨ TExpr::emitUnary(ਸਿਖਰ op, ਪੂਰਨ a, ਪੂਰਨ rdepth, ਅਚਲ Scalar& params)
{
    ਅਚਲ ਪੂਰਨ nd = isFlexConst(*ਇਹ, a) ? EW_ਡੂੰਘਾਈ_NONE : arginfo[a].ਡੂੰਘਾਈ;
    ਅਚਲ ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    ਜੇ (op == OP_NEG)
        ਵਾਪਸ emit_ਦੁਆਧਾਰੀ(OP_SUB, ਜੋੜੋ_Const(EW_ਡੂੰਘਾਈ_NONE, Scalar(0.), 1), a, rdepth, params);
    ਜੇ (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        ਜੇ (!prog.empty() && arginfo[a].ਕਿਸਮ == ਆਰਜ਼ੀ && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().ਨਤੀਜਾ == a &&
            arginfo[a].ਸੂਚਕਾਂਕ == ntemps - 1)
        {
            ਅਚਲ ਪੂਰਨ x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].ਕਿਸਮ = NONE;
            ntemps--;
            ਵਾਪਸ emit_ਦੁਆਧਾਰੀ(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        ਵਾਪਸ emit_ਦੁਆਧਾਰੀ(OP_ABSDIFF, a, ਜੋੜੋ_Const(EW_ਡੂੰਘਾਈ_NONE, Scalar(0.), 1), rdepth, params);
    }

    ਪੂਰਨ ਨਤੀਜਾ = rdepth;
    ਜੇ (ਨਤੀਜਾ == EW_ਡੂੰਘਾਈ_NONE)
    {
        ਚੋਣ (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        ਹਾਲਤ CAT_MATH:    ਨਤੀਜਾ = is_Float_ਡੂੰਘਾਈ(nd) ? nd : CV_32F; ਰੋਕੋ;
        ਹਾਲਤ CAT_BITWISE: ਨਤੀਜਾ = (nd == EW_ਡੂੰਘਾਈ_NONE) ? CV_32S : nd; ਰੋਕੋ;  // NOT
        ਮੂਲ:          ਨਤੀਜਾ = (nd == EW_ਡੂੰਘਾਈ_NONE) ? CV_32F : nd; ਰੋਕੋ;  // NEG, ABS
        }
    }

    ਪੂਰਨ ਡੂੰਘਾਈ, wide;
    ਚੋਣ (cat)
    {
    ਹਾਲਤ CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        ਡੂੰਘਾਈ = (is_Float_ਡੂੰਘਾਈ(nd) && nd == ਨਤੀਜਾ) ? nd
              : (nd == CV_64F || ਨਤੀਜਾ == CV_64F) ? CV_64F : CV_32F;
        wide = ਡੂੰਘਾਈ;
        ਰੋਕੋ;
    ਹਾਲਤ CAT_BITWISE: ਡੂੰਘਾਈ = ਨਤੀਜਾ; wide = ਨਤੀਜਾ; ਰੋਕੋ;
    ਮੂਲ:          ਡੂੰਘਾਈ = (nd == EW_ਡੂੰਘਾਈ_NONE) ? ਨਤੀਜਾ : nd; wide = ਨਤੀਜਾ; ਰੋਕੋ;  // NEG/ABS
    }

    ਜੇ (!const_ਸਮਾਉਂਦਾ(*ਇਹ, a, ਡੂੰਘਾਈ)) ਡੂੰਘਾਈ = (cat == CAT_MATH) ? ਡੂੰਘਾਈ : CV_32F;
    ਪੂਰਨ s0 = isFlexConst(*ਇਹ, a) ? typedConstFrom(a, ਡੂੰਘਾਈ) : a;
    ਪੂਰਨ c0 = maybe_ਜੋੜੋ_Cast(s0, ਡੂੰਘਾਈ);

    TKernel k = getElemwiseFunc(op, ਡੂੰਘਾਈ, EW_ਡੂੰਘਾਈ_NONE, EW_ਡੂੰਘਾਈ_NONE, ਨਤੀਜਾ);
    ਜੇ (k.fptr)
    {
        ਪੂਰਨ res = ਜੋੜੋ_ਆਰਜ਼ੀ(ਨਤੀਜਾ);
        ਜੋੜੋ_Insn(op, c0, 0, 0, res, k, params);
        ਵਾਪਸ res;
    }
    k = getElemwiseFunc(op, ਡੂੰਘਾਈ, EW_ਡੂੰਘਾਈ_NONE, EW_ਡੂੰਘਾਈ_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ਪੂਰਨ w = ਜੋੜੋ_ਆਰਜ਼ੀ(wide);
    ਜੋੜੋ_Insn(op, c0, 0, 0, w, k, params);
    ਵਾਪਸ maybe_ਜੋੜੋ_Cast(w, ਨਤੀਜਾ);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
ਪੂਰਨ TExpr::emitTernary(ਸਿਖਰ op, ਪੂਰਨ a, ਪੂਰਨ b, ਪੂਰਨ c, ਪੂਰਨ rdepth)
{
    ਸਵੈਚਾਲਿਤ dep = [&](ਪੂਰਨ s) { ਵਾਪਸ isFlexConst(*ਇਹ, s) ? EW_ਡੂੰਘਾਈ_NONE : arginfo[s].ਡੂੰਘਾਈ; };

    ਜੇ (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        ਜੇ (isFlexConst(*ਇਹ, a) || CV_ELEM_SIZE1(arginfo[a].ਡੂੰਘਾਈ) != 1)
            a = emit_ਦੁਆਧਾਰੀ(OP_CMP_NE, a, ਜੋੜੋ_Const(EW_ਡੂੰਘਾਈ_NONE, Scalar(0.), 1), CV_8U, Scalar());
        ਪੂਰਨ ਨਤੀਜਾ = rdepth != EW_ਡੂੰਘਾਈ_NONE ? rdepth : promoteArith(dep(b), dep(c));
        ਜੇ (ਨਤੀਜਾ == EW_ਡੂੰਘਾਈ_NONE) ਨਤੀਜਾ = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        ਪੂਰਨ sb = isFlexConst(*ਇਹ, b) ? typedConstFrom(b, ਨਤੀਜਾ) : maybe_ਜੋੜੋ_Cast(b, ਨਤੀਜਾ);
        ਪੂਰਨ sc = isFlexConst(*ਇਹ, c) ? typedConstFrom(c, ਨਤੀਜਾ) : maybe_ਜੋੜੋ_Cast(c, ਨਤੀਜਾ);
        ਪੂਰਨ res = ਜੋੜੋ_ਆਰਜ਼ੀ(ਨਤੀਜਾ);
        ਜੋੜੋ_Insn(OP_SELECT, a, sb, sc, res);
        ਵਾਪਸ res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    ਪੂਰਨ ਨਤੀਜਾ = rdepth != EW_ਡੂੰਘਾਈ_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    ਜੇ (ਨਤੀਜਾ == EW_ਡੂੰਘਾਈ_NONE) ਨਤੀਜਾ = CV_32F;
    ਸਵੈਚਾਲਿਤ typed = [&](ਪੂਰਨ s) {
        ਵਾਪਸ isFlexConst(*ਇਹ, s) ? typedConstFrom(s, ਨਤੀਜਾ) : maybe_ਜੋੜੋ_Cast(s, ਨਤੀਜਾ);
    };
    ਪੂਰਨ c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, ਨਤੀਜਾ, ਨਤੀਜਾ, ਨਤੀਜਾ, ਨਤੀਜਾ);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ਪੂਰਨ res = ਜੋੜੋ_ਆਰਜ਼ੀ(ਨਤੀਜਾ);
    ਜੋੜੋ_Insn(op, c0, c1, c2, res, k);
    ਵਾਪਸ res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
ਪੂਰਨ TExpr::move_ਮੰਜ਼ਿਲ_Output(ਪੂਰਨ ਆਰਜ਼ੀ_2, ਪੂਰਨ out)
{
    ਅਚਲ ਪੂਰਨ ninsn = (ਪੂਰਨ)prog.size();
    ਪੂਰਨ ਉਤਪਾਦਕ = -1;
    ਤਾਰਕਿਕ ਵਰਤਿਆ_As_Arg = ਝੂਠ;
    ਲਈ (ਪੂਰਨ i = 0; i < ninsn; i++)
    {
        ਅਚਲ TExpr::Insn& ins = prog[i];
        ਜੇ (ins.ਨਤੀਜਾ == ਆਰਜ਼ੀ_2) ਉਤਪਾਦਕ = i;
        ਜੇ (ins.arg0 == ਆਰਜ਼ੀ_2 || ins.arg1 == ਆਰਜ਼ੀ_2 || ins.arg2 == ਆਰਜ਼ੀ_2) ਵਰਤਿਆ_As_Arg = ਸੱਚ;
    }
    ਜੇ (arginfo[ਆਰਜ਼ੀ_2].ਕਿਸਮ == ਆਰਜ਼ੀ && ਉਤਪਾਦਕ >= 0 && !ਵਰਤਿਆ_As_Arg && !arginfo[ਆਰਜ਼ੀ_2].pinned &&
        arginfo[ਆਰਜ਼ੀ_2].ਡੂੰਘਾਈ == arginfo[out].ਡੂੰਘਾਈ)
    {
        // MOVE semantics: redirect `temp`'s single producer to write `out` directly, then leave the
        // source slot EMPTY - reclassify it to NONE. A NONE slot gets no physical buffer (compile's
        // liveness only walks TEMP slots) and is skipped everywhere in exec, so no stale dead-TEMP slot
        // remains (which, buffer or not, would still cost per-tile setup in the general path) - and no
        // slot removal / index renumbering is needed. This keeps single-op programs at zero temps =>
        // compile()'s no-temp early-out (zero heap traffic) still fires. Every caller moves the result
        // it JUST emitted, so `temp` is always the last-created temp - decrement ntemps to retire its
        // index and keep the temp indices dense (0..ntemps-1) for compile()'s liveness arrays.
        CV_Assert(arginfo[ਆਰਜ਼ੀ_2].ਸੂਚਕਾਂਕ == ntemps - 1);
        prog[ਉਤਪਾਦਕ].ਨਤੀਜਾ = out;
        arginfo[ਆਰਜ਼ੀ_2].ਕਿਸਮ = NONE;
        ntemps--;
        ਵਾਪਸ out;
    }
    ਜੋੜੋ_Insn(OP_CAST, ਆਰਜ਼ੀ_2, 0, 0, out);                     // same/different depth copy into `out`
    ਵਾਪਸ out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
ਪੂਰਨ TExpr::output(ਪੂਰਨ ਜੜ੍ਹ_ਥਾਂ)
{
    ਵਾਪਸ move_ਮੰਜ਼ਿਲ_Output(ਜੜ੍ਹ_ਥਾਂ, ਜੋੜੋ_Output(arginfo[ਜੜ੍ਹ_ਥਾਂ].ਡੂੰਘਾਈ));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
ਖਾਲੀ TExpr::compile()
{
    ਅਚਲ ਪੂਰਨ ninsn = (ਪੂਰਨ)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ਅਚਲ ਪੂਰਨ nslots = (ਪੂਰਨ)arginfo.size();
    nconsts = 0;
    ਲਈ (ਪੂਰਨ s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        ਜੇ (a.ਕਿਸਮ != CONST || a.ਡੂੰਘਾਈ == EW_ਡੂੰਘਾਈ_NONE) ਜਾਰੀ;
        ਅਚਲ ਪੂਰਨ cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.ਡੂੰਘਾਈ;
        ਅਚਲ size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        Auto_ਬਫਰ<uint64_t, ਆਕਾਰ(uint64_t)> srcbuf(div_ਉੱਪਰ((size_t)cn * sesz, (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਕਾਰ(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.ਅੰਕੜੇ();
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(srcbytes, (ਅਚਲ uchar*)(constbuf.ਅੰਕੜੇ() + a.constofs), (size_t)cn * sesz);
        ਅਚਲ size_t ofs = appendConstBuf(constbuf, dd, ਖਾਲੀ_ਸੂਚਕ, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.ਅੰਕੜੇ() + ofs);
        ਜੇ (sd == dd) ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, srcbytes, (size_t)cn * desz);
        ਨਹੀਂਤਾਂ getConvertFunc(sd, dd)(srcbytes, 0, ਖਾਲੀ_ਸੂਚਕ, 0, dst, 0, ਮਾਪ(cn, 1), ਖਾਲੀ_ਸੂਚਕ);
        a.constofs = ofs; a.srcdepth = dd;
        nconsts++;
    }

    // ---- liveness: pack temps into a minimal set of reusable physical buffers. A program with no
    //      temps (single-op add/sub/mul/...) needs none of this - early out with ZERO heap traffic,
    //      so building such a program (the dominant cv::add-style call) allocates nothing. The temp
    //      case uses stack-backed AutoBuffers (inline for typical small expressions). ----
    nbuffers = 0;
    buf_Esz_ਅਗੇਤਰ.resize(1); buf_Esz_ਅਗੇਤਰ[0] = 0;
    capElems = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;                       // no temps => exec runs each tile in one block, no scratch
    ਜੇ (ntemps == 0) ਵਾਪਸ;

    Auto_ਬਫਰ<ਪੂਰਨ, 32> ਆਰਜ਼ੀ_ਦਾ_ਥਾਂ(nslots);
    ਲਈ (ਪੂਰਨ s = 0; s < nslots; s++) ਆਰਜ਼ੀ_ਦਾ_ਥਾਂ[s] = -1;
    ਲਈ (ਪੂਰਨ s = 1; s < nslots; s++)
        ਜੇ (arginfo[s].ਕਿਸਮ == ਆਰਜ਼ੀ) ਆਰਜ਼ੀ_ਦਾ_ਥਾਂ[s] = arginfo[s].ਸੂਚਕਾਂਕ;

    Auto_ਬਫਰ<ਪੂਰਨ, 16> ਆਖਰੀ_Use(ntemps);
    ਲਈ (ਪੂਰਨ t = 0; t < ntemps; t++) ਆਖਰੀ_Use[t] = -1;
    ਲਈ (ਪੂਰਨ i = 0; i < ninsn; i++)
    {
        ਅਚਲ TExpr::Insn& ins = prog[i];
        ਪੂਰਨ as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        ਲਈ (ਪੂਰਨ k = 0; k < 3; k++)
            ਜੇ (ਆਰਜ਼ੀ_ਦਾ_ਥਾਂ[as[k]] >= 0) ਆਖਰੀ_Use[ਆਰਜ਼ੀ_ਦਾ_ਥਾਂ[as[k]]] = i;
    }

    ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ.resize(ntemps);
    ਲਈ (ਪੂਰਨ t = 0; t < ntemps; t++) ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ[t] = -1;
    Auto_ਬਫਰ<ਪੂਰਨ, 16> freeBufs(ntemps);
    ਪੂਰਨ nfree = 0, nbuf = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < ninsn; i++)
    {
        ਅਚਲ TExpr::Insn& ins = prog[i];
        ਪੂਰਨ rt = ਆਰਜ਼ੀ_ਦਾ_ਥਾਂ[ins.ਨਤੀਜਾ];
        ਜੇ (rt >= 0 && ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ[rt] < 0)
            ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        ਪੂਰਨ as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        ਲਈ (ਪੂਰਨ k = 0; k < 3; k++)
        {
            ਪੂਰਨ t = ਆਰਜ਼ੀ_ਦਾ_ਥਾਂ[as[k]];
            ਜੇ (t >= 0 && ਆਖਰੀ_Use[t] == i)
            {
                freeBufs[nfree++] = ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ[t];
                ਆਖਰੀ_Use[t] = -1;   // the same temp may be several args of this insn ("d*d") -
                                   // release its buffer once, or freeBufs overflows/double-frees
            }
        }
    }

    nbuffers = nbuf;

    // Byte layout of the physical temp buffers, per output element: prefix sums of each buffer's
    // max element size. bufEszPrefix[b]*region = buffer b's byte offset in the scratch; [nbuffers] =
    // total temp bytes/element. Precomputed here (depths are build-time) so exec()/its fast path
    // never recompute it per call, and the fast path can size its scratch in O(1).
    buf_Esz_ਅਗੇਤਰ.resize(nbuffers + 1);
    ਲਈ (ਪੂਰਨ b = 0; b <= nbuffers; b++) buf_Esz_ਅਗੇਤਰ[b] = 0;
    ਲਈ (ਪੂਰਨ s = 1; s < nslots; s++)
        ਜੇ (arginfo[s].ਕਿਸਮ == ਆਰਜ਼ੀ)
        {
            ਪੂਰਨ b = ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ[arginfo[s].ਸੂਚਕਾਂਕ], e = (ਪੂਰਨ)CV_ELEM_SIZE1(arginfo[s].ਡੂੰਘਾਈ);
            ਜੇ (e > buf_Esz_ਅਗੇਤਰ[b + 1]) buf_Esz_ਅਗੇਤਰ[b + 1] = e;     // max elem size in buffer b
        }
    ਲਈ (ਪੂਰਨ b = 0; b < nbuffers; b++) buf_Esz_ਅਗੇਤਰ[b + 1] += buf_Esz_ਅਗੇਤਰ[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ਅਚਲ ਪੂਰਨ ਕੁੱਲ_Esz = buf_Esz_ਅਗੇਤਰ[nbuffers];
    capElems = ਕੁੱਲ_Esz > 0 ? std::max(64, (4 * 1024) / ਕੁੱਲ_Esz) : ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
ਸਥਿਰ ਖਾਲੀ matLogical(ਅਚਲ Mat& m, MatShape& shp, Ew_ਕਦਮਾਂ& ਕਦਮ, ਪੂਰਨ& esz1)
{
    esz1 = (ਪੂਰਨ)m.elemSize1();
    ਪੂਰਨ nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    ਲਈ (ਪੂਰਨ i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        ਕਦਮ[i] = m.ਕਦਮ[i] / esz1;
    }
    shp[nd] = cn;
    ਕਦਮ[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
ਸਥਿਰ ਤਾਰਕਿਕ ਪ੍ਰਸਾਰਣ_Shape(ਅਚਲ MatShape* shps, ਪੂਰਨ K, MatShape& out)
{
    ਪੂਰਨ nd = 0;
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
    {
        ਅਚਲ MatShape& s = shps[k];
        ਪੂਰਨ off = nd - s.dims;
        ਲਈ (ਪੂਰਨ i = 0; i < s.dims; i++)
        {
            ਪੂਰਨ d = s[i], &o = out[off + i];
            ਜੇ (o == 1) o = d;
            ਨਹੀਂਤਾਂ ਜੇ (d != 1 && d != o) ਵਾਪਸ ਝੂਠ;
        }
    }
    ਵਾਪਸ ਸੱਚ;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
ਸਥਿਰ ਪੂਰਨ op_ਲਾਗਤ(ਸਿਖਰ op)
{
    ਚੋਣ (op)
    {
    ਹਾਲਤ OP_ਜੋੜੋ: ਹਾਲਤ OP_SUB: ਹਾਲਤ OP_MUL: ਹਾਲਤ OP_MIN: ਹਾਲਤ OP_MAX:
    ਹਾਲਤ OP_ABSDIFF: ਹਾਲਤ OP_AND: ਹਾਲਤ OP_OR: ਹਾਲਤ OP_XOR: ਹਾਲਤ OP_NOT:
    ਹਾਲਤ OP_NEG: ਹਾਲਤ OP_ABS: ਹਾਲਤ OP_CAST: ਹਾਲਤ OP_RELU: ਹਾਲਤ OP_SELECT:
    ਹਾਲਤ OP_CMP_EQ: ਹਾਲਤ OP_CMP_NE: ਹਾਲਤ OP_CMP_LT:
    ਹਾਲਤ OP_CMP_LE: ਹਾਲਤ OP_CMP_GT: ਹਾਲਤ OP_CMP_GE: ਵਾਪਸ 1;
    ਹਾਲਤ OP_DIV: ਹਾਲਤ OP_SQRT: ਹਾਲਤ OP_HYPOT: ਹਾਲਤ OP_CONVERT_SCALE: ਵਾਪਸ 3;
    ਹਾਲਤ OP_SIN: ਹਾਲਤ OP_COS: ਹਾਲਤ OP_TANH: ਹਾਲਤ OP_ERF: ਹਾਲਤ OP_ATAN2:
    ਹਾਲਤ OP_EXP: ਹਾਲਤ OP_LOG: ਹਾਲਤ OP_POW:           ਵਾਪਸ 6;
    ਮੂਲ:                                         ਵਾਪਸ 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
ਖਾਲੀ TExpr::clear()
{
    prog.allocate(0);
    arginfo.allocate(0);
    ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
ਖਾਲੀ TExpr::dump(std::ostream& os) ਅਚਲ
{
    ਸਵੈਚਾਲਿਤ dn = [](ਪੂਰਨ d) -> ਅਚਲ ਅੱਖਰ* {
        ਚੋਣ (d) {
        ਹਾਲਤ EW_ਡੂੰਘਾਈ_NONE: ਵਾਪਸ "flex";
        ਹਾਲਤ CV_8U:  ਵਾਪਸ "u8";  ਹਾਲਤ CV_8S:  ਵਾਪਸ "s8";
        ਹਾਲਤ CV_16U: ਵਾਪਸ "u16"; ਹਾਲਤ CV_16S: ਵਾਪਸ "s16";
        ਹਾਲਤ CV_32U: ਵਾਪਸ "u32"; ਹਾਲਤ CV_32S: ਵਾਪਸ "s32";
        ਹਾਲਤ CV_64U: ਵਾਪਸ "u64"; ਹਾਲਤ CV_64S: ਵਾਪਸ "s64";
        ਹਾਲਤ CV_16F: ਵਾਪਸ "f16"; ਹਾਲਤ CV_16BF:ਵਾਪਸ "bf16";
        ਹਾਲਤ CV_32F: ਵਾਪਸ "f32"; ਹਾਲਤ CV_64F: ਵਾਪਸ "f64";
        ਹਾਲਤ CV_Bool:ਵਾਪਸ "bool";
        ਮੂਲ:     ਵਾਪਸ "?"; }
    };
    ਸਵੈਚਾਲਿਤ kn = [](Arg_ਕਿਸਮ k) -> ਅਚਲ ਅੱਖਰ* {
        ਚੋਣ (k) { ਹਾਲਤ NONE: ਵਾਪਸ "none"; ਹਾਲਤ ਨਿਵੇਸ਼: ਵਾਪਸ "in"; ਹਾਲਤ CONST: ਵਾਪਸ "const";
                     ਹਾਲਤ ਆਰਜ਼ੀ: ਵਾਪਸ "temp"; ਹਾਲਤ OUTPUT: ਵਾਪਸ "out"; } ਵਾਪਸ "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (ਪੂਰਨ)prog.size() << "\n";
    os << "  slots:\n";
    ਲਈ (ਪੂਰਨ s = 0; s < (ਪੂਰਨ)arginfo.size(); s++) {
        ਅਚਲ Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.ਕਿਸਮ);
        ਜੇ (a.ਕਿਸਮ != NONE) os << " " << dn(a.ਡੂੰਘਾਈ);
        ਜੇ (a.ਕਿਸਮ == ਨਿਵੇਸ਼ || a.ਕਿਸਮ == OUTPUT || a.ਕਿਸਮ == ਆਰਜ਼ੀ) os << " idx=" << a.ਸੂਚਕਾਂਕ;
        ਜੇ (a.ਕਿਸਮ == CONST) {
            ਅਚਲ ਪੂਰਨ cn = std::max(1, a.channels);
            Auto_ਬਫਰ<ਦੁੱਗਣਾ, MAX_LOCAL_CN> v(cn);
            ਅਚਲ uchar* src = (ਅਚਲ uchar*)(constbuf.ਅੰਕੜੇ() + a.constofs);
            ਜੇ (a.srcdepth == CV_64F) ਸਮ੍ਰਿਤੀ_ਨਕਲ(v.ਅੰਕੜੇ(), src, (size_t)cn * ਆਕਾਰ(ਦੁੱਗਣਾ));
            ਨਹੀਂਤਾਂ getConvertFunc(a.srcdepth, CV_64F)(src, 0, ਖਾਲੀ_ਸੂਚਕ, 0, (uchar*)v.ਅੰਕੜੇ(), 0, ਮਾਪ(cn, 1), ਖਾਲੀ_ਸੂਚਕ);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            ਲਈ (ਪੂਰਨ c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)prog.size(); i++) {
        ਅਚਲ Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        ਜੇ (ins.arg1) os << ", " << ins.arg1;
        ਜੇ (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.ਨਤੀਜਾ;
        ਜੇ (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        ਜੇ (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        ਜੇ (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
ਪੂਰਨ TExpr::ਜੋੜੋ_ਨਿਵੇਸ਼(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    Arg a; a.ਕਿਸਮ = ਨਿਵੇਸ਼; a.ਡੂੰਘਾਈ = ਡੂੰਘਾਈ; a.ਸੂਚਕਾਂਕ = ninputs++;
    arginfo.push_back(a); ਵਾਪਸ (ਪੂਰਨ)arginfo.size() - 1;
}

ਪੂਰਨ TExpr::ਜੋੜੋ_Output(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    Arg a; a.ਕਿਸਮ = OUTPUT; a.ਡੂੰਘਾਈ = ਡੂੰਘਾਈ; a.ਸੂਚਕਾਂਕ = noutputs++;
    arginfo.push_back(a); ਵਾਪਸ (ਪੂਰਨ)arginfo.size() - 1;
}

ਪੂਰਨ TExpr::ਜੋੜੋ_ਆਰਜ਼ੀ(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    Arg a; a.ਕਿਸਮ = ਆਰਜ਼ੀ; a.ਡੂੰਘਾਈ = ਡੂੰਘਾਈ; a.ਸੂਚਕਾਂਕ = ntemps++;
    arginfo.push_back(a); ਵਾਪਸ (ਪੂਰਨ)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
ਪੂਰਨ TExpr::ਜੋੜੋ_Const(ਪੂਰਨ ਡੂੰਘਾਈ, ਅਚਲ Scalar& v, ਪੂਰਨ channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.ਕਿਸਮ = CONST; a.ਡੂੰਘਾਈ = ਡੂੰਘਾਈ; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); ਵਾਪਸ (ਪੂਰਨ)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
ਪੂਰਨ TExpr::ਜੋੜੋ_Const(ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ srcdepth, ਅਚਲ ਖਾਲੀ* ਅੰਕੜੇ, ਪੂਰਨ channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.ਕਿਸਮ = CONST; a.ਡੂੰਘਾਈ = ਡੂੰਘਾਈ; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, ਅੰਕੜੇ, channels);
    arginfo.push_back(a); ਵਾਪਸ (ਪੂਰਨ)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
ਪੂਰਨ TExpr::typedConstFrom(ਪੂਰਨ src_ਥਾਂ, ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਅਚਲ Arg& src = arginfo[src_ਥਾਂ];
    Arg a; a.ਕਿਸਮ = CONST; a.ਡੂੰਘਾਈ = ਡੂੰਘਾਈ; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); ਵਾਪਸ (ਪੂਰਨ)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
ਪੂਰਨ TExpr::ਜੋੜੋ_Insn(ਸਿਖਰ op, ਪੂਰਨ a0, ਪੂਰਨ a1, ਪੂਰਨ a2, ਪੂਰਨ ਨਤੀਜਾ, ਅਚਲ TKernel& kernel, ਅਚਲ Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.ਨਤੀਜਾ = ਨਤੀਜਾ;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    ਵਾਪਸ (ਪੂਰਨ)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
ਪੂਰਨ TExpr::ਜੋੜੋ_Insn(ਸਿਖਰ op, ਪੂਰਨ a0, ਪੂਰਨ a1, ਪੂਰਨ a2, ਪੂਰਨ ਨਤੀਜਾ, ਅਚਲ Scalar& params)
{
    ਪੂਰਨ d0 = a0 ? arginfo[a0].ਡੂੰਘਾਈ : EW_ਡੂੰਘਾਈ_NONE;
    ਪੂਰਨ d1 = a1 ? arginfo[a1].ਡੂੰਘਾਈ : EW_ਡੂੰਘਾਈ_NONE;
    ਪੂਰਨ d2 = a2 ? arginfo[a2].ਡੂੰਘਾਈ : EW_ਡੂੰਘਾਈ_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[ਨਤੀਜਾ].ਡੂੰਘਾਈ);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    ਵਾਪਸ ਜੋੜੋ_Insn(op, a0, a1, a2, ਨਤੀਜਾ, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
ਪੂਰਨ TExpr::maybe_ਜੋੜੋ_Cast(ਪੂਰਨ arg, ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਜੇ (arginfo[arg].ਡੂੰਘਾਈ == ਡੂੰਘਾਈ) ਵਾਪਸ arg;
    ਪੂਰਨ t = ਜੋੜੋ_ਆਰਜ਼ੀ(ਡੂੰਘਾਈ);
    ਜੋੜੋ_Insn(OP_CAST, arg, 0, 0, t);
    ਵਾਪਸ t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਲੜੀਵਾਰ_Insn(ਅਚਲ TExpr::Insn& ins,
                           ਅਚਲ ਖਾਲੀ* p0, size_t y0, size_t x0,
                           ਅਚਲ ਖਾਲੀ* p1, size_t y1, size_t x1,
                           ਅਚਲ ਖਾਲੀ* p2, size_t y2, size_t x2,
                           ਖਾਲੀ* pr, size_t yr, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ ਕੋਡ = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(ਕੋਡ >= 0);
}

ਬਣਤਰ EwBody {
    ਅਚਲ TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ਅਚਲ ਪੂਰਨ* ਥਾਂ_Map;
    ਅਚਲ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ* ਥਾਂ_ਕਿਸਮ; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ਅਚਲ ਪੂਰਨ* buf_Esz_ਅਗੇਤਰ;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    ਪੂਰਨ ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

ਖਾਲੀ TExpr::outputShape(ਅਚਲ Mat* ਅਚਲ* inputs, MatShape& spatial, ਪੂਰਨ& channels) ਅਚਲ
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ਅਚਲ ਪੂਰਨ rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    ਤਾਰਕਿਕ sameShape = ninputs >= 1;
    ਲਈ (ਪੂਰਨ i = 1; sameShape && i < ninputs; i++)
    {
        ਅਚਲ Mat& a = *inputs[i];
        ਜੇ (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = ਝੂਠ;
        ਨਹੀਂਤਾਂ ਲਈ (ਪੂਰਨ d = 0; d < a.dims; d++) ਜੇ (a.size[d] != inputs[0]->size[d]) { sameShape = ਝੂਠ; ਰੋਕੋ; }
    }
    ਜੇ (sameShape) { spatial = inputs[0]->size; channels = rcn0; ਵਾਪਸ; }

    Auto_ਬਫਰ<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; Ew_ਕਦਮਾਂ ਕਦਮ; ਪੂਰਨ esz1;
    ਲਈ (ਪੂਰਨ i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, ਕਦਮ, esz1); bshapes[i] = shp; }
    MatShape ਭਰਿਆ;
    CV_Assert(ਪ੍ਰਸਾਰਣ_Shape(bshapes.ਅੰਕੜੇ(), ninputs, ਭਰਿਆ) && "ew: inputs not broadcast-compatible");
    ਅਚਲ ਪੂਰਨ nd_ਭਰਿਆ = (ਪੂਰਨ)ਭਰਿਆ.size();
    channels = ਭਰਿਆ[nd_ਭਰਿਆ - 1];
    spatial = ਭਰਿਆ; spatial.resize(nd_ਭਰਿਆ - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ਖਾਲੀ TExpr::outputShape(ਅਚਲ Mat* inputs, MatShape& spatial, ਪੂਰਨ& channels) ਅਚਲ
{
    Auto_ਬਫਰ<ਅਚਲ Mat*, 16> ptrs(std::max(ninputs, 1));
    ਲਈ (ਪੂਰਨ i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.ਅੰਕੜੇ(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ਸਥਿਰ constexpr ਪੂਰਨ LOCAL_HDRS = 3;
ਸਥਿਰ constexpr ਪੂਰਨ LOCAL_CONSTS = 4;
ਸਥਿਰ constexpr ਪੂਰਨ LOCAL_OPS = 16;

ਖਾਲੀ TExpr::exec(ਅਚਲ Mat* ਅਚਲ* inputs, Mat* outputs)
{
    ਵਰਤੋਂ BrTile = ਪ੍ਰਸਾਰਣ_Op::Tile;
    ਵਰਤੋਂ Br_ਟੁਕੜਾ = ਪ੍ਰਸਾਰਣ_Op::ਟੁਕੜਾ;
    ਅਚਲ ਪੂਰਨ nslots = (ਪੂਰਨ)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].ਕਿਸਮ == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    ਜੇ (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        ਅਚਲ Mat& r = *inputs[0];
        ਅਚਲ ਪੂਰਨ rcn = r.channels();
        ਤਾਰਕਿਕ ok = r.isContinuous();
        ਲਈ (ਪੂਰਨ i = 1; ok && i < ninputs; i++)
        {
            ਅਚਲ Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        ਲਈ (ਪੂਰਨ s = 1; ok && s < nslots; s++)
            ਜੇ (arginfo[s].ਕਿਸਮ == OUTPUT) ok = ok && outputs[arginfo[s].ਸੂਚਕਾਂਕ].isContinuous();
        ਅਚਲ size_t ਕੁੱਲ = ok ? r.ਕੁੱਲ() * rcn : 0;
        ਜੇ (ok && ਕੁੱਲ == 0)
        {
            ਅਚਲ MatShape rshape = r.size;
            ਲਈ (ਪੂਰਨ s = 1; s < nslots; s++)
                ਜੇ (arginfo[s].ਕਿਸਮ == OUTPUT)
                    outputs[arginfo[s].ਸੂਚਕਾਂਕ].create(rshape, CV_MAKETYPE(arginfo[s].ਡੂੰਘਾਈ, rcn));
            ਵਾਪਸ;
        }
        ਜੇ (ok && ਕੁੱਲ <= EW_FASTPATH_MAX)
        {
            ਅਚਲ MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            ਲਈ (ਪੂਰਨ s = 1; s < nslots; s++)
                ਜੇ (arginfo[s].ਕਿਸਮ == OUTPUT)
                    outputs[arginfo[s].ਸੂਚਕਾਂਕ].create(rshape, CV_MAKETYPE(arginfo[s].ਡੂੰਘਾਈ, rcn));
            ਅਚਲ ਪੂਰਨ ninsn = (ਪੂਰਨ)prog.size();

            ਜੇ (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                ਸਵੈਚਾਲਿਤ ptr_ਦਾ = [&](ਪੂਰਨ s) -> ਖਾਲੀ* {
                    ਜੇ (s <= 0) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
                    ਅਚਲ Arg& ai = arginfo[s];
                    ਜੇ (ai.ਕਿਸਮ == ਨਿਵੇਸ਼)  ਵਾਪਸ (ਖਾਲੀ*)inputs[ai.ਸੂਚਕਾਂਕ]->ਅੰਕੜੇ;
                    ਜੇ (ai.ਕਿਸਮ == OUTPUT) ਵਾਪਸ (ਖਾਲੀ*)outputs[ai.ਸੂਚਕਾਂਕ].ਅੰਕੜੇ;
                    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;   // NONE
                };
                ਅਚਲ ਪੂਰਨ w = (ਪੂਰਨ)ਕੁੱਲ;
                ਲਈ (ਪੂਰਨ n = 0; n < ninsn; n++)
                {
                    ਅਚਲ Insn& ins = prog[n];
                    ਲੜੀਵਾਰ_Insn(ins, ptr_ਦਾ(ins.arg0), 0, 1, ptr_ਦਾ(ins.arg1), 0, 1,
                            ptr_ਦਾ(ins.arg2), 0, 1, ptr_ਦਾ(ins.ਨਤੀਜਾ), 0, w, 1);
                }
                ਵਾਪਸ;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ਅਚਲ ਪੂਰਨ ਕੁੱਲ_Esz = buf_Esz_ਅਗੇਤਰ[nbuffers];
            ਅਚਲ ਪੂਰਨ wf0 = std::min((ਪੂਰਨ)ਕੁੱਲ, capElems);

            ਅਚਲ size_t region = align_ਮਾਪ((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            Auto_ਬਫਰ<uint64_t, (4*1024 + 256)/ਆਕਾਰ(uint64_t)> scratchBuf(
                div_ਉੱਪਰ((size_t)ਕੁੱਲ_Esz * region, (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਕਾਰ(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.ਅੰਕੜੇ();
            ਲਈ (ਪੂਰਨ x0 = 0; x0 < (ਪੂਰਨ)ਕੁੱਲ; x0 += wf0)
            {
                ਅਚਲ ਪੂਰਨ wf = std::min(wf0, (ਪੂਰਨ)ਕੁੱਲ - x0);
                ਸਵੈਚਾਲਿਤ ptr_ਦਾ = [&](ਪੂਰਨ s) -> ਖਾਲੀ* {
                    ਜੇ (s <= 0) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
                    ਅਚਲ Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.ਡੂੰਘਾਈ);
                    ਜੇ (ai.ਕਿਸਮ == ਨਿਵੇਸ਼)  ਵਾਪਸ (uchar*)inputs[ai.ਸੂਚਕਾਂਕ]->ਅੰਕੜੇ  + (size_t)x0 * esz;
                    ਜੇ (ai.ਕਿਸਮ == OUTPUT) ਵਾਪਸ (uchar*)outputs[ai.ਸੂਚਕਾਂਕ].ਅੰਕੜੇ + (size_t)x0 * esz;
                    ਜੇ (ai.ਕਿਸਮ != ਆਰਜ਼ੀ)   ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;                                    // NONE: moved-from
                    ਅਚਲ ਪੂਰਨ b = ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ.empty() ? ai.ਸੂਚਕਾਂਕ : ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ[ai.ਸੂਚਕਾਂਕ];  //ਆਰਜ਼ੀ
                    ਵਾਪਸ scratch + (size_t)buf_Esz_ਅਗੇਤਰ[b] * region;                        // fragment-local
                };
                ਲਈ (ਪੂਰਨ n = 0; n < ninsn; n++)
                {
                    ਅਚਲ Insn& ins = prog[n];
                    ਲੜੀਵਾਰ_Insn(ins, ptr_ਦਾ(ins.arg0), 0, 1, ptr_ਦਾ(ins.arg1), 0, 1,
                            ptr_ਦਾ(ins.arg2), 0, 1, ptr_ਦਾ(ins.ਨਤੀਜਾ), 0, wf, 1);
                }
            }
            ਵਾਪਸ;
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
    ਪੂਰਨ rchannels;
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
    ਤਾਰਕਿਕ inplace = ਝੂਠ;
    ਲਈ (ਪੂਰਨ i = 0; i < ninputs && !inplace; i++)
        ਲਈ (ਪੂਰਨ j = 0; j < noutputs; j++)
            ਜੇ (inputs[i]->ਅੰਕੜੇ && inputs[i]->ਅੰਕੜੇ == outputs[j].ਅੰਕੜੇ) { inplace = ਸੱਚ; ਰੋਕੋ; }
    ਪੂਰਨ nhdrs = inplace ? ninputs : 0;

    Auto_ਬਫਰ<ਅਚਲ Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    Auto_ਬਫਰ<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    Auto_ਬਫਰ<ਅਚਲ Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    Auto_ਬਫਰ<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    Auto_ਬਫਰ<ਪੂਰਨ, LOCAL_OPS> ਥਾਂ_Map(nslots);
    Auto_ਬਫਰ<ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ, LOCAL_OPS> ਥਾਂ_ਕਿਸਮ(nslots);
    ਪੂਰਨ narr = 0, nc = 0;
    ਜੇ (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        ਲਈ (ਪੂਰਨ j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.ਅੰਕੜੇ();
    }
    ਥਾਂ_ਕਿਸਮ[0] = (ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)TExpr::NONE;
    ਲਈ (ਪੂਰਨ s = 1; s < nslots; s++)
    {
        ਅਚਲ TExpr::Arg& ai = arginfo[s];
        ਥਾਂ_ਕਿਸਮ[s] = (ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)ai.ਕਿਸਮ;
        ਜੇ (ai.ਕਿਸਮ == TExpr::ਨਿਵੇਸ਼) {
            ਥਾਂ_Map[s] = narr;
            arr[narr++] = inputs[ai.ਸੂਚਕਾਂਕ];
        }
        ਨਹੀਂਤਾਂ ਜੇ (ai.ਕਿਸਮ == TExpr::OUTPUT) {
            outputs[ai.ਸੂਚਕਾਂਕ].create(spatial, CV_MAKETYPE(ai.ਡੂੰਘਾਈ, rchannels));
            ਥਾਂ_Map[s] = narr; arr[narr++] = &outputs[ai.ਸੂਚਕਾਂਕ];
        }
        ਨਹੀਂਤਾਂ ਜੇ (ai.ਕਿਸਮ == TExpr::CONST && ai.ਡੂੰਘਾਈ == EW_ਡੂੰਘਾਈ_NONE) {
            ਥਾਂ_ਕਿਸਮ[s] = (ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)TExpr::NONE;   // dead flexible literal: never referenced
            ਥਾਂ_Map[s] = 0;
        }
        ਨਹੀਂਤਾਂ ਜੇ (ai.ਕਿਸਮ == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ਅਚਲ ਪੂਰਨ c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.ਡੂੰਘਾਈ, c),
                                  (ਖਾਲੀ*)(constbuf.ਅੰਕੜੇ() + ai.constofs));
            ਥਾਂ_ਕਿਸਮ[s] = (ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ)TExpr::ਨਿਵੇਸ਼;   // to the body it is just an array operand
            ਥਾਂ_Map[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        ਨਹੀਂਤਾਂ ਜੇ (ai.ਕਿਸਮ == TExpr::ਆਰਜ਼ੀ) {
            ਥਾਂ_Map[s] = ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ.empty() ? ai.ਸੂਚਕਾਂਕ : ਬਫਰ_ਦਾ_ਆਰਜ਼ੀ[ai.ਸੂਚਕਾਂਕ];  // physical buffer id
        }
        ਨਹੀਂਤਾਂ {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            ਥਾਂ_Map[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ਅਚਲ ਪੂਰਨ ninsn = (ਪੂਰਨ)prog.size();
    ਲੰਮਾ ਲੰਮਾ ਲਾਗਤ_ਪ੍ਰਤੀ_Elem = 0;
    ਲਈ (ਪੂਰਨ n = 0; n < ninsn; n++)
        ਲਾਗਤ_ਪ੍ਰਤੀ_Elem += op_ਲਾਗਤ(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    ਲੰਮਾ ਲੰਮਾ otot = (ਲੰਮਾ ਲੰਮਾ)rchannels;
    ਲਈ (ਪੂਰਨ d = 0; d < (ਪੂਰਨ)spatial.size(); d++) otot *= spatial[d];
    ਜੇ (otot == 0)
        ਵਾਪਸ;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ਅਚਲ ਦੁੱਗਣਾ T = (ਦੁੱਗਣਾ)std::max(get_ਅੰਕ_ਧਾਗੇ(), 1);
    ਅਚਲ ਦੁੱਗਣਾ nstripes = std::min(
        (ਦੁੱਗਣਾ)otot * (ਦੁੱਗਣਾ)std::max<ਲੰਮਾ ਲੰਮਾ>(ਲਾਗਤ_ਪ੍ਰਤੀ_Elem, 1) * (1./ (ਦੁੱਗਣਾ)(1 << 16)),
        std::min(4.*T, std::max(32., 3.*T)));

    EwBody body;
    body.prog = prog.ਅੰਕੜੇ();
    body.ninsn = ninsn;
    body.ਥਾਂ_Map = ਥਾਂ_Map.ਅੰਕੜੇ();

    // ---- 5. drive: broadcastOp does geometry + 2D tiling + parallelism; the body runs the
    //         frozen program on each tile (temps tile-local, re-pointed from the tile slices).
    //         expandChannels=true => the body always sees single-channel data.
    //
    //         All state the body needs is packed into one POD (EwBody) so the lambda captures a
    //         SINGLE reference: the closure is then one pointer, fits std::function's small-buffer
    //         and never heap-allocates. Inside, the hot fields are copied into locals so the
    //         per-tile/per-insn loops read them from registers, not through the captured pointer. -

    body.buf_Esz_ਅਗੇਤਰ = buf_Esz_ਅਗੇਤਰ.ਅੰਕੜੇ();
    body.ਥਾਂ_ਕਿਸਮ = ਥਾਂ_ਕਿਸਮ.ਅੰਕੜੇ();
    body.nslots = nslots;
    body.nbuffers = nbuffers;
    body.capElems = capElems;

    ਪ੍ਰਸਾਰਣ_Op_2(arr.ਅੰਕੜੇ(), narr, [&](ਅਚਲ BrTile& tile)
    {
        EwBody& bc = body;
        ਅਚਲ TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ਅਚਲ ਪੂਰਨ* ਅਚਲ smap  = bc.ਥਾਂ_Map;
        ਅਚਲ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ* ਅਚਲ skind = bc.ਥਾਂ_ਕਿਸਮ;
        ਅਚਲ ਪੂਰਨ nin = bc.ninsn, nsl = bc.nslots;
        ਅਚਲ ਪੂਰਨ w = tile.ਚੌੜਾਈ, h = tile.ਉਚਾਈ;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ਅਚਲ ਪੂਰਨ* ਅਚਲ esz_ਅਗੇਤਰ = bc.buf_Esz_ਅਗੇਤਰ;
        ਅਚਲ ਪੂਰਨ bw = std::min(w, bc.capElems);
        ਅਚਲ ਪੂਰਨ bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ਅਚਲ size_t region = align_ਮਾਪ((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        Auto_ਬਫਰ<uint64_t, (4*1024 + 256)/ਆਕਾਰ(uint64_t)> tstoreBuf(
            div_ਉੱਪਰ((size_t)esz_ਅਗੇਤਰ[bc.nbuffers] * region, (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਕਾਰ(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.ਅੰਕੜੇ();

        Auto_ਬਫਰ<Br_ਟੁਕੜਾ, LOCAL_OPS> ਦਲੀਲਾਂ(nsl);
        ਲਈ (ਪੂਰਨ y0 = 0; y0 < h; y0 += bh)
        {
            ਅਚਲ ਪੂਰਨ hf = std::min(bh, h - y0);
            ਲਈ (ਪੂਰਨ x0 = 0; x0 < w; x0 += bw)
            {
                ਅਚਲ ਪੂਰਨ wf = std::min(bw, w - x0);
                ਲਈ (ਪੂਰਨ s = 1; s < nsl; s++)
                {
                    Br_ਟੁਕੜਾ& a = ਦਲੀਲਾਂ[s];
                    ਅਚਲ ਪੂਰਨ k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].ਡੂੰਘਾਈ);
                    ਜੇ (k == TExpr::ਆਰਜ਼ੀ)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)esz_ਅਗੇਤਰ[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    ਨਹੀਂਤਾਂ                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ਅਚਲ Br_ਟੁਕੜਾ& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                ਲਈ (ਪੂਰਨ n = 0; n < nin; n++)
                {
                    ਅਚਲ TExpr::Insn& ins = insns[n];
                    ਅਚਲ Br_ਟੁਕੜਾ& a0 = ਦਲੀਲਾਂ[ins.arg0]; ਅਚਲ Br_ਟੁਕੜਾ& a1 = ਦਲੀਲਾਂ[ins.arg1];
                    ਅਚਲ Br_ਟੁਕੜਾ& a2 = ਦਲੀਲਾਂ[ins.arg2]; ਅਚਲ Br_ਟੁਕੜਾ& rr = ਦਲੀਲਾਂ[ins.ਨਤੀਜਾ];
                    ਲੜੀਵਾਰ_Insn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (ਖਾਲੀ*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, ਸੱਚ, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ਖਾਲੀ TExpr::exec(ਅਚਲ Mat* inputs, Mat* outputs)
{
    Auto_ਬਫਰ<ਅਚਲ Mat*, 16> ptrs(std::max(ninputs, 1));
    ਲਈ (ਪੂਰਨ i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    exec(ptrs.ਅੰਕੜੇ(), outputs);
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
ਖਾਲੀ make_ਦੁਆਧਾਰੀ_Arith_ਪ੍ਰੋਗਰਾਮ(TExpr& p, ਸਿਖਰ op, ਪੂਰਨ depth0, ਪੂਰਨ depth1, ਪੂਰਨ rdepth,
                            ਪੂਰਨ ਨਕਾਬ_ਡੂੰਘਾਈ, ਦੁੱਗਣਾ scale)
{
    p.clear();
    ਜੇ (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        ਜੇ (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        ਨਹੀਂਤਾਂ
        {
            rdepth = promoteArith(depth0, depth1);
            ਜੇ (op == OP_ABSDIFF) rdepth = absdiff_ਨਤੀਜਾ_ਡੂੰਘਾਈ(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ਅਚਲ ਤਾਰਕਿਕ masked = ਨਕਾਬ_ਡੂੰਘਾਈ != EW_ਡੂੰਘਾਈ_NONE;
    ਪੂਰਨ sIn0  = p.ਜੋੜੋ_ਨਿਵੇਸ਼(depth0);
    ਪੂਰਨ sIn1  = p.ਜੋੜੋ_ਨਿਵੇਸ਼(depth1);
    ਪੂਰਨ s_ਨਕਾਬ = masked ? p.ਜੋੜੋ_ਨਿਵੇਸ਼(ਨਕਾਬ_ਡੂੰਘਾਈ) : 0;
    ਪੂਰਨ s_ਬਾਹਰਲਾ  = p.ਜੋੜੋ_Output(rdepth);

    ਪੂਰਨ r = p.emit_ਦੁਆਧਾਰੀ(op, sIn0, sIn1, rdepth, Scalar(scale));
    ਜੇ (masked)
        p.ਜੋੜੋ_Insn(OP_SELECT, s_ਨਕਾਬ, r, s_ਬਾਹਰਲਾ, s_ਬਾਹਰਲਾ);          // dst = mask ? r : dst
    ਨਹੀਂਤਾਂ
        p.move_ਮੰਜ਼ਿਲ_Output(r, s_ਬਾਹਰਲਾ);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
ਖਾਲੀ make_ਜੋੜੋ_Weighted_ਪ੍ਰੋਗਰਾਮ(TExpr& p, ਪੂਰਨ depth0, ਪੂਰਨ depth1, ਪੂਰਨ rdepth,
                            ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ gamma)
{
    p.clear();
    ਪੂਰਨ sA   = p.ਜੋੜੋ_ਨਿਵੇਸ਼(depth0);
    ਪੂਰਨ sB   = p.ਜੋੜੋ_ਨਿਵੇਸ਼(depth1);
    ਪੂਰਨ s_ਬਾਹਰਲਾ = p.ਜੋੜੋ_Output(rdepth);
    ਪੂਰਨ r = p.emit_ਦੁਆਧਾਰੀ(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.move_ਮੰਜ਼ਿਲ_Output(r, s_ਬਾਹਰਲਾ);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

ਨਾਮਾਕਾਸ਼ {

// --- tokens ------------------------------------------------------------------------------
ਗਿਣਤੀ TokType { T_ਅੰਕ, T_ਨਿਵੇਸ਼, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ਸੌਂਪੋ, T_ਸਿਰਾ };

ਬਣਤਰ Token
{
    TokType type = T_ਸਿਰਾ;
    ਦੁੱਗਣਾ num = 0;
    ਪੂਰਨ ਨਿਵੇਸ਼_2 = 0;
    std::string ਪਾਠ;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
ਬਣਤਰ Lexer
{
    std::string_view s;
    size_t pos = 0;

    ਸਪਸ਼ਟ Lexer(std::string_view src) : s(src) {}

    ਸਥਿਰ ਤਾਰਕਿਕ is_Ident_ਸ਼ੁਰੂ(ਅੱਖਰ c) { ਵਾਪਸ std::ਅੱਖਰ_ਹੈ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)c) || c == '_'; }
    ਸਥਿਰ ਤਾਰਕਿਕ isIdentChar(ਅੱਖਰ c)  { ਵਾਪਸ std::ਅੱਖਰ_ਜਾਂ_ਅੰਕ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)c) || c == '_'; }

    Token next()
    {
        ਜਦੋਂ (pos < s.size() && std::ਖਾਲੀ_ਥਾਂ_ਹੈ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)s[pos])) pos++;
        Token t;
        ਜੇ (pos >= s.size()) { t.type = T_ਸਿਰਾ; ਵਾਪਸ t; }

        ਅੱਖਰ c = s[pos];

        ਜੇ (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t ਸ਼ੁਰੂ = pos;
            ਜਦੋਂ (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_ਨਿਵੇਸ਼;
            t.ਨਿਵੇਸ਼_2 = std::atoi(std::string(s.substr(ਸ਼ੁਰੂ, pos - ਸ਼ੁਰੂ)).c_str());
            pos++;                          // consume '}'
            ਵਾਪਸ t;
        }
        ਜੇ (std::ਅੰਕ_ਹੈ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::ਅੰਕ_ਹੈ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)s[pos + 1])))
        {
            ਅੱਖਰ* end = ਖਾਲੀ_ਸੂਚਕ;
            std::string num(s.substr(pos));
            t.type = T_ਅੰਕ;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            ਵਾਪਸ t;
        }
        ਜੇ (is_Ident_ਸ਼ੁਰੂ(c))
        {
            size_t ਸ਼ੁਰੂ = pos;
            ਜਦੋਂ (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.ਪਾਠ.assign(s.substr(ਸ਼ੁਰੂ, pos - ਸ਼ੁਰੂ));
            ਵਾਪਸ t;
        }
        ਚੋਣ (c)
        {
        ਹਾਲਤ '(': pos++; t.type = T_LPAREN; ਵਾਪਸ t;
        ਹਾਲਤ ')': pos++; t.type = T_RPAREN; ਵਾਪਸ t;
        ਹਾਲਤ ',': pos++; t.type = T_COMMA;  ਵਾਪਸ t;
        ਹਾਲਤ ';': pos++; t.type = T_SEMI;   ਵਾਪਸ t;
        }
        // multi/!single-char operators
        ਸਵੈਚਾਲਿਤ two = [&](ਅਚਲ ਅੱਖਰ* op) {
            ਵਾਪਸ pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        ਜੇ (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.ਪਾਠ.assign(s.substr(pos, 2)); pos += 2; ਵਾਪਸ t; }
        ਜੇ (c == '=') { pos++; t.type = T_ਸੌਂਪੋ; ਵਾਪਸ t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.ਪਾਠ.assign(1, c); pos++;
        ਵਾਪਸ t;
    }
};

// --- operator / function tables ----------------------------------------------------------
ਸਥਿਰ ਪੂਰਨ binPrec(ਅਚਲ std::string& op)
{
    ਜੇ (op == "**") ਵਾਪਸ 8;              // power binds tighter than '*'; RIGHT-associative
    ਜੇ (op == "*" || op == "/") ਵਾਪਸ 7;
    ਜੇ (op == "+" || op == "-") ਵਾਪਸ 6;
    ਜੇ (op == "<" || op == "<=" || op == ">" || op == ">=") ਵਾਪਸ 5;
    ਜੇ (op == "==" || op == "!=") ਵਾਪਸ 4;
    ਜੇ (op == "&") ਵਾਪਸ 3;
    ਜੇ (op == "^") ਵਾਪਸ 2;
    ਜੇ (op == "|") ਵਾਪਸ 1;
    ਵਾਪਸ -1;
}

ਸਥਿਰ ਸਿਖਰ binOp(ਅਚਲ std::string& op)
{
    ਜੇ (op == "**") ਵਾਪਸ OP_POW;
    ਜੇ (op == "+")  ਵਾਪਸ OP_ਜੋੜੋ;
    ਜੇ (op == "-")  ਵਾਪਸ OP_SUB;
    ਜੇ (op == "*")  ਵਾਪਸ OP_MUL;
    ਜੇ (op == "/")  ਵਾਪਸ OP_DIV;
    ਜੇ (op == "<")  ਵਾਪਸ OP_CMP_LT;
    ਜੇ (op == "<=") ਵਾਪਸ OP_CMP_LE;
    ਜੇ (op == ">")  ਵਾਪਸ OP_CMP_GT;
    ਜੇ (op == ">=") ਵਾਪਸ OP_CMP_GE;
    ਜੇ (op == "==") ਵਾਪਸ OP_CMP_EQ;
    ਜੇ (op == "!=") ਵਾਪਸ OP_CMP_NE;
    ਜੇ (op == "&")  ਵਾਪਸ OP_AND;
    ਜੇ (op == "|")  ਵਾਪਸ OP_OR;
    ਜੇ (op == "^")  ਵਾਪਸ OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
ਸਥਿਰ ਪੂਰਨ type_ਡੂੰਘਾਈ(ਅਚਲ std::string& name)
{
    ਜੇ (name == "float")    ਵਾਪਸ CV_32F;
    ਜੇ (name == "double")   ਵਾਪਸ CV_64F;
    ਜੇ (name == "half" || name == "float16") ਵਾਪਸ CV_16F;
    ਜੇ (name == "bfloat16") ਵਾਪਸ CV_16BF;
    ਜੇ (name == "uint8")    ਵਾਪਸ CV_8U;
    ਜੇ (name == "int8")     ਵਾਪਸ CV_8S;
    ਜੇ (name == "uint16")   ਵਾਪਸ CV_16U;
    ਜੇ (name == "int16")    ਵਾਪਸ CV_16S;
    ਜੇ (name == "uint32")   ਵਾਪਸ CV_32U;
    ਜੇ (name == "int32")    ਵਾਪਸ CV_32S;
    ਜੇ (name == "uint64")   ਵਾਪਸ CV_64U;
    ਜੇ (name == "int64")    ਵਾਪਸ CV_64S;
    ਵਾਪਸ -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
ਸਥਿਰ ਸਿਖਰ fnOp(ਅਚਲ std::string& name, ਪੂਰਨ& arity)
{
    ਬਣਤਰ E { ਅਚਲ ਅੱਖਰ* n; ਸਿਖਰ op; };
    ਸਥਿਰ ਅਚਲ E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    ਸਥਿਰ ਅਚਲ E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    ਸਥਿਰ ਅਚਲ E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    ਲਈ (ਅਚਲ E& e : unary)  ਜੇ (name == e.n) { arity = 1; ਵਾਪਸ e.op; }
    ਲਈ (ਅਚਲ E& e : binary) ਜੇ (name == e.n) { arity = 2; ਵਾਪਸ e.op; }
    ਲਈ (ਅਚਲ E& e : tern)   ਜੇ (name == e.n) { arity = 3; ਵਾਪਸ e.op; }
    arity = 0; ਵਾਪਸ OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
ਬਣਤਰ Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ਅਚਲ ਪੂਰਨ* ਨਿਵੇਸ਼_ਥਾਂ;                 // slot id of each external input (precreated)
    ਪੂਰਨ ninputs;
    std::map<std::string, ਪੂਰਨ> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& ਵਿਅੰਜਕ, ਅਚਲ ਪੂਰਨ* islot, ਪੂਰਨ nin)
        : lex(src), e(ਵਿਅੰਜਕ), ਨਿਵੇਸ਼_ਥਾਂ(islot), ninputs(nin) { cur = lex.next(); }

    ਖਾਲੀ advance() { cur = lex.next(); }
    ਤਾਰਕਿਕ isOp(ਅਚਲ ਅੱਖਰ* op) ਅਚਲ { ਵਾਪਸ cur.type == T_OP && cur.ਪਾਠ == op; }

    ਖਾਲੀ expect(TokType t, ਅਚਲ ਅੱਖਰ* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    ਤਾਰਕਿਕ isFlexConst(ਪੂਰਨ ਥਾਂ) ਅਚਲ
    {
        ਵਾਪਸ e.arginfo[ਥਾਂ].ਕਿਸਮ == TExpr::CONST && e.arginfo[ਥਾਂ].ਡੂੰਘਾਈ == EW_ਡੂੰਘਾਈ_NONE;
    }

    ਪੂਰਨ parsePrimary()
    {
        ਜੇ (cur.type == T_ਅੰਕ)   { ਪੂਰਨ s = e.ਜੋੜੋ_Const(EW_ਡੂੰਘਾਈ_NONE, Scalar(cur.num), 1); advance(); ਵਾਪਸ s; }
        ਜੇ (cur.type == T_ਨਿਵੇਸ਼) { ਪੂਰਨ idx = cur.ਨਿਵੇਸ਼_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   ਵਾਪਸ ਨਿਵੇਸ਼_ਥਾਂ[idx]; }
        ਜੇ (cur.type == T_LPAREN){ advance(); ਪੂਰਨ x = parseTernary(); expect(T_RPAREN, "expected ')'"); ਵਾਪਸ x; }
        ਜੇ (cur.type == T_IDENT)
        {
            std::string name = cur.ਪਾਠ; advance();
            ਜੇ (cur.type != T_LPAREN)             // variable reference
            {
                ਸਵੈਚਾਲਿਤ it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                ਵਾਪਸ it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<ਪੂਰਨ, 8> ਦਲੀਲਾਂ = {}; ਪੂਰਨ nargs = 0;
            ਜੇ (cur.type != T_RPAREN)
            {
                ਦਲੀਲਾਂ[nargs++] = parseTernary();
                ਜਦੋਂ (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (ਪੂਰਨ)ਦਲੀਲਾਂ.size() && "ew::expression: too many arguments");
                    ਦਲੀਲਾਂ[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            ਪੂਰਨ td = type_ਡੂੰਘਾਈ(name);
            ਜੇ (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                ਪੂਰਨ s = ਦਲੀਲਾਂ[0];
                ਵਾਪਸ isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybe_ਜੋੜੋ_Cast(s, td);
            }

            ਪੂਰਨ arity = 0; ਸਿਖਰ op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            ਜੇ (arity == 1) ਵਾਪਸ e.emitUnary(op, ਦਲੀਲਾਂ[0]);
            ਜੇ (arity == 2) ਵਾਪਸ e.emit_ਦੁਆਧਾਰੀ(op, ਦਲੀਲਾਂ[0], ਦਲੀਲਾਂ[1]);
            ਵਾਪਸ e.emitTernary(op, ਦਲੀਲਾਂ[0], ਦਲੀਲਾਂ[1], ਦਲੀਲਾਂ[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    ਪੂਰਨ parseUnary()
    {
        ਜੇ (isOp("-") || isOp("!"))
        {
            std::string op = cur.ਪਾਠ; advance();
            ਪੂਰਨ ਸੰਕਾਰਜ = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            ਜੇ (op == "-" && isFlexConst(ਸੰਕਾਰਜ))
            {
                Auto_ਬਫਰ<ਦੁੱਗਣਾ, MAX_LOCAL_CN> v; ਪੂਰਨ cn = constDoubles(e, ਸੰਕਾਰਜ, v);
                Scalar neg; ਲਈ (ਪੂਰਨ i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                ਵਾਪਸ e.ਜੋੜੋ_Const(EW_ਡੂੰਘਾਈ_NONE, neg, std::min(cn, 4));
            }
            ਵਾਪਸ e.emitUnary(op == "-" ? OP_NEG : OP_NOT, ਸੰਕਾਰਜ);
        }
        ਵਾਪਸ parsePrimary();
    }

    ਪੂਰਨ parse_ਵਿਅੰਜਕ(ਪੂਰਨ minPrec)
    {
        ਪੂਰਨ ਖੱਬੇ = parseUnary();
        ਜਦੋਂ (cur.type == T_OP)
        {
            ਪੂਰਨ p = binPrec(cur.ਪਾਠ);
            ਜੇ (p < minPrec) ਰੋਕੋ;
            std::string op = cur.ਪਾਠ; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            ਪੂਰਨ ਸੱਜੇ = parse_ਵਿਅੰਜਕ(op == "**" ? p : p + 1);
            ਖੱਬੇ = e.emit_ਦੁਆਧਾਰੀ(binOp(op), ਖੱਬੇ, ਸੱਜੇ);
        }
        ਵਾਪਸ ਖੱਬੇ;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    ਪੂਰਨ parseTernary()
    {
        ਪੂਰਨ cond = parse_ਵਿਅੰਜਕ(0);
        ਜੇ (!isOp("?")) ਵਾਪਸ cond;
        advance();
        ਪੂਰਨ thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        ਪੂਰਨ elsev = parseTernary();               // right-associative chain
        ਵਾਪਸ e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    ਖਾਲੀ parse_ਨਤੀਜਾ()
    {
        ਜੇ (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); ਪੂਰਨ n_ਆਰਜ਼ੀ = e.ntemps;
            advance();
            ਪੂਰਨ e0 = parseTernary();
            ਜੇ (cur.type == T_COMMA)
            {
                e.output(e0);
                ਜਦੋਂ (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                ਵਾਪਸ;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = n_ਆਰਜ਼ੀ;
        }
        e.output(parseTernary());
    }

    ਖਾਲੀ parse()
    {
        ਜਦੋਂ (ਸੱਚ)
        {
            ਜੇ (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.ਪਾਠ; advance();
                ਜੇ (cur.type == T_ਸੌਂਪੋ)
                {
                    advance();
                    ਅਚਲ ਪੂਰਨ ਥਾਂ = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[ਥਾਂ].pinned = ਸੱਚ;
                    env[name] = ਥਾਂ;
                    expect(T_SEMI, "expected ';' after assignment");
                    ਜਾਰੀ;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parse_ਨਤੀਜਾ();
            ਜੇ (cur.type == T_SEMI) advance();
            ਰੋਕੋ;
        }
        CV_Assert(cur.type == T_ਸਿਰਾ && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

ਨਾਮਾਕਾਸ਼ cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
ਖਾਲੀ texpr(ਅਚਲ std::string& ਵਿਅੰਜਕ, ਨਿਵੇਸ਼_Array_ਦਾ_Arrays _inputs, Output_Array_ਦਾ_Arrays _outputs)
{
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT);
    ਅਚਲ std::vector<Mat>& inps = *(ਅਚਲ std::vector<Mat>*)_inputs.getObj();
    ਅਚਲ ਪੂਰਨ ninputs = (ਪੂਰਨ)inps.size();
    ਲਈ (ਪੂਰਨ i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    Auto_ਬਫਰ<ਪੂਰਨ> islot(std::max(ninputs, 1));
    ਲਈ (ਪੂਰਨ i = 0; i < ninputs; i++) islot[i] = e.ਜੋੜੋ_ਨਿਵੇਸ਼(inps[i].ਡੂੰਘਾਈ());
    Parser(ਵਿਅੰਜਕ, e, islot.ਅੰਕੜੇ(), ninputs).parse();
    e.compile();

    ਸਵੈਚਾਲਿਤ ਕਿਸਮ = _outputs.ਕਿਸਮ();
    ਜੇ (ਕਿਸਮ == ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.ਅੰਕੜੇ(), outs.ਅੰਕੜੇ());
    } ਨਹੀਂਤਾਂ {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
