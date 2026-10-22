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

سانچا <قسم_ناں T> ساكن مضمن
فراغ scalar_منزل_Raw_انکڑے_2(ثابت Scalar& s, T * ثابت buf, ثابت صحيح cn, ثابت صحيح unroll_منزل)
{
    صحيح i = 0;
    لكل(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    لكل(; i < unroll_منزل; i++)
        buf[i] = buf[i-cn];
}

فراغ scalar_منزل_Raw_انکڑے(ثابت Scalar& s, فراغ* _buf, صحيح type, صحيح unroll_منزل)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    بدل(ڈونگھائی)
    {
    حالة CV_8U:
        scalar_منزل_Raw_انکڑے_2(s, (uchar*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_8S:
        scalar_منزل_Raw_انکڑے_2(s, (schar*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_Bool:
        scalar_منزل_Raw_انکڑے_2(s, (منطقي*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_16U:
        scalar_منزل_Raw_انکڑے_2(s, (ushort*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_16S:
        scalar_منزل_Raw_انکڑے_2(s, (چھوٹا*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_16F:
        scalar_منزل_Raw_انکڑے_2(s, (hfloat*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_16BF:
        scalar_منزل_Raw_انکڑے_2(s, (bfloat*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_8F_E4M3FN:
        scalar_منزل_Raw_انکڑے_2(s, (fp8_t*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_8F_E4M3FNUZ:
        scalar_منزل_Raw_انکڑے_2(s, (fp8a_t*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_32U:
        scalar_منزل_Raw_انکڑے_2(s, (بنانشانی*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_32S:
        scalar_منزل_Raw_انکڑے_2(s, (صحيح*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_32F:
        scalar_منزل_Raw_انکڑے_2(s, (عائم*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_64U:
        scalar_منزل_Raw_انکڑے_2(s, (uint64_t*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_64S:
        scalar_منزل_Raw_انکڑے_2(s, (int64_t*)_buf, cn, unroll_منزل);
        اكسر;
    حالة CV_64F:
        scalar_منزل_Raw_انکڑے_2(s, (مزدوج*)_buf, cn, unroll_منزل);
        اكسر;
    افتراضي:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

فراغ convertAndUnrollScalar( ثابت Mat& sc, صحيح buftype, uchar* scbuf, size_t blocksize )
{
    صحيح scn = (صحيح)sc.کل(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_ناپ(buftype);
    ثنائی_Func cvtFn = getConvertFunc(sc.ڈونگھائی(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, ناپ(std::min(cn, scn), 1), 0);
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

سانچا<قسم_ناں T> ساكن فراغ
copy_نقاب(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, ناپ size)
{
    لكل( ; size.اچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت T* src = (ثابت T*)_src;
        T* dst = (T*)_dst;
        صحيح x = 0;
         #إذا CV_ENABLE_UNROLLED
        لكل( ; x <= size.چوڑائی - 4; x += 4 )
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
        #جے_انت
        لكل( ; x < size.چوڑائی; x++ )
            إذا( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچا<> فراغ
copy_نقاب<uchar>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, ناپ size)
{
    CV_IPP_لگاتار_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (صحيح)sstep, _dst, (صحيح)dstep, ippi_ناپ(size), نقاب, (صحيح)mstep) >= 0)

    لكل( ; size.اچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحيح x = 0;
        #إذا (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            لكل( ; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_لدو(src  + x),
                        v_dst   = vx_لدو(dst  + x),
                        v_nmask = v_eq(vx_لدو(نقاب + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #جے_انت
        لكل( ; x < size.چوڑائی; x++ )
            إذا( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچا<> فراغ
copy_نقاب<Vec3b>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, ناپ size)
{
    لكل( ; size.اچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_لدو(نقاب + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_لدو_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_لدو_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#جے_انت
        لكل( ; x < size.چوڑائی; x++ )
            إذا( نقاب[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

سانچا<> فراغ
copy_نقاب<ushort>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, ناپ size)
{
    CV_IPP_لگاتار_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ثابت Ipp16u *)_src, (صحيح)sstep, (Ipp16u *)_dst, (صحيح)dstep, ippi_ناپ(size), نقاب, (صحيح)mstep) >= 0)

    لكل( ; size.اچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحيح x = 0;
        #إذا (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            لكل( ; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_لدو(src + x), v_src2 = vx_لدو(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_لدو(dst + x), v_dst2 = vx_لدو(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_لدو(نقاب + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #جے_انت
        لكل( ; x < size.چوڑائی; x++ )
            إذا( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچا<> فراغ
copy_نقاب<Vec3s>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, ناپ size)
{
    لكل( ; size.اچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_لدو(نقاب + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_لدو_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_لدو_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_لدو_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_لدو_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

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
#جے_انت
        لكل( ; x < size.چوڑائی; x++ )
            إذا( نقاب[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

سانچا<> فراغ
copy_نقاب<صحيح>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, ناپ size)
{
    لكل( ; size.اچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت صحيح* src = (ثابت صحيح*)_src;
        صحيح* dst = (صحيح*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل (; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_لدو(src + x), v_dst0 = vx_لدو(dst + x);
            v_int32 v_src1 = vx_لدو(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_لدو(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_لدو(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_لدو(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_لدو(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_لدو(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_لدو(نقاب + x), vx_setzero_u8());
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
#جے_انت
        لكل (; x < size.چوڑائی; x++)
            إذا ( نقاب[x] )
                dst[x] = src[x];
    }
}

ساكن فراغ
copy_نقاب_Generic(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, ناپ size, فراغ* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    لكل( ; size.اچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = _src;
        uchar* dst = _dst;
        صحيح x = 0;
        لكل( ; x < size.چوڑائی; x++, src += esz, dst += esz )
        {
            إذا( !نقاب[x] )
                استمر;
            لكل( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#تعریف_کرو DEF_COPY_نقاب(suffix, type) \
ساكن فراغ copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, ناپ size, فراغ*) \
{ \
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}

#إذا defined HAVE_IPP
#تعریف_کرو DEF_COPY_نقاب_F(suffix, type, ippfavor, ipptype) \
ساكن فراغ copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, ناپ size, فراغ*) \
{ \
    CV_IPP_لگاتار_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ثابت ipptype *)src, (صحيح)sstep, (ipptype *)dst, (صحيح)dstep, ippi_ناپ(size), (ثابت Ipp8u *)نقاب, (صحيح)mstep) >= 0)\
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}
#وإلا
#تعریف_کرو DEF_COPY_نقاب_F(suffix, type, ippfavor, ipptype) \
ساكن فراغ copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, ناپ size, فراغ*) \
{ \
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}
#جے_انت

#إذا IPP_نسخہ_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_نقاب(32sC3, Vec3i)
DEF_COPY_نقاب(8uC3, Vec3b)
#وإلا
DEF_COPY_نقاب_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_نقاب_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#جے_انت
DEF_COPY_نقاب(8u, uchar)
DEF_COPY_نقاب(16u, ushort)
DEF_COPY_نقاب_F(32s, صحيح, 32s_C1MR, Ipp32s)
DEF_COPY_نقاب_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_نقاب(32sC2, Vec2i)
DEF_COPY_نقاب_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_نقاب(32sC6, Vec6i)
DEF_COPY_نقاب(32sC8, Vec8i)

ثنائی_Func copy_نقاب_Tab[] =
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

ثنائی_Func get_Copy_نقاب_Func(size_t esz)
{
    أرجع esz <= 32 && copy_نقاب_Tab[esz] ? copy_نقاب_Tab[esz] : copy_نقاب_Generic;
}

/* dst = src */
فراغ Mat::copy_منزل( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

#جے_تعریف_شدہ HAVE_CUDA
    إذا (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*هذا);
        أرجع;
    }
#جے_انت

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
        _dst.قسم() == داخلہ_Array_2::STD_سمتیہ ||
        (_dst.fixed_ناپ() && _dst.dims() == 1);
    إذا( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        إذا (empty())
            أرجع;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != عدم);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_ناپ(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        لكل( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        صحيح lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, انکڑے, std::max(dims, 1), sz, dstofs, dst.قدم.p, قدم.p);
        أرجع;
    }

    إذا( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        إذا( انکڑے == dst.انکڑے )
            أرجع;

        إذا( صفاں > 0 && خانے_گنتی > 0 )
        {
            Mat src = *هذا;
            ناپ sz = getContinuousSize2D(src, dst, (صحيح)elem_ناپ());
            CV_CheckGE(sz.چوڑائی, 0, "");

            ثابت uchar* sptr = src.انکڑے;
            uchar* dptr = dst.انکڑے;

#إذا IPP_نسخہ_X100 >= 201700
            CV_IPP_لگاتار_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (صحيح)src.قدم, dptr, (صحيح)dst.قدم, ippi_ناپ_L(sz.چوڑائی, sz.اچائی)) >= 0)
#جے_انت

            لكل (; sz.اچائی--; sptr += src.قدم, dptr += dst.قدم)
                حافظہ_نقل(dptr, sptr, sz.چوڑائی);
        }
        أرجع;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    إذا( انکڑے == dst.انکڑے )
        أرجع;

    إذا( کل() != 0 )
    {
        ثابت Mat* arrays[] = { هذا, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_ناپ();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            حافظہ_نقل(ptrs[1], ptrs[0], sz);
    }
}

#جے_تعریف_شدہ HAVE_IPP
ساكن منطقي ipp_copy_منزل(ثابت Mat &src, Mat &dst, ثابت Mat &نقاب)
{
#جے_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(نقاب.channels() > 1 || نقاب.ڈونگھائی() != CV_8U)
        أرجع خطأ;

    إذا (src.dims <= 2)
    {
        Ippi_ناپ size = ippi_ناپ(src.size());
        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_نقاب, src.ptr(), (صحيح)src.قدم, dst.ptr(), (صحيح)dst.قدم, size, (صحيح)src.elemSize1(), src.channels(), نقاب.ptr(), (صحيح)نقاب.قدم) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&src, &dst, &نقاب, عدم};
        uchar          *ptrs[3]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ناپ size = ippi_ناپ(it.size, 1);

        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_نقاب, ptrs[0], 0, ptrs[1], 0, size, (صحيح)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_بن_ورتیا(src); CV_بن_ورتیا(dst); CV_بن_ورتیا(نقاب);
    أرجع خطأ;
#جے_انت
}
#جے_انت

فراغ Mat::copy_منزل( OutputArray _dst, داخلہ_Array نقاب_2 ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat نقاب = نقاب_2.getMat();
    إذا( !نقاب.انکڑے )
    {
        copy_منزل(_dst);
        أرجع;
    }

    صحيح cn = channels(), mcn = نقاب.channels();
    CV_Assert( (نقاب.ڈونگھائی() == CV_8U || نقاب.ڈونگھائی() == CV_8S || نقاب.ڈونگھائی() == CV_Bool) && (mcn == 1 || mcn == cn) );
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

        إذا (dst.انکڑے != dst0.انکڑے) // re-allocation happened
        {
#جے_تعریف_شدہ OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#جے_انت
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_لگاتار_FAST(ipp_copy_منزل(*هذا, dst, نقاب))
    إذا ( هذا->dims <= 2 ) {
        إذا ( هذا->size() == dst.size() && هذا->size() == dst.size() ) {
            CALL_HAL(copy_منزل_نقاب, cv_hal_copy_منزل_Masked, هذا->انکڑے, هذا->قدم, dst.انکڑے, dst.قدم, هذا->خانے_گنتی, هذا->صفاں, هذا->type(), نقاب.انکڑے, نقاب.قدم, نقاب.type());
        }
    }
    وإلا إذا ( هذا->isContinuous() && dst.isContinuous() && نقاب.isContinuous() )
    {
        size_t sz = هذا->کل();
        إذا (sz < ودھ_توں_ودھ_عدد) {
            CALL_HAL(copy_منزل_نقاب, cv_hal_copy_منزل_Masked, هذا->انکڑے, 0, dst.انکڑے, 0, (صحيح)sz, 1, هذا->type(), نقاب.انکڑے, 0, نقاب.type());
        }
    }

    size_t esz = color_نقاب ? elemSize1() : elem_ناپ();
    ثنائی_Func copymask = get_Copy_نقاب_Func(esz);

    إذا( dims <= 2 )
    {
        Mat src = *هذا;
        ناپ sz = getContinuousSize2D(src, dst, نقاب, mcn);
        copymask(src.انکڑے, src.قدم, نقاب.انکڑے, نقاب.قدم, dst.انکڑے, dst.قدم, sz, &esz);
        أرجع;
    }

    ثابت Mat* arrays[] = { هذا, &dst, &نقاب, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ناپ sz((صحيح)(it.size*mcn), 1);

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
فراغ Mat::copyAt( OutputArray _dst, داخلہ_Array نقاب_2 ) ثابت
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
    بدل (mat.ڈونگھائی())
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
    size_t elsize = it.size*elem_ناپ();
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
            scalar_منزل_Raw_انکڑے(s, scalar, type(), 12);
            size_t بلاک_ناپ_2 = 12*elemSize1();

            لكل( size_t j = 0; j < elsize; j += بلاک_ناپ_2 )
            {
                size_t sz = MIN(بلاک_ناپ_2, elsize - j);
                CV_Assert(sz <= حجم(scalar));
                حافظہ_نقل( dptr + j, scalar, sz );
            }
        }

        لكل( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            حافظہ_نقل( dptr, انکڑے, elsize );
        }
    }
    أرجع *هذا;
}

#جے_تعریف_شدہ HAVE_IPP
ساكن منطقي ipp_Mat_سیٹ_منزل_Mat(Mat &dst, Mat &_val, Mat &نقاب)
{
#جے_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(نقاب.empty())
        أرجع خطأ;

    إذا(نقاب.ڈونگھائی() != CV_8U || نقاب.channels() > 1)
        أرجع خطأ;

    إذا(dst.channels() > 4)
        أرجع خطأ;

    إذا (dst.ڈونگھائی() == CV_32F)
    {
        لكل (صحيح i = 0; i < (صحيح)(_val.کل()); i++)
        {
            عائم v = (عائم)(_val.at<مزدوج>(i));  // cast to float
            إذا (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                أرجع خطأ;
        }
    }

    إذا(dst.dims <= 2)
    {
        Ippi_ناپ            size     = ippi_ناپ(dst.size());
        Ipp_انکڑے_Type         انکڑے_Type = ippi_Get_انکڑے_Type(dst.ڈونگھائی());
        ::ipp::Iw_قدر_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_سیٹ_نقاب, s, dst.ptr(), (صحيح)dst.قدم, size, انکڑے_Type, dst.channels(), نقاب.ptr(), (صحيح)نقاب.قدم) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, نقاب.empty()?عدم:&نقاب, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ناپ            size     = {(صحيح)it.size, 1};
        Ipp_انکڑے_Type         انکڑے_Type = ippi_Get_انکڑے_Type(dst.ڈونگھائی());
        ::ipp::Iw_قدر_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        لكل( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_سیٹ_نقاب, s, ptrs[0], 0, size, انکڑے_Type, dst.channels(), ptrs[1], 0) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_بن_ورتیا(dst); CV_بن_ورتیا(_val); CV_بن_ورتیا(نقاب);
    أرجع خطأ;
#جے_انت
}
#جے_انت

Mat& Mat::سیٹ_منزل(داخلہ_Array قدر, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    إذا( empty() )
        أرجع *هذا;

    Mat value = قدر.getMat(), نقاب = نقاب_2.getMat();

    CV_Assert( checkScalar(value, type(), قدر.قسم(), داخلہ_Array_2::MAT ));
    صحيح cn = channels(), mcn = نقاب.channels();
    CV_Assert( نقاب.empty() || ((نقاب.ڈونگھائی() == CV_8U || نقاب.ڈونگھائی() == CV_8S || نقاب.ڈونگھائی() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == نقاب.size) );

    CV_IPP_لگاتار_FAST(ipp_Mat_سیٹ_منزل_Mat(*هذا, value, نقاب), *هذا)

    size_t esz = mcn > 1 ? elemSize1() : elem_ناپ();
    ثنائی_Func copymask = get_Copy_نقاب_Func(esz);

    ثابت Mat* arrays[] = { هذا, !نقاب.empty() ? &نقاب : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    صحيح totalsz = (صحيح)it.size*mcn;
    صحيح بلاک_Size0 = std::min(totalsz, (صحيح)((بلاک_ناپ + esz-1)/esz));
    بلاک_Size0 -= بلاک_Size0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_بفر<uchar> _scbuf(بلاک_Size0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.انکڑے(), (صحيح)حجم(مزدوج));
    convertAndUnrollScalar( value, type(), scbuf, بلاک_Size0/mcn );

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( صحيح j = 0; j < totalsz; j += بلاک_Size0 )
        {
            ناپ sz(std::min(بلاک_Size0, totalsz - j), 1);
            size_t بلاک_ناپ_2 = sz.چوڑائی*esz;
            إذا( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.چوڑائی;
            }
            وإلا
                حافظہ_نقل(ptrs[0], scbuf, بلاک_ناپ_2);
            ptrs[0] += بلاک_ناپ_2;
        }
    }
    أرجع *هذا;
}


Mat& Mat::سیٹ_Zero()
{
    CV_INSTRUMENT_REGION();

    إذا( empty() )
        أرجع *هذا;

    size_t esz = elem_ناپ();

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
        _src.copy_منزل(_dst);
        أرجع سچ;
    }

    صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type),
            صفاں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memop_Type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, kercn)),
                         nx, ny, صفاں_فی_WI, kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.دلیلاں(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_ناپ(dst));

    size_t globalsize[] = { (size_t)src.خانے_گنتی * cn / kercn, ((size_t)src.صفاں + صفاں_فی_WI - 1) / صفاں_فی_WI };
    أرجع k.لگاتار(2, globalsize, عدم, خطأ);
}

#جے_انت

فراغ repeat(داخلہ_Array _src, صحيح ny, صحيح nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    ناپ ssize = _src.size();
    إذا( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        أرجع;
    }
    CV_Assert( (int64_t)ssize.اچائی * ny <= ودھ_توں_ودھ_عدد && (int64_t)ssize.چوڑائی * nx <= ودھ_توں_ودھ_عدد );
    _dst.create(ssize.اچائی*ny, ssize.چوڑائی*nx, _src.type());

#إذا !defined __APPLE__
    CV_OCL_لگاتار(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#جے_انت

    Mat src = _src.getMat(), dst = _dst.getMat();
    ناپ dsize = dst.size();
    صحيح esz = (صحيح)src.elem_ناپ();
    صحيح x, y;
    ssize.چوڑائی *= esz; dsize.چوڑائی *= esz;

    لكل( y = 0; y < ssize.اچائی; y++ )
    {
        لكل( x = 0; x < dsize.چوڑائی; x += ssize.چوڑائی )
            حافظہ_نقل( dst.ptr(y) + x, src.ptr(y), ssize.چوڑائی );
    }

    لكل( ; y < dsize.اچائی; y++ )
        حافظہ_نقل( dst.ptr(y), dst.ptr(y - ssize.اچائی), dsize.چوڑائی );
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

#جے_تعریف_شدہ CV_STATIC_ANALYSIS
    إذا(p >= 0 && p < len)
#وإلا
    إذا( (بنانشانی)p < (بنانشانی)len )
#جے_انت
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
#جے_تعریف_شدہ CV_STATIC_ANALYSIS
        طالما(p < 0 || p >= len);
#وإلا
        طالما( (بنانشانی)p >= (بنانشانی)len );
#جے_انت
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

فراغ copyMakeBorder_8u( ثابت uchar* src, size_t srcstep, cv::ناپ srcroi,
                        uchar* dst, size_t dststep, cv::ناپ dstroi,
                        صحيح چوٹی_2, صحيح کھبے, صحيح cn, صحيح borderType )
{
    ثابت صحيح isz = (صحيح)حجم(صحيح);
    صحيح i, j, k, elem_ناپ = 1;
    منطقي int_منوال = خطأ;

    إذا( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_ناپ = isz;
        int_منوال = سچ;
    }

    cv::Auto_بفر<صحيح> _tab((dstroi.چوڑائی - srcroi.چوڑائی)*cn);
    صحيح* tab = _tab.انکڑے();
    صحيح سجے = dstroi.چوڑائی - srcroi.چوڑائی - کھبے;
    صحيح bottom = dstroi.اچائی - srcroi.اچائی - چوٹی_2;

    لكل( i = 0; i < کھبے; i++ )
    {
        j = cv::borderInterpolate(i - کھبے, srcroi.چوڑائی, borderType)*cn;
        لكل( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    لكل( i = 0; i < سجے; i++ )
    {
        j = cv::borderInterpolate(srcroi.چوڑائی + i, srcroi.چوڑائی, borderType)*cn;
        لكل( k = 0; k < cn; k++ )
            tab[(i+کھبے)*cn + k] = j + k;
    }

    srcroi.چوڑائی *= cn;
    dstroi.چوڑائی *= cn;
    کھبے *= cn;
    سجے *= cn;

    uchar* dstInner = dst + dststep*چوٹی_2 + کھبے*elem_ناپ;

    لكل( i = 0; i < srcroi.اچائی; i++, dstInner += dststep, src += srcstep )
    {
        إذا( dstInner != src )
            حافظہ_نقل(dstInner, src, srcroi.چوڑائی*elem_ناپ);

        إذا( int_منوال )
        {
            ثابت صحيح* isrc = (صحيح*)src;
            صحيح* idstInner = (صحيح*)dstInner;
            لكل( j = 0; j < کھبے; j++ )
                idstInner[j - کھبے] = isrc[tab[j]];
            لكل( j = 0; j < سجے; j++ )
                idstInner[j + srcroi.چوڑائی] = isrc[tab[j + کھبے]];
        }
        وإلا
        {
            لكل( j = 0; j < کھبے; j++ )
                dstInner[j - کھبے] = src[tab[j]];
            لكل( j = 0; j < سجے; j++ )
                dstInner[j + srcroi.چوڑائی] = src[tab[j + کھبے]];
        }
    }

    dstroi.چوڑائی *= elem_ناپ;

    لكل( i = 0; i < چوٹی_2; i++ )
    {
        j = cv::borderInterpolate(i - چوٹی_2, srcroi.اچائی, borderType);
        حافظہ_نقل(dst + i*dststep, dst + (چوٹی_2+j)*dststep, dstroi.چوڑائی);
    }

    dst += dststep*چوٹی_2;
    لكل( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.اچائی, srcroi.اچائی, borderType);
        حافظہ_نقل(dst + (i + srcroi.اچائی)*dststep, dst + j*dststep, dstroi.چوڑائی);
    }
}


فراغ copyMakeConstBorder_8u( ثابت uchar* src, size_t srcstep, cv::ناپ srcroi,
                             uchar* dst, size_t dststep, cv::ناپ dstroi,
                             صحيح چوٹی_2, صحيح کھبے, صحيح cn, ثابت uchar* value )
{
    صحيح i, j;
    cv::Auto_بفر<uchar> _constBuf(dstroi.چوڑائی*cn);
    uchar* constBuf = _constBuf.انکڑے();
    صحيح سجے = dstroi.چوڑائی - srcroi.چوڑائی - کھبے;
    صحيح bottom = dstroi.اچائی - srcroi.اچائی - چوٹی_2;

    لكل( i = 0; i < dstroi.چوڑائی; i++ )
    {
        لكل( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.چوڑائی *= cn;
    dstroi.چوڑائی *= cn;
    کھبے *= cn;
    سجے *= cn;

    uchar* dstInner = dst + dststep*چوٹی_2 + کھبے;

    لكل( i = 0; i < srcroi.اچائی; i++, dstInner += dststep, src += srcstep )
    {
        إذا( dstInner != src )
            حافظہ_نقل( dstInner, src, srcroi.چوڑائی );
        حافظہ_نقل( dstInner - کھبے, constBuf, کھبے );
        حافظہ_نقل( dstInner + srcroi.چوڑائی, constBuf, سجے );
    }

    لكل( i = 0; i < چوٹی_2; i++ )
        حافظہ_نقل(dst + i * dststep, constBuf, dstroi.چوڑائی);

    dst += (چوٹی_2 + srcroi.اچائی) * dststep;
    لكل( i = 0; i < bottom; i++ )
        حافظہ_نقل(dst + i * dststep, constBuf, dstroi.چوڑائی);
}

}

#جے_تعریف_شدہ HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_copyMakeBorder( داخلہ_Array _src, OutputArray _dst, صحيح چوٹی_2, صحيح bottom,
                                صحيح کھبے, صحيح سجے, صحيح borderType, ثابت Scalar& value )
{
    صحيح type = _src.type(), cn = CV_MAT_CN(type), ڈونگھائی = CV_MAT_ڈونگھائی(type),
            صفاں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    منطقي isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    إذا ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        أرجع خطأ;

    ثابت حرف * ثابت borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    صحيح scalarcn = cn == 3 ? 4 : cn;
    صحيح sctype = CV_MAKETYPE(ڈونگھائی, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memop_Type_منزل_Str(type), borderMap[borderType],
                                 ocl::memop_Type_منزل_Str(ڈونگھائی), cn,
                                 ocl::memop_Type_منزل_Str(sctype), صفاں_فی_WI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat();
    إذا( src.isSubmatrix() && !isolated )
    {
        ناپ سارا_ناپ;
        نقطہ ofs;
        src.locateROI(سارا_ناپ, ofs);
        صحيح dtop = std::min(ofs.y, چوٹی_2);
        صحيح dbottom = std::min(سارا_ناپ.اچائی - src.صفاں - ofs.y, bottom);
        صحيح dleft = std::min(ofs.x, کھبے);
        صحيح dright = std::min(سارا_ناپ.چوڑائی - src.خانے_گنتی - ofs.x, سجے);
        src.adjustROI(dtop, dbottom, dleft, dright);
        چوٹی_2 -= dtop;
        کھبے -= dleft;
        bottom -= dbottom;
        سجے -= dright;
    }

    _dst.create(src.صفاں + چوٹی_2 + bottom, src.خانے_گنتی + کھبے + سجے, type);
    UMat dst = _dst.getUMat();

    إذا (چوٹی_2 == 0 && کھبے == 0 && bottom == 0 && سجے == 0)
    {
        إذا(src.u != dst.u || src.قدم != dst.قدم)
            src.copy_منزل(dst);
        أرجع سچ;
    }

    k.دلیلاں(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           چوٹی_2, کھبے, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.خانے_گنتی, ((size_t)dst.صفاں + صفاں_فی_WI - 1) / صفاں_فی_WI };
    أرجع k.لگاتار(2, globalsize, عدم, خطأ);
}

}
#جے_انت

#جے_تعریف_شدہ HAVE_IPP
نطاق cv {

ساكن منطقي ipp_copyMakeBorder( Mat &_src, Mat &_dst, صحيح چوٹی_2, صحيح bottom,
                                صحيح کھبے, صحيح سجے, صحيح _borderType, ثابت Scalar& value )
{
#إذا defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_ناپ border_ناپ(کھبے, چوٹی_2, سجے, bottom);
    ::ipp::Iwi_ناپ       size(_src.خانے_گنتی, _src.صفاں);
    Ipp_انکڑے_Type          انکڑے_Type   = ippi_Get_انکڑے_Type(_src.ڈونگھائی());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    إذا((صحيح)borderType == -1)
        أرجع خطأ;

    إذا(_src.dims > 2)
        أرجع خطأ;

    Rect dstRect(border_ناپ.کھبے, border_ناپ.چوٹی_2,
        _dst.خانے_گنتی - border_ناپ.سجے - border_ناپ.کھبے,
        _dst.صفاں - border_ناپ.bottom - border_ناپ.چوٹی_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->قدم, subDst.ptr(), subDst.قدم, size, انکڑے_Type, _src.channels(), border_ناپ, borderType, &value[0]) >= 0;
#وإلا
    CV_بن_ورتیا(_src); CV_بن_ورتیا(_dst); CV_بن_ورتیا(چوٹی_2); CV_بن_ورتیا(bottom); CV_بن_ورتیا(کھبے); CV_بن_ورتیا(سجے);
    CV_بن_ورتیا(_borderType); CV_بن_ورتیا(value);
    أرجع خطأ;
#جے_انت
}
}
#جے_انت

فراغ cv::copyMakeBorder( داخلہ_Array _src, OutputArray _dst, صحيح چوٹی_2, صحيح bottom,
                         صحيح کھبے, صحيح سجے, صحيح borderType, ثابت Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( چوٹی_2 >= 0 && bottom >= 0 && کھبے >= 0 && سجے >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.صفاں + چوٹی_2 + bottom <= ودھ_توں_ودھ_عدد );
    CV_Assert( (int64_t)src.خانے_گنتی + کھبے + سجے <= ودھ_توں_ودھ_عدد );

    CV_OCL_لگاتار(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, چوٹی_2, bottom, کھبے, سجے, borderType, value))
    صحيح type = src.type();

    إذا( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        ناپ سارا_ناپ;
        نقطہ ofs;
        src.locateROI(سارا_ناپ, ofs);
        صحيح dtop = std::min(ofs.y, چوٹی_2);
        صحيح dbottom = std::min(سارا_ناپ.اچائی - src.صفاں - ofs.y, bottom);
        صحيح dleft = std::min(ofs.x, کھبے);
        صحيح dright = std::min(سارا_ناپ.چوڑائی - src.خانے_گنتی - ofs.x, سجے);
        src.adjustROI(dtop, dbottom, dleft, dright);
        چوٹی_2 -= dtop;
        کھبے -= dleft;
        bottom -= dbottom;
        سجے -= dright;
    }

    _dst.create( src.صفاں + چوٹی_2 + bottom, src.خانے_گنتی + کھبے + سجے, type );
    Mat dst = _dst.getMat();

    إذا(چوٹی_2 == 0 && کھبے == 0 && bottom == 0 && سجے == 0)
    {
        إذا(src.انکڑے != dst.انکڑے || src.قدم != dst.قدم)
            src.copy_منزل(dst);
        أرجع;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_لگاتار_FAST(ipp_copyMakeBorder(src, dst, چوٹی_2, bottom, کھبے, سجے, borderType, value))

    إذا( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.قدم, src.size(),
                           dst.ptr(), dst.قدم, dst.size(),
                           چوٹی_2, کھبے, (صحيح)src.elem_ناپ(), borderType );
    وإلا
    {
        صحيح cn = src.channels(), cn1 = cn;
        Auto_بفر<مزدوج> buf(cn);
        إذا( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_منزل_Raw_انکڑے(value, buf.انکڑے(), CV_MAKETYPE(src.ڈونگھائی(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.قدم, src.size(),
                                dst.ptr(), dst.قدم, dst.size(),
                                چوٹی_2, کھبے, (صحيح)src.elem_ناپ(), (uchar*)buf.انکڑے() );
    }
}
/* End of file. */
