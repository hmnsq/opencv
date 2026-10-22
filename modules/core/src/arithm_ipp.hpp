// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#ଯଦି ARITHM_USE_IPP

ନାମାକାଶ cv { ନାମାକାଶ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#define ARITHM_IPP_BIN(fun, ...)                        \
କର {                                                    \
    ଯଦି (!CV_IPP_CHECK_COND)                             \
        ଫେରନ୍ତୁ 0;                                       \
    ଯଦି (height == 1)                                    \
        step1 = step2 = step = width * ଆକାର(dst[0]);  \
    ଯଦି (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ADD(CV_IMPL_IPP);                       \
        ଫେରନ୍ତୁ 1;                                       \
    }                                                   \
    setIppErrorStatus();                                \
    ଫେରନ୍ତୁ 0;                                           \
} ଯେତେବେଳେ(0)

//=======================================
// Addition
//=======================================

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_add8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiAdd_8u_C1RSfs, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_add16u(ଅଚଳ ushort* src1, size_t step1, ଅଚଳ ushort* src2, size_t step2,
                             ushort* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiAdd_16u_C1RSfs, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_add16s(ଅଚଳ short* src1, size_t step1, ଅଚଳ short* src2, size_t step2,
                             short* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiAdd_16s_C1RSfs, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_add32f(ଅଚଳ ଦଶମିକ* src1, size_t step1, ଅଚଳ ଦଶମିକ* src2, size_t step2,
                             ଦଶମିକ* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiAdd_32f_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

#define arithm_ipp_add8s(...)  0
#define arithm_ipp_add32s(...) 0
#define arithm_ipp_add64f(...) 0

//=======================================
// Subtract
//=======================================

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_sub8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_sub16u(ଅଚଳ ushort* src1, size_t step1, ଅଚଳ ushort* src2, size_t step2,
                             ushort* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_sub16s(ଅଚଳ short* src1, size_t step1, ଅଚଳ short* src2, size_t step2,
                            short* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_sub32f(ଅଚଳ ଦଶମିକ* src1, size_t step1, ଅଚଳ ଦଶମିକ* src2, size_t step2,
                            ଦଶମିକ* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

#define arithm_ipp_sub8s(...)  0
#define arithm_ipp_sub32s(...) 0
#define arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#define ARITHM_IPP_MIN_MAX(fun, type)                            \
କର {                                                             \
    ଯଦି (!CV_IPP_CHECK_COND)                                      \
        ଫେରନ୍ତୁ 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    ଯଦି (height == 1)                                             \
        step1 = step2 = step = width * ଆକାର(dst[0]);           \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;                                                   \
    ପାଇଁ(; i < height; i++)                                       \
    {                                                            \
        ଯଦି (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width))    \
            ଭାଙ୍ଗ;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + step);                          \
    }                                                            \
    ଯଦି (i == height)                                             \
    {                                                            \
        CV_IMPL_ADD(CV_IMPL_IPP);                                \
        ଫେରନ୍ତୁ 1;                                                \
    }                                                            \
    setIppErrorStatus();                                         \
    ଫେରନ୍ତୁ 0;                                                    \
} ଯେତେବେଳେ(0)

//=======================================
// Max
//=======================================

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_max8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                           uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_max16u(ଅଚଳ ushort* src1, size_t step1, ଅଚଳ ushort* src2, size_t step2,
                             ushort* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_max32f(ଅଚଳ ଦଶମିକ* src1, size_t step1, ଅଚଳ ଦଶମିକ* src2, size_t step2,
                             ଦଶମିକ* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, ଦଶମିକ);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_max64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, size_t step1, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, size_t step2,
                             ଦ୍ୱିଗୁଣ* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, ଦ୍ୱିଗୁଣ);
}

#define arithm_ipp_max8s(...)  0
#define arithm_ipp_max16s(...) 0
#define arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_min8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_min16u(ଅଚଳ ushort* src1, size_t step1, ଅଚଳ ushort* src2, size_t step2,
                            ushort* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_min32f(ଅଚଳ ଦଶମିକ* src1, size_t step1, ଅଚଳ ଦଶମିକ* src2,size_t step2,
                             ଦଶମିକ* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, ଦଶମିକ);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_min64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, size_t step1, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, size_t step2,
                             ଦ୍ୱିଗୁଣ* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, ଦ୍ୱିଗୁଣ);
}

#define arithm_ipp_min8s(...)  0
#define arithm_ipp_min16s(...) 0
#define arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_absdiff8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                                uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_absdiff16u(ଅଚଳ ushort* src1, size_t step1, ଅଚଳ ushort* src2, size_t step2,
                                ushort* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_absdiff32f(ଅଚଳ ଦଶମିକ* src1, size_t step1, ଅଚଳ ଦଶମିକ* src2, size_t step2,
                                ଦଶମିକ* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}
#define arithm_ipp_absdiff8s(...)  0
#define arithm_ipp_absdiff16s(...) 0
#define arithm_ipp_absdiff32s(...) 0
#define arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_and8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_or8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                           uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_xor8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_not8u(ଅଚଳ uchar* src1, size_t step1, uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ଯଦି (!CV_IPP_CHECK_COND)
        ଫେରନ୍ତୁ 0;
    ଯଦି (height == 1)
        step1 = step = width * ଆକାର(dst[0]);
    ଯଦି (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height)))
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        ଫେରନ୍ତୁ 1;
    }
    setIppErrorStatus();
    ଫେରନ୍ତୁ 0;
}

//=======================================
// Compare
//=======================================

#define ARITHM_IPP_CMP(fun, ...)                          \
କର {                                                      \
    ଯଦି (!CV_IPP_CHECK_COND)                               \
        ଫେରନ୍ତୁ 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    ଯଦି (op < 0)                                           \
        ଫେରନ୍ତୁ 0;                                         \
    ଯଦି (height == 1)                                      \
        step1 = step2 = step = width * ଆକାର(dst[0]);    \
    ଯଦି (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ADD(CV_IMPL_IPP);                         \
        ଫେରନ୍ତୁ 1;                                         \
    }                                                     \
    setIppErrorStatus();                                  \
    ଫେରନ୍ତୁ 0;                                             \
} ଯେତେବେଳେ(0)

ଧାଡ଼ିଭିତରେ IppCmpOp arithm_ipp_convert_cmp(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop)
{
    ବାଛ(cmpop)
    {
        ସ୍ଥିତି CMP_EQ: ଫେରନ୍ତୁ ippCmpEq;
        ସ୍ଥିତି CMP_GT: ଫେରନ୍ତୁ ippCmpGreater;
        ସ୍ଥିତି CMP_GE: ଫେରନ୍ତୁ ippCmpGreaterEq;
        ସ୍ଥିତି CMP_LT: ଫେରନ୍ତୁ ippCmpLess;
        ସ୍ଥିତି CMP_LE: ଫେରନ୍ତୁ ippCmpLessEq;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:     ଫେରନ୍ତୁ (IppCmpOp)-1;
    }
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_cmp8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                            uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_cmp16u(ଅଚଳ ushort* src1, size_t step1, ଅଚଳ ushort* src2, size_t step2,
                            uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_cmp16s(ଅଚଳ short* src1, size_t step1, ଅଚଳ short* src2, size_t step2,
                             uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_cmp32f(ଅଚଳ ଦଶମିକ* src1, size_t step1, ଅଚଳ ଦଶମିକ* src2, size_t step2,
                             uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
}

#define arithm_ipp_cmp8s(...)  0
#define arithm_ipp_cmp32s(...) 0
#define arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#define ARITHM_IPP_MUL(fun, ...)                      \
କର {                                                  \
    ଯଦି (!CV_IPP_CHECK_COND)                           \
        ଫେରନ୍ତୁ 0;                                     \
    ଦଶମିକ fscale = (ଦଶମିକ)scale;                      \
    ଯଦି (std::fabs(fscale - 1) > FLT_EPSILON)          \
        ଫେରନ୍ତୁ 0;                                     \
    ଯଦି (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ADD(CV_IMPL_IPP);                     \
        ଫେରନ୍ତୁ 1;                                     \
    }                                                 \
    setIppErrorStatus();                              \
    ଫେରନ୍ତୁ 0;                                         \
} ଯେତେବେଳେ(0)

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_mul8u(ଅଚଳ uchar *src1, size_t step1, ଅଚଳ uchar *src2, size_t step2,
                            uchar *dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଦ୍ୱିଗୁଣ scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2,dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}
ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_mul16u(ଅଚଳ ushort *src1, size_t step1, ଅଚଳ ushort *src2, size_t step2,
                            ushort *dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଦ୍ୱିଗୁଣ scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2,dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_mul16s(ଅଚଳ short *src1, size_t step1, ଅଚଳ short *src2, size_t step2,
                            short *dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଦ୍ୱିଗୁଣ scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2,dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height), 0);
}

ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା arithm_ipp_mul32f(ଅଚଳ ଦଶମିକ *src1, size_t step1, ଅଚଳ ଦଶମିକ *src2, size_t step2,
                            ଦଶମିକ *dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଦ୍ୱିଗୁଣ scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step1, src2, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step2,dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step, ippiSize(width, height));
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
    ଯଦି (__CV_EXPAND(fun(__VA_ARGS__)))  \
        ଫେରନ୍ତୁ;                         \
}

#endif // ARITHM_USE_IPP


#ଯଦି !ARITHM_USE_IPP
#define ARITHM_CALL_IPP(...)
#endif
