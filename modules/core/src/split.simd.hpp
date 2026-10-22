// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"

நாமாகாஶ cv { நாமாகாஶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

வெறுமை split8u(மாறிலி uchar* src, uchar** dst, முழு len, முழு cn);
வெறுமை split16u(மாறிலி ushort* src, ushort** dst, முழு len, முழு cn);
வெறுமை split32s(மாறிலி முழு* src, முழு** dst, முழு len, முழு cn);
வெறுமை split64s(மாறிலி int64* src, int64** dst, முழு len, முழு cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> நிலையான வெறுமை
vecsplit_( மாறிலி T* src, T** dst, முழு len, முழு cn )
{
    மாறிலி முழு VECSZ = VTraits<VecT>::vlanes();
    முழு i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    முழு r0 = (முழு)((size_t)(வெறுமை*)dst0 % (VECSZ*அளவு(T)));
    முழு r1 = (முழு)((size_t)(வெறுமை*)dst1 % (VECSZ*அளவு(T)));
    முழு r2 = cn > 2 ? (முழு)((size_t)(வெறுமை*)dst[2] % (VECSZ*அளவு(T))) : r0;
    முழு r3 = cn > 3 ? (முழு)((size_t)(வெறுமை*)dst[3] % (VECSZ*அளவு(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    எனில்( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        எனில்( r0 == r1 && r0 == r2 && r0 == r3 && r0 % அளவு(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / அளவு(T));
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
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            எனில்( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    இல்லையெனில் எனில்( cn == 3 )
    {
        T* dst2 = dst[2];
        க்கு( i = 0; i < len; i += VECSZ )
        {
            எனில்( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
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
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        க்கு( i = 0; i < len; i += VECSZ )
        {
            எனில்( i > len - VECSZ )
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
split_( மாறிலி T* src, T** dst, முழு len, முழு cn )
{
    முழு k = cn % 4 ? cn % 4 : 4;
    முழு i, j;
    எனில்( k == 1 )
    {
        T* dst0 = dst[0];

        எனில்(cn == 1)
        {
            memcpy(dst0, src, len * அளவு(T));
        }
        இல்லையெனில்
        {
            க்கு( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    இல்லையெனில் எனில்( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        க்கு( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    இல்லையெனில் எனில்( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        க்கு( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    இல்லையெனில்
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        க்கு( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    க்கு( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        க்கு( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

வெறுமை split8u(மாறிலி uchar* src, uchar** dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில்( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    இல்லையெனில்
#endif
        split_(src, dst, len, cn);
}

வெறுமை split16u(மாறிலி ushort* src, ushort** dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில்( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    இல்லையெனில்
#endif
        split_(src, dst, len, cn);
}

வெறுமை split32s(மாறிலி முழு* src, முழு** dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில்( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<முழு, v_int32>(src, dst, len, cn);
    இல்லையெனில்
#endif
        split_(src, dst, len, cn);
}

வெறுமை split64s(மாறிலி int64* src, int64** dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில்( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    இல்லையெனில்
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
