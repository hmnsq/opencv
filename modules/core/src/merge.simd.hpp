// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಶೂನ್ಯ merge8u(ಸ್ಥಿರಾಂಕ uchar** src, uchar* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);
ಶೂನ್ಯ merge16u(ಸ್ಥಿರಾಂಕ ushort** src, ushort* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);
ಶೂನ್ಯ merge32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ** src, ಪೂರ್ಣಾಂಕ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);
ಶೂನ್ಯ merge64s(ಸ್ಥಿರಾಂಕ int64** src, int64* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
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
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು VecT> ಸ್ಥಿರ ಶೂನ್ಯ
vecmerge_( ಸ್ಥಿರಾಂಕ T** src, T* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<VecT>::vlanes();
    ಪೂರ್ಣಾಂಕ i, i0 = 0;
    ಸ್ಥಿರಾಂಕ T* src0 = src[0];
    ಸ್ಥಿರಾಂಕ T* src1 = src[1];

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dst_Elem_ಗಾತ್ರ = cn * ಗಾತ್ರ(T);
    ಪೂರ್ಣಾಂಕ r = (ಪೂರ್ಣಾಂಕ)((size_t)(ಶೂನ್ಯ*)dst % (VECSZ*ಗಾತ್ರ(T)));
    hal::Store_ಬಹುಲಕ ಬಹುಲಕ = hal::STORE_ALIGNED_NOCACHE;
    ಆದರೆ( r != 0 )
    {
        ಬಹುಲಕ = hal::STORE_UNALIGNED;
        ಆದರೆ (r % dst_Elem_ಗಾತ್ರ == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dst_Elem_ಗಾತ್ರ);
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
            VecT a = vx_ತುಂಬು(src0 + i), b = vx_ತುಂಬು(src1 + i);
            v_store_interleave(dst + i*cn, a, b, ಬಹುಲಕ);
            ಆದರೆ( i < i0 )
            {
                i = i0 - VECSZ;
                ಬಹುಲಕ = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 3 )
    {
        ಸ್ಥಿರಾಂಕ T* src2 = src[2];
        ಗಾಗಿ( i = 0; i < len; i += VECSZ )
        {
            ಆದರೆ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ಬಹುಲಕ = hal::STORE_UNALIGNED;
            }
            VecT a = vx_ತುಂಬು(src0 + i), b = vx_ತುಂಬು(src1 + i), c = vx_ತುಂಬು(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, ಬಹುಲಕ);
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
        ಸ್ಥಿರಾಂಕ T* src2 = src[2];
        ಸ್ಥಿರಾಂಕ T* src3 = src[3];
        ಗಾಗಿ( i = 0; i < len; i += VECSZ )
        {
            ಆದರೆ( i > len - VECSZ )
            {
                i = len - VECSZ;
                ಬಹುಲಕ = hal::STORE_UNALIGNED;
            }
            VecT a = vx_ತುಂಬು(src0 + i), b = vx_ತುಂಬು(src1 + i);
            VecT c = vx_ತುಂಬು(src2 + i), d = vx_ತುಂಬು(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, ಬಹುಲಕ);
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
ವಿಲೀನ_2( ಸ್ಥಿರಾಂಕ T** src, T* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    ಪೂರ್ಣಾಂಕ k = cn % 4 ? cn % 4 : 4;
    ಪೂರ್ಣಾಂಕ i, j;
    ಆದರೆ( k == 1 )
    {
        ಸ್ಥಿರಾಂಕ T* src0 = src[0];
        ಗಾಗಿ( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 2 )
    {
        ಸ್ಥಿರಾಂಕ T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        ಗಾಗಿ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 3 )
    {
        ಸ್ಥಿರಾಂಕ T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        ಗಾಗಿ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        ಗಾಗಿ( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    ಗಾಗಿ( ; k < cn; k += 4 )
    {
        ಸ್ಥಿರಾಂಕ T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        ಗಾಗಿ( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

ಶೂನ್ಯ merge8u(ಸ್ಥಿರಾಂಕ uchar** src, uchar* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ವಿಲೀನ_2(src, dst, len, cn);
}

ಶೂನ್ಯ merge16u(ಸ್ಥಿರಾಂಕ ushort** src, ushort* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ವಿಲೀನ_2(src, dst, len, cn);
}

ಶೂನ್ಯ merge32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ** src, ಪೂರ್ಣಾಂಕ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ಪೂರ್ಣಾಂಕ, v_int32>(src, dst, len, cn);
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ವಿಲೀನ_2(src, dst, len, cn);
}

ಶೂನ್ಯ merge64s(ಸ್ಥಿರಾಂಕ int64** src, int64* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ವಿಲೀನ_2(src, dst, len, cn);
}

#ಆದರೆ_ಅಂತ್ಯ
CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
}} // namespace
