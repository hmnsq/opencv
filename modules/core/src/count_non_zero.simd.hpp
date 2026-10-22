// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*ಎಣಿಕೆ_Non_Zero_Func)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, ಪೂರ್ಣಾಂಕ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಎಣಿಕೆ_Non_Zero_Func get_ಎಣಿಕೆ_Non_Zero_Tab(ಪೂರ್ಣಾಂಕ ಆಳ);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_Non_Zero_2(ಸ್ಥಿರಾಂಕ T* src, ಪೂರ್ಣಾಂಕ len )
{
    ಪೂರ್ಣಾಂಕ nz = 0;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ )
        nz += src[i] != 0;
    ಹಿಂತಿರುಗಿ nz;
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SIMD_ONLY
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
#ವ್ಯಾಖ್ಯಾನಿಸು SIMD_ONLY(ಅಭಿವ್ಯಕ್ತಿ) ಅಭಿವ್ಯಕ್ತಿ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು SIMD_ONLY(ಅಭಿವ್ಯಕ್ತಿ)
#ಆದರೆ_ಅಂತ್ಯ

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_COUNTNONZERO_AVX512
#ಆದರೆ defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_COUNTNONZERO_AVX512 1
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_COUNTNONZERO_AVX512 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ CV_COUNTNONZERO_AVX512
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cnz_popcount32(ಚಿಹ್ನೆರಹಿತ x)
{
#ಆದರೆ defined(__GNUC__) || defined(__clang__)
    ಹಿಂತಿರುಗಿ __builtin_popcount(x);
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ c = 0;
    ಗಾಗಿ (; x; x &= x - 1)
        ++c;
    ಹಿಂತಿರುಗಿ c;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cnz_popcount64(uint64 x)
{
#ಆದರೆ defined(__GNUC__) || defined(__clang__)
    ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)__builtin_popcountll((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)x);
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ c = 0;
    ಗಾಗಿ (; x; x &= x - 1)
        ++c;
    ಹಿಂತಿರುಗಿ c;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cnz_lane_ತೆಗೆ(ಪೂರ್ಣಾಂಕ m) { ಹಿಂತಿರುಗಿ cnz_popcount32((ಚಿಹ್ನೆರಹಿತ)m); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cnz_lane_ತೆಗೆ(int64 m) { ಹಿಂತಿರುಗಿ cnz_popcount64((uint64)m); }
#ಆದರೆ_ಅಂತ್ಯ // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CNZ_SIMD_BODY
#ಆದರೆ CV_COUNTNONZERO_AVX512
#ವ್ಯಾಖ್ಯಾನಿಸು CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, ಕೂಡಿಸು_op, update_ಮೊತ್ತ) \
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    ಪೂರ್ಣಾಂಕ nzeros = 0; \
    ಗಾಗಿ (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_ತೆಗೆ(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ತುಂಬು(src + i), v_zero)))); \
        nzeros += cnz_lane_ತೆಗೆ(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ತುಂಬು(src + i + vlanes), v_zero)))); \
    } \
    ಗಾಗಿ (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_ತೆಗೆ(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ತುಂಬು(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, ಕೂಡಿಸು_op, update_ಮೊತ್ತ) \
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    ಗಾಗಿ (i = 0; i <= len - vlanes*8; i += vlanes*8) \
    { \
        VT x0 = vx_ತುಂಬು(src + i); \
        VT x1 = vx_ತುಂಬು(src + i + vlanes); \
        VT x2 = vx_ತುಂಬು(src + i + vlanes*2); \
        VT x3 = vx_ತುಂಬು(src + i + vlanes*3); \
        VT x4 = vx_ತುಂಬು(src + i + vlanes*4); \
        VT x5 = vx_ತುಂಬು(src + i + vlanes*5); \
        VT x6 = vx_ತುಂಬು(src + i + vlanes*6); \
        VT x7 = vx_ತುಂಬು(src + i + vlanes*7); \
        x0 = cmp_op(x0, v_zero); \
        x1 = cmp_op(x1, v_zero); \
        x2 = cmp_op(x2, v_zero); \
        x3 = cmp_op(x3, v_zero); \
        x4 = cmp_op(x4, v_zero); \
        x5 = cmp_op(x5, v_zero); \
        x6 = cmp_op(x6, v_zero); \
        x7 = cmp_op(x7, v_zero); \
        x0 = ಕೂಡಿಸು_op(x0, x1); \
        x2 = ಕೂಡಿಸು_op(x2, x3); \
        x4 = ಕೂಡಿಸು_op(x4, x5); \
        x6 = ಕೂಡಿಸು_op(x6, x7); \
        x0 = ಕೂಡಿಸು_op(x0, x2); \
        x4 = ಕೂಡಿಸು_op(x4, x6); \
        x0 = ಕೂಡಿಸು_op(ಕೂಡಿಸು_op(x0, x4), v_8); \
        update_ಮೊತ್ತ(v_sum0, v_sum1, x0); \
    } \
    ಗಾಗಿ (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_ತುಂಬು(src + i); \
        x0 = ಕೂಡಿಸು_op(cmp_op(x0, v_zero), v_1); \
        update_ಮೊತ್ತ(v_sum0, v_sum1, x0); \
    } \
    nz += (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_sum0, v_sum1)); \
    v_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_NONZERO_FUNC
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, ಕೂಡಿಸು_op, update_ಮೊತ್ತ, scalar_cmp_op) \
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ funcname( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src_ptr, ಪೂರ್ಣಾಂಕ len ) \
{ \
    ಸ್ಥಿರಾಂಕ T* src = static_cast<ಸ್ಥಿರಾಂಕ T*>(src_ptr); \
    ಪೂರ್ಣಾಂಕ i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, ಕೂಡಿಸು_op, update_ಮೊತ್ತ) ) \
    ಗಾಗಿ( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    ಹಿಂತಿರುಗಿ nz; \
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CHECK_NZ_INT
#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_NZ_INT(x) ((x) != 0)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CHECK_NZ_FP
#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_NZ_FP(x) ((x)*2 != 0)
// 16-bit float: mask the sign bit so -0.0 (0x8000) reads as zero.
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CHECK_NZ_FP16
#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_NZ_FP16(x) (((x) & 0x7fff) != 0)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು VEC_CMP_EQ_Z_FP16
#ವ್ಯಾಖ್ಯಾನಿಸು VEC_CMP_EQ_Z_FP16(x, z) v_eq(v_ಕೂಡಿಸು_wrap(x, x), z)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು VEC_CMP_EQ_Z_FP
#ವ್ಯಾಖ್ಯಾನಿಸು VEC_CMP_EQ_Z_FP(x, z) v_eq(v_ಕೂಡಿಸು(x, x), z)

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು UPDATE_ಮೊತ್ತ_U8
#ವ್ಯಾಖ್ಯಾನಿಸು UPDATE_ಮೊತ್ತ_U8(v_sum0, v_sum1, x0) \
    v_uint16 w0 = v_expand_ಕೆಳಗಿನ(x0); \
    v_uint16 w1 = v_expand_ಮೇಲಿನ(x0); \
    v_sum0 = v_ಕೂಡಿಸು(v_sum0, v_expand_ಕೆಳಗಿನ(w0)); \
    v_sum1 = v_ಕೂಡಿಸು(v_sum1, v_expand_ಮೇಲಿನ(w0)); \
    v_sum0 = v_ಕೂಡಿಸು(v_sum0, v_expand_ಕೆಳಗಿನ(w1)); \
    v_sum1 = v_ಕೂಡಿಸು(v_sum1, v_expand_ಮೇಲಿನ(w1))

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು UPDATE_ಮೊತ್ತ_U16
#ವ್ಯಾಖ್ಯಾನಿಸು UPDATE_ಮೊತ್ತ_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_ಕೂಡಿಸು(v_sum0, v_expand_ಕೆಳಗಿನ(x0)); \
    v_sum1 = v_ಕೂಡಿಸು(v_sum1, v_expand_ಮೇಲಿನ(x0))

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು UPDATE_ಮೊತ್ತ_S32
#ವ್ಯಾಖ್ಯಾನಿಸು UPDATE_ಮೊತ್ತ_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_ಕೂಡಿಸು(v_sum0, x0)

DEFINE_NONZERO_FUNC(ಎಣಿಕೆ_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_ಕೂಡಿಸು_wrap, UPDATE_ಮೊತ್ತ_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ಎಣಿಕೆ_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_ಕೂಡಿಸು_wrap, UPDATE_ಮೊತ್ತ_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ಎಣಿಕೆ_Non_Zero32s, s32, s32, s32, ಪೂರ್ಣಾಂಕ, v_int32, v_int32, v_eq, v_ಕೂಡಿಸು, UPDATE_ಮೊತ್ತ_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ಎಣಿಕೆ_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_ಕೂಡಿಸು, UPDATE_ಮೊತ್ತ_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(ಎಣಿಕೆ_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_ಕೂಡಿಸು_wrap, UPDATE_ಮೊತ್ತ_U16, CHECK_NZ_FP16)

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_NONZERO_FUNC_NOSIMD
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ funcname(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಪೂರ್ಣಾಂಕ len) \
{ \
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_Non_Zero_2(static_cast<ಸ್ಥಿರಾಂಕ T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(ಎಣಿಕೆ_Non_Zero64s, int64)

#ಆದರೆ CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_Non_Zero64f( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src_ptr, ಪೂರ್ಣಾಂಕ len )
{
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src = static_cast<ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*>(src_ptr);
    ಪೂರ್ಣಾಂಕ i = 0, nz = 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    ಪೂರ್ಣಾಂಕ nzeros = 0;
    ಗಾಗಿ (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_ತೆಗೆ(v_signmask(v_eq(vx_ತುಂಬು(src + i), v_zero)));
        nzeros += cnz_lane_ತೆಗೆ(v_signmask(v_eq(vx_ತುಂಬು(src + i + vlanes), v_zero)));
    }
    ಗಾಗಿ (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_ತೆಗೆ(v_signmask(v_eq(vx_ತುಂಬು(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    ಗಾಗಿ (; i < len; i++)
        nz += src[i] != 0;
    ಹಿಂತಿರುಗಿ nz;
}
#ಇಲ್ಲದಿದ್ದರೆ
DEFINE_NONZERO_FUNC_NOSIMD(ಎಣಿಕೆ_Non_Zero64f, ದ್ವಿಗುಣ)
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ_Non_Zero_Func get_ಎಣಿಕೆ_Non_Zero_Tab(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ಎಣಿಕೆ_Non_Zero_Func ಎಣಿಕೆ_Non_Zero_Tab[CV_ಆಳ_MAX] =
    {
        ಎಣಿಕೆ_Non_Zero8u,
        ಎಣಿಕೆ_Non_Zero8u,
        ಎಣಿಕೆ_Non_Zero16u,
        ಎಣಿಕೆ_Non_Zero16u,
        ಎಣಿಕೆ_Non_Zero32s,
        ಎಣಿಕೆ_Non_Zero32f,
        ಎಣಿಕೆ_Non_Zero64f,
        ಎಣಿಕೆ_Non_Zero16f,
        ಎಣಿಕೆ_Non_Zero16f, // for bf16 it's the same code as for f16
        ಎಣಿಕೆ_Non_Zero8u,
        ಎಣಿಕೆ_Non_Zero64s,
        ಎಣಿಕೆ_Non_Zero64s,
        ಎಣಿಕೆ_Non_Zero32s,
        0
    };

    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_Non_Zero_Tab[ಆಳ];
}

#ಆದರೆ_ಅಂತ್ಯ

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace
