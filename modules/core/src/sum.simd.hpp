// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "stat.hpp"

namespace cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, цео (*fn)(const T*, const uchar*, ST*, цео, цео)>
static цео sumWrap(const uchar* src, const uchar* mask, uchar* sum, цео len, цео cn)
{
    врати fn((const T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(цео depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
struct Sum_SIMD
{
    Sum_SIMD(цео) {}
    цео operator () (const T*, const uchar*, ST*, цео, цео) const
    {
        врати 0;
    }
};

#ако (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    ако (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    инако ако (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } инако ако (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } инако ако (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
static void init_maskbuf(ST* maskbuf, цео cn, цео simd_width)
{
    memset(maskbuf, 0, simd_width*9*sizeof(maskbuf[0]));
    ако (cn == 1)
        ;
    инако ако (cn == 2)
        за (цео i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    инако ако (cn == 3)
        за (цео i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    инако ако (cn == 4 && simd_width >= 4) {
        за (цео i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> struct Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(цео cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    цео operator ()(const T* src, const uchar* mask, ST* dst, цео len, цео cn) const \
    { \
        ако (mask || (cn < 1 || cn > 4)) \
            врати 0; \
        len *= cn; \
        цео x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ако (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } инако { \
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
        за (; x <= len - simd_width*6; x += simd_width*6) { \
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
        врати x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> struct Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(цео cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    цео operator ()(const T* src, const uchar* mask, ST* dst, цео len, цео cn) const \
    { \
        ако (mask || (cn < 1 || cn > 4)) \
            врати 0; \
        len *= cn; \
        цео x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ако (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } инако { \
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
        за (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        врати x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, цео, цео, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, цео, цео, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, цео, цео, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, цео, цео, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, float, цео, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, float, цео, v_float32, vx_load_expand)

#ако (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> struct Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(цео cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    цео operator ()(const T* src, const uchar* mask, ST* dst, цео len, цео cn) const \
    { \
        цео x = 0, simd_width = VTraits<VecT>::vlanes(); \
        ако (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            врати 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        ако (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } инако { \
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
        за (; x <= len - simd_width*6; x += simd_width*6) { \
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
        врати x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(цео, double, int64, v_float64)
DEFINE_SUM_SIMD_32(float, double, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
static цео sum_(const T* src0, const uchar* mask, ST* dst, цео len, цео cn )
{
    const T* src = src0;
    ако( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        цео i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        ако( k == 1 )
        {
            ST s0 = dst[0];

            #ако CV_ENABLE_UNROLLED
            за(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            за( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        инако ако( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            за( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        инако ако( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            за( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        за( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            за( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        врати len;
    }

    цео i, nzm = 0;
    ако( cn == 1 )
    {
        ST s = dst[0];
        за( i = 0; i < len; i++ )
            ако( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    инако ако( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        за( i = 0; i < len; i++, src += 3 )
            ако( mask[i] )
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
    инако
    {
        за( i = 0; i < len; i++, src += cn )
            ако( mask[i] )
            {
                цео k = 0;
                #ако CV_ENABLE_UNROLLED
                за( ; k <= cn - 4; k += 4 )
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
                за( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    врати nzm;
}


static цео sum8u( const uchar* src, const uchar* mask, цео* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_(src, mask, dst, len, cn); }

static цео sum8s( const schar* src, const uchar* mask, цео* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_(src, mask, dst, len, cn); }

static цео sum16u( const ushort* src, const uchar* mask, цео* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_(src, mask, dst, len, cn); }

static цео sum16s( const short* src, const uchar* mask, цео* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_(src, mask, dst, len, cn); }

static цео sum32u( const unsigned* src, const uchar* mask, double* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_<unsigned, double, uint64>(src, mask, dst, len, cn); }

static цео sum32s( const цео* src, const uchar* mask, double* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_<цео, double, int64>(src, mask, dst, len, cn); }

static цео sum64u( const uint64* src, const uchar* mask, double* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_(src, mask, dst, len, cn); }

static цео sum64s( const int64* src, const uchar* mask, double* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_(src, mask, dst, len, cn); }

static цео sum32f( const float* src, const uchar* mask, double* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_(src, mask, dst, len, cn); }

static цео sum64f( const double* src, const uchar* mask, double* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_(src, mask, dst, len, cn); }

static цео sum16f( const hfloat* src, const uchar* mask, float* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_<hfloat, float, float>(src, mask, dst, len, cn); }

static цео sum16bf( const bfloat* src, const uchar* mask, float* dst, цео len, цео cn )
{ CV_INSTRUMENT_REGION(); врати sum_<bfloat, float, float>(src, mask, dst, len, cn); }

SumFunc getSumFunc(цео depth)
{
    static SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, цео, sum8u>,
        sumWrap<schar, цео, sum8s>,
        sumWrap<ushort, цео, sum16u>,
        sumWrap<short, цео, sum16s>,
        sumWrap<цео, double, sum32s>,
        sumWrap<float, double, sum32f>,
        sumWrap<double, double, sum64f>,
        sumWrap<hfloat, float, sum16f>,
        sumWrap<bfloat, float, sum16bf>,
        0,
        sumWrap<uint64, double, sum64u>,
        sumWrap<int64, double, sum64s>,
        sumWrap<unsigned, double, sum32u>,
        0
    };

    врати sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
