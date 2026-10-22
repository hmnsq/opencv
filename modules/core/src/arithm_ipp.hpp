// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#જો ARITHM_USE_IPP

નામાકાશ cv { નામાકાશ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
કરો {                                                    \
    જો (!CV_IPP_CHECK_COND)                             \
        પાછા 0;                                       \
    જો (height == 1)                                    \
        step1 = step2 = step = width * કદ(dst[0]);  \
    જો (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        પાછા 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    પાછા 0;                                           \
} જ્યારે(0)

//=======================================
// Addition
//=======================================

અંતઃરેખ પૂર્ણાંક arithm_ipp_add8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                            uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_add16u(અચલ ushort* src1, size_t step1, અચલ ushort* src2, size_t step2,
                             ushort* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_add16s(અચલ short* src1, size_t step1, અચલ short* src2, size_t step2,
                             short* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_add32f(અચલ દશાંશ* src1, size_t step1, અચલ દશાંશ* src2, size_t step2,
                             દશાંશ* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

અંતઃરેખ પૂર્ણાંક arithm_ipp_sub8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                            uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (પૂર્ણાંક)step2, src1, (પૂર્ણાંક)step1, dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_sub16u(અચલ ushort* src1, size_t step1, અચલ ushort* src2, size_t step2,
                             ushort* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (પૂર્ણાંક)step2, src1, (પૂર્ણાંક)step1, dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_sub16s(અચલ short* src1, size_t step1, અચલ short* src2, size_t step2,
                            short* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (પૂર્ણાંક)step2, src1, (પૂર્ણાંક)step1, dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_sub32f(અચલ દશાંશ* src1, size_t step1, અચલ દશાંશ* src2, size_t step2,
                            દશાંશ* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (પૂર્ણાંક)step2, src1, (પૂર્ણાંક)step1, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
કરો {                                                             \
    જો (!CV_IPP_CHECK_COND)                                      \
        પાછા 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    જો (height == 1)                                             \
        step1 = step2 = step = width * કદ(dst[0]);           \
    પૂર્ણાંક i = 0;                                                   \
    માટે(; i < height; i++)                                       \
    {                                                            \
        જો (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            તોડો;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    જો (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        પાછા 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    પાછા 0;                                                    \
} જ્યારે(0)

//=======================================
// Max
//=======================================

અંતઃરેખ પૂર્ણાંક arithm_ipp_max8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                           uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_max16u(અચલ ushort* src1, size_t step1, અચલ ushort* src2, size_t step2,
                             ushort* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_max32f(અચલ દશાંશ* src1, size_t step1, અચલ દશાંશ* src2, size_t step2,
                             દશાંશ* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, દશાંશ);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_max64f(અચલ દ્વિગુણ* src1, size_t step1, અચલ દ્વિગુણ* src2, size_t step2,
                             દ્વિગુણ* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, દ્વિગુણ);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

અંતઃરેખ પૂર્ણાંક arithm_ipp_min8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                            uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_min16u(અચલ ushort* src1, size_t step1, અચલ ushort* src2, size_t step2,
                            ushort* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_min32f(અચલ દશાંશ* src1, size_t step1, અચલ દશાંશ* src2,size_t step2,
                             દશાંશ* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, દશાંશ);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_min64f(અચલ દ્વિગુણ* src1, size_t step1, અચલ દ્વિગુણ* src2, size_t step2,
                             દ્વિગુણ* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, દ્વિગુણ);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

અંતઃરેખ પૂર્ણાંક arithm_ipp_absdiff8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                                uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_absdiff16u(અચલ ushort* src1, size_t step1, અચલ ushort* src2, size_t step2,
                                ushort* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_absdiff32f(અચલ દશાંશ* src1, size_t step1, અચલ દશાંશ* src2, size_t step2,
                                દશાંશ* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

અંતઃરેખ પૂર્ણાંક arithm_ipp_and8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                            uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_or8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                           uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_xor8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                            uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_not8u(અચલ uchar* src1, size_t step1, uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height)
{
    જો (!CV_IPP_CHECK_COND)
        પાછા 0;
    જો (height == 1)
        step1 = step = width * કદ(dst[0]);
    જો (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (પૂર્ણાંક)step1, dst, (પૂર્ણાંક)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        પાછા 1;
    }
    setIppErrorStatus();
    પાછા 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
કરો {                                                      \
    જો (!CV_IPP_CHECK_COND)                               \
        પાછા 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    જો (op < 0)                                           \
        પાછા 0;                                         \
    જો (height == 1)                                      \
        step1 = step2 = step = width * કદ(dst[0]);    \
    જો (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        પાછા 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    પાછા 0;                                             \
} જ્યારે(0)

અંતઃરેખ IppCmpOp arithm_ipp_convert_cmp(પૂર્ણાંક cmpop)
{
    પસંદગી(cmpop)
    {
        સ્થિતિ CMP_EQ: પાછા ippCmpEq;
        સ્થિતિ CMP_GT: પાછા ippCmpGreater;
        સ્થિતિ CMP_GE: પાછા ippCmpGreaterEq;
        સ્થિતિ CMP_LT: પાછા ippCmpLess;
        સ્થિતિ CMP_LE: પાછા ippCmpLessEq;
        મૂળભૂત:     પાછા (IppCmpOp)-1;
    }
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_cmp8u(અચલ uchar* src1, size_t step1, અચલ uchar* src2, size_t step2,
                            uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, પૂર્ણાંક cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_cmp16u(અચલ ushort* src1, size_t step1, અચલ ushort* src2, size_t step2,
                            uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, પૂર્ણાંક cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_cmp16s(અચલ short* src1, size_t step1, અચલ short* src2, size_t step2,
                             uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, પૂર્ણાંક cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_cmp32f(અચલ દશાંશ* src1, size_t step1, અચલ દશાંશ* src2, size_t step2,
                             uchar* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, પૂર્ણાંક cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2, dst, (પૂર્ણાંક)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
કરો {                                                  \
    જો (!CV_IPP_CHECK_COND)                           \
        પાછા 0;                                     \
    દશાંશ fscale = (દશાંશ)scale;                      \
    જો (std::fabs(fscale - 1) > FLT_EPSILON)          \
        પાછા 0;                                     \
    જો (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        પાછા 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    પાછા 0;                                         \
} જ્યારે(0)

અંતઃરેખ પૂર્ણાંક arithm_ipp_mul8u(અચલ uchar *src1, size_t step1, અચલ uchar *src2, size_t step2,
                            uchar *dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, દ્વિગુણ scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2,dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}
અંતઃરેખ પૂર્ણાંક arithm_ipp_mul16u(અચલ ushort *src1, size_t step1, અચલ ushort *src2, size_t step2,
                            ushort *dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, દ્વિગુણ scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2,dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_mul16s(અચલ short *src1, size_t step1, અચલ short *src2, size_t step2,
                            short *dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, દ્વિગુણ scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2,dst, (પૂર્ણાંક)step, ippiSize(width, height), 0);
}

અંતઃરેખ પૂર્ણાંક arithm_ipp_mul32f(અચલ દશાંશ *src1, size_t step1, અચલ દશાંશ *src2, size_t step2,
                            દશાંશ *dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height, દ્વિગુણ scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (પૂર્ણાંક)step1, src2, (પૂર્ણાંક)step2,dst, (પૂર્ણાંક)step, ippiSize(width, height));
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
    જો (__CV_EXPAND(fun(__VA_ARGS__)))  \
        પાછા;                         \
}

#endif // ARITHM_USE_IPP


#જો !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
