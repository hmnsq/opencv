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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

ନାମାକାଶ cv
{

template <typename T> ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ scalarToRawData_(ଅଚଳ Scalar& s, T * ଅଚଳ buf, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା unroll_to)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ପାଇଁ(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    ପାଇଁ(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

ଶୂନ୍ୟ scalarToRawData(ଅଚଳ Scalar& s, ଶୂନ୍ୟ* _buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unroll_to)
{
    CV_INSTRUMENT_REGION();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    ବାଛ(depth)
    {
    ସ୍ଥିତି CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_Bool:
        scalarToRawData_(s, (ତାର୍କିକ*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_32S:
        scalarToRawData_(s, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_32F:
        scalarToRawData_(s, (ଦଶମିକ*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_64F:
        scalarToRawData_(s, (ଦ୍ୱିଗୁଣ*)_buf, cn, unroll_to);
        ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

ଶୂନ୍ୟ convertAndUnrollScalar( ଅଚଳ Mat& sc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା buftype, uchar* scbuf, size_t blocksize )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    ଦ୍ୱିମିକ_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    ଯଦି( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        ପାଇଁ( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    ପାଇଁ( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ
copyMask_(ଅଚଳ uchar* _src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    ପାଇଁ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ଅଚଳ T* src = (ଅଚଳ T*)_src;
        T* dst = (T*)_dst;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
         #ଯଦି CV_ENABLE_UNROLLED
        ପାଇଁ( ; x <= size.width - 4; x += 4 )
        {
            ଯଦି( mask[x] )
                dst[x] = src[x];
            ଯଦି( mask[x+1] )
                dst[x+1] = src[x+1];
            ଯଦି( mask[x+2] )
                dst[x+2] = src[x+2];
            ଯଦି( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        ପାଇଁ( ; x < size.width; x++ )
            ଯଦି( mask[x] )
                dst[x] = src[x];
    }
}

template<> ଶୂନ୍ୟ
copyMask_<uchar>(ଅଚଳ uchar* _src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sstep, _dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dstep, ippiSize(size), mask, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mstep) >= 0)

    ପାଇଁ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ଅଚଳ uchar* src = (ଅଚଳ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        #ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            ପାଇଁ( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        ପାଇଁ( ; x < size.width; x++ )
            ଯଦି( mask[x] )
                dst[x] = src[x];
    }
}

template<> ଶୂନ୍ୟ
copyMask_<Vec3b>(ଅଚଳ uchar* _src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    ପାଇଁ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ଅଚଳ uchar* src = (ଅଚଳ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        ପାଇଁ( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        ପାଇଁ( ; x < size.width; x++ )
            ଯଦି( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> ଶୂନ୍ୟ
copyMask_<ushort>(ଅଚଳ uchar* _src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ଅଚଳ Ipp16u *)_src, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sstep, (Ipp16u *)_dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dstep, ippiSize(size), mask, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mstep) >= 0)

    ପାଇଁ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ଅଚଳ ushort* src = (ଅଚଳ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        #ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            ପାଇଁ( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        ପାଇଁ( ; x < size.width; x++ )
            ଯଦି( mask[x] )
                dst[x] = src[x];
    }
}

template<> ଶୂନ୍ୟ
copyMask_<Vec3s>(ଅଚଳ uchar* _src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    ପାଇଁ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ଅଚଳ ushort* src = (ଅଚଳ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        ପାଇଁ( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        ପାଇଁ( ; x < size.width; x++ )
            ଯଦି( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> ଶୂନ୍ୟ
copyMask_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(ଅଚଳ uchar* _src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    ପାଇଁ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src = (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_src;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dst = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_dst;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        ପାଇଁ (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        ପାଇଁ (; x < size.width; x++)
            ଯଦି ( mask[x] )
                dst[x] = src[x];
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
copyMaskGeneric(ଅଚଳ uchar* _src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, ଶୂନ୍ୟ* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    ପାଇଁ( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ଅଚଳ uchar* src = _src;
        uchar* dst = _dst;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ପାଇଁ( ; x < size.width; x++, src += esz, dst += esz )
        {
            ଯଦି( !mask[x] )
                ଜାରି;
            ପାଇଁ( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
ସ୍ଥିର ଶୂନ୍ୟ copyMask##suffix(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#ଯଦି defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
ସ୍ଥିର ଶୂନ୍ୟ copyMask##suffix(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ଅଚଳ ipptype *)src, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sstep, (ipptype *)dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dstep, ippiSize(size), (ଅଚଳ Ipp8u *)mask, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#ଅନ୍ୟଥା
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
ସ୍ଥିର ଶୂନ୍ୟ copyMask##suffix(ଅଚଳ uchar* src, size_t sstep, ଅଚଳ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ଶୂନ୍ୟ*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#ଯଦି IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#ଅନ୍ୟଥା
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

ଦ୍ୱିମିକ_Func copyMaskTab[] =
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

ଦ୍ୱିମିକ_Func getCopyMaskFunc(size_t esz)
{
    ଫେରନ୍ତୁ esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
ଶୂନ୍ୟ Mat::copyTo( OutputArray _dst ) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    ଯଦି (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ଏହି);
        ଫେରନ୍ତୁ;
    }
#endif

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = _dst.type();
    ଯଦି( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ଫେରନ୍ତୁ;
    }

    ଯଦି( dims == 0 && empty() )
    {
        _dst.release();
        ଶୂନ୍ୟ* obj = _dst.getObj();
        ଯଦି (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ଅନ୍ୟଥା ଯଦି (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ଅନ୍ୟଥା ଯଦି (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ଫେରନ୍ତୁ;
    }

    ତାର୍କିକ allowTransposed = dims == 1 ||
        _dst.kind() == ନିବେଶ_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    ଯଦି( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        ଯଦି (empty())
            ଫେରନ୍ତୁ;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != ଖାଲି);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        ପାଇଁ( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        ଯଦି( data == dst.data )
            ଫେରନ୍ତୁ;

        ଯଦି( rows > 0 && cols > 0 )
        {
            Mat src = *ଏହି;
            Size sz = getContinuousSize2D(src, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)elemSize());
            CV_CheckGE(sz.width, 0, "");

            ଅଚଳ uchar* sptr = src.data;
            uchar* dptr = dst.data;

#ଯଦି IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.step, dptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            ପାଇଁ (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        ଫେରନ୍ତୁ;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    ଯଦି( data == dst.data )
        ଫେରନ୍ତୁ;

    ଯଦି( total() != 0 )
    {
        ଅଚଳ Mat* arrays[] = { ଏହି, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
ସ୍ଥିର ତାର୍କିକ ipp_copyTo(ଅଚଳ Mat &src, Mat &dst, ଅଚଳ Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ଯଦି(mask.channels() > 1 || mask.depth() != CV_8U)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        ଫେରନ୍ତୁ CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.step, dst.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.step, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize1(), src.channels(), mask.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mask.step) >= 0;
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ Mat      *arrays[] = {&src, &dst, &mask, ଖାଲି};
        uchar          *ptrs[3]  = {ଖାଲି};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        ପାଇଁ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#ଅନ୍ୟଥା
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}
#endif

ଶୂନ୍ୟ Mat::copyTo( OutputArray _dst, ନିବେଶ_Array _mask ) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    ଯଦି( !mask.data )
    {
        copyTo(_dst);
        ଫେରନ୍ତୁ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    ତାର୍କିକ colorMask = mcn > 1;
    ଯଦି( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        ଯଦି (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*ଏହି, dst, mask))
    ଯଦି ( ଏହି->dims <= 2 ) {
        ଯଦି ( ଏହି->size() == dst.size() && ଏହି->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, ଏହି->data, ଏହି->step, dst.data, dst.step, ଏହି->cols, ଏହି->rows, ଏହି->type(), mask.data, mask.step, mask.type());
        }
    }
    ଅନ୍ୟଥା ଯଦି ( ଏହି->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = ଏହି->total();
        ଯଦି (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, ଏହି->data, 0, dst.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz, 1, ଏହି->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    ଦ୍ୱିମିକ_Func copymask = getCopyMaskFunc(esz);

    ଯଦି( dims <= 2 )
    {
        Mat src = *ଏହି;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        ଫେରନ୍ତୁ;
    }

    ଅଚଳ Mat* arrays[] = { ଏହି, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(it.size*mcn), 1);

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
ଶୂନ୍ୟ Mat::copyAt( OutputArray _dst ) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
ଶୂନ୍ୟ Mat::copyAt( OutputArray _dst, ନିବେଶ_Array _mask ) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


ସ୍ଥିର ତାର୍କିକ can_apply_memset(ଅଚଳ Mat &mat, ଅଚଳ Scalar &s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା &fill_ମୂଲ୍ୟ)
{
    // check if depth is 1 byte.
    ବାଛ (mat.depth())
    {
    ସ୍ଥିତି CV_8U: fill_ମୂଲ୍ୟ = saturate_cast<uchar>( s.val[0] ); ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_8S: fill_ମୂଲ୍ୟ = saturate_cast<schar>( s.val[0] ); ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    // check if all element is same.
    ଅଚଳ int64* is = (ଅଚଳ int64*)&s.val[0];
    ବାଛ (mat.channels())
    {
    ସ୍ଥିତି 1: ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 2: ଫେରନ୍ତୁ (is[0] == is[1]);
    ସ୍ଥିତି 3: ଫେରନ୍ତୁ (is[0] == is[1] && is[1] == is[2]);
    ସ୍ଥିତି 4: ଫେରନ୍ତୁ (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
}

Mat& Mat::operator = (ଅଚଳ Scalar& s)
{
    CV_INSTRUMENT_REGION();

    ଯଦି (ଏହି->empty())
        ଫେରନ୍ତୁ *ଏହି;

    ଅଚଳ Mat* arrays[] = { ଏହି };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    ଅଚଳ int64* is = (ଅଚଳ int64*)&s.val[0];

    ଯଦି( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା fill_ମୂଲ୍ୟ = 0;
        ଯଦି ( can_apply_memset(*ଏହି, s, fill_ମୂଲ୍ୟ) )
        {
            ପାଇଁ (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_ମୂଲ୍ୟ, elsize);
            ଫେରନ୍ତୁ *ଏହି;
        }

        ଯଦି( it.nplanes > 0 )
        {
            ଦ୍ୱିଗୁଣ scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            ପାଇଁ( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= ଆକାର(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        ପାଇଁ( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    ଫେରନ୍ତୁ *ଏହି;
}

#ifdef HAVE_IPP
ସ୍ଥିର ତାର୍କିକ ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ଯଦି(mask.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି(mask.depth() != CV_8U || mask.channels() > 1)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି(dst.channels() > 4)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (dst.depth() == CV_32F)
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(_val.total()); i++)
        {
            ଦଶମିକ v = (ଦଶମିକ)(_val.at<ଦ୍ୱିଗୁଣ>(i));  // cast to float
            ଯଦି (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
    }

    ଯଦି(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_ମୂଲ୍ୟ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ଫେରନ୍ତୁ CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.step, size, dataType, dst.channels(), mask.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mask.step) >= 0;
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ Mat      *arrays[] = {&dst, mask.empty()?ଖାଲି:&mask, ଖାଲି};
        uchar          *ptrs[2]  = {ଖାଲି};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_ମୂଲ୍ୟ_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#ଅନ୍ୟଥା
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}
#endif

Mat& Mat::setTo(ନିବେଶ_Array ମୂଲ୍ୟ, ନିବେଶ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    ଯଦି( empty() )
        ଫେରନ୍ତୁ *ଏହି;

    Mat value = ମୂଲ୍ୟ.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), ମୂଲ୍ୟ.kind(), ନିବେଶ_Array_2::MAT ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*ଏହି, value, mask), *ଏହି)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    ଦ୍ୱିମିକ_Func copymask = getCopyMaskFunc(esz);

    ଅଚଳ Mat* arrays[] = { ଏହି, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା totalsz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size*mcn;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା blockSize0 = std::min(totalsz, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(ଦ୍ୱିଗୁଣ));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            ଯଦି( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            ଅନ୍ୟଥା
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    ଫେରନ୍ତୁ *ଏହି;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    ଯଦି( empty() )
        ଫେରନ୍ତୁ *ଏହି;

    size_t esz = elemSize();

    ଅଚଳ Mat* arrays[] = { ଏହି, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    ଫେରନ୍ତୁ *ଏହି;
}


#ଯଦି defined HAVE_OPENCL && !defined __APPLE__

ସ୍ଥିର ତାର୍କିକ ocl_repeat(ନିବେଶ_Array _src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ny, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nx, OutputArray _dst)
{
    ଯଦି (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ଶୂନ୍ୟ repeat(ନିବେଶ_Array _src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ny, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    ଯଦି( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        ଫେରନ୍ତୁ;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#ଯଦି !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, y;
    ssize.width *= esz; dsize.width *= esz;

    ପାଇଁ( y = 0; y < ssize.height; y++ )
    {
        ପାଇଁ( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    ପାଇଁ( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(ଅଚଳ Mat& src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ny, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nx)
{
    ଯଦି( nx == 1 && ny == 1 )
        ଫେରନ୍ତୁ src;
    Mat dst;
    repeat(src, ny, nx, dst);
    ଫେରନ୍ତୁ dst;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::borderInterpolate( ପୂର୍ଣ୍ଣସଂଖ୍ୟା p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    ଯଦି(p >= 0 && p < len)
#ଅନ୍ୟଥା
    ଯଦି( (unsigned)p < (unsigned)len )
#endif
        ;
    ଅନ୍ୟଥା ଯଦି( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    ଅନ୍ୟଥା ଯଦି( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = borderType == BORDER_REFLECT_101;
        ଯଦି( len == 1 )
            ଫେରନ୍ତୁ 0;
        କର
        {
            ଯଦି( p < 0 )
                p = -p - 1 + delta;
            ଅନ୍ୟଥା
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        ଯେତେବେଳେ(p < 0 || p >= len);
#ଅନ୍ୟଥା
        ଯେତେବେଳେ( (unsigned)p >= (unsigned)len );
#endif
    }
    ଅନ୍ୟଥା ଯଦି( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        ଯଦି( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            ଯଦି( p64 < 0 )
                p64 += len;
            p = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)p64;
        }
    }
    ଅନ୍ୟଥା ଯଦି( borderType == BORDER_CONSTANT )
        p = -1;
    ଅନ୍ୟଥା
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    ଫେରନ୍ତୁ p;
}

ନାମାକାଶ
{

ଶୂନ୍ୟ copyMakeBorder_8u( ଅଚଳ uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶୀର୍ଷ_2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା borderType )
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା isz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k, elemSize = 1;
    ତାର୍କିକ intMode = ମିଥ୍ୟା;

    ଯଦି( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = ସତ୍ୟ;
    }

    cv::AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> _tab((dstroi.width - srcroi.width)*cn);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* tab = _tab.data();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ = dstroi.width - srcroi.width - ବାମ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା bottom = dstroi.height - srcroi.height - ଶୀର୍ଷ_2;

    ପାଇଁ( i = 0; i < ବାମ; i++ )
    {
        j = cv::borderInterpolate(i - ବାମ, srcroi.width, borderType)*cn;
        ପାଇଁ( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    ପାଇଁ( i = 0; i < ଡାହାଣ; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        ପାଇଁ( k = 0; k < cn; k++ )
            tab[(i+ବାମ)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    ବାମ *= cn;
    ଡାହାଣ *= cn;

    uchar* dstInner = dst + dststep*ଶୀର୍ଷ_2 + ବାମ*elemSize;

    ପାଇଁ( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        ଯଦି( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        ଯଦି( intMode )
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* isrc = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)src;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା* idstInner = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dstInner;
            ପାଇଁ( j = 0; j < ବାମ; j++ )
                idstInner[j - ବାମ] = isrc[tab[j]];
            ପାଇଁ( j = 0; j < ଡାହାଣ; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + ବାମ]];
        }
        ଅନ୍ୟଥା
        {
            ପାଇଁ( j = 0; j < ବାମ; j++ )
                dstInner[j - ବାମ] = src[tab[j]];
            ପାଇଁ( j = 0; j < ଡାହାଣ; j++ )
                dstInner[j + srcroi.width] = src[tab[j + ବାମ]];
        }
    }

    dstroi.width *= elemSize;

    ପାଇଁ( i = 0; i < ଶୀର୍ଷ_2; i++ )
    {
        j = cv::borderInterpolate(i - ଶୀର୍ଷ_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (ଶୀର୍ଷ_2+j)*dststep, dstroi.width);
    }

    dst += dststep*ଶୀର୍ଷ_2;
    ପାଇଁ( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


ଶୂନ୍ୟ copyMakeConstBorder_8u( ଅଚଳ uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶୀର୍ଷ_2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ଅଚଳ uchar* value )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ = dstroi.width - srcroi.width - ବାମ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା bottom = dstroi.height - srcroi.height - ଶୀର୍ଷ_2;

    ପାଇଁ( i = 0; i < dstroi.width; i++ )
    {
        ପାଇଁ( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    ବାମ *= cn;
    ଡାହାଣ *= cn;

    uchar* dstInner = dst + dststep*ଶୀର୍ଷ_2 + ବାମ;

    ପାଇଁ( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        ଯଦି( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - ବାମ, constBuf, ବାମ );
        memcpy( dstInner + srcroi.width, constBuf, ଡାହାଣ );
    }

    ପାଇଁ( i = 0; i < ଶୀର୍ଷ_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (ଶୀର୍ଷ_2 + srcroi.height) * dststep;
    ପାଇଁ( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

ନାମାକାଶ cv {

ସ୍ଥିର ତାର୍କିକ ocl_copyMakeBorder( ନିବେଶ_Array _src, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶୀର୍ଷ_2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bottom,
                                ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା borderType, ଅଚଳ Scalar& value )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ତାର୍କିକ isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    ଯଦି ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ ଅକ୍ଷର * ଅଚଳ borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarcn = cn == 3 ? 4 : cn;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src = _src.getUMat();
    ଯଦି( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtop = std::min(ofs.y, ଶୀର୍ଷ_2);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dleft = std::min(ofs.x, ବାମ);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dright = std::min(wholeSize.width - src.cols - ofs.x, ଡାହାଣ);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ଶୀର୍ଷ_2 -= dtop;
        ବାମ -= dleft;
        bottom -= dbottom;
        ଡାହାଣ -= dright;
    }

    _dst.create(src.rows + ଶୀର୍ଷ_2 + bottom, src.cols + ବାମ + ଡାହାଣ, type);
    UMat dst = _dst.getUMat();

    ଯଦି (ଶୀର୍ଷ_2 == 0 && ବାମ == 0 && bottom == 0 && ଡାହାଣ == 0)
    {
        ଯଦି(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           ଶୀର୍ଷ_2, ବାମ, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

}
#endif

#ifdef HAVE_IPP
ନାମାକାଶ cv {

ସ୍ଥିର ତାର୍କିକ ipp_copyMakeBorder( Mat &_src, Mat &_dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶୀର୍ଷ_2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bottom,
                                ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _borderType, ଅଚଳ Scalar& value )
{
#ଯଦି defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(ବାମ, ଶୀର୍ଷ_2, ଡାହାଣ, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    ଯଦି((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)borderType == -1)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି(_src.dims > 2)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    Rect dstRect(borderSize.ବାମ, borderSize.ଶୀର୍ଷ_2,
        _dst.cols - borderSize.ଡାହାଣ - borderSize.ବାମ,
        _dst.rows - borderSize.bottom - borderSize.ଶୀର୍ଷ_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    ଫେରନ୍ତୁ CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#ଅନ୍ୟଥା
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(ଶୀର୍ଷ_2); CV_UNUSED(bottom); CV_UNUSED(ବାମ); CV_UNUSED(ଡାହାଣ);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}
}
#endif

ଶୂନ୍ୟ cv::copyMakeBorder( ନିବେଶ_Array _src, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶୀର୍ଷ_2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bottom,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା borderType, ଅଚଳ Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( ଶୀର୍ଷ_2 >= 0 && bottom >= 0 && ବାମ >= 0 && ଡାହାଣ >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + ଶୀର୍ଷ_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + ବାମ + ଡାହାଣ <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, ଶୀର୍ଷ_2, bottom, ବାମ, ଡାହାଣ, borderType, value))
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src.type();

    ଯଦି( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtop = std::min(ofs.y, ଶୀର୍ଷ_2);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dleft = std::min(ofs.x, ବାମ);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dright = std::min(wholeSize.width - src.cols - ofs.x, ଡାହାଣ);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ଶୀର୍ଷ_2 -= dtop;
        ବାମ -= dleft;
        bottom -= dbottom;
        ଡାହାଣ -= dright;
    }

    _dst.create( src.rows + ଶୀର୍ଷ_2 + bottom, src.cols + ବାମ + ଡାହାଣ, type );
    Mat dst = _dst.getMat();

    ଯଦି(ଶୀର୍ଷ_2 == 0 && ବାମ == 0 && bottom == 0 && ଡାହାଣ == 0)
    {
        ଯଦି(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        ଫେରନ୍ତୁ;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, ଶୀର୍ଷ_2, bottom, ବାମ, ଡାହାଣ, borderType, value))

    ଯଦି( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           ଶୀର୍ଷ_2, ବାମ, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize(), borderType );
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = src.channels(), cn1 = cn;
        AutoBuffer<ଦ୍ୱିଗୁଣ> buf(cn);
        ଯଦି( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                ଶୀର୍ଷ_2, ବାମ, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
