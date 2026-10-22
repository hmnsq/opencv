// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"

نطاق cv {

تعريفنوع صحيح (*ڳڻپ_Non_Zero_Func)(ثابت فراغ*, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ڳڻپ_Non_Zero_Func get_ڳڻپ_Non_Zero_Tab(صحيح اونهائي);

#جيڪڏهن_وصف_نه_ٿيل CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

سانچو<قسم_نالو T>
ساكن صحيح ڳڻپ_Non_Zero_2(ثابت T* src, صحيح len )
{
    صحيح nz = 0;
    لكل( صحيح i = 0; i < len; i++ )
        nz += src[i] != 0;
    أرجع nz;
}

#وصف_رد SIMD_ONLY
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
#وصف_ڪريو SIMD_ONLY(اظهار) اظهار
#وإلا
#وصف_ڪريو SIMD_ONLY(اظهار)
#جيڪڏهن_پورو

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#وصف_رد CV_COUNTNONZERO_AVX512
#إذا defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#وصف_ڪريو CV_COUNTNONZERO_AVX512 1
#وإلا
#وصف_ڪريو CV_COUNTNONZERO_AVX512 0
#جيڪڏهن_پورو

#إذا CV_COUNTNONZERO_AVX512
ساكن مضمن صحيح cnz_popcount32(بنانشان x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    أرجع __builtin_popcount(x);
#وإلا
    صحيح c = 0;
    لكل (; x; x &= x - 1)
        ++c;
    أرجع c;
#جيڪڏهن_پورو
}

ساكن مضمن صحيح cnz_popcount64(uint64 x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    أرجع (صحيح)__builtin_popcountll((بنانشان ڊگهو ڊگهو)x);
#وإلا
    صحيح c = 0;
    لكل (; x; x &= x - 1)
        ++c;
    أرجع c;
#جيڪڏهن_پورو
}

ساكن مضمن صحيح cnz_lane_ڪڍو(صحيح m) { أرجع cnz_popcount32((بنانشان)m); }
ساكن مضمن صحيح cnz_lane_ڪڍو(int64 m) { أرجع cnz_popcount64((uint64)m); }
#جيڪڏهن_پورو // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#وصف_رد CNZ_SIMD_BODY
#إذا CV_COUNTNONZERO_AVX512
#وصف_ڪريو CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, جوڙيو_op, update_جوڙ) \
    ثابت صحيح vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    صحيح nzeros = 0; \
    لكل (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_ڪڍو(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ڀريو(src + i), v_zero)))); \
        nzeros += cnz_lane_ڪڍو(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ڀريو(src + i + vlanes), v_zero)))); \
    } \
    لكل (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_ڪڍو(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ڀريو(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#وإلا
#وصف_ڪريو CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, جوڙيو_op, update_جوڙ) \
    ثابت صحيح vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    لكل (i = 0; i <= len - vlanes*8; i += vlanes*8) \
    { \
        VT x0 = vx_ڀريو(src + i); \
        VT x1 = vx_ڀريو(src + i + vlanes); \
        VT x2 = vx_ڀريو(src + i + vlanes*2); \
        VT x3 = vx_ڀريو(src + i + vlanes*3); \
        VT x4 = vx_ڀريو(src + i + vlanes*4); \
        VT x5 = vx_ڀريو(src + i + vlanes*5); \
        VT x6 = vx_ڀريو(src + i + vlanes*6); \
        VT x7 = vx_ڀريو(src + i + vlanes*7); \
        x0 = cmp_op(x0, v_zero); \
        x1 = cmp_op(x1, v_zero); \
        x2 = cmp_op(x2, v_zero); \
        x3 = cmp_op(x3, v_zero); \
        x4 = cmp_op(x4, v_zero); \
        x5 = cmp_op(x5, v_zero); \
        x6 = cmp_op(x6, v_zero); \
        x7 = cmp_op(x7, v_zero); \
        x0 = جوڙيو_op(x0, x1); \
        x2 = جوڙيو_op(x2, x3); \
        x4 = جوڙيو_op(x4, x5); \
        x6 = جوڙيو_op(x6, x7); \
        x0 = جوڙيو_op(x0, x2); \
        x4 = جوڙيو_op(x4, x6); \
        x0 = جوڙيو_op(جوڙيو_op(x0, x4), v_8); \
        update_جوڙ(v_sum0, v_sum1, x0); \
    } \
    لكل (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_ڀريو(src + i); \
        x0 = جوڙيو_op(cmp_op(x0, v_zero), v_1); \
        update_جوڙ(v_sum0, v_sum1, x0); \
    } \
    nz += (صحيح)v_reduce_جوڙ(v_جوڙيو(v_sum0, v_sum1)); \
    v_cleanup();
#جيڪڏهن_پورو

#وصف_رد DEFINE_NONZERO_FUNC
#وصف_ڪريو DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, جوڙيو_op, update_جوڙ, scalar_cmp_op) \
ساكن صحيح funcname( ثابت فراغ* src_ptr, صحيح len ) \
{ \
    ثابت T* src = static_cast<ثابت T*>(src_ptr); \
    صحيح i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, جوڙيو_op, update_جوڙ) ) \
    لكل( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    أرجع nz; \
}

#وصف_رد CHECK_NZ_INT
#وصف_ڪريو CHECK_NZ_INT(x) ((x) != 0)
#وصف_رد CHECK_NZ_FP
#وصف_ڪريو CHECK_NZ_FP(x) ((x)*2 != 0)
// 16-bit float: mask the sign bit so -0.0 (0x8000) reads as zero.
#وصف_رد CHECK_NZ_FP16
#وصف_ڪريو CHECK_NZ_FP16(x) (((x) & 0x7fff) != 0)
#وصف_رد VEC_CMP_EQ_Z_FP16
#وصف_ڪريو VEC_CMP_EQ_Z_FP16(x, z) v_eq(v_جوڙيو_wrap(x, x), z)
#وصف_رد VEC_CMP_EQ_Z_FP
#وصف_ڪريو VEC_CMP_EQ_Z_FP(x, z) v_eq(v_جوڙيو(x, x), z)

#وصف_رد UPDATE_جوڙ_U8
#وصف_ڪريو UPDATE_جوڙ_U8(v_sum0, v_sum1, x0) \
    v_uint16 w0 = v_expand_هيٺيون(x0); \
    v_uint16 w1 = v_expand_مٿاهون(x0); \
    v_sum0 = v_جوڙيو(v_sum0, v_expand_هيٺيون(w0)); \
    v_sum1 = v_جوڙيو(v_sum1, v_expand_مٿاهون(w0)); \
    v_sum0 = v_جوڙيو(v_sum0, v_expand_هيٺيون(w1)); \
    v_sum1 = v_جوڙيو(v_sum1, v_expand_مٿاهون(w1))

#وصف_رد UPDATE_جوڙ_U16
#وصف_ڪريو UPDATE_جوڙ_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_جوڙيو(v_sum0, v_expand_هيٺيون(x0)); \
    v_sum1 = v_جوڙيو(v_sum1, v_expand_مٿاهون(x0))

#وصف_رد UPDATE_جوڙ_S32
#وصف_ڪريو UPDATE_جوڙ_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_جوڙيو(v_sum0, x0)

DEFINE_NONZERO_FUNC(ڳڻپ_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_جوڙيو_wrap, UPDATE_جوڙ_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ڳڻپ_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_جوڙيو_wrap, UPDATE_جوڙ_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ڳڻپ_Non_Zero32s, s32, s32, s32, صحيح, v_int32, v_int32, v_eq, v_جوڙيو, UPDATE_جوڙ_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ڳڻپ_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_جوڙيو, UPDATE_جوڙ_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(ڳڻپ_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_جوڙيو_wrap, UPDATE_جوڙ_U16, CHECK_NZ_FP16)

#وصف_رد DEFINE_NONZERO_FUNC_NOSIMD
#وصف_ڪريو DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
ساكن صحيح funcname(ثابت فراغ* src, صحيح len) \
{ \
    أرجع ڳڻپ_Non_Zero_2(static_cast<ثابت T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(ڳڻپ_Non_Zero64s, int64)

#إذا CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
ساكن صحيح ڳڻپ_Non_Zero64f( ثابت فراغ* src_ptr, صحيح len )
{
    ثابت مزدوج* src = static_cast<ثابت مزدوج*>(src_ptr);
    صحيح i = 0, nz = 0;
    ثابت صحيح vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    صحيح nzeros = 0;
    لكل (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_ڪڍو(v_signmask(v_eq(vx_ڀريو(src + i), v_zero)));
        nzeros += cnz_lane_ڪڍو(v_signmask(v_eq(vx_ڀريو(src + i + vlanes), v_zero)));
    }
    لكل (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_ڪڍو(v_signmask(v_eq(vx_ڀريو(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    لكل (; i < len; i++)
        nz += src[i] != 0;
    أرجع nz;
}
#وإلا
DEFINE_NONZERO_FUNC_NOSIMD(ڳڻپ_Non_Zero64f, مزدوج)
#جيڪڏهن_پورو

ڳڻپ_Non_Zero_Func get_ڳڻپ_Non_Zero_Tab(صحيح اونهائي)
{
    ساكن ڳڻپ_Non_Zero_Func ڳڻپ_Non_Zero_Tab[CV_اونهائي_MAX] =
    {
        ڳڻپ_Non_Zero8u,
        ڳڻپ_Non_Zero8u,
        ڳڻپ_Non_Zero16u,
        ڳڻپ_Non_Zero16u,
        ڳڻپ_Non_Zero32s,
        ڳڻپ_Non_Zero32f,
        ڳڻپ_Non_Zero64f,
        ڳڻپ_Non_Zero16f,
        ڳڻپ_Non_Zero16f, // for bf16 it's the same code as for f16
        ڳڻپ_Non_Zero8u,
        ڳڻپ_Non_Zero64s,
        ڳڻپ_Non_Zero64s,
        ڳڻپ_Non_Zero32s,
        0
    };

    أرجع ڳڻپ_Non_Zero_Tab[اونهائي];
}

#جيڪڏهن_پورو

CV_CPU_OPTIMIZATION_NAMESPACE_پڇاڙي
} // namespace
