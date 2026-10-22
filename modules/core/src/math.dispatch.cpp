// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm_expr.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal_replacement.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "math.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "math.simd_declarations.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਪੂਰਨ halUnaryKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                          ਅਚਲ ਖਾਲੀ*, size_t, size_t, ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                          ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                          ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ* userdata)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*HalFunc)(ਅਚਲ T*, T*, ਪੂਰਨ);
    ਅਚਲ HalFunc fn = (HalFunc)userdata;
    s0y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(T);
    CV_Assert(s0x <= 1u);
    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    T* dst = (T*)dst_;
    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && s0y == s0x*(size_t)ਚੌੜਾਈ) { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }
    ਅਚਲ ਪੂਰਨ urows = (s0y == 0 && ਉਚਾਈ > 1) ? 1 : ਉਚਾਈ;   // vertical broadcast: 1 row + copies
    ਲਈ (ਪੂਰਨ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ਪੂਰਨ ਕੋਡ;
        ਜੇ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            ਕੋਡ = fn(src0, &v, 1);
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++) dst[x] = v;
        }
        ਨਹੀਂਤਾਂ
            ਕੋਡ = fn(src0, dst, ਚੌੜਾਈ);
        ਜੇ (ਕੋਡ != CV_HAL_ERROR_OK)
            ਵਾਪਸ ਕੋਡ;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    ਲਈ (ਪੂਰਨ y = urows; y < ਉਚਾਈ; y++)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + (size_t)y*dsty, dst, (size_t)ਚੌੜਾਈ*ਆਕਾਰ(T));
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ HalFunc>
ਸਥਿਰ TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    ਜੇ (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        ਵਾਪਸ {halUnaryKernel<T>, (ਖਾਲੀ*)fn, 0};
    ਵਾਪਸ {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
ਸਥਿਰ TKernel getEngineMathFunc(ਸਿਖਰ op, ਪੂਰਨ T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
ਖਾਲੀ math_ਵਿਸਤਾਰ_Engine(ਸਿਖਰ op, ਪੂਰਨ ਡੂੰਘਾਈ, ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ* dst, ਪੂਰਨ n)
{
    TKernel k = getEngineMathFunc(op, ਡੂੰਘਾਈ);
    CV_Assert(k.fptr);
    ਅਚਲ ਦੁੱਗਣਾ noparams[4] = {};
    k.fptr(src, 0, 1, ਖਾਲੀ_ਸੂਚਕ, 0, 0, ਖਾਲੀ_ਸੂਚਕ, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(ਸਿਖਰ op, ਪੂਰਨ T)
{
    ਜੇ ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        ਸਥਿਰ ਅਚਲ TKernel exp32 = probeHalUnary<ਦਸ਼ਮਲਵ >(cv_hal_exp32f);
        ਸਥਿਰ ਅਚਲ TKernel exp64 = probeHalUnary<ਦੁੱਗਣਾ>(cv_hal_exp64f);
        ਸਥਿਰ ਅਚਲ TKernel log32 = probeHalUnary<ਦਸ਼ਮਲਵ >(cv_hal_log32f);
        ਸਥਿਰ ਅਚਲ TKernel log64 = probeHalUnary<ਦੁੱਗਣਾ>(cv_hal_log64f);
        ਅਚਲ TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        ਜੇ (k->fptr)
            ਵਾਪਸ *k;
    }
    ਵਾਪਸ getEngineMathFunc(op, T);
}

TKernel getPowFunc(ਪੂਰਨ T, ਪੂਰਨ R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
