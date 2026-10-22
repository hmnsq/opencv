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

#include "precomp.hpp"

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
#ᨀᨘ defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#ᨀᨘᨁ
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

namespace cv {

// forward declarations
typedef void (*TransformFunc)(const void* src, void* dst, const void* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn);
typedef void (*ScaleAddFunc)(const void* src1, const void* src2, void* dst, puur_naa_nka len, const void* alpha);
typedef void (*MulTransposedFunc)(const Mat& src, const/*preallocated*/ Mat& dst, const Mat& delta, dviguu_na scale);
typedef dviguu_na (*MahalanobisImplFunc)(const Mat& v1, const Mat& v2, const Mat& icovar, dviguu_na *diff_buffer /*[len]*/, puur_naa_nka len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

void gemm32f(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
             float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
             puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags);
void gemm64f(const dviguu_na* src1, size_t src1_step, const dviguu_na* src2, size_t src2_step,
             dviguu_na alpha, const dviguu_na* src3, size_t src3_step, dviguu_na beta, dviguu_na* dst, size_t dst_step,
             puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags);
void gemm32fc(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
              float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
              puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags);
void gemm64fc(const dviguu_na* src1, size_t src1_step, const dviguu_na* src2, size_t src2_step,
              dviguu_na alpha, const dviguu_na* src3, size_t src3_step, dviguu_na beta, dviguu_na* dst, size_t dst_step,
              puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags);

TransformFunc getTransformFunc(puur_naa_nka depth);
TransformFunc getDiagTransformFunc(puur_naa_nka depth);
TransformFunc getPerspectiveTransform(puur_naa_nka depth);

ScaleAddFunc getScaleAddFunc(puur_naa_nka depth);

MahalanobisImplFunc getMahalanobisImplFunc(puur_naa_nka depth);

MulTransposedFunc getMulTransposedFunc(puur_naa_nka stype, puur_naa_nka dtype, bool ata);

dviguu_na dotProd_8u(const uchar* src1, const uchar* src2, puur_naa_nka len);
dviguu_na dotProd_8s(const schar* src1, const schar* src2, puur_naa_nka len);
dviguu_na dotProd_16u(const ushort* src1, const ushort* src2, puur_naa_nka len);
dviguu_na dotProd_16s(const short* src1, const short* src2, puur_naa_nka len);
dviguu_na dotProd_32s(const puur_naa_nka* src1, const puur_naa_nka* src2, puur_naa_nka len);
dviguu_na dotProd_32f(const float* src1, const float* src2, puur_naa_nka len);
dviguu_na dotProd_64f(const dviguu_na* src1, const dviguu_na* src2, puur_naa_nka len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#ᨀᨘ !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

static void
GEMM_CopyBlock( const uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    puur_naa_nka j;
    size.width *= (puur_naa_nka)(pix_size / parimaa_na(puur_naa_nka));

    for( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #ᨀᨘ CV_ENABLE_UNROLLED
        for( ; j <= size.width - 4; j += 4 )
        {
            puur_naa_nka t0 = ((const puur_naa_nka*)src)[j];
            puur_naa_nka t1 = ((const puur_naa_nka*)src)[j+1];
            ((puur_naa_nka*)dst)[j] = t0;
            ((puur_naa_nka*)dst)[j+1] = t1;
            t0 = ((const puur_naa_nka*)src)[j+2];
            t1 = ((const puur_naa_nka*)src)[j+3];
            ((puur_naa_nka*)dst)[j+2] = t0;
            ((puur_naa_nka*)dst)[j+3] = t1;
        }
        #endif
        for( ; j < size.width; j++ )
            ((puur_naa_nka*)dst)[j] = ((const puur_naa_nka*)src)[j];
    }
}


static void
GEMM_TransposeBlock( const uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    puur_naa_nka i, j;
    for( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        const uchar* _src = src;
        switch( pix_size )
        {
        pak_sa parimaa_na(puur_naa_nka):
            for( j = 0; j < size.height; j++, _src += src_step )
                ((puur_naa_nka*)dst)[j] = ((puur_naa_nka*)_src)[0];
            bha_nga;
        pak_sa parimaa_na(puur_naa_nka)*2:
            for( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                puur_naa_nka t0 = ((puur_naa_nka*)_src)[0];
                puur_naa_nka t1 = ((puur_naa_nka*)_src)[1];
                ((puur_naa_nka*)dst)[j] = t0;
                ((puur_naa_nka*)dst)[j+1] = t1;
            }
            bha_nga;
        pak_sa parimaa_na(puur_naa_nka)*4:
            for( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                puur_naa_nka t0 = ((puur_naa_nka*)_src)[0];
                puur_naa_nka t1 = ((puur_naa_nka*)_src)[1];
                ((puur_naa_nka*)dst)[j] = t0;
                ((puur_naa_nka*)dst)[j+1] = t1;
                t0 = ((puur_naa_nka*)_src)[2];
                t1 = ((puur_naa_nka*)_src)[3];
                ((puur_naa_nka*)dst)[j+2] = t0;
                ((puur_naa_nka*)dst)[j+3] = t1;
            }
            bha_nga;
        default:
            CV_Assert(0);
            ᨀᨘᨒ;
        }
    }
}

#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> sa_nracanaa VecTraits;

template<> sa_nracanaa VecTraits<float> {
    typedef v_float32 VT;
    static anta_hstha VT zero() { ᨀᨘᨒ vx_setzero_f32(); }
    static anta_hstha VT load(const float* p) { ᨀᨘᨒ vx_load(p); }
    static anta_hstha VT setall(float v) { ᨀᨘᨒ vx_setall_f32(v); }
};
#endif

#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> sa_nracanaa VecTraits<dviguu_na> {
    typedef v_float64 VT;
    static anta_hstha VT zero() { ᨀᨘᨒ vx_setzero_f64(); }
    static anta_hstha VT load(const dviguu_na* p) { ᨀᨘᨒ vx_load(p); }
    static anta_hstha VT setall(dviguu_na v) { ᨀᨘᨒ vx_setall_f64(v); }
};
#endif

#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
static anta_hstha ST simdDotProduct(const ST* a, const ST* b, puur_naa_nka n, puur_naa_nka& k)
{
    typedef typename VecTraits<ST>::VT VecT;
    const puur_naa_nka vlanes = VTraits<VecT>::vlanes();
    const puur_naa_nka stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    for( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    for( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    ᨀᨘᨒ v_reduce_sum(vs0);
}
#endif

#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
static anta_hstha dviguu_na simdDotProduct_f32f64(const float* a, const float* b, puur_naa_nka n, puur_naa_nka& k)
{
    const puur_naa_nka vlanes_f32 = VTraits<v_float32>::vlanes();
    const puur_naa_nka stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    for( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_high(va0), v_cvt_f64_high(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_high(va1), v_cvt_f64_high(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    for( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_high(va), v_cvt_f64_high(vb), vs0);
    }
    ᨀᨘᨒ v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
static anta_hstha void simdGEMM_kj_f32f64(
    const float* a_data, const float* _b_data, size_t b_step,
    const float* _c_data, size_t c_step1,
    float* d_data,
    puur_naa_nka n, puur_naa_nka m,
    dviguu_na alpha, dviguu_na beta,
    puur_naa_nka& j)
{
    const puur_naa_nka vlanes_f32 = VTraits<v_float32>::vlanes();
    const puur_naa_nka vlanes_f64 = VTraits<v_float64>::vlanes();

    ᨀᨘ( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        const puur_naa_nka max_m = 1600 / (puur_naa_nka)parimaa_na(float);
        cv::AutoBuffer<dviguu_na> _s_buf(max_m);
        dviguu_na* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            for( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            for( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            const float* b_ptr = _b_data;
            for( puur_naa_nka k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((dviguu_na)a_data[k]);
                j = 0;
                for( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_high(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                dviguu_na a_val = (dviguu_na)a_data[k];
                for( ; j < m; j++ )
                    s_buf[j] += a_val * (dviguu_na)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            const float* c_data = _c_data;
            j = 0;
            ᨀᨘ( !_c_data )
            {
                for( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                for( ; j < m; j++ )
                    d_data[j] = (float)(s_buf[j] * alpha);
            }
            ᨀᨘᨁ ᨀᨘ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                for( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_high(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                for( ; j < m; j++ )
                    d_data[j] = (float)(s_buf[j] * alpha + (dviguu_na)c_data[j] * beta);
            }
            ᨀᨘᨁ
            {
                for( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (float)(s_buf[j] * alpha + (dviguu_na)c_data[0] * beta);
            }
        }
        j = m;
    }
    ᨀᨘᨁ ᨀᨘ( vlanes_f32 * (puur_naa_nka)parimaa_na(float) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        const float* c_data = _c_data;
        for( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            const float* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            for( puur_naa_nka k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((dviguu_na)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_high(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            ᨀᨘ( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ᨀᨘᨁ ᨀᨘ( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_high(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ᨀᨘᨁ
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) float buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                for( puur_naa_nka jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (float)beta;
            }
        }
    }
}
#endif

#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
static anta_hstha void simdGEMM_kj(
    const ST* a_data, const ST* _b_data, size_t b_step,
    const ST* _c_data, size_t c_step1,
    ST* d_data,
    puur_naa_nka n, puur_naa_nka m,
    dviguu_na alpha, dviguu_na beta,
    puur_naa_nka& j)
{
    typedef typename VecTraits<ST>::VT VecT;
    const puur_naa_nka vlanes = VTraits<VecT>::vlanes();

    ᨀᨘ( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        const puur_naa_nka max_m = 1600 / (puur_naa_nka)parimaa_na(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            for( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            for( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            const ST* b_ptr = _b_data;
            for( puur_naa_nka k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                for( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                for( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            const ST* c_data = _c_data;
            j = 0;
            ᨀᨘ( !_c_data )
            {
                for( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                for( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            ᨀᨘᨁ ᨀᨘ( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                for( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                for( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            ᨀᨘᨁ
            {
                for( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    ᨀᨘᨁ ᨀᨘ( vlanes * (puur_naa_nka)parimaa_na(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        const ST* c_data = _c_data;
        for( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            const ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            for( puur_naa_nka k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            ᨀᨘ( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            ᨀᨘᨁ ᨀᨘ( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            ᨀᨘᨁ
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                for( puur_naa_nka jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> static void
GEMMSingleMul( const T* a_data, size_t a_step,
               const T* b_data, size_t b_step,
               const T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    puur_naa_nka i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    const T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= parimaa_na(a_data[0]);
    b_step /= parimaa_na(b_data[0]);
    c_step /= parimaa_na(c_data[0]);
    d_step /= parimaa_na(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    ᨀᨘ( !c_data )
        c_step0 = c_step1 = 0;
    ᨀᨘᨁ ᨀᨘ( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    ᨀᨘᨁ
        c_step0 = 1, c_step1 = c_step;

    ᨀᨘ( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        ᨀᨘ( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    ᨀᨘ( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        ᨀᨘ( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            for( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        ᨀᨘ( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            for( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        for( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            for( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                ᨀᨘ( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                ᨀᨘᨁ
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            for( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                ᨀᨘ( !c_data )
                    d_data[j] = T(s0);
                ᨀᨘᨁ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ᨀᨘᨁ ᨀᨘ( flags & GEMM_2_T ) /* A * Bt */
    {
        for( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            ᨀᨘ( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#ᨀᨘ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ᨀᨘ( parimaa_na(WT) == parimaa_na(dviguu_na) )
                {
                    ᨀᨘ( parimaa_na(T) == parimaa_na(dviguu_na) )
                        s0 = (WT)simdDotProduct((const dviguu_na*)a_data, (const dviguu_na*)b_data, n, k);
                    ᨀᨘᨁ ᨀᨘ( parimaa_na(T) == parimaa_na(float) )
                        s0 = (WT)simdDotProduct_f32f64((const float*)a_data, (const float*)b_data, n, k);
                }
                ᨀᨘᨁ
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #ᨀᨘ CV_ENABLE_UNROLLED
                    for( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                for( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                ᨀᨘ( !c_data )
                    d_data[j] = T(s0);
                ᨀᨘᨁ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ᨀᨘᨁ ᨀᨘ( d_size.width*parimaa_na(d_data[0]) <= 1600 )
    {
        for( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            ᨀᨘ( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ᨀᨘ( parimaa_na(WT) == parimaa_na(dviguu_na) )
            {
                ᨀᨘ( parimaa_na(T) == parimaa_na(dviguu_na) )
                    simdGEMM_kj((const dviguu_na*)a_data, (const dviguu_na*)_b_data, b_step,
                                        (const dviguu_na*)_c_data, c_step1,
                                (dviguu_na*)d_data, n, m, alpha, beta, j);
                ᨀᨘᨁ ᨀᨘ( parimaa_na(T) == parimaa_na(float) )
                    simdGEMM_kj_f32f64((const float*)a_data, (const float*)_b_data, b_step,
                                        (const float*)_c_data, c_step1,
                                (float*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            for( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                const T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                for( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                ᨀᨘ( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                ᨀᨘᨁ
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            for( ; j < m; j++, c_data += c_step1 )
            {
                const T* b = _b_data + j;
                WT s0(0);

                for( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                ᨀᨘ( !c_data )
                    d_data[j] = T(s0);
                ᨀᨘᨁ
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    ᨀᨘᨁ
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        for( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            ᨀᨘ( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            for( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                ᨀᨘ( parimaa_na(WT) == parimaa_na(dviguu_na) )
                {
                    ᨀᨘ( parimaa_na(T) == parimaa_na(dviguu_na) )
                    {
                        const puur_naa_nka vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((const dviguu_na*)&al));
                        for( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((const dviguu_na*)(d_buf + j));
                            v_float64 vb = vx_load((const dviguu_na*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((dviguu_na*)(d_buf + j), vd);
                        }
                    }
                    ᨀᨘᨁ
                    {
                        const puur_naa_nka vlanes_f32 = VTraits<v_float32>::vlanes();
                        const puur_naa_nka vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((const dviguu_na*)&al));
                        for( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((const float*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_high(vb);
                            v_float64 vd_lo = vx_load((const dviguu_na*)(d_buf + j));
                            v_float64 vd_hi = vx_load((const dviguu_na*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((dviguu_na*)(d_buf + j), vd_lo);
                            v_store((dviguu_na*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                for( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            ᨀᨘ( !c_data )
                for( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            ᨀᨘᨁ
                for( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
static anta_hstha void setDoubleDataZero(WT* d_data, puur_naa_nka m, puur_naa_nka vlanes)
{
    v_float64 vz = vx_setzero_f64();
    puur_naa_nka j = 0;
    for( ; j <= m - vlanes; j += vlanes )
        v_store((dviguu_na*)(d_data + j), vz);
    for( ; j < m; j++ )
        d_data[j] = WT(0);
}

template<typename ST>
static anta_hstha void simdMulAdd(const ST* b, dviguu_na* d, puur_naa_nka m, dviguu_na a_val);

template<>
anta_hstha void simdMulAdd<float>(
    const float* b, dviguu_na* d, puur_naa_nka m, dviguu_na a_val)
{
    const puur_naa_nka vlanes_f32 = VTraits<v_float32>::vlanes();
    const puur_naa_nka vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    puur_naa_nka j = 0;
    for( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_high(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    for( ; j < m; j++ )
        d[j] += a_val * (dviguu_na)b[j];
}

template<>
anta_hstha void simdMulAdd<dviguu_na>(
    const dviguu_na* b, dviguu_na* d, puur_naa_nka m, dviguu_na a_val)
{
    const puur_naa_nka vlanes = VTraits<v_float64>::vlanes();
    const puur_naa_nka stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    puur_naa_nka j = 0;
    for( ; j <= m - stride2; j += stride2 )
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
    for( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    for( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
static anta_hstha void simdBlockMul_kj(
    const ST* a_data, const ST* b_data, size_t b_step,
    dviguu_na* d_data, puur_naa_nka n, puur_naa_nka m, bool do_acc)
{
    const puur_naa_nka vlanes = VTraits<v_float64>::vlanes();
    ᨀᨘ( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    for( puur_naa_nka k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (dviguu_na)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> static void
GEMMBlockMul( const T* a_data, size_t a_step,
              const T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, puur_naa_nka flags )
{
    puur_naa_nka i, j, k, n = a_size.width, m = d_size.width;
    const T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    puur_naa_nka do_acc = flags & 16;

    a_step /= parimaa_na(a_data[0]);
    b_step /= parimaa_na(b_data[0]);
    d_step /= parimaa_na(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    ᨀᨘ( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    ᨀᨘ( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        for( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            ᨀᨘ( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#ᨀᨘ CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                ᨀᨘ( parimaa_na(WT) == parimaa_na(dviguu_na) )
                {
                    ᨀᨘ( parimaa_na(T) == parimaa_na(dviguu_na) )
                        s0 += (WT)simdDotProduct((const dviguu_na*)a_data, (const dviguu_na*)b_data, n, k);
                    ᨀᨘᨁ ᨀᨘ( parimaa_na(T) == parimaa_na(float) )
                        s0 += (WT)simdDotProduct_f32f64((const float*)a_data, (const float*)b_data, n, k);
                }
                ᨀᨘᨁ
#endif
                {
                    WT s1(0);
                    for( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                for( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    ᨀᨘᨁ
    {
        for( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            ᨀᨘ( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            ᨀᨘ( parimaa_na(WT) == parimaa_na(dviguu_na) )
            {
                ᨀᨘ( parimaa_na(T) == parimaa_na(dviguu_na) )
                    simdBlockMul_kj(
                        (const dviguu_na*)a_data, (const dviguu_na*)b_data, b_step,
                        (dviguu_na*)d_data, n, m, do_acc != 0);
                ᨀᨘᨁ
                    simdBlockMul_kj(
                        (const float*)a_data, (const float*)b_data, b_step,
                        (dviguu_na*)d_data, n, m, do_acc != 0);
            }
            ᨀᨘᨁ
#endif
            {
                for( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    const T* b = b_data + j;

                    ᨀᨘ( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    ᨀᨘᨁ
                        s0 = s1 = s2 = s3 = WT(0);

                    for( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                for( ; j < m; j++ )
                {
                    const T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    for( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> static void
GEMMStore( const T* c_data, size_t c_step,
           const WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    const T* _c_data = c_data;
    puur_naa_nka j;
    size_t c_step0, c_step1;

    c_step /= parimaa_na(c_data[0]);
    d_buf_step /= parimaa_na(d_buf[0]);
    d_step /= parimaa_na(d_data[0]);

    ᨀᨘ( !c_data )
        c_step0 = c_step1 = 0;
    ᨀᨘᨁ ᨀᨘ( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    ᨀᨘᨁ
        c_step0 = 1, c_step1 = c_step;

    for( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        ᨀᨘ( _c_data )
        {
            c_data = _c_data;
            j=0;
             #ᨀᨘ CV_ENABLE_UNROLLED
            for(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
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
            for( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        ᨀᨘᨁ
        {
            j = 0;
             #ᨀᨘ CV_ENABLE_UNROLLED
            for( ; j <= d_size.width - 4; j += 4 )
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
            for( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


typedef void (*GEMMSingleMulFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, const void* src3, size_t step3,
                   void* dst, size_t dststep, Size srcsize, Size dstsize,
                   dviguu_na alpha, dviguu_na beta, puur_naa_nka flags );

typedef void (*GEMMBlockMulFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   Size srcsize, Size dstsize, puur_naa_nka flags );

typedef void (*GEMMStoreFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   Size dstsize, dviguu_na alpha, dviguu_na beta, puur_naa_nka flags );

template<typename T, typename WT, void (*fn)(const T*, size_t, const T*, size_t, const T*, size_t, T*, size_t, Size, Size, dviguu_na, dviguu_na, puur_naa_nka)>
static void gemmSingleMulWrap( const void* src1, size_t step1,
                   const void* src2, size_t step2, const void* src3, size_t step3,
                   void* dst, size_t dststep, Size srcsize, Size dstsize,
                   dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    fn((const T*)src1, step1, (const T*)src2, step2, (const T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, void (*fn)(const T*, size_t, const T*, size_t, WT*, size_t, Size, Size, puur_naa_nka)>
static void gemmBlockMulWrap( const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   Size srcsize, Size dstsize, puur_naa_nka flags )
{
    fn((const T*)src1, step1, (const T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, void (*fn)(const T*, size_t, const WT*, size_t, T*, size_t, Size, dviguu_na, dviguu_na, puur_naa_nka)>
static void gemmStoreWrap( const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   Size dstsize, dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    fn((const T*)src1, step1, (const WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

static void GEMMSingleMul_32f( const float* a_data, size_t a_step,
              const float* b_data, size_t b_step,
              const float* c_data, size_t c_step,
              float* d_data, size_t d_step,
              Size a_size, Size d_size,
              dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    GEMMSingleMul<float,dviguu_na>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

static void GEMMSingleMul_64f( const dviguu_na* a_data, size_t a_step,
                              const dviguu_na* b_data, size_t b_step,
                              const dviguu_na* c_data, size_t c_step,
                              dviguu_na* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    GEMMSingleMul<dviguu_na,dviguu_na>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


static void GEMMSingleMul_32fc( const Complexf* a_data, size_t a_step,
                              const Complexf* b_data, size_t b_step,
                              const Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

static void GEMMSingleMul_64fc( const Complexd* a_data, size_t a_step,
                              const Complexd* b_data, size_t b_step,
                              const Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

static void GEMMBlockMul_32f( const float* a_data, size_t a_step,
             const float* b_data, size_t b_step,
             dviguu_na* d_data, size_t d_step,
             Size a_size, Size d_size, puur_naa_nka flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_64f( const dviguu_na* a_data, size_t a_step,
                             const dviguu_na* b_data, size_t b_step,
                             dviguu_na* d_data, size_t d_step,
                             Size a_size, Size d_size, puur_naa_nka flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_32fc( const Complexf* a_data, size_t a_step,
                             const Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, puur_naa_nka flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_64fc( const Complexd* a_data, size_t a_step,
                             const Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, puur_naa_nka flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMStore_32f( const float* c_data, size_t c_step,
          const dviguu_na* d_buf, size_t d_buf_step,
          float* d_data, size_t d_step, Size d_size,
          dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_64f( const dviguu_na* c_data, size_t c_step,
                      const dviguu_na* d_buf, size_t d_buf_step,
                      dviguu_na* d_data, size_t d_step, Size d_size,
                      dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_32fc( const Complexf* c_data, size_t c_step,
                          const Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_64fc( const Complexd* c_data, size_t c_step,
                          const Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          dviguu_na alpha, dviguu_na beta, puur_naa_nka flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

static void gemmImpl( Mat A, Mat B, dviguu_na alpha,
           Mat C, dviguu_na beta, Mat D, puur_naa_nka flags )
{
    CV_INSTRUMENT_REGION();

    const puur_naa_nka block_lin_size = 128;
    const puur_naa_nka block_size = block_lin_size * block_lin_size;

    static dviguu_na zero[] = {0,0,0,0};
    static float zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    puur_naa_nka i, len = 0, type = A.type();

    switch( flags & (GEMM_1_T|GEMM_2_T) )
    {
    pak_sa 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        bha_nga;
    pak_sa 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        bha_nga;
    pak_sa 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        bha_nga;
    pak_sa 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        bha_nga;
    }

    ᨀᨘ( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        ᨀᨘ( type == CV_32F )
        {
            float* d = D.ptr<float>();
            const float *a = A.ptr<float>(),
                        *b = B.ptr<float>(),
                        *c = (const float*)C.data;
            size_t d_step = D.step/parimaa_na(d[0]),
                a_step = A.step/parimaa_na(a[0]),
                b_step = B.step/parimaa_na(b[0]),
                c_step = C.data ? C.step/parimaa_na(c[0]) : 0;

            ᨀᨘ( !c )
                c = zerof;

            switch( len )
            {
            pak_sa 2:
                ᨀᨘ( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                    }
                }
                ᨀᨘᨁ ᨀᨘ( a != d )
                {
                    puur_naa_nka c_step0 = 1;
                    ᨀᨘ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                    }
                }
                ᨀᨘᨁ
                    bha_nga;
                ᨀᨘᨒ;
            pak_sa 3:
                ᨀᨘ( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        float t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                        d[2] = (float)(t2*alpha + c[2]*beta);
                    }
                }
                ᨀᨘᨁ ᨀᨘ( a != d )
                {
                    puur_naa_nka c_step0 = 1;
                    ᨀᨘ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        float t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (float)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                ᨀᨘᨁ
                    bha_nga;
                ᨀᨘᨒ;
            pak_sa 4:
                ᨀᨘ( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        float t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        float t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                        d[2] = (float)(t2*alpha + c[2]*beta);
                        d[3] = (float)(t3*alpha + c[3]*beta);
                    }
                }
                ᨀᨘᨁ ᨀᨘ( len <= 16 && a != d )
                {
                    puur_naa_nka c_step0 = 1;
                    ᨀᨘ( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        float t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        float t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (float)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (float)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                ᨀᨘᨁ
                    bha_nga;
                ᨀᨘᨒ;
            }
        }

        ᨀᨘ( type == CV_64F )
        {
            dviguu_na* d = D.ptr<dviguu_na>();
            const dviguu_na *a = A.ptr<dviguu_na>(),
                         *b = B.ptr<dviguu_na>(),
                         *c = (const dviguu_na*)C.data;
            size_t d_step = D.step/parimaa_na(d[0]),
                a_step = A.step/parimaa_na(a[0]),
                b_step = B.step/parimaa_na(b[0]),
                c_step = C.data ? C.step/parimaa_na(c[0]) : 0;
            ᨀᨘ( !c )
                c = zero;

            switch( len )
            {
            pak_sa 2:
                ᨀᨘ( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        dviguu_na t0 = a[0]*b[0] + a[1]*b[b_step];
                        dviguu_na t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                ᨀᨘᨁ ᨀᨘ( a != d )
                {
                    puur_naa_nka c_step0 = 1;
                    ᨀᨘ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        dviguu_na t0 = a[0]*b[0] + a[1]*b[b_step];
                        dviguu_na t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                ᨀᨘᨁ
                    bha_nga;
                ᨀᨘᨒ;
            pak_sa 3:
                ᨀᨘ( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        dviguu_na t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        dviguu_na t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        dviguu_na t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                ᨀᨘᨁ ᨀᨘ( a != d )
                {
                    puur_naa_nka c_step0 = 1;
                    ᨀᨘ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        dviguu_na t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        dviguu_na t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        dviguu_na t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                ᨀᨘᨁ
                    bha_nga;
                ᨀᨘᨒ;
            pak_sa 4:
                ᨀᨘ( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        dviguu_na t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        dviguu_na t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        dviguu_na t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        dviguu_na t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                ᨀᨘᨁ ᨀᨘ( d_size.width <= 16 && a != d )
                {
                    puur_naa_nka c_step0 = 1;
                    ᨀᨘ( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        dviguu_na t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        dviguu_na t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        dviguu_na t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        dviguu_na t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                ᨀᨘᨁ
                    bha_nga;
                ᨀᨘᨒ;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    const uchar* Cdata = C.data;
    size_t Cstep = C.data ? (size_t)C.step : 0;
    AutoBuffer<uchar> buf;

    ᨀᨘ( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<float, dviguu_na, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<float, dviguu_na, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<float, dviguu_na, GEMMStore_32f>;
    }
    ᨀᨘᨁ ᨀᨘ( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<dviguu_na, dviguu_na, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<dviguu_na, dviguu_na, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<dviguu_na, dviguu_na, GEMMStore_64f>;
    }
    ᨀᨘᨁ ᨀᨘ( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    ᨀᨘᨁ
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    ᨀᨘ( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
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
    else*/ ᨀᨘ( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    ᨀᨘᨁ
    {
        puur_naa_nka is_a_t = flags & GEMM_1_T;
        puur_naa_nka is_b_t = flags & GEMM_2_T;
        puur_naa_nka elem_size = CV_ELEM_SIZE(type);
        puur_naa_nka dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        puur_naa_nka j, k, di = 0, dj = 0, dk = 0;
        puur_naa_nka dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        puur_naa_nka work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        ᨀᨘ( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        ᨀᨘᨁ
            a_step0 = elem_size, a_step1 = A.step;

        ᨀᨘ( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        ᨀᨘᨁ
            b_step0 = elem_size, b_step1 = b_step;

        ᨀᨘ( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        ᨀᨘᨁ ᨀᨘ( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        ᨀᨘᨁ
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        ᨀᨘ( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        ᨀᨘ( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        ᨀᨘ( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        ᨀᨘ( is_a_t )
            a_buf = b_buf + b_buf_size;

        for( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            ᨀᨘ( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            for( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                const uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                ᨀᨘ( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                ᨀᨘ( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                for( k = 0; k < len; k += dk )
                {
                    const uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    const uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    ᨀᨘ( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    ᨀᨘ( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    ᨀᨘᨁ
                        a_bl_size.width = di, a_bl_size.height = dk;

                    ᨀᨘ( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    ᨀᨘ( dj < d_size.width )
                    {
                        Size b_size;
                        ᨀᨘ( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        ᨀᨘᨁ
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    ᨀᨘ( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    ᨀᨘᨁ
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                ᨀᨘ( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>anta_hstha static void
callGemmImpl(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
          const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags, puur_naa_nka type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    puur_naa_nka b_m, b_n, c_m, c_n, m_d;

    ᨀᨘ(flags & GEMM_2_T)
    {
        b_m = n_d;
        ᨀᨘ(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        ᨀᨘᨁ
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    ᨀᨘᨁ
    {
        b_n = n_d;
        ᨀᨘ(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        ᨀᨘᨁ
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    ᨀᨘ(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    ᨀᨘᨁ
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    ᨀᨘ(src1 != NULL)
        A = Mat(m_a, n_a, type, (void*)src1, src1_step);
    ᨀᨘ(src2 != NULL)
        B = Mat(b_m, b_n, type, (void*)src2, src2_step);
    ᨀᨘ(src3 != NULL && beta != 0.0)
        C = Mat(c_m, c_n, type, (void*)src3, src3_step);
    Mat D(m_d, n_d, type, (void*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

void gemm32f(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
             float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
             puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

void gemm64f(const dviguu_na* src1, size_t src1_step, const dviguu_na* src2, size_t src2_step,
             dviguu_na alpha, const dviguu_na* src3, size_t src3_step, dviguu_na beta, dviguu_na* dst, size_t dst_step,
             puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

void gemm32fc(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
              float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
              puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

void gemm64fc(const dviguu_na* src1, size_t src1_step, const dviguu_na* src2, size_t src2_step,
              dviguu_na alpha, const dviguu_na* src3, size_t src3_step, dviguu_na beta, dviguu_na* dst, size_t dst_step,
              puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> static void
transform_( const T* src, T* dst, const WT* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn )
{
    puur_naa_nka x;

    ᨀᨘ( scn == 2 && dcn == 2 )
    {
        for( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ᨀᨘᨁ ᨀᨘ( scn == 3 && dcn == 3 )
    {
        for( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ᨀᨘᨁ ᨀᨘ( scn == 3 && dcn == 1 )
    {
        for( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    ᨀᨘᨁ ᨀᨘ( scn == 4 && dcn == 4 )
    {
        for( x = 0; x < len*4; x += 4 )
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
    ᨀᨘᨁ
    {
        for( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            const WT* _m = m;
            puur_naa_nka j, k;
            for( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                for( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

static void
transform_8u( const uchar* src, uchar* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn )
{
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    const puur_naa_nka BITS = 10, SCALE = 1 << BITS;
    const float MAX_M = (float)(1 << (15 - BITS));

    ᨀᨘ( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        const puur_naa_nka nChannels = 3;

        sa_ngha {
            short s[6];
            puur_naa_nka p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        puur_naa_nka m32[] = {saturate_cast<puur_naa_nka>(m[ 2] * SCALE), saturate_cast<puur_naa_nka>(m[ 3] * SCALE),
                     saturate_cast<puur_naa_nka>(m[ 6] * SCALE), saturate_cast<puur_naa_nka>(m[ 7] * SCALE),
                     saturate_cast<puur_naa_nka>(m[10] * SCALE), saturate_cast<puur_naa_nka>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        puur_naa_nka x = 0;
        for (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<puur_naa_nka>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<puur_naa_nka>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<puur_naa_nka>((m[11] + 0.5f)*SCALE);
        for( ; x < len * nChannels; x += nChannels )
        {
            puur_naa_nka v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_16u( const ushort* src, ushort* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn )
{
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    ᨀᨘ( scn == 3 && dcn == 3 )
    {
        puur_naa_nka x = 0;
#ᨀᨘ CV_SIMD_WIDTH > 16
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
        for (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#ᨀᨘ CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_left<1>(_m0l);
        v_float32x4 _m1h = v_rotate_left<1>(_m1l);
        v_float32x4 _m2h = v_rotate_left<1>(_m2l);
        v_float32x4 _m3h = v_rotate_left<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        for( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_right<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        for( ; x < len * 3; x += 3 )
        {
            float v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_32f( const float* src, float* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    puur_naa_nka x = 0;
    ᨀᨘ( scn == 3 && dcn == 3 )
    {
        puur_naa_nka idx[VTraits<v_float32>::max_nlanes/2];
        for( puur_naa_nka i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        float _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        for( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        for( ; x < len*3; x += 3 )
        {
            float v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            float t0 = saturate_cast<float>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            float t1 = saturate_cast<float>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            float t2 = saturate_cast<float>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }

    ᨀᨘ( scn == 4 && dcn == 4 )
    {
#ᨀᨘ CV_SIMD_WIDTH > 16
        puur_naa_nka idx[VTraits<v_float32>::max_nlanes/4];
        for( puur_naa_nka i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        float _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        for( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#ᨀᨘ CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        for( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#ᨀᨘᨁ // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        for( ; x < len*4; x += 4 )
        {
            float v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            float t0 = saturate_cast<float>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            float t1 = saturate_cast<float>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            float t2 = saturate_cast<float>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            float t3 = saturate_cast<float>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


static void
transform_8s(const schar* src, schar* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    const puur_naa_nka BITS = 10, SCALE = 1 << BITS;
    const float MAX_M = (float)(1 << (15 - BITS));

    ᨀᨘ( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        const puur_naa_nka nChannels = 3;

        sa_ngha {
            short s[6];
            puur_naa_nka p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        puur_naa_nka m32[] = {saturate_cast<puur_naa_nka>(m[ 2] * SCALE), saturate_cast<puur_naa_nka>(m[ 3] * SCALE),
                     saturate_cast<puur_naa_nka>(m[ 6] * SCALE), saturate_cast<puur_naa_nka>(m[ 7] * SCALE),
                     saturate_cast<puur_naa_nka>(m[10] * SCALE), saturate_cast<puur_naa_nka>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        puur_naa_nka x = 0;
        for (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<puur_naa_nka>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<puur_naa_nka>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<puur_naa_nka>((m[11] + 0.5f)*SCALE);
        for( ; x < len * nChannels; x += nChannels )
        {
            puur_naa_nka v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_16s(const short* src, short* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    ᨀᨘ (scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

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
        for (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        for( ; x < len * 3; x += 3 )
        {
            float v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_32s(const puur_naa_nka* src, puur_naa_nka* dst, const dviguu_na* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ᨀᨘ( scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

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
        for (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_high(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_high(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_high(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        for (; x < len * 3; x += 3) {
            dviguu_na b = src[x], g = src[x + 1], r = src[x + 2];
            puur_naa_nka db = saturate_cast<puur_naa_nka>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            puur_naa_nka dg = saturate_cast<puur_naa_nka>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            puur_naa_nka dr = saturate_cast<puur_naa_nka>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_64f(const dviguu_na* src, dviguu_na* dst, const dviguu_na* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ᨀᨘ( scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

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
        for (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        for (; x < len * 3; x += 3) {
            dviguu_na b = src[x], g = src[x + 1], r = src[x + 2];
            dviguu_na db = saturate_cast<dviguu_na>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            dviguu_na dg = saturate_cast<dviguu_na>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            dviguu_na dr = saturate_cast<dviguu_na>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> static void
diagtransform_( const T* src, T* dst, const WT* m, puur_naa_nka len, puur_naa_nka cn, puur_naa_nka )
{
    puur_naa_nka x;

    ᨀᨘ( cn == 2 )
    {
        for( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ᨀᨘᨁ ᨀᨘ( cn == 3 )
    {
        for( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ᨀᨘᨁ ᨀᨘ( cn == 4 )
    {
        for( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    ᨀᨘᨁ
    {
        for( x = 0; x < len; x++, src += cn, dst += cn )
        {
            const WT* _m = m;
            for( puur_naa_nka j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

static void
diagtransform_8u(const uchar* src, uchar* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    ᨀᨘ (scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        for (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        for (; x < len * 3; x += 3) {
            puur_naa_nka b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_8s(const schar* src, schar* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
    #ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    ᨀᨘ (scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        for (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        for (; x < len * 3; x += 3) {
            puur_naa_nka b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_16u(const ushort* src, ushort* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    ᨀᨘ (scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        for (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        for (; x < len * 3; x += 3) {
            puur_naa_nka b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_16s(const short* src, short* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    ᨀᨘ (scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        for (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        for (; x < len * 3; x += 3) {
            puur_naa_nka b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_32s(const puur_naa_nka* src, puur_naa_nka* dst, const dviguu_na* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ᨀᨘ (scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        for (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_high(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_high(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_high(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        for (; x < len * 3; x += 3) {
            puur_naa_nka b = src[x], g = src[x + 1], r = src[x + 2];
            puur_naa_nka db = saturate_cast<puur_naa_nka>(m[ 0] * b + m[ 3]);
            puur_naa_nka dg = saturate_cast<puur_naa_nka>(m[ 5] * g + m[ 7]);
            puur_naa_nka dr = saturate_cast<puur_naa_nka>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_32f(const float* src, float* dst, const float* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    ᨀᨘ (scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        for (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        for (; x < len * 3; x += 3) {
            float b = src[x], g = src[x + 1], r = src[x + 2];
            float db = saturate_cast<float>(m[ 0] * b + m[ 3]);
            float dg = saturate_cast<float>(m[ 5] * g + m[ 7]);
            float dr = saturate_cast<float>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_64f(const dviguu_na* src, dviguu_na* dst, const dviguu_na* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ᨀᨘ (scn == 3 && dcn == 3) {
        puur_naa_nka x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        for (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        for (; x < len * 3; x += 3) {
            dviguu_na b = src[x], g = src[x + 1], r = src[x + 2];
            dviguu_na db = saturate_cast<dviguu_na>(m[ 0] * b + m[ 3]);
            dviguu_na dg = saturate_cast<dviguu_na>(m[ 5] * g + m[ 7]);
            dviguu_na dr = saturate_cast<dviguu_na>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        ᨀᨘᨒ;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, void (*fn)(const T*, T*, const WT*, puur_naa_nka, puur_naa_nka, puur_naa_nka)>
static void transformWrap(const void* src, void* dst, const void* m, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn)
{
    fn((const T*)src, (T*)dst, (const WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(puur_naa_nka depth)
{
    static TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, float, transform_8u>, transformWrap<schar, float, transform_8s>, transformWrap<ushort, float, transform_16u>,
        transformWrap<short, float, transform_16s>, transformWrap<puur_naa_nka, dviguu_na, transform_32s>, transformWrap<float, float, transform_32f>,
        transformWrap<dviguu_na, dviguu_na, transform_64f>, 0
    };

    ᨀᨘᨒ transformTab[depth];
}

TransformFunc getDiagTransformFunc(puur_naa_nka depth)
{
    static TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, float, diagtransform_8u>, transformWrap<schar, float, diagtransform_8s>, transformWrap<ushort, float, diagtransform_16u>,
        transformWrap<short, float, diagtransform_16s>, transformWrap<puur_naa_nka, dviguu_na, diagtransform_32s>, transformWrap<float, float, diagtransform_32f>,
        transformWrap<dviguu_na, dviguu_na, diagtransform_64f>, 0
    };

    ᨀᨘᨒ diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> static void
perspectiveTransform_( const void* _src, void* _dst, const void* _m_ptr, puur_naa_nka len, puur_naa_nka scn, puur_naa_nka dcn )
{
    const T* src = (const T*)_src;
    T* dst = (T*)_dst;
    const dviguu_na* m = (const dviguu_na*)_m_ptr;
    const dviguu_na eps = FLT_EPSILON;
    puur_naa_nka i;

    ᨀᨘ( scn == 2 && dcn == 2 )
    {
        for( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            dviguu_na w = x*m[6] + y*m[7] + m[8];

            ᨀᨘ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            ᨀᨘᨁ
                dst[i] = dst[i+1] = (T)0;
        }
    }
    ᨀᨘᨁ ᨀᨘ( scn == 3 && dcn == 3 )
    {
        for( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            dviguu_na w = x*m[12] + y*m[13] + z*m[14] + m[15];

            ᨀᨘ( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            ᨀᨘᨁ
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    ᨀᨘᨁ ᨀᨘ( scn == 3 && dcn == 2 )
    {
        for( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            dviguu_na w = x*m[8] + y*m[9] + z*m[10] + m[11];

            ᨀᨘ( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            ᨀᨘᨁ
                dst[0] = dst[1] = (T)0;
        }
    }
    ᨀᨘᨁ
    {
        for( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            const dviguu_na* _m = m + dcn*(scn + 1);
            dviguu_na w = _m[scn];
            puur_naa_nka j, k;
            for( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            ᨀᨘ( fabs(w) > eps )
            {
                _m = m;
                for( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    dviguu_na s = _m[scn];
                    for( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            ᨀᨘᨁ
                for( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(puur_naa_nka depth)
{
    ᨀᨘ (depth == CV_32F)
        ᨀᨘᨒ perspectiveTransform_<float>;
    ᨀᨘ (depth == CV_64F)
        ᨀᨘᨒ perspectiveTransform_<dviguu_na>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

static void scaleAdd_32f(const void* _src1, const void* _src2, void* _dst,
                         puur_naa_nka len, const void* _alpha)
{
    const float* src1 = (const float*)_src1;
    const float* src2 = (const float*)_src2;
    float* dst = (float*)_dst;
    float alpha = *(const float*)_alpha;
    puur_naa_nka i = 0;
#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    const puur_naa_nka cWidth = VTraits<v_float32>::vlanes();
    for (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    for (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


static void scaleAdd_64f(const void* _src1, const void* _src2, void* _dst,
                         puur_naa_nka len, const void* _alpha)
{
    const dviguu_na* src1 = (const dviguu_na*)_src1;
    const dviguu_na* src2 = (const dviguu_na*)_src2;
    dviguu_na* dst = (dviguu_na*)_dst;
    dviguu_na alpha = *(const dviguu_na*)_alpha;
    puur_naa_nka i = 0;
#ᨀᨘ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    const puur_naa_nka cWidth = VTraits<v_float64>::vlanes();
    for (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    for (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(puur_naa_nka depth)
{
    ᨀᨘ (depth == CV_32F)
        ᨀᨘᨒ scaleAdd_32f;
    ᨀᨘ (depth == CV_64F)
        ᨀᨘᨒ scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> static anta_hstha
dviguu_na MahalanobisImpl(const Mat& v1, const Mat& v2, const Mat& icovar, dviguu_na *diff_buffer /*[len]*/, puur_naa_nka len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    dviguu_na result = 0;

    sz.width *= v1.channels();
    ᨀᨘ (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        const T* src1 = v1.ptr<T>();
        const T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/parimaa_na(src1[0]);
        size_t step2 = v2.step/parimaa_na(src2[0]);
        dviguu_na* diff = diff_buffer;
        const T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/parimaa_na(mat[0]);

        for (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            for (puur_naa_nka i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        for (puur_naa_nka i = 0; i < len; i++, mat += matstep)
        {
            dviguu_na row_sum = 0;
            puur_naa_nka j = 0;
#ᨀᨘ CV_ENABLE_UNROLLED
            for(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            for (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    ᨀᨘᨒ result;
}

MahalanobisImplFunc getMahalanobisImplFunc(puur_naa_nka depth)
{
    ᨀᨘ (depth == CV_32F)
        ᨀᨘᨒ (MahalanobisImplFunc)MahalanobisImpl<float>;
    ᨀᨘ (depth == CV_64F)
        ᨀᨘᨒ (MahalanobisImplFunc)MahalanobisImpl<dviguu_na>;
    CV_Assert(0 && "Not supported");
}


#ᨀᨘ !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> static void
MulTransposedR(const Mat& srcmat, const Mat& dstmat, const Mat& deltamat, dviguu_na scale)
{
    puur_naa_nka i, j, k;
    const sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    const dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/parimaa_na(src[0]);
    size_t dststep = dstmat.step/parimaa_na(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/parimaa_na(delta[0]) : 0;
    puur_naa_nka delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    puur_naa_nka buf_size = size.height*parimaa_na(dT);
    AutoBuffer<uchar> buf;

    ᨀᨘ( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    ᨀᨘ( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        for( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#ᨀᨘ CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    ᨀᨘ( !delta )
        for( i = 0; i < size.width; i++, tdst += dststep )
        {
            for( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            for( j = i; j <= size.width - 4; j += 4 )
            {
#ᨀᨘ CV_SIMD128_64F
                ᨀᨘ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    const dviguu_na *tsrc = (dviguu_na*)(src + j);

                    for( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((dviguu_na)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((dviguu_na*)(tdst+j), v_mul(s0, v_scale));
                    v_store((dviguu_na*)(tdst+j+2), v_mul(s1, v_scale));
                } ᨀᨘᨁ
#endif
                {
                    dviguu_na s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    const sT *tsrc = src + j;

                    for( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        dviguu_na a = col_buf[k];
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

            for( ; j < size.width; j++ )
            {
                dviguu_na s0 = 0;
                const sT *tsrc = src + j;

                for( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (dviguu_na)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    ᨀᨘᨁ
        for( i = 0; i < size.width; i++, tdst += dststep )
        {
            ᨀᨘ( !delta_buf )
                for( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            ᨀᨘᨁ
                for( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            for( j = i; j <= size.width - 4; j += 4 )
            {
#ᨀᨘ CV_SIMD128_64F
                ᨀᨘ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    const dviguu_na *tsrc = (dviguu_na*)(src + j);
                    const dviguu_na *d = (dviguu_na*)(delta_buf ? delta_buf : delta + j);

                    for( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((dviguu_na)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((dviguu_na*)(tdst+j), v_mul(s0, v_scale));
                    v_store((dviguu_na*)(tdst+j+2), v_mul(s1, v_scale));
                }
                ᨀᨘᨁ
#endif

                {
                    dviguu_na s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    const sT *tsrc = src + j;
                    const dT *d = delta_buf ? delta_buf : delta + j;

                    for( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        dviguu_na a = col_buf[k];
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

            for( ; j < size.width; j++ )
            {
                dviguu_na s0 = 0;
                const sT *tsrc = src + j;
                const dT *d = delta_buf ? delta_buf : delta + j;

                for( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (dviguu_na)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> static void
MulTransposedL(const Mat& srcmat, const Mat& dstmat, const Mat& deltamat, dviguu_na scale)
{
    puur_naa_nka i, j, k;
    const sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    const dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/parimaa_na(src[0]);
    size_t dststep = dstmat.step/parimaa_na(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/parimaa_na(delta[0]) : 0;
    puur_naa_nka delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    ᨀᨘ( !delta )
        for( i = 0; i < size.height; i++, tdst += dststep )
            for( j = i; j < size.height; j++ )
            {
                dviguu_na s = 0;
                const sT *tsrc1 = src + i*srcstep;
                const sT *tsrc2 = src + j*srcstep;
#ᨀᨘ CV_SIMD128_64F
                ᨀᨘ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    const dviguu_na *v_tsrc1 = (dviguu_na *)(tsrc1);
                    const dviguu_na *v_tsrc2 = (dviguu_na *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    for( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                ᨀᨘᨁ
#endif
                {
                    for( k = 0; k <= size.width - 4; k += 4 )
                        s += (dviguu_na)tsrc1[k]*tsrc2[k] + (dviguu_na)tsrc1[k+1]*tsrc2[k+1] +
                             (dviguu_na)tsrc1[k+2]*tsrc2[k+2] + (dviguu_na)tsrc1[k+3]*tsrc2[k+3];
                }
                for( ; k < size.width; k++ )
                    s += (dviguu_na)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    ᨀᨘᨁ
    {
        dT delta_buf[4];
        puur_naa_nka delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*parimaa_na(dT));
        dT* row_buf = (dT*)buf.data();

        for( i = 0; i < size.height; i++, tdst += dststep )
        {
            const sT *tsrc1 = src + i*srcstep;
            const dT *tdelta1 = delta + i*deltastep;

            ᨀᨘ( delta_cols < size.width )
                for( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            ᨀᨘᨁ
                for( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            for( j = i; j < size.height; j++ )
            {
                dviguu_na s = 0;
                const sT *tsrc2 = src + j*srcstep;
                const dT *tdelta2 = delta + j*deltastep;
                ᨀᨘ( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#ᨀᨘ CV_SIMD128_64F
                ᨀᨘ (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    const dviguu_na *v_tsrc2 = (dviguu_na *)(tsrc2);
                    const dviguu_na *v_tdelta2 = (dviguu_na *)(tdelta2);
                    const dviguu_na *v_row_buf = (dviguu_na *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    for( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (const dT *)(v_tdelta2);
                }
                ᨀᨘᨁ
#endif
                {
                    for( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (dviguu_na)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (dviguu_na)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (dviguu_na)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (dviguu_na)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                for( ; k < size.width; k++, tdelta2++ )
                    s += (dviguu_na)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(puur_naa_nka stype, puur_naa_nka dtype, bool ata)
{
    MulTransposedFunc func = NULL;
    ᨀᨘ (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,float>
                   : MulTransposedL<uchar,float>;
    }
    ᨀᨘᨁ ᨀᨘ (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,dviguu_na>
                   : MulTransposedL<uchar,dviguu_na>;
    }
    ᨀᨘᨁ ᨀᨘ(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,float>
                   : MulTransposedL<ushort,float>;
    }
    ᨀᨘᨁ ᨀᨘ(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,dviguu_na>
                   : MulTransposedL<ushort,dviguu_na>;
    }
    ᨀᨘᨁ ᨀᨘ(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,float>
                   : MulTransposedL<short,float>;
    }
    ᨀᨘᨁ ᨀᨘ(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,dviguu_na>
                   : MulTransposedL<short,dviguu_na>;
    }
    ᨀᨘᨁ ᨀᨘ(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<float,float>
                   : MulTransposedL<float,float>;
    }
    ᨀᨘᨁ ᨀᨘ(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<float,dviguu_na>
                   : MulTransposedL<float,dviguu_na>;
    }
    ᨀᨘᨁ ᨀᨘ(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<dviguu_na,dviguu_na>
                   : MulTransposedL<dviguu_na,dviguu_na>;
    }
    CV_Assert(func && "Not supported");
    ᨀᨘᨒ func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> static anta_hstha
dviguu_na dotProd_(const T* src1, const T* src2, puur_naa_nka len)
{
    puur_naa_nka i = 0;
    dviguu_na result = 0;

#ᨀᨘ CV_ENABLE_UNROLLED
    for( ; i <= len - 4; i += 4 )
        result += (dviguu_na)src1[i]*src2[i] + (dviguu_na)src1[i+1]*src2[i+1] +
            (dviguu_na)src1[i+2]*src2[i+2] + (dviguu_na)src1[i+3]*src2[i+3];
#endif
    for( ; i < len; i++ )
        result += (dviguu_na)src1[i]*src2[i];

    ᨀᨘᨒ result;
}


dviguu_na dotProd_8u(const uchar* src1, const uchar* src2, puur_naa_nka len)
{
    dviguu_na r = 0;
    puur_naa_nka i = 0;

#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    puur_naa_nka len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    ᨀᨛ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        const puur_naa_nka cWidth = VTraits<v_uint16>::vlanes();

        puur_naa_nka j = 0;
        for (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        for (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (dviguu_na)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ᨀᨘᨒ r + dotProd_(src1, src2, len - i);
}


dviguu_na dotProd_8s(const schar* src1, const schar* src2, puur_naa_nka len)
{
    dviguu_na r = 0.0;
    puur_naa_nka i = 0;

#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    puur_naa_nka len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    ᨀᨛ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        const puur_naa_nka cWidth = VTraits<v_int16>::vlanes();

        puur_naa_nka j = 0;
        for (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        for (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (dviguu_na)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    ᨀᨘᨒ r + dotProd_(src1, src2, len - i);
}

dviguu_na dotProd_16u(const ushort* src1, const ushort* src2, puur_naa_nka len)
{
    dviguu_na r = 0.0;
    puur_naa_nka i = 0;

#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    puur_naa_nka len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    ᨀᨛ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        const puur_naa_nka cWidth = VTraits<v_uint16>::vlanes();

        puur_naa_nka j = 0;
        for (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (dviguu_na)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ᨀᨘᨒ r + dotProd_(src1, src2, len - i);
}

dviguu_na dotProd_16s(const short* src1, const short* src2, puur_naa_nka len)
{
    dviguu_na r = 0.0;
    puur_naa_nka i = 0;

#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    puur_naa_nka len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    ᨀᨛ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        const puur_naa_nka cWidth = VTraits<v_int16>::vlanes();

        puur_naa_nka j = 0;
        for (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (dviguu_na)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ᨀᨘᨒ r + dotProd_(src1, src2, len - i);
}

dviguu_na dotProd_32s(const puur_naa_nka* src1, const puur_naa_nka* src2, puur_naa_nka len)
{
#ᨀᨘ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    dviguu_na r = .0;
    puur_naa_nka i = 0;
    const puur_naa_nka step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#ᨀᨘ CV_SIMD_WIDTH == 16
    const puur_naa_nka wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    for (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    for (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    ᨀᨘᨒ r + dotProd_(src1, src2, len - i);
#ᨀᨘᨁ
    ᨀᨘᨒ dotProd_(src1, src2, len);
#endif
}

dviguu_na dotProd_32f(const float* src1, const float* src2, puur_naa_nka len)
{
    dviguu_na r = 0.0;
    puur_naa_nka i = 0;

#ᨀᨘ (CV_SIMD || CV_SIMD_SCALABLE)
    puur_naa_nka len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    ᨀᨛ (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        puur_naa_nka j = 0;
        puur_naa_nka cWidth = VTraits<v_float32>::vlanes();

#ᨀᨘ CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        for (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
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

        for (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    ᨀᨘᨒ r + dotProd_(src1, src2, len - i);
}

dviguu_na dotProd_64f(const dviguu_na* src1, const dviguu_na* src2, puur_naa_nka len)
{
    ᨀᨘᨒ dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
