// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#ఒకవేళ ARITHM_USE_IPP

నామాకాశ cv { నామాకాశ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#నిర్వచించు ARITHM_IPP_BIN(fun, ...)                        \
చేయి {                                                    \
    ఒకవేళ (!CV_IPP_CHECK_COND)                             \
        తిరిగి 0;                                       \
    ఒకవేళ (ఎత్తు == 1)                                    \
        step1 = step2 = అడుగు = వెడల్పు * పరిమాణం(dst[0]);  \
    ఒకవేళ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_కూడు(CV_IMPL_IPP);                       \
        తిరిగి 1;                                       \
    }                                                   \
    సమితి_Ipp_Error_స్థితి();                                \
    తిరిగి 0;                                           \
} అయితే(0)

//=======================================
// Addition
//=======================================

అంతఃవరుస పూర్ణాంకం arithm_ipp_add8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                            uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippi_కూడు_8u_C1_RSfs, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_add16u(స్థిరరాశి ushort* src1, size_t step1, స్థిరరాశి ushort* src2, size_t step2,
                             ushort* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippi_కూడు_16u_C1_RSfs, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_add16s(స్థిరరాశి పొట్టి* src1, size_t step1, స్థిరరాశి పొట్టి* src2, size_t step2,
                             పొట్టి* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippi_కూడు_16s_C1_RSfs, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_add32f(స్థిరరాశి దశాంశం* src1, size_t step1, స్థిరరాశి దశాంశం* src2, size_t step2,
                             దశాంశం* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippi_కూడు_32f_C1_R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

#నిర్వచించు arithm_ipp_add8s(...)  0
#నిర్వచించు arithm_ipp_add32s(...) 0
#నిర్వచించు arithm_ipp_add64f(...) 0

//=======================================
//వ్యవకలనం
//=======================================

అంతఃవరుస పూర్ణాంకం arithm_ipp_sub8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                            uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (పూర్ణాంకం)step2, src1, (పూర్ణాంకం)step1, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_sub16u(స్థిరరాశి ushort* src1, size_t step1, స్థిరరాశి ushort* src2, size_t step2,
                             ushort* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (పూర్ణాంకం)step2, src1, (పూర్ణాంకం)step1, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_sub16s(స్థిరరాశి పొట్టి* src1, size_t step1, స్థిరరాశి పొట్టి* src2, size_t step2,
                            పొట్టి* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (పూర్ణాంకం)step2, src1, (పూర్ణాంకం)step1, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_sub32f(స్థిరరాశి దశాంశం* src1, size_t step1, స్థిరరాశి దశాంశం* src2, size_t step2,
                            దశాంశం* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (పూర్ణాంకం)step2, src1, (పూర్ణాంకం)step1, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

#నిర్వచించు arithm_ipp_sub8s(...)  0
#నిర్వచించు arithm_ipp_sub32s(...) 0
#నిర్వచించు arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#నిర్వచించు ARITHM_IPP_MIN_MAX(fun, type)                            \
చేయి {                                                             \
    ఒకవేళ (!CV_IPP_CHECK_COND)                                      \
        తిరిగి 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    ఒకవేళ (ఎత్తు == 1)                                             \
        step1 = step2 = అడుగు = వెడల్పు * పరిమాణం(dst[0]);           \
    పూర్ణాంకం i = 0;                                                   \
    కొరకు(; i < ఎత్తు; i++)                                       \
    {                                                            \
        ఒకవేళ (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, వెడల్పు))    \
            ఆపు;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + అడుగు);                          \
    }                                                            \
    ఒకవేళ (i == ఎత్తు)                                             \
    {                                                            \
        CV_IMPL_కూడు(CV_IMPL_IPP);                                \
        తిరిగి 1;                                                \
    }                                                            \
    సమితి_Ipp_Error_స్థితి();                                         \
    తిరిగి 0;                                                    \
} అయితే(0)

//=======================================
// Max
//=======================================

అంతఃవరుస పూర్ణాంకం arithm_ipp_max8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                           uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_max16u(స్థిరరాశి ushort* src1, size_t step1, స్థిరరాశి ushort* src2, size_t step2,
                             ushort* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_max32f(స్థిరరాశి దశాంశం* src1, size_t step1, స్థిరరాశి దశాంశం* src2, size_t step2,
                             దశాంశం* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, దశాంశం);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_max64f(స్థిరరాశి ద్విగుణ* src1, size_t step1, స్థిరరాశి ద్విగుణ* src2, size_t step2,
                             ద్విగుణ* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, ద్విగుణ);
}

#నిర్వచించు arithm_ipp_max8s(...)  0
#నిర్వచించు arithm_ipp_max16s(...) 0
#నిర్వచించు arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

అంతఃవరుస పూర్ణాంకం arithm_ipp_min8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                            uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_min16u(స్థిరరాశి ushort* src1, size_t step1, స్థిరరాశి ushort* src2, size_t step2,
                            ushort* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_min32f(స్థిరరాశి దశాంశం* src1, size_t step1, స్థిరరాశి దశాంశం* src2,size_t step2,
                             దశాంశం* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, దశాంశం);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_min64f(స్థిరరాశి ద్విగుణ* src1, size_t step1, స్థిరరాశి ద్విగుణ* src2, size_t step2,
                             ద్విగుణ* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, ద్విగుణ);
}

#నిర్వచించు arithm_ipp_min8s(...)  0
#నిర్వచించు arithm_ipp_min16s(...) 0
#నిర్వచించు arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

అంతఃవరుస పూర్ణాంకం arithm_ipp_absdiff8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                                uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_absdiff16u(స్థిరరాశి ushort* src1, size_t step1, స్థిరరాశి ushort* src2, size_t step2,
                                ushort* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_absdiff32f(స్థిరరాశి దశాంశం* src1, size_t step1, స్థిరరాశి దశాంశం* src2, size_t step2,
                                దశాంశం* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}
#నిర్వచించు arithm_ipp_absdiff8s(...)  0
#నిర్వచించు arithm_ipp_absdiff16s(...) 0
#నిర్వచించు arithm_ipp_absdiff32s(...) 0
#నిర్వచించు arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

అంతఃవరుస పూర్ణాంకం arithm_ipp_and8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                            uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_or8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                           uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_xor8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                            uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_not8u(స్థిరరాశి uchar* src1, size_t step1, uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు)
{
    ఒకవేళ (!CV_IPP_CHECK_COND)
        తిరిగి 0;
    ఒకవేళ (ఎత్తు == 1)
        step1 = అడుగు = వెడల్పు * పరిమాణం(dst[0]);
    ఒకవేళ (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (పూర్ణాంకం)step1, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు)))
    {
        CV_IMPL_కూడు(CV_IMPL_IPP);
        తిరిగి 1;
    }
    సమితి_Ipp_Error_స్థితి();
    తిరిగి 0;
}

//=======================================
// Compare
//=======================================

#నిర్వచించు ARITHM_IPP_CMP(fun, ...)                          \
చేయి {                                                      \
    ఒకవేళ (!CV_IPP_CHECK_COND)                               \
        తిరిగి 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    ఒకవేళ (op < 0)                                           \
        తిరిగి 0;                                         \
    ఒకవేళ (ఎత్తు == 1)                                      \
        step1 = step2 = అడుగు = వెడల్పు * పరిమాణం(dst[0]);    \
    ఒకవేళ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_కూడు(CV_IMPL_IPP);                         \
        తిరిగి 1;                                         \
    }                                                     \
    సమితి_Ipp_Error_స్థితి();                                  \
    తిరిగి 0;                                             \
} అయితే(0)

అంతఃవరుస IppCmpOp arithm_ipp_convert_cmp(పూర్ణాంకం cmpop)
{
    ఎంపిక(cmpop)
    {
        స్థితి CMP_EQ: తిరిగి ippCmpEq;
        స్థితి CMP_GT: తిరిగి ippCmpGreater;
        స్థితి CMP_GE: తిరిగి ippCmpGreaterEq;
        స్థితి CMP_LT: తిరిగి ippCmpLess;
        స్థితి CMP_LE: తిరిగి ippCmpLessEq;
        అప్రమేయ:     తిరిగి (IppCmpOp)-1;
    }
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_cmp8u(స్థిరరాశి uchar* src1, size_t step1, స్థిరరాశి uchar* src2, size_t step2,
                            uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_cmp16u(స్థిరరాశి ushort* src1, size_t step1, స్థిరరాశి ushort* src2, size_t step2,
                            uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_cmp16s(స్థిరరాశి పొట్టి* src1, size_t step1, స్థిరరాశి పొట్టి* src2, size_t step2,
                             uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_cmp32f(స్థిరరాశి దశాంశం* src1, size_t step1, స్థిరరాశి దశాంశం* src2, size_t step2,
                             uchar* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2, dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

#నిర్వచించు arithm_ipp_cmp8s(...)  0
#నిర్వచించు arithm_ipp_cmp32s(...) 0
#నిర్వచించు arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#నిర్వచించు ARITHM_IPP_MUL(fun, ...)                      \
చేయి {                                                  \
    ఒకవేళ (!CV_IPP_CHECK_COND)                           \
        తిరిగి 0;                                     \
    దశాంశం fscale = (దశాంశం)scale;                      \
    ఒకవేళ (std::fabs(fscale - 1) > FLT_EPSILON)          \
        తిరిగి 0;                                     \
    ఒకవేళ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_కూడు(CV_IMPL_IPP);                     \
        తిరిగి 1;                                     \
    }                                                 \
    సమితి_Ipp_Error_స్థితి();                              \
    తిరిగి 0;                                         \
} అయితే(0)

అంతఃవరుస పూర్ణాంకం arithm_ipp_mul8u(స్థిరరాశి uchar *src1, size_t step1, స్థిరరాశి uchar *src2, size_t step2,
                            uchar *dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2,dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}
అంతఃవరుస పూర్ణాంకం arithm_ipp_mul16u(స్థిరరాశి ushort *src1, size_t step1, స్థిరరాశి ushort *src2, size_t step2,
                            ushort *dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2,dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_mul16s(స్థిరరాశి పొట్టి *src1, size_t step1, స్థిరరాశి పొట్టి *src2, size_t step2,
                            పొట్టి *dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2,dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు), 0);
}

అంతఃవరుస పూర్ణాంకం arithm_ipp_mul32f(స్థిరరాశి దశాంశం *src1, size_t step1, స్థిరరాశి దశాంశం *src2, size_t step2,
                            దశాంశం *dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (పూర్ణాంకం)step1, src2, (పూర్ణాంకం)step2,dst, (పూర్ణాంకం)అడుగు, ippi_కొలత(వెడల్పు, ఎత్తు));
}

#నిర్వచించు arithm_ipp_mul8s(...)  0
#నిర్వచించు arithm_ipp_mul32s(...) 0
#నిర్వచించు arithm_ipp_mul64f(...) 0

//=======================================
// Div
//=======================================

#నిర్వచించు arithm_ipp_div8u(...)  0
#నిర్వచించు arithm_ipp_div8s(...)  0
#నిర్వచించు arithm_ipp_div16u(...) 0
#నిర్వచించు arithm_ipp_div16s(...) 0
#నిర్వచించు arithm_ipp_div32s(...) 0
#నిర్వచించు arithm_ipp_div32f(...) 0
#నిర్వచించు arithm_ipp_div64f(...) 0

//=======================================
// AddWeighted
//=======================================

#నిర్వచించు arithm_ipp_కూడు_Weighted8u(...)  0
#నిర్వచించు arithm_ipp_కూడు_Weighted8s(...)  0
#నిర్వచించు arithm_ipp_కూడు_Weighted16u(...) 0
#నిర్వచించు arithm_ipp_కూడు_Weighted16s(...) 0
#నిర్వచించు arithm_ipp_కూడు_Weighted32s(...) 0
#నిర్వచించు arithm_ipp_కూడు_Weighted32f(...) 0
#నిర్వచించు arithm_ipp_కూడు_Weighted64f(...) 0

//=======================================
// Reciprocial
//=======================================

#నిర్వచించు arithm_ipp_recip8u(...)  0
#నిర్వచించు arithm_ipp_recip8s(...)  0
#నిర్వచించు arithm_ipp_recip16u(...) 0
#నిర్వచించు arithm_ipp_recip16s(...) 0
#నిర్వచించు arithm_ipp_recip32s(...) 0
#నిర్వచించు arithm_ipp_recip32f(...) 0
#నిర్వచించు arithm_ipp_recip64f(...) 0

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

#నిర్వచించు ARITHM_CALL_IPP(fun, ...)       \
{                                       \
    ఒకవేళ (__CV_EXPAND(fun(__VA_ARGS__)))  \
        తిరిగి;                         \
}

#అయితే_ముగింపు // ARITHM_USE_IPP


#ఒకవేళ !ARITHM_USE_IPP
#నిర్వచించు ARITHM_CALL_IPP(...)
#అయితే_ముగింపు
