// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"

namespace cv {

प्रकार पूर्णांक (*गिनती_Non_Zero_Func)(स्थिर व्योम*, पूर्णांक);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

गिनती_Non_Zero_Func get_गिनती_Non_Zero_Tab(पूर्णांक depth);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ढाँचा<typename T>
अटल पूर्णांक गिनती_Non_Zero_2(स्थिर T* src, पूर्णांक len )
{
    पूर्णांक nz = 0;
    क्रम( पूर्णांक i = 0; i < len; i++ )
        nz += src[i] != 0;
    वापस nz;
}

#अघोषित SIMD_ONLY
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
#घोषणा SIMD_ONLY(व्यंजक) व्यंजक
#अन्यथा
#घोषणा SIMD_ONLY(व्यंजक)
#पूर्ण_अगर

// AVX-512 exposes wide per-lane mask registers, so counting zero lanes with
// v_signmask()+popcount is cheaper than the batched add-based reduction.
// Narrower SIMD (AVX2, NEON, LASX, ...) keeps the legacy batched kernel to
// avoid regressions on smaller vector widths.
#अघोषित CV_COUNTNONZERO_AVX512
#अगर defined(CV_CPU_COMPILE_AVX512_SKX) || defined(CV_CPU_COMPILE_AVX512_ICL)
#घोषणा CV_COUNTNONZERO_AVX512 1
#अन्यथा
#घोषणा CV_COUNTNONZERO_AVX512 0
#पूर्ण_अगर

#अगर CV_COUNTNONZERO_AVX512
अटल अंतरभूत पूर्णांक cnz_popcount32(अचिन्हित x)
{
#अगर defined(__GNUC__) || defined(__clang__)
    वापस __builtin_popcount(x);
#अन्यथा
    पूर्णांक c = 0;
    क्रम (; x; x &= x - 1)
        ++c;
    वापस c;
#पूर्ण_अगर
}

अटल अंतरभूत पूर्णांक cnz_popcount64(uint64 x)
{
#अगर defined(__GNUC__) || defined(__clang__)
    वापस (पूर्णांक)__builtin_popcountll((अचिन्हित दीर्घ दीर्घ)x);
#अन्यथा
    पूर्णांक c = 0;
    क्रम (; x; x &= x - 1)
        ++c;
    वापस c;
#पूर्ण_अगर
}

अटल अंतरभूत पूर्णांक cnz_lane_निकालो(पूर्णांक m) { वापस cnz_popcount32((अचिन्हित)m); }
अटल अंतरभूत पूर्णांक cnz_lane_निकालो(int64 m) { वापस cnz_popcount64((uint64)m); }
#पूर्ण_अगर // CV_COUNTNONZERO_AVX512

// SIMD body of the countNonZero kernels. The AVX-512 variant counts zero lanes
// via signmask+popcount; every other target keeps the legacy batched add kernel.
#अघोषित CNZ_SIMD_BODY
#अगर CV_COUNTNONZERO_AVX512
#घोषणा CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_योग) \
    स्थिर पूर्णांक vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    पूर्णांक nzeros = 0; \
    क्रम (; i <= len - vlanes*2; i += vlanes*2) \
    { \
        nzeros += cnz_lane_निकालो(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
        nzeros += cnz_lane_निकालो(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i + vlanes), v_zero)))); \
    } \
    क्रम (; i <= len - vlanes; i += vlanes) \
    { \
        nzeros += cnz_lane_निकालो(v_signmask(v_reinterpret_as_##rsuffix(cmp_op(vx_load(src + i), v_zero)))); \
    } \
    nz += i - nzeros; \
    v_cleanup();
#अन्यथा
#घोषणा CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_योग) \
    स्थिर पूर्णांक vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    VT v_1 = vx_setall_##suffix(1); \
    VT v_8 = vx_setall_##suffix(8); \
    ST v_sum0 = vx_setzero_##ssuffix(); \
    ST v_sum1 = v_sum0; \
    क्रम (i = 0; i <= len - vlanes*8; i += vlanes*8) \
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
        update_योग(v_sum0, v_sum1, x0); \
    } \
    क्रम (; i <= len - vlanes; i += vlanes) \
    { \
        VT x0 = vx_load(src + i); \
        x0 = add_op(cmp_op(x0, v_zero), v_1); \
        update_योग(v_sum0, v_sum1, x0); \
    } \
    nz += (पूर्णांक)v_reduce_योग(v_add(v_sum0, v_sum1)); \
    v_cleanup();
#पूर्ण_अगर

#अघोषित DEFINE_NONZERO_FUNC
#घोषणा DEFINE_NONZERO_FUNC(funcname, suffix, ssuffix, rsuffix, T, VT, ST, cmp_op, add_op, update_योग, scalar_cmp_op) \
अटल पूर्णांक funcname( स्थिर व्योम* src_ptr, पूर्णांक len ) \
{ \
    स्थिर T* src = static_cast<स्थिर T*>(src_ptr); \
    पूर्णांक i = 0, nz = 0; \
    SIMD_ONLY( CNZ_SIMD_BODY(suffix, ssuffix, rsuffix, VT, ST, cmp_op, add_op, update_योग) ) \
    क्रम( ; i < len; i++ ) \
    { \
        nz += scalar_cmp_op(src[i]); \
    } \
    वापस nz; \
}

#अघोषित CHECK_NZ_INT
#घोषणा CHECK_NZ_INT(x) ((x) != 0)
#अघोषित CHECK_NZ_FP
#घोषणा CHECK_NZ_FP(x) ((x)*2 != 0)
// 16-bit float: mask the sign bit so -0.0 (0x8000) reads as zero.
#अघोषित CHECK_NZ_FP16
#घोषणा CHECK_NZ_FP16(x) (((x) & 0x7fff) != 0)
#अघोषित VEC_CMP_EQ_Z_FP16
#घोषणा VEC_CMP_EQ_Z_FP16(x, z) v_eq(v_add_wrap(x, x), z)
#अघोषित VEC_CMP_EQ_Z_FP
#घोषणा VEC_CMP_EQ_Z_FP(x, z) v_eq(v_add(x, x), z)

#अघोषित UPDATE_योग_U8
#घोषणा UPDATE_योग_U8(v_sum0, v_sum1, x0) \
    v_uint16 w0 = v_expand_निचला(x0); \
    v_uint16 w1 = v_expand_ऊपरी(x0); \
    v_sum0 = v_add(v_sum0, v_expand_निचला(w0)); \
    v_sum1 = v_add(v_sum1, v_expand_ऊपरी(w0)); \
    v_sum0 = v_add(v_sum0, v_expand_निचला(w1)); \
    v_sum1 = v_add(v_sum1, v_expand_ऊपरी(w1))

#अघोषित UPDATE_योग_U16
#घोषणा UPDATE_योग_U16(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, v_expand_निचला(x0)); \
    v_sum1 = v_add(v_sum1, v_expand_ऊपरी(x0))

#अघोषित UPDATE_योग_S32
#घोषणा UPDATE_योग_S32(v_sum0, v_sum1, x0) \
    v_sum0 = v_add(v_sum0, x0)

DEFINE_NONZERO_FUNC(गिनती_Non_Zero8u, u8, u32, s8, uchar, v_uint8, v_uint32, v_eq, v_add_wrap, UPDATE_योग_U8, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(गिनती_Non_Zero16u, u16, u32, s16, ushort, v_uint16, v_uint32, v_eq, v_add_wrap, UPDATE_योग_U16, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(गिनती_Non_Zero32s, s32, s32, s32, पूर्णांक, v_int32, v_int32, v_eq, v_add, UPDATE_योग_S32, CHECK_NZ_INT)
DEFINE_NONZERO_FUNC(गिनती_Non_Zero32f, u32, u32, s32, uint, v_uint32, v_uint32, VEC_CMP_EQ_Z_FP, v_add, UPDATE_योग_S32, CHECK_NZ_FP)
DEFINE_NONZERO_FUNC(गिनती_Non_Zero16f, u16, u32, s16, ushort, v_uint16, v_uint32, VEC_CMP_EQ_Z_FP16, v_add_wrap, UPDATE_योग_U16, CHECK_NZ_FP16)

#अघोषित DEFINE_NONZERO_FUNC_NOSIMD
#घोषणा DEFINE_NONZERO_FUNC_NOSIMD(funcname, T) \
अटल पूर्णांक funcname(स्थिर व्योम* src, पूर्णांक len) \
{ \
    वापस गिनती_Non_Zero_2(static_cast<स्थिर T*>(src), len); \
}

DEFINE_NONZERO_FUNC_NOSIMD(गिनती_Non_Zero64s, int64)

#अगर CV_COUNTNONZERO_AVX512 && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// 64-bit floats: FP compare against zero treats -0.0 as zero, matching the
// scalar `src[i] != 0` path.
अटल पूर्णांक गिनती_Non_Zero64f( स्थिर व्योम* src_ptr, पूर्णांक len )
{
    स्थिर द्विगुन* src = static_cast<स्थिर द्विगुन*>(src_ptr);
    पूर्णांक i = 0, nz = 0;
    स्थिर पूर्णांक vlanes = VTraits<v_float64>::vlanes();
    v_float64 v_zero = vx_setzero_f64();
    पूर्णांक nzeros = 0;
    क्रम (; i <= len - vlanes*2; i += vlanes*2)
    {
        nzeros += cnz_lane_निकालो(v_signmask(v_eq(vx_load(src + i), v_zero)));
        nzeros += cnz_lane_निकालो(v_signmask(v_eq(vx_load(src + i + vlanes), v_zero)));
    }
    क्रम (; i <= len - vlanes; i += vlanes)
        nzeros += cnz_lane_निकालो(v_signmask(v_eq(vx_load(src + i), v_zero)));
    nz += i - nzeros;
    v_cleanup();
    क्रम (; i < len; i++)
        nz += src[i] != 0;
    वापस nz;
}
#अन्यथा
DEFINE_NONZERO_FUNC_NOSIMD(गिनती_Non_Zero64f, द्विगुन)
#पूर्ण_अगर

गिनती_Non_Zero_Func get_गिनती_Non_Zero_Tab(पूर्णांक depth)
{
    अटल गिनती_Non_Zero_Func गिनती_Non_Zero_Tab[CV_DEPTH_MAX] =
    {
        गिनती_Non_Zero8u,
        गिनती_Non_Zero8u,
        गिनती_Non_Zero16u,
        गिनती_Non_Zero16u,
        गिनती_Non_Zero32s,
        गिनती_Non_Zero32f,
        गिनती_Non_Zero64f,
        गिनती_Non_Zero16f,
        गिनती_Non_Zero16f, // for bf16 it's the same code as for f16
        गिनती_Non_Zero8u,
        गिनती_Non_Zero64s,
        गिनती_Non_Zero64s,
        गिनती_Non_Zero32s,
        0
    };

    वापस गिनती_Non_Zero_Tab[depth];
}

#पूर्ण_अगर

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
