// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#अगर ARITHM_USE_IPP

namespace cv { namespace hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#घोषणा ARITHM_IPP_BIN(fun, ...)                        \
करो {                                                    \
    अगर (!CV_IPP_CHECK_COND)                             \
        वापस 0;                                       \
    अगर (height == 1)                                    \
        step1 = step2 = step = width * माप(dst[0]);  \
    अगर (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        वापस 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    वापस 0;                                           \
} जबतक(0)

//=======================================
// Addition
//=======================================

अंतरभूत पूर्णांक arithm_ipp_add8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}

अंतरभूत पूर्णांक arithm_ipp_add16u(स्थिर ushort* src1, माप_प्रकार step1, स्थिर ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}

अंतरभूत पूर्णांक arithm_ipp_add16s(स्थिर लघु* src1, माप_प्रकार step1, स्थिर लघु* src2, माप_प्रकार step2,
                             लघु* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}

अंतरभूत पूर्णांक arithm_ipp_add32f(स्थिर भग्न* src1, माप_प्रकार step1, स्थिर भग्न* src2, माप_प्रकार step2,
                             भग्न* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

#घोषणा arithm_ipp_add8s(...)  0
#घोषणा arithm_ipp_add32s(...) 0
#घोषणा arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

अंतरभूत पूर्णांक arithm_ipp_sub8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (पूर्णांक)step2, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}

अंतरभूत पूर्णांक arithm_ipp_sub16u(स्थिर ushort* src1, माप_प्रकार step1, स्थिर ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (पूर्णांक)step2, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}

अंतरभूत पूर्णांक arithm_ipp_sub16s(स्थिर लघु* src1, माप_प्रकार step1, स्थिर लघु* src2, माप_प्रकार step2,
                            लघु* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (पूर्णांक)step2, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}

अंतरभूत पूर्णांक arithm_ipp_sub32f(स्थिर भग्न* src1, माप_प्रकार step1, स्थिर भग्न* src2, माप_प्रकार step2,
                            भग्न* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (पूर्णांक)step2, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

#घोषणा arithm_ipp_sub8s(...)  0
#घोषणा arithm_ipp_sub32s(...) 0
#घोषणा arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#घोषणा ARITHM_IPP_MIN_MAX(fun, type)                            \
करो {                                                             \
    अगर (!CV_IPP_CHECK_COND)                                      \
        वापस 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    अगर (height == 1)                                             \
        step1 = step2 = step = width * माप(dst[0]);           \
    पूर्णांक i = 0;                                                   \
    क्रम(; i < height; i++)                                       \
    {                                                            \
        अगर (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            अवरोध;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    अगर (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        वापस 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    वापस 0;                                                    \
} जबतक(0)

//=======================================
// Max
//=======================================

अंतरभूत पूर्णांक arithm_ipp_max8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

अंतरभूत पूर्णांक arithm_ipp_max16u(स्थिर ushort* src1, माप_प्रकार step1, स्थिर ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

अंतरभूत पूर्णांक arithm_ipp_max32f(स्थिर भग्न* src1, माप_प्रकार step1, स्थिर भग्न* src2, माप_प्रकार step2,
                             भग्न* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, भग्न);
}

अंतरभूत पूर्णांक arithm_ipp_max64f(स्थिर द्विगुन* src1, माप_प्रकार step1, स्थिर द्विगुन* src2, माप_प्रकार step2,
                             द्विगुन* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, द्विगुन);
}

#घोषणा arithm_ipp_max8s(...)  0
#घोषणा arithm_ipp_max16s(...) 0
#घोषणा arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

अंतरभूत पूर्णांक arithm_ipp_min8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

अंतरभूत पूर्णांक arithm_ipp_min16u(स्थिर ushort* src1, माप_प्रकार step1, स्थिर ushort* src2, माप_प्रकार step2,
                            ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

अंतरभूत पूर्णांक arithm_ipp_min32f(स्थिर भग्न* src1, माप_प्रकार step1, स्थिर भग्न* src2,माप_प्रकार step2,
                             भग्न* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, भग्न);
}

अंतरभूत पूर्णांक arithm_ipp_min64f(स्थिर द्विगुन* src1, माप_प्रकार step1, स्थिर द्विगुन* src2, माप_प्रकार step2,
                             द्विगुन* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, द्विगुन);
}

#घोषणा arithm_ipp_min8s(...)  0
#घोषणा arithm_ipp_min16s(...) 0
#घोषणा arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

अंतरभूत पूर्णांक arithm_ipp_absdiff8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                                uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

अंतरभूत पूर्णांक arithm_ipp_absdiff16u(स्थिर ushort* src1, माप_प्रकार step1, स्थिर ushort* src2, माप_प्रकार step2,
                                ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

अंतरभूत पूर्णांक arithm_ipp_absdiff32f(स्थिर भग्न* src1, माप_प्रकार step1, स्थिर भग्न* src2, माप_प्रकार step2,
                                भग्न* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}
#घोषणा arithm_ipp_absdiff8s(...)  0
#घोषणा arithm_ipp_absdiff16s(...) 0
#घोषणा arithm_ipp_absdiff32s(...) 0
#घोषणा arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

अंतरभूत पूर्णांक arithm_ipp_and8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

अंतरभूत पूर्णांक arithm_ipp_or8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

अंतरभूत पूर्णांक arithm_ipp_xor8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

अंतरभूत पूर्णांक arithm_ipp_not8u(स्थिर uchar* src1, माप_प्रकार step1, uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    अगर (!CV_IPP_CHECK_COND)
        वापस 0;
    अगर (height == 1)
        step1 = step = width * माप(dst[0]);
    अगर (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippi_आकार(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        वापस 1;
    }
    setIppErrorStatus();
    वापस 0;
}

//=======================================
// Compare
//=======================================

#घोषणा ARITHM_IPP_CMP(fun, ...)                          \
करो {                                                      \
    अगर (!CV_IPP_CHECK_COND)                               \
        वापस 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    अगर (op < 0)                                           \
        वापस 0;                                         \
    अगर (height == 1)                                      \
        step1 = step2 = step = width * माप(dst[0]);    \
    अगर (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        वापस 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    वापस 0;                                             \
} जबतक(0)

अंतरभूत IppCmpOp arithm_ipp_convert_cmp(पूर्णांक cmpop)
{
    चयन(cmpop)
    {
        हाल CMP_EQ: वापस ippCmpEq;
        हाल CMP_GT: वापस ippCmpGreater;
        हाल CMP_GE: वापस ippCmpGreaterEq;
        हाल CMP_LT: वापस ippCmpLess;
        हाल CMP_LE: वापस ippCmpLessEq;
        शेष:     वापस (IppCmpOp)-1;
    }
}

अंतरभूत पूर्णांक arithm_ipp_cmp8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, पूर्णांक cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

अंतरभूत पूर्णांक arithm_ipp_cmp16u(स्थिर ushort* src1, माप_प्रकार step1, स्थिर ushort* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, पूर्णांक cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

अंतरभूत पूर्णांक arithm_ipp_cmp16s(स्थिर लघु* src1, माप_प्रकार step1, स्थिर लघु* src2, माप_प्रकार step2,
                             uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, पूर्णांक cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

अंतरभूत पूर्णांक arithm_ipp_cmp32f(स्थिर भग्न* src1, माप_प्रकार step1, स्थिर भग्न* src2, माप_प्रकार step2,
                             uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, पूर्णांक cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippi_आकार(width, height));
}

#घोषणा arithm_ipp_cmp8s(...)  0
#घोषणा arithm_ipp_cmp32s(...) 0
#घोषणा arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#घोषणा ARITHM_IPP_MUL(fun, ...)                      \
करो {                                                  \
    अगर (!CV_IPP_CHECK_COND)                           \
        वापस 0;                                     \
    भग्न fscale = (भग्न)scale;                      \
    अगर (std::भ_असल(fscale - 1) > भग्न_अंतर)          \
        वापस 0;                                     \
    अगर (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        वापस 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    वापस 0;                                         \
} जबतक(0)

अंतरभूत पूर्णांक arithm_ipp_mul8u(स्थिर uchar *src1, माप_प्रकार step1, स्थिर uchar *src2, माप_प्रकार step2,
                            uchar *dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, द्विगुन scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2,dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}
अंतरभूत पूर्णांक arithm_ipp_mul16u(स्थिर ushort *src1, माप_प्रकार step1, स्थिर ushort *src2, माप_प्रकार step2,
                            ushort *dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, द्विगुन scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2,dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}

अंतरभूत पूर्णांक arithm_ipp_mul16s(स्थिर लघु *src1, माप_प्रकार step1, स्थिर लघु *src2, माप_प्रकार step2,
                            लघु *dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, द्विगुन scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2,dst, (पूर्णांक)step, ippi_आकार(width, height), 0);
}

अंतरभूत पूर्णांक arithm_ipp_mul32f(स्थिर भग्न *src1, माप_प्रकार step1, स्थिर भग्न *src2, माप_प्रकार step2,
                            भग्न *dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, द्विगुन scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2,dst, (पूर्णांक)step, ippi_आकार(width, height));
}

#घोषणा arithm_ipp_mul8s(...)  0
#घोषणा arithm_ipp_mul32s(...) 0
#घोषणा arithm_ipp_mul64f(...) 0

//=======================================
// Div
//=======================================

#घोषणा arithm_ipp_div8u(...)  0
#घोषणा arithm_ipp_div8s(...)  0
#घोषणा arithm_ipp_div16u(...) 0
#घोषणा arithm_ipp_div16s(...) 0
#घोषणा arithm_ipp_div32s(...) 0
#घोषणा arithm_ipp_div32f(...) 0
#घोषणा arithm_ipp_div64f(...) 0

//=======================================
// AddWeighted
//=======================================

#घोषणा arithm_ipp_addWeighted8u(...)  0
#घोषणा arithm_ipp_addWeighted8s(...)  0
#घोषणा arithm_ipp_addWeighted16u(...) 0
#घोषणा arithm_ipp_addWeighted16s(...) 0
#घोषणा arithm_ipp_addWeighted32s(...) 0
#घोषणा arithm_ipp_addWeighted32f(...) 0
#घोषणा arithm_ipp_addWeighted64f(...) 0

//=======================================
// Reciprocial
//=======================================

#घोषणा arithm_ipp_recip8u(...)  0
#घोषणा arithm_ipp_recip8s(...)  0
#घोषणा arithm_ipp_recip16u(...) 0
#घोषणा arithm_ipp_recip16s(...) 0
#घोषणा arithm_ipp_recip32s(...) 0
#घोषणा arithm_ipp_recip32f(...) 0
#घोषणा arithm_ipp_recip64f(...) 0

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

#घोषणा ARITHM_CALL_IPP(fun, ...)       \
{                                       \
    अगर (__CV_EXPAND(fun(__VA_ARGS__)))  \
        वापस;                         \
}

#पूर्ण_अगर // ARITHM_USE_IPP


#अगर !ARITHM_USE_IPP
#घोषणा ARITHM_CALL_IPP(...)
#पूर्ण_अगर
