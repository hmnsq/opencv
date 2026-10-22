// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"
#समावेश "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#समावेश "mathfuncs_core.simd.hpp"
#समावेश "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

व्योम cartToPolar32f(स्थिर भग्न* x, स्थिर भग्न* y, भग्न* mag, भग्न* angle, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम cartToPolar64f(स्थिर द्विगुन* x, स्थिर द्विगुन* y, द्विगुन* mag, द्विगुन* angle, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम polarToCart32f(स्थिर भग्न* mag, स्थिर भग्न* angle, भग्न* x, भग्न* y, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम polarToCart64f(स्थिर द्विगुन* mag, स्थिर द्विगुन* angle, द्विगुन* x, द्विगुन* y, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम fastAtan32f(स्थिर भग्न *Y, स्थिर भग्न *X, भग्न *angle, पूर्णांक len, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम fastAtan64f(स्थिर द्विगुन *Y, स्थिर द्विगुन *X, द्विगुन *angle, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
व्योम fastAtan2(स्थिर भग्न *Y, स्थिर भग्न *X, भग्न *angle, पूर्णांक len, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

व्योम magnitude32f(स्थिर भग्न* x, स्थिर भग्न* y, भग्न* mag, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_शीर्ष_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम magnitude64f(स्थिर द्विगुन* x, स्थिर द्विगुन* y, द्विगुन* mag, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_शीर्ष_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


व्योम invSqrt32f(स्थिर भग्न* src, भग्न* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


व्योम invSqrt64f(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


व्योम sqrt32f(स्थिर भग्न* src, भग्न* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


व्योम sqrt64f(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम exp32f(स्थिर भग्न *src, भग्न *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

व्योम exp64f(स्थिर द्विगुन *src, द्विगुन *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

व्योम log32f(स्थिर भग्न *src, भग्न *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

व्योम log64f(स्थिर द्विगुन *src, द्विगुन *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

व्योम exp(स्थिर भग्न* src, भग्न* dst, पूर्णांक n)
{
    exp32f(src, dst, n);
}

व्योम exp(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक n)
{
    exp64f(src, dst, n);
}

व्योम log(स्थिर भग्न* src, भग्न* dst, पूर्णांक n)
{
    log32f(src, dst, n);
}

व्योम log(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक n)
{
    log64f(src, dst, n);
}

व्योम magnitude(स्थिर भग्न* x, स्थिर भग्न* y, भग्न* dst, पूर्णांक n)
{
    magnitude32f(x, y, dst, n);
}

व्योम magnitude(स्थिर द्विगुन* x, स्थिर द्विगुन* y, द्विगुन* dst, पूर्णांक n)
{
    magnitude64f(x, y, dst, n);
}

व्योम वर्ग_मूल(स्थिर भग्न* src, भग्न* dst, पूर्णांक len)
{
    sqrt32f(src, dst, len);
}

व्योम वर्ग_मूल(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक len)
{
    sqrt64f(src, dst, len);
}

व्योम invSqrt(स्थिर भग्न* src, भग्न* dst, पूर्णांक len)
{
    invSqrt32f(src, dst, len);
}

व्योम invSqrt(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

भग्न cv::fastAtan2( भग्न y, भग्न x )
{
    using namespace cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
