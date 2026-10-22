// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise MATH + SELECT kernels (math.simd.hpp) - the sibling of
// arithm.dispatch.cpp: plain functions forwarding to the CPU-optimal kernel via CV_CPU_DISPATCH.
// getElemwiseFunc (arithm.dispatch.cpp) routes the corresponding TOps here.

#समावेश "precomp.hpp"
#समावेश "arithm_expr.hpp"
#समावेश "hal_replacement.hpp"
#समावेश "math.simd.hpp"
#समावेश "math.simd_declarations.hpp"

namespace cv { namespace ew {

// ---- pluggable-HAL bridge for exp/log --------------------------------------------------------
// The raw cv_hal_* entry points return int for a reason: without an installed HAL they are stubs
// returning CV_HAL_ERROR_NOT_IMPLEMENTED. getMathFunc PROBES each one once (a 1-element call on
// the safe input 1.0 - fine for both exp and log): implemented -> wrap it as an engine kernel
// (the function pointer rides in TKernel::userdata, castKernel-style) and the engine adds tiling
// and parallelism on top of the vendor code; not implemented -> the engine's own v_exp/v_log
// kernels. Uniform over ANY HAL (an external vendor one, the IPP HAL module, ...) - the get is
// called once per program build, the probe cost is nothing next to the kernel calls that follow.
ढाँचा<typename T>
अटल पूर्णांक halUnaryKernel(स्थिर व्योम* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                          स्थिर व्योम*, माप_प्रकार, माप_प्रकार, स्थिर व्योम*, माप_प्रकार, माप_प्रकार,
                          व्योम* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                          स्थिर द्विगुन*, पूर्णांक, व्योम* userdata)
{
    प्रकार पूर्णांक (*HalFunc)(स्थिर T*, T*, पूर्णांक);
    स्थिर HalFunc fn = (HalFunc)userdata;
    s0y /= माप(T);
    dsty /= माप(T);
    CV_Assert(s0x <= 1u);
    स्थिर T* src0 = (स्थिर T*)src0_;
    T* dst = (T*)dst_;
    अगर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width) { width *= height; height = 1; }
    स्थिर पूर्णांक urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    क्रम (पूर्णांक y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        पूर्णांक code;
        अगर (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v;
            code = fn(src0, &v, 1);
            क्रम (पूर्णांक x = 0; x < width; x++) dst[x] = v;
        }
        अन्यथा
            code = fn(src0, dst, width);
        अगर (code != CV_HAL_ERROR_OK)
            वापस code;                  // shouldn't happen (probed at get time) - let exec assert
    }
    dst = (T*)dst_;
    क्रम (पूर्णांक y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*माप(T));
    वापस 0;
}

ढाँचा<typename T, typename HalFunc>
अटल TKernel probeHalUnary(HalFunc fn)
{
    T one = (T)1, r = (T)0;
    अगर (fn(&one, &r, 1) == CV_HAL_ERROR_OK)
        वापस {halUnaryKernel<T>, (व्योम*)fn, 0};
    वापस {};
}

// The engine's OWN kernel for (op, T) - v_exp/v_log & co, no HAL/IPP tiers. The final fallback of
// getMathFunc, and what hal::exp32f & co use as THEIR built-in implementation (the former table
// kernels are gone), via mathSpanEngine below.
अटल TKernel getEngineMathFunc(शीर्ष op, पूर्णांक T)
{
    CV_CPU_DISPATCH(getMathFunc_,   (op, T),      CV_CPU_DISPATCH_MODES_ALL);
}

// run the engine's own math kernel over one contiguous span (the shape hal::exp32f & co need)
व्योम mathSpanEngine(शीर्ष op, पूर्णांक depth, स्थिर व्योम* src, व्योम* dst, पूर्णांक n)
{
    TKernel k = getEngineMathFunc(op, depth);
    CV_Assert(k.fptr);
    स्थिर द्विगुन noparams[4] = {};
    k.fptr(src, 0, 1, nullptr, 0, 0, nullptr, 0, 0, dst, 0, n, 1, noparams, k.flags, k.userdata);
}

TKernel getMathFunc(शीर्ष op, पूर्णांक T)
{
    अगर ((op == OP_EXP || op == OP_LOG) && (T == CV_32F || T == CV_64F))
    {
        // IPP now rides the cv_hal_* hooks too (hal/ipp), so the single probe below picks it up
        // together with any external vendor HAL - no separate IPP tier needed here.
        // probe results are process-lifetime stable; cache them (thread-safe magic statics)
        अटल स्थिर TKernel exp32 = probeHalUnary<भग्न >(cv_hal_exp32f);
        अटल स्थिर TKernel exp64 = probeHalUnary<द्विगुन>(cv_hal_exp64f);
        अटल स्थिर TKernel log32 = probeHalUnary<भग्न >(cv_hal_log32f);
        अटल स्थिर TKernel log64 = probeHalUnary<द्विगुन>(cv_hal_log64f);
        स्थिर TKernel* k = op == OP_EXP ? (T == CV_32F ? &exp32 : &exp64)
                                        : (T == CV_32F ? &log32 : &log64);
        अगर (k->fptr)
            वापस *k;
    }
    वापस getEngineMathFunc(op, T);
}

TKernel getPowFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getPowFunc_,    (T, R),       CV_CPU_DISPATCH_MODES_ALL); }

}} // namespace cv::ew
