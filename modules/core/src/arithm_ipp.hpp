// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#ਜੇ ARITHM_USE_IPP

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

//=======================================
// Arithmetic and logical operations
// +, -, *, /, &, |, ^, ~, abs ...
//=======================================

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARITHM_IPP_BIN(fun, ...)                        \
ਕਰੋ {                                                    \
    ਜੇ (!CV_IPP_CHECK_COND)                             \
        ਵਾਪਸ 0;                                       \
    ਜੇ (ਉਚਾਈ == 1)                                    \
        step1 = step2 = ਕਦਮ = ਚੌੜਾਈ * ਆਕਾਰ(dst[0]);  \
    ਜੇ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__))   \
    {                                                   \
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);                       \
        ਵਾਪਸ 1;                                       \
    }                                                   \
    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();                                \
    ਵਾਪਸ 0;                                           \
} ਜਦੋਂ(0)

//=======================================
// Addition
//=======================================

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_add8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                            uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippi_ਜੋੜੋ_8u_C1_RSfs, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_add16u(ਅਚਲ ushort* src1, size_t step1, ਅਚਲ ushort* src2, size_t step2,
                             ushort* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippi_ਜੋੜੋ_16u_C1_RSfs, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_add16s(ਅਚਲ ਛੋਟਾ* src1, size_t step1, ਅਚਲ ਛੋਟਾ* src2, size_t step2,
                             ਛੋਟਾ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippi_ਜੋੜੋ_16s_C1_RSfs, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_add32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t step1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t step2,
                             ਦਸ਼ਮਲਵ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippi_ਜੋੜੋ_32f_C1_R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_add8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_add32s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_add64f(...) 0

//=======================================
//ਘਟਾਓ
//=======================================

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_sub8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                            uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiSub_8u_C1RSfs, src2, (ਪੂਰਨ)step2, src1, (ਪੂਰਨ)step1, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_sub16u(ਅਚਲ ushort* src1, size_t step1, ਅਚਲ ushort* src2, size_t step2,
                             ushort* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiSub_16u_C1RSfs, src2, (ਪੂਰਨ)step2, src1, (ਪੂਰਨ)step1, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_sub16s(ਅਚਲ ਛੋਟਾ* src1, size_t step1, ਅਚਲ ਛੋਟਾ* src2, size_t step2,
                            ਛੋਟਾ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiSub_16s_C1RSfs, src2, (ਪੂਰਨ)step2, src1, (ਪੂਰਨ)step1, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_sub32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t step1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t step2,
                            ਦਸ਼ਮਲਵ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiSub_32f_C1R, src2, (ਪੂਰਨ)step2, src1, (ਪੂਰਨ)step1, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_sub8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_sub32s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_sub64f(...) 0

///////////////////////////////////////////////////////////////////////////////////////////////////

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARITHM_IPP_MIN_MAX(fun, type)                            \
ਕਰੋ {                                                             \
    ਜੇ (!CV_IPP_CHECK_COND)                                      \
        ਵਾਪਸ 0;                                                \
    type* s1 = (type*)src1;                                      \
    type* s2 = (type*)src2;                                      \
    type* d  = dst;                                              \
    ਜੇ (ਉਚਾਈ == 1)                                             \
        step1 = step2 = ਕਦਮ = ਚੌੜਾਈ * ਆਕਾਰ(dst[0]);           \
    ਪੂਰਨ i = 0;                                                   \
    ਲਈ(; i < ਉਚਾਈ; i++)                                       \
    {                                                            \
        ਜੇ (0 > CV_INSTRUMENT_FUN_IPP(fun, s1, s2, d, ਚੌੜਾਈ))    \
            ਰੋਕੋ;                                               \
        s1 = (type*)((uchar*)s1 + step1);                        \
        s2 = (type*)((uchar*)s2 + step2);                        \
        d  = (type*)((uchar*)d + ਕਦਮ);                          \
    }                                                            \
    ਜੇ (i == ਉਚਾਈ)                                             \
    {                                                            \
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);                                \
        ਵਾਪਸ 1;                                                \
    }                                                            \
    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();                                         \
    ਵਾਪਸ 0;                                                    \
} ਜਦੋਂ(0)

//=======================================
// Max
//=======================================

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_max8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                           uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_8u, uchar);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_max16u(ਅਚਲ ushort* src1, size_t step1, ਅਚਲ ushort* src2, size_t step2,
                             ushort* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_16u, ushort);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_max32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t step1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t step2,
                             ਦਸ਼ਮਲਵ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_32f, ਦਸ਼ਮਲਵ);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_max64f(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t step1, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t step2,
                             ਦੁੱਗਣਾ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_MIN_MAX(ippsMaxEvery_64f, ਦੁੱਗਣਾ);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_max8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_max16s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_max32s(...) 0

//=======================================
// Min
//=======================================

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_min8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                            uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_8u, uchar);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_min16u(ਅਚਲ ushort* src1, size_t step1, ਅਚਲ ushort* src2, size_t step2,
                            ushort* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_16u, ushort);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_min32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t step1, ਅਚਲ ਦਸ਼ਮਲਵ* src2,size_t step2,
                             ਦਸ਼ਮਲਵ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_32f, ਦਸ਼ਮਲਵ);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_min64f(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t step1, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t step2,
                             ਦੁੱਗਣਾ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_MIN_MAX(ippsMinEvery_64f, ਦੁੱਗਣਾ);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_min8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_min16s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_min32s(...) 0

//=======================================
// AbsDiff
//=======================================

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_absdiff8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                                uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiAbsDiff_8u_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_absdiff16u(ਅਚਲ ushort* src1, size_t step1, ਅਚਲ ushort* src2, size_t step2,
                                ushort* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiAbsDiff_16u_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_absdiff32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t step1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t step2,
                                ਦਸ਼ਮਲਵ* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiAbsDiff_32f_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_absdiff8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_absdiff16s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_absdiff32s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_absdiff64f(...) 0

//=======================================
// Logical
//=======================================

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_and8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                            uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiAnd_8u_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_or8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                           uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiOr_8u_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_xor8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                            uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ARITHM_IPP_BIN(ippiXor_8u_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_not8u(ਅਚਲ uchar* src1, size_t step1, uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ਜੇ (!CV_IPP_CHECK_COND)
        ਵਾਪਸ 0;
    ਜੇ (ਉਚਾਈ == 1)
        step1 = ਕਦਮ = ਚੌੜਾਈ * ਆਕਾਰ(dst[0]);
    ਜੇ (0 <= CV_INSTRUMENT_FUN_IPP(ippiNot_8u_C1R, src1, (ਪੂਰਨ)step1, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ)))
    {
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
        ਵਾਪਸ 1;
    }
    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
    ਵਾਪਸ 0;
}

//=======================================
// Compare
//=======================================

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARITHM_IPP_CMP(fun, ...)                          \
ਕਰੋ {                                                      \
    ਜੇ (!CV_IPP_CHECK_COND)                               \
        ਵਾਪਸ 0;                                         \
    IppCmpOp op = arithm_ipp_convert_cmp(cmpop);          \
    ਜੇ (op < 0)                                           \
        ਵਾਪਸ 0;                                         \
    ਜੇ (ਉਚਾਈ == 1)                                      \
        step1 = step2 = ਕਦਮ = ਚੌੜਾਈ * ਆਕਾਰ(dst[0]);    \
    ਜੇ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__, op)) \
    {                                                     \
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);                         \
        ਵਾਪਸ 1;                                         \
    }                                                     \
    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();                                  \
    ਵਾਪਸ 0;                                             \
} ਜਦੋਂ(0)

ਸਤਰਅੰਦਰ IppCmpOp arithm_ipp_convert_cmp(ਪੂਰਨ cmpop)
{
    ਚੋਣ(cmpop)
    {
        ਹਾਲਤ CMP_EQ: ਵਾਪਸ ippCmpEq;
        ਹਾਲਤ CMP_GT: ਵਾਪਸ ippCmpGreater;
        ਹਾਲਤ CMP_GE: ਵਾਪਸ ippCmpGreaterEq;
        ਹਾਲਤ CMP_LT: ਵਾਪਸ ippCmpLess;
        ਹਾਲਤ CMP_LE: ਵਾਪਸ ippCmpLessEq;
        ਮੂਲ:     ਵਾਪਸ (IppCmpOp)-1;
    }
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_cmp8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                            uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_8u_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_cmp16u(ਅਚਲ ushort* src1, size_t step1, ਅਚਲ ushort* src2, size_t step2,
                            uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16u_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_cmp16s(ਅਚਲ ਛੋਟਾ* src1, size_t step1, ਅਚਲ ਛੋਟਾ* src2, size_t step2,
                             uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_16s_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_cmp32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t step1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t step2,
                             uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ cmpop)
{
    ARITHM_IPP_CMP(ippiCompare_32f_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2, dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_cmp8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_cmp32s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_cmp64f(...) 0

//=======================================
// Multiply
//=======================================

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARITHM_IPP_MUL(fun, ...)                      \
ਕਰੋ {                                                  \
    ਜੇ (!CV_IPP_CHECK_COND)                           \
        ਵਾਪਸ 0;                                     \
    ਦਸ਼ਮਲਵ fscale = (ਦਸ਼ਮਲਵ)scale;                      \
    ਜੇ (std::fabs(fscale - 1) > FLT_EPSILON)          \
        ਵਾਪਸ 0;                                     \
    ਜੇ (0 <= CV_INSTRUMENT_FUN_IPP(fun, __VA_ARGS__)) \
    {                                                 \
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);                     \
        ਵਾਪਸ 1;                                     \
    }                                                 \
    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();                              \
    ਵਾਪਸ 0;                                         \
} ਜਦੋਂ(0)

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_mul8u(ਅਚਲ uchar *src1, size_t step1, ਅਚਲ uchar *src2, size_t step2,
                            uchar *dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale)
{
    ARITHM_IPP_MUL(ippiMul_8u_C1RSfs, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2,dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}
ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_mul16u(ਅਚਲ ushort *src1, size_t step1, ਅਚਲ ushort *src2, size_t step2,
                            ushort *dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale)
{
    ARITHM_IPP_MUL(ippiMul_16u_C1RSfs, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2,dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_mul16s(ਅਚਲ ਛੋਟਾ *src1, size_t step1, ਅਚਲ ਛੋਟਾ *src2, size_t step2,
                            ਛੋਟਾ *dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale)
{
    ARITHM_IPP_MUL(ippiMul_16s_C1RSfs, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2,dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ), 0);
}

ਸਤਰਅੰਦਰ ਪੂਰਨ arithm_ipp_mul32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1, size_t step1, ਅਚਲ ਦਸ਼ਮਲਵ *src2, size_t step2,
                            ਦਸ਼ਮਲਵ *dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale)
{
    ARITHM_IPP_MUL(ippiMul_32f_C1R, src1, (ਪੂਰਨ)step1, src2, (ਪੂਰਨ)step2,dst, (ਪੂਰਨ)ਕਦਮ, ippi_ਮਾਪ(ਚੌੜਾਈ, ਉਚਾਈ));
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_mul8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_mul32s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_mul64f(...) 0

//=======================================
// Div
//=======================================

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_div8u(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_div8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_div16u(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_div16s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_div32s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_div32f(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_div64f(...) 0

//=======================================
// AddWeighted
//=======================================

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_ਜੋੜੋ_Weighted8u(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_ਜੋੜੋ_Weighted8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_ਜੋੜੋ_Weighted16u(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_ਜੋੜੋ_Weighted16s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_ਜੋੜੋ_Weighted32s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_ਜੋੜੋ_Weighted32f(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_ਜੋੜੋ_Weighted64f(...) 0

//=======================================
// Reciprocial
//=======================================

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_recip8u(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_recip8s(...)  0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_recip16u(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_recip16s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_recip32s(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_recip32f(...) 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arithm_ipp_recip64f(...) 0

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARITHM_CALL_IPP(fun, ...)       \
{                                       \
    ਜੇ (__CV_EXPAND(fun(__VA_ARGS__)))  \
        ਵਾਪਸ;                         \
}

#ਜੇ_ਅੰਤ // ARITHM_USE_IPP


#ਜੇ !ARITHM_USE_IPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARITHM_CALL_IPP(...)
#ਜੇ_ਅੰਤ
