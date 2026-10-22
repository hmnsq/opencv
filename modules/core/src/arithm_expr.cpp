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

स्थिरांक अक्षर* opName(शीर्ष op)
{
    निवड (op)
    {
    स्थिती OP_NOP:           परत "nop";
    स्थिती OP_NEG:           परत "neg";
    स्थिती OP_ABS:           परत "abs";
    स्थिती OP_NOT:           परत "not";
    स्थिती OP_SQRT:          परत "sqrt";
    स्थिती OP_EXP:           परत "exp";
    स्थिती OP_LOG:           परत "log";
    स्थिती OP_SIN:           परत "sin";
    स्थिती OP_COS:           परत "cos";
    स्थिती OP_TANH:          परत "tanh";
    स्थिती OP_ERF:           परत "erf";
    स्थिती OP_RELU:          परत "relu";
    स्थिती OP_CAST:          परत "cast";
    स्थिती OP_ADD:           परत "add";
    स्थिती OP_SUB:           परत "sub";
    स्थिती OP_MUL:           परत "mul";
    स्थिती OP_DIV:           परत "div";
    स्थिती OP_POW:           परत "pow";
    स्थिती OP_MIN:           परत "min";
    स्थिती OP_MAX:           परत "max";
    स्थिती OP_ABSDIFF:       परत "absdiff";
    स्थिती OP_ADDW:          परत "addWeighted";
    स्थिती OP_HYPOT:         परत "hypot";
    स्थिती OP_ATAN2:         परत "atan2";
    स्थिती OP_AND:           परत "and";
    स्थिती OP_OR:            परत "or";
    स्थिती OP_XOR:           परत "xor";
    स्थिती OP_CMP_EQ:        परत "cmp_eq";
    स्थिती OP_CMP_NE:        परत "cmp_ne";
    स्थिती OP_CMP_LT:        परत "cmp_lt";
    स्थिती OP_CMP_LE:        परत "cmp_le";
    स्थिती OP_CMP_GT:        परत "cmp_gt";
    स्थिती OP_CMP_GE:        परत "cmp_ge";
    स्थिती OP_CLAMP:         परत "clamp";
    स्थिती OP_SELECT:        परत "select";
    स्थिती OP_CONVERT_SCALE: परत "convert_scale";
    पूर्वनिर्धारित:               परत "?";
    }
}

// --- op category (declared in ew_op.hpp) --------------------------------------------------
ElemwiseCategory opCategory(शीर्ष op)
{
    निवड (op)
    {
    स्थिती OP_AND: स्थिती OP_OR: स्थिती OP_XOR: स्थिती OP_NOT:
        परत CAT_BITWISE;
    स्थिती OP_CMP_EQ: स्थिती OP_CMP_NE: स्थिती OP_CMP_LT:
    स्थिती OP_CMP_LE: स्थिती OP_CMP_GT: स्थिती OP_CMP_GE:
        परत CAT_COMPARE;
    स्थिती OP_SQRT: स्थिती OP_EXP: स्थिती OP_LOG:
    स्थिती OP_SIN: स्थिती OP_COS: स्थिती OP_TANH: स्थिती OP_ERF: स्थिती OP_ATAN2: स्थिती OP_RELU:
        परत CAT_MATH;
    स्थिती OP_CAST: स्थिती OP_CONVERT_SCALE:
        परत CAT_CAST;
    स्थिती OP_SELECT:
        परत CAT_SELECT;
    पूर्वनिर्धारित:
        परत CAT_ARITH;   // add/sub/mul/div/pow/min/max/absdiff/neg/abs/clamp
    }
}


// ============================ compiler (was ew_compile.cpp) ============================

// ---------------------------------------------------------------------------
// Type inference helpers (deliberately small so they can grow later).
// ---------------------------------------------------------------------------
स्थिर तार्किक isFloatDepth(पूर्णांक d)
{
    परत d == CV_16F || d == CV_16BF || d == CV_32F || d == CV_64F;
}

// Can `depth` hold `v` exactly? Floats: yes (close enough for our promotion). Integers: only if v
// is integral and in range. Used so a const operand (e.g. 2.5 in a*2.5) is NOT quantized into a
// narrow-integer direct kernel - such an op must fall back to the float working type instead.
स्थिर तार्किक depthRepresents(द्विगुण v, पूर्णांक depth)
{
    जर (isFloatDepth(depth)) परत सत्य;
    जर (v != std::न्यूनमान(v)) परत असत्य;
    निवड (depth)
    {
    स्थिती CV_8U:  परत v >= 0            && v <= 255;
    स्थिती CV_8S:  परत v >= -128         && v <= 127;
    स्थिती CV_16U: परत v >= 0            && v <= 65535;
    स्थिती CV_16S: परत v >= -32768       && v <= 32767;
    स्थिती CV_32U: परत v >= 0            && v <= 4294967295.0;
    स्थिती CV_32S: परत v >= -2147483648.0 && v <= 2147483647.0;
    स्थिती CV_64U: परत v >= 0            && v <= 18446744073709551615.0;
    स्थिती CV_64S: परत v >= -9223372036854775808.0 && v <= 9223372036854775807.0;
    पूर्वनिर्धारित:     परत असत्य;
    }
}

// [min, max] representable value of an integer depth, as doubles (exact for <=32-bit; the 64-bit
// endpoints round to the nearest double). Used by the compare-with-const boundary rewrite.
स्थिर शून्य intRange(पूर्णांक depth, द्विगुण& lo, द्विगुण& hi)
{
    निवड (depth)
    {
    स्थिती CV_8U:  lo = 0;                     hi = 255; थांबा;
    स्थिती CV_8S:  lo = -128;                  hi = 127; थांबा;
    स्थिती CV_16U: lo = 0;                     hi = 65535; थांबा;
    स्थिती CV_16S: lo = -32768;                hi = 32767; थांबा;
    स्थिती CV_32U: lo = 0;                     hi = 4294967295.0; थांबा;
    स्थिती CV_32S: lo = -2147483648.0;         hi = 2147483647.0; थांबा;
    स्थिती CV_64U: lo = 0;                     hi = 18446744073709551615.0; थांबा;
    स्थिती CV_64S: lo = -9223372036854775808.0; hi = 9223372036854775807.0; थांबा;
    पूर्वनिर्धारित:     lo = 0;                     hi = 0; थांबा;
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

    जर (a == b) परत a;

    पूर्णांक wa = पूर्णांक((leszlut >> (a+1)*3) & 7u), wb = पूर्णांक((leszlut >> (b+1)*3) & 7u);
    पूर्णांक fa = पूर्णांक((fltlut >> (a+1)) & 1u),    fb = पूर्णांक((fltlut >> (b+1)) & 1u);

    जर (fa + fb == 0)                          // both integer
    {
        पूर्णांक sa = पूर्णांक((signlut >> (a+1)) & 1u), sb = पूर्णांक((signlut >> (b+1)) & 1u);
        जर (sa == sb)                          // same signedness -> the wider one (keeps the sign)
            परत wa >= wb ? a : b;
        // mixed sign -> a SIGNED result holding the unsigned operand: the signed width if it is already
        // wider, else one step past the unsigned width; past 64 bits there is no int -> f64.
        पूर्णांक rw = std::max(wa + (1 - sa), wb + (1 - sb));
        constexpr पूर्णांक ilut = (CV_64F << 5*5) | (CV_64S << 4*5) | (CV_32S << 3*5) |
                             (CV_16S << 2*5) | (CV_8S << 1*5) | (CV_8S << 0*5);
        परत (ilut >> rw*5) & CV_MAT_DEPTH_MASK;
    }

    // at least one float -> the smallest float covering both operands. Lift each integer operand to the
    // float size class that holds it (8-bit -> 16-float, 16-bit -> 32F, 32/64-bit -> 64F); a float
    // operand keeps its own. Size 2 with exactly one float -> that float (a 8-bit int + 16-float pair);
    // otherwise 32F, or 64F once the size class reaches 4.
    wa += 1 - fa;
    wb += 1 - fb;
    पूर्णांक maxw = std::max(wa, wb);
    जर (maxw == 2 && fa + fb == 1)
        परत fa*a + (1 - fa)*b;
    परत CV_32F + (maxw >= 4);
}

// Declared in ew_op.hpp. A signed integer |a-b| reaches 2^width-1, so absdiff returns the unsigned
// type of the same width; unsigned/float depths are unchanged.
पूर्णांक absdiffResultDepth(पूर्णांक depth)
{
    निवड (depth)
    {
    स्थिती CV_8S:  परत CV_8U;
    स्थिती CV_16S: परत CV_16U;
    स्थिती CV_32S: परत CV_32U;
    स्थिती CV_64S: परत CV_64U;
    पूर्वनिर्धारित:     परत depth;
    }
}

// numpy-ish promotion of two KNOWN depths for the COMPUTE type (float dominates; the wider integer
// otherwise, and - unlike promoteArith - two WIDE integers float-promote: promote2(16U,64S)=64F).
// This is cv::add's "wtype": the type both operands are brought to before the op runs.
स्थिर पूर्णांक promote2(पूर्णांक a, पूर्णांक b)
{
    जर (a == b) परत a;

    constexpr अचिन्हित lbits = 3, lmask = (1u << lbits) - 1u;
    स्थिरांक uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    अचिन्हित pr_a = अचिन्हित((typelut >> (a*lbits)) & lmask);
    अचिन्हित pr_b = अचिन्हित((typelut >> (b*lbits)) & lmask);
    अचिन्हित max_pr = std::max(pr_a, pr_b);
    constexpr अचिन्हित dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    स्थिरांक अचिन्हित ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                            (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    परत पूर्णांक((ctypelut >> (max_pr*dbits)) & dmask);
}

// Common type for a bit-pattern op (AND/OR/XOR). Unlike promoteArith there is NO numeric promotion:
// a bitwise op keeps the operand's own type, and a scalar operand simply takes the array's type and
// is reinterpreted by its bits. A flexible CONST (EW_DEPTH_NONE) yields to the concrete operand; two
// concrete depths must share the element WIDTH (cv::bitwise requires equal types) - if they differ we
// keep the wider one (the kernel dispatches by element size). NONE x NONE stays NONE (caller defaults).
स्थिर पूर्णांक promoteBitwise(पूर्णांक a, पूर्णांक b)
{
    जर (a == EW_DEPTH_NONE) परत b;
    जर (b == EW_DEPTH_NONE) परत a;
    जर (a == b) परत a;
    परत CV_ELEM_SIZE1(a) >= CV_ELEM_SIZE1(b) ? a : b;
}

// A wide type in which add(depth,depth->wide) exists and the sum is held without a premature clamp
// (the signed-widening fallback for ADD/SUB, whose difference may go negative).
स्थिर पूर्णांक safeWide(पूर्णांक depth)
{
    constexpr अचिन्हित lbits = 3, lmask = (1u << lbits) - 1u;
    स्थिरांक uint64_t typelut = (uint64_t)((0ULL << CV_8U*lbits) | (0ULL << CV_8S*lbits) |
                                        (1ULL << CV_16U*lbits) | (1ULL << CV_16S*lbits) |
                                        (2ULL << CV_32U*lbits) | (2ULL << CV_32S*lbits) |
                                        (3ULL << CV_16F*lbits) | (3ULL << CV_16BF*lbits) |
                                        (3ULL << CV_32F*lbits) | (4ULL << CV_64F*lbits) |
                                        (4ULL << CV_64S*lbits) | (4ULL << CV_64U*lbits));
    अचिन्हित pr_depth = अचिन्हित((typelut >> (depth*lbits)) & lmask);
    constexpr अचिन्हित dbits = CV_CN_SHIFT, dmask = (1u << dbits) - 1u;
    स्थिरांक अचिन्हित ctypelut = ((CV_16S << 0*dbits) | (CV_32S << 1*dbits) | (CV_64S << 2*dbits) |
                              (CV_32F << 3*dbits) | (CV_64F << 4*dbits));
    परत पूर्णांक((ctypelut >> (pr_depth*dbits)) & dmask);
}

// Inline capacity for the small per-const double scratch used during type inference; larger
// channel counts (e.g. Vec<_,16>) just spill the AutoBuffer to the heap.
गणना { MAX_LOCAL_CN = 16 };

// Append `channels` values of depth `srcdepth` (raw bytes at `data`, or zero-filled if null) to
// constbuf, padded to a whole number of uint64_t slots. Returns the offset (in uint64_t units).
स्थिर माप_प्रकार appendConstBuf(AutoBuffer<uint64_t, 16>& constbuf, पूर्णांक srcdepth, स्थिरांक शून्य* data, पूर्णांक channels)
{
    स्थिरांक पूर्णांक cn = std::max(1, channels);
    स्थिरांक माप_प्रकार nbytes = (माप_प्रकार)cn * CV_ELEM_SIZE1(srcdepth);
    स्थिरांक माप_प्रकार nu64 = (nbytes + आकार(uint64_t) - 1) / आकार(uint64_t);
    स्थिरांक माप_प्रकार ofs = constbuf.size();
    constbuf.resize(ofs + nu64);
    uchar* dst = (uchar*)(constbuf.data() + ofs);
    जर (data) स_नकल(dst, data, nbytes); अन्यथा स_रखो(dst, 0, nbytes);
    परत ofs;
}

// Read a CONST slot's source values (constbuf, `srcdepth`) as doubles into `buf`. Returns channels.
स्थिर पूर्णांक constDoubles(स्थिरांक TExpr& e, पूर्णांक s, AutoBuffer<द्विगुण, MAX_LOCAL_CN>& buf)
{
    स्थिरांक TExpr::Arg& a = e.arginfo[s];
    स्थिरांक पूर्णांक cn = std::max(1, a.channels);
    buf.resize(cn);
    स्थिरांक uchar* src = (स्थिरांक uchar*)(e.constbuf.data() + a.constofs);
    जर (a.srcdepth == CV_64F)
        स_नकल(buf.data(), src, (माप_प्रकार)cn * आकार(द्विगुण));   // common (Scalar / parsed literal)
    अन्यथा
        getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)buf.data(), 0, Size(cn, 1), nullptr);
    परत cn;
}

// Materialize a flexible CONST `s` at depth `d`, or return `s` unchanged for a typed operand
// (the emit* layer then casts it to the compute depth). Used by the emit* policy layers.
स्थिर ओळीत तार्किक isFlexConst(स्थिरांक TExpr& e, पूर्णांक s)
{
    परत e.arginfo[s].kind == TExpr::CONST && e.arginfo[s].depth == EW_DEPTH_NONE;
}

// Read a CONST slot that carries exactly ONE value. OP_ADDW transports alpha/beta/gamma in the
// instruction's params block - a single value applied to every channel - so a per-channel const
// cannot ride it and must keep the expanded form.
स्थिर तार्किक scalar_Const_मूल्य(स्थिरांक TExpr& e, पूर्णांक s, द्विगुण& v)
{
    स्थिरांक TExpr::Arg& a = e.arginfo[s];
    जर (a.kind != TExpr::CONST || std::max(1, a.channels) != 1) परत असत्य;
    AutoBuffer<द्विगुण, MAX_LOCAL_CN> buf;
    constDoubles(e, s, buf);
    v = buf[0];
    परत सत्य;
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
स्थिर तार्किक matchScaledChain(स्थिरांक TExpr& e, पूर्णांक end, पूर्णांक t,
                             पूर्णांक& arrSlot, द्विगुण& scale, पूर्णांक& startIdx)
{
    जर (end < 0) परत असत्य;
    पूर्णांक j = end, mulResult = t;

    // optional trailing narrowing cast
    जर (e.prog[j].op == OP_CAST && e.prog[j].result == t)
    {
        mulResult = e.prog[j].arg0;
        जर (--j < 0) परत असत्य;
    }

    स्थिरांक TExpr::Insn& mul = e.prog[j];
    जर (mul.op != OP_MUL || mul.result != mulResult || mul.params[0] != 1.0) परत असत्य;

    पूर्णांक arr;
    जर      (scalar_Const_मूल्य(e, mul.arg1, scale)) arr = mul.arg0;
    अन्यथा जर (scalar_Const_मूल्य(e, mul.arg0, scale)) arr = mul.arg1;
    अन्यथा परत असत्य;                                  // array*array is not an addWeighted term
    startIdx = j;

    // optional leading widening cast feeding the multiply
    जर (j - 1 >= 0 && e.prog[j-1].op == OP_CAST && e.prog[j-1].result == arr &&
        e.arginfo[arr].kind == TExpr::TEMP)
    {
        arr = e.prog[j-1].arg0;
        startIdx = j - 1;
    }

    जर (e.arginfo[arr].kind == TExpr::CONST) परत असत्य;   // scalar*scalar is folded elsewhere
    arrSlot = arr;
    परत सत्य;
}

// Can a flexible CONST `s` be represented exactly at depth `d`? (typed operands trivially "fit").
स्थिर तार्किक constFits(स्थिरांक TExpr& e, पूर्णांक s, पूर्णांक d)
{
    जर (!isFlexConst(e, s)) परत सत्य;
    AutoBuffer<द्विगुण, MAX_LOCAL_CN> v;
    पूर्णांक cn = constDoubles(e, s, v);
    साठी (पूर्णांक ch = 0; ch < cn; ch++)
        जर (!depthRepresents(v[ch], d)) परत असत्य;
    परत सत्य;
}

// ---------------------------------------------------------------------------
// TExpr::emitBinary(): type policy for a 2-input op. Derive the result depth, the compute depth and a
// wide fallback per op family, then cast both operands and emit (direct, else wide+narrow). Operands
// may be typed (INPUT/TEMP) or a flexible CONST (materialized at the compute depth here).
// ---------------------------------------------------------------------------
पूर्णांक TExpr::emit_द्विमान(शीर्ष op, पूर्णांक a, पूर्णांक b, पूर्णांक rdepth, स्थिरांक Scalar& params)
{
    // addWeighted a*alpha + b*beta + gamma (params = {alpha, beta, gamma}): ONE fused kernel (two v_fma).
    // Inputs are the same type T (cast to a common type if not). The kernel outputs T/f32 (small ints,
    // f16/bf16, f32) or f64 directly, at its own natural work precision; for any other requested rdepth
    // it computes at that natural precision and casts the RESULT to rdepth afterward (never the inputs
    // up before the op, that would spend an extra cast and run the op at needlessly high precision).
    जर (op == OP_ADDW)
    {
        पूर्णांक Tt = arginfo[a].depth;
        जर (arginfo[a].depth != arginfo[b].depth)
        {
            Tt = promoteArith(arginfo[a].depth, arginfo[b].depth);
            a = maybeAddCast(a, Tt); b = maybeAddCast(b, Tt);
        }
        जर (Tt == CV_Bool)
            CV_Error(Error::StsNotImplemented, "addWeighted: CV_Bool inputs are not supported, cast explicitly "
                     "first, e.g. cv::texpr(\"uint8({0})*{1} + uint8({2})*{3}\", {a, alpha, b, beta})");
        जर (rdepth == EW_DEPTH_NONE) rdepth = Tt;              // default dtype = input depth
        TKernel k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, rdepth);
        पूर्णांक outD = rdepth;
        जर (!k.fptr)                                          // no direct T->rdepth kernel: compute at T's natural work depth, cast after
        {
            outD = (Tt==CV_32U || Tt==CV_32S || Tt==CV_64U || Tt==CV_64S || Tt==CV_64F) ? CV_64F : CV_32F;
            k = getElemwiseFunc(OP_ADDW, Tt, Tt, EW_DEPTH_NONE, outD);
            CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
        }
        स्थिरांक पूर्णांक out = addTemp(outD);
        addInsn(OP_ADDW, a, b, 0, out, k, Scalar(params[0], params[1], params[2]));
        जर (outD == rdepth) परत out;
        स्थिरांक पूर्णांक out2 = addTemp(rdepth);
        addInsn(OP_CAST, out, 0, 0, out2);
        परत out2;
    }

    // peephole: a*alpha + b*beta [+ gamma] -> the fused OP_ADDW kernel (two v_fma). Written out,
    // the form costs four instructions, three temp buffers and four passes over the data; OP_ADDW
    // does the whole thing in one pass with no temp. Like the abs(x - y) -> absdiff peephole in
    // emitUnary() this RETIRES the instructions it folds, so - same as there - it must leave a
    // pinned (named) slot alone: see TExpr::Arg::pinned.
    जर (op == OP_ADD)
    {
        स्थिरांक पूर्णांक n = (पूर्णांक)prog.size();

        // (x*alpha) + (y*beta). Both multiplies are the last two instructions and their results the
        // last two temps (the operands were emitted immediately before this add), so folding them is
        // a pop of the program tail and no slot renumbering is needed.
        पूर्णांक x = 0, y = 0, startA = 0, startB = 0;
        द्विगुण alpha = 0, beta = 0;
        जर (n >= 2 && a != b &&
            arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            arginfo[b].kind == TEMP && !arginfo[b].pinned &&
            matchScaledChain(*हे, n - 1, b, y, beta, startB) &&
            matchScaledChain(*हे, startB - 1, a, x, alpha, startA) &&
            x != a && x != b && y != a && y != b &&
            // OP_ADDW has no CV_Bool form (it would have to pick a numeric work type); leave a bool
            // operand to the plain expansion rather than turning it into an error.
            arginfo[x].depth != CV_Bool && arginfo[y].depth != CV_Bool)
        {
            // The two runs together are exactly the tail [startA, n), so folding them is a truncation
            // of the program; every temp they produced was created after every surviving one, so
            // retiring them keeps the temp indices dense (0..ntemps-1) for compile()'s liveness.
            पूर्णांक retired = 0, lowest = पूर्णांक_उच्च;
            साठी (पूर्णांक i = startA; i < n; i++)
            {
                स्थिरांक पूर्णांक r = prog[i].result;
                जर (arginfo[r].kind != TEMP || arginfo[r].pinned) { retired = -1; थांबा; }
                lowest = std::min(lowest, arginfo[r].index);
                retired++;
            }
            जर (retired > 0 && lowest == ntemps - retired)
            {
                साठी (पूर्णांक i = startA; i < n; i++) arginfo[prog[i].result].kind = NONE;
                prog.resize(startA);
                ntemps -= retired;
                परत emit_द्विमान(OP_ADDW, x, y, rdepth, Scalar(alpha, beta, 0.));
            }
        }

        // (x*alpha + y*beta) + gamma: the ADDW is already emitted and its gamma still 0, so the
        // trailing scalar folds into the instruction's params instead of costing another pass.
        {
            पूर्णांक acc = 0; द्विगुण gamma = 0;
            जर      (scalar_Const_मूल्य(*हे, b, gamma)) acc = a;
            अन्यथा जर (scalar_Const_मूल्य(*हे, a, gamma)) acc = b;
            जर (acc != 0 && n >= 1 &&
                arginfo[acc].kind == TEMP && !arginfo[acc].pinned &&
                arginfo[acc].index == ntemps - 1 &&
                prog[n-1].op == OP_ADDW && prog[n-1].result == acc &&
                (rdepth == EW_DEPTH_NONE || rdepth == arginfo[acc].depth))
            {
                prog[n-1].params[2] += gamma;
                परत acc;
            }
        }
    }

    स्थिरांक पूर्णांक nd0 = isFlexConst(*हे, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    स्थिरांक पूर्णांक nd1 = isFlexConst(*हे, b) ? EW_DEPTH_NONE : arginfo[b].depth;
    स्थिरांक ElemwiseCategory cat = opCategory(op);

    // result depth (auto unless forced): compare -> mask (u8); everything else -> promoteArith.
    पूर्णांक result = rdepth;
    जर (result == EW_DEPTH_NONE)
    {
        result = (cat == CAT_COMPARE) ? CV_8U
               : (cat == CAT_BITWISE) ? promoteBitwise(nd0, nd1)   // no numeric promotion for bit ops
               : promoteArith(nd0, nd1);
        जर (result == EW_DEPTH_NONE) result = CV_32F;          // const (op) const
    }

    // common type over the CONCRETE operands (a flexible const does not force it).
    पूर्णांक base;
    जर (nd0 == EW_DEPTH_NONE && nd1 == EW_DEPTH_NONE) base = result;
    अन्यथा जर (nd0 == EW_DEPTH_NONE) base = nd1;
    अन्यथा जर (nd1 == EW_DEPTH_NONE) base = nd0;
    अन्यथा base = promote2(nd0, nd1);

    // COMPARE of an INTEGER array against a threshold that doesn't fit that type as-is (fractional,
    // out-of-range, or EQ/NE of a non-representable value): emit a SINGLE NATIVE integer compare instead
    // of widening both sides to f64. Per channel the relation is either a REAL boundary (a>=B / a<=B) or
    // a CONSTANT (always-false / always-true). Boundaries: a>t==a>=floor(t)+1; a>=t==a>=ceil(t);
    // a<t==a<=ceil(t)-1; a<=t==a<=floor(t); a==(non-rep) never true; a!=(non-rep) always true. One family
    // op (GE for GT/GE, LE for LT/LE, else EQ/NE) runs with a placeholder threshold on const channels;
    // the per-channel fix-up  result = (rawmask & M) | V  is FOLDED into the compare kernel via its flags
    // (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass, no separate patch kernel.
    जर (cat == CAT_COMPARE && nd0 != EW_DEPTH_NONE && !isFloatDepth(nd0) &&
        isFlexConst(*हे, b) && !constFits(*हे, b, base))
    {
        द्विगुण lo, hi; intRange(nd0, lo, hi);
        जर (hi > lo)   // hi<=lo => a depth intRange doesn't model (CV_Bool ...): leave to the f64 path
        {
            गणना { REAL, CFALSE, CTRUE };
            AutoBuffer<द्विगुण, MAX_LOCAL_CN> tv; पूर्णांक cn = constDoubles(*हे, b, tv);
            CV_Assert(cn <= 4);          // a CONST is <= 4 channels (addConst) - fixed arrays, no heap
            पूर्णांक kind[4] = {}; द्विगुण bound[4] = {};   // {} for -Wmaybe-uninitialized only
                                                      // (filled for all cn used below)
            स्थिरांक शीर्ष fam = (op == OP_CMP_GT || op == OP_CMP_GE) ? OP_CMP_GE
                          : (op == OP_CMP_LT || op == OP_CMP_LE) ? OP_CMP_LE : op;   // EQ/NE unchanged
            साठी (पूर्णांक c = 0; c < cn; c++)
            {
                स्थिरांक द्विगुण t = tv[c]; पूर्णांक k; द्विगुण B = t;
                निवड (op)
                {
                स्थिती OP_CMP_GT: B = std::न्यूनमान(t)+1; k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; थांबा;
                स्थिती OP_CMP_GE: B = std::उच्चमान(t);    k = B > hi ? CFALSE : B <= lo ? CTRUE : REAL; थांबा;
                स्थिती OP_CMP_LT: B = std::उच्चमान(t)-1;  k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; थांबा;
                स्थिती OP_CMP_LE: B = std::न्यूनमान(t);   k = B < lo ? CFALSE : B >= hi ? CTRUE : REAL; थांबा;
                स्थिती OP_CMP_EQ: k = depthRepresents(t, nd0) ? REAL : CFALSE; थांबा;   // a==non-rep -> false
                पूर्वनिर्धारित:        k = depthRepresents(t, nd0) ? REAL : CTRUE;  थांबा;   // NE: a!=non-rep -> true
                }
                kind[c] = k; bound[c] = B;
            }
            // If every channel lands on ONE op (single-channel, all-real, all-false, all-true, or
            // real+true for a GE/LE family) the compare ALONE yields the result - a plain native compare,
            // no fix-up. REAL -> fam(bound); forced-TRUE -> the family's always-true form (GE lo / LE hi);
            // forced-FALSE -> "a < lo".
            स्वतः ucOp  = [&](पूर्णांक c){ परत kind[c]==REAL ? fam : kind[c]==CFALSE ? OP_CMP_LT
                                          : (fam == OP_CMP_LE ? OP_CMP_LE : OP_CMP_GE); };
            स्वतः ucThr = [&](पूर्णांक c){ परत kind[c]==REAL ? bound[c]
                                          : (kind[c]==CTRUE && fam==OP_CMP_LE) ? hi : lo; };
            स्थिरांक शीर्ष u0 = ucOp(0);
            तार्किक uniform = सत्य;
            साठी (पूर्णांक c = 1; c < cn && uniform; c++) uniform = (ucOp(c) == u0);
            जर (uniform)
            {
                Scalar s; साठी (पूर्णांक c = 0; c < cn; c++) s[c] = ucThr(c);
                b = addConst(EW_DEPTH_NONE, s, cn); op = u0;   // rewrite threshold + op -> fall through
            }
            अन्यथा
            {
                // genuine per-channel op split (only a multi-channel scalar can cause it -> the executor
                // lays it out as a short-row tile). One family compare with a placeholder threshold on
                // const channels; the per-channel fix-up (rawmask & M) | V is FOLDED into the kernel via
                // its flags (M=255 real / 0 const, V=255 always-true / 0 else) - no extra pass.
                Scalar ts; पूर्णांक patchFlags = 0;
                साठी (पूर्णांक c = 0; c < cn; c++)
                {
                    ts[c] = kind[c]==REAL ? bound[c] : lo;     // in-range placeholder for a const channel
                    स्थिरांक पूर्णांक mbits = kind[c]==REAL  ? 3 : 0;  // M: real -> 255 (keep), const -> 0
                    स्थिरांक पूर्णांक vbits = kind[c]==CTRUE ? 3 : 0;  // V: always-true -> 255, else 0
                    patchFlags |= (mbits | (vbits << 2)) << (EW_CMP_PATCH_SHIFT + c*4);
                }
                TKernel kern = getElemwiseFunc(fam, base, base, EW_DEPTH_NONE, result);
                kern.flags |= EW_CMP_PATCH | patchFlags;
                स्थिरांक पूर्णांक out = addTemp(result);
                addInsn(fam, a, addConst(base, ts, cn), 0, out, kern);
                परत out;
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
    तार्किक divGuard = असत्य;
    निवड (op)
    {
    स्थिती OP_MIN: स्थिती OP_MAX:
    स्थिती OP_AND: स्थिती OP_OR: स्थिती OP_XOR:
        depth = result; wide = result; थांबा;
    स्थिती OP_ABSDIFF:
    {
        // operands meet at the integer-preserving common type; |a-b| of a signed integer needs the
        // UNSIGNED type of the same width (8s->8u, ...), which is both the result and the safe type.
        पूर्णांक common = (nd0 == EW_DEPTH_NONE) ? nd1
                   : (nd1 == EW_DEPTH_NONE) ? nd0 : promoteArith(nd0, nd1);
        जर (common == EW_DEPTH_NONE) common = result;          // both operands flexible consts
        depth = common;
        wide  = absdiffResultDepth(common);
        जर (rdepth == EW_DEPTH_NONE) result = wide;            // auto result: unsigned same width
        थांबा;
    }
    स्थिती OP_CMP_EQ: स्थिती OP_CMP_NE: स्थिती OP_CMP_LT:
    स्थिती OP_CMP_LE: स्थिती OP_CMP_GT: स्थिती OP_CMP_GE:
        // compare in the common operand type; if a flexible-const threshold does not FIT that type
        // (out of range, e.g. u8 > -10, or fractional, e.g. u8 > 2.5) fall back to f64 so it is NOT
        // saturated into the operand type (which would move the boundary). f64 is exact for every
        // integer array up to 32-bit; only a 64-bit-int array vs an out-of-range threshold stays
        // approximate (a pathological case). array-vs-array never has a const, so it stays integer.
        depth = base; wide = CV_64F; थांबा;
    स्थिती OP_MUL: स्थिती OP_DIV:
    {
        depth = base;
        स्थिरांक तार्किक w = (depth==CV_32U || depth==CV_32S || depth==CV_64U ||
                        depth==CV_64S || depth==CV_64F);
        wide = w ? CV_64F : CV_32F;
        जर (op == OP_DIV)
        {
            // guard b==0 -> 0 when BOTH operands are integer (matches cv::divide); a flexible const
            // counts as integer iff its value is integral. promote2 can float two wide ints, so this
            // must read the ORIGINAL operands, not `depth`.
            स्वतः intOperand = [&](पूर्णांक s, पूर्णांक nd) {
                जर (nd != EW_DEPTH_NONE) परत !isFloatDepth(nd);
                AutoBuffer<द्विगुण, MAX_LOCAL_CN> v; constDoubles(*हे, s, v);
                परत v[0] == std::न्यूनमान(v[0]); };
            divGuard = intOperand(a, nd0) && intOperand(b, nd1);
        }
        थांबा;
    }
    स्थिती OP_POW: स्थिती OP_HYPOT: स्थिती OP_ATAN2:
        depth = (base == CV_64F) ? CV_64F : CV_32F; wide = depth; थांबा;
    पूर्वनिर्धारित:        // OP_ADD, OP_SUB
        depth = base; wide = safeWide(depth); थांबा;
    }

    // a flexible const that can't be represented exactly at `depth` (e.g. 2.5 in a u8 op) forces the
    // wide working path; a NON-INTEGRAL const additionally forces a FLOAT compute (an integer wide
    // would quantize the fraction - e.g. add(u8, 1.7) must not round 1.7 to 2).
    स्वतः fracConst = [&](पूर्णांक s) {
        जर (!isFlexConst(*हे, s)) परत असत्य;
        AutoBuffer<द्विगुण, MAX_LOCAL_CN> v; पूर्णांक n = constDoubles(*हे, s, v);
        साठी (पूर्णांक ch = 0; ch < n; ch++)
            जर (v[ch] != std::न्यूनमान(v[ch])) परत सत्य;
        परत असत्य;
    };
    // BITWISE never widens or floats: a bit-pattern op keeps the array's own integer type, and an
    // out-of-range/fractional scalar is just saturate/round-cast into it (matching scalarToRawData /
    // cv::bitwise's classic convertAndUnrollScalar). Skipping the widening below leaves depth == result.
    जर (cat != CAT_BITWISE && (!constFits(*हे, a, depth) || !constFits(*हे, b, depth)))
    {
        depth = wide;
        स्थिरांक तार्किक frac = fracConst(a) || fracConst(b);
        // MIN/MAX just SELECT an operand, so a fractional scalar threshold is round-cast into the
        // array type (min(u8, 3.7) == min(u8, 4)) - matching the classic cv::min/max - instead of
        // promoting the whole op to float (add/sub/absdiff DO need the float path to keep the fraction).
        जर (frac && !isFloatDepth(depth) && op != OP_MIN && op != OP_MAX)
            depth = wide = (result == CV_64F || base == CV_64F) ? CV_64F : CV_32F;
        // mul/div carry a scale-like float const at full f64 precision (cv::multiply/divide compute in
        // f64), so don't settle for f32 - e.g. 110 * 147.2863... must round to 16201, not 16202.
        जर (frac && (op == OP_MUL || op == OP_DIV) && depth == CV_32F)
            depth = wide = CV_64F;
    }

    पूर्णांक s0 = isFlexConst(*हे, a) ? typedConstFrom(a, depth) : a;
    पूर्णांक s1 = isFlexConst(*हे, b) ? typedConstFrom(b, depth) : b;
    पूर्णांक c0 = maybeAddCast(s0, depth), c1 = maybeAddCast(s1, depth);

    // emit `depth -> result` directly when a kernel exists, else compute in `wide` and cast down.
    // OP_DIV resolves via getDivFunc (carrying the /0 guard); every other op via getElemwiseFunc.
    स्वतः resolve = [&](पूर्णांक rd) {
        परत op == OP_DIV ? getDivFunc(depth, rd, divGuard)
                            : getElemwiseFunc(op, depth, depth, EW_DEPTH_NONE, rd);
    };
    TKernel k = resolve(result);
    जर (k.fptr)
    {
        पूर्णांक res = addTemp(result);
        addInsn(op, c0, c1, 0, res, k, params);
        परत res;
    }
    k = resolve(wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णांक w = addTemp(wide);
    addInsn(op, c0, c1, 0, w, k, params);
    परत maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitUnary(): type policy for a 1-input op. MATH ops (sqrt/exp/...) compute in the float
// domain; NEG/ABS keep the operand type; NOT stays integer. (OP_CAST is not routed here - an explicit
// cast is just maybeAddCast / a typed addConst at the call site.)
// ---------------------------------------------------------------------------
पूर्णांक TExpr::emitUnary(शीर्ष op, पूर्णांक a, पूर्णांक rdepth, स्थिरांक Scalar& params)
{
    स्थिरांक पूर्णांक nd = isFlexConst(*हे, a) ? EW_DEPTH_NONE : arginfo[a].depth;
    स्थिरांक ElemwiseCategory cat = opCategory(op);

    // NEG and ABS have no kernels of their own - they are compositions over the binary family
    // with a zero constant (flexible, so emitBinary types it as the operand's own type).
    जर (op == OP_NEG)
        परत emit_द्विमान(OP_SUB, addConst(EW_DEPTH_NONE, Scalar(0.), 1), a, rdepth, params);
    जर (op == OP_ABS)
    {
        // peephole: abs(x - y) -> absdiff(x, y), ALWAYS. Strictly speaking the two differ on
        // integers - the literal subtract saturates first (u8: max(x-y, 0); signed: clipped
        // difference), absdiff computes the true |x - y| - but whoever writes abs(a - b) MEANS
        // absdiff; the saturation artifacts are never the desired result. So we deliberately
        // "don't notice" the difference and hand out the useful semantics. The sub is necessarily
        // the last instruction and its result the last temp (abs is emitted right after its
        // argument) - retire both, the moveToOutput manoeuvre.
        जर (!prog.empty() && arginfo[a].kind == TEMP && !arginfo[a].pinned &&
            prog.back().op == OP_SUB && prog.back().result == a &&
            arginfo[a].index == ntemps - 1)
        {
            स्थिरांक पूर्णांक x = prog.back().arg0, y = prog.back().arg1;
            prog.pop_back();
            arginfo[a].kind = NONE;
            ntemps--;
            परत emit_द्विमान(OP_ABSDIFF, x, y, rdepth, params);
        }
        // abs IS absdiff(a, 0), including the auto result type: a signed |a| lands in the
        // UNSIGNED type of the same width (|-128| = 128 fits u8 exactly; pinning the result to
        // the signed operand type would saturate it to 127). Fully uniform with the peephole.
        परत emit_द्विमान(OP_ABSDIFF, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), rdepth, params);
    }

    पूर्णांक result = rdepth;
    जर (result == EW_DEPTH_NONE)
    {
        निवड (cat)
        {
        // math is T -> T over the float depths (f16/bf16/f32/f64 kernels exist natively);
        // an integer input computes - and lands - in the float domain
        स्थिती CAT_MATH:    result = isFloatDepth(nd) ? nd : CV_32F; थांबा;
        स्थिती CAT_BITWISE: result = (nd == EW_DEPTH_NONE) ? CV_32S : nd; थांबा;  // NOT
        पूर्वनिर्धारित:          result = (nd == EW_DEPTH_NONE) ? CV_32F : nd; थांबा;  // NEG, ABS
        }
    }

    पूर्णांक depth, wide;
    निवड (cat)
    {
    स्थिती CAT_MATH:
        // T == result over a float depth => the native kernel (incl. the f16/bf16 in-kernel f32
        // hub - no materialized f32 temps). Everything else computes in f32/f64 and casts.
        depth = (isFloatDepth(nd) && nd == result) ? nd
              : (nd == CV_64F || result == CV_64F) ? CV_64F : CV_32F;
        wide = depth;
        थांबा;
    स्थिती CAT_BITWISE: depth = result; wide = result; थांबा;
    पूर्वनिर्धारित:          depth = (nd == EW_DEPTH_NONE) ? result : nd; wide = result; थांबा;  // NEG/ABS
    }

    जर (!constFits(*हे, a, depth)) depth = (cat == CAT_MATH) ? depth : CV_32F;
    पूर्णांक s0 = isFlexConst(*हे, a) ? typedConstFrom(a, depth) : a;
    पूर्णांक c0 = maybeAddCast(s0, depth);

    TKernel k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, result);
    जर (k.fptr)
    {
        पूर्णांक res = addTemp(result);
        addInsn(op, c0, 0, 0, res, k, params);
        परत res;
    }
    k = getElemwiseFunc(op, depth, EW_DEPTH_NONE, EW_DEPTH_NONE, wide);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णांक w = addTemp(wide);
    addInsn(op, c0, 0, 0, w, k, params);
    परत maybeAddCast(w, result);
}

// ---------------------------------------------------------------------------
// TExpr::emitTernary(): clamp(x,lo,hi) and select(mask,x,y). clamp brings all three data operands to
// a common type and emits there; select keeps arg0 (the mask) untouched and only unifies the two
// branches. (Both are minimal: not yet test-covered, no kernels wired up.)
// ---------------------------------------------------------------------------
पूर्णांक TExpr::emitTernary(शीर्ष op, पूर्णांक a, पूर्णांक b, पूर्णांक c, पूर्णांक rdepth)
{
    स्वतः dep = [&](पूर्णांक s) { परत isFlexConst(*हे, s) ? EW_DEPTH_NONE : arginfo[s].depth; };

    जर (op == OP_SELECT)                         // select(mask=a, x=b, y=c)
    {
        // the kernel consumes a 1-byte mask as-is (u8/s8/bool, mask != 0 semantics). Any other
        // mask type - a wider array or a literal - is normalized by an explicit `mask != 0`
        // compare (u8 result), NOT by a value cast (which would saturate/round the values).
        जर (isFlexConst(*हे, a) || CV_ELEM_SIZE1(arginfo[a].depth) != 1)
            a = emit_द्विमान(OP_CMP_NE, a, addConst(EW_DEPTH_NONE, Scalar(0.), 1), CV_8U, Scalar());
        पूर्णांक result = rdepth != EW_DEPTH_NONE ? rdepth : promoteArith(dep(b), dep(c));
        जर (result == EW_DEPTH_NONE) result = CV_32F;
        // a literal branch (select(m, x, 0)) is a flexible const: type it at `result` directly
        // (a value conversion in constbuf), never through an OP_CAST of a depth-less slot
        पूर्णांक sb = isFlexConst(*हे, b) ? typedConstFrom(b, result) : maybeAddCast(b, result);
        पूर्णांक sc = isFlexConst(*हे, c) ? typedConstFrom(c, result) : maybeAddCast(c, result);
        पूर्णांक res = addTemp(result);
        addInsn(OP_SELECT, a, sb, sc, res);
        परत res;
    }

    // clamp(x,lo,hi): unify all three operands at `result` (compute == result, no wide fallback).
    // The x operand dominates the auto type: clamp(u8_img, 10, 200) must stay u8, and literal
    // bounds are flexible consts typed at `result` directly (never OP_CAST of a depth-less slot).
    पूर्णांक result = rdepth != EW_DEPTH_NONE ? rdepth
                                         : promoteArith(promoteArith(dep(a), dep(b)), dep(c));
    जर (result == EW_DEPTH_NONE) result = CV_32F;
    स्वतः typed = [&](पूर्णांक s) {
        परत isFlexConst(*हे, s) ? typedConstFrom(s, result) : maybeAddCast(s, result);
    };
    पूर्णांक c0 = typed(a), c1 = typed(b), c2 = typed(c);
    TKernel k = getElemwiseFunc(op, result, result, result, result);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    पूर्णांक res = addTemp(result);
    addInsn(op, c0, c1, c2, res, k);
    परत res;
}

// ---------------------------------------------------------------------------
// TExpr::moveToOutput(): land `temp` in the existing slot `out`. Prefer redirecting `temp`'s single
// producer to write `out` directly (no copy), dropping `temp` when it was the last-added slot so
// ntemps stays minimal (keeps compile()'s no-temp fast exit for single-op programs). Otherwise copy.
// ---------------------------------------------------------------------------
पूर्णांक TExpr::moveToOutput(पूर्णांक temp, पूर्णांक out)
{
    स्थिरांक पूर्णांक ninsn = (पूर्णांक)prog.size();
    पूर्णांक producer = -1;
    तार्किक usedAsArg = असत्य;
    साठी (पूर्णांक i = 0; i < ninsn; i++)
    {
        स्थिरांक TExpr::Insn& ins = prog[i];
        जर (ins.result == temp) producer = i;
        जर (ins.arg0 == temp || ins.arg1 == temp || ins.arg2 == temp) usedAsArg = सत्य;
    }
    जर (arginfo[temp].kind == TEMP && producer >= 0 && !usedAsArg && !arginfo[temp].pinned &&
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
        परत out;
    }
    addInsn(OP_CAST, temp, 0, 0, out);                     // same/different depth copy into `out`
    परत out;
}

// ---------------------------------------------------------------------------
// TExpr::output(): declare a fresh OUTPUT of `rootSlot`'s depth and moveToOutput into it.
// ---------------------------------------------------------------------------
पूर्णांक TExpr::output(पूर्णांक मूळ_Slot)
{
    परत moveToOutput(मूळ_Slot, addOutput(arginfo[मूळ_Slot].depth));
}

// ---------------------------------------------------------------------------
// TExpr::compile(): finalize the already-typed program. Bind every instruction's kernel (skipping
// any whose pointer was pre-set, e.g. div's caller-known /0 policy) and pack the temps into a
// minimal set of reusable physical buffers (liveness). All per-arg bookkeeping here is transient.
// ---------------------------------------------------------------------------
शून्य TExpr::compile()
{
    स्थिरांक पूर्णांक ninsn = (पूर्णांक)prog.size();

    // Kernels are bound eagerly by addInsn at build time, so there is no binding pass here - compile()
    // only counts consts and packs temp buffers.

    // Materialize consts: convert each live CONST's source values (still in `srcdepth` at constofs)
    // to its resolved `depth`, appending the result to constbuf; constofs then points at the converted
    // values and srcdepth becomes the resolved depth. Flexible literals (depth==NONE) are dead - the
    // emit* layers replaced them with typed copies - and are skipped. nconsts sizes exec's per-const
    // header buffer and gates the fast path. (getConvertFunc may reallocate constbuf, so snapshot the
    // source bytes first.)
    स्थिरांक पूर्णांक nslots = (पूर्णांक)arginfo.size();
    nconsts = 0;
    साठी (पूर्णांक s = 1; s < nslots; s++)
    {
        Arg& a = arginfo[s];
        जर (a.kind != CONST || a.depth == EW_DEPTH_NONE) सुरूठेवा;
        स्थिरांक पूर्णांक cn = std::max(1, a.channels), sd = a.srcdepth, dd = a.depth;
        स्थिरांक माप_प्रकार sesz = CV_ELEM_SIZE1(sd), desz = CV_ELEM_SIZE1(dd);
        // uint64_t element type is used for buffer alignment on worst case
        AutoBuffer<uint64_t, आकार(uint64_t)> srcbuf(divUp((माप_प्रकार)cn * sesz, (अचिन्हित)आकार(uint64_t)));
        uchar* srcbytes = (uchar*)srcbuf.data();
        स_नकल(srcbytes, (स्थिरांक uchar*)(constbuf.data() + a.constofs), (माप_प्रकार)cn * sesz);
        स्थिरांक माप_प्रकार ofs = appendConstBuf(constbuf, dd, nullptr, cn);   // reserve converted region
        uchar* dst = (uchar*)(constbuf.data() + ofs);
        जर (sd == dd) स_नकल(dst, srcbytes, (माप_प्रकार)cn * desz);
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
    capElems = पूर्णांक_उच्च;                       // no temps => exec runs each tile in one block, no scratch
    जर (ntemps == 0) परत;

    AutoBuffer<पूर्णांक, 32> tempOfSlot(nslots);
    साठी (पूर्णांक s = 0; s < nslots; s++) tempOfSlot[s] = -1;
    साठी (पूर्णांक s = 1; s < nslots; s++)
        जर (arginfo[s].kind == TEMP) tempOfSlot[s] = arginfo[s].index;

    AutoBuffer<पूर्णांक, 16> lastUse(ntemps);
    साठी (पूर्णांक t = 0; t < ntemps; t++) lastUse[t] = -1;
    साठी (पूर्णांक i = 0; i < ninsn; i++)
    {
        स्थिरांक TExpr::Insn& ins = prog[i];
        पूर्णांक as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        साठी (पूर्णांक k = 0; k < 3; k++)
            जर (tempOfSlot[as[k]] >= 0) lastUse[tempOfSlot[as[k]]] = i;
    }

    bufferOfTemp.resize(ntemps);
    साठी (पूर्णांक t = 0; t < ntemps; t++) bufferOfTemp[t] = -1;
    AutoBuffer<पूर्णांक, 16> freeBufs(ntemps);
    पूर्णांक nfree = 0, nbuf = 0;
    साठी (पूर्णांक i = 0; i < ninsn; i++)
    {
        स्थिरांक TExpr::Insn& ins = prog[i];
        पूर्णांक rt = tempOfSlot[ins.result];
        जर (rt >= 0 && bufferOfTemp[rt] < 0)
            bufferOfTemp[rt] = nfree > 0 ? freeBufs[--nfree] : nbuf++;
        पूर्णांक as[3] = { ins.arg0, ins.arg1, ins.arg2 };
        साठी (पूर्णांक k = 0; k < 3; k++)
        {
            पूर्णांक t = tempOfSlot[as[k]];
            जर (t >= 0 && lastUse[t] == i)
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
    साठी (पूर्णांक b = 0; b <= nbuffers; b++) bufEszPrefix[b] = 0;
    साठी (पूर्णांक s = 1; s < nslots; s++)
        जर (arginfo[s].kind == TEMP)
        {
            पूर्णांक b = bufferOfTemp[arginfo[s].index], e = (पूर्णांक)CV_ELEM_SIZE1(arginfo[s].depth);
            जर (e > bufEszPrefix[b + 1]) bufEszPrefix[b + 1] = e;     // max elem size in buffer b
        }
    साठी (पूर्णांक b = 0; b < nbuffers; b++) bufEszPrefix[b + 1] += bufEszPrefix[b];   // -> prefix sums

    // L1 fragment cap: # elements one ~4KB scratch fragment holds (exec fragments the strip so the
    // intermediates stay hot). Pure function of the temp byte layout, hence computed here once.
    स्थिरांक पूर्णांक totalEsz = bufEszPrefix[nbuffers];
    capElems = totalEsz > 0 ? std::max(64, (4 * 1024) / totalEsz) : पूर्णांक_उच्च;
}


// ============================ executor + builders (was ew_exec.cpp) ============================

// Logical shape of a Mat with channels as the innermost dimension; steps in elemsize1 units.
// Used here only to infer the broadcast RESULT shape (spatial + channels) for output allocation;
// broadcastOp does its own channel-aware layout for the traversal itself.
स्थिर शून्य matLogical(स्थिरांक Mat& m, MatShape& shp, EwSteps& step, पूर्णांक& esz1)
{
    esz1 = (पूर्णांक)m.elemSize1();
    पूर्णांक nd = m.dims, cn = m.channels();
    shp.resize(nd + 1);
    साठी (पूर्णांक i = 0; i < nd; i++)
    {
        shp[i] = m.size[i];
        step[i] = m.step[i] / esz1;
    }
    shp[nd] = cn;
    step[nd] = 1;
}

// numpy-style broadcast of several right-aligned shapes.
स्थिर तार्किक broadcastShape(स्थिरांक MatShape* shps, पूर्णांक K, MatShape& out)
{
    पूर्णांक nd = 0;
    साठी (पूर्णांक k = 0; k < K; k++) nd = std::max(nd, shps[k].dims);
    out.assign(nd, 1);
    साठी (पूर्णांक k = 0; k < K; k++)
    {
        स्थिरांक MatShape& s = shps[k];
        पूर्णांक off = nd - s.dims;
        साठी (पूर्णांक i = 0; i < s.dims; i++)
        {
            पूर्णांक d = s[i], &o = out[off + i];
            जर (o == 1) o = d;
            अन्यथा जर (d != 1 && d != o) परत असत्य;
        }
    }
    परत सत्य;
}

// Rough per-element cost of an op for the parallel_for_ stripe-count hint, in units of ~1/4 cycle
// per element of the VECTORIZED kernel (measured on the SIMD paths; e.g. the atan2/exp polynomials
// run at ~1.5 cycles/element, not the ~30 a scalar-cost model would suggest - the old values
// over-split the work into stripes too small to amortize the per-job dispatch overhead).
// Unknown ops default to ~division. cv::expression will own this once it drives broadcastOp.
स्थिर पूर्णांक opCost(शीर्ष op)
{
    निवड (op)
    {
    स्थिती OP_ADD: स्थिती OP_SUB: स्थिती OP_MUL: स्थिती OP_MIN: स्थिती OP_MAX:
    स्थिती OP_ABSDIFF: स्थिती OP_AND: स्थिती OP_OR: स्थिती OP_XOR: स्थिती OP_NOT:
    स्थिती OP_NEG: स्थिती OP_ABS: स्थिती OP_CAST: स्थिती OP_RELU: स्थिती OP_SELECT:
    स्थिती OP_CMP_EQ: स्थिती OP_CMP_NE: स्थिती OP_CMP_LT:
    स्थिती OP_CMP_LE: स्थिती OP_CMP_GT: स्थिती OP_CMP_GE: परत 1;
    स्थिती OP_DIV: स्थिती OP_SQRT: स्थिती OP_HYPOT: स्थिती OP_CONVERT_SCALE: परत 3;
    स्थिती OP_SIN: स्थिती OP_COS: स्थिती OP_TANH: स्थिती OP_ERF: स्थिती OP_ATAN2:
    स्थिती OP_EXP: स्थिती OP_LOG: स्थिती OP_POW:           परत 6;
    पूर्वनिर्धारित:                                         परत 3;
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
शून्य TExpr::dump(std::ostream& os) स्थिरांक
{
    स्वतः dn = [](पूर्णांक d) -> स्थिरांक अक्षर* {
        निवड (d) {
        स्थिती EW_DEPTH_NONE: परत "flex";
        स्थिती CV_8U:  परत "u8";  स्थिती CV_8S:  परत "s8";
        स्थिती CV_16U: परत "u16"; स्थिती CV_16S: परत "s16";
        स्थिती CV_32U: परत "u32"; स्थिती CV_32S: परत "s32";
        स्थिती CV_64U: परत "u64"; स्थिती CV_64S: परत "s64";
        स्थिती CV_16F: परत "f16"; स्थिती CV_16BF:परत "bf16";
        स्थिती CV_32F: परत "f32"; स्थिती CV_64F: परत "f64";
        स्थिती CV_Bool:परत "bool";
        पूर्वनिर्धारित:     परत "?"; }
    };
    स्वतः kn = [](ArgKind k) -> स्थिरांक अक्षर* {
        निवड (k) { स्थिती NONE: परत "none"; स्थिती आदान: परत "in"; स्थिती CONST: परत "const";
                     स्थिती TEMP: परत "temp"; स्थिती OUTPUT: परत "out"; } परत "?";
    };
    os << "TExpr: inputs=" << ninputs << " outputs=" << noutputs << " temps=" << ntemps
       << " buffers=" << nbuffers << " consts=" << nconsts << " insns=" << (पूर्णांक)prog.size() << "\n";
    os << "  slots:\n";
    साठी (पूर्णांक s = 0; s < (पूर्णांक)arginfo.size(); s++) {
        स्थिरांक Arg& a = arginfo[s];
        os << "    [" << s << "] " << kn(a.kind);
        जर (a.kind != NONE) os << " " << dn(a.depth);
        जर (a.kind == आदान || a.kind == OUTPUT || a.kind == TEMP) os << " idx=" << a.index;
        जर (a.kind == CONST) {
            स्थिरांक पूर्णांक cn = std::max(1, a.channels);
            AutoBuffer<द्विगुण, MAX_LOCAL_CN> v(cn);
            स्थिरांक uchar* src = (स्थिरांक uchar*)(constbuf.data() + a.constofs);
            जर (a.srcdepth == CV_64F) स_नकल(v.data(), src, (माप_प्रकार)cn * आकार(द्विगुण));
            अन्यथा getConvertFunc(a.srcdepth, CV_64F)(src, 0, nullptr, 0, (uchar*)v.data(), 0, Size(cn, 1), nullptr);
            os << " cn=" << cn << " src=" << dn(a.srcdepth) << " {";
            साठी (पूर्णांक c = 0; c < cn; c++) os << (c ? "," : "") << v[c];
            os << "}";
        }
        os << "\n";
    }
    os << "  prog:\n";
    साठी (पूर्णांक i = 0; i < (पूर्णांक)prog.size(); i++) {
        स्थिरांक Insn& ins = prog[i];
        os << "    " << i << ": " << opName(ins.op) << "(" << ins.arg0;
        जर (ins.arg1) os << ", " << ins.arg1;
        जर (ins.arg2) os << ", " << ins.arg2;
        os << ") -> " << ins.result;
        जर (ins.kernel.flags) os << " kflags=" << ins.kernel.flags;
        जर (ins.params[0] != 1.0 || ins.params[1] != 0.0)
            os << " params=[" << ins.params[0] << "," << ins.params[1] << "," << ins.params[2] << "]";
        जर (!ins.kernel.fptr) os << " [UNBOUND]";
        os << "\n";
    }
}

// ---- program builders: append a slot / instruction, return its index ----
पूर्णांक TExpr::add_आदान(पूर्णांक depth)
{
    Arg a; a.kind = आदान; a.depth = depth; a.index = ninputs++;
    arginfo.push_back(a); परत (पूर्णांक)arginfo.size() - 1;
}

पूर्णांक TExpr::addOutput(पूर्णांक depth)
{
    Arg a; a.kind = OUTPUT; a.depth = depth; a.index = noutputs++;
    arginfo.push_back(a); परत (पूर्णांक)arginfo.size() - 1;
}

पूर्णांक TExpr::addTemp(पूर्णांक depth)
{
    Arg a; a.kind = TEMP; a.depth = depth; a.index = ntemps++;
    arginfo.push_back(a); परत (पूर्णांक)arginfo.size() - 1;
}

// Source = a cv::Scalar (f64, up to 4 channels): stored as CV_64F in constbuf.
पूर्णांक TExpr::addConst(पूर्णांक depth, स्थिरांक Scalar& v, पूर्णांक channels)
{
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = CV_64F;
    a.constofs = appendConstBuf(constbuf, CV_64F, v.val, channels);
    arginfo.push_back(a); परत (पूर्णांक)arginfo.size() - 1;
}

// Source = native bytes of any depth/channel-count (e.g. a Vec<_,N> scalar): stored as-is.
पूर्णांक TExpr::addConst(पूर्णांक depth, पूर्णांक srcdepth, स्थिरांक शून्य* data, पूर्णांक channels)
{
    // A CONST is a broadcast scalar - capped at 4 channels (a Scalar). Need more? Pass a 0-D Mat with
    // the desired channel count as an INPUT: broadcasting handles it, and it isn't limited to 4.
    CV_Assert(channels <= 4);
    Arg a; a.kind = CONST; a.depth = depth; a.channels = channels; a.srcdepth = srcdepth;
    a.constofs = appendConstBuf(constbuf, srcdepth, data, channels);
    arginfo.push_back(a); परत (पूर्णांक)arginfo.size() - 1;
}

// A typed copy of flexible CONST `srcSlot` at the resolved `depth`, sharing its (still-source)
// values in constbuf; compile() converts each such slot's values to its `depth`.
पूर्णांक TExpr::typedConstFrom(पूर्णांक srcSlot, पूर्णांक depth)
{
    स्थिरांक Arg& src = arginfo[srcSlot];
    Arg a; a.kind = CONST; a.depth = depth; a.channels = src.channels;
    a.srcdepth = src.srcdepth; a.constofs = src.constofs;    // shares the source region
    arginfo.push_back(a); परत (पूर्णांक)arginfo.size() - 1;
}

// Append one instruction with a pre-resolved kernel (the caller probed getElemwiseFunc, or knows
// the kernel - e.g. div's /0-aware kernel). No re-resolution.
पूर्णांक TExpr::addInsn(शीर्ष op, पूर्णांक a0, पूर्णांक a1, पूर्णांक a2, पूर्णांक result, स्थिरांक TKernel& kernel, स्थिरांक Scalar& params)
{
    TExpr::Insn ins; ins.op = op; ins.arg0 = a0; ins.arg1 = a1; ins.arg2 = a2; ins.result = result;
    ins.params = params; ins.kernel = kernel;
    prog.push_back(ins);
    परत (पूर्णांक)prog.size() - 1;
}

// Append one instruction, resolving its kernel NOW from the operand/result depths (final at build
// time). compile() therefore never re-resolves. A builder that needs a specific kernel (div's /0
// policy) pushes a pre-bound Insn directly instead.
पूर्णांक TExpr::addInsn(शीर्ष op, पूर्णांक a0, पूर्णांक a1, पूर्णांक a2, पूर्णांक result, स्थिरांक Scalar& params)
{
    पूर्णांक d0 = a0 ? arginfo[a0].depth : EW_DEPTH_NONE;
    पूर्णांक d1 = a1 ? arginfo[a1].depth : EW_DEPTH_NONE;
    पूर्णांक d2 = a2 ? arginfo[a2].depth : EW_DEPTH_NONE;
    TKernel k = getElemwiseFunc(op, d0, d1, d2, arginfo[result].depth);
    CV_Assert(k.fptr && "ew: no kernel for this op/type combination");
    परत addInsn(op, a0, a1, a2, result, k, params);
}

// Cast `arg` to `depth` only if it is not already that depth (the sole cast-insertion helper for
// the hand builders and the emit* layers); returns the slot holding the value at `depth`.
पूर्णांक TExpr::maybeAddCast(पूर्णांक arg, पूर्णांक depth)
{
    जर (arginfo[arg].depth == depth) परत arg;
    पूर्णांक t = addTemp(depth);
    addInsn(OP_CAST, arg, 0, 0, t);
    परत t;
}

// Run one resolved instruction over a width x height tile. Every kernel uses ONE calling
// convention - the universal KernelFunc (element steps, the instruction's params block, plus
// kernel.flags/kernel.userdata). OP_CAST / OP_CONVERT_SCALE bind castKernel, which forwards to a
// core convert BinaryFunc (carried in kernel.userdata) over the distinct sub-region and then
// expands it across any broadcast axis (see castKernel/expandKernel in ew_kernels.cpp).
स्थिर ओळीत शून्य runInsn(स्थिरांक TExpr::Insn& ins,
                           स्थिरांक शून्य* p0, माप_प्रकार y0, माप_प्रकार x0,
                           स्थिरांक शून्य* p1, माप_प्रकार y1, माप_प्रकार x1,
                           स्थिरांक शून्य* p2, माप_प्रकार y2, माप_प्रकार x2,
                           शून्य* pr, माप_प्रकार yr, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक code = ins.kernel.fptr(p0, y0, x0, p1, y1, x1, p2, y2, x2, pr, yr, w, h,
                                ins.params.val, ins.kernel.flags, ins.kernel.userdata);
    CV_Assert(code >= 0);
}

संरचना EwBody {
    स्थिरांक TExpr::Insn* prog;
    // slot -> a NON-NEGATIVE index whose meaning is given by slotKind[s]:
    //   TExpr::INPUT / TExpr::OUTPUT : index into tile.slices (the broadcast operand list; consts,
    //                                 now 0-dim broadcast operands, are relabeled INPUT and live here)
    //   TExpr::TEMP               : physical temp-buffer id
    स्थिरांक पूर्णांक* slotMap;
    स्थिरांक चिन्हित अक्षर* slotKind; // [nslots] arginfo[s].kind, for resolving slotMap[s]
    स्थिरांक पूर्णांक* bufEszPrefix;  // [nbuffers+1] prefix sums of temp elem sizes; [nbuffers] = total
    पूर्णांक ninsn, nslots, nbuffers, capElems;   // capElems: L1 fragment element cap (0 if no temps)
};

शून्य TExpr::outputShape(स्थिरांक Mat* स्थिरांक* inputs, MatShape& spatial, पूर्णांक& channels) स्थिरांक
{
    // Fast path: every real input shares one shape + channel count => the result IS inputs[0]'s
    // (consts add no dims/channels). Otherwise re-broadcast the logical (spatial + channel) shapes.
    स्थिरांक पूर्णांक rcn0 = ninputs >= 1 ? inputs[0]->channels() : 1;
    तार्किक sameShape = ninputs >= 1;
    साठी (पूर्णांक i = 1; sameShape && i < ninputs; i++)
    {
        स्थिरांक Mat& a = *inputs[i];
        जर (a.dims != inputs[0]->dims || a.channels() != rcn0) sameShape = असत्य;
        अन्यथा साठी (पूर्णांक d = 0; d < a.dims; d++) जर (a.size[d] != inputs[0]->size[d]) { sameShape = असत्य; थांबा; }
    }
    जर (sameShape) { spatial = inputs[0]->size; channels = rcn0; परत; }

    AutoBuffer<MatShape, 16> bshapes(std::max(ninputs, 1));
    MatShape shp; EwSteps step; पूर्णांक esz1;
    साठी (पूर्णांक i = 0; i < ninputs; i++) { matLogical(*inputs[i], shp, step, esz1); bshapes[i] = shp; }
    MatShape full;
    CV_Assert(broadcastShape(bshapes.data(), ninputs, full) && "ew: inputs not broadcast-compatible");
    स्थिरांक पूर्णांक ndFull = (पूर्णांक)full.size();
    channels = full[ndFull - 1];
    spatial = full; spatial.resize(ndFull - 1);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
शून्य TExpr::outputShape(स्थिरांक Mat* inputs, MatShape& spatial, पूर्णांक& channels) स्थिरांक
{
    AutoBuffer<स्थिरांक Mat*, 16> ptrs(std::max(ninputs, 1));
    साठी (पूर्णांक i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
    outputShape(ptrs.data(), spatial, channels);
}

// At namespace scope, NOT inside exec(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
स्थिर constexpr पूर्णांक LOCAL_HDRS = 3;
स्थिर constexpr पूर्णांक LOCAL_CONSTS = 4;
स्थिर constexpr पूर्णांक LOCAL_OPS = 16;

शून्य TExpr::exec(स्थिरांक Mat* स्थिरांक* inputs, Mat* outputs)
{
    using BrTile = BroadcastOp::Tile;
    using BrSlice = BroadcastOp::Slice;
    स्थिरांक पूर्णांक nslots = (पूर्णांक)arginfo.size();
    CV_Assert(nslots >= 1 && arginfo[0].kind == TExpr::NONE);

    // ---- cheap fast path: a const-free program over small, same-shape, continuous arrays. Channels
    //      fold into one flat contiguous run, so the whole job is a flat (total x 1) strip - run the
    //      program directly here and skip the per-call prep loop, broadcastOp's geometry, 2D tiling
    //      and the parallel framework (all pure overhead at this size). Temps are allowed: their
    //      byte layout (bufEszPrefix) is known from compile(), so we size an L1 scratch in O(1) and
    //      walk the strip in L1-sized fragments (the intermediates stay hot). nconsts/nbuffers/
    //      bufEszPrefix come from compile(); the rest is a quick check of the args. ----
    जर (nconsts == 0 && ninputs >= 1)
    {
        constexpr माप_प्रकार EW_FASTPATH_MAX = 1u << 17;   // above this the tiled/parallel path wins
        स्थिरांक Mat& r = *inputs[0];
        स्थिरांक पूर्णांक rcn = r.channels();
        तार्किक ok = r.isContinuous();
        साठी (पूर्णांक i = 1; ok && i < ninputs; i++)
        {
            स्थिरांक Mat& a = *inputs[i];
            ok = ok && a.isContinuous() && a.channels() == rcn && a.size == r.size;
        }
        // the flat strip writes the output(s) contiguously too, so it can't serve a non-contiguous
        // (cropped-ROI) destination - those fall to the general strided path below (an empty output,
        // which exec will allocate contiguous, counts as continuous here).
        साठी (पूर्णांक s = 1; ok && s < nslots; s++)
            जर (arginfo[s].kind == OUTPUT) ok = ok && outputs[arginfo[s].index].isContinuous();
        स्थिरांक माप_प्रकार total = ok ? r.total() * rcn : 0;
        जर (ok && total == 0)
        {
            स्थिरांक MatShape rshape = r.size;
            साठी (पूर्णांक s = 1; s < nslots; s++)
                जर (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            परत;
        }
        जर (ok && total <= EW_FASTPATH_MAX)
        {
            स्थिरांक MatShape rshape = r.size;   // spatial dims (channels separate), as a MatShape
            साठी (पूर्णांक s = 1; s < nslots; s++)
                जर (arginfo[s].kind == OUTPUT)
                    outputs[arginfo[s].index].create(rshape, CV_MAKETYPE(arginfo[s].depth, rcn));
            स्थिरांक पूर्णांक ninsn = (पूर्णांक)prog.size();

            जर (nbuffers == 0)
            {
                // No temps (single-op add/sub/mul/...): one flat (total x 1) pass, operands point
                // straight at the Mat data - no scratch, no fragment loop. Referenced slots are INPUT/
                // OUTPUT (a moved-from NONE slot may exist but is never an instruction argument).
                स्वतः ptrOf = [&](पूर्णांक s) -> शून्य* {
                    जर (s <= 0) परत nullptr;
                    स्थिरांक Arg& ai = arginfo[s];
                    जर (ai.kind == आदान)  परत (शून्य*)inputs[ai.index]->data;
                    जर (ai.kind == OUTPUT) परत (शून्य*)outputs[ai.index].data;
                    परत nullptr;   // NONE
                };
                स्थिरांक पूर्णांक w = (पूर्णांक)total;
                साठी (पूर्णांक n = 0; n < ninsn; n++)
                {
                    स्थिरांक Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, w, 1);
                }
                परत;
            }

            // Temps present: walk the strip in L1-sized fragments so the intermediates stay hot.
            // Byte layout (bufEszPrefix) and the fragment cap (capElems) both come from compile();
            // scratch is totalEsz*wf0 (<= ~4KB).
            स्थिरांक पूर्णांक totalEsz = bufEszPrefix[nbuffers];
            स्थिरांक पूर्णांक wf0 = std::min((पूर्णांक)total, capElems);

            स्थिरांक माप_प्रकार region = alignSize((माप_प्रकार)wf0, 8);
            // Scratch for the temp buffers. AutoBuffer no longer value-inits its tail, so a fresh per-call
            // buffer is free (we only WRITE to it); the inline 4KB covers the L1-capped size, heap backs
            // the rare larger case. uint64_t is used for memory alignment on the worst case.
            AutoBuffer<uint64_t, (4*1024 + 256)/आकार(uint64_t)> scratchBuf(
                divUp((माप_प्रकार)totalEsz * region, (अचिन्हित)आकार(uint64_t)));
            uchar* scratch = (uchar*)scratchBuf.data();
            साठी (पूर्णांक x0 = 0; x0 < (पूर्णांक)total; x0 += wf0)
            {
                स्थिरांक पूर्णांक wf = std::min(wf0, (पूर्णांक)total - x0);
                स्वतः ptrOf = [&](पूर्णांक s) -> शून्य* {
                    जर (s <= 0) परत nullptr;
                    स्थिरांक Arg& ai = arginfo[s];
                    माप_प्रकार esz = CV_ELEM_SIZE1(ai.depth);
                    जर (ai.kind == आदान)  परत (uchar*)inputs[ai.index]->data  + (माप_प्रकार)x0 * esz;
                    जर (ai.kind == OUTPUT) परत (uchar*)outputs[ai.index].data + (माप_प्रकार)x0 * esz;
                    जर (ai.kind != TEMP)   परत nullptr;                                    // NONE: moved-from
                    स्थिरांक पूर्णांक b = bufferOfTemp.empty() ? ai.index : bufferOfTemp[ai.index];  // TEMP
                    परत scratch + (माप_प्रकार)bufEszPrefix[b] * region;                        // fragment-local
                };
                साठी (पूर्णांक n = 0; n < ninsn; n++)
                {
                    स्थिरांक Insn& ins = prog[n];
                    runInsn(ins, ptrOf(ins.arg0), 0, 1, ptrOf(ins.arg1), 0, 1,
                            ptrOf(ins.arg2), 0, 1, ptrOf(ins.result), 0, wf, 1);
                }
            }
            परत;
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
    तार्किक inplace = असत्य;
    साठी (पूर्णांक i = 0; i < ninputs && !inplace; i++)
        साठी (पूर्णांक j = 0; j < noutputs; j++)
            जर (inputs[i]->data && inputs[i]->data == outputs[j].data) { inplace = सत्य; थांबा; }
    पूर्णांक nhdrs = inplace ? ninputs : 0;

    AutoBuffer<स्थिरांक Mat*, LOCAL_OPS> arr(ninputs + noutputs + nconsts);
    AutoBuffer<Mat, LOCAL_HDRS> hdrs(std::max(nhdrs, 1));   // non-owning input headers (in-place incref)
    AutoBuffer<स्थिरांक Mat*, LOCAL_HDRS> inptr(std::max(nhdrs, 1));  // repointed input list (in-place only)
    AutoBuffer<Mat, LOCAL_CONSTS> constHdrBuf(std::max(nconsts, 1));  // 0-dim headers over constbuf
    AutoBuffer<पूर्णांक, LOCAL_OPS> slotMap(nslots);
    AutoBuffer<चिन्हित अक्षर, LOCAL_OPS> slotKind(nslots);
    पूर्णांक narr = 0, nc = 0;
    जर (inplace) {
        // save (incref) inputs in the case of in-place operation
        // to protect them from premature deallocation
        साठी (पूर्णांक j = 0; j < ninputs; j++) { hdrs[j] = *inputs[j]; inptr[j] = &hdrs[j]; }
        inputs = inptr.data();
    }
    slotKind[0] = (चिन्हित अक्षर)TExpr::NONE;
    साठी (पूर्णांक s = 1; s < nslots; s++)
    {
        स्थिरांक TExpr::Arg& ai = arginfo[s];
        slotKind[s] = (चिन्हित अक्षर)ai.kind;
        जर (ai.kind == TExpr::आदान) {
            slotMap[s] = narr;
            arr[narr++] = inputs[ai.index];
        }
        अन्यथा जर (ai.kind == TExpr::OUTPUT) {
            outputs[ai.index].create(spatial, CV_MAKETYPE(ai.depth, rchannels));
            slotMap[s] = narr; arr[narr++] = &outputs[ai.index];
        }
        अन्यथा जर (ai.kind == TExpr::CONST && ai.depth == EW_DEPTH_NONE) {
            slotKind[s] = (चिन्हित अक्षर)TExpr::NONE;   // dead flexible literal: never referenced
            slotMap[s] = 0;
        }
        अन्यथा जर (ai.kind == TExpr::CONST) {
            // a materialized const rides the broadcast machinery as a 0-dim, per-channel operand: a
            // multichannel const forces CH_DIM (per-channel scalars); a single value broadcasts
            // everywhere. compile() already converted its values (in constbuf) to ai.depth.
            स्थिरांक पूर्णांक c = std::max(1, ai.channels);
            constHdrBuf[nc] = Mat(MatShape::scalar(), CV_MAKETYPE(ai.depth, c),
                                  (शून्य*)(constbuf.data() + ai.constofs));
            slotKind[s] = (चिन्हित अक्षर)TExpr::आदान;   // to the body it is just an array operand
            slotMap[s] = narr; arr[narr++] = &constHdrBuf[nc];
            nc++;
        }
        अन्यथा जर (ai.kind == TExpr::TEMP) {
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
    स्थिरांक पूर्णांक ninsn = (पूर्णांक)prog.size();
    दीर्घ दीर्घ costPerElem = 0;
    साठी (पूर्णांक n = 0; n < ninsn; n++)
        costPerElem += opCost(prog[n].op);

    // ---- 4. parallel work hint: total output scalars x summed per-element op cost / budget. ----
    दीर्घ दीर्घ otot = (दीर्घ दीर्घ)rchannels;
    साठी (पूर्णांक d = 0; d < (पूर्णांक)spatial.size(); d++) otot *= spatial[d];
    जर (otot == 0)
        परत;
    // One stripe per 2^16 cost units. The unit is ~1/4 cycle of the VECTORIZED kernel, but
    // element-wise work is memory-bound, so a stripe costs several times what that model says -
    // a coarser budget leaves a 32-core box running 2 stripes on DNN-sized tensors (1-4 MB).
    // ~4 stripes per thread is plenty of granularity for element-wise work; more only multiplies
    // the per-job dispatch overhead (notably on the macOS/GCD backend). The absolute ceiling is
    // 32 pieces, EXCEPT on machines with many (heterogeneous) cores: there anything coarser than
    // ~3 pieces per worker makes the slow (efficiency) cores equal-share bottlenecks, so the
    // ceiling grows as 3*nthreads instead.
    // getNumThreads() is clamped from below: some backends may report 0 (WINRT, plugins).
    स्थिरांक द्विगुण T = (द्विगुण)std::max(get_अंक_Threads(), 1);
    स्थिरांक द्विगुण nstripes = std::min(
        (द्विगुण)otot * (द्विगुण)std::max<दीर्घ दीर्घ>(costPerElem, 1) * (1./ (द्विगुण)(1 << 16)),
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

    broadcastOp(arr.data(), narr, [&](स्थिरांक BrTile& tile)
    {
        EwBody& bc = body;
        स्थिरांक TExpr::Insn* insns = bc.prog;            // hot fields -> locals (registers/stack)
        स्थिरांक पूर्णांक* स्थिरांक smap  = bc.slotMap;
        स्थिरांक चिन्हित अक्षर* स्थिरांक skind = bc.slotKind;
        स्थिरांक पूर्णांक nin = bc.ninsn, nsl = bc.nslots;
        स्थिरांक पूर्णांक w = tile.width, h = tile.height;

        // Run the program over L1-sized 2D blocks so the intermediates stay in cache. ONE rule covers
        // every tile shape: keep the strip as WIDE as fits L1 (long inner loop => good SIMD + hits the
        // kernels' width-specific branches), then add as many rows as still fit (bw*bh <= capElems).
        // For the dominant 1D tile (w huge, h==1) this is a column strip; for a tall-thin tile
        // (w==channels, h huge - e.g. a masked op) it instead splits the LONG axis (height), so the
        // temp block stays contiguous and the kernels keep full width. With NO temps capElems==INT_MAX
        // => bw=w, bh=h: one block over the whole tile, region temp store is empty (zero bytes), and
        // each operand slot is re-pointed once straight at its tile slice. Each temp buffer occupies
        // [bufEszPrefix[buf]*region, ...) bytes in tstore; region = bw*bh.
        स्थिरांक पूर्णांक* स्थिरांक eszPrefix = bc.bufEszPrefix;
        स्थिरांक पूर्णांक bw = std::min(w, bc.capElems);
        स्थिरांक पूर्णांक bh = std::min(h, std::max(1, bc.capElems / std::max(1, bw)));
        स्थिरांक माप_प्रकार region = alignSize((माप_प्रकार)bw * bh, 8);
        // uint64_t elements (see the note on scratchBuf above): keeps the block-local temp store
        // 8-byte aligned, which the 64-bit-lane kernels require on 32-bit ABIs.
        AutoBuffer<uint64_t, (4*1024 + 256)/आकार(uint64_t)> tstoreBuf(
            divUp((माप_प्रकार)eszPrefix[bc.nbuffers] * region, (अचिन्हित)आकार(uint64_t)));  // inline (<= ~4KB)
        uchar* tstore = (uchar*)tstoreBuf.data();

        AutoBuffer<BrSlice, LOCAL_OPS> args(nsl);
        साठी (पूर्णांक y0 = 0; y0 < h; y0 += bh)
        {
            स्थिरांक पूर्णांक hf = std::min(bh, h - y0);
            साठी (पूर्णांक x0 = 0; x0 < w; x0 += bw)
            {
                स्थिरांक पूर्णांक wf = std::min(bw, w - x0);
                साठी (पूर्णांक s = 1; s < nsl; s++)
                {
                    BrSlice& a = args[s];
                    स्थिरांक पूर्णांक k = skind[s];
                    माप_प्रकार esz = CV_ELEM_SIZE1(arginfo[s].depth);
                    जर (k == TExpr::TEMP)                      // contiguous block-local buffer
                    {
                        a.ptr = tstore + (माप_प्रकार)eszPrefix[smap[s]] * region;
                        a.stepy = (माप_प्रकार)wf*esz; a.stepx = 1;
                    }
                    अन्यथा                                    // array operand (incl. 0-dim consts): this
                                                            // block of the broadcast slice
                    {
                        स्थिरांक BrSlice& sl = tile.slices[smap[s]];
                        a.ptr = (uchar*)sl.ptr +
                            ((माप_प्रकार)y0 * sl.stepy + (माप_प्रकार)x0 * sl.stepx) * esz;
                        a.stepy = sl.stepy*esz; a.stepx = sl.stepx;
                    }
                }

                साठी (पूर्णांक n = 0; n < nin; n++)
                {
                    स्थिरांक TExpr::Insn& ins = insns[n];
                    स्थिरांक BrSlice& a0 = args[ins.arg0]; स्थिरांक BrSlice& a1 = args[ins.arg1];
                    स्थिरांक BrSlice& a2 = args[ins.arg2]; स्थिरांक BrSlice& rr = args[ins.result];
                    runInsn(ins, a0.ptr, a0.stepy, a0.stepx, a1.ptr, a1.stepy, a1.stepx,
                            a2.ptr, a2.stepy, a2.stepx, (शून्य*)rr.ptr, rr.stepy, wf, hf);
                }
            }
        }
    }, सत्य, nstripes);
}

// Convenience overload: inputs as a contiguous Mat array -> build the pointer array + forward.
शून्य TExpr::exec(स्थिरांक Mat* inputs, Mat* outputs)
{
    AutoBuffer<स्थिरांक Mat*, 16> ptrs(std::max(ninputs, 1));
    साठी (पूर्णांक i = 0; i < ninputs; i++) ptrs[i] = &inputs[i];
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
शून्य make_द्विमान_Arith_Program(TExpr& p, शीर्ष op, पूर्णांक depth0, पूर्णांक depth1, पूर्णांक rdepth,
                            पूर्णांक maskDepth, द्विगुण scale)
{
    p.clear();
    जर (rdepth < 0)                                          // rdepth == -1 => auto (like cv::'s dtype=-1)
    {
        जर (opCategory(op) == CAT_COMPARE)
            rdepth = CV_8U;                                  // compare -> u8 mask
        अन्यथा
        {
            rdepth = promoteArith(depth0, depth1);
            जर (op == OP_ABSDIFF) rdepth = absdiffResultDepth(rdepth);   // signed |a-b| -> unsigned same width
        }
    }
    स्थिरांक तार्किक masked = maskDepth != EW_DEPTH_NONE;
    पूर्णांक sIn0  = p.add_आदान(depth0);
    पूर्णांक sIn1  = p.add_आदान(depth1);
    पूर्णांक sMask = masked ? p.add_आदान(maskDepth) : 0;
    पूर्णांक sOut  = p.addOutput(rdepth);

    पूर्णांक r = p.emit_द्विमान(op, sIn0, sIn1, rdepth, Scalar(scale));
    जर (masked)
        p.addInsn(OP_SELECT, sMask, r, sOut, sOut);          // dst = mask ? r : dst
    अन्यथा
        p.moveToOutput(r, sOut);                             // straight into the output, no dead temp

    p.compile();    // pack temp buffers + count consts (kernels already bound by addInsn)
}

// addWeighted(a, alpha, b, beta, gamma) = a*alpha + b*beta + gamma. One fused kernel (two v_fma) via
// emitBinary(OP_ADDW); a final cast is appended only when the requested rdepth isn't a type the kernel
// emits directly. alpha/beta/gamma travel in the instruction's params block, not as operands.
शून्य makeAddWeightedProgram(TExpr& p, पूर्णांक depth0, पूर्णांक depth1, पूर्णांक rdepth,
                            द्विगुण alpha, द्विगुण beta, द्विगुण gamma)
{
    p.clear();
    पूर्णांक sA   = p.add_आदान(depth0);
    पूर्णांक sB   = p.add_आदान(depth1);
    पूर्णांक sOut = p.addOutput(rdepth);
    पूर्णांक r = p.emit_द्विमान(OP_ADDW, sA, sB, rdepth, Scalar(alpha, beta, gamma));
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
    पूर्णांक आदान_2 = 0;
    std::string text;     // identifier or operator spelling
};

// --- lexer -------------------------------------------------------------------------------
संरचना Lexer
{
    std::string_view s;
    माप_प्रकार pos = 0;

    explicit Lexer(std::string_view src) : s(src) {}

    स्थिर तार्किक isIdentStart(अक्षर c) { परत std::है_अक्षर((अचिन्हित अक्षर)c) || c == '_'; }
    स्थिर तार्किक isIdentChar(अक्षर c)  { परत std::है_अक्षर_अंक((अचिन्हित अक्षर)c) || c == '_'; }

    Token next()
    {
        पर्यंत (pos < s.size() && std::है_खाली((अचिन्हित अक्षर)s[pos])) pos++;
        Token t;
        जर (pos >= s.size()) { t.type = T_END; परत t; }

        अक्षर c = s[pos];

        जर (c == '{')                       // input placeholder {N}
        {
            pos++;
            माप_प्रकार start = pos;
            पर्यंत (pos < s.size() && s[pos] != '}') pos++;
            CV_Assert(pos < s.size() && "ew::expression: unterminated '{'");
            t.type = T_आदान;
            t.आदान_2 = std::म_से_प(std::string(s.substr(start, pos - start)).c_str());
            pos++;                          // consume '}'
            परत t;
        }
        जर (std::है_अंक((अचिन्हित अक्षर)c) || (c == '.' && pos + 1 < s.size() &&
                                               std::है_अंक((अचिन्हित अक्षर)s[pos + 1])))
        {
            अक्षर* end = nullptr;
            std::string num(s.substr(pos));
            t.type = T_अंक;
            t.num = std::म_से_भग्न(num.c_str(), &end);
            pos += (माप_प्रकार)(end - num.c_str());
            परत t;
        }
        जर (isIdentStart(c))
        {
            माप_प्रकार start = pos;
            पर्यंत (pos < s.size() && isIdentChar(s[pos])) pos++;
            t.type = T_IDENT;
            t.text.assign(s.substr(start, pos - start));
            परत t;
        }
        निवड (c)
        {
        स्थिती '(': pos++; t.type = T_LPAREN; परत t;
        स्थिती ')': pos++; t.type = T_RPAREN; परत t;
        स्थिती ',': pos++; t.type = T_COMMA;  परत t;
        स्थिती ';': pos++; t.type = T_SEMI;   परत t;
        }
        // multi/!single-char operators
        स्वतः two = [&](स्थिरांक अक्षर* op) {
            परत pos + 1 < s.size() && s[pos] == op[0] && s[pos + 1] == op[1];
        };
        t.type = T_OP;
        जर (two("<=") || two(">=") || two("==") || two("!=") || two("**"))
        { t.text.assign(s.substr(pos, 2)); pos += 2; परत t; }
        जर (c == '=') { pos++; t.type = T_ASSIGN; परत t; }
        CV_Assert(std::म_अक्षर("+-*/<>&|^!?:", c) && "ew::expression: unexpected character");
        t.text.assign(1, c); pos++;
        परत t;
    }
};

// --- operator / function tables ----------------------------------------------------------
स्थिर पूर्णांक binPrec(स्थिरांक std::string& op)
{
    जर (op == "**") परत 8;              // power binds tighter than '*'; RIGHT-associative
    जर (op == "*" || op == "/") परत 7;
    जर (op == "+" || op == "-") परत 6;
    जर (op == "<" || op == "<=" || op == ">" || op == ">=") परत 5;
    जर (op == "==" || op == "!=") परत 4;
    जर (op == "&") परत 3;
    जर (op == "^") परत 2;
    जर (op == "|") परत 1;
    परत -1;
}

स्थिर शीर्ष binOp(स्थिरांक std::string& op)
{
    जर (op == "**") परत OP_POW;
    जर (op == "+")  परत OP_ADD;
    जर (op == "-")  परत OP_SUB;
    जर (op == "*")  परत OP_MUL;
    जर (op == "/")  परत OP_DIV;
    जर (op == "<")  परत OP_CMP_LT;
    जर (op == "<=") परत OP_CMP_LE;
    जर (op == ">")  परत OP_CMP_GT;
    जर (op == ">=") परत OP_CMP_GE;
    जर (op == "==") परत OP_CMP_EQ;
    जर (op == "!=") परत OP_CMP_NE;
    जर (op == "&")  परत OP_AND;
    जर (op == "|")  परत OP_OR;
    जर (op == "^")  परत OP_XOR;
    CV_Error(Error::StsParseError, "ew::expression: bad binary operator");
}

// type-cast function name -> depth, or -1 if not a type name
स्थिर पूर्णांक typeDepth(स्थिरांक std::string& name)
{
    जर (name == "float")    परत CV_32F;
    जर (name == "double")   परत CV_64F;
    जर (name == "half" || name == "float16") परत CV_16F;
    जर (name == "bfloat16") परत CV_16BF;
    जर (name == "uint8")    परत CV_8U;
    जर (name == "int8")     परत CV_8S;
    जर (name == "uint16")   परत CV_16U;
    जर (name == "int16")    परत CV_16S;
    जर (name == "uint32")   परत CV_32U;
    जर (name == "int32")    परत CV_32S;
    जर (name == "uint64")   परत CV_64U;
    जर (name == "int64")    परत CV_64S;
    परत -1;
}

// element-wise op function name -> (op, arity), or arity 0 if unknown
स्थिर शीर्ष fnOp(स्थिरांक std::string& name, पूर्णांक& arity)
{
    संरचना E { स्थिरांक अक्षर* n; शीर्ष op; };
    स्थिर स्थिरांक E unary[]  = { {"abs",OP_ABS},{"sqrt",OP_SQRT},{"exp",OP_EXP},{"log",OP_LOG},
                                {"sin",OP_SIN},{"cos",OP_COS},{"tanh",OP_TANH},{"erf",OP_ERF},
                                {"relu",OP_RELU} };
    स्थिर स्थिरांक E binary[] = { {"max",OP_MAX},{"min",OP_MIN},{"pow",OP_POW},{"absdiff",OP_ABSDIFF},
                                {"hypot",OP_HYPOT},{"mag",OP_HYPOT},     // mag = the cv::magnitude-flavored alias
                                {"atan2",OP_ATAN2} };
    स्थिर स्थिरांक E tern[]   = { {"clamp",OP_CLAMP},{"select",OP_SELECT} };
    साठी (स्थिरांक E& e : unary)  जर (name == e.n) { arity = 1; परत e.op; }
    साठी (स्थिरांक E& e : binary) जर (name == e.n) { arity = 2; परत e.op; }
    साठी (स्थिरांक E& e : tern)   जर (name == e.n) { arity = 3; परत e.op; }
    arity = 0; परत OP_NOP;
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
    स्थिरांक पूर्णांक* आदान_Slot;                 // slot id of each external input (precreated)
    पूर्णांक ninputs;
    std::map<std::string, पूर्णांक> env;       // named temporaries -> slot

    Parser(std::string_view src, TExpr& अभिव्यक्ती, स्थिरांक पूर्णांक* islot, पूर्णांक nin)
        : lex(src), e(अभिव्यक्ती), आदान_Slot(islot), ninputs(nin) { cur = lex.next(); }

    शून्य advance() { cur = lex.next(); }
    तार्किक isOp(स्थिरांक अक्षर* op) स्थिरांक { परत cur.type == T_OP && cur.text == op; }

    शून्य expect(TokType t, स्थिरांक अक्षर* what)
    {
        CV_Assert(cur.type == t && what);
        advance();
    }

    // A flexible CONST slot holds a literal whose depth the emit* layers pick per use.
    तार्किक isFlexConst(पूर्णांक slot) स्थिरांक
    {
        परत e.arginfo[slot].kind == TExpr::CONST && e.arginfo[slot].depth == EW_DEPTH_NONE;
    }

    पूर्णांक parsePrimary()
    {
        जर (cur.type == T_अंक)   { पूर्णांक s = e.addConst(EW_DEPTH_NONE, Scalar(cur.num), 1); advance(); परत s; }
        जर (cur.type == T_आदान) { पूर्णांक idx = cur.आदान_2; advance();
                                   CV_Assert(idx >= 0 && idx < ninputs && "ew::expression: input index out of range");
                                   परत आदान_Slot[idx]; }
        जर (cur.type == T_LPAREN){ advance(); पूर्णांक x = parseTernary(); expect(T_RPAREN, "expected ')'"); परत x; }
        जर (cur.type == T_IDENT)
        {
            std::string name = cur.text; advance();
            जर (cur.type != T_LPAREN)             // variable reference
            {
                स्वतः it = env.find(name);
                CV_Assert(it != env.end() && "ew::expression: undefined name");
                परत it->second;
            }
            advance();                            // consume '('
            // every function takes <= 3 args (clamp/select); a stack buffer avoids a std::vector
            // heap alloc. The {} is for gcc's -Wmaybe-uninitialized only.
            std::array<पूर्णांक, 8> args = {}; पूर्णांक nargs = 0;
            जर (cur.type != T_RPAREN)
            {
                args[nargs++] = parseTernary();
                पर्यंत (cur.type == T_COMMA) { advance();
                    CV_Assert(nargs < (पूर्णांक)args.size() && "ew::expression: too many arguments");
                    args[nargs++] = parseTernary(); }
            }
            expect(T_RPAREN, "expected ')'");

            पूर्णांक td = typeDepth(name);
            जर (td >= 0)
            {
                CV_Assert(nargs == 1);                       // type cast: just convert the operand
                पूर्णांक s = args[0];
                परत isFlexConst(s) ? e.typedConstFrom(s, td)
                                      : e.maybeAddCast(s, td);
            }

            पूर्णांक arity = 0; शीर्ष op = fnOp(name, arity);
            CV_Assert(arity != 0 && "ew::expression: unknown function");
            CV_Assert(nargs == arity && "ew::expression: wrong number of arguments");
            जर (arity == 1) परत e.emitUnary(op, args[0]);
            जर (arity == 2) परत e.emit_द्विमान(op, args[0], args[1]);
            परत e.emitTernary(op, args[0], args[1], args[2]);
        }
        CV_Error(Error::StsParseError, "ew::expression: expected a primary expression");
    }

    पूर्णांक parseUnary()
    {
        जर (isOp("-") || isOp("!"))
        {
            std::string op = cur.text; advance();
            पूर्णांक operand = parseUnary();
            // constant-fold a leading sign so that "-1.5" does not need an OP_NEG kernel
            जर (op == "-" && isFlexConst(operand))
            {
                AutoBuffer<द्विगुण, MAX_LOCAL_CN> v; पूर्णांक cn = constDoubles(e, operand, v);
                Scalar neg; साठी (पूर्णांक i = 0; i < cn && i < 4; i++) neg[i] = -v[i];
                परत e.addConst(EW_DEPTH_NONE, neg, std::min(cn, 4));
            }
            परत e.emitUnary(op == "-" ? OP_NEG : OP_NOT, operand);
        }
        परत parsePrimary();
    }

    पूर्णांक parse_अभिव्यक्ती(पूर्णांक minPrec)
    {
        पूर्णांक डावा = parseUnary();
        पर्यंत (cur.type == T_OP)
        {
            पूर्णांक p = binPrec(cur.text);
            जर (p < minPrec) थांबा;
            std::string op = cur.text; advance();
            // '**' is right-associative (a ** b ** c == a ** (b ** c)): recurse at the SAME
            // precedence so the right side swallows further '**'s; everything else at p+1.
            पूर्णांक उजवा = parse_अभिव्यक्ती(op == "**" ? p : p + 1);
            डावा = e.emit_द्विमान(binOp(op), डावा, उजवा);
        }
        परत डावा;
    }

    // The ternary conditional  cond ? a : b  == select(cond, a, b). Lowest precedence (below all
    // arithmetic/compare/bitwise), right-associative: f1 ? a : f2 ? b : c groups as
    // f1 ? a : (f2 ? b : c). This is the general expression entry point.
    पूर्णांक parseTernary()
    {
        पूर्णांक cond = parse_अभिव्यक्ती(0);
        जर (!isOp("?")) परत cond;
        advance();
        पूर्णांक thenv = parseTernary();
        CV_Assert(isOp(":") && "ew::expression: expected ':' in a ?: conditional");
        advance();
        पूर्णांक elsev = parseTernary();               // right-associative chain
        परत e.emitTernary(OP_SELECT, cond, thenv, elsev);
    }

    // final result: a single expression, or a top-level tuple (e0, e1, ...).
    शून्य parseResult()
    {
        जर (cur.type == T_LPAREN)
        {
            // backtrack point: save the lexer AND the program length, since the trial parse below
            // emits instructions/slots that must be rolled back if this turns out NOT to be a tuple.
            Lexer save = lex; Token savedCur = cur;
            माप_प्रकार nInsn = e.prog.size(), nArg = e.arginfo.size(); पूर्णांक nTemp = e.ntemps;
            advance();
            पूर्णांक e0 = parseTernary();
            जर (cur.type == T_COMMA)
            {
                e.output(e0);
                पर्यंत (cur.type == T_COMMA) { advance(); e.output(parseTernary()); }
                expect(T_RPAREN, "expected ')'");
                परत;
            }
            lex = save; cur = savedCur;                   // not a tuple -> undo the trial parse
            e.prog.resize(nInsn); e.arginfo.resize(nArg); e.ntemps = nTemp;
        }
        e.output(parseTernary());
    }

    शून्य parse()
    {
        पर्यंत (सत्य)
        {
            जर (cur.type == T_IDENT)
            {
                Lexer save = lex; Token savedCur = cur;   // lexer-only backtrack (nothing emitted yet)
                std::string name = cur.text; advance();
                जर (cur.type == T_ASSIGN)
                {
                    advance();
                    स्थिरांक पूर्णांक slot = parseTernary();
                    // The name can be used any number of times below (including not at all), so the
                    // value must outlive the expression that produced it: pin the slot so the
                    // retire-the-temp optimizations skip it.
                    e.arginfo[slot].pinned = सत्य;
                    env[name] = slot;
                    expect(T_SEMI, "expected ';' after assignment");
                    सुरूठेवा;
                }
                lex = save; cur = savedCur;               // not an assignment
            }
            parseResult();
            जर (cur.type == T_SEMI) advance();
            थांबा;
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
शून्य texpr(स्थिरांक std::string& अभिव्यक्ती, आदान_Array_Of_Arrays _inputs, OutputArrayOfArrays _outputs)
{
    using नामाकाश cv::ew;
    CV_INSTRUMENT_REGION();
    CV_Assert(_inputs.kind() == आदान_Array_2::STD_VECTOR_MAT);
    स्थिरांक std::vector<Mat>& inps = *(स्थिरांक std::vector<Mat>*)_inputs.getObj();
    स्थिरांक पूर्णांक ninputs = (पूर्णांक)inps.size();
    साठी (पूर्णांक i = 0; i < ninputs; i++)
        CV_Assert(!inps[i].empty());

    // Input depths are known now, so build a fully-typed program straight away: one INPUT slot per
    // input (slot index i carries input i's depth), then parse the expression into instructions.
    TExpr e;
    AutoBuffer<पूर्णांक> islot(std::max(ninputs, 1));
    साठी (पूर्णांक i = 0; i < ninputs; i++) islot[i] = e.add_आदान(inps[i].depth());
    Parser(अभिव्यक्ती, e, islot.data(), ninputs).parse();
    e.compile();

    स्वतः kind = _outputs.kind();
    जर (kind == आदान_Array_2::STD_VECTOR_MAT) {
        std::vector<Mat>& outs = _outputs.getMatVecRef();
        outs.resize(e.noutputs);
        e.exec(inps.data(), outs.data());
    } अन्यथा {
        CV_Error(Error::StsNotImplemented, "vector<Mat> is expected as output of texpr");
    }
}

}
