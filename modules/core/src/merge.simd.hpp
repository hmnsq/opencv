// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"

நாமாகாஶ cv { நாமாகாஶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

வெறுமை merge8u(மாறிலி uchar** src, uchar* dst, முழு len, முழு cn);
வெறுமை merge16u(மாறிலி ushort** src, ushort* dst, முழு len, முழு cn);
வெறுமை merge32s(மாறிலி முழு** src, முழு* dst, முழு len, முழு cn);
வெறுமை merge64s(மாறிலி int64** src, int64* dst, முழு len, முழு cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
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
template<typename T, typename VecT> நிலையான வெறுமை
vecmerge_( மாறிலி T** src, T* dst, முழு len, முழு cn )
{
    மாறிலி முழு VECSZ = VTraits<VecT>::vlanes();
    முழு i, i0 = 0;
    மாறிலி T* src0 = src[0];
    மாறிலி T* src1 = src[1];

    மாறிலி முழு dstElemSize = cn * அளவு(T);
    முழு r = (முழு)((size_t)(வெறுமை*)dst % (VECSZ*அளவு(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    எனில்( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        எனில் (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    எனில்( cn == 2 )
    {
        க்கு( i = 0; i < len; i += VECSZ )
        {
            எனில்( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            எனில்( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    இல்லையெனில் எனில்( cn == 3 )
    {
        மாறிலி T* src2 = src[2];
        க்கு( i = 0; i < len; i += VECSZ )
        {
            எனில்( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            எனில்( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    இல்லையெனில்
    {
        CV_Assert( cn == 4 );
        மாறிலி T* src2 = src[2];
        மாறிலி T* src3 = src[3];
        க்கு( i = 0; i < len; i += VECSZ )
        {
            எனில்( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            எனில்( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> நிலையான வெறுமை
merge_( மாறிலி T** src, T* dst, முழு len, முழு cn )
{
    முழு k = cn % 4 ? cn % 4 : 4;
    முழு i, j;
    எனில்( k == 1 )
    {
        மாறிலி T* src0 = src[0];
        க்கு( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    இல்லையெனில் எனில்( k == 2 )
    {
        மாறிலி T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        க்கு( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    இல்லையெனில் எனில்( k == 3 )
    {
        மாறிலி T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        க்கு( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    இல்லையெனில்
    {
        மாறிலி T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        க்கு( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    க்கு( ; k < cn; k += 4 )
    {
        மாறிலி T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        க்கு( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

வெறுமை merge8u(மாறிலி uchar** src, uchar* dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில்( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    இல்லையெனில்
#endif
        merge_(src, dst, len, cn);
}

வெறுமை merge16u(மாறிலி ushort** src, ushort* dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில்( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    இல்லையெனில்
#endif
        merge_(src, dst, len, cn);
}

வெறுமை merge32s(மாறிலி முழு** src, முழு* dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில்( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<முழு, v_int32>(src, dst, len, cn);
    இல்லையெனில்
#endif
        merge_(src, dst, len, cn);
}

வெறுமை merge64s(மாறிலி int64** src, int64* dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில்( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    இல்லையெனில்
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
