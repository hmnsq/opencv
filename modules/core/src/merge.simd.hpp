// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv { નામાકાશ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

શૂન્ય merge8u(અચલ uchar** src, uchar* dst, પૂર્ણાંક len, પૂર્ણાંક cn);
શૂન્ય merge16u(અચલ ushort** src, ushort* dst, પૂર્ણાંક len, પૂર્ણાંક cn);
શૂન્ય merge32s(અચલ પૂર્ણાંક** src, પૂર્ણાંક* dst, પૂર્ણાંક len, પૂર્ણાંક cn);
શૂન્ય merge64s(અચલ int64** src, int64* dst, પૂર્ણાંક len, પૂર્ણાંક cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#જો (CV_SIMD || CV_SIMD_SCALABLE)
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
template<typename T, typename VecT> સ્થિર શૂન્ય
vecmerge_( અચલ T** src, T* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    અચલ પૂર્ણાંક VECSZ = VTraits<VecT>::vlanes();
    પૂર્ણાંક i, i0 = 0;
    અચલ T* src0 = src[0];
    અચલ T* src1 = src[1];

    અચલ પૂર્ણાંક dstElemSize = cn * કદ(T);
    પૂર્ણાંક r = (પૂર્ણાંક)((size_t)(શૂન્ય*)dst % (VECSZ*કદ(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    જો( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        જો (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    જો( cn == 2 )
    {
        માટે( i = 0; i < len; i += VECSZ )
        {
            જો( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            જો( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    નહીંતર જો( cn == 3 )
    {
        અચલ T* src2 = src[2];
        માટે( i = 0; i < len; i += VECSZ )
        {
            જો( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            જો( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    નહીંતર
    {
        CV_Assert( cn == 4 );
        અચલ T* src2 = src[2];
        અચલ T* src3 = src[3];
        માટે( i = 0; i < len; i += VECSZ )
        {
            જો( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            જો( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> સ્થિર શૂન્ય
merge_( અચલ T** src, T* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    પૂર્ણાંક k = cn % 4 ? cn % 4 : 4;
    પૂર્ણાંક i, j;
    જો( k == 1 )
    {
        અચલ T* src0 = src[0];
        માટે( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    નહીંતર જો( k == 2 )
    {
        અચલ T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        માટે( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    નહીંતર જો( k == 3 )
    {
        અચલ T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        માટે( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    નહીંતર
    {
        અચલ T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        માટે( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    માટે( ; k < cn; k += 4 )
    {
        અચલ T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        માટે( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

શૂન્ય merge8u(અચલ uchar** src, uchar* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    નહીંતર
#endif
        merge_(src, dst, len, cn);
}

શૂન્ય merge16u(અચલ ushort** src, ushort* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    નહીંતર
#endif
        merge_(src, dst, len, cn);
}

શૂન્ય merge32s(અચલ પૂર્ણાંક** src, પૂર્ણાંક* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<પૂર્ણાંક, v_int32>(src, dst, len, cn);
    નહીંતર
#endif
        merge_(src, dst, len, cn);
}

શૂન્ય merge64s(અચલ int64** src, int64* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    નહીંતર
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
