// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "arithm_expr.hpp"
#అన్తర్భావ <algorithm>
#అన్తర్భావ <climits>
#అన్తర్భావ <cmath>
#అన్తర్భావ <cstdlib>
#అన్తర్భావ <cstring>
#అన్తర్భావ <map>
#అన్తర్భావ <ostream>
#అన్తర్భావ <string>

నామాకాశ cv { నామాకాశ ew {

// ============================ op metadata (was ew_op.cpp) ============================

స్థిరరాశి అక్షరం* opName(శిఖరం op)
{
    ఎంపిక (op)
    {
    స్థితి OP_NOP:           తిరిగి "nop";
    స్థితి OP_NEG:           తిరిగి "neg";
    స్థితి OP_ABS:           తిరిగి "abs";
    స్థితి OP_NOT:           తిరిగి "not";
    స్థితి OP_SQRT:          తిరిగి "sqrt";
    స్థితి OP_EXP:           తిరిగి "exp";
    స్థితి OP_LOG:           తిరిగి "log";
    స్థితి OP_SIN:           తిరిగి "sin";
    స్థితి OP_COS:           తిరిగి "cos";
    స్థితి OP_TANH:          తిరిగి "tanh";
    స్థితి OP_ERF:           తిరిగి "erf";
    స్థితి OP_RELU:          తిరిగి "relu";
    స్థితి OP_CAST:          తిరిగి "cast";
    స్థితి OP_కూడు:           తిరిగి "add";
    స్థితి OP_SUB:           తిరిగి "sub";
    స్థితి OP_MUL:           తిరిగి "mul";
    స్థితి OP_DIV:           తిరిగి "div";
    స్థితి OP_POW:           తిరిగి "pow";
    స్థితి OP_MIN:           తిరిగి "min";
    స్థితి OP_MAX:           తిరిగి "max";
    స్థితి OP_ABSDIFF:       తిరిగి "absdiff";
    స్థితి OP_ADDW:          తిరిగి "addWeighted";
    స్థితి OP_HYPOT:         తిరిగి "hypot";
    స్థితి OP_ATAN2:         తిరిగి "atan2";
    స్థితి OP_AND:           తిరిగి "and";
    స్థితి OP_OR:            తిరిగి "or";
    స్థితి OP_XOR:           తిరిగి "xor";
    స్థితి OP_CMP_EQ:        తిరిగి "cmp_eq";
    స్థితి OP_CMP_NE:        తిరిగి "cmp_ne";
    స్థితి OP_CMP_LT:        తిరిగి "cmp_lt";
    స్థితి OP_CMP_LE:        తిరిగి "cmp_le";
    స్థితి OP_CMP_GT:        తిరిగి "cmp_gt";
    స్థితి OP_CMP_GE:        తిరిగి "cmp_ge";
    స్థితి OP_CLAMP:         తిరిగి "clamp";
    స్థితి OP_SELECT:        తిరిగి "select";
    స్థితి OP_CONVERT_SCALE: తిరిగి "convert_scale";
    అప్రమేయ:               తిరిగి "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(శిఖరం op)
{
    ఎంపిక (op)
    {
    స్థితి OP_AND: స్థితి OP_OR: స్థితి OP_XOR: స్థితి OP_NOT:
        తిరిగి CAT_BITWISE;
    స్థితి OP_CMP_EQ: స్థితి OP_CMP_NE: స్థితి OP_CMP_LT:
    స్థితి OP_CMP_LE: స్థితి OP_CMP_GT: స్థితి OP_CMP_GE:
        తిరిగి CAT_COMPARE;
    స్థితి OP_SQRT: స్థితి OP_EXP: స్థితి OP_LOG:
    స్థితి OP_SIN: స్థితి OP_COS: స్థితి OP_TANH: స్థితి OP_ERF: స్థితి OP_ATAN2: స్థితి OP_RELU:
        తిరిగి CAT_MATH;
    స్థితి OP_CAST: స్థితి OP_CONVERT_SCALE:
        తిరిగి CAT_CAST;
    స్థితి OP_SELECT:
        తిరిగి CAT_SELECT;
    అప్రమేయ:
        తిరిగి CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
స్థిర తార్కిక is_Float_లోతు(పూర్ణాంకం d)
{
    తిరిగి d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
స్థిర తార్కిక లోతు_Represents(ద్విగుణ v, పూర్ణాంకం లోతు)
{
    ఒకవేళ (is_Float_లోతు(లోతు)) తిరిగి నిజం;
    ఒకవేళ (v != std::floor(v)) తిరిగి అబద్ధం;
    ఎంపిక (లోతు)
    {
    స్థితి CV_8U:  తిరిగి v >= 0            && v <= 255;
    స్థితి CV_8S:  తిరిగి v >= -128         && v <= 127;
    స్థితి CV_16U: తిరిగి v >= 0            && v <= 65535;
    స్థితి CV_16S: తిరిగి v >= -32768       && v <= 32767;
    స్థితి CV_32U: తిరిగి v >= 0            && v <= 4294967295.0;
    స్థితి CV_32S: తిరిగి v >= -2147483648.0 && v <= 2147483647.0;
    స్థితి CV_64U: తిరిగి v >= 0            && v <= 18446744073709551615.0;
    స్థితి CV_64S: తిరిగి v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    అప్రమేయ:     తిరిగి అబద్ధం;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
స్థిర శూన్య intRange(పూర్ణాంకం లోతు, ద్విగుణ& lo, ద్విగుణ& hi)
{
    ఎంపిక (లోతు)
    {
    స్థితి CV_8U:  lo = 0;                     hi = 255; ఆపు;
    స్థితి CV_8S:  lo = -128;                  hi = 127; ఆపు;
    స్థితి CV_16U: lo = 0;                     hi = 65535; ఆపు;
    స్థితి CV_16S: lo = -32768;                hi = 32767; ఆపు;
    స్థితి CV_32U: lo = 0;                     hi = 4294967295.0; ఆపు;
    స్థితి CV_32S: lo = -2147483648.0;         hi = 2147483647.0; ఆపు;
    స్థితి CV_64U: lo = 0;                     hi = 18446744073709551615.0; ఆపు;
    స్థితి CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; ఆపు;
    అప్రమేయ:     lo = 0;                     hi = 0; ఆపు;
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
పూర్ణాంకం promoteArith(పూర్ణాంకం a, పూర్ణాంకం b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr గుర్తులేని signlut = 0b1001111110100u;
    constexpr గుర్తులేని fltlut  = 0b1111000000u;

    ఒకవేళ (a == b) తిరిగి a;

    పూర్ణాంకం wa = పూర్ణాంకం((leszlut >> (a+1)*3) & 7u), wb = పూర్ణాంకం((leszlut >> (b+1)*3) & 7u);
    పూర్ణాంకం fa = పూర్ణాంకం((fltlut >> (a+1)) & 1u),    fb = పూర్ణాంకం((fltlut >> (b+1)) & 1u);

    ఒకవేళ (fa + fb == 0)                          // both integer
    {
        పూర్ణాంకం sa = పూర్ణాంకం((signlut >> (a+1)) & 1u), sb = పూర్ణాంకం((signlut >> (b+1)) & 1u);
        ఒకవేళ (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            తిరిగి wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        పూర్ణాంకం rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr పూర్ణాంకం ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        తిరిగి (ilut >> rw*5) & CV_MAT_లోతు_ముసుగు;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    పూర్ణాంకం maxw = std::max(wa, wb);
    ఒకవేళ (maxw == 2 && fa + fb == 1)
        తిరిగి fa*a + (1 - fa)*b;
    తిరిగి CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
పూర్ణాంకం absdiff_ఫలితం_లోతు(పూర్ణాంకం లోతు)
{
    ఎంపిక (లోతు)
    {
    స్థితి CV_8S:  తిరిగి CV_8U;
    స్థితి CV_16S: తిరిగి CV_16U;
    స్థితి CV_32S: తిరిగి CV_32U;
    స్థితి CV_64S: తిరిగి CV_64U;
    అప్రమేయ:     తిరిగి లోతు;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
స్థిర పూర్ణాంకం promote2(పూర్ణాంకం a, పూర్ణాంకం b)
{
    ఒకవేళ (a == b) తిరిగి a;

    constexpr గుర్తులేని lbits = 3, lmask = (1u << lbits) - 1u;
    స్థిరరాశి uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    గుర్తులేని pr_a = గుర్తులేని((typelut >> (a*lbits)) & lmask);
    గుర్తులేని pr_b = గుర్తులేని((typelut >> (b*lbits)) & lmask);
    గుర్తులేని max_pr = std::max(pr_a, pr_b);
    constexpr గుర్తులేని dbits = CV_CN_జరుపు, dmask = (1u << dbits) - 1u;
    స్థిరరాశి గుర్తులేని ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    తిరిగి పూర్ణాంకం((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
స్థిర పూర్ణాంకం promoteBitwise(పూర్ణాంకం a, పూర్ణాంకం b)
{
    ఒకవేళ (a == EW_లోతు_NONE) తిరిగి b;
    ఒకవేళ (b == EW_లోతు_NONE) తిరిగి a;
    ఒకవేళ (a == b) తిరిగి a;
    తిరిగి CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
స్థిర పూర్ణాంకం సురక్షిత_Wide(పూర్ణాంకం లోతు)
{
    constexpr గుర్తులేని lbits = 3, lmask = (1u << lbits) - 1u;
    స్థిరరాశి uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    గుర్తులేని pr_లోతు = గుర్తులేని((typelut >> (లోతు*lbits)) & lmask);
    constexpr గుర్తులేని dbits = CV_CN_జరుపు, dmask = (1u << dbits) - 1u;
    స్థిరరాశి గుర్తులేని ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    తిరిగి పూర్ణాంకం((ctypelut >> (pr_లోతు*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
గణన { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
స్థిర size_t appendConstBuf(Auto_బఫరు<uint64_t, 16>& constbuf, పూర్ణాంకం srcdepth, స్థిరరాశి శూన్య* దత్తాంశం, పూర్ణాంకం channels)
{
    స్థిరరాశి పూర్ణాంకం cn = std::max(1, channels);
    స్థిరరాశి size_t nbytes = (size_t)cn * CV_ELEM_SIZE1(srcdepth);
    స్థిరరాశి size_t nu64 = (nbytes + పరిమాణం(uint64_t) - 1) / పరిమాణం(uint64_t);
    స్థిరరాశి size_t ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.దత్తాంశం() + ofs);
    ఒకవేళ (దత్తాంశం) జ్ఞాపకం_నకలు(dst, దత్తాంశం, nbytes); లేకపోతే memset(dst, 0, nbytes);
    తిరిగి ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
స్థిర పూర్ణాంకం constDoubles(స్థిరరాశి TExpr& e, పూర్ణాంకం s, Auto_బఫరు<ద్విగుణ, MAX_LOCAL_CN>& buf)
{
    స్థిరరాశి TExpr::Arg& a = e.arginfo[s];
    స్థిరరాశి పూర్ణాంకం cn = std::max(1, a.channels);
    buf.resize(cn);
    స్థిరరాశి uchar* src = (స్థిరరాశి uchar*)(e.constbuf.దత్తాంశం() + a.constofs);
    ఒకవేళ (a.srcdepth == CV_64F)
        జ్ఞాపకం_నకలు(buf.దత్తాంశం(), src, (size_t)cn * పరిమాణం(ద్విగుణ));   // common (Scalar / parsed literal)
    లేకపోతే
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, శూన్యసూచిక, 0, (uchar*)buf.దత్తాంశం(), 0, కొలత(cn, 1), శూన్యసూచిక);
    తిరిగి cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
స్థిర అంతఃవరుస తార్కిక isFlexConst(స్థిరరాశి TExpr& e, పూర్ణాంకం s)
{
    తిరిగి e.arginfo[s].రకం == TExpr::CONST && e.arginfo[s].లోతు == EW_లోతు_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
స్థిర తార్కిక scalar_Const_విలువ(స్థిరరాశి TExpr& e, పూర్ణాంకం s, ద్విగుణ& v)
{
    స్థిరరాశి TExpr::Arg& a = e.arginfo[s];
    ఒకవేళ (a.రకం != TExpr::CONST || std::max(1, a.channels) != 1) తిరిగి అబద్ధం;
    Auto_బఫరు<ద్విగుణ, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    తిరిగి నిజం;
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
స్థిర తార్కిక సరిపోలిక_Scaled_Chain(స్థిరరాశి TExpr& e, పూర్ణాంకం end, పూర్ణాంకం t,
                             పూర్ణాంకం& arr_స్థానం, ద్విగుణ& scale, పూర్ణాంకం& ప్రారంభం_Idx)
{
    ఒకవేళ (end < 0) తిరిగి అబద్ధం;
    పూర్ణాంకం j = end, mul_ఫలితం = t;

    // optional trailing narrowing cast
    ఒకవేళ (e.prog[j].op == OP_CAST && e.prog[j].ఫలితం == t)
    {
        mul_ఫలితం = e.prog[j].arg0;
        ఒకవేళ (--j < 0) తిరిగి అబద్ధం;
    }

    స్థిరరాశి TExpr::Insn& mul = e.prog[j];
    ఒకవేళ (mul.op != OP_MUL || mul.ఫలితం != mul_ఫలితం || mul.params[0] != 1.0) తిరిగి అబద్ధం;

    పూర్ణాంకం arr;
    ఒకవేళ      (scalar_Const_విలువ(e, mul.arg1, scale)) arr = mul.arg0;
    లేకపోతే ఒకవేళ (scalar_Const_విలువ(e, mul.arg0, scale)) arr = mul.arg1;
    లేకపోతే తిరిగి అబద్ధం;                                  // array*array is not an addWeighted term
    ప్రారంభం_Idx = j;

    // optional leading widening cast feeding the multiply
    ఒకవేళ (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].ఫలితం == arr &&
        e.arginfo[arr].రకం == TExpr::తాత్కాలిక)
    {
        arr = e.prog[j-1].arg0;
        ప్రారంభం_Idx = j - 1;
    }

    ఒకవేళ (e.arginfo[arr].రకం == TExpr::CONST) తిరిగి అబద్ధం;   // scalar*scalar is folded elsewhere
    arr_స్థానం = arr;
    తిరిగి నిజం;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
స్థిర తార్కిక const_సరిపోతుంది(స్థిరరాశి TExpr& e, పూర్ణాంకం s, పూర్ణాంకం d)
{
    ఒకవేళ (!isFlexConst(e, s)) తిరిగి నిజం;
    Auto_బఫరు<ద్విగుణ, MAX_LOCAL_CN> v;
    పూర్ణాంకం cn = constDoubles(e, s, v);
    కొరకు (పూర్ణాంకం ch = 0; ch < cn; ch++)
        ఒకవేళ (!లోతు_Represents(v[ch], d)) తిరిగి అబద్ధం;
    తిరిగి నిజం;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
పూర్ణాంకం TExpr::emit_ద్విమాన(శిఖరం op, పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం rdepth, స్థిరరాశి Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    ఒకవేళ (op == OP_ADDW)
    {
        పూర్ణాంకం Tt = arginfo[a].లోతు;
        ఒకవేళ (arginfo[a].లోతు != arginfo[b].లోతు)
        {
            Tt = promoteArith(arginfo[a].లోతు, arginfo[b].లోతు);
            a = maybe_కూడు_Cast(a, Tt); b = maybe_కూడు_Cast(b, Tt);
        }
        ఒకవేళ (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        ఒకవేళ (rdepth == EW_లోతు_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_లోతు_NONE, rdepth);
        పూర్ణాంకం బహిర్గతం_D = rdepth;
        ఒకవేళ (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            బహిర్గతం_D = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_లోతు_NONE, బహిర్గతం_D);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        స్థిరరాశి పూర్ణాంకం out = కూడు_తాత్కాలిక(బహిర్గతం_D);
        కూడు_Insn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        ఒకవేళ (బహిర్గతం_D == rdepth) తిరిగి out;
        స్థిరరాశి పూర్ణాంకం out2 = కూడు_తాత్కాలిక(rdepth);
        కూడు_Insn(OP_CAST, out, 0, 0, out2);
        తిరిగి out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    ఒకవేళ (op == OP_కూడు)
    {
        స్థిరరాశి పూర్ణాంకం n = (పూర్ణాంకం)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        పూర్ణాంకం x = 0, y = 0, ప్రారంభం_A = 0, ప్రారంభం_B = 0;
        ద్విగుణ alpha = 0, beta = 0;
        ఒకవేళ (n >= 2 && a != b &&
            arginfo[a].రకం == తాత్కాలిక && !arginfo[a].pinned &&
            arginfo[b].రకం == తాత్కాలిక && !arginfo[b].pinned &&
            సరిపోలిక_Scaled_Chain(*ఇది, n - 1, b, y, beta, ప్రారంభం_B) &&
            సరిపోలిక_Scaled_Chain(*ఇది, ప్రారంభం_B - 1, a, x, alpha, ప్రారంభం_A) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].లోతు != CV_Bool && arginfo[y].లోతు != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            పూర్ణాంకం retired = 0, lowest = గరిష్ఠ_పూర్ణాంకం;
            కొరకు (పూర్ణాంకం i = ప్రారంభం_A; i < n; i++)
            {
                స్థిరరాశి పూర్ణాంకం r = prog[i].ఫలితం;
                ఒకవేళ (arginfo[r].రకం != తాత్కాలిక || arginfo[r].pinned) { retired = -1; ఆపు; }
                lowest = std::min(lowest, arginfo[r].సూచీ);
                retired++;
            }
            ఒకవేళ (retired > 0 && lowest == ntemps - retired)
            {
                కొరకు (పూర్ణాంకం i = ప్రారంభం_A; i < n; i++) arginfo[prog[i].ఫలితం].రకం = NONE;
                prog.resize(ప్రారంభం_A);
                ntemps -= retired;
                తిరిగి emit_ద్విమాన(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            పూర్ణాంకం acc = 0; ద్విగుణ gamma = 0;
            ఒకవేళ      (scalar_Const_విలువ(*ఇది, b, gamma)) acc = a;
            లేకపోతే ఒకవేళ (scalar_Const_విలువ(*ఇది, a, gamma)) acc = b;
            ఒకవేళ (acc != 0 && n >= 1 &&
                arginfo[acc].రకం == తాత్కాలిక && !arginfo[acc].pinned &&
                arginfo[acc].సూచీ == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].ఫలితం == acc &&
                (rdepth == EW_లోతు_NONE || rdepth == arginfo[acc].లోతు))
            {
                prog[n-1].params[2] += gamma;
                తిరిగి acc;
            }
        }
    }

    స్థిరరాశి పూర్ణాంకం nd0 = isFlexConst(*ఇది, a) ? EW_లోతు_NONE : arginfo[a].లోతు;
    స్థిరరాశి పూర్ణాంకం nd1 = isFlexConst(*ఇది, b) ? EW_లోతు_NONE : arginfo[b].లోతు;
    స్థిరరాశి ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    పూర్ణాంకం ఫలితం = rdepth;
    ఒకవేళ (ఫలితం == EW_లోతు_NONE)
    {
        ఫలితం = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        ఒకవేళ (ఫలితం == EW_లోతు_NONE) ఫలితం = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    పూర్ణాంకం పునాది;
    ఒకవేళ (nd0 == EW_లోతు_NONE && nd1 == EW_లోతు_NONE) పునాది = ఫలితం;
    లేకపోతే ఒకవేళ (nd0 == EW_లోతు_NONE) పునాది = nd1;
    లేకపోతే ఒకవేళ (nd1 == EW_లోతు_NONE) పునాది = nd0;
    లేకపోతే పునాది = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    ఒకవేళ (cat == CAT_COMPARE && nd0 != EW_లోతు_NONE && !is_Float_లోతు(nd0) &&
        isFlexConst(*ఇది, b) && !const_సరిపోతుంది(*ఇది, b, పునాది))
    {
        ద్విగుణ lo, hi; intRange(nd0, lo, hi);
        ఒకవేళ (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            గణన { REAL, CFALSE, CTRUE };
            Auto_బఫరు<ద్విగుణ, MAX_LOCAL_CN> tv; పూర్ణాంకం cn = constDoubles(*ఇది, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            పూర్ణాంకం రకం[4] = {}; ద్విగుణ bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            స్థిరరాశి శిఖరం fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
            {
                స్థిరరాశి ద్విగుణ t = tv[c]; పూర్ణాంకం k; ద్విగుణ B = t;
                ఎంపిక (op)
                {
                స్థితి OP_CMP_GT: B = std::floor(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ఆపు;
                స్థితి OP_CMP_GE: B = std::ceil(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; ఆపు;
                స్థితి OP_CMP_LT: B = std::ceil(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ఆపు;
                స్థితి OP_CMP_LE: B = std::floor(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; ఆపు;
                స్థితి OP_CMP_EQ: k = లోతు_Represents(t, nd0) ? REAL : CFALSE; ఆపు;   // a==non-rep -> false
                అప్రమేయ:        k = లోతు_Represents(t, nd0) ? REAL : CTRUE;  ఆపు;   // NE: a!=non-rep -> true
                }
                రకం[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            స్వయంచాలక ucOp  = [&](పూర్ణాంకం c){ తిరిగి రకం[c]==REAL ? fam : రకం[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            స్వయంచాలక ucThr = [&](పూర్ణాంకం c){ తిరిగి రకం[c]==REAL ? bound[c]
                                          : (రకం[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            స్థిరరాశి శిఖరం u0 = ucOp(0);
            తార్కిక ఏకరీతి = నిజం;
            కొరకు (పూర్ణాంకం c = 1; c < cn && ఏకరీతి; c++) ఏకరీతి = (ucOp(c) == u0);
            ఒకవేళ (ఏకరీతి)
            {
                Scalar s; కొరకు (పూర్ణాంకం c = 0; c < cn; c++) s[c] = ucThr(c);
                b = కూడు_Const(EW_లోతు_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            లేకపోతే
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; పూర్ణాంకం patchFlags = 0;
                కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                {
                    ts[c] = రకం[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    స్థిరరాశి పూర్ణాంకం mbits = రకం[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    స్థిరరాశి పూర్ణాంకం vbits = రకం[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_జరుపు + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, పునాది, పునాది, EW_లోతు_NONE, ఫలితం);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                స్థిరరాశి పూర్ణాంకం out = కూడు_తాత్కాలిక(ఫలితం);
                కూడు_Insn(fam, a, కూడు_Const(పునాది, ts, cn), 0, out, kern);
                తిరిగి out;
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
    పూర్ణాంకం లోతు, wide;
    తార్కిక divGuard = అబద్ధం;
    ఎంపిక (op)
    {
    స్థితి OP_MIN: స్థితి OP_MAX:
    స్థితి OP_AND: స్థితి OP_OR: స్థితి OP_XOR:
        లోతు = ఫలితం; wide = ఫలితం; ఆపు;
    స్థితి OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        పూర్ణాంకం common = (nd0 == EW_లోతు_NONE) ? nd1
                   : (nd1 == EW_లోతు_NONE) ? nd0 : promoteArith(nd0, nd1);
        ఒకవేళ (common == EW_లోతు_NONE) common = ఫలితం;          // both operands flexible consts
        లోతు = common;
        wide  = absdiff_ఫలితం_లోతు(common);
        ఒకవేళ (rdepth == EW_లోతు_NONE) ఫలితం = wide;            // auto result: unsigned same width
        ఆపు;
    }
    స్థితి OP_CMP_EQ: స్థితి OP_CMP_NE: స్థితి OP_CMP_LT:
    స్థితి OP_CMP_LE: స్థితి OP_CMP_GT: స్థితి OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        లోతు = పునాది; wide = CV_64F; ఆపు;
    స్థితి OP_MUL: స్థితి OP_DIV:
    {
        లోతు = పునాది;
        స్థిరరాశి తార్కిక w = (లోతు==CV_32U || లోతు==CV_32S || లోతు==CV_64U ||
                        లోతు==CV_64S || లోతు==CV_64F);
        wide = w ? CV_64F : CV_32F;
        ఒకవేళ (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            స్వయంచాలక int_నిర్వాహ్యం = [&](పూర్ణాంకం s, పూర్ణాంకం nd) {
                ఒకవేళ (nd != EW_లోతు_NONE) తిరిగి !is_Float_లోతు(nd);
                Auto_బఫరు<ద్విగుణ, MAX_LOCAL_CN> v; constDoubles(*ఇది, s, v);
                తిరిగి v[0] == std::floor(v[0]); };
            divGuard = int_నిర్వాహ్యం(a, nd0) && int_నిర్వాహ్యం(b, nd1);
        }
        ఆపు;
    }
    స్థితి OP_POW: స్థితి OP_HYPOT: స్థితి OP_ATAN2:
        లోతు = (పునాది == CV_64F) ? CV_64F : CV_32F; wide = లోతు; ఆపు;
    అప్రమేయ:        // OP_ADD, OP_SUB
        లోతు = పునాది; wide = సురక్షిత_Wide(లోతు); ఆపు;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    స్వయంచాలక fracConst = [&](పూర్ణాంకం s) {
        ఒకవేళ (!isFlexConst(*ఇది, s)) తిరిగి అబద్ధం;
        Auto_బఫరు<ద్విగుణ, MAX_LOCAL_CN> v; పూర్ణాంకం n = constDoubles(*ఇది, s, v);
        కొరకు (పూర్ణాంకం ch = 0; ch < n; ch++)
            ఒకవేళ (v[ch] != std::floor(v[ch])) తిరిగి నిజం;
        తిరిగి అబద్ధం;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    ఒకవేళ (cat != CAT_BITWISE && (!const_సరిపోతుంది(*ఇది, a, లోతు) || !const_సరిపోతుంది(*ఇది, b, లోతు)))
    {
        లోతు = wide;
        స్థిరరాశి తార్కిక frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        ఒకవేళ (frac && !is_Float_లోతు(లోతు) && op != OP_MIN && op != OP_MAX)
            లోతు = wide = (ఫలితం == CV_64F || పునాది == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        ఒకవేళ (frac && (op == OP_MUL || op == OP_DIV) && లోతు == CV_32F)
            లోతు = wide = CV_64F;
    }

    పూర్ణాంకం s0 = isFlexConst(*ఇది, a) ? typedConstFrom(a, లోతు) : a;
    పూర్ణాంకం s1 = isFlexConst(*ఇది, b) ? typedConstFrom(b, లోతు) : b;
    పూర్ణాంకం c0 = maybe_కూడు_Cast(s0, లోతు), c1 = maybe_కూడు_Cast(s1, లోతు);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    స్వయంచాలక resolve = [&](పూర్ణాంకం rd) {
        తిరిగి op == OP_DIV ? getDivFunc(లోతు, rd, divGuard)
                            : getElemwiseFunc(op, లోతు, లోతు, EW_లోతు_NONE, rd);
    };
    TKernel k = resolve(ఫలితం);
    ఒకవేళ (k.fptr)
    {
        పూర్ణాంకం res = కూడు_తాత్కాలిక(ఫలితం);
        కూడు_Insn(op, c0, c1, 0, res, k, params);
        తిరిగి res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    పూర్ణాంకం w = కూడు_తాత్కాలిక(wide);
    కూడు_Insn(op, c0, c1, 0, w, k, params);
    తిరిగి maybe_కూడు_Cast(w, ఫలితం);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
పూర్ణాంకం TExpr::emitUnary(శిఖరం op, పూర్ణాంకం a, పూర్ణాంకం rdepth, స్థిరరాశి Scalar& params)
{
    స్థిరరాశి పూర్ణాంకం nd = isFlexConst(*ఇది, a) ? EW_లోతు_NONE : arginfo[a].లోతు;
    స్థిరరాశి ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    ఒకవేళ (op == OP_NEG)
        తిరిగి emit_ద్విమాన(OP_SUB, కూడు_Const(EW_లోతు_NONE, Scalar(0.), 1), a, rdepth, params);
    ఒకవేళ (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        ఒకవేళ (!prog.empty() && arginfo[a].రకం == తాత్కాలిక && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().ఫలితం == a &&
            arginfo[a].సూచీ == ntemps - 1)
        {
            స్థిరరాశి పూర్ణాంకం x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].రకం = NONE;
            ntemps--;
            తిరిగి emit_ద్విమాన(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        తిరిగి emit_ద్విమాన(OP_ABSDIFF, a, కూడు_Const(EW_లోతు_NONE, Scalar(0.), 1), rdepth, params);
    }

    పూర్ణాంకం ఫలితం = rdepth;
    ఒకవేళ (ఫలితం == EW_లోతు_NONE)
    {
        ఎంపిక (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        స్థితి CAT_MATH:    ఫలితం = is_Float_లోతు(nd) ? nd : CV_32F; ఆపు;
        స్థితి CAT_BITWISE: ఫలితం = (nd == EW_లోతు_NONE) ? CV_32S : nd; ఆపు;  // NOT
        అప్రమేయ:          ఫలితం = (nd == EW_లోతు_NONE) ? CV_32F : nd; ఆపు;  // NEG, ABS
        }
    }

    పూర్ణాంకం లోతు, wide;
    ఎంపిక (cat)
    {
    స్థితి CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        లోతు = (is_Float_లోతు(nd) && nd == ఫలితం) ? nd
              : (nd == CV_64F || ఫలితం == CV_64F) ? CV_64F : CV_32F;
        wide = లోతు;
        ఆపు;
    స్థితి CAT_BITWISE: లోతు = ఫలితం; wide = ఫలితం; ఆపు;
    అప్రమేయ:          లోతు = (nd == EW_లోతు_NONE) ? ఫలితం : nd; wide = ఫలితం; ఆపు;  // NEG/ABS
    }

    ఒకవేళ (!const_సరిపోతుంది(*ఇది, a, లోతు)) లోతు = (cat == CAT_MATH) ? లోతు : CV_32F;
    పూర్ణాంకం s0 = isFlexConst(*ఇది, a) ? typedConstFrom(a, లోతు) : a;
    పూర్ణాంకం c0 = maybe_కూడు_Cast(s0, లోతు);

    TKernel k = getElemwiseFunc(op, లోతు, EW_లోతు_NONE, EW_లోతు_NONE, ఫలితం);
    ఒకవేళ (k.fptr)
    {
        పూర్ణాంకం res = కూడు_తాత్కాలిక(ఫలితం);
        కూడు_Insn(op, c0, 0, 0, res, k, params);
        తిరిగి res;
    }
    k = getElemwiseFunc(op, లోతు, EW_లోతు_NONE, EW_లోతు_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    పూర్ణాంకం w = కూడు_తాత్కాలిక(wide);
    కూడు_Insn(op, c0, 0, 0, w, k, params);
    తిరిగి maybe_కూడు_Cast(w, ఫలితం);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
పూర్ణాంకం TExpr::emitTernary(శిఖరం op, పూర్ణాంకం a, పూర్ణాంకం b, పూర్ణాంకం c, పూర్ణాంకం rdepth)
{
    స్వయంచాలక dep = [&](పూర్ణాంకం s) { తిరిగి isFlexConst(*ఇది, s) ? EW_లోతు_NONE : arginfo[s].లోతు; };

    ఒకవేళ (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        ఒకవేళ (isFlexConst(*ఇది, a) || CV_ELEM_SIZE1(arginfo[a].లోతు) != 1)
            a = emit_ద్విమాన(OP_CMP_NE, a, కూడు_Const(EW_లోతు_NONE, Scalar(0.), 1), CV_8U, Scalar());
        పూర్ణాంకం ఫలితం = rdepth != EW_లోతు_NONE ? rdepth : promoteArith(dep(b), dep(c));
        ఒకవేళ (ఫలితం == EW_లోతు_NONE) ఫలితం = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        పూర్ణాంకం sb = isFlexConst(*ఇది, b) ? typedConstFrom(b, ఫలితం) : maybe_కూడు_Cast(b, ఫలితం);
        పూర్ణాంకం sc = isFlexConst(*ఇది, c) ? typedConstFrom(c, ఫలితం) : maybe_కూడు_Cast(c, ఫలితం);
        పూర్ణాంకం res = కూడు_తాత్కాలిక(ఫలితం);
        కూడు_Insn(OP_SELECT, a, sb, sc, res);
        తిరిగి res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    పూర్ణాంకం ఫలితం = rdepth != EW_లోతు_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    ఒకవేళ (ఫలితం == EW_లోతు_NONE) ఫలితం = CV_32F;
    స్వయంచాలక typed = [&](పూర్ణాంకం s) {
        తిరిగి isFlexConst(*ఇది, s) ? typedConstFrom(s, ఫలితం) : maybe_కూడు_Cast(s, ఫలితం);
    };
    పూర్ణాంకం c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, ఫలితం, ఫలితం, ఫలితం, ఫలితం);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    పూర్ణాంకం res = కూడు_తాత్కాలిక(ఫలితం);
    కూడు_Insn(op, c0, c1, c2, res, k);
    తిరిగి res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
పూర్ణాంకం TExpr::move_గమ్యం_Output(పూర్ణాంకం తాత్కాలిక_2, పూర్ణాంకం out)
{
    స్థిరరాశి పూర్ణాంకం ninsn = (పూర్ణాంకం)prog.size();
    పూర్ణాంకం ఉత్పాదకుడు = -1;
    తార్కిక వాడిన_As_Arg = అబద్ధం;
    కొరకు (పూర్ణాంకం i = 0; i < ninsn; i++)
    {
        స్థిరరాశి TExpr::Insn& ins = prog[i];
        ఒకవేళ (ins.ఫలితం == తాత్కాలిక_2) ఉత్పాదకుడు = i;
        ఒకవేళ (ins.arg0 == తాత్కాలిక_2 || ins.arg1 == తాత్కాలిక_2 || ins.arg2 == తాత్కాలిక_2) వాడిన_As_Arg = నిజం;
    }
    ఒకవేళ (arginfo[తాత్కాలిక_2].రకం == తాత్కాలిక && ఉత్పాదకుడు >= 0 && !వాడిన_As_Arg && !arginfo[తాత్కాలిక_2].pinned &&
        arginfo[తాత్కాలిక_2].లోతు == arginfo[out].లోతు)
    {
        // MOVE semantics: redirect `temp`'s single producer to write `out` directly, then leave the
        // source slot EMPTY - reclassify it to NONE. A NONE slot gets no physical buffer (compile's
        // liveness only walks TEMP slots) and is skipped everywhere in exec, so no stale dead-TEMP slot
        // remains (which, buffer or not, would still cost per-tile setup in the general path) - and no
        // slot removal / index renumbering is needed. This keeps single-op programs at zero temps =>
        // compile()'s no-temp early-out (zero heap traffic) still fires. Every caller moves the result
        // it JUST emitted, so `temp` is always the last-created temp - decrement ntemps to retire its
        // index and keep the temp indices dense (0..ntemps-1) for compile()'s liveness arrays.
        CV_Assert(arginfo[తాత్కాలిక_2].సూచీ == ntemps - 1);
        prog[ఉత్పాదకుడు].ఫలితం = out;
        arginfo[తాత్కాలిక_2].రకం = NONE;
        ntemps--;
        తిరిగి out;
    }
    కూడు_Insn(OP_CAST, తాత్కాలిక_2, 0, 0, out);                     // same/different depth copy into `out`
    తిరిగి out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
పూర్ణాంకం TExpr::output(పూర్ణాంకం మూలం_స్థానం)
{
    తిరిగి move_గమ్యం_Output(మూలం_స్థానం, కూడు_Output(arginfo[మూలం_స్థానం].లోతు));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
శూన్య TExpr::compile()
{
    స్థిరరాశి పూర్ణాంకం ninsn = (పూర్ణాంకం)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    స్థిరరాశి పూర్ణాంకం nslots = (పూర్ణాంకం)arginfo.size();
    nconsts = 0;
    కొరకు (పూర్ణాంకం s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        ఒకవేళ (a.రకం != CONST || a.లోతు == EW_లోతు_NONE) కొనసాగు;
        స్థిరరాశి పూర్ణాంకం cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.లోతు;
        స్థిరరాశి size_t sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        Auto_బఫరు<uint64_t, పరిమాణం(uint64_t)> srcbuf(div_పైకి((size_t)cn * sesz, (గుర్తులేని)పరిమాణం(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.దత్తాంశం();
        జ్ఞాపకం_నకలు(srcbytes, (స్థిరరాశి uchar*)(constbuf.దత్తాంశం() + a.constofs), (size_t)cn * sesz);
        స్థిరరాశి size_t ofs = appendConstBuf(constbuf, dd, శూన్యసూచిక, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.దత్తాంశం() + ofs);
        ఒకవేళ (sd == dd) జ్ఞాపకం_నకలు(dst, srcbytes, (size_t)cn * desz);
        లేకపోతే getConvertFunc(sd, dd)(srcbytes, 0, శూన్యసూచిక, 0, dst, 0, కొలత(cn, 1), శూన్యసూచిక);
        a.constofs = ofs; a.srcdepth = dd;
        nconsts++;
    }

    // ---- liveness: pack temps into a minimal set of reusable physical buffers. A program with no
    //      temps (single-op add/sub/mul/...) needs none of this - early out with ZERO heap traffic,
    //      so building such a program (the dominant cv::add-style call) allocates nothing. The temp
    //      case uses stack-backed AutoBuffers (inline for typical small expressions). ----
    nbuffers = 0;
    buf_Esz_ఉపసర్గ.resize(1); buf_Esz_ఉపసర్గ[0] = 0;
    capElems = గరిష్ఠ_పూర్ణాంకం;                       // no temps => exec runs each tile in one block, no scratch
    ఒకవేళ (ntemps == 0) తిరిగి;

    Auto_బఫరు<పూర్ణాంకం, 32> తాత్కాలిక_యొక్క_స్థానం(nslots);
    కొరకు (పూర్ణాంకం s = 0; s < nslots; s++) తాత్కాలిక_యొక్క_స్థానం[s] = -1;
    కొరకు (పూర్ణాంకం s = 1; s < nslots; s++)
        ఒకవేళ (arginfo[s].రకం == తాత్కాలిక) తాత్కాలిక_యొక్క_స్థానం[s] = arginfo[s].సూచీ;

    Auto_బఫరు<పూర్ణాంకం, 16> చివరి_Use(ntemps);
    కొరకు (పూర్ణాంకం t = 0; t < ntemps; t++) చివరి_Use[t] = -1;
    కొరకు (పూర్ణాంకం i = 0; i < ninsn; i++)
    {
        స్థిరరాశి TExpr::Insn& ins = prog[i];
        పూర్ణాంకం as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        కొరకు (పూర్ణాంకం k = 0; k < 3; k++)
            ఒకవేళ (తాత్కాలిక_యొక్క_స్థానం[as[k]] >= 0) చివరి_Use[తాత్కాలిక_యొక్క_స్థానం[as[k]]] = i;
    }

    బఫరు_యొక్క_తాత్కాలిక.resize(ntemps);
    కొరకు (పూర్ణాంకం t = 0; t < ntemps; t++) బఫరు_యొక్క_తాత్కాలిక[t] = -1;
    Auto_బఫరు<పూర్ణాంకం, 16> freeBufs(ntemps);
    పూర్ణాంకం nfree = 0, nbuf = 0;
    కొరకు (పూర్ణాంకం i = 0; i < ninsn; i++)
    {
        స్థిరరాశి TExpr::Insn& ins = prog[i];
        పూర్ణాంకం rt = తాత్కాలిక_యొక్క_స్థానం[ins.ఫలితం];
        ఒకవేళ (rt >= 0 && బఫరు_యొక్క_తాత్కాలిక[rt] < 0)
            బఫరు_యొక్క_తాత్కాలిక[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        పూర్ణాంకం as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        కొరకు (పూర్ణాంకం k = 0; k < 3; k++)
        {
            పూర్ణాంకం t = తాత్కాలిక_యొక్క_స్థానం[as[k]];
            ఒకవేళ (t >= 0 && చివరి_Use[t] == i)
            {
                freeBufs[nfree++] = బఫరు_యొక్క_తాత్కాలిక[t];
                చివరి_Use[t] = -1;   // the same temp may be several args of this insn ("d*d") -
                                   // release its buffer once, or freeBufs overflows/double-frees
            }
        }
    }

    nbuffers = nbuf;

    // Byte layout of the physical temp buffers, per output element: prefix sums of each buffer's
    // max element size. bufEszPrefix[b]*region = buffer b's byte offset in the scratch; [nbuffers] =
    // total temp bytes/element. Precomputed here (depths are build-time) so exec()/its fast path
    // never recompute it per call, and the fast path can size its scratch in O(1).
    buf_Esz_ఉపసర్గ.resize(nbuffers + 1);
    కొరకు (పూర్ణాంకం b = 0; b <= nbuffers; b++) buf_Esz_ఉపసర్గ[b] = 0;
    కొరకు (పూర్ణాంకం s = 1; s < nslots; s++)
        ఒకవేళ (arginfo[s].రకం == తాత్కాలిక)
        {
            పూర్ణాంకం b = బఫరు_యొక్క_తాత్కాలిక[arginfo[s].సూచీ], e = (పూర్ణాంకం)CV_ELEM_SIZE1(arginfo[s].లోతు);
            ఒకవేళ (e > buf_Esz_ఉపసర్గ[b + 1]) buf_Esz_ఉపసర్గ[b + 1] = e;     // max elem size in buffer b
        }
    కొరకు (పూర్ణాంకం b = 0; b < nbuffers; b++) buf_Esz_ఉపసర్గ[b + 1] += buf_Esz_ఉపసర్గ[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    స్థిరరాశి పూర్ణాంకం మొత్తం_Esz = buf_Esz_ఉపసర్గ[nbuffers];
    capElems = మొత్తం_Esz > 0 ? std::max(64, (4 * 1024) / మొత్తం_Esz) : గరిష్ఠ_పూర్ణాంకం;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
స్థిర శూన్య matLogical(స్థిరరాశి Mat& m, MatShape& shp, Ew_అడుగులు& అడుగు, పూర్ణాంకం& esz1)
{
    esz1 = (పూర్ణాంకం)m.elemSize1();
    పూర్ణాంకం nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    కొరకు (పూర్ణాంకం i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        అడుగు[i] = m.అడుగు[i] / esz1;
    }
    shp[nd] = cn;
    అడుగు[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
స్థిర తార్కిక ప్రసారం_Shape(స్థిరరాశి MatShape* shps, పూర్ణాంకం K, MatShape& out)
{
    పూర్ణాంకం nd = 0;
    కొరకు (పూర్ణాంకం k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
    {
        స్థిరరాశి MatShape& s = shps[k];
        పూర్ణాంకం off = nd - s.dims;
        కొరకు (పూర్ణాంకం i = 0; i < s.dims; i++)
        {
            పూర్ణాంకం d = s[i], &o = out[off + i];
            ఒకవేళ (o == 1) o = d;
            లేకపోతే ఒకవేళ (d != 1 && d != o) తిరిగి అబద్ధం;
        }
    }
    తిరిగి నిజం;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
స్థిర పూర్ణాంకం op_ఖర్చు(శిఖరం op)
{
    ఎంపిక (op)
    {
    స్థితి OP_కూడు: స్థితి OP_SUB: స్థితి OP_MUL: స్థితి OP_MIN: స్థితి OP_MAX:
    స్థితి OP_ABSDIFF: స్థితి OP_AND: స్థితి OP_OR: స్థితి OP_XOR: స్థితి OP_NOT:
    స్థితి OP_NEG: స్థితి OP_ABS: స్థితి OP_CAST: స్థితి OP_RELU: స్థితి OP_SELECT:
    స్థితి OP_CMP_EQ: స్థితి OP_CMP_NE: స్థితి OP_CMP_LT:
    స్థితి OP_CMP_LE: స్థితి OP_CMP_GT: స్థితి OP_CMP_GE: తిరిగి 1;
    స్థితి OP_DIV: స్థితి OP_SQRT: స్థితి OP_HYPOT: స్థితి OP_CONVERT_SCALE: తిరిగి 3;
    స్థితి OP_SIN: స్థితి OP_COS: స్థితి OP_TANH: స్థితి OP_ERF: స్థితి OP_ATAN2:
    స్థితి OP_EXP: స్థితి OP_LOG: స్థితి OP_POW:           తిరిగి 6;
    అప్రమేయ:                                         తిరిగి 3;
    }
}

TExpr::TExpr()
{
    clear();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
శూన్య TExpr::clear()
{
    prog.allocate(0);
    arginfo.allocate(0);
    బఫరు_యొక్క_తాత్కాలిక.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
శూన్య TExpr::dump(std::ostream& os) స్థిరరాశి
{
    స్వయంచాలక dn = [](పూర్ణాంకం d) -> స్థిరరాశి అక్షరం* {
        ఎంపిక (d) {
        స్థితి EW_లోతు_NONE: తిరిగి "flex";
        స్థితి CV_8U:  తిరిగి "u8";  స్థితి CV_8S:  తిరిగి "s8";
        స్థితి CV_16U: తిరిగి "u16"; స్థితి CV_16S: తిరిగి "s16";
        స్థితి CV_32U: తిరిగి "u32"; స్థితి CV_32S: తిరిగి "s32";
        స్థితి CV_64U: తిరిగి "u64"; స్థితి CV_64S: తిరిగి "s64";
        స్థితి CV_16F: తిరిగి "f16"; స్థితి CV_16BF:తిరిగి "bf16";
        స్థితి CV_32F: తిరిగి "f32"; స్థితి CV_64F: తిరిగి "f64";
        స్థితి CV_Bool:తిరిగి "bool";
        అప్రమేయ:     తిరిగి "?"; }
    };
    స్వయంచాలక kn = [](Arg_రకం k) -> స్థిరరాశి అక్షరం* {
        ఎంపిక (k) { స్థితి NONE: తిరిగి "none"; స్థితి ఆదానం: తిరిగి "in"; స్థితి CONST: తిరిగి "const";
                     స్థితి తాత్కాలిక: తిరిగి "temp"; స్థితి OUTPUT: తిరిగి "out"; } తిరిగి "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (పూర్ణాంకం)prog.size() << "\n";
    os << "  slots:\n";
    కొరకు (పూర్ణాంకం s = 0; s < (పూర్ణాంకం)arginfo.size(); s++) {
        స్థిరరాశి Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.రకం);
        ఒకవేళ (a.రకం != NONE) os << " " << dn(a.లోతు);
        ఒకవేళ (a.రకం == ఆదానం || a.రకం == OUTPUT || a.రకం == తాత్కాలిక) os << " idx=" << a.సూచీ;
        ఒకవేళ (a.రకం == CONST) {
            స్థిరరాశి పూర్ణాంకం cn = std::max(1, a.channels);
            Auto_బఫరు<ద్విగుణ, MAX_LOCAL_CN> v(cn);
            స్థిరరాశి uchar* src = (స్థిరరాశి uchar*)(constbuf.దత్తాంశం() + a.constofs);
            ఒకవేళ (a.srcdepth == CV_64F) జ్ఞాపకం_నకలు(v.దత్తాంశం(), src, (size_t)cn * పరిమాణం(ద్విగుణ));
            లేకపోతే getConvertFunc(a.srcdepth, CV_64F)(src, 0, శూన్యసూచిక, 0, (uchar*)v.దత్తాంశం(), 0, కొలత(cn, 1), శూన్యసూచిక);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            కొరకు (పూర్ణాంకం c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)prog.size(); i++) {
        స్థిరరాశి Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        ఒకవేళ (ins.arg1) os << ", " << ins.arg1;
        ఒకవేళ (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.ఫలితం;
        ఒకవేళ (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        ఒకవేళ (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        ఒకవేళ (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
పూర్ణాంకం TExpr::కూడు_ఆదానం(పూర్ణాంకం లోతు)
{
    Arg a; a.రకం = ఆదానం; a.లోతు = లోతు; a.సూచీ = ninputs++;
    arginfo.push_back(a); తిరిగి (పూర్ణాంకం)arginfo.size() - 1;
}

పూర్ణాంకం TExpr::కూడు_Output(పూర్ణాంకం లోతు)
{
    Arg a; a.రకం = OUTPUT; a.లోతు = లోతు; a.సూచీ = noutputs++;
    arginfo.push_back(a); తిరిగి (పూర్ణాంకం)arginfo.size() - 1;
}

పూర్ణాంకం TExpr::కూడు_తాత్కాలిక(పూర్ణాంకం లోతు)
{
    Arg a; a.రకం = తాత్కాలిక; a.లోతు = లోతు; a.సూచీ = ntemps++;
    arginfo.push_back(a); తిరిగి (పూర్ణాంకం)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
పూర్ణాంకం TExpr::కూడు_Const(పూర్ణాంకం లోతు, స్థిరరాశి Scalar& v, పూర్ణాంకం channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.రకం = CONST; a.లోతు = లోతు; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); తిరిగి (పూర్ణాంకం)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
పూర్ణాంకం TExpr::కూడు_Const(పూర్ణాంకం లోతు, పూర్ణాంకం srcdepth, స్థిరరాశి శూన్య* దత్తాంశం, పూర్ణాంకం channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.రకం = CONST; a.లోతు = లోతు; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, దత్తాంశం, channels);
    arginfo.push_back(a); తిరిగి (పూర్ణాంకం)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
పూర్ణాంకం TExpr::typedConstFrom(పూర్ణాంకం src_స్థానం, పూర్ణాంకం లోతు)
{
    స్థిరరాశి Arg& src = arginfo[src_స్థానం];
    Arg a; a.రకం = CONST; a.లోతు = లోతు; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); తిరిగి (పూర్ణాంకం)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
పూర్ణాంకం TExpr::కూడు_Insn(శిఖరం op, పూర్ణాంకం a0, పూర్ణాంకం a1, పూర్ణాంకం a2, పూర్ణాంకం ఫలితం, స్థిరరాశి TKernel& kernel, స్థిరరాశి Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.ఫలితం = ఫలితం;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    తిరిగి (పూర్ణాంకం)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
పూర్ణాంకం TExpr::కూడు_Insn(శిఖరం op, పూర్ణాంకం a0, పూర్ణాంకం a1, పూర్ణాంకం a2, పూర్ణాంకం ఫలితం, స్థిరరాశి Scalar& params)
{
    పూర్ణాంకం d0 = a0 ? arginfo[a0].లోతు : EW_లోతు_NONE;
    పూర్ణాంకం d1 = a1 ? arginfo[a1].లోతు : EW_లోతు_NONE;
    పూర్ణాంకం d2 = a2 ? arginfo[a2].లోతు : EW_లోతు_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[ఫలితం].లోతు);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    తిరిగి కూడు_Insn(op, a0, a1, a2, ఫలితం, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
పూర్ణాంకం TExpr::maybe_కూడు_Cast(పూర్ణాంకం arg, పూర్ణాంకం లోతు)
{
    ఒకవేళ (arginfo[arg].లోతు == లోతు) తిరిగి arg;
    పూర్ణాంకం t = కూడు_తాత్కాలిక(లోతు);
    కూడు_Insn(OP_CAST, arg, 0, 0, t);
    తిరిగి t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
స్థిర అంతఃవరుస శూన్య నిరంతరం_Insn(స్థిరరాశి TExpr::Insn& ins,
                           స్థిరరాశి శూన్య* p0, size_t y0, size_t x0,
                           స్థిరరాశి శూన్య* p1, size_t y1, size_t x1,
                           స్థిరరాశి శూన్య* p2, size_t y2, size_t x2,
                           శూన్య* pr, size_t yr, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం సంకేతం_2 = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(సంకేతం_2 >= 0);
}

నిర్మాణం EwBody {
    స్థిరరాశి TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    స్థిరరాశి పూర్ణాంకం* స్థానం_Map;
    స్థిరరాశి గుర్తుగల అక్షరం* స్థానం_రకం; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    స్థిరరాశి పూర్ణాంకం* buf_Esz_ఉపసర్గ;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    పూర్ణాంకం ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

శూన్య TExpr::outputShape(స్థిరరాశి Mat* స్థిరరాశి* inputs, MatShape& spatial, పూర్ణాంకం& channels) స్థిరరాశి
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    స్థిరరాశి పూర్ణాంకం rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    తార్కిక sameShape = ninputs >= 1;
    కొరకు (పూర్ణాంకం i = 1; sameShape && i < ninputs; i++)
    {
        స్థిరరాశి Mat& a = *inputs[i];
        ఒకవేళ (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = అబద్ధం;
        లేకపోతే కొరకు (పూర్ణాంకం d = 0; d < a.dims; d++) ఒకవేళ (a.size[d] != inputs[0]->size[d]) { sameShape = అబద్ధం; ఆపు; }
    }
    ఒకవేళ (sameShape) { spatial = inputs[0]->size; channels = rcn0; తిరిగి; }

    Auto_బఫరు<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; Ew_అడుగులు అడుగు; పూర్ణాంకం esz1;
    కొరకు (పూర్ణాంకం i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, అడుగు, esz1); bshapes[i] = shp; }
    MatShape నిండిన;
    CV_Assert(ప్రసారం_Shape(bshapes.దత్తాంశం(), ninputs, నిండిన) && "ew: inputs not broadcast-compatible");
    స్థిరరాశి పూర్ణాంకం nd_నిండిన = (పూర్ణాంకం)నిండిన.size();
    channels = నిండిన[nd_నిండిన - 1];
    spatial = నిండిన; spatial.resize(nd_నిండిన - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
శూన్య TExpr::outputShape(స్థిరరాశి Mat* inputs, MatShape& spatial, పూర్ణాంకం& channels) స్థిరరాశి
{
    Auto_బఫరు<స్థిరరాశి Mat*, 16> ptrs(std::max(ninputs, 1));
    కొరకు (పూర్ణాంకం i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.దత్తాంశం(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
స్థిర constexpr పూర్ణాంకం LOCAL_HDRS = 3;
స్థిర constexpr పూర్ణాంకం LOCAL_CONSTS = 4;
స్థిర constexpr పూర్ణాంకం LOCAL_OPS = 16;

శూన్య TExpr::exec(స్థిరరాశి Mat* స్థిరరాశి* inputs, Mat* outputs)
{
    ఉపయోగించి BrTile = ప్రసారం_Op::Tile;
    ఉపయోగించి Br_ముక్క = ప్రసారం_Op::ముక్క;
    స్థిరరాశి పూర్ణాంకం nslots = (పూర్ణాంకం)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].రకం == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    ఒకవేళ (nconsts == 0 && ninputs >= 1)
    {
        constexpr size_t EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        స్థిరరాశి Mat& r = *inputs[0];
        స్థిరరాశి పూర్ణాంకం rcn = r.channels();
        తార్కిక ok = r.isContinuous();
        కొరకు (పూర్ణాంకం i = 1; ok && i < ninputs; i++)
        {
            స్థిరరాశి Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        కొరకు (పూర్ణాంకం s = 1; ok && s < nslots; s++)
            ఒకవేళ (arginfo[s].రకం == OUTPUT) ok = ok && outputs[arginfo[s].సూచీ].isContinuous();
        స్థిరరాశి size_t మొత్తం = ok ? r.మొత్తం() * rcn : 0;
        ఒకవేళ (ok && మొత్తం == 0)
        {
            స్థిరరాశి MatShape rshape = r.size;
            కొరకు (పూర్ణాంకం s = 1; s < nslots; s++)
                ఒకవేళ (arginfo[s].రకం == OUTPUT)
                    outputs[arginfo[s].సూచీ].create(rshape, CV_MAKETYPE(arginfo[s].లోతు, rcn));
            తిరిగి;
        }
        ఒకవేళ (ok && మొత్తం <= EW_FASTPATH_MAX)
        {
            స్థిరరాశి MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            కొరకు (పూర్ణాంకం s = 1; s < nslots; s++)
                ఒకవేళ (arginfo[s].రకం == OUTPUT)
                    outputs[arginfo[s].సూచీ].create(rshape, CV_MAKETYPE(arginfo[s].లోతు, rcn));
            స్థిరరాశి పూర్ణాంకం ninsn = (పూర్ణాంకం)prog.size();

            ఒకవేళ (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                స్వయంచాలక ptr_యొక్క = [&](పూర్ణాంకం s) -> శూన్య* {
                    ఒకవేళ (s <= 0) తిరిగి శూన్యసూచిక;
                    స్థిరరాశి Arg& ai = arginfo[s];
                    ఒకవేళ (ai.రకం == ఆదానం)  తిరిగి (శూన్య*)inputs[ai.సూచీ]->దత్తాంశం;
                    ఒకవేళ (ai.రకం == OUTPUT) తిరిగి (శూన్య*)outputs[ai.సూచీ].దత్తాంశం;
                    తిరిగి శూన్యసూచిక;   // NONE
                };
                స్థిరరాశి పూర్ణాంకం w = (పూర్ణాంకం)మొత్తం;
                కొరకు (పూర్ణాంకం n = 0; n < ninsn; n++)
                {
                    స్థిరరాశి Insn& ins = prog[n];
                    నిరంతరం_Insn(ins, ptr_యొక్క(ins.arg0), 0, 1, ptr_యొక్క(ins.arg1), 0, 1,
                            ptr_యొక్క(ins.arg2), 0, 1, ptr_యొక్క(ins.ఫలితం), 0, w, 1);
                }
                తిరిగి;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            స్థిరరాశి పూర్ణాంకం మొత్తం_Esz = buf_Esz_ఉపసర్గ[nbuffers];
            స్థిరరాశి పూర్ణాంకం wf0 = std::min((పూర్ణాంకం)మొత్తం, capElems);

            స్థిరరాశి size_t region = align_కొలత((size_t)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            Auto_బఫరు<uint64_t, (4*1024 + 256)/పరిమాణం(uint64_t)> scratchBuf(
                div_పైకి((size_t)మొత్తం_Esz * region, (గుర్తులేని)పరిమాణం(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.దత్తాంశం();
            కొరకు (పూర్ణాంకం x0 = 0; x0 < (పూర్ణాంకం)మొత్తం; x0 += wf0)
            {
                స్థిరరాశి పూర్ణాంకం wf = std::min(wf0, (పూర్ణాంకం)మొత్తం - x0);
                స్వయంచాలక ptr_యొక్క = [&](పూర్ణాంకం s) -> శూన్య* {
                    ఒకవేళ (s <= 0) తిరిగి శూన్యసూచిక;
                    స్థిరరాశి Arg& ai = arginfo[s];
                    size_t esz = CV_ELEM_SIZE1(ai.లోతు);
                    ఒకవేళ (ai.రకం == ఆదానం)  తిరిగి (uchar*)inputs[ai.సూచీ]->దత్తాంశం  + (size_t)x0 * esz;
                    ఒకవేళ (ai.రకం == OUTPUT) తిరిగి (uchar*)outputs[ai.సూచీ].దత్తాంశం + (size_t)x0 * esz;
                    ఒకవేళ (ai.రకం != తాత్కాలిక)   తిరిగి శూన్యసూచిక;                                    // NONE: moved-from
                    స్థిరరాశి పూర్ణాంకం b = బఫరు_యొక్క_తాత్కాలిక.empty() ? ai.సూచీ : బఫరు_యొక్క_తాత్కాలిక[ai.సూచీ];  //తాత్కాలిక
                    తిరిగి scratch + (size_t)buf_Esz_ఉపసర్గ[b] * region;                        // fragment-local
                };
                కొరకు (పూర్ణాంకం n = 0; n < ninsn; n++)
                {
                    స్థిరరాశి Insn& ins = prog[n];
                    నిరంతరం_Insn(ins, ptr_యొక్క(ins.arg0), 0, 1, ptr_యొక్క(ins.arg1), 0, 1,
                            ptr_యొక్క(ins.arg2), 0, 1, ptr_యొక్క(ins.ఫలితం), 0, wf, 1);
                }
            }
            తిరిగి;
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
    పూర్ణాంకం rchannels;
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
    తార్కిక inplace = అబద్ధం;
    కొరకు (పూర్ణాంకం i = 0; i < ninputs && !inplace; i++)
        కొరకు (పూర్ణాంకం j = 0; j < noutputs; j++)
            ఒకవేళ (inputs[i]->దత్తాంశం && inputs[i]->దత్తాంశం == outputs[j].దత్తాంశం) { inplace = నిజం; ఆపు; }
    పూర్ణాంకం nhdrs = inplace ? ninputs : 0;

    Auto_బఫరు<స్థిరరాశి Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    Auto_బఫరు<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    Auto_బఫరు<స్థిరరాశి Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    Auto_బఫరు<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    Auto_బఫరు<పూర్ణాంకం, LOCAL_OPS> స్థానం_Map(nslots);
    Auto_బఫరు<గుర్తుగల అక్షరం, LOCAL_OPS> స్థానం_రకం(nslots);
    పూర్ణాంకం narr = 0, nc = 0;
    ఒకవేళ (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        కొరకు (పూర్ణాంకం j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.దత్తాంశం();
    }
    స్థానం_రకం[0] = (గుర్తుగల అక్షరం)TExpr::NONE;
    కొరకు (పూర్ణాంకం s = 1; s < nslots; s++)
    {
        స్థిరరాశి TExpr::Arg& ai = arginfo[s];
        స్థానం_రకం[s] = (గుర్తుగల అక్షరం)ai.రకం;
        ఒకవేళ (ai.రకం == TExpr::ఆదానం) {
            స్థానం_Map[s] = narr;
            arr[narr++] = inputs[ai.సూచీ];
        }
        లేకపోతే ఒకవేళ (ai.రకం == TExpr::OUTPUT) {
            outputs[ai.సూచీ].create(spatial, CV_MAKETYPE(ai.లోతు, rchannels));
            స్థానం_Map[s] = narr; arr[narr++] = &outputs[ai.సూచీ];
        }
        లేకపోతే ఒకవేళ (ai.రకం == TExpr::CONST && ai.లోతు == EW_లోతు_NONE) {
            స్థానం_రకం[s] = (గుర్తుగల అక్షరం)TExpr::NONE;   // dead flexible literal: never referenced
            స్థానం_Map[s] = 0;
        }
        లేకపోతే ఒకవేళ (ai.రకం == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            స్థిరరాశి పూర్ణాంకం c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.లోతు, c),
                                  (శూన్య*)(constbuf.దత్తాంశం() + ai.constofs));
            స్థానం_రకం[s] = (గుర్తుగల అక్షరం)TExpr::ఆదానం;   // to the body it is just an array operand
            స్థానం_Map[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        లేకపోతే ఒకవేళ (ai.రకం == TExpr::తాత్కాలిక) {
            స్థానం_Map[s] = బఫరు_యొక్క_తాత్కాలిక.empty() ? ai.సూచీ : బఫరు_యొక్క_తాత్కాలిక[ai.సూచీ];  // physical buffer id
        }
        లేకపోతే {  // TExpr::NONE: a moved-from temp (or dead literal) - inert, no operand, no buffer. Never
                // referenced by any instruction; do NOT touch bufferOfTemp (its retired index may be >= ntemps).
            స్థానం_Map[s] = 0;
        }
    }

    // Temp-buffer byte layout (prefix sums, bufEszPrefix) and the L1 fragment cap (capElems) were
    // both precomputed by compile(); the body uses them directly (prefix[buf]*region = a buffer's
    // byte offset). capElems == INT_MAX when there are no temps => the body runs each tile in a
    // single block (no scratch).

    // ---- 3. ONE pass over instructions: summed per-element cost (kernels were bound at compile()). ----
    స్థిరరాశి పూర్ణాంకం ninsn = (పూర్ణాంకం)prog.size();
    దీర్ఘ దీర్ఘ ఖర్చు_చొప్పున_Elem = 0;
    కొరకు (పూర్ణాంకం n = 0; n < ninsn; n++)
        ఖర్చు_చొప్పున_Elem += op_ఖర్చు(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    దీర్ఘ దీర్ఘ otot = (దీర్ఘ దీర్ఘ)rchannels;
    కొరకు (పూర్ణాంకం d = 0; d < (పూర్ణాంకం)spatial.size(); d++) otot *= spatial[d];
    ఒకవేళ (otot == 0)
        తిరిగి;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    స్థిరరాశి ద్విగుణ T = (ద్విగుణ)std::max(get_అంకె_దారాలు(), 1);
    స్థిరరాశి ద్విగుణ nstripes = std::min(
        (ద్విగుణ)otot * (ద్విగుణ)std::max<దీర్ఘ దీర్ఘ>(ఖర్చు_చొప్పున_Elem, 1) * (1./ (ద్విగుణ)(1 << 16)),
        std::min(4.*T, std::max(32., 3.*T)));

    EwBody body;
    body.prog = prog.దత్తాంశం();
    body.ninsn = ninsn;
    body.స్థానం_Map = స్థానం_Map.దత్తాంశం();

    // ---- 5. drive: broadcastOp does geometry + 2D tiling + parallelism; the body runs the
    //         frozen program on each tile (temps tile-local, re-pointed from the tile slices).
    //         expandChannels=true => the body always sees single-channel data.
    //
    //         All state the body needs is packed into one POD (EwBody) so the lambda captures a
    //         SINGLE reference: the closure is then one pointer, fits std::function's small-buffer
    //         and never heap-allocates. Inside, the hot fields are copied into locals so the
    //         per-tile/per-insn loops read them from registers, not through the captured pointer. -

    body.buf_Esz_ఉపసర్గ = buf_Esz_ఉపసర్గ.దత్తాంశం();
    body.స్థానం_రకం = స్థానం_రకం.దత్తాంశం();
    body.nslots = nslots;
    body.nbuffers = nbuffers;
    body.capElems = capElems;

    ప్రసారం_Op_2(arr.దత్తాంశం(), narr, [&](స్థిరరాశి BrTile& tile)
    {
        EwBody& bc = body;
        స్థిరరాశి TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        స్థిరరాశి పూర్ణాంకం* స్థిరరాశి smap  = bc.స్థానం_Map;
        స్థిరరాశి గుర్తుగల అక్షరం* స్థిరరాశి skind = bc.స్థానం_రకం;
        స్థిరరాశి పూర్ణాంకం nin = bc.ninsn, nsl = bc.nslots;
        స్థిరరాశి పూర్ణాంకం w = tile.వెడల్పు, h = tile.ఎత్తు;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        స్థిరరాశి పూర్ణాంకం* స్థిరరాశి esz_ఉపసర్గ = bc.buf_Esz_ఉపసర్గ;
        స్థిరరాశి పూర్ణాంకం bw = std::min(w, bc.capElems);
        స్థిరరాశి పూర్ణాంకం bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        స్థిరరాశి size_t region = align_కొలత((size_t)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        Auto_బఫరు<uint64_t, (4*1024 + 256)/పరిమాణం(uint64_t)> tstoreBuf(
            div_పైకి((size_t)esz_ఉపసర్గ[bc.nbuffers] * region, (గుర్తులేని)పరిమాణం(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.దత్తాంశం();

        Auto_బఫరు<Br_ముక్క, LOCAL_OPS> వాదనలు(nsl);
        కొరకు (పూర్ణాంకం y0 = 0; y0 < h; y0 += bh)
        {
            స్థిరరాశి పూర్ణాంకం hf = std::min(bh, h - y0);
            కొరకు (పూర్ణాంకం x0 = 0; x0 < w; x0 += bw)
            {
                స్థిరరాశి పూర్ణాంకం wf = std::min(bw, w - x0);
                కొరకు (పూర్ణాంకం s = 1; s < nsl; s++)
                {
                    Br_ముక్క& a = వాదనలు[s];
                    స్థిరరాశి పూర్ణాంకం k = skind[s];
                    size_t esz = CV_ELEM_SIZE1(arginfo[s].లోతు);
                    ఒకవేళ (k == TExpr::తాత్కాలిక)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (size_t)esz_ఉపసర్గ[smap[s]] * region;
                        a.stepy = (size_t)wf*esz; a.stepx = 1;
                    }
                    లేకపోతే                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        స్థిరరాశి Br_ముక్క& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((size_t)y0 * sl.stepy + (size_t)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                కొరకు (పూర్ణాంకం n = 0; n < nin; n++)
                {
                    స్థిరరాశి TExpr::Insn& ins = insns[n];
                    స్థిరరాశి Br_ముక్క& a0 = వాదనలు[ins.arg0]; స్థిరరాశి Br_ముక్క& a1 = వాదనలు[ins.arg1];
                    స్థిరరాశి Br_ముక్క& a2 = వాదనలు[ins.arg2]; స్థిరరాశి Br_ముక్క& rr = వాదనలు[ins.ఫలితం];
                    నిరంతరం_Insn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (శూన్య*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, నిజం, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
శూన్య TExpr::exec(స్థిరరాశి Mat* inputs, Mat* outputs)
{
    Auto_బఫరు<స్థిరరాశి Mat*, 16> ptrs(std::max(ninputs, 1));
    కొరకు (పూర్ణాంకం i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    exec(ptrs.దత్తాంశం(), outputs);
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
శూన్య make_ద్విమాన_Arith_కార్యక్రమం(TExpr& p, శిఖరం op, పూర్ణాంకం depth0, పూర్ణాంకం depth1, పూర్ణాంకం rdepth,
                            పూర్ణాంకం ముసుగు_లోతు, ద్విగుణ scale)
{
    p.clear();
    ఒకవేళ (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        ఒకవేళ (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        లేకపోతే
        {
            rdepth = promoteArith(depth0, depth1);
            ఒకవేళ (op == OP_ABSDIFF) rdepth = absdiff_ఫలితం_లోతు(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    స్థిరరాశి తార్కిక masked = ముసుగు_లోతు != EW_లోతు_NONE;
    పూర్ణాంకం sIn0  = p.కూడు_ఆదానం(depth0);
    పూర్ణాంకం sIn1  = p.కూడు_ఆదానం(depth1);
    పూర్ణాంకం s_ముసుగు = masked ? p.కూడు_ఆదానం(ముసుగు_లోతు) : 0;
    పూర్ణాంకం s_బహిర్గతం  = p.కూడు_Output(rdepth);

    పూర్ణాంకం r = p.emit_ద్విమాన(op, sIn0, sIn1, rdepth, Scalar(scale));
    ఒకవేళ (masked)
        p.కూడు_Insn(OP_SELECT, s_ముసుగు, r, s_బహిర్గతం, s_బహిర్గతం);          // dst = mask ? r : dst
    లేకపోతే
        p.move_గమ్యం_Output(r, s_బహిర్గతం);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
శూన్య make_కూడు_Weighted_కార్యక్రమం(TExpr& p, పూర్ణాంకం depth0, పూర్ణాంకం depth1, పూర్ణాంకం rdepth,
                            ద్విగుణ alpha, ద్విగుణ beta, ద్విగుణ gamma)
{
    p.clear();
    పూర్ణాంకం sA   = p.కూడు_ఆదానం(depth0);
    పూర్ణాంకం sB   = p.కూడు_ఆదానం(depth1);
    పూర్ణాంకం s_బహిర్గతం = p.కూడు_Output(rdepth);
    పూర్ణాంకం r = p.emit_ద్విమాన(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.move_గమ్యం_Output(r, s_బహిర్గతం);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

నామాకాశ {

// --- tokens ------------------------------------------------------------------------------
గణన TokType { T_అంకె, T_ఆదానం, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_కేటాయించు, T_చివర };

నిర్మాణం Token
{
    TokType type = T_చివర;
    ద్విగుణ num = 0;
    పూర్ణాంకం ఆదానం_2 = 0;
    std::string పాఠ్యం;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
నిర్మాణం Lexer
{
    std::string_view s;
    size_t pos = 0;

    స్పష్టమైన Lexer(std::string_view src) : s(src) {}

    స్థిర తార్కిక is_Ident_ప్రారంభం(అక్షరం c) { తిరిగి std::అక్షరమా((గుర్తులేని అక్షరం)c) || c == '_'; }
    స్థిర తార్కిక isIdentChar(అక్షరం c)  { తిరిగి std::అక్షరమా_అంకెనా((గుర్తులేని అక్షరం)c) || c == '_'; }

    Token next()
    {
        అయితే (pos < s.size() && std::ఖాళీనా((గుర్తులేని అక్షరం)s[pos])) pos++;
        Token t;
        ఒకవేళ (pos >= s.size()) { t.type = T_చివర; తిరిగి t; }

        అక్షరం c = s[pos];

        ఒకవేళ (c == '{')                       // input placeholder {N}
        {
            pos++;
            size_t ప్రారంభం = pos;
            అయితే (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_ఆదానం;
            t.ఆదానం_2 = std::atoi(std::string(s.substr(ప్రారంభం, pos - ప్రారంభం)).c_str());
            pos++;                          // consume '}'
            తిరిగి t;
        }
        ఒకవేళ (std::అంకెనా((గుర్తులేని అక్షరం)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::అంకెనా((గుర్తులేని అక్షరం)s[pos + 1])))
        {
            అక్షరం* end = శూన్యసూచిక;
            std::string num(s.substr(pos));
            t.type = T_అంకె;
            t.num = std::strtod(num.c_str(), &end);
            pos += (size_t)(end - num.c_str());
            తిరిగి t;
        }
        ఒకవేళ (is_Ident_ప్రారంభం(c))
        {
            size_t ప్రారంభం = pos;
            అయితే (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.పాఠ్యం.assign(s.substr(ప్రారంభం, pos - ప్రారంభం));
            తిరిగి t;
        }
        ఎంపిక (c)
        {
        స్థితి '(': pos++; t.type = T_LPAREN; తిరిగి t;
        స్థితి ')': pos++; t.type = T_RPAREN; తిరిగి t;
        స్థితి ',': pos++; t.type = T_COMMA;  తిరిగి t;
        స్థితి ';': pos++; t.type = T_SEMI;   తిరిగి t;
        }
        // multi/!single-char operators
        స్వయంచాలక two = [&](స్థిరరాశి అక్షరం* op) {
            తిరిగి pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        ఒకవేళ (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.పాఠ్యం.assign(s.substr(pos, 2)); pos += 2; తిరిగి t; }
        ఒకవేళ (c == '=') { pos++; t.type = T_కేటాయించు; తిరిగి t; }
        CV_Assert(std::strchr("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.పాఠ్యం.assign(1, c); pos++;
        తిరిగి t;
    }
};

// --- operator / function tables ----------------------------------------------------------
స్థిర పూర్ణాంకం binPrec(స్థిరరాశి std::string& op)
{
    ఒకవేళ (op == "**") తిరిగి 8;              // power binds tighter than '*'; RIGHT-associative
    ఒకవేళ (op == "*" || op == "/") తిరిగి 7;
    ఒకవేళ (op == "+" || op == "-") తిరిగి 6;
    ఒకవేళ (op == "<" || op == "<=" || op == ">" || op == ">=") తిరిగి 5;
    ఒకవేళ (op == "==" || op == "!=") తిరిగి 4;
    ఒకవేళ (op == "&") తిరిగి 3;
    ఒకవేళ (op == "^") తిరిగి 2;
    ఒకవేళ (op == "|") తిరిగి 1;
    తిరిగి -1;
}

స్థిర శిఖరం binOp(స్థిరరాశి std::string& op)
{
    ఒకవేళ (op == "**") తిరిగి OP_POW;
    ఒకవేళ (op == "+")  తిరిగి OP_కూడు;
    ఒకవేళ (op == "-")  తిరిగి OP_SUB;
    ఒకవేళ (op == "*")  తిరిగి OP_MUL;
    ఒకవేళ (op == "/")  తిరిగి OP_DIV;
    ఒకవేళ (op == "<")  తిరిగి OP_CMP_LT;
    ఒకవేళ (op == "<=") తిరిగి OP_CMP_LE;
    ఒకవేళ (op == ">")  తిరిగి OP_CMP_GT;
    ఒకవేళ (op == ">=") తిరిగి OP_CMP_GE;
    ఒకవేళ (op == "==") తిరిగి OP_CMP_EQ;
    ఒకవేళ (op == "!=") తిరిగి OP_CMP_NE;
    ఒకవేళ (op == "&")  తిరిగి OP_AND;
    ఒకవేళ (op == "|")  తిరిగి OP_OR;
    ఒకవేళ (op == "^")  తిరిగి OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
స్థిర పూర్ణాంకం type_లోతు(స్థిరరాశి std::string& name)
{
    ఒకవేళ (name == "float")    తిరిగి CV_32F;
    ఒకవేళ (name == "double")   తిరిగి CV_64F;
    ఒకవేళ (name == "half" || name == "float16") తిరిగి CV_16F;
    ఒకవేళ (name == "bfloat16") తిరిగి CV_16BF;
    ఒకవేళ (name == "uint8")    తిరిగి CV_8U;
    ఒకవేళ (name == "int8")     తిరిగి CV_8S;
    ఒకవేళ (name == "uint16")   తిరిగి CV_16U;
    ఒకవేళ (name == "int16")    తిరిగి CV_16S;
    ఒకవేళ (name == "uint32")   తిరిగి CV_32U;
    ఒకవేళ (name == "int32")    తిరిగి CV_32S;
    ఒకవేళ (name == "uint64")   తిరిగి CV_64U;
    ఒకవేళ (name == "int64")    తిరిగి CV_64S;
    తిరిగి -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
స్థిర శిఖరం fnOp(స్థిరరాశి std::string& name, పూర్ణాంకం& arity)
{
    నిర్మాణం E { స్థిరరాశి అక్షరం* n; శిఖరం op; };
    స్థిర స్థిరరాశి E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    స్థిర స్థిరరాశి E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    స్థిర స్థిరరాశి E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    కొరకు (స్థిరరాశి E& e : unary)  ఒకవేళ (name == e.n) { arity = 1; తిరిగి e.op; }
    కొరకు (స్థిరరాశి E& e : binary) ఒకవేళ (name == e.n) { arity = 2; తిరిగి e.op; }
    కొరకు (స్థిరరాశి E& e : tern)   ఒకవేళ (name == e.n) { arity = 3; తిరిగి e.op; }
    arity = 0; తిరిగి OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
నిర్మాణం Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    స్థిరరాశి పూర్ణాంకం* ఆదానం_స్థానం;                 // slot id of each external input (precreated)
    పూర్ణాంకం ninputs;
    std::map<std::string, పూర్ణాంకం> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& సమాసం, స్థిరరాశి పూర్ణాంకం* islot, పూర్ణాంకం nin)
        : lex(src), e(సమాసం), ఆదానం_స్థానం(islot), ninputs(nin) { cur = lex.next(); }

    శూన్య advance() { cur = lex.next(); }
    తార్కిక isOp(స్థిరరాశి అక్షరం* op) స్థిరరాశి { తిరిగి cur.type == T_OP && cur.పాఠ్యం == op; }

    శూన్య expect(TokType t, స్థిరరాశి అక్షరం* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    తార్కిక isFlexConst(పూర్ణాంకం స్థానం) స్థిరరాశి
    {
        తిరిగి e.arginfo[స్థానం].రకం == TExpr::CONST && e.arginfo[స్థానం].లోతు == EW_లోతు_NONE;
    }

    పూర్ణాంకం parsePrimary()
    {
        ఒకవేళ (cur.type == T_అంకె)   { పూర్ణాంకం s = e.కూడు_Const(EW_లోతు_NONE, Scalar(cur.num), 1); advance(); తిరిగి s; }
        ఒకవేళ (cur.type == T_ఆదానం) { పూర్ణాంకం idx = cur.ఆదానం_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   తిరిగి ఆదానం_స్థానం[idx]; }
        ఒకవేళ (cur.type == T_LPAREN){ advance(); పూర్ణాంకం x = parseTernary(); expect(T_RPAREN, "expected ')'"); తిరిగి x; }
        ఒకవేళ (cur.type == T_IDENT)
        {
            std::string name = cur.పాఠ్యం; advance();
            ఒకవేళ (cur.type != T_LPAREN)             // variable reference
            {
                స్వయంచాలక it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                తిరిగి it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<పూర్ణాంకం, 8> వాదనలు = {}; పూర్ణాంకం nargs = 0;
            ఒకవేళ (cur.type != T_RPAREN)
            {
                వాదనలు[nargs++] = parseTernary();
                అయితే (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (పూర్ణాంకం)వాదనలు.size() && "ew::expression: too many arguments");
                    వాదనలు[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            పూర్ణాంకం td = type_లోతు(name);
            ఒకవేళ (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                పూర్ణాంకం s = వాదనలు[0];
                తిరిగి isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybe_కూడు_Cast(s, td);
            }

            పూర్ణాంకం arity = 0; శిఖరం op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            ఒకవేళ (arity == 1) తిరిగి e.emitUnary(op, వాదనలు[0]);
            ఒకవేళ (arity == 2) తిరిగి e.emit_ద్విమాన(op, వాదనలు[0], వాదనలు[1]);
            తిరిగి e.emitTernary(op, వాదనలు[0], వాదనలు[1], వాదనలు[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    పూర్ణాంకం parseUnary()
    {
        ఒకవేళ (isOp("-") || isOp("!"))
        {
            std::string op = cur.పాఠ్యం; advance();
            పూర్ణాంకం నిర్వాహ్యం = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            ఒకవేళ (op == "-" && isFlexConst(నిర్వాహ్యం))
            {
                Auto_బఫరు<ద్విగుణ, MAX_LOCAL_CN> v; పూర్ణాంకం cn = constDoubles(e, నిర్వాహ్యం, v);
                Scalar neg; కొరకు (పూర్ణాంకం i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                తిరిగి e.కూడు_Const(EW_లోతు_NONE, neg, std::min(cn, 4));
            }
            తిరిగి e.emitUnary(op == "-" ? OP_NEG : OP_NOT, నిర్వాహ్యం);
        }
        తిరిగి parsePrimary();
    }

    పూర్ణాంకం parse_సమాసం(పూర్ణాంకం minPrec)
    {
        పూర్ణాంకం ఎడమ = parseUnary();
        అయితే (cur.type == T_OP)
        {
            పూర్ణాంకం p = binPrec(cur.పాఠ్యం);
            ఒకవేళ (p < minPrec) ఆపు;
            std::string op = cur.పాఠ్యం; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            పూర్ణాంకం కుడి = parse_సమాసం(op == "**" ? p : p + 1);
            ఎడమ = e.emit_ద్విమాన(binOp(op), ఎడమ, కుడి);
        }
        తిరిగి ఎడమ;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    పూర్ణాంకం parseTernary()
    {
        పూర్ణాంకం cond = parse_సమాసం(0);
        ఒకవేళ (!isOp("?")) తిరిగి cond;
        advance();
        పూర్ణాంకం thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        పూర్ణాంకం elsev = parseTernary();               // right-associative chain
        తిరిగి e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    శూన్య parse_ఫలితం()
    {
        ఒకవేళ (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            size_t nInsn = e.prog.size(), nArg = e.arginfo.size(); పూర్ణాంకం n_తాత్కాలిక = e.ntemps;
            advance();
            పూర్ణాంకం e0 = parseTernary();
            ఒకవేళ (cur.type == T_COMMA)
            {
                e.output(e0);
                అయితే (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                తిరిగి;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = n_తాత్కాలిక;
        }
        e.output(parseTernary());
    }

    శూన్య parse()
    {
        అయితే (నిజం)
        {
            ఒకవేళ (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.పాఠ్యం; advance();
                ఒకవేళ (cur.type == T_కేటాయించు)
                {
                    advance();
                    స్థిరరాశి పూర్ణాంకం స్థానం = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[స్థానం].pinned = నిజం;
                    env[name] = స్థానం;
                    expect(T_SEMI, "expected ';' after assignment");
                    కొనసాగు;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parse_ఫలితం();
            ఒకవేళ (cur.type == T_SEMI) advance();
            ఆపు;
        }
        CV_Assert(cur.type == T_చివర && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

నామాకాశ cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
శూన్య texpr(స్థిరరాశి std::string& సమాసం, ఆదానం_Array_యొక్క_Arrays _inputs, Output_Array_యొక్క_Arrays _outputs)
{
    ఉపయోగించి నామాకాశ cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.రకం() == ఆదానం_Array_2::STD_సదిశ_MAT);
    స్థిరరాశి std::vector<Mat>& inps = *(స్థిరరాశి std::vector<Mat>*)_inputs.getObj();
    స్థిరరాశి పూర్ణాంకం ninputs = (పూర్ణాంకం)inps.size();
    కొరకు (పూర్ణాంకం i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    Auto_బఫరు<పూర్ణాంకం> islot(std::max(ninputs, 1));
    కొరకు (పూర్ణాంకం i = 0; i < ninputs; i++) islot[i] = e.కూడు_ఆదానం(inps[i].లోతు());
    Parser(సమాసం, e, islot.దత్తాంశం(), ninputs).parse();
    e.compile();

    స్వయంచాలక రకం = _outputs.రకం();
    ఒకవేళ (రకం == ఆదానం_Array_2::STD_సదిశ_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.దత్తాంశం(), outs.దత్తాంశం());
    } లేకపోతే {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
