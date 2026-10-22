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

نامخلا cv
{

سانچہ <قسم_نام T> ساکن اندرونی
خالی scalar_مقصد_Raw_ڈیٹا_2(ثابت Scalar& s, T * ثابت buf, ثابت صحیح cn, ثابت صحیح unroll_مقصد)
{
    صحیح i = 0;
    برائے(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    برائے(; i < unroll_مقصد; i++)
        buf[i] = buf[i-cn];
}

خالی scalar_مقصد_Raw_ڈیٹا(ثابت Scalar& s, خالی* _buf, صحیح type, صحیح unroll_مقصد)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    انتخاب(گہرائی)
    {
    حالت CV_8U:
        scalar_مقصد_Raw_ڈیٹا_2(s, (uchar*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_8S:
        scalar_مقصد_Raw_ڈیٹا_2(s, (schar*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_Bool:
        scalar_مقصد_Raw_ڈیٹا_2(s, (منطقی*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_16U:
        scalar_مقصد_Raw_ڈیٹا_2(s, (ushort*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_16S:
        scalar_مقصد_Raw_ڈیٹا_2(s, (مختصر*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_16F:
        scalar_مقصد_Raw_ڈیٹا_2(s, (hfloat*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_16BF:
        scalar_مقصد_Raw_ڈیٹا_2(s, (bfloat*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_8F_E4M3FN:
        scalar_مقصد_Raw_ڈیٹا_2(s, (fp8_t*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_8F_E4M3FNUZ:
        scalar_مقصد_Raw_ڈیٹا_2(s, (fp8a_t*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_32U:
        scalar_مقصد_Raw_ڈیٹا_2(s, (غیرعلامتی*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_32S:
        scalar_مقصد_Raw_ڈیٹا_2(s, (صحیح*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_32F:
        scalar_مقصد_Raw_ڈیٹا_2(s, (اعشاری*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_64U:
        scalar_مقصد_Raw_ڈیٹا_2(s, (uint64_t*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_64S:
        scalar_مقصد_Raw_ڈیٹا_2(s, (int64_t*)_buf, cn, unroll_مقصد);
        توڑو;
    حالت CV_64F:
        scalar_مقصد_Raw_ڈیٹا_2(s, (دوہرا*)_buf, cn, unroll_مقصد);
        توڑو;
    طےشدہ:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

خالی convertAndUnrollScalar( ثابت Mat& sc, صحیح buftype, uchar* scbuf, size_t blocksize )
{
    صحیح scn = (صحیح)sc.کل(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_سائز(buftype);
    ثنائی_Func cvtFn = getConvertFunc(sc.گہرائی(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, سائز(std::min(cn, scn), 1), 0);
    // unroll the scalar
    اگر( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        برائے( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    برائے( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

سانچہ<قسم_نام T> ساکن خالی
copy_نقاب(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائز size)
{
    برائے( ; size.اونچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت T* src = (ثابت T*)_src;
        T* dst = (T*)_dst;
        صحیح x = 0;
         #اگر CV_ENABLE_UNROLLED
        برائے( ; x <= size.چوڑائی - 4; x += 4 )
        {
            اگر( نقاب[x] )
                dst[x] = src[x];
            اگر( نقاب[x+1] )
                dst[x+1] = src[x+1];
            اگر( نقاب[x+2] )
                dst[x+2] = src[x+2];
            اگر( نقاب[x+3] )
                dst[x+3] = src[x+3];
        }
        #اگر_ختم
        برائے( ; x < size.چوڑائی; x++ )
            اگر( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچہ<> خالی
copy_نقاب<uchar>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائز size)
{
    CV_IPP_تسلسل_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (صحیح)sstep, _dst, (صحیح)dstep, ippi_سائز(size), نقاب, (صحیح)mstep) >= 0)

    برائے( ; size.اونچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحیح x = 0;
        #اگر (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            برائے( ; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint8 v_src   = vx_بھرو(src  + x),
                        v_dst   = vx_بھرو(dst  + x),
                        v_nmask = v_eq(vx_بھرو(نقاب + x), v_zero);

                v_dst = v_select(v_nmask, v_dst, v_src);
                v_store(dst + x, v_dst);
            }
        }
        vx_cleanup();
        #اگر_ختم
        برائے( ; x < size.چوڑائی; x++ )
            اگر( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچہ<> خالی
copy_نقاب<Vec3b>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائز size)
{
    برائے( ; size.اونچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = (ثابت uchar*)_src;
        uchar* dst = (uchar*)_dst;
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        برائے( ; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_بھرو(نقاب + x), vx_setzero_u8());

            v_uint8 v_src0, v_src1, v_src2;
            v_uint8 v_dst0, v_dst1, v_dst2;
            v_بھرو_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_بھرو_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);

            v_dst0 = v_select(v_nmask, v_dst0, v_src0);
            v_dst1 = v_select(v_nmask, v_dst1, v_src1);
            v_dst2 = v_select(v_nmask, v_dst2, v_src2);

            v_store_interleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
        }
        vx_cleanup();
#اگر_ختم
        برائے( ; x < size.چوڑائی; x++ )
            اگر( نقاب[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

سانچہ<> خالی
copy_نقاب<ushort>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائز size)
{
    CV_IPP_تسلسل_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ثابت Ipp16u *)_src, (صحیح)sstep, (Ipp16u *)_dst, (صحیح)dstep, ippi_سائز(size), نقاب, (صحیح)mstep) >= 0)

    برائے( ; size.اونچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحیح x = 0;
        #اگر (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            برائے( ; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
            {
                v_uint16 v_src1 = vx_بھرو(src + x), v_src2 = vx_بھرو(src + x + VTraits<v_uint16>::vlanes()),
                         v_dst1 = vx_بھرو(dst + x), v_dst2 = vx_بھرو(dst + x + VTraits<v_uint16>::vlanes());

                v_uint8 v_nmask1, v_nmask2;
                v_uint8 v_nmask = v_eq(vx_بھرو(نقاب + x), v_zero);
                v_zip(v_nmask, v_nmask, v_nmask1, v_nmask2);

                v_dst1 = v_select(v_reinterpret_as_u16(v_nmask1), v_dst1, v_src1);
                v_dst2 = v_select(v_reinterpret_as_u16(v_nmask2), v_dst2, v_src2);
                v_store(dst + x, v_dst1);
                v_store(dst + x + VTraits<v_uint16>::vlanes(), v_dst2);
            }
        }
        vx_cleanup();
        #اگر_ختم
        برائے( ; x < size.چوڑائی; x++ )
            اگر( نقاب[x] )
                dst[x] = src[x];
    }
}

سانچہ<> خالی
copy_نقاب<Vec3s>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائز size)
{
    برائے( ; size.اونچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت ushort* src = (ثابت ushort*)_src;
        ushort* dst = (ushort*)_dst;
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        برائے( ; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
        {
            v_uint8 v_nmask = v_eq(vx_بھرو(نقاب + x), vx_setzero_u8());
            v_uint8 v_nmask0, v_nmask1;
            v_zip(v_nmask, v_nmask, v_nmask0, v_nmask1);

            v_uint16 v_src0, v_src1, v_src2;
            v_uint16 v_dst0, v_dst1, v_dst2;
            v_بھرو_deinterleave(src + 3 * x, v_src0, v_src1, v_src2);
            v_بھرو_deinterleave(dst + 3 * x, v_dst0, v_dst1, v_dst2);
            v_uint16 v_src3, v_src4, v_src5;
            v_uint16 v_dst3, v_dst4, v_dst5;
            v_بھرو_deinterleave(src + 3 * (x + VTraits<v_uint16>::vlanes()), v_src3, v_src4, v_src5);
            v_بھرو_deinterleave(dst + 3 * (x + VTraits<v_uint16>::vlanes()), v_dst3, v_dst4, v_dst5);

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
#اگر_ختم
        برائے( ; x < size.چوڑائی; x++ )
            اگر( نقاب[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

سانچہ<> خالی
copy_نقاب<صحیح>(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائز size)
{
    برائے( ; size.اونچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت صحیح* src = (ثابت صحیح*)_src;
        صحیح* dst = (صحیح*)_dst;
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        برائے (; x <= size.چوڑائی - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
        {
            v_int32 v_src0 = vx_بھرو(src + x), v_dst0 = vx_بھرو(dst + x);
            v_int32 v_src1 = vx_بھرو(src + x +     VTraits<v_int32>::vlanes()), v_dst1 = vx_بھرو(dst + x +     VTraits<v_int32>::vlanes());
            v_int32 v_src2 = vx_بھرو(src + x + 2 * VTraits<v_int32>::vlanes()), v_dst2 = vx_بھرو(dst + x + 2 * VTraits<v_int32>::vlanes());
            v_int32 v_src3 = vx_بھرو(src + x + 3 * VTraits<v_int32>::vlanes()), v_dst3 = vx_بھرو(dst + x + 3 * VTraits<v_int32>::vlanes());

            v_uint8 v_nmask = v_eq(vx_بھرو(نقاب + x), vx_setzero_u8());
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
#اگر_ختم
        برائے (; x < size.چوڑائی; x++)
            اگر ( نقاب[x] )
                dst[x] = src[x];
    }
}

ساکن خالی
copy_نقاب_Generic(ثابت uchar* _src, size_t sstep, ثابت uchar* نقاب, size_t mstep, uchar* _dst, size_t dstep, سائز size, خالی* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    برائے( ; size.اونچائی--; نقاب += mstep, _src += sstep, _dst += dstep )
    {
        ثابت uchar* src = _src;
        uchar* dst = _dst;
        صحیح x = 0;
        برائے( ; x < size.چوڑائی; x++, src += esz, dst += esz )
        {
            اگر( !نقاب[x] )
                جاری;
            برائے( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#تعریف_کرو DEF_COPY_نقاب(suffix, type) \
ساکن خالی copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, سائز size, خالی*) \
{ \
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}

#اگر defined HAVE_IPP
#تعریف_کرو DEF_COPY_نقاب_F(suffix, type, ippfavor, ipptype) \
ساکن خالی copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, سائز size, خالی*) \
{ \
    CV_IPP_تسلسل_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ثابت ipptype *)src, (صحیح)sstep, (ipptype *)dst, (صحیح)dstep, ippi_سائز(size), (ثابت Ipp8u *)نقاب, (صحیح)mstep) >= 0)\
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}
#ورنہ
#تعریف_کرو DEF_COPY_نقاب_F(suffix, type, ippfavor, ipptype) \
ساکن خالی copy_نقاب_2##suffix(ثابت uchar* src, size_t sstep, ثابت uchar* نقاب, size_t mstep, \
                             uchar* dst, size_t dstep, سائز size, خالی*) \
{ \
    copy_نقاب<type>(src, sstep, نقاب, mstep, dst, dstep, size); \
}
#اگر_ختم

#اگر IPP_نسخہ_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_نقاب(32sC3, Vec3i)
DEF_COPY_نقاب(8uC3, Vec3b)
#ورنہ
DEF_COPY_نقاب_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_نقاب_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#اگر_ختم
DEF_COPY_نقاب(8u, uchar)
DEF_COPY_نقاب(16u, ushort)
DEF_COPY_نقاب_F(32s, صحیح, 32s_C1MR, Ipp32s)
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
    واپس esz <= 32 && copy_نقاب_Tab[esz] ? copy_نقاب_Tab[esz] : copy_نقاب_Generic;
}

/* dst = src */
خالی Mat::copy_مقصد( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

#اگر_تعریف_شدہ HAVE_CUDA
    اگر (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*یہ);
        واپس;
    }
#اگر_ختم

    صحیح stype = type();
    صحیح dtype = _dst.type();
    اگر( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_مقصد( _dst, dtype );
        واپس;
    }

    اگر( dims == 0 && empty() )
    {
        _dst.release();
        خالی* obj = _dst.getObj();
        اگر (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ورنہ اگر (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ورنہ اگر (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        واپس;
    }

    منطقی allowTransposed = dims == 1 ||
        _dst.قسم() == داخلہ_Array_2::STD_سمتیہ ||
        (_dst.fixed_سائز() && _dst.dims() == 1);
    اگر( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        اگر (empty())
            واپس;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != تہی);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_سائز(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        برائے( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        صحیح lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, ڈیٹا, std::max(dims, 1), sz, dstofs, dst.قدم.p, قدم.p);
        واپس;
    }

    اگر( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        اگر( ڈیٹا == dst.ڈیٹا )
            واپس;

        اگر( صفیں > 0 && ستون_تعداد > 0 )
        {
            Mat src = *یہ;
            سائز sz = getContinuousSize2D(src, dst, (صحیح)elem_سائز());
            CV_CheckGE(sz.چوڑائی, 0, "");

            ثابت uchar* sptr = src.ڈیٹا;
            uchar* dptr = dst.ڈیٹا;

#اگر IPP_نسخہ_X100 >= 201700
            CV_IPP_تسلسل_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (صحیح)src.قدم, dptr, (صحیح)dst.قدم, ippi_سائز_L(sz.چوڑائی, sz.اونچائی)) >= 0)
#اگر_ختم

            برائے (; sz.اونچائی--; sptr += src.قدم, dptr += dst.قدم)
                حافظہ_نقل(dptr, sptr, sz.چوڑائی);
        }
        واپس;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    اگر( ڈیٹا == dst.ڈیٹا )
        واپس;

    اگر( کل() != 0 )
    {
        ثابت Mat* arrays[] = { یہ, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_سائز();

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
            حافظہ_نقل(ptrs[1], ptrs[0], sz);
    }
}

#اگر_تعریف_شدہ HAVE_IPP
ساکن منطقی ipp_copy_مقصد(ثابت Mat &src, Mat &dst, ثابت Mat &نقاب)
{
#اگر_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    اگر(نقاب.channels() > 1 || نقاب.گہرائی() != CV_8U)
        واپس جھوٹ;

    اگر (src.dims <= 2)
    {
        Ippi_سائز size = ippi_سائز(src.size());
        واپس CV_INSTRUMENT_FUN_IPP(llwi_Copy_نقاب, src.ptr(), (صحیح)src.قدم, dst.ptr(), (صحیح)dst.قدم, size, (صحیح)src.elemSize1(), src.channels(), نقاب.ptr(), (صحیح)نقاب.قدم) >= 0;
    }
    ورنہ
    {
        ثابت Mat      *arrays[] = {&src, &dst, &نقاب, تہی};
        uchar          *ptrs[3]  = {تہی};
        NAryMatIterator it(arrays, ptrs);

        Ippi_سائز size = ippi_سائز(it.size, 1);

        برائے (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwi_Copy_نقاب, ptrs[0], 0, ptrs[1], 0, size, (صحیح)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                واپس جھوٹ;
        }
        واپس سچ;
    }
#ورنہ
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(dst); CV_غیر_مستعمل(نقاب);
    واپس جھوٹ;
#اگر_ختم
}
#اگر_ختم

خالی Mat::copy_مقصد( OutputArray _dst, داخلہ_Array نقاب_2 ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat نقاب = نقاب_2.getMat();
    اگر( !نقاب.ڈیٹا )
    {
        copy_مقصد(_dst);
        واپس;
    }

    صحیح cn = channels(), mcn = نقاب.channels();
    CV_Assert( (نقاب.گہرائی() == CV_8U || نقاب.گہرائی() == CV_8S || نقاب.گہرائی() == CV_Bool) && (mcn == 1 || mcn == cn) );
    منطقی color_نقاب = mcn > 1;
    اگر( dims <= 2 )
    {
        CV_Assert( size() == نقاب.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        اگر (dst.ڈیٹا != dst0.ڈیٹا) // re-allocation happened
        {
#اگر_تعریف_شدہ OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#اگر_ختم
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_تسلسل_FAST(ipp_copy_مقصد(*یہ, dst, نقاب))
    اگر ( یہ->dims <= 2 ) {
        اگر ( یہ->size() == dst.size() && یہ->size() == dst.size() ) {
            CALL_HAL(copy_مقصد_نقاب, cv_hal_copy_مقصد_Masked, یہ->ڈیٹا, یہ->قدم, dst.ڈیٹا, dst.قدم, یہ->ستون_تعداد, یہ->صفیں, یہ->type(), نقاب.ڈیٹا, نقاب.قدم, نقاب.type());
        }
    }
    ورنہ اگر ( یہ->isContinuous() && dst.isContinuous() && نقاب.isContinuous() )
    {
        size_t sz = یہ->کل();
        اگر (sz < زیادہ_سے_زیادہ_عدد) {
            CALL_HAL(copy_مقصد_نقاب, cv_hal_copy_مقصد_Masked, یہ->ڈیٹا, 0, dst.ڈیٹا, 0, (صحیح)sz, 1, یہ->type(), نقاب.ڈیٹا, 0, نقاب.type());
        }
    }

    size_t esz = color_نقاب ? elemSize1() : elem_سائز();
    ثنائی_Func copymask = get_Copy_نقاب_Func(esz);

    اگر( dims <= 2 )
    {
        Mat src = *یہ;
        سائز sz = getContinuousSize2D(src, dst, نقاب, mcn);
        copymask(src.ڈیٹا, src.قدم, نقاب.ڈیٹا, نقاب.قدم, dst.ڈیٹا, dst.قدم, sz, &esz);
        واپس;
    }

    ثابت Mat* arrays[] = { یہ, &dst, &نقاب, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    سائز sz((صحیح)(it.size*mcn), 1);

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
خالی Mat::copyAt( OutputArray _dst ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_مقصد(_dst);
}
خالی Mat::copyAt( OutputArray _dst, داخلہ_Array نقاب_2 ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copy_مقصد(_dst, نقاب_2);
}


ساکن منطقی can_apply_memset(ثابت Mat &mat, ثابت Scalar &s, صحیح &fill_قدر)
{
    // check if depth is 1 byte.
    انتخاب (mat.گہرائی())
    {
    حالت CV_8U: fill_قدر = saturate_cast<uchar>( s.val[0] ); توڑو;
    حالت CV_8S: fill_قدر = saturate_cast<schar>( s.val[0] ); توڑو;
    طےشدہ: واپس جھوٹ;
    }

    // check if all element is same.
    ثابت int64* is = (ثابت int64*)&s.val[0];
    انتخاب (mat.channels())
    {
    حالت 1: واپس سچ;
    حالت 2: واپس (is[0] == is[1]);
    حالت 3: واپس (is[0] == is[1] && is[1] == is[2]);
    حالت 4: واپس (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    طےشدہ: واپس جھوٹ;
    }
}

Mat& Mat::عامل = (ثابت Scalar& s)
{
    CV_INSTRUMENT_REGION();

    اگر (یہ->empty())
        واپس *یہ;

    ثابت Mat* arrays[] = { یہ };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elem_سائز();
    ثابت int64* is = (ثابت int64*)&s.val[0];

    اگر( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    ورنہ
    {
        صحیح fill_قدر = 0;
        اگر ( can_apply_memset(*یہ, s, fill_قدر) )
        {
            برائے (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_قدر, elsize);
            واپس *یہ;
        }

        اگر( it.nplanes > 0 )
        {
            دوہرا scalar[12];
            scalar_مقصد_Raw_ڈیٹا(s, scalar, type(), 12);
            size_t بلاک_سائز_2 = 12*elemSize1();

            برائے( size_t j = 0; j < elsize; j += بلاک_سائز_2 )
            {
                size_t sz = MIN(بلاک_سائز_2, elsize - j);
                CV_Assert(sz <= حجم(scalar));
                حافظہ_نقل( dptr + j, scalar, sz );
            }
        }

        برائے( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            حافظہ_نقل( dptr, ڈیٹا, elsize );
        }
    }
    واپس *یہ;
}

#اگر_تعریف_شدہ HAVE_IPP
ساکن منطقی ipp_Mat_سیٹ_مقصد_Mat(Mat &dst, Mat &_val, Mat &نقاب)
{
#اگر_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    اگر(نقاب.empty())
        واپس جھوٹ;

    اگر(نقاب.گہرائی() != CV_8U || نقاب.channels() > 1)
        واپس جھوٹ;

    اگر(dst.channels() > 4)
        واپس جھوٹ;

    اگر (dst.گہرائی() == CV_32F)
    {
        برائے (صحیح i = 0; i < (صحیح)(_val.کل()); i++)
        {
            اعشاری v = (اعشاری)(_val.at<دوہرا>(i));  // cast to float
            اگر (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                واپس جھوٹ;
        }
    }

    اگر(dst.dims <= 2)
    {
        Ippi_سائز            size     = ippi_سائز(dst.size());
        Ipp_ڈیٹا_Type         ڈیٹا_Type = ippi_Get_ڈیٹا_Type(dst.گہرائی());
        ::ipp::Iw_قدر_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        واپس CV_INSTRUMENT_FUN_IPP(llwi_سیٹ_نقاب, s, dst.ptr(), (صحیح)dst.قدم, size, ڈیٹا_Type, dst.channels(), نقاب.ptr(), (صحیح)نقاب.قدم) >= 0;
    }
    ورنہ
    {
        ثابت Mat      *arrays[] = {&dst, نقاب.empty()?تہی:&نقاب, تہی};
        uchar          *ptrs[2]  = {تہی};
        NAryMatIterator it(arrays, ptrs);

        Ippi_سائز            size     = {(صحیح)it.size, 1};
        Ipp_ڈیٹا_Type         ڈیٹا_Type = ippi_Get_ڈیٹا_Type(dst.گہرائی());
        ::ipp::Iw_قدر_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        برائے( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwi_سیٹ_نقاب, s, ptrs[0], 0, size, ڈیٹا_Type, dst.channels(), ptrs[1], 0) < 0)
                واپس جھوٹ;
        }
        واپس سچ;
    }
#ورنہ
    CV_غیر_مستعمل(dst); CV_غیر_مستعمل(_val); CV_غیر_مستعمل(نقاب);
    واپس جھوٹ;
#اگر_ختم
}
#اگر_ختم

Mat& Mat::سیٹ_مقصد(داخلہ_Array قدر, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    اگر( empty() )
        واپس *یہ;

    Mat value = قدر.getMat(), نقاب = نقاب_2.getMat();

    CV_Assert( checkScalar(value, type(), قدر.قسم(), داخلہ_Array_2::MAT ));
    صحیح cn = channels(), mcn = نقاب.channels();
    CV_Assert( نقاب.empty() || ((نقاب.گہرائی() == CV_8U || نقاب.گہرائی() == CV_8S || نقاب.گہرائی() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == نقاب.size) );

    CV_IPP_تسلسل_FAST(ipp_Mat_سیٹ_مقصد_Mat(*یہ, value, نقاب), *یہ)

    size_t esz = mcn > 1 ? elemSize1() : elem_سائز();
    ثنائی_Func copymask = get_Copy_نقاب_Func(esz);

    ثابت Mat* arrays[] = { یہ, !نقاب.empty() ? &نقاب : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    صحیح totalsz = (صحیح)it.size*mcn;
    صحیح بلاک_Size0 = std::min(totalsz, (صحیح)((بلاک_سائز + esz-1)/esz));
    بلاک_Size0 -= بلاک_Size0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_بفر<uchar> _scbuf(بلاک_Size0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.ڈیٹا(), (صحیح)حجم(دوہرا));
    convertAndUnrollScalar( value, type(), scbuf, بلاک_Size0/mcn );

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( صحیح j = 0; j < totalsz; j += بلاک_Size0 )
        {
            سائز sz(std::min(بلاک_Size0, totalsz - j), 1);
            size_t بلاک_سائز_2 = sz.چوڑائی*esz;
            اگر( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.چوڑائی;
            }
            ورنہ
                حافظہ_نقل(ptrs[0], scbuf, بلاک_سائز_2);
            ptrs[0] += بلاک_سائز_2;
        }
    }
    واپس *یہ;
}


Mat& Mat::سیٹ_Zero()
{
    CV_INSTRUMENT_REGION();

    اگر( empty() )
        واپس *یہ;

    size_t esz = elem_سائز();

    ثابت Mat* arrays[] = { یہ, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    واپس *یہ;
}


#اگر defined HAVE_OPENCL && !defined __APPLE__

ساکن منطقی ocl_repeat(داخلہ_Array _src, صحیح ny, صحیح nx, OutputArray _dst)
{
    اگر (ny == 1 && nx == 1)
    {
        _src.copy_مقصد(_dst);
        واپس سچ;
    }

    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type),
            صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memop_Type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)),
                         nx, ny, صفیں_فی_WI, kercn));
    اگر (k.empty())
        واپس جھوٹ;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.دلائل(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_سائز(dst));

    size_t globalsize[] = { (size_t)src.ستون_تعداد * cn / kercn, ((size_t)src.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

خالی repeat(داخلہ_Array _src, صحیح ny, صحیح nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    سائز ssize = _src.size();
    اگر( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        واپس;
    }
    CV_Assert( (int64_t)ssize.اونچائی * ny <= زیادہ_سے_زیادہ_عدد && (int64_t)ssize.چوڑائی * nx <= زیادہ_سے_زیادہ_عدد );
    _dst.create(ssize.اونچائی*ny, ssize.چوڑائی*nx, _src.type());

#اگر !defined __APPLE__
    CV_OCL_تسلسل(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#اگر_ختم

    Mat src = _src.getMat(), dst = _dst.getMat();
    سائز dsize = dst.size();
    صحیح esz = (صحیح)src.elem_سائز();
    صحیح x, y;
    ssize.چوڑائی *= esz; dsize.چوڑائی *= esz;

    برائے( y = 0; y < ssize.اونچائی; y++ )
    {
        برائے( x = 0; x < dsize.چوڑائی; x += ssize.چوڑائی )
            حافظہ_نقل( dst.ptr(y) + x, src.ptr(y), ssize.چوڑائی );
    }

    برائے( ; y < dsize.اونچائی; y++ )
        حافظہ_نقل( dst.ptr(y), dst.ptr(y - ssize.اونچائی), dsize.چوڑائی );
}

Mat repeat(ثابت Mat& src, صحیح ny, صحیح nx)
{
    اگر( nx == 1 && ny == 1 )
        واپس src;
    Mat dst;
    repeat(src, ny, nx, dst);
    واپس dst;
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

#اگر_تعریف_شدہ CV_STATIC_ANALYSIS
    اگر(p >= 0 && p < len)
#ورنہ
    اگر( (غیرعلامتی)p < (غیرعلامتی)len )
#اگر_ختم
        ;
    ورنہ اگر( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    ورنہ اگر( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        صحیح delta = borderType == BORDER_REFLECT_101;
        اگر( len == 1 )
            واپس 0;
        کرو
        {
            اگر( p < 0 )
                p = -p - 1 + delta;
            ورنہ
                p = len - 1 - (p - len) - delta;
        }
#اگر_تعریف_شدہ CV_STATIC_ANALYSIS
        جب(p < 0 || p >= len);
#ورنہ
        جب( (غیرعلامتی)p >= (غیرعلامتی)len );
#اگر_ختم
    }
    ورنہ اگر( borderType == BORDER_WRAP )
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
    ورنہ اگر( borderType == BORDER_CONSTANT )
        p = -1;
    ورنہ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    واپس p;
}

نامخلا
{

خالی copyMakeBorder_8u( ثابت uchar* src, size_t srcstep, cv::سائز srcroi,
                        uchar* dst, size_t dststep, cv::سائز dstroi,
                        صحیح چوٹی_2, صحیح بائیں, صحیح cn, صحیح borderType )
{
    ثابت صحیح isz = (صحیح)حجم(صحیح);
    صحیح i, j, k, elem_سائز = 1;
    منطقی int_منوال = جھوٹ;

    اگر( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_سائز = isz;
        int_منوال = سچ;
    }

    cv::Auto_بفر<صحیح> _tab((dstroi.چوڑائی - srcroi.چوڑائی)*cn);
    صحیح* tab = _tab.ڈیٹا();
    صحیح دائیں = dstroi.چوڑائی - srcroi.چوڑائی - بائیں;
    صحیح bottom = dstroi.اونچائی - srcroi.اونچائی - چوٹی_2;

    برائے( i = 0; i < بائیں; i++ )
    {
        j = cv::borderInterpolate(i - بائیں, srcroi.چوڑائی, borderType)*cn;
        برائے( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    برائے( i = 0; i < دائیں; i++ )
    {
        j = cv::borderInterpolate(srcroi.چوڑائی + i, srcroi.چوڑائی, borderType)*cn;
        برائے( k = 0; k < cn; k++ )
            tab[(i+بائیں)*cn + k] = j + k;
    }

    srcroi.چوڑائی *= cn;
    dstroi.چوڑائی *= cn;
    بائیں *= cn;
    دائیں *= cn;

    uchar* dstInner = dst + dststep*چوٹی_2 + بائیں*elem_سائز;

    برائے( i = 0; i < srcroi.اونچائی; i++, dstInner += dststep, src += srcstep )
    {
        اگر( dstInner != src )
            حافظہ_نقل(dstInner, src, srcroi.چوڑائی*elem_سائز);

        اگر( int_منوال )
        {
            ثابت صحیح* isrc = (صحیح*)src;
            صحیح* idstInner = (صحیح*)dstInner;
            برائے( j = 0; j < بائیں; j++ )
                idstInner[j - بائیں] = isrc[tab[j]];
            برائے( j = 0; j < دائیں; j++ )
                idstInner[j + srcroi.چوڑائی] = isrc[tab[j + بائیں]];
        }
        ورنہ
        {
            برائے( j = 0; j < بائیں; j++ )
                dstInner[j - بائیں] = src[tab[j]];
            برائے( j = 0; j < دائیں; j++ )
                dstInner[j + srcroi.چوڑائی] = src[tab[j + بائیں]];
        }
    }

    dstroi.چوڑائی *= elem_سائز;

    برائے( i = 0; i < چوٹی_2; i++ )
    {
        j = cv::borderInterpolate(i - چوٹی_2, srcroi.اونچائی, borderType);
        حافظہ_نقل(dst + i*dststep, dst + (چوٹی_2+j)*dststep, dstroi.چوڑائی);
    }

    dst += dststep*چوٹی_2;
    برائے( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.اونچائی, srcroi.اونچائی, borderType);
        حافظہ_نقل(dst + (i + srcroi.اونچائی)*dststep, dst + j*dststep, dstroi.چوڑائی);
    }
}


خالی copyMakeConstBorder_8u( ثابت uchar* src, size_t srcstep, cv::سائز srcroi,
                             uchar* dst, size_t dststep, cv::سائز dstroi,
                             صحیح چوٹی_2, صحیح بائیں, صحیح cn, ثابت uchar* value )
{
    صحیح i, j;
    cv::Auto_بفر<uchar> _constBuf(dstroi.چوڑائی*cn);
    uchar* constBuf = _constBuf.ڈیٹا();
    صحیح دائیں = dstroi.چوڑائی - srcroi.چوڑائی - بائیں;
    صحیح bottom = dstroi.اونچائی - srcroi.اونچائی - چوٹی_2;

    برائے( i = 0; i < dstroi.چوڑائی; i++ )
    {
        برائے( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.چوڑائی *= cn;
    dstroi.چوڑائی *= cn;
    بائیں *= cn;
    دائیں *= cn;

    uchar* dstInner = dst + dststep*چوٹی_2 + بائیں;

    برائے( i = 0; i < srcroi.اونچائی; i++, dstInner += dststep, src += srcstep )
    {
        اگر( dstInner != src )
            حافظہ_نقل( dstInner, src, srcroi.چوڑائی );
        حافظہ_نقل( dstInner - بائیں, constBuf, بائیں );
        حافظہ_نقل( dstInner + srcroi.چوڑائی, constBuf, دائیں );
    }

    برائے( i = 0; i < چوٹی_2; i++ )
        حافظہ_نقل(dst + i * dststep, constBuf, dstroi.چوڑائی);

    dst += (چوٹی_2 + srcroi.اونچائی) * dststep;
    برائے( i = 0; i < bottom; i++ )
        حافظہ_نقل(dst + i * dststep, constBuf, dstroi.چوڑائی);
}

}

#اگر_تعریف_شدہ HAVE_OPENCL

نامخلا cv {

ساکن منطقی ocl_copyMakeBorder( داخلہ_Array _src, OutputArray _dst, صحیح چوٹی_2, صحیح bottom,
                                صحیح بائیں, صحیح دائیں, صحیح borderType, ثابت Scalar& value )
{
    صحیح type = _src.type(), cn = CV_MAT_CN(type), گہرائی = CV_MAT_گہرائی(type),
            صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    منطقی isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    اگر ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        واپس جھوٹ;

    ثابت حرف * ثابت borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    صحیح scalarcn = cn == 3 ? 4 : cn;
    صحیح sctype = CV_MAKETYPE(گہرائی, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memop_Type_مقصد_Str(type), borderMap[borderType],
                                 ocl::memop_Type_مقصد_Str(گہرائی), cn,
                                 ocl::memop_Type_مقصد_Str(sctype), صفیں_فی_WI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    اگر (k.empty())
        واپس جھوٹ;

    UMat src = _src.getUMat();
    اگر( src.isSubmatrix() && !isolated )
    {
        سائز پورا_سائز;
        نقطہ ofs;
        src.locateROI(پورا_سائز, ofs);
        صحیح dtop = std::min(ofs.y, چوٹی_2);
        صحیح dbottom = std::min(پورا_سائز.اونچائی - src.صفیں - ofs.y, bottom);
        صحیح dleft = std::min(ofs.x, بائیں);
        صحیح dright = std::min(پورا_سائز.چوڑائی - src.ستون_تعداد - ofs.x, دائیں);
        src.adjustROI(dtop, dbottom, dleft, dright);
        چوٹی_2 -= dtop;
        بائیں -= dleft;
        bottom -= dbottom;
        دائیں -= dright;
    }

    _dst.create(src.صفیں + چوٹی_2 + bottom, src.ستون_تعداد + بائیں + دائیں, type);
    UMat dst = _dst.getUMat();

    اگر (چوٹی_2 == 0 && بائیں == 0 && bottom == 0 && دائیں == 0)
    {
        اگر(src.u != dst.u || src.قدم != dst.قدم)
            src.copy_مقصد(dst);
        واپس سچ;
    }

    k.دلائل(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           چوٹی_2, بائیں, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.ستون_تعداد, ((size_t)dst.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

}
#اگر_ختم

#اگر_تعریف_شدہ HAVE_IPP
نامخلا cv {

ساکن منطقی ipp_copyMakeBorder( Mat &_src, Mat &_dst, صحیح چوٹی_2, صحیح bottom,
                                صحیح بائیں, صحیح دائیں, صحیح _borderType, ثابت Scalar& value )
{
#اگر defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_سائز border_سائز(بائیں, چوٹی_2, دائیں, bottom);
    ::ipp::Iwi_سائز       size(_src.ستون_تعداد, _src.صفیں);
    Ipp_ڈیٹا_Type          ڈیٹا_Type   = ippi_Get_ڈیٹا_Type(_src.گہرائی());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    اگر((صحیح)borderType == -1)
        واپس جھوٹ;

    اگر(_src.dims > 2)
        واپس جھوٹ;

    Rect dstRect(border_سائز.بائیں, border_سائز.چوٹی_2,
        _dst.ستون_تعداد - border_سائز.دائیں - border_سائز.بائیں,
        _dst.صفیں - border_سائز.bottom - border_سائز.چوٹی_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    واپس CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->قدم, subDst.ptr(), subDst.قدم, size, ڈیٹا_Type, _src.channels(), border_سائز, borderType, &value[0]) >= 0;
#ورنہ
    CV_غیر_مستعمل(_src); CV_غیر_مستعمل(_dst); CV_غیر_مستعمل(چوٹی_2); CV_غیر_مستعمل(bottom); CV_غیر_مستعمل(بائیں); CV_غیر_مستعمل(دائیں);
    CV_غیر_مستعمل(_borderType); CV_غیر_مستعمل(value);
    واپس جھوٹ;
#اگر_ختم
}
}
#اگر_ختم

خالی cv::copyMakeBorder( داخلہ_Array _src, OutputArray _dst, صحیح چوٹی_2, صحیح bottom,
                         صحیح بائیں, صحیح دائیں, صحیح borderType, ثابت Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( چوٹی_2 >= 0 && bottom >= 0 && بائیں >= 0 && دائیں >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.صفیں + چوٹی_2 + bottom <= زیادہ_سے_زیادہ_عدد );
    CV_Assert( (int64_t)src.ستون_تعداد + بائیں + دائیں <= زیادہ_سے_زیادہ_عدد );

    CV_OCL_تسلسل(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, چوٹی_2, bottom, بائیں, دائیں, borderType, value))
    صحیح type = src.type();

    اگر( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        سائز پورا_سائز;
        نقطہ ofs;
        src.locateROI(پورا_سائز, ofs);
        صحیح dtop = std::min(ofs.y, چوٹی_2);
        صحیح dbottom = std::min(پورا_سائز.اونچائی - src.صفیں - ofs.y, bottom);
        صحیح dleft = std::min(ofs.x, بائیں);
        صحیح dright = std::min(پورا_سائز.چوڑائی - src.ستون_تعداد - ofs.x, دائیں);
        src.adjustROI(dtop, dbottom, dleft, dright);
        چوٹی_2 -= dtop;
        بائیں -= dleft;
        bottom -= dbottom;
        دائیں -= dright;
    }

    _dst.create( src.صفیں + چوٹی_2 + bottom, src.ستون_تعداد + بائیں + دائیں, type );
    Mat dst = _dst.getMat();

    اگر(چوٹی_2 == 0 && بائیں == 0 && bottom == 0 && دائیں == 0)
    {
        اگر(src.ڈیٹا != dst.ڈیٹا || src.قدم != dst.قدم)
            src.copy_مقصد(dst);
        واپس;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_تسلسل_FAST(ipp_copyMakeBorder(src, dst, چوٹی_2, bottom, بائیں, دائیں, borderType, value))

    اگر( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.قدم, src.size(),
                           dst.ptr(), dst.قدم, dst.size(),
                           چوٹی_2, بائیں, (صحیح)src.elem_سائز(), borderType );
    ورنہ
    {
        صحیح cn = src.channels(), cn1 = cn;
        Auto_بفر<دوہرا> buf(cn);
        اگر( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_مقصد_Raw_ڈیٹا(value, buf.ڈیٹا(), CV_MAKETYPE(src.گہرائی(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.قدم, src.size(),
                                dst.ptr(), dst.قدم, dst.size(),
                                چوٹی_2, بائیں, (صحیح)src.elem_سائز(), (uchar*)buf.ڈیٹا() );
    }
}
/* End of file. */
