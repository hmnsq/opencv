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

#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

namespace cv
{

template <typename T> static inline
void scalarToRawData_(const Scalar& s, T * const buf, const 정수 cn, const 정수 unroll_to)
{
    정수 i = 0;
    위해(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    위해(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

void scalarToRawData(const Scalar& s, void* _buf, 정수 type, 정수 unroll_to)
{
    CV_INSTRUMENT_REGION();

    const 정수 depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    switch(depth)
    {
    case CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        break;
    case CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        break;
    case CV_Bool:
        scalarToRawData_(s, (bool*)_buf, cn, unroll_to);
        break;
    case CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        break;
    case CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        break;
    case CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        break;
    case CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        break;
    case CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        break;
    case CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        break;
    case CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        break;
    case CV_32S:
        scalarToRawData_(s, (정수*)_buf, cn, unroll_to);
        break;
    case CV_32F:
        scalarToRawData_(s, (float*)_buf, cn, unroll_to);
        break;
    case CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        break;
    case CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        break;
    case CV_64F:
        scalarToRawData_(s, (double*)_buf, cn, unroll_to);
        break;
    default:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

void convertAndUnrollScalar( const Mat& sc, 정수 buftype, uchar* scbuf, size_t blocksize )
{
    정수 scn = (정수)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    BinaryFunc cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    만약( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        위해( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    위해( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> static void
copyMask_(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    위해( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const T* src = (const T*)_src;
        T* dst = (T*)_dst;
        정수 x = 0;
         #만약 CV_ENABLE_UNROLLED
        위해( ; x <= size.width - 4; x += 4 )
        {
            만약( mask[x] )
                dst[x] = src[x];
            만약( mask[x+1] )
                dst[x+1] = src[x+1];
            만약( mask[x+2] )
                dst[x+2] = src[x+2];
            만약( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        위해( ; x < size.width; x++ )
            만약( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<uchar>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (정수)sstep, _dst, (정수)dstep, ippiSize(size), mask, (정수)mstep) >= 0)

    위해( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = (const uchar*)_src;
        uchar* dst = (uchar*)_dst;
        정수 x = 0;
        #만약 (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            위해( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        위해( ; x < size.width; x++ )
            만약( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<Vec3b>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    위해( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = (const uchar*)_src;
        uchar* dst = (uchar*)_dst;
        정수 x = 0;
#만약 (CV_SIMD || CV_SIMD_SCALABLE)
        위해( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        위해( ; x < size.width; x++ )
            만약( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> void
copyMask_<ushort>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (const Ipp16u *)_src, (정수)sstep, (Ipp16u *)_dst, (정수)dstep, ippiSize(size), mask, (정수)mstep) >= 0)

    위해( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const ushort* src = (const ushort*)_src;
        ushort* dst = (ushort*)_dst;
        정수 x = 0;
        #만약 (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            위해( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        위해( ; x < size.width; x++ )
            만약( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<Vec3s>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    위해( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const ushort* src = (const ushort*)_src;
        ushort* dst = (ushort*)_dst;
        정수 x = 0;
#만약 (CV_SIMD || CV_SIMD_SCALABLE)
        위해( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        위해( ; x < size.width; x++ )
            만약( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> void
copyMask_<정수>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    위해( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const 정수* src = (const 정수*)_src;
        정수* dst = (정수*)_dst;
        정수 x = 0;
#만약 (CV_SIMD || CV_SIMD_SCALABLE)
        위해 (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        위해 (; x < size.width; x++)
            만약 ( mask[x] )
                dst[x] = src[x];
    }
}

static void
copyMaskGeneric(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, void* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    위해( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = _src;
        uchar* dst = _dst;
        정수 x = 0;
        위해( ; x < size.width; x++, src += esz, dst += esz )
        {
            만약( !mask[x] )
                continue;
            위해( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#만약 defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (const ipptype *)src, (정수)sstep, (ipptype *)dst, (정수)dstep, ippiSize(size), (const Ipp8u *)mask, (정수)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#아니면
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#만약 IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#아니면
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, 정수, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

BinaryFunc copyMaskTab[] =
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

BinaryFunc getCopyMaskFunc(size_t esz)
{
    반환 esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
void Mat::copyTo( OutputArray _dst ) const
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    만약 (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*this);
        반환;
    }
#endif

    정수 stype = type();
    정수 dtype = _dst.type();
    만약( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        반환;
    }

    만약( dims == 0 && empty() )
    {
        _dst.release();
        void* obj = _dst.getObj();
        만약 (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        아니면 만약 (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        아니면 만약 (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        반환;
    }

    bool allowTransposed = dims == 1 ||
        _dst.kind() == _InputArray::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    만약( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        만약 (empty())
            반환;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != NULL);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        위해( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        정수 lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        반환;
    }

    만약( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        만약( data == dst.data )
            반환;

        만약( rows > 0 && cols > 0 )
        {
            Mat src = *this;
            Size sz = getContinuousSize2D(src, dst, (정수)elemSize());
            CV_CheckGE(sz.width, 0, "");

            const uchar* sptr = src.data;
            uchar* dptr = dst.data;

#만약 IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (정수)src.step, dptr, (정수)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            위해 (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        반환;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    만약( data == dst.data )
        반환;

    만약( total() != 0 )
    {
        const Mat* arrays[] = { this, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        위해( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
static bool ipp_copyTo(const Mat &src, Mat &dst, const Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    만약(mask.channels() > 1 || mask.depth() != CV_8U)
        반환 false;

    만약 (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        반환 CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (정수)src.step, dst.ptr(), (정수)dst.step, size, (정수)src.elemSize1(), src.channels(), mask.ptr(), (정수)mask.step) >= 0;
    }
    아니면
    {
        const Mat      *arrays[] = {&src, &dst, &mask, NULL};
        uchar          *ptrs[3]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        위해 (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            만약(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (정수)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                반환 false;
        }
        반환 true;
    }
#아니면
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    반환 false;
#endif
}
#endif

void Mat::copyTo( OutputArray _dst, InputArray _mask ) const
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    만약( !mask.data )
    {
        copyTo(_dst);
        반환;
    }

    정수 cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    bool colorMask = mcn > 1;
    만약( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        만약 (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*this, dst, mask))
    만약 ( this->dims <= 2 ) {
        만약 ( this->size() == dst.size() && this->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, this->data, this->step, dst.data, dst.step, this->cols, this->rows, this->type(), mask.data, mask.step, mask.type());
        }
    }
    아니면 만약 ( this->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = this->total();
        만약 (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, this->data, 0, dst.data, 0, (정수)sz, 1, this->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    만약( dims <= 2 )
    {
        Mat src = *this;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        반환;
    }

    const Mat* arrays[] = { this, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((정수)(it.size*mcn), 1);

    위해( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
void Mat::copyAt( OutputArray _dst ) const
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
void Mat::copyAt( OutputArray _dst, InputArray _mask ) const
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


static bool can_apply_memset(const Mat &mat, const Scalar &s, 정수 &fill_value)
{
    // check if depth is 1 byte.
    switch (mat.depth())
    {
    case CV_8U: fill_value = saturate_cast<uchar>( s.val[0] ); break;
    case CV_8S: fill_value = saturate_cast<schar>( s.val[0] ); break;
    default: 반환 false;
    }

    // check if all element is same.
    const int64* is = (const int64*)&s.val[0];
    switch (mat.channels())
    {
    case 1: 반환 true;
    case 2: 반환 (is[0] == is[1]);
    case 3: 반환 (is[0] == is[1] && is[1] == is[2]);
    case 4: 반환 (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    default: 반환 false;
    }
}

Mat& Mat::operator = (const Scalar& s)
{
    CV_INSTRUMENT_REGION();

    만약 (this->empty())
        반환 *this;

    const Mat* arrays[] = { this };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    const int64* is = (const int64*)&s.val[0];

    만약( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        위해( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    아니면
    {
        정수 fill_value = 0;
        만약 ( can_apply_memset(*this, s, fill_value) )
        {
            위해 (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_value, elsize);
            반환 *this;
        }

        만약( it.nplanes > 0 )
        {
            double scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            위해( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= sizeof(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        위해( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    반환 *this;
}

#ifdef HAVE_IPP
static bool ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    만약(mask.empty())
        반환 false;

    만약(mask.depth() != CV_8U || mask.channels() > 1)
        반환 false;

    만약(dst.channels() > 4)
        반환 false;

    만약 (dst.depth() == CV_32F)
    {
        위해 (정수 i = 0; i < (정수)(_val.total()); i++)
        {
            float v = (float)(_val.at<double>(i));  // cast to float
            만약 (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                반환 false;
        }
    }

    만약(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::IwValueFloat s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        반환 CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (정수)dst.step, size, dataType, dst.channels(), mask.ptr(), (정수)mask.step) >= 0;
    }
    아니면
    {
        const Mat      *arrays[] = {&dst, mask.empty()?NULL:&mask, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(정수)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::IwValueFloat s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        위해( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            만약(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                반환 false;
        }
        반환 true;
    }
#아니면
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    반환 false;
#endif
}
#endif

Mat& Mat::setTo(InputArray _value, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    만약( empty() )
        반환 *this;

    Mat value = _value.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), _value.kind(), _InputArray::MAT ));
    정수 cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*this, value, mask), *this)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    const Mat* arrays[] = { this, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    정수 totalsz = (정수)it.size*mcn;
    정수 blockSize0 = std::min(totalsz, (정수)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (정수)sizeof(double));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    위해( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        위해( 정수 j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            만약( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            아니면
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    반환 *this;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    만약( empty() )
        반환 *this;

    size_t esz = elemSize();

    const Mat* arrays[] = { this, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    위해( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    반환 *this;
}


#만약 defined HAVE_OPENCL && !defined __APPLE__

static bool ocl_repeat(InputArray _src, 정수 ny, 정수 nx, OutputArray _dst)
{
    만약 (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        반환 true;
    }

    정수 type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    만약 (k.empty())
        반환 false;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    반환 k.run(2, globalsize, NULL, false);
}

#endif

void repeat(InputArray _src, 정수 ny, 정수 nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    만약( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        반환;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#만약 !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    정수 esz = (정수)src.elemSize();
    정수 x, y;
    ssize.width *= esz; dsize.width *= esz;

    위해( y = 0; y < ssize.height; y++ )
    {
        위해( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    위해( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(const Mat& src, 정수 ny, 정수 nx)
{
    만약( nx == 1 && ny == 1 )
        반환 src;
    Mat dst;
    repeat(src, ny, nx, dst);
    반환 dst;
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
정수 cv::borderInterpolate( 정수 p, 정수 len, 정수 borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    만약(p >= 0 && p < len)
#아니면
    만약( (unsigned)p < (unsigned)len )
#endif
        ;
    아니면 만약( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    아니면 만약( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        정수 delta = borderType == BORDER_REFLECT_101;
        만약( len == 1 )
            반환 0;
        do
        {
            만약( p < 0 )
                p = -p - 1 + delta;
            아니면
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        동안(p < 0 || p >= len);
#아니면
        동안( (unsigned)p >= (unsigned)len );
#endif
    }
    아니면 만약( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        만약( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            만약( p64 < 0 )
                p64 += len;
            p = (정수)p64;
        }
    }
    아니면 만약( borderType == BORDER_CONSTANT )
        p = -1;
    아니면
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    반환 p;
}

namespace
{

void copyMakeBorder_8u( const uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        정수 top, 정수 left, 정수 cn, 정수 borderType )
{
    const 정수 isz = (정수)sizeof(정수);
    정수 i, j, k, elemSize = 1;
    bool intMode = false;

    만약( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = true;
    }

    cv::AutoBuffer<정수> _tab((dstroi.width - srcroi.width)*cn);
    정수* tab = _tab.data();
    정수 right = dstroi.width - srcroi.width - left;
    정수 bottom = dstroi.height - srcroi.height - top;

    위해( i = 0; i < left; i++ )
    {
        j = cv::borderInterpolate(i - left, srcroi.width, borderType)*cn;
        위해( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    위해( i = 0; i < right; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        위해( k = 0; k < cn; k++ )
            tab[(i+left)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + dststep*top + left*elemSize;

    위해( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        만약( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        만약( intMode )
        {
            const 정수* isrc = (정수*)src;
            정수* idstInner = (정수*)dstInner;
            위해( j = 0; j < left; j++ )
                idstInner[j - left] = isrc[tab[j]];
            위해( j = 0; j < right; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + left]];
        }
        아니면
        {
            위해( j = 0; j < left; j++ )
                dstInner[j - left] = src[tab[j]];
            위해( j = 0; j < right; j++ )
                dstInner[j + srcroi.width] = src[tab[j + left]];
        }
    }

    dstroi.width *= elemSize;

    위해( i = 0; i < top; i++ )
    {
        j = cv::borderInterpolate(i - top, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (top+j)*dststep, dstroi.width);
    }

    dst += dststep*top;
    위해( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


void copyMakeConstBorder_8u( const uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             정수 top, 정수 left, 정수 cn, const uchar* value )
{
    정수 i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    정수 right = dstroi.width - srcroi.width - left;
    정수 bottom = dstroi.height - srcroi.height - top;

    위해( i = 0; i < dstroi.width; i++ )
    {
        위해( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + dststep*top + left;

    위해( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        만약( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - left, constBuf, left );
        memcpy( dstInner + srcroi.width, constBuf, right );
    }

    위해( i = 0; i < top; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (top + srcroi.height) * dststep;
    위해( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

namespace cv {

static bool ocl_copyMakeBorder( InputArray _src, OutputArray _dst, 정수 top, 정수 bottom,
                                정수 left, 정수 right, 정수 borderType, const Scalar& value )
{
    정수 type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    bool isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    만약 ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        반환 false;

    const char * const borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    정수 scalarcn = cn == 3 ? 4 : cn;
    정수 sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    만약 (k.empty())
        반환 false;

    UMat src = _src.getUMat();
    만약( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        정수 dtop = std::min(ofs.y, top);
        정수 dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        정수 dleft = std::min(ofs.x, left);
        정수 dright = std::min(wholeSize.width - src.cols - ofs.x, right);
        src.adjustROI(dtop, dbottom, dleft, dright);
        top -= dtop;
        left -= dleft;
        bottom -= dbottom;
        right -= dright;
    }

    _dst.create(src.rows + top + bottom, src.cols + left + right, type);
    UMat dst = _dst.getUMat();

    만약 (top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        만약(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        반환 true;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           top, left, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    반환 k.run(2, globalsize, NULL, false);
}

}
#endif

#ifdef HAVE_IPP
namespace cv {

static bool ipp_copyMakeBorder( Mat &_src, Mat &_dst, 정수 top, 정수 bottom,
                                정수 left, 정수 right, 정수 _borderType, const Scalar& value )
{
#만약 defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(left, top, right, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    만약((정수)borderType == -1)
        반환 false;

    만약(_src.dims > 2)
        반환 false;

    Rect dstRect(borderSize.left, borderSize.top,
        _dst.cols - borderSize.right - borderSize.left,
        _dst.rows - borderSize.bottom - borderSize.top);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    반환 CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#아니면
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(top); CV_UNUSED(bottom); CV_UNUSED(left); CV_UNUSED(right);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    반환 false;
#endif
}
}
#endif

void cv::copyMakeBorder( InputArray _src, OutputArray _dst, 정수 top, 정수 bottom,
                         정수 left, 정수 right, 정수 borderType, const Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( top >= 0 && bottom >= 0 && left >= 0 && right >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + top + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + left + right <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, top, bottom, left, right, borderType, value))
    정수 type = src.type();

    만약( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        정수 dtop = std::min(ofs.y, top);
        정수 dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        정수 dleft = std::min(ofs.x, left);
        정수 dright = std::min(wholeSize.width - src.cols - ofs.x, right);
        src.adjustROI(dtop, dbottom, dleft, dright);
        top -= dtop;
        left -= dleft;
        bottom -= dbottom;
        right -= dright;
    }

    _dst.create( src.rows + top + bottom, src.cols + left + right, type );
    Mat dst = _dst.getMat();

    만약(top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        만약(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        반환;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, top, bottom, left, right, borderType, value))

    만약( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           top, left, (정수)src.elemSize(), borderType );
    아니면
    {
        정수 cn = src.channels(), cn1 = cn;
        AutoBuffer<double> buf(cn);
        만약( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                top, left, (정수)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
