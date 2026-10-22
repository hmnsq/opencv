// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಶೂನ್ಯ split8u(ಸ್ಥಿರಾಂಕ uchar* src, uchar** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);
ಶೂನ್ಯ split16u(ಸ್ಥಿರಾಂಕ ushort* src, ushort** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);
ಶೂನ್ಯ split32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಪೂರ್ಣಾಂಕ** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);
ಶೂನ್ಯ split64s(ಸ್ಥಿರಾಂಕ int64* src, int64** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು VecT> ಸ್ಥಿರ ಶೂನ್ಯ
vecsplit_( ಸ್ಥಿರಾಂಕ T* src, T** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<VecT>::vlanes();
    ಪೂರ್ಣಾಂಕ i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    ಪೂರ್ಣಾಂಕ r0 = (ಪೂರ್ಣಾಂಕ)((size_t)(ಶೂನ್ಯ*)dst0 % (VECSZ*ಗಾತ್ರ(T)));
    ಪೂರ್ಣಾಂಕ r1 = (ಪೂರ್ಣಾಂಕ)((size_t)(ಶೂನ್ಯ*)dst1 % (VECSZ*ಗಾತ್ರ(T)));
    ಪೂರ್ಣಾಂಕ r2 = cn > 2 ? (ಪೂರ್ಣಾಂಕ)((size_t)(ಶೂನ್ಯ*)dst[2] % (VECSZ*ಗಾತ್ರ(T))) : r0;
    ಪೂರ್ಣಾಂಕ r3 = cn > 3 ? (ಪೂರ್ಣಾಂಕ)((size_t)(ಶೂನ್ಯ*)dst[3] % (VECSZ*ಗಾತ್ರ(T))) : r0;

    hal::Store_ಬಹುಲಕ ಬಹುಲಕ = hal::STORE_ALIGNED_NOCACHE;
    ಆದರೆ( (r0|r1|r2|r3) != 0 )
    {
        ಬಹುಲಕ = hal::STORE_UNALIGNED;
        ಆದರೆ( r0 == r1 && r0 == r2 && r0 == r3 && r0 % ಗಾತ್ರ(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / ಗಾತ್ರ(T));
    }

    ಆದರೆ( cn == 2 )
    {
        ಗಾಗಿ( i = 0; i < len; i += VECSZ )
        {
            ಆದರೆ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ಬಹುಲಕ = hal::STORE_UNALIGNED;
            }
            VecT a, b;
            v_ತುಂಬು_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, ಬಹುಲಕ);
            v_store(dst1 + i, b, ಬಹುಲಕ);
            ಆದರೆ( i < i0 )
            {
                i = i0 - VECSZ;
                ಬಹುಲಕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 3 )
    {
        T* dst2 = dst[2];
        ಗಾಗಿ( i = 0; i < len; i += VECSZ )
        {
            ಆದರೆ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ಬಹುಲಕ = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_ತುಂಬು_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, ಬಹುಲಕ);
            v_store(dst1 + i, b, ಬಹುಲಕ);
            v_store(dst2 + i, c, ಬಹುಲಕ);
            ಆದರೆ( i < i0 )
            {
                i = i0 - VECSZ;
                ಬಹುಲಕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( cn == 4 );
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        ಗಾಗಿ( i = 0; i < len; i += VECSZ )
        {
            ಆದರೆ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ಬಹುಲಕ = hal::STORE_UNALIGNED;
            }
            VecT a, b, c, d;
            v_ತುಂಬು_deinterleave(src + i*cn, a, b, c, d);
            v_store(dst0 + i, a, ಬಹುಲಕ);
            v_store(dst1 + i, b, ಬಹುಲಕ);
            v_store(dst2 + i, c, ಬಹುಲಕ);
            v_store(dst3 + i, d, ಬಹುಲಕ);
            ಆದರೆ( i < i0 )
            {
                i = i0 - VECSZ;
                ಬಹುಲಕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಭಜಿಸು_2( ಸ್ಥಿರಾಂಕ T* src, T** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    ಪೂರ್ಣಾಂಕ k = cn % 4 ? cn % 4 : 4;
    ಪೂರ್ಣಾಂಕ i, j;
    ಆದರೆ( k == 1 )
    {
        T* dst0 = dst[0];

        ಆದರೆ(cn == 1)
        {
            ಸ್ಮರಣೆ_ನಕಲು(dst0, src, len * ಗಾತ್ರ(T));
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        ಗಾಗಿ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        ಗಾಗಿ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        ಗಾಗಿ( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    ಗಾಗಿ( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        ಗಾಗಿ( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

ಶೂನ್ಯ split8u(ಸ್ಥಿರಾಂಕ uchar* src, uchar** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ವಿಭಜಿಸು_2(src, dst, len, cn);
}

ಶೂನ್ಯ split16u(ಸ್ಥಿರಾಂಕ ushort* src, ushort** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ವಿಭಜಿಸು_2(src, dst, len, cn);
}

ಶೂನ್ಯ split32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಪೂರ್ಣಾಂಕ** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ಪೂರ್ಣಾಂಕ, v_int32>(src, dst, len, cn);
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ವಿಭಜಿಸು_2(src, dst, len, cn);
}

ಶೂನ್ಯ split64s(ಸ್ಥಿರಾಂಕ int64* src, int64** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ವಿಭಜಿಸು_2(src, dst, len, cn);
}

#ಆದರೆ_ಅಂತ್ಯ
CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
}} // namespace
