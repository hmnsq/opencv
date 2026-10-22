// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "stat.hpp"

नामाकाश cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, पूर्णाङ्क (*fn)(ध्रुव T*, ध्रुव uchar*, ST*, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर पूर्णाङ्क sumWrap(ध्रुव uchar* src, ध्रुव uchar* mask, uchar* sum, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    प्रत्यागम fn((ध्रुव T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(पूर्णाङ्क depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
संरचना Sum_SIMD
{
    Sum_SIMD(पूर्णाङ्क) {}
    पूर्णाङ्क operator () (ध्रुव T*, ध्रुव uchar*, ST*, पूर्णाङ्क, पूर्णाङ्क) ध्रुव
    {
        प्रत्यागम 0;
    }
};

#जइ (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    जइ (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    अण्णहा जइ (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } अण्णहा जइ (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } अण्णहा जइ (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
स्थिर शून्य init_maskbuf(ST* maskbuf, पूर्णाङ्क cn, पूर्णाङ्क simd_width)
{
    memset(maskbuf, 0, simd_width*9*परिमाण(maskbuf[0]));
    जइ (cn == 1)
        ;
    अण्णहा जइ (cn == 2)
        प्रति (पूर्णाङ्क i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    अण्णहा जइ (cn == 3)
        प्रति (पूर्णाङ्क i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    अण्णहा जइ (cn == 4 && simd_width >= 4) {
        प्रति (पूर्णाङ्क i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> संरचना Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(पूर्णाङ्क cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णाङ्क operator ()(ध्रुव T* src, ध्रुव uchar* mask, ST* dst, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव \
    { \
        जइ (mask || (cn < 1 || cn > 4)) \
            प्रत्यागम 0; \
        len *= cn; \
        पूर्णाङ्क x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        जइ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अण्णहा { \
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
        प्रति (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_नीचः(v0)); \
            s1 = v_add(s1, v_expand_उच्चः(v0)); \
            s2 = v_add(s2, v_expand_नीचः(v1)); \
            s0 = v_add(s0, v_expand_उच्चः(v1)); \
            s1 = v_add(s1, v_expand_नीचः(v2)); \
            s2 = v_add(s2, v_expand_उच्चः(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        प्रत्यागम x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> संरचना Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(पूर्णाङ्क cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णाङ्क operator ()(ध्रुव T* src, ध्रुव uchar* mask, ST* dst, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव \
    { \
        जइ (mask || (cn < 1 || cn > 4)) \
            प्रत्यागम 0; \
        len *= cn; \
        पूर्णाङ्क x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        जइ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अण्णहा { \
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
        प्रति (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        प्रत्यागम x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, पूर्णाङ्क, पूर्णाङ्क, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, पूर्णाङ्क, पूर्णाङ्क, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, पूर्णाङ्क, पूर्णाङ्क, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, पूर्णाङ्क, पूर्णाङ्क, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, भिन्न, पूर्णाङ्क, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, भिन्न, पूर्णाङ्क, v_float32, vx_load_expand)

#जइ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> संरचना Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(पूर्णाङ्क cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    पूर्णाङ्क operator ()(ध्रुव T* src, ध्रुव uchar* mask, ST* dst, पूर्णाङ्क len, पूर्णाङ्क cn) ध्रुव \
    { \
        पूर्णाङ्क x = 0, simd_width = VTraits<VecT>::vlanes(); \
        जइ (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            प्रत्यागम 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        जइ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } अण्णहा { \
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
        प्रति (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_उच्चः(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_उच्चः(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_उच्चः(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        प्रत्यागम x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(पूर्णाङ्क, द्विगुण, int64, v_float64)
DEFINE_SUM_SIMD_32(भिन्न, द्विगुण, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
स्थिर पूर्णाङ्क sum_(ध्रुव T* src0, ध्रुव uchar* mask, ST* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    ध्रुव T* src = src0;
    जइ( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        पूर्णाङ्क i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        जइ( k == 1 )
        {
            ST s0 = dst[0];

            #जइ CV_ENABLE_UNROLLED
            प्रति(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            प्रति( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        अण्णहा जइ( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            प्रति( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        अण्णहा जइ( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            प्रति( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        प्रति( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            प्रति( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        प्रत्यागम len;
    }

    पूर्णाङ्क i, nzm = 0;
    जइ( cn == 1 )
    {
        ST s = dst[0];
        प्रति( i = 0; i < len; i++ )
            जइ( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    अण्णहा जइ( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        प्रति( i = 0; i < len; i++, src += 3 )
            जइ( mask[i] )
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
    अण्णहा
    {
        प्रति( i = 0; i < len; i++, src += cn )
            जइ( mask[i] )
            {
                पूर्णाङ्क k = 0;
                #जइ CV_ENABLE_UNROLLED
                प्रति( ; k <= cn - 4; k += 4 )
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
                प्रति( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    प्रत्यागम nzm;
}


स्थिर पूर्णाङ्क sum8u( ध्रुव uchar* src, ध्रुव uchar* mask, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum8s( ध्रुव schar* src, ध्रुव uchar* mask, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum16u( ध्रुव ushort* src, ध्रुव uchar* mask, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum16s( ध्रुव short* src, ध्रुव uchar* mask, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum32u( ध्रुव unsigned* src, ध्रुव uchar* mask, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_<unsigned, द्विगुण, uint64>(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum32s( ध्रुव पूर्णाङ्क* src, ध्रुव uchar* mask, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_<पूर्णाङ्क, द्विगुण, int64>(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum64u( ध्रुव uint64* src, ध्रुव uchar* mask, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum64s( ध्रुव int64* src, ध्रुव uchar* mask, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum32f( ध्रुव भिन्न* src, ध्रुव uchar* mask, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum64f( ध्रुव द्विगुण* src, ध्रुव uchar* mask, द्विगुण* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum16f( ध्रुव hfloat* src, ध्रुव uchar* mask, भिन्न* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_<hfloat, भिन्न, भिन्न>(src, mask, dst, len, cn); }

स्थिर पूर्णाङ्क sum16bf( ध्रुव bfloat* src, ध्रुव uchar* mask, भिन्न* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{ CV_INSTRUMENT_REGION(); प्रत्यागम sum_<bfloat, भिन्न, भिन्न>(src, mask, dst, len, cn); }

SumFunc getSumFunc(पूर्णाङ्क depth)
{
    स्थिर SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, पूर्णाङ्क, sum8u>,
        sumWrap<schar, पूर्णाङ्क, sum8s>,
        sumWrap<ushort, पूर्णाङ्क, sum16u>,
        sumWrap<short, पूर्णाङ्क, sum16s>,
        sumWrap<पूर्णाङ्क, द्विगुण, sum32s>,
        sumWrap<भिन्न, द्विगुण, sum32f>,
        sumWrap<द्विगुण, द्विगुण, sum64f>,
        sumWrap<hfloat, भिन्न, sum16f>,
        sumWrap<bfloat, भिन्न, sum16bf>,
        0,
        sumWrap<uint64, द्विगुण, sum64u>,
        sumWrap<int64, द्विगुण, sum64s>,
        sumWrap<unsigned, द्विगुण, sum32u>,
        0
    };

    प्रत्यागम sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
