// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"
#شامل "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#شامل "mathfuncs_core.simd.hpp"
#شامل "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv { نامخلا hal {

خالی cart_مقصد_Polar32f(ثابت اعشاری* x, ثابت اعشاری* y, اعشاری* mag, اعشاری* angle, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cart_مقصد_Polar32f, cv_hal_cart_مقصد_Polar32f, x, y, mag, angle, len, angle_اندر_Degrees);

    CV_CPU_DISPATCH(cart_مقصد_Polar32f, (x, y, mag, angle, len, angle_اندر_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی cart_مقصد_Polar64f(ثابت دوہرا* x, ثابت دوہرا* y, دوہرا* mag, دوہرا* angle, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cart_مقصد_Polar64f, cv_hal_cart_مقصد_Polar64f, x, y, mag, angle, len, angle_اندر_Degrees);

    CV_CPU_DISPATCH(cart_مقصد_Polar64f, (x, y, mag, angle, len, angle_اندر_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی polar_مقصد_Cart32f(ثابت اعشاری* mag, ثابت اعشاری* angle, اعشاری* x, اعشاری* y, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polar_مقصد_Cart32f, cv_hal_polar_مقصد_Cart32f, mag, angle, x, y, len, angle_اندر_Degrees);

    CV_CPU_DISPATCH(polar_مقصد_Cart32f, (mag, angle, x, y, len, angle_اندر_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی polar_مقصد_Cart64f(ثابت دوہرا* mag, ثابت دوہرا* angle, دوہرا* x, دوہرا* y, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polar_مقصد_Cart64f, cv_hal_polar_مقصد_Cart64f, mag, angle, x, y, len, angle_اندر_Degrees);

    CV_CPU_DISPATCH(polar_مقصد_Cart64f, (mag, angle, x, y, len, angle_اندر_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی fastAtan32f(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angle_اندر_Degrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angle_اندر_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی fastAtan64f(ثابت دوہرا *Y, ثابت دوہرا *X, دوہرا *angle, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angle_اندر_Degrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angle_اندر_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
خالی fastAtan2(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angle_اندر_Degrees);
}

خالی magnitude32f(ثابت اعشاری* x, ثابت اعشاری* y, اعشاری* mag, صحیح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_تسلسل(IPP_نسخہ_X100 > 201800 || cv::ipp::get_Ipp_چوٹی_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی magnitude64f(ثابت دوہرا* x, ثابت دوہرا* y, دوہرا* mag, صحیح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_تسلسل(IPP_نسخہ_X100 > 201800 || cv::ipp::get_Ipp_چوٹی_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


خالی invSqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


خالی invSqrt64f(ثابت دوہرا* src, دوہرا* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


خالی sqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


خالی sqrt64f(ثابت دوہرا* src, دوہرا* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی exp32f(ثابت اعشاری *src, اعشاری *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::math_وسعت_Engine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

خالی exp64f(ثابت دوہرا *src, دوہرا *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::math_وسعت_Engine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

خالی log32f(ثابت اعشاری *src, اعشاری *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::math_وسعت_Engine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

خالی log64f(ثابت دوہرا *src, دوہرا *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::math_وسعت_Engine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

خالی قوت_نما(ثابت اعشاری* src, اعشاری* dst, صحیح n)
{
    exp32f(src, dst, n);
}

خالی قوت_نما(ثابت دوہرا* src, دوہرا* dst, صحیح n)
{
    exp64f(src, dst, n);
}

خالی لوگارتھم(ثابت اعشاری* src, اعشاری* dst, صحیح n)
{
    log32f(src, dst, n);
}

خالی لوگارتھم(ثابت دوہرا* src, دوہرا* dst, صحیح n)
{
    log64f(src, dst, n);
}

خالی magnitude(ثابت اعشاری* x, ثابت اعشاری* y, اعشاری* dst, صحیح n)
{
    magnitude32f(x, y, dst, n);
}

خالی magnitude(ثابت دوہرا* x, ثابت دوہرا* y, دوہرا* dst, صحیح n)
{
    magnitude64f(x, y, dst, n);
}

خالی جذر(ثابت اعشاری* src, اعشاری* dst, صحیح len)
{
    sqrt32f(src, dst, len);
}

خالی جذر(ثابت دوہرا* src, دوہرا* dst, صحیح len)
{
    sqrt64f(src, dst, len);
}

خالی invSqrt(ثابت اعشاری* src, اعشاری* dst, صحیح len)
{
    invSqrt32f(src, dst, len);
}

خالی invSqrt(ثابت دوہرا* src, دوہرا* dst, صحیح len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

اعشاری cv::fastAtan2( اعشاری y, اعشاری x )
{
    استعمال نامخلا cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
