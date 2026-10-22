// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਖਾਲੀ merge8u(ਅਚਲ uchar** src, uchar* dst, ਪੂਰਨ len, ਪੂਰਨ cn);
ਖਾਲੀ merge16u(ਅਚਲ ushort** src, ushort* dst, ਪੂਰਨ len, ਪੂਰਨ cn);
ਖਾਲੀ merge32s(ਅਚਲ ਪੂਰਨ** src, ਪੂਰਨ* dst, ਪੂਰਨ len, ਪੂਰਨ cn);
ਖਾਲੀ merge64s(ਅਚਲ int64** src, int64* dst, ਪੂਰਨ len, ਪੂਰਨ cn);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
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
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VecT> ਸਥਿਰ ਖਾਲੀ
vecmerge_( ਅਚਲ T** src, T* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<VecT>::vlanes();
    ਪੂਰਨ i, i0 = 0;
    ਅਚਲ T* src0 = src[0];
    ਅਚਲ T* src1 = src[1];

    ਅਚਲ ਪੂਰਨ dst_Elem_ਮਾਪ = cn * ਆਕਾਰ(T);
    ਪੂਰਨ r = (ਪੂਰਨ)((size_t)(ਖਾਲੀ*)dst % (VECSZ*ਆਕਾਰ(T)));
    hal::Store_ਬਹੁਲਕ ਬਹੁਲਕ = hal::STORE_ALIGNED_NOCACHE;
    ਜੇ( r != 0 )
    {
        ਬਹੁਲਕ = hal::STORE_UNALIGNED;
        ਜੇ (r % dst_Elem_ਮਾਪ == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dst_Elem_ਮਾਪ);
    }

    ਜੇ( cn == 2 )
    {
        ਲਈ( i = 0; i < len; i += VECSZ )
        {
            ਜੇ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ਬਹੁਲਕ = hal::STORE_UNALIGNED;
            }
            VecT a = vx_ਲੱਦੋ(src0 + i), b = vx_ਲੱਦੋ(src1 + i);
            v_store_interleave(dst + i*cn, a, b, ਬਹੁਲਕ);
            ਜੇ( i < i0 )
            {
                i = i0 - VECSZ;
                ਬਹੁਲਕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( cn == 3 )
    {
        ਅਚਲ T* src2 = src[2];
        ਲਈ( i = 0; i < len; i += VECSZ )
        {
            ਜੇ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ਬਹੁਲਕ = hal::STORE_UNALIGNED;
            }
            VecT a = vx_ਲੱਦੋ(src0 + i), b = vx_ਲੱਦੋ(src1 + i), c = vx_ਲੱਦੋ(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, ਬਹੁਲਕ);
            ਜੇ( i < i0 )
            {
                i = i0 - VECSZ;
                ਬਹੁਲਕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( cn == 4 );
        ਅਚਲ T* src2 = src[2];
        ਅਚਲ T* src3 = src[3];
        ਲਈ( i = 0; i < len; i += VECSZ )
        {
            ਜੇ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ਬਹੁਲਕ = hal::STORE_UNALIGNED;
            }
            VecT a = vx_ਲੱਦੋ(src0 + i), b = vx_ਲੱਦੋ(src1 + i);
            VecT c = vx_ਲੱਦੋ(src2 + i), d = vx_ਲੱਦੋ(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, ਬਹੁਲਕ);
            ਜੇ( i < i0 )
            {
                i = i0 - VECSZ;
                ਬਹੁਲਕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ_2( ਅਚਲ T** src, T* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    ਪੂਰਨ k = cn % 4 ? cn % 4 : 4;
    ਪੂਰਨ i, j;
    ਜੇ( k == 1 )
    {
        ਅਚਲ T* src0 = src[0];
        ਲਈ( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == 2 )
    {
        ਅਚਲ T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        ਲਈ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == 3 )
    {
        ਅਚਲ T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        ਲਈ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        ਲਈ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    ਲਈ( ; k < cn; k += 4 )
    {
        ਅਚਲ T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        ਲਈ( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

ਖਾਲੀ merge8u(ਅਚਲ uchar** src, uchar* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਮਿਲਾਓ_2(src, dst, len, cn);
}

ਖਾਲੀ merge16u(ਅਚਲ ushort** src, ushort* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਮਿਲਾਓ_2(src, dst, len, cn);
}

ਖਾਲੀ merge32s(ਅਚਲ ਪੂਰਨ** src, ਪੂਰਨ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ਪੂਰਨ, v_int32>(src, dst, len, cn);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਮਿਲਾਓ_2(src, dst, len, cn);
}

ਖਾਲੀ merge64s(ਅਚਲ int64** src, int64* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਮਿਲਾਓ_2(src, dst, len, cn);
}

#ਜੇ_ਅੰਤ
CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
}} // namespace
