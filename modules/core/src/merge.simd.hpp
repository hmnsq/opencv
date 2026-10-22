// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"

نطاق cv { نطاق hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

فراغ merge8u(ثابت uchar** src, uchar* dst, صحيح len, صحيح cn);
فراغ merge16u(ثابت ushort** src, ushort* dst, صحيح len, صحيح cn);
فراغ merge32s(ثابت صحيح** src, صحيح* dst, صحيح len, صحيح cn);
فراغ merge64s(ثابت int64** src, int64* dst, صحيح len, صحيح cn);

#جے_بنا_تعریف CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
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
سانچا<قسم_ناں T, قسم_ناں VecT> ساكن فراغ
vecmerge_( ثابت T** src, T* dst, صحيح len, صحيح cn )
{
    ثابت صحيح VECSZ = VTraits<VecT>::vlanes();
    صحيح i, i0 = 0;
    ثابت T* src0 = src[0];
    ثابت T* src1 = src[1];

    ثابت صحيح dst_Elem_ناپ = cn * حجم(T);
    صحيح r = (صحيح)((size_t)(فراغ*)dst % (VECSZ*حجم(T)));
    hal::Store_منوال منوال = hal::STORE_ALIGNED_NOCACHE;
    إذا( r != 0 )
    {
        منوال = hal::STORE_UNALIGNED;
        إذا (r % dst_Elem_ناپ == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dst_Elem_ناپ);
    }

    إذا( cn == 2 )
    {
        لكل( i = 0; i < len; i += VECSZ )
        {
            إذا( i > len - VECSZ )
            {
                i = len - VECSZ;
                منوال = hal::STORE_UNALIGNED;
            }
            VecT a = vx_لدو(src0 + i), b = vx_لدو(src1 + i);
            v_store_interleave(dst + i*cn, a, b, منوال);
            إذا( i < i0 )
            {
                i = i0 - VECSZ;
                منوال = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    وإلا إذا( cn == 3 )
    {
        ثابت T* src2 = src[2];
        لكل( i = 0; i < len; i += VECSZ )
        {
            إذا( i > len - VECSZ )
            {
                i = len - VECSZ;
                منوال = hal::STORE_UNALIGNED;
            }
            VecT a = vx_لدو(src0 + i), b = vx_لدو(src1 + i), c = vx_لدو(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, منوال);
            إذا( i < i0 )
            {
                i = i0 - VECSZ;
                منوال = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    وإلا
    {
        CV_Assert( cn == 4 );
        ثابت T* src2 = src[2];
        ثابت T* src3 = src[3];
        لكل( i = 0; i < len; i += VECSZ )
        {
            إذا( i > len - VECSZ )
            {
                i = len - VECSZ;
                منوال = hal::STORE_UNALIGNED;
            }
            VecT a = vx_لدو(src0 + i), b = vx_لدو(src1 + i);
            VecT c = vx_لدو(src2 + i), d = vx_لدو(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, منوال);
            إذا( i < i0 )
            {
                i = i0 - VECSZ;
                منوال = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#جے_انت

سانچا<قسم_ناں T> ساكن فراغ
ملاؤ_2( ثابت T** src, T* dst, صحيح len, صحيح cn )
{
    صحيح k = cn % 4 ? cn % 4 : 4;
    صحيح i, j;
    إذا( k == 1 )
    {
        ثابت T* src0 = src[0];
        لكل( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    وإلا إذا( k == 2 )
    {
        ثابت T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        لكل( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    وإلا إذا( k == 3 )
    {
        ثابت T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        لكل( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    وإلا
    {
        ثابت T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        لكل( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    لكل( ; k < cn; k += 4 )
    {
        ثابت T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        لكل( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

فراغ merge8u(ثابت uchar** src, uchar* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    وإلا
#جے_انت
        ملاؤ_2(src, dst, len, cn);
}

فراغ merge16u(ثابت ushort** src, ushort* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    وإلا
#جے_انت
        ملاؤ_2(src, dst, len, cn);
}

فراغ merge32s(ثابت صحيح** src, صحيح* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<صحيح, v_int32>(src, dst, len, cn);
    وإلا
#جے_انت
        ملاؤ_2(src, dst, len, cn);
}

فراغ merge64s(ثابت int64** src, int64* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    وإلا
#جے_انت
        ملاؤ_2(src, dst, len, cn);
}

#جے_انت
CV_CPU_OPTIMIZATION_NAMESPACE_سرا
}} // namespace
