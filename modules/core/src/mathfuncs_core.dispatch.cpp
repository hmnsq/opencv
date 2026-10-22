// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#ضمن "mathfuncs_core.simd.hpp"
#ضمن "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv { نطاق hal {

فراغ cartToPolar32f(ثابت عائم* x, ثابت عائم* y, عائم* mag, عائم* angle, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ cartToPolar64f(ثابت مزدوج* x, ثابت مزدوج* y, مزدوج* mag, مزدوج* angle, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ polarToCart32f(ثابت عائم* mag, ثابت عائم* angle, عائم* x, عائم* y, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ polarToCart64f(ثابت مزدوج* mag, ثابت مزدوج* angle, مزدوج* x, مزدوج* y, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ fastAtan32f(ثابت عائم *Y, ثابت عائم *X, عائم *angle, صحيح len, منطقي angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ fastAtan64f(ثابت مزدوج *Y, ثابت مزدوج *X, مزدوج *angle, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
فراغ fastAtan2(ثابت عائم *Y, ثابت عائم *X, عائم *angle, صحيح len, منطقي angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

فراغ magnitude32f(ثابت عائم* x, ثابت عائم* y, عائم* mag, صحيح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_چوققا_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ magnitude64f(ثابت مزدوج* x, ثابت مزدوج* y, مزدوج* mag, صحيح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_چوققا_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


فراغ invSqrt32f(ثابت عائم* src, عائم* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


فراغ invSqrt64f(ثابت مزدوج* src, مزدوج* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


فراغ sqrt32f(ثابت عائم* src, عائم* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


فراغ sqrt64f(ثابت مزدوج* src, مزدوج* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ exp32f(ثابت عائم *src, عائم *dst, صحيح n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

فراغ exp64f(ثابت مزدوج *src, مزدوج *dst, صحيح n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

فراغ log32f(ثابت عائم *src, عائم *dst, صحيح n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

فراغ log64f(ثابت مزدوج *src, مزدوج *dst, صحيح n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

فراغ exp(ثابت عائم* src, عائم* dst, صحيح n)
{
    exp32f(src, dst, n);
}

فراغ exp(ثابت مزدوج* src, مزدوج* dst, صحيح n)
{
    exp64f(src, dst, n);
}

فراغ log(ثابت عائم* src, عائم* dst, صحيح n)
{
    log32f(src, dst, n);
}

فراغ log(ثابت مزدوج* src, مزدوج* dst, صحيح n)
{
    log64f(src, dst, n);
}

فراغ magnitude(ثابت عائم* x, ثابت عائم* y, عائم* dst, صحيح n)
{
    magnitude32f(x, y, dst, n);
}

فراغ magnitude(ثابت مزدوج* x, ثابت مزدوج* y, مزدوج* dst, صحيح n)
{
    magnitude64f(x, y, dst, n);
}

فراغ sqrt(ثابت عائم* src, عائم* dst, صحيح len)
{
    sqrt32f(src, dst, len);
}

فراغ sqrt(ثابت مزدوج* src, مزدوج* dst, صحيح len)
{
    sqrt64f(src, dst, len);
}

فراغ invSqrt(ثابت عائم* src, عائم* dst, صحيح len)
{
    invSqrt32f(src, dst, len);
}

فراغ invSqrt(ثابت مزدوج* src, مزدوج* dst, صحيح len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

عائم cv::fastAtan2( عائم y, عائم x )
{
    using نطاق cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
