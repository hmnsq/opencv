// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "stat.hpp"

نامخلا cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

سانچہ<قسم_نام T, قسم_نام ST, صحیح (*fn)(ثابت T*, ثابت uchar*, ST*, صحیح, صحیح)>
ساکن صحیح مجموع_Wrap(ثابت uchar* src, ثابت uchar* نقاب, uchar* مجموع, صحیح len, صحیح cn)
{
    واپس fn((ثابت T*)src, نقاب, (ST*)مجموع, len, cn);
}

مجموع_Func get_مجموع_Func(صحیح گہرائی);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

سانچہ <قسم_نام T, قسم_نام ST>
ساخت مجموع_SIMD
{
    مجموع_SIMD(صحیح) {}
    صحیح عامل () (ثابت T*, ثابت uchar*, ST*, صحیح, صحیح) ثابت
    {
        واپس 0;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

#تعریف_منسوخ REDUCE_PARTIAL_SUMS
#تعریف_کرو REDUCE_PARTIAL_SUMS() \
    اگر (cn == 1) \
        dst[0] += v_reduce_مجموع(v_جوڑو(v_جوڑو(s0, s1), s2)); \
    ورنہ اگر (cn == 2) { \
        s0 = v_جوڑو(v_جوڑو(s0, s1), s2); \
        dst[0] += v_reduce_مجموع(v_and(s0, m0)); \
        dst[1] += v_reduce_مجموع(v_and(s0, m1)); \
    } ورنہ اگر (cn == 3) { \
        dst[0] += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_and(s0, m0), v_and(s1, m1)), v_and(s2, m2))); \
        dst[1] += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_and(s0, m3), v_and(s1, m4)), v_and(s2, m5))); \
        dst[2] += v_reduce_مجموع(v_جوڑو(v_جوڑو(v_and(s0, m6), v_and(s1, m7)), v_and(s2, m8))); \
    } ورنہ اگر (cn == 4) { \
        s0 = v_جوڑو(v_جوڑو(s0, s1), s2); \
        dst[0] += v_reduce_مجموع(v_and(s0, m0)); \
        dst[1] += v_reduce_مجموع(v_and(s0, m1)); \
        dst[2] += v_reduce_مجموع(v_and(s0, m2)); \
        dst[3] += v_reduce_مجموع(v_and(s0, m3)); \
    }

سانچہ<قسم_نام ST>
ساکن خالی init_maskbuf(ST* maskbuf, صحیح cn, صحیح simd_چوڑائی)
{
    memset(maskbuf, 0, simd_چوڑائی*9*حجم(maskbuf[0]));
    اگر (cn == 1)
        ;
    ورنہ اگر (cn == 2)
        برائے (صحیح i = 0; i < simd_چوڑائی; i += 2) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_چوڑائی] = (ST)-1;
        }
    ورنہ اگر (cn == 3)
        برائے (صحیح i = 0; i < simd_چوڑائی*3; i += 3) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_چوڑائی*3] = (ST)-1;
            maskbuf[i+2+simd_چوڑائی*6] = (ST)-1;
        }
    ورنہ اگر (cn == 4 && simd_چوڑائی >= 4) {
        برائے (صحیح i = 0; i < simd_چوڑائی; i += 4) {
            maskbuf[i] = (ST)-1;
            maskbuf[i+1+simd_چوڑائی] = (ST)-1;
            maskbuf[i+2+simd_چوڑائی*2] = (ST)-1;
            maskbuf[i+3+simd_چوڑائی*3] = (ST)-1;
        }
    }
}

#تعریف_منسوخ DEFINE_مجموع_SIMD_8
#تعریف_کرو DEFINE_مجموع_SIMD_8(T, ST, iST, VecT, بھرو_op) \
سانچہ<> ساخت مجموع_SIMD<T, ST> \
{ \
    مجموع_SIMD(صحیح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحیح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحیح len, صحیح cn) ثابت \
    { \
        اگر (نقاب || (cn < 1 || cn > 4)) \
            واپس 0; \
        len *= cn; \
        صحیح x = 0, simd_چوڑائی = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_بھرو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        اگر (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ورنہ { \
            m1 = vx_بھرو(maskbuf + simd_چوڑائی); \
            m2 = vx_بھرو(maskbuf + simd_چوڑائی*2); \
            m3 = vx_بھرو(maskbuf + simd_چوڑائی*3); \
            m4 = vx_بھرو(maskbuf + simd_چوڑائی*4); \
            m5 = vx_بھرو(maskbuf + simd_چوڑائی*5); \
            m6 = vx_بھرو(maskbuf + simd_چوڑائی*6); \
            m7 = vx_بھرو(maskbuf + simd_چوڑائی*7); \
            m8 = vx_بھرو(maskbuf + simd_چوڑائی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        برائے (; x <= len - simd_چوڑائی*6; x += simd_چوڑائی*6) { \
            خودکار v0 = بھرو_op(src + x); \
            خودکار v1 = بھرو_op(src + x + simd_چوڑائی*2); \
            خودکار v2 = بھرو_op(src + x + simd_چوڑائی*4); \
            s0 = v_جوڑو(s0, v_expand_کم(v0)); \
            s1 = v_جوڑو(s1, v_expand_بلند(v0)); \
            s2 = v_جوڑو(s2, v_expand_کم(v1)); \
            s0 = v_جوڑو(s0, v_expand_بلند(v1)); \
            s1 = v_جوڑو(s1, v_expand_کم(v2)); \
            s2 = v_جوڑو(s2, v_expand_بلند(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        واپس x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#تعریف_منسوخ DEFINE_مجموع_SIMD_16
#تعریف_کرو DEFINE_مجموع_SIMD_16(T, ST, iST, VecT, بھرو_op) \
سانچہ<> ساخت مجموع_SIMD<T, ST> \
{ \
    مجموع_SIMD(صحیح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحیح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحیح len, صحیح cn) ثابت \
    { \
        اگر (نقاب || (cn < 1 || cn > 4)) \
            واپس 0; \
        len *= cn; \
        صحیح x = 0, simd_چوڑائی = VTraits<VecT>::vlanes(); \
        VecT m0 = vx_بھرو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        اگر (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ورنہ { \
            m1 = vx_بھرو(maskbuf + simd_چوڑائی); \
            m2 = vx_بھرو(maskbuf + simd_چوڑائی*2); \
            m3 = vx_بھرو(maskbuf + simd_چوڑائی*3); \
            m4 = vx_بھرو(maskbuf + simd_چوڑائی*4); \
            m5 = vx_بھرو(maskbuf + simd_چوڑائی*5); \
            m6 = vx_بھرو(maskbuf + simd_چوڑائی*6); \
            m7 = vx_بھرو(maskbuf + simd_چوڑائی*7); \
            m8 = vx_بھرو(maskbuf + simd_چوڑائی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        برائے (; x <= len - simd_چوڑائی*3; x += simd_چوڑائی*3) { \
            خودکار v0 = بھرو_op(src + x); \
            خودکار v1 = بھرو_op(src + x + simd_چوڑائی); \
            خودکار v2 = بھرو_op(src + x + simd_چوڑائی*2); \
            s0 = v_جوڑو(s0, v0); \
            s1 = v_جوڑو(s1, v1); \
            s2 = v_جوڑو(s2, v2); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        واپس x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

#تعریف_منسوخ بھرو_u8_as_s16
#تعریف_منسوخ بھرو_u16_as_s32
#تعریف_کرو بھرو_u8_as_s16(addr) v_reinterpret_as_s16(vx_بھرو_expand(addr))
#تعریف_کرو بھرو_u16_as_s32(addr) v_reinterpret_as_s32(vx_بھرو_expand(addr))

DEFINE_مجموع_SIMD_8(uchar, صحیح, صحیح, v_int32, بھرو_u8_as_s16)
DEFINE_مجموع_SIMD_8(schar, صحیح, صحیح, v_int32, vx_بھرو_expand)
DEFINE_مجموع_SIMD_16(ushort, صحیح, صحیح, v_int32, بھرو_u16_as_s32)
DEFINE_مجموع_SIMD_16(مختصر, صحیح, صحیح, v_int32, vx_بھرو_expand)
DEFINE_مجموع_SIMD_16(hfloat, اعشاری, صحیح, v_float32, vx_بھرو_expand)
DEFINE_مجموع_SIMD_16(bfloat, اعشاری, صحیح, v_float32, vx_بھرو_expand)

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

#تعریف_منسوخ DEFINE_مجموع_SIMD_32
#تعریف_کرو DEFINE_مجموع_SIMD_32(T, ST, iST, VecT) \
سانچہ<> ساخت مجموع_SIMD<T, ST> \
{ \
    مجموع_SIMD(صحیح cn) \
    { \
        init_maskbuf((iST*)maskbuf, cn, VTraits<VecT>::vlanes()); \
    } \
    صحیح عامل ()(ثابت T* src, ثابت uchar* نقاب, ST* dst, صحیح len, صحیح cn) ثابت \
    { \
        صحیح x = 0, simd_چوڑائی = VTraits<VecT>::vlanes(); \
        اگر (نقاب || (cn < 1 || cn > 3+(simd_چوڑائی>=4))) \
            واپس 0; \
        len *= cn; \
        VecT m0 = vx_بھرو(maskbuf), m1, m2, m3, m4, m5, m6, m7, m8; \
        اگر (cn == 1) { \
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m0; \
        } ورنہ { \
            m1 = vx_بھرو(maskbuf + simd_چوڑائی); \
            m2 = vx_بھرو(maskbuf + simd_چوڑائی*2); \
            m3 = vx_بھرو(maskbuf + simd_چوڑائی*3); \
            m4 = vx_بھرو(maskbuf + simd_چوڑائی*4); \
            m5 = vx_بھرو(maskbuf + simd_چوڑائی*5); \
            m6 = vx_بھرو(maskbuf + simd_چوڑائی*6); \
            m7 = vx_بھرو(maskbuf + simd_چوڑائی*7); \
            m8 = vx_بھرو(maskbuf + simd_چوڑائی*8); \
        } \
        VecT s0 = v_xor(m0, m0), s1 = s0, s2 = s0; \
        برائے (; x <= len - simd_چوڑائی*6; x += simd_چوڑائی*6) { \
            خودکار v0 = vx_بھرو(src + x); \
            خودکار v1 = vx_بھرو(src + x + simd_چوڑائی*2); \
            خودکار v2 = vx_بھرو(src + x + simd_چوڑائی*4); \
            s0 = v_جوڑو(s0, v_cvt_f64(v0)); \
            s1 = v_جوڑو(s1, v_cvt_f64_بلند(v0)); \
            s2 = v_جوڑو(s2, v_cvt_f64(v1)); \
            s0 = v_جوڑو(s0, v_cvt_f64_بلند(v1)); \
            s1 = v_جوڑو(s1, v_cvt_f64(v2)); \
            s2 = v_جوڑو(s2, v_cvt_f64_بلند(v2)); \
        } \
        REDUCE_PARTIAL_SUMS(); \
        vx_cleanup(); \
        واپس x / cn; \
    } \
    ST maskbuf[VTraits<VecT>::max_nlanes*9]; \
};

DEFINE_مجموع_SIMD_32(صحیح, دوہرا, int64, v_float64)
DEFINE_مجموع_SIMD_32(اعشاری, دوہرا, int64, v_float64)
#اگر_ختم
#اگر_ختم

سانچہ<قسم_نام T, قسم_نام ST, قسم_نام WT=T>
ساکن صحیح مجموع_2(ثابت T* src0, ثابت uchar* نقاب, ST* dst, صحیح len, صحیح cn )
{
    ثابت T* src = src0;
    اگر( !نقاب )
    {
        مجموع_SIMD<T, ST> vop(cn);
        صحیح i0 = vop(src0, نقاب, dst, len, cn), i = i0, k = cn % 4;
        src += i0 * cn;

        اگر( k == 1 )
        {
            ST s0 = dst[0];

            #اگر CV_ENABLE_UNROLLED
            برائے(; i <= len - 4; i += 4, src += cn*4 )
                s0 += (WT)src[0] + (WT)src[cn] + (WT)src[cn*2] + (WT)src[cn*3];
            #اگر_ختم
            برائے( ; i < len; i++, src += cn )
                s0 += (WT)src[0];
            dst[0] = s0;
        }
        ورنہ اگر( k == 2 )
        {
            ST s0 = dst[0], s1 = dst[1];
            برائے( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
            }
            dst[0] = s0;
            dst[1] = s1;
        }
        ورنہ اگر( k == 3 )
        {
            ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
            برائے( ; i < len; i++, src += cn )
            {
                s0 += (WT)src[0];
                s1 += (WT)src[1];
                s2 += (WT)src[2];
            }
            dst[0] = s0;
            dst[1] = s1;
            dst[2] = s2;
        }

        برائے( ; k < cn; k += 4 )
        {
            src = src0 + i0*cn + k;
            ST s0 = dst[k], s1 = dst[k+1], s2 = dst[k+2], s3 = dst[k+3];
            برائے( i = i0; i < len; i++, src += cn )
            {
                s0 += (WT)src[0]; s1 += (WT)src[1];
                s2 += (WT)src[2]; s3 += (WT)src[3];
            }
            dst[k] = s0;
            dst[k+1] = s1;
            dst[k+2] = s2;
            dst[k+3] = s3;
        }
        واپس len;
    }

    صحیح i, nzm = 0;
    اگر( cn == 1 )
    {
        ST s = dst[0];
        برائے( i = 0; i < len; i++ )
            اگر( نقاب[i] )
            {
                s += (WT)src[i];
                nzm++;
            }
        dst[0] = s;
    }
    ورنہ اگر( cn == 3 )
    {
        ST s0 = dst[0], s1 = dst[1], s2 = dst[2];
        برائے( i = 0; i < len; i++, src += 3 )
            اگر( نقاب[i] )
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
    ورنہ
    {
        برائے( i = 0; i < len; i++, src += cn )
            اگر( نقاب[i] )
            {
                صحیح k = 0;
                #اگر CV_ENABLE_UNROLLED
                برائے( ; k <= cn - 4; k += 4 )
                {
                    ST s0, s1;
                    s0 = dst[k] + (WT)src[k];
                    s1 = dst[k+1] + (WT)src[k+1];
                    dst[k] = s0; dst[k+1] = s1;
                    s0 = dst[k+2] + (WT)src[k+2];
                    s1 = dst[k+3] + (WT)src[k+3];
                    dst[k+2] = s0; dst[k+3] = s1;
                }
                #اگر_ختم
                برائے( ; k < cn; k++ )
                    dst[k] += (WT)src[k];
                nzm++;
            }
    }
    واپس nzm;
}


ساکن صحیح sum8u( ثابت uchar* src, ثابت uchar* نقاب, صحیح* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2(src, نقاب, dst, len, cn); }

ساکن صحیح sum8s( ثابت schar* src, ثابت uchar* نقاب, صحیح* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2(src, نقاب, dst, len, cn); }

ساکن صحیح sum16u( ثابت ushort* src, ثابت uchar* نقاب, صحیح* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2(src, نقاب, dst, len, cn); }

ساکن صحیح sum16s( ثابت مختصر* src, ثابت uchar* نقاب, صحیح* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2(src, نقاب, dst, len, cn); }

ساکن صحیح sum32u( ثابت غیرعلامتی* src, ثابت uchar* نقاب, دوہرا* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2<غیرعلامتی, دوہرا, uint64>(src, نقاب, dst, len, cn); }

ساکن صحیح sum32s( ثابت صحیح* src, ثابت uchar* نقاب, دوہرا* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2<صحیح, دوہرا, int64>(src, نقاب, dst, len, cn); }

ساکن صحیح sum64u( ثابت uint64* src, ثابت uchar* نقاب, دوہرا* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2(src, نقاب, dst, len, cn); }

ساکن صحیح sum64s( ثابت int64* src, ثابت uchar* نقاب, دوہرا* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2(src, نقاب, dst, len, cn); }

ساکن صحیح sum32f( ثابت اعشاری* src, ثابت uchar* نقاب, دوہرا* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2(src, نقاب, dst, len, cn); }

ساکن صحیح sum64f( ثابت دوہرا* src, ثابت uchar* نقاب, دوہرا* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2(src, نقاب, dst, len, cn); }

ساکن صحیح sum16f( ثابت hfloat* src, ثابت uchar* نقاب, اعشاری* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2<hfloat, اعشاری, اعشاری>(src, نقاب, dst, len, cn); }

ساکن صحیح sum16bf( ثابت bfloat* src, ثابت uchar* نقاب, اعشاری* dst, صحیح len, صحیح cn )
{ CV_INSTRUMENT_REGION(); واپس مجموع_2<bfloat, اعشاری, اعشاری>(src, نقاب, dst, len, cn); }

مجموع_Func get_مجموع_Func(صحیح گہرائی)
{
    ساکن مجموع_Func مجموع_Tab[CV_گہرائی_MAX] =
    {
        مجموع_Wrap<uchar, صحیح, sum8u>,
        مجموع_Wrap<schar, صحیح, sum8s>,
        مجموع_Wrap<ushort, صحیح, sum16u>,
        مجموع_Wrap<مختصر, صحیح, sum16s>,
        مجموع_Wrap<صحیح, دوہرا, sum32s>,
        مجموع_Wrap<اعشاری, دوہرا, sum32f>,
        مجموع_Wrap<دوہرا, دوہرا, sum64f>,
        مجموع_Wrap<hfloat, اعشاری, sum16f>,
        مجموع_Wrap<bfloat, اعشاری, sum16bf>,
        0,
        مجموع_Wrap<uint64, دوہرا, sum64u>,
        مجموع_Wrap<int64, دوہرا, sum64s>,
        مجموع_Wrap<غیرعلامتی, دوہرا, sum32u>,
        0
    };

    واپس مجموع_Tab[گہرائی];
}

#اگر_ختم

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
} // namespace
