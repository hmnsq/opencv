// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"
#ಅನ್ತರ್ಭಾವ "hal_replacement.hpp"
#ಅನ್ತರ್ಭಾವ "math.simd.hpp"
#ಅನ್ತರ್ಭಾವ "math.simd_declarations.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ halUnaryKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                          ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                          ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                          ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ* userdata)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*HalFunc)(ಸ್ಥಿರಾಂಕ T*, T*, ಪೂರ್ಣಾಂಕ);
    ಸ್ಥಿರಾಂಕ HalFunc fn = (HalFunc)userdata;
    s0y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(T);
    CV_Assert(s0x <= 1u);
    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    T* dst = (T*)dst_;
    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && s0y == s0x*(size_t)ಅಗಲ) { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ urows = (s0y == 0 && ಎತ್ತರ > 1) ? 1 : ಎತ್ತರ;   // vertical broadcast: 1 row + copies
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;
        ಆದರೆ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            ಸಂಕೇತ_2 = fn(src0, &v, 1);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++) dst[x] = v;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            ಸಂಕೇತ_2 = fn(src0, dst, ಅಗಲ);
        ಆದರೆ (ಸಂಕೇತ_2 != CV_HAL_ERROR_OK)
            ಹಿಂತಿರುಗಿ ಸಂಕೇತ_2;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = urows; y < ಎತ್ತರ; y++)
        ಸ್ಮರಣೆ_ನಕಲು(dst + (size_t)y*dsty, dst, (size_t)ಅಗಲ*ಗಾತ್ರ(T));
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು HalFunc>
ಸ್ಥಿರ TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    ಆದರೆ (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        ಹಿಂತಿರುಗಿ {halUnaryKernel<T>, (ಶೂನ್ಯ*)fn, 0};
    ಹಿಂತಿರುಗಿ {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
ಸ್ಥಿರ TKernel getEngineMathFunc(ತುದಿ op, ಪೂರ್ಣಾಂಕ T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
ಶೂನ್ಯ math_ಹರವು_Engine(ತುದಿ op, ಪೂರ್ಣಾಂಕ ಆಳ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ n)
{
    TKernel k = getEngineMathFunc(op, ಆಳ);
    CV_Assert(k.fptr);
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ noparams[4] = {};
    k.fptr(src, 0, 1, ಶೂನ್ಯಸೂಚಕ, 0, 0, ಶೂನ್ಯಸೂಚಕ, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(ತುದಿ op, ಪೂರ್ಣಾಂಕ T)
{
    ಆದರೆ ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ TKernel exp32 = probeHalUnary<ದಶಮಾಂಶ >(cv_hal_exp32f);
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ TKernel exp64 = probeHalUnary<ದ್ವಿಗುಣ>(cv_hal_exp64f);
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ TKernel log32 = probeHalUnary<ದಶಮಾಂಶ >(cv_hal_log32f);
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ TKernel log64 = probeHalUnary<ದ್ವಿಗುಣ>(cv_hal_log64f);
        ಸ್ಥಿರಾಂಕ TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        ಆದರೆ (k->fptr)
            ಹಿಂತಿರುಗಿ *k;
    }
    ಹಿಂತಿರುಗಿ getEngineMathFunc(op, T);
}

TKernel getPowFunc(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
