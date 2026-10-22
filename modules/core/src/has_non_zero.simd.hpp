// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"

نطاق cv {

تعريفنوع منطقي (*HasNonZeroFunc)(ثابت فراغ*, size_t);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

HasNonZeroFunc getHasNonZeroFunc(صحيح ڈونگھائی);

#جے_بنا_تعریف CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#تعریف_رد SIMD_ONLY
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
#تعریف_کرو SIMD_ONLY(اظہاریہ) اظہاریہ
#وإلا
#تعریف_کرو SIMD_ONLY(اظہاریہ)
#جے_انت

#تعریف_رد DEFINE_HASNONZERO_FUNC
#تعریف_کرو DEFINE_HASNONZERO_FUNC(funcname, suffix, T, VT, cmp_op, scalar_nz_op) \
ساكن منطقي funcname( ثابت فراغ* _src, size_t len ) \
{ \
    ثابت T* src = (ثابت T*)_src; \
    size_t i = 0; \
    SIMD_ONLY( \
    ثابت صحيح vlanes = VTraits<VT>::vlanes(); \
    VT v_zero = vx_setzero_##suffix(); \
    لكل (i = 0; i + vlanes*8 <= len; i += vlanes*8) \
    { \
        VT x0 = vx_لدو(src + i); \
        VT x1 = vx_لدو(src + i + vlanes); \
        VT x2 = vx_لدو(src + i + vlanes*2); \
        VT x3 = vx_لدو(src + i + vlanes*3); \
        VT x4 = vx_لدو(src + i + vlanes*4); \
        VT x5 = vx_لدو(src + i + vlanes*5); \
        VT x6 = vx_لدو(src + i + vlanes*6); \
        VT x7 = vx_لدو(src + i + vlanes*7); \
        x0 = v_or(x0, x1); \
        x2 = v_or(x2, x3); \
        x4 = v_or(x4, x5); \
        x6 = v_or(x6, x7); \
        x0 = v_or(x0, x2); \
        x4 = v_or(x4, x6); \
        x0 = v_or(x0, x4); \
        x0 = cmp_op(x0, v_zero); \
        إذا (v_check_any(x0)) \
            أرجع سچ; \
    } \
    لكل (; i < len; i += vlanes) \
    { \
        إذا (i + vlanes > len) { \
            إذا (i == 0) \
                اكسر; \
            i = len - vlanes; \
        } \
        VT x0 = vx_لدو(src + i); \
        x0 = cmp_op(x0, v_zero); \
        إذا (v_check_any(x0)) \
            أرجع سچ; \
    } \
    v_cleanup();) \
    لكل( ; i < len; i++ ) \
    { \
        T x = src[i]; \
        إذا (scalar_nz_op(x) != 0) \
            أرجع سچ; \
    } \
    أرجع خطأ; \
}

#تعریف_رد CHECK_NZ_INT
#تعریف_کرو CHECK_NZ_INT(x) ((x) != 0)
#تعریف_رد CHECK_NZ_FP
#تعریف_کرو CHECK_NZ_FP(x) (((x)<<1) != 0)
#تعریف_رد CHECK_NZ_FP16
#تعریف_کرو CHECK_NZ_FP16(x) (((x)&0x7fff) != 0)
#تعریف_رد VEC_CMP_EQ_Z_FP16
#تعریف_کرو VEC_CMP_EQ_Z_FP16(x, z) v_ne(v_جوڑو_wrap(x, x), z)
#تعریف_رد VEC_CMP_EQ_Z_FP
#تعریف_کرو VEC_CMP_EQ_Z_FP(x, z) v_ne(v_جوڑو(x, x), z)

DEFINE_HASNONZERO_FUNC(hasNonZero8u, u8, uchar, v_uint8, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero16u, u16, ushort, v_uint16, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero32s, s32, صحيح, v_int32, v_ne, CHECK_NZ_INT)
DEFINE_HASNONZERO_FUNC(hasNonZero64s, s64, int64, v_int64, v_ne, CHECK_NZ_INT)

DEFINE_HASNONZERO_FUNC(hasNonZero32f, s32, صحيح, v_int32, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero64f, s64, int64, v_int64, VEC_CMP_EQ_Z_FP, CHECK_NZ_FP)
DEFINE_HASNONZERO_FUNC(hasNonZero16f, u16, ushort, v_uint16, VEC_CMP_EQ_Z_FP16, CHECK_NZ_FP16)

HasNonZeroFunc getHasNonZeroFunc(صحيح ڈونگھائی)
{
    ساكن HasNonZeroFunc hasNonZeroTab[CV_ڈونگھائی_MAX] =
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

    أرجع hasNonZeroTab[ڈونگھائی];
}

#جے_انت

CV_CPU_OPTIMIZATION_NAMESPACE_سرا
} // namespace
