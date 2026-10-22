// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#إذا ARITHM_USE_IPP

نطاق cv { نطاق hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#تعریف_کړه ARITHM_IPP_BIN(fun, ...)                        \
افعل {                                                    \
    إذا (!CV_IPP_CHECK_COND)                             \
        أرجع 0;                                       \
    إذا (لوړوالی == 1)                                    \
        step1 = step2 = ګام = پلنوالی * حجم(dst[0]);  \
    إذا (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_جمع_کړه(CV_IMPL_IPP);                       \
        أرجع 1;                                       \
    }                                                   \
    سېټ_Ipp_Error_حالت();                                \
    أرجع 0;                                           \
} طالما(0)

//=======================================
// Addition
//=======================================

مضمن صحيح arithm_ipp_add8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippi_جمع_کړه_8u_C1_RSfs, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}

مضمن صحيح arithm_ipp_add16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippi_جمع_کړه_16u_C1_RSfs, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}

مضمن صحيح arithm_ipp_add16s(ثابت لنډ* src1, size_t step1, ثابت لنډ* src2, size_t step2,
                             لنډ* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippi_جمع_کړه_16s_C1_RSfs, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}

مضمن صحيح arithm_ipp_add32f(ثابت عائم* src1, size_t step1, ثابت عائم* src2, size_t step2,
                             عائم* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippi_جمع_کړه_32f_C1_R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

#تعریف_کړه arithm_ipp_add8s(...)  0
#تعریف_کړه arithm_ipp_add32s(...) 0
#تعریف_کړه arithm_ipp_add64f(...) 0

//=======================================
//کمول
//=======================================

مضمن صحيح arithm_ipp_sub8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (صحيح)step2, src1, (صحيح)step1, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}

مضمن صحيح arithm_ipp_sub16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (صحيح)step2, src1, (صحيح)step1, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}

مضمن صحيح arithm_ipp_sub16s(ثابت لنډ* src1, size_t step1, ثابت لنډ* src2, size_t step2,
                            لنډ* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (صحيح)step2, src1, (صحيح)step1, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}

مضمن صحيح arithm_ipp_sub32f(ثابت عائم* src1, size_t step1, ثابت عائم* src2, size_t step2,
                            عائم* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (صحيح)step2, src1, (صحيح)step1, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

#تعریف_کړه arithm_ipp_sub8s(...)  0
#تعریف_کړه arithm_ipp_sub32s(...) 0
#تعریف_کړه arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#تعریف_کړه ARITHM_IPP_MIN_MAX(fun, type)                            \
افعل {                                                             \
    إذا (!CV_IPP_CHECK_COND)                                      \
        أرجع 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    إذا (لوړوالی == 1)                                             \
        step1 = step2 = ګام = پلنوالی * حجم(dst[0]);           \
    صحيح i = 0;                                                   \
    لكل(; i < لوړوالی; i++)                                       \
    {                                                            \
        إذا (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, پلنوالی))    \
            اكسر;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + ګام);                          \
    }                                                            \
    إذا (i == لوړوالی)                                             \
    {                                                            \
        CV_IMPL_جمع_کړه(CV_IMPL_IPP);                                \
        أرجع 1;                                                \
    }                                                            \
    سېټ_Ipp_Error_حالت();                                         \
    أرجع 0;                                                    \
} طالما(0)

//=======================================
// Max
//=======================================

مضمن صحيح arithm_ipp_max8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                           uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

مضمن صحيح arithm_ipp_max16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

مضمن صحيح arithm_ipp_max32f(ثابت عائم* src1, size_t step1, ثابت عائم* src2, size_t step2,
                             عائم* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, عائم);
}

مضمن صحيح arithm_ipp_max64f(ثابت مزدوج* src1, size_t step1, ثابت مزدوج* src2, size_t step2,
                             مزدوج* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, مزدوج);
}

#تعریف_کړه arithm_ipp_max8s(...)  0
#تعریف_کړه arithm_ipp_max16s(...) 0
#تعریف_کړه arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

مضمن صحيح arithm_ipp_min8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

مضمن صحيح arithm_ipp_min16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                            ushort* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

مضمن صحيح arithm_ipp_min32f(ثابت عائم* src1, size_t step1, ثابت عائم* src2,size_t step2,
                             عائم* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, عائم);
}

مضمن صحيح arithm_ipp_min64f(ثابت مزدوج* src1, size_t step1, ثابت مزدوج* src2, size_t step2,
                             مزدوج* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, مزدوج);
}

#تعریف_کړه arithm_ipp_min8s(...)  0
#تعریف_کړه arithm_ipp_min16s(...) 0
#تعریف_کړه arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

مضمن صحيح arithm_ipp_absdiff8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                                uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

مضمن صحيح arithm_ipp_absdiff16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                                ushort* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

مضمن صحيح arithm_ipp_absdiff32f(ثابت عائم* src1, size_t step1, ثابت عائم* src2, size_t step2,
                                عائم* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}
#تعریف_کړه arithm_ipp_absdiff8s(...)  0
#تعریف_کړه arithm_ipp_absdiff16s(...) 0
#تعریف_کړه arithm_ipp_absdiff32s(...) 0
#تعریف_کړه arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

مضمن صحيح arithm_ipp_and8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

مضمن صحيح arithm_ipp_or8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                           uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

مضمن صحيح arithm_ipp_xor8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

مضمن صحيح arithm_ipp_not8u(ثابت uchar* src1, size_t step1, uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی)
{
    إذا (!CV_IPP_CHECK_COND)
        أرجع 0;
    إذا (لوړوالی == 1)
        step1 = ګام = پلنوالی * حجم(dst[0]);
    إذا (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (صحيح)step1, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی)))
    {
        CV_IMPL_جمع_کړه(CV_IMPL_IPP);
        أرجع 1;
    }
    سېټ_Ipp_Error_حالت();
    أرجع 0;
}

//=======================================
// Compare
//=======================================

#تعریف_کړه ARITHM_IPP_CMP(fun, ...)                          \
افعل {                                                      \
    إذا (!CV_IPP_CHECK_COND)                               \
        أرجع 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    إذا (op < 0)                                           \
        أرجع 0;                                         \
    إذا (لوړوالی == 1)                                      \
        step1 = step2 = ګام = پلنوالی * حجم(dst[0]);    \
    إذا (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_جمع_کړه(CV_IMPL_IPP);                         \
        أرجع 1;                                         \
    }                                                     \
    سېټ_Ipp_Error_حالت();                                  \
    أرجع 0;                                             \
} طالما(0)

مضمن IppCmpOp arithm_ipp_convert_cmp(صحيح cmpop)
{
    بدل(cmpop)
    {
        حالة CMP_EQ: أرجع ippCmpEq;
        حالة CMP_GT: أرجع ippCmpGreater;
        حالة CMP_GE: أرجع ippCmpGreaterEq;
        حالة CMP_LT: أرجع ippCmpLess;
        حالة CMP_LE: أرجع ippCmpLessEq;
        افتراضي:     أرجع (IppCmpOp)-1;
    }
}

مضمن صحيح arithm_ipp_cmp8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

مضمن صحيح arithm_ipp_cmp16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                            uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

مضمن صحيح arithm_ipp_cmp16s(ثابت لنډ* src1, size_t step1, ثابت لنډ* src2, size_t step2,
                             uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

مضمن صحيح arithm_ipp_cmp32f(ثابت عائم* src1, size_t step1, ثابت عائم* src2, size_t step2,
                             uchar* dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (صحيح)step1, src2, (صحيح)step2, dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

#تعریف_کړه arithm_ipp_cmp8s(...)  0
#تعریف_کړه arithm_ipp_cmp32s(...) 0
#تعریف_کړه arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#تعریف_کړه ARITHM_IPP_MUL(fun, ...)                      \
افعل {                                                  \
    إذا (!CV_IPP_CHECK_COND)                           \
        أرجع 0;                                     \
    عائم fscale = (عائم)scale;                      \
    إذا (std::fabs(fscale - 1) > FLT_EPSILON)          \
        أرجع 0;                                     \
    إذا (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_جمع_کړه(CV_IMPL_IPP);                     \
        أرجع 1;                                     \
    }                                                 \
    سېټ_Ipp_Error_حالت();                              \
    أرجع 0;                                         \
} طالما(0)

مضمن صحيح arithm_ipp_mul8u(ثابت uchar *src1, size_t step1, ثابت uchar *src2, size_t step2,
                            uchar *dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (صحيح)step1, src2, (صحيح)step2,dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}
مضمن صحيح arithm_ipp_mul16u(ثابت ushort *src1, size_t step1, ثابت ushort *src2, size_t step2,
                            ushort *dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (صحيح)step1, src2, (صحيح)step2,dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}

مضمن صحيح arithm_ipp_mul16s(ثابت لنډ *src1, size_t step1, ثابت لنډ *src2, size_t step2,
                            لنډ *dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (صحيح)step1, src2, (صحيح)step2,dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی), 0);
}

مضمن صحيح arithm_ipp_mul32f(ثابت عائم *src1, size_t step1, ثابت عائم *src2, size_t step2,
                            عائم *dst, size_t ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (صحيح)step1, src2, (صحيح)step2,dst, (صحيح)ګام, ippi_اندازه(پلنوالی, لوړوالی));
}

#تعریف_کړه arithm_ipp_mul8s(...)  0
#تعریف_کړه arithm_ipp_mul32s(...) 0
#تعریف_کړه arithm_ipp_mul64f(...) 0

//=======================================
// Div
//=======================================

#تعریف_کړه arithm_ipp_div8u(...)  0
#تعریف_کړه arithm_ipp_div8s(...)  0
#تعریف_کړه arithm_ipp_div16u(...) 0
#تعریف_کړه arithm_ipp_div16s(...) 0
#تعریف_کړه arithm_ipp_div32s(...) 0
#تعریف_کړه arithm_ipp_div32f(...) 0
#تعریف_کړه arithm_ipp_div64f(...) 0

//=======================================
// AddWeighted
//=======================================

#تعریف_کړه arithm_ipp_جمع_کړه_Weighted8u(...)  0
#تعریف_کړه arithm_ipp_جمع_کړه_Weighted8s(...)  0
#تعریف_کړه arithm_ipp_جمع_کړه_Weighted16u(...) 0
#تعریف_کړه arithm_ipp_جمع_کړه_Weighted16s(...) 0
#تعریف_کړه arithm_ipp_جمع_کړه_Weighted32s(...) 0
#تعریف_کړه arithm_ipp_جمع_کړه_Weighted32f(...) 0
#تعریف_کړه arithm_ipp_جمع_کړه_Weighted64f(...) 0

//=======================================
// Reciprocial
//=======================================

#تعریف_کړه arithm_ipp_recip8u(...)  0
#تعریف_کړه arithm_ipp_recip8s(...)  0
#تعریف_کړه arithm_ipp_recip16u(...) 0
#تعریف_کړه arithm_ipp_recip16s(...) 0
#تعریف_کړه arithm_ipp_recip32s(...) 0
#تعریف_کړه arithm_ipp_recip32f(...) 0
#تعریف_کړه arithm_ipp_recip64f(...) 0

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

#تعریف_کړه ARITHM_CALL_IPP(fun, ...)       \
{                                       \
    إذا (__CV_EXPAND(fun(__VA_ARGS__)))  \
        أرجع;                         \
}

#د_که_پای // ARITHM_USE_IPP


#إذا !ARITHM_USE_IPP
#تعریف_کړه ARITHM_CALL_IPP(...)
#د_که_پای
