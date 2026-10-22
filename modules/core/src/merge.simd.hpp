// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

void merge8u(const uchar** src, uchar* dst, số_nguyên len, số_nguyên cn);
void merge16u(const ushort** src, ushort* dst, số_nguyên len, số_nguyên cn);
void merge32s(const số_nguyên** src, số_nguyên* dst, số_nguyên len, số_nguyên cn);
void merge64s(const int64** src, int64* dst, số_nguyên len, số_nguyên cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#nếu (CV_SIMD || CV_SIMD_SCALABLE)
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
template<typename T, typename VecT> static void
vecmerge_( const T** src, T* dst, số_nguyên len, số_nguyên cn )
{
    const số_nguyên VECSZ = VTraits<VecT>::vlanes();
    số_nguyên i, i0 = 0;
    const T* src0 = src[0];
    const T* src1 = src[1];

    const số_nguyên dstElemSize = cn * sizeof(T);
    số_nguyên r = (số_nguyên)((size_t)(void*)dst % (VECSZ*sizeof(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    nếu( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        nếu (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    nếu( cn == 2 )
    {
        cho( i = 0; i < len; i += VECSZ )
        {
            nếu( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            nếu( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    không_thì nếu( cn == 3 )
    {
        const T* src2 = src[2];
        cho( i = 0; i < len; i += VECSZ )
        {
            nếu( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            nếu( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    không_thì
    {
        CV_Assert( cn == 4 );
        const T* src2 = src[2];
        const T* src3 = src[3];
        cho( i = 0; i < len; i += VECSZ )
        {
            nếu( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            nếu( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> static void
merge_( const T** src, T* dst, số_nguyên len, số_nguyên cn )
{
    số_nguyên k = cn % 4 ? cn % 4 : 4;
    số_nguyên i, j;
    nếu( k == 1 )
    {
        const T* src0 = src[0];
        cho( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    không_thì nếu( k == 2 )
    {
        const T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        cho( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    không_thì nếu( k == 3 )
    {
        const T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        cho( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    không_thì
    {
        const T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        cho( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    cho( ; k < cn; k += 4 )
    {
        const T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        cho( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

void merge8u(const uchar** src, uchar* dst, số_nguyên len, số_nguyên cn )
{
    CV_INSTRUMENT_REGION();
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    nếu( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    không_thì
#endif
        merge_(src, dst, len, cn);
}

void merge16u(const ushort** src, ushort* dst, số_nguyên len, số_nguyên cn )
{
    CV_INSTRUMENT_REGION();
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    nếu( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    không_thì
#endif
        merge_(src, dst, len, cn);
}

void merge32s(const số_nguyên** src, số_nguyên* dst, số_nguyên len, số_nguyên cn )
{
    CV_INSTRUMENT_REGION();
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    nếu( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<số_nguyên, v_int32>(src, dst, len, cn);
    không_thì
#endif
        merge_(src, dst, len, cn);
}

void merge64s(const int64** src, int64* dst, số_nguyên len, số_nguyên cn )
{
    CV_INSTRUMENT_REGION();
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    nếu( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    không_thì
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
