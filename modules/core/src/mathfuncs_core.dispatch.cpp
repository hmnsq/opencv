// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "arithm_expr.hpp"      // ew::mathSpanEngine - the engine fallback for exp/log

#അന്തര്ഭാവ "mathfuncs_core.simd.hpp"
#അന്തര്ഭാവ "mathfuncs_core.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv { നാമാകാശ hal {

ശൂന്യം cartToPolar32f(സ്ഥിരം ദശാംശം* x, സ്ഥിരം ദശാംശം* y, ദശാംശം* mag, ദശാംശം* angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar32f, cv_hal_cartToPolar32f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar32f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം cartToPolar64f(സ്ഥിരം ഇരട്ട* x, സ്ഥിരം ഇരട്ട* y, ഇരട്ട* mag, ഇരട്ട* angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(cartToPolar64f, cv_hal_cartToPolar64f, x, y, mag, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(cartToPolar64f, (x, y, mag, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം polarToCart32f(സ്ഥിരം ദശാംശം* mag, സ്ഥിരം ദശാംശം* angle, ദശാംശം* x, ദശാംശം* y, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart32f, cv_hal_polarToCart32f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart32f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം polarToCart64f(സ്ഥിരം ഇരട്ട* mag, സ്ഥിരം ഇരട്ട* angle, ഇരട്ട* x, ഇരട്ട* y, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(polarToCart64f, cv_hal_polarToCart64f, mag, angle, x, y, len, angleInDegrees);

    CV_CPU_DISPATCH(polarToCart64f, (mag, angle, x, y, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം fastAtan32f(സ്ഥിരം ദശാംശം *Y, സ്ഥിരം ദശാംശം *X, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan32f, cv_hal_fastAtan32f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan32f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം fastAtan64f(സ്ഥിരം ഇരട്ട *Y, സ്ഥിരം ഇരട്ട *X, ഇരട്ട *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(fastAtan64f, cv_hal_fastAtan64f, Y, X, angle, len, angleInDegrees);

    CV_CPU_DISPATCH(fastAtan64f, (Y, X, angle, len, angleInDegrees),
        CV_CPU_DISPATCH_MODES_ALL);
}

// deprecated
ശൂന്യം fastAtan2(സ്ഥിരം ദശാംശം *Y, സ്ഥിരം ദശാംശം *X, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

ശൂന്യം magnitude32f(സ്ഥിരം ദശാംശം* x, സ്ഥിരം ദശാംശം* y, ദശാംശം* mag, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude32f, cv_hal_magnitude32f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_മുകൾ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_32f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude32f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം magnitude64f(സ്ഥിരം ഇരട്ട* x, സ്ഥിരം ഇരട്ട* y, ഇരട്ട* mag, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(magnitude64f, cv_hal_magnitude64f, x, y, mag, len);
    // SSE42 performance issues
    CV_IPP_RUN(IPP_VERSION_X100 > 201800 || cv::ipp::get_Ipp_മുകൾ_Features() != ippCPUID_SSE42, CV_INSTRUMENT_FUN_IPP(ippsMagnitude_64f, x, y, mag, len) >= 0);

    CV_CPU_DISPATCH(magnitude64f, (x, y, mag, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ശൂന്യം invSqrt32f(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt32f, cv_hal_invSqrt32f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ശൂന്യം invSqrt64f(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(invSqrt64f, cv_hal_invSqrt64f, src, dst, len);

    CV_CPU_DISPATCH(invSqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ശൂന്യം sqrt32f(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt32f, cv_hal_sqrt32f, src, dst, len);

    CV_CPU_DISPATCH(sqrt32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}


ശൂന്യം sqrt64f(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(sqrt64f, cv_hal_sqrt64f, src, dst, len);

    CV_CPU_DISPATCH(sqrt64f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം exp32f(സ്ഥിരം ദശാംശം *src, ദശാംശം *dst, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp32f, cv_hal_exp32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ശൂന്യം exp64f(സ്ഥിരം ഇരട്ട *src, ഇരട്ട *dst, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(exp64f, cv_hal_exp64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_EXP, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ശൂന്യം log32f(സ്ഥിരം ദശാംശം *src, ദശാംശം *dst, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log32f, cv_hal_log32f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_32F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

ശൂന്യം log64f(സ്ഥിരം ഇരട്ട *src, ഇരട്ട *dst, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    CALL_HAL(log64f, cv_hal_log64f, src, dst, n);

    ew::mathSpanEngine(ew::OP_LOG, CV_64F, src, dst, n);   // the engine's vector kernel (the old
                                                       // table implementation is removed)
}

//=============================================================================
// for compatibility with 3.0

ശൂന്യം exp(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ n)
{
    exp32f(src, dst, n);
}

ശൂന്യം exp(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ n)
{
    exp64f(src, dst, n);
}

ശൂന്യം log(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ n)
{
    log32f(src, dst, n);
}

ശൂന്യം log(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ n)
{
    log64f(src, dst, n);
}

ശൂന്യം magnitude(സ്ഥിരം ദശാംശം* x, സ്ഥിരം ദശാംശം* y, ദശാംശം* dst, പൂർണ്ണസംഖ്യ n)
{
    magnitude32f(x, y, dst, n);
}

ശൂന്യം magnitude(സ്ഥിരം ഇരട്ട* x, സ്ഥിരം ഇരട്ട* y, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ n)
{
    magnitude64f(x, y, dst, n);
}

ശൂന്യം sqrt(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len)
{
    sqrt32f(src, dst, len);
}

ശൂന്യം sqrt(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len)
{
    sqrt64f(src, dst, len);
}

ശൂന്യം invSqrt(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len)
{
    invSqrt32f(src, dst, len);
}

ശൂന്യം invSqrt(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len)
{
    invSqrt64f(src, dst, len);
}

}} // namespace cv::hal::

ദശാംശം cv::fastAtan2( ദശാംശം y, ദശാംശം x )
{
    using നാമാകാശ cv::hal;
    CV_CPU_CALL_BASELINE(fastAtan2, (y, x));
}
