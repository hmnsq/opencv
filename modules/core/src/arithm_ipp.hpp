// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#ಯದಿ ARITHM_USE_IPP

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
ಕುರು {                                                    \
    ಯದಿ (!CV_IPP_CHECK_COND)                             \
        ಪ್ರತ್ಯಾಗಮ 0;                                       \
    ಯದಿ (height == 1)                                    \
        step1 = step2 = step = width * ಪರಿಮಾಣ(dst[0]);  \
    ಯದಿ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        ಪ್ರತ್ಯಾಗಮ 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    ಪ್ರತ್ಯಾಗಮ 0;                                           \
} ಯಾವತ್(0)

//=======================================
// Addition
//=======================================

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_add8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_add16u(ಧ್ರುವ ushort* src1, size_t step1, ಧ್ರುವ ushort* src2, size_t step2,
                             ushort* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_add16s(ಧ್ರುವ short* src1, size_t step1, ಧ್ರುವ short* src2, size_t step2,
                             short* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_add32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t step1, ಧ್ರುವ ಭಿನ್ನ* src2, size_t step2,
                             ಭಿನ್ನ* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_sub8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_sub16u(ಧ್ರುವ ushort* src1, size_t step1, ಧ್ರುವ ushort* src2, size_t step2,
                             ushort* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_sub16s(ಧ್ರುವ short* src1, size_t step1, ಧ್ರುವ short* src2, size_t step2,
                            short* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_sub32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t step1, ಧ್ರುವ ಭಿನ್ನ* src2, size_t step2,
                            ಭಿನ್ನ* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
ಕುರು {                                                             \
    ಯದಿ (!CV_IPP_CHECK_COND)                                      \
        ಪ್ರತ್ಯಾಗಮ 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    ಯದಿ (height == 1)                                             \
        step1 = step2 = step = width * ಪರಿಮಾಣ(dst[0]);           \
    ಪೂರ್ಣಾಙ್ಕ i = 0;                                                   \
    ಪ್ರತಿ(; i < height; i++)                                       \
    {                                                            \
        ಯದಿ (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            ಭಙ್ಗ;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    ಯದಿ (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        ಪ್ರತ್ಯಾಗಮ 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    ಪ್ರತ್ಯಾಗಮ 0;                                                    \
} ಯಾವತ್(0)

//=======================================
// Max
//=======================================

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_max8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                           uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_max16u(ಧ್ರುವ ushort* src1, size_t step1, ಧ್ರುವ ushort* src2, size_t step2,
                             ushort* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_max32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t step1, ಧ್ರುವ ಭಿನ್ನ* src2, size_t step2,
                             ಭಿನ್ನ* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, ಭಿನ್ನ);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_max64f(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t step1, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t step2,
                             ದ್ವಿಗುಣ* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, ದ್ವಿಗುಣ);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_min8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_min16u(ಧ್ರುವ ushort* src1, size_t step1, ಧ್ರುವ ushort* src2, size_t step2,
                            ushort* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_min32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t step1, ಧ್ರುವ ಭಿನ್ನ* src2,size_t step2,
                             ಭಿನ್ನ* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, ಭಿನ್ನ);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_min64f(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t step1, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t step2,
                             ದ್ವಿಗುಣ* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, ದ್ವಿಗುಣ);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_absdiff8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                                uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_absdiff16u(ಧ್ರುವ ushort* src1, size_t step1, ಧ್ರುವ ushort* src2, size_t step2,
                                ushort* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_absdiff32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t step1, ಧ್ರುವ ಭಿನ್ನ* src2, size_t step2,
                                ಭಿನ್ನ* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_and8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_or8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                           uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_xor8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_not8u(ಧ್ರುವ uchar* src1, size_t step1, uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ಯದಿ (!CV_IPP_CHECK_COND)
        ಪ್ರತ್ಯಾಗಮ 0;
    ಯದಿ (height == 1)
        step1 = step = width * ಪರಿಮಾಣ(dst[0]);
    ಯದಿ (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        ಪ್ರತ್ಯಾಗಮ 1;
    }
    setIppErrorStatus();
    ಪ್ರತ್ಯಾಗಮ 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
ಕುರು {                                                      \
    ಯದಿ (!CV_IPP_CHECK_COND)                               \
        ಪ್ರತ್ಯಾಗಮ 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    ಯದಿ (op < 0)                                           \
        ಪ್ರತ್ಯಾಗಮ 0;                                         \
    ಯದಿ (height == 1)                                      \
        step1 = step2 = step = width * ಪರಿಮಾಣ(dst[0]);    \
    ಯದಿ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        ಪ್ರತ್ಯಾಗಮ 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    ಪ್ರತ್ಯಾಗಮ 0;                                             \
} ಯಾವತ್(0)

ಅನ್ತಃಸ್ಥ IppCmpOp arithm_ipp_convert_cmp(ಪೂರ್ಣಾಙ್ಕ cmpop)
{
    ವಿಕಲ್ಪ(cmpop)
    {
        ಪಕ್ಷ CMP_EQ: ಪ್ರತ್ಯಾಗಮ ippCmpEq;
        ಪಕ್ಷ CMP_GT: ಪ್ರತ್ಯಾಗಮ ippCmpGreater;
        ಪಕ್ಷ CMP_GE: ಪ್ರತ್ಯಾಗಮ ippCmpGreaterEq;
        ಪಕ್ಷ CMP_LT: ಪ್ರತ್ಯಾಗಮ ippCmpLess;
        ಪಕ್ಷ CMP_LE: ಪ್ರತ್ಯಾಗಮ ippCmpLessEq;
        ಸ್ವಭಾವ:     ಪ್ರತ್ಯಾಗಮ (IppCmpOp)-1;
    }
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_cmp8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_cmp16u(ಧ್ರುವ ushort* src1, size_t step1, ಧ್ರುವ ushort* src2, size_t step2,
                            uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_cmp16s(ಧ್ರುವ short* src1, size_t step1, ಧ್ರುವ short* src2, size_t step2,
                             uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_cmp32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t step1, ಧ್ರುವ ಭಿನ್ನ* src2, size_t step2,
                             uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2, dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
ಕುರು {                                                  \
    ಯದಿ (!CV_IPP_CHECK_COND)                           \
        ಪ್ರತ್ಯಾಗಮ 0;                                     \
    ಭಿನ್ನ fscale = (ಭಿನ್ನ)scale;                      \
    ಯದಿ (std::fabs(fscale - 1) > FLT_EPSILON)          \
        ಪ್ರತ್ಯಾಗಮ 0;                                     \
    ಯದಿ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        ಪ್ರತ್ಯಾಗಮ 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    ಪ್ರತ್ಯಾಗಮ 0;                                         \
} ಯಾವತ್(0)

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_mul8u(ಧ್ರುವ uchar *src1, size_t step1, ಧ್ರುವ uchar *src2, size_t step2,
                            uchar *dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ದ್ವಿಗುಣ scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2,dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}
ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_mul16u(ಧ್ರುವ ushort *src1, size_t step1, ಧ್ರುವ ushort *src2, size_t step2,
                            ushort *dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ದ್ವಿಗುಣ scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2,dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_mul16s(ಧ್ರುವ short *src1, size_t step1, ಧ್ರುವ short *src2, size_t step2,
                            short *dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ದ್ವಿಗುಣ scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2,dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height), 0);
}

ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ arithm_ipp_mul32f(ಧ್ರುವ ಭಿನ್ನ *src1, size_t step1, ಧ್ರುವ ಭಿನ್ನ *src2, size_t step2,
                            ಭಿನ್ನ *dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ದ್ವಿಗುಣ scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (ಪೂರ್ಣಾಙ್ಕ)step1, src2, (ಪೂರ್ಣಾಙ್ಕ)step2,dst, (ಪೂರ್ಣಾಙ್ಕ)step, ippiSize(width, height));
}

#define arithm_ipp_mul8s(...)  0
#define arithm_ipp_mul32s(...) 0
#define arithm_ipp_mul64f(...) 0

//=======================================
// Div
//=======================================

#define arithm_ipp_div8u(...)  0
#define arithm_ipp_div8s(...)  0
#define arithm_ipp_div16u(...) 0
#define arithm_ipp_div16s(...) 0
#define arithm_ipp_div32s(...) 0
#define arithm_ipp_div32f(...) 0
#define arithm_ipp_div64f(...) 0

//=======================================
// AddWeighted
//=======================================

#define arithm_ipp_addWeighted8u(...)  0
#define arithm_ipp_addWeighted8s(...)  0
#define arithm_ipp_addWeighted16u(...) 0
#define arithm_ipp_addWeighted16s(...) 0
#define arithm_ipp_addWeighted32s(...) 0
#define arithm_ipp_addWeighted32f(...) 0
#define arithm_ipp_addWeighted64f(...) 0

//=======================================
// Reciprocial
//=======================================

#define arithm_ipp_recip8u(...)  0
#define arithm_ipp_recip8s(...)  0
#define arithm_ipp_recip16u(...) 0
#define arithm_ipp_recip16s(...) 0
#define arithm_ipp_recip32s(...) 0
#define arithm_ipp_recip32f(...) 0
#define arithm_ipp_recip64f(...) 0

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

#define ARITHM_CALL_IPP(fun, ...)       \
{                                       \
    ಯದಿ (__CV_EXPAND(fun(__VA_ARGS__)))  \
        ಪ್ರತ್ಯಾಗಮ;                         \
}

#endif // ARITHM_USE_IPP


#ಯದಿ !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
