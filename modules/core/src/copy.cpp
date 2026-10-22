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

#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

نطاق cv
{

template <typename T> ساكن مضمن
فراغ scalarToRawData_(ثابت Scalar& s, T * ثابت buf, ثابت صحيح cn, ثابت صحيح unroll_to)
{
    صحيح i = 0;
    لكل(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    لكل(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

فراغ scalarToRawData(ثابت Scalar& s, فراغ* _buf, صحيح type, صحيح unroll_to)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    بدل(depth)
    {
    حالة CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_Bool:
        scalarToRawData_(s, (منطقي*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_32S:
        scalarToRawData_(s, (صحيح*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_32F:
        scalarToRawData_(s, (عائم*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        اكسر;
    حالة CV_64F:
        scalarToRawData_(s, (مزدوج*)_buf, cn, unroll_to);
        اكسر;
    افتراضي:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

فراغ convertAndUnrollScalar( ثابت Mat& sc, صحيح buftype, uchar* scbuf, size_t blocksize )
{
    صحيح scn = (صحيح)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    ثنائی_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    إذا( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        لكل( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    لكل( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> ساكن فراغ
copyMask_(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    لكل( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت T* src = (ثابت T*)_src;
        T* dst = (T*)_dst;
        صحيح x = 0;
         #إذا CV_ENABLE_UNROLLED
        لكل( ; x <= size.width - 4; x += 4 )
        {
            إذا( mask[x] )
                dst[x] = src[x];
            إذا( mask[x+1] )
                dst[x+1] = src[x+1];
            إذا( mask[x+2] )
                dst[x+2] = src[x+2];
            إذا( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        لكل( ; x < size.width; x++ )
            إذا( mask[x] )
                dst[x] = src[x];
    }
}

template<> فراغ
copyMask_<uchar>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (صحيح)sstep, _dst, (صحيح)dstep, ippiSize(size), mask, (صحيح)mstep) >= 0)

    لكل( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحيح x = 0;
        #إذا (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            لكل( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        لكل( ; x < size.width; x++ )
            إذا( mask[x] )
                dst[x] = src[x];
    }
}

template<> فراغ
copyMask_<Vec3b>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    لكل( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        لكل( ; x < size.width; x++ )
            إذا( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> فراغ
copyMask_<ushort>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ثابت Ipp16u *)_src, (صحيح)sstep, (Ipp16u *)_dst, (صحيح)dstep, ippiSize(size), mask, (صحيح)mstep) >= 0)

    لكل( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحيح x = 0;
        #إذا (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            لكل( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        لكل( ; x < size.width; x++ )
            إذا( mask[x] )
                dst[x] = src[x];
    }
}

template<> فراغ
copyMask_<Vec3s>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    لكل( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        لكل( ; x < size.width; x++ )
            إذا( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> فراغ
copyMask_<صحيح>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    لكل( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت صحيح* src = (ثابت صحيح*)_src;
        صحيح* dst = (صحيح*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        لكل (; x < size.width; x++)
            إذا ( mask[x] )
                dst[x] = src[x];
    }
}

ساكن فراغ
copyMaskGeneric(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, فراغ* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    لكل( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = _src;
        uchar* dst = _dst;
        صحيح x = 0;
        لكل( ; x < size.width; x++, src += esz, dst += esz )
        {
            إذا( !mask[x] )
                استمر;
            لكل( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
ساكن فراغ copyMask##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, فراغ*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#إذا defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
ساكن فراغ copyMask##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, فراغ*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ثابت ipptype *)src, (صحيح)sstep, (ipptype *)dst, (صحيح)dstep, ippiSize(size), (ثابت Ipp8u *)mask, (صحيح)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#وإلا
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
ساكن فراغ copyMask##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, فراغ*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#إذا IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#وإلا
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, صحيح, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

ثنائی_Func copyMaskTab[] =
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

ثنائی_Func getCopyMaskFunc(size_t esz)
{
    أرجع esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
فراغ Mat::copyTo( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    إذا (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*هذا);
        أرجع;
    }
#endif

    صحيح stype = type();
    صحيح dtype = _dst.type();
    إذا( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        أرجع;
    }

    إذا( dims == 0 && empty() )
    {
        _dst.release();
        فراغ* obj = _dst.getObj();
        إذا (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        وإلا إذا (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        وإلا إذا (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        أرجع;
    }

    منطقي allowTransposed = dims == 1 ||
        _dst.kind() == داخلہ_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    إذا( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        إذا (empty())
            أرجع;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != عدم);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        لكل( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        صحيح lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        أرجع;
    }

    إذا( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        إذا( data == dst.data )
            أرجع;

        إذا( rows > 0 && cols > 0 )
        {
            Mat src = *هذا;
            Size sz = getContinuousSize2D(src, dst, (صحيح)elemSize());
            CV_CheckGE(sz.width, 0, "");

            ثابت uchar* sptr = src.data;
            uchar* dptr = dst.data;

#إذا IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (صحيح)src.step, dptr, (صحيح)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            لكل (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        أرجع;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    إذا( data == dst.data )
        أرجع;

    إذا( total() != 0 )
    {
        ثابت Mat* arrays[] = { هذا, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
ساكن منطقي ipp_copyTo(ثابت Mat &src, Mat &dst, ثابت Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(mask.channels() > 1 || mask.depth() != CV_8U)
        أرجع خطأ;

    إذا (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (صحيح)src.step, dst.ptr(), (صحيح)dst.step, size, (صحيح)src.elemSize1(), src.channels(), mask.ptr(), (صحيح)mask.step) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&src, &dst, &mask, عدم};
        uchar          *ptrs[3]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (صحيح)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                أرجع خطأ;
        }
        أرجع true;
    }
#وإلا
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    أرجع خطأ;
#endif
}
#endif

فراغ Mat::copyTo( OutputArray _dst, داخلہ_Array _mask ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    إذا( !mask.data )
    {
        copyTo(_dst);
        أرجع;
    }

    صحيح cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    منطقي colorMask = mcn > 1;
    إذا( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        إذا (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*هذا, dst, mask))
    إذا ( هذا->dims <= 2 ) {
        إذا ( هذا->size() == dst.size() && هذا->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, هذا->data, هذا->step, dst.data, dst.step, هذا->cols, هذا->rows, هذا->type(), mask.data, mask.step, mask.type());
        }
    }
    وإلا إذا ( هذا->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = هذا->total();
        إذا (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, هذا->data, 0, dst.data, 0, (صحيح)sz, 1, هذا->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    ثنائی_Func copymask = getCopyMaskFunc(esz);

    إذا( dims <= 2 )
    {
        Mat src = *هذا;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        أرجع;
    }

    ثابت Mat* arrays[] = { هذا, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((صحيح)(it.size*mcn), 1);

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
فراغ Mat::copyAt( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
فراغ Mat::copyAt( OutputArray _dst, داخلہ_Array _mask ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


ساكن منطقي can_apply_memset(ثابت Mat &mat, ثابت Scalar &s, صحيح &fill_قدر)
{
    // check if depth is 1 byte.
    بدل (mat.depth())
    {
    حالة CV_8U: fill_قدر = saturate_cast<uchar>( s.val[0] ); اكسر;
    حالة CV_8S: fill_قدر = saturate_cast<schar>( s.val[0] ); اكسر;
    افتراضي: أرجع خطأ;
    }

    // check if all element is same.
    ثابت int64* is = (ثابت int64*)&s.val[0];
    بدل (mat.channels())
    {
    حالة 1: أرجع true;
    حالة 2: أرجع (is[0] == is[1]);
    حالة 3: أرجع (is[0] == is[1] && is[1] == is[2]);
    حالة 4: أرجع (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    افتراضي: أرجع خطأ;
    }
}

Mat& Mat::operator = (ثابت Scalar& s)
{
    CV_INSTRUMENT_REGION();

    إذا (هذا->empty())
        أرجع *هذا;

    ثابت Mat* arrays[] = { هذا };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    ثابت int64* is = (ثابت int64*)&s.val[0];

    إذا( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    وإلا
    {
        صحيح fill_قدر = 0;
        إذا ( can_apply_memset(*هذا, s, fill_قدر) )
        {
            لكل (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_قدر, elsize);
            أرجع *هذا;
        }

        إذا( it.nplanes > 0 )
        {
            مزدوج scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            لكل( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= حجم(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        لكل( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    أرجع *هذا;
}

#ifdef HAVE_IPP
ساكن منطقي ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(mask.empty())
        أرجع خطأ;

    إذا(mask.depth() != CV_8U || mask.channels() > 1)
        أرجع خطأ;

    إذا(dst.channels() > 4)
        أرجع خطأ;

    إذا (dst.depth() == CV_32F)
    {
        لكل (صحيح i = 0; i < (صحيح)(_val.total()); i++)
        {
            عائم v = (عائم)(_val.at<مزدوج>(i));  // cast to float
            إذا (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                أرجع خطأ;
        }
    }

    إذا(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_قدر_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        أرجع CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (صحيح)dst.step, size, dataType, dst.channels(), mask.ptr(), (صحيح)mask.step) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, mask.empty()?عدم:&mask, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(صحيح)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_قدر_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        لكل( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                أرجع خطأ;
        }
        أرجع true;
    }
#وإلا
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    أرجع خطأ;
#endif
}
#endif

Mat& Mat::setTo(داخلہ_Array قدر, داخلہ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    إذا( empty() )
        أرجع *هذا;

    Mat value = قدر.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), قدر.kind(), داخلہ_Array_2::MAT ));
    صحيح cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*هذا, value, mask), *هذا)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    ثنائی_Func copymask = getCopyMaskFunc(esz);

    ثابت Mat* arrays[] = { هذا, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    صحيح totalsz = (صحيح)it.size*mcn;
    صحيح blockSize0 = std::min(totalsz, (صحيح)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (صحيح)حجم(مزدوج));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( صحيح j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            إذا( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            وإلا
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    أرجع *هذا;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    إذا( empty() )
        أرجع *هذا;

    size_t esz = elemSize();

    ثابت Mat* arrays[] = { هذا, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    أرجع *هذا;
}


#إذا defined HAVE_OPENCL && !defined __APPLE__

ساكن منطقي ocl_repeat(داخلہ_Array _src, صحيح ny, صحيح nx, OutputArray _dst)
{
    إذا (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        أرجع true;
    }

    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, عدم, خطأ);
}

#endif

فراغ repeat(داخلہ_Array _src, صحيح ny, صحيح nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    إذا( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        أرجع;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#إذا !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    صحيح esz = (صحيح)src.elemSize();
    صحيح x, y;
    ssize.width *= esz; dsize.width *= esz;

    لكل( y = 0; y < ssize.height; y++ )
    {
        لكل( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    لكل( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(ثابت Mat& src, صحيح ny, صحيح nx)
{
    إذا( nx == 1 && ny == 1 )
        أرجع src;
    Mat dst;
    repeat(src, ny, nx, dst);
    أرجع dst;
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
صحيح cv::borderInterpolate( صحيح p, صحيح len, صحيح borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    إذا(p >= 0 && p < len)
#وإلا
    إذا( (unsigned)p < (unsigned)len )
#endif
        ;
    وإلا إذا( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    وإلا إذا( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        صحيح delta = borderType == BORDER_REFLECT_101;
        إذا( len == 1 )
            أرجع 0;
        افعل
        {
            إذا( p < 0 )
                p = -p - 1 + delta;
            وإلا
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        طالما(p < 0 || p >= len);
#وإلا
        طالما( (unsigned)p >= (unsigned)len );
#endif
    }
    وإلا إذا( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        إذا( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            إذا( p64 < 0 )
                p64 += len;
            p = (صحيح)p64;
        }
    }
    وإلا إذا( borderType == BORDER_CONSTANT )
        p = -1;
    وإلا
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    أرجع p;
}

نطاق
{

فراغ copyMakeBorder_8u( ثابت uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        صحيح چوٹی_2, صحيح بایاں, صحيح cn, صحيح borderType )
{
    ثابت صحيح isz = (صحيح)حجم(صحيح);
    صحيح i, j, k, elemSize = 1;
    منطقي intMode = خطأ;

    إذا( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = true;
    }

    cv::AutoBuffer<صحيح> _tab((dstroi.width - srcroi.width)*cn);
    صحيح* tab = _tab.data();
    صحيح دایاں = dstroi.width - srcroi.width - بایاں;
    صحيح bottom = dstroi.height - srcroi.height - چوٹی_2;

    لكل( i = 0; i < بایاں; i++ )
    {
        j = cv::borderInterpolate(i - بایاں, srcroi.width, borderType)*cn;
        لكل( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    لكل( i = 0; i < دایاں; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        لكل( k = 0; k < cn; k++ )
            tab[(i+بایاں)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    بایاں *= cn;
    دایاں *= cn;

    uchar* dstInner = dst + dststep*چوٹی_2 + بایاں*elemSize;

    لكل( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        إذا( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        إذا( intMode )
        {
            ثابت صحيح* isrc = (صحيح*)src;
            صحيح* idstInner = (صحيح*)dstInner;
            لكل( j = 0; j < بایاں; j++ )
                idstInner[j - بایاں] = isrc[tab[j]];
            لكل( j = 0; j < دایاں; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + بایاں]];
        }
        وإلا
        {
            لكل( j = 0; j < بایاں; j++ )
                dstInner[j - بایاں] = src[tab[j]];
            لكل( j = 0; j < دایاں; j++ )
                dstInner[j + srcroi.width] = src[tab[j + بایاں]];
        }
    }

    dstroi.width *= elemSize;

    لكل( i = 0; i < چوٹی_2; i++ )
    {
        j = cv::borderInterpolate(i - چوٹی_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (چوٹی_2+j)*dststep, dstroi.width);
    }

    dst += dststep*چوٹی_2;
    لكل( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


فراغ copyMakeConstBorder_8u( ثابت uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             صحيح چوٹی_2, صحيح بایاں, صحيح cn, ثابت uchar* value )
{
    صحيح i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    صحيح دایاں = dstroi.width - srcroi.width - بایاں;
    صحيح bottom = dstroi.height - srcroi.height - چوٹی_2;

    لكل( i = 0; i < dstroi.width; i++ )
    {
        لكل( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    بایاں *= cn;
    دایاں *= cn;

    uchar* dstInner = dst + dststep*چوٹی_2 + بایاں;

    لكل( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        إذا( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - بایاں, constBuf, بایاں );
        memcpy( dstInner + srcroi.width, constBuf, دایاں );
    }

    لكل( i = 0; i < چوٹی_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (چوٹی_2 + srcroi.height) * dststep;
    لكل( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_copyMakeBorder( داخلہ_Array _src, OutputArray _dst, صحيح چوٹی_2, صحيح bottom,
                                صحيح بایاں, صحيح دایاں, صحيح borderType, ثابت Scalar& value )
{
    صحيح type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    منطقي isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    إذا ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        أرجع خطأ;

    ثابت حرف * ثابت borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    صحيح scalarcn = cn == 3 ? 4 : cn;
    صحيح sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat();
    إذا( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        صحيح dtop = std::min(ofs.y, چوٹی_2);
        صحيح dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        صحيح dleft = std::min(ofs.x, بایاں);
        صحيح dright = std::min(wholeSize.width - src.cols - ofs.x, دایاں);
        src.adjustROI(dtop, dbottom, dleft, dright);
        چوٹی_2 -= dtop;
        بایاں -= dleft;
        bottom -= dbottom;
        دایاں -= dright;
    }

    _dst.create(src.rows + چوٹی_2 + bottom, src.cols + بایاں + دایاں, type);
    UMat dst = _dst.getUMat();

    إذا (چوٹی_2 == 0 && بایاں == 0 && bottom == 0 && دایاں == 0)
    {
        إذا(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        أرجع true;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           چوٹی_2, بایاں, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, عدم, خطأ);
}

}
#endif

#ifdef HAVE_IPP
نطاق cv {

ساكن منطقي ipp_copyMakeBorder( Mat &_src, Mat &_dst, صحيح چوٹی_2, صحيح bottom,
                                صحيح بایاں, صحيح دایاں, صحيح _borderType, ثابت Scalar& value )
{
#إذا defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(بایاں, چوٹی_2, دایاں, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    إذا((صحيح)borderType == -1)
        أرجع خطأ;

    إذا(_src.dims > 2)
        أرجع خطأ;

    Rect dstRect(borderSize.بایاں, borderSize.چوٹی_2,
        _dst.cols - borderSize.دایاں - borderSize.بایاں,
        _dst.rows - borderSize.bottom - borderSize.چوٹی_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#وإلا
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(چوٹی_2); CV_UNUSED(bottom); CV_UNUSED(بایاں); CV_UNUSED(دایاں);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    أرجع خطأ;
#endif
}
}
#endif

فراغ cv::copyMakeBorder( داخلہ_Array _src, OutputArray _dst, صحيح چوٹی_2, صحيح bottom,
                         صحيح بایاں, صحيح دایاں, صحيح borderType, ثابت Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( چوٹی_2 >= 0 && bottom >= 0 && بایاں >= 0 && دایاں >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + چوٹی_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + بایاں + دایاں <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, چوٹی_2, bottom, بایاں, دایاں, borderType, value))
    صحيح type = src.type();

    إذا( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        صحيح dtop = std::min(ofs.y, چوٹی_2);
        صحيح dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        صحيح dleft = std::min(ofs.x, بایاں);
        صحيح dright = std::min(wholeSize.width - src.cols - ofs.x, دایاں);
        src.adjustROI(dtop, dbottom, dleft, dright);
        چوٹی_2 -= dtop;
        بایاں -= dleft;
        bottom -= dbottom;
        دایاں -= dright;
    }

    _dst.create( src.rows + چوٹی_2 + bottom, src.cols + بایاں + دایاں, type );
    Mat dst = _dst.getMat();

    إذا(چوٹی_2 == 0 && بایاں == 0 && bottom == 0 && دایاں == 0)
    {
        إذا(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        أرجع;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, چوٹی_2, bottom, بایاں, دایاں, borderType, value))

    إذا( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           چوٹی_2, بایاں, (صحيح)src.elemSize(), borderType );
    وإلا
    {
        صحيح cn = src.channels(), cn1 = cn;
        AutoBuffer<مزدوج> buf(cn);
        إذا( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                چوٹی_2, بایاں, (صحيح)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
