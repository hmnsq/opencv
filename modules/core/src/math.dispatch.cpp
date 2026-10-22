// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "arithm_expr.hpp"
#অন্তর্ভাব "hal_replacement.hpp"
#অন্তর্ভাব "math.simd.hpp"
#অন্তর্ভাব "math.simd_declarations.hpp"

নামাকাশ cv { নামাকাশ ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
template<typename T>
স্থির পূর্ণসংখ্যা halUnaryKernel(ধ্রুবক শূন্য* src0_, size_t s0y, size_t s0x,
                          ধ্রুবক শূন্য*, size_t, size_t, ধ্রুবক শূন্য*, size_t, size_t,
                          শূন্য* dst_, size_t dsty, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height,
                          ধ্রুবক দ্বিগুণ*, পূর্ণসংখ্যা, শূন্য* userdata)
{
    প্রকারলেখ পূর্ণসংখ্যা (*HalFunc)(ধ্রুবক T*, T*, পূর্ণসংখ্যা);
    ধ্রুবক HalFunc fn = (HalFunc)userdata;
    s0y /= আকার(T);
    dsty /= আকার(T);
    CV_Assert(s0x <= 1u);
    ধ্রুবক T* src0 = (ধ্রুবক T*)src0_;
    T* dst = (T*)dst_;
    যদি (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ধ্রুবক পূর্ণসংখ্যা urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    জন্য (পূর্ণসংখ্যা y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        পূর্ণসংখ্যা code;
        যদি (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            জন্য (পূর্ণসংখ্যা x = 0; x < width; x++) dst[x] = v;
        }
        অন্যথা
            code = fn(src0, dst, width);
        যদি (code != CV_HAL_ERROR_OK)
            ফেরত code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    জন্য (পূর্ণসংখ্যা y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*আকার(T));
    ফেরত 0;
}

template<typename T, typename HalFunc>
স্থির TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    যদি (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        ফেরত {halUnaryKernel<T>, (শূন্য*)fn, 0};
    ফেরত {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
স্থির TKernel getEngineMathFunc(শীর্ষ op, পূর্ণসংখ্যা T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
শূন্য mathSpanEngine(শীর্ষ op, পূর্ণসংখ্যা depth, ধ্রুবক শূন্য* src, শূন্য* dst, পূর্ণসংখ্যা n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    ধ্রুবক দ্বিগুণ noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(শীর্ষ op, পূর্ণসংখ্যা T)
{
    যদি ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        স্থির ধ্রুবক TKernel exp32 = probeHalUnary<দশমিক >(cv_hal_exp32f);
        স্থির ধ্রুবক TKernel exp64 = probeHalUnary<দ্বিগুণ>(cv_hal_exp64f);
        স্থির ধ্রুবক TKernel log32 = probeHalUnary<দশমিক >(cv_hal_log32f);
        স্থির ধ্রুবক TKernel log64 = probeHalUnary<দ্বিগুণ>(cv_hal_log64f);
        ধ্রুবক TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        যদি (k->fptr)
            ফেরত *k;
    }
    ফেরত getEngineMathFunc(op, T);
}

TKernel getPowFunc(পূর্ণসংখ্যা T, পূর্ণসংখ্যা R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
