// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "stat.hpp"

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

سانچا<قسم_ناں T, قسم_ناں ST, صحيح (*fn)(ثابت T*, ثابت uchar*, ST*, صحيح, صحيح)>
ساكن صحيح جوڑ_Wrap(ثابت uchar* src, ثابت uchar* نقاب, uchar* جوڑ, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src, نقاب, (ST*)جوڑ, len, cn);
}

جوڑ_Func get_جوڑ_Func(صحيح ڈونگھائی);

#جے_بنا_تعریف CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

سانچا <قسم_ناں T, قسم_ناں ST>
بنية جوڑ_SIMD
{
    جوڑ_SIMD(صحيح) {}
    صحيح عامل () (ثابت T*, ثابت uchar*, ST*, صحيح, صحيح) ثابت
    {
        أرجع 0;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

#تعریف_رد REDUCE_PARTIAL_SUMS
#تعریف_کرو REDUCE_PARTIAL_SUMS() \
    إذا (cn == 1) \
        dst[0] += v_reduce_جوڑ(v_جوڑو(v_جوڑو(s0, s1), s2)); \
    وإلا إذا (cn == 2) { \
        s0 = v_جوڑو(v_جوڑو(s0, s1), s2); \
        dst[0] += v_reduce_جوڑ(v_and(s0, m0)); \
        dst[1] += v_reduce_جوڑ(v_and(s0, m1)); \
    } وإلا إذا (cn == 3) { \
        dst[0] += v_reduce_جوڑ(v_جوڑو(v_جوڑو(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_جوڑ(v_جوڑو(v_جوڑو(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_جوڑ(v_جوڑو(v_جوڑو(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } وإلا إذا (cn == 4) { \
        s0 = v_جوڑو(v_جوڑو(s0, s1), s2); \
        dst[0] += v_reduce_جوڑ(v_and(s0, m0)); \
        dst[1] += v_reduce_جوڑ(v_and(s0, m1)); \
        dst[2] += v_reduce_جوڑ(v_and(s0, m2)); \
        dst[3] += v_reduce_جوڑ(v_and(s0, m3)); \
    }

سانچا<قسم_ناں ST>
ساكن فراغ init_maskbuf(ST* maskbuf, صحيح cn, صحيح simd_چوڑائی)
{
    memset(maskbuf, 0, simd_چوڑائی*9*حجم(maskbuf[0]));
    إذا (cn == 1)
        ;
    وإلا إذا (cn == 2)
        لكل (صحيح i = 0; i < simd_چوڑائی; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_چوڑائی] = (ST)-1;
        }
    وإلا إذا (cn == 3)
        لكل (صحيح i = 0; i < simd_چوڑائی*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_چوڑائی*3] = (ST)-1;
            maskbuf[i+2+simd_چوڑائی*6] = (ST)-1;
        }
    وإلا إذا (cn == 4 && simd_چوڑائی >= 4) {
        لكل (صحيح i = 0; i < simd_چوڑائی; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_چوڑائی] = (ST)-1;
            maskbuf[i+2+simd_چوڑائی*2] = (ST)-1;
            maskbuf[i+3+simd_چوڑائی*3] = (ST)-1;
        }
    }
}

#تعریف_رد DEFINE_جوڑ_SIMD_8
#تعریف_کرو DEFINE_جوڑ_SIMD_8(T, ST, iST, VecT, لدو_op) \
سانچا<> بنية جوڑ_SIMD<T, ST> \
{ \
    جوڑ_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        إذا (نقاب || (cn < 1 || cn > 4)) \
            أرجع 0; \
        len *= cn; \
        صحيح x = 0, simd_چوڑائی = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_لدو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_لدو(maskbuf + simd_چوڑائی); \
            m2 = vx_لدو(maskbuf + simd_چوڑائی*2); \
            m3 = vx_لدو(maskbuf + simd_چوڑائی*3); \
            m4 = vx_لدو(maskbuf + simd_چوڑائی*4); \
            m5 = vx_لدو(maskbuf + simd_چوڑائی*5); \
            m6 = vx_لدو(maskbuf + simd_چوڑائی*6); \
            m7 = vx_لدو(maskbuf + simd_چوڑائی*7); \
            m8 = vx_لدو(maskbuf + simd_چوڑائی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_چوڑائی*6; x += simd_چوڑائی*6) { \
            خودکار v0 = لدو_op(src + x); \
            خودکار v1 = لدو_op(src + x + simd_چوڑائی*2); \
            خودکار v2 = لدو_op(src + x + simd_چوڑائی*4); \
            s0 = v_جوڑو(s0, v_expand_کم(v0)); \
            s1 = v_جوڑو(s1, v_expand_بلند(v0)); \
            s2 = v_جوڑو(s2, v_expand_کم(v1)); \
            s0 = v_جوڑو(s0, v_expand_بلند(v1)); \
            s1 = v_جوڑو(s1, v_expand_کم(v2)); \
            s2 = v_جوڑو(s2, v_expand_بلند(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#تعریف_رد DEFINE_جوڑ_SIMD_16
#تعریف_کرو DEFINE_جوڑ_SIMD_16(T, ST, iST, VecT, لدو_op) \
سانچا<> بنية جوڑ_SIMD<T, ST> \
{ \
    جوڑ_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        إذا (نقاب || (cn < 1 || cn > 4)) \
            أرجع 0; \
        len *= cn; \
        صحيح x = 0, simd_چوڑائی = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_لدو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_لدو(maskbuf + simd_چوڑائی); \
            m2 = vx_لدو(maskbuf + simd_چوڑائی*2); \
            m3 = vx_لدو(maskbuf + simd_چوڑائی*3); \
            m4 = vx_لدو(maskbuf + simd_چوڑائی*4); \
            m5 = vx_لدو(maskbuf + simd_چوڑائی*5); \
            m6 = vx_لدو(maskbuf + simd_چوڑائی*6); \
            m7 = vx_لدو(maskbuf + simd_چوڑائی*7); \
            m8 = vx_لدو(maskbuf + simd_چوڑائی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_چوڑائی*3; x += simd_چوڑائی*3) { \
            خودکار v0 = لدو_op(src + x); \
            خودکار v1 = لدو_op(src + x + simd_چوڑائی); \
            خودکار v2 = لدو_op(src + x + simd_چوڑائی*2); \
            s0 = v_جوڑو(s0, v0); \
            s1 = v_جوڑو(s1, v1); \
            s2 = v_جوڑو(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#تعریف_رد لدو_u8_as_s16
#تعریف_رد لدو_u16_as_s32
#تعریف_کرو لدو_u8_as_s16(addr) v_reinterpret_as_s16(vx_لدو_expand(addr))
#تعریف_کرو لدو_u16_as_s32(addr) v_reinterpret_as_s32(vx_لدو_expand(addr))

DEFINE_جوڑ_SIMD_8(uchar, صحيح, صحيح, v_int32, لدو_u8_as_s16)
DEFINE_جوڑ_SIMD_8(schar, صحيح, صحيح, v_int32, vx_لدو_expand)
DEFINE_جوڑ_SIMD_16(ushort, صحيح, صحيح, v_int32, لدو_u16_as_s32)
DEFINE_جوڑ_SIMD_16(چھوٹا, صحيح, صحيح, v_int32, vx_لدو_expand)
DEFINE_جوڑ_SIMD_16(hfloat, عائم, صحيح, v_float32, vx_لدو_expand)
DEFINE_جوڑ_SIMD_16(bfloat, عائم, صحيح, v_float32, vx_لدو_expand)

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#تعریف_رد DEFINE_جوڑ_SIMD_32
#تعریف_کرو DEFINE_جوڑ_SIMD_32(T, ST, iST, VecT) \
سانچا<> بنية جوڑ_SIMD<T, ST> \
{ \
    جوڑ_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        صحيح x = 0, simd_چوڑائی = VTraits<VecT>::vlanes(); \
        إذا (نقاب || (cn < 1 || cn > 3+(simd_چوڑائی>=4))) \
            أرجع 0; \
        len *= cn; \
        VecT m0 = vx_لدو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_لدو(maskbuf + simd_چوڑائی); \
            m2 = vx_لدو(maskbuf + simd_چوڑائی*2); \
            m3 = vx_لدو(maskbuf + simd_چوڑائی*3); \
            m4 = vx_لدو(maskbuf + simd_چوڑائی*4); \
            m5 = vx_لدو(maskbuf + simd_چوڑائی*5); \
            m6 = vx_لدو(maskbuf + simd_چوڑائی*6); \
            m7 = vx_لدو(maskbuf + simd_چوڑائی*7); \
            m8 = vx_لدو(maskbuf + simd_چوڑائی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_چوڑائی*6; x += simd_چوڑائی*6) { \
            خودکار v0 = vx_لدو(src + x); \
            خودکار v1 = vx_لدو(src + x + simd_چوڑائی*2); \
            خودکار v2 = vx_لدو(src + x + simd_چوڑائی*4); \
            s0 = v_جوڑو(s0, v_cvt_f64(v0)); \
            s1 = v_جوڑو(s1, v_cvt_f64_بلند(v0)); \
            s2 = v_جوڑو(s2, v_cvt_f64(v1)); \
            s0 = v_جوڑو(s0, v_cvt_f64_بلند(v1)); \
            s1 = v_جوڑو(s1, v_cvt_f64(v2)); \
            s2 = v_جوڑو(s2, v_cvt_f64_بلند(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_جوڑ_SIMD_32(صحيح, مزدوج, int64, v_float64)
DEFINE_جوڑ_SIMD_32(عائم, مزدوج, int64, v_float64)
#جے_انت
#جے_انت

سانچا<قسم_ناں T, قسم_ناں ST, قسم_ناں WT=T>
ساكن صحيح جوڑ_2(ثابت T* src0, ثابت uchar* نقاب, ST* dst, صحيح len, صحيح cn )
{
    ثابت T* src = src0;
    إذا( !نقاب )
    {
        جوڑ_SIMD<T, ST> vop(cn);
        صحيح i0 = vop(src0, نقاب, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        إذا( k == 1 )
        {
            ST s0 = dst[0];

            #إذا CV_ENABLE_UNROLLED
            لكل(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #جے_انت
            لكل( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        وإلا إذا( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            لكل( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        وإلا إذا( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            لكل( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        لكل( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            لكل( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        أرجع len;
    }

    صحيح i, nzm = 0;
    إذا( cn == 1 )
    {
        ST s = dst[0];
        لكل( i = 0; i < len; i++ )
            إذا( نقاب[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    وإلا إذا( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        لكل( i = 0; i < len; i++, src += 3 )
            إذا( نقاب[i] )
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
    وإلا
    {
        لكل( i = 0; i < len; i++, src += cn )
            إذا( نقاب[i] )
            {
                صحيح k = 0;
                #إذا CV_ENABLE_UNROLLED
                لكل( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #جے_انت
                لكل( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    أرجع nzm;
}


ساكن صحيح sum8u( ثابت uchar* src, ثابت uchar* نقاب, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum8s( ثابت schar* src, ثابت uchar* نقاب, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum16u( ثابت ushort* src, ثابت uchar* نقاب, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum16s( ثابت چھوٹا* src, ثابت uchar* نقاب, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum32u( ثابت بنانشانی* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2<بنانشانی, مزدوج, uint64>(src, نقاب, dst, len, cn); }

ساكن صحيح sum32s( ثابت صحيح* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2<صحيح, مزدوج, int64>(src, نقاب, dst, len, cn); }

ساكن صحيح sum64u( ثابت uint64* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum64s( ثابت int64* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum32f( ثابت عائم* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum64f( ثابت مزدوج* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum16f( ثابت hfloat* src, ثابت uchar* نقاب, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2<hfloat, عائم, عائم>(src, نقاب, dst, len, cn); }

ساكن صحيح sum16bf( ثابت bfloat* src, ثابت uchar* نقاب, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڑ_2<bfloat, عائم, عائم>(src, نقاب, dst, len, cn); }

جوڑ_Func get_جوڑ_Func(صحيح ڈونگھائی)
{
    ساكن جوڑ_Func جوڑ_Tab[CV_ڈونگھائی_MAX] =
    {
        جوڑ_Wrap<uchar, صحيح, sum8u>,
        جوڑ_Wrap<schar, صحيح, sum8s>,
        جوڑ_Wrap<ushort, صحيح, sum16u>,
        جوڑ_Wrap<چھوٹا, صحيح, sum16s>,
        جوڑ_Wrap<صحيح, مزدوج, sum32s>,
        جوڑ_Wrap<عائم, مزدوج, sum32f>,
        جوڑ_Wrap<مزدوج, مزدوج, sum64f>,
        جوڑ_Wrap<hfloat, عائم, sum16f>,
        جوڑ_Wrap<bfloat, عائم, sum16bf>,
        0,
        جوڑ_Wrap<uint64, مزدوج, sum64u>,
        جوڑ_Wrap<int64, مزدوج, sum64s>,
        جوڑ_Wrap<بنانشانی, مزدوج, sum32u>,
        0
    };

    أرجع جوڑ_Tab[ڈونگھائی];
}

#جے_انت

CV_CPU_OPTIMIZATION_NAMESPACE_سرا
} // namespace
