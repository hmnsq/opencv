// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "arithm_expr.hpp"
#अन्तर्भाव "hal_replacement.hpp"
#अन्तर्भाव "math.simd.hpp"
#अन्तर्भाव "math.simd_declarations.hpp"

नामाकाश cv { नामाकाश ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
ढाँचा<typename T>
स्थिर पूर्णाङ्क halUnaryKernel(ध्रुव शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                          ध्रुव शून्य*, माप_प्रकार, माप_प्रकार, ध्रुव शून्य*, माप_प्रकार, माप_प्रकार,
                          शून्य* dst_, माप_प्रकार dsty, पूर्णाङ्क width, पूर्णाङ्क height,
                          ध्रुव द्विगुण*, पूर्णाङ्क, शून्य* userdata)
{
    प्रकारनिर्देश पूर्णाङ्क (*HalFunc)(ध्रुव T*, T*, पूर्णाङ्क);
    ध्रुव HalFunc fn = (HalFunc)userdata;
    s0y /= परिमाण(T);
    dsty /= परिमाण(T);
    CV_Assert(s0x <= 1u);
    ध्रुव T* src0 = (ध्रुव T*)src0_;
    T* dst = (T*)dst_;
    यदि (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width) { width *= height; height = 1; }
    ध्रुव पूर्णाङ्क urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    प्रति (पूर्णाङ्क y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        पूर्णाङ्क code;
        यदि (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            प्रति (पूर्णाङ्क x = 0; x < width; x++) dst[x] = v;
        }
        अन्यथा
            code = fn(src0, dst, width);
        यदि (code != CV_HAL_ERROR_OK)
            प्रत्यागम code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    प्रति (पूर्णाङ्क y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*परिमाण(T));
    प्रत्यागम 0;
}

ढाँचा<typename T, typename HalFunc>
स्थिर TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    यदि (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        प्रत्यागम {halUnaryKernel<T>, (शून्य*)fn, 0};
    प्रत्यागम {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
स्थिर TKernel getEngineMathFunc(शीर्ष op, पूर्णाङ्क T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
शून्य mathSpanEngine(शीर्ष op, पूर्णाङ्क depth, ध्रुव शून्य* src, शून्य* dst, पूर्णाङ्क n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    ध्रुव द्विगुण noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(शीर्ष op, पूर्णाङ्क T)
{
    यदि ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        स्थिर ध्रुव TKernel exp32 = probeHalUnary<भिन्न >(cv_hal_exp32f);
        स्थिर ध्रुव TKernel exp64 = probeHalUnary<द्विगुण>(cv_hal_exp64f);
        स्थिर ध्रुव TKernel log32 = probeHalUnary<भिन्न >(cv_hal_log32f);
        स्थिर ध्रुव TKernel log64 = probeHalUnary<द्विगुण>(cv_hal_log64f);
        ध्रुव TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        यदि (k->fptr)
            प्रत्यागम *k;
    }
    प्रत्यागम getEngineMathFunc(op, T);
}

TKernel getPowFunc(पूर्णाङ्क T, पूर्णाङ्क R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
