// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಶೂನ್ಯ merge8u(ಧ್ರುವ uchar** src, uchar* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);
ಶೂನ್ಯ merge16u(ಧ್ರುವ ushort** src, ushort* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);
ಶೂನ್ಯ merge32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ** src, ಪೂರ್ಣಾಙ್ಕ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);
ಶೂನ್ಯ merge64s(ಧ್ರುವ int64** src, int64* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
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
template<typename T, typename VecT> ಸ್ಥಿರ ಶೂನ್ಯ
vecmerge_( ಧ್ರುವ T** src, T* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<VecT>::vlanes();
    ಪೂರ್ಣಾಙ್ಕ i, i0 = 0;
    ಧ್ರುವ T* src0 = src[0];
    ಧ್ರುವ T* src1 = src[1];

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dstElemSize = cn * ಪರಿಮಾಣ(T);
    ಪೂರ್ಣಾಙ್ಕ r = (ಪೂರ್ಣಾಙ್ಕ)((size_t)(ಶೂನ್ಯ*)dst % (VECSZ*ಪರಿಮಾಣ(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    ಯದಿ( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        ಯದಿ (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    ಯದಿ( cn == 2 )
    {
        ಪ್ರತಿ( i = 0; i < len; i += VECSZ )
        {
            ಯದಿ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            ಯದಿ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( cn == 3 )
    {
        ಧ್ರುವ T* src2 = src[2];
        ಪ್ರತಿ( i = 0; i < len; i += VECSZ )
        {
            ಯದಿ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            ಯದಿ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ಅನ್ಯಥಾ
    {
        CV_Assert( cn == 4 );
        ಧ್ರುವ T* src2 = src[2];
        ಧ್ರುವ T* src3 = src[3];
        ಪ್ರತಿ( i = 0; i < len; i += VECSZ )
        {
            ಯದಿ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            ಯದಿ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ
merge_( ಧ್ರುವ T** src, T* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    ಪೂರ್ಣಾಙ್ಕ k = cn % 4 ? cn % 4 : 4;
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಯದಿ( k == 1 )
    {
        ಧ್ರುವ T* src0 = src[0];
        ಪ್ರತಿ( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    ಅನ್ಯಥಾ ಯದಿ( k == 2 )
    {
        ಧ್ರುವ T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        ಪ್ರತಿ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( k == 3 )
    {
        ಧ್ರುವ T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        ಪ್ರತಿ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        ಪ್ರತಿ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    ಪ್ರತಿ( ; k < cn; k += 4 )
    {
        ಧ್ರುವ T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        ಪ್ರತಿ( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

ಶೂನ್ಯ merge8u(ಧ್ರುವ uchar** src, uchar* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    ಅನ್ಯಥಾ
#endif
        merge_(src, dst, len, cn);
}

ಶೂನ್ಯ merge16u(ಧ್ರುವ ushort** src, ushort* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    ಅನ್ಯಥಾ
#endif
        merge_(src, dst, len, cn);
}

ಶೂನ್ಯ merge32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ** src, ಪೂರ್ಣಾಙ್ಕ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ಪೂರ್ಣಾಙ್ಕ, v_int32>(src, dst, len, cn);
    ಅನ್ಯಥಾ
#endif
        merge_(src, dst, len, cn);
}

ಶೂನ್ಯ merge64s(ಧ್ರುವ int64** src, int64* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    ಅನ್ಯಥಾ
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
