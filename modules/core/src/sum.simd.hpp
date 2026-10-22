// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "stat.hpp"

நாமாகாஶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, முழு (*fn)(மாறிலி T*, மாறிலி uchar*, ST*, முழு, முழு)>
நிலையான முழு sumWrap(மாறிலி uchar* src, மாறிலி uchar* mask, uchar* sum, முழு len, முழு cn)
{
    திரும்பு fn((மாறிலி T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(முழு depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
கட்டமைப்பு Sum_SIMD
{
    Sum_SIMD(முழு) {}
    முழு operator () (மாறிலி T*, மாறிலி uchar*, ST*, முழு, முழு) மாறிலி
    {
        திரும்பு 0;
    }
};

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    எனில் (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    இல்லையெனில் எனில் (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } இல்லையெனில் எனில் (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } இல்லையெனில் எனில் (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
நிலையான வெறுமை init_maskbuf(ST* maskbuf, முழு cn, முழு simd_width)
{
    memset(maskbuf, 0, simd_width*9*அளவு(maskbuf[0]));
    எனில் (cn == 1)
        ;
    இல்லையெனில் எனில் (cn == 2)
        க்கு (முழு i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    இல்லையெனில் எனில் (cn == 3)
        க்கு (முழு i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    இல்லையெனில் எனில் (cn == 4 && simd_width >= 4) {
        க்கு (முழு i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> கட்டமைப்பு Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(முழு cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    முழு operator ()(மாறிலி T* src, மாறிலி uchar* mask, ST* dst, முழு len, முழு cn) மாறிலி \
    { \
        எனில் (mask || (cn < 1 || cn > 4)) \
            திரும்பு 0; \
        len *= cn; \
        முழு x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        எனில் (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } இல்லையெனில் { \
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
        க்கு (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_கீழ்(v0)); \
            s1 = v_add(s1, v_expand_உயர்வு(v0)); \
            s2 = v_add(s2, v_expand_கீழ்(v1)); \
            s0 = v_add(s0, v_expand_உயர்வு(v1)); \
            s1 = v_add(s1, v_expand_கீழ்(v2)); \
            s2 = v_add(s2, v_expand_உயர்வு(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        திரும்பு x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> கட்டமைப்பு Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(முழு cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    முழு operator ()(மாறிலி T* src, மாறிலி uchar* mask, ST* dst, முழு len, முழு cn) மாறிலி \
    { \
        எனில் (mask || (cn < 1 || cn > 4)) \
            திரும்பு 0; \
        len *= cn; \
        முழு x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        எனில் (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } இல்லையெனில் { \
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
        க்கு (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        திரும்பு x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, முழு, முழு, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, முழு, முழு, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, முழு, முழு, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, முழு, முழு, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, தசம, முழு, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, தசம, முழு, v_float32, vx_load_expand)

#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> கட்டமைப்பு Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(முழு cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    முழு operator ()(மாறிலி T* src, மாறிலி uchar* mask, ST* dst, முழு len, முழு cn) மாறிலி \
    { \
        முழு x = 0, simd_width = VTraits<VecT>::vlanes(); \
        எனில் (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            திரும்பு 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        எனில் (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } இல்லையெனில் { \
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
        க்கு (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_உயர்வு(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_உயர்வு(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_உயர்வு(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        திரும்பு x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(முழு, இரட்டை, int64, v_float64)
DEFINE_SUM_SIMD_32(தசம, இரட்டை, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
நிலையான முழு sum_(மாறிலி T* src0, மாறிலி uchar* mask, ST* dst, முழு len, முழு cn )
{
    மாறிலி T* src = src0;
    எனில்( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        முழு i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        எனில்( k == 1 )
        {
            ST s0 = dst[0];

            #எனில் CV_ENABLE_UNROLLED
            க்கு(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            க்கு( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        இல்லையெனில் எனில்( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            க்கு( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        இல்லையெனில் எனில்( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            க்கு( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        க்கு( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            க்கு( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        திரும்பு len;
    }

    முழு i, nzm = 0;
    எனில்( cn == 1 )
    {
        ST s = dst[0];
        க்கு( i = 0; i < len; i++ )
            எனில்( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    இல்லையெனில் எனில்( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        க்கு( i = 0; i < len; i++, src += 3 )
            எனில்( mask[i] )
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
    இல்லையெனில்
    {
        க்கு( i = 0; i < len; i++, src += cn )
            எனில்( mask[i] )
            {
                முழு k = 0;
                #எனில் CV_ENABLE_UNROLLED
                க்கு( ; k <= cn - 4; k += 4 )
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
                க்கு( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    திரும்பு nzm;
}


நிலையான முழு sum8u( மாறிலி uchar* src, மாறிலி uchar* mask, முழு* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_(src, mask, dst, len, cn); }

நிலையான முழு sum8s( மாறிலி schar* src, மாறிலி uchar* mask, முழு* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_(src, mask, dst, len, cn); }

நிலையான முழு sum16u( மாறிலி ushort* src, மாறிலி uchar* mask, முழு* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_(src, mask, dst, len, cn); }

நிலையான முழு sum16s( மாறிலி short* src, மாறிலி uchar* mask, முழு* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_(src, mask, dst, len, cn); }

நிலையான முழு sum32u( மாறிலி unsigned* src, மாறிலி uchar* mask, இரட்டை* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_<unsigned, இரட்டை, uint64>(src, mask, dst, len, cn); }

நிலையான முழு sum32s( மாறிலி முழு* src, மாறிலி uchar* mask, இரட்டை* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_<முழு, இரட்டை, int64>(src, mask, dst, len, cn); }

நிலையான முழு sum64u( மாறிலி uint64* src, மாறிலி uchar* mask, இரட்டை* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_(src, mask, dst, len, cn); }

நிலையான முழு sum64s( மாறிலி int64* src, மாறிலி uchar* mask, இரட்டை* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_(src, mask, dst, len, cn); }

நிலையான முழு sum32f( மாறிலி தசம* src, மாறிலி uchar* mask, இரட்டை* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_(src, mask, dst, len, cn); }

நிலையான முழு sum64f( மாறிலி இரட்டை* src, மாறிலி uchar* mask, இரட்டை* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_(src, mask, dst, len, cn); }

நிலையான முழு sum16f( மாறிலி hfloat* src, மாறிலி uchar* mask, தசம* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_<hfloat, தசம, தசம>(src, mask, dst, len, cn); }

நிலையான முழு sum16bf( மாறிலி bfloat* src, மாறிலி uchar* mask, தசம* dst, முழு len, முழு cn )
{ CV_INSTRUMENT_REGION(); திரும்பு sum_<bfloat, தசம, தசம>(src, mask, dst, len, cn); }

SumFunc getSumFunc(முழு depth)
{
    நிலையான SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, முழு, sum8u>,
        sumWrap<schar, முழு, sum8s>,
        sumWrap<ushort, முழு, sum16u>,
        sumWrap<short, முழு, sum16s>,
        sumWrap<முழு, இரட்டை, sum32s>,
        sumWrap<தசம, இரட்டை, sum32f>,
        sumWrap<இரட்டை, இரட்டை, sum64f>,
        sumWrap<hfloat, தசம, sum16f>,
        sumWrap<bfloat, தசம, sum16bf>,
        0,
        sumWrap<uint64, இரட்டை, sum64u>,
        sumWrap<int64, இரட்டை, sum64s>,
        sumWrap<unsigned, இரட்டை, sum32u>,
        0
    };

    திரும்பு sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
