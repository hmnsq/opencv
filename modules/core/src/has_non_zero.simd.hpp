// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ತಾರ್ಕಿಕ (*HasNonZeroFunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

HasNonZeroFunc getHasNonZeroFunc(ಪೂರ್ಣಾಂಕ ಆಳ);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SIMD_ONLY
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
#ವ್ಯಾಖ್ಯಾನಿಸು SIMD_ONLY(ಅಭಿವ್ಯಕ್ತಿ) ಅಭಿವ್ಯಕ್ತಿ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು SIMD_ONLY(ಅಭಿವ್ಯಕ್ತಿ)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_HASNONZERO_FUNC
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_HASNONZERO_FUNC(funcname, suffix, T, VT, cmp_op, scalar_nz_op) \
ಸ್ಥಿರ ತಾರ್ಕಿಕ funcname( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src, size_t len ) \
{ \
    ಸ್ಥಿರಾಂಕ T* src = (ಸ್ಥಿರಾಂಕ T*)_src; \
    size_t i = 0; \
    SIMD_ONLY( \
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    ಗಾಗಿ (i = 0; i + vlanes*8 <= len; i += vlanes*8) \
    { \
        VT x0 = vx_ತುಂಬು(src + i); \
        VT x1 = vx_ತುಂಬು(src + i + vlanes); \
        VT x2 = vx_ತುಂಬು(src + i + vlanes*2); \
        VT x3 = vx_ತುಂಬು(src + i + vlanes*3); \
        VT x4 = vx_ತುಂಬು(src + i + vlanes*4); \
        VT x5 = vx_ತುಂಬು(src + i + vlanes*5); \
        VT x6 = vx_ತುಂಬು(src + i + vlanes*6); \
        VT x7 = vx_ತುಂಬು(src + i + vlanes*7); \
        x0 = v_or(x0, x1); \
        x2 = v_or(x2, x3); \
        x4 = v_or(x4, x5); \
        x6 = v_or(x6, x7); \
        x0 = v_or(x0, x2); \
        x4 = v_or(x4, x6); \
        x0 = v_or(x0, x4); \
        x0 = cmp_op(x0, v_zero); \
        ಆದರೆ (v_check_any(x0)) \
            ಹಿಂತಿರುಗಿ ನಿಜ; \
    } \
    ಗಾಗಿ (; i < len; i += vlanes) \
    { \
        ಆದರೆ (i + vlanes > len) { \
            ಆದರೆ (i == 0) \
                ನಿಲ್ಲಿಸು; \
            i = len - vlanes; \
        } \
        VT x0 = vx_ತುಂಬು(src + i); \
        x0 = cmp_op(x0, v_zero); \
        ಆದರೆ (v_check_any(x0)) \
            ಹಿಂತಿರುಗಿ ನಿಜ; \
    } \
    v_cleanup();) \
    ಗಾಗಿ( ; i < len; i++ ) \
    { \
        T x = src[i]; \
        ಆದರೆ (scalar_nz_op(x) != 0) \
            ಹಿಂತಿರುಗಿ ನಿಜ; \
    } \
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; \
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CHECK_NZ_INT
#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_NZ_INT(x) ((x) != 0)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CHECK_NZ_FP
#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_NZ_FP(x) (((x)<<1) != 0)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CHECK_NZ_FP16
#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_NZ_FP16(x) (((x)&0x7fff) != 0)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು VEC_CMP_EQ_Z_FP16
#ವ್ಯಾಖ್ಯಾನಿಸು VEC_CMP_EQ_Z_FP16(x, z) v_ne(v_ಕೂಡಿಸು_wrap(x, x), z)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು VEC_CMP_EQ_Z_FP
#ವ್ಯಾಖ್ಯಾನಿಸು VEC_CMP_EQ_Z_FP(x, z) v_ne(v_ಕೂಡಿಸು(x, x), z)

DEFINE_HASNONZERO_FUNC(hasNonZero8u, u8, uchar, v_uint8, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero16u, u16, ushort, v_uint16, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero32s, s32, ಪೂರ್ಣಾಂಕ, v_int32, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero64s, s64, int64, v_int64, v_ne, CHECK_NZ_INT)

DEFINE_HASNONZERO_FUNC(hasNonZero32f, s32, ಪೂರ್ಣಾಂಕ, v_int32, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero64f, s64, int64, v_int64, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero16f, u16, ushort, v_uint16, VEC_CMP_EQ_Z_FP16, CHECK_NZ_FP16)

HasNonZeroFunc getHasNonZeroFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ HasNonZeroFunc hasNonZeroTab[CV_ಆಳ_MAX] =
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

    ಹಿಂತಿರುಗಿ hasNonZeroTab[ಆಳ];
}

#ಆದರೆ_ಅಂತ್ಯ

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace
