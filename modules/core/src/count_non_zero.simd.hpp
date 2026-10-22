// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"

namespace cv {

typedef ចំនួនគត់ (*CountNonZeroFunc)(const void*, ចំនួនគត់);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

CountNonZeroFunc getCountNonZeroTab(ចំនួនគត់ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template<typename T>
static ចំនួនគត់ countNonZero_(const T* src, ចំនួនគត់ len )
{
    ចំនួនគត់ nz = 0;
    សម្រាប់( ចំនួនគត់ i = 0; i < len; i++ )
        nz += src[i] != 0;
    ត្រឡប់ nz;
}

#undef SIMD_ONLY
#ប្រសិនបើ (CV_SIMD || CV_SIMD_SCALABLE)
#define SIMD_ONLY(expr) expr
#បើមិនដូច្នេះ
#define SIMD_ONLY(expr)
#endif

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#undef CV_COUNTNONZERO_AVX512
#ប្រសិនបើ defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#define CV_COUNTNONZERO_AVX512 1
#បើមិនដូច្នេះ
#define CV_COUNTNONZERO_AVX512 0
#endif

#ប្រសិនបើ CV_COUNTNONZERO_AVX512
static anta_hstha ចំនួនគត់ cnz_popcount32(unsigned x)
{
#ប្រសិនបើ defined(__GNUC__) || defined(__clang__)
    ត្រឡប់ __builtin_popcount(x);
#បើមិនដូច្នេះ
    ចំនួនគត់ c = 0;
    សម្រាប់ (; x; x &= x - 1)
        ++c;
    ត្រឡប់ c;
#endif
}

static anta_hstha ចំនួនគត់ cnz_popcount64(uint64 x)
{
#ប្រសិនបើ defined(__GNUC__) || defined(__clang__)
    ត្រឡប់ (ចំនួនគត់)__builtin_popcountll((unsigned long long)x);
#បើមិនដូច្នេះ
    ចំនួនគត់ c = 0;
    សម្រាប់ (; x; x &= x - 1)
        ++c;
    ត្រឡប់ c;
#endif
}

static anta_hstha ចំនួនគត់ cnz_lane_pop(ចំនួនគត់ m) { ត្រឡប់ cnz_popcount32((unsigned)m); }
static anta_hstha ចំនួនគត់ cnz_lane_pop(int64 m) { ត្រឡប់ cnz_popcount64((uint64)m); }
#endif // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#undef CNZ_SIMD_BODY
#ប្រសិនបើ CV_COUNTNONZERO_AVX512
#define CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) \
    const ចំនួនគត់ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    ចំនួនគត់ nzeros = 0; \
    សម្រាប់ (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_pop(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
        nzeros += cnz_lane_pop(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i + vlanes), v_zero)))); \
    } \
    សម្រាប់ (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_pop(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#បើមិនដូច្នេះ
#define CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) \
    const ចំនួនគត់ vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    សម្រាប់ (i = 0; i <= len - vlanes*8; i += vlanes*8) \
    { \
        VT x0 = vx_load(src + i); \
        VT x1 = vx_load(src + i + vlanes); \
        VT x2 = vx_load(src + i + vlanes*2); \
        VT x3 = vx_load(src + i + vlanes*3); \
        VT x4 = vx_load(src + i + vlanes*4); \
        VT x5 = vx_load(src + i + vlanes*5); \
        VT x6 = vx_load(src + i + vlanes*6); \
        VT x7 = vx_load(src + i + vlanes*7); \
        x0 = cmp_op(x0, v_zero); \
        x1 = cmp_op(x1, v_zero); \
        x2 = cmp_op(x2, v_zero); \
        x3 = cmp_op(x3, v_zero); \
        x4 = cmp_op(x4, v_zero); \
        x5 = cmp_op(x5, v_zero); \
        x6 = cmp_op(x6, v_zero); \
        x7 = cmp_op(x7, v_zero); \
        x0 = add_op(x0, x1); \
        x2 = add_op(x2, x3); \
        x4 = add_op(x4, x5); \
        x6 = add_op(x6, x7); \
        x0 = add_op(x0, x2); \
        x4 = add_op(x4, x6); \
        x0 = add_op(add_op(x0, x4), v_8); \
        update_sum(v_sum0, v_sum1, x0); \
    } \
    សម្រាប់ (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_load(src + i); \
        x0 = add_op(cmp_op(x0, v_zero), v_1); \
        update_sum(v_sum0, v_sum1, x0); \
    } \
    nz += (ចំនួនគត់)v_reduce_sum(v_add(v_sum0, v_sum1)); \
    v_cleanup();
#endif

#undef DEFINE_NONZERO_FUNC
#define DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, add_op, update_sum, scalar_cmp_op) \
static ចំនួនគត់ funcname( const void* src_ptr, ចំនួនគត់ len ) \
{ \
    const T* src = static_cast<const T*>(src_ptr); \
    ចំនួនគត់ i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) ) \
    សម្រាប់( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    ត្រឡប់ nz; \
}

#undef CHECK_NZ_INT
#define CHECK_NZ_INT(x) ((x) != 0)
#undef CHECK_NZ_FP
#define CHECK_NZ_FP(x) ((x)*2 != 0)
// 16-bit float: mask the sign bit so -0.0 (0x8000) reads as zero.
#undef CHECK_NZ_FP16
#define CHECK_NZ_FP16(x) (((x) & 0x7fff) != 0)
#undef VEC_CMP_EQ_Z_FP16
#define VEC_CMP_EQ_Z_FP16(x, z) v_eq(v_add_wrap(x, x), z)
#undef VEC_CMP_EQ_Z_FP
#define VEC_CMP_EQ_Z_FP(x, z) v_eq(v_add(x, x), z)

#undef UPDATE_SUM_U8
#define UPDATE_SUM_U8(v_sum0, v_sum1, x0) \
    v_uint16 w0 = v_expand_low(x0); \
    v_uint16 w1 = v_expand_high(x0); \
    v_sum0 = v_add(v_sum0, v_expand_low(w0)); \
    v_sum1 = v_add(v_sum1, v_expand_high(w0)); \
    v_sum0 = v_add(v_sum0, v_expand_low(w1)); \
    v_sum1 = v_add(v_sum1, v_expand_high(w1))

#undef UPDATE_SUM_U16
#define UPDATE_SUM_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, v_expand_low(x0)); \
    v_sum1 = v_add(v_sum1, v_expand_high(x0))

#undef UPDATE_SUM_S32
#define UPDATE_SUM_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, x0)

DEFINE_NONZERO_FUNC(countNonZero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_add_wrap, UPDATE_SUM_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(countNonZero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_add_wrap, UPDATE_SUM_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(countNonZero32s, s32, s32, s32, ចំនួនគត់, v_int32, v_int32, v_eq, v_add, UPDATE_SUM_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(countNonZero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_add, UPDATE_SUM_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(countNonZero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_add_wrap, UPDATE_SUM_U16, CHECK_NZ_FP16)

#undef DEFINE_NONZERO_FUNC_NOSIMD
#define DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
static ចំនួនគត់ funcname(const void* src, ចំនួនគត់ len) \
{ \
    ត្រឡប់ countNonZero_(static_cast<const T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(countNonZero64s, int64)

#ប្រសិនបើ CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
static ចំនួនគត់ countNonZero64f( const void* src_ptr, ចំនួនគត់ len )
{
    const dviguu_na* src = static_cast<const dviguu_na*>(src_ptr);
    ចំនួនគត់ i = 0, nz = 0;
    const ចំនួនគត់ vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    ចំនួនគត់ nzeros = 0;
    សម្រាប់ (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_pop(v_signmask(v_eq(vx_load(src + i), v_zero)));
        nzeros += cnz_lane_pop(v_signmask(v_eq(vx_load(src + i + vlanes), v_zero)));
    }
    សម្រាប់ (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_pop(v_signmask(v_eq(vx_load(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    សម្រាប់ (; i < len; i++)
        nz += src[i] != 0;
    ត្រឡប់ nz;
}
#បើមិនដូច្នេះ
DEFINE_NONZERO_FUNC_NOSIMD(countNonZero64f, dviguu_na)
#endif

CountNonZeroFunc getCountNonZeroTab(ចំនួនគត់ depth)
{
    static CountNonZeroFunc countNonZeroTab[CV_DEPTH_MAX] =
    {
        countNonZero8u,
        countNonZero8u,
        countNonZero16u,
        countNonZero16u,
        countNonZero32s,
        countNonZero32f,
        countNonZero64f,
        countNonZero16f,
        countNonZero16f, // for bf16 it's the same code as for f16
        countNonZero8u,
        countNonZero64s,
        countNonZero64s,
        countNonZero32s,
        0
    };

    ត្រឡប់ countNonZeroTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
