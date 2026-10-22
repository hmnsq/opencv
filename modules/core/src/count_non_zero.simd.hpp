// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv {

రకనిర్వచనం పూర్ణాంకం (*లెక్క_Non_Zero_Func)(స్థిరరాశి శూన్య*, పూర్ణాంకం);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

లెక్క_Non_Zero_Func get_లెక్క_Non_Zero_Tab(పూర్ణాంకం లోతు);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

మూస<రకం_పేరు T>
స్థిర పూర్ణాంకం లెక్క_Non_Zero_2(స్థిరరాశి T* src, పూర్ణాంకం len )
{
    పూర్ణాంకం nz = 0;
    కొరకు( పూర్ణాంకం i = 0; i < len; i++ )
        nz += src[i] != 0;
    తిరిగి nz;
}

#నిర్వచనం_రద్దు SIMD_ONLY
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
#నిర్వచించు SIMD_ONLY(సమాసం) సమాసం
#లేకపోతే
#నిర్వచించు SIMD_ONLY(సమాసం)
#అయితే_ముగింపు

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#నిర్వచనం_రద్దు CV_COUNTNONZERO_AVX512
#ఒకవేళ defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#నిర్వచించు CV_COUNTNONZERO_AVX512 1
#లేకపోతే
#నిర్వచించు CV_COUNTNONZERO_AVX512 0
#అయితే_ముగింపు

#ఒకవేళ CV_COUNTNONZERO_AVX512
స్థిర అంతఃవరుస పూర్ణాంకం cnz_popcount32(గుర్తులేని x)
{
#ఒకవేళ defined(__GNUC__) || defined(__clang__)
    తిరిగి __builtin_popcount(x);
#లేకపోతే
    పూర్ణాంకం c = 0;
    కొరకు (; x; x &= x - 1)
        ++c;
    తిరిగి c;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస పూర్ణాంకం cnz_popcount64(uint64 x)
{
#ఒకవేళ defined(__GNUC__) || defined(__clang__)
    తిరిగి (పూర్ణాంకం)__builtin_popcountll((గుర్తులేని దీర్ఘ దీర్ఘ)x);
#లేకపోతే
    పూర్ణాంకం c = 0;
    కొరకు (; x; x &= x - 1)
        ++c;
    తిరిగి c;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస పూర్ణాంకం cnz_lane_తీయి(పూర్ణాంకం m) { తిరిగి cnz_popcount32((గుర్తులేని)m); }
స్థిర అంతఃవరుస పూర్ణాంకం cnz_lane_తీయి(int64 m) { తిరిగి cnz_popcount64((uint64)m); }
#అయితే_ముగింపు // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#నిర్వచనం_రద్దు CNZ_SIMD_BODY
#ఒకవేళ CV_COUNTNONZERO_AVX512
#నిర్వచించు CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, కూడు_op, update_కూడిక) \
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    పూర్ణాంకం nzeros = 0; \
    కొరకు (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_తీయి(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ఎక్కించు(src + i), v_zero)))); \
        nzeros += cnz_lane_తీయి(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ఎక్కించు(src + i + vlanes), v_zero)))); \
    } \
    కొరకు (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_తీయి(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_ఎక్కించు(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#లేకపోతే
#నిర్వచించు CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, కూడు_op, update_కూడిక) \
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    కొరకు (i = 0; i <= len - vlanes*8; i += vlanes*8) \
    { \
        VT x0 = vx_ఎక్కించు(src + i); \
        VT x1 = vx_ఎక్కించు(src + i + vlanes); \
        VT x2 = vx_ఎక్కించు(src + i + vlanes*2); \
        VT x3 = vx_ఎక్కించు(src + i + vlanes*3); \
        VT x4 = vx_ఎక్కించు(src + i + vlanes*4); \
        VT x5 = vx_ఎక్కించు(src + i + vlanes*5); \
        VT x6 = vx_ఎక్కించు(src + i + vlanes*6); \
        VT x7 = vx_ఎక్కించు(src + i + vlanes*7); \
        x0 = cmp_op(x0, v_zero); \
        x1 = cmp_op(x1, v_zero); \
        x2 = cmp_op(x2, v_zero); \
        x3 = cmp_op(x3, v_zero); \
        x4 = cmp_op(x4, v_zero); \
        x5 = cmp_op(x5, v_zero); \
        x6 = cmp_op(x6, v_zero); \
        x7 = cmp_op(x7, v_zero); \
        x0 = కూడు_op(x0, x1); \
        x2 = కూడు_op(x2, x3); \
        x4 = కూడు_op(x4, x5); \
        x6 = కూడు_op(x6, x7); \
        x0 = కూడు_op(x0, x2); \
        x4 = కూడు_op(x4, x6); \
        x0 = కూడు_op(కూడు_op(x0, x4), v_8); \
        update_కూడిక(v_sum0, v_sum1, x0); \
    } \
    కొరకు (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_ఎక్కించు(src + i); \
        x0 = కూడు_op(cmp_op(x0, v_zero), v_1); \
        update_కూడిక(v_sum0, v_sum1, x0); \
    } \
    nz += (పూర్ణాంకం)v_reduce_కూడిక(v_కూడు(v_sum0, v_sum1)); \
    v_cleanup();
#అయితే_ముగింపు

#నిర్వచనం_రద్దు DEFINE_NONZERO_FUNC
#నిర్వచించు DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, కూడు_op, update_కూడిక, scalar_cmp_op) \
స్థిర పూర్ణాంకం funcname( స్థిరరాశి శూన్య* src_ptr, పూర్ణాంకం len ) \
{ \
    స్థిరరాశి T* src = static_cast<స్థిరరాశి T*>(src_ptr); \
    పూర్ణాంకం i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, కూడు_op, update_కూడిక) ) \
    కొరకు( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    తిరిగి nz; \
}

#నిర్వచనం_రద్దు CHECK_NZ_INT
#నిర్వచించు CHECK_NZ_INT(x) ((x) != 0)
#నిర్వచనం_రద్దు CHECK_NZ_FP
#నిర్వచించు CHECK_NZ_FP(x) ((x)*2 != 0)
// 16-bit float: mask the sign bit so -0.0 (0x8000) reads as zero.
#నిర్వచనం_రద్దు CHECK_NZ_FP16
#నిర్వచించు CHECK_NZ_FP16(x) (((x) & 0x7fff) != 0)
#నిర్వచనం_రద్దు VEC_CMP_EQ_Z_FP16
#నిర్వచించు VEC_CMP_EQ_Z_FP16(x, z) v_eq(v_కూడు_wrap(x, x), z)
#నిర్వచనం_రద్దు VEC_CMP_EQ_Z_FP
#నిర్వచించు VEC_CMP_EQ_Z_FP(x, z) v_eq(v_కూడు(x, x), z)

#నిర్వచనం_రద్దు UPDATE_కూడిక_U8
#నిర్వచించు UPDATE_కూడిక_U8(v_sum0, v_sum1, x0) \
    v_uint16 w0 = v_expand_దిగువ(x0); \
    v_uint16 w1 = v_expand_ఎగువ(x0); \
    v_sum0 = v_కూడు(v_sum0, v_expand_దిగువ(w0)); \
    v_sum1 = v_కూడు(v_sum1, v_expand_ఎగువ(w0)); \
    v_sum0 = v_కూడు(v_sum0, v_expand_దిగువ(w1)); \
    v_sum1 = v_కూడు(v_sum1, v_expand_ఎగువ(w1))

#నిర్వచనం_రద్దు UPDATE_కూడిక_U16
#నిర్వచించు UPDATE_కూడిక_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_కూడు(v_sum0, v_expand_దిగువ(x0)); \
    v_sum1 = v_కూడు(v_sum1, v_expand_ఎగువ(x0))

#నిర్వచనం_రద్దు UPDATE_కూడిక_S32
#నిర్వచించు UPDATE_కూడిక_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_కూడు(v_sum0, x0)

DEFINE_NONZERO_FUNC(లెక్క_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_కూడు_wrap, UPDATE_కూడిక_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(లెక్క_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_కూడు_wrap, UPDATE_కూడిక_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(లెక్క_Non_Zero32s, s32, s32, s32, పూర్ణాంకం, v_int32, v_int32, v_eq, v_కూడు, UPDATE_కూడిక_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(లెక్క_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_కూడు, UPDATE_కూడిక_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(లెక్క_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_కూడు_wrap, UPDATE_కూడిక_U16, CHECK_NZ_FP16)

#నిర్వచనం_రద్దు DEFINE_NONZERO_FUNC_NOSIMD
#నిర్వచించు DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
స్థిర పూర్ణాంకం funcname(స్థిరరాశి శూన్య* src, పూర్ణాంకం len) \
{ \
    తిరిగి లెక్క_Non_Zero_2(static_cast<స్థిరరాశి T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(లెక్క_Non_Zero64s, int64)

#ఒకవేళ CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
స్థిర పూర్ణాంకం లెక్క_Non_Zero64f( స్థిరరాశి శూన్య* src_ptr, పూర్ణాంకం len )
{
    స్థిరరాశి ద్విగుణ* src = static_cast<స్థిరరాశి ద్విగుణ*>(src_ptr);
    పూర్ణాంకం i = 0, nz = 0;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    పూర్ణాంకం nzeros = 0;
    కొరకు (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_తీయి(v_signmask(v_eq(vx_ఎక్కించు(src + i), v_zero)));
        nzeros += cnz_lane_తీయి(v_signmask(v_eq(vx_ఎక్కించు(src + i + vlanes), v_zero)));
    }
    కొరకు (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_తీయి(v_signmask(v_eq(vx_ఎక్కించు(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    కొరకు (; i < len; i++)
        nz += src[i] != 0;
    తిరిగి nz;
}
#లేకపోతే
DEFINE_NONZERO_FUNC_NOSIMD(లెక్క_Non_Zero64f, ద్విగుణ)
#అయితే_ముగింపు

లెక్క_Non_Zero_Func get_లెక్క_Non_Zero_Tab(పూర్ణాంకం లోతు)
{
    స్థిర లెక్క_Non_Zero_Func లెక్క_Non_Zero_Tab[CV_లోతు_MAX] =
    {
        లెక్క_Non_Zero8u,
        లెక్క_Non_Zero8u,
        లెక్క_Non_Zero16u,
        లెక్క_Non_Zero16u,
        లెక్క_Non_Zero32s,
        లెక్క_Non_Zero32f,
        లెక్క_Non_Zero64f,
        లెక్క_Non_Zero16f,
        లెక్క_Non_Zero16f, // for bf16 it's the same code as for f16
        లెక్క_Non_Zero8u,
        లెక్క_Non_Zero64s,
        లెక్క_Non_Zero64s,
        లెక్క_Non_Zero32s,
        0
    };

    తిరిగి లెక్క_Non_Zero_Tab[లోతు];
}

#అయితే_ముగింపు

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace
