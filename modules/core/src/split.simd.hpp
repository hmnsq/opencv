// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಶೂನ್ಯ split8u(ಧ್ರುವ uchar* src, uchar** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);
ಶೂನ್ಯ split16u(ಧ್ರುವ ushort* src, ushort** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);
ಶೂನ್ಯ split32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಪೂರ್ಣಾಙ್ಕ** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);
ಶೂನ್ಯ split64s(ಧ್ರುವ int64* src, int64** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
template<typename T, typename VecT> ಸ್ಥಿರ ಶೂನ್ಯ
vecsplit_( ಧ್ರುವ T* src, T** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<VecT>::vlanes();
    ಪೂರ್ಣಾಙ್ಕ i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    ಪೂರ್ಣಾಙ್ಕ r0 = (ಪೂರ್ಣಾಙ್ಕ)((size_t)(ಶೂನ್ಯ*)dst0 % (VECSZ*ಪರಿಮಾಣ(T)));
    ಪೂರ್ಣಾಙ್ಕ r1 = (ಪೂರ್ಣಾಙ್ಕ)((size_t)(ಶೂನ್ಯ*)dst1 % (VECSZ*ಪರಿಮಾಣ(T)));
    ಪೂರ್ಣಾಙ್ಕ r2 = cn > 2 ? (ಪೂರ್ಣಾಙ್ಕ)((size_t)(ಶೂನ್ಯ*)dst[2] % (VECSZ*ಪರಿಮಾಣ(T))) : r0;
    ಪೂರ್ಣಾಙ್ಕ r3 = cn > 3 ? (ಪೂರ್ಣಾಙ್ಕ)((size_t)(ಶೂನ್ಯ*)dst[3] % (VECSZ*ಪರಿಮಾಣ(T))) : r0;

    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    ಯದಿ( (r0|r1|r2|r3) != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        ಯದಿ( r0 == r1 && r0 == r2 && r0 == r3 && r0 % ಪರಿಮಾಣ(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / ಪರಿಮಾಣ(T));
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
            VecT a, b;
            v_load_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            ಯದಿ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( cn == 3 )
    {
        T* dst2 = dst[2];
        ಪ್ರತಿ( i = 0; i < len; i += VECSZ )
        {
            ಯದಿ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_load_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, mode);
            v_store(dst1 + i, b, mode);
            v_store(dst2 + i, c, mode);
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
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        ಪ್ರತಿ( i = 0; i < len; i += VECSZ )
        {
            ಯದಿ( i > len - VECSZ )
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
split_( ಧ್ರುವ T* src, T** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    ಪೂರ್ಣಾಙ್ಕ k = cn % 4 ? cn % 4 : 4;
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಯದಿ( k == 1 )
    {
        T* dst0 = dst[0];

        ಯದಿ(cn == 1)
        {
            memcpy(dst0, src, len * ಪರಿಮಾಣ(T));
        }
        ಅನ್ಯಥಾ
        {
            ಪ್ರತಿ( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        ಪ್ರತಿ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        ಪ್ರತಿ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    ಅನ್ಯಥಾ
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        ಪ್ರತಿ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    ಪ್ರತಿ( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        ಪ್ರತಿ( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

ಶೂನ್ಯ split8u(ಧ್ರುವ uchar* src, uchar** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    ಅನ್ಯಥಾ
#endif
        split_(src, dst, len, cn);
}

ಶೂನ್ಯ split16u(ಧ್ರುವ ushort* src, ushort** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    ಅನ್ಯಥಾ
#endif
        split_(src, dst, len, cn);
}

ಶೂನ್ಯ split32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಪೂರ್ಣಾಙ್ಕ** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ಪೂರ್ಣಾಙ್ಕ, v_int32>(src, dst, len, cn);
    ಅನ್ಯಥಾ
#endif
        split_(src, dst, len, cn);
}

ಶೂನ್ಯ split64s(ಧ್ರುವ int64* src, int64** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    ಅನ್ಯಥಾ
#endif
        split_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
