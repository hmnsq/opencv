// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine: op metadata, the graph compiler (type inference + cast
// insertion + liveness), the executor (broadcast traversal via BroadcastOp), the hand builders
// (makeBinaryArithProgram/makeAddWeightedProgram) and the cv::expression parser. Merged from the
// prototype's ew_op/ew_compile/ew_exec/ew_parser. Kernels are reached through the dispatchers in
// arithm.dispatch.cpp (getElemwiseFunc/getDivFunc, declared in arithm_expr.hpp).

#समावेश "precomp.hpp"
#समावेश "arithm_expr.hpp"
#समावेश <algorithm>
#समावेश <climits>
#समावेश <cmath>
#समावेश <cstdlib>
#समावेश <cstring>
#समावेश <map>
#समावेश <ostream>
#समावेश <string>

namespace cv { namespace ew {

// ============================ op metadata (was ew_op.cpp) ============================

स्थिर अक्षर* opName(शीर्ष op)
{
    चयन (op)
    {
    हाल OP_NOP:           वापस "nop";
    हाल OP_NEG:           वापस "neg";
    हाल OP_ABS:           वापस "abs";
    हाल OP_NOT:           वापस "not";
    हाल OP_SQRT:          वापस "sqrt";
    हाल OP_EXP:           वापस "exp";
    हाल OP_LOG:           वापस "log";
    हाल OP_SIN:           वापस "sin";
    हाल OP_COS:           वापस "cos";
    हाल OP_TANH:          वापस "tanh";
    हाल OP_ERF:           वापस "erf";
    हाल OP_RELU:          वापस "relu";
    हाल OP_CAST:          वापस "cast";
    हाल OP_ADD:           वापस "add";
    हाल OP_SUB:           वापस "sub";
    हाल OP_MUL:           वापस "mul";
    हाल OP_DIV:           वापस "div";
    हाल OP_POW:           वापस "pow";
    हाल OP_MIN:           वापस "min";
    हाल OP_MAX:           वापस "max";
    हाल OP_ABSDIFF:       वापस "absdiff";
    हाल OP_ADDW:          वापस "addWeighted";
    हाल OP_HYPOT:         वापस "hypot";
    हाल OP_ATAN2:         वापस "atan2";
    हाल OP_AND:           वापस "and";
    हाल OP_OR:            वापस "or";
    हाल OP_XOR:           वापस "xor";
    हाल OP_CMP_EQ:        वापस "cmp_eq";
    हाल OP_CMP_NE:        वापस "cmp_ne";
    हाल OP_CMP_LT:        वापस "cmp_lt";
    हाल OP_CMP_LE:        वापस "cmp_le";
    हाल OP_CMP_GT:        वापस "cmp_gt";
    हाल OP_CMP_GE:        वापस "cmp_ge";
    हाल OP_CLAMP:         वापस "clamp";
    हाल OP_SELECT:        वापस "select";
    हाल OP_CONVERT_SCALE: वापस "convert_scale";
    शेष:               वापस "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(शीर्ष op)
{
    चयन (op)
    {
    हाल OP_AND: हाल OP_OR: हाल OP_XOR: हाल OP_NOT:
        वापस CAT_BITWISE;
    हाल OP_CMP_EQ: हाल OP_CMP_NE: हाल OP_CMP_LT:
    हाल OP_CMP_LE: हाल OP_CMP_GT: हाल OP_CMP_GE:
        वापस CAT_COMPARE;
    हाल OP_SQRT: हाल OP_EXP: हाल OP_LOG:
    हाल OP_SIN: हाल OP_COS: हाल OP_TANH: हाल OP_ERF: हाल OP_ATAN2: हाल OP_RELU:
        वापस CAT_MATH;
    हाल OP_CAST: हाल OP_CONVERT_SCALE:
        वापस CAT_CAST;
    हाल OP_SELECT:
        वापस CAT_SELECT;
    शेष:
        वापस CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
अटल bool isFloatDepth(पूर्णांक d)
{
    वापस d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
अटल bool depthRepresents(द्विगुन v, पूर्णांक depth)
{
    अगर (isFloatDepth(depth)) वापस true;
    अगर (v != std::न्यूनमान(v)) वापस false;
    चयन (depth)
    {
    हाल CV_8U:  वापस v >= 0            && v <= 255;
    हाल CV_8S:  वापस v >= -128         && v <= 127;
    हाल CV_16U: वापस v >= 0            && v <= 65535;
    हाल CV_16S: वापस v >= -32768       && v <= 32767;
    हाल CV_32U: वापस v >= 0            && v <= 4294967295.0;
    हाल CV_32S: वापस v >= -2147483648.0 && v <= 2147483647.0;
    हाल CV_64U: वापस v >= 0            && v <= 18446744073709551615.0;
    हाल CV_64S: वापस v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    शेष:     वापस false;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
अटल व्योम intRange(पूर्णांक depth, द्विगुन& lo, द्विगुन& hi)
{
    चयन (depth)
    {
    हाल CV_8U:  lo = 0;                     hi = 255; अवरोध;
    हाल CV_8S:  lo = -128;                  hi = 127; अवरोध;
    हाल CV_16U: lo = 0;                     hi = 65535; अवरोध;
    हाल CV_16S: lo = -32768;                hi = 32767; अवरोध;
    हाल CV_32U: lo = 0;                     hi = 4294967295.0; अवरोध;
    हाल CV_32S: lo = -2147483648.0;         hi = 2147483647.0; अवरोध;
    हाल CV_64U: lo = 0;                     hi = 18446744073709551615.0; अवरोध;
    हाल CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; अवरोध;
    शेष:     lo = 0;                     hi = 0; अवरोध;
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
पूर्णांक promoteArith(पूर्णांक a, पूर्णांक b)
{
    constexpr uint64_t leszlut = 034412243322110ULL;
    constexpr अचिन्हित signlut = 0b1001111110100u;
    constexpr अचिन्हित fltlut  = 0b1111000000u;

    अगर (a == b) वापस a;

    पूर्णांक wa = पूर्णांक((leszlut >> (a+1)*3) & 7u), wb = पूर्णांक((leszlut >> (b+1)*3) & 7u);
    पूर्णांक fa = पूर्णांक((fltlut >> (a+1)) & 1u),    fb = पूर्णांक((fltlut >> (b+1)) & 1u);

    अगर (fa + fb == 0)                          // both integer
    {
        पूर्णांक sa = पूर्णांक((signlut >> (a+1)) & 1u), sb = पूर्णांक((signlut >> (b+1)) & 1u);
        अगर (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            वापस wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        पूर्णांक rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr पूर्णांक ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        वापस (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    पूर्णांक maxw = std::max(wa, wb);
    अगर (maxw == 2 && fa + fb == 1)
        वापस fa*a + (1 - fa)*b;
    वापस CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
पूर्णांक absdiff_परिणाम_Depth(पूर्णांक depth)
{
    चयन (depth)
    {
    हाल CV_8S:  वापस CV_8U;
    हाल CV_16S: वापस CV_16U;
    हाल CV_32S: वापस CV_32U;
    हाल CV_64S: वापस CV_64U;
    शेष:     वापस depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
अटल पूर्णांक promote2(पूर्णांक a, पूर्णांक b)
{
    अगर (a == b) वापस a;

    constexpr अचिन्हित lbits = 3, lmask = (1u << lbits) - 1u;
    स्थिर uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    अचिन्हित pr_a = अचिन्हित((typelut >> (a*lbits)) & lmask);
    अचिन्हित pr_b = अचिन्हित((typelut >> (b*lbits)) & lmask);
    अचिन्हित max_pr = std::max(pr_a, pr_b);
    constexpr अचिन्हित dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    स्थिर अचिन्हित ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    वापस पूर्णांक((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
अटल पूर्णांक promoteBitwise(पूर्णांक a, पूर्णांक b)
{
    अगर (a == EW_DEPTH_NONE) वापस b;
    अगर (b == EW_DEPTH_NONE) वापस a;
    अगर (a == b) वापस a;
    वापस CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
अटल पूर्णांक safeWide(पूर्णांक depth)
{
    constexpr अचिन्हित lbits = 3, lmask = (1u << lbits) - 1u;
    स्थिर uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    अचिन्हित pr_depth = अचिन्हित((typelut >> (depth*lbits)) & lmask);
    constexpr अचिन्हित dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    स्थिर अचिन्हित ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    वापस पूर्णांक((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
क्रमागत { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
अटल माप_प्रकार appendConstBuf(Auto_बफर<uint64_t, 16>& constbuf, पूर्णांक srcdepth, स्थिर व्योम* तथ्य, पूर्णांक channels)
{
    स्थिर पूर्णांक cn = std::max(1, channels);
    स्थिर माप_प्रकार nbytes = (माप_प्रकार)cn * CV_ELEM_SIZE1(srcdepth);
    स्थिर माप_प्रकार nu64 = (nbytes + माप(uint64_t) - 1) / माप(uint64_t);
    स्थिर माप_प्रकार ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.तथ्य() + ofs);
    अगर (तथ्य) स_नकल(dst, तथ्य, nbytes); अन्यथा स_रखो(dst, 0, nbytes);
    वापस ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
अटल पूर्णांक constDoubles(स्थिर TExpr& e, पूर्णांक s, Auto_बफर<द्विगुन, MAX_LOCAL_CN>& buf)
{
    स्थिर TExpr::Arg& a = e.arginfo[s];
    स्थिर पूर्णांक cn = std::max(1, a.channels);
    buf.resize(cn);
    स्थिर uchar* src = (स्थिर uchar*)(e.constbuf.तथ्य() + a.constofs);
    अगर (a.srcdepth == CV_64F)
        स_नकल(buf.तथ्य(), src, (माप_प्रकार)cn * माप(द्विगुन));   // common (Scalar / parsed literal)
    अन्यथा
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.तथ्य(), 0, आकार(cn, 1), nullptr);
    वापस cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
अटल अंतरभूत bool isFlexConst(स्थिर TExpr& e, पूर्णांक s)
{
    वापस e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
अटल bool scalar_Const_मूल्य(स्थिर TExpr& e, पूर्णांक s, द्विगुन& v)
{
    स्थिर TExpr::Arg& a = e.arginfo[s];
    अगर (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) वापस false;
    Auto_बफर<द्विगुन, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    वापस true;
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
अटल bool matchScaledChain(स्थिर TExpr& e, पूर्णांक end, पूर्णांक t,
                             पूर्णांक& arrSlot, द्विगुन& scale, पूर्णांक& startIdx)
{
    अगर (end < 0) वापस false;
    पूर्णांक j = end, mul_परिणाम = t;

    // optional trailing narrowing cast
    अगर (e.prog[j].op == OP_CAST && e.prog[j].परिणाम == t)
    {
        mul_परिणाम = e.prog[j].arg0;
        अगर (--j < 0) वापस false;
    }

    स्थिर TExpr::Insn& mul = e.prog[j];
    अगर (mul.op != OP_MUL || mul.परिणाम != mul_परिणाम || mul.params[0] != 1.0) वापस false;

    पूर्णांक arr;
    अगर      (scalar_Const_मूल्य(e, mul.arg1, scale)) arr = mul.arg0;
    अन्यथा अगर (scalar_Const_मूल्य(e, mul.arg0, scale)) arr = mul.arg1;
    अन्यथा वापस false;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    अगर (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].परिणाम == arr &&
        e.arginfo[arr].kind == TExpr::अस्थायी)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    अगर (e.arginfo[arr].kind == TExpr::CONST) वापस false;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    वापस true;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
अटल bool constFits(स्थिर TExpr& e, पूर्णांक s, पूर्णांक d)
{
    अगर (!isFlexConst(e, s)) वापस true;
    Auto_बफर<द्विगुन, MAX_LOCAL_CN> v;
    पूर्णांक cn = constDoubles(e, s, v);
    क्रम (पूर्णांक ch = 0; ch < cn; ch++)
        अगर (!depthRepresents(v[ch], d)) वापस false;
    वापस true;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
पूर्णांक TExpr::emit_द्विआधारी(शीर्ष op, पूर्णांक a, पूर्णांक b, पूर्णांक rdepth, स्थिर Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    अगर (op == OP_ADDW)
    {
        पूर्णांक Tt = arginfo[a].depth;
        अगर (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        अगर (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        अगर (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        पूर्णांक outD = rdepth;
        अगर (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        स्थिर पूर्णांक out = add_अस्थायी(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        अगर (outD == rdepth) वापस out;
        स्थिर पूर्णांक out2 = add_अस्थायी(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        वापस out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    अगर (op == OP_ADD)
    {
        स्थिर पूर्णांक n = (पूर्णांक)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        पूर्णांक x = 0, y = 0, startA = 0, startB = 0;
        द्विगुन alpha = 0, beta = 0;
        अगर (n >= 2 && a != b &&
            arginfo[a].kind == अस्थायी && !arginfo[a].pinned &&
            arginfo[b].kind == अस्थायी && !arginfo[b].pinned &&
            matchScaledChain(*यह, n - 1, b, y, beta, startB) &&
            matchScaledChain(*यह, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            पूर्णांक retired = 0, lowest = पूर्णांक_उच्च;
            क्रम (पूर्णांक i = startA; i < n; i++)
            {
                स्थिर पूर्णांक r = prog[i].परिणाम;
                अगर (arginfo[r].kind != अस्थायी || arginfo[r].pinned) { retired = -1; अवरोध; }
                lowest = std::min(lowest, arginfo[r].सूचकांक);
                retired++;
            }
            अगर (retired > 0 && lowest == ntemps - retired)
            {
                क्रम (पूर्णांक i = startA; i < n; i++) arginfo[prog[i].परिणाम].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                वापस emit_द्विआधारी(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            पूर्णांक acc = 0; द्विगुन gamma = 0;
            अगर      (scalar_Const_मूल्य(*यह, b, gamma)) acc = a;
            अन्यथा अगर (scalar_Const_मूल्य(*यह, a, gamma)) acc = b;
            अगर (acc != 0 && n >= 1 &&
                arginfo[acc].kind == अस्थायी && !arginfo[acc].pinned &&
                arginfo[acc].सूचकांक == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].परिणाम == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                वापस acc;
            }
        }
    }

    स्थिर पूर्णांक nd0 = isFlexConst(*यह, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    स्थिर पूर्णांक nd1 = isFlexConst(*यह, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    स्थिर ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    पूर्णांक परिणाम = rdepth;
    अगर (परिणाम == EW_DEPTH_NONE)
    {
        परिणाम = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        अगर (परिणाम == EW_DEPTH_NONE) परिणाम = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    पूर्णांक base;
    अगर (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = परिणाम;
    अन्यथा अगर (nd0 == EW_DEPTH_NONE) base = nd1;
    अन्यथा अगर (nd1 == EW_DEPTH_NONE) base = nd0;
    अन्यथा base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    अगर (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*यह, b) && !constFits(*यह, b, base))
    {
        द्विगुन lo, hi; intRange(nd0, lo, hi);
        अगर (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            क्रमागत { REAL, CFALSE, CTRUE };
            Auto_बफर<द्विगुन, MAX_LOCAL_CN> tv; पूर्णांक cn = constDoubles(*यह, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            पूर्णांक kind[4] = {}; द्विगुन bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            स्थिर शीर्ष fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            क्रम (पूर्णांक c = 0; c < cn; c++)
            {
                स्थिर द्विगुन t = tv[c]; पूर्णांक k; द्विगुन B = t;
                चयन (op)
                {
                हाल OP_CMP_GT: B = std::न्यूनमान(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; अवरोध;
                हाल OP_CMP_GE: B = std::उच्चमान(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; अवरोध;
                हाल OP_CMP_LT: B = std::उच्चमान(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; अवरोध;
                हाल OP_CMP_LE: B = std::न्यूनमान(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; अवरोध;
                हाल OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; अवरोध;   // a==non-rep -> false
                शेष:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  अवरोध;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            स्वतः ucOp  = [&](पूर्णांक c){ वापस kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            स्वतः ucThr = [&](पूर्णांक c){ वापस kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            स्थिर शीर्ष u0 = ucOp(0);
            bool uniform = true;
            क्रम (पूर्णांक c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            अगर (uniform)
            {
                Scalar s; क्रम (पूर्णांक c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            अन्यथा
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; पूर्णांक patchFlags = 0;
                क्रम (पूर्णांक c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    स्थिर पूर्णांक mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    स्थिर पूर्णांक vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, परिणाम);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                स्थिर पूर्णांक out = add_अस्थायी(परिणाम);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                वापस out;
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
    पूर्णांक depth, wide;
    bool divGuard = false;
    चयन (op)
    {
    हाल OP_MIN: हाल OP_MAX:
    हाल OP_AND: हाल OP_OR: हाल OP_XOR:
        depth = परिणाम; wide = परिणाम; अवरोध;
    हाल OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        पूर्णांक common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        अगर (common == EW_DEPTH_NONE) common = परिणाम;          // both operands flexible consts
        depth = common;
        wide  = absdiff_परिणाम_Depth(common);
        अगर (rdepth == EW_DEPTH_NONE) परिणाम = wide;            // auto result: unsigned same width
        अवरोध;
    }
    हाल OP_CMP_EQ: हाल OP_CMP_NE: हाल OP_CMP_LT:
    हाल OP_CMP_LE: हाल OP_CMP_GT: हाल OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; अवरोध;
    हाल OP_MUL: हाल OP_DIV:
    {
        depth = base;
        स्थिर bool w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        अगर (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            स्वतः intOperand = [&](पूर्णांक s, पूर्णांक nd) {
                अगर (nd != EW_DEPTH_NONE) वापस !isFloatDepth(nd);
                Auto_बफर<द्विगुन, MAX_LOCAL_CN> v; constDoubles(*यह, s, v);
                वापस v[0] == std::न्यूनमान(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        अवरोध;
    }
    हाल OP_POW: हाल OP_HYPOT: हाल OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; अवरोध;
    शेष:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); अवरोध;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    स्वतः fracConst = [&](पूर्णांक s) {
        अगर (!isFlexConst(*यह, s)) वापस false;
        Auto_बफर<द्विगुन, MAX_LOCAL_CN> v; पूर्णांक n = constDoubles(*यह, s, v);
        क्रम (पूर्णांक ch = 0; ch < n; ch++)
            अगर (v[ch] != std::न्यूनमान(v[ch])) वापस true;
        वापस false;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    अगर (cat != CAT_BITWISE && (!constFits(*यह, a, depth) || !constFits(*यह, b, depth)))
    {
        depth = wide;
        स्थिर bool frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        अगर (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (परिणाम == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        अगर (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    पूर्णांक s0 = isFlexConst(*यह, a) ? typedConstFrom(a, depth) : a;
    पूर्णांक s1 = isFlexConst(*यह, b) ? typedConstFrom(b, depth) : b;
    पूर्णांक c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    स्वतः resolve = [&](पूर्णांक rd) {
        वापस op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(परिणाम);
    अगर (k.fptr)
    {
        पूर्णांक res = add_अस्थायी(परिणाम);
        addInsn(op, c0, c1, 0, res, k, params);
        वापस res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णांक w = add_अस्थायी(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    वापस maybeAddCast(w, परिणाम);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
पूर्णांक TExpr::emitUnary(शीर्ष op, पूर्णांक a, पूर्णांक rdepth, स्थिर Scalar& params)
{
    स्थिर पूर्णांक nd = isFlexConst(*यह, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    स्थिर ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    अगर (op == OP_NEG)
        वापस emit_द्विआधारी(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    अगर (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        अगर (!prog.empty() && arginfo[a].kind == अस्थायी && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().परिणाम == a &&
            arginfo[a].सूचकांक == ntemps - 1)
        {
            स्थिर पूर्णांक x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            वापस emit_द्विआधारी(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        वापस emit_द्विआधारी(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    पूर्णांक परिणाम = rdepth;
    अगर (परिणाम == EW_DEPTH_NONE)
    {
        चयन (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        हाल CAT_MATH:    परिणाम = isFloatDepth(nd) ? nd : CV_32F; अवरोध;
        हाल CAT_BITWISE: परिणाम = (nd == EW_DEPTH_NONE) ? CV_32S : nd; अवरोध;  // NOT
        शेष:          परिणाम = (nd == EW_DEPTH_NONE) ? CV_32F : nd; अवरोध;  // NEG, ABS
        }
    }

    पूर्णांक depth, wide;
    चयन (cat)
    {
    हाल CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == परिणाम) ? nd
              : (nd == CV_64F || परिणाम == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        अवरोध;
    हाल CAT_BITWISE: depth = परिणाम; wide = परिणाम; अवरोध;
    शेष:          depth = (nd == EW_DEPTH_NONE) ? परिणाम : nd; wide = परिणाम; अवरोध;  // NEG/ABS
    }

    अगर (!constFits(*यह, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    पूर्णांक s0 = isFlexConst(*यह, a) ? typedConstFrom(a, depth) : a;
    पूर्णांक c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, परिणाम);
    अगर (k.fptr)
    {
        पूर्णांक res = add_अस्थायी(परिणाम);
        addInsn(op, c0, 0, 0, res, k, params);
        वापस res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णांक w = add_अस्थायी(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    वापस maybeAddCast(w, परिणाम);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
पूर्णांक TExpr::emitTernary(शीर्ष op, पूर्णांक a, पूर्णांक b, पूर्णांक c, पूर्णांक rdepth)
{
    स्वतः dep = [&](पूर्णांक s) { वापस isFlexConst(*यह, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    अगर (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        अगर (isFlexConst(*यह, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_द्विआधारी(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        पूर्णांक परिणाम = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        अगर (परिणाम == EW_DEPTH_NONE) परिणाम = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        पूर्णांक sb = isFlexConst(*यह, b) ? typedConstFrom(b, परिणाम) : maybeAddCast(b, परिणाम);
        पूर्णांक sc = isFlexConst(*यह, c) ? typedConstFrom(c, परिणाम) : maybeAddCast(c, परिणाम);
        पूर्णांक res = add_अस्थायी(परिणाम);
        addInsn(OP_SELECT, a, sb, sc, res);
        वापस res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    पूर्णांक परिणाम = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    अगर (परिणाम == EW_DEPTH_NONE) परिणाम = CV_32F;
    स्वतः typed = [&](पूर्णांक s) {
        वापस isFlexConst(*यह, s) ? typedConstFrom(s, परिणाम) : maybeAddCast(s, परिणाम);
    };
    पूर्णांक c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, परिणाम, परिणाम, परिणाम, परिणाम);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णांक res = add_अस्थायी(परिणाम);
    addInsn(op, c0, c1, c2, res, k);
    वापस res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
पूर्णांक TExpr::moveToOutput(पूर्णांक अस्थायी_2, पूर्णांक out)
{
    स्थिर पूर्णांक ninsn = (पूर्णांक)prog.size();
    पूर्णांक producer = -1;
    bool usedAsArg = false;
    क्रम (पूर्णांक i = 0; i < ninsn; i++)
    {
        स्थिर TExpr::Insn& म_लो = prog[i];
        अगर (म_लो.परिणाम == अस्थायी_2) producer = i;
        अगर (म_लो.arg0 == अस्थायी_2 || म_लो.arg1 == अस्थायी_2 || म_लो.arg2 == अस्थायी_2) usedAsArg = true;
    }
    अगर (arginfo[अस्थायी_2].kind == अस्थायी && producer >= 0 && !usedAsArg && !arginfo[अस्थायी_2].pinned &&
        arginfo[अस्थायी_2].depth == arginfo[out].depth)
    {
        // MOVE semantics: redirect `temp`'s single producer to write `out` directly, then leave the
        // source slot EMPTY - reclassify it to NONE. A NONE slot gets no physical buffer (compile's
        // liveness only walks TEMP slots) and is skipped everywhere in exec, so no stale dead-TEMP slot
        // remains (which, buffer or not, would still cost per-tile setup in the general path) - and no
        // slot removal / index renumbering is needed. This keeps single-op programs at zero temps =>
        // compile()'s no-temp early-out (zero heap traffic) still fires. Every caller moves the result
        // it JUST emitted, so `temp` is always the last-created temp - decrement ntemps to retire its
        // index and keep the temp indices dense (0..ntemps-1) for compile()'s liveness arrays.
        CV_Assert(arginfo[अस्थायी_2].सूचकांक == ntemps - 1);
        prog[producer].परिणाम = out;
        arginfo[अस्थायी_2].kind = NONE;
        ntemps--;
        वापस out;
    }
    addInsn(OP_CAST, अस्थायी_2, 0, 0, out);                     // same/different depth copy into `out`
    वापस out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
पूर्णांक TExpr::output(पूर्णांक मूल_Slot)
{
    वापस moveToOutput(मूल_Slot, addOutput(arginfo[मूल_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
व्योम TExpr::compile()
{
    स्थिर पूर्णांक ninsn = (पूर्णांक)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    स्थिर पूर्णांक nslots = (पूर्णांक)arginfo.size();
    nconsts = 0;
    क्रम (पूर्णांक s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        अगर (a.kind != CONST || a.depth == EW_DEPTH_NONE) जारी;
        स्थिर पूर्णांक cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        स्थिर माप_प्रकार sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        Auto_बफर<uint64_t, माप(uint64_t)> srcbuf(divUp((माप_प्रकार)cn * sesz, (अचिन्हित)माप(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.तथ्य();
        स_नकल(srcbytes, (स्थिर uchar*)(constbuf.तथ्य() + a.constofs), (माप_प्रकार)cn * sesz);
        स्थिर माप_प्रकार ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.तथ्य() + ofs);
        अगर (sd == dd) स_नकल(dst, srcbytes, (माप_प्रकार)cn * desz);
        अन्यथा getConvertFunc(sd, dd)(srcbytes, 0, nullptr, 0, dst, 0, आकार(cn, 1), nullptr);
        a.constofs = ofs; a.srcdepth = dd;
        nconsts++;
    }

    // ---- liveness: pack temps into a minimal set of reusable physical buffers. A program with no
    //      temps (single-op add/sub/mul/...) needs none of this - early out with ZERO heap traffic,
    //      so building such a program (the dominant cv::add-style call) allocates nothing. The temp
    //      case uses stack-backed AutoBuffers (inline for typical small expressions). ----
    nbuffers = 0;
    bufEszPrefix.resize(1); bufEszPrefix[0] = 0;
    capElems = पूर्णांक_उच्च;                       // no temps => exec runs each tile in one block, no scratch
    अगर (ntemps == 0) वापस;

    Auto_बफर<पूर्णांक, 32> अस्थायी_Of_Slot(nslots);
    क्रम (पूर्णांक s = 0; s < nslots; s++) अस्थायी_Of_Slot[s] = -1;
    क्रम (पूर्णांक s = 1; s < nslots; s++)
        अगर (arginfo[s].kind == अस्थायी) अस्थायी_Of_Slot[s] = arginfo[s].सूचकांक;

    Auto_बफर<पूर्णांक, 16> lastUse(ntemps);
    क्रम (पूर्णांक t = 0; t < ntemps; t++) lastUse[t] = -1;
    क्रम (पूर्णांक i = 0; i < ninsn; i++)
    {
        स्थिर TExpr::Insn& म_लो = prog[i];
        पूर्णांक as[3] = { म_लो.arg0, म_लो.arg1, म_लो.arg2 };
        क्रम (पूर्णांक k = 0; k < 3; k++)
            अगर (अस्थायी_Of_Slot[as[k]] >= 0) lastUse[अस्थायी_Of_Slot[as[k]]] = i;
    }

    बफर_Of_अस्थायी.resize(ntemps);
    क्रम (पूर्णांक t = 0; t < ntemps; t++) बफर_Of_अस्थायी[t] = -1;
    Auto_बफर<पूर्णांक, 16> freeBufs(ntemps);
    पूर्णांक nfree = 0, nbuf = 0;
    क्रम (पूर्णांक i = 0; i < ninsn; i++)
    {
        स्थिर TExpr::Insn& म_लो = prog[i];
        पूर्णांक rt = अस्थायी_Of_Slot[म_लो.परिणाम];
        अगर (rt >= 0 && बफर_Of_अस्थायी[rt] < 0)
            बफर_Of_अस्थायी[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        पूर्णांक as[3] = { म_लो.arg0, म_लो.arg1, म_लो.arg2 };
        क्रम (पूर्णांक k = 0; k < 3; k++)
        {
            पूर्णांक t = अस्थायी_Of_Slot[as[k]];
            अगर (t >= 0 && lastUse[t] == i)
            {
                freeBufs[nfree++] = बफर_Of_अस्थायी[t];
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
    क्रम (पूर्णांक b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    क्रम (पूर्णांक s = 1; s < nslots; s++)
        अगर (arginfo[s].kind == अस्थायी)
        {
            पूर्णांक b = बफर_Of_अस्थायी[arginfo[s].सूचकांक], e = (पूर्णांक)CV_ELEM_SIZE1(arginfo[s].depth);
            अगर (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    क्रम (पूर्णांक b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    स्थिर पूर्णांक योग_Esz = bufEszPrefix[nbuffers];
    capElems = योग_Esz > 0 ? std::max(64, (4 * 1024) / योग_Esz) : पूर्णांक_उच्च;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
अटल व्योम matLogical(स्थिर Mat& m, MatShape& shp, EwSteps& step, पूर्णांक& esz1)
{
    esz1 = (पूर्णांक)m.elemSize1();
    पूर्णांक nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    क्रम (पूर्णांक i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
अटल bool broadcastShape(स्थिर MatShape* shps, पूर्णांक K, MatShape& out)
{
    पूर्णांक nd = 0;
    क्रम (पूर्णांक k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    क्रम (पूर्णांक k = 0; k < K; k++)
    {
        स्थिर MatShape& s = shps[k];
        पूर्णांक off = nd - s.dims;
        क्रम (पूर्णांक i = 0; i < s.dims; i++)
        {
            पूर्णांक d = s[i], &o = out[off + i];
            अगर (o == 1) o = d;
            अन्यथा अगर (d != 1 && d != o) वापस false;
        }
    }
    वापस true;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
अटल पूर्णांक opCost(शीर्ष op)
{
    चयन (op)
    {
    हाल OP_ADD: हाल OP_SUB: हाल OP_MUL: हाल OP_MIN: हाल OP_MAX:
    हाल OP_ABSDIFF: हाल OP_AND: हाल OP_OR: हाल OP_XOR: हाल OP_NOT:
    हाल OP_NEG: हाल OP_ABS: हाल OP_CAST: हाल OP_RELU: हाल OP_SELECT:
    हाल OP_CMP_EQ: हाल OP_CMP_NE: हाल OP_CMP_LT:
    हाल OP_CMP_LE: हाल OP_CMP_GT: हाल OP_CMP_GE: वापस 1;
    हाल OP_DIV: हाल OP_SQRT: हाल OP_HYPOT: हाल OP_CONVERT_SCALE: वापस 3;
    हाल OP_SIN: हाल OP_COS: हाल OP_TANH: हाल OP_ERF: हाल OP_ATAN2:
    हाल OP_EXP: हाल OP_LOG: हाल OP_POW:           वापस 6;
    शेष:                                         वापस 3;
    }
}

TExpr::TExpr()
{
    सफा();
}

// Reset to an empty program: drop all instructions/slots and re-seat slot 0 = NONE.
व्योम TExpr::सफा()
{
    prog.allocate(0);
    arginfo.allocate(0);
    बफर_Of_अस्थायी.allocate(0);
    constbuf.allocate(0);
    ninputs = noutputs = ntemps = nbuffers = 0;
    Arg none;                       // slot 0: the reserved empty operand (kind == NONE)
    arginfo.push_back(none);
}

// Human-readable dump of the program (slot table + instruction list). Const values are shown in f64.
व्योम TExpr::dump(std::ब_प्रवाह& os) स्थिर
{
    स्वतः dn = [](पूर्णांक d) -> स्थिर अक्षर* {
        चयन (d) {
        हाल EW_DEPTH_NONE: वापस "flex";
        हाल CV_8U:  वापस "u8";  हाल CV_8S:  वापस "s8";
        हाल CV_16U: वापस "u16"; हाल CV_16S: वापस "s16";
        हाल CV_32U: वापस "u32"; हाल CV_32S: वापस "s32";
        हाल CV_64U: वापस "u64"; हाल CV_64S: वापस "s64";
        हाल CV_16F: वापस "f16"; हाल CV_16BF:वापस "bf16";
        हाल CV_32F: वापस "f32"; हाल CV_64F: वापस "f64";
        हाल CV_Bool:वापस "bool";
        शेष:     वापस "?"; }
    };
    स्वतः kn = [](ArgKind k) -> स्थिर अक्षर* {
        चयन (k) { हाल NONE: वापस "none"; हाल निवेश: वापस "in"; हाल CONST: वापस "const";
                     हाल अस्थायी: वापस "temp"; हाल OUTPUT: वापस "out"; } वापस "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (पूर्णांक)prog.size() << "\n";
    os << "  slots:\n";
    क्रम (पूर्णांक s = 0; s < (पूर्णांक)arginfo.size(); s++) {
        स्थिर Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        अगर (a.kind != NONE) os << " " << dn(a.depth);
        अगर (a.kind == निवेश || a.kind == OUTPUT || a.kind == अस्थायी) os << " idx=" << a.सूचकांक;
        अगर (a.kind == CONST) {
            स्थिर पूर्णांक cn = std::max(1, a.channels);
            Auto_बफर<द्विगुन, MAX_LOCAL_CN> v(cn);
            स्थिर uchar* src = (स्थिर uchar*)(constbuf.तथ्य() + a.constofs);
            अगर (a.srcdepth == CV_64F) स_नकल(v.तथ्य(), src, (माप_प्रकार)cn * माप(द्विगुन));
            अन्यथा getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.तथ्य(), 0, आकार(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            क्रम (पूर्णांक c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    क्रम (पूर्णांक i = 0; i < (पूर्णांक)prog.size(); i++) {
        स्थिर Insn& म_लो = prog[i];
        os << "    " << i << ": " << opName(म_लो.op) << "(" << म_लो.arg0;
        अगर (म_लो.arg1) os << ", " << म_लो.arg1;
        अगर (म_लो.arg2) os << ", " << म_लो.arg2;
        os << ") -> " << म_लो.परिणाम;
        अगर (म_लो.kernel.flags) os << " kflags=" << म_लो.kernel.flags;
        अगर (म_लो.params[0] != 1.0 || म_लो.params[1] != 0.0)
            os << " params=[" << म_लो.params[0] << "," << म_लो.params[1] << "," << म_लो.params[2] << "]";
        अगर (!म_लो.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
पूर्णांक TExpr::add_निवेश(पूर्णांक depth)
{
    Arg a; a.kind = निवेश; a.depth = depth; a.सूचकांक = ninputs++;
    arginfo.push_back(a); वापस (पूर्णांक)arginfo.size() - 1;
}

पूर्णांक TExpr::addOutput(पूर्णांक depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.सूचकांक = noutputs++;
    arginfo.push_back(a); वापस (पूर्णांक)arginfo.size() - 1;
}

पूर्णांक TExpr::add_अस्थायी(पूर्णांक depth)
{
    Arg a; a.kind = अस्थायी; a.depth = depth; a.सूचकांक = ntemps++;
    arginfo.push_back(a); वापस (पूर्णांक)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
पूर्णांक TExpr::addConst(पूर्णांक depth, स्थिर Scalar& v, पूर्णांक channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); वापस (पूर्णांक)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
पूर्णांक TExpr::addConst(पूर्णांक depth, पूर्णांक srcdepth, स्थिर व्योम* तथ्य, पूर्णांक channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, तथ्य, channels);
    arginfo.push_back(a); वापस (पूर्णांक)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
पूर्णांक TExpr::typedConstFrom(पूर्णांक srcSlot, पूर्णांक depth)
{
    स्थिर Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); वापस (पूर्णांक)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
पूर्णांक TExpr::addInsn(शीर्ष op, पूर्णांक a0, पूर्णांक a1, पूर्णांक a2, पूर्णांक परिणाम, स्थिर TKernel& kernel, स्थिर Scalar& params)
{
    TExpr::Insn म_लो; म_लो.op = op; म_लो.arg0 = a0; म_लो.arg1 = a1; म_लो.arg2 = a2; म_लो.परिणाम = परिणाम;
    म_लो.params = params; म_लो.kernel = kernel;
    prog.push_back(म_लो);
    वापस (पूर्णांक)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
पूर्णांक TExpr::addInsn(शीर्ष op, पूर्णांक a0, पूर्णांक a1, पूर्णांक a2, पूर्णांक परिणाम, स्थिर Scalar& params)
{
    पूर्णांक d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    पूर्णांक d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    पूर्णांक d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[परिणाम].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    वापस addInsn(op, a0, a1, a2, परिणाम, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
पूर्णांक TExpr::maybeAddCast(पूर्णांक arg, पूर्णांक depth)
{
    अगर (arginfo[arg].depth == depth) वापस arg;
    पूर्णांक t = add_अस्थायी(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    वापस t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
अटल अंतरभूत व्योम runInsn(स्थिर TExpr::Insn& म_लो,
                           स्थिर व्योम* p0, माप_प्रकार y0, माप_प्रकार x0,
                           स्थिर व्योम* p1, माप_प्रकार y1, माप_प्रकार x1,
                           स्थिर व्योम* p2, माप_प्रकार y2, माप_प्रकार x2,
                           व्योम* pr, माप_प्रकार yr, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक code = म_लो.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                म_लो.params.val, म_लो.kernel.flags, म_लो.kernel.userdata);
    CV_Assert(code >= 0);
}

काष्ठा EwBody {
    स्थिर TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    स्थिर पूर्णांक* slotMap;
    स्थिर चिन्हित अक्षर* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    स्थिर पूर्णांक* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    पूर्णांक ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

व्योम TExpr::outputShape(स्थिर Mat* स्थिर* inputs, MatShape& spatial, पूर्णांक& channels) स्थिर
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    स्थिर पूर्णांक rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    bool sameShape = ninputs >= 1;
    क्रम (पूर्णांक i = 1; sameShape && i < ninputs; i++)
    {
        स्थिर Mat& a = *inputs[i];
        अगर (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = false;
        अन्यथा क्रम (पूर्णांक d = 0; d < a.dims; d++) अगर (a.size[d] != inputs[0]->size[d]) { sameShape = false; अवरोध; }
    }
    अगर (sameShape) { spatial = inputs[0]->size; channels = rcn0; वापस; }

    Auto_बफर<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; पूर्णांक esz1;
    क्रम (पूर्णांक i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.तथ्य(), ninputs, full) && "ew: inputs not broadcast-compatible");
    स्थिर पूर्णांक ndFull = (पूर्णांक)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
व्योम TExpr::outputShape(स्थिर Mat* inputs, MatShape& spatial, पूर्णांक& channels) स्थिर
{
    Auto_बफर<स्थिर Mat*, 16> ptrs(std::max(ninputs, 1));
    क्रम (पूर्णांक i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.तथ्य(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
अटल constexpr पूर्णांक LOCAL_HDRS = 3;
अटल constexpr पूर्णांक LOCAL_CONSTS = 4;
अटल constexpr पूर्णांक LOCAL_OPS = 16;

व्योम TExpr::exec(स्थिर Mat* स्थिर* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    स्थिर पूर्णांक nslots = (पूर्णांक)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    अगर (nconsts == 0 && ninputs >= 1)
    {
        constexpr माप_प्रकार EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        स्थिर Mat& r = *inputs[0];
        स्थिर पूर्णांक rcn = r.channels();
        bool ok = r.isContinuous();
        क्रम (पूर्णांक i = 1; ok && i < ninputs; i++)
        {
            स्थिर Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        क्रम (पूर्णांक s = 1; ok && s < nslots; s++)
            अगर (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].सूचकांक].isContinuous();
        स्थिर माप_प्रकार योग = ok ? r.योग() * rcn : 0;
        अगर (ok && योग == 0)
        {
            स्थिर MatShape rshape = r.size;
            क्रम (पूर्णांक s = 1; s < nslots; s++)
                अगर (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].सूचकांक].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            वापस;
        }
        अगर (ok && योग <= EW_FASTPATH_MAX)
        {
            स्थिर MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            क्रम (पूर्णांक s = 1; s < nslots; s++)
                अगर (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].सूचकांक].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            स्थिर पूर्णांक ninsn = (पूर्णांक)prog.size();

            अगर (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                स्वतः ptrOf = [&](पूर्णांक s) -> व्योम* {
                    अगर (s <= 0) वापस nullptr;
                    स्थिर Arg& ai = arginfo[s];
                    अगर (ai.kind == निवेश)  वापस (व्योम*)inputs[ai.सूचकांक]->तथ्य;
                    अगर (ai.kind == OUTPUT) वापस (व्योम*)outputs[ai.सूचकांक].तथ्य;
                    वापस nullptr;   // NONE
                };
                स्थिर पूर्णांक w = (पूर्णांक)योग;
                क्रम (पूर्णांक n = 0; n < ninsn; n++)
                {
                    स्थिर Insn& म_लो = prog[n];
                    runInsn(म_लो, ptrOf(म_लो.arg0), 0, 1, ptrOf(म_लो.arg1), 0, 1,
                            ptrOf(म_लो.arg2), 0, 1, ptrOf(म_लो.परिणाम), 0, w, 1);
                }
                वापस;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            स्थिर पूर्णांक योग_Esz = bufEszPrefix[nbuffers];
            स्थिर पूर्णांक wf0 = std::min((पूर्णांक)योग, capElems);

            स्थिर माप_प्रकार region = align_आकार((माप_प्रकार)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            Auto_बफर<uint64_t, (4*1024 + 256)/माप(uint64_t)> scratchBuf(
                divUp((माप_प्रकार)योग_Esz * region, (अचिन्हित)माप(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.तथ्य();
            क्रम (पूर्णांक x0 = 0; x0 < (पूर्णांक)योग; x0 += wf0)
            {
                स्थिर पूर्णांक wf = std::min(wf0, (पूर्णांक)योग - x0);
                स्वतः ptrOf = [&](पूर्णांक s) -> व्योम* {
                    अगर (s <= 0) वापस nullptr;
                    स्थिर Arg& ai = arginfo[s];
                    माप_प्रकार esz = CV_ELEM_SIZE1(ai.depth);
                    अगर (ai.kind == निवेश)  वापस (uchar*)inputs[ai.सूचकांक]->तथ्य  + (माप_प्रकार)x0 * esz;
                    अगर (ai.kind == OUTPUT) वापस (uchar*)outputs[ai.सूचकांक].तथ्य + (माप_प्रकार)x0 * esz;
                    अगर (ai.kind != अस्थायी)   वापस nullptr;                                    // NONE: moved-from
                    स्थिर पूर्णांक b = बफर_Of_अस्थायी.empty() ? ai.सूचकांक : बफर_Of_अस्थायी[ai.सूचकांक];  //अस्थायी
                    वापस scratch + (माप_प्रकार)bufEszPrefix[b] * region;                        // fragment-local
                };
                क्रम (पूर्णांक n = 0; n < ninsn; n++)
                {
                    स्थिर Insn& म_लो = prog[n];
                    runInsn(म_लो, ptrOf(म_लो.arg0), 0, 1, ptrOf(म_लो.arg1), 0, 1,
                            ptrOf(म_लो.arg2), 0, 1, ptrOf(म_लो.परिणाम), 0, wf, 1);
                }
            }
            वापस;
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
    पूर्णांक rchannels;
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
    bool inplace = false;
    क्रम (पूर्णांक i = 0; i < ninputs && !inplace; i++)
        क्रम (पूर्णांक j = 0; j < noutputs; j++)
            अगर (inputs[i]->तथ्य && inputs[i]->तथ्य == outputs[j].तथ्य) { inplace = true; अवरोध; }
    पूर्णांक nhdrs = inplace ? ninputs : 0;

    Auto_बफर<स्थिर Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    Auto_बफर<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    Auto_बफर<स्थिर Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    Auto_बफर<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    Auto_बफर<पूर्णांक, LOCAL_OPS> slotMap(nslots);
    Auto_बफर<चिन्हित अक्षर, LOCAL_OPS> slotKind(nslots);
    पूर्णांक narr = 0, nc = 0;
    अगर (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        क्रम (पूर्णांक j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.तथ्य();
    }
    slotKind[0] = (चिन्हित अक्षर)TExpr::NONE;
    क्रम (पूर्णांक s = 1; s < nslots; s++)
    {
        स्थिर TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (चिन्हित अक्षर)ai.kind;
        अगर (ai.kind == TExpr::निवेश) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.सूचकांक];
        }
        अन्यथा अगर (ai.kind == TExpr::OUTPUT) {
            outputs[ai.सूचकांक].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.सूचकांक];
        }
        अन्यथा अगर (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (चिन्हित अक्षर)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        अन्यथा अगर (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            स्थिर पूर्णांक c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (व्योम*)(constbuf.तथ्य() + ai.constofs));
            slotKind[s] = (चिन्हित अक्षर)TExpr::निवेश;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        अन्यथा अगर (ai.kind == TExpr::अस्थायी) {
            slotMap[s] = बफर_Of_अस्थायी.empty() ? ai.सूचकांक : बफर_Of_अस्थायी[ai.सूचकांक];  // physical buffer id
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
    स्थिर पूर्णांक ninsn = (पूर्णांक)prog.size();
    दीर्घ दीर्घ costPerElem = 0;
    क्रम (पूर्णांक n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    दीर्घ दीर्घ otot = (दीर्घ दीर्घ)rchannels;
    क्रम (पूर्णांक d = 0; d < (पूर्णांक)spatial.size(); d++) otot *= spatial[d];
    अगर (otot == 0)
        वापस;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    स्थिर द्विगुन T = (द्विगुन)std::max(get_अंक_Threads(), 1);
    स्थिर द्विगुन nstripes = std::min(
        (द्विगुन)otot * (द्विगुन)std::max<दीर्घ दीर्घ>(costPerElem, 1) * (1./ (द्विगुन)(1 << 16)),
        std::min(4.*T, std::max(32., 3.*T)));

    EwBody body;
    body.prog = prog.तथ्य();
    body.ninsn = ninsn;
    body.slotMap = slotMap.तथ्य();

    // ---- 5. drive: broadcastOp does geometry + 2D tiling + parallelism; the body runs the
    //         frozen program on each tile (temps tile-local, re-pointed from the tile slices).
    //         expandChannels=true => the body always sees single-channel data.
    //
    //         All state the body needs is packed into one POD (EwBody) so the lambda captures a
    //         SINGLE reference: the closure is then one pointer, fits std::function's small-buffer
    //         and never heap-allocates. Inside, the hot fields are copied into locals so the
    //         per-tile/per-insn loops read them from registers, not through the captured pointer. -

    body.bufEszPrefix = bufEszPrefix.तथ्य();
    body.slotKind = slotKind.तथ्य();
    body.nslots = nslots;
    body.nbuffers = nbuffers;
    body.capElems = capElems;

    broadcastOp(arr.तथ्य(), narr, [&](स्थिर BrTile& tile)
    {
        EwBody& bc = body;
        स्थिर TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        स्थिर पूर्णांक* स्थिर smap  = bc.slotMap;
        स्थिर चिन्हित अक्षर* स्थिर skind = bc.slotKind;
        स्थिर पूर्णांक nin = bc.ninsn, nsl = bc.nslots;
        स्थिर पूर्णांक w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        स्थिर पूर्णांक* स्थिर eszPrefix = bc.bufEszPrefix;
        स्थिर पूर्णांक bw = std::min(w, bc.capElems);
        स्थिर पूर्णांक bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        स्थिर माप_प्रकार region = align_आकार((माप_प्रकार)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        Auto_बफर<uint64_t, (4*1024 + 256)/माप(uint64_t)> tstoreBuf(
            divUp((माप_प्रकार)eszPrefix[bc.nbuffers] * region, (अचिन्हित)माप(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.तथ्य();

        Auto_बफर<BrSlice, LOCAL_OPS> args(nsl);
        क्रम (पूर्णांक y0 = 0; y0 < h; y0 += bh)
        {
            स्थिर पूर्णांक hf = std::min(bh, h - y0);
            क्रम (पूर्णांक x0 = 0; x0 < w; x0 += bw)
            {
                स्थिर पूर्णांक wf = std::min(bw, w - x0);
                क्रम (पूर्णांक s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    स्थिर पूर्णांक k = skind[s];
                    माप_प्रकार esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    अगर (k == TExpr::अस्थायी)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (माप_प्रकार)eszPrefix[smap[s]] * region;
                        a.stepy = (माप_प्रकार)wf*esz; a.stepx = 1;
                    }
                    अन्यथा                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        स्थिर BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((माप_प्रकार)y0 * sl.stepy + (माप_प्रकार)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                क्रम (पूर्णांक n = 0; n < nin; n++)
                {
                    स्थिर TExpr::Insn& म_लो = insns[n];
                    स्थिर BrSlice& a0 = args[म_लो.arg0]; स्थिर BrSlice& a1 = args[म_लो.arg1];
                    स्थिर BrSlice& a2 = args[म_लो.arg2]; स्थिर BrSlice& rr = args[म_लो.परिणाम];
                    runInsn(म_लो, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (व्योम*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, true, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
व्योम TExpr::exec(स्थिर Mat* inputs, Mat* outputs)
{
    Auto_बफर<स्थिर Mat*, 16> ptrs(std::max(ninputs, 1));
    क्रम (पूर्णांक i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    exec(ptrs.तथ्य(), outputs);
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
व्योम make_द्विआधारी_Arith_Program(TExpr& p, शीर्ष op, पूर्णांक depth0, पूर्णांक depth1, पूर्णांक rdepth,
                            पूर्णांक maskDepth, द्विगुन scale)
{
    p.सफा();
    अगर (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        अगर (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        अन्यथा
        {
            rdepth = promoteArith(depth0, depth1);
            अगर (op == OP_ABSDIFF) rdepth = absdiff_परिणाम_Depth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    स्थिर bool masked = maskDepth != EW_DEPTH_NONE;
    पूर्णांक sIn0  = p.add_निवेश(depth0);
    पूर्णांक sIn1  = p.add_निवेश(depth1);
    पूर्णांक sMask = masked ? p.add_निवेश(maskDepth) : 0;
    पूर्णांक sOut  = p.addOutput(rdepth);

    पूर्णांक r = p.emit_द्विआधारी(op, sIn0, sIn1, rdepth, Scalar(scale));
    अगर (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    अन्यथा
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
व्योम makeAddWeightedProgram(TExpr& p, पूर्णांक depth0, पूर्णांक depth1, पूर्णांक rdepth,
                            द्विगुन alpha, द्विगुन beta, द्विगुन gamma)
{
    p.सफा();
    पूर्णांक sA   = p.add_निवेश(depth0);
    पूर्णांक sB   = p.add_निवेश(depth1);
    पूर्णांक sOut = p.addOutput(rdepth);
    पूर्णांक r = p.emit_द्विआधारी(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
    p.moveToOutput(r, sOut);
    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}


// ============================ parser (was ew_parser.cpp) ============================

namespace {

// --- tokens ------------------------------------------------------------------------------
क्रमागत TokType { T_अंक, T_निवेश, T_IDENT, T_OP, T_LPAREN, T_RPAREN, T_COMMA, T_SEMI, T_ASSIGN, T_END };

काष्ठा Token
{
    TokType type = T_END;
    द्विगुन num = 0;
    पूर्णांक निवेश_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
काष्ठा Lexer
{
    std::string_view s;
    माप_प्रकार pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    अटल bool isIdentStart(अक्षर c) { वापस std::है_अक्षर((अचिन्हित अक्षर)c) || c == '_'; }
    अटल bool isIdentChar(अक्षर c)  { वापस std::है_अक्षर_अंक((अचिन्हित अक्षर)c) || c == '_'; }

    Token next()
    {
        जबतक (pos < s.size() && std::है_खाली((अचिन्हित अक्षर)s[pos])) pos++;
        Token t;
        अगर (pos >= s.size()) { t.type = T_END; वापस t; }

        अक्षर c = s[pos];

        अगर (c == '{')                       // input placeholder {N}
        {
            pos++;
            माप_प्रकार start = pos;
            जबतक (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_निवेश;
            t.निवेश_2 = std::म_से_प(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            वापस t;
        }
        अगर (std::है_अंक((अचिन्हित अक्षर)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::है_अंक((अचिन्हित अक्षर)s[pos + 1])))
        {
            अक्षर* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_अंक;
            t.num = std::म_से_भग्न(num.c_str(), &end);
            pos += (माप_प्रकार)(end - num.c_str());
            वापस t;
        }
        अगर (isIdentStart(c))
        {
            माप_प्रकार start = pos;
            जबतक (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            वापस t;
        }
        चयन (c)
        {
        हाल '(': pos++; t.type = T_LPAREN; वापस t;
        हाल ')': pos++; t.type = T_RPAREN; वापस t;
        हाल ',': pos++; t.type = T_COMMA;  वापस t;
        हाल ';': pos++; t.type = T_SEMI;   वापस t;
        }
        // multi/!single-char operators
        स्वतः two = [&](स्थिर अक्षर* op) {
            वापस pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        अगर (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; वापस t; }
        अगर (c == '=') { pos++; t.type = T_ASSIGN; वापस t; }
        CV_Assert(std::म_अक्षर("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        वापस t;
    }
};

// --- operator / function tables ----------------------------------------------------------
अटल पूर्णांक binPrec(स्थिर std::string& op)
{
    अगर (op == "**") वापस 8;              // power binds tighter than '*'; RIGHT-associative
    अगर (op == "*" || op == "/") वापस 7;
    अगर (op == "+" || op == "-") वापस 6;
    अगर (op == "<" || op == "<=" || op == ">" || op == ">=") वापस 5;
    अगर (op == "==" || op == "!=") वापस 4;
    अगर (op == "&") वापस 3;
    अगर (op == "^") वापस 2;
    अगर (op == "|") वापस 1;
    वापस -1;
}

अटल शीर्ष binOp(स्थिर std::string& op)
{
    अगर (op == "**") वापस OP_POW;
    अगर (op == "+")  वापस OP_ADD;
    अगर (op == "-")  वापस OP_SUB;
    अगर (op == "*")  वापस OP_MUL;
    अगर (op == "/")  वापस OP_DIV;
    अगर (op == "<")  वापस OP_CMP_LT;
    अगर (op == "<=") वापस OP_CMP_LE;
    अगर (op == ">")  वापस OP_CMP_GT;
    अगर (op == ">=") वापस OP_CMP_GE;
    अगर (op == "==") वापस OP_CMP_EQ;
    अगर (op == "!=") वापस OP_CMP_NE;
    अगर (op == "&")  वापस OP_AND;
    अगर (op == "|")  वापस OP_OR;
    अगर (op == "^")  वापस OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
अटल पूर्णांक typeDepth(स्थिर std::string& name)
{
    अगर (name == "float")    वापस CV_32F;
    अगर (name == "double")   वापस CV_64F;
    अगर (name == "half" || name == "float16") वापस CV_16F;
    अगर (name == "bfloat16") वापस CV_16BF;
    अगर (name == "uint8")    वापस CV_8U;
    अगर (name == "int8")     वापस CV_8S;
    अगर (name == "uint16")   वापस CV_16U;
    अगर (name == "int16")    वापस CV_16S;
    अगर (name == "uint32")   वापस CV_32U;
    अगर (name == "int32")    वापस CV_32S;
    अगर (name == "uint64")   वापस CV_64U;
    अगर (name == "int64")    वापस CV_64S;
    वापस -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
अटल शीर्ष fnOp(स्थिर std::string& name, पूर्णांक& arity)
{
    काष्ठा E { स्थिर अक्षर* n; शीर्ष op; };
    अटल स्थिर E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    अटल स्थिर E द्विग[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    अटल स्थिर E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    क्रम (स्थिर E& e : unary)  अगर (name == e.n) { arity = 1; वापस e.op; }
    क्रम (स्थिर E& e : द्विग) अगर (name == e.n) { arity = 2; वापस e.op; }
    क्रम (स्थिर E& e : tern)   अगर (name == e.n) { arity = 3; वापस e.op; }
    arity = 0; वापस OP_NOP;
}

// --- parser ------------------------------------------------------------------------------
// Builds the program DIRECTLY into the TExpr (no intermediate graph), exactly like the hand
// builders: each parse step calls e.emitUnary()/emitBinary()/addConst()/output() and returns the arg
// SLOT holding its value. Input depths are known up front (from the input Mats), so every operand is
// typed as it is parsed - the emit* layers infer result depths and insert casts on the spot.
काष्ठा Parser
{
    Lexer lex;
    Token cur;
    TExpr& e;
    स्थिर पूर्णांक* निवेश_Slot;                 // slot id of each external input (precreated)
    पूर्णांक ninputs;
    std::map<std::string, पूर्णांक> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& व्यंजक, स्थिर पूर्णांक* islot, पूर्णांक nin)
        : lex(src), e(व्यंजक), निवेश_Slot(islot), ninputs(nin) { cur = lex.next(); }

    व्योम advance() { cur = lex.next(); }
    bool isOp(स्थिर अक्षर* op) स्थिर { वापस cur.type == T_OP && cur.text == op; }

    व्योम expect(TokType t, स्थिर अक्षर* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    bool isFlexConst(पूर्णांक slot) स्थिर
    {
        वापस e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    पूर्णांक parsePrimary()
    {
        अगर (cur.type == T_अंक)   { पूर्णांक s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); वापस s; }
        अगर (cur.type == T_निवेश) { पूर्णांक idx = cur.निवेश_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   वापस निवेश_Slot[idx]; }
        अगर (cur.type == T_LPAREN){ advance(); पूर्णांक x = parseTernary(); expect(T_RPAREN, "expected ')'"); वापस x; }
        अगर (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            अगर (cur.type != T_LPAREN)             // variable reference
            {
                स्वतः it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                वापस it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<पूर्णांक, 8> args = {}; पूर्णांक nargs = 0;
            अगर (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                जबतक (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (पूर्णांक)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            पूर्णांक td = typeDepth(name);
            अगर (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                पूर्णांक s = args[0];
                वापस isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            पूर्णांक arity = 0; शीर्ष op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            अगर (arity == 1) वापस e.emitUnary(op, args[0]);
            अगर (arity == 2) वापस e.emit_द्विआधारी(op, args[0], args[1]);
            वापस e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    पूर्णांक parseUnary()
    {
        अगर (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            पूर्णांक operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            अगर (op == "-" && isFlexConst(operand))
            {
                Auto_बफर<द्विगुन, MAX_LOCAL_CN> v; पूर्णांक cn = constDoubles(e, operand, v);
                Scalar neg; क्रम (पूर्णांक i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                वापस e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            वापस e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        वापस parsePrimary();
    }

    पूर्णांक parse_व्यंजक(पूर्णांक minPrec)
    {
        पूर्णांक बाँए = parseUnary();
        जबतक (cur.type == T_OP)
        {
            पूर्णांक p = binPrec(cur.text);
            अगर (p < minPrec) अवरोध;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            पूर्णांक दाँए = parse_व्यंजक(op == "**" ? p : p + 1);
            बाँए = e.emit_द्विआधारी(binOp(op), बाँए, दाँए);
        }
        वापस बाँए;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    पूर्णांक parseTernary()
    {
        पूर्णांक cond = parse_व्यंजक(0);
        अगर (!isOp("?")) वापस cond;
        advance();
        पूर्णांक thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        पूर्णांक elsev = parseTernary();               // right-associative chain
        वापस e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    व्योम parse_परिणाम()
    {
        अगर (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            माप_प्रकार nInsn = e.prog.size(), nArg = e.arginfo.size(); पूर्णांक n_अस्थायी = e.ntemps;
            advance();
            पूर्णांक e0 = parseTernary();
            अगर (cur.type == T_COMMA)
            {
                e.output(e0);
                जबतक (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                वापस;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = n_अस्थायी;
        }
        e.output(parseTernary());
    }

    व्योम parse()
    {
        जबतक (true)
        {
            अगर (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                अगर (cur.type == T_ASSIGN)
                {
                    advance();
                    स्थिर पूर्णांक slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = true;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    जारी;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parse_परिणाम();
            अगर (cur.type == T_SEMI) advance();
            अवरोध;
        }
        CV_Assert(cur.type == T_END && "ew::expression: trailing tokens");
        CV_Assert(e.noutputs > 0 && "ew::expression: no result");
    }
};

} // anonymous namespace

}} // namespace cv::ew

namespace cv {

// Public entry point (declared in opencv2/core.hpp): parse a broadcasting element-wise expression,
// compile it and run it over the inputs. This IS the engine's string front-end - there is no
// separate cv::ew::expression indirection.
व्योम texpr(स्थिर std::string& व्यंजक, निवेश_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using namespace cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == निवेश_Array_2::STD_VECTOR_MAT);
    स्थिर std::vector<Mat>& inps = *(स्थिर std::vector<Mat>*)_inputs.getObj();
    स्थिर पूर्णांक ninputs = (पूर्णांक)inps.size();
    क्रम (पूर्णांक i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    Auto_बफर<पूर्णांक> islot(std::max(ninputs, 1));
    क्रम (पूर्णांक i = 0; i < ninputs; i++) islot[i] = e.add_निवेश(inps[i].depth());
    Parser(व्यंजक, e, islot.तथ्य(), ninputs).parse();
    e.compile();

    स्वतः kind = _outputs.kind();
    अगर (kind == निवेश_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& म_दो = _outputs.getMatVecRef();
        म_दो.resize(e.noutputs);
        e.exec(inps.तथ्य(), म_दो.तथ्य());
    } अन्यथा {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
