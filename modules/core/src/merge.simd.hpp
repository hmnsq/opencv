// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"

नामाकाश cv { नामाकाश hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

शून्य merge8u(ध्रुव uchar** src, uchar* dst, पूर्णाङ्क len, पूर्णाङ्क cn);
शून्य merge16u(ध्रुव ushort** src, ushort* dst, पूर्णाङ्क len, पूर्णाङ्क cn);
शून्य merge32s(ध्रुव पूर्णाङ्क** src, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn);
शून्य merge64s(ध्रुव int64** src, int64* dst, पूर्णाङ्क len, पूर्णाङ्क cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#सचे (CV_SIMD || CV_SIMD_SCALABLE)
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
template<typename T, typename VecT> स्थिर शून्य
vecmerge_( ध्रुव T** src, T* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    ध्रुव पूर्णाङ्क VECSZ = VTraits<VecT>::vlanes();
    पूर्णाङ्क i, i0 = 0;
    ध्रुव T* src0 = src[0];
    ध्रुव T* src1 = src[1];

    ध्रुव पूर्णाङ्क dstElemSize = cn * परिमाण(T);
    पूर्णाङ्क r = (पूर्णाङ्क)((size_t)(शून्य*)dst % (VECSZ*परिमाण(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    सचे( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        सचे (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    सचे( cn == 2 )
    {
        अत्थाय( i = 0; i < len; i += VECSZ )
        {
            सचे( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            सचे( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अञ्ञथा सचे( cn == 3 )
    {
        ध्रुव T* src2 = src[2];
        अत्थाय( i = 0; i < len; i += VECSZ )
        {
            सचे( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            सचे( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अञ्ञथा
    {
        CV_Assert( cn == 4 );
        ध्रुव T* src2 = src[2];
        ध्रुव T* src3 = src[3];
        अत्थाय( i = 0; i < len; i += VECSZ )
        {
            सचे( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            सचे( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> स्थिर शून्य
merge_( ध्रुव T** src, T* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    पूर्णाङ्क k = cn % 4 ? cn % 4 : 4;
    पूर्णाङ्क i, j;
    सचे( k == 1 )
    {
        ध्रुव T* src0 = src[0];
        अत्थाय( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    अञ्ञथा सचे( k == 2 )
    {
        ध्रुव T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        अत्थाय( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    अञ्ञथा सचे( k == 3 )
    {
        ध्रुव T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        अत्थाय( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    अञ्ञथा
    {
        ध्रुव T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        अत्थाय( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    अत्थाय( ; k < cn; k += 4 )
    {
        ध्रुव T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        अत्थाय( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

शून्य merge8u(ध्रुव uchar** src, uchar* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    सचे( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    अञ्ञथा
#endif
        merge_(src, dst, len, cn);
}

शून्य merge16u(ध्रुव ushort** src, ushort* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    सचे( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    अञ्ञथा
#endif
        merge_(src, dst, len, cn);
}

शून्य merge32s(ध्रुव पूर्णाङ्क** src, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    सचे( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<पूर्णाङ्क, v_int32>(src, dst, len, cn);
    अञ्ञथा
#endif
        merge_(src, dst, len, cn);
}

शून्य merge64s(ध्रुव int64** src, int64* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
    सचे( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    अञ्ञथा
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
