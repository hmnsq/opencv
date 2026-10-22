// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#અન્તર્ભાવ "mathfuncs_core.simd.hpp"
#અન્તર્ભાવ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv { નામાકાશ hal {

શૂન્ય cartToPolar32f(અચલ દશાંશ* x, અચલ દશાંશ* y, દશાંશ* mag, દશાંશ* angle, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય cartToPolar64f(અચલ દ્વિગુણ* x, અચલ દ્વિગુણ* y, દ્વિગુણ* mag, દ્વિગુણ* angle, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય polarToCart32f(અચલ દશાંશ* mag, અચલ દશાંશ* angle, દશાંશ* x, દશાંશ* y, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય polarToCart64f(અચલ દ્વિગુણ* mag, અચલ દ્વિગુણ* angle, દ્વિગુણ* x, દ્વિગુણ* y, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય fastAtan32f(અચલ દશાંશ *Y, અચલ દશાંશ *X, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય fastAtan64f(અચલ દ્વિગુણ *Y, અચલ દ્વિગુણ *X, દ્વિગુણ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
શૂન્ય fastAtan2(અચલ દશાંશ *Y, અચલ દશાંશ *X, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

શૂન્ય magnitude32f(અચલ દશાંશ* x, અચલ દશાંશ* y, દશાંશ* mag, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_ટોચ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય magnitude64f(અચલ દ્વિગુણ* x, અચલ દ્વિગુણ* y, દ્વિગુણ* mag, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_ટોચ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


શૂન્ય invSqrt32f(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


શૂન્ય invSqrt64f(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


શૂન્ય sqrt32f(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


શૂન્ય sqrt64f(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય exp32f(અચલ દશાંશ *src, દશાંશ *dst, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

શૂન્ય exp64f(અચલ દ્વિગુણ *src, દ્વિગુણ *dst, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

શૂન્ય log32f(અચલ દશાંશ *src, દશાંશ *dst, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

શૂન્ય log64f(અચલ દ્વિગુણ *src, દ્વિગુણ *dst, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

શૂન્ય exp(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક n)
{
    exp32f(src, dst, n);
}

શૂન્ય exp(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક n)
{
    exp64f(src, dst, n);
}

શૂન્ય log(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક n)
{
    log32f(src, dst, n);
}

શૂન્ય log(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક n)
{
    log64f(src, dst, n);
}

શૂન્ય magnitude(અચલ દશાંશ* x, અચલ દશાંશ* y, દશાંશ* dst, પૂર્ણાંક n)
{
    magnitude32f(x, y, dst, n);
}

શૂન્ય magnitude(અચલ દ્વિગુણ* x, અચલ દ્વિગુણ* y, દ્વિગુણ* dst, પૂર્ણાંક n)
{
    magnitude64f(x, y, dst, n);
}

શૂન્ય sqrt(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક len)
{
    sqrt32f(src, dst, len);
}

શૂન્ય sqrt(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક len)
{
    sqrt64f(src, dst, len);
}

શૂન્ય invSqrt(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક len)
{
    invSqrt32f(src, dst, len);
}

શૂન્ય invSqrt(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

દશાંશ cv::fastAtan2( દશાંશ y, દશાંશ x )
{
    using નામાકાશ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
