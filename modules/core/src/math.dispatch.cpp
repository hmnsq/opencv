// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "arithm_expr.hpp"
#అన్తర్భావ "hal_replacement.hpp"
#అన్తర్భావ "math.simd.hpp"
#అన్తర్భావ "math.simd_declarations.hpp"

నామాకాశ cv { నామాకాశ ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
మూస<రకం_పేరు T>
స్థిర పూర్ణాంకం halUnaryKernel(స్థిరరాశి శూన్య* src0_, size_t s0y, size_t s0x,
                          స్థిరరాశి శూన్య*, size_t, size_t, స్థిరరాశి శూన్య*, size_t, size_t,
                          శూన్య* dst_, size_t dsty, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                          స్థిరరాశి ద్విగుణ*, పూర్ణాంకం, శూన్య* userdata)
{
    రకనిర్వచనం పూర్ణాంకం (*HalFunc)(స్థిరరాశి T*, T*, పూర్ణాంకం);
    స్థిరరాశి HalFunc fn = (HalFunc)userdata;
    s0y /= పరిమాణం(T);
    dsty /= పరిమాణం(T);
    CV_Assert(s0x <= 1u);
    స్థిరరాశి T* src0 = (స్థిరరాశి T*)src0_;
    T* dst = (T*)dst_;
    ఒకవేళ (ఎత్తు > 1 && dsty == (size_t)వెడల్పు && s0y == s0x*(size_t)వెడల్పు) { వెడల్పు *= ఎత్తు; ఎత్తు = 1; }
    స్థిరరాశి పూర్ణాంకం urows = (s0y == 0 && ఎత్తు > 1) ? 1 : ఎత్తు;   // vertical broadcast: 1 row + copies
    కొరకు (పూర్ణాంకం y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        పూర్ణాంకం సంకేతం_2;
        ఒకవేళ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            సంకేతం_2 = fn(src0, &v, 1);
            కొరకు (పూర్ణాంకం x = 0; x < వెడల్పు; x++) dst[x] = v;
        }
        లేకపోతే
            సంకేతం_2 = fn(src0, dst, వెడల్పు);
        ఒకవేళ (సంకేతం_2 != CV_HAL_ERROR_OK)
            తిరిగి సంకేతం_2;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    కొరకు (పూర్ణాంకం y = urows; y < ఎత్తు; y++)
        జ్ఞాపకం_నకలు(dst + (size_t)y*dsty, dst, (size_t)వెడల్పు*పరిమాణం(T));
    తిరిగి 0;
}

మూస<రకం_పేరు T, రకం_పేరు HalFunc>
స్థిర TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    ఒకవేళ (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        తిరిగి {halUnaryKernel<T>, (శూన్య*)fn, 0};
    తిరిగి {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
స్థిర TKernel getEngineMathFunc(శిఖరం op, పూర్ణాంకం T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
శూన్య math_పరిధి_Engine(శిఖరం op, పూర్ణాంకం లోతు, స్థిరరాశి శూన్య* src, శూన్య* dst, పూర్ణాంకం n)
{
    TKernel k = getEngineMathFunc(op, లోతు);
    CV_Assert(k.fptr);
    స్థిరరాశి ద్విగుణ noparams[4] = {};
    k.fptr(src, 0, 1, శూన్యసూచిక, 0, 0, శూన్యసూచిక, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(శిఖరం op, పూర్ణాంకం T)
{
    ఒకవేళ ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        స్థిర స్థిరరాశి TKernel exp32 = probeHalUnary<దశాంశం >(cv_hal_exp32f);
        స్థిర స్థిరరాశి TKernel exp64 = probeHalUnary<ద్విగుణ>(cv_hal_exp64f);
        స్థిర స్థిరరాశి TKernel log32 = probeHalUnary<దశాంశం >(cv_hal_log32f);
        స్థిర స్థిరరాశి TKernel log64 = probeHalUnary<ద్విగుణ>(cv_hal_log64f);
        స్థిరరాశి TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        ఒకవేళ (k->fptr)
            తిరిగి *k;
    }
    తిరిగి getEngineMathFunc(op, T);
}

TKernel getPowFunc(పూర్ణాంకం T, పూర్ణాంకం R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
