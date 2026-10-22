// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "stat.hpp"

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, صحيح (*fn)(ثابت T*, ثابت uchar*, ST*, صحيح, صحيح)>
ساكن صحيح sumWrap(ثابت uchar* src, ثابت uchar* mask, uchar* sum, صحيح len, صحيح cn)
{
    החזר fn((ثابت T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(صحيح depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
بنية Sum_SIMD
{
    Sum_SIMD(صحيح) {}
    صحيح operator () (ثابت T*, ثابت uchar*, ST*, صحيح, صحيح) ثابت
    {
        החזר 0;
    }
};

#אם (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    אם (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    אחרת אם (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } אחרת אם (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } אחרת אם (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
ساكن فراغ init_maskbuf(ST* maskbuf, صحيح cn, صحيح simd_width)
{
    memset(maskbuf, 0, simd_width*9*حجم(maskbuf[0]));
    אם (cn == 1)
        ;
    אחרת אם (cn == 2)
        עבור (صحيح i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    אחרת אם (cn == 3)
        עבור (صحيح i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    אחרת אם (cn == 4 && simd_width >= 4) {
        עבור (صحيح i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> بنية Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح operator ()(ثابت T* src, ثابت uchar* mask, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        אם (mask || (cn < 1 || cn > 4)) \
            החזר 0; \
        len *= cn; \
        صحيح x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        אם (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } אחרת { \
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
        עבור (; x <= len - simd_width*6; x += simd_width*6) { \
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
        החזר x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> بنية Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح operator ()(ثابت T* src, ثابت uchar* mask, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        אם (mask || (cn < 1 || cn > 4)) \
            החזר 0; \
        len *= cn; \
        صحيح x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        אם (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } אחרת { \
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
        עבור (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        החזר x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, صحيح, صحيح, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, صحيح, صحيح, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, صحيح, صحيح, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, صحيح, صحيح, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, عائم, صحيح, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, عائم, صحيح, v_float32, vx_load_expand)

#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> بنية Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح operator ()(ثابت T* src, ثابت uchar* mask, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        صحيح x = 0, simd_width = VTraits<VecT>::vlanes(); \
        אם (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            החזר 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        אם (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } אחרת { \
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
        עבור (; x <= len - simd_width*6; x += simd_width*6) { \
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
        החזר x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(صحيح, مزدوج, int64, v_float64)
DEFINE_SUM_SIMD_32(عائم, مزدوج, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
ساكن صحيح sum_(ثابت T* src0, ثابت uchar* mask, ST* dst, صحيح len, صحيح cn )
{
    ثابت T* src = src0;
    אם( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        صحيح i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        אם( k == 1 )
        {
            ST s0 = dst[0];

            #אם CV_ENABLE_UNROLLED
            עבור(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            עבור( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        אחרת אם( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            עבור( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        אחרת אם( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            עבור( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        עבור( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            עבור( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        החזר len;
    }

    صحيح i, nzm = 0;
    אם( cn == 1 )
    {
        ST s = dst[0];
        עבור( i = 0; i < len; i++ )
            אם( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    אחרת אם( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        עבור( i = 0; i < len; i++, src += 3 )
            אם( mask[i] )
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
    אחרת
    {
        עבור( i = 0; i < len; i++, src += cn )
            אם( mask[i] )
            {
                صحيح k = 0;
                #אם CV_ENABLE_UNROLLED
                עבור( ; k <= cn - 4; k += 4 )
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
                עבור( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    החזר nzm;
}


ساكن صحيح sum8u( ثابت uchar* src, ثابت uchar* mask, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_(src, mask, dst, len, cn); }

ساكن صحيح sum8s( ثابت schar* src, ثابت uchar* mask, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_(src, mask, dst, len, cn); }

ساكن صحيح sum16u( ثابت ushort* src, ثابت uchar* mask, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_(src, mask, dst, len, cn); }

ساكن صحيح sum16s( ثابت short* src, ثابت uchar* mask, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_(src, mask, dst, len, cn); }

ساكن صحيح sum32u( ثابت unsigned* src, ثابت uchar* mask, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_<unsigned, مزدوج, uint64>(src, mask, dst, len, cn); }

ساكن صحيح sum32s( ثابت صحيح* src, ثابت uchar* mask, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_<صحيح, مزدوج, int64>(src, mask, dst, len, cn); }

ساكن صحيح sum64u( ثابت uint64* src, ثابت uchar* mask, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_(src, mask, dst, len, cn); }

ساكن صحيح sum64s( ثابت int64* src, ثابت uchar* mask, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_(src, mask, dst, len, cn); }

ساكن صحيح sum32f( ثابت عائم* src, ثابت uchar* mask, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_(src, mask, dst, len, cn); }

ساكن صحيح sum64f( ثابت مزدوج* src, ثابت uchar* mask, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_(src, mask, dst, len, cn); }

ساكن صحيح sum16f( ثابت hfloat* src, ثابت uchar* mask, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_<hfloat, عائم, عائم>(src, mask, dst, len, cn); }

ساكن صحيح sum16bf( ثابت bfloat* src, ثابت uchar* mask, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); החזר sum_<bfloat, عائم, عائم>(src, mask, dst, len, cn); }

SumFunc getSumFunc(صحيح depth)
{
    ساكن SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, صحيح, sum8u>,
        sumWrap<schar, صحيح, sum8s>,
        sumWrap<ushort, صحيح, sum16u>,
        sumWrap<short, صحيح, sum16s>,
        sumWrap<صحيح, مزدوج, sum32s>,
        sumWrap<عائم, مزدوج, sum32f>,
        sumWrap<مزدوج, مزدوج, sum64f>,
        sumWrap<hfloat, عائم, sum16f>,
        sumWrap<bfloat, عائم, sum16bf>,
        0,
        sumWrap<uint64, مزدوج, sum64u>,
        sumWrap<int64, مزدوج, sum64s>,
        sumWrap<unsigned, مزدوج, sum32u>,
        0
    };

    החזר sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
