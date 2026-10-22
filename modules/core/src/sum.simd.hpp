// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "stat.hpp"

namespace cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, ཧྲིལ (*fn)(const T*, const uchar*, ST*, ཧྲིལ, ཧྲིལ)>
static ཧྲིལ sumWrap(const uchar* src, const uchar* mask, uchar* sum, ཧྲིལ len, ཧྲིལ cn)
{
    ཕྱིར fn((const T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(ཧྲིལ depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
sa_nracanaa Sum_SIMD
{
    Sum_SIMD(ཧྲིལ) {}
    ཧྲིལ operator () (const T*, const uchar*, ST*, ཧྲིལ, ཧྲིལ) const
    {
        ཕྱིར 0;
    }
};

#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    གལ་ཏེ (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    དེ་མིན གལ་ཏེ (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } དེ་མིན གལ་ཏེ (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } དེ་མིན གལ་ཏེ (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
static void init_maskbuf(ST* maskbuf, ཧྲིལ cn, ཧྲིལ simd_width)
{
    memset(maskbuf, 0, simd_width*9*parimaa_na(maskbuf[0]));
    གལ་ཏེ (cn == 1)
        ;
    དེ་མིན གལ་ཏེ (cn == 2)
        ཆེད་དུ (ཧྲིལ i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    དེ་མིན གལ་ཏེ (cn == 3)
        ཆེད་དུ (ཧྲིལ i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    དེ་མིན གལ་ཏེ (cn == 4 && simd_width >= 4) {
        ཆེད་དུ (ཧྲིལ i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> sa_nracanaa Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ཧྲིལ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ཧྲིལ operator ()(const T* src, const uchar* mask, ST* dst, ཧྲིལ len, ཧྲིལ cn) const \
    { \
        གལ་ཏེ (mask || (cn < 1 || cn > 4)) \
            ཕྱིར 0; \
        len *= cn; \
        ཧྲིལ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        གལ་ཏེ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } དེ་མིན { \
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
        ཆེད་དུ (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_low(v0)); \
            s1 = v_add(s1, v_expand_high(v0)); \
            s2 = v_add(s2, v_expand_low(v1)); \
            s0 = v_add(s0, v_expand_high(v1)); \
            s1 = v_add(s1, v_expand_low(v2)); \
            s2 = v_add(s2, v_expand_high(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ཕྱིར x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> sa_nracanaa Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ཧྲིལ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ཧྲིལ operator ()(const T* src, const uchar* mask, ST* dst, ཧྲིལ len, ཧྲིལ cn) const \
    { \
        གལ་ཏེ (mask || (cn < 1 || cn > 4)) \
            ཕྱིར 0; \
        len *= cn; \
        ཧྲིལ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        གལ་ཏེ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } དེ་མིན { \
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
        ཆེད་དུ (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ཕྱིར x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, ཧྲིལ, ཧྲིལ, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, ཧྲིལ, ཧྲིལ, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, ཧྲིལ, ཧྲིལ, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, ཧྲིལ, ཧྲིལ, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, float, ཧྲིལ, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, float, ཧྲིལ, v_float32, vx_load_expand)

#གལ་ཏེ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> sa_nracanaa Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(ཧྲིལ cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    ཧྲིལ operator ()(const T* src, const uchar* mask, ST* dst, ཧྲིལ len, ཧྲིལ cn) const \
    { \
        ཧྲིལ x = 0, simd_width = VTraits<VecT>::vlanes(); \
        གལ་ཏེ (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            ཕྱིར 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        གལ་ཏེ (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } དེ་མིན { \
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
        ཆེད་དུ (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_high(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_high(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_high(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ཕྱིར x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(ཧྲིལ, dviguu_na, int64, v_float64)
DEFINE_SUM_SIMD_32(float, dviguu_na, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
static ཧྲིལ sum_(const T* src0, const uchar* mask, ST* dst, ཧྲིལ len, ཧྲིལ cn )
{
    const T* src = src0;
    གལ་ཏེ( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        ཧྲིལ i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        གལ་ཏེ( k == 1 )
        {
            ST s0 = dst[0];

            #གལ་ཏེ CV_ENABLE_UNROLLED
            ཆེད་དུ(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            ཆེད་དུ( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        དེ་མིན གལ་ཏེ( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            ཆེད་དུ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        དེ་མིན གལ་ཏེ( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            ཆེད་དུ( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        ཆེད་དུ( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            ཆེད་དུ( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        ཕྱིར len;
    }

    ཧྲིལ i, nzm = 0;
    གལ་ཏེ( cn == 1 )
    {
        ST s = dst[0];
        ཆེད་དུ( i = 0; i < len; i++ )
            གལ་ཏེ( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    དེ་མིན གལ་ཏེ( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        ཆེད་དུ( i = 0; i < len; i++, src += 3 )
            གལ་ཏེ( mask[i] )
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
    དེ་མིན
    {
        ཆེད་དུ( i = 0; i < len; i++, src += cn )
            གལ་ཏེ( mask[i] )
            {
                ཧྲིལ k = 0;
                #གལ་ཏེ CV_ENABLE_UNROLLED
                ཆེད་དུ( ; k <= cn - 4; k += 4 )
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
                ཆེད་དུ( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    ཕྱིར nzm;
}


static ཧྲིལ sum8u( const uchar* src, const uchar* mask, ཧྲིལ* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_(src, mask, dst, len, cn); }

static ཧྲིལ sum8s( const schar* src, const uchar* mask, ཧྲིལ* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_(src, mask, dst, len, cn); }

static ཧྲིལ sum16u( const ushort* src, const uchar* mask, ཧྲིལ* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_(src, mask, dst, len, cn); }

static ཧྲིལ sum16s( const short* src, const uchar* mask, ཧྲིལ* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_(src, mask, dst, len, cn); }

static ཧྲིལ sum32u( const unsigned* src, const uchar* mask, dviguu_na* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_<unsigned, dviguu_na, uint64>(src, mask, dst, len, cn); }

static ཧྲིལ sum32s( const ཧྲིལ* src, const uchar* mask, dviguu_na* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_<ཧྲིལ, dviguu_na, int64>(src, mask, dst, len, cn); }

static ཧྲིལ sum64u( const uint64* src, const uchar* mask, dviguu_na* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_(src, mask, dst, len, cn); }

static ཧྲིལ sum64s( const int64* src, const uchar* mask, dviguu_na* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_(src, mask, dst, len, cn); }

static ཧྲིལ sum32f( const float* src, const uchar* mask, dviguu_na* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_(src, mask, dst, len, cn); }

static ཧྲིལ sum64f( const dviguu_na* src, const uchar* mask, dviguu_na* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_(src, mask, dst, len, cn); }

static ཧྲིལ sum16f( const hfloat* src, const uchar* mask, float* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_<hfloat, float, float>(src, mask, dst, len, cn); }

static ཧྲིལ sum16bf( const bfloat* src, const uchar* mask, float* dst, ཧྲིལ len, ཧྲིལ cn )
{ CV_INSTRUMENT_REGION(); ཕྱིར sum_<bfloat, float, float>(src, mask, dst, len, cn); }

SumFunc getSumFunc(ཧྲིལ depth)
{
    static SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, ཧྲིལ, sum8u>,
        sumWrap<schar, ཧྲིལ, sum8s>,
        sumWrap<ushort, ཧྲིལ, sum16u>,
        sumWrap<short, ཧྲིལ, sum16s>,
        sumWrap<ཧྲིལ, dviguu_na, sum32s>,
        sumWrap<float, dviguu_na, sum32f>,
        sumWrap<dviguu_na, dviguu_na, sum64f>,
        sumWrap<hfloat, float, sum16f>,
        sumWrap<bfloat, float, sum16bf>,
        0,
        sumWrap<uint64, dviguu_na, sum64u>,
        sumWrap<int64, dviguu_na, sum64s>,
        sumWrap<unsigned, dviguu_na, sum32u>,
        0
    };

    ཕྱིར sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
