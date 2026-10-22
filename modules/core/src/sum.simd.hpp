// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "stat.hpp"

فضاینام cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, صحیح (*fn)(ثابت T*, ثابت uchar*, ST*, صحیح, صحیح)>
ایستا صحیح sumWrap(ثابت uchar* src, ثابت uchar* mask, uchar* sum, صحیح len, صحیح cn)
{
    بازگشت fn((ثابت T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(صحیح depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
ساختار Sum_SIMD
{
    Sum_SIMD(صحیح) {}
    صحیح operator () (ثابت T*, ثابت uchar*, ST*, صحیح, صحیح) ثابت
    {
        بازگشت 0;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    اگر (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    وگرنه اگر (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } وگرنه اگر (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } وگرنه اگر (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
ایستا تهی init_maskbuf(ST* maskbuf, صحیح cn, صحیح simd_width)
{
    memset(maskbuf, 0, simd_width*9*اندازه(maskbuf[0]));
    اگر (cn == 1)
        ;
    وگرنه اگر (cn == 2)
        برای (صحیح i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    وگرنه اگر (cn == 3)
        برای (صحیح i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    وگرنه اگر (cn == 4 && simd_width >= 4) {
        برای (صحیح i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> ساختار Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(صحیح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحیح operator ()(ثابت T* src, ثابت uchar* mask, ST* dst, صحیح len, صحیح cn) ثابت \
    { \
        اگر (mask || (cn < 1 || cn > 4)) \
            بازگشت 0; \
        len *= cn; \
        صحیح x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        اگر (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وگرنه { \
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
        برای (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_کم(v0)); \
            s1 = v_add(s1, v_expand_بالا(v0)); \
            s2 = v_add(s2, v_expand_کم(v1)); \
            s0 = v_add(s0, v_expand_بالا(v1)); \
            s1 = v_add(s1, v_expand_کم(v2)); \
            s2 = v_add(s2, v_expand_بالا(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        بازگشت x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> ساختار Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(صحیح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحیح operator ()(ثابت T* src, ثابت uchar* mask, ST* dst, صحیح len, صحیح cn) ثابت \
    { \
        اگر (mask || (cn < 1 || cn > 4)) \
            بازگشت 0; \
        len *= cn; \
        صحیح x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        اگر (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وگرنه { \
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
        برای (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        بازگشت x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, صحیح, صحیح, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, صحیح, صحیح, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, صحیح, صحیح, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, صحیح, صحیح, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, اعشاری, صحیح, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, اعشاری, صحیح, v_float32, vx_load_expand)

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> ساختار Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(صحیح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحیح operator ()(ثابت T* src, ثابت uchar* mask, ST* dst, صحیح len, صحیح cn) ثابت \
    { \
        صحیح x = 0, simd_width = VTraits<VecT>::vlanes(); \
        اگر (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            بازگشت 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        اگر (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وگرنه { \
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
        برای (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_بالا(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_بالا(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_بالا(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        بازگشت x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(صحیح, دوگانه, int64, v_float64)
DEFINE_SUM_SIMD_32(اعشاری, دوگانه, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
ایستا صحیح sum_(ثابت T* src0, ثابت uchar* mask, ST* dst, صحیح len, صحیح cn )
{
    ثابت T* src = src0;
    اگر( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        صحیح i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        اگر( k == 1 )
        {
            ST s0 = dst[0];

            #اگر CV_ENABLE_UNROLLED
            برای(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            برای( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        وگرنه اگر( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            برای( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        وگرنه اگر( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            برای( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        برای( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            برای( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        بازگشت len;
    }

    صحیح i, nzm = 0;
    اگر( cn == 1 )
    {
        ST s = dst[0];
        برای( i = 0; i < len; i++ )
            اگر( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    وگرنه اگر( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        برای( i = 0; i < len; i++, src += 3 )
            اگر( mask[i] )
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
    وگرنه
    {
        برای( i = 0; i < len; i++, src += cn )
            اگر( mask[i] )
            {
                صحیح k = 0;
                #اگر CV_ENABLE_UNROLLED
                برای( ; k <= cn - 4; k += 4 )
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
                برای( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    بازگشت nzm;
}


ایستا صحیح sum8u( ثابت uchar* src, ثابت uchar* mask, صحیح* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_(src, mask, dst, len, cn); }

ایستا صحیح sum8s( ثابت schar* src, ثابت uchar* mask, صحیح* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_(src, mask, dst, len, cn); }

ایستا صحیح sum16u( ثابت ushort* src, ثابت uchar* mask, صحیح* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_(src, mask, dst, len, cn); }

ایستا صحیح sum16s( ثابت short* src, ثابت uchar* mask, صحیح* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_(src, mask, dst, len, cn); }

ایستا صحیح sum32u( ثابت unsigned* src, ثابت uchar* mask, دوگانه* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_<unsigned, دوگانه, uint64>(src, mask, dst, len, cn); }

ایستا صحیح sum32s( ثابت صحیح* src, ثابت uchar* mask, دوگانه* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_<صحیح, دوگانه, int64>(src, mask, dst, len, cn); }

ایستا صحیح sum64u( ثابت uint64* src, ثابت uchar* mask, دوگانه* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_(src, mask, dst, len, cn); }

ایستا صحیح sum64s( ثابت int64* src, ثابت uchar* mask, دوگانه* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_(src, mask, dst, len, cn); }

ایستا صحیح sum32f( ثابت اعشاری* src, ثابت uchar* mask, دوگانه* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_(src, mask, dst, len, cn); }

ایستا صحیح sum64f( ثابت دوگانه* src, ثابت uchar* mask, دوگانه* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_(src, mask, dst, len, cn); }

ایستا صحیح sum16f( ثابت hfloat* src, ثابت uchar* mask, اعشاری* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_<hfloat, اعشاری, اعشاری>(src, mask, dst, len, cn); }

ایستا صحیح sum16bf( ثابت bfloat* src, ثابت uchar* mask, اعشاری* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); بازگشت sum_<bfloat, اعشاری, اعشاری>(src, mask, dst, len, cn); }

SumFunc getSumFunc(صحیح depth)
{
    ایستا SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, صحیح, sum8u>,
        sumWrap<schar, صحیح, sum8s>,
        sumWrap<ushort, صحیح, sum16u>,
        sumWrap<short, صحیح, sum16s>,
        sumWrap<صحیح, دوگانه, sum32s>,
        sumWrap<اعشاری, دوگانه, sum32f>,
        sumWrap<دوگانه, دوگانه, sum64f>,
        sumWrap<hfloat, اعشاری, sum16f>,
        sumWrap<bfloat, اعشاری, sum16bf>,
        0,
        sumWrap<uint64, دوگانه, sum64u>,
        sumWrap<int64, دوگانه, sum64s>,
        sumWrap<unsigned, دوگانه, sum32u>,
        0
    };

    بازگشت sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
