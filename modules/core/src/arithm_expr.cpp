// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "arithm_expr.hpp"
#અન્તર્ભાવ <algorithm>
#અન્તર્ભાવ <climits>
#અન્તર્ભાવ <cmath>
#અન્તર્ભાવ <cstdlib>
#અન્તર્ભાવ <cstring>
#અન્તર્ભાવ <map>
#અન્તર્ભાવ <ostream>
#અન્તર્ભાવ <string>

નામાકાશ cv { નામાકાશ ew {

// ============================ op metadata (was ew_op.cpp) ============================

અચલ અક્ષર* opName(ટોચ op)
{
    પસંદગી (op)
    {
    સ્થિતિ OP_NOP:           પાછા "nop";
    સ્થિતિ OP_NEG:           પાછા "neg";
    સ્થિતિ OP_ABS:           પાછા "abs";
    સ્થિતિ OP_NOT:           પાછા "not";
    સ્થિતિ OP_SQRT:          પાછા "sqrt";
    સ્થિતિ OP_EXP:           પાછા "exp";
    સ્થિતિ OP_LOG:           પાછા "log";
    સ્થિતિ OP_SIN:           પાછા "sin";
    સ્થિતિ OP_COS:           પાછા "cos";
    સ્થિતિ OP_TANH:          પાછા "tanh";
    સ્થિતિ OP_ERF:           પાછા "erf";
    સ્થિતિ OP_RELU:          પાછા "relu";
    સ્થિતિ OP_CAST:          પાછા "cast";
    સ્થિતિ OP_ADD:           પાછા "add";
    સ્થિતિ OP_SUB:           પાછા "sub";
    સ્થિતિ OP_MUL:           પાછા "mul";
    સ્થિતિ OP_DIV:           પાછા "div";
    સ્થિતિ OP_POW:           પાછા "pow";
    સ્થિતિ OP_MIN:           પાછા "min";
    સ્થિતિ OP_MAX:           પાછા "max";
    સ્થિતિ OP_ABSDIFF:       પાછા "absdiff";
    સ્થિતિ OP_ADDW:          પાછા "addWeighted";
    સ્થિતિ OP_HYPOT:         પાછા "hypot";
    સ્થિતિ OP_ATAN2:         પાછા "atan2";
    સ્થિતિ OP_AND:           પાછા "and";
    સ્થિતિ OP_OR:            પાછા "or";
    સ્થિતિ OP_XOR:           પાછા "xor";
    સ્થિતિ OP_CMP_EQ:        પાછા "cmp_eq";
    સ્થિતિ OP_CMP_NE:        પાછા "cmp_ne";
    સ્થિતિ OP_CMP_LT:        પાછા "cmp_lt";
    સ્થિતિ OP_CMP_LE:        પાછા "cmp_le";
    સ્થિતિ OP_CMP_GT:        પાછા "cmp_gt";
    સ્થિતિ OP_CMP_GE:        પાછા "cmp_ge";
    સ્થિતિ OP_CLAMP:         પાછા "clamp";
    સ્થિતિ OP_SELECT:        પાછા "select";
    સ્થિતિ OP_CONVERT_SCALE: પાછા "convert_scale";
    મૂળભૂત:               પાછા "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(ટોચ op)
{
    પસંદગી (op)
    {
    સ્થિતિ OP_AND: સ્થિતિ OP_OR: સ્થિતિ OP_XOR: સ્થિતિ OP_NOT:
        પાછા CAT_BITWISE;
    સ્થિતિ OP_CMP_EQ: સ્થિતિ OP_CMP_NE: સ્થિતિ OP_CMP_LT:
    સ્થિતિ OP_CMP_LE: સ્થિતિ OP_CMP_GT: સ્થિતિ OP_CMP_GE:
        પાછા CAT_COMPARE;
    સ્થિતિ OP_SQRT: સ્થિતિ OP_EXP: સ્થિતિ OP_LOG:
    સ્થિતિ OP_SIN: સ્થિતિ OP_COS: સ્થિતિ OP_TANH: સ્થિતિ OP_ERF: સ્થિતિ OP_ATAN2: સ્થિતિ OP_RELU:
        પાછા CAT_MATH;
    સ્થિતિ OP_CAST: સ્થિતિ OP_CONVERT_SCALE:
        પાછા CAT_CAST;
    સ્થિતિ OP_SELECT:
        પાછા CAT_SELECT;
    મૂળભૂત:
        પાછા CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
સ્થિર તાર્કિક isFloatDepth(પૂર્ણાંક d)
{
    પાછા d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
સ્થિર તાર્કિક depthRepresents(દ્વિગુણ v, પૂર્ણાંક depth)
{
    જો (isFloatDepth(depth)) પાછા સાચું;
    જો (v != std::floor(v)) પાછા ખોટું;
    પસંદગી (depth)
    {
    સ્થિતિ CV_8U:  પાછા v >= 0            && v <= 255;
    સ્થિતિ CV_8S:  પાછા v >= -128         && v <= 127;
    સ્થિતિ CV_16U: પાછા v >= 0            && v <= 65535;
    સ્થિતિ CV_16S: પાછા v >= -32768       && v <= 32767;
    સ્થિતિ CV_32U: પાછા v >= 0            && v <= 4294967295.0;
    સ્થિતિ CV_32S: પાછા v >= -2147483648.0 && v <= 2147483647.0;
    સ્થિતિ CV_64U: પાછા v >= 0            && v <= 18446744073709551615.0;
    સ્થિતિ CV_64S: પાછા v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    મૂળભૂત:     પાછા ખોટું;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
સ્થિર શૂન્ય intRange(પૂર્ણાંક depth, દ્વિગુણ& lo, દ્વિગુણ& hi)
{
    પસંદગી (depth)
    {
    સ્થિતિ CV_8U:  lo = 0;                     hi = 255; તોડો;
    સ્થિતિ CV_8S:  lo = -128;                  hi = 127; તોડો;
    સ્થિતિ CV_16U: lo = 0;                     hi = 65535; તોડો;
    સ્થિતિ CV_16S: lo = -32768;                hi = 32767; તોડો;
    સ્થિતિ CV_32U: lo = 0;                     hi = 4294967295.0; તોડો;
    સ્થિતિ CV_32S: lo = -2147483648.0;         hi = 2147483647.0; તોડો;
    સ્થિતિ CV_64U: lo = 0;                     hi = 18446744073709551615.0; તોડો;
    સ્થિતિ CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; તોડો;
    મૂળભૂત:     lo = 0;                     hi = 0; તોડો;
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
પૂર્ણાંક promoteArith(પૂર્ણાંક a, પૂર્ણાંક b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr unsigned signlut = 0b1001111110100u;
    constexpr unsigned fltlut  = 0b1111000000u;

    જો (a == b) પાછા a;

    પૂર્ણાંક wa = પૂર્ણાંક((leszlut >> (a+1)*3) & 7u), wb = પૂર્ણાંક((leszlut >> (b+1)*3) & 7u);
    પૂર્ણાંક fa = પૂર્ણાંક((fltlut >> (a+1)) & 1u),    fb = પૂર્ણાંક((fltlut >> (b+1)) & 1u);

    જો (fa + fb == 0)                          // both integer
    {
        પૂર્ણાંક sa = પૂર્ણાંક((signlut >> (a+1)) & 1u), sb = પૂર્ણાંક((signlut >> (b+1)) & 1u);
        જો (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            પાછા wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        પૂર્ણાંક rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr પૂર્ણાંક ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        પાછા (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    પૂર્ણાંક maxw = std::max(wa, wb);
    જો (maxw == 2 && fa + fb == 1)
        પાછા fa*a + (1 - fa)*b;
    પાછા CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
પૂર્ણાંક absdiffResultDepth(પૂર્ણાંક depth)
{
    પસંદગી (depth)
    {
    સ્થિતિ CV_8S:  પાછા CV_8U;
    સ્થિતિ CV_16S: પાછા CV_16U;
    સ્થિતિ CV_32S: પાછા CV_32U;
    સ્થિતિ CV_64S: પાછા CV_64U;
    મૂળભૂત:     પાછા depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
સ્થિર પૂર્ણાંક promote2(પૂર્ણાંક a, પૂર્ણાંક b)
{
    જો (a == b) પાછા a;

    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    અચલ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_a = unsigned((typelut >> (a*lbits)) & lmask);
    unsigned pr_b = unsigned((typelut >> (b*lbits)) & lmask);
    unsigned max_pr = std::max(pr_a, pr_b);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    અચલ unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    પાછા પૂર્ણાંક((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
સ્થિર પૂર્ણાંક promoteBitwise(પૂર્ણાંક a, પૂર્ણાંક b)
{
    જો (a == EW_DEPTH_NONE) પાછા b;
    જો (b == EW_DEPTH_NONE) પાછા a;
    જો (a == b) પાછા a;
    પાછા CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
સ્થિર પૂર્ણાંક safeWide(પૂર્ણાંક depth)
{
    constexpr unsigned lbits = 3, lmask = (1u << lbits) - 1u;
    અચલ uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    unsigned pr_depth = unsigned((typelut >> (depth*lbits)) & lmask);
    constexpr unsigned dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    અચલ unsigned ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    પાછા પૂર્ણાંક((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
ગણના { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
સ્થિર size_t appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, પૂર્ણાંક srcdepth, અચલ શૂન્ય* data, પૂર્ણાંક channels)
{
    અચલ પૂર્ણાંક cn = std::max(1, channels);
    અચલ size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    અચલ size_t nu64 = (nbytes + કદ(uint64_t) - 1) / કદ(uint64_t);
    અચલ size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    જો (data) memcpy(dst, data, nbytes); નહીંતર memset(dst, 0, nbytes);
    પાછા ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
સ્થિર પૂર્ણાંક constDoubles(અચલ TExpr& e, પૂર્ણાંક s, AutoBuffer<દ્વિગુણ, MAX_LOCAL_CN>& buf)
{
    અચલ TExpr::Arg& a = e.arginfo[s];
    અચલ પૂર્ણાંક cn = std::max(1, a.channels);
    buf.resize(cn);
    અચલ uchar* src = (અચલ uchar*)(e.constbuf.data() + a.constofs);
    જો (a.srcdepth == CV_64F)
        memcpy(buf.data(), src, (size_t)cn * કદ(દ્વિગુણ));   // common (Scalar / parsed literal)
    નહીંતર
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    પાછા cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
સ્થિર અંતઃરેખ તાર્કિક isFlexConst(અચલ TExpr& e, પૂર્ણાંક s)
{
    પાછા e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
સ્થિર તાર્કિક scalar_Const_કિંમત(અચલ TExpr& e, પૂર્ણાંક s, દ્વિગુણ& v)
{
    અચલ TExpr::Arg& a = e.arginfo[s];
    જો (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) પાછા ખોટું;
    AutoBuffer<દ્વિગુણ, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    પાછા સાચું;
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
સ્થિર તાર્કિક matchScaledChain(અચલ TExpr& e, પૂર્ણાંક end, પૂર્ણાંક t,
                             પૂર્ણાંક& arrSlot, દ્વિગુણ& scale, પૂર્ણાંક& startIdx)
{
    જો (end < 0) પાછા ખોટું;
    પૂર્ણાંક j = end, mulResult = t;

    // optional trailing narrowing cast
    જો (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        જો (--j < 0) પાછા ખોટું;
    }

    અચલ TExpr::Insn& mul = e.prog[j];
    જો (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) પાછા ખોટું;

    પૂર્ણાંક arr;
    જો      (scalar_Const_કિંમત(e, mul.arg1, scale)) arr = mul.arg0;
    નહીંતર જો (scalar_Const_કિંમત(e, mul.arg0, scale)) arr = mul.arg1;
    નહીંતર પાછા ખોટું;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    જો (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    જો (e.arginfo[arr].kind == TExpr::CONST) પાછા ખોટું;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    પાછા સાચું;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
સ્થિર તાર્કિક constFits(અચલ TExpr& e, પૂર્ણાંક s, પૂર્ણાંક d)
{
    જો (!isFlexConst(e, s)) પાછા સાચું;
    AutoBuffer<દ્વિગુણ, MAX_LOCAL_CN> v;
    પૂર્ણાંક cn = constDoubles(e, s, v);
    માટે (પૂર્ણાંક ch = 0; ch < cn; ch++)
        જો (!depthRepresents(v[ch], d)) પાછા ખોટું;
    પાછા સાચું;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
પૂર્ણાંક TExpr::emit_દ્વિઅંકી(ટોચ op, પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક rdepth, અચલ Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    જો (op == OP_ADDW)
    {
        પૂર્ણાંક Tt = arginfo[a].depth;
        જો (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        જો (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        જો (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        પૂર્ણાંક outD = rdepth;
        જો (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        અચલ પૂર્ણાંક out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        જો (outD == rdepth) પાછા out;
        અચલ પૂર્ણાંક out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        પાછા out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    જો (op == OP_ADD)
    {
        અચલ પૂર્ણાંક n = (પૂર્ણાંક)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        પૂર્ણાંક x = 0, y = 0, startA = 0, startB = 0;
        દ્વિગુણ alpha = 0, beta = 0;
        જો (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*આ, n - 1, b, y, beta, startB) &&
            matchScaledChain(*આ, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            પૂર્ણાંક retired = 0, lowest = INT_MAX;
            માટે (પૂર્ણાંક i = startA; i < n; i++)
            {
                અચલ પૂર્ણાંક r = prog[i].result;
                જો (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; તોડો; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            જો (retired > 0 && lowest == ntemps - retired)
            {
                માટે (પૂર્ણાંક i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                પાછા emit_દ્વિઅંકી(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            પૂર્ણાંક acc = 0; દ્વિગુણ gamma = 0;
            જો      (scalar_Const_કિંમત(*આ, b, gamma)) acc = a;
            નહીંતર જો (scalar_Const_કિંમત(*આ, a, gamma)) acc = b;
            જો (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                પાછા acc;
            }
        }
    }

    અચલ પૂર્ણાંક nd0 = isFlexConst(*આ, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    અચલ પૂર્ણાંક nd1 = isFlexConst(*આ, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    અચલ ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    પૂર્ણાંક result = rdepth;
    જો (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        જો (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    પૂર્ણાંક base;
    જો (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    નહીંતર જો (nd0 == EW_DEPTH_NONE) base = nd1;
    નહીંતર જો (nd1 == EW_DEPTH_NONE) base = nd0;
    નહીંતર base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    જો (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*આ, b) && !constFits(*આ, b, base))
    {
        દ્વિગુણ lo, hi; intRange(nd0, lo, hi);
        જો (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            ગણના { REAL, CFALSE, CTRUE };
            AutoBuffer<દ્વિગુણ, MAX_LOCAL_CN> tv; પૂર્ણાંક cn = constDoubles(*આ, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            પૂર્ણાંક kind[4] = {}; દ્વિગુણ bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            અચલ ટોચ fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            માટે (પૂર્ણાંક c = 0; c < cn; c++)
            {
                અચલ દ્વિગુણ t = tv[c]; પૂર્ણાંક k; દ્વિગુણ B = t;
                પસંદગી (op)
                {
                સ્થિતિ OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; તોડો;
                સ્થિતિ OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; તોડો;
                સ્થિતિ OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; તોડો;
                સ્થિતિ OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; તોડો;
                સ્થિતિ OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; તોડો;   // a==non-rep -> false
                મૂળભૂત:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  તોડો;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            auto ucOp  = [&](પૂર્ણાંક c){ પાછા kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            auto ucThr = [&](પૂર્ણાંક c){ પાછા kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            અચલ ટોચ u0 = ucOp(0);
            તાર્કિક uniform = સાચું;
            માટે (પૂર્ણાંક c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            જો (uniform)
            {
                Scalar s; માટે (પૂર્ણાંક c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            નહીંતર
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; પૂર્ણાંક patchFlags = 0;
                માટે (પૂર્ણાંક c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    અચલ પૂર્ણાંક mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    અચલ પૂર્ણાંક vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                અચલ પૂર્ણાંક out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                પાછા out;
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
    પૂર્ણાંક depth, wide;
    તાર્કિક divGuard = ખોટું;
    પસંદગી (op)
    {
    સ્થિતિ OP_MIN: સ્થિતિ OP_MAX:
    સ્થિતિ OP_AND: સ્થિતિ OP_OR: સ્થિતિ OP_XOR:
        depth = result; wide = result; તોડો;
    સ્થિતિ OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        પૂર્ણાંક common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        જો (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        જો (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        તોડો;
    }
    સ્થિતિ OP_CMP_EQ: સ્થિતિ OP_CMP_NE: સ્થિતિ OP_CMP_LT:
    સ્થિતિ OP_CMP_LE: સ્થિતિ OP_CMP_GT: સ્થિતિ OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; તોડો;
    સ્થિતિ OP_MUL: સ્થિતિ OP_DIV:
    {
        depth = base;
        અચલ તાર્કિક w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        જો (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            auto intOperand = [&](પૂર્ણાંક s, પૂર્ણાંક nd) {
                જો (nd != EW_DEPTH_NONE) પાછા !isFloatDepth(nd);
                AutoBuffer<દ્વિગુણ, MAX_LOCAL_CN> v; constDoubles(*આ, s, v);
                પાછા v[0] == std::floor(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        તોડો;
    }
    સ્થિતિ OP_POW: સ્થિતિ OP_HYPOT: સ્થિતિ OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; તોડો;
    મૂળભૂત:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); તોડો;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    auto fracConst = [&](પૂર્ણાંક s) {
        જો (!isFlexConst(*આ, s)) પાછા ખોટું;
        AutoBuffer<દ્વિગુણ, MAX_LOCAL_CN> v; પૂર્ણાંક n = constDoubles(*આ, s, v);
        માટે (પૂર્ણાંક ch = 0; ch < n; ch++)
            જો (v[ch] != std::floor(v[ch])) પાછા સાચું;
        પાછા ખોટું;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    જો (cat != CAT_BITWISE && (!constFits(*આ, a, depth) || !constFits(*આ, b, depth)))
    {
        depth = wide;
        અચલ તાર્કિક frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        જો (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        જો (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    પૂર્ણાંક s0 = isFlexConst(*આ, a) ? typedConstFrom(a, depth) : a;
    પૂર્ણાંક s1 = isFlexConst(*આ, b) ? typedConstFrom(b, depth) : b;
    પૂર્ણાંક c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    auto resolve = [&](પૂર્ણાંક rd) {
        પાછા op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    જો (k.fptr)
    {
        પૂર્ણાંક res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        પાછા res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    પૂર્ણાંક w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    પાછા maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
પૂર્ણાંક TExpr::emitUnary(ટોચ op, પૂર્ણાંક a, પૂર્ણાંક rdepth, અચલ Scalar& params)
{
    અચલ પૂર્ણાંક nd = isFlexConst(*આ, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    અચલ ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    જો (op == OP_NEG)
        પાછા emit_દ્વિઅંકી(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    જો (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        જો (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            અચલ પૂર્ણાંક x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            પાછા emit_દ્વિઅંકી(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        પાછા emit_દ્વિઅંકી(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    પૂર્ણાંક result = rdepth;
    જો (result == EW_DEPTH_NONE)
    {
        પસંદગી (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        સ્થિતિ CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; તોડો;
        સ્થિતિ CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; તોડો;  // NOT
        મૂળભૂત:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; તોડો;  // NEG, ABS
        }
    }

    પૂર્ણાંક depth, wide;
    પસંદગી (cat)
    {
    સ્થિતિ CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        તોડો;
    સ્થિતિ CAT_BITWISE: depth = result; wide = result; તોડો;
    મૂળભૂત:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; તોડો;  // NEG/ABS
    }

    જો (!constFits(*આ, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    પૂર્ણાંક s0 = isFlexConst(*આ, a) ? typedConstFrom(a, depth) : a;
    પૂર્ણાંક c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    જો (k.fptr)
    {
        પૂર્ણાંક res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        પાછા res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    પૂર્ણાંક w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    પાછા maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
પૂર્ણાંક TExpr::emitTernary(ટોચ op, પૂર્ણાંક a, પૂર્ણાંક b, પૂર્ણાંક c, પૂર્ણાંક rdepth)
{
    auto dep = [&](પૂર્ણાંક s) { પાછા isFlexConst(*આ, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    જો (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        જો (isFlexConst(*આ, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_દ્વિઅંકી(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        પૂર્ણાંક result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        જો (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        પૂર્ણાંક sb = isFlexConst(*આ, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        પૂર્ણાંક sc = isFlexConst(*આ, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        પૂર્ણાંક res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        પાછા res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    પૂર્ણાંક result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    જો (result == EW_DEPTH_NONE) result = CV_32F;
    auto typed = [&](પૂર્ણાંક s) {
        પાછા isFlexConst(*આ, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    પૂર્ણાંક c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    પૂર્ણાંક res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    પાછા res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
પૂર્ણાંક TExpr::moveToOutput(પૂર્ણાંક temp, પૂર્ણાંક out)
{
    અચલ પૂર્ણાંક ninsn = (પૂર્ણાંક)prog.size();
    પૂર્ણાંક producer = -1;
    તાર્કિક usedAsArg = ખોટું;
    માટે (પૂર્ણાંક i = 0; i < ninsn; i++)
    {
        અચલ TExpr::Insn& ins = prog[i];
        જો (ins.result == temp) producer = i;
        જો (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = સાચું;
    }
    જો (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        પાછા out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    પાછા out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
પૂર્ણાંક TExpr::output(પૂર્ણાંક મૂળ_Slot)
{
    પાછા moveToOutput(મૂળ_Slot, addOutput(arginfo[મૂળ_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
શૂન્ય TExpr::compile()
{
    અચલ પૂર્ણાંક ninsn = (પૂર્ણાંક)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    અચલ પૂર્ણાંક nslots = (પૂર્ણાંક)arginfo.size();
    nconsts = 0;
    માટે (પૂર્ણાંક s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        જો (a.kind != CONST || a.depth == EW_DEPTH_NONE) ચાલુ;
        અચલ પૂર્ણાંક cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        અચલ size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, કદ(uint64_t)> srcbuf(divUp((size_t)cn * sesz, (unsigned)કદ(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        memcpy(srcbytes, (અચલ uchar*)(constbuf.data() + a.constofs), (size_t)cn * sesz);
        અચલ size_t ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        જો (sd == dd) memcpy(dst, srcbytes, (size_t)cn * desz);
        નહીંતર getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, Size(cn, 1), nullptr);
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
    જો (ntemps == 0) પાછા;

    AutoBuffer<પૂર્ણાંક, 32> tempOfSlot(nslots);
    માટે (પૂર્ણાંક s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    માટે (પૂર્ણાંક s = 1; s < nslots; s++)
        જો (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<પૂર્ણાંક, 16> lastUse(ntemps);
    માટે (પૂર્ણાંક t = 0; t < ntemps; t++) lastUse[t] = -1;
    માટે (પૂર્ણાંક i = 0; i < ninsn; i++)
    {
        અચલ TExpr::Insn& ins = prog[i];
        પૂર્ણાંક as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        માટે (પૂર્ણાંક k = 0; k < 3; k++)
            જો (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    માટે (પૂર્ણાંક t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<પૂર્ણાંક, 16> freeBufs(ntemps);
    પૂર્ણાંક nfree = 0, nbuf = 0;
    માટે (પૂર્ણાંક i = 0; i < ninsn; i++)
    {
        અચલ TExpr::Insn& ins = prog[i];
        પૂર્ણાંક rt = tempOfSlot[ins.result];
        જો (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        પૂર્ણાંક as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        માટે (પૂર્ણાંક k = 0; k < 3; k++)
        {
            પૂર્ણાંક t = tempOfSlot[as[k]];
            જો (t >= 0 && lastUse[t] == i)
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
    માટે (પૂર્ણાંક b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    માટે (પૂર્ણાંક s = 1; s < nslots; s++)
        જો (arginfo[s].kind == TEMP)
        {
            પૂર્ણાંક b = bufferOfTemp[arginfo[s].index], e = (પૂર્ણાંક)CV_ELEM_SIZE1(arginfo[s].depth);
            જો (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    માટે (પૂર્ણાંક b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    અચલ પૂર્ણાંક totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : INT_MAX;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
સ્થિર શૂન્ય matLogical(અચલ Mat& m, MatShape& shp, EwSteps& step, પૂર્ણાંક& esz1)
{
    esz1 = (પૂર્ણાંક)m.elemSize1();
    પૂર્ણાંક nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    માટે (પૂર્ણાંક i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
સ્થિર તાર્કિક broadcastShape(અચલ MatShape* shps, પૂર્ણાંક K, MatShape& out)
{
    પૂર્ણાંક nd = 0;
    માટે (પૂર્ણાંક k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    માટે (પૂર્ણાંક k = 0; k < K; k++)
    {
        અચલ MatShape& s = shps[k];
        પૂર્ણાંક off = nd - s.dims;
        માટે (પૂર્ણાંક i = 0; i < s.dims; i++)
        {
            પૂર્ણાંક d = s[i], &o = out[off + i];
            જો (o == 1) o = d;
            નહીંતર જો (d != 1 && d != o) પાછા ખોટું;
        }
    }
    પાછા સાચું;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
સ્થિર પૂર્ણાંક opCost(ટોચ op)
{
    પસંદગી (op)
    {
    સ્થિતિ OP_ADD: સ્થિતિ OP_SUB: સ્થિતિ OP_MUL: સ્થિતિ OP_MIN: સ્થિતિ OP_MAX:
    સ્થિતિ OP_ABSDIFF: સ્થિતિ OP_AND: સ્થિતિ OP_OR: સ્થિતિ OP_XOR: સ્થિતિ OP_NOT:
    સ્થિતિ OP_NEG: સ્થિતિ OP_ABS: સ્થિતિ OP_CAST: સ્થિતિ OP_RELU: સ્થિતિ OP_SELECT:
    સ્થિતિ OP_CMP_EQ: સ્થિતિ OP_CMP_NE: સ્થિતિ OP_CMP_LT:
    સ્થિતિ OP_CMP_LE: સ્થિતિ OP_CMP_GT: સ્થિતિ OP_CMP_GE: પાછા 1;
    સ્થિતિ OP_DIV: સ્થિતિ OP_SQRT: સ્થિતિ OP_HYPOT: સ્થિતિ OP_CONVERT_SCALE: પાછા 3;
    સ્થિતિ OP_SIN: સ્થિતિ OP_COS: સ્થિતિ OP_TANH: સ્થિતિ OP_ERF: સ્થિતિ OP_ATAN2:
    સ્થિતિ OP_EXP: સ્થિતિ OP_LOG: સ્થિતિ OP_POW:           પાછા 6;
    મૂળભૂત:                                         પાછા 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
શૂન્ય TExpr::clear()
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
શૂન્ય TExpr::dump(std::ostream& os) અચલ
{
    auto dn = [](પૂર્ણાંક d) -> અચલ અક્ષર* {
        પસંદગી (d) {
        સ્થિતિ EW_DEPTH_NONE: પાછા "flex";
        સ્થિતિ CV_8U:  પાછા "u8";  સ્થિતિ CV_8S:  પાછા "s8";
        સ્થિતિ CV_16U: પાછા "u16"; સ્થિતિ CV_16S: પાછા "s16";
        સ્થિતિ CV_32U: પાછા "u32"; સ્થિતિ CV_32S: પાછા "s32";
        સ્થિતિ CV_64U: પાછા "u64"; સ્થિતિ CV_64S: પાછા "s64";
        સ્થિતિ CV_16F: પાછા "f16"; સ્થિતિ CV_16BF:પાછા "bf16";
        સ્થિતિ CV_32F: પાછા "f32"; સ્થિતિ CV_64F: પાછા "f64";
        સ્થિતિ CV_Bool:પાછા "bool";
        મૂળભૂત:     પાછા "?"; }
    };
    auto kn = [](ArgKind k) -> અચલ અક્ષર* {
        પસંદગી (k) { સ્થિતિ NONE: પાછા "none"; સ્થિતિ આગત: પાછા "in"; સ્થિતિ CONST: પાછા "const";
                     સ્થિતિ TEMP: પાછા "temp"; સ્થિતિ OUTPUT: પાછા "out"; } પાછા "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (પૂર્ણાંક)prog.size() << "\n";
    os << "  slots:\n";
    માટે (પૂર્ણાંક s = 0; s < (પૂર્ણાંક)arginfo.size(); s++) {
        અચલ Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        જો (a.kind != NONE) os << " " << dn(a.depth);
        જો (a.kind == આગત || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        જો (a.kind == CONST) {
            અચલ પૂર્ણાંક cn = std::max(1, a.channels);
            AutoBuffer<દ્વિગુણ, MAX_LOCAL_CN> v(cn);
            અચલ uchar* src = (અચલ uchar*)(constbuf.data() + a.constofs);
            જો (a.srcdepth == CV_64F) memcpy(v.data(), src, (size_t)cn * કદ(દ્વિગુણ));
            નહીંતર getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            માટે (પૂર્ણાંક c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    માટે (પૂર્ણાંક i = 0; i < (પૂર્ણાંક)prog.size(); i++) {
        અચલ Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        જો (ins.arg1) os << ", " << ins.arg1;
        જો (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        જો (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        જો (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        જો (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
પૂર્ણાંક TExpr::add_આગત(પૂર્ણાંક depth)
{
    Arg a; a.kind = આગત; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); પાછા (પૂર્ણાંક)arginfo.size() - 1;
}

પૂર્ણાંક TExpr::addOutput(પૂર્ણાંક depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); પાછા (પૂર્ણાંક)arginfo.size() - 1;
}

પૂર્ણાંક TExpr::addTemp(પૂર્ણાંક depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); પાછા (પૂર્ણાંક)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
પૂર્ણાંક TExpr::addConst(પૂર્ણાંક depth, અચલ Scalar& v, પૂર્ણાંક channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); પાછા (પૂર્ણાંક)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
પૂર્ણાંક TExpr::addConst(પૂર્ણાંક depth, પૂર્ણાંક srcdepth, અચલ શૂન્ય* data, પૂર્ણાંક channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); પાછા (પૂર્ણાંક)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
પૂર્ણાંક TExpr::typedConstFrom(પૂર્ણાંક srcSlot, પૂર્ણાંક depth)
{
    અચલ Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); પાછા (પૂર્ણાંક)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
પૂર્ણાંક TExpr::addInsn(ટોચ op, પૂર્ણાંક a0, પૂર્ણાંક a1, પૂર્ણાંક a2, પૂર્ણાંક result, અચલ TKernel& kernel, અચલ Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    પાછા (પૂર્ણાંક)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
પૂર્ણાંક TExpr::addInsn(ટોચ op, પૂર્ણાંક a0, પૂર્ણાંક a1, પૂર્ણાંક a2, પૂર્ણાંક result, અચલ Scalar& params)
{
    પૂર્ણાંક d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    પૂર્ણાંક d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    પૂર્ણાંક d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    પાછા addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
પૂર્ણાંક TExpr::maybeAddCast(પૂર્ણાંક arg, પૂર્ણાંક depth)
{
    જો (arginfo[arg].depth == depth) પાછા arg;
    પૂર્ણાંક t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    પાછા t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
સ્થિર અંતઃરેખ શૂન્ય runInsn(અચલ TExpr::Insn& ins,
                           અચલ શૂન્ય* p0, size_t y0, size_t x0,
                           અચલ શૂન્ય* p1, size_t y1, size_t x1,
                           અચલ શૂન્ય* p2, size_t y2, size_t x2,
                           શૂન્ય* pr, size_t yr, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પૂર્ણાંક code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

સંરચના EwBody {
    અચલ TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    અચલ પૂર્ણાંક* slotMap;
    અચલ signed અક્ષર* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    અચલ પૂર્ણાંક* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    પૂર્ણાંક ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

શૂન્ય TExpr::outputShape(અચલ Mat* અચલ* inputs, MatShape& spatial, પૂર્ણાંક& channels) અચલ
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    અચલ પૂર્ણાંક rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    તાર્કિક sameShape = ninputs >= 1;
    માટે (પૂર્ણાંક i = 1; sameShape && i < ninputs; i++)
    {
        અચલ Mat& a = *inputs[i];
        જો (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = ખોટું;
        નહીંતર માટે (પૂર્ણાંક d = 0; d < a.dims; d++) જો (a.size[d] != inputs[0]->size[d]) { sameShape = ખોટું; તોડો; }
    }
    જો (sameShape) { spatial = inputs[0]->size; channels = rcn0; પાછા; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; પૂર્ણાંક esz1;
    માટે (પૂર્ણાંક i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    અચલ પૂર્ણાંક ndFull = (પૂર્ણાંક)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
શૂન્ય TExpr::outputShape(અચલ Mat* inputs, MatShape& spatial, પૂર્ણાંક& channels) અચલ
{
    AutoBuffer<અચલ Mat*, 16> ptrs(std::max(ninputs, 1));
    માટે (પૂર્ણાંક i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
સ્થિર constexpr પૂર્ણાંક LOCAL_HDRS = 3;
સ્થિર constexpr પૂર્ણાંક LOCAL_CONSTS = 4;
સ્થિર constexpr પૂર્ણાંક LOCAL_OPS = 16;

શૂન્ય TExpr::exec(અચલ Mat* અચલ* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    અચલ પૂર્ણાંક nslots = (પૂર્ણાંક)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    જો (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        અચલ Mat& r = *inputs[0];
        અચલ પૂર્ણાંક rcn = r.channels();
        તાર્કિક ok = r.isContinuous();
        માટે (પૂર્ણાંક i = 1; ok && i < ninputs; i++)
        {
            અચલ Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        માટે (પૂર્ણાંક s = 1; ok && s < nslots; s++)
            જો (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        અચલ size_t total = ok ? r.total() * rcn : 0;
        જો (ok && total == 0)
        {
            અચલ MatShape rshape = r.size;
            માટે (પૂર્ણાંક s = 1; s < nslots; s++)
                જો (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            પાછા;
        }
        જો (ok && total <= EW_FASTPATH_MAX)
        {
            અચલ MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            માટે (પૂર્ણાંક s = 1; s < nslots; s++)
                જો (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            અચલ પૂર્ણાંક ninsn = (પૂર્ણાંક)prog.size();

            જો (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                auto ptrOf = [&](પૂર્ણાંક s) -> શૂન્ય* {
                    જો (s <= 0) પાછા nullptr;
                    અચલ Arg& ai = arginfo[s];
                    જો (ai.kind == આગત)  પાછા (શૂન્ય*)inputs[ai.index]->data;
                    જો (ai.kind == OUTPUT) પાછા (શૂન્ય*)outputs[ai.index].data;
                    પાછા nullptr;   // NONE
                };
                અચલ પૂર્ણાંક w = (પૂર્ણાંક)total;
                માટે (પૂર્ણાંક n = 0; n < ninsn; n++)
                {
                    અચલ Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                પાછા;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            અચલ પૂર્ણાંક totalEsz = bufEszPrefix[nbuffers];
            અચલ પૂર્ણાંક wf0 = std::min((પૂર્ણાંક)total, capElems);

            અચલ size_t region = alignSize((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/કદ(uint64_t)> scratchBuf(
                divUp((size_t)totalEsz * region, (unsigned)કદ(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            માટે (પૂર્ણાંક x0 = 0; x0 < (પૂર્ણાંક)total; x0 += wf0)
            {
                અચલ પૂર્ણાંક wf = std::min(wf0, (પૂર્ણાંક)total - x0);
                auto ptrOf = [&](પૂર્ણાંક s) -> શૂન્ય* {
                    જો (s <= 0) પાછા nullptr;
                    અચલ Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.depth);
                    જો (ai.kind == આગત)  પાછા (uchar*)inputs[ai.index]->data  + (size_t)x0 * esz;
                    જો (ai.kind == OUTPUT) પાછા (uchar*)outputs[ai.index].data + (size_t)x0 * esz;
                    જો (ai.kind != TEMP)   પાછા nullptr;                                    // NONE: moved-from
                    અચલ પૂર્ણાંક b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    પાછા scratch + (size_t)bufEszPrefix[b] * region;                        // fragment-local
                };
                માટે (પૂર્ણાંક n = 0; n < ninsn; n++)
                {
                    અચલ Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            પાછા;
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
    પૂર્ણાંક rchannels;
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
    તાર્કિક inplace = ખોટું;
    માટે (પૂર્ણાંક i = 0; i < ninputs && !inplace; i++)
        માટે (પૂર્ણાંક j = 0; j < noutputs; j++)
            જો (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = સાચું; તોડો; }
    પૂર્ણાંક nhdrs = inplace ? ninputs : 0;

    AutoBuffer<અચલ Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<અચલ Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<પૂર્ણાંક, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<signed અક્ષર, LOCAL_OPS> slotKind(nslots);
    પૂર્ણાંક narr = 0, nc = 0;
    જો (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        માટે (પૂર્ણાંક j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (signed અક્ષર)TExpr::NONE;
    માટે (પૂર્ણાંક s = 1; s < nslots; s++)
    {
        અચલ TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (signed અક્ષર)ai.kind;
        જો (ai.kind == TExpr::આગત) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        નહીંતર જો (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        નહીંતર જો (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (signed અક્ષર)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        નહીંતર જો (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            અચલ પૂર્ણાંક c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (શૂન્ય*)(constbuf.data() + ai.constofs));
            slotKind[s] = (signed અક્ષર)TExpr::આગત;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        નહીંતર જો (ai.kind == TExpr::TEMP) {
            slotMap[s] = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // physical buffer id
        }
        નહીંતર {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            slotMap[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    અચલ પૂર્ણાંક ninsn = (પૂર્ણાંક)prog.size();
    long long costPerElem = 0;
    માટે (પૂર્ણાંક n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    long long otot = (long long)rchannels;
    માટે (પૂર્ણાંક d = 0; d < (પૂર્ણાંક)spatial.size(); d++) otot *= spatial[d];
    જો (otot == 0)
        પાછા;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    અચલ દ્વિગુણ T = (દ્વિગુણ)std::max(get_અંક_Threads(), 1);
    અચલ દ્વિગુણ nstripes = std::min(
        (દ્વિગુણ)otot * (દ્વિગુણ)std::max<long long>(costPerElem, 1) * (1./ (દ્વિગુણ)(1 << 16)),
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

    broadcastOp(arr.data(), narr, [&](અચલ BrTile& tile)
    {
        EwBody& bc = body;
        અચલ TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        અચલ પૂર્ણાંક* અચલ smap  = bc.slotMap;
        અચલ signed અક્ષર* અચલ skind = bc.slotKind;
        અચલ પૂર્ણાંક nin = bc.ninsn, nsl = bc.nslots;
        અચલ પૂર્ણાંક w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        અચલ પૂર્ણાંક* અચલ eszPrefix = bc.bufEszPrefix;
        અચલ પૂર્ણાંક bw = std::min(w, bc.capElems);
        અચલ પૂર્ણાંક bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        અચલ size_t region = alignSize((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/કદ(uint64_t)> tstoreBuf(
            divUp((size_t)eszPrefix[bc.nbuffers] * region, (unsigned)કદ(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        માટે (પૂર્ણાંક y0 = 0; y0 < h; y0 += bh)
        {
            અચલ પૂર્ણાંક hf = std::min(bh, h - y0);
            માટે (પૂર્ણાંક x0 = 0; x0 < w; x0 += bw)
            {
                અચલ પૂર્ણાંક wf = std::min(bw, w - x0);
                માટે (પૂર્ણાંક s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    અચલ પૂર્ણાંક k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    જો (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)eszPrefix[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    નહીંતર                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        અચલ BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                માટે (પૂર્ણાંક n = 0; n < nin; n++)
                {
                    અચલ TExpr::Insn& ins = insns[n];
                    અચલ BrSlice& a0 = args[ins.arg0]; અચલ BrSlice& a1 = args[ins.arg1];
                    અચલ BrSlice& a2 = args[ins.arg2]; અચલ BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (શૂન્ય*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, સાચું, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
શૂન્ય TExpr::exec(અચલ Mat* inputs, Mat* outputs)
{
    AutoBuffer<અચલ Mat*, 16> ptrs(std::max(ninputs, 1));
    માટે (પૂર્ણાંક i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
શૂન્ય make_દ્વિઅંકી_Arith_Program(TExpr& p, ટોચ op, પૂર્ણાંક depth0, પૂર્ણાંક depth1, પૂર્ણાંક rdepth,
                            પૂર્ણાંક maskDepth, દ્વિગુણ scale)
{
    p.clear();
    જો (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        જો (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        નહીંતર
        {
            rdepth = promoteArith(depth0, depth1);
            જો (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    અચલ તાર્કિક masked = maskDepth != EW_DEPTH_NONE;
    પૂર્ણાંક sIn0  = p.add_આગત(depth0);
    પૂર્ણાંક sIn1  = p.add_આગત(depth1);
    પૂર્ણાંક sMask = masked ? p.add_આગત(maskDepth) : 0;
    પૂર્ણાંક sOut  = p.addOutput(rdepth);

    પૂર્ણાંક r = p.emit_દ્વિઅંકી(op, sIn0, sIn1, rdepth, Scalar(scale));
    જો (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    નહીંતર
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
શૂન્ય makeAddWeightedProgram(TExpr& p, પૂર્ણાંક depth0, પૂર્ણાંક depth1, પૂર્ણાંક rdepth,
                            દ્વિગુણ alpha, દ્વિગુણ beta, દ્વિગુણ gamma)
{
    p.clear();
    પૂર્ણાંક sA   = p.add_આગત(depth0);
    પૂર્ણાંક sB   = p.add_આગત(depth1);
    પૂર્ણાંક sOut = p.addOutput(rdepth);
    પૂર્ણાંક r = p.emit_દ્વિઅંકી(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

નામાકાશ {

// --- tokens ------------------------------------------------------------------------------
ગણના TokType { T_અંક, T_આગત, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

સંરચના Token
{
    TokType type = T_END;
    દ્વિગુણ num = 0;
    પૂર્ણાંક આગત_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
સંરચના Lexer
{
    std::string_view s;
    size_t pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    સ્થિર તાર્કિક isIdentStart(અક્ષર c) { પાછા std::isalpha((unsigned અક્ષર)c) || c == '_'; }
    સ્થિર તાર્કિક isIdentChar(અક્ષર c)  { પાછા std::isalnum((unsigned અક્ષર)c) || c == '_'; }

    Token next()
    {
        જ્યારે (pos < s.size() && std::isspace((unsigned અક્ષર)s[pos])) pos++;
        Token t;
        જો (pos >= s.size()) { t.type = T_END; પાછા t; }

        અક્ષર c = s[pos];

        જો (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t start = pos;
            જ્યારે (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_આગત;
            t.આગત_2 = std::atoi(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            પાછા t;
        }
        જો (std::isdigit((unsigned અક્ષર)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::isdigit((unsigned અક્ષર)s[pos + 1])))
        {
            અક્ષર* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_અંક;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            પાછા t;
        }
        જો (isIdentStart(c))
        {
            size_t start = pos;
            જ્યારે (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            પાછા t;
        }
        પસંદગી (c)
        {
        સ્થિતિ '(': pos++; t.type = T_LPAREN; પાછા t;
        સ્થિતિ ')': pos++; t.type = T_RPAREN; પાછા t;
        સ્થિતિ ',': pos++; t.type = T_COMMA;  પાછા t;
        સ્થિતિ ';': pos++; t.type = T_SEMI;   પાછા t;
        }
        // multi/!single-char operators
        auto two = [&](અચલ અક્ષર* op) {
            પાછા pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        જો (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; પાછા t; }
        જો (c == '=') { pos++; t.type = T_ASSIGN; પાછા t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        પાછા t;
    }
};

// --- operator / function tables ----------------------------------------------------------
સ્થિર પૂર્ણાંક binPrec(અચલ std::string& op)
{
    જો (op == "**") પાછા 8;              // power binds tighter than '*'; RIGHT-associative
    જો (op == "*" || op == "/") પાછા 7;
    જો (op == "+" || op == "-") પાછા 6;
    જો (op == "<" || op == "<=" || op == ">" || op == ">=") પાછા 5;
    જો (op == "==" || op == "!=") પાછા 4;
    જો (op == "&") પાછા 3;
    જો (op == "^") પાછા 2;
    જો (op == "|") પાછા 1;
    પાછા -1;
}

સ્થિર ટોચ binOp(અચલ std::string& op)
{
    જો (op == "**") પાછા OP_POW;
    જો (op == "+")  પાછા OP_ADD;
    જો (op == "-")  પાછા OP_SUB;
    જો (op == "*")  પાછા OP_MUL;
    જો (op == "/")  પાછા OP_DIV;
    જો (op == "<")  પાછા OP_CMP_LT;
    જો (op == "<=") પાછા OP_CMP_LE;
    જો (op == ">")  પાછા OP_CMP_GT;
    જો (op == ">=") પાછા OP_CMP_GE;
    જો (op == "==") પાછા OP_CMP_EQ;
    જો (op == "!=") પાછા OP_CMP_NE;
    જો (op == "&")  પાછા OP_AND;
    જો (op == "|")  પાછા OP_OR;
    જો (op == "^")  પાછા OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
સ્થિર પૂર્ણાંક typeDepth(અચલ std::string& name)
{
    જો (name == "float")    પાછા CV_32F;
    જો (name == "double")   પાછા CV_64F;
    જો (name == "half" || name == "float16") પાછા CV_16F;
    જો (name == "bfloat16") પાછા CV_16BF;
    જો (name == "uint8")    પાછા CV_8U;
    જો (name == "int8")     પાછા CV_8S;
    જો (name == "uint16")   પાછા CV_16U;
    જો (name == "int16")    પાછા CV_16S;
    જો (name == "uint32")   પાછા CV_32U;
    જો (name == "int32")    પાછા CV_32S;
    જો (name == "uint64")   પાછા CV_64U;
    જો (name == "int64")    પાછા CV_64S;
    પાછા -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
સ્થિર ટોચ fnOp(અચલ std::string& name, પૂર્ણાંક& arity)
{
    સંરચના E { અચલ અક્ષર* n; ટોચ op; };
    સ્થિર અચલ E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    સ્થિર અચલ E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    સ્થિર અચલ E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    માટે (અચલ E& e : unary)  જો (name == e.n) { arity = 1; પાછા e.op; }
    માટે (અચલ E& e : binary) જો (name == e.n) { arity = 2; પાછા e.op; }
    માટે (અચલ E& e : tern)   જો (name == e.n) { arity = 3; પાછા e.op; }
    arity = 0; પાછા OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
સંરચના Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    અચલ પૂર્ણાંક* આગત_Slot;                 // slot id of each external input (precreated)
    પૂર્ણાંક ninputs;
    std::map<std::string, પૂર્ણાંક> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& પદાવલિ, અચલ પૂર્ણાંક* islot, પૂર્ણાંક nin)
        : lex(src), e(પદાવલિ), આગત_Slot(islot), ninputs(nin) { cur = lex.next(); }

    શૂન્ય advance() { cur = lex.next(); }
    તાર્કિક isOp(અચલ અક્ષર* op) અચલ { પાછા cur.type == T_OP && cur.text == op; }

    શૂન્ય expect(TokType t, અચલ અક્ષર* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    તાર્કિક isFlexConst(પૂર્ણાંક slot) અચલ
    {
        પાછા e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    પૂર્ણાંક parsePrimary()
    {
        જો (cur.type == T_અંક)   { પૂર્ણાંક s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); પાછા s; }
        જો (cur.type == T_આગત) { પૂર્ણાંક idx = cur.આગત_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   પાછા આગત_Slot[idx]; }
        જો (cur.type == T_LPAREN){ advance(); પૂર્ણાંક x = parseTernary(); expect(T_RPAREN, "expected ')'"); પાછા x; }
        જો (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            જો (cur.type != T_LPAREN)             // variable reference
            {
                auto it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                પાછા it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<પૂર્ણાંક, 8> args = {}; પૂર્ણાંક nargs = 0;
            જો (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                જ્યારે (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (પૂર્ણાંક)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            પૂર્ણાંક td = typeDepth(name);
            જો (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                પૂર્ણાંક s = args[0];
                પાછા isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            પૂર્ણાંક arity = 0; ટોચ op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            જો (arity == 1) પાછા e.emitUnary(op, args[0]);
            જો (arity == 2) પાછા e.emit_દ્વિઅંકી(op, args[0], args[1]);
            પાછા e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    પૂર્ણાંક parseUnary()
    {
        જો (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            પૂર્ણાંક operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            જો (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<દ્વિગુણ, MAX_LOCAL_CN> v; પૂર્ણાંક cn = constDoubles(e, operand, v);
                Scalar neg; માટે (પૂર્ણાંક i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                પાછા e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            પાછા e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        પાછા parsePrimary();
    }

    પૂર્ણાંક parse_પદાવલિ(પૂર્ણાંક minPrec)
    {
        પૂર્ણાંક ડાબું = parseUnary();
        જ્યારે (cur.type == T_OP)
        {
            પૂર્ણાંક p = binPrec(cur.text);
            જો (p < minPrec) તોડો;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            પૂર્ણાંક જમણું = parse_પદાવલિ(op == "**" ? p : p + 1);
            ડાબું = e.emit_દ્વિઅંકી(binOp(op), ડાબું, જમણું);
        }
        પાછા ડાબું;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    પૂર્ણાંક parseTernary()
    {
        પૂર્ણાંક cond = parse_પદાવલિ(0);
        જો (!isOp("?")) પાછા cond;
        advance();
        પૂર્ણાંક thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        પૂર્ણાંક elsev = parseTernary();               // right-associative chain
        પાછા e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    શૂન્ય parseResult()
    {
        જો (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); પૂર્ણાંક nTemp = e.ntemps;
            advance();
            પૂર્ણાંક e0 = parseTernary();
            જો (cur.type == T_COMMA)
            {
                e.output(e0);
                જ્યારે (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                પાછા;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    શૂન્ય parse()
    {
        જ્યારે (સાચું)
        {
            જો (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                જો (cur.type == T_ASSIGN)
                {
                    advance();
                    અચલ પૂર્ણાંક slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = સાચું;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    ચાલુ;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            જો (cur.type == T_SEMI) advance();
            તોડો;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

નામાકાશ cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
શૂન્ય texpr(અચલ std::string& પદાવલિ, આગત_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using નામાકાશ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == આગત_Array_2::STD_VECTOR_MAT);
    અચલ std::vector<Mat>& inps = *(અચલ std::vector<Mat>*)_inputs.getObj();
    અચલ પૂર્ણાંક ninputs = (પૂર્ણાંક)inps.size();
    માટે (પૂર્ણાંક i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<પૂર્ણાંક> islot(std::max(ninputs, 1));
    માટે (પૂર્ણાંક i = 0; i < ninputs; i++) islot[i] = e.add_આગત(inps[i].depth());
    Parser(પદાવલિ, e, islot.data(), ninputs).parse();
    e.compile();

    auto kind = _outputs.kind();
    જો (kind == આગત_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } નહીંતર {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
