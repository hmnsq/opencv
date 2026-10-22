// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#అన్తర్భావ "mathfuncs_core.simd.hpp"
#అన్తర్భావ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv { నామాకాశ hal {

శూన్య cart_గమ్యం_Polar32f(స్థిరరాశి దశాంశం* x, స్థిరరాశి దశాంశం* y, దశాంశం* mag, దశాంశం* angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cart_గమ్యం_Polar32f, cv_hal_cart_గమ్యం_Polar32f, x, y, mag, angle, len, angle_లోపల_Degrees);

    CV_CPU_DISPATCH(cart_గమ్యం_Polar32f, (x, y, mag, angle, len, angle_లోపల_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య cart_గమ్యం_Polar64f(స్థిరరాశి ద్విగుణ* x, స్థిరరాశి ద్విగుణ* y, ద్విగుణ* mag, ద్విగుణ* angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cart_గమ్యం_Polar64f, cv_hal_cart_గమ్యం_Polar64f, x, y, mag, angle, len, angle_లోపల_Degrees);

    CV_CPU_DISPATCH(cart_గమ్యం_Polar64f, (x, y, mag, angle, len, angle_లోపల_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య polar_గమ్యం_Cart32f(స్థిరరాశి దశాంశం* mag, స్థిరరాశి దశాంశం* angle, దశాంశం* x, దశాంశం* y, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polar_గమ్యం_Cart32f, cv_hal_polar_గమ్యం_Cart32f, mag, angle, x, y, len, angle_లోపల_Degrees);

    CV_CPU_DISPATCH(polar_గమ్యం_Cart32f, (mag, angle, x, y, len, angle_లోపల_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య polar_గమ్యం_Cart64f(స్థిరరాశి ద్విగుణ* mag, స్థిరరాశి ద్విగుణ* angle, ద్విగుణ* x, ద్విగుణ* y, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polar_గమ్యం_Cart64f, cv_hal_polar_గమ్యం_Cart64f, mag, angle, x, y, len, angle_లోపల_Degrees);

    CV_CPU_DISPATCH(polar_గమ్యం_Cart64f, (mag, angle, x, y, len, angle_లోపల_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య fastAtan32f(స్థిరరాశి దశాంశం *Y, స్థిరరాశి దశాంశం *X, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angle_లోపల_Degrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angle_లోపల_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య fastAtan64f(స్థిరరాశి ద్విగుణ *Y, స్థిరరాశి ద్విగుణ *X, ద్విగుణ *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angle_లోపల_Degrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angle_లోపల_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
శూన్య fastAtan2(స్థిరరాశి దశాంశం *Y, స్థిరరాశి దశాంశం *X, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angle_లోపల_Degrees);
}

శూన్య magnitude32f(స్థిరరాశి దశాంశం* x, స్థిరరాశి దశాంశం* y, దశాంశం* mag, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_నిరంతరం(IPP_సంస్కరణ_X100 > 201800 || cv::ipp::get_Ipp_శిఖరం_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య magnitude64f(స్థిరరాశి ద్విగుణ* x, స్థిరరాశి ద్విగుణ* y, ద్విగుణ* mag, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_నిరంతరం(IPP_సంస్కరణ_X100 > 201800 || cv::ipp::get_Ipp_శిఖరం_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


శూన్య invSqrt32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


శూన్య invSqrt64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


శూన్య sqrt32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


శూన్య sqrt64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య exp32f(స్థిరరాశి దశాంశం *src, దశాంశం *dst, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::math_పరిధి_Engine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

శూన్య exp64f(స్థిరరాశి ద్విగుణ *src, ద్విగుణ *dst, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::math_పరిధి_Engine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

శూన్య log32f(స్థిరరాశి దశాంశం *src, దశాంశం *dst, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::math_పరిధి_Engine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

శూన్య log64f(స్థిరరాశి ద్విగుణ *src, ద్విగుణ *dst, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::math_పరిధి_Engine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

శూన్య ఘాతాంకం(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం n)
{
    exp32f(src, dst, n);
}

శూన్య ఘాతాంకం(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం n)
{
    exp64f(src, dst, n);
}

శూన్య సంవర్గమానం(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం n)
{
    log32f(src, dst, n);
}

శూన్య సంవర్గమానం(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం n)
{
    log64f(src, dst, n);
}

శూన్య magnitude(స్థిరరాశి దశాంశం* x, స్థిరరాశి దశాంశం* y, దశాంశం* dst, పూర్ణాంకం n)
{
    magnitude32f(x, y, dst, n);
}

శూన్య magnitude(స్థిరరాశి ద్విగుణ* x, స్థిరరాశి ద్విగుణ* y, ద్విగుణ* dst, పూర్ణాంకం n)
{
    magnitude64f(x, y, dst, n);
}

శూన్య వర్గమూలం(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len)
{
    sqrt32f(src, dst, len);
}

శూన్య వర్గమూలం(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len)
{
    sqrt64f(src, dst, len);
}

శూన్య invSqrt(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len)
{
    invSqrt32f(src, dst, len);
}

శూన్య invSqrt(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

దశాంశం cv::fastAtan2( దశాంశం y, దశాంశం x )
{
    ఉపయోగించి నామాకాశ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
