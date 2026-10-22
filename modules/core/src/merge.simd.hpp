// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"

నామాకాశ cv { నామాకాశ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

శూన్య merge8u(స్థిరరాశి uchar** src, uchar* dst, పూర్ణాంకం len, పూర్ణాంకం cn);
శూన్య merge16u(స్థిరరాశి ushort** src, ushort* dst, పూర్ణాంకం len, పూర్ణాంకం cn);
శూన్య merge32s(స్థిరరాశి పూర్ణాంకం** src, పూర్ణాంకం* dst, పూర్ణాంకం len, పూర్ణాంకం cn);
శూన్య merge64s(స్థిరరాశి int64** src, int64* dst, పూర్ణాంకం len, పూర్ణాంకం cn);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
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
మూస<రకం_పేరు T, రకం_పేరు VecT> స్థిర శూన్య
vecmerge_( స్థిరరాశి T** src, T* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<VecT>::vlanes();
    పూర్ణాంకం i, i0 = 0;
    స్థిరరాశి T* src0 = src[0];
    స్థిరరాశి T* src1 = src[1];

    స్థిరరాశి పూర్ణాంకం dst_Elem_కొలత = cn * పరిమాణం(T);
    పూర్ణాంకం r = (పూర్ణాంకం)((size_t)(శూన్య*)dst % (VECSZ*పరిమాణం(T)));
    hal::Store_బహుళకం బహుళకం = hal::STORE_ALIGNED_NOCACHE;
    ఒకవేళ( r != 0 )
    {
        బహుళకం = hal::STORE_UNALIGNED;
        ఒకవేళ (r % dst_Elem_కొలత == 0 && len > VECSZ*2)
            i0 = VECSZ - (r / dst_Elem_కొలత);
    }

    ఒకవేళ( cn == 2 )
    {
        కొరకు( i = 0; i < len; i += VECSZ )
        {
            ఒకవేళ( i > len - VECSZ )
            {
                i = len - VECSZ;
                బహుళకం = hal::STORE_UNALIGNED;
            }
            VecT a = vx_ఎక్కించు(src0 + i), b = vx_ఎక్కించు(src1 + i);
            v_store_interleave(dst + i*cn, a, b, బహుళకం);
            ఒకవేళ( i < i0 )
            {
                i = i0 - VECSZ;
                బహుళకం = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    లేకపోతే ఒకవేళ( cn == 3 )
    {
        స్థిరరాశి T* src2 = src[2];
        కొరకు( i = 0; i < len; i += VECSZ )
        {
            ఒకవేళ( i > len - VECSZ )
            {
                i = len - VECSZ;
                బహుళకం = hal::STORE_UNALIGNED;
            }
            VecT a = vx_ఎక్కించు(src0 + i), b = vx_ఎక్కించు(src1 + i), c = vx_ఎక్కించు(src2 + i);
            v_store_interleave(dst + i*cn, a, b, c, బహుళకం);
            ఒకవేళ( i < i0 )
            {
                i = i0 - VECSZ;
                బహుళకం = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    లేకపోతే
    {
        CV_Assert( cn == 4 );
        స్థిరరాశి T* src2 = src[2];
        స్థిరరాశి T* src3 = src[3];
        కొరకు( i = 0; i < len; i += VECSZ )
        {
            ఒకవేళ( i > len - VECSZ )
            {
                i = len - VECSZ;
                బహుళకం = hal::STORE_UNALIGNED;
            }
            VecT a = vx_ఎక్కించు(src0 + i), b = vx_ఎక్కించు(src1 + i);
            VecT c = vx_ఎక్కించు(src2 + i), d = vx_ఎక్కించు(src3 + i);
            v_store_interleave(dst + i*cn, a, b, c, d, బహుళకం);
            ఒకవేళ( i < i0 )
            {
                i = i0 - VECSZ;
                బహుళకం = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    vx_cleanup();
}
#అయితే_ముగింపు

మూస<రకం_పేరు T> స్థిర శూన్య
విలీనం_2( స్థిరరాశి T** src, T* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    పూర్ణాంకం k = cn % 4 ? cn % 4 : 4;
    పూర్ణాంకం i, j;
    ఒకవేళ( k == 1 )
    {
        స్థిరరాశి T* src0 = src[0];
        కొరకు( i = j = 0; i < len; i++, j += cn )
            dst[j] = src0[i];
    }
    లేకపోతే ఒకవేళ( k == 2 )
    {
        స్థిరరాశి T *src0 = src[0], *src1 = src[1];
        i = j = 0;
        కొరకు( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
        }
    }
    లేకపోతే ఒకవేళ( k == 3 )
    {
        స్థిరరాశి T *src0 = src[0], *src1 = src[1], *src2 = src[2];
        i = j = 0;
        కొరకు( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i];
            dst[j+1] = src1[i];
            dst[j+2] = src2[i];
        }
    }
    లేకపోతే
    {
        స్థిరరాశి T *src0 = src[0], *src1 = src[1], *src2 = src[2], *src3 = src[3];
        i = j = 0;
        కొరకు( ; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }

    కొరకు( ; k < cn; k += 4 )
    {
        స్థిరరాశి T *src0 = src[k], *src1 = src[k+1], *src2 = src[k+2], *src3 = src[k+3];
        కొరకు( i = 0, j = k; i < len; i++, j += cn )
        {
            dst[j] = src0[i]; dst[j+1] = src1[i];
            dst[j+2] = src2[i]; dst[j+3] = src3[i];
        }
    }
}

శూన్య merge8u(స్థిరరాశి uchar** src, uchar* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<uchar, v_uint8>(src, dst, len, cn);
    లేకపోతే
#అయితే_ముగింపు
        విలీనం_2(src, dst, len, cn);
}

శూన్య merge16u(స్థిరరాశి ushort** src, ushort* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<ushort, v_uint16>(src, dst, len, cn);
    లేకపోతే
#అయితే_ముగింపు
        విలీనం_2(src, dst, len, cn);
}

శూన్య merge32s(స్థిరరాశి పూర్ణాంకం** src, పూర్ణాంకం* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( len >= VTraits<v_int32>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<పూర్ణాంకం, v_int32>(src, dst, len, cn);
    లేకపోతే
#అయితే_ముగింపు
        విలీనం_2(src, dst, len, cn);
}

శూన్య merge64s(స్థిరరాశి int64** src, int64* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecmerge_<int64, v_int64>(src, dst, len, cn);
    లేకపోతే
#అయితే_ముగింపు
        విలీనం_2(src, dst, len, cn);
}

#అయితే_ముగింపు
CV_CPU_OPTIMIZATION_NAMESPACE_చివర
}} // namespace
