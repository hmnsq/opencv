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

ढाँचा <typename T> स्थिर ओळीत
शून्य scalarToRawData_(स्थिरांक Scalar& s, T * स्थिरांक buf, स्थिरांक पूर्णांक cn, स्थिरांक पूर्णांक unroll_to)
{
    पूर्णांक i = 0;
    साठी(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    साठी(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

शून्य scalarToRawData(स्थिरांक Scalar& s, शून्य* _buf, पूर्णांक type, पूर्णांक unroll_to)
{
    CV_INSTRUMENT_REGION();

    स्थिरांक पूर्णांक depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    निवड(depth)
    {
    स्थिती CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_Bool:
        scalarToRawData_(s, (तार्किक*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_16S:
        scalarToRawData_(s, (लघु*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_32U:
        scalarToRawData_(s, (अचिन्हित*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_32S:
        scalarToRawData_(s, (पूर्णांक*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_32F:
        scalarToRawData_(s, (दशांश*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        थांबा;
    स्थिती CV_64F:
        scalarToRawData_(s, (द्विगुण*)_buf, cn, unroll_to);
        थांबा;
    पूर्वनिर्धारित:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

शून्य convertAndUnrollScalar( स्थिरांक Mat& sc, पूर्णांक buftype, uchar* scbuf, माप_प्रकार blocksize )
{
    पूर्णांक scn = (पूर्णांक)sc.total(), cn = CV_MAT_CN(buftype);
    माप_प्रकार esz = CV_ELEM_SIZE(buftype);
    द्विमान_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    जर( scn < cn )
    {
        CV_Assert( scn == 1 );
        माप_प्रकार esz1 = CV_ELEM_SIZE1(buftype);
        साठी( माप_प्रकार i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    साठी( माप_प्रकार i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

ढाँचा<typename T> स्थिर शून्य
copyMask_(स्थिरांक uchar* _src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, Size size)
{
    साठी( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिरांक T* src = (स्थिरांक T*)_src;
        T* dst = (T*)_dst;
        पूर्णांक x = 0;
         #जर CV_ENABLE_UNROLLED
        साठी( ; x <= size.width - 4; x += 4 )
        {
            जर( mask[x] )
                dst[x] = src[x];
            जर( mask[x+1] )
                dst[x+1] = src[x+1];
            जर( mask[x+2] )
                dst[x+2] = src[x+2];
            जर( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        साठी( ; x < size.width; x++ )
            जर( mask[x] )
                dst[x] = src[x];
    }
}

ढाँचा<> शून्य
copyMask_<uchar>(स्थिरांक uchar* _src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (पूर्णांक)sstep, _dst, (पूर्णांक)dstep, ippiSize(size), mask, (पूर्णांक)mstep) >= 0)

    साठी( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिरांक uchar* src = (स्थिरांक uchar*)_src;
        uchar* dst = (uchar*)_dst;
        पूर्णांक x = 0;
        #जर (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            साठी( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        साठी( ; x < size.width; x++ )
            जर( mask[x] )
                dst[x] = src[x];
    }
}

ढाँचा<> शून्य
copyMask_<Vec3b>(स्थिरांक uchar* _src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, Size size)
{
    साठी( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिरांक uchar* src = (स्थिरांक uchar*)_src;
        uchar* dst = (uchar*)_dst;
        पूर्णांक x = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
        साठी( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        साठी( ; x < size.width; x++ )
            जर( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

ढाँचा<> शून्य
copyMask_<ushort>(स्थिरांक uchar* _src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (स्थिरांक Ipp16u *)_src, (पूर्णांक)sstep, (Ipp16u *)_dst, (पूर्णांक)dstep, ippiSize(size), mask, (पूर्णांक)mstep) >= 0)

    साठी( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिरांक ushort* src = (स्थिरांक ushort*)_src;
        ushort* dst = (ushort*)_dst;
        पूर्णांक x = 0;
        #जर (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            साठी( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        साठी( ; x < size.width; x++ )
            जर( mask[x] )
                dst[x] = src[x];
    }
}

ढाँचा<> शून्य
copyMask_<Vec3s>(स्थिरांक uchar* _src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, Size size)
{
    साठी( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिरांक ushort* src = (स्थिरांक ushort*)_src;
        ushort* dst = (ushort*)_dst;
        पूर्णांक x = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
        साठी( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        साठी( ; x < size.width; x++ )
            जर( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

ढाँचा<> शून्य
copyMask_<पूर्णांक>(स्थिरांक uchar* _src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, Size size)
{
    साठी( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिरांक पूर्णांक* src = (स्थिरांक पूर्णांक*)_src;
        पूर्णांक* dst = (पूर्णांक*)_dst;
        पूर्णांक x = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
        साठी (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        साठी (; x < size.width; x++)
            जर ( mask[x] )
                dst[x] = src[x];
    }
}

स्थिर शून्य
copyMaskGeneric(स्थिरांक uchar* _src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, uchar* _dst, माप_प्रकार dstep, Size size, शून्य* _esz)
{
    माप_प्रकार k, esz = *(माप_प्रकार*)_esz;
    साठी( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        स्थिरांक uchar* src = _src;
        uchar* dst = _dst;
        पूर्णांक x = 0;
        साठी( ; x < size.width; x++, src += esz, dst += esz )
        {
            जर( !mask[x] )
                सुरूठेवा;
            साठी( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
स्थिर शून्य copyMask##suffix(स्थिरांक uchar* src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, \
                             uchar* dst, माप_प्रकार dstep, Size size, शून्य*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#जर defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
स्थिर शून्य copyMask##suffix(स्थिरांक uchar* src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, \
                             uchar* dst, माप_प्रकार dstep, Size size, शून्य*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (स्थिरांक ipptype *)src, (पूर्णांक)sstep, (ipptype *)dst, (पूर्णांक)dstep, ippiSize(size), (स्थिरांक Ipp8u *)mask, (पूर्णांक)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#अन्यथा
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
स्थिर शून्य copyMask##suffix(स्थिरांक uchar* src, माप_प्रकार sstep, स्थिरांक uchar* mask, माप_प्रकार mstep, \
                             uchar* dst, माप_प्रकार dstep, Size size, शून्य*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#जर IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#अन्यथा
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, पूर्णांक, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

द्विमान_Func copyMaskTab[] =
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

द्विमान_Func getCopyMaskFunc(माप_प्रकार esz)
{
    परत esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
शून्य Mat::copyTo( OutputArray _dst ) स्थिरांक
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    जर (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*हे);
        परत;
    }
#endif

    पूर्णांक stype = type();
    पूर्णांक dtype = _dst.type();
    जर( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        परत;
    }

    जर( dims == 0 && empty() )
    {
        _dst.release();
        शून्य* obj = _dst.getObj();
        जर (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        अन्यथा जर (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        अन्यथा जर (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        परत;
    }

    तार्किक allowTransposed = dims == 1 ||
        _dst.kind() == आदान_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    जर( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        जर (empty())
            परत;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != रिक्त);
        माप_प्रकार i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        साठी( i = 0; i < (माप_प्रकार)dims; i++ )
            sz[i] = size.p[i];
        पूर्णांक lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        परत;
    }

    जर( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        जर( data == dst.data )
            परत;

        जर( rows > 0 && cols > 0 )
        {
            Mat src = *हे;
            Size sz = getContinuousSize2D(src, dst, (पूर्णांक)elemSize());
            CV_CheckGE(sz.width, 0, "");

            स्थिरांक uchar* sptr = src.data;
            uchar* dptr = dst.data;

#जर IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (पूर्णांक)src.step, dptr, (पूर्णांक)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            साठी (; sz.height--; sptr += src.step, dptr += dst.step)
                स_नकल(dptr, sptr, sz.width);
        }
        परत;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    जर( data == dst.data )
        परत;

    जर( total() != 0 )
    {
        स्थिरांक Mat* arrays[] = { हे, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        माप_प्रकार sz = it.size*elemSize();

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            स_नकल(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
स्थिर तार्किक ipp_copyTo(स्थिरांक Mat &src, Mat &dst, स्थिरांक Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    जर(mask.channels() > 1 || mask.depth() != CV_8U)
        परत असत्य;

    जर (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        परत CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (पूर्णांक)src.step, dst.ptr(), (पूर्णांक)dst.step, size, (पूर्णांक)src.elemSize1(), src.channels(), mask.ptr(), (पूर्णांक)mask.step) >= 0;
    }
    अन्यथा
    {
        स्थिरांक Mat      *arrays[] = {&src, &dst, &mask, रिक्त};
        uchar          *ptrs[3]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        साठी (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            जर(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (पूर्णांक)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                परत असत्य;
        }
        परत सत्य;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    परत असत्य;
#endif
}
#endif

शून्य Mat::copyTo( OutputArray _dst, आदान_Array _mask ) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    जर( !mask.data )
    {
        copyTo(_dst);
        परत;
    }

    पूर्णांक cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    तार्किक colorMask = mcn > 1;
    जर( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        जर (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*हे, dst, mask))
    जर ( हे->dims <= 2 ) {
        जर ( हे->size() == dst.size() && हे->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, हे->data, हे->step, dst.data, dst.step, हे->cols, हे->rows, हे->type(), mask.data, mask.step, mask.type());
        }
    }
    अन्यथा जर ( हे->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        माप_प्रकार sz = हे->total();
        जर (sz < पूर्णांक_उच्च) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, हे->data, 0, dst.data, 0, (पूर्णांक)sz, 1, हे->type(), mask.data, 0, mask.type());
        }
    }

    माप_प्रकार esz = colorMask ? elemSize1() : elemSize();
    द्विमान_Func copymask = getCopyMaskFunc(esz);

    जर( dims <= 2 )
    {
        Mat src = *हे;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        परत;
    }

    स्थिरांक Mat* arrays[] = { हे, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((पूर्णांक)(it.size*mcn), 1);

    साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
शून्य Mat::copyAt( OutputArray _dst ) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
शून्य Mat::copyAt( OutputArray _dst, आदान_Array _mask ) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


स्थिर तार्किक can_apply_memset(स्थिरांक Mat &mat, स्थिरांक Scalar &s, पूर्णांक &fill_मूल्य)
{
    // check if depth is 1 byte.
    निवड (mat.depth())
    {
    स्थिती CV_8U: fill_मूल्य = saturate_cast<uchar>( s.val[0] ); थांबा;
    स्थिती CV_8S: fill_मूल्य = saturate_cast<schar>( s.val[0] ); थांबा;
    पूर्वनिर्धारित: परत असत्य;
    }

    // check if all element is same.
    स्थिरांक int64* is = (स्थिरांक int64*)&s.val[0];
    निवड (mat.channels())
    {
    स्थिती 1: परत सत्य;
    स्थिती 2: परत (is[0] == is[1]);
    स्थिती 3: परत (is[0] == is[1] && is[1] == is[2]);
    स्थिती 4: परत (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    पूर्वनिर्धारित: परत असत्य;
    }
}

Mat& Mat::चालक = (स्थिरांक Scalar& s)
{
    CV_INSTRUMENT_REGION();

    जर (हे->empty())
        परत *हे;

    स्थिरांक Mat* arrays[] = { हे };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    माप_प्रकार elsize = it.size*elemSize();
    स्थिरांक int64* is = (स्थिरांक int64*)&s.val[0];

    जर( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            स_रखो( dptr, 0, elsize );
    }
    अन्यथा
    {
        पूर्णांक fill_मूल्य = 0;
        जर ( can_apply_memset(*हे, s, fill_मूल्य) )
        {
            साठी (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
                स_रखो(dptr, fill_मूल्य, elsize);
            परत *हे;
        }

        जर( it.nplanes > 0 )
        {
            द्विगुण scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            माप_प्रकार blockSize = 12*elemSize1();

            साठी( माप_प्रकार j = 0; j < elsize; j += blockSize )
            {
                माप_प्रकार sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= आकार(scalar));
                स_नकल( dptr + j, scalar, sz );
            }
        }

        साठी( माप_प्रकार i = 1; i < it.nplanes; i++ )
        {
            ++it;
            स_नकल( dptr, data, elsize );
        }
    }
    परत *हे;
}

#ifdef HAVE_IPP
स्थिर तार्किक ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    जर(mask.empty())
        परत असत्य;

    जर(mask.depth() != CV_8U || mask.channels() > 1)
        परत असत्य;

    जर(dst.channels() > 4)
        परत असत्य;

    जर (dst.depth() == CV_32F)
    {
        साठी (पूर्णांक i = 0; i < (पूर्णांक)(_val.total()); i++)
        {
            दशांश v = (दशांश)(_val.at<द्विगुण>(i));  // cast to float
            जर (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                परत असत्य;
        }
    }

    जर(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_मूल्य_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        परत CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (पूर्णांक)dst.step, size, dataType, dst.channels(), mask.ptr(), (पूर्णांक)mask.step) >= 0;
    }
    अन्यथा
    {
        स्थिरांक Mat      *arrays[] = {&dst, mask.empty()?रिक्त:&mask, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(पूर्णांक)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_मूल्य_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            जर(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                परत असत्य;
        }
        परत सत्य;
    }
#अन्यथा
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    परत असत्य;
#endif
}
#endif

Mat& Mat::setTo(आदान_Array मूल्य, आदान_Array _mask)
{
    CV_INSTRUMENT_REGION();

    जर( empty() )
        परत *हे;

    Mat value = मूल्य.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), मूल्य.kind(), आदान_Array_2::MAT ));
    पूर्णांक cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*हे, value, mask), *हे)

    माप_प्रकार esz = mcn > 1 ? elemSize1() : elemSize();
    द्विमान_Func copymask = getCopyMaskFunc(esz);

    स्थिरांक Mat* arrays[] = { हे, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक totalsz = (पूर्णांक)it.size*mcn;
    पूर्णांक blockSize0 = std::min(totalsz, (पूर्णांक)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (पूर्णांक)आकार(द्विगुण));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        साठी( पूर्णांक j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            माप_प्रकार blockSize = sz.width*esz;
            जर( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            अन्यथा
                स_नकल(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    परत *हे;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    जर( empty() )
        परत *हे;

    माप_प्रकार esz = elemSize();

    स्थिरांक Mat* arrays[] = { हे, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        स_रखो(ptrs[0], 0, esz*it.size);
    परत *हे;
}


#जर defined HAVE_OPENCL && !defined __APPLE__

स्थिर तार्किक ocl_repeat(आदान_Array _src, पूर्णांक ny, पूर्णांक nx, OutputArray _dst)
{
    जर (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        परत सत्य;
    }

    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    जर (k.empty())
        परत असत्य;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    माप_प्रकार globalsize[] = { (माप_प्रकार)src.cols * cn / kercn, ((माप_प्रकार)src.rows + rowsPerWI - 1) / rowsPerWI };
    परत k.run(2, globalsize, रिक्त, असत्य);
}

#endif

शून्य repeat(आदान_Array _src, पूर्णांक ny, पूर्णांक nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    जर( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        परत;
    }
    CV_Assert( (int64_t)ssize.height * ny <= पूर्णांक_उच्च && (int64_t)ssize.width * nx <= पूर्णांक_उच्च );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#जर !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    पूर्णांक esz = (पूर्णांक)src.elemSize();
    पूर्णांक x, y;
    ssize.width *= esz; dsize.width *= esz;

    साठी( y = 0; y < ssize.height; y++ )
    {
        साठी( x = 0; x < dsize.width; x += ssize.width )
            स_नकल( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    साठी( ; y < dsize.height; y++ )
        स_नकल( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(स्थिरांक Mat& src, पूर्णांक ny, पूर्णांक nx)
{
    जर( nx == 1 && ny == 1 )
        परत src;
    Mat dst;
    repeat(src, ny, nx, dst);
    परत dst;
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

#ifdef CV_STATIC_ANALYSIS
    जर(p >= 0 && p < len)
#अन्यथा
    जर( (अचिन्हित)p < (अचिन्हित)len )
#endif
        ;
    अन्यथा जर( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    अन्यथा जर( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        पूर्णांक delta = borderType == BORDER_REFLECT_101;
        जर( len == 1 )
            परत 0;
        करा
        {
            जर( p < 0 )
                p = -p - 1 + delta;
            अन्यथा
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        पर्यंत(p < 0 || p >= len);
#अन्यथा
        पर्यंत( (अचिन्हित)p >= (अचिन्हित)len );
#endif
    }
    अन्यथा जर( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        जर( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            जर( p64 < 0 )
                p64 += len;
            p = (पूर्णांक)p64;
        }
    }
    अन्यथा जर( borderType == BORDER_CONSTANT )
        p = -1;
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    परत p;
}

नामाकाश
{

शून्य copyMakeBorder_8u( स्थिरांक uchar* src, माप_प्रकार srcstep, cv::Size srcroi,
                        uchar* dst, माप_प्रकार dststep, cv::Size dstroi,
                        पूर्णांक शीर्ष_2, पूर्णांक डावा, पूर्णांक cn, पूर्णांक borderType )
{
    स्थिरांक पूर्णांक isz = (पूर्णांक)आकार(पूर्णांक);
    पूर्णांक i, j, k, elemSize = 1;
    तार्किक intMode = असत्य;

    जर( (cn | srcstep | dststep | (माप_प्रकार)src | (माप_प्रकार)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = सत्य;
    }

    cv::AutoBuffer<पूर्णांक> _tab((dstroi.width - srcroi.width)*cn);
    पूर्णांक* tab = _tab.data();
    पूर्णांक उजवा = dstroi.width - srcroi.width - डावा;
    पूर्णांक bottom = dstroi.height - srcroi.height - शीर्ष_2;

    साठी( i = 0; i < डावा; i++ )
    {
        j = cv::borderInterpolate(i - डावा, srcroi.width, borderType)*cn;
        साठी( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    साठी( i = 0; i < उजवा; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        साठी( k = 0; k < cn; k++ )
            tab[(i+डावा)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    डावा *= cn;
    उजवा *= cn;

    uchar* dstInner = dst + dststep*शीर्ष_2 + डावा*elemSize;

    साठी( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        जर( dstInner != src )
            स_नकल(dstInner, src, srcroi.width*elemSize);

        जर( intMode )
        {
            स्थिरांक पूर्णांक* isrc = (पूर्णांक*)src;
            पूर्णांक* idstInner = (पूर्णांक*)dstInner;
            साठी( j = 0; j < डावा; j++ )
                idstInner[j - डावा] = isrc[tab[j]];
            साठी( j = 0; j < उजवा; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + डावा]];
        }
        अन्यथा
        {
            साठी( j = 0; j < डावा; j++ )
                dstInner[j - डावा] = src[tab[j]];
            साठी( j = 0; j < उजवा; j++ )
                dstInner[j + srcroi.width] = src[tab[j + डावा]];
        }
    }

    dstroi.width *= elemSize;

    साठी( i = 0; i < शीर्ष_2; i++ )
    {
        j = cv::borderInterpolate(i - शीर्ष_2, srcroi.height, borderType);
        स_नकल(dst + i*dststep, dst + (शीर्ष_2+j)*dststep, dstroi.width);
    }

    dst += dststep*शीर्ष_2;
    साठी( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        स_नकल(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


शून्य copyMakeConstBorder_8u( स्थिरांक uchar* src, माप_प्रकार srcstep, cv::Size srcroi,
                             uchar* dst, माप_प्रकार dststep, cv::Size dstroi,
                             पूर्णांक शीर्ष_2, पूर्णांक डावा, पूर्णांक cn, स्थिरांक uchar* value )
{
    पूर्णांक i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    पूर्णांक उजवा = dstroi.width - srcroi.width - डावा;
    पूर्णांक bottom = dstroi.height - srcroi.height - शीर्ष_2;

    साठी( i = 0; i < dstroi.width; i++ )
    {
        साठी( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    डावा *= cn;
    उजवा *= cn;

    uchar* dstInner = dst + dststep*शीर्ष_2 + डावा;

    साठी( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        जर( dstInner != src )
            स_नकल( dstInner, src, srcroi.width );
        स_नकल( dstInner - डावा, constBuf, डावा );
        स_नकल( dstInner + srcroi.width, constBuf, उजवा );
    }

    साठी( i = 0; i < शीर्ष_2; i++ )
        स_नकल(dst + i * dststep, constBuf, dstroi.width);

    dst += (शीर्ष_2 + srcroi.height) * dststep;
    साठी( i = 0; i < bottom; i++ )
        स_नकल(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

नामाकाश cv {

स्थिर तार्किक ocl_copyMakeBorder( आदान_Array _src, OutputArray _dst, पूर्णांक शीर्ष_2, पूर्णांक bottom,
                                पूर्णांक डावा, पूर्णांक उजवा, पूर्णांक borderType, स्थिरांक Scalar& value )
{
    पूर्णांक type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    तार्किक isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    जर ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        परत असत्य;

    स्थिरांक अक्षर * स्थिरांक borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    पूर्णांक scalarcn = cn == 3 ? 4 : cn;
    पूर्णांक sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    जर (k.empty())
        परत असत्य;

    UMat src = _src.getUMat();
    जर( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        पूर्णांक dtop = std::min(ofs.y, शीर्ष_2);
        पूर्णांक dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        पूर्णांक dleft = std::min(ofs.x, डावा);
        पूर्णांक dright = std::min(wholeSize.width - src.cols - ofs.x, उजवा);
        src.adjustROI(dtop, dbottom, dleft, dright);
        शीर्ष_2 -= dtop;
        डावा -= dleft;
        bottom -= dbottom;
        उजवा -= dright;
    }

    _dst.create(src.rows + शीर्ष_2 + bottom, src.cols + डावा + उजवा, type);
    UMat dst = _dst.getUMat();

    जर (शीर्ष_2 == 0 && डावा == 0 && bottom == 0 && उजवा == 0)
    {
        जर(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        परत सत्य;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           शीर्ष_2, डावा, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst.cols, ((माप_प्रकार)dst.rows + rowsPerWI - 1) / rowsPerWI };
    परत k.run(2, globalsize, रिक्त, असत्य);
}

}
#endif

#ifdef HAVE_IPP
नामाकाश cv {

स्थिर तार्किक ipp_copyMakeBorder( Mat &_src, Mat &_dst, पूर्णांक शीर्ष_2, पूर्णांक bottom,
                                पूर्णांक डावा, पूर्णांक उजवा, पूर्णांक _borderType, स्थिरांक Scalar& value )
{
#जर defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(डावा, शीर्ष_2, उजवा, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    जर((पूर्णांक)borderType == -1)
        परत असत्य;

    जर(_src.dims > 2)
        परत असत्य;

    Rect dstRect(borderSize.डावा, borderSize.शीर्ष_2,
        _dst.cols - borderSize.उजवा - borderSize.डावा,
        _dst.rows - borderSize.bottom - borderSize.शीर्ष_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    परत CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#अन्यथा
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(शीर्ष_2); CV_UNUSED(bottom); CV_UNUSED(डावा); CV_UNUSED(उजवा);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    परत असत्य;
#endif
}
}
#endif

शून्य cv::copyMakeBorder( आदान_Array _src, OutputArray _dst, पूर्णांक शीर्ष_2, पूर्णांक bottom,
                         पूर्णांक डावा, पूर्णांक उजवा, पूर्णांक borderType, स्थिरांक Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( शीर्ष_2 >= 0 && bottom >= 0 && डावा >= 0 && उजवा >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + शीर्ष_2 + bottom <= पूर्णांक_उच्च );
    CV_Assert( (int64_t)src.cols + डावा + उजवा <= पूर्णांक_उच्च );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, शीर्ष_2, bottom, डावा, उजवा, borderType, value))
    पूर्णांक type = src.type();

    जर( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        पूर्णांक dtop = std::min(ofs.y, शीर्ष_2);
        पूर्णांक dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        पूर्णांक dleft = std::min(ofs.x, डावा);
        पूर्णांक dright = std::min(wholeSize.width - src.cols - ofs.x, उजवा);
        src.adjustROI(dtop, dbottom, dleft, dright);
        शीर्ष_2 -= dtop;
        डावा -= dleft;
        bottom -= dbottom;
        उजवा -= dright;
    }

    _dst.create( src.rows + शीर्ष_2 + bottom, src.cols + डावा + उजवा, type );
    Mat dst = _dst.getMat();

    जर(शीर्ष_2 == 0 && डावा == 0 && bottom == 0 && उजवा == 0)
    {
        जर(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        परत;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, शीर्ष_2, bottom, डावा, उजवा, borderType, value))

    जर( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           शीर्ष_2, डावा, (पूर्णांक)src.elemSize(), borderType );
    अन्यथा
    {
        पूर्णांक cn = src.channels(), cn1 = cn;
        AutoBuffer<द्विगुण> buf(cn);
        जर( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                शीर्ष_2, डावा, (पूर्णांक)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
