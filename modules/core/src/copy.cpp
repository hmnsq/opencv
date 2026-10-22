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

template <typename T> static anta_hstha
void scalarToRawData_(const Scalar& s, T * const buf, const ကိန်းပြည့် cn, const ကိန်းပြည့် unroll_to)
{
    ကိန်းပြည့် i = 0;
    အတွက်(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    အတွက်(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

void scalarToRawData(const Scalar& s, void* _buf, ကိန်းပြည့် type, ကိန်းပြည့် unroll_to)
{
    CV_INSTRUMENT_REGION();

    const ကိန်းပြည့် depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    switch(depth)
    {
    pak_sa CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_Bool:
        scalarToRawData_(s, (bool*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_32S:
        scalarToRawData_(s, (ကိန်းပြည့်*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_32F:
        scalarToRawData_(s, (float*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        bha_nga;
    pak_sa CV_64F:
        scalarToRawData_(s, (dviguu_na*)_buf, cn, unroll_to);
        bha_nga;
    default:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

void convertAndUnrollScalar( const Mat& sc, ကိန်းပြည့် buftype, uchar* scbuf, size_t blocksize )
{
    ကိန်းပြည့် scn = (ကိန်းပြည့်)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    BinaryFunc cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    အကယ်၍( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        အတွက်( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    အတွက်( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> static void
copyMask_(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    အတွက်( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const T* src = (const T*)_src;
        T* dst = (T*)_dst;
        ကိန်းပြည့် x = 0;
         #အကယ်၍ CV_ENABLE_UNROLLED
        အတွက်( ; x <= size.width - 4; x += 4 )
        {
            အကယ်၍( mask[x] )
                dst[x] = src[x];
            အကယ်၍( mask[x+1] )
                dst[x+1] = src[x+1];
            အကယ်၍( mask[x+2] )
                dst[x+2] = src[x+2];
            အကယ်၍( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        အတွက်( ; x < size.width; x++ )
            အကယ်၍( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<uchar>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (ကိန်းပြည့်)sstep, _dst, (ကိန်းပြည့်)dstep, ippiSize(size), mask, (ကိန်းပြည့်)mstep) >= 0)

    အတွက်( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = (const uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ကိန်းပြည့် x = 0;
        #အကယ်၍ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            အတွက်( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        အတွက်( ; x < size.width; x++ )
            အကယ်၍( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<Vec3b>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    အတွက်( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = (const uchar*)_src;
        uchar* dst = (uchar*)_dst;
        ကိန်းပြည့် x = 0;
#အကယ်၍ (CV_SIMD || CV_SIMD_SCALABLE)
        အတွက်( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        အတွက်( ; x < size.width; x++ )
            အကယ်၍( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> void
copyMask_<ushort>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (const Ipp16u *)_src, (ကိန်းပြည့်)sstep, (Ipp16u *)_dst, (ကိန်းပြည့်)dstep, ippiSize(size), mask, (ကိန်းပြည့်)mstep) >= 0)

    အတွက်( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const ushort* src = (const ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ကိန်းပြည့် x = 0;
        #အကယ်၍ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            အတွက်( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        အတွက်( ; x < size.width; x++ )
            အကယ်၍( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<Vec3s>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    အတွက်( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const ushort* src = (const ushort*)_src;
        ushort* dst = (ushort*)_dst;
        ကိန်းပြည့် x = 0;
#အကယ်၍ (CV_SIMD || CV_SIMD_SCALABLE)
        အတွက်( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        အတွက်( ; x < size.width; x++ )
            အကယ်၍( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> void
copyMask_<ကိန်းပြည့်>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    အတွက်( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const ကိန်းပြည့်* src = (const ကိန်းပြည့်*)_src;
        ကိန်းပြည့်* dst = (ကိန်းပြည့်*)_dst;
        ကိန်းပြည့် x = 0;
#အကယ်၍ (CV_SIMD || CV_SIMD_SCALABLE)
        အတွက် (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        အတွက် (; x < size.width; x++)
            အကယ်၍ ( mask[x] )
                dst[x] = src[x];
    }
}

static void
copyMaskGeneric(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, void* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    အတွက်( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = _src;
        uchar* dst = _dst;
        ကိန်းပြည့် x = 0;
        အတွက်( ; x < size.width; x++, src += esz, dst += esz )
        {
            အကယ်၍( !mask[x] )
                continue;
            အတွက်( k = 0; k < esz; k++ )
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

#အကယ်၍ defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (const ipptype *)src, (ကိန်းပြည့်)sstep, (ipptype *)dst, (ကိန်းပြည့်)dstep, ippiSize(size), (const Ipp8u *)mask, (ကိန်းပြည့်)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#မဟုတ်ပါက
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#အကယ်၍ IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#မဟုတ်ပါက
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, ကိန်းပြည့်, 32s_C1MR, Ipp32s)
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
    ပြန်ပေး esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
void Mat::copyTo( OutputArray _dst ) const
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    အကယ်၍ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*this);
        ပြန်ပေး;
    }
#endif

    ကိန်းပြည့် stype = type();
    ကိန်းပြည့် dtype = _dst.type();
    အကယ်၍( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ပြန်ပေး;
    }

    အကယ်၍( dims == 0 && empty() )
    {
        _dst.release();
        void* obj = _dst.getObj();
        အကယ်၍ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        မဟုတ်ပါက အကယ်၍ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        မဟုတ်ပါက အကယ်၍ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ပြန်ပေး;
    }

    bool allowTransposed = dims == 1 ||
        _dst.kind() == _InputArray::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    အကယ်၍( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        အကယ်၍ (empty())
            ပြန်ပေး;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != NULL);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        အတွက်( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        ကိန်းပြည့် lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        ပြန်ပေး;
    }

    အကယ်၍( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        အကယ်၍( data == dst.data )
            ပြန်ပေး;

        အကယ်၍( rows > 0 && cols > 0 )
        {
            Mat src = *this;
            Size sz = getContinuousSize2D(src, dst, (ကိန်းပြည့်)elemSize());
            CV_CheckGE(sz.width, 0, "");

            const uchar* sptr = src.data;
            uchar* dptr = dst.data;

#အကယ်၍ IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (ကိန်းပြည့်)src.step, dptr, (ကိန်းပြည့်)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            အတွက် (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        ပြန်ပေး;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    အကယ်၍( data == dst.data )
        ပြန်ပေး;

    အကယ်၍( total() != 0 )
    {
        const Mat* arrays[] = { this, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
static bool ipp_copyTo(const Mat &src, Mat &dst, const Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    အကယ်၍(mask.channels() > 1 || mask.depth() != CV_8U)
        ပြန်ပေး false;

    အကယ်၍ (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        ပြန်ပေး CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (ကိန်းပြည့်)src.step, dst.ptr(), (ကိန်းပြည့်)dst.step, size, (ကိန်းပြည့်)src.elemSize1(), src.channels(), mask.ptr(), (ကိန်းပြည့်)mask.step) >= 0;
    }
    မဟုတ်ပါက
    {
        const Mat      *arrays[] = {&src, &dst, &mask, NULL};
        uchar          *ptrs[3]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        အတွက် (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            အကယ်၍(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (ကိန်းပြည့်)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                ပြန်ပေး false;
        }
        ပြန်ပေး true;
    }
#မဟုတ်ပါက
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    ပြန်ပေး false;
#endif
}
#endif

void Mat::copyTo( OutputArray _dst, InputArray _mask ) const
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    အကယ်၍( !mask.data )
    {
        copyTo(_dst);
        ပြန်ပေး;
    }

    ကိန်းပြည့် cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    bool colorMask = mcn > 1;
    အကယ်၍( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        အကယ်၍ (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*this, dst, mask))
    အကယ်၍ ( this->dims <= 2 ) {
        အကယ်၍ ( this->size() == dst.size() && this->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, this->data, this->step, dst.data, dst.step, this->cols, this->rows, this->type(), mask.data, mask.step, mask.type());
        }
    }
    မဟုတ်ပါက အကယ်၍ ( this->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = this->total();
        အကယ်၍ (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, this->data, 0, dst.data, 0, (ကိန်းပြည့်)sz, 1, this->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    အကယ်၍( dims <= 2 )
    {
        Mat src = *this;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        ပြန်ပေး;
    }

    const Mat* arrays[] = { this, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((ကိန်းပြည့်)(it.size*mcn), 1);

    အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
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


static bool can_apply_memset(const Mat &mat, const Scalar &s, ကိန်းပြည့် &fill_value)
{
    // check if depth is 1 byte.
    switch (mat.depth())
    {
    pak_sa CV_8U: fill_value = saturate_cast<uchar>( s.val[0] ); bha_nga;
    pak_sa CV_8S: fill_value = saturate_cast<schar>( s.val[0] ); bha_nga;
    default: ပြန်ပေး false;
    }

    // check if all element is same.
    const int64* is = (const int64*)&s.val[0];
    switch (mat.channels())
    {
    pak_sa 1: ပြန်ပေး true;
    pak_sa 2: ပြန်ပေး (is[0] == is[1]);
    pak_sa 3: ပြန်ပေး (is[0] == is[1] && is[1] == is[2]);
    pak_sa 4: ပြန်ပေး (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    default: ပြန်ပေး false;
    }
}

Mat& Mat::operator = (const Scalar& s)
{
    CV_INSTRUMENT_REGION();

    အကယ်၍ (this->empty())
        ပြန်ပေး *this;

    const Mat* arrays[] = { this };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    const int64* is = (const int64*)&s.val[0];

    အကယ်၍( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    မဟုတ်ပါက
    {
        ကိန်းပြည့် fill_value = 0;
        အကယ်၍ ( can_apply_memset(*this, s, fill_value) )
        {
            အတွက် (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_value, elsize);
            ပြန်ပေး *this;
        }

        အကယ်၍( it.nplanes > 0 )
        {
            dviguu_na scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            အတွက်( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= parimaa_na(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        အတွက်( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    ပြန်ပေး *this;
}

#ifdef HAVE_IPP
static bool ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    အကယ်၍(mask.empty())
        ပြန်ပေး false;

    အကယ်၍(mask.depth() != CV_8U || mask.channels() > 1)
        ပြန်ပေး false;

    အကယ်၍(dst.channels() > 4)
        ပြန်ပေး false;

    အကယ်၍ (dst.depth() == CV_32F)
    {
        အတွက် (ကိန်းပြည့် i = 0; i < (ကိန်းပြည့်)(_val.total()); i++)
        {
            float v = (float)(_val.at<dviguu_na>(i));  // cast to float
            အကယ်၍ (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                ပြန်ပေး false;
        }
    }

    အကယ်၍(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::IwValueFloat s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ပြန်ပေး CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (ကိန်းပြည့်)dst.step, size, dataType, dst.channels(), mask.ptr(), (ကိန်းပြည့်)mask.step) >= 0;
    }
    မဟုတ်ပါက
    {
        const Mat      *arrays[] = {&dst, mask.empty()?NULL:&mask, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(ကိန်းပြည့်)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::IwValueFloat s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        အတွက်( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            အကယ်၍(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                ပြန်ပေး false;
        }
        ပြန်ပေး true;
    }
#မဟုတ်ပါက
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    ပြန်ပေး false;
#endif
}
#endif

Mat& Mat::setTo(InputArray _value, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    အကယ်၍( empty() )
        ပြန်ပေး *this;

    Mat value = _value.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), _value.kind(), _InputArray::MAT ));
    ကိန်းပြည့် cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*this, value, mask), *this)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    const Mat* arrays[] = { this, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    ကိန်းပြည့် totalsz = (ကိန်းပြည့်)it.size*mcn;
    ကိန်းပြည့် blockSize0 = std::min(totalsz, (ကိန်းပြည့်)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (ကိန်းပြည့်)parimaa_na(dviguu_na));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        အတွက်( ကိန်းပြည့် j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            အကယ်၍( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            မဟုတ်ပါက
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    ပြန်ပေး *this;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    အကယ်၍( empty() )
        ပြန်ပေး *this;

    size_t esz = elemSize();

    const Mat* arrays[] = { this, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    ပြန်ပေး *this;
}


#အကယ်၍ defined HAVE_OPENCL && !defined __APPLE__

static bool ocl_repeat(InputArray _src, ကိန်းပြည့် ny, ကိန်းပြည့် nx, OutputArray _dst)
{
    အကယ်၍ (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        ပြန်ပေး true;
    }

    ကိန်းပြည့် type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    ပြန်ပေး k.run(2, globalsize, NULL, false);
}

#endif

void repeat(InputArray _src, ကိန်းပြည့် ny, ကိန်းပြည့် nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    အကယ်၍( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        ပြန်ပေး;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#အကယ်၍ !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    ကိန်းပြည့် esz = (ကိန်းပြည့်)src.elemSize();
    ကိန်းပြည့် x, y;
    ssize.width *= esz; dsize.width *= esz;

    အတွက်( y = 0; y < ssize.height; y++ )
    {
        အတွက်( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    အတွက်( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(const Mat& src, ကိန်းပြည့် ny, ကိန်းပြည့် nx)
{
    အကယ်၍( nx == 1 && ny == 1 )
        ပြန်ပေး src;
    Mat dst;
    repeat(src, ny, nx, dst);
    ပြန်ပေး dst;
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
ကိန်းပြည့် cv::borderInterpolate( ကိန်းပြည့် p, ကိန်းပြည့် len, ကိန်းပြည့် borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    အကယ်၍(p >= 0 && p < len)
#မဟုတ်ပါက
    အကယ်၍( (unsigned)p < (unsigned)len )
#endif
        ;
    မဟုတ်ပါက အကယ်၍( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    မဟုတ်ပါက အကယ်၍( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        ကိန်းပြည့် delta = borderType == BORDER_REFLECT_101;
        အကယ်၍( len == 1 )
            ပြန်ပေး 0;
        do
        {
            အကယ်၍( p < 0 )
                p = -p - 1 + delta;
            မဟုတ်ပါက
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        နေစဉ်(p < 0 || p >= len);
#မဟုတ်ပါက
        နေစဉ်( (unsigned)p >= (unsigned)len );
#endif
    }
    မဟုတ်ပါက အကယ်၍( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        အကယ်၍( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            အကယ်၍( p64 < 0 )
                p64 += len;
            p = (ကိန်းပြည့်)p64;
        }
    }
    မဟုတ်ပါက အကယ်၍( borderType == BORDER_CONSTANT )
        p = -1;
    မဟုတ်ပါက
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    ပြန်ပေး p;
}

namespace
{

void copyMakeBorder_8u( const uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        ကိန်းပြည့် top, ကိန်းပြည့် left, ကိန်းပြည့် cn, ကိန်းပြည့် borderType )
{
    const ကိန်းပြည့် isz = (ကိန်းပြည့်)parimaa_na(ကိန်းပြည့်);
    ကိန်းပြည့် i, j, k, elemSize = 1;
    bool intMode = false;

    အကယ်၍( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = true;
    }

    cv::AutoBuffer<ကိန်းပြည့်> _tab((dstroi.width - srcroi.width)*cn);
    ကိန်းပြည့်* tab = _tab.data();
    ကိန်းပြည့် right = dstroi.width - srcroi.width - left;
    ကိန်းပြည့် bottom = dstroi.height - srcroi.height - top;

    အတွက်( i = 0; i < left; i++ )
    {
        j = cv::borderInterpolate(i - left, srcroi.width, borderType)*cn;
        အတွက်( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    အတွက်( i = 0; i < right; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        အတွက်( k = 0; k < cn; k++ )
            tab[(i+left)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + dststep*top + left*elemSize;

    အတွက်( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        အကယ်၍( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        အကယ်၍( intMode )
        {
            const ကိန်းပြည့်* isrc = (ကိန်းပြည့်*)src;
            ကိန်းပြည့်* idstInner = (ကိန်းပြည့်*)dstInner;
            အတွက်( j = 0; j < left; j++ )
                idstInner[j - left] = isrc[tab[j]];
            အတွက်( j = 0; j < right; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + left]];
        }
        မဟုတ်ပါက
        {
            အတွက်( j = 0; j < left; j++ )
                dstInner[j - left] = src[tab[j]];
            အတွက်( j = 0; j < right; j++ )
                dstInner[j + srcroi.width] = src[tab[j + left]];
        }
    }

    dstroi.width *= elemSize;

    အတွက်( i = 0; i < top; i++ )
    {
        j = cv::borderInterpolate(i - top, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (top+j)*dststep, dstroi.width);
    }

    dst += dststep*top;
    အတွက်( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


void copyMakeConstBorder_8u( const uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             ကိန်းပြည့် top, ကိန်းပြည့် left, ကိန်းပြည့် cn, const uchar* value )
{
    ကိန်းပြည့် i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    ကိန်းပြည့် right = dstroi.width - srcroi.width - left;
    ကိန်းပြည့် bottom = dstroi.height - srcroi.height - top;

    အတွက်( i = 0; i < dstroi.width; i++ )
    {
        အတွက်( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + dststep*top + left;

    အတွက်( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        အကယ်၍( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - left, constBuf, left );
        memcpy( dstInner + srcroi.width, constBuf, right );
    }

    အတွက်( i = 0; i < top; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (top + srcroi.height) * dststep;
    အတွက်( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

namespace cv {

static bool ocl_copyMakeBorder( InputArray _src, OutputArray _dst, ကိန်းပြည့် top, ကိန်းပြည့် bottom,
                                ကိန်းပြည့် left, ကိန်းပြည့် right, ကိန်းပြည့် borderType, const Scalar& value )
{
    ကိန်းပြည့် type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    bool isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    အကယ်၍ ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        ပြန်ပေး false;

    const var_na * const borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    ကိန်းပြည့် scalarcn = cn == 3 ? 4 : cn;
    ကိန်းပြည့် sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    UMat src = _src.getUMat();
    အကယ်၍( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        ကိန်းပြည့် dtop = std::min(ofs.y, top);
        ကိန်းပြည့် dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        ကိန်းပြည့် dleft = std::min(ofs.x, left);
        ကိန်းပြည့် dright = std::min(wholeSize.width - src.cols - ofs.x, right);
        src.adjustROI(dtop, dbottom, dleft, dright);
        top -= dtop;
        left -= dleft;
        bottom -= dbottom;
        right -= dright;
    }

    _dst.create(src.rows + top + bottom, src.cols + left + right, type);
    UMat dst = _dst.getUMat();

    အကယ်၍ (top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        အကယ်၍(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        ပြန်ပေး true;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           top, left, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ပြန်ပေး k.run(2, globalsize, NULL, false);
}

}
#endif

#ifdef HAVE_IPP
namespace cv {

static bool ipp_copyMakeBorder( Mat &_src, Mat &_dst, ကိန်းပြည့် top, ကိန်းပြည့် bottom,
                                ကိန်းပြည့် left, ကိန်းပြည့် right, ကိန်းပြည့် _borderType, const Scalar& value )
{
#အကယ်၍ defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(left, top, right, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    အကယ်၍((ကိန်းပြည့်)borderType == -1)
        ပြန်ပေး false;

    အကယ်၍(_src.dims > 2)
        ပြန်ပေး false;

    Rect dstRect(borderSize.left, borderSize.top,
        _dst.cols - borderSize.right - borderSize.left,
        _dst.rows - borderSize.bottom - borderSize.top);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    ပြန်ပေး CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#မဟုတ်ပါက
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(top); CV_UNUSED(bottom); CV_UNUSED(left); CV_UNUSED(right);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    ပြန်ပေး false;
#endif
}
}
#endif

void cv::copyMakeBorder( InputArray _src, OutputArray _dst, ကိန်းပြည့် top, ကိန်းပြည့် bottom,
                         ကိန်းပြည့် left, ကိန်းပြည့် right, ကိန်းပြည့် borderType, const Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( top >= 0 && bottom >= 0 && left >= 0 && right >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + top + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + left + right <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, top, bottom, left, right, borderType, value))
    ကိန်းပြည့် type = src.type();

    အကယ်၍( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        ကိန်းပြည့် dtop = std::min(ofs.y, top);
        ကိန်းပြည့် dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        ကိန်းပြည့် dleft = std::min(ofs.x, left);
        ကိန်းပြည့် dright = std::min(wholeSize.width - src.cols - ofs.x, right);
        src.adjustROI(dtop, dbottom, dleft, dright);
        top -= dtop;
        left -= dleft;
        bottom -= dbottom;
        right -= dright;
    }

    _dst.create( src.rows + top + bottom, src.cols + left + right, type );
    Mat dst = _dst.getMat();

    အကယ်၍(top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        အကယ်၍(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        ပြန်ပေး;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, top, bottom, left, right, borderType, value))

    အကယ်၍( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           top, left, (ကိန်းပြည့်)src.elemSize(), borderType );
    မဟုတ်ပါက
    {
        ကိန်းပြည့် cn = src.channels(), cn1 = cn;
        AutoBuffer<dviguu_na> buf(cn);
        အကယ်၍( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                top, left, (ကိန်းပြည့်)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
