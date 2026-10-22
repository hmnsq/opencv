// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#ಅನ್ತರ್ಭಾವ "mathfuncs_core.simd.hpp"
#ಅನ್ತರ್ಭಾವ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಶೂನ್ಯ cartToPolar32f(ಧ್ರುವ ಭಿನ್ನ* x, ಧ್ರುವ ಭಿನ್ನ* y, ಭಿನ್ನ* mag, ಭಿನ್ನ* angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ cartToPolar64f(ಧ್ರುವ ದ್ವಿಗುಣ* x, ಧ್ರುವ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ದ್ವಿಗುಣ* angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ polarToCart32f(ಧ್ರುವ ಭಿನ್ನ* mag, ಧ್ರುವ ಭಿನ್ನ* angle, ಭಿನ್ನ* x, ಭಿನ್ನ* y, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ polarToCart64f(ಧ್ರುವ ದ್ವಿಗುಣ* mag, ಧ್ರುವ ದ್ವಿಗುಣ* angle, ದ್ವಿಗುಣ* x, ದ್ವಿಗುಣ* y, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ fastAtan32f(ಧ್ರುವ ಭಿನ್ನ *Y, ಧ್ರುವ ಭಿನ್ನ *X, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ fastAtan64f(ಧ್ರುವ ದ್ವಿಗುಣ *Y, ಧ್ರುವ ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
ಶೂನ್ಯ fastAtan2(ಧ್ರುವ ಭಿನ್ನ *Y, ಧ್ರುವ ಭಿನ್ನ *X, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

ಶೂನ್ಯ magnitude32f(ಧ್ರುವ ಭಿನ್ನ* x, ಧ್ರುವ ಭಿನ್ನ* y, ಭಿನ್ನ* mag, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_ತುದಿ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ magnitude64f(ಧ್ರುವ ದ್ವಿಗುಣ* x, ಧ್ರುವ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_ತುದಿ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ಶೂನ್ಯ invSqrt32f(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ಶೂನ್ಯ invSqrt64f(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ಶೂನ್ಯ sqrt32f(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ಶೂನ್ಯ sqrt64f(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ exp32f(ಧ್ರುವ ಭಿನ್ನ *src, ಭಿನ್ನ *dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ಶೂನ್ಯ exp64f(ಧ್ರುವ ದ್ವಿಗುಣ *src, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ಶೂನ್ಯ log32f(ಧ್ರುವ ಭಿನ್ನ *src, ಭಿನ್ನ *dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ಶೂನ್ಯ log64f(ಧ್ರುವ ದ್ವಿಗುಣ *src, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

ಶೂನ್ಯ exp(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    exp32f(src, dst, n);
}

ಶೂನ್ಯ exp(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    exp64f(src, dst, n);
}

ಶೂನ್ಯ log(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    log32f(src, dst, n);
}

ಶೂನ್ಯ log(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    log64f(src, dst, n);
}

ಶೂನ್ಯ magnitude(ಧ್ರುವ ಭಿನ್ನ* x, ಧ್ರುವ ಭಿನ್ನ* y, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    magnitude32f(x, y, dst, n);
}

ಶೂನ್ಯ magnitude(ಧ್ರುವ ದ್ವಿಗುಣ* x, ಧ್ರುವ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    magnitude64f(x, y, dst, n);
}

ಶೂನ್ಯ sqrt(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    sqrt32f(src, dst, len);
}

ಶೂನ್ಯ sqrt(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    sqrt64f(src, dst, len);
}

ಶೂನ್ಯ invSqrt(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    invSqrt32f(src, dst, len);
}

ಶೂನ್ಯ invSqrt(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

ಭಿನ್ನ cv::fastAtan2( ಭಿನ್ನ y, ಭಿನ್ನ x )
{
    using ನಾಮಾಕಾಶ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
