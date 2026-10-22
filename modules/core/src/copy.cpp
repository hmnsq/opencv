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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"

നാമാകാശ cv
{

template <typename T> സ്ഥിതം വരിയിൽ
ശൂന്യം scalarToRawData_(സ്ഥിരം Scalar& s, T * സ്ഥിരം buf, സ്ഥിരം പൂർണ്ണസംഖ്യ cn, സ്ഥിരം പൂർണ്ണസംഖ്യ unroll_to)
{
    പൂർണ്ണസംഖ്യ i = 0;
    വേണ്ടി(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    വേണ്ടി(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

ശൂന്യം scalarToRawData(സ്ഥിരം Scalar& s, ശൂന്യം* _buf, പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ unroll_to)
{
    CV_INSTRUMENT_REGION();

    സ്ഥിരം പൂർണ്ണസംഖ്യ depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    തിരഞ്ഞെടുപ്പ്(depth)
    {
    അവസ്ഥ CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_Bool:
        scalarToRawData_(s, (തർക്കം*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_32S:
        scalarToRawData_(s, (പൂർണ്ണസംഖ്യ*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_32F:
        scalarToRawData_(s, (ദശാംശം*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        നിർത്തുക;
    അവസ്ഥ CV_64F:
        scalarToRawData_(s, (ഇരട്ട*)_buf, cn, unroll_to);
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

ശൂന്യം convertAndUnrollScalar( സ്ഥിരം Mat& sc, പൂർണ്ണസംഖ്യ buftype, uchar* scbuf, size_t blocksize )
{
    പൂർണ്ണസംഖ്യ scn = (പൂർണ്ണസംഖ്യ)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    ദ്വിമാന_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    എങ്കിൽ( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        വേണ്ടി( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    വേണ്ടി( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> സ്ഥിതം ശൂന്യം
copyMask_(സ്ഥിരം uchar* _src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    വേണ്ടി( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        സ്ഥിരം T* src = (സ്ഥിരം T*)_src;
        T* dst = (T*)_dst;
        പൂർണ്ണസംഖ്യ x = 0;
         #എങ്കിൽ CV_ENABLE_UNROLLED
        വേണ്ടി( ; x <= size.width - 4; x += 4 )
        {
            എങ്കിൽ( mask[x] )
                dst[x] = src[x];
            എങ്കിൽ( mask[x+1] )
                dst[x+1] = src[x+1];
            എങ്കിൽ( mask[x+2] )
                dst[x+2] = src[x+2];
            എങ്കിൽ( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        വേണ്ടി( ; x < size.width; x++ )
            എങ്കിൽ( mask[x] )
                dst[x] = src[x];
    }
}

template<> ശൂന്യം
copyMask_<uchar>(സ്ഥിരം uchar* _src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (പൂർണ്ണസംഖ്യ)sstep, _dst, (പൂർണ്ണസംഖ്യ)dstep, ippiSize(size), mask, (പൂർണ്ണസംഖ്യ)mstep) >= 0)

    വേണ്ടി( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        സ്ഥിരം uchar* src = (സ്ഥിരം uchar*)_src;
        uchar* dst = (uchar*)_dst;
        പൂർണ്ണസംഖ്യ x = 0;
        #എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            വേണ്ടി( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        വേണ്ടി( ; x < size.width; x++ )
            എങ്കിൽ( mask[x] )
                dst[x] = src[x];
    }
}

template<> ശൂന്യം
copyMask_<Vec3b>(സ്ഥിരം uchar* _src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    വേണ്ടി( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        സ്ഥിരം uchar* src = (സ്ഥിരം uchar*)_src;
        uchar* dst = (uchar*)_dst;
        പൂർണ്ണസംഖ്യ x = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        വേണ്ടി( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        വേണ്ടി( ; x < size.width; x++ )
            എങ്കിൽ( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> ശൂന്യം
copyMask_<ushort>(സ്ഥിരം uchar* _src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (സ്ഥിരം Ipp16u *)_src, (പൂർണ്ണസംഖ്യ)sstep, (Ipp16u *)_dst, (പൂർണ്ണസംഖ്യ)dstep, ippiSize(size), mask, (പൂർണ്ണസംഖ്യ)mstep) >= 0)

    വേണ്ടി( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        സ്ഥിരം ushort* src = (സ്ഥിരം ushort*)_src;
        ushort* dst = (ushort*)_dst;
        പൂർണ്ണസംഖ്യ x = 0;
        #എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            വേണ്ടി( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        വേണ്ടി( ; x < size.width; x++ )
            എങ്കിൽ( mask[x] )
                dst[x] = src[x];
    }
}

template<> ശൂന്യം
copyMask_<Vec3s>(സ്ഥിരം uchar* _src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    വേണ്ടി( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        സ്ഥിരം ushort* src = (സ്ഥിരം ushort*)_src;
        ushort* dst = (ushort*)_dst;
        പൂർണ്ണസംഖ്യ x = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        വേണ്ടി( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        വേണ്ടി( ; x < size.width; x++ )
            എങ്കിൽ( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> ശൂന്യം
copyMask_<പൂർണ്ണസംഖ്യ>(സ്ഥിരം uchar* _src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    വേണ്ടി( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ* src = (സ്ഥിരം പൂർണ്ണസംഖ്യ*)_src;
        പൂർണ്ണസംഖ്യ* dst = (പൂർണ്ണസംഖ്യ*)_dst;
        പൂർണ്ണസംഖ്യ x = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        വേണ്ടി (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        വേണ്ടി (; x < size.width; x++)
            എങ്കിൽ ( mask[x] )
                dst[x] = src[x];
    }
}

സ്ഥിതം ശൂന്യം
copyMaskGeneric(സ്ഥിരം uchar* _src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, ശൂന്യം* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    വേണ്ടി( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        സ്ഥിരം uchar* src = _src;
        uchar* dst = _dst;
        പൂർണ്ണസംഖ്യ x = 0;
        വേണ്ടി( ; x < size.width; x++, src += esz, dst += esz )
        {
            എങ്കിൽ( !mask[x] )
                തുടരുക;
            വേണ്ടി( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
സ്ഥിതം ശൂന്യം copyMask##suffix(സ്ഥിരം uchar* src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ശൂന്യം*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#എങ്കിൽ defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
സ്ഥിതം ശൂന്യം copyMask##suffix(സ്ഥിരം uchar* src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ശൂന്യം*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (സ്ഥിരം ipptype *)src, (പൂർണ്ണസംഖ്യ)sstep, (ipptype *)dst, (പൂർണ്ണസംഖ്യ)dstep, ippiSize(size), (സ്ഥിരം Ipp8u *)mask, (പൂർണ്ണസംഖ്യ)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#അല്ലെങ്കിൽ
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
സ്ഥിതം ശൂന്യം copyMask##suffix(സ്ഥിരം uchar* src, size_t sstep, സ്ഥിരം uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, ശൂന്യം*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#എങ്കിൽ IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#അല്ലെങ്കിൽ
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, പൂർണ്ണസംഖ്യ, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

ദ്വിമാന_Func copyMaskTab[] =
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

ദ്വിമാന_Func getCopyMaskFunc(size_t esz)
{
    തിരികെ esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
ശൂന്യം Mat::copyTo( OutputArray _dst ) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    എങ്കിൽ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ഇത്);
        തിരികെ;
    }
#endif

    പൂർണ്ണസംഖ്യ stype = type();
    പൂർണ്ണസംഖ്യ dtype = _dst.type();
    എങ്കിൽ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        തിരികെ;
    }

    എങ്കിൽ( dims == 0 && empty() )
    {
        _dst.release();
        ശൂന്യം* obj = _dst.getObj();
        എങ്കിൽ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        അല്ലെങ്കിൽ എങ്കിൽ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        അല്ലെങ്കിൽ എങ്കിൽ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        തിരികെ;
    }

    തർക്കം allowTransposed = dims == 1 ||
        _dst.kind() == നിവേശം_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    എങ്കിൽ( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        എങ്കിൽ (empty())
            തിരികെ;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != ശൂന്യ);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        വേണ്ടി( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        പൂർണ്ണസംഖ്യ lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        തിരികെ;
    }

    എങ്കിൽ( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        എങ്കിൽ( data == dst.data )
            തിരികെ;

        എങ്കിൽ( rows > 0 && cols > 0 )
        {
            Mat src = *ഇത്;
            Size sz = getContinuousSize2D(src, dst, (പൂർണ്ണസംഖ്യ)elemSize());
            CV_CheckGE(sz.width, 0, "");

            സ്ഥിരം uchar* sptr = src.data;
            uchar* dptr = dst.data;

#എങ്കിൽ IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (പൂർണ്ണസംഖ്യ)src.step, dptr, (പൂർണ്ണസംഖ്യ)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            വേണ്ടി (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        തിരികെ;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    എങ്കിൽ( data == dst.data )
        തിരികെ;

    എങ്കിൽ( total() != 0 )
    {
        സ്ഥിരം Mat* arrays[] = { ഇത്, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
സ്ഥിതം തർക്കം ipp_copyTo(സ്ഥിരം Mat &src, Mat &dst, സ്ഥിരം Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    എങ്കിൽ(mask.channels() > 1 || mask.depth() != CV_8U)
        തിരികെ അസത്യം;

    എങ്കിൽ (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        തിരികെ CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (പൂർണ്ണസംഖ്യ)src.step, dst.ptr(), (പൂർണ്ണസംഖ്യ)dst.step, size, (പൂർണ്ണസംഖ്യ)src.elemSize1(), src.channels(), mask.ptr(), (പൂർണ്ണസംഖ്യ)mask.step) >= 0;
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം Mat      *arrays[] = {&src, &dst, &mask, ശൂന്യ};
        uchar          *ptrs[3]  = {ശൂന്യ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        വേണ്ടി (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (പൂർണ്ണസംഖ്യ)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                തിരികെ അസത്യം;
        }
        തിരികെ സത്യം;
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    തിരികെ അസത്യം;
#endif
}
#endif

ശൂന്യം Mat::copyTo( OutputArray _dst, നിവേശം_Array _mask ) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    എങ്കിൽ( !mask.data )
    {
        copyTo(_dst);
        തിരികെ;
    }

    പൂർണ്ണസംഖ്യ cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    തർക്കം colorMask = mcn > 1;
    എങ്കിൽ( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        എങ്കിൽ (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*ഇത്, dst, mask))
    എങ്കിൽ ( ഇത്->dims <= 2 ) {
        എങ്കിൽ ( ഇത്->size() == dst.size() && ഇത്->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, ഇത്->data, ഇത്->step, dst.data, dst.step, ഇത്->cols, ഇത്->rows, ഇത്->type(), mask.data, mask.step, mask.type());
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ ( ഇത്->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = ഇത്->total();
        എങ്കിൽ (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, ഇത്->data, 0, dst.data, 0, (പൂർണ്ണസംഖ്യ)sz, 1, ഇത്->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    ദ്വിമാന_Func copymask = getCopyMaskFunc(esz);

    എങ്കിൽ( dims <= 2 )
    {
        Mat src = *ഇത്;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        തിരികെ;
    }

    സ്ഥിരം Mat* arrays[] = { ഇത്, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((പൂർണ്ണസംഖ്യ)(it.size*mcn), 1);

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
ശൂന്യം Mat::copyAt( OutputArray _dst ) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
ശൂന്യം Mat::copyAt( OutputArray _dst, നിവേശം_Array _mask ) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


സ്ഥിതം തർക്കം can_apply_memset(സ്ഥിരം Mat &mat, സ്ഥിരം Scalar &s, പൂർണ്ണസംഖ്യ &fill_മൂല്യം)
{
    // check if depth is 1 byte.
    തിരഞ്ഞെടുപ്പ് (mat.depth())
    {
    അവസ്ഥ CV_8U: fill_മൂല്യം = saturate_cast<uchar>( s.val[0] ); നിർത്തുക;
    അവസ്ഥ CV_8S: fill_മൂല്യം = saturate_cast<schar>( s.val[0] ); നിർത്തുക;
    സ്ഥിരസ്ഥിതി: തിരികെ അസത്യം;
    }

    // check if all element is same.
    സ്ഥിരം int64* is = (സ്ഥിരം int64*)&s.val[0];
    തിരഞ്ഞെടുപ്പ് (mat.channels())
    {
    അവസ്ഥ 1: തിരികെ സത്യം;
    അവസ്ഥ 2: തിരികെ (is[0] == is[1]);
    അവസ്ഥ 3: തിരികെ (is[0] == is[1] && is[1] == is[2]);
    അവസ്ഥ 4: തിരികെ (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    സ്ഥിരസ്ഥിതി: തിരികെ അസത്യം;
    }
}

Mat& Mat::operator = (സ്ഥിരം Scalar& s)
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ (ഇത്->empty())
        തിരികെ *ഇത്;

    സ്ഥിരം Mat* arrays[] = { ഇത് };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    സ്ഥിരം int64* is = (സ്ഥിരം int64*)&s.val[0];

    എങ്കിൽ( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ fill_മൂല്യം = 0;
        എങ്കിൽ ( can_apply_memset(*ഇത്, s, fill_മൂല്യം) )
        {
            വേണ്ടി (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_മൂല്യം, elsize);
            തിരികെ *ഇത്;
        }

        എങ്കിൽ( it.nplanes > 0 )
        {
            ഇരട്ട scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            വേണ്ടി( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= വലുപ്പം(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        വേണ്ടി( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    തിരികെ *ഇത്;
}

#ifdef HAVE_IPP
സ്ഥിതം തർക്കം ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    എങ്കിൽ(mask.empty())
        തിരികെ അസത്യം;

    എങ്കിൽ(mask.depth() != CV_8U || mask.channels() > 1)
        തിരികെ അസത്യം;

    എങ്കിൽ(dst.channels() > 4)
        തിരികെ അസത്യം;

    എങ്കിൽ (dst.depth() == CV_32F)
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)(_val.total()); i++)
        {
            ദശാംശം v = (ദശാംശം)(_val.at<ഇരട്ട>(i));  // cast to float
            എങ്കിൽ (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                തിരികെ അസത്യം;
        }
    }

    എങ്കിൽ(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_മൂല്യം_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        തിരികെ CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (പൂർണ്ണസംഖ്യ)dst.step, size, dataType, dst.channels(), mask.ptr(), (പൂർണ്ണസംഖ്യ)mask.step) >= 0;
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം Mat      *arrays[] = {&dst, mask.empty()?ശൂന്യ:&mask, ശൂന്യ};
        uchar          *ptrs[2]  = {ശൂന്യ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(പൂർണ്ണസംഖ്യ)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_മൂല്യം_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                തിരികെ അസത്യം;
        }
        തിരികെ സത്യം;
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    തിരികെ അസത്യം;
#endif
}
#endif

Mat& Mat::setTo(നിവേശം_Array മൂല്യം, നിവേശം_Array _mask)
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( empty() )
        തിരികെ *ഇത്;

    Mat value = മൂല്യം.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), മൂല്യം.kind(), നിവേശം_Array_2::MAT ));
    പൂർണ്ണസംഖ്യ cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*ഇത്, value, mask), *ഇത്)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    ദ്വിമാന_Func copymask = getCopyMaskFunc(esz);

    സ്ഥിരം Mat* arrays[] = { ഇത്, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ totalsz = (പൂർണ്ണസംഖ്യ)it.size*mcn;
    പൂർണ്ണസംഖ്യ blockSize0 = std::min(totalsz, (പൂർണ്ണസംഖ്യ)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (പൂർണ്ണസംഖ്യ)വലുപ്പം(ഇരട്ട));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            എങ്കിൽ( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            അല്ലെങ്കിൽ
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    തിരികെ *ഇത്;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( empty() )
        തിരികെ *ഇത്;

    size_t esz = elemSize();

    സ്ഥിരം Mat* arrays[] = { ഇത്, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    തിരികെ *ഇത്;
}


#എങ്കിൽ defined HAVE_OPENCL && !defined __APPLE__

സ്ഥിതം തർക്കം ocl_repeat(നിവേശം_Array _src, പൂർണ്ണസംഖ്യ ny, പൂർണ്ണസംഖ്യ nx, OutputArray _dst)
{
    എങ്കിൽ (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        തിരികെ സത്യം;
    }

    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

ശൂന്യം repeat(നിവേശം_Array _src, പൂർണ്ണസംഖ്യ ny, പൂർണ്ണസംഖ്യ nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    എങ്കിൽ( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        തിരികെ;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#എങ്കിൽ !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    പൂർണ്ണസംഖ്യ esz = (പൂർണ്ണസംഖ്യ)src.elemSize();
    പൂർണ്ണസംഖ്യ x, y;
    ssize.width *= esz; dsize.width *= esz;

    വേണ്ടി( y = 0; y < ssize.height; y++ )
    {
        വേണ്ടി( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    വേണ്ടി( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(സ്ഥിരം Mat& src, പൂർണ്ണസംഖ്യ ny, പൂർണ്ണസംഖ്യ nx)
{
    എങ്കിൽ( nx == 1 && ny == 1 )
        തിരികെ src;
    Mat dst;
    repeat(src, ny, nx, dst);
    തിരികെ dst;
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
പൂർണ്ണസംഖ്യ cv::borderInterpolate( പൂർണ്ണസംഖ്യ p, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    എങ്കിൽ(p >= 0 && p < len)
#അല്ലെങ്കിൽ
    എങ്കിൽ( (unsigned)p < (unsigned)len )
#endif
        ;
    അല്ലെങ്കിൽ എങ്കിൽ( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    അല്ലെങ്കിൽ എങ്കിൽ( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        പൂർണ്ണസംഖ്യ delta = borderType == BORDER_REFLECT_101;
        എങ്കിൽ( len == 1 )
            തിരികെ 0;
        ചെയ്യുക
        {
            എങ്കിൽ( p < 0 )
                p = -p - 1 + delta;
            അല്ലെങ്കിൽ
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        ആകുമ്പോൾ(p < 0 || p >= len);
#അല്ലെങ്കിൽ
        ആകുമ്പോൾ( (unsigned)p >= (unsigned)len );
#endif
    }
    അല്ലെങ്കിൽ എങ്കിൽ( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        എങ്കിൽ( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            എങ്കിൽ( p64 < 0 )
                p64 += len;
            p = (പൂർണ്ണസംഖ്യ)p64;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( borderType == BORDER_CONSTANT )
        p = -1;
    അല്ലെങ്കിൽ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    തിരികെ p;
}

നാമാകാശ
{

ശൂന്യം copyMakeBorder_8u( സ്ഥിരം uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        പൂർണ്ണസംഖ്യ മുകൾ_2, പൂർണ്ണസംഖ്യ ഇടത്, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ borderType )
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ isz = (പൂർണ്ണസംഖ്യ)വലുപ്പം(പൂർണ്ണസംഖ്യ);
    പൂർണ്ണസംഖ്യ i, j, k, elemSize = 1;
    തർക്കം intMode = അസത്യം;

    എങ്കിൽ( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = സത്യം;
    }

    cv::AutoBuffer<പൂർണ്ണസംഖ്യ> _tab((dstroi.width - srcroi.width)*cn);
    പൂർണ്ണസംഖ്യ* tab = _tab.data();
    പൂർണ്ണസംഖ്യ വലത് = dstroi.width - srcroi.width - ഇടത്;
    പൂർണ്ണസംഖ്യ bottom = dstroi.height - srcroi.height - മുകൾ_2;

    വേണ്ടി( i = 0; i < ഇടത്; i++ )
    {
        j = cv::borderInterpolate(i - ഇടത്, srcroi.width, borderType)*cn;
        വേണ്ടി( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    വേണ്ടി( i = 0; i < വലത്; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        വേണ്ടി( k = 0; k < cn; k++ )
            tab[(i+ഇടത്)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    ഇടത് *= cn;
    വലത് *= cn;

    uchar* dstInner = dst + dststep*മുകൾ_2 + ഇടത്*elemSize;

    വേണ്ടി( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        എങ്കിൽ( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        എങ്കിൽ( intMode )
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ* isrc = (പൂർണ്ണസംഖ്യ*)src;
            പൂർണ്ണസംഖ്യ* idstInner = (പൂർണ്ണസംഖ്യ*)dstInner;
            വേണ്ടി( j = 0; j < ഇടത്; j++ )
                idstInner[j - ഇടത്] = isrc[tab[j]];
            വേണ്ടി( j = 0; j < വലത്; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + ഇടത്]];
        }
        അല്ലെങ്കിൽ
        {
            വേണ്ടി( j = 0; j < ഇടത്; j++ )
                dstInner[j - ഇടത്] = src[tab[j]];
            വേണ്ടി( j = 0; j < വലത്; j++ )
                dstInner[j + srcroi.width] = src[tab[j + ഇടത്]];
        }
    }

    dstroi.width *= elemSize;

    വേണ്ടി( i = 0; i < മുകൾ_2; i++ )
    {
        j = cv::borderInterpolate(i - മുകൾ_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (മുകൾ_2+j)*dststep, dstroi.width);
    }

    dst += dststep*മുകൾ_2;
    വേണ്ടി( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


ശൂന്യം copyMakeConstBorder_8u( സ്ഥിരം uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             പൂർണ്ണസംഖ്യ മുകൾ_2, പൂർണ്ണസംഖ്യ ഇടത്, പൂർണ്ണസംഖ്യ cn, സ്ഥിരം uchar* value )
{
    പൂർണ്ണസംഖ്യ i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    പൂർണ്ണസംഖ്യ വലത് = dstroi.width - srcroi.width - ഇടത്;
    പൂർണ്ണസംഖ്യ bottom = dstroi.height - srcroi.height - മുകൾ_2;

    വേണ്ടി( i = 0; i < dstroi.width; i++ )
    {
        വേണ്ടി( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    ഇടത് *= cn;
    വലത് *= cn;

    uchar* dstInner = dst + dststep*മുകൾ_2 + ഇടത്;

    വേണ്ടി( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        എങ്കിൽ( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - ഇടത്, constBuf, ഇടത് );
        memcpy( dstInner + srcroi.width, constBuf, വലത് );
    }

    വേണ്ടി( i = 0; i < മുകൾ_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (മുകൾ_2 + srcroi.height) * dststep;
    വേണ്ടി( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

നാമാകാശ cv {

സ്ഥിതം തർക്കം ocl_copyMakeBorder( നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ മുകൾ_2, പൂർണ്ണസംഖ്യ bottom,
                                പൂർണ്ണസംഖ്യ ഇടത്, പൂർണ്ണസംഖ്യ വലത്, പൂർണ്ണസംഖ്യ borderType, സ്ഥിരം Scalar& value )
{
    പൂർണ്ണസംഖ്യ type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    തർക്കം isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    എങ്കിൽ ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        തിരികെ അസത്യം;

    സ്ഥിരം അക്ഷരം * സ്ഥിരം borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    പൂർണ്ണസംഖ്യ scalarcn = cn == 3 ? 4 : cn;
    പൂർണ്ണസംഖ്യ sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src = _src.getUMat();
    എങ്കിൽ( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        പൂർണ്ണസംഖ്യ dtop = std::min(ofs.y, മുകൾ_2);
        പൂർണ്ണസംഖ്യ dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        പൂർണ്ണസംഖ്യ dleft = std::min(ofs.x, ഇടത്);
        പൂർണ്ണസംഖ്യ dright = std::min(wholeSize.width - src.cols - ofs.x, വലത്);
        src.adjustROI(dtop, dbottom, dleft, dright);
        മുകൾ_2 -= dtop;
        ഇടത് -= dleft;
        bottom -= dbottom;
        വലത് -= dright;
    }

    _dst.create(src.rows + മുകൾ_2 + bottom, src.cols + ഇടത് + വലത്, type);
    UMat dst = _dst.getUMat();

    എങ്കിൽ (മുകൾ_2 == 0 && ഇടത് == 0 && bottom == 0 && വലത് == 0)
    {
        എങ്കിൽ(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        തിരികെ സത്യം;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           മുകൾ_2, ഇടത്, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

}
#endif

#ifdef HAVE_IPP
നാമാകാശ cv {

സ്ഥിതം തർക്കം ipp_copyMakeBorder( Mat &_src, Mat &_dst, പൂർണ്ണസംഖ്യ മുകൾ_2, പൂർണ്ണസംഖ്യ bottom,
                                പൂർണ്ണസംഖ്യ ഇടത്, പൂർണ്ണസംഖ്യ വലത്, പൂർണ്ണസംഖ്യ _borderType, സ്ഥിരം Scalar& value )
{
#എങ്കിൽ defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(ഇടത്, മുകൾ_2, വലത്, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    എങ്കിൽ((പൂർണ്ണസംഖ്യ)borderType == -1)
        തിരികെ അസത്യം;

    എങ്കിൽ(_src.dims > 2)
        തിരികെ അസത്യം;

    Rect dstRect(borderSize.ഇടത്, borderSize.മുകൾ_2,
        _dst.cols - borderSize.വലത് - borderSize.ഇടത്,
        _dst.rows - borderSize.bottom - borderSize.മുകൾ_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    തിരികെ CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#അല്ലെങ്കിൽ
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(മുകൾ_2); CV_UNUSED(bottom); CV_UNUSED(ഇടത്); CV_UNUSED(വലത്);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    തിരികെ അസത്യം;
#endif
}
}
#endif

ശൂന്യം cv::copyMakeBorder( നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ മുകൾ_2, പൂർണ്ണസംഖ്യ bottom,
                         പൂർണ്ണസംഖ്യ ഇടത്, പൂർണ്ണസംഖ്യ വലത്, പൂർണ്ണസംഖ്യ borderType, സ്ഥിരം Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( മുകൾ_2 >= 0 && bottom >= 0 && ഇടത് >= 0 && വലത് >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + മുകൾ_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + ഇടത് + വലത് <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, മുകൾ_2, bottom, ഇടത്, വലത്, borderType, value))
    പൂർണ്ണസംഖ്യ type = src.type();

    എങ്കിൽ( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        പൂർണ്ണസംഖ്യ dtop = std::min(ofs.y, മുകൾ_2);
        പൂർണ്ണസംഖ്യ dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        പൂർണ്ണസംഖ്യ dleft = std::min(ofs.x, ഇടത്);
        പൂർണ്ണസംഖ്യ dright = std::min(wholeSize.width - src.cols - ofs.x, വലത്);
        src.adjustROI(dtop, dbottom, dleft, dright);
        മുകൾ_2 -= dtop;
        ഇടത് -= dleft;
        bottom -= dbottom;
        വലത് -= dright;
    }

    _dst.create( src.rows + മുകൾ_2 + bottom, src.cols + ഇടത് + വലത്, type );
    Mat dst = _dst.getMat();

    എങ്കിൽ(മുകൾ_2 == 0 && ഇടത് == 0 && bottom == 0 && വലത് == 0)
    {
        എങ്കിൽ(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        തിരികെ;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, മുകൾ_2, bottom, ഇടത്, വലത്, borderType, value))

    എങ്കിൽ( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           മുകൾ_2, ഇടത്, (പൂർണ്ണസംഖ്യ)src.elemSize(), borderType );
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ cn = src.channels(), cn1 = cn;
        AutoBuffer<ഇരട്ട> buf(cn);
        എങ്കിൽ( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                മുകൾ_2, ഇടത്, (പൂർണ്ണസംഖ്യ)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
