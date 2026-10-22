// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#اگر ARITHM_USE_IPP

نامخلا cv { نامخلا hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#تعریف_کرو ARITHM_IPP_BIN(fun, ...)                        \
کرو {                                                    \
    اگر (!CV_IPP_CHECK_COND)                             \
        واپس 0;                                       \
    اگر (اونچائی == 1)                                    \
        step1 = step2 = قدم = چوڑائی * حجم(dst[0]);  \
    اگر (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_جوڑو(CV_IMPL_IPP);                       \
        واپس 1;                                       \
    }                                                   \
    سیٹ_Ipp_Error_حالت();                                \
    واپس 0;                                           \
} جب(0)

//=======================================
// Addition
//=======================================

اندرونی صحیح arithm_ipp_add8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippi_جوڑو_8u_C1_RSfs, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}

اندرونی صحیح arithm_ipp_add16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippi_جوڑو_16u_C1_RSfs, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}

اندرونی صحیح arithm_ipp_add16s(ثابت مختصر* src1, size_t step1, ثابت مختصر* src2, size_t step2,
                             مختصر* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippi_جوڑو_16s_C1_RSfs, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}

اندرونی صحیح arithm_ipp_add32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                             اعشاری* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippi_جوڑو_32f_C1_R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

#تعریف_کرو arithm_ipp_add8s(...)  0
#تعریف_کرو arithm_ipp_add32s(...) 0
#تعریف_کرو arithm_ipp_add64f(...) 0

//=======================================
//تفریق
//=======================================

اندرونی صحیح arithm_ipp_sub8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (صحیح)step2, src1, (صحیح)step1, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}

اندرونی صحیح arithm_ipp_sub16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (صحیح)step2, src1, (صحیح)step1, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}

اندرونی صحیح arithm_ipp_sub16s(ثابت مختصر* src1, size_t step1, ثابت مختصر* src2, size_t step2,
                            مختصر* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (صحیح)step2, src1, (صحیح)step1, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}

اندرونی صحیح arithm_ipp_sub32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                            اعشاری* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (صحیح)step2, src1, (صحیح)step1, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

#تعریف_کرو arithm_ipp_sub8s(...)  0
#تعریف_کرو arithm_ipp_sub32s(...) 0
#تعریف_کرو arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#تعریف_کرو ARITHM_IPP_MIN_MAX(fun, type)                            \
کرو {                                                             \
    اگر (!CV_IPP_CHECK_COND)                                      \
        واپس 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    اگر (اونچائی == 1)                                             \
        step1 = step2 = قدم = چوڑائی * حجم(dst[0]);           \
    صحیح i = 0;                                                   \
    برائے(; i < اونچائی; i++)                                       \
    {                                                            \
        اگر (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, چوڑائی))    \
            توڑو;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + قدم);                          \
    }                                                            \
    اگر (i == اونچائی)                                             \
    {                                                            \
        CV_IMPL_جوڑو(CV_IMPL_IPP);                                \
        واپس 1;                                                \
    }                                                            \
    سیٹ_Ipp_Error_حالت();                                         \
    واپس 0;                                                    \
} جب(0)

//=======================================
// Max
//=======================================

اندرونی صحیح arithm_ipp_max8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                           uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

اندرونی صحیح arithm_ipp_max16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                             ushort* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

اندرونی صحیح arithm_ipp_max32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                             اعشاری* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, اعشاری);
}

اندرونی صحیح arithm_ipp_max64f(ثابت دوہرا* src1, size_t step1, ثابت دوہرا* src2, size_t step2,
                             دوہرا* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, دوہرا);
}

#تعریف_کرو arithm_ipp_max8s(...)  0
#تعریف_کرو arithm_ipp_max16s(...) 0
#تعریف_کرو arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

اندرونی صحیح arithm_ipp_min8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

اندرونی صحیح arithm_ipp_min16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                            ushort* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

اندرونی صحیح arithm_ipp_min32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2,size_t step2,
                             اعشاری* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, اعشاری);
}

اندرونی صحیح arithm_ipp_min64f(ثابت دوہرا* src1, size_t step1, ثابت دوہرا* src2, size_t step2,
                             دوہرا* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, دوہرا);
}

#تعریف_کرو arithm_ipp_min8s(...)  0
#تعریف_کرو arithm_ipp_min16s(...) 0
#تعریف_کرو arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

اندرونی صحیح arithm_ipp_absdiff8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                                uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

اندرونی صحیح arithm_ipp_absdiff16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                                ushort* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

اندرونی صحیح arithm_ipp_absdiff32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                                اعشاری* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}
#تعریف_کرو arithm_ipp_absdiff8s(...)  0
#تعریف_کرو arithm_ipp_absdiff16s(...) 0
#تعریف_کرو arithm_ipp_absdiff32s(...) 0
#تعریف_کرو arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

اندرونی صحیح arithm_ipp_and8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

اندرونی صحیح arithm_ipp_or8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                           uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

اندرونی صحیح arithm_ipp_xor8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

اندرونی صحیح arithm_ipp_not8u(ثابت uchar* src1, size_t step1, uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی)
{
    اگر (!CV_IPP_CHECK_COND)
        واپس 0;
    اگر (اونچائی == 1)
        step1 = قدم = چوڑائی * حجم(dst[0]);
    اگر (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (صحیح)step1, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی)))
    {
        CV_IMPL_جوڑو(CV_IMPL_IPP);
        واپس 1;
    }
    سیٹ_Ipp_Error_حالت();
    واپس 0;
}

//=======================================
// Compare
//=======================================

#تعریف_کرو ARITHM_IPP_CMP(fun, ...)                          \
کرو {                                                      \
    اگر (!CV_IPP_CHECK_COND)                               \
        واپس 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    اگر (op < 0)                                           \
        واپس 0;                                         \
    اگر (اونچائی == 1)                                      \
        step1 = step2 = قدم = چوڑائی * حجم(dst[0]);    \
    اگر (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_جوڑو(CV_IMPL_IPP);                         \
        واپس 1;                                         \
    }                                                     \
    سیٹ_Ipp_Error_حالت();                                  \
    واپس 0;                                             \
} جب(0)

اندرونی IppCmpOp arithm_ipp_convert_cmp(صحیح cmpop)
{
    انتخاب(cmpop)
    {
        حالت CMP_EQ: واپس ippCmpEq;
        حالت CMP_GT: واپس ippCmpGreater;
        حالت CMP_GE: واپس ippCmpGreaterEq;
        حالت CMP_LT: واپس ippCmpLess;
        حالت CMP_LE: واپس ippCmpLessEq;
        طےشدہ:     واپس (IppCmpOp)-1;
    }
}

اندرونی صحیح arithm_ipp_cmp8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                            uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی, صحیح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

اندرونی صحیح arithm_ipp_cmp16u(ثابت ushort* src1, size_t step1, ثابت ushort* src2, size_t step2,
                            uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی, صحیح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

اندرونی صحیح arithm_ipp_cmp16s(ثابت مختصر* src1, size_t step1, ثابت مختصر* src2, size_t step2,
                             uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی, صحیح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

اندرونی صحیح arithm_ipp_cmp32f(ثابت اعشاری* src1, size_t step1, ثابت اعشاری* src2, size_t step2,
                             uchar* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی, صحیح cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (صحیح)step1, src2, (صحیح)step2, dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

#تعریف_کرو arithm_ipp_cmp8s(...)  0
#تعریف_کرو arithm_ipp_cmp32s(...) 0
#تعریف_کرو arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#تعریف_کرو ARITHM_IPP_MUL(fun, ...)                      \
کرو {                                                  \
    اگر (!CV_IPP_CHECK_COND)                           \
        واپس 0;                                     \
    اعشاری fscale = (اعشاری)scale;                      \
    اگر (std::fabs(fscale - 1) > FLT_EPSILON)          \
        واپس 0;                                     \
    اگر (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_جوڑو(CV_IMPL_IPP);                     \
        واپس 1;                                     \
    }                                                 \
    سیٹ_Ipp_Error_حالت();                              \
    واپس 0;                                         \
} جب(0)

اندرونی صحیح arithm_ipp_mul8u(ثابت uchar *src1, size_t step1, ثابت uchar *src2, size_t step2,
                            uchar *dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی, دوہرا scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2,dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}
اندرونی صحیح arithm_ipp_mul16u(ثابت ushort *src1, size_t step1, ثابت ushort *src2, size_t step2,
                            ushort *dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی, دوہرا scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2,dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}

اندرونی صحیح arithm_ipp_mul16s(ثابت مختصر *src1, size_t step1, ثابت مختصر *src2, size_t step2,
                            مختصر *dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی, دوہرا scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (صحیح)step1, src2, (صحیح)step2,dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی), 0);
}

اندرونی صحیح arithm_ipp_mul32f(ثابت اعشاری *src1, size_t step1, ثابت اعشاری *src2, size_t step2,
                            اعشاری *dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی, دوہرا scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (صحیح)step1, src2, (صحیح)step2,dst, (صحیح)قدم, ippi_سائز(چوڑائی, اونچائی));
}

#تعریف_کرو arithm_ipp_mul8s(...)  0
#تعریف_کرو arithm_ipp_mul32s(...) 0
#تعریف_کرو arithm_ipp_mul64f(...) 0

//=======================================
// Div
//=======================================

#تعریف_کرو arithm_ipp_div8u(...)  0
#تعریف_کرو arithm_ipp_div8s(...)  0
#تعریف_کرو arithm_ipp_div16u(...) 0
#تعریف_کرو arithm_ipp_div16s(...) 0
#تعریف_کرو arithm_ipp_div32s(...) 0
#تعریف_کرو arithm_ipp_div32f(...) 0
#تعریف_کرو arithm_ipp_div64f(...) 0

//=======================================
// AddWeighted
//=======================================

#تعریف_کرو arithm_ipp_جوڑو_Weighted8u(...)  0
#تعریف_کرو arithm_ipp_جوڑو_Weighted8s(...)  0
#تعریف_کرو arithm_ipp_جوڑو_Weighted16u(...) 0
#تعریف_کرو arithm_ipp_جوڑو_Weighted16s(...) 0
#تعریف_کرو arithm_ipp_جوڑو_Weighted32s(...) 0
#تعریف_کرو arithm_ipp_جوڑو_Weighted32f(...) 0
#تعریف_کرو arithm_ipp_جوڑو_Weighted64f(...) 0

//=======================================
// Reciprocial
//=======================================

#تعریف_کرو arithm_ipp_recip8u(...)  0
#تعریف_کرو arithm_ipp_recip8s(...)  0
#تعریف_کرو arithm_ipp_recip16u(...) 0
#تعریف_کرو arithm_ipp_recip16s(...) 0
#تعریف_کرو arithm_ipp_recip32s(...) 0
#تعریف_کرو arithm_ipp_recip32f(...) 0
#تعریف_کرو arithm_ipp_recip64f(...) 0

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

#تعریف_کرو ARITHM_CALL_IPP(fun, ...)       \
{                                       \
    اگر (__CV_EXPAND(fun(__VA_ARGS__)))  \
        واپس;                         \
}

#اگر_ختم // ARITHM_USE_IPP


#اگر !ARITHM_USE_IPP
#تعریف_کرو ARITHM_CALL_IPP(...)
#اگر_ختم
