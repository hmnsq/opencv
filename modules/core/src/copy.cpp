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

قالب <نام_نوع T> ساكن مضمن
فراغ scalar_مقصد_Raw_داده_2(ثابت Scalar& s, T * ثابت buf, ثابت صحيح cn, ثابت صحيح unroll_مقصد)
{
    صحيح i = 0;
    لكل(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    لكل(; i < unroll_مقصد; i++)
        buf[i] = buf[i-cn];
}

فراغ scalar_مقصد_Raw_داده(ثابت Scalar& s, فراغ* _buf, صحيح type, صحيح unroll_مقصد)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    بدل(عمق)
    {
    حالة CV_8U:
        scalar_مقصد_Raw_داده_2(s, (uchar*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_8S:
        scalar_مقصد_Raw_داده_2(s, (schar*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_Bool:
        scalar_مقصد_Raw_داده_2(s, (منطقي*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_16U:
        scalar_مقصد_Raw_داده_2(s, (ushort*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_16S:
        scalar_مقصد_Raw_داده_2(s, (کوتاه*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_16F:
        scalar_مقصد_Raw_داده_2(s, (hfloat*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_16BF:
        scalar_مقصد_Raw_داده_2(s, (bfloat*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_8F_E4M3FN:
        scalar_مقصد_Raw_داده_2(s, (fp8_t*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_8F_E4M3FNUZ:
        scalar_مقصد_Raw_داده_2(s, (fp8a_t*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_32U:
        scalar_مقصد_Raw_داده_2(s, (بیعلامت*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_32S:
        scalar_مقصد_Raw_داده_2(s, (صحيح*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_32F:
        scalar_مقصد_Raw_داده_2(s, (عائم*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_64U:
        scalar_مقصد_Raw_داده_2(s, (uint64_t*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_64S:
        scalar_مقصد_Raw_داده_2(s, (int64_t*)_buf, cn, unroll_مقصد);
        اكسر;
    حالة CV_64F:
        scalar_مقصد_Raw_داده_2(s, (مزدوج*)_buf, cn, unroll_مقصد);
        اكسر;
    افتراضي:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

فراغ convertAndUnrollScalar( ثابت Mat& sc, صحيح buftype, uchar* scbuf, size_t blocksize )
{
    صحيح scn = (صحيح)sc.مجموع(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_اندازه(buftype);
    دودویی_Func cvtFn = getConvertFunc(sc.عمق(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, اندازه(std::min(cn, scn), 1), 0);
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

قالب<نام_نوع T> ساكن فراغ
copy_ماسک(ثابت uchar* _src, size_t sstep, ثابت uchar* ماسک, size_t mstep, uchar* _dst, size_t dstep, اندازه size)
{
    لكل( ; size.ارتفاع--; ماسک += mstep, _src += sstep, _dst += dstep )
    {
        ثابت T* src = (ثابت T*)_src;
        T* dst = (T*)_dst;
        صحيح x = 0;
         #إذا CV_ENABLE_UNROLLED
        لكل( ; x <= size.عرض - 4; x += 4 )
        {
            إذا( ماسک[x] )
                dst[x] = src[x];
            إذا( ماسک[x+1] )
                dst[x+1] = src[x+1];
            إذا( ماسک[x+2] )
                dst[x+2] = src[x+2];
            إذا( ماسک[x+3] )
                dst[x+3] = src[x+3];
        }
        #پایان_اگر
        لكل( ; x < size.عرض; x++ )
            إذا( ماسک[x] )
                dst[x] = src[x];
    }
}

قالب<> فراغ
copy_ماسک<uchar>(ثابت uchar* _src, size_t sstep, ثابت uchar* ماسک, size_t mstep, uchar* _dst, size_t dstep, اندازه size)
{
    CV_IPP_پیاپی_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (صحيح)sstep, _dst, (صحيح)dstep, ippi_اندازه(size), ماسک, (صحيح)mstep) >= 0)

    لكل( ; size.ارتفاع--; ماسک += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحيح x = 0;
        #إذا (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            لكل( ; x <= size.عرض - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_بارگیری(src  + x),
                        v_dst   = vx_بارگیری(dst  + x),
                        v_nmask = v_eq(vx_بارگیری(ماسک + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #پایان_اگر
        لكل( ; x < size.عرض; x++ )
            إذا( ماسک[x] )
                dst[x] = src[x];
    }
}

قالب<> فراغ
copy_ماسک<Vec3b>(ثابت uchar* _src, size_t sstep, ثابت uchar* ماسک, size_t mstep, uchar* _dst, size_t dstep, اندازه size)
{
    لكل( ; size.ارتفاع--; ماسک += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; x <= size.عرض - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_بارگیری(ماسک + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_بارگیری_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_بارگیری_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#پایان_اگر
        لكل( ; x < size.عرض; x++ )
            إذا( ماسک[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

قالب<> فراغ
copy_ماسک<ushort>(ثابت uchar* _src, size_t sstep, ثابت uchar* ماسک, size_t mstep, uchar* _dst, size_t dstep, اندازه size)
{
    CV_IPP_پیاپی_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ثابت Ipp16u *)_src, (صحيح)sstep, (Ipp16u *)_dst, (صحيح)dstep, ippi_اندازه(size), ماسک, (صحيح)mstep) >= 0)

    لكل( ; size.ارتفاع--; ماسک += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحيح x = 0;
        #إذا (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            لكل( ; x <= size.عرض - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_بارگیری(src + x), v_src2 = vx_بارگیری(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_بارگیری(dst + x), v_dst2 = vx_بارگیری(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_بارگیری(ماسک + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #پایان_اگر
        لكل( ; x < size.عرض; x++ )
            إذا( ماسک[x] )
                dst[x] = src[x];
    }
}

قالب<> فراغ
copy_ماسک<Vec3s>(ثابت uchar* _src, size_t sstep, ثابت uchar* ماسک, size_t mstep, uchar* _dst, size_t dstep, اندازه size)
{
    لكل( ; size.ارتفاع--; ماسک += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل( ; x <= size.عرض - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_بارگیری(ماسک + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_بارگیری_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_بارگیری_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_بارگیری_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_بارگیری_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

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
#پایان_اگر
        لكل( ; x < size.عرض; x++ )
            إذا( ماسک[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

قالب<> فراغ
copy_ماسک<صحيح>(ثابت uchar* _src, size_t sstep, ثابت uchar* ماسک, size_t mstep, uchar* _dst, size_t dstep, اندازه size)
{
    لكل( ; size.ارتفاع--; ماسک += mstep, _src += sstep, _dst += dstep )
    {
        ثابت صحيح* src = (ثابت صحيح*)_src;
        صحيح* dst = (صحيح*)_dst;
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        لكل (; x <= size.عرض - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_بارگیری(src + x), v_dst0 = vx_بارگیری(dst + x);
            v_int32 v_src1 = vx_بارگیری(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_بارگیری(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_بارگیری(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_بارگیری(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_بارگیری(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_بارگیری(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_بارگیری(ماسک + x), vx_setzero_u8());
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
#پایان_اگر
        لكل (; x < size.عرض; x++)
            إذا ( ماسک[x] )
                dst[x] = src[x];
    }
}

ساكن فراغ
copy_ماسک_Generic(ثابت uchar* _src, size_t sstep, ثابت uchar* ماسک, size_t mstep, uchar* _dst, size_t dstep, اندازه size, فراغ* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    لكل( ; size.ارتفاع--; ماسک += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = _src;
        uchar* dst = _dst;
        صحيح x = 0;
        لكل( ; x < size.عرض; x++, src += esz, dst += esz )
        {
            إذا( !ماسک[x] )
                استمر;
            لكل( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#تعریف_کن DEF_COPY_ماسک(suffix, type) \
ساكن فراغ copy_ماسک_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* ماسک, size_t mstep, \
                             uchar* dst, size_t dstep, اندازه size, فراغ*) \
{ \
    copy_ماسک<type>(src, sstep, ماسک, mstep, dst, dstep, size); \
}

#إذا defined HAVE_IPP
#تعریف_کن DEF_COPY_ماسک_F(suffix, type, ippfavor, ipptype) \
ساكن فراغ copy_ماسک_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* ماسک, size_t mstep, \
                             uchar* dst, size_t dstep, اندازه size, فراغ*) \
{ \
    CV_IPP_پیاپی_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ثابت ipptype *)src, (صحيح)sstep, (ipptype *)dst, (صحيح)dstep, ippi_اندازه(size), (ثابت Ipp8u *)ماسک, (صحيح)mstep) >= 0)\
    copy_ماسک<type>(src, sstep, ماسک, mstep, dst, dstep, size); \
}
#وإلا
#تعریف_کن DEF_COPY_ماسک_F(suffix, type, ippfavor, ipptype) \
ساكن فراغ copy_ماسک_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* ماسک, size_t mstep, \
                             uchar* dst, size_t dstep, اندازه size, فراغ*) \
{ \
    copy_ماسک<type>(src, sstep, ماسک, mstep, dst, dstep, size); \
}
#پایان_اگر

#إذا IPP_نسخه_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_ماسک(32sC3, Vec3i)
DEF_COPY_ماسک(8uC3, Vec3b)
#وإلا
DEF_COPY_ماسک_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_ماسک_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#پایان_اگر
DEF_COPY_ماسک(8u, uchar)
DEF_COPY_ماسک(16u, ushort)
DEF_COPY_ماسک_F(32s, صحيح, 32s_C1MR, Ipp32s)
DEF_COPY_ماسک_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_ماسک(32sC2, Vec2i)
DEF_COPY_ماسک_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_ماسک(32sC6, Vec6i)
DEF_COPY_ماسک(32sC8, Vec8i)

دودویی_Func copy_ماسک_Tab[] =
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

دودویی_Func get_Copy_ماسک_Func(size_t esz)
{
    أرجع esz <= 32 && copy_ماسک_Tab[esz] ? copy_ماسک_Tab[esz] : copy_ماسک_Generic;
}

/* dst = src */
فراغ Mat::copy_مقصد( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

#اگر_تعریف_شده HAVE_CUDA
    إذا (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*هذا);
        أرجع;
    }
#پایان_اگر

    صحيح stype = type();
    صحيح dtype = _dst.type();
    إذا( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_مقصد( _dst, dtype );
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
        _dst.نوع() == درون_داد_Array_2::STD_وکتور ||
        (_dst.fixed_اندازه() && _dst.dims() == 1);
    إذا( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        إذا (empty())
            أرجع;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != عدم);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_اندازه(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        لكل( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        صحيح lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, داده, std::max(dims, 1), sz, dstofs, dst.گام.p, گام.p);
        أرجع;
    }

    إذا( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        إذا( داده == dst.داده )
            أرجع;

        إذا( ردیفها > 0 && تعداد_ستون > 0 )
        {
            Mat src = *هذا;
            اندازه sz = getContinuousSize2D(src, dst, (صحيح)elem_اندازه());
            CV_CheckGE(sz.عرض, 0, "");

            ثابت uchar* sptr = src.داده;
            uchar* dptr = dst.داده;

#إذا IPP_نسخه_X100 >= 201700
            CV_IPP_پیاپی_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (صحيح)src.گام, dptr, (صحيح)dst.گام, ippi_اندازه_L(sz.عرض, sz.ارتفاع)) >= 0)
#پایان_اگر

            لكل (; sz.ارتفاع--; sptr += src.گام, dptr += dst.گام)
                کپی_حافظه(dptr, sptr, sz.عرض);
        }
        أرجع;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    إذا( داده == dst.داده )
        أرجع;

    إذا( مجموع() != 0 )
    {
        ثابت Mat* arrays[] = { هذا, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_اندازه();

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            کپی_حافظه(ptrs[1], ptrs[0], sz);
    }
}

#اگر_تعریف_شده HAVE_IPP
ساكن منطقي ipp_copy_مقصد(ثابت Mat &src, Mat &dst, ثابت Mat &ماسک)
{
#اگر_تعریف_شده HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(ماسک.channels() > 1 || ماسک.عمق() != CV_8U)
        أرجع خطأ;

    إذا (src.dims <= 2)
    {
        Ippi_اندازه size = ippi_اندازه(src.size());
        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_ماسک, src.ptr(), (صحيح)src.گام, dst.ptr(), (صحيح)dst.گام, size, (صحيح)src.elemSize1(), src.channels(), ماسک.ptr(), (صحيح)ماسک.گام) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&src, &dst, &ماسک, عدم};
        uchar          *ptrs[3]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_اندازه size = ippi_اندازه(it.size, 1);

        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ماسک, ptrs[0], 0, ptrs[1], 0, size, (صحيح)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                أرجع خطأ;
        }
        أرجع درست;
    }
#وإلا
    CV_استفادهنشده(src); CV_استفادهنشده(dst); CV_استفادهنشده(ماسک);
    أرجع خطأ;
#پایان_اگر
}
#پایان_اگر

فراغ Mat::copy_مقصد( OutputArray _dst, درون_داد_Array ماسک_2 ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat ماسک = ماسک_2.getMat();
    إذا( !ماسک.داده )
    {
        copy_مقصد(_dst);
        أرجع;
    }

    صحيح cn = channels(), mcn = ماسک.channels();
    CV_Assert( (ماسک.عمق() == CV_8U || ماسک.عمق() == CV_8S || ماسک.عمق() == CV_Bool) && (mcn == 1 || mcn == cn) );
    منطقي color_ماسک = mcn > 1;
    إذا( dims <= 2 )
    {
        CV_Assert( size() == ماسک.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        إذا (dst.داده != dst0.داده) // re-allocation happened
        {
#اگر_تعریف_شده OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#پایان_اگر
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_پیاپی_FAST(ipp_copy_مقصد(*هذا, dst, ماسک))
    إذا ( هذا->dims <= 2 ) {
        إذا ( هذا->size() == dst.size() && هذا->size() == dst.size() ) {
            CALL_HAL(copy_مقصد_ماسک, cv_hal_copy_مقصد_Masked, هذا->داده, هذا->گام, dst.داده, dst.گام, هذا->تعداد_ستون, هذا->ردیفها, هذا->type(), ماسک.داده, ماسک.گام, ماسک.type());
        }
    }
    وإلا إذا ( هذا->isContinuous() && dst.isContinuous() && ماسک.isContinuous() )
    {
        size_t sz = هذا->مجموع();
        إذا (sz < بیشترین_صحیح) {
            CALL_HAL(copy_مقصد_ماسک, cv_hal_copy_مقصد_Masked, هذا->داده, 0, dst.داده, 0, (صحيح)sz, 1, هذا->type(), ماسک.داده, 0, ماسک.type());
        }
    }

    size_t esz = color_ماسک ? elemSize1() : elem_اندازه();
    دودویی_Func copymask = get_Copy_ماسک_Func(esz);

    إذا( dims <= 2 )
    {
        Mat src = *هذا;
        اندازه sz = getContinuousSize2D(src, dst, ماسک, mcn);
        copymask(src.داده, src.گام, ماسک.داده, ماسک.گام, dst.داده, dst.گام, sz, &esz);
        أرجع;
    }

    ثابت Mat* arrays[] = { هذا, &dst, &ماسک, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    اندازه sz((صحيح)(it.size*mcn), 1);

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
    copy_مقصد(_dst);
}
فراغ Mat::copyAt( OutputArray _dst, درون_داد_Array ماسک_2 ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_مقصد(_dst, ماسک_2);
}


ساكن منطقي can_apply_memset(ثابت Mat &mat, ثابت Scalar &s, صحيح &fill_مقدار)
{
    // check if depth is 1 byte.
    بدل (mat.عمق())
    {
    حالة CV_8U: fill_مقدار = saturate_cast<uchar>( s.val[0] ); اكسر;
    حالة CV_8S: fill_مقدار = saturate_cast<schar>( s.val[0] ); اكسر;
    افتراضي: أرجع خطأ;
    }

    // check if all element is same.
    ثابت int64* is = (ثابت int64*)&s.val[0];
    بدل (mat.channels())
    {
    حالة 1: أرجع درست;
    حالة 2: أرجع (is[0] == is[1]);
    حالة 3: أرجع (is[0] == is[1] && is[1] == is[2]);
    حالة 4: أرجع (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    افتراضي: أرجع خطأ;
    }
}

Mat& Mat::عملگر = (ثابت Scalar& s)
{
    CV_INSTRUMENT_REGION();

    إذا (هذا->empty())
        أرجع *هذا;

    ثابت Mat* arrays[] = { هذا };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elem_اندازه();
    ثابت int64* is = (ثابت int64*)&s.val[0];

    إذا( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    وإلا
    {
        صحيح fill_مقدار = 0;
        إذا ( can_apply_memset(*هذا, s, fill_مقدار) )
        {
            لكل (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_مقدار, elsize);
            أرجع *هذا;
        }

        إذا( it.nplanes > 0 )
        {
            مزدوج scalar[12];
            scalar_مقصد_Raw_داده(s, scalar, type(), 12);
            size_t بلوک_اندازه_2 = 12*elemSize1();

            لكل( size_t j = 0; j < elsize; j += بلوک_اندازه_2 )
            {
                size_t sz = MIN(بلوک_اندازه_2, elsize - j);
                CV_Assert(sz <= حجم(scalar));
                کپی_حافظه( dptr + j, scalar, sz );
            }
        }

        لكل( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            کپی_حافظه( dptr, داده, elsize );
        }
    }
    أرجع *هذا;
}

#اگر_تعریف_شده HAVE_IPP
ساكن منطقي ipp_Mat_مجموعه_مقصد_Mat(Mat &dst, Mat &_val, Mat &ماسک)
{
#اگر_تعریف_شده HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(ماسک.empty())
        أرجع خطأ;

    إذا(ماسک.عمق() != CV_8U || ماسک.channels() > 1)
        أرجع خطأ;

    إذا(dst.channels() > 4)
        أرجع خطأ;

    إذا (dst.عمق() == CV_32F)
    {
        لكل (صحيح i = 0; i < (صحيح)(_val.مجموع()); i++)
        {
            عائم v = (عائم)(_val.at<مزدوج>(i));  // cast to float
            إذا (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                أرجع خطأ;
        }
    }

    إذا(dst.dims <= 2)
    {
        Ippi_اندازه            size     = ippi_اندازه(dst.size());
        Ipp_داده_Type         داده_Type = ippi_Get_داده_Type(dst.عمق());
        ::ipp::Iw_مقدار_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_مجموعه_ماسک, s, dst.ptr(), (صحيح)dst.گام, size, داده_Type, dst.channels(), ماسک.ptr(), (صحيح)ماسک.گام) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, ماسک.empty()?عدم:&ماسک, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_اندازه            size     = {(صحيح)it.size, 1};
        Ipp_داده_Type         داده_Type = ippi_Get_داده_Type(dst.عمق());
        ::ipp::Iw_مقدار_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        لكل( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_مجموعه_ماسک, s, ptrs[0], 0, size, داده_Type, dst.channels(), ptrs[1], 0) < 0)
                أرجع خطأ;
        }
        أرجع درست;
    }
#وإلا
    CV_استفادهنشده(dst); CV_استفادهنشده(_val); CV_استفادهنشده(ماسک);
    أرجع خطأ;
#پایان_اگر
}
#پایان_اگر

Mat& Mat::مجموعه_مقصد(درون_داد_Array مقدار, درون_داد_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    إذا( empty() )
        أرجع *هذا;

    Mat value = مقدار.getMat(), ماسک = ماسک_2.getMat();

    CV_Assert( checkScalar(value, type(), مقدار.نوع(), درون_داد_Array_2::MAT ));
    صحيح cn = channels(), mcn = ماسک.channels();
    CV_Assert( ماسک.empty() || ((ماسک.عمق() == CV_8U || ماسک.عمق() == CV_8S || ماسک.عمق() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == ماسک.size) );

    CV_IPP_پیاپی_FAST(ipp_Mat_مجموعه_مقصد_Mat(*هذا, value, ماسک), *هذا)

    size_t esz = mcn > 1 ? elemSize1() : elem_اندازه();
    دودویی_Func copymask = get_Copy_ماسک_Func(esz);

    ثابت Mat* arrays[] = { هذا, !ماسک.empty() ? &ماسک : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    صحيح totalsz = (صحيح)it.size*mcn;
    صحيح بلوک_Size0 = std::min(totalsz, (صحيح)((بلوک_اندازه + esz-1)/esz));
    بلوک_Size0 -= بلوک_Size0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_میانگیر<uchar> _scbuf(بلوک_Size0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.داده(), (صحيح)حجم(مزدوج));
    convertAndUnrollScalar( value, type(), scbuf, بلوک_Size0/mcn );

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( صحيح j = 0; j < totalsz; j += بلوک_Size0 )
        {
            اندازه sz(std::min(بلوک_Size0, totalsz - j), 1);
            size_t بلوک_اندازه_2 = sz.عرض*esz;
            إذا( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.عرض;
            }
            وإلا
                کپی_حافظه(ptrs[0], scbuf, بلوک_اندازه_2);
            ptrs[0] += بلوک_اندازه_2;
        }
    }
    أرجع *هذا;
}


Mat& Mat::مجموعه_Zero()
{
    CV_INSTRUMENT_REGION();

    إذا( empty() )
        أرجع *هذا;

    size_t esz = elem_اندازه();

    ثابت Mat* arrays[] = { هذا, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    أرجع *هذا;
}


#إذا defined HAVE_OPENCL && !defined __APPLE__

ساكن منطقي ocl_repeat(درون_داد_Array _src, صحيح ny, صحيح nx, OutputArray _dst)
{
    إذا (ny == 1 && nx == 1)
    {
        _src.copy_مقصد(_dst);
        أرجع درست;
    }

    صحيح type = _src.type(), عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type),
            ردیفها_در_هر_WI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predict_Optimal_وکتور_عرض(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memop_Type_مقصد_Str(CV_MAKE_TYPE(عمق, kercn)),
                         nx, ny, ردیفها_در_هر_WI, kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.آرگومانها(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_اندازه(dst));

    size_t globalsize[] = { (size_t)src.تعداد_ستون * cn / kercn, ((size_t)src.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

فراغ repeat(درون_داد_Array _src, صحيح ny, صحيح nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    اندازه ssize = _src.size();
    إذا( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        أرجع;
    }
    CV_Assert( (int64_t)ssize.ارتفاع * ny <= بیشترین_صحیح && (int64_t)ssize.عرض * nx <= بیشترین_صحیح );
    _dst.create(ssize.ارتفاع*ny, ssize.عرض*nx, _src.type());

#إذا !defined __APPLE__
    CV_OCL_پیاپی(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#پایان_اگر

    Mat src = _src.getMat(), dst = _dst.getMat();
    اندازه dsize = dst.size();
    صحيح esz = (صحيح)src.elem_اندازه();
    صحيح x, y;
    ssize.عرض *= esz; dsize.عرض *= esz;

    لكل( y = 0; y < ssize.ارتفاع; y++ )
    {
        لكل( x = 0; x < dsize.عرض; x += ssize.عرض )
            کپی_حافظه( dst.ptr(y) + x, src.ptr(y), ssize.عرض );
    }

    لكل( ; y < dsize.ارتفاع; y++ )
        کپی_حافظه( dst.ptr(y), dst.ptr(y - ssize.ارتفاع), dsize.عرض );
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

#اگر_تعریف_شده CV_STATIC_ANALYSIS
    إذا(p >= 0 && p < len)
#وإلا
    إذا( (بیعلامت)p < (بیعلامت)len )
#پایان_اگر
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
#اگر_تعریف_شده CV_STATIC_ANALYSIS
        طالما(p < 0 || p >= len);
#وإلا
        طالما( (بیعلامت)p >= (بیعلامت)len );
#پایان_اگر
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

فراغ copyMakeBorder_8u( ثابت uchar* src, size_t srcstep, cv::اندازه srcroi,
                        uchar* dst, size_t dststep, cv::اندازه dstroi,
                        صحيح بالاترین_2, صحيح چپ, صحيح cn, صحيح borderType )
{
    ثابت صحيح isz = (صحيح)حجم(صحيح);
    صحيح i, j, k, elem_اندازه = 1;
    منطقي int_نما = خطأ;

    إذا( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_اندازه = isz;
        int_نما = درست;
    }

    cv::Auto_میانگیر<صحيح> _tab((dstroi.عرض - srcroi.عرض)*cn);
    صحيح* tab = _tab.داده();
    صحيح راست = dstroi.عرض - srcroi.عرض - چپ;
    صحيح bottom = dstroi.ارتفاع - srcroi.ارتفاع - بالاترین_2;

    لكل( i = 0; i < چپ; i++ )
    {
        j = cv::borderInterpolate(i - چپ, srcroi.عرض, borderType)*cn;
        لكل( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    لكل( i = 0; i < راست; i++ )
    {
        j = cv::borderInterpolate(srcroi.عرض + i, srcroi.عرض, borderType)*cn;
        لكل( k = 0; k < cn; k++ )
            tab[(i+چپ)*cn + k] = j + k;
    }

    srcroi.عرض *= cn;
    dstroi.عرض *= cn;
    چپ *= cn;
    راست *= cn;

    uchar* dstInner = dst + dststep*بالاترین_2 + چپ*elem_اندازه;

    لكل( i = 0; i < srcroi.ارتفاع; i++, dstInner += dststep, src += srcstep )
    {
        إذا( dstInner != src )
            کپی_حافظه(dstInner, src, srcroi.عرض*elem_اندازه);

        إذا( int_نما )
        {
            ثابت صحيح* isrc = (صحيح*)src;
            صحيح* idstInner = (صحيح*)dstInner;
            لكل( j = 0; j < چپ; j++ )
                idstInner[j - چپ] = isrc[tab[j]];
            لكل( j = 0; j < راست; j++ )
                idstInner[j + srcroi.عرض] = isrc[tab[j + چپ]];
        }
        وإلا
        {
            لكل( j = 0; j < چپ; j++ )
                dstInner[j - چپ] = src[tab[j]];
            لكل( j = 0; j < راست; j++ )
                dstInner[j + srcroi.عرض] = src[tab[j + چپ]];
        }
    }

    dstroi.عرض *= elem_اندازه;

    لكل( i = 0; i < بالاترین_2; i++ )
    {
        j = cv::borderInterpolate(i - بالاترین_2, srcroi.ارتفاع, borderType);
        کپی_حافظه(dst + i*dststep, dst + (بالاترین_2+j)*dststep, dstroi.عرض);
    }

    dst += dststep*بالاترین_2;
    لكل( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.ارتفاع, srcroi.ارتفاع, borderType);
        کپی_حافظه(dst + (i + srcroi.ارتفاع)*dststep, dst + j*dststep, dstroi.عرض);
    }
}


فراغ copyMakeConstBorder_8u( ثابت uchar* src, size_t srcstep, cv::اندازه srcroi,
                             uchar* dst, size_t dststep, cv::اندازه dstroi,
                             صحيح بالاترین_2, صحيح چپ, صحيح cn, ثابت uchar* value )
{
    صحيح i, j;
    cv::Auto_میانگیر<uchar> _constBuf(dstroi.عرض*cn);
    uchar* constBuf = _constBuf.داده();
    صحيح راست = dstroi.عرض - srcroi.عرض - چپ;
    صحيح bottom = dstroi.ارتفاع - srcroi.ارتفاع - بالاترین_2;

    لكل( i = 0; i < dstroi.عرض; i++ )
    {
        لكل( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.عرض *= cn;
    dstroi.عرض *= cn;
    چپ *= cn;
    راست *= cn;

    uchar* dstInner = dst + dststep*بالاترین_2 + چپ;

    لكل( i = 0; i < srcroi.ارتفاع; i++, dstInner += dststep, src += srcstep )
    {
        إذا( dstInner != src )
            کپی_حافظه( dstInner, src, srcroi.عرض );
        کپی_حافظه( dstInner - چپ, constBuf, چپ );
        کپی_حافظه( dstInner + srcroi.عرض, constBuf, راست );
    }

    لكل( i = 0; i < بالاترین_2; i++ )
        کپی_حافظه(dst + i * dststep, constBuf, dstroi.عرض);

    dst += (بالاترین_2 + srcroi.ارتفاع) * dststep;
    لكل( i = 0; i < bottom; i++ )
        کپی_حافظه(dst + i * dststep, constBuf, dstroi.عرض);
}

}

#اگر_تعریف_شده HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_copyMakeBorder( درون_داد_Array _src, OutputArray _dst, صحيح بالاترین_2, صحيح bottom,
                                صحيح چپ, صحيح راست, صحيح borderType, ثابت Scalar& value )
{
    صحيح type = _src.type(), cn = CV_MAT_CN(type), عمق = CV_MAT_عمق(type),
            ردیفها_در_هر_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    منطقي isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    إذا ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        أرجع خطأ;

    ثابت حرف * ثابت borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    صحيح scalarcn = cn == 3 ? 4 : cn;
    صحيح sctype = CV_MAKETYPE(عمق, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memop_Type_مقصد_Str(type), borderMap[borderType],
                                 ocl::memop_Type_مقصد_Str(عمق), cn,
                                 ocl::memop_Type_مقصد_Str(sctype), ردیفها_در_هر_WI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat();
    إذا( src.isSubmatrix() && !isolated )
    {
        اندازه کامل_اندازه;
        نقطه ofs;
        src.locateROI(کامل_اندازه, ofs);
        صحيح dtop = std::min(ofs.y, بالاترین_2);
        صحيح dbottom = std::min(کامل_اندازه.ارتفاع - src.ردیفها - ofs.y, bottom);
        صحيح dleft = std::min(ofs.x, چپ);
        صحيح dright = std::min(کامل_اندازه.عرض - src.تعداد_ستون - ofs.x, راست);
        src.adjustROI(dtop, dbottom, dleft, dright);
        بالاترین_2 -= dtop;
        چپ -= dleft;
        bottom -= dbottom;
        راست -= dright;
    }

    _dst.create(src.ردیفها + بالاترین_2 + bottom, src.تعداد_ستون + چپ + راست, type);
    UMat dst = _dst.getUMat();

    إذا (بالاترین_2 == 0 && چپ == 0 && bottom == 0 && راست == 0)
    {
        إذا(src.u != dst.u || src.گام != dst.گام)
            src.copy_مقصد(dst);
        أرجع درست;
    }

    k.آرگومانها(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           بالاترین_2, چپ, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.تعداد_ستون, ((size_t)dst.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

}
#پایان_اگر

#اگر_تعریف_شده HAVE_IPP
نطاق cv {

ساكن منطقي ipp_copyMakeBorder( Mat &_src, Mat &_dst, صحيح بالاترین_2, صحيح bottom,
                                صحيح چپ, صحيح راست, صحيح _borderType, ثابت Scalar& value )
{
#إذا defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_اندازه border_اندازه(چپ, بالاترین_2, راست, bottom);
    ::ipp::Iwi_اندازه       size(_src.تعداد_ستون, _src.ردیفها);
    Ipp_داده_Type          داده_Type   = ippi_Get_داده_Type(_src.عمق());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    إذا((صحيح)borderType == -1)
        أرجع خطأ;

    إذا(_src.dims > 2)
        أرجع خطأ;

    Rect dstRect(border_اندازه.چپ, border_اندازه.بالاترین_2,
        _dst.تعداد_ستون - border_اندازه.راست - border_اندازه.چپ,
        _dst.ردیفها - border_اندازه.bottom - border_اندازه.بالاترین_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->گام, subDst.ptr(), subDst.گام, size, داده_Type, _src.channels(), border_اندازه, borderType, &value[0]) >= 0;
#وإلا
    CV_استفادهنشده(_src); CV_استفادهنشده(_dst); CV_استفادهنشده(بالاترین_2); CV_استفادهنشده(bottom); CV_استفادهنشده(چپ); CV_استفادهنشده(راست);
    CV_استفادهنشده(_borderType); CV_استفادهنشده(value);
    أرجع خطأ;
#پایان_اگر
}
}
#پایان_اگر

فراغ cv::copyMakeBorder( درون_داد_Array _src, OutputArray _dst, صحيح بالاترین_2, صحيح bottom,
                         صحيح چپ, صحيح راست, صحيح borderType, ثابت Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( بالاترین_2 >= 0 && bottom >= 0 && چپ >= 0 && راست >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.ردیفها + بالاترین_2 + bottom <= بیشترین_صحیح );
    CV_Assert( (int64_t)src.تعداد_ستون + چپ + راست <= بیشترین_صحیح );

    CV_OCL_پیاپی(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, بالاترین_2, bottom, چپ, راست, borderType, value))
    صحيح type = src.type();

    إذا( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        اندازه کامل_اندازه;
        نقطه ofs;
        src.locateROI(کامل_اندازه, ofs);
        صحيح dtop = std::min(ofs.y, بالاترین_2);
        صحيح dbottom = std::min(کامل_اندازه.ارتفاع - src.ردیفها - ofs.y, bottom);
        صحيح dleft = std::min(ofs.x, چپ);
        صحيح dright = std::min(کامل_اندازه.عرض - src.تعداد_ستون - ofs.x, راست);
        src.adjustROI(dtop, dbottom, dleft, dright);
        بالاترین_2 -= dtop;
        چپ -= dleft;
        bottom -= dbottom;
        راست -= dright;
    }

    _dst.create( src.ردیفها + بالاترین_2 + bottom, src.تعداد_ستون + چپ + راست, type );
    Mat dst = _dst.getMat();

    إذا(بالاترین_2 == 0 && چپ == 0 && bottom == 0 && راست == 0)
    {
        إذا(src.داده != dst.داده || src.گام != dst.گام)
            src.copy_مقصد(dst);
        أرجع;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_پیاپی_FAST(ipp_copyMakeBorder(src, dst, بالاترین_2, bottom, چپ, راست, borderType, value))

    إذا( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.گام, src.size(),
                           dst.ptr(), dst.گام, dst.size(),
                           بالاترین_2, چپ, (صحيح)src.elem_اندازه(), borderType );
    وإلا
    {
        صحيح cn = src.channels(), cn1 = cn;
        Auto_میانگیر<مزدوج> buf(cn);
        إذا( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_مقصد_Raw_داده(value, buf.داده(), CV_MAKETYPE(src.عمق(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.گام, src.size(),
                                dst.ptr(), dst.گام, dst.size(),
                                بالاترین_2, چپ, (صحيح)src.elem_اندازه(), (uchar*)buf.داده() );
    }
}
/* End of file. */
