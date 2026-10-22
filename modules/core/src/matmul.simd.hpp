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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LAPACK
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_GEMM_BASELINE_ONLY
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_MAHALANOBIS_BASELINE_ONLY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_MULTRANSPOSED_BASELINE_ONLY

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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_GEMM_SIMD_ENABLED
#ਜੇ defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_GEMM_SIMD_ENABLED 0
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_GEMM_SIMD_ENABLED 1
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv {

// forward declarations
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*TransformFunc)(ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ* dst, ਅਚਲ ਖਾਲੀ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*Scale_ਜੋੜੋ_Func)(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਖਾਲੀ* dst, ਪੂਰਨ len, ਅਚਲ ਖਾਲੀ* alpha);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*MulTransposedFunc)(ਅਚਲ Mat& src, ਅਚਲ/*preallocated*/ Mat& dst, ਅਚਲ Mat& delta, ਦੁੱਗਣਾ scale);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦੁੱਗਣਾ (*MahalanobisImplFunc)(ਅਚਲ Mat& v1, ਅਚਲ Mat& v2, ਅਚਲ Mat& icovar, ਦੁੱਗਣਾ *diff_ਬਫਰ /*[len]*/, ਪੂਰਨ len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

ਖਾਲੀ gemm32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
             ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
             ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags);
ਖਾਲੀ gemm64f(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
             ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
             ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags);
ਖਾਲੀ gemm32fc(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
              ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
              ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags);
ਖਾਲੀ gemm64fc(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
              ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
              ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags);

TransformFunc getTransformFunc(ਪੂਰਨ ਡੂੰਘਾਈ);
TransformFunc getDiagTransformFunc(ਪੂਰਨ ਡੂੰਘਾਈ);
TransformFunc getPerspectiveTransform(ਪੂਰਨ ਡੂੰਘਾਈ);

Scale_ਜੋੜੋ_Func get_Scale_ਜੋੜੋ_Func(ਪੂਰਨ ਡੂੰਘਾਈ);

MahalanobisImplFunc getMahalanobisImplFunc(ਪੂਰਨ ਡੂੰਘਾਈ);

MulTransposedFunc getMulTransposedFunc(ਪੂਰਨ stype, ਪੂਰਨ dtype, ਤਾਰਕਿਕ ata);

ਦੁੱਗਣਾ dotProd_8u(ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਪੂਰਨ len);
ਦੁੱਗਣਾ dotProd_8s(ਅਚਲ schar* src1, ਅਚਲ schar* src2, ਪੂਰਨ len);
ਦੁੱਗਣਾ dotProd_16u(ਅਚਲ ushort* src1, ਅਚਲ ushort* src2, ਪੂਰਨ len);
ਦੁੱਗਣਾ dotProd_16s(ਅਚਲ ਛੋਟਾ* src1, ਅਚਲ ਛੋਟਾ* src2, ਪੂਰਨ len);
ਦੁੱਗਣਾ dotProd_32s(ਅਚਲ ਪੂਰਨ* src1, ਅਚਲ ਪੂਰਨ* src2, ਪੂਰਨ len);
ਦੁੱਗਣਾ dotProd_32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, ਪੂਰਨ len);
ਦੁੱਗਣਾ dotProd_64f(ਅਚਲ ਦੁੱਗਣਾ* src1, ਅਚਲ ਦੁੱਗਣਾ* src2, ਪੂਰਨ len);



#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ਜੇ !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_ਬਹੁਲਕ)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

ਸਥਿਰ ਖਾਲੀ
GEMM_Copy_ਖੰਡ( ਅਚਲ uchar* src, size_t src_ਕਦਮ,
                uchar* dst, size_t dst_ਕਦਮ,
                ਮਾਪ size, size_t pix_ਮਾਪ )
{
    ਪੂਰਨ j;
    size.ਚੌੜਾਈ *= (ਪੂਰਨ)(pix_ਮਾਪ / ਆਕਾਰ(ਪੂਰਨ));

    ਲਈ( ; size.ਉਚਾਈ--; src += src_ਕਦਮ, dst += dst_ਕਦਮ )
    {
        j=0;
         #ਜੇ CV_ENABLE_UNROLLED
        ਲਈ( ; j <= size.ਚੌੜਾਈ - 4; j += 4 )
        {
            ਪੂਰਨ t0 = ((ਅਚਲ ਪੂਰਨ*)src)[j];
            ਪੂਰਨ t1 = ((ਅਚਲ ਪੂਰਨ*)src)[j+1];
            ((ਪੂਰਨ*)dst)[j] = t0;
            ((ਪੂਰਨ*)dst)[j+1] = t1;
            t0 = ((ਅਚਲ ਪੂਰਨ*)src)[j+2];
            t1 = ((ਅਚਲ ਪੂਰਨ*)src)[j+3];
            ((ਪੂਰਨ*)dst)[j+2] = t0;
            ((ਪੂਰਨ*)dst)[j+3] = t1;
        }
        #ਜੇ_ਅੰਤ
        ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            ((ਪੂਰਨ*)dst)[j] = ((ਅਚਲ ਪੂਰਨ*)src)[j];
    }
}


ਸਥਿਰ ਖਾਲੀ
GEMM_Transpose_ਖੰਡ( ਅਚਲ uchar* src, size_t src_ਕਦਮ,
                     uchar* dst, size_t dst_ਕਦਮ,
                     ਮਾਪ size, size_t pix_ਮਾਪ )
{
    ਪੂਰਨ i, j;
    ਲਈ( i = 0; i < size.ਚੌੜਾਈ; i++, dst += dst_ਕਦਮ, src += pix_ਮਾਪ )
    {
        ਅਚਲ uchar* _src = src;
        ਚੋਣ( pix_ਮਾਪ )
        {
        ਹਾਲਤ ਆਕਾਰ(ਪੂਰਨ):
            ਲਈ( j = 0; j < size.ਉਚਾਈ; j++, _src += src_ਕਦਮ )
                ((ਪੂਰਨ*)dst)[j] = ((ਪੂਰਨ*)_src)[0];
            ਰੋਕੋ;
        ਹਾਲਤ ਆਕਾਰ(ਪੂਰਨ)*2:
            ਲਈ( j = 0; j < size.ਉਚਾਈ*2; j += 2, _src += src_ਕਦਮ )
            {
                ਪੂਰਨ t0 = ((ਪੂਰਨ*)_src)[0];
                ਪੂਰਨ t1 = ((ਪੂਰਨ*)_src)[1];
                ((ਪੂਰਨ*)dst)[j] = t0;
                ((ਪੂਰਨ*)dst)[j+1] = t1;
            }
            ਰੋਕੋ;
        ਹਾਲਤ ਆਕਾਰ(ਪੂਰਨ)*4:
            ਲਈ( j = 0; j < size.ਉਚਾਈ*4; j += 4, _src += src_ਕਦਮ )
            {
                ਪੂਰਨ t0 = ((ਪੂਰਨ*)_src)[0];
                ਪੂਰਨ t1 = ((ਪੂਰਨ*)_src)[1];
                ((ਪੂਰਨ*)dst)[j] = t0;
                ((ਪੂਰਨ*)dst)[j+1] = t1;
                t0 = ((ਪੂਰਨ*)_src)[2];
                t1 = ((ਪੂਰਨ*)_src)[3];
                ((ਪੂਰਨ*)dst)[j+2] = t0;
                ((ਪੂਰਨ*)dst)[j+3] = t1;
            }
            ਰੋਕੋ;
        ਮੂਲ:
            CV_Assert(0);
            ਵਾਪਸ;
        }
    }
}

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ST> ਬਣਤਰ VecTraits;

ਸਾਂਚਾ<> ਬਣਤਰ VecTraits<ਦਸ਼ਮਲਵ> {
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ v_float32 VT;
    ਸਥਿਰ ਸਤਰਅੰਦਰ VT zero() { ਵਾਪਸ vx_setzero_f32(); }
    ਸਥਿਰ ਸਤਰਅੰਦਰ VT load(ਅਚਲ ਦਸ਼ਮਲਵ* p) { ਵਾਪਸ vx_ਲੱਦੋ(p); }
    ਸਥਿਰ ਸਤਰਅੰਦਰ VT setall(ਦਸ਼ਮਲਵ v) { ਵਾਪਸ vx_setall_f32(v); }
};
#ਜੇ_ਅੰਤ

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ਸਾਂਚਾ<> ਬਣਤਰ VecTraits<ਦੁੱਗਣਾ> {
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ v_float64 VT;
    ਸਥਿਰ ਸਤਰਅੰਦਰ VT zero() { ਵਾਪਸ vx_setzero_f64(); }
    ਸਥਿਰ ਸਤਰਅੰਦਰ VT load(ਅਚਲ ਦੁੱਗਣਾ* p) { ਵਾਪਸ vx_ਲੱਦੋ(p); }
    ਸਥਿਰ ਸਤਰਅੰਦਰ VT setall(ਦੁੱਗਣਾ v) { ਵਾਪਸ vx_setall_f64(v); }
};
#ਜੇ_ਅੰਤ

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ST>
ਸਥਿਰ ਸਤਰਅੰਦਰ ST simd_Dot_ਗੁਣਨਫਲ(ਅਚਲ ST* a, ਅਚਲ ST* b, ਪੂਰਨ n, ਪੂਰਨ& k)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ VecTraits<ST>::VT VecT;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VecT>::vlanes();
    ਅਚਲ ਪੂਰਨ stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    ਲਈ( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_ਜੋੜੋ(v_ਜੋੜੋ(vs0, vs1), v_ਜੋੜੋ(vs2, vs3));
    ਲਈ( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    ਵਾਪਸ v_reduce_ਜੋੜ(vs0);
}
#ਜੇ_ਅੰਤ

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ simd_Dot_ਗੁਣਨਫਲ_f32f64(ਅਚਲ ਦਸ਼ਮਲਵ* a, ਅਚਲ ਦਸ਼ਮਲਵ* b, ਪੂਰਨ n, ਪੂਰਨ& k)
{
    ਅਚਲ ਪੂਰਨ vlanes_f32 = VTraits<v_float32>::vlanes();
    ਅਚਲ ਪੂਰਨ stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    ਲਈ( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_ਲੱਦੋ(a + k);
        v_float32 vb0 = vx_ਲੱਦੋ(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_ਉੱਚਾ(va0), v_cvt_f64_ਉੱਚਾ(vb0), vs1);

        v_float32 va1 = vx_ਲੱਦੋ(a + k + vlanes_f32);
        v_float32 vb1 = vx_ਲੱਦੋ(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_ਉੱਚਾ(va1), v_cvt_f64_ਉੱਚਾ(vb1), vs3);
    }
    vs0 = v_ਜੋੜੋ(v_ਜੋੜੋ(vs0, vs1), v_ਜੋੜੋ(vs2, vs3));
    ਲਈ( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_ਲੱਦੋ(a + k);
        v_float32 vb = vx_ਲੱਦੋ(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_ਉੱਚਾ(va), v_cvt_f64_ਉੱਚਾ(vb), vs0);
    }
    ਵਾਪਸ v_reduce_ਜੋੜ(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ simdGEMM_kj_f32f64(
    ਅਚਲ ਦਸ਼ਮਲਵ* a_ਅੰਕੜੇ, ਅਚਲ ਦਸ਼ਮਲਵ* b_ਅੰਕੜੇ_2, size_t b_ਕਦਮ,
    ਅਚਲ ਦਸ਼ਮਲਵ* c_ਅੰਕੜੇ, size_t c_step1,
    ਦਸ਼ਮਲਵ* d_ਅੰਕੜੇ,
    ਪੂਰਨ n, ਪੂਰਨ m,
    ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta,
    ਪੂਰਨ& j)
{
    ਅਚਲ ਪੂਰਨ vlanes_f32 = VTraits<v_float32>::vlanes();
    ਅਚਲ ਪੂਰਨ vlanes_f64 = VTraits<v_float64>::vlanes();

    ਜੇ( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ਅਚਲ ਪੂਰਨ max_m = 1600 / (ਪੂਰਨ)ਆਕਾਰ(ਦਸ਼ਮਲਵ);
        cv::Auto_ਬਫਰ<ਦੁੱਗਣਾ> _s_buf(max_m);
        ਦੁੱਗਣਾ* s_buf = _s_buf.ਅੰਕੜੇ();

        {
            v_float64 vz = vx_setzero_f64();
            ਲਈ( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            ਲਈ( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ਅਚਲ ਦਸ਼ਮਲਵ* b_ptr = b_ਅੰਕੜੇ_2;
            ਲਈ( ਪੂਰਨ k = 0; k < n; k++, b_ptr += b_ਕਦਮ )
            {
                v_float64 va = vx_setall_f64((ਦੁੱਗਣਾ)a_ਅੰਕੜੇ[k]);
                j = 0;
                ਲਈ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_ਲੱਦੋ(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_ਉੱਚਾ(vb);
                    v_float64 vd_lo = vx_ਲੱਦੋ(s_buf + j);
                    v_float64 vd_hi = vx_ਲੱਦੋ(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                ਦੁੱਗਣਾ a_val = (ਦੁੱਗਣਾ)a_ਅੰਕੜੇ[k];
                ਲਈ( ; j < m; j++ )
                    s_buf[j] += a_val * (ਦੁੱਗਣਾ)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ਅਚਲ ਦਸ਼ਮਲਵ* c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;
            j = 0;
            ਜੇ( !c_ਅੰਕੜੇ )
            {
                ਲਈ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_ਲੱਦੋ(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_ਲੱਦੋ(s_buf + j + vlanes_f64), valpha);
                    v_store(d_ਅੰਕੜੇ + j, v_cvt_f32(vs_lo, vs_hi));
                }
                ਲਈ( ; j < m; j++ )
                    d_ਅੰਕੜੇ[j] = (ਦਸ਼ਮਲਵ)(s_buf[j] * alpha);
            }
            ਨਹੀਂਤਾਂ ਜੇ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                ਲਈ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_ਲੱਦੋ(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_ਲੱਦੋ(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_ਲੱਦੋ(c_ਅੰਕੜੇ_2 + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_ਉੱਚਾ(vc), vbeta, vs_hi);
                    v_store(d_ਅੰਕੜੇ + j, v_cvt_f32(vs_lo, vs_hi));
                }
                ਲਈ( ; j < m; j++ )
                    d_ਅੰਕੜੇ[j] = (ਦਸ਼ਮਲਵ)(s_buf[j] * alpha + (ਦੁੱਗਣਾ)c_ਅੰਕੜੇ_2[j] * beta);
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ( j = 0; j < m; j++, c_ਅੰਕੜੇ_2 += c_step1 )
                    d_ਅੰਕੜੇ[j] = (ਦਸ਼ਮਲਵ)(s_buf[j] * alpha + (ਦੁੱਗਣਾ)c_ਅੰਕੜੇ_2[0] * beta);
            }
        }
        j = m;
    }
    ਨਹੀਂਤਾਂ ਜੇ( vlanes_f32 * (ਪੂਰਨ)ਆਕਾਰ(ਦਸ਼ਮਲਵ) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ਅਚਲ ਦਸ਼ਮਲਵ* c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;
        ਲਈ( ; j <= m - vlanes_f32; j += vlanes_f32, c_ਅੰਕੜੇ_2 += vlanes_f32 * c_step1 )
        {
            ਅਚਲ ਦਸ਼ਮਲਵ* b = b_ਅੰਕੜੇ_2 + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            ਲਈ( ਪੂਰਨ k = 0; k < n; k++, b += b_ਕਦਮ )
            {
                v_float64 va = vx_setall_f64((ਦੁੱਗਣਾ)a_ਅੰਕੜੇ[k]);
                v_float32 vb = vx_ਲੱਦੋ(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_ਉੱਚਾ(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            ਜੇ( !c_ਅੰਕੜੇ )
            {
                v_store(d_ਅੰਕੜੇ + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ਨਹੀਂਤਾਂ ਜੇ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_ਲੱਦੋ(c_ਅੰਕੜੇ_2);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_ਉੱਚਾ(vc), vbeta, vs_hi);
                v_store(d_ਅੰਕੜੇ + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ਨਹੀਂਤਾਂ
            {
                CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) ਦਸ਼ਮਲਵ buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                ਲਈ( ਪੂਰਨ jj = 0; jj < vlanes_f32; jj++ )
                    d_ਅੰਕੜੇ[j + jj] = buf[jj] + c_ਅੰਕੜੇ_2[jj * c_step1] * (ਦਸ਼ਮਲਵ)beta;
            }
        }
    }
}
#ਜੇ_ਅੰਤ

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ST>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ simdGEMM_kj(
    ਅਚਲ ST* a_ਅੰਕੜੇ, ਅਚਲ ST* b_ਅੰਕੜੇ_2, size_t b_ਕਦਮ,
    ਅਚਲ ST* c_ਅੰਕੜੇ, size_t c_step1,
    ST* d_ਅੰਕੜੇ,
    ਪੂਰਨ n, ਪੂਰਨ m,
    ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta,
    ਪੂਰਨ& j)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ VecTraits<ST>::VT VecT;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VecT>::vlanes();

    ਜੇ( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ਅਚਲ ਪੂਰਨ max_m = 1600 / (ਪੂਰਨ)ਆਕਾਰ(ST);
        CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            ਲਈ( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            ਲਈ( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ਅਚਲ ST* b_ptr = b_ਅੰਕੜੇ_2;
            ਲਈ( ਪੂਰਨ k = 0; k < n; k++, b_ptr += b_ਕਦਮ )
            {
                VecT va = VecTraits<ST>::setall(a_ਅੰਕੜੇ[k]);
                j = 0;
                ਲਈ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_ਲੱਦੋ_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_ਅੰਕੜੇ[k];
                ਲਈ( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ਅਚਲ ST* c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;
            j = 0;
            ਜੇ( !c_ਅੰਕੜੇ )
            {
                ਲਈ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_ਲੱਦੋ_aligned(s_buf + j), valpha);
                    v_store(d_ਅੰਕੜੇ + j, vs);
                }
                ਲਈ( ; j < m; j++ )
                    d_ਅੰਕੜੇ[j] = s_buf[j] * (ST)alpha;
            }
            ਨਹੀਂਤਾਂ ਜੇ( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                ਲਈ( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_ਲੱਦੋ_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_ਅੰਕੜੇ_2 + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_ਅੰਕੜੇ + j, vs);
                }
                ਲਈ( ; j < m; j++ )
                    d_ਅੰਕੜੇ[j] = s_buf[j] * (ST)alpha + c_ਅੰਕੜੇ_2[j] * (ST)beta;
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ( j = 0; j < m; j++, c_ਅੰਕੜੇ_2 += c_step1 )
                    d_ਅੰਕੜੇ[j] = s_buf[j] * (ST)alpha + c_ਅੰਕੜੇ_2[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    ਨਹੀਂਤਾਂ ਜੇ( vlanes * (ਪੂਰਨ)ਆਕਾਰ(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ਅਚਲ ST* c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;
        ਲਈ( ; j <= m - vlanes; j += vlanes, c_ਅੰਕੜੇ_2 += vlanes * c_step1 )
        {
            ਅਚਲ ST* b = b_ਅੰਕੜੇ_2 + j;
            VecT vs = VecTraits<ST>::zero();
            ਲਈ( ਪੂਰਨ k = 0; k < n; k++, b += b_ਕਦਮ )
            {
                VecT va = VecTraits<ST>::setall(a_ਅੰਕੜੇ[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            ਜੇ( !c_ਅੰਕੜੇ )
            {
                v_store(d_ਅੰਕੜੇ + j, vs);
            }
            ਨਹੀਂਤਾਂ ਜੇ( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_ਅੰਕੜੇ_2);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_ਅੰਕੜੇ + j, vs);
            }
            ਨਹੀਂਤਾਂ
            {
                CV_DECL_ALIGNED(CV_SIMD_ਚੌੜਾਈ) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                ਲਈ( ਪੂਰਨ jj = 0; jj < vlanes; jj++ )
                    d_ਅੰਕੜੇ[j + jj] = buf[jj] + c_ਅੰਕੜੇ_2[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#ਜੇ_ਅੰਤ


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT> ਸਥਿਰ ਖਾਲੀ
GEMMSingleMul( ਅਚਲ T* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
               ਅਚਲ T* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
               ਅਚਲ T* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
               T* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
               ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ,
               ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    ਪੂਰਨ i, j, k, n = a_ਮਾਪ.ਚੌੜਾਈ, m = d_ਮਾਪ.ਚੌੜਾਈ, drows = d_ਮਾਪ.ਉਚਾਈ;
    ਅਚਲ T *a_ਅੰਕੜੇ_2 = a_ਅੰਕੜੇ, *b_ਅੰਕੜੇ_2 = b_ਅੰਕੜੇ, *c_ਅੰਕੜੇ = c_ਅੰਕੜੇ_2;
    cv::Auto_ਬਫਰ<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_ਕਦਮ;

    a_ਕਦਮ /= ਆਕਾਰ(a_ਅੰਕੜੇ[0]);
    b_ਕਦਮ /= ਆਕਾਰ(b_ਅੰਕੜੇ[0]);
    c_ਕਦਮ /= ਆਕਾਰ(c_ਅੰਕੜੇ_2[0]);
    d_ਕਦਮ /= ਆਕਾਰ(d_ਅੰਕੜੇ[0]);
    a_step0 = a_ਕਦਮ;
    a_step1 = 1;

    ਜੇ( !c_ਅੰਕੜੇ_2 )
        c_step0 = c_step1 = 0;
    ਨਹੀਂਤਾਂ ਜੇ( !(flags & GEMM_3_T) )
        c_step0 = c_ਕਦਮ, c_step1 = 1;
    ਨਹੀਂਤਾਂ
        c_step0 = 1, c_step1 = c_ਕਦਮ;

    ਜੇ( flags & GEMM_1_T )
    {
        CV_ਅਦਲਾ_ਬਦਲੀ( a_step0, a_step1, t_ਕਦਮ );
        n = a_ਮਾਪ.ਉਚਾਈ;
        ਜੇ( a_ਕਦਮ > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.ਅੰਕੜੇ();
        }
    }

    ਜੇ( n == 1 ) /* external product */
    {
        cv::Auto_ਬਫਰ<T> _b_buf;
        T* b_buf = 0;

        ਜੇ( a_ਕਦਮ > 1 && a_ਮਾਪ.ਉਚਾਈ > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.ਅੰਕੜੇ();
            ਲਈ( k = 0; k < drows; k++ )
                a_buf[k] = a_ਅੰਕੜੇ[a_ਕਦਮ*k];
            a_ਅੰਕੜੇ = a_buf;
        }

        ਜੇ( b_ਕਦਮ > 1 )
        {
            _b_buf.allocate(d_ਮਾਪ.ਚੌੜਾਈ);
            b_buf = _b_buf.ਅੰਕੜੇ();
            ਲਈ( j = 0; j < d_ਮਾਪ.ਚੌੜਾਈ; j++ )
                b_buf[j] = b_ਅੰਕੜੇ[j*b_ਕਦਮ];
            b_ਅੰਕੜੇ = b_buf;
        }

        ਲਈ( i = 0; i < drows; i++, c_ਅੰਕੜੇ += c_step0, d_ਅੰਕੜੇ += d_ਕਦਮ )
        {
            WT al = WT(a_ਅੰਕੜੇ[i])*alpha;
            c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;
            ਲਈ( j = 0; j <= d_ਮਾਪ.ਚੌੜਾਈ - 2; j += 2, c_ਅੰਕੜੇ_2 += 2*c_step1 )
            {
                WT s0 = al*WT(b_ਅੰਕੜੇ[j]);
                WT s1 = al*WT(b_ਅੰਕੜੇ[j+1]);
                ਜੇ( !c_ਅੰਕੜੇ_2 )
                {
                    d_ਅੰਕੜੇ[j] = T(s0);
                    d_ਅੰਕੜੇ[j+1] = T(s1);
                }
                ਨਹੀਂਤਾਂ
                {
                    d_ਅੰਕੜੇ[j] = T(s0 + WT(c_ਅੰਕੜੇ_2[0])*beta);
                    d_ਅੰਕੜੇ[j+1] = T(s1 + WT(c_ਅੰਕੜੇ_2[c_step1])*beta);
                }
            }

            ਲਈ( ; j < d_ਮਾਪ.ਚੌੜਾਈ; j++, c_ਅੰਕੜੇ_2 += c_step1 )
            {
                WT s0 = al*WT(b_ਅੰਕੜੇ[j]);
                ਜੇ( !c_ਅੰਕੜੇ_2 )
                    d_ਅੰਕੜੇ[j] = T(s0);
                ਨਹੀਂਤਾਂ
                    d_ਅੰਕੜੇ[j] = T(s0 + WT(c_ਅੰਕੜੇ_2[0])*beta);
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( flags & GEMM_2_T ) /* A * Bt */
    {
        ਲਈ( i = 0; i < drows; i++, a_ਅੰਕੜੇ_2 += a_step0, c_ਅੰਕੜੇ += c_step0, d_ਅੰਕੜੇ += d_ਕਦਮ )
        {
            a_ਅੰਕੜੇ = a_ਅੰਕੜੇ_2;
            b_ਅੰਕੜੇ = b_ਅੰਕੜੇ_2;
            c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;

            ਜੇ( a_buf )
            {
                ਲਈ( k = 0; k < n; k++ )
                    a_buf[k] = a_ਅੰਕੜੇ[a_step1*k];
                a_ਅੰਕੜੇ = a_buf;
            }

            ਲਈ( j = 0; j < d_ਮਾਪ.ਚੌੜਾਈ; j++, b_ਅੰਕੜੇ += b_ਕਦਮ,
                                               c_ਅੰਕੜੇ_2 += c_step1 )
            {
                WT s0(0);
                k = 0;
#ਜੇ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ਜੇ( ਆਕਾਰ(WT) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
                {
                    ਜੇ( ਆਕਾਰ(T) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
                        s0 = (WT)simd_Dot_ਗੁਣਨਫਲ((ਅਚਲ ਦੁੱਗਣਾ*)a_ਅੰਕੜੇ, (ਅਚਲ ਦੁੱਗਣਾ*)b_ਅੰਕੜੇ, n, k);
                    ਨਹੀਂਤਾਂ ਜੇ( ਆਕਾਰ(T) == ਆਕਾਰ(ਦਸ਼ਮਲਵ) )
                        s0 = (WT)simd_Dot_ਗੁਣਨਫਲ_f32f64((ਅਚਲ ਦਸ਼ਮਲਵ*)a_ਅੰਕੜੇ, (ਅਚਲ ਦਸ਼ਮਲਵ*)b_ਅੰਕੜੇ, n, k);
                }
                ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
                {
                    WT s1(0), s2(0), s3(0);
                     #ਜੇ CV_ENABLE_UNROLLED
                    ਲਈ( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_ਅੰਕੜੇ[k])*WT(b_ਅੰਕੜੇ[k]);
                        s1 += WT(a_ਅੰਕੜੇ[k+1])*WT(b_ਅੰਕੜੇ[k+1]);
                        s2 += WT(a_ਅੰਕੜੇ[k+2])*WT(b_ਅੰਕੜੇ[k+2]);
                        s3 += WT(a_ਅੰਕੜੇ[k+3])*WT(b_ਅੰਕੜੇ[k+3]);
                    }
                    #ਜੇ_ਅੰਤ
                    s0 += s1 + s2 + s3;
                }
                ਲਈ( ; k < n; k++ )
                    s0 += WT(a_ਅੰਕੜੇ[k])*WT(b_ਅੰਕੜੇ[k]);
                s0 = s0*alpha;

                ਜੇ( !c_ਅੰਕੜੇ_2 )
                    d_ਅੰਕੜੇ[j] = T(s0);
                ਨਹੀਂਤਾਂ
                    d_ਅੰਕੜੇ[j] = T(s0 + WT(c_ਅੰਕੜੇ_2[0])*beta);
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( d_ਮਾਪ.ਚੌੜਾਈ*ਆਕਾਰ(d_ਅੰਕੜੇ[0]) <= 1600 )
    {
        ਲਈ( i = 0; i < drows; i++, a_ਅੰਕੜੇ_2 += a_step0,
                                    c_ਅੰਕੜੇ += c_step0,
                                    d_ਅੰਕੜੇ += d_ਕਦਮ )
        {
            a_ਅੰਕੜੇ = a_ਅੰਕੜੇ_2, c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;

            ਜੇ( a_buf )
            {
                ਲਈ( k = 0; k < n; k++ )
                    a_buf[k] = a_ਅੰਕੜੇ[a_step1*k];
                a_ਅੰਕੜੇ = a_buf;
            }

            j = 0;
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ਜੇ( ਆਕਾਰ(WT) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
            {
                ਜੇ( ਆਕਾਰ(T) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
                    simdGEMM_kj((ਅਚਲ ਦੁੱਗਣਾ*)a_ਅੰਕੜੇ, (ਅਚਲ ਦੁੱਗਣਾ*)b_ਅੰਕੜੇ_2, b_ਕਦਮ,
                                        (ਅਚਲ ਦੁੱਗਣਾ*)c_ਅੰਕੜੇ, c_step1,
                                (ਦੁੱਗਣਾ*)d_ਅੰਕੜੇ, n, m, alpha, beta, j);
                ਨਹੀਂਤਾਂ ਜੇ( ਆਕਾਰ(T) == ਆਕਾਰ(ਦਸ਼ਮਲਵ) )
                    simdGEMM_kj_f32f64((ਅਚਲ ਦਸ਼ਮਲਵ*)a_ਅੰਕੜੇ, (ਅਚਲ ਦਸ਼ਮਲਵ*)b_ਅੰਕੜੇ_2, b_ਕਦਮ,
                                        (ਅਚਲ ਦਸ਼ਮਲਵ*)c_ਅੰਕੜੇ, c_step1,
                                (ਦਸ਼ਮਲਵ*)d_ਅੰਕੜੇ, n, m, alpha, beta, j);
            }
#ਜੇ_ਅੰਤ
            c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            ਲਈ( ; j <= m - 4; j += 4, c_ਅੰਕੜੇ_2 += 4*c_step1 )
            {
                ਅਚਲ T* b = b_ਅੰਕੜੇ_2 + j;
                WT s0(0), s1(0), s2(0), s3(0);

                ਲਈ( k = 0; k < n; k++, b += b_ਕਦਮ )
                {
                    WT a(a_ਅੰਕੜੇ[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                ਜੇ( !c_ਅੰਕੜੇ_2 )
                {
                    d_ਅੰਕੜੇ[j] = T(s0*alpha);
                    d_ਅੰਕੜੇ[j+1] = T(s1*alpha);
                    d_ਅੰਕੜੇ[j+2] = T(s2*alpha);
                    d_ਅੰਕੜੇ[j+3] = T(s3*alpha);
                }
                ਨਹੀਂਤਾਂ
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_ਅੰਕੜੇ[j] = T(s0 + WT(c_ਅੰਕੜੇ_2[0])*beta);
                    d_ਅੰਕੜੇ[j+1] = T(s1 + WT(c_ਅੰਕੜੇ_2[c_step1])*beta);
                    d_ਅੰਕੜੇ[j+2] = T(s2 + WT(c_ਅੰਕੜੇ_2[c_step1*2])*beta);
                    d_ਅੰਕੜੇ[j+3] = T(s3 + WT(c_ਅੰਕੜੇ_2[c_step1*3])*beta);
                }
            }

            ਲਈ( ; j < m; j++, c_ਅੰਕੜੇ_2 += c_step1 )
            {
                ਅਚਲ T* b = b_ਅੰਕੜੇ_2 + j;
                WT s0(0);

                ਲਈ( k = 0; k < n; k++, b += b_ਕਦਮ )
                    s0 += WT(a_ਅੰਕੜੇ[k]) * WT(b[0]);

                s0 = s0*alpha;
                ਜੇ( !c_ਅੰਕੜੇ_2 )
                    d_ਅੰਕੜੇ[j] = T(s0);
                ਨਹੀਂਤਾਂ
                    d_ਅੰਕੜੇ[j] = T(s0 + WT(c_ਅੰਕੜੇ_2[0])*beta);
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        cv::Auto_ਬਫਰ<WT> _d_buf(m);
        WT* d_buf = _d_buf.ਅੰਕੜੇ();

        ਲਈ( i = 0; i < drows; i++, a_ਅੰਕੜੇ_2 += a_step0, c_ਅੰਕੜੇ += c_step0, d_ਅੰਕੜੇ += d_ਕਦਮ )
        {
            a_ਅੰਕੜੇ = a_ਅੰਕੜੇ_2;
            b_ਅੰਕੜੇ = b_ਅੰਕੜੇ_2;
            c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;

            ਜੇ( a_buf )
            {
                ਲਈ( k = 0; k < n; k++ )
                    a_buf[k] = a_ਅੰਕੜੇ_2[a_step1*k];
                a_ਅੰਕੜੇ = a_buf;
            }

            ਲਈ( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            ਲਈ( k = 0; k < n; k++, b_ਅੰਕੜੇ += b_ਕਦਮ )
            {
                WT al(a_ਅੰਕੜੇ[k]);
                j=0;
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                ਜੇ( ਆਕਾਰ(WT) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
                {
                    ਜੇ( ਆਕਾਰ(T) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
                    {
                        ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ਅਚਲ ਦੁੱਗਣਾ*)&al));
                        ਲਈ( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_ਲੱਦੋ((ਅਚਲ ਦੁੱਗਣਾ*)(d_buf + j));
                            v_float64 vb = vx_ਲੱਦੋ((ਅਚਲ ਦੁੱਗਣਾ*)(b_ਅੰਕੜੇ + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((ਦੁੱਗਣਾ*)(d_buf + j), vd);
                        }
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        ਅਚਲ ਪੂਰਨ vlanes_f32 = VTraits<v_float32>::vlanes();
                        ਅਚਲ ਪੂਰਨ vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ਅਚਲ ਦੁੱਗਣਾ*)&al));
                        ਲਈ( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_ਲੱਦੋ((ਅਚਲ ਦਸ਼ਮਲਵ*)(b_ਅੰਕੜੇ + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_ਉੱਚਾ(vb);
                            v_float64 vd_lo = vx_ਲੱਦੋ((ਅਚਲ ਦੁੱਗਣਾ*)(d_buf + j));
                            v_float64 vd_hi = vx_ਲੱਦੋ((ਅਚਲ ਦੁੱਗਣਾ*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((ਦੁੱਗਣਾ*)(d_buf + j), vd_lo);
                            v_store((ਦੁੱਗਣਾ*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#ਜੇ_ਅੰਤ
                ਲਈ( ; j < m; j++ )
                    d_buf[j] += WT(b_ਅੰਕੜੇ[j])*al;
            }

            ਜੇ( !c_ਅੰਕੜੇ_2 )
                ਲਈ( j = 0; j < m; j++ )
                    d_ਅੰਕੜੇ[j] = T(d_buf[j]*alpha);
            ਨਹੀਂਤਾਂ
                ਲਈ( j = 0; j < m; j++, c_ਅੰਕੜੇ_2 += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_ਅੰਕੜੇ[j] = T(t + WT(c_ਅੰਕੜੇ_2[0])*beta);
                }
        }
    }
}
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ WT>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_Double_ਅੰਕੜੇ_Zero(WT* d_ਅੰਕੜੇ, ਪੂਰਨ m, ਪੂਰਨ vlanes)
{
    v_float64 vz = vx_setzero_f64();
    ਪੂਰਨ j = 0;
    ਲਈ( ; j <= m - vlanes; j += vlanes )
        v_store((ਦੁੱਗਣਾ*)(d_ਅੰਕੜੇ + j), vz);
    ਲਈ( ; j < m; j++ )
        d_ਅੰਕੜੇ[j] = WT(0);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ST>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ simd_Mul_ਜੋੜੋ(ਅਚਲ ST* b, ਦੁੱਗਣਾ* d, ਪੂਰਨ m, ਦੁੱਗਣਾ a_val);

ਸਾਂਚਾ<>
ਸਤਰਅੰਦਰ ਖਾਲੀ simd_Mul_ਜੋੜੋ<ਦਸ਼ਮਲਵ>(
    ਅਚਲ ਦਸ਼ਮਲਵ* b, ਦੁੱਗਣਾ* d, ਪੂਰਨ m, ਦੁੱਗਣਾ a_val)
{
    ਅਚਲ ਪੂਰਨ vlanes_f32 = VTraits<v_float32>::vlanes();
    ਅਚਲ ਪੂਰਨ vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    ਪੂਰਨ j = 0;
    ਲਈ( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_ਲੱਦੋ(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_ਉੱਚਾ(vb);
        v_float64 vd_lo = vx_ਲੱਦੋ(d + j);
        v_float64 vd_hi = vx_ਲੱਦੋ(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    ਲਈ( ; j < m; j++ )
        d[j] += a_val * (ਦੁੱਗਣਾ)b[j];
}

ਸਾਂਚਾ<>
ਸਤਰਅੰਦਰ ਖਾਲੀ simd_Mul_ਜੋੜੋ<ਦੁੱਗਣਾ>(
    ਅਚਲ ਦੁੱਗਣਾ* b, ਦੁੱਗਣਾ* d, ਪੂਰਨ m, ਦੁੱਗਣਾ a_val)
{
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_float64>::vlanes();
    ਅਚਲ ਪੂਰਨ stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    ਪੂਰਨ j = 0;
    ਲਈ( ; j <= m - stride2; j += stride2 )
    {
        v_float64 vd0 = vx_ਲੱਦੋ(d + j);
        v_float64 vb0 = vx_ਲੱਦੋ(b + j);
        v_float64 vd1 = vx_ਲੱਦੋ(d + j + vlanes);
        v_float64 vb1 = vx_ਲੱਦੋ(b + j + vlanes);
        vd0 = v_muladd(va, vb0, vd0);
        vd1 = v_muladd(va, vb1, vd1);
        v_store(d + j, vd0);
        v_store(d + j + vlanes, vd1);
    }
    ਲਈ( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_ਲੱਦੋ(d + j);
        v_float64 vb = vx_ਲੱਦੋ(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    ਲਈ( ; j < m; j++ )
        d[j] += a_val * b[j];
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ ST>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ simd_ਖੰਡ_Mul_kj(
    ਅਚਲ ST* a_ਅੰਕੜੇ, ਅਚਲ ST* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
    ਦੁੱਗਣਾ* d_ਅੰਕੜੇ, ਪੂਰਨ n, ਪੂਰਨ m, ਤਾਰਕਿਕ do_acc)
{
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_float64>::vlanes();
    ਜੇ( !do_acc )
        ਸੈੱਟ_Double_ਅੰਕੜੇ_Zero(d_ਅੰਕੜੇ, m, vlanes);
    ਲਈ( ਪੂਰਨ k = 0; k < n; k++, b_ਅੰਕੜੇ += b_ਕਦਮ )
        simd_Mul_ਜੋੜੋ(b_ਅੰਕੜੇ, d_ਅੰਕੜੇ, m, (ਦੁੱਗਣਾ)a_ਅੰਕੜੇ[k]);
}
#ਜੇ_ਅੰਤ // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT> ਸਥਿਰ ਖਾਲੀ
GEMMBlockMul( ਅਚਲ T* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
              ਅਚਲ T* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
              WT* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
              ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ, ਪੂਰਨ flags )
{
    ਪੂਰਨ i, j, k, n = a_ਮਾਪ.ਚੌੜਾਈ, m = d_ਮਾਪ.ਚੌੜਾਈ;
    ਅਚਲ T *a_ਅੰਕੜੇ_2 = a_ਅੰਕੜੇ, *b_ਅੰਕੜੇ_2 = b_ਅੰਕੜੇ;
    cv::Auto_ਬਫਰ<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_ਕਦਮ;
    ਪੂਰਨ do_acc = flags & 16;

    a_ਕਦਮ /= ਆਕਾਰ(a_ਅੰਕੜੇ[0]);
    b_ਕਦਮ /= ਆਕਾਰ(b_ਅੰਕੜੇ[0]);
    d_ਕਦਮ /= ਆਕਾਰ(d_ਅੰਕੜੇ[0]);

    a_step0 = a_ਕਦਮ;
    a_step1 = 1;

    ਜੇ( flags & GEMM_1_T )
    {
        CV_ਅਦਲਾ_ਬਦਲੀ( a_step0, a_step1, t_ਕਦਮ );
        n = a_ਮਾਪ.ਉਚਾਈ;
        _a_buf.allocate(n);
        a_buf = _a_buf.ਅੰਕੜੇ();
    }

    ਜੇ( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i++, a_ਅੰਕੜੇ_2 += a_step0, d_ਅੰਕੜੇ += d_ਕਦਮ )
        {
            a_ਅੰਕੜੇ = a_ਅੰਕੜੇ_2; b_ਅੰਕੜੇ = b_ਅੰਕੜੇ_2;

            ਜੇ( a_buf )
            {
                ਲਈ( k = 0; k < n; k++ )
                    a_buf[k] = a_ਅੰਕੜੇ[a_step1*k];
                a_ਅੰਕੜੇ = a_buf;
            }

            ਲਈ( j = 0; j < d_ਮਾਪ.ਚੌੜਾਈ; j++, b_ਅੰਕੜੇ += b_ਕਦਮ )
            {
                WT s0 = do_acc ? d_ਅੰਕੜੇ[j] : WT(0);
                k = 0;
#ਜੇ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ਜੇ( ਆਕਾਰ(WT) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
                {
                    ਜੇ( ਆਕਾਰ(T) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
                        s0 += (WT)simd_Dot_ਗੁਣਨਫਲ((ਅਚਲ ਦੁੱਗਣਾ*)a_ਅੰਕੜੇ, (ਅਚਲ ਦੁੱਗਣਾ*)b_ਅੰਕੜੇ, n, k);
                    ਨਹੀਂਤਾਂ ਜੇ( ਆਕਾਰ(T) == ਆਕਾਰ(ਦਸ਼ਮਲਵ) )
                        s0 += (WT)simd_Dot_ਗੁਣਨਫਲ_f32f64((ਅਚਲ ਦਸ਼ਮਲਵ*)a_ਅੰਕੜੇ, (ਅਚਲ ਦਸ਼ਮਲਵ*)b_ਅੰਕੜੇ, n, k);
                }
                ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
                {
                    WT s1(0);
                    ਲਈ( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_ਅੰਕੜੇ[k])*WT(b_ਅੰਕੜੇ[k]);
                        s1 += WT(a_ਅੰਕੜੇ[k+1])*WT(b_ਅੰਕੜੇ[k+1]);
                    }
                    s0 += s1;
                }
                ਲਈ( ; k < n; k++ )
                    s0 += WT(a_ਅੰਕੜੇ[k])*WT(b_ਅੰਕੜੇ[k]);

                d_ਅੰਕੜੇ[j] = s0;
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i++, a_ਅੰਕੜੇ_2 += a_step0, d_ਅੰਕੜੇ += d_ਕਦਮ )
        {
            a_ਅੰਕੜੇ = a_ਅੰਕੜੇ_2, b_ਅੰਕੜੇ = b_ਅੰਕੜੇ_2;

            ਜੇ( a_buf )
            {
                ਲਈ( k = 0; k < n; k++ )
                    a_buf[k] = a_ਅੰਕੜੇ[a_step1*k];
                a_ਅੰਕੜੇ = a_buf;
            }
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ਜੇ( ਆਕਾਰ(WT) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
            {
                ਜੇ( ਆਕਾਰ(T) == ਆਕਾਰ(ਦੁੱਗਣਾ) )
                    simd_ਖੰਡ_Mul_kj(
                        (ਅਚਲ ਦੁੱਗਣਾ*)a_ਅੰਕੜੇ, (ਅਚਲ ਦੁੱਗਣਾ*)b_ਅੰਕੜੇ, b_ਕਦਮ,
                        (ਦੁੱਗਣਾ*)d_ਅੰਕੜੇ, n, m, do_acc != 0);
                ਨਹੀਂਤਾਂ
                    simd_ਖੰਡ_Mul_kj(
                        (ਅਚਲ ਦਸ਼ਮਲਵ*)a_ਅੰਕੜੇ, (ਅਚਲ ਦਸ਼ਮਲਵ*)b_ਅੰਕੜੇ, b_ਕਦਮ,
                        (ਦੁੱਗਣਾ*)d_ਅੰਕੜੇ, n, m, do_acc != 0);
            }
            ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
            {
                ਲਈ( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ਅਚਲ T* b = b_ਅੰਕੜੇ + j;

                    ਜੇ( do_acc )
                    {
                        s0 = d_ਅੰਕੜੇ[j]; s1 = d_ਅੰਕੜੇ[j+1];
                        s2 = d_ਅੰਕੜੇ[j+2]; s3 = d_ਅੰਕੜੇ[j+3];
                    }
                    ਨਹੀਂਤਾਂ
                        s0 = s1 = s2 = s3 = WT(0);

                    ਲਈ( k = 0; k < n; k++, b += b_ਕਦਮ )
                    {
                        WT a(a_ਅੰਕੜੇ[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_ਅੰਕੜੇ[j] = s0; d_ਅੰਕੜੇ[j+1] = s1;
                    d_ਅੰਕੜੇ[j+2] = s2; d_ਅੰਕੜੇ[j+3] = s3;
                }

                ਲਈ( ; j < m; j++ )
                {
                    ਅਚਲ T* b = b_ਅੰਕੜੇ + j;
                    WT s0 = do_acc ? d_ਅੰਕੜੇ[j] : WT(0);

                    ਲਈ( k = 0; k < n; k++, b += b_ਕਦਮ )
                        s0 += WT(a_ਅੰਕੜੇ[k]) * WT(b[0]);

                    d_ਅੰਕੜੇ[j] = s0;
                }
            }
        }
    }
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT> ਸਥਿਰ ਖਾਲੀ
GEMMStore( ਅਚਲ T* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
           ਅਚਲ WT* d_buf, size_t d_buf_ਕਦਮ,
           T* d_ਅੰਕੜੇ, size_t d_ਕਦਮ, ਮਾਪ d_ਮਾਪ,
           ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    ਅਚਲ T* c_ਅੰਕੜੇ = c_ਅੰਕੜੇ_2;
    ਪੂਰਨ j;
    size_t c_step0, c_step1;

    c_ਕਦਮ /= ਆਕਾਰ(c_ਅੰਕੜੇ_2[0]);
    d_buf_ਕਦਮ /= ਆਕਾਰ(d_buf[0]);
    d_ਕਦਮ /= ਆਕਾਰ(d_ਅੰਕੜੇ[0]);

    ਜੇ( !c_ਅੰਕੜੇ_2 )
        c_step0 = c_step1 = 0;
    ਨਹੀਂਤਾਂ ਜੇ( !(flags & GEMM_3_T) )
        c_step0 = c_ਕਦਮ, c_step1 = 1;
    ਨਹੀਂਤਾਂ
        c_step0 = 1, c_step1 = c_ਕਦਮ;

    ਲਈ( ; d_ਮਾਪ.ਉਚਾਈ--; c_ਅੰਕੜੇ += c_step0, d_buf += d_buf_ਕਦਮ, d_ਅੰਕੜੇ += d_ਕਦਮ )
    {
        ਜੇ( c_ਅੰਕੜੇ )
        {
            c_ਅੰਕੜੇ_2 = c_ਅੰਕੜੇ;
            j=0;
             #ਜੇ CV_ENABLE_UNROLLED
            ਲਈ(; j <= d_ਮਾਪ.ਚੌੜਾਈ - 4; j += 4, c_ਅੰਕੜੇ_2 += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_ਅੰਕੜੇ_2[0]);
                t1 += beta*WT(c_ਅੰਕੜੇ_2[c_step1]);
                d_ਅੰਕੜੇ[j] = T(t0);
                d_ਅੰਕੜੇ[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_ਅੰਕੜੇ_2[c_step1*2]);
                t1 += beta*WT(c_ਅੰਕੜੇ_2[c_step1*3]);
                d_ਅੰਕੜੇ[j+2] = T(t0);
                d_ਅੰਕੜੇ[j+3] = T(t1);
            }
            #ਜੇ_ਅੰਤ
            ਲਈ( ; j < d_ਮਾਪ.ਚੌੜਾਈ; j++, c_ਅੰਕੜੇ_2 += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_ਅੰਕੜੇ[j] = T(t0 + WT(c_ਅੰਕੜੇ_2[0])*beta);
            }
        }
        ਨਹੀਂਤਾਂ
        {
            j = 0;
             #ਜੇ CV_ENABLE_UNROLLED
            ਲਈ( ; j <= d_ਮਾਪ.ਚੌੜਾਈ - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_ਅੰਕੜੇ[j] = T(t0);
                d_ਅੰਕੜੇ[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_ਅੰਕੜੇ[j+2] = T(t0);
                d_ਅੰਕੜੇ[j+3] = T(t1);
            }
            #ਜੇ_ਅੰਤ
            ਲਈ( ; j < d_ਮਾਪ.ਚੌੜਾਈ; j++ )
                d_ਅੰਕੜੇ[j] = T(alpha*d_buf[j]);
        }
    }
}


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*GEMMSingleMulFunc)( ਅਚਲ ਖਾਲੀ* src1, size_t step1,
                   ਅਚਲ ਖਾਲੀ* src2, size_t step2, ਅਚਲ ਖਾਲੀ* src3, size_t step3,
                   ਖਾਲੀ* dst, size_t dststep, ਮਾਪ srcsize, ਮਾਪ dstsize,
                   ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags );

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*GEMMBlockMulFunc)( ਅਚਲ ਖਾਲੀ* src1, size_t step1,
                   ਅਚਲ ਖਾਲੀ* src2, size_t step2, ਖਾਲੀ* dst, size_t dststep,
                   ਮਾਪ srcsize, ਮਾਪ dstsize, ਪੂਰਨ flags );

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*GEMMStoreFunc)( ਅਚਲ ਖਾਲੀ* src1, size_t step1,
                   ਅਚਲ ਖਾਲੀ* src2, size_t step2, ਖਾਲੀ* dst, size_t dststep,
                   ਮਾਪ dstsize, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags );

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT, ਖਾਲੀ (*fn)(ਅਚਲ T*, size_t, ਅਚਲ T*, size_t, ਅਚਲ T*, size_t, T*, size_t, ਮਾਪ, ਮਾਪ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਪੂਰਨ)>
ਸਥਿਰ ਖਾਲੀ gemmSingleMulWrap( ਅਚਲ ਖਾਲੀ* src1, size_t step1,
                   ਅਚਲ ਖਾਲੀ* src2, size_t step2, ਅਚਲ ਖਾਲੀ* src3, size_t step3,
                   ਖਾਲੀ* dst, size_t dststep, ਮਾਪ srcsize, ਮਾਪ dstsize,
                   ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    fn((ਅਚਲ T*)src1, step1, (ਅਚਲ T*)src2, step2, (ਅਚਲ T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT, ਖਾਲੀ (*fn)(ਅਚਲ T*, size_t, ਅਚਲ T*, size_t, WT*, size_t, ਮਾਪ, ਮਾਪ, ਪੂਰਨ)>
ਸਥਿਰ ਖਾਲੀ gemm_ਖੰਡ_Mul_Wrap( ਅਚਲ ਖਾਲੀ* src1, size_t step1,
                   ਅਚਲ ਖਾਲੀ* src2, size_t step2, ਖਾਲੀ* dst, size_t dststep,
                   ਮਾਪ srcsize, ਮਾਪ dstsize, ਪੂਰਨ flags )
{
    fn((ਅਚਲ T*)src1, step1, (ਅਚਲ T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT, ਖਾਲੀ (*fn)(ਅਚਲ T*, size_t, ਅਚਲ WT*, size_t, T*, size_t, ਮਾਪ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਪੂਰਨ)>
ਸਥਿਰ ਖਾਲੀ gemmStoreWrap( ਅਚਲ ਖਾਲੀ* src1, size_t step1,
                   ਅਚਲ ਖਾਲੀ* src2, size_t step2, ਖਾਲੀ* dst, size_t dststep,
                   ਮਾਪ dstsize, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    fn((ਅਚਲ T*)src1, step1, (ਅਚਲ WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

ਸਥਿਰ ਖਾਲੀ GEMMSingleMul_32f( ਅਚਲ ਦਸ਼ਮਲਵ* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
              ਅਚਲ ਦਸ਼ਮਲਵ* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
              ਅਚਲ ਦਸ਼ਮਲਵ* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
              ਦਸ਼ਮਲਵ* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
              ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ,
              ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    GEMMSingleMul<ਦਸ਼ਮਲਵ,ਦੁੱਗਣਾ>(a_ਅੰਕੜੇ, a_ਕਦਮ, b_ਅੰਕੜੇ, b_ਕਦਮ, c_ਅੰਕੜੇ_2,
                                c_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ,
                                alpha, beta, flags);
}

ਸਥਿਰ ਖਾਲੀ GEMMSingleMul_64f( ਅਚਲ ਦੁੱਗਣਾ* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
                              ਅਚਲ ਦੁੱਗਣਾ* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
                              ਅਚਲ ਦੁੱਗਣਾ* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
                              ਦੁੱਗਣਾ* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
                              ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ,
                              ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    GEMMSingleMul<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ>(a_ਅੰਕੜੇ, a_ਕਦਮ, b_ਅੰਕੜੇ, b_ਕਦਮ, c_ਅੰਕੜੇ_2,
                                c_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ,
                                alpha, beta, flags);
}


ਸਥਿਰ ਖਾਲੀ GEMMSingleMul_32fc( ਅਚਲ Complexf* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
                              ਅਚਲ Complexf* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
                              ਅਚਲ Complexf* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
                              Complexf* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
                              ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ,
                              ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_ਅੰਕੜੇ, a_ਕਦਮ, b_ਅੰਕੜੇ, b_ਕਦਮ, c_ਅੰਕੜੇ_2,
                                c_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ,
                                alpha, beta, flags);
}

ਸਥਿਰ ਖਾਲੀ GEMMSingleMul_64fc( ਅਚਲ Complexd* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
                              ਅਚਲ Complexd* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
                              ਅਚਲ Complexd* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
                              Complexd* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
                              ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ,
                              ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_ਅੰਕੜੇ, a_ਕਦਮ, b_ਅੰਕੜੇ, b_ਕਦਮ, c_ਅੰਕੜੇ_2,
                                 c_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ,
                                 alpha, beta, flags);
}

ਸਥਿਰ ਖਾਲੀ GEMMBlockMul_32f( ਅਚਲ ਦਸ਼ਮਲਵ* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
             ਅਚਲ ਦਸ਼ਮਲਵ* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
             ਦੁੱਗਣਾ* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
             ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ, ਪੂਰਨ flags )
{
    GEMMBlockMul(a_ਅੰਕੜੇ, a_ਕਦਮ, b_ਅੰਕੜੇ, b_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ, flags);
}


ਸਥਿਰ ਖਾਲੀ GEMMBlockMul_64f( ਅਚਲ ਦੁੱਗਣਾ* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
                             ਅਚਲ ਦੁੱਗਣਾ* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
                             ਦੁੱਗਣਾ* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
                             ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ, ਪੂਰਨ flags )
{
    GEMMBlockMul(a_ਅੰਕੜੇ, a_ਕਦਮ, b_ਅੰਕੜੇ, b_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ, flags);
}


ਸਥਿਰ ਖਾਲੀ GEMMBlockMul_32fc( ਅਚਲ Complexf* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
                             ਅਚਲ Complexf* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
                             Complexd* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
                             ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ, ਪੂਰਨ flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_ਅੰਕੜੇ, a_ਕਦਮ, b_ਅੰਕੜੇ, b_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ, flags);
}


ਸਥਿਰ ਖਾਲੀ GEMMBlockMul_64fc( ਅਚਲ Complexd* a_ਅੰਕੜੇ, size_t a_ਕਦਮ,
                             ਅਚਲ Complexd* b_ਅੰਕੜੇ, size_t b_ਕਦਮ,
                             Complexd* d_ਅੰਕੜੇ, size_t d_ਕਦਮ,
                             ਮਾਪ a_ਮਾਪ, ਮਾਪ d_ਮਾਪ, ਪੂਰਨ flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_ਅੰਕੜੇ, a_ਕਦਮ, b_ਅੰਕੜੇ, b_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ, flags);
}


ਸਥਿਰ ਖਾਲੀ GEMMStore_32f( ਅਚਲ ਦਸ਼ਮਲਵ* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
          ਅਚਲ ਦੁੱਗਣਾ* d_buf, size_t d_buf_ਕਦਮ,
          ਦਸ਼ਮਲਵ* d_ਅੰਕੜੇ, size_t d_ਕਦਮ, ਮਾਪ d_ਮਾਪ,
          ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    GEMMStore(c_ਅੰਕੜੇ_2, c_ਕਦਮ, d_buf, d_buf_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, d_ਮਾਪ, alpha, beta, flags);
}


ਸਥਿਰ ਖਾਲੀ GEMMStore_64f( ਅਚਲ ਦੁੱਗਣਾ* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
                      ਅਚਲ ਦੁੱਗਣਾ* d_buf, size_t d_buf_ਕਦਮ,
                      ਦੁੱਗਣਾ* d_ਅੰਕੜੇ, size_t d_ਕਦਮ, ਮਾਪ d_ਮਾਪ,
                      ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    GEMMStore(c_ਅੰਕੜੇ_2, c_ਕਦਮ, d_buf, d_buf_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, d_ਮਾਪ, alpha, beta, flags);
}


ਸਥਿਰ ਖਾਲੀ GEMMStore_32fc( ਅਚਲ Complexf* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
                          ਅਚਲ Complexd* d_buf, size_t d_buf_ਕਦਮ,
                          Complexf* d_ਅੰਕੜੇ, size_t d_ਕਦਮ, ਮਾਪ d_ਮਾਪ,
                          ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    GEMMStore(c_ਅੰਕੜੇ_2, c_ਕਦਮ, d_buf, d_buf_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, d_ਮਾਪ, alpha, beta, flags);
}


ਸਥਿਰ ਖਾਲੀ GEMMStore_64fc( ਅਚਲ Complexd* c_ਅੰਕੜੇ_2, size_t c_ਕਦਮ,
                          ਅਚਲ Complexd* d_buf, size_t d_buf_ਕਦਮ,
                          Complexd* d_ਅੰਕੜੇ, size_t d_ਕਦਮ, ਮਾਪ d_ਮਾਪ,
                          ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਪੂਰਨ flags )
{
    GEMMStore(c_ਅੰਕੜੇ_2, c_ਕਦਮ, d_buf, d_buf_ਕਦਮ, d_ਅੰਕੜੇ, d_ਕਦਮ, d_ਮਾਪ, alpha, beta, flags);
}

ਸਥਿਰ ਖਾਲੀ gemmImpl( Mat A, Mat B, ਦੁੱਗਣਾ alpha,
           Mat C, ਦੁੱਗਣਾ beta, Mat D, ਪੂਰਨ flags )
{
    CV_INSTRUMENT_REGION();

    ਅਚਲ ਪੂਰਨ ਖੰਡ_lin_ਮਾਪ = 128;
    ਅਚਲ ਪੂਰਨ ਖੰਡ_ਮਾਪ_3 = ਖੰਡ_lin_ਮਾਪ * ਖੰਡ_lin_ਮਾਪ;

    ਸਥਿਰ ਦੁੱਗਣਾ zero[] = {0,0,0,0};
    ਸਥਿਰ ਦਸ਼ਮਲਵ zerof[] = {0,0,0,0};

    ਮਾਪ a_ਮਾਪ = A.size(), d_ਮਾਪ;
    ਪੂਰਨ i, len = 0, type = A.type();

    ਚੋਣ( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ਹਾਲਤ 0:
        d_ਮਾਪ = ਮਾਪ( B.ਖਾਨੇ_ਗਿਣਤੀ, a_ਮਾਪ.ਉਚਾਈ );
        len = B.ਪੰਕਤੀਆਂ;
        ਰੋਕੋ;
    ਹਾਲਤ 1:
        d_ਮਾਪ = ਮਾਪ( B.ਖਾਨੇ_ਗਿਣਤੀ, a_ਮਾਪ.ਚੌੜਾਈ );
        len = B.ਪੰਕਤੀਆਂ;
        ਰੋਕੋ;
    ਹਾਲਤ 2:
        d_ਮਾਪ = ਮਾਪ( B.ਪੰਕਤੀਆਂ, a_ਮਾਪ.ਉਚਾਈ );
        len = B.ਖਾਨੇ_ਗਿਣਤੀ;
        ਰੋਕੋ;
    ਹਾਲਤ 3:
        d_ਮਾਪ = ਮਾਪ( B.ਪੰਕਤੀਆਂ, a_ਮਾਪ.ਚੌੜਾਈ );
        len = B.ਖਾਨੇ_ਗਿਣਤੀ;
        ਰੋਕੋ;
    }

    ਜੇ( flags == 0 && 2 <= len && len <= 4 && (len == d_ਮਾਪ.ਚੌੜਾਈ || len == d_ਮਾਪ.ਉਚਾਈ) )
    {
        ਜੇ( type == CV_32F )
        {
            ਦਸ਼ਮਲਵ* d = D.ptr<ਦਸ਼ਮਲਵ>();
            ਅਚਲ ਦਸ਼ਮਲਵ *a = A.ptr<ਦਸ਼ਮਲਵ>(),
                        *b = B.ptr<ਦਸ਼ਮਲਵ>(),
                        *c = (ਅਚਲ ਦਸ਼ਮਲਵ*)C.ਅੰਕੜੇ;
            size_t d_ਕਦਮ = D.ਕਦਮ/ਆਕਾਰ(d[0]),
                a_ਕਦਮ = A.ਕਦਮ/ਆਕਾਰ(a[0]),
                b_ਕਦਮ = B.ਕਦਮ/ਆਕਾਰ(b[0]),
                c_ਕਦਮ = C.ਅੰਕੜੇ ? C.ਕਦਮ/ਆਕਾਰ(c[0]) : 0;

            ਜੇ( !c )
                c = zerof;

            ਚੋਣ( len )
            {
            ਹਾਲਤ 2:
                ਜੇ( len == d_ਮਾਪ.ਚੌੜਾਈ && b != d )
                {
                    ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i++, d += d_ਕਦਮ, a += a_ਕਦਮ, c += c_ਕਦਮ )
                    {
                        ਦਸ਼ਮਲਵ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ];
                        ਦਸ਼ਮਲਵ t1 = a[0]*b[1] + a[1]*b[b_ਕਦਮ+1];
                        d[0] = (ਦਸ਼ਮਲਵ)(t0*alpha + c[0]*beta);
                        d[1] = (ਦਸ਼ਮਲਵ)(t1*alpha + c[1]*beta);
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ( a != d )
                {
                    ਪੂਰਨ c_step0 = 1;
                    ਜੇ( c == zerof )
                    {
                        c_step0 = 0;
                        c_ਕਦਮ = 1;
                    }

                    ਲਈ( i = 0; i < d_ਮਾਪ.ਚੌੜਾਈ; i++, d++, b++, c += c_step0 )
                    {
                        ਦਸ਼ਮਲਵ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ];
                        ਦਸ਼ਮਲਵ t1 = a[a_ਕਦਮ]*b[0] + a[a_ਕਦਮ+1]*b[b_ਕਦਮ];
                        d[0] = (ਦਸ਼ਮਲਵ)(t0*alpha + c[0]*beta);
                        d[d_ਕਦਮ] = (ਦਸ਼ਮਲਵ)(t1*alpha + c[c_ਕਦਮ]*beta);
                    }
                }
                ਨਹੀਂਤਾਂ
                    ਰੋਕੋ;
                ਵਾਪਸ;
            ਹਾਲਤ 3:
                ਜੇ( len == d_ਮਾਪ.ਚੌੜਾਈ && b != d )
                {
                    ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i++, d += d_ਕਦਮ, a += a_ਕਦਮ, c += c_ਕਦਮ )
                    {
                        ਦਸ਼ਮਲਵ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ] + a[2]*b[b_ਕਦਮ*2];
                        ਦਸ਼ਮਲਵ t1 = a[0]*b[1] + a[1]*b[b_ਕਦਮ+1] + a[2]*b[b_ਕਦਮ*2+1];
                        ਦਸ਼ਮਲਵ t2 = a[0]*b[2] + a[1]*b[b_ਕਦਮ+2] + a[2]*b[b_ਕਦਮ*2+2];
                        d[0] = (ਦਸ਼ਮਲਵ)(t0*alpha + c[0]*beta);
                        d[1] = (ਦਸ਼ਮਲਵ)(t1*alpha + c[1]*beta);
                        d[2] = (ਦਸ਼ਮਲਵ)(t2*alpha + c[2]*beta);
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ( a != d )
                {
                    ਪੂਰਨ c_step0 = 1;
                    ਜੇ( c == zerof )
                    {
                        c_step0 = 0;
                        c_ਕਦਮ = 1;
                    }

                    ਲਈ( i = 0; i < d_ਮਾਪ.ਚੌੜਾਈ; i++, d++, b++, c += c_step0 )
                    {
                        ਦਸ਼ਮਲਵ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ] + a[2]*b[b_ਕਦਮ*2];
                        ਦਸ਼ਮਲਵ t1 = a[a_ਕਦਮ]*b[0] + a[a_ਕਦਮ+1]*b[b_ਕਦਮ] + a[a_ਕਦਮ+2]*b[b_ਕਦਮ*2];
                        ਦਸ਼ਮਲਵ t2 = a[a_ਕਦਮ*2]*b[0] + a[a_ਕਦਮ*2+1]*b[b_ਕਦਮ] + a[a_ਕਦਮ*2+2]*b[b_ਕਦਮ*2];

                        d[0] = (ਦਸ਼ਮਲਵ)(t0*alpha + c[0]*beta);
                        d[d_ਕਦਮ] = (ਦਸ਼ਮਲਵ)(t1*alpha + c[c_ਕਦਮ]*beta);
                        d[d_ਕਦਮ*2] = (ਦਸ਼ਮਲਵ)(t2*alpha + c[c_ਕਦਮ*2]*beta);
                    }
                }
                ਨਹੀਂਤਾਂ
                    ਰੋਕੋ;
                ਵਾਪਸ;
            ਹਾਲਤ 4:
                ਜੇ( len == d_ਮਾਪ.ਚੌੜਾਈ && b != d )
                {
                    ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i++, d += d_ਕਦਮ, a += a_ਕਦਮ, c += c_ਕਦਮ )
                    {
                        ਦਸ਼ਮਲਵ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ] + a[2]*b[b_ਕਦਮ*2] + a[3]*b[b_ਕਦਮ*3];
                        ਦਸ਼ਮਲਵ t1 = a[0]*b[1] + a[1]*b[b_ਕਦਮ+1] + a[2]*b[b_ਕਦਮ*2+1] + a[3]*b[b_ਕਦਮ*3+1];
                        ਦਸ਼ਮਲਵ t2 = a[0]*b[2] + a[1]*b[b_ਕਦਮ+2] + a[2]*b[b_ਕਦਮ*2+2] + a[3]*b[b_ਕਦਮ*3+2];
                        ਦਸ਼ਮਲਵ t3 = a[0]*b[3] + a[1]*b[b_ਕਦਮ+3] + a[2]*b[b_ਕਦਮ*2+3] + a[3]*b[b_ਕਦਮ*3+3];
                        d[0] = (ਦਸ਼ਮਲਵ)(t0*alpha + c[0]*beta);
                        d[1] = (ਦਸ਼ਮਲਵ)(t1*alpha + c[1]*beta);
                        d[2] = (ਦਸ਼ਮਲਵ)(t2*alpha + c[2]*beta);
                        d[3] = (ਦਸ਼ਮਲਵ)(t3*alpha + c[3]*beta);
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ( len <= 16 && a != d )
                {
                    ਪੂਰਨ c_step0 = 1;
                    ਜੇ( c == zerof )
                    {
                        c_step0 = 0;
                        c_ਕਦਮ = 1;
                    }

                    ਲਈ( i = 0; i < d_ਮਾਪ.ਚੌੜਾਈ; i++, d++, b++, c += c_step0 )
                    {
                        ਦਸ਼ਮਲਵ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ] + a[2]*b[b_ਕਦਮ*2] + a[3]*b[b_ਕਦਮ*3];
                        ਦਸ਼ਮਲਵ t1 = a[a_ਕਦਮ]*b[0] + a[a_ਕਦਮ+1]*b[b_ਕਦਮ] +
                                   a[a_ਕਦਮ+2]*b[b_ਕਦਮ*2] + a[a_ਕਦਮ+3]*b[b_ਕਦਮ*3];
                        ਦਸ਼ਮਲਵ t2 = a[a_ਕਦਮ*2]*b[0] + a[a_ਕਦਮ*2+1]*b[b_ਕਦਮ] +
                                   a[a_ਕਦਮ*2+2]*b[b_ਕਦਮ*2] + a[a_ਕਦਮ*2+3]*b[b_ਕਦਮ*3];
                        ਦਸ਼ਮਲਵ t3 = a[a_ਕਦਮ*3]*b[0] + a[a_ਕਦਮ*3+1]*b[b_ਕਦਮ] +
                                   a[a_ਕਦਮ*3+2]*b[b_ਕਦਮ*2] + a[a_ਕਦਮ*3+3]*b[b_ਕਦਮ*3];
                        d[0] = (ਦਸ਼ਮਲਵ)(t0*alpha + c[0]*beta);
                        d[d_ਕਦਮ] = (ਦਸ਼ਮਲਵ)(t1*alpha + c[c_ਕਦਮ]*beta);
                        d[d_ਕਦਮ*2] = (ਦਸ਼ਮਲਵ)(t2*alpha + c[c_ਕਦਮ*2]*beta);
                        d[d_ਕਦਮ*3] = (ਦਸ਼ਮਲਵ)(t3*alpha + c[c_ਕਦਮ*3]*beta);
                    }
                }
                ਨਹੀਂਤਾਂ
                    ਰੋਕੋ;
                ਵਾਪਸ;
            }
        }

        ਜੇ( type == CV_64F )
        {
            ਦੁੱਗਣਾ* d = D.ptr<ਦੁੱਗਣਾ>();
            ਅਚਲ ਦੁੱਗਣਾ *a = A.ptr<ਦੁੱਗਣਾ>(),
                         *b = B.ptr<ਦੁੱਗਣਾ>(),
                         *c = (ਅਚਲ ਦੁੱਗਣਾ*)C.ਅੰਕੜੇ;
            size_t d_ਕਦਮ = D.ਕਦਮ/ਆਕਾਰ(d[0]),
                a_ਕਦਮ = A.ਕਦਮ/ਆਕਾਰ(a[0]),
                b_ਕਦਮ = B.ਕਦਮ/ਆਕਾਰ(b[0]),
                c_ਕਦਮ = C.ਅੰਕੜੇ ? C.ਕਦਮ/ਆਕਾਰ(c[0]) : 0;
            ਜੇ( !c )
                c = zero;

            ਚੋਣ( len )
            {
            ਹਾਲਤ 2:
                ਜੇ( len == d_ਮਾਪ.ਚੌੜਾਈ && b != d )
                {
                    ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i++, d += d_ਕਦਮ, a += a_ਕਦਮ, c += c_ਕਦਮ )
                    {
                        ਦੁੱਗਣਾ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ];
                        ਦੁੱਗਣਾ t1 = a[0]*b[1] + a[1]*b[b_ਕਦਮ+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ( a != d )
                {
                    ਪੂਰਨ c_step0 = 1;
                    ਜੇ( c == zero )
                    {
                        c_step0 = 0;
                        c_ਕਦਮ = 1;
                    }

                    ਲਈ( i = 0; i < d_ਮਾਪ.ਚੌੜਾਈ; i++, d++, b++, c += c_step0 )
                    {
                        ਦੁੱਗਣਾ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ];
                        ਦੁੱਗਣਾ t1 = a[a_ਕਦਮ]*b[0] + a[a_ਕਦਮ+1]*b[b_ਕਦਮ];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_ਕਦਮ] = t1*alpha + c[c_ਕਦਮ]*beta;
                    }
                }
                ਨਹੀਂਤਾਂ
                    ਰੋਕੋ;
                ਵਾਪਸ;
            ਹਾਲਤ 3:
                ਜੇ( len == d_ਮਾਪ.ਚੌੜਾਈ && b != d )
                {
                    ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i++, d += d_ਕਦਮ, a += a_ਕਦਮ, c += c_ਕਦਮ )
                    {
                        ਦੁੱਗਣਾ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ] + a[2]*b[b_ਕਦਮ*2];
                        ਦੁੱਗਣਾ t1 = a[0]*b[1] + a[1]*b[b_ਕਦਮ+1] + a[2]*b[b_ਕਦਮ*2+1];
                        ਦੁੱਗਣਾ t2 = a[0]*b[2] + a[1]*b[b_ਕਦਮ+2] + a[2]*b[b_ਕਦਮ*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ( a != d )
                {
                    ਪੂਰਨ c_step0 = 1;
                    ਜੇ( c == zero )
                    {
                        c_step0 = 0;
                        c_ਕਦਮ = 1;
                    }

                    ਲਈ( i = 0; i < d_ਮਾਪ.ਚੌੜਾਈ; i++, d++, b++, c += c_step0 )
                    {
                        ਦੁੱਗਣਾ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ] + a[2]*b[b_ਕਦਮ*2];
                        ਦੁੱਗਣਾ t1 = a[a_ਕਦਮ]*b[0] + a[a_ਕਦਮ+1]*b[b_ਕਦਮ] + a[a_ਕਦਮ+2]*b[b_ਕਦਮ*2];
                        ਦੁੱਗਣਾ t2 = a[a_ਕਦਮ*2]*b[0] + a[a_ਕਦਮ*2+1]*b[b_ਕਦਮ] + a[a_ਕਦਮ*2+2]*b[b_ਕਦਮ*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_ਕਦਮ] = t1*alpha + c[c_ਕਦਮ]*beta;
                        d[d_ਕਦਮ*2] = t2*alpha + c[c_ਕਦਮ*2]*beta;
                    }
                }
                ਨਹੀਂਤਾਂ
                    ਰੋਕੋ;
                ਵਾਪਸ;
            ਹਾਲਤ 4:
                ਜੇ( len == d_ਮਾਪ.ਚੌੜਾਈ && b != d )
                {
                    ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i++, d += d_ਕਦਮ, a += a_ਕਦਮ, c += c_ਕਦਮ )
                    {
                        ਦੁੱਗਣਾ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ] + a[2]*b[b_ਕਦਮ*2] + a[3]*b[b_ਕਦਮ*3];
                        ਦੁੱਗਣਾ t1 = a[0]*b[1] + a[1]*b[b_ਕਦਮ+1] + a[2]*b[b_ਕਦਮ*2+1] + a[3]*b[b_ਕਦਮ*3+1];
                        ਦੁੱਗਣਾ t2 = a[0]*b[2] + a[1]*b[b_ਕਦਮ+2] + a[2]*b[b_ਕਦਮ*2+2] + a[3]*b[b_ਕਦਮ*3+2];
                        ਦੁੱਗਣਾ t3 = a[0]*b[3] + a[1]*b[b_ਕਦਮ+3] + a[2]*b[b_ਕਦਮ*2+3] + a[3]*b[b_ਕਦਮ*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ( d_ਮਾਪ.ਚੌੜਾਈ <= 16 && a != d )
                {
                    ਪੂਰਨ c_step0 = 1;
                    ਜੇ( c == zero )
                    {
                        c_step0 = 0;
                        c_ਕਦਮ = 1;
                    }

                    ਲਈ( i = 0; i < d_ਮਾਪ.ਚੌੜਾਈ; i++, d++, b++, c += c_step0 )
                    {
                        ਦੁੱਗਣਾ t0 = a[0]*b[0] + a[1]*b[b_ਕਦਮ] + a[2]*b[b_ਕਦਮ*2] + a[3]*b[b_ਕਦਮ*3];
                        ਦੁੱਗਣਾ t1 = a[a_ਕਦਮ]*b[0] + a[a_ਕਦਮ+1]*b[b_ਕਦਮ] +
                                    a[a_ਕਦਮ+2]*b[b_ਕਦਮ*2] + a[a_ਕਦਮ+3]*b[b_ਕਦਮ*3];
                        ਦੁੱਗਣਾ t2 = a[a_ਕਦਮ*2]*b[0] + a[a_ਕਦਮ*2+1]*b[b_ਕਦਮ] +
                                    a[a_ਕਦਮ*2+2]*b[b_ਕਦਮ*2] + a[a_ਕਦਮ*2+3]*b[b_ਕਦਮ*3];
                        ਦੁੱਗਣਾ t3 = a[a_ਕਦਮ*3]*b[0] + a[a_ਕਦਮ*3+1]*b[b_ਕਦਮ] +
                                    a[a_ਕਦਮ*3+2]*b[b_ਕਦਮ*2] + a[a_ਕਦਮ*3+3]*b[b_ਕਦਮ*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_ਕਦਮ] = t1*alpha + c[c_ਕਦਮ]*beta;
                        d[d_ਕਦਮ*2] = t2*alpha + c[c_ਕਦਮ*2]*beta;
                        d[d_ਕਦਮ*3] = t3*alpha + c[c_ਕਦਮ*3]*beta;
                    }
                }
                ਨਹੀਂਤਾਂ
                    ਰੋਕੋ;
                ਵਾਪਸ;
            }
        }
    }

    {
    size_t b_ਕਦਮ = B.ਕਦਮ;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc ਖੰਡ_Mul_Func;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ਅਚਲ uchar* Cdata = C.ਅੰਕੜੇ;
    size_t Cstep = C.ਅੰਕੜੇ ? (size_t)C.ਕਦਮ : 0;
    Auto_ਬਫਰ<uchar> buf;

    ਜੇ( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, GEMMSingleMul_32f>;
        ਖੰਡ_Mul_Func = gemm_ਖੰਡ_Mul_Wrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ, GEMMStore_32f>;
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, GEMMSingleMul_64f>;
        ਖੰਡ_Mul_Func = gemm_ਖੰਡ_Mul_Wrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, GEMMStore_64f>;
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        ਖੰਡ_Mul_Func = gemm_ਖੰਡ_Mul_Wrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        ਖੰਡ_Mul_Func = gemm_ਖੰਡ_Mul_Wrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    ਜੇ( (d_ਮਾਪ.ਚੌੜਾਈ == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_ਕਦਮ = d_ਮਾਪ.ਚੌੜਾਈ == 1 ? 0 : CV_ELEM_ਮਾਪ(type);
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
    else*/ ਜੇ( ((d_ਮਾਪ.ਉਚਾਈ <= ਖੰਡ_lin_ਮਾਪ/2 || d_ਮਾਪ.ਚੌੜਾਈ <= ਖੰਡ_lin_ਮਾਪ/2) &&
        len <= 10000) || len <= 10 ||
        (d_ਮਾਪ.ਚੌੜਾਈ <= ਖੰਡ_lin_ਮਾਪ &&
        d_ਮਾਪ.ਉਚਾਈ <= ਖੰਡ_lin_ਮਾਪ && len <= ਖੰਡ_lin_ਮਾਪ) )
    {
        singleMulFunc( A.ptr(), A.ਕਦਮ, B.ptr(), b_ਕਦਮ, Cdata, Cstep,
                       matD->ptr(), matD->ਕਦਮ, a_ਮਾਪ, d_ਮਾਪ, alpha, beta, flags );
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ is_a_t = flags & GEMM_1_T;
        ਪੂਰਨ is_b_t = flags & GEMM_2_T;
        ਪੂਰਨ elem_ਮਾਪ_2 = CV_ELEM_ਮਾਪ(type);
        ਪੂਰਨ dk0_1, dk0_2;
        size_t a_buf_ਮਾਪ = 0, b_buf_ਮਾਪ, d_buf_ਮਾਪ;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        ਪੂਰਨ j, k, di = 0, dj = 0, dk = 0;
        ਪੂਰਨ dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        ਪੂਰਨ work_elem_ਮਾਪ = elem_ਮਾਪ_2 << (CV_MAT_ਡੂੰਘਾਈ(type) == CV_32F ? 1 : 0);

        ਜੇ( !is_a_t )
            a_step0 = A.ਕਦਮ, a_step1 = elem_ਮਾਪ_2;
        ਨਹੀਂਤਾਂ
            a_step0 = elem_ਮਾਪ_2, a_step1 = A.ਕਦਮ;

        ਜੇ( !is_b_t )
            b_step0 = b_ਕਦਮ, b_step1 = elem_ਮਾਪ_2;
        ਨਹੀਂਤਾਂ
            b_step0 = elem_ਮਾਪ_2, b_step1 = b_ਕਦਮ;

        ਜੇ( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        ਨਹੀਂਤਾਂ ਜੇ( !(flags & GEMM_3_T) )
            c_step0 = C.ਕਦਮ, c_step1 = elem_ਮਾਪ_2;
        ਨਹੀਂਤਾਂ
            c_step0 = elem_ਮਾਪ_2, c_step1 = C.ਕਦਮ;

        dm0 = std::min( ਖੰਡ_lin_ਮਾਪ, d_ਮਾਪ.ਉਚਾਈ );
        dn0 = std::min( ਖੰਡ_lin_ਮਾਪ, d_ਮਾਪ.ਚੌੜਾਈ );
        dk0_1 = ਖੰਡ_ਮਾਪ_3 / dm0;
        dk0_2 = ਖੰਡ_ਮਾਪ_3 / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        ਜੇ( dk0*dm0 > ਖੰਡ_ਮਾਪ_3 )
            dm0 = ਖੰਡ_ਮਾਪ_3 / dk0;
        ਜੇ( dk0*dn0 > ਖੰਡ_ਮਾਪ_3 )
            dn0 = ਖੰਡ_ਮਾਪ_3 / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_ਮਾਪ = (size_t)(dk0+dk0/8+1)*dk0_1*elem_ਮਾਪ_2;
        d_buf_ਮਾਪ = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_ਮਾਪ;

        ਜੇ( is_a_t )
        {
            a_buf_ਮਾਪ = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_ਮਾਪ_2;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_ਮਾਪ + b_buf_ਮਾਪ + a_buf_ਮਾਪ);
        d_buf = buf.ਅੰਕੜੇ();
        b_buf = d_buf + d_buf_ਮਾਪ;

        ਜੇ( is_a_t )
            a_buf = b_buf + b_buf_ਮਾਪ;

        ਲਈ( i = 0; i < d_ਮਾਪ.ਉਚਾਈ; i += di )
        {
            di = dm0;
            ਜੇ( i + di >= d_ਮਾਪ.ਉਚਾਈ || 8*(i + di) + di > 8*d_ਮਾਪ.ਉਚਾਈ )
                di = d_ਮਾਪ.ਉਚਾਈ - i;

            ਲਈ( j = 0; j < d_ਮਾਪ.ਚੌੜਾਈ; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->ਕਦਮ + j*elem_ਮਾਪ_2;
                ਅਚਲ uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t d_ਕਦਮ_2 = matD->ਕਦਮ;
                dj = dn0;

                ਜੇ( j + dj >= d_ਮਾਪ.ਚੌੜਾਈ || 8*(j + dj) + dj > 8*d_ਮਾਪ.ਚੌੜਾਈ )
                    dj = d_ਮਾਪ.ਚੌੜਾਈ - j;

                flags &= 15;
                ਜੇ( dk0 < len )
                {
                    _d = d_buf;
                    d_ਕਦਮ_2 = dj*work_elem_ਮਾਪ;
                }

                ਲਈ( k = 0; k < len; k += dk )
                {
                    ਅਚਲ uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t a_ਕਦਮ_2 = A.ਕਦਮ;
                    ਅਚਲ uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t b_ਕਦਮ_2 = b_ਕਦਮ;
                    ਮਾਪ a_bl_ਮਾਪ;

                    dk = dk0;
                    ਜੇ( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    ਜੇ( !is_a_t )
                        a_bl_ਮਾਪ.ਚੌੜਾਈ = dk, a_bl_ਮਾਪ.ਉਚਾਈ = di;
                    ਨਹੀਂਤਾਂ
                        a_bl_ਮਾਪ.ਚੌੜਾਈ = di, a_bl_ਮਾਪ.ਉਚਾਈ = dk;

                    ਜੇ( a_buf && is_a_t )
                    {
                        a_ਕਦਮ_2 = dk*elem_ਮਾਪ_2;
                        GEMM_Transpose_ਖੰਡ( _a, A.ਕਦਮ, a_buf, a_ਕਦਮ_2, a_bl_ਮਾਪ, elem_ਮਾਪ_2 );
                        std::swap( a_bl_ਮਾਪ.ਚੌੜਾਈ, a_bl_ਮਾਪ.ਉਚਾਈ );
                        _a = a_buf;
                    }

                    ਜੇ( dj < d_ਮਾਪ.ਚੌੜਾਈ )
                    {
                        ਮਾਪ b_ਮਾਪ;
                        ਜੇ( !is_b_t )
                            b_ਮਾਪ.ਚੌੜਾਈ = dj, b_ਮਾਪ.ਉਚਾਈ = dk;
                        ਨਹੀਂਤਾਂ
                            b_ਮਾਪ.ਚੌੜਾਈ = dk, b_ਮਾਪ.ਉਚਾਈ = dj;

                        b_ਕਦਮ_2 = b_ਮਾਪ.ਚੌੜਾਈ*elem_ਮਾਪ_2;
                        GEMM_Copy_ਖੰਡ( _b, b_ਕਦਮ, b_buf, b_ਕਦਮ_2, b_ਮਾਪ, elem_ਮਾਪ_2 );
                        _b = b_buf;
                    }

                    ਜੇ( dk0 < len )
                        ਖੰਡ_Mul_Func( _a, a_ਕਦਮ_2, _b, b_ਕਦਮ_2, _d, d_ਕਦਮ_2,
                                      a_bl_ਮਾਪ, ਮਾਪ(dj,di), flags );
                    ਨਹੀਂਤਾਂ
                        singleMulFunc( _a, a_ਕਦਮ_2, _b, b_ਕਦਮ_2, _c, Cstep,
                                       _d, d_ਕਦਮ_2, a_bl_ਮਾਪ, ਮਾਪ(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                ਜੇ( dk0 < len )
                    storeFunc( _c, Cstep, _d, d_ਕਦਮ_2,
                               matD->ptr(i) + j*elem_ਮਾਪ_2,
                               matD->ਕਦਮ, ਮਾਪ(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype>ਸਤਰਅੰਦਰ ਸਥਿਰ ਖਾਲੀ
callGemmImpl(ਅਚਲ fptype *src1, size_t src1_ਕਦਮ, ਅਚਲ fptype *src2, size_t src2_ਕਦਮ, fptype alpha,
          ਅਚਲ fptype *src3, size_t src3_ਕਦਮ, fptype beta, fptype *dst, size_t dst_ਕਦਮ, ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags, ਪੂਰਨ type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    ਪੂਰਨ b_m, b_n, c_m, c_n, m_d;

    ਜੇ(flags & GEMM_2_T)
    {
        b_m = n_d;
        ਜੇ(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        ਨਹੀਂਤਾਂ
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        b_n = n_d;
        ਜੇ(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        ਨਹੀਂਤਾਂ
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    ਜੇ(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    ਨਹੀਂਤਾਂ
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    ਜੇ(src1 != ਨਿਰਾ)
        A = Mat(m_a, n_a, type, (ਖਾਲੀ*)src1, src1_ਕਦਮ);
    ਜੇ(src2 != ਨਿਰਾ)
        B = Mat(b_m, b_n, type, (ਖਾਲੀ*)src2, src2_ਕਦਮ);
    ਜੇ(src3 != ਨਿਰਾ && beta != 0.0)
        C = Mat(c_m, c_n, type, (ਖਾਲੀ*)src3, src3_ਕਦਮ);
    Mat D(m_d, n_d, type, (ਖਾਲੀ*)dst, dst_ਕਦਮ);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

ਖਾਲੀ gemm32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
             ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
             ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags, CV_32F);
}

ਖਾਲੀ gemm64f(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
             ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
             ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags, CV_64F);
}

ਖਾਲੀ gemm32fc(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
              ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
              ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags, CV_32FC2);
}

ਖਾਲੀ gemm64fc(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
              ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
              ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags, CV_64FC2);
}

#ਜੇ_ਅੰਤ // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT> ਸਥਿਰ ਖਾਲੀ
transform_( ਅਚਲ T* src, T* dst, ਅਚਲ WT* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn )
{
    ਪੂਰਨ x;

    ਜੇ( scn == 2 && dcn == 2 )
    {
        ਲਈ( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( scn == 3 && dcn == 3 )
    {
        ਲਈ( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( scn == 3 && dcn == 1 )
    {
        ਲਈ( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    ਨਹੀਂਤਾਂ ਜੇ( scn == 4 && dcn == 4 )
    {
        ਲਈ( x = 0; x < len*4; x += 4 )
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
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ਅਚਲ WT* _m = m;
            ਪੂਰਨ j, k;
            ਲਈ( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                ਲਈ( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
transform_8u( ਅਚਲ uchar* src, uchar* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn )
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ ਬਿੱਟ = 10, SCALE = 1 << ਬਿੱਟ;
    ਅਚਲ ਦਸ਼ਮਲਵ MAX_M = (ਦਸ਼ਮਲਵ)(1 << (15 - ਬਿੱਟ));

    ਜੇ( scn == 3 && dcn == 3 &&
        std::ਨਿਰਪੇਖ(m[0]) < MAX_M && std::ਨਿਰਪੇਖ(m[1]) < MAX_M && std::ਨਿਰਪੇਖ(m[ 2]) < MAX_M*256 && std::ਨਿਰਪੇਖ(m[ 3]) < MAX_M*256 &&
        std::ਨਿਰਪੇਖ(m[4]) < MAX_M && std::ਨਿਰਪੇਖ(m[5]) < MAX_M && std::ਨਿਰਪੇਖ(m[ 6]) < MAX_M*256 && std::ਨਿਰਪੇਖ(m[ 7]) < MAX_M*256 &&
        std::ਨਿਰਪੇਖ(m[8]) < MAX_M && std::ਨਿਰਪੇਖ(m[9]) < MAX_M && std::ਨਿਰਪੇਖ(m[10]) < MAX_M*256 && std::ਨਿਰਪੇਖ(m[11]) < MAX_M*256 )
    {
        ਅਚਲ ਪੂਰਨ nChannels = 3;

        ਸੰਘ {
            ਛੋਟਾ s[6];
            ਪੂਰਨ p[3];
        } m16;
        m16.s[0] = saturate_cast<ਛੋਟਾ>(m[0] * SCALE); m16.s[1] = saturate_cast<ਛੋਟਾ>(m[1] * SCALE);
        m16.s[2] = saturate_cast<ਛੋਟਾ>(m[4] * SCALE); m16.s[3] = saturate_cast<ਛੋਟਾ>(m[5] * SCALE);
        m16.s[4] = saturate_cast<ਛੋਟਾ>(m[8] * SCALE); m16.s[5] = saturate_cast<ਛੋਟਾ>(m[9] * SCALE);
        ਪੂਰਨ m32[] = {saturate_cast<ਪੂਰਨ>(m[ 2] * SCALE), saturate_cast<ਪੂਰਨ>(m[ 3] * SCALE),
                     saturate_cast<ਪੂਰਨ>(m[ 6] * SCALE), saturate_cast<ਪੂਰਨ>(m[ 7] * SCALE),
                     saturate_cast<ਪੂਰਨ>(m[10] * SCALE), saturate_cast<ਪੂਰਨ>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        ਪੂਰਨ x = 0;
        ਲਈ (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
        {
            v_uint8 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
            v_uint8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_uint16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_uint16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack_u(dbl, dbh), v_pack_u(dgl, dgh), v_pack_u(drl, drh));
        }
        m32[1] = saturate_cast<ਪੂਰਨ>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<ਪੂਰਨ>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<ਪੂਰਨ>((m[11] + 0.5f)*SCALE);
        ਲਈ( ; x < len * nChannels; x += nChannels )
        {
            ਪੂਰਨ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> ਬਿੱਟ);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> ਬਿੱਟ);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> ਬਿੱਟ);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    transform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
transform_16u( ਅਚਲ ushort* src, ushort* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn )
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ( scn == 3 && dcn == 3 )
    {
        ਪੂਰਨ x = 0;
#ਜੇ CV_SIMD_ਚੌੜਾਈ > 16
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
        ਲਈ (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
        {
            v_uint16 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
            v_uint32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_ਜੋੜੋ_wrap(v_pack(v_ਗੇੜਾ(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m2,  m3)))),
                                   v_ਗੇੜਾ(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m2,  m3))))), delta);
            dg = v_ਜੋੜੋ_wrap(v_pack(v_ਗੇੜਾ(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m6,  m7)))),
                                   v_ਗੇੜਾ(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m6,  m7))))), delta);
            dr = v_ਜੋੜੋ_wrap(v_pack(v_ਗੇੜਾ(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)), m10, m11)))),
                                   v_ਗੇੜਾ(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)), m10, m11))))), delta);
            v_store_interleave(dst + x, v_reinterpret_as_u16(db), v_reinterpret_as_u16(dg), v_reinterpret_as_u16(dr));
        }
#ਜੇ_ਅੰਤ
#ਜੇ CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_ਖੱਬਾ<1>(_m0l);
        v_float32x4 _m1h = v_rotate_ਖੱਬਾ<1>(_m1l);
        v_float32x4 _m2h = v_rotate_ਖੱਬਾ<1>(_m2l);
        v_float32x4 _m3h = v_rotate_ਖੱਬਾ<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        ਲਈ( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_ਸੱਜਾ<1>(v_reinterpret_as_u16(v_ਜੋੜੋ_wrap(v_pack(
                             v_ਗੇੜਾ(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_ਲੱਦੋ_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_ਗੇੜਾ(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_ਲੱਦੋ_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#ਜੇ_ਅੰਤ //CV_SIMD128
        ਲਈ( ; x < len * 3; x += 3 )
        {
            ਦਸ਼ਮਲਵ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    transform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
transform_32f( ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    ਪੂਰਨ x = 0;
    ਜੇ( scn == 3 && dcn == 3 )
    {
        ਪੂਰਨ idx[VTraits<v_float32>::max_nlanes/2];
        ਲਈ( ਪੂਰਨ i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        ਦਸ਼ਮਲਵ _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        ਲਈ( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        ਲਈ( ; x < len*3; x += 3 )
        {
            ਦਸ਼ਮਲਵ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            ਦਸ਼ਮਲਵ t0 = saturate_cast<ਦਸ਼ਮਲਵ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            ਦਸ਼ਮਲਵ t1 = saturate_cast<ਦਸ਼ਮਲਵ>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            ਦਸ਼ਮਲਵ t2 = saturate_cast<ਦਸ਼ਮਲਵ>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }

    ਜੇ( scn == 4 && dcn == 4 )
    {
#ਜੇ CV_SIMD_ਚੌੜਾਈ > 16
        ਪੂਰਨ idx[VTraits<v_float32>::max_nlanes/4];
        ਲਈ( ਪੂਰਨ i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        ਦਸ਼ਮਲਵ _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        ਲਈ( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_ਲੱਦੋ(src + x);
            v_store(dst + x, v_ਜੋੜੋ(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#ਜੇ_ਅੰਤ
#ਜੇ CV_SIMD128
        v_float32x4 _m0 = v_ਲੱਦੋ(m     );
        v_float32x4 _m1 = v_ਲੱਦੋ(m +  5);
        v_float32x4 _m2 = v_ਲੱਦੋ(m + 10);
        v_float32x4 _m3 = v_ਲੱਦੋ(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        ਲਈ( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_ਲੱਦੋ(src + x);
            v_store(dst + x, v_ਜੋੜੋ(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#ਨਹੀਂਤਾਂ // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        ਲਈ( ; x < len*4; x += 4 )
        {
            ਦਸ਼ਮਲਵ v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            ਦਸ਼ਮਲਵ t0 = saturate_cast<ਦਸ਼ਮਲਵ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            ਦਸ਼ਮਲਵ t1 = saturate_cast<ਦਸ਼ਮਲਵ>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            ਦਸ਼ਮਲਵ t2 = saturate_cast<ਦਸ਼ਮਲਵ>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            ਦਸ਼ਮਲਵ t3 = saturate_cast<ਦਸ਼ਮਲਵ>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#ਜੇ_ਅੰਤ
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    transform_(src, dst, m, len, scn, dcn);
}


ਸਥਿਰ ਖਾਲੀ
transform_8s(ਅਚਲ schar* src, schar* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ ਬਿੱਟ = 10, SCALE = 1 << ਬਿੱਟ;
    ਅਚਲ ਦਸ਼ਮਲਵ MAX_M = (ਦਸ਼ਮਲਵ)(1 << (15 - ਬਿੱਟ));

    ਜੇ( scn == 3 && dcn == 3 &&
        std::ਨਿਰਪੇਖ(m[0]) < MAX_M && std::ਨਿਰਪੇਖ(m[1]) < MAX_M && std::ਨਿਰਪੇਖ(m[ 2]) < MAX_M*256 && std::ਨਿਰਪੇਖ(m[ 3]) < MAX_M*256 &&
        std::ਨਿਰਪੇਖ(m[4]) < MAX_M && std::ਨਿਰਪੇਖ(m[5]) < MAX_M && std::ਨਿਰਪੇਖ(m[ 6]) < MAX_M*256 && std::ਨਿਰਪੇਖ(m[ 7]) < MAX_M*256 &&
        std::ਨਿਰਪੇਖ(m[8]) < MAX_M && std::ਨਿਰਪੇਖ(m[9]) < MAX_M && std::ਨਿਰਪੇਖ(m[10]) < MAX_M*256 && std::ਨਿਰਪੇਖ(m[11]) < MAX_M*256 )
    {
        ਅਚਲ ਪੂਰਨ nChannels = 3;

        ਸੰਘ {
            ਛੋਟਾ s[6];
            ਪੂਰਨ p[3];
        } m16;
        m16.s[0] = saturate_cast<ਛੋਟਾ>(m[0] * SCALE); m16.s[1] = saturate_cast<ਛੋਟਾ>(m[1] * SCALE);
        m16.s[2] = saturate_cast<ਛੋਟਾ>(m[4] * SCALE); m16.s[3] = saturate_cast<ਛੋਟਾ>(m[5] * SCALE);
        m16.s[4] = saturate_cast<ਛੋਟਾ>(m[8] * SCALE); m16.s[5] = saturate_cast<ਛੋਟਾ>(m[9] * SCALE);
        ਪੂਰਨ m32[] = {saturate_cast<ਪੂਰਨ>(m[ 2] * SCALE), saturate_cast<ਪੂਰਨ>(m[ 3] * SCALE),
                     saturate_cast<ਪੂਰਨ>(m[ 6] * SCALE), saturate_cast<ਪੂਰਨ>(m[ 7] * SCALE),
                     saturate_cast<ਪੂਰਨ>(m[10] * SCALE), saturate_cast<ਪੂਰਨ>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        ਪੂਰਨ x = 0;
        ਲਈ (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
        {
            v_int8 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
            v_int8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_int16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_int16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<ਬਿੱਟ>(v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_ਜੋੜੋ(v_ਜੋੜੋ(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack(dbl, dbh), v_pack(dgl, dgh), v_pack(drl, drh));
        }
        m32[1] = saturate_cast<ਪੂਰਨ>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<ਪੂਰਨ>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<ਪੂਰਨ>((m[11] + 0.5f)*SCALE);
        ਲਈ( ; x < len * nChannels; x += nChannels )
        {
            ਪੂਰਨ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> ਬਿੱਟ);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> ਬਿੱਟ);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> ਬਿੱਟ);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    transform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
transform_16s(ਅਚਲ ਛੋਟਾ* src, ਛੋਟਾ* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

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
        ਲਈ (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
        {
            v_int16 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
            v_int32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_pack(v_ਗੇੜਾ(v_muladd(v_cvt_f32(bl), m0, v_muladd(v_cvt_f32(gl), m1, v_muladd(v_cvt_f32(rl),  m2,  m3)))),
                        v_ਗੇੜਾ(v_muladd(v_cvt_f32(bh), m0, v_muladd(v_cvt_f32(gh), m1, v_muladd(v_cvt_f32(rh),  m2,  m3)))));
            dg = v_pack(v_ਗੇੜਾ(v_muladd(v_cvt_f32(bl), m4, v_muladd(v_cvt_f32(gl), m5, v_muladd(v_cvt_f32(rl),  m6,  m7)))),
                        v_ਗੇੜਾ(v_muladd(v_cvt_f32(bh), m4, v_muladd(v_cvt_f32(gh), m5, v_muladd(v_cvt_f32(rh),  m6,  m7)))));
            dr = v_pack(v_ਗੇੜਾ(v_muladd(v_cvt_f32(bl), m8, v_muladd(v_cvt_f32(gl), m9, v_muladd(v_cvt_f32(rl), m10, m11)))),
                        v_ਗੇੜਾ(v_muladd(v_cvt_f32(bh), m8, v_muladd(v_cvt_f32(gh), m9, v_muladd(v_cvt_f32(rh), m10, m11)))));
            v_store_interleave(dst + x, db, dg, dr);
        }
        ਲਈ( ; x < len * 3; x += 3 )
        {
            ਦਸ਼ਮਲਵ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ਛੋਟਾ t0 = saturate_cast<ਛੋਟਾ>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ਛੋਟਾ t1 = saturate_cast<ਛੋਟਾ>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ਛੋਟਾ t2 = saturate_cast<ਛੋਟਾ>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    transform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
transform_32s(ਅਚਲ ਪੂਰਨ* src, ਪੂਰਨ* dst, ਅਚਲ ਦੁੱਗਣਾ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ਜੇ( scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

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
        ਲਈ (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ਉੱਚਾ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ਉੱਚਾ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ਉੱਚਾ(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_ਗੇੜਾ(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_ਗੇੜਾ(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_ਗੇੜਾ(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਦੁੱਗਣਾ b = src[x], g = src[x + 1], r = src[x + 2];
            ਪੂਰਨ db = saturate_cast<ਪੂਰਨ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ਪੂਰਨ dg = saturate_cast<ਪੂਰਨ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ਪੂਰਨ dr = saturate_cast<ਪੂਰਨ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    transform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
transform_64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਅਚਲ ਦੁੱਗਣਾ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ਜੇ( scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

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
        ਲਈ (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਦੁੱਗਣਾ b = src[x], g = src[x + 1], r = src[x + 2];
            ਦੁੱਗਣਾ db = saturate_cast<ਦੁੱਗਣਾ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            ਦੁੱਗਣਾ dg = saturate_cast<ਦੁੱਗਣਾ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            ਦੁੱਗਣਾ dr = saturate_cast<ਦੁੱਗਣਾ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    transform_(src, dst, m, len, scn, dcn);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT> ਸਥਿਰ ਖਾਲੀ
diagtransform_( ਅਚਲ T* src, T* dst, ਅਚਲ WT* m, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ )
{
    ਪੂਰਨ x;

    ਜੇ( cn == 2 )
    {
        ਲਈ( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( cn == 3 )
    {
        ਲਈ( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( cn == 4 )
    {
        ਲਈ( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ਅਚਲ WT* _m = m;
            ਲਈ( ਪੂਰਨ j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
diagtransform_8u(ਅਚਲ uchar* src, uchar* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ਲਈ (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
            v_uint8 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack_u(v_ਗੇੜਾ(db_f32_l0), v_ਗੇੜਾ(db_f32_l1)), v_pack_u(v_ਗੇੜਾ(db_f32_h0), v_ਗੇੜਾ(db_f32_h1))),
                               v_pack(v_pack_u(v_ਗੇੜਾ(dg_f32_l0), v_ਗੇੜਾ(dg_f32_l1)), v_pack_u(v_ਗੇੜਾ(dg_f32_h0), v_ਗੇੜਾ(dg_f32_h1))),
                               v_pack(v_pack_u(v_ਗੇੜਾ(dr_f32_l0), v_ਗੇੜਾ(dr_f32_l1)), v_pack_u(v_ਗੇੜਾ(dr_f32_h0), v_ਗੇੜਾ(dr_f32_h1))));
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਪੂਰਨ b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
diagtransform_8s(ਅਚਲ schar* src, schar* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
    #ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ਲਈ (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
            v_int8 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack(v_ਗੇੜਾ(db_f32_l0), v_ਗੇੜਾ(db_f32_l1)), v_pack(v_ਗੇੜਾ(db_f32_h0), v_ਗੇੜਾ(db_f32_h1))),
                               v_pack(v_pack(v_ਗੇੜਾ(dg_f32_l0), v_ਗੇੜਾ(dg_f32_l1)), v_pack(v_ਗੇੜਾ(dg_f32_h0), v_ਗੇੜਾ(dg_f32_h1))),
                               v_pack(v_pack(v_ਗੇੜਾ(dr_f32_l0), v_ਗੇੜਾ(dr_f32_l1)), v_pack(v_ਗੇੜਾ(dr_f32_h0), v_ਗੇੜਾ(dr_f32_h1))));
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਪੂਰਨ b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
diagtransform_16u(ਅਚਲ ushort* src, ushort* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ਲਈ (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
            v_uint16 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
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
                               v_pack_u(v_ਗੇੜਾ(db_f32_l), v_ਗੇੜਾ(db_f32_h)),
                               v_pack_u(v_ਗੇੜਾ(dg_f32_l), v_ਗੇੜਾ(dg_f32_h)),
                               v_pack_u(v_ਗੇੜਾ(dr_f32_l), v_ਗੇੜਾ(dr_f32_h)));
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਪੂਰਨ b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
diagtransform_16s(ਅਚਲ ਛੋਟਾ* src, ਛੋਟਾ* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ਲਈ (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
            v_int16 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
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
                               v_pack(v_ਗੇੜਾ(db_f32_l), v_ਗੇੜਾ(db_f32_h)),
                               v_pack(v_ਗੇੜਾ(dg_f32_l), v_ਗੇੜਾ(dg_f32_h)),
                               v_pack(v_ਗੇੜਾ(dr_f32_l), v_ਗੇੜਾ(dr_f32_h)));
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਪੂਰਨ b = src[x], g = src[x + 1], r = src[x + 2];
            ਛੋਟਾ db = saturate_cast<ਛੋਟਾ>(m[ 0] * b + m[ 3]);
            ਛੋਟਾ dg = saturate_cast<ਛੋਟਾ>(m[ 5] * g + m[ 7]);
            ਛੋਟਾ dr = saturate_cast<ਛੋਟਾ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
diagtransform_32s(ਅਚਲ ਪੂਰਨ* src, ਪੂਰਨ* dst, ਅਚਲ ਦੁੱਗਣਾ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ਜੇ (scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ਲਈ (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ਉੱਚਾ(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ਉੱਚਾ(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ਉੱਚਾ(r), rl = v_cvt_f64(r);

            v_int32 db = v_ਗੇੜਾ(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_ਗੇੜਾ(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_ਗੇੜਾ(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਪੂਰਨ b = src[x], g = src[x + 1], r = src[x + 2];
            ਪੂਰਨ db = saturate_cast<ਪੂਰਨ>(m[ 0] * b + m[ 3]);
            ਪੂਰਨ dg = saturate_cast<ਪੂਰਨ>(m[ 5] * g + m[ 7]);
            ਪੂਰਨ dr = saturate_cast<ਪੂਰਨ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
diagtransform_32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਅਚਲ ਦਸ਼ਮਲਵ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        ਲਈ (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਦਸ਼ਮਲਵ b = src[x], g = src[x + 1], r = src[x + 2];
            ਦਸ਼ਮਲਵ db = saturate_cast<ਦਸ਼ਮਲਵ>(m[ 0] * b + m[ 3]);
            ਦਸ਼ਮਲਵ dg = saturate_cast<ਦਸ਼ਮਲਵ>(m[ 5] * g + m[ 7]);
            ਦਸ਼ਮਲਵ dr = saturate_cast<ਦਸ਼ਮਲਵ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    diagtransform_(src, dst, m, len, scn, dcn);
}

ਸਥਿਰ ਖਾਲੀ
diagtransform_64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਅਚਲ ਦੁੱਗਣਾ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ਜੇ (scn == 3 && dcn == 3) {
        ਪੂਰਨ x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        ਲਈ (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_ਲੱਦੋ_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        ਲਈ (; x < len * 3; x += 3) {
            ਦੁੱਗਣਾ b = src[x], g = src[x + 1], r = src[x + 2];
            ਦੁੱਗਣਾ db = saturate_cast<ਦੁੱਗਣਾ>(m[ 0] * b + m[ 3]);
            ਦੁੱਗਣਾ dg = saturate_cast<ਦੁੱਗਣਾ>(m[ 5] * g + m[ 7]);
            ਦੁੱਗਣਾ dr = saturate_cast<ਦੁੱਗਣਾ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    diagtransform_(src, dst, m, len, scn, dcn);
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT, ਖਾਲੀ (*fn)(ਅਚਲ T*, T*, ਅਚਲ WT*, ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ)>
ਸਥਿਰ ਖਾਲੀ transformWrap(ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ* dst, ਅਚਲ ਖਾਲੀ* m, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn)
{
    fn((ਅਚਲ T*)src, (T*)dst, (ਅਚਲ WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ TransformFunc transformTab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        transformWrap<uchar, ਦਸ਼ਮਲਵ, transform_8u>, transformWrap<schar, ਦਸ਼ਮਲਵ, transform_8s>, transformWrap<ushort, ਦਸ਼ਮਲਵ, transform_16u>,
        transformWrap<ਛੋਟਾ, ਦਸ਼ਮਲਵ, transform_16s>, transformWrap<ਪੂਰਨ, ਦੁੱਗਣਾ, transform_32s>, transformWrap<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, transform_32f>,
        transformWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, transform_64f>, 0
    };

    ਵਾਪਸ transformTab[ਡੂੰਘਾਈ];
}

TransformFunc getDiagTransformFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ TransformFunc diagTransformTab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        transformWrap<uchar, ਦਸ਼ਮਲਵ, diagtransform_8u>, transformWrap<schar, ਦਸ਼ਮਲਵ, diagtransform_8s>, transformWrap<ushort, ਦਸ਼ਮਲਵ, diagtransform_16u>,
        transformWrap<ਛੋਟਾ, ਦਸ਼ਮਲਵ, diagtransform_16s>, transformWrap<ਪੂਰਨ, ਦੁੱਗਣਾ, diagtransform_32s>, transformWrap<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, diagtransform_32f>,
        transformWrap<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, diagtransform_64f>, 0
    };

    ਵਾਪਸ diagTransformTab[ਡੂੰਘਾਈ];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
perspectiveTransform_( ਅਚਲ ਖਾਲੀ* _src, ਖਾਲੀ* _dst, ਅਚਲ ਖਾਲੀ* _m_ptr, ਪੂਰਨ len, ਪੂਰਨ scn, ਪੂਰਨ dcn )
{
    ਅਚਲ T* src = (ਅਚਲ T*)_src;
    T* dst = (T*)_dst;
    ਅਚਲ ਦੁੱਗਣਾ* m = (ਅਚਲ ਦੁੱਗਣਾ*)_m_ptr;
    ਅਚਲ ਦੁੱਗਣਾ eps = FLT_EPSILON;
    ਪੂਰਨ i;

    ਜੇ( scn == 2 && dcn == 2 )
    {
        ਲਈ( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            ਦੁੱਗਣਾ w = x*m[6] + y*m[7] + m[8];

            ਜੇ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            ਨਹੀਂਤਾਂ
                dst[i] = dst[i+1] = (T)0;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( scn == 3 && dcn == 3 )
    {
        ਲਈ( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            ਦੁੱਗਣਾ w = x*m[12] + y*m[13] + z*m[14] + m[15];

            ਜੇ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            ਨਹੀਂਤਾਂ
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( scn == 3 && dcn == 2 )
    {
        ਲਈ( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            ਦੁੱਗਣਾ w = x*m[8] + y*m[9] + z*m[10] + m[11];

            ਜੇ( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            ਨਹੀਂਤਾਂ
                dst[0] = dst[1] = (T)0;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ਅਚਲ ਦੁੱਗਣਾ* _m = m + dcn*(scn + 1);
            ਦੁੱਗਣਾ w = _m[scn];
            ਪੂਰਨ j, k;
            ਲਈ( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            ਜੇ( fabs(w) > eps )
            {
                _m = m;
                ਲਈ( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    ਦੁੱਗਣਾ s = _m[scn];
                    ਲਈ( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            ਨਹੀਂਤਾਂ
                ਲਈ( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਜੇ (ਡੂੰਘਾਈ == CV_32F)
        ਵਾਪਸ perspectiveTransform_<ਦਸ਼ਮਲਵ>;
    ਜੇ (ਡੂੰਘਾਈ == CV_64F)
        ਵਾਪਸ perspectiveTransform_<ਦੁੱਗਣਾ>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

ਸਥਿਰ ਖਾਲੀ scale_ਜੋੜੋ_32f(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, ਖਾਲੀ* _dst,
                         ਪੂਰਨ len, ਅਚਲ ਖਾਲੀ* _alpha)
{
    ਅਚਲ ਦਸ਼ਮਲਵ* src1 = (ਅਚਲ ਦਸ਼ਮਲਵ*)_src1;
    ਅਚਲ ਦਸ਼ਮਲਵ* src2 = (ਅਚਲ ਦਸ਼ਮਲਵ*)_src2;
    ਦਸ਼ਮਲਵ* dst = (ਦਸ਼ਮਲਵ*)_dst;
    ਦਸ਼ਮਲਵ alpha = *(ਅਚਲ ਦਸ਼ਮਲਵ*)_alpha;
    ਪੂਰਨ i = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ਅਚਲ ਪੂਰਨ c_ਚੌੜਾਈ = VTraits<v_float32>::vlanes();
    ਲਈ (; i <= len - c_ਚੌੜਾਈ; i += c_ਚੌੜਾਈ)
        v_store(dst + i, v_muladd(vx_ਲੱਦੋ(src1 + i), v_alpha, vx_ਲੱਦੋ(src2 + i)));
    vx_cleanup();
#ਜੇ_ਅੰਤ
    ਲਈ (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


ਸਥਿਰ ਖਾਲੀ scale_ਜੋੜੋ_64f(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, ਖਾਲੀ* _dst,
                         ਪੂਰਨ len, ਅਚਲ ਖਾਲੀ* _alpha)
{
    ਅਚਲ ਦੁੱਗਣਾ* src1 = (ਅਚਲ ਦੁੱਗਣਾ*)_src1;
    ਅਚਲ ਦੁੱਗਣਾ* src2 = (ਅਚਲ ਦੁੱਗਣਾ*)_src2;
    ਦੁੱਗਣਾ* dst = (ਦੁੱਗਣਾ*)_dst;
    ਦੁੱਗਣਾ alpha = *(ਅਚਲ ਦੁੱਗਣਾ*)_alpha;
    ਪੂਰਨ i = 0;
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ਅਚਲ ਪੂਰਨ c_ਚੌੜਾਈ = VTraits<v_float64>::vlanes();
    ਲਈ (; i <= len - c_ਚੌੜਾਈ; i += c_ਚੌੜਾਈ)
        v_store(dst + i, v_muladd(vx_ਲੱਦੋ(src1 + i), a2, vx_ਲੱਦੋ(src2 + i)));
    vx_cleanup();
#ਜੇ_ਅੰਤ
    ਲਈ (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

Scale_ਜੋੜੋ_Func get_Scale_ਜੋੜੋ_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਜੇ (ਡੂੰਘਾਈ == CV_32F)
        ਵਾਪਸ scale_ਜੋੜੋ_32f;
    ਜੇ (ਡੂੰਘਾਈ == CV_64F)
        ਵਾਪਸ scale_ਜੋੜੋ_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਦੁੱਗਣਾ MahalanobisImpl(ਅਚਲ Mat& v1, ਅਚਲ Mat& v2, ਅਚਲ Mat& icovar, ਦੁੱਗਣਾ *diff_ਬਫਰ /*[len]*/, ਪੂਰਨ len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    ਮਾਪ sz = v1.size();
    ਦੁੱਗਣਾ ਨਤੀਜਾ = 0;

    sz.ਚੌੜਾਈ *= v1.channels();
    ਜੇ (v1.isContinuous() && v2.isContinuous())
    {
        sz.ਚੌੜਾਈ *= sz.ਉਚਾਈ;
        sz.ਉਚਾਈ = 1;
    }

    {
        ਅਚਲ T* src1 = v1.ptr<T>();
        ਅਚਲ T* src2 = v2.ptr<T>();
        size_t step1 = v1.ਕਦਮ/ਆਕਾਰ(src1[0]);
        size_t step2 = v2.ਕਦਮ/ਆਕਾਰ(src2[0]);
        ਦੁੱਗਣਾ* diff = diff_ਬਫਰ;
        ਅਚਲ T* mat = icovar.ptr<T>();
        size_t matstep = icovar.ਕਦਮ/ਆਕਾਰ(mat[0]);

        ਲਈ (; sz.ਉਚਾਈ--; src1 += step1, src2 += step2, diff += sz.ਚੌੜਾਈ)
        {
            ਲਈ (ਪੂਰਨ i = 0; i < sz.ਚੌੜਾਈ; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_ਬਫਰ;
        ਲਈ (ਪੂਰਨ i = 0; i < len; i++, mat += matstep)
        {
            ਦੁੱਗਣਾ ਪੰਕਤੀ_ਜੋੜ = 0;
            ਪੂਰਨ j = 0;
#ਜੇ CV_ENABLE_UNROLLED
            ਲਈ(; j <= len - 4; j += 4 )
                ਪੰਕਤੀ_ਜੋੜ += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#ਜੇ_ਅੰਤ
            ਲਈ (; j < len; j++)
                ਪੰਕਤੀ_ਜੋੜ += diff[j]*mat[j];
            ਨਤੀਜਾ += ਪੰਕਤੀ_ਜੋੜ * diff[i];
        }
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

MahalanobisImplFunc getMahalanobisImplFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਜੇ (ਡੂੰਘਾਈ == CV_32F)
        ਵਾਪਸ (MahalanobisImplFunc)MahalanobisImpl<ਦਸ਼ਮਲਵ>;
    ਜੇ (ਡੂੰਘਾਈ == CV_64F)
        ਵਾਪਸ (MahalanobisImplFunc)MahalanobisImpl<ਦੁੱਗਣਾ>;
    CV_Assert(0 && "Not supported");
}


#ਜੇ !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_ਬਹੁਲਕ)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ sT, ਕਿਸਮ_ਨਾਂ dT> ਸਥਿਰ ਖਾਲੀ
MulTransposedR(ਅਚਲ Mat& srcmat, ਅਚਲ Mat& dstmat, ਅਚਲ Mat& deltamat, ਦੁੱਗਣਾ scale)
{
    ਪੂਰਨ i, j, k;
    ਅਚਲ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ਅਚਲ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.ਕਦਮ/ਆਕਾਰ(src[0]);
    size_t dststep = dstmat.ਕਦਮ/ਆਕਾਰ(dst[0]);
    size_t deltastep = deltamat.ਪੰਕਤੀਆਂ > 1 ? deltamat.ਕਦਮ/ਆਕਾਰ(delta[0]) : 0;
    ਪੂਰਨ delta_ਖਾਨੇ_ਗਿਣਤੀ = deltamat.ਖਾਨੇ_ਗਿਣਤੀ;
    ਮਾਪ size = srcmat.size();
    dT* tdst = dst;
    dT* ਖਾਨਾ_buf = 0;
    dT* delta_buf = 0;
    ਪੂਰਨ buf_ਮਾਪ_2 = size.ਉਚਾਈ*ਆਕਾਰ(dT);
    Auto_ਬਫਰ<uchar> buf;

    ਜੇ( delta && delta_ਖਾਨੇ_ਗਿਣਤੀ < size.ਚੌੜਾਈ )
    {
        CV_Assert( delta_ਖਾਨੇ_ਗਿਣਤੀ == 1 );
        buf_ਮਾਪ_2 *= 5;
    }
    buf.allocate(buf_ਮਾਪ_2);
    ਖਾਨਾ_buf = (dT*)buf.ਅੰਕੜੇ();

    ਜੇ( delta && delta_ਖਾਨੇ_ਗਿਣਤੀ < size.ਚੌੜਾਈ )
    {
        delta_buf = ਖਾਨਾ_buf + size.ਉਚਾਈ;
        ਲਈ( i = 0; i < size.ਉਚਾਈ; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#ਜੇ CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#ਜੇ_ਅੰਤ

    ਜੇ( !delta )
        ਲਈ( i = 0; i < size.ਚੌੜਾਈ; i++, tdst += dststep )
        {
            ਲਈ( k = 0; k < size.ਉਚਾਈ; k++ )
                ਖਾਨਾ_buf[k] = src[k*srcstep+i];

            ਲਈ( j = i; j <= size.ਚੌੜਾਈ - 4; j += 4 )
            {
#ਜੇ CV_SIMD128_64F
                ਜੇ (ਅੰਕੜੇ_Type_2<sT>::ਡੂੰਘਾਈ == CV_64F && ਅੰਕੜੇ_Type_2<dT>::ਡੂੰਘਾਈ == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ਅਚਲ ਦੁੱਗਣਾ *tsrc = (ਦੁੱਗਣਾ*)(src + j);

                    ਲਈ( k = 0; k < size.ਉਚਾਈ; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((ਦੁੱਗਣਾ)ਖਾਨਾ_buf[k]);
                        s0 = v_ਜੋੜੋ(s0, v_mul(a, v_ਲੱਦੋ(tsrc + 0)));
                        s1 = v_ਜੋੜੋ(s1, v_mul(a, v_ਲੱਦੋ(tsrc + 2)));
                    }

                    v_store((ਦੁੱਗਣਾ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ਦੁੱਗਣਾ*)(tdst+j+2), v_mul(s1, v_scale));
                } ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
                {
                    ਦੁੱਗਣਾ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ਅਚਲ sT *tsrc = src + j;

                    ਲਈ( k = 0; k < size.ਉਚਾਈ; k++, tsrc += srcstep )
                    {
                        ਦੁੱਗਣਾ a = ਖਾਨਾ_buf[k];
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

            ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            {
                ਦੁੱਗਣਾ s0 = 0;
                ਅਚਲ sT *tsrc = src + j;

                ਲਈ( k = 0; k < size.ਉਚਾਈ; k++, tsrc += srcstep )
                    s0 += (ਦੁੱਗਣਾ)ਖਾਨਾ_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    ਨਹੀਂਤਾਂ
        ਲਈ( i = 0; i < size.ਚੌੜਾਈ; i++, tdst += dststep )
        {
            ਜੇ( !delta_buf )
                ਲਈ( k = 0; k < size.ਉਚਾਈ; k++ )
                    ਖਾਨਾ_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            ਨਹੀਂਤਾਂ
                ਲਈ( k = 0; k < size.ਉਚਾਈ; k++ )
                    ਖਾਨਾ_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            ਲਈ( j = i; j <= size.ਚੌੜਾਈ - 4; j += 4 )
            {
#ਜੇ CV_SIMD128_64F
                ਜੇ (ਅੰਕੜੇ_Type_2<sT>::ਡੂੰਘਾਈ == CV_64F && ਅੰਕੜੇ_Type_2<dT>::ਡੂੰਘਾਈ == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ਅਚਲ ਦੁੱਗਣਾ *tsrc = (ਦੁੱਗਣਾ*)(src + j);
                    ਅਚਲ ਦੁੱਗਣਾ *d = (ਦੁੱਗਣਾ*)(delta_buf ? delta_buf : delta + j);

                    ਲਈ( k = 0; k < size.ਉਚਾਈ; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((ਦੁੱਗਣਾ)ਖਾਨਾ_buf[k]);
                        s0 = v_ਜੋੜੋ(s0, v_mul(a, v_sub(v_ਲੱਦੋ(tsrc + 0), v_ਲੱਦੋ(d + 0))));
                        s1 = v_ਜੋੜੋ(s1, v_mul(a, v_sub(v_ਲੱਦੋ(tsrc + 2), v_ਲੱਦੋ(d + 2))));
                    }

                    v_store((ਦੁੱਗਣਾ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((ਦੁੱਗਣਾ*)(tdst+j+2), v_mul(s1, v_scale));
                }
                ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ

                {
                    ਦੁੱਗਣਾ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ਅਚਲ sT *tsrc = src + j;
                    ਅਚਲ dT *d = delta_buf ? delta_buf : delta + j;

                    ਲਈ( k = 0; k < size.ਉਚਾਈ; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        ਦੁੱਗਣਾ a = ਖਾਨਾ_buf[k];
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

            ਲਈ( ; j < size.ਚੌੜਾਈ; j++ )
            {
                ਦੁੱਗਣਾ s0 = 0;
                ਅਚਲ sT *tsrc = src + j;
                ਅਚਲ dT *d = delta_buf ? delta_buf : delta + j;

                ਲਈ( k = 0; k < size.ਉਚਾਈ; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (ਦੁੱਗਣਾ)ਖਾਨਾ_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ sT, ਕਿਸਮ_ਨਾਂ dT> ਸਥਿਰ ਖਾਲੀ
MulTransposedL(ਅਚਲ Mat& srcmat, ਅਚਲ Mat& dstmat, ਅਚਲ Mat& deltamat, ਦੁੱਗਣਾ scale)
{
    ਪੂਰਨ i, j, k;
    ਅਚਲ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ਅਚਲ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.ਕਦਮ/ਆਕਾਰ(src[0]);
    size_t dststep = dstmat.ਕਦਮ/ਆਕਾਰ(dst[0]);
    size_t deltastep = deltamat.ਪੰਕਤੀਆਂ > 1 ? deltamat.ਕਦਮ/ਆਕਾਰ(delta[0]) : 0;
    ਪੂਰਨ delta_ਖਾਨੇ_ਗਿਣਤੀ = deltamat.ਖਾਨੇ_ਗਿਣਤੀ;
    ਮਾਪ size = srcmat.size();
    dT* tdst = dst;

    ਜੇ( !delta )
        ਲਈ( i = 0; i < size.ਉਚਾਈ; i++, tdst += dststep )
            ਲਈ( j = i; j < size.ਉਚਾਈ; j++ )
            {
                ਦੁੱਗਣਾ s = 0;
                ਅਚਲ sT *tsrc1 = src + i*srcstep;
                ਅਚਲ sT *tsrc2 = src + j*srcstep;
#ਜੇ CV_SIMD128_64F
                ਜੇ (ਅੰਕੜੇ_Type_2<sT>::ਡੂੰਘਾਈ == CV_64F && ਅੰਕੜੇ_Type_2<dT>::ਡੂੰਘਾਈ == CV_64F)
                {
                    ਅਚਲ ਦੁੱਗਣਾ *v_tsrc1 = (ਦੁੱਗਣਾ *)(tsrc1);
                    ਅਚਲ ਦੁੱਗਣਾ *v_tsrc2 = (ਦੁੱਗਣਾ *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    ਲਈ( k = 0; k <= size.ਚੌੜਾਈ - 4; k += 4 )
                        v_s = v_ਜੋੜੋ(v_s, v_ਜੋੜੋ(v_mul(v_ਲੱਦੋ(v_tsrc1 + k), v_ਲੱਦੋ(v_tsrc2 + k)), v_mul(v_ਲੱਦੋ(v_tsrc1 + k + 2), v_ਲੱਦੋ(v_tsrc2 + k + 2))));
                    s += v_reduce_ਜੋੜ(v_s);
                }
                ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
                {
                    ਲਈ( k = 0; k <= size.ਚੌੜਾਈ - 4; k += 4 )
                        s += (ਦੁੱਗਣਾ)tsrc1[k]*tsrc2[k] + (ਦੁੱਗਣਾ)tsrc1[k+1]*tsrc2[k+1] +
                             (ਦੁੱਗਣਾ)tsrc1[k+2]*tsrc2[k+2] + (ਦੁੱਗਣਾ)tsrc1[k+3]*tsrc2[k+3];
                }
                ਲਈ( ; k < size.ਚੌੜਾਈ; k++ )
                    s += (ਦੁੱਗਣਾ)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    ਨਹੀਂਤਾਂ
    {
        dT delta_buf[4];
        ਪੂਰਨ delta_ਖਿਸਕਾਓ = delta_ਖਾਨੇ_ਗਿਣਤੀ == size.ਚੌੜਾਈ ? 4 : 0;
        Auto_ਬਫਰ<uchar> buf(size.ਚੌੜਾਈ*ਆਕਾਰ(dT));
        dT* ਪੰਕਤੀ_buf = (dT*)buf.ਅੰਕੜੇ();

        ਲਈ( i = 0; i < size.ਉਚਾਈ; i++, tdst += dststep )
        {
            ਅਚਲ sT *tsrc1 = src + i*srcstep;
            ਅਚਲ dT *tdelta1 = delta + i*deltastep;

            ਜੇ( delta_ਖਾਨੇ_ਗਿਣਤੀ < size.ਚੌੜਾਈ )
                ਲਈ( k = 0; k < size.ਚੌੜਾਈ; k++ )
                    ਪੰਕਤੀ_buf[k] = tsrc1[k] - tdelta1[0];
            ਨਹੀਂਤਾਂ
                ਲਈ( k = 0; k < size.ਚੌੜਾਈ; k++ )
                    ਪੰਕਤੀ_buf[k] = tsrc1[k] - tdelta1[k];

            ਲਈ( j = i; j < size.ਉਚਾਈ; j++ )
            {
                ਦੁੱਗਣਾ s = 0;
                ਅਚਲ sT *tsrc2 = src + j*srcstep;
                ਅਚਲ dT *tdelta2 = delta + j*deltastep;
                ਜੇ( delta_ਖਾਨੇ_ਗਿਣਤੀ < size.ਚੌੜਾਈ )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#ਜੇ CV_SIMD128_64F
                ਜੇ (ਅੰਕੜੇ_Type_2<sT>::ਡੂੰਘਾਈ == CV_64F && ਅੰਕੜੇ_Type_2<dT>::ਡੂੰਘਾਈ == CV_64F)
                {
                    ਅਚਲ ਦੁੱਗਣਾ *v_tsrc2 = (ਦੁੱਗਣਾ *)(tsrc2);
                    ਅਚਲ ਦੁੱਗਣਾ *v_tdelta2 = (ਦੁੱਗਣਾ *)(tdelta2);
                    ਅਚਲ ਦੁੱਗਣਾ *v_ਪੰਕਤੀ_buf = (ਦੁੱਗਣਾ *)(ਪੰਕਤੀ_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    ਲਈ( k = 0; k <= size.ਚੌੜਾਈ - 4; k += 4, v_tdelta2 += delta_ਖਿਸਕਾਓ )
                        v_s = v_ਜੋੜੋ(v_s, v_ਜੋੜੋ(v_mul(v_sub(v_ਲੱਦੋ(v_tsrc2 + k), v_ਲੱਦੋ(v_tdelta2)), v_ਲੱਦੋ(v_ਪੰਕਤੀ_buf + k)), v_mul(v_sub(v_ਲੱਦੋ(v_tsrc2 + k + 2), v_ਲੱਦੋ(v_tdelta2 + 2)), v_ਲੱਦੋ(v_ਪੰਕਤੀ_buf + k + 2))));
                    s += v_reduce_ਜੋੜ(v_s);

                    tdelta2 = (ਅਚਲ dT *)(v_tdelta2);
                }
                ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
                {
                    ਲਈ( k = 0; k <= size.ਚੌੜਾਈ-4; k += 4, tdelta2 += delta_ਖਿਸਕਾਓ )
                        s += (ਦੁੱਗਣਾ)ਪੰਕਤੀ_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (ਦੁੱਗਣਾ)ਪੰਕਤੀ_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (ਦੁੱਗਣਾ)ਪੰਕਤੀ_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (ਦੁੱਗਣਾ)ਪੰਕਤੀ_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                ਲਈ( ; k < size.ਚੌੜਾਈ; k++, tdelta2++ )
                    s += (ਦੁੱਗਣਾ)ਪੰਕਤੀ_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(ਪੂਰਨ stype, ਪੂਰਨ dtype, ਤਾਰਕਿਕ ata)
{
    MulTransposedFunc func = ਨਿਰਾ;
    ਜੇ (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,ਦਸ਼ਮਲਵ>
                   : MulTransposedL<uchar,ਦਸ਼ਮਲਵ>;
    }
    ਨਹੀਂਤਾਂ ਜੇ (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,ਦੁੱਗਣਾ>
                   : MulTransposedL<uchar,ਦੁੱਗਣਾ>;
    }
    ਨਹੀਂਤਾਂ ਜੇ(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,ਦਸ਼ਮਲਵ>
                   : MulTransposedL<ushort,ਦਸ਼ਮਲਵ>;
    }
    ਨਹੀਂਤਾਂ ਜੇ(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,ਦੁੱਗਣਾ>
                   : MulTransposedL<ushort,ਦੁੱਗਣਾ>;
    }
    ਨਹੀਂਤਾਂ ਜੇ(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ਛੋਟਾ,ਦਸ਼ਮਲਵ>
                   : MulTransposedL<ਛੋਟਾ,ਦਸ਼ਮਲਵ>;
    }
    ਨਹੀਂਤਾਂ ਜੇ(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ਛੋਟਾ,ਦੁੱਗਣਾ>
                   : MulTransposedL<ਛੋਟਾ,ਦੁੱਗਣਾ>;
    }
    ਨਹੀਂਤਾਂ ਜੇ(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ਦਸ਼ਮਲਵ,ਦਸ਼ਮਲਵ>
                   : MulTransposedL<ਦਸ਼ਮਲਵ,ਦਸ਼ਮਲਵ>;
    }
    ਨਹੀਂਤਾਂ ਜੇ(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ਦਸ਼ਮਲਵ,ਦੁੱਗਣਾ>
                   : MulTransposedL<ਦਸ਼ਮਲਵ,ਦੁੱਗਣਾ>;
    }
    ਨਹੀਂਤਾਂ ਜੇ(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ>
                   : MulTransposedL<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ>;
    }
    CV_Assert(func && "Not supported");
    ਵਾਪਸ func;
}
#ਜੇ_ਅੰਤ // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਦੁੱਗਣਾ dotProd_(ਅਚਲ T* src1, ਅਚਲ T* src2, ਪੂਰਨ len)
{
    ਪੂਰਨ i = 0;
    ਦੁੱਗਣਾ ਨਤੀਜਾ = 0;

#ਜੇ CV_ENABLE_UNROLLED
    ਲਈ( ; i <= len - 4; i += 4 )
        ਨਤੀਜਾ += (ਦੁੱਗਣਾ)src1[i]*src2[i] + (ਦੁੱਗਣਾ)src1[i+1]*src2[i+1] +
            (ਦੁੱਗਣਾ)src1[i+2]*src2[i+2] + (ਦੁੱਗਣਾ)src1[i+3]*src2[i+3];
#ਜੇ_ਅੰਤ
    ਲਈ( ; i < len; i++ )
        ਨਤੀਜਾ += (ਦੁੱਗਣਾ)src1[i]*src2[i];

    ਵਾਪਸ ਨਤੀਜਾ;
}


ਦੁੱਗਣਾ dotProd_8u(ਅਚਲ uchar* src1, ਅਚਲ uchar* src2, ਪੂਰਨ len)
{
    ਦੁੱਗਣਾ r = 0;
    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਪੂਰਨ len0 = len & -VTraits<v_uint16>::vlanes(), ਖੰਡ_Size0 = (1 << 15), ਖੰਡ_ਮਾਪ_2;

    ਜਦੋਂ (i < len0)
    {
        ਖੰਡ_ਮਾਪ_2 = std::min(len0 - i, ਖੰਡ_Size0);
        v_uint32 v_ਜੋੜ = vx_setzero_u32();
        ਅਚਲ ਪੂਰਨ c_ਚੌੜਾਈ = VTraits<v_uint16>::vlanes();

        ਪੂਰਨ j = 0;
        ਲਈ (; j <= ਖੰਡ_ਮਾਪ_2 - c_ਚੌੜਾਈ * 2; j += c_ਚੌੜਾਈ * 2)
        {
            v_uint8 v_src1 = vx_ਲੱਦੋ(src1 + j);
            v_uint8 v_src2 = vx_ਲੱਦੋ(src2 + j);
            v_ਜੋੜ = v_dotprod_expand_fast(v_src1, v_src2, v_ਜੋੜ);
        }

        ਲਈ (; j <= ਖੰਡ_ਮਾਪ_2 - c_ਚੌੜਾਈ; j += c_ਚੌੜਾਈ)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_ਲੱਦੋ_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_ਲੱਦੋ_expand(src2 + j));
            v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (ਦੁੱਗਣਾ)v_reduce_ਜੋੜ(v_ਜੋੜ);

        src1 += ਖੰਡ_ਮਾਪ_2;
        src2 += ਖੰਡ_ਮਾਪ_2;
        i += ਖੰਡ_ਮਾਪ_2;
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ
    ਵਾਪਸ r + dotProd_(src1, src2, len - i);
}


ਦੁੱਗਣਾ dotProd_8s(ਅਚਲ schar* src1, ਅਚਲ schar* src2, ਪੂਰਨ len)
{
    ਦੁੱਗਣਾ r = 0.0;
    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਪੂਰਨ len0 = len & -VTraits<v_int16>::vlanes(), ਖੰਡ_Size0 = (1 << 14), ਖੰਡ_ਮਾਪ_2;

    ਜਦੋਂ (i < len0)
    {
        ਖੰਡ_ਮਾਪ_2 = std::min(len0 - i, ਖੰਡ_Size0);
        v_int32 v_ਜੋੜ = vx_setzero_s32();
        ਅਚਲ ਪੂਰਨ c_ਚੌੜਾਈ = VTraits<v_int16>::vlanes();

        ਪੂਰਨ j = 0;
        ਲਈ (; j <= ਖੰਡ_ਮਾਪ_2 - c_ਚੌੜਾਈ * 2; j += c_ਚੌੜਾਈ * 2)
        {
            v_int8 v_src1 = vx_ਲੱਦੋ(src1 + j);
            v_int8 v_src2 = vx_ਲੱਦੋ(src2 + j);
            v_ਜੋੜ = v_dotprod_expand_fast(v_src1, v_src2, v_ਜੋੜ);
        }

        ਲਈ (; j <= ਖੰਡ_ਮਾਪ_2 - c_ਚੌੜਾਈ; j += c_ਚੌੜਾਈ)
        {
            v_int16 v_src1 = vx_ਲੱਦੋ_expand(src1 + j);
            v_int16 v_src2 = vx_ਲੱਦੋ_expand(src2 + j);
            v_ਜੋੜ = v_dotprod_fast(v_src1, v_src2, v_ਜੋੜ);
        }
        r += (ਦੁੱਗਣਾ)v_reduce_ਜੋੜ(v_ਜੋੜ);

        src1 += ਖੰਡ_ਮਾਪ_2;
        src2 += ਖੰਡ_ਮਾਪ_2;
        i += ਖੰਡ_ਮਾਪ_2;
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ

    ਵਾਪਸ r + dotProd_(src1, src2, len - i);
}

ਦੁੱਗਣਾ dotProd_16u(ਅਚਲ ushort* src1, ਅਚਲ ushort* src2, ਪੂਰਨ len)
{
    ਦੁੱਗਣਾ r = 0.0;
    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਪੂਰਨ len0 = len & -VTraits<v_uint16>::vlanes(), ਖੰਡ_Size0 = (1 << 24), ਖੰਡ_ਮਾਪ_2;

    ਜਦੋਂ (i < len0)
    {
        ਖੰਡ_ਮਾਪ_2 = std::min(len0 - i, ਖੰਡ_Size0);
        v_uint64 v_ਜੋੜ = vx_setzero_u64();
        ਅਚਲ ਪੂਰਨ c_ਚੌੜਾਈ = VTraits<v_uint16>::vlanes();

        ਪੂਰਨ j = 0;
        ਲਈ (; j <= ਖੰਡ_ਮਾਪ_2 - c_ਚੌੜਾਈ; j += c_ਚੌੜਾਈ)
        {
            v_uint16 v_src1 = vx_ਲੱਦੋ(src1 + j);
            v_uint16 v_src2 = vx_ਲੱਦੋ(src2 + j);
            v_ਜੋੜ = v_dotprod_expand_fast(v_src1, v_src2, v_ਜੋੜ);
        }
        r += (ਦੁੱਗਣਾ)v_reduce_ਜੋੜ(v_ਜੋੜ);

        src1 += ਖੰਡ_ਮਾਪ_2;
        src2 += ਖੰਡ_ਮਾਪ_2;
        i += ਖੰਡ_ਮਾਪ_2;
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ
    ਵਾਪਸ r + dotProd_(src1, src2, len - i);
}

ਦੁੱਗਣਾ dotProd_16s(ਅਚਲ ਛੋਟਾ* src1, ਅਚਲ ਛੋਟਾ* src2, ਪੂਰਨ len)
{
    ਦੁੱਗਣਾ r = 0.0;
    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਪੂਰਨ len0 = len & -VTraits<v_int16>::vlanes(), ਖੰਡ_Size0 = (1 << 24), ਖੰਡ_ਮਾਪ_2;

    ਜਦੋਂ (i < len0)
    {
        ਖੰਡ_ਮਾਪ_2 = std::min(len0 - i, ਖੰਡ_Size0);
        v_int64 v_ਜੋੜ = vx_setzero_s64();
        ਅਚਲ ਪੂਰਨ c_ਚੌੜਾਈ = VTraits<v_int16>::vlanes();

        ਪੂਰਨ j = 0;
        ਲਈ (; j <= ਖੰਡ_ਮਾਪ_2 - c_ਚੌੜਾਈ; j += c_ਚੌੜਾਈ)
        {
            v_int16 v_src1 = vx_ਲੱਦੋ(src1 + j);
            v_int16 v_src2 = vx_ਲੱਦੋ(src2 + j);
            v_ਜੋੜ = v_dotprod_expand_fast(v_src1, v_src2, v_ਜੋੜ);
        }
        r += (ਦੁੱਗਣਾ)v_reduce_ਜੋੜ(v_ਜੋੜ);

        src1 += ਖੰਡ_ਮਾਪ_2;
        src2 += ਖੰਡ_ਮਾਪ_2;
        i += ਖੰਡ_ਮਾਪ_2;
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ
    ਵਾਪਸ r + dotProd_(src1, src2, len - i);
}

ਦੁੱਗਣਾ dotProd_32s(ਅਚਲ ਪੂਰਨ* src1, ਅਚਲ ਪੂਰਨ* src2, ਪੂਰਨ len)
{
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਦੁੱਗਣਾ r = .0;
    ਪੂਰਨ i = 0;
    ਅਚਲ ਪੂਰਨ ਕਦਮ  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#ਜੇ CV_SIMD_ਚੌੜਾਈ == 16
    ਅਚਲ ਪੂਰਨ wstep = ਕਦਮ * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    ਲਈ (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
    {
        v_int32 v_src10 = vx_ਲੱਦੋ(src1);
        v_int32 v_src20 = vx_ਲੱਦੋ(src2);
        v_int32 v_src11 = vx_ਲੱਦੋ(src1 + ਕਦਮ);
        v_int32 v_src21 = vx_ਲੱਦੋ(src2 + ਕਦਮ);
        v_sum0 = v_dotprod_expand_fast(v_src10, v_src20, v_sum0);
        v_sum1 = v_dotprod_expand_fast(v_src11, v_src21, v_sum1);
    }
    v_sum0 = v_ਜੋੜੋ(v_sum0, v_sum1);
#ਜੇ_ਅੰਤ
    ਲਈ (; i <= len - ਕਦਮ; i += ਕਦਮ, src1 += ਕਦਮ, src2 += ਕਦਮ)
    {
        v_int32 v_src1 = vx_ਲੱਦੋ(src1);
        v_int32 v_src2 = vx_ਲੱਦੋ(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_ਜੋੜ(v_sum0);
    vx_cleanup();
    ਵਾਪਸ r + dotProd_(src1, src2, len - i);
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ dotProd_(src1, src2, len);
#ਜੇ_ਅੰਤ
}

ਦੁੱਗਣਾ dotProd_32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, ਅਚਲ ਦਸ਼ਮਲਵ* src2, ਪੂਰਨ len)
{
    ਦੁੱਗਣਾ r = 0.0;
    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਪੂਰਨ len0 = len & -VTraits<v_float32>::vlanes(), ਖੰਡ_Size0 = (1 << 13), ਖੰਡ_ਮਾਪ_2;

    ਜਦੋਂ (i < len0)
    {
        ਖੰਡ_ਮਾਪ_2 = std::min(len0 - i, ਖੰਡ_Size0);
        v_float32 v_ਜੋੜ = vx_setzero_f32();

        ਪੂਰਨ j = 0;
        ਪੂਰਨ c_ਚੌੜਾਈ = VTraits<v_float32>::vlanes();

#ਜੇ CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        ਲਈ (; j <= ਖੰਡ_ਮਾਪ_2 - (c_ਚੌੜਾਈ * 4); j += (c_ਚੌੜਾਈ * 4))
        {
            v_ਜੋੜ  = v_muladd(vx_ਲੱਦੋ(src1 + j),
                              vx_ਲੱਦੋ(src2 + j), v_ਜੋੜ);
            v_sum1 = v_muladd(vx_ਲੱਦੋ(src1 + j + c_ਚੌੜਾਈ),
                              vx_ਲੱਦੋ(src2 + j + c_ਚੌੜਾਈ), v_sum1);
            v_sum2 = v_muladd(vx_ਲੱਦੋ(src1 + j + (c_ਚੌੜਾਈ * 2)),
                              vx_ਲੱਦੋ(src2 + j + (c_ਚੌੜਾਈ * 2)), v_sum2);
            v_sum3 = v_muladd(vx_ਲੱਦੋ(src1 + j + (c_ਚੌੜਾਈ * 3)),
                              vx_ਲੱਦੋ(src2 + j + (c_ਚੌੜਾਈ * 3)), v_sum3);
        }

        v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_ਜੋੜੋ(v_ਜੋੜੋ(v_sum1, v_sum2), v_sum3));
#ਜੇ_ਅੰਤ

        ਲਈ (; j <= ਖੰਡ_ਮਾਪ_2 - c_ਚੌੜਾਈ; j += c_ਚੌੜਾਈ)
            v_ਜੋੜ = v_muladd(vx_ਲੱਦੋ(src1 + j), vx_ਲੱਦੋ(src2 + j), v_ਜੋੜ);

        r += v_reduce_ਜੋੜ(v_ਜੋੜ);

        src1 += ਖੰਡ_ਮਾਪ_2;
        src2 += ਖੰਡ_ਮਾਪ_2;
        i += ਖੰਡ_ਮਾਪ_2;
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ
    ਵਾਪਸ r + dotProd_(src1, src2, len - i);
}

ਦੁੱਗਣਾ dotProd_64f(ਅਚਲ ਦੁੱਗਣਾ* src1, ਅਚਲ ਦੁੱਗਣਾ* src2, ਪੂਰਨ len)
{
    ਵਾਪਸ dotProd_(src1, src2, len);
}

#ਜੇ_ਅੰਤ
CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace
