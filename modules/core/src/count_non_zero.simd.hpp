// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"

نطاق cv {

تعريفنوع صحيح (*شمېر_Non_Zero_Func)(ثابت فراغ*, صحيح);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

شمېر_Non_Zero_Func get_شمېر_Non_Zero_Tab(صحيح ژوروالی);

#که_نه_تعریف_شوی CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

کينډۍ<ډول_نوم T>
ساكن صحيح شمېر_Non_Zero_2(ثابت T* src, صحيح len )
{
    صحيح nz = 0;
    لكل( صحيح i = 0; i < len; i++ )
        nz += src[i] != 0;
    أرجع nz;
}

#تعریف_لغوه SIMD_ONLY
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
#تعریف_کړه SIMD_ONLY(بیان) بیان
#وإلا
#تعریف_کړه SIMD_ONLY(بیان)
#د_که_پای

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#تعریف_لغوه CV_COUNTNONZERO_AVX512
#إذا defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#تعریف_کړه CV_COUNTNONZERO_AVX512 1
#وإلا
#تعریف_کړه CV_COUNTNONZERO_AVX512 0
#د_که_پای

#إذا CV_COUNTNONZERO_AVX512
ساكن مضمن صحيح cnz_popcount32(بینښه x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    أرجع __builtin_popcount(x);
#وإلا
    صحيح c = 0;
    لكل (; x; x &= x - 1)
        ++c;
    أرجع c;
#د_که_پای
}

ساكن مضمن صحيح cnz_popcount64(uint64 x)
{
#إذا defined(__GNUC__) || defined(__clang__)
    أرجع (صحيح)__builtin_popcountll((بینښه اوږد اوږد)x);
#وإلا
    صحيح c = 0;
    لكل (; x; x &= x - 1)
        ++c;
    أرجع c;
#د_که_پای
}

ساكن مضمن صحيح cnz_lane_راوباسه(صحيح m) { أرجع cnz_popcount32((بینښه)m); }
ساكن مضمن صحيح cnz_lane_راوباسه(int64 m) { أرجع cnz_popcount64((uint64)m); }
#د_که_پای // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#تعریف_لغوه CNZ_SIMD_BODY
#إذا CV_COUNTNONZERO_AVX512
#تعریف_کړه CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, جمع_کړه_op, update_مجموعه) \
    ثابت صحيح vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    صحيح nzeros = 0; \
    لكل (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_راوباسه(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_بارول(src + i), v_zero)))); \
        nzeros += cnz_lane_راوباسه(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_بارول(src + i + vlanes), v_zero)))); \
    } \
    لكل (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_راوباسه(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_بارول(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#وإلا
#تعریف_کړه CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, جمع_کړه_op, update_مجموعه) \
    ثابت صحيح vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    لكل (i = 0; i <= len - vlanes*8; i += vlanes*8) \
    { \
        VT x0 = vx_بارول(src + i); \
        VT x1 = vx_بارول(src + i + vlanes); \
        VT x2 = vx_بارول(src + i + vlanes*2); \
        VT x3 = vx_بارول(src + i + vlanes*3); \
        VT x4 = vx_بارول(src + i + vlanes*4); \
        VT x5 = vx_بارول(src + i + vlanes*5); \
        VT x6 = vx_بارول(src + i + vlanes*6); \
        VT x7 = vx_بارول(src + i + vlanes*7); \
        x0 = cmp_op(x0, v_zero); \
        x1 = cmp_op(x1, v_zero); \
        x2 = cmp_op(x2, v_zero); \
        x3 = cmp_op(x3, v_zero); \
        x4 = cmp_op(x4, v_zero); \
        x5 = cmp_op(x5, v_zero); \
        x6 = cmp_op(x6, v_zero); \
        x7 = cmp_op(x7, v_zero); \
        x0 = جمع_کړه_op(x0, x1); \
        x2 = جمع_کړه_op(x2, x3); \
        x4 = جمع_کړه_op(x4, x5); \
        x6 = جمع_کړه_op(x6, x7); \
        x0 = جمع_کړه_op(x0, x2); \
        x4 = جمع_کړه_op(x4, x6); \
        x0 = جمع_کړه_op(جمع_کړه_op(x0, x4), v_8); \
        update_مجموعه(v_sum0, v_sum1, x0); \
    } \
    لكل (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_بارول(src + i); \
        x0 = جمع_کړه_op(cmp_op(x0, v_zero), v_1); \
        update_مجموعه(v_sum0, v_sum1, x0); \
    } \
    nz += (صحيح)v_reduce_مجموعه(v_جمع_کړه(v_sum0, v_sum1)); \
    v_cleanup();
#د_که_پای

#تعریف_لغوه DEFINE_NONZERO_FUNC
#تعریف_کړه DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, جمع_کړه_op, update_مجموعه, scalar_cmp_op) \
ساكن صحيح funcname( ثابت فراغ* src_ptr, صحيح len ) \
{ \
    ثابت T* src = static_cast<ثابت T*>(src_ptr); \
    صحيح i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, جمع_کړه_op, update_مجموعه) ) \
    لكل( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    أرجع nz; \
}

#تعریف_لغوه CHECK_NZ_INT
#تعریف_کړه CHECK_NZ_INT(x) ((x) != 0)
#تعریف_لغوه CHECK_NZ_FP
#تعریف_کړه CHECK_NZ_FP(x) ((x)*2 != 0)
// 16-bit float: mask the sign bit so -0.0 (0x8000) reads as zero.
#تعریف_لغوه CHECK_NZ_FP16
#تعریف_کړه CHECK_NZ_FP16(x) (((x) & 0x7fff) != 0)
#تعریف_لغوه VEC_CMP_EQ_Z_FP16
#تعریف_کړه VEC_CMP_EQ_Z_FP16(x, z) v_eq(v_جمع_کړه_wrap(x, x), z)
#تعریف_لغوه VEC_CMP_EQ_Z_FP
#تعریف_کړه VEC_CMP_EQ_Z_FP(x, z) v_eq(v_جمع_کړه(x, x), z)

#تعریف_لغوه UPDATE_مجموعه_U8
#تعریف_کړه UPDATE_مجموعه_U8(v_sum0, v_sum1, x0) \
    v_uint16 w0 = v_expand_ټیټ(x0); \
    v_uint16 w1 = v_expand_لوړ(x0); \
    v_sum0 = v_جمع_کړه(v_sum0, v_expand_ټیټ(w0)); \
    v_sum1 = v_جمع_کړه(v_sum1, v_expand_لوړ(w0)); \
    v_sum0 = v_جمع_کړه(v_sum0, v_expand_ټیټ(w1)); \
    v_sum1 = v_جمع_کړه(v_sum1, v_expand_لوړ(w1))

#تعریف_لغوه UPDATE_مجموعه_U16
#تعریف_کړه UPDATE_مجموعه_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_جمع_کړه(v_sum0, v_expand_ټیټ(x0)); \
    v_sum1 = v_جمع_کړه(v_sum1, v_expand_لوړ(x0))

#تعریف_لغوه UPDATE_مجموعه_S32
#تعریف_کړه UPDATE_مجموعه_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_جمع_کړه(v_sum0, x0)

DEFINE_NONZERO_FUNC(شمېر_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_جمع_کړه_wrap, UPDATE_مجموعه_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(شمېر_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_جمع_کړه_wrap, UPDATE_مجموعه_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(شمېر_Non_Zero32s, s32, s32, s32, صحيح, v_int32, v_int32, v_eq, v_جمع_کړه, UPDATE_مجموعه_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(شمېر_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_جمع_کړه, UPDATE_مجموعه_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(شمېر_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_جمع_کړه_wrap, UPDATE_مجموعه_U16, CHECK_NZ_FP16)

#تعریف_لغوه DEFINE_NONZERO_FUNC_NOSIMD
#تعریف_کړه DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
ساكن صحيح funcname(ثابت فراغ* src, صحيح len) \
{ \
    أرجع شمېر_Non_Zero_2(static_cast<ثابت T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(شمېر_Non_Zero64s, int64)

#إذا CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
ساكن صحيح شمېر_Non_Zero64f( ثابت فراغ* src_ptr, صحيح len )
{
    ثابت مزدوج* src = static_cast<ثابت مزدوج*>(src_ptr);
    صحيح i = 0, nz = 0;
    ثابت صحيح vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    صحيح nzeros = 0;
    لكل (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_راوباسه(v_signmask(v_eq(vx_بارول(src + i), v_zero)));
        nzeros += cnz_lane_راوباسه(v_signmask(v_eq(vx_بارول(src + i + vlanes), v_zero)));
    }
    لكل (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_راوباسه(v_signmask(v_eq(vx_بارول(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    لكل (; i < len; i++)
        nz += src[i] != 0;
    أرجع nz;
}
#وإلا
DEFINE_NONZERO_FUNC_NOSIMD(شمېر_Non_Zero64f, مزدوج)
#د_که_پای

شمېر_Non_Zero_Func get_شمېر_Non_Zero_Tab(صحيح ژوروالی)
{
    ساكن شمېر_Non_Zero_Func شمېر_Non_Zero_Tab[CV_ژوروالی_MAX] =
    {
        شمېر_Non_Zero8u,
        شمېر_Non_Zero8u,
        شمېر_Non_Zero16u,
        شمېر_Non_Zero16u,
        شمېر_Non_Zero32s,
        شمېر_Non_Zero32f,
        شمېر_Non_Zero64f,
        شمېر_Non_Zero16f,
        شمېر_Non_Zero16f, // for bf16 it's the same code as for f16
        شمېر_Non_Zero8u,
        شمېر_Non_Zero64s,
        شمېر_Non_Zero64s,
        شمېر_Non_Zero32s,
        0
    };

    أرجع شمېر_Non_Zero_Tab[ژوروالی];
}

#د_که_پای

CV_CPU_OPTIMIZATION_NAMESPACE_پای_ټکی
} // namespace
