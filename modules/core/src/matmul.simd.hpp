/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Copyright (C) 2014-2015, Itseez Inc., all rights reserved.
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.
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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ifdef HAVE_LAPACK
#define CV_GEMM_BASELINE_ONLY
#endif
#define CV_MAHALANOBIS_BASELINE_ONLY
#define CV_MULTRANSPOSED_BASELINE_ONLY

// The SIMD GEMM kernels below reorder floating-point accumulation (and use FMA
// where the ISA provides it), producing a result that is correct (<1e-9 vs a
// long-double reference) but differs from the scalar reference in the last bits.
// The multi-view camera-calibration solver is ill-conditioned - fisheye
// ComputeHomography forms L^T*L, takes an SVD, then iterates (J^T*J)^-1 - so it
// amplifies that tiny deviation enough to flip its outcome on some build targets.
// CI (PR #29472, RegisterCamerasTest) shows this only on the 32-bit x86 (Windows)
// and macOS-x64 (Intel) jobs; Windows x64, macOS-ARM64 and all Linux targets pass.
// FP background: on 32-bit x86 the x87 FPU holds intermediates in 80-bit extended
// precision whereas SIMD accumulates in 64-bit; on modern x86-64 and ARM both
// scalar and SIMD use strict IEEE-754 width (64-bit double / 32-bit float, no
// hidden extended precision). The failure is therefore a toolchain/ABI-specific
// bifurcation rather than a single-feature effect, so we simply keep the
// reference-matching scalar GEMM path on the affected targets.
#ifndef CV_GEMM_SIMD_ENABLED
#ଯଦି defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#ଅନ୍ୟଥା
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

ନାମାକାଶ cv {

// forward declarations
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*TransformFunc)(ଅଚଳ ଶୂନ୍ୟ* src, ଶୂନ୍ୟ* dst, ଅଚଳ ଶୂନ୍ୟ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ScaleAddFunc)(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ ଶୂନ୍ୟ* alpha);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*MulTransposedFunc)(ଅଚଳ Mat& src, ଅଚଳ/*preallocated*/ Mat& dst, ଅଚଳ Mat& delta, ଦ୍ୱିଗୁଣ scale);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦ୍ୱିଗୁଣ (*MahalanobisImplFunc)(ଅଚଳ Mat& v1, ଅଚଳ Mat& v2, ଅଚଳ Mat& icovar, ଦ୍ୱିଗୁଣ *diff_buffer /*[len]*/, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

ଶୂନ୍ୟ gemm32f(ଅଚଳ ଦଶମିକ* src1, size_t src1_step, ଅଚଳ ଦଶମିକ* src2, size_t src2_step,
             ଦଶମିକ alpha, ଅଚଳ ଦଶମିକ* src3, size_t src3_step, ଦଶମିକ beta, ଦଶମିକ* dst, size_t dst_step,
             ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
ଶୂନ୍ୟ gemm64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, size_t src1_step, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, size_t src2_step,
             ଦ୍ୱିଗୁଣ alpha, ଅଚଳ ଦ୍ୱିଗୁଣ* src3, size_t src3_step, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ* dst, size_t dst_step,
             ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
ଶୂନ୍ୟ gemm32fc(ଅଚଳ ଦଶମିକ* src1, size_t src1_step, ଅଚଳ ଦଶମିକ* src2, size_t src2_step,
              ଦଶମିକ alpha, ଅଚଳ ଦଶମିକ* src3, size_t src3_step, ଦଶମିକ beta, ଦଶମିକ* dst, size_t dst_step,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
ଶୂନ୍ୟ gemm64fc(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, size_t src1_step, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, size_t src2_step,
              ଦ୍ୱିଗୁଣ alpha, ଅଚଳ ଦ୍ୱିଗୁଣ* src3, size_t src3_step, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ* dst, size_t dst_step,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

TransformFunc getTransformFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);
TransformFunc getDiagTransformFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);
TransformFunc getPerspectiveTransform(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);

ScaleAddFunc getScaleAddFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);

MahalanobisImplFunc getMahalanobisImplFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth);

MulTransposedFunc getMulTransposedFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, ତାର୍କିକ ata);

ଦ୍ୱିଗୁଣ dotProd_8u(ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଦ୍ୱିଗୁଣ dotProd_8s(ଅଚଳ schar* src1, ଅଚଳ schar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଦ୍ୱିଗୁଣ dotProd_16u(ଅଚଳ ushort* src1, ଅଚଳ ushort* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଦ୍ୱିଗୁଣ dotProd_16s(ଅଚଳ short* src1, ଅଚଳ short* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଦ୍ୱିଗୁଣ dotProd_32s(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଦ୍ୱିଗୁଣ dotProd_32f(ଅଚଳ ଦଶମିକ* src1, ଅଚଳ ଦଶମିକ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);
ଦ୍ୱିଗୁଣ dotProd_64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ଯଦି !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

ସ୍ଥିର ଶୂନ୍ୟ
GEMM_CopyBlock( ଅଚଳ uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    size.width *= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(pix_size / ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));

    ପାଇଁ( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #ଯଦି CV_ENABLE_UNROLLED
        ପାଇଁ( ; j <= size.width - 4; j += 4 )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା t0 = ((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)src)[j];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା t1 = ((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)src)[j+1];
            ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j] = t0;
            ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j+1] = t1;
            t0 = ((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)src)[j+2];
            t1 = ((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)src)[j+3];
            ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j+2] = t0;
            ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j+3] = t1;
        }
        #endif
        ପାଇଁ( ; j < size.width; j++ )
            ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j] = ((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)src)[j];
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
GEMM_TransposeBlock( ଅଚଳ uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପାଇଁ( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        ଅଚଳ uchar* _src = src;
        ବାଛ( pix_size )
        {
        ସ୍ଥିତି ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା):
            ପାଇଁ( j = 0; j < size.height; j++, _src += src_step )
                ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j] = ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_src)[0];
            ଭାଙ୍ଗ;
        ସ୍ଥିତି ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*2:
            ପାଇଁ( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା t0 = ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_src)[0];
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା t1 = ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_src)[1];
                ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j] = t0;
                ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j+1] = t1;
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*4:
            ପାଇଁ( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା t0 = ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_src)[0];
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା t1 = ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_src)[1];
                ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j] = t0;
                ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j+1] = t1;
                t0 = ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_src)[2];
                t1 = ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_src)[3];
                ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j+2] = t0;
                ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dst)[j+3] = t1;
            }
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            CV_Assert(0);
            ଫେରନ୍ତୁ;
        }
    }
}

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> ସଂରଚନା VecTraits;

template<> ସଂରଚନା VecTraits<ଦଶମିକ> {
    ପ୍ରକାରବ୍ୟାଖ୍ୟା v_float32 VT;
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ VT zero() { ଫେରନ୍ତୁ vx_setzero_f32(); }
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ VT load(ଅଚଳ ଦଶମିକ* p) { ଫେରନ୍ତୁ vx_load(p); }
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ VT setall(ଦଶମିକ v) { ଫେରନ୍ତୁ vx_setall_f32(v); }
};
#endif

#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> ସଂରଚନା VecTraits<ଦ୍ୱିଗୁଣ> {
    ପ୍ରକାରବ୍ୟାଖ୍ୟା v_float64 VT;
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ VT zero() { ଫେରନ୍ତୁ vx_setzero_f64(); }
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ VT load(ଅଚଳ ଦ୍ୱିଗୁଣ* p) { ଫେରନ୍ତୁ vx_load(p); }
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ VT setall(ଦ୍ୱିଗୁଣ v) { ଫେରନ୍ତୁ vx_setall_f64(v); }
};
#endif

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ST simdDotProduct(ଅଚଳ ST* a, ଅଚଳ ST* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& k)
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename VecTraits<ST>::VT VecT;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<VecT>::vlanes();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    ପାଇଁ( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    ପାଇଁ( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    ଫେରନ୍ତୁ v_reduce_sum(vs0);
}
#endif

#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ simdDotProduct_f32f64(ଅଚଳ ଦଶମିକ* a, ଅଚଳ ଦଶମିକ* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& k)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes_f32 = VTraits<v_float32>::vlanes();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    ପାଇଁ( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_ଉପର(va0), v_cvt_f64_ଉପର(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_ଉପର(va1), v_cvt_f64_ଉପର(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    ପାଇଁ( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_ଉପର(va), v_cvt_f64_ଉପର(vb), vs0);
    }
    ଫେରନ୍ତୁ v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ simdGEMM_kj_f32f64(
    ଅଚଳ ଦଶମିକ* a_data, ଅଚଳ ଦଶମିକ* _b_data, size_t b_step,
    ଅଚଳ ଦଶମିକ* _c_data, size_t c_step1,
    ଦଶମିକ* d_data,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m,
    ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା& j)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes_f32 = VTraits<v_float32>::vlanes();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes_f64 = VTraits<v_float64>::vlanes();

    ଯଦି( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_m = 1600 / (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(ଦଶମିକ);
        cv::AutoBuffer<ଦ୍ୱିଗୁଣ> _s_buf(max_m);
        ଦ୍ୱିଗୁଣ* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            ପାଇଁ( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            ପାଇଁ( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ଅଚଳ ଦଶମିକ* b_ptr = _b_data;
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((ଦ୍ୱିଗୁଣ)a_data[k]);
                j = 0;
                ପାଇଁ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_ଉପର(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                ଦ୍ୱିଗୁଣ a_val = (ଦ୍ୱିଗୁଣ)a_data[k];
                ପାଇଁ( ; j < m; j++ )
                    s_buf[j] += a_val * (ଦ୍ୱିଗୁଣ)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ଅଚଳ ଦଶମିକ* c_data = _c_data;
            j = 0;
            ଯଦି( !_c_data )
            {
                ପାଇଁ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                ପାଇଁ( ; j < m; j++ )
                    d_data[j] = (ଦଶମିକ)(s_buf[j] * alpha);
            }
            ଅନ୍ୟଥା ଯଦି( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                ପାଇଁ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_ଉପର(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                ପାଇଁ( ; j < m; j++ )
                    d_data[j] = (ଦଶମିକ)(s_buf[j] * alpha + (ଦ୍ୱିଗୁଣ)c_data[j] * beta);
            }
            ଅନ୍ୟଥା
            {
                ପାଇଁ( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (ଦଶମିକ)(s_buf[j] * alpha + (ଦ୍ୱିଗୁଣ)c_data[0] * beta);
            }
        }
        j = m;
    }
    ଅନ୍ୟଥା ଯଦି( vlanes_f32 * (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(ଦଶମିକ) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ଅଚଳ ଦଶମିକ* c_data = _c_data;
        ପାଇଁ( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            ଅଚଳ ଦଶମିକ* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((ଦ୍ୱିଗୁଣ)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_ଉପର(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            ଯଦି( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ଅନ୍ୟଥା ଯଦି( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_ଉପର(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ଅନ୍ୟଥା
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ଦଶମିକ buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ଦଶମିକ)beta;
            }
        }
    }
}
#endif

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ simdGEMM_kj(
    ଅଚଳ ST* a_data, ଅଚଳ ST* _b_data, size_t b_step,
    ଅଚଳ ST* _c_data, size_t c_step1,
    ST* d_data,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m,
    ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା& j)
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename VecTraits<ST>::VT VecT;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<VecT>::vlanes();

    ଯଦି( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_m = 1600 / (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            ପାଇଁ( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            ପାଇଁ( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ଅଚଳ ST* b_ptr = _b_data;
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                ପାଇଁ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                ପାଇଁ( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ଅଚଳ ST* c_data = _c_data;
            j = 0;
            ଯଦି( !_c_data )
            {
                ପାଇଁ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                ପାଇଁ( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            ଅନ୍ୟଥା ଯଦି( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                ପାଇଁ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                ପାଇଁ( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            ଅନ୍ୟଥା
            {
                ପାଇଁ( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    ଅନ୍ୟଥା ଯଦି( vlanes * (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ଅଚଳ ST* c_data = _c_data;
        ପାଇଁ( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            ଅଚଳ ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            ଯଦି( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            ଅନ୍ୟଥା ଯଦି( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            ଅନ୍ୟଥା
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> ସ୍ଥିର ଶୂନ୍ୟ
GEMMSingleMul( ଅଚଳ T* a_data, size_t a_step,
               ଅଚଳ T* b_data, size_t b_step,
               ଅଚଳ T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    ଅଚଳ T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= ଆକାର(a_data[0]);
    b_step /= ଆକାର(b_data[0]);
    c_step /= ଆକାର(c_data[0]);
    d_step /= ଆକାର(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    ଯଦି( !c_data )
        c_step0 = c_step1 = 0;
    ଅନ୍ୟଥା ଯଦି( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    ଅନ୍ୟଥା
        c_step0 = 1, c_step1 = c_step;

    ଯଦି( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        ଯଦି( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    ଯଦି( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        ଯଦି( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            ପାଇଁ( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        ଯଦି( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            ପାଇଁ( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        ପାଇଁ( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            ପାଇଁ( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                ଯଦି( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                ଅନ୍ୟଥା
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            ପାଇଁ( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                ଯଦି( !c_data )
                    d_data[j] = T(s0);
                ଅନ୍ୟଥା
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ଅନ୍ୟଥା ଯଦି( flags & GEMM_2_T ) /* A * Bt */
    {
        ପାଇଁ( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            ଯଦି( a_buf )
            {
                ପାଇଁ( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            ପାଇଁ( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#ଯଦି CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ଯଦି( ଆକାର(WT) == ଆକାର(ଦ୍ୱିଗୁଣ) )
                {
                    ଯଦି( ଆକାର(T) == ଆକାର(ଦ୍ୱିଗୁଣ) )
                        s0 = (WT)simdDotProduct((ଅଚଳ ଦ୍ୱିଗୁଣ*)a_data, (ଅଚଳ ଦ୍ୱିଗୁଣ*)b_data, n, k);
                    ଅନ୍ୟଥା ଯଦି( ଆକାର(T) == ଆକାର(ଦଶମିକ) )
                        s0 = (WT)simdDotProduct_f32f64((ଅଚଳ ଦଶମିକ*)a_data, (ଅଚଳ ଦଶମିକ*)b_data, n, k);
                }
                ଅନ୍ୟଥା
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #ଯଦି CV_ENABLE_UNROLLED
                    ପାଇଁ( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                ପାଇଁ( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                ଯଦି( !c_data )
                    d_data[j] = T(s0);
                ଅନ୍ୟଥା
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ଅନ୍ୟଥା ଯଦି( d_size.width*ଆକାର(d_data[0]) <= 1600 )
    {
        ପାଇଁ( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            ଯଦି( a_buf )
            {
                ପାଇଁ( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ଯଦି( ଆକାର(WT) == ଆକାର(ଦ୍ୱିଗୁଣ) )
            {
                ଯଦି( ଆକାର(T) == ଆକାର(ଦ୍ୱିଗୁଣ) )
                    simdGEMM_kj((ଅଚଳ ଦ୍ୱିଗୁଣ*)a_data, (ଅଚଳ ଦ୍ୱିଗୁଣ*)_b_data, b_step,
                                        (ଅଚଳ ଦ୍ୱିଗୁଣ*)_c_data, c_step1,
                                (ଦ୍ୱିଗୁଣ*)d_data, n, m, alpha, beta, j);
                ଅନ୍ୟଥା ଯଦି( ଆକାର(T) == ଆକାର(ଦଶମିକ) )
                    simdGEMM_kj_f32f64((ଅଚଳ ଦଶମିକ*)a_data, (ଅଚଳ ଦଶମିକ*)_b_data, b_step,
                                        (ଅଚଳ ଦଶମିକ*)_c_data, c_step1,
                                (ଦଶମିକ*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            ପାଇଁ( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                ଅଚଳ T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                ପାଇଁ( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                ଯଦି( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                ଅନ୍ୟଥା
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            ପାଇଁ( ; j < m; j++, c_data += c_step1 )
            {
                ଅଚଳ T* b = _b_data + j;
                WT s0(0);

                ପାଇଁ( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                ଯଦି( !c_data )
                    d_data[j] = T(s0);
                ଅନ୍ୟଥା
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ଅନ୍ୟଥା
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        ପାଇଁ( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            ଯଦି( a_buf )
            {
                ପାଇଁ( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            ପାଇଁ( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            ପାଇଁ( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                ଯଦି( ଆକାର(WT) == ଆକାର(ଦ୍ୱିଗୁଣ) )
                {
                    ଯଦି( ଆକାର(T) == ଆକାର(ଦ୍ୱିଗୁଣ) )
                    {
                        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ଅଚଳ ଦ୍ୱିଗୁଣ*)&al));
                        ପାଇଁ( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((ଅଚଳ ଦ୍ୱିଗୁଣ*)(d_buf + j));
                            v_float64 vb = vx_load((ଅଚଳ ଦ୍ୱିଗୁଣ*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((ଦ୍ୱିଗୁଣ*)(d_buf + j), vd);
                        }
                    }
                    ଅନ୍ୟଥା
                    {
                        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes_f32 = VTraits<v_float32>::vlanes();
                        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ଅଚଳ ଦ୍ୱିଗୁଣ*)&al));
                        ପାଇଁ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((ଅଚଳ ଦଶମିକ*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_ଉପର(vb);
                            v_float64 vd_lo = vx_load((ଅଚଳ ଦ୍ୱିଗୁଣ*)(d_buf + j));
                            v_float64 vd_hi = vx_load((ଅଚଳ ଦ୍ୱିଗୁଣ*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((ଦ୍ୱିଗୁଣ*)(d_buf + j), vd_lo);
                            v_store((ଦ୍ୱିଗୁଣ*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                ପାଇଁ( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            ଯଦି( !c_data )
                ପାଇଁ( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            ଅନ୍ୟଥା
                ପାଇଁ( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ setDoubleDataZero(WT* d_data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes)
{
    v_float64 vz = vx_setzero_f64();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
    ପାଇଁ( ; j <= m - vlanes; j += vlanes )
        v_store((ଦ୍ୱିଗୁଣ*)(d_data + j), vz);
    ପାଇଁ( ; j < m; j++ )
        d_data[j] = WT(0);
}

template<typename ST>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ simdMulAdd(ଅଚଳ ST* b, ଦ୍ୱିଗୁଣ* d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ a_val);

template<>
ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ simdMulAdd<ଦଶମିକ>(
    ଅଚଳ ଦଶମିକ* b, ଦ୍ୱିଗୁଣ* d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ a_val)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes_f32 = VTraits<v_float32>::vlanes();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
    ପାଇଁ( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_ଉପର(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    ପାଇଁ( ; j < m; j++ )
        d[j] += a_val * (ଦ୍ୱିଗୁଣ)b[j];
}

template<>
ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ simdMulAdd<ଦ୍ୱିଗୁଣ>(
    ଅଚଳ ଦ୍ୱିଗୁଣ* b, ଦ୍ୱିଗୁଣ* d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ a_val)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<v_float64>::vlanes();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
    ପାଇଁ( ; j <= m - stride2; j += stride2 )
    {
        v_float64 vd0 = vx_load(d + j);
        v_float64 vb0 = vx_load(b + j);
        v_float64 vd1 = vx_load(d + j + vlanes);
        v_float64 vb1 = vx_load(b + j + vlanes);
        vd0 = v_muladd(va, vb0, vd0);
        vd1 = v_muladd(va, vb1, vd1);
        v_store(d + j, vd0);
        v_store(d + j + vlanes, vd1);
    }
    ପାଇଁ( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    ପାଇଁ( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ simdBlockMul_kj(
    ଅଚଳ ST* a_data, ଅଚଳ ST* b_data, size_t b_step,
    ଦ୍ୱିଗୁଣ* d_data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ତାର୍କିକ do_acc)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<v_float64>::vlanes();
    ଯଦି( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (ଦ୍ୱିଗୁଣ)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> ସ୍ଥିର ଶୂନ୍ୟ
GEMMBlockMul( ଅଚଳ T* a_data, size_t a_step,
              ଅଚଳ T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k, n = a_size.width, m = d_size.width;
    ଅଚଳ T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_acc = flags & 16;

    a_step /= ଆକାର(a_data[0]);
    b_step /= ଆକାର(b_data[0]);
    d_step /= ଆକାର(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    ଯଦି( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    ଯଦି( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        ପାଇଁ( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            ଯଦି( a_buf )
            {
                ପାଇଁ( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            ପାଇଁ( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#ଯଦି CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ଯଦି( ଆକାର(WT) == ଆକାର(ଦ୍ୱିଗୁଣ) )
                {
                    ଯଦି( ଆକାର(T) == ଆକାର(ଦ୍ୱିଗୁଣ) )
                        s0 += (WT)simdDotProduct((ଅଚଳ ଦ୍ୱିଗୁଣ*)a_data, (ଅଚଳ ଦ୍ୱିଗୁଣ*)b_data, n, k);
                    ଅନ୍ୟଥା ଯଦି( ଆକାର(T) == ଆକାର(ଦଶମିକ) )
                        s0 += (WT)simdDotProduct_f32f64((ଅଚଳ ଦଶମିକ*)a_data, (ଅଚଳ ଦଶମିକ*)b_data, n, k);
                }
                ଅନ୍ୟଥା
#endif
                {
                    WT s1(0);
                    ପାଇଁ( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                ପାଇଁ( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            ଯଦି( a_buf )
            {
                ପାଇଁ( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ଯଦି( ଆକାର(WT) == ଆକାର(ଦ୍ୱିଗୁଣ) )
            {
                ଯଦି( ଆକାର(T) == ଆକାର(ଦ୍ୱିଗୁଣ) )
                    simdBlockMul_kj(
                        (ଅଚଳ ଦ୍ୱିଗୁଣ*)a_data, (ଅଚଳ ଦ୍ୱିଗୁଣ*)b_data, b_step,
                        (ଦ୍ୱିଗୁଣ*)d_data, n, m, do_acc != 0);
                ଅନ୍ୟଥା
                    simdBlockMul_kj(
                        (ଅଚଳ ଦଶମିକ*)a_data, (ଅଚଳ ଦଶମିକ*)b_data, b_step,
                        (ଦ୍ୱିଗୁଣ*)d_data, n, m, do_acc != 0);
            }
            ଅନ୍ୟଥା
#endif
            {
                ପାଇଁ( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ଅଚଳ T* b = b_data + j;

                    ଯଦି( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    ଅନ୍ୟଥା
                        s0 = s1 = s2 = s3 = WT(0);

                    ପାଇଁ( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                ପାଇଁ( ; j < m; j++ )
                {
                    ଅଚଳ T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    ପାଇଁ( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> ସ୍ଥିର ଶୂନ୍ୟ
GEMMStore( ଅଚଳ T* c_data, size_t c_step,
           ଅଚଳ WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    ଅଚଳ T* _c_data = c_data;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    size_t c_step0, c_step1;

    c_step /= ଆକାର(c_data[0]);
    d_buf_step /= ଆକାର(d_buf[0]);
    d_step /= ଆକାର(d_data[0]);

    ଯଦି( !c_data )
        c_step0 = c_step1 = 0;
    ଅନ୍ୟଥା ଯଦି( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    ଅନ୍ୟଥା
        c_step0 = 1, c_step1 = c_step;

    ପାଇଁ( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        ଯଦି( _c_data )
        {
            c_data = _c_data;
            j=0;
             #ଯଦି CV_ENABLE_UNROLLED
            ପାଇଁ(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_data[0]);
                t1 += beta*WT(c_data[c_step1]);
                d_data[j] = T(t0);
                d_data[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_data[c_step1*2]);
                t1 += beta*WT(c_data[c_step1*3]);
                d_data[j+2] = T(t0);
                d_data[j+3] = T(t1);
            }
            #endif
            ପାଇଁ( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        ଅନ୍ୟଥା
        {
            j = 0;
             #ଯଦି CV_ENABLE_UNROLLED
            ପାଇଁ( ; j <= d_size.width - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_data[j] = T(t0);
                d_data[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_data[j+2] = T(t0);
                d_data[j+3] = T(t1);
            }
            #endif
            ପାଇଁ( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*GEMMSingleMulFunc)( ଅଚଳ ଶୂନ୍ୟ* src1, size_t step1,
                   ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2, ଅଚଳ ଶୂନ୍ୟ* src3, size_t step3,
                   ଶୂନ୍ୟ* dst, size_t dststep, Size srcsize, Size dstsize,
                   ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags );

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*GEMMBlockMulFunc)( ଅଚଳ ଶୂନ୍ୟ* src1, size_t step1,
                   ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2, ଶୂନ୍ୟ* dst, size_t dststep,
                   Size srcsize, Size dstsize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags );

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*GEMMStoreFunc)( ଅଚଳ ଶୂନ୍ୟ* src1, size_t step1,
                   ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2, ଶୂନ୍ୟ* dst, size_t dststep,
                   Size dstsize, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags );

template<typename T, typename WT, ଶୂନ୍ୟ (*fn)(ଅଚଳ T*, size_t, ଅଚଳ T*, size_t, ଅଚଳ T*, size_t, T*, size_t, Size, Size, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ଶୂନ୍ୟ gemmSingleMulWrap( ଅଚଳ ଶୂନ୍ୟ* src1, size_t step1,
                   ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2, ଅଚଳ ଶୂନ୍ୟ* src3, size_t step3,
                   ଶୂନ୍ୟ* dst, size_t dststep, Size srcsize, Size dstsize,
                   ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    fn((ଅଚଳ T*)src1, step1, (ଅଚଳ T*)src2, step2, (ଅଚଳ T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, ଶୂନ୍ୟ (*fn)(ଅଚଳ T*, size_t, ଅଚଳ T*, size_t, WT*, size_t, Size, Size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ଶୂନ୍ୟ gemmBlockMulWrap( ଅଚଳ ଶୂନ୍ୟ* src1, size_t step1,
                   ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2, ଶୂନ୍ୟ* dst, size_t dststep,
                   Size srcsize, Size dstsize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    fn((ଅଚଳ T*)src1, step1, (ଅଚଳ T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, ଶୂନ୍ୟ (*fn)(ଅଚଳ T*, size_t, ଅଚଳ WT*, size_t, T*, size_t, Size, ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ଶୂନ୍ୟ gemmStoreWrap( ଅଚଳ ଶୂନ୍ୟ* src1, size_t step1,
                   ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2, ଶୂନ୍ୟ* dst, size_t dststep,
                   Size dstsize, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    fn((ଅଚଳ T*)src1, step1, (ଅଚଳ WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

ସ୍ଥିର ଶୂନ୍ୟ GEMMSingleMul_32f( ଅଚଳ ଦଶମିକ* a_data, size_t a_step,
              ଅଚଳ ଦଶମିକ* b_data, size_t b_step,
              ଅଚଳ ଦଶମିକ* c_data, size_t c_step,
              ଦଶମିକ* d_data, size_t d_step,
              Size a_size, Size d_size,
              ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMSingleMul<ଦଶମିକ,ଦ୍ୱିଗୁଣ>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

ସ୍ଥିର ଶୂନ୍ୟ GEMMSingleMul_64f( ଅଚଳ ଦ୍ୱିଗୁଣ* a_data, size_t a_step,
                              ଅଚଳ ଦ୍ୱିଗୁଣ* b_data, size_t b_step,
                              ଅଚଳ ଦ୍ୱିଗୁଣ* c_data, size_t c_step,
                              ଦ୍ୱିଗୁଣ* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMSingleMul<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


ସ୍ଥିର ଶୂନ୍ୟ GEMMSingleMul_32fc( ଅଚଳ Complexf* a_data, size_t a_step,
                              ଅଚଳ Complexf* b_data, size_t b_step,
                              ଅଚଳ Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

ସ୍ଥିର ଶୂନ୍ୟ GEMMSingleMul_64fc( ଅଚଳ Complexd* a_data, size_t a_step,
                              ଅଚଳ Complexd* b_data, size_t b_step,
                              ଅଚଳ Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

ସ୍ଥିର ଶୂନ୍ୟ GEMMBlockMul_32f( ଅଚଳ ଦଶମିକ* a_data, size_t a_step,
             ଅଚଳ ଦଶମିକ* b_data, size_t b_step,
             ଦ୍ୱିଗୁଣ* d_data, size_t d_step,
             Size a_size, Size d_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ସ୍ଥିର ଶୂନ୍ୟ GEMMBlockMul_64f( ଅଚଳ ଦ୍ୱିଗୁଣ* a_data, size_t a_step,
                             ଅଚଳ ଦ୍ୱିଗୁଣ* b_data, size_t b_step,
                             ଦ୍ୱିଗୁଣ* d_data, size_t d_step,
                             Size a_size, Size d_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ସ୍ଥିର ଶୂନ୍ୟ GEMMBlockMul_32fc( ଅଚଳ Complexf* a_data, size_t a_step,
                             ଅଚଳ Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ସ୍ଥିର ଶୂନ୍ୟ GEMMBlockMul_64fc( ଅଚଳ Complexd* a_data, size_t a_step,
                             ଅଚଳ Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ସ୍ଥିର ଶୂନ୍ୟ GEMMStore_32f( ଅଚଳ ଦଶମିକ* c_data, size_t c_step,
          ଅଚଳ ଦ୍ୱିଗୁଣ* d_buf, size_t d_buf_step,
          ଦଶମିକ* d_data, size_t d_step, Size d_size,
          ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ସ୍ଥିର ଶୂନ୍ୟ GEMMStore_64f( ଅଚଳ ଦ୍ୱିଗୁଣ* c_data, size_t c_step,
                      ଅଚଳ ଦ୍ୱିଗୁଣ* d_buf, size_t d_buf_step,
                      ଦ୍ୱିଗୁଣ* d_data, size_t d_step, Size d_size,
                      ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ସ୍ଥିର ଶୂନ୍ୟ GEMMStore_32fc( ଅଚଳ Complexf* c_data, size_t c_step,
                          ଅଚଳ Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ସ୍ଥିର ଶୂନ୍ୟ GEMMStore_64fc( ଅଚଳ Complexd* c_data, size_t c_step,
                          ଅଚଳ Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

ସ୍ଥିର ଶୂନ୍ୟ gemmImpl( Mat A, Mat B, ଦ୍ୱିଗୁଣ alpha,
           Mat C, ଦ୍ୱିଗୁଣ beta, Mat D, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    CV_INSTRUMENT_REGION();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_lin_size = 128;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_size = block_lin_size * block_lin_size;

    ସ୍ଥିର ଦ୍ୱିଗୁଣ zero[] = {0,0,0,0};
    ସ୍ଥିର ଦଶମିକ zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, len = 0, type = A.type();

    ବାଛ( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ସ୍ଥିତି 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        ଭାଙ୍ଗ;
    }

    ଯଦି( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        ଯଦି( type == CV_32F )
        {
            ଦଶମିକ* d = D.ptr<ଦଶମିକ>();
            ଅଚଳ ଦଶମିକ *a = A.ptr<ଦଶମିକ>(),
                        *b = B.ptr<ଦଶମିକ>(),
                        *c = (ଅଚଳ ଦଶମିକ*)C.data;
            size_t d_step = D.step/ଆକାର(d[0]),
                a_step = A.step/ଆକାର(a[0]),
                b_step = B.step/ଆକାର(b[0]),
                c_step = C.data ? C.step/ଆକାର(c[0]) : 0;

            ଯଦି( !c )
                c = zerof;

            ବାଛ( len )
            {
            ସ୍ଥିତି 2:
                ଯଦି( len == d_size.width && b != d )
                {
                    ପାଇଁ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ଦଶମିକ t0 = a[0]*b[0] + a[1]*b[b_step];
                        ଦଶମିକ t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (ଦଶମିକ)(t0*alpha + c[0]*beta);
                        d[1] = (ଦଶମିକ)(t1*alpha + c[1]*beta);
                    }
                }
                ଅନ୍ୟଥା ଯଦି( a != d )
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_step0 = 1;
                    ଯଦି( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ପାଇଁ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ଦଶମିକ t0 = a[0]*b[0] + a[1]*b[b_step];
                        ଦଶମିକ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (ଦଶମିକ)(t0*alpha + c[0]*beta);
                        d[d_step] = (ଦଶମିକ)(t1*alpha + c[c_step]*beta);
                    }
                }
                ଅନ୍ୟଥା
                    ଭାଙ୍ଗ;
                ଫେରନ୍ତୁ;
            ସ୍ଥିତି 3:
                ଯଦି( len == d_size.width && b != d )
                {
                    ପାଇଁ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ଦଶମିକ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ଦଶମିକ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        ଦଶମିକ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (ଦଶମିକ)(t0*alpha + c[0]*beta);
                        d[1] = (ଦଶମିକ)(t1*alpha + c[1]*beta);
                        d[2] = (ଦଶମିକ)(t2*alpha + c[2]*beta);
                    }
                }
                ଅନ୍ୟଥା ଯଦି( a != d )
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_step0 = 1;
                    ଯଦି( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ପାଇଁ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ଦଶମିକ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ଦଶମିକ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        ଦଶମିକ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (ଦଶମିକ)(t0*alpha + c[0]*beta);
                        d[d_step] = (ଦଶମିକ)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (ଦଶମିକ)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                ଅନ୍ୟଥା
                    ଭାଙ୍ଗ;
                ଫେରନ୍ତୁ;
            ସ୍ଥିତି 4:
                ଯଦି( len == d_size.width && b != d )
                {
                    ପାଇଁ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ଦଶମିକ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ଦଶମିକ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        ଦଶମିକ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        ଦଶମିକ t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (ଦଶମିକ)(t0*alpha + c[0]*beta);
                        d[1] = (ଦଶମିକ)(t1*alpha + c[1]*beta);
                        d[2] = (ଦଶମିକ)(t2*alpha + c[2]*beta);
                        d[3] = (ଦଶମିକ)(t3*alpha + c[3]*beta);
                    }
                }
                ଅନ୍ୟଥା ଯଦି( len <= 16 && a != d )
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_step0 = 1;
                    ଯଦି( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ପାଇଁ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ଦଶମିକ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ଦଶମିକ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        ଦଶମିକ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        ଦଶମିକ t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (ଦଶମିକ)(t0*alpha + c[0]*beta);
                        d[d_step] = (ଦଶମିକ)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (ଦଶମିକ)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (ଦଶମିକ)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                ଅନ୍ୟଥା
                    ଭାଙ୍ଗ;
                ଫେରନ୍ତୁ;
            }
        }

        ଯଦି( type == CV_64F )
        {
            ଦ୍ୱିଗୁଣ* d = D.ptr<ଦ୍ୱିଗୁଣ>();
            ଅଚଳ ଦ୍ୱିଗୁଣ *a = A.ptr<ଦ୍ୱିଗୁଣ>(),
                         *b = B.ptr<ଦ୍ୱିଗୁଣ>(),
                         *c = (ଅଚଳ ଦ୍ୱିଗୁଣ*)C.data;
            size_t d_step = D.step/ଆକାର(d[0]),
                a_step = A.step/ଆକାର(a[0]),
                b_step = B.step/ଆକାର(b[0]),
                c_step = C.data ? C.step/ଆକାର(c[0]) : 0;
            ଯଦି( !c )
                c = zero;

            ବାଛ( len )
            {
            ସ୍ଥିତି 2:
                ଯଦି( len == d_size.width && b != d )
                {
                    ପାଇଁ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ଦ୍ୱିଗୁଣ t0 = a[0]*b[0] + a[1]*b[b_step];
                        ଦ୍ୱିଗୁଣ t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                ଅନ୍ୟଥା ଯଦି( a != d )
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_step0 = 1;
                    ଯଦି( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ପାଇଁ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ଦ୍ୱିଗୁଣ t0 = a[0]*b[0] + a[1]*b[b_step];
                        ଦ୍ୱିଗୁଣ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                ଅନ୍ୟଥା
                    ଭାଙ୍ଗ;
                ଫେରନ୍ତୁ;
            ସ୍ଥିତି 3:
                ଯଦି( len == d_size.width && b != d )
                {
                    ପାଇଁ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ଦ୍ୱିଗୁଣ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ଦ୍ୱିଗୁଣ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        ଦ୍ୱିଗୁଣ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                ଅନ୍ୟଥା ଯଦି( a != d )
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_step0 = 1;
                    ଯଦି( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ପାଇଁ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ଦ୍ୱିଗୁଣ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ଦ୍ୱିଗୁଣ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        ଦ୍ୱିଗୁଣ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                ଅନ୍ୟଥା
                    ଭାଙ୍ଗ;
                ଫେରନ୍ତୁ;
            ସ୍ଥିତି 4:
                ଯଦି( len == d_size.width && b != d )
                {
                    ପାଇଁ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ଦ୍ୱିଗୁଣ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ଦ୍ୱିଗୁଣ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        ଦ୍ୱିଗୁଣ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        ଦ୍ୱିଗୁଣ t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                ଅନ୍ୟଥା ଯଦି( d_size.width <= 16 && a != d )
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_step0 = 1;
                    ଯଦି( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ପାଇଁ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ଦ୍ୱିଗୁଣ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ଦ୍ୱିଗୁଣ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        ଦ୍ୱିଗୁଣ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        ଦ୍ୱିଗୁଣ t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                ଅନ୍ୟଥା
                    ଭାଙ୍ଗ;
                ଫେରନ୍ତୁ;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ଅଚଳ uchar* Cdata = C.data;
    size_t Cstep = C.data ? (size_t)C.step : 0;
    AutoBuffer<uchar> buf;

    ଯଦି( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ଦଶମିକ, ଦ୍ୱିଗୁଣ, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<ଦଶମିକ, ଦ୍ୱିଗୁଣ, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<ଦଶମିକ, ଦ୍ୱିଗୁଣ, GEMMStore_32f>;
    }
    ଅନ୍ୟଥା ଯଦି( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, GEMMStore_64f>;
    }
    ଅନ୍ୟଥା ଯଦି( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    ଅନ୍ୟଥା
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    ଯଦି( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_step = d_size.width == 1 ? 0 : CV_ELEM_SIZE(type);
        flags |= GEMM_2_T;
    }

    /*if( (d_size.width | d_size.height | len) >= 16 && icvBLAS_GEMM_32f_p != 0 )
    {
        blas_func = type == CV_32FC1 ? (icvBLAS_GEMM_32f_t)icvBLAS_GEMM_32f_p :
                    type == CV_64FC1 ? (icvBLAS_GEMM_32f_t)icvBLAS_GEMM_64f_p :
                    type == CV_32FC2 ? (icvBLAS_GEMM_32f_t)icvBLAS_GEMM_32fc_p :
                    type == CV_64FC2 ? (icvBLAS_GEMM_32f_t)icvBLAS_GEMM_64fc_p : 0;
    }

    if( blas_func )
    {
        const char* transa = flags & GEMM_1_T ? "t" : "n";
        const char* transb = flags & GEMM_2_T ? "t" : "n";
        int lda, ldb, ldd;

        if( C->data.ptr )
        {
            if( C->data.ptr != D->data.ptr )
            {
                if( !(flags & GEMM_3_T) )
                    cvCopy( C, D );
                else
                    cvTranspose( C, D );
            }
        }

        if( CV_MAT_DEPTH(type) == CV_32F )
        {
            Complex32f _alpha, _beta;

            lda = A->step/sizeof(float);
            ldb = b_step/sizeof(float);
            ldd = D->step/sizeof(float);
            _alpha.re = (float)alpha;
            _alpha.im = 0;
            _beta.re = C->data.ptr ? (float)beta : 0;
            _beta.im = 0;
            if( CV_MAT_CN(type) == 2 )
                lda /= 2, ldb /= 2, ldd /= 2;

            blas_func( transb, transa, &d_size.width, &d_size.height, &len,
                   &_alpha, B->data.ptr, &ldb, A->data.ptr, &lda,
                   &_beta, D->data.ptr, &ldd );
        }
        else
        {
            CvComplex64f _alpha, _beta;

            lda = A->step/sizeof(double);
            ldb = b_step/sizeof(double);
            ldd = D->step/sizeof(double);
            _alpha.re = alpha;
            _alpha.im = 0;
            _beta.re = C->data.ptr ? beta : 0;
            _beta.im = 0;
            if( CV_MAT_CN(type) == 2 )
                lda /= 2, ldb /= 2, ldd /= 2;

            blas_func( transb, transa, &d_size.width, &d_size.height, &len,
                   &_alpha, B->data.ptr, &ldb, A->data.ptr, &lda,
                   &_beta, D->data.ptr, &ldd );
        }
    }
    else*/ ଯଦି( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_a_t = flags & GEMM_1_T;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_b_t = flags & GEMM_2_T;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_size = CV_ELEM_SIZE(type);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, k, di = 0, dj = 0, dk = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        ଯଦି( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        ଅନ୍ୟଥା
            a_step0 = elem_size, a_step1 = A.step;

        ଯଦି( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        ଅନ୍ୟଥା
            b_step0 = elem_size, b_step1 = b_step;

        ଯଦି( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        ଅନ୍ୟଥା ଯଦି( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        ଅନ୍ୟଥା
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        ଯଦି( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        ଯଦି( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        ଯଦି( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        ଯଦି( is_a_t )
            a_buf = b_buf + b_buf_size;

        ପାଇଁ( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            ଯଦି( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            ପାଇଁ( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                ଅଚଳ uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                ଯଦି( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                ଯଦି( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                ପାଇଁ( k = 0; k < len; k += dk )
                {
                    ଅଚଳ uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    ଅଚଳ uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    ଯଦି( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    ଯଦି( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    ଅନ୍ୟଥା
                        a_bl_size.width = di, a_bl_size.height = dk;

                    ଯଦି( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    ଯଦି( dj < d_size.width )
                    {
                        Size b_size;
                        ଯଦି( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        ଅନ୍ୟଥା
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    ଯଦି( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    ଅନ୍ୟଥା
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                ଯଦି( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>ଧାଡ଼ିଭିତରେ ସ୍ଥିର ଶୂନ୍ୟ
callGemmImpl(ଅଚଳ fptype *src1, size_t src1_step, ଅଚଳ fptype *src2, size_t src2_step, fptype alpha,
          ଅଚଳ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b_m, b_n, c_m, c_n, m_d;

    ଯଦି(flags & GEMM_2_T)
    {
        b_m = n_d;
        ଯଦି(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        ଅନ୍ୟଥା
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    ଅନ୍ୟଥା
    {
        b_n = n_d;
        ଯଦି(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        ଅନ୍ୟଥା
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    ଯଦି(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    ଅନ୍ୟଥା
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    ଯଦି(src1 != ଖାଲି)
        A = Mat(m_a, n_a, type, (ଶୂନ୍ୟ*)src1, src1_step);
    ଯଦି(src2 != ଖାଲି)
        B = Mat(b_m, b_n, type, (ଶୂନ୍ୟ*)src2, src2_step);
    ଯଦି(src3 != ଖାଲି && beta != 0.0)
        C = Mat(c_m, c_n, type, (ଶୂନ୍ୟ*)src3, src3_step);
    Mat D(m_d, n_d, type, (ଶୂନ୍ୟ*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

ଶୂନ୍ୟ gemm32f(ଅଚଳ ଦଶମିକ* src1, size_t src1_step, ଅଚଳ ଦଶମିକ* src2, size_t src2_step,
             ଦଶମିକ alpha, ଅଚଳ ଦଶମିକ* src3, size_t src3_step, ଦଶମିକ beta, ଦଶମିକ* dst, size_t dst_step,
             ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

ଶୂନ୍ୟ gemm64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, size_t src1_step, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, size_t src2_step,
             ଦ୍ୱିଗୁଣ alpha, ଅଚଳ ଦ୍ୱିଗୁଣ* src3, size_t src3_step, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ* dst, size_t dst_step,
             ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

ଶୂନ୍ୟ gemm32fc(ଅଚଳ ଦଶମିକ* src1, size_t src1_step, ଅଚଳ ଦଶମିକ* src2, size_t src2_step,
              ଦଶମିକ alpha, ଅଚଳ ଦଶମିକ* src3, size_t src3_step, ଦଶମିକ beta, ଦଶମିକ* dst, size_t dst_step,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

ଶୂନ୍ୟ gemm64fc(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, size_t src1_step, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, size_t src2_step,
              ଦ୍ୱିଗୁଣ alpha, ଅଚଳ ଦ୍ୱିଗୁଣ* src3, size_t src3_step, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ* dst, size_t dst_step,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> ସ୍ଥିର ଶୂନ୍ୟ
transform_( ଅଚଳ T* src, T* dst, ଅଚଳ WT* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x;

    ଯଦି( scn == 2 && dcn == 2 )
    {
        ପାଇଁ( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ଅନ୍ୟଥା ଯଦି( scn == 3 && dcn == 3 )
    {
        ପାଇଁ( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ଅନ୍ୟଥା ଯଦି( scn == 3 && dcn == 1 )
    {
        ପାଇଁ( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    ଅନ୍ୟଥା ଯଦି( scn == 4 && dcn == 4 )
    {
        ପାଇଁ( x = 0; x < len*4; x += 4 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]*v3 + m[4]);
            T t1 = saturate_cast<T>(m[5]*v0 + m[6]*v1 + m[7]*v2 + m[8]*v3 + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            t1 = saturate_cast<T>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ଅଚଳ WT* _m = m;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, k;
            ପାଇଁ( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                ପାଇଁ( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
transform_8u( ଅଚଳ uchar* src, uchar* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn )
{
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା BITS = 10, SCALE = 1 << BITS;
    ଅଚଳ ଦଶମିକ MAX_M = (ଦଶମିକ)(1 << (15 - BITS));

    ଯଦି( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nChannels = 3;

        ସଂଘ {
            short s[6];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା m32[] = {saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 2] * SCALE), saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 3] * SCALE),
                     saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 6] * SCALE), saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 7] * SCALE),
                     saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[10] * SCALE), saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ପାଇଁ (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
        {
            v_uint8 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_uint8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_uint16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_uint16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_add(v_add(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_add(v_add(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_add(v_add(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_add(v_add(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_add(v_add(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_add(v_add(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack_u(dbl, dbh), v_pack_u(dgl, dgh), v_pack_u(drl, drh));
        }
        m32[1] = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>((m[11] + 0.5f)*SCALE);
        ପାଇଁ( ; x < len * nChannels; x += nChannels )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
transform_16u( ଅଚଳ ushort* src, ushort* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn )
{
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି( scn == 3 && dcn == 3 )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
#ଯଦି CV_SIMD_WIDTH > 16
        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m1  = vx_setall_f32(m[ 1]);
        v_float32 m2  = vx_setall_f32(m[ 2]);
        v_float32 m3  = vx_setall_f32(m[ 3] - 32768.f);
        v_float32 m4  = vx_setall_f32(m[ 4]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m6  = vx_setall_f32(m[ 6]);
        v_float32 m7  = vx_setall_f32(m[ 7] - 32768.f);
        v_float32 m8  = vx_setall_f32(m[ 8]);
        v_float32 m9  = vx_setall_f32(m[ 9]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11] - 32768.f);
        v_int16 delta = vx_setall_s16(-32768);
        ପାଇଁ (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
        {
            v_uint16 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_uint32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_add_wrap(v_pack(v_round(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m2,  m3)))),
                                   v_round(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m2,  m3))))), delta);
            dg = v_add_wrap(v_pack(v_round(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m6,  m7)))),
                                   v_round(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m6,  m7))))), delta);
            dr = v_add_wrap(v_pack(v_round(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)), m10, m11)))),
                                   v_round(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)), m10, m11))))), delta);
            v_store_interleave(dst + x, v_reinterpret_as_u16(db), v_reinterpret_as_u16(dg), v_reinterpret_as_u16(dr));
        }
#endif
#ଯଦି CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_ବାମ<1>(_m0l);
        v_float32x4 _m1h = v_rotate_ବାମ<1>(_m1l);
        v_float32x4 _m2h = v_rotate_ବାମ<1>(_m2l);
        v_float32x4 _m3h = v_rotate_ବାମ<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        ପାଇଁ( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_ଡାହାଣ<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        ପାଇଁ( ; x < len * 3; x += 3 )
        {
            ଦଶମିକ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
transform_32f( ଅଚଳ ଦଶମିକ* src, ଦଶମିକ* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
    ଯଦି( scn == 3 && dcn == 3 )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx[VTraits<v_float32>::max_nlanes/2];
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        ଦଶମିକ _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        ପାଇଁ( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        ପାଇଁ( ; x < len*3; x += 3 )
        {
            ଦଶମିକ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            ଦଶମିକ t0 = saturate_cast<ଦଶମିକ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            ଦଶମିକ t1 = saturate_cast<ଦଶମିକ>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            ଦଶମିକ t2 = saturate_cast<ଦଶମିକ>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }

    ଯଦି( scn == 4 && dcn == 4 )
    {
#ଯଦି CV_SIMD_WIDTH > 16
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx[VTraits<v_float32>::max_nlanes/4];
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        ଦଶମିକ _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        ପାଇଁ( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#ଯଦି CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        ପାଇଁ( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#ଅନ୍ୟଥା // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        ପାଇଁ( ; x < len*4; x += 4 )
        {
            ଦଶମିକ v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            ଦଶମିକ t0 = saturate_cast<ଦଶମିକ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            ଦଶମିକ t1 = saturate_cast<ଦଶମିକ>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            ଦଶମିକ t2 = saturate_cast<ଦଶମିକ>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            ଦଶମିକ t3 = saturate_cast<ଦଶମିକ>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


ସ୍ଥିର ଶୂନ୍ୟ
transform_8s(ଅଚଳ schar* src, schar* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା BITS = 10, SCALE = 1 << BITS;
    ଅଚଳ ଦଶମିକ MAX_M = (ଦଶମିକ)(1 << (15 - BITS));

    ଯଦି( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nChannels = 3;

        ସଂଘ {
            short s[6];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା m32[] = {saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 2] * SCALE), saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 3] * SCALE),
                     saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 6] * SCALE), saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 7] * SCALE),
                     saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[10] * SCALE), saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ପାଇଁ (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
        {
            v_int8 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_int8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_int16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_int16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_add(v_add(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_add(v_add(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_add(v_add(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_add(v_add(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_add(v_add(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<BITS>(v_add(v_add(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_add(v_add(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack(dbl, dbh), v_pack(dgl, dgh), v_pack(drl, drh));
        }
        m32[1] = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>((m[11] + 0.5f)*SCALE);
        ପାଇଁ( ; x < len * nChannels; x += nChannels )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
transform_16s(ଅଚଳ short* src, short* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି (scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m1  = vx_setall_f32(m[ 1]);
        v_float32 m2  = vx_setall_f32(m[ 2]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m4  = vx_setall_f32(m[ 4]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m6  = vx_setall_f32(m[ 6]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m8  = vx_setall_f32(m[ 8]);
        v_float32 m9  = vx_setall_f32(m[ 9]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
        {
            v_int16 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_int32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_pack(v_round(v_muladd(v_cvt_f32(bl), m0, v_muladd(v_cvt_f32(gl), m1, v_muladd(v_cvt_f32(rl),  m2,  m3)))),
                        v_round(v_muladd(v_cvt_f32(bh), m0, v_muladd(v_cvt_f32(gh), m1, v_muladd(v_cvt_f32(rh),  m2,  m3)))));
            dg = v_pack(v_round(v_muladd(v_cvt_f32(bl), m4, v_muladd(v_cvt_f32(gl), m5, v_muladd(v_cvt_f32(rl),  m6,  m7)))),
                        v_round(v_muladd(v_cvt_f32(bh), m4, v_muladd(v_cvt_f32(gh), m5, v_muladd(v_cvt_f32(rh),  m6,  m7)))));
            dr = v_pack(v_round(v_muladd(v_cvt_f32(bl), m8, v_muladd(v_cvt_f32(gl), m9, v_muladd(v_cvt_f32(rl), m10, m11)))),
                        v_round(v_muladd(v_cvt_f32(bh), m8, v_muladd(v_cvt_f32(gh), m9, v_muladd(v_cvt_f32(rh), m10, m11)))));
            v_store_interleave(dst + x, db, dg, dr);
        }
        ପାଇଁ( ; x < len * 3; x += 3 )
        {
            ଦଶମିକ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
transform_32s(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dst, ଅଚଳ ଦ୍ୱିଗୁଣ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ଯଦି( scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m1  = vx_setall_f64(m[ 1]);
        v_float64 m2  = vx_setall_f64(m[ 2]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m4  = vx_setall_f64(m[ 4]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m6  = vx_setall_f64(m[ 6]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m8  = vx_setall_f64(m[ 8]);
        v_float64 m9  = vx_setall_f64(m[ 9]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ଉପର(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ଉପର(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ଉପର(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ଦ୍ୱିଗୁଣ b = src[x], g = src[x + 1], r = src[x + 2];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା db = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dg = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dr = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
transform_64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଦ୍ୱିଗୁଣ* dst, ଅଚଳ ଦ୍ୱିଗୁଣ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ଯଦି( scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m1  = vx_setall_f64(m[ 1]);
        v_float64 m2  = vx_setall_f64(m[ 2]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m4  = vx_setall_f64(m[ 4]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m6  = vx_setall_f64(m[ 6]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m8  = vx_setall_f64(m[ 8]);
        v_float64 m9  = vx_setall_f64(m[ 9]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ଦ୍ୱିଗୁଣ b = src[x], g = src[x + 1], r = src[x + 2];
            ଦ୍ୱିଗୁଣ db = saturate_cast<ଦ୍ୱିଗୁଣ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ଦ୍ୱିଗୁଣ dg = saturate_cast<ଦ୍ୱିଗୁଣ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ଦ୍ୱିଗୁଣ dr = saturate_cast<ଦ୍ୱିଗୁଣ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> ସ୍ଥିର ଶୂନ୍ୟ
diagtransform_( ଅଚଳ T* src, T* dst, ଅଚଳ WT* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x;

    ଯଦି( cn == 2 )
    {
        ପାଇଁ( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ଅନ୍ୟଥା ଯଦି( cn == 3 )
    {
        ପାଇଁ( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ଅନ୍ୟଥା ଯଦି( cn == 4 )
    {
        ପାଇଁ( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ଅଚଳ WT* _m = m;
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
diagtransform_8u(ଅଚଳ uchar* src, uchar* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି (scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
            v_uint8 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_uint16 b_u16_l, g_u16_l, r_u16_l;
            v_uint16 b_u16_h, g_u16_h, r_u16_h;
            v_expand(b, b_u16_l, b_u16_h);
            v_expand(g, g_u16_l, g_u16_h);
            v_expand(r, r_u16_l, r_u16_h);

            v_uint32 b_u32_l0, g_u32_l0, r_u32_l0;
            v_uint32 b_u32_l1, g_u32_l1, r_u32_l1;
            v_uint32 b_u32_h0, g_u32_h0, r_u32_h0;
            v_uint32 b_u32_h1, g_u32_h1, r_u32_h1;
            v_expand(b_u16_l, b_u32_l0, b_u32_l1);
            v_expand(b_u16_h, b_u32_h0, b_u32_h1);
            v_expand(g_u16_l, g_u32_l0, g_u32_l1);
            v_expand(g_u16_h, g_u32_h0, g_u32_h1);
            v_expand(r_u16_l, r_u32_l0, r_u32_l1);
            v_expand(r_u16_h, r_u32_h0, r_u32_h1);

            v_float32 db_f32_l0 = v_fma( m0, v_cvt_f32(v_reinterpret_as_s32(b_u32_l0)),  m3);
            v_float32 db_f32_l1 = v_fma( m0, v_cvt_f32(v_reinterpret_as_s32(b_u32_l1)),  m3);
            v_float32 db_f32_h0 = v_fma( m0, v_cvt_f32(v_reinterpret_as_s32(b_u32_h0)),  m3);
            v_float32 db_f32_h1 = v_fma( m0, v_cvt_f32(v_reinterpret_as_s32(b_u32_h1)),  m3);

            v_float32 dg_f32_l0 = v_fma( m5, v_cvt_f32(v_reinterpret_as_s32(g_u32_l0)),  m7);
            v_float32 dg_f32_l1 = v_fma( m5, v_cvt_f32(v_reinterpret_as_s32(g_u32_l1)),  m7);
            v_float32 dg_f32_h0 = v_fma( m5, v_cvt_f32(v_reinterpret_as_s32(g_u32_h0)),  m7);
            v_float32 dg_f32_h1 = v_fma( m5, v_cvt_f32(v_reinterpret_as_s32(g_u32_h1)),  m7);

            v_float32 dr_f32_l0 = v_fma(m10, v_cvt_f32(v_reinterpret_as_s32(r_u32_l0)), m11);
            v_float32 dr_f32_l1 = v_fma(m10, v_cvt_f32(v_reinterpret_as_s32(r_u32_l1)), m11);
            v_float32 dr_f32_h0 = v_fma(m10, v_cvt_f32(v_reinterpret_as_s32(r_u32_h0)), m11);
            v_float32 dr_f32_h1 = v_fma(m10, v_cvt_f32(v_reinterpret_as_s32(r_u32_h1)), m11);

            v_store_interleave(dst + x,
                               v_pack(v_pack_u(v_round(db_f32_l0), v_round(db_f32_l1)), v_pack_u(v_round(db_f32_h0), v_round(db_f32_h1))),
                               v_pack(v_pack_u(v_round(dg_f32_l0), v_round(dg_f32_l1)), v_pack_u(v_round(dg_f32_h0), v_round(dg_f32_h1))),
                               v_pack(v_pack_u(v_round(dr_f32_l0), v_round(dr_f32_l1)), v_pack_u(v_round(dr_f32_h0), v_round(dr_f32_h1))));
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
diagtransform_8s(ଅଚଳ schar* src, schar* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
    #ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି (scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
            v_int8 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_int16 b_u16_l, g_u16_l, r_u16_l;
            v_int16 b_u16_h, g_u16_h, r_u16_h;
            v_expand(b, b_u16_l, b_u16_h);
            v_expand(g, g_u16_l, g_u16_h);
            v_expand(r, r_u16_l, r_u16_h);

            v_int32 b_u32_l0, g_u32_l0, r_u32_l0;
            v_int32 b_u32_l1, g_u32_l1, r_u32_l1;
            v_int32 b_u32_h0, g_u32_h0, r_u32_h0;
            v_int32 b_u32_h1, g_u32_h1, r_u32_h1;
            v_expand(b_u16_l, b_u32_l0, b_u32_l1);
            v_expand(b_u16_h, b_u32_h0, b_u32_h1);
            v_expand(g_u16_l, g_u32_l0, g_u32_l1);
            v_expand(g_u16_h, g_u32_h0, g_u32_h1);
            v_expand(r_u16_l, r_u32_l0, r_u32_l1);
            v_expand(r_u16_h, r_u32_h0, r_u32_h1);

            v_float32 db_f32_l0 = v_fma( m0, v_cvt_f32(b_u32_l0),  m3);
            v_float32 db_f32_l1 = v_fma( m0, v_cvt_f32(b_u32_l1),  m3);
            v_float32 db_f32_h0 = v_fma( m0, v_cvt_f32(b_u32_h0),  m3);
            v_float32 db_f32_h1 = v_fma( m0, v_cvt_f32(b_u32_h1),  m3);

            v_float32 dg_f32_l0 = v_fma( m5, v_cvt_f32(g_u32_l0),  m7);
            v_float32 dg_f32_l1 = v_fma( m5, v_cvt_f32(g_u32_l1),  m7);
            v_float32 dg_f32_h0 = v_fma( m5, v_cvt_f32(g_u32_h0),  m7);
            v_float32 dg_f32_h1 = v_fma( m5, v_cvt_f32(g_u32_h1),  m7);

            v_float32 dr_f32_l0 = v_fma(m10, v_cvt_f32(r_u32_l0), m11);
            v_float32 dr_f32_l1 = v_fma(m10, v_cvt_f32(r_u32_l1), m11);
            v_float32 dr_f32_h0 = v_fma(m10, v_cvt_f32(r_u32_h0), m11);
            v_float32 dr_f32_h1 = v_fma(m10, v_cvt_f32(r_u32_h1), m11);

            v_store_interleave(dst + x,
                               v_pack(v_pack(v_round(db_f32_l0), v_round(db_f32_l1)), v_pack(v_round(db_f32_h0), v_round(db_f32_h1))),
                               v_pack(v_pack(v_round(dg_f32_l0), v_round(dg_f32_l1)), v_pack(v_round(dg_f32_h0), v_round(dg_f32_h1))),
                               v_pack(v_pack(v_round(dr_f32_l0), v_round(dr_f32_l1)), v_pack(v_round(dr_f32_h0), v_round(dr_f32_h1))));
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
diagtransform_16u(ଅଚଳ ushort* src, ushort* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି (scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
            v_uint16 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_uint32 b_u32_l, g_u32_l, r_u32_l;
            v_uint32 b_u32_h, g_u32_h, r_u32_h;
            v_expand(b, b_u32_l, b_u32_h);
            v_expand(g, g_u32_l, g_u32_h);
            v_expand(r, r_u32_l, r_u32_h);

            v_float32 db_f32_l = v_fma( m0, v_cvt_f32(v_reinterpret_as_s32(b_u32_l)),  m3);
            v_float32 db_f32_h = v_fma( m0, v_cvt_f32(v_reinterpret_as_s32(b_u32_h)),  m3);

            v_float32 dg_f32_l = v_fma( m5, v_cvt_f32(v_reinterpret_as_s32(g_u32_l)),  m7);
            v_float32 dg_f32_h = v_fma( m5, v_cvt_f32(v_reinterpret_as_s32(g_u32_h)),  m7);

            v_float32 dr_f32_l = v_fma(m10, v_cvt_f32(v_reinterpret_as_s32(r_u32_l)), m11);
            v_float32 dr_f32_h = v_fma(m10, v_cvt_f32(v_reinterpret_as_s32(r_u32_h)), m11);

            v_store_interleave(dst + x,
                               v_pack_u(v_round(db_f32_l), v_round(db_f32_h)),
                               v_pack_u(v_round(dg_f32_l), v_round(dg_f32_h)),
                               v_pack_u(v_round(dr_f32_l), v_round(dr_f32_h)));
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
diagtransform_16s(ଅଚଳ short* src, short* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି (scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
            v_int16 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_int32 b_u32_l, g_u32_l, r_u32_l;
            v_int32 b_u32_h, g_u32_h, r_u32_h;
            v_expand(b, b_u32_l, b_u32_h);
            v_expand(g, g_u32_l, g_u32_h);
            v_expand(r, r_u32_l, r_u32_h);

            v_float32 db_f32_l = v_fma( m0, v_cvt_f32(b_u32_l),  m3);
            v_float32 db_f32_h = v_fma( m0, v_cvt_f32(b_u32_h),  m3);

            v_float32 dg_f32_l = v_fma( m5, v_cvt_f32(g_u32_l),  m7);
            v_float32 dg_f32_h = v_fma( m5, v_cvt_f32(g_u32_h),  m7);

            v_float32 dr_f32_l = v_fma(m10, v_cvt_f32(r_u32_l), m11);
            v_float32 dr_f32_h = v_fma(m10, v_cvt_f32(r_u32_h), m11);

            v_store_interleave(dst + x,
                               v_pack(v_round(db_f32_l), v_round(db_f32_h)),
                               v_pack(v_round(dg_f32_l), v_round(dg_f32_h)),
                               v_pack(v_round(dr_f32_l), v_round(dr_f32_h)));
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
diagtransform_32s(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dst, ଅଚଳ ଦ୍ୱିଗୁଣ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ଯଦି (scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ଉପର(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ଉପର(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ଉପର(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = src[x], g = src[x + 1], r = src[x + 2];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା db = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 0] * b + m[ 3]);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dg = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[ 5] * g + m[ 7]);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dr = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
diagtransform_32f(ଅଚଳ ଦଶମିକ* src, ଦଶମିକ* dst, ଅଚଳ ଦଶମିକ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ଯଦି (scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ଦଶମିକ b = src[x], g = src[x + 1], r = src[x + 2];
            ଦଶମିକ db = saturate_cast<ଦଶମିକ>(m[ 0] * b + m[ 3]);
            ଦଶମିକ dg = saturate_cast<ଦଶମିକ>(m[ 5] * g + m[ 7]);
            ଦଶମିକ dr = saturate_cast<ଦଶମିକ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ସ୍ଥିର ଶୂନ୍ୟ
diagtransform_64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src, ଦ୍ୱିଗୁଣ* dst, ଅଚଳ ଦ୍ୱିଗୁଣ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ଯଦି (scn == 3 && dcn == 3) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ପାଇଁ (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        ପାଇଁ (; x < len * 3; x += 3) {
            ଦ୍ୱିଗୁଣ b = src[x], g = src[x + 1], r = src[x + 2];
            ଦ୍ୱିଗୁଣ db = saturate_cast<ଦ୍ୱିଗୁଣ>(m[ 0] * b + m[ 3]);
            ଦ୍ୱିଗୁଣ dg = saturate_cast<ଦ୍ୱିଗୁଣ>(m[ 5] * g + m[ 7]);
            ଦ୍ୱିଗୁଣ dr = saturate_cast<ଦ୍ୱିଗୁଣ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ଫେରନ୍ତୁ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, ଶୂନ୍ୟ (*fn)(ଅଚଳ T*, T*, ଅଚଳ WT*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ଶୂନ୍ୟ transformWrap(ଅଚଳ ଶୂନ୍ୟ* src, ଶୂନ୍ୟ* dst, ଅଚଳ ଶୂନ୍ୟ* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn)
{
    fn((ଅଚଳ T*)src, (T*)dst, (ଅଚଳ WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, ଦଶମିକ, transform_8u>, transformWrap<schar, ଦଶମିକ, transform_8s>, transformWrap<ushort, ଦଶମିକ, transform_16u>,
        transformWrap<short, ଦଶମିକ, transform_16s>, transformWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, transform_32s>, transformWrap<ଦଶମିକ, ଦଶମିକ, transform_32f>,
        transformWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, transform_64f>, 0
    };

    ଫେରନ୍ତୁ transformTab[depth];
}

TransformFunc getDiagTransformFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, ଦଶମିକ, diagtransform_8u>, transformWrap<schar, ଦଶମିକ, diagtransform_8s>, transformWrap<ushort, ଦଶମିକ, diagtransform_16u>,
        transformWrap<short, ଦଶମିକ, diagtransform_16s>, transformWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦ୍ୱିଗୁଣ, diagtransform_32s>, transformWrap<ଦଶମିକ, ଦଶମିକ, diagtransform_32f>,
        transformWrap<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, diagtransform_64f>, 0
    };

    ଫେରନ୍ତୁ diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ
perspectiveTransform_( ଅଚଳ ଶୂନ୍ୟ* _src, ଶୂନ୍ୟ* _dst, ଅଚଳ ଶୂନ୍ୟ* _m_ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn )
{
    ଅଚଳ T* src = (ଅଚଳ T*)_src;
    T* dst = (T*)_dst;
    ଅଚଳ ଦ୍ୱିଗୁଣ* m = (ଅଚଳ ଦ୍ୱିଗୁଣ*)_m_ptr;
    ଅଚଳ ଦ୍ୱିଗୁଣ eps = FLT_EPSILON;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

    ଯଦି( scn == 2 && dcn == 2 )
    {
        ପାଇଁ( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            ଦ୍ୱିଗୁଣ w = x*m[6] + y*m[7] + m[8];

            ଯଦି( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            ଅନ୍ୟଥା
                dst[i] = dst[i+1] = (T)0;
        }
    }
    ଅନ୍ୟଥା ଯଦି( scn == 3 && dcn == 3 )
    {
        ପାଇଁ( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            ଦ୍ୱିଗୁଣ w = x*m[12] + y*m[13] + z*m[14] + m[15];

            ଯଦି( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            ଅନ୍ୟଥା
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    ଅନ୍ୟଥା ଯଦି( scn == 3 && dcn == 2 )
    {
        ପାଇଁ( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            ଦ୍ୱିଗୁଣ w = x*m[8] + y*m[9] + z*m[10] + m[11];

            ଯଦି( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            ଅନ୍ୟଥା
                dst[0] = dst[1] = (T)0;
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ଅଚଳ ଦ୍ୱିଗୁଣ* _m = m + dcn*(scn + 1);
            ଦ୍ୱିଗୁଣ w = _m[scn];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, k;
            ପାଇଁ( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            ଯଦି( fabs(w) > eps )
            {
                _m = m;
                ପାଇଁ( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    ଦ୍ୱିଗୁଣ s = _m[scn];
                    ପାଇଁ( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            ଅନ୍ୟଥା
                ପାଇଁ( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଯଦି (depth == CV_32F)
        ଫେରନ୍ତୁ perspectiveTransform_<ଦଶମିକ>;
    ଯଦି (depth == CV_64F)
        ଫେରନ୍ତୁ perspectiveTransform_<ଦ୍ୱିଗୁଣ>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

ସ୍ଥିର ଶୂନ୍ୟ scaleAdd_32f(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, ଶୂନ୍ୟ* _dst,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ ଶୂନ୍ୟ* _alpha)
{
    ଅଚଳ ଦଶମିକ* src1 = (ଅଚଳ ଦଶମିକ*)_src1;
    ଅଚଳ ଦଶମିକ* src2 = (ଅଚଳ ଦଶମିକ*)_src2;
    ଦଶମିକ* dst = (ଦଶମିକ*)_dst;
    ଦଶମିକ alpha = *(ଅଚଳ ଦଶମିକ*)_alpha;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cWidth = VTraits<v_float32>::vlanes();
    ପାଇଁ (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    ପାଇଁ (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


ସ୍ଥିର ଶୂନ୍ୟ scaleAdd_64f(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, ଶୂନ୍ୟ* _dst,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ ଶୂନ୍ୟ* _alpha)
{
    ଅଚଳ ଦ୍ୱିଗୁଣ* src1 = (ଅଚଳ ଦ୍ୱିଗୁଣ*)_src1;
    ଅଚଳ ଦ୍ୱିଗୁଣ* src2 = (ଅଚଳ ଦ୍ୱିଗୁଣ*)_src2;
    ଦ୍ୱିଗୁଣ* dst = (ଦ୍ୱିଗୁଣ*)_dst;
    ଦ୍ୱିଗୁଣ alpha = *(ଅଚଳ ଦ୍ୱିଗୁଣ*)_alpha;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cWidth = VTraits<v_float64>::vlanes();
    ପାଇଁ (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    ପାଇଁ (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଯଦି (depth == CV_32F)
        ଫେରନ୍ତୁ scaleAdd_32f;
    ଯଦି (depth == CV_64F)
        ଫେରନ୍ତୁ scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଦ୍ୱିଗୁଣ MahalanobisImpl(ଅଚଳ Mat& v1, ଅଚଳ Mat& v2, ଅଚଳ Mat& icovar, ଦ୍ୱିଗୁଣ *diff_buffer /*[len]*/, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    ଦ୍ୱିଗୁଣ result = 0;

    sz.width *= v1.channels();
    ଯଦି (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        ଅଚଳ T* src1 = v1.ptr<T>();
        ଅଚଳ T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/ଆକାର(src1[0]);
        size_t step2 = v2.step/ଆକାର(src2[0]);
        ଦ୍ୱିଗୁଣ* diff = diff_buffer;
        ଅଚଳ T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/ଆକାର(mat[0]);

        ପାଇଁ (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++, mat += matstep)
        {
            ଦ୍ୱିଗୁଣ row_sum = 0;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
#ଯଦି CV_ENABLE_UNROLLED
            ପାଇଁ(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            ପାଇଁ (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    ଫେରନ୍ତୁ result;
}

MahalanobisImplFunc getMahalanobisImplFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଯଦି (depth == CV_32F)
        ଫେରନ୍ତୁ (MahalanobisImplFunc)MahalanobisImpl<ଦଶମିକ>;
    ଯଦି (depth == CV_64F)
        ଫେରନ୍ତୁ (MahalanobisImplFunc)MahalanobisImpl<ଦ୍ୱିଗୁଣ>;
    CV_Assert(0 && "Not supported");
}


#ଯଦି !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> ସ୍ଥିର ଶୂନ୍ୟ
MulTransposedR(ଅଚଳ Mat& srcmat, ଅଚଳ Mat& dstmat, ଅଚଳ Mat& deltamat, ଦ୍ୱିଗୁଣ scale)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ଅଚଳ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ଅଚଳ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/ଆକାର(src[0]);
    size_t dststep = dstmat.step/ଆକାର(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/ଆକାର(delta[0]) : 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା buf_size = size.height*ଆକାର(dT);
    AutoBuffer<uchar> buf;

    ଯଦି( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    ଯଦି( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        ପାଇଁ( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#ଯଦି CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    ଯଦି( !delta )
        ପାଇଁ( i = 0; i < size.width; i++, tdst += dststep )
        {
            ପାଇଁ( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            ପାଇଁ( j = i; j <= size.width - 4; j += 4 )
            {
#ଯଦି CV_SIMD128_64F
                ଯଦି (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ଅଚଳ ଦ୍ୱିଗୁଣ *tsrc = (ଦ୍ୱିଗୁଣ*)(src + j);

                    ପାଇଁ( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((ଦ୍ୱିଗୁଣ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((ଦ୍ୱିଗୁଣ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ଦ୍ୱିଗୁଣ*)(tdst+j+2), v_mul(s1, v_scale));
                } ଅନ୍ୟଥା
#endif
                {
                    ଦ୍ୱିଗୁଣ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ଅଚଳ sT *tsrc = src + j;

                    ପାଇଁ( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        ଦ୍ୱିଗୁଣ a = col_buf[k];
                        s0 += a * tsrc[0];
                        s1 += a * tsrc[1];
                        s2 += a * tsrc[2];
                        s3 += a * tsrc[3];
                    }

                    tdst[j] = (dT)(s0*scale);
                    tdst[j+1] = (dT)(s1*scale);
                    tdst[j+2] = (dT)(s2*scale);
                    tdst[j+3] = (dT)(s3*scale);
                }
            }

            ପାଇଁ( ; j < size.width; j++ )
            {
                ଦ୍ୱିଗୁଣ s0 = 0;
                ଅଚଳ sT *tsrc = src + j;

                ପାଇଁ( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (ଦ୍ୱିଗୁଣ)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    ଅନ୍ୟଥା
        ପାଇଁ( i = 0; i < size.width; i++, tdst += dststep )
        {
            ଯଦି( !delta_buf )
                ପାଇଁ( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            ଅନ୍ୟଥା
                ପାଇଁ( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            ପାଇଁ( j = i; j <= size.width - 4; j += 4 )
            {
#ଯଦି CV_SIMD128_64F
                ଯଦି (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ଅଚଳ ଦ୍ୱିଗୁଣ *tsrc = (ଦ୍ୱିଗୁଣ*)(src + j);
                    ଅଚଳ ଦ୍ୱିଗୁଣ *d = (ଦ୍ୱିଗୁଣ*)(delta_buf ? delta_buf : delta + j);

                    ପାଇଁ( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((ଦ୍ୱିଗୁଣ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((ଦ୍ୱିଗୁଣ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ଦ୍ୱିଗୁଣ*)(tdst+j+2), v_mul(s1, v_scale));
                }
                ଅନ୍ୟଥା
#endif

                {
                    ଦ୍ୱିଗୁଣ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ଅଚଳ sT *tsrc = src + j;
                    ଅଚଳ dT *d = delta_buf ? delta_buf : delta + j;

                    ପାଇଁ( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        ଦ୍ୱିଗୁଣ a = col_buf[k];
                        s0 += a * (tsrc[0] - d[0]);
                        s1 += a * (tsrc[1] - d[1]);
                        s2 += a * (tsrc[2] - d[2]);
                        s3 += a * (tsrc[3] - d[3]);
                    }

                    tdst[j] = (dT)(s0*scale);
                    tdst[j+1] = (dT)(s1*scale);
                    tdst[j+2] = (dT)(s2*scale);
                    tdst[j+3] = (dT)(s3*scale);
                }
            }

            ପାଇଁ( ; j < size.width; j++ )
            {
                ଦ୍ୱିଗୁଣ s0 = 0;
                ଅଚଳ sT *tsrc = src + j;
                ଅଚଳ dT *d = delta_buf ? delta_buf : delta + j;

                ପାଇଁ( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (ଦ୍ୱିଗୁଣ)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> ସ୍ଥିର ଶୂନ୍ୟ
MulTransposedL(ଅଚଳ Mat& srcmat, ଅଚଳ Mat& dstmat, ଅଚଳ Mat& deltamat, ଦ୍ୱିଗୁଣ scale)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ଅଚଳ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ଅଚଳ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/ଆକାର(src[0]);
    size_t dststep = dstmat.step/ଆକାର(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/ଆକାର(delta[0]) : 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    ଯଦି( !delta )
        ପାଇଁ( i = 0; i < size.height; i++, tdst += dststep )
            ପାଇଁ( j = i; j < size.height; j++ )
            {
                ଦ୍ୱିଗୁଣ s = 0;
                ଅଚଳ sT *tsrc1 = src + i*srcstep;
                ଅଚଳ sT *tsrc2 = src + j*srcstep;
#ଯଦି CV_SIMD128_64F
                ଯଦି (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ଅଚଳ ଦ୍ୱିଗୁଣ *v_tsrc1 = (ଦ୍ୱିଗୁଣ *)(tsrc1);
                    ଅଚଳ ଦ୍ୱିଗୁଣ *v_tsrc2 = (ଦ୍ୱିଗୁଣ *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    ପାଇଁ( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                ଅନ୍ୟଥା
#endif
                {
                    ପାଇଁ( k = 0; k <= size.width - 4; k += 4 )
                        s += (ଦ୍ୱିଗୁଣ)tsrc1[k]*tsrc2[k] + (ଦ୍ୱିଗୁଣ)tsrc1[k+1]*tsrc2[k+1] +
                             (ଦ୍ୱିଗୁଣ)tsrc1[k+2]*tsrc2[k+2] + (ଦ୍ୱିଗୁଣ)tsrc1[k+3]*tsrc2[k+3];
                }
                ପାଇଁ( ; k < size.width; k++ )
                    s += (ଦ୍ୱିଗୁଣ)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    ଅନ୍ୟଥା
    {
        dT delta_buf[4];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*ଆକାର(dT));
        dT* row_buf = (dT*)buf.data();

        ପାଇଁ( i = 0; i < size.height; i++, tdst += dststep )
        {
            ଅଚଳ sT *tsrc1 = src + i*srcstep;
            ଅଚଳ dT *tdelta1 = delta + i*deltastep;

            ଯଦି( delta_cols < size.width )
                ପାଇଁ( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            ଅନ୍ୟଥା
                ପାଇଁ( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            ପାଇଁ( j = i; j < size.height; j++ )
            {
                ଦ୍ୱିଗୁଣ s = 0;
                ଅଚଳ sT *tsrc2 = src + j*srcstep;
                ଅଚଳ dT *tdelta2 = delta + j*deltastep;
                ଯଦି( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#ଯଦି CV_SIMD128_64F
                ଯଦି (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ଅଚଳ ଦ୍ୱିଗୁଣ *v_tsrc2 = (ଦ୍ୱିଗୁଣ *)(tsrc2);
                    ଅଚଳ ଦ୍ୱିଗୁଣ *v_tdelta2 = (ଦ୍ୱିଗୁଣ *)(tdelta2);
                    ଅଚଳ ଦ୍ୱିଗୁଣ *v_row_buf = (ଦ୍ୱିଗୁଣ *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    ପାଇଁ( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (ଅଚଳ dT *)(v_tdelta2);
                }
                ଅନ୍ୟଥା
#endif
                {
                    ପାଇଁ( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (ଦ୍ୱିଗୁଣ)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (ଦ୍ୱିଗୁଣ)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (ଦ୍ୱିଗୁଣ)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (ଦ୍ୱିଗୁଣ)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                ପାଇଁ( ; k < size.width; k++, tdelta2++ )
                    s += (ଦ୍ୱିଗୁଣ)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, ତାର୍କିକ ata)
{
    MulTransposedFunc func = ଖାଲି;
    ଯଦି (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,ଦଶମିକ>
                   : MulTransposedL<uchar,ଦଶମିକ>;
    }
    ଅନ୍ୟଥା ଯଦି (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,ଦ୍ୱିଗୁଣ>
                   : MulTransposedL<uchar,ଦ୍ୱିଗୁଣ>;
    }
    ଅନ୍ୟଥା ଯଦି(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,ଦଶମିକ>
                   : MulTransposedL<ushort,ଦଶମିକ>;
    }
    ଅନ୍ୟଥା ଯଦି(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,ଦ୍ୱିଗୁଣ>
                   : MulTransposedL<ushort,ଦ୍ୱିଗୁଣ>;
    }
    ଅନ୍ୟଥା ଯଦି(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,ଦଶମିକ>
                   : MulTransposedL<short,ଦଶମିକ>;
    }
    ଅନ୍ୟଥା ଯଦି(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,ଦ୍ୱିଗୁଣ>
                   : MulTransposedL<short,ଦ୍ୱିଗୁଣ>;
    }
    ଅନ୍ୟଥା ଯଦି(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ଦଶମିକ,ଦଶମିକ>
                   : MulTransposedL<ଦଶମିକ,ଦଶମିକ>;
    }
    ଅନ୍ୟଥା ଯଦି(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ଦଶମିକ,ଦ୍ୱିଗୁଣ>
                   : MulTransposedL<ଦଶମିକ,ଦ୍ୱିଗୁଣ>;
    }
    ଅନ୍ୟଥା ଯଦି(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ>
                   : MulTransposedL<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ>;
    }
    CV_Assert(func && "Not supported");
    ଫେରନ୍ତୁ func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଦ୍ୱିଗୁଣ dotProd_(ଅଚଳ T* src1, ଅଚଳ T* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ଦ୍ୱିଗୁଣ result = 0;

#ଯଦି CV_ENABLE_UNROLLED
    ପାଇଁ( ; i <= len - 4; i += 4 )
        result += (ଦ୍ୱିଗୁଣ)src1[i]*src2[i] + (ଦ୍ୱିଗୁଣ)src1[i+1]*src2[i+1] +
            (ଦ୍ୱିଗୁଣ)src1[i+2]*src2[i+2] + (ଦ୍ୱିଗୁଣ)src1[i+3]*src2[i+3];
#endif
    ପାଇଁ( ; i < len; i++ )
        result += (ଦ୍ୱିଗୁଣ)src1[i]*src2[i];

    ଫେରନ୍ତୁ result;
}


ଦ୍ୱିଗୁଣ dotProd_8u(ଅଚଳ uchar* src1, ଅଚଳ uchar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ଦ୍ୱିଗୁଣ r = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    ଯେତେବେଳେ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cWidth = VTraits<v_uint16>::vlanes();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପାଇଁ (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        ପାଇଁ (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (ଦ୍ୱିଗୁଣ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ଫେରନ୍ତୁ r + dotProd_(src1, src2, len - i);
}


ଦ୍ୱିଗୁଣ dotProd_8s(ଅଚଳ schar* src1, ଅଚଳ schar* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ଦ୍ୱିଗୁଣ r = 0.0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    ଯେତେବେଳେ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cWidth = VTraits<v_int16>::vlanes();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପାଇଁ (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        ପାଇଁ (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (ଦ୍ୱିଗୁଣ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    ଫେରନ୍ତୁ r + dotProd_(src1, src2, len - i);
}

ଦ୍ୱିଗୁଣ dotProd_16u(ଅଚଳ ushort* src1, ଅଚଳ ushort* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ଦ୍ୱିଗୁଣ r = 0.0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    ଯେତେବେଳେ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cWidth = VTraits<v_uint16>::vlanes();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପାଇଁ (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (ଦ୍ୱିଗୁଣ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ଫେରନ୍ତୁ r + dotProd_(src1, src2, len - i);
}

ଦ୍ୱିଗୁଣ dotProd_16s(ଅଚଳ short* src1, ଅଚଳ short* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ଦ୍ୱିଗୁଣ r = 0.0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    ଯେତେବେଳେ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cWidth = VTraits<v_int16>::vlanes();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପାଇଁ (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (ଦ୍ୱିଗୁଣ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ଫେରନ୍ତୁ r + dotProd_(src1, src2, len - i);
}

ଦ୍ୱିଗୁଣ dotProd_32s(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
#ଯଦି CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ଦ୍ୱିଗୁଣ r = .0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#ଯଦି CV_SIMD_WIDTH == 16
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    ପାଇଁ (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
    {
        v_int32 v_src10 = vx_load(src1);
        v_int32 v_src20 = vx_load(src2);
        v_int32 v_src11 = vx_load(src1 + step);
        v_int32 v_src21 = vx_load(src2 + step);
        v_sum0 = v_dotprod_expand_fast(v_src10, v_src20, v_sum0);
        v_sum1 = v_dotprod_expand_fast(v_src11, v_src21, v_sum1);
    }
    v_sum0 = v_add(v_sum0, v_sum1);
#endif
    ପାଇଁ (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    ଫେରନ୍ତୁ r + dotProd_(src1, src2, len - i);
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ dotProd_(src1, src2, len);
#endif
}

ଦ୍ୱିଗୁଣ dotProd_32f(ଅଚଳ ଦଶମିକ* src1, ଅଚଳ ଦଶମିକ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ଦ୍ୱିଗୁଣ r = 0.0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    ଯେତେବେଳେ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cWidth = VTraits<v_float32>::vlanes();

#ଯଦି CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        ପାଇଁ (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
        {
            v_sum  = v_muladd(vx_load(src1 + j),
                              vx_load(src2 + j), v_sum);
            v_sum1 = v_muladd(vx_load(src1 + j + cWidth),
                              vx_load(src2 + j + cWidth), v_sum1);
            v_sum2 = v_muladd(vx_load(src1 + j + (cWidth * 2)),
                              vx_load(src2 + j + (cWidth * 2)), v_sum2);
            v_sum3 = v_muladd(vx_load(src1 + j + (cWidth * 3)),
                              vx_load(src2 + j + (cWidth * 3)), v_sum3);
        }

        v_sum = v_add(v_sum, v_add(v_add(v_sum1, v_sum2), v_sum3));
#endif

        ପାଇଁ (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ଫେରନ୍ତୁ r + dotProd_(src1, src2, len - i);
}

ଦ୍ୱିଗୁଣ dotProd_64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ଫେରନ୍ତୁ dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
