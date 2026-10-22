// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"

فضاینام cv {

تعریفنوع صحیح (*شمار_Non_Zero_Func)(ثابت تهی*, صحیح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

شمار_Non_Zero_Func get_شمار_Non_Zero_Tab(صحیح depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template<typename T>
ایستا صحیح شمار_Non_Zero_2(ثابت T* src, صحیح len )
{
    صحیح nz = 0;
    برای( صحیح i = 0; i < len; i++ )
        nz += src[i] != 0;
    بازگشت nz;
}

#undef SIMD_ONLY
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
#define SIMD_ONLY(عبارت) عبارت
#وگرنه
#define SIMD_ONLY(عبارت)
#endif

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#undef CV_COUNTNONZERO_AVX512
#اگر defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#define CV_COUNTNONZERO_AVX512 1
#وگرنه
#define CV_COUNTNONZERO_AVX512 0
#endif

#اگر CV_COUNTNONZERO_AVX512
ایستا درونخطی صحیح cnz_popcount32(unsigned x)
{
#اگر defined(__GNUC__) || defined(__clang__)
    بازگشت __builtin_popcount(x);
#وگرنه
    صحیح c = 0;
    برای (; x; x &= x - 1)
        ++c;
    بازگشت c;
#endif
}

ایستا درونخطی صحیح cnz_popcount64(uint64 x)
{
#اگر defined(__GNUC__) || defined(__clang__)
    بازگشت (صحیح)__builtin_popcountll((unsigned long long)x);
#وگرنه
    صحیح c = 0;
    برای (; x; x &= x - 1)
        ++c;
    بازگشت c;
#endif
}

ایستا درونخطی صحیح cnz_lane_برداشت(صحیح m) { بازگشت cnz_popcount32((unsigned)m); }
ایستا درونخطی صحیح cnz_lane_برداشت(int64 m) { بازگشت cnz_popcount64((uint64)m); }
#endif // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#undef CNZ_SIMD_BODY
#اگر CV_COUNTNONZERO_AVX512
#define CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) \
    ثابت صحیح vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    صحیح nzeros = 0; \
    برای (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_برداشت(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
        nzeros += cnz_lane_برداشت(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i + vlanes), v_zero)))); \
    } \
    برای (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_برداشت(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#وگرنه
#define CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) \
    ثابت صحیح vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    برای (i = 0; i <= len - vlanes*8; i += vlanes*8) \
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
    برای (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_load(src + i); \
        x0 = add_op(cmp_op(x0, v_zero), v_1); \
        update_sum(v_sum0, v_sum1, x0); \
    } \
    nz += (صحیح)v_reduce_sum(v_add(v_sum0, v_sum1)); \
    v_cleanup();
#endif

#undef DEFINE_NONZERO_FUNC
#define DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, add_op, update_sum, scalar_cmp_op) \
ایستا صحیح funcname( ثابت تهی* src_ptr, صحیح len ) \
{ \
    ثابت T* src = static_cast<ثابت T*>(src_ptr); \
    صحیح i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) ) \
    برای( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    بازگشت nz; \
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
    v_uint16 w0 = v_expand_کم(x0); \
    v_uint16 w1 = v_expand_بالا(x0); \
    v_sum0 = v_add(v_sum0, v_expand_کم(w0)); \
    v_sum1 = v_add(v_sum1, v_expand_بالا(w0)); \
    v_sum0 = v_add(v_sum0, v_expand_کم(w1)); \
    v_sum1 = v_add(v_sum1, v_expand_بالا(w1))

#undef UPDATE_SUM_U16
#define UPDATE_SUM_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, v_expand_کم(x0)); \
    v_sum1 = v_add(v_sum1, v_expand_بالا(x0))

#undef UPDATE_SUM_S32
#define UPDATE_SUM_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, x0)

DEFINE_NONZERO_FUNC(شمار_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_add_wrap, UPDATE_SUM_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(شمار_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_add_wrap, UPDATE_SUM_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(شمار_Non_Zero32s, s32, s32, s32, صحیح, v_int32, v_int32, v_eq, v_add, UPDATE_SUM_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(شمار_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_add, UPDATE_SUM_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(شمار_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_add_wrap, UPDATE_SUM_U16, CHECK_NZ_FP16)

#undef DEFINE_NONZERO_FUNC_NOSIMD
#define DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
ایستا صحیح funcname(ثابت تهی* src, صحیح len) \
{ \
    بازگشت شمار_Non_Zero_2(static_cast<ثابت T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(شمار_Non_Zero64s, int64)

#اگر CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
ایستا صحیح شمار_Non_Zero64f( ثابت تهی* src_ptr, صحیح len )
{
    ثابت دوگانه* src = static_cast<ثابت دوگانه*>(src_ptr);
    صحیح i = 0, nz = 0;
    ثابت صحیح vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    صحیح nzeros = 0;
    برای (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_برداشت(v_signmask(v_eq(vx_load(src + i), v_zero)));
        nzeros += cnz_lane_برداشت(v_signmask(v_eq(vx_load(src + i + vlanes), v_zero)));
    }
    برای (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_برداشت(v_signmask(v_eq(vx_load(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    برای (; i < len; i++)
        nz += src[i] != 0;
    بازگشت nz;
}
#وگرنه
DEFINE_NONZERO_FUNC_NOSIMD(شمار_Non_Zero64f, دوگانه)
#endif

شمار_Non_Zero_Func get_شمار_Non_Zero_Tab(صحیح depth)
{
    ایستا شمار_Non_Zero_Func شمار_Non_Zero_Tab[CV_DEPTH_MAX] =
    {
        شمار_Non_Zero8u,
        شمار_Non_Zero8u,
        شمار_Non_Zero16u,
        شمار_Non_Zero16u,
        شمار_Non_Zero32s,
        شمار_Non_Zero32f,
        شمار_Non_Zero64f,
        شمار_Non_Zero16f,
        شمار_Non_Zero16f, // for bf16 it's the same code as for f16
        شمار_Non_Zero8u,
        شمار_Non_Zero64s,
        شمار_Non_Zero64s,
        شمار_Non_Zero32s,
        0
    };

    بازگشت شمار_Non_Zero_Tab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
