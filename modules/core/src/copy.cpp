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

template <typename T> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ scalarToRawData_(ಧ್ರುವ Scalar& s, T * ಧ್ರುವ buf, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ unroll_to)
{
    ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಪ್ರತಿ(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    ಪ್ರತಿ(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

ಶೂನ್ಯ scalarToRawData(ಧ್ರುವ Scalar& s, ಶೂನ್ಯ* _buf, ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ unroll_to)
{
    CV_INSTRUMENT_REGION();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    ವಿಕಲ್ಪ(depth)
    {
    ಪಕ್ಷ CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_Bool:
        scalarToRawData_(s, (ತಾರ್ಕಿಕ*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_32S:
        scalarToRawData_(s, (ಪೂರ್ಣಾಙ್ಕ*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_32F:
        scalarToRawData_(s, (ಭಿನ್ನ*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_64F:
        scalarToRawData_(s, (ದ್ವಿಗುಣ*)_buf, cn, unroll_to);
        ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

ಶೂನ್ಯ convertAndUnrollScalar( ಧ್ರುವ Mat& sc, ಪೂರ್ಣಾಙ್ಕ buftype, uchar* scbuf, size_t blocksize )
{
    ಪೂರ್ಣಾಙ್ಕ scn = (ಪೂರ್ಣಾಙ್ಕ)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    ದ್ವಿಮಾನ_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    ಯದಿ( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        ಪ್ರತಿ( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    ಪ್ರತಿ( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ
copyMask_(ಧ್ರುವ uchar* _src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    ಪ್ರತಿ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ಧ್ರುವ T* src = (ಧ್ರುವ T*)_src;
        T* dst = (T*)_dst;
        ಪೂರ್ಣಾಙ್ಕ x = 0;
         #ಯದಿ CV_ENABLE_UNROLLED
        ಪ್ರತಿ( ; x <= size.width - 4; x += 4 )
        {
            ಯದಿ( mask[x] )
                dst[x] = src[x];
            ಯದಿ( mask[x+1] )
                dst[x+1] = src[x+1];
            ಯದಿ( mask[x+2] )
                dst[x+2] = src[x+2];
            ಯದಿ( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        ಪ್ರತಿ( ; x < size.width; x++ )
            ಯದಿ( mask[x] )
                dst[x] = src[x];
    }
}

template<> ಶೂನ್ಯ
copyMask_<uchar>(ಧ್ರುವ uchar* _src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (ಪೂರ್ಣಾಙ್ಕ)sstep, _dst, (ಪೂರ್ಣಾಙ್ಕ)dstep, ippiSize(size), mask, (ಪೂರ್ಣಾಙ್ಕ)mstep) >= 0)

    ಪ್ರತಿ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ಧ್ರುವ uchar* src = (ಧ್ರುವ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ಪೂರ್ಣಾಙ್ಕ x = 0;
        #ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            ಪ್ರತಿ( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_load(src  + x),
                        v_dst   = vx_load(dst  + x),
                        v_nmask = v_eq(vx_load(mask + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #endif
        ಪ್ರತಿ( ; x < size.width; x++ )
            ಯದಿ( mask[x] )
                dst[x] = src[x];
    }
}

template<> ಶೂನ್ಯ
copyMask_<Vec3b>(ಧ್ರುವ uchar* _src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    ಪ್ರತಿ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ಧ್ರುವ uchar* src = (ಧ್ರುವ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ಪೂರ್ಣಾಙ್ಕ x = 0;
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
        ಪ್ರತಿ( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_load(mask + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_load_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_load_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#endif
        ಪ್ರತಿ( ; x < size.width; x++ )
            ಯದಿ( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> ಶೂನ್ಯ
copyMask_<ushort>(ಧ್ರುವ uchar* _src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ಧ್ರುವ Ipp16u *)_src, (ಪೂರ್ಣಾಙ್ಕ)sstep, (Ipp16u *)_dst, (ಪೂರ್ಣಾಙ್ಕ)dstep, ippiSize(size), mask, (ಪೂರ್ಣಾಙ್ಕ)mstep) >= 0)

    ಪ್ರತಿ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ಧ್ರುವ ushort* src = (ಧ್ರುವ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ಪೂರ್ಣಾಙ್ಕ x = 0;
        #ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            ಪ್ರತಿ( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_load(src + x), v_src2 = vx_load(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_load(dst + x), v_dst2 = vx_load(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_load(mask + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #endif
        ಪ್ರತಿ( ; x < size.width; x++ )
            ಯದಿ( mask[x] )
                dst[x] = src[x];
    }
}

template<> ಶೂನ್ಯ
copyMask_<Vec3s>(ಧ್ರುವ uchar* _src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    ಪ್ರತಿ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ಧ್ರುವ ushort* src = (ಧ್ರುವ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ಪೂರ್ಣಾಙ್ಕ x = 0;
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
        ಪ್ರತಿ( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_load(mask + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_load_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_load_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_load_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_load_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

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
#endif
        ಪ್ರತಿ( ; x < size.width; x++ )
            ಯದಿ( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> ಶೂನ್ಯ
copyMask_<ಪೂರ್ಣಾಙ್ಕ>(ಧ್ರುವ uchar* _src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    ಪ್ರತಿ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src = (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)_src;
        ಪೂರ್ಣಾಙ್ಕ* dst = (ಪೂರ್ಣಾಙ್ಕ*)_dst;
        ಪೂರ್ಣಾಙ್ಕ x = 0;
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
        ಪ್ರತಿ (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_load(src + x), v_dst0 = vx_load(dst + x);
            v_int32 v_src1 = vx_load(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_load(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_load(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_load(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_load(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_load(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_load(mask + x), vx_setzero_u8());
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
#endif
        ಪ್ರತಿ (; x < size.width; x++)
            ಯದಿ ( mask[x] )
                dst[x] = src[x];
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
copyMaskGeneric(ಧ್ರುವ uchar* _src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, ಶೂನ್ಯ* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    ಪ್ರತಿ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ಧ್ರುವ uchar* src = _src;
        uchar* dst = _dst;
        ಪೂರ್ಣಾಙ್ಕ x = 0;
        ಪ್ರತಿ( ; x < size.width; x++, src += esz, dst += esz )
        {
            ಯದಿ( !mask[x] )
                ಅನುವರ್ತ;
            ಪ್ರತಿ( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
ಸ್ಥಿರ ಶೂನ್ಯ copyMask##suffix(ಧ್ರುವ uchar* src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ಶೂನ್ಯ*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#ಯದಿ defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
ಸ್ಥಿರ ಶೂನ್ಯ copyMask##suffix(ಧ್ರುವ uchar* src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ಶೂನ್ಯ*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ಧ್ರುವ ipptype *)src, (ಪೂರ್ಣಾಙ್ಕ)sstep, (ipptype *)dst, (ಪೂರ್ಣಾಙ್ಕ)dstep, ippiSize(size), (ಧ್ರುವ Ipp8u *)mask, (ಪೂರ್ಣಾಙ್ಕ)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#ಅನ್ಯಥಾ
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
ಸ್ಥಿರ ಶೂನ್ಯ copyMask##suffix(ಧ್ರುವ uchar* src, size_t sstep, ಧ್ರುವ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ಶೂನ್ಯ*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#ಯದಿ IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#ಅನ್ಯಥಾ
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, ಪೂರ್ಣಾಙ್ಕ, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

ದ್ವಿಮಾನ_Func copyMaskTab[] =
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

ದ್ವಿಮಾನ_Func getCopyMaskFunc(size_t esz)
{
    ಪ್ರತ್ಯಾಗಮ esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
ಶೂನ್ಯ Mat::copyTo( OutputArray _dst ) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    ಯದಿ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ಏತತ್);
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    ಪೂರ್ಣಾಙ್ಕ stype = type();
    ಪೂರ್ಣಾಙ್ಕ dtype = _dst.type();
    ಯದಿ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( dims == 0 && empty() )
    {
        _dst.release();
        ಶೂನ್ಯ* obj = _dst.getObj();
        ಯದಿ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ಅನ್ಯಥಾ ಯದಿ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ಅನ್ಯಥಾ ಯದಿ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ತಾರ್ಕಿಕ allowTransposed = dims == 1 ||
        _dst.kind() == ಒಳಹರಿವು_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    ಯದಿ( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        ಯದಿ (empty())
            ಪ್ರತ್ಯಾಗಮ;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != ರಿಕ್ತ);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        ಪ್ರತಿ( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        ಪೂರ್ಣಾಙ್ಕ lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        ಯದಿ( data == dst.data )
            ಪ್ರತ್ಯಾಗಮ;

        ಯದಿ( rows > 0 && cols > 0 )
        {
            Mat src = *ಏತತ್;
            Size sz = getContinuousSize2D(src, dst, (ಪೂರ್ಣಾಙ್ಕ)elemSize());
            CV_CheckGE(sz.width, 0, "");

            ಧ್ರುವ uchar* sptr = src.data;
            uchar* dptr = dst.data;

#ಯದಿ IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (ಪೂರ್ಣಾಙ್ಕ)src.step, dptr, (ಪೂರ್ಣಾಙ್ಕ)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            ಪ್ರತಿ (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        ಪ್ರತ್ಯಾಗಮ;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    ಯದಿ( data == dst.data )
        ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ( total() != 0 )
    {
        ಧ್ರುವ Mat* arrays[] = { ಏತತ್, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_copyTo(ಧ್ರುವ Mat &src, Mat &dst, ಧ್ರುವ Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಯದಿ(mask.channels() > 1 || mask.depth() != CV_8U)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        ಪ್ರತ್ಯಾಗಮ CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (ಪೂರ್ಣಾಙ್ಕ)src.step, dst.ptr(), (ಪೂರ್ಣಾಙ್ಕ)dst.step, size, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize1(), src.channels(), mask.ptr(), (ಪೂರ್ಣಾಙ್ಕ)mask.step) >= 0;
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ Mat      *arrays[] = {&src, &dst, &mask, ರಿಕ್ತ};
        uchar          *ptrs[3]  = {ರಿಕ್ತ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        ಪ್ರತಿ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}
#endif

ಶೂನ್ಯ Mat::copyTo( OutputArray _dst, ಒಳಹರಿವು_Array _mask ) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    ಯದಿ( !mask.data )
    {
        copyTo(_dst);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪೂರ್ಣಾಙ್ಕ cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    ತಾರ್ಕಿಕ colorMask = mcn > 1;
    ಯದಿ( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        ಯದಿ (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*ಏತತ್, dst, mask))
    ಯದಿ ( ಏತತ್->dims <= 2 ) {
        ಯದಿ ( ಏತತ್->size() == dst.size() && ಏತತ್->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, ಏತತ್->data, ಏತತ್->step, dst.data, dst.step, ಏತತ್->cols, ಏತತ್->rows, ಏತತ್->type(), mask.data, mask.step, mask.type());
        }
    }
    ಅನ್ಯಥಾ ಯದಿ ( ಏತತ್->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = ಏತತ್->total();
        ಯದಿ (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, ಏತತ್->data, 0, dst.data, 0, (ಪೂರ್ಣಾಙ್ಕ)sz, 1, ಏತತ್->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    ದ್ವಿಮಾನ_Func copymask = getCopyMaskFunc(esz);

    ಯದಿ( dims <= 2 )
    {
        Mat src = *ಏತತ್;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಧ್ರುವ Mat* arrays[] = { ಏತತ್, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((ಪೂರ್ಣಾಙ್ಕ)(it.size*mcn), 1);

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
ಶೂನ್ಯ Mat::copyAt( OutputArray _dst ) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
ಶೂನ್ಯ Mat::copyAt( OutputArray _dst, ಒಳಹರಿವು_Array _mask ) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


ಸ್ಥಿರ ತಾರ್ಕಿಕ can_apply_memset(ಧ್ರುವ Mat &mat, ಧ್ರುವ Scalar &s, ಪೂರ್ಣಾಙ್ಕ &fill_ಮೌಲ್ಯ)
{
    // check if depth is 1 byte.
    ವಿಕಲ್ಪ (mat.depth())
    {
    ಪಕ್ಷ CV_8U: fill_ಮೌಲ್ಯ = saturate_cast<uchar>( s.val[0] ); ಭಙ್ಗ;
    ಪಕ್ಷ CV_8S: fill_ಮೌಲ್ಯ = saturate_cast<schar>( s.val[0] ); ಭಙ್ಗ;
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    // check if all element is same.
    ಧ್ರುವ int64* is = (ಧ್ರುವ int64*)&s.val[0];
    ವಿಕಲ್ಪ (mat.channels())
    {
    ಪಕ್ಷ 1: ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪಕ್ಷ 2: ಪ್ರತ್ಯಾಗಮ (is[0] == is[1]);
    ಪಕ್ಷ 3: ಪ್ರತ್ಯಾಗಮ (is[0] == is[1] && is[1] == is[2]);
    ಪಕ್ಷ 4: ಪ್ರತ್ಯಾಗಮ (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
}

Mat& Mat::operator = (ಧ್ರುವ Scalar& s)
{
    CV_INSTRUMENT_REGION();

    ಯದಿ (ಏತತ್->empty())
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    ಧ್ರುವ Mat* arrays[] = { ಏತತ್ };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    ಧ್ರುವ int64* is = (ಧ್ರುವ int64*)&s.val[0];

    ಯದಿ( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ fill_ಮೌಲ್ಯ = 0;
        ಯದಿ ( can_apply_memset(*ಏತತ್, s, fill_ಮೌಲ್ಯ) )
        {
            ಪ್ರತಿ (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_ಮೌಲ್ಯ, elsize);
            ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
        }

        ಯದಿ( it.nplanes > 0 )
        {
            ದ್ವಿಗುಣ scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            ಪ್ರತಿ( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= ಪರಿಮಾಣ(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        ಪ್ರತಿ( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

#ifdef HAVE_IPP
ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಯದಿ(mask.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ(mask.depth() != CV_8U || mask.channels() > 1)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ(dst.channels() > 4)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (dst.depth() == CV_32F)
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)(_val.total()); i++)
        {
            ಭಿನ್ನ v = (ಭಿನ್ನ)(_val.at<ದ್ವಿಗುಣ>(i));  // cast to float
            ಯದಿ (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
    }

    ಯದಿ(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_ಮೌಲ್ಯ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ಪ್ರತ್ಯಾಗಮ CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (ಪೂರ್ಣಾಙ್ಕ)dst.step, size, dataType, dst.channels(), mask.ptr(), (ಪೂರ್ಣಾಙ್ಕ)mask.step) >= 0;
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ Mat      *arrays[] = {&dst, mask.empty()?ರಿಕ್ತ:&mask, ರಿಕ್ತ};
        uchar          *ptrs[2]  = {ರಿಕ್ತ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(ಪೂರ್ಣಾಙ್ಕ)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_ಮೌಲ್ಯ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}
#endif

Mat& Mat::setTo(ಒಳಹರಿವು_Array ಮೌಲ್ಯ, ಒಳಹರಿವು_Array _mask)
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( empty() )
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    Mat value = ಮೌಲ್ಯ.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), ಮೌಲ್ಯ.kind(), ಒಳಹರಿವು_Array_2::MAT ));
    ಪೂರ್ಣಾಙ್ಕ cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*ಏತತ್, value, mask), *ಏತತ್)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    ದ್ವಿಮಾನ_Func copymask = getCopyMaskFunc(esz);

    ಧ್ರುವ Mat* arrays[] = { ಏತತ್, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ totalsz = (ಪೂರ್ಣಾಙ್ಕ)it.size*mcn;
    ಪೂರ್ಣಾಙ್ಕ blockSize0 = std::min(totalsz, (ಪೂರ್ಣಾಙ್ಕ)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(ದ್ವಿಗುಣ));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            ಯದಿ( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            ಅನ್ಯಥಾ
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( empty() )
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    size_t esz = elemSize();

    ಧ್ರುವ Mat* arrays[] = { ಏತತ್, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}


#ಯದಿ defined HAVE_OPENCL && !defined __APPLE__

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_repeat(ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಙ್ಕ ny, ಪೂರ್ಣಾಙ್ಕ nx, OutputArray _dst)
{
    ಯದಿ (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಶೂನ್ಯ repeat(ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಙ್ಕ ny, ಪೂರ್ಣಾಙ್ಕ nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    ಯದಿ( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        ಪ್ರತ್ಯಾಗಮ;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#ಯದಿ !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    ಪೂರ್ಣಾಙ್ಕ esz = (ಪೂರ್ಣಾಙ್ಕ)src.elemSize();
    ಪೂರ್ಣಾಙ್ಕ x, y;
    ssize.width *= esz; dsize.width *= esz;

    ಪ್ರತಿ( y = 0; y < ssize.height; y++ )
    {
        ಪ್ರತಿ( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    ಪ್ರತಿ( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(ಧ್ರುವ Mat& src, ಪೂರ್ಣಾಙ್ಕ ny, ಪೂರ್ಣಾಙ್ಕ nx)
{
    ಯದಿ( nx == 1 && ny == 1 )
        ಪ್ರತ್ಯಾಗಮ src;
    Mat dst;
    repeat(src, ny, nx, dst);
    ಪ್ರತ್ಯಾಗಮ dst;
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
ಪೂರ್ಣಾಙ್ಕ cv::borderInterpolate( ಪೂರ್ಣಾಙ್ಕ p, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    ಯದಿ(p >= 0 && p < len)
#ಅನ್ಯಥಾ
    ಯದಿ( (unsigned)p < (unsigned)len )
#endif
        ;
    ಅನ್ಯಥಾ ಯದಿ( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    ಅನ್ಯಥಾ ಯದಿ( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        ಪೂರ್ಣಾಙ್ಕ delta = borderType == BORDER_REFLECT_101;
        ಯದಿ( len == 1 )
            ಪ್ರತ್ಯಾಗಮ 0;
        ಕುರು
        {
            ಯದಿ( p < 0 )
                p = -p - 1 + delta;
            ಅನ್ಯಥಾ
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        ಯಾವತ್(p < 0 || p >= len);
#ಅನ್ಯಥಾ
        ಯಾವತ್( (unsigned)p >= (unsigned)len );
#endif
    }
    ಅನ್ಯಥಾ ಯದಿ( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        ಯದಿ( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            ಯದಿ( p64 < 0 )
                p64 += len;
            p = (ಪೂರ್ಣಾಙ್ಕ)p64;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( borderType == BORDER_CONSTANT )
        p = -1;
    ಅನ್ಯಥಾ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    ಪ್ರತ್ಯಾಗಮ p;
}

ನಾಮಾಕಾಶ
{

ಶೂನ್ಯ copyMakeBorder_8u( ಧ್ರುವ uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        ಪೂರ್ಣಾಙ್ಕ ತುದಿ_2, ಪೂರ್ಣಾಙ್ಕ ಎಡ, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ borderType )
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ isz = (ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);
    ಪೂರ್ಣಾಙ್ಕ i, j, k, elemSize = 1;
    ತಾರ್ಕಿಕ intMode = ಅಸತ್ಯ;

    ಯದಿ( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = ಸತ್ಯ;
    }

    cv::AutoBuffer<ಪೂರ್ಣಾಙ್ಕ> _tab((dstroi.width - srcroi.width)*cn);
    ಪೂರ್ಣಾಙ್ಕ* tab = _tab.data();
    ಪೂರ್ಣಾಙ್ಕ ಬಲ = dstroi.width - srcroi.width - ಎಡ;
    ಪೂರ್ಣಾಙ್ಕ bottom = dstroi.height - srcroi.height - ತುದಿ_2;

    ಪ್ರತಿ( i = 0; i < ಎಡ; i++ )
    {
        j = cv::borderInterpolate(i - ಎಡ, srcroi.width, borderType)*cn;
        ಪ್ರತಿ( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    ಪ್ರತಿ( i = 0; i < ಬಲ; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        ಪ್ರತಿ( k = 0; k < cn; k++ )
            tab[(i+ಎಡ)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    ಎಡ *= cn;
    ಬಲ *= cn;

    uchar* dstInner = dst + dststep*ತುದಿ_2 + ಎಡ*elemSize;

    ಪ್ರತಿ( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        ಯದಿ( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        ಯದಿ( intMode )
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* isrc = (ಪೂರ್ಣಾಙ್ಕ*)src;
            ಪೂರ್ಣಾಙ್ಕ* idstInner = (ಪೂರ್ಣಾಙ್ಕ*)dstInner;
            ಪ್ರತಿ( j = 0; j < ಎಡ; j++ )
                idstInner[j - ಎಡ] = isrc[tab[j]];
            ಪ್ರತಿ( j = 0; j < ಬಲ; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + ಎಡ]];
        }
        ಅನ್ಯಥಾ
        {
            ಪ್ರತಿ( j = 0; j < ಎಡ; j++ )
                dstInner[j - ಎಡ] = src[tab[j]];
            ಪ್ರತಿ( j = 0; j < ಬಲ; j++ )
                dstInner[j + srcroi.width] = src[tab[j + ಎಡ]];
        }
    }

    dstroi.width *= elemSize;

    ಪ್ರತಿ( i = 0; i < ತುದಿ_2; i++ )
    {
        j = cv::borderInterpolate(i - ತುದಿ_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (ತುದಿ_2+j)*dststep, dstroi.width);
    }

    dst += dststep*ತುದಿ_2;
    ಪ್ರತಿ( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


ಶೂನ್ಯ copyMakeConstBorder_8u( ಧ್ರುವ uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             ಪೂರ್ಣಾಙ್ಕ ತುದಿ_2, ಪೂರ್ಣಾಙ್ಕ ಎಡ, ಪೂರ್ಣಾಙ್ಕ cn, ಧ್ರುವ uchar* value )
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    ಪೂರ್ಣಾಙ್ಕ ಬಲ = dstroi.width - srcroi.width - ಎಡ;
    ಪೂರ್ಣಾಙ್ಕ bottom = dstroi.height - srcroi.height - ತುದಿ_2;

    ಪ್ರತಿ( i = 0; i < dstroi.width; i++ )
    {
        ಪ್ರತಿ( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    ಎಡ *= cn;
    ಬಲ *= cn;

    uchar* dstInner = dst + dststep*ತುದಿ_2 + ಎಡ;

    ಪ್ರತಿ( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        ಯದಿ( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - ಎಡ, constBuf, ಎಡ );
        memcpy( dstInner + srcroi.width, constBuf, ಬಲ );
    }

    ಪ್ರತಿ( i = 0; i < ತುದಿ_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (ತುದಿ_2 + srcroi.height) * dststep;
    ಪ್ರತಿ( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_copyMakeBorder( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ ತುದಿ_2, ಪೂರ್ಣಾಙ್ಕ bottom,
                                ಪೂರ್ಣಾಙ್ಕ ಎಡ, ಪೂರ್ಣಾಙ್ಕ ಬಲ, ಪೂರ್ಣಾಙ್ಕ borderType, ಧ್ರುವ Scalar& value )
{
    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    ಯದಿ ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    ಪೂರ್ಣಾಙ್ಕ scalarcn = cn == 3 ? 4 : cn;
    ಪೂರ್ಣಾಙ್ಕ sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src = _src.getUMat();
    ಯದಿ( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        ಪೂರ್ಣಾಙ್ಕ dtop = std::min(ofs.y, ತುದಿ_2);
        ಪೂರ್ಣಾಙ್ಕ dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        ಪೂರ್ಣಾಙ್ಕ dleft = std::min(ofs.x, ಎಡ);
        ಪೂರ್ಣಾಙ್ಕ dright = std::min(wholeSize.width - src.cols - ofs.x, ಬಲ);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ತುದಿ_2 -= dtop;
        ಎಡ -= dleft;
        bottom -= dbottom;
        ಬಲ -= dright;
    }

    _dst.create(src.rows + ತುದಿ_2 + bottom, src.cols + ಎಡ + ಬಲ, type);
    UMat dst = _dst.getUMat();

    ಯದಿ (ತುದಿ_2 == 0 && ಎಡ == 0 && bottom == 0 && ಬಲ == 0)
    {
        ಯದಿ(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           ತುದಿ_2, ಎಡ, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

}
#endif

#ifdef HAVE_IPP
ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_copyMakeBorder( Mat &_src, Mat &_dst, ಪೂರ್ಣಾಙ್ಕ ತುದಿ_2, ಪೂರ್ಣಾಙ್ಕ bottom,
                                ಪೂರ್ಣಾಙ್ಕ ಎಡ, ಪೂರ್ಣಾಙ್ಕ ಬಲ, ಪೂರ್ಣಾಙ್ಕ _borderType, ಧ್ರುವ Scalar& value )
{
#ಯದಿ defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(ಎಡ, ತುದಿ_2, ಬಲ, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    ಯದಿ((ಪೂರ್ಣಾಙ್ಕ)borderType == -1)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ(_src.dims > 2)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    Rect dstRect(borderSize.ಎಡ, borderSize.ತುದಿ_2,
        _dst.cols - borderSize.ಬಲ - borderSize.ಎಡ,
        _dst.rows - borderSize.bottom - borderSize.ತುದಿ_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    ಪ್ರತ್ಯಾಗಮ CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#ಅನ್ಯಥಾ
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(ತುದಿ_2); CV_UNUSED(bottom); CV_UNUSED(ಎಡ); CV_UNUSED(ಬಲ);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}
}
#endif

ಶೂನ್ಯ cv::copyMakeBorder( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ ತುದಿ_2, ಪೂರ್ಣಾಙ್ಕ bottom,
                         ಪೂರ್ಣಾಙ್ಕ ಎಡ, ಪೂರ್ಣಾಙ್ಕ ಬಲ, ಪೂರ್ಣಾಙ್ಕ borderType, ಧ್ರುವ Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( ತುದಿ_2 >= 0 && bottom >= 0 && ಎಡ >= 0 && ಬಲ >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + ತುದಿ_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + ಎಡ + ಬಲ <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, ತುದಿ_2, bottom, ಎಡ, ಬಲ, borderType, value))
    ಪೂರ್ಣಾಙ್ಕ type = src.type();

    ಯದಿ( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        ಪೂರ್ಣಾಙ್ಕ dtop = std::min(ofs.y, ತುದಿ_2);
        ಪೂರ್ಣಾಙ್ಕ dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        ಪೂರ್ಣಾಙ್ಕ dleft = std::min(ofs.x, ಎಡ);
        ಪೂರ್ಣಾಙ್ಕ dright = std::min(wholeSize.width - src.cols - ofs.x, ಬಲ);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ತುದಿ_2 -= dtop;
        ಎಡ -= dleft;
        bottom -= dbottom;
        ಬಲ -= dright;
    }

    _dst.create( src.rows + ತುದಿ_2 + bottom, src.cols + ಎಡ + ಬಲ, type );
    Mat dst = _dst.getMat();

    ಯದಿ(ತುದಿ_2 == 0 && ಎಡ == 0 && bottom == 0 && ಬಲ == 0)
    {
        ಯದಿ(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        ಪ್ರತ್ಯಾಗಮ;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, ತುದಿ_2, bottom, ಎಡ, ಬಲ, borderType, value))

    ಯದಿ( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           ತುದಿ_2, ಎಡ, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize(), borderType );
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ cn = src.channels(), cn1 = cn;
        AutoBuffer<ದ್ವಿಗುಣ> buf(cn);
        ಯದಿ( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                ತುದಿ_2, ಎಡ, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
