// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"

नामाकाश cv {

प्रकारनिर्देश पूर्णाङ्क (*गणनम्_Non_Zero_Func)(ध्रुव शून्य*, पूर्णाङ्क);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

गणनम्_Non_Zero_Func get_गणनम्_Non_Zero_Tab(पूर्णाङ्क गभीरता);

#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

प्रतिरूपम्<प्रकारनाम T>
स्थिर पूर्णाङ्क गणनम्_Non_Zero_2(ध्रुव T* src, पूर्णाङ्क len )
{
    पूर्णाङ्क nz = 0;
    प्रति( पूर्णाङ्क i = 0; i < len; i++ )
        nz += src[i] != 0;
    प्रत्यागम nz;
}

#अपरिभाषय SIMD_ONLY
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
#परिभाषय SIMD_ONLY(व्यञ्जकम्) व्यञ्जकम्
#अन्यथा
#परिभाषय SIMD_ONLY(व्यञ्जकम्)
#यदि_अन्तः

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#अपरिभाषय CV_COUNTNONZERO_AVX512
#यदि defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#परिभाषय CV_COUNTNONZERO_AVX512 1
#अन्यथा
#परिभाषय CV_COUNTNONZERO_AVX512 0
#यदि_अन्तः

#यदि CV_COUNTNONZERO_AVX512
स्थिर अन्तःस्थ पूर्णाङ्क cnz_popcount32(अचिह्नित x)
{
#यदि defined(__GNUC__) || defined(__clang__)
    प्रत्यागम __builtin_popcount(x);
#अन्यथा
    पूर्णाङ्क c = 0;
    प्रति (; x; x &= x - 1)
        ++c;
    प्रत्यागम c;
#यदि_अन्तः
}

स्थिर अन्तःस्थ पूर्णाङ्क cnz_popcount64(uint64 x)
{
#यदि defined(__GNUC__) || defined(__clang__)
    प्रत्यागम (पूर्णाङ्क)__builtin_popcountll((अचिह्नित दीर्घ दीर्घ)x);
#अन्यथा
    पूर्णाङ्क c = 0;
    प्रति (; x; x &= x - 1)
        ++c;
    प्रत्यागम c;
#यदि_अन्तः
}

स्थिर अन्तःस्थ पूर्णाङ्क cnz_lane_निष्कासय(पूर्णाङ्क m) { प्रत्यागम cnz_popcount32((अचिह्नित)m); }
स्थिर अन्तःस्थ पूर्णाङ्क cnz_lane_निष्कासय(int64 m) { प्रत्यागम cnz_popcount64((uint64)m); }
#यदि_अन्तः // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#अपरिभाषय CNZ_SIMD_BODY
#यदि CV_COUNTNONZERO_AVX512
#परिभाषय CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, सङ्कलय_op, update_योग) \
    ध्रुव पूर्णाङ्क vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    पूर्णाङ्क nzeros = 0; \
    प्रति (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_निष्कासय(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_आरोपय(src + i), v_zero)))); \
        nzeros += cnz_lane_निष्कासय(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_आरोपय(src + i + vlanes), v_zero)))); \
    } \
    प्रति (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_निष्कासय(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_आरोपय(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#अन्यथा
#परिभाषय CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, सङ्कलय_op, update_योग) \
    ध्रुव पूर्णाङ्क vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    प्रति (i = 0; i <= len - vlanes*8; i += vlanes*8) \
    { \
        VT x0 = vx_आरोपय(src + i); \
        VT x1 = vx_आरोपय(src + i + vlanes); \
        VT x2 = vx_आरोपय(src + i + vlanes*2); \
        VT x3 = vx_आरोपय(src + i + vlanes*3); \
        VT x4 = vx_आरोपय(src + i + vlanes*4); \
        VT x5 = vx_आरोपय(src + i + vlanes*5); \
        VT x6 = vx_आरोपय(src + i + vlanes*6); \
        VT x7 = vx_आरोपय(src + i + vlanes*7); \
        x0 = cmp_op(x0, v_zero); \
        x1 = cmp_op(x1, v_zero); \
        x2 = cmp_op(x2, v_zero); \
        x3 = cmp_op(x3, v_zero); \
        x4 = cmp_op(x4, v_zero); \
        x5 = cmp_op(x5, v_zero); \
        x6 = cmp_op(x6, v_zero); \
        x7 = cmp_op(x7, v_zero); \
        x0 = सङ्कलय_op(x0, x1); \
        x2 = सङ्कलय_op(x2, x3); \
        x4 = सङ्कलय_op(x4, x5); \
        x6 = सङ्कलय_op(x6, x7); \
        x0 = सङ्कलय_op(x0, x2); \
        x4 = सङ्कलय_op(x4, x6); \
        x0 = सङ्कलय_op(सङ्कलय_op(x0, x4), v_8); \
        update_योग(v_sum0, v_sum1, x0); \
    } \
    प्रति (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_आरोपय(src + i); \
        x0 = सङ्कलय_op(cmp_op(x0, v_zero), v_1); \
        update_योग(v_sum0, v_sum1, x0); \
    } \
    nz += (पूर्णाङ्क)v_reduce_योग(v_सङ्कलय(v_sum0, v_sum1)); \
    v_cleanup();
#यदि_अन्तः

#अपरिभाषय DEFINE_NONZERO_FUNC
#परिभाषय DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, सङ्कलय_op, update_योग, scalar_cmp_op) \
स्थिर पूर्णाङ्क funcname( ध्रुव शून्य* src_ptr, पूर्णाङ्क len ) \
{ \
    ध्रुव T* src = static_cast<ध्रुव T*>(src_ptr); \
    पूर्णाङ्क i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, सङ्कलय_op, update_योग) ) \
    प्रति( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    प्रत्यागम nz; \
}

#अपरिभाषय CHECK_NZ_INT
#परिभाषय CHECK_NZ_INT(x) ((x) != 0)
#अपरिभाषय CHECK_NZ_FP
#परिभाषय CHECK_NZ_FP(x) ((x)*2 != 0)
// 16-bit float: mask the sign bit so -0.0 (0x8000) reads as zero.
#अपरिभाषय CHECK_NZ_FP16
#परिभाषय CHECK_NZ_FP16(x) (((x) & 0x7fff) != 0)
#अपरिभाषय VEC_CMP_EQ_Z_FP16
#परिभाषय VEC_CMP_EQ_Z_FP16(x, z) v_eq(v_सङ्कलय_wrap(x, x), z)
#अपरिभाषय VEC_CMP_EQ_Z_FP
#परिभाषय VEC_CMP_EQ_Z_FP(x, z) v_eq(v_सङ्कलय(x, x), z)

#अपरिभाषय UPDATE_योग_U8
#परिभाषय UPDATE_योग_U8(v_sum0, v_sum1, x0) \
    v_uint16 w0 = v_expand_नीचः(x0); \
    v_uint16 w1 = v_expand_उच्चः(x0); \
    v_sum0 = v_सङ्कलय(v_sum0, v_expand_नीचः(w0)); \
    v_sum1 = v_सङ्कलय(v_sum1, v_expand_उच्चः(w0)); \
    v_sum0 = v_सङ्कलय(v_sum0, v_expand_नीचः(w1)); \
    v_sum1 = v_सङ्कलय(v_sum1, v_expand_उच्चः(w1))

#अपरिभाषय UPDATE_योग_U16
#परिभाषय UPDATE_योग_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_सङ्कलय(v_sum0, v_expand_नीचः(x0)); \
    v_sum1 = v_सङ्कलय(v_sum1, v_expand_उच्चः(x0))

#अपरिभाषय UPDATE_योग_S32
#परिभाषय UPDATE_योग_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_सङ्कलय(v_sum0, x0)

DEFINE_NONZERO_FUNC(गणनम्_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_सङ्कलय_wrap, UPDATE_योग_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(गणनम्_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_सङ्कलय_wrap, UPDATE_योग_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(गणनम्_Non_Zero32s, s32, s32, s32, पूर्णाङ्क, v_int32, v_int32, v_eq, v_सङ्कलय, UPDATE_योग_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(गणनम्_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_सङ्कलय, UPDATE_योग_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(गणनम्_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_सङ्कलय_wrap, UPDATE_योग_U16, CHECK_NZ_FP16)

#अपरिभाषय DEFINE_NONZERO_FUNC_NOSIMD
#परिभाषय DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
स्थिर पूर्णाङ्क funcname(ध्रुव शून्य* src, पूर्णाङ्क len) \
{ \
    प्रत्यागम गणनम्_Non_Zero_2(static_cast<ध्रुव T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(गणनम्_Non_Zero64s, int64)

#यदि CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
स्थिर पूर्णाङ्क गणनम्_Non_Zero64f( ध्रुव शून्य* src_ptr, पूर्णाङ्क len )
{
    ध्रुव द्विगुण* src = static_cast<ध्रुव द्विगुण*>(src_ptr);
    पूर्णाङ्क i = 0, nz = 0;
    ध्रुव पूर्णाङ्क vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    पूर्णाङ्क nzeros = 0;
    प्रति (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_निष्कासय(v_signmask(v_eq(vx_आरोपय(src + i), v_zero)));
        nzeros += cnz_lane_निष्कासय(v_signmask(v_eq(vx_आरोपय(src + i + vlanes), v_zero)));
    }
    प्रति (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_निष्कासय(v_signmask(v_eq(vx_आरोपय(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    प्रति (; i < len; i++)
        nz += src[i] != 0;
    प्रत्यागम nz;
}
#अन्यथा
DEFINE_NONZERO_FUNC_NOSIMD(गणनम्_Non_Zero64f, द्विगुण)
#यदि_अन्तः

गणनम्_Non_Zero_Func get_गणनम्_Non_Zero_Tab(पूर्णाङ्क गभीरता)
{
    स्थिर गणनम्_Non_Zero_Func गणनम्_Non_Zero_Tab[CV_गभीरता_MAX] =
    {
        गणनम्_Non_Zero8u,
        गणनम्_Non_Zero8u,
        गणनम्_Non_Zero16u,
        गणनम्_Non_Zero16u,
        गणनम्_Non_Zero32s,
        गणनम्_Non_Zero32f,
        गणनम्_Non_Zero64f,
        गणनम्_Non_Zero16f,
        गणनम्_Non_Zero16f, // for bf16 it's the same code as for f16
        गणनम्_Non_Zero8u,
        गणनम्_Non_Zero64s,
        गणनम्_Non_Zero64s,
        गणनम्_Non_Zero32s,
        0
    };

    प्रत्यागम गणनम्_Non_Zero_Tab[गभीरता];
}

#यदि_अन्तः

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
} // namespace
