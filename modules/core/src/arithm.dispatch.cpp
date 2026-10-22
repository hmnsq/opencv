// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "arithm_expr.hpp"
#అన్తర్భావ "arithm.simd.hpp"
#అన్తర్భావ "arithm.simd_declarations.hpp"

నామాకాశ cv { నామాకాశ ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel get_కూడు_Func(పూర్ణాంకం T, పూర్ణాంకం R)            { CV_CPU_DISPATCH(get_కూడు_Func_2,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(పూర్ణాంకం T, పూర్ణాంకం R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(పూర్ణాంకం T, పూర్ణాంకం R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(పూర్ణాంకం T, పూర్ణాంకం R, తార్కిక chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(పూర్ణాంకం T, పూర్ణాంకం R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(పూర్ణాంకం T, పూర్ణాంకం R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(పూర్ణాంకం T, పూర్ణాంకం R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(పూర్ణాంకం T, పూర్ణాంకం R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(పూర్ణాంకం T, పూర్ణాంకం R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(శిఖరం op, పూర్ణాంకం T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(శిఖరం op, పూర్ణాంకం esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(పూర్ణాంకం esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel get_కూడు_Weighted_Func(పూర్ణాంకం T, పూర్ణాంకం R)    { CV_CPU_DISPATCH(get_కూడు_Weighted_Func_2, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(పూర్ణాంకం mdepth, పూర్ణాంకం T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(పూర్ణాంకం T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
స్థిర TKernel getCastFunc(పూర్ణాంకం sd, పూర్ణాంకం dd, తార్కిక scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(శిఖరం op, పూర్ణాంకం depth0, పూర్ణాంకం depth1, పూర్ణాంకం depth2, పూర్ణాంకం rdepth)
{
    (శూన్య)depth2;

    ఒకవేళ (op == OP_CAST)          తిరిగి getCastFunc(depth0, rdepth, అబద్ధం);
    ఒకవేళ (op == OP_CONVERT_SCALE) తిరిగి getCastFunc(depth0, rdepth, నిజం);

    ఒకవేళ (op == OP_కూడు || op == OP_SUB)
    {
        ఒకవేళ (depth0 != depth1) తిరిగి {};   // operands must be the same type
        తిరిగి op == OP_కూడు ? get_కూడు_Func(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    ఒకవేళ (op == OP_ADDW)
    {
        ఒకవేళ (depth0 != depth1) తిరిగి {};
        తిరిగి get_కూడు_Weighted_Func(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    ఒకవేళ (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        ఒకవేళ (depth0 != depth1) తిరిగి {};
        ఒకవేళ (op == OP_MUL) తిరిగి getMulFunc(depth0, rdepth);
        ఒకవేళ (op == OP_POW) తిరిగి getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        స్థిరరాశి తార్కిక isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        తిరిగి getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    ఒకవేళ (op == OP_MIN || op == OP_MAX)
    {
        ఒకవేళ (depth0 != depth1 || rdepth != depth0) తిరిగి {};
        తిరిగి op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    ఒకవేళ (op == OP_ABSDIFF)
    {
        ఒకవేళ (depth0 != depth1) తిరిగి {};
        తిరిగి getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    ఒకవేళ (opCategory(op) == CAT_COMPARE)
    {
        ఒకవేళ (depth0 != depth1 || rdepth != CV_8U) తిరిగి {};
        తిరిగి getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    ఒకవేళ (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        ఒకవేళ (depth0 != depth1 || rdepth != depth0) తిరిగి {};
        తిరిగి getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    ఒకవేళ (op == OP_NOT)
    {
        ఒకవేళ (rdepth != depth0) తిరిగి {};
        తిరిగి getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    ఒకవేళ (op == OP_HYPOT || op == OP_ATAN2)
    {
        ఒకవేళ (depth0 != depth1) తిరిగి {};
        తిరిగి op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    ఒకవేళ (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        ఒకవేళ (rdepth != depth0) తిరిగి {};
        తిరిగి getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    ఒకవేళ (op == OP_SELECT)
    {
        ఒకవేళ (depth1 != depth2 || rdepth != depth1) తిరిగి {};
        తిరిగి getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    ఒకవేళ (op == OP_CLAMP)
    {
        ఒకవేళ (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) తిరిగి {};
        తిరిగి getClampFunc(rdepth);
    }

    తిరిగి {};
}

}} // namespace cv::ew

నామాకాశ cv { నామాకాశ hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
శూన్య mul8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
           uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, శూన్య* scale)
{
    స్థిరరాశి ద్విగుణ params[] = { scale ? *(స్థిరరాశి ద్విగుణ*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, శూన్యసూచిక, 0, 0, dst, అడుగు, వెడల్పు, ఎత్తు,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
స్థిర శూన్య bitwise8u(ew::శిఖరం op, స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                      uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    స్థిరరాశి ద్విగుణ noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != శూన్యసూచిక);
    k.fptr(src1, step1, 1, src2, step2, 1, శూన్యసూచిక, 0, 0, dst, అడుగు, వెడల్పు, ఎత్తు,
           noparams, k.flags, k.userdata);
}

శూన్య and8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
           uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, శూన్య*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, అడుగు, వెడల్పు, ఎత్తు); }

శూన్య or8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
          uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, శూన్య*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, అడుగు, వెడల్పు, ఎత్తు); }

శూన్య xor8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
           uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, శూన్య*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, అడుగు, వెడల్పు, ఎత్తు); }

శూన్య not8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, శూన్య*)
{
    స్థిరరాశి ద్విగుణ noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != శూన్యసూచిక);
    k.fptr(src1, step1, 1, శూన్యసూచిక, 0, 0, శూన్యసూచిక, 0, 0, dst, అడుగు, వెడల్పు, ఎత్తు,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
