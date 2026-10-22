// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#ضمن "precomp.hpp"
#ضمن "arithm_expr.hpp"
#ضمن "hal_replacement.hpp"
#ضمن "math.simd.hpp"
#ضمن "math.simd_declarations.hpp"

نطاق cv { نطاق ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
template<typename T>
ساكن صحيح halUnaryKernel(ثابت فراغ* src0_, size_t s0y, size_t s0x,
                          ثابت فراغ*, size_t, size_t, ثابت فراغ*, size_t, size_t,
                          فراغ* dst_, size_t dsty, صحيح width, صحيح height,
                          ثابت مزدوج*, صحيح, فراغ* userdata)
{
    تعريفنوع صحيح (*HalFunc)(ثابت T*, T*, صحيح);
    ثابت HalFunc fn = (HalFunc)userdata;
    s0y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x <= 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    T* dst = (T*)dst_;
    إذا (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ثابت صحيح urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    لكل (صحيح y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        صحيح code;
        إذا (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            لكل (صحيح x = 0; x < width; x++) dst[x] = v;
        }
        وإلا
            code = fn(src0, dst, width);
        إذا (code != CV_HAL_ERROR_OK)
            إرجاع code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    لكل (صحيح y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*حجم(T));
    إرجاع 0;
}

template<typename T, typename HalFunc>
ساكن TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    إذا (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        إرجاع {halUnaryKernel<T>, (فراغ*)fn, 0};
    إرجاع {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
ساكن TKernel getEngineMathFunc(قمة op, صحيح T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
فراغ mathSpanEngine(قمة op, صحيح depth, ثابت فراغ* src, فراغ* dst, صحيح n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    ثابت مزدوج noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(قمة op, صحيح T)
{
    إذا ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        ساكن ثابت TKernel exp32 = probeHalUnary<عائم >(cv_hal_exp32f);
        ساكن ثابت TKernel exp64 = probeHalUnary<مزدوج>(cv_hal_exp64f);
        ساكن ثابت TKernel log32 = probeHalUnary<عائم >(cv_hal_log32f);
        ساكن ثابت TKernel log64 = probeHalUnary<مزدوج>(cv_hal_log64f);
        ثابت TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        إذا (k->fptr)
            إرجاع *k;
    }
    إرجاع getEngineMathFunc(op, T);
}

TKernel getPowFunc(صحيح T, صحيح R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
