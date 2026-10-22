// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"

نامخلا cv { نامخلا hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

خالی split8u(ثابت uchar* src, uchar** dst, صحیح len, صحیح cn);
خالی split16u(ثابت ushort* src, ushort** dst, صحیح len, صحیح cn);
خالی split32s(ثابت صحیح* src, صحیح** dst, صحیح len, صحیح cn);
خالی split64s(ثابت int64* src, int64** dst, صحیح len, صحیح cn);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
سانچہ<قسم_نام T, قسم_نام VecT> ساکن خالی
vecsplit_( ثابت T* src, T** dst, صحیح len, صحیح cn )
{
    ثابت صحیح VECSZ = VTraits<VecT>::vlanes();
    صحیح i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    صحیح r0 = (صحیح)((size_t)(خالی*)dst0 % (VECSZ*حجم(T)));
    صحیح r1 = (صحیح)((size_t)(خالی*)dst1 % (VECSZ*حجم(T)));
    صحیح r2 = cn > 2 ? (صحیح)((size_t)(خالی*)dst[2] % (VECSZ*حجم(T))) : r0;
    صحیح r3 = cn > 3 ? (صحیح)((size_t)(خالی*)dst[3] % (VECSZ*حجم(T))) : r0;

    hal::Store_منوال منوال = hal::STORE_ALIGNED_NOCACHE;
    اگر( (r0|r1|r2|r3) != 0 )
    {
        منوال = hal::STORE_UNALIGNED;
        اگر( r0 == r1 && r0 == r2 && r0 == r3 && r0 % حجم(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / حجم(T));
    }

    اگر( cn == 2 )
    {
        برائے( i = 0; i < len; i += VECSZ )
        {
            اگر( i > len - VECSZ )
            {
                i = len - VECSZ;
                منوال = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_بھرو_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, منوال);
            v_store(dst1 + i, b, منوال);
            اگر( i < i0 )
            {
                i = i0 - VECSZ;
                منوال = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ورنہ اگر( cn == 3 )
    {
        T* dst2 = dst[2];
        برائے( i = 0; i < len; i += VECSZ )
        {
            اگر( i > len - VECSZ )
            {
                i = len - VECSZ;
                منوال = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_بھرو_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, منوال);
            v_store(dst1 + i, b, منوال);
            v_store(dst2 + i, c, منوال);
            اگر( i < i0 )
            {
                i = i0 - VECSZ;
                منوال = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ورنہ
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        برائے( i = 0; i < len; i += VECSZ )
        {
            اگر( i > len - VECSZ )
            {
                i = len - VECSZ;
                منوال = hal::STORE_UNALIGNED;
            }
            VecT a, b, c, d;
            v_بھرو_deinterleave(src + i*cn, a, b, c, d);
            v_store(dst0 + i, a, منوال);
            v_store(dst1 + i, b, منوال);
            v_store(dst2 + i, c, منوال);
            v_store(dst3 + i, d, منوال);
            اگر( i < i0 )
            {
                i = i0 - VECSZ;
                منوال = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#اگر_ختم

سانچہ<قسم_نام T> ساکن خالی
بانٹو_2( ثابت T* src, T** dst, صحیح len, صحیح cn )
{
    صحیح k = cn % 4 ? cn % 4 : 4;
    صحیح i, j;
    اگر( k == 1 )
    {
        T* dst0 = dst[0];

        اگر(cn == 1)
        {
            حافظہ_نقل(dst0, src, len * حجم(T));
        }
        ورنہ
        {
            برائے( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    ورنہ اگر( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        برائے( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    ورنہ اگر( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        برائے( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    ورنہ
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        برائے( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    برائے( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        برائے( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

خالی split8u(ثابت uchar* src, uchar** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    ورنہ
#اگر_ختم
        بانٹو_2(src, dst, len, cn);
}

خالی split16u(ثابت ushort* src, ushort** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    ورنہ
#اگر_ختم
        بانٹو_2(src, dst, len, cn);
}

خالی split32s(ثابت صحیح* src, صحیح** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<صحیح, v_int32>(src, dst, len, cn);
    ورنہ
#اگر_ختم
        بانٹو_2(src, dst, len, cn);
}

خالی split64s(ثابت int64* src, int64** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    ورنہ
#اگر_ختم
        بانٹو_2(src, dst, len, cn);
}

#اگر_ختم
CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
}} // namespace
