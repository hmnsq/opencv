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

سانچو <قسم_نالو T> ساكن مضمن
فراغ scalar_منزل_Raw_معلومات_2(ثابت Scalar& s, T * ثابت buf, ثابت صحيح cn, ثابت صحيح unroll_منزل)
{
    صحيح i = 0;
    لكل(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    لكل(; i < unroll_منزل; i++)
        buf[i] = buf[i-cn];
}

فراغ scalar_منزل_Raw_معلومات(ثابت Scalar& s, فراغ* _buf, صحيح type, صحيح unroll_منزل)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    بدل(اونهائي)
    {
    حالة CV_8U:
        scalar_منزل_Raw_معلومات_2(s, (uchar*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_8S:
        scalar_منزل_Raw_معلومات_2(s, (schar*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_Bool:
        scalar_منزل_Raw_معلومات_2(s, (منطقي*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_16U:
        scalar_منزل_Raw_معلومات_2(s, (ushort*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_16S:
        scalar_منزل_Raw_معلومات_2(s, (ننڍو*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_16F:
        scalar_منزل_Raw_معلومات_2(s, (hfloat*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_16BF:
        scalar_منزل_Raw_معلومات_2(s, (bfloat*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_8F_E4M3FN:
        scalar_منزل_Raw_معلومات_2(s, (fp8_t*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_8F_E4M3FNUZ:
        scalar_منزل_Raw_معلومات_2(s, (fp8a_t*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_32U:
        scalar_منزل_Raw_معلومات_2(s, (بنانشان*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_32S:
        scalar_منزل_Raw_معلومات_2(s, (صحيح*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_32F:
        scalar_منزل_Raw_معلومات_2(s, (عائم*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_64U:
        scalar_منزل_Raw_معلومات_2(s, (uint64_t*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_64S:
        scalar_منزل_Raw_معلومات_2(s, (int64_t*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_64F:
        scalar_منزل_Raw_معلومات_2(s, (مزدوج*)_buf, cn, unroll_منزل);
        اكسر;
    افتراضي:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

فراغ convertAndUnrollScalar( ثابت Mat& sc, صحيح buftype, uchar* scbuf, size_t blocksize )
{
    صحيح scn = (صحيح)sc.ڪل(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_سائيز(buftype);
    ٻٽو_Func cvtFn = getConvertFunc(sc.اونهائي(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, سائيز(std::min(cn, scn), 1), 0);
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

سانچو<قسم_نالو T> ساكن فراغ
copy_نقاب(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائيز size)
{
    لكل( ; size.اوچائي--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت T* src = (ثابت T*)_src;
        T* dst = (T*)_dst;
        صحيح x = 0;
         #إذا CV_ENABLE_UNROLLED
        لكل( ; x <= size.ويڪر - 4; x += 4 )
        {
            إذا( نقاب[x] )
                dst[x] = src[x];
            إذا( نقاب[x+1] )
                dst[x+1] = src[x+1];
            إذا( نقاب[x+2] )
                dst[x+2] = src[x+2];
            إذا( نقاب[x+3] )
                dst[x+3] = src[x+3];
        }
        #جيڪڏهن_پورو
        لكل( ; x < size.ويڪر; x++ )
            إذا( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچو<> فراغ
copy_نقاب<uchar>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائيز size)
{
    CV_IPP_لاڳيتو_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (صحيح)sstep, _dst, (صحيح)dstep, ippi_سائيز(size), نقاب, (صحيح)mstep) >= 0)

    لكل( ; size.اوچائي--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحيح x = 0;
        #إذا (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            لكل( ; x <= size.ويڪر - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_ڀريو(src  + x),
                        v_dst   = vx_ڀريو(dst  + x),
                        v_nmask = v_eq(vx_ڀريو(نقاب + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #جيڪڏهن_پورو
        لكل( ; x < size.ويڪر; x++ )
            إذا( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچو<> فراغ
copy_نقاب<Vec3b>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائيز size)
{
    لكل( ; size.اوچائي--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; x <= size.ويڪر - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_ڀريو(نقاب + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_ڀريو_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_ڀريو_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#جيڪڏهن_پورو
        لكل( ; x < size.ويڪر; x++ )
            إذا( نقاب[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

سانچو<> فراغ
copy_نقاب<ushort>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائيز size)
{
    CV_IPP_لاڳيتو_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ثابت Ipp16u *)_src, (صحيح)sstep, (Ipp16u *)_dst, (صحيح)dstep, ippi_سائيز(size), نقاب, (صحيح)mstep) >= 0)

    لكل( ; size.اوچائي--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحيح x = 0;
        #إذا (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            لكل( ; x <= size.ويڪر - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_ڀريو(src + x), v_src2 = vx_ڀريو(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_ڀريو(dst + x), v_dst2 = vx_ڀريو(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_ڀريو(نقاب + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #جيڪڏهن_پورو
        لكل( ; x < size.ويڪر; x++ )
            إذا( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچو<> فراغ
copy_نقاب<Vec3s>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائيز size)
{
    لكل( ; size.اوچائي--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; x <= size.ويڪر - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_ڀريو(نقاب + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_ڀريو_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_ڀريو_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_ڀريو_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_ڀريو_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

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
#جيڪڏهن_پورو
        لكل( ; x < size.ويڪر; x++ )
            إذا( نقاب[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

سانچو<> فراغ
copy_نقاب<صحيح>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائيز size)
{
    لكل( ; size.اوچائي--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت صحيح* src = (ثابت صحيح*)_src;
        صحيح* dst = (صحيح*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل (; x <= size.ويڪر - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_ڀريو(src + x), v_dst0 = vx_ڀريو(dst + x);
            v_int32 v_src1 = vx_ڀريو(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_ڀريو(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_ڀريو(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_ڀريو(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_ڀريو(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_ڀريو(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_ڀريو(نقاب + x), vx_setzero_u8());
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
#جيڪڏهن_پورو
        لكل (; x < size.ويڪر; x++)
            إذا ( نقاب[x] )
                dst[x] = src[x];
    }
}

ساكن فراغ
copy_نقاب_Generic(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائيز size, فراغ* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    لكل( ; size.اوچائي--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = _src;
        uchar* dst = _dst;
        صحيح x = 0;
        لكل( ; x < size.ويڪر; x++, src += esz, dst += esz )
        {
            إذا( !نقاب[x] )
                استمر;
            لكل( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#وصف_ڪريو DEF_COPY_نقاب(suffix, type) \
ساكن فراغ copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, سائيز size, فراغ*) \
{ \
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}

#إذا defined HAVE_IPP
#وصف_ڪريو DEF_COPY_نقاب_F(suffix, type, ippfavor, ipptype) \
ساكن فراغ copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, سائيز size, فراغ*) \
{ \
    CV_IPP_لاڳيتو_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ثابت ipptype *)src, (صحيح)sstep, (ipptype *)dst, (صحيح)dstep, ippi_سائيز(size), (ثابت Ipp8u *)نقاب, (صحيح)mstep) >= 0)\
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}
#وإلا
#وصف_ڪريو DEF_COPY_نقاب_F(suffix, type, ippfavor, ipptype) \
ساكن فراغ copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, سائيز size, فراغ*) \
{ \
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}
#جيڪڏهن_پورو

#إذا IPP_نسخو_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_نقاب(32sC3, Vec3i)
DEF_COPY_نقاب(8uC3, Vec3b)
#وإلا
DEF_COPY_نقاب_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_نقاب_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#جيڪڏهن_پورو
DEF_COPY_نقاب(8u, uchar)
DEF_COPY_نقاب(16u, ushort)
DEF_COPY_نقاب_F(32s, صحيح, 32s_C1MR, Ipp32s)
DEF_COPY_نقاب_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_نقاب(32sC2, Vec2i)
DEF_COPY_نقاب_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_نقاب(32sC6, Vec6i)
DEF_COPY_نقاب(32sC8, Vec8i)

ٻٽو_Func copy_نقاب_Tab[] =
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

ٻٽو_Func get_Copy_نقاب_Func(size_t esz)
{
    أرجع esz <= 32 && copy_نقاب_Tab[esz] ? copy_نقاب_Tab[esz] : copy_نقاب_Generic;
}

/* dst = src */
فراغ Mat::copy_منزل( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    إذا (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*هذا);
        أرجع;
    }
#جيڪڏهن_پورو

    صحيح stype = type();
    صحيح dtype = _dst.type();
    إذا( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_منزل( _dst, dtype );
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
        _dst.قسم() == داخلا_Array_2::STD_ويڪٽر ||
        (_dst.fixed_سائيز() && _dst.dims() == 1);
    إذا( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        إذا (empty())
            أرجع;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != عدم);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_سائيز(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        لكل( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        صحيح lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, معلومات, std::max(dims, 1), sz, dstofs, dst.قدم.p, قدم.p);
        أرجع;
    }

    إذا( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        إذا( معلومات == dst.معلومات )
            أرجع;

        إذا( صفون > 0 && کانن_ڳڻپ > 0 )
        {
            Mat src = *هذا;
            سائيز sz = getContinuousSize2D(src, dst, (صحيح)elem_سائيز());
            CV_CheckGE(sz.ويڪر, 0, "");

            ثابت uchar* sptr = src.معلومات;
            uchar* dptr = dst.معلومات;

#إذا IPP_نسخو_X100 >= 201700
            CV_IPP_لاڳيتو_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (صحيح)src.قدم, dptr, (صحيح)dst.قدم, ippi_سائيز_L(sz.ويڪر, sz.اوچائي)) >= 0)
#جيڪڏهن_پورو

            لكل (; sz.اوچائي--; sptr += src.قدم, dptr += dst.قدم)
                يادگيري_نقل(dptr, sptr, sz.ويڪر);
        }
        أرجع;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    إذا( معلومات == dst.معلومات )
        أرجع;

    إذا( ڪل() != 0 )
    {
        ثابت Mat* arrays[] = { هذا, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_سائيز();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            يادگيري_نقل(ptrs[1], ptrs[0], sz);
    }
}

#جيڪڏهن_وصف_ٿيل HAVE_IPP
ساكن منطقي ipp_copy_منزل(ثابت Mat &src, Mat &dst, ثابت Mat &نقاب)
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(نقاب.channels() > 1 || نقاب.اونهائي() != CV_8U)
        أرجع خطأ;

    إذا (src.dims <= 2)
    {
        Ippi_سائيز size = ippi_سائيز(src.size());
        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_نقاب, src.ptr(), (صحيح)src.قدم, dst.ptr(), (صحيح)dst.قدم, size, (صحيح)src.elemSize1(), src.channels(), نقاب.ptr(), (صحيح)نقاب.قدم) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&src, &dst, &نقاب, عدم};
        uchar          *ptrs[3]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_سائيز size = ippi_سائيز(it.size, 1);

        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_نقاب, ptrs[0], 0, ptrs[1], 0, size, (صحيح)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(src); CV_اڻ_استعمال_ٿيل(dst); CV_اڻ_استعمال_ٿيل(نقاب);
    أرجع خطأ;
#جيڪڏهن_پورو
}
#جيڪڏهن_پورو

فراغ Mat::copy_منزل( OutputArray _dst, داخلا_Array نقاب_2 ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat نقاب = نقاب_2.getMat();
    إذا( !نقاب.معلومات )
    {
        copy_منزل(_dst);
        أرجع;
    }

    صحيح cn = channels(), mcn = نقاب.channels();
    CV_Assert( (نقاب.اونهائي() == CV_8U || نقاب.اونهائي() == CV_8S || نقاب.اونهائي() == CV_Bool) && (mcn == 1 || mcn == cn) );
    منطقي color_نقاب = mcn > 1;
    إذا( dims <= 2 )
    {
        CV_Assert( size() == نقاب.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        إذا (dst.معلومات != dst0.معلومات) // re-allocation happened
        {
#جيڪڏهن_وصف_ٿيل OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#جيڪڏهن_پورو
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_لاڳيتو_FAST(ipp_copy_منزل(*هذا, dst, نقاب))
    إذا ( هذا->dims <= 2 ) {
        إذا ( هذا->size() == dst.size() && هذا->size() == dst.size() ) {
            CALL_HAL(copy_منزل_نقاب, cv_hal_copy_منزل_Masked, هذا->معلومات, هذا->قدم, dst.معلومات, dst.قدم, هذا->کانن_ڳڻپ, هذا->صفون, هذا->type(), نقاب.معلومات, نقاب.قدم, نقاب.type());
        }
    }
    وإلا إذا ( هذا->isContinuous() && dst.isContinuous() && نقاب.isContinuous() )
    {
        size_t sz = هذا->ڪل();
        إذا (sz < وڌ_کان_وڌ_عدد) {
            CALL_HAL(copy_منزل_نقاب, cv_hal_copy_منزل_Masked, هذا->معلومات, 0, dst.معلومات, 0, (صحيح)sz, 1, هذا->type(), نقاب.معلومات, 0, نقاب.type());
        }
    }

    size_t esz = color_نقاب ? elemSize1() : elem_سائيز();
    ٻٽو_Func copymask = get_Copy_نقاب_Func(esz);

    إذا( dims <= 2 )
    {
        Mat src = *هذا;
        سائيز sz = getContinuousSize2D(src, dst, نقاب, mcn);
        copymask(src.معلومات, src.قدم, نقاب.معلومات, نقاب.قدم, dst.معلومات, dst.قدم, sz, &esz);
        أرجع;
    }

    ثابت Mat* arrays[] = { هذا, &dst, &نقاب, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    سائيز sz((صحيح)(it.size*mcn), 1);

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
    copy_منزل(_dst);
}
فراغ Mat::copyAt( OutputArray _dst, داخلا_Array نقاب_2 ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_منزل(_dst, نقاب_2);
}


ساكن منطقي can_apply_memset(ثابت Mat &mat, ثابت Scalar &s, صحيح &fill_قدر)
{
    // check if depth is 1 byte.
    بدل (mat.اونهائي())
    {
    حالة CV_8U: fill_قدر = saturate_cast<uchar>( s.val[0] ); اكسر;
    حالة CV_8S: fill_قدر = saturate_cast<schar>( s.val[0] ); اكسر;
    افتراضي: أرجع خطأ;
    }

    // check if all element is same.
    ثابت int64* is = (ثابت int64*)&s.val[0];
    بدل (mat.channels())
    {
    حالة 1: أرجع سچ;
    حالة 2: أرجع (is[0] == is[1]);
    حالة 3: أرجع (is[0] == is[1] && is[1] == is[2]);
    حالة 4: أرجع (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    افتراضي: أرجع خطأ;
    }
}

Mat& Mat::عامل = (ثابت Scalar& s)
{
    CV_INSTRUMENT_REGION();

    إذا (هذا->empty())
        أرجع *هذا;

    ثابت Mat* arrays[] = { هذا };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elem_سائيز();
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
            scalar_منزل_Raw_معلومات(s, scalar, type(), 12);
            size_t بلاڪ_سائيز_2 = 12*elemSize1();

            لكل( size_t j = 0; j < elsize; j += بلاڪ_سائيز_2 )
            {
                size_t sz = MIN(بلاڪ_سائيز_2, elsize - j);
                CV_Assert(sz <= حجم(scalar));
                يادگيري_نقل( dptr + j, scalar, sz );
            }
        }

        لكل( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            يادگيري_نقل( dptr, معلومات, elsize );
        }
    }
    أرجع *هذا;
}

#جيڪڏهن_وصف_ٿيل HAVE_IPP
ساكن منطقي ipp_Mat_سيٽ_منزل_Mat(Mat &dst, Mat &_val, Mat &نقاب)
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(نقاب.empty())
        أرجع خطأ;

    إذا(نقاب.اونهائي() != CV_8U || نقاب.channels() > 1)
        أرجع خطأ;

    إذا(dst.channels() > 4)
        أرجع خطأ;

    إذا (dst.اونهائي() == CV_32F)
    {
        لكل (صحيح i = 0; i < (صحيح)(_val.ڪل()); i++)
        {
            عائم v = (عائم)(_val.at<مزدوج>(i));  // cast to float
            إذا (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                أرجع خطأ;
        }
    }

    إذا(dst.dims <= 2)
    {
        Ippi_سائيز            size     = ippi_سائيز(dst.size());
        Ipp_معلومات_Type         معلومات_Type = ippi_Get_معلومات_Type(dst.اونهائي());
        ::ipp::Iw_قدر_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_سيٽ_نقاب, s, dst.ptr(), (صحيح)dst.قدم, size, معلومات_Type, dst.channels(), نقاب.ptr(), (صحيح)نقاب.قدم) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, نقاب.empty()?عدم:&نقاب, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_سائيز            size     = {(صحيح)it.size, 1};
        Ipp_معلومات_Type         معلومات_Type = ippi_Get_معلومات_Type(dst.اونهائي());
        ::ipp::Iw_قدر_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        لكل( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_سيٽ_نقاب, s, ptrs[0], 0, size, معلومات_Type, dst.channels(), ptrs[1], 0) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(dst); CV_اڻ_استعمال_ٿيل(_val); CV_اڻ_استعمال_ٿيل(نقاب);
    أرجع خطأ;
#جيڪڏهن_پورو
}
#جيڪڏهن_پورو

Mat& Mat::سيٽ_منزل(داخلا_Array قدر, داخلا_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    إذا( empty() )
        أرجع *هذا;

    Mat value = قدر.getMat(), نقاب = نقاب_2.getMat();

    CV_Assert( checkScalar(value, type(), قدر.قسم(), داخلا_Array_2::MAT ));
    صحيح cn = channels(), mcn = نقاب.channels();
    CV_Assert( نقاب.empty() || ((نقاب.اونهائي() == CV_8U || نقاب.اونهائي() == CV_8S || نقاب.اونهائي() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == نقاب.size) );

    CV_IPP_لاڳيتو_FAST(ipp_Mat_سيٽ_منزل_Mat(*هذا, value, نقاب), *هذا)

    size_t esz = mcn > 1 ? elemSize1() : elem_سائيز();
    ٻٽو_Func copymask = get_Copy_نقاب_Func(esz);

    ثابت Mat* arrays[] = { هذا, !نقاب.empty() ? &نقاب : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    صحيح totalsz = (صحيح)it.size*mcn;
    صحيح بلاڪ_Size0 = std::min(totalsz, (صحيح)((بلاڪ_سائيز + esz-1)/esz));
    بلاڪ_Size0 -= بلاڪ_Size0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_بفر<uchar> _scbuf(بلاڪ_Size0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.معلومات(), (صحيح)حجم(مزدوج));
    convertAndUnrollScalar( value, type(), scbuf, بلاڪ_Size0/mcn );

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( صحيح j = 0; j < totalsz; j += بلاڪ_Size0 )
        {
            سائيز sz(std::min(بلاڪ_Size0, totalsz - j), 1);
            size_t بلاڪ_سائيز_2 = sz.ويڪر*esz;
            إذا( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.ويڪر;
            }
            وإلا
                يادگيري_نقل(ptrs[0], scbuf, بلاڪ_سائيز_2);
            ptrs[0] += بلاڪ_سائيز_2;
        }
    }
    أرجع *هذا;
}


Mat& Mat::سيٽ_Zero()
{
    CV_INSTRUMENT_REGION();

    إذا( empty() )
        أرجع *هذا;

    size_t esz = elem_سائيز();

    ثابت Mat* arrays[] = { هذا, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    أرجع *هذا;
}


#إذا defined HAVE_OPENCL && !defined __APPLE__

ساكن منطقي ocl_repeat(داخلا_Array _src, صحيح ny, صحيح nx, OutputArray _dst)
{
    إذا (ny == 1 && nx == 1)
    {
        _src.copy_منزل(_dst);
        أرجع سچ;
    }

    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type),
            صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predict_Optimal_ويڪٽر_ويڪر(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memop_Type_منزل_Str(CV_MAKE_TYPE(اونهائي, kercn)),
                         nx, ny, صفون_في_WI, kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.دليلون(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_سائيز(dst));

    size_t globalsize[] = { (size_t)src.کانن_ڳڻپ * cn / kercn, ((size_t)src.صفون + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

#جيڪڏهن_پورو

فراغ repeat(داخلا_Array _src, صحيح ny, صحيح nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    سائيز ssize = _src.size();
    إذا( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        أرجع;
    }
    CV_Assert( (int64_t)ssize.اوچائي * ny <= وڌ_کان_وڌ_عدد && (int64_t)ssize.ويڪر * nx <= وڌ_کان_وڌ_عدد );
    _dst.create(ssize.اوچائي*ny, ssize.ويڪر*nx, _src.type());

#إذا !defined __APPLE__
    CV_OCL_لاڳيتو(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#جيڪڏهن_پورو

    Mat src = _src.getMat(), dst = _dst.getMat();
    سائيز dsize = dst.size();
    صحيح esz = (صحيح)src.elem_سائيز();
    صحيح x, y;
    ssize.ويڪر *= esz; dsize.ويڪر *= esz;

    لكل( y = 0; y < ssize.اوچائي; y++ )
    {
        لكل( x = 0; x < dsize.ويڪر; x += ssize.ويڪر )
            يادگيري_نقل( dst.ptr(y) + x, src.ptr(y), ssize.ويڪر );
    }

    لكل( ; y < dsize.اوچائي; y++ )
        يادگيري_نقل( dst.ptr(y), dst.ptr(y - ssize.اوچائي), dsize.ويڪر );
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

#جيڪڏهن_وصف_ٿيل CV_STATIC_ANALYSIS
    إذا(p >= 0 && p < len)
#وإلا
    إذا( (بنانشان)p < (بنانشان)len )
#جيڪڏهن_پورو
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
#جيڪڏهن_وصف_ٿيل CV_STATIC_ANALYSIS
        طالما(p < 0 || p >= len);
#وإلا
        طالما( (بنانشان)p >= (بنانشان)len );
#جيڪڏهن_پورو
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

فراغ copyMakeBorder_8u( ثابت uchar* src, size_t srcstep, cv::سائيز srcroi,
                        uchar* dst, size_t dststep, cv::سائيز dstroi,
                        صحيح چوٽي_2, صحيح کاٻي, صحيح cn, صحيح borderType )
{
    ثابت صحيح isz = (صحيح)حجم(صحيح);
    صحيح i, j, k, elem_سائيز = 1;
    منطقي int_گهڻائي = خطأ;

    إذا( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_سائيز = isz;
        int_گهڻائي = سچ;
    }

    cv::Auto_بفر<صحيح> _tab((dstroi.ويڪر - srcroi.ويڪر)*cn);
    صحيح* tab = _tab.معلومات();
    صحيح ساڄي = dstroi.ويڪر - srcroi.ويڪر - کاٻي;
    صحيح bottom = dstroi.اوچائي - srcroi.اوچائي - چوٽي_2;

    لكل( i = 0; i < کاٻي; i++ )
    {
        j = cv::borderInterpolate(i - کاٻي, srcroi.ويڪر, borderType)*cn;
        لكل( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    لكل( i = 0; i < ساڄي; i++ )
    {
        j = cv::borderInterpolate(srcroi.ويڪر + i, srcroi.ويڪر, borderType)*cn;
        لكل( k = 0; k < cn; k++ )
            tab[(i+کاٻي)*cn + k] = j + k;
    }

    srcroi.ويڪر *= cn;
    dstroi.ويڪر *= cn;
    کاٻي *= cn;
    ساڄي *= cn;

    uchar* dstInner = dst + dststep*چوٽي_2 + کاٻي*elem_سائيز;

    لكل( i = 0; i < srcroi.اوچائي; i++, dstInner += dststep, src += srcstep )
    {
        إذا( dstInner != src )
            يادگيري_نقل(dstInner, src, srcroi.ويڪر*elem_سائيز);

        إذا( int_گهڻائي )
        {
            ثابت صحيح* isrc = (صحيح*)src;
            صحيح* idstInner = (صحيح*)dstInner;
            لكل( j = 0; j < کاٻي; j++ )
                idstInner[j - کاٻي] = isrc[tab[j]];
            لكل( j = 0; j < ساڄي; j++ )
                idstInner[j + srcroi.ويڪر] = isrc[tab[j + کاٻي]];
        }
        وإلا
        {
            لكل( j = 0; j < کاٻي; j++ )
                dstInner[j - کاٻي] = src[tab[j]];
            لكل( j = 0; j < ساڄي; j++ )
                dstInner[j + srcroi.ويڪر] = src[tab[j + کاٻي]];
        }
    }

    dstroi.ويڪر *= elem_سائيز;

    لكل( i = 0; i < چوٽي_2; i++ )
    {
        j = cv::borderInterpolate(i - چوٽي_2, srcroi.اوچائي, borderType);
        يادگيري_نقل(dst + i*dststep, dst + (چوٽي_2+j)*dststep, dstroi.ويڪر);
    }

    dst += dststep*چوٽي_2;
    لكل( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.اوچائي, srcroi.اوچائي, borderType);
        يادگيري_نقل(dst + (i + srcroi.اوچائي)*dststep, dst + j*dststep, dstroi.ويڪر);
    }
}


فراغ copyMakeConstBorder_8u( ثابت uchar* src, size_t srcstep, cv::سائيز srcroi,
                             uchar* dst, size_t dststep, cv::سائيز dstroi,
                             صحيح چوٽي_2, صحيح کاٻي, صحيح cn, ثابت uchar* value )
{
    صحيح i, j;
    cv::Auto_بفر<uchar> _constBuf(dstroi.ويڪر*cn);
    uchar* constBuf = _constBuf.معلومات();
    صحيح ساڄي = dstroi.ويڪر - srcroi.ويڪر - کاٻي;
    صحيح bottom = dstroi.اوچائي - srcroi.اوچائي - چوٽي_2;

    لكل( i = 0; i < dstroi.ويڪر; i++ )
    {
        لكل( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.ويڪر *= cn;
    dstroi.ويڪر *= cn;
    کاٻي *= cn;
    ساڄي *= cn;

    uchar* dstInner = dst + dststep*چوٽي_2 + کاٻي;

    لكل( i = 0; i < srcroi.اوچائي; i++, dstInner += dststep, src += srcstep )
    {
        إذا( dstInner != src )
            يادگيري_نقل( dstInner, src, srcroi.ويڪر );
        يادگيري_نقل( dstInner - کاٻي, constBuf, کاٻي );
        يادگيري_نقل( dstInner + srcroi.ويڪر, constBuf, ساڄي );
    }

    لكل( i = 0; i < چوٽي_2; i++ )
        يادگيري_نقل(dst + i * dststep, constBuf, dstroi.ويڪر);

    dst += (چوٽي_2 + srcroi.اوچائي) * dststep;
    لكل( i = 0; i < bottom; i++ )
        يادگيري_نقل(dst + i * dststep, constBuf, dstroi.ويڪر);
}

}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_copyMakeBorder( داخلا_Array _src, OutputArray _dst, صحيح چوٽي_2, صحيح bottom,
                                صحيح کاٻي, صحيح ساڄي, صحيح borderType, ثابت Scalar& value )
{
    صحيح type = _src.type(), cn = CV_MAT_CN(type), اونهائي = CV_MAT_اونهائي(type),
            صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    منطقي isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    إذا ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        أرجع خطأ;

    ثابت حرف * ثابت borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    صحيح scalarcn = cn == 3 ? 4 : cn;
    صحيح sctype = CV_MAKETYPE(اونهائي, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memop_Type_منزل_Str(type), borderMap[borderType],
                                 ocl::memop_Type_منزل_Str(اونهائي), cn,
                                 ocl::memop_Type_منزل_Str(sctype), صفون_في_WI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat();
    إذا( src.isSubmatrix() && !isolated )
    {
        سائيز سڄو_سائيز;
        نقطو ofs;
        src.locateROI(سڄو_سائيز, ofs);
        صحيح dtop = std::min(ofs.y, چوٽي_2);
        صحيح dbottom = std::min(سڄو_سائيز.اوچائي - src.صفون - ofs.y, bottom);
        صحيح dleft = std::min(ofs.x, کاٻي);
        صحيح dright = std::min(سڄو_سائيز.ويڪر - src.کانن_ڳڻپ - ofs.x, ساڄي);
        src.adjustROI(dtop, dbottom, dleft, dright);
        چوٽي_2 -= dtop;
        کاٻي -= dleft;
        bottom -= dbottom;
        ساڄي -= dright;
    }

    _dst.create(src.صفون + چوٽي_2 + bottom, src.کانن_ڳڻپ + کاٻي + ساڄي, type);
    UMat dst = _dst.getUMat();

    إذا (چوٽي_2 == 0 && کاٻي == 0 && bottom == 0 && ساڄي == 0)
    {
        إذا(src.u != dst.u || src.قدم != dst.قدم)
            src.copy_منزل(dst);
        أرجع سچ;
    }

    k.دليلون(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           چوٽي_2, کاٻي, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.کانن_ڳڻپ, ((size_t)dst.صفون + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_IPP
نطاق cv {

ساكن منطقي ipp_copyMakeBorder( Mat &_src, Mat &_dst, صحيح چوٽي_2, صحيح bottom,
                                صحيح کاٻي, صحيح ساڄي, صحيح _borderType, ثابت Scalar& value )
{
#إذا defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_سائيز border_سائيز(کاٻي, چوٽي_2, ساڄي, bottom);
    ::ipp::Iwi_سائيز       size(_src.کانن_ڳڻپ, _src.صفون);
    Ipp_معلومات_Type          معلومات_Type   = ippi_Get_معلومات_Type(_src.اونهائي());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    إذا((صحيح)borderType == -1)
        أرجع خطأ;

    إذا(_src.dims > 2)
        أرجع خطأ;

    Rect dstRect(border_سائيز.کاٻي, border_سائيز.چوٽي_2,
        _dst.کانن_ڳڻپ - border_سائيز.ساڄي - border_سائيز.کاٻي,
        _dst.صفون - border_سائيز.bottom - border_سائيز.چوٽي_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->قدم, subDst.ptr(), subDst.قدم, size, معلومات_Type, _src.channels(), border_سائيز, borderType, &value[0]) >= 0;
#وإلا
    CV_اڻ_استعمال_ٿيل(_src); CV_اڻ_استعمال_ٿيل(_dst); CV_اڻ_استعمال_ٿيل(چوٽي_2); CV_اڻ_استعمال_ٿيل(bottom); CV_اڻ_استعمال_ٿيل(کاٻي); CV_اڻ_استعمال_ٿيل(ساڄي);
    CV_اڻ_استعمال_ٿيل(_borderType); CV_اڻ_استعمال_ٿيل(value);
    أرجع خطأ;
#جيڪڏهن_پورو
}
}
#جيڪڏهن_پورو

فراغ cv::copyMakeBorder( داخلا_Array _src, OutputArray _dst, صحيح چوٽي_2, صحيح bottom,
                         صحيح کاٻي, صحيح ساڄي, صحيح borderType, ثابت Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( چوٽي_2 >= 0 && bottom >= 0 && کاٻي >= 0 && ساڄي >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.صفون + چوٽي_2 + bottom <= وڌ_کان_وڌ_عدد );
    CV_Assert( (int64_t)src.کانن_ڳڻپ + کاٻي + ساڄي <= وڌ_کان_وڌ_عدد );

    CV_OCL_لاڳيتو(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, چوٽي_2, bottom, کاٻي, ساڄي, borderType, value))
    صحيح type = src.type();

    إذا( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        سائيز سڄو_سائيز;
        نقطو ofs;
        src.locateROI(سڄو_سائيز, ofs);
        صحيح dtop = std::min(ofs.y, چوٽي_2);
        صحيح dbottom = std::min(سڄو_سائيز.اوچائي - src.صفون - ofs.y, bottom);
        صحيح dleft = std::min(ofs.x, کاٻي);
        صحيح dright = std::min(سڄو_سائيز.ويڪر - src.کانن_ڳڻپ - ofs.x, ساڄي);
        src.adjustROI(dtop, dbottom, dleft, dright);
        چوٽي_2 -= dtop;
        کاٻي -= dleft;
        bottom -= dbottom;
        ساڄي -= dright;
    }

    _dst.create( src.صفون + چوٽي_2 + bottom, src.کانن_ڳڻپ + کاٻي + ساڄي, type );
    Mat dst = _dst.getMat();

    إذا(چوٽي_2 == 0 && کاٻي == 0 && bottom == 0 && ساڄي == 0)
    {
        إذا(src.معلومات != dst.معلومات || src.قدم != dst.قدم)
            src.copy_منزل(dst);
        أرجع;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_لاڳيتو_FAST(ipp_copyMakeBorder(src, dst, چوٽي_2, bottom, کاٻي, ساڄي, borderType, value))

    إذا( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.قدم, src.size(),
                           dst.ptr(), dst.قدم, dst.size(),
                           چوٽي_2, کاٻي, (صحيح)src.elem_سائيز(), borderType );
    وإلا
    {
        صحيح cn = src.channels(), cn1 = cn;
        Auto_بفر<مزدوج> buf(cn);
        إذا( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_منزل_Raw_معلومات(value, buf.معلومات(), CV_MAKETYPE(src.اونهائي(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.قدم, src.size(),
                                dst.ptr(), dst.قدم, dst.size(),
                                چوٽي_2, کاٻي, (صحيح)src.elem_سائيز(), (uchar*)buf.معلومات() );
    }
}
/* End of file. */
