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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

নামাকাশ cv
{

template <typename T> স্থির অন্তঃরেখ
শূন্য scalar_To_Raw_তথ্য_2(ধ্রুবক Scalar& s, T * ধ্রুবক buf, ধ্রুবক পূর্ণসংখ্যা cn, ধ্রুবক পূর্ণসংখ্যা unroll_to)
{
    পূর্ণসংখ্যা i = 0;
    জন্য(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    জন্য(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

শূন্য scalar_To_Raw_তথ্য(ধ্রুবক Scalar& s, শূন্য* _buf, পূর্ণসংখ্যা type, পূর্ণসংখ্যা unroll_to)
{
    CV_INSTRUMENT_REGION();

    ধ্রুবক পূর্ণসংখ্যা depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    নির্বাচন(depth)
    {
    ক্ষেত্র CV_8U:
        scalar_To_Raw_তথ্য_2(s, (uchar*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_8S:
        scalar_To_Raw_তথ্য_2(s, (schar*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_Bool:
        scalar_To_Raw_তথ্য_2(s, (যৌক্তিক*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_16U:
        scalar_To_Raw_তথ্য_2(s, (ushort*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_16S:
        scalar_To_Raw_তথ্য_2(s, (short*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_16F:
        scalar_To_Raw_তথ্য_2(s, (hfloat*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_16BF:
        scalar_To_Raw_তথ্য_2(s, (bfloat*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_8F_E4M3FN:
        scalar_To_Raw_তথ্য_2(s, (fp8_t*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_8F_E4M3FNUZ:
        scalar_To_Raw_তথ্য_2(s, (fp8a_t*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_32U:
        scalar_To_Raw_তথ্য_2(s, (unsigned*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_32S:
        scalar_To_Raw_তথ্য_2(s, (পূর্ণসংখ্যা*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_32F:
        scalar_To_Raw_তথ্য_2(s, (দশমিক*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_64U:
        scalar_To_Raw_তথ্য_2(s, (uint64_t*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_64S:
        scalar_To_Raw_তথ্য_2(s, (int64_t*)_buf, cn, unroll_to);
        বিরতি;
    ক্ষেত্র CV_64F:
        scalar_To_Raw_তথ্য_2(s, (দ্বিগুণ*)_buf, cn, unroll_to);
        বিরতি;
    পূর্বনির্ধারিত:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

শূন্য convertAndUnrollScalar( ধ্রুবক Mat& sc, পূর্ণসংখ্যা buftype, uchar* scbuf, size_t blocksize )
{
    পূর্ণসংখ্যা scn = (পূর্ণসংখ্যা)sc.যোগ(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_আকার(buftype);
    দ্বিমিক_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, আকার_2(std::min(cn, scn), 1), 0);
    // unroll the scalar
    যদি( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        জন্য( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    জন্য( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> স্থির শূন্য
copyMask_(ধ্রুবক uchar* _src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, uchar* _dst, size_t dstep, আকার_2 size)
{
    জন্য( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুবক T* src = (ধ্রুবক T*)_src;
        T* dst = (T*)_dst;
        পূর্ণসংখ্যা x = 0;
         #যদি CV_ENABLE_UNROLLED
        জন্য( ; x <= size.width - 4; x += 4 )
        {
            যদি( mask[x] )
                dst[x] = src[x];
            যদি( mask[x+1] )
                dst[x+1] = src[x+1];
            যদি( mask[x+2] )
                dst[x+2] = src[x+2];
            যদি( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        জন্য( ; x < size.width; x++ )
            যদি( mask[x] )
                dst[x] = src[x];
    }
}

template<> শূন্য
copyMask_<uchar>(ধ্রুবক uchar* _src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, uchar* _dst, size_t dstep, আকার_2 size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (পূর্ণসংখ্যা)sstep, _dst, (পূর্ণসংখ্যা)dstep, ippi_আকার(size), mask, (পূর্ণসংখ্যা)mstep) >= 0)

    জন্য( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুবক uchar* src = (ধ্রুবক uchar*)_src;
        uchar* dst = (uchar*)_dst;
        পূর্ণসংখ্যা x = 0;
        #যদি (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            জন্য( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        জন্য( ; x < size.width; x++ )
            যদি( mask[x] )
                dst[x] = src[x];
    }
}

template<> শূন্য
copyMask_<Vec3b>(ধ্রুবক uchar* _src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, uchar* _dst, size_t dstep, আকার_2 size)
{
    জন্য( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুবক uchar* src = (ধ্রুবক uchar*)_src;
        uchar* dst = (uchar*)_dst;
        পূর্ণসংখ্যা x = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
        জন্য( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        জন্য( ; x < size.width; x++ )
            যদি( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> শূন্য
copyMask_<ushort>(ধ্রুবক uchar* _src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, uchar* _dst, size_t dstep, আকার_2 size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ধ্রুবক Ipp16u *)_src, (পূর্ণসংখ্যা)sstep, (Ipp16u *)_dst, (পূর্ণসংখ্যা)dstep, ippi_আকার(size), mask, (পূর্ণসংখ্যা)mstep) >= 0)

    জন্য( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুবক ushort* src = (ধ্রুবক ushort*)_src;
        ushort* dst = (ushort*)_dst;
        পূর্ণসংখ্যা x = 0;
        #যদি (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            জন্য( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        জন্য( ; x < size.width; x++ )
            যদি( mask[x] )
                dst[x] = src[x];
    }
}

template<> শূন্য
copyMask_<Vec3s>(ধ্রুবক uchar* _src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, uchar* _dst, size_t dstep, আকার_2 size)
{
    জন্য( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুবক ushort* src = (ধ্রুবক ushort*)_src;
        ushort* dst = (ushort*)_dst;
        পূর্ণসংখ্যা x = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
        জন্য( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        জন্য( ; x < size.width; x++ )
            যদি( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> শূন্য
copyMask_<পূর্ণসংখ্যা>(ধ্রুবক uchar* _src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, uchar* _dst, size_t dstep, আকার_2 size)
{
    জন্য( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুবক পূর্ণসংখ্যা* src = (ধ্রুবক পূর্ণসংখ্যা*)_src;
        পূর্ণসংখ্যা* dst = (পূর্ণসংখ্যা*)_dst;
        পূর্ণসংখ্যা x = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
        জন্য (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        জন্য (; x < size.width; x++)
            যদি ( mask[x] )
                dst[x] = src[x];
    }
}

স্থির শূন্য
copyMaskGeneric(ধ্রুবক uchar* _src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, uchar* _dst, size_t dstep, আকার_2 size, শূন্য* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    জন্য( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুবক uchar* src = _src;
        uchar* dst = _dst;
        পূর্ণসংখ্যা x = 0;
        জন্য( ; x < size.width; x++, src += esz, dst += esz )
        {
            যদি( !mask[x] )
                চালু;
            জন্য( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
স্থির শূন্য copyMask##suffix(ধ্রুবক uchar* src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, আকার_2 size, শূন্য*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#যদি defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
স্থির শূন্য copyMask##suffix(ধ্রুবক uchar* src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, আকার_2 size, শূন্য*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ধ্রুবক ipptype *)src, (পূর্ণসংখ্যা)sstep, (ipptype *)dst, (পূর্ণসংখ্যা)dstep, ippi_আকার(size), (ধ্রুবক Ipp8u *)mask, (পূর্ণসংখ্যা)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#অন্যথা
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
স্থির শূন্য copyMask##suffix(ধ্রুবক uchar* src, size_t sstep, ধ্রুবক uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, আকার_2 size, শূন্য*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#যদি IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#অন্যথা
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, পূর্ণসংখ্যা, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

দ্বিমিক_Func copyMaskTab[] =
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

দ্বিমিক_Func getCopyMaskFunc(size_t esz)
{
    ফেরত esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
শূন্য Mat::copyTo( OutputArray _dst ) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    যদি (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*এই);
        ফেরত;
    }
#endif

    পূর্ণসংখ্যা stype = type();
    পূর্ণসংখ্যা dtype = _dst.type();
    যদি( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ফেরত;
    }

    যদি( dims == 0 && empty() )
    {
        _dst.release();
        শূন্য* obj = _dst.getObj();
        যদি (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        অন্যথা যদি (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        অন্যথা যদি (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ফেরত;
    }

    যৌক্তিক allowTransposed = dims == 1 ||
        _dst.kind() == প্রবেশ_Array_2::STD_VECTOR ||
        (_dst.fixed_আকার() && _dst.dims() == 1);
    যদি( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        যদি (empty())
            ফেরত;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != ফাঁকা);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_আকার(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        জন্য( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        পূর্ণসংখ্যা lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, তথ্য, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        ফেরত;
    }

    যদি( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        যদি( তথ্য == dst.তথ্য )
            ফেরত;

        যদি( rows > 0 && cols > 0 )
        {
            Mat src = *এই;
            আকার_2 sz = getContinuousSize2D(src, dst, (পূর্ণসংখ্যা)elem_আকার());
            CV_CheckGE(sz.width, 0, "");

            ধ্রুবক uchar* sptr = src.তথ্য;
            uchar* dptr = dst.তথ্য;

#যদি IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (পূর্ণসংখ্যা)src.step, dptr, (পূর্ণসংখ্যা)dst.step, ippi_আকার_L(sz.width, sz.height)) >= 0)
#endif

            জন্য (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        ফেরত;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    যদি( তথ্য == dst.তথ্য )
        ফেরত;

    যদি( যোগ() != 0 )
    {
        ধ্রুবক Mat* arrays[] = { এই, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elem_আকার();

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
স্থির যৌক্তিক ipp_copyTo(ধ্রুবক Mat &src, Mat &dst, ধ্রুবক Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    যদি(mask.channels() > 1 || mask.depth() != CV_8U)
        ফেরত মিথ্যা;

    যদি (src.dims <= 2)
    {
        Ippi_আকার size = ippi_আকার(src.size());
        ফেরত CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (পূর্ণসংখ্যা)src.step, dst.ptr(), (পূর্ণসংখ্যা)dst.step, size, (পূর্ণসংখ্যা)src.elemSize1(), src.channels(), mask.ptr(), (পূর্ণসংখ্যা)mask.step) >= 0;
    }
    অন্যথা
    {
        ধ্রুবক Mat      *arrays[] = {&src, &dst, &mask, ফাঁকা};
        uchar          *ptrs[3]  = {ফাঁকা};
        NAryMatIterator it(arrays, ptrs);

        Ippi_আকার size = ippi_আকার(it.size, 1);

        জন্য (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (পূর্ণসংখ্যা)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                ফেরত মিথ্যা;
        }
        ফেরত সত্য;
    }
#অন্যথা
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    ফেরত মিথ্যা;
#endif
}
#endif

শূন্য Mat::copyTo( OutputArray _dst, প্রবেশ_Array _mask ) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    যদি( !mask.তথ্য )
    {
        copyTo(_dst);
        ফেরত;
    }

    পূর্ণসংখ্যা cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    যৌক্তিক colorMask = mcn > 1;
    যদি( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        যদি (dst.তথ্য != dst0.তথ্য) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*এই, dst, mask))
    যদি ( এই->dims <= 2 ) {
        যদি ( এই->size() == dst.size() && এই->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, এই->তথ্য, এই->step, dst.তথ্য, dst.step, এই->cols, এই->rows, এই->type(), mask.তথ্য, mask.step, mask.type());
        }
    }
    অন্যথা যদি ( এই->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = এই->যোগ();
        যদি (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, এই->তথ্য, 0, dst.তথ্য, 0, (পূর্ণসংখ্যা)sz, 1, এই->type(), mask.তথ্য, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elem_আকার();
    দ্বিমিক_Func copymask = getCopyMaskFunc(esz);

    যদি( dims <= 2 )
    {
        Mat src = *এই;
        আকার_2 sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.তথ্য, src.step, mask.তথ্য, mask.step, dst.তথ্য, dst.step, sz, &esz);
        ফেরত;
    }

    ধ্রুবক Mat* arrays[] = { এই, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    আকার_2 sz((পূর্ণসংখ্যা)(it.size*mcn), 1);

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
শূন্য Mat::copyAt( OutputArray _dst ) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
শূন্য Mat::copyAt( OutputArray _dst, প্রবেশ_Array _mask ) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


স্থির যৌক্তিক can_apply_memset(ধ্রুবক Mat &mat, ধ্রুবক Scalar &s, পূর্ণসংখ্যা &fill_মান)
{
    // check if depth is 1 byte.
    নির্বাচন (mat.depth())
    {
    ক্ষেত্র CV_8U: fill_মান = saturate_cast<uchar>( s.val[0] ); বিরতি;
    ক্ষেত্র CV_8S: fill_মান = saturate_cast<schar>( s.val[0] ); বিরতি;
    পূর্বনির্ধারিত: ফেরত মিথ্যা;
    }

    // check if all element is same.
    ধ্রুবক int64* is = (ধ্রুবক int64*)&s.val[0];
    নির্বাচন (mat.channels())
    {
    ক্ষেত্র 1: ফেরত সত্য;
    ক্ষেত্র 2: ফেরত (is[0] == is[1]);
    ক্ষেত্র 3: ফেরত (is[0] == is[1] && is[1] == is[2]);
    ক্ষেত্র 4: ফেরত (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    পূর্বনির্ধারিত: ফেরত মিথ্যা;
    }
}

Mat& Mat::operator = (ধ্রুবক Scalar& s)
{
    CV_INSTRUMENT_REGION();

    যদি (এই->empty())
        ফেরত *এই;

    ধ্রুবক Mat* arrays[] = { এই };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elem_আকার();
    ধ্রুবক int64* is = (ধ্রুবক int64*)&s.val[0];

    যদি( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    অন্যথা
    {
        পূর্ণসংখ্যা fill_মান = 0;
        যদি ( can_apply_memset(*এই, s, fill_মান) )
        {
            জন্য (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_মান, elsize);
            ফেরত *এই;
        }

        যদি( it.nplanes > 0 )
        {
            দ্বিগুণ scalar[12];
            scalar_To_Raw_তথ্য(s, scalar, type(), 12);
            size_t block_আকার = 12*elemSize1();

            জন্য( size_t j = 0; j < elsize; j += block_আকার )
            {
                size_t sz = MIN(block_আকার, elsize - j);
                CV_Assert(sz <= আকার(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        জন্য( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, তথ্য, elsize );
        }
    }
    ফেরত *এই;
}

#ifdef HAVE_IPP
স্থির যৌক্তিক ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    যদি(mask.empty())
        ফেরত মিথ্যা;

    যদি(mask.depth() != CV_8U || mask.channels() > 1)
        ফেরত মিথ্যা;

    যদি(dst.channels() > 4)
        ফেরত মিথ্যা;

    যদি (dst.depth() == CV_32F)
    {
        জন্য (পূর্ণসংখ্যা i = 0; i < (পূর্ণসংখ্যা)(_val.যোগ()); i++)
        {
            দশমিক v = (দশমিক)(_val.at<দ্বিগুণ>(i));  // cast to float
            যদি (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                ফেরত মিথ্যা;
        }
    }

    যদি(dst.dims <= 2)
    {
        Ippi_আকার            size     = ippi_আকার(dst.size());
        Ipp_তথ্য_Type         তথ্য_Type = ippi_Get_তথ্য_Type(dst.depth());
        ::ipp::Iw_মান_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        ফেরত CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (পূর্ণসংখ্যা)dst.step, size, তথ্য_Type, dst.channels(), mask.ptr(), (পূর্ণসংখ্যা)mask.step) >= 0;
    }
    অন্যথা
    {
        ধ্রুবক Mat      *arrays[] = {&dst, mask.empty()?ফাঁকা:&mask, ফাঁকা};
        uchar          *ptrs[2]  = {ফাঁকা};
        NAryMatIterator it(arrays, ptrs);

        Ippi_আকার            size     = {(পূর্ণসংখ্যা)it.size, 1};
        Ipp_তথ্য_Type         তথ্য_Type = ippi_Get_তথ্য_Type(dst.depth());
        ::ipp::Iw_মান_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, তথ্য_Type, dst.channels(), ptrs[1], 0) < 0)
                ফেরত মিথ্যা;
        }
        ফেরত সত্য;
    }
#অন্যথা
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    ফেরত মিথ্যা;
#endif
}
#endif

Mat& Mat::setTo(প্রবেশ_Array মান, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    যদি( empty() )
        ফেরত *এই;

    Mat value = মান.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), মান.kind(), প্রবেশ_Array_2::MAT ));
    পূর্ণসংখ্যা cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*এই, value, mask), *এই)

    size_t esz = mcn > 1 ? elemSize1() : elem_আকার();
    দ্বিমিক_Func copymask = getCopyMaskFunc(esz);

    ধ্রুবক Mat* arrays[] = { এই, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা totalsz = (পূর্ণসংখ্যা)it.size*mcn;
    পূর্ণসংখ্যা blockSize0 = std::min(totalsz, (পূর্ণসংখ্যা)((BLOCK_আকার + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    Auto_বাফার<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.তথ্য(), (পূর্ণসংখ্যা)আকার(দ্বিগুণ));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( পূর্ণসংখ্যা j = 0; j < totalsz; j += blockSize0 )
        {
            আকার_2 sz(std::min(blockSize0, totalsz - j), 1);
            size_t block_আকার = sz.width*esz;
            যদি( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            অন্যথা
                memcpy(ptrs[0], scbuf, block_আকার);
            ptrs[0] += block_আকার;
        }
    }
    ফেরত *এই;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    যদি( empty() )
        ফেরত *এই;

    size_t esz = elem_আকার();

    ধ্রুবক Mat* arrays[] = { এই, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    ফেরত *এই;
}


#যদি defined HAVE_OPENCL && !defined __APPLE__

স্থির যৌক্তিক ocl_repeat(প্রবেশ_Array _src, পূর্ণসংখ্যা ny, পূর্ণসংখ্যা nx, OutputArray _dst)
{
    যদি (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        ফেরত সত্য;
    }

    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::Write_Only_No_আকার(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

শূন্য repeat(প্রবেশ_Array _src, পূর্ণসংখ্যা ny, পূর্ণসংখ্যা nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    আকার_2 ssize = _src.size();
    যদি( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        ফেরত;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#যদি !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    আকার_2 dsize = dst.size();
    পূর্ণসংখ্যা esz = (পূর্ণসংখ্যা)src.elem_আকার();
    পূর্ণসংখ্যা x, y;
    ssize.width *= esz; dsize.width *= esz;

    জন্য( y = 0; y < ssize.height; y++ )
    {
        জন্য( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    জন্য( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(ধ্রুবক Mat& src, পূর্ণসংখ্যা ny, পূর্ণসংখ্যা nx)
{
    যদি( nx == 1 && ny == 1 )
        ফেরত src;
    Mat dst;
    repeat(src, ny, nx, dst);
    ফেরত dst;
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
পূর্ণসংখ্যা cv::borderInterpolate( পূর্ণসংখ্যা p, পূর্ণসংখ্যা len, পূর্ণসংখ্যা borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    যদি(p >= 0 && p < len)
#অন্যথা
    যদি( (unsigned)p < (unsigned)len )
#endif
        ;
    অন্যথা যদি( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    অন্যথা যদি( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        পূর্ণসংখ্যা delta = borderType == BORDER_REFLECT_101;
        যদি( len == 1 )
            ফেরত 0;
        করো
        {
            যদি( p < 0 )
                p = -p - 1 + delta;
            অন্যথা
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        যখন(p < 0 || p >= len);
#অন্যথা
        যখন( (unsigned)p >= (unsigned)len );
#endif
    }
    অন্যথা যদি( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        যদি( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            যদি( p64 < 0 )
                p64 += len;
            p = (পূর্ণসংখ্যা)p64;
        }
    }
    অন্যথা যদি( borderType == BORDER_CONSTANT )
        p = -1;
    অন্যথা
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    ফেরত p;
}

নামাকাশ
{

শূন্য copyMakeBorder_8u( ধ্রুবক uchar* src, size_t srcstep, cv::আকার_2 srcroi,
                        uchar* dst, size_t dststep, cv::আকার_2 dstroi,
                        পূর্ণসংখ্যা শীর্ষ_2, পূর্ণসংখ্যা বাম, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা borderType )
{
    ধ্রুবক পূর্ণসংখ্যা isz = (পূর্ণসংখ্যা)আকার(পূর্ণসংখ্যা);
    পূর্ণসংখ্যা i, j, k, elem_আকার = 1;
    যৌক্তিক intMode = মিথ্যা;

    যদি( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elem_আকার = isz;
        intMode = সত্য;
    }

    cv::Auto_বাফার<পূর্ণসংখ্যা> _tab((dstroi.width - srcroi.width)*cn);
    পূর্ণসংখ্যা* tab = _tab.তথ্য();
    পূর্ণসংখ্যা ডান = dstroi.width - srcroi.width - বাম;
    পূর্ণসংখ্যা bottom = dstroi.height - srcroi.height - শীর্ষ_2;

    জন্য( i = 0; i < বাম; i++ )
    {
        j = cv::borderInterpolate(i - বাম, srcroi.width, borderType)*cn;
        জন্য( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    জন্য( i = 0; i < ডান; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        জন্য( k = 0; k < cn; k++ )
            tab[(i+বাম)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    বাম *= cn;
    ডান *= cn;

    uchar* dstInner = dst + dststep*শীর্ষ_2 + বাম*elem_আকার;

    জন্য( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        যদি( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elem_আকার);

        যদি( intMode )
        {
            ধ্রুবক পূর্ণসংখ্যা* isrc = (পূর্ণসংখ্যা*)src;
            পূর্ণসংখ্যা* idstInner = (পূর্ণসংখ্যা*)dstInner;
            জন্য( j = 0; j < বাম; j++ )
                idstInner[j - বাম] = isrc[tab[j]];
            জন্য( j = 0; j < ডান; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + বাম]];
        }
        অন্যথা
        {
            জন্য( j = 0; j < বাম; j++ )
                dstInner[j - বাম] = src[tab[j]];
            জন্য( j = 0; j < ডান; j++ )
                dstInner[j + srcroi.width] = src[tab[j + বাম]];
        }
    }

    dstroi.width *= elem_আকার;

    জন্য( i = 0; i < শীর্ষ_2; i++ )
    {
        j = cv::borderInterpolate(i - শীর্ষ_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (শীর্ষ_2+j)*dststep, dstroi.width);
    }

    dst += dststep*শীর্ষ_2;
    জন্য( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


শূন্য copyMakeConstBorder_8u( ধ্রুবক uchar* src, size_t srcstep, cv::আকার_2 srcroi,
                             uchar* dst, size_t dststep, cv::আকার_2 dstroi,
                             পূর্ণসংখ্যা শীর্ষ_2, পূর্ণসংখ্যা বাম, পূর্ণসংখ্যা cn, ধ্রুবক uchar* value )
{
    পূর্ণসংখ্যা i, j;
    cv::Auto_বাফার<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.তথ্য();
    পূর্ণসংখ্যা ডান = dstroi.width - srcroi.width - বাম;
    পূর্ণসংখ্যা bottom = dstroi.height - srcroi.height - শীর্ষ_2;

    জন্য( i = 0; i < dstroi.width; i++ )
    {
        জন্য( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    বাম *= cn;
    ডান *= cn;

    uchar* dstInner = dst + dststep*শীর্ষ_2 + বাম;

    জন্য( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        যদি( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - বাম, constBuf, বাম );
        memcpy( dstInner + srcroi.width, constBuf, ডান );
    }

    জন্য( i = 0; i < শীর্ষ_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (শীর্ষ_2 + srcroi.height) * dststep;
    জন্য( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির যৌক্তিক ocl_copyMakeBorder( প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা শীর্ষ_2, পূর্ণসংখ্যা bottom,
                                পূর্ণসংখ্যা বাম, পূর্ণসংখ্যা ডান, পূর্ণসংখ্যা borderType, ধ্রুবক Scalar& value )
{
    পূর্ণসংখ্যা type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    যৌক্তিক isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    যদি ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        ফেরত মিথ্যা;

    ধ্রুবক অক্ষর * ধ্রুবক borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    পূর্ণসংখ্যা scalarcn = cn == 3 ? 4 : cn;
    পূর্ণসংখ্যা sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src = _src.getUMat();
    যদি( src.isSubmatrix() && !isolated )
    {
        আকার_2 whole_আকার;
        Point ofs;
        src.locateROI(whole_আকার, ofs);
        পূর্ণসংখ্যা dtop = std::min(ofs.y, শীর্ষ_2);
        পূর্ণসংখ্যা dbottom = std::min(whole_আকার.height - src.rows - ofs.y, bottom);
        পূর্ণসংখ্যা dleft = std::min(ofs.x, বাম);
        পূর্ণসংখ্যা dright = std::min(whole_আকার.width - src.cols - ofs.x, ডান);
        src.adjustROI(dtop, dbottom, dleft, dright);
        শীর্ষ_2 -= dtop;
        বাম -= dleft;
        bottom -= dbottom;
        ডান -= dright;
    }

    _dst.create(src.rows + শীর্ষ_2 + bottom, src.cols + বাম + ডান, type);
    UMat dst = _dst.getUMat();

    যদি (শীর্ষ_2 == 0 && বাম == 0 && bottom == 0 && ডান == 0)
    {
        যদি(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        ফেরত সত্য;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           শীর্ষ_2, বাম, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

}
#endif

#ifdef HAVE_IPP
নামাকাশ cv {

স্থির যৌক্তিক ipp_copyMakeBorder( Mat &_src, Mat &_dst, পূর্ণসংখ্যা শীর্ষ_2, পূর্ণসংখ্যা bottom,
                                পূর্ণসংখ্যা বাম, পূর্ণসংখ্যা ডান, পূর্ণসংখ্যা _borderType, ধ্রুবক Scalar& value )
{
#যদি defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::Iwi_Border_আকার border_আকার(বাম, শীর্ষ_2, ডান, bottom);
    ::ipp::Iwi_আকার       size(_src.cols, _src.rows);
    Ipp_তথ্য_Type          তথ্য_Type   = ippi_Get_তথ্য_Type(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    যদি((পূর্ণসংখ্যা)borderType == -1)
        ফেরত মিথ্যা;

    যদি(_src.dims > 2)
        ফেরত মিথ্যা;

    Rect dstRect(border_আকার.বাম, border_আকার.শীর্ষ_2,
        _dst.cols - border_আকার.ডান - border_আকার.বাম,
        _dst.rows - border_আকার.bottom - border_আকার.শীর্ষ_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    ফেরত CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, তথ্য_Type, _src.channels(), border_আকার, borderType, &value[0]) >= 0;
#অন্যথা
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(শীর্ষ_2); CV_UNUSED(bottom); CV_UNUSED(বাম); CV_UNUSED(ডান);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    ফেরত মিথ্যা;
#endif
}
}
#endif

শূন্য cv::copyMakeBorder( প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা শীর্ষ_2, পূর্ণসংখ্যা bottom,
                         পূর্ণসংখ্যা বাম, পূর্ণসংখ্যা ডান, পূর্ণসংখ্যা borderType, ধ্রুবক Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( শীর্ষ_2 >= 0 && bottom >= 0 && বাম >= 0 && ডান >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + শীর্ষ_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + বাম + ডান <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, শীর্ষ_2, bottom, বাম, ডান, borderType, value))
    পূর্ণসংখ্যা type = src.type();

    যদি( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        আকার_2 whole_আকার;
        Point ofs;
        src.locateROI(whole_আকার, ofs);
        পূর্ণসংখ্যা dtop = std::min(ofs.y, শীর্ষ_2);
        পূর্ণসংখ্যা dbottom = std::min(whole_আকার.height - src.rows - ofs.y, bottom);
        পূর্ণসংখ্যা dleft = std::min(ofs.x, বাম);
        পূর্ণসংখ্যা dright = std::min(whole_আকার.width - src.cols - ofs.x, ডান);
        src.adjustROI(dtop, dbottom, dleft, dright);
        শীর্ষ_2 -= dtop;
        বাম -= dleft;
        bottom -= dbottom;
        ডান -= dright;
    }

    _dst.create( src.rows + শীর্ষ_2 + bottom, src.cols + বাম + ডান, type );
    Mat dst = _dst.getMat();

    যদি(শীর্ষ_2 == 0 && বাম == 0 && bottom == 0 && ডান == 0)
    {
        যদি(src.তথ্য != dst.তথ্য || src.step != dst.step)
            src.copyTo(dst);
        ফেরত;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, শীর্ষ_2, bottom, বাম, ডান, borderType, value))

    যদি( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           শীর্ষ_2, বাম, (পূর্ণসংখ্যা)src.elem_আকার(), borderType );
    অন্যথা
    {
        পূর্ণসংখ্যা cn = src.channels(), cn1 = cn;
        Auto_বাফার<দ্বিগুণ> buf(cn);
        যদি( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalar_To_Raw_তথ্য(value, buf.তথ্য(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                শীর্ষ_2, বাম, (পূর্ণসংখ্যা)src.elem_আকার(), (uchar*)buf.তথ্য() );
    }
}
/* End of file. */
