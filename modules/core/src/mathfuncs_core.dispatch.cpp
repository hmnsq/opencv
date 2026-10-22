// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#ಅನ್ತರ್ಭಾವ "mathfuncs_core.simd.hpp"
#ಅನ್ತರ್ಭಾವ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಶೂನ್ಯ cart_ಗಮ್ಯ_Polar32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* x, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* y, ದಶಮಾಂಶ* mag, ದಶಮಾಂಶ* angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cart_ಗಮ್ಯ_Polar32f, cv_hal_cart_ಗಮ್ಯ_Polar32f, x, y, mag, angle, len, angle_ಒಳ_Degrees);

    CV_CPU_DISPATCH(cart_ಗಮ್ಯ_Polar32f, (x, y, mag, angle, len, angle_ಒಳ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ cart_ಗಮ್ಯ_Polar64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* x, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ದ್ವಿಗುಣ* angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cart_ಗಮ್ಯ_Polar64f, cv_hal_cart_ಗಮ್ಯ_Polar64f, x, y, mag, angle, len, angle_ಒಳ_Degrees);

    CV_CPU_DISPATCH(cart_ಗಮ್ಯ_Polar64f, (x, y, mag, angle, len, angle_ಒಳ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ polar_ಗಮ್ಯ_Cart32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* mag, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* angle, ದಶಮಾಂಶ* x, ದಶಮಾಂಶ* y, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polar_ಗಮ್ಯ_Cart32f, cv_hal_polar_ಗಮ್ಯ_Cart32f, mag, angle, x, y, len, angle_ಒಳ_Degrees);

    CV_CPU_DISPATCH(polar_ಗಮ್ಯ_Cart32f, (mag, angle, x, y, len, angle_ಒಳ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ polar_ಗಮ್ಯ_Cart64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* mag, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* angle, ದ್ವಿಗುಣ* x, ದ್ವಿಗುಣ* y, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polar_ಗಮ್ಯ_Cart64f, cv_hal_polar_ಗಮ್ಯ_Cart64f, mag, angle, x, y, len, angle_ಒಳ_Degrees);

    CV_CPU_DISPATCH(polar_ಗಮ್ಯ_Cart64f, (mag, angle, x, y, len, angle_ಒಳ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ fastAtan32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angle_ಒಳ_Degrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angle_ಒಳ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ fastAtan64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *Y, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angle_ಒಳ_Degrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angle_ಒಳ_Degrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
ಶೂನ್ಯ fastAtan2(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angle_ಒಳ_Degrees);
}

ಶೂನ್ಯ magnitude32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* x, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* y, ದಶಮಾಂಶ* mag, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_ಸತತ(IPP_ಆವೃತ್ತಿ_X100 > 201800 || cv::ipp::get_Ipp_ತುದಿ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ magnitude64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* x, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_ಸತತ(IPP_ಆವೃತ್ತಿ_X100 > 201800 || cv::ipp::get_Ipp_ತುದಿ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ಶೂನ್ಯ invSqrt32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ಶೂನ್ಯ invSqrt64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ಶೂನ್ಯ sqrt32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ಶೂನ್ಯ sqrt64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ exp32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src, ದಶಮಾಂಶ *dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::math_ಹರವು_Engine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ಶೂನ್ಯ exp64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::math_ಹರವು_Engine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ಶೂನ್ಯ log32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src, ದಶಮಾಂಶ *dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::math_ಹರವು_Engine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ಶೂನ್ಯ log64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::math_ಹರವು_Engine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

ಶೂನ್ಯ ಘಾತಾಂಕ(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ n)
{
    exp32f(src, dst, n);
}

ಶೂನ್ಯ ಘಾತಾಂಕ(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ n)
{
    exp64f(src, dst, n);
}

ಶೂನ್ಯ ಲಘುಗಣಕ(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ n)
{
    log32f(src, dst, n);
}

ಶೂನ್ಯ ಲಘುಗಣಕ(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ n)
{
    log64f(src, dst, n);
}

ಶೂನ್ಯ magnitude(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* x, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* y, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ n)
{
    magnitude32f(x, y, dst, n);
}

ಶೂನ್ಯ magnitude(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* x, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ n)
{
    magnitude64f(x, y, dst, n);
}

ಶೂನ್ಯ ವರ್ಗಮೂಲ(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len)
{
    sqrt32f(src, dst, len);
}

ಶೂನ್ಯ ವರ್ಗಮೂಲ(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len)
{
    sqrt64f(src, dst, len);
}

ಶೂನ್ಯ invSqrt(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len)
{
    invSqrt32f(src, dst, len);
}

ಶೂನ್ಯ invSqrt(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

ದಶಮಾಂಶ cv::fastAtan2( ದಶಮಾಂಶ y, ದಶಮಾಂಶ x )
{
    ಬಳಸಿ ನಾಮಾಕಾಶ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
