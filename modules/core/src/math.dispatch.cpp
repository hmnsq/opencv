// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "arithm_expr.hpp"
#ଅନ୍ତର୍ଭାଵ "hal_replacement.hpp"
#ଅନ୍ତର୍ଭାଵ "math.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "math.simd_declarations.hpp"

ନାମାକାଶ cv { ନାମାକାଶ ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
template<typename T>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା halUnaryKernel(ଅଚଳ ଶୂନ୍ୟ* src0_, size_t s0y, size_t s0x,
                          ଅଚଳ ଶୂନ୍ୟ*, size_t, size_t, ଅଚଳ ଶୂନ୍ୟ*, size_t, size_t,
                          ଶୂନ୍ୟ* dst_, size_t dsty, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height,
                          ଅଚଳ ଦ୍ୱିଗୁଣ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ* userdata)
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*HalFunc)(ଅଚଳ T*, T*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
    ଅଚଳ HalFunc fn = (HalFunc)userdata;
    s0y /= ଆକାର(T);
    dsty /= ଆକାର(T);
    CV_Assert(s0x <= 1u);
    ଅଚଳ T* src0 = (ଅଚଳ T*)src0_;
    T* dst = (T*)dst_;
    ଯଦି (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;
        ଯଦି (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < width; x++) dst[x] = v;
        }
        ଅନ୍ୟଥା
            code = fn(src0, dst, width);
        ଯଦି (code != CV_HAL_ERROR_OK)
            ଫେରନ୍ତୁ code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*ଆକାର(T));
    ଫେରନ୍ତୁ 0;
}

template<typename T, typename HalFunc>
ସ୍ଥିର TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    ଯଦି (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        ଫେରନ୍ତୁ {halUnaryKernel<T>, (ଶୂନ୍ୟ*)fn, 0};
    ଫେରନ୍ତୁ {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
ସ୍ଥିର TKernel getEngineMathFunc(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
ଶୂନ୍ୟ mathSpanEngine(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ଅଚଳ ଶୂନ୍ୟ* src, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    ଅଚଳ ଦ୍ୱିଗୁଣ noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା T)
{
    ଯଦି ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        ସ୍ଥିର ଅଚଳ TKernel exp32 = probeHalUnary<ଦଶମିକ >(cv_hal_exp32f);
        ସ୍ଥିର ଅଚଳ TKernel exp64 = probeHalUnary<ଦ୍ୱିଗୁଣ>(cv_hal_exp64f);
        ସ୍ଥିର ଅଚଳ TKernel log32 = probeHalUnary<ଦଶମିକ >(cv_hal_log32f);
        ସ୍ଥିର ଅଚଳ TKernel log64 = probeHalUnary<ଦ୍ୱିଗୁଣ>(cv_hal_log64f);
        ଅଚଳ TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        ଯଦି (k->fptr)
            ଫେରନ୍ତୁ *k;
    }
    ଫେରନ୍ତୁ getEngineMathFunc(op, T);
}

TKernel getPowFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
