// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#ប្រសិនបើ ARITHM_USE_IPP

namespace cv { namespace hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
do {                                                    \
    ប្រសិនបើ (!CV_IPP_CHECK_COND)                             \
        ត្រឡប់ 0;                                       \
    ប្រសិនបើ (height == 1)                                    \
        step1 = step2 = step = width * parimaa_na(dst[0]);  \
    ប្រសិនបើ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        ត្រឡប់ 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    ត្រឡប់ 0;                                           \
} ខណៈពេល(0)

//=======================================
// Addition
//=======================================

anta_hstha ចំនួនគត់ arithm_ipp_add8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                            uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}

anta_hstha ចំនួនគត់ arithm_ipp_add16u(const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                             ushort* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}

anta_hstha ចំនួនគត់ arithm_ipp_add16s(const short* src1, size_t step1, const short* src2, size_t step2,
                             short* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}

anta_hstha ចំនួនគត់ arithm_ipp_add32f(const float* src1, size_t step1, const float* src2, size_t step2,
                             float* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

anta_hstha ចំនួនគត់ arithm_ipp_sub8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                            uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (ចំនួនគត់)step2, src1, (ចំនួនគត់)step1, dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}

anta_hstha ចំនួនគត់ arithm_ipp_sub16u(const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                             ushort* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (ចំនួនគត់)step2, src1, (ចំនួនគត់)step1, dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}

anta_hstha ចំនួនគត់ arithm_ipp_sub16s(const short* src1, size_t step1, const short* src2, size_t step2,
                            short* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (ចំនួនគត់)step2, src1, (ចំនួនគត់)step1, dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}

anta_hstha ចំនួនគត់ arithm_ipp_sub32f(const float* src1, size_t step1, const float* src2, size_t step2,
                            float* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (ចំនួនគត់)step2, src1, (ចំនួនគត់)step1, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
do {                                                             \
    ប្រសិនបើ (!CV_IPP_CHECK_COND)                                      \
        ត្រឡប់ 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    ប្រសិនបើ (height == 1)                                             \
        step1 = step2 = step = width * parimaa_na(dst[0]);           \
    ចំនួនគត់ i = 0;                                                   \
    សម្រាប់(; i < height; i++)                                       \
    {                                                            \
        ប្រសិនបើ (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            bha_nga;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    ប្រសិនបើ (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        ត្រឡប់ 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    ត្រឡប់ 0;                                                    \
} ខណៈពេល(0)

//=======================================
// Max
//=======================================

anta_hstha ចំនួនគត់ arithm_ipp_max8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                           uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

anta_hstha ចំនួនគត់ arithm_ipp_max16u(const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                             ushort* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

anta_hstha ចំនួនគត់ arithm_ipp_max32f(const float* src1, size_t step1, const float* src2, size_t step2,
                             float* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, float);
}

anta_hstha ចំនួនគត់ arithm_ipp_max64f(const dviguu_na* src1, size_t step1, const dviguu_na* src2, size_t step2,
                             dviguu_na* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, dviguu_na);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

anta_hstha ចំនួនគត់ arithm_ipp_min8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                            uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

anta_hstha ចំនួនគត់ arithm_ipp_min16u(const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                            ushort* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

anta_hstha ចំនួនគត់ arithm_ipp_min32f(const float* src1, size_t step1, const float* src2,size_t step2,
                             float* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, float);
}

anta_hstha ចំនួនគត់ arithm_ipp_min64f(const dviguu_na* src1, size_t step1, const dviguu_na* src2, size_t step2,
                             dviguu_na* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, dviguu_na);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

anta_hstha ចំនួនគត់ arithm_ipp_absdiff8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                                uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

anta_hstha ចំនួនគត់ arithm_ipp_absdiff16u(const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                                ushort* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

anta_hstha ចំនួនគត់ arithm_ipp_absdiff32f(const float* src1, size_t step1, const float* src2, size_t step2,
                                float* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

anta_hstha ចំនួនគត់ arithm_ipp_and8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                            uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

anta_hstha ចំនួនគត់ arithm_ipp_or8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                           uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

anta_hstha ចំនួនគត់ arithm_ipp_xor8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                            uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

anta_hstha ចំនួនគត់ arithm_ipp_not8u(const uchar* src1, size_t step1, uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height)
{
    ប្រសិនបើ (!CV_IPP_CHECK_COND)
        ត្រឡប់ 0;
    ប្រសិនបើ (height == 1)
        step1 = step = width * parimaa_na(dst[0]);
    ប្រសិនបើ (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (ចំនួនគត់)step1, dst, (ចំនួនគត់)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        ត្រឡប់ 1;
    }
    setIppErrorStatus();
    ត្រឡប់ 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
do {                                                      \
    ប្រសិនបើ (!CV_IPP_CHECK_COND)                               \
        ត្រឡប់ 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    ប្រសិនបើ (op < 0)                                           \
        ត្រឡប់ 0;                                         \
    ប្រសិនបើ (height == 1)                                      \
        step1 = step2 = step = width * parimaa_na(dst[0]);    \
    ប្រសិនបើ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        ត្រឡប់ 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    ត្រឡប់ 0;                                             \
} ខណៈពេល(0)

anta_hstha IppCmpOp arithm_ipp_convert_cmp(ចំនួនគត់ cmpop)
{
    switch(cmpop)
    {
        pak_sa CMP_EQ: ត្រឡប់ ippCmpEq;
        pak_sa CMP_GT: ត្រឡប់ ippCmpGreater;
        pak_sa CMP_GE: ត្រឡប់ ippCmpGreaterEq;
        pak_sa CMP_LT: ត្រឡប់ ippCmpLess;
        pak_sa CMP_LE: ត្រឡប់ ippCmpLessEq;
        default:     ត្រឡប់ (IppCmpOp)-1;
    }
}

anta_hstha ចំនួនគត់ arithm_ipp_cmp8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                            uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height, ចំនួនគត់ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

anta_hstha ចំនួនគត់ arithm_ipp_cmp16u(const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                            uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height, ចំនួនគត់ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

anta_hstha ចំនួនគត់ arithm_ipp_cmp16s(const short* src1, size_t step1, const short* src2, size_t step2,
                             uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height, ចំនួនគត់ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

anta_hstha ចំនួនគត់ arithm_ipp_cmp32f(const float* src1, size_t step1, const float* src2, size_t step2,
                             uchar* dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height, ចំនួនគត់ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2, dst, (ចំនួនគត់)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
do {                                                  \
    ប្រសិនបើ (!CV_IPP_CHECK_COND)                           \
        ត្រឡប់ 0;                                     \
    float fscale = (float)scale;                      \
    ប្រសិនបើ (std::fabs(fscale - 1) > FLT_EPSILON)          \
        ត្រឡប់ 0;                                     \
    ប្រសិនបើ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        ត្រឡប់ 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    ត្រឡប់ 0;                                         \
} ខណៈពេល(0)

anta_hstha ចំនួនគត់ arithm_ipp_mul8u(const uchar *src1, size_t step1, const uchar *src2, size_t step2,
                            uchar *dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height, dviguu_na scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2,dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}
anta_hstha ចំនួនគត់ arithm_ipp_mul16u(const ushort *src1, size_t step1, const ushort *src2, size_t step2,
                            ushort *dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height, dviguu_na scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2,dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}

anta_hstha ចំនួនគត់ arithm_ipp_mul16s(const short *src1, size_t step1, const short *src2, size_t step2,
                            short *dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height, dviguu_na scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2,dst, (ចំនួនគត់)step, ippiSize(width, height), 0);
}

anta_hstha ចំនួនគត់ arithm_ipp_mul32f(const float *src1, size_t step1, const float *src2, size_t step2,
                            float *dst, size_t step, ចំនួនគត់ width, ចំនួនគត់ height, dviguu_na scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (ចំនួនគត់)step1, src2, (ចំនួនគត់)step2,dst, (ចំនួនគត់)step, ippiSize(width, height));
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
    ប្រសិនបើ (__CV_EXPAND(fun(__VA_ARGS__)))  \
        ត្រឡប់;                         \
}

#endif // ARITHM_USE_IPP


#ប្រសិនបើ !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
