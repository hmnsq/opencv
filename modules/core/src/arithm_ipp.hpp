// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#എങ്കിൽ ARITHM_USE_IPP

നാമാകാശ cv { നാമാകാശ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
ചെയ്യുക {                                                    \
    എങ്കിൽ (!CV_IPP_CHECK_COND)                             \
        തിരികെ 0;                                       \
    എങ്കിൽ (height == 1)                                    \
        step1 = step2 = step = width * വലുപ്പം(dst[0]);  \
    എങ്കിൽ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        തിരികെ 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    തിരികെ 0;                                           \
} ആകുമ്പോൾ(0)

//=======================================
// Addition
//=======================================

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_add8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                            uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_add16u(സ്ഥിരം ushort* src1, size_t step1, സ്ഥിരം ushort* src2, size_t step2,
                             ushort* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_add16s(സ്ഥിരം short* src1, size_t step1, സ്ഥിരം short* src2, size_t step2,
                             short* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_add32f(സ്ഥിരം ദശാംശം* src1, size_t step1, സ്ഥിരം ദശാംശം* src2, size_t step2,
                             ദശാംശം* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_sub8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                            uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (പൂർണ്ണസംഖ്യ)step2, src1, (പൂർണ്ണസംഖ്യ)step1, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_sub16u(സ്ഥിരം ushort* src1, size_t step1, സ്ഥിരം ushort* src2, size_t step2,
                             ushort* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (പൂർണ്ണസംഖ്യ)step2, src1, (പൂർണ്ണസംഖ്യ)step1, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_sub16s(സ്ഥിരം short* src1, size_t step1, സ്ഥിരം short* src2, size_t step2,
                            short* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (പൂർണ്ണസംഖ്യ)step2, src1, (പൂർണ്ണസംഖ്യ)step1, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_sub32f(സ്ഥിരം ദശാംശം* src1, size_t step1, സ്ഥിരം ദശാംശം* src2, size_t step2,
                            ദശാംശം* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (പൂർണ്ണസംഖ്യ)step2, src1, (പൂർണ്ണസംഖ്യ)step1, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
ചെയ്യുക {                                                             \
    എങ്കിൽ (!CV_IPP_CHECK_COND)                                      \
        തിരികെ 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    എങ്കിൽ (height == 1)                                             \
        step1 = step2 = step = width * വലുപ്പം(dst[0]);           \
    പൂർണ്ണസംഖ്യ i = 0;                                                   \
    വേണ്ടി(; i < height; i++)                                       \
    {                                                            \
        എങ്കിൽ (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            നിർത്തുക;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    എങ്കിൽ (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        തിരികെ 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    തിരികെ 0;                                                    \
} ആകുമ്പോൾ(0)

//=======================================
// Max
//=======================================

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_max8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                           uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_max16u(സ്ഥിരം ushort* src1, size_t step1, സ്ഥിരം ushort* src2, size_t step2,
                             ushort* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_max32f(സ്ഥിരം ദശാംശം* src1, size_t step1, സ്ഥിരം ദശാംശം* src2, size_t step2,
                             ദശാംശം* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, ദശാംശം);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_max64f(സ്ഥിരം ഇരട്ട* src1, size_t step1, സ്ഥിരം ഇരട്ട* src2, size_t step2,
                             ഇരട്ട* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, ഇരട്ട);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_min8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                            uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_min16u(സ്ഥിരം ushort* src1, size_t step1, സ്ഥിരം ushort* src2, size_t step2,
                            ushort* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_min32f(സ്ഥിരം ദശാംശം* src1, size_t step1, സ്ഥിരം ദശാംശം* src2,size_t step2,
                             ദശാംശം* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, ദശാംശം);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_min64f(സ്ഥിരം ഇരട്ട* src1, size_t step1, സ്ഥിരം ഇരട്ട* src2, size_t step2,
                             ഇരട്ട* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, ഇരട്ട);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_absdiff8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                                uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_absdiff16u(സ്ഥിരം ushort* src1, size_t step1, സ്ഥിരം ushort* src2, size_t step2,
                                ushort* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_absdiff32f(സ്ഥിരം ദശാംശം* src1, size_t step1, സ്ഥിരം ദശാംശം* src2, size_t step2,
                                ദശാംശം* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_and8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                            uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_or8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                           uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_xor8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                            uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_not8u(സ്ഥിരം uchar* src1, size_t step1, uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    എങ്കിൽ (!CV_IPP_CHECK_COND)
        തിരികെ 0;
    എങ്കിൽ (height == 1)
        step1 = step = width * വലുപ്പം(dst[0]);
    എങ്കിൽ (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        തിരികെ 1;
    }
    setIppErrorStatus();
    തിരികെ 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
ചെയ്യുക {                                                      \
    എങ്കിൽ (!CV_IPP_CHECK_COND)                               \
        തിരികെ 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    എങ്കിൽ (op < 0)                                           \
        തിരികെ 0;                                         \
    എങ്കിൽ (height == 1)                                      \
        step1 = step2 = step = width * വലുപ്പം(dst[0]);    \
    എങ്കിൽ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        തിരികെ 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    തിരികെ 0;                                             \
} ആകുമ്പോൾ(0)

വരിയിൽ IppCmpOp arithm_ipp_convert_cmp(പൂർണ്ണസംഖ്യ cmpop)
{
    തിരഞ്ഞെടുപ്പ്(cmpop)
    {
        അവസ്ഥ CMP_EQ: തിരികെ ippCmpEq;
        അവസ്ഥ CMP_GT: തിരികെ ippCmpGreater;
        അവസ്ഥ CMP_GE: തിരികെ ippCmpGreaterEq;
        അവസ്ഥ CMP_LT: തിരികെ ippCmpLess;
        അവസ്ഥ CMP_LE: തിരികെ ippCmpLessEq;
        സ്ഥിരസ്ഥിതി:     തിരികെ (IppCmpOp)-1;
    }
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_cmp8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                            uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_cmp16u(സ്ഥിരം ushort* src1, size_t step1, സ്ഥിരം ushort* src2, size_t step2,
                            uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_cmp16s(സ്ഥിരം short* src1, size_t step1, സ്ഥിരം short* src2, size_t step2,
                             uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_cmp32f(സ്ഥിരം ദശാംശം* src1, size_t step1, സ്ഥിരം ദശാംശം* src2, size_t step2,
                             uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2, dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
ചെയ്യുക {                                                  \
    എങ്കിൽ (!CV_IPP_CHECK_COND)                           \
        തിരികെ 0;                                     \
    ദശാംശം fscale = (ദശാംശം)scale;                      \
    എങ്കിൽ (std::fabs(fscale - 1) > FLT_EPSILON)          \
        തിരികെ 0;                                     \
    എങ്കിൽ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        തിരികെ 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    തിരികെ 0;                                         \
} ആകുമ്പോൾ(0)

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_mul8u(സ്ഥിരം uchar *src1, size_t step1, സ്ഥിരം uchar *src2, size_t step2,
                            uchar *dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ഇരട്ട scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2,dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}
വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_mul16u(സ്ഥിരം ushort *src1, size_t step1, സ്ഥിരം ushort *src2, size_t step2,
                            ushort *dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ഇരട്ട scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2,dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_mul16s(സ്ഥിരം short *src1, size_t step1, സ്ഥിരം short *src2, size_t step2,
                            short *dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ഇരട്ട scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2,dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height), 0);
}

വരിയിൽ പൂർണ്ണസംഖ്യ arithm_ipp_mul32f(സ്ഥിരം ദശാംശം *src1, size_t step1, സ്ഥിരം ദശാംശം *src2, size_t step2,
                            ദശാംശം *dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ഇരട്ട scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (പൂർണ്ണസംഖ്യ)step1, src2, (പൂർണ്ണസംഖ്യ)step2,dst, (പൂർണ്ണസംഖ്യ)step, ippiSize(width, height));
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
    എങ്കിൽ (__CV_EXPAND(fun(__VA_ARGS__)))  \
        തിരികെ;                         \
}

#endif // ARITHM_USE_IPP


#എങ്കിൽ !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
