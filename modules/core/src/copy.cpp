/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Copyright (C) 2014, Itseez Inc., all rights reserved.
// Copyright (C) 2025, SpaceMIT Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

నామాకాశ cv
{

మూస <రకం_పేరు T> స్థిర అంతఃవరుస
శూన్య scalar_గమ్యం_Raw_దత్తాంశం_2(స్థిరరాశి Scalar& s, T * స్థిరరాశి buf, స్థిరరాశి పూర్ణాంకం cn, స్థిరరాశి పూర్ణాంకం unroll_గమ్యం)
{
    పూర్ణాంకం i = 0;
    కొరకు(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    కొరకు(; i < unroll_గమ్యం; i++)
        buf[i] = buf[i-cn];
}

శూన్య scalar_గమ్యం_Raw_దత్తాంశం(స్థిరరాశి Scalar& s, శూన్య* _buf, పూర్ణాంకం type, పూర్ణాంకం unroll_గమ్యం)
{
    CV_INSTRUMENT_REGION();

    స్థిరరాశి పూర్ణాంకం లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    ఎంపిక(లోతు)
    {
    స్థితి CV_8U:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (uchar*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_8S:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (schar*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_Bool:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (తార్కిక*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_16U:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (ushort*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_16S:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (పొట్టి*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_16F:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (hfloat*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_16BF:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (bfloat*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_8F_E4M3FN:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (fp8_t*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_8F_E4M3FNUZ:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (fp8a_t*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_32U:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (గుర్తులేని*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_32S:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (పూర్ణాంకం*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_32F:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (దశాంశం*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_64U:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (uint64_t*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_64S:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (int64_t*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    స్థితి CV_64F:
        scalar_గమ్యం_Raw_దత్తాంశం_2(s, (ద్విగుణ*)_buf, cn, unroll_గమ్యం);
        ఆపు;
    అప్రమేయ:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

శూన్య convertAndUnrollScalar( స్థిరరాశి Mat& sc, పూర్ణాంకం buftype, uchar* scbuf, size_t blocksize )
{
    పూర్ణాంకం scn = (పూర్ణాంకం)sc.మొత్తం(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_కొలత(buftype);
    ద్విమాన_Func cvtFn = getConvertFunc(sc.లోతు(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, కొలత(std::min(cn, scn), 1), 0);
    // unroll the scalar
    ఒకవేళ( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        కొరకు( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    కొరకు( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

మూస<రకం_పేరు T> స్థిర శూన్య
copy_ముసుగు(స్థిరరాశి uchar* _src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, uchar* _dst, size_t dstep, కొలత size)
{
    కొరకు( ; size.ఎత్తు--; ముసుగు += mstep, _src += sstep, _dst += dstep )
    {
        స్థిరరాశి T* src = (స్థిరరాశి T*)_src;
        T* dst = (T*)_dst;
        పూర్ణాంకం x = 0;
         #ఒకవేళ CV_ENABLE_UNROLLED
        కొరకు( ; x <= size.వెడల్పు - 4; x += 4 )
        {
            ఒకవేళ( ముసుగు[x] )
                dst[x] = src[x];
            ఒకవేళ( ముసుగు[x+1] )
                dst[x+1] = src[x+1];
            ఒకవేళ( ముసుగు[x+2] )
                dst[x+2] = src[x+2];
            ఒకవేళ( ముసుగు[x+3] )
                dst[x+3] = src[x+3];
        }
        #అయితే_ముగింపు
        కొరకు( ; x < size.వెడల్పు; x++ )
            ఒకవేళ( ముసుగు[x] )
                dst[x] = src[x];
    }
}

మూస<> శూన్య
copy_ముసుగు<uchar>(స్థిరరాశి uchar* _src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, uchar* _dst, size_t dstep, కొలత size)
{
    CV_IPP_నిరంతరం_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (పూర్ణాంకం)sstep, _dst, (పూర్ణాంకం)dstep, ippi_కొలత(size), ముసుగు, (పూర్ణాంకం)mstep) >= 0)

    కొరకు( ; size.ఎత్తు--; ముసుగు += mstep, _src += sstep, _dst += dstep )
    {
        స్థిరరాశి uchar* src = (స్థిరరాశి uchar*)_src;
        uchar* dst = (uchar*)_dst;
        పూర్ణాంకం x = 0;
        #ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            కొరకు( ; x <= size.వెడల్పు - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_ఎక్కించు(src  + x),
                        v_dst   = vx_ఎక్కించు(dst  + x),
                        v_nmask = v_eq(vx_ఎక్కించు(ముసుగు + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #అయితే_ముగింపు
        కొరకు( ; x < size.వెడల్పు; x++ )
            ఒకవేళ( ముసుగు[x] )
                dst[x] = src[x];
    }
}

మూస<> శూన్య
copy_ముసుగు<Vec3b>(స్థిరరాశి uchar* _src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, uchar* _dst, size_t dstep, కొలత size)
{
    కొరకు( ; size.ఎత్తు--; ముసుగు += mstep, _src += sstep, _dst += dstep )
    {
        స్థిరరాశి uchar* src = (స్థిరరాశి uchar*)_src;
        uchar* dst = (uchar*)_dst;
        పూర్ణాంకం x = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        కొరకు( ; x <= size.వెడల్పు - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_ఎక్కించు(ముసుగు + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_ఎక్కించు_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_ఎక్కించు_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#అయితే_ముగింపు
        కొరకు( ; x < size.వెడల్పు; x++ )
            ఒకవేళ( ముసుగు[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

మూస<> శూన్య
copy_ముసుగు<ushort>(స్థిరరాశి uchar* _src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, uchar* _dst, size_t dstep, కొలత size)
{
    CV_IPP_నిరంతరం_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (స్థిరరాశి Ipp16u *)_src, (పూర్ణాంకం)sstep, (Ipp16u *)_dst, (పూర్ణాంకం)dstep, ippi_కొలత(size), ముసుగు, (పూర్ణాంకం)mstep) >= 0)

    కొరకు( ; size.ఎత్తు--; ముసుగు += mstep, _src += sstep, _dst += dstep )
    {
        స్థిరరాశి ushort* src = (స్థిరరాశి ushort*)_src;
        ushort* dst = (ushort*)_dst;
        పూర్ణాంకం x = 0;
        #ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            కొరకు( ; x <= size.వెడల్పు - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_ఎక్కించు(src + x), v_src2 = vx_ఎక్కించు(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_ఎక్కించు(dst + x), v_dst2 = vx_ఎక్కించు(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_ఎక్కించు(ముసుగు + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #అయితే_ముగింపు
        కొరకు( ; x < size.వెడల్పు; x++ )
            ఒకవేళ( ముసుగు[x] )
                dst[x] = src[x];
    }
}

మూస<> శూన్య
copy_ముసుగు<Vec3s>(స్థిరరాశి uchar* _src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, uchar* _dst, size_t dstep, కొలత size)
{
    కొరకు( ; size.ఎత్తు--; ముసుగు += mstep, _src += sstep, _dst += dstep )
    {
        స్థిరరాశి ushort* src = (స్థిరరాశి ushort*)_src;
        ushort* dst = (ushort*)_dst;
        పూర్ణాంకం x = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        కొరకు( ; x <= size.వెడల్పు - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_ఎక్కించు(ముసుగు + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_ఎక్కించు_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_ఎక్కించు_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_ఎక్కించు_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_ఎక్కించు_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

            v_dst0 = v_select(v_reinterpret_as_u16(v_nmask0), v_dst0, v_src0);
            v_dst1 = v_select(v_reinterpret_as_u16(v_nmask0), v_dst1, v_src1);
            v_dst2 = v_select(v_reinterpret_as_u16(v_nmask0), v_dst2, v_src2);
            v_dst3 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst3, v_src3);
            v_dst4 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst4, v_src4);
            v_dst5 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst5, v_src5);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_store_interleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);
        }
        vx_cleanup();
#అయితే_ముగింపు
        కొరకు( ; x < size.వెడల్పు; x++ )
            ఒకవేళ( ముసుగు[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

మూస<> శూన్య
copy_ముసుగు<పూర్ణాంకం>(స్థిరరాశి uchar* _src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, uchar* _dst, size_t dstep, కొలత size)
{
    కొరకు( ; size.ఎత్తు--; ముసుగు += mstep, _src += sstep, _dst += dstep )
    {
        స్థిరరాశి పూర్ణాంకం* src = (స్థిరరాశి పూర్ణాంకం*)_src;
        పూర్ణాంకం* dst = (పూర్ణాంకం*)_dst;
        పూర్ణాంకం x = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        కొరకు (; x <= size.వెడల్పు - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_ఎక్కించు(src + x), v_dst0 = vx_ఎక్కించు(dst + x);
            v_int32 v_src1 = vx_ఎక్కించు(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_ఎక్కించు(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_ఎక్కించు(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_ఎక్కించు(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_ఎక్కించు(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_ఎక్కించు(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_ఎక్కించు(ముసుగు + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);
            v_uint8 v_nmask00, v_nmask01, v_nmask10, v_nmask11;
            v_zip(v_nmask0, v_nmask0, v_nmask00, v_nmask01);
            v_zip(v_nmask1, v_nmask1, v_nmask10, v_nmask11);

            v_dst0 = v_select(v_reinterpret_as_s32(v_nmask00), v_dst0, v_src0);
            v_dst1 = v_select(v_reinterpret_as_s32(v_nmask01), v_dst1, v_src1);
            v_dst2 = v_select(v_reinterpret_as_s32(v_nmask10), v_dst2, v_src2);
            v_dst3 = v_select(v_reinterpret_as_s32(v_nmask11), v_dst3, v_src3);

            vx_store(dst + x, v_dst0);
            vx_store(dst + x +     VTraits<v_int32>::vlanes(), v_dst1);
            vx_store(dst + x + 2 * VTraits<v_int32>::vlanes(), v_dst2);
            vx_store(dst + x + 3 * VTraits<v_int32>::vlanes(), v_dst3);
        }
        vx_cleanup();
#అయితే_ముగింపు
        కొరకు (; x < size.వెడల్పు; x++)
            ఒకవేళ ( ముసుగు[x] )
                dst[x] = src[x];
    }
}

స్థిర శూన్య
copy_ముసుగు_Generic(స్థిరరాశి uchar* _src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, uchar* _dst, size_t dstep, కొలత size, శూన్య* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    కొరకు( ; size.ఎత్తు--; ముసుగు += mstep, _src += sstep, _dst += dstep )
    {
        స్థిరరాశి uchar* src = _src;
        uchar* dst = _dst;
        పూర్ణాంకం x = 0;
        కొరకు( ; x < size.వెడల్పు; x++, src += esz, dst += esz )
        {
            ఒకవేళ( !ముసుగు[x] )
                కొనసాగు;
            కొరకు( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#నిర్వచించు DEF_COPY_ముసుగు(suffix, type) \
స్థిర శూన్య copy_ముసుగు_2##suffix(స్థిరరాశి uchar* src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, \
                             uchar* dst, size_t dstep, కొలత size, శూన్య*) \
{ \
    copy_ముసుగు<type>(src, sstep, ముసుగు, mstep, dst, dstep, size); \
}

#ఒకవేళ defined HAVE_IPP
#నిర్వచించు DEF_COPY_ముసుగు_F(suffix, type, ippfavor, ipptype) \
స్థిర శూన్య copy_ముసుగు_2##suffix(స్థిరరాశి uchar* src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, \
                             uchar* dst, size_t dstep, కొలత size, శూన్య*) \
{ \
    CV_IPP_నిరంతరం_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (స్థిరరాశి ipptype *)src, (పూర్ణాంకం)sstep, (ipptype *)dst, (పూర్ణాంకం)dstep, ippi_కొలత(size), (స్థిరరాశి Ipp8u *)ముసుగు, (పూర్ణాంకం)mstep) >= 0)\
    copy_ముసుగు<type>(src, sstep, ముసుగు, mstep, dst, dstep, size); \
}
#లేకపోతే
#నిర్వచించు DEF_COPY_ముసుగు_F(suffix, type, ippfavor, ipptype) \
స్థిర శూన్య copy_ముసుగు_2##suffix(స్థిరరాశి uchar* src, size_t sstep, స్థిరరాశి uchar* ముసుగు, size_t mstep, \
                             uchar* dst, size_t dstep, కొలత size, శూన్య*) \
{ \
    copy_ముసుగు<type>(src, sstep, ముసుగు, mstep, dst, dstep, size); \
}
#అయితే_ముగింపు

#ఒకవేళ IPP_సంస్కరణ_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_ముసుగు(32sC3, Vec3i)
DEF_COPY_ముసుగు(8uC3, Vec3b)
#లేకపోతే
DEF_COPY_ముసుగు_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_ముసుగు_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#అయితే_ముగింపు
DEF_COPY_ముసుగు(8u, uchar)
DEF_COPY_ముసుగు(16u, ushort)
DEF_COPY_ముసుగు_F(32s, పూర్ణాంకం, 32s_C1MR, Ipp32s)
DEF_COPY_ముసుగు_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_ముసుగు(32sC2, Vec2i)
DEF_COPY_ముసుగు_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_ముసుగు(32sC6, Vec6i)
DEF_COPY_ముసుగు(32sC8, Vec8i)

ద్విమాన_Func copy_ముసుగు_Tab[] =
{
    0,
    copyMask8u,
    copyMask16u,
    copyMask8uC3,
    copyMask32s,
    0,
    copyMask16uC3,
    0,
    copyMask32sC2,
    0, 0, 0,
    copyMask32sC3,
    0, 0, 0,
    copyMask32sC4,
    0, 0, 0, 0, 0, 0, 0,
    copyMask32sC6,
    0, 0, 0, 0, 0, 0, 0,
    copyMask32sC8
};

ద్విమాన_Func get_Copy_ముసుగు_Func(size_t esz)
{
    తిరిగి esz <= 32 && copy_ముసుగు_Tab[esz] ? copy_ముసుగు_Tab[esz] : copy_ముసుగు_Generic;
}

/* dst = src */
శూన్య Mat::copy_గమ్యం( OutputArray _dst ) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

#నిర్వచితమైతే HAVE_CUDA
    ఒకవేళ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ఇది);
        తిరిగి;
    }
#అయితే_ముగింపు

    పూర్ణాంకం stype = type();
    పూర్ణాంకం dtype = _dst.type();
    ఒకవేళ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_గమ్యం( _dst, dtype );
        తిరిగి;
    }

    ఒకవేళ( dims == 0 && empty() )
    {
        _dst.release();
        శూన్య* obj = _dst.getObj();
        ఒకవేళ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        లేకపోతే ఒకవేళ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        లేకపోతే ఒకవేళ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        తిరిగి;
    }

    తార్కిక allowTransposed = dims == 1 ||
        _dst.రకం() == ఆదానం_Array_2::STD_సదిశ ||
        (_dst.fixed_కొలత() && _dst.dims() == 1);
    ఒకవేళ( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        ఒకవేళ (empty())
            తిరిగి;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != ఖాళీ);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_కొలత(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        కొరకు( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        పూర్ణాంకం lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, దత్తాంశం, std::max(dims, 1), sz, dstofs, dst.అడుగు.p, అడుగు.p);
        తిరిగి;
    }

    ఒకవేళ( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        ఒకవేళ( దత్తాంశం == dst.దత్తాంశం )
            తిరిగి;

        ఒకవేళ( అడ్డువరుసలు > 0 && నిలువుల_సంఖ్య > 0 )
        {
            Mat src = *ఇది;
            కొలత sz = getContinuousSize2D(src, dst, (పూర్ణాంకం)elem_కొలత());
            CV_CheckGE(sz.వెడల్పు, 0, "");

            స్థిరరాశి uchar* sptr = src.దత్తాంశం;
            uchar* dptr = dst.దత్తాంశం;

#ఒకవేళ IPP_సంస్కరణ_X100 >= 201700
            CV_IPP_నిరంతరం_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (పూర్ణాంకం)src.అడుగు, dptr, (పూర్ణాంకం)dst.అడుగు, ippi_కొలత_L(sz.వెడల్పు, sz.ఎత్తు)) >= 0)
#అయితే_ముగింపు

            కొరకు (; sz.ఎత్తు--; sptr += src.అడుగు, dptr += dst.అడుగు)
                జ్ఞాపకం_నకలు(dptr, sptr, sz.వెడల్పు);
        }
        తిరిగి;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    ఒకవేళ( దత్తాంశం == dst.దత్తాంశం )
        తిరిగి;

    ఒకవేళ( మొత్తం() != 0 )
    {
        స్థిరరాశి Mat* arrays[] = { ఇది, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_కొలత();

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
            జ్ఞాపకం_నకలు(ptrs[1], ptrs[0], sz);
    }
}

#నిర్వచితమైతే HAVE_IPP
స్థిర తార్కిక ipp_copy_గమ్యం(స్థిరరాశి Mat &src, Mat &dst, స్థిరరాశి Mat &ముసుగు)
{
#నిర్వచితమైతే HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ఒకవేళ(ముసుగు.channels() > 1 || ముసుగు.లోతు() != CV_8U)
        తిరిగి అబద్ధం;

    ఒకవేళ (src.dims <= 2)
    {
        Ippi_కొలత size = ippi_కొలత(src.size());
        తిరిగి CV_INSTRUMENT_FUN_IPP(llwi_Copy_ముసుగు, src.ptr(), (పూర్ణాంకం)src.అడుగు, dst.ptr(), (పూర్ణాంకం)dst.అడుగు, size, (పూర్ణాంకం)src.elemSize1(), src.channels(), ముసుగు.ptr(), (పూర్ణాంకం)ముసుగు.అడుగు) >= 0;
    }
    లేకపోతే
    {
        స్థిరరాశి Mat      *arrays[] = {&src, &dst, &ముసుగు, ఖాళీ};
        uchar          *ptrs[3]  = {ఖాళీ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_కొలత size = ippi_కొలత(it.size, 1);

        కొరకు (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ముసుగు, ptrs[0], 0, ptrs[1], 0, size, (పూర్ణాంకం)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                తిరిగి అబద్ధం;
        }
        తిరిగి నిజం;
    }
#లేకపోతే
    CV_వాడని(src); CV_వాడని(dst); CV_వాడని(ముసుగు);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}
#అయితే_ముగింపు

శూన్య Mat::copy_గమ్యం( OutputArray _dst, ఆదానం_Array ముసుగు_2 ) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat ముసుగు = ముసుగు_2.getMat();
    ఒకవేళ( !ముసుగు.దత్తాంశం )
    {
        copy_గమ్యం(_dst);
        తిరిగి;
    }

    పూర్ణాంకం cn = channels(), mcn = ముసుగు.channels();
    CV_Assert( (ముసుగు.లోతు() == CV_8U || ముసుగు.లోతు() == CV_8S || ముసుగు.లోతు() == CV_Bool) && (mcn == 1 || mcn == cn) );
    తార్కిక color_ముసుగు = mcn > 1;
    ఒకవేళ( dims <= 2 )
    {
        CV_Assert( size() == ముసుగు.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        ఒకవేళ (dst.దత్తాంశం != dst0.దత్తాంశం) // re-allocation happened
        {
#నిర్వచితమైతే OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#అయితే_ముగింపు
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_నిరంతరం_FAST(ipp_copy_గమ్యం(*ఇది, dst, ముసుగు))
    ఒకవేళ ( ఇది->dims <= 2 ) {
        ఒకవేళ ( ఇది->size() == dst.size() && ఇది->size() == dst.size() ) {
            CALL_HAL(copy_గమ్యం_ముసుగు, cv_hal_copy_గమ్యం_Masked, ఇది->దత్తాంశం, ఇది->అడుగు, dst.దత్తాంశం, dst.అడుగు, ఇది->నిలువుల_సంఖ్య, ఇది->అడ్డువరుసలు, ఇది->type(), ముసుగు.దత్తాంశం, ముసుగు.అడుగు, ముసుగు.type());
        }
    }
    లేకపోతే ఒకవేళ ( ఇది->isContinuous() && dst.isContinuous() && ముసుగు.isContinuous() )
    {
        size_t sz = ఇది->మొత్తం();
        ఒకవేళ (sz < గరిష్ఠ_పూర్ణాంకం) {
            CALL_HAL(copy_గమ్యం_ముసుగు, cv_hal_copy_గమ్యం_Masked, ఇది->దత్తాంశం, 0, dst.దత్తాంశం, 0, (పూర్ణాంకం)sz, 1, ఇది->type(), ముసుగు.దత్తాంశం, 0, ముసుగు.type());
        }
    }

    size_t esz = color_ముసుగు ? elemSize1() : elem_కొలత();
    ద్విమాన_Func copymask = get_Copy_ముసుగు_Func(esz);

    ఒకవేళ( dims <= 2 )
    {
        Mat src = *ఇది;
        కొలత sz = getContinuousSize2D(src, dst, ముసుగు, mcn);
        copymask(src.దత్తాంశం, src.అడుగు, ముసుగు.దత్తాంశం, ముసుగు.అడుగు, dst.దత్తాంశం, dst.అడుగు, sz, &esz);
        తిరిగి;
    }

    స్థిరరాశి Mat* arrays[] = { ఇది, &dst, &ముసుగు, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    కొలత sz((పూర్ణాంకం)(it.size*mcn), 1);

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
శూన్య Mat::copyAt( OutputArray _dst ) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_గమ్యం(_dst);
}
శూన్య Mat::copyAt( OutputArray _dst, ఆదానం_Array ముసుగు_2 ) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_గమ్యం(_dst, ముసుగు_2);
}


స్థిర తార్కిక can_apply_memset(స్థిరరాశి Mat &mat, స్థిరరాశి Scalar &s, పూర్ణాంకం &fill_విలువ)
{
    // check if depth is 1 byte.
    ఎంపిక (mat.లోతు())
    {
    స్థితి CV_8U: fill_విలువ = saturate_cast<uchar>( s.val[0] ); ఆపు;
    స్థితి CV_8S: fill_విలువ = saturate_cast<schar>( s.val[0] ); ఆపు;
    అప్రమేయ: తిరిగి అబద్ధం;
    }

    // check if all element is same.
    స్థిరరాశి int64* is = (స్థిరరాశి int64*)&s.val[0];
    ఎంపిక (mat.channels())
    {
    స్థితి 1: తిరిగి నిజం;
    స్థితి 2: తిరిగి (is[0] == is[1]);
    స్థితి 3: తిరిగి (is[0] == is[1] && is[1] == is[2]);
    స్థితి 4: తిరిగి (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    అప్రమేయ: తిరిగి అబద్ధం;
    }
}

Mat& Mat::క్రియాకారకం = (స్థిరరాశి Scalar& s)
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (ఇది->empty())
        తిరిగి *ఇది;

    స్థిరరాశి Mat* arrays[] = { ఇది };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elem_కొలత();
    స్థిరరాశి int64* is = (స్థిరరాశి int64*)&s.val[0];

    ఒకవేళ( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    లేకపోతే
    {
        పూర్ణాంకం fill_విలువ = 0;
        ఒకవేళ ( can_apply_memset(*ఇది, s, fill_విలువ) )
        {
            కొరకు (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_విలువ, elsize);
            తిరిగి *ఇది;
        }

        ఒకవేళ( it.nplanes > 0 )
        {
            ద్విగుణ scalar[12];
            scalar_గమ్యం_Raw_దత్తాంశం(s, scalar, type(), 12);
            size_t ఖండం_కొలత_2 = 12*elemSize1();

            కొరకు( size_t j = 0; j < elsize; j += ఖండం_కొలత_2 )
            {
                size_t sz = MIN(ఖండం_కొలత_2, elsize - j);
                CV_Assert(sz <= పరిమాణం(scalar));
                జ్ఞాపకం_నకలు( dptr + j, scalar, sz );
            }
        }

        కొరకు( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            జ్ఞాపకం_నకలు( dptr, దత్తాంశం, elsize );
        }
    }
    తిరిగి *ఇది;
}

#నిర్వచితమైతే HAVE_IPP
స్థిర తార్కిక ipp_Mat_సమితి_గమ్యం_Mat(Mat &dst, Mat &_val, Mat &ముసుగు)
{
#నిర్వచితమైతే HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ఒకవేళ(ముసుగు.empty())
        తిరిగి అబద్ధం;

    ఒకవేళ(ముసుగు.లోతు() != CV_8U || ముసుగు.channels() > 1)
        తిరిగి అబద్ధం;

    ఒకవేళ(dst.channels() > 4)
        తిరిగి అబద్ధం;

    ఒకవేళ (dst.లోతు() == CV_32F)
    {
        కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)(_val.మొత్తం()); i++)
        {
            దశాంశం v = (దశాంశం)(_val.at<ద్విగుణ>(i));  // cast to float
            ఒకవేళ (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                తిరిగి అబద్ధం;
        }
    }

    ఒకవేళ(dst.dims <= 2)
    {
        Ippi_కొలత            size     = ippi_కొలత(dst.size());
        Ipp_దత్తాంశం_Type         దత్తాంశం_Type = ippi_Get_దత్తాంశం_Type(dst.లోతు());
        ::ipp::Iw_విలువ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        తిరిగి CV_INSTRUMENT_FUN_IPP(llwi_సమితి_ముసుగు, s, dst.ptr(), (పూర్ణాంకం)dst.అడుగు, size, దత్తాంశం_Type, dst.channels(), ముసుగు.ptr(), (పూర్ణాంకం)ముసుగు.అడుగు) >= 0;
    }
    లేకపోతే
    {
        స్థిరరాశి Mat      *arrays[] = {&dst, ముసుగు.empty()?ఖాళీ:&ముసుగు, ఖాళీ};
        uchar          *ptrs[2]  = {ఖాళీ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_కొలత            size     = {(పూర్ణాంకం)it.size, 1};
        Ipp_దత్తాంశం_Type         దత్తాంశం_Type = ippi_Get_దత్తాంశం_Type(dst.లోతు());
        ::ipp::Iw_విలువ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(llwi_సమితి_ముసుగు, s, ptrs[0], 0, size, దత్తాంశం_Type, dst.channels(), ptrs[1], 0) < 0)
                తిరిగి అబద్ధం;
        }
        తిరిగి నిజం;
    }
#లేకపోతే
    CV_వాడని(dst); CV_వాడని(_val); CV_వాడని(ముసుగు);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}
#అయితే_ముగింపు

Mat& Mat::సమితి_గమ్యం(ఆదానం_Array విలువ, ఆదానం_Array ముసుగు_2)
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( empty() )
        తిరిగి *ఇది;

    Mat value = విలువ.getMat(), ముసుగు = ముసుగు_2.getMat();

    CV_Assert( checkScalar(value, type(), విలువ.రకం(), ఆదానం_Array_2::MAT ));
    పూర్ణాంకం cn = channels(), mcn = ముసుగు.channels();
    CV_Assert( ముసుగు.empty() || ((ముసుగు.లోతు() == CV_8U || ముసుగు.లోతు() == CV_8S || ముసుగు.లోతు() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == ముసుగు.size) );

    CV_IPP_నిరంతరం_FAST(ipp_Mat_సమితి_గమ్యం_Mat(*ఇది, value, ముసుగు), *ఇది)

    size_t esz = mcn > 1 ? elemSize1() : elem_కొలత();
    ద్విమాన_Func copymask = get_Copy_ముసుగు_Func(esz);

    స్థిరరాశి Mat* arrays[] = { ఇది, !ముసుగు.empty() ? &ముసుగు : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం totalsz = (పూర్ణాంకం)it.size*mcn;
    పూర్ణాంకం ఖండం_Size0 = std::min(totalsz, (పూర్ణాంకం)((ఖండం_కొలత + esz-1)/esz));
    ఖండం_Size0 -= ఖండం_Size0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_బఫరు<uchar> _scbuf(ఖండం_Size0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.దత్తాంశం(), (పూర్ణాంకం)పరిమాణం(ద్విగుణ));
    convertAndUnrollScalar( value, type(), scbuf, ఖండం_Size0/mcn );

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( పూర్ణాంకం j = 0; j < totalsz; j += ఖండం_Size0 )
        {
            కొలత sz(std::min(ఖండం_Size0, totalsz - j), 1);
            size_t ఖండం_కొలత_2 = sz.వెడల్పు*esz;
            ఒకవేళ( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.వెడల్పు;
            }
            లేకపోతే
                జ్ఞాపకం_నకలు(ptrs[0], scbuf, ఖండం_కొలత_2);
            ptrs[0] += ఖండం_కొలత_2;
        }
    }
    తిరిగి *ఇది;
}


Mat& Mat::సమితి_Zero()
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( empty() )
        తిరిగి *ఇది;

    size_t esz = elem_కొలత();

    స్థిరరాశి Mat* arrays[] = { ఇది, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    తిరిగి *ఇది;
}


#ఒకవేళ defined HAVE_OPENCL && !defined __APPLE__

స్థిర తార్కిక ocl_repeat(ఆదానం_Array _src, పూర్ణాంకం ny, పూర్ణాంకం nx, OutputArray _dst)
{
    ఒకవేళ (ny == 1 && nx == 1)
    {
        _src.copy_గమ్యం(_dst);
        తిరిగి నిజం;
    }

    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type),
            అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predict_Optimal_సదిశ_వెడల్పు(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memop_Type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)),
                         nx, ny, అడ్డువరుసలు_చొప్పున_WI, kercn));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.వాదనలు(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_కొలత(dst));

    size_t globalsize[] = { (size_t)src.నిలువుల_సంఖ్య * cn / kercn, ((size_t)src.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

శూన్య repeat(ఆదానం_Array _src, పూర్ణాంకం ny, పూర్ణాంకం nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    కొలత ssize = _src.size();
    ఒకవేళ( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        తిరిగి;
    }
    CV_Assert( (int64_t)ssize.ఎత్తు * ny <= గరిష్ఠ_పూర్ణాంకం && (int64_t)ssize.వెడల్పు * nx <= గరిష్ఠ_పూర్ణాంకం );
    _dst.create(ssize.ఎత్తు*ny, ssize.వెడల్పు*nx, _src.type());

#ఒకవేళ !defined __APPLE__
    CV_OCL_నిరంతరం(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#అయితే_ముగింపు

    Mat src = _src.getMat(), dst = _dst.getMat();
    కొలత dsize = dst.size();
    పూర్ణాంకం esz = (పూర్ణాంకం)src.elem_కొలత();
    పూర్ణాంకం x, y;
    ssize.వెడల్పు *= esz; dsize.వెడల్పు *= esz;

    కొరకు( y = 0; y < ssize.ఎత్తు; y++ )
    {
        కొరకు( x = 0; x < dsize.వెడల్పు; x += ssize.వెడల్పు )
            జ్ఞాపకం_నకలు( dst.ptr(y) + x, src.ptr(y), ssize.వెడల్పు );
    }

    కొరకు( ; y < dsize.ఎత్తు; y++ )
        జ్ఞాపకం_నకలు( dst.ptr(y), dst.ptr(y - ssize.ఎత్తు), dsize.వెడల్పు );
}

Mat repeat(స్థిరరాశి Mat& src, పూర్ణాంకం ny, పూర్ణాంకం nx)
{
    ఒకవేళ( nx == 1 && ny == 1 )
        తిరిగి src;
    Mat dst;
    repeat(src, ny, nx, dst);
    తిరిగి dst;
}


} // cv


/*
 Various border types, image boundaries are denoted with '|'

 * BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
 * BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb
 * BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba
 * BORDER_WRAP:          cdefgh|abcdefgh|abcdefg
 * BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'
 */
పూర్ణాంకం cv::borderInterpolate( పూర్ణాంకం p, పూర్ణాంకం len, పూర్ణాంకం borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#నిర్వచితమైతే CV_STATIC_ANALYSIS
    ఒకవేళ(p >= 0 && p < len)
#లేకపోతే
    ఒకవేళ( (గుర్తులేని)p < (గుర్తులేని)len )
#అయితే_ముగింపు
        ;
    లేకపోతే ఒకవేళ( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    లేకపోతే ఒకవేళ( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        పూర్ణాంకం delta = borderType == BORDER_REFLECT_101;
        ఒకవేళ( len == 1 )
            తిరిగి 0;
        చేయి
        {
            ఒకవేళ( p < 0 )
                p = -p - 1 + delta;
            లేకపోతే
                p = len - 1 - (p - len) - delta;
        }
#నిర్వచితమైతే CV_STATIC_ANALYSIS
        అయితే(p < 0 || p >= len);
#లేకపోతే
        అయితే( (గుర్తులేని)p >= (గుర్తులేని)len );
#అయితే_ముగింపు
    }
    లేకపోతే ఒకవేళ( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        ఒకవేళ( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            ఒకవేళ( p64 < 0 )
                p64 += len;
            p = (పూర్ణాంకం)p64;
        }
    }
    లేకపోతే ఒకవేళ( borderType == BORDER_CONSTANT )
        p = -1;
    లేకపోతే
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    తిరిగి p;
}

నామాకాశ
{

శూన్య copyMakeBorder_8u( స్థిరరాశి uchar* src, size_t srcstep, cv::కొలత srcroi,
                        uchar* dst, size_t dststep, cv::కొలత dstroi,
                        పూర్ణాంకం శిఖరం_2, పూర్ణాంకం ఎడమ, పూర్ణాంకం cn, పూర్ణాంకం borderType )
{
    స్థిరరాశి పూర్ణాంకం isz = (పూర్ణాంకం)పరిమాణం(పూర్ణాంకం);
    పూర్ణాంకం i, j, k, elem_కొలత = 1;
    తార్కిక int_బహుళకం = అబద్ధం;

    ఒకవేళ( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_కొలత = isz;
        int_బహుళకం = నిజం;
    }

    cv::Auto_బఫరు<పూర్ణాంకం> _tab((dstroi.వెడల్పు - srcroi.వెడల్పు)*cn);
    పూర్ణాంకం* tab = _tab.దత్తాంశం();
    పూర్ణాంకం కుడి = dstroi.వెడల్పు - srcroi.వెడల్పు - ఎడమ;
    పూర్ణాంకం bottom = dstroi.ఎత్తు - srcroi.ఎత్తు - శిఖరం_2;

    కొరకు( i = 0; i < ఎడమ; i++ )
    {
        j = cv::borderInterpolate(i - ఎడమ, srcroi.వెడల్పు, borderType)*cn;
        కొరకు( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    కొరకు( i = 0; i < కుడి; i++ )
    {
        j = cv::borderInterpolate(srcroi.వెడల్పు + i, srcroi.వెడల్పు, borderType)*cn;
        కొరకు( k = 0; k < cn; k++ )
            tab[(i+ఎడమ)*cn + k] = j + k;
    }

    srcroi.వెడల్పు *= cn;
    dstroi.వెడల్పు *= cn;
    ఎడమ *= cn;
    కుడి *= cn;

    uchar* dstInner = dst + dststep*శిఖరం_2 + ఎడమ*elem_కొలత;

    కొరకు( i = 0; i < srcroi.ఎత్తు; i++, dstInner += dststep, src += srcstep )
    {
        ఒకవేళ( dstInner != src )
            జ్ఞాపకం_నకలు(dstInner, src, srcroi.వెడల్పు*elem_కొలత);

        ఒకవేళ( int_బహుళకం )
        {
            స్థిరరాశి పూర్ణాంకం* isrc = (పూర్ణాంకం*)src;
            పూర్ణాంకం* idstInner = (పూర్ణాంకం*)dstInner;
            కొరకు( j = 0; j < ఎడమ; j++ )
                idstInner[j - ఎడమ] = isrc[tab[j]];
            కొరకు( j = 0; j < కుడి; j++ )
                idstInner[j + srcroi.వెడల్పు] = isrc[tab[j + ఎడమ]];
        }
        లేకపోతే
        {
            కొరకు( j = 0; j < ఎడమ; j++ )
                dstInner[j - ఎడమ] = src[tab[j]];
            కొరకు( j = 0; j < కుడి; j++ )
                dstInner[j + srcroi.వెడల్పు] = src[tab[j + ఎడమ]];
        }
    }

    dstroi.వెడల్పు *= elem_కొలత;

    కొరకు( i = 0; i < శిఖరం_2; i++ )
    {
        j = cv::borderInterpolate(i - శిఖరం_2, srcroi.ఎత్తు, borderType);
        జ్ఞాపకం_నకలు(dst + i*dststep, dst + (శిఖరం_2+j)*dststep, dstroi.వెడల్పు);
    }

    dst += dststep*శిఖరం_2;
    కొరకు( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.ఎత్తు, srcroi.ఎత్తు, borderType);
        జ్ఞాపకం_నకలు(dst + (i + srcroi.ఎత్తు)*dststep, dst + j*dststep, dstroi.వెడల్పు);
    }
}


శూన్య copyMakeConstBorder_8u( స్థిరరాశి uchar* src, size_t srcstep, cv::కొలత srcroi,
                             uchar* dst, size_t dststep, cv::కొలత dstroi,
                             పూర్ణాంకం శిఖరం_2, పూర్ణాంకం ఎడమ, పూర్ణాంకం cn, స్థిరరాశి uchar* value )
{
    పూర్ణాంకం i, j;
    cv::Auto_బఫరు<uchar> _constBuf(dstroi.వెడల్పు*cn);
    uchar* constBuf = _constBuf.దత్తాంశం();
    పూర్ణాంకం కుడి = dstroi.వెడల్పు - srcroi.వెడల్పు - ఎడమ;
    పూర్ణాంకం bottom = dstroi.ఎత్తు - srcroi.ఎత్తు - శిఖరం_2;

    కొరకు( i = 0; i < dstroi.వెడల్పు; i++ )
    {
        కొరకు( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.వెడల్పు *= cn;
    dstroi.వెడల్పు *= cn;
    ఎడమ *= cn;
    కుడి *= cn;

    uchar* dstInner = dst + dststep*శిఖరం_2 + ఎడమ;

    కొరకు( i = 0; i < srcroi.ఎత్తు; i++, dstInner += dststep, src += srcstep )
    {
        ఒకవేళ( dstInner != src )
            జ్ఞాపకం_నకలు( dstInner, src, srcroi.వెడల్పు );
        జ్ఞాపకం_నకలు( dstInner - ఎడమ, constBuf, ఎడమ );
        జ్ఞాపకం_నకలు( dstInner + srcroi.వెడల్పు, constBuf, కుడి );
    }

    కొరకు( i = 0; i < శిఖరం_2; i++ )
        జ్ఞాపకం_నకలు(dst + i * dststep, constBuf, dstroi.వెడల్పు);

    dst += (శిఖరం_2 + srcroi.ఎత్తు) * dststep;
    కొరకు( i = 0; i < bottom; i++ )
        జ్ఞాపకం_నకలు(dst + i * dststep, constBuf, dstroi.వెడల్పు);
}

}

#నిర్వచితమైతే HAVE_OPENCL

నామాకాశ cv {

స్థిర తార్కిక ocl_copyMakeBorder( ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం శిఖరం_2, పూర్ణాంకం bottom,
                                పూర్ణాంకం ఎడమ, పూర్ణాంకం కుడి, పూర్ణాంకం borderType, స్థిరరాశి Scalar& value )
{
    పూర్ణాంకం type = _src.type(), cn = CV_MAT_CN(type), లోతు = CV_MAT_లోతు(type),
            అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    తార్కిక isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    ఒకవేళ ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        తిరిగి అబద్ధం;

    స్థిరరాశి అక్షరం * స్థిరరాశి borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    పూర్ణాంకం scalarcn = cn == 3 ? 4 : cn;
    పూర్ణాంకం sctype = CV_MAKETYPE(లోతు, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memop_Type_గమ్యం_Str(type), borderMap[borderType],
                                 ocl::memop_Type_గమ్యం_Str(లోతు), cn,
                                 ocl::memop_Type_గమ్యం_Str(sctype), అడ్డువరుసలు_చొప్పున_WI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat();
    ఒకవేళ( src.isSubmatrix() && !isolated )
    {
        కొలత సంపూర్ణ_కొలత;
        బిందువు ofs;
        src.locateROI(సంపూర్ణ_కొలత, ofs);
        పూర్ణాంకం dtop = std::min(ofs.y, శిఖరం_2);
        పూర్ణాంకం dbottom = std::min(సంపూర్ణ_కొలత.ఎత్తు - src.అడ్డువరుసలు - ofs.y, bottom);
        పూర్ణాంకం dleft = std::min(ofs.x, ఎడమ);
        పూర్ణాంకం dright = std::min(సంపూర్ణ_కొలత.వెడల్పు - src.నిలువుల_సంఖ్య - ofs.x, కుడి);
        src.adjustROI(dtop, dbottom, dleft, dright);
        శిఖరం_2 -= dtop;
        ఎడమ -= dleft;
        bottom -= dbottom;
        కుడి -= dright;
    }

    _dst.create(src.అడ్డువరుసలు + శిఖరం_2 + bottom, src.నిలువుల_సంఖ్య + ఎడమ + కుడి, type);
    UMat dst = _dst.getUMat();

    ఒకవేళ (శిఖరం_2 == 0 && ఎడమ == 0 && bottom == 0 && కుడి == 0)
    {
        ఒకవేళ(src.u != dst.u || src.అడుగు != dst.అడుగు)
            src.copy_గమ్యం(dst);
        తిరిగి నిజం;
    }

    k.వాదనలు(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           శిఖరం_2, ఎడమ, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.నిలువుల_సంఖ్య, ((size_t)dst.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

}
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_IPP
నామాకాశ cv {

స్థిర తార్కిక ipp_copyMakeBorder( Mat &_src, Mat &_dst, పూర్ణాంకం శిఖరం_2, పూర్ణాంకం bottom,
                                పూర్ణాంకం ఎడమ, పూర్ణాంకం కుడి, పూర్ణాంకం _borderType, స్థిరరాశి Scalar& value )
{
#ఒకవేళ defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_కొలత border_కొలత(ఎడమ, శిఖరం_2, కుడి, bottom);
    ::ipp::Iwi_కొలత       size(_src.నిలువుల_సంఖ్య, _src.అడ్డువరుసలు);
    Ipp_దత్తాంశం_Type          దత్తాంశం_Type   = ippi_Get_దత్తాంశం_Type(_src.లోతు());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    ఒకవేళ((పూర్ణాంకం)borderType == -1)
        తిరిగి అబద్ధం;

    ఒకవేళ(_src.dims > 2)
        తిరిగి అబద్ధం;

    Rect dstRect(border_కొలత.ఎడమ, border_కొలత.శిఖరం_2,
        _dst.నిలువుల_సంఖ్య - border_కొలత.కుడి - border_కొలత.ఎడమ,
        _dst.అడ్డువరుసలు - border_కొలత.bottom - border_కొలత.శిఖరం_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    తిరిగి CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->అడుగు, subDst.ptr(), subDst.అడుగు, size, దత్తాంశం_Type, _src.channels(), border_కొలత, borderType, &value[0]) >= 0;
#లేకపోతే
    CV_వాడని(_src); CV_వాడని(_dst); CV_వాడని(శిఖరం_2); CV_వాడని(bottom); CV_వాడని(ఎడమ); CV_వాడని(కుడి);
    CV_వాడని(_borderType); CV_వాడని(value);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}
}
#అయితే_ముగింపు

శూన్య cv::copyMakeBorder( ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం శిఖరం_2, పూర్ణాంకం bottom,
                         పూర్ణాంకం ఎడమ, పూర్ణాంకం కుడి, పూర్ణాంకం borderType, స్థిరరాశి Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( శిఖరం_2 >= 0 && bottom >= 0 && ఎడమ >= 0 && కుడి >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.అడ్డువరుసలు + శిఖరం_2 + bottom <= గరిష్ఠ_పూర్ణాంకం );
    CV_Assert( (int64_t)src.నిలువుల_సంఖ్య + ఎడమ + కుడి <= గరిష్ఠ_పూర్ణాంకం );

    CV_OCL_నిరంతరం(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, శిఖరం_2, bottom, ఎడమ, కుడి, borderType, value))
    పూర్ణాంకం type = src.type();

    ఒకవేళ( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        కొలత సంపూర్ణ_కొలత;
        బిందువు ofs;
        src.locateROI(సంపూర్ణ_కొలత, ofs);
        పూర్ణాంకం dtop = std::min(ofs.y, శిఖరం_2);
        పూర్ణాంకం dbottom = std::min(సంపూర్ణ_కొలత.ఎత్తు - src.అడ్డువరుసలు - ofs.y, bottom);
        పూర్ణాంకం dleft = std::min(ofs.x, ఎడమ);
        పూర్ణాంకం dright = std::min(సంపూర్ణ_కొలత.వెడల్పు - src.నిలువుల_సంఖ్య - ofs.x, కుడి);
        src.adjustROI(dtop, dbottom, dleft, dright);
        శిఖరం_2 -= dtop;
        ఎడమ -= dleft;
        bottom -= dbottom;
        కుడి -= dright;
    }

    _dst.create( src.అడ్డువరుసలు + శిఖరం_2 + bottom, src.నిలువుల_సంఖ్య + ఎడమ + కుడి, type );
    Mat dst = _dst.getMat();

    ఒకవేళ(శిఖరం_2 == 0 && ఎడమ == 0 && bottom == 0 && కుడి == 0)
    {
        ఒకవేళ(src.దత్తాంశం != dst.దత్తాంశం || src.అడుగు != dst.అడుగు)
            src.copy_గమ్యం(dst);
        తిరిగి;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_నిరంతరం_FAST(ipp_copyMakeBorder(src, dst, శిఖరం_2, bottom, ఎడమ, కుడి, borderType, value))

    ఒకవేళ( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.అడుగు, src.size(),
                           dst.ptr(), dst.అడుగు, dst.size(),
                           శిఖరం_2, ఎడమ, (పూర్ణాంకం)src.elem_కొలత(), borderType );
    లేకపోతే
    {
        పూర్ణాంకం cn = src.channels(), cn1 = cn;
        Auto_బఫరు<ద్విగుణ> buf(cn);
        ఒకవేళ( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_గమ్యం_Raw_దత్తాంశం(value, buf.దత్తాంశం(), CV_MAKETYPE(src.లోతు(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.అడుగు, src.size(),
                                dst.ptr(), dst.అడుగు, dst.size(),
                                శిఖరం_2, ఎడమ, (పూర్ణాంకం)src.elem_కొలత(), (uchar*)buf.దత్తాంశం() );
    }
}
/* End of file. */
