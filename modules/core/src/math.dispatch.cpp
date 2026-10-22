// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "arithm_expr.hpp"
#અન્તર્ભાવ "hal_replacement.hpp"
#અન્તર્ભાવ "math.simd.hpp"
#અન્તર્ભાવ "math.simd_declarations.hpp"

નામાકાશ cv { નામાકાશ ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
template<typename T>
સ્થિર પૂર્ણાંક halUnaryKernel(અચલ શૂન્ય* src0_, size_t s0y, size_t s0x,
                          અચલ શૂન્ય*, size_t, size_t, અચલ શૂન્ય*, size_t, size_t,
                          શૂન્ય* dst_, size_t dsty, પૂર્ણાંક width, પૂર્ણાંક height,
                          અચલ દ્વિગુણ*, પૂર્ણાંક, શૂન્ય* userdata)
{
    પ્રકારવ્યાખ્યા પૂર્ણાંક (*HalFunc)(અચલ T*, T*, પૂર્ણાંક);
    અચલ HalFunc fn = (HalFunc)userdata;
    s0y /= કદ(T);
    dsty /= કદ(T);
    CV_Assert(s0x <= 1u);
    અચલ T* src0 = (અચલ T*)src0_;
    T* dst = (T*)dst_;
    જો (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    અચલ પૂર્ણાંક urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    માટે (પૂર્ણાંક y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        પૂર્ણાંક code;
        જો (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            માટે (પૂર્ણાંક x = 0; x < width; x++) dst[x] = v;
        }
        નહીંતર
            code = fn(src0, dst, width);
        જો (code != CV_HAL_ERROR_OK)
            પાછા code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    માટે (પૂર્ણાંક y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*કદ(T));
    પાછા 0;
}

template<typename T, typename HalFunc>
સ્થિર TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    જો (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        પાછા {halUnaryKernel<T>, (શૂન્ય*)fn, 0};
    પાછા {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
સ્થિર TKernel getEngineMathFunc(ટોચ op, પૂર્ણાંક T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
શૂન્ય mathSpanEngine(ટોચ op, પૂર્ણાંક depth, અચલ શૂન્ય* src, શૂન્ય* dst, પૂર્ણાંક n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    અચલ દ્વિગુણ noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(ટોચ op, પૂર્ણાંક T)
{
    જો ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        સ્થિર અચલ TKernel exp32 = probeHalUnary<દશાંશ >(cv_hal_exp32f);
        સ્થિર અચલ TKernel exp64 = probeHalUnary<દ્વિગુણ>(cv_hal_exp64f);
        સ્થિર અચલ TKernel log32 = probeHalUnary<દશાંશ >(cv_hal_log32f);
        સ્થિર અચલ TKernel log64 = probeHalUnary<દ્વિગુણ>(cv_hal_log64f);
        અચલ TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        જો (k->fptr)
            પાછા *k;
    }
    પાછા getEngineMathFunc(op, T);
}

TKernel getPowFunc(પૂર્ણાંક T, પૂર્ણાંક R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
