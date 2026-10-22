// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#ଅନ୍ତର୍ଭାଵ "mathfuncs_core.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv { ନାମାକାଶ hal {

ଶୂନ୍ୟ cartToPolar32f(ଅଚଳ ଦଶମିକ* x, ଅଚଳ ଦଶମିକ* y, ଦଶମିକ* mag, ଦଶମିକ* angle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ତାର୍କିକ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ cartToPolar64f(ଅଚଳ ଦ୍ୱିଗୁଣ* x, ଅଚଳ ଦ୍ୱିଗୁଣ* y, ଦ୍ୱିଗୁଣ* mag, ଦ୍ୱିଗୁଣ* angle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ତାର୍କିକ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ polarToCart32f(ଅଚଳ ଦଶମିକ* mag, ଅଚଳ ଦଶମିକ* angle, ଦଶମିକ* x, ଦଶମିକ* y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ତାର୍କିକ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ polarToCart64f(ଅଚଳ ଦ୍ୱିଗୁଣ* mag, ଅଚଳ ଦ୍ୱିଗୁଣ* angle, ଦ୍ୱିଗୁଣ* x, ଦ୍ୱିଗୁଣ* y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ତାର୍କିକ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ fastAtan32f(ଅଚଳ ଦଶମିକ *Y, ଅଚଳ ଦଶମିକ *X, ଦଶମିକ *angle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ତାର୍କିକ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ fastAtan64f(ଅଚଳ ଦ୍ୱିଗୁଣ *Y, ଅଚଳ ଦ୍ୱିଗୁଣ *X, ଦ୍ୱିଗୁଣ *angle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ତାର୍କିକ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
ଶୂନ୍ୟ fastAtan2(ଅଚଳ ଦଶମିକ *Y, ଅଚଳ ଦଶମିକ *X, ଦଶମିକ *angle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ତାର୍କିକ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

ଶୂନ୍ୟ magnitude32f(ଅଚଳ ଦଶମିକ* x, ଅଚଳ ଦଶମିକ* y, ଦଶମିକ* mag, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_ଶୀର୍ଷ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ magnitude64f(ଅଚଳ ଦ୍ୱିଗୁଣ* x, ଅଚଳ ଦ୍ୱିଗୁଣ* y, ଦ୍ୱିଗୁଣ* mag, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_ଶୀର୍ଷ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ଶୂନ୍ୟ invSqrt32f(ଅଚଳ ଦଶମିକ* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ଶୂନ୍ୟ invSqrt64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ଶୂନ୍ୟ sqrt32f(ଅଚଳ ଦଶମିକ* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ଶୂନ୍ୟ sqrt64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ exp32f(ଅଚଳ ଦଶମିକ *src, ଦଶମିକ *dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ଶୂନ୍ୟ exp64f(ଅଚଳ ଦ୍ୱିଗୁଣ *src, ଦ୍ୱିଗୁଣ *dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ଶୂନ୍ୟ log32f(ଅଚଳ ଦଶମିକ *src, ଦଶମିକ *dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ଶୂନ୍ୟ log64f(ଅଚଳ ଦ୍ୱିଗୁଣ *src, ଦ୍ୱିଗୁଣ *dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

ଶୂନ୍ୟ exp(ଅଚଳ ଦଶମିକ* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    exp32f(src, dst, n);
}

ଶୂନ୍ୟ exp(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    exp64f(src, dst, n);
}

ଶୂନ୍ୟ log(ଅଚଳ ଦଶମିକ* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    log32f(src, dst, n);
}

ଶୂନ୍ୟ log(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    log64f(src, dst, n);
}

ଶୂନ୍ୟ magnitude(ଅଚଳ ଦଶମିକ* x, ଅଚଳ ଦଶମିକ* y, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    magnitude32f(x, y, dst, n);
}

ଶୂନ୍ୟ magnitude(ଅଚଳ ଦ୍ୱିଗୁଣ* x, ଅଚଳ ଦ୍ୱିଗୁଣ* y, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    magnitude64f(x, y, dst, n);
}

ଶୂନ୍ୟ sqrt(ଅଚଳ ଦଶମିକ* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    sqrt32f(src, dst, len);
}

ଶୂନ୍ୟ sqrt(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    sqrt64f(src, dst, len);
}

ଶୂନ୍ୟ invSqrt(ଅଚଳ ଦଶମିକ* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    invSqrt32f(src, dst, len);
}

ଶୂନ୍ୟ invSqrt(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

ଦଶମିକ cv::fastAtan2( ଦଶମିକ y, ଦଶମିକ x )
{
    using ନାମାକାଶ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
