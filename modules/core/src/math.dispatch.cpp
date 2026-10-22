// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#include "precomp.hpp"
#include "arithm_expr.hpp"
#include "hal_replacement.hpp"
#include "math.simd.hpp"
#include "math.simd_declarations.hpp"

namespace cv { namespace ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
template<typename T>
static ціле halUnaryKernel(const void* src0_, size_t s0y, size_t s0x,
                          const void*, size_t, size_t, const void*, size_t, size_t,
                          void* dst_, size_t dsty, ціле width, ціле height,
                          const double*, ціле, void* userdata)
{
    typedef ціле (*HalFunc)(const T*, T*, ціле);
    const HalFunc fn = (HalFunc)userdata;
    s0y /= sizeof(T);
    dsty /= sizeof(T);
    CV_Assert(s0x <= 1u);
    const T* src0 = (const T*)src0_;
    T* dst = (T*)dst_;
    якщо (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    const ціле urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    для (ціле y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ціле code;
        якщо (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            для (ціле x = 0; x < width; x++) dst[x] = v;
        }
        інакше
            code = fn(src0, dst, width);
        якщо (code != CV_HAL_ERROR_OK)
            повернути code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    для (ціле y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*sizeof(T));
    повернути 0;
}

template<typename T, typename HalFunc>
static TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    якщо (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        повернути {halUnaryKernel<T>, (void*)fn, 0};
    повернути {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
static TKernel getEngineMathFunc(TOp op, ціле T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
void mathSpanEngine(TOp op, ціле depth, const void* src, void* dst, ціле n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    const double noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(TOp op, ціле T)
{
    якщо ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        static const TKernel exp32 = probeHalUnary<float >(cv_hal_exp32f);
        static const TKernel exp64 = probeHalUnary<double>(cv_hal_exp64f);
        static const TKernel log32 = probeHalUnary<float >(cv_hal_log32f);
        static const TKernel log64 = probeHalUnary<double>(cv_hal_log64f);
        const TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        якщо (k->fptr)
            повернути *k;
    }
    повернути getEngineMathFunc(op, T);
}

TKernel getPowFunc(ціле T, ціле R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
