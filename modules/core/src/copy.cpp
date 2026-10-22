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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

ನಾಮಾಕಾಶ cv
{

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(ಸ್ಥಿರಾಂಕ Scalar& s, T * ಸ್ಥಿರಾಂಕ buf, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ unroll_ಗಮ್ಯ)
{
    ಪೂರ್ಣಾಂಕ i = 0;
    ಗಾಗಿ(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    ಗಾಗಿ(; i < unroll_ಗಮ್ಯ; i++)
        buf[i] = buf[i-cn];
}

ಶೂನ್ಯ scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ Scalar& s, ಶೂನ್ಯ* _buf, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ unroll_ಗಮ್ಯ)
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    ಆಯ್ಕೆ(ಆಳ)
    {
    ಸ್ಥಿತಿ CV_8U:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (uchar*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (schar*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_Bool:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (ತಾರ್ಕಿಕ*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (ushort*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (ಗಿಡ್ಡ*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16F:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (hfloat*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (bfloat*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8F_E4M3FN:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (fp8_t*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8F_E4M3FNUZ:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (fp8a_t*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (ಚಿಹ್ನೆರಹಿತ*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (ಪೂರ್ಣಾಂಕ*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (ದಶಮಾಂಶ*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (uint64_t*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (int64_t*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64F:
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ_2(s, (ದ್ವಿಗುಣ*)_buf, cn, unroll_ಗಮ್ಯ);
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

ಶೂನ್ಯ convertAndUnrollScalar( ಸ್ಥಿರಾಂಕ Mat& sc, ಪೂರ್ಣಾಂಕ buftype, uchar* scbuf, size_t blocksize )
{
    ಪೂರ್ಣಾಂಕ scn = (ಪೂರ್ಣಾಂಕ)sc.ಒಟ್ಟು(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_ಗಾತ್ರ(buftype);
    ದ್ವಿಮಾನ_Func cvtFn = getConvertFunc(sc.ಆಳ(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, ಗಾತ್ರ_2(std::min(cn, scn), 1), 0);
    // unroll the scalar
    ಆದರೆ( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        ಗಾಗಿ( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    ಗಾಗಿ( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
copy_ಮುಖವಾಡ(ಸ್ಥಿರಾಂಕ uchar* _src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, uchar* _dst, size_t dstep, ಗಾತ್ರ_2 size)
{
    ಗಾಗಿ( ; size.ಎತ್ತರ--; ಮುಖವಾಡ += mstep, _src += sstep, _dst += dstep )
    {
        ಸ್ಥಿರಾಂಕ T* src = (ಸ್ಥಿರಾಂಕ T*)_src;
        T* dst = (T*)_dst;
        ಪೂರ್ಣಾಂಕ x = 0;
         #ಆದರೆ CV_ENABLE_UNROLLED
        ಗಾಗಿ( ; x <= size.ಅಗಲ - 4; x += 4 )
        {
            ಆದರೆ( ಮುಖವಾಡ[x] )
                dst[x] = src[x];
            ಆದರೆ( ಮುಖವಾಡ[x+1] )
                dst[x+1] = src[x+1];
            ಆದರೆ( ಮುಖವಾಡ[x+2] )
                dst[x+2] = src[x+2];
            ಆದರೆ( ಮುಖವಾಡ[x+3] )
                dst[x+3] = src[x+3];
        }
        #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; x < size.ಅಗಲ; x++ )
            ಆದರೆ( ಮುಖವಾಡ[x] )
                dst[x] = src[x];
    }
}

ಮಾದರಿ<> ಶೂನ್ಯ
copy_ಮುಖವಾಡ<uchar>(ಸ್ಥಿರಾಂಕ uchar* _src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, uchar* _dst, size_t dstep, ಗಾತ್ರ_2 size)
{
    CV_IPP_ಸತತ_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (ಪೂರ್ಣಾಂಕ)sstep, _dst, (ಪೂರ್ಣಾಂಕ)dstep, ippi_ಗಾತ್ರ(size), ಮುಖವಾಡ, (ಪೂರ್ಣಾಂಕ)mstep) >= 0)

    ಗಾಗಿ( ; size.ಎತ್ತರ--; ಮುಖವಾಡ += mstep, _src += sstep, _dst += dstep )
    {
        ಸ್ಥಿರಾಂಕ uchar* src = (ಸ್ಥಿರಾಂಕ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ಪೂರ್ಣಾಂಕ x = 0;
        #ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            ಗಾಗಿ( ; x <= size.ಅಗಲ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_ತುಂಬು(src  + x),
                        v_dst   = vx_ತುಂಬು(dst  + x),
                        v_nmask = v_eq(vx_ತುಂಬು(ಮುಖವಾಡ + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; x < size.ಅಗಲ; x++ )
            ಆದರೆ( ಮುಖವಾಡ[x] )
                dst[x] = src[x];
    }
}

ಮಾದರಿ<> ಶೂನ್ಯ
copy_ಮುಖವಾಡ<Vec3b>(ಸ್ಥಿರಾಂಕ uchar* _src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, uchar* _dst, size_t dstep, ಗಾತ್ರ_2 size)
{
    ಗಾಗಿ( ; size.ಎತ್ತರ--; ಮುಖವಾಡ += mstep, _src += sstep, _dst += dstep )
    {
        ಸ್ಥಿರಾಂಕ uchar* src = (ಸ್ಥಿರಾಂಕ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ಪೂರ್ಣಾಂಕ x = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಗಾಗಿ( ; x <= size.ಅಗಲ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_ತುಂಬು(ಮುಖವಾಡ + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_ತುಂಬು_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_ತುಂಬು_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; x < size.ಅಗಲ; x++ )
            ಆದರೆ( ಮುಖವಾಡ[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

ಮಾದರಿ<> ಶೂನ್ಯ
copy_ಮುಖವಾಡ<ushort>(ಸ್ಥಿರಾಂಕ uchar* _src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, uchar* _dst, size_t dstep, ಗಾತ್ರ_2 size)
{
    CV_IPP_ಸತತ_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ಸ್ಥಿರಾಂಕ Ipp16u *)_src, (ಪೂರ್ಣಾಂಕ)sstep, (Ipp16u *)_dst, (ಪೂರ್ಣಾಂಕ)dstep, ippi_ಗಾತ್ರ(size), ಮುಖವಾಡ, (ಪೂರ್ಣಾಂಕ)mstep) >= 0)

    ಗಾಗಿ( ; size.ಎತ್ತರ--; ಮುಖವಾಡ += mstep, _src += sstep, _dst += dstep )
    {
        ಸ್ಥಿರಾಂಕ ushort* src = (ಸ್ಥಿರಾಂಕ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ಪೂರ್ಣಾಂಕ x = 0;
        #ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            ಗಾಗಿ( ; x <= size.ಅಗಲ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_ತುಂಬು(src + x), v_src2 = vx_ತುಂಬು(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_ತುಂಬು(dst + x), v_dst2 = vx_ತುಂಬು(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_ತುಂಬು(ಮುಖವಾಡ + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; x < size.ಅಗಲ; x++ )
            ಆದರೆ( ಮುಖವಾಡ[x] )
                dst[x] = src[x];
    }
}

ಮಾದರಿ<> ಶೂನ್ಯ
copy_ಮುಖವಾಡ<Vec3s>(ಸ್ಥಿರಾಂಕ uchar* _src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, uchar* _dst, size_t dstep, ಗಾತ್ರ_2 size)
{
    ಗಾಗಿ( ; size.ಎತ್ತರ--; ಮುಖವಾಡ += mstep, _src += sstep, _dst += dstep )
    {
        ಸ್ಥಿರಾಂಕ ushort* src = (ಸ್ಥಿರಾಂಕ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ಪೂರ್ಣಾಂಕ x = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಗಾಗಿ( ; x <= size.ಅಗಲ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_ತುಂಬು(ಮುಖವಾಡ + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_ತುಂಬು_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_ತುಂಬು_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_ತುಂಬು_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_ತುಂಬು_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

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
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; x < size.ಅಗಲ; x++ )
            ಆದರೆ( ಮುಖವಾಡ[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

ಮಾದರಿ<> ಶೂನ್ಯ
copy_ಮುಖವಾಡ<ಪೂರ್ಣಾಂಕ>(ಸ್ಥಿರಾಂಕ uchar* _src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, uchar* _dst, size_t dstep, ಗಾತ್ರ_2 size)
{
    ಗಾಗಿ( ; size.ಎತ್ತರ--; ಮುಖವಾಡ += mstep, _src += sstep, _dst += dstep )
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src = (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)_src;
        ಪೂರ್ಣಾಂಕ* dst = (ಪೂರ್ಣಾಂಕ*)_dst;
        ಪೂರ್ಣಾಂಕ x = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಗಾಗಿ (; x <= size.ಅಗಲ - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_ತುಂಬು(src + x), v_dst0 = vx_ತುಂಬು(dst + x);
            v_int32 v_src1 = vx_ತುಂಬು(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_ತುಂಬು(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_ತುಂಬು(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_ತುಂಬು(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_ತುಂಬು(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_ತುಂಬು(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_ತುಂಬು(ಮುಖವಾಡ + x), vx_setzero_u8());
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
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; x < size.ಅಗಲ; x++)
            ಆದರೆ ( ಮುಖವಾಡ[x] )
                dst[x] = src[x];
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
copy_ಮುಖವಾಡ_Generic(ಸ್ಥಿರಾಂಕ uchar* _src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, uchar* _dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    ಗಾಗಿ( ; size.ಎತ್ತರ--; ಮುಖವಾಡ += mstep, _src += sstep, _dst += dstep )
    {
        ಸ್ಥಿರಾಂಕ uchar* src = _src;
        uchar* dst = _dst;
        ಪೂರ್ಣಾಂಕ x = 0;
        ಗಾಗಿ( ; x < size.ಅಗಲ; x++, src += esz, dst += esz )
        {
            ಆದರೆ( !ಮುಖವಾಡ[x] )
                ಮುಂದುವರಿ;
            ಗಾಗಿ( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#ವ್ಯಾಖ್ಯಾನಿಸು DEF_COPY_ಮುಖವಾಡ(suffix, type) \
ಸ್ಥಿರ ಶೂನ್ಯ copy_ಮುಖವಾಡ_2##suffix(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, \
                             uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*) \
{ \
    copy_ಮುಖವಾಡ<type>(src, sstep, ಮುಖವಾಡ, mstep, dst, dstep, size); \
}

#ಆದರೆ defined HAVE_IPP
#ವ್ಯಾಖ್ಯಾನಿಸು DEF_COPY_ಮುಖವಾಡ_F(suffix, type, ippfavor, ipptype) \
ಸ್ಥಿರ ಶೂನ್ಯ copy_ಮುಖವಾಡ_2##suffix(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, \
                             uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*) \
{ \
    CV_IPP_ಸತತ_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ಸ್ಥಿರಾಂಕ ipptype *)src, (ಪೂರ್ಣಾಂಕ)sstep, (ipptype *)dst, (ಪೂರ್ಣಾಂಕ)dstep, ippi_ಗಾತ್ರ(size), (ಸ್ಥಿರಾಂಕ Ipp8u *)ಮುಖವಾಡ, (ಪೂರ್ಣಾಂಕ)mstep) >= 0)\
    copy_ಮುಖವಾಡ<type>(src, sstep, ಮುಖವಾಡ, mstep, dst, dstep, size); \
}
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು DEF_COPY_ಮುಖವಾಡ_F(suffix, type, ippfavor, ipptype) \
ಸ್ಥಿರ ಶೂನ್ಯ copy_ಮುಖವಾಡ_2##suffix(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t mstep, \
                             uchar* dst, size_t dstep, ಗಾತ್ರ_2 size, ಶೂನ್ಯ*) \
{ \
    copy_ಮುಖವಾಡ<type>(src, sstep, ಮುಖವಾಡ, mstep, dst, dstep, size); \
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_ಮುಖವಾಡ(32sC3, Vec3i)
DEF_COPY_ಮುಖವಾಡ(8uC3, Vec3b)
#ಇಲ್ಲದಿದ್ದರೆ
DEF_COPY_ಮುಖವಾಡ_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_ಮುಖವಾಡ_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#ಆದರೆ_ಅಂತ್ಯ
DEF_COPY_ಮುಖವಾಡ(8u, uchar)
DEF_COPY_ಮುಖವಾಡ(16u, ushort)
DEF_COPY_ಮುಖವಾಡ_F(32s, ಪೂರ್ಣಾಂಕ, 32s_C1MR, Ipp32s)
DEF_COPY_ಮುಖವಾಡ_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_ಮುಖವಾಡ(32sC2, Vec2i)
DEF_COPY_ಮುಖವಾಡ_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_ಮುಖವಾಡ(32sC6, Vec6i)
DEF_COPY_ಮುಖವಾಡ(32sC8, Vec8i)

ದ್ವಿಮಾನ_Func copy_ಮುಖವಾಡ_Tab[] =
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

ದ್ವಿಮಾನ_Func get_Copy_ಮುಖವಾಡ_Func(size_t esz)
{
    ಹಿಂತಿರುಗಿ esz <= 32 && copy_ಮುಖವಾಡ_Tab[esz] ? copy_ಮುಖವಾಡ_Tab[esz] : copy_ಮುಖವಾಡ_Generic;
}

/* dst = src */
ಶೂನ್ಯ Mat::copy_ಗಮ್ಯ( OutputArray _dst ) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಆದರೆ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ಇದು);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ಣಾಂಕ stype = type();
    ಪೂರ್ಣಾಂಕ dtype = _dst.type();
    ಆದರೆ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_ಗಮ್ಯ( _dst, dtype );
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( dims == 0 && empty() )
    {
        _dst.release();
        ಶೂನ್ಯ* obj = _dst.getObj();
        ಆದರೆ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ಹಿಂತಿರುಗಿ;
    }

    ತಾರ್ಕಿಕ allowTransposed = dims == 1 ||
        _dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ ||
        (_dst.fixed_ಗಾತ್ರ() && _dst.dims() == 1);
    ಆದರೆ( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        ಆದರೆ (empty())
            ಹಿಂತಿರುಗಿ;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != ಖಾಲಿ);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_ಗಾತ್ರ(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        ಗಾಗಿ( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        ಪೂರ್ಣಾಂಕ lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, ದತ್ತಾಂಶ, std::max(dims, 1), sz, dstofs, dst.ಹೆಜ್ಜೆ.p, ಹೆಜ್ಜೆ.p);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        ಆದರೆ( ದತ್ತಾಂಶ == dst.ದತ್ತಾಂಶ )
            ಹಿಂತಿರುಗಿ;

        ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು > 0 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ > 0 )
        {
            Mat src = *ಇದು;
            ಗಾತ್ರ_2 sz = getContinuousSize2D(src, dst, (ಪೂರ್ಣಾಂಕ)elem_ಗಾತ್ರ());
            CV_CheckGE(sz.ಅಗಲ, 0, "");

            ಸ್ಥಿರಾಂಕ uchar* sptr = src.ದತ್ತಾಂಶ;
            uchar* dptr = dst.ದತ್ತಾಂಶ;

#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 201700
            CV_IPP_ಸತತ_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (ಪೂರ್ಣಾಂಕ)src.ಹೆಜ್ಜೆ, dptr, (ಪೂರ್ಣಾಂಕ)dst.ಹೆಜ್ಜೆ, ippi_ಗಾತ್ರ_L(sz.ಅಗಲ, sz.ಎತ್ತರ)) >= 0)
#ಆದರೆ_ಅಂತ್ಯ

            ಗಾಗಿ (; sz.ಎತ್ತರ--; sptr += src.ಹೆಜ್ಜೆ, dptr += dst.ಹೆಜ್ಜೆ)
                ಸ್ಮರಣೆ_ನಕಲು(dptr, sptr, sz.ಅಗಲ);
        }
        ಹಿಂತಿರುಗಿ;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    ಆದರೆ( ದತ್ತಾಂಶ == dst.ದತ್ತಾಂಶ )
        ಹಿಂತಿರುಗಿ;

    ಆದರೆ( ಒಟ್ಟು() != 0 )
    {
        ಸ್ಥಿರಾಂಕ Mat* arrays[] = { ಇದು, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_ಗಾತ್ರ();

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            ಸ್ಮರಣೆ_ನಕಲು(ptrs[1], ptrs[0], sz);
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_copy_ಗಮ್ಯ(ಸ್ಥಿರಾಂಕ Mat &src, Mat &dst, ಸ್ಥಿರಾಂಕ Mat &ಮುಖವಾಡ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಆದರೆ(ಮುಖವಾಡ.channels() > 1 || ಮುಖವಾಡ.ಆಳ() != CV_8U)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (src.dims <= 2)
    {
        Ippi_ಗಾತ್ರ size = ippi_ಗಾತ್ರ(src.size());
        ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(llwi_Copy_ಮುಖವಾಡ, src.ptr(), (ಪೂರ್ಣಾಂಕ)src.ಹೆಜ್ಜೆ, dst.ptr(), (ಪೂರ್ಣಾಂಕ)dst.ಹೆಜ್ಜೆ, size, (ಪೂರ್ಣಾಂಕ)src.elemSize1(), src.channels(), ಮುಖವಾಡ.ptr(), (ಪೂರ್ಣಾಂಕ)ಮುಖವಾಡ.ಹೆಜ್ಜೆ) >= 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ Mat      *arrays[] = {&src, &dst, &ಮುಖವಾಡ, ಖಾಲಿ};
        uchar          *ptrs[3]  = {ಖಾಲಿ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ಗಾತ್ರ size = ippi_ಗಾತ್ರ(it.size, 1);

        ಗಾಗಿ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ಮುಖವಾಡ, ptrs[0], 0, ptrs[1], 0, size, (ಪೂರ್ಣಾಂಕ)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(dst); CV_ಬಳಸದ(ಮುಖವಾಡ);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ Mat::copy_ಗಮ್ಯ( OutputArray _dst, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2 ) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();
    ಆದರೆ( !ಮುಖವಾಡ.ದತ್ತಾಂಶ )
    {
        copy_ಗಮ್ಯ(_dst);
        ಹಿಂತಿರುಗಿ;
    }

    ಪೂರ್ಣಾಂಕ cn = channels(), mcn = ಮುಖವಾಡ.channels();
    CV_Assert( (ಮುಖವಾಡ.ಆಳ() == CV_8U || ಮುಖವಾಡ.ಆಳ() == CV_8S || ಮುಖವಾಡ.ಆಳ() == CV_Bool) && (mcn == 1 || mcn == cn) );
    ತಾರ್ಕಿಕ color_ಮುಖವಾಡ = mcn > 1;
    ಆದರೆ( dims <= 2 )
    {
        CV_Assert( size() == ಮುಖವಾಡ.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        ಆದರೆ (dst.ದತ್ತಾಂಶ != dst0.ದತ್ತಾಂಶ) // re-allocation happened
        {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#ಆದರೆ_ಅಂತ್ಯ
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_ಸತತ_FAST(ipp_copy_ಗಮ್ಯ(*ಇದು, dst, ಮುಖವಾಡ))
    ಆದರೆ ( ಇದು->dims <= 2 ) {
        ಆದರೆ ( ಇದು->size() == dst.size() && ಇದು->size() == dst.size() ) {
            CALL_HAL(copy_ಗಮ್ಯ_ಮುಖವಾಡ, cv_hal_copy_ಗಮ್ಯ_Masked, ಇದು->ದತ್ತಾಂಶ, ಇದು->ಹೆಜ್ಜೆ, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ, ಇದು->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಇದು->ಅಡ್ಡಸಾಲುಗಳು, ಇದು->type(), ಮುಖವಾಡ.ದತ್ತಾಂಶ, ಮುಖವಾಡ.ಹೆಜ್ಜೆ, ಮುಖವಾಡ.type());
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( ಇದು->isContinuous() && dst.isContinuous() && ಮುಖವಾಡ.isContinuous() )
    {
        size_t sz = ಇದು->ಒಟ್ಟು();
        ಆದರೆ (sz < ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ) {
            CALL_HAL(copy_ಗಮ್ಯ_ಮುಖವಾಡ, cv_hal_copy_ಗಮ್ಯ_Masked, ಇದು->ದತ್ತಾಂಶ, 0, dst.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)sz, 1, ಇದು->type(), ಮುಖವಾಡ.ದತ್ತಾಂಶ, 0, ಮುಖವಾಡ.type());
        }
    }

    size_t esz = color_ಮುಖವಾಡ ? elemSize1() : elem_ಗಾತ್ರ();
    ದ್ವಿಮಾನ_Func copymask = get_Copy_ಮುಖವಾಡ_Func(esz);

    ಆದರೆ( dims <= 2 )
    {
        Mat src = *ಇದು;
        ಗಾತ್ರ_2 sz = getContinuousSize2D(src, dst, ಮುಖವಾಡ, mcn);
        copymask(src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, ಮುಖವಾಡ.ದತ್ತಾಂಶ, ಮುಖವಾಡ.ಹೆಜ್ಜೆ, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ, sz, &esz);
        ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = { ಇದು, &dst, &ಮುಖವಾಡ, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ಗಾತ್ರ_2 sz((ಪೂರ್ಣಾಂಕ)(it.size*mcn), 1);

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
ಶೂನ್ಯ Mat::copyAt( OutputArray _dst ) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_ಗಮ್ಯ(_dst);
}
ಶೂನ್ಯ Mat::copyAt( OutputArray _dst, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2 ) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_ಗಮ್ಯ(_dst, ಮುಖವಾಡ_2);
}


ಸ್ಥಿರ ತಾರ್ಕಿಕ can_apply_memset(ಸ್ಥಿರಾಂಕ Mat &mat, ಸ್ಥಿರಾಂಕ Scalar &s, ಪೂರ್ಣಾಂಕ &fill_ಮೌಲ್ಯ)
{
    // check if depth is 1 byte.
    ಆಯ್ಕೆ (mat.ಆಳ())
    {
    ಸ್ಥಿತಿ CV_8U: fill_ಮೌಲ್ಯ = saturate_cast<uchar>( s.val[0] ); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S: fill_ಮೌಲ್ಯ = saturate_cast<schar>( s.val[0] ); ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ: ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    // check if all element is same.
    ಸ್ಥಿರಾಂಕ int64* is = (ಸ್ಥಿರಾಂಕ int64*)&s.val[0];
    ಆಯ್ಕೆ (mat.channels())
    {
    ಸ್ಥಿತಿ 1: ಹಿಂತಿರುಗಿ ನಿಜ;
    ಸ್ಥಿತಿ 2: ಹಿಂತಿರುಗಿ (is[0] == is[1]);
    ಸ್ಥಿತಿ 3: ಹಿಂತಿರುಗಿ (is[0] == is[1] && is[1] == is[2]);
    ಸ್ಥಿತಿ 4: ಹಿಂತಿರುಗಿ (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    ಪೂರ್ವನಿಯೋಜಿತ: ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
}

Mat& Mat::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Scalar& s)
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (ಇದು->empty())
        ಹಿಂತಿರುಗಿ *ಇದು;

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = { ಇದು };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elem_ಗಾತ್ರ();
    ಸ್ಥಿರಾಂಕ int64* is = (ಸ್ಥಿರಾಂಕ int64*)&s.val[0];

    ಆದರೆ( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ fill_ಮೌಲ್ಯ = 0;
        ಆದರೆ ( can_apply_memset(*ಇದು, s, fill_ಮೌಲ್ಯ) )
        {
            ಗಾಗಿ (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_ಮೌಲ್ಯ, elsize);
            ಹಿಂತಿರುಗಿ *ಇದು;
        }

        ಆದರೆ( it.nplanes > 0 )
        {
            ದ್ವಿಗುಣ scalar[12];
            scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ(s, scalar, type(), 12);
            size_t ಖಂಡ_ಗಾತ್ರ_2 = 12*elemSize1();

            ಗಾಗಿ( size_t j = 0; j < elsize; j += ಖಂಡ_ಗಾತ್ರ_2 )
            {
                size_t sz = MIN(ಖಂಡ_ಗಾತ್ರ_2, elsize - j);
                CV_Assert(sz <= ಗಾತ್ರ(scalar));
                ಸ್ಮರಣೆ_ನಕಲು( dptr + j, scalar, sz );
            }
        }

        ಗಾಗಿ( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            ಸ್ಮರಣೆ_ನಕಲು( dptr, ದತ್ತಾಂಶ, elsize );
        }
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_Mat_ಗಣ_ಗಮ್ಯ_Mat(Mat &dst, Mat &_val, Mat &ಮುಖವಾಡ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಆದರೆ(ಮುಖವಾಡ.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ(ಮುಖವಾಡ.ಆಳ() != CV_8U || ಮುಖವಾಡ.channels() > 1)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ(dst.channels() > 4)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (dst.ಆಳ() == CV_32F)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)(_val.ಒಟ್ಟು()); i++)
        {
            ದಶಮಾಂಶ v = (ದಶಮಾಂಶ)(_val.at<ದ್ವಿಗುಣ>(i));  // cast to float
            ಆದರೆ (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
    }

    ಆದರೆ(dst.dims <= 2)
    {
        Ippi_ಗಾತ್ರ            size     = ippi_ಗಾತ್ರ(dst.size());
        Ipp_ದತ್ತಾಂಶ_Type         ದತ್ತಾಂಶ_Type = ippi_Get_ದತ್ತಾಂಶ_Type(dst.ಆಳ());
        ::ipp::Iw_ಮೌಲ್ಯ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(llwi_ಗಣ_ಮುಖವಾಡ, s, dst.ptr(), (ಪೂರ್ಣಾಂಕ)dst.ಹೆಜ್ಜೆ, size, ದತ್ತಾಂಶ_Type, dst.channels(), ಮುಖವಾಡ.ptr(), (ಪೂರ್ಣಾಂಕ)ಮುಖವಾಡ.ಹೆಜ್ಜೆ) >= 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ Mat      *arrays[] = {&dst, ಮುಖವಾಡ.empty()?ಖಾಲಿ:&ಮುಖವಾಡ, ಖಾಲಿ};
        uchar          *ptrs[2]  = {ಖಾಲಿ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ಗಾತ್ರ            size     = {(ಪೂರ್ಣಾಂಕ)it.size, 1};
        Ipp_ದತ್ತಾಂಶ_Type         ದತ್ತಾಂಶ_Type = ippi_Get_ದತ್ತಾಂಶ_Type(dst.ಆಳ());
        ::ipp::Iw_ಮೌಲ್ಯ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(llwi_ಗಣ_ಮುಖವಾಡ, s, ptrs[0], 0, size, ದತ್ತಾಂಶ_Type, dst.channels(), ptrs[1], 0) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(dst); CV_ಬಳಸದ(_val); CV_ಬಳಸದ(ಮುಖವಾಡ);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ

Mat& Mat::ಗಣ_ಗಮ್ಯ(ಒಳಹರಿವು_Array ಮೌಲ್ಯ, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( empty() )
        ಹಿಂತಿರುಗಿ *ಇದು;

    Mat value = ಮೌಲ್ಯ.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();

    CV_Assert( checkScalar(value, type(), ಮೌಲ್ಯ.ಬಗೆ(), ಒಳಹರಿವು_Array_2::MAT ));
    ಪೂರ್ಣಾಂಕ cn = channels(), mcn = ಮುಖವಾಡ.channels();
    CV_Assert( ಮುಖವಾಡ.empty() || ((ಮುಖವಾಡ.ಆಳ() == CV_8U || ಮುಖವಾಡ.ಆಳ() == CV_8S || ಮುಖವಾಡ.ಆಳ() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == ಮುಖವಾಡ.size) );

    CV_IPP_ಸತತ_FAST(ipp_Mat_ಗಣ_ಗಮ್ಯ_Mat(*ಇದು, value, ಮುಖವಾಡ), *ಇದು)

    size_t esz = mcn > 1 ? elemSize1() : elem_ಗಾತ್ರ();
    ದ್ವಿಮಾನ_Func copymask = get_Copy_ಮುಖವಾಡ_Func(esz);

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = { ಇದು, !ಮುಖವಾಡ.empty() ? &ಮುಖವಾಡ : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ totalsz = (ಪೂರ್ಣಾಂಕ)it.size*mcn;
    ಪೂರ್ಣಾಂಕ ಖಂಡ_Size0 = std::min(totalsz, (ಪೂರ್ಣಾಂಕ)((ಖಂಡ_ಗಾತ್ರ + esz-1)/esz));
    ಖಂಡ_Size0 -= ಖಂಡ_Size0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_ಬಫರ್<uchar> _scbuf(ಖಂಡ_Size0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.ದತ್ತಾಂಶ(), (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ದ್ವಿಗುಣ));
    convertAndUnrollScalar( value, type(), scbuf, ಖಂಡ_Size0/mcn );

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < totalsz; j += ಖಂಡ_Size0 )
        {
            ಗಾತ್ರ_2 sz(std::min(ಖಂಡ_Size0, totalsz - j), 1);
            size_t ಖಂಡ_ಗಾತ್ರ_2 = sz.ಅಗಲ*esz;
            ಆದರೆ( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.ಅಗಲ;
            }
            ಇಲ್ಲದಿದ್ದರೆ
                ಸ್ಮರಣೆ_ನಕಲು(ptrs[0], scbuf, ಖಂಡ_ಗಾತ್ರ_2);
            ptrs[0] += ಖಂಡ_ಗಾತ್ರ_2;
        }
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}


Mat& Mat::ಗಣ_Zero()
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( empty() )
        ಹಿಂತಿರುಗಿ *ಇದು;

    size_t esz = elem_ಗಾತ್ರ();

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = { ಇದು, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    ಹಿಂತಿರುಗಿ *ಇದು;
}


#ಆದರೆ defined HAVE_OPENCL && !defined __APPLE__

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_repeat(ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಂಕ ny, ಪೂರ್ಣಾಂಕ nx, OutputArray _dst)
{
    ಆದರೆ (ny == 1 && nx == 1)
    {
        _src.copy_ಗಮ್ಯ(_dst);
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memop_Type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)),
                         nx, ny, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI, kercn));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.ವಾದಗಳು(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dst));

    size_t globalsize[] = { (size_t)src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)src.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ repeat(ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಂಕ ny, ಪೂರ್ಣಾಂಕ nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    ಗಾತ್ರ_2 ssize = _src.size();
    ಆದರೆ( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        ಹಿಂತಿರುಗಿ;
    }
    CV_Assert( (int64_t)ssize.ಎತ್ತರ * ny <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ && (int64_t)ssize.ಅಗಲ * nx <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
    _dst.create(ssize.ಎತ್ತರ*ny, ssize.ಅಗಲ*nx, _src.type());

#ಆದರೆ !defined __APPLE__
    CV_OCL_ಸತತ(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#ಆದರೆ_ಅಂತ್ಯ

    Mat src = _src.getMat(), dst = _dst.getMat();
    ಗಾತ್ರ_2 dsize = dst.size();
    ಪೂರ್ಣಾಂಕ esz = (ಪೂರ್ಣಾಂಕ)src.elem_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ x, y;
    ssize.ಅಗಲ *= esz; dsize.ಅಗಲ *= esz;

    ಗಾಗಿ( y = 0; y < ssize.ಎತ್ತರ; y++ )
    {
        ಗಾಗಿ( x = 0; x < dsize.ಅಗಲ; x += ssize.ಅಗಲ )
            ಸ್ಮರಣೆ_ನಕಲು( dst.ptr(y) + x, src.ptr(y), ssize.ಅಗಲ );
    }

    ಗಾಗಿ( ; y < dsize.ಎತ್ತರ; y++ )
        ಸ್ಮರಣೆ_ನಕಲು( dst.ptr(y), dst.ptr(y - ssize.ಎತ್ತರ), dsize.ಅಗಲ );
}

Mat repeat(ಸ್ಥಿರಾಂಕ Mat& src, ಪೂರ್ಣಾಂಕ ny, ಪೂರ್ಣಾಂಕ nx)
{
    ಆದರೆ( nx == 1 && ny == 1 )
        ಹಿಂತಿರುಗಿ src;
    Mat dst;
    repeat(src, ny, nx, dst);
    ಹಿಂತಿರುಗಿ dst;
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
ಪೂರ್ಣಾಂಕ cv::borderInterpolate( ಪೂರ್ಣಾಂಕ p, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_STATIC_ANALYSIS
    ಆದರೆ(p >= 0 && p < len)
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ( (ಚಿಹ್ನೆರಹಿತ)p < (ಚಿಹ್ನೆರಹಿತ)len )
#ಆದರೆ_ಅಂತ್ಯ
        ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        ಪೂರ್ಣಾಂಕ delta = borderType == BORDER_REFLECT_101;
        ಆದರೆ( len == 1 )
            ಹಿಂತಿರುಗಿ 0;
        ಮಾಡು
        {
            ಆದರೆ( p < 0 )
                p = -p - 1 + delta;
            ಇಲ್ಲದಿದ್ದರೆ
                p = len - 1 - (p - len) - delta;
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_STATIC_ANALYSIS
        ಆಗ(p < 0 || p >= len);
#ಇಲ್ಲದಿದ್ದರೆ
        ಆಗ( (ಚಿಹ್ನೆರಹಿತ)p >= (ಚಿಹ್ನೆರಹಿತ)len );
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        ಆದರೆ( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            ಆದರೆ( p64 < 0 )
                p64 += len;
            p = (ಪೂರ್ಣಾಂಕ)p64;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( borderType == BORDER_CONSTANT )
        p = -1;
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    ಹಿಂತಿರುಗಿ p;
}

ನಾಮಾಕಾಶ
{

ಶೂನ್ಯ copyMakeBorder_8u( ಸ್ಥಿರಾಂಕ uchar* src, size_t srcstep, cv::ಗಾತ್ರ_2 srcroi,
                        uchar* dst, size_t dststep, cv::ಗಾತ್ರ_2 dstroi,
                        ಪೂರ್ಣಾಂಕ ತುದಿ_2, ಪೂರ್ಣಾಂಕ ಎಡ, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ borderType )
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ isz = (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);
    ಪೂರ್ಣಾಂಕ i, j, k, elem_ಗಾತ್ರ = 1;
    ತಾರ್ಕಿಕ int_ಬಹುಲಕ = ಸುಳ್ಳು;

    ಆದರೆ( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_ಗಾತ್ರ = isz;
        int_ಬಹುಲಕ = ನಿಜ;
    }

    cv::Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> _tab((dstroi.ಅಗಲ - srcroi.ಅಗಲ)*cn);
    ಪೂರ್ಣಾಂಕ* tab = _tab.ದತ್ತಾಂಶ();
    ಪೂರ್ಣಾಂಕ ಬಲ = dstroi.ಅಗಲ - srcroi.ಅಗಲ - ಎಡ;
    ಪೂರ್ಣಾಂಕ bottom = dstroi.ಎತ್ತರ - srcroi.ಎತ್ತರ - ತುದಿ_2;

    ಗಾಗಿ( i = 0; i < ಎಡ; i++ )
    {
        j = cv::borderInterpolate(i - ಎಡ, srcroi.ಅಗಲ, borderType)*cn;
        ಗಾಗಿ( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    ಗಾಗಿ( i = 0; i < ಬಲ; i++ )
    {
        j = cv::borderInterpolate(srcroi.ಅಗಲ + i, srcroi.ಅಗಲ, borderType)*cn;
        ಗಾಗಿ( k = 0; k < cn; k++ )
            tab[(i+ಎಡ)*cn + k] = j + k;
    }

    srcroi.ಅಗಲ *= cn;
    dstroi.ಅಗಲ *= cn;
    ಎಡ *= cn;
    ಬಲ *= cn;

    uchar* dstInner = dst + dststep*ತುದಿ_2 + ಎಡ*elem_ಗಾತ್ರ;

    ಗಾಗಿ( i = 0; i < srcroi.ಎತ್ತರ; i++, dstInner += dststep, src += srcstep )
    {
        ಆದರೆ( dstInner != src )
            ಸ್ಮರಣೆ_ನಕಲು(dstInner, src, srcroi.ಅಗಲ*elem_ಗಾತ್ರ);

        ಆದರೆ( int_ಬಹುಲಕ )
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* isrc = (ಪೂರ್ಣಾಂಕ*)src;
            ಪೂರ್ಣಾಂಕ* idstInner = (ಪೂರ್ಣಾಂಕ*)dstInner;
            ಗಾಗಿ( j = 0; j < ಎಡ; j++ )
                idstInner[j - ಎಡ] = isrc[tab[j]];
            ಗಾಗಿ( j = 0; j < ಬಲ; j++ )
                idstInner[j + srcroi.ಅಗಲ] = isrc[tab[j + ಎಡ]];
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ( j = 0; j < ಎಡ; j++ )
                dstInner[j - ಎಡ] = src[tab[j]];
            ಗಾಗಿ( j = 0; j < ಬಲ; j++ )
                dstInner[j + srcroi.ಅಗಲ] = src[tab[j + ಎಡ]];
        }
    }

    dstroi.ಅಗಲ *= elem_ಗಾತ್ರ;

    ಗಾಗಿ( i = 0; i < ತುದಿ_2; i++ )
    {
        j = cv::borderInterpolate(i - ತುದಿ_2, srcroi.ಎತ್ತರ, borderType);
        ಸ್ಮರಣೆ_ನಕಲು(dst + i*dststep, dst + (ತುದಿ_2+j)*dststep, dstroi.ಅಗಲ);
    }

    dst += dststep*ತುದಿ_2;
    ಗಾಗಿ( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.ಎತ್ತರ, srcroi.ಎತ್ತರ, borderType);
        ಸ್ಮರಣೆ_ನಕಲು(dst + (i + srcroi.ಎತ್ತರ)*dststep, dst + j*dststep, dstroi.ಅಗಲ);
    }
}


ಶೂನ್ಯ copyMakeConstBorder_8u( ಸ್ಥಿರಾಂಕ uchar* src, size_t srcstep, cv::ಗಾತ್ರ_2 srcroi,
                             uchar* dst, size_t dststep, cv::ಗಾತ್ರ_2 dstroi,
                             ಪೂರ್ಣಾಂಕ ತುದಿ_2, ಪೂರ್ಣಾಂಕ ಎಡ, ಪೂರ್ಣಾಂಕ cn, ಸ್ಥಿರಾಂಕ uchar* value )
{
    ಪೂರ್ಣಾಂಕ i, j;
    cv::Auto_ಬಫರ್<uchar> _constBuf(dstroi.ಅಗಲ*cn);
    uchar* constBuf = _constBuf.ದತ್ತಾಂಶ();
    ಪೂರ್ಣಾಂಕ ಬಲ = dstroi.ಅಗಲ - srcroi.ಅಗಲ - ಎಡ;
    ಪೂರ್ಣಾಂಕ bottom = dstroi.ಎತ್ತರ - srcroi.ಎತ್ತರ - ತುದಿ_2;

    ಗಾಗಿ( i = 0; i < dstroi.ಅಗಲ; i++ )
    {
        ಗಾಗಿ( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.ಅಗಲ *= cn;
    dstroi.ಅಗಲ *= cn;
    ಎಡ *= cn;
    ಬಲ *= cn;

    uchar* dstInner = dst + dststep*ತುದಿ_2 + ಎಡ;

    ಗಾಗಿ( i = 0; i < srcroi.ಎತ್ತರ; i++, dstInner += dststep, src += srcstep )
    {
        ಆದರೆ( dstInner != src )
            ಸ್ಮರಣೆ_ನಕಲು( dstInner, src, srcroi.ಅಗಲ );
        ಸ್ಮರಣೆ_ನಕಲು( dstInner - ಎಡ, constBuf, ಎಡ );
        ಸ್ಮರಣೆ_ನಕಲು( dstInner + srcroi.ಅಗಲ, constBuf, ಬಲ );
    }

    ಗಾಗಿ( i = 0; i < ತುದಿ_2; i++ )
        ಸ್ಮರಣೆ_ನಕಲು(dst + i * dststep, constBuf, dstroi.ಅಗಲ);

    dst += (ತುದಿ_2 + srcroi.ಎತ್ತರ) * dststep;
    ಗಾಗಿ( i = 0; i < bottom; i++ )
        ಸ್ಮರಣೆ_ನಕಲು(dst + i * dststep, constBuf, dstroi.ಅಗಲ);
}

}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_copyMakeBorder( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ ತುದಿ_2, ಪೂರ್ಣಾಂಕ bottom,
                                ಪೂರ್ಣಾಂಕ ಎಡ, ಪೂರ್ಣಾಂಕ ಬಲ, ಪೂರ್ಣಾಂಕ borderType, ಸ್ಥಿರಾಂಕ Scalar& value )
{
    ಪೂರ್ಣಾಂಕ type = _src.type(), cn = CV_MAT_CN(type), ಆಳ = CV_MAT_ಆಳ(type),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    ಆದರೆ ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    ಪೂರ್ಣಾಂಕ scalarcn = cn == 3 ? 4 : cn;
    ಪೂರ್ಣಾಂಕ sctype = CV_MAKETYPE(ಆಳ, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memop_Type_ಗಮ್ಯ_Str(type), borderMap[borderType],
                                 ocl::memop_Type_ಗಮ್ಯ_Str(ಆಳ), cn,
                                 ocl::memop_Type_ಗಮ್ಯ_Str(sctype), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat();
    ಆದರೆ( src.isSubmatrix() && !isolated )
    {
        ಗಾತ್ರ_2 ಸಂಪೂರ್ಣ_ಗಾತ್ರ;
        ಬಿಂದು ofs;
        src.locateROI(ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ofs);
        ಪೂರ್ಣಾಂಕ dtop = std::min(ofs.y, ತುದಿ_2);
        ಪೂರ್ಣಾಂಕ dbottom = std::min(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ - src.ಅಡ್ಡಸಾಲುಗಳು - ofs.y, bottom);
        ಪೂರ್ಣಾಂಕ dleft = std::min(ofs.x, ಎಡ);
        ಪೂರ್ಣಾಂಕ dright = std::min(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ - src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - ofs.x, ಬಲ);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ತುದಿ_2 -= dtop;
        ಎಡ -= dleft;
        bottom -= dbottom;
        ಬಲ -= dright;
    }

    _dst.create(src.ಅಡ್ಡಸಾಲುಗಳು + ತುದಿ_2 + bottom, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + ಎಡ + ಬಲ, type);
    UMat dst = _dst.getUMat();

    ಆದರೆ (ತುದಿ_2 == 0 && ಎಡ == 0 && bottom == 0 && ಬಲ == 0)
    {
        ಆದರೆ(src.u != dst.u || src.ಹೆಜ್ಜೆ != dst.ಹೆಜ್ಜೆ)
            src.copy_ಗಮ್ಯ(dst);
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    k.ವಾದಗಳು(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           ತುದಿ_2, ಎಡ, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ((size_t)dst.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_copyMakeBorder( Mat &_src, Mat &_dst, ಪೂರ್ಣಾಂಕ ತುದಿ_2, ಪೂರ್ಣಾಂಕ bottom,
                                ಪೂರ್ಣಾಂಕ ಎಡ, ಪೂರ್ಣಾಂಕ ಬಲ, ಪೂರ್ಣಾಂಕ _borderType, ಸ್ಥಿರಾಂಕ Scalar& value )
{
#ಆದರೆ defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_ಗಾತ್ರ border_ಗಾತ್ರ(ಎಡ, ತುದಿ_2, ಬಲ, bottom);
    ::ipp::Iwi_ಗಾತ್ರ       size(_src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, _src.ಅಡ್ಡಸಾಲುಗಳು);
    Ipp_ದತ್ತಾಂಶ_Type          ದತ್ತಾಂಶ_Type   = ippi_Get_ದತ್ತಾಂಶ_Type(_src.ಆಳ());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    ಆದರೆ((ಪೂರ್ಣಾಂಕ)borderType == -1)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ(_src.dims > 2)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    Rect dstRect(border_ಗಾತ್ರ.ಎಡ, border_ಗಾತ್ರ.ತುದಿ_2,
        _dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - border_ಗಾತ್ರ.ಬಲ - border_ಗಾತ್ರ.ಎಡ,
        _dst.ಅಡ್ಡಸಾಲುಗಳು - border_ಗಾತ್ರ.bottom - border_ಗಾತ್ರ.ತುದಿ_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->ಹೆಜ್ಜೆ, subDst.ptr(), subDst.ಹೆಜ್ಜೆ, size, ದತ್ತಾಂಶ_Type, _src.channels(), border_ಗಾತ್ರ, borderType, &value[0]) >= 0;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(_src); CV_ಬಳಸದ(_dst); CV_ಬಳಸದ(ತುದಿ_2); CV_ಬಳಸದ(bottom); CV_ಬಳಸದ(ಎಡ); CV_ಬಳಸದ(ಬಲ);
    CV_ಬಳಸದ(_borderType); CV_ಬಳಸದ(value);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cv::copyMakeBorder( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ ತುದಿ_2, ಪೂರ್ಣಾಂಕ bottom,
                         ಪೂರ್ಣಾಂಕ ಎಡ, ಪೂರ್ಣಾಂಕ ಬಲ, ಪೂರ್ಣಾಂಕ borderType, ಸ್ಥಿರಾಂಕ Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( ತುದಿ_2 >= 0 && bottom >= 0 && ಎಡ >= 0 && ಬಲ >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.ಅಡ್ಡಸಾಲುಗಳು + ತುದಿ_2 + bottom <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
    CV_Assert( (int64_t)src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + ಎಡ + ಬಲ <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );

    CV_OCL_ಸತತ(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, ತುದಿ_2, bottom, ಎಡ, ಬಲ, borderType, value))
    ಪೂರ್ಣಾಂಕ type = src.type();

    ಆದರೆ( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        ಗಾತ್ರ_2 ಸಂಪೂರ್ಣ_ಗಾತ್ರ;
        ಬಿಂದು ofs;
        src.locateROI(ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ofs);
        ಪೂರ್ಣಾಂಕ dtop = std::min(ofs.y, ತುದಿ_2);
        ಪೂರ್ಣಾಂಕ dbottom = std::min(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ - src.ಅಡ್ಡಸಾಲುಗಳು - ofs.y, bottom);
        ಪೂರ್ಣಾಂಕ dleft = std::min(ofs.x, ಎಡ);
        ಪೂರ್ಣಾಂಕ dright = std::min(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ - src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - ofs.x, ಬಲ);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ತುದಿ_2 -= dtop;
        ಎಡ -= dleft;
        bottom -= dbottom;
        ಬಲ -= dright;
    }

    _dst.create( src.ಅಡ್ಡಸಾಲುಗಳು + ತುದಿ_2 + bottom, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + ಎಡ + ಬಲ, type );
    Mat dst = _dst.getMat();

    ಆದರೆ(ತುದಿ_2 == 0 && ಎಡ == 0 && bottom == 0 && ಬಲ == 0)
    {
        ಆದರೆ(src.ದತ್ತಾಂಶ != dst.ದತ್ತಾಂಶ || src.ಹೆಜ್ಜೆ != dst.ಹೆಜ್ಜೆ)
            src.copy_ಗಮ್ಯ(dst);
        ಹಿಂತಿರುಗಿ;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_ಸತತ_FAST(ipp_copyMakeBorder(src, dst, ತುದಿ_2, bottom, ಎಡ, ಬಲ, borderType, value))

    ಆದರೆ( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.ಹೆಜ್ಜೆ, src.size(),
                           dst.ptr(), dst.ಹೆಜ್ಜೆ, dst.size(),
                           ತುದಿ_2, ಎಡ, (ಪೂರ್ಣಾಂಕ)src.elem_ಗಾತ್ರ(), borderType );
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ cn = src.channels(), cn1 = cn;
        Auto_ಬಫರ್<ದ್ವಿಗುಣ> buf(cn);
        ಆದರೆ( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ(value, buf.ದತ್ತಾಂಶ(), CV_MAKETYPE(src.ಆಳ(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.ಹೆಜ್ಜೆ, src.size(),
                                dst.ptr(), dst.ಹೆಜ್ಜೆ, dst.size(),
                                ತುದಿ_2, ಎಡ, (ಪೂರ್ಣಾಂಕ)src.elem_ಗಾತ್ರ(), (uchar*)buf.ದತ್ತಾಂಶ() );
    }
}
/* End of file. */
