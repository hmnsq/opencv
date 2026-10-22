// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#ਅਨ੍ਤਰ੍ਭਾਵ "mathfuncs_core.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

ਖਾਲੀ cart_ਮੰਜ਼ਿਲ_Polar32f(ਅਚਲ ਦਸ਼ਮਲਵ* x, ਅਚਲ ਦਸ਼ਮਲਵ* y, ਦਸ਼ਮਲਵ* mag, ਦਸ਼ਮਲਵ* angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cart_ਮੰਜ਼ਿਲ_Polar32f, cv_hal_cart_ਮੰਜ਼ਿਲ_Polar32f, x, y, mag, angle, len, angle_ਵਿੱਚ_Degrees);

    CV_CPU_DISPATCH(cart_ਮੰਜ਼ਿਲ_Polar32f, (x, y, mag, angle, len, angle_ਵਿੱਚ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ cart_ਮੰਜ਼ਿਲ_Polar64f(ਅਚਲ ਦੁੱਗਣਾ* x, ਅਚਲ ਦੁੱਗਣਾ* y, ਦੁੱਗਣਾ* mag, ਦੁੱਗਣਾ* angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cart_ਮੰਜ਼ਿਲ_Polar64f, cv_hal_cart_ਮੰਜ਼ਿਲ_Polar64f, x, y, mag, angle, len, angle_ਵਿੱਚ_Degrees);

    CV_CPU_DISPATCH(cart_ਮੰਜ਼ਿਲ_Polar64f, (x, y, mag, angle, len, angle_ਵਿੱਚ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ polar_ਮੰਜ਼ਿਲ_Cart32f(ਅਚਲ ਦਸ਼ਮਲਵ* mag, ਅਚਲ ਦਸ਼ਮਲਵ* angle, ਦਸ਼ਮਲਵ* x, ਦਸ਼ਮਲਵ* y, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polar_ਮੰਜ਼ਿਲ_Cart32f, cv_hal_polar_ਮੰਜ਼ਿਲ_Cart32f, mag, angle, x, y, len, angle_ਵਿੱਚ_Degrees);

    CV_CPU_DISPATCH(polar_ਮੰਜ਼ਿਲ_Cart32f, (mag, angle, x, y, len, angle_ਵਿੱਚ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ polar_ਮੰਜ਼ਿਲ_Cart64f(ਅਚਲ ਦੁੱਗਣਾ* mag, ਅਚਲ ਦੁੱਗਣਾ* angle, ਦੁੱਗਣਾ* x, ਦੁੱਗਣਾ* y, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polar_ਮੰਜ਼ਿਲ_Cart64f, cv_hal_polar_ਮੰਜ਼ਿਲ_Cart64f, mag, angle, x, y, len, angle_ਵਿੱਚ_Degrees);

    CV_CPU_DISPATCH(polar_ਮੰਜ਼ਿਲ_Cart64f, (mag, angle, x, y, len, angle_ਵਿੱਚ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ fastAtan32f(ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਅਚਲ ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angle_ਵਿੱਚ_Degrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angle_ਵਿੱਚ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ fastAtan64f(ਅਚਲ ਦੁੱਗਣਾ *Y, ਅਚਲ ਦੁੱਗਣਾ *X, ਦੁੱਗਣਾ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angle_ਵਿੱਚ_Degrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angle_ਵਿੱਚ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
ਖਾਲੀ fastAtan2(ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਅਚਲ ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angle_ਵਿੱਚ_Degrees);
}

ਖਾਲੀ magnitude32f(ਅਚਲ ਦਸ਼ਮਲਵ* x, ਅਚਲ ਦਸ਼ਮਲਵ* y, ਦਸ਼ਮਲਵ* mag, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_ਲੜੀਵਾਰ(IPP_ਸੰਸਕਰਣ_X100 > 201800 || cv::ipp::get_Ipp_ਸਿਖਰ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ magnitude64f(ਅਚਲ ਦੁੱਗਣਾ* x, ਅਚਲ ਦੁੱਗਣਾ* y, ਦੁੱਗਣਾ* mag, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_ਲੜੀਵਾਰ(IPP_ਸੰਸਕਰਣ_X100 > 201800 || cv::ipp::get_Ipp_ਸਿਖਰ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ਖਾਲੀ invSqrt32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ਖਾਲੀ invSqrt64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ਖਾਲੀ sqrt32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ਖਾਲੀ sqrt64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ exp32f(ਅਚਲ ਦਸ਼ਮਲਵ *src, ਦਸ਼ਮਲਵ *dst, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::math_ਵਿਸਤਾਰ_Engine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ਖਾਲੀ exp64f(ਅਚਲ ਦੁੱਗਣਾ *src, ਦੁੱਗਣਾ *dst, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::math_ਵਿਸਤਾਰ_Engine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ਖਾਲੀ log32f(ਅਚਲ ਦਸ਼ਮਲਵ *src, ਦਸ਼ਮਲਵ *dst, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::math_ਵਿਸਤਾਰ_Engine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ਖਾਲੀ log64f(ਅਚਲ ਦੁੱਗਣਾ *src, ਦੁੱਗਣਾ *dst, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::math_ਵਿਸਤਾਰ_Engine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

ਖਾਲੀ ਘਾਤਾਂਕ(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ n)
{
    exp32f(src, dst, n);
}

ਖਾਲੀ ਘਾਤਾਂਕ(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ n)
{
    exp64f(src, dst, n);
}

ਖਾਲੀ ਲਘੂਗਣਕ(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ n)
{
    log32f(src, dst, n);
}

ਖਾਲੀ ਲਘੂਗਣਕ(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ n)
{
    log64f(src, dst, n);
}

ਖਾਲੀ magnitude(ਅਚਲ ਦਸ਼ਮਲਵ* x, ਅਚਲ ਦਸ਼ਮਲਵ* y, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ n)
{
    magnitude32f(x, y, dst, n);
}

ਖਾਲੀ magnitude(ਅਚਲ ਦੁੱਗਣਾ* x, ਅਚਲ ਦੁੱਗਣਾ* y, ਦੁੱਗਣਾ* dst, ਪੂਰਨ n)
{
    magnitude64f(x, y, dst, n);
}

ਖਾਲੀ ਵਰਗਮੂਲ(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len)
{
    sqrt32f(src, dst, len);
}

ਖਾਲੀ ਵਰਗਮੂਲ(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len)
{
    sqrt64f(src, dst, len);
}

ਖਾਲੀ invSqrt(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len)
{
    invSqrt32f(src, dst, len);
}

ਖਾਲੀ invSqrt(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

ਦਸ਼ਮਲਵ cv::fastAtan2( ਦਸ਼ਮਲਵ y, ਦਸ਼ਮਲਵ x )
{
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
