// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#यदि ARITHM_USE_IPP

नामाकाश cv { नामाकाश hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
गर {                                                    \
    यदि (!CV_IPP_CHECK_COND)                             \
        फर्काऊ 0;                                       \
    यदि (height == 1)                                    \
        step1 = step2 = step = width * परिमाण(dst[0]);  \
    यदि (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        फर्काऊ 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    फर्काऊ 0;                                           \
} जबसम्म(0)

//=======================================
// Addition
//=======================================

अन्तःस्थ पूर्णाङ्क arithm_ipp_add8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_add16u(अचर ushort* src1, माप_प्रकार step1, अचर ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_add16s(अचर लघु* src1, माप_प्रकार step1, अचर लघु* src2, माप_प्रकार step2,
                             लघु* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_add32f(अचर भिन्न* src1, माप_प्रकार step1, अचर भिन्न* src2, माप_प्रकार step2,
                             भिन्न* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

अन्तःस्थ पूर्णाङ्क arithm_ipp_sub8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (पूर्णाङ्क)step2, src1, (पूर्णाङ्क)step1, dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_sub16u(अचर ushort* src1, माप_प्रकार step1, अचर ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (पूर्णाङ्क)step2, src1, (पूर्णाङ्क)step1, dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_sub16s(अचर लघु* src1, माप_प्रकार step1, अचर लघु* src2, माप_प्रकार step2,
                            लघु* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (पूर्णाङ्क)step2, src1, (पूर्णाङ्क)step1, dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_sub32f(अचर भिन्न* src1, माप_प्रकार step1, अचर भिन्न* src2, माप_प्रकार step2,
                            भिन्न* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (पूर्णाङ्क)step2, src1, (पूर्णाङ्क)step1, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
गर {                                                             \
    यदि (!CV_IPP_CHECK_COND)                                      \
        फर्काऊ 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    यदि (height == 1)                                             \
        step1 = step2 = step = width * परिमाण(dst[0]);           \
    पूर्णाङ्क i = 0;                                                   \
    लागि(; i < height; i++)                                       \
    {                                                            \
        यदि (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            रोक;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    यदि (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        फर्काऊ 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    फर्काऊ 0;                                                    \
} जबसम्म(0)

//=======================================
// Max
//=======================================

अन्तःस्थ पूर्णाङ्क arithm_ipp_max8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                           uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_max16u(अचर ushort* src1, माप_प्रकार step1, अचर ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_max32f(अचर भिन्न* src1, माप_प्रकार step1, अचर भिन्न* src2, माप_प्रकार step2,
                             भिन्न* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, भिन्न);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_max64f(अचर द्विगुण* src1, माप_प्रकार step1, अचर द्विगुण* src2, माप_प्रकार step2,
                             द्विगुण* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, द्विगुण);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

अन्तःस्थ पूर्णाङ्क arithm_ipp_min8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_min16u(अचर ushort* src1, माप_प्रकार step1, अचर ushort* src2, माप_प्रकार step2,
                            ushort* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_min32f(अचर भिन्न* src1, माप_प्रकार step1, अचर भिन्न* src2,माप_प्रकार step2,
                             भिन्न* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, भिन्न);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_min64f(अचर द्विगुण* src1, माप_प्रकार step1, अचर द्विगुण* src2, माप_प्रकार step2,
                             द्विगुण* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, द्विगुण);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

अन्तःस्थ पूर्णाङ्क arithm_ipp_absdiff8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                                uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_absdiff16u(अचर ushort* src1, माप_प्रकार step1, अचर ushort* src2, माप_प्रकार step2,
                                ushort* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_absdiff32f(अचर भिन्न* src1, माप_प्रकार step1, अचर भिन्न* src2, माप_प्रकार step2,
                                भिन्न* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

अन्तःस्थ पूर्णाङ्क arithm_ipp_and8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_or8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                           uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_xor8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_not8u(अचर uchar* src1, माप_प्रकार step1, uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    यदि (!CV_IPP_CHECK_COND)
        फर्काऊ 0;
    यदि (height == 1)
        step1 = step = width * परिमाण(dst[0]);
    यदि (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (पूर्णाङ्क)step1, dst, (पूर्णाङ्क)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        फर्काऊ 1;
    }
    setIppErrorStatus();
    फर्काऊ 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
गर {                                                      \
    यदि (!CV_IPP_CHECK_COND)                               \
        फर्काऊ 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    यदि (op < 0)                                           \
        फर्काऊ 0;                                         \
    यदि (height == 1)                                      \
        step1 = step2 = step = width * परिमाण(dst[0]);    \
    यदि (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        फर्काऊ 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    फर्काऊ 0;                                             \
} जबसम्म(0)

अन्तःस्थ IppCmpOp arithm_ipp_convert_cmp(पूर्णाङ्क cmpop)
{
    विकल्प(cmpop)
    {
        पक्ष CMP_EQ: फर्काऊ ippCmpEq;
        पक्ष CMP_GT: फर्काऊ ippCmpGreater;
        पक्ष CMP_GE: फर्काऊ ippCmpGreaterEq;
        पक्ष CMP_LT: फर्काऊ ippCmpLess;
        पक्ष CMP_LE: फर्काऊ ippCmpLessEq;
        स्वभाव:     फर्काऊ (IppCmpOp)-1;
    }
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_cmp8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, पूर्णाङ्क cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_cmp16u(अचर ushort* src1, माप_प्रकार step1, अचर ushort* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, पूर्णाङ्क cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_cmp16s(अचर लघु* src1, माप_प्रकार step1, अचर लघु* src2, माप_प्रकार step2,
                             uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, पूर्णाङ्क cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_cmp32f(अचर भिन्न* src1, माप_प्रकार step1, अचर भिन्न* src2, माप_प्रकार step2,
                             uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, पूर्णाङ्क cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2, dst, (पूर्णाङ्क)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
गर {                                                  \
    यदि (!CV_IPP_CHECK_COND)                           \
        फर्काऊ 0;                                     \
    भिन्न fscale = (भिन्न)scale;                      \
    यदि (std::भ_असल(fscale - 1) > भग्न_अंतर)          \
        फर्काऊ 0;                                     \
    यदि (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        फर्काऊ 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    फर्काऊ 0;                                         \
} जबसम्म(0)

अन्तःस्थ पूर्णाङ्क arithm_ipp_mul8u(अचर uchar *src1, माप_प्रकार step1, अचर uchar *src2, माप_प्रकार step2,
                            uchar *dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, द्विगुण scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2,dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}
अन्तःस्थ पूर्णाङ्क arithm_ipp_mul16u(अचर ushort *src1, माप_प्रकार step1, अचर ushort *src2, माप_प्रकार step2,
                            ushort *dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, द्विगुण scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2,dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_mul16s(अचर लघु *src1, माप_प्रकार step1, अचर लघु *src2, माप_प्रकार step2,
                            लघु *dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, द्विगुण scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2,dst, (पूर्णाङ्क)step, ippiSize(width, height), 0);
}

अन्तःस्थ पूर्णाङ्क arithm_ipp_mul32f(अचर भिन्न *src1, माप_प्रकार step1, अचर भिन्न *src2, माप_प्रकार step2,
                            भिन्न *dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, द्विगुण scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (पूर्णाङ्क)step1, src2, (पूर्णाङ्क)step2,dst, (पूर्णाङ्क)step, ippiSize(width, height));
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
    यदि (__CV_EXPAND(fun(__VA_ARGS__)))  \
        फर्काऊ;                         \
}

#endif // ARITHM_USE_IPP


#यदि !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
