// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "arithm_expr.hpp"
#અન્તર્ભાવ "arithm.simd.hpp"
#અન્તર્ભાવ "arithm.simd_declarations.hpp"

નામાકાશ cv { નામાકાશ ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(પૂર્ણાંક T, પૂર્ણાંક R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(પૂર્ણાંક T, પૂર્ણાંક R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(પૂર્ણાંક T, પૂર્ણાંક R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(પૂર્ણાંક T, પૂર્ણાંક R, તાર્કિક chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(પૂર્ણાંક T, પૂર્ણાંક R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(પૂર્ણાંક T, પૂર્ણાંક R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(પૂર્ણાંક T, પૂર્ણાંક R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(પૂર્ણાંક T, પૂર્ણાંક R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(પૂર્ણાંક T, પૂર્ણાંક R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(ટોચ op, પૂર્ણાંક T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(ટોચ op, પૂર્ણાંક esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(પૂર્ણાંક esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(પૂર્ણાંક T, પૂર્ણાંક R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(પૂર્ણાંક mdepth, પૂર્ણાંક T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(પૂર્ણાંક T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
સ્થિર TKernel getCastFunc(પૂર્ણાંક sd, પૂર્ણાંક dd, તાર્કિક scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(ટોચ op, પૂર્ણાંક depth0, પૂર્ણાંક depth1, પૂર્ણાંક depth2, પૂર્ણાંક rdepth)
{
    (શૂન્ય)depth2;

    જો (op == OP_CAST)          પાછા getCastFunc(depth0, rdepth, ખોટું);
    જો (op == OP_CONVERT_SCALE) પાછા getCastFunc(depth0, rdepth, સાચું);

    જો (op == OP_ADD || op == OP_SUB)
    {
        જો (depth0 != depth1) પાછા {};   // operands must be the same type
        પાછા op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    જો (op == OP_ADDW)
    {
        જો (depth0 != depth1) પાછા {};
        પાછા getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    જો (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        જો (depth0 != depth1) પાછા {};
        જો (op == OP_MUL) પાછા getMulFunc(depth0, rdepth);
        જો (op == OP_POW) પાછા getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        અચલ તાર્કિક isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        પાછા getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    જો (op == OP_MIN || op == OP_MAX)
    {
        જો (depth0 != depth1 || rdepth != depth0) પાછા {};
        પાછા op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    જો (op == OP_ABSDIFF)
    {
        જો (depth0 != depth1) પાછા {};
        પાછા getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    જો (opCategory(op) == CAT_COMPARE)
    {
        જો (depth0 != depth1 || rdepth != CV_8U) પાછા {};
        પાછા getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    જો (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        જો (depth0 != depth1 || rdepth != depth0) પાછા {};
        પાછા getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    જો (op == OP_NOT)
    {
        જો (rdepth != depth0) પાછા {};
        પાછા getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    જો (op == OP_HYPOT || op == OP_ATAN2)
    {
        જો (depth0 != depth1) પાછા {};
        પાછા op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    જો (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        જો (rdepth != depth0) પાછા {};
        પાછા getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    જો (op == OP_SELECT)
    {
        જો (depth1 != depth2 || rdepth != depth1) પાછા {};
        પાછા getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    જો (op == OP_CLAMP)
    {
        જો (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) પાછા {};
        પાછા getClampFunc(rdepth);
    }

    પાછા {};
}

}} // namespace cv::ew

નામાકાશ cv { નામાકાશ hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
શૂન્ય mul8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
           uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, શૂન્ય* scale)
{
    અચલ દ્વિગુણ params[] = { scale ? *(અચલ દ્વિગુણ*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
સ્થિર શૂન્ય bitwise8u(ew::ટોચ op, અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                      uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    અચલ દ્વિગુણ noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

શૂન્ય and8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
           uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, શૂન્ય*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

શૂન્ય or8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
          uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, શૂન્ય*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

શૂન્ય xor8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
           uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, શૂન્ય*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

શૂન્ય not8u(અચલ uchar* src1, size_t step1, અચલ uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, શૂન્ય*)
{
    અચલ દ્વિગુણ noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
