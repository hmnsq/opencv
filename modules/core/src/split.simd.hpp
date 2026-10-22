// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

void split8u(const uchar* src, uchar** dst, số_nguyên len, số_nguyên cn);
void split16u(const ushort* src, ushort** dst, số_nguyên len, số_nguyên cn);
void split32s(const số_nguyên* src, số_nguyên** dst, số_nguyên len, số_nguyên cn);
void split64s(const int64* src, int64** dst, số_nguyên len, số_nguyên cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#nếu (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> static void
vecsplit_( const T* src, T** dst, số_nguyên len, số_nguyên cn )
{
    const số_nguyên VECSZ = VTraits<VecT>::vlanes();
    số_nguyên i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    số_nguyên r0 = (số_nguyên)((size_t)(void*)dst0 % (VECSZ*sizeof(T)));
    số_nguyên r1 = (số_nguyên)((size_t)(void*)dst1 % (VECSZ*sizeof(T)));
    số_nguyên r2 = cn > 2 ? (số_nguyên)((size_t)(void*)dst[2] % (VECSZ*sizeof(T))) : r0;
    số_nguyên r3 = cn > 3 ? (số_nguyên)((size_t)(void*)dst[3] % (VECSZ*sizeof(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    nếu( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        nếu( r0 == r1 && r0 == r2 && r0 == r3 && r0 % sizeof(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / sizeof(T));
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
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            nếu( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    không_thì nếu( cn == 3 )
    {
        T* dst2 = dst[2];
        cho( i = 0; i < len; i += VECSZ )
        {
            nếu( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
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
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        cho( i = 0; i < len; i += VECSZ )
        {
            nếu( i > len - VECSZ )
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
split_( const T* src, T** dst, số_nguyên len, số_nguyên cn )
{
    số_nguyên k = cn % 4 ? cn % 4 : 4;
    số_nguyên i, j;
    nếu( k == 1 )
    {
        T* dst0 = dst[0];

        nếu(cn == 1)
        {
            memcpy(dst0, src, len * sizeof(T));
        }
        không_thì
        {
            cho( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    không_thì nếu( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        cho( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    không_thì nếu( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        cho( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    không_thì
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        cho( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    cho( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        cho( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

void split8u(const uchar* src, uchar** dst, số_nguyên len, số_nguyên cn )
{
    CV_INSTRUMENT_REGION();
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    nếu( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    không_thì
#endif
        split_(src, dst, len, cn);
}

void split16u(const ushort* src, ushort** dst, số_nguyên len, số_nguyên cn )
{
    CV_INSTRUMENT_REGION();
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    nếu( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    không_thì
#endif
        split_(src, dst, len, cn);
}

void split32s(const số_nguyên* src, số_nguyên** dst, số_nguyên len, số_nguyên cn )
{
    CV_INSTRUMENT_REGION();
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    nếu( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<số_nguyên, v_int32>(src, dst, len, cn);
    không_thì
#endif
        split_(src, dst, len, cn);
}

void split64s(const int64* src, int64** dst, số_nguyên len, số_nguyên cn )
{
    CV_INSTRUMENT_REGION();
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
    nếu( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    không_thì
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
