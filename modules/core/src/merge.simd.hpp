// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"

നാമാകാശ cv { നാമാകാശ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ശൂന്യം merge8u(സ്ഥിരം uchar** src, uchar* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);
ശൂന്യം merge16u(സ്ഥിരം ushort** src, ushort* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);
ശൂന്യം merge32s(സ്ഥിരം പൂർണ്ണസംഖ്യ** src, പൂർണ്ണസംഖ്യ* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);
ശൂന്യം merge64s(സ്ഥിരം int64** src, int64* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
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
template<typename T, typename VecT> സ്ഥിതം ശൂന്യം
vecmerge_( സ്ഥിരം T** src, T* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<VecT>::vlanes();
    പൂർണ്ണസംഖ്യ i, i0 = 0;
    സ്ഥിരം T* src0 = src[0];
    സ്ഥിരം T* src1 = src[1];

    സ്ഥിരം പൂർണ്ണസംഖ്യ dstElemSize = cn * വലുപ്പം(T);
    പൂർണ്ണസംഖ്യ r = (പൂർണ്ണസംഖ്യ)((size_t)(ശൂന്യം*)dst % (VECSZ*വലുപ്പം(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    എങ്കിൽ( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        എങ്കിൽ (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    എങ്കിൽ( cn == 2 )
    {
        വേണ്ടി( i = 0; i < len; i += VECSZ )
        {
            എങ്കിൽ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            എങ്കിൽ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( cn == 3 )
    {
        സ്ഥിരം T* src2 = src[2];
        വേണ്ടി( i = 0; i < len; i += VECSZ )
        {
            എങ്കിൽ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            എങ്കിൽ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        CV_Assert( cn == 4 );
        സ്ഥിരം T* src2 = src[2];
        സ്ഥിരം T* src3 = src[3];
        വേണ്ടി( i = 0; i < len; i += VECSZ )
        {
            എങ്കിൽ( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            എങ്കിൽ( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> സ്ഥിതം ശൂന്യം
merge_( സ്ഥിരം T** src, T* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    പൂർണ്ണസംഖ്യ k = cn % 4 ? cn % 4 : 4;
    പൂർണ്ണസംഖ്യ i, j;
    എങ്കിൽ( k == 1 )
    {
        സ്ഥിരം T* src0 = src[0];
        വേണ്ടി( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == 2 )
    {
        സ്ഥിരം T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        വേണ്ടി( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( k == 3 )
    {
        സ്ഥിരം T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        വേണ്ടി( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        വേണ്ടി( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    വേണ്ടി( ; k < cn; k += 4 )
    {
        സ്ഥിരം T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        വേണ്ടി( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

ശൂന്യം merge8u(സ്ഥിരം uchar** src, uchar* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    അല്ലെങ്കിൽ
#endif
        merge_(src, dst, len, cn);
}

ശൂന്യം merge16u(സ്ഥിരം ushort** src, ushort* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    അല്ലെങ്കിൽ
#endif
        merge_(src, dst, len, cn);
}

ശൂന്യം merge32s(സ്ഥിരം പൂർണ്ണസംഖ്യ** src, പൂർണ്ണസംഖ്യ* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<പൂർണ്ണസംഖ്യ, v_int32>(src, dst, len, cn);
    അല്ലെങ്കിൽ
#endif
        merge_(src, dst, len, cn);
}

ശൂന്യം merge64s(സ്ഥിരം int64** src, int64* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    അല്ലെങ്കിൽ
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
