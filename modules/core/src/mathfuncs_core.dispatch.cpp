// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#অন্তর্ভাৱ "mathfuncs_core.simd.hpp"
#অন্তর্ভাৱ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv { নামাকাশ hal {

শূন্য cartToPolar32f(ধ্রুৱ ভিন্ন* x, ধ্রুৱ ভিন্ন* y, ভিন্ন* mag, ভিন্ন* angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য cartToPolar64f(ধ্রুৱ দ্ৱিগুণ* x, ধ্রুৱ দ্ৱিগুণ* y, দ্ৱিগুণ* mag, দ্ৱিগুণ* angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য polarToCart32f(ধ্রুৱ ভিন্ন* mag, ধ্রুৱ ভিন্ন* angle, ভিন্ন* x, ভিন্ন* y, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য polarToCart64f(ধ্রুৱ দ্ৱিগুণ* mag, ধ্রুৱ দ্ৱিগুণ* angle, দ্ৱিগুণ* x, দ্ৱিগুণ* y, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য fastAtan32f(ধ্রুৱ ভিন্ন *Y, ধ্রুৱ ভিন্ন *X, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য fastAtan64f(ধ্রুৱ দ্ৱিগুণ *Y, ধ্রুৱ দ্ৱিগুণ *X, দ্ৱিগুণ *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
শূন্য fastAtan2(ধ্রুৱ ভিন্ন *Y, ধ্রুৱ ভিন্ন *X, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

শূন্য magnitude32f(ধ্রুৱ ভিন্ন* x, ধ্রুৱ ভিন্ন* y, ভিন্ন* mag, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_শীৰ্ষ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য magnitude64f(ধ্রুৱ দ্ৱিগুণ* x, ধ্রুৱ দ্ৱিগুণ* y, দ্ৱিগুণ* mag, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_শীৰ্ষ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


শূন্য invSqrt32f(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


শূন্য invSqrt64f(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


শূন্য sqrt32f(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


শূন্য sqrt64f(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য exp32f(ধ্রুৱ ভিন্ন *src, ভিন্ন *dst, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

শূন্য exp64f(ধ্রুৱ দ্ৱিগুণ *src, দ্ৱিগুণ *dst, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

শূন্য log32f(ধ্রুৱ ভিন্ন *src, ভিন্ন *dst, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

শূন্য log64f(ধ্রুৱ দ্ৱিগুণ *src, দ্ৱিগুণ *dst, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

শূন্য exp(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক n)
{
    exp32f(src, dst, n);
}

শূন্য exp(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক n)
{
    exp64f(src, dst, n);
}

শূন্য log(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক n)
{
    log32f(src, dst, n);
}

শূন্য log(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক n)
{
    log64f(src, dst, n);
}

শূন্য magnitude(ধ্রুৱ ভিন্ন* x, ধ্রুৱ ভিন্ন* y, ভিন্ন* dst, পূর্ণাঙ্ক n)
{
    magnitude32f(x, y, dst, n);
}

শূন্য magnitude(ধ্রুৱ দ্ৱিগুণ* x, ধ্রুৱ দ্ৱিগুণ* y, দ্ৱিগুণ* dst, পূর্ণাঙ্ক n)
{
    magnitude64f(x, y, dst, n);
}

শূন্য sqrt(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক len)
{
    sqrt32f(src, dst, len);
}

শূন্য sqrt(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক len)
{
    sqrt64f(src, dst, len);
}

শূন্য invSqrt(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক len)
{
    invSqrt32f(src, dst, len);
}

শূন্য invSqrt(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

ভিন্ন cv::fastAtan2( ভিন্ন y, ভিন্ন x )
{
    using নামাকাশ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
