// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {

ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*HasNonZeroFunc)(ಧ್ರುವ ಶೂನ್ಯ*, size_t);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

HasNonZeroFunc getHasNonZeroFunc(ಪೂರ್ಣಾಙ್ಕ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#undef SIMD_ONLY
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
#define SIMD_ONLY(ಅಭಿವ್ಯಕ್ತಿ) ಅಭಿವ್ಯಕ್ತಿ
#ಅನ್ಯಥಾ
#define SIMD_ONLY(ಅಭಿವ್ಯಕ್ತಿ)
#endif

#undef DEFINE_HASNONZERO_FUNC
#define DEFINE_HASNONZERO_FUNC(funcname, suffix, T, VT, cmp_op, scalar_nz_op) \
ಸ್ಥಿರ ತಾರ್ಕಿಕ funcname( ಧ್ರುವ ಶೂನ್ಯ* _src, size_t len ) \
{ \
    ಧ್ರುವ T* src = (ಧ್ರುವ T*)_src; \
    size_t i = 0; \
    SIMD_ONLY( \
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    ಪ್ರತಿ (i = 0; i + vlanes*8 <= len; i += vlanes*8) \
    { \
        VT x0 = vx_load(src + i); \
        VT x1 = vx_load(src + i + vlanes); \
        VT x2 = vx_load(src + i + vlanes*2); \
        VT x3 = vx_load(src + i + vlanes*3); \
        VT x4 = vx_load(src + i + vlanes*4); \
        VT x5 = vx_load(src + i + vlanes*5); \
        VT x6 = vx_load(src + i + vlanes*6); \
        VT x7 = vx_load(src + i + vlanes*7); \
        x0 = v_or(x0, x1); \
        x2 = v_or(x2, x3); \
        x4 = v_or(x4, x5); \
        x6 = v_or(x6, x7); \
        x0 = v_or(x0, x2); \
        x4 = v_or(x4, x6); \
        x0 = v_or(x0, x4); \
        x0 = cmp_op(x0, v_zero); \
        ಯದಿ (v_check_any(x0)) \
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; \
    } \
    ಪ್ರತಿ (; i < len; i += vlanes) \
    { \
        ಯದಿ (i + vlanes > len) { \
            ಯದಿ (i == 0) \
                ಭಙ್ಗ; \
            i = len - vlanes; \
        } \
        VT x0 = vx_load(src + i); \
        x0 = cmp_op(x0, v_zero); \
        ಯದಿ (v_check_any(x0)) \
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; \
    } \
    v_cleanup();) \
    ಪ್ರತಿ( ; i < len; i++ ) \
    { \
        T x = src[i]; \
        ಯದಿ (scalar_nz_op(x) != 0) \
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; \
    } \
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; \
}

#undef CHECK_NZ_INT
#define CHECK_NZ_INT(x) ((x) != 0)
#undef CHECK_NZ_FP
#define CHECK_NZ_FP(x) (((x)<<1) != 0)
#undef CHECK_NZ_FP16
#define CHECK_NZ_FP16(x) (((x)&0x7fff) != 0)
#undef VEC_CMP_EQ_Z_FP16
#define VEC_CMP_EQ_Z_FP16(x, z) v_ne(v_add_wrap(x, x), z)
#undef VEC_CMP_EQ_Z_FP
#define VEC_CMP_EQ_Z_FP(x, z) v_ne(v_add(x, x), z)

DEFINE_HASNONZERO_FUNC(hasNonZero8u, u8, uchar, v_uint8, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero16u, u16, ushort, v_uint16, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero32s, s32, ಪೂರ್ಣಾಙ್ಕ, v_int32, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero64s, s64, int64, v_int64, v_ne, CHECK_NZ_INT)

DEFINE_HASNONZERO_FUNC(hasNonZero32f, s32, ಪೂರ್ಣಾಙ್ಕ, v_int32, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero64f, s64, int64, v_int64, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero16f, u16, ushort, v_uint16, VEC_CMP_EQ_Z_FP16, CHECK_NZ_FP16)

HasNonZeroFunc getHasNonZeroFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ HasNonZeroFunc hasNonZeroTab[CV_DEPTH_MAX] =
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

    ಪ್ರತ್ಯಾಗಮ hasNonZeroTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
