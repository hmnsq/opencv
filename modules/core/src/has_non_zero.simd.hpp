// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv {

రకనిర్వచనం తార్కిక (*HasNonZeroFunc)(స్థిరరాశి శూన్య*, size_t);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

HasNonZeroFunc getHasNonZeroFunc(పూర్ణాంకం లోతు);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#నిర్వచనం_రద్దు SIMD_ONLY
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
#నిర్వచించు SIMD_ONLY(సమాసం) సమాసం
#లేకపోతే
#నిర్వచించు SIMD_ONLY(సమాసం)
#అయితే_ముగింపు

#నిర్వచనం_రద్దు DEFINE_HASNONZERO_FUNC
#నిర్వచించు DEFINE_HASNONZERO_FUNC(funcname, suffix, T, VT, cmp_op, scalar_nz_op) \
స్థిర తార్కిక funcname( స్థిరరాశి శూన్య* _src, size_t len ) \
{ \
    స్థిరరాశి T* src = (స్థిరరాశి T*)_src; \
    size_t i = 0; \
    SIMD_ONLY( \
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    కొరకు (i = 0; i + vlanes*8 <= len; i += vlanes*8) \
    { \
        VT x0 = vx_ఎక్కించు(src + i); \
        VT x1 = vx_ఎక్కించు(src + i + vlanes); \
        VT x2 = vx_ఎక్కించు(src + i + vlanes*2); \
        VT x3 = vx_ఎక్కించు(src + i + vlanes*3); \
        VT x4 = vx_ఎక్కించు(src + i + vlanes*4); \
        VT x5 = vx_ఎక్కించు(src + i + vlanes*5); \
        VT x6 = vx_ఎక్కించు(src + i + vlanes*6); \
        VT x7 = vx_ఎక్కించు(src + i + vlanes*7); \
        x0 = v_or(x0, x1); \
        x2 = v_or(x2, x3); \
        x4 = v_or(x4, x5); \
        x6 = v_or(x6, x7); \
        x0 = v_or(x0, x2); \
        x4 = v_or(x4, x6); \
        x0 = v_or(x0, x4); \
        x0 = cmp_op(x0, v_zero); \
        ఒకవేళ (v_check_any(x0)) \
            తిరిగి నిజం; \
    } \
    కొరకు (; i < len; i += vlanes) \
    { \
        ఒకవేళ (i + vlanes > len) { \
            ఒకవేళ (i == 0) \
                ఆపు; \
            i = len - vlanes; \
        } \
        VT x0 = vx_ఎక్కించు(src + i); \
        x0 = cmp_op(x0, v_zero); \
        ఒకవేళ (v_check_any(x0)) \
            తిరిగి నిజం; \
    } \
    v_cleanup();) \
    కొరకు( ; i < len; i++ ) \
    { \
        T x = src[i]; \
        ఒకవేళ (scalar_nz_op(x) != 0) \
            తిరిగి నిజం; \
    } \
    తిరిగి అబద్ధం; \
}

#నిర్వచనం_రద్దు CHECK_NZ_INT
#నిర్వచించు CHECK_NZ_INT(x) ((x) != 0)
#నిర్వచనం_రద్దు CHECK_NZ_FP
#నిర్వచించు CHECK_NZ_FP(x) (((x)<<1) != 0)
#నిర్వచనం_రద్దు CHECK_NZ_FP16
#నిర్వచించు CHECK_NZ_FP16(x) (((x)&0x7fff) != 0)
#నిర్వచనం_రద్దు VEC_CMP_EQ_Z_FP16
#నిర్వచించు VEC_CMP_EQ_Z_FP16(x, z) v_ne(v_కూడు_wrap(x, x), z)
#నిర్వచనం_రద్దు VEC_CMP_EQ_Z_FP
#నిర్వచించు VEC_CMP_EQ_Z_FP(x, z) v_ne(v_కూడు(x, x), z)

DEFINE_HASNONZERO_FUNC(hasNonZero8u, u8, uchar, v_uint8, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero16u, u16, ushort, v_uint16, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero32s, s32, పూర్ణాంకం, v_int32, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero64s, s64, int64, v_int64, v_ne, CHECK_NZ_INT)

DEFINE_HASNONZERO_FUNC(hasNonZero32f, s32, పూర్ణాంకం, v_int32, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero64f, s64, int64, v_int64, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero16f, u16, ushort, v_uint16, VEC_CMP_EQ_Z_FP16, CHECK_NZ_FP16)

HasNonZeroFunc getHasNonZeroFunc(పూర్ణాంకం లోతు)
{
    స్థిర HasNonZeroFunc hasNonZeroTab[CV_లోతు_MAX] =
    {
        hasNonZero8u,
        hasNonZero8u,
        hasNonZero16u,
        hasNonZero16u,
        hasNonZero32s,
        hasNonZero32f,
        hasNonZero64f,
        hasNonZero16f,
        hasNonZero16f,
        hasNonZero8u,
        hasNonZero64s,
        hasNonZero64s,
        hasNonZero32s,
        0
    };

    తిరిగి hasNonZeroTab[లోతు];
}

#అయితే_ముగింపు

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace
