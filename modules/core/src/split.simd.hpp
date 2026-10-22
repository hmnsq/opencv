// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"

నామాకాశ cv { నామాకాశ hal {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

శూన్య split8u(స్థిరరాశి uchar* src, uchar** dst, పూర్ణాంకం len, పూర్ణాంకం cn);
శూన్య split16u(స్థిరరాశి ushort* src, ushort** dst, పూర్ణాంకం len, పూర్ణాంకం cn);
శూన్య split32s(స్థిరరాశి పూర్ణాంకం* src, పూర్ణాంకం** dst, పూర్ణాంకం len, పూర్ణాంకం cn);
శూన్య split64s(స్థిరరాశి int64* src, int64** dst, పూర్ణాంకం len, పూర్ణాంకం cn);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
// see the comments for vecmerge_ in merge.cpp
మూస<రకం_పేరు T, రకం_పేరు VecT> స్థిర శూన్య
vecsplit_( స్థిరరాశి T* src, T** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<VecT>::vlanes();
    పూర్ణాంకం i, i0 = 0;
    T* dst0 = dst[0];
    T* dst1 = dst[1];

    పూర్ణాంకం r0 = (పూర్ణాంకం)((size_t)(శూన్య*)dst0 % (VECSZ*పరిమాణం(T)));
    పూర్ణాంకం r1 = (పూర్ణాంకం)((size_t)(శూన్య*)dst1 % (VECSZ*పరిమాణం(T)));
    పూర్ణాంకం r2 = cn > 2 ? (పూర్ణాంకం)((size_t)(శూన్య*)dst[2] % (VECSZ*పరిమాణం(T))) : r0;
    పూర్ణాంకం r3 = cn > 3 ? (పూర్ణాంకం)((size_t)(శూన్య*)dst[3] % (VECSZ*పరిమాణం(T))) : r0;

    hal::Store_బహుళకం బహుళకం = hal::STORE_ALIGNED_NOCACHE;
    ఒకవేళ( (r0|r1|r2|r3) != 0 )
    {
        బహుళకం = hal::STORE_UNALIGNED;
        ఒకవేళ( r0 == r1 && r0 == r2 && r0 == r3 && r0 % పరిమాణం(T) == 0 && len > VECSZ*2 )
            i0 = VECSZ - (r0 / పరిమాణం(T));
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
            VecT a, b;
            v_ఎక్కించు_deinterleave(src + i*cn, a, b);
            v_store(dst0 + i, a, బహుళకం);
            v_store(dst1 + i, b, బహుళకం);
            ఒకవేళ( i < i0 )
            {
                i = i0 - VECSZ;
                బహుళకం = hal::STORE_ALIGNED_NOCACHE;
            }
        }
    }
    లేకపోతే ఒకవేళ( cn == 3 )
    {
        T* dst2 = dst[2];
        కొరకు( i = 0; i < len; i += VECSZ )
        {
            ఒకవేళ( i > len - VECSZ )
            {
                i = len - VECSZ;
                బహుళకం = hal::STORE_UNALIGNED;
            }
            VecT a, b, c;
            v_ఎక్కించు_deinterleave(src + i*cn, a, b, c);
            v_store(dst0 + i, a, బహుళకం);
            v_store(dst1 + i, b, బహుళకం);
            v_store(dst2 + i, c, బహుళకం);
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
        T* dst2 = dst[2];
        T* dst3 = dst[3];
        కొరకు( i = 0; i < len; i += VECSZ )
        {
            ఒకవేళ( i > len - VECSZ )
            {
                i = len - VECSZ;
                బహుళకం = hal::STORE_UNALIGNED;
            }
            VecT a, b, c, d;
            v_ఎక్కించు_deinterleave(src + i*cn, a, b, c, d);
            v_store(dst0 + i, a, బహుళకం);
            v_store(dst1 + i, b, బహుళకం);
            v_store(dst2 + i, c, బహుళకం);
            v_store(dst3 + i, d, బహుళకం);
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
విభజించు_2( స్థిరరాశి T* src, T** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    పూర్ణాంకం k = cn % 4 ? cn % 4 : 4;
    పూర్ణాంకం i, j;
    ఒకవేళ( k == 1 )
    {
        T* dst0 = dst[0];

        ఒకవేళ(cn == 1)
        {
            జ్ఞాపకం_నకలు(dst0, src, len * పరిమాణం(T));
        }
        లేకపోతే
        {
            కొరకు( i = 0, j = 0 ; i < len; i++, j += cn )
                dst0[i] = src[j];
        }
    }
    లేకపోతే ఒకవేళ( k == 2 )
    {
        T *dst0 = dst[0], *dst1 = dst[1];
        i = j = 0;

        కొరకు( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
        }
    }
    లేకపోతే ఒకవేళ( k == 3 )
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2];
        i = j = 0;

        కొరకు( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j];
            dst1[i] = src[j+1];
            dst2[i] = src[j+2];
        }
    }
    లేకపోతే
    {
        T *dst0 = dst[0], *dst1 = dst[1], *dst2 = dst[2], *dst3 = dst[3];
        i = j = 0;

        కొరకు( ; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }

    కొరకు( ; k < cn; k += 4 )
    {
        T *dst0 = dst[k], *dst1 = dst[k+1], *dst2 = dst[k+2], *dst3 = dst[k+3];
        కొరకు( i = 0, j = k; i < len; i++, j += cn )
        {
            dst0[i] = src[j]; dst1[i] = src[j+1];
            dst2[i] = src[j+2]; dst3[i] = src[j+3];
        }
    }
}

శూన్య split8u(స్థిరరాశి uchar* src, uchar** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( len >= VTraits<v_uint8>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<uchar, v_uint8>(src, dst, len, cn);
    లేకపోతే
#అయితే_ముగింపు
        విభజించు_2(src, dst, len, cn);
}

శూన్య split16u(స్థిరరాశి ushort* src, ushort** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( len >= VTraits<v_uint16>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<ushort, v_uint16>(src, dst, len, cn);
    లేకపోతే
#అయితే_ముగింపు
        విభజించు_2(src, dst, len, cn);
}

శూన్య split32s(స్థిరరాశి పూర్ణాంకం* src, పూర్ణాంకం** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( len >= VTraits<v_uint32>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<పూర్ణాంకం, v_int32>(src, dst, len, cn);
    లేకపోతే
#అయితే_ముగింపు
        విభజించు_2(src, dst, len, cn);
}

శూన్య split64s(స్థిరరాశి int64* src, int64** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( len >= VTraits<v_int64>::vlanes() && 2 <= cn && cn <= 4 )
        vecsplit_<int64, v_int64>(src, dst, len, cn);
    లేకపోతే
#అయితే_ముగింపు
        విభజించు_2(src, dst, len, cn);
}

#అయితే_ముగింపు
CV_CPU_OPTIMIZATION_NAMESPACE_చివర
}} // namespace
