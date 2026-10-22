// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "arithm_expr.hpp"
#अन्तर्भाव <algorithm>
#अन्तर्भाव <climits>
#अन्तर्भाव <cmath>
#अन्तर्भाव <cstdlib>
#अन्तर्भाव <cstring>
#अन्तर्भाव <map>
#अन्तर्भाव <ostream>
#अन्तर्भाव <string>

नामाकाश cv { नामाकाश ew {

// ============================ op metadata (was ew_op.cpp) ============================

ध्रुव वर्ण* opName(शीर्ष op)
{
    विकल्प (op)
    {
    पक्ष OP_NOP:           प्रत्यागम "nop";
    पक्ष OP_NEG:           प्रत्यागम "neg";
    पक्ष OP_ABS:           प्रत्यागम "abs";
    पक्ष OP_NOT:           प्रत्यागम "not";
    पक्ष OP_SQRT:          प्रत्यागम "sqrt";
    पक्ष OP_EXP:           प्रत्यागम "exp";
    पक्ष OP_LOG:           प्रत्यागम "log";
    पक्ष OP_SIN:           प्रत्यागम "sin";
    पक्ष OP_COS:           प्रत्यागम "cos";
    पक्ष OP_TANH:          प्रत्यागम "tanh";
    पक्ष OP_ERF:           प्रत्यागम "erf";
    पक्ष OP_RELU:          प्रत्यागम "relu";
    पक्ष OP_CAST:          प्रत्यागम "cast";
    पक्ष OP_ADD:           प्रत्यागम "add";
    पक्ष OP_SUB:           प्रत्यागम "sub";
    पक्ष OP_MUL:           प्रत्यागम "mul";
    पक्ष OP_DIV:           प्रत्यागम "div";
    पक्ष OP_POW:           प्रत्यागम "pow";
    पक्ष OP_MIN:           प्रत्यागम "min";
    पक्ष OP_MAX:           प्रत्यागम "max";
    पक्ष OP_ABSDIFF:       प्रत्यागम "absdiff";
    पक्ष OP_ADDW:          प्रत्यागम "addWeighted";
    पक्ष OP_HYPOT:         प्रत्यागम "hypot";
    पक्ष OP_ATAN2:         प्रत्यागम "atan2";
    पक्ष OP_AND:           प्रत्यागम "and";
    पक्ष OP_OR:            प्रत्यागम "or";
    पक्ष OP_XOR:           प्रत्यागम "xor";
    पक्ष OP_CMP_EQ:        प्रत्यागम "cmp_eq";
    पक्ष OP_CMP_NE:        प्रत्यागम "cmp_ne";
    पक्ष OP_CMP_LT:        प्रत्यागम "cmp_lt";
    पक्ष OP_CMP_LE:        प्रत्यागम "cmp_le";
    पक्ष OP_CMP_GT:        प्रत्यागम "cmp_gt";
    पक्ष OP_CMP_GE:        प्रत्यागम "cmp_ge";
    पक्ष OP_CLAMP:         प्रत्यागम "clamp";
    पक्ष OP_SELECT:        प्रत्यागम "select";
    पक्ष OP_CONVERT_SCALE: प्रत्यागम "convert_scale";
    स्वभाव:               प्रत्यागम "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(शीर्ष op)
{
    विकल्प (op)
    {
    पक्ष OP_AND: पक्ष OP_OR: पक्ष OP_XOR: पक्ष OP_NOT:
        प्रत्यागम CAT_BITWISE;
    पक्ष OP_CMP_EQ: पक्ष OP_CMP_NE: पक्ष OP_CMP_LT:
    पक्ष OP_CMP_LE: पक्ष OP_CMP_GT: पक्ष OP_CMP_GE:
        प्रत्यागम CAT_COMPARE;
    पक्ष OP_SQRT: पक्ष OP_EXP: पक्ष OP_LOG:
    पक्ष OP_SIN: पक्ष OP_COS: पक्ष OP_TANH: पक्ष OP_ERF: पक्ष OP_ATAN2: पक्ष OP_RELU:
        प्रत्यागम CAT_MATH;
    पक्ष OP_CAST: पक्ष OP_CONVERT_SCALE:
        प्रत्यागम CAT_CAST;
    पक्ष OP_SELECT:
        प्रत्यागम CAT_SELECT;
    स्वभाव:
        प्रत्यागम CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
स्थिर तार्किक isFloatDepth(पूर्णाङ्क d)
{
    प्रत्यागम d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
स्थिर तार्किक depthRepresents(द्विगुण v, पूर्णाङ्क depth)
{
    यदि (isFloatDepth(depth)) प्रत्यागम सत्य;
    यदि (v != std::floor(v)) प्रत्यागम असत्य;
    विकल्प (depth)
    {
    पक्ष CV_8U:  प्रत्यागम v >= 0            && v <= 255;
    पक्ष CV_8S:  प्रत्यागम v >= -128         && v <= 127;
    पक्ष CV_16U: प्रत्यागम v >= 0            && v <= 65535;
    पक्ष CV_16S: प्रत्यागम v >= -32768       && v <= 32767;
    पक्ष CV_32U: प्रत्यागम v >= 0            && v <= 4294967295.0;
    पक्ष CV_32S: प्रत्यागम v >= -2147483648.0 && v <= 2147483647.0;
    पक्ष CV_64U: प्रत्यागम v >= 0            && v <= 18446744073709551615.0;
    पक्ष CV_64S: प्रत्यागम v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    स्वभाव:     प्रत्यागम असत्य;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
स्थिर शून्य intRange(पूर्णाङ्क depth, द्विगुण& lo, द्विगुण& hi)
{
    विकल्प (depth)
    {
    पक्ष CV_8U:  lo = 0;                     hi = 255; भङ्ग;
    पक्ष CV_8S:  lo = -128;                  hi = 127; भङ्ग;
    पक्ष CV_16U: lo = 0;                     hi = 65535; भङ्ग;
    पक्ष CV_16S: lo = -32768;                hi = 32767; भङ्ग;
    पक्ष CV_32U: lo = 0;                     hi = 4294967295.0; भङ्ग;
    पक्ष CV_32S: lo = -2147483648.0;         hi = 2147483647.0; भङ्ग;
    पक्ष CV_64U: lo = 0;                     hi = 18446744073709551615.0; भङ्ग;
    पक्ष CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; भङ्ग;
    स्वभाव:     lo = 0;                     hi = 0; भङ्ग;
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
पूर्णाङ्क promoteArith(पूर्णाङ्क a, पूर्णाङ्क b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    यदि (a == b) प्रत्यागम a;

    पूर्णाङ्क wa = पूर्णाङ्क((leszlut >> (a+1)*3) & 7u), wb = पूर्णाङ्क((leszlut >> (b+1)*3) & 7u);
    पूर्णाङ्क fa = पूर्णाङ्क((fltlut >> (a+1)) & 1u),    fb = पूर्णाङ्क((fltlut >> (b+1)) & 1u);

    यदि (fa + fb == 0)                          // both integer
    {
        पूर्णाङ्क sa = पूर्णाङ्क((signlut >> (a+1)) & 1u), sb = पूर्णाङ्क((signlut >> (b+1)) & 1u);
        यदि (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            प्रत्यागम wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        पूर्णाङ्क rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr पूर्णाङ्क ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        प्रत्यागम (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    पूर्णाङ्क maxw = std::max(wa, wb);
    यदि (maxw == 2 && fa + fb == 1)
        प्रत्यागम fa*a + (1 - fa)*b;
    प्रत्यागम CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
पूर्णाङ्क absdiffResultDepth(पूर्णाङ्क depth)
{
    विकल्प (depth)
    {
    पक्ष CV_8S:  प्रत्यागम CV_8U;
    पक्ष CV_16S: प्रत्यागम CV_16U;
    पक्ष CV_32S: प्रत्यागम CV_32U;
    पक्ष CV_64S: प्रत्यागम CV_64U;
    स्वभाव:     प्रत्यागम depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
स्थिर पूर्णाङ्क promote2(पूर्णाङ्क a, पूर्णाङ्क b)
{
    यदि (a == b) प्रत्यागम a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ध्रुव uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ध्रुव unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    प्रत्यागम पूर्णाङ्क((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
स्थिर पूर्णाङ्क promoteBitwise(पूर्णाङ्क a, पूर्णाङ्क b)
{
    यदि (a == EW_DEPTH_NONE) प्रत्यागम b;
    यदि (b == EW_DEPTH_NONE) प्रत्यागम a;
    यदि (a == b) प्रत्यागम a;
    प्रत्यागम CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
स्थिर पूर्णाङ्क safeWide(पूर्णाङ्क depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    ध्रुव uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    ध्रुव unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    प्रत्यागम पूर्णाङ्क((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
गणना { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
स्थिर size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, पूर्णाङ्क srcdepth, ध्रुव शून्य* data, पूर्णाङ्क channels)
{
    ध्रुव पूर्णाङ्क cn = std::max(1, channels);
    ध्रुव size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    ध्रुव size_t nu64 = (nbytes + परिमाण(uint64_t) - 1) / परिमाण(uint64_t);
    ध्रुव size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    यदि (data) memcpy(dst, data, nbytes); अन्यथा memset(dst, 0, nbytes);
    प्रत्यागम ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
स्थिर पूर्णाङ्क constDoubles(ध्रुव TExpr& e, पूर्णाङ्क s, AutoBuffer<द्विगुण, MAX_LOCAL_CN>& buf)
{
    ध्रुव TExpr::Arg& a = e.arginfo[s];
    ध्रुव पूर्णाङ्क cn = std::max(1, a.channels);
    buf.resize(cn);
    ध्रुव uchar* src = (ध्रुव uchar*)(e.constbuf.data() + a.constofs);
    यदि (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * परिमाण(द्विगुण));   // common (Scalar / parsed literal)
    अन्यथा
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    प्रत्यागम cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
स्थिर अन्तःस्थ तार्किक isFlexConst(ध्रुव TExpr& e, पूर्णाङ्क s)
{
    प्रत्यागम e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
स्थिर तार्किक scalar_Const_मूल्य(ध्रुव TExpr& e, पूर्णाङ्क s, द्विगुण& v)
{
    ध्रुव TExpr::Arg& a = e.arginfo[s];
    यदि (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) प्रत्यागम असत्य;
    AutoBuffer<द्विगुण, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    प्रत्यागम सत्य;
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
स्थिर तार्किक matchScaledChain(ध्रुव TExpr& e, पूर्णाङ्क end, पूर्णाङ्क t,
                             पूर्णाङ्क& arrSlot, द्विगुण& scale, पूर्णाङ्क& startIdx)
{
    यदि (end < 0) प्रत्यागम असत्य;
    पूर्णाङ्क j = end, mulResult = t;

    // optional trailing narrowing cast
    यदि (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        यदि (--j < 0) प्रत्यागम असत्य;
    }

    ध्रुव TExpr::Insn& mul = e.prog[j];
    यदि (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) प्रत्यागम असत्य;

    पूर्णाङ्क arr;
    यदि      (scalar_Const_मूल्य(e, mul.arg1, scale)) arr = mul.arg0;
    अन्यथा यदि (scalar_Const_मूल्य(e, mul.arg0, scale)) arr = mul.arg1;
    अन्यथा प्रत्यागम असत्य;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    यदि (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    यदि (e.arginfo[arr].kind == TExpr::CONST) प्रत्यागम असत्य;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    प्रत्यागम सत्य;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
स्थिर तार्किक constFits(ध्रुव TExpr& e, पूर्णाङ्क s, पूर्णाङ्क d)
{
    यदि (!isFlexConst(e, s)) प्रत्यागम सत्य;
    AutoBuffer<द्विगुण, MAX_LOCAL_CN> v;
    पूर्णाङ्क cn = constDoubles(e, s, v);
    प्रति (पूर्णाङ्क ch = 0; ch < cn; ch++)
        यदि (!depthRepresents(v[ch], d)) प्रत्यागम असत्य;
    प्रत्यागम सत्य;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
पूर्णाङ्क TExpr::emit_द्विमान(शीर्ष op, पूर्णाङ्क a, पूर्णाङ्क b, पूर्णाङ्क rdepth, ध्रुव Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    यदि (op == OP_ADDW)
    {
        पूर्णाङ्क Tt = arginfo[a].depth;
        यदि (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        यदि (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        यदि (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        पूर्णाङ्क outD = rdepth;
        यदि (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        ध्रुव पूर्णाङ्क out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        यदि (outD == rdepth) प्रत्यागम out;
        ध्रुव पूर्णाङ्क out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        प्रत्यागम out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    यदि (op == OP_ADD)
    {
        ध्रुव पूर्णाङ्क n = (पूर्णाङ्क)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        पूर्णाङ्क x = 0, y = 0, startA = 0, startB = 0;
        द्विगुण alpha = 0, beta = 0;
        यदि (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*एतत्, n - 1, b, y, beta, startB) &&
            matchScaledChain(*एतत्, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            पूर्णाङ्क retired = 0, lowest = INT_MAX;
            प्रति (पूर्णाङ्क i = startA; i < n; i++)
            {
                ध्रुव पूर्णाङ्क r = prog[i].result;
                यदि (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; भङ्ग; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            यदि (retired > 0 && lowest == ntemps - retired)
            {
                प्रति (पूर्णाङ्क i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                प्रत्यागम emit_द्विमान(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            पूर्णाङ्क acc = 0; द्विगुण gamma = 0;
            यदि      (scalar_Const_मूल्य(*एतत्, b, gamma)) acc = a;
            अन्यथा यदि (scalar_Const_मूल्य(*एतत्, a, gamma)) acc = b;
            यदि (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                प्रत्यागम acc;
            }
        }
    }

    ध्रुव पूर्णाङ्क nd0 = isFlexConst(*एतत्, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ध्रुव पूर्णाङ्क nd1 = isFlexConst(*एतत्, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    ध्रुव ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    पूर्णाङ्क result = rdepth;
    यदि (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        यदि (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    पूर्णाङ्क base;
    यदि (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    अन्यथा यदि (nd0 == EW_DEPTH_NONE) base = nd1;
    अन्यथा यदि (nd1 == EW_DEPTH_NONE) base = nd0;
    अन्यथा base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    यदि (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*एतत्, b) && !constFits(*एतत्, b, base))
    {
        द्विगुण lo, hi; intRange(nd0, lo, hi);
        यदि (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            गणना { REAL, CFALSE, CTRUE };
            AutoBuffer<द्विगुण, MAX_LOCAL_CN> tv; पूर्णाङ्क cn = constDoubles(*एतत्, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            पूर्णाङ्क kind[4] = {}; द्विगुण bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            ध्रुव शीर्ष fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            प्रति (पूर्णाङ्क c = 0; c < cn; c++)
            {
                ध्रुव द्विगुण t = tv[c]; पूर्णाङ्क k; द्विगुण B = t;
                विकल्प (op)
                {
                पक्ष OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; भङ्ग;
                पक्ष OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; भङ्ग;
                पक्ष OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; भङ्ग;
                पक्ष OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; भङ्ग;
                पक्ष OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; भङ्ग;   // a==non-rep -> false
                स्वभाव:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  भङ्ग;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](पूर्णाङ्क c){ प्रत्यागम kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](पूर्णाङ्क c){ प्रत्यागम kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            ध्रुव शीर्ष u0 = ucOp(0);
            तार्किक uniform = सत्य;
            प्रति (पूर्णाङ्क c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            यदि (uniform)
            {
                Scalar s; प्रति (पूर्णाङ्क c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            अन्यथा
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; पूर्णाङ्क patchFlags = 0;
                प्रति (पूर्णाङ्क c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    ध्रुव पूर्णाङ्क mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    ध्रुव पूर्णाङ्क vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                ध्रुव पूर्णाङ्क out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                प्रत्यागम out;
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
    पूर्णाङ्क depth, wide;
    तार्किक divGuard = असत्य;
    विकल्प (op)
    {
    पक्ष OP_MIN: पक्ष OP_MAX:
    पक्ष OP_AND: पक्ष OP_OR: पक्ष OP_XOR:
        depth = result; wide = result; भङ्ग;
    पक्ष OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        पूर्णाङ्क common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        यदि (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        यदि (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        भङ्ग;
    }
    पक्ष OP_CMP_EQ: पक्ष OP_CMP_NE: पक्ष OP_CMP_LT:
    पक्ष OP_CMP_LE: पक्ष OP_CMP_GT: पक्ष OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; भङ्ग;
    पक्ष OP_MUL: पक्ष OP_DIV:
    {
        depth = base;
        ध्रुव तार्किक w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        यदि (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](पूर्णाङ्क s, पूर्णाङ्क nd) {
                यदि (nd != EW_DEPTH_NONE) प्रत्यागम !isFloatDepth(nd);
                AutoBuffer<द्विगुण, MAX_LOCAL_CN> v; constDoubles(*एतत्, s, v);
                प्रत्यागम v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        भङ्ग;
    }
    पक्ष OP_POW: पक्ष OP_HYPOT: पक्ष OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; भङ्ग;
    स्वभाव:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); भङ्ग;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](पूर्णाङ्क s) {
        यदि (!isFlexConst(*एतत्, s)) प्रत्यागम असत्य;
        AutoBuffer<द्विगुण, MAX_LOCAL_CN> v; पूर्णाङ्क n = constDoubles(*एतत्, s, v);
        प्रति (पूर्णाङ्क ch = 0; ch < n; ch++)
            यदि (v[ch] != std::floor(v[ch])) प्रत्यागम सत्य;
        प्रत्यागम असत्य;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    यदि (cat != CAT_BITWISE && (!constFits(*एतत्, a, depth) || !constFits(*एतत्, b, depth)))
    {
        depth = wide;
        ध्रुव तार्किक frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        यदि (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        यदि (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    पूर्णाङ्क s0 = isFlexConst(*एतत्, a) ? typedConstFrom(a, depth) : a;
    पूर्णाङ्क s1 = isFlexConst(*एतत्, b) ? typedConstFrom(b, depth) : b;
    पूर्णाङ्क c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](पूर्णाङ्क rd) {
        प्रत्यागम op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    यदि (k.fptr)
    {
        पूर्णाङ्क res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        प्रत्यागम res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णाङ्क w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    प्रत्यागम maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
पूर्णाङ्क TExpr::emitUnary(शीर्ष op, पूर्णाङ्क a, पूर्णाङ्क rdepth, ध्रुव Scalar& params)
{
    ध्रुव पूर्णाङ्क nd = isFlexConst(*एतत्, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    ध्रुव ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    यदि (op == OP_NEG)
        प्रत्यागम emit_द्विमान(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    यदि (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        यदि (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            ध्रुव पूर्णाङ्क x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            प्रत्यागम emit_द्विमान(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        प्रत्यागम emit_द्विमान(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    पूर्णाङ्क result = rdepth;
    यदि (result == EW_DEPTH_NONE)
    {
        विकल्प (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        पक्ष CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; भङ्ग;
        पक्ष CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; भङ्ग;  // NOT
        स्वभाव:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; भङ्ग;  // NEG, ABS
        }
    }

    पूर्णाङ्क depth, wide;
    विकल्प (cat)
    {
    पक्ष CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        भङ्ग;
    पक्ष CAT_BITWISE: depth = result; wide = result; भङ्ग;
    स्वभाव:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; भङ्ग;  // NEG/ABS
    }

    यदि (!constFits(*एतत्, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    पूर्णाङ्क s0 = isFlexConst(*एतत्, a) ? typedConstFrom(a, depth) : a;
    पूर्णाङ्क c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    यदि (k.fptr)
    {
        पूर्णाङ्क res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        प्रत्यागम res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णाङ्क w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    प्रत्यागम maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
पूर्णाङ्क TExpr::emitTernary(शीर्ष op, पूर्णाङ्क a, पूर्णाङ्क b, पूर्णाङ्क c, पूर्णाङ्क rdepth)
{
    auto dep = [&](पूर्णाङ्क s) { प्रत्यागम isFlexConst(*एतत्, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    यदि (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        यदि (isFlexConst(*एतत्, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_द्विमान(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        पूर्णाङ्क result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        यदि (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        पूर्णाङ्क sb = isFlexConst(*एतत्, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        पूर्णाङ्क sc = isFlexConst(*एतत्, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        पूर्णाङ्क res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        प्रत्यागम res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    पूर्णाङ्क result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    यदि (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](पूर्णाङ्क s) {
        प्रत्यागम isFlexConst(*एतत्, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    पूर्णाङ्क c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णाङ्क res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    प्रत्यागम res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
पूर्णाङ्क TExpr::moveToOutput(पूर्णाङ्क temp, पूर्णाङ्क out)
{
    ध्रुव पूर्णाङ्क ninsn = (पूर्णाङ्क)prog.size();
    पूर्णाङ्क producer = -1;
    तार्किक usedAsArg = असत्य;
    प्रति (पूर्णाङ्क i = 0; i < ninsn; i++)
    {
        ध्रुव TExpr::Insn& ins = prog[i];
        यदि (ins.result == temp) producer = i;
        यदि (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = सत्य;
    }
    यदि (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        प्रत्यागम out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    प्रत्यागम out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
पूर्णाङ्क TExpr::output(पूर्णाङ्क मूळ_Slot)
{
    प्रत्यागम moveToOutput(मूळ_Slot, addOutput(arginfo[मूळ_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
शून्य TExpr::compile()
{
    ध्रुव पूर्णाङ्क ninsn = (पूर्णाङ्क)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    ध्रुव पूर्णाङ्क nslots = (पूर्णाङ्क)arginfo.size();
    nconsts = 0;
    प्रति (पूर्णाङ्क s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        यदि (a.kind != CONST || a.depth == EW_DEPTH_NONE) अनुवर्त;
        ध्रुव पूर्णाङ्क cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        ध्रुव size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, परिमाण(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)परिमाण(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (ध्रुव uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        ध्रुव size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        यदि (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        अन्यथा getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
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
    यदि (ntemps == 0) प्रत्यागम;

    AutoBuffer<पूर्णाङ्क, 32> tempOfSlot(nslots);
    प्रति (पूर्णाङ्क s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    प्रति (पूर्णाङ्क s = 1; s < nslots; s++)
        यदि (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<पूर्णाङ्क, 16> lastUse(ntemps);
    प्रति (पूर्णाङ्क t = 0; t < ntemps; t++) lastUse[t] = -1;
    प्रति (पूर्णाङ्क i = 0; i < ninsn; i++)
    {
        ध्रुव TExpr::Insn& ins = prog[i];
        पूर्णाङ्क as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        प्रति (पूर्णाङ्क k = 0; k < 3; k++)
            यदि (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    प्रति (पूर्णाङ्क t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<पूर्णाङ्क, 16> freeBufs(ntemps);
    पूर्णाङ्क nfree = 0, nbuf = 0;
    प्रति (पूर्णाङ्क i = 0; i < ninsn; i++)
    {
        ध्रुव TExpr::Insn& ins = prog[i];
        पूर्णाङ्क rt = tempOfSlot[ins.result];
        यदि (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        पूर्णाङ्क as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        प्रति (पूर्णाङ्क k = 0; k < 3; k++)
        {
            पूर्णाङ्क t = tempOfSlot[as[k]];
            यदि (t >= 0 && lastUse[t] == i)
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
    प्रति (पूर्णाङ्क b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    प्रति (पूर्णाङ्क s = 1; s < nslots; s++)
        यदि (arginfo[s].kind == TEMP)
        {
            पूर्णाङ्क b = bufferOfTemp[arginfo[s].index], e = (पूर्णाङ्क)CV_ELEM_SIZE1(arginfo[s].depth);
            यदि (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    प्रति (पूर्णाङ्क b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    ध्रुव पूर्णाङ्क totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
स्थिर शून्य matLogical(ध्रुव Mat& m, MatShape& shp, EwSteps& step, पूर्णाङ्क& esz1)
{
    esz1 = (पूर्णाङ्क)m.elemSize1();
    पूर्णाङ्क nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    प्रति (पूर्णाङ्क i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
स्थिर तार्किक broadcastShape(ध्रुव MatShape* shps, पूर्णाङ्क K, MatShape& out)
{
    पूर्णाङ्क nd = 0;
    प्रति (पूर्णाङ्क k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    प्रति (पूर्णाङ्क k = 0; k < K; k++)
    {
        ध्रुव MatShape& s = shps[k];
        पूर्णाङ्क off = nd - s.dims;
        प्रति (पूर्णाङ्क i = 0; i < s.dims; i++)
        {
            पूर्णाङ्क d = s[i], &o = out[off + i];
            यदि (o == 1) o = d;
            अन्यथा यदि (d != 1 && d != o) प्रत्यागम असत्य;
        }
    }
    प्रत्यागम सत्य;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
स्थिर पूर्णाङ्क opCost(शीर्ष op)
{
    विकल्प (op)
    {
    पक्ष OP_ADD: पक्ष OP_SUB: पक्ष OP_MUL: पक्ष OP_MIN: पक्ष OP_MAX:
    पक्ष OP_ABSDIFF: पक्ष OP_AND: पक्ष OP_OR: पक्ष OP_XOR: पक्ष OP_NOT:
    पक्ष OP_NEG: पक्ष OP_ABS: पक्ष OP_CAST: पक्ष OP_RELU: पक्ष OP_SELECT:
    पक्ष OP_CMP_EQ: पक्ष OP_CMP_NE: पक्ष OP_CMP_LT:
    पक्ष OP_CMP_LE: पक्ष OP_CMP_GT: पक्ष OP_CMP_GE: प्रत्यागम 1;
    पक्ष OP_DIV: पक्ष OP_SQRT: पक्ष OP_HYPOT: पक्ष OP_CONVERT_SCALE: प्रत्यागम 3;
    पक्ष OP_SIN: पक्ष OP_COS: पक्ष OP_TANH: पक्ष OP_ERF: पक्ष OP_ATAN2:
    पक्ष OP_EXP: पक्ष OP_LOG: पक्ष OP_POW:           प्रत्यागम 6;
    स्वभाव:                                         प्रत्यागम 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
शून्य TExpr::clear()
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
शून्य TExpr::dump(std::ostream& os) ध्रुव
{
    auto dn = [](पूर्णाङ्क d) -> ध्रुव वर्ण* {
        विकल्प (d) {
        पक्ष EW_DEPTH_NONE: प्रत्यागम "flex";
        पक्ष CV_8U:  प्रत्यागम "u8";  पक्ष CV_8S:  प्रत्यागम "s8";
        पक्ष CV_16U: प्रत्यागम "u16"; पक्ष CV_16S: प्रत्यागम "s16";
        पक्ष CV_32U: प्रत्यागम "u32"; पक्ष CV_32S: प्रत्यागम "s32";
        पक्ष CV_64U: प्रत्यागम "u64"; पक्ष CV_64S: प्रत्यागम "s64";
        पक्ष CV_16F: प्रत्यागम "f16"; पक्ष CV_16BF:प्रत्यागम "bf16";
        पक्ष CV_32F: प्रत्यागम "f32"; पक्ष CV_64F: प्रत्यागम "f64";
        पक्ष CV_Bool:प्रत्यागम "bool";
        स्वभाव:     प्रत्यागम "?"; }
    };
    auto kn = [](ArgKind k) -> ध्रुव वर्ण* {
        विकल्प (k) { पक्ष NONE: प्रत्यागम "none"; पक्ष आदान: प्रत्यागम "in"; पक्ष CONST: प्रत्यागम "const";
                     पक्ष TEMP: प्रत्यागम "temp"; पक्ष OUTPUT: प्रत्यागम "out"; } प्रत्यागम "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (पूर्णाङ्क)prog.size() << "\n";
    os << "  slots:\n";
    प्रति (पूर्णाङ्क s = 0; s < (पूर्णाङ्क)arginfo.size(); s++) {
        ध्रुव Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        यदि (a.kind != NONE) os << " " << dn(a.depth);
        यदि (a.kind == आदान || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        यदि (a.kind == CONST) {
            ध्रुव पूर्णाङ्क cn = std::max(1, a.channels);
            AutoBuffer<द्विगुण, MAX_LOCAL_CN> v(cn);
            ध्रुव uchar* src = (ध्रुव uchar*)(constbuf.data() + a.constofs);
            यदि (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * परिमाण(द्विगुण));
            अन्यथा getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            प्रति (पूर्णाङ्क c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)prog.size(); i++) {
        ध्रुव Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        यदि (ins.arg1) os << ", " << ins.arg1;
        यदि (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        यदि (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        यदि (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        यदि (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
पूर्णाङ्क TExpr::add_आदान(पूर्णाङ्क depth)
{
    Arg a; a.kind = आदान; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); प्रत्यागम (पूर्णाङ्क)arginfo.size() - 1;
}

पूर्णाङ्क TExpr::addOutput(पूर्णाङ्क depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); प्रत्यागम (पूर्णाङ्क)arginfo.size() - 1;
}

पूर्णाङ्क TExpr::addTemp(पूर्णाङ्क depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); प्रत्यागम (पूर्णाङ्क)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
पूर्णाङ्क TExpr::addConst(पूर्णाङ्क depth, ध्रुव Scalar& v, पूर्णाङ्क channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); प्रत्यागम (पूर्णाङ्क)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
पूर्णाङ्क TExpr::addConst(पूर्णाङ्क depth, पूर्णाङ्क srcdepth, ध्रुव शून्य* data, पूर्णाङ्क channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); प्रत्यागम (पूर्णाङ्क)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
पूर्णाङ्क TExpr::typedConstFrom(पूर्णाङ्क srcSlot, पूर्णाङ्क depth)
{
    ध्रुव Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); प्रत्यागम (पूर्णाङ्क)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
पूर्णाङ्क TExpr::addInsn(शीर्ष op, पूर्णाङ्क a0, पूर्णाङ्क a1, पूर्णाङ्क a2, पूर्णाङ्क result, ध्रुव TKernel& kernel, ध्रुव Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    प्रत्यागम (पूर्णाङ्क)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
पूर्णाङ्क TExpr::addInsn(शीर्ष op, पूर्णाङ्क a0, पूर्णाङ्क a1, पूर्णाङ्क a2, पूर्णाङ्क result, ध्रुव Scalar& params)
{
    पूर्णाङ्क d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    पूर्णाङ्क d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    पूर्णाङ्क d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    प्रत्यागम addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
पूर्णाङ्क TExpr::maybeAddCast(पूर्णाङ्क arg, पूर्णाङ्क depth)
{
    यदि (arginfo[arg].depth == depth) प्रत्यागम arg;
    पूर्णाङ्क t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    प्रत्यागम t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
स्थिर अन्तःस्थ शून्य runInsn(ध्रुव TExpr::Insn& ins,
                           ध्रुव शून्य* p0, size_t y0, size_t x0,
                           ध्रुव शून्य* p1, size_t y1, size_t x1,
                           ध्रुव शून्य* p2, size_t y2, size_t x2,
                           शून्य* pr, size_t yr, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

संरचना EwBody {
    ध्रुव TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    ध्रुव पूर्णाङ्क* slotMap;
    ध्रुव signed वर्ण* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    ध्रुव पूर्णाङ्क* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    पूर्णाङ्क ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

शून्य TExpr::outputShape(ध्रुव Mat* ध्रुव* inputs, MatShape& spatial, पूर्णाङ्क& channels) ध्रुव
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    ध्रुव पूर्णाङ्क rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    तार्किक sameShape = ninputs >= 1;
    प्रति (पूर्णाङ्क i = 1; sameShape && i < ninputs; i++)
    {
        ध्रुव Mat& a = *inputs[i];
        यदि (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = असत्य;
        अन्यथा प्रति (पूर्णाङ्क d = 0; d < a.dims; d++) यदि (a.size[d] != inputs[0]->size[d]) { sameShape = असत्य; भङ्ग; }
    }
    यदि (sameShape) { spatial = inputs[0]->size; channels = rcn0; प्रत्यागम; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; पूर्णाङ्क esz1;
    प्रति (पूर्णाङ्क i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    ध्रुव पूर्णाङ्क ndFull = (पूर्णाङ्क)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
शून्य TExpr::outputShape(ध्रुव Mat* inputs, MatShape& spatial, पूर्णाङ्क& channels) ध्रुव
{
    AutoBuffer<ध्रुव Mat*, 16> ptrs(std::max(ninputs, 1));
    प्रति (पूर्णाङ्क i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
स्थिर constexpr पूर्णाङ्क LOCAL_HDRS = 3;
स्थिर constexpr पूर्णाङ्क LOCAL_CONSTS = 4;
स्थिर constexpr पूर्णाङ्क LOCAL_OPS = 16;

शून्य TExpr::exec(ध्रुव Mat* ध्रुव* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    ध्रुव पूर्णाङ्क nslots = (पूर्णाङ्क)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    यदि (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        ध्रुव Mat& r = *inputs[0];
        ध्रुव पूर्णाङ्क rcn = r.channels();
        तार्किक ok = r.isContinuous();
        प्रति (पूर्णाङ्क i = 1; ok && i < ninputs; i++)
        {
            ध्रुव Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        प्रति (पूर्णाङ्क s = 1; ok && s < nslots; s++)
            यदि (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        ध्रुव size_t total = ok ? r.total() * rcn : 0;
        यदि (ok && total == 0)
        {
            ध्रुव MatShape rshape = r.size;
            प्रति (पूर्णाङ्क s = 1; s < nslots; s++)
                यदि (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            प्रत्यागम;
        }
        यदि (ok && total <= EW_FASTPATH_MAX)
        {
            ध्रुव MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            प्रति (पूर्णाङ्क s = 1; s < nslots; s++)
                यदि (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            ध्रुव पूर्णाङ्क ninsn = (पूर्णाङ्क)prog.size();

            यदि (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](पूर्णाङ्क s) -> शून्य* {
                    यदि (s <= 0) प्रत्यागम nullptr;
                    ध्रुव Arg& ai = arginfo[s];
                    यदि (ai.kind == आदान)  प्रत्यागम (शून्य*)inputs[ai.index]->data;
                    यदि (ai.kind == OUTPUT) प्रत्यागम (शून्य*)outputs[ai.index].data;
                    प्रत्यागम nullptr;   // NONE
                };
                ध्रुव पूर्णाङ्क w = (पूर्णाङ्क)total;
                प्रति (पूर्णाङ्क n = 0; n < ninsn; n++)
                {
                    ध्रुव Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                प्रत्यागम;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            ध्रुव पूर्णाङ्क totalEsz = bufEszPrefix[nbuffers];
            ध्रुव पूर्णाङ्क wf0 = std::min((पूर्णाङ्क)total, capElems);

            ध्रुव size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/परिमाण(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)परिमाण(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            प्रति (पूर्णाङ्क x0 = 0; x0 < (पूर्णाङ्क)total; x0 += wf0)
            {
                ध्रुव पूर्णाङ्क wf = std::min(wf0, (पूर्णाङ्क)total - x0);
                auto ptrOf = [&](पूर्णाङ्क s) -> शून्य* {
                    यदि (s <= 0) प्रत्यागम nullptr;
                    ध्रुव Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    यदि (ai.kind == आदान)  प्रत्यागम (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    यदि (ai.kind == OUTPUT) प्रत्यागम (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    यदि (ai.kind != TEMP)   प्रत्यागम nullptr;                                    // NONE: moved-from
                    ध्रुव पूर्णाङ्क b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    प्रत्यागम scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                प्रति (पूर्णाङ्क n = 0; n < ninsn; n++)
                {
                    ध्रुव Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            प्रत्यागम;
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
    पूर्णाङ्क rchannels;
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
    तार्किक inplace = असत्य;
    प्रति (पूर्णाङ्क i = 0; i < ninputs && !inplace; i++)
        प्रति (पूर्णाङ्क j = 0; j < noutputs; j++)
            यदि (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = सत्य; भङ्ग; }
    पूर्णाङ्क nhdrs = inplace ? ninputs : 0;

    AutoBuffer<ध्रुव Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<ध्रुव Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<पूर्णाङ्क, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed वर्ण, LOCAL_OPS> slotKind(nslots);
    पूर्णाङ्क narr = 0, nc = 0;
    यदि (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        प्रति (पूर्णाङ्क j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed वर्ण)TExpr::NONE;
    प्रति (पूर्णाङ्क s = 1; s < nslots; s++)
    {
        ध्रुव TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed वर्ण)ai.kind;
        यदि (ai.kind == TExpr::आदान) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        अन्यथा यदि (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        अन्यथा यदि (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed वर्ण)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        अन्यथा यदि (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            ध्रुव पूर्णाङ्क c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (शून्य*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed वर्ण)TExpr::आदान;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        अन्यथा यदि (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
        }
        अन्यथा {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    ध्रुव पूर्णाङ्क ninsn = (पूर्णाङ्क)prog.size();
    long long costPerElem = 0;
    प्रति (पूर्णाङ्क n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    प्रति (पूर्णाङ्क d = 0; d < (पूर्णाङ्क)spatial.size(); d++) otot *= spatial[d];
    यदि (otot == 0)
        प्रत्यागम;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    ध्रुव द्विगुण T = (द्विगुण)std::max(get_अंक_Threads(), 1);
    ध्रुव द्विगुण nstripes = std::min(
        (द्विगुण)otot * (द्विगुण)std::max<long long>(costPerElem, 1) * (1./ (द्विगुण)(1 << 16)),
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

    broadcastOp(arr.data(), narr, [&](ध्रुव BrTile& tile)
    {
        EwBody& bc = body;
        ध्रुव TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        ध्रुव पूर्णाङ्क* ध्रुव smap  = bc.slotMap;
        ध्रुव signed वर्ण* ध्रुव skind = bc.slotKind;
        ध्रुव पूर्णाङ्क nin = bc.ninsn, nsl = bc.nslots;
        ध्रुव पूर्णाङ्क w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        ध्रुव पूर्णाङ्क* ध्रुव eszPrefix = bc.bufEszPrefix;
        ध्रुव पूर्णाङ्क bw = std::min(w, bc.capElems);
        ध्रुव पूर्णाङ्क bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        ध्रुव size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/परिमाण(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)परिमाण(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        प्रति (पूर्णाङ्क y0 = 0; y0 < h; y0 += bh)
        {
            ध्रुव पूर्णाङ्क hf = std::min(bh, h - y0);
            प्रति (पूर्णाङ्क x0 = 0; x0 < w; x0 += bw)
            {
                ध्रुव पूर्णाङ्क wf = std::min(bw, w - x0);
                प्रति (पूर्णाङ्क s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    ध्रुव पूर्णाङ्क k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    यदि (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    अन्यथा                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        ध्रुव BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                प्रति (पूर्णाङ्क n = 0; n < nin; n++)
                {
                    ध्रुव TExpr::Insn& ins = insns[n];
                    ध्रुव BrSlice& a0 = args[ins.arg0]; ध्रुव BrSlice& a1 = args[ins.arg1];
                    ध्रुव BrSlice& a2 = args[ins.arg2]; ध्रुव BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (शून्य*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, सत्य, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
शून्य TExpr::exec(ध्रुव Mat* inputs, Mat* outputs)
{
    AutoBuffer<ध्रुव Mat*, 16> ptrs(std::max(ninputs, 1));
    प्रति (पूर्णाङ्क i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
शून्य make_द्विमान_Arith_Program(TExpr& p, शीर्ष op, पूर्णाङ्क depth0, पूर्णाङ्क depth1, पूर्णाङ्क rdepth,
                            पूर्णाङ्क maskDepth, द्विगुण scale)
{
    p.clear();
    यदि (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        यदि (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        अन्यथा
        {
            rdepth = promoteArith(depth0, depth1);
            यदि (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    ध्रुव तार्किक masked = maskDepth != EW_DEPTH_NONE;
    पूर्णाङ्क sIn0  = p.add_आदान(depth0);
    पूर्णाङ्क sIn1  = p.add_आदान(depth1);
    पूर्णाङ्क sMask = masked ? p.add_आदान(maskDepth) : 0;
    पूर्णाङ्क sOut  = p.addOutput(rdepth);

    पूर्णाङ्क r = p.emit_द्विमान(op, sIn0, sIn1, rdepth, Scalar(scale));
    यदि (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    अन्यथा
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
शून्य makeAddWeightedProgram(TExpr& p, पूर्णाङ्क depth0, पूर्णाङ्क depth1, पूर्णाङ्क rdepth,
                            द्विगुण alpha, द्विगुण beta, द्विगुण gamma)
{
    p.clear();
    पूर्णाङ्क sA   = p.add_आदान(depth0);
    पूर्णाङ्क sB   = p.add_आदान(depth1);
    पूर्णाङ्क sOut = p.addOutput(rdepth);
    पूर्णाङ्क r = p.emit_द्विमान(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

नामाकाश {

// --- tokens ------------------------------------------------------------------------------
गणना TokType { T_अंक, T_आदान, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

संरचना Token
{
    TokType type = T_END;
    द्विगुण num = 0;
    पूर्णाङ्क आदान_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
संरचना Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    स्थिर तार्किक isIdentStart(वर्ण c) { प्रत्यागम std::isalpha((unsigned वर्ण)c) || c == '_'; }
    स्थिर तार्किक isIdentChar(वर्ण c)  { प्रत्यागम std::isalnum((unsigned वर्ण)c) || c == '_'; }

    Token next()
    {
        यावत् (pos < s.size() && std::isspace((unsigned वर्ण)s[pos])) pos++;
        Token t;
        यदि (pos >= s.size()) { t.type = T_END; प्रत्यागम t; }

        वर्ण c = s[pos];

        यदि (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            यावत् (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_आदान;
            t.आदान_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            प्रत्यागम t;
        }
        यदि (std::isdigit((unsigned वर्ण)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned वर्ण)s[pos + 1])))
        {
            वर्ण* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_अंक;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            प्रत्यागम t;
        }
        यदि (isIdentStart(c))
        {
            size_t start = pos;
            यावत् (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            प्रत्यागम t;
        }
        विकल्प (c)
        {
        पक्ष '(': pos++; t.type = T_LPAREN; प्रत्यागम t;
        पक्ष ')': pos++; t.type = T_RPAREN; प्रत्यागम t;
        पक्ष ',': pos++; t.type = T_COMMA;  प्रत्यागम t;
        पक्ष ';': pos++; t.type = T_SEMI;   प्रत्यागम t;
        }
        // multi/!single-char operators
        auto two = [&](ध्रुव वर्ण* op) {
            प्रत्यागम pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        यदि (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; प्रत्यागम t; }
        यदि (c == '=') { pos++; t.type = T_ASSIGN; प्रत्यागम t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        प्रत्यागम t;
    }
};

// --- operator / function tables ----------------------------------------------------------
स्थिर पूर्णाङ्क binPrec(ध्रुव std::string& op)
{
    यदि (op == "**") प्रत्यागम 8;              // power binds tighter than '*'; RIGHT-associative
    यदि (op == "*" || op == "/") प्रत्यागम 7;
    यदि (op == "+" || op == "-") प्रत्यागम 6;
    यदि (op == "<" || op == "<=" || op == ">" || op == ">=") प्रत्यागम 5;
    यदि (op == "==" || op == "!=") प्रत्यागम 4;
    यदि (op == "&") प्रत्यागम 3;
    यदि (op == "^") प्रत्यागम 2;
    यदि (op == "|") प्रत्यागम 1;
    प्रत्यागम -1;
}

स्थिर शीर्ष binOp(ध्रुव std::string& op)
{
    यदि (op == "**") प्रत्यागम OP_POW;
    यदि (op == "+")  प्रत्यागम OP_ADD;
    यदि (op == "-")  प्रत्यागम OP_SUB;
    यदि (op == "*")  प्रत्यागम OP_MUL;
    यदि (op == "/")  प्रत्यागम OP_DIV;
    यदि (op == "<")  प्रत्यागम OP_CMP_LT;
    यदि (op == "<=") प्रत्यागम OP_CMP_LE;
    यदि (op == ">")  प्रत्यागम OP_CMP_GT;
    यदि (op == ">=") प्रत्यागम OP_CMP_GE;
    यदि (op == "==") प्रत्यागम OP_CMP_EQ;
    यदि (op == "!=") प्रत्यागम OP_CMP_NE;
    यदि (op == "&")  प्रत्यागम OP_AND;
    यदि (op == "|")  प्रत्यागम OP_OR;
    यदि (op == "^")  प्रत्यागम OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
स्थिर पूर्णाङ्क typeDepth(ध्रुव std::string& name)
{
    यदि (name == "float")    प्रत्यागम CV_32F;
    यदि (name == "double")   प्रत्यागम CV_64F;
    यदि (name == "half" || name == "float16") प्रत्यागम CV_16F;
    यदि (name == "bfloat16") प्रत्यागम CV_16BF;
    यदि (name == "uint8")    प्रत्यागम CV_8U;
    यदि (name == "int8")     प्रत्यागम CV_8S;
    यदि (name == "uint16")   प्रत्यागम CV_16U;
    यदि (name == "int16")    प्रत्यागम CV_16S;
    यदि (name == "uint32")   प्रत्यागम CV_32U;
    यदि (name == "int32")    प्रत्यागम CV_32S;
    यदि (name == "uint64")   प्रत्यागम CV_64U;
    यदि (name == "int64")    प्रत्यागम CV_64S;
    प्रत्यागम -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
स्थिर शीर्ष fnOp(ध्रुव std::string& name, पूर्णाङ्क& arity)
{
    संरचना E { ध्रुव वर्ण* n; शीर्ष op; };
    स्थिर ध्रुव E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    स्थिर ध्रुव E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    स्थिर ध्रुव E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    प्रति (ध्रुव E& e : unary)  यदि (name == e.n) { arity = 1; प्रत्यागम e.op; }
    प्रति (ध्रुव E& e : binary) यदि (name == e.n) { arity = 2; प्रत्यागम e.op; }
    प्रति (ध्रुव E& e : tern)   यदि (name == e.n) { arity = 3; प्रत्यागम e.op; }
    arity = 0; प्रत्यागम OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
संरचना Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    ध्रुव पूर्णाङ्क* आदान_Slot;                 // slot id of each external input (precreated)
    पूर्णाङ्क ninputs;
    std::map<std::string, पूर्णाङ्क> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& अभिव्यक्ती, ध्रुव पूर्णाङ्क* islot, पूर्णाङ्क nin)
        : lex(src), e(अभिव्यक्ती), आदान_Slot(islot), ninputs(nin) { cur = lex.next(); }

    शून्य advance() { cur = lex.next(); }
    तार्किक isOp(ध्रुव वर्ण* op) ध्रुव { प्रत्यागम cur.type == T_OP && cur.text == op; }

    शून्य expect(TokType t, ध्रुव वर्ण* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    तार्किक isFlexConst(पूर्णाङ्क slot) ध्रुव
    {
        प्रत्यागम e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    पूर्णाङ्क parsePrimary()
    {
        यदि (cur.type == T_अंक)   { पूर्णाङ्क s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); प्रत्यागम s; }
        यदि (cur.type == T_आदान) { पूर्णाङ्क idx = cur.आदान_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   प्रत्यागम आदान_Slot[idx]; }
        यदि (cur.type == T_LPAREN){ advance(); पूर्णाङ्क x = parseTernary(); expect(T_RPAREN, "expected ')'"); प्रत्यागम x; }
        यदि (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            यदि (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                प्रत्यागम it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<पूर्णाङ्क, 8> args = {}; पूर्णाङ्क nargs = 0;
            यदि (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                यावत् (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (पूर्णाङ्क)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            पूर्णाङ्क td = typeDepth(name);
            यदि (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                पूर्णाङ्क s = args[0];
                प्रत्यागम isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            पूर्णाङ्क arity = 0; शीर्ष op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            यदि (arity == 1) प्रत्यागम e.emitUnary(op, args[0]);
            यदि (arity == 2) प्रत्यागम e.emit_द्विमान(op, args[0], args[1]);
            प्रत्यागम e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    पूर्णाङ्क parseUnary()
    {
        यदि (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            पूर्णाङ्क operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            यदि (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<द्विगुण, MAX_LOCAL_CN> v; पूर्णाङ्क cn = constDoubles(e, operand, v);
                Scalar neg; प्रति (पूर्णाङ्क i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                प्रत्यागम e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            प्रत्यागम e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        प्रत्यागम parsePrimary();
    }

    पूर्णाङ्क parse_अभिव्यक्ती(पूर्णाङ्क minPrec)
    {
        पूर्णाङ्क डावा = parseUnary();
        यावत् (cur.type == T_OP)
        {
            पूर्णाङ्क p = binPrec(cur.text);
            यदि (p < minPrec) भङ्ग;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            पूर्णाङ्क उजवा = parse_अभिव्यक्ती(op == "**" ? p : p + 1);
            डावा = e.emit_द्विमान(binOp(op), डावा, उजवा);
        }
        प्रत्यागम डावा;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    पूर्णाङ्क parseTernary()
    {
        पूर्णाङ्क cond = parse_अभिव्यक्ती(0);
        यदि (!isOp("?")) प्रत्यागम cond;
        advance();
        पूर्णाङ्क thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        पूर्णाङ्क elsev = parseTernary();               // right-associative chain
        प्रत्यागम e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    शून्य parseResult()
    {
        यदि (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); पूर्णाङ्क nTemp = e.ntemps;
            advance();
            पूर्णाङ्क e0 = parseTernary();
            यदि (cur.type == T_COMMA)
            {
                e.output(e0);
                यावत् (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                प्रत्यागम;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    शून्य parse()
    {
        यावत् (सत्य)
        {
            यदि (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                यदि (cur.type == T_ASSIGN)
                {
                    advance();
                    ध्रुव पूर्णाङ्क slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = सत्य;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    अनुवर्त;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            यदि (cur.type == T_SEMI) advance();
            भङ्ग;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

नामाकाश cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
शून्य texpr(ध्रुव std::string& अभिव्यक्ती, आदान_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using नामाकाश cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == आदान_Array_2::STD_VECTOR_MAT);
    ध्रुव std::vector<Mat>& inps = *(ध्रुव std::vector<Mat>*)_inputs.getObj();
    ध्रुव पूर्णाङ्क ninputs = (पूर्णाङ्क)inps.size();
    प्रति (पूर्णाङ्क i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<पूर्णाङ्क> islot(std::max(ninputs, 1));
    प्रति (पूर्णाङ्क i = 0; i < ninputs; i++) islot[i] = e.add_आदान(inps[i].depth());
    Parser(अभिव्यक्ती, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    यदि (kind == आदान_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } अन्यथा {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
