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
स्थिर पूर्णांक halUnaryKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                          स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार, स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                          शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                          स्थिरांक द्विगुण*, पूर्णांक, शून्य* userdata)
{
    प्रकारव्याख्या पूर्णांक (*HalFunc)(स्थिरांक T*, T*, पूर्णांक);
    स्थिरांक HalFunc fn = (HalFunc)userdata;
    s0y /= आकार(T);
    dsty /= आकार(T);
    CV_Assert(s0x <= 1u);
    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    T* dst = (T*)dst_;
    जर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width) { width *= height; height = 1; }
    स्थिरांक पूर्णांक urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    साठी (पूर्णांक y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        पूर्णांक code;
        जर (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            साठी (पूर्णांक x = 0; x < width; x++) dst[x] = v;
        }
        अन्यथा
            code = fn(src0, dst, width);
        जर (code != CV_HAL_ERROR_OK)
            परत code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    साठी (पूर्णांक y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*आकार(T));
    परत 0;
}

ढाँचा<typename T, typename HalFunc>
स्थिर TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    जर (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        परत {halUnaryKernel<T>, (शून्य*)fn, 0};
    परत {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
स्थिर TKernel getEngineMathFunc(शीर्ष op, पूर्णांक T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
शून्य mathSpanEngine(शीर्ष op, पूर्णांक depth, स्थिरांक शून्य* src, शून्य* dst, पूर्णांक n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    स्थिरांक द्विगुण noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(शीर्ष op, पूर्णांक T)
{
    जर ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        स्थिर स्थिरांक TKernel exp32 = probeHalUnary<दशांश >(cv_hal_exp32f);
        स्थिर स्थिरांक TKernel exp64 = probeHalUnary<द्विगुण>(cv_hal_exp64f);
        स्थिर स्थिरांक TKernel log32 = probeHalUnary<दशांश >(cv_hal_log32f);
        स्थिर स्थिरांक TKernel log64 = probeHalUnary<द्विगुण>(cv_hal_log64f);
        स्थिरांक TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        जर (k->fptr)
            परत *k;
    }
    परत getEngineMathFunc(op, T);
}

TKernel getPowFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
