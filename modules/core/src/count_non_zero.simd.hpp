// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"

நாமாகாஶ cv {

வகைவரை முழு (*மொத்தம்_Non_Zero_Func)(மாறிலி வெறுமை*, முழு);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

மொத்தம்_Non_Zero_Func get_மொத்தம்_Non_Zero_Tab(முழு depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template<typename T>
நிலையான முழு மொத்தம்_Non_Zero_2(மாறிலி T* src, முழு len )
{
    முழு nz = 0;
    க்கு( முழு i = 0; i < len; i++ )
        nz += src[i] != 0;
    திரும்பு nz;
}

#undef SIMD_ONLY
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
#define SIMD_ONLY(கோவை) கோவை
#இல்லையெனில்
#define SIMD_ONLY(கோவை)
#endif

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#undef CV_COUNTNONZERO_AVX512
#எனில் defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#define CV_COUNTNONZERO_AVX512 1
#இல்லையெனில்
#define CV_COUNTNONZERO_AVX512 0
#endif

#எனில் CV_COUNTNONZERO_AVX512
நிலையான உள்வரி முழு cnz_popcount32(unsigned x)
{
#எனில் defined(__GNUC__) || defined(__clang__)
    திரும்பு __builtin_popcount(x);
#இல்லையெனில்
    முழு c = 0;
    க்கு (; x; x &= x - 1)
        ++c;
    திரும்பு c;
#endif
}

நிலையான உள்வரி முழு cnz_popcount64(uint64 x)
{
#எனில் defined(__GNUC__) || defined(__clang__)
    திரும்பு (முழு)__builtin_popcountll((unsigned long long)x);
#இல்லையெனில்
    முழு c = 0;
    க்கு (; x; x &= x - 1)
        ++c;
    திரும்பு c;
#endif
}

நிலையான உள்வரி முழு cnz_lane_எடு(முழு m) { திரும்பு cnz_popcount32((unsigned)m); }
நிலையான உள்வரி முழு cnz_lane_எடு(int64 m) { திரும்பு cnz_popcount64((uint64)m); }
#endif // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#undef CNZ_SIMD_BODY
#எனில் CV_COUNTNONZERO_AVX512
#define CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) \
    மாறிலி முழு vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    முழு nzeros = 0; \
    க்கு (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_எடு(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
        nzeros += cnz_lane_எடு(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i + vlanes), v_zero)))); \
    } \
    க்கு (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_எடு(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#இல்லையெனில்
#define CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) \
    மாறிலி முழு vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    க்கு (i = 0; i <= len - vlanes*8; i += vlanes*8) \
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
    க்கு (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_load(src + i); \
        x0 = add_op(cmp_op(x0, v_zero), v_1); \
        update_sum(v_sum0, v_sum1, x0); \
    } \
    nz += (முழு)v_reduce_sum(v_add(v_sum0, v_sum1)); \
    v_cleanup();
#endif

#undef DEFINE_NONZERO_FUNC
#define DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, add_op, update_sum, scalar_cmp_op) \
நிலையான முழு funcname( மாறிலி வெறுமை* src_ptr, முழு len ) \
{ \
    மாறிலி T* src = static_cast<மாறிலி T*>(src_ptr); \
    முழு i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) ) \
    க்கு( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    திரும்பு nz; \
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
    v_uint16 w0 = v_expand_கீழ்(x0); \
    v_uint16 w1 = v_expand_உயர்வு(x0); \
    v_sum0 = v_add(v_sum0, v_expand_கீழ்(w0)); \
    v_sum1 = v_add(v_sum1, v_expand_உயர்வு(w0)); \
    v_sum0 = v_add(v_sum0, v_expand_கீழ்(w1)); \
    v_sum1 = v_add(v_sum1, v_expand_உயர்வு(w1))

#undef UPDATE_SUM_U16
#define UPDATE_SUM_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, v_expand_கீழ்(x0)); \
    v_sum1 = v_add(v_sum1, v_expand_உயர்வு(x0))

#undef UPDATE_SUM_S32
#define UPDATE_SUM_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, x0)

DEFINE_NONZERO_FUNC(மொத்தம்_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_add_wrap, UPDATE_SUM_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(மொத்தம்_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_add_wrap, UPDATE_SUM_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(மொத்தம்_Non_Zero32s, s32, s32, s32, முழு, v_int32, v_int32, v_eq, v_add, UPDATE_SUM_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(மொத்தம்_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_add, UPDATE_SUM_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(மொத்தம்_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_add_wrap, UPDATE_SUM_U16, CHECK_NZ_FP16)

#undef DEFINE_NONZERO_FUNC_NOSIMD
#define DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
நிலையான முழு funcname(மாறிலி வெறுமை* src, முழு len) \
{ \
    திரும்பு மொத்தம்_Non_Zero_2(static_cast<மாறிலி T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(மொத்தம்_Non_Zero64s, int64)

#எனில் CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
நிலையான முழு மொத்தம்_Non_Zero64f( மாறிலி வெறுமை* src_ptr, முழு len )
{
    மாறிலி இரட்டை* src = static_cast<மாறிலி இரட்டை*>(src_ptr);
    முழு i = 0, nz = 0;
    மாறிலி முழு vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    முழு nzeros = 0;
    க்கு (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_எடு(v_signmask(v_eq(vx_load(src + i), v_zero)));
        nzeros += cnz_lane_எடு(v_signmask(v_eq(vx_load(src + i + vlanes), v_zero)));
    }
    க்கு (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_எடு(v_signmask(v_eq(vx_load(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    க்கு (; i < len; i++)
        nz += src[i] != 0;
    திரும்பு nz;
}
#இல்லையெனில்
DEFINE_NONZERO_FUNC_NOSIMD(மொத்தம்_Non_Zero64f, இரட்டை)
#endif

மொத்தம்_Non_Zero_Func get_மொத்தம்_Non_Zero_Tab(முழு depth)
{
    நிலையான மொத்தம்_Non_Zero_Func மொத்தம்_Non_Zero_Tab[CV_DEPTH_MAX] =
    {
        மொத்தம்_Non_Zero8u,
        மொத்தம்_Non_Zero8u,
        மொத்தம்_Non_Zero16u,
        மொத்தம்_Non_Zero16u,
        மொத்தம்_Non_Zero32s,
        மொத்தம்_Non_Zero32f,
        மொத்தம்_Non_Zero64f,
        மொத்தம்_Non_Zero16f,
        மொத்தம்_Non_Zero16f, // for bf16 it's the same code as for f16
        மொத்தம்_Non_Zero8u,
        மொத்தம்_Non_Zero64s,
        மொத்தம்_Non_Zero64s,
        மொத்தம்_Non_Zero32s,
        0
    };

    திரும்பு மொத்தம்_Non_Zero_Tab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
