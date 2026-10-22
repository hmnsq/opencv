// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

void split8u(const uchar* src, uchar** dst, ціле len, ціле cn);
void split16u(const ushort* src, ushort** dst, ціле len, ціле cn);
void split32s(const ціле* src, ціле** dst, ціле len, ціле cn);
void split64s(const int64* src, int64** dst, ціле len, ціле cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#якщо (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> static void
vecsplit_( const T* src, T** dst, ціле len, ціле cn )
{
    const ціле VECSZ = VTraits<VecT>::vlanes();
    ціле i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    ціле r0 = (ціле)((size_t)(void*)dst0 % (VECSZ*sizeof(T)));
    ціле r1 = (ціле)((size_t)(void*)dst1 % (VECSZ*sizeof(T)));
    ціле r2 = cn > 2 ? (ціле)((size_t)(void*)dst[2] % (VECSZ*sizeof(T))) : r0;
    ціле r3 = cn > 3 ? (ціле)((size_t)(void*)dst[3] % (VECSZ*sizeof(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    якщо( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        якщо( r0 == r1 && r0 == r2 && r0 == r3 && r0 % sizeof(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / sizeof(T));
    }

    якщо( cn == 2 )
    {
        для( i = 0; i < len; i += VECSZ )
        {
            якщо( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            якщо( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    інакше якщо( cn == 3 )
    {
        T* dst2 = dst[2];
        для( i = 0; i < len; i += VECSZ )
        {
            якщо( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            якщо( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    інакше
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        для( i = 0; i < len; i += VECSZ )
        {
            якщо( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c, d;
            v_load_deinterleave(src + i*cn, a, b, c, d);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            v_store(dst3 + i, d, mode);
            якщо( i < i0 )
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
split_( const T* src, T** dst, ціле len, ціле cn )
{
    ціле k = cn % 4 ? cn % 4 : 4;
    ціле i, j;
    якщо( k == 1 )
    {
        T* dst0 = dst[0];

        якщо(cn == 1)
        {
            memcpy(dst0, src, len * sizeof(T));
        }
        інакше
        {
            для( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    інакше якщо( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        для( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    інакше якщо( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        для( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    інакше
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        для( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    для( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        для( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

void split8u(const uchar* src, uchar** dst, ціле len, ціле cn )
{
    CV_INSTRUMENT_REGION();
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    якщо( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    інакше
#endif
        split_(src, dst, len, cn);
}

void split16u(const ushort* src, ushort** dst, ціле len, ціле cn )
{
    CV_INSTRUMENT_REGION();
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    якщо( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    інакше
#endif
        split_(src, dst, len, cn);
}

void split32s(const ціле* src, ціле** dst, ціле len, ціле cn )
{
    CV_INSTRUMENT_REGION();
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    якщо( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ціле, v_int32>(src, dst, len, cn);
    інакше
#endif
        split_(src, dst, len, cn);
}

void split64s(const int64* src, int64** dst, ціле len, ціле cn )
{
    CV_INSTRUMENT_REGION();
#якщо (CV_SIMD || CV_SIMD_SCALABLE)
    якщо( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    інакше
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
