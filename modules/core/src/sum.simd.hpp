// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "stat.hpp"

নামাকাশ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

template<typename T, typename ST, পূর্ণসংখ্যা (*fn)(ধ্রুবক T*, ধ্রুবক uchar*, ST*, পূর্ণসংখ্যা, পূর্ণসংখ্যা)>
স্থির পূর্ণসংখ্যা যোগ_Wrap(ধ্রুবক uchar* src, ধ্রুবক uchar* mask, uchar* যোগ_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    ফেরত fn((ধ্রুবক T*)src, mask, (ST*)যোগ_2, len, cn);
}

যোগ_Func get_যোগ_Func(পূর্ণসংখ্যা depth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

template <typename T, typename ST>
কাঠামো যোগ_SIMD
{
    যোগ_SIMD(পূর্ণসংখ্যা) {}
    পূর্ণসংখ্যা operator () (ধ্রুবক T*, ধ্রুবক uchar*, ST*, পূর্ণসংখ্যা, পূর্ণসংখ্যা) ধ্রুবক
    {
        ফেরত 0;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)

#undef REDUCE_PARTIAL_SUMS
#define REDUCE_PARTIAL_SUMS() \
    যদি (cn == 1) \
        dst[0] += v_reduce_যোগ(v_add(v_add(s0, s1), s2)); \
    অন্যথা যদি (cn == 2) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_যোগ(v_and(s0, m0)); \
        dst[1] += v_reduce_যোগ(v_and(s0, m1)); \
    } অন্যথা যদি (cn == 3) { \
        dst[0] += v_reduce_যোগ(v_add(v_add(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_যোগ(v_add(v_add(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_যোগ(v_add(v_add(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } অন্যথা যদি (cn == 4) { \
        s0 = v_add(v_add(s0, s1), s2); \
        dst[0] += v_reduce_যোগ(v_and(s0, m0)); \
        dst[1] += v_reduce_যোগ(v_and(s0, m1)); \
        dst[2] += v_reduce_যোগ(v_and(s0, m2)); \
        dst[3] += v_reduce_যোগ(v_and(s0, m3)); \
    }

template<typename ST>
স্থির শূন্য init_maskbuf(ST* maskbuf, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা simd_width)
{
    memset(maskbuf, 0, simd_width*9*আকার(maskbuf[0]));
    যদি (cn == 1)
        ;
    অন্যথা যদি (cn == 2)
        জন্য (পূর্ণসংখ্যা i = 0; i < simd_width; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
        }
    অন্যথা যদি (cn == 3)
        জন্য (পূর্ণসংখ্যা i = 0; i < simd_width*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width*3] = (ST)-1;
            maskbuf[i+2+simd_width*6] = (ST)-1;
        }
    অন্যথা যদি (cn == 4 && simd_width >= 4) {
        জন্য (পূর্ণসংখ্যা i = 0; i < simd_width; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_width] = (ST)-1;
            maskbuf[i+2+simd_width*2] = (ST)-1;
            maskbuf[i+3+simd_width*3] = (ST)-1;
        }
    }
}

#undef DEFINE_যোগ_SIMD_8
#define DEFINE_যোগ_SIMD_8(T, ST, iST, VecT, load_op) \
template<> কাঠামো যোগ_SIMD<T, ST> \
{ \
    যোগ_SIMD(পূর্ণসংখ্যা cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    পূর্ণসংখ্যা operator ()(ধ্রুবক T* src, ধ্রুবক uchar* mask, ST* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক \
    { \
        যদি (mask || (cn < 1 || cn > 4)) \
            ফেরত 0; \
        len *= cn; \
        পূর্ণসংখ্যা x = 0, simd_width = VTraits<VecT>::vlanes(); \
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
        জন্য (; x <= len - simd_width*6; x += simd_width*6) { \
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
        ফেরত x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef DEFINE_যোগ_SIMD_16
#define DEFINE_যোগ_SIMD_16(T, ST, iST, VecT, load_op) \
template<> কাঠামো যোগ_SIMD<T, ST> \
{ \
    যোগ_SIMD(পূর্ণসংখ্যা cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    পূর্ণসংখ্যা operator ()(ধ্রুবক T* src, ধ্রুবক uchar* mask, ST* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক \
    { \
        যদি (mask || (cn < 1 || cn > 4)) \
            ফেরত 0; \
        len *= cn; \
        পূর্ণসংখ্যা x = 0, simd_width = VTraits<VecT>::vlanes(); \
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
        জন্য (; x <= len - simd_width*3; x += simd_width*3) { \
            auto v0 = load_op(src + x); \
            auto v1 = load_op(src + x + simd_width); \
            auto v2 = load_op(src + x + simd_width*2); \
            s0 = v_add(s0, v0); \
            s1 = v_add(s1, v1); \
            s2 = v_add(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        ফেরত x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#undef load_u8_as_s16
#undef load_u16_as_s32
#define load_u8_as_s16(addr) v_reinterpret_as_s16(vx_load_expand(addr))
#define load_u16_as_s32(addr) v_reinterpret_as_s32(vx_load_expand(addr))

DEFINE_যোগ_SIMD_8(uchar, পূর্ণসংখ্যা, পূর্ণসংখ্যা, v_int32, load_u8_as_s16)
DEFINE_যোগ_SIMD_8(schar, পূর্ণসংখ্যা, পূর্ণসংখ্যা, v_int32, vx_load_expand)
DEFINE_যোগ_SIMD_16(ushort, পূর্ণসংখ্যা, পূর্ণসংখ্যা, v_int32, load_u16_as_s32)
DEFINE_যোগ_SIMD_16(short, পূর্ণসংখ্যা, পূর্ণসংখ্যা, v_int32, vx_load_expand)
DEFINE_যোগ_SIMD_16(hfloat, দশমিক, পূর্ণসংখ্যা, v_float32, vx_load_expand)
DEFINE_যোগ_SIMD_16(bfloat, দশমিক, পূর্ণসংখ্যা, v_float32, vx_load_expand)

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#undef DEFINE_যোগ_SIMD_32
#define DEFINE_যোগ_SIMD_32(T, ST, iST, VecT) \
template<> কাঠামো যোগ_SIMD<T, ST> \
{ \
    যোগ_SIMD(পূর্ণসংখ্যা cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    পূর্ণসংখ্যা operator ()(ধ্রুবক T* src, ধ্রুবক uchar* mask, ST* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn) ধ্রুবক \
    { \
        পূর্ণসংখ্যা x = 0, simd_width = VTraits<VecT>::vlanes(); \
        যদি (mask || (cn < 1 || cn > 3+(simd_width>=4))) \
            ফেরত 0; \
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
        জন্য (; x <= len - simd_width*6; x += simd_width*6) { \
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
        ফেরত x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_যোগ_SIMD_32(পূর্ণসংখ্যা, দ্বিগুণ, int64, v_float64)
DEFINE_যোগ_SIMD_32(দশমিক, দ্বিগুণ, int64, v_float64)
#endif
#endif

template<typename T, typename ST, typename WT=T>
স্থির পূর্ণসংখ্যা যোগ_4(ধ্রুবক T* src0, ধ্রুবক uchar* mask, ST* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    ধ্রুবক T* src = src0;
    যদি( !mask )
    {
        যোগ_SIMD<T, ST> vop(cn);
        পূর্ণসংখ্যা i0 = vop(src0, mask, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        যদি( k == 1 )
        {
            ST s0 = dst[0];

            #যদি CV_ENABLE_UNROLLED
            জন্য(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #endif
            জন্য( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        অন্যথা যদি( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            জন্য( ; i < len; i++, src += cn )
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
            জন্য( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        জন্য( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            জন্য( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        ফেরত len;
    }

    পূর্ণসংখ্যা i, nzm = 0;
    যদি( cn == 1 )
    {
        ST s = dst[0];
        জন্য( i = 0; i < len; i++ )
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
        জন্য( i = 0; i < len; i++, src += 3 )
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
        জন্য( i = 0; i < len; i++, src += cn )
            যদি( mask[i] )
            {
                পূর্ণসংখ্যা k = 0;
                #যদি CV_ENABLE_UNROLLED
                জন্য( ; k <= cn - 4; k += 4 )
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
                জন্য( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    ফেরত nzm;
}


স্থির পূর্ণসংখ্যা sum8u( ধ্রুবক uchar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum8s( ধ্রুবক schar* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum16u( ধ্রুবক ushort* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum16s( ধ্রুবক short* src, ধ্রুবক uchar* mask, পূর্ণসংখ্যা* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum32u( ধ্রুবক unsigned* src, ধ্রুবক uchar* mask, দ্বিগুণ* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4<unsigned, দ্বিগুণ, uint64>(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum32s( ধ্রুবক পূর্ণসংখ্যা* src, ধ্রুবক uchar* mask, দ্বিগুণ* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4<পূর্ণসংখ্যা, দ্বিগুণ, int64>(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum64u( ধ্রুবক uint64* src, ধ্রুবক uchar* mask, দ্বিগুণ* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum64s( ধ্রুবক int64* src, ধ্রুবক uchar* mask, দ্বিগুণ* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum32f( ধ্রুবক দশমিক* src, ধ্রুবক uchar* mask, দ্বিগুণ* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum64f( ধ্রুবক দ্বিগুণ* src, ধ্রুবক uchar* mask, দ্বিগুণ* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum16f( ধ্রুবক hfloat* src, ধ্রুবক uchar* mask, দশমিক* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4<hfloat, দশমিক, দশমিক>(src, mask, dst, len, cn); }

স্থির পূর্ণসংখ্যা sum16bf( ধ্রুবক bfloat* src, ধ্রুবক uchar* mask, দশমিক* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{ CV_INSTRUMENT_REGION(); ফেরত যোগ_4<bfloat, দশমিক, দশমিক>(src, mask, dst, len, cn); }

যোগ_Func get_যোগ_Func(পূর্ণসংখ্যা depth)
{
    স্থির যোগ_Func যোগ_Tab[CV_DEPTH_MAX] =
    {
        যোগ_Wrap<uchar, পূর্ণসংখ্যা, sum8u>,
        যোগ_Wrap<schar, পূর্ণসংখ্যা, sum8s>,
        যোগ_Wrap<ushort, পূর্ণসংখ্যা, sum16u>,
        যোগ_Wrap<short, পূর্ণসংখ্যা, sum16s>,
        যোগ_Wrap<পূর্ণসংখ্যা, দ্বিগুণ, sum32s>,
        যোগ_Wrap<দশমিক, দ্বিগুণ, sum32f>,
        যোগ_Wrap<দ্বিগুণ, দ্বিগুণ, sum64f>,
        যোগ_Wrap<hfloat, দশমিক, sum16f>,
        যোগ_Wrap<bfloat, দশমিক, sum16bf>,
        0,
        যোগ_Wrap<uint64, দ্বিগুণ, sum64u>,
        যোগ_Wrap<int64, দ্বিগুণ, sum64s>,
        যোগ_Wrap<unsigned, দ্বিগুণ, sum32u>,
        0
    };

    ফেরত যোগ_Tab[depth];
}

#endif

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
