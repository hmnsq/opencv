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

template <typename T> स्थिर अन्तःस्थ
शून्य scalarToRawData_(ध्रुव Scalar& s, T * ध्रुव buf, ध्रुव पूर्णाङ्क cn, ध्रुव पूर्णाङ्क unroll_to)
{
    पूर्णाङ्क i = 0;
    प्रति(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    प्रति(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

शून्य scalarToRawData(ध्रुव Scalar& s, शून्य* _buf, पूर्णाङ्क type, पूर्णाङ्क unroll_to)
{
    CV_INSTRUMENT_REGION();

    ध्रुव पूर्णाङ्क depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    विकल्प(depth)
    {
    पक्ष CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_Bool:
        scalarToRawData_(s, (तार्किक*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_32S:
        scalarToRawData_(s, (पूर्णाङ्क*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_32F:
        scalarToRawData_(s, (भिन्न*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        भङ्ग;
    पक्ष CV_64F:
        scalarToRawData_(s, (द्विगुण*)_buf, cn, unroll_to);
        भङ्ग;
    स्वभाव:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

शून्य convertAndUnrollScalar( ध्रुव Mat& sc, पूर्णाङ्क buftype, uchar* scbuf, size_t blocksize )
{
    पूर्णाङ्क scn = (पूर्णाङ्क)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    द्विमान_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    जइ( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        प्रति( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    प्रति( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> स्थिर शून्य
copyMask_(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    प्रति( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव T* src = (ध्रुव T*)_src;
        T* dst = (T*)_dst;
        पूर्णाङ्क x = 0;
         #जइ CV_ENABLE_UNROLLED
        प्रति( ; x <= size.width - 4; x += 4 )
        {
            जइ( mask[x] )
                dst[x] = src[x];
            जइ( mask[x+1] )
                dst[x+1] = src[x+1];
            जइ( mask[x+2] )
                dst[x+2] = src[x+2];
            जइ( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        प्रति( ; x < size.width; x++ )
            जइ( mask[x] )
                dst[x] = src[x];
    }
}

template<> शून्य
copyMask_<uchar>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (पूर्णाङ्क)sstep, _dst, (पूर्णाङ्क)dstep, ippiSize(size), mask, (पूर्णाङ्क)mstep) >= 0)

    प्रति( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव uchar* src = (ध्रुव uchar*)_src;
        uchar* dst = (uchar*)_dst;
        पूर्णाङ्क x = 0;
        #जइ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            प्रति( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        प्रति( ; x < size.width; x++ )
            जइ( mask[x] )
                dst[x] = src[x];
    }
}

template<> शून्य
copyMask_<Vec3b>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    प्रति( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव uchar* src = (ध्रुव uchar*)_src;
        uchar* dst = (uchar*)_dst;
        पूर्णाङ्क x = 0;
#जइ (CV_SIMD || CV_SIMD_SCALABLE)
        प्रति( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        प्रति( ; x < size.width; x++ )
            जइ( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> शून्य
copyMask_<ushort>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ध्रुव Ipp16u *)_src, (पूर्णाङ्क)sstep, (Ipp16u *)_dst, (पूर्णाङ्क)dstep, ippiSize(size), mask, (पूर्णाङ्क)mstep) >= 0)

    प्रति( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव ushort* src = (ध्रुव ushort*)_src;
        ushort* dst = (ushort*)_dst;
        पूर्णाङ्क x = 0;
        #जइ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            प्रति( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        प्रति( ; x < size.width; x++ )
            जइ( mask[x] )
                dst[x] = src[x];
    }
}

template<> शून्य
copyMask_<Vec3s>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    प्रति( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव ushort* src = (ध्रुव ushort*)_src;
        ushort* dst = (ushort*)_dst;
        पूर्णाङ्क x = 0;
#जइ (CV_SIMD || CV_SIMD_SCALABLE)
        प्रति( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        प्रति( ; x < size.width; x++ )
            जइ( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> शून्य
copyMask_<पूर्णाङ्क>(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    प्रति( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव पूर्णाङ्क* src = (ध्रुव पूर्णाङ्क*)_src;
        पूर्णाङ्क* dst = (पूर्णाङ्क*)_dst;
        पूर्णाङ्क x = 0;
#जइ (CV_SIMD || CV_SIMD_SCALABLE)
        प्रति (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        प्रति (; x < size.width; x++)
            जइ ( mask[x] )
                dst[x] = src[x];
    }
}

स्थिर शून्य
copyMaskGeneric(ध्रुव uchar* _src, size_t sstep, ध्रुव uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, शून्य* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    प्रति( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ध्रुव uchar* src = _src;
        uchar* dst = _dst;
        पूर्णाङ्क x = 0;
        प्रति( ; x < size.width; x++, src += esz, dst += esz )
        {
            जइ( !mask[x] )
                अनुवर्त;
            प्रति( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
स्थिर शून्य copyMask##suffix(ध्रुव uchar* src, size_t sstep, ध्रुव uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, शून्य*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#जइ defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
स्थिर शून्य copyMask##suffix(ध्रुव uchar* src, size_t sstep, ध्रुव uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, शून्य*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ध्रुव ipptype *)src, (पूर्णाङ्क)sstep, (ipptype *)dst, (पूर्णाङ्क)dstep, ippiSize(size), (ध्रुव Ipp8u *)mask, (पूर्णाङ्क)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#अण्णहा
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
स्थिर शून्य copyMask##suffix(ध्रुव uchar* src, size_t sstep, ध्रुव uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, शून्य*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#जइ IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#अण्णहा
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, पूर्णाङ्क, 32s_C1MR, Ipp32s)
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

द्विमान_Func getCopyMaskFunc(size_t esz)
{
    प्रत्यागम esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
शून्य Mat::copyTo( OutputArray _dst ) ध्रुव
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    जइ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*एतत्);
        प्रत्यागम;
    }
#endif

    पूर्णाङ्क stype = type();
    पूर्णाङ्क dtype = _dst.type();
    जइ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        प्रत्यागम;
    }

    जइ( dims == 0 && empty() )
    {
        _dst.release();
        शून्य* obj = _dst.getObj();
        जइ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        अण्णहा जइ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        अण्णहा जइ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        प्रत्यागम;
    }

    तार्किक allowTransposed = dims == 1 ||
        _dst.kind() == निवेशः_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    जइ( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        जइ (empty())
            प्रत्यागम;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != रिक्त);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        प्रति( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        पूर्णाङ्क lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        प्रत्यागम;
    }

    जइ( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        जइ( data == dst.data )
            प्रत्यागम;

        जइ( rows > 0 && cols > 0 )
        {
            Mat src = *एतत्;
            Size sz = getContinuousSize2D(src, dst, (पूर्णाङ्क)elemSize());
            CV_CheckGE(sz.width, 0, "");

            ध्रुव uchar* sptr = src.data;
            uchar* dptr = dst.data;

#जइ IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (पूर्णाङ्क)src.step, dptr, (पूर्णाङ्क)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            प्रति (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        प्रत्यागम;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    जइ( data == dst.data )
        प्रत्यागम;

    जइ( total() != 0 )
    {
        ध्रुव Mat* arrays[] = { एतत्, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
स्थिर तार्किक ipp_copyTo(ध्रुव Mat &src, Mat &dst, ध्रुव Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    जइ(mask.channels() > 1 || mask.depth() != CV_8U)
        प्रत्यागम मिच्छ;

    जइ (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (पूर्णाङ्क)src.step, dst.ptr(), (पूर्णाङ्क)dst.step, size, (पूर्णाङ्क)src.elemSize1(), src.channels(), mask.ptr(), (पूर्णाङ्क)mask.step) >= 0;
    }
    अण्णहा
    {
        ध्रुव Mat      *arrays[] = {&src, &dst, &mask, रिक्त};
        uchar          *ptrs[3]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            जइ(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (पूर्णाङ्क)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                प्रत्यागम मिच्छ;
        }
        प्रत्यागम सच्च;
    }
#अण्णहा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    प्रत्यागम मिच्छ;
#endif
}
#endif

शून्य Mat::copyTo( OutputArray _dst, निवेशः_Array _mask ) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    जइ( !mask.data )
    {
        copyTo(_dst);
        प्रत्यागम;
    }

    पूर्णाङ्क cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    तार्किक colorMask = mcn > 1;
    जइ( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        जइ (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*एतत्, dst, mask))
    जइ ( एतत्->dims <= 2 ) {
        जइ ( एतत्->size() == dst.size() && एतत्->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, एतत्->data, एतत्->step, dst.data, dst.step, एतत्->cols, एतत्->rows, एतत्->type(), mask.data, mask.step, mask.type());
        }
    }
    अण्णहा जइ ( एतत्->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = एतत्->total();
        जइ (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, एतत्->data, 0, dst.data, 0, (पूर्णाङ्क)sz, 1, एतत्->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    द्विमान_Func copymask = getCopyMaskFunc(esz);

    जइ( dims <= 2 )
    {
        Mat src = *एतत्;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        प्रत्यागम;
    }

    ध्रुव Mat* arrays[] = { एतत्, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((पूर्णाङ्क)(it.size*mcn), 1);

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
    copyTo(_dst);
}
शून्य Mat::copyAt( OutputArray _dst, निवेशः_Array _mask ) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


स्थिर तार्किक can_apply_memset(ध्रुव Mat &mat, ध्रुव Scalar &s, पूर्णाङ्क &fill_मानम्)
{
    // check if depth is 1 byte.
    विकल्प (mat.depth())
    {
    पक्ष CV_8U: fill_मानम् = saturate_cast<uchar>( s.val[0] ); भङ्ग;
    पक्ष CV_8S: fill_मानम् = saturate_cast<schar>( s.val[0] ); भङ्ग;
    स्वभाव: प्रत्यागम मिच्छ;
    }

    // check if all element is same.
    ध्रुव int64* is = (ध्रुव int64*)&s.val[0];
    विकल्प (mat.channels())
    {
    पक्ष 1: प्रत्यागम सच्च;
    पक्ष 2: प्रत्यागम (is[0] == is[1]);
    पक्ष 3: प्रत्यागम (is[0] == is[1] && is[1] == is[2]);
    पक्ष 4: प्रत्यागम (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    स्वभाव: प्रत्यागम मिच्छ;
    }
}

Mat& Mat::operator = (ध्रुव Scalar& s)
{
    CV_INSTRUMENT_REGION();

    जइ (एतत्->empty())
        प्रत्यागम *एतत्;

    ध्रुव Mat* arrays[] = { एतत् };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    ध्रुव int64* is = (ध्रुव int64*)&s.val[0];

    जइ( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    अण्णहा
    {
        पूर्णाङ्क fill_मानम् = 0;
        जइ ( can_apply_memset(*एतत्, s, fill_मानम्) )
        {
            प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_मानम्, elsize);
            प्रत्यागम *एतत्;
        }

        जइ( it.nplanes > 0 )
        {
            द्विगुण scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            प्रति( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= परिमाण(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        प्रति( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    प्रत्यागम *एतत्;
}

#ifdef HAVE_IPP
स्थिर तार्किक ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    जइ(mask.empty())
        प्रत्यागम मिच्छ;

    जइ(mask.depth() != CV_8U || mask.channels() > 1)
        प्रत्यागम मिच्छ;

    जइ(dst.channels() > 4)
        प्रत्यागम मिच्छ;

    जइ (dst.depth() == CV_32F)
    {
        प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)(_val.total()); i++)
        {
            भिन्न v = (भिन्न)(_val.at<द्विगुण>(i));  // cast to float
            जइ (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                प्रत्यागम मिच्छ;
        }
    }

    जइ(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_मानम्_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (पूर्णाङ्क)dst.step, size, dataType, dst.channels(), mask.ptr(), (पूर्णाङ्क)mask.step) >= 0;
    }
    अण्णहा
    {
        ध्रुव Mat      *arrays[] = {&dst, mask.empty()?रिक्त:&mask, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(पूर्णाङ्क)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_मानम्_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            जइ(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                प्रत्यागम मिच्छ;
        }
        प्रत्यागम सच्च;
    }
#अण्णहा
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    प्रत्यागम मिच्छ;
#endif
}
#endif

Mat& Mat::setTo(निवेशः_Array मानम्, निवेशः_Array _mask)
{
    CV_INSTRUMENT_REGION();

    जइ( empty() )
        प्रत्यागम *एतत्;

    Mat value = मानम्.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), मानम्.kind(), निवेशः_Array_2::MAT ));
    पूर्णाङ्क cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*एतत्, value, mask), *एतत्)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    द्विमान_Func copymask = getCopyMaskFunc(esz);

    ध्रुव Mat* arrays[] = { एतत्, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क totalsz = (पूर्णाङ्क)it.size*mcn;
    पूर्णाङ्क blockSize0 = std::min(totalsz, (पूर्णाङ्क)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (पूर्णाङ्क)परिमाण(द्विगुण));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( पूर्णाङ्क j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            जइ( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            अण्णहा
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    प्रत्यागम *एतत्;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    जइ( empty() )
        प्रत्यागम *एतत्;

    size_t esz = elemSize();

    ध्रुव Mat* arrays[] = { एतत्, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    प्रत्यागम *एतत्;
}


#जइ defined HAVE_OPENCL && !defined __APPLE__

स्थिर तार्किक ocl_repeat(निवेशः_Array _src, पूर्णाङ्क ny, पूर्णाङ्क nx, OutputArray _dst)
{
    जइ (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        प्रत्यागम सच्च;
    }

    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    जइ (k.empty())
        प्रत्यागम मिच्छ;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, रिक्त, मिच्छ);
}

#endif

शून्य repeat(निवेशः_Array _src, पूर्णाङ्क ny, पूर्णाङ्क nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    जइ( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        प्रत्यागम;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#जइ !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    पूर्णाङ्क esz = (पूर्णाङ्क)src.elemSize();
    पूर्णाङ्क x, y;
    ssize.width *= esz; dsize.width *= esz;

    प्रति( y = 0; y < ssize.height; y++ )
    {
        प्रति( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    प्रति( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(ध्रुव Mat& src, पूर्णाङ्क ny, पूर्णाङ्क nx)
{
    जइ( nx == 1 && ny == 1 )
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

#ifdef CV_STATIC_ANALYSIS
    जइ(p >= 0 && p < len)
#अण्णहा
    जइ( (unsigned)p < (unsigned)len )
#endif
        ;
    अण्णहा जइ( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    अण्णहा जइ( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        पूर्णाङ्क delta = borderType == BORDER_REFLECT_101;
        जइ( len == 1 )
            प्रत्यागम 0;
        करेहि
        {
            जइ( p < 0 )
                p = -p - 1 + delta;
            अण्णहा
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        जाव(p < 0 || p >= len);
#अण्णहा
        जाव( (unsigned)p >= (unsigned)len );
#endif
    }
    अण्णहा जइ( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        जइ( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            जइ( p64 < 0 )
                p64 += len;
            p = (पूर्णाङ्क)p64;
        }
    }
    अण्णहा जइ( borderType == BORDER_CONSTANT )
        p = -1;
    अण्णहा
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    प्रत्यागम p;
}

नामाकाश
{

शून्य copyMakeBorder_8u( ध्रुव uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        पूर्णाङ्क शिखरम्_2, पूर्णाङ्क वामः, पूर्णाङ्क cn, पूर्णाङ्क borderType )
{
    ध्रुव पूर्णाङ्क isz = (पूर्णाङ्क)परिमाण(पूर्णाङ्क);
    पूर्णाङ्क i, j, k, elemSize = 1;
    तार्किक intMode = मिच्छ;

    जइ( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = सच्च;
    }

    cv::AutoBuffer<पूर्णाङ्क> _tab((dstroi.width - srcroi.width)*cn);
    पूर्णाङ्क* tab = _tab.data();
    पूर्णाङ्क दक्षिणः = dstroi.width - srcroi.width - वामः;
    पूर्णाङ्क bottom = dstroi.height - srcroi.height - शिखरम्_2;

    प्रति( i = 0; i < वामः; i++ )
    {
        j = cv::borderInterpolate(i - वामः, srcroi.width, borderType)*cn;
        प्रति( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    प्रति( i = 0; i < दक्षिणः; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        प्रति( k = 0; k < cn; k++ )
            tab[(i+वामः)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    वामः *= cn;
    दक्षिणः *= cn;

    uchar* dstInner = dst + dststep*शिखरम्_2 + वामः*elemSize;

    प्रति( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        जइ( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        जइ( intMode )
        {
            ध्रुव पूर्णाङ्क* isrc = (पूर्णाङ्क*)src;
            पूर्णाङ्क* idstInner = (पूर्णाङ्क*)dstInner;
            प्रति( j = 0; j < वामः; j++ )
                idstInner[j - वामः] = isrc[tab[j]];
            प्रति( j = 0; j < दक्षिणः; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + वामः]];
        }
        अण्णहा
        {
            प्रति( j = 0; j < वामः; j++ )
                dstInner[j - वामः] = src[tab[j]];
            प्रति( j = 0; j < दक्षिणः; j++ )
                dstInner[j + srcroi.width] = src[tab[j + वामः]];
        }
    }

    dstroi.width *= elemSize;

    प्रति( i = 0; i < शिखरम्_2; i++ )
    {
        j = cv::borderInterpolate(i - शिखरम्_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (शिखरम्_2+j)*dststep, dstroi.width);
    }

    dst += dststep*शिखरम्_2;
    प्रति( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


शून्य copyMakeConstBorder_8u( ध्रुव uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             पूर्णाङ्क शिखरम्_2, पूर्णाङ्क वामः, पूर्णाङ्क cn, ध्रुव uchar* value )
{
    पूर्णाङ्क i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    पूर्णाङ्क दक्षिणः = dstroi.width - srcroi.width - वामः;
    पूर्णाङ्क bottom = dstroi.height - srcroi.height - शिखरम्_2;

    प्रति( i = 0; i < dstroi.width; i++ )
    {
        प्रति( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    वामः *= cn;
    दक्षिणः *= cn;

    uchar* dstInner = dst + dststep*शिखरम्_2 + वामः;

    प्रति( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        जइ( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - वामः, constBuf, वामः );
        memcpy( dstInner + srcroi.width, constBuf, दक्षिणः );
    }

    प्रति( i = 0; i < शिखरम्_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (शिखरम्_2 + srcroi.height) * dststep;
    प्रति( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

नामाकाश cv {

स्थिर तार्किक ocl_copyMakeBorder( निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क शिखरम्_2, पूर्णाङ्क bottom,
                                पूर्णाङ्क वामः, पूर्णाङ्क दक्षिणः, पूर्णाङ्क borderType, ध्रुव Scalar& value )
{
    पूर्णाङ्क type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    तार्किक isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    जइ ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        प्रत्यागम मिच्छ;

    ध्रुव वर्ण * ध्रुव borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    पूर्णाङ्क scalarcn = cn == 3 ? 4 : cn;
    पूर्णाङ्क sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    जइ (k.empty())
        प्रत्यागम मिच्छ;

    UMat src = _src.getUMat();
    जइ( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        पूर्णाङ्क dtop = std::min(ofs.y, शिखरम्_2);
        पूर्णाङ्क dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        पूर्णाङ्क dleft = std::min(ofs.x, वामः);
        पूर्णाङ्क dright = std::min(wholeSize.width - src.cols - ofs.x, दक्षिणः);
        src.adjustROI(dtop, dbottom, dleft, dright);
        शिखरम्_2 -= dtop;
        वामः -= dleft;
        bottom -= dbottom;
        दक्षिणः -= dright;
    }

    _dst.create(src.rows + शिखरम्_2 + bottom, src.cols + वामः + दक्षिणः, type);
    UMat dst = _dst.getUMat();

    जइ (शिखरम्_2 == 0 && वामः == 0 && bottom == 0 && दक्षिणः == 0)
    {
        जइ(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        प्रत्यागम सच्च;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           शिखरम्_2, वामः, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, रिक्त, मिच्छ);
}

}
#endif

#ifdef HAVE_IPP
नामाकाश cv {

स्थिर तार्किक ipp_copyMakeBorder( Mat &_src, Mat &_dst, पूर्णाङ्क शिखरम्_2, पूर्णाङ्क bottom,
                                पूर्णाङ्क वामः, पूर्णाङ्क दक्षिणः, पूर्णाङ्क _borderType, ध्रुव Scalar& value )
{
#जइ defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(वामः, शिखरम्_2, दक्षिणः, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    जइ((पूर्णाङ्क)borderType == -1)
        प्रत्यागम मिच्छ;

    जइ(_src.dims > 2)
        प्रत्यागम मिच्छ;

    Rect dstRect(borderSize.वामः, borderSize.शिखरम्_2,
        _dst.cols - borderSize.दक्षिणः - borderSize.वामः,
        _dst.rows - borderSize.bottom - borderSize.शिखरम्_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#अण्णहा
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(शिखरम्_2); CV_UNUSED(bottom); CV_UNUSED(वामः); CV_UNUSED(दक्षिणः);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    प्रत्यागम मिच्छ;
#endif
}
}
#endif

शून्य cv::copyMakeBorder( निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क शिखरम्_2, पूर्णाङ्क bottom,
                         पूर्णाङ्क वामः, पूर्णाङ्क दक्षिणः, पूर्णाङ्क borderType, ध्रुव Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( शिखरम्_2 >= 0 && bottom >= 0 && वामः >= 0 && दक्षिणः >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + शिखरम्_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + वामः + दक्षिणः <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, शिखरम्_2, bottom, वामः, दक्षिणः, borderType, value))
    पूर्णाङ्क type = src.type();

    जइ( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        पूर्णाङ्क dtop = std::min(ofs.y, शिखरम्_2);
        पूर्णाङ्क dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        पूर्णाङ्क dleft = std::min(ofs.x, वामः);
        पूर्णाङ्क dright = std::min(wholeSize.width - src.cols - ofs.x, दक्षिणः);
        src.adjustROI(dtop, dbottom, dleft, dright);
        शिखरम्_2 -= dtop;
        वामः -= dleft;
        bottom -= dbottom;
        दक्षिणः -= dright;
    }

    _dst.create( src.rows + शिखरम्_2 + bottom, src.cols + वामः + दक्षिणः, type );
    Mat dst = _dst.getMat();

    जइ(शिखरम्_2 == 0 && वामः == 0 && bottom == 0 && दक्षिणः == 0)
    {
        जइ(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        प्रत्यागम;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, शिखरम्_2, bottom, वामः, दक्षिणः, borderType, value))

    जइ( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           शिखरम्_2, वामः, (पूर्णाङ्क)src.elemSize(), borderType );
    अण्णहा
    {
        पूर्णाङ्क cn = src.channels(), cn1 = cn;
        AutoBuffer<द्विगुण> buf(cn);
        जइ( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                शिखरम्_2, वामः, (पूर्णाङ्क)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
