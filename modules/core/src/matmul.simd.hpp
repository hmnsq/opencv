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

#অন্তর্ভাব "precomp.hpp"

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
#যদি defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#অন্যথা
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

নামাকাশ cv {

// forward declarations
প্রকারলেখ শূন্য (*TransformFunc)(ধ্রুবক শূন্য* src, শূন্য* dst, ধ্রুবক শূন্য* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn);
প্রকারলেখ শূন্য (*ScaleAddFunc)(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, শূন্য* dst, পূর্ণসংখ্যা len, ধ্রুবক শূন্য* alpha);
প্রকারলেখ শূন্য (*MulTransposedFunc)(ধ্রুবক Mat& src, ধ্রুবক/*preallocated*/ Mat& dst, ধ্রুবক Mat& delta, দ্বিগুণ scale);
প্রকারলেখ দ্বিগুণ (*MahalanobisImplFunc)(ধ্রুবক Mat& v1, ধ্রুবক Mat& v2, ধ্রুবক Mat& icovar, দ্বিগুণ *diff_বাফার /*[len]*/, পূর্ণসংখ্যা len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

শূন্য gemm32f(ধ্রুবক দশমিক* src1, size_t src1_step, ধ্রুবক দশমিক* src2, size_t src2_step,
             দশমিক alpha, ধ্রুবক দশমিক* src3, size_t src3_step, দশমিক beta, দশমিক* dst, size_t dst_step,
             পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags);
শূন্য gemm64f(ধ্রুবক দ্বিগুণ* src1, size_t src1_step, ধ্রুবক দ্বিগুণ* src2, size_t src2_step,
             দ্বিগুণ alpha, ধ্রুবক দ্বিগুণ* src3, size_t src3_step, দ্বিগুণ beta, দ্বিগুণ* dst, size_t dst_step,
             পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags);
শূন্য gemm32fc(ধ্রুবক দশমিক* src1, size_t src1_step, ধ্রুবক দশমিক* src2, size_t src2_step,
              দশমিক alpha, ধ্রুবক দশমিক* src3, size_t src3_step, দশমিক beta, দশমিক* dst, size_t dst_step,
              পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags);
শূন্য gemm64fc(ধ্রুবক দ্বিগুণ* src1, size_t src1_step, ধ্রুবক দ্বিগুণ* src2, size_t src2_step,
              দ্বিগুণ alpha, ধ্রুবক দ্বিগুণ* src3, size_t src3_step, দ্বিগুণ beta, দ্বিগুণ* dst, size_t dst_step,
              পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags);

TransformFunc getTransformFunc(পূর্ণসংখ্যা depth);
TransformFunc getDiagTransformFunc(পূর্ণসংখ্যা depth);
TransformFunc getPerspectiveTransform(পূর্ণসংখ্যা depth);

ScaleAddFunc getScaleAddFunc(পূর্ণসংখ্যা depth);

MahalanobisImplFunc getMahalanobisImplFunc(পূর্ণসংখ্যা depth);

MulTransposedFunc getMulTransposedFunc(পূর্ণসংখ্যা stype, পূর্ণসংখ্যা dtype, যৌক্তিক ata);

দ্বিগুণ dotProd_8u(ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, পূর্ণসংখ্যা len);
দ্বিগুণ dotProd_8s(ধ্রুবক schar* src1, ধ্রুবক schar* src2, পূর্ণসংখ্যা len);
দ্বিগুণ dotProd_16u(ধ্রুবক ushort* src1, ধ্রুবক ushort* src2, পূর্ণসংখ্যা len);
দ্বিগুণ dotProd_16s(ধ্রুবক short* src1, ধ্রুবক short* src2, পূর্ণসংখ্যা len);
দ্বিগুণ dotProd_32s(ধ্রুবক পূর্ণসংখ্যা* src1, ধ্রুবক পূর্ণসংখ্যা* src2, পূর্ণসংখ্যা len);
দ্বিগুণ dotProd_32f(ধ্রুবক দশমিক* src1, ধ্রুবক দশমিক* src2, পূর্ণসংখ্যা len);
দ্বিগুণ dotProd_64f(ধ্রুবক দ্বিগুণ* src1, ধ্রুবক দ্বিগুণ* src2, পূর্ণসংখ্যা len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#যদি !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

স্থির শূন্য
GEMM_CopyBlock( ধ্রুবক uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                আকার_2 size, size_t pix_আকার )
{
    পূর্ণসংখ্যা j;
    size.width *= (পূর্ণসংখ্যা)(pix_আকার / আকার(পূর্ণসংখ্যা));

    জন্য( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #যদি CV_ENABLE_UNROLLED
        জন্য( ; j <= size.width - 4; j += 4 )
        {
            পূর্ণসংখ্যা t0 = ((ধ্রুবক পূর্ণসংখ্যা*)src)[j];
            পূর্ণসংখ্যা t1 = ((ধ্রুবক পূর্ণসংখ্যা*)src)[j+1];
            ((পূর্ণসংখ্যা*)dst)[j] = t0;
            ((পূর্ণসংখ্যা*)dst)[j+1] = t1;
            t0 = ((ধ্রুবক পূর্ণসংখ্যা*)src)[j+2];
            t1 = ((ধ্রুবক পূর্ণসংখ্যা*)src)[j+3];
            ((পূর্ণসংখ্যা*)dst)[j+2] = t0;
            ((পূর্ণসংখ্যা*)dst)[j+3] = t1;
        }
        #endif
        জন্য( ; j < size.width; j++ )
            ((পূর্ণসংখ্যা*)dst)[j] = ((ধ্রুবক পূর্ণসংখ্যা*)src)[j];
    }
}


স্থির শূন্য
GEMM_TransposeBlock( ধ্রুবক uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     আকার_2 size, size_t pix_আকার )
{
    পূর্ণসংখ্যা i, j;
    জন্য( i = 0; i < size.width; i++, dst += dst_step, src += pix_আকার )
    {
        ধ্রুবক uchar* _src = src;
        নির্বাচন( pix_আকার )
        {
        ক্ষেত্র আকার(পূর্ণসংখ্যা):
            জন্য( j = 0; j < size.height; j++, _src += src_step )
                ((পূর্ণসংখ্যা*)dst)[j] = ((পূর্ণসংখ্যা*)_src)[0];
            বিরতি;
        ক্ষেত্র আকার(পূর্ণসংখ্যা)*2:
            জন্য( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                পূর্ণসংখ্যা t0 = ((পূর্ণসংখ্যা*)_src)[0];
                পূর্ণসংখ্যা t1 = ((পূর্ণসংখ্যা*)_src)[1];
                ((পূর্ণসংখ্যা*)dst)[j] = t0;
                ((পূর্ণসংখ্যা*)dst)[j+1] = t1;
            }
            বিরতি;
        ক্ষেত্র আকার(পূর্ণসংখ্যা)*4:
            জন্য( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                পূর্ণসংখ্যা t0 = ((পূর্ণসংখ্যা*)_src)[0];
                পূর্ণসংখ্যা t1 = ((পূর্ণসংখ্যা*)_src)[1];
                ((পূর্ণসংখ্যা*)dst)[j] = t0;
                ((পূর্ণসংখ্যা*)dst)[j+1] = t1;
                t0 = ((পূর্ণসংখ্যা*)_src)[2];
                t1 = ((পূর্ণসংখ্যা*)_src)[3];
                ((পূর্ণসংখ্যা*)dst)[j+2] = t0;
                ((পূর্ণসংখ্যা*)dst)[j+3] = t1;
            }
            বিরতি;
        পূর্বনির্ধারিত:
            CV_Assert(0);
            ফেরত;
        }
    }
}

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> কাঠামো VecTraits;

template<> কাঠামো VecTraits<দশমিক> {
    প্রকারলেখ v_float32 VT;
    স্থির অন্তঃরেখ VT zero() { ফেরত vx_setzero_f32(); }
    স্থির অন্তঃরেখ VT load(ধ্রুবক দশমিক* p) { ফেরত vx_load(p); }
    স্থির অন্তঃরেখ VT setall(দশমিক v) { ফেরত vx_setall_f32(v); }
};
#endif

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> কাঠামো VecTraits<দ্বিগুণ> {
    প্রকারলেখ v_float64 VT;
    স্থির অন্তঃরেখ VT zero() { ফেরত vx_setzero_f64(); }
    স্থির অন্তঃরেখ VT load(ধ্রুবক দ্বিগুণ* p) { ফেরত vx_load(p); }
    স্থির অন্তঃরেখ VT setall(দ্বিগুণ v) { ফেরত vx_setall_f64(v); }
};
#endif

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
স্থির অন্তঃরেখ ST simdDotProduct(ধ্রুবক ST* a, ধ্রুবক ST* b, পূর্ণসংখ্যা n, পূর্ণসংখ্যা& k)
{
    প্রকারলেখ typename VecTraits<ST>::VT VecT;
    ধ্রুবক পূর্ণসংখ্যা vlanes = VTraits<VecT>::vlanes();
    ধ্রুবক পূর্ণসংখ্যা stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    জন্য( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    জন্য( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    ফেরত v_reduce_যোগ(vs0);
}
#endif

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
স্থির অন্তঃরেখ দ্বিগুণ simdDotProduct_f32f64(ধ্রুবক দশমিক* a, ধ্রুবক দশমিক* b, পূর্ণসংখ্যা n, পূর্ণসংখ্যা& k)
{
    ধ্রুবক পূর্ণসংখ্যা vlanes_f32 = VTraits<v_float32>::vlanes();
    ধ্রুবক পূর্ণসংখ্যা stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    জন্য( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_উপরের(va0), v_cvt_f64_উপরের(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_উপরের(va1), v_cvt_f64_উপরের(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    জন্য( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_উপরের(va), v_cvt_f64_উপরের(vb), vs0);
    }
    ফেরত v_reduce_যোগ(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
স্থির অন্তঃরেখ শূন্য simdGEMM_kj_f32f64(
    ধ্রুবক দশমিক* a_তথ্য, ধ্রুবক দশমিক* b_তথ্য_2, size_t b_step,
    ধ্রুবক দশমিক* c_তথ্য, size_t c_step1,
    দশমিক* d_তথ্য,
    পূর্ণসংখ্যা n, পূর্ণসংখ্যা m,
    দ্বিগুণ alpha, দ্বিগুণ beta,
    পূর্ণসংখ্যা& j)
{
    ধ্রুবক পূর্ণসংখ্যা vlanes_f32 = VTraits<v_float32>::vlanes();
    ধ্রুবক পূর্ণসংখ্যা vlanes_f64 = VTraits<v_float64>::vlanes();

    যদি( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ধ্রুবক পূর্ণসংখ্যা max_m = 1600 / (পূর্ণসংখ্যা)আকার(দশমিক);
        cv::Auto_বাফার<দ্বিগুণ> _s_buf(max_m);
        দ্বিগুণ* s_buf = _s_buf.তথ্য();

        {
            v_float64 vz = vx_setzero_f64();
            জন্য( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            জন্য( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ধ্রুবক দশমিক* b_ptr = b_তথ্য_2;
            জন্য( পূর্ণসংখ্যা k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((দ্বিগুণ)a_তথ্য[k]);
                j = 0;
                জন্য( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_উপরের(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                দ্বিগুণ a_val = (দ্বিগুণ)a_তথ্য[k];
                জন্য( ; j < m; j++ )
                    s_buf[j] += a_val * (দ্বিগুণ)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ধ্রুবক দশমিক* c_তথ্য_2 = c_তথ্য;
            j = 0;
            যদি( !c_তথ্য )
            {
                জন্য( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_তথ্য + j, v_cvt_f32(vs_lo, vs_hi));
                }
                জন্য( ; j < m; j++ )
                    d_তথ্য[j] = (দশমিক)(s_buf[j] * alpha);
            }
            অন্যথা যদি( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                জন্য( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_তথ্য_2 + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_উপরের(vc), vbeta, vs_hi);
                    v_store(d_তথ্য + j, v_cvt_f32(vs_lo, vs_hi));
                }
                জন্য( ; j < m; j++ )
                    d_তথ্য[j] = (দশমিক)(s_buf[j] * alpha + (দ্বিগুণ)c_তথ্য_2[j] * beta);
            }
            অন্যথা
            {
                জন্য( j = 0; j < m; j++, c_তথ্য_2 += c_step1 )
                    d_তথ্য[j] = (দশমিক)(s_buf[j] * alpha + (দ্বিগুণ)c_তথ্য_2[0] * beta);
            }
        }
        j = m;
    }
    অন্যথা যদি( vlanes_f32 * (পূর্ণসংখ্যা)আকার(দশমিক) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ধ্রুবক দশমিক* c_তথ্য_2 = c_তথ্য;
        জন্য( ; j <= m - vlanes_f32; j += vlanes_f32, c_তথ্য_2 += vlanes_f32 * c_step1 )
        {
            ধ্রুবক দশমিক* b = b_তথ্য_2 + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            জন্য( পূর্ণসংখ্যা k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((দ্বিগুণ)a_তথ্য[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_উপরের(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            যদি( !c_তথ্য )
            {
                v_store(d_তথ্য + j, v_cvt_f32(vs_lo, vs_hi));
            }
            অন্যথা যদি( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_তথ্য_2);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_উপরের(vc), vbeta, vs_hi);
                v_store(d_তথ্য + j, v_cvt_f32(vs_lo, vs_hi));
            }
            অন্যথা
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) দশমিক buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                জন্য( পূর্ণসংখ্যা jj = 0; jj < vlanes_f32; jj++ )
                    d_তথ্য[j + jj] = buf[jj] + c_তথ্য_2[jj * c_step1] * (দশমিক)beta;
            }
        }
    }
}
#endif

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
স্থির অন্তঃরেখ শূন্য simdGEMM_kj(
    ধ্রুবক ST* a_তথ্য, ধ্রুবক ST* b_তথ্য_2, size_t b_step,
    ধ্রুবক ST* c_তথ্য, size_t c_step1,
    ST* d_তথ্য,
    পূর্ণসংখ্যা n, পূর্ণসংখ্যা m,
    দ্বিগুণ alpha, দ্বিগুণ beta,
    পূর্ণসংখ্যা& j)
{
    প্রকারলেখ typename VecTraits<ST>::VT VecT;
    ধ্রুবক পূর্ণসংখ্যা vlanes = VTraits<VecT>::vlanes();

    যদি( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ধ্রুবক পূর্ণসংখ্যা max_m = 1600 / (পূর্ণসংখ্যা)আকার(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            জন্য( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            জন্য( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ধ্রুবক ST* b_ptr = b_তথ্য_2;
            জন্য( পূর্ণসংখ্যা k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_তথ্য[k]);
                j = 0;
                জন্য( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_তথ্য[k];
                জন্য( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ধ্রুবক ST* c_তথ্য_2 = c_তথ্য;
            j = 0;
            যদি( !c_তথ্য )
            {
                জন্য( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_তথ্য + j, vs);
                }
                জন্য( ; j < m; j++ )
                    d_তথ্য[j] = s_buf[j] * (ST)alpha;
            }
            অন্যথা যদি( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                জন্য( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_তথ্য_2 + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_তথ্য + j, vs);
                }
                জন্য( ; j < m; j++ )
                    d_তথ্য[j] = s_buf[j] * (ST)alpha + c_তথ্য_2[j] * (ST)beta;
            }
            অন্যথা
            {
                জন্য( j = 0; j < m; j++, c_তথ্য_2 += c_step1 )
                    d_তথ্য[j] = s_buf[j] * (ST)alpha + c_তথ্য_2[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    অন্যথা যদি( vlanes * (পূর্ণসংখ্যা)আকার(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ধ্রুবক ST* c_তথ্য_2 = c_তথ্য;
        জন্য( ; j <= m - vlanes; j += vlanes, c_তথ্য_2 += vlanes * c_step1 )
        {
            ধ্রুবক ST* b = b_তথ্য_2 + j;
            VecT vs = VecTraits<ST>::zero();
            জন্য( পূর্ণসংখ্যা k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_তথ্য[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            যদি( !c_তথ্য )
            {
                v_store(d_তথ্য + j, vs);
            }
            অন্যথা যদি( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_তথ্য_2);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_তথ্য + j, vs);
            }
            অন্যথা
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                জন্য( পূর্ণসংখ্যা jj = 0; jj < vlanes; jj++ )
                    d_তথ্য[j + jj] = buf[jj] + c_তথ্য_2[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> স্থির শূন্য
GEMMSingleMul( ধ্রুবক T* a_তথ্য, size_t a_step,
               ধ্রুবক T* b_তথ্য, size_t b_step,
               ধ্রুবক T* c_তথ্য_2, size_t c_step,
               T* d_তথ্য, size_t d_step,
               আকার_2 a_আকার, আকার_2 d_আকার,
               দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    পূর্ণসংখ্যা i, j, k, n = a_আকার.width, m = d_আকার.width, drows = d_আকার.height;
    ধ্রুবক T *a_তথ্য_2 = a_তথ্য, *b_তথ্য_2 = b_তথ্য, *c_তথ্য = c_তথ্য_2;
    cv::Auto_বাফার<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= আকার(a_তথ্য[0]);
    b_step /= আকার(b_তথ্য[0]);
    c_step /= আকার(c_তথ্য_2[0]);
    d_step /= আকার(d_তথ্য[0]);
    a_step0 = a_step;
    a_step1 = 1;

    যদি( !c_তথ্য_2 )
        c_step0 = c_step1 = 0;
    অন্যথা যদি( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    অন্যথা
        c_step0 = 1, c_step1 = c_step;

    যদি( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_আকার.height;
        যদি( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.তথ্য();
        }
    }

    যদি( n == 1 ) /* external product */
    {
        cv::Auto_বাফার<T> _b_buf;
        T* b_buf = 0;

        যদি( a_step > 1 && a_আকার.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.তথ্য();
            জন্য( k = 0; k < drows; k++ )
                a_buf[k] = a_তথ্য[a_step*k];
            a_তথ্য = a_buf;
        }

        যদি( b_step > 1 )
        {
            _b_buf.allocate(d_আকার.width);
            b_buf = _b_buf.তথ্য();
            জন্য( j = 0; j < d_আকার.width; j++ )
                b_buf[j] = b_তথ্য[j*b_step];
            b_তথ্য = b_buf;
        }

        জন্য( i = 0; i < drows; i++, c_তথ্য += c_step0, d_তথ্য += d_step )
        {
            WT al = WT(a_তথ্য[i])*alpha;
            c_তথ্য_2 = c_তথ্য;
            জন্য( j = 0; j <= d_আকার.width - 2; j += 2, c_তথ্য_2 += 2*c_step1 )
            {
                WT s0 = al*WT(b_তথ্য[j]);
                WT s1 = al*WT(b_তথ্য[j+1]);
                যদি( !c_তথ্য_2 )
                {
                    d_তথ্য[j] = T(s0);
                    d_তথ্য[j+1] = T(s1);
                }
                অন্যথা
                {
                    d_তথ্য[j] = T(s0 + WT(c_তথ্য_2[0])*beta);
                    d_তথ্য[j+1] = T(s1 + WT(c_তথ্য_2[c_step1])*beta);
                }
            }

            জন্য( ; j < d_আকার.width; j++, c_তথ্য_2 += c_step1 )
            {
                WT s0 = al*WT(b_তথ্য[j]);
                যদি( !c_তথ্য_2 )
                    d_তথ্য[j] = T(s0);
                অন্যথা
                    d_তথ্য[j] = T(s0 + WT(c_তথ্য_2[0])*beta);
            }
        }
    }
    অন্যথা যদি( flags & GEMM_2_T ) /* A * Bt */
    {
        জন্য( i = 0; i < drows; i++, a_তথ্য_2 += a_step0, c_তথ্য += c_step0, d_তথ্য += d_step )
        {
            a_তথ্য = a_তথ্য_2;
            b_তথ্য = b_তথ্য_2;
            c_তথ্য_2 = c_তথ্য;

            যদি( a_buf )
            {
                জন্য( k = 0; k < n; k++ )
                    a_buf[k] = a_তথ্য[a_step1*k];
                a_তথ্য = a_buf;
            }

            জন্য( j = 0; j < d_আকার.width; j++, b_তথ্য += b_step,
                                               c_তথ্য_2 += c_step1 )
            {
                WT s0(0);
                k = 0;
#যদি CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                যদি( আকার(WT) == আকার(দ্বিগুণ) )
                {
                    যদি( আকার(T) == আকার(দ্বিগুণ) )
                        s0 = (WT)simdDotProduct((ধ্রুবক দ্বিগুণ*)a_তথ্য, (ধ্রুবক দ্বিগুণ*)b_তথ্য, n, k);
                    অন্যথা যদি( আকার(T) == আকার(দশমিক) )
                        s0 = (WT)simdDotProduct_f32f64((ধ্রুবক দশমিক*)a_তথ্য, (ধ্রুবক দশমিক*)b_তথ্য, n, k);
                }
                অন্যথা
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #যদি CV_ENABLE_UNROLLED
                    জন্য( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_তথ্য[k])*WT(b_তথ্য[k]);
                        s1 += WT(a_তথ্য[k+1])*WT(b_তথ্য[k+1]);
                        s2 += WT(a_তথ্য[k+2])*WT(b_তথ্য[k+2]);
                        s3 += WT(a_তথ্য[k+3])*WT(b_তথ্য[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                জন্য( ; k < n; k++ )
                    s0 += WT(a_তথ্য[k])*WT(b_তথ্য[k]);
                s0 = s0*alpha;

                যদি( !c_তথ্য_2 )
                    d_তথ্য[j] = T(s0);
                অন্যথা
                    d_তথ্য[j] = T(s0 + WT(c_তথ্য_2[0])*beta);
            }
        }
    }
    অন্যথা যদি( d_আকার.width*আকার(d_তথ্য[0]) <= 1600 )
    {
        জন্য( i = 0; i < drows; i++, a_তথ্য_2 += a_step0,
                                    c_তথ্য += c_step0,
                                    d_তথ্য += d_step )
        {
            a_তথ্য = a_তথ্য_2, c_তথ্য_2 = c_তথ্য;

            যদি( a_buf )
            {
                জন্য( k = 0; k < n; k++ )
                    a_buf[k] = a_তথ্য[a_step1*k];
                a_তথ্য = a_buf;
            }

            j = 0;
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            যদি( আকার(WT) == আকার(দ্বিগুণ) )
            {
                যদি( আকার(T) == আকার(দ্বিগুণ) )
                    simdGEMM_kj((ধ্রুবক দ্বিগুণ*)a_তথ্য, (ধ্রুবক দ্বিগুণ*)b_তথ্য_2, b_step,
                                        (ধ্রুবক দ্বিগুণ*)c_তথ্য, c_step1,
                                (দ্বিগুণ*)d_তথ্য, n, m, alpha, beta, j);
                অন্যথা যদি( আকার(T) == আকার(দশমিক) )
                    simdGEMM_kj_f32f64((ধ্রুবক দশমিক*)a_তথ্য, (ধ্রুবক দশমিক*)b_তথ্য_2, b_step,
                                        (ধ্রুবক দশমিক*)c_তথ্য, c_step1,
                                (দশমিক*)d_তথ্য, n, m, alpha, beta, j);
            }
#endif
            c_তথ্য_2 = c_তথ্য + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            জন্য( ; j <= m - 4; j += 4, c_তথ্য_2 += 4*c_step1 )
            {
                ধ্রুবক T* b = b_তথ্য_2 + j;
                WT s0(0), s1(0), s2(0), s3(0);

                জন্য( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_তথ্য[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                যদি( !c_তথ্য_2 )
                {
                    d_তথ্য[j] = T(s0*alpha);
                    d_তথ্য[j+1] = T(s1*alpha);
                    d_তথ্য[j+2] = T(s2*alpha);
                    d_তথ্য[j+3] = T(s3*alpha);
                }
                অন্যথা
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_তথ্য[j] = T(s0 + WT(c_তথ্য_2[0])*beta);
                    d_তথ্য[j+1] = T(s1 + WT(c_তথ্য_2[c_step1])*beta);
                    d_তথ্য[j+2] = T(s2 + WT(c_তথ্য_2[c_step1*2])*beta);
                    d_তথ্য[j+3] = T(s3 + WT(c_তথ্য_2[c_step1*3])*beta);
                }
            }

            জন্য( ; j < m; j++, c_তথ্য_2 += c_step1 )
            {
                ধ্রুবক T* b = b_তথ্য_2 + j;
                WT s0(0);

                জন্য( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_তথ্য[k]) * WT(b[0]);

                s0 = s0*alpha;
                যদি( !c_তথ্য_2 )
                    d_তথ্য[j] = T(s0);
                অন্যথা
                    d_তথ্য[j] = T(s0 + WT(c_তথ্য_2[0])*beta);
            }
        }
    }
    অন্যথা
    {
        cv::Auto_বাফার<WT> _d_buf(m);
        WT* d_buf = _d_buf.তথ্য();

        জন্য( i = 0; i < drows; i++, a_তথ্য_2 += a_step0, c_তথ্য += c_step0, d_তথ্য += d_step )
        {
            a_তথ্য = a_তথ্য_2;
            b_তথ্য = b_তথ্য_2;
            c_তথ্য_2 = c_তথ্য;

            যদি( a_buf )
            {
                জন্য( k = 0; k < n; k++ )
                    a_buf[k] = a_তথ্য_2[a_step1*k];
                a_তথ্য = a_buf;
            }

            জন্য( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            জন্য( k = 0; k < n; k++, b_তথ্য += b_step )
            {
                WT al(a_তথ্য[k]);
                j=0;
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                যদি( আকার(WT) == আকার(দ্বিগুণ) )
                {
                    যদি( আকার(T) == আকার(দ্বিগুণ) )
                    {
                        ধ্রুবক পূর্ণসংখ্যা vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ধ্রুবক দ্বিগুণ*)&al));
                        জন্য( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((ধ্রুবক দ্বিগুণ*)(d_buf + j));
                            v_float64 vb = vx_load((ধ্রুবক দ্বিগুণ*)(b_তথ্য + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((দ্বিগুণ*)(d_buf + j), vd);
                        }
                    }
                    অন্যথা
                    {
                        ধ্রুবক পূর্ণসংখ্যা vlanes_f32 = VTraits<v_float32>::vlanes();
                        ধ্রুবক পূর্ণসংখ্যা vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ধ্রুবক দ্বিগুণ*)&al));
                        জন্য( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((ধ্রুবক দশমিক*)(b_তথ্য + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_উপরের(vb);
                            v_float64 vd_lo = vx_load((ধ্রুবক দ্বিগুণ*)(d_buf + j));
                            v_float64 vd_hi = vx_load((ধ্রুবক দ্বিগুণ*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((দ্বিগুণ*)(d_buf + j), vd_lo);
                            v_store((দ্বিগুণ*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                জন্য( ; j < m; j++ )
                    d_buf[j] += WT(b_তথ্য[j])*al;
            }

            যদি( !c_তথ্য_2 )
                জন্য( j = 0; j < m; j++ )
                    d_তথ্য[j] = T(d_buf[j]*alpha);
            অন্যথা
                জন্য( j = 0; j < m; j++, c_তথ্য_2 += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_তথ্য[j] = T(t + WT(c_তথ্য_2[0])*beta);
                }
        }
    }
}
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
স্থির অন্তঃরেখ শূন্য set_Double_তথ্য_Zero(WT* d_তথ্য, পূর্ণসংখ্যা m, পূর্ণসংখ্যা vlanes)
{
    v_float64 vz = vx_setzero_f64();
    পূর্ণসংখ্যা j = 0;
    জন্য( ; j <= m - vlanes; j += vlanes )
        v_store((দ্বিগুণ*)(d_তথ্য + j), vz);
    জন্য( ; j < m; j++ )
        d_তথ্য[j] = WT(0);
}

template<typename ST>
স্থির অন্তঃরেখ শূন্য simdMulAdd(ধ্রুবক ST* b, দ্বিগুণ* d, পূর্ণসংখ্যা m, দ্বিগুণ a_val);

template<>
অন্তঃরেখ শূন্য simdMulAdd<দশমিক>(
    ধ্রুবক দশমিক* b, দ্বিগুণ* d, পূর্ণসংখ্যা m, দ্বিগুণ a_val)
{
    ধ্রুবক পূর্ণসংখ্যা vlanes_f32 = VTraits<v_float32>::vlanes();
    ধ্রুবক পূর্ণসংখ্যা vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    পূর্ণসংখ্যা j = 0;
    জন্য( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_উপরের(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    জন্য( ; j < m; j++ )
        d[j] += a_val * (দ্বিগুণ)b[j];
}

template<>
অন্তঃরেখ শূন্য simdMulAdd<দ্বিগুণ>(
    ধ্রুবক দ্বিগুণ* b, দ্বিগুণ* d, পূর্ণসংখ্যা m, দ্বিগুণ a_val)
{
    ধ্রুবক পূর্ণসংখ্যা vlanes = VTraits<v_float64>::vlanes();
    ধ্রুবক পূর্ণসংখ্যা stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    পূর্ণসংখ্যা j = 0;
    জন্য( ; j <= m - stride2; j += stride2 )
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
    জন্য( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    জন্য( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
স্থির অন্তঃরেখ শূন্য simdBlockMul_kj(
    ধ্রুবক ST* a_তথ্য, ধ্রুবক ST* b_তথ্য, size_t b_step,
    দ্বিগুণ* d_তথ্য, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, যৌক্তিক do_acc)
{
    ধ্রুবক পূর্ণসংখ্যা vlanes = VTraits<v_float64>::vlanes();
    যদি( !do_acc )
        set_Double_তথ্য_Zero(d_তথ্য, m, vlanes);
    জন্য( পূর্ণসংখ্যা k = 0; k < n; k++, b_তথ্য += b_step )
        simdMulAdd(b_তথ্য, d_তথ্য, m, (দ্বিগুণ)a_তথ্য[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> স্থির শূন্য
GEMMBlockMul( ধ্রুবক T* a_তথ্য, size_t a_step,
              ধ্রুবক T* b_তথ্য, size_t b_step,
              WT* d_তথ্য, size_t d_step,
              আকার_2 a_আকার, আকার_2 d_আকার, পূর্ণসংখ্যা flags )
{
    পূর্ণসংখ্যা i, j, k, n = a_আকার.width, m = d_আকার.width;
    ধ্রুবক T *a_তথ্য_2 = a_তথ্য, *b_তথ্য_2 = b_তথ্য;
    cv::Auto_বাফার<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    পূর্ণসংখ্যা do_acc = flags & 16;

    a_step /= আকার(a_তথ্য[0]);
    b_step /= আকার(b_তথ্য[0]);
    d_step /= আকার(d_তথ্য[0]);

    a_step0 = a_step;
    a_step1 = 1;

    যদি( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_আকার.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.তথ্য();
    }

    যদি( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        জন্য( i = 0; i < d_আকার.height; i++, a_তথ্য_2 += a_step0, d_তথ্য += d_step )
        {
            a_তথ্য = a_তথ্য_2; b_তথ্য = b_তথ্য_2;

            যদি( a_buf )
            {
                জন্য( k = 0; k < n; k++ )
                    a_buf[k] = a_তথ্য[a_step1*k];
                a_তথ্য = a_buf;
            }

            জন্য( j = 0; j < d_আকার.width; j++, b_তথ্য += b_step )
            {
                WT s0 = do_acc ? d_তথ্য[j] : WT(0);
                k = 0;
#যদি CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                যদি( আকার(WT) == আকার(দ্বিগুণ) )
                {
                    যদি( আকার(T) == আকার(দ্বিগুণ) )
                        s0 += (WT)simdDotProduct((ধ্রুবক দ্বিগুণ*)a_তথ্য, (ধ্রুবক দ্বিগুণ*)b_তথ্য, n, k);
                    অন্যথা যদি( আকার(T) == আকার(দশমিক) )
                        s0 += (WT)simdDotProduct_f32f64((ধ্রুবক দশমিক*)a_তথ্য, (ধ্রুবক দশমিক*)b_তথ্য, n, k);
                }
                অন্যথা
#endif
                {
                    WT s1(0);
                    জন্য( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_তথ্য[k])*WT(b_তথ্য[k]);
                        s1 += WT(a_তথ্য[k+1])*WT(b_তথ্য[k+1]);
                    }
                    s0 += s1;
                }
                জন্য( ; k < n; k++ )
                    s0 += WT(a_তথ্য[k])*WT(b_তথ্য[k]);

                d_তথ্য[j] = s0;
            }
        }
    }
    অন্যথা
    {
        জন্য( i = 0; i < d_আকার.height; i++, a_তথ্য_2 += a_step0, d_তথ্য += d_step )
        {
            a_তথ্য = a_তথ্য_2, b_তথ্য = b_তথ্য_2;

            যদি( a_buf )
            {
                জন্য( k = 0; k < n; k++ )
                    a_buf[k] = a_তথ্য[a_step1*k];
                a_তথ্য = a_buf;
            }
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            যদি( আকার(WT) == আকার(দ্বিগুণ) )
            {
                যদি( আকার(T) == আকার(দ্বিগুণ) )
                    simdBlockMul_kj(
                        (ধ্রুবক দ্বিগুণ*)a_তথ্য, (ধ্রুবক দ্বিগুণ*)b_তথ্য, b_step,
                        (দ্বিগুণ*)d_তথ্য, n, m, do_acc != 0);
                অন্যথা
                    simdBlockMul_kj(
                        (ধ্রুবক দশমিক*)a_তথ্য, (ধ্রুবক দশমিক*)b_তথ্য, b_step,
                        (দ্বিগুণ*)d_তথ্য, n, m, do_acc != 0);
            }
            অন্যথা
#endif
            {
                জন্য( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ধ্রুবক T* b = b_তথ্য + j;

                    যদি( do_acc )
                    {
                        s0 = d_তথ্য[j]; s1 = d_তথ্য[j+1];
                        s2 = d_তথ্য[j+2]; s3 = d_তথ্য[j+3];
                    }
                    অন্যথা
                        s0 = s1 = s2 = s3 = WT(0);

                    জন্য( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_তথ্য[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_তথ্য[j] = s0; d_তথ্য[j+1] = s1;
                    d_তথ্য[j+2] = s2; d_তথ্য[j+3] = s3;
                }

                জন্য( ; j < m; j++ )
                {
                    ধ্রুবক T* b = b_তথ্য + j;
                    WT s0 = do_acc ? d_তথ্য[j] : WT(0);

                    জন্য( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_তথ্য[k]) * WT(b[0]);

                    d_তথ্য[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> স্থির শূন্য
GEMMStore( ধ্রুবক T* c_তথ্য_2, size_t c_step,
           ধ্রুবক WT* d_buf, size_t d_buf_step,
           T* d_তথ্য, size_t d_step, আকার_2 d_আকার,
           দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    ধ্রুবক T* c_তথ্য = c_তথ্য_2;
    পূর্ণসংখ্যা j;
    size_t c_step0, c_step1;

    c_step /= আকার(c_তথ্য_2[0]);
    d_buf_step /= আকার(d_buf[0]);
    d_step /= আকার(d_তথ্য[0]);

    যদি( !c_তথ্য_2 )
        c_step0 = c_step1 = 0;
    অন্যথা যদি( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    অন্যথা
        c_step0 = 1, c_step1 = c_step;

    জন্য( ; d_আকার.height--; c_তথ্য += c_step0, d_buf += d_buf_step, d_তথ্য += d_step )
    {
        যদি( c_তথ্য )
        {
            c_তথ্য_2 = c_তথ্য;
            j=0;
             #যদি CV_ENABLE_UNROLLED
            জন্য(; j <= d_আকার.width - 4; j += 4, c_তথ্য_2 += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_তথ্য_2[0]);
                t1 += beta*WT(c_তথ্য_2[c_step1]);
                d_তথ্য[j] = T(t0);
                d_তথ্য[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_তথ্য_2[c_step1*2]);
                t1 += beta*WT(c_তথ্য_2[c_step1*3]);
                d_তথ্য[j+2] = T(t0);
                d_তথ্য[j+3] = T(t1);
            }
            #endif
            জন্য( ; j < d_আকার.width; j++, c_তথ্য_2 += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_তথ্য[j] = T(t0 + WT(c_তথ্য_2[0])*beta);
            }
        }
        অন্যথা
        {
            j = 0;
             #যদি CV_ENABLE_UNROLLED
            জন্য( ; j <= d_আকার.width - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_তথ্য[j] = T(t0);
                d_তথ্য[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_তথ্য[j+2] = T(t0);
                d_তথ্য[j+3] = T(t1);
            }
            #endif
            জন্য( ; j < d_আকার.width; j++ )
                d_তথ্য[j] = T(alpha*d_buf[j]);
        }
    }
}


প্রকারলেখ শূন্য (*GEMMSingleMulFunc)( ধ্রুবক শূন্য* src1, size_t step1,
                   ধ্রুবক শূন্য* src2, size_t step2, ধ্রুবক শূন্য* src3, size_t step3,
                   শূন্য* dst, size_t dststep, আকার_2 srcsize, আকার_2 dstsize,
                   দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags );

প্রকারলেখ শূন্য (*GEMMBlockMulFunc)( ধ্রুবক শূন্য* src1, size_t step1,
                   ধ্রুবক শূন্য* src2, size_t step2, শূন্য* dst, size_t dststep,
                   আকার_2 srcsize, আকার_2 dstsize, পূর্ণসংখ্যা flags );

প্রকারলেখ শূন্য (*GEMMStoreFunc)( ধ্রুবক শূন্য* src1, size_t step1,
                   ধ্রুবক শূন্য* src2, size_t step2, শূন্য* dst, size_t dststep,
                   আকার_2 dstsize, দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags );

template<typename T, typename WT, শূন্য (*fn)(ধ্রুবক T*, size_t, ধ্রুবক T*, size_t, ধ্রুবক T*, size_t, T*, size_t, আকার_2, আকার_2, দ্বিগুণ, দ্বিগুণ, পূর্ণসংখ্যা)>
স্থির শূন্য gemmSingleMulWrap( ধ্রুবক শূন্য* src1, size_t step1,
                   ধ্রুবক শূন্য* src2, size_t step2, ধ্রুবক শূন্য* src3, size_t step3,
                   শূন্য* dst, size_t dststep, আকার_2 srcsize, আকার_2 dstsize,
                   দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    fn((ধ্রুবক T*)src1, step1, (ধ্রুবক T*)src2, step2, (ধ্রুবক T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, শূন্য (*fn)(ধ্রুবক T*, size_t, ধ্রুবক T*, size_t, WT*, size_t, আকার_2, আকার_2, পূর্ণসংখ্যা)>
স্থির শূন্য gemmBlockMulWrap( ধ্রুবক শূন্য* src1, size_t step1,
                   ধ্রুবক শূন্য* src2, size_t step2, শূন্য* dst, size_t dststep,
                   আকার_2 srcsize, আকার_2 dstsize, পূর্ণসংখ্যা flags )
{
    fn((ধ্রুবক T*)src1, step1, (ধ্রুবক T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, শূন্য (*fn)(ধ্রুবক T*, size_t, ধ্রুবক WT*, size_t, T*, size_t, আকার_2, দ্বিগুণ, দ্বিগুণ, পূর্ণসংখ্যা)>
স্থির শূন্য gemmStoreWrap( ধ্রুবক শূন্য* src1, size_t step1,
                   ধ্রুবক শূন্য* src2, size_t step2, শূন্য* dst, size_t dststep,
                   আকার_2 dstsize, দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    fn((ধ্রুবক T*)src1, step1, (ধ্রুবক WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

স্থির শূন্য GEMMSingleMul_32f( ধ্রুবক দশমিক* a_তথ্য, size_t a_step,
              ধ্রুবক দশমিক* b_তথ্য, size_t b_step,
              ধ্রুবক দশমিক* c_তথ্য_2, size_t c_step,
              দশমিক* d_তথ্য, size_t d_step,
              আকার_2 a_আকার, আকার_2 d_আকার,
              দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    GEMMSingleMul<দশমিক,দ্বিগুণ>(a_তথ্য, a_step, b_তথ্য, b_step, c_তথ্য_2,
                                c_step, d_তথ্য, d_step, a_আকার, d_আকার,
                                alpha, beta, flags);
}

স্থির শূন্য GEMMSingleMul_64f( ধ্রুবক দ্বিগুণ* a_তথ্য, size_t a_step,
                              ধ্রুবক দ্বিগুণ* b_তথ্য, size_t b_step,
                              ধ্রুবক দ্বিগুণ* c_তথ্য_2, size_t c_step,
                              দ্বিগুণ* d_তথ্য, size_t d_step,
                              আকার_2 a_আকার, আকার_2 d_আকার,
                              দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    GEMMSingleMul<দ্বিগুণ,দ্বিগুণ>(a_তথ্য, a_step, b_তথ্য, b_step, c_তথ্য_2,
                                c_step, d_তথ্য, d_step, a_আকার, d_আকার,
                                alpha, beta, flags);
}


স্থির শূন্য GEMMSingleMul_32fc( ধ্রুবক Complexf* a_তথ্য, size_t a_step,
                              ধ্রুবক Complexf* b_তথ্য, size_t b_step,
                              ধ্রুবক Complexf* c_তথ্য_2, size_t c_step,
                              Complexf* d_তথ্য, size_t d_step,
                              আকার_2 a_আকার, আকার_2 d_আকার,
                              দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_তথ্য, a_step, b_তথ্য, b_step, c_তথ্য_2,
                                c_step, d_তথ্য, d_step, a_আকার, d_আকার,
                                alpha, beta, flags);
}

স্থির শূন্য GEMMSingleMul_64fc( ধ্রুবক Complexd* a_তথ্য, size_t a_step,
                              ধ্রুবক Complexd* b_তথ্য, size_t b_step,
                              ধ্রুবক Complexd* c_তথ্য_2, size_t c_step,
                              Complexd* d_তথ্য, size_t d_step,
                              আকার_2 a_আকার, আকার_2 d_আকার,
                              দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_তথ্য, a_step, b_তথ্য, b_step, c_তথ্য_2,
                                 c_step, d_তথ্য, d_step, a_আকার, d_আকার,
                                 alpha, beta, flags);
}

স্থির শূন্য GEMMBlockMul_32f( ধ্রুবক দশমিক* a_তথ্য, size_t a_step,
             ধ্রুবক দশমিক* b_তথ্য, size_t b_step,
             দ্বিগুণ* d_তথ্য, size_t d_step,
             আকার_2 a_আকার, আকার_2 d_আকার, পূর্ণসংখ্যা flags )
{
    GEMMBlockMul(a_তথ্য, a_step, b_তথ্য, b_step, d_তথ্য, d_step, a_আকার, d_আকার, flags);
}


স্থির শূন্য GEMMBlockMul_64f( ধ্রুবক দ্বিগুণ* a_তথ্য, size_t a_step,
                             ধ্রুবক দ্বিগুণ* b_তথ্য, size_t b_step,
                             দ্বিগুণ* d_তথ্য, size_t d_step,
                             আকার_2 a_আকার, আকার_2 d_আকার, পূর্ণসংখ্যা flags )
{
    GEMMBlockMul(a_তথ্য, a_step, b_তথ্য, b_step, d_তথ্য, d_step, a_আকার, d_আকার, flags);
}


স্থির শূন্য GEMMBlockMul_32fc( ধ্রুবক Complexf* a_তথ্য, size_t a_step,
                             ধ্রুবক Complexf* b_তথ্য, size_t b_step,
                             Complexd* d_তথ্য, size_t d_step,
                             আকার_2 a_আকার, আকার_2 d_আকার, পূর্ণসংখ্যা flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_তথ্য, a_step, b_তথ্য, b_step, d_তথ্য, d_step, a_আকার, d_আকার, flags);
}


স্থির শূন্য GEMMBlockMul_64fc( ধ্রুবক Complexd* a_তথ্য, size_t a_step,
                             ধ্রুবক Complexd* b_তথ্য, size_t b_step,
                             Complexd* d_তথ্য, size_t d_step,
                             আকার_2 a_আকার, আকার_2 d_আকার, পূর্ণসংখ্যা flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_তথ্য, a_step, b_তথ্য, b_step, d_তথ্য, d_step, a_আকার, d_আকার, flags);
}


স্থির শূন্য GEMMStore_32f( ধ্রুবক দশমিক* c_তথ্য_2, size_t c_step,
          ধ্রুবক দ্বিগুণ* d_buf, size_t d_buf_step,
          দশমিক* d_তথ্য, size_t d_step, আকার_2 d_আকার,
          দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    GEMMStore(c_তথ্য_2, c_step, d_buf, d_buf_step, d_তথ্য, d_step, d_আকার, alpha, beta, flags);
}


স্থির শূন্য GEMMStore_64f( ধ্রুবক দ্বিগুণ* c_তথ্য_2, size_t c_step,
                      ধ্রুবক দ্বিগুণ* d_buf, size_t d_buf_step,
                      দ্বিগুণ* d_তথ্য, size_t d_step, আকার_2 d_আকার,
                      দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    GEMMStore(c_তথ্য_2, c_step, d_buf, d_buf_step, d_তথ্য, d_step, d_আকার, alpha, beta, flags);
}


স্থির শূন্য GEMMStore_32fc( ধ্রুবক Complexf* c_তথ্য_2, size_t c_step,
                          ধ্রুবক Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_তথ্য, size_t d_step, আকার_2 d_আকার,
                          দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    GEMMStore(c_তথ্য_2, c_step, d_buf, d_buf_step, d_তথ্য, d_step, d_আকার, alpha, beta, flags);
}


স্থির শূন্য GEMMStore_64fc( ধ্রুবক Complexd* c_তথ্য_2, size_t c_step,
                          ধ্রুবক Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_তথ্য, size_t d_step, আকার_2 d_আকার,
                          দ্বিগুণ alpha, দ্বিগুণ beta, পূর্ণসংখ্যা flags )
{
    GEMMStore(c_তথ্য_2, c_step, d_buf, d_buf_step, d_তথ্য, d_step, d_আকার, alpha, beta, flags);
}

স্থির শূন্য gemmImpl( Mat A, Mat B, দ্বিগুণ alpha,
           Mat C, দ্বিগুণ beta, Mat D, পূর্ণসংখ্যা flags )
{
    CV_INSTRUMENT_REGION();

    ধ্রুবক পূর্ণসংখ্যা block_lin_আকার = 128;
    ধ্রুবক পূর্ণসংখ্যা block_আকার_2 = block_lin_আকার * block_lin_আকার;

    স্থির দ্বিগুণ zero[] = {0,0,0,0};
    স্থির দশমিক zerof[] = {0,0,0,0};

    আকার_2 a_আকার = A.size(), d_আকার;
    পূর্ণসংখ্যা i, len = 0, type = A.type();

    নির্বাচন( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ক্ষেত্র 0:
        d_আকার = আকার_2( B.cols, a_আকার.height );
        len = B.rows;
        বিরতি;
    ক্ষেত্র 1:
        d_আকার = আকার_2( B.cols, a_আকার.width );
        len = B.rows;
        বিরতি;
    ক্ষেত্র 2:
        d_আকার = আকার_2( B.rows, a_আকার.height );
        len = B.cols;
        বিরতি;
    ক্ষেত্র 3:
        d_আকার = আকার_2( B.rows, a_আকার.width );
        len = B.cols;
        বিরতি;
    }

    যদি( flags == 0 && 2 <= len && len <= 4 && (len == d_আকার.width || len == d_আকার.height) )
    {
        যদি( type == CV_32F )
        {
            দশমিক* d = D.ptr<দশমিক>();
            ধ্রুবক দশমিক *a = A.ptr<দশমিক>(),
                        *b = B.ptr<দশমিক>(),
                        *c = (ধ্রুবক দশমিক*)C.তথ্য;
            size_t d_step = D.step/আকার(d[0]),
                a_step = A.step/আকার(a[0]),
                b_step = B.step/আকার(b[0]),
                c_step = C.তথ্য ? C.step/আকার(c[0]) : 0;

            যদি( !c )
                c = zerof;

            নির্বাচন( len )
            {
            ক্ষেত্র 2:
                যদি( len == d_আকার.width && b != d )
                {
                    জন্য( i = 0; i < d_আকার.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দশমিক t0 = a[0]*b[0] + a[1]*b[b_step];
                        দশমিক t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (দশমিক)(t0*alpha + c[0]*beta);
                        d[1] = (দশমিক)(t1*alpha + c[1]*beta);
                    }
                }
                অন্যথা যদি( a != d )
                {
                    পূর্ণসংখ্যা c_step0 = 1;
                    যদি( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    জন্য( i = 0; i < d_আকার.width; i++, d++, b++, c += c_step0 )
                    {
                        দশমিক t0 = a[0]*b[0] + a[1]*b[b_step];
                        দশমিক t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (দশমিক)(t0*alpha + c[0]*beta);
                        d[d_step] = (দশমিক)(t1*alpha + c[c_step]*beta);
                    }
                }
                অন্যথা
                    বিরতি;
                ফেরত;
            ক্ষেত্র 3:
                যদি( len == d_আকার.width && b != d )
                {
                    জন্য( i = 0; i < d_আকার.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দশমিক t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        দশমিক t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        দশমিক t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (দশমিক)(t0*alpha + c[0]*beta);
                        d[1] = (দশমিক)(t1*alpha + c[1]*beta);
                        d[2] = (দশমিক)(t2*alpha + c[2]*beta);
                    }
                }
                অন্যথা যদি( a != d )
                {
                    পূর্ণসংখ্যা c_step0 = 1;
                    যদি( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    জন্য( i = 0; i < d_আকার.width; i++, d++, b++, c += c_step0 )
                    {
                        দশমিক t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        দশমিক t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        দশমিক t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (দশমিক)(t0*alpha + c[0]*beta);
                        d[d_step] = (দশমিক)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (দশমিক)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                অন্যথা
                    বিরতি;
                ফেরত;
            ক্ষেত্র 4:
                যদি( len == d_আকার.width && b != d )
                {
                    জন্য( i = 0; i < d_আকার.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দশমিক t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        দশমিক t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        দশমিক t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        দশমিক t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (দশমিক)(t0*alpha + c[0]*beta);
                        d[1] = (দশমিক)(t1*alpha + c[1]*beta);
                        d[2] = (দশমিক)(t2*alpha + c[2]*beta);
                        d[3] = (দশমিক)(t3*alpha + c[3]*beta);
                    }
                }
                অন্যথা যদি( len <= 16 && a != d )
                {
                    পূর্ণসংখ্যা c_step0 = 1;
                    যদি( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    জন্য( i = 0; i < d_আকার.width; i++, d++, b++, c += c_step0 )
                    {
                        দশমিক t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        দশমিক t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        দশমিক t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        দশমিক t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (দশমিক)(t0*alpha + c[0]*beta);
                        d[d_step] = (দশমিক)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (দশমিক)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (দশমিক)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                অন্যথা
                    বিরতি;
                ফেরত;
            }
        }

        যদি( type == CV_64F )
        {
            দ্বিগুণ* d = D.ptr<দ্বিগুণ>();
            ধ্রুবক দ্বিগুণ *a = A.ptr<দ্বিগুণ>(),
                         *b = B.ptr<দ্বিগুণ>(),
                         *c = (ধ্রুবক দ্বিগুণ*)C.তথ্য;
            size_t d_step = D.step/আকার(d[0]),
                a_step = A.step/আকার(a[0]),
                b_step = B.step/আকার(b[0]),
                c_step = C.তথ্য ? C.step/আকার(c[0]) : 0;
            যদি( !c )
                c = zero;

            নির্বাচন( len )
            {
            ক্ষেত্র 2:
                যদি( len == d_আকার.width && b != d )
                {
                    জন্য( i = 0; i < d_আকার.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দ্বিগুণ t0 = a[0]*b[0] + a[1]*b[b_step];
                        দ্বিগুণ t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                অন্যথা যদি( a != d )
                {
                    পূর্ণসংখ্যা c_step0 = 1;
                    যদি( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    জন্য( i = 0; i < d_আকার.width; i++, d++, b++, c += c_step0 )
                    {
                        দ্বিগুণ t0 = a[0]*b[0] + a[1]*b[b_step];
                        দ্বিগুণ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                অন্যথা
                    বিরতি;
                ফেরত;
            ক্ষেত্র 3:
                যদি( len == d_আকার.width && b != d )
                {
                    জন্য( i = 0; i < d_আকার.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দ্বিগুণ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        দ্বিগুণ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        দ্বিগুণ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                অন্যথা যদি( a != d )
                {
                    পূর্ণসংখ্যা c_step0 = 1;
                    যদি( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    জন্য( i = 0; i < d_আকার.width; i++, d++, b++, c += c_step0 )
                    {
                        দ্বিগুণ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        দ্বিগুণ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        দ্বিগুণ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                অন্যথা
                    বিরতি;
                ফেরত;
            ক্ষেত্র 4:
                যদি( len == d_আকার.width && b != d )
                {
                    জন্য( i = 0; i < d_আকার.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দ্বিগুণ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        দ্বিগুণ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        দ্বিগুণ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        দ্বিগুণ t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                অন্যথা যদি( d_আকার.width <= 16 && a != d )
                {
                    পূর্ণসংখ্যা c_step0 = 1;
                    যদি( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    জন্য( i = 0; i < d_আকার.width; i++, d++, b++, c += c_step0 )
                    {
                        দ্বিগুণ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        দ্বিগুণ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        দ্বিগুণ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        দ্বিগুণ t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                অন্যথা
                    বিরতি;
                ফেরত;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ধ্রুবক uchar* Cdata = C.তথ্য;
    size_t Cstep = C.তথ্য ? (size_t)C.step : 0;
    Auto_বাফার<uchar> buf;

    যদি( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<দশমিক, দ্বিগুণ, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<দশমিক, দ্বিগুণ, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<দশমিক, দ্বিগুণ, GEMMStore_32f>;
    }
    অন্যথা যদি( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<দ্বিগুণ, দ্বিগুণ, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<দ্বিগুণ, দ্বিগুণ, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<দ্বিগুণ, দ্বিগুণ, GEMMStore_64f>;
    }
    অন্যথা যদি( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    অন্যথা
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    যদি( (d_আকার.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_step = d_আকার.width == 1 ? 0 : CV_ELEM_আকার(type);
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
    else*/ যদি( ((d_আকার.height <= block_lin_আকার/2 || d_আকার.width <= block_lin_আকার/2) &&
        len <= 10000) || len <= 10 ||
        (d_আকার.width <= block_lin_আকার &&
        d_আকার.height <= block_lin_আকার && len <= block_lin_আকার) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_আকার, d_আকার, alpha, beta, flags );
    }
    অন্যথা
    {
        পূর্ণসংখ্যা is_a_t = flags & GEMM_1_T;
        পূর্ণসংখ্যা is_b_t = flags & GEMM_2_T;
        পূর্ণসংখ্যা elem_আকার_2 = CV_ELEM_আকার(type);
        পূর্ণসংখ্যা dk0_1, dk0_2;
        size_t a_buf_আকার = 0, b_buf_আকার, d_buf_আকার;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        পূর্ণসংখ্যা j, k, di = 0, dj = 0, dk = 0;
        পূর্ণসংখ্যা dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        পূর্ণসংখ্যা work_elem_আকার = elem_আকার_2 << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        যদি( !is_a_t )
            a_step0 = A.step, a_step1 = elem_আকার_2;
        অন্যথা
            a_step0 = elem_আকার_2, a_step1 = A.step;

        যদি( !is_b_t )
            b_step0 = b_step, b_step1 = elem_আকার_2;
        অন্যথা
            b_step0 = elem_আকার_2, b_step1 = b_step;

        যদি( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        অন্যথা যদি( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_আকার_2;
        অন্যথা
            c_step0 = elem_আকার_2, c_step1 = C.step;

        dm0 = std::min( block_lin_আকার, d_আকার.height );
        dn0 = std::min( block_lin_আকার, d_আকার.width );
        dk0_1 = block_আকার_2 / dm0;
        dk0_2 = block_আকার_2 / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        যদি( dk0*dm0 > block_আকার_2 )
            dm0 = block_আকার_2 / dk0;
        যদি( dk0*dn0 > block_আকার_2 )
            dn0 = block_আকার_2 / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_আকার = (size_t)(dk0+dk0/8+1)*dk0_1*elem_আকার_2;
        d_buf_আকার = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_আকার;

        যদি( is_a_t )
        {
            a_buf_আকার = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_আকার_2;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_আকার + b_buf_আকার + a_buf_আকার);
        d_buf = buf.তথ্য();
        b_buf = d_buf + d_buf_আকার;

        যদি( is_a_t )
            a_buf = b_buf + b_buf_আকার;

        জন্য( i = 0; i < d_আকার.height; i += di )
        {
            di = dm0;
            যদি( i + di >= d_আকার.height || 8*(i + di) + di > 8*d_আকার.height )
                di = d_আকার.height - i;

            জন্য( j = 0; j < d_আকার.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_আকার_2;
                ধ্রুবক uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                যদি( j + dj >= d_আকার.width || 8*(j + dj) + dj > 8*d_আকার.width )
                    dj = d_আকার.width - j;

                flags &= 15;
                যদি( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_আকার;
                }

                জন্য( k = 0; k < len; k += dk )
                {
                    ধ্রুবক uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    ধ্রুবক uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    আকার_2 a_bl_আকার;

                    dk = dk0;
                    যদি( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    যদি( !is_a_t )
                        a_bl_আকার.width = dk, a_bl_আকার.height = di;
                    অন্যথা
                        a_bl_আকার.width = di, a_bl_আকার.height = dk;

                    যদি( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_আকার_2;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_আকার, elem_আকার_2 );
                        std::swap( a_bl_আকার.width, a_bl_আকার.height );
                        _a = a_buf;
                    }

                    যদি( dj < d_আকার.width )
                    {
                        আকার_2 b_আকার;
                        যদি( !is_b_t )
                            b_আকার.width = dj, b_আকার.height = dk;
                        অন্যথা
                            b_আকার.width = dk, b_আকার.height = dj;

                        _b_step = b_আকার.width*elem_আকার_2;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_আকার, elem_আকার_2 );
                        _b = b_buf;
                    }

                    যদি( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_আকার, আকার_2(dj,di), flags );
                    অন্যথা
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_আকার, আকার_2(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                যদি( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_আকার_2,
                               matD->step, আকার_2(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>অন্তঃরেখ স্থির শূন্য
callGemmImpl(ধ্রুবক fptype *src1, size_t src1_step, ধ্রুবক fptype *src2, size_t src2_step, fptype alpha,
          ধ্রুবক fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    পূর্ণসংখ্যা b_m, b_n, c_m, c_n, m_d;

    যদি(flags & GEMM_2_T)
    {
        b_m = n_d;
        যদি(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        অন্যথা
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    অন্যথা
    {
        b_n = n_d;
        যদি(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        অন্যথা
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    যদি(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    অন্যথা
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    যদি(src1 != ফাঁকা)
        A = Mat(m_a, n_a, type, (শূন্য*)src1, src1_step);
    যদি(src2 != ফাঁকা)
        B = Mat(b_m, b_n, type, (শূন্য*)src2, src2_step);
    যদি(src3 != ফাঁকা && beta != 0.0)
        C = Mat(c_m, c_n, type, (শূন্য*)src3, src3_step);
    Mat D(m_d, n_d, type, (শূন্য*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

শূন্য gemm32f(ধ্রুবক দশমিক* src1, size_t src1_step, ধ্রুবক দশমিক* src2, size_t src2_step,
             দশমিক alpha, ধ্রুবক দশমিক* src3, size_t src3_step, দশমিক beta, দশমিক* dst, size_t dst_step,
             পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

শূন্য gemm64f(ধ্রুবক দ্বিগুণ* src1, size_t src1_step, ধ্রুবক দ্বিগুণ* src2, size_t src2_step,
             দ্বিগুণ alpha, ধ্রুবক দ্বিগুণ* src3, size_t src3_step, দ্বিগুণ beta, দ্বিগুণ* dst, size_t dst_step,
             পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

শূন্য gemm32fc(ধ্রুবক দশমিক* src1, size_t src1_step, ধ্রুবক দশমিক* src2, size_t src2_step,
              দশমিক alpha, ধ্রুবক দশমিক* src3, size_t src3_step, দশমিক beta, দশমিক* dst, size_t dst_step,
              পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

শূন্য gemm64fc(ধ্রুবক দ্বিগুণ* src1, size_t src1_step, ধ্রুবক দ্বিগুণ* src2, size_t src2_step,
              দ্বিগুণ alpha, ধ্রুবক দ্বিগুণ* src3, size_t src3_step, দ্বিগুণ beta, দ্বিগুণ* dst, size_t dst_step,
              পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> স্থির শূন্য
transform_( ধ্রুবক T* src, T* dst, ধ্রুবক WT* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn )
{
    পূর্ণসংখ্যা x;

    যদি( scn == 2 && dcn == 2 )
    {
        জন্য( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    অন্যথা যদি( scn == 3 && dcn == 3 )
    {
        জন্য( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    অন্যথা যদি( scn == 3 && dcn == 1 )
    {
        জন্য( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    অন্যথা যদি( scn == 4 && dcn == 4 )
    {
        জন্য( x = 0; x < len*4; x += 4 )
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
    অন্যথা
    {
        জন্য( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ধ্রুবক WT* _m = m;
            পূর্ণসংখ্যা j, k;
            জন্য( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                জন্য( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

স্থির শূন্য
transform_8u( ধ্রুবক uchar* src, uchar* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn )
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুবক পূর্ণসংখ্যা BITS = 10, SCALE = 1 << BITS;
    ধ্রুবক দশমিক MAX_M = (দশমিক)(1 << (15 - BITS));

    যদি( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ধ্রুবক পূর্ণসংখ্যা nChannels = 3;

        সংঘ {
            short s[6];
            পূর্ণসংখ্যা p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        পূর্ণসংখ্যা m32[] = {saturate_cast<পূর্ণসংখ্যা>(m[ 2] * SCALE), saturate_cast<পূর্ণসংখ্যা>(m[ 3] * SCALE),
                     saturate_cast<পূর্ণসংখ্যা>(m[ 6] * SCALE), saturate_cast<পূর্ণসংখ্যা>(m[ 7] * SCALE),
                     saturate_cast<পূর্ণসংখ্যা>(m[10] * SCALE), saturate_cast<পূর্ণসংখ্যা>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        পূর্ণসংখ্যা x = 0;
        জন্য (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<পূর্ণসংখ্যা>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<পূর্ণসংখ্যা>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<পূর্ণসংখ্যা>((m[11] + 0.5f)*SCALE);
        জন্য( ; x < len * nChannels; x += nChannels )
        {
            পূর্ণসংখ্যা v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_16u( ধ্রুবক ushort* src, ushort* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn )
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( scn == 3 && dcn == 3 )
    {
        পূর্ণসংখ্যা x = 0;
#যদি CV_SIMD_WIDTH > 16
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
        জন্য (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#যদি CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_বাম<1>(_m0l);
        v_float32x4 _m1h = v_rotate_বাম<1>(_m1l);
        v_float32x4 _m2h = v_rotate_বাম<1>(_m2l);
        v_float32x4 _m3h = v_rotate_বাম<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        জন্য( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_ডান<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        জন্য( ; x < len * 3; x += 3 )
        {
            দশমিক v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_32f( ধ্রুবক দশমিক* src, দশমিক* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#যদি (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    পূর্ণসংখ্যা x = 0;
    যদি( scn == 3 && dcn == 3 )
    {
        পূর্ণসংখ্যা idx[VTraits<v_float32>::max_nlanes/2];
        জন্য( পূর্ণসংখ্যা i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        দশমিক _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        জন্য( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        জন্য( ; x < len*3; x += 3 )
        {
            দশমিক v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            দশমিক t0 = saturate_cast<দশমিক>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            দশমিক t1 = saturate_cast<দশমিক>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            দশমিক t2 = saturate_cast<দশমিক>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ফেরত;
    }

    যদি( scn == 4 && dcn == 4 )
    {
#যদি CV_SIMD_WIDTH > 16
        পূর্ণসংখ্যা idx[VTraits<v_float32>::max_nlanes/4];
        জন্য( পূর্ণসংখ্যা i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        দশমিক _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        জন্য( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#যদি CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        জন্য( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#অন্যথা // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        জন্য( ; x < len*4; x += 4 )
        {
            দশমিক v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            দশমিক t0 = saturate_cast<দশমিক>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            দশমিক t1 = saturate_cast<দশমিক>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            দশমিক t2 = saturate_cast<দশমিক>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            দশমিক t3 = saturate_cast<দশমিক>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        ফেরত;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


স্থির শূন্য
transform_8s(ধ্রুবক schar* src, schar* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুবক পূর্ণসংখ্যা BITS = 10, SCALE = 1 << BITS;
    ধ্রুবক দশমিক MAX_M = (দশমিক)(1 << (15 - BITS));

    যদি( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ধ্রুবক পূর্ণসংখ্যা nChannels = 3;

        সংঘ {
            short s[6];
            পূর্ণসংখ্যা p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        পূর্ণসংখ্যা m32[] = {saturate_cast<পূর্ণসংখ্যা>(m[ 2] * SCALE), saturate_cast<পূর্ণসংখ্যা>(m[ 3] * SCALE),
                     saturate_cast<পূর্ণসংখ্যা>(m[ 6] * SCALE), saturate_cast<পূর্ণসংখ্যা>(m[ 7] * SCALE),
                     saturate_cast<পূর্ণসংখ্যা>(m[10] * SCALE), saturate_cast<পূর্ণসংখ্যা>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        পূর্ণসংখ্যা x = 0;
        জন্য (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<পূর্ণসংখ্যা>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<পূর্ণসংখ্যা>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<পূর্ণসংখ্যা>((m[11] + 0.5f)*SCALE);
        জন্য( ; x < len * nChannels; x += nChannels )
        {
            পূর্ণসংখ্যা v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_16s(ধ্রুবক short* src, short* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

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
        জন্য (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        জন্য( ; x < len * 3; x += 3 )
        {
            দশমিক v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_32s(ধ্রুবক পূর্ণসংখ্যা* src, পূর্ণসংখ্যা* dst, ধ্রুবক দ্বিগুণ* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    যদি( scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

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
        জন্য (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_উপরের(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_উপরের(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_উপরের(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        জন্য (; x < len * 3; x += 3) {
            দ্বিগুণ b = src[x], g = src[x + 1], r = src[x + 2];
            পূর্ণসংখ্যা db = saturate_cast<পূর্ণসংখ্যা>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            পূর্ণসংখ্যা dg = saturate_cast<পূর্ণসংখ্যা>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            পূর্ণসংখ্যা dr = saturate_cast<পূর্ণসংখ্যা>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_64f(ধ্রুবক দ্বিগুণ* src, দ্বিগুণ* dst, ধ্রুবক দ্বিগুণ* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    যদি( scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

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
        জন্য (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        জন্য (; x < len * 3; x += 3) {
            দ্বিগুণ b = src[x], g = src[x + 1], r = src[x + 2];
            দ্বিগুণ db = saturate_cast<দ্বিগুণ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            দ্বিগুণ dg = saturate_cast<দ্বিগুণ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            দ্বিগুণ dr = saturate_cast<দ্বিগুণ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> স্থির শূন্য
diagtransform_( ধ্রুবক T* src, T* dst, ধ্রুবক WT* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা )
{
    পূর্ণসংখ্যা x;

    যদি( cn == 2 )
    {
        জন্য( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    অন্যথা যদি( cn == 3 )
    {
        জন্য( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    অন্যথা যদি( cn == 4 )
    {
        জন্য( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    অন্যথা
    {
        জন্য( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ধ্রুবক WT* _m = m;
            জন্য( পূর্ণসংখ্যা j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

স্থির শূন্য
diagtransform_8u(ধ্রুবক uchar* src, uchar* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        জন্য (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        জন্য (; x < len * 3; x += 3) {
            পূর্ণসংখ্যা b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_8s(ধ্রুবক schar* src, schar* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
    #যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        জন্য (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        জন্য (; x < len * 3; x += 3) {
            পূর্ণসংখ্যা b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_16u(ধ্রুবক ushort* src, ushort* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        জন্য (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        জন্য (; x < len * 3; x += 3) {
            পূর্ণসংখ্যা b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_16s(ধ্রুবক short* src, short* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        জন্য (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        জন্য (; x < len * 3; x += 3) {
            পূর্ণসংখ্যা b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_32s(ধ্রুবক পূর্ণসংখ্যা* src, পূর্ণসংখ্যা* dst, ধ্রুবক দ্বিগুণ* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        জন্য (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_উপরের(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_উপরের(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_উপরের(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        জন্য (; x < len * 3; x += 3) {
            পূর্ণসংখ্যা b = src[x], g = src[x + 1], r = src[x + 2];
            পূর্ণসংখ্যা db = saturate_cast<পূর্ণসংখ্যা>(m[ 0] * b + m[ 3]);
            পূর্ণসংখ্যা dg = saturate_cast<পূর্ণসংখ্যা>(m[ 5] * g + m[ 7]);
            পূর্ণসংখ্যা dr = saturate_cast<পূর্ণসংখ্যা>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_32f(ধ্রুবক দশমিক* src, দশমিক* dst, ধ্রুবক দশমিক* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        জন্য (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        জন্য (; x < len * 3; x += 3) {
            দশমিক b = src[x], g = src[x + 1], r = src[x + 2];
            দশমিক db = saturate_cast<দশমিক>(m[ 0] * b + m[ 3]);
            দশমিক dg = saturate_cast<দশমিক>(m[ 5] * g + m[ 7]);
            দশমিক dr = saturate_cast<দশমিক>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_64f(ধ্রুবক দ্বিগুণ* src, দ্বিগুণ* dst, ধ্রুবক দ্বিগুণ* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণসংখ্যা x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        জন্য (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        জন্য (; x < len * 3; x += 3) {
            দ্বিগুণ b = src[x], g = src[x + 1], r = src[x + 2];
            দ্বিগুণ db = saturate_cast<দ্বিগুণ>(m[ 0] * b + m[ 3]);
            দ্বিগুণ dg = saturate_cast<দ্বিগুণ>(m[ 5] * g + m[ 7]);
            দ্বিগুণ dr = saturate_cast<দ্বিগুণ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ফেরত;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, শূন্য (*fn)(ধ্রুবক T*, T*, ধ্রুবক WT*, পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা)>
স্থির শূন্য transformWrap(ধ্রুবক শূন্য* src, শূন্য* dst, ধ্রুবক শূন্য* m, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn)
{
    fn((ধ্রুবক T*)src, (T*)dst, (ধ্রুবক WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(পূর্ণসংখ্যা depth)
{
    স্থির TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, দশমিক, transform_8u>, transformWrap<schar, দশমিক, transform_8s>, transformWrap<ushort, দশমিক, transform_16u>,
        transformWrap<short, দশমিক, transform_16s>, transformWrap<পূর্ণসংখ্যা, দ্বিগুণ, transform_32s>, transformWrap<দশমিক, দশমিক, transform_32f>,
        transformWrap<দ্বিগুণ, দ্বিগুণ, transform_64f>, 0
    };

    ফেরত transformTab[depth];
}

TransformFunc getDiagTransformFunc(পূর্ণসংখ্যা depth)
{
    স্থির TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, দশমিক, diagtransform_8u>, transformWrap<schar, দশমিক, diagtransform_8s>, transformWrap<ushort, দশমিক, diagtransform_16u>,
        transformWrap<short, দশমিক, diagtransform_16s>, transformWrap<পূর্ণসংখ্যা, দ্বিগুণ, diagtransform_32s>, transformWrap<দশমিক, দশমিক, diagtransform_32f>,
        transformWrap<দ্বিগুণ, দ্বিগুণ, diagtransform_64f>, 0
    };

    ফেরত diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> স্থির শূন্য
perspectiveTransform_( ধ্রুবক শূন্য* _src, শূন্য* _dst, ধ্রুবক শূন্য* _m_ptr, পূর্ণসংখ্যা len, পূর্ণসংখ্যা scn, পূর্ণসংখ্যা dcn )
{
    ধ্রুবক T* src = (ধ্রুবক T*)_src;
    T* dst = (T*)_dst;
    ধ্রুবক দ্বিগুণ* m = (ধ্রুবক দ্বিগুণ*)_m_ptr;
    ধ্রুবক দ্বিগুণ eps = FLT_EPSILON;
    পূর্ণসংখ্যা i;

    যদি( scn == 2 && dcn == 2 )
    {
        জন্য( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            দ্বিগুণ w = x*m[6] + y*m[7] + m[8];

            যদি( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            অন্যথা
                dst[i] = dst[i+1] = (T)0;
        }
    }
    অন্যথা যদি( scn == 3 && dcn == 3 )
    {
        জন্য( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            দ্বিগুণ w = x*m[12] + y*m[13] + z*m[14] + m[15];

            যদি( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            অন্যথা
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    অন্যথা যদি( scn == 3 && dcn == 2 )
    {
        জন্য( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            দ্বিগুণ w = x*m[8] + y*m[9] + z*m[10] + m[11];

            যদি( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            অন্যথা
                dst[0] = dst[1] = (T)0;
        }
    }
    অন্যথা
    {
        জন্য( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ধ্রুবক দ্বিগুণ* _m = m + dcn*(scn + 1);
            দ্বিগুণ w = _m[scn];
            পূর্ণসংখ্যা j, k;
            জন্য( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            যদি( fabs(w) > eps )
            {
                _m = m;
                জন্য( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    দ্বিগুণ s = _m[scn];
                    জন্য( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            অন্যথা
                জন্য( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(পূর্ণসংখ্যা depth)
{
    যদি (depth == CV_32F)
        ফেরত perspectiveTransform_<দশমিক>;
    যদি (depth == CV_64F)
        ফেরত perspectiveTransform_<দ্বিগুণ>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

স্থির শূন্য scaleAdd_32f(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, শূন্য* _dst,
                         পূর্ণসংখ্যা len, ধ্রুবক শূন্য* _alpha)
{
    ধ্রুবক দশমিক* src1 = (ধ্রুবক দশমিক*)_src1;
    ধ্রুবক দশমিক* src2 = (ধ্রুবক দশমিক*)_src2;
    দশমিক* dst = (দশমিক*)_dst;
    দশমিক alpha = *(ধ্রুবক দশমিক*)_alpha;
    পূর্ণসংখ্যা i = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ধ্রুবক পূর্ণসংখ্যা cWidth = VTraits<v_float32>::vlanes();
    জন্য (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    জন্য (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


স্থির শূন্য scaleAdd_64f(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, শূন্য* _dst,
                         পূর্ণসংখ্যা len, ধ্রুবক শূন্য* _alpha)
{
    ধ্রুবক দ্বিগুণ* src1 = (ধ্রুবক দ্বিগুণ*)_src1;
    ধ্রুবক দ্বিগুণ* src2 = (ধ্রুবক দ্বিগুণ*)_src2;
    দ্বিগুণ* dst = (দ্বিগুণ*)_dst;
    দ্বিগুণ alpha = *(ধ্রুবক দ্বিগুণ*)_alpha;
    পূর্ণসংখ্যা i = 0;
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ধ্রুবক পূর্ণসংখ্যা cWidth = VTraits<v_float64>::vlanes();
    জন্য (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    জন্য (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(পূর্ণসংখ্যা depth)
{
    যদি (depth == CV_32F)
        ফেরত scaleAdd_32f;
    যদি (depth == CV_64F)
        ফেরত scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> স্থির অন্তঃরেখ
দ্বিগুণ MahalanobisImpl(ধ্রুবক Mat& v1, ধ্রুবক Mat& v2, ধ্রুবক Mat& icovar, দ্বিগুণ *diff_বাফার /*[len]*/, পূর্ণসংখ্যা len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    আকার_2 sz = v1.size();
    দ্বিগুণ ফলাফল = 0;

    sz.width *= v1.channels();
    যদি (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        ধ্রুবক T* src1 = v1.ptr<T>();
        ধ্রুবক T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/আকার(src1[0]);
        size_t step2 = v2.step/আকার(src2[0]);
        দ্বিগুণ* diff = diff_বাফার;
        ধ্রুবক T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/আকার(mat[0]);

        জন্য (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            জন্য (পূর্ণসংখ্যা i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_বাফার;
        জন্য (পূর্ণসংখ্যা i = 0; i < len; i++, mat += matstep)
        {
            দ্বিগুণ row_যোগ = 0;
            পূর্ণসংখ্যা j = 0;
#যদি CV_ENABLE_UNROLLED
            জন্য(; j <= len - 4; j += 4 )
                row_যোগ += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            জন্য (; j < len; j++)
                row_যোগ += diff[j]*mat[j];
            ফলাফল += row_যোগ * diff[i];
        }
    }
    ফেরত ফলাফল;
}

MahalanobisImplFunc getMahalanobisImplFunc(পূর্ণসংখ্যা depth)
{
    যদি (depth == CV_32F)
        ফেরত (MahalanobisImplFunc)MahalanobisImpl<দশমিক>;
    যদি (depth == CV_64F)
        ফেরত (MahalanobisImplFunc)MahalanobisImpl<দ্বিগুণ>;
    CV_Assert(0 && "Not supported");
}


#যদি !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> স্থির শূন্য
MulTransposedR(ধ্রুবক Mat& srcmat, ধ্রুবক Mat& dstmat, ধ্রুবক Mat& deltamat, দ্বিগুণ scale)
{
    পূর্ণসংখ্যা i, j, k;
    ধ্রুবক sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ধ্রুবক dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/আকার(src[0]);
    size_t dststep = dstmat.step/আকার(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/আকার(delta[0]) : 0;
    পূর্ণসংখ্যা delta_cols = deltamat.cols;
    আকার_2 size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    পূর্ণসংখ্যা buf_আকার_2 = size.height*আকার(dT);
    Auto_বাফার<uchar> buf;

    যদি( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_আকার_2 *= 5;
    }
    buf.allocate(buf_আকার_2);
    col_buf = (dT*)buf.তথ্য();

    যদি( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        জন্য( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#যদি CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    যদি( !delta )
        জন্য( i = 0; i < size.width; i++, tdst += dststep )
        {
            জন্য( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            জন্য( j = i; j <= size.width - 4; j += 4 )
            {
#যদি CV_SIMD128_64F
                যদি (তথ্য_Type_2<sT>::depth == CV_64F && তথ্য_Type_2<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ধ্রুবক দ্বিগুণ *tsrc = (দ্বিগুণ*)(src + j);

                    জন্য( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((দ্বিগুণ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((দ্বিগুণ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((দ্বিগুণ*)(tdst+j+2), v_mul(s1, v_scale));
                } অন্যথা
#endif
                {
                    দ্বিগুণ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ধ্রুবক sT *tsrc = src + j;

                    জন্য( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        দ্বিগুণ a = col_buf[k];
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

            জন্য( ; j < size.width; j++ )
            {
                দ্বিগুণ s0 = 0;
                ধ্রুবক sT *tsrc = src + j;

                জন্য( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (দ্বিগুণ)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    অন্যথা
        জন্য( i = 0; i < size.width; i++, tdst += dststep )
        {
            যদি( !delta_buf )
                জন্য( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            অন্যথা
                জন্য( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            জন্য( j = i; j <= size.width - 4; j += 4 )
            {
#যদি CV_SIMD128_64F
                যদি (তথ্য_Type_2<sT>::depth == CV_64F && তথ্য_Type_2<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ধ্রুবক দ্বিগুণ *tsrc = (দ্বিগুণ*)(src + j);
                    ধ্রুবক দ্বিগুণ *d = (দ্বিগুণ*)(delta_buf ? delta_buf : delta + j);

                    জন্য( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((দ্বিগুণ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((দ্বিগুণ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((দ্বিগুণ*)(tdst+j+2), v_mul(s1, v_scale));
                }
                অন্যথা
#endif

                {
                    দ্বিগুণ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ধ্রুবক sT *tsrc = src + j;
                    ধ্রুবক dT *d = delta_buf ? delta_buf : delta + j;

                    জন্য( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        দ্বিগুণ a = col_buf[k];
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

            জন্য( ; j < size.width; j++ )
            {
                দ্বিগুণ s0 = 0;
                ধ্রুবক sT *tsrc = src + j;
                ধ্রুবক dT *d = delta_buf ? delta_buf : delta + j;

                জন্য( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (দ্বিগুণ)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> স্থির শূন্য
MulTransposedL(ধ্রুবক Mat& srcmat, ধ্রুবক Mat& dstmat, ধ্রুবক Mat& deltamat, দ্বিগুণ scale)
{
    পূর্ণসংখ্যা i, j, k;
    ধ্রুবক sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ধ্রুবক dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/আকার(src[0]);
    size_t dststep = dstmat.step/আকার(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/আকার(delta[0]) : 0;
    পূর্ণসংখ্যা delta_cols = deltamat.cols;
    আকার_2 size = srcmat.size();
    dT* tdst = dst;

    যদি( !delta )
        জন্য( i = 0; i < size.height; i++, tdst += dststep )
            জন্য( j = i; j < size.height; j++ )
            {
                দ্বিগুণ s = 0;
                ধ্রুবক sT *tsrc1 = src + i*srcstep;
                ধ্রুবক sT *tsrc2 = src + j*srcstep;
#যদি CV_SIMD128_64F
                যদি (তথ্য_Type_2<sT>::depth == CV_64F && তথ্য_Type_2<dT>::depth == CV_64F)
                {
                    ধ্রুবক দ্বিগুণ *v_tsrc1 = (দ্বিগুণ *)(tsrc1);
                    ধ্রুবক দ্বিগুণ *v_tsrc2 = (দ্বিগুণ *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    জন্য( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_যোগ(v_s);
                }
                অন্যথা
#endif
                {
                    জন্য( k = 0; k <= size.width - 4; k += 4 )
                        s += (দ্বিগুণ)tsrc1[k]*tsrc2[k] + (দ্বিগুণ)tsrc1[k+1]*tsrc2[k+1] +
                             (দ্বিগুণ)tsrc1[k+2]*tsrc2[k+2] + (দ্বিগুণ)tsrc1[k+3]*tsrc2[k+3];
                }
                জন্য( ; k < size.width; k++ )
                    s += (দ্বিগুণ)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    অন্যথা
    {
        dT delta_buf[4];
        পূর্ণসংখ্যা delta_shift = delta_cols == size.width ? 4 : 0;
        Auto_বাফার<uchar> buf(size.width*আকার(dT));
        dT* row_buf = (dT*)buf.তথ্য();

        জন্য( i = 0; i < size.height; i++, tdst += dststep )
        {
            ধ্রুবক sT *tsrc1 = src + i*srcstep;
            ধ্রুবক dT *tdelta1 = delta + i*deltastep;

            যদি( delta_cols < size.width )
                জন্য( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            অন্যথা
                জন্য( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            জন্য( j = i; j < size.height; j++ )
            {
                দ্বিগুণ s = 0;
                ধ্রুবক sT *tsrc2 = src + j*srcstep;
                ধ্রুবক dT *tdelta2 = delta + j*deltastep;
                যদি( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#যদি CV_SIMD128_64F
                যদি (তথ্য_Type_2<sT>::depth == CV_64F && তথ্য_Type_2<dT>::depth == CV_64F)
                {
                    ধ্রুবক দ্বিগুণ *v_tsrc2 = (দ্বিগুণ *)(tsrc2);
                    ধ্রুবক দ্বিগুণ *v_tdelta2 = (দ্বিগুণ *)(tdelta2);
                    ধ্রুবক দ্বিগুণ *v_row_buf = (দ্বিগুণ *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    জন্য( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_যোগ(v_s);

                    tdelta2 = (ধ্রুবক dT *)(v_tdelta2);
                }
                অন্যথা
#endif
                {
                    জন্য( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (দ্বিগুণ)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (দ্বিগুণ)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (দ্বিগুণ)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (দ্বিগুণ)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                জন্য( ; k < size.width; k++, tdelta2++ )
                    s += (দ্বিগুণ)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(পূর্ণসংখ্যা stype, পূর্ণসংখ্যা dtype, যৌক্তিক ata)
{
    MulTransposedFunc func = ফাঁকা;
    যদি (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,দশমিক>
                   : MulTransposedL<uchar,দশমিক>;
    }
    অন্যথা যদি (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,দ্বিগুণ>
                   : MulTransposedL<uchar,দ্বিগুণ>;
    }
    অন্যথা যদি(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,দশমিক>
                   : MulTransposedL<ushort,দশমিক>;
    }
    অন্যথা যদি(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,দ্বিগুণ>
                   : MulTransposedL<ushort,দ্বিগুণ>;
    }
    অন্যথা যদি(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,দশমিক>
                   : MulTransposedL<short,দশমিক>;
    }
    অন্যথা যদি(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,দ্বিগুণ>
                   : MulTransposedL<short,দ্বিগুণ>;
    }
    অন্যথা যদি(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<দশমিক,দশমিক>
                   : MulTransposedL<দশমিক,দশমিক>;
    }
    অন্যথা যদি(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<দশমিক,দ্বিগুণ>
                   : MulTransposedL<দশমিক,দ্বিগুণ>;
    }
    অন্যথা যদি(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<দ্বিগুণ,দ্বিগুণ>
                   : MulTransposedL<দ্বিগুণ,দ্বিগুণ>;
    }
    CV_Assert(func && "Not supported");
    ফেরত func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> স্থির অন্তঃরেখ
দ্বিগুণ dotProd_(ধ্রুবক T* src1, ধ্রুবক T* src2, পূর্ণসংখ্যা len)
{
    পূর্ণসংখ্যা i = 0;
    দ্বিগুণ ফলাফল = 0;

#যদি CV_ENABLE_UNROLLED
    জন্য( ; i <= len - 4; i += 4 )
        ফলাফল += (দ্বিগুণ)src1[i]*src2[i] + (দ্বিগুণ)src1[i+1]*src2[i+1] +
            (দ্বিগুণ)src1[i+2]*src2[i+2] + (দ্বিগুণ)src1[i+3]*src2[i+3];
#endif
    জন্য( ; i < len; i++ )
        ফলাফল += (দ্বিগুণ)src1[i]*src2[i];

    ফেরত ফলাফল;
}


দ্বিগুণ dotProd_8u(ধ্রুবক uchar* src1, ধ্রুবক uchar* src2, পূর্ণসংখ্যা len)
{
    দ্বিগুণ r = 0;
    পূর্ণসংখ্যা i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণসংখ্যা len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), block_আকার;

    যখন (i < len0)
    {
        block_আকার = std::min(len0 - i, blockSize0);
        v_uint32 v_যোগ = vx_setzero_u32();
        ধ্রুবক পূর্ণসংখ্যা cWidth = VTraits<v_uint16>::vlanes();

        পূর্ণসংখ্যা j = 0;
        জন্য (; j <= block_আকার - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_যোগ = v_dotprod_expand_fast(v_src1, v_src2, v_যোগ);
        }

        জন্য (; j <= block_আকার - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_যোগ = v_add(v_যোগ, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (দ্বিগুণ)v_reduce_যোগ(v_যোগ);

        src1 += block_আকার;
        src2 += block_আকার;
        i += block_আকার;
    }
    vx_cleanup();
#endif
    ফেরত r + dotProd_(src1, src2, len - i);
}


দ্বিগুণ dotProd_8s(ধ্রুবক schar* src1, ধ্রুবক schar* src2, পূর্ণসংখ্যা len)
{
    দ্বিগুণ r = 0.0;
    পূর্ণসংখ্যা i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণসংখ্যা len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), block_আকার;

    যখন (i < len0)
    {
        block_আকার = std::min(len0 - i, blockSize0);
        v_int32 v_যোগ = vx_setzero_s32();
        ধ্রুবক পূর্ণসংখ্যা cWidth = VTraits<v_int16>::vlanes();

        পূর্ণসংখ্যা j = 0;
        জন্য (; j <= block_আকার - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_যোগ = v_dotprod_expand_fast(v_src1, v_src2, v_যোগ);
        }

        জন্য (; j <= block_আকার - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_যোগ = v_dotprod_fast(v_src1, v_src2, v_যোগ);
        }
        r += (দ্বিগুণ)v_reduce_যোগ(v_যোগ);

        src1 += block_আকার;
        src2 += block_আকার;
        i += block_আকার;
    }
    vx_cleanup();
#endif

    ফেরত r + dotProd_(src1, src2, len - i);
}

দ্বিগুণ dotProd_16u(ধ্রুবক ushort* src1, ধ্রুবক ushort* src2, পূর্ণসংখ্যা len)
{
    দ্বিগুণ r = 0.0;
    পূর্ণসংখ্যা i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণসংখ্যা len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), block_আকার;

    যখন (i < len0)
    {
        block_আকার = std::min(len0 - i, blockSize0);
        v_uint64 v_যোগ = vx_setzero_u64();
        ধ্রুবক পূর্ণসংখ্যা cWidth = VTraits<v_uint16>::vlanes();

        পূর্ণসংখ্যা j = 0;
        জন্য (; j <= block_আকার - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_যোগ = v_dotprod_expand_fast(v_src1, v_src2, v_যোগ);
        }
        r += (দ্বিগুণ)v_reduce_যোগ(v_যোগ);

        src1 += block_আকার;
        src2 += block_আকার;
        i += block_আকার;
    }
    vx_cleanup();
#endif
    ফেরত r + dotProd_(src1, src2, len - i);
}

দ্বিগুণ dotProd_16s(ধ্রুবক short* src1, ধ্রুবক short* src2, পূর্ণসংখ্যা len)
{
    দ্বিগুণ r = 0.0;
    পূর্ণসংখ্যা i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণসংখ্যা len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), block_আকার;

    যখন (i < len0)
    {
        block_আকার = std::min(len0 - i, blockSize0);
        v_int64 v_যোগ = vx_setzero_s64();
        ধ্রুবক পূর্ণসংখ্যা cWidth = VTraits<v_int16>::vlanes();

        পূর্ণসংখ্যা j = 0;
        জন্য (; j <= block_আকার - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_যোগ = v_dotprod_expand_fast(v_src1, v_src2, v_যোগ);
        }
        r += (দ্বিগুণ)v_reduce_যোগ(v_যোগ);

        src1 += block_আকার;
        src2 += block_আকার;
        i += block_আকার;
    }
    vx_cleanup();
#endif
    ফেরত r + dotProd_(src1, src2, len - i);
}

দ্বিগুণ dotProd_32s(ধ্রুবক পূর্ণসংখ্যা* src1, ধ্রুবক পূর্ণসংখ্যা* src2, পূর্ণসংখ্যা len)
{
#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    দ্বিগুণ r = .0;
    পূর্ণসংখ্যা i = 0;
    ধ্রুবক পূর্ণসংখ্যা step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#যদি CV_SIMD_WIDTH == 16
    ধ্রুবক পূর্ণসংখ্যা wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    জন্য (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    জন্য (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_যোগ(v_sum0);
    vx_cleanup();
    ফেরত r + dotProd_(src1, src2, len - i);
#অন্যথা
    ফেরত dotProd_(src1, src2, len);
#endif
}

দ্বিগুণ dotProd_32f(ধ্রুবক দশমিক* src1, ধ্রুবক দশমিক* src2, পূর্ণসংখ্যা len)
{
    দ্বিগুণ r = 0.0;
    পূর্ণসংখ্যা i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণসংখ্যা len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), block_আকার;

    যখন (i < len0)
    {
        block_আকার = std::min(len0 - i, blockSize0);
        v_float32 v_যোগ = vx_setzero_f32();

        পূর্ণসংখ্যা j = 0;
        পূর্ণসংখ্যা cWidth = VTraits<v_float32>::vlanes();

#যদি CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        জন্য (; j <= block_আকার - (cWidth * 4); j += (cWidth * 4))
        {
            v_যোগ  = v_muladd(vx_load(src1 + j),
                              vx_load(src2 + j), v_যোগ);
            v_sum1 = v_muladd(vx_load(src1 + j + cWidth),
                              vx_load(src2 + j + cWidth), v_sum1);
            v_sum2 = v_muladd(vx_load(src1 + j + (cWidth * 2)),
                              vx_load(src2 + j + (cWidth * 2)), v_sum2);
            v_sum3 = v_muladd(vx_load(src1 + j + (cWidth * 3)),
                              vx_load(src2 + j + (cWidth * 3)), v_sum3);
        }

        v_যোগ = v_add(v_যোগ, v_add(v_add(v_sum1, v_sum2), v_sum3));
#endif

        জন্য (; j <= block_আকার - cWidth; j += cWidth)
            v_যোগ = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_যোগ);

        r += v_reduce_যোগ(v_যোগ);

        src1 += block_আকার;
        src2 += block_আকার;
        i += block_আকার;
    }
    vx_cleanup();
#endif
    ফেরত r + dotProd_(src1, src2, len - i);
}

দ্বিগুণ dotProd_64f(ধ্রুবক দ্বিগুণ* src1, ধ্রুবক দ্বিগুণ* src2, পূর্ণসংখ্যা len)
{
    ফেরত dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
