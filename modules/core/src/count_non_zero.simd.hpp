// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

ନାମାକାଶ cv {

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*ଗଣନା_Non_Zero_Func)(ଅଚଳ ଶୂନ୍ୟ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ଗଣନା_Non_Zero_Func get_ଗଣନା_Non_Zero_Tab(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template<typename T>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_Non_Zero_2(ଅଚଳ T* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nz = 0;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ )
        nz += src[i] != 0;
    ଫେରନ୍ତୁ nz;
}

#undef SIMD_ONLY
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
#define SIMD_ONLY(ପଦସମଷ୍ଟି) ପଦସମଷ୍ଟି
#ଅନ୍ୟଥା
#define SIMD_ONLY(ପଦସମଷ୍ଟି)
#endif

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#undef CV_COUNTNONZERO_AVX512
#ଯଦି defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#define CV_COUNTNONZERO_AVX512 1
#ଅନ୍ୟଥା
#define CV_COUNTNONZERO_AVX512 0
#endif

#ଯଦି CV_COUNTNONZERO_AVX512
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnz_popcount32(unsigned x)
{
#ଯଦି defined(__GNUC__) || defined(__clang__)
    ଫେରନ୍ତୁ __builtin_popcount(x);
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0;
    ପାଇଁ (; x; x &= x - 1)
        ++c;
    ଫେରନ୍ତୁ c;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnz_popcount64(uint64 x)
{
#ଯଦି defined(__GNUC__) || defined(__clang__)
    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)__builtin_popcountll((unsigned long long)x);
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0;
    ପାଇଁ (; x; x &= x - 1)
        ++c;
    ଫେରନ୍ତୁ c;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnz_lane_କାଢ଼(ପୂର୍ଣ୍ଣସଂଖ୍ୟା m) { ଫେରନ୍ତୁ cnz_popcount32((unsigned)m); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cnz_lane_କାଢ଼(int64 m) { ଫେରନ୍ତୁ cnz_popcount64((uint64)m); }
#endif // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#undef CNZ_SIMD_BODY
#ଯଦି CV_COUNTNONZERO_AVX512
#define CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) \
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nzeros = 0; \
    ପାଇଁ (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_କାଢ଼(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
        nzeros += cnz_lane_କାଢ଼(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i + vlanes), v_zero)))); \
    } \
    ପାଇଁ (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_କାଢ଼(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#ଅନ୍ୟଥା
#define CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) \
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    ପାଇଁ (i = 0; i <= len - vlanes*8; i += vlanes*8) \
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
    ପାଇଁ (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_load(src + i); \
        x0 = add_op(cmp_op(x0, v_zero), v_1); \
        update_sum(v_sum0, v_sum1, x0); \
    } \
    nz += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v_reduce_sum(v_add(v_sum0, v_sum1)); \
    v_cleanup();
#endif

#undef DEFINE_NONZERO_FUNC
#define DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, add_op, update_sum, scalar_cmp_op) \
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା funcname( ଅଚଳ ଶୂନ୍ୟ* src_ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len ) \
{ \
    ଅଚଳ T* src = static_cast<ଅଚଳ T*>(src_ptr); \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_sum) ) \
    ପାଇଁ( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    ଫେରନ୍ତୁ nz; \
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
    v_uint16 w0 = v_expand_ତଳ(x0); \
    v_uint16 w1 = v_expand_ଉପର(x0); \
    v_sum0 = v_add(v_sum0, v_expand_ତଳ(w0)); \
    v_sum1 = v_add(v_sum1, v_expand_ଉପର(w0)); \
    v_sum0 = v_add(v_sum0, v_expand_ତଳ(w1)); \
    v_sum1 = v_add(v_sum1, v_expand_ଉପର(w1))

#undef UPDATE_SUM_U16
#define UPDATE_SUM_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, v_expand_ତଳ(x0)); \
    v_sum1 = v_add(v_sum1, v_expand_ଉପର(x0))

#undef UPDATE_SUM_S32
#define UPDATE_SUM_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, x0)

DEFINE_NONZERO_FUNC(ଗଣନା_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_add_wrap, UPDATE_SUM_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ଗଣନା_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_add_wrap, UPDATE_SUM_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ଗଣନା_Non_Zero32s, s32, s32, s32, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_int32, v_int32, v_eq, v_add, UPDATE_SUM_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(ଗଣନା_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_add, UPDATE_SUM_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(ଗଣନା_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_add_wrap, UPDATE_SUM_U16, CHECK_NZ_FP16)

#undef DEFINE_NONZERO_FUNC_NOSIMD
#define DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା funcname(ଅଚଳ ଶୂନ୍ୟ* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) \
{ \
    ଫେରନ୍ତୁ ଗଣନା_Non_Zero_2(static_cast<ଅଚଳ T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(ଗଣନା_Non_Zero64s, int64)

#ଯଦି CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_Non_Zero64f( ଅଚଳ ଶୂନ୍ୟ* src_ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len )
{
    ଅଚଳ ଦ୍ୱିଗୁଣ* src = static_cast<ଅଚଳ ଦ୍ୱିଗୁଣ*>(src_ptr);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, nz = 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nzeros = 0;
    ପାଇଁ (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_କାଢ଼(v_signmask(v_eq(vx_load(src + i), v_zero)));
        nzeros += cnz_lane_କାଢ଼(v_signmask(v_eq(vx_load(src + i + vlanes), v_zero)));
    }
    ପାଇଁ (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_କାଢ଼(v_signmask(v_eq(vx_load(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    ପାଇଁ (; i < len; i++)
        nz += src[i] != 0;
    ଫେରନ୍ତୁ nz;
}
#ଅନ୍ୟଥା
DEFINE_NONZERO_FUNC_NOSIMD(ଗଣନା_Non_Zero64f, ଦ୍ୱିଗୁଣ)
#endif

ଗଣନା_Non_Zero_Func get_ଗଣନା_Non_Zero_Tab(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର ଗଣନା_Non_Zero_Func ଗଣନା_Non_Zero_Tab[CV_DEPTH_MAX] =
    {
        ଗଣନା_Non_Zero8u,
        ଗଣନା_Non_Zero8u,
        ଗଣନା_Non_Zero16u,
        ଗଣନା_Non_Zero16u,
        ଗଣନା_Non_Zero32s,
        ଗଣନା_Non_Zero32f,
        ଗଣନା_Non_Zero64f,
        ଗଣନା_Non_Zero16f,
        ଗଣନା_Non_Zero16f, // for bf16 it's the same code as for f16
        ଗଣନା_Non_Zero8u,
        ଗଣନା_Non_Zero64s,
        ଗଣନା_Non_Zero64s,
        ଗଣନା_Non_Zero32s,
        0
    };

    ଫେରନ୍ତୁ ଗଣନା_Non_Zero_Tab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
