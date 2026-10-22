// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#எனில் ARITHM_USE_IPP

நாமாகாஶ cv { நாமாகாஶ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
செய் {                                                    \
    எனில் (!CV_IPP_CHECK_COND)                             \
        திரும்பு 0;                                       \
    எனில் (height == 1)                                    \
        step1 = step2 = step = width * அளவு(dst[0]);  \
    எனில் (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        திரும்பு 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    திரும்பு 0;                                           \
} போது(0)

//=======================================
// Addition
//=======================================

உள்வரி முழு arithm_ipp_add8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                            uchar* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height), 0);
}

உள்வரி முழு arithm_ipp_add16u(மாறிலி ushort* src1, size_t step1, மாறிலி ushort* src2, size_t step2,
                             ushort* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height), 0);
}

உள்வரி முழு arithm_ipp_add16s(மாறிலி short* src1, size_t step1, மாறிலி short* src2, size_t step2,
                             short* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height), 0);
}

உள்வரி முழு arithm_ipp_add32f(மாறிலி தசம* src1, size_t step1, மாறிலி தசம* src2, size_t step2,
                             தசம* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

உள்வரி முழு arithm_ipp_sub8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                            uchar* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (முழு)step2, src1, (முழு)step1, dst, (முழு)step, ippiSize(width, height), 0);
}

உள்வரி முழு arithm_ipp_sub16u(மாறிலி ushort* src1, size_t step1, மாறிலி ushort* src2, size_t step2,
                             ushort* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (முழு)step2, src1, (முழு)step1, dst, (முழு)step, ippiSize(width, height), 0);
}

உள்வரி முழு arithm_ipp_sub16s(மாறிலி short* src1, size_t step1, மாறிலி short* src2, size_t step2,
                            short* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (முழு)step2, src1, (முழு)step1, dst, (முழு)step, ippiSize(width, height), 0);
}

உள்வரி முழு arithm_ipp_sub32f(மாறிலி தசம* src1, size_t step1, மாறிலி தசம* src2, size_t step2,
                            தசம* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (முழு)step2, src1, (முழு)step1, dst, (முழு)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
செய் {                                                             \
    எனில் (!CV_IPP_CHECK_COND)                                      \
        திரும்பு 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    எனில் (height == 1)                                             \
        step1 = step2 = step = width * அளவு(dst[0]);           \
    முழு i = 0;                                                   \
    க்கு(; i < height; i++)                                       \
    {                                                            \
        எனில் (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            நிறுத்து;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    எனில் (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        திரும்பு 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    திரும்பு 0;                                                    \
} போது(0)

//=======================================
// Max
//=======================================

உள்வரி முழு arithm_ipp_max8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                           uchar* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

உள்வரி முழு arithm_ipp_max16u(மாறிலி ushort* src1, size_t step1, மாறிலி ushort* src2, size_t step2,
                             ushort* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

உள்வரி முழு arithm_ipp_max32f(மாறிலி தசம* src1, size_t step1, மாறிலி தசம* src2, size_t step2,
                             தசம* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, தசம);
}

உள்வரி முழு arithm_ipp_max64f(மாறிலி இரட்டை* src1, size_t step1, மாறிலி இரட்டை* src2, size_t step2,
                             இரட்டை* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, இரட்டை);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

உள்வரி முழு arithm_ipp_min8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                            uchar* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

உள்வரி முழு arithm_ipp_min16u(மாறிலி ushort* src1, size_t step1, மாறிலி ushort* src2, size_t step2,
                            ushort* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

உள்வரி முழு arithm_ipp_min32f(மாறிலி தசம* src1, size_t step1, மாறிலி தசம* src2,size_t step2,
                             தசம* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, தசம);
}

உள்வரி முழு arithm_ipp_min64f(மாறிலி இரட்டை* src1, size_t step1, மாறிலி இரட்டை* src2, size_t step2,
                             இரட்டை* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, இரட்டை);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

உள்வரி முழு arithm_ipp_absdiff8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                                uchar* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

உள்வரி முழு arithm_ipp_absdiff16u(மாறிலி ushort* src1, size_t step1, மாறிலி ushort* src2, size_t step2,
                                ushort* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

உள்வரி முழு arithm_ipp_absdiff32f(மாறிலி தசம* src1, size_t step1, மாறிலி தசம* src2, size_t step2,
                                தசம* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

உள்வரி முழு arithm_ipp_and8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                            uchar* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

உள்வரி முழு arithm_ipp_or8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                           uchar* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

உள்வரி முழு arithm_ipp_xor8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                            uchar* dst, size_t step, முழு width, முழு height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

உள்வரி முழு arithm_ipp_not8u(மாறிலி uchar* src1, size_t step1, uchar* dst, size_t step, முழு width, முழு height)
{
    எனில் (!CV_IPP_CHECK_COND)
        திரும்பு 0;
    எனில் (height == 1)
        step1 = step = width * அளவு(dst[0]);
    எனில் (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (முழு)step1, dst, (முழு)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        திரும்பு 1;
    }
    setIppErrorStatus();
    திரும்பு 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
செய் {                                                      \
    எனில் (!CV_IPP_CHECK_COND)                               \
        திரும்பு 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    எனில் (op < 0)                                           \
        திரும்பு 0;                                         \
    எனில் (height == 1)                                      \
        step1 = step2 = step = width * அளவு(dst[0]);    \
    எனில் (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        திரும்பு 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    திரும்பு 0;                                             \
} போது(0)

உள்வரி IppCmpOp arithm_ipp_convert_cmp(முழு cmpop)
{
    தேர்வு(cmpop)
    {
        நிலை CMP_EQ: திரும்பு ippCmpEq;
        நிலை CMP_GT: திரும்பு ippCmpGreater;
        நிலை CMP_GE: திரும்பு ippCmpGreaterEq;
        நிலை CMP_LT: திரும்பு ippCmpLess;
        நிலை CMP_LE: திரும்பு ippCmpLessEq;
        இயல்பு:     திரும்பு (IppCmpOp)-1;
    }
}

உள்வரி முழு arithm_ipp_cmp8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                            uchar* dst, size_t step, முழு width, முழு height, முழு cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

உள்வரி முழு arithm_ipp_cmp16u(மாறிலி ushort* src1, size_t step1, மாறிலி ushort* src2, size_t step2,
                            uchar* dst, size_t step, முழு width, முழு height, முழு cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

உள்வரி முழு arithm_ipp_cmp16s(மாறிலி short* src1, size_t step1, மாறிலி short* src2, size_t step2,
                             uchar* dst, size_t step, முழு width, முழு height, முழு cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

உள்வரி முழு arithm_ipp_cmp32f(மாறிலி தசம* src1, size_t step1, மாறிலி தசம* src2, size_t step2,
                             uchar* dst, size_t step, முழு width, முழு height, முழு cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (முழு)step1, src2, (முழு)step2, dst, (முழு)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
செய் {                                                  \
    எனில் (!CV_IPP_CHECK_COND)                           \
        திரும்பு 0;                                     \
    தசம fscale = (தசம)scale;                      \
    எனில் (std::fabs(fscale - 1) > FLT_EPSILON)          \
        திரும்பு 0;                                     \
    எனில் (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        திரும்பு 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    திரும்பு 0;                                         \
} போது(0)

உள்வரி முழு arithm_ipp_mul8u(மாறிலி uchar *src1, size_t step1, மாறிலி uchar *src2, size_t step2,
                            uchar *dst, size_t step, முழு width, முழு height, இரட்டை scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (முழு)step1, src2, (முழு)step2,dst, (முழு)step, ippiSize(width, height), 0);
}
உள்வரி முழு arithm_ipp_mul16u(மாறிலி ushort *src1, size_t step1, மாறிலி ushort *src2, size_t step2,
                            ushort *dst, size_t step, முழு width, முழு height, இரட்டை scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (முழு)step1, src2, (முழு)step2,dst, (முழு)step, ippiSize(width, height), 0);
}

உள்வரி முழு arithm_ipp_mul16s(மாறிலி short *src1, size_t step1, மாறிலி short *src2, size_t step2,
                            short *dst, size_t step, முழு width, முழு height, இரட்டை scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (முழு)step1, src2, (முழு)step2,dst, (முழு)step, ippiSize(width, height), 0);
}

உள்வரி முழு arithm_ipp_mul32f(மாறிலி தசம *src1, size_t step1, மாறிலி தசம *src2, size_t step2,
                            தசம *dst, size_t step, முழு width, முழு height, இரட்டை scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (முழு)step1, src2, (முழு)step2,dst, (முழு)step, ippiSize(width, height));
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
    எனில் (__CV_EXPAND(fun(__VA_ARGS__)))  \
        திரும்பு;                         \
}

#endif // ARITHM_USE_IPP


#எனில் !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
