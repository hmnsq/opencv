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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

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
#ಯದಿ defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#ಅನ್ಯಥಾ
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

ನಾಮಾಕಾಶ cv {

// forward declarations
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*TransformFunc)(ಧ್ರುವ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಧ್ರುವ ಶೂನ್ಯ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ScaleAddFunc)(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ ಶೂನ್ಯ* alpha);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*MulTransposedFunc)(ಧ್ರುವ Mat& src, ಧ್ರುವ/*preallocated*/ Mat& dst, ಧ್ರುವ Mat& delta, ದ್ವಿಗುಣ scale);
ಪ್ರಕಾರನಿರ್ದೇಶ ದ್ವಿಗುಣ (*MahalanobisImplFunc)(ಧ್ರುವ Mat& v1, ಧ್ರುವ Mat& v2, ಧ್ರುವ Mat& icovar, ದ್ವಿಗುಣ *diff_buffer /*[len]*/, ಪೂರ್ಣಾಙ್ಕ len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

ಶೂನ್ಯ gemm32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ* src2, size_t src2_step,
             ಭಿನ್ನ alpha, ಧ್ರುವ ಭಿನ್ನ* src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ* dst, size_t dst_step,
             ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags);
ಶೂನ್ಯ gemm64f(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t src2_step,
             ದ್ವಿಗುಣ alpha, ಧ್ರುವ ದ್ವಿಗುಣ* src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_step,
             ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags);
ಶೂನ್ಯ gemm32fc(ಧ್ರುವ ಭಿನ್ನ* src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ* src2, size_t src2_step,
              ಭಿನ್ನ alpha, ಧ್ರುವ ಭಿನ್ನ* src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ* dst, size_t dst_step,
              ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags);
ಶೂನ್ಯ gemm64fc(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t src2_step,
              ದ್ವಿಗುಣ alpha, ಧ್ರುವ ದ್ವಿಗುಣ* src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_step,
              ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags);

TransformFunc getTransformFunc(ಪೂರ್ಣಾಙ್ಕ depth);
TransformFunc getDiagTransformFunc(ಪೂರ್ಣಾಙ್ಕ depth);
TransformFunc getPerspectiveTransform(ಪೂರ್ಣಾಙ್ಕ depth);

ScaleAddFunc getScaleAddFunc(ಪೂರ್ಣಾಙ್ಕ depth);

MahalanobisImplFunc getMahalanobisImplFunc(ಪೂರ್ಣಾಙ್ಕ depth);

MulTransposedFunc getMulTransposedFunc(ಪೂರ್ಣಾಙ್ಕ stype, ಪೂರ್ಣಾಙ್ಕ dtype, ತಾರ್ಕಿಕ ata);

ದ್ವಿಗುಣ dotProd_8u(ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಪೂರ್ಣಾಙ್ಕ len);
ದ್ವಿಗುಣ dotProd_8s(ಧ್ರುವ schar* src1, ಧ್ರುವ schar* src2, ಪೂರ್ಣಾಙ್ಕ len);
ದ್ವಿಗುಣ dotProd_16u(ಧ್ರುವ ushort* src1, ಧ್ರುವ ushort* src2, ಪೂರ್ಣಾಙ್ಕ len);
ದ್ವಿಗುಣ dotProd_16s(ಧ್ರುವ short* src1, ಧ್ರುವ short* src2, ಪೂರ್ಣಾಙ್ಕ len);
ದ್ವಿಗುಣ dotProd_32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src2, ಪೂರ್ಣಾಙ್ಕ len);
ದ್ವಿಗುಣ dotProd_32f(ಧ್ರುವ ಭಿನ್ನ* src1, ಧ್ರುವ ಭಿನ್ನ* src2, ಪೂರ್ಣಾಙ್ಕ len);
ದ್ವಿಗುಣ dotProd_64f(ಧ್ರುವ ದ್ವಿಗುಣ* src1, ಧ್ರುವ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಙ್ಕ len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ಯದಿ !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

ಸ್ಥಿರ ಶೂನ್ಯ
GEMM_CopyBlock( ಧ್ರುವ uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    ಪೂರ್ಣಾಙ್ಕ j;
    size.width *= (ಪೂರ್ಣಾಙ್ಕ)(pix_size / ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));

    ಪ್ರತಿ( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #ಯದಿ CV_ENABLE_UNROLLED
        ಪ್ರತಿ( ; j <= size.width - 4; j += 4 )
        {
            ಪೂರ್ಣಾಙ್ಕ t0 = ((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)src)[j];
            ಪೂರ್ಣಾಙ್ಕ t1 = ((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)src)[j+1];
            ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j] = t0;
            ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j+1] = t1;
            t0 = ((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)src)[j+2];
            t1 = ((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)src)[j+3];
            ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j+2] = t0;
            ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j+3] = t1;
        }
        #endif
        ಪ್ರತಿ( ; j < size.width; j++ )
            ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j] = ((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)src)[j];
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
GEMM_TransposeBlock( ಧ್ರುವ uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪ್ರತಿ( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        ಧ್ರುವ uchar* _src = src;
        ವಿಕಲ್ಪ( pix_size )
        {
        ಪಕ್ಷ ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ):
            ಪ್ರತಿ( j = 0; j < size.height; j++, _src += src_step )
                ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j] = ((ಪೂರ್ಣಾಙ್ಕ*)_src)[0];
            ಭಙ್ಗ;
        ಪಕ್ಷ ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ)*2:
            ಪ್ರತಿ( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                ಪೂರ್ಣಾಙ್ಕ t0 = ((ಪೂರ್ಣಾಙ್ಕ*)_src)[0];
                ಪೂರ್ಣಾಙ್ಕ t1 = ((ಪೂರ್ಣಾಙ್ಕ*)_src)[1];
                ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j] = t0;
                ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j+1] = t1;
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ)*4:
            ಪ್ರತಿ( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                ಪೂರ್ಣಾಙ್ಕ t0 = ((ಪೂರ್ಣಾಙ್ಕ*)_src)[0];
                ಪೂರ್ಣಾಙ್ಕ t1 = ((ಪೂರ್ಣಾಙ್ಕ*)_src)[1];
                ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j] = t0;
                ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j+1] = t1;
                t0 = ((ಪೂರ್ಣಾಙ್ಕ*)_src)[2];
                t1 = ((ಪೂರ್ಣಾಙ್ಕ*)_src)[3];
                ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j+2] = t0;
                ((ಪೂರ್ಣಾಙ್ಕ*)dst)[j+3] = t1;
            }
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            CV_Assert(0);
            ಪ್ರತ್ಯಾಗಮ;
        }
    }
}

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> ಸಂರಚನಾ VecTraits;

template<> ಸಂರಚನಾ VecTraits<ಭಿನ್ನ> {
    ಪ್ರಕಾರನಿರ್ದೇಶ v_float32 VT;
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ VT zero() { ಪ್ರತ್ಯಾಗಮ vx_setzero_f32(); }
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ VT load(ಧ್ರುವ ಭಿನ್ನ* p) { ಪ್ರತ್ಯಾಗಮ vx_load(p); }
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ VT setall(ಭಿನ್ನ v) { ಪ್ರತ್ಯಾಗಮ vx_setall_f32(v); }
};
#endif

#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> ಸಂರಚನಾ VecTraits<ದ್ವಿಗುಣ> {
    ಪ್ರಕಾರನಿರ್ದೇಶ v_float64 VT;
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ VT zero() { ಪ್ರತ್ಯಾಗಮ vx_setzero_f64(); }
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ VT load(ಧ್ರುವ ದ್ವಿಗುಣ* p) { ಪ್ರತ್ಯಾಗಮ vx_load(p); }
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ VT setall(ದ್ವಿಗುಣ v) { ಪ್ರತ್ಯಾಗಮ vx_setall_f64(v); }
};
#endif

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ST simdDotProduct(ಧ್ರುವ ST* a, ಧ್ರುವ ST* b, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ& k)
{
    ಪ್ರಕಾರನಿರ್ದೇಶ typename VecTraits<ST>::VT VecT;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes = VTraits<VecT>::vlanes();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    ಪ್ರತಿ( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    ಪ್ರತಿ( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    ಪ್ರತ್ಯಾಗಮ v_reduce_sum(vs0);
}
#endif

#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ simdDotProduct_f32f64(ಧ್ರುವ ಭಿನ್ನ* a, ಧ್ರುವ ಭಿನ್ನ* b, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ& k)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes_f32 = VTraits<v_float32>::vlanes();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    ಪ್ರತಿ( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_ಮೇಲಿನ(va0), v_cvt_f64_ಮೇಲಿನ(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_ಮೇಲಿನ(va1), v_cvt_f64_ಮೇಲಿನ(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    ಪ್ರತಿ( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_ಮೇಲಿನ(va), v_cvt_f64_ಮೇಲಿನ(vb), vs0);
    }
    ಪ್ರತ್ಯಾಗಮ v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ simdGEMM_kj_f32f64(
    ಧ್ರುವ ಭಿನ್ನ* a_data, ಧ್ರುವ ಭಿನ್ನ* _b_data, size_t b_step,
    ಧ್ರುವ ಭಿನ್ನ* _c_data, size_t c_step1,
    ಭಿನ್ನ* d_data,
    ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m,
    ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta,
    ಪೂರ್ಣಾಙ್ಕ& j)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes_f32 = VTraits<v_float32>::vlanes();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes_f64 = VTraits<v_float64>::vlanes();

    ಯದಿ( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ max_m = 1600 / (ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(ಭಿನ್ನ);
        cv::AutoBuffer<ದ್ವಿಗುಣ> _s_buf(max_m);
        ದ್ವಿಗುಣ* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            ಪ್ರತಿ( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            ಪ್ರತಿ( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ಧ್ರುವ ಭಿನ್ನ* b_ptr = _b_data;
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((ದ್ವಿಗುಣ)a_data[k]);
                j = 0;
                ಪ್ರತಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_ಮೇಲಿನ(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                ದ್ವಿಗುಣ a_val = (ದ್ವಿಗುಣ)a_data[k];
                ಪ್ರತಿ( ; j < m; j++ )
                    s_buf[j] += a_val * (ದ್ವಿಗುಣ)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ಧ್ರುವ ಭಿನ್ನ* c_data = _c_data;
            j = 0;
            ಯದಿ( !_c_data )
            {
                ಪ್ರತಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                ಪ್ರತಿ( ; j < m; j++ )
                    d_data[j] = (ಭಿನ್ನ)(s_buf[j] * alpha);
            }
            ಅನ್ಯಥಾ ಯದಿ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                ಪ್ರತಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_ಮೇಲಿನ(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                ಪ್ರತಿ( ; j < m; j++ )
                    d_data[j] = (ಭಿನ್ನ)(s_buf[j] * alpha + (ದ್ವಿಗುಣ)c_data[j] * beta);
            }
            ಅನ್ಯಥಾ
            {
                ಪ್ರತಿ( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (ಭಿನ್ನ)(s_buf[j] * alpha + (ದ್ವಿಗುಣ)c_data[0] * beta);
            }
        }
        j = m;
    }
    ಅನ್ಯಥಾ ಯದಿ( vlanes_f32 * (ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(ಭಿನ್ನ) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ಧ್ರುವ ಭಿನ್ನ* c_data = _c_data;
        ಪ್ರತಿ( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            ಧ್ರುವ ಭಿನ್ನ* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((ದ್ವಿಗುಣ)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_ಮೇಲಿನ(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            ಯದಿ( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ಅನ್ಯಥಾ ಯದಿ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_ಮೇಲಿನ(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ಅನ್ಯಥಾ
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ಭಿನ್ನ buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ಭಿನ್ನ)beta;
            }
        }
    }
}
#endif

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ simdGEMM_kj(
    ಧ್ರುವ ST* a_data, ಧ್ರುವ ST* _b_data, size_t b_step,
    ಧ್ರುವ ST* _c_data, size_t c_step1,
    ST* d_data,
    ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m,
    ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta,
    ಪೂರ್ಣಾಙ್ಕ& j)
{
    ಪ್ರಕಾರನಿರ್ದೇಶ typename VecTraits<ST>::VT VecT;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes = VTraits<VecT>::vlanes();

    ಯದಿ( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ max_m = 1600 / (ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            ಪ್ರತಿ( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            ಪ್ರತಿ( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ಧ್ರುವ ST* b_ptr = _b_data;
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                ಪ್ರತಿ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                ಪ್ರತಿ( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ಧ್ರುವ ST* c_data = _c_data;
            j = 0;
            ಯದಿ( !_c_data )
            {
                ಪ್ರತಿ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                ಪ್ರತಿ( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            ಅನ್ಯಥಾ ಯದಿ( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                ಪ್ರತಿ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                ಪ್ರತಿ( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            ಅನ್ಯಥಾ
            {
                ಪ್ರತಿ( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    ಅನ್ಯಥಾ ಯದಿ( vlanes * (ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ಧ್ರುವ ST* c_data = _c_data;
        ಪ್ರತಿ( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            ಧ್ರುವ ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            ಯದಿ( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            ಅನ್ಯಥಾ ಯದಿ( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            ಅನ್ಯಥಾ
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> ಸ್ಥಿರ ಶೂನ್ಯ
GEMMSingleMul( ಧ್ರುವ T* a_data, size_t a_step,
               ಧ್ರುವ T* b_data, size_t b_step,
               ಧ್ರುವ T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    ಪೂರ್ಣಾಙ್ಕ i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    ಧ್ರುವ T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= ಪರಿಮಾಣ(a_data[0]);
    b_step /= ಪರಿಮಾಣ(b_data[0]);
    c_step /= ಪರಿಮಾಣ(c_data[0]);
    d_step /= ಪರಿಮಾಣ(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    ಯದಿ( !c_data )
        c_step0 = c_step1 = 0;
    ಅನ್ಯಥಾ ಯದಿ( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    ಅನ್ಯಥಾ
        c_step0 = 1, c_step1 = c_step;

    ಯದಿ( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        ಯದಿ( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    ಯದಿ( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        ಯದಿ( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            ಪ್ರತಿ( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        ಯದಿ( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            ಪ್ರತಿ( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        ಪ್ರತಿ( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            ಪ್ರತಿ( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                ಯದಿ( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                ಅನ್ಯಥಾ
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            ಪ್ರತಿ( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                ಯದಿ( !c_data )
                    d_data[j] = T(s0);
                ಅನ್ಯಥಾ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( flags & GEMM_2_T ) /* A * Bt */
    {
        ಪ್ರತಿ( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            ಯದಿ( a_buf )
            {
                ಪ್ರತಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            ಪ್ರತಿ( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#ಯದಿ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ಯದಿ( ಪರಿಮಾಣ(WT) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
                {
                    ಯದಿ( ಪರಿಮಾಣ(T) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
                        s0 = (WT)simdDotProduct((ಧ್ರುವ ದ್ವಿಗುಣ*)a_data, (ಧ್ರುವ ದ್ವಿಗುಣ*)b_data, n, k);
                    ಅನ್ಯಥಾ ಯದಿ( ಪರಿಮಾಣ(T) == ಪರಿಮಾಣ(ಭಿನ್ನ) )
                        s0 = (WT)simdDotProduct_f32f64((ಧ್ರುವ ಭಿನ್ನ*)a_data, (ಧ್ರುವ ಭಿನ್ನ*)b_data, n, k);
                }
                ಅನ್ಯಥಾ
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #ಯದಿ CV_ENABLE_UNROLLED
                    ಪ್ರತಿ( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                ಪ್ರತಿ( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                ಯದಿ( !c_data )
                    d_data[j] = T(s0);
                ಅನ್ಯಥಾ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( d_size.width*ಪರಿಮಾಣ(d_data[0]) <= 1600 )
    {
        ಪ್ರತಿ( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            ಯದಿ( a_buf )
            {
                ಪ್ರತಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ಯದಿ( ಪರಿಮಾಣ(WT) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
            {
                ಯದಿ( ಪರಿಮಾಣ(T) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
                    simdGEMM_kj((ಧ್ರುವ ದ್ವಿಗುಣ*)a_data, (ಧ್ರುವ ದ್ವಿಗುಣ*)_b_data, b_step,
                                        (ಧ್ರುವ ದ್ವಿಗುಣ*)_c_data, c_step1,
                                (ದ್ವಿಗುಣ*)d_data, n, m, alpha, beta, j);
                ಅನ್ಯಥಾ ಯದಿ( ಪರಿಮಾಣ(T) == ಪರಿಮಾಣ(ಭಿನ್ನ) )
                    simdGEMM_kj_f32f64((ಧ್ರುವ ಭಿನ್ನ*)a_data, (ಧ್ರುವ ಭಿನ್ನ*)_b_data, b_step,
                                        (ಧ್ರುವ ಭಿನ್ನ*)_c_data, c_step1,
                                (ಭಿನ್ನ*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            ಪ್ರತಿ( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                ಧ್ರುವ T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                ಪ್ರತಿ( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                ಯದಿ( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                ಅನ್ಯಥಾ
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            ಪ್ರತಿ( ; j < m; j++, c_data += c_step1 )
            {
                ಧ್ರುವ T* b = _b_data + j;
                WT s0(0);

                ಪ್ರತಿ( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                ಯದಿ( !c_data )
                    d_data[j] = T(s0);
                ಅನ್ಯಥಾ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ಅನ್ಯಥಾ
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        ಪ್ರತಿ( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            ಯದಿ( a_buf )
            {
                ಪ್ರತಿ( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            ಪ್ರತಿ( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            ಪ್ರತಿ( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                ಯದಿ( ಪರಿಮಾಣ(WT) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
                {
                    ಯದಿ( ಪರಿಮಾಣ(T) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
                    {
                        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ಧ್ರುವ ದ್ವಿಗುಣ*)&al));
                        ಪ್ರತಿ( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((ಧ್ರುವ ದ್ವಿಗುಣ*)(d_buf + j));
                            v_float64 vb = vx_load((ಧ್ರುವ ದ್ವಿಗುಣ*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((ದ್ವಿಗುಣ*)(d_buf + j), vd);
                        }
                    }
                    ಅನ್ಯಥಾ
                    {
                        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes_f32 = VTraits<v_float32>::vlanes();
                        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ಧ್ರುವ ದ್ವಿಗುಣ*)&al));
                        ಪ್ರತಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((ಧ್ರುವ ಭಿನ್ನ*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_ಮೇಲಿನ(vb);
                            v_float64 vd_lo = vx_load((ಧ್ರುವ ದ್ವಿಗುಣ*)(d_buf + j));
                            v_float64 vd_hi = vx_load((ಧ್ರುವ ದ್ವಿಗುಣ*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((ದ್ವಿಗುಣ*)(d_buf + j), vd_lo);
                            v_store((ದ್ವಿಗುಣ*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                ಪ್ರತಿ( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            ಯದಿ( !c_data )
                ಪ್ರತಿ( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            ಅನ್ಯಥಾ
                ಪ್ರತಿ( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ setDoubleDataZero(WT* d_data, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ vlanes)
{
    v_float64 vz = vx_setzero_f64();
    ಪೂರ್ಣಾಙ್ಕ j = 0;
    ಪ್ರತಿ( ; j <= m - vlanes; j += vlanes )
        v_store((ದ್ವಿಗುಣ*)(d_data + j), vz);
    ಪ್ರತಿ( ; j < m; j++ )
        d_data[j] = WT(0);
}

template<typename ST>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ simdMulAdd(ಧ್ರುವ ST* b, ದ್ವಿಗುಣ* d, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ a_val);

template<>
ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ simdMulAdd<ಭಿನ್ನ>(
    ಧ್ರುವ ಭಿನ್ನ* b, ದ್ವಿಗುಣ* d, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ a_val)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes_f32 = VTraits<v_float32>::vlanes();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    ಪೂರ್ಣಾಙ್ಕ j = 0;
    ಪ್ರತಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_ಮೇಲಿನ(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    ಪ್ರತಿ( ; j < m; j++ )
        d[j] += a_val * (ದ್ವಿಗುಣ)b[j];
}

template<>
ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ simdMulAdd<ದ್ವಿಗುಣ>(
    ಧ್ರುವ ದ್ವಿಗುಣ* b, ದ್ವಿಗುಣ* d, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ a_val)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes = VTraits<v_float64>::vlanes();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    ಪೂರ್ಣಾಙ್ಕ j = 0;
    ಪ್ರತಿ( ; j <= m - stride2; j += stride2 )
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
    ಪ್ರತಿ( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    ಪ್ರತಿ( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ simdBlockMul_kj(
    ಧ್ರುವ ST* a_data, ಧ್ರುವ ST* b_data, size_t b_step,
    ದ್ವಿಗುಣ* d_data, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ತಾರ್ಕಿಕ do_acc)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes = VTraits<v_float64>::vlanes();
    ಯದಿ( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (ದ್ವಿಗುಣ)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> ಸ್ಥಿರ ಶೂನ್ಯ
GEMMBlockMul( ಧ್ರುವ T* a_data, size_t a_step,
              ಧ್ರುವ T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, ಪೂರ್ಣಾಙ್ಕ flags )
{
    ಪೂರ್ಣಾಙ್ಕ i, j, k, n = a_size.width, m = d_size.width;
    ಧ್ರುವ T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    ಪೂರ್ಣಾಙ್ಕ do_acc = flags & 16;

    a_step /= ಪರಿಮಾಣ(a_data[0]);
    b_step /= ಪರಿಮಾಣ(b_data[0]);
    d_step /= ಪರಿಮಾಣ(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    ಯದಿ( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    ಯದಿ( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        ಪ್ರತಿ( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            ಯದಿ( a_buf )
            {
                ಪ್ರತಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            ಪ್ರತಿ( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#ಯದಿ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ಯದಿ( ಪರಿಮಾಣ(WT) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
                {
                    ಯದಿ( ಪರಿಮಾಣ(T) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
                        s0 += (WT)simdDotProduct((ಧ್ರುವ ದ್ವಿಗುಣ*)a_data, (ಧ್ರುವ ದ್ವಿಗುಣ*)b_data, n, k);
                    ಅನ್ಯಥಾ ಯದಿ( ಪರಿಮಾಣ(T) == ಪರಿಮಾಣ(ಭಿನ್ನ) )
                        s0 += (WT)simdDotProduct_f32f64((ಧ್ರುವ ಭಿನ್ನ*)a_data, (ಧ್ರುವ ಭಿನ್ನ*)b_data, n, k);
                }
                ಅನ್ಯಥಾ
#endif
                {
                    WT s1(0);
                    ಪ್ರತಿ( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                ಪ್ರತಿ( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            ಯದಿ( a_buf )
            {
                ಪ್ರತಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ಯದಿ( ಪರಿಮಾಣ(WT) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
            {
                ಯದಿ( ಪರಿಮಾಣ(T) == ಪರಿಮಾಣ(ದ್ವಿಗುಣ) )
                    simdBlockMul_kj(
                        (ಧ್ರುವ ದ್ವಿಗುಣ*)a_data, (ಧ್ರುವ ದ್ವಿಗುಣ*)b_data, b_step,
                        (ದ್ವಿಗುಣ*)d_data, n, m, do_acc != 0);
                ಅನ್ಯಥಾ
                    simdBlockMul_kj(
                        (ಧ್ರುವ ಭಿನ್ನ*)a_data, (ಧ್ರುವ ಭಿನ್ನ*)b_data, b_step,
                        (ದ್ವಿಗುಣ*)d_data, n, m, do_acc != 0);
            }
            ಅನ್ಯಥಾ
#endif
            {
                ಪ್ರತಿ( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ಧ್ರುವ T* b = b_data + j;

                    ಯದಿ( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    ಅನ್ಯಥಾ
                        s0 = s1 = s2 = s3 = WT(0);

                    ಪ್ರತಿ( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                ಪ್ರತಿ( ; j < m; j++ )
                {
                    ಧ್ರುವ T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    ಪ್ರತಿ( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> ಸ್ಥಿರ ಶೂನ್ಯ
GEMMStore( ಧ್ರುವ T* c_data, size_t c_step,
           ಧ್ರುವ WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    ಧ್ರುವ T* _c_data = c_data;
    ಪೂರ್ಣಾಙ್ಕ j;
    size_t c_step0, c_step1;

    c_step /= ಪರಿಮಾಣ(c_data[0]);
    d_buf_step /= ಪರಿಮಾಣ(d_buf[0]);
    d_step /= ಪರಿಮಾಣ(d_data[0]);

    ಯದಿ( !c_data )
        c_step0 = c_step1 = 0;
    ಅನ್ಯಥಾ ಯದಿ( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    ಅನ್ಯಥಾ
        c_step0 = 1, c_step1 = c_step;

    ಪ್ರತಿ( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        ಯದಿ( _c_data )
        {
            c_data = _c_data;
            j=0;
             #ಯದಿ CV_ENABLE_UNROLLED
            ಪ್ರತಿ(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
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
            ಪ್ರತಿ( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        ಅನ್ಯಥಾ
        {
            j = 0;
             #ಯದಿ CV_ENABLE_UNROLLED
            ಪ್ರತಿ( ; j <= d_size.width - 4; j += 4 )
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
            ಪ್ರತಿ( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*GEMMSingleMulFunc)( ಧ್ರುವ ಶೂನ್ಯ* src1, size_t step1,
                   ಧ್ರುವ ಶೂನ್ಯ* src2, size_t step2, ಧ್ರುವ ಶೂನ್ಯ* src3, size_t step3,
                   ಶೂನ್ಯ* dst, size_t dststep, Size srcsize, Size dstsize,
                   ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags );

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*GEMMBlockMulFunc)( ಧ್ರುವ ಶೂನ್ಯ* src1, size_t step1,
                   ಧ್ರುವ ಶೂನ್ಯ* src2, size_t step2, ಶೂನ್ಯ* dst, size_t dststep,
                   Size srcsize, Size dstsize, ಪೂರ್ಣಾಙ್ಕ flags );

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*GEMMStoreFunc)( ಧ್ರುವ ಶೂನ್ಯ* src1, size_t step1,
                   ಧ್ರುವ ಶೂನ್ಯ* src2, size_t step2, ಶೂನ್ಯ* dst, size_t dststep,
                   Size dstsize, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags );

template<typename T, typename WT, ಶೂನ್ಯ (*fn)(ಧ್ರುವ T*, size_t, ಧ್ರುವ T*, size_t, ಧ್ರುವ T*, size_t, T*, size_t, Size, Size, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ gemmSingleMulWrap( ಧ್ರುವ ಶೂನ್ಯ* src1, size_t step1,
                   ಧ್ರುವ ಶೂನ್ಯ* src2, size_t step2, ಧ್ರುವ ಶೂನ್ಯ* src3, size_t step3,
                   ಶೂನ್ಯ* dst, size_t dststep, Size srcsize, Size dstsize,
                   ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    fn((ಧ್ರುವ T*)src1, step1, (ಧ್ರುವ T*)src2, step2, (ಧ್ರುವ T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, ಶೂನ್ಯ (*fn)(ಧ್ರುವ T*, size_t, ಧ್ರುವ T*, size_t, WT*, size_t, Size, Size, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ gemmBlockMulWrap( ಧ್ರುವ ಶೂನ್ಯ* src1, size_t step1,
                   ಧ್ರುವ ಶೂನ್ಯ* src2, size_t step2, ಶೂನ್ಯ* dst, size_t dststep,
                   Size srcsize, Size dstsize, ಪೂರ್ಣಾಙ್ಕ flags )
{
    fn((ಧ್ರುವ T*)src1, step1, (ಧ್ರುವ T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, ಶೂನ್ಯ (*fn)(ಧ್ರುವ T*, size_t, ಧ್ರುವ WT*, size_t, T*, size_t, Size, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ gemmStoreWrap( ಧ್ರುವ ಶೂನ್ಯ* src1, size_t step1,
                   ಧ್ರುವ ಶೂನ್ಯ* src2, size_t step2, ಶೂನ್ಯ* dst, size_t dststep,
                   Size dstsize, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    fn((ಧ್ರುವ T*)src1, step1, (ಧ್ರುವ WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ GEMMSingleMul_32f( ಧ್ರುವ ಭಿನ್ನ* a_data, size_t a_step,
              ಧ್ರುವ ಭಿನ್ನ* b_data, size_t b_step,
              ಧ್ರುವ ಭಿನ್ನ* c_data, size_t c_step,
              ಭಿನ್ನ* d_data, size_t d_step,
              Size a_size, Size d_size,
              ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMSingleMul<ಭಿನ್ನ,ದ್ವಿಗುಣ>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ GEMMSingleMul_64f( ಧ್ರುವ ದ್ವಿಗುಣ* a_data, size_t a_step,
                              ಧ್ರುವ ದ್ವಿಗುಣ* b_data, size_t b_step,
                              ಧ್ರುವ ದ್ವಿಗುಣ* c_data, size_t c_step,
                              ದ್ವಿಗುಣ* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMSingleMul<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMSingleMul_32fc( ಧ್ರುವ Complexf* a_data, size_t a_step,
                              ಧ್ರುವ Complexf* b_data, size_t b_step,
                              ಧ್ರುವ Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ GEMMSingleMul_64fc( ಧ್ರುವ Complexd* a_data, size_t a_step,
                              ಧ್ರುವ Complexd* b_data, size_t b_step,
                              ಧ್ರುವ Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ GEMMBlockMul_32f( ಧ್ರುವ ಭಿನ್ನ* a_data, size_t a_step,
             ಧ್ರುವ ಭಿನ್ನ* b_data, size_t b_step,
             ದ್ವಿಗುಣ* d_data, size_t d_step,
             Size a_size, Size d_size, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMBlockMul_64f( ಧ್ರುವ ದ್ವಿಗುಣ* a_data, size_t a_step,
                             ಧ್ರುವ ದ್ವಿಗುಣ* b_data, size_t b_step,
                             ದ್ವಿಗುಣ* d_data, size_t d_step,
                             Size a_size, Size d_size, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMBlockMul_32fc( ಧ್ರುವ Complexf* a_data, size_t a_step,
                             ಧ್ರುವ Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMBlockMul_64fc( ಧ್ರುವ Complexd* a_data, size_t a_step,
                             ಧ್ರುವ Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMStore_32f( ಧ್ರುವ ಭಿನ್ನ* c_data, size_t c_step,
          ಧ್ರುವ ದ್ವಿಗುಣ* d_buf, size_t d_buf_step,
          ಭಿನ್ನ* d_data, size_t d_step, Size d_size,
          ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMStore_64f( ಧ್ರುವ ದ್ವಿಗುಣ* c_data, size_t c_step,
                      ಧ್ರುವ ದ್ವಿಗುಣ* d_buf, size_t d_buf_step,
                      ದ್ವಿಗುಣ* d_data, size_t d_step, Size d_size,
                      ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMStore_32fc( ಧ್ರುವ Complexf* c_data, size_t c_step,
                          ಧ್ರುವ Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMStore_64fc( ಧ್ರುವ Complexd* c_data, size_t c_step,
                          ಧ್ರುವ Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಙ್ಕ flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ gemmImpl( Mat A, Mat B, ದ್ವಿಗುಣ alpha,
           Mat C, ದ್ವಿಗುಣ beta, Mat D, ಪೂರ್ಣಾಙ್ಕ flags )
{
    CV_INSTRUMENT_REGION();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ block_lin_size = 128;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ block_size = block_lin_size * block_lin_size;

    ಸ್ಥಿರ ದ್ವಿಗುಣ zero[] = {0,0,0,0};
    ಸ್ಥಿರ ಭಿನ್ನ zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    ಪೂರ್ಣಾಙ್ಕ i, len = 0, type = A.type();

    ವಿಕಲ್ಪ( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ಪಕ್ಷ 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        ಭಙ್ಗ;
    ಪಕ್ಷ 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        ಭಙ್ಗ;
    ಪಕ್ಷ 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        ಭಙ್ಗ;
    ಪಕ್ಷ 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        ಭಙ್ಗ;
    }

    ಯದಿ( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        ಯದಿ( type == CV_32F )
        {
            ಭಿನ್ನ* d = D.ptr<ಭಿನ್ನ>();
            ಧ್ರುವ ಭಿನ್ನ *a = A.ptr<ಭಿನ್ನ>(),
                        *b = B.ptr<ಭಿನ್ನ>(),
                        *c = (ಧ್ರುವ ಭಿನ್ನ*)C.data;
            size_t d_step = D.step/ಪರಿಮಾಣ(d[0]),
                a_step = A.step/ಪರಿಮಾಣ(a[0]),
                b_step = B.step/ಪರಿಮಾಣ(b[0]),
                c_step = C.data ? C.step/ಪರಿಮಾಣ(c[0]) : 0;

            ಯದಿ( !c )
                c = zerof;

            ವಿಕಲ್ಪ( len )
            {
            ಪಕ್ಷ 2:
                ಯದಿ( len == d_size.width && b != d )
                {
                    ಪ್ರತಿ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ಭಿನ್ನ t0 = a[0]*b[0] + a[1]*b[b_step];
                        ಭಿನ್ನ t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (ಭಿನ್ನ)(t0*alpha + c[0]*beta);
                        d[1] = (ಭಿನ್ನ)(t1*alpha + c[1]*beta);
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ( a != d )
                {
                    ಪೂರ್ಣಾಙ್ಕ c_step0 = 1;
                    ಯದಿ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ಪ್ರತಿ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ಭಿನ್ನ t0 = a[0]*b[0] + a[1]*b[b_step];
                        ಭಿನ್ನ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (ಭಿನ್ನ)(t0*alpha + c[0]*beta);
                        d[d_step] = (ಭಿನ್ನ)(t1*alpha + c[c_step]*beta);
                    }
                }
                ಅನ್ಯಥಾ
                    ಭಙ್ಗ;
                ಪ್ರತ್ಯಾಗಮ;
            ಪಕ್ಷ 3:
                ಯದಿ( len == d_size.width && b != d )
                {
                    ಪ್ರತಿ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ಭಿನ್ನ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ಭಿನ್ನ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        ಭಿನ್ನ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (ಭಿನ್ನ)(t0*alpha + c[0]*beta);
                        d[1] = (ಭಿನ್ನ)(t1*alpha + c[1]*beta);
                        d[2] = (ಭಿನ್ನ)(t2*alpha + c[2]*beta);
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ( a != d )
                {
                    ಪೂರ್ಣಾಙ್ಕ c_step0 = 1;
                    ಯದಿ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ಪ್ರತಿ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ಭಿನ್ನ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ಭಿನ್ನ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        ಭಿನ್ನ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (ಭಿನ್ನ)(t0*alpha + c[0]*beta);
                        d[d_step] = (ಭಿನ್ನ)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (ಭಿನ್ನ)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                ಅನ್ಯಥಾ
                    ಭಙ್ಗ;
                ಪ್ರತ್ಯಾಗಮ;
            ಪಕ್ಷ 4:
                ಯದಿ( len == d_size.width && b != d )
                {
                    ಪ್ರತಿ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ಭಿನ್ನ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ಭಿನ್ನ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        ಭಿನ್ನ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        ಭಿನ್ನ t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (ಭಿನ್ನ)(t0*alpha + c[0]*beta);
                        d[1] = (ಭಿನ್ನ)(t1*alpha + c[1]*beta);
                        d[2] = (ಭಿನ್ನ)(t2*alpha + c[2]*beta);
                        d[3] = (ಭಿನ್ನ)(t3*alpha + c[3]*beta);
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ( len <= 16 && a != d )
                {
                    ಪೂರ್ಣಾಙ್ಕ c_step0 = 1;
                    ಯದಿ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ಪ್ರತಿ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ಭಿನ್ನ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ಭಿನ್ನ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        ಭಿನ್ನ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        ಭಿನ್ನ t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (ಭಿನ್ನ)(t0*alpha + c[0]*beta);
                        d[d_step] = (ಭಿನ್ನ)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (ಭಿನ್ನ)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (ಭಿನ್ನ)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                ಅನ್ಯಥಾ
                    ಭಙ್ಗ;
                ಪ್ರತ್ಯಾಗಮ;
            }
        }

        ಯದಿ( type == CV_64F )
        {
            ದ್ವಿಗುಣ* d = D.ptr<ದ್ವಿಗುಣ>();
            ಧ್ರುವ ದ್ವಿಗುಣ *a = A.ptr<ದ್ವಿಗುಣ>(),
                         *b = B.ptr<ದ್ವಿಗುಣ>(),
                         *c = (ಧ್ರುವ ದ್ವಿಗುಣ*)C.data;
            size_t d_step = D.step/ಪರಿಮಾಣ(d[0]),
                a_step = A.step/ಪರಿಮಾಣ(a[0]),
                b_step = B.step/ಪರಿಮಾಣ(b[0]),
                c_step = C.data ? C.step/ಪರಿಮಾಣ(c[0]) : 0;
            ಯದಿ( !c )
                c = zero;

            ವಿಕಲ್ಪ( len )
            {
            ಪಕ್ಷ 2:
                ಯದಿ( len == d_size.width && b != d )
                {
                    ಪ್ರತಿ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_step];
                        ದ್ವಿಗುಣ t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ( a != d )
                {
                    ಪೂರ್ಣಾಙ್ಕ c_step0 = 1;
                    ಯದಿ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ಪ್ರತಿ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_step];
                        ದ್ವಿಗುಣ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                ಅನ್ಯಥಾ
                    ಭಙ್ಗ;
                ಪ್ರತ್ಯಾಗಮ;
            ಪಕ್ಷ 3:
                ಯದಿ( len == d_size.width && b != d )
                {
                    ಪ್ರತಿ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ದ್ವಿಗುಣ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        ದ್ವಿಗುಣ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ( a != d )
                {
                    ಪೂರ್ಣಾಙ್ಕ c_step0 = 1;
                    ಯದಿ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ಪ್ರತಿ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        ದ್ವಿಗುಣ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        ದ್ವಿಗುಣ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                ಅನ್ಯಥಾ
                    ಭಙ್ಗ;
                ಪ್ರತ್ಯಾಗಮ;
            ಪಕ್ಷ 4:
                ಯದಿ( len == d_size.width && b != d )
                {
                    ಪ್ರತಿ( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ದ್ವಿಗುಣ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        ದ್ವಿಗುಣ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        ದ್ವಿಗುಣ t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ( d_size.width <= 16 && a != d )
                {
                    ಪೂರ್ಣಾಙ್ಕ c_step0 = 1;
                    ಯದಿ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    ಪ್ರತಿ( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        ದ್ವಿಗುಣ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        ದ್ವಿಗುಣ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        ದ್ವಿಗುಣ t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                ಅನ್ಯಥಾ
                    ಭಙ್ಗ;
                ಪ್ರತ್ಯಾಗಮ;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ಧ್ರುವ uchar* Cdata = C.data;
    size_t Cstep = C.data ? (size_t)C.step : 0;
    AutoBuffer<uchar> buf;

    ಯದಿ( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ಭಿನ್ನ, ದ್ವಿಗುಣ, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<ಭಿನ್ನ, ದ್ವಿಗುಣ, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<ಭಿನ್ನ, ದ್ವಿಗುಣ, GEMMStore_32f>;
    }
    ಅನ್ಯಥಾ ಯದಿ( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, GEMMStore_64f>;
    }
    ಅನ್ಯಥಾ ಯದಿ( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    ಅನ್ಯಥಾ
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    ಯದಿ( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
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
    else*/ ಯದಿ( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ is_a_t = flags & GEMM_1_T;
        ಪೂರ್ಣಾಙ್ಕ is_b_t = flags & GEMM_2_T;
        ಪೂರ್ಣಾಙ್ಕ elem_size = CV_ELEM_SIZE(type);
        ಪೂರ್ಣಾಙ್ಕ dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        ಪೂರ್ಣಾಙ್ಕ j, k, di = 0, dj = 0, dk = 0;
        ಪೂರ್ಣಾಙ್ಕ dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        ಪೂರ್ಣಾಙ್ಕ work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        ಯದಿ( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        ಅನ್ಯಥಾ
            a_step0 = elem_size, a_step1 = A.step;

        ಯದಿ( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        ಅನ್ಯಥಾ
            b_step0 = elem_size, b_step1 = b_step;

        ಯದಿ( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        ಅನ್ಯಥಾ ಯದಿ( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        ಅನ್ಯಥಾ
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        ಯದಿ( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        ಯದಿ( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        ಯದಿ( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        ಯದಿ( is_a_t )
            a_buf = b_buf + b_buf_size;

        ಪ್ರತಿ( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            ಯದಿ( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            ಪ್ರತಿ( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                ಧ್ರುವ uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                ಯದಿ( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                ಯದಿ( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                ಪ್ರತಿ( k = 0; k < len; k += dk )
                {
                    ಧ್ರುವ uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    ಧ್ರುವ uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    ಯದಿ( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    ಯದಿ( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    ಅನ್ಯಥಾ
                        a_bl_size.width = di, a_bl_size.height = dk;

                    ಯದಿ( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    ಯದಿ( dj < d_size.width )
                    {
                        Size b_size;
                        ಯದಿ( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        ಅನ್ಯಥಾ
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    ಯದಿ( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    ಅನ್ಯಥಾ
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                ಯದಿ( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ ಶೂನ್ಯ
callGemmImpl(ಧ್ರುವ fptype *src1, size_t src1_step, ಧ್ರುವ fptype *src2, size_t src2_step, fptype alpha,
          ಧ್ರುವ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    ಪೂರ್ಣಾಙ್ಕ b_m, b_n, c_m, c_n, m_d;

    ಯದಿ(flags & GEMM_2_T)
    {
        b_m = n_d;
        ಯದಿ(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        ಅನ್ಯಥಾ
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    ಅನ್ಯಥಾ
    {
        b_n = n_d;
        ಯದಿ(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        ಅನ್ಯಥಾ
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    ಯದಿ(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    ಅನ್ಯಥಾ
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    ಯದಿ(src1 != ರಿಕ್ತ)
        A = Mat(m_a, n_a, type, (ಶೂನ್ಯ*)src1, src1_step);
    ಯದಿ(src2 != ರಿಕ್ತ)
        B = Mat(b_m, b_n, type, (ಶೂನ್ಯ*)src2, src2_step);
    ಯದಿ(src3 != ರಿಕ್ತ && beta != 0.0)
        C = Mat(c_m, c_n, type, (ಶೂನ್ಯ*)src3, src3_step);
    Mat D(m_d, n_d, type, (ಶೂನ್ಯ*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

ಶೂನ್ಯ gemm32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ* src2, size_t src2_step,
             ಭಿನ್ನ alpha, ಧ್ರುವ ಭಿನ್ನ* src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ* dst, size_t dst_step,
             ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

ಶೂನ್ಯ gemm64f(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t src2_step,
             ದ್ವಿಗುಣ alpha, ಧ್ರುವ ದ್ವಿಗುಣ* src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_step,
             ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

ಶೂನ್ಯ gemm32fc(ಧ್ರುವ ಭಿನ್ನ* src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ* src2, size_t src2_step,
              ಭಿನ್ನ alpha, ಧ್ರುವ ಭಿನ್ನ* src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ* dst, size_t dst_step,
              ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

ಶೂನ್ಯ gemm64fc(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t src2_step,
              ದ್ವಿಗುಣ alpha, ಧ್ರುವ ದ್ವಿಗುಣ* src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_step,
              ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> ಸ್ಥಿರ ಶೂನ್ಯ
transform_( ಧ್ರುವ T* src, T* dst, ಧ್ರುವ WT* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn )
{
    ಪೂರ್ಣಾಙ್ಕ x;

    ಯದಿ( scn == 2 && dcn == 2 )
    {
        ಪ್ರತಿ( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( scn == 3 && dcn == 3 )
    {
        ಪ್ರತಿ( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( scn == 3 && dcn == 1 )
    {
        ಪ್ರತಿ( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    ಅನ್ಯಥಾ ಯದಿ( scn == 4 && dcn == 4 )
    {
        ಪ್ರತಿ( x = 0; x < len*4; x += 4 )
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
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ಧ್ರುವ WT* _m = m;
            ಪೂರ್ಣಾಙ್ಕ j, k;
            ಪ್ರತಿ( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                ಪ್ರತಿ( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_8u( ಧ್ರುವ uchar* src, uchar* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn )
{
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ BITS = 10, SCALE = 1 << BITS;
    ಧ್ರುವ ಭಿನ್ನ MAX_M = (ಭಿನ್ನ)(1 << (15 - BITS));

    ಯದಿ( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nChannels = 3;

        ಸಙ್ಘ {
            short s[6];
            ಪೂರ್ಣಾಙ್ಕ p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        ಪೂರ್ಣಾಙ್ಕ m32[] = {saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 2] * SCALE), saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 3] * SCALE),
                     saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 6] * SCALE), saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 7] * SCALE),
                     saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[10] * SCALE), saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        ಪೂರ್ಣಾಙ್ಕ x = 0;
        ಪ್ರತಿ (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>((m[11] + 0.5f)*SCALE);
        ಪ್ರತಿ( ; x < len * nChannels; x += nChannels )
        {
            ಪೂರ್ಣಾಙ್ಕ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_16u( ಧ್ರುವ ushort* src, ushort* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn )
{
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ( scn == 3 && dcn == 3 )
    {
        ಪೂರ್ಣಾಙ್ಕ x = 0;
#ಯದಿ CV_SIMD_WIDTH > 16
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
        ಪ್ರತಿ (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#ಯದಿ CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_ಎಡ<1>(_m0l);
        v_float32x4 _m1h = v_rotate_ಎಡ<1>(_m1l);
        v_float32x4 _m2h = v_rotate_ಎಡ<1>(_m2l);
        v_float32x4 _m3h = v_rotate_ಎಡ<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        ಪ್ರತಿ( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_ಬಲ<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        ಪ್ರತಿ( ; x < len * 3; x += 3 )
        {
            ಭಿನ್ನ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_32f( ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    ಪೂರ್ಣಾಙ್ಕ x = 0;
    ಯದಿ( scn == 3 && dcn == 3 )
    {
        ಪೂರ್ಣಾಙ್ಕ idx[VTraits<v_float32>::max_nlanes/2];
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        ಭಿನ್ನ _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        ಪ್ರತಿ( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        ಪ್ರತಿ( ; x < len*3; x += 3 )
        {
            ಭಿನ್ನ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            ಭಿನ್ನ t0 = saturate_cast<ಭಿನ್ನ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            ಭಿನ್ನ t1 = saturate_cast<ಭಿನ್ನ>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            ಭಿನ್ನ t2 = saturate_cast<ಭಿನ್ನ>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( scn == 4 && dcn == 4 )
    {
#ಯದಿ CV_SIMD_WIDTH > 16
        ಪೂರ್ಣಾಙ್ಕ idx[VTraits<v_float32>::max_nlanes/4];
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        ಭಿನ್ನ _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        ಪ್ರತಿ( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#ಯದಿ CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        ಪ್ರತಿ( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#ಅನ್ಯಥಾ // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        ಪ್ರತಿ( ; x < len*4; x += 4 )
        {
            ಭಿನ್ನ v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            ಭಿನ್ನ t0 = saturate_cast<ಭಿನ್ನ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            ಭಿನ್ನ t1 = saturate_cast<ಭಿನ್ನ>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            ಭಿನ್ನ t2 = saturate_cast<ಭಿನ್ನ>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            ಭಿನ್ನ t3 = saturate_cast<ಭಿನ್ನ>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


ಸ್ಥಿರ ಶೂನ್ಯ
transform_8s(ಧ್ರುವ schar* src, schar* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ BITS = 10, SCALE = 1 << BITS;
    ಧ್ರುವ ಭಿನ್ನ MAX_M = (ಭಿನ್ನ)(1 << (15 - BITS));

    ಯದಿ( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nChannels = 3;

        ಸಙ್ಘ {
            short s[6];
            ಪೂರ್ಣಾಙ್ಕ p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        ಪೂರ್ಣಾಙ್ಕ m32[] = {saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 2] * SCALE), saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 3] * SCALE),
                     saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 6] * SCALE), saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 7] * SCALE),
                     saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[10] * SCALE), saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        ಪೂರ್ಣಾಙ್ಕ x = 0;
        ಪ್ರತಿ (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>((m[11] + 0.5f)*SCALE);
        ಪ್ರತಿ( ; x < len * nChannels; x += nChannels )
        {
            ಪೂರ್ಣಾಙ್ಕ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_16s(ಧ್ರುವ short* src, short* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

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
        ಪ್ರತಿ (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        ಪ್ರತಿ( ; x < len * 3; x += 3 )
        {
            ಭಿನ್ನ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಪೂರ್ಣಾಙ್ಕ* dst, ಧ್ರುವ ದ್ವಿಗುಣ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಯದಿ( scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

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
        ಪ್ರತಿ (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ಮೇಲಿನ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ಮೇಲಿನ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ಮೇಲಿನ(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ದ್ವಿಗುಣ b = src[x], g = src[x + 1], r = src[x + 2];
            ಪೂರ್ಣಾಙ್ಕ db = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ಪೂರ್ಣಾಙ್ಕ dg = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ಪೂರ್ಣಾಙ್ಕ dr = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_64f(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಧ್ರುವ ದ್ವಿಗುಣ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಯದಿ( scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

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
        ಪ್ರತಿ (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ದ್ವಿಗುಣ b = src[x], g = src[x + 1], r = src[x + 2];
            ದ್ವಿಗುಣ db = saturate_cast<ದ್ವಿಗುಣ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ದ್ವಿಗುಣ dg = saturate_cast<ದ್ವಿಗುಣ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ದ್ವಿಗುಣ dr = saturate_cast<ದ್ವಿಗುಣ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_( ಧ್ರುವ T* src, T* dst, ಧ್ರುವ WT* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ )
{
    ಪೂರ್ಣಾಙ್ಕ x;

    ಯದಿ( cn == 2 )
    {
        ಪ್ರತಿ( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( cn == 3 )
    {
        ಪ್ರತಿ( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( cn == 4 )
    {
        ಪ್ರತಿ( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ಧ್ರುವ WT* _m = m;
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_8u(ಧ್ರುವ uchar* src, uchar* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಪ್ರತಿ (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಙ್ಕ b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_8s(ಧ್ರುವ schar* src, schar* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
    #ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಪ್ರತಿ (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಙ್ಕ b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_16u(ಧ್ರುವ ushort* src, ushort* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಪ್ರತಿ (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಙ್ಕ b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_16s(ಧ್ರುವ short* src, short* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಪ್ರತಿ (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಙ್ಕ b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಪೂರ್ಣಾಙ್ಕ* dst, ಧ್ರುವ ದ್ವಿಗುಣ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಯದಿ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ಪ್ರತಿ (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ಮೇಲಿನ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ಮೇಲಿನ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ಮೇಲಿನ(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಙ್ಕ b = src[x], g = src[x + 1], r = src[x + 2];
            ಪೂರ್ಣಾಙ್ಕ db = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 0] * b + m[ 3]);
            ಪೂರ್ಣಾಙ್ಕ dg = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[ 5] * g + m[ 7]);
            ಪೂರ್ಣಾಙ್ಕ dr = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_32f(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಧ್ರುವ ಭಿನ್ನ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಯದಿ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಪ್ರತಿ (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ಭಿನ್ನ b = src[x], g = src[x + 1], r = src[x + 2];
            ಭಿನ್ನ db = saturate_cast<ಭಿನ್ನ>(m[ 0] * b + m[ 3]);
            ಭಿನ್ನ dg = saturate_cast<ಭಿನ್ನ>(m[ 5] * g + m[ 7]);
            ಭಿನ್ನ dr = saturate_cast<ಭಿನ್ನ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_64f(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಧ್ರುವ ದ್ವಿಗುಣ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಯದಿ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಙ್ಕ x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ಪ್ರತಿ (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಪ್ರತಿ (; x < len * 3; x += 3) {
            ದ್ವಿಗುಣ b = src[x], g = src[x + 1], r = src[x + 2];
            ದ್ವಿಗುಣ db = saturate_cast<ದ್ವಿಗುಣ>(m[ 0] * b + m[ 3]);
            ದ್ವಿಗುಣ dg = saturate_cast<ದ್ವಿಗುಣ>(m[ 5] * g + m[ 7]);
            ದ್ವಿಗುಣ dr = saturate_cast<ದ್ವಿಗುಣ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, ಶೂನ್ಯ (*fn)(ಧ್ರುವ T*, T*, ಧ್ರುವ WT*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ transformWrap(ಧ್ರುವ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಧ್ರುವ ಶೂನ್ಯ* m, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn)
{
    fn((ಧ್ರುವ T*)src, (T*)dst, (ಧ್ರುವ WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, ಭಿನ್ನ, transform_8u>, transformWrap<schar, ಭಿನ್ನ, transform_8s>, transformWrap<ushort, ಭಿನ್ನ, transform_16u>,
        transformWrap<short, ಭಿನ್ನ, transform_16s>, transformWrap<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, transform_32s>, transformWrap<ಭಿನ್ನ, ಭಿನ್ನ, transform_32f>,
        transformWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, transform_64f>, 0
    };

    ಪ್ರತ್ಯಾಗಮ transformTab[depth];
}

TransformFunc getDiagTransformFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, ಭಿನ್ನ, diagtransform_8u>, transformWrap<schar, ಭಿನ್ನ, diagtransform_8s>, transformWrap<ushort, ಭಿನ್ನ, diagtransform_16u>,
        transformWrap<short, ಭಿನ್ನ, diagtransform_16s>, transformWrap<ಪೂರ್ಣಾಙ್ಕ, ದ್ವಿಗುಣ, diagtransform_32s>, transformWrap<ಭಿನ್ನ, ಭಿನ್ನ, diagtransform_32f>,
        transformWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, diagtransform_64f>, 0
    };

    ಪ್ರತ್ಯಾಗಮ diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ
perspectiveTransform_( ಧ್ರುವ ಶೂನ್ಯ* _src, ಶೂನ್ಯ* _dst, ಧ್ರುವ ಶೂನ್ಯ* _m_ptr, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ scn, ಪೂರ್ಣಾಙ್ಕ dcn )
{
    ಧ್ರುವ T* src = (ಧ್ರುವ T*)_src;
    T* dst = (T*)_dst;
    ಧ್ರುವ ದ್ವಿಗುಣ* m = (ಧ್ರುವ ದ್ವಿಗುಣ*)_m_ptr;
    ಧ್ರುವ ದ್ವಿಗುಣ eps = FLT_EPSILON;
    ಪೂರ್ಣಾಙ್ಕ i;

    ಯದಿ( scn == 2 && dcn == 2 )
    {
        ಪ್ರತಿ( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            ದ್ವಿಗುಣ w = x*m[6] + y*m[7] + m[8];

            ಯದಿ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            ಅನ್ಯಥಾ
                dst[i] = dst[i+1] = (T)0;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( scn == 3 && dcn == 3 )
    {
        ಪ್ರತಿ( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            ದ್ವಿಗುಣ w = x*m[12] + y*m[13] + z*m[14] + m[15];

            ಯದಿ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            ಅನ್ಯಥಾ
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( scn == 3 && dcn == 2 )
    {
        ಪ್ರತಿ( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            ದ್ವಿಗುಣ w = x*m[8] + y*m[9] + z*m[10] + m[11];

            ಯದಿ( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            ಅನ್ಯಥಾ
                dst[0] = dst[1] = (T)0;
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ಧ್ರುವ ದ್ವಿಗುಣ* _m = m + dcn*(scn + 1);
            ದ್ವಿಗುಣ w = _m[scn];
            ಪೂರ್ಣಾಙ್ಕ j, k;
            ಪ್ರತಿ( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            ಯದಿ( fabs(w) > eps )
            {
                _m = m;
                ಪ್ರತಿ( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    ದ್ವಿಗುಣ s = _m[scn];
                    ಪ್ರತಿ( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            ಅನ್ಯಥಾ
                ಪ್ರತಿ( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಯದಿ (depth == CV_32F)
        ಪ್ರತ್ಯಾಗಮ perspectiveTransform_<ಭಿನ್ನ>;
    ಯದಿ (depth == CV_64F)
        ಪ್ರತ್ಯಾಗಮ perspectiveTransform_<ದ್ವಿಗುಣ>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

ಸ್ಥಿರ ಶೂನ್ಯ scaleAdd_32f(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, ಶೂನ್ಯ* _dst,
                         ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ ಶೂನ್ಯ* _alpha)
{
    ಧ್ರುವ ಭಿನ್ನ* src1 = (ಧ್ರುವ ಭಿನ್ನ*)_src1;
    ಧ್ರುವ ಭಿನ್ನ* src2 = (ಧ್ರುವ ಭಿನ್ನ*)_src2;
    ಭಿನ್ನ* dst = (ಭಿನ್ನ*)_dst;
    ಭಿನ್ನ alpha = *(ಧ್ರುವ ಭಿನ್ನ*)_alpha;
    ಪೂರ್ಣಾಙ್ಕ i = 0;
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cWidth = VTraits<v_float32>::vlanes();
    ಪ್ರತಿ (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    ಪ್ರತಿ (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


ಸ್ಥಿರ ಶೂನ್ಯ scaleAdd_64f(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, ಶೂನ್ಯ* _dst,
                         ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ ಶೂನ್ಯ* _alpha)
{
    ಧ್ರುವ ದ್ವಿಗುಣ* src1 = (ಧ್ರುವ ದ್ವಿಗುಣ*)_src1;
    ಧ್ರುವ ದ್ವಿಗುಣ* src2 = (ಧ್ರುವ ದ್ವಿಗುಣ*)_src2;
    ದ್ವಿಗುಣ* dst = (ದ್ವಿಗುಣ*)_dst;
    ದ್ವಿಗುಣ alpha = *(ಧ್ರುವ ದ್ವಿಗುಣ*)_alpha;
    ಪೂರ್ಣಾಙ್ಕ i = 0;
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cWidth = VTraits<v_float64>::vlanes();
    ಪ್ರತಿ (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    ಪ್ರತಿ (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಯದಿ (depth == CV_32F)
        ಪ್ರತ್ಯಾಗಮ scaleAdd_32f;
    ಯದಿ (depth == CV_64F)
        ಪ್ರತ್ಯಾಗಮ scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ದ್ವಿಗುಣ MahalanobisImpl(ಧ್ರುವ Mat& v1, ಧ್ರುವ Mat& v2, ಧ್ರುವ Mat& icovar, ದ್ವಿಗುಣ *diff_buffer /*[len]*/, ಪೂರ್ಣಾಙ್ಕ len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    ದ್ವಿಗುಣ result = 0;

    sz.width *= v1.channels();
    ಯದಿ (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        ಧ್ರುವ T* src1 = v1.ptr<T>();
        ಧ್ರುವ T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/ಪರಿಮಾಣ(src1[0]);
        size_t step2 = v2.step/ಪರಿಮಾಣ(src2[0]);
        ದ್ವಿಗುಣ* diff = diff_buffer;
        ಧ್ರುವ T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/ಪರಿಮಾಣ(mat[0]);

        ಪ್ರತಿ (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++, mat += matstep)
        {
            ದ್ವಿಗುಣ row_sum = 0;
            ಪೂರ್ಣಾಙ್ಕ j = 0;
#ಯದಿ CV_ENABLE_UNROLLED
            ಪ್ರತಿ(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            ಪ್ರತಿ (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

MahalanobisImplFunc getMahalanobisImplFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಯದಿ (depth == CV_32F)
        ಪ್ರತ್ಯಾಗಮ (MahalanobisImplFunc)MahalanobisImpl<ಭಿನ್ನ>;
    ಯದಿ (depth == CV_64F)
        ಪ್ರತ್ಯಾಗಮ (MahalanobisImplFunc)MahalanobisImpl<ದ್ವಿಗುಣ>;
    CV_Assert(0 && "Not supported");
}


#ಯದಿ !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> ಸ್ಥಿರ ಶೂನ್ಯ
MulTransposedR(ಧ್ರುವ Mat& srcmat, ಧ್ರುವ Mat& dstmat, ಧ್ರುವ Mat& deltamat, ದ್ವಿಗುಣ scale)
{
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಧ್ರುವ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ಧ್ರುವ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/ಪರಿಮಾಣ(src[0]);
    size_t dststep = dstmat.step/ಪರಿಮಾಣ(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/ಪರಿಮಾಣ(delta[0]) : 0;
    ಪೂರ್ಣಾಙ್ಕ delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    ಪೂರ್ಣಾಙ್ಕ buf_size = size.height*ಪರಿಮಾಣ(dT);
    AutoBuffer<uchar> buf;

    ಯದಿ( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    ಯದಿ( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        ಪ್ರತಿ( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#ಯದಿ CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    ಯದಿ( !delta )
        ಪ್ರತಿ( i = 0; i < size.width; i++, tdst += dststep )
        {
            ಪ್ರತಿ( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            ಪ್ರತಿ( j = i; j <= size.width - 4; j += 4 )
            {
#ಯದಿ CV_SIMD128_64F
                ಯದಿ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ಧ್ರುವ ದ್ವಿಗುಣ *tsrc = (ದ್ವಿಗುಣ*)(src + j);

                    ಪ್ರತಿ( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((ದ್ವಿಗುಣ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((ದ್ವಿಗುಣ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ದ್ವಿಗುಣ*)(tdst+j+2), v_mul(s1, v_scale));
                } ಅನ್ಯಥಾ
#endif
                {
                    ದ್ವಿಗುಣ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ಧ್ರುವ sT *tsrc = src + j;

                    ಪ್ರತಿ( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        ದ್ವಿಗುಣ a = col_buf[k];
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

            ಪ್ರತಿ( ; j < size.width; j++ )
            {
                ದ್ವಿಗುಣ s0 = 0;
                ಧ್ರುವ sT *tsrc = src + j;

                ಪ್ರತಿ( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (ದ್ವಿಗುಣ)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    ಅನ್ಯಥಾ
        ಪ್ರತಿ( i = 0; i < size.width; i++, tdst += dststep )
        {
            ಯದಿ( !delta_buf )
                ಪ್ರತಿ( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            ಅನ್ಯಥಾ
                ಪ್ರತಿ( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            ಪ್ರತಿ( j = i; j <= size.width - 4; j += 4 )
            {
#ಯದಿ CV_SIMD128_64F
                ಯದಿ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ಧ್ರುವ ದ್ವಿಗುಣ *tsrc = (ದ್ವಿಗುಣ*)(src + j);
                    ಧ್ರುವ ದ್ವಿಗುಣ *d = (ದ್ವಿಗುಣ*)(delta_buf ? delta_buf : delta + j);

                    ಪ್ರತಿ( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((ದ್ವಿಗುಣ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((ದ್ವಿಗುಣ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ದ್ವಿಗುಣ*)(tdst+j+2), v_mul(s1, v_scale));
                }
                ಅನ್ಯಥಾ
#endif

                {
                    ದ್ವಿಗುಣ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ಧ್ರುವ sT *tsrc = src + j;
                    ಧ್ರುವ dT *d = delta_buf ? delta_buf : delta + j;

                    ಪ್ರತಿ( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        ದ್ವಿಗುಣ a = col_buf[k];
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

            ಪ್ರತಿ( ; j < size.width; j++ )
            {
                ದ್ವಿಗುಣ s0 = 0;
                ಧ್ರುವ sT *tsrc = src + j;
                ಧ್ರುವ dT *d = delta_buf ? delta_buf : delta + j;

                ಪ್ರತಿ( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (ದ್ವಿಗುಣ)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> ಸ್ಥಿರ ಶೂನ್ಯ
MulTransposedL(ಧ್ರುವ Mat& srcmat, ಧ್ರುವ Mat& dstmat, ಧ್ರುವ Mat& deltamat, ದ್ವಿಗುಣ scale)
{
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಧ್ರುವ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ಧ್ರುವ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/ಪರಿಮಾಣ(src[0]);
    size_t dststep = dstmat.step/ಪರಿಮಾಣ(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/ಪರಿಮಾಣ(delta[0]) : 0;
    ಪೂರ್ಣಾಙ್ಕ delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    ಯದಿ( !delta )
        ಪ್ರತಿ( i = 0; i < size.height; i++, tdst += dststep )
            ಪ್ರತಿ( j = i; j < size.height; j++ )
            {
                ದ್ವಿಗುಣ s = 0;
                ಧ್ರುವ sT *tsrc1 = src + i*srcstep;
                ಧ್ರುವ sT *tsrc2 = src + j*srcstep;
#ಯದಿ CV_SIMD128_64F
                ಯದಿ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ಧ್ರುವ ದ್ವಿಗುಣ *v_tsrc1 = (ದ್ವಿಗುಣ *)(tsrc1);
                    ಧ್ರುವ ದ್ವಿಗುಣ *v_tsrc2 = (ದ್ವಿಗುಣ *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    ಪ್ರತಿ( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                ಅನ್ಯಥಾ
#endif
                {
                    ಪ್ರತಿ( k = 0; k <= size.width - 4; k += 4 )
                        s += (ದ್ವಿಗುಣ)tsrc1[k]*tsrc2[k] + (ದ್ವಿಗುಣ)tsrc1[k+1]*tsrc2[k+1] +
                             (ದ್ವಿಗುಣ)tsrc1[k+2]*tsrc2[k+2] + (ದ್ವಿಗುಣ)tsrc1[k+3]*tsrc2[k+3];
                }
                ಪ್ರತಿ( ; k < size.width; k++ )
                    s += (ದ್ವಿಗುಣ)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    ಅನ್ಯಥಾ
    {
        dT delta_buf[4];
        ಪೂರ್ಣಾಙ್ಕ delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*ಪರಿಮಾಣ(dT));
        dT* row_buf = (dT*)buf.data();

        ಪ್ರತಿ( i = 0; i < size.height; i++, tdst += dststep )
        {
            ಧ್ರುವ sT *tsrc1 = src + i*srcstep;
            ಧ್ರುವ dT *tdelta1 = delta + i*deltastep;

            ಯದಿ( delta_cols < size.width )
                ಪ್ರತಿ( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            ಅನ್ಯಥಾ
                ಪ್ರತಿ( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            ಪ್ರತಿ( j = i; j < size.height; j++ )
            {
                ದ್ವಿಗುಣ s = 0;
                ಧ್ರುವ sT *tsrc2 = src + j*srcstep;
                ಧ್ರುವ dT *tdelta2 = delta + j*deltastep;
                ಯದಿ( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#ಯದಿ CV_SIMD128_64F
                ಯದಿ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ಧ್ರುವ ದ್ವಿಗುಣ *v_tsrc2 = (ದ್ವಿಗುಣ *)(tsrc2);
                    ಧ್ರುವ ದ್ವಿಗುಣ *v_tdelta2 = (ದ್ವಿಗುಣ *)(tdelta2);
                    ಧ್ರುವ ದ್ವಿಗುಣ *v_row_buf = (ದ್ವಿಗುಣ *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    ಪ್ರತಿ( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (ಧ್ರುವ dT *)(v_tdelta2);
                }
                ಅನ್ಯಥಾ
#endif
                {
                    ಪ್ರತಿ( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (ದ್ವಿಗುಣ)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (ದ್ವಿಗುಣ)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (ದ್ವಿಗುಣ)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (ದ್ವಿಗುಣ)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                ಪ್ರತಿ( ; k < size.width; k++, tdelta2++ )
                    s += (ದ್ವಿಗುಣ)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(ಪೂರ್ಣಾಙ್ಕ stype, ಪೂರ್ಣಾಙ್ಕ dtype, ತಾರ್ಕಿಕ ata)
{
    MulTransposedFunc func = ರಿಕ್ತ;
    ಯದಿ (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,ಭಿನ್ನ>
                   : MulTransposedL<uchar,ಭಿನ್ನ>;
    }
    ಅನ್ಯಥಾ ಯದಿ (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,ದ್ವಿಗುಣ>
                   : MulTransposedL<uchar,ದ್ವಿಗುಣ>;
    }
    ಅನ್ಯಥಾ ಯದಿ(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,ಭಿನ್ನ>
                   : MulTransposedL<ushort,ಭಿನ್ನ>;
    }
    ಅನ್ಯಥಾ ಯದಿ(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,ದ್ವಿಗುಣ>
                   : MulTransposedL<ushort,ದ್ವಿಗುಣ>;
    }
    ಅನ್ಯಥಾ ಯದಿ(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,ಭಿನ್ನ>
                   : MulTransposedL<short,ಭಿನ್ನ>;
    }
    ಅನ್ಯಥಾ ಯದಿ(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,ದ್ವಿಗುಣ>
                   : MulTransposedL<short,ದ್ವಿಗುಣ>;
    }
    ಅನ್ಯಥಾ ಯದಿ(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ಭಿನ್ನ,ಭಿನ್ನ>
                   : MulTransposedL<ಭಿನ್ನ,ಭಿನ್ನ>;
    }
    ಅನ್ಯಥಾ ಯದಿ(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ಭಿನ್ನ,ದ್ವಿಗುಣ>
                   : MulTransposedL<ಭಿನ್ನ,ದ್ವಿಗುಣ>;
    }
    ಅನ್ಯಥಾ ಯದಿ(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ>
                   : MulTransposedL<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ>;
    }
    CV_Assert(func && "Not supported");
    ಪ್ರತ್ಯಾಗಮ func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ದ್ವಿಗುಣ dotProd_(ಧ್ರುವ T* src1, ಧ್ರುವ T* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    ಪೂರ್ಣಾಙ್ಕ i = 0;
    ದ್ವಿಗುಣ result = 0;

#ಯದಿ CV_ENABLE_UNROLLED
    ಪ್ರತಿ( ; i <= len - 4; i += 4 )
        result += (ದ್ವಿಗುಣ)src1[i]*src2[i] + (ದ್ವಿಗುಣ)src1[i+1]*src2[i+1] +
            (ದ್ವಿಗುಣ)src1[i+2]*src2[i+2] + (ದ್ವಿಗುಣ)src1[i+3]*src2[i+3];
#endif
    ಪ್ರತಿ( ; i < len; i++ )
        result += (ದ್ವಿಗುಣ)src1[i]*src2[i];

    ಪ್ರತ್ಯಾಗಮ result;
}


ದ್ವಿಗುಣ dotProd_8u(ಧ್ರುವ uchar* src1, ಧ್ರುವ uchar* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    ದ್ವಿಗುಣ r = 0;
    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಙ್ಕ len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    ಯಾವತ್ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cWidth = VTraits<v_uint16>::vlanes();

        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪ್ರತಿ (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        ಪ್ರತಿ (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (ದ್ವಿಗುಣ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ಪ್ರತ್ಯಾಗಮ r + dotProd_(src1, src2, len - i);
}


ದ್ವಿಗುಣ dotProd_8s(ಧ್ರುವ schar* src1, ಧ್ರುವ schar* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    ದ್ವಿಗುಣ r = 0.0;
    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಙ್ಕ len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    ಯಾವತ್ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cWidth = VTraits<v_int16>::vlanes();

        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪ್ರತಿ (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        ಪ್ರತಿ (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (ದ್ವಿಗುಣ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    ಪ್ರತ್ಯಾಗಮ r + dotProd_(src1, src2, len - i);
}

ದ್ವಿಗುಣ dotProd_16u(ಧ್ರುವ ushort* src1, ಧ್ರುವ ushort* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    ದ್ವಿಗುಣ r = 0.0;
    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಙ್ಕ len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    ಯಾವತ್ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cWidth = VTraits<v_uint16>::vlanes();

        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪ್ರತಿ (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (ದ್ವಿಗುಣ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ಪ್ರತ್ಯಾಗಮ r + dotProd_(src1, src2, len - i);
}

ದ್ವಿಗುಣ dotProd_16s(ಧ್ರುವ short* src1, ಧ್ರುವ short* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    ದ್ವಿಗುಣ r = 0.0;
    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಙ್ಕ len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    ಯಾವತ್ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cWidth = VTraits<v_int16>::vlanes();

        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪ್ರತಿ (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (ದ್ವಿಗುಣ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ಪ್ರತ್ಯಾಗಮ r + dotProd_(src1, src2, len - i);
}

ದ್ವಿಗುಣ dotProd_32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
#ಯದಿ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ದ್ವಿಗುಣ r = .0;
    ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#ಯದಿ CV_SIMD_WIDTH == 16
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    ಪ್ರತಿ (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    ಪ್ರತಿ (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    ಪ್ರತ್ಯಾಗಮ r + dotProd_(src1, src2, len - i);
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ dotProd_(src1, src2, len);
#endif
}

ದ್ವಿಗುಣ dotProd_32f(ಧ್ರುವ ಭಿನ್ನ* src1, ಧ್ರುವ ಭಿನ್ನ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    ದ್ವಿಗುಣ r = 0.0;
    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಙ್ಕ len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    ಯಾವತ್ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        ಪೂರ್ಣಾಙ್ಕ j = 0;
        ಪೂರ್ಣಾಙ್ಕ cWidth = VTraits<v_float32>::vlanes();

#ಯದಿ CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        ಪ್ರತಿ (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
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

        ಪ್ರತಿ (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ಪ್ರತ್ಯಾಗಮ r + dotProd_(src1, src2, len - i);
}

ದ್ವಿಗುಣ dotProd_64f(ಧ್ರುವ ದ್ವಿಗುಣ* src1, ಧ್ರುವ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    ಪ್ರತ್ಯಾಗಮ dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
