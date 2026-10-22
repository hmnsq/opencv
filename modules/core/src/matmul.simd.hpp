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

#অন্তর্ভাৱ "precomp.hpp"

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
প্রকারনির্দেশ শূন্য (*TransformFunc)(ধ্রুৱ শূন্য* src, শূন্য* dst, ধ্রুৱ শূন্য* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn);
প্রকারনির্দেশ শূন্য (*ScaleAddFunc)(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, শূন্য* dst, পূর্ণাঙ্ক len, ধ্রুৱ শূন্য* alpha);
প্রকারনির্দেশ শূন্য (*MulTransposedFunc)(ধ্রুৱ Mat& src, ধ্রুৱ/*preallocated*/ Mat& dst, ধ্রুৱ Mat& delta, দ্ৱিগুণ scale);
প্রকারনির্দেশ দ্ৱিগুণ (*MahalanobisImplFunc)(ধ্রুৱ Mat& v1, ধ্রুৱ Mat& v2, ধ্রুৱ Mat& icovar, দ্ৱিগুণ *diff_buffer /*[len]*/, পূর্ণাঙ্ক len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

শূন্য gemm32f(ধ্রুৱ ভিন্ন* src1, size_t src1_step, ধ্রুৱ ভিন্ন* src2, size_t src2_step,
             ভিন্ন alpha, ধ্রুৱ ভিন্ন* src3, size_t src3_step, ভিন্ন beta, ভিন্ন* dst, size_t dst_step,
             পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags);
শূন্য gemm64f(ধ্রুৱ দ্ৱিগুণ* src1, size_t src1_step, ধ্রুৱ দ্ৱিগুণ* src2, size_t src2_step,
             দ্ৱিগুণ alpha, ধ্রুৱ দ্ৱিগুণ* src3, size_t src3_step, দ্ৱিগুণ beta, দ্ৱিগুণ* dst, size_t dst_step,
             পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags);
শূন্য gemm32fc(ধ্রুৱ ভিন্ন* src1, size_t src1_step, ধ্রুৱ ভিন্ন* src2, size_t src2_step,
              ভিন্ন alpha, ধ্রুৱ ভিন্ন* src3, size_t src3_step, ভিন্ন beta, ভিন্ন* dst, size_t dst_step,
              পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags);
শূন্য gemm64fc(ধ্রুৱ দ্ৱিগুণ* src1, size_t src1_step, ধ্রুৱ দ্ৱিগুণ* src2, size_t src2_step,
              দ্ৱিগুণ alpha, ধ্রুৱ দ্ৱিগুণ* src3, size_t src3_step, দ্ৱিগুণ beta, দ্ৱিগুণ* dst, size_t dst_step,
              পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags);

TransformFunc getTransformFunc(পূর্ণাঙ্ক depth);
TransformFunc getDiagTransformFunc(পূর্ণাঙ্ক depth);
TransformFunc getPerspectiveTransform(পূর্ণাঙ্ক depth);

ScaleAddFunc getScaleAddFunc(পূর্ণাঙ্ক depth);

MahalanobisImplFunc getMahalanobisImplFunc(পূর্ণাঙ্ক depth);

MulTransposedFunc getMulTransposedFunc(পূর্ণাঙ্ক stype, পূর্ণাঙ্ক dtype, তার্কিক ata);

দ্ৱিগুণ dotProd_8u(ধ্রুৱ uchar* src1, ধ্রুৱ uchar* src2, পূর্ণাঙ্ক len);
দ্ৱিগুণ dotProd_8s(ধ্রুৱ schar* src1, ধ্রুৱ schar* src2, পূর্ণাঙ্ক len);
দ্ৱিগুণ dotProd_16u(ধ্রুৱ ushort* src1, ধ্রুৱ ushort* src2, পূর্ণাঙ্ক len);
দ্ৱিগুণ dotProd_16s(ধ্রুৱ short* src1, ধ্রুৱ short* src2, পূর্ণাঙ্ক len);
দ্ৱিগুণ dotProd_32s(ধ্রুৱ পূর্ণাঙ্ক* src1, ধ্রুৱ পূর্ণাঙ্ক* src2, পূর্ণাঙ্ক len);
দ্ৱিগুণ dotProd_32f(ধ্রুৱ ভিন্ন* src1, ধ্রুৱ ভিন্ন* src2, পূর্ণাঙ্ক len);
দ্ৱিগুণ dotProd_64f(ধ্রুৱ দ্ৱিগুণ* src1, ধ্রুৱ দ্ৱিগুণ* src2, পূর্ণাঙ্ক len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#যদি !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

স্থির শূন্য
GEMM_CopyBlock( ধ্রুৱ uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    পূর্ণাঙ্ক j;
    size.width *= (পূর্ণাঙ্ক)(pix_size / পরিমাণ(পূর্ণাঙ্ক));

    বাবে( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #যদি CV_ENABLE_UNROLLED
        বাবে( ; j <= size.width - 4; j += 4 )
        {
            পূর্ণাঙ্ক t0 = ((ধ্রুৱ পূর্ণাঙ্ক*)src)[j];
            পূর্ণাঙ্ক t1 = ((ধ্রুৱ পূর্ণাঙ্ক*)src)[j+1];
            ((পূর্ণাঙ্ক*)dst)[j] = t0;
            ((পূর্ণাঙ্ক*)dst)[j+1] = t1;
            t0 = ((ধ্রুৱ পূর্ণাঙ্ক*)src)[j+2];
            t1 = ((ধ্রুৱ পূর্ণাঙ্ক*)src)[j+3];
            ((পূর্ণাঙ্ক*)dst)[j+2] = t0;
            ((পূর্ণাঙ্ক*)dst)[j+3] = t1;
        }
        #endif
        বাবে( ; j < size.width; j++ )
            ((পূর্ণাঙ্ক*)dst)[j] = ((ধ্রুৱ পূর্ণাঙ্ক*)src)[j];
    }
}


স্থির শূন্য
GEMM_TransposeBlock( ধ্রুৱ uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    পূর্ণাঙ্ক i, j;
    বাবে( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        ধ্রুৱ uchar* _src = src;
        ৱিকল্প( pix_size )
        {
        পক্ষ পরিমাণ(পূর্ণাঙ্ক):
            বাবে( j = 0; j < size.height; j++, _src += src_step )
                ((পূর্ণাঙ্ক*)dst)[j] = ((পূর্ণাঙ্ক*)_src)[0];
            ভঙ্গ;
        পক্ষ পরিমাণ(পূর্ণাঙ্ক)*2:
            বাবে( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                পূর্ণাঙ্ক t0 = ((পূর্ণাঙ্ক*)_src)[0];
                পূর্ণাঙ্ক t1 = ((পূর্ণাঙ্ক*)_src)[1];
                ((পূর্ণাঙ্ক*)dst)[j] = t0;
                ((পূর্ণাঙ্ক*)dst)[j+1] = t1;
            }
            ভঙ্গ;
        পক্ষ পরিমাণ(পূর্ণাঙ্ক)*4:
            বাবে( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                পূর্ণাঙ্ক t0 = ((পূর্ণাঙ্ক*)_src)[0];
                পূর্ণাঙ্ক t1 = ((পূর্ণাঙ্ক*)_src)[1];
                ((পূর্ণাঙ্ক*)dst)[j] = t0;
                ((পূর্ণাঙ্ক*)dst)[j+1] = t1;
                t0 = ((পূর্ণাঙ্ক*)_src)[2];
                t1 = ((পূর্ণাঙ্ক*)_src)[3];
                ((পূর্ণাঙ্ক*)dst)[j+2] = t0;
                ((পূর্ণাঙ্ক*)dst)[j+3] = t1;
            }
            ভঙ্গ;
        স্ৱভাৱ:
            CV_Assert(0);
            ঘূৰাই;
        }
    }
}

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> সংরচনা VecTraits;

template<> সংরচনা VecTraits<ভিন্ন> {
    প্রকারনির্দেশ v_float32 VT;
    স্থির অন্তঃস্থ VT zero() { ঘূৰাই vx_setzero_f32(); }
    স্থির অন্তঃস্থ VT load(ধ্রুৱ ভিন্ন* p) { ঘূৰাই vx_load(p); }
    স্থির অন্তঃস্থ VT setall(ভিন্ন v) { ঘূৰাই vx_setall_f32(v); }
};
#endif

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> সংরচনা VecTraits<দ্ৱিগুণ> {
    প্রকারনির্দেশ v_float64 VT;
    স্থির অন্তঃস্থ VT zero() { ঘূৰাই vx_setzero_f64(); }
    স্থির অন্তঃস্থ VT load(ধ্রুৱ দ্ৱিগুণ* p) { ঘূৰাই vx_load(p); }
    স্থির অন্তঃস্থ VT setall(দ্ৱিগুণ v) { ঘূৰাই vx_setall_f64(v); }
};
#endif

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
স্থির অন্তঃস্থ ST simdDotProduct(ধ্রুৱ ST* a, ধ্রুৱ ST* b, পূর্ণাঙ্ক n, পূর্ণাঙ্ক& k)
{
    প্রকারনির্দেশ typename VecTraits<ST>::VT VecT;
    ধ্রুৱ পূর্ণাঙ্ক vlanes = VTraits<VecT>::vlanes();
    ধ্রুৱ পূর্ণাঙ্ক stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    বাবে( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    বাবে( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    ঘূৰাই v_reduce_sum(vs0);
}
#endif

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
স্থির অন্তঃস্থ দ্ৱিগুণ simdDotProduct_f32f64(ধ্রুৱ ভিন্ন* a, ধ্রুৱ ভিন্ন* b, পূর্ণাঙ্ক n, পূর্ণাঙ্ক& k)
{
    ধ্রুৱ পূর্ণাঙ্ক vlanes_f32 = VTraits<v_float32>::vlanes();
    ধ্রুৱ পূর্ণাঙ্ক stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    বাবে( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_উপৰেৰ(va0), v_cvt_f64_উপৰেৰ(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_উপৰেৰ(va1), v_cvt_f64_উপৰেৰ(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    বাবে( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_উপৰেৰ(va), v_cvt_f64_উপৰেৰ(vb), vs0);
    }
    ঘূৰাই v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
স্থির অন্তঃস্থ শূন্য simdGEMM_kj_f32f64(
    ধ্রুৱ ভিন্ন* a_data, ধ্রুৱ ভিন্ন* _b_data, size_t b_step,
    ধ্রুৱ ভিন্ন* _c_data, size_t c_step1,
    ভিন্ন* d_data,
    পূর্ণাঙ্ক n, পূর্ণাঙ্ক m,
    দ্ৱিগুণ alpha, দ্ৱিগুণ beta,
    পূর্ণাঙ্ক& j)
{
    ধ্রুৱ পূর্ণাঙ্ক vlanes_f32 = VTraits<v_float32>::vlanes();
    ধ্রুৱ পূর্ণাঙ্ক vlanes_f64 = VTraits<v_float64>::vlanes();

    যদি( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ধ্রুৱ পূর্ণাঙ্ক max_m = 1600 / (পূর্ণাঙ্ক)পরিমাণ(ভিন্ন);
        cv::AutoBuffer<দ্ৱিগুণ> _s_buf(max_m);
        দ্ৱিগুণ* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            বাবে( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            বাবে( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ধ্রুৱ ভিন্ন* b_ptr = _b_data;
            বাবে( পূর্ণাঙ্ক k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((দ্ৱিগুণ)a_data[k]);
                j = 0;
                বাবে( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_উপৰেৰ(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                দ্ৱিগুণ a_val = (দ্ৱিগুণ)a_data[k];
                বাবে( ; j < m; j++ )
                    s_buf[j] += a_val * (দ্ৱিগুণ)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ধ্রুৱ ভিন্ন* c_data = _c_data;
            j = 0;
            যদি( !_c_data )
            {
                বাবে( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                বাবে( ; j < m; j++ )
                    d_data[j] = (ভিন্ন)(s_buf[j] * alpha);
            }
            অন্যথা যদি( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                বাবে( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_উপৰেৰ(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                বাবে( ; j < m; j++ )
                    d_data[j] = (ভিন্ন)(s_buf[j] * alpha + (দ্ৱিগুণ)c_data[j] * beta);
            }
            অন্যথা
            {
                বাবে( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (ভিন্ন)(s_buf[j] * alpha + (দ্ৱিগুণ)c_data[0] * beta);
            }
        }
        j = m;
    }
    অন্যথা যদি( vlanes_f32 * (পূর্ণাঙ্ক)পরিমাণ(ভিন্ন) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ধ্রুৱ ভিন্ন* c_data = _c_data;
        বাবে( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            ধ্রুৱ ভিন্ন* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            বাবে( পূর্ণাঙ্ক k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((দ্ৱিগুণ)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_উপৰেৰ(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            যদি( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            অন্যথা যদি( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_উপৰেৰ(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            অন্যথা
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ভিন্ন buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                বাবে( পূর্ণাঙ্ক jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ভিন্ন)beta;
            }
        }
    }
}
#endif

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
স্থির অন্তঃস্থ শূন্য simdGEMM_kj(
    ধ্রুৱ ST* a_data, ধ্রুৱ ST* _b_data, size_t b_step,
    ধ্রুৱ ST* _c_data, size_t c_step1,
    ST* d_data,
    পূর্ণাঙ্ক n, পূর্ণাঙ্ক m,
    দ্ৱিগুণ alpha, দ্ৱিগুণ beta,
    পূর্ণাঙ্ক& j)
{
    প্রকারনির্দেশ typename VecTraits<ST>::VT VecT;
    ধ্রুৱ পূর্ণাঙ্ক vlanes = VTraits<VecT>::vlanes();

    যদি( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ধ্রুৱ পূর্ণাঙ্ক max_m = 1600 / (পূর্ণাঙ্ক)পরিমাণ(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            বাবে( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            বাবে( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ধ্রুৱ ST* b_ptr = _b_data;
            বাবে( পূর্ণাঙ্ক k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                বাবে( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                বাবে( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ধ্রুৱ ST* c_data = _c_data;
            j = 0;
            যদি( !_c_data )
            {
                বাবে( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                বাবে( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            অন্যথা যদি( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                বাবে( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                বাবে( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            অন্যথা
            {
                বাবে( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    অন্যথা যদি( vlanes * (পূর্ণাঙ্ক)পরিমাণ(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ধ্রুৱ ST* c_data = _c_data;
        বাবে( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            ধ্রুৱ ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            বাবে( পূর্ণাঙ্ক k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            যদি( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            অন্যথা যদি( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            অন্যথা
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                বাবে( পূর্ণাঙ্ক jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> স্থির শূন্য
GEMMSingleMul( ধ্রুৱ T* a_data, size_t a_step,
               ধ্রুৱ T* b_data, size_t b_step,
               ধ্রুৱ T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    পূর্ণাঙ্ক i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    ধ্রুৱ T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= পরিমাণ(a_data[0]);
    b_step /= পরিমাণ(b_data[0]);
    c_step /= পরিমাণ(c_data[0]);
    d_step /= পরিমাণ(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    যদি( !c_data )
        c_step0 = c_step1 = 0;
    অন্যথা যদি( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    অন্যথা
        c_step0 = 1, c_step1 = c_step;

    যদি( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        যদি( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    যদি( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        যদি( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            বাবে( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        যদি( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            বাবে( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        বাবে( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            বাবে( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                যদি( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                অন্যথা
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            বাবে( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                যদি( !c_data )
                    d_data[j] = T(s0);
                অন্যথা
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    অন্যথা যদি( flags & GEMM_2_T ) /* A * Bt */
    {
        বাবে( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            যদি( a_buf )
            {
                বাবে( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            বাবে( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#যদি CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                যদি( পরিমাণ(WT) == পরিমাণ(দ্ৱিগুণ) )
                {
                    যদি( পরিমাণ(T) == পরিমাণ(দ্ৱিগুণ) )
                        s0 = (WT)simdDotProduct((ধ্রুৱ দ্ৱিগুণ*)a_data, (ধ্রুৱ দ্ৱিগুণ*)b_data, n, k);
                    অন্যথা যদি( পরিমাণ(T) == পরিমাণ(ভিন্ন) )
                        s0 = (WT)simdDotProduct_f32f64((ধ্রুৱ ভিন্ন*)a_data, (ধ্রুৱ ভিন্ন*)b_data, n, k);
                }
                অন্যথা
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #যদি CV_ENABLE_UNROLLED
                    বাবে( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                বাবে( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                যদি( !c_data )
                    d_data[j] = T(s0);
                অন্যথা
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    অন্যথা যদি( d_size.width*পরিমাণ(d_data[0]) <= 1600 )
    {
        বাবে( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            যদি( a_buf )
            {
                বাবে( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            যদি( পরিমাণ(WT) == পরিমাণ(দ্ৱিগুণ) )
            {
                যদি( পরিমাণ(T) == পরিমাণ(দ্ৱিগুণ) )
                    simdGEMM_kj((ধ্রুৱ দ্ৱিগুণ*)a_data, (ধ্রুৱ দ্ৱিগুণ*)_b_data, b_step,
                                        (ধ্রুৱ দ্ৱিগুণ*)_c_data, c_step1,
                                (দ্ৱিগুণ*)d_data, n, m, alpha, beta, j);
                অন্যথা যদি( পরিমাণ(T) == পরিমাণ(ভিন্ন) )
                    simdGEMM_kj_f32f64((ধ্রুৱ ভিন্ন*)a_data, (ধ্রুৱ ভিন্ন*)_b_data, b_step,
                                        (ধ্রুৱ ভিন্ন*)_c_data, c_step1,
                                (ভিন্ন*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            বাবে( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                ধ্রুৱ T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                বাবে( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                যদি( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                অন্যথা
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            বাবে( ; j < m; j++, c_data += c_step1 )
            {
                ধ্রুৱ T* b = _b_data + j;
                WT s0(0);

                বাবে( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                যদি( !c_data )
                    d_data[j] = T(s0);
                অন্যথা
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    অন্যথা
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        বাবে( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            যদি( a_buf )
            {
                বাবে( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            বাবে( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            বাবে( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                যদি( পরিমাণ(WT) == পরিমাণ(দ্ৱিগুণ) )
                {
                    যদি( পরিমাণ(T) == পরিমাণ(দ্ৱিগুণ) )
                    {
                        ধ্রুৱ পূর্ণাঙ্ক vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ধ্রুৱ দ্ৱিগুণ*)&al));
                        বাবে( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((ধ্রুৱ দ্ৱিগুণ*)(d_buf + j));
                            v_float64 vb = vx_load((ধ্রুৱ দ্ৱিগুণ*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((দ্ৱিগুণ*)(d_buf + j), vd);
                        }
                    }
                    অন্যথা
                    {
                        ধ্রুৱ পূর্ণাঙ্ক vlanes_f32 = VTraits<v_float32>::vlanes();
                        ধ্রুৱ পূর্ণাঙ্ক vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ধ্রুৱ দ্ৱিগুণ*)&al));
                        বাবে( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((ধ্রুৱ ভিন্ন*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_উপৰেৰ(vb);
                            v_float64 vd_lo = vx_load((ধ্রুৱ দ্ৱিগুণ*)(d_buf + j));
                            v_float64 vd_hi = vx_load((ধ্রুৱ দ্ৱিগুণ*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((দ্ৱিগুণ*)(d_buf + j), vd_lo);
                            v_store((দ্ৱিগুণ*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                বাবে( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            যদি( !c_data )
                বাবে( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            অন্যথা
                বাবে( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
স্থির অন্তঃস্থ শূন্য setDoubleDataZero(WT* d_data, পূর্ণাঙ্ক m, পূর্ণাঙ্ক vlanes)
{
    v_float64 vz = vx_setzero_f64();
    পূর্ণাঙ্ক j = 0;
    বাবে( ; j <= m - vlanes; j += vlanes )
        v_store((দ্ৱিগুণ*)(d_data + j), vz);
    বাবে( ; j < m; j++ )
        d_data[j] = WT(0);
}

template<typename ST>
স্থির অন্তঃস্থ শূন্য simdMulAdd(ধ্রুৱ ST* b, দ্ৱিগুণ* d, পূর্ণাঙ্ক m, দ্ৱিগুণ a_val);

template<>
অন্তঃস্থ শূন্য simdMulAdd<ভিন্ন>(
    ধ্রুৱ ভিন্ন* b, দ্ৱিগুণ* d, পূর্ণাঙ্ক m, দ্ৱিগুণ a_val)
{
    ধ্রুৱ পূর্ণাঙ্ক vlanes_f32 = VTraits<v_float32>::vlanes();
    ধ্রুৱ পূর্ণাঙ্ক vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    পূর্ণাঙ্ক j = 0;
    বাবে( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_উপৰেৰ(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    বাবে( ; j < m; j++ )
        d[j] += a_val * (দ্ৱিগুণ)b[j];
}

template<>
অন্তঃস্থ শূন্য simdMulAdd<দ্ৱিগুণ>(
    ধ্রুৱ দ্ৱিগুণ* b, দ্ৱিগুণ* d, পূর্ণাঙ্ক m, দ্ৱিগুণ a_val)
{
    ধ্রুৱ পূর্ণাঙ্ক vlanes = VTraits<v_float64>::vlanes();
    ধ্রুৱ পূর্ণাঙ্ক stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    পূর্ণাঙ্ক j = 0;
    বাবে( ; j <= m - stride2; j += stride2 )
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
    বাবে( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    বাবে( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
স্থির অন্তঃস্থ শূন্য simdBlockMul_kj(
    ধ্রুৱ ST* a_data, ধ্রুৱ ST* b_data, size_t b_step,
    দ্ৱিগুণ* d_data, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, তার্কিক do_acc)
{
    ধ্রুৱ পূর্ণাঙ্ক vlanes = VTraits<v_float64>::vlanes();
    যদি( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    বাবে( পূর্ণাঙ্ক k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (দ্ৱিগুণ)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> স্থির শূন্য
GEMMBlockMul( ধ্রুৱ T* a_data, size_t a_step,
              ধ্রুৱ T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, পূর্ণাঙ্ক flags )
{
    পূর্ণাঙ্ক i, j, k, n = a_size.width, m = d_size.width;
    ধ্রুৱ T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    পূর্ণাঙ্ক do_acc = flags & 16;

    a_step /= পরিমাণ(a_data[0]);
    b_step /= পরিমাণ(b_data[0]);
    d_step /= পরিমাণ(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    যদি( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    যদি( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        বাবে( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            যদি( a_buf )
            {
                বাবে( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            বাবে( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#যদি CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                যদি( পরিমাণ(WT) == পরিমাণ(দ্ৱিগুণ) )
                {
                    যদি( পরিমাণ(T) == পরিমাণ(দ্ৱিগুণ) )
                        s0 += (WT)simdDotProduct((ধ্রুৱ দ্ৱিগুণ*)a_data, (ধ্রুৱ দ্ৱিগুণ*)b_data, n, k);
                    অন্যথা যদি( পরিমাণ(T) == পরিমাণ(ভিন্ন) )
                        s0 += (WT)simdDotProduct_f32f64((ধ্রুৱ ভিন্ন*)a_data, (ধ্রুৱ ভিন্ন*)b_data, n, k);
                }
                অন্যথা
#endif
                {
                    WT s1(0);
                    বাবে( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                বাবে( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    অন্যথা
    {
        বাবে( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            যদি( a_buf )
            {
                বাবে( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            যদি( পরিমাণ(WT) == পরিমাণ(দ্ৱিগুণ) )
            {
                যদি( পরিমাণ(T) == পরিমাণ(দ্ৱিগুণ) )
                    simdBlockMul_kj(
                        (ধ্রুৱ দ্ৱিগুণ*)a_data, (ধ্রুৱ দ্ৱিগুণ*)b_data, b_step,
                        (দ্ৱিগুণ*)d_data, n, m, do_acc != 0);
                অন্যথা
                    simdBlockMul_kj(
                        (ধ্রুৱ ভিন্ন*)a_data, (ধ্রুৱ ভিন্ন*)b_data, b_step,
                        (দ্ৱিগুণ*)d_data, n, m, do_acc != 0);
            }
            অন্যথা
#endif
            {
                বাবে( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ধ্রুৱ T* b = b_data + j;

                    যদি( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    অন্যথা
                        s0 = s1 = s2 = s3 = WT(0);

                    বাবে( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                বাবে( ; j < m; j++ )
                {
                    ধ্রুৱ T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    বাবে( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> স্থির শূন্য
GEMMStore( ধ্রুৱ T* c_data, size_t c_step,
           ধ্রুৱ WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    ধ্রুৱ T* _c_data = c_data;
    পূর্ণাঙ্ক j;
    size_t c_step0, c_step1;

    c_step /= পরিমাণ(c_data[0]);
    d_buf_step /= পরিমাণ(d_buf[0]);
    d_step /= পরিমাণ(d_data[0]);

    যদি( !c_data )
        c_step0 = c_step1 = 0;
    অন্যথা যদি( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    অন্যথা
        c_step0 = 1, c_step1 = c_step;

    বাবে( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        যদি( _c_data )
        {
            c_data = _c_data;
            j=0;
             #যদি CV_ENABLE_UNROLLED
            বাবে(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
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
            বাবে( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        অন্যথা
        {
            j = 0;
             #যদি CV_ENABLE_UNROLLED
            বাবে( ; j <= d_size.width - 4; j += 4 )
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
            বাবে( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


প্রকারনির্দেশ শূন্য (*GEMMSingleMulFunc)( ধ্রুৱ শূন্য* src1, size_t step1,
                   ধ্রুৱ শূন্য* src2, size_t step2, ধ্রুৱ শূন্য* src3, size_t step3,
                   শূন্য* dst, size_t dststep, Size srcsize, Size dstsize,
                   দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags );

প্রকারনির্দেশ শূন্য (*GEMMBlockMulFunc)( ধ্রুৱ শূন্য* src1, size_t step1,
                   ধ্রুৱ শূন্য* src2, size_t step2, শূন্য* dst, size_t dststep,
                   Size srcsize, Size dstsize, পূর্ণাঙ্ক flags );

প্রকারনির্দেশ শূন্য (*GEMMStoreFunc)( ধ্রুৱ শূন্য* src1, size_t step1,
                   ধ্রুৱ শূন্য* src2, size_t step2, শূন্য* dst, size_t dststep,
                   Size dstsize, দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags );

template<typename T, typename WT, শূন্য (*fn)(ধ্রুৱ T*, size_t, ধ্রুৱ T*, size_t, ধ্রুৱ T*, size_t, T*, size_t, Size, Size, দ্ৱিগুণ, দ্ৱিগুণ, পূর্ণাঙ্ক)>
স্থির শূন্য gemmSingleMulWrap( ধ্রুৱ শূন্য* src1, size_t step1,
                   ধ্রুৱ শূন্য* src2, size_t step2, ধ্রুৱ শূন্য* src3, size_t step3,
                   শূন্য* dst, size_t dststep, Size srcsize, Size dstsize,
                   দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    fn((ধ্রুৱ T*)src1, step1, (ধ্রুৱ T*)src2, step2, (ধ্রুৱ T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, শূন্য (*fn)(ধ্রুৱ T*, size_t, ধ্রুৱ T*, size_t, WT*, size_t, Size, Size, পূর্ণাঙ্ক)>
স্থির শূন্য gemmBlockMulWrap( ধ্রুৱ শূন্য* src1, size_t step1,
                   ধ্রুৱ শূন্য* src2, size_t step2, শূন্য* dst, size_t dststep,
                   Size srcsize, Size dstsize, পূর্ণাঙ্ক flags )
{
    fn((ধ্রুৱ T*)src1, step1, (ধ্রুৱ T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, শূন্য (*fn)(ধ্রুৱ T*, size_t, ধ্রুৱ WT*, size_t, T*, size_t, Size, দ্ৱিগুণ, দ্ৱিগুণ, পূর্ণাঙ্ক)>
স্থির শূন্য gemmStoreWrap( ধ্রুৱ শূন্য* src1, size_t step1,
                   ধ্রুৱ শূন্য* src2, size_t step2, শূন্য* dst, size_t dststep,
                   Size dstsize, দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    fn((ধ্রুৱ T*)src1, step1, (ধ্রুৱ WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

স্থির শূন্য GEMMSingleMul_32f( ধ্রুৱ ভিন্ন* a_data, size_t a_step,
              ধ্রুৱ ভিন্ন* b_data, size_t b_step,
              ধ্রুৱ ভিন্ন* c_data, size_t c_step,
              ভিন্ন* d_data, size_t d_step,
              Size a_size, Size d_size,
              দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    GEMMSingleMul<ভিন্ন,দ্ৱিগুণ>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

স্থির শূন্য GEMMSingleMul_64f( ধ্রুৱ দ্ৱিগুণ* a_data, size_t a_step,
                              ধ্রুৱ দ্ৱিগুণ* b_data, size_t b_step,
                              ধ্রুৱ দ্ৱিগুণ* c_data, size_t c_step,
                              দ্ৱিগুণ* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    GEMMSingleMul<দ্ৱিগুণ,দ্ৱিগুণ>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


স্থির শূন্য GEMMSingleMul_32fc( ধ্রুৱ Complexf* a_data, size_t a_step,
                              ধ্রুৱ Complexf* b_data, size_t b_step,
                              ধ্রুৱ Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

স্থির শূন্য GEMMSingleMul_64fc( ধ্রুৱ Complexd* a_data, size_t a_step,
                              ধ্রুৱ Complexd* b_data, size_t b_step,
                              ধ্রুৱ Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

স্থির শূন্য GEMMBlockMul_32f( ধ্রুৱ ভিন্ন* a_data, size_t a_step,
             ধ্রুৱ ভিন্ন* b_data, size_t b_step,
             দ্ৱিগুণ* d_data, size_t d_step,
             Size a_size, Size d_size, পূর্ণাঙ্ক flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


স্থির শূন্য GEMMBlockMul_64f( ধ্রুৱ দ্ৱিগুণ* a_data, size_t a_step,
                             ধ্রুৱ দ্ৱিগুণ* b_data, size_t b_step,
                             দ্ৱিগুণ* d_data, size_t d_step,
                             Size a_size, Size d_size, পূর্ণাঙ্ক flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


স্থির শূন্য GEMMBlockMul_32fc( ধ্রুৱ Complexf* a_data, size_t a_step,
                             ধ্রুৱ Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, পূর্ণাঙ্ক flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


স্থির শূন্য GEMMBlockMul_64fc( ধ্রুৱ Complexd* a_data, size_t a_step,
                             ধ্রুৱ Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, পূর্ণাঙ্ক flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


স্থির শূন্য GEMMStore_32f( ধ্রুৱ ভিন্ন* c_data, size_t c_step,
          ধ্রুৱ দ্ৱিগুণ* d_buf, size_t d_buf_step,
          ভিন্ন* d_data, size_t d_step, Size d_size,
          দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


স্থির শূন্য GEMMStore_64f( ধ্রুৱ দ্ৱিগুণ* c_data, size_t c_step,
                      ধ্রুৱ দ্ৱিগুণ* d_buf, size_t d_buf_step,
                      দ্ৱিগুণ* d_data, size_t d_step, Size d_size,
                      দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


স্থির শূন্য GEMMStore_32fc( ধ্রুৱ Complexf* c_data, size_t c_step,
                          ধ্রুৱ Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


স্থির শূন্য GEMMStore_64fc( ধ্রুৱ Complexd* c_data, size_t c_step,
                          ধ্রুৱ Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          দ্ৱিগুণ alpha, দ্ৱিগুণ beta, পূর্ণাঙ্ক flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

স্থির শূন্য gemmImpl( Mat A, Mat B, দ্ৱিগুণ alpha,
           Mat C, দ্ৱিগুণ beta, Mat D, পূর্ণাঙ্ক flags )
{
    CV_INSTRUMENT_REGION();

    ধ্রুৱ পূর্ণাঙ্ক block_lin_size = 128;
    ধ্রুৱ পূর্ণাঙ্ক block_size = block_lin_size * block_lin_size;

    স্থির দ্ৱিগুণ zero[] = {0,0,0,0};
    স্থির ভিন্ন zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    পূর্ণাঙ্ক i, len = 0, type = A.type();

    ৱিকল্প( flags & (GEMM_1_T|GEMM_2_T) )
    {
    পক্ষ 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        ভঙ্গ;
    পক্ষ 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        ভঙ্গ;
    পক্ষ 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        ভঙ্গ;
    পক্ষ 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        ভঙ্গ;
    }

    যদি( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        যদি( type == CV_32F )
        {
            ভিন্ন* d = D.ptr<ভিন্ন>();
            ধ্রুৱ ভিন্ন *a = A.ptr<ভিন্ন>(),
                        *b = B.ptr<ভিন্ন>(),
                        *c = (ধ্রুৱ ভিন্ন*)C.data;
            size_t d_step = D.step/পরিমাণ(d[0]),
                a_step = A.step/পরিমাণ(a[0]),
                b_step = B.step/পরিমাণ(b[0]),
                c_step = C.data ? C.step/পরিমাণ(c[0]) : 0;

            যদি( !c )
                c = zerof;

            ৱিকল্প( len )
            {
            পক্ষ 2:
                যদি( len == d_size.width && b != d )
                {
                    বাবে( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ভিন্ন t0 = a[0]*b[0] + a[1]*b[b_step];
                        ভিন্ন t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (ভিন্ন)(t0*alpha + c[0]*beta);
                        d[1] = (ভিন্ন)(t1*alpha + c[1]*beta);
                    }
                }
                অন্যথা যদি( a != d )
                {
                    পূর্ণাঙ্ক c_step0 = 1;
                    যদি( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    বাবে( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ভিন্ন t0 = a[0]*b[0] + a[1]*b[b_step];
                        ভিন্ন t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (ভিন্ন)(t0*alpha + c[0]*beta);
                        d[d_step] = (ভিন্ন)(t1*alpha + c[c_step]*beta);
                    }
                }
                অন্যথা
                    ভঙ্গ;
                ঘূৰাই;
            পক্ষ 3:
                যদি( len == d_size.width && b != d )
                {
                    বাবে( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ভিন্ন t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ভিন্ন t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        ভিন্ন t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (ভিন্ন)(t0*alpha + c[0]*beta);
                        d[1] = (ভিন্ন)(t1*alpha + c[1]*beta);
                        d[2] = (ভিন্ন)(t2*alpha + c[2]*beta);
                    }
                }
                অন্যথা যদি( a != d )
                {
                    পূর্ণাঙ্ক c_step0 = 1;
                    যদি( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    বাবে( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ভিন্ন t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ভিন্ন t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        ভিন্ন t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (ভিন্ন)(t0*alpha + c[0]*beta);
                        d[d_step] = (ভিন্ন)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (ভিন্ন)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                অন্যথা
                    ভঙ্গ;
                ঘূৰাই;
            পক্ষ 4:
                যদি( len == d_size.width && b != d )
                {
                    বাবে( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ভিন্ন t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ভিন্ন t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        ভিন্ন t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        ভিন্ন t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (ভিন্ন)(t0*alpha + c[0]*beta);
                        d[1] = (ভিন্ন)(t1*alpha + c[1]*beta);
                        d[2] = (ভিন্ন)(t2*alpha + c[2]*beta);
                        d[3] = (ভিন্ন)(t3*alpha + c[3]*beta);
                    }
                }
                অন্যথা যদি( len <= 16 && a != d )
                {
                    পূর্ণাঙ্ক c_step0 = 1;
                    যদি( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    বাবে( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ভিন্ন t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ভিন্ন t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        ভিন্ন t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        ভিন্ন t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (ভিন্ন)(t0*alpha + c[0]*beta);
                        d[d_step] = (ভিন্ন)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (ভিন্ন)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (ভিন্ন)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                অন্যথা
                    ভঙ্গ;
                ঘূৰাই;
            }
        }

        যদি( type == CV_64F )
        {
            দ্ৱিগুণ* d = D.ptr<দ্ৱিগুণ>();
            ধ্রুৱ দ্ৱিগুণ *a = A.ptr<দ্ৱিগুণ>(),
                         *b = B.ptr<দ্ৱিগুণ>(),
                         *c = (ধ্রুৱ দ্ৱিগুণ*)C.data;
            size_t d_step = D.step/পরিমাণ(d[0]),
                a_step = A.step/পরিমাণ(a[0]),
                b_step = B.step/পরিমাণ(b[0]),
                c_step = C.data ? C.step/পরিমাণ(c[0]) : 0;
            যদি( !c )
                c = zero;

            ৱিকল্প( len )
            {
            পক্ষ 2:
                যদি( len == d_size.width && b != d )
                {
                    বাবে( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দ্ৱিগুণ t0 = a[0]*b[0] + a[1]*b[b_step];
                        দ্ৱিগুণ t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                অন্যথা যদি( a != d )
                {
                    পূর্ণাঙ্ক c_step0 = 1;
                    যদি( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    বাবে( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        দ্ৱিগুণ t0 = a[0]*b[0] + a[1]*b[b_step];
                        দ্ৱিগুণ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                অন্যথা
                    ভঙ্গ;
                ঘূৰাই;
            পক্ষ 3:
                যদি( len == d_size.width && b != d )
                {
                    বাবে( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দ্ৱিগুণ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        দ্ৱিগুণ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        দ্ৱিগুণ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                অন্যথা যদি( a != d )
                {
                    পূর্ণাঙ্ক c_step0 = 1;
                    যদি( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    বাবে( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        দ্ৱিগুণ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        দ্ৱিগুণ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        দ্ৱিগুণ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                অন্যথা
                    ভঙ্গ;
                ঘূৰাই;
            পক্ষ 4:
                যদি( len == d_size.width && b != d )
                {
                    বাবে( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        দ্ৱিগুণ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        দ্ৱিগুণ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        দ্ৱিগুণ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        দ্ৱিগুণ t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                অন্যথা যদি( d_size.width <= 16 && a != d )
                {
                    পূর্ণাঙ্ক c_step0 = 1;
                    যদি( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    বাবে( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        দ্ৱিগুণ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        দ্ৱিগুণ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        দ্ৱিগুণ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        দ্ৱিগুণ t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                অন্যথা
                    ভঙ্গ;
                ঘূৰাই;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ধ্রুৱ uchar* Cdata = C.data;
    size_t Cstep = C.data ? (size_t)C.step : 0;
    AutoBuffer<uchar> buf;

    যদি( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ভিন্ন, দ্ৱিগুণ, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<ভিন্ন, দ্ৱিগুণ, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<ভিন্ন, দ্ৱিগুণ, GEMMStore_32f>;
    }
    অন্যথা যদি( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<দ্ৱিগুণ, দ্ৱিগুণ, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<দ্ৱিগুণ, দ্ৱিগুণ, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<দ্ৱিগুণ, দ্ৱিগুণ, GEMMStore_64f>;
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

    যদি( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
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
    else*/ যদি( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    অন্যথা
    {
        পূর্ণাঙ্ক is_a_t = flags & GEMM_1_T;
        পূর্ণাঙ্ক is_b_t = flags & GEMM_2_T;
        পূর্ণাঙ্ক elem_size = CV_ELEM_SIZE(type);
        পূর্ণাঙ্ক dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        পূর্ণাঙ্ক j, k, di = 0, dj = 0, dk = 0;
        পূর্ণাঙ্ক dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        পূর্ণাঙ্ক work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        যদি( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        অন্যথা
            a_step0 = elem_size, a_step1 = A.step;

        যদি( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        অন্যথা
            b_step0 = elem_size, b_step1 = b_step;

        যদি( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        অন্যথা যদি( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        অন্যথা
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        যদি( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        যদি( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        যদি( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        যদি( is_a_t )
            a_buf = b_buf + b_buf_size;

        বাবে( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            যদি( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            বাবে( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                ধ্রুৱ uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                যদি( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                যদি( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                বাবে( k = 0; k < len; k += dk )
                {
                    ধ্রুৱ uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    ধ্রুৱ uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    যদি( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    যদি( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    অন্যথা
                        a_bl_size.width = di, a_bl_size.height = dk;

                    যদি( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    যদি( dj < d_size.width )
                    {
                        Size b_size;
                        যদি( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        অন্যথা
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    যদি( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    অন্যথা
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                যদি( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>অন্তঃস্থ স্থির শূন্য
callGemmImpl(ধ্রুৱ fptype *src1, size_t src1_step, ধ্রুৱ fptype *src2, size_t src2_step, fptype alpha,
          ধ্রুৱ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    পূর্ণাঙ্ক b_m, b_n, c_m, c_n, m_d;

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
    যদি(src1 != রিক্ত)
        A = Mat(m_a, n_a, type, (শূন্য*)src1, src1_step);
    যদি(src2 != রিক্ত)
        B = Mat(b_m, b_n, type, (শূন্য*)src2, src2_step);
    যদি(src3 != রিক্ত && beta != 0.0)
        C = Mat(c_m, c_n, type, (শূন্য*)src3, src3_step);
    Mat D(m_d, n_d, type, (শূন্য*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

শূন্য gemm32f(ধ্রুৱ ভিন্ন* src1, size_t src1_step, ধ্রুৱ ভিন্ন* src2, size_t src2_step,
             ভিন্ন alpha, ধ্রুৱ ভিন্ন* src3, size_t src3_step, ভিন্ন beta, ভিন্ন* dst, size_t dst_step,
             পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

শূন্য gemm64f(ধ্রুৱ দ্ৱিগুণ* src1, size_t src1_step, ধ্রুৱ দ্ৱিগুণ* src2, size_t src2_step,
             দ্ৱিগুণ alpha, ধ্রুৱ দ্ৱিগুণ* src3, size_t src3_step, দ্ৱিগুণ beta, দ্ৱিগুণ* dst, size_t dst_step,
             পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

শূন্য gemm32fc(ধ্রুৱ ভিন্ন* src1, size_t src1_step, ধ্রুৱ ভিন্ন* src2, size_t src2_step,
              ভিন্ন alpha, ধ্রুৱ ভিন্ন* src3, size_t src3_step, ভিন্ন beta, ভিন্ন* dst, size_t dst_step,
              পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

শূন্য gemm64fc(ধ্রুৱ দ্ৱিগুণ* src1, size_t src1_step, ধ্রুৱ দ্ৱিগুণ* src2, size_t src2_step,
              দ্ৱিগুণ alpha, ধ্রুৱ দ্ৱিগুণ* src3, size_t src3_step, দ্ৱিগুণ beta, দ্ৱিগুণ* dst, size_t dst_step,
              পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> স্থির শূন্য
transform_( ধ্রুৱ T* src, T* dst, ধ্রুৱ WT* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn )
{
    পূর্ণাঙ্ক x;

    যদি( scn == 2 && dcn == 2 )
    {
        বাবে( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    অন্যথা যদি( scn == 3 && dcn == 3 )
    {
        বাবে( x = 0; x < len*3; x += 3 )
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
        বাবে( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    অন্যথা যদি( scn == 4 && dcn == 4 )
    {
        বাবে( x = 0; x < len*4; x += 4 )
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
        বাবে( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ধ্রুৱ WT* _m = m;
            পূর্ণাঙ্ক j, k;
            বাবে( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                বাবে( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

স্থির শূন্য
transform_8u( ধ্রুৱ uchar* src, uchar* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn )
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক BITS = 10, SCALE = 1 << BITS;
    ধ্রুৱ ভিন্ন MAX_M = (ভিন্ন)(1 << (15 - BITS));

    যদি( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ধ্রুৱ পূর্ণাঙ্ক nChannels = 3;

        সঙ্ঘ {
            short s[6];
            পূর্ণাঙ্ক p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        পূর্ণাঙ্ক m32[] = {saturate_cast<পূর্ণাঙ্ক>(m[ 2] * SCALE), saturate_cast<পূর্ণাঙ্ক>(m[ 3] * SCALE),
                     saturate_cast<পূর্ণাঙ্ক>(m[ 6] * SCALE), saturate_cast<পূর্ণাঙ্ক>(m[ 7] * SCALE),
                     saturate_cast<পূর্ণাঙ্ক>(m[10] * SCALE), saturate_cast<পূর্ণাঙ্ক>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        পূর্ণাঙ্ক x = 0;
        বাবে (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<পূর্ণাঙ্ক>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<পূর্ণাঙ্ক>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<পূর্ণাঙ্ক>((m[11] + 0.5f)*SCALE);
        বাবে( ; x < len * nChannels; x += nChannels )
        {
            পূর্ণাঙ্ক v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_16u( ধ্রুৱ ushort* src, ushort* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn )
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি( scn == 3 && dcn == 3 )
    {
        পূর্ণাঙ্ক x = 0;
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
        বাবে (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
        বাবে( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_ডান<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        বাবে( ; x < len * 3; x += 3 )
        {
            ভিন্ন v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_32f( ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#যদি (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    পূর্ণাঙ্ক x = 0;
    যদি( scn == 3 && dcn == 3 )
    {
        পূর্ণাঙ্ক idx[VTraits<v_float32>::max_nlanes/2];
        বাবে( পূর্ণাঙ্ক i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        ভিন্ন _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        বাবে( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        বাবে( ; x < len*3; x += 3 )
        {
            ভিন্ন v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            ভিন্ন t0 = saturate_cast<ভিন্ন>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            ভিন্ন t1 = saturate_cast<ভিন্ন>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            ভিন্ন t2 = saturate_cast<ভিন্ন>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ঘূৰাই;
    }

    যদি( scn == 4 && dcn == 4 )
    {
#যদি CV_SIMD_WIDTH > 16
        পূর্ণাঙ্ক idx[VTraits<v_float32>::max_nlanes/4];
        বাবে( পূর্ণাঙ্ক i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        ভিন্ন _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        বাবে( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
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
        বাবে( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#অন্যথা // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        বাবে( ; x < len*4; x += 4 )
        {
            ভিন্ন v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            ভিন্ন t0 = saturate_cast<ভিন্ন>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            ভিন্ন t1 = saturate_cast<ভিন্ন>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            ভিন্ন t2 = saturate_cast<ভিন্ন>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            ভিন্ন t3 = saturate_cast<ভিন্ন>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


স্থির শূন্য
transform_8s(ধ্রুৱ schar* src, schar* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক BITS = 10, SCALE = 1 << BITS;
    ধ্রুৱ ভিন্ন MAX_M = (ভিন্ন)(1 << (15 - BITS));

    যদি( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ধ্রুৱ পূর্ণাঙ্ক nChannels = 3;

        সঙ্ঘ {
            short s[6];
            পূর্ণাঙ্ক p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        পূর্ণাঙ্ক m32[] = {saturate_cast<পূর্ণাঙ্ক>(m[ 2] * SCALE), saturate_cast<পূর্ণাঙ্ক>(m[ 3] * SCALE),
                     saturate_cast<পূর্ণাঙ্ক>(m[ 6] * SCALE), saturate_cast<পূর্ণাঙ্ক>(m[ 7] * SCALE),
                     saturate_cast<পূর্ণাঙ্ক>(m[10] * SCALE), saturate_cast<পূর্ণাঙ্ক>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        পূর্ণাঙ্ক x = 0;
        বাবে (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<পূর্ণাঙ্ক>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<পূর্ণাঙ্ক>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<পূর্ণাঙ্ক>((m[11] + 0.5f)*SCALE);
        বাবে( ; x < len * nChannels; x += nChannels )
        {
            পূর্ণাঙ্ক v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_16s(ধ্রুৱ short* src, short* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

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
        বাবে (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        বাবে( ; x < len * 3; x += 3 )
        {
            ভিন্ন v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_32s(ধ্রুৱ পূর্ণাঙ্ক* src, পূর্ণাঙ্ক* dst, ধ্রুৱ দ্ৱিগুণ* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    যদি( scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

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
        বাবে (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_উপৰেৰ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_উপৰেৰ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_উপৰেৰ(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        বাবে (; x < len * 3; x += 3) {
            দ্ৱিগুণ b = src[x], g = src[x + 1], r = src[x + 2];
            পূর্ণাঙ্ক db = saturate_cast<পূর্ণাঙ্ক>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            পূর্ণাঙ্ক dg = saturate_cast<পূর্ণাঙ্ক>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            পূর্ণাঙ্ক dr = saturate_cast<পূর্ণাঙ্ক>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
transform_64f(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, ধ্রুৱ দ্ৱিগুণ* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    যদি( scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

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
        বাবে (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        বাবে (; x < len * 3; x += 3) {
            দ্ৱিগুণ b = src[x], g = src[x + 1], r = src[x + 2];
            দ্ৱিগুণ db = saturate_cast<দ্ৱিগুণ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            দ্ৱিগুণ dg = saturate_cast<দ্ৱিগুণ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            দ্ৱিগুণ dr = saturate_cast<দ্ৱিগুণ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> স্থির শূন্য
diagtransform_( ধ্রুৱ T* src, T* dst, ধ্রুৱ WT* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক )
{
    পূর্ণাঙ্ক x;

    যদি( cn == 2 )
    {
        বাবে( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    অন্যথা যদি( cn == 3 )
    {
        বাবে( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    অন্যথা যদি( cn == 4 )
    {
        বাবে( x = 0; x < len*4; x += 4 )
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
        বাবে( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ধ্রুৱ WT* _m = m;
            বাবে( পূর্ণাঙ্ক j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

স্থির শূন্য
diagtransform_8u(ধ্রুৱ uchar* src, uchar* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        বাবে (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        বাবে (; x < len * 3; x += 3) {
            পূর্ণাঙ্ক b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_8s(ধ্রুৱ schar* src, schar* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
    #যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        বাবে (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        বাবে (; x < len * 3; x += 3) {
            পূর্ণাঙ্ক b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_16u(ধ্রুৱ ushort* src, ushort* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        বাবে (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        বাবে (; x < len * 3; x += 3) {
            পূর্ণাঙ্ক b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_16s(ধ্রুৱ short* src, short* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        বাবে (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        বাবে (; x < len * 3; x += 3) {
            পূর্ণাঙ্ক b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_32s(ধ্রুৱ পূর্ণাঙ্ক* src, পূর্ণাঙ্ক* dst, ধ্রুৱ দ্ৱিগুণ* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        বাবে (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_উপৰেৰ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_উপৰেৰ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_উপৰেৰ(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        বাবে (; x < len * 3; x += 3) {
            পূর্ণাঙ্ক b = src[x], g = src[x + 1], r = src[x + 2];
            পূর্ণাঙ্ক db = saturate_cast<পূর্ণাঙ্ক>(m[ 0] * b + m[ 3]);
            পূর্ণাঙ্ক dg = saturate_cast<পূর্ণাঙ্ক>(m[ 5] * g + m[ 7]);
            পূর্ণাঙ্ক dr = saturate_cast<পূর্ণাঙ্ক>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_32f(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, ধ্রুৱ ভিন্ন* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        বাবে (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        বাবে (; x < len * 3; x += 3) {
            ভিন্ন b = src[x], g = src[x + 1], r = src[x + 2];
            ভিন্ন db = saturate_cast<ভিন্ন>(m[ 0] * b + m[ 3]);
            ভিন্ন dg = saturate_cast<ভিন্ন>(m[ 5] * g + m[ 7]);
            ভিন্ন dr = saturate_cast<ভিন্ন>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

স্থির শূন্য
diagtransform_64f(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, ধ্রুৱ দ্ৱিগুণ* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    যদি (scn == 3 && dcn == 3) {
        পূর্ণাঙ্ক x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        বাবে (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        বাবে (; x < len * 3; x += 3) {
            দ্ৱিগুণ b = src[x], g = src[x + 1], r = src[x + 2];
            দ্ৱিগুণ db = saturate_cast<দ্ৱিগুণ>(m[ 0] * b + m[ 3]);
            দ্ৱিগুণ dg = saturate_cast<দ্ৱিগুণ>(m[ 5] * g + m[ 7]);
            দ্ৱিগুণ dr = saturate_cast<দ্ৱিগুণ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ঘূৰাই;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, শূন্য (*fn)(ধ্রুৱ T*, T*, ধ্রুৱ WT*, পূর্ণাঙ্ক, পূর্ণাঙ্ক, পূর্ণাঙ্ক)>
স্থির শূন্য transformWrap(ধ্রুৱ শূন্য* src, শূন্য* dst, ধ্রুৱ শূন্য* m, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn)
{
    fn((ধ্রুৱ T*)src, (T*)dst, (ধ্রুৱ WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(পূর্ণাঙ্ক depth)
{
    স্থির TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, ভিন্ন, transform_8u>, transformWrap<schar, ভিন্ন, transform_8s>, transformWrap<ushort, ভিন্ন, transform_16u>,
        transformWrap<short, ভিন্ন, transform_16s>, transformWrap<পূর্ণাঙ্ক, দ্ৱিগুণ, transform_32s>, transformWrap<ভিন্ন, ভিন্ন, transform_32f>,
        transformWrap<দ্ৱিগুণ, দ্ৱিগুণ, transform_64f>, 0
    };

    ঘূৰাই transformTab[depth];
}

TransformFunc getDiagTransformFunc(পূর্ণাঙ্ক depth)
{
    স্থির TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, ভিন্ন, diagtransform_8u>, transformWrap<schar, ভিন্ন, diagtransform_8s>, transformWrap<ushort, ভিন্ন, diagtransform_16u>,
        transformWrap<short, ভিন্ন, diagtransform_16s>, transformWrap<পূর্ণাঙ্ক, দ্ৱিগুণ, diagtransform_32s>, transformWrap<ভিন্ন, ভিন্ন, diagtransform_32f>,
        transformWrap<দ্ৱিগুণ, দ্ৱিগুণ, diagtransform_64f>, 0
    };

    ঘূৰাই diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> স্থির শূন্য
perspectiveTransform_( ধ্রুৱ শূন্য* _src, শূন্য* _dst, ধ্রুৱ শূন্য* _m_ptr, পূর্ণাঙ্ক len, পূর্ণাঙ্ক scn, পূর্ণাঙ্ক dcn )
{
    ধ্রুৱ T* src = (ধ্রুৱ T*)_src;
    T* dst = (T*)_dst;
    ধ্রুৱ দ্ৱিগুণ* m = (ধ্রুৱ দ্ৱিগুণ*)_m_ptr;
    ধ্রুৱ দ্ৱিগুণ eps = FLT_EPSILON;
    পূর্ণাঙ্ক i;

    যদি( scn == 2 && dcn == 2 )
    {
        বাবে( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            দ্ৱিগুণ w = x*m[6] + y*m[7] + m[8];

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
        বাবে( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            দ্ৱিগুণ w = x*m[12] + y*m[13] + z*m[14] + m[15];

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
        বাবে( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            দ্ৱিগুণ w = x*m[8] + y*m[9] + z*m[10] + m[11];

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
        বাবে( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ধ্রুৱ দ্ৱিগুণ* _m = m + dcn*(scn + 1);
            দ্ৱিগুণ w = _m[scn];
            পূর্ণাঙ্ক j, k;
            বাবে( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            যদি( fabs(w) > eps )
            {
                _m = m;
                বাবে( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    দ্ৱিগুণ s = _m[scn];
                    বাবে( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            অন্যথা
                বাবে( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(পূর্ণাঙ্ক depth)
{
    যদি (depth == CV_32F)
        ঘূৰাই perspectiveTransform_<ভিন্ন>;
    যদি (depth == CV_64F)
        ঘূৰাই perspectiveTransform_<দ্ৱিগুণ>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

স্থির শূন্য scaleAdd_32f(ধ্রুৱ শূন্য* _src1, ধ্রুৱ শূন্য* _src2, শূন্য* _dst,
                         পূর্ণাঙ্ক len, ধ্রুৱ শূন্য* _alpha)
{
    ধ্রুৱ ভিন্ন* src1 = (ধ্রুৱ ভিন্ন*)_src1;
    ধ্রুৱ ভিন্ন* src2 = (ধ্রুৱ ভিন্ন*)_src2;
    ভিন্ন* dst = (ভিন্ন*)_dst;
    ভিন্ন alpha = *(ধ্রুৱ ভিন্ন*)_alpha;
    পূর্ণাঙ্ক i = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ধ্রুৱ পূর্ণাঙ্ক cWidth = VTraits<v_float32>::vlanes();
    বাবে (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    বাবে (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


স্থির শূন্য scaleAdd_64f(ধ্রুৱ শূন্য* _src1, ধ্রুৱ শূন্য* _src2, শূন্য* _dst,
                         পূর্ণাঙ্ক len, ধ্রুৱ শূন্য* _alpha)
{
    ধ্রুৱ দ্ৱিগুণ* src1 = (ধ্রুৱ দ্ৱিগুণ*)_src1;
    ধ্রুৱ দ্ৱিগুণ* src2 = (ধ্রুৱ দ্ৱিগুণ*)_src2;
    দ্ৱিগুণ* dst = (দ্ৱিগুণ*)_dst;
    দ্ৱিগুণ alpha = *(ধ্রুৱ দ্ৱিগুণ*)_alpha;
    পূর্ণাঙ্ক i = 0;
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ধ্রুৱ পূর্ণাঙ্ক cWidth = VTraits<v_float64>::vlanes();
    বাবে (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    বাবে (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(পূর্ণাঙ্ক depth)
{
    যদি (depth == CV_32F)
        ঘূৰাই scaleAdd_32f;
    যদি (depth == CV_64F)
        ঘূৰাই scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> স্থির অন্তঃস্থ
দ্ৱিগুণ MahalanobisImpl(ধ্রুৱ Mat& v1, ধ্রুৱ Mat& v2, ধ্রুৱ Mat& icovar, দ্ৱিগুণ *diff_buffer /*[len]*/, পূর্ণাঙ্ক len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    দ্ৱিগুণ result = 0;

    sz.width *= v1.channels();
    যদি (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        ধ্রুৱ T* src1 = v1.ptr<T>();
        ধ্রুৱ T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/পরিমাণ(src1[0]);
        size_t step2 = v2.step/পরিমাণ(src2[0]);
        দ্ৱিগুণ* diff = diff_buffer;
        ধ্রুৱ T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/পরিমাণ(mat[0]);

        বাবে (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            বাবে (পূর্ণাঙ্ক i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        বাবে (পূর্ণাঙ্ক i = 0; i < len; i++, mat += matstep)
        {
            দ্ৱিগুণ row_sum = 0;
            পূর্ণাঙ্ক j = 0;
#যদি CV_ENABLE_UNROLLED
            বাবে(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            বাবে (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    ঘূৰাই result;
}

MahalanobisImplFunc getMahalanobisImplFunc(পূর্ণাঙ্ক depth)
{
    যদি (depth == CV_32F)
        ঘূৰাই (MahalanobisImplFunc)MahalanobisImpl<ভিন্ন>;
    যদি (depth == CV_64F)
        ঘূৰাই (MahalanobisImplFunc)MahalanobisImpl<দ্ৱিগুণ>;
    CV_Assert(0 && "Not supported");
}


#যদি !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> স্থির শূন্য
MulTransposedR(ধ্রুৱ Mat& srcmat, ধ্রুৱ Mat& dstmat, ধ্রুৱ Mat& deltamat, দ্ৱিগুণ scale)
{
    পূর্ণাঙ্ক i, j, k;
    ধ্রুৱ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ধ্রুৱ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/পরিমাণ(src[0]);
    size_t dststep = dstmat.step/পরিমাণ(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/পরিমাণ(delta[0]) : 0;
    পূর্ণাঙ্ক delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    পূর্ণাঙ্ক buf_size = size.height*পরিমাণ(dT);
    AutoBuffer<uchar> buf;

    যদি( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    যদি( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        বাবে( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#যদি CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    যদি( !delta )
        বাবে( i = 0; i < size.width; i++, tdst += dststep )
        {
            বাবে( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            বাবে( j = i; j <= size.width - 4; j += 4 )
            {
#যদি CV_SIMD128_64F
                যদি (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ধ্রুৱ দ্ৱিগুণ *tsrc = (দ্ৱিগুণ*)(src + j);

                    বাবে( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((দ্ৱিগুণ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((দ্ৱিগুণ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((দ্ৱিগুণ*)(tdst+j+2), v_mul(s1, v_scale));
                } অন্যথা
#endif
                {
                    দ্ৱিগুণ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ধ্রুৱ sT *tsrc = src + j;

                    বাবে( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        দ্ৱিগুণ a = col_buf[k];
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

            বাবে( ; j < size.width; j++ )
            {
                দ্ৱিগুণ s0 = 0;
                ধ্রুৱ sT *tsrc = src + j;

                বাবে( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (দ্ৱিগুণ)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    অন্যথা
        বাবে( i = 0; i < size.width; i++, tdst += dststep )
        {
            যদি( !delta_buf )
                বাবে( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            অন্যথা
                বাবে( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            বাবে( j = i; j <= size.width - 4; j += 4 )
            {
#যদি CV_SIMD128_64F
                যদি (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ধ্রুৱ দ্ৱিগুণ *tsrc = (দ্ৱিগুণ*)(src + j);
                    ধ্রুৱ দ্ৱিগুণ *d = (দ্ৱিগুণ*)(delta_buf ? delta_buf : delta + j);

                    বাবে( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((দ্ৱিগুণ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((দ্ৱিগুণ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((দ্ৱিগুণ*)(tdst+j+2), v_mul(s1, v_scale));
                }
                অন্যথা
#endif

                {
                    দ্ৱিগুণ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ধ্রুৱ sT *tsrc = src + j;
                    ধ্রুৱ dT *d = delta_buf ? delta_buf : delta + j;

                    বাবে( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        দ্ৱিগুণ a = col_buf[k];
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

            বাবে( ; j < size.width; j++ )
            {
                দ্ৱিগুণ s0 = 0;
                ধ্রুৱ sT *tsrc = src + j;
                ধ্রুৱ dT *d = delta_buf ? delta_buf : delta + j;

                বাবে( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (দ্ৱিগুণ)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> স্থির শূন্য
MulTransposedL(ধ্রুৱ Mat& srcmat, ধ্রুৱ Mat& dstmat, ধ্রুৱ Mat& deltamat, দ্ৱিগুণ scale)
{
    পূর্ণাঙ্ক i, j, k;
    ধ্রুৱ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ধ্রুৱ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/পরিমাণ(src[0]);
    size_t dststep = dstmat.step/পরিমাণ(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/পরিমাণ(delta[0]) : 0;
    পূর্ণাঙ্ক delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    যদি( !delta )
        বাবে( i = 0; i < size.height; i++, tdst += dststep )
            বাবে( j = i; j < size.height; j++ )
            {
                দ্ৱিগুণ s = 0;
                ধ্রুৱ sT *tsrc1 = src + i*srcstep;
                ধ্রুৱ sT *tsrc2 = src + j*srcstep;
#যদি CV_SIMD128_64F
                যদি (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ধ্রুৱ দ্ৱিগুণ *v_tsrc1 = (দ্ৱিগুণ *)(tsrc1);
                    ধ্রুৱ দ্ৱিগুণ *v_tsrc2 = (দ্ৱিগুণ *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    বাবে( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                অন্যথা
#endif
                {
                    বাবে( k = 0; k <= size.width - 4; k += 4 )
                        s += (দ্ৱিগুণ)tsrc1[k]*tsrc2[k] + (দ্ৱিগুণ)tsrc1[k+1]*tsrc2[k+1] +
                             (দ্ৱিগুণ)tsrc1[k+2]*tsrc2[k+2] + (দ্ৱিগুণ)tsrc1[k+3]*tsrc2[k+3];
                }
                বাবে( ; k < size.width; k++ )
                    s += (দ্ৱিগুণ)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    অন্যথা
    {
        dT delta_buf[4];
        পূর্ণাঙ্ক delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*পরিমাণ(dT));
        dT* row_buf = (dT*)buf.data();

        বাবে( i = 0; i < size.height; i++, tdst += dststep )
        {
            ধ্রুৱ sT *tsrc1 = src + i*srcstep;
            ধ্রুৱ dT *tdelta1 = delta + i*deltastep;

            যদি( delta_cols < size.width )
                বাবে( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            অন্যথা
                বাবে( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            বাবে( j = i; j < size.height; j++ )
            {
                দ্ৱিগুণ s = 0;
                ধ্রুৱ sT *tsrc2 = src + j*srcstep;
                ধ্রুৱ dT *tdelta2 = delta + j*deltastep;
                যদি( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#যদি CV_SIMD128_64F
                যদি (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ধ্রুৱ দ্ৱিগুণ *v_tsrc2 = (দ্ৱিগুণ *)(tsrc2);
                    ধ্রুৱ দ্ৱিগুণ *v_tdelta2 = (দ্ৱিগুণ *)(tdelta2);
                    ধ্রুৱ দ্ৱিগুণ *v_row_buf = (দ্ৱিগুণ *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    বাবে( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (ধ্রুৱ dT *)(v_tdelta2);
                }
                অন্যথা
#endif
                {
                    বাবে( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (দ্ৱিগুণ)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (দ্ৱিগুণ)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (দ্ৱিগুণ)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (দ্ৱিগুণ)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                বাবে( ; k < size.width; k++, tdelta2++ )
                    s += (দ্ৱিগুণ)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(পূর্ণাঙ্ক stype, পূর্ণাঙ্ক dtype, তার্কিক ata)
{
    MulTransposedFunc func = রিক্ত;
    যদি (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,ভিন্ন>
                   : MulTransposedL<uchar,ভিন্ন>;
    }
    অন্যথা যদি (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,দ্ৱিগুণ>
                   : MulTransposedL<uchar,দ্ৱিগুণ>;
    }
    অন্যথা যদি(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,ভিন্ন>
                   : MulTransposedL<ushort,ভিন্ন>;
    }
    অন্যথা যদি(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,দ্ৱিগুণ>
                   : MulTransposedL<ushort,দ্ৱিগুণ>;
    }
    অন্যথা যদি(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,ভিন্ন>
                   : MulTransposedL<short,ভিন্ন>;
    }
    অন্যথা যদি(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,দ্ৱিগুণ>
                   : MulTransposedL<short,দ্ৱিগুণ>;
    }
    অন্যথা যদি(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ভিন্ন,ভিন্ন>
                   : MulTransposedL<ভিন্ন,ভিন্ন>;
    }
    অন্যথা যদি(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ভিন্ন,দ্ৱিগুণ>
                   : MulTransposedL<ভিন্ন,দ্ৱিগুণ>;
    }
    অন্যথা যদি(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<দ্ৱিগুণ,দ্ৱিগুণ>
                   : MulTransposedL<দ্ৱিগুণ,দ্ৱিগুণ>;
    }
    CV_Assert(func && "Not supported");
    ঘূৰাই func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> স্থির অন্তঃস্থ
দ্ৱিগুণ dotProd_(ধ্রুৱ T* src1, ধ্রুৱ T* src2, পূর্ণাঙ্ক len)
{
    পূর্ণাঙ্ক i = 0;
    দ্ৱিগুণ result = 0;

#যদি CV_ENABLE_UNROLLED
    বাবে( ; i <= len - 4; i += 4 )
        result += (দ্ৱিগুণ)src1[i]*src2[i] + (দ্ৱিগুণ)src1[i+1]*src2[i+1] +
            (দ্ৱিগুণ)src1[i+2]*src2[i+2] + (দ্ৱিগুণ)src1[i+3]*src2[i+3];
#endif
    বাবে( ; i < len; i++ )
        result += (দ্ৱিগুণ)src1[i]*src2[i];

    ঘূৰাই result;
}


দ্ৱিগুণ dotProd_8u(ধ্রুৱ uchar* src1, ধ্রুৱ uchar* src2, পূর্ণাঙ্ক len)
{
    দ্ৱিগুণ r = 0;
    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণাঙ্ক len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    যেতিয়ালৈ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        ধ্রুৱ পূর্ণাঙ্ক cWidth = VTraits<v_uint16>::vlanes();

        পূর্ণাঙ্ক j = 0;
        বাবে (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        বাবে (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (দ্ৱিগুণ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ঘূৰাই r + dotProd_(src1, src2, len - i);
}


দ্ৱিগুণ dotProd_8s(ধ্রুৱ schar* src1, ধ্রুৱ schar* src2, পূর্ণাঙ্ক len)
{
    দ্ৱিগুণ r = 0.0;
    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণাঙ্ক len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    যেতিয়ালৈ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        ধ্রুৱ পূর্ণাঙ্ক cWidth = VTraits<v_int16>::vlanes();

        পূর্ণাঙ্ক j = 0;
        বাবে (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        বাবে (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (দ্ৱিগুণ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    ঘূৰাই r + dotProd_(src1, src2, len - i);
}

দ্ৱিগুণ dotProd_16u(ধ্রুৱ ushort* src1, ধ্রুৱ ushort* src2, পূর্ণাঙ্ক len)
{
    দ্ৱিগুণ r = 0.0;
    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণাঙ্ক len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    যেতিয়ালৈ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        ধ্রুৱ পূর্ণাঙ্ক cWidth = VTraits<v_uint16>::vlanes();

        পূর্ণাঙ্ক j = 0;
        বাবে (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (দ্ৱিগুণ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ঘূৰাই r + dotProd_(src1, src2, len - i);
}

দ্ৱিগুণ dotProd_16s(ধ্রুৱ short* src1, ধ্রুৱ short* src2, পূর্ণাঙ্ক len)
{
    দ্ৱিগুণ r = 0.0;
    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণাঙ্ক len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    যেতিয়ালৈ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        ধ্রুৱ পূর্ণাঙ্ক cWidth = VTraits<v_int16>::vlanes();

        পূর্ণাঙ্ক j = 0;
        বাবে (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (দ্ৱিগুণ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ঘূৰাই r + dotProd_(src1, src2, len - i);
}

দ্ৱিগুণ dotProd_32s(ধ্রুৱ পূর্ণাঙ্ক* src1, ধ্রুৱ পূর্ণাঙ্ক* src2, পূর্ণাঙ্ক len)
{
#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    দ্ৱিগুণ r = .0;
    পূর্ণাঙ্ক i = 0;
    ধ্রুৱ পূর্ণাঙ্ক step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#যদি CV_SIMD_WIDTH == 16
    ধ্রুৱ পূর্ণাঙ্ক wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    বাবে (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    বাবে (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    ঘূৰাই r + dotProd_(src1, src2, len - i);
#অন্যথা
    ঘূৰাই dotProd_(src1, src2, len);
#endif
}

দ্ৱিগুণ dotProd_32f(ধ্রুৱ ভিন্ন* src1, ধ্রুৱ ভিন্ন* src2, পূর্ণাঙ্ক len)
{
    দ্ৱিগুণ r = 0.0;
    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    পূর্ণাঙ্ক len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    যেতিয়ালৈ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        পূর্ণাঙ্ক j = 0;
        পূর্ণাঙ্ক cWidth = VTraits<v_float32>::vlanes();

#যদি CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        বাবে (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
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

        বাবে (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ঘূৰাই r + dotProd_(src1, src2, len - i);
}

দ্ৱিগুণ dotProd_64f(ধ্রুৱ দ্ৱিগুণ* src1, ধ্রুৱ দ্ৱিগুণ* src2, পূর্ণাঙ্ক len)
{
    ঘূৰাই dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
