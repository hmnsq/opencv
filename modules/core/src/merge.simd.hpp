// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

void merge8u(const uchar** src, uchar* dst, цео len, цео cn);
void merge16u(const ushort** src, ushort* dst, цео len, цео cn);
void merge32s(const цео** src, цео* dst, цео len, цео cn);
void merge64s(const int64** src, int64* dst, цео len, цео cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ако (CV_SIMD || CV_SIMD_SCALABLE)
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
vecmerge_( const T** src, T* dst, цео len, цео cn )
{
    const цео VECSZ = VTraits<VecT>::vlanes();
    цео i, i0 = 0;
    const T* src0 = src[0];
    const T* src1 = src[1];

    const цео dstElemSize = cn * sizeof(T);
    цео r = (цео)((size_t)(void*)dst % (VECSZ*sizeof(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    ако( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        ако (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    ако( cn == 2 )
    {
        за( i = 0; i < len; i += VECSZ )
        {
            ако( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            ако( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    инако ако( cn == 3 )
    {
        const T* src2 = src[2];
        за( i = 0; i < len; i += VECSZ )
        {
            ако( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            ако( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    инако
    {
        CV_Assert( cn == 4 );
        const T* src2 = src[2];
        const T* src3 = src[3];
        за( i = 0; i < len; i += VECSZ )
        {
            ако( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            ако( i < i0 )
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
merge_( const T** src, T* dst, цео len, цео cn )
{
    цео k = cn % 4 ? cn % 4 : 4;
    цео i, j;
    ако( k == 1 )
    {
        const T* src0 = src[0];
        за( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    инако ако( k == 2 )
    {
        const T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        за( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    инако ако( k == 3 )
    {
        const T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        за( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    инако
    {
        const T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        за( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    за( ; k < cn; k += 4 )
    {
        const T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        за( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

void merge8u(const uchar** src, uchar* dst, цео len, цео cn )
{
    CV_INSTRUMENT_REGION();
#ако (CV_SIMD || CV_SIMD_SCALABLE)
    ако( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    инако
#endif
        merge_(src, dst, len, cn);
}

void merge16u(const ushort** src, ushort* dst, цео len, цео cn )
{
    CV_INSTRUMENT_REGION();
#ако (CV_SIMD || CV_SIMD_SCALABLE)
    ако( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    инако
#endif
        merge_(src, dst, len, cn);
}

void merge32s(const цео** src, цео* dst, цео len, цео cn )
{
    CV_INSTRUMENT_REGION();
#ако (CV_SIMD || CV_SIMD_SCALABLE)
    ако( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<цео, v_int32>(src, dst, len, cn);
    инако
#endif
        merge_(src, dst, len, cn);
}

void merge64s(const int64** src, int64* dst, цео len, цео cn )
{
    CV_INSTRUMENT_REGION();
#ако (CV_SIMD || CV_SIMD_SCALABLE)
    ако( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    инако
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
