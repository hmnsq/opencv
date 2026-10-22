// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#اگر ARITHM_USE_IPP

فضاینام cv { فضاینام hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
انجامبده {                                                    \
    اگر (!CV_IPP_CHECK_COND)                             \
        بازگشت 0;                                       \
    اگر (height == 1)                                    \
        step1 = step2 = step = width * اندازه(dst[0]);  \
    اگر (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        بازگشت 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    بازگشت 0;                                           \
} وقتی(0)

//=======================================
// Addition
//=======================================

درونخطی صحیح arithm_ipp_add8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height), 0);
}

درونخطی صحیح arithm_ipp_add16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height), 0);
}

درونخطی صحیح arithm_ipp_add16s(ثابت short* src1, size_t step1, ثابت short* src2, size_t step2,
                             short* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height), 0);
}

درونخطی صحیح arithm_ipp_add32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                             اعشاری* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

درونخطی صحیح arithm_ipp_sub8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (صحیح)step2, src1, (صحیح)step1, dst, (صحیح)step, ippiSize(width, height), 0);
}

درونخطی صحیح arithm_ipp_sub16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (صحیح)step2, src1, (صحیح)step1, dst, (صحیح)step, ippiSize(width, height), 0);
}

درونخطی صحیح arithm_ipp_sub16s(ثابت short* src1, size_t step1, ثابت short* src2, size_t step2,
                            short* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (صحیح)step2, src1, (صحیح)step1, dst, (صحیح)step, ippiSize(width, height), 0);
}

درونخطی صحیح arithm_ipp_sub32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                            اعشاری* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (صحیح)step2, src1, (صحیح)step1, dst, (صحیح)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
انجامبده {                                                             \
    اگر (!CV_IPP_CHECK_COND)                                      \
        بازگشت 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    اگر (height == 1)                                             \
        step1 = step2 = step = width * اندازه(dst[0]);           \
    صحیح i = 0;                                                   \
    برای(; i < height; i++)                                       \
    {                                                            \
        اگر (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            بشکن;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    اگر (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        بازگشت 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    بازگشت 0;                                                    \
} وقتی(0)

//=======================================
// Max
//=======================================

درونخطی صحیح arithm_ipp_max8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                           uchar* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

درونخطی صحیح arithm_ipp_max16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

درونخطی صحیح arithm_ipp_max32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                             اعشاری* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, اعشاری);
}

درونخطی صحیح arithm_ipp_max64f(ثابت دوگانه* src1, size_t step1, ثابت دوگانه* src2, size_t step2,
                             دوگانه* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, دوگانه);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

درونخطی صحیح arithm_ipp_min8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

درونخطی صحیح arithm_ipp_min16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                            ushort* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

درونخطی صحیح arithm_ipp_min32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2,size_t step2,
                             اعشاری* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, اعشاری);
}

درونخطی صحیح arithm_ipp_min64f(ثابت دوگانه* src1, size_t step1, ثابت دوگانه* src2, size_t step2,
                             دوگانه* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, دوگانه);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

درونخطی صحیح arithm_ipp_absdiff8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                                uchar* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

درونخطی صحیح arithm_ipp_absdiff16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                                ushort* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

درونخطی صحیح arithm_ipp_absdiff32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                                اعشاری* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

درونخطی صحیح arithm_ipp_and8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

درونخطی صحیح arithm_ipp_or8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                           uchar* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

درونخطی صحیح arithm_ipp_xor8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t step, صحیح width, صحیح height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

درونخطی صحیح arithm_ipp_not8u(ثابت uchar* src1, size_t step1, uchar* dst, size_t step, صحیح width, صحیح height)
{
    اگر (!CV_IPP_CHECK_COND)
        بازگشت 0;
    اگر (height == 1)
        step1 = step = width * اندازه(dst[0]);
    اگر (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (صحیح)step1, dst, (صحیح)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        بازگشت 1;
    }
    setIppErrorStatus();
    بازگشت 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
انجامبده {                                                      \
    اگر (!CV_IPP_CHECK_COND)                               \
        بازگشت 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    اگر (op < 0)                                           \
        بازگشت 0;                                         \
    اگر (height == 1)                                      \
        step1 = step2 = step = width * اندازه(dst[0]);    \
    اگر (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        بازگشت 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    بازگشت 0;                                             \
} وقتی(0)

درونخطی IppCmpOp arithm_ipp_convert_cmp(صحیح cmpop)
{
    انتخاب(cmpop)
    {
        حالت CMP_EQ: بازگشت ippCmpEq;
        حالت CMP_GT: بازگشت ippCmpGreater;
        حالت CMP_GE: بازگشت ippCmpGreaterEq;
        حالت CMP_LT: بازگشت ippCmpLess;
        حالت CMP_LE: بازگشت ippCmpLessEq;
        پیشفرض:     بازگشت (IppCmpOp)-1;
    }
}

درونخطی صحیح arithm_ipp_cmp8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t step, صحیح width, صحیح height, صحیح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

درونخطی صحیح arithm_ipp_cmp16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                            uchar* dst, size_t step, صحیح width, صحیح height, صحیح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

درونخطی صحیح arithm_ipp_cmp16s(ثابت short* src1, size_t step1, ثابت short* src2, size_t step2,
                             uchar* dst, size_t step, صحیح width, صحیح height, صحیح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

درونخطی صحیح arithm_ipp_cmp32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                             uchar* dst, size_t step, صحیح width, صحیح height, صحیح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
انجامبده {                                                  \
    اگر (!CV_IPP_CHECK_COND)                           \
        بازگشت 0;                                     \
    اعشاری fscale = (اعشاری)scale;                      \
    اگر (std::fabs(fscale - 1) > FLT_EPSILON)          \
        بازگشت 0;                                     \
    اگر (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        بازگشت 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    بازگشت 0;                                         \
} وقتی(0)

درونخطی صحیح arithm_ipp_mul8u(ثابت uchar *src1, size_t step1, ثابت uchar *src2, size_t step2,
                            uchar *dst, size_t step, صحیح width, صحیح height, دوگانه scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2,dst, (صحیح)step, ippiSize(width, height), 0);
}
درونخطی صحیح arithm_ipp_mul16u(ثابت ushort *src1, size_t step1, ثابت ushort *src2, size_t step2,
                            ushort *dst, size_t step, صحیح width, صحیح height, دوگانه scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2,dst, (صحیح)step, ippiSize(width, height), 0);
}

درونخطی صحیح arithm_ipp_mul16s(ثابت short *src1, size_t step1, ثابت short *src2, size_t step2,
                            short *dst, size_t step, صحیح width, صحیح height, دوگانه scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2,dst, (صحیح)step, ippiSize(width, height), 0);
}

درونخطی صحیح arithm_ipp_mul32f(ثابت اعشاری *src1, size_t step1, ثابت اعشاری *src2, size_t step2,
                            اعشاری *dst, size_t step, صحیح width, صحیح height, دوگانه scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (صحیح)step1, src2, (صحیح)step2,dst, (صحیح)step, ippiSize(width, height));
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
    اگر (__CV_EXPAND(fun(__VA_ARGS__)))  \
        بازگشت;                         \
}

#endif // ARITHM_USE_IPP


#اگر !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
