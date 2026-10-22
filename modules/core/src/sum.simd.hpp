// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "stat.hpp"

নামাকাশ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, পূর্ণাঙ্ক (*fn)(ধ্রুব T*, ধ্রুব uchar*, ST*, পূর্ণাঙ্ক, পূর্ণাঙ্ক)>
স্থির পূর্ণাঙ্ক sumWrap(ধ্রুব uchar* src, ধ্রুব uchar* mask, uchar* sum, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn)
{
    প্রত্যাগম fn((ধ্রুব T*)src, mask, (ST*)sum, len, cn);
}

SumFunc getSumFunc(পূর্ণাঙ্ক depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
সংরচনা Sum_SIMD
{
    Sum_SIMD(পূর্ণাঙ্ক) {}
    পূর্ণাঙ্ক operator () (ধ্রুব T*, ধ্রুব uchar*, ST*, পূর্ণাঙ্ক, পূর্ণাঙ্ক) ধ্রুব
    {
        প্রত্যাগম 0;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    যদি (cn == 1) \
        dst[0] += v_reduce_sum(v_add(v_add(s0, s1), s2)); \
    অন্যথা যদি (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
    } অন্যথা যদি (cn == 3) { \
        dst[0] += v_reduce_sum(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_sum(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_sum(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } অন্যথা যদি (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_sum(v_and(s0, m0)); \
        dst[1] += v_reduce_sum(v_and(s0, m1)); \
        dst[2] += v_reduce_sum(v_and(s0, m2)); \
        dst[3] += v_reduce_sum(v_and(s0, m3)); \
    }

template<typename ST>
স্থির শূন্য init_maskbuf(ST* maskbuf, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক simd_width)
{
    memset(maskbuf, 0, simd_width*9*পরিমাণ(maskbuf[0]));
    যদি (cn == 1)
        ;
    অন্যথা যদি (cn == 2)
        প্রতি (পূর্ণাঙ্ক i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    অন্যথা যদি (cn == 3)
        প্রতি (পূর্ণাঙ্ক i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    অন্যথা যদি (cn == 4 && simd_width >= 4) {
        প্রতি (পূর্ণাঙ্ক i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_SUM_SIMD_8
#define DEFINE_SUM_SIMD_8(T, ST, iST, VecT, load_op) \
template<> সংরচনা Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(পূর্ণাঙ্ক cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    পূর্ণাঙ্ক operator ()(ধ্রুব T* src, ধ্রুব uchar* mask, ST* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব \
    { \
        যদি (mask || (cn < 1 || cn > 4)) \
            প্রত্যাগম 0; \
        len *= cn; \
        পূর্ণাঙ্ক x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        যদি (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } অন্যথা { \
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
        প্রতি (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width*2); \
            auto v2 = load_op(src + x + simd_width*4); \
            s0 = v_add(s0, v_expand_নিচের(v0)); \
            s1 = v_add(s1, v_expand_উপরের(v0)); \
            s2 = v_add(s2, v_expand_নিচের(v1)); \
            s0 = v_add(s0, v_expand_উপরের(v1)); \
            s1 = v_add(s1, v_expand_নিচের(v2)); \
            s2 = v_add(s2, v_expand_উপরের(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        প্রত্যাগম x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_SUM_SIMD_16
#define DEFINE_SUM_SIMD_16(T, ST, iST, VecT, load_op) \
template<> সংরচনা Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(পূর্ণাঙ্ক cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    পূর্ণাঙ্ক operator ()(ধ্রুব T* src, ধ্রুব uchar* mask, ST* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব \
    { \
        যদি (mask || (cn < 1 || cn > 4)) \
            প্রত্যাগম 0; \
        len *= cn; \
        পূর্ণাঙ্ক x = 0, simd_width = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        যদি (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } অন্যথা { \
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
        প্রতি (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        প্রত্যাগম x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_SUM_SIMD_8(uchar, পূর্ণাঙ্ক, পূর্ণাঙ্ক, v_int32, load_u8_as_s16)
DEFINE_SUM_SIMD_8(schar, পূর্ণাঙ্ক, পূর্ণাঙ্ক, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(ushort, পূর্ণাঙ্ক, পূর্ণাঙ্ক, v_int32, load_u16_as_s32)
DEFINE_SUM_SIMD_16(short, পূর্ণাঙ্ক, পূর্ণাঙ্ক, v_int32, vx_load_expand)
DEFINE_SUM_SIMD_16(hfloat, ভিন্ন, পূর্ণাঙ্ক, v_float32, vx_load_expand)
DEFINE_SUM_SIMD_16(bfloat, ভিন্ন, পূর্ণাঙ্ক, v_float32, vx_load_expand)

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_SUM_SIMD_32
#define DEFINE_SUM_SIMD_32(T, ST, iST, VecT) \
template<> সংরচনা Sum_SIMD<T, ST> \
{ \
    Sum_SIMD(পূর্ণাঙ্ক cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    পূর্ণাঙ্ক operator ()(ধ্রুব T* src, ধ্রুব uchar* mask, ST* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn) ধ্রুব \
    { \
        পূর্ণাঙ্ক x = 0, simd_width = VTraits<VecT>::vlanes(); \
        যদি (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            প্রত্যাগম 0; \
        len *= cn; \
        VecT m0 = vx_load(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        যদি (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } অন্যথা { \
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
        প্রতি (; x <= len - simd_width*6; x += simd_width*6) { \
            auto v0 = vx_load(src + x); \
            auto v1 = vx_load(src + x + simd_width*2); \
            auto v2 = vx_load(src + x + simd_width*4); \
            s0 = v_add(s0, v_cvt_f64(v0)); \
            s1 = v_add(s1, v_cvt_f64_উপরের(v0)); \
            s2 = v_add(s2, v_cvt_f64(v1)); \
            s0 = v_add(s0, v_cvt_f64_উপরের(v1)); \
            s1 = v_add(s1, v_cvt_f64(v2)); \
            s2 = v_add(s2, v_cvt_f64_উপরের(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        প্রত্যাগম x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_SUM_SIMD_32(পূর্ণাঙ্ক, দ্বিগুণ, int64, v_float64)
DEFINE_SUM_SIMD_32(ভিন্ন, দ্বিগুণ, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
স্থির পূর্ণাঙ্ক sum_(ধ্রুব T* src0, ধ্রুব uchar* mask, ST* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    ধ্রুব T* src = src0;
    যদি( !mask )
    {
        Sum_SIMD<T, ST> vop(cn);
        পূর্ণাঙ্ক i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        যদি( k == 1 )
        {
            ST s0 = dst[0];

            #যদি CV_ENABLE_UNROLLED
            প্রতি(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            প্রতি( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        অন্যথা যদি( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            প্রতি( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        অন্যথা যদি( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            প্রতি( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        প্রতি( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            প্রতি( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        প্রত্যাগম len;
    }

    পূর্ণাঙ্ক i, nzm = 0;
    যদি( cn == 1 )
    {
        ST s = dst[0];
        প্রতি( i = 0; i < len; i++ )
            যদি( mask[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    অন্যথা যদি( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        প্রতি( i = 0; i < len; i++, src += 3 )
            যদি( mask[i] )
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
    অন্যথা
    {
        প্রতি( i = 0; i < len; i++, src += cn )
            যদি( mask[i] )
            {
                পূর্ণাঙ্ক k = 0;
                #যদি CV_ENABLE_UNROLLED
                প্রতি( ; k <= cn - 4; k += 4 )
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
                প্রতি( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    প্রত্যাগম nzm;
}


স্থির পূর্ণাঙ্ক sum8u( ধ্রুব uchar* src, ধ্রুব uchar* mask, পূর্ণাঙ্ক* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum8s( ধ্রুব schar* src, ধ্রুব uchar* mask, পূর্ণাঙ্ক* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum16u( ধ্রুব ushort* src, ধ্রুব uchar* mask, পূর্ণাঙ্ক* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum16s( ধ্রুব short* src, ধ্রুব uchar* mask, পূর্ণাঙ্ক* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum32u( ধ্রুব unsigned* src, ধ্রুব uchar* mask, দ্বিগুণ* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_<unsigned, দ্বিগুণ, uint64>(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum32s( ধ্রুব পূর্ণাঙ্ক* src, ধ্রুব uchar* mask, দ্বিগুণ* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_<পূর্ণাঙ্ক, দ্বিগুণ, int64>(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum64u( ধ্রুব uint64* src, ধ্রুব uchar* mask, দ্বিগুণ* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum64s( ধ্রুব int64* src, ধ্রুব uchar* mask, দ্বিগুণ* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum32f( ধ্রুব ভিন্ন* src, ধ্রুব uchar* mask, দ্বিগুণ* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum64f( ধ্রুব দ্বিগুণ* src, ধ্রুব uchar* mask, দ্বিগুণ* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum16f( ধ্রুব hfloat* src, ধ্রুব uchar* mask, ভিন্ন* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_<hfloat, ভিন্ন, ভিন্ন>(src, mask, dst, len, cn); }

স্থির পূর্ণাঙ্ক sum16bf( ধ্রুব bfloat* src, ধ্রুব uchar* mask, ভিন্ন* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{ CV_INSTRUMENT_REGION(); প্রত্যাগম sum_<bfloat, ভিন্ন, ভিন্ন>(src, mask, dst, len, cn); }

SumFunc getSumFunc(পূর্ণাঙ্ক depth)
{
    স্থির SumFunc sumTab[CV_DEPTH_MAX] =
    {
        sumWrap<uchar, পূর্ণাঙ্ক, sum8u>,
        sumWrap<schar, পূর্ণাঙ্ক, sum8s>,
        sumWrap<ushort, পূর্ণাঙ্ক, sum16u>,
        sumWrap<short, পূর্ণাঙ্ক, sum16s>,
        sumWrap<পূর্ণাঙ্ক, দ্বিগুণ, sum32s>,
        sumWrap<ভিন্ন, দ্বিগুণ, sum32f>,
        sumWrap<দ্বিগুণ, দ্বিগুণ, sum64f>,
        sumWrap<hfloat, ভিন্ন, sum16f>,
        sumWrap<bfloat, ভিন্ন, sum16bf>,
        0,
        sumWrap<uint64, দ্বিগুণ, sum64u>,
        sumWrap<int64, দ্বিগুণ, sum64s>,
        sumWrap<unsigned, দ্বিগুণ, sum32u>,
        0
    };

    প্রত্যাগম sumTab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
