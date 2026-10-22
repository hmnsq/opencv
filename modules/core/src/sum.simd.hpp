// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "stat.hpp"

ନାମାକାଶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*fn)(ଅଚଳ T*, ଅଚଳ uchar*, ST*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sumWrap(ଅଚଳ uchar* src, ଅଚଳ uchar* mask, uchar* sum, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ଫେରନ୍ତୁ fn((ଅଚଳ T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
ସଂରଚନା Sum_SIMD
{
    Sum_SIMD(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) {}
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ T*, ଅଚଳ uchar*, ST*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଅଚଳ
    {
        ଫେରନ୍ତୁ 0;
    }
};

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    ଯଦି (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    ଅନ୍ୟଥା ଯଦି (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } ଅନ୍ୟଥା ଯଦି (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } ଅନ୍ୟଥା ଯଦି (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
ସ୍ଥିର ଶୂନ୍ୟ init_maskbuf(ST* maskbuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା simd_width)
{
    memset(maskbuf, 0, simd_width*9*ଆକାର(maskbuf[0]));
    ଯଦି (cn == 1)
        ;
    ଅନ୍ୟଥା ଯଦି (cn == 2)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    ଅନ୍ୟଥା ଯଦି (cn == 3)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    ଅନ୍ୟଥା ଯଦି (cn == 4 && simd_width >= 4) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> ସଂରଚନା Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator ()(ଅଚଳ T* src, ଅଚଳ uchar* mask, ST* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ \
    { \
        ଯଦି (mask || (cn < 1 || cn > 4)) \
            ଫେରନ୍ତୁ 0; \
        len *= cn; \
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ଯଦି (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ଅନ୍ୟଥା { \
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
        ପାଇଁ (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_ତଳ(v0)); \
            s1 = v_add(s1, v_expand_ଉପର(v0)); \
            s2 = v_add(s2, v_expand_ତଳ(v1)); \
            s0 = v_add(s0, v_expand_ଉପର(v1)); \
            s1 = v_add(s1, v_expand_ତଳ(v2)); \
            s2 = v_add(s2, v_expand_ଉପର(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ଫେରନ୍ତୁ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> ସଂରଚନା Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator ()(ଅଚଳ T* src, ଅଚଳ uchar* mask, ST* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ \
    { \
        ଯଦି (mask || (cn < 1 || cn > 4)) \
            ଫେରନ୍ତୁ 0; \
        len *= cn; \
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ଯଦି (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ଅନ୍ୟଥା { \
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
        ପାଇଁ (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ଫେରନ୍ତୁ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, ଦଶମିକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, ଦଶମିକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_float32, vx_load_expand)

#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> ସଂରଚନା Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator ()(ଅଚଳ T* src, ଅଚଳ uchar* mask, ST* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn) ଅଚଳ \
    { \
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0, simd_width = VTraits<VecT>::vlanes(); \
        ଯଦି (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            ଫେରନ୍ତୁ 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ଯଦି (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ଅନ୍ୟଥା { \
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
        ପାଇଁ (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_ଉପର(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_ଉପର(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_ଉପର(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ଫେରନ୍ତୁ x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, int64, v_float64)
DEFINE_SUM_SIMD_32(ଦଶମିକ, ଦ୍ୱିଗୁଣ, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum_(ଅଚଳ T* src0, ଅଚଳ uchar* mask, ST* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    ଅଚଳ T* src = src0;
    ଯଦି( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        ଯଦି( k == 1 )
        {
            ST s0 = dst[0];

            #ଯଦି CV_ENABLE_UNROLLED
            ପାଇଁ(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            ପାଇଁ( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        ଅନ୍ୟଥା ଯଦି( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            ପାଇଁ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        ଅନ୍ୟଥା ଯଦି( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            ପାଇଁ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        ପାଇଁ( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            ପାଇଁ( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        ଫେରନ୍ତୁ len;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, nzm = 0;
    ଯଦି( cn == 1 )
    {
        ST s = dst[0];
        ପାଇଁ( i = 0; i < len; i++ )
            ଯଦି( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    ଅନ୍ୟଥା ଯଦି( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        ପାଇଁ( i = 0; i < len; i++, src += 3 )
            ଯଦି( mask[i] )
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
    ଅନ୍ୟଥା
    {
        ପାଇଁ( i = 0; i < len; i++, src += cn )
            ଯଦି( mask[i] )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
                #ଯଦି CV_ENABLE_UNROLLED
                ପାଇଁ( ; k <= cn - 4; k += 4 )
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
                ପାଇଁ( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    ଫେରନ୍ତୁ nzm;
}


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum8u( ଅଚଳ uchar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum8s( ଅଚଳ schar* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum16u( ଅଚଳ ushort* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum16s( ଅଚଳ short* src, ଅଚଳ uchar* mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum32u( ଅଚଳ unsigned* src, ଅଚଳ uchar* mask, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_<unsigned, ଦ୍ୱିଗୁଣ, uint64>(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum32s( ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ଅଚଳ uchar* mask, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, int64>(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum64u( ଅଚଳ uint64* src, ଅଚଳ uchar* mask, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum64s( ଅଚଳ int64* src, ଅଚଳ uchar* mask, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum32f( ଅଚଳ ଦଶମିକ* src, ଅଚଳ uchar* mask, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum64f( ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଅଚଳ uchar* mask, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum16f( ଅଚଳ hfloat* src, ଅଚଳ uchar* mask, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_<hfloat, ଦଶମିକ, ଦଶମିକ>(src, mask, dst, len, cn); }

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sum16bf( ଅଚଳ bfloat* src, ଅଚଳ uchar* mask, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{ CV_INSTRUMENT_REGION(); ଫେରନ୍ତୁ sum_<bfloat, ଦଶମିକ, ଦଶମିକ>(src, mask, dst, len, cn); }

SumFunc getSumFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sum8u>,
        sumWrap<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sum8s>,
        sumWrap<ushort, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sum16u>,
        sumWrap<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sum16s>,
        sumWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, sum32s>,
        sumWrap<ଦଶମିକ, ଦ୍ୱିଗୁଣ, sum32f>,
        sumWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, sum64f>,
        sumWrap<hfloat, ଦଶମିକ, sum16f>,
        sumWrap<bfloat, ଦଶମିକ, sum16bf>,
        0,
        sumWrap<uint64, ଦ୍ୱିଗୁଣ, sum64u>,
        sumWrap<int64, ଦ୍ୱିଗୁଣ, sum64s>,
        sumWrap<unsigned, ଦ୍ୱିଗୁଣ, sum32u>,
        0
    };

    ଫେରନ୍ତୁ sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
