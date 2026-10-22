// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "arithm_expr.hpp"
#অন্তর্ভাৱ "hal_replacement.hpp"
#অন্তর্ভাৱ "math.simd.hpp"
#অন্তর্ভাৱ "math.simd_declarations.hpp"

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
স্থির পূর্ণাঙ্ক halUnaryKernel(ধ্রুৱ শূন্য* src0_, size_t s0y, size_t s0x,
                          ধ্রুৱ শূন্য*, size_t, size_t, ধ্রুৱ শূন্য*, size_t, size_t,
                          শূন্য* dst_, size_t dsty, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height,
                          ধ্রুৱ দ্ৱিগুণ*, পূর্ণাঙ্ক, শূন্য* userdata)
{
    প্রকারনির্দেশ পূর্ণাঙ্ক (*HalFunc)(ধ্রুৱ T*, T*, পূর্ণাঙ্ক);
    ধ্রুৱ HalFunc fn = (HalFunc)userdata;
    s0y /= পরিমাণ(T);
    dsty /= পরিমাণ(T);
    CV_Assert(s0x <= 1u);
    ধ্রুৱ T* src0 = (ধ্রুৱ T*)src0_;
    T* dst = (T*)dst_;
    যদি (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ধ্রুৱ পূর্ণাঙ্ক urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    বাবে (পূর্ণাঙ্ক y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        পূর্ণাঙ্ক code;
        যদি (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            বাবে (পূর্ণাঙ্ক x = 0; x < width; x++) dst[x] = v;
        }
        অন্যথা
            code = fn(src0, dst, width);
        যদি (code != CV_HAL_ERROR_OK)
            ঘূৰাই code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    বাবে (পূর্ণাঙ্ক y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*পরিমাণ(T));
    ঘূৰাই 0;
}

template<typename T, typename HalFunc>
স্থির TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    যদি (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        ঘূৰাই {halUnaryKernel<T>, (শূন্য*)fn, 0};
    ঘূৰাই {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
স্থির TKernel getEngineMathFunc(শীৰ্ষ op, পূর্ণাঙ্ক T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
শূন্য mathSpanEngine(শীৰ্ষ op, পূর্ণাঙ্ক depth, ধ্রুৱ শূন্য* src, শূন্য* dst, পূর্ণাঙ্ক n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    ধ্রুৱ দ্ৱিগুণ noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(শীৰ্ষ op, পূর্ণাঙ্ক T)
{
    যদি ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        স্থির ধ্রুৱ TKernel exp32 = probeHalUnary<ভিন্ন >(cv_hal_exp32f);
        স্থির ধ্রুৱ TKernel exp64 = probeHalUnary<দ্ৱিগুণ>(cv_hal_exp64f);
        স্থির ধ্রুৱ TKernel log32 = probeHalUnary<ভিন্ন >(cv_hal_log32f);
        স্থির ধ্রুৱ TKernel log64 = probeHalUnary<দ্ৱিগুণ>(cv_hal_log64f);
        ধ্রুৱ TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        যদি (k->fptr)
            ঘূৰাই *k;
    }
    ঘূৰাই getEngineMathFunc(op, T);
}

TKernel getPowFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
