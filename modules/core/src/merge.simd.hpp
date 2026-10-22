// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"

नामाकाश cv { नामाकाश hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

शून्य merge8u(स्थिरांक uchar** src, uchar* dst, पूर्णांक len, पूर्णांक cn);
शून्य merge16u(स्थिरांक ushort** src, ushort* dst, पूर्णांक len, पूर्णांक cn);
शून्य merge32s(स्थिरांक पूर्णांक** src, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn);
शून्य merge64s(स्थिरांक int64** src, int64* dst, पूर्णांक len, पूर्णांक cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#जर (CV_SIMD || CV_SIMD_SCALABLE)
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
ढाँचा<typename T, typename VecT> स्थिर शून्य
vecmerge_( स्थिरांक T** src, T* dst, पूर्णांक len, पूर्णांक cn )
{
    स्थिरांक पूर्णांक VECSZ = VTraits<VecT>::vlanes();
    पूर्णांक i, i0 = 0;
    स्थिरांक T* src0 = src[0];
    स्थिरांक T* src1 = src[1];

    स्थिरांक पूर्णांक dstElemSize = cn * आकार(T);
    पूर्णांक r = (पूर्णांक)((माप_प्रकार)(शून्य*)dst % (VECSZ*आकार(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    जर( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        जर (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    जर( cn == 2 )
    {
        साठी( i = 0; i < len; i += VECSZ )
        {
            जर( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            जर( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अन्यथा जर( cn == 3 )
    {
        स्थिरांक T* src2 = src[2];
        साठी( i = 0; i < len; i += VECSZ )
        {
            जर( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            जर( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अन्यथा
    {
        CV_Assert( cn == 4 );
        स्थिरांक T* src2 = src[2];
        स्थिरांक T* src3 = src[3];
        साठी( i = 0; i < len; i += VECSZ )
        {
            जर( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            जर( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

ढाँचा<typename T> स्थिर शून्य
merge_( स्थिरांक T** src, T* dst, पूर्णांक len, पूर्णांक cn )
{
    पूर्णांक k = cn % 4 ? cn % 4 : 4;
    पूर्णांक i, j;
    जर( k == 1 )
    {
        स्थिरांक T* src0 = src[0];
        साठी( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    अन्यथा जर( k == 2 )
    {
        स्थिरांक T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        साठी( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    अन्यथा जर( k == 3 )
    {
        स्थिरांक T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        साठी( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    अन्यथा
    {
        स्थिरांक T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        साठी( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    साठी( ; k < cn; k += 4 )
    {
        स्थिरांक T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        साठी( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

शून्य merge8u(स्थिरांक uchar** src, uchar* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    अन्यथा
#endif
        merge_(src, dst, len, cn);
}

शून्य merge16u(स्थिरांक ushort** src, ushort* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    अन्यथा
#endif
        merge_(src, dst, len, cn);
}

शून्य merge32s(स्थिरांक पूर्णांक** src, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<पूर्णांक, v_int32>(src, dst, len, cn);
    अन्यथा
#endif
        merge_(src, dst, len, cn);
}

शून्य merge64s(स्थिरांक int64** src, int64* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    अन्यथा
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
