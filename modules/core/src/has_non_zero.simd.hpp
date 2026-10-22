// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*HasNonZeroFunc)(ਅਚਲ ਖਾਲੀ*, size_t);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

HasNonZeroFunc getHasNonZeroFunc(ਪੂਰਨ ਡੂੰਘਾਈ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SIMD_ONLY
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SIMD_ONLY(ਵਿਅੰਜਕ) ਵਿਅੰਜਕ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SIMD_ONLY(ਵਿਅੰਜਕ)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_HASNONZERO_FUNC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_HASNONZERO_FUNC(funcname, suffix, T, VT, cmp_op, scalar_nz_op) \
ਸਥਿਰ ਤਾਰਕਿਕ funcname( ਅਚਲ ਖਾਲੀ* _src, size_t len ) \
{ \
    ਅਚਲ T* src = (ਅਚਲ T*)_src; \
    size_t i = 0; \
    SIMD_ONLY( \
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    ਲਈ (i = 0; i + vlanes*8 <= len; i += vlanes*8) \
    { \
        VT x0 = vx_ਲੱਦੋ(src + i); \
        VT x1 = vx_ਲੱਦੋ(src + i + vlanes); \
        VT x2 = vx_ਲੱਦੋ(src + i + vlanes*2); \
        VT x3 = vx_ਲੱਦੋ(src + i + vlanes*3); \
        VT x4 = vx_ਲੱਦੋ(src + i + vlanes*4); \
        VT x5 = vx_ਲੱਦੋ(src + i + vlanes*5); \
        VT x6 = vx_ਲੱਦੋ(src + i + vlanes*6); \
        VT x7 = vx_ਲੱਦੋ(src + i + vlanes*7); \
        x0 = v_or(x0, x1); \
        x2 = v_or(x2, x3); \
        x4 = v_or(x4, x5); \
        x6 = v_or(x6, x7); \
        x0 = v_or(x0, x2); \
        x4 = v_or(x4, x6); \
        x0 = v_or(x0, x4); \
        x0 = cmp_op(x0, v_zero); \
        ਜੇ (v_check_any(x0)) \
            ਵਾਪਸ ਸੱਚ; \
    } \
    ਲਈ (; i < len; i += vlanes) \
    { \
        ਜੇ (i + vlanes > len) { \
            ਜੇ (i == 0) \
                ਰੋਕੋ; \
            i = len - vlanes; \
        } \
        VT x0 = vx_ਲੱਦੋ(src + i); \
        x0 = cmp_op(x0, v_zero); \
        ਜੇ (v_check_any(x0)) \
            ਵਾਪਸ ਸੱਚ; \
    } \
    v_cleanup();) \
    ਲਈ( ; i < len; i++ ) \
    { \
        T x = src[i]; \
        ਜੇ (scalar_nz_op(x) != 0) \
            ਵਾਪਸ ਸੱਚ; \
    } \
    ਵਾਪਸ ਝੂਠ; \
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CHECK_NZ_INT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_NZ_INT(x) ((x) != 0)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CHECK_NZ_FP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_NZ_FP(x) (((x)<<1) != 0)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CHECK_NZ_FP16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_NZ_FP16(x) (((x)&0x7fff) != 0)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ VEC_CMP_EQ_Z_FP16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VEC_CMP_EQ_Z_FP16(x, z) v_ne(v_ਜੋੜੋ_wrap(x, x), z)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ VEC_CMP_EQ_Z_FP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VEC_CMP_EQ_Z_FP(x, z) v_ne(v_ਜੋੜੋ(x, x), z)

DEFINE_HASNONZERO_FUNC(hasNonZero8u, u8, uchar, v_uint8, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero16u, u16, ushort, v_uint16, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero32s, s32, ਪੂਰਨ, v_int32, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero64s, s64, int64, v_int64, v_ne, CHECK_NZ_INT)

DEFINE_HASNONZERO_FUNC(hasNonZero32f, s32, ਪੂਰਨ, v_int32, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero64f, s64, int64, v_int64, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero16f, u16, ushort, v_uint16, VEC_CMP_EQ_Z_FP16, CHECK_NZ_FP16)

HasNonZeroFunc getHasNonZeroFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ HasNonZeroFunc hasNonZeroTab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        hasNonZero8u,
        hasNonZero8u,
        hasNonZero16u,
        hasNonZero16u,
        hasNonZero32s,
        hasNonZero32f,
        hasNonZero64f,
        hasNonZero16f,
        hasNonZero16f,
        hasNonZero8u,
        hasNonZero64s,
        hasNonZero64s,
        hasNonZero32s,
        0
    };

    ਵਾਪਸ hasNonZeroTab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਅੰਤ

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace
