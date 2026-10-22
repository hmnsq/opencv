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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"

நாமாகாஶ cv
{

template <typename T> நிலையான உள்வரி
வெறுமை scalarToRawData_(மாறிலி Scalar& s, T * மாறிலி buf, மாறிலி முழு cn, மாறிலி முழு unroll_to)
{
    முழு i = 0;
    க்கு(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    க்கு(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

வெறுமை scalarToRawData(மாறிலி Scalar& s, வெறுமை* _buf, முழு type, முழு unroll_to)
{
    CV_INSTRUMENT_REGION();

    மாறிலி முழு depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    தேர்வு(depth)
    {
    நிலை CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_Bool:
        scalarToRawData_(s, (தருக்க*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_32S:
        scalarToRawData_(s, (முழு*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_32F:
        scalarToRawData_(s, (தசம*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        நிறுத்து;
    நிலை CV_64F:
        scalarToRawData_(s, (இரட்டை*)_buf, cn, unroll_to);
        நிறுத்து;
    இயல்பு:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

வெறுமை convertAndUnrollScalar( மாறிலி Mat& sc, முழு buftype, uchar* scbuf, size_t blocksize )
{
    முழு scn = (முழு)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    இருமம்_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    எனில்( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        க்கு( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    க்கு( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> நிலையான வெறுமை
copyMask_(மாறிலி uchar* _src, size_t sstep, மாறிலி uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    க்கு( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        மாறிலி T* src = (மாறிலி T*)_src;
        T* dst = (T*)_dst;
        முழு x = 0;
         #எனில் CV_ENABLE_UNROLLED
        க்கு( ; x <= size.width - 4; x += 4 )
        {
            எனில்( mask[x] )
                dst[x] = src[x];
            எனில்( mask[x+1] )
                dst[x+1] = src[x+1];
            எனில்( mask[x+2] )
                dst[x+2] = src[x+2];
            எனில்( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        க்கு( ; x < size.width; x++ )
            எனில்( mask[x] )
                dst[x] = src[x];
    }
}

template<> வெறுமை
copyMask_<uchar>(மாறிலி uchar* _src, size_t sstep, மாறிலி uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (முழு)sstep, _dst, (முழு)dstep, ippiSize(size), mask, (முழு)mstep) >= 0)

    க்கு( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        மாறிலி uchar* src = (மாறிலி uchar*)_src;
        uchar* dst = (uchar*)_dst;
        முழு x = 0;
        #எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            க்கு( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        க்கு( ; x < size.width; x++ )
            எனில்( mask[x] )
                dst[x] = src[x];
    }
}

template<> வெறுமை
copyMask_<Vec3b>(மாறிலி uchar* _src, size_t sstep, மாறிலி uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    க்கு( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        மாறிலி uchar* src = (மாறிலி uchar*)_src;
        uchar* dst = (uchar*)_dst;
        முழு x = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        க்கு( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        க்கு( ; x < size.width; x++ )
            எனில்( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> வெறுமை
copyMask_<ushort>(மாறிலி uchar* _src, size_t sstep, மாறிலி uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (மாறிலி Ipp16u *)_src, (முழு)sstep, (Ipp16u *)_dst, (முழு)dstep, ippiSize(size), mask, (முழு)mstep) >= 0)

    க்கு( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        மாறிலி ushort* src = (மாறிலி ushort*)_src;
        ushort* dst = (ushort*)_dst;
        முழு x = 0;
        #எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            க்கு( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        க்கு( ; x < size.width; x++ )
            எனில்( mask[x] )
                dst[x] = src[x];
    }
}

template<> வெறுமை
copyMask_<Vec3s>(மாறிலி uchar* _src, size_t sstep, மாறிலி uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    க்கு( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        மாறிலி ushort* src = (மாறிலி ushort*)_src;
        ushort* dst = (ushort*)_dst;
        முழு x = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        க்கு( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        க்கு( ; x < size.width; x++ )
            எனில்( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> வெறுமை
copyMask_<முழு>(மாறிலி uchar* _src, size_t sstep, மாறிலி uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    க்கு( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        மாறிலி முழு* src = (மாறிலி முழு*)_src;
        முழு* dst = (முழு*)_dst;
        முழு x = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        க்கு (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        க்கு (; x < size.width; x++)
            எனில் ( mask[x] )
                dst[x] = src[x];
    }
}

நிலையான வெறுமை
copyMaskGeneric(மாறிலி uchar* _src, size_t sstep, மாறிலி uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, வெறுமை* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    க்கு( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        மாறிலி uchar* src = _src;
        uchar* dst = _dst;
        முழு x = 0;
        க்கு( ; x < size.width; x++, src += esz, dst += esz )
        {
            எனில்( !mask[x] )
                தொடர்;
            க்கு( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
நிலையான வெறுமை copyMask##suffix(மாறிலி uchar* src, size_t sstep, மாறிலி uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, வெறுமை*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#எனில் defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
நிலையான வெறுமை copyMask##suffix(மாறிலி uchar* src, size_t sstep, மாறிலி uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, வெறுமை*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (மாறிலி ipptype *)src, (முழு)sstep, (ipptype *)dst, (முழு)dstep, ippiSize(size), (மாறிலி Ipp8u *)mask, (முழு)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#இல்லையெனில்
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
நிலையான வெறுமை copyMask##suffix(மாறிலி uchar* src, size_t sstep, மாறிலி uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, வெறுமை*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#எனில் IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#இல்லையெனில்
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, முழு, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

இருமம்_Func copyMaskTab[] =
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

இருமம்_Func getCopyMaskFunc(size_t esz)
{
    திரும்பு esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
வெறுமை Mat::copyTo( OutputArray _dst ) மாறிலி
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    எனில் (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*இது);
        திரும்பு;
    }
#endif

    முழு stype = type();
    முழு dtype = _dst.type();
    எனில்( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        திரும்பு;
    }

    எனில்( dims == 0 && empty() )
    {
        _dst.release();
        வெறுமை* obj = _dst.getObj();
        எனில் (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        இல்லையெனில் எனில் (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        இல்லையெனில் எனில் (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        திரும்பு;
    }

    தருக்க allowTransposed = dims == 1 ||
        _dst.kind() == உள்ளீடு_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    எனில்( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        எனில் (empty())
            திரும்பு;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != வெற்று);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        க்கு( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        முழு lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        திரும்பு;
    }

    எனில்( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        எனில்( data == dst.data )
            திரும்பு;

        எனில்( rows > 0 && cols > 0 )
        {
            Mat src = *இது;
            Size sz = getContinuousSize2D(src, dst, (முழு)elemSize());
            CV_CheckGE(sz.width, 0, "");

            மாறிலி uchar* sptr = src.data;
            uchar* dptr = dst.data;

#எனில் IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (முழு)src.step, dptr, (முழு)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            க்கு (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        திரும்பு;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    எனில்( data == dst.data )
        திரும்பு;

    எனில்( total() != 0 )
    {
        மாறிலி Mat* arrays[] = { இது, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
நிலையான தருக்க ipp_copyTo(மாறிலி Mat &src, Mat &dst, மாறிலி Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    எனில்(mask.channels() > 1 || mask.depth() != CV_8U)
        திரும்பு பொய்;

    எனில் (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        திரும்பு CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (முழு)src.step, dst.ptr(), (முழு)dst.step, size, (முழு)src.elemSize1(), src.channels(), mask.ptr(), (முழு)mask.step) >= 0;
    }
    இல்லையெனில்
    {
        மாறிலி Mat      *arrays[] = {&src, &dst, &mask, வெற்று};
        uchar          *ptrs[3]  = {வெற்று};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        க்கு (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (முழு)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                திரும்பு பொய்;
        }
        திரும்பு உண்மை;
    }
#இல்லையெனில்
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    திரும்பு பொய்;
#endif
}
#endif

வெறுமை Mat::copyTo( OutputArray _dst, உள்ளீடு_Array _mask ) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    எனில்( !mask.data )
    {
        copyTo(_dst);
        திரும்பு;
    }

    முழு cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    தருக்க colorMask = mcn > 1;
    எனில்( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        எனில் (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*இது, dst, mask))
    எனில் ( இது->dims <= 2 ) {
        எனில் ( இது->size() == dst.size() && இது->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, இது->data, இது->step, dst.data, dst.step, இது->cols, இது->rows, இது->type(), mask.data, mask.step, mask.type());
        }
    }
    இல்லையெனில் எனில் ( இது->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = இது->total();
        எனில் (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, இது->data, 0, dst.data, 0, (முழு)sz, 1, இது->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    இருமம்_Func copymask = getCopyMaskFunc(esz);

    எனில்( dims <= 2 )
    {
        Mat src = *இது;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        திரும்பு;
    }

    மாறிலி Mat* arrays[] = { இது, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((முழு)(it.size*mcn), 1);

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
வெறுமை Mat::copyAt( OutputArray _dst ) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
வெறுமை Mat::copyAt( OutputArray _dst, உள்ளீடு_Array _mask ) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


நிலையான தருக்க can_apply_memset(மாறிலி Mat &mat, மாறிலி Scalar &s, முழு &fill_மதிப்பு)
{
    // check if depth is 1 byte.
    தேர்வு (mat.depth())
    {
    நிலை CV_8U: fill_மதிப்பு = saturate_cast<uchar>( s.val[0] ); நிறுத்து;
    நிலை CV_8S: fill_மதிப்பு = saturate_cast<schar>( s.val[0] ); நிறுத்து;
    இயல்பு: திரும்பு பொய்;
    }

    // check if all element is same.
    மாறிலி int64* is = (மாறிலி int64*)&s.val[0];
    தேர்வு (mat.channels())
    {
    நிலை 1: திரும்பு உண்மை;
    நிலை 2: திரும்பு (is[0] == is[1]);
    நிலை 3: திரும்பு (is[0] == is[1] && is[1] == is[2]);
    நிலை 4: திரும்பு (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    இயல்பு: திரும்பு பொய்;
    }
}

Mat& Mat::operator = (மாறிலி Scalar& s)
{
    CV_INSTRUMENT_REGION();

    எனில் (இது->empty())
        திரும்பு *இது;

    மாறிலி Mat* arrays[] = { இது };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    மாறிலி int64* is = (மாறிலி int64*)&s.val[0];

    எனில்( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    இல்லையெனில்
    {
        முழு fill_மதிப்பு = 0;
        எனில் ( can_apply_memset(*இது, s, fill_மதிப்பு) )
        {
            க்கு (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_மதிப்பு, elsize);
            திரும்பு *இது;
        }

        எனில்( it.nplanes > 0 )
        {
            இரட்டை scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            க்கு( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= அளவு(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        க்கு( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    திரும்பு *இது;
}

#ifdef HAVE_IPP
நிலையான தருக்க ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    எனில்(mask.empty())
        திரும்பு பொய்;

    எனில்(mask.depth() != CV_8U || mask.channels() > 1)
        திரும்பு பொய்;

    எனில்(dst.channels() > 4)
        திரும்பு பொய்;

    எனில் (dst.depth() == CV_32F)
    {
        க்கு (முழு i = 0; i < (முழு)(_val.total()); i++)
        {
            தசம v = (தசம)(_val.at<இரட்டை>(i));  // cast to float
            எனில் (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                திரும்பு பொய்;
        }
    }

    எனில்(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_மதிப்பு_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        திரும்பு CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (முழு)dst.step, size, dataType, dst.channels(), mask.ptr(), (முழு)mask.step) >= 0;
    }
    இல்லையெனில்
    {
        மாறிலி Mat      *arrays[] = {&dst, mask.empty()?வெற்று:&mask, வெற்று};
        uchar          *ptrs[2]  = {வெற்று};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(முழு)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_மதிப்பு_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                திரும்பு பொய்;
        }
        திரும்பு உண்மை;
    }
#இல்லையெனில்
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    திரும்பு பொய்;
#endif
}
#endif

Mat& Mat::setTo(உள்ளீடு_Array மதிப்பு, உள்ளீடு_Array _mask)
{
    CV_INSTRUMENT_REGION();

    எனில்( empty() )
        திரும்பு *இது;

    Mat value = மதிப்பு.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), மதிப்பு.kind(), உள்ளீடு_Array_2::MAT ));
    முழு cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*இது, value, mask), *இது)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    இருமம்_Func copymask = getCopyMaskFunc(esz);

    மாறிலி Mat* arrays[] = { இது, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    முழு totalsz = (முழு)it.size*mcn;
    முழு blockSize0 = std::min(totalsz, (முழு)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (முழு)அளவு(இரட்டை));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( முழு j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            எனில்( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            இல்லையெனில்
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    திரும்பு *இது;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    எனில்( empty() )
        திரும்பு *இது;

    size_t esz = elemSize();

    மாறிலி Mat* arrays[] = { இது, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    திரும்பு *இது;
}


#எனில் defined HAVE_OPENCL && !defined __APPLE__

நிலையான தருக்க ocl_repeat(உள்ளீடு_Array _src, முழு ny, முழு nx, OutputArray _dst)
{
    எனில் (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        திரும்பு உண்மை;
    }

    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

வெறுமை repeat(உள்ளீடு_Array _src, முழு ny, முழு nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    எனில்( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        திரும்பு;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#எனில் !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    முழு esz = (முழு)src.elemSize();
    முழு x, y;
    ssize.width *= esz; dsize.width *= esz;

    க்கு( y = 0; y < ssize.height; y++ )
    {
        க்கு( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    க்கு( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(மாறிலி Mat& src, முழு ny, முழு nx)
{
    எனில்( nx == 1 && ny == 1 )
        திரும்பு src;
    Mat dst;
    repeat(src, ny, nx, dst);
    திரும்பு dst;
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
முழு cv::borderInterpolate( முழு p, முழு len, முழு borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    எனில்(p >= 0 && p < len)
#இல்லையெனில்
    எனில்( (unsigned)p < (unsigned)len )
#endif
        ;
    இல்லையெனில் எனில்( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    இல்லையெனில் எனில்( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        முழு delta = borderType == BORDER_REFLECT_101;
        எனில்( len == 1 )
            திரும்பு 0;
        செய்
        {
            எனில்( p < 0 )
                p = -p - 1 + delta;
            இல்லையெனில்
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        போது(p < 0 || p >= len);
#இல்லையெனில்
        போது( (unsigned)p >= (unsigned)len );
#endif
    }
    இல்லையெனில் எனில்( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        எனில்( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            எனில்( p64 < 0 )
                p64 += len;
            p = (முழு)p64;
        }
    }
    இல்லையெனில் எனில்( borderType == BORDER_CONSTANT )
        p = -1;
    இல்லையெனில்
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    திரும்பு p;
}

நாமாகாஶ
{

வெறுமை copyMakeBorder_8u( மாறிலி uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        முழு உச்சி_2, முழு இடது, முழு cn, முழு borderType )
{
    மாறிலி முழு isz = (முழு)அளவு(முழு);
    முழு i, j, k, elemSize = 1;
    தருக்க intMode = பொய்;

    எனில்( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = உண்மை;
    }

    cv::AutoBuffer<முழு> _tab((dstroi.width - srcroi.width)*cn);
    முழு* tab = _tab.data();
    முழு வலது = dstroi.width - srcroi.width - இடது;
    முழு bottom = dstroi.height - srcroi.height - உச்சி_2;

    க்கு( i = 0; i < இடது; i++ )
    {
        j = cv::borderInterpolate(i - இடது, srcroi.width, borderType)*cn;
        க்கு( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    க்கு( i = 0; i < வலது; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        க்கு( k = 0; k < cn; k++ )
            tab[(i+இடது)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    இடது *= cn;
    வலது *= cn;

    uchar* dstInner = dst + dststep*உச்சி_2 + இடது*elemSize;

    க்கு( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        எனில்( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        எனில்( intMode )
        {
            மாறிலி முழு* isrc = (முழு*)src;
            முழு* idstInner = (முழு*)dstInner;
            க்கு( j = 0; j < இடது; j++ )
                idstInner[j - இடது] = isrc[tab[j]];
            க்கு( j = 0; j < வலது; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + இடது]];
        }
        இல்லையெனில்
        {
            க்கு( j = 0; j < இடது; j++ )
                dstInner[j - இடது] = src[tab[j]];
            க்கு( j = 0; j < வலது; j++ )
                dstInner[j + srcroi.width] = src[tab[j + இடது]];
        }
    }

    dstroi.width *= elemSize;

    க்கு( i = 0; i < உச்சி_2; i++ )
    {
        j = cv::borderInterpolate(i - உச்சி_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (உச்சி_2+j)*dststep, dstroi.width);
    }

    dst += dststep*உச்சி_2;
    க்கு( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


வெறுமை copyMakeConstBorder_8u( மாறிலி uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             முழு உச்சி_2, முழு இடது, முழு cn, மாறிலி uchar* value )
{
    முழு i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    முழு வலது = dstroi.width - srcroi.width - இடது;
    முழு bottom = dstroi.height - srcroi.height - உச்சி_2;

    க்கு( i = 0; i < dstroi.width; i++ )
    {
        க்கு( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    இடது *= cn;
    வலது *= cn;

    uchar* dstInner = dst + dststep*உச்சி_2 + இடது;

    க்கு( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        எனில்( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - இடது, constBuf, இடது );
        memcpy( dstInner + srcroi.width, constBuf, வலது );
    }

    க்கு( i = 0; i < உச்சி_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (உச்சி_2 + srcroi.height) * dststep;
    க்கு( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

நாமாகாஶ cv {

நிலையான தருக்க ocl_copyMakeBorder( உள்ளீடு_Array _src, OutputArray _dst, முழு உச்சி_2, முழு bottom,
                                முழு இடது, முழு வலது, முழு borderType, மாறிலி Scalar& value )
{
    முழு type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    தருக்க isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    எனில் ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        திரும்பு பொய்;

    மாறிலி எழுத்து * மாறிலி borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    முழு scalarcn = cn == 3 ? 4 : cn;
    முழு sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src = _src.getUMat();
    எனில்( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        முழு dtop = std::min(ofs.y, உச்சி_2);
        முழு dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        முழு dleft = std::min(ofs.x, இடது);
        முழு dright = std::min(wholeSize.width - src.cols - ofs.x, வலது);
        src.adjustROI(dtop, dbottom, dleft, dright);
        உச்சி_2 -= dtop;
        இடது -= dleft;
        bottom -= dbottom;
        வலது -= dright;
    }

    _dst.create(src.rows + உச்சி_2 + bottom, src.cols + இடது + வலது, type);
    UMat dst = _dst.getUMat();

    எனில் (உச்சி_2 == 0 && இடது == 0 && bottom == 0 && வலது == 0)
    {
        எனில்(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        திரும்பு உண்மை;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           உச்சி_2, இடது, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

}
#endif

#ifdef HAVE_IPP
நாமாகாஶ cv {

நிலையான தருக்க ipp_copyMakeBorder( Mat &_src, Mat &_dst, முழு உச்சி_2, முழு bottom,
                                முழு இடது, முழு வலது, முழு _borderType, மாறிலி Scalar& value )
{
#எனில் defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(இடது, உச்சி_2, வலது, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    எனில்((முழு)borderType == -1)
        திரும்பு பொய்;

    எனில்(_src.dims > 2)
        திரும்பு பொய்;

    Rect dstRect(borderSize.இடது, borderSize.உச்சி_2,
        _dst.cols - borderSize.வலது - borderSize.இடது,
        _dst.rows - borderSize.bottom - borderSize.உச்சி_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    திரும்பு CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#இல்லையெனில்
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(உச்சி_2); CV_UNUSED(bottom); CV_UNUSED(இடது); CV_UNUSED(வலது);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    திரும்பு பொய்;
#endif
}
}
#endif

வெறுமை cv::copyMakeBorder( உள்ளீடு_Array _src, OutputArray _dst, முழு உச்சி_2, முழு bottom,
                         முழு இடது, முழு வலது, முழு borderType, மாறிலி Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( உச்சி_2 >= 0 && bottom >= 0 && இடது >= 0 && வலது >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + உச்சி_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + இடது + வலது <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, உச்சி_2, bottom, இடது, வலது, borderType, value))
    முழு type = src.type();

    எனில்( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        முழு dtop = std::min(ofs.y, உச்சி_2);
        முழு dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        முழு dleft = std::min(ofs.x, இடது);
        முழு dright = std::min(wholeSize.width - src.cols - ofs.x, வலது);
        src.adjustROI(dtop, dbottom, dleft, dright);
        உச்சி_2 -= dtop;
        இடது -= dleft;
        bottom -= dbottom;
        வலது -= dright;
    }

    _dst.create( src.rows + உச்சி_2 + bottom, src.cols + இடது + வலது, type );
    Mat dst = _dst.getMat();

    எனில்(உச்சி_2 == 0 && இடது == 0 && bottom == 0 && வலது == 0)
    {
        எனில்(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        திரும்பு;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, உச்சி_2, bottom, இடது, வலது, borderType, value))

    எனில்( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           உச்சி_2, இடது, (முழு)src.elemSize(), borderType );
    இல்லையெனில்
    {
        முழு cn = src.channels(), cn1 = cn;
        AutoBuffer<இரட்டை> buf(cn);
        எனில்( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                உச்சி_2, இடது, (முழு)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
