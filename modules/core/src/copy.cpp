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

#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

فضاینام cv
{

template <typename T> ایستا درونخطی
تهی scalarToRawData_(ثابت Scalar& s, T * ثابت buf, ثابت صحیح cn, ثابت صحیح unroll_to)
{
    صحیح i = 0;
    برای(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    برای(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

تهی scalarToRawData(ثابت Scalar& s, تهی* _buf, صحیح type, صحیح unroll_to)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    انتخاب(depth)
    {
    حالت CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_Bool:
        scalarToRawData_(s, (منطقی*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_32S:
        scalarToRawData_(s, (صحیح*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_32F:
        scalarToRawData_(s, (اعشاری*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        بشکن;
    حالت CV_64F:
        scalarToRawData_(s, (دوگانه*)_buf, cn, unroll_to);
        بشکن;
    پیشفرض:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

تهی convertAndUnrollScalar( ثابت Mat& sc, صحیح buftype, uchar* scbuf, size_t blocksize )
{
    صحیح scn = (صحیح)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    دودویی_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    اگر( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        برای( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    برای( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> ایستا تهی
copyMask_(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    برای( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت T* src = (ثابت T*)_src;
        T* dst = (T*)_dst;
        صحیح x = 0;
         #اگر CV_ENABLE_UNROLLED
        برای( ; x <= size.width - 4; x += 4 )
        {
            اگر( mask[x] )
                dst[x] = src[x];
            اگر( mask[x+1] )
                dst[x+1] = src[x+1];
            اگر( mask[x+2] )
                dst[x+2] = src[x+2];
            اگر( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        برای( ; x < size.width; x++ )
            اگر( mask[x] )
                dst[x] = src[x];
    }
}

template<> تهی
copyMask_<uchar>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (صحیح)sstep, _dst, (صحیح)dstep, ippiSize(size), mask, (صحیح)mstep) >= 0)

    برای( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحیح x = 0;
        #اگر (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            برای( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        برای( ; x < size.width; x++ )
            اگر( mask[x] )
                dst[x] = src[x];
    }
}

template<> تهی
copyMask_<Vec3b>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    برای( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        برای( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        برای( ; x < size.width; x++ )
            اگر( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> تهی
copyMask_<ushort>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ثابت Ipp16u *)_src, (صحیح)sstep, (Ipp16u *)_dst, (صحیح)dstep, ippiSize(size), mask, (صحیح)mstep) >= 0)

    برای( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحیح x = 0;
        #اگر (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            برای( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        برای( ; x < size.width; x++ )
            اگر( mask[x] )
                dst[x] = src[x];
    }
}

template<> تهی
copyMask_<Vec3s>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    برای( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        برای( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        برای( ; x < size.width; x++ )
            اگر( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> تهی
copyMask_<صحیح>(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    برای( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت صحیح* src = (ثابت صحیح*)_src;
        صحیح* dst = (صحیح*)_dst;
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        برای (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        برای (; x < size.width; x++)
            اگر ( mask[x] )
                dst[x] = src[x];
    }
}

ایستا تهی
copyMaskGeneric(ثابت uchar* _src, size_t sstep, ثابت uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, تهی* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    برای( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = _src;
        uchar* dst = _dst;
        صحیح x = 0;
        برای( ; x < size.width; x++, src += esz, dst += esz )
        {
            اگر( !mask[x] )
                ادامه;
            برای( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
ایستا تهی copyMask##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, تهی*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#اگر defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
ایستا تهی copyMask##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, تهی*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ثابت ipptype *)src, (صحیح)sstep, (ipptype *)dst, (صحیح)dstep, ippiSize(size), (ثابت Ipp8u *)mask, (صحیح)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#وگرنه
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
ایستا تهی copyMask##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, تهی*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#اگر IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#وگرنه
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, صحیح, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

دودویی_Func copyMaskTab[] =
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

دودویی_Func getCopyMaskFunc(size_t esz)
{
    بازگشت esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
تهی Mat::copyTo( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    اگر (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*این);
        بازگشت;
    }
#endif

    صحیح stype = type();
    صحیح dtype = _dst.type();
    اگر( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        بازگشت;
    }

    اگر( dims == 0 && empty() )
    {
        _dst.release();
        تهی* obj = _dst.getObj();
        اگر (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        وگرنه اگر (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        وگرنه اگر (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        بازگشت;
    }

    منطقی allowTransposed = dims == 1 ||
        _dst.kind() == درون_داد_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    اگر( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        اگر (empty())
            بازگشت;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != پوچ);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        برای( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        صحیح lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        بازگشت;
    }

    اگر( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        اگر( data == dst.data )
            بازگشت;

        اگر( rows > 0 && cols > 0 )
        {
            Mat src = *این;
            Size sz = getContinuousSize2D(src, dst, (صحیح)elemSize());
            CV_CheckGE(sz.width, 0, "");

            ثابت uchar* sptr = src.data;
            uchar* dptr = dst.data;

#اگر IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (صحیح)src.step, dptr, (صحیح)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            برای (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        بازگشت;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    اگر( data == dst.data )
        بازگشت;

    اگر( total() != 0 )
    {
        ثابت Mat* arrays[] = { این, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
ایستا منطقی ipp_copyTo(ثابت Mat &src, Mat &dst, ثابت Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    اگر(mask.channels() > 1 || mask.depth() != CV_8U)
        بازگشت نادرست;

    اگر (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        بازگشت CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (صحیح)src.step, dst.ptr(), (صحیح)dst.step, size, (صحیح)src.elemSize1(), src.channels(), mask.ptr(), (صحیح)mask.step) >= 0;
    }
    وگرنه
    {
        ثابت Mat      *arrays[] = {&src, &dst, &mask, پوچ};
        uchar          *ptrs[3]  = {پوچ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        برای (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (صحیح)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                بازگشت نادرست;
        }
        بازگشت درست;
    }
#وگرنه
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    بازگشت نادرست;
#endif
}
#endif

تهی Mat::copyTo( OutputArray _dst, درون_داد_Array _mask ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    اگر( !mask.data )
    {
        copyTo(_dst);
        بازگشت;
    }

    صحیح cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    منطقی colorMask = mcn > 1;
    اگر( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        اگر (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*این, dst, mask))
    اگر ( این->dims <= 2 ) {
        اگر ( این->size() == dst.size() && این->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, این->data, این->step, dst.data, dst.step, این->cols, این->rows, این->type(), mask.data, mask.step, mask.type());
        }
    }
    وگرنه اگر ( این->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = این->total();
        اگر (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, این->data, 0, dst.data, 0, (صحیح)sz, 1, این->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    دودویی_Func copymask = getCopyMaskFunc(esz);

    اگر( dims <= 2 )
    {
        Mat src = *این;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        بازگشت;
    }

    ثابت Mat* arrays[] = { این, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((صحیح)(it.size*mcn), 1);

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
تهی Mat::copyAt( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
تهی Mat::copyAt( OutputArray _dst, درون_داد_Array _mask ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


ایستا منطقی can_apply_memset(ثابت Mat &mat, ثابت Scalar &s, صحیح &fill_مقدار)
{
    // check if depth is 1 byte.
    انتخاب (mat.depth())
    {
    حالت CV_8U: fill_مقدار = saturate_cast<uchar>( s.val[0] ); بشکن;
    حالت CV_8S: fill_مقدار = saturate_cast<schar>( s.val[0] ); بشکن;
    پیشفرض: بازگشت نادرست;
    }

    // check if all element is same.
    ثابت int64* is = (ثابت int64*)&s.val[0];
    انتخاب (mat.channels())
    {
    حالت 1: بازگشت درست;
    حالت 2: بازگشت (is[0] == is[1]);
    حالت 3: بازگشت (is[0] == is[1] && is[1] == is[2]);
    حالت 4: بازگشت (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    پیشفرض: بازگشت نادرست;
    }
}

Mat& Mat::operator = (ثابت Scalar& s)
{
    CV_INSTRUMENT_REGION();

    اگر (این->empty())
        بازگشت *این;

    ثابت Mat* arrays[] = { این };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    ثابت int64* is = (ثابت int64*)&s.val[0];

    اگر( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        برای( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    وگرنه
    {
        صحیح fill_مقدار = 0;
        اگر ( can_apply_memset(*این, s, fill_مقدار) )
        {
            برای (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_مقدار, elsize);
            بازگشت *این;
        }

        اگر( it.nplanes > 0 )
        {
            دوگانه scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            برای( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= اندازه(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        برای( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    بازگشت *این;
}

#ifdef HAVE_IPP
ایستا منطقی ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    اگر(mask.empty())
        بازگشت نادرست;

    اگر(mask.depth() != CV_8U || mask.channels() > 1)
        بازگشت نادرست;

    اگر(dst.channels() > 4)
        بازگشت نادرست;

    اگر (dst.depth() == CV_32F)
    {
        برای (صحیح i = 0; i < (صحیح)(_val.total()); i++)
        {
            اعشاری v = (اعشاری)(_val.at<دوگانه>(i));  // cast to float
            اگر (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                بازگشت نادرست;
        }
    }

    اگر(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_مقدار_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        بازگشت CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (صحیح)dst.step, size, dataType, dst.channels(), mask.ptr(), (صحیح)mask.step) >= 0;
    }
    وگرنه
    {
        ثابت Mat      *arrays[] = {&dst, mask.empty()?پوچ:&mask, پوچ};
        uchar          *ptrs[2]  = {پوچ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(صحیح)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_مقدار_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        برای( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                بازگشت نادرست;
        }
        بازگشت درست;
    }
#وگرنه
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    بازگشت نادرست;
#endif
}
#endif

Mat& Mat::setTo(درون_داد_Array مقدار, درون_داد_Array _mask)
{
    CV_INSTRUMENT_REGION();

    اگر( empty() )
        بازگشت *این;

    Mat value = مقدار.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), مقدار.kind(), درون_داد_Array_2::MAT ));
    صحیح cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*این, value, mask), *این)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    دودویی_Func copymask = getCopyMaskFunc(esz);

    ثابت Mat* arrays[] = { این, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    صحیح totalsz = (صحیح)it.size*mcn;
    صحیح blockSize0 = std::min(totalsz, (صحیح)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (صحیح)اندازه(دوگانه));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( صحیح j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            اگر( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            وگرنه
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    بازگشت *این;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    اگر( empty() )
        بازگشت *این;

    size_t esz = elemSize();

    ثابت Mat* arrays[] = { این, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    بازگشت *این;
}


#اگر defined HAVE_OPENCL && !defined __APPLE__

ایستا منطقی ocl_repeat(درون_داد_Array _src, صحیح ny, صحیح nx, OutputArray _dst)
{
    اگر (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        بازگشت درست;
    }

    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    اگر (k.empty())
        بازگشت نادرست;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

تهی repeat(درون_داد_Array _src, صحیح ny, صحیح nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    اگر( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        بازگشت;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#اگر !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    صحیح esz = (صحیح)src.elemSize();
    صحیح x, y;
    ssize.width *= esz; dsize.width *= esz;

    برای( y = 0; y < ssize.height; y++ )
    {
        برای( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    برای( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(ثابت Mat& src, صحیح ny, صحیح nx)
{
    اگر( nx == 1 && ny == 1 )
        بازگشت src;
    Mat dst;
    repeat(src, ny, nx, dst);
    بازگشت dst;
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
صحیح cv::borderInterpolate( صحیح p, صحیح len, صحیح borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    اگر(p >= 0 && p < len)
#وگرنه
    اگر( (unsigned)p < (unsigned)len )
#endif
        ;
    وگرنه اگر( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    وگرنه اگر( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        صحیح delta = borderType == BORDER_REFLECT_101;
        اگر( len == 1 )
            بازگشت 0;
        انجامبده
        {
            اگر( p < 0 )
                p = -p - 1 + delta;
            وگرنه
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        وقتی(p < 0 || p >= len);
#وگرنه
        وقتی( (unsigned)p >= (unsigned)len );
#endif
    }
    وگرنه اگر( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        اگر( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            اگر( p64 < 0 )
                p64 += len;
            p = (صحیح)p64;
        }
    }
    وگرنه اگر( borderType == BORDER_CONSTANT )
        p = -1;
    وگرنه
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    بازگشت p;
}

فضاینام
{

تهی copyMakeBorder_8u( ثابت uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        صحیح بالاترین_2, صحیح چپ, صحیح cn, صحیح borderType )
{
    ثابت صحیح isz = (صحیح)اندازه(صحیح);
    صحیح i, j, k, elemSize = 1;
    منطقی intMode = نادرست;

    اگر( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = درست;
    }

    cv::AutoBuffer<صحیح> _tab((dstroi.width - srcroi.width)*cn);
    صحیح* tab = _tab.data();
    صحیح راست = dstroi.width - srcroi.width - چپ;
    صحیح bottom = dstroi.height - srcroi.height - بالاترین_2;

    برای( i = 0; i < چپ; i++ )
    {
        j = cv::borderInterpolate(i - چپ, srcroi.width, borderType)*cn;
        برای( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    برای( i = 0; i < راست; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        برای( k = 0; k < cn; k++ )
            tab[(i+چپ)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    چپ *= cn;
    راست *= cn;

    uchar* dstInner = dst + dststep*بالاترین_2 + چپ*elemSize;

    برای( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        اگر( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        اگر( intMode )
        {
            ثابت صحیح* isrc = (صحیح*)src;
            صحیح* idstInner = (صحیح*)dstInner;
            برای( j = 0; j < چپ; j++ )
                idstInner[j - چپ] = isrc[tab[j]];
            برای( j = 0; j < راست; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + چپ]];
        }
        وگرنه
        {
            برای( j = 0; j < چپ; j++ )
                dstInner[j - چپ] = src[tab[j]];
            برای( j = 0; j < راست; j++ )
                dstInner[j + srcroi.width] = src[tab[j + چپ]];
        }
    }

    dstroi.width *= elemSize;

    برای( i = 0; i < بالاترین_2; i++ )
    {
        j = cv::borderInterpolate(i - بالاترین_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (بالاترین_2+j)*dststep, dstroi.width);
    }

    dst += dststep*بالاترین_2;
    برای( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


تهی copyMakeConstBorder_8u( ثابت uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             صحیح بالاترین_2, صحیح چپ, صحیح cn, ثابت uchar* value )
{
    صحیح i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    صحیح راست = dstroi.width - srcroi.width - چپ;
    صحیح bottom = dstroi.height - srcroi.height - بالاترین_2;

    برای( i = 0; i < dstroi.width; i++ )
    {
        برای( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    چپ *= cn;
    راست *= cn;

    uchar* dstInner = dst + dststep*بالاترین_2 + چپ;

    برای( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        اگر( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - چپ, constBuf, چپ );
        memcpy( dstInner + srcroi.width, constBuf, راست );
    }

    برای( i = 0; i < بالاترین_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (بالاترین_2 + srcroi.height) * dststep;
    برای( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

فضاینام cv {

ایستا منطقی ocl_copyMakeBorder( درون_داد_Array _src, OutputArray _dst, صحیح بالاترین_2, صحیح bottom,
                                صحیح چپ, صحیح راست, صحیح borderType, ثابت Scalar& value )
{
    صحیح type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    منطقی isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    اگر ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        بازگشت نادرست;

    ثابت نویسه * ثابت borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    صحیح scalarcn = cn == 3 ? 4 : cn;
    صحیح sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    اگر (k.empty())
        بازگشت نادرست;

    UMat src = _src.getUMat();
    اگر( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        صحیح dtop = std::min(ofs.y, بالاترین_2);
        صحیح dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        صحیح dleft = std::min(ofs.x, چپ);
        صحیح dright = std::min(wholeSize.width - src.cols - ofs.x, راست);
        src.adjustROI(dtop, dbottom, dleft, dright);
        بالاترین_2 -= dtop;
        چپ -= dleft;
        bottom -= dbottom;
        راست -= dright;
    }

    _dst.create(src.rows + بالاترین_2 + bottom, src.cols + چپ + راست, type);
    UMat dst = _dst.getUMat();

    اگر (بالاترین_2 == 0 && چپ == 0 && bottom == 0 && راست == 0)
    {
        اگر(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        بازگشت درست;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           بالاترین_2, چپ, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

}
#endif

#ifdef HAVE_IPP
فضاینام cv {

ایستا منطقی ipp_copyMakeBorder( Mat &_src, Mat &_dst, صحیح بالاترین_2, صحیح bottom,
                                صحیح چپ, صحیح راست, صحیح _borderType, ثابت Scalar& value )
{
#اگر defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(چپ, بالاترین_2, راست, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    اگر((صحیح)borderType == -1)
        بازگشت نادرست;

    اگر(_src.dims > 2)
        بازگشت نادرست;

    Rect dstRect(borderSize.چپ, borderSize.بالاترین_2,
        _dst.cols - borderSize.راست - borderSize.چپ,
        _dst.rows - borderSize.bottom - borderSize.بالاترین_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    بازگشت CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#وگرنه
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(بالاترین_2); CV_UNUSED(bottom); CV_UNUSED(چپ); CV_UNUSED(راست);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    بازگشت نادرست;
#endif
}
}
#endif

تهی cv::copyMakeBorder( درون_داد_Array _src, OutputArray _dst, صحیح بالاترین_2, صحیح bottom,
                         صحیح چپ, صحیح راست, صحیح borderType, ثابت Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( بالاترین_2 >= 0 && bottom >= 0 && چپ >= 0 && راست >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + بالاترین_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + چپ + راست <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, بالاترین_2, bottom, چپ, راست, borderType, value))
    صحیح type = src.type();

    اگر( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        صحیح dtop = std::min(ofs.y, بالاترین_2);
        صحیح dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        صحیح dleft = std::min(ofs.x, چپ);
        صحیح dright = std::min(wholeSize.width - src.cols - ofs.x, راست);
        src.adjustROI(dtop, dbottom, dleft, dright);
        بالاترین_2 -= dtop;
        چپ -= dleft;
        bottom -= dbottom;
        راست -= dright;
    }

    _dst.create( src.rows + بالاترین_2 + bottom, src.cols + چپ + راست, type );
    Mat dst = _dst.getMat();

    اگر(بالاترین_2 == 0 && چپ == 0 && bottom == 0 && راست == 0)
    {
        اگر(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        بازگشت;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, بالاترین_2, bottom, چپ, راست, borderType, value))

    اگر( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           بالاترین_2, چپ, (صحیح)src.elemSize(), borderType );
    وگرنه
    {
        صحیح cn = src.channels(), cn1 = cn;
        AutoBuffer<دوگانه> buf(cn);
        اگر( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                بالاترین_2, چپ, (صحیح)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
