// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*ਗਿਣਤੀ_Non_Zero_Func)(ਅਚਲ ਖਾਲੀ*, ਪੂਰਨ);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਗਿਣਤੀ_Non_Zero_Func get_ਗਿਣਤੀ_Non_Zero_Tab(ਪੂਰਨ ਡੂੰਘਾਈ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_Non_Zero_2(ਅਚਲ T* src, ਪੂਰਨ len )
{
    ਪੂਰਨ nz = 0;
    ਲਈ( ਪੂਰਨ i = 0; i < len; i++ )
        nz += src[i] != 0;
    ਵਾਪਸ nz;
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SIMD_ONLY
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SIMD_ONLY(ਵਿਅੰਜਕ) ਵਿਅੰਜਕ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SIMD_ONLY(ਵਿਅੰਜਕ)
#ਜੇ_ਅੰਤ

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_COUNTNONZERO_AVX512
#ਜੇ defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_COUNTNONZERO_AVX512 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_COUNTNONZERO_AVX512 0
#ਜੇ_ਅੰਤ

#ਜੇ CV_COUNTNONZERO_AVX512
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cnz_popcount32(ਬਿਨਾਨਿਸ਼ਾਨੀ x)
{
#ਜੇ defined(__GNUC__) || defined(__clang__)
    ਵਾਪਸ __builtin_popcount(x);
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ c = 0;
    ਲਈ (; x; x &= x - 1)
        ++c;
    ਵਾਪਸ c;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cnz_popcount64(uint64 x)
{
#ਜੇ defined(__GNUC__) || defined(__clang__)
    ਵਾਪਸ (ਪੂਰਨ)__builtin_popcountll((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)x);
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ c = 0;
    ਲਈ (; x; x &= x - 1)
        ++c;
    ਵਾਪਸ c;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cnz_lane_ਕੱਢੋ(ਪੂਰਨ m) { ਵਾਪਸ cnz_popcount32((ਬਿਨਾਨਿਸ਼ਾਨੀ)m); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cnz_lane_ਕੱਢੋ(int64 m) { ਵਾਪਸ cnz_popcount64((uint64)m); }
#ਜੇ_ਅੰਤ // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CNZ_SIMD_BODY
#ਜੇ CV_COUNTNONZERO_AVX512
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, ਜੋੜੋ_op, update_ਜੋੜ) \
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    ਪੂਰਨ nzeros = 0; \
    ਲਈ (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_ਕੱਢੋ(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ਲੱਦੋ(src + i), v_zero)))); \
        nzeros += cnz_lane_ਕੱਢੋ(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ਲੱਦੋ(src + i + vlanes), v_zero)))); \
    } \
    ਲਈ (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_ਕੱਢੋ(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ਲੱਦੋ(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, ਜੋੜੋ_op, update_ਜੋੜ) \
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    ਲਈ (i = 0; i <= len - vlanes*8; i += vlanes*8) \
    { \
        VT x0 = vx_ਲੱਦੋ(src + i); \
        VT x1 = vx_ਲੱਦੋ(src + i + vlanes); \
        VT x2 = vx_ਲੱਦੋ(src + i + vlanes*2); \
        VT x3 = vx_ਲੱਦੋ(src + i + vlanes*3); \
        VT x4 = vx_ਲੱਦੋ(src + i + vlanes*4); \
        VT x5 = vx_ਲੱਦੋ(src + i + vlanes*5); \
        VT x6 = vx_ਲੱਦੋ(src + i + vlanes*6); \
        VT x7 = vx_ਲੱਦੋ(src + i + vlanes*7); \
        x0 = cmp_op(x0, v_zero); \
        x1 = cmp_op(x1, v_zero); \
        x2 = cmp_op(x2, v_zero); \
        x3 = cmp_op(x3, v_zero); \
        x4 = cmp_op(x4, v_zero); \
        x5 = cmp_op(x5, v_zero); \
        x6 = cmp_op(x6, v_zero); \
        x7 = cmp_op(x7, v_zero); \
        x0 = ਜੋੜੋ_op(x0, x1); \
        x2 = ਜੋੜੋ_op(x2, x3); \
        x4 = ਜੋੜੋ_op(x4, x5); \
        x6 = ਜੋੜੋ_op(x6, x7); \
        x0 = ਜੋੜੋ_op(x0, x2); \
        x4 = ਜੋੜੋ_op(x4, x6); \
        x0 = ਜੋੜੋ_op(ਜੋੜੋ_op(x0, x4), v_8); \
        update_ਜੋੜ(v_sum0, v_sum1, x0); \
    } \
    ਲਈ (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_ਲੱਦੋ(src + i); \
        x0 = ਜੋੜੋ_op(cmp_op(x0, v_zero), v_1); \
        update_ਜੋੜ(v_sum0, v_sum1, x0); \
    } \
    nz += (ਪੂਰਨ)v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_sum0, v_sum1)); \
    v_cleanup();
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_NONZERO_FUNC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, ਜੋੜੋ_op, update_ਜੋੜ, scalar_cmp_op) \
ਸਥਿਰ ਪੂਰਨ funcname( ਅਚਲ ਖਾਲੀ* src_ptr, ਪੂਰਨ len ) \
{ \
    ਅਚਲ T* src = static_cast<ਅਚਲ T*>(src_ptr); \
    ਪੂਰਨ i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, ਜੋੜੋ_op, update_ਜੋੜ) ) \
    ਲਈ( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    ਵਾਪਸ nz; \
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CHECK_NZ_INT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_NZ_INT(x) ((x) != 0)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CHECK_NZ_FP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_NZ_FP(x) ((x)*2 != 0)
// 16-bit float: mask the sign bit so -0.0 (0x8000) reads as zero.
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CHECK_NZ_FP16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_NZ_FP16(x) (((x) & 0x7fff) != 0)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ VEC_CMP_EQ_Z_FP16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VEC_CMP_EQ_Z_FP16(x, z) v_eq(v_ਜੋੜੋ_wrap(x, x), z)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ VEC_CMP_EQ_Z_FP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VEC_CMP_EQ_Z_FP(x, z) v_eq(v_ਜੋੜੋ(x, x), z)

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ UPDATE_ਜੋੜ_U8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UPDATE_ਜੋੜ_U8(v_sum0, v_sum1, x0) \
    v_uint16 w0 = v_expand_ਨੀਵਾਂ(x0); \
    v_uint16 w1 = v_expand_ਉੱਚਾ(x0); \
    v_sum0 = v_ਜੋੜੋ(v_sum0, v_expand_ਨੀਵਾਂ(w0)); \
    v_sum1 = v_ਜੋੜੋ(v_sum1, v_expand_ਉੱਚਾ(w0)); \
    v_sum0 = v_ਜੋੜੋ(v_sum0, v_expand_ਨੀਵਾਂ(w1)); \
    v_sum1 = v_ਜੋੜੋ(v_sum1, v_expand_ਉੱਚਾ(w1))

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ UPDATE_ਜੋੜ_U16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UPDATE_ਜੋੜ_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_ਜੋੜੋ(v_sum0, v_expand_ਨੀਵਾਂ(x0)); \
    v_sum1 = v_ਜੋੜੋ(v_sum1, v_expand_ਉੱਚਾ(x0))

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ UPDATE_ਜੋੜ_S32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UPDATE_ਜੋੜ_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_ਜੋੜੋ(v_sum0, x0)

DEFINE_NONZERO_FUNC(ਗਿਣਤੀ_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_ਜੋੜੋ_wrap, UPDATE_ਜੋੜ_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ਗਿਣਤੀ_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_ਜੋੜੋ_wrap, UPDATE_ਜੋੜ_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ਗਿਣਤੀ_Non_Zero32s, s32, s32, s32, ਪੂਰਨ, v_int32, v_int32, v_eq, v_ਜੋੜੋ, UPDATE_ਜੋੜ_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ਗਿਣਤੀ_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_ਜੋੜੋ, UPDATE_ਜੋੜ_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(ਗਿਣਤੀ_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_ਜੋੜੋ_wrap, UPDATE_ਜੋੜ_U16, CHECK_NZ_FP16)

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_NONZERO_FUNC_NOSIMD
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
ਸਥਿਰ ਪੂਰਨ funcname(ਅਚਲ ਖਾਲੀ* src, ਪੂਰਨ len) \
{ \
    ਵਾਪਸ ਗਿਣਤੀ_Non_Zero_2(static_cast<ਅਚਲ T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(ਗਿਣਤੀ_Non_Zero64s, int64)

#ਜੇ CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
ਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_Non_Zero64f( ਅਚਲ ਖਾਲੀ* src_ptr, ਪੂਰਨ len )
{
    ਅਚਲ ਦੁੱਗਣਾ* src = static_cast<ਅਚਲ ਦੁੱਗਣਾ*>(src_ptr);
    ਪੂਰਨ i = 0, nz = 0;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    ਪੂਰਨ nzeros = 0;
    ਲਈ (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_ਕੱਢੋ(v_signmask(v_eq(vx_ਲੱਦੋ(src + i), v_zero)));
        nzeros += cnz_lane_ਕੱਢੋ(v_signmask(v_eq(vx_ਲੱਦੋ(src + i + vlanes), v_zero)));
    }
    ਲਈ (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_ਕੱਢੋ(v_signmask(v_eq(vx_ਲੱਦੋ(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    ਲਈ (; i < len; i++)
        nz += src[i] != 0;
    ਵਾਪਸ nz;
}
#ਨਹੀਂਤਾਂ
DEFINE_NONZERO_FUNC_NOSIMD(ਗਿਣਤੀ_Non_Zero64f, ਦੁੱਗਣਾ)
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ_Non_Zero_Func get_ਗਿਣਤੀ_Non_Zero_Tab(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਗਿਣਤੀ_Non_Zero_Func ਗਿਣਤੀ_Non_Zero_Tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        ਗਿਣਤੀ_Non_Zero8u,
        ਗਿਣਤੀ_Non_Zero8u,
        ਗਿਣਤੀ_Non_Zero16u,
        ਗਿਣਤੀ_Non_Zero16u,
        ਗਿਣਤੀ_Non_Zero32s,
        ਗਿਣਤੀ_Non_Zero32f,
        ਗਿਣਤੀ_Non_Zero64f,
        ਗਿਣਤੀ_Non_Zero16f,
        ਗਿਣਤੀ_Non_Zero16f, // for bf16 it's the same code as for f16
        ਗਿਣਤੀ_Non_Zero8u,
        ਗਿਣਤੀ_Non_Zero64s,
        ਗਿਣਤੀ_Non_Zero64s,
        ਗਿਣਤੀ_Non_Zero32s,
        0
    };

    ਵਾਪਸ ਗਿਣਤੀ_Non_Zero_Tab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਅੰਤ

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace
