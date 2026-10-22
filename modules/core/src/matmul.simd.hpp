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

#ضمن "precomp.hpp"

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
#אם defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#אחרת
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

نطاق cv {

// forward declarations
تعريفنوع فراغ (*TransformFunc)(ثابت فراغ* src, فراغ* dst, ثابت فراغ* m, صحيح len, صحيح scn, صحيح dcn);
تعريفنوع فراغ (*ScaleAddFunc)(ثابت فراغ* src1, ثابت فراغ* src2, فراغ* dst, صحيح len, ثابت فراغ* alpha);
تعريفنوع فراغ (*MulTransposedFunc)(ثابت Mat& src, ثابت/*preallocated*/ Mat& dst, ثابت Mat& delta, مزدوج scale);
تعريفنوع مزدوج (*MahalanobisImplFunc)(ثابت Mat& v1, ثابت Mat& v2, ثابت Mat& icovar, مزدوج *diff_buffer /*[len]*/, صحيح len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

فراغ gemm32f(ثابت عائم* src1, size_t src1_step, ثابت عائم* src2, size_t src2_step,
             عائم alpha, ثابت عائم* src3, size_t src3_step, عائم beta, عائم* dst, size_t dst_step,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags);
فراغ gemm64f(ثابت مزدوج* src1, size_t src1_step, ثابت مزدوج* src2, size_t src2_step,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_step, مزدوج beta, مزدوج* dst, size_t dst_step,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags);
فراغ gemm32fc(ثابت عائم* src1, size_t src1_step, ثابت عائم* src2, size_t src2_step,
              عائم alpha, ثابت عائم* src3, size_t src3_step, عائم beta, عائم* dst, size_t dst_step,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags);
فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_step, ثابت مزدوج* src2, size_t src2_step,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_step, مزدوج beta, مزدوج* dst, size_t dst_step,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags);

TransformFunc getTransformFunc(صحيح depth);
TransformFunc getDiagTransformFunc(صحيح depth);
TransformFunc getPerspectiveTransform(صحيح depth);

ScaleAddFunc getScaleAddFunc(صحيح depth);

MahalanobisImplFunc getMahalanobisImplFunc(صحيح depth);

MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata);

مزدوج dotProd_8u(ثابت uchar* src1, ثابت uchar* src2, صحيح len);
مزدوج dotProd_8s(ثابت schar* src1, ثابت schar* src2, صحيح len);
مزدوج dotProd_16u(ثابت ushort* src1, ثابت ushort* src2, صحيح len);
مزدوج dotProd_16s(ثابت short* src1, ثابت short* src2, صحيح len);
مزدوج dotProd_32s(ثابت صحيح* src1, ثابت صحيح* src2, صحيح len);
مزدوج dotProd_32f(ثابت عائم* src1, ثابت عائم* src2, صحيح len);
مزدوج dotProd_64f(ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#אם !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

ساكن فراغ
GEMM_CopyBlock( ثابت uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    صحيح j;
    size.width *= (صحيح)(pix_size / حجم(صحيح));

    עבור( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #אם CV_ENABLE_UNROLLED
        עבור( ; j <= size.width - 4; j += 4 )
        {
            صحيح t0 = ((ثابت صحيح*)src)[j];
            صحيح t1 = ((ثابت صحيح*)src)[j+1];
            ((صحيح*)dst)[j] = t0;
            ((صحيح*)dst)[j+1] = t1;
            t0 = ((ثابت صحيح*)src)[j+2];
            t1 = ((ثابت صحيح*)src)[j+3];
            ((صحيح*)dst)[j+2] = t0;
            ((صحيح*)dst)[j+3] = t1;
        }
        #endif
        עבור( ; j < size.width; j++ )
            ((صحيح*)dst)[j] = ((ثابت صحيح*)src)[j];
    }
}


ساكن فراغ
GEMM_TransposeBlock( ثابت uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    صحيح i, j;
    עבור( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        ثابت uchar* _src = src;
        بدل( pix_size )
        {
        حالة حجم(صحيح):
            עבור( j = 0; j < size.height; j++, _src += src_step )
                ((صحيح*)dst)[j] = ((صحيح*)_src)[0];
            اكسر;
        حالة حجم(صحيح)*2:
            עבור( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                صحيح t0 = ((صحيح*)_src)[0];
                صحيح t1 = ((صحيح*)_src)[1];
                ((صحيح*)dst)[j] = t0;
                ((صحيح*)dst)[j+1] = t1;
            }
            اكسر;
        حالة حجم(صحيح)*4:
            עבור( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                صحيح t0 = ((صحيح*)_src)[0];
                صحيح t1 = ((صحيح*)_src)[1];
                ((صحيح*)dst)[j] = t0;
                ((صحيح*)dst)[j+1] = t1;
                t0 = ((صحيح*)_src)[2];
                t1 = ((صحيح*)_src)[3];
                ((صحيح*)dst)[j+2] = t0;
                ((صحيح*)dst)[j+3] = t1;
            }
            اكسر;
        افتراضي:
            CV_Assert(0);
            החזר;
        }
    }
}

#אם (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> بنية VecTraits;

template<> بنية VecTraits<عائم> {
    تعريفنوع v_float32 VT;
    ساكن مضمن VT zero() { החזר vx_setzero_f32(); }
    ساكن مضمن VT load(ثابت عائم* p) { החזר vx_load(p); }
    ساكن مضمن VT setall(عائم v) { החזר vx_setall_f32(v); }
};
#endif

#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> بنية VecTraits<مزدوج> {
    تعريفنوع v_float64 VT;
    ساكن مضمن VT zero() { החזר vx_setzero_f64(); }
    ساكن مضمن VT load(ثابت مزدوج* p) { החזר vx_load(p); }
    ساكن مضمن VT setall(مزدوج v) { החזר vx_setall_f64(v); }
};
#endif

#אם (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
ساكن مضمن ST simdDotProduct(ثابت ST* a, ثابت ST* b, صحيح n, صحيح& k)
{
    تعريفنوع typename VecTraits<ST>::VT VecT;
    ثابت صحيح vlanes = VTraits<VecT>::vlanes();
    ثابت صحيح stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    עבור( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    עבור( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    החזר v_reduce_sum(vs0);
}
#endif

#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
ساكن مضمن مزدوج simdDotProduct_f32f64(ثابت عائم* a, ثابت عائم* b, صحيح n, صحيح& k)
{
    ثابت صحيح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحيح stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    עבור( ; k <= n - stride2; k += stride2 )
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
    עבור( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_high(va), v_cvt_f64_high(vb), vs0);
    }
    החזר v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
ساكن مضمن فراغ simdGEMM_kj_f32f64(
    ثابت عائم* a_data, ثابت عائم* _b_data, size_t b_step,
    ثابت عائم* _c_data, size_t c_step1,
    عائم* d_data,
    صحيح n, صحيح m,
    مزدوج alpha, مزدوج beta,
    صحيح& j)
{
    ثابت صحيح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحيح vlanes_f64 = VTraits<v_float64>::vlanes();

    אם( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ثابت صحيح max_m = 1600 / (صحيح)حجم(عائم);
        cv::AutoBuffer<مزدوج> _s_buf(max_m);
        مزدوج* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            עבור( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            עבור( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ثابت عائم* b_ptr = _b_data;
            עבור( صحيح k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((مزدوج)a_data[k]);
                j = 0;
                עבור( ; j <= m - vlanes_f32; j += vlanes_f32 )
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
                مزدوج a_val = (مزدوج)a_data[k];
                עבור( ; j < m; j++ )
                    s_buf[j] += a_val * (مزدوج)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ثابت عائم* c_data = _c_data;
            j = 0;
            אם( !_c_data )
            {
                עבור( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                עבור( ; j < m; j++ )
                    d_data[j] = (عائم)(s_buf[j] * alpha);
            }
            אחרת אם( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                עבור( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_high(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                עבור( ; j < m; j++ )
                    d_data[j] = (عائم)(s_buf[j] * alpha + (مزدوج)c_data[j] * beta);
            }
            אחרת
            {
                עבור( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (عائم)(s_buf[j] * alpha + (مزدوج)c_data[0] * beta);
            }
        }
        j = m;
    }
    אחרת אם( vlanes_f32 * (صحيح)حجم(عائم) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ثابت عائم* c_data = _c_data;
        עבור( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            ثابت عائم* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            עבור( صحيح k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((مزدوج)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_high(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            אם( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            אחרת אם( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_high(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            אחרת
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) عائم buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                עבור( صحيح jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (عائم)beta;
            }
        }
    }
}
#endif

#אם (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
ساكن مضمن فراغ simdGEMM_kj(
    ثابت ST* a_data, ثابت ST* _b_data, size_t b_step,
    ثابت ST* _c_data, size_t c_step1,
    ST* d_data,
    صحيح n, صحيح m,
    مزدوج alpha, مزدوج beta,
    صحيح& j)
{
    تعريفنوع typename VecTraits<ST>::VT VecT;
    ثابت صحيح vlanes = VTraits<VecT>::vlanes();

    אם( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ثابت صحيح max_m = 1600 / (صحيح)حجم(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            עבור( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            עבור( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ثابت ST* b_ptr = _b_data;
            עבור( صحيح k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                עבור( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                עבור( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ثابت ST* c_data = _c_data;
            j = 0;
            אם( !_c_data )
            {
                עבור( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                עבור( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            אחרת אם( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                עבור( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                עבור( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            אחרת
            {
                עבור( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    אחרת אם( vlanes * (صحيح)حجم(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ثابت ST* c_data = _c_data;
        עבור( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            ثابت ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            עבור( صحيح k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            אם( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            אחרת אם( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            אחרת
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                עבור( صحيح jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> ساكن فراغ
GEMMSingleMul( ثابت T* a_data, size_t a_step,
               ثابت T* b_data, size_t b_step,
               ثابت T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               مزدوج alpha, مزدوج beta, صحيح flags )
{
    صحيح i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    ثابت T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= حجم(a_data[0]);
    b_step /= حجم(b_data[0]);
    c_step /= حجم(c_data[0]);
    d_step /= حجم(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    אם( !c_data )
        c_step0 = c_step1 = 0;
    אחרת אם( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    אחרת
        c_step0 = 1, c_step1 = c_step;

    אם( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        אם( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    אם( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        אם( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            עבור( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        אם( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            עבור( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        עבור( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            עבור( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                אם( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                אחרת
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            עבור( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                אם( !c_data )
                    d_data[j] = T(s0);
                אחרת
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    אחרת אם( flags & GEMM_2_T ) /* A * Bt */
    {
        עבור( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            אם( a_buf )
            {
                עבור( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            עבור( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#אם CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                אם( حجم(WT) == حجم(مزدوج) )
                {
                    אם( حجم(T) == حجم(مزدوج) )
                        s0 = (WT)simdDotProduct((ثابت مزدوج*)a_data, (ثابت مزدوج*)b_data, n, k);
                    אחרת אם( حجم(T) == حجم(عائم) )
                        s0 = (WT)simdDotProduct_f32f64((ثابت عائم*)a_data, (ثابت عائم*)b_data, n, k);
                }
                אחרת
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #אם CV_ENABLE_UNROLLED
                    עבור( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                עבור( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                אם( !c_data )
                    d_data[j] = T(s0);
                אחרת
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    אחרת אם( d_size.width*حجم(d_data[0]) <= 1600 )
    {
        עבור( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            אם( a_buf )
            {
                עבור( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            אם( حجم(WT) == حجم(مزدوج) )
            {
                אם( حجم(T) == حجم(مزدوج) )
                    simdGEMM_kj((ثابت مزدوج*)a_data, (ثابت مزدوج*)_b_data, b_step,
                                        (ثابت مزدوج*)_c_data, c_step1,
                                (مزدوج*)d_data, n, m, alpha, beta, j);
                אחרת אם( حجم(T) == حجم(عائم) )
                    simdGEMM_kj_f32f64((ثابت عائم*)a_data, (ثابت عائم*)_b_data, b_step,
                                        (ثابت عائم*)_c_data, c_step1,
                                (عائم*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            עבור( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                ثابت T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                עבור( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                אם( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                אחרת
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            עבור( ; j < m; j++, c_data += c_step1 )
            {
                ثابت T* b = _b_data + j;
                WT s0(0);

                עבור( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                אם( !c_data )
                    d_data[j] = T(s0);
                אחרת
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    אחרת
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        עבור( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            אם( a_buf )
            {
                עבור( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            עבור( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            עבור( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                אם( حجم(WT) == حجم(مزدوج) )
                {
                    אם( حجم(T) == حجم(مزدوج) )
                    {
                        ثابت صحيح vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ثابت مزدوج*)&al));
                        עבור( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((ثابت مزدوج*)(d_buf + j));
                            v_float64 vb = vx_load((ثابت مزدوج*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((مزدوج*)(d_buf + j), vd);
                        }
                    }
                    אחרת
                    {
                        ثابت صحيح vlanes_f32 = VTraits<v_float32>::vlanes();
                        ثابت صحيح vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ثابت مزدوج*)&al));
                        עבור( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((ثابت عائم*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_high(vb);
                            v_float64 vd_lo = vx_load((ثابت مزدوج*)(d_buf + j));
                            v_float64 vd_hi = vx_load((ثابت مزدوج*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((مزدوج*)(d_buf + j), vd_lo);
                            v_store((مزدوج*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                עבור( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            אם( !c_data )
                עבור( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            אחרת
                עבור( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
ساكن مضمن فراغ setDoubleDataZero(WT* d_data, صحيح m, صحيح vlanes)
{
    v_float64 vz = vx_setzero_f64();
    صحيح j = 0;
    עבור( ; j <= m - vlanes; j += vlanes )
        v_store((مزدوج*)(d_data + j), vz);
    עבור( ; j < m; j++ )
        d_data[j] = WT(0);
}

template<typename ST>
ساكن مضمن فراغ simdMulAdd(ثابت ST* b, مزدوج* d, صحيح m, مزدوج a_val);

template<>
مضمن فراغ simdMulAdd<عائم>(
    ثابت عائم* b, مزدوج* d, صحيح m, مزدوج a_val)
{
    ثابت صحيح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحيح vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    صحيح j = 0;
    עבור( ; j <= m - vlanes_f32; j += vlanes_f32 )
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
    עבור( ; j < m; j++ )
        d[j] += a_val * (مزدوج)b[j];
}

template<>
مضمن فراغ simdMulAdd<مزدوج>(
    ثابت مزدوج* b, مزدوج* d, صحيح m, مزدوج a_val)
{
    ثابت صحيح vlanes = VTraits<v_float64>::vlanes();
    ثابت صحيح stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    صحيح j = 0;
    עבור( ; j <= m - stride2; j += stride2 )
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
    עבור( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    עבור( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
ساكن مضمن فراغ simdBlockMul_kj(
    ثابت ST* a_data, ثابت ST* b_data, size_t b_step,
    مزدوج* d_data, صحيح n, صحيح m, منطقي do_acc)
{
    ثابت صحيح vlanes = VTraits<v_float64>::vlanes();
    אם( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    עבור( صحيح k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (مزدوج)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> ساكن فراغ
GEMMBlockMul( ثابت T* a_data, size_t a_step,
              ثابت T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, صحيح flags )
{
    صحيح i, j, k, n = a_size.width, m = d_size.width;
    ثابت T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    صحيح do_acc = flags & 16;

    a_step /= حجم(a_data[0]);
    b_step /= حجم(b_data[0]);
    d_step /= حجم(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    אם( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    אם( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        עבור( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            אם( a_buf )
            {
                עבור( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            עבור( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#אם CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                אם( حجم(WT) == حجم(مزدوج) )
                {
                    אם( حجم(T) == حجم(مزدوج) )
                        s0 += (WT)simdDotProduct((ثابت مزدوج*)a_data, (ثابت مزدوج*)b_data, n, k);
                    אחרת אם( حجم(T) == حجم(عائم) )
                        s0 += (WT)simdDotProduct_f32f64((ثابت عائم*)a_data, (ثابت عائم*)b_data, n, k);
                }
                אחרת
#endif
                {
                    WT s1(0);
                    עבור( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                עבור( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    אחרת
    {
        עבור( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            אם( a_buf )
            {
                עבור( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            אם( حجم(WT) == حجم(مزدوج) )
            {
                אם( حجم(T) == حجم(مزدوج) )
                    simdBlockMul_kj(
                        (ثابت مزدوج*)a_data, (ثابت مزدوج*)b_data, b_step,
                        (مزدوج*)d_data, n, m, do_acc != 0);
                אחרת
                    simdBlockMul_kj(
                        (ثابت عائم*)a_data, (ثابت عائم*)b_data, b_step,
                        (مزدوج*)d_data, n, m, do_acc != 0);
            }
            אחרת
#endif
            {
                עבור( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ثابت T* b = b_data + j;

                    אם( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    אחרת
                        s0 = s1 = s2 = s3 = WT(0);

                    עבור( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                עבור( ; j < m; j++ )
                {
                    ثابت T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    עבור( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> ساكن فراغ
GEMMStore( ثابت T* c_data, size_t c_step,
           ثابت WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           مزدوج alpha, مزدوج beta, صحيح flags )
{
    ثابت T* _c_data = c_data;
    صحيح j;
    size_t c_step0, c_step1;

    c_step /= حجم(c_data[0]);
    d_buf_step /= حجم(d_buf[0]);
    d_step /= حجم(d_data[0]);

    אם( !c_data )
        c_step0 = c_step1 = 0;
    אחרת אם( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    אחרת
        c_step0 = 1, c_step1 = c_step;

    עבור( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        אם( _c_data )
        {
            c_data = _c_data;
            j=0;
             #אם CV_ENABLE_UNROLLED
            עבור(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
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
            עבור( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        אחרת
        {
            j = 0;
             #אם CV_ENABLE_UNROLLED
            עבור( ; j <= d_size.width - 4; j += 4 )
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
            עבור( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


تعريفنوع فراغ (*GEMMSingleMulFunc)( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, ثابت فراغ* src3, size_t step3,
                   فراغ* dst, size_t dststep, Size srcsize, Size dstsize,
                   مزدوج alpha, مزدوج beta, صحيح flags );

تعريفنوع فراغ (*GEMMBlockMulFunc)( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, فراغ* dst, size_t dststep,
                   Size srcsize, Size dstsize, صحيح flags );

تعريفنوع فراغ (*GEMMStoreFunc)( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, فراغ* dst, size_t dststep,
                   Size dstsize, مزدوج alpha, مزدوج beta, صحيح flags );

template<typename T, typename WT, فراغ (*fn)(ثابت T*, size_t, ثابت T*, size_t, ثابت T*, size_t, T*, size_t, Size, Size, مزدوج, مزدوج, صحيح)>
ساكن فراغ gemmSingleMulWrap( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, ثابت فراغ* src3, size_t step3,
                   فراغ* dst, size_t dststep, Size srcsize, Size dstsize,
                   مزدوج alpha, مزدوج beta, صحيح flags )
{
    fn((ثابت T*)src1, step1, (ثابت T*)src2, step2, (ثابت T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, فراغ (*fn)(ثابت T*, size_t, ثابت T*, size_t, WT*, size_t, Size, Size, صحيح)>
ساكن فراغ gemmBlockMulWrap( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, فراغ* dst, size_t dststep,
                   Size srcsize, Size dstsize, صحيح flags )
{
    fn((ثابت T*)src1, step1, (ثابت T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, فراغ (*fn)(ثابت T*, size_t, ثابت WT*, size_t, T*, size_t, Size, مزدوج, مزدوج, صحيح)>
ساكن فراغ gemmStoreWrap( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, فراغ* dst, size_t dststep,
                   Size dstsize, مزدوج alpha, مزدوج beta, صحيح flags )
{
    fn((ثابت T*)src1, step1, (ثابت WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

ساكن فراغ GEMMSingleMul_32f( ثابت عائم* a_data, size_t a_step,
              ثابت عائم* b_data, size_t b_step,
              ثابت عائم* c_data, size_t c_step,
              عائم* d_data, size_t d_step,
              Size a_size, Size d_size,
              مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMSingleMul<عائم,مزدوج>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

ساكن فراغ GEMMSingleMul_64f( ثابت مزدوج* a_data, size_t a_step,
                              ثابت مزدوج* b_data, size_t b_step,
                              ثابت مزدوج* c_data, size_t c_step,
                              مزدوج* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMSingleMul<مزدوج,مزدوج>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


ساكن فراغ GEMMSingleMul_32fc( ثابت Complexf* a_data, size_t a_step,
                              ثابت Complexf* b_data, size_t b_step,
                              ثابت Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

ساكن فراغ GEMMSingleMul_64fc( ثابت Complexd* a_data, size_t a_step,
                              ثابت Complexd* b_data, size_t b_step,
                              ثابت Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

ساكن فراغ GEMMBlockMul_32f( ثابت عائم* a_data, size_t a_step,
             ثابت عائم* b_data, size_t b_step,
             مزدوج* d_data, size_t d_step,
             Size a_size, Size d_size, صحيح flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ساكن فراغ GEMMBlockMul_64f( ثابت مزدوج* a_data, size_t a_step,
                             ثابت مزدوج* b_data, size_t b_step,
                             مزدوج* d_data, size_t d_step,
                             Size a_size, Size d_size, صحيح flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ساكن فراغ GEMMBlockMul_32fc( ثابت Complexf* a_data, size_t a_step,
                             ثابت Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, صحيح flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ساكن فراغ GEMMBlockMul_64fc( ثابت Complexd* a_data, size_t a_step,
                             ثابت Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, صحيح flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ساكن فراغ GEMMStore_32f( ثابت عائم* c_data, size_t c_step,
          ثابت مزدوج* d_buf, size_t d_buf_step,
          عائم* d_data, size_t d_step, Size d_size,
          مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ساكن فراغ GEMMStore_64f( ثابت مزدوج* c_data, size_t c_step,
                      ثابت مزدوج* d_buf, size_t d_buf_step,
                      مزدوج* d_data, size_t d_step, Size d_size,
                      مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ساكن فراغ GEMMStore_32fc( ثابت Complexf* c_data, size_t c_step,
                          ثابت Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ساكن فراغ GEMMStore_64fc( ثابت Complexd* c_data, size_t c_step,
                          ثابت Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

ساكن فراغ gemmImpl( Mat A, Mat B, مزدوج alpha,
           Mat C, مزدوج beta, Mat D, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح block_lin_size = 128;
    ثابت صحيح block_size = block_lin_size * block_lin_size;

    ساكن مزدوج zero[] = {0,0,0,0};
    ساكن عائم zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    صحيح i, len = 0, type = A.type();

    بدل( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالة 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        اكسر;
    حالة 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        اكسر;
    حالة 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        اكسر;
    حالة 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        اكسر;
    }

    אם( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        אם( type == CV_32F )
        {
            عائم* d = D.ptr<عائم>();
            ثابت عائم *a = A.ptr<عائم>(),
                        *b = B.ptr<عائم>(),
                        *c = (ثابت عائم*)C.data;
            size_t d_step = D.step/حجم(d[0]),
                a_step = A.step/حجم(a[0]),
                b_step = B.step/حجم(b[0]),
                c_step = C.data ? C.step/حجم(c[0]) : 0;

            אם( !c )
                c = zerof;

            بدل( len )
            {
            حالة 2:
                אם( len == d_size.width && b != d )
                {
                    עבור( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_step];
                        عائم t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[1] = (عائم)(t1*alpha + c[1]*beta);
                    }
                }
                אחרת אם( a != d )
                {
                    صحيح c_step0 = 1;
                    אם( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    עבור( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_step];
                        عائم t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[d_step] = (عائم)(t1*alpha + c[c_step]*beta);
                    }
                }
                אחרת
                    اكسر;
                החזר;
            حالة 3:
                אם( len == d_size.width && b != d )
                {
                    עבור( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        عائم t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        عائم t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[1] = (عائم)(t1*alpha + c[1]*beta);
                        d[2] = (عائم)(t2*alpha + c[2]*beta);
                    }
                }
                אחרת אם( a != d )
                {
                    صحيح c_step0 = 1;
                    אם( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    עבור( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        عائم t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        عائم t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[d_step] = (عائم)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (عائم)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                אחרת
                    اكسر;
                החזר;
            حالة 4:
                אם( len == d_size.width && b != d )
                {
                    עבור( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        عائم t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        عائم t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        عائم t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[1] = (عائم)(t1*alpha + c[1]*beta);
                        d[2] = (عائم)(t2*alpha + c[2]*beta);
                        d[3] = (عائم)(t3*alpha + c[3]*beta);
                    }
                }
                אחרת אם( len <= 16 && a != d )
                {
                    صحيح c_step0 = 1;
                    אם( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    עבור( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        عائم t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        عائم t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        عائم t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[d_step] = (عائم)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (عائم)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (عائم)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                אחרת
                    اكسر;
                החזר;
            }
        }

        אם( type == CV_64F )
        {
            مزدوج* d = D.ptr<مزدوج>();
            ثابت مزدوج *a = A.ptr<مزدوج>(),
                         *b = B.ptr<مزدوج>(),
                         *c = (ثابت مزدوج*)C.data;
            size_t d_step = D.step/حجم(d[0]),
                a_step = A.step/حجم(a[0]),
                b_step = B.step/حجم(b[0]),
                c_step = C.data ? C.step/حجم(c[0]) : 0;
            אם( !c )
                c = zero;

            بدل( len )
            {
            حالة 2:
                אם( len == d_size.width && b != d )
                {
                    עבור( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_step];
                        مزدوج t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                אחרת אם( a != d )
                {
                    صحيح c_step0 = 1;
                    אם( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    עבור( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_step];
                        مزدوج t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                אחרת
                    اكسر;
                החזר;
            حالة 3:
                אם( len == d_size.width && b != d )
                {
                    עבור( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        مزدوج t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        مزدوج t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                אחרת אם( a != d )
                {
                    صحيح c_step0 = 1;
                    אם( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    עבור( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        مزدوج t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        مزدوج t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                אחרת
                    اكسر;
                החזר;
            حالة 4:
                אם( len == d_size.width && b != d )
                {
                    עבור( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        مزدوج t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        مزدوج t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        مزدوج t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                אחרת אם( d_size.width <= 16 && a != d )
                {
                    صحيح c_step0 = 1;
                    אם( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    עבור( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        مزدوج t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        مزدوج t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        مزدوج t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                אחרת
                    اكسر;
                החזר;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ثابت uchar* Cdata = C.data;
    size_t Cstep = C.data ? (size_t)C.step : 0;
    AutoBuffer<uchar> buf;

    אם( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<عائم, مزدوج, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<عائم, مزدوج, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<عائم, مزدوج, GEMMStore_32f>;
    }
    אחרת אם( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<مزدوج, مزدوج, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<مزدوج, مزدوج, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<مزدوج, مزدوج, GEMMStore_64f>;
    }
    אחרת אם( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    אחרת
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    אם( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
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
    else*/ אם( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    אחרת
    {
        صحيح is_a_t = flags & GEMM_1_T;
        صحيح is_b_t = flags & GEMM_2_T;
        صحيح elem_size = CV_ELEM_SIZE(type);
        صحيح dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        صحيح j, k, di = 0, dj = 0, dk = 0;
        صحيح dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        صحيح work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        אם( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        אחרת
            a_step0 = elem_size, a_step1 = A.step;

        אם( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        אחרת
            b_step0 = elem_size, b_step1 = b_step;

        אם( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        אחרת אם( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        אחרת
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        אם( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        אם( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        אם( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        אם( is_a_t )
            a_buf = b_buf + b_buf_size;

        עבור( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            אם( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            עבור( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                ثابت uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                אם( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                אם( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                עבור( k = 0; k < len; k += dk )
                {
                    ثابت uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    ثابت uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    אם( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    אם( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    אחרת
                        a_bl_size.width = di, a_bl_size.height = dk;

                    אם( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    אם( dj < d_size.width )
                    {
                        Size b_size;
                        אם( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        אחרת
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    אם( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    אחרת
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                אם( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>مضمن ساكن فراغ
callGemmImpl(ثابت fptype *src1, size_t src1_step, ثابت fptype *src2, size_t src2_step, fptype alpha,
          ثابت fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags, صحيح type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    صحيح b_m, b_n, c_m, c_n, m_d;

    אם(flags & GEMM_2_T)
    {
        b_m = n_d;
        אם(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        אחרת
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    אחרת
    {
        b_n = n_d;
        אם(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        אחרת
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    אם(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    אחרת
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    אם(src1 != عدم)
        A = Mat(m_a, n_a, type, (فراغ*)src1, src1_step);
    אם(src2 != عدم)
        B = Mat(b_m, b_n, type, (فراغ*)src2, src2_step);
    אם(src3 != عدم && beta != 0.0)
        C = Mat(c_m, c_n, type, (فراغ*)src3, src3_step);
    Mat D(m_d, n_d, type, (فراغ*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

فراغ gemm32f(ثابت عائم* src1, size_t src1_step, ثابت عائم* src2, size_t src2_step,
             عائم alpha, ثابت عائم* src3, size_t src3_step, عائم beta, عائم* dst, size_t dst_step,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

فراغ gemm64f(ثابت مزدوج* src1, size_t src1_step, ثابت مزدوج* src2, size_t src2_step,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_step, مزدوج beta, مزدوج* dst, size_t dst_step,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

فراغ gemm32fc(ثابت عائم* src1, size_t src1_step, ثابت عائم* src2, size_t src2_step,
              عائم alpha, ثابت عائم* src3, size_t src3_step, عائم beta, عائم* dst, size_t dst_step,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_step, ثابت مزدوج* src2, size_t src2_step,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_step, مزدوج beta, مزدوج* dst, size_t dst_step,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> ساكن فراغ
transform_( ثابت T* src, T* dst, ثابت WT* m, صحيح len, صحيح scn, صحيح dcn )
{
    صحيح x;

    אם( scn == 2 && dcn == 2 )
    {
        עבור( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    אחרת אם( scn == 3 && dcn == 3 )
    {
        עבור( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    אחרת אם( scn == 3 && dcn == 1 )
    {
        עבור( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    אחרת אם( scn == 4 && dcn == 4 )
    {
        עבור( x = 0; x < len*4; x += 4 )
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
    אחרת
    {
        עבור( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ثابت WT* _m = m;
            صحيح j, k;
            עבור( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                עבור( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

ساكن فراغ
transform_8u( ثابت uchar* src, uchar* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn )
{
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح BITS = 10, SCALE = 1 << BITS;
    ثابت عائم MAX_M = (عائم)(1 << (15 - BITS));

    אם( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ثابت صحيح nChannels = 3;

        اتحاد {
            short s[6];
            صحيح p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        صحيح m32[] = {saturate_cast<صحيح>(m[ 2] * SCALE), saturate_cast<صحيح>(m[ 3] * SCALE),
                     saturate_cast<صحيح>(m[ 6] * SCALE), saturate_cast<صحيح>(m[ 7] * SCALE),
                     saturate_cast<صحيح>(m[10] * SCALE), saturate_cast<صحيح>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        صحيح x = 0;
        עבור (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<صحيح>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<صحيح>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<صحيح>((m[11] + 0.5f)*SCALE);
        עבור( ; x < len * nChannels; x += nChannels )
        {
            صحيح v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        החזר;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_16u( ثابت ushort* src, ushort* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn )
{
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    אם( scn == 3 && dcn == 3 )
    {
        صحيح x = 0;
#אם CV_SIMD_WIDTH > 16
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
        עבור (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#אם CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_left<1>(_m0l);
        v_float32x4 _m1h = v_rotate_left<1>(_m1l);
        v_float32x4 _m2h = v_rotate_left<1>(_m2l);
        v_float32x4 _m3h = v_rotate_left<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        עבור( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_right<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        עבור( ; x < len * 3; x += 3 )
        {
            عائم v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        החזר;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_32f( ثابت عائم* src, عائم* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#אם (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    صحيح x = 0;
    אם( scn == 3 && dcn == 3 )
    {
        صحيح idx[VTraits<v_float32>::max_nlanes/2];
        עבור( صحيح i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        عائم _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        עבור( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        עבור( ; x < len*3; x += 3 )
        {
            عائم v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            عائم t0 = saturate_cast<عائم>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            عائم t1 = saturate_cast<عائم>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            عائم t2 = saturate_cast<عائم>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        החזר;
    }

    אם( scn == 4 && dcn == 4 )
    {
#אם CV_SIMD_WIDTH > 16
        صحيح idx[VTraits<v_float32>::max_nlanes/4];
        עבור( صحيح i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        عائم _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        עבור( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#אם CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        עבור( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#אחרת // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        עבור( ; x < len*4; x += 4 )
        {
            عائم v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            عائم t0 = saturate_cast<عائم>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            عائم t1 = saturate_cast<عائم>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            عائم t2 = saturate_cast<عائم>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            عائم t3 = saturate_cast<عائم>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        החזר;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


ساكن فراغ
transform_8s(ثابت schar* src, schar* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح BITS = 10, SCALE = 1 << BITS;
    ثابت عائم MAX_M = (عائم)(1 << (15 - BITS));

    אם( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ثابت صحيح nChannels = 3;

        اتحاد {
            short s[6];
            صحيح p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        صحيح m32[] = {saturate_cast<صحيح>(m[ 2] * SCALE), saturate_cast<صحيح>(m[ 3] * SCALE),
                     saturate_cast<صحيح>(m[ 6] * SCALE), saturate_cast<صحيح>(m[ 7] * SCALE),
                     saturate_cast<صحيح>(m[10] * SCALE), saturate_cast<صحيح>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        صحيح x = 0;
        עבור (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<صحيح>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<صحيح>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<صحيح>((m[11] + 0.5f)*SCALE);
        עבור( ; x < len * nChannels; x += nChannels )
        {
            صحيح v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        החזר;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_16s(ثابت short* src, short* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    אם (scn == 3 && dcn == 3) {
        صحيح x = 0;

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
        עבור (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        עבור( ; x < len * 3; x += 3 )
        {
            عائم v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        החזר;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_32s(ثابت صحيح* src, صحيح* dst, ثابت مزدوج* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    אם( scn == 3 && dcn == 3) {
        صحيح x = 0;

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
        עבור (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
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
        עבור (; x < len * 3; x += 3) {
            مزدوج b = src[x], g = src[x + 1], r = src[x + 2];
            صحيح db = saturate_cast<صحيح>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            صحيح dg = saturate_cast<صحيح>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            صحيح dr = saturate_cast<صحيح>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_64f(ثابت مزدوج* src, مزدوج* dst, ثابت مزدوج* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    אם( scn == 3 && dcn == 3) {
        صحيح x = 0;

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
        עבור (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        עבור (; x < len * 3; x += 3) {
            مزدوج b = src[x], g = src[x + 1], r = src[x + 2];
            مزدوج db = saturate_cast<مزدوج>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            مزدوج dg = saturate_cast<مزدوج>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            مزدوج dr = saturate_cast<مزدوج>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> ساكن فراغ
diagtransform_( ثابت T* src, T* dst, ثابت WT* m, صحيح len, صحيح cn, صحيح )
{
    صحيح x;

    אם( cn == 2 )
    {
        עבור( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    אחרת אם( cn == 3 )
    {
        עבור( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    אחרת אם( cn == 4 )
    {
        עבור( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    אחרת
    {
        עבור( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ثابت WT* _m = m;
            עבור( صحيح j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

ساكن فراغ
diagtransform_8u(ثابت uchar* src, uchar* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    אם (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        עבור (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        עבור (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_8s(ثابت schar* src, schar* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
    #אם (CV_SIMD || CV_SIMD_SCALABLE)
    אם (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        עבור (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        עבור (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_16u(ثابت ushort* src, ushort* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    אם (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        עבור (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        עבור (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_16s(ثابت short* src, short* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    אם (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        עבור (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        עבור (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_32s(ثابت صحيح* src, صحيح* dst, ثابت مزدوج* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    אם (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        עבור (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
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
        עבור (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            صحيح db = saturate_cast<صحيح>(m[ 0] * b + m[ 3]);
            صحيح dg = saturate_cast<صحيح>(m[ 5] * g + m[ 7]);
            صحيح dr = saturate_cast<صحيح>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_32f(ثابت عائم* src, عائم* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    אם (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        עבור (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        עבור (; x < len * 3; x += 3) {
            عائم b = src[x], g = src[x + 1], r = src[x + 2];
            عائم db = saturate_cast<عائم>(m[ 0] * b + m[ 3]);
            عائم dg = saturate_cast<عائم>(m[ 5] * g + m[ 7]);
            عائم dr = saturate_cast<عائم>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_64f(ثابت مزدوج* src, مزدوج* dst, ثابت مزدوج* m, صحيح len, صحيح scn, صحيح dcn)
{
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    אם (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        עבור (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        עבור (; x < len * 3; x += 3) {
            مزدوج b = src[x], g = src[x + 1], r = src[x + 2];
            مزدوج db = saturate_cast<مزدوج>(m[ 0] * b + m[ 3]);
            مزدوج dg = saturate_cast<مزدوج>(m[ 5] * g + m[ 7]);
            مزدوج dr = saturate_cast<مزدوج>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        החזר;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, فراغ (*fn)(ثابت T*, T*, ثابت WT*, صحيح, صحيح, صحيح)>
ساكن فراغ transformWrap(ثابت فراغ* src, فراغ* dst, ثابت فراغ* m, صحيح len, صحيح scn, صحيح dcn)
{
    fn((ثابت T*)src, (T*)dst, (ثابت WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(صحيح depth)
{
    ساكن TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, عائم, transform_8u>, transformWrap<schar, عائم, transform_8s>, transformWrap<ushort, عائم, transform_16u>,
        transformWrap<short, عائم, transform_16s>, transformWrap<صحيح, مزدوج, transform_32s>, transformWrap<عائم, عائم, transform_32f>,
        transformWrap<مزدوج, مزدوج, transform_64f>, 0
    };

    החזר transformTab[depth];
}

TransformFunc getDiagTransformFunc(صحيح depth)
{
    ساكن TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, عائم, diagtransform_8u>, transformWrap<schar, عائم, diagtransform_8s>, transformWrap<ushort, عائم, diagtransform_16u>,
        transformWrap<short, عائم, diagtransform_16s>, transformWrap<صحيح, مزدوج, diagtransform_32s>, transformWrap<عائم, عائم, diagtransform_32f>,
        transformWrap<مزدوج, مزدوج, diagtransform_64f>, 0
    };

    החזר diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> ساكن فراغ
perspectiveTransform_( ثابت فراغ* _src, فراغ* _dst, ثابت فراغ* _m_ptr, صحيح len, صحيح scn, صحيح dcn )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    ثابت مزدوج* m = (ثابت مزدوج*)_m_ptr;
    ثابت مزدوج eps = FLT_EPSILON;
    صحيح i;

    אם( scn == 2 && dcn == 2 )
    {
        עבור( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            مزدوج w = x*m[6] + y*m[7] + m[8];

            אם( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            אחרת
                dst[i] = dst[i+1] = (T)0;
        }
    }
    אחרת אם( scn == 3 && dcn == 3 )
    {
        עבור( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            مزدوج w = x*m[12] + y*m[13] + z*m[14] + m[15];

            אם( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            אחרת
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    אחרת אם( scn == 3 && dcn == 2 )
    {
        עבור( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            مزدوج w = x*m[8] + y*m[9] + z*m[10] + m[11];

            אם( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            אחרת
                dst[0] = dst[1] = (T)0;
        }
    }
    אחרת
    {
        עבור( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ثابت مزدوج* _m = m + dcn*(scn + 1);
            مزدوج w = _m[scn];
            صحيح j, k;
            עבור( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            אם( fabs(w) > eps )
            {
                _m = m;
                עבור( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    مزدوج s = _m[scn];
                    עבור( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            אחרת
                עבור( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(صحيح depth)
{
    אם (depth == CV_32F)
        החזר perspectiveTransform_<عائم>;
    אם (depth == CV_64F)
        החזר perspectiveTransform_<مزدوج>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

ساكن فراغ scaleAdd_32f(ثابت فراغ* _src1, ثابت فراغ* _src2, فراغ* _dst,
                         صحيح len, ثابت فراغ* _alpha)
{
    ثابت عائم* src1 = (ثابت عائم*)_src1;
    ثابت عائم* src2 = (ثابت عائم*)_src2;
    عائم* dst = (عائم*)_dst;
    عائم alpha = *(ثابت عائم*)_alpha;
    صحيح i = 0;
#אם (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ثابت صحيح cWidth = VTraits<v_float32>::vlanes();
    עבור (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    עבור (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


ساكن فراغ scaleAdd_64f(ثابت فراغ* _src1, ثابت فراغ* _src2, فراغ* _dst,
                         صحيح len, ثابت فراغ* _alpha)
{
    ثابت مزدوج* src1 = (ثابت مزدوج*)_src1;
    ثابت مزدوج* src2 = (ثابت مزدوج*)_src2;
    مزدوج* dst = (مزدوج*)_dst;
    مزدوج alpha = *(ثابت مزدوج*)_alpha;
    صحيح i = 0;
#אם (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ثابت صحيح cWidth = VTraits<v_float64>::vlanes();
    עבור (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    עבור (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(صحيح depth)
{
    אם (depth == CV_32F)
        החזר scaleAdd_32f;
    אם (depth == CV_64F)
        החזר scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> ساكن مضمن
مزدوج MahalanobisImpl(ثابت Mat& v1, ثابت Mat& v2, ثابت Mat& icovar, مزدوج *diff_buffer /*[len]*/, صحيح len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    مزدوج result = 0;

    sz.width *= v1.channels();
    אם (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        ثابت T* src1 = v1.ptr<T>();
        ثابت T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/حجم(src1[0]);
        size_t step2 = v2.step/حجم(src2[0]);
        مزدوج* diff = diff_buffer;
        ثابت T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/حجم(mat[0]);

        עבור (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            עבור (صحيح i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        עבור (صحيح i = 0; i < len; i++, mat += matstep)
        {
            مزدوج row_sum = 0;
            صحيح j = 0;
#אם CV_ENABLE_UNROLLED
            עבור(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            עבור (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    החזר result;
}

MahalanobisImplFunc getMahalanobisImplFunc(صحيح depth)
{
    אם (depth == CV_32F)
        החזר (MahalanobisImplFunc)MahalanobisImpl<عائم>;
    אם (depth == CV_64F)
        החזר (MahalanobisImplFunc)MahalanobisImpl<مزدوج>;
    CV_Assert(0 && "Not supported");
}


#אם !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> ساكن فراغ
MulTransposedR(ثابت Mat& srcmat, ثابت Mat& dstmat, ثابت Mat& deltamat, مزدوج scale)
{
    صحيح i, j, k;
    ثابت sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ثابت dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/حجم(src[0]);
    size_t dststep = dstmat.step/حجم(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/حجم(delta[0]) : 0;
    صحيح delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    صحيح buf_size = size.height*حجم(dT);
    AutoBuffer<uchar> buf;

    אם( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    אם( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        עבור( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#אם CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    אם( !delta )
        עבור( i = 0; i < size.width; i++, tdst += dststep )
        {
            עבור( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            עבור( j = i; j <= size.width - 4; j += 4 )
            {
#אם CV_SIMD128_64F
                אם (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ثابت مزدوج *tsrc = (مزدوج*)(src + j);

                    עבור( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((مزدوج)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((مزدوج*)(tdst+j), v_mul(s0, v_scale));
                    v_store((مزدوج*)(tdst+j+2), v_mul(s1, v_scale));
                } אחרת
#endif
                {
                    مزدوج s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ثابت sT *tsrc = src + j;

                    עבור( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        مزدوج a = col_buf[k];
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

            עבור( ; j < size.width; j++ )
            {
                مزدوج s0 = 0;
                ثابت sT *tsrc = src + j;

                עבור( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (مزدوج)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    אחרת
        עבור( i = 0; i < size.width; i++, tdst += dststep )
        {
            אם( !delta_buf )
                עבור( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            אחרת
                עבור( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            עבור( j = i; j <= size.width - 4; j += 4 )
            {
#אם CV_SIMD128_64F
                אם (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ثابت مزدوج *tsrc = (مزدوج*)(src + j);
                    ثابت مزدوج *d = (مزدوج*)(delta_buf ? delta_buf : delta + j);

                    עבור( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((مزدوج)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((مزدوج*)(tdst+j), v_mul(s0, v_scale));
                    v_store((مزدوج*)(tdst+j+2), v_mul(s1, v_scale));
                }
                אחרת
#endif

                {
                    مزدوج s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ثابت sT *tsrc = src + j;
                    ثابت dT *d = delta_buf ? delta_buf : delta + j;

                    עבור( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        مزدوج a = col_buf[k];
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

            עבור( ; j < size.width; j++ )
            {
                مزدوج s0 = 0;
                ثابت sT *tsrc = src + j;
                ثابت dT *d = delta_buf ? delta_buf : delta + j;

                עבור( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (مزدوج)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> ساكن فراغ
MulTransposedL(ثابت Mat& srcmat, ثابت Mat& dstmat, ثابت Mat& deltamat, مزدوج scale)
{
    صحيح i, j, k;
    ثابت sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ثابت dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/حجم(src[0]);
    size_t dststep = dstmat.step/حجم(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/حجم(delta[0]) : 0;
    صحيح delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    אם( !delta )
        עבור( i = 0; i < size.height; i++, tdst += dststep )
            עבור( j = i; j < size.height; j++ )
            {
                مزدوج s = 0;
                ثابت sT *tsrc1 = src + i*srcstep;
                ثابت sT *tsrc2 = src + j*srcstep;
#אם CV_SIMD128_64F
                אם (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ثابت مزدوج *v_tsrc1 = (مزدوج *)(tsrc1);
                    ثابت مزدوج *v_tsrc2 = (مزدوج *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    עבור( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                אחרת
#endif
                {
                    עבור( k = 0; k <= size.width - 4; k += 4 )
                        s += (مزدوج)tsrc1[k]*tsrc2[k] + (مزدوج)tsrc1[k+1]*tsrc2[k+1] +
                             (مزدوج)tsrc1[k+2]*tsrc2[k+2] + (مزدوج)tsrc1[k+3]*tsrc2[k+3];
                }
                עבור( ; k < size.width; k++ )
                    s += (مزدوج)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    אחרת
    {
        dT delta_buf[4];
        صحيح delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*حجم(dT));
        dT* row_buf = (dT*)buf.data();

        עבור( i = 0; i < size.height; i++, tdst += dststep )
        {
            ثابت sT *tsrc1 = src + i*srcstep;
            ثابت dT *tdelta1 = delta + i*deltastep;

            אם( delta_cols < size.width )
                עבור( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            אחרת
                עבור( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            עבור( j = i; j < size.height; j++ )
            {
                مزدوج s = 0;
                ثابت sT *tsrc2 = src + j*srcstep;
                ثابت dT *tdelta2 = delta + j*deltastep;
                אם( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#אם CV_SIMD128_64F
                אם (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ثابت مزدوج *v_tsrc2 = (مزدوج *)(tsrc2);
                    ثابت مزدوج *v_tdelta2 = (مزدوج *)(tdelta2);
                    ثابت مزدوج *v_row_buf = (مزدوج *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    עבור( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (ثابت dT *)(v_tdelta2);
                }
                אחרת
#endif
                {
                    עבור( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (مزدوج)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (مزدوج)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (مزدوج)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (مزدوج)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                עבור( ; k < size.width; k++, tdelta2++ )
                    s += (مزدوج)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata)
{
    MulTransposedFunc func = عدم;
    אם (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,عائم>
                   : MulTransposedL<uchar,عائم>;
    }
    אחרת אם (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,مزدوج>
                   : MulTransposedL<uchar,مزدوج>;
    }
    אחרת אם(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,عائم>
                   : MulTransposedL<ushort,عائم>;
    }
    אחרת אם(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,مزدوج>
                   : MulTransposedL<ushort,مزدوج>;
    }
    אחרת אם(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,عائم>
                   : MulTransposedL<short,عائم>;
    }
    אחרת אם(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,مزدوج>
                   : MulTransposedL<short,مزدوج>;
    }
    אחרת אם(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<عائم,عائم>
                   : MulTransposedL<عائم,عائم>;
    }
    אחרת אם(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<عائم,مزدوج>
                   : MulTransposedL<عائم,مزدوج>;
    }
    אחרת אם(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<مزدوج,مزدوج>
                   : MulTransposedL<مزدوج,مزدوج>;
    }
    CV_Assert(func && "Not supported");
    החזר func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> ساكن مضمن
مزدوج dotProd_(ثابت T* src1, ثابت T* src2, صحيح len)
{
    صحيح i = 0;
    مزدوج result = 0;

#אם CV_ENABLE_UNROLLED
    עבור( ; i <= len - 4; i += 4 )
        result += (مزدوج)src1[i]*src2[i] + (مزدوج)src1[i+1]*src2[i+1] +
            (مزدوج)src1[i+2]*src2[i+2] + (مزدوج)src1[i+3]*src2[i+3];
#endif
    עבור( ; i < len; i++ )
        result += (مزدوج)src1[i]*src2[i];

    החזר result;
}


مزدوج dotProd_8u(ثابت uchar* src1, ثابت uchar* src2, صحيح len)
{
    مزدوج r = 0;
    صحيح i = 0;

#אם (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    כלעוד (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        ثابت صحيح cWidth = VTraits<v_uint16>::vlanes();

        صحيح j = 0;
        עבור (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        עבור (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (مزدوج)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    החזר r + dotProd_(src1, src2, len - i);
}


مزدوج dotProd_8s(ثابت schar* src1, ثابت schar* src2, صحيح len)
{
    مزدوج r = 0.0;
    صحيح i = 0;

#אם (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    כלעוד (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        ثابت صحيح cWidth = VTraits<v_int16>::vlanes();

        صحيح j = 0;
        עבור (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        עבור (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (مزدوج)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    החזר r + dotProd_(src1, src2, len - i);
}

مزدوج dotProd_16u(ثابت ushort* src1, ثابت ushort* src2, صحيح len)
{
    مزدوج r = 0.0;
    صحيح i = 0;

#אם (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    כלעוד (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        ثابت صحيح cWidth = VTraits<v_uint16>::vlanes();

        صحيح j = 0;
        עבור (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (مزدوج)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    החזר r + dotProd_(src1, src2, len - i);
}

مزدوج dotProd_16s(ثابت short* src1, ثابت short* src2, صحيح len)
{
    مزدوج r = 0.0;
    صحيح i = 0;

#אם (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    כלעוד (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        ثابت صحيح cWidth = VTraits<v_int16>::vlanes();

        صحيح j = 0;
        עבור (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (مزدوج)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    החזר r + dotProd_(src1, src2, len - i);
}

مزدوج dotProd_32s(ثابت صحيح* src1, ثابت صحيح* src2, صحيح len)
{
#אם CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    مزدوج r = .0;
    صحيح i = 0;
    ثابت صحيح step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#אם CV_SIMD_WIDTH == 16
    ثابت صحيح wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    עבור (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    עבור (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    החזר r + dotProd_(src1, src2, len - i);
#אחרת
    החזר dotProd_(src1, src2, len);
#endif
}

مزدوج dotProd_32f(ثابت عائم* src1, ثابت عائم* src2, صحيح len)
{
    مزدوج r = 0.0;
    صحيح i = 0;

#אם (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    כלעוד (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        صحيح j = 0;
        صحيح cWidth = VTraits<v_float32>::vlanes();

#אם CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        עבור (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
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

        עבור (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    החזר r + dotProd_(src1, src2, len - i);
}

مزدوج dotProd_64f(ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح len)
{
    החזר dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
