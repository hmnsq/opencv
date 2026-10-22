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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"

નામાકાશ cv
{

template <typename T> સ્થિર અંતઃરેખ
શૂન્ય scalarToRawData_(અચલ Scalar& s, T * અચલ buf, અચલ પૂર્ણાંક cn, અચલ પૂર્ણાંક unroll_to)
{
    પૂર્ણાંક i = 0;
    માટે(; i < cn; i++)
        buf[i] = saturate_cast<T>(s.val[i]);
    માટે(; i < unroll_to; i++)
        buf[i] = buf[i-cn];
}

શૂન્ય scalarToRawData(અચલ Scalar& s, શૂન્ય* _buf, પૂર્ણાંક type, પૂર્ણાંક unroll_to)
{
    CV_INSTRUMENT_REGION();

    અચલ પૂર્ણાંક depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    પસંદગી(depth)
    {
    સ્થિતિ CV_8U:
        scalarToRawData_(s, (uchar*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_8S:
        scalarToRawData_(s, (schar*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_Bool:
        scalarToRawData_(s, (તાર્કિક*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_16U:
        scalarToRawData_(s, (ushort*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_16S:
        scalarToRawData_(s, (short*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_16F:
        scalarToRawData_(s, (hfloat*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_16BF:
        scalarToRawData_(s, (bfloat*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_8F_E4M3FN:
        scalarToRawData_(s, (fp8_t*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_8F_E4M3FNUZ:
        scalarToRawData_(s, (fp8a_t*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_32U:
        scalarToRawData_(s, (unsigned*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_32S:
        scalarToRawData_(s, (પૂર્ણાંક*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_32F:
        scalarToRawData_(s, (દશાંશ*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_64U:
        scalarToRawData_(s, (uint64_t*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_64S:
        scalarToRawData_(s, (int64_t*)_buf, cn, unroll_to);
        તોડો;
    સ્થિતિ CV_64F:
        scalarToRawData_(s, (દ્વિગુણ*)_buf, cn, unroll_to);
        તોડો;
    મૂળભૂત:
        CV_Error(cv::Error::StsUnsupportedFormat,"");
    }
}

શૂન્ય convertAndUnrollScalar( અચલ Mat& sc, પૂર્ણાંક buftype, uchar* scbuf, size_t blocksize )
{
    પૂર્ણાંક scn = (પૂર્ણાંક)sc.total(), cn = CV_MAT_CN(buftype);
    size_t esz = CV_ELEM_SIZE(buftype);
    દ્વિઅંકી_Func cvtFn = getConvertFunc(sc.depth(), buftype);
    CV_Assert(cvtFn);
    cvtFn(sc.ptr(), 1, 0, 1, scbuf, 1, Size(std::min(cn, scn), 1), 0);
    // unroll the scalar
    જો( scn < cn )
    {
        CV_Assert( scn == 1 );
        size_t esz1 = CV_ELEM_SIZE1(buftype);
        માટે( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    માટે( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}

template<typename T> સ્થિર શૂન્ય
copyMask_(અચલ uchar* _src, size_t sstep, અચલ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    માટે( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        અચલ T* src = (અચલ T*)_src;
        T* dst = (T*)_dst;
        પૂર્ણાંક x = 0;
         #જો CV_ENABLE_UNROLLED
        માટે( ; x <= size.width - 4; x += 4 )
        {
            જો( mask[x] )
                dst[x] = src[x];
            જો( mask[x+1] )
                dst[x+1] = src[x+1];
            જો( mask[x+2] )
                dst[x+2] = src[x+2];
            જો( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        માટે( ; x < size.width; x++ )
            જો( mask[x] )
                dst[x] = src[x];
    }
}

template<> શૂન્ય
copyMask_<uchar>(અચલ uchar* _src, size_t sstep, અચલ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (પૂર્ણાંક)sstep, _dst, (પૂર્ણાંક)dstep, ippiSize(size), mask, (પૂર્ણાંક)mstep) >= 0)

    માટે( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        અચલ uchar* src = (અચલ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        પૂર્ણાંક x = 0;
        #જો (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            માટે( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        માટે( ; x < size.width; x++ )
            જો( mask[x] )
                dst[x] = src[x];
    }
}

template<> શૂન્ય
copyMask_<Vec3b>(અચલ uchar* _src, size_t sstep, અચલ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    માટે( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        અચલ uchar* src = (અચલ uchar*)_src;
        uchar* dst = (uchar*)_dst;
        પૂર્ણાંક x = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
        માટે( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        માટે( ; x < size.width; x++ )
            જો( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> શૂન્ય
copyMask_<ushort>(અચલ uchar* _src, size_t sstep, અચલ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (અચલ Ipp16u *)_src, (પૂર્ણાંક)sstep, (Ipp16u *)_dst, (પૂર્ણાંક)dstep, ippiSize(size), mask, (પૂર્ણાંક)mstep) >= 0)

    માટે( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        અચલ ushort* src = (અચલ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        પૂર્ણાંક x = 0;
        #જો (CV_SIMD || CV_SIMD_SCALABLE)
        {
            v_uint8 v_zero = vx_setzero_u8();

            માટે( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        માટે( ; x < size.width; x++ )
            જો( mask[x] )
                dst[x] = src[x];
    }
}

template<> શૂન્ય
copyMask_<Vec3s>(અચલ uchar* _src, size_t sstep, અચલ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    માટે( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        અચલ ushort* src = (અચલ ushort*)_src;
        ushort* dst = (ushort*)_dst;
        પૂર્ણાંક x = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
        માટે( ; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes() )
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
        માટે( ; x < size.width; x++ )
            જો( mask[x] ) {
                dst[3 * x] = src[3 * x];
                dst[3 * x + 1] = src[3 * x + 1];
                dst[3 * x + 2] = src[3 * x + 2];
            }
    }
}

template<> શૂન્ય
copyMask_<પૂર્ણાંક>(અચલ uchar* _src, size_t sstep, અચલ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    માટે( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        અચલ પૂર્ણાંક* src = (અચલ પૂર્ણાંક*)_src;
        પૂર્ણાંક* dst = (પૂર્ણાંક*)_dst;
        પૂર્ણાંક x = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
        માટે (; x <= size.width - VTraits<v_uint8>::vlanes(); x += VTraits<v_uint8>::vlanes())
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
        માટે (; x < size.width; x++)
            જો ( mask[x] )
                dst[x] = src[x];
    }
}

સ્થિર શૂન્ય
copyMaskGeneric(અચલ uchar* _src, size_t sstep, અચલ uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, શૂન્ય* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    માટે( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        અચલ uchar* src = _src;
        uchar* dst = _dst;
        પૂર્ણાંક x = 0;
        માટે( ; x < size.width; x++, src += esz, dst += esz )
        {
            જો( !mask[x] )
                ચાલુ;
            માટે( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
સ્થિર શૂન્ય copyMask##suffix(અચલ uchar* src, size_t sstep, અચલ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, શૂન્ય*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#જો defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
સ્થિર શૂન્ય copyMask##suffix(અચલ uchar* src, size_t sstep, અચલ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, શૂન્ય*) \
{ \
    CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (અચલ ipptype *)src, (પૂર્ણાંક)sstep, (ipptype *)dst, (પૂર્ણાંક)dstep, ippiSize(size), (અચલ Ipp8u *)mask, (પૂર્ણાંક)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#નહીંતર
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
સ્થિર શૂન્ય copyMask##suffix(અચલ uchar* src, size_t sstep, અચલ uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, શૂન્ય*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#જો IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#નહીંતર
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, પૂર્ણાંક, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

દ્વિઅંકી_Func copyMaskTab[] =
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

દ્વિઅંકી_Func getCopyMaskFunc(size_t esz)
{
    પાછા esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
શૂન્ય Mat::copyTo( OutputArray _dst ) અચલ
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    જો (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*આ);
        પાછા;
    }
#endif

    પૂર્ણાંક stype = type();
    પૂર્ણાંક dtype = _dst.type();
    જો( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        પાછા;
    }

    જો( dims == 0 && empty() )
    {
        _dst.release();
        શૂન્ય* obj = _dst.getObj();
        જો (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        નહીંતર જો (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        નહીંતર જો (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        પાછા;
    }

    તાર્કિક allowTransposed = dims == 1 ||
        _dst.kind() == આગત_Array_2::STD_VECTOR ||
        (_dst.fixedSize() && _dst.dims() == 1);
    જો( _dst.isUMat() )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        જો (empty())
            પાછા;
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u != ખાલી);
        size_t i, sz[CV_MAX_DIM] = {1}, dstofs[CV_MAX_DIM] = {0}, esz = CV_ELEM_SIZE(stype);
        CV_Assert(dims >= 0 && dims < CV_MAX_DIM);
        માટે( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        પૂર્ણાંક lastdim = dims >= 1 ? dims-1 : 0;
        sz[lastdim] *= esz;
        dst.ndoffset(dstofs);
        dstofs[lastdim] *= esz;
        dst.u->currAllocator->upload(dst.u, data, std::max(dims, 1), sz, dstofs, dst.step.p, step.p);
        પાછા;
    }

    જો( dims <= 2 )
    {
        _dst.create( dims, size.p, stype, -1, allowTransposed );
        Mat dst = _dst.getMat();
        જો( data == dst.data )
            પાછા;

        જો( rows > 0 && cols > 0 )
        {
            Mat src = *આ;
            Size sz = getContinuousSize2D(src, dst, (પૂર્ણાંક)elemSize());
            CV_CheckGE(sz.width, 0, "");

            અચલ uchar* sptr = src.data;
            uchar* dptr = dst.data;

#જો IPP_VERSION_X100 >= 201700
            CV_IPP_RUN_FAST(CV_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R_L, sptr, (પૂર્ણાંક)src.step, dptr, (પૂર્ણાંક)dst.step, ippiSizeL(sz.width, sz.height)) >= 0)
#endif

            માટે (; sz.height--; sptr += src.step, dptr += dst.step)
                memcpy(dptr, sptr, sz.width);
        }
        પાછા;
    }

    _dst.create( size, stype );
    Mat dst = _dst.getMat();
    જો( data == dst.data )
        પાછા;

    જો( total() != 0 )
    {
        અચલ Mat* arrays[] = { આ, &dst };
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

#ifdef HAVE_IPP
સ્થિર તાર્કિક ipp_copyTo(અચલ Mat &src, Mat &dst, અચલ Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    જો(mask.channels() > 1 || mask.depth() != CV_8U)
        પાછા ખોટું;

    જો (src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());
        પાછા CV_INSTRUMENT_FUN_IPP(llwiCopyMask, src.ptr(), (પૂર્ણાંક)src.step, dst.ptr(), (પૂર્ણાંક)dst.step, size, (પૂર્ણાંક)src.elemSize1(), src.channels(), mask.ptr(), (પૂર્ણાંક)mask.step) >= 0;
    }
    નહીંતર
    {
        અચલ Mat      *arrays[] = {&src, &dst, &mask, ખાલી};
        uchar          *ptrs[3]  = {ખાલી};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = ippiSize(it.size, 1);

        માટે (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            જો(CV_INSTRUMENT_FUN_IPP(llwiCopyMask, ptrs[0], 0, ptrs[1], 0, size, (પૂર્ણાંક)src.elemSize1(), src.channels(), ptrs[2], 0) < 0)
                પાછા ખોટું;
        }
        પાછા સાચું;
    }
#નહીંતર
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(mask);
    પાછા ખોટું;
#endif
}
#endif

શૂન્ય Mat::copyTo( OutputArray _dst, આગત_Array _mask ) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat mask = _mask.getMat();
    જો( !mask.data )
    {
        copyTo(_dst);
        પાછા;
    }

    પૂર્ણાંક cn = channels(), mcn = mask.channels();
    CV_Assert( (mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) && (mcn == 1 || mcn == cn) );
    તાર્કિક colorMask = mcn > 1;
    જો( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
    }

    Mat dst;
    {
        Mat dst0 = _dst.getMat();
        _dst.create(size, type()); // TODO Prohibit 'dst' re-creation, user should pass it explicitly with correct size/type or empty
        dst = _dst.getMat();

        જો (dst.data != dst0.data) // re-allocation happened
        {
#ifdef OPENCV_FUTURE
            CV_Assert(dst0.empty() &&
                "copyTo(): dst size/type mismatch (looks like a bug) - use dst.release() before copyTo() call to suppress this message");
#endif
            dst = Scalar(0); // do not leave dst uninitialized
        }
    }

    CV_IPP_RUN_FAST(ipp_copyTo(*આ, dst, mask))
    જો ( આ->dims <= 2 ) {
        જો ( આ->size() == dst.size() && આ->size() == dst.size() ) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, આ->data, આ->step, dst.data, dst.step, આ->cols, આ->rows, આ->type(), mask.data, mask.step, mask.type());
        }
    }
    નહીંતર જો ( આ->isContinuous() && dst.isContinuous() && mask.isContinuous() )
    {
        size_t sz = આ->total();
        જો (sz < INT_MAX) {
            CALL_HAL(copyToMask, cv_hal_copyToMasked, આ->data, 0, dst.data, 0, (પૂર્ણાંક)sz, 1, આ->type(), mask.data, 0, mask.type());
        }
    }

    size_t esz = colorMask ? elemSize1() : elemSize();
    દ્વિઅંકી_Func copymask = getCopyMaskFunc(esz);

    જો( dims <= 2 )
    {
        Mat src = *આ;
        Size sz = getContinuousSize2D(src, dst, mask, mcn);
        copymask(src.data, src.step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        પાછા;
    }

    અચલ Mat* arrays[] = { આ, &dst, &mask, 0 };
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    Size sz((પૂર્ણાંક)(it.size*mcn), 1);

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

/* dst = src */
શૂન્ય Mat::copyAt( OutputArray _dst ) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst);
}
શૂન્ય Mat::copyAt( OutputArray _dst, આગત_Array _mask ) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat dst = _dst.getMat();
    CV_CheckTrue( !dst.empty(), "dst must not be empty" );
    CV_CheckTypeEQ(type(), dst.type(), "Make the type of dst the same as src");
    CV_CheckEQ(size(), dst.size(), "Make the size of dst the same as src");
    copyTo(_dst, _mask);
}


સ્થિર તાર્કિક can_apply_memset(અચલ Mat &mat, અચલ Scalar &s, પૂર્ણાંક &fill_કિંમત)
{
    // check if depth is 1 byte.
    પસંદગી (mat.depth())
    {
    સ્થિતિ CV_8U: fill_કિંમત = saturate_cast<uchar>( s.val[0] ); તોડો;
    સ્થિતિ CV_8S: fill_કિંમત = saturate_cast<schar>( s.val[0] ); તોડો;
    મૂળભૂત: પાછા ખોટું;
    }

    // check if all element is same.
    અચલ int64* is = (અચલ int64*)&s.val[0];
    પસંદગી (mat.channels())
    {
    સ્થિતિ 1: પાછા સાચું;
    સ્થિતિ 2: પાછા (is[0] == is[1]);
    સ્થિતિ 3: પાછા (is[0] == is[1] && is[1] == is[2]);
    સ્થિતિ 4: પાછા (is[0] == is[1] && is[1] == is[2] && is[2] == is[3]);
    મૂળભૂત: પાછા ખોટું;
    }
}

Mat& Mat::operator = (અચલ Scalar& s)
{
    CV_INSTRUMENT_REGION();

    જો (આ->empty())
        પાછા *આ;

    અચલ Mat* arrays[] = { આ };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    અચલ int64* is = (અચલ int64*)&s.val[0];

    જો( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    નહીંતર
    {
        પૂર્ણાંક fill_કિંમત = 0;
        જો ( can_apply_memset(*આ, s, fill_કિંમત) )
        {
            માટે (size_t i = 0; i < it.nplanes; i++, ++it)
                memset(dptr, fill_કિંમત, elsize);
            પાછા *આ;
        }

        જો( it.nplanes > 0 )
        {
            દ્વિગુણ scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            માટે( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                CV_Assert(sz <= કદ(scalar));
                memcpy( dptr + j, scalar, sz );
            }
        }

        માટે( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    પાછા *આ;
}

#ifdef HAVE_IPP
સ્થિર તાર્કિક ipp_Mat_setTo_Mat(Mat &dst, Mat &_val, Mat &mask)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    જો(mask.empty())
        પાછા ખોટું;

    જો(mask.depth() != CV_8U || mask.channels() > 1)
        પાછા ખોટું;

    જો(dst.channels() > 4)
        પાછા ખોટું;

    જો (dst.depth() == CV_32F)
    {
        માટે (પૂર્ણાંક i = 0; i < (પૂર્ણાંક)(_val.total()); i++)
        {
            દશાંશ v = (દશાંશ)(_val.at<દ્વિગુણ>(i));  // cast to float
            જો (cvIsNaN(v) || cvIsInf(v))  // accept finite numbers only
                પાછા ખોટું;
        }
    }

    જો(dst.dims <= 2)
    {
        IppiSize            size     = ippiSize(dst.size());
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_કિંમત_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        પાછા CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, dst.ptr(), (પૂર્ણાંક)dst.step, size, dataType, dst.channels(), mask.ptr(), (પૂર્ણાંક)mask.step) >= 0;
    }
    નહીંતર
    {
        અચલ Mat      *arrays[] = {&dst, mask.empty()?ખાલી:&mask, ખાલી};
        uchar          *ptrs[2]  = {ખાલી};
        NAryMatIterator it(arrays, ptrs);

        IppiSize            size     = {(પૂર્ણાંક)it.size, 1};
        IppDataType         dataType = ippiGetDataType(dst.depth());
        ::ipp::Iw_કિંમત_Float s;
        convertAndUnrollScalar(_val, CV_MAKETYPE(CV_64F, dst.channels()), (uchar*)((Ipp64f*)s), 1);

        માટે( size_t i = 0; i < it.nplanes; i++, ++it)
        {
            જો(CV_INSTRUMENT_FUN_IPP(llwiSetMask, s, ptrs[0], 0, size, dataType, dst.channels(), ptrs[1], 0) < 0)
                પાછા ખોટું;
        }
        પાછા સાચું;
    }
#નહીંતર
    CV_UNUSED(dst); CV_UNUSED(_val); CV_UNUSED(mask);
    પાછા ખોટું;
#endif
}
#endif

Mat& Mat::setTo(આગત_Array કિંમત, આગત_Array _mask)
{
    CV_INSTRUMENT_REGION();

    જો( empty() )
        પાછા *આ;

    Mat value = કિંમત.getMat(), mask = _mask.getMat();

    CV_Assert( checkScalar(value, type(), કિંમત.kind(), આગત_Array_2::MAT ));
    પૂર્ણાંક cn = channels(), mcn = mask.channels();
    CV_Assert( mask.empty() || ((mask.depth() == CV_8U || mask.depth() == CV_8S || mask.depth() == CV_Bool) &&
               (mcn == 1 || mcn == cn) && size == mask.size) );

    CV_IPP_RUN_FAST(ipp_Mat_setTo_Mat(*આ, value, mask), *આ)

    size_t esz = mcn > 1 ? elemSize1() : elemSize();
    દ્વિઅંકી_Func copymask = getCopyMaskFunc(esz);

    અચલ Mat* arrays[] = { આ, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    પૂર્ણાંક totalsz = (પૂર્ણાંક)it.size*mcn;
    પૂર્ણાંક blockSize0 = std::min(totalsz, (પૂર્ણાંક)((BLOCK_SIZE + esz-1)/esz));
    blockSize0 -= blockSize0 % mcn;    // must be divisible without remainder for unrolling and advancing
    AutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = alignPtr((uchar*)_scbuf.data(), (પૂર્ણાંક)કદ(દ્વિગુણ));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0/mcn );

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( પૂર્ણાંક j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(std::min(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            જો( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            નહીંતર
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    પાછા *આ;
}


Mat& Mat::setZero()
{
    CV_INSTRUMENT_REGION();

    જો( empty() )
        પાછા *આ;

    size_t esz = elemSize();

    અચલ Mat* arrays[] = { આ, 0 };
    uchar* ptrs[]={0};
    NAryMatIterator it(arrays, ptrs);

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        memset(ptrs[0], 0, esz*it.size);
    પાછા *આ;
}


#જો defined HAVE_OPENCL && !defined __APPLE__

સ્થિર તાર્કિક ocl_repeat(આગત_Array _src, પૂર્ણાંક ny, પૂર્ણાંક nx, OutputArray _dst)
{
    જો (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        પાછા સાચું;
    }

    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    જો (k.empty())
        પાછા ખોટું;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

શૂન્ય repeat(આગત_Array _src, પૂર્ણાંક ny, પૂર્ણાંક nx, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(_src.getObj() != _dst.getObj());
    CV_Assert( _src.dims() <= 2 );
    CV_Assert( ny > 0 && nx > 0 );

    Size ssize = _src.size();
    જો( ssize.empty() )
    {
        _dst.create(0, 0, _src.type());
        પાછા;
    }
    CV_Assert( (int64_t)ssize.height * ny <= INT_MAX && (int64_t)ssize.width * nx <= INT_MAX );
    _dst.create(ssize.height*ny, ssize.width*nx, _src.type());

#જો !defined __APPLE__
    CV_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();
    Size dsize = dst.size();
    પૂર્ણાંક esz = (પૂર્ણાંક)src.elemSize();
    પૂર્ણાંક x, y;
    ssize.width *= esz; dsize.width *= esz;

    માટે( y = 0; y < ssize.height; y++ )
    {
        માટે( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    માટે( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

Mat repeat(અચલ Mat& src, પૂર્ણાંક ny, પૂર્ણાંક nx)
{
    જો( nx == 1 && ny == 1 )
        પાછા src;
    Mat dst;
    repeat(src, ny, nx, dst);
    પાછા dst;
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
પૂર્ણાંક cv::borderInterpolate( પૂર્ણાંક p, પૂર્ણાંક len, પૂર્ણાંક borderType )
{
    CV_TRACE_FUNCTION_VERBOSE();

    CV_DbgAssert(len > 0);

#ifdef CV_STATIC_ANALYSIS
    જો(p >= 0 && p < len)
#નહીંતર
    જો( (unsigned)p < (unsigned)len )
#endif
        ;
    નહીંતર જો( borderType == BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    નહીંતર જો( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
    {
        પૂર્ણાંક delta = borderType == BORDER_REFLECT_101;
        જો( len == 1 )
            પાછા 0;
        કરો
        {
            જો( p < 0 )
                p = -p - 1 + delta;
            નહીંતર
                p = len - 1 - (p - len) - delta;
        }
#ifdef CV_STATIC_ANALYSIS
        જ્યારે(p < 0 || p >= len);
#નહીંતર
        જ્યારે( (unsigned)p >= (unsigned)len );
#endif
    }
    નહીંતર જો( borderType == BORDER_WRAP )
    {
        CV_Assert(len > 0);
        જો( p < 0 || p >= len )
        {
            int64_t p64 = p;
            p64 %= (int64_t)len;
            જો( p64 < 0 )
                p64 += len;
            p = (પૂર્ણાંક)p64;
        }
    }
    નહીંતર જો( borderType == BORDER_CONSTANT )
        p = -1;
    નહીંતર
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported border type" );
    પાછા p;
}

નામાકાશ
{

શૂન્ય copyMakeBorder_8u( અચલ uchar* src, size_t srcstep, cv::Size srcroi,
                        uchar* dst, size_t dststep, cv::Size dstroi,
                        પૂર્ણાંક ટોચ_2, પૂર્ણાંક ડાબું, પૂર્ણાંક cn, પૂર્ણાંક borderType )
{
    અચલ પૂર્ણાંક isz = (પૂર્ણાંક)કદ(પૂર્ણાંક);
    પૂર્ણાંક i, j, k, elemSize = 1;
    તાર્કિક intMode = ખોટું;

    જો( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = સાચું;
    }

    cv::AutoBuffer<પૂર્ણાંક> _tab((dstroi.width - srcroi.width)*cn);
    પૂર્ણાંક* tab = _tab.data();
    પૂર્ણાંક જમણું = dstroi.width - srcroi.width - ડાબું;
    પૂર્ણાંક bottom = dstroi.height - srcroi.height - ટોચ_2;

    માટે( i = 0; i < ડાબું; i++ )
    {
        j = cv::borderInterpolate(i - ડાબું, srcroi.width, borderType)*cn;
        માટે( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    માટે( i = 0; i < જમણું; i++ )
    {
        j = cv::borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        માટે( k = 0; k < cn; k++ )
            tab[(i+ડાબું)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    ડાબું *= cn;
    જમણું *= cn;

    uchar* dstInner = dst + dststep*ટોચ_2 + ડાબું*elemSize;

    માટે( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        જો( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        જો( intMode )
        {
            અચલ પૂર્ણાંક* isrc = (પૂર્ણાંક*)src;
            પૂર્ણાંક* idstInner = (પૂર્ણાંક*)dstInner;
            માટે( j = 0; j < ડાબું; j++ )
                idstInner[j - ડાબું] = isrc[tab[j]];
            માટે( j = 0; j < જમણું; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + ડાબું]];
        }
        નહીંતર
        {
            માટે( j = 0; j < ડાબું; j++ )
                dstInner[j - ડાબું] = src[tab[j]];
            માટે( j = 0; j < જમણું; j++ )
                dstInner[j + srcroi.width] = src[tab[j + ડાબું]];
        }
    }

    dstroi.width *= elemSize;

    માટે( i = 0; i < ટોચ_2; i++ )
    {
        j = cv::borderInterpolate(i - ટોચ_2, srcroi.height, borderType);
        memcpy(dst + i*dststep, dst + (ટોચ_2+j)*dststep, dstroi.width);
    }

    dst += dststep*ટોચ_2;
    માટે( i = 0; i < bottom; i++ )
    {
        j = cv::borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


શૂન્ય copyMakeConstBorder_8u( અચલ uchar* src, size_t srcstep, cv::Size srcroi,
                             uchar* dst, size_t dststep, cv::Size dstroi,
                             પૂર્ણાંક ટોચ_2, પૂર્ણાંક ડાબું, પૂર્ણાંક cn, અચલ uchar* value )
{
    પૂર્ણાંક i, j;
    cv::AutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf.data();
    પૂર્ણાંક જમણું = dstroi.width - srcroi.width - ડાબું;
    પૂર્ણાંક bottom = dstroi.height - srcroi.height - ટોચ_2;

    માટે( i = 0; i < dstroi.width; i++ )
    {
        માટે( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    ડાબું *= cn;
    જમણું *= cn;

    uchar* dstInner = dst + dststep*ટોચ_2 + ડાબું;

    માટે( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        જો( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - ડાબું, constBuf, ડાબું );
        memcpy( dstInner + srcroi.width, constBuf, જમણું );
    }

    માટે( i = 0; i < ટોચ_2; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);

    dst += (ટોચ_2 + srcroi.height) * dststep;
    માટે( i = 0; i < bottom; i++ )
        memcpy(dst + i * dststep, constBuf, dstroi.width);
}

}

#ifdef HAVE_OPENCL

નામાકાશ cv {

સ્થિર તાર્કિક ocl_copyMakeBorder( આગત_Array _src, OutputArray _dst, પૂર્ણાંક ટોચ_2, પૂર્ણાંક bottom,
                                પૂર્ણાંક ડાબું, પૂર્ણાંક જમણું, પૂર્ણાંક borderType, અચલ Scalar& value )
{
    પૂર્ણાંક type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    તાર્કિક isolated = (borderType & BORDER_ISOLATED) != 0;
    borderType &= ~cv::BORDER_ISOLATED;

    જો ( !(borderType == BORDER_CONSTANT || borderType == BORDER_REPLICATE || borderType == BORDER_REFLECT ||
           borderType == BORDER_WRAP || borderType == BORDER_REFLECT_101) ||
         cn > 4)
        પાછા ખોટું;

    અચલ અક્ષર * અચલ borderMap[] = { "BORDER_CONSTANT", "BORDER_REPLICATE", "BORDER_REFLECT", "BORDER_WRAP", "BORDER_REFLECT_101" };
    પૂર્ણાંક scalarcn = cn == 3 ? 4 : cn;
    પૂર્ણાંક sctype = CV_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    જો (k.empty())
        પાછા ખોટું;

    UMat src = _src.getUMat();
    જો( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        પૂર્ણાંક dtop = std::min(ofs.y, ટોચ_2);
        પૂર્ણાંક dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        પૂર્ણાંક dleft = std::min(ofs.x, ડાબું);
        પૂર્ણાંક dright = std::min(wholeSize.width - src.cols - ofs.x, જમણું);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ટોચ_2 -= dtop;
        ડાબું -= dleft;
        bottom -= dbottom;
        જમણું -= dright;
    }

    _dst.create(src.rows + ટોચ_2 + bottom, src.cols + ડાબું + જમણું, type);
    UMat dst = _dst.getUMat();

    જો (ટોચ_2 == 0 && ડાબું == 0 && bottom == 0 && જમણું == 0)
    {
        જો(src.u != dst.u || src.step != dst.step)
            src.copyTo(dst);
        પાછા સાચું;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           ટોચ_2, ડાબું, ocl::KernelArg::Constant(Mat(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

}
#endif

#ifdef HAVE_IPP
નામાકાશ cv {

સ્થિર તાર્કિક ipp_copyMakeBorder( Mat &_src, Mat &_dst, પૂર્ણાંક ટોચ_2, પૂર્ણાંક bottom,
                                પૂર્ણાંક ડાબું, પૂર્ણાંક જમણું, પૂર્ણાંક _borderType, અચલ Scalar& value )
{
#જો defined HAVE_IPP_IW_LL && !IPP_DISABLE_PERF_COPYMAKE
    CV_INSTRUMENT_REGION_IPP();

    ::ipp::IwiBorderSize borderSize(ડાબું, ટોચ_2, જમણું, bottom);
    ::ipp::IwiSize       size(_src.cols, _src.rows);
    IppDataType          dataType   = ippiGetDataType(_src.depth());
    IppiBorderType       borderType = ippiGetBorderType(_borderType);
    જો((પૂર્ણાંક)borderType == -1)
        પાછા ખોટું;

    જો(_src.dims > 2)
        પાછા ખોટું;

    Rect dstRect(borderSize.ડાબું, borderSize.ટોચ_2,
        _dst.cols - borderSize.જમણું - borderSize.ડાબું,
        _dst.rows - borderSize.bottom - borderSize.ટોચ_2);
    Mat  subDst = Mat(_dst, dstRect);
    Mat *pSrc   = &_src;

    પાછા CV_INSTRUMENT_FUN_IPP(llwiCopyMakeBorder, pSrc->ptr(), pSrc->step, subDst.ptr(), subDst.step, size, dataType, _src.channels(), borderSize, borderType, &value[0]) >= 0;
#નહીંતર
    CV_UNUSED(_src); CV_UNUSED(_dst); CV_UNUSED(ટોચ_2); CV_UNUSED(bottom); CV_UNUSED(ડાબું); CV_UNUSED(જમણું);
    CV_UNUSED(_borderType); CV_UNUSED(value);
    પાછા ખોટું;
#endif
}
}
#endif

શૂન્ય cv::copyMakeBorder( આગત_Array _src, OutputArray _dst, પૂર્ણાંક ટોચ_2, પૂર્ણાંક bottom,
                         પૂર્ણાંક ડાબું, પૂર્ણાંક જમણું, પૂર્ણાંક borderType, અચલ Scalar& value )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( ટોચ_2 >= 0 && bottom >= 0 && ડાબું >= 0 && જમણું >= 0 && _src.dims() <= 2 );
    CV_Assert( !_src.empty() );

    Mat src = _src.getMat();
    CV_Assert( (int64_t)src.rows + ટોચ_2 + bottom <= INT_MAX );
    CV_Assert( (int64_t)src.cols + ડાબું + જમણું <= INT_MAX );

    CV_OCL_RUN(_dst.isUMat(),
               ocl_copyMakeBorder(_src, _dst, ટોચ_2, bottom, ડાબું, જમણું, borderType, value))
    પૂર્ણાંક type = src.type();

    જો( src.isSubmatrix() && (borderType & BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        પૂર્ણાંક dtop = std::min(ofs.y, ટોચ_2);
        પૂર્ણાંક dbottom = std::min(wholeSize.height - src.rows - ofs.y, bottom);
        પૂર્ણાંક dleft = std::min(ofs.x, ડાબું);
        પૂર્ણાંક dright = std::min(wholeSize.width - src.cols - ofs.x, જમણું);
        src.adjustROI(dtop, dbottom, dleft, dright);
        ટોચ_2 -= dtop;
        ડાબું -= dleft;
        bottom -= dbottom;
        જમણું -= dright;
    }

    _dst.create( src.rows + ટોચ_2 + bottom, src.cols + ડાબું + જમણું, type );
    Mat dst = _dst.getMat();

    જો(ટોચ_2 == 0 && ડાબું == 0 && bottom == 0 && જમણું == 0)
    {
        જો(src.data != dst.data || src.step != dst.step)
            src.copyTo(dst);
        પાછા;
    }

    borderType &= ~BORDER_ISOLATED;

    CV_IPP_RUN_FAST(ipp_copyMakeBorder(src, dst, ટોચ_2, bottom, ડાબું, જમણું, borderType, value))

    જો( borderType != BORDER_CONSTANT )
        copyMakeBorder_8u( src.ptr(), src.step, src.size(),
                           dst.ptr(), dst.step, dst.size(),
                           ટોચ_2, ડાબું, (પૂર્ણાંક)src.elemSize(), borderType );
    નહીંતર
    {
        પૂર્ણાંક cn = src.channels(), cn1 = cn;
        AutoBuffer<દ્વિગુણ> buf(cn);
        જો( cn > 4 )
        {
            CV_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf.data(), CV_MAKETYPE(src.depth(), cn1), cn);
        copyMakeConstBorder_8u( src.ptr(), src.step, src.size(),
                                dst.ptr(), dst.step, dst.size(),
                                ટોચ_2, ડાબું, (પૂર્ણાંક)src.elemSize(), (uchar*)buf.data() );
    }
}
/* End of file. */
