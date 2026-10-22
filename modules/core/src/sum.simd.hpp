// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "stat.hpp"

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

کينډۍ<ډول_نوم T, ډول_نوم ST, صحيح (*fn)(ثابت T*, ثابت uchar*, ST*, صحيح, صحيح)>
ساكن صحيح مجموعه_Wrap(ثابت uchar* src, ثابت uchar* ماسک, uchar* مجموعه, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src, ماسک, (ST*)مجموعه, len, cn);
}

مجموعه_Func get_مجموعه_Func(صحيح ژوروالی);

#که_نه_تعریف_شوی CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

کينډۍ <ډول_نوم T, ډول_نوم ST>
بنية مجموعه_SIMD
{
    مجموعه_SIMD(صحيح) {}
    صحيح چلوونکی () (ثابت T*, ثابت uchar*, ST*, صحيح, صحيح) ثابت
    {
        أرجع 0;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

#تعریف_لغوه REDUCE_PARTIAL_SUMS
#تعریف_کړه REDUCE_PARTIAL_SUMS() \
    إذا (cn == 1) \
        dst[0] += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(s0, s1), s2)); \
    وإلا إذا (cn == 2) { \
        s0 = v_جمع_کړه(v_جمع_کړه(s0, s1), s2); \
        dst[0] += v_reduce_مجموعه(v_and(s0, m0)); \
        dst[1] += v_reduce_مجموعه(v_and(s0, m1)); \
    } وإلا إذا (cn == 3) { \
        dst[0] += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_مجموعه(v_جمع_کړه(v_جمع_کړه(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } وإلا إذا (cn == 4) { \
        s0 = v_جمع_کړه(v_جمع_کړه(s0, s1), s2); \
        dst[0] += v_reduce_مجموعه(v_and(s0, m0)); \
        dst[1] += v_reduce_مجموعه(v_and(s0, m1)); \
        dst[2] += v_reduce_مجموعه(v_and(s0, m2)); \
        dst[3] += v_reduce_مجموعه(v_and(s0, m3)); \
    }

کينډۍ<ډول_نوم ST>
ساكن فراغ init_maskbuf(ST* maskbuf, صحيح cn, صحيح simd_پلنوالی)
{
    memset(maskbuf, 0, simd_پلنوالی*9*حجم(maskbuf[0]));
    إذا (cn == 1)
        ;
    وإلا إذا (cn == 2)
        لكل (صحيح i = 0; i < simd_پلنوالی; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_پلنوالی] = (ST)-1;
        }
    وإلا إذا (cn == 3)
        لكل (صحيح i = 0; i < simd_پلنوالی*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_پلنوالی*3] = (ST)-1;
            maskbuf[i+2+simd_پلنوالی*6] = (ST)-1;
        }
    وإلا إذا (cn == 4 && simd_پلنوالی >= 4) {
        لكل (صحيح i = 0; i < simd_پلنوالی; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_پلنوالی] = (ST)-1;
            maskbuf[i+2+simd_پلنوالی*2] = (ST)-1;
            maskbuf[i+3+simd_پلنوالی*3] = (ST)-1;
        }
    }
}

#تعریف_لغوه DEFINE_مجموعه_SIMD_8
#تعریف_کړه DEFINE_مجموعه_SIMD_8(T, ST, iST, VecT, بارول_op) \
کينډۍ<> بنية مجموعه_SIMD<T, ST> \
{ \
    مجموعه_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح چلوونکی ()(ثابت T* src, ثابت uchar* ماسک, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        إذا (ماسک || (cn < 1 || cn > 4)) \
            أرجع 0; \
        len *= cn; \
        صحيح x = 0, simd_پلنوالی = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_بارول(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_بارول(maskbuf + simd_پلنوالی); \
            m2 = vx_بارول(maskbuf + simd_پلنوالی*2); \
            m3 = vx_بارول(maskbuf + simd_پلنوالی*3); \
            m4 = vx_بارول(maskbuf + simd_پلنوالی*4); \
            m5 = vx_بارول(maskbuf + simd_پلنوالی*5); \
            m6 = vx_بارول(maskbuf + simd_پلنوالی*6); \
            m7 = vx_بارول(maskbuf + simd_پلنوالی*7); \
            m8 = vx_بارول(maskbuf + simd_پلنوالی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_پلنوالی*6; x += simd_پلنوالی*6) { \
            خپلکاری v0 = بارول_op(src + x); \
            خپلکاری v1 = بارول_op(src + x + simd_پلنوالی*2); \
            خپلکاری v2 = بارول_op(src + x + simd_پلنوالی*4); \
            s0 = v_جمع_کړه(s0, v_expand_ټیټ(v0)); \
            s1 = v_جمع_کړه(s1, v_expand_لوړ(v0)); \
            s2 = v_جمع_کړه(s2, v_expand_ټیټ(v1)); \
            s0 = v_جمع_کړه(s0, v_expand_لوړ(v1)); \
            s1 = v_جمع_کړه(s1, v_expand_ټیټ(v2)); \
            s2 = v_جمع_کړه(s2, v_expand_لوړ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#تعریف_لغوه DEFINE_مجموعه_SIMD_16
#تعریف_کړه DEFINE_مجموعه_SIMD_16(T, ST, iST, VecT, بارول_op) \
کينډۍ<> بنية مجموعه_SIMD<T, ST> \
{ \
    مجموعه_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح چلوونکی ()(ثابت T* src, ثابت uchar* ماسک, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        إذا (ماسک || (cn < 1 || cn > 4)) \
            أرجع 0; \
        len *= cn; \
        صحيح x = 0, simd_پلنوالی = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_بارول(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_بارول(maskbuf + simd_پلنوالی); \
            m2 = vx_بارول(maskbuf + simd_پلنوالی*2); \
            m3 = vx_بارول(maskbuf + simd_پلنوالی*3); \
            m4 = vx_بارول(maskbuf + simd_پلنوالی*4); \
            m5 = vx_بارول(maskbuf + simd_پلنوالی*5); \
            m6 = vx_بارول(maskbuf + simd_پلنوالی*6); \
            m7 = vx_بارول(maskbuf + simd_پلنوالی*7); \
            m8 = vx_بارول(maskbuf + simd_پلنوالی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_پلنوالی*3; x += simd_پلنوالی*3) { \
            خپلکاری v0 = بارول_op(src + x); \
            خپلکاری v1 = بارول_op(src + x + simd_پلنوالی); \
            خپلکاری v2 = بارول_op(src + x + simd_پلنوالی*2); \
            s0 = v_جمع_کړه(s0, v0); \
            s1 = v_جمع_کړه(s1, v1); \
            s2 = v_جمع_کړه(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#تعریف_لغوه بارول_u8_as_s16
#تعریف_لغوه بارول_u16_as_s32
#تعریف_کړه بارول_u8_as_s16(addr) v_reinterpret_as_s16(vx_بارول_expand(addr))
#تعریف_کړه بارول_u16_as_s32(addr) v_reinterpret_as_s32(vx_بارول_expand(addr))

DEFINE_مجموعه_SIMD_8(uchar, صحيح, صحيح, v_int32, بارول_u8_as_s16)
DEFINE_مجموعه_SIMD_8(schar, صحيح, صحيح, v_int32, vx_بارول_expand)
DEFINE_مجموعه_SIMD_16(ushort, صحيح, صحيح, v_int32, بارول_u16_as_s32)
DEFINE_مجموعه_SIMD_16(لنډ, صحيح, صحيح, v_int32, vx_بارول_expand)
DEFINE_مجموعه_SIMD_16(hfloat, عائم, صحيح, v_float32, vx_بارول_expand)
DEFINE_مجموعه_SIMD_16(bfloat, عائم, صحيح, v_float32, vx_بارول_expand)

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#تعریف_لغوه DEFINE_مجموعه_SIMD_32
#تعریف_کړه DEFINE_مجموعه_SIMD_32(T, ST, iST, VecT) \
کينډۍ<> بنية مجموعه_SIMD<T, ST> \
{ \
    مجموعه_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح چلوونکی ()(ثابت T* src, ثابت uchar* ماسک, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        صحيح x = 0, simd_پلنوالی = VTraits<VecT>::vlanes(); \
        إذا (ماسک || (cn < 1 || cn > 3+(simd_پلنوالی>=4))) \
            أرجع 0; \
        len *= cn; \
        VecT m0 = vx_بارول(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_بارول(maskbuf + simd_پلنوالی); \
            m2 = vx_بارول(maskbuf + simd_پلنوالی*2); \
            m3 = vx_بارول(maskbuf + simd_پلنوالی*3); \
            m4 = vx_بارول(maskbuf + simd_پلنوالی*4); \
            m5 = vx_بارول(maskbuf + simd_پلنوالی*5); \
            m6 = vx_بارول(maskbuf + simd_پلنوالی*6); \
            m7 = vx_بارول(maskbuf + simd_پلنوالی*7); \
            m8 = vx_بارول(maskbuf + simd_پلنوالی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_پلنوالی*6; x += simd_پلنوالی*6) { \
            خپلکاری v0 = vx_بارول(src + x); \
            خپلکاری v1 = vx_بارول(src + x + simd_پلنوالی*2); \
            خپلکاری v2 = vx_بارول(src + x + simd_پلنوالی*4); \
            s0 = v_جمع_کړه(s0, v_cvt_f64(v0)); \
            s1 = v_جمع_کړه(s1, v_cvt_f64_لوړ(v0)); \
            s2 = v_جمع_کړه(s2, v_cvt_f64(v1)); \
            s0 = v_جمع_کړه(s0, v_cvt_f64_لوړ(v1)); \
            s1 = v_جمع_کړه(s1, v_cvt_f64(v2)); \
            s2 = v_جمع_کړه(s2, v_cvt_f64_لوړ(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_مجموعه_SIMD_32(صحيح, مزدوج, int64, v_float64)
DEFINE_مجموعه_SIMD_32(عائم, مزدوج, int64, v_float64)
#د_که_پای
#د_که_پای

کينډۍ<ډول_نوم T, ډول_نوم ST, ډول_نوم WT=T>
ساكن صحيح مجموعه_2(ثابت T* src0, ثابت uchar* ماسک, ST* dst, صحيح len, صحيح cn )
{
    ثابت T* src = src0;
    إذا( !ماسک )
    {
        مجموعه_SIMD<T, ST> vop(cn);
        صحيح i0 = vop(src0, ماسک, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        إذا( k == 1 )
        {
            ST s0 = dst[0];

            #إذا CV_ENABLE_UNROLLED
            لكل(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #د_که_پای
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
                #د_که_پای
                لكل( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    أرجع nzm;
}


ساكن صحيح sum8u( ثابت uchar* src, ثابت uchar* ماسک, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum8s( ثابت schar* src, ثابت uchar* ماسک, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum16u( ثابت ushort* src, ثابت uchar* ماسک, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum16s( ثابت لنډ* src, ثابت uchar* ماسک, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum32u( ثابت بینښه* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2<بینښه, مزدوج, uint64>(src, ماسک, dst, len, cn); }

ساكن صحيح sum32s( ثابت صحيح* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2<صحيح, مزدوج, int64>(src, ماسک, dst, len, cn); }

ساكن صحيح sum64u( ثابت uint64* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum64s( ثابت int64* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum32f( ثابت عائم* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum64f( ثابت مزدوج* src, ثابت uchar* ماسک, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2(src, ماسک, dst, len, cn); }

ساكن صحيح sum16f( ثابت hfloat* src, ثابت uchar* ماسک, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2<hfloat, عائم, عائم>(src, ماسک, dst, len, cn); }

ساكن صحيح sum16bf( ثابت bfloat* src, ثابت uchar* ماسک, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع مجموعه_2<bfloat, عائم, عائم>(src, ماسک, dst, len, cn); }

مجموعه_Func get_مجموعه_Func(صحيح ژوروالی)
{
    ساكن مجموعه_Func مجموعه_Tab[CV_ژوروالی_MAX] =
    {
        مجموعه_Wrap<uchar, صحيح, sum8u>,
        مجموعه_Wrap<schar, صحيح, sum8s>,
        مجموعه_Wrap<ushort, صحيح, sum16u>,
        مجموعه_Wrap<لنډ, صحيح, sum16s>,
        مجموعه_Wrap<صحيح, مزدوج, sum32s>,
        مجموعه_Wrap<عائم, مزدوج, sum32f>,
        مجموعه_Wrap<مزدوج, مزدوج, sum64f>,
        مجموعه_Wrap<hfloat, عائم, sum16f>,
        مجموعه_Wrap<bfloat, عائم, sum16bf>,
        0,
        مجموعه_Wrap<uint64, مزدوج, sum64u>,
        مجموعه_Wrap<int64, مزدوج, sum64s>,
        مجموعه_Wrap<بینښه, مزدوج, sum32u>,
        0
    };

    أرجع مجموعه_Tab[ژوروالی];
}

#د_که_پای

CV_CPU_OPTIMIZATION_NAMESPACE_پای_ټکی
} // namespace
