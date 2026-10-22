// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"

نامخلا cv { نامخلا hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

خالی merge8u(ثابت uchar** src, uchar* dst, صحیح len, صحیح cn);
خالی merge16u(ثابت ushort** src, ushort* dst, صحیح len, صحیح cn);
خالی merge32s(ثابت صحیح** src, صحیح* dst, صحیح len, صحیح cn);
خالی merge64s(ثابت int64** src, int64* dst, صحیح len, صحیح cn);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
/*
  The trick with STORE_UNALIGNED/STORE_ALIGNED_NOCACHE is the following:
  on IA there are instructions movntps and such to which
  v_store_interleave(...., STORE_ALIGNED_NOCACHE) is mapped.
  Those instructions write directly into memory w/o touching cache
  that results in dramatic speed improvements, especially on
  large arrays (FullHD, 4K etc.).

  Those intrinsics require the destination address to be aligned
  by 16/32 bits (with SSE2 and AVX2, respectively).
  So we potentially split the processing into 3 stages:
  1) the optional prefix part [0:i0), where we use simple unaligned stores.
  2) the optional main part [i0:len - VECSZ], where we use "nocache" mode.
     But in some cases we have to use unaligned stores in this part.
  3) the optional suffix part (the tail) (len - VECSZ:len) where we switch back to "unaligned" mode
     to process the remaining len - VECSZ elements.
  In principle there can be very poorly aligned data where there is no main part.
  For that we set i0=0 and use unaligned stores for the whole array.
*/
سانچہ<قسم_نام T, قسم_نام VecT> ساکن خالی
vecmerge_( ثابت T** src, T* dst, صحیح len, صحیح cn )
{
    ثابت صحیح VECSZ = VTraits<VecT>::vlanes();
    صحیح i, i0 = 0;
    ثابت T* src0 = src[0];
    ثابت T* src1 = src[1];

    ثابت صحیح dst_Elem_سائز = cn * حجم(T);
    صحیح r = (صحیح)((size_t)(خالی*)dst % (VECSZ*حجم(T)));
    hal::Store_منوال منوال = hal::STORE_ALIGNED_NOCACHE;
    اگر( r != 0 )
    {
        منوال = hal::STORE_UNALIGNED;
        اگر (r % dst_Elem_سائز == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dst_Elem_سائز);
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
            VecT a = vx_بھرو(src0 + i), b = vx_بھرو(src1 + i);
            v_store_interleave(dst + i*cn, a, b, منوال);
            اگر( i < i0 )
            {
                i = i0 - VECSZ;
                منوال = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ورنہ اگر( cn == 3 )
    {
        ثابت T* src2 = src[2];
        برائے( i = 0; i < len; i += VECSZ )
        {
            اگر( i > len - VECSZ )
            {
                i = len - VECSZ;
                منوال = hal::STORE_UNALIGNED;
            }
            VecT a = vx_بھرو(src0 + i), b = vx_بھرو(src1 + i), c = vx_بھرو(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, منوال);
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
        ثابت T* src2 = src[2];
        ثابت T* src3 = src[3];
        برائے( i = 0; i < len; i += VECSZ )
        {
            اگر( i > len - VECSZ )
            {
                i = len - VECSZ;
                منوال = hal::STORE_UNALIGNED;
            }
            VecT a = vx_بھرو(src0 + i), b = vx_بھرو(src1 + i);
            VecT c = vx_بھرو(src2 + i), d = vx_بھرو(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, منوال);
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
ملاؤ_2( ثابت T** src, T* dst, صحیح len, صحیح cn )
{
    صحیح k = cn % 4 ? cn % 4 : 4;
    صحیح i, j;
    اگر( k == 1 )
    {
        ثابت T* src0 = src[0];
        برائے( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    ورنہ اگر( k == 2 )
    {
        ثابت T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        برائے( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    ورنہ اگر( k == 3 )
    {
        ثابت T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        برائے( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    ورنہ
    {
        ثابت T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        برائے( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    برائے( ; k < cn; k += 4 )
    {
        ثابت T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        برائے( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

خالی merge8u(ثابت uchar** src, uchar* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    ورنہ
#اگر_ختم
        ملاؤ_2(src, dst, len, cn);
}

خالی merge16u(ثابت ushort** src, ushort* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    ورنہ
#اگر_ختم
        ملاؤ_2(src, dst, len, cn);
}

خالی merge32s(ثابت صحیح** src, صحیح* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<صحیح, v_int32>(src, dst, len, cn);
    ورنہ
#اگر_ختم
        ملاؤ_2(src, dst, len, cn);
}

خالی merge64s(ثابت int64** src, int64* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    ورنہ
#اگر_ختم
        ملاؤ_2(src, dst, len, cn);
}

#اگر_ختم
CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
}} // namespace
