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

#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"

namespace cv
{

ढाँचा <typename T> अटल अंतरभूत
व्योम scalar_To_Raw_तथ्य_2(स्थिर Scalar& s, T * स्थिर buf, स्थिर पूर्णांक cn, स्थिर पूर्णांक unroll_to)
{
    पूर्णांक i = 0;
    क्रम(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    क्रम(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

व्योम scalar_To_Raw_तथ्य(स्थिर Scalar& s, व्योम* _buf, पूर्णांक type, पूर्णांक unroll_to)
{
    CV_INSTRUMENT_REGION();

    स्थिर पूर्णांक depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    चयन(depth)
    {
    हाल CV_8U:
        scalar_To_Raw_तथ्य_2(s, (uchar*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_8S:
        scalar_To_Raw_तथ्य_2(s, (schar*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_Bool:
        scalar_To_Raw_तथ्य_2(s, (bool*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_16U:
        scalar_To_Raw_तथ्य_2(s, (ushort*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_16S:
        scalar_To_Raw_तथ्य_2(s, (लघु*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_16F:
        scalar_To_Raw_तथ्य_2(s, (hfloat*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_16BF:
        scalar_To_Raw_तथ्य_2(s, (bfloat*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_8F_E4M3FN:
        scalar_To_Raw_तथ्य_2(s, (fp8_t*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_8F_E4M3FNUZ:
        scalar_To_Raw_तथ्य_2(s, (fp8a_t*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_32U:
        scalar_To_Raw_तथ्य_2(s, (अचिन्हित*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_32S:
        scalar_To_Raw_तथ्य_2(s, (पूर्णांक*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_32F:
        scalar_To_Raw_तथ्य_2(s, (भग्न*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_64U:
        scalar_To_Raw_तथ्य_2(s, (uint64_t*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_64S:
        scalar_To_Raw_तथ्य_2(s, (int64_t*)_buf, cn, unroll_to);
        अवरोध;
    हाल CV_64F:
        scalar_To_Raw_तथ्य_2(s, (द्विगुन*)_buf, cn, unroll_to);
        अवरोध;
    शेष:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

व्योम convertAndUnrollScalar( स्थिर Mat& sc, पूर्णांक buftype, uchar* scbuf, माप_प्रकार blocksize )
{
    पूर्णांक scn = (पूर्णांक)sc.योग(), cn = CV_MAT_CN(buftype);
    माप_प्रकार esz = CV_ELEM_आकार(buftype);
    द्विआधारी_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, आकार(std::min(cn, scn), 1), 0);
    // unroll the scalar
    अगर( scn < cn )
    {
        CV_Assert( scn == 1 );
        माप_प्रकार esz1 = CV_ELEM_SIZE1(buftype);
        क्रम( माप_प्रकार i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    क्रम( माप_प्रकार i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

ढाँचा<typename T> अटल व्योम
copyMask_(स्थिर uchar* _src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, आकार size)
{
    क्रम( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिर T* src = (स्थिर T*)_src;
        T* dst = (T*)_dst;
        पूर्णांक x = 0;
         #अगर CV_ENABLE_UNROLLED
        क्रम( ; x <= size.width - 4; x += 4 )
        {
            अगर( mask[x] )
                dst[x] = src[x];
            अगर( mask[x+1] )
                dst[x+1] = src[x+1];
            अगर( mask[x+2] )
                dst[x+2] = src[x+2];
            अगर( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #पूर्ण_अगर
        क्रम( ; x < size.width; x++ )
            अगर( mask[x] )
                dst[x] = src[x];
    }
}

ढाँचा<> व्योम
copyMask_<uchar>(स्थिर uchar* _src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, आकार size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (पूर्णांक)sstep, _dst, (पूर्णांक)dstep, ippi_आकार(size), mask, (पूर्णांक)mstep) >= 0)

    क्रम( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिर uchar* src = (स्थिर uchar*)_src;
        uchar* dst = (uchar*)_dst;
        पूर्णांक x = 0;
        #अगर (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            क्रम( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_load(src  + x),
                        v_dst   = vx_load(dst  + x),
                        v_nmask = v_eq(vx_load(mask + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #पूर्ण_अगर
        क्रम( ; x < size.width; x++ )
            अगर( mask[x] )
                dst[x] = src[x];
    }
}

ढाँचा<> व्योम
copyMask_<Vec3b>(स्थिर uchar* _src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, आकार size)
{
    क्रम( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिर uchar* src = (स्थिर uchar*)_src;
        uchar* dst = (uchar*)_dst;
        पूर्णांक x = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
        क्रम( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
#पूर्ण_अगर
        क्रम( ; x < size.width; x++ )
            अगर( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

ढाँचा<> व्योम
copyMask_<ushort>(स्थिर uchar* _src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, आकार size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (स्थिर Ipp16u *)_src, (पूर्णांक)sstep, (Ipp16u *)_dst, (पूर्णांक)dstep, ippi_आकार(size), mask, (पूर्णांक)mstep) >= 0)

    क्रम( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिर ushort* src = (स्थिर ushort*)_src;
        ushort* dst = (ushort*)_dst;
        पूर्णांक x = 0;
        #अगर (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            क्रम( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        #पूर्ण_अगर
        क्रम( ; x < size.width; x++ )
            अगर( mask[x] )
                dst[x] = src[x];
    }
}

ढाँचा<> व्योम
copyMask_<Vec3s>(स्थिर uchar* _src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, आकार size)
{
    क्रम( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिर ushort* src = (स्थिर ushort*)_src;
        ushort* dst = (ushort*)_dst;
        पूर्णांक x = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
        क्रम( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
#पूर्ण_अगर
        क्रम( ; x < size.width; x++ )
            अगर( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

ढाँचा<> व्योम
copyMask_<पूर्णांक>(स्थिर uchar* _src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, आकार size)
{
    क्रम( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिर पूर्णांक* src = (स्थिर पूर्णांक*)_src;
        पूर्णांक* dst = (पूर्णांक*)_dst;
        पूर्णांक x = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
        क्रम (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
#पूर्ण_अगर
        क्रम (; x < size.width; x++)
            अगर ( mask[x] )
                dst[x] = src[x];
    }
}

अटल व्योम
copyMaskGeneric(स्थिर uchar* _src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, आकार size, व्योम* _esz)
{
    माप_प्रकार k, esz = *(माप_प्रकार*)_esz;
    क्रम( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिर uchar* src = _src;
        uchar* dst = _dst;
        पूर्णांक x = 0;
        क्रम( ; x < size.width; x++, src += esz, dst += esz )
        {
            अगर( !mask[x] )
                जारी;
            क्रम( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#घोषणा DEF_COPY_MASK(suffix, type) \
अटल व्योम copyMask##suffix(स्थिर uchar* src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, \
                             uchar* dst, माप_प्रकार dstep, आकार size, व्योम*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#अगर defined HAVE_IPP
#घोषणा DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
अटल व्योम copyMask##suffix(स्थिर uchar* src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, \
                             uchar* dst, माप_प्रकार dstep, आकार size, व्योम*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (स्थिर ipptype *)src, (पूर्णांक)sstep, (ipptype *)dst, (पूर्णांक)dstep, ippi_आकार(size), (स्थिर Ipp8u *)mask, (पूर्णांक)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#अन्यथा
#घोषणा DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
अटल व्योम copyMask##suffix(स्थिर uchar* src, माप_प्रकार sstep, स्थिर uchar* mask, माप_प्रकार mstep, \
                             uchar* dst, माप_प्रकार dstep, आकार size, व्योम*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#पूर्ण_अगर

#अगर IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#अन्यथा
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#पूर्ण_अगर
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, पूर्णांक, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

द्विआधारी_Func copyMaskTab[] =
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

द्विआधारी_Func getCopyMaskFunc(माप_प्रकार esz)
{
    वापस esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
व्योम Mat::copyTo( OutputArray _dst ) स्थिर
{
    CV_INSTRUMENT_REGION();

#अगर_घोषित HAVE_CUDA
    अगर (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*यह);
        वापस;
    }
#पूर्ण_अगर

    पूर्णांक stype = type();
    पूर्णांक dtype = _dst.type();
    अगर( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        वापस;
    }

    अगर( dims == 0 && empty() )
    {
        _dst.release();
        व्योम* obj = _dst.getObj();
        अगर (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        अन्यथा अगर (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        अन्यथा अगर (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        वापस;
    }

    bool allowTransposed = dims == 1 ||
        _dst.kind() == निवेश_Array_2::STD_VECTOR ||
        (_dst.fixed_आकार() && _dst.dims() == 1);
    अगर( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        अगर (empty())
            वापस;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != शून्य);
        माप_प्रकार i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_आकार(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        क्रम( i = 0; i < (माप_प्रकार)dims; i++ )
            sz[i] = size.p[i];
        पूर्णांक lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, तथ्य, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        वापस;
    }

    अगर( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        अगर( तथ्य == dst.तथ्य )
            वापस;

        अगर( rows > 0 && cols > 0 )
        {
            Mat src = *यह;
            आकार sz = getContinuousSize2D(src, dst, (पूर्णांक)elem_आकार());
            CV_CheckGE(sz.width, 0, "");

            स्थिर uchar* sptr = src.तथ्य;
            uchar* dptr = dst.तथ्य;

#अगर IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (पूर्णांक)src.step, dptr, (पूर्णांक)dst.step, ippi_आकार_L(sz.width, sz.height)) >= 0)
#पूर्ण_अगर

            क्रम (; sz.height--; sptr += src.step, dptr += dst.step)
                स_नकल(dptr, sptr, sz.width);
        }
        वापस;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    अगर( तथ्य == dst.तथ्य )
        वापस;

    अगर( योग() != 0 )
    {
        स्थिर Mat* arrays[] = { यह, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        माप_प्रकार sz = it.size*elem_आकार();

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            स_नकल(ptrs[1], ptrs[0], sz);
    }
}

#अगर_घोषित HAVE_IPP
अटल bool ipp_copyTo(स्थिर Mat &src, Mat &dst, स्थिर Mat &mask)
{
#अगर_घोषित HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    अगर(mask.channels() > 1 || mask.depth() != CV_8U)
        वापस false;

    अगर (src.dims <= 2)
    {
        Ippi_आकार size = ippi_आकार(src.size());
        वापस CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (पूर्णांक)src.step, dst.ptr(), (पूर्णांक)dst.step, size, (पूर्णांक)src.elemSize1(), src.channels(), mask.ptr(), (पूर्णांक)mask.step) >= 0;
    }
    अन्यथा
    {
        स्थिर Mat      *arrays[] = {&src, &dst, &mask, शून्य};
        uchar          *ptrs[3]  = {शून्य};
        NAryMatIterator it(arrays, ptrs);

        Ippi_आकार size = ippi_आकार(it.size, 1);

        क्रम (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            अगर(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (पूर्णांक)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                वापस false;
        }
        वापस true;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    वापस false;
#पूर्ण_अगर
}
#पूर्ण_अगर

व्योम Mat::copyTo( OutputArray _dst, निवेश_Array _mask ) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    अगर( !mask.तथ्य )
    {
        copyTo(_dst);
        वापस;
    }

    पूर्णांक cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    bool colorMask = mcn > 1;
    अगर( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        अगर (dst.तथ्य != dst0.तथ्य) // re-allocation happened
        {
#अगर_घोषित OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#पूर्ण_अगर
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*यह, dst, mask))
    अगर ( यह->dims <= 2 ) {
        अगर ( यह->size() == dst.size() && यह->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, यह->तथ्य, यह->step, dst.तथ्य, dst.step, यह->cols, यह->rows, यह->type(), mask.तथ्य, mask.step, mask.type());
        }
    }
    अन्यथा अगर ( यह->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        माप_प्रकार sz = यह->योग();
        अगर (sz < पूर्णांक_उच्च) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, यह->तथ्य, 0, dst.तथ्य, 0, (पूर्णांक)sz, 1, यह->type(), mask.तथ्य, 0, mask.type());
        }
    }

    माप_प्रकार esz = colorMask ? elemSize1() : elem_आकार();
    द्विआधारी_Func copymask = getCopyMaskFunc(esz);

    अगर( dims <= 2 )
    {
        Mat src = *यह;
        आकार sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.तथ्य, src.step, mask.तथ्य, mask.step, dst.तथ्य, dst.step, sz, &esz);
        वापस;
    }

    स्थिर Mat* arrays[] = { यह, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    आकार sz((पूर्णांक)(it.size*mcn), 1);

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
व्योम Mat::copyAt( OutputArray _dst ) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
व्योम Mat::copyAt( OutputArray _dst, निवेश_Array _mask ) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


अटल bool can_apply_memset(स्थिर Mat &mat, स्थिर Scalar &s, पूर्णांक &fill_मूल्य)
{
    // check if depth is 1 byte.
    चयन (mat.depth())
    {
    हाल CV_8U: fill_मूल्य = saturate_cast<uchar>( s.val[0] ); अवरोध;
    हाल CV_8S: fill_मूल्य = saturate_cast<schar>( s.val[0] ); अवरोध;
    शेष: वापस false;
    }

    // check if all element is same.
    स्थिर int64* is = (स्थिर int64*)&s.val[0];
    चयन (mat.channels())
    {
    हाल 1: वापस true;
    हाल 2: वापस (is[0] == is[1]);
    हाल 3: वापस (is[0] == is[1] && is[1] == is[2]);
    हाल 4: वापस (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    शेष: वापस false;
    }
}

Mat& Mat::चालक = (स्थिर Scalar& s)
{
    CV_INSTRUMENT_REGION();

    अगर (यह->empty())
        वापस *यह;

    स्थिर Mat* arrays[] = { यह };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    माप_प्रकार elsize = it.size*elem_आकार();
    स्थिर int64* is = (स्थिर int64*)&s.val[0];

    अगर( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            स_रखो( dptr, 0, elsize );
    }
    अन्यथा
    {
        पूर्णांक fill_मूल्य = 0;
        अगर ( can_apply_memset(*यह, s, fill_मूल्य) )
        {
            क्रम (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
                स_रखो(dptr, fill_मूल्य, elsize);
            वापस *यह;
        }

        अगर( it.nplanes > 0 )
        {
            द्विगुन scalar[12];
            scalar_To_Raw_तथ्य(s, scalar, type(), 12);
            माप_प्रकार block_आकार = 12*elemSize1();

            क्रम( माप_प्रकार j = 0; j < elsize; j += block_आकार )
            {
                माप_प्रकार sz = MIN(block_आकार, elsize - j);
                CV_Assert(sz <= माप(scalar));
                स_नकल( dptr + j, scalar, sz );
            }
        }

        क्रम( माप_प्रकार i = 1; i < it.nplanes; i++ )
        {
            ++it;
            स_नकल( dptr, तथ्य, elsize );
        }
    }
    वापस *यह;
}

#अगर_घोषित HAVE_IPP
अटल bool ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#अगर_घोषित HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    अगर(mask.empty())
        वापस false;

    अगर(mask.depth() != CV_8U || mask.channels() > 1)
        वापस false;

    अगर(dst.channels() > 4)
        वापस false;

    अगर (dst.depth() == CV_32F)
    {
        क्रम (पूर्णांक i = 0; i < (पूर्णांक)(_val.योग()); i++)
        {
            भग्न v = (भग्न)(_val.at<द्विगुन>(i));  // cast to float
            अगर (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                वापस false;
        }
    }

    अगर(dst.dims <= 2)
    {
        Ippi_आकार            size     = ippi_आकार(dst.size());
        Ipp_तथ्य_Type         तथ्य_Type = ippi_Get_तथ्य_Type(dst.depth());
        ::ipp::Iw_मूल्य_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        वापस CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (पूर्णांक)dst.step, size, तथ्य_Type, dst.channels(), mask.ptr(), (पूर्णांक)mask.step) >= 0;
    }
    अन्यथा
    {
        स्थिर Mat      *arrays[] = {&dst, mask.empty()?शून्य:&mask, शून्य};
        uchar          *ptrs[2]  = {शून्य};
        NAryMatIterator it(arrays, ptrs);

        Ippi_आकार            size     = {(पूर्णांक)it.size, 1};
        Ipp_तथ्य_Type         तथ्य_Type = ippi_Get_तथ्य_Type(dst.depth());
        ::ipp::Iw_मूल्य_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            अगर(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, तथ्य_Type, dst.channels(), ptrs[1], 0) < 0)
                वापस false;
        }
        वापस true;
    }
#अन्यथा
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    वापस false;
#पूर्ण_अगर
}
#पूर्ण_अगर

Mat& Mat::setTo(निवेश_Array मूल्य, निवेश_Array _mask)
{
    CV_INSTRUMENT_REGION();

    अगर( empty() )
        वापस *यह;

    Mat value = मूल्य.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), मूल्य.kind(), निवेश_Array_2::MAT ));
    पूर्णांक cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*यह, value, mask), *यह)

    माप_प्रकार esz = mcn > 1 ? elemSize1() : elem_आकार();
    द्विआधारी_Func copymask = getCopyMaskFunc(esz);

    स्थिर Mat* arrays[] = { यह, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक totalsz = (पूर्णांक)it.size*mcn;
    पूर्णांक blockSize0 = std::min(totalsz, (पूर्णांक)((BLOCK_आकार + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_बफर<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.तथ्य(), (पूर्णांक)माप(द्विगुन));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( पूर्णांक j = 0; j < totalsz; j += blockSize0 )
        {
            आकार sz(std::min(blockSize0, totalsz - j), 1);
            माप_प्रकार block_आकार = sz.width*esz;
            अगर( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            अन्यथा
                स_नकल(ptrs[0], scbuf, block_आकार);
            ptrs[0] += block_आकार;
        }
    }
    वापस *यह;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    अगर( empty() )
        वापस *यह;

    माप_प्रकार esz = elem_आकार();

    स्थिर Mat* arrays[] = { यह, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        स_रखो(ptrs[0], 0, esz*it.size);
    वापस *यह;
}


#अगर defined HAVE_OPENCL && !defined __APPLE__

अटल bool ocl_repeat(निवेश_Array _src, पूर्णांक ny, पूर्णांक nx, OutputArray _dst)
{
    अगर (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        वापस true;
    }

    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    अगर (k.empty())
        वापस false;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_आकार(dst));

    माप_प्रकार globalsize[] = { (माप_प्रकार)src.cols * cn / kercn, ((माप_प्रकार)src.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

व्योम repeat(निवेश_Array _src, पूर्णांक ny, पूर्णांक nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    आकार ssize = _src.size();
    अगर( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        वापस;
    }
    CV_Assert( (int64_t)ssize.height * ny <= पूर्णांक_उच्च && (int64_t)ssize.width * nx <= पूर्णांक_उच्च );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#अगर !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#पूर्ण_अगर

    Mat src = _src.getMat(), dst = _dst.getMat();
    आकार dsize = dst.size();
    पूर्णांक esz = (पूर्णांक)src.elem_आकार();
    पूर्णांक x, y;
    ssize.width *= esz; dsize.width *= esz;

    क्रम( y = 0; y < ssize.height; y++ )
    {
        क्रम( x = 0; x < dsize.width; x += ssize.width )
            स_नकल( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    क्रम( ; y < dsize.height; y++ )
        स_नकल( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(स्थिर Mat& src, पूर्णांक ny, पूर्णांक nx)
{
    अगर( nx == 1 && ny == 1 )
        वापस src;
    Mat dst;
    repeat(src, ny, nx, dst);
    वापस dst;
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
पूर्णांक cv::borderInterpolate( पूर्णांक p, पूर्णांक len, पूर्णांक borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#अगर_घोषित CV_STATIC_ANALYSIS
    अगर(p >= 0 && p < len)
#अन्यथा
    अगर( (अचिन्हित)p < (अचिन्हित)len )
#पूर्ण_अगर
        ;
    अन्यथा अगर( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    अन्यथा अगर( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        पूर्णांक delta = borderType == BORDER_REFLECT_101;
        अगर( len == 1 )
            वापस 0;
        करो
        {
            अगर( p < 0 )
                p = -p - 1 + delta;
            अन्यथा
                p = len - 1 - (p - len) - delta;
        }
#अगर_घोषित CV_STATIC_ANALYSIS
        जबतक(p < 0 || p >= len);
#अन्यथा
        जबतक( (अचिन्हित)p >= (अचिन्हित)len );
#पूर्ण_अगर
    }
    अन्यथा अगर( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        अगर( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            अगर( p64 < 0 )
                p64 += len;
            p = (पूर्णांक)p64;
        }
    }
    अन्यथा अगर( borderType == BORDER_CONSTANT )
        p = -1;
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    वापस p;
}

namespace
{

व्योम copyMakeBorder_8u( स्थिर uchar* src, माप_प्रकार srcstep, cv::आकार srcroi,
                        uchar* dst, माप_प्रकार dststep, cv::आकार dstroi,
                        पूर्णांक शीर्ष_2, पूर्णांक बाँए, पूर्णांक cn, पूर्णांक borderType )
{
    स्थिर पूर्णांक isz = (पूर्णांक)माप(पूर्णांक);
    पूर्णांक i, j, k, elem_आकार = 1;
    bool intMode = false;

    अगर( (cn | srcstep | dststep | (माप_प्रकार)src | (माप_प्रकार)dst) % isz == 0 )
    {
        cn /= isz;
        elem_आकार = isz;
        intMode = true;
    }

    cv::Auto_बफर<पूर्णांक> _tab((dstroi.width - srcroi.width)*cn);
    पूर्णांक* tab = _tab.तथ्य();
    पूर्णांक दाँए = dstroi.width - srcroi.width - बाँए;
    पूर्णांक bottom = dstroi.height - srcroi.height - शीर्ष_2;

    क्रम( i = 0; i < बाँए; i++ )
    {
        j = cv::borderInterpolate(i - बाँए, srcroi.width, borderType)*cn;
        क्रम( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    क्रम( i = 0; i < दाँए; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        क्रम( k = 0; k < cn; k++ )
            tab[(i+बाँए)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    बाँए *= cn;
    दाँए *= cn;

    uchar* dstInner = dst + dststep*शीर्ष_2 + बाँए*elem_आकार;

    क्रम( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        अगर( dstInner != src )
            स_नकल(dstInner, src, srcroi.width*elem_आकार);

        अगर( intMode )
        {
            स्थिर पूर्णांक* isrc = (पूर्णांक*)src;
            पूर्णांक* idstInner = (पूर्णांक*)dstInner;
            क्रम( j = 0; j < बाँए; j++ )
                idstInner[j - बाँए] = isrc[tab[j]];
            क्रम( j = 0; j < दाँए; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + बाँए]];
        }
        अन्यथा
        {
            क्रम( j = 0; j < बाँए; j++ )
                dstInner[j - बाँए] = src[tab[j]];
            क्रम( j = 0; j < दाँए; j++ )
                dstInner[j + srcroi.width] = src[tab[j + बाँए]];
        }
    }

    dstroi.width *= elem_आकार;

    क्रम( i = 0; i < शीर्ष_2; i++ )
    {
        j = cv::borderInterpolate(i - शीर्ष_2, srcroi.height, borderType);
        स_नकल(dst + i*dststep, dst + (शीर्ष_2+j)*dststep, dstroi.width);
    }

    dst += dststep*शीर्ष_2;
    क्रम( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        स_नकल(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


व्योम copyMakeConstBorder_8u( स्थिर uchar* src, माप_प्रकार srcstep, cv::आकार srcroi,
                             uchar* dst, माप_प्रकार dststep, cv::आकार dstroi,
                             पूर्णांक शीर्ष_2, पूर्णांक बाँए, पूर्णांक cn, स्थिर uchar* value )
{
    पूर्णांक i, j;
    cv::Auto_बफर<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.तथ्य();
    पूर्णांक दाँए = dstroi.width - srcroi.width - बाँए;
    पूर्णांक bottom = dstroi.height - srcroi.height - शीर्ष_2;

    क्रम( i = 0; i < dstroi.width; i++ )
    {
        क्रम( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    बाँए *= cn;
    दाँए *= cn;

    uchar* dstInner = dst + dststep*शीर्ष_2 + बाँए;

    क्रम( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        अगर( dstInner != src )
            स_नकल( dstInner, src, srcroi.width );
        स_नकल( dstInner - बाँए, constBuf, बाँए );
        स_नकल( dstInner + srcroi.width, constBuf, दाँए );
    }

    क्रम( i = 0; i < शीर्ष_2; i++ )
        स_नकल(dst + i * dststep, constBuf, dstroi.width);

    dst += (शीर्ष_2 + srcroi.height) * dststep;
    क्रम( i = 0; i < bottom; i++ )
        स_नकल(dst + i * dststep, constBuf, dstroi.width);
}

}

#अगर_घोषित HAVE_OPENCL

namespace cv {

अटल bool ocl_copyMakeBorder( निवेश_Array _src, OutputArray _dst, पूर्णांक शीर्ष_2, पूर्णांक bottom,
                                पूर्णांक बाँए, पूर्णांक दाँए, पूर्णांक borderType, स्थिर Scalar& value )
{
    पूर्णांक type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    bool isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    अगर ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        वापस false;

    स्थिर अक्षर * स्थिर borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    पूर्णांक scalarcn = cn == 3 ? 4 : cn;
    पूर्णांक sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    अगर (k.empty())
        वापस false;

    UMat src = _src.getUMat();
    अगर( src.isSubmatrix() && !isolated )
    {
        आकार whole_आकार;
        Point ofs;
        src.locateROI(whole_आकार, ofs);
        पूर्णांक dtop = std::min(ofs.y, शीर्ष_2);
        पूर्णांक dbottom = std::min(whole_आकार.height - src.rows - ofs.y, bottom);
        पूर्णांक dleft = std::min(ofs.x, बाँए);
        पूर्णांक dright = std::min(whole_आकार.width - src.cols - ofs.x, दाँए);
        src.adjustROI(dtop, dbottom, dleft, dright);
        शीर्ष_2 -= dtop;
        बाँए -= dleft;
        bottom -= dbottom;
        दाँए -= dright;
    }

    _dst.create(src.rows + शीर्ष_2 + bottom, src.cols + बाँए + दाँए, type);
    UMat dst = _dst.getUMat();

    अगर (शीर्ष_2 == 0 && बाँए == 0 && bottom == 0 && दाँए == 0)
    {
        अगर(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        वापस true;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           शीर्ष_2, बाँए, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst.cols, ((माप_प्रकार)dst.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

}
#पूर्ण_अगर

#अगर_घोषित HAVE_IPP
namespace cv {

अटल bool ipp_copyMakeBorder( Mat &_src, Mat &_dst, पूर्णांक शीर्ष_2, पूर्णांक bottom,
                                पूर्णांक बाँए, पूर्णांक दाँए, पूर्णांक _borderType, स्थिर Scalar& value )
{
#अगर defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_आकार border_आकार(बाँए, शीर्ष_2, दाँए, bottom);
    ::ipp::Iwi_आकार       size(_src.cols, _src.rows);
    Ipp_तथ्य_Type          तथ्य_Type   = ippi_Get_तथ्य_Type(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    अगर((पूर्णांक)borderType == -1)
        वापस false;

    अगर(_src.dims > 2)
        वापस false;

    Rect dstRect(border_आकार.बाँए, border_आकार.शीर्ष_2,
        _dst.cols - border_आकार.दाँए - border_आकार.बाँए,
        _dst.rows - border_आकार.bottom - border_आकार.शीर्ष_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    वापस CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, तथ्य_Type, _src.channels(), border_आकार, borderType, &value[0]) >= 0;
#अन्यथा
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(शीर्ष_2); CV_UNUSED(bottom); CV_UNUSED(बाँए); CV_UNUSED(दाँए);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    वापस false;
#पूर्ण_अगर
}
}
#पूर्ण_अगर

व्योम cv::copyMakeBorder( निवेश_Array _src, OutputArray _dst, पूर्णांक शीर्ष_2, पूर्णांक bottom,
                         पूर्णांक बाँए, पूर्णांक दाँए, पूर्णांक borderType, स्थिर Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( शीर्ष_2 >= 0 && bottom >= 0 && बाँए >= 0 && दाँए >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + शीर्ष_2 + bottom <= पूर्णांक_उच्च );
    CV_Assert( (int64_t)src.cols + बाँए + दाँए <= पूर्णांक_उच्च );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, शीर्ष_2, bottom, बाँए, दाँए, borderType, value))
    पूर्णांक type = src.type();

    अगर( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        आकार whole_आकार;
        Point ofs;
        src.locateROI(whole_आकार, ofs);
        पूर्णांक dtop = std::min(ofs.y, शीर्ष_2);
        पूर्णांक dbottom = std::min(whole_आकार.height - src.rows - ofs.y, bottom);
        पूर्णांक dleft = std::min(ofs.x, बाँए);
        पूर्णांक dright = std::min(whole_आकार.width - src.cols - ofs.x, दाँए);
        src.adjustROI(dtop, dbottom, dleft, dright);
        शीर्ष_2 -= dtop;
        बाँए -= dleft;
        bottom -= dbottom;
        दाँए -= dright;
    }

    _dst.create( src.rows + शीर्ष_2 + bottom, src.cols + बाँए + दाँए, type );
    Mat dst = _dst.getMat();

    अगर(शीर्ष_2 == 0 && बाँए == 0 && bottom == 0 && दाँए == 0)
    {
        अगर(src.तथ्य != dst.तथ्य || src.step != dst.step)
            src.copyTo(dst);
        वापस;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, शीर्ष_2, bottom, बाँए, दाँए, borderType, value))

    अगर( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           शीर्ष_2, बाँए, (पूर्णांक)src.elem_आकार(), borderType );
    अन्यथा
    {
        पूर्णांक cn = src.channels(), cn1 = cn;
        Auto_बफर<द्विगुन> buf(cn);
        अगर( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_To_Raw_तथ्य(value, buf.तथ्य(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                शीर्ष_2, बाँए, (पूर्णांक)src.elem_आकार(), (uchar*)buf.तथ्य() );
    }
}
/* End of file. */
