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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

नामाकाश cv
{

प्रतिरूपम् <प्रकारनाम T> स्थिर अन्तःस्थ
शून्य scalar_गन्तव्यम्_Raw_दत्त_2(ध्रुव Scalar& s, T * ध्रुव buf, ध्रुव पूर्णाङ्क cn, ध्रुव पूर्णाङ्क unroll_गन्तव्यम्)
{
    पूर्णाङ्क i = 0;
    प्रति(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    प्रति(; i < unroll_गन्तव्यम्; i++)
        buf[i] = buf[i-cn];
}

शून्य scalar_गन्तव्यम्_Raw_दत्त(ध्रुव Scalar& s, शून्य* _buf, पूर्णाङ्क type, पूर्णाङ्क unroll_गन्तव्यम्)
{
    CV_INSTRUMENT_REGION();

    ध्रुव पूर्णाङ्क गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    विकल्प(गभीरता)
    {
    पक्ष CV_8U:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (uchar*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_8S:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (schar*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_Bool:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (तार्किक*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_16U:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (ushort*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_16S:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (ह्रस्व*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_16F:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (hfloat*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_16BF:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (bfloat*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_8F_E4M3FN:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (fp8_t*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_8F_E4M3FNUZ:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (fp8a_t*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_32U:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (अचिह्नित*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_32S:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (पूर्णाङ्क*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_32F:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (भिन्न*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_64U:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (uint64_t*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_64S:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (int64_t*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    पक्ष CV_64F:
        scalar_गन्तव्यम्_Raw_दत्त_2(s, (द्विगुण*)_buf, cn, unroll_गन्तव्यम्);
        भङ्ग;
    स्वभाव:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

शून्य convertAndUnrollScalar( ध्रुव Mat& sc, पूर्णाङ्क buftype, uchar* scbuf, size_t blocksize )
{
    पूर्णाङ्क scn = (पूर्णाङ्क)sc.योग(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_आकार(buftype);
    द्विमान_Func cvtFn = getConvertFunc(sc.गभीरता(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, आकार(std::min(cn, scn), 1), 0);
    // unroll the scalar
    यदि( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        प्रति( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    प्रति( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
copy_आवरणम्(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, uchar* _dst, size_t dstep, आकार size)
{
    प्रति( ; size.height--; आवरणम् += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव T* src = (ध्रुव T*)_src;
        T* dst = (T*)_dst;
        पूर्णाङ्क x = 0;
         #यदि CV_ENABLE_UNROLLED
        प्रति( ; x <= size.विस्तारः - 4; x += 4 )
        {
            यदि( आवरणम्[x] )
                dst[x] = src[x];
            यदि( आवरणम्[x+1] )
                dst[x+1] = src[x+1];
            यदि( आवरणम्[x+2] )
                dst[x+2] = src[x+2];
            यदि( आवरणम्[x+3] )
                dst[x+3] = src[x+3];
        }
        #यदि_अन्तः
        प्रति( ; x < size.विस्तारः; x++ )
            यदि( आवरणम्[x] )
                dst[x] = src[x];
    }
}

प्रतिरूपम्<> शून्य
copy_आवरणम्<uchar>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, uchar* _dst, size_t dstep, आकार size)
{
    CV_IPP_सततम्_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (पूर्णाङ्क)sstep, _dst, (पूर्णाङ्क)dstep, ippi_आकार(size), आवरणम्, (पूर्णाङ्क)mstep) >= 0)

    प्रति( ; size.height--; आवरणम् += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव uchar* src = (ध्रुव uchar*)_src;
        uchar* dst = (uchar*)_dst;
        पूर्णाङ्क x = 0;
        #यदि (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            प्रति( ; x <= size.विस्तारः - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_आरोपय(src  + x),
                        v_dst   = vx_आरोपय(dst  + x),
                        v_nmask = v_eq(vx_आरोपय(आवरणम् + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #यदि_अन्तः
        प्रति( ; x < size.विस्तारः; x++ )
            यदि( आवरणम्[x] )
                dst[x] = src[x];
    }
}

प्रतिरूपम्<> शून्य
copy_आवरणम्<Vec3b>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, uchar* _dst, size_t dstep, आकार size)
{
    प्रति( ; size.height--; आवरणम् += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव uchar* src = (ध्रुव uchar*)_src;
        uchar* dst = (uchar*)_dst;
        पूर्णाङ्क x = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        प्रति( ; x <= size.विस्तारः - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_आरोपय(आवरणम् + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_आरोपय_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_आरोपय_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#यदि_अन्तः
        प्रति( ; x < size.विस्तारः; x++ )
            यदि( आवरणम्[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

प्रतिरूपम्<> शून्य
copy_आवरणम्<ushort>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, uchar* _dst, size_t dstep, आकार size)
{
    CV_IPP_सततम्_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ध्रुव Ipp16u *)_src, (पूर्णाङ्क)sstep, (Ipp16u *)_dst, (पूर्णाङ्क)dstep, ippi_आकार(size), आवरणम्, (पूर्णाङ्क)mstep) >= 0)

    प्रति( ; size.height--; आवरणम् += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव ushort* src = (ध्रुव ushort*)_src;
        ushort* dst = (ushort*)_dst;
        पूर्णाङ्क x = 0;
        #यदि (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            प्रति( ; x <= size.विस्तारः - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_आरोपय(src + x), v_src2 = vx_आरोपय(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_आरोपय(dst + x), v_dst2 = vx_आरोपय(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_आरोपय(आवरणम् + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #यदि_अन्तः
        प्रति( ; x < size.विस्तारः; x++ )
            यदि( आवरणम्[x] )
                dst[x] = src[x];
    }
}

प्रतिरूपम्<> शून्य
copy_आवरणम्<Vec3s>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, uchar* _dst, size_t dstep, आकार size)
{
    प्रति( ; size.height--; आवरणम् += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव ushort* src = (ध्रुव ushort*)_src;
        ushort* dst = (ushort*)_dst;
        पूर्णाङ्क x = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        प्रति( ; x <= size.विस्तारः - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_आरोपय(आवरणम् + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_आरोपय_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_आरोपय_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_आरोपय_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_आरोपय_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

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
#यदि_अन्तः
        प्रति( ; x < size.विस्तारः; x++ )
            यदि( आवरणम्[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

प्रतिरूपम्<> शून्य
copy_आवरणम्<पूर्णाङ्क>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, uchar* _dst, size_t dstep, आकार size)
{
    प्रति( ; size.height--; आवरणम् += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव पूर्णाङ्क* src = (ध्रुव पूर्णाङ्क*)_src;
        पूर्णाङ्क* dst = (पूर्णाङ्क*)_dst;
        पूर्णाङ्क x = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        प्रति (; x <= size.विस्तारः - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_आरोपय(src + x), v_dst0 = vx_आरोपय(dst + x);
            v_int32 v_src1 = vx_आरोपय(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_आरोपय(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_आरोपय(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_आरोपय(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_आरोपय(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_आरोपय(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_आरोपय(आवरणम् + x), vx_setzero_u8());
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
#यदि_अन्तः
        प्रति (; x < size.विस्तारः; x++)
            यदि ( आवरणम्[x] )
                dst[x] = src[x];
    }
}

स्थिर शून्य
copy_आवरणम्_Generic(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, uchar* _dst, size_t dstep, आकार size, शून्य* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    प्रति( ; size.height--; आवरणम् += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव uchar* src = _src;
        uchar* dst = _dst;
        पूर्णाङ्क x = 0;
        प्रति( ; x < size.विस्तारः; x++, src += esz, dst += esz )
        {
            यदि( !आवरणम्[x] )
                अनुवर्त;
            प्रति( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#परिभाषय DEF_COPY_आवरणम्(suffix, type) \
स्थिर शून्य copy_आवरणम्_2##suffix(ध्रुव uchar* src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, \
                             uchar* dst, size_t dstep, आकार size, शून्य*) \
{ \
    copy_आवरणम्<type>(src, sstep, आवरणम्, mstep, dst, dstep, size); \
}

#यदि defined HAVE_IPP
#परिभाषय DEF_COPY_आवरणम्_F(suffix, type, ippfavor, ipptype) \
स्थिर शून्य copy_आवरणम्_2##suffix(ध्रुव uchar* src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, \
                             uchar* dst, size_t dstep, आकार size, शून्य*) \
{ \
    CV_IPP_सततम्_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ध्रुव ipptype *)src, (पूर्णाङ्क)sstep, (ipptype *)dst, (पूर्णाङ्क)dstep, ippi_आकार(size), (ध्रुव Ipp8u *)आवरणम्, (पूर्णाङ्क)mstep) >= 0)\
    copy_आवरणम्<type>(src, sstep, आवरणम्, mstep, dst, dstep, size); \
}
#अन्यथा
#परिभाषय DEF_COPY_आवरणम्_F(suffix, type, ippfavor, ipptype) \
स्थिर शून्य copy_आवरणम्_2##suffix(ध्रुव uchar* src, size_t sstep, ध्रुव uchar* आवरणम्, size_t mstep, \
                             uchar* dst, size_t dstep, आकार size, शून्य*) \
{ \
    copy_आवरणम्<type>(src, sstep, आवरणम्, mstep, dst, dstep, size); \
}
#यदि_अन्तः

#यदि IPP_संस्करणम्_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_आवरणम्(32sC3, Vec3i)
DEF_COPY_आवरणम्(8uC3, Vec3b)
#अन्यथा
DEF_COPY_आवरणम्_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_आवरणम्_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#यदि_अन्तः
DEF_COPY_आवरणम्(8u, uchar)
DEF_COPY_आवरणम्(16u, ushort)
DEF_COPY_आवरणम्_F(32s, पूर्णाङ्क, 32s_C1MR, Ipp32s)
DEF_COPY_आवरणम्_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_आवरणम्(32sC2, Vec2i)
DEF_COPY_आवरणम्_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_आवरणम्(32sC6, Vec6i)
DEF_COPY_आवरणम्(32sC8, Vec8i)

द्विमान_Func copy_आवरणम्_Tab[] =
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

द्विमान_Func get_Copy_आवरणम्_Func(size_t esz)
{
    प्रत्यागम esz <= 32 && copy_आवरणम्_Tab[esz] ? copy_आवरणम्_Tab[esz] : copy_आवरणम्_Generic;
}

/* dst = src */
शून्य Mat::copy_गन्तव्यम्( OutputArray _dst ) ध्रुव
{
    CV_INSTRUMENT_REGION();

#यदि_परिभाषितम् HAVE_CUDA
    यदि (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*एतत्);
        प्रत्यागम;
    }
#यदि_अन्तः

    पूर्णाङ्क stype = type();
    पूर्णाङ्क dtype = _dst.type();
    यदि( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_गन्तव्यम्( _dst, dtype );
        प्रत्यागम;
    }

    यदि( dims == 0 && empty() )
    {
        _dst.release();
        शून्य* obj = _dst.getObj();
        यदि (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        अन्यथा यदि (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        अन्यथा यदि (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        प्रत्यागम;
    }

    तार्किक allowTransposed = dims == 1 ||
        _dst.विधा() == निवेशः_Array_2::STD_सदिशम् ||
        (_dst.fixed_आकार() && _dst.dims() == 1);
    यदि( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        यदि (empty())
            प्रत्यागम;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != रिक्त);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_आकार(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        प्रति( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        पूर्णाङ्क lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, दत्त, std::max(dims, 1), sz, dstofs, dst.चरणः.p, चरणः.p);
        प्रत्यागम;
    }

    यदि( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        यदि( दत्त == dst.दत्त )
            प्रत्यागम;

        यदि( पङ्क्तिकाः > 0 && स्तम्भसङ्ख्या > 0 )
        {
            Mat src = *एतत्;
            आकार sz = getContinuousSize2D(src, dst, (पूर्णाङ्क)elem_आकार());
            CV_CheckGE(sz.विस्तारः, 0, "");

            ध्रुव uchar* sptr = src.दत्त;
            uchar* dptr = dst.दत्त;

#यदि IPP_संस्करणम्_X100 >= 201700
            CV_IPP_सततम्_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (पूर्णाङ्क)src.चरणः, dptr, (पूर्णाङ्क)dst.चरणः, ippi_आकार_L(sz.विस्तारः, sz.height)) >= 0)
#यदि_अन्तः

            प्रति (; sz.height--; sptr += src.चरणः, dptr += dst.चरणः)
                स्मृतिं_प्रतिलिख(dptr, sptr, sz.विस्तारः);
        }
        प्रत्यागम;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    यदि( दत्त == dst.दत्त )
        प्रत्यागम;

    यदि( योग() != 0 )
    {
        ध्रुव Mat* arrays[] = { एतत्, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_आकार();

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            स्मृतिं_प्रतिलिख(ptrs[1], ptrs[0], sz);
    }
}

#यदि_परिभाषितम् HAVE_IPP
स्थिर तार्किक ipp_copy_गन्तव्यम्(ध्रुव Mat &src, Mat &dst, ध्रुव Mat &आवरणम्)
{
#यदि_परिभाषितम् HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    यदि(आवरणम्.channels() > 1 || आवरणम्.गभीरता() != CV_8U)
        प्रत्यागम असत्य;

    यदि (src.dims <= 2)
    {
        Ippi_आकार size = ippi_आकार(src.size());
        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwi_Copy_आवरणम्, src.ptr(), (पूर्णाङ्क)src.चरणः, dst.ptr(), (पूर्णाङ्क)dst.चरणः, size, (पूर्णाङ्क)src.elemSize1(), src.channels(), आवरणम्.ptr(), (पूर्णाङ्क)आवरणम्.चरणः) >= 0;
    }
    अन्यथा
    {
        ध्रुव Mat      *arrays[] = {&src, &dst, &आवरणम्, रिक्त};
        uchar          *ptrs[3]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        Ippi_आकार size = ippi_आकार(it.size, 1);

        प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwi_Copy_आवरणम्, ptrs[0], 0, ptrs[1], 0, size, (पूर्णाङ्क)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }
#अन्यथा
    CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(dst); CV_अप्रयुक्तम्(आवरणम्);
    प्रत्यागम असत्य;
#यदि_अन्तः
}
#यदि_अन्तः

शून्य Mat::copy_गन्तव्यम्( OutputArray _dst, निवेशः_Array आवरणम्_2 ) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat आवरणम् = आवरणम्_2.getMat();
    यदि( !आवरणम्.दत्त )
    {
        copy_गन्तव्यम्(_dst);
        प्रत्यागम;
    }

    पूर्णाङ्क cn = channels(), mcn = आवरणम्.channels();
    CV_Assert( (आवरणम्.गभीरता() == CV_8U || आवरणम्.गभीरता() == CV_8S || आवरणम्.गभीरता() == CV_Bool) && (mcn == 1 || mcn == cn) );
    तार्किक color_आवरणम् = mcn > 1;
    यदि( dims <= 2 )
    {
        CV_Assert( size() == आवरणम्.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        यदि (dst.दत्त != dst0.दत्त) // re-allocation happened
        {
#यदि_परिभाषितम् OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#यदि_अन्तः
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_सततम्_FAST(ipp_copy_गन्तव्यम्(*एतत्, dst, आवरणम्))
    यदि ( एतत्->dims <= 2 ) {
        यदि ( एतत्->size() == dst.size() && एतत्->size() == dst.size() ) {
            CALL_HAL(copy_गन्तव्यम्_आवरणम्, cv_hal_copy_गन्तव्यम्_Masked, एतत्->दत्त, एतत्->चरणः, dst.दत्त, dst.चरणः, एतत्->स्तम्भसङ्ख्या, एतत्->पङ्क्तिकाः, एतत्->type(), आवरणम्.दत्त, आवरणम्.चरणः, आवरणम्.type());
        }
    }
    अन्यथा यदि ( एतत्->isContinuous() && dst.isContinuous() && आवरणम्.isContinuous() )
    {
        size_t sz = एतत्->योग();
        यदि (sz < अधिकतम_पूर्णाङ्कः) {
            CALL_HAL(copy_गन्तव्यम्_आवरणम्, cv_hal_copy_गन्तव्यम्_Masked, एतत्->दत्त, 0, dst.दत्त, 0, (पूर्णाङ्क)sz, 1, एतत्->type(), आवरणम्.दत्त, 0, आवरणम्.type());
        }
    }

    size_t esz = color_आवरणम् ? elemSize1() : elem_आकार();
    द्विमान_Func copymask = get_Copy_आवरणम्_Func(esz);

    यदि( dims <= 2 )
    {
        Mat src = *एतत्;
        आकार sz = getContinuousSize2D(src, dst, आवरणम्, mcn);
        copymask(src.दत्त, src.चरणः, आवरणम्.दत्त, आवरणम्.चरणः, dst.दत्त, dst.चरणः, sz, &esz);
        प्रत्यागम;
    }

    ध्रुव Mat* arrays[] = { एतत्, &dst, &आवरणम्, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    आकार sz((पूर्णाङ्क)(it.size*mcn), 1);

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
शून्य Mat::copyAt( OutputArray _dst ) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_गन्तव्यम्(_dst);
}
शून्य Mat::copyAt( OutputArray _dst, निवेशः_Array आवरणम्_2 ) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_गन्तव्यम्(_dst, आवरणम्_2);
}


स्थिर तार्किक can_apply_memset(ध्रुव Mat &mat, ध्रुव Scalar &s, पूर्णाङ्क &fill_मानम्)
{
    // check if depth is 1 byte.
    विकल्प (mat.गभीरता())
    {
    पक्ष CV_8U: fill_मानम् = saturate_cast<uchar>( s.val[0] ); भङ्ग;
    पक्ष CV_8S: fill_मानम् = saturate_cast<schar>( s.val[0] ); भङ्ग;
    स्वभाव: प्रत्यागम असत्य;
    }

    // check if all element is same.
    ध्रुव int64* is = (ध्रुव int64*)&s.val[0];
    विकल्प (mat.channels())
    {
    पक्ष 1: प्रत्यागम सत्य;
    पक्ष 2: प्रत्यागम (is[0] == is[1]);
    पक्ष 3: प्रत्यागम (is[0] == is[1] && is[1] == is[2]);
    पक्ष 4: प्रत्यागम (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    स्वभाव: प्रत्यागम असत्य;
    }
}

Mat& Mat::संक्रियकः = (ध्रुव Scalar& s)
{
    CV_INSTRUMENT_REGION();

    यदि (एतत्->empty())
        प्रत्यागम *एतत्;

    ध्रुव Mat* arrays[] = { एतत् };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elem_आकार();
    ध्रुव int64* is = (ध्रुव int64*)&s.val[0];

    यदि( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            स्मृतिं_पूरय( dptr, 0, elsize );
    }
    अन्यथा
    {
        पूर्णाङ्क fill_मानम् = 0;
        यदि ( can_apply_memset(*एतत्, s, fill_मानम्) )
        {
            प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
                स्मृतिं_पूरय(dptr, fill_मानम्, elsize);
            प्रत्यागम *एतत्;
        }

        यदि( it.nplanes > 0 )
        {
            द्विगुण scalar[12];
            scalar_गन्तव्यम्_Raw_दत्त(s, scalar, type(), 12);
            size_t खण्डम्_आकार_2 = 12*elemSize1();

            प्रति( size_t j = 0; j < elsize; j += खण्डम्_आकार_2 )
            {
                size_t sz = MIN(खण्डम्_आकार_2, elsize - j);
                CV_Assert(sz <= परिमाण(scalar));
                स्मृतिं_प्रतिलिख( dptr + j, scalar, sz );
            }
        }

        प्रति( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            स्मृतिं_प्रतिलिख( dptr, दत्त, elsize );
        }
    }
    प्रत्यागम *एतत्;
}

#यदि_परिभाषितम् HAVE_IPP
स्थिर तार्किक ipp_Mat_समुच्चयः_गन्तव्यम्_Mat(Mat &dst, Mat &_val, Mat &आवरणम्)
{
#यदि_परिभाषितम् HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    यदि(आवरणम्.empty())
        प्रत्यागम असत्य;

    यदि(आवरणम्.गभीरता() != CV_8U || आवरणम्.channels() > 1)
        प्रत्यागम असत्य;

    यदि(dst.channels() > 4)
        प्रत्यागम असत्य;

    यदि (dst.गभीरता() == CV_32F)
    {
        प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)(_val.योग()); i++)
        {
            भिन्न v = (भिन्न)(_val.at<द्विगुण>(i));  // cast to float
            यदि (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                प्रत्यागम असत्य;
        }
    }

    यदि(dst.dims <= 2)
    {
        Ippi_आकार            size     = ippi_आकार(dst.size());
        Ipp_दत्त_Type         दत्त_Type = ippi_Get_दत्त_Type(dst.गभीरता());
        ::ipp::Iw_मानम्_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwi_समुच्चयः_आवरणम्, s, dst.ptr(), (पूर्णाङ्क)dst.चरणः, size, दत्त_Type, dst.channels(), आवरणम्.ptr(), (पूर्णाङ्क)आवरणम्.चरणः) >= 0;
    }
    अन्यथा
    {
        ध्रुव Mat      *arrays[] = {&dst, आवरणम्.empty()?रिक्त:&आवरणम्, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        Ippi_आकार            size     = {(पूर्णाङ्क)it.size, 1};
        Ipp_दत्त_Type         दत्त_Type = ippi_Get_दत्त_Type(dst.गभीरता());
        ::ipp::Iw_मानम्_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwi_समुच्चयः_आवरणम्, s, ptrs[0], 0, size, दत्त_Type, dst.channels(), ptrs[1], 0) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }
#अन्यथा
    CV_अप्रयुक्तम्(dst); CV_अप्रयुक्तम्(_val); CV_अप्रयुक्तम्(आवरणम्);
    प्रत्यागम असत्य;
#यदि_अन्तः
}
#यदि_अन्तः

Mat& Mat::समुच्चयः_गन्तव्यम्(निवेशः_Array मानम्, निवेशः_Array आवरणम्_2)
{
    CV_INSTRUMENT_REGION();

    यदि( empty() )
        प्रत्यागम *एतत्;

    Mat value = मानम्.getMat(), आवरणम् = आवरणम्_2.getMat();

    CV_Assert( checkScalar(value, type(), मानम्.विधा(), निवेशः_Array_2::MAT ));
    पूर्णाङ्क cn = channels(), mcn = आवरणम्.channels();
    CV_Assert( आवरणम्.empty() || ((आवरणम्.गभीरता() == CV_8U || आवरणम्.गभीरता() == CV_8S || आवरणम्.गभीरता() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == आवरणम्.size) );

    CV_IPP_सततम्_FAST(ipp_Mat_समुच्चयः_गन्तव्यम्_Mat(*एतत्, value, आवरणम्), *एतत्)

    size_t esz = mcn > 1 ? elemSize1() : elem_आकार();
    द्विमान_Func copymask = get_Copy_आवरणम्_Func(esz);

    ध्रुव Mat* arrays[] = { एतत्, !आवरणम्.empty() ? &आवरणम् : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क totalsz = (पूर्णाङ्क)it.size*mcn;
    पूर्णाङ्क खण्डम्_Size0 = std::min(totalsz, (पूर्णाङ्क)((खण्डम्_आकार + esz-1)/esz));
    खण्डम्_Size0 -= खण्डम्_Size0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_बफर<uchar> _scbuf(खण्डम्_Size0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.दत्त(), (पूर्णाङ्क)परिमाण(द्विगुण));
    convertAndUnrollScalar( value, type(), scbuf, खण्डम्_Size0/mcn );

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( पूर्णाङ्क j = 0; j < totalsz; j += खण्डम्_Size0 )
        {
            आकार sz(std::min(खण्डम्_Size0, totalsz - j), 1);
            size_t खण्डम्_आकार_2 = sz.विस्तारः*esz;
            यदि( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.विस्तारः;
            }
            अन्यथा
                स्मृतिं_प्रतिलिख(ptrs[0], scbuf, खण्डम्_आकार_2);
            ptrs[0] += खण्डम्_आकार_2;
        }
    }
    प्रत्यागम *एतत्;
}


Mat& Mat::समुच्चयः_Zero()
{
    CV_INSTRUMENT_REGION();

    यदि( empty() )
        प्रत्यागम *एतत्;

    size_t esz = elem_आकार();

    ध्रुव Mat* arrays[] = { एतत्, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        स्मृतिं_पूरय(ptrs[0], 0, esz*it.size);
    प्रत्यागम *एतत्;
}


#यदि defined HAVE_OPENCL && !defined __APPLE__

स्थिर तार्किक ocl_repeat(निवेशः_Array _src, पूर्णाङ्क ny, पूर्णाङ्क nx, OutputArray _dst)
{
    यदि (ny == 1 && nx == 1)
    {
        _src.copy_गन्तव्यम्(_dst);
        प्रत्यागम सत्य;
    }

    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type),
            पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predict_Optimal_सदिशम्_विस्तारः(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memop_Type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, kercn)),
                         nx, ny, पङ्क्तिकाः_प्रत्येकम्_WI, kercn));
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.तर्काः(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_आकार(dst));

    size_t globalsize[] = { (size_t)src.स्तम्भसङ्ख्या * cn / kercn, ((size_t)src.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

#यदि_अन्तः

शून्य repeat(निवेशः_Array _src, पूर्णाङ्क ny, पूर्णाङ्क nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    आकार ssize = _src.size();
    यदि( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        प्रत्यागम;
    }
    CV_Assert( (int64_t)ssize.height * ny <= अधिकतम_पूर्णाङ्कः && (int64_t)ssize.विस्तारः * nx <= अधिकतम_पूर्णाङ्कः );
    _dst.create(ssize.height*ny, ssize.विस्तारः*nx, _src.type());

#यदि !defined __APPLE__
    CV_OCL_सततम्(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#यदि_अन्तः

    Mat src = _src.getMat(), dst = _dst.getMat();
    आकार dsize = dst.size();
    पूर्णाङ्क esz = (पूर्णाङ्क)src.elem_आकार();
    पूर्णाङ्क x, y;
    ssize.विस्तारः *= esz; dsize.विस्तारः *= esz;

    प्रति( y = 0; y < ssize.height; y++ )
    {
        प्रति( x = 0; x < dsize.विस्तारः; x += ssize.विस्तारः )
            स्मृतिं_प्रतिलिख( dst.ptr(y) + x, src.ptr(y), ssize.विस्तारः );
    }

    प्रति( ; y < dsize.height; y++ )
        स्मृतिं_प्रतिलिख( dst.ptr(y), dst.ptr(y - ssize.height), dsize.विस्तारः );
}

Mat repeat(ध्रुव Mat& src, पूर्णाङ्क ny, पूर्णाङ्क nx)
{
    यदि( nx == 1 && ny == 1 )
        प्रत्यागम src;
    Mat dst;
    repeat(src, ny, nx, dst);
    प्रत्यागम dst;
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
पूर्णाङ्क cv::borderInterpolate( पूर्णाङ्क p, पूर्णाङ्क len, पूर्णाङ्क borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#यदि_परिभाषितम् CV_STATIC_ANALYSIS
    यदि(p >= 0 && p < len)
#अन्यथा
    यदि( (अचिह्नित)p < (अचिह्नित)len )
#यदि_अन्तः
        ;
    अन्यथा यदि( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    अन्यथा यदि( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        पूर्णाङ्क delta = borderType == BORDER_REFLECT_101;
        यदि( len == 1 )
            प्रत्यागम 0;
        कुरु
        {
            यदि( p < 0 )
                p = -p - 1 + delta;
            अन्यथा
                p = len - 1 - (p - len) - delta;
        }
#यदि_परिभाषितम् CV_STATIC_ANALYSIS
        यावत्(p < 0 || p >= len);
#अन्यथा
        यावत्( (अचिह्नित)p >= (अचिह्नित)len );
#यदि_अन्तः
    }
    अन्यथा यदि( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        यदि( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            यदि( p64 < 0 )
                p64 += len;
            p = (पूर्णाङ्क)p64;
        }
    }
    अन्यथा यदि( borderType == BORDER_CONSTANT )
        p = -1;
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    प्रत्यागम p;
}

नामाकाश
{

शून्य copyMakeBorder_8u( ध्रुव uchar* src, size_t srcstep, cv::आकार srcroi,
                        uchar* dst, size_t dststep, cv::आकार dstroi,
                        पूर्णाङ्क शिखरम्_2, पूर्णाङ्क वामतः, पूर्णाङ्क cn, पूर्णाङ्क borderType )
{
    ध्रुव पूर्णाङ्क isz = (पूर्णाङ्क)परिमाण(पूर्णाङ्क);
    पूर्णाङ्क i, j, k, elem_आकार = 1;
    तार्किक int_बहुलकम् = असत्य;

    यदि( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_आकार = isz;
        int_बहुलकम् = सत्य;
    }

    cv::Auto_बफर<पूर्णाङ्क> _tab((dstroi.विस्तारः - srcroi.विस्तारः)*cn);
    पूर्णाङ्क* tab = _tab.दत्त();
    पूर्णाङ्क दक्षिणतः = dstroi.विस्तारः - srcroi.विस्तारः - वामतः;
    पूर्णाङ्क bottom = dstroi.height - srcroi.height - शिखरम्_2;

    प्रति( i = 0; i < वामतः; i++ )
    {
        j = cv::borderInterpolate(i - वामतः, srcroi.विस्तारः, borderType)*cn;
        प्रति( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    प्रति( i = 0; i < दक्षिणतः; i++ )
    {
        j = cv::borderInterpolate(srcroi.विस्तारः + i, srcroi.विस्तारः, borderType)*cn;
        प्रति( k = 0; k < cn; k++ )
            tab[(i+वामतः)*cn + k] = j + k;
    }

    srcroi.विस्तारः *= cn;
    dstroi.विस्तारः *= cn;
    वामतः *= cn;
    दक्षिणतः *= cn;

    uchar* dstInner = dst + dststep*शिखरम्_2 + वामतः*elem_आकार;

    प्रति( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        यदि( dstInner != src )
            स्मृतिं_प्रतिलिख(dstInner, src, srcroi.विस्तारः*elem_आकार);

        यदि( int_बहुलकम् )
        {
            ध्रुव पूर्णाङ्क* isrc = (पूर्णाङ्क*)src;
            पूर्णाङ्क* idstInner = (पूर्णाङ्क*)dstInner;
            प्रति( j = 0; j < वामतः; j++ )
                idstInner[j - वामतः] = isrc[tab[j]];
            प्रति( j = 0; j < दक्षिणतः; j++ )
                idstInner[j + srcroi.विस्तारः] = isrc[tab[j + वामतः]];
        }
        अन्यथा
        {
            प्रति( j = 0; j < वामतः; j++ )
                dstInner[j - वामतः] = src[tab[j]];
            प्रति( j = 0; j < दक्षिणतः; j++ )
                dstInner[j + srcroi.विस्तारः] = src[tab[j + वामतः]];
        }
    }

    dstroi.विस्तारः *= elem_आकार;

    प्रति( i = 0; i < शिखरम्_2; i++ )
    {
        j = cv::borderInterpolate(i - शिखरम्_2, srcroi.height, borderType);
        स्मृतिं_प्रतिलिख(dst + i*dststep, dst + (शिखरम्_2+j)*dststep, dstroi.विस्तारः);
    }

    dst += dststep*शिखरम्_2;
    प्रति( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        स्मृतिं_प्रतिलिख(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.विस्तारः);
    }
}


शून्य copyMakeConstBorder_8u( ध्रुव uchar* src, size_t srcstep, cv::आकार srcroi,
                             uchar* dst, size_t dststep, cv::आकार dstroi,
                             पूर्णाङ्क शिखरम्_2, पूर्णाङ्क वामतः, पूर्णाङ्क cn, ध्रुव uchar* value )
{
    पूर्णाङ्क i, j;
    cv::Auto_बफर<uchar> _constBuf(dstroi.विस्तारः*cn);
    uchar* constBuf = _constBuf.दत्त();
    पूर्णाङ्क दक्षिणतः = dstroi.विस्तारः - srcroi.विस्तारः - वामतः;
    पूर्णाङ्क bottom = dstroi.height - srcroi.height - शिखरम्_2;

    प्रति( i = 0; i < dstroi.विस्तारः; i++ )
    {
        प्रति( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.विस्तारः *= cn;
    dstroi.विस्तारः *= cn;
    वामतः *= cn;
    दक्षिणतः *= cn;

    uchar* dstInner = dst + dststep*शिखरम्_2 + वामतः;

    प्रति( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        यदि( dstInner != src )
            स्मृतिं_प्रतिलिख( dstInner, src, srcroi.विस्तारः );
        स्मृतिं_प्रतिलिख( dstInner - वामतः, constBuf, वामतः );
        स्मृतिं_प्रतिलिख( dstInner + srcroi.विस्तारः, constBuf, दक्षिणतः );
    }

    प्रति( i = 0; i < शिखरम्_2; i++ )
        स्मृतिं_प्रतिलिख(dst + i * dststep, constBuf, dstroi.विस्तारः);

    dst += (शिखरम्_2 + srcroi.height) * dststep;
    प्रति( i = 0; i < bottom; i++ )
        स्मृतिं_प्रतिलिख(dst + i * dststep, constBuf, dstroi.विस्तारः);
}

}

#यदि_परिभाषितम् HAVE_OPENCL

नामाकाश cv {

स्थिर तार्किक ocl_copyMakeBorder( निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क शिखरम्_2, पूर्णाङ्क bottom,
                                पूर्णाङ्क वामतः, पूर्णाङ्क दक्षिणतः, पूर्णाङ्क borderType, ध्रुव Scalar& value )
{
    पूर्णाङ्क type = _src.type(), cn = CV_MAT_CN(type), गभीरता = CV_MAT_गभीरता(type),
            पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    तार्किक isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    यदि ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        प्रत्यागम असत्य;

    ध्रुव वर्ण * ध्रुव borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    पूर्णाङ्क scalarcn = cn == 3 ? 4 : cn;
    पूर्णाङ्क sctype = CV_MAKETYPE(गभीरता, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memop_Type_गन्तव्यम्_Str(type), borderMap[borderType],
                                 ocl::memop_Type_गन्तव्यम्_Str(गभीरता), cn,
                                 ocl::memop_Type_गन्तव्यम्_Str(sctype), पङ्क्तिकाः_प्रत्येकम्_WI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src = _src.getUMat();
    यदि( src.isSubmatrix() && !isolated )
    {
        आकार सम्पूर्णम्_आकार;
        Point ofs;
        src.locateROI(सम्पूर्णम्_आकार, ofs);
        पूर्णाङ्क dtop = std::min(ofs.y, शिखरम्_2);
        पूर्णाङ्क dbottom = std::min(सम्पूर्णम्_आकार.height - src.पङ्क्तिकाः - ofs.y, bottom);
        पूर्णाङ्क dleft = std::min(ofs.x, वामतः);
        पूर्णाङ्क dright = std::min(सम्पूर्णम्_आकार.विस्तारः - src.स्तम्भसङ्ख्या - ofs.x, दक्षिणतः);
        src.adjustROI(dtop, dbottom, dleft, dright);
        शिखरम्_2 -= dtop;
        वामतः -= dleft;
        bottom -= dbottom;
        दक्षिणतः -= dright;
    }

    _dst.create(src.पङ्क्तिकाः + शिखरम्_2 + bottom, src.स्तम्भसङ्ख्या + वामतः + दक्षिणतः, type);
    UMat dst = _dst.getUMat();

    यदि (शिखरम्_2 == 0 && वामतः == 0 && bottom == 0 && दक्षिणतः == 0)
    {
        यदि(src.u != dst.u || src.चरणः != dst.चरणः)
            src.copy_गन्तव्यम्(dst);
        प्रत्यागम सत्य;
    }

    k.तर्काः(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           शिखरम्_2, वामतः, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.स्तम्भसङ्ख्या, ((size_t)dst.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

}
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_IPP
नामाकाश cv {

स्थिर तार्किक ipp_copyMakeBorder( Mat &_src, Mat &_dst, पूर्णाङ्क शिखरम्_2, पूर्णाङ्क bottom,
                                पूर्णाङ्क वामतः, पूर्णाङ्क दक्षिणतः, पूर्णाङ्क _borderType, ध्रुव Scalar& value )
{
#यदि defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_आकार border_आकार(वामतः, शिखरम्_2, दक्षिणतः, bottom);
    ::ipp::Iwi_आकार       size(_src.स्तम्भसङ्ख्या, _src.पङ्क्तिकाः);
    Ipp_दत्त_Type          दत्त_Type   = ippi_Get_दत्त_Type(_src.गभीरता());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    यदि((पूर्णाङ्क)borderType == -1)
        प्रत्यागम असत्य;

    यदि(_src.dims > 2)
        प्रत्यागम असत्य;

    Rect dstRect(border_आकार.वामतः, border_आकार.शिखरम्_2,
        _dst.स्तम्भसङ्ख्या - border_आकार.दक्षिणतः - border_आकार.वामतः,
        _dst.पङ्क्तिकाः - border_आकार.bottom - border_आकार.शिखरम्_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->चरणः, subDst.ptr(), subDst.चरणः, size, दत्त_Type, _src.channels(), border_आकार, borderType, &value[0]) >= 0;
#अन्यथा
    CV_अप्रयुक्तम्(_src); CV_अप्रयुक्तम्(_dst); CV_अप्रयुक्तम्(शिखरम्_2); CV_अप्रयुक्तम्(bottom); CV_अप्रयुक्तम्(वामतः); CV_अप्रयुक्तम्(दक्षिणतः);
    CV_अप्रयुक्तम्(_borderType); CV_अप्रयुक्तम्(value);
    प्रत्यागम असत्य;
#यदि_अन्तः
}
}
#यदि_अन्तः

शून्य cv::copyMakeBorder( निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क शिखरम्_2, पूर्णाङ्क bottom,
                         पूर्णाङ्क वामतः, पूर्णाङ्क दक्षिणतः, पूर्णाङ्क borderType, ध्रुव Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( शिखरम्_2 >= 0 && bottom >= 0 && वामतः >= 0 && दक्षिणतः >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.पङ्क्तिकाः + शिखरम्_2 + bottom <= अधिकतम_पूर्णाङ्कः );
    CV_Assert( (int64_t)src.स्तम्भसङ्ख्या + वामतः + दक्षिणतः <= अधिकतम_पूर्णाङ्कः );

    CV_OCL_सततम्(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, शिखरम्_2, bottom, वामतः, दक्षिणतः, borderType, value))
    पूर्णाङ्क type = src.type();

    यदि( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        आकार सम्पूर्णम्_आकार;
        Point ofs;
        src.locateROI(सम्पूर्णम्_आकार, ofs);
        पूर्णाङ्क dtop = std::min(ofs.y, शिखरम्_2);
        पूर्णाङ्क dbottom = std::min(सम्पूर्णम्_आकार.height - src.पङ्क्तिकाः - ofs.y, bottom);
        पूर्णाङ्क dleft = std::min(ofs.x, वामतः);
        पूर्णाङ्क dright = std::min(सम्पूर्णम्_आकार.विस्तारः - src.स्तम्भसङ्ख्या - ofs.x, दक्षिणतः);
        src.adjustROI(dtop, dbottom, dleft, dright);
        शिखरम्_2 -= dtop;
        वामतः -= dleft;
        bottom -= dbottom;
        दक्षिणतः -= dright;
    }

    _dst.create( src.पङ्क्तिकाः + शिखरम्_2 + bottom, src.स्तम्भसङ्ख्या + वामतः + दक्षिणतः, type );
    Mat dst = _dst.getMat();

    यदि(शिखरम्_2 == 0 && वामतः == 0 && bottom == 0 && दक्षिणतः == 0)
    {
        यदि(src.दत्त != dst.दत्त || src.चरणः != dst.चरणः)
            src.copy_गन्तव्यम्(dst);
        प्रत्यागम;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_सततम्_FAST(ipp_copyMakeBorder(src, dst, शिखरम्_2, bottom, वामतः, दक्षिणतः, borderType, value))

    यदि( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.चरणः, src.size(),
                           dst.ptr(), dst.चरणः, dst.size(),
                           शिखरम्_2, वामतः, (पूर्णाङ्क)src.elem_आकार(), borderType );
    अन्यथा
    {
        पूर्णाङ्क cn = src.channels(), cn1 = cn;
        Auto_बफर<द्विगुण> buf(cn);
        यदि( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_गन्तव्यम्_Raw_दत्त(value, buf.दत्त(), CV_MAKETYPE(src.गभीरता(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.चरणः, src.size(),
                                dst.ptr(), dst.चरणः, dst.size(),
                                शिखरम्_2, वामतः, (पूर्णाङ्क)src.elem_आकार(), (uchar*)buf.दत्त() );
    }
}
/* End of file. */
