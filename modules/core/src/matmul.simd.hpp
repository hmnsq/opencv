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

#അന്തര്ഭാവ "precomp.hpp"

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
#എങ്കിൽ defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#അല്ലെങ്കിൽ
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

നാമാകാശ cv {

// forward declarations
തരനിർവചനം ശൂന്യം (*TransformFunc)(സ്ഥിരം ശൂന്യം* src, ശൂന്യം* dst, സ്ഥിരം ശൂന്യം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn);
തരനിർവചനം ശൂന്യം (*ScaleAddFunc)(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len, സ്ഥിരം ശൂന്യം* alpha);
തരനിർവചനം ശൂന്യം (*MulTransposedFunc)(സ്ഥിരം Mat& src, സ്ഥിരം/*preallocated*/ Mat& dst, സ്ഥിരം Mat& delta, ഇരട്ട scale);
തരനിർവചനം ഇരട്ട (*MahalanobisImplFunc)(സ്ഥിരം Mat& v1, സ്ഥിരം Mat& v2, സ്ഥിരം Mat& icovar, ഇരട്ട *diff_buffer /*[len]*/, പൂർണ്ണസംഖ്യ len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

ശൂന്യം gemm32f(സ്ഥിരം ദശാംശം* src1, size_t src1_step, സ്ഥിരം ദശാംശം* src2, size_t src2_step,
             ദശാംശം alpha, സ്ഥിരം ദശാംശം* src3, size_t src3_step, ദശാംശം beta, ദശാംശം* dst, size_t dst_step,
             പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags);
ശൂന്യം gemm64f(സ്ഥിരം ഇരട്ട* src1, size_t src1_step, സ്ഥിരം ഇരട്ട* src2, size_t src2_step,
             ഇരട്ട alpha, സ്ഥിരം ഇരട്ട* src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട* dst, size_t dst_step,
             പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags);
ശൂന്യം gemm32fc(സ്ഥിരം ദശാംശം* src1, size_t src1_step, സ്ഥിരം ദശാംശം* src2, size_t src2_step,
              ദശാംശം alpha, സ്ഥിരം ദശാംശം* src3, size_t src3_step, ദശാംശം beta, ദശാംശം* dst, size_t dst_step,
              പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags);
ശൂന്യം gemm64fc(സ്ഥിരം ഇരട്ട* src1, size_t src1_step, സ്ഥിരം ഇരട്ട* src2, size_t src2_step,
              ഇരട്ട alpha, സ്ഥിരം ഇരട്ട* src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട* dst, size_t dst_step,
              പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags);

TransformFunc getTransformFunc(പൂർണ്ണസംഖ്യ depth);
TransformFunc getDiagTransformFunc(പൂർണ്ണസംഖ്യ depth);
TransformFunc getPerspectiveTransform(പൂർണ്ണസംഖ്യ depth);

ScaleAddFunc getScaleAddFunc(പൂർണ്ണസംഖ്യ depth);

MahalanobisImplFunc getMahalanobisImplFunc(പൂർണ്ണസംഖ്യ depth);

MulTransposedFunc getMulTransposedFunc(പൂർണ്ണസംഖ്യ stype, പൂർണ്ണസംഖ്യ dtype, തർക്കം ata);

ഇരട്ട dotProd_8u(സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, പൂർണ്ണസംഖ്യ len);
ഇരട്ട dotProd_8s(സ്ഥിരം schar* src1, സ്ഥിരം schar* src2, പൂർണ്ണസംഖ്യ len);
ഇരട്ട dotProd_16u(സ്ഥിരം ushort* src1, സ്ഥിരം ushort* src2, പൂർണ്ണസംഖ്യ len);
ഇരട്ട dotProd_16s(സ്ഥിരം short* src1, സ്ഥിരം short* src2, പൂർണ്ണസംഖ്യ len);
ഇരട്ട dotProd_32s(സ്ഥിരം പൂർണ്ണസംഖ്യ* src1, സ്ഥിരം പൂർണ്ണസംഖ്യ* src2, പൂർണ്ണസംഖ്യ len);
ഇരട്ട dotProd_32f(സ്ഥിരം ദശാംശം* src1, സ്ഥിരം ദശാംശം* src2, പൂർണ്ണസംഖ്യ len);
ഇരട്ട dotProd_64f(സ്ഥിരം ഇരട്ട* src1, സ്ഥിരം ഇരട്ട* src2, പൂർണ്ണസംഖ്യ len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#എങ്കിൽ !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

സ്ഥിതം ശൂന്യം
GEMM_CopyBlock( സ്ഥിരം uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    പൂർണ്ണസംഖ്യ j;
    size.width *= (പൂർണ്ണസംഖ്യ)(pix_size / വലുപ്പം(പൂർണ്ണസംഖ്യ));

    വേണ്ടി( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #എങ്കിൽ CV_ENABLE_UNROLLED
        വേണ്ടി( ; j <= size.width - 4; j += 4 )
        {
            പൂർണ്ണസംഖ്യ t0 = ((സ്ഥിരം പൂർണ്ണസംഖ്യ*)src)[j];
            പൂർണ്ണസംഖ്യ t1 = ((സ്ഥിരം പൂർണ്ണസംഖ്യ*)src)[j+1];
            ((പൂർണ്ണസംഖ്യ*)dst)[j] = t0;
            ((പൂർണ്ണസംഖ്യ*)dst)[j+1] = t1;
            t0 = ((സ്ഥിരം പൂർണ്ണസംഖ്യ*)src)[j+2];
            t1 = ((സ്ഥിരം പൂർണ്ണസംഖ്യ*)src)[j+3];
            ((പൂർണ്ണസംഖ്യ*)dst)[j+2] = t0;
            ((പൂർണ്ണസംഖ്യ*)dst)[j+3] = t1;
        }
        #endif
        വേണ്ടി( ; j < size.width; j++ )
            ((പൂർണ്ണസംഖ്യ*)dst)[j] = ((സ്ഥിരം പൂർണ്ണസംഖ്യ*)src)[j];
    }
}


സ്ഥിതം ശൂന്യം
GEMM_TransposeBlock( സ്ഥിരം uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    പൂർണ്ണസംഖ്യ i, j;
    വേണ്ടി( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        സ്ഥിരം uchar* _src = src;
        തിരഞ്ഞെടുപ്പ്( pix_size )
        {
        അവസ്ഥ വലുപ്പം(പൂർണ്ണസംഖ്യ):
            വേണ്ടി( j = 0; j < size.height; j++, _src += src_step )
                ((പൂർണ്ണസംഖ്യ*)dst)[j] = ((പൂർണ്ണസംഖ്യ*)_src)[0];
            നിർത്തുക;
        അവസ്ഥ വലുപ്പം(പൂർണ്ണസംഖ്യ)*2:
            വേണ്ടി( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                പൂർണ്ണസംഖ്യ t0 = ((പൂർണ്ണസംഖ്യ*)_src)[0];
                പൂർണ്ണസംഖ്യ t1 = ((പൂർണ്ണസംഖ്യ*)_src)[1];
                ((പൂർണ്ണസംഖ്യ*)dst)[j] = t0;
                ((പൂർണ്ണസംഖ്യ*)dst)[j+1] = t1;
            }
            നിർത്തുക;
        അവസ്ഥ വലുപ്പം(പൂർണ്ണസംഖ്യ)*4:
            വേണ്ടി( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                പൂർണ്ണസംഖ്യ t0 = ((പൂർണ്ണസംഖ്യ*)_src)[0];
                പൂർണ്ണസംഖ്യ t1 = ((പൂർണ്ണസംഖ്യ*)_src)[1];
                ((പൂർണ്ണസംഖ്യ*)dst)[j] = t0;
                ((പൂർണ്ണസംഖ്യ*)dst)[j+1] = t1;
                t0 = ((പൂർണ്ണസംഖ്യ*)_src)[2];
                t1 = ((പൂർണ്ണസംഖ്യ*)_src)[3];
                ((പൂർണ്ണസംഖ്യ*)dst)[j+2] = t0;
                ((പൂർണ്ണസംഖ്യ*)dst)[j+3] = t1;
            }
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            CV_Assert(0);
            തിരികെ;
        }
    }
}

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> ഘടന VecTraits;

template<> ഘടന VecTraits<ദശാംശം> {
    തരനിർവചനം v_float32 VT;
    സ്ഥിതം വരിയിൽ VT zero() { തിരികെ vx_setzero_f32(); }
    സ്ഥിതം വരിയിൽ VT load(സ്ഥിരം ദശാംശം* p) { തിരികെ vx_load(p); }
    സ്ഥിതം വരിയിൽ VT setall(ദശാംശം v) { തിരികെ vx_setall_f32(v); }
};
#endif

#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> ഘടന VecTraits<ഇരട്ട> {
    തരനിർവചനം v_float64 VT;
    സ്ഥിതം വരിയിൽ VT zero() { തിരികെ vx_setzero_f64(); }
    സ്ഥിതം വരിയിൽ VT load(സ്ഥിരം ഇരട്ട* p) { തിരികെ vx_load(p); }
    സ്ഥിതം വരിയിൽ VT setall(ഇരട്ട v) { തിരികെ vx_setall_f64(v); }
};
#endif

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
സ്ഥിതം വരിയിൽ ST simdDotProduct(സ്ഥിരം ST* a, സ്ഥിരം ST* b, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ& k)
{
    തരനിർവചനം typename VecTraits<ST>::VT VecT;
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes = VTraits<VecT>::vlanes();
    സ്ഥിരം പൂർണ്ണസംഖ്യ stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    വേണ്ടി( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    വേണ്ടി( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    തിരികെ v_reduce_sum(vs0);
}
#endif

#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
സ്ഥിതം വരിയിൽ ഇരട്ട simdDotProduct_f32f64(സ്ഥിരം ദശാംശം* a, സ്ഥിരം ദശാംശം* b, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ& k)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes_f32 = VTraits<v_float32>::vlanes();
    സ്ഥിരം പൂർണ്ണസംഖ്യ stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    വേണ്ടി( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_മുകളിലെ(va0), v_cvt_f64_മുകളിലെ(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_മുകളിലെ(va1), v_cvt_f64_മുകളിലെ(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    വേണ്ടി( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_മുകളിലെ(va), v_cvt_f64_മുകളിലെ(vb), vs0);
    }
    തിരികെ v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
സ്ഥിതം വരിയിൽ ശൂന്യം simdGEMM_kj_f32f64(
    സ്ഥിരം ദശാംശം* a_data, സ്ഥിരം ദശാംശം* _b_data, size_t b_step,
    സ്ഥിരം ദശാംശം* _c_data, size_t c_step1,
    ദശാംശം* d_data,
    പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m,
    ഇരട്ട alpha, ഇരട്ട beta,
    പൂർണ്ണസംഖ്യ& j)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes_f32 = VTraits<v_float32>::vlanes();
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes_f64 = VTraits<v_float64>::vlanes();

    എങ്കിൽ( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        സ്ഥിരം പൂർണ്ണസംഖ്യ max_m = 1600 / (പൂർണ്ണസംഖ്യ)വലുപ്പം(ദശാംശം);
        cv::AutoBuffer<ഇരട്ട> _s_buf(max_m);
        ഇരട്ട* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            വേണ്ടി( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            വേണ്ടി( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            സ്ഥിരം ദശാംശം* b_ptr = _b_data;
            വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((ഇരട്ട)a_data[k]);
                j = 0;
                വേണ്ടി( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_മുകളിലെ(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                ഇരട്ട a_val = (ഇരട്ട)a_data[k];
                വേണ്ടി( ; j < m; j++ )
                    s_buf[j] += a_val * (ഇരട്ട)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            സ്ഥിരം ദശാംശം* c_data = _c_data;
            j = 0;
            എങ്കിൽ( !_c_data )
            {
                വേണ്ടി( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                വേണ്ടി( ; j < m; j++ )
                    d_data[j] = (ദശാംശം)(s_buf[j] * alpha);
            }
            അല്ലെങ്കിൽ എങ്കിൽ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                വേണ്ടി( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_മുകളിലെ(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                വേണ്ടി( ; j < m; j++ )
                    d_data[j] = (ദശാംശം)(s_buf[j] * alpha + (ഇരട്ട)c_data[j] * beta);
            }
            അല്ലെങ്കിൽ
            {
                വേണ്ടി( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (ദശാംശം)(s_buf[j] * alpha + (ഇരട്ട)c_data[0] * beta);
            }
        }
        j = m;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( vlanes_f32 * (പൂർണ്ണസംഖ്യ)വലുപ്പം(ദശാംശം) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        സ്ഥിരം ദശാംശം* c_data = _c_data;
        വേണ്ടി( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            സ്ഥിരം ദശാംശം* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((ഇരട്ട)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_മുകളിലെ(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            എങ്കിൽ( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            അല്ലെങ്കിൽ എങ്കിൽ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_മുകളിലെ(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            അല്ലെങ്കിൽ
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ദശാംശം buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                വേണ്ടി( പൂർണ്ണസംഖ്യ jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ദശാംശം)beta;
            }
        }
    }
}
#endif

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
സ്ഥിതം വരിയിൽ ശൂന്യം simdGEMM_kj(
    സ്ഥിരം ST* a_data, സ്ഥിരം ST* _b_data, size_t b_step,
    സ്ഥിരം ST* _c_data, size_t c_step1,
    ST* d_data,
    പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m,
    ഇരട്ട alpha, ഇരട്ട beta,
    പൂർണ്ണസംഖ്യ& j)
{
    തരനിർവചനം typename VecTraits<ST>::VT VecT;
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes = VTraits<VecT>::vlanes();

    എങ്കിൽ( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        സ്ഥിരം പൂർണ്ണസംഖ്യ max_m = 1600 / (പൂർണ്ണസംഖ്യ)വലുപ്പം(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            വേണ്ടി( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            വേണ്ടി( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            സ്ഥിരം ST* b_ptr = _b_data;
            വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                വേണ്ടി( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                വേണ്ടി( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            സ്ഥിരം ST* c_data = _c_data;
            j = 0;
            എങ്കിൽ( !_c_data )
            {
                വേണ്ടി( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                വേണ്ടി( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            അല്ലെങ്കിൽ എങ്കിൽ( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                വേണ്ടി( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                വേണ്ടി( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            അല്ലെങ്കിൽ
            {
                വേണ്ടി( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    അല്ലെങ്കിൽ എങ്കിൽ( vlanes * (പൂർണ്ണസംഖ്യ)വലുപ്പം(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        സ്ഥിരം ST* c_data = _c_data;
        വേണ്ടി( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            സ്ഥിരം ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            എങ്കിൽ( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            അല്ലെങ്കിൽ എങ്കിൽ( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            അല്ലെങ്കിൽ
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                വേണ്ടി( പൂർണ്ണസംഖ്യ jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> സ്ഥിതം ശൂന്യം
GEMMSingleMul( സ്ഥിരം T* a_data, size_t a_step,
               സ്ഥിരം T* b_data, size_t b_step,
               സ്ഥിരം T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    പൂർണ്ണസംഖ്യ i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    സ്ഥിരം T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= വലുപ്പം(a_data[0]);
    b_step /= വലുപ്പം(b_data[0]);
    c_step /= വലുപ്പം(c_data[0]);
    d_step /= വലുപ്പം(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    എങ്കിൽ( !c_data )
        c_step0 = c_step1 = 0;
    അല്ലെങ്കിൽ എങ്കിൽ( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    അല്ലെങ്കിൽ
        c_step0 = 1, c_step1 = c_step;

    എങ്കിൽ( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        എങ്കിൽ( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    എങ്കിൽ( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        എങ്കിൽ( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            വേണ്ടി( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        എങ്കിൽ( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            വേണ്ടി( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        വേണ്ടി( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            വേണ്ടി( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                എങ്കിൽ( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                അല്ലെങ്കിൽ
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            വേണ്ടി( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                എങ്കിൽ( !c_data )
                    d_data[j] = T(s0);
                അല്ലെങ്കിൽ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( flags & GEMM_2_T ) /* A * Bt */
    {
        വേണ്ടി( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            എങ്കിൽ( a_buf )
            {
                വേണ്ടി( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            വേണ്ടി( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#എങ്കിൽ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                എങ്കിൽ( വലുപ്പം(WT) == വലുപ്പം(ഇരട്ട) )
                {
                    എങ്കിൽ( വലുപ്പം(T) == വലുപ്പം(ഇരട്ട) )
                        s0 = (WT)simdDotProduct((സ്ഥിരം ഇരട്ട*)a_data, (സ്ഥിരം ഇരട്ട*)b_data, n, k);
                    അല്ലെങ്കിൽ എങ്കിൽ( വലുപ്പം(T) == വലുപ്പം(ദശാംശം) )
                        s0 = (WT)simdDotProduct_f32f64((സ്ഥിരം ദശാംശം*)a_data, (സ്ഥിരം ദശാംശം*)b_data, n, k);
                }
                അല്ലെങ്കിൽ
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #എങ്കിൽ CV_ENABLE_UNROLLED
                    വേണ്ടി( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                വേണ്ടി( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                എങ്കിൽ( !c_data )
                    d_data[j] = T(s0);
                അല്ലെങ്കിൽ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( d_size.width*വലുപ്പം(d_data[0]) <= 1600 )
    {
        വേണ്ടി( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            എങ്കിൽ( a_buf )
            {
                വേണ്ടി( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            എങ്കിൽ( വലുപ്പം(WT) == വലുപ്പം(ഇരട്ട) )
            {
                എങ്കിൽ( വലുപ്പം(T) == വലുപ്പം(ഇരട്ട) )
                    simdGEMM_kj((സ്ഥിരം ഇരട്ട*)a_data, (സ്ഥിരം ഇരട്ട*)_b_data, b_step,
                                        (സ്ഥിരം ഇരട്ട*)_c_data, c_step1,
                                (ഇരട്ട*)d_data, n, m, alpha, beta, j);
                അല്ലെങ്കിൽ എങ്കിൽ( വലുപ്പം(T) == വലുപ്പം(ദശാംശം) )
                    simdGEMM_kj_f32f64((സ്ഥിരം ദശാംശം*)a_data, (സ്ഥിരം ദശാംശം*)_b_data, b_step,
                                        (സ്ഥിരം ദശാംശം*)_c_data, c_step1,
                                (ദശാംശം*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            വേണ്ടി( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                സ്ഥിരം T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                വേണ്ടി( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                എങ്കിൽ( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                അല്ലെങ്കിൽ
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            വേണ്ടി( ; j < m; j++, c_data += c_step1 )
            {
                സ്ഥിരം T* b = _b_data + j;
                WT s0(0);

                വേണ്ടി( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                എങ്കിൽ( !c_data )
                    d_data[j] = T(s0);
                അല്ലെങ്കിൽ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        വേണ്ടി( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            എങ്കിൽ( a_buf )
            {
                വേണ്ടി( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            വേണ്ടി( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            വേണ്ടി( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                എങ്കിൽ( വലുപ്പം(WT) == വലുപ്പം(ഇരട്ട) )
                {
                    എങ്കിൽ( വലുപ്പം(T) == വലുപ്പം(ഇരട്ട) )
                    {
                        സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((സ്ഥിരം ഇരട്ട*)&al));
                        വേണ്ടി( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((സ്ഥിരം ഇരട്ട*)(d_buf + j));
                            v_float64 vb = vx_load((സ്ഥിരം ഇരട്ട*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((ഇരട്ട*)(d_buf + j), vd);
                        }
                    }
                    അല്ലെങ്കിൽ
                    {
                        സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes_f32 = VTraits<v_float32>::vlanes();
                        സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((സ്ഥിരം ഇരട്ട*)&al));
                        വേണ്ടി( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((സ്ഥിരം ദശാംശം*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_മുകളിലെ(vb);
                            v_float64 vd_lo = vx_load((സ്ഥിരം ഇരട്ട*)(d_buf + j));
                            v_float64 vd_hi = vx_load((സ്ഥിരം ഇരട്ട*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((ഇരട്ട*)(d_buf + j), vd_lo);
                            v_store((ഇരട്ട*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                വേണ്ടി( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            എങ്കിൽ( !c_data )
                വേണ്ടി( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            അല്ലെങ്കിൽ
                വേണ്ടി( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
സ്ഥിതം വരിയിൽ ശൂന്യം setDoubleDataZero(WT* d_data, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ vlanes)
{
    v_float64 vz = vx_setzero_f64();
    പൂർണ്ണസംഖ്യ j = 0;
    വേണ്ടി( ; j <= m - vlanes; j += vlanes )
        v_store((ഇരട്ട*)(d_data + j), vz);
    വേണ്ടി( ; j < m; j++ )
        d_data[j] = WT(0);
}

template<typename ST>
സ്ഥിതം വരിയിൽ ശൂന്യം simdMulAdd(സ്ഥിരം ST* b, ഇരട്ട* d, പൂർണ്ണസംഖ്യ m, ഇരട്ട a_val);

template<>
വരിയിൽ ശൂന്യം simdMulAdd<ദശാംശം>(
    സ്ഥിരം ദശാംശം* b, ഇരട്ട* d, പൂർണ്ണസംഖ്യ m, ഇരട്ട a_val)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes_f32 = VTraits<v_float32>::vlanes();
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    പൂർണ്ണസംഖ്യ j = 0;
    വേണ്ടി( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_മുകളിലെ(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    വേണ്ടി( ; j < m; j++ )
        d[j] += a_val * (ഇരട്ട)b[j];
}

template<>
വരിയിൽ ശൂന്യം simdMulAdd<ഇരട്ട>(
    സ്ഥിരം ഇരട്ട* b, ഇരട്ട* d, പൂർണ്ണസംഖ്യ m, ഇരട്ട a_val)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes = VTraits<v_float64>::vlanes();
    സ്ഥിരം പൂർണ്ണസംഖ്യ stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    പൂർണ്ണസംഖ്യ j = 0;
    വേണ്ടി( ; j <= m - stride2; j += stride2 )
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
    വേണ്ടി( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    വേണ്ടി( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
സ്ഥിതം വരിയിൽ ശൂന്യം simdBlockMul_kj(
    സ്ഥിരം ST* a_data, സ്ഥിരം ST* b_data, size_t b_step,
    ഇരട്ട* d_data, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, തർക്കം do_acc)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes = VTraits<v_float64>::vlanes();
    എങ്കിൽ( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (ഇരട്ട)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> സ്ഥിതം ശൂന്യം
GEMMBlockMul( സ്ഥിരം T* a_data, size_t a_step,
              സ്ഥിരം T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, പൂർണ്ണസംഖ്യ flags )
{
    പൂർണ്ണസംഖ്യ i, j, k, n = a_size.width, m = d_size.width;
    സ്ഥിരം T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    പൂർണ്ണസംഖ്യ do_acc = flags & 16;

    a_step /= വലുപ്പം(a_data[0]);
    b_step /= വലുപ്പം(b_data[0]);
    d_step /= വലുപ്പം(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    എങ്കിൽ( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    എങ്കിൽ( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        വേണ്ടി( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            എങ്കിൽ( a_buf )
            {
                വേണ്ടി( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            വേണ്ടി( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#എങ്കിൽ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                എങ്കിൽ( വലുപ്പം(WT) == വലുപ്പം(ഇരട്ട) )
                {
                    എങ്കിൽ( വലുപ്പം(T) == വലുപ്പം(ഇരട്ട) )
                        s0 += (WT)simdDotProduct((സ്ഥിരം ഇരട്ട*)a_data, (സ്ഥിരം ഇരട്ട*)b_data, n, k);
                    അല്ലെങ്കിൽ എങ്കിൽ( വലുപ്പം(T) == വലുപ്പം(ദശാംശം) )
                        s0 += (WT)simdDotProduct_f32f64((സ്ഥിരം ദശാംശം*)a_data, (സ്ഥിരം ദശാംശം*)b_data, n, k);
                }
                അല്ലെങ്കിൽ
#endif
                {
                    WT s1(0);
                    വേണ്ടി( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                വേണ്ടി( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            എങ്കിൽ( a_buf )
            {
                വേണ്ടി( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            എങ്കിൽ( വലുപ്പം(WT) == വലുപ്പം(ഇരട്ട) )
            {
                എങ്കിൽ( വലുപ്പം(T) == വലുപ്പം(ഇരട്ട) )
                    simdBlockMul_kj(
                        (സ്ഥിരം ഇരട്ട*)a_data, (സ്ഥിരം ഇരട്ട*)b_data, b_step,
                        (ഇരട്ട*)d_data, n, m, do_acc != 0);
                അല്ലെങ്കിൽ
                    simdBlockMul_kj(
                        (സ്ഥിരം ദശാംശം*)a_data, (സ്ഥിരം ദശാംശം*)b_data, b_step,
                        (ഇരട്ട*)d_data, n, m, do_acc != 0);
            }
            അല്ലെങ്കിൽ
#endif
            {
                വേണ്ടി( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    സ്ഥിരം T* b = b_data + j;

                    എങ്കിൽ( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    അല്ലെങ്കിൽ
                        s0 = s1 = s2 = s3 = WT(0);

                    വേണ്ടി( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                വേണ്ടി( ; j < m; j++ )
                {
                    സ്ഥിരം T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    വേണ്ടി( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> സ്ഥിതം ശൂന്യം
GEMMStore( സ്ഥിരം T* c_data, size_t c_step,
           സ്ഥിരം WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    സ്ഥിരം T* _c_data = c_data;
    പൂർണ്ണസംഖ്യ j;
    size_t c_step0, c_step1;

    c_step /= വലുപ്പം(c_data[0]);
    d_buf_step /= വലുപ്പം(d_buf[0]);
    d_step /= വലുപ്പം(d_data[0]);

    എങ്കിൽ( !c_data )
        c_step0 = c_step1 = 0;
    അല്ലെങ്കിൽ എങ്കിൽ( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    അല്ലെങ്കിൽ
        c_step0 = 1, c_step1 = c_step;

    വേണ്ടി( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        എങ്കിൽ( _c_data )
        {
            c_data = _c_data;
            j=0;
             #എങ്കിൽ CV_ENABLE_UNROLLED
            വേണ്ടി(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
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
            വേണ്ടി( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        അല്ലെങ്കിൽ
        {
            j = 0;
             #എങ്കിൽ CV_ENABLE_UNROLLED
            വേണ്ടി( ; j <= d_size.width - 4; j += 4 )
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
            വേണ്ടി( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


തരനിർവചനം ശൂന്യം (*GEMMSingleMulFunc)( സ്ഥിരം ശൂന്യം* src1, size_t step1,
                   സ്ഥിരം ശൂന്യം* src2, size_t step2, സ്ഥിരം ശൂന്യം* src3, size_t step3,
                   ശൂന്യം* dst, size_t dststep, Size srcsize, Size dstsize,
                   ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags );

തരനിർവചനം ശൂന്യം (*GEMMBlockMulFunc)( സ്ഥിരം ശൂന്യം* src1, size_t step1,
                   സ്ഥിരം ശൂന്യം* src2, size_t step2, ശൂന്യം* dst, size_t dststep,
                   Size srcsize, Size dstsize, പൂർണ്ണസംഖ്യ flags );

തരനിർവചനം ശൂന്യം (*GEMMStoreFunc)( സ്ഥിരം ശൂന്യം* src1, size_t step1,
                   സ്ഥിരം ശൂന്യം* src2, size_t step2, ശൂന്യം* dst, size_t dststep,
                   Size dstsize, ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags );

template<typename T, typename WT, ശൂന്യം (*fn)(സ്ഥിരം T*, size_t, സ്ഥിരം T*, size_t, സ്ഥിരം T*, size_t, T*, size_t, Size, Size, ഇരട്ട, ഇരട്ട, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം ശൂന്യം gemmSingleMulWrap( സ്ഥിരം ശൂന്യം* src1, size_t step1,
                   സ്ഥിരം ശൂന്യം* src2, size_t step2, സ്ഥിരം ശൂന്യം* src3, size_t step3,
                   ശൂന്യം* dst, size_t dststep, Size srcsize, Size dstsize,
                   ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    fn((സ്ഥിരം T*)src1, step1, (സ്ഥിരം T*)src2, step2, (സ്ഥിരം T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, ശൂന്യം (*fn)(സ്ഥിരം T*, size_t, സ്ഥിരം T*, size_t, WT*, size_t, Size, Size, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം ശൂന്യം gemmBlockMulWrap( സ്ഥിരം ശൂന്യം* src1, size_t step1,
                   സ്ഥിരം ശൂന്യം* src2, size_t step2, ശൂന്യം* dst, size_t dststep,
                   Size srcsize, Size dstsize, പൂർണ്ണസംഖ്യ flags )
{
    fn((സ്ഥിരം T*)src1, step1, (സ്ഥിരം T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, ശൂന്യം (*fn)(സ്ഥിരം T*, size_t, സ്ഥിരം WT*, size_t, T*, size_t, Size, ഇരട്ട, ഇരട്ട, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം ശൂന്യം gemmStoreWrap( സ്ഥിരം ശൂന്യം* src1, size_t step1,
                   സ്ഥിരം ശൂന്യം* src2, size_t step2, ശൂന്യം* dst, size_t dststep,
                   Size dstsize, ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    fn((സ്ഥിരം T*)src1, step1, (സ്ഥിരം WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

സ്ഥിതം ശൂന്യം GEMMSingleMul_32f( സ്ഥിരം ദശാംശം* a_data, size_t a_step,
              സ്ഥിരം ദശാംശം* b_data, size_t b_step,
              സ്ഥിരം ദശാംശം* c_data, size_t c_step,
              ദശാംശം* d_data, size_t d_step,
              Size a_size, Size d_size,
              ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    GEMMSingleMul<ദശാംശം,ഇരട്ട>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

സ്ഥിതം ശൂന്യം GEMMSingleMul_64f( സ്ഥിരം ഇരട്ട* a_data, size_t a_step,
                              സ്ഥിരം ഇരട്ട* b_data, size_t b_step,
                              സ്ഥിരം ഇരട്ട* c_data, size_t c_step,
                              ഇരട്ട* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    GEMMSingleMul<ഇരട്ട,ഇരട്ട>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


സ്ഥിതം ശൂന്യം GEMMSingleMul_32fc( സ്ഥിരം Complexf* a_data, size_t a_step,
                              സ്ഥിരം Complexf* b_data, size_t b_step,
                              സ്ഥിരം Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

സ്ഥിതം ശൂന്യം GEMMSingleMul_64fc( സ്ഥിരം Complexd* a_data, size_t a_step,
                              സ്ഥിരം Complexd* b_data, size_t b_step,
                              സ്ഥിരം Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

സ്ഥിതം ശൂന്യം GEMMBlockMul_32f( സ്ഥിരം ദശാംശം* a_data, size_t a_step,
             സ്ഥിരം ദശാംശം* b_data, size_t b_step,
             ഇരട്ട* d_data, size_t d_step,
             Size a_size, Size d_size, പൂർണ്ണസംഖ്യ flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


സ്ഥിതം ശൂന്യം GEMMBlockMul_64f( സ്ഥിരം ഇരട്ട* a_data, size_t a_step,
                             സ്ഥിരം ഇരട്ട* b_data, size_t b_step,
                             ഇരട്ട* d_data, size_t d_step,
                             Size a_size, Size d_size, പൂർണ്ണസംഖ്യ flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


സ്ഥിതം ശൂന്യം GEMMBlockMul_32fc( സ്ഥിരം Complexf* a_data, size_t a_step,
                             സ്ഥിരം Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, പൂർണ്ണസംഖ്യ flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


സ്ഥിതം ശൂന്യം GEMMBlockMul_64fc( സ്ഥിരം Complexd* a_data, size_t a_step,
                             സ്ഥിരം Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, പൂർണ്ണസംഖ്യ flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


സ്ഥിതം ശൂന്യം GEMMStore_32f( സ്ഥിരം ദശാംശം* c_data, size_t c_step,
          സ്ഥിരം ഇരട്ട* d_buf, size_t d_buf_step,
          ദശാംശം* d_data, size_t d_step, Size d_size,
          ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


സ്ഥിതം ശൂന്യം GEMMStore_64f( സ്ഥിരം ഇരട്ട* c_data, size_t c_step,
                      സ്ഥിരം ഇരട്ട* d_buf, size_t d_buf_step,
                      ഇരട്ട* d_data, size_t d_step, Size d_size,
                      ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


സ്ഥിതം ശൂന്യം GEMMStore_32fc( സ്ഥിരം Complexf* c_data, size_t c_step,
                          സ്ഥിരം Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


സ്ഥിതം ശൂന്യം GEMMStore_64fc( സ്ഥിരം Complexd* c_data, size_t c_step,
                          സ്ഥിരം Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          ഇരട്ട alpha, ഇരട്ട beta, പൂർണ്ണസംഖ്യ flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

സ്ഥിതം ശൂന്യം gemmImpl( Mat A, Mat B, ഇരട്ട alpha,
           Mat C, ഇരട്ട beta, Mat D, പൂർണ്ണസംഖ്യ flags )
{
    CV_INSTRUMENT_REGION();

    സ്ഥിരം പൂർണ്ണസംഖ്യ block_lin_size = 128;
    സ്ഥിരം പൂർണ്ണസംഖ്യ block_size = block_lin_size * block_lin_size;

    സ്ഥിതം ഇരട്ട zero[] = {0,0,0,0};
    സ്ഥിതം ദശാംശം zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    പൂർണ്ണസംഖ്യ i, len = 0, type = A.type();

    തിരഞ്ഞെടുപ്പ്( flags & (GEMM_1_T|GEMM_2_T) )
    {
    അവസ്ഥ 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        നിർത്തുക;
    അവസ്ഥ 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        നിർത്തുക;
    അവസ്ഥ 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        നിർത്തുക;
    അവസ്ഥ 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        നിർത്തുക;
    }

    എങ്കിൽ( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        എങ്കിൽ( type == CV_32F )
        {
            ദശാംശം* d = D.ptr<ദശാംശം>();
            സ്ഥിരം ദശാംശം *a = A.ptr<ദശാംശം>(),
                        *b = B.ptr<ദശാംശം>(),
                        *c = (സ്ഥിരം ദശാംശം*)C.data;
            size_t d_step = D.step/വലുപ്പം(d[0]),
                a_step = A.step/വലുപ്പം(a[0]),
                b_step = B.step/വലുപ്പം(b[0]),
                c_step = C.data ? C.step/വലുപ്പം(c[0]) : 0;

            എങ്കിൽ( !c )
                c = zerof;

            തിരഞ്ഞെടുപ്പ്( len )
            {
            അവസ്ഥ 2:
                എങ്കിൽ( len == d_size.width && b != d )
                {
                    വേണ്ടി( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ദശാംശം t0 = a[0]*b[0] + a[1]*b[b_step];
                        ദശാംശം t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (ദശാംശം)(t0*alpha + c[0]*beta);
                        d[1] = (ദശാംശം)(t1*alpha + c[1]*beta);
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ( a != d )
                {
                    പൂർണ്ണസംഖ്യ c_step0 = 1;
                    എങ്കിൽ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    വേണ്ടി( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ദശാംശം t0 = a[0]*b[0] + a[1]*b[b_step];
                        ദശാംശം t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (ദശാംശം)(t0*alpha + c[0]*beta);
                        d[d_step] = (ദശാംശം)(t1*alpha + c[c_step]*beta);
                    }
                }
                അല്ലെങ്കിൽ
                    നിർത്തുക;
                തിരികെ;
            അവസ്ഥ 3:
                എങ്കിൽ( len == d_size.width && b != d )
                {
                    വേണ്ടി( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ദശാംശം t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ദശാംശം t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        ദശാംശം t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (ദശാംശം)(t0*alpha + c[0]*beta);
                        d[1] = (ദശാംശം)(t1*alpha + c[1]*beta);
                        d[2] = (ദശാംശം)(t2*alpha + c[2]*beta);
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ( a != d )
                {
                    പൂർണ്ണസംഖ്യ c_step0 = 1;
                    എങ്കിൽ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    വേണ്ടി( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ദശാംശം t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ദശാംശം t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        ദശാംശം t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (ദശാംശം)(t0*alpha + c[0]*beta);
                        d[d_step] = (ദശാംശം)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (ദശാംശം)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                അല്ലെങ്കിൽ
                    നിർത്തുക;
                തിരികെ;
            അവസ്ഥ 4:
                എങ്കിൽ( len == d_size.width && b != d )
                {
                    വേണ്ടി( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ദശാംശം t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ദശാംശം t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        ദശാംശം t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        ദശാംശം t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (ദശാംശം)(t0*alpha + c[0]*beta);
                        d[1] = (ദശാംശം)(t1*alpha + c[1]*beta);
                        d[2] = (ദശാംശം)(t2*alpha + c[2]*beta);
                        d[3] = (ദശാംശം)(t3*alpha + c[3]*beta);
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ( len <= 16 && a != d )
                {
                    പൂർണ്ണസംഖ്യ c_step0 = 1;
                    എങ്കിൽ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    വേണ്ടി( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ദശാംശം t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ദശാംശം t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        ദശാംശം t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        ദശാംശം t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (ദശാംശം)(t0*alpha + c[0]*beta);
                        d[d_step] = (ദശാംശം)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (ദശാംശം)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (ദശാംശം)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                അല്ലെങ്കിൽ
                    നിർത്തുക;
                തിരികെ;
            }
        }

        എങ്കിൽ( type == CV_64F )
        {
            ഇരട്ട* d = D.ptr<ഇരട്ട>();
            സ്ഥിരം ഇരട്ട *a = A.ptr<ഇരട്ട>(),
                         *b = B.ptr<ഇരട്ട>(),
                         *c = (സ്ഥിരം ഇരട്ട*)C.data;
            size_t d_step = D.step/വലുപ്പം(d[0]),
                a_step = A.step/വലുപ്പം(a[0]),
                b_step = B.step/വലുപ്പം(b[0]),
                c_step = C.data ? C.step/വലുപ്പം(c[0]) : 0;
            എങ്കിൽ( !c )
                c = zero;

            തിരഞ്ഞെടുപ്പ്( len )
            {
            അവസ്ഥ 2:
                എങ്കിൽ( len == d_size.width && b != d )
                {
                    വേണ്ടി( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ഇരട്ട t0 = a[0]*b[0] + a[1]*b[b_step];
                        ഇരട്ട t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ( a != d )
                {
                    പൂർണ്ണസംഖ്യ c_step0 = 1;
                    എങ്കിൽ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    വേണ്ടി( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ഇരട്ട t0 = a[0]*b[0] + a[1]*b[b_step];
                        ഇരട്ട t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                അല്ലെങ്കിൽ
                    നിർത്തുക;
                തിരികെ;
            അവസ്ഥ 3:
                എങ്കിൽ( len == d_size.width && b != d )
                {
                    വേണ്ടി( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ഇരട്ട t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ഇരട്ട t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        ഇരട്ട t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ( a != d )
                {
                    പൂർണ്ണസംഖ്യ c_step0 = 1;
                    എങ്കിൽ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    വേണ്ടി( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ഇരട്ട t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ഇരട്ട t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        ഇരട്ട t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                അല്ലെങ്കിൽ
                    നിർത്തുക;
                തിരികെ;
            അവസ്ഥ 4:
                എങ്കിൽ( len == d_size.width && b != d )
                {
                    വേണ്ടി( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ഇരട്ട t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ഇരട്ട t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        ഇരട്ട t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        ഇരട്ട t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ( d_size.width <= 16 && a != d )
                {
                    പൂർണ്ണസംഖ്യ c_step0 = 1;
                    എങ്കിൽ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    വേണ്ടി( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ഇരട്ട t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ഇരട്ട t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        ഇരട്ട t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        ഇരട്ട t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                അല്ലെങ്കിൽ
                    നിർത്തുക;
                തിരികെ;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    സ്ഥിരം uchar* Cdata = C.data;
    size_t Cstep = C.data ? (size_t)C.step : 0;
    AutoBuffer<uchar> buf;

    എങ്കിൽ( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ദശാംശം, ഇരട്ട, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<ദശാംശം, ഇരട്ട, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<ദശാംശം, ഇരട്ട, GEMMStore_32f>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ഇരട്ട, ഇരട്ട, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<ഇരട്ട, ഇരട്ട, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<ഇരട്ട, ഇരട്ട, GEMMStore_64f>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    അല്ലെങ്കിൽ
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    എങ്കിൽ( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
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
    else*/ എങ്കിൽ( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ is_a_t = flags & GEMM_1_T;
        പൂർണ്ണസംഖ്യ is_b_t = flags & GEMM_2_T;
        പൂർണ്ണസംഖ്യ elem_size = CV_ELEM_SIZE(type);
        പൂർണ്ണസംഖ്യ dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        പൂർണ്ണസംഖ്യ j, k, di = 0, dj = 0, dk = 0;
        പൂർണ്ണസംഖ്യ dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        പൂർണ്ണസംഖ്യ work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        എങ്കിൽ( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        അല്ലെങ്കിൽ
            a_step0 = elem_size, a_step1 = A.step;

        എങ്കിൽ( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        അല്ലെങ്കിൽ
            b_step0 = elem_size, b_step1 = b_step;

        എങ്കിൽ( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        അല്ലെങ്കിൽ
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        എങ്കിൽ( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        എങ്കിൽ( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        എങ്കിൽ( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        എങ്കിൽ( is_a_t )
            a_buf = b_buf + b_buf_size;

        വേണ്ടി( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            എങ്കിൽ( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            വേണ്ടി( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                സ്ഥിരം uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                എങ്കിൽ( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                എങ്കിൽ( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                വേണ്ടി( k = 0; k < len; k += dk )
                {
                    സ്ഥിരം uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    സ്ഥിരം uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    എങ്കിൽ( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    എങ്കിൽ( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    അല്ലെങ്കിൽ
                        a_bl_size.width = di, a_bl_size.height = dk;

                    എങ്കിൽ( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    എങ്കിൽ( dj < d_size.width )
                    {
                        Size b_size;
                        എങ്കിൽ( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        അല്ലെങ്കിൽ
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    എങ്കിൽ( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    അല്ലെങ്കിൽ
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                എങ്കിൽ( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>വരിയിൽ സ്ഥിതം ശൂന്യം
callGemmImpl(സ്ഥിരം fptype *src1, size_t src1_step, സ്ഥിരം fptype *src2, size_t src2_step, fptype alpha,
          സ്ഥിരം fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    പൂർണ്ണസംഖ്യ b_m, b_n, c_m, c_n, m_d;

    എങ്കിൽ(flags & GEMM_2_T)
    {
        b_m = n_d;
        എങ്കിൽ(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        അല്ലെങ്കിൽ
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    അല്ലെങ്കിൽ
    {
        b_n = n_d;
        എങ്കിൽ(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        അല്ലെങ്കിൽ
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    എങ്കിൽ(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    അല്ലെങ്കിൽ
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    എങ്കിൽ(src1 != ശൂന്യ)
        A = Mat(m_a, n_a, type, (ശൂന്യം*)src1, src1_step);
    എങ്കിൽ(src2 != ശൂന്യ)
        B = Mat(b_m, b_n, type, (ശൂന്യം*)src2, src2_step);
    എങ്കിൽ(src3 != ശൂന്യ && beta != 0.0)
        C = Mat(c_m, c_n, type, (ശൂന്യം*)src3, src3_step);
    Mat D(m_d, n_d, type, (ശൂന്യം*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

ശൂന്യം gemm32f(സ്ഥിരം ദശാംശം* src1, size_t src1_step, സ്ഥിരം ദശാംശം* src2, size_t src2_step,
             ദശാംശം alpha, സ്ഥിരം ദശാംശം* src3, size_t src3_step, ദശാംശം beta, ദശാംശം* dst, size_t dst_step,
             പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

ശൂന്യം gemm64f(സ്ഥിരം ഇരട്ട* src1, size_t src1_step, സ്ഥിരം ഇരട്ട* src2, size_t src2_step,
             ഇരട്ട alpha, സ്ഥിരം ഇരട്ട* src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട* dst, size_t dst_step,
             പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

ശൂന്യം gemm32fc(സ്ഥിരം ദശാംശം* src1, size_t src1_step, സ്ഥിരം ദശാംശം* src2, size_t src2_step,
              ദശാംശം alpha, സ്ഥിരം ദശാംശം* src3, size_t src3_step, ദശാംശം beta, ദശാംശം* dst, size_t dst_step,
              പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

ശൂന്യം gemm64fc(സ്ഥിരം ഇരട്ട* src1, size_t src1_step, സ്ഥിരം ഇരട്ട* src2, size_t src2_step,
              ഇരട്ട alpha, സ്ഥിരം ഇരട്ട* src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട* dst, size_t dst_step,
              പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> സ്ഥിതം ശൂന്യം
transform_( സ്ഥിരം T* src, T* dst, സ്ഥിരം WT* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn )
{
    പൂർണ്ണസംഖ്യ x;

    എങ്കിൽ( scn == 2 && dcn == 2 )
    {
        വേണ്ടി( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( scn == 3 && dcn == 3 )
    {
        വേണ്ടി( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( scn == 3 && dcn == 1 )
    {
        വേണ്ടി( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( scn == 4 && dcn == 4 )
    {
        വേണ്ടി( x = 0; x < len*4; x += 4 )
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
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            സ്ഥിരം WT* _m = m;
            പൂർണ്ണസംഖ്യ j, k;
            വേണ്ടി( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                വേണ്ടി( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

സ്ഥിതം ശൂന്യം
transform_8u( സ്ഥിരം uchar* src, uchar* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn )
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ BITS = 10, SCALE = 1 << BITS;
    സ്ഥിരം ദശാംശം MAX_M = (ദശാംശം)(1 << (15 - BITS));

    എങ്കിൽ( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ nChannels = 3;

        സംഘം {
            short s[6];
            പൂർണ്ണസംഖ്യ p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        പൂർണ്ണസംഖ്യ m32[] = {saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 2] * SCALE), saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 3] * SCALE),
                     saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 6] * SCALE), saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 7] * SCALE),
                     saturate_cast<പൂർണ്ണസംഖ്യ>(m[10] * SCALE), saturate_cast<പൂർണ്ണസംഖ്യ>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        പൂർണ്ണസംഖ്യ x = 0;
        വേണ്ടി (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<പൂർണ്ണസംഖ്യ>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<പൂർണ്ണസംഖ്യ>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<പൂർണ്ണസംഖ്യ>((m[11] + 0.5f)*SCALE);
        വേണ്ടി( ; x < len * nChannels; x += nChannels )
        {
            പൂർണ്ണസംഖ്യ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
transform_16u( സ്ഥിരം ushort* src, ushort* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn )
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ( scn == 3 && dcn == 3 )
    {
        പൂർണ്ണസംഖ്യ x = 0;
#എങ്കിൽ CV_SIMD_WIDTH > 16
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
        വേണ്ടി (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#എങ്കിൽ CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_ഇടത്<1>(_m0l);
        v_float32x4 _m1h = v_rotate_ഇടത്<1>(_m1l);
        v_float32x4 _m2h = v_rotate_ഇടത്<1>(_m2l);
        v_float32x4 _m3h = v_rotate_ഇടത്<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        വേണ്ടി( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_വലത്<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        വേണ്ടി( ; x < len * 3; x += 3 )
        {
            ദശാംശം v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
transform_32f( സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    പൂർണ്ണസംഖ്യ x = 0;
    എങ്കിൽ( scn == 3 && dcn == 3 )
    {
        പൂർണ്ണസംഖ്യ idx[VTraits<v_float32>::max_nlanes/2];
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        ദശാംശം _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        വേണ്ടി( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        വേണ്ടി( ; x < len*3; x += 3 )
        {
            ദശാംശം v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            ദശാംശം t0 = saturate_cast<ദശാംശം>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            ദശാംശം t1 = saturate_cast<ദശാംശം>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            ദശാംശം t2 = saturate_cast<ദശാംശം>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        തിരികെ;
    }

    എങ്കിൽ( scn == 4 && dcn == 4 )
    {
#എങ്കിൽ CV_SIMD_WIDTH > 16
        പൂർണ്ണസംഖ്യ idx[VTraits<v_float32>::max_nlanes/4];
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        ദശാംശം _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        വേണ്ടി( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#എങ്കിൽ CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        വേണ്ടി( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#അല്ലെങ്കിൽ // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        വേണ്ടി( ; x < len*4; x += 4 )
        {
            ദശാംശം v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            ദശാംശം t0 = saturate_cast<ദശാംശം>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            ദശാംശം t1 = saturate_cast<ദശാംശം>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            ദശാംശം t2 = saturate_cast<ദശാംശം>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            ദശാംശം t3 = saturate_cast<ദശാംശം>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        തിരികെ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


സ്ഥിതം ശൂന്യം
transform_8s(സ്ഥിരം schar* src, schar* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ BITS = 10, SCALE = 1 << BITS;
    സ്ഥിരം ദശാംശം MAX_M = (ദശാംശം)(1 << (15 - BITS));

    എങ്കിൽ( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ nChannels = 3;

        സംഘം {
            short s[6];
            പൂർണ്ണസംഖ്യ p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        പൂർണ്ണസംഖ്യ m32[] = {saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 2] * SCALE), saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 3] * SCALE),
                     saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 6] * SCALE), saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 7] * SCALE),
                     saturate_cast<പൂർണ്ണസംഖ്യ>(m[10] * SCALE), saturate_cast<പൂർണ്ണസംഖ്യ>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        പൂർണ്ണസംഖ്യ x = 0;
        വേണ്ടി (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<പൂർണ്ണസംഖ്യ>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<പൂർണ്ണസംഖ്യ>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<പൂർണ്ണസംഖ്യ>((m[11] + 0.5f)*SCALE);
        വേണ്ടി( ; x < len * nChannels; x += nChannels )
        {
            പൂർണ്ണസംഖ്യ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
transform_16s(സ്ഥിരം short* src, short* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ (scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

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
        വേണ്ടി (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        വേണ്ടി( ; x < len * 3; x += 3 )
        {
            ദശാംശം v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
transform_32s(സ്ഥിരം പൂർണ്ണസംഖ്യ* src, പൂർണ്ണസംഖ്യ* dst, സ്ഥിരം ഇരട്ട* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    എങ്കിൽ( scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

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
        വേണ്ടി (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_മുകളിലെ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_മുകളിലെ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_മുകളിലെ(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        വേണ്ടി (; x < len * 3; x += 3) {
            ഇരട്ട b = src[x], g = src[x + 1], r = src[x + 2];
            പൂർണ്ണസംഖ്യ db = saturate_cast<പൂർണ്ണസംഖ്യ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            പൂർണ്ണസംഖ്യ dg = saturate_cast<പൂർണ്ണസംഖ്യ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            പൂർണ്ണസംഖ്യ dr = saturate_cast<പൂർണ്ണസംഖ്യ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
transform_64f(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, സ്ഥിരം ഇരട്ട* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    എങ്കിൽ( scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

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
        വേണ്ടി (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        വേണ്ടി (; x < len * 3; x += 3) {
            ഇരട്ട b = src[x], g = src[x + 1], r = src[x + 2];
            ഇരട്ട db = saturate_cast<ഇരട്ട>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ഇരട്ട dg = saturate_cast<ഇരട്ട>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ഇരട്ട dr = saturate_cast<ഇരട്ട>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> സ്ഥിതം ശൂന്യം
diagtransform_( സ്ഥിരം T* src, T* dst, സ്ഥിരം WT* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ )
{
    പൂർണ്ണസംഖ്യ x;

    എങ്കിൽ( cn == 2 )
    {
        വേണ്ടി( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( cn == 3 )
    {
        വേണ്ടി( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( cn == 4 )
    {
        വേണ്ടി( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( x = 0; x < len; x++, src += cn, dst += cn )
        {
            സ്ഥിരം WT* _m = m;
            വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

സ്ഥിതം ശൂന്യം
diagtransform_8u(സ്ഥിരം uchar* src, uchar* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ (scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        വേണ്ടി (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        വേണ്ടി (; x < len * 3; x += 3) {
            പൂർണ്ണസംഖ്യ b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
diagtransform_8s(സ്ഥിരം schar* src, schar* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
    #എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ (scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        വേണ്ടി (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        വേണ്ടി (; x < len * 3; x += 3) {
            പൂർണ്ണസംഖ്യ b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
diagtransform_16u(സ്ഥിരം ushort* src, ushort* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ (scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        വേണ്ടി (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        വേണ്ടി (; x < len * 3; x += 3) {
            പൂർണ്ണസംഖ്യ b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
diagtransform_16s(സ്ഥിരം short* src, short* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ (scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        വേണ്ടി (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        വേണ്ടി (; x < len * 3; x += 3) {
            പൂർണ്ണസംഖ്യ b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
diagtransform_32s(സ്ഥിരം പൂർണ്ണസംഖ്യ* src, പൂർണ്ണസംഖ്യ* dst, സ്ഥിരം ഇരട്ട* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    എങ്കിൽ (scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        വേണ്ടി (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_മുകളിലെ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_മുകളിലെ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_മുകളിലെ(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        വേണ്ടി (; x < len * 3; x += 3) {
            പൂർണ്ണസംഖ്യ b = src[x], g = src[x + 1], r = src[x + 2];
            പൂർണ്ണസംഖ്യ db = saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 0] * b + m[ 3]);
            പൂർണ്ണസംഖ്യ dg = saturate_cast<പൂർണ്ണസംഖ്യ>(m[ 5] * g + m[ 7]);
            പൂർണ്ണസംഖ്യ dr = saturate_cast<പൂർണ്ണസംഖ്യ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
diagtransform_32f(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, സ്ഥിരം ദശാംശം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ (scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        വേണ്ടി (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        വേണ്ടി (; x < len * 3; x += 3) {
            ദശാംശം b = src[x], g = src[x + 1], r = src[x + 2];
            ദശാംശം db = saturate_cast<ദശാംശം>(m[ 0] * b + m[ 3]);
            ദശാംശം dg = saturate_cast<ദശാംശം>(m[ 5] * g + m[ 7]);
            ദശാംശം dr = saturate_cast<ദശാംശം>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

സ്ഥിതം ശൂന്യം
diagtransform_64f(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, സ്ഥിരം ഇരട്ട* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    എങ്കിൽ (scn == 3 && dcn == 3) {
        പൂർണ്ണസംഖ്യ x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        വേണ്ടി (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        വേണ്ടി (; x < len * 3; x += 3) {
            ഇരട്ട b = src[x], g = src[x + 1], r = src[x + 2];
            ഇരട്ട db = saturate_cast<ഇരട്ട>(m[ 0] * b + m[ 3]);
            ഇരട്ട dg = saturate_cast<ഇരട്ട>(m[ 5] * g + m[ 7]);
            ഇരട്ട dr = saturate_cast<ഇരട്ട>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        തിരികെ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, ശൂന്യം (*fn)(സ്ഥിരം T*, T*, സ്ഥിരം WT*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം ശൂന്യം transformWrap(സ്ഥിരം ശൂന്യം* src, ശൂന്യം* dst, സ്ഥിരം ശൂന്യം* m, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn)
{
    fn((സ്ഥിരം T*)src, (T*)dst, (സ്ഥിരം WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, ദശാംശം, transform_8u>, transformWrap<schar, ദശാംശം, transform_8s>, transformWrap<ushort, ദശാംശം, transform_16u>,
        transformWrap<short, ദശാംശം, transform_16s>, transformWrap<പൂർണ്ണസംഖ്യ, ഇരട്ട, transform_32s>, transformWrap<ദശാംശം, ദശാംശം, transform_32f>,
        transformWrap<ഇരട്ട, ഇരട്ട, transform_64f>, 0
    };

    തിരികെ transformTab[depth];
}

TransformFunc getDiagTransformFunc(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, ദശാംശം, diagtransform_8u>, transformWrap<schar, ദശാംശം, diagtransform_8s>, transformWrap<ushort, ദശാംശം, diagtransform_16u>,
        transformWrap<short, ദശാംശം, diagtransform_16s>, transformWrap<പൂർണ്ണസംഖ്യ, ഇരട്ട, diagtransform_32s>, transformWrap<ദശാംശം, ദശാംശം, diagtransform_32f>,
        transformWrap<ഇരട്ട, ഇരട്ട, diagtransform_64f>, 0
    };

    തിരികെ diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> സ്ഥിതം ശൂന്യം
perspectiveTransform_( സ്ഥിരം ശൂന്യം* _src, ശൂന്യം* _dst, സ്ഥിരം ശൂന്യം* _m_ptr, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ scn, പൂർണ്ണസംഖ്യ dcn )
{
    സ്ഥിരം T* src = (സ്ഥിരം T*)_src;
    T* dst = (T*)_dst;
    സ്ഥിരം ഇരട്ട* m = (സ്ഥിരം ഇരട്ട*)_m_ptr;
    സ്ഥിരം ഇരട്ട eps = FLT_EPSILON;
    പൂർണ്ണസംഖ്യ i;

    എങ്കിൽ( scn == 2 && dcn == 2 )
    {
        വേണ്ടി( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            ഇരട്ട w = x*m[6] + y*m[7] + m[8];

            എങ്കിൽ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            അല്ലെങ്കിൽ
                dst[i] = dst[i+1] = (T)0;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( scn == 3 && dcn == 3 )
    {
        വേണ്ടി( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            ഇരട്ട w = x*m[12] + y*m[13] + z*m[14] + m[15];

            എങ്കിൽ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            അല്ലെങ്കിൽ
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( scn == 3 && dcn == 2 )
    {
        വേണ്ടി( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            ഇരട്ട w = x*m[8] + y*m[9] + z*m[10] + m[11];

            എങ്കിൽ( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            അല്ലെങ്കിൽ
                dst[0] = dst[1] = (T)0;
        }
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            സ്ഥിരം ഇരട്ട* _m = m + dcn*(scn + 1);
            ഇരട്ട w = _m[scn];
            പൂർണ്ണസംഖ്യ j, k;
            വേണ്ടി( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            എങ്കിൽ( fabs(w) > eps )
            {
                _m = m;
                വേണ്ടി( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    ഇരട്ട s = _m[scn];
                    വേണ്ടി( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            അല്ലെങ്കിൽ
                വേണ്ടി( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(പൂർണ്ണസംഖ്യ depth)
{
    എങ്കിൽ (depth == CV_32F)
        തിരികെ perspectiveTransform_<ദശാംശം>;
    എങ്കിൽ (depth == CV_64F)
        തിരികെ perspectiveTransform_<ഇരട്ട>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

സ്ഥിതം ശൂന്യം scaleAdd_32f(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, ശൂന്യം* _dst,
                         പൂർണ്ണസംഖ്യ len, സ്ഥിരം ശൂന്യം* _alpha)
{
    സ്ഥിരം ദശാംശം* src1 = (സ്ഥിരം ദശാംശം*)_src1;
    സ്ഥിരം ദശാംശം* src2 = (സ്ഥിരം ദശാംശം*)_src2;
    ദശാംശം* dst = (ദശാംശം*)_dst;
    ദശാംശം alpha = *(സ്ഥിരം ദശാംശം*)_alpha;
    പൂർണ്ണസംഖ്യ i = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    സ്ഥിരം പൂർണ്ണസംഖ്യ cWidth = VTraits<v_float32>::vlanes();
    വേണ്ടി (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    വേണ്ടി (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


സ്ഥിതം ശൂന്യം scaleAdd_64f(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, ശൂന്യം* _dst,
                         പൂർണ്ണസംഖ്യ len, സ്ഥിരം ശൂന്യം* _alpha)
{
    സ്ഥിരം ഇരട്ട* src1 = (സ്ഥിരം ഇരട്ട*)_src1;
    സ്ഥിരം ഇരട്ട* src2 = (സ്ഥിരം ഇരട്ട*)_src2;
    ഇരട്ട* dst = (ഇരട്ട*)_dst;
    ഇരട്ട alpha = *(സ്ഥിരം ഇരട്ട*)_alpha;
    പൂർണ്ണസംഖ്യ i = 0;
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    സ്ഥിരം പൂർണ്ണസംഖ്യ cWidth = VTraits<v_float64>::vlanes();
    വേണ്ടി (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    വേണ്ടി (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(പൂർണ്ണസംഖ്യ depth)
{
    എങ്കിൽ (depth == CV_32F)
        തിരികെ scaleAdd_32f;
    എങ്കിൽ (depth == CV_64F)
        തിരികെ scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> സ്ഥിതം വരിയിൽ
ഇരട്ട MahalanobisImpl(സ്ഥിരം Mat& v1, സ്ഥിരം Mat& v2, സ്ഥിരം Mat& icovar, ഇരട്ട *diff_buffer /*[len]*/, പൂർണ്ണസംഖ്യ len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    ഇരട്ട result = 0;

    sz.width *= v1.channels();
    എങ്കിൽ (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        സ്ഥിരം T* src1 = v1.ptr<T>();
        സ്ഥിരം T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/വലുപ്പം(src1[0]);
        size_t step2 = v2.step/വലുപ്പം(src2[0]);
        ഇരട്ട* diff = diff_buffer;
        സ്ഥിരം T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/വലുപ്പം(mat[0]);

        വേണ്ടി (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++, mat += matstep)
        {
            ഇരട്ട row_sum = 0;
            പൂർണ്ണസംഖ്യ j = 0;
#എങ്കിൽ CV_ENABLE_UNROLLED
            വേണ്ടി(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            വേണ്ടി (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    തിരികെ result;
}

MahalanobisImplFunc getMahalanobisImplFunc(പൂർണ്ണസംഖ്യ depth)
{
    എങ്കിൽ (depth == CV_32F)
        തിരികെ (MahalanobisImplFunc)MahalanobisImpl<ദശാംശം>;
    എങ്കിൽ (depth == CV_64F)
        തിരികെ (MahalanobisImplFunc)MahalanobisImpl<ഇരട്ട>;
    CV_Assert(0 && "Not supported");
}


#എങ്കിൽ !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> സ്ഥിതം ശൂന്യം
MulTransposedR(സ്ഥിരം Mat& srcmat, സ്ഥിരം Mat& dstmat, സ്ഥിരം Mat& deltamat, ഇരട്ട scale)
{
    പൂർണ്ണസംഖ്യ i, j, k;
    സ്ഥിരം sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    സ്ഥിരം dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/വലുപ്പം(src[0]);
    size_t dststep = dstmat.step/വലുപ്പം(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/വലുപ്പം(delta[0]) : 0;
    പൂർണ്ണസംഖ്യ delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    പൂർണ്ണസംഖ്യ buf_size = size.height*വലുപ്പം(dT);
    AutoBuffer<uchar> buf;

    എങ്കിൽ( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    എങ്കിൽ( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        വേണ്ടി( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#എങ്കിൽ CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    എങ്കിൽ( !delta )
        വേണ്ടി( i = 0; i < size.width; i++, tdst += dststep )
        {
            വേണ്ടി( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            വേണ്ടി( j = i; j <= size.width - 4; j += 4 )
            {
#എങ്കിൽ CV_SIMD128_64F
                എങ്കിൽ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    സ്ഥിരം ഇരട്ട *tsrc = (ഇരട്ട*)(src + j);

                    വേണ്ടി( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((ഇരട്ട)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((ഇരട്ട*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ഇരട്ട*)(tdst+j+2), v_mul(s1, v_scale));
                } അല്ലെങ്കിൽ
#endif
                {
                    ഇരട്ട s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    സ്ഥിരം sT *tsrc = src + j;

                    വേണ്ടി( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        ഇരട്ട a = col_buf[k];
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

            വേണ്ടി( ; j < size.width; j++ )
            {
                ഇരട്ട s0 = 0;
                സ്ഥിരം sT *tsrc = src + j;

                വേണ്ടി( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (ഇരട്ട)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    അല്ലെങ്കിൽ
        വേണ്ടി( i = 0; i < size.width; i++, tdst += dststep )
        {
            എങ്കിൽ( !delta_buf )
                വേണ്ടി( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            അല്ലെങ്കിൽ
                വേണ്ടി( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            വേണ്ടി( j = i; j <= size.width - 4; j += 4 )
            {
#എങ്കിൽ CV_SIMD128_64F
                എങ്കിൽ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    സ്ഥിരം ഇരട്ട *tsrc = (ഇരട്ട*)(src + j);
                    സ്ഥിരം ഇരട്ട *d = (ഇരട്ട*)(delta_buf ? delta_buf : delta + j);

                    വേണ്ടി( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((ഇരട്ട)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((ഇരട്ട*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ഇരട്ട*)(tdst+j+2), v_mul(s1, v_scale));
                }
                അല്ലെങ്കിൽ
#endif

                {
                    ഇരട്ട s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    സ്ഥിരം sT *tsrc = src + j;
                    സ്ഥിരം dT *d = delta_buf ? delta_buf : delta + j;

                    വേണ്ടി( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        ഇരട്ട a = col_buf[k];
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

            വേണ്ടി( ; j < size.width; j++ )
            {
                ഇരട്ട s0 = 0;
                സ്ഥിരം sT *tsrc = src + j;
                സ്ഥിരം dT *d = delta_buf ? delta_buf : delta + j;

                വേണ്ടി( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (ഇരട്ട)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> സ്ഥിതം ശൂന്യം
MulTransposedL(സ്ഥിരം Mat& srcmat, സ്ഥിരം Mat& dstmat, സ്ഥിരം Mat& deltamat, ഇരട്ട scale)
{
    പൂർണ്ണസംഖ്യ i, j, k;
    സ്ഥിരം sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    സ്ഥിരം dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/വലുപ്പം(src[0]);
    size_t dststep = dstmat.step/വലുപ്പം(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/വലുപ്പം(delta[0]) : 0;
    പൂർണ്ണസംഖ്യ delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    എങ്കിൽ( !delta )
        വേണ്ടി( i = 0; i < size.height; i++, tdst += dststep )
            വേണ്ടി( j = i; j < size.height; j++ )
            {
                ഇരട്ട s = 0;
                സ്ഥിരം sT *tsrc1 = src + i*srcstep;
                സ്ഥിരം sT *tsrc2 = src + j*srcstep;
#എങ്കിൽ CV_SIMD128_64F
                എങ്കിൽ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    സ്ഥിരം ഇരട്ട *v_tsrc1 = (ഇരട്ട *)(tsrc1);
                    സ്ഥിരം ഇരട്ട *v_tsrc2 = (ഇരട്ട *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    വേണ്ടി( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                അല്ലെങ്കിൽ
#endif
                {
                    വേണ്ടി( k = 0; k <= size.width - 4; k += 4 )
                        s += (ഇരട്ട)tsrc1[k]*tsrc2[k] + (ഇരട്ട)tsrc1[k+1]*tsrc2[k+1] +
                             (ഇരട്ട)tsrc1[k+2]*tsrc2[k+2] + (ഇരട്ട)tsrc1[k+3]*tsrc2[k+3];
                }
                വേണ്ടി( ; k < size.width; k++ )
                    s += (ഇരട്ട)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    അല്ലെങ്കിൽ
    {
        dT delta_buf[4];
        പൂർണ്ണസംഖ്യ delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*വലുപ്പം(dT));
        dT* row_buf = (dT*)buf.data();

        വേണ്ടി( i = 0; i < size.height; i++, tdst += dststep )
        {
            സ്ഥിരം sT *tsrc1 = src + i*srcstep;
            സ്ഥിരം dT *tdelta1 = delta + i*deltastep;

            എങ്കിൽ( delta_cols < size.width )
                വേണ്ടി( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            അല്ലെങ്കിൽ
                വേണ്ടി( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            വേണ്ടി( j = i; j < size.height; j++ )
            {
                ഇരട്ട s = 0;
                സ്ഥിരം sT *tsrc2 = src + j*srcstep;
                സ്ഥിരം dT *tdelta2 = delta + j*deltastep;
                എങ്കിൽ( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#എങ്കിൽ CV_SIMD128_64F
                എങ്കിൽ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    സ്ഥിരം ഇരട്ട *v_tsrc2 = (ഇരട്ട *)(tsrc2);
                    സ്ഥിരം ഇരട്ട *v_tdelta2 = (ഇരട്ട *)(tdelta2);
                    സ്ഥിരം ഇരട്ട *v_row_buf = (ഇരട്ട *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    വേണ്ടി( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (സ്ഥിരം dT *)(v_tdelta2);
                }
                അല്ലെങ്കിൽ
#endif
                {
                    വേണ്ടി( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (ഇരട്ട)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (ഇരട്ട)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (ഇരട്ട)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (ഇരട്ട)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                വേണ്ടി( ; k < size.width; k++, tdelta2++ )
                    s += (ഇരട്ട)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(പൂർണ്ണസംഖ്യ stype, പൂർണ്ണസംഖ്യ dtype, തർക്കം ata)
{
    MulTransposedFunc func = ശൂന്യ;
    എങ്കിൽ (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,ദശാംശം>
                   : MulTransposedL<uchar,ദശാംശം>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,ഇരട്ട>
                   : MulTransposedL<uchar,ഇരട്ട>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,ദശാംശം>
                   : MulTransposedL<ushort,ദശാംശം>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,ഇരട്ട>
                   : MulTransposedL<ushort,ഇരട്ട>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,ദശാംശം>
                   : MulTransposedL<short,ദശാംശം>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,ഇരട്ട>
                   : MulTransposedL<short,ഇരട്ട>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ദശാംശം,ദശാംശം>
                   : MulTransposedL<ദശാംശം,ദശാംശം>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ദശാംശം,ഇരട്ട>
                   : MulTransposedL<ദശാംശം,ഇരട്ട>;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ഇരട്ട,ഇരട്ട>
                   : MulTransposedL<ഇരട്ട,ഇരട്ട>;
    }
    CV_Assert(func && "Not supported");
    തിരികെ func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> സ്ഥിതം വരിയിൽ
ഇരട്ട dotProd_(സ്ഥിരം T* src1, സ്ഥിരം T* src2, പൂർണ്ണസംഖ്യ len)
{
    പൂർണ്ണസംഖ്യ i = 0;
    ഇരട്ട result = 0;

#എങ്കിൽ CV_ENABLE_UNROLLED
    വേണ്ടി( ; i <= len - 4; i += 4 )
        result += (ഇരട്ട)src1[i]*src2[i] + (ഇരട്ട)src1[i+1]*src2[i+1] +
            (ഇരട്ട)src1[i+2]*src2[i+2] + (ഇരട്ട)src1[i+3]*src2[i+3];
#endif
    വേണ്ടി( ; i < len; i++ )
        result += (ഇരട്ട)src1[i]*src2[i];

    തിരികെ result;
}


ഇരട്ട dotProd_8u(സ്ഥിരം uchar* src1, സ്ഥിരം uchar* src2, പൂർണ്ണസംഖ്യ len)
{
    ഇരട്ട r = 0;
    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    പൂർണ്ണസംഖ്യ len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    ആകുമ്പോൾ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        സ്ഥിരം പൂർണ്ണസംഖ്യ cWidth = VTraits<v_uint16>::vlanes();

        പൂർണ്ണസംഖ്യ j = 0;
        വേണ്ടി (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        വേണ്ടി (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (ഇരട്ട)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    തിരികെ r + dotProd_(src1, src2, len - i);
}


ഇരട്ട dotProd_8s(സ്ഥിരം schar* src1, സ്ഥിരം schar* src2, പൂർണ്ണസംഖ്യ len)
{
    ഇരട്ട r = 0.0;
    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    പൂർണ്ണസംഖ്യ len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    ആകുമ്പോൾ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        സ്ഥിരം പൂർണ്ണസംഖ്യ cWidth = VTraits<v_int16>::vlanes();

        പൂർണ്ണസംഖ്യ j = 0;
        വേണ്ടി (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        വേണ്ടി (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (ഇരട്ട)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    തിരികെ r + dotProd_(src1, src2, len - i);
}

ഇരട്ട dotProd_16u(സ്ഥിരം ushort* src1, സ്ഥിരം ushort* src2, പൂർണ്ണസംഖ്യ len)
{
    ഇരട്ട r = 0.0;
    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    പൂർണ്ണസംഖ്യ len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    ആകുമ്പോൾ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        സ്ഥിരം പൂർണ്ണസംഖ്യ cWidth = VTraits<v_uint16>::vlanes();

        പൂർണ്ണസംഖ്യ j = 0;
        വേണ്ടി (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (ഇരട്ട)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    തിരികെ r + dotProd_(src1, src2, len - i);
}

ഇരട്ട dotProd_16s(സ്ഥിരം short* src1, സ്ഥിരം short* src2, പൂർണ്ണസംഖ്യ len)
{
    ഇരട്ട r = 0.0;
    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    പൂർണ്ണസംഖ്യ len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    ആകുമ്പോൾ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        സ്ഥിരം പൂർണ്ണസംഖ്യ cWidth = VTraits<v_int16>::vlanes();

        പൂർണ്ണസംഖ്യ j = 0;
        വേണ്ടി (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (ഇരട്ട)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    തിരികെ r + dotProd_(src1, src2, len - i);
}

ഇരട്ട dotProd_32s(സ്ഥിരം പൂർണ്ണസംഖ്യ* src1, സ്ഥിരം പൂർണ്ണസംഖ്യ* src2, പൂർണ്ണസംഖ്യ len)
{
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ഇരട്ട r = .0;
    പൂർണ്ണസംഖ്യ i = 0;
    സ്ഥിരം പൂർണ്ണസംഖ്യ step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#എങ്കിൽ CV_SIMD_WIDTH == 16
    സ്ഥിരം പൂർണ്ണസംഖ്യ wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    വേണ്ടി (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    വേണ്ടി (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    തിരികെ r + dotProd_(src1, src2, len - i);
#അല്ലെങ്കിൽ
    തിരികെ dotProd_(src1, src2, len);
#endif
}

ഇരട്ട dotProd_32f(സ്ഥിരം ദശാംശം* src1, സ്ഥിരം ദശാംശം* src2, പൂർണ്ണസംഖ്യ len)
{
    ഇരട്ട r = 0.0;
    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    പൂർണ്ണസംഖ്യ len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    ആകുമ്പോൾ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        പൂർണ്ണസംഖ്യ j = 0;
        പൂർണ്ണസംഖ്യ cWidth = VTraits<v_float32>::vlanes();

#എങ്കിൽ CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        വേണ്ടി (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
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

        വേണ്ടി (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    തിരികെ r + dotProd_(src1, src2, len - i);
}

ഇരട്ട dotProd_64f(സ്ഥിരം ഇരട്ട* src1, സ്ഥിരം ഇരട്ട* src2, പൂർണ്ണസംഖ്യ len)
{
    തിരികെ dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
