// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"

namespace cv {

प्रकार bool (*HasNonZeroFunc)(स्थिर व्योम*, माप_प्रकार);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

HasNonZeroFunc getHasNonZeroFunc(पूर्णांक depth);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#अघोषित SIMD_ONLY
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
#घोषणा SIMD_ONLY(व्यंजक) व्यंजक
#अन्यथा
#घोषणा SIMD_ONLY(व्यंजक)
#पूर्ण_अगर

#अघोषित DEFINE_HASNONZERO_FUNC
#घोषणा DEFINE_HASNONZERO_FUNC(funcname, suffix, T, VT, cmp_op, scalar_nz_op) \
अटल bool funcname( स्थिर व्योम* _src, माप_प्रकार len ) \
{ \
    स्थिर T* src = (स्थिर T*)_src; \
    माप_प्रकार i = 0; \
    SIMD_ONLY( \
    स्थिर पूर्णांक vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    क्रम (i = 0; i + vlanes*8 <= len; i += vlanes*8) \
    { \
        VT x0 = vx_load(src + i); \
        VT x1 = vx_load(src + i + vlanes); \
        VT x2 = vx_load(src + i + vlanes*2); \
        VT x3 = vx_load(src + i + vlanes*3); \
        VT x4 = vx_load(src + i + vlanes*4); \
        VT x5 = vx_load(src + i + vlanes*5); \
        VT x6 = vx_load(src + i + vlanes*6); \
        VT x7 = vx_load(src + i + vlanes*7); \
        x0 = v_or(x0, x1); \
        x2 = v_or(x2, x3); \
        x4 = v_or(x4, x5); \
        x6 = v_or(x6, x7); \
        x0 = v_or(x0, x2); \
        x4 = v_or(x4, x6); \
        x0 = v_or(x0, x4); \
        x0 = cmp_op(x0, v_zero); \
        अगर (v_check_any(x0)) \
            वापस true; \
    } \
    क्रम (; i < len; i += vlanes) \
    { \
        अगर (i + vlanes > len) { \
            अगर (i == 0) \
                अवरोध; \
            i = len - vlanes; \
        } \
        VT x0 = vx_load(src + i); \
        x0 = cmp_op(x0, v_zero); \
        अगर (v_check_any(x0)) \
            वापस true; \
    } \
    v_cleanup();) \
    क्रम( ; i < len; i++ ) \
    { \
        T x = src[i]; \
        अगर (scalar_nz_op(x) != 0) \
            वापस true; \
    } \
    वापस false; \
}

#अघोषित CHECK_NZ_INT
#घोषणा CHECK_NZ_INT(x) ((x) != 0)
#अघोषित CHECK_NZ_FP
#घोषणा CHECK_NZ_FP(x) (((x)<<1) != 0)
#अघोषित CHECK_NZ_FP16
#घोषणा CHECK_NZ_FP16(x) (((x)&0x7fff) != 0)
#अघोषित VEC_CMP_EQ_Z_FP16
#घोषणा VEC_CMP_EQ_Z_FP16(x, z) v_ne(v_add_wrap(x, x), z)
#अघोषित VEC_CMP_EQ_Z_FP
#घोषणा VEC_CMP_EQ_Z_FP(x, z) v_ne(v_add(x, x), z)

DEFINE_HASNONZERO_FUNC(hasNonZero8u, u8, uchar, v_uint8, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero16u, u16, ushort, v_uint16, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero32s, s32, पूर्णांक, v_int32, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero64s, s64, int64, v_int64, v_ne, CHECK_NZ_INT)

DEFINE_HASNONZERO_FUNC(hasNonZero32f, s32, पूर्णांक, v_int32, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero64f, s64, int64, v_int64, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero16f, u16, ushort, v_uint16, VEC_CMP_EQ_Z_FP16, CHECK_NZ_FP16)

HasNonZeroFunc getHasNonZeroFunc(पूर्णांक depth)
{
    अटल HasNonZeroFunc hasNonZeroTab[CV_DEPTH_MAX] =
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

    वापस hasNonZeroTab[depth];
}

#पूर्ण_अगर

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
