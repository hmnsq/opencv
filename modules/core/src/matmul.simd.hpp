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

#అన్తర్భావ "precomp.hpp"

#నిర్వచితమైతే HAVE_LAPACK
#నిర్వచించు CV_GEMM_BASELINE_ONLY
#అయితే_ముగింపు
#నిర్వచించు CV_MAHALANOBIS_BASELINE_ONLY
#నిర్వచించు CV_MULTRANSPOSED_BASELINE_ONLY

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
#నిర్వచితం_కాకపోతే CV_GEMM_SIMD_ENABLED
#ఒకవేళ defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#నిర్వచించు CV_GEMM_SIMD_ENABLED 0
#లేకపోతే
#నిర్వచించు CV_GEMM_SIMD_ENABLED 1
#అయితే_ముగింపు
#అయితే_ముగింపు

నామాకాశ cv {

// forward declarations
రకనిర్వచనం శూన్య (*TransformFunc)(స్థిరరాశి శూన్య* src, శూన్య* dst, స్థిరరాశి శూన్య* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn);
రకనిర్వచనం శూన్య (*Scale_కూడు_Func)(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, శూన్య* dst, పూర్ణాంకం len, స్థిరరాశి శూన్య* alpha);
రకనిర్వచనం శూన్య (*MulTransposedFunc)(స్థిరరాశి Mat& src, స్థిరరాశి/*preallocated*/ Mat& dst, స్థిరరాశి Mat& delta, ద్విగుణ scale);
రకనిర్వచనం ద్విగుణ (*MahalanobisImplFunc)(స్థిరరాశి Mat& v1, స్థిరరాశి Mat& v2, స్థిరరాశి Mat& icovar, ద్విగుణ *diff_బఫరు /*[len]*/, పూర్ణాంకం len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

శూన్య gemm32f(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
             దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
             పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags);
శూన్య gemm64f(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
             ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
             పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags);
శూన్య gemm32fc(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
              దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
              పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags);
శూన్య gemm64fc(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
              ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
              పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags);

TransformFunc getTransformFunc(పూర్ణాంకం లోతు);
TransformFunc getDiagTransformFunc(పూర్ణాంకం లోతు);
TransformFunc getPerspectiveTransform(పూర్ణాంకం లోతు);

Scale_కూడు_Func get_Scale_కూడు_Func(పూర్ణాంకం లోతు);

MahalanobisImplFunc getMahalanobisImplFunc(పూర్ణాంకం లోతు);

MulTransposedFunc getMulTransposedFunc(పూర్ణాంకం stype, పూర్ణాంకం dtype, తార్కిక ata);

ద్విగుణ dotProd_8u(స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, పూర్ణాంకం len);
ద్విగుణ dotProd_8s(స్థిరరాశి schar* src1, స్థిరరాశి schar* src2, పూర్ణాంకం len);
ద్విగుణ dotProd_16u(స్థిరరాశి ushort* src1, స్థిరరాశి ushort* src2, పూర్ణాంకం len);
ద్విగుణ dotProd_16s(స్థిరరాశి పొట్టి* src1, స్థిరరాశి పొట్టి* src2, పూర్ణాంకం len);
ద్విగుణ dotProd_32s(స్థిరరాశి పూర్ణాంకం* src1, స్థిరరాశి పూర్ణాంకం* src2, పూర్ణాంకం len);
ద్విగుణ dotProd_32f(స్థిరరాశి దశాంశం* src1, స్థిరరాశి దశాంశం* src2, పూర్ణాంకం len);
ద్విగుణ dotProd_64f(స్థిరరాశి ద్విగుణ* src1, స్థిరరాశి ద్విగుణ* src2, పూర్ణాంకం len);



#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ఒకవేళ !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_బహుళకం)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

స్థిర శూన్య
GEMM_Copy_ఖండం( స్థిరరాశి uchar* src, size_t src_అడుగు,
                uchar* dst, size_t dst_అడుగు,
                కొలత size, size_t pix_కొలత )
{
    పూర్ణాంకం j;
    size.వెడల్పు *= (పూర్ణాంకం)(pix_కొలత / పరిమాణం(పూర్ణాంకం));

    కొరకు( ; size.ఎత్తు--; src += src_అడుగు, dst += dst_అడుగు )
    {
        j=0;
         #ఒకవేళ CV_ENABLE_UNROLLED
        కొరకు( ; j <= size.వెడల్పు - 4; j += 4 )
        {
            పూర్ణాంకం t0 = ((స్థిరరాశి పూర్ణాంకం*)src)[j];
            పూర్ణాంకం t1 = ((స్థిరరాశి పూర్ణాంకం*)src)[j+1];
            ((పూర్ణాంకం*)dst)[j] = t0;
            ((పూర్ణాంకం*)dst)[j+1] = t1;
            t0 = ((స్థిరరాశి పూర్ణాంకం*)src)[j+2];
            t1 = ((స్థిరరాశి పూర్ణాంకం*)src)[j+3];
            ((పూర్ణాంకం*)dst)[j+2] = t0;
            ((పూర్ణాంకం*)dst)[j+3] = t1;
        }
        #అయితే_ముగింపు
        కొరకు( ; j < size.వెడల్పు; j++ )
            ((పూర్ణాంకం*)dst)[j] = ((స్థిరరాశి పూర్ణాంకం*)src)[j];
    }
}


స్థిర శూన్య
GEMM_Transpose_ఖండం( స్థిరరాశి uchar* src, size_t src_అడుగు,
                     uchar* dst, size_t dst_అడుగు,
                     కొలత size, size_t pix_కొలత )
{
    పూర్ణాంకం i, j;
    కొరకు( i = 0; i < size.వెడల్పు; i++, dst += dst_అడుగు, src += pix_కొలత )
    {
        స్థిరరాశి uchar* _src = src;
        ఎంపిక( pix_కొలత )
        {
        స్థితి పరిమాణం(పూర్ణాంకం):
            కొరకు( j = 0; j < size.ఎత్తు; j++, _src += src_అడుగు )
                ((పూర్ణాంకం*)dst)[j] = ((పూర్ణాంకం*)_src)[0];
            ఆపు;
        స్థితి పరిమాణం(పూర్ణాంకం)*2:
            కొరకు( j = 0; j < size.ఎత్తు*2; j += 2, _src += src_అడుగు )
            {
                పూర్ణాంకం t0 = ((పూర్ణాంకం*)_src)[0];
                పూర్ణాంకం t1 = ((పూర్ణాంకం*)_src)[1];
                ((పూర్ణాంకం*)dst)[j] = t0;
                ((పూర్ణాంకం*)dst)[j+1] = t1;
            }
            ఆపు;
        స్థితి పరిమాణం(పూర్ణాంకం)*4:
            కొరకు( j = 0; j < size.ఎత్తు*4; j += 4, _src += src_అడుగు )
            {
                పూర్ణాంకం t0 = ((పూర్ణాంకం*)_src)[0];
                పూర్ణాంకం t1 = ((పూర్ణాంకం*)_src)[1];
                ((పూర్ణాంకం*)dst)[j] = t0;
                ((పూర్ణాంకం*)dst)[j+1] = t1;
                t0 = ((పూర్ణాంకం*)_src)[2];
                t1 = ((పూర్ణాంకం*)_src)[3];
                ((పూర్ణాంకం*)dst)[j+2] = t0;
                ((పూర్ణాంకం*)dst)[j+3] = t1;
            }
            ఆపు;
        అప్రమేయ:
            CV_Assert(0);
            తిరిగి;
        }
    }
}

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
మూస<రకం_పేరు ST> నిర్మాణం VecTraits;

మూస<> నిర్మాణం VecTraits<దశాంశం> {
    రకనిర్వచనం v_float32 VT;
    స్థిర అంతఃవరుస VT zero() { తిరిగి vx_setzero_f32(); }
    స్థిర అంతఃవరుస VT load(స్థిరరాశి దశాంశం* p) { తిరిగి vx_ఎక్కించు(p); }
    స్థిర అంతఃవరుస VT setall(దశాంశం v) { తిరిగి vx_setall_f32(v); }
};
#అయితే_ముగింపు

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
మూస<> నిర్మాణం VecTraits<ద్విగుణ> {
    రకనిర్వచనం v_float64 VT;
    స్థిర అంతఃవరుస VT zero() { తిరిగి vx_setzero_f64(); }
    స్థిర అంతఃవరుస VT load(స్థిరరాశి ద్విగుణ* p) { తిరిగి vx_ఎక్కించు(p); }
    స్థిర అంతఃవరుస VT setall(ద్విగుణ v) { తిరిగి vx_setall_f64(v); }
};
#అయితే_ముగింపు

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
మూస<రకం_పేరు ST>
స్థిర అంతఃవరుస ST simd_Dot_లబ్ధం(స్థిరరాశి ST* a, స్థిరరాశి ST* b, పూర్ణాంకం n, పూర్ణాంకం& k)
{
    రకనిర్వచనం రకం_పేరు VecTraits<ST>::VT VecT;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VecT>::vlanes();
    స్థిరరాశి పూర్ణాంకం stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    కొరకు( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_కూడు(v_కూడు(vs0, vs1), v_కూడు(vs2, vs3));
    కొరకు( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    తిరిగి v_reduce_కూడిక(vs0);
}
#అయితే_ముగింపు

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
స్థిర అంతఃవరుస ద్విగుణ simd_Dot_లబ్ధం_f32f64(స్థిరరాశి దశాంశం* a, స్థిరరాశి దశాంశం* b, పూర్ణాంకం n, పూర్ణాంకం& k)
{
    స్థిరరాశి పూర్ణాంకం vlanes_f32 = VTraits<v_float32>::vlanes();
    స్థిరరాశి పూర్ణాంకం stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    కొరకు( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_ఎక్కించు(a + k);
        v_float32 vb0 = vx_ఎక్కించు(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_ఎగువ(va0), v_cvt_f64_ఎగువ(vb0), vs1);

        v_float32 va1 = vx_ఎక్కించు(a + k + vlanes_f32);
        v_float32 vb1 = vx_ఎక్కించు(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_ఎగువ(va1), v_cvt_f64_ఎగువ(vb1), vs3);
    }
    vs0 = v_కూడు(v_కూడు(vs0, vs1), v_కూడు(vs2, vs3));
    కొరకు( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_ఎక్కించు(a + k);
        v_float32 vb = vx_ఎక్కించు(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_ఎగువ(va), v_cvt_f64_ఎగువ(vb), vs0);
    }
    తిరిగి v_reduce_కూడిక(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
స్థిర అంతఃవరుస శూన్య simdGEMM_kj_f32f64(
    స్థిరరాశి దశాంశం* a_దత్తాంశం, స్థిరరాశి దశాంశం* b_దత్తాంశం_2, size_t b_అడుగు,
    స్థిరరాశి దశాంశం* c_దత్తాంశం, size_t c_step1,
    దశాంశం* d_దత్తాంశం,
    పూర్ణాంకం n, పూర్ణాంకం m,
    ద్విగుణ alpha, ద్విగుణ beta,
    పూర్ణాంకం& j)
{
    స్థిరరాశి పూర్ణాంకం vlanes_f32 = VTraits<v_float32>::vlanes();
    స్థిరరాశి పూర్ణాంకం vlanes_f64 = VTraits<v_float64>::vlanes();

    ఒకవేళ( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        స్థిరరాశి పూర్ణాంకం max_m = 1600 / (పూర్ణాంకం)పరిమాణం(దశాంశం);
        cv::Auto_బఫరు<ద్విగుణ> _s_buf(max_m);
        ద్విగుణ* s_buf = _s_buf.దత్తాంశం();

        {
            v_float64 vz = vx_setzero_f64();
            కొరకు( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            కొరకు( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            స్థిరరాశి దశాంశం* b_ptr = b_దత్తాంశం_2;
            కొరకు( పూర్ణాంకం k = 0; k < n; k++, b_ptr += b_అడుగు )
            {
                v_float64 va = vx_setall_f64((ద్విగుణ)a_దత్తాంశం[k]);
                j = 0;
                కొరకు( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_ఎక్కించు(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_ఎగువ(vb);
                    v_float64 vd_lo = vx_ఎక్కించు(s_buf + j);
                    v_float64 vd_hi = vx_ఎక్కించు(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                ద్విగుణ a_val = (ద్విగుణ)a_దత్తాంశం[k];
                కొరకు( ; j < m; j++ )
                    s_buf[j] += a_val * (ద్విగుణ)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            స్థిరరాశి దశాంశం* c_దత్తాంశం_2 = c_దత్తాంశం;
            j = 0;
            ఒకవేళ( !c_దత్తాంశం )
            {
                కొరకు( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_ఎక్కించు(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_ఎక్కించు(s_buf + j + vlanes_f64), valpha);
                    v_store(d_దత్తాంశం + j, v_cvt_f32(vs_lo, vs_hi));
                }
                కొరకు( ; j < m; j++ )
                    d_దత్తాంశం[j] = (దశాంశం)(s_buf[j] * alpha);
            }
            లేకపోతే ఒకవేళ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                కొరకు( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_ఎక్కించు(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_ఎక్కించు(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_ఎక్కించు(c_దత్తాంశం_2 + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_ఎగువ(vc), vbeta, vs_hi);
                    v_store(d_దత్తాంశం + j, v_cvt_f32(vs_lo, vs_hi));
                }
                కొరకు( ; j < m; j++ )
                    d_దత్తాంశం[j] = (దశాంశం)(s_buf[j] * alpha + (ద్విగుణ)c_దత్తాంశం_2[j] * beta);
            }
            లేకపోతే
            {
                కొరకు( j = 0; j < m; j++, c_దత్తాంశం_2 += c_step1 )
                    d_దత్తాంశం[j] = (దశాంశం)(s_buf[j] * alpha + (ద్విగుణ)c_దత్తాంశం_2[0] * beta);
            }
        }
        j = m;
    }
    లేకపోతే ఒకవేళ( vlanes_f32 * (పూర్ణాంకం)పరిమాణం(దశాంశం) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        స్థిరరాశి దశాంశం* c_దత్తాంశం_2 = c_దత్తాంశం;
        కొరకు( ; j <= m - vlanes_f32; j += vlanes_f32, c_దత్తాంశం_2 += vlanes_f32 * c_step1 )
        {
            స్థిరరాశి దశాంశం* b = b_దత్తాంశం_2 + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            కొరకు( పూర్ణాంకం k = 0; k < n; k++, b += b_అడుగు )
            {
                v_float64 va = vx_setall_f64((ద్విగుణ)a_దత్తాంశం[k]);
                v_float32 vb = vx_ఎక్కించు(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_ఎగువ(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            ఒకవేళ( !c_దత్తాంశం )
            {
                v_store(d_దత్తాంశం + j, v_cvt_f32(vs_lo, vs_hi));
            }
            లేకపోతే ఒకవేళ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_ఎక్కించు(c_దత్తాంశం_2);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_ఎగువ(vc), vbeta, vs_hi);
                v_store(d_దత్తాంశం + j, v_cvt_f32(vs_lo, vs_hi));
            }
            లేకపోతే
            {
                CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) దశాంశం buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                కొరకు( పూర్ణాంకం jj = 0; jj < vlanes_f32; jj++ )
                    d_దత్తాంశం[j + jj] = buf[jj] + c_దత్తాంశం_2[jj * c_step1] * (దశాంశం)beta;
            }
        }
    }
}
#అయితే_ముగింపు

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
మూస<రకం_పేరు ST>
స్థిర అంతఃవరుస శూన్య simdGEMM_kj(
    స్థిరరాశి ST* a_దత్తాంశం, స్థిరరాశి ST* b_దత్తాంశం_2, size_t b_అడుగు,
    స్థిరరాశి ST* c_దత్తాంశం, size_t c_step1,
    ST* d_దత్తాంశం,
    పూర్ణాంకం n, పూర్ణాంకం m,
    ద్విగుణ alpha, ద్విగుణ beta,
    పూర్ణాంకం& j)
{
    రకనిర్వచనం రకం_పేరు VecTraits<ST>::VT VecT;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VecT>::vlanes();

    ఒకవేళ( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        స్థిరరాశి పూర్ణాంకం max_m = 1600 / (పూర్ణాంకం)పరిమాణం(ST);
        CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            కొరకు( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            కొరకు( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            స్థిరరాశి ST* b_ptr = b_దత్తాంశం_2;
            కొరకు( పూర్ణాంకం k = 0; k < n; k++, b_ptr += b_అడుగు )
            {
                VecT va = VecTraits<ST>::setall(a_దత్తాంశం[k]);
                j = 0;
                కొరకు( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_ఎక్కించు_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_దత్తాంశం[k];
                కొరకు( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            స్థిరరాశి ST* c_దత్తాంశం_2 = c_దత్తాంశం;
            j = 0;
            ఒకవేళ( !c_దత్తాంశం )
            {
                కొరకు( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_ఎక్కించు_aligned(s_buf + j), valpha);
                    v_store(d_దత్తాంశం + j, vs);
                }
                కొరకు( ; j < m; j++ )
                    d_దత్తాంశం[j] = s_buf[j] * (ST)alpha;
            }
            లేకపోతే ఒకవేళ( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                కొరకు( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_ఎక్కించు_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_దత్తాంశం_2 + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_దత్తాంశం + j, vs);
                }
                కొరకు( ; j < m; j++ )
                    d_దత్తాంశం[j] = s_buf[j] * (ST)alpha + c_దత్తాంశం_2[j] * (ST)beta;
            }
            లేకపోతే
            {
                కొరకు( j = 0; j < m; j++, c_దత్తాంశం_2 += c_step1 )
                    d_దత్తాంశం[j] = s_buf[j] * (ST)alpha + c_దత్తాంశం_2[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    లేకపోతే ఒకవేళ( vlanes * (పూర్ణాంకం)పరిమాణం(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        స్థిరరాశి ST* c_దత్తాంశం_2 = c_దత్తాంశం;
        కొరకు( ; j <= m - vlanes; j += vlanes, c_దత్తాంశం_2 += vlanes * c_step1 )
        {
            స్థిరరాశి ST* b = b_దత్తాంశం_2 + j;
            VecT vs = VecTraits<ST>::zero();
            కొరకు( పూర్ణాంకం k = 0; k < n; k++, b += b_అడుగు )
            {
                VecT va = VecTraits<ST>::setall(a_దత్తాంశం[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            ఒకవేళ( !c_దత్తాంశం )
            {
                v_store(d_దత్తాంశం + j, vs);
            }
            లేకపోతే ఒకవేళ( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_దత్తాంశం_2);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_దత్తాంశం + j, vs);
            }
            లేకపోతే
            {
                CV_DECL_ALIGNED(CV_SIMD_వెడల్పు) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                కొరకు( పూర్ణాంకం jj = 0; jj < vlanes; jj++ )
                    d_దత్తాంశం[j + jj] = buf[jj] + c_దత్తాంశం_2[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#అయితే_ముగింపు


మూస<రకం_పేరు T, రకం_పేరు WT> స్థిర శూన్య
GEMMSingleMul( స్థిరరాశి T* a_దత్తాంశం, size_t a_అడుగు,
               స్థిరరాశి T* b_దత్తాంశం, size_t b_అడుగు,
               స్థిరరాశి T* c_దత్తాంశం_2, size_t c_అడుగు,
               T* d_దత్తాంశం, size_t d_అడుగు,
               కొలత a_కొలత, కొలత d_కొలత,
               ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    పూర్ణాంకం i, j, k, n = a_కొలత.వెడల్పు, m = d_కొలత.వెడల్పు, drows = d_కొలత.ఎత్తు;
    స్థిరరాశి T *a_దత్తాంశం_2 = a_దత్తాంశం, *b_దత్తాంశం_2 = b_దత్తాంశం, *c_దత్తాంశం = c_దత్తాంశం_2;
    cv::Auto_బఫరు<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_అడుగు;

    a_అడుగు /= పరిమాణం(a_దత్తాంశం[0]);
    b_అడుగు /= పరిమాణం(b_దత్తాంశం[0]);
    c_అడుగు /= పరిమాణం(c_దత్తాంశం_2[0]);
    d_అడుగు /= పరిమాణం(d_దత్తాంశం[0]);
    a_step0 = a_అడుగు;
    a_step1 = 1;

    ఒకవేళ( !c_దత్తాంశం_2 )
        c_step0 = c_step1 = 0;
    లేకపోతే ఒకవేళ( !(flags & GEMM_3_T) )
        c_step0 = c_అడుగు, c_step1 = 1;
    లేకపోతే
        c_step0 = 1, c_step1 = c_అడుగు;

    ఒకవేళ( flags & GEMM_1_T )
    {
        CV_మార్పిడి( a_step0, a_step1, t_అడుగు );
        n = a_కొలత.ఎత్తు;
        ఒకవేళ( a_అడుగు > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.దత్తాంశం();
        }
    }

    ఒకవేళ( n == 1 ) /* external product */
    {
        cv::Auto_బఫరు<T> _b_buf;
        T* b_buf = 0;

        ఒకవేళ( a_అడుగు > 1 && a_కొలత.ఎత్తు > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.దత్తాంశం();
            కొరకు( k = 0; k < drows; k++ )
                a_buf[k] = a_దత్తాంశం[a_అడుగు*k];
            a_దత్తాంశం = a_buf;
        }

        ఒకవేళ( b_అడుగు > 1 )
        {
            _b_buf.allocate(d_కొలత.వెడల్పు);
            b_buf = _b_buf.దత్తాంశం();
            కొరకు( j = 0; j < d_కొలత.వెడల్పు; j++ )
                b_buf[j] = b_దత్తాంశం[j*b_అడుగు];
            b_దత్తాంశం = b_buf;
        }

        కొరకు( i = 0; i < drows; i++, c_దత్తాంశం += c_step0, d_దత్తాంశం += d_అడుగు )
        {
            WT al = WT(a_దత్తాంశం[i])*alpha;
            c_దత్తాంశం_2 = c_దత్తాంశం;
            కొరకు( j = 0; j <= d_కొలత.వెడల్పు - 2; j += 2, c_దత్తాంశం_2 += 2*c_step1 )
            {
                WT s0 = al*WT(b_దత్తాంశం[j]);
                WT s1 = al*WT(b_దత్తాంశం[j+1]);
                ఒకవేళ( !c_దత్తాంశం_2 )
                {
                    d_దత్తాంశం[j] = T(s0);
                    d_దత్తాంశం[j+1] = T(s1);
                }
                లేకపోతే
                {
                    d_దత్తాంశం[j] = T(s0 + WT(c_దత్తాంశం_2[0])*beta);
                    d_దత్తాంశం[j+1] = T(s1 + WT(c_దత్తాంశం_2[c_step1])*beta);
                }
            }

            కొరకు( ; j < d_కొలత.వెడల్పు; j++, c_దత్తాంశం_2 += c_step1 )
            {
                WT s0 = al*WT(b_దత్తాంశం[j]);
                ఒకవేళ( !c_దత్తాంశం_2 )
                    d_దత్తాంశం[j] = T(s0);
                లేకపోతే
                    d_దత్తాంశం[j] = T(s0 + WT(c_దత్తాంశం_2[0])*beta);
            }
        }
    }
    లేకపోతే ఒకవేళ( flags & GEMM_2_T ) /* A * Bt */
    {
        కొరకు( i = 0; i < drows; i++, a_దత్తాంశం_2 += a_step0, c_దత్తాంశం += c_step0, d_దత్తాంశం += d_అడుగు )
        {
            a_దత్తాంశం = a_దత్తాంశం_2;
            b_దత్తాంశం = b_దత్తాంశం_2;
            c_దత్తాంశం_2 = c_దత్తాంశం;

            ఒకవేళ( a_buf )
            {
                కొరకు( k = 0; k < n; k++ )
                    a_buf[k] = a_దత్తాంశం[a_step1*k];
                a_దత్తాంశం = a_buf;
            }

            కొరకు( j = 0; j < d_కొలత.వెడల్పు; j++, b_దత్తాంశం += b_అడుగు,
                                               c_దత్తాంశం_2 += c_step1 )
            {
                WT s0(0);
                k = 0;
#ఒకవేళ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ఒకవేళ( పరిమాణం(WT) == పరిమాణం(ద్విగుణ) )
                {
                    ఒకవేళ( పరిమాణం(T) == పరిమాణం(ద్విగుణ) )
                        s0 = (WT)simd_Dot_లబ్ధం((స్థిరరాశి ద్విగుణ*)a_దత్తాంశం, (స్థిరరాశి ద్విగుణ*)b_దత్తాంశం, n, k);
                    లేకపోతే ఒకవేళ( పరిమాణం(T) == పరిమాణం(దశాంశం) )
                        s0 = (WT)simd_Dot_లబ్ధం_f32f64((స్థిరరాశి దశాంశం*)a_దత్తాంశం, (స్థిరరాశి దశాంశం*)b_దత్తాంశం, n, k);
                }
                లేకపోతే
#అయితే_ముగింపు
                {
                    WT s1(0), s2(0), s3(0);
                     #ఒకవేళ CV_ENABLE_UNROLLED
                    కొరకు( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_దత్తాంశం[k])*WT(b_దత్తాంశం[k]);
                        s1 += WT(a_దత్తాంశం[k+1])*WT(b_దత్తాంశం[k+1]);
                        s2 += WT(a_దత్తాంశం[k+2])*WT(b_దత్తాంశం[k+2]);
                        s3 += WT(a_దత్తాంశం[k+3])*WT(b_దత్తాంశం[k+3]);
                    }
                    #అయితే_ముగింపు
                    s0 += s1 + s2 + s3;
                }
                కొరకు( ; k < n; k++ )
                    s0 += WT(a_దత్తాంశం[k])*WT(b_దత్తాంశం[k]);
                s0 = s0*alpha;

                ఒకవేళ( !c_దత్తాంశం_2 )
                    d_దత్తాంశం[j] = T(s0);
                లేకపోతే
                    d_దత్తాంశం[j] = T(s0 + WT(c_దత్తాంశం_2[0])*beta);
            }
        }
    }
    లేకపోతే ఒకవేళ( d_కొలత.వెడల్పు*పరిమాణం(d_దత్తాంశం[0]) <= 1600 )
    {
        కొరకు( i = 0; i < drows; i++, a_దత్తాంశం_2 += a_step0,
                                    c_దత్తాంశం += c_step0,
                                    d_దత్తాంశం += d_అడుగు )
        {
            a_దత్తాంశం = a_దత్తాంశం_2, c_దత్తాంశం_2 = c_దత్తాంశం;

            ఒకవేళ( a_buf )
            {
                కొరకు( k = 0; k < n; k++ )
                    a_buf[k] = a_దత్తాంశం[a_step1*k];
                a_దత్తాంశం = a_buf;
            }

            j = 0;
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ఒకవేళ( పరిమాణం(WT) == పరిమాణం(ద్విగుణ) )
            {
                ఒకవేళ( పరిమాణం(T) == పరిమాణం(ద్విగుణ) )
                    simdGEMM_kj((స్థిరరాశి ద్విగుణ*)a_దత్తాంశం, (స్థిరరాశి ద్విగుణ*)b_దత్తాంశం_2, b_అడుగు,
                                        (స్థిరరాశి ద్విగుణ*)c_దత్తాంశం, c_step1,
                                (ద్విగుణ*)d_దత్తాంశం, n, m, alpha, beta, j);
                లేకపోతే ఒకవేళ( పరిమాణం(T) == పరిమాణం(దశాంశం) )
                    simdGEMM_kj_f32f64((స్థిరరాశి దశాంశం*)a_దత్తాంశం, (స్థిరరాశి దశాంశం*)b_దత్తాంశం_2, b_అడుగు,
                                        (స్థిరరాశి దశాంశం*)c_దత్తాంశం, c_step1,
                                (దశాంశం*)d_దత్తాంశం, n, m, alpha, beta, j);
            }
#అయితే_ముగింపు
            c_దత్తాంశం_2 = c_దత్తాంశం + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            కొరకు( ; j <= m - 4; j += 4, c_దత్తాంశం_2 += 4*c_step1 )
            {
                స్థిరరాశి T* b = b_దత్తాంశం_2 + j;
                WT s0(0), s1(0), s2(0), s3(0);

                కొరకు( k = 0; k < n; k++, b += b_అడుగు )
                {
                    WT a(a_దత్తాంశం[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                ఒకవేళ( !c_దత్తాంశం_2 )
                {
                    d_దత్తాంశం[j] = T(s0*alpha);
                    d_దత్తాంశం[j+1] = T(s1*alpha);
                    d_దత్తాంశం[j+2] = T(s2*alpha);
                    d_దత్తాంశం[j+3] = T(s3*alpha);
                }
                లేకపోతే
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_దత్తాంశం[j] = T(s0 + WT(c_దత్తాంశం_2[0])*beta);
                    d_దత్తాంశం[j+1] = T(s1 + WT(c_దత్తాంశం_2[c_step1])*beta);
                    d_దత్తాంశం[j+2] = T(s2 + WT(c_దత్తాంశం_2[c_step1*2])*beta);
                    d_దత్తాంశం[j+3] = T(s3 + WT(c_దత్తాంశం_2[c_step1*3])*beta);
                }
            }

            కొరకు( ; j < m; j++, c_దత్తాంశం_2 += c_step1 )
            {
                స్థిరరాశి T* b = b_దత్తాంశం_2 + j;
                WT s0(0);

                కొరకు( k = 0; k < n; k++, b += b_అడుగు )
                    s0 += WT(a_దత్తాంశం[k]) * WT(b[0]);

                s0 = s0*alpha;
                ఒకవేళ( !c_దత్తాంశం_2 )
                    d_దత్తాంశం[j] = T(s0);
                లేకపోతే
                    d_దత్తాంశం[j] = T(s0 + WT(c_దత్తాంశం_2[0])*beta);
            }
        }
    }
    లేకపోతే
    {
        cv::Auto_బఫరు<WT> _d_buf(m);
        WT* d_buf = _d_buf.దత్తాంశం();

        కొరకు( i = 0; i < drows; i++, a_దత్తాంశం_2 += a_step0, c_దత్తాంశం += c_step0, d_దత్తాంశం += d_అడుగు )
        {
            a_దత్తాంశం = a_దత్తాంశం_2;
            b_దత్తాంశం = b_దత్తాంశం_2;
            c_దత్తాంశం_2 = c_దత్తాంశం;

            ఒకవేళ( a_buf )
            {
                కొరకు( k = 0; k < n; k++ )
                    a_buf[k] = a_దత్తాంశం_2[a_step1*k];
                a_దత్తాంశం = a_buf;
            }

            కొరకు( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            కొరకు( k = 0; k < n; k++, b_దత్తాంశం += b_అడుగు )
            {
                WT al(a_దత్తాంశం[k]);
                j=0;
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                ఒకవేళ( పరిమాణం(WT) == పరిమాణం(ద్విగుణ) )
                {
                    ఒకవేళ( పరిమాణం(T) == పరిమాణం(ద్విగుణ) )
                    {
                        స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((స్థిరరాశి ద్విగుణ*)&al));
                        కొరకు( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_ఎక్కించు((స్థిరరాశి ద్విగుణ*)(d_buf + j));
                            v_float64 vb = vx_ఎక్కించు((స్థిరరాశి ద్విగుణ*)(b_దత్తాంశం + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((ద్విగుణ*)(d_buf + j), vd);
                        }
                    }
                    లేకపోతే
                    {
                        స్థిరరాశి పూర్ణాంకం vlanes_f32 = VTraits<v_float32>::vlanes();
                        స్థిరరాశి పూర్ణాంకం vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((స్థిరరాశి ద్విగుణ*)&al));
                        కొరకు( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_ఎక్కించు((స్థిరరాశి దశాంశం*)(b_దత్తాంశం + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_ఎగువ(vb);
                            v_float64 vd_lo = vx_ఎక్కించు((స్థిరరాశి ద్విగుణ*)(d_buf + j));
                            v_float64 vd_hi = vx_ఎక్కించు((స్థిరరాశి ద్విగుణ*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((ద్విగుణ*)(d_buf + j), vd_lo);
                            v_store((ద్విగుణ*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#అయితే_ముగింపు
                కొరకు( ; j < m; j++ )
                    d_buf[j] += WT(b_దత్తాంశం[j])*al;
            }

            ఒకవేళ( !c_దత్తాంశం_2 )
                కొరకు( j = 0; j < m; j++ )
                    d_దత్తాంశం[j] = T(d_buf[j]*alpha);
            లేకపోతే
                కొరకు( j = 0; j < m; j++, c_దత్తాంశం_2 += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_దత్తాంశం[j] = T(t + WT(c_దత్తాంశం_2[0])*beta);
                }
        }
    }
}
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
మూస<రకం_పేరు WT>
స్థిర అంతఃవరుస శూన్య సమితి_Double_దత్తాంశం_Zero(WT* d_దత్తాంశం, పూర్ణాంకం m, పూర్ణాంకం vlanes)
{
    v_float64 vz = vx_setzero_f64();
    పూర్ణాంకం j = 0;
    కొరకు( ; j <= m - vlanes; j += vlanes )
        v_store((ద్విగుణ*)(d_దత్తాంశం + j), vz);
    కొరకు( ; j < m; j++ )
        d_దత్తాంశం[j] = WT(0);
}

మూస<రకం_పేరు ST>
స్థిర అంతఃవరుస శూన్య simd_Mul_కూడు(స్థిరరాశి ST* b, ద్విగుణ* d, పూర్ణాంకం m, ద్విగుణ a_val);

మూస<>
అంతఃవరుస శూన్య simd_Mul_కూడు<దశాంశం>(
    స్థిరరాశి దశాంశం* b, ద్విగుణ* d, పూర్ణాంకం m, ద్విగుణ a_val)
{
    స్థిరరాశి పూర్ణాంకం vlanes_f32 = VTraits<v_float32>::vlanes();
    స్థిరరాశి పూర్ణాంకం vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    పూర్ణాంకం j = 0;
    కొరకు( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_ఎక్కించు(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_ఎగువ(vb);
        v_float64 vd_lo = vx_ఎక్కించు(d + j);
        v_float64 vd_hi = vx_ఎక్కించు(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    కొరకు( ; j < m; j++ )
        d[j] += a_val * (ద్విగుణ)b[j];
}

మూస<>
అంతఃవరుస శూన్య simd_Mul_కూడు<ద్విగుణ>(
    స్థిరరాశి ద్విగుణ* b, ద్విగుణ* d, పూర్ణాంకం m, ద్విగుణ a_val)
{
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_float64>::vlanes();
    స్థిరరాశి పూర్ణాంకం stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    పూర్ణాంకం j = 0;
    కొరకు( ; j <= m - stride2; j += stride2 )
    {
        v_float64 vd0 = vx_ఎక్కించు(d + j);
        v_float64 vb0 = vx_ఎక్కించు(b + j);
        v_float64 vd1 = vx_ఎక్కించు(d + j + vlanes);
        v_float64 vb1 = vx_ఎక్కించు(b + j + vlanes);
        vd0 = v_muladd(va, vb0, vd0);
        vd1 = v_muladd(va, vb1, vd1);
        v_store(d + j, vd0);
        v_store(d + j + vlanes, vd1);
    }
    కొరకు( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_ఎక్కించు(d + j);
        v_float64 vb = vx_ఎక్కించు(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    కొరకు( ; j < m; j++ )
        d[j] += a_val * b[j];
}

మూస<రకం_పేరు ST>
స్థిర అంతఃవరుస శూన్య simd_ఖండం_Mul_kj(
    స్థిరరాశి ST* a_దత్తాంశం, స్థిరరాశి ST* b_దత్తాంశం, size_t b_అడుగు,
    ద్విగుణ* d_దత్తాంశం, పూర్ణాంకం n, పూర్ణాంకం m, తార్కిక do_acc)
{
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_float64>::vlanes();
    ఒకవేళ( !do_acc )
        సమితి_Double_దత్తాంశం_Zero(d_దత్తాంశం, m, vlanes);
    కొరకు( పూర్ణాంకం k = 0; k < n; k++, b_దత్తాంశం += b_అడుగు )
        simd_Mul_కూడు(b_దత్తాంశం, d_దత్తాంశం, m, (ద్విగుణ)a_దత్తాంశం[k]);
}
#అయితే_ముగింపు // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

మూస<రకం_పేరు T, రకం_పేరు WT> స్థిర శూన్య
GEMMBlockMul( స్థిరరాశి T* a_దత్తాంశం, size_t a_అడుగు,
              స్థిరరాశి T* b_దత్తాంశం, size_t b_అడుగు,
              WT* d_దత్తాంశం, size_t d_అడుగు,
              కొలత a_కొలత, కొలత d_కొలత, పూర్ణాంకం flags )
{
    పూర్ణాంకం i, j, k, n = a_కొలత.వెడల్పు, m = d_కొలత.వెడల్పు;
    స్థిరరాశి T *a_దత్తాంశం_2 = a_దత్తాంశం, *b_దత్తాంశం_2 = b_దత్తాంశం;
    cv::Auto_బఫరు<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_అడుగు;
    పూర్ణాంకం do_acc = flags & 16;

    a_అడుగు /= పరిమాణం(a_దత్తాంశం[0]);
    b_అడుగు /= పరిమాణం(b_దత్తాంశం[0]);
    d_అడుగు /= పరిమాణం(d_దత్తాంశం[0]);

    a_step0 = a_అడుగు;
    a_step1 = 1;

    ఒకవేళ( flags & GEMM_1_T )
    {
        CV_మార్పిడి( a_step0, a_step1, t_అడుగు );
        n = a_కొలత.ఎత్తు;
        _a_buf.allocate(n);
        a_buf = _a_buf.దత్తాంశం();
    }

    ఒకవేళ( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        కొరకు( i = 0; i < d_కొలత.ఎత్తు; i++, a_దత్తాంశం_2 += a_step0, d_దత్తాంశం += d_అడుగు )
        {
            a_దత్తాంశం = a_దత్తాంశం_2; b_దత్తాంశం = b_దత్తాంశం_2;

            ఒకవేళ( a_buf )
            {
                కొరకు( k = 0; k < n; k++ )
                    a_buf[k] = a_దత్తాంశం[a_step1*k];
                a_దత్తాంశం = a_buf;
            }

            కొరకు( j = 0; j < d_కొలత.వెడల్పు; j++, b_దత్తాంశం += b_అడుగు )
            {
                WT s0 = do_acc ? d_దత్తాంశం[j] : WT(0);
                k = 0;
#ఒకవేళ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ఒకవేళ( పరిమాణం(WT) == పరిమాణం(ద్విగుణ) )
                {
                    ఒకవేళ( పరిమాణం(T) == పరిమాణం(ద్విగుణ) )
                        s0 += (WT)simd_Dot_లబ్ధం((స్థిరరాశి ద్విగుణ*)a_దత్తాంశం, (స్థిరరాశి ద్విగుణ*)b_దత్తాంశం, n, k);
                    లేకపోతే ఒకవేళ( పరిమాణం(T) == పరిమాణం(దశాంశం) )
                        s0 += (WT)simd_Dot_లబ్ధం_f32f64((స్థిరరాశి దశాంశం*)a_దత్తాంశం, (స్థిరరాశి దశాంశం*)b_దత్తాంశం, n, k);
                }
                లేకపోతే
#అయితే_ముగింపు
                {
                    WT s1(0);
                    కొరకు( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_దత్తాంశం[k])*WT(b_దత్తాంశం[k]);
                        s1 += WT(a_దత్తాంశం[k+1])*WT(b_దత్తాంశం[k+1]);
                    }
                    s0 += s1;
                }
                కొరకు( ; k < n; k++ )
                    s0 += WT(a_దత్తాంశం[k])*WT(b_దత్తాంశం[k]);

                d_దత్తాంశం[j] = s0;
            }
        }
    }
    లేకపోతే
    {
        కొరకు( i = 0; i < d_కొలత.ఎత్తు; i++, a_దత్తాంశం_2 += a_step0, d_దత్తాంశం += d_అడుగు )
        {
            a_దత్తాంశం = a_దత్తాంశం_2, b_దత్తాంశం = b_దత్తాంశం_2;

            ఒకవేళ( a_buf )
            {
                కొరకు( k = 0; k < n; k++ )
                    a_buf[k] = a_దత్తాంశం[a_step1*k];
                a_దత్తాంశం = a_buf;
            }
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ఒకవేళ( పరిమాణం(WT) == పరిమాణం(ద్విగుణ) )
            {
                ఒకవేళ( పరిమాణం(T) == పరిమాణం(ద్విగుణ) )
                    simd_ఖండం_Mul_kj(
                        (స్థిరరాశి ద్విగుణ*)a_దత్తాంశం, (స్థిరరాశి ద్విగుణ*)b_దత్తాంశం, b_అడుగు,
                        (ద్విగుణ*)d_దత్తాంశం, n, m, do_acc != 0);
                లేకపోతే
                    simd_ఖండం_Mul_kj(
                        (స్థిరరాశి దశాంశం*)a_దత్తాంశం, (స్థిరరాశి దశాంశం*)b_దత్తాంశం, b_అడుగు,
                        (ద్విగుణ*)d_దత్తాంశం, n, m, do_acc != 0);
            }
            లేకపోతే
#అయితే_ముగింపు
            {
                కొరకు( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    స్థిరరాశి T* b = b_దత్తాంశం + j;

                    ఒకవేళ( do_acc )
                    {
                        s0 = d_దత్తాంశం[j]; s1 = d_దత్తాంశం[j+1];
                        s2 = d_దత్తాంశం[j+2]; s3 = d_దత్తాంశం[j+3];
                    }
                    లేకపోతే
                        s0 = s1 = s2 = s3 = WT(0);

                    కొరకు( k = 0; k < n; k++, b += b_అడుగు )
                    {
                        WT a(a_దత్తాంశం[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_దత్తాంశం[j] = s0; d_దత్తాంశం[j+1] = s1;
                    d_దత్తాంశం[j+2] = s2; d_దత్తాంశం[j+3] = s3;
                }

                కొరకు( ; j < m; j++ )
                {
                    స్థిరరాశి T* b = b_దత్తాంశం + j;
                    WT s0 = do_acc ? d_దత్తాంశం[j] : WT(0);

                    కొరకు( k = 0; k < n; k++, b += b_అడుగు )
                        s0 += WT(a_దత్తాంశం[k]) * WT(b[0]);

                    d_దత్తాంశం[j] = s0;
                }
            }
        }
    }
}


మూస<రకం_పేరు T, రకం_పేరు WT> స్థిర శూన్య
GEMMStore( స్థిరరాశి T* c_దత్తాంశం_2, size_t c_అడుగు,
           స్థిరరాశి WT* d_buf, size_t d_buf_అడుగు,
           T* d_దత్తాంశం, size_t d_అడుగు, కొలత d_కొలత,
           ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    స్థిరరాశి T* c_దత్తాంశం = c_దత్తాంశం_2;
    పూర్ణాంకం j;
    size_t c_step0, c_step1;

    c_అడుగు /= పరిమాణం(c_దత్తాంశం_2[0]);
    d_buf_అడుగు /= పరిమాణం(d_buf[0]);
    d_అడుగు /= పరిమాణం(d_దత్తాంశం[0]);

    ఒకవేళ( !c_దత్తాంశం_2 )
        c_step0 = c_step1 = 0;
    లేకపోతే ఒకవేళ( !(flags & GEMM_3_T) )
        c_step0 = c_అడుగు, c_step1 = 1;
    లేకపోతే
        c_step0 = 1, c_step1 = c_అడుగు;

    కొరకు( ; d_కొలత.ఎత్తు--; c_దత్తాంశం += c_step0, d_buf += d_buf_అడుగు, d_దత్తాంశం += d_అడుగు )
    {
        ఒకవేళ( c_దత్తాంశం )
        {
            c_దత్తాంశం_2 = c_దత్తాంశం;
            j=0;
             #ఒకవేళ CV_ENABLE_UNROLLED
            కొరకు(; j <= d_కొలత.వెడల్పు - 4; j += 4, c_దత్తాంశం_2 += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_దత్తాంశం_2[0]);
                t1 += beta*WT(c_దత్తాంశం_2[c_step1]);
                d_దత్తాంశం[j] = T(t0);
                d_దత్తాంశం[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_దత్తాంశం_2[c_step1*2]);
                t1 += beta*WT(c_దత్తాంశం_2[c_step1*3]);
                d_దత్తాంశం[j+2] = T(t0);
                d_దత్తాంశం[j+3] = T(t1);
            }
            #అయితే_ముగింపు
            కొరకు( ; j < d_కొలత.వెడల్పు; j++, c_దత్తాంశం_2 += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_దత్తాంశం[j] = T(t0 + WT(c_దత్తాంశం_2[0])*beta);
            }
        }
        లేకపోతే
        {
            j = 0;
             #ఒకవేళ CV_ENABLE_UNROLLED
            కొరకు( ; j <= d_కొలత.వెడల్పు - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_దత్తాంశం[j] = T(t0);
                d_దత్తాంశం[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_దత్తాంశం[j+2] = T(t0);
                d_దత్తాంశం[j+3] = T(t1);
            }
            #అయితే_ముగింపు
            కొరకు( ; j < d_కొలత.వెడల్పు; j++ )
                d_దత్తాంశం[j] = T(alpha*d_buf[j]);
        }
    }
}


రకనిర్వచనం శూన్య (*GEMMSingleMulFunc)( స్థిరరాశి శూన్య* src1, size_t step1,
                   స్థిరరాశి శూన్య* src2, size_t step2, స్థిరరాశి శూన్య* src3, size_t step3,
                   శూన్య* dst, size_t dststep, కొలత srcsize, కొలత dstsize,
                   ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags );

రకనిర్వచనం శూన్య (*GEMMBlockMulFunc)( స్థిరరాశి శూన్య* src1, size_t step1,
                   స్థిరరాశి శూన్య* src2, size_t step2, శూన్య* dst, size_t dststep,
                   కొలత srcsize, కొలత dstsize, పూర్ణాంకం flags );

రకనిర్వచనం శూన్య (*GEMMStoreFunc)( స్థిరరాశి శూన్య* src1, size_t step1,
                   స్థిరరాశి శూన్య* src2, size_t step2, శూన్య* dst, size_t dststep,
                   కొలత dstsize, ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags );

మూస<రకం_పేరు T, రకం_పేరు WT, శూన్య (*fn)(స్థిరరాశి T*, size_t, స్థిరరాశి T*, size_t, స్థిరరాశి T*, size_t, T*, size_t, కొలత, కొలత, ద్విగుణ, ద్విగుణ, పూర్ణాంకం)>
స్థిర శూన్య gemmSingleMulWrap( స్థిరరాశి శూన్య* src1, size_t step1,
                   స్థిరరాశి శూన్య* src2, size_t step2, స్థిరరాశి శూన్య* src3, size_t step3,
                   శూన్య* dst, size_t dststep, కొలత srcsize, కొలత dstsize,
                   ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    fn((స్థిరరాశి T*)src1, step1, (స్థిరరాశి T*)src2, step2, (స్థిరరాశి T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

మూస<రకం_పేరు T, రకం_పేరు WT, శూన్య (*fn)(స్థిరరాశి T*, size_t, స్థిరరాశి T*, size_t, WT*, size_t, కొలత, కొలత, పూర్ణాంకం)>
స్థిర శూన్య gemm_ఖండం_Mul_Wrap( స్థిరరాశి శూన్య* src1, size_t step1,
                   స్థిరరాశి శూన్య* src2, size_t step2, శూన్య* dst, size_t dststep,
                   కొలత srcsize, కొలత dstsize, పూర్ణాంకం flags )
{
    fn((స్థిరరాశి T*)src1, step1, (స్థిరరాశి T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

మూస<రకం_పేరు T, రకం_పేరు WT, శూన్య (*fn)(స్థిరరాశి T*, size_t, స్థిరరాశి WT*, size_t, T*, size_t, కొలత, ద్విగుణ, ద్విగుణ, పూర్ణాంకం)>
స్థిర శూన్య gemmStoreWrap( స్థిరరాశి శూన్య* src1, size_t step1,
                   స్థిరరాశి శూన్య* src2, size_t step2, శూన్య* dst, size_t dststep,
                   కొలత dstsize, ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    fn((స్థిరరాశి T*)src1, step1, (స్థిరరాశి WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

స్థిర శూన్య GEMMSingleMul_32f( స్థిరరాశి దశాంశం* a_దత్తాంశం, size_t a_అడుగు,
              స్థిరరాశి దశాంశం* b_దత్తాంశం, size_t b_అడుగు,
              స్థిరరాశి దశాంశం* c_దత్తాంశం_2, size_t c_అడుగు,
              దశాంశం* d_దత్తాంశం, size_t d_అడుగు,
              కొలత a_కొలత, కొలత d_కొలత,
              ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    GEMMSingleMul<దశాంశం,ద్విగుణ>(a_దత్తాంశం, a_అడుగు, b_దత్తాంశం, b_అడుగు, c_దత్తాంశం_2,
                                c_అడుగు, d_దత్తాంశం, d_అడుగు, a_కొలత, d_కొలత,
                                alpha, beta, flags);
}

స్థిర శూన్య GEMMSingleMul_64f( స్థిరరాశి ద్విగుణ* a_దత్తాంశం, size_t a_అడుగు,
                              స్థిరరాశి ద్విగుణ* b_దత్తాంశం, size_t b_అడుగు,
                              స్థిరరాశి ద్విగుణ* c_దత్తాంశం_2, size_t c_అడుగు,
                              ద్విగుణ* d_దత్తాంశం, size_t d_అడుగు,
                              కొలత a_కొలత, కొలత d_కొలత,
                              ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    GEMMSingleMul<ద్విగుణ,ద్విగుణ>(a_దత్తాంశం, a_అడుగు, b_దత్తాంశం, b_అడుగు, c_దత్తాంశం_2,
                                c_అడుగు, d_దత్తాంశం, d_అడుగు, a_కొలత, d_కొలత,
                                alpha, beta, flags);
}


స్థిర శూన్య GEMMSingleMul_32fc( స్థిరరాశి Complexf* a_దత్తాంశం, size_t a_అడుగు,
                              స్థిరరాశి Complexf* b_దత్తాంశం, size_t b_అడుగు,
                              స్థిరరాశి Complexf* c_దత్తాంశం_2, size_t c_అడుగు,
                              Complexf* d_దత్తాంశం, size_t d_అడుగు,
                              కొలత a_కొలత, కొలత d_కొలత,
                              ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_దత్తాంశం, a_అడుగు, b_దత్తాంశం, b_అడుగు, c_దత్తాంశం_2,
                                c_అడుగు, d_దత్తాంశం, d_అడుగు, a_కొలత, d_కొలత,
                                alpha, beta, flags);
}

స్థిర శూన్య GEMMSingleMul_64fc( స్థిరరాశి Complexd* a_దత్తాంశం, size_t a_అడుగు,
                              స్థిరరాశి Complexd* b_దత్తాంశం, size_t b_అడుగు,
                              స్థిరరాశి Complexd* c_దత్తాంశం_2, size_t c_అడుగు,
                              Complexd* d_దత్తాంశం, size_t d_అడుగు,
                              కొలత a_కొలత, కొలత d_కొలత,
                              ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_దత్తాంశం, a_అడుగు, b_దత్తాంశం, b_అడుగు, c_దత్తాంశం_2,
                                 c_అడుగు, d_దత్తాంశం, d_అడుగు, a_కొలత, d_కొలత,
                                 alpha, beta, flags);
}

స్థిర శూన్య GEMMBlockMul_32f( స్థిరరాశి దశాంశం* a_దత్తాంశం, size_t a_అడుగు,
             స్థిరరాశి దశాంశం* b_దత్తాంశం, size_t b_అడుగు,
             ద్విగుణ* d_దత్తాంశం, size_t d_అడుగు,
             కొలత a_కొలత, కొలత d_కొలత, పూర్ణాంకం flags )
{
    GEMMBlockMul(a_దత్తాంశం, a_అడుగు, b_దత్తాంశం, b_అడుగు, d_దత్తాంశం, d_అడుగు, a_కొలత, d_కొలత, flags);
}


స్థిర శూన్య GEMMBlockMul_64f( స్థిరరాశి ద్విగుణ* a_దత్తాంశం, size_t a_అడుగు,
                             స్థిరరాశి ద్విగుణ* b_దత్తాంశం, size_t b_అడుగు,
                             ద్విగుణ* d_దత్తాంశం, size_t d_అడుగు,
                             కొలత a_కొలత, కొలత d_కొలత, పూర్ణాంకం flags )
{
    GEMMBlockMul(a_దత్తాంశం, a_అడుగు, b_దత్తాంశం, b_అడుగు, d_దత్తాంశం, d_అడుగు, a_కొలత, d_కొలత, flags);
}


స్థిర శూన్య GEMMBlockMul_32fc( స్థిరరాశి Complexf* a_దత్తాంశం, size_t a_అడుగు,
                             స్థిరరాశి Complexf* b_దత్తాంశం, size_t b_అడుగు,
                             Complexd* d_దత్తాంశం, size_t d_అడుగు,
                             కొలత a_కొలత, కొలత d_కొలత, పూర్ణాంకం flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_దత్తాంశం, a_అడుగు, b_దత్తాంశం, b_అడుగు, d_దత్తాంశం, d_అడుగు, a_కొలత, d_కొలత, flags);
}


స్థిర శూన్య GEMMBlockMul_64fc( స్థిరరాశి Complexd* a_దత్తాంశం, size_t a_అడుగు,
                             స్థిరరాశి Complexd* b_దత్తాంశం, size_t b_అడుగు,
                             Complexd* d_దత్తాంశం, size_t d_అడుగు,
                             కొలత a_కొలత, కొలత d_కొలత, పూర్ణాంకం flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_దత్తాంశం, a_అడుగు, b_దత్తాంశం, b_అడుగు, d_దత్తాంశం, d_అడుగు, a_కొలత, d_కొలత, flags);
}


స్థిర శూన్య GEMMStore_32f( స్థిరరాశి దశాంశం* c_దత్తాంశం_2, size_t c_అడుగు,
          స్థిరరాశి ద్విగుణ* d_buf, size_t d_buf_అడుగు,
          దశాంశం* d_దత్తాంశం, size_t d_అడుగు, కొలత d_కొలత,
          ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    GEMMStore(c_దత్తాంశం_2, c_అడుగు, d_buf, d_buf_అడుగు, d_దత్తాంశం, d_అడుగు, d_కొలత, alpha, beta, flags);
}


స్థిర శూన్య GEMMStore_64f( స్థిరరాశి ద్విగుణ* c_దత్తాంశం_2, size_t c_అడుగు,
                      స్థిరరాశి ద్విగుణ* d_buf, size_t d_buf_అడుగు,
                      ద్విగుణ* d_దత్తాంశం, size_t d_అడుగు, కొలత d_కొలత,
                      ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    GEMMStore(c_దత్తాంశం_2, c_అడుగు, d_buf, d_buf_అడుగు, d_దత్తాంశం, d_అడుగు, d_కొలత, alpha, beta, flags);
}


స్థిర శూన్య GEMMStore_32fc( స్థిరరాశి Complexf* c_దత్తాంశం_2, size_t c_అడుగు,
                          స్థిరరాశి Complexd* d_buf, size_t d_buf_అడుగు,
                          Complexf* d_దత్తాంశం, size_t d_అడుగు, కొలత d_కొలత,
                          ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    GEMMStore(c_దత్తాంశం_2, c_అడుగు, d_buf, d_buf_అడుగు, d_దత్తాంశం, d_అడుగు, d_కొలత, alpha, beta, flags);
}


స్థిర శూన్య GEMMStore_64fc( స్థిరరాశి Complexd* c_దత్తాంశం_2, size_t c_అడుగు,
                          స్థిరరాశి Complexd* d_buf, size_t d_buf_అడుగు,
                          Complexd* d_దత్తాంశం, size_t d_అడుగు, కొలత d_కొలత,
                          ద్విగుణ alpha, ద్విగుణ beta, పూర్ణాంకం flags )
{
    GEMMStore(c_దత్తాంశం_2, c_అడుగు, d_buf, d_buf_అడుగు, d_దత్తాంశం, d_అడుగు, d_కొలత, alpha, beta, flags);
}

స్థిర శూన్య gemmImpl( Mat A, Mat B, ద్విగుణ alpha,
           Mat C, ద్విగుణ beta, Mat D, పూర్ణాంకం flags )
{
    CV_INSTRUMENT_REGION();

    స్థిరరాశి పూర్ణాంకం ఖండం_lin_కొలత = 128;
    స్థిరరాశి పూర్ణాంకం ఖండం_కొలత_3 = ఖండం_lin_కొలత * ఖండం_lin_కొలత;

    స్థిర ద్విగుణ zero[] = {0,0,0,0};
    స్థిర దశాంశం zerof[] = {0,0,0,0};

    కొలత a_కొలత = A.size(), d_కొలత;
    పూర్ణాంకం i, len = 0, type = A.type();

    ఎంపిక( flags & (GEMM_1_T|GEMM_2_T) )
    {
    స్థితి 0:
        d_కొలత = కొలత( B.నిలువుల_సంఖ్య, a_కొలత.ఎత్తు );
        len = B.అడ్డువరుసలు;
        ఆపు;
    స్థితి 1:
        d_కొలత = కొలత( B.నిలువుల_సంఖ్య, a_కొలత.వెడల్పు );
        len = B.అడ్డువరుసలు;
        ఆపు;
    స్థితి 2:
        d_కొలత = కొలత( B.అడ్డువరుసలు, a_కొలత.ఎత్తు );
        len = B.నిలువుల_సంఖ్య;
        ఆపు;
    స్థితి 3:
        d_కొలత = కొలత( B.అడ్డువరుసలు, a_కొలత.వెడల్పు );
        len = B.నిలువుల_సంఖ్య;
        ఆపు;
    }

    ఒకవేళ( flags == 0 && 2 <= len && len <= 4 && (len == d_కొలత.వెడల్పు || len == d_కొలత.ఎత్తు) )
    {
        ఒకవేళ( type == CV_32F )
        {
            దశాంశం* d = D.ptr<దశాంశం>();
            స్థిరరాశి దశాంశం *a = A.ptr<దశాంశం>(),
                        *b = B.ptr<దశాంశం>(),
                        *c = (స్థిరరాశి దశాంశం*)C.దత్తాంశం;
            size_t d_అడుగు = D.అడుగు/పరిమాణం(d[0]),
                a_అడుగు = A.అడుగు/పరిమాణం(a[0]),
                b_అడుగు = B.అడుగు/పరిమాణం(b[0]),
                c_అడుగు = C.దత్తాంశం ? C.అడుగు/పరిమాణం(c[0]) : 0;

            ఒకవేళ( !c )
                c = zerof;

            ఎంపిక( len )
            {
            స్థితి 2:
                ఒకవేళ( len == d_కొలత.వెడల్పు && b != d )
                {
                    కొరకు( i = 0; i < d_కొలత.ఎత్తు; i++, d += d_అడుగు, a += a_అడుగు, c += c_అడుగు )
                    {
                        దశాంశం t0 = a[0]*b[0] + a[1]*b[b_అడుగు];
                        దశాంశం t1 = a[0]*b[1] + a[1]*b[b_అడుగు+1];
                        d[0] = (దశాంశం)(t0*alpha + c[0]*beta);
                        d[1] = (దశాంశం)(t1*alpha + c[1]*beta);
                    }
                }
                లేకపోతే ఒకవేళ( a != d )
                {
                    పూర్ణాంకం c_step0 = 1;
                    ఒకవేళ( c == zerof )
                    {
                        c_step0 = 0;
                        c_అడుగు = 1;
                    }

                    కొరకు( i = 0; i < d_కొలత.వెడల్పు; i++, d++, b++, c += c_step0 )
                    {
                        దశాంశం t0 = a[0]*b[0] + a[1]*b[b_అడుగు];
                        దశాంశం t1 = a[a_అడుగు]*b[0] + a[a_అడుగు+1]*b[b_అడుగు];
                        d[0] = (దశాంశం)(t0*alpha + c[0]*beta);
                        d[d_అడుగు] = (దశాంశం)(t1*alpha + c[c_అడుగు]*beta);
                    }
                }
                లేకపోతే
                    ఆపు;
                తిరిగి;
            స్థితి 3:
                ఒకవేళ( len == d_కొలత.వెడల్పు && b != d )
                {
                    కొరకు( i = 0; i < d_కొలత.ఎత్తు; i++, d += d_అడుగు, a += a_అడుగు, c += c_అడుగు )
                    {
                        దశాంశం t0 = a[0]*b[0] + a[1]*b[b_అడుగు] + a[2]*b[b_అడుగు*2];
                        దశాంశం t1 = a[0]*b[1] + a[1]*b[b_అడుగు+1] + a[2]*b[b_అడుగు*2+1];
                        దశాంశం t2 = a[0]*b[2] + a[1]*b[b_అడుగు+2] + a[2]*b[b_అడుగు*2+2];
                        d[0] = (దశాంశం)(t0*alpha + c[0]*beta);
                        d[1] = (దశాంశం)(t1*alpha + c[1]*beta);
                        d[2] = (దశాంశం)(t2*alpha + c[2]*beta);
                    }
                }
                లేకపోతే ఒకవేళ( a != d )
                {
                    పూర్ణాంకం c_step0 = 1;
                    ఒకవేళ( c == zerof )
                    {
                        c_step0 = 0;
                        c_అడుగు = 1;
                    }

                    కొరకు( i = 0; i < d_కొలత.వెడల్పు; i++, d++, b++, c += c_step0 )
                    {
                        దశాంశం t0 = a[0]*b[0] + a[1]*b[b_అడుగు] + a[2]*b[b_అడుగు*2];
                        దశాంశం t1 = a[a_అడుగు]*b[0] + a[a_అడుగు+1]*b[b_అడుగు] + a[a_అడుగు+2]*b[b_అడుగు*2];
                        దశాంశం t2 = a[a_అడుగు*2]*b[0] + a[a_అడుగు*2+1]*b[b_అడుగు] + a[a_అడుగు*2+2]*b[b_అడుగు*2];

                        d[0] = (దశాంశం)(t0*alpha + c[0]*beta);
                        d[d_అడుగు] = (దశాంశం)(t1*alpha + c[c_అడుగు]*beta);
                        d[d_అడుగు*2] = (దశాంశం)(t2*alpha + c[c_అడుగు*2]*beta);
                    }
                }
                లేకపోతే
                    ఆపు;
                తిరిగి;
            స్థితి 4:
                ఒకవేళ( len == d_కొలత.వెడల్పు && b != d )
                {
                    కొరకు( i = 0; i < d_కొలత.ఎత్తు; i++, d += d_అడుగు, a += a_అడుగు, c += c_అడుగు )
                    {
                        దశాంశం t0 = a[0]*b[0] + a[1]*b[b_అడుగు] + a[2]*b[b_అడుగు*2] + a[3]*b[b_అడుగు*3];
                        దశాంశం t1 = a[0]*b[1] + a[1]*b[b_అడుగు+1] + a[2]*b[b_అడుగు*2+1] + a[3]*b[b_అడుగు*3+1];
                        దశాంశం t2 = a[0]*b[2] + a[1]*b[b_అడుగు+2] + a[2]*b[b_అడుగు*2+2] + a[3]*b[b_అడుగు*3+2];
                        దశాంశం t3 = a[0]*b[3] + a[1]*b[b_అడుగు+3] + a[2]*b[b_అడుగు*2+3] + a[3]*b[b_అడుగు*3+3];
                        d[0] = (దశాంశం)(t0*alpha + c[0]*beta);
                        d[1] = (దశాంశం)(t1*alpha + c[1]*beta);
                        d[2] = (దశాంశం)(t2*alpha + c[2]*beta);
                        d[3] = (దశాంశం)(t3*alpha + c[3]*beta);
                    }
                }
                లేకపోతే ఒకవేళ( len <= 16 && a != d )
                {
                    పూర్ణాంకం c_step0 = 1;
                    ఒకవేళ( c == zerof )
                    {
                        c_step0 = 0;
                        c_అడుగు = 1;
                    }

                    కొరకు( i = 0; i < d_కొలత.వెడల్పు; i++, d++, b++, c += c_step0 )
                    {
                        దశాంశం t0 = a[0]*b[0] + a[1]*b[b_అడుగు] + a[2]*b[b_అడుగు*2] + a[3]*b[b_అడుగు*3];
                        దశాంశం t1 = a[a_అడుగు]*b[0] + a[a_అడుగు+1]*b[b_అడుగు] +
                                   a[a_అడుగు+2]*b[b_అడుగు*2] + a[a_అడుగు+3]*b[b_అడుగు*3];
                        దశాంశం t2 = a[a_అడుగు*2]*b[0] + a[a_అడుగు*2+1]*b[b_అడుగు] +
                                   a[a_అడుగు*2+2]*b[b_అడుగు*2] + a[a_అడుగు*2+3]*b[b_అడుగు*3];
                        దశాంశం t3 = a[a_అడుగు*3]*b[0] + a[a_అడుగు*3+1]*b[b_అడుగు] +
                                   a[a_అడుగు*3+2]*b[b_అడుగు*2] + a[a_అడుగు*3+3]*b[b_అడుగు*3];
                        d[0] = (దశాంశం)(t0*alpha + c[0]*beta);
                        d[d_అడుగు] = (దశాంశం)(t1*alpha + c[c_అడుగు]*beta);
                        d[d_అడుగు*2] = (దశాంశం)(t2*alpha + c[c_అడుగు*2]*beta);
                        d[d_అడుగు*3] = (దశాంశం)(t3*alpha + c[c_అడుగు*3]*beta);
                    }
                }
                లేకపోతే
                    ఆపు;
                తిరిగి;
            }
        }

        ఒకవేళ( type == CV_64F )
        {
            ద్విగుణ* d = D.ptr<ద్విగుణ>();
            స్థిరరాశి ద్విగుణ *a = A.ptr<ద్విగుణ>(),
                         *b = B.ptr<ద్విగుణ>(),
                         *c = (స్థిరరాశి ద్విగుణ*)C.దత్తాంశం;
            size_t d_అడుగు = D.అడుగు/పరిమాణం(d[0]),
                a_అడుగు = A.అడుగు/పరిమాణం(a[0]),
                b_అడుగు = B.అడుగు/పరిమాణం(b[0]),
                c_అడుగు = C.దత్తాంశం ? C.అడుగు/పరిమాణం(c[0]) : 0;
            ఒకవేళ( !c )
                c = zero;

            ఎంపిక( len )
            {
            స్థితి 2:
                ఒకవేళ( len == d_కొలత.వెడల్పు && b != d )
                {
                    కొరకు( i = 0; i < d_కొలత.ఎత్తు; i++, d += d_అడుగు, a += a_అడుగు, c += c_అడుగు )
                    {
                        ద్విగుణ t0 = a[0]*b[0] + a[1]*b[b_అడుగు];
                        ద్విగుణ t1 = a[0]*b[1] + a[1]*b[b_అడుగు+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                లేకపోతే ఒకవేళ( a != d )
                {
                    పూర్ణాంకం c_step0 = 1;
                    ఒకవేళ( c == zero )
                    {
                        c_step0 = 0;
                        c_అడుగు = 1;
                    }

                    కొరకు( i = 0; i < d_కొలత.వెడల్పు; i++, d++, b++, c += c_step0 )
                    {
                        ద్విగుణ t0 = a[0]*b[0] + a[1]*b[b_అడుగు];
                        ద్విగుణ t1 = a[a_అడుగు]*b[0] + a[a_అడుగు+1]*b[b_అడుగు];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_అడుగు] = t1*alpha + c[c_అడుగు]*beta;
                    }
                }
                లేకపోతే
                    ఆపు;
                తిరిగి;
            స్థితి 3:
                ఒకవేళ( len == d_కొలత.వెడల్పు && b != d )
                {
                    కొరకు( i = 0; i < d_కొలత.ఎత్తు; i++, d += d_అడుగు, a += a_అడుగు, c += c_అడుగు )
                    {
                        ద్విగుణ t0 = a[0]*b[0] + a[1]*b[b_అడుగు] + a[2]*b[b_అడుగు*2];
                        ద్విగుణ t1 = a[0]*b[1] + a[1]*b[b_అడుగు+1] + a[2]*b[b_అడుగు*2+1];
                        ద్విగుణ t2 = a[0]*b[2] + a[1]*b[b_అడుగు+2] + a[2]*b[b_అడుగు*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                లేకపోతే ఒకవేళ( a != d )
                {
                    పూర్ణాంకం c_step0 = 1;
                    ఒకవేళ( c == zero )
                    {
                        c_step0 = 0;
                        c_అడుగు = 1;
                    }

                    కొరకు( i = 0; i < d_కొలత.వెడల్పు; i++, d++, b++, c += c_step0 )
                    {
                        ద్విగుణ t0 = a[0]*b[0] + a[1]*b[b_అడుగు] + a[2]*b[b_అడుగు*2];
                        ద్విగుణ t1 = a[a_అడుగు]*b[0] + a[a_అడుగు+1]*b[b_అడుగు] + a[a_అడుగు+2]*b[b_అడుగు*2];
                        ద్విగుణ t2 = a[a_అడుగు*2]*b[0] + a[a_అడుగు*2+1]*b[b_అడుగు] + a[a_అడుగు*2+2]*b[b_అడుగు*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_అడుగు] = t1*alpha + c[c_అడుగు]*beta;
                        d[d_అడుగు*2] = t2*alpha + c[c_అడుగు*2]*beta;
                    }
                }
                లేకపోతే
                    ఆపు;
                తిరిగి;
            స్థితి 4:
                ఒకవేళ( len == d_కొలత.వెడల్పు && b != d )
                {
                    కొరకు( i = 0; i < d_కొలత.ఎత్తు; i++, d += d_అడుగు, a += a_అడుగు, c += c_అడుగు )
                    {
                        ద్విగుణ t0 = a[0]*b[0] + a[1]*b[b_అడుగు] + a[2]*b[b_అడుగు*2] + a[3]*b[b_అడుగు*3];
                        ద్విగుణ t1 = a[0]*b[1] + a[1]*b[b_అడుగు+1] + a[2]*b[b_అడుగు*2+1] + a[3]*b[b_అడుగు*3+1];
                        ద్విగుణ t2 = a[0]*b[2] + a[1]*b[b_అడుగు+2] + a[2]*b[b_అడుగు*2+2] + a[3]*b[b_అడుగు*3+2];
                        ద్విగుణ t3 = a[0]*b[3] + a[1]*b[b_అడుగు+3] + a[2]*b[b_అడుగు*2+3] + a[3]*b[b_అడుగు*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                లేకపోతే ఒకవేళ( d_కొలత.వెడల్పు <= 16 && a != d )
                {
                    పూర్ణాంకం c_step0 = 1;
                    ఒకవేళ( c == zero )
                    {
                        c_step0 = 0;
                        c_అడుగు = 1;
                    }

                    కొరకు( i = 0; i < d_కొలత.వెడల్పు; i++, d++, b++, c += c_step0 )
                    {
                        ద్విగుణ t0 = a[0]*b[0] + a[1]*b[b_అడుగు] + a[2]*b[b_అడుగు*2] + a[3]*b[b_అడుగు*3];
                        ద్విగుణ t1 = a[a_అడుగు]*b[0] + a[a_అడుగు+1]*b[b_అడుగు] +
                                    a[a_అడుగు+2]*b[b_అడుగు*2] + a[a_అడుగు+3]*b[b_అడుగు*3];
                        ద్విగుణ t2 = a[a_అడుగు*2]*b[0] + a[a_అడుగు*2+1]*b[b_అడుగు] +
                                    a[a_అడుగు*2+2]*b[b_అడుగు*2] + a[a_అడుగు*2+3]*b[b_అడుగు*3];
                        ద్విగుణ t3 = a[a_అడుగు*3]*b[0] + a[a_అడుగు*3+1]*b[b_అడుగు] +
                                    a[a_అడుగు*3+2]*b[b_అడుగు*2] + a[a_అడుగు*3+3]*b[b_అడుగు*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_అడుగు] = t1*alpha + c[c_అడుగు]*beta;
                        d[d_అడుగు*2] = t2*alpha + c[c_అడుగు*2]*beta;
                        d[d_అడుగు*3] = t3*alpha + c[c_అడుగు*3]*beta;
                    }
                }
                లేకపోతే
                    ఆపు;
                తిరిగి;
            }
        }
    }

    {
    size_t b_అడుగు = B.అడుగు;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc ఖండం_Mul_Func;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    స్థిరరాశి uchar* Cdata = C.దత్తాంశం;
    size_t Cstep = C.దత్తాంశం ? (size_t)C.అడుగు : 0;
    Auto_బఫరు<uchar> buf;

    ఒకవేళ( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<దశాంశం, ద్విగుణ, GEMMSingleMul_32f>;
        ఖండం_Mul_Func = gemm_ఖండం_Mul_Wrap<దశాంశం, ద్విగుణ, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<దశాంశం, ద్విగుణ, GEMMStore_32f>;
    }
    లేకపోతే ఒకవేళ( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ద్విగుణ, ద్విగుణ, GEMMSingleMul_64f>;
        ఖండం_Mul_Func = gemm_ఖండం_Mul_Wrap<ద్విగుణ, ద్విగుణ, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<ద్విగుణ, ద్విగుణ, GEMMStore_64f>;
    }
    లేకపోతే ఒకవేళ( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        ఖండం_Mul_Func = gemm_ఖండం_Mul_Wrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    లేకపోతే
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        ఖండం_Mul_Func = gemm_ఖండం_Mul_Wrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    ఒకవేళ( (d_కొలత.వెడల్పు == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_అడుగు = d_కొలత.వెడల్పు == 1 ? 0 : CV_ELEM_కొలత(type);
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
    else*/ ఒకవేళ( ((d_కొలత.ఎత్తు <= ఖండం_lin_కొలత/2 || d_కొలత.వెడల్పు <= ఖండం_lin_కొలత/2) &&
        len <= 10000) || len <= 10 ||
        (d_కొలత.వెడల్పు <= ఖండం_lin_కొలత &&
        d_కొలత.ఎత్తు <= ఖండం_lin_కొలత && len <= ఖండం_lin_కొలత) )
    {
        singleMulFunc( A.ptr(), A.అడుగు, B.ptr(), b_అడుగు, Cdata, Cstep,
                       matD->ptr(), matD->అడుగు, a_కొలత, d_కొలత, alpha, beta, flags );
    }
    లేకపోతే
    {
        పూర్ణాంకం is_a_t = flags & GEMM_1_T;
        పూర్ణాంకం is_b_t = flags & GEMM_2_T;
        పూర్ణాంకం elem_కొలత_2 = CV_ELEM_కొలత(type);
        పూర్ణాంకం dk0_1, dk0_2;
        size_t a_buf_కొలత = 0, b_buf_కొలత, d_buf_కొలత;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        పూర్ణాంకం j, k, di = 0, dj = 0, dk = 0;
        పూర్ణాంకం dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        పూర్ణాంకం work_elem_కొలత = elem_కొలత_2 << (CV_MAT_లోతు(type) == CV_32F ? 1 : 0);

        ఒకవేళ( !is_a_t )
            a_step0 = A.అడుగు, a_step1 = elem_కొలత_2;
        లేకపోతే
            a_step0 = elem_కొలత_2, a_step1 = A.అడుగు;

        ఒకవేళ( !is_b_t )
            b_step0 = b_అడుగు, b_step1 = elem_కొలత_2;
        లేకపోతే
            b_step0 = elem_కొలత_2, b_step1 = b_అడుగు;

        ఒకవేళ( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        లేకపోతే ఒకవేళ( !(flags & GEMM_3_T) )
            c_step0 = C.అడుగు, c_step1 = elem_కొలత_2;
        లేకపోతే
            c_step0 = elem_కొలత_2, c_step1 = C.అడుగు;

        dm0 = std::min( ఖండం_lin_కొలత, d_కొలత.ఎత్తు );
        dn0 = std::min( ఖండం_lin_కొలత, d_కొలత.వెడల్పు );
        dk0_1 = ఖండం_కొలత_3 / dm0;
        dk0_2 = ఖండం_కొలత_3 / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        ఒకవేళ( dk0*dm0 > ఖండం_కొలత_3 )
            dm0 = ఖండం_కొలత_3 / dk0;
        ఒకవేళ( dk0*dn0 > ఖండం_కొలత_3 )
            dn0 = ఖండం_కొలత_3 / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_కొలత = (size_t)(dk0+dk0/8+1)*dk0_1*elem_కొలత_2;
        d_buf_కొలత = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_కొలత;

        ఒకవేళ( is_a_t )
        {
            a_buf_కొలత = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_కొలత_2;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_కొలత + b_buf_కొలత + a_buf_కొలత);
        d_buf = buf.దత్తాంశం();
        b_buf = d_buf + d_buf_కొలత;

        ఒకవేళ( is_a_t )
            a_buf = b_buf + b_buf_కొలత;

        కొరకు( i = 0; i < d_కొలత.ఎత్తు; i += di )
        {
            di = dm0;
            ఒకవేళ( i + di >= d_కొలత.ఎత్తు || 8*(i + di) + di > 8*d_కొలత.ఎత్తు )
                di = d_కొలత.ఎత్తు - i;

            కొరకు( j = 0; j < d_కొలత.వెడల్పు; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->అడుగు + j*elem_కొలత_2;
                స్థిరరాశి uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t d_అడుగు_2 = matD->అడుగు;
                dj = dn0;

                ఒకవేళ( j + dj >= d_కొలత.వెడల్పు || 8*(j + dj) + dj > 8*d_కొలత.వెడల్పు )
                    dj = d_కొలత.వెడల్పు - j;

                flags &= 15;
                ఒకవేళ( dk0 < len )
                {
                    _d = d_buf;
                    d_అడుగు_2 = dj*work_elem_కొలత;
                }

                కొరకు( k = 0; k < len; k += dk )
                {
                    స్థిరరాశి uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t a_అడుగు_2 = A.అడుగు;
                    స్థిరరాశి uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t b_అడుగు_2 = b_అడుగు;
                    కొలత a_bl_కొలత;

                    dk = dk0;
                    ఒకవేళ( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    ఒకవేళ( !is_a_t )
                        a_bl_కొలత.వెడల్పు = dk, a_bl_కొలత.ఎత్తు = di;
                    లేకపోతే
                        a_bl_కొలత.వెడల్పు = di, a_bl_కొలత.ఎత్తు = dk;

                    ఒకవేళ( a_buf && is_a_t )
                    {
                        a_అడుగు_2 = dk*elem_కొలత_2;
                        GEMM_Transpose_ఖండం( _a, A.అడుగు, a_buf, a_అడుగు_2, a_bl_కొలత, elem_కొలత_2 );
                        std::swap( a_bl_కొలత.వెడల్పు, a_bl_కొలత.ఎత్తు );
                        _a = a_buf;
                    }

                    ఒకవేళ( dj < d_కొలత.వెడల్పు )
                    {
                        కొలత b_కొలత;
                        ఒకవేళ( !is_b_t )
                            b_కొలత.వెడల్పు = dj, b_కొలత.ఎత్తు = dk;
                        లేకపోతే
                            b_కొలత.వెడల్పు = dk, b_కొలత.ఎత్తు = dj;

                        b_అడుగు_2 = b_కొలత.వెడల్పు*elem_కొలత_2;
                        GEMM_Copy_ఖండం( _b, b_అడుగు, b_buf, b_అడుగు_2, b_కొలత, elem_కొలత_2 );
                        _b = b_buf;
                    }

                    ఒకవేళ( dk0 < len )
                        ఖండం_Mul_Func( _a, a_అడుగు_2, _b, b_అడుగు_2, _d, d_అడుగు_2,
                                      a_bl_కొలత, కొలత(dj,di), flags );
                    లేకపోతే
                        singleMulFunc( _a, a_అడుగు_2, _b, b_అడుగు_2, _c, Cstep,
                                       _d, d_అడుగు_2, a_bl_కొలత, కొలత(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                ఒకవేళ( dk0 < len )
                    storeFunc( _c, Cstep, _d, d_అడుగు_2,
                               matD->ptr(i) + j*elem_కొలత_2,
                               matD->అడుగు, కొలత(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

మూస <రకం_పేరు fptype>అంతఃవరుస స్థిర శూన్య
callGemmImpl(స్థిరరాశి fptype *src1, size_t src1_అడుగు, స్థిరరాశి fptype *src2, size_t src2_అడుగు, fptype alpha,
          స్థిరరాశి fptype *src3, size_t src3_అడుగు, fptype beta, fptype *dst, size_t dst_అడుగు, పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags, పూర్ణాంకం type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    పూర్ణాంకం b_m, b_n, c_m, c_n, m_d;

    ఒకవేళ(flags & GEMM_2_T)
    {
        b_m = n_d;
        ఒకవేళ(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        లేకపోతే
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    లేకపోతే
    {
        b_n = n_d;
        ఒకవేళ(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        లేకపోతే
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    ఒకవేళ(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    లేకపోతే
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    ఒకవేళ(src1 != ఖాళీ)
        A = Mat(m_a, n_a, type, (శూన్య*)src1, src1_అడుగు);
    ఒకవేళ(src2 != ఖాళీ)
        B = Mat(b_m, b_n, type, (శూన్య*)src2, src2_అడుగు);
    ఒకవేళ(src3 != ఖాళీ && beta != 0.0)
        C = Mat(c_m, c_n, type, (శూన్య*)src3, src3_అడుగు);
    Mat D(m_d, n_d, type, (శూన్య*)dst, dst_అడుగు);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

శూన్య gemm32f(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
             దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
             పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags, CV_32F);
}

శూన్య gemm64f(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
             ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
             పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags, CV_64F);
}

శూన్య gemm32fc(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
              దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
              పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags, CV_32FC2);
}

శూన్య gemm64fc(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
              ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
              పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags, CV_64FC2);
}

#అయితే_ముగింపు // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

మూస<రకం_పేరు T, రకం_పేరు WT> స్థిర శూన్య
transform_( స్థిరరాశి T* src, T* dst, స్థిరరాశి WT* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn )
{
    పూర్ణాంకం x;

    ఒకవేళ( scn == 2 && dcn == 2 )
    {
        కొరకు( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    లేకపోతే ఒకవేళ( scn == 3 && dcn == 3 )
    {
        కొరకు( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    లేకపోతే ఒకవేళ( scn == 3 && dcn == 1 )
    {
        కొరకు( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    లేకపోతే ఒకవేళ( scn == 4 && dcn == 4 )
    {
        కొరకు( x = 0; x < len*4; x += 4 )
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
    లేకపోతే
    {
        కొరకు( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            స్థిరరాశి WT* _m = m;
            పూర్ణాంకం j, k;
            కొరకు( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                కొరకు( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

స్థిర శూన్య
transform_8u( స్థిరరాశి uchar* src, uchar* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn )
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం బిట్లు = 10, SCALE = 1 << బిట్లు;
    స్థిరరాశి దశాంశం MAX_M = (దశాంశం)(1 << (15 - బిట్లు));

    ఒకవేళ( scn == 3 && dcn == 3 &&
        std::నిరపేక్ష(m[0]) < MAX_M && std::నిరపేక్ష(m[1]) < MAX_M && std::నిరపేక్ష(m[ 2]) < MAX_M*256 && std::నిరపేక్ష(m[ 3]) < MAX_M*256 &&
        std::నిరపేక్ష(m[4]) < MAX_M && std::నిరపేక్ష(m[5]) < MAX_M && std::నిరపేక్ష(m[ 6]) < MAX_M*256 && std::నిరపేక్ష(m[ 7]) < MAX_M*256 &&
        std::నిరపేక్ష(m[8]) < MAX_M && std::నిరపేక్ష(m[9]) < MAX_M && std::నిరపేక్ష(m[10]) < MAX_M*256 && std::నిరపేక్ష(m[11]) < MAX_M*256 )
    {
        స్థిరరాశి పూర్ణాంకం nChannels = 3;

        సంఘం {
            పొట్టి s[6];
            పూర్ణాంకం p[3];
        } m16;
        m16.s[0] = saturate_cast<పొట్టి>(m[0] * SCALE); m16.s[1] = saturate_cast<పొట్టి>(m[1] * SCALE);
        m16.s[2] = saturate_cast<పొట్టి>(m[4] * SCALE); m16.s[3] = saturate_cast<పొట్టి>(m[5] * SCALE);
        m16.s[4] = saturate_cast<పొట్టి>(m[8] * SCALE); m16.s[5] = saturate_cast<పొట్టి>(m[9] * SCALE);
        పూర్ణాంకం m32[] = {saturate_cast<పూర్ణాంకం>(m[ 2] * SCALE), saturate_cast<పూర్ణాంకం>(m[ 3] * SCALE),
                     saturate_cast<పూర్ణాంకం>(m[ 6] * SCALE), saturate_cast<పూర్ణాంకం>(m[ 7] * SCALE),
                     saturate_cast<పూర్ణాంకం>(m[10] * SCALE), saturate_cast<పూర్ణాంకం>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        పూర్ణాంకం x = 0;
        కొరకు (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
        {
            v_uint8 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
            v_uint8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_uint16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_uint16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_కూడు(v_కూడు(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack_u(dbl, dbh), v_pack_u(dgl, dgh), v_pack_u(drl, drh));
        }
        m32[1] = saturate_cast<పూర్ణాంకం>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<పూర్ణాంకం>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<పూర్ణాంకం>((m[11] + 0.5f)*SCALE);
        కొరకు( ; x < len * nChannels; x += nChannels )
        {
            పూర్ణాంకం v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> బిట్లు);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> బిట్లు);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> బిట్లు);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    transform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
transform_16u( స్థిరరాశి ushort* src, ushort* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn )
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ( scn == 3 && dcn == 3 )
    {
        పూర్ణాంకం x = 0;
#ఒకవేళ CV_SIMD_వెడల్పు > 16
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
        కొరకు (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
        {
            v_uint16 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
            v_uint32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_కూడు_wrap(v_pack(v_విడత(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m2,  m3)))),
                                   v_విడత(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m2,  m3))))), delta);
            dg = v_కూడు_wrap(v_pack(v_విడత(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m6,  m7)))),
                                   v_విడత(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m6,  m7))))), delta);
            dr = v_కూడు_wrap(v_pack(v_విడత(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)), m10, m11)))),
                                   v_విడత(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)), m10, m11))))), delta);
            v_store_interleave(dst + x, v_reinterpret_as_u16(db), v_reinterpret_as_u16(dg), v_reinterpret_as_u16(dr));
        }
#అయితే_ముగింపు
#ఒకవేళ CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_ఎడమ<1>(_m0l);
        v_float32x4 _m1h = v_rotate_ఎడమ<1>(_m1l);
        v_float32x4 _m2h = v_rotate_ఎడమ<1>(_m2l);
        v_float32x4 _m3h = v_rotate_ఎడమ<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        కొరకు( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_కుడి<1>(v_reinterpret_as_u16(v_కూడు_wrap(v_pack(
                             v_విడత(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_ఎక్కించు_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_విడత(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_ఎక్కించు_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#అయితే_ముగింపు //CV_SIMD128
        కొరకు( ; x < len * 3; x += 3 )
        {
            దశాంశం v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    transform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
transform_32f( స్థిరరాశి దశాంశం* src, దశాంశం* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    పూర్ణాంకం x = 0;
    ఒకవేళ( scn == 3 && dcn == 3 )
    {
        పూర్ణాంకం idx[VTraits<v_float32>::max_nlanes/2];
        కొరకు( పూర్ణాంకం i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        దశాంశం _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        కొరకు( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        కొరకు( ; x < len*3; x += 3 )
        {
            దశాంశం v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            దశాంశం t0 = saturate_cast<దశాంశం>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            దశాంశం t1 = saturate_cast<దశాంశం>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            దశాంశం t2 = saturate_cast<దశాంశం>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        తిరిగి;
    }

    ఒకవేళ( scn == 4 && dcn == 4 )
    {
#ఒకవేళ CV_SIMD_వెడల్పు > 16
        పూర్ణాంకం idx[VTraits<v_float32>::max_nlanes/4];
        కొరకు( పూర్ణాంకం i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        దశాంశం _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        కొరకు( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_ఎక్కించు(src + x);
            v_store(dst + x, v_కూడు(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#అయితే_ముగింపు
#ఒకవేళ CV_SIMD128
        v_float32x4 _m0 = v_ఎక్కించు(m     );
        v_float32x4 _m1 = v_ఎక్కించు(m +  5);
        v_float32x4 _m2 = v_ఎక్కించు(m + 10);
        v_float32x4 _m3 = v_ఎక్కించు(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        కొరకు( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_ఎక్కించు(src + x);
            v_store(dst + x, v_కూడు(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#లేకపోతే // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        కొరకు( ; x < len*4; x += 4 )
        {
            దశాంశం v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            దశాంశం t0 = saturate_cast<దశాంశం>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            దశాంశం t1 = saturate_cast<దశాంశం>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            దశాంశం t2 = saturate_cast<దశాంశం>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            దశాంశం t3 = saturate_cast<దశాంశం>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#అయితే_ముగింపు
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    transform_(src, dst, m, len, scn, dcn);
}


స్థిర శూన్య
transform_8s(స్థిరరాశి schar* src, schar* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం బిట్లు = 10, SCALE = 1 << బిట్లు;
    స్థిరరాశి దశాంశం MAX_M = (దశాంశం)(1 << (15 - బిట్లు));

    ఒకవేళ( scn == 3 && dcn == 3 &&
        std::నిరపేక్ష(m[0]) < MAX_M && std::నిరపేక్ష(m[1]) < MAX_M && std::నిరపేక్ష(m[ 2]) < MAX_M*256 && std::నిరపేక్ష(m[ 3]) < MAX_M*256 &&
        std::నిరపేక్ష(m[4]) < MAX_M && std::నిరపేక్ష(m[5]) < MAX_M && std::నిరపేక్ష(m[ 6]) < MAX_M*256 && std::నిరపేక్ష(m[ 7]) < MAX_M*256 &&
        std::నిరపేక్ష(m[8]) < MAX_M && std::నిరపేక్ష(m[9]) < MAX_M && std::నిరపేక్ష(m[10]) < MAX_M*256 && std::నిరపేక్ష(m[11]) < MAX_M*256 )
    {
        స్థిరరాశి పూర్ణాంకం nChannels = 3;

        సంఘం {
            పొట్టి s[6];
            పూర్ణాంకం p[3];
        } m16;
        m16.s[0] = saturate_cast<పొట్టి>(m[0] * SCALE); m16.s[1] = saturate_cast<పొట్టి>(m[1] * SCALE);
        m16.s[2] = saturate_cast<పొట్టి>(m[4] * SCALE); m16.s[3] = saturate_cast<పొట్టి>(m[5] * SCALE);
        m16.s[4] = saturate_cast<పొట్టి>(m[8] * SCALE); m16.s[5] = saturate_cast<పొట్టి>(m[9] * SCALE);
        పూర్ణాంకం m32[] = {saturate_cast<పూర్ణాంకం>(m[ 2] * SCALE), saturate_cast<పూర్ణాంకం>(m[ 3] * SCALE),
                     saturate_cast<పూర్ణాంకం>(m[ 6] * SCALE), saturate_cast<పూర్ణాంకం>(m[ 7] * SCALE),
                     saturate_cast<పూర్ణాంకం>(m[10] * SCALE), saturate_cast<పూర్ణాంకం>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        పూర్ణాంకం x = 0;
        కొరకు (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
        {
            v_int8 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
            v_int8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_int16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_int16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_కూడు(v_కూడు(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_కూడు(v_కూడు(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_కూడు(v_కూడు(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_కూడు(v_కూడు(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_కూడు(v_కూడు(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<బిట్లు>(v_కూడు(v_కూడు(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_కూడు(v_కూడు(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack(dbl, dbh), v_pack(dgl, dgh), v_pack(drl, drh));
        }
        m32[1] = saturate_cast<పూర్ణాంకం>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<పూర్ణాంకం>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<పూర్ణాంకం>((m[11] + 0.5f)*SCALE);
        కొరకు( ; x < len * nChannels; x += nChannels )
        {
            పూర్ణాంకం v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> బిట్లు);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> బిట్లు);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> బిట్లు);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    transform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
transform_16s(స్థిరరాశి పొట్టి* src, పొట్టి* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

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
        కొరకు (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
        {
            v_int16 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
            v_int32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_pack(v_విడత(v_muladd(v_cvt_f32(bl), m0, v_muladd(v_cvt_f32(gl), m1, v_muladd(v_cvt_f32(rl),  m2,  m3)))),
                        v_విడత(v_muladd(v_cvt_f32(bh), m0, v_muladd(v_cvt_f32(gh), m1, v_muladd(v_cvt_f32(rh),  m2,  m3)))));
            dg = v_pack(v_విడత(v_muladd(v_cvt_f32(bl), m4, v_muladd(v_cvt_f32(gl), m5, v_muladd(v_cvt_f32(rl),  m6,  m7)))),
                        v_విడత(v_muladd(v_cvt_f32(bh), m4, v_muladd(v_cvt_f32(gh), m5, v_muladd(v_cvt_f32(rh),  m6,  m7)))));
            dr = v_pack(v_విడత(v_muladd(v_cvt_f32(bl), m8, v_muladd(v_cvt_f32(gl), m9, v_muladd(v_cvt_f32(rl), m10, m11)))),
                        v_విడత(v_muladd(v_cvt_f32(bh), m8, v_muladd(v_cvt_f32(gh), m9, v_muladd(v_cvt_f32(rh), m10, m11)))));
            v_store_interleave(dst + x, db, dg, dr);
        }
        కొరకు( ; x < len * 3; x += 3 )
        {
            దశాంశం v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            పొట్టి t0 = saturate_cast<పొట్టి>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            పొట్టి t1 = saturate_cast<పొట్టి>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            పొట్టి t2 = saturate_cast<పొట్టి>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    transform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
transform_32s(స్థిరరాశి పూర్ణాంకం* src, పూర్ణాంకం* dst, స్థిరరాశి ద్విగుణ* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ఒకవేళ( scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

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
        కొరకు (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ఎగువ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ఎగువ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ఎగువ(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_విడత(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_విడత(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_విడత(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        కొరకు (; x < len * 3; x += 3) {
            ద్విగుణ b = src[x], g = src[x + 1], r = src[x + 2];
            పూర్ణాంకం db = saturate_cast<పూర్ణాంకం>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            పూర్ణాంకం dg = saturate_cast<పూర్ణాంకం>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            పూర్ణాంకం dr = saturate_cast<పూర్ణాంకం>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    transform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
transform_64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, స్థిరరాశి ద్విగుణ* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ఒకవేళ( scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

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
        కొరకు (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        కొరకు (; x < len * 3; x += 3) {
            ద్విగుణ b = src[x], g = src[x + 1], r = src[x + 2];
            ద్విగుణ db = saturate_cast<ద్విగుణ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ద్విగుణ dg = saturate_cast<ద్విగుణ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ద్విగుణ dr = saturate_cast<ద్విగుణ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    transform_(src, dst, m, len, scn, dcn);
}

మూస<రకం_పేరు T, రకం_పేరు WT> స్థిర శూన్య
diagtransform_( స్థిరరాశి T* src, T* dst, స్థిరరాశి WT* m, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం )
{
    పూర్ణాంకం x;

    ఒకవేళ( cn == 2 )
    {
        కొరకు( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    లేకపోతే ఒకవేళ( cn == 3 )
    {
        కొరకు( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    లేకపోతే ఒకవేళ( cn == 4 )
    {
        కొరకు( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    లేకపోతే
    {
        కొరకు( x = 0; x < len; x++, src += cn, dst += cn )
        {
            స్థిరరాశి WT* _m = m;
            కొరకు( పూర్ణాంకం j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

స్థిర శూన్య
diagtransform_8u(స్థిరరాశి uchar* src, uchar* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        కొరకు (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
            v_uint8 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack_u(v_విడత(db_f32_l0), v_విడత(db_f32_l1)), v_pack_u(v_విడత(db_f32_h0), v_విడత(db_f32_h1))),
                               v_pack(v_pack_u(v_విడత(dg_f32_l0), v_విడత(dg_f32_l1)), v_pack_u(v_విడత(dg_f32_h0), v_విడత(dg_f32_h1))),
                               v_pack(v_pack_u(v_విడత(dr_f32_l0), v_విడత(dr_f32_l1)), v_pack_u(v_విడత(dr_f32_h0), v_విడత(dr_f32_h1))));
        }
        కొరకు (; x < len * 3; x += 3) {
            పూర్ణాంకం b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    diagtransform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
diagtransform_8s(స్థిరరాశి schar* src, schar* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
    #ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        కొరకు (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
            v_int8 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack(v_విడత(db_f32_l0), v_విడత(db_f32_l1)), v_pack(v_విడత(db_f32_h0), v_విడత(db_f32_h1))),
                               v_pack(v_pack(v_విడత(dg_f32_l0), v_విడత(dg_f32_l1)), v_pack(v_విడత(dg_f32_h0), v_విడత(dg_f32_h1))),
                               v_pack(v_pack(v_విడత(dr_f32_l0), v_విడత(dr_f32_l1)), v_pack(v_విడత(dr_f32_h0), v_విడత(dr_f32_h1))));
        }
        కొరకు (; x < len * 3; x += 3) {
            పూర్ణాంకం b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    diagtransform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
diagtransform_16u(స్థిరరాశి ushort* src, ushort* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        కొరకు (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
            v_uint16 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
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
                               v_pack_u(v_విడత(db_f32_l), v_విడత(db_f32_h)),
                               v_pack_u(v_విడత(dg_f32_l), v_విడత(dg_f32_h)),
                               v_pack_u(v_విడత(dr_f32_l), v_విడత(dr_f32_h)));
        }
        కొరకు (; x < len * 3; x += 3) {
            పూర్ణాంకం b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    diagtransform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
diagtransform_16s(స్థిరరాశి పొట్టి* src, పొట్టి* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        కొరకు (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
            v_int16 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
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
                               v_pack(v_విడత(db_f32_l), v_విడత(db_f32_h)),
                               v_pack(v_విడత(dg_f32_l), v_విడత(dg_f32_h)),
                               v_pack(v_విడత(dr_f32_l), v_విడత(dr_f32_h)));
        }
        కొరకు (; x < len * 3; x += 3) {
            పూర్ణాంకం b = src[x], g = src[x + 1], r = src[x + 2];
            పొట్టి db = saturate_cast<పొట్టి>(m[ 0] * b + m[ 3]);
            పొట్టి dg = saturate_cast<పొట్టి>(m[ 5] * g + m[ 7]);
            పొట్టి dr = saturate_cast<పొట్టి>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    diagtransform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
diagtransform_32s(స్థిరరాశి పూర్ణాంకం* src, పూర్ణాంకం* dst, స్థిరరాశి ద్విగుణ* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ఒకవేళ (scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        కొరకు (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ఎగువ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ఎగువ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ఎగువ(r), rl = v_cvt_f64(r);

            v_int32 db = v_విడత(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_విడత(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_విడత(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        కొరకు (; x < len * 3; x += 3) {
            పూర్ణాంకం b = src[x], g = src[x + 1], r = src[x + 2];
            పూర్ణాంకం db = saturate_cast<పూర్ణాంకం>(m[ 0] * b + m[ 3]);
            పూర్ణాంకం dg = saturate_cast<పూర్ణాంకం>(m[ 5] * g + m[ 7]);
            పూర్ణాంకం dr = saturate_cast<పూర్ణాంకం>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    diagtransform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
diagtransform_32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, స్థిరరాశి దశాంశం* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        కొరకు (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        కొరకు (; x < len * 3; x += 3) {
            దశాంశం b = src[x], g = src[x + 1], r = src[x + 2];
            దశాంశం db = saturate_cast<దశాంశం>(m[ 0] * b + m[ 3]);
            దశాంశం dg = saturate_cast<దశాంశం>(m[ 5] * g + m[ 7]);
            దశాంశం dr = saturate_cast<దశాంశం>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    diagtransform_(src, dst, m, len, scn, dcn);
}

స్థిర శూన్య
diagtransform_64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, స్థిరరాశి ద్విగుణ* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ఒకవేళ (scn == 3 && dcn == 3) {
        పూర్ణాంకం x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        కొరకు (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_ఎక్కించు_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        కొరకు (; x < len * 3; x += 3) {
            ద్విగుణ b = src[x], g = src[x + 1], r = src[x + 2];
            ద్విగుణ db = saturate_cast<ద్విగుణ>(m[ 0] * b + m[ 3]);
            ద్విగుణ dg = saturate_cast<ద్విగుణ>(m[ 5] * g + m[ 7]);
            ద్విగుణ dr = saturate_cast<ద్విగుణ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        తిరిగి;
    }
#అయితే_ముగింపు

    diagtransform_(src, dst, m, len, scn, dcn);
}


మూస<రకం_పేరు T, రకం_పేరు WT, శూన్య (*fn)(స్థిరరాశి T*, T*, స్థిరరాశి WT*, పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం)>
స్థిర శూన్య transformWrap(స్థిరరాశి శూన్య* src, శూన్య* dst, స్థిరరాశి శూన్య* m, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn)
{
    fn((స్థిరరాశి T*)src, (T*)dst, (స్థిరరాశి WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(పూర్ణాంకం లోతు)
{
    స్థిర TransformFunc transformTab[CV_లోతు_MAX] =
    {
        transformWrap<uchar, దశాంశం, transform_8u>, transformWrap<schar, దశాంశం, transform_8s>, transformWrap<ushort, దశాంశం, transform_16u>,
        transformWrap<పొట్టి, దశాంశం, transform_16s>, transformWrap<పూర్ణాంకం, ద్విగుణ, transform_32s>, transformWrap<దశాంశం, దశాంశం, transform_32f>,
        transformWrap<ద్విగుణ, ద్విగుణ, transform_64f>, 0
    };

    తిరిగి transformTab[లోతు];
}

TransformFunc getDiagTransformFunc(పూర్ణాంకం లోతు)
{
    స్థిర TransformFunc diagTransformTab[CV_లోతు_MAX] =
    {
        transformWrap<uchar, దశాంశం, diagtransform_8u>, transformWrap<schar, దశాంశం, diagtransform_8s>, transformWrap<ushort, దశాంశం, diagtransform_16u>,
        transformWrap<పొట్టి, దశాంశం, diagtransform_16s>, transformWrap<పూర్ణాంకం, ద్విగుణ, diagtransform_32s>, transformWrap<దశాంశం, దశాంశం, diagtransform_32f>,
        transformWrap<ద్విగుణ, ద్విగుణ, diagtransform_64f>, 0
    };

    తిరిగి diagTransformTab[లోతు];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

మూస<రకం_పేరు T> స్థిర శూన్య
perspectiveTransform_( స్థిరరాశి శూన్య* _src, శూన్య* _dst, స్థిరరాశి శూన్య* _m_ptr, పూర్ణాంకం len, పూర్ణాంకం scn, పూర్ణాంకం dcn )
{
    స్థిరరాశి T* src = (స్థిరరాశి T*)_src;
    T* dst = (T*)_dst;
    స్థిరరాశి ద్విగుణ* m = (స్థిరరాశి ద్విగుణ*)_m_ptr;
    స్థిరరాశి ద్విగుణ eps = FLT_EPSILON;
    పూర్ణాంకం i;

    ఒకవేళ( scn == 2 && dcn == 2 )
    {
        కొరకు( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            ద్విగుణ w = x*m[6] + y*m[7] + m[8];

            ఒకవేళ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            లేకపోతే
                dst[i] = dst[i+1] = (T)0;
        }
    }
    లేకపోతే ఒకవేళ( scn == 3 && dcn == 3 )
    {
        కొరకు( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            ద్విగుణ w = x*m[12] + y*m[13] + z*m[14] + m[15];

            ఒకవేళ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            లేకపోతే
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    లేకపోతే ఒకవేళ( scn == 3 && dcn == 2 )
    {
        కొరకు( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            ద్విగుణ w = x*m[8] + y*m[9] + z*m[10] + m[11];

            ఒకవేళ( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            లేకపోతే
                dst[0] = dst[1] = (T)0;
        }
    }
    లేకపోతే
    {
        కొరకు( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            స్థిరరాశి ద్విగుణ* _m = m + dcn*(scn + 1);
            ద్విగుణ w = _m[scn];
            పూర్ణాంకం j, k;
            కొరకు( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            ఒకవేళ( fabs(w) > eps )
            {
                _m = m;
                కొరకు( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    ద్విగుణ s = _m[scn];
                    కొరకు( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            లేకపోతే
                కొరకు( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(పూర్ణాంకం లోతు)
{
    ఒకవేళ (లోతు == CV_32F)
        తిరిగి perspectiveTransform_<దశాంశం>;
    ఒకవేళ (లోతు == CV_64F)
        తిరిగి perspectiveTransform_<ద్విగుణ>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

స్థిర శూన్య scale_కూడు_32f(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, శూన్య* _dst,
                         పూర్ణాంకం len, స్థిరరాశి శూన్య* _alpha)
{
    స్థిరరాశి దశాంశం* src1 = (స్థిరరాశి దశాంశం*)_src1;
    స్థిరరాశి దశాంశం* src2 = (స్థిరరాశి దశాంశం*)_src2;
    దశాంశం* dst = (దశాంశం*)_dst;
    దశాంశం alpha = *(స్థిరరాశి దశాంశం*)_alpha;
    పూర్ణాంకం i = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    స్థిరరాశి పూర్ణాంకం c_వెడల్పు = VTraits<v_float32>::vlanes();
    కొరకు (; i <= len - c_వెడల్పు; i += c_వెడల్పు)
        v_store(dst + i, v_muladd(vx_ఎక్కించు(src1 + i), v_alpha, vx_ఎక్కించు(src2 + i)));
    vx_cleanup();
#అయితే_ముగింపు
    కొరకు (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


స్థిర శూన్య scale_కూడు_64f(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, శూన్య* _dst,
                         పూర్ణాంకం len, స్థిరరాశి శూన్య* _alpha)
{
    స్థిరరాశి ద్విగుణ* src1 = (స్థిరరాశి ద్విగుణ*)_src1;
    స్థిరరాశి ద్విగుణ* src2 = (స్థిరరాశి ద్విగుణ*)_src2;
    ద్విగుణ* dst = (ద్విగుణ*)_dst;
    ద్విగుణ alpha = *(స్థిరరాశి ద్విగుణ*)_alpha;
    పూర్ణాంకం i = 0;
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    స్థిరరాశి పూర్ణాంకం c_వెడల్పు = VTraits<v_float64>::vlanes();
    కొరకు (; i <= len - c_వెడల్పు; i += c_వెడల్పు)
        v_store(dst + i, v_muladd(vx_ఎక్కించు(src1 + i), a2, vx_ఎక్కించు(src2 + i)));
    vx_cleanup();
#అయితే_ముగింపు
    కొరకు (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

Scale_కూడు_Func get_Scale_కూడు_Func(పూర్ణాంకం లోతు)
{
    ఒకవేళ (లోతు == CV_32F)
        తిరిగి scale_కూడు_32f;
    ఒకవేళ (లోతు == CV_64F)
        తిరిగి scale_కూడు_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

మూస<రకం_పేరు T> స్థిర అంతఃవరుస
ద్విగుణ MahalanobisImpl(స్థిరరాశి Mat& v1, స్థిరరాశి Mat& v2, స్థిరరాశి Mat& icovar, ద్విగుణ *diff_బఫరు /*[len]*/, పూర్ణాంకం len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    కొలత sz = v1.size();
    ద్విగుణ ఫలితం = 0;

    sz.వెడల్పు *= v1.channels();
    ఒకవేళ (v1.isContinuous() && v2.isContinuous())
    {
        sz.వెడల్పు *= sz.ఎత్తు;
        sz.ఎత్తు = 1;
    }

    {
        స్థిరరాశి T* src1 = v1.ptr<T>();
        స్థిరరాశి T* src2 = v2.ptr<T>();
        size_t step1 = v1.అడుగు/పరిమాణం(src1[0]);
        size_t step2 = v2.అడుగు/పరిమాణం(src2[0]);
        ద్విగుణ* diff = diff_బఫరు;
        స్థిరరాశి T* mat = icovar.ptr<T>();
        size_t matstep = icovar.అడుగు/పరిమాణం(mat[0]);

        కొరకు (; sz.ఎత్తు--; src1 += step1, src2 += step2, diff += sz.వెడల్పు)
        {
            కొరకు (పూర్ణాంకం i = 0; i < sz.వెడల్పు; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_బఫరు;
        కొరకు (పూర్ణాంకం i = 0; i < len; i++, mat += matstep)
        {
            ద్విగుణ అడ్డువరుస_కూడిక = 0;
            పూర్ణాంకం j = 0;
#ఒకవేళ CV_ENABLE_UNROLLED
            కొరకు(; j <= len - 4; j += 4 )
                అడ్డువరుస_కూడిక += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#అయితే_ముగింపు
            కొరకు (; j < len; j++)
                అడ్డువరుస_కూడిక += diff[j]*mat[j];
            ఫలితం += అడ్డువరుస_కూడిక * diff[i];
        }
    }
    తిరిగి ఫలితం;
}

MahalanobisImplFunc getMahalanobisImplFunc(పూర్ణాంకం లోతు)
{
    ఒకవేళ (లోతు == CV_32F)
        తిరిగి (MahalanobisImplFunc)MahalanobisImpl<దశాంశం>;
    ఒకవేళ (లోతు == CV_64F)
        తిరిగి (MahalanobisImplFunc)MahalanobisImpl<ద్విగుణ>;
    CV_Assert(0 && "Not supported");
}


#ఒకవేళ !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_బహుళకం)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

మూస<రకం_పేరు sT, రకం_పేరు dT> స్థిర శూన్య
MulTransposedR(స్థిరరాశి Mat& srcmat, స్థిరరాశి Mat& dstmat, స్థిరరాశి Mat& deltamat, ద్విగుణ scale)
{
    పూర్ణాంకం i, j, k;
    స్థిరరాశి sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    స్థిరరాశి dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.అడుగు/పరిమాణం(src[0]);
    size_t dststep = dstmat.అడుగు/పరిమాణం(dst[0]);
    size_t deltastep = deltamat.అడ్డువరుసలు > 1 ? deltamat.అడుగు/పరిమాణం(delta[0]) : 0;
    పూర్ణాంకం delta_నిలువుల_సంఖ్య = deltamat.నిలువుల_సంఖ్య;
    కొలత size = srcmat.size();
    dT* tdst = dst;
    dT* నిలువు_buf = 0;
    dT* delta_buf = 0;
    పూర్ణాంకం buf_కొలత_2 = size.ఎత్తు*పరిమాణం(dT);
    Auto_బఫరు<uchar> buf;

    ఒకవేళ( delta && delta_నిలువుల_సంఖ్య < size.వెడల్పు )
    {
        CV_Assert( delta_నిలువుల_సంఖ్య == 1 );
        buf_కొలత_2 *= 5;
    }
    buf.allocate(buf_కొలత_2);
    నిలువు_buf = (dT*)buf.దత్తాంశం();

    ఒకవేళ( delta && delta_నిలువుల_సంఖ్య < size.వెడల్పు )
    {
        delta_buf = నిలువు_buf + size.ఎత్తు;
        కొరకు( i = 0; i < size.ఎత్తు; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#ఒకవేళ CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#అయితే_ముగింపు

    ఒకవేళ( !delta )
        కొరకు( i = 0; i < size.వెడల్పు; i++, tdst += dststep )
        {
            కొరకు( k = 0; k < size.ఎత్తు; k++ )
                నిలువు_buf[k] = src[k*srcstep+i];

            కొరకు( j = i; j <= size.వెడల్పు - 4; j += 4 )
            {
#ఒకవేళ CV_SIMD128_64F
                ఒకవేళ (దత్తాంశం_Type_2<sT>::లోతు == CV_64F && దత్తాంశం_Type_2<dT>::లోతు == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    స్థిరరాశి ద్విగుణ *tsrc = (ద్విగుణ*)(src + j);

                    కొరకు( k = 0; k < size.ఎత్తు; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((ద్విగుణ)నిలువు_buf[k]);
                        s0 = v_కూడు(s0, v_mul(a, v_ఎక్కించు(tsrc + 0)));
                        s1 = v_కూడు(s1, v_mul(a, v_ఎక్కించు(tsrc + 2)));
                    }

                    v_store((ద్విగుణ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ద్విగుణ*)(tdst+j+2), v_mul(s1, v_scale));
                } లేకపోతే
#అయితే_ముగింపు
                {
                    ద్విగుణ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    స్థిరరాశి sT *tsrc = src + j;

                    కొరకు( k = 0; k < size.ఎత్తు; k++, tsrc += srcstep )
                    {
                        ద్విగుణ a = నిలువు_buf[k];
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

            కొరకు( ; j < size.వెడల్పు; j++ )
            {
                ద్విగుణ s0 = 0;
                స్థిరరాశి sT *tsrc = src + j;

                కొరకు( k = 0; k < size.ఎత్తు; k++, tsrc += srcstep )
                    s0 += (ద్విగుణ)నిలువు_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    లేకపోతే
        కొరకు( i = 0; i < size.వెడల్పు; i++, tdst += dststep )
        {
            ఒకవేళ( !delta_buf )
                కొరకు( k = 0; k < size.ఎత్తు; k++ )
                    నిలువు_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            లేకపోతే
                కొరకు( k = 0; k < size.ఎత్తు; k++ )
                    నిలువు_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            కొరకు( j = i; j <= size.వెడల్పు - 4; j += 4 )
            {
#ఒకవేళ CV_SIMD128_64F
                ఒకవేళ (దత్తాంశం_Type_2<sT>::లోతు == CV_64F && దత్తాంశం_Type_2<dT>::లోతు == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    స్థిరరాశి ద్విగుణ *tsrc = (ద్విగుణ*)(src + j);
                    స్థిరరాశి ద్విగుణ *d = (ద్విగుణ*)(delta_buf ? delta_buf : delta + j);

                    కొరకు( k = 0; k < size.ఎత్తు; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((ద్విగుణ)నిలువు_buf[k]);
                        s0 = v_కూడు(s0, v_mul(a, v_sub(v_ఎక్కించు(tsrc + 0), v_ఎక్కించు(d + 0))));
                        s1 = v_కూడు(s1, v_mul(a, v_sub(v_ఎక్కించు(tsrc + 2), v_ఎక్కించు(d + 2))));
                    }

                    v_store((ద్విగుణ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ద్విగుణ*)(tdst+j+2), v_mul(s1, v_scale));
                }
                లేకపోతే
#అయితే_ముగింపు

                {
                    ద్విగుణ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    స్థిరరాశి sT *tsrc = src + j;
                    స్థిరరాశి dT *d = delta_buf ? delta_buf : delta + j;

                    కొరకు( k = 0; k < size.ఎత్తు; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        ద్విగుణ a = నిలువు_buf[k];
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

            కొరకు( ; j < size.వెడల్పు; j++ )
            {
                ద్విగుణ s0 = 0;
                స్థిరరాశి sT *tsrc = src + j;
                స్థిరరాశి dT *d = delta_buf ? delta_buf : delta + j;

                కొరకు( k = 0; k < size.ఎత్తు; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (ద్విగుణ)నిలువు_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


మూస<రకం_పేరు sT, రకం_పేరు dT> స్థిర శూన్య
MulTransposedL(స్థిరరాశి Mat& srcmat, స్థిరరాశి Mat& dstmat, స్థిరరాశి Mat& deltamat, ద్విగుణ scale)
{
    పూర్ణాంకం i, j, k;
    స్థిరరాశి sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    స్థిరరాశి dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.అడుగు/పరిమాణం(src[0]);
    size_t dststep = dstmat.అడుగు/పరిమాణం(dst[0]);
    size_t deltastep = deltamat.అడ్డువరుసలు > 1 ? deltamat.అడుగు/పరిమాణం(delta[0]) : 0;
    పూర్ణాంకం delta_నిలువుల_సంఖ్య = deltamat.నిలువుల_సంఖ్య;
    కొలత size = srcmat.size();
    dT* tdst = dst;

    ఒకవేళ( !delta )
        కొరకు( i = 0; i < size.ఎత్తు; i++, tdst += dststep )
            కొరకు( j = i; j < size.ఎత్తు; j++ )
            {
                ద్విగుణ s = 0;
                స్థిరరాశి sT *tsrc1 = src + i*srcstep;
                స్థిరరాశి sT *tsrc2 = src + j*srcstep;
#ఒకవేళ CV_SIMD128_64F
                ఒకవేళ (దత్తాంశం_Type_2<sT>::లోతు == CV_64F && దత్తాంశం_Type_2<dT>::లోతు == CV_64F)
                {
                    స్థిరరాశి ద్విగుణ *v_tsrc1 = (ద్విగుణ *)(tsrc1);
                    స్థిరరాశి ద్విగుణ *v_tsrc2 = (ద్విగుణ *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    కొరకు( k = 0; k <= size.వెడల్పు - 4; k += 4 )
                        v_s = v_కూడు(v_s, v_కూడు(v_mul(v_ఎక్కించు(v_tsrc1 + k), v_ఎక్కించు(v_tsrc2 + k)), v_mul(v_ఎక్కించు(v_tsrc1 + k + 2), v_ఎక్కించు(v_tsrc2 + k + 2))));
                    s += v_reduce_కూడిక(v_s);
                }
                లేకపోతే
#అయితే_ముగింపు
                {
                    కొరకు( k = 0; k <= size.వెడల్పు - 4; k += 4 )
                        s += (ద్విగుణ)tsrc1[k]*tsrc2[k] + (ద్విగుణ)tsrc1[k+1]*tsrc2[k+1] +
                             (ద్విగుణ)tsrc1[k+2]*tsrc2[k+2] + (ద్విగుణ)tsrc1[k+3]*tsrc2[k+3];
                }
                కొరకు( ; k < size.వెడల్పు; k++ )
                    s += (ద్విగుణ)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    లేకపోతే
    {
        dT delta_buf[4];
        పూర్ణాంకం delta_జరుపు = delta_నిలువుల_సంఖ్య == size.వెడల్పు ? 4 : 0;
        Auto_బఫరు<uchar> buf(size.వెడల్పు*పరిమాణం(dT));
        dT* అడ్డువరుస_buf = (dT*)buf.దత్తాంశం();

        కొరకు( i = 0; i < size.ఎత్తు; i++, tdst += dststep )
        {
            స్థిరరాశి sT *tsrc1 = src + i*srcstep;
            స్థిరరాశి dT *tdelta1 = delta + i*deltastep;

            ఒకవేళ( delta_నిలువుల_సంఖ్య < size.వెడల్పు )
                కొరకు( k = 0; k < size.వెడల్పు; k++ )
                    అడ్డువరుస_buf[k] = tsrc1[k] - tdelta1[0];
            లేకపోతే
                కొరకు( k = 0; k < size.వెడల్పు; k++ )
                    అడ్డువరుస_buf[k] = tsrc1[k] - tdelta1[k];

            కొరకు( j = i; j < size.ఎత్తు; j++ )
            {
                ద్విగుణ s = 0;
                స్థిరరాశి sT *tsrc2 = src + j*srcstep;
                స్థిరరాశి dT *tdelta2 = delta + j*deltastep;
                ఒకవేళ( delta_నిలువుల_సంఖ్య < size.వెడల్పు )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#ఒకవేళ CV_SIMD128_64F
                ఒకవేళ (దత్తాంశం_Type_2<sT>::లోతు == CV_64F && దత్తాంశం_Type_2<dT>::లోతు == CV_64F)
                {
                    స్థిరరాశి ద్విగుణ *v_tsrc2 = (ద్విగుణ *)(tsrc2);
                    స్థిరరాశి ద్విగుణ *v_tdelta2 = (ద్విగుణ *)(tdelta2);
                    స్థిరరాశి ద్విగుణ *v_అడ్డువరుస_buf = (ద్విగుణ *)(అడ్డువరుస_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    కొరకు( k = 0; k <= size.వెడల్పు - 4; k += 4, v_tdelta2 += delta_జరుపు )
                        v_s = v_కూడు(v_s, v_కూడు(v_mul(v_sub(v_ఎక్కించు(v_tsrc2 + k), v_ఎక్కించు(v_tdelta2)), v_ఎక్కించు(v_అడ్డువరుస_buf + k)), v_mul(v_sub(v_ఎక్కించు(v_tsrc2 + k + 2), v_ఎక్కించు(v_tdelta2 + 2)), v_ఎక్కించు(v_అడ్డువరుస_buf + k + 2))));
                    s += v_reduce_కూడిక(v_s);

                    tdelta2 = (స్థిరరాశి dT *)(v_tdelta2);
                }
                లేకపోతే
#అయితే_ముగింపు
                {
                    కొరకు( k = 0; k <= size.వెడల్పు-4; k += 4, tdelta2 += delta_జరుపు )
                        s += (ద్విగుణ)అడ్డువరుస_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (ద్విగుణ)అడ్డువరుస_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (ద్విగుణ)అడ్డువరుస_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (ద్విగుణ)అడ్డువరుస_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                కొరకు( ; k < size.వెడల్పు; k++, tdelta2++ )
                    s += (ద్విగుణ)అడ్డువరుస_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(పూర్ణాంకం stype, పూర్ణాంకం dtype, తార్కిక ata)
{
    MulTransposedFunc func = ఖాళీ;
    ఒకవేళ (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,దశాంశం>
                   : MulTransposedL<uchar,దశాంశం>;
    }
    లేకపోతే ఒకవేళ (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,ద్విగుణ>
                   : MulTransposedL<uchar,ద్విగుణ>;
    }
    లేకపోతే ఒకవేళ(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,దశాంశం>
                   : MulTransposedL<ushort,దశాంశం>;
    }
    లేకపోతే ఒకవేళ(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,ద్విగుణ>
                   : MulTransposedL<ushort,ద్విగుణ>;
    }
    లేకపోతే ఒకవేళ(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<పొట్టి,దశాంశం>
                   : MulTransposedL<పొట్టి,దశాంశం>;
    }
    లేకపోతే ఒకవేళ(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<పొట్టి,ద్విగుణ>
                   : MulTransposedL<పొట్టి,ద్విగుణ>;
    }
    లేకపోతే ఒకవేళ(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<దశాంశం,దశాంశం>
                   : MulTransposedL<దశాంశం,దశాంశం>;
    }
    లేకపోతే ఒకవేళ(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<దశాంశం,ద్విగుణ>
                   : MulTransposedL<దశాంశం,ద్విగుణ>;
    }
    లేకపోతే ఒకవేళ(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ద్విగుణ,ద్విగుణ>
                   : MulTransposedL<ద్విగుణ,ద్విగుణ>;
    }
    CV_Assert(func && "Not supported");
    తిరిగి func;
}
#అయితే_ముగింపు // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

మూస<రకం_పేరు T> స్థిర అంతఃవరుస
ద్విగుణ dotProd_(స్థిరరాశి T* src1, స్థిరరాశి T* src2, పూర్ణాంకం len)
{
    పూర్ణాంకం i = 0;
    ద్విగుణ ఫలితం = 0;

#ఒకవేళ CV_ENABLE_UNROLLED
    కొరకు( ; i <= len - 4; i += 4 )
        ఫలితం += (ద్విగుణ)src1[i]*src2[i] + (ద్విగుణ)src1[i+1]*src2[i+1] +
            (ద్విగుణ)src1[i+2]*src2[i+2] + (ద్విగుణ)src1[i+3]*src2[i+3];
#అయితే_ముగింపు
    కొరకు( ; i < len; i++ )
        ఫలితం += (ద్విగుణ)src1[i]*src2[i];

    తిరిగి ఫలితం;
}


ద్విగుణ dotProd_8u(స్థిరరాశి uchar* src1, స్థిరరాశి uchar* src2, పూర్ణాంకం len)
{
    ద్విగుణ r = 0;
    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    పూర్ణాంకం len0 = len & -VTraits<v_uint16>::vlanes(), ఖండం_Size0 = (1 << 15), ఖండం_కొలత_2;

    అయితే (i < len0)
    {
        ఖండం_కొలత_2 = std::min(len0 - i, ఖండం_Size0);
        v_uint32 v_కూడిక = vx_setzero_u32();
        స్థిరరాశి పూర్ణాంకం c_వెడల్పు = VTraits<v_uint16>::vlanes();

        పూర్ణాంకం j = 0;
        కొరకు (; j <= ఖండం_కొలత_2 - c_వెడల్పు * 2; j += c_వెడల్పు * 2)
        {
            v_uint8 v_src1 = vx_ఎక్కించు(src1 + j);
            v_uint8 v_src2 = vx_ఎక్కించు(src2 + j);
            v_కూడిక = v_dotprod_expand_fast(v_src1, v_src2, v_కూడిక);
        }

        కొరకు (; j <= ఖండం_కొలత_2 - c_వెడల్పు; j += c_వెడల్పు)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_ఎక్కించు_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_ఎక్కించు_expand(src2 + j));
            v_కూడిక = v_కూడు(v_కూడిక, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (ద్విగుణ)v_reduce_కూడిక(v_కూడిక);

        src1 += ఖండం_కొలత_2;
        src2 += ఖండం_కొలత_2;
        i += ఖండం_కొలత_2;
    }
    vx_cleanup();
#అయితే_ముగింపు
    తిరిగి r + dotProd_(src1, src2, len - i);
}


ద్విగుణ dotProd_8s(స్థిరరాశి schar* src1, స్థిరరాశి schar* src2, పూర్ణాంకం len)
{
    ద్విగుణ r = 0.0;
    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    పూర్ణాంకం len0 = len & -VTraits<v_int16>::vlanes(), ఖండం_Size0 = (1 << 14), ఖండం_కొలత_2;

    అయితే (i < len0)
    {
        ఖండం_కొలత_2 = std::min(len0 - i, ఖండం_Size0);
        v_int32 v_కూడిక = vx_setzero_s32();
        స్థిరరాశి పూర్ణాంకం c_వెడల్పు = VTraits<v_int16>::vlanes();

        పూర్ణాంకం j = 0;
        కొరకు (; j <= ఖండం_కొలత_2 - c_వెడల్పు * 2; j += c_వెడల్పు * 2)
        {
            v_int8 v_src1 = vx_ఎక్కించు(src1 + j);
            v_int8 v_src2 = vx_ఎక్కించు(src2 + j);
            v_కూడిక = v_dotprod_expand_fast(v_src1, v_src2, v_కూడిక);
        }

        కొరకు (; j <= ఖండం_కొలత_2 - c_వెడల్పు; j += c_వెడల్పు)
        {
            v_int16 v_src1 = vx_ఎక్కించు_expand(src1 + j);
            v_int16 v_src2 = vx_ఎక్కించు_expand(src2 + j);
            v_కూడిక = v_dotprod_fast(v_src1, v_src2, v_కూడిక);
        }
        r += (ద్విగుణ)v_reduce_కూడిక(v_కూడిక);

        src1 += ఖండం_కొలత_2;
        src2 += ఖండం_కొలత_2;
        i += ఖండం_కొలత_2;
    }
    vx_cleanup();
#అయితే_ముగింపు

    తిరిగి r + dotProd_(src1, src2, len - i);
}

ద్విగుణ dotProd_16u(స్థిరరాశి ushort* src1, స్థిరరాశి ushort* src2, పూర్ణాంకం len)
{
    ద్విగుణ r = 0.0;
    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    పూర్ణాంకం len0 = len & -VTraits<v_uint16>::vlanes(), ఖండం_Size0 = (1 << 24), ఖండం_కొలత_2;

    అయితే (i < len0)
    {
        ఖండం_కొలత_2 = std::min(len0 - i, ఖండం_Size0);
        v_uint64 v_కూడిక = vx_setzero_u64();
        స్థిరరాశి పూర్ణాంకం c_వెడల్పు = VTraits<v_uint16>::vlanes();

        పూర్ణాంకం j = 0;
        కొరకు (; j <= ఖండం_కొలత_2 - c_వెడల్పు; j += c_వెడల్పు)
        {
            v_uint16 v_src1 = vx_ఎక్కించు(src1 + j);
            v_uint16 v_src2 = vx_ఎక్కించు(src2 + j);
            v_కూడిక = v_dotprod_expand_fast(v_src1, v_src2, v_కూడిక);
        }
        r += (ద్విగుణ)v_reduce_కూడిక(v_కూడిక);

        src1 += ఖండం_కొలత_2;
        src2 += ఖండం_కొలత_2;
        i += ఖండం_కొలత_2;
    }
    vx_cleanup();
#అయితే_ముగింపు
    తిరిగి r + dotProd_(src1, src2, len - i);
}

ద్విగుణ dotProd_16s(స్థిరరాశి పొట్టి* src1, స్థిరరాశి పొట్టి* src2, పూర్ణాంకం len)
{
    ద్విగుణ r = 0.0;
    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    పూర్ణాంకం len0 = len & -VTraits<v_int16>::vlanes(), ఖండం_Size0 = (1 << 24), ఖండం_కొలత_2;

    అయితే (i < len0)
    {
        ఖండం_కొలత_2 = std::min(len0 - i, ఖండం_Size0);
        v_int64 v_కూడిక = vx_setzero_s64();
        స్థిరరాశి పూర్ణాంకం c_వెడల్పు = VTraits<v_int16>::vlanes();

        పూర్ణాంకం j = 0;
        కొరకు (; j <= ఖండం_కొలత_2 - c_వెడల్పు; j += c_వెడల్పు)
        {
            v_int16 v_src1 = vx_ఎక్కించు(src1 + j);
            v_int16 v_src2 = vx_ఎక్కించు(src2 + j);
            v_కూడిక = v_dotprod_expand_fast(v_src1, v_src2, v_కూడిక);
        }
        r += (ద్విగుణ)v_reduce_కూడిక(v_కూడిక);

        src1 += ఖండం_కొలత_2;
        src2 += ఖండం_కొలత_2;
        i += ఖండం_కొలత_2;
    }
    vx_cleanup();
#అయితే_ముగింపు
    తిరిగి r + dotProd_(src1, src2, len - i);
}

ద్విగుణ dotProd_32s(స్థిరరాశి పూర్ణాంకం* src1, స్థిరరాశి పూర్ణాంకం* src2, పూర్ణాంకం len)
{
#ఒకవేళ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ద్విగుణ r = .0;
    పూర్ణాంకం i = 0;
    స్థిరరాశి పూర్ణాంకం అడుగు  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#ఒకవేళ CV_SIMD_వెడల్పు == 16
    స్థిరరాశి పూర్ణాంకం wstep = అడుగు * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    కొరకు (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
    {
        v_int32 v_src10 = vx_ఎక్కించు(src1);
        v_int32 v_src20 = vx_ఎక్కించు(src2);
        v_int32 v_src11 = vx_ఎక్కించు(src1 + అడుగు);
        v_int32 v_src21 = vx_ఎక్కించు(src2 + అడుగు);
        v_sum0 = v_dotprod_expand_fast(v_src10, v_src20, v_sum0);
        v_sum1 = v_dotprod_expand_fast(v_src11, v_src21, v_sum1);
    }
    v_sum0 = v_కూడు(v_sum0, v_sum1);
#అయితే_ముగింపు
    కొరకు (; i <= len - అడుగు; i += అడుగు, src1 += అడుగు, src2 += అడుగు)
    {
        v_int32 v_src1 = vx_ఎక్కించు(src1);
        v_int32 v_src2 = vx_ఎక్కించు(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_కూడిక(v_sum0);
    vx_cleanup();
    తిరిగి r + dotProd_(src1, src2, len - i);
#లేకపోతే
    తిరిగి dotProd_(src1, src2, len);
#అయితే_ముగింపు
}

ద్విగుణ dotProd_32f(స్థిరరాశి దశాంశం* src1, స్థిరరాశి దశాంశం* src2, పూర్ణాంకం len)
{
    ద్విగుణ r = 0.0;
    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    పూర్ణాంకం len0 = len & -VTraits<v_float32>::vlanes(), ఖండం_Size0 = (1 << 13), ఖండం_కొలత_2;

    అయితే (i < len0)
    {
        ఖండం_కొలత_2 = std::min(len0 - i, ఖండం_Size0);
        v_float32 v_కూడిక = vx_setzero_f32();

        పూర్ణాంకం j = 0;
        పూర్ణాంకం c_వెడల్పు = VTraits<v_float32>::vlanes();

#ఒకవేళ CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        కొరకు (; j <= ఖండం_కొలత_2 - (c_వెడల్పు * 4); j += (c_వెడల్పు * 4))
        {
            v_కూడిక  = v_muladd(vx_ఎక్కించు(src1 + j),
                              vx_ఎక్కించు(src2 + j), v_కూడిక);
            v_sum1 = v_muladd(vx_ఎక్కించు(src1 + j + c_వెడల్పు),
                              vx_ఎక్కించు(src2 + j + c_వెడల్పు), v_sum1);
            v_sum2 = v_muladd(vx_ఎక్కించు(src1 + j + (c_వెడల్పు * 2)),
                              vx_ఎక్కించు(src2 + j + (c_వెడల్పు * 2)), v_sum2);
            v_sum3 = v_muladd(vx_ఎక్కించు(src1 + j + (c_వెడల్పు * 3)),
                              vx_ఎక్కించు(src2 + j + (c_వెడల్పు * 3)), v_sum3);
        }

        v_కూడిక = v_కూడు(v_కూడిక, v_కూడు(v_కూడు(v_sum1, v_sum2), v_sum3));
#అయితే_ముగింపు

        కొరకు (; j <= ఖండం_కొలత_2 - c_వెడల్పు; j += c_వెడల్పు)
            v_కూడిక = v_muladd(vx_ఎక్కించు(src1 + j), vx_ఎక్కించు(src2 + j), v_కూడిక);

        r += v_reduce_కూడిక(v_కూడిక);

        src1 += ఖండం_కొలత_2;
        src2 += ఖండం_కొలత_2;
        i += ఖండం_కొలత_2;
    }
    vx_cleanup();
#అయితే_ముగింపు
    తిరిగి r + dotProd_(src1, src2, len - i);
}

ద్విగుణ dotProd_64f(స్థిరరాశి ద్విగుణ* src1, స్థిరరాశి ద్విగుణ* src2, పూర్ణాంకం len)
{
    తిరిగి dotProd_(src1, src2, len);
}

#అయితే_ముగింపు
CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace
