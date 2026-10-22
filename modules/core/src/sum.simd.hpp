// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "stat.hpp"

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

قالب<نام_نوع T, نام_نوع ST, صحيح (*fn)(ثابت T*, ثابت uchar*, ST*, صحيح, صحيح)>
ساكن صحيح جمع_Wrap(ثابت uchar* src, ثابت uchar* ماسک, uchar* جمع, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src, ماسک, (ST*)جمع, len, cn);
}

جمع_Func get_جمع_Func(صحيح عمق);

#اگر_تعریف_نشده CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

قالب <نام_نوع T, نام_نوع ST>
بنية جمع_SIMD
{
    جمع_SIMD(صحيح) {}
    صحيح عملگر () (ثابت T*, ثابت uchar*, ST*, صحيح, صحيح) ثابت
    {
        أرجع 0;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

#لغو_تعریف REDUCE_PARTIAL_SUMS
#تعریف_کن REDUCE_PARTIAL_SUMS() \
    إذا (cn == 1) \
        dst[0] += v_reduce_جمع(v_جمع_کن(v_جمع_کن(s0, s1), s2)); \
    وإلا إذا (cn == 2) { \
        s0 = v_جمع_کن(v_جمع_کن(s0, s1), s2); \
        dst[0] += v_reduce_جمع(v_and(s0, m0)); \
        dst[1] += v_reduce_جمع(v_and(s0, m1)); \
    } وإلا إذا (cn == 3) { \
        dst[0] += v_reduce_جمع(v_جمع_کن(v_جمع_کن(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_جمع(v_جمع_کن(v_جمع_کن(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_جمع(v_جمع_کن(v_جمع_کن(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } وإلا إذا (cn == 4) { \
        s0 = v_جمع_کن(v_جمع_کن(s0, s1), s2); \
        dst[0] += v_reduce_جمع(v_and(s0, m0)); \
        dst[1] += v_reduce_جمع(v_and(s0, m1)); \
        dst[2] += v_reduce_جمع(v_and(s0, m2)); \
        dst[3] += v_reduce_جمع(v_and(s0, m3)); \
    }

قالب<نام_نوع ST>
ساكن فراغ init_maskbuf(ST* maskbuf, صحيح cn, صحيح simd_عرض)
{
    memset(maskbuf, 0, simd_عرض*9*حجم(maskbuf[0]));
    إذا (cn == 1)
        ;
    وإلا إذا (cn == 2)
        لكل (صحيح i = 0; i < simd_عرض; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_عرض] = (ST)-1;
        }
    وإلا إذا (cn == 3)
        لكل (صحيح i = 0; i < simd_عرض*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_عرض*3] = (ST)-1;
            maskbuf[i+2+simd_عرض*6] = (ST)-1;
        }
    وإلا إذا (cn == 4 && simd_عرض >= 4) {
        لكل (صحيح i = 0; i < simd_عرض; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_عرض] = (ST)-1;
            maskbuf[i+2+simd_عرض*2] = (ST)-1;
            maskbuf[i+3+simd_عرض*3] = (ST)-1;
        }
    }
}

#لغو_تعریف DEFINE_جمع_SIMD_8
#تعریف_کن DEFINE_جمع_SIMD_8(T, ST, iST, VecT, بارگیری_op) \
قالب<> بنية جمع_SIMD<T, ST> \
{ \
    جمع_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عملگر ()(ثابت T* src, ثابت uchar* ماسک, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        إذا (ماسک || (cn < 1 || cn > 4)) \
            أرجع 0; \
        len *= cn; \
        صحيح x = 0, simd_عرض = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_بارگیری(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_بارگیری(maskbuf + simd_عرض); \
            m2 = vx_بارگیری(maskbuf + simd_عرض*2); \
            m3 = vx_بارگیری(maskbuf + simd_عرض*3); \
            m4 = vx_بارگیری(maskbuf + simd_عرض*4); \
            m5 = vx_بارگیری(maskbuf + simd_عرض*5); \
            m6 = vx_بارگیری(maskbuf + simd_عرض*6); \
            m7 = vx_بارگیری(maskbuf + simd_عرض*7); \
            m8 = vx_بارگیری(maskbuf + simd_عرض*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_عرض*6; x += simd_عرض*6) { \
            خودکار v0 = بارگیری_op(src + x); \
            خودکار v1 = بارگیری_op(src + x + simd_عرض*2); \
            خودکار v2 = بارگیری_op(src + x + simd_عرض*4); \
            s0 = v_جمع_کن(s0, v_expand_کم(v0)); \
            s1 = v_جمع_کن(s1, v_expand_بالا(v0)); \
            s2 = v_جمع_کن(s2, v_expand_کم(v1)); \
            s0 = v_جمع_کن(s0, v_expand_بالا(v1)); \
            s1 = v_جمع_کن(s1, v_expand_کم(v2)); \
            s2 = v_جمع_کن(s2, v_expand_بالا(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#لغو_تعریف DEFINE_جمع_SIMD_16
#تعریف_کن DEFINE_جمع_SIMD_16(T, ST, iST, VecT, بارگیری_op) \
قالب<> بنية جمع_SIMD<T, ST> \
{ \
    جمع_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عملگر ()(ثابت T* src, ثابت uchar* ماسک, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        إذا (ماسک || (cn < 1 || cn > 4)) \
            أرجع 0; \
        len *= cn; \
        صحيح x = 0, simd_عرض = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_بارگیری(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_بارگیری(maskbuf + simd_عرض); \
            m2 = vx_بارگیری(maskbuf + simd_عرض*2); \
            m3 = vx_بارگیری(maskbuf + simd_عرض*3); \
            m4 = vx_بارگیری(maskbuf + simd_عرض*4); \
            m5 = vx_بارگیری(maskbuf + simd_عرض*5); \
            m6 = vx_بارگیری(maskbuf + simd_عرض*6); \
            m7 = vx_بارگیری(maskbuf + simd_عرض*7); \
            m8 = vx_بارگیری(maskbuf + simd_عرض*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_عرض*3; x += simd_عرض*3) { \
            خودکار v0 = بارگیری_op(src + x); \
            خودکار v1 = بارگیری_op(src + x + simd_عرض); \
            خودکار v2 = بارگیری_op(src + x + simd_عرض*2); \
            s0 = v_جمع_کن(s0, v0); \
            s1 = v_جمع_کن(s1, v1); \
            s2 = v_جمع_کن(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#لغو_تعریف بارگیری_u8_as_s16
#لغو_تعریف بارگیری_u16_as_s32
#تعریف_کن بارگیری_u8_as_s16(addr) v_reinterpret_as_s16(vx_بارگیری_expand(addr))
#تعریف_کن بارگیری_u16_as_s32(addr) v_reinterpret_as_s32(vx_بارگیری_expand(addr))

DEFINE_جمع_SIMD_8(uchar, صحيح, صحيح, v_int32, بارگیری_u8_as_s16)
DEFINE_جمع_SIMD_8(schar, صحيح, صحيح, v_int32, vx_بارگیری_expand)
DEFINE_جمع_SIMD_16(ushort, صحيح, صحيح, v_int32, بارگیری_u16_as_s32)
DEFINE_جمع_SIMD_16(کوتاه, صحيح, صحيح, v_int32, vx_بارگیری_expand)
DEFINE_جمع_SIMD_16(hfloat, عائم, صحيح, v_float32, vx_بارگیری_expand)
DEFINE_جمع_SIMD_16(bfloat, عائم, صحيح, v_float32, vx_بارگیری_expand)

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#لغو_تعریف DEFINE_جمع_SIMD_32
#تعریف_کن DEFINE_جمع_SIMD_32(T, ST, iST, VecT) \
قالب<> بنية جمع_SIMD<T, ST> \
{ \
    جمع_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عملگر ()(ثابت T* src, ثابت uchar* ماسک, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        صحيح x = 0, simd_عرض = VTraits<VecT>::vlanes(); \
        إذا (ماسک || (cn < 1 || cn > 3+(simd_عرض>=4))) \
            أرجع 0; \
        len *= cn; \
        VecT m0 = vx_بارگیری(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_بارگیری(maskbuf + simd_عرض); \
            m2 = vx_بارگیری(maskbuf + simd_عرض*2); \
            m3 = vx_بارگیری(maskbuf + simd_عرض*3); \
            m4 = vx_بارگیری(maskbuf + simd_عرض*4); \
            m5 = vx_بارگیری(maskbuf + simd_عرض*5); \
            m6 = vx_بارگیری(maskbuf + simd_عرض*6); \
            m7 = vx_بارگیری(maskbuf + simd_عرض*7); \
            m8 = vx_بارگیری(maskbuf + simd_عرض*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_عرض*6; x += simd_عرض*6) { \
            خودکار v0 = vx_بارگیری(src + x); \
            خودکار v1 = vx_بارگیری(src + x + simd_عرض*2); \
            خودکار v2 = vx_بارگیری(src + x + simd_عرض*4); \
            s0 = v_جمع_کن(s0, v_cvt_f64(v0)); \
            s1 = v_جمع_کن(s1, v_cvt_f64_بالا(v0)); \
            s2 = v_جمع_کن(s2, v_cvt_f64(v1)); \
            s0 = v_جمع_کن(s0, v_cvt_f64_بالا(v1)); \
            s1 = v_جمع_کن(s1, v_cvt_f64(v2)); \
            s2 = v_جمع_کن(s2, v_cvt_f64_بالا(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_جمع_SIMD_32(صحيح, مزدوج, int64, v_float64)
DEFINE_جمع_SIMD_32(عائم, مزدوج, int64, v_float64)
#پایان_اگر
#پایان_اگر

قالب<نام_نوع T, نام_نوع ST, نام_نوع WT=T>
ساكن صحيح جمع_2(ثابت T* src0, ثابت uchar* ماسک, ST* dst, صحيح len, صحيح cn )
{
    ثابت T* src = src0;
    إذا( !ماسک )
    {
        جمع_SIMD<T, ST> vop(cn);
        صحيح i0 = vop(src0, ماسک, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        إذا( k == 1 )
        {
            ST s0 = dst[0];

            #إذا CV_ENABLE_UNROLLED
            لكل(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #پایان_اگر
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
            إذا( ماسک[i] )
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
            إذا( ماسک[i] )
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
            إذا( ماسک[i] )
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
                #پایان_اگر
                لكل( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    أرجع nzm;
}


ساكن صحيح sum8u( ثابت uchar* src, ثابت uchar* ماسک, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum8s( ثابت schar* src, ثابت uchar* ماسک, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum16u( ثابت ushort* src, ثابت uchar* ماسک, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum16s( ثابت کوتاه* src, ثابت uchar* ماسک, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum32u( ثابت بیعلامت* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2<بیعلامت, مزدوج, uint64>(src, ماسک, dst, len, cn); }

ساكن صحيح sum32s( ثابت صحيح* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2<صحيح, مزدوج, int64>(src, ماسک, dst, len, cn); }

ساكن صحيح sum64u( ثابت uint64* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum64s( ثابت int64* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum32f( ثابت عائم* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum64f( ثابت مزدوج* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum16f( ثابت hfloat* src, ثابت uchar* ماسک, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2<hfloat, عائم, عائم>(src, ماسک, dst, len, cn); }

ساكن صحيح sum16bf( ثابت bfloat* src, ثابت uchar* ماسک, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جمع_2<bfloat, عائم, عائم>(src, ماسک, dst, len, cn); }

جمع_Func get_جمع_Func(صحيح عمق)
{
    ساكن جمع_Func جمع_Tab[CV_عمق_MAX] =
    {
        جمع_Wrap<uchar, صحيح, sum8u>,
        جمع_Wrap<schar, صحيح, sum8s>,
        جمع_Wrap<ushort, صحيح, sum16u>,
        جمع_Wrap<کوتاه, صحيح, sum16s>,
        جمع_Wrap<صحيح, مزدوج, sum32s>,
        جمع_Wrap<عائم, مزدوج, sum32f>,
        جمع_Wrap<مزدوج, مزدوج, sum64f>,
        جمع_Wrap<hfloat, عائم, sum16f>,
        جمع_Wrap<bfloat, عائم, sum16bf>,
        0,
        جمع_Wrap<uint64, مزدوج, sum64u>,
        جمع_Wrap<int64, مزدوج, sum64s>,
        جمع_Wrap<بیعلامت, مزدوج, sum32u>,
        0
    };

    أرجع جمع_Tab[عمق];
}

#پایان_اگر

CV_CPU_OPTIMIZATION_NAMESPACE_انتها
} // namespace
