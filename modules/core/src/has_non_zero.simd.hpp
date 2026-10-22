// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"

नामाकाश cv {

प्रकारनिर्देश तार्किक (*HasNonZeroFunc)(ध्रुव शून्य*, size_t);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

HasNonZeroFunc getHasNonZeroFunc(पूर्णाङ्क गभीरता);

#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#अपरिभाषय SIMD_ONLY
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
#परिभाषय SIMD_ONLY(व्यञ्जकम्) व्यञ्जकम्
#अन्यथा
#परिभाषय SIMD_ONLY(व्यञ्जकम्)
#यदि_अन्तः

#अपरिभाषय DEFINE_HASNONZERO_FUNC
#परिभाषय DEFINE_HASNONZERO_FUNC(funcname, suffix, T, VT, cmp_op, scalar_nz_op) \
स्थिर तार्किक funcname( ध्रुव शून्य* _src, size_t len ) \
{ \
    ध्रुव T* src = (ध्रुव T*)_src; \
    size_t i = 0; \
    SIMD_ONLY( \
    ध्रुव पूर्णाङ्क vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    प्रति (i = 0; i + vlanes*8 <= len; i += vlanes*8) \
    { \
        VT x0 = vx_आरोपय(src + i); \
        VT x1 = vx_आरोपय(src + i + vlanes); \
        VT x2 = vx_आरोपय(src + i + vlanes*2); \
        VT x3 = vx_आरोपय(src + i + vlanes*3); \
        VT x4 = vx_आरोपय(src + i + vlanes*4); \
        VT x5 = vx_आरोपय(src + i + vlanes*5); \
        VT x6 = vx_आरोपय(src + i + vlanes*6); \
        VT x7 = vx_आरोपय(src + i + vlanes*7); \
        x0 = v_or(x0, x1); \
        x2 = v_or(x2, x3); \
        x4 = v_or(x4, x5); \
        x6 = v_or(x6, x7); \
        x0 = v_or(x0, x2); \
        x4 = v_or(x4, x6); \
        x0 = v_or(x0, x4); \
        x0 = cmp_op(x0, v_zero); \
        यदि (v_check_any(x0)) \
            प्रत्यागम सत्य; \
    } \
    प्रति (; i < len; i += vlanes) \
    { \
        यदि (i + vlanes > len) { \
            यदि (i == 0) \
                भङ्ग; \
            i = len - vlanes; \
        } \
        VT x0 = vx_आरोपय(src + i); \
        x0 = cmp_op(x0, v_zero); \
        यदि (v_check_any(x0)) \
            प्रत्यागम सत्य; \
    } \
    v_cleanup();) \
    प्रति( ; i < len; i++ ) \
    { \
        T x = src[i]; \
        यदि (scalar_nz_op(x) != 0) \
            प्रत्यागम सत्य; \
    } \
    प्रत्यागम असत्य; \
}

#अपरिभाषय CHECK_NZ_INT
#परिभाषय CHECK_NZ_INT(x) ((x) != 0)
#अपरिभाषय CHECK_NZ_FP
#परिभाषय CHECK_NZ_FP(x) (((x)<<1) != 0)
#अपरिभाषय CHECK_NZ_FP16
#परिभाषय CHECK_NZ_FP16(x) (((x)&0x7fff) != 0)
#अपरिभाषय VEC_CMP_EQ_Z_FP16
#परिभाषय VEC_CMP_EQ_Z_FP16(x, z) v_ne(v_सङ्कलय_wrap(x, x), z)
#अपरिभाषय VEC_CMP_EQ_Z_FP
#परिभाषय VEC_CMP_EQ_Z_FP(x, z) v_ne(v_सङ्कलय(x, x), z)

DEFINE_HASNONZERO_FUNC(hasNonZero8u, u8, uchar, v_uint8, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero16u, u16, ushort, v_uint16, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero32s, s32, पूर्णाङ्क, v_int32, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero64s, s64, int64, v_int64, v_ne, CHECK_NZ_INT)

DEFINE_HASNONZERO_FUNC(hasNonZero32f, s32, पूर्णाङ्क, v_int32, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero64f, s64, int64, v_int64, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero16f, u16, ushort, v_uint16, VEC_CMP_EQ_Z_FP16, CHECK_NZ_FP16)

HasNonZeroFunc getHasNonZeroFunc(पूर्णाङ्क गभीरता)
{
    स्थिर HasNonZeroFunc hasNonZeroTab[CV_गभीरता_MAX] =
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

    प्रत्यागम hasNonZeroTab[गभीरता];
}

#यदि_अन्तः

CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
} // namespace
