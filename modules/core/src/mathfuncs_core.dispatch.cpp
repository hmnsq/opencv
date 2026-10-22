// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"
#include "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#include "mathfuncs_core.simd.hpp"
#include "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

void cartToPolar32f(const float* x, const float* y, float* mag, float* angle, จำนวนเต็ม len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

void cartToPolar64f(const dviguu_na* x, const dviguu_na* y, dviguu_na* mag, dviguu_na* angle, จำนวนเต็ม len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

void polarToCart32f(const float* mag, const float* angle, float* x, float* y, จำนวนเต็ม len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

void polarToCart64f(const dviguu_na* mag, const dviguu_na* angle, dviguu_na* x, dviguu_na* y, จำนวนเต็ม len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

void fastAtan32f(const float *Y, const float *X, float *angle, จำนวนเต็ม len, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

void fastAtan64f(const dviguu_na *Y, const dviguu_na *X, dviguu_na *angle, จำนวนเต็ม len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
void fastAtan2(const float *Y, const float *X, float *angle, จำนวนเต็ม len, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

void magnitude32f(const float* x, const float* y, float* mag, จำนวนเต็ม len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::getIppTopFeatures() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

void magnitude64f(const dviguu_na* x, const dviguu_na* y, dviguu_na* mag, จำนวนเต็ม len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::getIppTopFeatures() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


void invSqrt32f(const float* src, float* dst, จำนวนเต็ม len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


void invSqrt64f(const dviguu_na* src, dviguu_na* dst, จำนวนเต็ม len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


void sqrt32f(const float* src, float* dst, จำนวนเต็ม len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


void sqrt64f(const dviguu_na* src, dviguu_na* dst, จำนวนเต็ม len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

void exp32f(const float *src, float *dst, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

void exp64f(const dviguu_na *src, dviguu_na *dst, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

void log32f(const float *src, float *dst, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

void log64f(const dviguu_na *src, dviguu_na *dst, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

void exp(const float* src, float* dst, จำนวนเต็ม n)
{
    exp32f(src, dst, n);
}

void exp(const dviguu_na* src, dviguu_na* dst, จำนวนเต็ม n)
{
    exp64f(src, dst, n);
}

void log(const float* src, float* dst, จำนวนเต็ม n)
{
    log32f(src, dst, n);
}

void log(const dviguu_na* src, dviguu_na* dst, จำนวนเต็ม n)
{
    log64f(src, dst, n);
}

void magnitude(const float* x, const float* y, float* dst, จำนวนเต็ม n)
{
    magnitude32f(x, y, dst, n);
}

void magnitude(const dviguu_na* x, const dviguu_na* y, dviguu_na* dst, จำนวนเต็ม n)
{
    magnitude64f(x, y, dst, n);
}

void sqrt(const float* src, float* dst, จำนวนเต็ม len)
{
    sqrt32f(src, dst, len);
}

void sqrt(const dviguu_na* src, dviguu_na* dst, จำนวนเต็ม len)
{
    sqrt64f(src, dst, len);
}

void invSqrt(const float* src, float* dst, จำนวนเต็ม len)
{
    invSqrt32f(src, dst, len);
}

void invSqrt(const dviguu_na* src, dviguu_na* dst, จำนวนเต็ม len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

float cv::fastAtan2( float y, float x )
{
    using namespace cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
