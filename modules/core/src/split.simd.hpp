// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

void split8u(const uchar* src, uchar** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn);
void split16u(const ushort* src, ushort** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn);
void split32s(const පූර්ණ_සංඛ්‍යා* src, පූර්ණ_සංඛ්‍යා** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn);
void split64s(const int64* src, int64** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#නම් (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> static void
vecsplit_( const T* src, T** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    const පූර්ණ_සංඛ්‍යා VECSZ = VTraits<VecT>::vlanes();
    පූර්ණ_සංඛ්‍යා i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    පූර්ණ_සංඛ්‍යා r0 = (පූර්ණ_සංඛ්‍යා)((size_t)(void*)dst0 % (VECSZ*parimaa_na(T)));
    පූර්ණ_සංඛ්‍යා r1 = (පූර්ණ_සංඛ්‍යා)((size_t)(void*)dst1 % (VECSZ*parimaa_na(T)));
    පූර්ණ_සංඛ්‍යා r2 = cn > 2 ? (පූර්ණ_සංඛ්‍යා)((size_t)(void*)dst[2] % (VECSZ*parimaa_na(T))) : r0;
    පූර්ණ_සංඛ්‍යා r3 = cn > 3 ? (පූර්ණ_සංඛ්‍යා)((size_t)(void*)dst[3] % (VECSZ*parimaa_na(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    නම්( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        නම්( r0 == r1 && r0 == r2 && r0 == r3 && r0 % parimaa_na(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / parimaa_na(T));
    }

    නම්( cn == 2 )
    {
        සඳහා( i = 0; i < len; i += VECSZ )
        {
            නම්( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            නම්( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    නැතහොත් නම්( cn == 3 )
    {
        T* dst2 = dst[2];
        සඳහා( i = 0; i < len; i += VECSZ )
        {
            නම්( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
            නම්( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    නැතහොත්
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        සඳහා( i = 0; i < len; i += VECSZ )
        {
            නම්( i > len - VECSZ )
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
            නම්( i < i0 )
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
split_( const T* src, T** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    පූර්ණ_සංඛ්‍යා k = cn % 4 ? cn % 4 : 4;
    පූර්ණ_සංඛ්‍යා i, j;
    නම්( k == 1 )
    {
        T* dst0 = dst[0];

        නම්(cn == 1)
        {
            memcpy(dst0, src, len * parimaa_na(T));
        }
        නැතහොත්
        {
            සඳහා( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    නැතහොත් නම්( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        සඳහා( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    නැතහොත් නම්( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        සඳහා( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    නැතහොත්
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        සඳහා( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    සඳහා( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        සඳහා( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

void split8u(const uchar* src, uchar** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    CV_INSTRUMENT_REGION();
#නම් (CV_SIMD || CV_SIMD_SCALABLE)
    නම්( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    නැතහොත්
#endif
        split_(src, dst, len, cn);
}

void split16u(const ushort* src, ushort** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    CV_INSTRUMENT_REGION();
#නම් (CV_SIMD || CV_SIMD_SCALABLE)
    නම්( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    නැතහොත්
#endif
        split_(src, dst, len, cn);
}

void split32s(const පූර්ණ_සංඛ්‍යා* src, පූර්ණ_සංඛ්‍යා** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    CV_INSTRUMENT_REGION();
#නම් (CV_SIMD || CV_SIMD_SCALABLE)
    නම්( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<පූර්ණ_සංඛ්‍යා, v_int32>(src, dst, len, cn);
    නැතහොත්
#endif
        split_(src, dst, len, cn);
}

void split64s(const int64* src, int64** dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    CV_INSTRUMENT_REGION();
#නම් (CV_SIMD || CV_SIMD_SCALABLE)
    නම්( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    නැතහොත්
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
