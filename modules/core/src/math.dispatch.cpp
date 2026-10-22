// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "arithm_expr.hpp"
#அந்தர்பாவ "hal_replacement.hpp"
#அந்தர்பாவ "math.simd.hpp"
#அந்தர்பாவ "math.simd_declarations.hpp"

நாமாகாஶ cv { நாமாகாஶ ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
template<typename T>
நிலையான முழு halUnaryKernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                          மாறிலி வெறுமை*, size_t, size_t, மாறிலி வெறுமை*, size_t, size_t,
                          வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                          மாறிலி இரட்டை*, முழு, வெறுமை* userdata)
{
    வகைவரை முழு (*HalFunc)(மாறிலி T*, T*, முழு);
    மாறிலி HalFunc fn = (HalFunc)userdata;
    s0y /= அளவு(T);
    dsty /= அளவு(T);
    CV_Assert(s0x <= 1u);
    மாறிலி T* src0 = (மாறிலி T*)src0_;
    T* dst = (T*)dst_;
    எனில் (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    மாறிலி முழு urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    க்கு (முழு y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        முழு code;
        எனில் (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            க்கு (முழு x = 0; x < width; x++) dst[x] = v;
        }
        இல்லையெனில்
            code = fn(src0, dst, width);
        எனில் (code != CV_HAL_ERROR_OK)
            திரும்பு code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    க்கு (முழு y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*அளவு(T));
    திரும்பு 0;
}

template<typename T, typename HalFunc>
நிலையான TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    எனில் (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        திரும்பு {halUnaryKernel<T>, (வெறுமை*)fn, 0};
    திரும்பு {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
நிலையான TKernel getEngineMathFunc(உச்சி op, முழு T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
வெறுமை mathSpanEngine(உச்சி op, முழு depth, மாறிலி வெறுமை* src, வெறுமை* dst, முழு n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    மாறிலி இரட்டை noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(உச்சி op, முழு T)
{
    எனில் ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        நிலையான மாறிலி TKernel exp32 = probeHalUnary<தசம >(cv_hal_exp32f);
        நிலையான மாறிலி TKernel exp64 = probeHalUnary<இரட்டை>(cv_hal_exp64f);
        நிலையான மாறிலி TKernel log32 = probeHalUnary<தசம >(cv_hal_log32f);
        நிலையான மாறிலி TKernel log64 = probeHalUnary<இரட்டை>(cv_hal_log64f);
        மாறிலி TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        எனில் (k->fptr)
            திரும்பு *k;
    }
    திரும்பு getEngineMathFunc(op, T);
}

TKernel getPowFunc(முழு T, முழு R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
