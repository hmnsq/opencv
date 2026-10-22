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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LAPACK
#ವ್ಯಾಖ್ಯಾನಿಸು CV_GEMM_BASELINE_ONLY
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_MAHALANOBIS_BASELINE_ONLY
#ವ್ಯಾಖ್ಯಾನಿಸು CV_MULTRANSPOSED_BASELINE_ONLY

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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_GEMM_SIMD_ENABLED
#ಆದರೆ defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#ವ್ಯಾಖ್ಯಾನಿಸು CV_GEMM_SIMD_ENABLED 0
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_GEMM_SIMD_ENABLED 1
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv {

// forward declarations
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*TransformFunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*Scale_ಕೂಡಿಸು_Func)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* alpha);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*MulTransposedFunc)(ಸ್ಥಿರಾಂಕ Mat& src, ಸ್ಥಿರಾಂಕ/*preallocated*/ Mat& dst, ಸ್ಥಿರಾಂಕ Mat& delta, ದ್ವಿಗುಣ scale);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ (*MahalanobisImplFunc)(ಸ್ಥಿರಾಂಕ Mat& v1, ಸ್ಥಿರಾಂಕ Mat& v2, ಸ್ಥಿರಾಂಕ Mat& icovar, ದ್ವಿಗುಣ *diff_ಬಫರ್ /*[len]*/, ಪೂರ್ಣಾಂಕ len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

ಶೂನ್ಯ gemm32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
             ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
             ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags);
ಶೂನ್ಯ gemm64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
             ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
             ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags);
ಶೂನ್ಯ gemm32fc(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
              ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
              ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags);
ಶೂನ್ಯ gemm64fc(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
              ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
              ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags);

TransformFunc getTransformFunc(ಪೂರ್ಣಾಂಕ ಆಳ);
TransformFunc getDiagTransformFunc(ಪೂರ್ಣಾಂಕ ಆಳ);
TransformFunc getPerspectiveTransform(ಪೂರ್ಣಾಂಕ ಆಳ);

Scale_ಕೂಡಿಸು_Func get_Scale_ಕೂಡಿಸು_Func(ಪೂರ್ಣಾಂಕ ಆಳ);

MahalanobisImplFunc getMahalanobisImplFunc(ಪೂರ್ಣಾಂಕ ಆಳ);

MulTransposedFunc getMulTransposedFunc(ಪೂರ್ಣಾಂಕ stype, ಪೂರ್ಣಾಂಕ dtype, ತಾರ್ಕಿಕ ata);

ದ್ವಿಗುಣ dotProd_8u(ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಪೂರ್ಣಾಂಕ len);
ದ್ವಿಗುಣ dotProd_8s(ಸ್ಥಿರಾಂಕ schar* src1, ಸ್ಥಿರಾಂಕ schar* src2, ಪೂರ್ಣಾಂಕ len);
ದ್ವಿಗುಣ dotProd_16u(ಸ್ಥಿರಾಂಕ ushort* src1, ಸ್ಥಿರಾಂಕ ushort* src2, ಪೂರ್ಣಾಂಕ len);
ದ್ವಿಗುಣ dotProd_16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src2, ಪೂರ್ಣಾಂಕ len);
ದ್ವಿಗುಣ dotProd_32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src2, ಪೂರ್ಣಾಂಕ len);
ದ್ವಿಗುಣ dotProd_32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, ಪೂರ್ಣಾಂಕ len);
ದ್ವಿಗುಣ dotProd_64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಂಕ len);



#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ಆದರೆ !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_ಬಹುಲಕ)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

ಸ್ಥಿರ ಶೂನ್ಯ
GEMM_Copy_ಖಂಡ( ಸ್ಥಿರಾಂಕ uchar* src, size_t src_ಹೆಜ್ಜೆ,
                uchar* dst, size_t dst_ಹೆಜ್ಜೆ,
                ಗಾತ್ರ_2 size, size_t pix_ಗಾತ್ರ )
{
    ಪೂರ್ಣಾಂಕ j;
    size.ಅಗಲ *= (ಪೂರ್ಣಾಂಕ)(pix_ಗಾತ್ರ / ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));

    ಗಾಗಿ( ; size.ಎತ್ತರ--; src += src_ಹೆಜ್ಜೆ, dst += dst_ಹೆಜ್ಜೆ )
    {
        j=0;
         #ಆದರೆ CV_ENABLE_UNROLLED
        ಗಾಗಿ( ; j <= size.ಅಗಲ - 4; j += 4 )
        {
            ಪೂರ್ಣಾಂಕ t0 = ((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)src)[j];
            ಪೂರ್ಣಾಂಕ t1 = ((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)src)[j+1];
            ((ಪೂರ್ಣಾಂಕ*)dst)[j] = t0;
            ((ಪೂರ್ಣಾಂಕ*)dst)[j+1] = t1;
            t0 = ((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)src)[j+2];
            t1 = ((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)src)[j+3];
            ((ಪೂರ್ಣಾಂಕ*)dst)[j+2] = t0;
            ((ಪೂರ್ಣಾಂಕ*)dst)[j+3] = t1;
        }
        #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            ((ಪೂರ್ಣಾಂಕ*)dst)[j] = ((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)src)[j];
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
GEMM_Transpose_ಖಂಡ( ಸ್ಥಿರಾಂಕ uchar* src, size_t src_ಹೆಜ್ಜೆ,
                     uchar* dst, size_t dst_ಹೆಜ್ಜೆ,
                     ಗಾತ್ರ_2 size, size_t pix_ಗಾತ್ರ )
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಗಾಗಿ( i = 0; i < size.ಅಗಲ; i++, dst += dst_ಹೆಜ್ಜೆ, src += pix_ಗಾತ್ರ )
    {
        ಸ್ಥಿರಾಂಕ uchar* _src = src;
        ಆಯ್ಕೆ( pix_ಗಾತ್ರ )
        {
        ಸ್ಥಿತಿ ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ):
            ಗಾಗಿ( j = 0; j < size.ಎತ್ತರ; j++, _src += src_ಹೆಜ್ಜೆ )
                ((ಪೂರ್ಣಾಂಕ*)dst)[j] = ((ಪೂರ್ಣಾಂಕ*)_src)[0];
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ)*2:
            ಗಾಗಿ( j = 0; j < size.ಎತ್ತರ*2; j += 2, _src += src_ಹೆಜ್ಜೆ )
            {
                ಪೂರ್ಣಾಂಕ t0 = ((ಪೂರ್ಣಾಂಕ*)_src)[0];
                ಪೂರ್ಣಾಂಕ t1 = ((ಪೂರ್ಣಾಂಕ*)_src)[1];
                ((ಪೂರ್ಣಾಂಕ*)dst)[j] = t0;
                ((ಪೂರ್ಣಾಂಕ*)dst)[j+1] = t1;
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ)*4:
            ಗಾಗಿ( j = 0; j < size.ಎತ್ತರ*4; j += 4, _src += src_ಹೆಜ್ಜೆ )
            {
                ಪೂರ್ಣಾಂಕ t0 = ((ಪೂರ್ಣಾಂಕ*)_src)[0];
                ಪೂರ್ಣಾಂಕ t1 = ((ಪೂರ್ಣಾಂಕ*)_src)[1];
                ((ಪೂರ್ಣಾಂಕ*)dst)[j] = t0;
                ((ಪೂರ್ಣಾಂಕ*)dst)[j+1] = t1;
                t0 = ((ಪೂರ್ಣಾಂಕ*)_src)[2];
                t1 = ((ಪೂರ್ಣಾಂಕ*)_src)[3];
                ((ಪೂರ್ಣಾಂಕ*)dst)[j+2] = t0;
                ((ಪೂರ್ಣಾಂಕ*)dst)[j+3] = t1;
            }
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            CV_Assert(0);
            ಹಿಂತಿರುಗಿ;
        }
    }
}

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ST> ರಚನೆ VecTraits;

ಮಾದರಿ<> ರಚನೆ VecTraits<ದಶಮಾಂಶ> {
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ v_float32 VT;
    ಸ್ಥಿರ ಸಾಲೊಳಗೆ VT zero() { ಹಿಂತಿರುಗಿ vx_setzero_f32(); }
    ಸ್ಥಿರ ಸಾಲೊಳಗೆ VT load(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* p) { ಹಿಂತಿರುಗಿ vx_ತುಂಬು(p); }
    ಸ್ಥಿರ ಸಾಲೊಳಗೆ VT setall(ದಶಮಾಂಶ v) { ಹಿಂತಿರುಗಿ vx_setall_f32(v); }
};
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ಮಾದರಿ<> ರಚನೆ VecTraits<ದ್ವಿಗುಣ> {
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ v_float64 VT;
    ಸ್ಥಿರ ಸಾಲೊಳಗೆ VT zero() { ಹಿಂತಿರುಗಿ vx_setzero_f64(); }
    ಸ್ಥಿರ ಸಾಲೊಳಗೆ VT load(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* p) { ಹಿಂತಿರುಗಿ vx_ತುಂಬು(p); }
    ಸ್ಥಿರ ಸಾಲೊಳಗೆ VT setall(ದ್ವಿಗುಣ v) { ಹಿಂತಿರುಗಿ vx_setall_f64(v); }
};
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ST>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ST simd_Dot_ಗುಣಲಬ್ಧ(ಸ್ಥಿರಾಂಕ ST* a, ಸ್ಥಿರಾಂಕ ST* b, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ& k)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು VecTraits<ST>::VT VecT;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VecT>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    ಗಾಗಿ( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(vs0, vs1), v_ಕೂಡಿಸು(vs2, vs3));
    ಗಾಗಿ( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    ಹಿಂತಿರುಗಿ v_reduce_ಮೊತ್ತ(vs0);
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ simd_Dot_ಗುಣಲಬ್ಧ_f32f64(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* a, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ& k)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes_f32 = VTraits<v_float32>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    ಗಾಗಿ( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_ತುಂಬು(a + k);
        v_float32 vb0 = vx_ತುಂಬು(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_ಮೇಲಿನ(va0), v_cvt_f64_ಮೇಲಿನ(vb0), vs1);

        v_float32 va1 = vx_ತುಂಬು(a + k + vlanes_f32);
        v_float32 vb1 = vx_ತುಂಬು(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_ಮೇಲಿನ(va1), v_cvt_f64_ಮೇಲಿನ(vb1), vs3);
    }
    vs0 = v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(vs0, vs1), v_ಕೂಡಿಸು(vs2, vs3));
    ಗಾಗಿ( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_ತುಂಬು(a + k);
        v_float32 vb = vx_ತುಂಬು(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_ಮೇಲಿನ(va), v_cvt_f64_ಮೇಲಿನ(vb), vs0);
    }
    ಹಿಂತಿರುಗಿ v_reduce_ಮೊತ್ತ(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ simdGEMM_kj_f32f64(
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* a_ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b_ದತ್ತಾಂಶ_2, size_t b_ಹೆಜ್ಜೆ,
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* c_ದತ್ತಾಂಶ, size_t c_step1,
    ದಶಮಾಂಶ* d_ದತ್ತಾಂಶ,
    ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m,
    ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta,
    ಪೂರ್ಣಾಂಕ& j)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes_f32 = VTraits<v_float32>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes_f64 = VTraits<v_float64>::vlanes();

    ಆದರೆ( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ max_m = 1600 / (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ದಶಮಾಂಶ);
        cv::Auto_ಬಫರ್<ದ್ವಿಗುಣ> _s_buf(max_m);
        ದ್ವಿಗುಣ* s_buf = _s_buf.ದತ್ತಾಂಶ();

        {
            v_float64 vz = vx_setzero_f64();
            ಗಾಗಿ( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            ಗಾಗಿ( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b_ptr = b_ದತ್ತಾಂಶ_2;
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < n; k++, b_ptr += b_ಹೆಜ್ಜೆ )
            {
                v_float64 va = vx_setall_f64((ದ್ವಿಗುಣ)a_ದತ್ತಾಂಶ[k]);
                j = 0;
                ಗಾಗಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_ತುಂಬು(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_ಮೇಲಿನ(vb);
                    v_float64 vd_lo = vx_ತುಂಬು(s_buf + j);
                    v_float64 vd_hi = vx_ತುಂಬು(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                ದ್ವಿಗುಣ a_val = (ದ್ವಿಗುಣ)a_ದತ್ತಾಂಶ[k];
                ಗಾಗಿ( ; j < m; j++ )
                    s_buf[j] += a_val * (ದ್ವಿಗುಣ)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;
            j = 0;
            ಆದರೆ( !c_ದತ್ತಾಂಶ )
            {
                ಗಾಗಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_ತುಂಬು(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_ತುಂಬು(s_buf + j + vlanes_f64), valpha);
                    v_store(d_ದತ್ತಾಂಶ + j, v_cvt_f32(vs_lo, vs_hi));
                }
                ಗಾಗಿ( ; j < m; j++ )
                    d_ದತ್ತಾಂಶ[j] = (ದಶಮಾಂಶ)(s_buf[j] * alpha);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                ಗಾಗಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_ತುಂಬು(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_ತುಂಬು(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_ತುಂಬು(c_ದತ್ತಾಂಶ_2 + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_ಮೇಲಿನ(vc), vbeta, vs_hi);
                    v_store(d_ದತ್ತಾಂಶ + j, v_cvt_f32(vs_lo, vs_hi));
                }
                ಗಾಗಿ( ; j < m; j++ )
                    d_ದತ್ತಾಂಶ[j] = (ದಶಮಾಂಶ)(s_buf[j] * alpha + (ದ್ವಿಗುಣ)c_ದತ್ತಾಂಶ_2[j] * beta);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ( j = 0; j < m; j++, c_ದತ್ತಾಂಶ_2 += c_step1 )
                    d_ದತ್ತಾಂಶ[j] = (ದಶಮಾಂಶ)(s_buf[j] * alpha + (ದ್ವಿಗುಣ)c_ದತ್ತಾಂಶ_2[0] * beta);
            }
        }
        j = m;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( vlanes_f32 * (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ದಶಮಾಂಶ) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;
        ಗಾಗಿ( ; j <= m - vlanes_f32; j += vlanes_f32, c_ದತ್ತಾಂಶ_2 += vlanes_f32 * c_step1 )
        {
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b = b_ದತ್ತಾಂಶ_2 + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < n; k++, b += b_ಹೆಜ್ಜೆ )
            {
                v_float64 va = vx_setall_f64((ದ್ವಿಗುಣ)a_ದತ್ತಾಂಶ[k]);
                v_float32 vb = vx_ತುಂಬು(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_ಮೇಲಿನ(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            ಆದರೆ( !c_ದತ್ತಾಂಶ )
            {
                v_store(d_ದತ್ತಾಂಶ + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_ತುಂಬು(c_ದತ್ತಾಂಶ_2);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_ಮೇಲಿನ(vc), vbeta, vs_hi);
                v_store(d_ದತ್ತಾಂಶ + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) ದಶಮಾಂಶ buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ jj = 0; jj < vlanes_f32; jj++ )
                    d_ದತ್ತಾಂಶ[j + jj] = buf[jj] + c_ದತ್ತಾಂಶ_2[jj * c_step1] * (ದಶಮಾಂಶ)beta;
            }
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ST>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ simdGEMM_kj(
    ಸ್ಥಿರಾಂಕ ST* a_ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ST* b_ದತ್ತಾಂಶ_2, size_t b_ಹೆಜ್ಜೆ,
    ಸ್ಥಿರಾಂಕ ST* c_ದತ್ತಾಂಶ, size_t c_step1,
    ST* d_ದತ್ತಾಂಶ,
    ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m,
    ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta,
    ಪೂರ್ಣಾಂಕ& j)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು VecTraits<ST>::VT VecT;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VecT>::vlanes();

    ಆದರೆ( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ max_m = 1600 / (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ST);
        CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            ಗಾಗಿ( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            ಗಾಗಿ( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ಸ್ಥಿರಾಂಕ ST* b_ptr = b_ದತ್ತಾಂಶ_2;
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < n; k++, b_ptr += b_ಹೆಜ್ಜೆ )
            {
                VecT va = VecTraits<ST>::setall(a_ದತ್ತಾಂಶ[k]);
                j = 0;
                ಗಾಗಿ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_ತುಂಬು_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_ದತ್ತಾಂಶ[k];
                ಗಾಗಿ( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ಸ್ಥಿರಾಂಕ ST* c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;
            j = 0;
            ಆದರೆ( !c_ದತ್ತಾಂಶ )
            {
                ಗಾಗಿ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_ತುಂಬು_aligned(s_buf + j), valpha);
                    v_store(d_ದತ್ತಾಂಶ + j, vs);
                }
                ಗಾಗಿ( ; j < m; j++ )
                    d_ದತ್ತಾಂಶ[j] = s_buf[j] * (ST)alpha;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                ಗಾಗಿ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_ತುಂಬು_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_ದತ್ತಾಂಶ_2 + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_ದತ್ತಾಂಶ + j, vs);
                }
                ಗಾಗಿ( ; j < m; j++ )
                    d_ದತ್ತಾಂಶ[j] = s_buf[j] * (ST)alpha + c_ದತ್ತಾಂಶ_2[j] * (ST)beta;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ( j = 0; j < m; j++, c_ದತ್ತಾಂಶ_2 += c_step1 )
                    d_ದತ್ತಾಂಶ[j] = s_buf[j] * (ST)alpha + c_ದತ್ತಾಂಶ_2[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( vlanes * (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ಸ್ಥಿರಾಂಕ ST* c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;
        ಗಾಗಿ( ; j <= m - vlanes; j += vlanes, c_ದತ್ತಾಂಶ_2 += vlanes * c_step1 )
        {
            ಸ್ಥಿರಾಂಕ ST* b = b_ದತ್ತಾಂಶ_2 + j;
            VecT vs = VecTraits<ST>::zero();
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < n; k++, b += b_ಹೆಜ್ಜೆ )
            {
                VecT va = VecTraits<ST>::setall(a_ದತ್ತಾಂಶ[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            ಆದರೆ( !c_ದತ್ತಾಂಶ )
            {
                v_store(d_ದತ್ತಾಂಶ + j, vs);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_ದತ್ತಾಂಶ_2);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_ದತ್ತಾಂಶ + j, vs);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_DECL_ALIGNED(CV_SIMD_ಅಗಲ) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ jj = 0; jj < vlanes; jj++ )
                    d_ದತ್ತಾಂಶ[j + jj] = buf[jj] + c_ದತ್ತಾಂಶ_2[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#ಆದರೆ_ಅಂತ್ಯ


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT> ಸ್ಥಿರ ಶೂನ್ಯ
GEMMSingleMul( ಸ್ಥಿರಾಂಕ T* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
               ಸ್ಥಿರಾಂಕ T* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
               ಸ್ಥಿರಾಂಕ T* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
               T* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
               ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
               ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    ಪೂರ್ಣಾಂಕ i, j, k, n = a_ಗಾತ್ರ.ಅಗಲ, m = d_ಗಾತ್ರ.ಅಗಲ, drows = d_ಗಾತ್ರ.ಎತ್ತರ;
    ಸ್ಥಿರಾಂಕ T *a_ದತ್ತಾಂಶ_2 = a_ದತ್ತಾಂಶ, *b_ದತ್ತಾಂಶ_2 = b_ದತ್ತಾಂಶ, *c_ದತ್ತಾಂಶ = c_ದತ್ತಾಂಶ_2;
    cv::Auto_ಬಫರ್<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_ಹೆಜ್ಜೆ;

    a_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(a_ದತ್ತಾಂಶ[0]);
    b_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(b_ದತ್ತಾಂಶ[0]);
    c_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(c_ದತ್ತಾಂಶ_2[0]);
    d_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(d_ದತ್ತಾಂಶ[0]);
    a_step0 = a_ಹೆಜ್ಜೆ;
    a_step1 = 1;

    ಆದರೆ( !c_ದತ್ತಾಂಶ_2 )
        c_step0 = c_step1 = 0;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( !(flags & GEMM_3_T) )
        c_step0 = c_ಹೆಜ್ಜೆ, c_step1 = 1;
    ಇಲ್ಲದಿದ್ದರೆ
        c_step0 = 1, c_step1 = c_ಹೆಜ್ಜೆ;

    ಆದರೆ( flags & GEMM_1_T )
    {
        CV_ಅದಲುಬದಲು( a_step0, a_step1, t_ಹೆಜ್ಜೆ );
        n = a_ಗಾತ್ರ.ಎತ್ತರ;
        ಆದರೆ( a_ಹೆಜ್ಜೆ > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.ದತ್ತಾಂಶ();
        }
    }

    ಆದರೆ( n == 1 ) /* external product */
    {
        cv::Auto_ಬಫರ್<T> _b_buf;
        T* b_buf = 0;

        ಆದರೆ( a_ಹೆಜ್ಜೆ > 1 && a_ಗಾತ್ರ.ಎತ್ತರ > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.ದತ್ತಾಂಶ();
            ಗಾಗಿ( k = 0; k < drows; k++ )
                a_buf[k] = a_ದತ್ತಾಂಶ[a_ಹೆಜ್ಜೆ*k];
            a_ದತ್ತಾಂಶ = a_buf;
        }

        ಆದರೆ( b_ಹೆಜ್ಜೆ > 1 )
        {
            _b_buf.allocate(d_ಗಾತ್ರ.ಅಗಲ);
            b_buf = _b_buf.ದತ್ತಾಂಶ();
            ಗಾಗಿ( j = 0; j < d_ಗಾತ್ರ.ಅಗಲ; j++ )
                b_buf[j] = b_ದತ್ತಾಂಶ[j*b_ಹೆಜ್ಜೆ];
            b_ದತ್ತಾಂಶ = b_buf;
        }

        ಗಾಗಿ( i = 0; i < drows; i++, c_ದತ್ತಾಂಶ += c_step0, d_ದತ್ತಾಂಶ += d_ಹೆಜ್ಜೆ )
        {
            WT al = WT(a_ದತ್ತಾಂಶ[i])*alpha;
            c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;
            ಗಾಗಿ( j = 0; j <= d_ಗಾತ್ರ.ಅಗಲ - 2; j += 2, c_ದತ್ತಾಂಶ_2 += 2*c_step1 )
            {
                WT s0 = al*WT(b_ದತ್ತಾಂಶ[j]);
                WT s1 = al*WT(b_ದತ್ತಾಂಶ[j+1]);
                ಆದರೆ( !c_ದತ್ತಾಂಶ_2 )
                {
                    d_ದತ್ತಾಂಶ[j] = T(s0);
                    d_ದತ್ತಾಂಶ[j+1] = T(s1);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    d_ದತ್ತಾಂಶ[j] = T(s0 + WT(c_ದತ್ತಾಂಶ_2[0])*beta);
                    d_ದತ್ತಾಂಶ[j+1] = T(s1 + WT(c_ದತ್ತಾಂಶ_2[c_step1])*beta);
                }
            }

            ಗಾಗಿ( ; j < d_ಗಾತ್ರ.ಅಗಲ; j++, c_ದತ್ತಾಂಶ_2 += c_step1 )
            {
                WT s0 = al*WT(b_ದತ್ತಾಂಶ[j]);
                ಆದರೆ( !c_ದತ್ತಾಂಶ_2 )
                    d_ದತ್ತಾಂಶ[j] = T(s0);
                ಇಲ್ಲದಿದ್ದರೆ
                    d_ದತ್ತಾಂಶ[j] = T(s0 + WT(c_ದತ್ತಾಂಶ_2[0])*beta);
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( flags & GEMM_2_T ) /* A * Bt */
    {
        ಗಾಗಿ( i = 0; i < drows; i++, a_ದತ್ತಾಂಶ_2 += a_step0, c_ದತ್ತಾಂಶ += c_step0, d_ದತ್ತಾಂಶ += d_ಹೆಜ್ಜೆ )
        {
            a_ದತ್ತಾಂಶ = a_ದತ್ತಾಂಶ_2;
            b_ದತ್ತಾಂಶ = b_ದತ್ತಾಂಶ_2;
            c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;

            ಆದರೆ( a_buf )
            {
                ಗಾಗಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_ದತ್ತಾಂಶ[a_step1*k];
                a_ದತ್ತಾಂಶ = a_buf;
            }

            ಗಾಗಿ( j = 0; j < d_ಗಾತ್ರ.ಅಗಲ; j++, b_ದತ್ತಾಂಶ += b_ಹೆಜ್ಜೆ,
                                               c_ದತ್ತಾಂಶ_2 += c_step1 )
            {
                WT s0(0);
                k = 0;
#ಆದರೆ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ಆದರೆ( ಗಾತ್ರ(WT) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
                {
                    ಆದರೆ( ಗಾತ್ರ(T) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
                        s0 = (WT)simd_Dot_ಗುಣಲಬ್ಧ((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)a_ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)b_ದತ್ತಾಂಶ, n, k);
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಗಾತ್ರ(T) == ಗಾತ್ರ(ದಶಮಾಂಶ) )
                        s0 = (WT)simd_Dot_ಗುಣಲಬ್ಧ_f32f64((ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)a_ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)b_ದತ್ತಾಂಶ, n, k);
                }
                ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
                {
                    WT s1(0), s2(0), s3(0);
                     #ಆದರೆ CV_ENABLE_UNROLLED
                    ಗಾಗಿ( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_ದತ್ತಾಂಶ[k])*WT(b_ದತ್ತಾಂಶ[k]);
                        s1 += WT(a_ದತ್ತಾಂಶ[k+1])*WT(b_ದತ್ತಾಂಶ[k+1]);
                        s2 += WT(a_ದತ್ತಾಂಶ[k+2])*WT(b_ದತ್ತಾಂಶ[k+2]);
                        s3 += WT(a_ದತ್ತಾಂಶ[k+3])*WT(b_ದತ್ತಾಂಶ[k+3]);
                    }
                    #ಆದರೆ_ಅಂತ್ಯ
                    s0 += s1 + s2 + s3;
                }
                ಗಾಗಿ( ; k < n; k++ )
                    s0 += WT(a_ದತ್ತಾಂಶ[k])*WT(b_ದತ್ತಾಂಶ[k]);
                s0 = s0*alpha;

                ಆದರೆ( !c_ದತ್ತಾಂಶ_2 )
                    d_ದತ್ತಾಂಶ[j] = T(s0);
                ಇಲ್ಲದಿದ್ದರೆ
                    d_ದತ್ತಾಂಶ[j] = T(s0 + WT(c_ದತ್ತಾಂಶ_2[0])*beta);
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d_ಗಾತ್ರ.ಅಗಲ*ಗಾತ್ರ(d_ದತ್ತಾಂಶ[0]) <= 1600 )
    {
        ಗಾಗಿ( i = 0; i < drows; i++, a_ದತ್ತಾಂಶ_2 += a_step0,
                                    c_ದತ್ತಾಂಶ += c_step0,
                                    d_ದತ್ತಾಂಶ += d_ಹೆಜ್ಜೆ )
        {
            a_ದತ್ತಾಂಶ = a_ದತ್ತಾಂಶ_2, c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;

            ಆದರೆ( a_buf )
            {
                ಗಾಗಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_ದತ್ತಾಂಶ[a_step1*k];
                a_ದತ್ತಾಂಶ = a_buf;
            }

            j = 0;
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ಆದರೆ( ಗಾತ್ರ(WT) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
            {
                ಆದರೆ( ಗಾತ್ರ(T) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
                    simdGEMM_kj((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)a_ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)b_ದತ್ತಾಂಶ_2, b_ಹೆಜ್ಜೆ,
                                        (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)c_ದತ್ತಾಂಶ, c_step1,
                                (ದ್ವಿಗುಣ*)d_ದತ್ತಾಂಶ, n, m, alpha, beta, j);
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಗಾತ್ರ(T) == ಗಾತ್ರ(ದಶಮಾಂಶ) )
                    simdGEMM_kj_f32f64((ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)a_ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)b_ದತ್ತಾಂಶ_2, b_ಹೆಜ್ಜೆ,
                                        (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)c_ದತ್ತಾಂಶ, c_step1,
                                (ದಶಮಾಂಶ*)d_ದತ್ತಾಂಶ, n, m, alpha, beta, j);
            }
#ಆದರೆ_ಅಂತ್ಯ
            c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            ಗಾಗಿ( ; j <= m - 4; j += 4, c_ದತ್ತಾಂಶ_2 += 4*c_step1 )
            {
                ಸ್ಥಿರಾಂಕ T* b = b_ದತ್ತಾಂಶ_2 + j;
                WT s0(0), s1(0), s2(0), s3(0);

                ಗಾಗಿ( k = 0; k < n; k++, b += b_ಹೆಜ್ಜೆ )
                {
                    WT a(a_ದತ್ತಾಂಶ[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                ಆದರೆ( !c_ದತ್ತಾಂಶ_2 )
                {
                    d_ದತ್ತಾಂಶ[j] = T(s0*alpha);
                    d_ದತ್ತಾಂಶ[j+1] = T(s1*alpha);
                    d_ದತ್ತಾಂಶ[j+2] = T(s2*alpha);
                    d_ದತ್ತಾಂಶ[j+3] = T(s3*alpha);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_ದತ್ತಾಂಶ[j] = T(s0 + WT(c_ದತ್ತಾಂಶ_2[0])*beta);
                    d_ದತ್ತಾಂಶ[j+1] = T(s1 + WT(c_ದತ್ತಾಂಶ_2[c_step1])*beta);
                    d_ದತ್ತಾಂಶ[j+2] = T(s2 + WT(c_ದತ್ತಾಂಶ_2[c_step1*2])*beta);
                    d_ದತ್ತಾಂಶ[j+3] = T(s3 + WT(c_ದತ್ತಾಂಶ_2[c_step1*3])*beta);
                }
            }

            ಗಾಗಿ( ; j < m; j++, c_ದತ್ತಾಂಶ_2 += c_step1 )
            {
                ಸ್ಥಿರಾಂಕ T* b = b_ದತ್ತಾಂಶ_2 + j;
                WT s0(0);

                ಗಾಗಿ( k = 0; k < n; k++, b += b_ಹೆಜ್ಜೆ )
                    s0 += WT(a_ದತ್ತಾಂಶ[k]) * WT(b[0]);

                s0 = s0*alpha;
                ಆದರೆ( !c_ದತ್ತಾಂಶ_2 )
                    d_ದತ್ತಾಂಶ[j] = T(s0);
                ಇಲ್ಲದಿದ್ದರೆ
                    d_ದತ್ತಾಂಶ[j] = T(s0 + WT(c_ದತ್ತಾಂಶ_2[0])*beta);
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        cv::Auto_ಬಫರ್<WT> _d_buf(m);
        WT* d_buf = _d_buf.ದತ್ತಾಂಶ();

        ಗಾಗಿ( i = 0; i < drows; i++, a_ದತ್ತಾಂಶ_2 += a_step0, c_ದತ್ತಾಂಶ += c_step0, d_ದತ್ತಾಂಶ += d_ಹೆಜ್ಜೆ )
        {
            a_ದತ್ತಾಂಶ = a_ದತ್ತಾಂಶ_2;
            b_ದತ್ತಾಂಶ = b_ದತ್ತಾಂಶ_2;
            c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;

            ಆದರೆ( a_buf )
            {
                ಗಾಗಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_ದತ್ತಾಂಶ_2[a_step1*k];
                a_ದತ್ತಾಂಶ = a_buf;
            }

            ಗಾಗಿ( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            ಗಾಗಿ( k = 0; k < n; k++, b_ದತ್ತಾಂಶ += b_ಹೆಜ್ಜೆ )
            {
                WT al(a_ದತ್ತಾಂಶ[k]);
                j=0;
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                ಆದರೆ( ಗಾತ್ರ(WT) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
                {
                    ಆದರೆ( ಗಾತ್ರ(T) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
                    {
                        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)&al));
                        ಗಾಗಿ( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_ತುಂಬು((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)(d_buf + j));
                            v_float64 vb = vx_ತುಂಬು((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)(b_ದತ್ತಾಂಶ + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((ದ್ವಿಗುಣ*)(d_buf + j), vd);
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes_f32 = VTraits<v_float32>::vlanes();
                        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)&al));
                        ಗಾಗಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_ತುಂಬು((ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)(b_ದತ್ತಾಂಶ + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_ಮೇಲಿನ(vb);
                            v_float64 vd_lo = vx_ತುಂಬು((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)(d_buf + j));
                            v_float64 vd_hi = vx_ತುಂಬು((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((ದ್ವಿಗುಣ*)(d_buf + j), vd_lo);
                            v_store((ದ್ವಿಗುಣ*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#ಆದರೆ_ಅಂತ್ಯ
                ಗಾಗಿ( ; j < m; j++ )
                    d_buf[j] += WT(b_ದತ್ತಾಂಶ[j])*al;
            }

            ಆದರೆ( !c_ದತ್ತಾಂಶ_2 )
                ಗಾಗಿ( j = 0; j < m; j++ )
                    d_ದತ್ತಾಂಶ[j] = T(d_buf[j]*alpha);
            ಇಲ್ಲದಿದ್ದರೆ
                ಗಾಗಿ( j = 0; j < m; j++, c_ದತ್ತಾಂಶ_2 += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_ದತ್ತಾಂಶ[j] = T(t + WT(c_ದತ್ತಾಂಶ_2[0])*beta);
                }
        }
    }
}
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು WT>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_Double_ದತ್ತಾಂಶ_Zero(WT* d_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ vlanes)
{
    v_float64 vz = vx_setzero_f64();
    ಪೂರ್ಣಾಂಕ j = 0;
    ಗಾಗಿ( ; j <= m - vlanes; j += vlanes )
        v_store((ದ್ವಿಗುಣ*)(d_ದತ್ತಾಂಶ + j), vz);
    ಗಾಗಿ( ; j < m; j++ )
        d_ದತ್ತಾಂಶ[j] = WT(0);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ST>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ simd_Mul_ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ ST* b, ದ್ವಿಗುಣ* d, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ a_val);

ಮಾದರಿ<>
ಸಾಲೊಳಗೆ ಶೂನ್ಯ simd_Mul_ಕೂಡಿಸು<ದಶಮಾಂಶ>(
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b, ದ್ವಿಗುಣ* d, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ a_val)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes_f32 = VTraits<v_float32>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    ಪೂರ್ಣಾಂಕ j = 0;
    ಗಾಗಿ( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_ತುಂಬು(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_ಮೇಲಿನ(vb);
        v_float64 vd_lo = vx_ತುಂಬು(d + j);
        v_float64 vd_hi = vx_ತುಂಬು(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    ಗಾಗಿ( ; j < m; j++ )
        d[j] += a_val * (ದ್ವಿಗುಣ)b[j];
}

ಮಾದರಿ<>
ಸಾಲೊಳಗೆ ಶೂನ್ಯ simd_Mul_ಕೂಡಿಸು<ದ್ವಿಗುಣ>(
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* b, ದ್ವಿಗುಣ* d, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ a_val)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_float64>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    ಪೂರ್ಣಾಂಕ j = 0;
    ಗಾಗಿ( ; j <= m - stride2; j += stride2 )
    {
        v_float64 vd0 = vx_ತುಂಬು(d + j);
        v_float64 vb0 = vx_ತುಂಬು(b + j);
        v_float64 vd1 = vx_ತುಂಬು(d + j + vlanes);
        v_float64 vb1 = vx_ತುಂಬು(b + j + vlanes);
        vd0 = v_muladd(va, vb0, vd0);
        vd1 = v_muladd(va, vb1, vd1);
        v_store(d + j, vd0);
        v_store(d + j + vlanes, vd1);
    }
    ಗಾಗಿ( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_ತುಂಬು(d + j);
        v_float64 vb = vx_ತುಂಬು(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    ಗಾಗಿ( ; j < m; j++ )
        d[j] += a_val * b[j];
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು ST>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ simd_ಖಂಡ_Mul_kj(
    ಸ್ಥಿರಾಂಕ ST* a_ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ST* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
    ದ್ವಿಗುಣ* d_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ತಾರ್ಕಿಕ do_acc)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_float64>::vlanes();
    ಆದರೆ( !do_acc )
        ಗಣ_Double_ದತ್ತಾಂಶ_Zero(d_ದತ್ತಾಂಶ, m, vlanes);
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < n; k++, b_ದತ್ತಾಂಶ += b_ಹೆಜ್ಜೆ )
        simd_Mul_ಕೂಡಿಸು(b_ದತ್ತಾಂಶ, d_ದತ್ತಾಂಶ, m, (ದ್ವಿಗುಣ)a_ದತ್ತಾಂಶ[k]);
}
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT> ಸ್ಥಿರ ಶೂನ್ಯ
GEMMBlockMul( ಸ್ಥಿರಾಂಕ T* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
              ಸ್ಥಿರಾಂಕ T* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
              WT* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
              ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ flags )
{
    ಪೂರ್ಣಾಂಕ i, j, k, n = a_ಗಾತ್ರ.ಅಗಲ, m = d_ಗಾತ್ರ.ಅಗಲ;
    ಸ್ಥಿರಾಂಕ T *a_ದತ್ತಾಂಶ_2 = a_ದತ್ತಾಂಶ, *b_ದತ್ತಾಂಶ_2 = b_ದತ್ತಾಂಶ;
    cv::Auto_ಬಫರ್<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_ಹೆಜ್ಜೆ;
    ಪೂರ್ಣಾಂಕ do_acc = flags & 16;

    a_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(a_ದತ್ತಾಂಶ[0]);
    b_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(b_ದತ್ತಾಂಶ[0]);
    d_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(d_ದತ್ತಾಂಶ[0]);

    a_step0 = a_ಹೆಜ್ಜೆ;
    a_step1 = 1;

    ಆದರೆ( flags & GEMM_1_T )
    {
        CV_ಅದಲುಬದಲು( a_step0, a_step1, t_ಹೆಜ್ಜೆ );
        n = a_ಗಾತ್ರ.ಎತ್ತರ;
        _a_buf.allocate(n);
        a_buf = _a_buf.ದತ್ತಾಂಶ();
    }

    ಆದರೆ( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i++, a_ದತ್ತಾಂಶ_2 += a_step0, d_ದತ್ತಾಂಶ += d_ಹೆಜ್ಜೆ )
        {
            a_ದತ್ತಾಂಶ = a_ದತ್ತಾಂಶ_2; b_ದತ್ತಾಂಶ = b_ದತ್ತಾಂಶ_2;

            ಆದರೆ( a_buf )
            {
                ಗಾಗಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_ದತ್ತಾಂಶ[a_step1*k];
                a_ದತ್ತಾಂಶ = a_buf;
            }

            ಗಾಗಿ( j = 0; j < d_ಗಾತ್ರ.ಅಗಲ; j++, b_ದತ್ತಾಂಶ += b_ಹೆಜ್ಜೆ )
            {
                WT s0 = do_acc ? d_ದತ್ತಾಂಶ[j] : WT(0);
                k = 0;
#ಆದರೆ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ಆದರೆ( ಗಾತ್ರ(WT) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
                {
                    ಆದರೆ( ಗಾತ್ರ(T) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
                        s0 += (WT)simd_Dot_ಗುಣಲಬ್ಧ((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)a_ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)b_ದತ್ತಾಂಶ, n, k);
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಗಾತ್ರ(T) == ಗಾತ್ರ(ದಶಮಾಂಶ) )
                        s0 += (WT)simd_Dot_ಗುಣಲಬ್ಧ_f32f64((ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)a_ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)b_ದತ್ತಾಂಶ, n, k);
                }
                ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
                {
                    WT s1(0);
                    ಗಾಗಿ( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_ದತ್ತಾಂಶ[k])*WT(b_ದತ್ತಾಂಶ[k]);
                        s1 += WT(a_ದತ್ತಾಂಶ[k+1])*WT(b_ದತ್ತಾಂಶ[k+1]);
                    }
                    s0 += s1;
                }
                ಗಾಗಿ( ; k < n; k++ )
                    s0 += WT(a_ದತ್ತಾಂಶ[k])*WT(b_ದತ್ತಾಂಶ[k]);

                d_ದತ್ತಾಂಶ[j] = s0;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i++, a_ದತ್ತಾಂಶ_2 += a_step0, d_ದತ್ತಾಂಶ += d_ಹೆಜ್ಜೆ )
        {
            a_ದತ್ತಾಂಶ = a_ದತ್ತಾಂಶ_2, b_ದತ್ತಾಂಶ = b_ದತ್ತಾಂಶ_2;

            ಆದರೆ( a_buf )
            {
                ಗಾಗಿ( k = 0; k < n; k++ )
                    a_buf[k] = a_ದತ್ತಾಂಶ[a_step1*k];
                a_ದತ್ತಾಂಶ = a_buf;
            }
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ಆದರೆ( ಗಾತ್ರ(WT) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
            {
                ಆದರೆ( ಗಾತ್ರ(T) == ಗಾತ್ರ(ದ್ವಿಗುಣ) )
                    simd_ಖಂಡ_Mul_kj(
                        (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)a_ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ,
                        (ದ್ವಿಗುಣ*)d_ದತ್ತಾಂಶ, n, m, do_acc != 0);
                ಇಲ್ಲದಿದ್ದರೆ
                    simd_ಖಂಡ_Mul_kj(
                        (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)a_ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ,
                        (ದ್ವಿಗುಣ*)d_ದತ್ತಾಂಶ, n, m, do_acc != 0);
            }
            ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
            {
                ಗಾಗಿ( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ಸ್ಥಿರಾಂಕ T* b = b_ದತ್ತಾಂಶ + j;

                    ಆದರೆ( do_acc )
                    {
                        s0 = d_ದತ್ತಾಂಶ[j]; s1 = d_ದತ್ತಾಂಶ[j+1];
                        s2 = d_ದತ್ತಾಂಶ[j+2]; s3 = d_ದತ್ತಾಂಶ[j+3];
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                        s0 = s1 = s2 = s3 = WT(0);

                    ಗಾಗಿ( k = 0; k < n; k++, b += b_ಹೆಜ್ಜೆ )
                    {
                        WT a(a_ದತ್ತಾಂಶ[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_ದತ್ತಾಂಶ[j] = s0; d_ದತ್ತಾಂಶ[j+1] = s1;
                    d_ದತ್ತಾಂಶ[j+2] = s2; d_ದತ್ತಾಂಶ[j+3] = s3;
                }

                ಗಾಗಿ( ; j < m; j++ )
                {
                    ಸ್ಥಿರಾಂಕ T* b = b_ದತ್ತಾಂಶ + j;
                    WT s0 = do_acc ? d_ದತ್ತಾಂಶ[j] : WT(0);

                    ಗಾಗಿ( k = 0; k < n; k++, b += b_ಹೆಜ್ಜೆ )
                        s0 += WT(a_ದತ್ತಾಂಶ[k]) * WT(b[0]);

                    d_ದತ್ತಾಂಶ[j] = s0;
                }
            }
        }
    }
}


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT> ಸ್ಥಿರ ಶೂನ್ಯ
GEMMStore( ಸ್ಥಿರಾಂಕ T* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
           ಸ್ಥಿರಾಂಕ WT* d_buf, size_t d_buf_ಹೆಜ್ಜೆ,
           T* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
           ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    ಸ್ಥಿರಾಂಕ T* c_ದತ್ತಾಂಶ = c_ದತ್ತಾಂಶ_2;
    ಪೂರ್ಣಾಂಕ j;
    size_t c_step0, c_step1;

    c_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(c_ದತ್ತಾಂಶ_2[0]);
    d_buf_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(d_buf[0]);
    d_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(d_ದತ್ತಾಂಶ[0]);

    ಆದರೆ( !c_ದತ್ತಾಂಶ_2 )
        c_step0 = c_step1 = 0;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( !(flags & GEMM_3_T) )
        c_step0 = c_ಹೆಜ್ಜೆ, c_step1 = 1;
    ಇಲ್ಲದಿದ್ದರೆ
        c_step0 = 1, c_step1 = c_ಹೆಜ್ಜೆ;

    ಗಾಗಿ( ; d_ಗಾತ್ರ.ಎತ್ತರ--; c_ದತ್ತಾಂಶ += c_step0, d_buf += d_buf_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ += d_ಹೆಜ್ಜೆ )
    {
        ಆದರೆ( c_ದತ್ತಾಂಶ )
        {
            c_ದತ್ತಾಂಶ_2 = c_ದತ್ತಾಂಶ;
            j=0;
             #ಆದರೆ CV_ENABLE_UNROLLED
            ಗಾಗಿ(; j <= d_ಗಾತ್ರ.ಅಗಲ - 4; j += 4, c_ದತ್ತಾಂಶ_2 += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_ದತ್ತಾಂಶ_2[0]);
                t1 += beta*WT(c_ದತ್ತಾಂಶ_2[c_step1]);
                d_ದತ್ತಾಂಶ[j] = T(t0);
                d_ದತ್ತಾಂಶ[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_ದತ್ತಾಂಶ_2[c_step1*2]);
                t1 += beta*WT(c_ದತ್ತಾಂಶ_2[c_step1*3]);
                d_ದತ್ತಾಂಶ[j+2] = T(t0);
                d_ದತ್ತಾಂಶ[j+3] = T(t1);
            }
            #ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ( ; j < d_ಗಾತ್ರ.ಅಗಲ; j++, c_ದತ್ತಾಂಶ_2 += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_ದತ್ತಾಂಶ[j] = T(t0 + WT(c_ದತ್ತಾಂಶ_2[0])*beta);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            j = 0;
             #ಆದರೆ CV_ENABLE_UNROLLED
            ಗಾಗಿ( ; j <= d_ಗಾತ್ರ.ಅಗಲ - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_ದತ್ತಾಂಶ[j] = T(t0);
                d_ದತ್ತಾಂಶ[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_ದತ್ತಾಂಶ[j+2] = T(t0);
                d_ದತ್ತಾಂಶ[j+3] = T(t1);
            }
            #ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ( ; j < d_ಗಾತ್ರ.ಅಗಲ; j++ )
                d_ದತ್ತಾಂಶ[j] = T(alpha*d_buf[j]);
        }
    }
}


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*GEMMSingleMulFunc)( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, size_t step1,
                   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src3, size_t step3,
                   ಶೂನ್ಯ* dst, size_t dststep, ಗಾತ್ರ_2 srcsize, ಗಾತ್ರ_2 dstsize,
                   ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags );

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*GEMMBlockMulFunc)( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, size_t step1,
                   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2, ಶೂನ್ಯ* dst, size_t dststep,
                   ಗಾತ್ರ_2 srcsize, ಗಾತ್ರ_2 dstsize, ಪೂರ್ಣಾಂಕ flags );

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*GEMMStoreFunc)( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, size_t step1,
                   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2, ಶೂನ್ಯ* dst, size_t dststep,
                   ಗಾತ್ರ_2 dstsize, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags );

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ T*, size_t, ಸ್ಥಿರಾಂಕ T*, size_t, ಸ್ಥಿರಾಂಕ T*, size_t, T*, size_t, ಗಾತ್ರ_2, ಗಾತ್ರ_2, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ gemmSingleMulWrap( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, size_t step1,
                   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src3, size_t step3,
                   ಶೂನ್ಯ* dst, size_t dststep, ಗಾತ್ರ_2 srcsize, ಗಾತ್ರ_2 dstsize,
                   ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    fn((ಸ್ಥಿರಾಂಕ T*)src1, step1, (ಸ್ಥಿರಾಂಕ T*)src2, step2, (ಸ್ಥಿರಾಂಕ T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ T*, size_t, ಸ್ಥಿರಾಂಕ T*, size_t, WT*, size_t, ಗಾತ್ರ_2, ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ gemm_ಖಂಡ_Mul_Wrap( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, size_t step1,
                   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2, ಶೂನ್ಯ* dst, size_t dststep,
                   ಗಾತ್ರ_2 srcsize, ಗಾತ್ರ_2 dstsize, ಪೂರ್ಣಾಂಕ flags )
{
    fn((ಸ್ಥಿರಾಂಕ T*)src1, step1, (ಸ್ಥಿರಾಂಕ T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ T*, size_t, ಸ್ಥಿರಾಂಕ WT*, size_t, T*, size_t, ಗಾತ್ರ_2, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ gemmStoreWrap( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, size_t step1,
                   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2, ಶೂನ್ಯ* dst, size_t dststep,
                   ಗಾತ್ರ_2 dstsize, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    fn((ಸ್ಥಿರಾಂಕ T*)src1, step1, (ಸ್ಥಿರಾಂಕ WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ GEMMSingleMul_32f( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
              ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
              ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
              ದಶಮಾಂಶ* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
              ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
              ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMSingleMul<ದಶಮಾಂಶ,ದ್ವಿಗುಣ>(a_ದತ್ತಾಂಶ, a_ಹೆಜ್ಜೆ, b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ, c_ದತ್ತಾಂಶ_2,
                                c_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ,
                                alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ GEMMSingleMul_64f( ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
                              ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
                              ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
                              ದ್ವಿಗುಣ* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
                              ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
                              ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMSingleMul<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ>(a_ದತ್ತಾಂಶ, a_ಹೆಜ್ಜೆ, b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ, c_ದತ್ತಾಂಶ_2,
                                c_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ,
                                alpha, beta, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMSingleMul_32fc( ಸ್ಥಿರಾಂಕ Complexf* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
                              ಸ್ಥಿರಾಂಕ Complexf* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
                              ಸ್ಥಿರಾಂಕ Complexf* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
                              Complexf* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
                              ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
                              ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_ದತ್ತಾಂಶ, a_ಹೆಜ್ಜೆ, b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ, c_ದತ್ತಾಂಶ_2,
                                c_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ,
                                alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ GEMMSingleMul_64fc( ಸ್ಥಿರಾಂಕ Complexd* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
                              ಸ್ಥಿರಾಂಕ Complexd* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
                              ಸ್ಥಿರಾಂಕ Complexd* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
                              Complexd* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
                              ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
                              ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_ದತ್ತಾಂಶ, a_ಹೆಜ್ಜೆ, b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ, c_ದತ್ತಾಂಶ_2,
                                 c_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ,
                                 alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ GEMMBlockMul_32f( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
             ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
             ದ್ವಿಗುಣ* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
             ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMBlockMul(a_ದತ್ತಾಂಶ, a_ಹೆಜ್ಜೆ, b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMBlockMul_64f( ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
                             ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
                             ದ್ವಿಗುಣ* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
                             ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMBlockMul(a_ದತ್ತಾಂಶ, a_ಹೆಜ್ಜೆ, b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMBlockMul_32fc( ಸ್ಥಿರಾಂಕ Complexf* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
                             ಸ್ಥಿರಾಂಕ Complexf* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
                             Complexd* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
                             ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_ದತ್ತಾಂಶ, a_ಹೆಜ್ಜೆ, b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMBlockMul_64fc( ಸ್ಥಿರಾಂಕ Complexd* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ,
                             ಸ್ಥಿರಾಂಕ Complexd* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ,
                             Complexd* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ,
                             ಗಾತ್ರ_2 a_ಗಾತ್ರ, ಗಾತ್ರ_2 d_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_ದತ್ತಾಂಶ, a_ಹೆಜ್ಜೆ, b_ದತ್ತಾಂಶ, b_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMStore_32f( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
          ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* d_buf, size_t d_buf_ಹೆಜ್ಜೆ,
          ದಶಮಾಂಶ* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
          ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMStore(c_ದತ್ತಾಂಶ_2, c_ಹೆಜ್ಜೆ, d_buf, d_buf_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, d_ಗಾತ್ರ, alpha, beta, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMStore_64f( ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
                      ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* d_buf, size_t d_buf_ಹೆಜ್ಜೆ,
                      ದ್ವಿಗುಣ* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
                      ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMStore(c_ದತ್ತಾಂಶ_2, c_ಹೆಜ್ಜೆ, d_buf, d_buf_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, d_ಗಾತ್ರ, alpha, beta, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMStore_32fc( ಸ್ಥಿರಾಂಕ Complexf* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
                          ಸ್ಥಿರಾಂಕ Complexd* d_buf, size_t d_buf_ಹೆಜ್ಜೆ,
                          Complexf* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
                          ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMStore(c_ದತ್ತಾಂಶ_2, c_ಹೆಜ್ಜೆ, d_buf, d_buf_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, d_ಗಾತ್ರ, alpha, beta, flags);
}


ಸ್ಥಿರ ಶೂನ್ಯ GEMMStore_64fc( ಸ್ಥಿರಾಂಕ Complexd* c_ದತ್ತಾಂಶ_2, size_t c_ಹೆಜ್ಜೆ,
                          ಸ್ಥಿರಾಂಕ Complexd* d_buf, size_t d_buf_ಹೆಜ್ಜೆ,
                          Complexd* d_ದತ್ತಾಂಶ, size_t d_ಹೆಜ್ಜೆ, ಗಾತ್ರ_2 d_ಗಾತ್ರ,
                          ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಪೂರ್ಣಾಂಕ flags )
{
    GEMMStore(c_ದತ್ತಾಂಶ_2, c_ಹೆಜ್ಜೆ, d_buf, d_buf_ಹೆಜ್ಜೆ, d_ದತ್ತಾಂಶ, d_ಹೆಜ್ಜೆ, d_ಗಾತ್ರ, alpha, beta, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ gemmImpl( Mat A, Mat B, ದ್ವಿಗುಣ alpha,
           Mat C, ದ್ವಿಗುಣ beta, Mat D, ಪೂರ್ಣಾಂಕ flags )
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಖಂಡ_lin_ಗಾತ್ರ = 128;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ_3 = ಖಂಡ_lin_ಗಾತ್ರ * ಖಂಡ_lin_ಗಾತ್ರ;

    ಸ್ಥಿರ ದ್ವಿಗುಣ zero[] = {0,0,0,0};
    ಸ್ಥಿರ ದಶಮಾಂಶ zerof[] = {0,0,0,0};

    ಗಾತ್ರ_2 a_ಗಾತ್ರ = A.size(), d_ಗಾತ್ರ;
    ಪೂರ್ಣಾಂಕ i, len = 0, type = A.type();

    ಆಯ್ಕೆ( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ಸ್ಥಿತಿ 0:
        d_ಗಾತ್ರ = ಗಾತ್ರ_2( B.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, a_ಗಾತ್ರ.ಎತ್ತರ );
        len = B.ಅಡ್ಡಸಾಲುಗಳು;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 1:
        d_ಗಾತ್ರ = ಗಾತ್ರ_2( B.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, a_ಗಾತ್ರ.ಅಗಲ );
        len = B.ಅಡ್ಡಸಾಲುಗಳು;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 2:
        d_ಗಾತ್ರ = ಗಾತ್ರ_2( B.ಅಡ್ಡಸಾಲುಗಳು, a_ಗಾತ್ರ.ಎತ್ತರ );
        len = B.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 3:
        d_ಗಾತ್ರ = ಗಾತ್ರ_2( B.ಅಡ್ಡಸಾಲುಗಳು, a_ಗಾತ್ರ.ಅಗಲ );
        len = B.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ( flags == 0 && 2 <= len && len <= 4 && (len == d_ಗಾತ್ರ.ಅಗಲ || len == d_ಗಾತ್ರ.ಎತ್ತರ) )
    {
        ಆದರೆ( type == CV_32F )
        {
            ದಶಮಾಂಶ* d = D.ptr<ದಶಮಾಂಶ>();
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *a = A.ptr<ದಶಮಾಂಶ>(),
                        *b = B.ptr<ದಶಮಾಂಶ>(),
                        *c = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)C.ದತ್ತಾಂಶ;
            size_t d_ಹೆಜ್ಜೆ = D.ಹೆಜ್ಜೆ/ಗಾತ್ರ(d[0]),
                a_ಹೆಜ್ಜೆ = A.ಹೆಜ್ಜೆ/ಗಾತ್ರ(a[0]),
                b_ಹೆಜ್ಜೆ = B.ಹೆಜ್ಜೆ/ಗಾತ್ರ(b[0]),
                c_ಹೆಜ್ಜೆ = C.ದತ್ತಾಂಶ ? C.ಹೆಜ್ಜೆ/ಗಾತ್ರ(c[0]) : 0;

            ಆದರೆ( !c )
                c = zerof;

            ಆಯ್ಕೆ( len )
            {
            ಸ್ಥಿತಿ 2:
                ಆದರೆ( len == d_ಗಾತ್ರ.ಅಗಲ && b != d )
                {
                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i++, d += d_ಹೆಜ್ಜೆ, a += a_ಹೆಜ್ಜೆ, c += c_ಹೆಜ್ಜೆ )
                    {
                        ದಶಮಾಂಶ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ];
                        ದಶಮಾಂಶ t1 = a[0]*b[1] + a[1]*b[b_ಹೆಜ್ಜೆ+1];
                        d[0] = (ದಶಮಾಂಶ)(t0*alpha + c[0]*beta);
                        d[1] = (ದಶಮಾಂಶ)(t1*alpha + c[1]*beta);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( a != d )
                {
                    ಪೂರ್ಣಾಂಕ c_step0 = 1;
                    ಆದರೆ( c == zerof )
                    {
                        c_step0 = 0;
                        c_ಹೆಜ್ಜೆ = 1;
                    }

                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಅಗಲ; i++, d++, b++, c += c_step0 )
                    {
                        ದಶಮಾಂಶ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ];
                        ದಶಮಾಂಶ t1 = a[a_ಹೆಜ್ಜೆ]*b[0] + a[a_ಹೆಜ್ಜೆ+1]*b[b_ಹೆಜ್ಜೆ];
                        d[0] = (ದಶಮಾಂಶ)(t0*alpha + c[0]*beta);
                        d[d_ಹೆಜ್ಜೆ] = (ದಶಮಾಂಶ)(t1*alpha + c[c_ಹೆಜ್ಜೆ]*beta);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ನಿಲ್ಲಿಸು;
                ಹಿಂತಿರುಗಿ;
            ಸ್ಥಿತಿ 3:
                ಆದರೆ( len == d_ಗಾತ್ರ.ಅಗಲ && b != d )
                {
                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i++, d += d_ಹೆಜ್ಜೆ, a += a_ಹೆಜ್ಜೆ, c += c_ಹೆಜ್ಜೆ )
                    {
                        ದಶಮಾಂಶ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ] + a[2]*b[b_ಹೆಜ್ಜೆ*2];
                        ದಶಮಾಂಶ t1 = a[0]*b[1] + a[1]*b[b_ಹೆಜ್ಜೆ+1] + a[2]*b[b_ಹೆಜ್ಜೆ*2+1];
                        ದಶಮಾಂಶ t2 = a[0]*b[2] + a[1]*b[b_ಹೆಜ್ಜೆ+2] + a[2]*b[b_ಹೆಜ್ಜೆ*2+2];
                        d[0] = (ದಶಮಾಂಶ)(t0*alpha + c[0]*beta);
                        d[1] = (ದಶಮಾಂಶ)(t1*alpha + c[1]*beta);
                        d[2] = (ದಶಮಾಂಶ)(t2*alpha + c[2]*beta);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( a != d )
                {
                    ಪೂರ್ಣಾಂಕ c_step0 = 1;
                    ಆದರೆ( c == zerof )
                    {
                        c_step0 = 0;
                        c_ಹೆಜ್ಜೆ = 1;
                    }

                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಅಗಲ; i++, d++, b++, c += c_step0 )
                    {
                        ದಶಮಾಂಶ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ] + a[2]*b[b_ಹೆಜ್ಜೆ*2];
                        ದಶಮಾಂಶ t1 = a[a_ಹೆಜ್ಜೆ]*b[0] + a[a_ಹೆಜ್ಜೆ+1]*b[b_ಹೆಜ್ಜೆ] + a[a_ಹೆಜ್ಜೆ+2]*b[b_ಹೆಜ್ಜೆ*2];
                        ದಶಮಾಂಶ t2 = a[a_ಹೆಜ್ಜೆ*2]*b[0] + a[a_ಹೆಜ್ಜೆ*2+1]*b[b_ಹೆಜ್ಜೆ] + a[a_ಹೆಜ್ಜೆ*2+2]*b[b_ಹೆಜ್ಜೆ*2];

                        d[0] = (ದಶಮಾಂಶ)(t0*alpha + c[0]*beta);
                        d[d_ಹೆಜ್ಜೆ] = (ದಶಮಾಂಶ)(t1*alpha + c[c_ಹೆಜ್ಜೆ]*beta);
                        d[d_ಹೆಜ್ಜೆ*2] = (ದಶಮಾಂಶ)(t2*alpha + c[c_ಹೆಜ್ಜೆ*2]*beta);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ನಿಲ್ಲಿಸು;
                ಹಿಂತಿರುಗಿ;
            ಸ್ಥಿತಿ 4:
                ಆದರೆ( len == d_ಗಾತ್ರ.ಅಗಲ && b != d )
                {
                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i++, d += d_ಹೆಜ್ಜೆ, a += a_ಹೆಜ್ಜೆ, c += c_ಹೆಜ್ಜೆ )
                    {
                        ದಶಮಾಂಶ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ] + a[2]*b[b_ಹೆಜ್ಜೆ*2] + a[3]*b[b_ಹೆಜ್ಜೆ*3];
                        ದಶಮಾಂಶ t1 = a[0]*b[1] + a[1]*b[b_ಹೆಜ್ಜೆ+1] + a[2]*b[b_ಹೆಜ್ಜೆ*2+1] + a[3]*b[b_ಹೆಜ್ಜೆ*3+1];
                        ದಶಮಾಂಶ t2 = a[0]*b[2] + a[1]*b[b_ಹೆಜ್ಜೆ+2] + a[2]*b[b_ಹೆಜ್ಜೆ*2+2] + a[3]*b[b_ಹೆಜ್ಜೆ*3+2];
                        ದಶಮಾಂಶ t3 = a[0]*b[3] + a[1]*b[b_ಹೆಜ್ಜೆ+3] + a[2]*b[b_ಹೆಜ್ಜೆ*2+3] + a[3]*b[b_ಹೆಜ್ಜೆ*3+3];
                        d[0] = (ದಶಮಾಂಶ)(t0*alpha + c[0]*beta);
                        d[1] = (ದಶಮಾಂಶ)(t1*alpha + c[1]*beta);
                        d[2] = (ದಶಮಾಂಶ)(t2*alpha + c[2]*beta);
                        d[3] = (ದಶಮಾಂಶ)(t3*alpha + c[3]*beta);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( len <= 16 && a != d )
                {
                    ಪೂರ್ಣಾಂಕ c_step0 = 1;
                    ಆದರೆ( c == zerof )
                    {
                        c_step0 = 0;
                        c_ಹೆಜ್ಜೆ = 1;
                    }

                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಅಗಲ; i++, d++, b++, c += c_step0 )
                    {
                        ದಶಮಾಂಶ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ] + a[2]*b[b_ಹೆಜ್ಜೆ*2] + a[3]*b[b_ಹೆಜ್ಜೆ*3];
                        ದಶಮಾಂಶ t1 = a[a_ಹೆಜ್ಜೆ]*b[0] + a[a_ಹೆಜ್ಜೆ+1]*b[b_ಹೆಜ್ಜೆ] +
                                   a[a_ಹೆಜ್ಜೆ+2]*b[b_ಹೆಜ್ಜೆ*2] + a[a_ಹೆಜ್ಜೆ+3]*b[b_ಹೆಜ್ಜೆ*3];
                        ದಶಮಾಂಶ t2 = a[a_ಹೆಜ್ಜೆ*2]*b[0] + a[a_ಹೆಜ್ಜೆ*2+1]*b[b_ಹೆಜ್ಜೆ] +
                                   a[a_ಹೆಜ್ಜೆ*2+2]*b[b_ಹೆಜ್ಜೆ*2] + a[a_ಹೆಜ್ಜೆ*2+3]*b[b_ಹೆಜ್ಜೆ*3];
                        ದಶಮಾಂಶ t3 = a[a_ಹೆಜ್ಜೆ*3]*b[0] + a[a_ಹೆಜ್ಜೆ*3+1]*b[b_ಹೆಜ್ಜೆ] +
                                   a[a_ಹೆಜ್ಜೆ*3+2]*b[b_ಹೆಜ್ಜೆ*2] + a[a_ಹೆಜ್ಜೆ*3+3]*b[b_ಹೆಜ್ಜೆ*3];
                        d[0] = (ದಶಮಾಂಶ)(t0*alpha + c[0]*beta);
                        d[d_ಹೆಜ್ಜೆ] = (ದಶಮಾಂಶ)(t1*alpha + c[c_ಹೆಜ್ಜೆ]*beta);
                        d[d_ಹೆಜ್ಜೆ*2] = (ದಶಮಾಂಶ)(t2*alpha + c[c_ಹೆಜ್ಜೆ*2]*beta);
                        d[d_ಹೆಜ್ಜೆ*3] = (ದಶಮಾಂಶ)(t3*alpha + c[c_ಹೆಜ್ಜೆ*3]*beta);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ನಿಲ್ಲಿಸು;
                ಹಿಂತಿರುಗಿ;
            }
        }

        ಆದರೆ( type == CV_64F )
        {
            ದ್ವಿಗುಣ* d = D.ptr<ದ್ವಿಗುಣ>();
            ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *a = A.ptr<ದ್ವಿಗುಣ>(),
                         *b = B.ptr<ದ್ವಿಗುಣ>(),
                         *c = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)C.ದತ್ತಾಂಶ;
            size_t d_ಹೆಜ್ಜೆ = D.ಹೆಜ್ಜೆ/ಗಾತ್ರ(d[0]),
                a_ಹೆಜ್ಜೆ = A.ಹೆಜ್ಜೆ/ಗಾತ್ರ(a[0]),
                b_ಹೆಜ್ಜೆ = B.ಹೆಜ್ಜೆ/ಗಾತ್ರ(b[0]),
                c_ಹೆಜ್ಜೆ = C.ದತ್ತಾಂಶ ? C.ಹೆಜ್ಜೆ/ಗಾತ್ರ(c[0]) : 0;
            ಆದರೆ( !c )
                c = zero;

            ಆಯ್ಕೆ( len )
            {
            ಸ್ಥಿತಿ 2:
                ಆದರೆ( len == d_ಗಾತ್ರ.ಅಗಲ && b != d )
                {
                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i++, d += d_ಹೆಜ್ಜೆ, a += a_ಹೆಜ್ಜೆ, c += c_ಹೆಜ್ಜೆ )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ];
                        ದ್ವಿಗುಣ t1 = a[0]*b[1] + a[1]*b[b_ಹೆಜ್ಜೆ+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( a != d )
                {
                    ಪೂರ್ಣಾಂಕ c_step0 = 1;
                    ಆದರೆ( c == zero )
                    {
                        c_step0 = 0;
                        c_ಹೆಜ್ಜೆ = 1;
                    }

                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಅಗಲ; i++, d++, b++, c += c_step0 )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ];
                        ದ್ವಿಗುಣ t1 = a[a_ಹೆಜ್ಜೆ]*b[0] + a[a_ಹೆಜ್ಜೆ+1]*b[b_ಹೆಜ್ಜೆ];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_ಹೆಜ್ಜೆ] = t1*alpha + c[c_ಹೆಜ್ಜೆ]*beta;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ನಿಲ್ಲಿಸು;
                ಹಿಂತಿರುಗಿ;
            ಸ್ಥಿತಿ 3:
                ಆದರೆ( len == d_ಗಾತ್ರ.ಅಗಲ && b != d )
                {
                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i++, d += d_ಹೆಜ್ಜೆ, a += a_ಹೆಜ್ಜೆ, c += c_ಹೆಜ್ಜೆ )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ] + a[2]*b[b_ಹೆಜ್ಜೆ*2];
                        ದ್ವಿಗುಣ t1 = a[0]*b[1] + a[1]*b[b_ಹೆಜ್ಜೆ+1] + a[2]*b[b_ಹೆಜ್ಜೆ*2+1];
                        ದ್ವಿಗುಣ t2 = a[0]*b[2] + a[1]*b[b_ಹೆಜ್ಜೆ+2] + a[2]*b[b_ಹೆಜ್ಜೆ*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( a != d )
                {
                    ಪೂರ್ಣಾಂಕ c_step0 = 1;
                    ಆದರೆ( c == zero )
                    {
                        c_step0 = 0;
                        c_ಹೆಜ್ಜೆ = 1;
                    }

                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಅಗಲ; i++, d++, b++, c += c_step0 )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ] + a[2]*b[b_ಹೆಜ್ಜೆ*2];
                        ದ್ವಿಗುಣ t1 = a[a_ಹೆಜ್ಜೆ]*b[0] + a[a_ಹೆಜ್ಜೆ+1]*b[b_ಹೆಜ್ಜೆ] + a[a_ಹೆಜ್ಜೆ+2]*b[b_ಹೆಜ್ಜೆ*2];
                        ದ್ವಿಗುಣ t2 = a[a_ಹೆಜ್ಜೆ*2]*b[0] + a[a_ಹೆಜ್ಜೆ*2+1]*b[b_ಹೆಜ್ಜೆ] + a[a_ಹೆಜ್ಜೆ*2+2]*b[b_ಹೆಜ್ಜೆ*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_ಹೆಜ್ಜೆ] = t1*alpha + c[c_ಹೆಜ್ಜೆ]*beta;
                        d[d_ಹೆಜ್ಜೆ*2] = t2*alpha + c[c_ಹೆಜ್ಜೆ*2]*beta;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ನಿಲ್ಲಿಸು;
                ಹಿಂತಿರುಗಿ;
            ಸ್ಥಿತಿ 4:
                ಆದರೆ( len == d_ಗಾತ್ರ.ಅಗಲ && b != d )
                {
                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i++, d += d_ಹೆಜ್ಜೆ, a += a_ಹೆಜ್ಜೆ, c += c_ಹೆಜ್ಜೆ )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ] + a[2]*b[b_ಹೆಜ್ಜೆ*2] + a[3]*b[b_ಹೆಜ್ಜೆ*3];
                        ದ್ವಿಗುಣ t1 = a[0]*b[1] + a[1]*b[b_ಹೆಜ್ಜೆ+1] + a[2]*b[b_ಹೆಜ್ಜೆ*2+1] + a[3]*b[b_ಹೆಜ್ಜೆ*3+1];
                        ದ್ವಿಗುಣ t2 = a[0]*b[2] + a[1]*b[b_ಹೆಜ್ಜೆ+2] + a[2]*b[b_ಹೆಜ್ಜೆ*2+2] + a[3]*b[b_ಹೆಜ್ಜೆ*3+2];
                        ದ್ವಿಗುಣ t3 = a[0]*b[3] + a[1]*b[b_ಹೆಜ್ಜೆ+3] + a[2]*b[b_ಹೆಜ್ಜೆ*2+3] + a[3]*b[b_ಹೆಜ್ಜೆ*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d_ಗಾತ್ರ.ಅಗಲ <= 16 && a != d )
                {
                    ಪೂರ್ಣಾಂಕ c_step0 = 1;
                    ಆದರೆ( c == zero )
                    {
                        c_step0 = 0;
                        c_ಹೆಜ್ಜೆ = 1;
                    }

                    ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಅಗಲ; i++, d++, b++, c += c_step0 )
                    {
                        ದ್ವಿಗುಣ t0 = a[0]*b[0] + a[1]*b[b_ಹೆಜ್ಜೆ] + a[2]*b[b_ಹೆಜ್ಜೆ*2] + a[3]*b[b_ಹೆಜ್ಜೆ*3];
                        ದ್ವಿಗುಣ t1 = a[a_ಹೆಜ್ಜೆ]*b[0] + a[a_ಹೆಜ್ಜೆ+1]*b[b_ಹೆಜ್ಜೆ] +
                                    a[a_ಹೆಜ್ಜೆ+2]*b[b_ಹೆಜ್ಜೆ*2] + a[a_ಹೆಜ್ಜೆ+3]*b[b_ಹೆಜ್ಜೆ*3];
                        ದ್ವಿಗುಣ t2 = a[a_ಹೆಜ್ಜೆ*2]*b[0] + a[a_ಹೆಜ್ಜೆ*2+1]*b[b_ಹೆಜ್ಜೆ] +
                                    a[a_ಹೆಜ್ಜೆ*2+2]*b[b_ಹೆಜ್ಜೆ*2] + a[a_ಹೆಜ್ಜೆ*2+3]*b[b_ಹೆಜ್ಜೆ*3];
                        ದ್ವಿಗುಣ t3 = a[a_ಹೆಜ್ಜೆ*3]*b[0] + a[a_ಹೆಜ್ಜೆ*3+1]*b[b_ಹೆಜ್ಜೆ] +
                                    a[a_ಹೆಜ್ಜೆ*3+2]*b[b_ಹೆಜ್ಜೆ*2] + a[a_ಹೆಜ್ಜೆ*3+3]*b[b_ಹೆಜ್ಜೆ*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_ಹೆಜ್ಜೆ] = t1*alpha + c[c_ಹೆಜ್ಜೆ]*beta;
                        d[d_ಹೆಜ್ಜೆ*2] = t2*alpha + c[c_ಹೆಜ್ಜೆ*2]*beta;
                        d[d_ಹೆಜ್ಜೆ*3] = t3*alpha + c[c_ಹೆಜ್ಜೆ*3]*beta;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ನಿಲ್ಲಿಸು;
                ಹಿಂತಿರುಗಿ;
            }
        }
    }

    {
    size_t b_ಹೆಜ್ಜೆ = B.ಹೆಜ್ಜೆ;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc ಖಂಡ_Mul_Func;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ಸ್ಥಿರಾಂಕ uchar* Cdata = C.ದತ್ತಾಂಶ;
    size_t Cstep = C.ದತ್ತಾಂಶ ? (size_t)C.ಹೆಜ್ಜೆ : 0;
    Auto_ಬಫರ್<uchar> buf;

    ಆದರೆ( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, GEMMSingleMul_32f>;
        ಖಂಡ_Mul_Func = gemm_ಖಂಡ_Mul_Wrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<ದಶಮಾಂಶ, ದ್ವಿಗುಣ, GEMMStore_32f>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, GEMMSingleMul_64f>;
        ಖಂಡ_Mul_Func = gemm_ಖಂಡ_Mul_Wrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, GEMMStore_64f>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        ಖಂಡ_Mul_Func = gemm_ಖಂಡ_Mul_Wrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        ಖಂಡ_Mul_Func = gemm_ಖಂಡ_Mul_Wrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    ಆದರೆ( (d_ಗಾತ್ರ.ಅಗಲ == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_ಹೆಜ್ಜೆ = d_ಗಾತ್ರ.ಅಗಲ == 1 ? 0 : CV_ELEM_ಗಾತ್ರ(type);
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
    else*/ ಆದರೆ( ((d_ಗಾತ್ರ.ಎತ್ತರ <= ಖಂಡ_lin_ಗಾತ್ರ/2 || d_ಗಾತ್ರ.ಅಗಲ <= ಖಂಡ_lin_ಗಾತ್ರ/2) &&
        len <= 10000) || len <= 10 ||
        (d_ಗಾತ್ರ.ಅಗಲ <= ಖಂಡ_lin_ಗಾತ್ರ &&
        d_ಗಾತ್ರ.ಎತ್ತರ <= ಖಂಡ_lin_ಗಾತ್ರ && len <= ಖಂಡ_lin_ಗಾತ್ರ) )
    {
        singleMulFunc( A.ptr(), A.ಹೆಜ್ಜೆ, B.ptr(), b_ಹೆಜ್ಜೆ, Cdata, Cstep,
                       matD->ptr(), matD->ಹೆಜ್ಜೆ, a_ಗಾತ್ರ, d_ಗಾತ್ರ, alpha, beta, flags );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ is_a_t = flags & GEMM_1_T;
        ಪೂರ್ಣಾಂಕ is_b_t = flags & GEMM_2_T;
        ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ_2 = CV_ELEM_ಗಾತ್ರ(type);
        ಪೂರ್ಣಾಂಕ dk0_1, dk0_2;
        size_t a_buf_ಗಾತ್ರ = 0, b_buf_ಗಾತ್ರ, d_buf_ಗಾತ್ರ;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        ಪೂರ್ಣಾಂಕ j, k, di = 0, dj = 0, dk = 0;
        ಪೂರ್ಣಾಂಕ dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        ಪೂರ್ಣಾಂಕ work_elem_ಗಾತ್ರ = elem_ಗಾತ್ರ_2 << (CV_MAT_ಆಳ(type) == CV_32F ? 1 : 0);

        ಆದರೆ( !is_a_t )
            a_step0 = A.ಹೆಜ್ಜೆ, a_step1 = elem_ಗಾತ್ರ_2;
        ಇಲ್ಲದಿದ್ದರೆ
            a_step0 = elem_ಗಾತ್ರ_2, a_step1 = A.ಹೆಜ್ಜೆ;

        ಆದರೆ( !is_b_t )
            b_step0 = b_ಹೆಜ್ಜೆ, b_step1 = elem_ಗಾತ್ರ_2;
        ಇಲ್ಲದಿದ್ದರೆ
            b_step0 = elem_ಗಾತ್ರ_2, b_step1 = b_ಹೆಜ್ಜೆ;

        ಆದರೆ( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( !(flags & GEMM_3_T) )
            c_step0 = C.ಹೆಜ್ಜೆ, c_step1 = elem_ಗಾತ್ರ_2;
        ಇಲ್ಲದಿದ್ದರೆ
            c_step0 = elem_ಗಾತ್ರ_2, c_step1 = C.ಹೆಜ್ಜೆ;

        dm0 = std::min( ಖಂಡ_lin_ಗಾತ್ರ, d_ಗಾತ್ರ.ಎತ್ತರ );
        dn0 = std::min( ಖಂಡ_lin_ಗಾತ್ರ, d_ಗಾತ್ರ.ಅಗಲ );
        dk0_1 = ಖಂಡ_ಗಾತ್ರ_3 / dm0;
        dk0_2 = ಖಂಡ_ಗಾತ್ರ_3 / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        ಆದರೆ( dk0*dm0 > ಖಂಡ_ಗಾತ್ರ_3 )
            dm0 = ಖಂಡ_ಗಾತ್ರ_3 / dk0;
        ಆದರೆ( dk0*dn0 > ಖಂಡ_ಗಾತ್ರ_3 )
            dn0 = ಖಂಡ_ಗಾತ್ರ_3 / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_ಗಾತ್ರ = (size_t)(dk0+dk0/8+1)*dk0_1*elem_ಗಾತ್ರ_2;
        d_buf_ಗಾತ್ರ = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_ಗಾತ್ರ;

        ಆದರೆ( is_a_t )
        {
            a_buf_ಗಾತ್ರ = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_ಗಾತ್ರ_2;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_ಗಾತ್ರ + b_buf_ಗಾತ್ರ + a_buf_ಗಾತ್ರ);
        d_buf = buf.ದತ್ತಾಂಶ();
        b_buf = d_buf + d_buf_ಗಾತ್ರ;

        ಆದರೆ( is_a_t )
            a_buf = b_buf + b_buf_ಗಾತ್ರ;

        ಗಾಗಿ( i = 0; i < d_ಗಾತ್ರ.ಎತ್ತರ; i += di )
        {
            di = dm0;
            ಆದರೆ( i + di >= d_ಗಾತ್ರ.ಎತ್ತರ || 8*(i + di) + di > 8*d_ಗಾತ್ರ.ಎತ್ತರ )
                di = d_ಗಾತ್ರ.ಎತ್ತರ - i;

            ಗಾಗಿ( j = 0; j < d_ಗಾತ್ರ.ಅಗಲ; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->ಹೆಜ್ಜೆ + j*elem_ಗಾತ್ರ_2;
                ಸ್ಥಿರಾಂಕ uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t d_ಹೆಜ್ಜೆ_2 = matD->ಹೆಜ್ಜೆ;
                dj = dn0;

                ಆದರೆ( j + dj >= d_ಗಾತ್ರ.ಅಗಲ || 8*(j + dj) + dj > 8*d_ಗಾತ್ರ.ಅಗಲ )
                    dj = d_ಗಾತ್ರ.ಅಗಲ - j;

                flags &= 15;
                ಆದರೆ( dk0 < len )
                {
                    _d = d_buf;
                    d_ಹೆಜ್ಜೆ_2 = dj*work_elem_ಗಾತ್ರ;
                }

                ಗಾಗಿ( k = 0; k < len; k += dk )
                {
                    ಸ್ಥಿರಾಂಕ uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t a_ಹೆಜ್ಜೆ_2 = A.ಹೆಜ್ಜೆ;
                    ಸ್ಥಿರಾಂಕ uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t b_ಹೆಜ್ಜೆ_2 = b_ಹೆಜ್ಜೆ;
                    ಗಾತ್ರ_2 a_bl_ಗಾತ್ರ;

                    dk = dk0;
                    ಆದರೆ( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    ಆದರೆ( !is_a_t )
                        a_bl_ಗಾತ್ರ.ಅಗಲ = dk, a_bl_ಗಾತ್ರ.ಎತ್ತರ = di;
                    ಇಲ್ಲದಿದ್ದರೆ
                        a_bl_ಗಾತ್ರ.ಅಗಲ = di, a_bl_ಗಾತ್ರ.ಎತ್ತರ = dk;

                    ಆದರೆ( a_buf && is_a_t )
                    {
                        a_ಹೆಜ್ಜೆ_2 = dk*elem_ಗಾತ್ರ_2;
                        GEMM_Transpose_ಖಂಡ( _a, A.ಹೆಜ್ಜೆ, a_buf, a_ಹೆಜ್ಜೆ_2, a_bl_ಗಾತ್ರ, elem_ಗಾತ್ರ_2 );
                        std::swap( a_bl_ಗಾತ್ರ.ಅಗಲ, a_bl_ಗಾತ್ರ.ಎತ್ತರ );
                        _a = a_buf;
                    }

                    ಆದರೆ( dj < d_ಗಾತ್ರ.ಅಗಲ )
                    {
                        ಗಾತ್ರ_2 b_ಗಾತ್ರ;
                        ಆದರೆ( !is_b_t )
                            b_ಗಾತ್ರ.ಅಗಲ = dj, b_ಗಾತ್ರ.ಎತ್ತರ = dk;
                        ಇಲ್ಲದಿದ್ದರೆ
                            b_ಗಾತ್ರ.ಅಗಲ = dk, b_ಗಾತ್ರ.ಎತ್ತರ = dj;

                        b_ಹೆಜ್ಜೆ_2 = b_ಗಾತ್ರ.ಅಗಲ*elem_ಗಾತ್ರ_2;
                        GEMM_Copy_ಖಂಡ( _b, b_ಹೆಜ್ಜೆ, b_buf, b_ಹೆಜ್ಜೆ_2, b_ಗಾತ್ರ, elem_ಗಾತ್ರ_2 );
                        _b = b_buf;
                    }

                    ಆದರೆ( dk0 < len )
                        ಖಂಡ_Mul_Func( _a, a_ಹೆಜ್ಜೆ_2, _b, b_ಹೆಜ್ಜೆ_2, _d, d_ಹೆಜ್ಜೆ_2,
                                      a_bl_ಗಾತ್ರ, ಗಾತ್ರ_2(dj,di), flags );
                    ಇಲ್ಲದಿದ್ದರೆ
                        singleMulFunc( _a, a_ಹೆಜ್ಜೆ_2, _b, b_ಹೆಜ್ಜೆ_2, _c, Cstep,
                                       _d, d_ಹೆಜ್ಜೆ_2, a_bl_ಗಾತ್ರ, ಗಾತ್ರ_2(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                ಆದರೆ( dk0 < len )
                    storeFunc( _c, Cstep, _d, d_ಹೆಜ್ಜೆ_2,
                               matD->ptr(i) + j*elem_ಗಾತ್ರ_2,
                               matD->ಹೆಜ್ಜೆ, ಗಾತ್ರ_2(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype>ಸಾಲೊಳಗೆ ಸ್ಥಿರ ಶೂನ್ಯ
callGemmImpl(ಸ್ಥಿರಾಂಕ fptype *src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ fptype *src2, size_t src2_ಹೆಜ್ಜೆ, fptype alpha,
          ಸ್ಥಿರಾಂಕ fptype *src3, size_t src3_ಹೆಜ್ಜೆ, fptype beta, fptype *dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    ಪೂರ್ಣಾಂಕ b_m, b_n, c_m, c_n, m_d;

    ಆದರೆ(flags & GEMM_2_T)
    {
        b_m = n_d;
        ಆದರೆ(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        b_n = n_d;
        ಆದರೆ(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    ಆದರೆ(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    ಆದರೆ(src1 != ಖಾಲಿ)
        A = Mat(m_a, n_a, type, (ಶೂನ್ಯ*)src1, src1_ಹೆಜ್ಜೆ);
    ಆದರೆ(src2 != ಖಾಲಿ)
        B = Mat(b_m, b_n, type, (ಶೂನ್ಯ*)src2, src2_ಹೆಜ್ಜೆ);
    ಆದರೆ(src3 != ಖಾಲಿ && beta != 0.0)
        C = Mat(c_m, c_n, type, (ಶೂನ್ಯ*)src3, src3_ಹೆಜ್ಜೆ);
    Mat D(m_d, n_d, type, (ಶೂನ್ಯ*)dst, dst_ಹೆಜ್ಜೆ);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

ಶೂನ್ಯ gemm32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
             ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
             ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags, CV_32F);
}

ಶೂನ್ಯ gemm64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
             ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
             ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags, CV_64F);
}

ಶೂನ್ಯ gemm32fc(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
              ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
              ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags, CV_32FC2);
}

ಶೂನ್ಯ gemm64fc(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
              ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
              ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags, CV_64FC2);
}

#ಆದರೆ_ಅಂತ್ಯ // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT> ಸ್ಥಿರ ಶೂನ್ಯ
transform_( ಸ್ಥಿರಾಂಕ T* src, T* dst, ಸ್ಥಿರಾಂಕ WT* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn )
{
    ಪೂರ್ಣಾಂಕ x;

    ಆದರೆ( scn == 2 && dcn == 2 )
    {
        ಗಾಗಿ( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( scn == 3 && dcn == 3 )
    {
        ಗಾಗಿ( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( scn == 3 && dcn == 1 )
    {
        ಗಾಗಿ( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( scn == 4 && dcn == 4 )
    {
        ಗಾಗಿ( x = 0; x < len*4; x += 4 )
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
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ಸ್ಥಿರಾಂಕ WT* _m = m;
            ಪೂರ್ಣಾಂಕ j, k;
            ಗಾಗಿ( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                ಗಾಗಿ( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_8u( ಸ್ಥಿರಾಂಕ uchar* src, uchar* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn )
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು = 10, SCALE = 1 << ಬಿಟ್ಟುಗಳು;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ MAX_M = (ದಶಮಾಂಶ)(1 << (15 - ಬಿಟ್ಟುಗಳು));

    ಆದರೆ( scn == 3 && dcn == 3 &&
        std::ನಿರಪೇಕ್ಷ(m[0]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[1]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[ 2]) < MAX_M*256 && std::ನಿರಪೇಕ್ಷ(m[ 3]) < MAX_M*256 &&
        std::ನಿರಪೇಕ್ಷ(m[4]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[5]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[ 6]) < MAX_M*256 && std::ನಿರಪೇಕ್ಷ(m[ 7]) < MAX_M*256 &&
        std::ನಿರಪೇಕ್ಷ(m[8]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[9]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[10]) < MAX_M*256 && std::ನಿರಪೇಕ್ಷ(m[11]) < MAX_M*256 )
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nChannels = 3;

        ಒಕ್ಕೂಟ {
            ಗಿಡ್ಡ s[6];
            ಪೂರ್ಣಾಂಕ p[3];
        } m16;
        m16.s[0] = saturate_cast<ಗಿಡ್ಡ>(m[0] * SCALE); m16.s[1] = saturate_cast<ಗಿಡ್ಡ>(m[1] * SCALE);
        m16.s[2] = saturate_cast<ಗಿಡ್ಡ>(m[4] * SCALE); m16.s[3] = saturate_cast<ಗಿಡ್ಡ>(m[5] * SCALE);
        m16.s[4] = saturate_cast<ಗಿಡ್ಡ>(m[8] * SCALE); m16.s[5] = saturate_cast<ಗಿಡ್ಡ>(m[9] * SCALE);
        ಪೂರ್ಣಾಂಕ m32[] = {saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 2] * SCALE), saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 3] * SCALE),
                     saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 6] * SCALE), saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 7] * SCALE),
                     saturate_cast<ಪೂರ್ಣಾಂಕ>(m[10] * SCALE), saturate_cast<ಪೂರ್ಣಾಂಕ>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        ಪೂರ್ಣಾಂಕ x = 0;
        ಗಾಗಿ (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
        {
            v_uint8 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
            v_uint8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_uint16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_uint16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack_u(dbl, dbh), v_pack_u(dgl, dgh), v_pack_u(drl, drh));
        }
        m32[1] = saturate_cast<ಪೂರ್ಣಾಂಕ>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<ಪೂರ್ಣಾಂಕ>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<ಪೂರ್ಣಾಂಕ>((m[11] + 0.5f)*SCALE);
        ಗಾಗಿ( ; x < len * nChannels; x += nChannels )
        {
            ಪೂರ್ಣಾಂಕ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> ಬಿಟ್ಟುಗಳು);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> ಬಿಟ್ಟುಗಳು);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> ಬಿಟ್ಟುಗಳು);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_16u( ಸ್ಥಿರಾಂಕ ushort* src, ushort* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn )
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ( scn == 3 && dcn == 3 )
    {
        ಪೂರ್ಣಾಂಕ x = 0;
#ಆದರೆ CV_SIMD_ಅಗಲ > 16
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
        ಗಾಗಿ (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
        {
            v_uint16 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
            v_uint32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_ಕೂಡಿಸು_wrap(v_pack(v_ಸುತ್ತು(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m2,  m3)))),
                                   v_ಸುತ್ತು(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m2,  m3))))), delta);
            dg = v_ಕೂಡಿಸು_wrap(v_pack(v_ಸುತ್ತು(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m6,  m7)))),
                                   v_ಸುತ್ತು(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m6,  m7))))), delta);
            dr = v_ಕೂಡಿಸು_wrap(v_pack(v_ಸುತ್ತು(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)), m10, m11)))),
                                   v_ಸುತ್ತು(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)), m10, m11))))), delta);
            v_store_interleave(dst + x, v_reinterpret_as_u16(db), v_reinterpret_as_u16(dg), v_reinterpret_as_u16(dr));
        }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_ಎಡ<1>(_m0l);
        v_float32x4 _m1h = v_rotate_ಎಡ<1>(_m1l);
        v_float32x4 _m2h = v_rotate_ಎಡ<1>(_m2l);
        v_float32x4 _m3h = v_rotate_ಎಡ<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        ಗಾಗಿ( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_ಬಲ<1>(v_reinterpret_as_u16(v_ಕೂಡಿಸು_wrap(v_pack(
                             v_ಸುತ್ತು(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_ತುಂಬು_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_ಸುತ್ತು(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_ತುಂಬು_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#ಆದರೆ_ಅಂತ್ಯ //CV_SIMD128
        ಗಾಗಿ( ; x < len * 3; x += 3 )
        {
            ದಶಮಾಂಶ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_32f( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    ಪೂರ್ಣಾಂಕ x = 0;
    ಆದರೆ( scn == 3 && dcn == 3 )
    {
        ಪೂರ್ಣಾಂಕ idx[VTraits<v_float32>::max_nlanes/2];
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        ದಶಮಾಂಶ _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        ಗಾಗಿ( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        ಗಾಗಿ( ; x < len*3; x += 3 )
        {
            ದಶಮಾಂಶ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            ದಶಮಾಂಶ t0 = saturate_cast<ದಶಮಾಂಶ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            ದಶಮಾಂಶ t1 = saturate_cast<ದಶಮಾಂಶ>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            ದಶಮಾಂಶ t2 = saturate_cast<ದಶಮಾಂಶ>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( scn == 4 && dcn == 4 )
    {
#ಆದರೆ CV_SIMD_ಅಗಲ > 16
        ಪೂರ್ಣಾಂಕ idx[VTraits<v_float32>::max_nlanes/4];
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        ದಶಮಾಂಶ _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        ಗಾಗಿ( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_ತುಂಬು(src + x);
            v_store(dst + x, v_ಕೂಡಿಸು(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ CV_SIMD128
        v_float32x4 _m0 = v_ತುಂಬು(m     );
        v_float32x4 _m1 = v_ತುಂಬು(m +  5);
        v_float32x4 _m2 = v_ತುಂಬು(m + 10);
        v_float32x4 _m3 = v_ತುಂಬು(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        ಗಾಗಿ( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_ತುಂಬು(src + x);
            v_store(dst + x, v_ಕೂಡಿಸು(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#ಇಲ್ಲದಿದ್ದರೆ // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        ಗಾಗಿ( ; x < len*4; x += 4 )
        {
            ದಶಮಾಂಶ v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            ದಶಮಾಂಶ t0 = saturate_cast<ದಶಮಾಂಶ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            ದಶಮಾಂಶ t1 = saturate_cast<ದಶಮಾಂಶ>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            ದಶಮಾಂಶ t2 = saturate_cast<ದಶಮಾಂಶ>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            ದಶಮಾಂಶ t3 = saturate_cast<ದಶಮಾಂಶ>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#ಆದರೆ_ಅಂತ್ಯ
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    transform_(src, dst, m, len, scn, dcn);
}


ಸ್ಥಿರ ಶೂನ್ಯ
transform_8s(ಸ್ಥಿರಾಂಕ schar* src, schar* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು = 10, SCALE = 1 << ಬಿಟ್ಟುಗಳು;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ MAX_M = (ದಶಮಾಂಶ)(1 << (15 - ಬಿಟ್ಟುಗಳು));

    ಆದರೆ( scn == 3 && dcn == 3 &&
        std::ನಿರಪೇಕ್ಷ(m[0]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[1]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[ 2]) < MAX_M*256 && std::ನಿರಪೇಕ್ಷ(m[ 3]) < MAX_M*256 &&
        std::ನಿರಪೇಕ್ಷ(m[4]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[5]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[ 6]) < MAX_M*256 && std::ನಿರಪೇಕ್ಷ(m[ 7]) < MAX_M*256 &&
        std::ನಿರಪೇಕ್ಷ(m[8]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[9]) < MAX_M && std::ನಿರಪೇಕ್ಷ(m[10]) < MAX_M*256 && std::ನಿರಪೇಕ್ಷ(m[11]) < MAX_M*256 )
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nChannels = 3;

        ಒಕ್ಕೂಟ {
            ಗಿಡ್ಡ s[6];
            ಪೂರ್ಣಾಂಕ p[3];
        } m16;
        m16.s[0] = saturate_cast<ಗಿಡ್ಡ>(m[0] * SCALE); m16.s[1] = saturate_cast<ಗಿಡ್ಡ>(m[1] * SCALE);
        m16.s[2] = saturate_cast<ಗಿಡ್ಡ>(m[4] * SCALE); m16.s[3] = saturate_cast<ಗಿಡ್ಡ>(m[5] * SCALE);
        m16.s[4] = saturate_cast<ಗಿಡ್ಡ>(m[8] * SCALE); m16.s[5] = saturate_cast<ಗಿಡ್ಡ>(m[9] * SCALE);
        ಪೂರ್ಣಾಂಕ m32[] = {saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 2] * SCALE), saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 3] * SCALE),
                     saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 6] * SCALE), saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 7] * SCALE),
                     saturate_cast<ಪೂರ್ಣಾಂಕ>(m[10] * SCALE), saturate_cast<ಪೂರ್ಣಾಂಕ>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        ಪೂರ್ಣಾಂಕ x = 0;
        ಗಾಗಿ (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
        {
            v_int8 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
            v_int8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_int16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_int16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<ಬಿಟ್ಟುಗಳು>(v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack(dbl, dbh), v_pack(dgl, dgh), v_pack(drl, drh));
        }
        m32[1] = saturate_cast<ಪೂರ್ಣಾಂಕ>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<ಪೂರ್ಣಾಂಕ>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<ಪೂರ್ಣಾಂಕ>((m[11] + 0.5f)*SCALE);
        ಗಾಗಿ( ; x < len * nChannels; x += nChannels )
        {
            ಪೂರ್ಣಾಂಕ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> ಬಿಟ್ಟುಗಳು);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> ಬಿಟ್ಟುಗಳು);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> ಬಿಟ್ಟುಗಳು);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಗಿಡ್ಡ* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

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
        ಗಾಗಿ (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
        {
            v_int16 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
            v_int32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_pack(v_ಸುತ್ತು(v_muladd(v_cvt_f32(bl), m0, v_muladd(v_cvt_f32(gl), m1, v_muladd(v_cvt_f32(rl),  m2,  m3)))),
                        v_ಸುತ್ತು(v_muladd(v_cvt_f32(bh), m0, v_muladd(v_cvt_f32(gh), m1, v_muladd(v_cvt_f32(rh),  m2,  m3)))));
            dg = v_pack(v_ಸುತ್ತು(v_muladd(v_cvt_f32(bl), m4, v_muladd(v_cvt_f32(gl), m5, v_muladd(v_cvt_f32(rl),  m6,  m7)))),
                        v_ಸುತ್ತು(v_muladd(v_cvt_f32(bh), m4, v_muladd(v_cvt_f32(gh), m5, v_muladd(v_cvt_f32(rh),  m6,  m7)))));
            dr = v_pack(v_ಸುತ್ತು(v_muladd(v_cvt_f32(bl), m8, v_muladd(v_cvt_f32(gl), m9, v_muladd(v_cvt_f32(rl), m10, m11)))),
                        v_ಸುತ್ತು(v_muladd(v_cvt_f32(bh), m8, v_muladd(v_cvt_f32(gh), m9, v_muladd(v_cvt_f32(rh), m10, m11)))));
            v_store_interleave(dst + x, db, dg, dr);
        }
        ಗಾಗಿ( ; x < len * 3; x += 3 )
        {
            ದಶಮಾಂಶ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ಗಿಡ್ಡ t0 = saturate_cast<ಗಿಡ್ಡ>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ಗಿಡ್ಡ t1 = saturate_cast<ಗಿಡ್ಡ>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ಗಿಡ್ಡ t2 = saturate_cast<ಗಿಡ್ಡ>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಪೂರ್ಣಾಂಕ* dst, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಆದರೆ( scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

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
        ಗಾಗಿ (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ಮೇಲಿನ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ಮೇಲಿನ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ಮೇಲಿನ(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_ಸುತ್ತು(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_ಸುತ್ತು(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_ಸುತ್ತು(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ದ್ವಿಗುಣ b = src[x], g = src[x + 1], r = src[x + 2];
            ಪೂರ್ಣಾಂಕ db = saturate_cast<ಪೂರ್ಣಾಂಕ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ಪೂರ್ಣಾಂಕ dg = saturate_cast<ಪೂರ್ಣಾಂಕ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ಪೂರ್ಣಾಂಕ dr = saturate_cast<ಪೂರ್ಣಾಂಕ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    transform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
transform_64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಆದರೆ( scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

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
        ಗಾಗಿ (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ದ್ವಿಗುಣ b = src[x], g = src[x + 1], r = src[x + 2];
            ದ್ವಿಗುಣ db = saturate_cast<ದ್ವಿಗುಣ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ದ್ವಿಗುಣ dg = saturate_cast<ದ್ವಿಗುಣ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ದ್ವಿಗುಣ dr = saturate_cast<ದ್ವಿಗುಣ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    transform_(src, dst, m, len, scn, dcn);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT> ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_( ಸ್ಥಿರಾಂಕ T* src, T* dst, ಸ್ಥಿರಾಂಕ WT* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ )
{
    ಪೂರ್ಣಾಂಕ x;

    ಆದರೆ( cn == 2 )
    {
        ಗಾಗಿ( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 3 )
    {
        ಗಾಗಿ( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( cn == 4 )
    {
        ಗಾಗಿ( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ಸ್ಥಿರಾಂಕ WT* _m = m;
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_8u(ಸ್ಥಿರಾಂಕ uchar* src, uchar* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಗಾಗಿ (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
            v_uint8 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack_u(v_ಸುತ್ತು(db_f32_l0), v_ಸುತ್ತು(db_f32_l1)), v_pack_u(v_ಸುತ್ತು(db_f32_h0), v_ಸುತ್ತು(db_f32_h1))),
                               v_pack(v_pack_u(v_ಸುತ್ತು(dg_f32_l0), v_ಸುತ್ತು(dg_f32_l1)), v_pack_u(v_ಸುತ್ತು(dg_f32_h0), v_ಸುತ್ತು(dg_f32_h1))),
                               v_pack(v_pack_u(v_ಸುತ್ತು(dr_f32_l0), v_ಸುತ್ತು(dr_f32_l1)), v_pack_u(v_ಸುತ್ತು(dr_f32_h0), v_ಸುತ್ತು(dr_f32_h1))));
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಂಕ b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_8s(ಸ್ಥಿರಾಂಕ schar* src, schar* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
    #ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಗಾಗಿ (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
            v_int8 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack(v_ಸುತ್ತು(db_f32_l0), v_ಸುತ್ತು(db_f32_l1)), v_pack(v_ಸುತ್ತು(db_f32_h0), v_ಸುತ್ತು(db_f32_h1))),
                               v_pack(v_pack(v_ಸುತ್ತು(dg_f32_l0), v_ಸುತ್ತು(dg_f32_l1)), v_pack(v_ಸುತ್ತು(dg_f32_h0), v_ಸುತ್ತು(dg_f32_h1))),
                               v_pack(v_pack(v_ಸುತ್ತು(dr_f32_l0), v_ಸುತ್ತು(dr_f32_l1)), v_pack(v_ಸುತ್ತು(dr_f32_h0), v_ಸುತ್ತು(dr_f32_h1))));
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಂಕ b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_16u(ಸ್ಥಿರಾಂಕ ushort* src, ushort* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಗಾಗಿ (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
            v_uint16 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
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
                               v_pack_u(v_ಸುತ್ತು(db_f32_l), v_ಸುತ್ತು(db_f32_h)),
                               v_pack_u(v_ಸುತ್ತು(dg_f32_l), v_ಸುತ್ತು(dg_f32_h)),
                               v_pack_u(v_ಸುತ್ತು(dr_f32_l), v_ಸುತ್ತು(dr_f32_h)));
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಂಕ b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src, ಗಿಡ್ಡ* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಗಾಗಿ (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
            v_int16 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
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
                               v_pack(v_ಸುತ್ತು(db_f32_l), v_ಸುತ್ತು(db_f32_h)),
                               v_pack(v_ಸುತ್ತು(dg_f32_l), v_ಸುತ್ತು(dg_f32_h)),
                               v_pack(v_ಸುತ್ತು(dr_f32_l), v_ಸುತ್ತು(dr_f32_h)));
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಂಕ b = src[x], g = src[x + 1], r = src[x + 2];
            ಗಿಡ್ಡ db = saturate_cast<ಗಿಡ್ಡ>(m[ 0] * b + m[ 3]);
            ಗಿಡ್ಡ dg = saturate_cast<ಗಿಡ್ಡ>(m[ 5] * g + m[ 7]);
            ಗಿಡ್ಡ dr = saturate_cast<ಗಿಡ್ಡ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಪೂರ್ಣಾಂಕ* dst, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಆದರೆ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ಗಾಗಿ (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ಮೇಲಿನ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ಮೇಲಿನ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ಮೇಲಿನ(r), rl = v_cvt_f64(r);

            v_int32 db = v_ಸುತ್ತು(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_ಸುತ್ತು(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_ಸುತ್ತು(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ಪೂರ್ಣಾಂಕ b = src[x], g = src[x + 1], r = src[x + 2];
            ಪೂರ್ಣಾಂಕ db = saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 0] * b + m[ 3]);
            ಪೂರ್ಣಾಂಕ dg = saturate_cast<ಪೂರ್ಣಾಂಕ>(m[ 5] * g + m[ 7]);
            ಪೂರ್ಣಾಂಕ dr = saturate_cast<ಪೂರ್ಣಾಂಕ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ಗಾಗಿ (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ದಶಮಾಂಶ b = src[x], g = src[x + 1], r = src[x + 2];
            ದಶಮಾಂಶ db = saturate_cast<ದಶಮಾಂಶ>(m[ 0] * b + m[ 3]);
            ದಶಮಾಂಶ dg = saturate_cast<ದಶಮಾಂಶ>(m[ 5] * g + m[ 7]);
            ದಶಮಾಂಶ dr = saturate_cast<ದಶಮಾಂಶ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ಸ್ಥಿರ ಶೂನ್ಯ
diagtransform_64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಆದರೆ (scn == 3 && dcn == 3) {
        ಪೂರ್ಣಾಂಕ x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ಗಾಗಿ (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_ತುಂಬು_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        ಗಾಗಿ (; x < len * 3; x += 3) {
            ದ್ವಿಗುಣ b = src[x], g = src[x + 1], r = src[x + 2];
            ದ್ವಿಗುಣ db = saturate_cast<ದ್ವಿಗುಣ>(m[ 0] * b + m[ 3]);
            ದ್ವಿಗುಣ dg = saturate_cast<ದ್ವಿಗುಣ>(m[ 5] * g + m[ 7]);
            ದ್ವಿಗುಣ dr = saturate_cast<ದ್ವಿಗುಣ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    diagtransform_(src, dst, m, len, scn, dcn);
}


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ T*, T*, ಸ್ಥಿರಾಂಕ WT*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ transformWrap(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* m, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn)
{
    fn((ಸ್ಥಿರಾಂಕ T*)src, (T*)dst, (ಸ್ಥಿರಾಂಕ WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ TransformFunc transformTab[CV_ಆಳ_MAX] =
    {
        transformWrap<uchar, ದಶಮಾಂಶ, transform_8u>, transformWrap<schar, ದಶಮಾಂಶ, transform_8s>, transformWrap<ushort, ದಶಮಾಂಶ, transform_16u>,
        transformWrap<ಗಿಡ್ಡ, ದಶಮಾಂಶ, transform_16s>, transformWrap<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, transform_32s>, transformWrap<ದಶಮಾಂಶ, ದಶಮಾಂಶ, transform_32f>,
        transformWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, transform_64f>, 0
    };

    ಹಿಂತಿರುಗಿ transformTab[ಆಳ];
}

TransformFunc getDiagTransformFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ TransformFunc diagTransformTab[CV_ಆಳ_MAX] =
    {
        transformWrap<uchar, ದಶಮಾಂಶ, diagtransform_8u>, transformWrap<schar, ದಶಮಾಂಶ, diagtransform_8s>, transformWrap<ushort, ದಶಮಾಂಶ, diagtransform_16u>,
        transformWrap<ಗಿಡ್ಡ, ದಶಮಾಂಶ, diagtransform_16s>, transformWrap<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, diagtransform_32s>, transformWrap<ದಶಮಾಂಶ, ದಶಮಾಂಶ, diagtransform_32f>,
        transformWrap<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, diagtransform_64f>, 0
    };

    ಹಿಂತಿರುಗಿ diagTransformTab[ಆಳ];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
perspectiveTransform_( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src, ಶೂನ್ಯ* _dst, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _m_ptr, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ scn, ಪೂರ್ಣಾಂಕ dcn )
{
    ಸ್ಥಿರಾಂಕ T* src = (ಸ್ಥಿರಾಂಕ T*)_src;
    T* dst = (T*)_dst;
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* m = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)_m_ptr;
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ eps = FLT_EPSILON;
    ಪೂರ್ಣಾಂಕ i;

    ಆದರೆ( scn == 2 && dcn == 2 )
    {
        ಗಾಗಿ( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            ದ್ವಿಗುಣ w = x*m[6] + y*m[7] + m[8];

            ಆದರೆ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            ಇಲ್ಲದಿದ್ದರೆ
                dst[i] = dst[i+1] = (T)0;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( scn == 3 && dcn == 3 )
    {
        ಗಾಗಿ( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            ದ್ವಿಗುಣ w = x*m[12] + y*m[13] + z*m[14] + m[15];

            ಆದರೆ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            ಇಲ್ಲದಿದ್ದರೆ
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( scn == 3 && dcn == 2 )
    {
        ಗಾಗಿ( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            ದ್ವಿಗುಣ w = x*m[8] + y*m[9] + z*m[10] + m[11];

            ಆದರೆ( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            ಇಲ್ಲದಿದ್ದರೆ
                dst[0] = dst[1] = (T)0;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* _m = m + dcn*(scn + 1);
            ದ್ವಿಗುಣ w = _m[scn];
            ಪೂರ್ಣಾಂಕ j, k;
            ಗಾಗಿ( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            ಆದರೆ( fabs(w) > eps )
            {
                _m = m;
                ಗಾಗಿ( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    ದ್ವಿಗುಣ s = _m[scn];
                    ಗಾಗಿ( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
                ಗಾಗಿ( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಆದರೆ (ಆಳ == CV_32F)
        ಹಿಂತಿರುಗಿ perspectiveTransform_<ದಶಮಾಂಶ>;
    ಆದರೆ (ಆಳ == CV_64F)
        ಹಿಂತಿರುಗಿ perspectiveTransform_<ದ್ವಿಗುಣ>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

ಸ್ಥಿರ ಶೂನ್ಯ scale_ಕೂಡಿಸು_32f(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, ಶೂನ್ಯ* _dst,
                         ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _alpha)
{
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1 = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)_src1;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2 = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)_src2;
    ದಶಮಾಂಶ* dst = (ದಶಮಾಂಶ*)_dst;
    ದಶಮಾಂಶ alpha = *(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)_alpha;
    ಪೂರ್ಣಾಂಕ i = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ c_ಅಗಲ = VTraits<v_float32>::vlanes();
    ಗಾಗಿ (; i <= len - c_ಅಗಲ; i += c_ಅಗಲ)
        v_store(dst + i, v_muladd(vx_ತುಂಬು(src1 + i), v_alpha, vx_ತುಂಬು(src2 + i)));
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


ಸ್ಥಿರ ಶೂನ್ಯ scale_ಕೂಡಿಸು_64f(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, ಶೂನ್ಯ* _dst,
                         ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _alpha)
{
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1 = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)_src1;
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2 = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)_src2;
    ದ್ವಿಗುಣ* dst = (ದ್ವಿಗುಣ*)_dst;
    ದ್ವಿಗುಣ alpha = *(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)_alpha;
    ಪೂರ್ಣಾಂಕ i = 0;
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ c_ಅಗಲ = VTraits<v_float64>::vlanes();
    ಗಾಗಿ (; i <= len - c_ಅಗಲ; i += c_ಅಗಲ)
        v_store(dst + i, v_muladd(vx_ತುಂಬು(src1 + i), a2, vx_ತುಂಬು(src2 + i)));
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

Scale_ಕೂಡಿಸು_Func get_Scale_ಕೂಡಿಸು_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಆದರೆ (ಆಳ == CV_32F)
        ಹಿಂತಿರುಗಿ scale_ಕೂಡಿಸು_32f;
    ಆದರೆ (ಆಳ == CV_64F)
        ಹಿಂತಿರುಗಿ scale_ಕೂಡಿಸು_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ದ್ವಿಗುಣ MahalanobisImpl(ಸ್ಥಿರಾಂಕ Mat& v1, ಸ್ಥಿರಾಂಕ Mat& v2, ಸ್ಥಿರಾಂಕ Mat& icovar, ದ್ವಿಗುಣ *diff_ಬಫರ್ /*[len]*/, ಪೂರ್ಣಾಂಕ len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    ಗಾತ್ರ_2 sz = v1.size();
    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0;

    sz.ಅಗಲ *= v1.channels();
    ಆದರೆ (v1.isContinuous() && v2.isContinuous())
    {
        sz.ಅಗಲ *= sz.ಎತ್ತರ;
        sz.ಎತ್ತರ = 1;
    }

    {
        ಸ್ಥಿರಾಂಕ T* src1 = v1.ptr<T>();
        ಸ್ಥಿರಾಂಕ T* src2 = v2.ptr<T>();
        size_t step1 = v1.ಹೆಜ್ಜೆ/ಗಾತ್ರ(src1[0]);
        size_t step2 = v2.ಹೆಜ್ಜೆ/ಗಾತ್ರ(src2[0]);
        ದ್ವಿಗುಣ* diff = diff_ಬಫರ್;
        ಸ್ಥಿರಾಂಕ T* mat = icovar.ptr<T>();
        size_t matstep = icovar.ಹೆಜ್ಜೆ/ಗಾತ್ರ(mat[0]);

        ಗಾಗಿ (; sz.ಎತ್ತರ--; src1 += step1, src2 += step2, diff += sz.ಅಗಲ)
        {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < sz.ಅಗಲ; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_ಬಫರ್;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++, mat += matstep)
        {
            ದ್ವಿಗುಣ ಅಡ್ಡಸಾಲು_ಮೊತ್ತ = 0;
            ಪೂರ್ಣಾಂಕ j = 0;
#ಆದರೆ CV_ENABLE_UNROLLED
            ಗಾಗಿ(; j <= len - 4; j += 4 )
                ಅಡ್ಡಸಾಲು_ಮೊತ್ತ += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ (; j < len; j++)
                ಅಡ್ಡಸಾಲು_ಮೊತ್ತ += diff[j]*mat[j];
            ಫಲಿತಾಂಶ += ಅಡ್ಡಸಾಲು_ಮೊತ್ತ * diff[i];
        }
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

MahalanobisImplFunc getMahalanobisImplFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಆದರೆ (ಆಳ == CV_32F)
        ಹಿಂತಿರುಗಿ (MahalanobisImplFunc)MahalanobisImpl<ದಶಮಾಂಶ>;
    ಆದರೆ (ಆಳ == CV_64F)
        ಹಿಂತಿರುಗಿ (MahalanobisImplFunc)MahalanobisImpl<ದ್ವಿಗುಣ>;
    CV_Assert(0 && "Not supported");
}


#ಆದರೆ !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_ಬಹುಲಕ)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು sT, ಬಗೆ_ಹೆಸರು dT> ಸ್ಥಿರ ಶೂನ್ಯ
MulTransposedR(ಸ್ಥಿರಾಂಕ Mat& srcmat, ಸ್ಥಿರಾಂಕ Mat& dstmat, ಸ್ಥಿರಾಂಕ Mat& deltamat, ದ್ವಿಗುಣ scale)
{
    ಪೂರ್ಣಾಂಕ i, j, k;
    ಸ್ಥಿರಾಂಕ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ಸ್ಥಿರಾಂಕ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.ಹೆಜ್ಜೆ/ಗಾತ್ರ(src[0]);
    size_t dststep = dstmat.ಹೆಜ್ಜೆ/ಗಾತ್ರ(dst[0]);
    size_t deltastep = deltamat.ಅಡ್ಡಸಾಲುಗಳು > 1 ? deltamat.ಹೆಜ್ಜೆ/ಗಾತ್ರ(delta[0]) : 0;
    ಪೂರ್ಣಾಂಕ delta_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = deltamat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಗಾತ್ರ_2 size = srcmat.size();
    dT* tdst = dst;
    dT* ಲಂಬಸಾಲು_buf = 0;
    dT* delta_buf = 0;
    ಪೂರ್ಣಾಂಕ buf_ಗಾತ್ರ_2 = size.ಎತ್ತರ*ಗಾತ್ರ(dT);
    Auto_ಬಫರ್<uchar> buf;

    ಆದರೆ( delta && delta_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ < size.ಅಗಲ )
    {
        CV_Assert( delta_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 );
        buf_ಗಾತ್ರ_2 *= 5;
    }
    buf.allocate(buf_ಗಾತ್ರ_2);
    ಲಂಬಸಾಲು_buf = (dT*)buf.ದತ್ತಾಂಶ();

    ಆದರೆ( delta && delta_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ < size.ಅಗಲ )
    {
        delta_buf = ಲಂಬಸಾಲು_buf + size.ಎತ್ತರ;
        ಗಾಗಿ( i = 0; i < size.ಎತ್ತರ; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#ಆದರೆ CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ( !delta )
        ಗಾಗಿ( i = 0; i < size.ಅಗಲ; i++, tdst += dststep )
        {
            ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++ )
                ಲಂಬಸಾಲು_buf[k] = src[k*srcstep+i];

            ಗಾಗಿ( j = i; j <= size.ಅಗಲ - 4; j += 4 )
            {
#ಆದರೆ CV_SIMD128_64F
                ಆದರೆ (ದತ್ತಾಂಶ_Type_2<sT>::ಆಳ == CV_64F && ದತ್ತಾಂಶ_Type_2<dT>::ಆಳ == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *tsrc = (ದ್ವಿಗುಣ*)(src + j);

                    ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((ದ್ವಿಗುಣ)ಲಂಬಸಾಲು_buf[k]);
                        s0 = v_ಕೂಡಿಸು(s0, v_mul(a, v_ತುಂಬು(tsrc + 0)));
                        s1 = v_ಕೂಡಿಸು(s1, v_mul(a, v_ತುಂಬು(tsrc + 2)));
                    }

                    v_store((ದ್ವಿಗುಣ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ದ್ವಿಗುಣ*)(tdst+j+2), v_mul(s1, v_scale));
                } ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
                {
                    ದ್ವಿಗುಣ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ಸ್ಥಿರಾಂಕ sT *tsrc = src + j;

                    ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++, tsrc += srcstep )
                    {
                        ದ್ವಿಗುಣ a = ಲಂಬಸಾಲು_buf[k];
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

            ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            {
                ದ್ವಿಗುಣ s0 = 0;
                ಸ್ಥಿರಾಂಕ sT *tsrc = src + j;

                ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++, tsrc += srcstep )
                    s0 += (ದ್ವಿಗುಣ)ಲಂಬಸಾಲು_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ( i = 0; i < size.ಅಗಲ; i++, tdst += dststep )
        {
            ಆದರೆ( !delta_buf )
                ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++ )
                    ಲಂಬಸಾಲು_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            ಇಲ್ಲದಿದ್ದರೆ
                ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++ )
                    ಲಂಬಸಾಲು_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            ಗಾಗಿ( j = i; j <= size.ಅಗಲ - 4; j += 4 )
            {
#ಆದರೆ CV_SIMD128_64F
                ಆದರೆ (ದತ್ತಾಂಶ_Type_2<sT>::ಆಳ == CV_64F && ದತ್ತಾಂಶ_Type_2<dT>::ಆಳ == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *tsrc = (ದ್ವಿಗುಣ*)(src + j);
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *d = (ದ್ವಿಗುಣ*)(delta_buf ? delta_buf : delta + j);

                    ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((ದ್ವಿಗುಣ)ಲಂಬಸಾಲು_buf[k]);
                        s0 = v_ಕೂಡಿಸು(s0, v_mul(a, v_sub(v_ತುಂಬು(tsrc + 0), v_ತುಂಬು(d + 0))));
                        s1 = v_ಕೂಡಿಸು(s1, v_mul(a, v_sub(v_ತುಂಬು(tsrc + 2), v_ತುಂಬು(d + 2))));
                    }

                    v_store((ದ್ವಿಗುಣ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ದ್ವಿಗುಣ*)(tdst+j+2), v_mul(s1, v_scale));
                }
                ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ

                {
                    ದ್ವಿಗುಣ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ಸ್ಥಿರಾಂಕ sT *tsrc = src + j;
                    ಸ್ಥಿರಾಂಕ dT *d = delta_buf ? delta_buf : delta + j;

                    ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        ದ್ವಿಗುಣ a = ಲಂಬಸಾಲು_buf[k];
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

            ಗಾಗಿ( ; j < size.ಅಗಲ; j++ )
            {
                ದ್ವಿಗುಣ s0 = 0;
                ಸ್ಥಿರಾಂಕ sT *tsrc = src + j;
                ಸ್ಥಿರಾಂಕ dT *d = delta_buf ? delta_buf : delta + j;

                ಗಾಗಿ( k = 0; k < size.ಎತ್ತರ; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (ದ್ವಿಗುಣ)ಲಂಬಸಾಲು_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು sT, ಬಗೆ_ಹೆಸರು dT> ಸ್ಥಿರ ಶೂನ್ಯ
MulTransposedL(ಸ್ಥಿರಾಂಕ Mat& srcmat, ಸ್ಥಿರಾಂಕ Mat& dstmat, ಸ್ಥಿರಾಂಕ Mat& deltamat, ದ್ವಿಗುಣ scale)
{
    ಪೂರ್ಣಾಂಕ i, j, k;
    ಸ್ಥಿರಾಂಕ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ಸ್ಥಿರಾಂಕ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.ಹೆಜ್ಜೆ/ಗಾತ್ರ(src[0]);
    size_t dststep = dstmat.ಹೆಜ್ಜೆ/ಗಾತ್ರ(dst[0]);
    size_t deltastep = deltamat.ಅಡ್ಡಸಾಲುಗಳು > 1 ? deltamat.ಹೆಜ್ಜೆ/ಗಾತ್ರ(delta[0]) : 0;
    ಪೂರ್ಣಾಂಕ delta_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = deltamat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಗಾತ್ರ_2 size = srcmat.size();
    dT* tdst = dst;

    ಆದರೆ( !delta )
        ಗಾಗಿ( i = 0; i < size.ಎತ್ತರ; i++, tdst += dststep )
            ಗಾಗಿ( j = i; j < size.ಎತ್ತರ; j++ )
            {
                ದ್ವಿಗುಣ s = 0;
                ಸ್ಥಿರಾಂಕ sT *tsrc1 = src + i*srcstep;
                ಸ್ಥಿರಾಂಕ sT *tsrc2 = src + j*srcstep;
#ಆದರೆ CV_SIMD128_64F
                ಆದರೆ (ದತ್ತಾಂಶ_Type_2<sT>::ಆಳ == CV_64F && ದತ್ತಾಂಶ_Type_2<dT>::ಆಳ == CV_64F)
                {
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *v_tsrc1 = (ದ್ವಿಗುಣ *)(tsrc1);
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *v_tsrc2 = (ದ್ವಿಗುಣ *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    ಗಾಗಿ( k = 0; k <= size.ಅಗಲ - 4; k += 4 )
                        v_s = v_ಕೂಡಿಸು(v_s, v_ಕೂಡಿಸು(v_mul(v_ತುಂಬು(v_tsrc1 + k), v_ತುಂಬು(v_tsrc2 + k)), v_mul(v_ತುಂಬು(v_tsrc1 + k + 2), v_ತುಂಬು(v_tsrc2 + k + 2))));
                    s += v_reduce_ಮೊತ್ತ(v_s);
                }
                ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
                {
                    ಗಾಗಿ( k = 0; k <= size.ಅಗಲ - 4; k += 4 )
                        s += (ದ್ವಿಗುಣ)tsrc1[k]*tsrc2[k] + (ದ್ವಿಗುಣ)tsrc1[k+1]*tsrc2[k+1] +
                             (ದ್ವಿಗುಣ)tsrc1[k+2]*tsrc2[k+2] + (ದ್ವಿಗುಣ)tsrc1[k+3]*tsrc2[k+3];
                }
                ಗಾಗಿ( ; k < size.ಅಗಲ; k++ )
                    s += (ದ್ವಿಗುಣ)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        dT delta_buf[4];
        ಪೂರ್ಣಾಂಕ delta_ಸರಿಸು = delta_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size.ಅಗಲ ? 4 : 0;
        Auto_ಬಫರ್<uchar> buf(size.ಅಗಲ*ಗಾತ್ರ(dT));
        dT* ಅಡ್ಡಸಾಲು_buf = (dT*)buf.ದತ್ತಾಂಶ();

        ಗಾಗಿ( i = 0; i < size.ಎತ್ತರ; i++, tdst += dststep )
        {
            ಸ್ಥಿರಾಂಕ sT *tsrc1 = src + i*srcstep;
            ಸ್ಥಿರಾಂಕ dT *tdelta1 = delta + i*deltastep;

            ಆದರೆ( delta_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ < size.ಅಗಲ )
                ಗಾಗಿ( k = 0; k < size.ಅಗಲ; k++ )
                    ಅಡ್ಡಸಾಲು_buf[k] = tsrc1[k] - tdelta1[0];
            ಇಲ್ಲದಿದ್ದರೆ
                ಗಾಗಿ( k = 0; k < size.ಅಗಲ; k++ )
                    ಅಡ್ಡಸಾಲು_buf[k] = tsrc1[k] - tdelta1[k];

            ಗಾಗಿ( j = i; j < size.ಎತ್ತರ; j++ )
            {
                ದ್ವಿಗುಣ s = 0;
                ಸ್ಥಿರಾಂಕ sT *tsrc2 = src + j*srcstep;
                ಸ್ಥಿರಾಂಕ dT *tdelta2 = delta + j*deltastep;
                ಆದರೆ( delta_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ < size.ಅಗಲ )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#ಆದರೆ CV_SIMD128_64F
                ಆದರೆ (ದತ್ತಾಂಶ_Type_2<sT>::ಆಳ == CV_64F && ದತ್ತಾಂಶ_Type_2<dT>::ಆಳ == CV_64F)
                {
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *v_tsrc2 = (ದ್ವಿಗುಣ *)(tsrc2);
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *v_tdelta2 = (ದ್ವಿಗುಣ *)(tdelta2);
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *v_ಅಡ್ಡಸಾಲು_buf = (ದ್ವಿಗುಣ *)(ಅಡ್ಡಸಾಲು_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    ಗಾಗಿ( k = 0; k <= size.ಅಗಲ - 4; k += 4, v_tdelta2 += delta_ಸರಿಸು )
                        v_s = v_ಕೂಡಿಸು(v_s, v_ಕೂಡಿಸು(v_mul(v_sub(v_ತುಂಬು(v_tsrc2 + k), v_ತುಂಬು(v_tdelta2)), v_ತುಂಬು(v_ಅಡ್ಡಸಾಲು_buf + k)), v_mul(v_sub(v_ತುಂಬು(v_tsrc2 + k + 2), v_ತುಂಬು(v_tdelta2 + 2)), v_ತುಂಬು(v_ಅಡ್ಡಸಾಲು_buf + k + 2))));
                    s += v_reduce_ಮೊತ್ತ(v_s);

                    tdelta2 = (ಸ್ಥಿರಾಂಕ dT *)(v_tdelta2);
                }
                ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
                {
                    ಗಾಗಿ( k = 0; k <= size.ಅಗಲ-4; k += 4, tdelta2 += delta_ಸರಿಸು )
                        s += (ದ್ವಿಗುಣ)ಅಡ್ಡಸಾಲು_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (ದ್ವಿಗುಣ)ಅಡ್ಡಸಾಲು_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (ದ್ವಿಗುಣ)ಅಡ್ಡಸಾಲು_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (ದ್ವಿಗುಣ)ಅಡ್ಡಸಾಲು_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                ಗಾಗಿ( ; k < size.ಅಗಲ; k++, tdelta2++ )
                    s += (ದ್ವಿಗುಣ)ಅಡ್ಡಸಾಲು_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(ಪೂರ್ಣಾಂಕ stype, ಪೂರ್ಣಾಂಕ dtype, ತಾರ್ಕಿಕ ata)
{
    MulTransposedFunc func = ಖಾಲಿ;
    ಆದರೆ (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,ದಶಮಾಂಶ>
                   : MulTransposedL<uchar,ದಶಮಾಂಶ>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,ದ್ವಿಗುಣ>
                   : MulTransposedL<uchar,ದ್ವಿಗುಣ>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,ದಶಮಾಂಶ>
                   : MulTransposedL<ushort,ದಶಮಾಂಶ>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,ದ್ವಿಗುಣ>
                   : MulTransposedL<ushort,ದ್ವಿಗುಣ>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ಗಿಡ್ಡ,ದಶಮಾಂಶ>
                   : MulTransposedL<ಗಿಡ್ಡ,ದಶಮಾಂಶ>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ಗಿಡ್ಡ,ದ್ವಿಗುಣ>
                   : MulTransposedL<ಗಿಡ್ಡ,ದ್ವಿಗುಣ>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ದಶಮಾಂಶ,ದಶಮಾಂಶ>
                   : MulTransposedL<ದಶಮಾಂಶ,ದಶಮಾಂಶ>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ದಶಮಾಂಶ,ದ್ವಿಗುಣ>
                   : MulTransposedL<ದಶಮಾಂಶ,ದ್ವಿಗುಣ>;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ>
                   : MulTransposedL<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ>;
    }
    CV_Assert(func && "Not supported");
    ಹಿಂತಿರುಗಿ func;
}
#ಆದರೆ_ಅಂತ್ಯ // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ದ್ವಿಗುಣ dotProd_(ಸ್ಥಿರಾಂಕ T* src1, ಸ್ಥಿರಾಂಕ T* src2, ಪೂರ್ಣಾಂಕ len)
{
    ಪೂರ್ಣಾಂಕ i = 0;
    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0;

#ಆದರೆ CV_ENABLE_UNROLLED
    ಗಾಗಿ( ; i <= len - 4; i += 4 )
        ಫಲಿತಾಂಶ += (ದ್ವಿಗುಣ)src1[i]*src2[i] + (ದ್ವಿಗುಣ)src1[i+1]*src2[i+1] +
            (ದ್ವಿಗುಣ)src1[i+2]*src2[i+2] + (ದ್ವಿಗುಣ)src1[i+3]*src2[i+3];
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ( ; i < len; i++ )
        ಫಲಿತಾಂಶ += (ದ್ವಿಗುಣ)src1[i]*src2[i];

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}


ದ್ವಿಗುಣ dotProd_8u(ಸ್ಥಿರಾಂಕ uchar* src1, ಸ್ಥಿರಾಂಕ uchar* src2, ಪೂರ್ಣಾಂಕ len)
{
    ದ್ವಿಗುಣ r = 0;
    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಂಕ len0 = len & -VTraits<v_uint16>::vlanes(), ಖಂಡ_Size0 = (1 << 15), ಖಂಡ_ಗಾತ್ರ_2;

    ಆಗ (i < len0)
    {
        ಖಂಡ_ಗಾತ್ರ_2 = std::min(len0 - i, ಖಂಡ_Size0);
        v_uint32 v_ಮೊತ್ತ = vx_setzero_u32();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ c_ಅಗಲ = VTraits<v_uint16>::vlanes();

        ಪೂರ್ಣಾಂಕ j = 0;
        ಗಾಗಿ (; j <= ಖಂಡ_ಗಾತ್ರ_2 - c_ಅಗಲ * 2; j += c_ಅಗಲ * 2)
        {
            v_uint8 v_src1 = vx_ತುಂಬು(src1 + j);
            v_uint8 v_src2 = vx_ತುಂಬು(src2 + j);
            v_ಮೊತ್ತ = v_dotprod_expand_fast(v_src1, v_src2, v_ಮೊತ್ತ);
        }

        ಗಾಗಿ (; j <= ಖಂಡ_ಗಾತ್ರ_2 - c_ಅಗಲ; j += c_ಅಗಲ)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_ತುಂಬು_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_ತುಂಬು_expand(src2 + j));
            v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (ದ್ವಿಗುಣ)v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);

        src1 += ಖಂಡ_ಗಾತ್ರ_2;
        src2 += ಖಂಡ_ಗಾತ್ರ_2;
        i += ಖಂಡ_ಗಾತ್ರ_2;
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ r + dotProd_(src1, src2, len - i);
}


ದ್ವಿಗುಣ dotProd_8s(ಸ್ಥಿರಾಂಕ schar* src1, ಸ್ಥಿರಾಂಕ schar* src2, ಪೂರ್ಣಾಂಕ len)
{
    ದ್ವಿಗುಣ r = 0.0;
    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಂಕ len0 = len & -VTraits<v_int16>::vlanes(), ಖಂಡ_Size0 = (1 << 14), ಖಂಡ_ಗಾತ್ರ_2;

    ಆಗ (i < len0)
    {
        ಖಂಡ_ಗಾತ್ರ_2 = std::min(len0 - i, ಖಂಡ_Size0);
        v_int32 v_ಮೊತ್ತ = vx_setzero_s32();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ c_ಅಗಲ = VTraits<v_int16>::vlanes();

        ಪೂರ್ಣಾಂಕ j = 0;
        ಗಾಗಿ (; j <= ಖಂಡ_ಗಾತ್ರ_2 - c_ಅಗಲ * 2; j += c_ಅಗಲ * 2)
        {
            v_int8 v_src1 = vx_ತುಂಬು(src1 + j);
            v_int8 v_src2 = vx_ತುಂಬು(src2 + j);
            v_ಮೊತ್ತ = v_dotprod_expand_fast(v_src1, v_src2, v_ಮೊತ್ತ);
        }

        ಗಾಗಿ (; j <= ಖಂಡ_ಗಾತ್ರ_2 - c_ಅಗಲ; j += c_ಅಗಲ)
        {
            v_int16 v_src1 = vx_ತುಂಬು_expand(src1 + j);
            v_int16 v_src2 = vx_ತುಂಬು_expand(src2 + j);
            v_ಮೊತ್ತ = v_dotprod_fast(v_src1, v_src2, v_ಮೊತ್ತ);
        }
        r += (ದ್ವಿಗುಣ)v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);

        src1 += ಖಂಡ_ಗಾತ್ರ_2;
        src2 += ಖಂಡ_ಗಾತ್ರ_2;
        i += ಖಂಡ_ಗಾತ್ರ_2;
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ r + dotProd_(src1, src2, len - i);
}

ದ್ವಿಗುಣ dotProd_16u(ಸ್ಥಿರಾಂಕ ushort* src1, ಸ್ಥಿರಾಂಕ ushort* src2, ಪೂರ್ಣಾಂಕ len)
{
    ದ್ವಿಗುಣ r = 0.0;
    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಂಕ len0 = len & -VTraits<v_uint16>::vlanes(), ಖಂಡ_Size0 = (1 << 24), ಖಂಡ_ಗಾತ್ರ_2;

    ಆಗ (i < len0)
    {
        ಖಂಡ_ಗಾತ್ರ_2 = std::min(len0 - i, ಖಂಡ_Size0);
        v_uint64 v_ಮೊತ್ತ = vx_setzero_u64();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ c_ಅಗಲ = VTraits<v_uint16>::vlanes();

        ಪೂರ್ಣಾಂಕ j = 0;
        ಗಾಗಿ (; j <= ಖಂಡ_ಗಾತ್ರ_2 - c_ಅಗಲ; j += c_ಅಗಲ)
        {
            v_uint16 v_src1 = vx_ತುಂಬು(src1 + j);
            v_uint16 v_src2 = vx_ತುಂಬು(src2 + j);
            v_ಮೊತ್ತ = v_dotprod_expand_fast(v_src1, v_src2, v_ಮೊತ್ತ);
        }
        r += (ದ್ವಿಗುಣ)v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);

        src1 += ಖಂಡ_ಗಾತ್ರ_2;
        src2 += ಖಂಡ_ಗಾತ್ರ_2;
        i += ಖಂಡ_ಗಾತ್ರ_2;
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ r + dotProd_(src1, src2, len - i);
}

ದ್ವಿಗುಣ dotProd_16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* src2, ಪೂರ್ಣಾಂಕ len)
{
    ದ್ವಿಗುಣ r = 0.0;
    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಂಕ len0 = len & -VTraits<v_int16>::vlanes(), ಖಂಡ_Size0 = (1 << 24), ಖಂಡ_ಗಾತ್ರ_2;

    ಆಗ (i < len0)
    {
        ಖಂಡ_ಗಾತ್ರ_2 = std::min(len0 - i, ಖಂಡ_Size0);
        v_int64 v_ಮೊತ್ತ = vx_setzero_s64();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ c_ಅಗಲ = VTraits<v_int16>::vlanes();

        ಪೂರ್ಣಾಂಕ j = 0;
        ಗಾಗಿ (; j <= ಖಂಡ_ಗಾತ್ರ_2 - c_ಅಗಲ; j += c_ಅಗಲ)
        {
            v_int16 v_src1 = vx_ತುಂಬು(src1 + j);
            v_int16 v_src2 = vx_ತುಂಬು(src2 + j);
            v_ಮೊತ್ತ = v_dotprod_expand_fast(v_src1, v_src2, v_ಮೊತ್ತ);
        }
        r += (ದ್ವಿಗುಣ)v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);

        src1 += ಖಂಡ_ಗಾತ್ರ_2;
        src2 += ಖಂಡ_ಗಾತ್ರ_2;
        i += ಖಂಡ_ಗಾತ್ರ_2;
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ r + dotProd_(src1, src2, len - i);
}

ದ್ವಿಗುಣ dotProd_32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src2, ಪೂರ್ಣಾಂಕ len)
{
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ದ್ವಿಗುಣ r = .0;
    ಪೂರ್ಣಾಂಕ i = 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#ಆದರೆ CV_SIMD_ಅಗಲ == 16
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ wstep = ಹೆಜ್ಜೆ * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    ಗಾಗಿ (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
    {
        v_int32 v_src10 = vx_ತುಂಬು(src1);
        v_int32 v_src20 = vx_ತುಂಬು(src2);
        v_int32 v_src11 = vx_ತುಂಬು(src1 + ಹೆಜ್ಜೆ);
        v_int32 v_src21 = vx_ತುಂಬು(src2 + ಹೆಜ್ಜೆ);
        v_sum0 = v_dotprod_expand_fast(v_src10, v_src20, v_sum0);
        v_sum1 = v_dotprod_expand_fast(v_src11, v_src21, v_sum1);
    }
    v_sum0 = v_ಕೂಡಿಸು(v_sum0, v_sum1);
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ (; i <= len - ಹೆಜ್ಜೆ; i += ಹೆಜ್ಜೆ, src1 += ಹೆಜ್ಜೆ, src2 += ಹೆಜ್ಜೆ)
    {
        v_int32 v_src1 = vx_ತುಂಬು(src1);
        v_int32 v_src2 = vx_ತುಂಬು(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_ಮೊತ್ತ(v_sum0);
    vx_cleanup();
    ಹಿಂತಿರುಗಿ r + dotProd_(src1, src2, len - i);
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ dotProd_(src1, src2, len);
#ಆದರೆ_ಅಂತ್ಯ
}

ದ್ವಿಗುಣ dotProd_32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, ಪೂರ್ಣಾಂಕ len)
{
    ದ್ವಿಗುಣ r = 0.0;
    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಪೂರ್ಣಾಂಕ len0 = len & -VTraits<v_float32>::vlanes(), ಖಂಡ_Size0 = (1 << 13), ಖಂಡ_ಗಾತ್ರ_2;

    ಆಗ (i < len0)
    {
        ಖಂಡ_ಗಾತ್ರ_2 = std::min(len0 - i, ಖಂಡ_Size0);
        v_float32 v_ಮೊತ್ತ = vx_setzero_f32();

        ಪೂರ್ಣಾಂಕ j = 0;
        ಪೂರ್ಣಾಂಕ c_ಅಗಲ = VTraits<v_float32>::vlanes();

#ಆದರೆ CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        ಗಾಗಿ (; j <= ಖಂಡ_ಗಾತ್ರ_2 - (c_ಅಗಲ * 4); j += (c_ಅಗಲ * 4))
        {
            v_ಮೊತ್ತ  = v_muladd(vx_ತುಂಬು(src1 + j),
                              vx_ತುಂಬು(src2 + j), v_ಮೊತ್ತ);
            v_sum1 = v_muladd(vx_ತುಂಬು(src1 + j + c_ಅಗಲ),
                              vx_ತುಂಬು(src2 + j + c_ಅಗಲ), v_sum1);
            v_sum2 = v_muladd(vx_ತುಂಬು(src1 + j + (c_ಅಗಲ * 2)),
                              vx_ತುಂಬು(src2 + j + (c_ಅಗಲ * 2)), v_sum2);
            v_sum3 = v_muladd(vx_ತುಂಬು(src1 + j + (c_ಅಗಲ * 3)),
                              vx_ತುಂಬು(src2 + j + (c_ಅಗಲ * 3)), v_sum3);
        }

        v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(v_sum1, v_sum2), v_sum3));
#ಆದರೆ_ಅಂತ್ಯ

        ಗಾಗಿ (; j <= ಖಂಡ_ಗಾತ್ರ_2 - c_ಅಗಲ; j += c_ಅಗಲ)
            v_ಮೊತ್ತ = v_muladd(vx_ತುಂಬು(src1 + j), vx_ತುಂಬು(src2 + j), v_ಮೊತ್ತ);

        r += v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);

        src1 += ಖಂಡ_ಗಾತ್ರ_2;
        src2 += ಖಂಡ_ಗಾತ್ರ_2;
        i += ಖಂಡ_ಗಾತ್ರ_2;
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ r + dotProd_(src1, src2, len - i);
}

ದ್ವಿಗುಣ dotProd_64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, ಪೂರ್ಣಾಂಕ len)
{
    ಹಿಂತಿರುಗಿ dotProd_(src1, src2, len);
}

#ಆದರೆ_ಅಂತ್ಯ
CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace
