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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

ਨਾਮਾਕਾਸ਼ cv
{

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(ਅਚਲ Scalar& s, T * ਅਚਲ buf, ਅਚਲ ਪੂਰਨ cn, ਅਚਲ ਪੂਰਨ unroll_ਮੰਜ਼ਿਲ)
{
    ਪੂਰਨ i = 0;
    ਲਈ(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    ਲਈ(; i < unroll_ਮੰਜ਼ਿਲ; i++)
        buf[i] = buf[i-cn];
}

ਖਾਲੀ scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ(ਅਚਲ Scalar& s, ਖਾਲੀ* _buf, ਪੂਰਨ type, ਪੂਰਨ unroll_ਮੰਜ਼ਿਲ)
{
    CV_INSTRUMENT_REGION();

    ਅਚਲ ਪੂਰਨ ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    ਚੋਣ(ਡੂੰਘਾਈ)
    {
    ਹਾਲਤ CV_8U:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (uchar*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_8S:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (schar*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_Bool:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (ਤਾਰਕਿਕ*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_16U:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (ushort*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_16S:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (ਛੋਟਾ*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_16F:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (hfloat*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_16BF:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (bfloat*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_8F_E4M3FN:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (fp8_t*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_8F_E4M3FNUZ:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (fp8a_t*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_32U:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (ਬਿਨਾਨਿਸ਼ਾਨੀ*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_32S:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (ਪੂਰਨ*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_32F:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (ਦਸ਼ਮਲਵ*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_64U:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (uint64_t*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_64S:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (int64_t*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਹਾਲਤ CV_64F:
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ_2(s, (ਦੁੱਗਣਾ*)_buf, cn, unroll_ਮੰਜ਼ਿਲ);
        ਰੋਕੋ;
    ਮੂਲ:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

ਖਾਲੀ convertAndUnrollScalar( ਅਚਲ Mat& sc, ਪੂਰਨ buftype, uchar* scbuf, size_t blocksize )
{
    ਪੂਰਨ scn = (ਪੂਰਨ)sc.ਕੁੱਲ(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_ਮਾਪ(buftype);
    ਦੁਆਧਾਰੀ_Func cvtFn = getConvertFunc(sc.ਡੂੰਘਾਈ(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, ਮਾਪ(std::min(cn, scn), 1), 0);
    // unroll the scalar
    ਜੇ( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        ਲਈ( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    ਲਈ( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
copy_ਨਕਾਬ(ਅਚਲ uchar* _src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, uchar* _dst, size_t dstep, ਮਾਪ size)
{
    ਲਈ( ; size.ਉਚਾਈ--; ਨਕਾਬ += mstep, _src += sstep, _dst += dstep )
    {
        ਅਚਲ T* src = (ਅਚਲ T*)_src;
        T* dst = (T*)_dst;
        ਪੂਰਨ x = 0;
         #ਜੇ CV_ENABLE_UNROLLED
        ਲਈ( ; x <= size.ਚੌੜਾਈ - 4; x += 4 )
        {
            ਜੇ( ਨਕਾਬ[x] )
                dst[x] = src[x];
            ਜੇ( ਨਕਾਬ[x+1] )
                dst[x+1] = src[x+1];
            ਜੇ( ਨਕਾਬ[x+2] )
                dst[x+2] = src[x+2];
            ਜੇ( ਨਕਾਬ[x+3] )
                dst[x+3] = src[x+3];
        }
        #ਜੇ_ਅੰਤ
        ਲਈ( ; x < size.ਚੌੜਾਈ; x++ )
            ਜੇ( ਨਕਾਬ[x] )
                dst[x] = src[x];
    }
}

ਸਾਂਚਾ<> ਖਾਲੀ
copy_ਨਕਾਬ<uchar>(ਅਚਲ uchar* _src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, uchar* _dst, size_t dstep, ਮਾਪ size)
{
    CV_IPP_ਲੜੀਵਾਰ_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (ਪੂਰਨ)sstep, _dst, (ਪੂਰਨ)dstep, ippi_ਮਾਪ(size), ਨਕਾਬ, (ਪੂਰਨ)mstep) >= 0)

    ਲਈ( ; size.ਉਚਾਈ--; ਨਕਾਬ += mstep, _src += sstep, _dst += dstep )
    {
        ਅਚਲ uchar* src = (ਅਚਲ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ਪੂਰਨ x = 0;
        #ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            ਲਈ( ; x <= size.ਚੌੜਾਈ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_ਲੱਦੋ(src  + x),
                        v_dst   = vx_ਲੱਦੋ(dst  + x),
                        v_nmask = v_eq(vx_ਲੱਦੋ(ਨਕਾਬ + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #ਜੇ_ਅੰਤ
        ਲਈ( ; x < size.ਚੌੜਾਈ; x++ )
            ਜੇ( ਨਕਾਬ[x] )
                dst[x] = src[x];
    }
}

ਸਾਂਚਾ<> ਖਾਲੀ
copy_ਨਕਾਬ<Vec3b>(ਅਚਲ uchar* _src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, uchar* _dst, size_t dstep, ਮਾਪ size)
{
    ਲਈ( ; size.ਉਚਾਈ--; ਨਕਾਬ += mstep, _src += sstep, _dst += dstep )
    {
        ਅਚਲ uchar* src = (ਅਚਲ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ਪੂਰਨ x = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਲਈ( ; x <= size.ਚੌੜਾਈ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_ਲੱਦੋ(ਨਕਾਬ + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_ਲੱਦੋ_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_ਲੱਦੋ_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#ਜੇ_ਅੰਤ
        ਲਈ( ; x < size.ਚੌੜਾਈ; x++ )
            ਜੇ( ਨਕਾਬ[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

ਸਾਂਚਾ<> ਖਾਲੀ
copy_ਨਕਾਬ<ushort>(ਅਚਲ uchar* _src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, uchar* _dst, size_t dstep, ਮਾਪ size)
{
    CV_IPP_ਲੜੀਵਾਰ_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ਅਚਲ Ipp16u *)_src, (ਪੂਰਨ)sstep, (Ipp16u *)_dst, (ਪੂਰਨ)dstep, ippi_ਮਾਪ(size), ਨਕਾਬ, (ਪੂਰਨ)mstep) >= 0)

    ਲਈ( ; size.ਉਚਾਈ--; ਨਕਾਬ += mstep, _src += sstep, _dst += dstep )
    {
        ਅਚਲ ushort* src = (ਅਚਲ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ਪੂਰਨ x = 0;
        #ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            ਲਈ( ; x <= size.ਚੌੜਾਈ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_ਲੱਦੋ(src + x), v_src2 = vx_ਲੱਦੋ(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_ਲੱਦੋ(dst + x), v_dst2 = vx_ਲੱਦੋ(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_ਲੱਦੋ(ਨਕਾਬ + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #ਜੇ_ਅੰਤ
        ਲਈ( ; x < size.ਚੌੜਾਈ; x++ )
            ਜੇ( ਨਕਾਬ[x] )
                dst[x] = src[x];
    }
}

ਸਾਂਚਾ<> ਖਾਲੀ
copy_ਨਕਾਬ<Vec3s>(ਅਚਲ uchar* _src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, uchar* _dst, size_t dstep, ਮਾਪ size)
{
    ਲਈ( ; size.ਉਚਾਈ--; ਨਕਾਬ += mstep, _src += sstep, _dst += dstep )
    {
        ਅਚਲ ushort* src = (ਅਚਲ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ਪੂਰਨ x = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਲਈ( ; x <= size.ਚੌੜਾਈ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_ਲੱਦੋ(ਨਕਾਬ + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_ਲੱਦੋ_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_ਲੱਦੋ_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_ਲੱਦੋ_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_ਲੱਦੋ_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

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
#ਜੇ_ਅੰਤ
        ਲਈ( ; x < size.ਚੌੜਾਈ; x++ )
            ਜੇ( ਨਕਾਬ[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

ਸਾਂਚਾ<> ਖਾਲੀ
copy_ਨਕਾਬ<ਪੂਰਨ>(ਅਚਲ uchar* _src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, uchar* _dst, size_t dstep, ਮਾਪ size)
{
    ਲਈ( ; size.ਉਚਾਈ--; ਨਕਾਬ += mstep, _src += sstep, _dst += dstep )
    {
        ਅਚਲ ਪੂਰਨ* src = (ਅਚਲ ਪੂਰਨ*)_src;
        ਪੂਰਨ* dst = (ਪੂਰਨ*)_dst;
        ਪੂਰਨ x = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਲਈ (; x <= size.ਚੌੜਾਈ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_ਲੱਦੋ(src + x), v_dst0 = vx_ਲੱਦੋ(dst + x);
            v_int32 v_src1 = vx_ਲੱਦੋ(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_ਲੱਦੋ(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_ਲੱਦੋ(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_ਲੱਦੋ(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_ਲੱਦੋ(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_ਲੱਦੋ(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_ਲੱਦੋ(ਨਕਾਬ + x), vx_setzero_u8());
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
#ਜੇ_ਅੰਤ
        ਲਈ (; x < size.ਚੌੜਾਈ; x++)
            ਜੇ ( ਨਕਾਬ[x] )
                dst[x] = src[x];
    }
}

ਸਥਿਰ ਖਾਲੀ
copy_ਨਕਾਬ_Generic(ਅਚਲ uchar* _src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, uchar* _dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    ਲਈ( ; size.ਉਚਾਈ--; ਨਕਾਬ += mstep, _src += sstep, _dst += dstep )
    {
        ਅਚਲ uchar* src = _src;
        uchar* dst = _dst;
        ਪੂਰਨ x = 0;
        ਲਈ( ; x < size.ਚੌੜਾਈ; x++, src += esz, dst += esz )
        {
            ਜੇ( !ਨਕਾਬ[x] )
                ਜਾਰੀ;
            ਲਈ( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_COPY_ਨਕਾਬ(suffix, type) \
ਸਥਿਰ ਖਾਲੀ copy_ਨਕਾਬ_2##suffix(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, \
                             uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*) \
{ \
    copy_ਨਕਾਬ<type>(src, sstep, ਨਕਾਬ, mstep, dst, dstep, size); \
}

#ਜੇ defined HAVE_IPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_COPY_ਨਕਾਬ_F(suffix, type, ippfavor, ipptype) \
ਸਥਿਰ ਖਾਲੀ copy_ਨਕਾਬ_2##suffix(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, \
                             uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*) \
{ \
    CV_IPP_ਲੜੀਵਾਰ_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ਅਚਲ ipptype *)src, (ਪੂਰਨ)sstep, (ipptype *)dst, (ਪੂਰਨ)dstep, ippi_ਮਾਪ(size), (ਅਚਲ Ipp8u *)ਨਕਾਬ, (ਪੂਰਨ)mstep) >= 0)\
    copy_ਨਕਾਬ<type>(src, sstep, ਨਕਾਬ, mstep, dst, dstep, size); \
}
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEF_COPY_ਨਕਾਬ_F(suffix, type, ippfavor, ipptype) \
ਸਥਿਰ ਖਾਲੀ copy_ਨਕਾਬ_2##suffix(ਅਚਲ uchar* src, size_t sstep, ਅਚਲ uchar* ਨਕਾਬ, size_t mstep, \
                             uchar* dst, size_t dstep, ਮਾਪ size, ਖਾਲੀ*) \
{ \
    copy_ਨਕਾਬ<type>(src, sstep, ਨਕਾਬ, mstep, dst, dstep, size); \
}
#ਜੇ_ਅੰਤ

#ਜੇ IPP_ਸੰਸਕਰਣ_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_ਨਕਾਬ(32sC3, Vec3i)
DEF_COPY_ਨਕਾਬ(8uC3, Vec3b)
#ਨਹੀਂਤਾਂ
DEF_COPY_ਨਕਾਬ_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_ਨਕਾਬ_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#ਜੇ_ਅੰਤ
DEF_COPY_ਨਕਾਬ(8u, uchar)
DEF_COPY_ਨਕਾਬ(16u, ushort)
DEF_COPY_ਨਕਾਬ_F(32s, ਪੂਰਨ, 32s_C1MR, Ipp32s)
DEF_COPY_ਨਕਾਬ_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_ਨਕਾਬ(32sC2, Vec2i)
DEF_COPY_ਨਕਾਬ_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_ਨਕਾਬ(32sC6, Vec6i)
DEF_COPY_ਨਕਾਬ(32sC8, Vec8i)

ਦੁਆਧਾਰੀ_Func copy_ਨਕਾਬ_Tab[] =
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

ਦੁਆਧਾਰੀ_Func get_Copy_ਨਕਾਬ_Func(size_t esz)
{
    ਵਾਪਸ esz <= 32 && copy_ਨਕਾਬ_Tab[esz] ? copy_ਨਕਾਬ_Tab[esz] : copy_ਨਕਾਬ_Generic;
}

/* dst = src */
ਖਾਲੀ Mat::copy_ਮੰਜ਼ਿਲ( OutputArray _dst ) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਜੇ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ਇਹ);
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    ਪੂਰਨ stype = type();
    ਪੂਰਨ dtype = _dst.type();
    ਜੇ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_ਮੰਜ਼ਿਲ( _dst, dtype );
        ਵਾਪਸ;
    }

    ਜੇ( dims == 0 && empty() )
    {
        _dst.release();
        ਖਾਲੀ* obj = _dst.getObj();
        ਜੇ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ਨਹੀਂਤਾਂ ਜੇ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ਨਹੀਂਤਾਂ ਜੇ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ਵਾਪਸ;
    }

    ਤਾਰਕਿਕ allowTransposed = dims == 1 ||
        _dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼ ||
        (_dst.fixed_ਮਾਪ() && _dst.dims() == 1);
    ਜੇ( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        ਜੇ (empty())
            ਵਾਪਸ;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != ਨਿਰਾ);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_ਮਾਪ(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        ਲਈ( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        ਪੂਰਨ lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, ਅੰਕੜੇ, std::max(dims, 1), sz, dstofs, dst.ਕਦਮ.p, ਕਦਮ.p);
        ਵਾਪਸ;
    }

    ਜੇ( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        ਜੇ( ਅੰਕੜੇ == dst.ਅੰਕੜੇ )
            ਵਾਪਸ;

        ਜੇ( ਪੰਕਤੀਆਂ > 0 && ਖਾਨੇ_ਗਿਣਤੀ > 0 )
        {
            Mat src = *ਇਹ;
            ਮਾਪ sz = getContinuousSize2D(src, dst, (ਪੂਰਨ)elem_ਮਾਪ());
            CV_CheckGE(sz.ਚੌੜਾਈ, 0, "");

            ਅਚਲ uchar* sptr = src.ਅੰਕੜੇ;
            uchar* dptr = dst.ਅੰਕੜੇ;

#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 201700
            CV_IPP_ਲੜੀਵਾਰ_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (ਪੂਰਨ)src.ਕਦਮ, dptr, (ਪੂਰਨ)dst.ਕਦਮ, ippi_ਮਾਪ_L(sz.ਚੌੜਾਈ, sz.ਉਚਾਈ)) >= 0)
#ਜੇ_ਅੰਤ

            ਲਈ (; sz.ਉਚਾਈ--; sptr += src.ਕਦਮ, dptr += dst.ਕਦਮ)
                ਸਮ੍ਰਿਤੀ_ਨਕਲ(dptr, sptr, sz.ਚੌੜਾਈ);
        }
        ਵਾਪਸ;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    ਜੇ( ਅੰਕੜੇ == dst.ਅੰਕੜੇ )
        ਵਾਪਸ;

    ਜੇ( ਕੁੱਲ() != 0 )
    {
        ਅਚਲ Mat* arrays[] = { ਇਹ, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_ਮਾਪ();

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptrs[1], ptrs[0], sz);
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
ਸਥਿਰ ਤਾਰਕਿਕ ipp_copy_ਮੰਜ਼ਿਲ(ਅਚਲ Mat &src, Mat &dst, ਅਚਲ Mat &ਨਕਾਬ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ਜੇ(ਨਕਾਬ.channels() > 1 || ਨਕਾਬ.ਡੂੰਘਾਈ() != CV_8U)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (src.dims <= 2)
    {
        Ippi_ਮਾਪ size = ippi_ਮਾਪ(src.size());
        ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(llwi_Copy_ਨਕਾਬ, src.ptr(), (ਪੂਰਨ)src.ਕਦਮ, dst.ptr(), (ਪੂਰਨ)dst.ਕਦਮ, size, (ਪੂਰਨ)src.elemSize1(), src.channels(), ਨਕਾਬ.ptr(), (ਪੂਰਨ)ਨਕਾਬ.ਕਦਮ) >= 0;
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ Mat      *arrays[] = {&src, &dst, &ਨਕਾਬ, ਨਿਰਾ};
        uchar          *ptrs[3]  = {ਨਿਰਾ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ਮਾਪ size = ippi_ਮਾਪ(it.size, 1);

        ਲਈ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ਨਕਾਬ, ptrs[0], 0, ptrs[1], 0, size, (ਪੂਰਨ)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
        ਵਾਪਸ ਸੱਚ;
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(dst); CV_ਅਣਵਰਤਿਆ(ਨਕਾਬ);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ Mat::copy_ਮੰਜ਼ਿਲ( OutputArray _dst, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2 ) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat ਨਕਾਬ = ਨਕਾਬ_2.getMat();
    ਜੇ( !ਨਕਾਬ.ਅੰਕੜੇ )
    {
        copy_ਮੰਜ਼ਿਲ(_dst);
        ਵਾਪਸ;
    }

    ਪੂਰਨ cn = channels(), mcn = ਨਕਾਬ.channels();
    CV_Assert( (ਨਕਾਬ.ਡੂੰਘਾਈ() == CV_8U || ਨਕਾਬ.ਡੂੰਘਾਈ() == CV_8S || ਨਕਾਬ.ਡੂੰਘਾਈ() == CV_Bool) && (mcn == 1 || mcn == cn) );
    ਤਾਰਕਿਕ color_ਨਕਾਬ = mcn > 1;
    ਜੇ( dims <= 2 )
    {
        CV_Assert( size() == ਨਕਾਬ.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        ਜੇ (dst.ਅੰਕੜੇ != dst0.ਅੰਕੜੇ) // re-allocation happened
        {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#ਜੇ_ਅੰਤ
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_copy_ਮੰਜ਼ਿਲ(*ਇਹ, dst, ਨਕਾਬ))
    ਜੇ ( ਇਹ->dims <= 2 ) {
        ਜੇ ( ਇਹ->size() == dst.size() && ਇਹ->size() == dst.size() ) {
            CALL_HAL(copy_ਮੰਜ਼ਿਲ_ਨਕਾਬ, cv_hal_copy_ਮੰਜ਼ਿਲ_Masked, ਇਹ->ਅੰਕੜੇ, ਇਹ->ਕਦਮ, dst.ਅੰਕੜੇ, dst.ਕਦਮ, ਇਹ->ਖਾਨੇ_ਗਿਣਤੀ, ਇਹ->ਪੰਕਤੀਆਂ, ਇਹ->type(), ਨਕਾਬ.ਅੰਕੜੇ, ਨਕਾਬ.ਕਦਮ, ਨਕਾਬ.type());
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ ( ਇਹ->isContinuous() && dst.isContinuous() && ਨਕਾਬ.isContinuous() )
    {
        size_t sz = ਇਹ->ਕੁੱਲ();
        ਜੇ (sz < ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) {
            CALL_HAL(copy_ਮੰਜ਼ਿਲ_ਨਕਾਬ, cv_hal_copy_ਮੰਜ਼ਿਲ_Masked, ਇਹ->ਅੰਕੜੇ, 0, dst.ਅੰਕੜੇ, 0, (ਪੂਰਨ)sz, 1, ਇਹ->type(), ਨਕਾਬ.ਅੰਕੜੇ, 0, ਨਕਾਬ.type());
        }
    }

    size_t esz = color_ਨਕਾਬ ? elemSize1() : elem_ਮਾਪ();
    ਦੁਆਧਾਰੀ_Func copymask = get_Copy_ਨਕਾਬ_Func(esz);

    ਜੇ( dims <= 2 )
    {
        Mat src = *ਇਹ;
        ਮਾਪ sz = getContinuousSize2D(src, dst, ਨਕਾਬ, mcn);
        copymask(src.ਅੰਕੜੇ, src.ਕਦਮ, ਨਕਾਬ.ਅੰਕੜੇ, ਨਕਾਬ.ਕਦਮ, dst.ਅੰਕੜੇ, dst.ਕਦਮ, sz, &esz);
        ਵਾਪਸ;
    }

    ਅਚਲ Mat* arrays[] = { ਇਹ, &dst, &ਨਕਾਬ, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ਮਾਪ sz((ਪੂਰਨ)(it.size*mcn), 1);

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
ਖਾਲੀ Mat::copyAt( OutputArray _dst ) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_ਮੰਜ਼ਿਲ(_dst);
}
ਖਾਲੀ Mat::copyAt( OutputArray _dst, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2 ) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_ਮੰਜ਼ਿਲ(_dst, ਨਕਾਬ_2);
}


ਸਥਿਰ ਤਾਰਕਿਕ can_apply_memset(ਅਚਲ Mat &mat, ਅਚਲ Scalar &s, ਪੂਰਨ &fill_ਮੁੱਲ)
{
    // check if depth is 1 byte.
    ਚੋਣ (mat.ਡੂੰਘਾਈ())
    {
    ਹਾਲਤ CV_8U: fill_ਮੁੱਲ = saturate_cast<uchar>( s.val[0] ); ਰੋਕੋ;
    ਹਾਲਤ CV_8S: fill_ਮੁੱਲ = saturate_cast<schar>( s.val[0] ); ਰੋਕੋ;
    ਮੂਲ: ਵਾਪਸ ਝੂਠ;
    }

    // check if all element is same.
    ਅਚਲ int64* is = (ਅਚਲ int64*)&s.val[0];
    ਚੋਣ (mat.channels())
    {
    ਹਾਲਤ 1: ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 2: ਵਾਪਸ (is[0] == is[1]);
    ਹਾਲਤ 3: ਵਾਪਸ (is[0] == is[1] && is[1] == is[2]);
    ਹਾਲਤ 4: ਵਾਪਸ (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    ਮੂਲ: ਵਾਪਸ ਝੂਠ;
    }
}

Mat& Mat::ਸੰਚਾਲਕ = (ਅਚਲ Scalar& s)
{
    CV_INSTRUMENT_REGION();

    ਜੇ (ਇਹ->empty())
        ਵਾਪਸ *ਇਹ;

    ਅਚਲ Mat* arrays[] = { ਇਹ };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elem_ਮਾਪ();
    ਅਚਲ int64* is = (ਅਚਲ int64*)&s.val[0];

    ਜੇ( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ fill_ਮੁੱਲ = 0;
        ਜੇ ( can_apply_memset(*ਇਹ, s, fill_ਮੁੱਲ) )
        {
            ਲਈ (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_ਮੁੱਲ, elsize);
            ਵਾਪਸ *ਇਹ;
        }

        ਜੇ( it.nplanes > 0 )
        {
            ਦੁੱਗਣਾ scalar[12];
            scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ(s, scalar, type(), 12);
            size_t ਖੰਡ_ਮਾਪ_2 = 12*elemSize1();

            ਲਈ( size_t j = 0; j < elsize; j += ਖੰਡ_ਮਾਪ_2 )
            {
                size_t sz = MIN(ਖੰਡ_ਮਾਪ_2, elsize - j);
                CV_Assert(sz <= ਆਕਾਰ(scalar));
                ਸਮ੍ਰਿਤੀ_ਨਕਲ( dptr + j, scalar, sz );
            }
        }

        ਲਈ( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( dptr, ਅੰਕੜੇ, elsize );
        }
    }
    ਵਾਪਸ *ਇਹ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
ਸਥਿਰ ਤਾਰਕਿਕ ipp_Mat_ਸੈੱਟ_ਮੰਜ਼ਿਲ_Mat(Mat &dst, Mat &_val, Mat &ਨਕਾਬ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ਜੇ(ਨਕਾਬ.empty())
        ਵਾਪਸ ਝੂਠ;

    ਜੇ(ਨਕਾਬ.ਡੂੰਘਾਈ() != CV_8U || ਨਕਾਬ.channels() > 1)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ(dst.channels() > 4)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (dst.ਡੂੰਘਾਈ() == CV_32F)
    {
        ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)(_val.ਕੁੱਲ()); i++)
        {
            ਦਸ਼ਮਲਵ v = (ਦਸ਼ਮਲਵ)(_val.at<ਦੁੱਗਣਾ>(i));  // cast to float
            ਜੇ (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                ਵਾਪਸ ਝੂਠ;
        }
    }

    ਜੇ(dst.dims <= 2)
    {
        Ippi_ਮਾਪ            size     = ippi_ਮਾਪ(dst.size());
        Ipp_ਅੰਕੜੇ_Type         ਅੰਕੜੇ_Type = ippi_Get_ਅੰਕੜੇ_Type(dst.ਡੂੰਘਾਈ());
        ::ipp::Iw_ਮੁੱਲ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(llwi_ਸੈੱਟ_ਨਕਾਬ, s, dst.ptr(), (ਪੂਰਨ)dst.ਕਦਮ, size, ਅੰਕੜੇ_Type, dst.channels(), ਨਕਾਬ.ptr(), (ਪੂਰਨ)ਨਕਾਬ.ਕਦਮ) >= 0;
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ Mat      *arrays[] = {&dst, ਨਕਾਬ.empty()?ਨਿਰਾ:&ਨਕਾਬ, ਨਿਰਾ};
        uchar          *ptrs[2]  = {ਨਿਰਾ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ਮਾਪ            size     = {(ਪੂਰਨ)it.size, 1};
        Ipp_ਅੰਕੜੇ_Type         ਅੰਕੜੇ_Type = ippi_Get_ਅੰਕੜੇ_Type(dst.ਡੂੰਘਾਈ());
        ::ipp::Iw_ਮੁੱਲ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(llwi_ਸੈੱਟ_ਨਕਾਬ, s, ptrs[0], 0, size, ਅੰਕੜੇ_Type, dst.channels(), ptrs[1], 0) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
        ਵਾਪਸ ਸੱਚ;
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(dst); CV_ਅਣਵਰਤਿਆ(_val); CV_ਅਣਵਰਤਿਆ(ਨਕਾਬ);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

Mat& Mat::ਸੈੱਟ_ਮੰਜ਼ਿਲ(ਨਿਵੇਸ਼_Array ਮੁੱਲ, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    CV_INSTRUMENT_REGION();

    ਜੇ( empty() )
        ਵਾਪਸ *ਇਹ;

    Mat value = ਮੁੱਲ.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();

    CV_Assert( checkScalar(value, type(), ਮੁੱਲ.ਕਿਸਮ(), ਨਿਵੇਸ਼_Array_2::MAT ));
    ਪੂਰਨ cn = channels(), mcn = ਨਕਾਬ.channels();
    CV_Assert( ਨਕਾਬ.empty() || ((ਨਕਾਬ.ਡੂੰਘਾਈ() == CV_8U || ਨਕਾਬ.ਡੂੰਘਾਈ() == CV_8S || ਨਕਾਬ.ਡੂੰਘਾਈ() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == ਨਕਾਬ.size) );

    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_Mat_ਸੈੱਟ_ਮੰਜ਼ਿਲ_Mat(*ਇਹ, value, ਨਕਾਬ), *ਇਹ)

    size_t esz = mcn > 1 ? elemSize1() : elem_ਮਾਪ();
    ਦੁਆਧਾਰੀ_Func copymask = get_Copy_ਨਕਾਬ_Func(esz);

    ਅਚਲ Mat* arrays[] = { ਇਹ, !ਨਕਾਬ.empty() ? &ਨਕਾਬ : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ totalsz = (ਪੂਰਨ)it.size*mcn;
    ਪੂਰਨ ਖੰਡ_Size0 = std::min(totalsz, (ਪੂਰਨ)((ਖੰਡ_ਮਾਪ + esz-1)/esz));
    ਖੰਡ_Size0 -= ਖੰਡ_Size0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_ਬਫਰ<uchar> _scbuf(ਖੰਡ_Size0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.ਅੰਕੜੇ(), (ਪੂਰਨ)ਆਕਾਰ(ਦੁੱਗਣਾ));
    convertAndUnrollScalar( value, type(), scbuf, ਖੰਡ_Size0/mcn );

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( ਪੂਰਨ j = 0; j < totalsz; j += ਖੰਡ_Size0 )
        {
            ਮਾਪ sz(std::min(ਖੰਡ_Size0, totalsz - j), 1);
            size_t ਖੰਡ_ਮਾਪ_2 = sz.ਚੌੜਾਈ*esz;
            ਜੇ( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.ਚੌੜਾਈ;
            }
            ਨਹੀਂਤਾਂ
                ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptrs[0], scbuf, ਖੰਡ_ਮਾਪ_2);
            ptrs[0] += ਖੰਡ_ਮਾਪ_2;
        }
    }
    ਵਾਪਸ *ਇਹ;
}


Mat& Mat::ਸੈੱਟ_Zero()
{
    CV_INSTRUMENT_REGION();

    ਜੇ( empty() )
        ਵਾਪਸ *ਇਹ;

    size_t esz = elem_ਮਾਪ();

    ਅਚਲ Mat* arrays[] = { ਇਹ, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    ਵਾਪਸ *ਇਹ;
}


#ਜੇ defined HAVE_OPENCL && !defined __APPLE__

ਸਥਿਰ ਤਾਰਕਿਕ ocl_repeat(ਨਿਵੇਸ਼_Array _src, ਪੂਰਨ ny, ਪੂਰਨ nx, OutputArray _dst)
{
    ਜੇ (ny == 1 && nx == 1)
    {
        _src.copy_ਮੰਜ਼ਿਲ(_dst);
        ਵਾਪਸ ਸੱਚ;
    }

    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)),
                         nx, ny, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI, kercn));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.ਦਲੀਲਾਂ(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_ਮਾਪ(dst));

    size_t globalsize[] = { (size_t)src.ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)src.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ repeat(ਨਿਵੇਸ਼_Array _src, ਪੂਰਨ ny, ਪੂਰਨ nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    ਮਾਪ ssize = _src.size();
    ਜੇ( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        ਵਾਪਸ;
    }
    CV_Assert( (int64_t)ssize.ਉਚਾਈ * ny <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ && (int64_t)ssize.ਚੌੜਾਈ * nx <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
    _dst.create(ssize.ਉਚਾਈ*ny, ssize.ਚੌੜਾਈ*nx, _src.type());

#ਜੇ !defined __APPLE__
    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#ਜੇ_ਅੰਤ

    Mat src = _src.getMat(), dst = _dst.getMat();
    ਮਾਪ dsize = dst.size();
    ਪੂਰਨ esz = (ਪੂਰਨ)src.elem_ਮਾਪ();
    ਪੂਰਨ x, y;
    ssize.ਚੌੜਾਈ *= esz; dsize.ਚੌੜਾਈ *= esz;

    ਲਈ( y = 0; y < ssize.ਉਚਾਈ; y++ )
    {
        ਲਈ( x = 0; x < dsize.ਚੌੜਾਈ; x += ssize.ਚੌੜਾਈ )
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( dst.ptr(y) + x, src.ptr(y), ssize.ਚੌੜਾਈ );
    }

    ਲਈ( ; y < dsize.ਉਚਾਈ; y++ )
        ਸਮ੍ਰਿਤੀ_ਨਕਲ( dst.ptr(y), dst.ptr(y - ssize.ਉਚਾਈ), dsize.ਚੌੜਾਈ );
}

Mat repeat(ਅਚਲ Mat& src, ਪੂਰਨ ny, ਪੂਰਨ nx)
{
    ਜੇ( nx == 1 && ny == 1 )
        ਵਾਪਸ src;
    Mat dst;
    repeat(src, ny, nx, dst);
    ਵਾਪਸ dst;
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
ਪੂਰਨ cv::borderInterpolate( ਪੂਰਨ p, ਪੂਰਨ len, ਪੂਰਨ borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_STATIC_ANALYSIS
    ਜੇ(p >= 0 && p < len)
#ਨਹੀਂਤਾਂ
    ਜੇ( (ਬਿਨਾਨਿਸ਼ਾਨੀ)p < (ਬਿਨਾਨਿਸ਼ਾਨੀ)len )
#ਜੇ_ਅੰਤ
        ;
    ਨਹੀਂਤਾਂ ਜੇ( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    ਨਹੀਂਤਾਂ ਜੇ( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        ਪੂਰਨ delta = borderType == BORDER_REFLECT_101;
        ਜੇ( len == 1 )
            ਵਾਪਸ 0;
        ਕਰੋ
        {
            ਜੇ( p < 0 )
                p = -p - 1 + delta;
            ਨਹੀਂਤਾਂ
                p = len - 1 - (p - len) - delta;
        }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_STATIC_ANALYSIS
        ਜਦੋਂ(p < 0 || p >= len);
#ਨਹੀਂਤਾਂ
        ਜਦੋਂ( (ਬਿਨਾਨਿਸ਼ਾਨੀ)p >= (ਬਿਨਾਨਿਸ਼ਾਨੀ)len );
#ਜੇ_ਅੰਤ
    }
    ਨਹੀਂਤਾਂ ਜੇ( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        ਜੇ( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            ਜੇ( p64 < 0 )
                p64 += len;
            p = (ਪੂਰਨ)p64;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( borderType == BORDER_CONSTANT )
        p = -1;
    ਨਹੀਂਤਾਂ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    ਵਾਪਸ p;
}

ਨਾਮਾਕਾਸ਼
{

ਖਾਲੀ copyMakeBorder_8u( ਅਚਲ uchar* src, size_t srcstep, cv::ਮਾਪ srcroi,
                        uchar* dst, size_t dststep, cv::ਮਾਪ dstroi,
                        ਪੂਰਨ ਸਿਖਰ_2, ਪੂਰਨ ਖੱਬੇ, ਪੂਰਨ cn, ਪੂਰਨ borderType )
{
    ਅਚਲ ਪੂਰਨ isz = (ਪੂਰਨ)ਆਕਾਰ(ਪੂਰਨ);
    ਪੂਰਨ i, j, k, elem_ਮਾਪ = 1;
    ਤਾਰਕਿਕ int_ਬਹੁਲਕ = ਝੂਠ;

    ਜੇ( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_ਮਾਪ = isz;
        int_ਬਹੁਲਕ = ਸੱਚ;
    }

    cv::Auto_ਬਫਰ<ਪੂਰਨ> _tab((dstroi.ਚੌੜਾਈ - srcroi.ਚੌੜਾਈ)*cn);
    ਪੂਰਨ* tab = _tab.ਅੰਕੜੇ();
    ਪੂਰਨ ਸੱਜੇ = dstroi.ਚੌੜਾਈ - srcroi.ਚੌੜਾਈ - ਖੱਬੇ;
    ਪੂਰਨ bottom = dstroi.ਉਚਾਈ - srcroi.ਉਚਾਈ - ਸਿਖਰ_2;

    ਲਈ( i = 0; i < ਖੱਬੇ; i++ )
    {
        j = cv::borderInterpolate(i - ਖੱਬੇ, srcroi.ਚੌੜਾਈ, borderType)*cn;
        ਲਈ( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    ਲਈ( i = 0; i < ਸੱਜੇ; i++ )
    {
        j = cv::borderInterpolate(srcroi.ਚੌੜਾਈ + i, srcroi.ਚੌੜਾਈ, borderType)*cn;
        ਲਈ( k = 0; k < cn; k++ )
            tab[(i+ਖੱਬੇ)*cn + k] = j + k;
    }

    srcroi.ਚੌੜਾਈ *= cn;
    dstroi.ਚੌੜਾਈ *= cn;
    ਖੱਬੇ *= cn;
    ਸੱਜੇ *= cn;

    uchar* dstInner = dst + dststep*ਸਿਖਰ_2 + ਖੱਬੇ*elem_ਮਾਪ;

    ਲਈ( i = 0; i < srcroi.ਉਚਾਈ; i++, dstInner += dststep, src += srcstep )
    {
        ਜੇ( dstInner != src )
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(dstInner, src, srcroi.ਚੌੜਾਈ*elem_ਮਾਪ);

        ਜੇ( int_ਬਹੁਲਕ )
        {
            ਅਚਲ ਪੂਰਨ* isrc = (ਪੂਰਨ*)src;
            ਪੂਰਨ* idstInner = (ਪੂਰਨ*)dstInner;
            ਲਈ( j = 0; j < ਖੱਬੇ; j++ )
                idstInner[j - ਖੱਬੇ] = isrc[tab[j]];
            ਲਈ( j = 0; j < ਸੱਜੇ; j++ )
                idstInner[j + srcroi.ਚੌੜਾਈ] = isrc[tab[j + ਖੱਬੇ]];
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਈ( j = 0; j < ਖੱਬੇ; j++ )
                dstInner[j - ਖੱਬੇ] = src[tab[j]];
            ਲਈ( j = 0; j < ਸੱਜੇ; j++ )
                dstInner[j + srcroi.ਚੌੜਾਈ] = src[tab[j + ਖੱਬੇ]];
        }
    }

    dstroi.ਚੌੜਾਈ *= elem_ਮਾਪ;

    ਲਈ( i = 0; i < ਸਿਖਰ_2; i++ )
    {
        j = cv::borderInterpolate(i - ਸਿਖਰ_2, srcroi.ਉਚਾਈ, borderType);
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + i*dststep, dst + (ਸਿਖਰ_2+j)*dststep, dstroi.ਚੌੜਾਈ);
    }

    dst += dststep*ਸਿਖਰ_2;
    ਲਈ( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.ਉਚਾਈ, srcroi.ਉਚਾਈ, borderType);
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + (i + srcroi.ਉਚਾਈ)*dststep, dst + j*dststep, dstroi.ਚੌੜਾਈ);
    }
}


ਖਾਲੀ copyMakeConstBorder_8u( ਅਚਲ uchar* src, size_t srcstep, cv::ਮਾਪ srcroi,
                             uchar* dst, size_t dststep, cv::ਮਾਪ dstroi,
                             ਪੂਰਨ ਸਿਖਰ_2, ਪੂਰਨ ਖੱਬੇ, ਪੂਰਨ cn, ਅਚਲ uchar* value )
{
    ਪੂਰਨ i, j;
    cv::Auto_ਬਫਰ<uchar> _constBuf(dstroi.ਚੌੜਾਈ*cn);
    uchar* constBuf = _constBuf.ਅੰਕੜੇ();
    ਪੂਰਨ ਸੱਜੇ = dstroi.ਚੌੜਾਈ - srcroi.ਚੌੜਾਈ - ਖੱਬੇ;
    ਪੂਰਨ bottom = dstroi.ਉਚਾਈ - srcroi.ਉਚਾਈ - ਸਿਖਰ_2;

    ਲਈ( i = 0; i < dstroi.ਚੌੜਾਈ; i++ )
    {
        ਲਈ( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.ਚੌੜਾਈ *= cn;
    dstroi.ਚੌੜਾਈ *= cn;
    ਖੱਬੇ *= cn;
    ਸੱਜੇ *= cn;

    uchar* dstInner = dst + dststep*ਸਿਖਰ_2 + ਖੱਬੇ;

    ਲਈ( i = 0; i < srcroi.ਉਚਾਈ; i++, dstInner += dststep, src += srcstep )
    {
        ਜੇ( dstInner != src )
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( dstInner, src, srcroi.ਚੌੜਾਈ );
        ਸਮ੍ਰਿਤੀ_ਨਕਲ( dstInner - ਖੱਬੇ, constBuf, ਖੱਬੇ );
        ਸਮ੍ਰਿਤੀ_ਨਕਲ( dstInner + srcroi.ਚੌੜਾਈ, constBuf, ਸੱਜੇ );
    }

    ਲਈ( i = 0; i < ਸਿਖਰ_2; i++ )
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + i * dststep, constBuf, dstroi.ਚੌੜਾਈ);

    dst += (ਸਿਖਰ_2 + srcroi.ਉਚਾਈ) * dststep;
    ਲਈ( i = 0; i < bottom; i++ )
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + i * dststep, constBuf, dstroi.ਚੌੜਾਈ);
}

}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਤਾਰਕਿਕ ocl_copyMakeBorder( ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ ਸਿਖਰ_2, ਪੂਰਨ bottom,
                                ਪੂਰਨ ਖੱਬੇ, ਪੂਰਨ ਸੱਜੇ, ਪੂਰਨ borderType, ਅਚਲ Scalar& value )
{
    ਪੂਰਨ type = _src.type(), cn = CV_MAT_CN(type), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ਤਾਰਕਿਕ isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    ਜੇ ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਅੱਖਰ * ਅਚਲ borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    ਪੂਰਨ scalarcn = cn == 3 ? 4 : cn;
    ਪੂਰਨ sctype = CV_MAKETYPE(ਡੂੰਘਾਈ, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(type), borderMap[borderType],
                                 ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), cn,
                                 ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(sctype), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat();
    ਜੇ( src.isSubmatrix() && !isolated )
    {
        ਮਾਪ ਸਾਰਾ_ਮਾਪ;
        ਬਿੰਦੂ ofs;
        src.locateROI(ਸਾਰਾ_ਮਾਪ, ofs);
        ਪੂਰਨ dtop = std::min(ofs.y, ਸਿਖਰ_2);
        ਪੂਰਨ dbottom = std::min(ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ - src.ਪੰਕਤੀਆਂ - ofs.y, bottom);
        ਪੂਰਨ dleft = std::min(ofs.x, ਖੱਬੇ);
        ਪੂਰਨ dright = std::min(ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ - src.ਖਾਨੇ_ਗਿਣਤੀ - ofs.x, ਸੱਜੇ);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ਸਿਖਰ_2 -= dtop;
        ਖੱਬੇ -= dleft;
        bottom -= dbottom;
        ਸੱਜੇ -= dright;
    }

    _dst.create(src.ਪੰਕਤੀਆਂ + ਸਿਖਰ_2 + bottom, src.ਖਾਨੇ_ਗਿਣਤੀ + ਖੱਬੇ + ਸੱਜੇ, type);
    UMat dst = _dst.getUMat();

    ਜੇ (ਸਿਖਰ_2 == 0 && ਖੱਬੇ == 0 && bottom == 0 && ਸੱਜੇ == 0)
    {
        ਜੇ(src.u != dst.u || src.ਕਦਮ != dst.ਕਦਮ)
            src.copy_ਮੰਜ਼ਿਲ(dst);
        ਵਾਪਸ ਸੱਚ;
    }

    k.ਦਲੀਲਾਂ(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           ਸਿਖਰ_2, ਖੱਬੇ, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.ਖਾਨੇ_ਗਿਣਤੀ, ((size_t)dst.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਤਾਰਕਿਕ ipp_copyMakeBorder( Mat &_src, Mat &_dst, ਪੂਰਨ ਸਿਖਰ_2, ਪੂਰਨ bottom,
                                ਪੂਰਨ ਖੱਬੇ, ਪੂਰਨ ਸੱਜੇ, ਪੂਰਨ _borderType, ਅਚਲ Scalar& value )
{
#ਜੇ defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_ਮਾਪ border_ਮਾਪ(ਖੱਬੇ, ਸਿਖਰ_2, ਸੱਜੇ, bottom);
    ::ipp::Iwi_ਮਾਪ       size(_src.ਖਾਨੇ_ਗਿਣਤੀ, _src.ਪੰਕਤੀਆਂ);
    Ipp_ਅੰਕੜੇ_Type          ਅੰਕੜੇ_Type   = ippi_Get_ਅੰਕੜੇ_Type(_src.ਡੂੰਘਾਈ());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    ਜੇ((ਪੂਰਨ)borderType == -1)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ(_src.dims > 2)
        ਵਾਪਸ ਝੂਠ;

    Rect dstRect(border_ਮਾਪ.ਖੱਬੇ, border_ਮਾਪ.ਸਿਖਰ_2,
        _dst.ਖਾਨੇ_ਗਿਣਤੀ - border_ਮਾਪ.ਸੱਜੇ - border_ਮਾਪ.ਖੱਬੇ,
        _dst.ਪੰਕਤੀਆਂ - border_ਮਾਪ.bottom - border_ਮਾਪ.ਸਿਖਰ_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->ਕਦਮ, subDst.ptr(), subDst.ਕਦਮ, size, ਅੰਕੜੇ_Type, _src.channels(), border_ਮਾਪ, borderType, &value[0]) >= 0;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(_src); CV_ਅਣਵਰਤਿਆ(_dst); CV_ਅਣਵਰਤਿਆ(ਸਿਖਰ_2); CV_ਅਣਵਰਤਿਆ(bottom); CV_ਅਣਵਰਤਿਆ(ਖੱਬੇ); CV_ਅਣਵਰਤਿਆ(ਸੱਜੇ);
    CV_ਅਣਵਰਤਿਆ(_borderType); CV_ਅਣਵਰਤਿਆ(value);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ cv::copyMakeBorder( ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ ਸਿਖਰ_2, ਪੂਰਨ bottom,
                         ਪੂਰਨ ਖੱਬੇ, ਪੂਰਨ ਸੱਜੇ, ਪੂਰਨ borderType, ਅਚਲ Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( ਸਿਖਰ_2 >= 0 && bottom >= 0 && ਖੱਬੇ >= 0 && ਸੱਜੇ >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.ਪੰਕਤੀਆਂ + ਸਿਖਰ_2 + bottom <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
    CV_Assert( (int64_t)src.ਖਾਨੇ_ਗਿਣਤੀ + ਖੱਬੇ + ਸੱਜੇ <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );

    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, ਸਿਖਰ_2, bottom, ਖੱਬੇ, ਸੱਜੇ, borderType, value))
    ਪੂਰਨ type = src.type();

    ਜੇ( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        ਮਾਪ ਸਾਰਾ_ਮਾਪ;
        ਬਿੰਦੂ ofs;
        src.locateROI(ਸਾਰਾ_ਮਾਪ, ofs);
        ਪੂਰਨ dtop = std::min(ofs.y, ਸਿਖਰ_2);
        ਪੂਰਨ dbottom = std::min(ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ - src.ਪੰਕਤੀਆਂ - ofs.y, bottom);
        ਪੂਰਨ dleft = std::min(ofs.x, ਖੱਬੇ);
        ਪੂਰਨ dright = std::min(ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ - src.ਖਾਨੇ_ਗਿਣਤੀ - ofs.x, ਸੱਜੇ);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ਸਿਖਰ_2 -= dtop;
        ਖੱਬੇ -= dleft;
        bottom -= dbottom;
        ਸੱਜੇ -= dright;
    }

    _dst.create( src.ਪੰਕਤੀਆਂ + ਸਿਖਰ_2 + bottom, src.ਖਾਨੇ_ਗਿਣਤੀ + ਖੱਬੇ + ਸੱਜੇ, type );
    Mat dst = _dst.getMat();

    ਜੇ(ਸਿਖਰ_2 == 0 && ਖੱਬੇ == 0 && bottom == 0 && ਸੱਜੇ == 0)
    {
        ਜੇ(src.ਅੰਕੜੇ != dst.ਅੰਕੜੇ || src.ਕਦਮ != dst.ਕਦਮ)
            src.copy_ਮੰਜ਼ਿਲ(dst);
        ਵਾਪਸ;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_copyMakeBorder(src, dst, ਸਿਖਰ_2, bottom, ਖੱਬੇ, ਸੱਜੇ, borderType, value))

    ਜੇ( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.ਕਦਮ, src.size(),
                           dst.ptr(), dst.ਕਦਮ, dst.size(),
                           ਸਿਖਰ_2, ਖੱਬੇ, (ਪੂਰਨ)src.elem_ਮਾਪ(), borderType );
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ cn = src.channels(), cn1 = cn;
        Auto_ਬਫਰ<ਦੁੱਗਣਾ> buf(cn);
        ਜੇ( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ(value, buf.ਅੰਕੜੇ(), CV_MAKETYPE(src.ਡੂੰਘਾਈ(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.ਕਦਮ, src.size(),
                                dst.ptr(), dst.ਕਦਮ, dst.size(),
                                ਸਿਖਰ_2, ਖੱਬੇ, (ਪੂਰਨ)src.elem_ਮਾਪ(), (uchar*)buf.ਅੰਕੜੇ() );
    }
}
/* End of file. */
