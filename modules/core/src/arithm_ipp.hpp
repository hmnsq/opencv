// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#ಆದರೆ ARITHM_USE_IPP

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#ವ್ಯಾಖ್ಯಾನಿಸು ARITHM_IPP_BIN(fun, ...)                        \
ಮಾಡು {                                                    \
    ಆದರೆ (!CV_IPP_CHECK_COND)                             \
        ಹಿಂತಿರುಗಿ 0;                                       \
    ಆದರೆ (ಎತ್ತರ == 1)                                    \
        step1 = step2 = ಹೆಜ್ಜೆ = ಅಗಲ * ಗಾತ್ರ(dst[0]);  \
    ಆದರೆ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);                       \
        ಹಿಂತಿರುಗಿ 1;                                       \
    }                                                   \
    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();                                \
    ಹಿಂತಿರುಗಿ 0;                                           \
} ಆಗ(0)

//=======================================
// Addition
//=======================================

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_add8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                            uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippi_ಕೂಡಿಸು_8u_C1_RSfs, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_add16u(ಸ್ಥಿರಾಂಕ ushort* src1, size_t step1, ಸ್ಥಿರಾಂಕ ushort* src2, size_t step2,
                             ushort* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippi_ಕೂಡಿಸು_16u_C1_RSfs, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_add16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src2, size_t step2,
                             ಗಿಡ್ಡ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippi_ಕೂಡಿಸು_16s_C1_RSfs, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_add32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t step2,
                             ದಶಮಾಂಶ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippi_ಕೂಡಿಸು_32f_C1_R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_add8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_add32s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_add64f(...) 0

//=======================================
//ಕಳೆ
//=======================================

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_sub8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                            uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (ಪೂರ್ಣಾಂಕ)step2, src1, (ಪೂರ್ಣಾಂಕ)step1, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_sub16u(ಸ್ಥಿರಾಂಕ ushort* src1, size_t step1, ಸ್ಥಿರಾಂಕ ushort* src2, size_t step2,
                             ushort* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (ಪೂರ್ಣಾಂಕ)step2, src1, (ಪೂರ್ಣಾಂಕ)step1, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_sub16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src2, size_t step2,
                            ಗಿಡ್ಡ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (ಪೂರ್ಣಾಂಕ)step2, src1, (ಪೂರ್ಣಾಂಕ)step1, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_sub32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t step2,
                            ದಶಮಾಂಶ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (ಪೂರ್ಣಾಂಕ)step2, src1, (ಪೂರ್ಣಾಂಕ)step1, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_sub8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_sub32s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#ವ್ಯಾಖ್ಯಾನಿಸು ARITHM_IPP_MIN_MAX(fun, type)                            \
ಮಾಡು {                                                             \
    ಆದರೆ (!CV_IPP_CHECK_COND)                                      \
        ಹಿಂತಿರುಗಿ 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    ಆದರೆ (ಎತ್ತರ == 1)                                             \
        step1 = step2 = ಹೆಜ್ಜೆ = ಅಗಲ * ಗಾತ್ರ(dst[0]);           \
    ಪೂರ್ಣಾಂಕ i = 0;                                                   \
    ಗಾಗಿ(; i < ಎತ್ತರ; i++)                                       \
    {                                                            \
        ಆದರೆ (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, ಅಗಲ))    \
            ನಿಲ್ಲಿಸು;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + ಹೆಜ್ಜೆ);                          \
    }                                                            \
    ಆದರೆ (i == ಎತ್ತರ)                                             \
    {                                                            \
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);                                \
        ಹಿಂತಿರುಗಿ 1;                                                \
    }                                                            \
    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();                                         \
    ಹಿಂತಿರುಗಿ 0;                                                    \
} ಆಗ(0)

//=======================================
// Max
//=======================================

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_max8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                           uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_max16u(ಸ್ಥಿರಾಂಕ ushort* src1, size_t step1, ಸ್ಥಿರಾಂಕ ushort* src2, size_t step2,
                             ushort* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_max32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t step2,
                             ದಶಮಾಂಶ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, ದಶಮಾಂಶ);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_max64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t step2,
                             ದ್ವಿಗುಣ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, ದ್ವಿಗುಣ);
}

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_max8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_max16s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_min8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                            uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_min16u(ಸ್ಥಿರಾಂಕ ushort* src1, size_t step1, ಸ್ಥಿರಾಂಕ ushort* src2, size_t step2,
                            ushort* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_min32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2,size_t step2,
                             ದಶಮಾಂಶ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, ದಶಮಾಂಶ);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_min64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t step2,
                             ದ್ವಿಗುಣ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, ದ್ವಿಗುಣ);
}

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_min8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_min16s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_absdiff8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                                uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_absdiff16u(ಸ್ಥಿರಾಂಕ ushort* src1, size_t step1, ಸ್ಥಿರಾಂಕ ushort* src2, size_t step2,
                                ushort* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_absdiff32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t step2,
                                ದಶಮಾಂಶ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_absdiff8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_absdiff16s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_absdiff32s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_and8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                            uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_or8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                           uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_xor8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                            uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_not8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ)
{
    ಆದರೆ (!CV_IPP_CHECK_COND)
        ಹಿಂತಿರುಗಿ 0;
    ಆದರೆ (ಎತ್ತರ == 1)
        step1 = ಹೆಜ್ಜೆ = ಅಗಲ * ಗಾತ್ರ(dst[0]);
    ಆದರೆ (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ)))
    {
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
        ಹಿಂತಿರುಗಿ 1;
    }
    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
    ಹಿಂತಿರುಗಿ 0;
}

//=======================================
// Compare
//=======================================

#ವ್ಯಾಖ್ಯಾನಿಸು ARITHM_IPP_CMP(fun, ...)                          \
ಮಾಡು {                                                      \
    ಆದರೆ (!CV_IPP_CHECK_COND)                               \
        ಹಿಂತಿರುಗಿ 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    ಆದರೆ (op < 0)                                           \
        ಹಿಂತಿರುಗಿ 0;                                         \
    ಆದರೆ (ಎತ್ತರ == 1)                                      \
        step1 = step2 = ಹೆಜ್ಜೆ = ಅಗಲ * ಗಾತ್ರ(dst[0]);    \
    ಆದರೆ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);                         \
        ಹಿಂತಿರುಗಿ 1;                                         \
    }                                                     \
    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();                                  \
    ಹಿಂತಿರುಗಿ 0;                                             \
} ಆಗ(0)

ಸಾಲೊಳಗೆ IppCmpOp arithm_ipp_convert_cmp(ಪೂರ್ಣಾಂಕ cmpop)
{
    ಆಯ್ಕೆ(cmpop)
    {
        ಸ್ಥಿತಿ CMP_EQ: ಹಿಂತಿರುಗಿ ippCmpEq;
        ಸ್ಥಿತಿ CMP_GT: ಹಿಂತಿರುಗಿ ippCmpGreater;
        ಸ್ಥಿತಿ CMP_GE: ಹಿಂತಿರುಗಿ ippCmpGreaterEq;
        ಸ್ಥಿತಿ CMP_LT: ಹಿಂತಿರುಗಿ ippCmpLess;
        ಸ್ಥಿತಿ CMP_LE: ಹಿಂತಿರುಗಿ ippCmpLessEq;
        ಪೂರ್ವನಿಯೋಜಿತ:     ಹಿಂತಿರುಗಿ (IppCmpOp)-1;
    }
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_cmp8u(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                            uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_cmp16u(ಸ್ಥಿರಾಂಕ ushort* src1, size_t step1, ಸ್ಥಿರಾಂಕ ushort* src2, size_t step2,
                            uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_cmp16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src2, size_t step2,
                             uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_cmp32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t step2,
                             uchar* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2, dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_cmp8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_cmp32s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#ವ್ಯಾಖ್ಯಾನಿಸು ARITHM_IPP_MUL(fun, ...)                      \
ಮಾಡು {                                                  \
    ಆದರೆ (!CV_IPP_CHECK_COND)                           \
        ಹಿಂತಿರುಗಿ 0;                                     \
    ದಶಮಾಂಶ fscale = (ದಶಮಾಂಶ)scale;                      \
    ಆದರೆ (std::fabs(fscale - 1) > FLT_EPSILON)          \
        ಹಿಂತಿರುಗಿ 0;                                     \
    ಆದರೆ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);                     \
        ಹಿಂತಿರುಗಿ 1;                                     \
    }                                                 \
    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();                              \
    ಹಿಂತಿರುಗಿ 0;                                         \
} ಆಗ(0)

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_mul8u(ಸ್ಥಿರಾಂಕ uchar *src1, size_t step1, ಸ್ಥಿರಾಂಕ uchar *src2, size_t step2,
                            uchar *dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2,dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_mul16u(ಸ್ಥಿರಾಂಕ ushort *src1, size_t step1, ಸ್ಥಿರಾಂಕ ushort *src2, size_t step2,
                            ushort *dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2,dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_mul16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1, size_t step1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2, size_t step2,
                            ಗಿಡ್ಡ *dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2,dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ), 0);
}

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ arithm_ipp_mul32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1, size_t step1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2, size_t step2,
                            ದಶಮಾಂಶ *dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (ಪೂರ್ಣಾಂಕ)step1, src2, (ಪೂರ್ಣಾಂಕ)step2,dst, (ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ(ಅಗಲ, ಎತ್ತರ));
}

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_mul8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_mul32s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_mul64f(...) 0

//=======================================
// Div
//=======================================

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_div8u(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_div8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_div16u(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_div16s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_div32s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_div32f(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_div64f(...) 0

//=======================================
// AddWeighted
//=======================================

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_ಕೂಡಿಸು_Weighted8u(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_ಕೂಡಿಸು_Weighted8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_ಕೂಡಿಸು_Weighted16u(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_ಕೂಡಿಸು_Weighted16s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_ಕೂಡಿಸು_Weighted32s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_ಕೂಡಿಸು_Weighted32f(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_ಕೂಡಿಸು_Weighted64f(...) 0

//=======================================
// Reciprocial
//=======================================

#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_recip8u(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_recip8s(...)  0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_recip16u(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_recip16s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_recip32s(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_recip32f(...) 0
#ವ್ಯಾಖ್ಯಾನಿಸು arithm_ipp_recip64f(...) 0

/** empty block in case if you have "fun"
#define arithm_ipp_8u(...)  0
#define arithm_ipp_8s(...)  0
#define arithm_ipp_16u(...) 0
#define arithm_ipp_16s(...) 0
#define arithm_ipp_32s(...) 0
#define arithm_ipp_32f(...) 0
#define arithm_ipp_64f(...) 0
**/

}} // cv::hal::

#ವ್ಯಾಖ್ಯಾನಿಸು ARITHM_CALL_IPP(fun, ...)       \
{                                       \
    ಆದರೆ (__CV_EXPAND(fun(__VA_ARGS__)))  \
        ಹಿಂತಿರುಗಿ;                         \
}

#ಆದರೆ_ಅಂತ್ಯ // ARITHM_USE_IPP


#ಆದರೆ !ARITHM_USE_IPP
#ವ್ಯಾಖ್ಯಾನಿಸು ARITHM_CALL_IPP(...)
#ಆದರೆ_ಅಂತ್ಯ
