// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#अन्तर्भाव "mathfuncs_core.simd.hpp"
#अन्तर्भाव "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv { नामाकाश hal {

शून्य cartToPolar32f(ध्रुव भिन्न* x, ध्रुव भिन्न* y, भिन्न* mag, भिन्न* angle, पूर्णाङ्क len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य cartToPolar64f(ध्रुव द्विगुण* x, ध्रुव द्विगुण* y, द्विगुण* mag, द्विगुण* angle, पूर्णाङ्क len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य polarToCart32f(ध्रुव भिन्न* mag, ध्रुव भिन्न* angle, भिन्न* x, भिन्न* y, पूर्णाङ्क len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य polarToCart64f(ध्रुव द्विगुण* mag, ध्रुव द्विगुण* angle, द्विगुण* x, द्विगुण* y, पूर्णाङ्क len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य fastAtan32f(ध्रुव भिन्न *Y, ध्रुव भिन्न *X, भिन्न *angle, पूर्णाङ्क len, तार्किक angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य fastAtan64f(ध्रुव द्विगुण *Y, ध्रुव द्विगुण *X, द्विगुण *angle, पूर्णाङ्क len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
शून्य fastAtan2(ध्रुव भिन्न *Y, ध्रुव भिन्न *X, भिन्न *angle, पूर्णाङ्क len, तार्किक angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

शून्य magnitude32f(ध्रुव भिन्न* x, ध्रुव भिन्न* y, भिन्न* mag, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_शीर्ष_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य magnitude64f(ध्रुव द्विगुण* x, ध्रुव द्विगुण* y, द्विगुण* mag, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_शीर्ष_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


शून्य invSqrt32f(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


शून्य invSqrt64f(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


शून्य sqrt32f(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


शून्य sqrt64f(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य exp32f(ध्रुव भिन्न *src, भिन्न *dst, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

शून्य exp64f(ध्रुव द्विगुण *src, द्विगुण *dst, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

शून्य log32f(ध्रुव भिन्न *src, भिन्न *dst, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

शून्य log64f(ध्रुव द्विगुण *src, द्विगुण *dst, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

शून्य exp(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क n)
{
    exp32f(src, dst, n);
}

शून्य exp(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क n)
{
    exp64f(src, dst, n);
}

शून्य log(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क n)
{
    log32f(src, dst, n);
}

शून्य log(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क n)
{
    log64f(src, dst, n);
}

शून्य magnitude(ध्रुव भिन्न* x, ध्रुव भिन्न* y, भिन्न* dst, पूर्णाङ्क n)
{
    magnitude32f(x, y, dst, n);
}

शून्य magnitude(ध्रुव द्विगुण* x, ध्रुव द्विगुण* y, द्विगुण* dst, पूर्णाङ्क n)
{
    magnitude64f(x, y, dst, n);
}

शून्य वर्ग_मूल(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क len)
{
    sqrt32f(src, dst, len);
}

शून्य वर्ग_मूल(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क len)
{
    sqrt64f(src, dst, len);
}

शून्य invSqrt(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क len)
{
    invSqrt32f(src, dst, len);
}

शून्य invSqrt(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

भिन्न cv::fastAtan2( भिन्न y, भिन्न x )
{
    using नामाकाश cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
