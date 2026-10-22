// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#அந்தர்பாவ "mathfuncs_core.simd.hpp"
#அந்தர்பாவ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv { நாமாகாஶ hal {

வெறுமை cartToPolar32f(மாறிலி தசம* x, மாறிலி தசம* y, தசம* mag, தசம* angle, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை cartToPolar64f(மாறிலி இரட்டை* x, மாறிலி இரட்டை* y, இரட்டை* mag, இரட்டை* angle, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை polarToCart32f(மாறிலி தசம* mag, மாறிலி தசம* angle, தசம* x, தசம* y, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை polarToCart64f(மாறிலி இரட்டை* mag, மாறிலி இரட்டை* angle, இரட்டை* x, இரட்டை* y, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை fastAtan32f(மாறிலி தசம *Y, மாறிலி தசம *X, தசம *angle, முழு len, தருக்க angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை fastAtan64f(மாறிலி இரட்டை *Y, மாறிலி இரட்டை *X, இரட்டை *angle, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
வெறுமை fastAtan2(மாறிலி தசம *Y, மாறிலி தசம *X, தசம *angle, முழு len, தருக்க angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

வெறுமை magnitude32f(மாறிலி தசம* x, மாறிலி தசம* y, தசம* mag, முழு len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_உச்சி_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை magnitude64f(மாறிலி இரட்டை* x, மாறிலி இரட்டை* y, இரட்டை* mag, முழு len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_உச்சி_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


வெறுமை invSqrt32f(மாறிலி தசம* src, தசம* dst, முழு len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


வெறுமை invSqrt64f(மாறிலி இரட்டை* src, இரட்டை* dst, முழு len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


வெறுமை sqrt32f(மாறிலி தசம* src, தசம* dst, முழு len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


வெறுமை sqrt64f(மாறிலி இரட்டை* src, இரட்டை* dst, முழு len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை exp32f(மாறிலி தசம *src, தசம *dst, முழு n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

வெறுமை exp64f(மாறிலி இரட்டை *src, இரட்டை *dst, முழு n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

வெறுமை log32f(மாறிலி தசம *src, தசம *dst, முழு n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

வெறுமை log64f(மாறிலி இரட்டை *src, இரட்டை *dst, முழு n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

வெறுமை exp(மாறிலி தசம* src, தசம* dst, முழு n)
{
    exp32f(src, dst, n);
}

வெறுமை exp(மாறிலி இரட்டை* src, இரட்டை* dst, முழு n)
{
    exp64f(src, dst, n);
}

வெறுமை log(மாறிலி தசம* src, தசம* dst, முழு n)
{
    log32f(src, dst, n);
}

வெறுமை log(மாறிலி இரட்டை* src, இரட்டை* dst, முழு n)
{
    log64f(src, dst, n);
}

வெறுமை magnitude(மாறிலி தசம* x, மாறிலி தசம* y, தசம* dst, முழு n)
{
    magnitude32f(x, y, dst, n);
}

வெறுமை magnitude(மாறிலி இரட்டை* x, மாறிலி இரட்டை* y, இரட்டை* dst, முழு n)
{
    magnitude64f(x, y, dst, n);
}

வெறுமை sqrt(மாறிலி தசம* src, தசம* dst, முழு len)
{
    sqrt32f(src, dst, len);
}

வெறுமை sqrt(மாறிலி இரட்டை* src, இரட்டை* dst, முழு len)
{
    sqrt64f(src, dst, len);
}

வெறுமை invSqrt(மாறிலி தசம* src, தசம* dst, முழு len)
{
    invSqrt32f(src, dst, len);
}

வெறுமை invSqrt(மாறிலி இரட்டை* src, இரட்டை* dst, முழு len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

தசம cv::fastAtan2( தசம y, தசம x )
{
    using நாமாகாஶ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
