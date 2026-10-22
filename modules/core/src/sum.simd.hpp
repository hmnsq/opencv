// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

ನಾಮಾಕಾಶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, ಪೂರ್ಣಾಙ್ಕ (*fn)(ಧ್ರುವ T*, ಧ್ರುವ uchar*, ST*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sumWrap(ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* mask, uchar* sum, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    ಪ್ರತ್ಯಾಗಮ fn((ಧ್ರುವ T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(ಪೂರ್ಣಾಙ್ಕ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
ಸಂರಚನಾ Sum_SIMD
{
    Sum_SIMD(ಪೂರ್ಣಾಙ್ಕ) {}
    ಪೂರ್ಣಾಙ್ಕ operator () (ಧ್ರುವ T*, ಧ್ರುವ uchar*, ST*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ) ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ 0;
    }
};

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    ಯದಿ (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    ಅನ್ಯಥಾ ಯದಿ (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } ಅನ್ಯಥಾ ಯದಿ (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } ಅನ್ಯಥಾ ಯದಿ (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
ಸ್ಥಿರ ಶೂನ್ಯ init_maskbuf(ST* maskbuf, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ simd_width)
{
    memset(maskbuf, 0, simd_width*9*ಪರಿಮಾಣ(maskbuf[0]));
    ಯದಿ (cn == 1)
        ;
    ಅನ್ಯಥಾ ಯದಿ (cn == 2)
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    ಅನ್ಯಥಾ ಯದಿ (cn == 3)
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    ಅನ್ಯಥಾ ಯದಿ (cn == 4 && simd_width >= 4) {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> ಸಂರಚನಾ Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ಪೂರ್ಣಾಙ್ಕ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ಪೂರ್ಣಾಙ್ಕ operator ()(ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ST* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ \
    { \
        ಯದಿ (mask || (cn < 1 || cn > 4)) \
            ಪ್ರತ್ಯಾಗಮ 0; \
        len *= cn; \
        ಪೂರ್ಣಾಙ್ಕ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ಯದಿ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ಅನ್ಯಥಾ { \
            m1 = vx_load(maskbuf + simd_width); \
            m2 = vx_load(maskbuf + simd_width*2); \
            m3 = vx_load(maskbuf + simd_width*3); \
            m4 = vx_load(maskbuf + simd_width*4); \
            m5 = vx_load(maskbuf + simd_width*5); \
            m6 = vx_load(maskbuf + simd_width*6); \
            m7 = vx_load(maskbuf + simd_width*7); \
            m8 = vx_load(maskbuf + simd_width*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ಪ್ರತಿ (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_ಕೆಳಗಿನ(v0)); \
            s1 = v_add(s1, v_expand_ಮೇಲಿನ(v0)); \
            s2 = v_add(s2, v_expand_ಕೆಳಗಿನ(v1)); \
            s0 = v_add(s0, v_expand_ಮೇಲಿನ(v1)); \
            s1 = v_add(s1, v_expand_ಕೆಳಗಿನ(v2)); \
            s2 = v_add(s2, v_expand_ಮೇಲಿನ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ಪ್ರತ್ಯಾಗಮ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> ಸಂರಚನಾ Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ಪೂರ್ಣಾಙ್ಕ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ಪೂರ್ಣಾಙ್ಕ operator ()(ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ST* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ \
    { \
        ಯದಿ (mask || (cn < 1 || cn > 4)) \
            ಪ್ರತ್ಯಾಗಮ 0; \
        len *= cn; \
        ಪೂರ್ಣಾಙ್ಕ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ಯದಿ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ಅನ್ಯಥಾ { \
            m1 = vx_load(maskbuf + simd_width); \
            m2 = vx_load(maskbuf + simd_width*2); \
            m3 = vx_load(maskbuf + simd_width*3); \
            m4 = vx_load(maskbuf + simd_width*4); \
            m5 = vx_load(maskbuf + simd_width*5); \
            m6 = vx_load(maskbuf + simd_width*6); \
            m7 = vx_load(maskbuf + simd_width*7); \
            m8 = vx_load(maskbuf + simd_width*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ಪ್ರತಿ (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ಪ್ರತ್ಯಾಗಮ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, ಭಿನ್ನ, ಪೂರ್ಣಾಙ್ಕ, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, ಭಿನ್ನ, ಪೂರ್ಣಾಙ್ಕ, v_float32, vx_load_expand)

#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> ಸಂರಚನಾ Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ಪೂರ್ಣಾಙ್ಕ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ಪೂರ್ಣಾಙ್ಕ operator ()(ಧ್ರುವ T* src, ಧ್ರುವ uchar* mask, ST* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn) ಧ್ರುವ \
    { \
        ಪೂರ್ಣಾಙ್ಕ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        ಯದಿ (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            ಪ್ರತ್ಯಾಗಮ 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ಯದಿ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ಅನ್ಯಥಾ { \
            m1 = vx_load(maskbuf + simd_width); \
            m2 = vx_load(maskbuf + simd_width*2); \
            m3 = vx_load(maskbuf + simd_width*3); \
            m4 = vx_load(maskbuf + simd_width*4); \
            m5 = vx_load(maskbuf + simd_width*5); \
            m6 = vx_load(maskbuf + simd_width*6); \
            m7 = vx_load(maskbuf + simd_width*7); \
            m8 = vx_load(maskbuf + simd_width*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        ಪ್ರತಿ (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_ಮೇಲಿನ(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_ಮೇಲಿನ(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_ಮೇಲಿನ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ಪ್ರತ್ಯಾಗಮ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, int64, v_float64)
DEFINE_SUM_SIMD_32(ಭಿನ್ನ, ದ್ವಿಗುಣ, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum_(ಧ್ರುವ T* src0, ಧ್ರುವ uchar* mask, ST* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    ಧ್ರುವ T* src = src0;
    ಯದಿ( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        ಪೂರ್ಣಾಙ್ಕ i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        ಯದಿ( k == 1 )
        {
            ST s0 = dst[0];

            #ಯದಿ CV_ENABLE_UNROLLED
            ಪ್ರತಿ(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            ಪ್ರತಿ( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        ಅನ್ಯಥಾ ಯದಿ( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            ಪ್ರತಿ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        ಅನ್ಯಥಾ ಯದಿ( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            ಪ್ರತಿ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        ಪ್ರತಿ( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            ಪ್ರತಿ( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        ಪ್ರತ್ಯಾಗಮ len;
    }

    ಪೂರ್ಣಾಙ್ಕ i, nzm = 0;
    ಯದಿ( cn == 1 )
    {
        ST s = dst[0];
        ಪ್ರತಿ( i = 0; i < len; i++ )
            ಯದಿ( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    ಅನ್ಯಥಾ ಯದಿ( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        ಪ್ರತಿ( i = 0; i < len; i++, src += 3 )
            ಯದಿ( mask[i] )
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
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( i = 0; i < len; i++, src += cn )
            ಯದಿ( mask[i] )
            {
                ಪೂರ್ಣಾಙ್ಕ k = 0;
                #ಯದಿ CV_ENABLE_UNROLLED
                ಪ್ರತಿ( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #endif
                ಪ್ರತಿ( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    ಪ್ರತ್ಯಾಗಮ nzm;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum8u( ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum8s( ಧ್ರುವ schar* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum16u( ಧ್ರುವ ushort* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum16s( ಧ್ರುವ short* src, ಧ್ರುವ uchar* mask, ಪೂರ್ಣಾಙ್ಕ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum32u( ಧ್ರುವ unsigned* src, ಧ್ರುವ uchar* mask, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_<unsigned, ದ್ವಿಗುಣ, uint64>(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum32s( ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಧ್ರುವ uchar* mask, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, int64>(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum64u( ಧ್ರುವ uint64* src, ಧ್ರುವ uchar* mask, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum64s( ಧ್ರುವ int64* src, ಧ್ರುವ uchar* mask, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum32f( ಧ್ರುವ ಭಿನ್ನ* src, ಧ್ರುವ uchar* mask, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum64f( ಧ್ರುವ ದ್ವಿಗುಣ* src, ಧ್ರುವ uchar* mask, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum16f( ಧ್ರುವ hfloat* src, ಧ್ರುವ uchar* mask, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_<hfloat, ಭಿನ್ನ, ಭಿನ್ನ>(src, mask, dst, len, cn); }

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sum16bf( ಧ್ರುವ bfloat* src, ಧ್ರುವ uchar* mask, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{ CV_INSTRUMENT_REGION(); ಪ್ರತ್ಯಾಗಮ sum_<bfloat, ಭಿನ್ನ, ಭಿನ್ನ>(src, mask, dst, len, cn); }

SumFunc getSumFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, ಪೂರ್ಣಾಙ್ಕ, sum8u>,
        sumWrap<schar, ಪೂರ್ಣಾಙ್ಕ, sum8s>,
        sumWrap<ushort, ಪೂರ್ಣಾಙ್ಕ, sum16u>,
        sumWrap<short, ಪೂರ್ಣಾಙ್ಕ, sum16s>,
        sumWrap<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, sum32s>,
        sumWrap<ಭಿನ್ನ, ದ್ವಿಗುಣ, sum32f>,
        sumWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, sum64f>,
        sumWrap<hfloat, ಭಿನ್ನ, sum16f>,
        sumWrap<bfloat, ಭಿನ್ನ, sum16bf>,
        0,
        sumWrap<uint64, ದ್ವಿಗುಣ, sum64u>,
        sumWrap<int64, ದ್ವಿಗುಣ, sum64s>,
        sumWrap<unsigned, ದ್ವಿಗುಣ, sum32u>,
        0
    };

    ಪ್ರತ್ಯಾಗಮ sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
