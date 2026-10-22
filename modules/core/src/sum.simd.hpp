// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

ನಾಮಾಕಾಶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ಪೂರ್ಣಾಂಕ (*fn)(ಸ್ಥಿರಾಂಕ T*, ಸ್ಥಿರಾಂಕ uchar*, ST*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_Wrap(ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, uchar* ಮೊತ್ತ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    ಹಿಂತಿರುಗಿ fn((ಸ್ಥಿರಾಂಕ T*)src, ಮುಖವಾಡ, (ST*)ಮೊತ್ತ, len, cn);
}

ಮೊತ್ತ_Func get_ಮೊತ್ತ_Func(ಪೂರ್ಣಾಂಕ ಆಳ);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST>
ರಚನೆ ಮೊತ್ತ_SIMD
{
    ಮೊತ್ತ_SIMD(ಪೂರ್ಣಾಂಕ) {}
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ T*, ಸ್ಥಿರಾಂಕ uchar*, ST*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ 0;
    }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು REDUCE_PARTIAL_SUMS
#ವ್ಯಾಖ್ಯಾನಿಸು REDUCE_PARTIAL_SUMS() \
    ಆದರೆ (cn == 1) \
        dst[0] += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(s0, s1), s2)); \
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 2) { \
        s0 = v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(s0, s1), s2); \
        dst[0] += v_reduce_ಮೊತ್ತ(v_and(s0, m0)); \
        dst[1] += v_reduce_ಮೊತ್ತ(v_and(s0, m1)); \
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 3) { \
        dst[0] += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_ಮೊತ್ತ(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 4) { \
        s0 = v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(s0, s1), s2); \
        dst[0] += v_reduce_ಮೊತ್ತ(v_and(s0, m0)); \
        dst[1] += v_reduce_ಮೊತ್ತ(v_and(s0, m1)); \
        dst[2] += v_reduce_ಮೊತ್ತ(v_and(s0, m2)); \
        dst[3] += v_reduce_ಮೊತ್ತ(v_and(s0, m3)); \
    }

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ST>
ಸ್ಥಿರ ಶೂನ್ಯ init_maskbuf(ST* maskbuf, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ simd_ಅಗಲ)
{
    memset(maskbuf, 0, simd_ಅಗಲ*9*ಗಾತ್ರ(maskbuf[0]));
    ಆದರೆ (cn == 1)
        ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 2)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < simd_ಅಗಲ; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ಅಗಲ] = (ST)-1;
        }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 3)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < simd_ಅಗಲ*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ಅಗಲ*3] = (ST)-1;
            maskbuf[i+2+simd_ಅಗಲ*6] = (ST)-1;
        }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 4 && simd_ಅಗಲ >= 4) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < simd_ಅಗಲ; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ಅಗಲ] = (ST)-1;
            maskbuf[i+2+simd_ಅಗಲ*2] = (ST)-1;
            maskbuf[i+3+simd_ಅಗಲ*3] = (ST)-1;
        }
    }
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_ಮೊತ್ತ_SIMD_8
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_ಮೊತ್ತ_SIMD_8(T, ST, iST, VecT, ತುಂಬು_op) \
ಮಾದರಿ<> ರಚನೆ ಮೊತ್ತ_SIMD<T, ST> \
{ \
    ಮೊತ್ತ_SIMD(ಪೂರ್ಣಾಂಕ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ \
    { \
        ಆದರೆ (ಮುಖವಾಡ || (cn < 1 || cn > 4)) \
            ಹಿಂತಿರುಗಿ 0; \
        len *= cn; \
        ಪೂರ್ಣಾಂಕ x = 0, simd_ಅಗಲ = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_ತುಂಬು(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ಆದರೆ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ಇಲ್ಲದಿದ್ದರೆ { \
            m1 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ); \
            m2 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*2); \
            m3 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*3); \
            m4 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*4); \
            m5 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*5); \
            m6 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*6); \
            m7 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*7); \
            m8 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ಗಾಗಿ (; x <= len - simd_ಅಗಲ*6; x += simd_ಅಗಲ*6) { \
            ಸ್ವಯಂಚಾಲಿತ v0 = ತುಂಬು_op(src + x); \
            ಸ್ವಯಂಚಾಲಿತ v1 = ತುಂಬು_op(src + x + simd_ಅಗಲ*2); \
            ಸ್ವಯಂಚಾಲಿತ v2 = ತುಂಬು_op(src + x + simd_ಅಗಲ*4); \
            s0 = v_ಕೂಡಿಸು(s0, v_expand_ಕೆಳಗಿನ(v0)); \
            s1 = v_ಕೂಡಿಸು(s1, v_expand_ಮೇಲಿನ(v0)); \
            s2 = v_ಕೂಡಿಸು(s2, v_expand_ಕೆಳಗಿನ(v1)); \
            s0 = v_ಕೂಡಿಸು(s0, v_expand_ಮೇಲಿನ(v1)); \
            s1 = v_ಕೂಡಿಸು(s1, v_expand_ಕೆಳಗಿನ(v2)); \
            s2 = v_ಕೂಡಿಸು(s2, v_expand_ಮೇಲಿನ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ಹಿಂತಿರುಗಿ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_ಮೊತ್ತ_SIMD_16
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_ಮೊತ್ತ_SIMD_16(T, ST, iST, VecT, ತುಂಬು_op) \
ಮಾದರಿ<> ರಚನೆ ಮೊತ್ತ_SIMD<T, ST> \
{ \
    ಮೊತ್ತ_SIMD(ಪೂರ್ಣಾಂಕ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ \
    { \
        ಆದರೆ (ಮುಖವಾಡ || (cn < 1 || cn > 4)) \
            ಹಿಂತಿರುಗಿ 0; \
        len *= cn; \
        ಪೂರ್ಣಾಂಕ x = 0, simd_ಅಗಲ = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_ತುಂಬು(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ಆದರೆ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ಇಲ್ಲದಿದ್ದರೆ { \
            m1 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ); \
            m2 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*2); \
            m3 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*3); \
            m4 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*4); \
            m5 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*5); \
            m6 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*6); \
            m7 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*7); \
            m8 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ಗಾಗಿ (; x <= len - simd_ಅಗಲ*3; x += simd_ಅಗಲ*3) { \
            ಸ್ವಯಂಚಾಲಿತ v0 = ತುಂಬು_op(src + x); \
            ಸ್ವಯಂಚಾಲಿತ v1 = ತುಂಬು_op(src + x + simd_ಅಗಲ); \
            ಸ್ವಯಂಚಾಲಿತ v2 = ತುಂಬು_op(src + x + simd_ಅಗಲ*2); \
            s0 = v_ಕೂಡಿಸು(s0, v0); \
            s1 = v_ಕೂಡಿಸು(s1, v1); \
            s2 = v_ಕೂಡಿಸು(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ಹಿಂತಿರುಗಿ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ತುಂಬು_u8_as_s16
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ತುಂಬು_u16_as_s32
#ವ್ಯಾಖ್ಯಾನಿಸು ತುಂಬು_u8_as_s16(addr) v_reinterpret_as_s16(vx_ತುಂಬು_expand(addr))
#ವ್ಯಾಖ್ಯಾನಿಸು ತುಂಬು_u16_as_s32(addr) v_reinterpret_as_s32(vx_ತುಂಬು_expand(addr))

DEFINE_ಮೊತ್ತ_SIMD_8(uchar, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, v_int32, ತುಂಬು_u8_as_s16)
DEFINE_ಮೊತ್ತ_SIMD_8(schar, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, v_int32, vx_ತುಂಬು_expand)
DEFINE_ಮೊತ್ತ_SIMD_16(ushort, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, v_int32, ತುಂಬು_u16_as_s32)
DEFINE_ಮೊತ್ತ_SIMD_16(ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, v_int32, vx_ತುಂಬು_expand)
DEFINE_ಮೊತ್ತ_SIMD_16(hfloat, ದಶಮಾಂಶ, ಪೂರ್ಣಾಂಕ, v_float32, vx_ತುಂಬು_expand)
DEFINE_ಮೊತ್ತ_SIMD_16(bfloat, ದಶಮಾಂಶ, ಪೂರ್ಣಾಂಕ, v_float32, vx_ತುಂಬು_expand)

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE_ಮೊತ್ತ_SIMD_32
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_ಮೊತ್ತ_SIMD_32(T, ST, iST, VecT) \
ಮಾದರಿ<> ರಚನೆ ಮೊತ್ತ_SIMD<T, ST> \
{ \
    ಮೊತ್ತ_SIMD(ಪೂರ್ಣಾಂಕ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) ಸ್ಥಿರಾಂಕ \
    { \
        ಪೂರ್ಣಾಂಕ x = 0, simd_ಅಗಲ = VTraits<VecT>::vlanes(); \
        ಆದರೆ (ಮುಖವಾಡ || (cn < 1 || cn > 3+(simd_ಅಗಲ>=4))) \
            ಹಿಂತಿರುಗಿ 0; \
        len *= cn; \
        VecT m0 = vx_ತುಂಬು(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ಆದರೆ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ಇಲ್ಲದಿದ್ದರೆ { \
            m1 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ); \
            m2 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*2); \
            m3 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*3); \
            m4 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*4); \
            m5 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*5); \
            m6 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*6); \
            m7 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*7); \
            m8 = vx_ತುಂಬು(maskbuf + simd_ಅಗಲ*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ಗಾಗಿ (; x <= len - simd_ಅಗಲ*6; x += simd_ಅಗಲ*6) { \
            ಸ್ವಯಂಚಾಲಿತ v0 = vx_ತುಂಬು(src + x); \
            ಸ್ವಯಂಚಾಲಿತ v1 = vx_ತುಂಬು(src + x + simd_ಅಗಲ*2); \
            ಸ್ವಯಂಚಾಲಿತ v2 = vx_ತುಂಬು(src + x + simd_ಅಗಲ*4); \
            s0 = v_ಕೂಡಿಸು(s0, v_cvt_f64(v0)); \
            s1 = v_ಕೂಡಿಸು(s1, v_cvt_f64_ಮೇಲಿನ(v0)); \
            s2 = v_ಕೂಡಿಸು(s2, v_cvt_f64(v1)); \
            s0 = v_ಕೂಡಿಸು(s0, v_cvt_f64_ಮೇಲಿನ(v1)); \
            s1 = v_ಕೂಡಿಸು(s1, v_cvt_f64(v2)); \
            s2 = v_ಕೂಡಿಸು(s2, v_cvt_f64_ಮೇಲಿನ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ಹಿಂತಿರುಗಿ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_ಮೊತ್ತ_SIMD_32(ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, int64, v_float64)
DEFINE_ಮೊತ್ತ_SIMD_32(ದಶಮಾಂಶ, ದ್ವಿಗುಣ, int64, v_float64)
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು ST, ಬಗೆ_ಹೆಸರು WT=T>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_2(ಸ್ಥಿರಾಂಕ T* src0, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ST* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    ಸ್ಥಿರಾಂಕ T* src = src0;
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಮೊತ್ತ_SIMD<T, ST> vop(cn);
        ಪೂರ್ಣಾಂಕ i0 = vop(src0, ಮುಖವಾಡ, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        ಆದರೆ( k == 1 )
        {
            ST s0 = dst[0];

            #ಆದರೆ CV_ENABLE_UNROLLED
            ಗಾಗಿ(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            ಗಾಗಿ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            ಗಾಗಿ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        ಗಾಗಿ( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            ಗಾಗಿ( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        ಹಿಂತಿರುಗಿ len;
    }

    ಪೂರ್ಣಾಂಕ i, nzm = 0;
    ಆದರೆ( cn == 1 )
    {
        ST s = dst[0];
        ಗಾಗಿ( i = 0; i < len; i++ )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        ಗಾಗಿ( i = 0; i < len; i++, src += 3 )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
                nzm++;
            }
        dst[0] = s0;
        dst[1] = s1;
        dst[2] = s2;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( i = 0; i < len; i++, src += cn )
            ಆದರೆ( ಮುಖವಾಡ[i] )
            {
                ಪೂರ್ಣಾಂಕ k = 0;
                #ಆದರೆ CV_ENABLE_UNROLLED
                ಗಾಗಿ( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #ಆದರೆ_ಅಂತ್ಯ
                ಗಾಗಿ( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    ಹಿಂತಿರುಗಿ nzm;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum8u( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum8s( ಸ್ಥಿರಾಂಕ schar* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum16u( ಸ್ಥಿರಾಂಕ ushort* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum16s( ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum32u( ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, uint64>(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum32s( ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, int64>(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum64u( ಸ್ಥಿರಾಂಕ uint64* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum64s( ಸ್ಥಿರಾಂಕ int64* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum32f( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum64f( ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum16f( ಸ್ಥಿರಾಂಕ hfloat* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2<hfloat, ದಶಮಾಂಶ, ದಶಮಾಂಶ>(src, ಮುಖವಾಡ, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sum16bf( ಸ್ಥಿರಾಂಕ bfloat* src, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{ CV_INSTRUMENT_REGION(); ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2<bfloat, ದಶಮಾಂಶ, ದಶಮಾಂಶ>(src, ಮುಖವಾಡ, dst, len, cn); }

ಮೊತ್ತ_Func get_ಮೊತ್ತ_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ಮೊತ್ತ_Func ಮೊತ್ತ_Tab[CV_ಆಳ_MAX] =
    {
        ಮೊತ್ತ_Wrap<uchar, ಪೂರ್ಣಾಂಕ, sum8u>,
        ಮೊತ್ತ_Wrap<schar, ಪೂರ್ಣಾಂಕ, sum8s>,
        ಮೊತ್ತ_Wrap<ushort, ಪೂರ್ಣಾಂಕ, sum16u>,
        ಮೊತ್ತ_Wrap<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ, sum16s>,
        ಮೊತ್ತ_Wrap<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, sum32s>,
        ಮೊತ್ತ_Wrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, sum32f>,
        ಮೊತ್ತ_Wrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, sum64f>,
        ಮೊತ್ತ_Wrap<hfloat, ದಶಮಾಂಶ, sum16f>,
        ಮೊತ್ತ_Wrap<bfloat, ದಶಮಾಂಶ, sum16bf>,
        0,
        ಮೊತ್ತ_Wrap<uint64, ದ್ವಿಗುಣ, sum64u>,
        ಮೊತ್ತ_Wrap<int64, ದ್ವಿಗುಣ, sum64s>,
        ಮೊತ್ತ_Wrap<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, sum32u>,
        0
    };

    ಹಿಂತಿರುಗಿ ಮೊತ್ತ_Tab[ಆಳ];
}

#ಆದರೆ_ಅಂತ್ಯ

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace
