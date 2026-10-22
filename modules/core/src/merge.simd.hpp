// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"

namespace cv { namespace hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

व्योम merge8u(स्थिर uchar** src, uchar* dst, पूर्णांक len, पूर्णांक cn);
व्योम merge16u(स्थिर ushort** src, ushort* dst, पूर्णांक len, पूर्णांक cn);
व्योम merge32s(स्थिर पूर्णांक** src, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn);
व्योम merge64s(स्थिर int64** src, int64* dst, पूर्णांक len, पूर्णांक cn);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
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
ढाँचा<typename T, typename VecT> अटल व्योम
vecmerge_( स्थिर T** src, T* dst, पूर्णांक len, पूर्णांक cn )
{
    स्थिर पूर्णांक VECSZ = VTraits<VecT>::vlanes();
    पूर्णांक i, i0 = 0;
    स्थिर T* src0 = src[0];
    स्थिर T* src1 = src[1];

    स्थिर पूर्णांक dst_Elem_आकार = cn * माप(T);
    पूर्णांक r = (पूर्णांक)((माप_प्रकार)(व्योम*)dst % (VECSZ*माप(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    अगर( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        अगर (r % dst_Elem_आकार == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dst_Elem_आकार);
    }

    अगर( cn == 2 )
    {
        क्रम( i = 0; i < len; i += VECSZ )
        {
            अगर( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            अगर( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अन्यथा अगर( cn == 3 )
    {
        स्थिर T* src2 = src[2];
        क्रम( i = 0; i < len; i += VECSZ )
        {
            अगर( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            अगर( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    अन्यथा
    {
        CV_Assert( cn == 4 );
        स्थिर T* src2 = src[2];
        स्थिर T* src3 = src[3];
        क्रम( i = 0; i < len; i += VECSZ )
        {
            अगर( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            अगर( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#पूर्ण_अगर

ढाँचा<typename T> अटल व्योम
merge_( स्थिर T** src, T* dst, पूर्णांक len, पूर्णांक cn )
{
    पूर्णांक k = cn % 4 ? cn % 4 : 4;
    पूर्णांक i, j;
    अगर( k == 1 )
    {
        स्थिर T* src0 = src[0];
        क्रम( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    अन्यथा अगर( k == 2 )
    {
        स्थिर T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        क्रम( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    अन्यथा अगर( k == 3 )
    {
        स्थिर T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        क्रम( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    अन्यथा
    {
        स्थिर T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        क्रम( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    क्रम( ; k < cn; k += 4 )
    {
        स्थिर T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        क्रम( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

व्योम merge8u(स्थिर uchar** src, uchar* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    अन्यथा
#पूर्ण_अगर
        merge_(src, dst, len, cn);
}

व्योम merge16u(स्थिर ushort** src, ushort* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    अन्यथा
#पूर्ण_अगर
        merge_(src, dst, len, cn);
}

व्योम merge32s(स्थिर पूर्णांक** src, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<पूर्णांक, v_int32>(src, dst, len, cn);
    अन्यथा
#पूर्ण_अगर
        merge_(src, dst, len, cn);
}

व्योम merge64s(स्थिर int64** src, int64* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    अन्यथा
#पूर्ण_अगर
        merge_(src, dst, len, cn);
}

#पूर्ण_अगर
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
