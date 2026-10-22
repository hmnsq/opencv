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

template <typename T> স্থির অন্তঃস্থ
শূন্য scalarToRawData_(ধ্রুব Scalar& s, T * ধ্রুব buf, ধ্রুব পূর্ণাঙ্ক cn, ধ্রুব পূর্ণাঙ্ক unroll_to)
{
    পূর্ণাঙ্ক i = 0;
    প্রতি(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    প্রতি(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

শূন্য scalarToRawData(ধ্রুব Scalar& s, শূন্য* _buf, পূর্ণাঙ্ক type, পূর্ণাঙ্ক unroll_to)
{
    CV_INSTRUMENT_REGION();

    ধ্রুব পূর্ণাঙ্ক depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    বিকল্প(depth)
    {
    পক্ষ CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_Bool:
        scalarToRawData_(s, (তার্কিক*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_32S:
        scalarToRawData_(s, (পূর্ণাঙ্ক*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_32F:
        scalarToRawData_(s, (ভিন্ন*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        ভঙ্গ;
    পক্ষ CV_64F:
        scalarToRawData_(s, (দ্বিগুণ*)_buf, cn, unroll_to);
        ভঙ্গ;
    স্বভাব:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

শূন্য convertAndUnrollScalar( ধ্রুব Mat& sc, পূর্ণাঙ্ক buftype, uchar* scbuf, size_t blocksize )
{
    পূর্ণাঙ্ক scn = (পূর্ণাঙ্ক)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    দ্বিমিক_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    যদি( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        প্রতি( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    প্রতি( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> স্থির শূন্য
copyMask_(ধ্রুব uchar* _src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    প্রতি( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুব T* src = (ধ্রুব T*)_src;
        T* dst = (T*)_dst;
        পূর্ণাঙ্ক x = 0;
         #যদি CV_ENABLE_UNROLLED
        প্রতি( ; x <= size.width - 4; x += 4 )
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
        প্রতি( ; x < size.width; x++ )
            যদি( mask[x] )
                dst[x] = src[x];
    }
}

template<> শূন্য
copyMask_<uchar>(ধ্রুব uchar* _src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (পূর্ণাঙ্ক)sstep, _dst, (পূর্ণাঙ্ক)dstep, ippiSize(size), mask, (পূর্ণাঙ্ক)mstep) >= 0)

    প্রতি( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুব uchar* src = (ধ্রুব uchar*)_src;
        uchar* dst = (uchar*)_dst;
        পূর্ণাঙ্ক x = 0;
        #যদি (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            প্রতি( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        প্রতি( ; x < size.width; x++ )
            যদি( mask[x] )
                dst[x] = src[x];
    }
}

template<> শূন্য
copyMask_<Vec3b>(ধ্রুব uchar* _src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    প্রতি( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুব uchar* src = (ধ্রুব uchar*)_src;
        uchar* dst = (uchar*)_dst;
        পূর্ণাঙ্ক x = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
        প্রতি( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        প্রতি( ; x < size.width; x++ )
            যদি( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> শূন্য
copyMask_<ushort>(ধ্রুব uchar* _src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (ধ্রুব Ipp16u *)_src, (পূর্ণাঙ্ক)sstep, (Ipp16u *)_dst, (পূর্ণাঙ্ক)dstep, ippiSize(size), mask, (পূর্ণাঙ্ক)mstep) >= 0)

    প্রতি( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুব ushort* src = (ধ্রুব ushort*)_src;
        ushort* dst = (ushort*)_dst;
        পূর্ণাঙ্ক x = 0;
        #যদি (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            প্রতি( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        প্রতি( ; x < size.width; x++ )
            যদি( mask[x] )
                dst[x] = src[x];
    }
}

template<> শূন্য
copyMask_<Vec3s>(ধ্রুব uchar* _src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    প্রতি( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুব ushort* src = (ধ্রুব ushort*)_src;
        ushort* dst = (ushort*)_dst;
        পূর্ণাঙ্ক x = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
        প্রতি( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        প্রতি( ; x < size.width; x++ )
            যদি( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> শূন্য
copyMask_<পূর্ণাঙ্ক>(ধ্রুব uchar* _src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    প্রতি( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুব পূর্ণাঙ্ক* src = (ধ্রুব পূর্ণাঙ্ক*)_src;
        পূর্ণাঙ্ক* dst = (পূর্ণাঙ্ক*)_dst;
        পূর্ণাঙ্ক x = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
        প্রতি (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        প্রতি (; x < size.width; x++)
            যদি ( mask[x] )
                dst[x] = src[x];
    }
}

স্থির শূন্য
copyMaskGeneric(ধ্রুব uchar* _src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, শূন্য* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    প্রতি( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        ধ্রুব uchar* src = _src;
        uchar* dst = _dst;
        পূর্ণাঙ্ক x = 0;
        প্রতি( ; x < size.width; x++, src += esz, dst += esz )
        {
            যদি( !mask[x] )
                অনুবর্ত;
            প্রতি( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
স্থির শূন্য copyMask##suffix(ধ্রুব uchar* src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, শূন্য*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#যদি defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
স্থির শূন্য copyMask##suffix(ধ্রুব uchar* src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, শূন্য*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (ধ্রুব ipptype *)src, (পূর্ণাঙ্ক)sstep, (ipptype *)dst, (পূর্ণাঙ্ক)dstep, ippiSize(size), (ধ্রুব Ipp8u *)mask, (পূর্ণাঙ্ক)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#অন্যথা
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
স্থির শূন্য copyMask##suffix(ধ্রুব uchar* src, size_t sstep, ধ্রুব uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, শূন্য*) \
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
DEF_COPY_MASK_F(32s, পূর্ণাঙ্ক, 32s_C1MR, Ipp32s)
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
    প্রত্যাগম esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
শূন্য Mat::copyTo( OutputArray _dst ) ধ্রুব
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    যদি (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*এতত্);
        প্রত্যাগম;
    }
#endif

    পূর্ণাঙ্ক stype = type();
    পূর্ণাঙ্ক dtype = _dst.type();
    যদি( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        প্রত্যাগম;
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
        প্রত্যাগম;
    }

    তার্কিক allowTransposed = dims == 1 ||
        _dst.kind() == প্রবেশ_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    যদি( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        যদি (empty())
            প্রত্যাগম;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != রিক্ত);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        প্রতি( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        পূর্ণাঙ্ক lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        প্রত্যাগম;
    }

    যদি( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        যদি( data == dst.data )
            প্রত্যাগম;

        যদি( rows > 0 && cols > 0 )
        {
            Mat src = *এতত্;
            Size sz = getContinuousSize2D(src, dst, (পূর্ণাঙ্ক)elemSize());
            CV_CheckGE(sz.width, 0, "");

            ধ্রুব uchar* sptr = src.data;
            uchar* dptr = dst.data;

#যদি IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (পূর্ণাঙ্ক)src.step, dptr, (পূর্ণাঙ্ক)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            প্রতি (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        প্রত্যাগম;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    যদি( data == dst.data )
        প্রত্যাগম;

    যদি( total() != 0 )
    {
        ধ্রুব Mat* arrays[] = { এতত্, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
স্থির তার্কিক ipp_copyTo(ধ্রুব Mat &src, Mat &dst, ধ্রুব Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    যদি(mask.channels() > 1 || mask.depth() != CV_8U)
        প্রত্যাগম অসত্য;

    যদি (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        প্রত্যাগম CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (পূর্ণাঙ্ক)src.step, dst.ptr(), (পূর্ণাঙ্ক)dst.step, size, (পূর্ণাঙ্ক)src.elemSize1(), src.channels(), mask.ptr(), (পূর্ণাঙ্ক)mask.step) >= 0;
    }
    অন্যথা
    {
        ধ্রুব Mat      *arrays[] = {&src, &dst, &mask, রিক্ত};
        uchar          *ptrs[3]  = {রিক্ত};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        প্রতি (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (পূর্ণাঙ্ক)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                প্রত্যাগম অসত্য;
        }
        প্রত্যাগম সত্য;
    }
#অন্যথা
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    প্রত্যাগম অসত্য;
#endif
}
#endif

শূন্য Mat::copyTo( OutputArray _dst, প্রবেশ_Array _mask ) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    যদি( !mask.data )
    {
        copyTo(_dst);
        প্রত্যাগম;
    }

    পূর্ণাঙ্ক cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    তার্কিক colorMask = mcn > 1;
    যদি( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        যদি (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*এতত্, dst, mask))
    যদি ( এতত্->dims <= 2 ) {
        যদি ( এতত্->size() == dst.size() && এতত্->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, এতত্->data, এতত্->step, dst.data, dst.step, এতত্->cols, এতত্->rows, এতত্->type(), mask.data, mask.step, mask.type());
        }
    }
    অন্যথা যদি ( এতত্->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = এতত্->total();
        যদি (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, এতত্->data, 0, dst.data, 0, (পূর্ণাঙ্ক)sz, 1, এতত্->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    দ্বিমিক_Func copymask = getCopyMaskFunc(esz);

    যদি( dims <= 2 )
    {
        Mat src = *এতত্;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        প্রত্যাগম;
    }

    ধ্রুব Mat* arrays[] = { এতত্, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((পূর্ণাঙ্ক)(it.size*mcn), 1);

    প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
শূন্য Mat::copyAt( OutputArray _dst ) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
শূন্য Mat::copyAt( OutputArray _dst, প্রবেশ_Array _mask ) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


স্থির তার্কিক can_apply_memset(ধ্রুব Mat &mat, ধ্রুব Scalar &s, পূর্ণাঙ্ক &fill_মান)
{
    // check if depth is 1 byte.
    বিকল্প (mat.depth())
    {
    পক্ষ CV_8U: fill_মান = saturate_cast<uchar>( s.val[0] ); ভঙ্গ;
    পক্ষ CV_8S: fill_মান = saturate_cast<schar>( s.val[0] ); ভঙ্গ;
    স্বভাব: প্রত্যাগম অসত্য;
    }

    // check if all element is same.
    ধ্রুব int64* is = (ধ্রুব int64*)&s.val[0];
    বিকল্প (mat.channels())
    {
    পক্ষ 1: প্রত্যাগম সত্য;
    পক্ষ 2: প্রত্যাগম (is[0] == is[1]);
    পক্ষ 3: প্রত্যাগম (is[0] == is[1] && is[1] == is[2]);
    পক্ষ 4: প্রত্যাগম (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    স্বভাব: প্রত্যাগম অসত্য;
    }
}

Mat& Mat::operator = (ধ্রুব Scalar& s)
{
    CV_INSTRUMENT_REGION();

    যদি (এতত্->empty())
        প্রত্যাগম *এতত্;

    ধ্রুব Mat* arrays[] = { এতত্ };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    ধ্রুব int64* is = (ধ্রুব int64*)&s.val[0];

    যদি( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    অন্যথা
    {
        পূর্ণাঙ্ক fill_মান = 0;
        যদি ( can_apply_memset(*এতত্, s, fill_মান) )
        {
            প্রতি (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_মান, elsize);
            প্রত্যাগম *এতত্;
        }

        যদি( it.nplanes > 0 )
        {
            দ্বিগুণ scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            প্রতি( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= পরিমাণ(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        প্রতি( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    প্রত্যাগম *এতত্;
}

#ifdef HAVE_IPP
স্থির তার্কিক ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    যদি(mask.empty())
        প্রত্যাগম অসত্য;

    যদি(mask.depth() != CV_8U || mask.channels() > 1)
        প্রত্যাগম অসত্য;

    যদি(dst.channels() > 4)
        প্রত্যাগম অসত্য;

    যদি (dst.depth() == CV_32F)
    {
        প্রতি (পূর্ণাঙ্ক i = 0; i < (পূর্ণাঙ্ক)(_val.total()); i++)
        {
            ভিন্ন v = (ভিন্ন)(_val.at<দ্বিগুণ>(i));  // cast to float
            যদি (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                প্রত্যাগম অসত্য;
        }
    }

    যদি(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_মান_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        প্রত্যাগম CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (পূর্ণাঙ্ক)dst.step, size, dataType, dst.channels(), mask.ptr(), (পূর্ণাঙ্ক)mask.step) >= 0;
    }
    অন্যথা
    {
        ধ্রুব Mat      *arrays[] = {&dst, mask.empty()?রিক্ত:&mask, রিক্ত};
        uchar          *ptrs[2]  = {রিক্ত};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(পূর্ণাঙ্ক)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_মান_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                প্রত্যাগম অসত্য;
        }
        প্রত্যাগম সত্য;
    }
#অন্যথা
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    প্রত্যাগম অসত্য;
#endif
}
#endif

Mat& Mat::setTo(প্রবেশ_Array মান, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    যদি( empty() )
        প্রত্যাগম *এতত্;

    Mat value = মান.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), মান.kind(), প্রবেশ_Array_2::MAT ));
    পূর্ণাঙ্ক cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*এতত্, value, mask), *এতত্)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    দ্বিমিক_Func copymask = getCopyMaskFunc(esz);

    ধ্রুব Mat* arrays[] = { এতত্, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক totalsz = (পূর্ণাঙ্ক)it.size*mcn;
    পূর্ণাঙ্ক blockSize0 = std::min(totalsz, (পূর্ণাঙ্ক)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (পূর্ণাঙ্ক)পরিমাণ(দ্বিগুণ));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        প্রতি( পূর্ণাঙ্ক j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            যদি( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            অন্যথা
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    প্রত্যাগম *এতত্;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    যদি( empty() )
        প্রত্যাগম *এতত্;

    size_t esz = elemSize();

    ধ্রুব Mat* arrays[] = { এতত্, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    প্রত্যাগম *এতত্;
}


#যদি defined HAVE_OPENCL && !defined __APPLE__

স্থির তার্কিক ocl_repeat(প্রবেশ_Array _src, পূর্ণাঙ্ক ny, পূর্ণাঙ্ক nx, OutputArray _dst)
{
    যদি (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        প্রত্যাগম সত্য;
    }

    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    প্রত্যাগম k.run(2, globalsize, রিক্ত, অসত্য);
}

#endif

শূন্য repeat(প্রবেশ_Array _src, পূর্ণাঙ্ক ny, পূর্ণাঙ্ক nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    যদি( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        প্রত্যাগম;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#যদি !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    পূর্ণাঙ্ক esz = (পূর্ণাঙ্ক)src.elemSize();
    পূর্ণাঙ্ক x, y;
    ssize.width *= esz; dsize.width *= esz;

    প্রতি( y = 0; y < ssize.height; y++ )
    {
        প্রতি( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    প্রতি( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(ধ্রুব Mat& src, পূর্ণাঙ্ক ny, পূর্ণাঙ্ক nx)
{
    যদি( nx == 1 && ny == 1 )
        প্রত্যাগম src;
    Mat dst;
    repeat(src, ny, nx, dst);
    প্রত্যাগম dst;
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
পূর্ণাঙ্ক cv::borderInterpolate( পূর্ণাঙ্ক p, পূর্ণাঙ্ক len, পূর্ণাঙ্ক borderType )
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
        পূর্ণাঙ্ক delta = borderType == BORDER_REFLECT_101;
        যদি( len == 1 )
            প্রত্যাগম 0;
        কুরু
        {
            যদি( p < 0 )
                p = -p - 1 + delta;
            অন্যথা
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        যাবত্(p < 0 || p >= len);
#অন্যথা
        যাবত্( (unsigned)p >= (unsigned)len );
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
            p = (পূর্ণাঙ্ক)p64;
        }
    }
    অন্যথা যদি( borderType == BORDER_CONSTANT )
        p = -1;
    অন্যথা
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    প্রত্যাগম p;
}

নামাকাশ
{

শূন্য copyMakeBorder_8u( ধ্রুব uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        পূর্ণাঙ্ক শীর্ষ_2, পূর্ণাঙ্ক বাম, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক borderType )
{
    ধ্রুব পূর্ণাঙ্ক isz = (পূর্ণাঙ্ক)পরিমাণ(পূর্ণাঙ্ক);
    পূর্ণাঙ্ক i, j, k, elemSize = 1;
    তার্কিক intMode = অসত্য;

    যদি( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = সত্য;
    }

    cv::AutoBuffer<পূর্ণাঙ্ক> _tab((dstroi.width - srcroi.width)*cn);
    পূর্ণাঙ্ক* tab = _tab.data();
    পূর্ণাঙ্ক ডান = dstroi.width - srcroi.width - বাম;
    পূর্ণাঙ্ক bottom = dstroi.height - srcroi.height - শীর্ষ_2;

    প্রতি( i = 0; i < বাম; i++ )
    {
        j = cv::borderInterpolate(i - বাম, srcroi.width, borderType)*cn;
        প্রতি( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    প্রতি( i = 0; i < ডান; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        প্রতি( k = 0; k < cn; k++ )
            tab[(i+বাম)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    বাম *= cn;
    ডান *= cn;

    uchar* dstInner = dst + dststep*শীর্ষ_2 + বাম*elemSize;

    প্রতি( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        যদি( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        যদি( intMode )
        {
            ধ্রুব পূর্ণাঙ্ক* isrc = (পূর্ণাঙ্ক*)src;
            পূর্ণাঙ্ক* idstInner = (পূর্ণাঙ্ক*)dstInner;
            প্রতি( j = 0; j < বাম; j++ )
                idstInner[j - বাম] = isrc[tab[j]];
            প্রতি( j = 0; j < ডান; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + বাম]];
        }
        অন্যথা
        {
            প্রতি( j = 0; j < বাম; j++ )
                dstInner[j - বাম] = src[tab[j]];
            প্রতি( j = 0; j < ডান; j++ )
                dstInner[j + srcroi.width] = src[tab[j + বাম]];
        }
    }

    dstroi.width *= elemSize;

    প্রতি( i = 0; i < শীর্ষ_2; i++ )
    {
        j = cv::borderInterpolate(i - শীর্ষ_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (শীর্ষ_2+j)*dststep, dstroi.width);
    }

    dst += dststep*শীর্ষ_2;
    প্রতি( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


শূন্য copyMakeConstBorder_8u( ধ্রুব uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             পূর্ণাঙ্ক শীর্ষ_2, পূর্ণাঙ্ক বাম, পূর্ণাঙ্ক cn, ধ্রুব uchar* value )
{
    পূর্ণাঙ্ক i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    পূর্ণাঙ্ক ডান = dstroi.width - srcroi.width - বাম;
    পূর্ণাঙ্ক bottom = dstroi.height - srcroi.height - শীর্ষ_2;

    প্রতি( i = 0; i < dstroi.width; i++ )
    {
        প্রতি( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    বাম *= cn;
    ডান *= cn;

    uchar* dstInner = dst + dststep*শীর্ষ_2 + বাম;

    প্রতি( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        যদি( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - বাম, constBuf, বাম );
        memcpy( dstInner + srcroi.width, constBuf, ডান );
    }

    প্রতি( i = 0; i < শীর্ষ_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (শীর্ষ_2 + srcroi.height) * dststep;
    প্রতি( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির তার্কিক ocl_copyMakeBorder( প্রবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক শীর্ষ_2, পূর্ণাঙ্ক bottom,
                                পূর্ণাঙ্ক বাম, পূর্ণাঙ্ক ডান, পূর্ণাঙ্ক borderType, ধ্রুব Scalar& value )
{
    পূর্ণাঙ্ক type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    তার্কিক isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    যদি ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        প্রত্যাগম অসত্য;

    ধ্রুব বর্ণ * ধ্রুব borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    পূর্ণাঙ্ক scalarcn = cn == 3 ? 4 : cn;
    পূর্ণাঙ্ক sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    UMat src = _src.getUMat();
    যদি( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        পূর্ণাঙ্ক dtop = std::min(ofs.y, শীর্ষ_2);
        পূর্ণাঙ্ক dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        পূর্ণাঙ্ক dleft = std::min(ofs.x, বাম);
        পূর্ণাঙ্ক dright = std::min(wholeSize.width - src.cols - ofs.x, ডান);
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
        প্রত্যাগম সত্য;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           শীর্ষ_2, বাম, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    প্রত্যাগম k.run(2, globalsize, রিক্ত, অসত্য);
}

}
#endif

#ifdef HAVE_IPP
নামাকাশ cv {

স্থির তার্কিক ipp_copyMakeBorder( Mat &_src, Mat &_dst, পূর্ণাঙ্ক শীর্ষ_2, পূর্ণাঙ্ক bottom,
                                পূর্ণাঙ্ক বাম, পূর্ণাঙ্ক ডান, পূর্ণাঙ্ক _borderType, ধ্রুব Scalar& value )
{
#যদি defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(বাম, শীর্ষ_2, ডান, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    যদি((পূর্ণাঙ্ক)borderType == -1)
        প্রত্যাগম অসত্য;

    যদি(_src.dims > 2)
        প্রত্যাগম অসত্য;

    Rect dstRect(borderSize.বাম, borderSize.শীর্ষ_2,
        _dst.cols - borderSize.ডান - borderSize.বাম,
        _dst.rows - borderSize.bottom - borderSize.শীর্ষ_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    প্রত্যাগম CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#অন্যথা
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(শীর্ষ_2); CV_UNUSED(bottom); CV_UNUSED(বাম); CV_UNUSED(ডান);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    প্রত্যাগম অসত্য;
#endif
}
}
#endif

শূন্য cv::copyMakeBorder( প্রবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক শীর্ষ_2, পূর্ণাঙ্ক bottom,
                         পূর্ণাঙ্ক বাম, পূর্ণাঙ্ক ডান, পূর্ণাঙ্ক borderType, ধ্রুব Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( শীর্ষ_2 >= 0 && bottom >= 0 && বাম >= 0 && ডান >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + শীর্ষ_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + বাম + ডান <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, শীর্ষ_2, bottom, বাম, ডান, borderType, value))
    পূর্ণাঙ্ক type = src.type();

    যদি( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        পূর্ণাঙ্ক dtop = std::min(ofs.y, শীর্ষ_2);
        পূর্ণাঙ্ক dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        পূর্ণাঙ্ক dleft = std::min(ofs.x, বাম);
        পূর্ণাঙ্ক dright = std::min(wholeSize.width - src.cols - ofs.x, ডান);
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
        যদি(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        প্রত্যাগম;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, শীর্ষ_2, bottom, বাম, ডান, borderType, value))

    যদি( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           শীর্ষ_2, বাম, (পূর্ণাঙ্ক)src.elemSize(), borderType );
    অন্যথা
    {
        পূর্ণাঙ্ক cn = src.channels(), cn1 = cn;
        AutoBuffer<দ্বিগুণ> buf(cn);
        যদি( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                শীর্ষ_2, বাম, (পূর্ণাঙ্ক)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
