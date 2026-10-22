// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "stat.hpp"

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

سانچو<قسم_نالو T, قسم_نالو ST, صحيح (*fn)(ثابت T*, ثابت uchar*, ST*, صحيح, صحيح)>
ساكن صحيح جوڙ_Wrap(ثابت uchar* src, ثابت uchar* نقاب, uchar* جوڙ, صحيح len, صحيح cn)
{
    أرجع fn((ثابت T*)src, نقاب, (ST*)جوڙ, len, cn);
}

جوڙ_Func get_جوڙ_Func(صحيح اونهائي);

#جيڪڏهن_وصف_نه_ٿيل CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

سانچو <قسم_نالو T, قسم_نالو ST>
بنية جوڙ_SIMD
{
    جوڙ_SIMD(صحيح) {}
    صحيح عامل () (ثابت T*, ثابت uchar*, ST*, صحيح, صحيح) ثابت
    {
        أرجع 0;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

#وصف_رد REDUCE_PARTIAL_SUMS
#وصف_ڪريو REDUCE_PARTIAL_SUMS() \
    إذا (cn == 1) \
        dst[0] += v_reduce_جوڙ(v_جوڙيو(v_جوڙيو(s0, s1), s2)); \
    وإلا إذا (cn == 2) { \
        s0 = v_جوڙيو(v_جوڙيو(s0, s1), s2); \
        dst[0] += v_reduce_جوڙ(v_and(s0, m0)); \
        dst[1] += v_reduce_جوڙ(v_and(s0, m1)); \
    } وإلا إذا (cn == 3) { \
        dst[0] += v_reduce_جوڙ(v_جوڙيو(v_جوڙيو(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_جوڙ(v_جوڙيو(v_جوڙيو(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_جوڙ(v_جوڙيو(v_جوڙيو(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } وإلا إذا (cn == 4) { \
        s0 = v_جوڙيو(v_جوڙيو(s0, s1), s2); \
        dst[0] += v_reduce_جوڙ(v_and(s0, m0)); \
        dst[1] += v_reduce_جوڙ(v_and(s0, m1)); \
        dst[2] += v_reduce_جوڙ(v_and(s0, m2)); \
        dst[3] += v_reduce_جوڙ(v_and(s0, m3)); \
    }

سانچو<قسم_نالو ST>
ساكن فراغ init_maskbuf(ST* maskbuf, صحيح cn, صحيح simd_ويڪر)
{
    memset(maskbuf, 0, simd_ويڪر*9*حجم(maskbuf[0]));
    إذا (cn == 1)
        ;
    وإلا إذا (cn == 2)
        لكل (صحيح i = 0; i < simd_ويڪر; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ويڪر] = (ST)-1;
        }
    وإلا إذا (cn == 3)
        لكل (صحيح i = 0; i < simd_ويڪر*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ويڪر*3] = (ST)-1;
            maskbuf[i+2+simd_ويڪر*6] = (ST)-1;
        }
    وإلا إذا (cn == 4 && simd_ويڪر >= 4) {
        لكل (صحيح i = 0; i < simd_ويڪر; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_ويڪر] = (ST)-1;
            maskbuf[i+2+simd_ويڪر*2] = (ST)-1;
            maskbuf[i+3+simd_ويڪر*3] = (ST)-1;
        }
    }
}

#وصف_رد DEFINE_جوڙ_SIMD_8
#وصف_ڪريو DEFINE_جوڙ_SIMD_8(T, ST, iST, VecT, ڀريو_op) \
سانچو<> بنية جوڙ_SIMD<T, ST> \
{ \
    جوڙ_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        إذا (نقاب || (cn < 1 || cn > 4)) \
            أرجع 0; \
        len *= cn; \
        صحيح x = 0, simd_ويڪر = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_ڀريو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_ڀريو(maskbuf + simd_ويڪر); \
            m2 = vx_ڀريو(maskbuf + simd_ويڪر*2); \
            m3 = vx_ڀريو(maskbuf + simd_ويڪر*3); \
            m4 = vx_ڀريو(maskbuf + simd_ويڪر*4); \
            m5 = vx_ڀريو(maskbuf + simd_ويڪر*5); \
            m6 = vx_ڀريو(maskbuf + simd_ويڪر*6); \
            m7 = vx_ڀريو(maskbuf + simd_ويڪر*7); \
            m8 = vx_ڀريو(maskbuf + simd_ويڪر*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_ويڪر*6; x += simd_ويڪر*6) { \
            خودڪار v0 = ڀريو_op(src + x); \
            خودڪار v1 = ڀريو_op(src + x + simd_ويڪر*2); \
            خودڪار v2 = ڀريو_op(src + x + simd_ويڪر*4); \
            s0 = v_جوڙيو(s0, v_expand_هيٺيون(v0)); \
            s1 = v_جوڙيو(s1, v_expand_مٿاهون(v0)); \
            s2 = v_جوڙيو(s2, v_expand_هيٺيون(v1)); \
            s0 = v_جوڙيو(s0, v_expand_مٿاهون(v1)); \
            s1 = v_جوڙيو(s1, v_expand_هيٺيون(v2)); \
            s2 = v_جوڙيو(s2, v_expand_مٿاهون(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#وصف_رد DEFINE_جوڙ_SIMD_16
#وصف_ڪريو DEFINE_جوڙ_SIMD_16(T, ST, iST, VecT, ڀريو_op) \
سانچو<> بنية جوڙ_SIMD<T, ST> \
{ \
    جوڙ_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        إذا (نقاب || (cn < 1 || cn > 4)) \
            أرجع 0; \
        len *= cn; \
        صحيح x = 0, simd_ويڪر = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_ڀريو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_ڀريو(maskbuf + simd_ويڪر); \
            m2 = vx_ڀريو(maskbuf + simd_ويڪر*2); \
            m3 = vx_ڀريو(maskbuf + simd_ويڪر*3); \
            m4 = vx_ڀريو(maskbuf + simd_ويڪر*4); \
            m5 = vx_ڀريو(maskbuf + simd_ويڪر*5); \
            m6 = vx_ڀريو(maskbuf + simd_ويڪر*6); \
            m7 = vx_ڀريو(maskbuf + simd_ويڪر*7); \
            m8 = vx_ڀريو(maskbuf + simd_ويڪر*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_ويڪر*3; x += simd_ويڪر*3) { \
            خودڪار v0 = ڀريو_op(src + x); \
            خودڪار v1 = ڀريو_op(src + x + simd_ويڪر); \
            خودڪار v2 = ڀريو_op(src + x + simd_ويڪر*2); \
            s0 = v_جوڙيو(s0, v0); \
            s1 = v_جوڙيو(s1, v1); \
            s2 = v_جوڙيو(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#وصف_رد ڀريو_u8_as_s16
#وصف_رد ڀريو_u16_as_s32
#وصف_ڪريو ڀريو_u8_as_s16(addr) v_reinterpret_as_s16(vx_ڀريو_expand(addr))
#وصف_ڪريو ڀريو_u16_as_s32(addr) v_reinterpret_as_s32(vx_ڀريو_expand(addr))

DEFINE_جوڙ_SIMD_8(uchar, صحيح, صحيح, v_int32, ڀريو_u8_as_s16)
DEFINE_جوڙ_SIMD_8(schar, صحيح, صحيح, v_int32, vx_ڀريو_expand)
DEFINE_جوڙ_SIMD_16(ushort, صحيح, صحيح, v_int32, ڀريو_u16_as_s32)
DEFINE_جوڙ_SIMD_16(ننڍو, صحيح, صحيح, v_int32, vx_ڀريو_expand)
DEFINE_جوڙ_SIMD_16(hfloat, عائم, صحيح, v_float32, vx_ڀريو_expand)
DEFINE_جوڙ_SIMD_16(bfloat, عائم, صحيح, v_float32, vx_ڀريو_expand)

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#وصف_رد DEFINE_جوڙ_SIMD_32
#وصف_ڪريو DEFINE_جوڙ_SIMD_32(T, ST, iST, VecT) \
سانچو<> بنية جوڙ_SIMD<T, ST> \
{ \
    جوڙ_SIMD(صحيح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحيح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحيح len, صحيح cn) ثابت \
    { \
        صحيح x = 0, simd_ويڪر = VTraits<VecT>::vlanes(); \
        إذا (نقاب || (cn < 1 || cn > 3+(simd_ويڪر>=4))) \
            أرجع 0; \
        len *= cn; \
        VecT m0 = vx_ڀريو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        إذا (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } وإلا { \
            m1 = vx_ڀريو(maskbuf + simd_ويڪر); \
            m2 = vx_ڀريو(maskbuf + simd_ويڪر*2); \
            m3 = vx_ڀريو(maskbuf + simd_ويڪر*3); \
            m4 = vx_ڀريو(maskbuf + simd_ويڪر*4); \
            m5 = vx_ڀريو(maskbuf + simd_ويڪر*5); \
            m6 = vx_ڀريو(maskbuf + simd_ويڪر*6); \
            m7 = vx_ڀريو(maskbuf + simd_ويڪر*7); \
            m8 = vx_ڀريو(maskbuf + simd_ويڪر*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        لكل (; x <= len - simd_ويڪر*6; x += simd_ويڪر*6) { \
            خودڪار v0 = vx_ڀريو(src + x); \
            خودڪار v1 = vx_ڀريو(src + x + simd_ويڪر*2); \
            خودڪار v2 = vx_ڀريو(src + x + simd_ويڪر*4); \
            s0 = v_جوڙيو(s0, v_cvt_f64(v0)); \
            s1 = v_جوڙيو(s1, v_cvt_f64_مٿاهون(v0)); \
            s2 = v_جوڙيو(s2, v_cvt_f64(v1)); \
            s0 = v_جوڙيو(s0, v_cvt_f64_مٿاهون(v1)); \
            s1 = v_جوڙيو(s1, v_cvt_f64(v2)); \
            s2 = v_جوڙيو(s2, v_cvt_f64_مٿاهون(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        أرجع x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_جوڙ_SIMD_32(صحيح, مزدوج, int64, v_float64)
DEFINE_جوڙ_SIMD_32(عائم, مزدوج, int64, v_float64)
#جيڪڏهن_پورو
#جيڪڏهن_پورو

سانچو<قسم_نالو T, قسم_نالو ST, قسم_نالو WT=T>
ساكن صحيح جوڙ_2(ثابت T* src0, ثابت uchar* نقاب, ST* dst, صحيح len, صحيح cn )
{
    ثابت T* src = src0;
    إذا( !نقاب )
    {
        جوڙ_SIMD<T, ST> vop(cn);
        صحيح i0 = vop(src0, نقاب, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        إذا( k == 1 )
        {
            ST s0 = dst[0];

            #إذا CV_ENABLE_UNROLLED
            لكل(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #جيڪڏهن_پورو
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
                #جيڪڏهن_پورو
                لكل( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    أرجع nzm;
}


ساكن صحيح sum8u( ثابت uchar* src, ثابت uchar* نقاب, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum8s( ثابت schar* src, ثابت uchar* نقاب, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum16u( ثابت ushort* src, ثابت uchar* نقاب, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum16s( ثابت ننڍو* src, ثابت uchar* نقاب, صحيح* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum32u( ثابت بنانشان* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2<بنانشان, مزدوج, uint64>(src, نقاب, dst, len, cn); }

ساكن صحيح sum32s( ثابت صحيح* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2<صحيح, مزدوج, int64>(src, نقاب, dst, len, cn); }

ساكن صحيح sum64u( ثابت uint64* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum64s( ثابت int64* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum32f( ثابت عائم* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum64f( ثابت مزدوج* src, ثابت uchar* نقاب, مزدوج* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2(src, نقاب, dst, len, cn); }

ساكن صحيح sum16f( ثابت hfloat* src, ثابت uchar* نقاب, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2<hfloat, عائم, عائم>(src, نقاب, dst, len, cn); }

ساكن صحيح sum16bf( ثابت bfloat* src, ثابت uchar* نقاب, عائم* dst, صحيح len, صحيح cn )
{ CV_INSTRUMENT_REGION(); أرجع جوڙ_2<bfloat, عائم, عائم>(src, نقاب, dst, len, cn); }

جوڙ_Func get_جوڙ_Func(صحيح اونهائي)
{
    ساكن جوڙ_Func جوڙ_Tab[CV_اونهائي_MAX] =
    {
        جوڙ_Wrap<uchar, صحيح, sum8u>,
        جوڙ_Wrap<schar, صحيح, sum8s>,
        جوڙ_Wrap<ushort, صحيح, sum16u>,
        جوڙ_Wrap<ننڍو, صحيح, sum16s>,
        جوڙ_Wrap<صحيح, مزدوج, sum32s>,
        جوڙ_Wrap<عائم, مزدوج, sum32f>,
        جوڙ_Wrap<مزدوج, مزدوج, sum64f>,
        جوڙ_Wrap<hfloat, عائم, sum16f>,
        جوڙ_Wrap<bfloat, عائم, sum16bf>,
        0,
        جوڙ_Wrap<uint64, مزدوج, sum64u>,
        جوڙ_Wrap<int64, مزدوج, sum64s>,
        جوڙ_Wrap<بنانشان, مزدوج, sum32u>,
        0
    };

    أرجع جوڙ_Tab[اونهائي];
}

#جيڪڏهن_پورو

CV_CPU_OPTIMIZATION_NAMESPACE_پڇاڙي
} // namespace
