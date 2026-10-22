// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাৱ "precomp.hpp"

নামাকাশ cv { নামাকাশ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

শূন্য merge8u(ধ্রুৱ uchar** src, uchar* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn);
শূন্য merge16u(ধ্রুৱ ushort** src, ushort* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn);
শূন্য merge32s(ধ্রুৱ পূর্ণাঙ্ক** src, পূর্ণাঙ্ক* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn);
শূন্য merge64s(ধ্রুৱ int64** src, int64* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
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
template<typename T, typename VecT> স্থির শূন্য
vecmerge_( ধ্রুৱ T** src, T* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<VecT>::vlanes();
    পূর্ণাঙ্ক i, i0 = 0;
    ধ্রুৱ T* src0 = src[0];
    ধ্রুৱ T* src1 = src[1];

    ধ্রুৱ পূর্ণাঙ্ক dstElemSize = cn * পরিমাণ(T);
    পূর্ণাঙ্ক r = (পূর্ণাঙ্ক)((size_t)(শূন্য*)dst % (VECSZ*পরিমাণ(T)));
    hal::StoreMode mode = hal::STORE_ALIGNED_NOCACHE;
    যদি( r != 0 )
    {
        mode = hal::STORE_UNALIGNED;
        যদি (r % dstElemSize == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dstElemSize);
    }

    যদি( cn == 2 )
    {
        বাবে( i = 0; i < len; i += VECSZ )
        {
            যদি( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            v_store_interleave(dst + i*cn, a, b, mode);
            যদি( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    অন্যথা যদি( cn == 3 )
    {
        ধ্রুৱ T* src2 = src[2];
        বাবে( i = 0; i < len; i += VECSZ )
        {
            যদি( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i), c = vx_load(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, mode);
            যদি( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    অন্যথা
    {
        CV_Assert( cn == 4 );
        ধ্রুৱ T* src2 = src[2];
        ধ্রুৱ T* src3 = src[3];
        বাবে( i = 0; i < len; i += VECSZ )
        {
            যদি( i > len - VECSZ )
            {
                i = len - VECSZ;
                mode = hal::STORE_UNALIGNED;
            }
            VecT a = vx_load(src0 + i), b = vx_load(src1 + i);
            VecT c = vx_load(src2 + i), d = vx_load(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, mode);
            যদি( i < i0 )
            {
                i = i0 - VECSZ;
                mode = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#endif

template<typename T> স্থির শূন্য
merge_( ধ্রুৱ T** src, T* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    পূর্ণাঙ্ক k = cn % 4 ? cn % 4 : 4;
    পূর্ণাঙ্ক i, j;
    যদি( k == 1 )
    {
        ধ্রুৱ T* src0 = src[0];
        বাবে( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    অন্যথা যদি( k == 2 )
    {
        ধ্রুৱ T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        বাবে( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    অন্যথা যদি( k == 3 )
    {
        ধ্রুৱ T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        বাবে( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    অন্যথা
    {
        ধ্রুৱ T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        বাবে( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    বাবে( ; k < cn; k += 4 )
    {
        ধ্রুৱ T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        বাবে( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

শূন্য merge8u(ধ্রুৱ uchar** src, uchar* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    অন্যথা
#endif
        merge_(src, dst, len, cn);
}

শূন্য merge16u(ধ্রুৱ ushort** src, ushort* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    অন্যথা
#endif
        merge_(src, dst, len, cn);
}

শূন্য merge32s(ধ্রুৱ পূর্ণাঙ্ক** src, পূর্ণাঙ্ক* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<পূর্ণাঙ্ক, v_int32>(src, dst, len, cn);
    অন্যথা
#endif
        merge_(src, dst, len, cn);
}

শূন্য merge64s(ধ্রুৱ int64** src, int64* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    অন্যথা
#endif
        merge_(src, dst, len, cn);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace
