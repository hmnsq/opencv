// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "arithm_expr.hpp"
#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <climits>
#അന്തര്ഭാവ <cmath>
#അന്തര്ഭാവ <cstdlib>
#അന്തര്ഭാവ <cstring>
#അന്തര്ഭാവ <map>
#അന്തര്ഭാവ <ostream>
#അന്തര്ഭാവ <string>

നാമാകാശ cv { നാമാകാശ ew {

// ============================ op metadata (was ew_op.cpp) ============================

സ്ഥിരം അക്ഷരം* opName(മുകൾ op)
{
    തിരഞ്ഞെടുപ്പ് (op)
    {
    അവസ്ഥ OP_NOP:           തിരികെ "nop";
    അവസ്ഥ OP_NEG:           തിരികെ "neg";
    അവസ്ഥ OP_ABS:           തിരികെ "abs";
    അവസ്ഥ OP_NOT:           തിരികെ "not";
    അവസ്ഥ OP_SQRT:          തിരികെ "sqrt";
    അവസ്ഥ OP_EXP:           തിരികെ "exp";
    അവസ്ഥ OP_LOG:           തിരികെ "log";
    അവസ്ഥ OP_SIN:           തിരികെ "sin";
    അവസ്ഥ OP_COS:           തിരികെ "cos";
    അവസ്ഥ OP_TANH:          തിരികെ "tanh";
    അവസ്ഥ OP_ERF:           തിരികെ "erf";
    അവസ്ഥ OP_RELU:          തിരികെ "relu";
    അവസ്ഥ OP_CAST:          തിരികെ "cast";
    അവസ്ഥ OP_ADD:           തിരികെ "add";
    അവസ്ഥ OP_SUB:           തിരികെ "sub";
    അവസ്ഥ OP_MUL:           തിരികെ "mul";
    അവസ്ഥ OP_DIV:           തിരികെ "div";
    അവസ്ഥ OP_POW:           തിരികെ "pow";
    അവസ്ഥ OP_MIN:           തിരികെ "min";
    അവസ്ഥ OP_MAX:           തിരികെ "max";
    അവസ്ഥ OP_ABSDIFF:       തിരികെ "absdiff";
    അവസ്ഥ OP_ADDW:          തിരികെ "addWeighted";
    അവസ്ഥ OP_HYPOT:         തിരികെ "hypot";
    അവസ്ഥ OP_ATAN2:         തിരികെ "atan2";
    അവസ്ഥ OP_AND:           തിരികെ "and";
    അവസ്ഥ OP_OR:            തിരികെ "or";
    അവസ്ഥ OP_XOR:           തിരികെ "xor";
    അവസ്ഥ OP_CMP_EQ:        തിരികെ "cmp_eq";
    അവസ്ഥ OP_CMP_NE:        തിരികെ "cmp_ne";
    അവസ്ഥ OP_CMP_LT:        തിരികെ "cmp_lt";
    അവസ്ഥ OP_CMP_LE:        തിരികെ "cmp_le";
    അവസ്ഥ OP_CMP_GT:        തിരികെ "cmp_gt";
    അവസ്ഥ OP_CMP_GE:        തിരികെ "cmp_ge";
    അവസ്ഥ OP_CLAMP:         തിരികെ "clamp";
    അവസ്ഥ OP_SELECT:        തിരികെ "select";
    അവസ്ഥ OP_CONVERT_SCALE: തിരികെ "convert_scale";
    സ്ഥിരസ്ഥിതി:               തിരികെ "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(മുകൾ op)
{
    തിരഞ്ഞെടുപ്പ് (op)
    {
    അവസ്ഥ OP_AND: അവസ്ഥ OP_OR: അവസ്ഥ OP_XOR: അവസ്ഥ OP_NOT:
        തിരികെ CAT_BITWISE;
    അവസ്ഥ OP_CMP_EQ: അവസ്ഥ OP_CMP_NE: അവസ്ഥ OP_CMP_LT:
    അവസ്ഥ OP_CMP_LE: അവസ്ഥ OP_CMP_GT: അവസ്ഥ OP_CMP_GE:
        തിരികെ CAT_COMPARE;
    അവസ്ഥ OP_SQRT: അവസ്ഥ OP_EXP: അവസ്ഥ OP_LOG:
    അവസ്ഥ OP_SIN: അവസ്ഥ OP_COS: അവസ്ഥ OP_TANH: അവസ്ഥ OP_ERF: അവസ്ഥ OP_ATAN2: അവസ്ഥ OP_RELU:
        തിരികെ CAT_MATH;
    അവസ്ഥ OP_CAST: അവസ്ഥ OP_CONVERT_SCALE:
        തിരികെ CAT_CAST;
    അവസ്ഥ OP_SELECT:
        തിരികെ CAT_SELECT;
    സ്ഥിരസ്ഥിതി:
        തിരികെ CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
സ്ഥിതം തർക്കം isFloatDepth(പൂർണ്ണസംഖ്യ d)
{
    തിരികെ d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
സ്ഥിതം തർക്കം depthRepresents(ഇരട്ട v, പൂർണ്ണസംഖ്യ depth)
{
    എങ്കിൽ (isFloatDepth(depth)) തിരികെ സത്യം;
    എങ്കിൽ (v != std::floor(v)) തിരികെ അസത്യം;
    തിരഞ്ഞെടുപ്പ് (depth)
    {
    അവസ്ഥ CV_8U:  തിരികെ v >= 0            && v <= 255;
    അവസ്ഥ CV_8S:  തിരികെ v >= -128         && v <= 127;
    അവസ്ഥ CV_16U: തിരികെ v >= 0            && v <= 65535;
    അവസ്ഥ CV_16S: തിരികെ v >= -32768       && v <= 32767;
    അവസ്ഥ CV_32U: തിരികെ v >= 0            && v <= 4294967295.0;
    അവസ്ഥ CV_32S: തിരികെ v >= -2147483648.0 && v <= 2147483647.0;
    അവസ്ഥ CV_64U: തിരികെ v >= 0            && v <= 18446744073709551615.0;
    അവസ്ഥ CV_64S: തിരികെ v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    സ്ഥിരസ്ഥിതി:     തിരികെ അസത്യം;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
സ്ഥിതം ശൂന്യം intRange(പൂർണ്ണസംഖ്യ depth, ഇരട്ട& lo, ഇരട്ട& hi)
{
    തിരഞ്ഞെടുപ്പ് (depth)
    {
    അവസ്ഥ CV_8U:  lo = 0;                     hi = 255; നിർത്തുക;
    അവസ്ഥ CV_8S:  lo = -128;                  hi = 127; നിർത്തുക;
    അവസ്ഥ CV_16U: lo = 0;                     hi = 65535; നിർത്തുക;
    അവസ്ഥ CV_16S: lo = -32768;                hi = 32767; നിർത്തുക;
    അവസ്ഥ CV_32U: lo = 0;                     hi = 4294967295.0; നിർത്തുക;
    അവസ്ഥ CV_32S: lo = -2147483648.0;         hi = 2147483647.0; നിർത്തുക;
    അവസ്ഥ CV_64U: lo = 0;                     hi = 18446744073709551615.0; നിർത്തുക;
    അവസ്ഥ CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; നിർത്തുക;
    സ്ഥിരസ്ഥിതി:     lo = 0;                     hi = 0; നിർത്തുക;
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
പൂർണ്ണസംഖ്യ promoteArith(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    എങ്കിൽ (a == b) തിരികെ a;

    പൂർണ്ണസംഖ്യ wa = പൂർണ്ണസംഖ്യ((leszlut >> (a+1)*3) & 7u), wb = പൂർണ്ണസംഖ്യ((leszlut >> (b+1)*3) & 7u);
    പൂർണ്ണസംഖ്യ fa = പൂർണ്ണസംഖ്യ((fltlut >> (a+1)) & 1u),    fb = പൂർണ്ണസംഖ്യ((fltlut >> (b+1)) & 1u);

    എങ്കിൽ (fa + fb == 0)                          // both integer
    {
        പൂർണ്ണസംഖ്യ sa = പൂർണ്ണസംഖ്യ((signlut >> (a+1)) & 1u), sb = പൂർണ്ണസംഖ്യ((signlut >> (b+1)) & 1u);
        എങ്കിൽ (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            തിരികെ wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        പൂർണ്ണസംഖ്യ rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr പൂർണ്ണസംഖ്യ ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        തിരികെ (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    പൂർണ്ണസംഖ്യ maxw = std::max(wa, wb);
    എങ്കിൽ (maxw == 2 && fa + fb == 1)
        തിരികെ fa*a + (1 - fa)*b;
    തിരികെ CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
പൂർണ്ണസംഖ്യ absdiffResultDepth(പൂർണ്ണസംഖ്യ depth)
{
    തിരഞ്ഞെടുപ്പ് (depth)
    {
    അവസ്ഥ CV_8S:  തിരികെ CV_8U;
    അവസ്ഥ CV_16S: തിരികെ CV_16U;
    അവസ്ഥ CV_32S: തിരികെ CV_32U;
    അവസ്ഥ CV_64S: തിരികെ CV_64U;
    സ്ഥിരസ്ഥിതി:     തിരികെ depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
സ്ഥിതം പൂർണ്ണസംഖ്യ promote2(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b)
{
    എങ്കിൽ (a == b) തിരികെ a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    സ്ഥിരം uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    സ്ഥിരം unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    തിരികെ പൂർണ്ണസംഖ്യ((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
സ്ഥിതം പൂർണ്ണസംഖ്യ promoteBitwise(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b)
{
    എങ്കിൽ (a == EW_DEPTH_NONE) തിരികെ b;
    എങ്കിൽ (b == EW_DEPTH_NONE) തിരികെ a;
    എങ്കിൽ (a == b) തിരികെ a;
    തിരികെ CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
സ്ഥിതം പൂർണ്ണസംഖ്യ safeWide(പൂർണ്ണസംഖ്യ depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    സ്ഥിരം uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    സ്ഥിരം unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    തിരികെ പൂർണ്ണസംഖ്യ((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
എണ്ണൽ { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
സ്ഥിതം size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, പൂർണ്ണസംഖ്യ srcdepth, സ്ഥിരം ശൂന്യം* data, പൂർണ്ണസംഖ്യ channels)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = std::max(1, channels);
    സ്ഥിരം size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    സ്ഥിരം size_t nu64 = (nbytes + വലുപ്പം(uint64_t) - 1) / വലുപ്പം(uint64_t);
    സ്ഥിരം size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    എങ്കിൽ (data) memcpy(dst, data, nbytes); അല്ലെങ്കിൽ memset(dst, 0, nbytes);
    തിരികെ ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
സ്ഥിതം പൂർണ്ണസംഖ്യ constDoubles(സ്ഥിരം TExpr& e, പൂർണ്ണസംഖ്യ s, AutoBuffer<ഇരട്ട, MAX_LOCAL_CN>& buf)
{
    സ്ഥിരം TExpr::Arg& a = e.arginfo[s];
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = std::max(1, a.channels);
    buf.resize(cn);
    സ്ഥിരം uchar* src = (സ്ഥിരം uchar*)(e.constbuf.data() + a.constofs);
    എങ്കിൽ (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * വലുപ്പം(ഇരട്ട));   // common (Scalar / parsed literal)
    അല്ലെങ്കിൽ
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    തിരികെ cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
സ്ഥിതം വരിയിൽ തർക്കം isFlexConst(സ്ഥിരം TExpr& e, പൂർണ്ണസംഖ്യ s)
{
    തിരികെ e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
സ്ഥിതം തർക്കം scalar_Const_മൂല്യം(സ്ഥിരം TExpr& e, പൂർണ്ണസംഖ്യ s, ഇരട്ട& v)
{
    സ്ഥിരം TExpr::Arg& a = e.arginfo[s];
    എങ്കിൽ (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) തിരികെ അസത്യം;
    AutoBuffer<ഇരട്ട, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    തിരികെ സത്യം;
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
സ്ഥിതം തർക്കം matchScaledChain(സ്ഥിരം TExpr& e, പൂർണ്ണസംഖ്യ end, പൂർണ്ണസംഖ്യ t,
                             പൂർണ്ണസംഖ്യ& arrSlot, ഇരട്ട& scale, പൂർണ്ണസംഖ്യ& startIdx)
{
    എങ്കിൽ (end < 0) തിരികെ അസത്യം;
    പൂർണ്ണസംഖ്യ j = end, mulResult = t;

    // optional trailing narrowing cast
    എങ്കിൽ (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        എങ്കിൽ (--j < 0) തിരികെ അസത്യം;
    }

    സ്ഥിരം TExpr::Insn& mul = e.prog[j];
    എങ്കിൽ (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ arr;
    എങ്കിൽ      (scalar_Const_മൂല്യം(e, mul.arg1, scale)) arr = mul.arg0;
    അല്ലെങ്കിൽ എങ്കിൽ (scalar_Const_മൂല്യം(e, mul.arg0, scale)) arr = mul.arg1;
    അല്ലെങ്കിൽ തിരികെ അസത്യം;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    എങ്കിൽ (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    എങ്കിൽ (e.arginfo[arr].kind == TExpr::CONST) തിരികെ അസത്യം;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    തിരികെ സത്യം;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
സ്ഥിതം തർക്കം constFits(സ്ഥിരം TExpr& e, പൂർണ്ണസംഖ്യ s, പൂർണ്ണസംഖ്യ d)
{
    എങ്കിൽ (!isFlexConst(e, s)) തിരികെ സത്യം;
    AutoBuffer<ഇരട്ട, MAX_LOCAL_CN> v;
    പൂർണ്ണസംഖ്യ cn = constDoubles(e, s, v);
    വേണ്ടി (പൂർണ്ണസംഖ്യ ch = 0; ch < cn; ch++)
        എങ്കിൽ (!depthRepresents(v[ch], d)) തിരികെ അസത്യം;
    തിരികെ സത്യം;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
പൂർണ്ണസംഖ്യ TExpr::emit_ദ്വിമാന(മുകൾ op, പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b, പൂർണ്ണസംഖ്യ rdepth, സ്ഥിരം Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    എങ്കിൽ (op == OP_ADDW)
    {
        പൂർണ്ണസംഖ്യ Tt = arginfo[a].depth;
        എങ്കിൽ (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        എങ്കിൽ (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        എങ്കിൽ (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        പൂർണ്ണസംഖ്യ outD = rdepth;
        എങ്കിൽ (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        സ്ഥിരം പൂർണ്ണസംഖ്യ out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        എങ്കിൽ (outD == rdepth) തിരികെ out;
        സ്ഥിരം പൂർണ്ണസംഖ്യ out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        തിരികെ out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    എങ്കിൽ (op == OP_ADD)
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ n = (പൂർണ്ണസംഖ്യ)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        പൂർണ്ണസംഖ്യ x = 0, y = 0, startA = 0, startB = 0;
        ഇരട്ട alpha = 0, beta = 0;
        എങ്കിൽ (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*ഇത്, n - 1, b, y, beta, startB) &&
            matchScaledChain(*ഇത്, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            പൂർണ്ണസംഖ്യ retired = 0, lowest = INT_MAX;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = startA; i < n; i++)
            {
                സ്ഥിരം പൂർണ്ണസംഖ്യ r = prog[i].result;
                എങ്കിൽ (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; നിർത്തുക; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            എങ്കിൽ (retired > 0 && lowest == ntemps - retired)
            {
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                തിരികെ emit_ദ്വിമാന(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            പൂർണ്ണസംഖ്യ acc = 0; ഇരട്ട gamma = 0;
            എങ്കിൽ      (scalar_Const_മൂല്യം(*ഇത്, b, gamma)) acc = a;
            അല്ലെങ്കിൽ എങ്കിൽ (scalar_Const_മൂല്യം(*ഇത്, a, gamma)) acc = b;
            എങ്കിൽ (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                തിരികെ acc;
            }
        }
    }

    സ്ഥിരം പൂർണ്ണസംഖ്യ nd0 = isFlexConst(*ഇത്, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    സ്ഥിരം പൂർണ്ണസംഖ്യ nd1 = isFlexConst(*ഇത്, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    സ്ഥിരം ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    പൂർണ്ണസംഖ്യ result = rdepth;
    എങ്കിൽ (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        എങ്കിൽ (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    പൂർണ്ണസംഖ്യ base;
    എങ്കിൽ (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    അല്ലെങ്കിൽ എങ്കിൽ (nd0 == EW_DEPTH_NONE) base = nd1;
    അല്ലെങ്കിൽ എങ്കിൽ (nd1 == EW_DEPTH_NONE) base = nd0;
    അല്ലെങ്കിൽ base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    എങ്കിൽ (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*ഇത്, b) && !constFits(*ഇത്, b, base))
    {
        ഇരട്ട lo, hi; intRange(nd0, lo, hi);
        എങ്കിൽ (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            എണ്ണൽ { REAL, CFALSE, CTRUE };
            AutoBuffer<ഇരട്ട, MAX_LOCAL_CN> tv; പൂർണ്ണസംഖ്യ cn = constDoubles(*ഇത്, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            പൂർണ്ണസംഖ്യ kind[4] = {}; ഇരട്ട bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            സ്ഥിരം മുകൾ fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < cn; c++)
            {
                സ്ഥിരം ഇരട്ട t = tv[c]; പൂർണ്ണസംഖ്യ k; ഇരട്ട B = t;
                തിരഞ്ഞെടുപ്പ് (op)
                {
                അവസ്ഥ OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; നിർത്തുക;
                അവസ്ഥ OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; നിർത്തുക;
                അവസ്ഥ OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; നിർത്തുക;
                അവസ്ഥ OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; നിർത്തുക;
                അവസ്ഥ OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; നിർത്തുക;   // a==non-rep -> false
                സ്ഥിരസ്ഥിതി:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  നിർത്തുക;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](പൂർണ്ണസംഖ്യ c){ തിരികെ kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](പൂർണ്ണസംഖ്യ c){ തിരികെ kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            സ്ഥിരം മുകൾ u0 = ucOp(0);
            തർക്കം uniform = സത്യം;
            വേണ്ടി (പൂർണ്ണസംഖ്യ c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            എങ്കിൽ (uniform)
            {
                Scalar s; വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            അല്ലെങ്കിൽ
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; പൂർണ്ണസംഖ്യ patchFlags = 0;
                വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    സ്ഥിരം പൂർണ്ണസംഖ്യ mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    സ്ഥിരം പൂർണ്ണസംഖ്യ vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                സ്ഥിരം പൂർണ്ണസംഖ്യ out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                തിരികെ out;
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
    പൂർണ്ണസംഖ്യ depth, wide;
    തർക്കം divGuard = അസത്യം;
    തിരഞ്ഞെടുപ്പ് (op)
    {
    അവസ്ഥ OP_MIN: അവസ്ഥ OP_MAX:
    അവസ്ഥ OP_AND: അവസ്ഥ OP_OR: അവസ്ഥ OP_XOR:
        depth = result; wide = result; നിർത്തുക;
    അവസ്ഥ OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        പൂർണ്ണസംഖ്യ common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        എങ്കിൽ (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        എങ്കിൽ (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        നിർത്തുക;
    }
    അവസ്ഥ OP_CMP_EQ: അവസ്ഥ OP_CMP_NE: അവസ്ഥ OP_CMP_LT:
    അവസ്ഥ OP_CMP_LE: അവസ്ഥ OP_CMP_GT: അവസ്ഥ OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; നിർത്തുക;
    അവസ്ഥ OP_MUL: അവസ്ഥ OP_DIV:
    {
        depth = base;
        സ്ഥിരം തർക്കം w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        എങ്കിൽ (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](പൂർണ്ണസംഖ്യ s, പൂർണ്ണസംഖ്യ nd) {
                എങ്കിൽ (nd != EW_DEPTH_NONE) തിരികെ !isFloatDepth(nd);
                AutoBuffer<ഇരട്ട, MAX_LOCAL_CN> v; constDoubles(*ഇത്, s, v);
                തിരികെ v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        നിർത്തുക;
    }
    അവസ്ഥ OP_POW: അവസ്ഥ OP_HYPOT: അവസ്ഥ OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; നിർത്തുക;
    സ്ഥിരസ്ഥിതി:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); നിർത്തുക;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](പൂർണ്ണസംഖ്യ s) {
        എങ്കിൽ (!isFlexConst(*ഇത്, s)) തിരികെ അസത്യം;
        AutoBuffer<ഇരട്ട, MAX_LOCAL_CN> v; പൂർണ്ണസംഖ്യ n = constDoubles(*ഇത്, s, v);
        വേണ്ടി (പൂർണ്ണസംഖ്യ ch = 0; ch < n; ch++)
            എങ്കിൽ (v[ch] != std::floor(v[ch])) തിരികെ സത്യം;
        തിരികെ അസത്യം;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    എങ്കിൽ (cat != CAT_BITWISE && (!constFits(*ഇത്, a, depth) || !constFits(*ഇത്, b, depth)))
    {
        depth = wide;
        സ്ഥിരം തർക്കം frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        എങ്കിൽ (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        എങ്കിൽ (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    പൂർണ്ണസംഖ്യ s0 = isFlexConst(*ഇത്, a) ? typedConstFrom(a, depth) : a;
    പൂർണ്ണസംഖ്യ s1 = isFlexConst(*ഇത്, b) ? typedConstFrom(b, depth) : b;
    പൂർണ്ണസംഖ്യ c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](പൂർണ്ണസംഖ്യ rd) {
        തിരികെ op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    എങ്കിൽ (k.fptr)
    {
        പൂർണ്ണസംഖ്യ res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        തിരികെ res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    പൂർണ്ണസംഖ്യ w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    തിരികെ maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
പൂർണ്ണസംഖ്യ TExpr::emitUnary(മുകൾ op, പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ rdepth, സ്ഥിരം Scalar& params)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ nd = isFlexConst(*ഇത്, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    സ്ഥിരം ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    എങ്കിൽ (op == OP_NEG)
        തിരികെ emit_ദ്വിമാന(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    എങ്കിൽ (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        എങ്കിൽ (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            തിരികെ emit_ദ്വിമാന(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        തിരികെ emit_ദ്വിമാന(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    പൂർണ്ണസംഖ്യ result = rdepth;
    എങ്കിൽ (result == EW_DEPTH_NONE)
    {
        തിരഞ്ഞെടുപ്പ് (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        അവസ്ഥ CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; നിർത്തുക;
        അവസ്ഥ CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; നിർത്തുക;  // NOT
        സ്ഥിരസ്ഥിതി:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; നിർത്തുക;  // NEG, ABS
        }
    }

    പൂർണ്ണസംഖ്യ depth, wide;
    തിരഞ്ഞെടുപ്പ് (cat)
    {
    അവസ്ഥ CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        നിർത്തുക;
    അവസ്ഥ CAT_BITWISE: depth = result; wide = result; നിർത്തുക;
    സ്ഥിരസ്ഥിതി:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; നിർത്തുക;  // NEG/ABS
    }

    എങ്കിൽ (!constFits(*ഇത്, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    പൂർണ്ണസംഖ്യ s0 = isFlexConst(*ഇത്, a) ? typedConstFrom(a, depth) : a;
    പൂർണ്ണസംഖ്യ c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    എങ്കിൽ (k.fptr)
    {
        പൂർണ്ണസംഖ്യ res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        തിരികെ res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    പൂർണ്ണസംഖ്യ w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    തിരികെ maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
പൂർണ്ണസംഖ്യ TExpr::emitTernary(മുകൾ op, പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ rdepth)
{
    auto dep = [&](പൂർണ്ണസംഖ്യ s) { തിരികെ isFlexConst(*ഇത്, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    എങ്കിൽ (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        എങ്കിൽ (isFlexConst(*ഇത്, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_ദ്വിമാന(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        പൂർണ്ണസംഖ്യ result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        എങ്കിൽ (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        പൂർണ്ണസംഖ്യ sb = isFlexConst(*ഇത്, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        പൂർണ്ണസംഖ്യ sc = isFlexConst(*ഇത്, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        പൂർണ്ണസംഖ്യ res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        തിരികെ res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    പൂർണ്ണസംഖ്യ result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    എങ്കിൽ (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](പൂർണ്ണസംഖ്യ s) {
        തിരികെ isFlexConst(*ഇത്, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    പൂർണ്ണസംഖ്യ c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    പൂർണ്ണസംഖ്യ res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    തിരികെ res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
പൂർണ്ണസംഖ്യ TExpr::moveToOutput(പൂർണ്ണസംഖ്യ temp, പൂർണ്ണസംഖ്യ out)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ ninsn = (പൂർണ്ണസംഖ്യ)prog.size();
    പൂർണ്ണസംഖ്യ producer = -1;
    തർക്കം usedAsArg = അസത്യം;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninsn; i++)
    {
        സ്ഥിരം TExpr::Insn& ins = prog[i];
        എങ്കിൽ (ins.result == temp) producer = i;
        എങ്കിൽ (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = സത്യം;
    }
    എങ്കിൽ (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        തിരികെ out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    തിരികെ out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
പൂർണ്ണസംഖ്യ TExpr::output(പൂർണ്ണസംഖ്യ വേര്_Slot)
{
    തിരികെ moveToOutput(വേര്_Slot, addOutput(arginfo[വേര്_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
ശൂന്യം TExpr::compile()
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ ninsn = (പൂർണ്ണസംഖ്യ)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    സ്ഥിരം പൂർണ്ണസംഖ്യ nslots = (പൂർണ്ണസംഖ്യ)arginfo.size();
    nconsts = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        എങ്കിൽ (a.kind != CONST || a.depth == EW_DEPTH_NONE) തുടരുക;
        സ്ഥിരം പൂർണ്ണസംഖ്യ cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        സ്ഥിരം size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, വലുപ്പം(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)വലുപ്പം(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (സ്ഥിരം uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        സ്ഥിരം size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        എങ്കിൽ (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        അല്ലെങ്കിൽ getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
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
    എങ്കിൽ (ntemps == 0) തിരികെ;

    AutoBuffer<പൂർണ്ണസംഖ്യ, 32> tempOfSlot(nslots);
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 1; s < nslots; s++)
        എങ്കിൽ (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<പൂർണ്ണസംഖ്യ, 16> lastUse(ntemps);
    വേണ്ടി (പൂർണ്ണസംഖ്യ t = 0; t < ntemps; t++) lastUse[t] = -1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninsn; i++)
    {
        സ്ഥിരം TExpr::Insn& ins = prog[i];
        പൂർണ്ണസംഖ്യ as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < 3; k++)
            എങ്കിൽ (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    വേണ്ടി (പൂർണ്ണസംഖ്യ t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<പൂർണ്ണസംഖ്യ, 16> freeBufs(ntemps);
    പൂർണ്ണസംഖ്യ nfree = 0, nbuf = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninsn; i++)
    {
        സ്ഥിരം TExpr::Insn& ins = prog[i];
        പൂർണ്ണസംഖ്യ rt = tempOfSlot[ins.result];
        എങ്കിൽ (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        പൂർണ്ണസംഖ്യ as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < 3; k++)
        {
            പൂർണ്ണസംഖ്യ t = tempOfSlot[as[k]];
            എങ്കിൽ (t >= 0 && lastUse[t] == i)
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
    വേണ്ടി (പൂർണ്ണസംഖ്യ b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 1; s < nslots; s++)
        എങ്കിൽ (arginfo[s].kind == TEMP)
        {
            പൂർണ്ണസംഖ്യ b = bufferOfTemp[arginfo[s].index], e = (പൂർണ്ണസംഖ്യ)CV_ELEM_SIZE1(arginfo[s].depth);
            എങ്കിൽ (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    വേണ്ടി (പൂർണ്ണസംഖ്യ b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    സ്ഥിരം പൂർണ്ണസംഖ്യ totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
സ്ഥിതം ശൂന്യം matLogical(സ്ഥിരം Mat& m, MatShape& shp, EwSteps& step, പൂർണ്ണസംഖ്യ& esz1)
{
    esz1 = (പൂർണ്ണസംഖ്യ)m.elemSize1();
    പൂർണ്ണസംഖ്യ nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
സ്ഥിതം തർക്കം broadcastShape(സ്ഥിരം MatShape* shps, പൂർണ്ണസംഖ്യ K, MatShape& out)
{
    പൂർണ്ണസംഖ്യ nd = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
    {
        സ്ഥിരം MatShape& s = shps[k];
        പൂർണ്ണസംഖ്യ off = nd - s.dims;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < s.dims; i++)
        {
            പൂർണ്ണസംഖ്യ d = s[i], &o = out[off + i];
            എങ്കിൽ (o == 1) o = d;
            അല്ലെങ്കിൽ എങ്കിൽ (d != 1 && d != o) തിരികെ അസത്യം;
        }
    }
    തിരികെ സത്യം;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
സ്ഥിതം പൂർണ്ണസംഖ്യ opCost(മുകൾ op)
{
    തിരഞ്ഞെടുപ്പ് (op)
    {
    അവസ്ഥ OP_ADD: അവസ്ഥ OP_SUB: അവസ്ഥ OP_MUL: അവസ്ഥ OP_MIN: അവസ്ഥ OP_MAX:
    അവസ്ഥ OP_ABSDIFF: അവസ്ഥ OP_AND: അവസ്ഥ OP_OR: അവസ്ഥ OP_XOR: അവസ്ഥ OP_NOT:
    അവസ്ഥ OP_NEG: അവസ്ഥ OP_ABS: അവസ്ഥ OP_CAST: അവസ്ഥ OP_RELU: അവസ്ഥ OP_SELECT:
    അവസ്ഥ OP_CMP_EQ: അവസ്ഥ OP_CMP_NE: അവസ്ഥ OP_CMP_LT:
    അവസ്ഥ OP_CMP_LE: അവസ്ഥ OP_CMP_GT: അവസ്ഥ OP_CMP_GE: തിരികെ 1;
    അവസ്ഥ OP_DIV: അവസ്ഥ OP_SQRT: അവസ്ഥ OP_HYPOT: അവസ്ഥ OP_CONVERT_SCALE: തിരികെ 3;
    അവസ്ഥ OP_SIN: അവസ്ഥ OP_COS: അവസ്ഥ OP_TANH: അവസ്ഥ OP_ERF: അവസ്ഥ OP_ATAN2:
    അവസ്ഥ OP_EXP: അവസ്ഥ OP_LOG: അവസ്ഥ OP_POW:           തിരികെ 6;
    സ്ഥിരസ്ഥിതി:                                         തിരികെ 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
ശൂന്യം TExpr::clear()
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
ശൂന്യം TExpr::dump(std::ostream& os) സ്ഥിരം
{
    auto dn = [](പൂർണ്ണസംഖ്യ d) -> സ്ഥിരം അക്ഷരം* {
        തിരഞ്ഞെടുപ്പ് (d) {
        അവസ്ഥ EW_DEPTH_NONE: തിരികെ "flex";
        അവസ്ഥ CV_8U:  തിരികെ "u8";  അവസ്ഥ CV_8S:  തിരികെ "s8";
        അവസ്ഥ CV_16U: തിരികെ "u16"; അവസ്ഥ CV_16S: തിരികെ "s16";
        അവസ്ഥ CV_32U: തിരികെ "u32"; അവസ്ഥ CV_32S: തിരികെ "s32";
        അവസ്ഥ CV_64U: തിരികെ "u64"; അവസ്ഥ CV_64S: തിരികെ "s64";
        അവസ്ഥ CV_16F: തിരികെ "f16"; അവസ്ഥ CV_16BF:തിരികെ "bf16";
        അവസ്ഥ CV_32F: തിരികെ "f32"; അവസ്ഥ CV_64F: തിരികെ "f64";
        അവസ്ഥ CV_Bool:തിരികെ "bool";
        സ്ഥിരസ്ഥിതി:     തിരികെ "?"; }
    };
    auto kn = [](ArgKind k) -> സ്ഥിരം അക്ഷരം* {
        തിരഞ്ഞെടുപ്പ് (k) { അവസ്ഥ NONE: തിരികെ "none"; അവസ്ഥ നിവേശം: തിരികെ "in"; അവസ്ഥ CONST: തിരികെ "const";
                     അവസ്ഥ TEMP: തിരികെ "temp"; അവസ്ഥ OUTPUT: തിരികെ "out"; } തിരികെ "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (പൂർണ്ണസംഖ്യ)prog.size() << "\n";
    os << "  slots:\n";
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < (പൂർണ്ണസംഖ്യ)arginfo.size(); s++) {
        സ്ഥിരം Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        എങ്കിൽ (a.kind != NONE) os << " " << dn(a.depth);
        എങ്കിൽ (a.kind == നിവേശം || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        എങ്കിൽ (a.kind == CONST) {
            സ്ഥിരം പൂർണ്ണസംഖ്യ cn = std::max(1, a.channels);
            AutoBuffer<ഇരട്ട, MAX_LOCAL_CN> v(cn);
            സ്ഥിരം uchar* src = (സ്ഥിരം uchar*)(constbuf.data() + a.constofs);
            എങ്കിൽ (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * വലുപ്പം(ഇരട്ട));
            അല്ലെങ്കിൽ getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)prog.size(); i++) {
        സ്ഥിരം Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        എങ്കിൽ (ins.arg1) os << ", " << ins.arg1;
        എങ്കിൽ (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        എങ്കിൽ (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        എങ്കിൽ (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        എങ്കിൽ (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
പൂർണ്ണസംഖ്യ TExpr::add_നിവേശം(പൂർണ്ണസംഖ്യ depth)
{
    Arg a; a.kind = നിവേശം; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); തിരികെ (പൂർണ്ണസംഖ്യ)arginfo.size() - 1;
}

പൂർണ്ണസംഖ്യ TExpr::addOutput(പൂർണ്ണസംഖ്യ depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); തിരികെ (പൂർണ്ണസംഖ്യ)arginfo.size() - 1;
}

പൂർണ്ണസംഖ്യ TExpr::addTemp(പൂർണ്ണസംഖ്യ depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); തിരികെ (പൂർണ്ണസംഖ്യ)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
പൂർണ്ണസംഖ്യ TExpr::addConst(പൂർണ്ണസംഖ്യ depth, സ്ഥിരം Scalar& v, പൂർണ്ണസംഖ്യ channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); തിരികെ (പൂർണ്ണസംഖ്യ)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
പൂർണ്ണസംഖ്യ TExpr::addConst(പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ srcdepth, സ്ഥിരം ശൂന്യം* data, പൂർണ്ണസംഖ്യ channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); തിരികെ (പൂർണ്ണസംഖ്യ)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
പൂർണ്ണസംഖ്യ TExpr::typedConstFrom(പൂർണ്ണസംഖ്യ srcSlot, പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിരം Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); തിരികെ (പൂർണ്ണസംഖ്യ)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
പൂർണ്ണസംഖ്യ TExpr::addInsn(മുകൾ op, പൂർണ്ണസംഖ്യ a0, പൂർണ്ണസംഖ്യ a1, പൂർണ്ണസംഖ്യ a2, പൂർണ്ണസംഖ്യ result, സ്ഥിരം TKernel& kernel, സ്ഥിരം Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    തിരികെ (പൂർണ്ണസംഖ്യ)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
പൂർണ്ണസംഖ്യ TExpr::addInsn(മുകൾ op, പൂർണ്ണസംഖ്യ a0, പൂർണ്ണസംഖ്യ a1, പൂർണ്ണസംഖ്യ a2, പൂർണ്ണസംഖ്യ result, സ്ഥിരം Scalar& params)
{
    പൂർണ്ണസംഖ്യ d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    പൂർണ്ണസംഖ്യ d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    പൂർണ്ണസംഖ്യ d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    തിരികെ addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
പൂർണ്ണസംഖ്യ TExpr::maybeAddCast(പൂർണ്ണസംഖ്യ arg, പൂർണ്ണസംഖ്യ depth)
{
    എങ്കിൽ (arginfo[arg].depth == depth) തിരികെ arg;
    പൂർണ്ണസംഖ്യ t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    തിരികെ t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
സ്ഥിതം വരിയിൽ ശൂന്യം runInsn(സ്ഥിരം TExpr::Insn& ins,
                           സ്ഥിരം ശൂന്യം* p0, size_t y0, size_t x0,
                           സ്ഥിരം ശൂന്യം* p1, size_t y1, size_t x1,
                           സ്ഥിരം ശൂന്യം* p2, size_t y2, size_t x2,
                           ശൂന്യം* pr, size_t yr, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പൂർണ്ണസംഖ്യ code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

ഘടന EwBody {
    സ്ഥിരം TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    സ്ഥിരം പൂർണ്ണസംഖ്യ* slotMap;
    സ്ഥിരം signed അക്ഷരം* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    സ്ഥിരം പൂർണ്ണസംഖ്യ* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    പൂർണ്ണസംഖ്യ ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

ശൂന്യം TExpr::outputShape(സ്ഥിരം Mat* സ്ഥിരം* inputs, MatShape& spatial, പൂർണ്ണസംഖ്യ& channels) സ്ഥിരം
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    സ്ഥിരം പൂർണ്ണസംഖ്യ rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    തർക്കം sameShape = ninputs >= 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; sameShape && i < ninputs; i++)
    {
        സ്ഥിരം Mat& a = *inputs[i];
        എങ്കിൽ (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = അസത്യം;
        അല്ലെങ്കിൽ വേണ്ടി (പൂർണ്ണസംഖ്യ d = 0; d < a.dims; d++) എങ്കിൽ (a.size[d] != inputs[0]->size[d]) { sameShape = അസത്യം; നിർത്തുക; }
    }
    എങ്കിൽ (sameShape) { spatial = inputs[0]->size; channels = rcn0; തിരികെ; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; പൂർണ്ണസംഖ്യ esz1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    സ്ഥിരം പൂർണ്ണസംഖ്യ ndFull = (പൂർണ്ണസംഖ്യ)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ശൂന്യം TExpr::outputShape(സ്ഥിരം Mat* inputs, MatShape& spatial, പൂർണ്ണസംഖ്യ& channels) സ്ഥിരം
{
    AutoBuffer<സ്ഥിരം Mat*, 16> ptrs(std::max(ninputs, 1));
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ LOCAL_HDRS = 3;
സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ LOCAL_CONSTS = 4;
സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ LOCAL_OPS = 16;

ശൂന്യം TExpr::exec(സ്ഥിരം Mat* സ്ഥിരം* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    സ്ഥിരം പൂർണ്ണസംഖ്യ nslots = (പൂർണ്ണസംഖ്യ)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    എങ്കിൽ (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        സ്ഥിരം Mat& r = *inputs[0];
        സ്ഥിരം പൂർണ്ണസംഖ്യ rcn = r.channels();
        തർക്കം ok = r.isContinuous();
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; ok && i < ninputs; i++)
        {
            സ്ഥിരം Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        വേണ്ടി (പൂർണ്ണസംഖ്യ s = 1; ok && s < nslots; s++)
            എങ്കിൽ (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        സ്ഥിരം size_t total = ok ? r.total() * rcn : 0;
        എങ്കിൽ (ok && total == 0)
        {
            സ്ഥിരം MatShape rshape = r.size;
            വേണ്ടി (പൂർണ്ണസംഖ്യ s = 1; s < nslots; s++)
                എങ്കിൽ (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            തിരികെ;
        }
        എങ്കിൽ (ok && total <= EW_FASTPATH_MAX)
        {
            സ്ഥിരം MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            വേണ്ടി (പൂർണ്ണസംഖ്യ s = 1; s < nslots; s++)
                എങ്കിൽ (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            സ്ഥിരം പൂർണ്ണസംഖ്യ ninsn = (പൂർണ്ണസംഖ്യ)prog.size();

            എങ്കിൽ (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](പൂർണ്ണസംഖ്യ s) -> ശൂന്യം* {
                    എങ്കിൽ (s <= 0) തിരികെ nullptr;
                    സ്ഥിരം Arg& ai = arginfo[s];
                    എങ്കിൽ (ai.kind == നിവേശം)  തിരികെ (ശൂന്യം*)inputs[ai.index]->data;
                    എങ്കിൽ (ai.kind == OUTPUT) തിരികെ (ശൂന്യം*)outputs[ai.index].data;
                    തിരികെ nullptr;   // NONE
                };
                സ്ഥിരം പൂർണ്ണസംഖ്യ w = (പൂർണ്ണസംഖ്യ)total;
                വേണ്ടി (പൂർണ്ണസംഖ്യ n = 0; n < ninsn; n++)
                {
                    സ്ഥിരം Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                തിരികെ;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            സ്ഥിരം പൂർണ്ണസംഖ്യ totalEsz = bufEszPrefix[nbuffers];
            സ്ഥിരം പൂർണ്ണസംഖ്യ wf0 = std::min((പൂർണ്ണസംഖ്യ)total, capElems);

            സ്ഥിരം size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/വലുപ്പം(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)വലുപ്പം(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            വേണ്ടി (പൂർണ്ണസംഖ്യ x0 = 0; x0 < (പൂർണ്ണസംഖ്യ)total; x0 += wf0)
            {
                സ്ഥിരം പൂർണ്ണസംഖ്യ wf = std::min(wf0, (പൂർണ്ണസംഖ്യ)total - x0);
                auto ptrOf = [&](പൂർണ്ണസംഖ്യ s) -> ശൂന്യം* {
                    എങ്കിൽ (s <= 0) തിരികെ nullptr;
                    സ്ഥിരം Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    എങ്കിൽ (ai.kind == നിവേശം)  തിരികെ (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    എങ്കിൽ (ai.kind == OUTPUT) തിരികെ (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    എങ്കിൽ (ai.kind != TEMP)   തിരികെ nullptr;                                    // NONE: moved-from
                    സ്ഥിരം പൂർണ്ണസംഖ്യ b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    തിരികെ scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                വേണ്ടി (പൂർണ്ണസംഖ്യ n = 0; n < ninsn; n++)
                {
                    സ്ഥിരം Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            തിരികെ;
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
    പൂർണ്ണസംഖ്യ rchannels;
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
    തർക്കം inplace = അസത്യം;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninputs && !inplace; i++)
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < noutputs; j++)
            എങ്കിൽ (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = സത്യം; നിർത്തുക; }
    പൂർണ്ണസംഖ്യ nhdrs = inplace ? ninputs : 0;

    AutoBuffer<സ്ഥിരം Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<സ്ഥിരം Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<പൂർണ്ണസംഖ്യ, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed അക്ഷരം, LOCAL_OPS> slotKind(nslots);
    പൂർണ്ണസംഖ്യ narr = 0, nc = 0;
    എങ്കിൽ (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed അക്ഷരം)TExpr::NONE;
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 1; s < nslots; s++)
    {
        സ്ഥിരം TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed അക്ഷരം)ai.kind;
        എങ്കിൽ (ai.kind == TExpr::നിവേശം) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        അല്ലെങ്കിൽ എങ്കിൽ (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        അല്ലെങ്കിൽ എങ്കിൽ (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed അക്ഷരം)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            സ്ഥിരം പൂർണ്ണസംഖ്യ c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (ശൂന്യം*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed അക്ഷരം)TExpr::നിവേശം;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
        }
        അല്ലെങ്കിൽ {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    സ്ഥിരം പൂർണ്ണസംഖ്യ ninsn = (പൂർണ്ണസംഖ്യ)prog.size();
    long long costPerElem = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    വേണ്ടി (പൂർണ്ണസംഖ്യ d = 0; d < (പൂർണ്ണസംഖ്യ)spatial.size(); d++) otot *= spatial[d];
    എങ്കിൽ (otot == 0)
        തിരികെ;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    സ്ഥിരം ഇരട്ട T = (ഇരട്ട)std::max(get_അക്കം_Threads(), 1);
    സ്ഥിരം ഇരട്ട nstripes = std::min(
        (ഇരട്ട)otot * (ഇരട്ട)std::max<long long>(costPerElem, 1) * (1./ (ഇരട്ട)(1 << 16)),
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

    broadcastOp(arr.data(), narr, [&](സ്ഥിരം BrTile& tile)
    {
        EwBody& bc = body;
        സ്ഥിരം TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        സ്ഥിരം പൂർണ്ണസംഖ്യ* സ്ഥിരം smap  = bc.slotMap;
        സ്ഥിരം signed അക്ഷരം* സ്ഥിരം skind = bc.slotKind;
        സ്ഥിരം പൂർണ്ണസംഖ്യ nin = bc.ninsn, nsl = bc.nslots;
        സ്ഥിരം പൂർണ്ണസംഖ്യ w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        സ്ഥിരം പൂർണ്ണസംഖ്യ* സ്ഥിരം eszPrefix = bc.bufEszPrefix;
        സ്ഥിരം പൂർണ്ണസംഖ്യ bw = std::min(w, bc.capElems);
        സ്ഥിരം പൂർണ്ണസംഖ്യ bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        സ്ഥിരം size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/വലുപ്പം(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)വലുപ്പം(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        വേണ്ടി (പൂർണ്ണസംഖ്യ y0 = 0; y0 < h; y0 += bh)
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ hf = std::min(bh, h - y0);
            വേണ്ടി (പൂർണ്ണസംഖ്യ x0 = 0; x0 < w; x0 += bw)
            {
                സ്ഥിരം പൂർണ്ണസംഖ്യ wf = std::min(bw, w - x0);
                വേണ്ടി (പൂർണ്ണസംഖ്യ s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    സ്ഥിരം പൂർണ്ണസംഖ്യ k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    എങ്കിൽ (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    അല്ലെങ്കിൽ                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        സ്ഥിരം BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                വേണ്ടി (പൂർണ്ണസംഖ്യ n = 0; n < nin; n++)
                {
                    സ്ഥിരം TExpr::Insn& ins = insns[n];
                    സ്ഥിരം BrSlice& a0 = args[ins.arg0]; സ്ഥിരം BrSlice& a1 = args[ins.arg1];
                    സ്ഥിരം BrSlice& a2 = args[ins.arg2]; സ്ഥിരം BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (ശൂന്യം*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, സത്യം, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
ശൂന്യം TExpr::exec(സ്ഥിരം Mat* inputs, Mat* outputs)
{
    AutoBuffer<സ്ഥിരം Mat*, 16> ptrs(std::max(ninputs, 1));
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
ശൂന്യം make_ദ്വിമാന_Arith_Program(TExpr& p, മുകൾ op, പൂർണ്ണസംഖ്യ depth0, പൂർണ്ണസംഖ്യ depth1, പൂർണ്ണസംഖ്യ rdepth,
                            പൂർണ്ണസംഖ്യ maskDepth, ഇരട്ട scale)
{
    p.clear();
    എങ്കിൽ (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        എങ്കിൽ (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        അല്ലെങ്കിൽ
        {
            rdepth = promoteArith(depth0, depth1);
            എങ്കിൽ (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    സ്ഥിരം തർക്കം masked = maskDepth != EW_DEPTH_NONE;
    പൂർണ്ണസംഖ്യ sIn0  = p.add_നിവേശം(depth0);
    പൂർണ്ണസംഖ്യ sIn1  = p.add_നിവേശം(depth1);
    പൂർണ്ണസംഖ്യ sMask = masked ? p.add_നിവേശം(maskDepth) : 0;
    പൂർണ്ണസംഖ്യ sOut  = p.addOutput(rdepth);

    പൂർണ്ണസംഖ്യ r = p.emit_ദ്വിമാന(op, sIn0, sIn1, rdepth, Scalar(scale));
    എങ്കിൽ (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    അല്ലെങ്കിൽ
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
ശൂന്യം makeAddWeightedProgram(TExpr& p, പൂർണ്ണസംഖ്യ depth0, പൂർണ്ണസംഖ്യ depth1, പൂർണ്ണസംഖ്യ rdepth,
                            ഇരട്ട alpha, ഇരട്ട beta, ഇരട്ട gamma)
{
    p.clear();
    പൂർണ്ണസംഖ്യ sA   = p.add_നിവേശം(depth0);
    പൂർണ്ണസംഖ്യ sB   = p.add_നിവേശം(depth1);
    പൂർണ്ണസംഖ്യ sOut = p.addOutput(rdepth);
    പൂർണ്ണസംഖ്യ r = p.emit_ദ്വിമാന(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

നാമാകാശ {

// --- tokens ------------------------------------------------------------------------------
എണ്ണൽ TokType { T_അക്കം, T_നിവേശം, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

ഘടന Token
{
    TokType type = T_END;
    ഇരട്ട num = 0;
    പൂർണ്ണസംഖ്യ നിവേശം_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
ഘടന Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    സ്ഥിതം തർക്കം isIdentStart(അക്ഷരം c) { തിരികെ std::isalpha((unsigned അക്ഷരം)c) || c == '_'; }
    സ്ഥിതം തർക്കം isIdentChar(അക്ഷരം c)  { തിരികെ std::isalnum((unsigned അക്ഷരം)c) || c == '_'; }

    Token next()
    {
        ആകുമ്പോൾ (pos < s.size() && std::isspace((unsigned അക്ഷരം)s[pos])) pos++;
        Token t;
        എങ്കിൽ (pos >= s.size()) { t.type = T_END; തിരികെ t; }

        അക്ഷരം c = s[pos];

        എങ്കിൽ (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            ആകുമ്പോൾ (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_നിവേശം;
            t.നിവേശം_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            തിരികെ t;
        }
        എങ്കിൽ (std::isdigit((unsigned അക്ഷരം)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned അക്ഷരം)s[pos + 1])))
        {
            അക്ഷരം* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_അക്കം;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            തിരികെ t;
        }
        എങ്കിൽ (isIdentStart(c))
        {
            size_t start = pos;
            ആകുമ്പോൾ (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            തിരികെ t;
        }
        തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ '(': pos++; t.type = T_LPAREN; തിരികെ t;
        അവസ്ഥ ')': pos++; t.type = T_RPAREN; തിരികെ t;
        അവസ്ഥ ',': pos++; t.type = T_COMMA;  തിരികെ t;
        അവസ്ഥ ';': pos++; t.type = T_SEMI;   തിരികെ t;
        }
        // multi/!single-char operators
        auto two = [&](സ്ഥിരം അക്ഷരം* op) {
            തിരികെ pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        എങ്കിൽ (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; തിരികെ t; }
        എങ്കിൽ (c == '=') { pos++; t.type = T_ASSIGN; തിരികെ t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        തിരികെ t;
    }
};

// --- operator / function tables ----------------------------------------------------------
സ്ഥിതം പൂർണ്ണസംഖ്യ binPrec(സ്ഥിരം std::string& op)
{
    എങ്കിൽ (op == "**") തിരികെ 8;              // power binds tighter than '*'; RIGHT-associative
    എങ്കിൽ (op == "*" || op == "/") തിരികെ 7;
    എങ്കിൽ (op == "+" || op == "-") തിരികെ 6;
    എങ്കിൽ (op == "<" || op == "<=" || op == ">" || op == ">=") തിരികെ 5;
    എങ്കിൽ (op == "==" || op == "!=") തിരികെ 4;
    എങ്കിൽ (op == "&") തിരികെ 3;
    എങ്കിൽ (op == "^") തിരികെ 2;
    എങ്കിൽ (op == "|") തിരികെ 1;
    തിരികെ -1;
}

സ്ഥിതം മുകൾ binOp(സ്ഥിരം std::string& op)
{
    എങ്കിൽ (op == "**") തിരികെ OP_POW;
    എങ്കിൽ (op == "+")  തിരികെ OP_ADD;
    എങ്കിൽ (op == "-")  തിരികെ OP_SUB;
    എങ്കിൽ (op == "*")  തിരികെ OP_MUL;
    എങ്കിൽ (op == "/")  തിരികെ OP_DIV;
    എങ്കിൽ (op == "<")  തിരികെ OP_CMP_LT;
    എങ്കിൽ (op == "<=") തിരികെ OP_CMP_LE;
    എങ്കിൽ (op == ">")  തിരികെ OP_CMP_GT;
    എങ്കിൽ (op == ">=") തിരികെ OP_CMP_GE;
    എങ്കിൽ (op == "==") തിരികെ OP_CMP_EQ;
    എങ്കിൽ (op == "!=") തിരികെ OP_CMP_NE;
    എങ്കിൽ (op == "&")  തിരികെ OP_AND;
    എങ്കിൽ (op == "|")  തിരികെ OP_OR;
    എങ്കിൽ (op == "^")  തിരികെ OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
സ്ഥിതം പൂർണ്ണസംഖ്യ typeDepth(സ്ഥിരം std::string& name)
{
    എങ്കിൽ (name == "float")    തിരികെ CV_32F;
    എങ്കിൽ (name == "double")   തിരികെ CV_64F;
    എങ്കിൽ (name == "half" || name == "float16") തിരികെ CV_16F;
    എങ്കിൽ (name == "bfloat16") തിരികെ CV_16BF;
    എങ്കിൽ (name == "uint8")    തിരികെ CV_8U;
    എങ്കിൽ (name == "int8")     തിരികെ CV_8S;
    എങ്കിൽ (name == "uint16")   തിരികെ CV_16U;
    എങ്കിൽ (name == "int16")    തിരികെ CV_16S;
    എങ്കിൽ (name == "uint32")   തിരികെ CV_32U;
    എങ്കിൽ (name == "int32")    തിരികെ CV_32S;
    എങ്കിൽ (name == "uint64")   തിരികെ CV_64U;
    എങ്കിൽ (name == "int64")    തിരികെ CV_64S;
    തിരികെ -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
സ്ഥിതം മുകൾ fnOp(സ്ഥിരം std::string& name, പൂർണ്ണസംഖ്യ& arity)
{
    ഘടന E { സ്ഥിരം അക്ഷരം* n; മുകൾ op; };
    സ്ഥിതം സ്ഥിരം E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    സ്ഥിതം സ്ഥിരം E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    സ്ഥിതം സ്ഥിരം E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    വേണ്ടി (സ്ഥിരം E& e : unary)  എങ്കിൽ (name == e.n) { arity = 1; തിരികെ e.op; }
    വേണ്ടി (സ്ഥിരം E& e : binary) എങ്കിൽ (name == e.n) { arity = 2; തിരികെ e.op; }
    വേണ്ടി (സ്ഥിരം E& e : tern)   എങ്കിൽ (name == e.n) { arity = 3; തിരികെ e.op; }
    arity = 0; തിരികെ OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
ഘടന Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    സ്ഥിരം പൂർണ്ണസംഖ്യ* നിവേശം_Slot;                 // slot id of each external input (precreated)
    പൂർണ്ണസംഖ്യ ninputs;
    std::map<std::string, പൂർണ്ണസംഖ്യ> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& വ്യഞ്ജകം, സ്ഥിരം പൂർണ്ണസംഖ്യ* islot, പൂർണ്ണസംഖ്യ nin)
        : lex(src), e(വ്യഞ്ജകം), നിവേശം_Slot(islot), ninputs(nin) { cur = lex.next(); }

    ശൂന്യം advance() { cur = lex.next(); }
    തർക്കം isOp(സ്ഥിരം അക്ഷരം* op) സ്ഥിരം { തിരികെ cur.type == T_OP && cur.text == op; }

    ശൂന്യം expect(TokType t, സ്ഥിരം അക്ഷരം* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    തർക്കം isFlexConst(പൂർണ്ണസംഖ്യ slot) സ്ഥിരം
    {
        തിരികെ e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    പൂർണ്ണസംഖ്യ parsePrimary()
    {
        എങ്കിൽ (cur.type == T_അക്കം)   { പൂർണ്ണസംഖ്യ s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); തിരികെ s; }
        എങ്കിൽ (cur.type == T_നിവേശം) { പൂർണ്ണസംഖ്യ idx = cur.നിവേശം_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   തിരികെ നിവേശം_Slot[idx]; }
        എങ്കിൽ (cur.type == T_LPAREN){ advance(); പൂർണ്ണസംഖ്യ x = parseTernary(); expect(T_RPAREN, "expected ')'"); തിരികെ x; }
        എങ്കിൽ (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            എങ്കിൽ (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                തിരികെ it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<പൂർണ്ണസംഖ്യ, 8> args = {}; പൂർണ്ണസംഖ്യ nargs = 0;
            എങ്കിൽ (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                ആകുമ്പോൾ (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (പൂർണ്ണസംഖ്യ)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            പൂർണ്ണസംഖ്യ td = typeDepth(name);
            എങ്കിൽ (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                പൂർണ്ണസംഖ്യ s = args[0];
                തിരികെ isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            പൂർണ്ണസംഖ്യ arity = 0; മുകൾ op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            എങ്കിൽ (arity == 1) തിരികെ e.emitUnary(op, args[0]);
            എങ്കിൽ (arity == 2) തിരികെ e.emit_ദ്വിമാന(op, args[0], args[1]);
            തിരികെ e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    പൂർണ്ണസംഖ്യ parseUnary()
    {
        എങ്കിൽ (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            പൂർണ്ണസംഖ്യ operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            എങ്കിൽ (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<ഇരട്ട, MAX_LOCAL_CN> v; പൂർണ്ണസംഖ്യ cn = constDoubles(e, operand, v);
                Scalar neg; വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                തിരികെ e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            തിരികെ e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        തിരികെ parsePrimary();
    }

    പൂർണ്ണസംഖ്യ parse_വ്യഞ്ജകം(പൂർണ്ണസംഖ്യ minPrec)
    {
        പൂർണ്ണസംഖ്യ ഇടത് = parseUnary();
        ആകുമ്പോൾ (cur.type == T_OP)
        {
            പൂർണ്ണസംഖ്യ p = binPrec(cur.text);
            എങ്കിൽ (p < minPrec) നിർത്തുക;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            പൂർണ്ണസംഖ്യ വലത് = parse_വ്യഞ്ജകം(op == "**" ? p : p + 1);
            ഇടത് = e.emit_ദ്വിമാന(binOp(op), ഇടത്, വലത്);
        }
        തിരികെ ഇടത്;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    പൂർണ്ണസംഖ്യ parseTernary()
    {
        പൂർണ്ണസംഖ്യ cond = parse_വ്യഞ്ജകം(0);
        എങ്കിൽ (!isOp("?")) തിരികെ cond;
        advance();
        പൂർണ്ണസംഖ്യ thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        പൂർണ്ണസംഖ്യ elsev = parseTernary();               // right-associative chain
        തിരികെ e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    ശൂന്യം parseResult()
    {
        എങ്കിൽ (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); പൂർണ്ണസംഖ്യ nTemp = e.ntemps;
            advance();
            പൂർണ്ണസംഖ്യ e0 = parseTernary();
            എങ്കിൽ (cur.type == T_COMMA)
            {
                e.output(e0);
                ആകുമ്പോൾ (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                തിരികെ;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    ശൂന്യം parse()
    {
        ആകുമ്പോൾ (സത്യം)
        {
            എങ്കിൽ (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                എങ്കിൽ (cur.type == T_ASSIGN)
                {
                    advance();
                    സ്ഥിരം പൂർണ്ണസംഖ്യ slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = സത്യം;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    തുടരുക;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            എങ്കിൽ (cur.type == T_SEMI) advance();
            നിർത്തുക;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

നാമാകാശ cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
ശൂന്യം texpr(സ്ഥിരം std::string& വ്യഞ്ജകം, നിവേശം_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using നാമാകാശ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == നിവേശം_Array_2::STD_VECTOR_MAT);
    സ്ഥിരം std::vector<Mat>& inps = *(സ്ഥിരം std::vector<Mat>*)_inputs.getObj();
    സ്ഥിരം പൂർണ്ണസംഖ്യ ninputs = (പൂർണ്ണസംഖ്യ)inps.size();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<പൂർണ്ണസംഖ്യ> islot(std::max(ninputs, 1));
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ninputs; i++) islot[i] = e.add_നിവേശം(inps[i].depth());
    Parser(വ്യഞ്ജകം, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    എങ്കിൽ (kind == നിവേശം_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } അല്ലെങ്കിൽ {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
