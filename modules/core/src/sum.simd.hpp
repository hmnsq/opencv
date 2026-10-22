// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"

ਨਾਮਾਕਾਸ਼ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਪੂਰਨ (*fn)(ਅਚਲ T*, ਅਚਲ uchar*, ST*, ਪੂਰਨ, ਪੂਰਨ)>
ਸਥਿਰ ਪੂਰਨ ਜੋੜ_Wrap(ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, uchar* ਜੋੜ, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    ਵਾਪਸ fn((ਅਚਲ T*)src, ਨਕਾਬ, (ST*)ਜੋੜ, len, cn);
}

ਜੋੜ_Func get_ਜੋੜ_Func(ਪੂਰਨ ਡੂੰਘਾਈ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST>
ਬਣਤਰ ਜੋੜ_SIMD
{
    ਜੋੜ_SIMD(ਪੂਰਨ) {}
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ T*, ਅਚਲ uchar*, ST*, ਪੂਰਨ, ਪੂਰਨ) ਅਚਲ
    {
        ਵਾਪਸ 0;
    }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ REDUCE_PARTIAL_SUMS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REDUCE_PARTIAL_SUMS() \
    ਜੇ (cn == 1) \
        dst[0] += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(s0, s1), s2)); \
    ਨਹੀਂਤਾਂ ਜੇ (cn == 2) { \
        s0 = v_ਜੋੜੋ(v_ਜੋੜੋ(s0, s1), s2); \
        dst[0] += v_reduce_ਜੋੜ(v_and(s0, m0)); \
        dst[1] += v_reduce_ਜੋੜ(v_and(s0, m1)); \
    } ਨਹੀਂਤਾਂ ਜੇ (cn == 3) { \
        dst[0] += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_ਜੋੜ(v_ਜੋੜੋ(v_ਜੋੜੋ(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } ਨਹੀਂਤਾਂ ਜੇ (cn == 4) { \
        s0 = v_ਜੋੜੋ(v_ਜੋੜੋ(s0, s1), s2); \
        dst[0] += v_reduce_ਜੋੜ(v_and(s0, m0)); \
        dst[1] += v_reduce_ਜੋੜ(v_and(s0, m1)); \
        dst[2] += v_reduce_ਜੋੜ(v_and(s0, m2)); \
        dst[3] += v_reduce_ਜੋੜ(v_and(s0, m3)); \
    }

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ST>
ਸਥਿਰ ਖਾਲੀ init_maskbuf(ST* maskbuf, ਪੂਰਨ cn, ਪੂਰਨ simd_ਚੌੜਾਈ)
{
    memset(maskbuf, 0, simd_ਚੌੜਾਈ*9*ਆਕਾਰ(maskbuf[0]));
    ਜੇ (cn == 1)
        ;
    ਨਹੀਂਤਾਂ ਜੇ (cn == 2)
        ਲਈ (ਪੂਰਨ i = 0; i < simd_ਚੌੜਾਈ; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ਚੌੜਾਈ] = (ST)-1;
        }
    ਨਹੀਂਤਾਂ ਜੇ (cn == 3)
        ਲਈ (ਪੂਰਨ i = 0; i < simd_ਚੌੜਾਈ*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ਚੌੜਾਈ*3] = (ST)-1;
            maskbuf[i+2+simd_ਚੌੜਾਈ*6] = (ST)-1;
        }
    ਨਹੀਂਤਾਂ ਜੇ (cn == 4 && simd_ਚੌੜਾਈ >= 4) {
        ਲਈ (ਪੂਰਨ i = 0; i < simd_ਚੌੜਾਈ; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ਚੌੜਾਈ] = (ST)-1;
            maskbuf[i+2+simd_ਚੌੜਾਈ*2] = (ST)-1;
            maskbuf[i+3+simd_ਚੌੜਾਈ*3] = (ST)-1;
        }
    }
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_ਜੋੜ_SIMD_8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਜੋੜ_SIMD_8(T, ST, iST, VecT, ਲੱਦੋ_op) \
ਸਾਂਚਾ<> ਬਣਤਰ ਜੋੜ_SIMD<T, ST> \
{ \
    ਜੋੜ_SIMD(ਪੂਰਨ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ਪੂਰਨ ਸੰਚਾਲਕ ()(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ST* dst, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ \
    { \
        ਜੇ (ਨਕਾਬ || (cn < 1 || cn > 4)) \
            ਵਾਪਸ 0; \
        len *= cn; \
        ਪੂਰਨ x = 0, simd_ਚੌੜਾਈ = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_ਲੱਦੋ(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ਜੇ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ਨਹੀਂਤਾਂ { \
            m1 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ); \
            m2 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*2); \
            m3 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*3); \
            m4 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*4); \
            m5 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*5); \
            m6 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*6); \
            m7 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*7); \
            m8 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ਲਈ (; x <= len - simd_ਚੌੜਾਈ*6; x += simd_ਚੌੜਾਈ*6) { \
            ਸਵੈਚਾਲਿਤ v0 = ਲੱਦੋ_op(src + x); \
            ਸਵੈਚਾਲਿਤ v1 = ਲੱਦੋ_op(src + x + simd_ਚੌੜਾਈ*2); \
            ਸਵੈਚਾਲਿਤ v2 = ਲੱਦੋ_op(src + x + simd_ਚੌੜਾਈ*4); \
            s0 = v_ਜੋੜੋ(s0, v_expand_ਨੀਵਾਂ(v0)); \
            s1 = v_ਜੋੜੋ(s1, v_expand_ਉੱਚਾ(v0)); \
            s2 = v_ਜੋੜੋ(s2, v_expand_ਨੀਵਾਂ(v1)); \
            s0 = v_ਜੋੜੋ(s0, v_expand_ਉੱਚਾ(v1)); \
            s1 = v_ਜੋੜੋ(s1, v_expand_ਨੀਵਾਂ(v2)); \
            s2 = v_ਜੋੜੋ(s2, v_expand_ਉੱਚਾ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ਵਾਪਸ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_ਜੋੜ_SIMD_16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਜੋੜ_SIMD_16(T, ST, iST, VecT, ਲੱਦੋ_op) \
ਸਾਂਚਾ<> ਬਣਤਰ ਜੋੜ_SIMD<T, ST> \
{ \
    ਜੋੜ_SIMD(ਪੂਰਨ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ਪੂਰਨ ਸੰਚਾਲਕ ()(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ST* dst, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ \
    { \
        ਜੇ (ਨਕਾਬ || (cn < 1 || cn > 4)) \
            ਵਾਪਸ 0; \
        len *= cn; \
        ਪੂਰਨ x = 0, simd_ਚੌੜਾਈ = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_ਲੱਦੋ(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ਜੇ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ਨਹੀਂਤਾਂ { \
            m1 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ); \
            m2 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*2); \
            m3 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*3); \
            m4 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*4); \
            m5 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*5); \
            m6 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*6); \
            m7 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*7); \
            m8 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ਲਈ (; x <= len - simd_ਚੌੜਾਈ*3; x += simd_ਚੌੜਾਈ*3) { \
            ਸਵੈਚਾਲਿਤ v0 = ਲੱਦੋ_op(src + x); \
            ਸਵੈਚਾਲਿਤ v1 = ਲੱਦੋ_op(src + x + simd_ਚੌੜਾਈ); \
            ਸਵੈਚਾਲਿਤ v2 = ਲੱਦੋ_op(src + x + simd_ਚੌੜਾਈ*2); \
            s0 = v_ਜੋੜੋ(s0, v0); \
            s1 = v_ਜੋੜੋ(s1, v1); \
            s2 = v_ਜੋੜੋ(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ਵਾਪਸ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਲੱਦੋ_u8_as_s16
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਲੱਦੋ_u16_as_s32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਲੱਦੋ_u8_as_s16(addr) v_reinterpret_as_s16(vx_ਲੱਦੋ_expand(addr))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਲੱਦੋ_u16_as_s32(addr) v_reinterpret_as_s32(vx_ਲੱਦੋ_expand(addr))

DEFINE_ਜੋੜ_SIMD_8(uchar, ਪੂਰਨ, ਪੂਰਨ, v_int32, ਲੱਦੋ_u8_as_s16)
DEFINE_ਜੋੜ_SIMD_8(schar, ਪੂਰਨ, ਪੂਰਨ, v_int32, vx_ਲੱਦੋ_expand)
DEFINE_ਜੋੜ_SIMD_16(ushort, ਪੂਰਨ, ਪੂਰਨ, v_int32, ਲੱਦੋ_u16_as_s32)
DEFINE_ਜੋੜ_SIMD_16(ਛੋਟਾ, ਪੂਰਨ, ਪੂਰਨ, v_int32, vx_ਲੱਦੋ_expand)
DEFINE_ਜੋੜ_SIMD_16(hfloat, ਦਸ਼ਮਲਵ, ਪੂਰਨ, v_float32, vx_ਲੱਦੋ_expand)
DEFINE_ਜੋੜ_SIMD_16(bfloat, ਦਸ਼ਮਲਵ, ਪੂਰਨ, v_float32, vx_ਲੱਦੋ_expand)

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEFINE_ਜੋੜ_SIMD_32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਜੋੜ_SIMD_32(T, ST, iST, VecT) \
ਸਾਂਚਾ<> ਬਣਤਰ ਜੋੜ_SIMD<T, ST> \
{ \
    ਜੋੜ_SIMD(ਪੂਰਨ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ਪੂਰਨ ਸੰਚਾਲਕ ()(ਅਚਲ T* src, ਅਚਲ uchar* ਨਕਾਬ, ST* dst, ਪੂਰਨ len, ਪੂਰਨ cn) ਅਚਲ \
    { \
        ਪੂਰਨ x = 0, simd_ਚੌੜਾਈ = VTraits<VecT>::vlanes(); \
        ਜੇ (ਨਕਾਬ || (cn < 1 || cn > 3+(simd_ਚੌੜਾਈ>=4))) \
            ਵਾਪਸ 0; \
        len *= cn; \
        VecT m0 = vx_ਲੱਦੋ(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ਜੇ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ਨਹੀਂਤਾਂ { \
            m1 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ); \
            m2 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*2); \
            m3 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*3); \
            m4 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*4); \
            m5 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*5); \
            m6 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*6); \
            m7 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*7); \
            m8 = vx_ਲੱਦੋ(maskbuf + simd_ਚੌੜਾਈ*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ਲਈ (; x <= len - simd_ਚੌੜਾਈ*6; x += simd_ਚੌੜਾਈ*6) { \
            ਸਵੈਚਾਲਿਤ v0 = vx_ਲੱਦੋ(src + x); \
            ਸਵੈਚਾਲਿਤ v1 = vx_ਲੱਦੋ(src + x + simd_ਚੌੜਾਈ*2); \
            ਸਵੈਚਾਲਿਤ v2 = vx_ਲੱਦੋ(src + x + simd_ਚੌੜਾਈ*4); \
            s0 = v_ਜੋੜੋ(s0, v_cvt_f64(v0)); \
            s1 = v_ਜੋੜੋ(s1, v_cvt_f64_ਉੱਚਾ(v0)); \
            s2 = v_ਜੋੜੋ(s2, v_cvt_f64(v1)); \
            s0 = v_ਜੋੜੋ(s0, v_cvt_f64_ਉੱਚਾ(v1)); \
            s1 = v_ਜੋੜੋ(s1, v_cvt_f64(v2)); \
            s2 = v_ਜੋੜੋ(s2, v_cvt_f64_ਉੱਚਾ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ਵਾਪਸ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_ਜੋੜ_SIMD_32(ਪੂਰਨ, ਦੁੱਗਣਾ, int64, v_float64)
DEFINE_ਜੋੜ_SIMD_32(ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, int64, v_float64)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਕਿਸਮ_ਨਾਂ WT=T>
ਸਥਿਰ ਪੂਰਨ ਜੋੜ_2(ਅਚਲ T* src0, ਅਚਲ uchar* ਨਕਾਬ, ST* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    ਅਚਲ T* src = src0;
    ਜੇ( !ਨਕਾਬ )
    {
        ਜੋੜ_SIMD<T, ST> vop(cn);
        ਪੂਰਨ i0 = vop(src0, ਨਕਾਬ, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        ਜੇ( k == 1 )
        {
            ST s0 = dst[0];

            #ਜੇ CV_ENABLE_UNROLLED
            ਲਈ(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #ਜੇ_ਅੰਤ
            ਲਈ( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        ਨਹੀਂਤਾਂ ਜੇ( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            ਲਈ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        ਨਹੀਂਤਾਂ ਜੇ( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            ਲਈ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        ਲਈ( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            ਲਈ( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        ਵਾਪਸ len;
    }

    ਪੂਰਨ i, nzm = 0;
    ਜੇ( cn == 1 )
    {
        ST s = dst[0];
        ਲਈ( i = 0; i < len; i++ )
            ਜੇ( ਨਕਾਬ[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    ਨਹੀਂਤਾਂ ਜੇ( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        ਲਈ( i = 0; i < len; i++, src += 3 )
            ਜੇ( ਨਕਾਬ[i] )
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
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( i = 0; i < len; i++, src += cn )
            ਜੇ( ਨਕਾਬ[i] )
            {
                ਪੂਰਨ k = 0;
                #ਜੇ CV_ENABLE_UNROLLED
                ਲਈ( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #ਜੇ_ਅੰਤ
                ਲਈ( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    ਵਾਪਸ nzm;
}


ਸਥਿਰ ਪੂਰਨ sum8u( ਅਚਲ uchar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum8s( ਅਚਲ schar* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum16u( ਅਚਲ ushort* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum16s( ਅਚਲ ਛੋਟਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਪੂਰਨ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum32u( ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, uint64>(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum32s( ਅਚਲ ਪੂਰਨ* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2<ਪੂਰਨ, ਦੁੱਗਣਾ, int64>(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum64u( ਅਚਲ uint64* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum64s( ਅਚਲ int64* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum32f( ਅਚਲ ਦਸ਼ਮਲਵ* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum64f( ਅਚਲ ਦੁੱਗਣਾ* src, ਅਚਲ uchar* ਨਕਾਬ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum16f( ਅਚਲ hfloat* src, ਅਚਲ uchar* ਨਕਾਬ, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2<hfloat, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>(src, ਨਕਾਬ, dst, len, cn); }

ਸਥਿਰ ਪੂਰਨ sum16bf( ਅਚਲ bfloat* src, ਅਚਲ uchar* ਨਕਾਬ, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{ CV_INSTRUMENT_REGION(); ਵਾਪਸ ਜੋੜ_2<bfloat, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>(src, ਨਕਾਬ, dst, len, cn); }

ਜੋੜ_Func get_ਜੋੜ_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਜੋੜ_Func ਜੋੜ_Tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        ਜੋੜ_Wrap<uchar, ਪੂਰਨ, sum8u>,
        ਜੋੜ_Wrap<schar, ਪੂਰਨ, sum8s>,
        ਜੋੜ_Wrap<ushort, ਪੂਰਨ, sum16u>,
        ਜੋੜ_Wrap<ਛੋਟਾ, ਪੂਰਨ, sum16s>,
        ਜੋੜ_Wrap<ਪੂਰਨ, ਦੁੱਗਣਾ, sum32s>,
        ਜੋੜ_Wrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, sum32f>,
        ਜੋੜ_Wrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, sum64f>,
        ਜੋੜ_Wrap<hfloat, ਦਸ਼ਮਲਵ, sum16f>,
        ਜੋੜ_Wrap<bfloat, ਦਸ਼ਮਲਵ, sum16bf>,
        0,
        ਜੋੜ_Wrap<uint64, ਦੁੱਗਣਾ, sum64u>,
        ਜੋੜ_Wrap<int64, ਦੁੱਗਣਾ, sum64s>,
        ਜੋੜ_Wrap<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, sum32u>,
        0
    };

    ਵਾਪਸ ਜੋੜ_Tab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਅੰਤ

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace
