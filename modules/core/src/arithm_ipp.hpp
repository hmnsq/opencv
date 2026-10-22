// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#যদি ARITHM_USE_IPP

নামাকাশ cv { নামাকাশ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
করো {                                                    \
    যদি (!CV_IPP_CHECK_COND)                             \
        ফেরত 0;                                       \
    যদি (height == 1)                                    \
        step1 = step2 = step = width * আকার(dst[0]);  \
    যদি (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        ফেরত 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    ফেরত 0;                                           \
} যখন(0)

//=======================================
// Addition
//=======================================

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_add8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                            uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_add16u(ধ্রুবক ushort* src1, size_t step1, ধ্রুবক ushort* src2, size_t step2,
                             ushort* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_add16s(ধ্রুবক short* src1, size_t step1, ধ্রুবক short* src2, size_t step2,
                             short* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_add32f(ধ্রুবক দশমিক* src1, size_t step1, ধ্রুবক দশমিক* src2, size_t step2,
                             দশমিক* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_sub8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                            uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (পূর্ণসংখ্যা)step2, src1, (পূর্ণসংখ্যা)step1, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_sub16u(ধ্রুবক ushort* src1, size_t step1, ধ্রুবক ushort* src2, size_t step2,
                             ushort* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (পূর্ণসংখ্যা)step2, src1, (পূর্ণসংখ্যা)step1, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_sub16s(ধ্রুবক short* src1, size_t step1, ধ্রুবক short* src2, size_t step2,
                            short* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (পূর্ণসংখ্যা)step2, src1, (পূর্ণসংখ্যা)step1, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_sub32f(ধ্রুবক দশমিক* src1, size_t step1, ধ্রুবক দশমিক* src2, size_t step2,
                            দশমিক* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (পূর্ণসংখ্যা)step2, src1, (পূর্ণসংখ্যা)step1, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
করো {                                                             \
    যদি (!CV_IPP_CHECK_COND)                                      \
        ফেরত 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    যদি (height == 1)                                             \
        step1 = step2 = step = width * আকার(dst[0]);           \
    পূর্ণসংখ্যা i = 0;                                                   \
    জন্য(; i < height; i++)                                       \
    {                                                            \
        যদি (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            বিরতি;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    যদি (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        ফেরত 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    ফেরত 0;                                                    \
} যখন(0)

//=======================================
// Max
//=======================================

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_max8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                           uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_max16u(ধ্রুবক ushort* src1, size_t step1, ধ্রুবক ushort* src2, size_t step2,
                             ushort* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_max32f(ধ্রুবক দশমিক* src1, size_t step1, ধ্রুবক দশমিক* src2, size_t step2,
                             দশমিক* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, দশমিক);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_max64f(ধ্রুবক দ্বিগুণ* src1, size_t step1, ধ্রুবক দ্বিগুণ* src2, size_t step2,
                             দ্বিগুণ* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, দ্বিগুণ);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_min8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                            uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_min16u(ধ্রুবক ushort* src1, size_t step1, ধ্রুবক ushort* src2, size_t step2,
                            ushort* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_min32f(ধ্রুবক দশমিক* src1, size_t step1, ধ্রুবক দশমিক* src2,size_t step2,
                             দশমিক* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, দশমিক);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_min64f(ধ্রুবক দ্বিগুণ* src1, size_t step1, ধ্রুবক দ্বিগুণ* src2, size_t step2,
                             দ্বিগুণ* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, দ্বিগুণ);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_absdiff8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                                uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_absdiff16u(ধ্রুবক ushort* src1, size_t step1, ধ্রুবক ushort* src2, size_t step2,
                                ushort* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_absdiff32f(ধ্রুবক দশমিক* src1, size_t step1, ধ্রুবক দশমিক* src2, size_t step2,
                                দশমিক* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_and8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                            uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_or8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                           uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_xor8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                            uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_not8u(ধ্রুবক uchar* src1, size_t step1, uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height)
{
    যদি (!CV_IPP_CHECK_COND)
        ফেরত 0;
    যদি (height == 1)
        step1 = step = width * আকার(dst[0]);
    যদি (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (পূর্ণসংখ্যা)step1, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        ফেরত 1;
    }
    setIppErrorStatus();
    ফেরত 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
করো {                                                      \
    যদি (!CV_IPP_CHECK_COND)                               \
        ফেরত 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    যদি (op < 0)                                           \
        ফেরত 0;                                         \
    যদি (height == 1)                                      \
        step1 = step2 = step = width * আকার(dst[0]);    \
    যদি (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        ফেরত 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    ফেরত 0;                                             \
} যখন(0)

অন্তঃরেখ IppCmpOp arithm_ipp_convert_cmp(পূর্ণসংখ্যা cmpop)
{
    নির্বাচন(cmpop)
    {
        ক্ষেত্র CMP_EQ: ফেরত ippCmpEq;
        ক্ষেত্র CMP_GT: ফেরত ippCmpGreater;
        ক্ষেত্র CMP_GE: ফেরত ippCmpGreaterEq;
        ক্ষেত্র CMP_LT: ফেরত ippCmpLess;
        ক্ষেত্র CMP_LE: ফেরত ippCmpLessEq;
        পূর্বনির্ধারিত:     ফেরত (IppCmpOp)-1;
    }
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_cmp8u(ধ্রুবক uchar* src1, size_t step1, ধ্রুবক uchar* src2, size_t step2,
                            uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, পূর্ণসংখ্যা cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_cmp16u(ধ্রুবক ushort* src1, size_t step1, ধ্রুবক ushort* src2, size_t step2,
                            uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, পূর্ণসংখ্যা cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_cmp16s(ধ্রুবক short* src1, size_t step1, ধ্রুবক short* src2, size_t step2,
                             uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, পূর্ণসংখ্যা cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_cmp32f(ধ্রুবক দশমিক* src1, size_t step1, ধ্রুবক দশমিক* src2, size_t step2,
                             uchar* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, পূর্ণসংখ্যা cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2, dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
করো {                                                  \
    যদি (!CV_IPP_CHECK_COND)                           \
        ফেরত 0;                                     \
    দশমিক fscale = (দশমিক)scale;                      \
    যদি (std::fabs(fscale - 1) > FLT_EPSILON)          \
        ফেরত 0;                                     \
    যদি (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        ফেরত 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    ফেরত 0;                                         \
} যখন(0)

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_mul8u(ধ্রুবক uchar *src1, size_t step1, ধ্রুবক uchar *src2, size_t step2,
                            uchar *dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, দ্বিগুণ scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2,dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}
অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_mul16u(ধ্রুবক ushort *src1, size_t step1, ধ্রুবক ushort *src2, size_t step2,
                            ushort *dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, দ্বিগুণ scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2,dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_mul16s(ধ্রুবক short *src1, size_t step1, ধ্রুবক short *src2, size_t step2,
                            short *dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, দ্বিগুণ scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2,dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height), 0);
}

অন্তঃরেখ পূর্ণসংখ্যা arithm_ipp_mul32f(ধ্রুবক দশমিক *src1, size_t step1, ধ্রুবক দশমিক *src2, size_t step2,
                            দশমিক *dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, দ্বিগুণ scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (পূর্ণসংখ্যা)step1, src2, (পূর্ণসংখ্যা)step2,dst, (পূর্ণসংখ্যা)step, ippi_আকার(width, height));
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
    যদি (__CV_EXPAND(fun(__VA_ARGS__)))  \
        ফেরত;                         \
}

#endif // ARITHM_USE_IPP


#যদি !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
