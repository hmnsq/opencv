// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#जर ARITHM_USE_IPP

नामाकाश cv { नामाकाश hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
करा {                                                    \
    जर (!CV_IPP_CHECK_COND)                             \
        परत 0;                                       \
    जर (height == 1)                                    \
        step1 = step2 = step = width * आकार(dst[0]);  \
    जर (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        परत 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    परत 0;                                           \
} पर्यंत(0)

//=======================================
// Addition
//=======================================

ओळीत पूर्णांक arithm_ipp_add8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height), 0);
}

ओळीत पूर्णांक arithm_ipp_add16u(स्थिरांक ushort* src1, माप_प्रकार step1, स्थिरांक ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height), 0);
}

ओळीत पूर्णांक arithm_ipp_add16s(स्थिरांक लघु* src1, माप_प्रकार step1, स्थिरांक लघु* src2, माप_प्रकार step2,
                             लघु* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height), 0);
}

ओळीत पूर्णांक arithm_ipp_add32f(स्थिरांक दशांश* src1, माप_प्रकार step1, स्थिरांक दशांश* src2, माप_प्रकार step2,
                             दशांश* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

ओळीत पूर्णांक arithm_ipp_sub8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (पूर्णांक)step2, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippiSize(width, height), 0);
}

ओळीत पूर्णांक arithm_ipp_sub16u(स्थिरांक ushort* src1, माप_प्रकार step1, स्थिरांक ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (पूर्णांक)step2, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippiSize(width, height), 0);
}

ओळीत पूर्णांक arithm_ipp_sub16s(स्थिरांक लघु* src1, माप_प्रकार step1, स्थिरांक लघु* src2, माप_प्रकार step2,
                            लघु* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (पूर्णांक)step2, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippiSize(width, height), 0);
}

ओळीत पूर्णांक arithm_ipp_sub32f(स्थिरांक दशांश* src1, माप_प्रकार step1, स्थिरांक दशांश* src2, माप_प्रकार step2,
                            दशांश* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (पूर्णांक)step2, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
करा {                                                             \
    जर (!CV_IPP_CHECK_COND)                                      \
        परत 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    जर (height == 1)                                             \
        step1 = step2 = step = width * आकार(dst[0]);           \
    पूर्णांक i = 0;                                                   \
    साठी(; i < height; i++)                                       \
    {                                                            \
        जर (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            थांबा;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    जर (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        परत 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    परत 0;                                                    \
} पर्यंत(0)

//=======================================
// Max
//=======================================

ओळीत पूर्णांक arithm_ipp_max8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

ओळीत पूर्णांक arithm_ipp_max16u(स्थिरांक ushort* src1, माप_प्रकार step1, स्थिरांक ushort* src2, माप_प्रकार step2,
                             ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

ओळीत पूर्णांक arithm_ipp_max32f(स्थिरांक दशांश* src1, माप_प्रकार step1, स्थिरांक दशांश* src2, माप_प्रकार step2,
                             दशांश* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, दशांश);
}

ओळीत पूर्णांक arithm_ipp_max64f(स्थिरांक द्विगुण* src1, माप_प्रकार step1, स्थिरांक द्विगुण* src2, माप_प्रकार step2,
                             द्विगुण* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, द्विगुण);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

ओळीत पूर्णांक arithm_ipp_min8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

ओळीत पूर्णांक arithm_ipp_min16u(स्थिरांक ushort* src1, माप_प्रकार step1, स्थिरांक ushort* src2, माप_प्रकार step2,
                            ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

ओळीत पूर्णांक arithm_ipp_min32f(स्थिरांक दशांश* src1, माप_प्रकार step1, स्थिरांक दशांश* src2,माप_प्रकार step2,
                             दशांश* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, दशांश);
}

ओळीत पूर्णांक arithm_ipp_min64f(स्थिरांक द्विगुण* src1, माप_प्रकार step1, स्थिरांक द्विगुण* src2, माप_प्रकार step2,
                             द्विगुण* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, द्विगुण);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

ओळीत पूर्णांक arithm_ipp_absdiff8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                                uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

ओळीत पूर्णांक arithm_ipp_absdiff16u(स्थिरांक ushort* src1, माप_प्रकार step1, स्थिरांक ushort* src2, माप_प्रकार step2,
                                ushort* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

ओळीत पूर्णांक arithm_ipp_absdiff32f(स्थिरांक दशांश* src1, माप_प्रकार step1, स्थिरांक दशांश* src2, माप_प्रकार step2,
                                दशांश* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

ओळीत पूर्णांक arithm_ipp_and8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

ओळीत पूर्णांक arithm_ipp_or8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

ओळीत पूर्णांक arithm_ipp_xor8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

ओळीत पूर्णांक arithm_ipp_not8u(स्थिरांक uchar* src1, माप_प्रकार step1, uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    जर (!CV_IPP_CHECK_COND)
        परत 0;
    जर (height == 1)
        step1 = step = width * आकार(dst[0]);
    जर (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (पूर्णांक)step1, dst, (पूर्णांक)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        परत 1;
    }
    setIppErrorStatus();
    परत 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
करा {                                                      \
    जर (!CV_IPP_CHECK_COND)                               \
        परत 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    जर (op < 0)                                           \
        परत 0;                                         \
    जर (height == 1)                                      \
        step1 = step2 = step = width * आकार(dst[0]);    \
    जर (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        परत 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    परत 0;                                             \
} पर्यंत(0)

ओळीत IppCmpOp arithm_ipp_convert_cmp(पूर्णांक cmpop)
{
    निवड(cmpop)
    {
        स्थिती CMP_EQ: परत ippCmpEq;
        स्थिती CMP_GT: परत ippCmpGreater;
        स्थिती CMP_GE: परत ippCmpGreaterEq;
        स्थिती CMP_LT: परत ippCmpLess;
        स्थिती CMP_LE: परत ippCmpLessEq;
        पूर्वनिर्धारित:     परत (IppCmpOp)-1;
    }
}

ओळीत पूर्णांक arithm_ipp_cmp8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, पूर्णांक cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

ओळीत पूर्णांक arithm_ipp_cmp16u(स्थिरांक ushort* src1, माप_प्रकार step1, स्थिरांक ushort* src2, माप_प्रकार step2,
                            uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, पूर्णांक cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

ओळीत पूर्णांक arithm_ipp_cmp16s(स्थिरांक लघु* src1, माप_प्रकार step1, स्थिरांक लघु* src2, माप_प्रकार step2,
                             uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, पूर्णांक cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

ओळीत पूर्णांक arithm_ipp_cmp32f(स्थिरांक दशांश* src1, माप_प्रकार step1, स्थिरांक दशांश* src2, माप_प्रकार step2,
                             uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, पूर्णांक cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2, dst, (पूर्णांक)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
करा {                                                  \
    जर (!CV_IPP_CHECK_COND)                           \
        परत 0;                                     \
    दशांश fscale = (दशांश)scale;                      \
    जर (std::भ_असल(fscale - 1) > भग्न_अंतर)          \
        परत 0;                                     \
    जर (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        परत 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    परत 0;                                         \
} पर्यंत(0)

ओळीत पूर्णांक arithm_ipp_mul8u(स्थिरांक uchar *src1, माप_प्रकार step1, स्थिरांक uchar *src2, माप_प्रकार step2,
                            uchar *dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, द्विगुण scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2,dst, (पूर्णांक)step, ippiSize(width, height), 0);
}
ओळीत पूर्णांक arithm_ipp_mul16u(स्थिरांक ushort *src1, माप_प्रकार step1, स्थिरांक ushort *src2, माप_प्रकार step2,
                            ushort *dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, द्विगुण scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2,dst, (पूर्णांक)step, ippiSize(width, height), 0);
}

ओळीत पूर्णांक arithm_ipp_mul16s(स्थिरांक लघु *src1, माप_प्रकार step1, स्थिरांक लघु *src2, माप_प्रकार step2,
                            लघु *dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, द्विगुण scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2,dst, (पूर्णांक)step, ippiSize(width, height), 0);
}

ओळीत पूर्णांक arithm_ipp_mul32f(स्थिरांक दशांश *src1, माप_प्रकार step1, स्थिरांक दशांश *src2, माप_प्रकार step2,
                            दशांश *dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, द्विगुण scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (पूर्णांक)step1, src2, (पूर्णांक)step2,dst, (पूर्णांक)step, ippiSize(width, height));
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
    जर (__CV_EXPAND(fun(__VA_ARGS__)))  \
        परत;                         \
}

#endif // ARITHM_USE_IPP


#जर !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
