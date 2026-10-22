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

#شامل "precomp.hpp"

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
#اگر defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#وگرنه
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

فضاینام cv {

// forward declarations
تعریفنوع تهی (*TransformFunc)(ثابت تهی* src, تهی* dst, ثابت تهی* m, صحیح len, صحیح scn, صحیح dcn);
تعریفنوع تهی (*ScaleAddFunc)(ثابت تهی* src1, ثابت تهی* src2, تهی* dst, صحیح len, ثابت تهی* alpha);
تعریفنوع تهی (*MulTransposedFunc)(ثابت Mat& src, ثابت/*preallocated*/ Mat& dst, ثابت Mat& delta, دوگانه scale);
تعریفنوع دوگانه (*MahalanobisImplFunc)(ثابت Mat& v1, ثابت Mat& v2, ثابت Mat& icovar, دوگانه *diff_buffer /*[len]*/, صحیح len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

تهی gemm32f(ثابت اعشاری* src1, size_t src1_step, ثابت اعشاری* src2, size_t src2_step,
             اعشاری alpha, ثابت اعشاری* src3, size_t src3_step, اعشاری beta, اعشاری* dst, size_t dst_step,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags);
تهی gemm64f(ثابت دوگانه* src1, size_t src1_step, ثابت دوگانه* src2, size_t src2_step,
             دوگانه alpha, ثابت دوگانه* src3, size_t src3_step, دوگانه beta, دوگانه* dst, size_t dst_step,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags);
تهی gemm32fc(ثابت اعشاری* src1, size_t src1_step, ثابت اعشاری* src2, size_t src2_step,
              اعشاری alpha, ثابت اعشاری* src3, size_t src3_step, اعشاری beta, اعشاری* dst, size_t dst_step,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags);
تهی gemm64fc(ثابت دوگانه* src1, size_t src1_step, ثابت دوگانه* src2, size_t src2_step,
              دوگانه alpha, ثابت دوگانه* src3, size_t src3_step, دوگانه beta, دوگانه* dst, size_t dst_step,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags);

TransformFunc getTransformFunc(صحیح depth);
TransformFunc getDiagTransformFunc(صحیح depth);
TransformFunc getPerspectiveTransform(صحیح depth);

ScaleAddFunc getScaleAddFunc(صحیح depth);

MahalanobisImplFunc getMahalanobisImplFunc(صحیح depth);

MulTransposedFunc getMulTransposedFunc(صحیح stype, صحیح dtype, منطقی ata);

دوگانه dotProd_8u(ثابت uchar* src1, ثابت uchar* src2, صحیح len);
دوگانه dotProd_8s(ثابت schar* src1, ثابت schar* src2, صحیح len);
دوگانه dotProd_16u(ثابت ushort* src1, ثابت ushort* src2, صحیح len);
دوگانه dotProd_16s(ثابت short* src1, ثابت short* src2, صحیح len);
دوگانه dotProd_32s(ثابت صحیح* src1, ثابت صحیح* src2, صحیح len);
دوگانه dotProd_32f(ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح len);
دوگانه dotProd_64f(ثابت دوگانه* src1, ثابت دوگانه* src2, صحیح len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#اگر !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

ایستا تهی
GEMM_CopyBlock( ثابت uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    صحیح j;
    size.width *= (صحیح)(pix_size / اندازه(صحیح));

    برای( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #اگر CV_ENABLE_UNROLLED
        برای( ; j <= size.width - 4; j += 4 )
        {
            صحیح t0 = ((ثابت صحیح*)src)[j];
            صحیح t1 = ((ثابت صحیح*)src)[j+1];
            ((صحیح*)dst)[j] = t0;
            ((صحیح*)dst)[j+1] = t1;
            t0 = ((ثابت صحیح*)src)[j+2];
            t1 = ((ثابت صحیح*)src)[j+3];
            ((صحیح*)dst)[j+2] = t0;
            ((صحیح*)dst)[j+3] = t1;
        }
        #endif
        برای( ; j < size.width; j++ )
            ((صحیح*)dst)[j] = ((ثابت صحیح*)src)[j];
    }
}


ایستا تهی
GEMM_TransposeBlock( ثابت uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    صحیح i, j;
    برای( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        ثابت uchar* _src = src;
        انتخاب( pix_size )
        {
        حالت اندازه(صحیح):
            برای( j = 0; j < size.height; j++, _src += src_step )
                ((صحیح*)dst)[j] = ((صحیح*)_src)[0];
            بشکن;
        حالت اندازه(صحیح)*2:
            برای( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                صحیح t0 = ((صحیح*)_src)[0];
                صحیح t1 = ((صحیح*)_src)[1];
                ((صحیح*)dst)[j] = t0;
                ((صحیح*)dst)[j+1] = t1;
            }
            بشکن;
        حالت اندازه(صحیح)*4:
            برای( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                صحیح t0 = ((صحیح*)_src)[0];
                صحیح t1 = ((صحیح*)_src)[1];
                ((صحیح*)dst)[j] = t0;
                ((صحیح*)dst)[j+1] = t1;
                t0 = ((صحیح*)_src)[2];
                t1 = ((صحیح*)_src)[3];
                ((صحیح*)dst)[j+2] = t0;
                ((صحیح*)dst)[j+3] = t1;
            }
            بشکن;
        پیشفرض:
            CV_Assert(0);
            بازگشت;
        }
    }
}

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> ساختار VecTraits;

template<> ساختار VecTraits<اعشاری> {
    تعریفنوع v_float32 VT;
    ایستا درونخطی VT zero() { بازگشت vx_setzero_f32(); }
    ایستا درونخطی VT load(ثابت اعشاری* p) { بازگشت vx_load(p); }
    ایستا درونخطی VT setall(اعشاری v) { بازگشت vx_setall_f32(v); }
};
#endif

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> ساختار VecTraits<دوگانه> {
    تعریفنوع v_float64 VT;
    ایستا درونخطی VT zero() { بازگشت vx_setzero_f64(); }
    ایستا درونخطی VT load(ثابت دوگانه* p) { بازگشت vx_load(p); }
    ایستا درونخطی VT setall(دوگانه v) { بازگشت vx_setall_f64(v); }
};
#endif

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
ایستا درونخطی ST simdDotProduct(ثابت ST* a, ثابت ST* b, صحیح n, صحیح& k)
{
    تعریفنوع typename VecTraits<ST>::VT VecT;
    ثابت صحیح vlanes = VTraits<VecT>::vlanes();
    ثابت صحیح stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    برای( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    برای( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    بازگشت v_reduce_sum(vs0);
}
#endif

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
ایستا درونخطی دوگانه simdDotProduct_f32f64(ثابت اعشاری* a, ثابت اعشاری* b, صحیح n, صحیح& k)
{
    ثابت صحیح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحیح stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    برای( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_بالا(va0), v_cvt_f64_بالا(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_بالا(va1), v_cvt_f64_بالا(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    برای( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_بالا(va), v_cvt_f64_بالا(vb), vs0);
    }
    بازگشت v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
ایستا درونخطی تهی simdGEMM_kj_f32f64(
    ثابت اعشاری* a_data, ثابت اعشاری* _b_data, size_t b_step,
    ثابت اعشاری* _c_data, size_t c_step1,
    اعشاری* d_data,
    صحیح n, صحیح m,
    دوگانه alpha, دوگانه beta,
    صحیح& j)
{
    ثابت صحیح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحیح vlanes_f64 = VTraits<v_float64>::vlanes();

    اگر( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ثابت صحیح max_m = 1600 / (صحیح)اندازه(اعشاری);
        cv::AutoBuffer<دوگانه> _s_buf(max_m);
        دوگانه* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            برای( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            برای( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ثابت اعشاری* b_ptr = _b_data;
            برای( صحیح k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((دوگانه)a_data[k]);
                j = 0;
                برای( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_بالا(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                دوگانه a_val = (دوگانه)a_data[k];
                برای( ; j < m; j++ )
                    s_buf[j] += a_val * (دوگانه)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ثابت اعشاری* c_data = _c_data;
            j = 0;
            اگر( !_c_data )
            {
                برای( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                برای( ; j < m; j++ )
                    d_data[j] = (اعشاری)(s_buf[j] * alpha);
            }
            وگرنه اگر( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                برای( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_بالا(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                برای( ; j < m; j++ )
                    d_data[j] = (اعشاری)(s_buf[j] * alpha + (دوگانه)c_data[j] * beta);
            }
            وگرنه
            {
                برای( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (اعشاری)(s_buf[j] * alpha + (دوگانه)c_data[0] * beta);
            }
        }
        j = m;
    }
    وگرنه اگر( vlanes_f32 * (صحیح)اندازه(اعشاری) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ثابت اعشاری* c_data = _c_data;
        برای( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            ثابت اعشاری* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            برای( صحیح k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((دوگانه)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_بالا(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            اگر( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            وگرنه اگر( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_بالا(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            وگرنه
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) اعشاری buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                برای( صحیح jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (اعشاری)beta;
            }
        }
    }
}
#endif

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
ایستا درونخطی تهی simdGEMM_kj(
    ثابت ST* a_data, ثابت ST* _b_data, size_t b_step,
    ثابت ST* _c_data, size_t c_step1,
    ST* d_data,
    صحیح n, صحیح m,
    دوگانه alpha, دوگانه beta,
    صحیح& j)
{
    تعریفنوع typename VecTraits<ST>::VT VecT;
    ثابت صحیح vlanes = VTraits<VecT>::vlanes();

    اگر( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ثابت صحیح max_m = 1600 / (صحیح)اندازه(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            برای( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            برای( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ثابت ST* b_ptr = _b_data;
            برای( صحیح k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                برای( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                برای( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ثابت ST* c_data = _c_data;
            j = 0;
            اگر( !_c_data )
            {
                برای( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                برای( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            وگرنه اگر( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                برای( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                برای( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            وگرنه
            {
                برای( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    وگرنه اگر( vlanes * (صحیح)اندازه(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ثابت ST* c_data = _c_data;
        برای( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            ثابت ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            برای( صحیح k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            اگر( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            وگرنه اگر( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            وگرنه
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                برای( صحیح jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> ایستا تهی
GEMMSingleMul( ثابت T* a_data, size_t a_step,
               ثابت T* b_data, size_t b_step,
               ثابت T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               دوگانه alpha, دوگانه beta, صحیح flags )
{
    صحیح i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    ثابت T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= اندازه(a_data[0]);
    b_step /= اندازه(b_data[0]);
    c_step /= اندازه(c_data[0]);
    d_step /= اندازه(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    اگر( !c_data )
        c_step0 = c_step1 = 0;
    وگرنه اگر( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    وگرنه
        c_step0 = 1, c_step1 = c_step;

    اگر( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        اگر( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    اگر( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        اگر( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            برای( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        اگر( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            برای( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        برای( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            برای( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                اگر( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                وگرنه
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            برای( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                اگر( !c_data )
                    d_data[j] = T(s0);
                وگرنه
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    وگرنه اگر( flags & GEMM_2_T ) /* A * Bt */
    {
        برای( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            اگر( a_buf )
            {
                برای( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            برای( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#اگر CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                اگر( اندازه(WT) == اندازه(دوگانه) )
                {
                    اگر( اندازه(T) == اندازه(دوگانه) )
                        s0 = (WT)simdDotProduct((ثابت دوگانه*)a_data, (ثابت دوگانه*)b_data, n, k);
                    وگرنه اگر( اندازه(T) == اندازه(اعشاری) )
                        s0 = (WT)simdDotProduct_f32f64((ثابت اعشاری*)a_data, (ثابت اعشاری*)b_data, n, k);
                }
                وگرنه
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #اگر CV_ENABLE_UNROLLED
                    برای( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                برای( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                اگر( !c_data )
                    d_data[j] = T(s0);
                وگرنه
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    وگرنه اگر( d_size.width*اندازه(d_data[0]) <= 1600 )
    {
        برای( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            اگر( a_buf )
            {
                برای( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            اگر( اندازه(WT) == اندازه(دوگانه) )
            {
                اگر( اندازه(T) == اندازه(دوگانه) )
                    simdGEMM_kj((ثابت دوگانه*)a_data, (ثابت دوگانه*)_b_data, b_step,
                                        (ثابت دوگانه*)_c_data, c_step1,
                                (دوگانه*)d_data, n, m, alpha, beta, j);
                وگرنه اگر( اندازه(T) == اندازه(اعشاری) )
                    simdGEMM_kj_f32f64((ثابت اعشاری*)a_data, (ثابت اعشاری*)_b_data, b_step,
                                        (ثابت اعشاری*)_c_data, c_step1,
                                (اعشاری*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            برای( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                ثابت T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                برای( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                اگر( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                وگرنه
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            برای( ; j < m; j++, c_data += c_step1 )
            {
                ثابت T* b = _b_data + j;
                WT s0(0);

                برای( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                اگر( !c_data )
                    d_data[j] = T(s0);
                وگرنه
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    وگرنه
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        برای( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            اگر( a_buf )
            {
                برای( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            برای( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            برای( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                اگر( اندازه(WT) == اندازه(دوگانه) )
                {
                    اگر( اندازه(T) == اندازه(دوگانه) )
                    {
                        ثابت صحیح vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ثابت دوگانه*)&al));
                        برای( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((ثابت دوگانه*)(d_buf + j));
                            v_float64 vb = vx_load((ثابت دوگانه*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((دوگانه*)(d_buf + j), vd);
                        }
                    }
                    وگرنه
                    {
                        ثابت صحیح vlanes_f32 = VTraits<v_float32>::vlanes();
                        ثابت صحیح vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ثابت دوگانه*)&al));
                        برای( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((ثابت اعشاری*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_بالا(vb);
                            v_float64 vd_lo = vx_load((ثابت دوگانه*)(d_buf + j));
                            v_float64 vd_hi = vx_load((ثابت دوگانه*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((دوگانه*)(d_buf + j), vd_lo);
                            v_store((دوگانه*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                برای( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            اگر( !c_data )
                برای( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            وگرنه
                برای( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
ایستا درونخطی تهی setDoubleDataZero(WT* d_data, صحیح m, صحیح vlanes)
{
    v_float64 vz = vx_setzero_f64();
    صحیح j = 0;
    برای( ; j <= m - vlanes; j += vlanes )
        v_store((دوگانه*)(d_data + j), vz);
    برای( ; j < m; j++ )
        d_data[j] = WT(0);
}

template<typename ST>
ایستا درونخطی تهی simdMulAdd(ثابت ST* b, دوگانه* d, صحیح m, دوگانه a_val);

template<>
درونخطی تهی simdMulAdd<اعشاری>(
    ثابت اعشاری* b, دوگانه* d, صحیح m, دوگانه a_val)
{
    ثابت صحیح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحیح vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    صحیح j = 0;
    برای( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_بالا(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    برای( ; j < m; j++ )
        d[j] += a_val * (دوگانه)b[j];
}

template<>
درونخطی تهی simdMulAdd<دوگانه>(
    ثابت دوگانه* b, دوگانه* d, صحیح m, دوگانه a_val)
{
    ثابت صحیح vlanes = VTraits<v_float64>::vlanes();
    ثابت صحیح stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    صحیح j = 0;
    برای( ; j <= m - stride2; j += stride2 )
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
    برای( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    برای( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
ایستا درونخطی تهی simdBlockMul_kj(
    ثابت ST* a_data, ثابت ST* b_data, size_t b_step,
    دوگانه* d_data, صحیح n, صحیح m, منطقی do_acc)
{
    ثابت صحیح vlanes = VTraits<v_float64>::vlanes();
    اگر( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    برای( صحیح k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (دوگانه)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> ایستا تهی
GEMMBlockMul( ثابت T* a_data, size_t a_step,
              ثابت T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, صحیح flags )
{
    صحیح i, j, k, n = a_size.width, m = d_size.width;
    ثابت T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    صحیح do_acc = flags & 16;

    a_step /= اندازه(a_data[0]);
    b_step /= اندازه(b_data[0]);
    d_step /= اندازه(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    اگر( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    اگر( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        برای( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            اگر( a_buf )
            {
                برای( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            برای( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#اگر CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                اگر( اندازه(WT) == اندازه(دوگانه) )
                {
                    اگر( اندازه(T) == اندازه(دوگانه) )
                        s0 += (WT)simdDotProduct((ثابت دوگانه*)a_data, (ثابت دوگانه*)b_data, n, k);
                    وگرنه اگر( اندازه(T) == اندازه(اعشاری) )
                        s0 += (WT)simdDotProduct_f32f64((ثابت اعشاری*)a_data, (ثابت اعشاری*)b_data, n, k);
                }
                وگرنه
#endif
                {
                    WT s1(0);
                    برای( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                برای( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    وگرنه
    {
        برای( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            اگر( a_buf )
            {
                برای( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            اگر( اندازه(WT) == اندازه(دوگانه) )
            {
                اگر( اندازه(T) == اندازه(دوگانه) )
                    simdBlockMul_kj(
                        (ثابت دوگانه*)a_data, (ثابت دوگانه*)b_data, b_step,
                        (دوگانه*)d_data, n, m, do_acc != 0);
                وگرنه
                    simdBlockMul_kj(
                        (ثابت اعشاری*)a_data, (ثابت اعشاری*)b_data, b_step,
                        (دوگانه*)d_data, n, m, do_acc != 0);
            }
            وگرنه
#endif
            {
                برای( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ثابت T* b = b_data + j;

                    اگر( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    وگرنه
                        s0 = s1 = s2 = s3 = WT(0);

                    برای( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                برای( ; j < m; j++ )
                {
                    ثابت T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    برای( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> ایستا تهی
GEMMStore( ثابت T* c_data, size_t c_step,
           ثابت WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           دوگانه alpha, دوگانه beta, صحیح flags )
{
    ثابت T* _c_data = c_data;
    صحیح j;
    size_t c_step0, c_step1;

    c_step /= اندازه(c_data[0]);
    d_buf_step /= اندازه(d_buf[0]);
    d_step /= اندازه(d_data[0]);

    اگر( !c_data )
        c_step0 = c_step1 = 0;
    وگرنه اگر( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    وگرنه
        c_step0 = 1, c_step1 = c_step;

    برای( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        اگر( _c_data )
        {
            c_data = _c_data;
            j=0;
             #اگر CV_ENABLE_UNROLLED
            برای(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
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
            برای( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        وگرنه
        {
            j = 0;
             #اگر CV_ENABLE_UNROLLED
            برای( ; j <= d_size.width - 4; j += 4 )
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
            برای( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


تعریفنوع تهی (*GEMMSingleMulFunc)( ثابت تهی* src1, size_t step1,
                   ثابت تهی* src2, size_t step2, ثابت تهی* src3, size_t step3,
                   تهی* dst, size_t dststep, Size srcsize, Size dstsize,
                   دوگانه alpha, دوگانه beta, صحیح flags );

تعریفنوع تهی (*GEMMBlockMulFunc)( ثابت تهی* src1, size_t step1,
                   ثابت تهی* src2, size_t step2, تهی* dst, size_t dststep,
                   Size srcsize, Size dstsize, صحیح flags );

تعریفنوع تهی (*GEMMStoreFunc)( ثابت تهی* src1, size_t step1,
                   ثابت تهی* src2, size_t step2, تهی* dst, size_t dststep,
                   Size dstsize, دوگانه alpha, دوگانه beta, صحیح flags );

template<typename T, typename WT, تهی (*fn)(ثابت T*, size_t, ثابت T*, size_t, ثابت T*, size_t, T*, size_t, Size, Size, دوگانه, دوگانه, صحیح)>
ایستا تهی gemmSingleMulWrap( ثابت تهی* src1, size_t step1,
                   ثابت تهی* src2, size_t step2, ثابت تهی* src3, size_t step3,
                   تهی* dst, size_t dststep, Size srcsize, Size dstsize,
                   دوگانه alpha, دوگانه beta, صحیح flags )
{
    fn((ثابت T*)src1, step1, (ثابت T*)src2, step2, (ثابت T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, تهی (*fn)(ثابت T*, size_t, ثابت T*, size_t, WT*, size_t, Size, Size, صحیح)>
ایستا تهی gemmBlockMulWrap( ثابت تهی* src1, size_t step1,
                   ثابت تهی* src2, size_t step2, تهی* dst, size_t dststep,
                   Size srcsize, Size dstsize, صحیح flags )
{
    fn((ثابت T*)src1, step1, (ثابت T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, تهی (*fn)(ثابت T*, size_t, ثابت WT*, size_t, T*, size_t, Size, دوگانه, دوگانه, صحیح)>
ایستا تهی gemmStoreWrap( ثابت تهی* src1, size_t step1,
                   ثابت تهی* src2, size_t step2, تهی* dst, size_t dststep,
                   Size dstsize, دوگانه alpha, دوگانه beta, صحیح flags )
{
    fn((ثابت T*)src1, step1, (ثابت WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

ایستا تهی GEMMSingleMul_32f( ثابت اعشاری* a_data, size_t a_step,
              ثابت اعشاری* b_data, size_t b_step,
              ثابت اعشاری* c_data, size_t c_step,
              اعشاری* d_data, size_t d_step,
              Size a_size, Size d_size,
              دوگانه alpha, دوگانه beta, صحیح flags )
{
    GEMMSingleMul<اعشاری,دوگانه>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

ایستا تهی GEMMSingleMul_64f( ثابت دوگانه* a_data, size_t a_step,
                              ثابت دوگانه* b_data, size_t b_step,
                              ثابت دوگانه* c_data, size_t c_step,
                              دوگانه* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              دوگانه alpha, دوگانه beta, صحیح flags )
{
    GEMMSingleMul<دوگانه,دوگانه>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


ایستا تهی GEMMSingleMul_32fc( ثابت Complexf* a_data, size_t a_step,
                              ثابت Complexf* b_data, size_t b_step,
                              ثابت Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              دوگانه alpha, دوگانه beta, صحیح flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

ایستا تهی GEMMSingleMul_64fc( ثابت Complexd* a_data, size_t a_step,
                              ثابت Complexd* b_data, size_t b_step,
                              ثابت Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              دوگانه alpha, دوگانه beta, صحیح flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

ایستا تهی GEMMBlockMul_32f( ثابت اعشاری* a_data, size_t a_step,
             ثابت اعشاری* b_data, size_t b_step,
             دوگانه* d_data, size_t d_step,
             Size a_size, Size d_size, صحیح flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ایستا تهی GEMMBlockMul_64f( ثابت دوگانه* a_data, size_t a_step,
                             ثابت دوگانه* b_data, size_t b_step,
                             دوگانه* d_data, size_t d_step,
                             Size a_size, Size d_size, صحیح flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ایستا تهی GEMMBlockMul_32fc( ثابت Complexf* a_data, size_t a_step,
                             ثابت Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, صحیح flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ایستا تهی GEMMBlockMul_64fc( ثابت Complexd* a_data, size_t a_step,
                             ثابت Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, صحیح flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


ایستا تهی GEMMStore_32f( ثابت اعشاری* c_data, size_t c_step,
          ثابت دوگانه* d_buf, size_t d_buf_step,
          اعشاری* d_data, size_t d_step, Size d_size,
          دوگانه alpha, دوگانه beta, صحیح flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ایستا تهی GEMMStore_64f( ثابت دوگانه* c_data, size_t c_step,
                      ثابت دوگانه* d_buf, size_t d_buf_step,
                      دوگانه* d_data, size_t d_step, Size d_size,
                      دوگانه alpha, دوگانه beta, صحیح flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ایستا تهی GEMMStore_32fc( ثابت Complexf* c_data, size_t c_step,
                          ثابت Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          دوگانه alpha, دوگانه beta, صحیح flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


ایستا تهی GEMMStore_64fc( ثابت Complexd* c_data, size_t c_step,
                          ثابت Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          دوگانه alpha, دوگانه beta, صحیح flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

ایستا تهی gemmImpl( Mat A, Mat B, دوگانه alpha,
           Mat C, دوگانه beta, Mat D, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح block_lin_size = 128;
    ثابت صحیح block_size = block_lin_size * block_lin_size;

    ایستا دوگانه zero[] = {0,0,0,0};
    ایستا اعشاری zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    صحیح i, len = 0, type = A.type();

    انتخاب( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالت 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        بشکن;
    حالت 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        بشکن;
    حالت 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        بشکن;
    حالت 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        بشکن;
    }

    اگر( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        اگر( type == CV_32F )
        {
            اعشاری* d = D.ptr<اعشاری>();
            ثابت اعشاری *a = A.ptr<اعشاری>(),
                        *b = B.ptr<اعشاری>(),
                        *c = (ثابت اعشاری*)C.data;
            size_t d_step = D.step/اندازه(d[0]),
                a_step = A.step/اندازه(a[0]),
                b_step = B.step/اندازه(b[0]),
                c_step = C.data ? C.step/اندازه(c[0]) : 0;

            اگر( !c )
                c = zerof;

            انتخاب( len )
            {
            حالت 2:
                اگر( len == d_size.width && b != d )
                {
                    برای( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_step];
                        اعشاری t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[1] = (اعشاری)(t1*alpha + c[1]*beta);
                    }
                }
                وگرنه اگر( a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    برای( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_step];
                        اعشاری t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[d_step] = (اعشاری)(t1*alpha + c[c_step]*beta);
                    }
                }
                وگرنه
                    بشکن;
                بازگشت;
            حالت 3:
                اگر( len == d_size.width && b != d )
                {
                    برای( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        اعشاری t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        اعشاری t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[1] = (اعشاری)(t1*alpha + c[1]*beta);
                        d[2] = (اعشاری)(t2*alpha + c[2]*beta);
                    }
                }
                وگرنه اگر( a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    برای( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        اعشاری t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        اعشاری t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[d_step] = (اعشاری)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (اعشاری)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                وگرنه
                    بشکن;
                بازگشت;
            حالت 4:
                اگر( len == d_size.width && b != d )
                {
                    برای( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        اعشاری t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        اعشاری t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        اعشاری t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[1] = (اعشاری)(t1*alpha + c[1]*beta);
                        d[2] = (اعشاری)(t2*alpha + c[2]*beta);
                        d[3] = (اعشاری)(t3*alpha + c[3]*beta);
                    }
                }
                وگرنه اگر( len <= 16 && a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    برای( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        اعشاری t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        اعشاری t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        اعشاری t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[d_step] = (اعشاری)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (اعشاری)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (اعشاری)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                وگرنه
                    بشکن;
                بازگشت;
            }
        }

        اگر( type == CV_64F )
        {
            دوگانه* d = D.ptr<دوگانه>();
            ثابت دوگانه *a = A.ptr<دوگانه>(),
                         *b = B.ptr<دوگانه>(),
                         *c = (ثابت دوگانه*)C.data;
            size_t d_step = D.step/اندازه(d[0]),
                a_step = A.step/اندازه(a[0]),
                b_step = B.step/اندازه(b[0]),
                c_step = C.data ? C.step/اندازه(c[0]) : 0;
            اگر( !c )
                c = zero;

            انتخاب( len )
            {
            حالت 2:
                اگر( len == d_size.width && b != d )
                {
                    برای( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        دوگانه t0 = a[0]*b[0] + a[1]*b[b_step];
                        دوگانه t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                وگرنه اگر( a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    برای( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        دوگانه t0 = a[0]*b[0] + a[1]*b[b_step];
                        دوگانه t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                وگرنه
                    بشکن;
                بازگشت;
            حالت 3:
                اگر( len == d_size.width && b != d )
                {
                    برای( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        دوگانه t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        دوگانه t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        دوگانه t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                وگرنه اگر( a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    برای( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        دوگانه t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        دوگانه t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        دوگانه t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                وگرنه
                    بشکن;
                بازگشت;
            حالت 4:
                اگر( len == d_size.width && b != d )
                {
                    برای( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        دوگانه t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        دوگانه t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        دوگانه t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        دوگانه t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                وگرنه اگر( d_size.width <= 16 && a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    برای( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        دوگانه t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        دوگانه t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        دوگانه t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        دوگانه t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                وگرنه
                    بشکن;
                بازگشت;
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

    اگر( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<اعشاری, دوگانه, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<اعشاری, دوگانه, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<اعشاری, دوگانه, GEMMStore_32f>;
    }
    وگرنه اگر( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<دوگانه, دوگانه, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<دوگانه, دوگانه, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<دوگانه, دوگانه, GEMMStore_64f>;
    }
    وگرنه اگر( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    وگرنه
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    اگر( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
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
    else*/ اگر( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    وگرنه
    {
        صحیح is_a_t = flags & GEMM_1_T;
        صحیح is_b_t = flags & GEMM_2_T;
        صحیح elem_size = CV_ELEM_SIZE(type);
        صحیح dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        صحیح j, k, di = 0, dj = 0, dk = 0;
        صحیح dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        صحیح work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        اگر( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        وگرنه
            a_step0 = elem_size, a_step1 = A.step;

        اگر( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        وگرنه
            b_step0 = elem_size, b_step1 = b_step;

        اگر( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        وگرنه اگر( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        وگرنه
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        اگر( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        اگر( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        اگر( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        اگر( is_a_t )
            a_buf = b_buf + b_buf_size;

        برای( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            اگر( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            برای( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                ثابت uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                اگر( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                اگر( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                برای( k = 0; k < len; k += dk )
                {
                    ثابت uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    ثابت uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    اگر( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    اگر( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    وگرنه
                        a_bl_size.width = di, a_bl_size.height = dk;

                    اگر( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    اگر( dj < d_size.width )
                    {
                        Size b_size;
                        اگر( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        وگرنه
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    اگر( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    وگرنه
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                اگر( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>درونخطی ایستا تهی
callGemmImpl(ثابت fptype *src1, size_t src1_step, ثابت fptype *src2, size_t src2_step, fptype alpha,
          ثابت fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags, صحیح type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    صحیح b_m, b_n, c_m, c_n, m_d;

    اگر(flags & GEMM_2_T)
    {
        b_m = n_d;
        اگر(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        وگرنه
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    وگرنه
    {
        b_n = n_d;
        اگر(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        وگرنه
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    اگر(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    وگرنه
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    اگر(src1 != پوچ)
        A = Mat(m_a, n_a, type, (تهی*)src1, src1_step);
    اگر(src2 != پوچ)
        B = Mat(b_m, b_n, type, (تهی*)src2, src2_step);
    اگر(src3 != پوچ && beta != 0.0)
        C = Mat(c_m, c_n, type, (تهی*)src3, src3_step);
    Mat D(m_d, n_d, type, (تهی*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

تهی gemm32f(ثابت اعشاری* src1, size_t src1_step, ثابت اعشاری* src2, size_t src2_step,
             اعشاری alpha, ثابت اعشاری* src3, size_t src3_step, اعشاری beta, اعشاری* dst, size_t dst_step,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

تهی gemm64f(ثابت دوگانه* src1, size_t src1_step, ثابت دوگانه* src2, size_t src2_step,
             دوگانه alpha, ثابت دوگانه* src3, size_t src3_step, دوگانه beta, دوگانه* dst, size_t dst_step,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

تهی gemm32fc(ثابت اعشاری* src1, size_t src1_step, ثابت اعشاری* src2, size_t src2_step,
              اعشاری alpha, ثابت اعشاری* src3, size_t src3_step, اعشاری beta, اعشاری* dst, size_t dst_step,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

تهی gemm64fc(ثابت دوگانه* src1, size_t src1_step, ثابت دوگانه* src2, size_t src2_step,
              دوگانه alpha, ثابت دوگانه* src3, size_t src3_step, دوگانه beta, دوگانه* dst, size_t dst_step,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> ایستا تهی
transform_( ثابت T* src, T* dst, ثابت WT* m, صحیح len, صحیح scn, صحیح dcn )
{
    صحیح x;

    اگر( scn == 2 && dcn == 2 )
    {
        برای( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    وگرنه اگر( scn == 3 && dcn == 3 )
    {
        برای( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    وگرنه اگر( scn == 3 && dcn == 1 )
    {
        برای( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    وگرنه اگر( scn == 4 && dcn == 4 )
    {
        برای( x = 0; x < len*4; x += 4 )
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
    وگرنه
    {
        برای( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ثابت WT* _m = m;
            صحیح j, k;
            برای( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                برای( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

ایستا تهی
transform_8u( ثابت uchar* src, uchar* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn )
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح BITS = 10, SCALE = 1 << BITS;
    ثابت اعشاری MAX_M = (اعشاری)(1 << (15 - BITS));

    اگر( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ثابت صحیح nChannels = 3;

        اجتماع {
            short s[6];
            صحیح p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        صحیح m32[] = {saturate_cast<صحیح>(m[ 2] * SCALE), saturate_cast<صحیح>(m[ 3] * SCALE),
                     saturate_cast<صحیح>(m[ 6] * SCALE), saturate_cast<صحیح>(m[ 7] * SCALE),
                     saturate_cast<صحیح>(m[10] * SCALE), saturate_cast<صحیح>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        صحیح x = 0;
        برای (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<صحیح>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<صحیح>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<صحیح>((m[11] + 0.5f)*SCALE);
        برای( ; x < len * nChannels; x += nChannels )
        {
            صحیح v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
transform_16u( ثابت ushort* src, ushort* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn )
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( scn == 3 && dcn == 3 )
    {
        صحیح x = 0;
#اگر CV_SIMD_WIDTH > 16
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
        برای (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#اگر CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_چپ<1>(_m0l);
        v_float32x4 _m1h = v_rotate_چپ<1>(_m1l);
        v_float32x4 _m2h = v_rotate_چپ<1>(_m2l);
        v_float32x4 _m3h = v_rotate_چپ<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        برای( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_راست<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        برای( ; x < len * 3; x += 3 )
        {
            اعشاری v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
transform_32f( ثابت اعشاری* src, اعشاری* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#اگر (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    صحیح x = 0;
    اگر( scn == 3 && dcn == 3 )
    {
        صحیح idx[VTraits<v_float32>::max_nlanes/2];
        برای( صحیح i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        اعشاری _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        برای( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        برای( ; x < len*3; x += 3 )
        {
            اعشاری v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            اعشاری t0 = saturate_cast<اعشاری>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            اعشاری t1 = saturate_cast<اعشاری>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            اعشاری t2 = saturate_cast<اعشاری>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        بازگشت;
    }

    اگر( scn == 4 && dcn == 4 )
    {
#اگر CV_SIMD_WIDTH > 16
        صحیح idx[VTraits<v_float32>::max_nlanes/4];
        برای( صحیح i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        اعشاری _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        برای( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#اگر CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        برای( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#وگرنه // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        برای( ; x < len*4; x += 4 )
        {
            اعشاری v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            اعشاری t0 = saturate_cast<اعشاری>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            اعشاری t1 = saturate_cast<اعشاری>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            اعشاری t2 = saturate_cast<اعشاری>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            اعشاری t3 = saturate_cast<اعشاری>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        بازگشت;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


ایستا تهی
transform_8s(ثابت schar* src, schar* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح BITS = 10, SCALE = 1 << BITS;
    ثابت اعشاری MAX_M = (اعشاری)(1 << (15 - BITS));

    اگر( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        ثابت صحیح nChannels = 3;

        اجتماع {
            short s[6];
            صحیح p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        صحیح m32[] = {saturate_cast<صحیح>(m[ 2] * SCALE), saturate_cast<صحیح>(m[ 3] * SCALE),
                     saturate_cast<صحیح>(m[ 6] * SCALE), saturate_cast<صحیح>(m[ 7] * SCALE),
                     saturate_cast<صحیح>(m[10] * SCALE), saturate_cast<صحیح>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        صحیح x = 0;
        برای (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<صحیح>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<صحیح>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<صحیح>((m[11] + 0.5f)*SCALE);
        برای( ; x < len * nChannels; x += nChannels )
        {
            صحیح v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
transform_16s(ثابت short* src, short* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (scn == 3 && dcn == 3) {
        صحیح x = 0;

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
        برای (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        برای( ; x < len * 3; x += 3 )
        {
            اعشاری v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
transform_32s(ثابت صحیح* src, صحیح* dst, ثابت دوگانه* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    اگر( scn == 3 && dcn == 3) {
        صحیح x = 0;

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
        برای (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_بالا(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_بالا(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_بالا(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        برای (; x < len * 3; x += 3) {
            دوگانه b = src[x], g = src[x + 1], r = src[x + 2];
            صحیح db = saturate_cast<صحیح>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            صحیح dg = saturate_cast<صحیح>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            صحیح dr = saturate_cast<صحیح>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
transform_64f(ثابت دوگانه* src, دوگانه* dst, ثابت دوگانه* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    اگر( scn == 3 && dcn == 3) {
        صحیح x = 0;

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
        برای (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        برای (; x < len * 3; x += 3) {
            دوگانه b = src[x], g = src[x + 1], r = src[x + 2];
            دوگانه db = saturate_cast<دوگانه>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            دوگانه dg = saturate_cast<دوگانه>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            دوگانه dr = saturate_cast<دوگانه>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> ایستا تهی
diagtransform_( ثابت T* src, T* dst, ثابت WT* m, صحیح len, صحیح cn, صحیح )
{
    صحیح x;

    اگر( cn == 2 )
    {
        برای( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    وگرنه اگر( cn == 3 )
    {
        برای( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    وگرنه اگر( cn == 4 )
    {
        برای( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    وگرنه
    {
        برای( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ثابت WT* _m = m;
            برای( صحیح j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

ایستا تهی
diagtransform_8u(ثابت uchar* src, uchar* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (scn == 3 && dcn == 3) {
        صحیح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        برای (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        برای (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
diagtransform_8s(ثابت schar* src, schar* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
{
    #اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (scn == 3 && dcn == 3) {
        صحیح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        برای (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        برای (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
diagtransform_16u(ثابت ushort* src, ushort* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (scn == 3 && dcn == 3) {
        صحیح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        برای (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        برای (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
diagtransform_16s(ثابت short* src, short* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (scn == 3 && dcn == 3) {
        صحیح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        برای (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        برای (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
diagtransform_32s(ثابت صحیح* src, صحیح* dst, ثابت دوگانه* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    اگر (scn == 3 && dcn == 3) {
        صحیح x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        برای (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_بالا(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_بالا(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_بالا(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        برای (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            صحیح db = saturate_cast<صحیح>(m[ 0] * b + m[ 3]);
            صحیح dg = saturate_cast<صحیح>(m[ 5] * g + m[ 7]);
            صحیح dr = saturate_cast<صحیح>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
diagtransform_32f(ثابت اعشاری* src, اعشاری* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (scn == 3 && dcn == 3) {
        صحیح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        برای (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        برای (; x < len * 3; x += 3) {
            اعشاری b = src[x], g = src[x + 1], r = src[x + 2];
            اعشاری db = saturate_cast<اعشاری>(m[ 0] * b + m[ 3]);
            اعشاری dg = saturate_cast<اعشاری>(m[ 5] * g + m[ 7]);
            اعشاری dr = saturate_cast<اعشاری>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

ایستا تهی
diagtransform_64f(ثابت دوگانه* src, دوگانه* dst, ثابت دوگانه* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    اگر (scn == 3 && dcn == 3) {
        صحیح x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        برای (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        برای (; x < len * 3; x += 3) {
            دوگانه b = src[x], g = src[x + 1], r = src[x + 2];
            دوگانه db = saturate_cast<دوگانه>(m[ 0] * b + m[ 3]);
            دوگانه dg = saturate_cast<دوگانه>(m[ 5] * g + m[ 7]);
            دوگانه dr = saturate_cast<دوگانه>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        بازگشت;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, تهی (*fn)(ثابت T*, T*, ثابت WT*, صحیح, صحیح, صحیح)>
ایستا تهی transformWrap(ثابت تهی* src, تهی* dst, ثابت تهی* m, صحیح len, صحیح scn, صحیح dcn)
{
    fn((ثابت T*)src, (T*)dst, (ثابت WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(صحیح depth)
{
    ایستا TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, اعشاری, transform_8u>, transformWrap<schar, اعشاری, transform_8s>, transformWrap<ushort, اعشاری, transform_16u>,
        transformWrap<short, اعشاری, transform_16s>, transformWrap<صحیح, دوگانه, transform_32s>, transformWrap<اعشاری, اعشاری, transform_32f>,
        transformWrap<دوگانه, دوگانه, transform_64f>, 0
    };

    بازگشت transformTab[depth];
}

TransformFunc getDiagTransformFunc(صحیح depth)
{
    ایستا TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, اعشاری, diagtransform_8u>, transformWrap<schar, اعشاری, diagtransform_8s>, transformWrap<ushort, اعشاری, diagtransform_16u>,
        transformWrap<short, اعشاری, diagtransform_16s>, transformWrap<صحیح, دوگانه, diagtransform_32s>, transformWrap<اعشاری, اعشاری, diagtransform_32f>,
        transformWrap<دوگانه, دوگانه, diagtransform_64f>, 0
    };

    بازگشت diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> ایستا تهی
perspectiveTransform_( ثابت تهی* _src, تهی* _dst, ثابت تهی* _m_ptr, صحیح len, صحیح scn, صحیح dcn )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    ثابت دوگانه* m = (ثابت دوگانه*)_m_ptr;
    ثابت دوگانه eps = FLT_EPSILON;
    صحیح i;

    اگر( scn == 2 && dcn == 2 )
    {
        برای( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            دوگانه w = x*m[6] + y*m[7] + m[8];

            اگر( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            وگرنه
                dst[i] = dst[i+1] = (T)0;
        }
    }
    وگرنه اگر( scn == 3 && dcn == 3 )
    {
        برای( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            دوگانه w = x*m[12] + y*m[13] + z*m[14] + m[15];

            اگر( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            وگرنه
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    وگرنه اگر( scn == 3 && dcn == 2 )
    {
        برای( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            دوگانه w = x*m[8] + y*m[9] + z*m[10] + m[11];

            اگر( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            وگرنه
                dst[0] = dst[1] = (T)0;
        }
    }
    وگرنه
    {
        برای( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ثابت دوگانه* _m = m + dcn*(scn + 1);
            دوگانه w = _m[scn];
            صحیح j, k;
            برای( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            اگر( fabs(w) > eps )
            {
                _m = m;
                برای( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    دوگانه s = _m[scn];
                    برای( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            وگرنه
                برای( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(صحیح depth)
{
    اگر (depth == CV_32F)
        بازگشت perspectiveTransform_<اعشاری>;
    اگر (depth == CV_64F)
        بازگشت perspectiveTransform_<دوگانه>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

ایستا تهی scaleAdd_32f(ثابت تهی* _src1, ثابت تهی* _src2, تهی* _dst,
                         صحیح len, ثابت تهی* _alpha)
{
    ثابت اعشاری* src1 = (ثابت اعشاری*)_src1;
    ثابت اعشاری* src2 = (ثابت اعشاری*)_src2;
    اعشاری* dst = (اعشاری*)_dst;
    اعشاری alpha = *(ثابت اعشاری*)_alpha;
    صحیح i = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ثابت صحیح cWidth = VTraits<v_float32>::vlanes();
    برای (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    برای (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


ایستا تهی scaleAdd_64f(ثابت تهی* _src1, ثابت تهی* _src2, تهی* _dst,
                         صحیح len, ثابت تهی* _alpha)
{
    ثابت دوگانه* src1 = (ثابت دوگانه*)_src1;
    ثابت دوگانه* src2 = (ثابت دوگانه*)_src2;
    دوگانه* dst = (دوگانه*)_dst;
    دوگانه alpha = *(ثابت دوگانه*)_alpha;
    صحیح i = 0;
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ثابت صحیح cWidth = VTraits<v_float64>::vlanes();
    برای (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    برای (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(صحیح depth)
{
    اگر (depth == CV_32F)
        بازگشت scaleAdd_32f;
    اگر (depth == CV_64F)
        بازگشت scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> ایستا درونخطی
دوگانه MahalanobisImpl(ثابت Mat& v1, ثابت Mat& v2, ثابت Mat& icovar, دوگانه *diff_buffer /*[len]*/, صحیح len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    دوگانه result = 0;

    sz.width *= v1.channels();
    اگر (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        ثابت T* src1 = v1.ptr<T>();
        ثابت T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/اندازه(src1[0]);
        size_t step2 = v2.step/اندازه(src2[0]);
        دوگانه* diff = diff_buffer;
        ثابت T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/اندازه(mat[0]);

        برای (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            برای (صحیح i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        برای (صحیح i = 0; i < len; i++, mat += matstep)
        {
            دوگانه row_sum = 0;
            صحیح j = 0;
#اگر CV_ENABLE_UNROLLED
            برای(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            برای (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    بازگشت result;
}

MahalanobisImplFunc getMahalanobisImplFunc(صحیح depth)
{
    اگر (depth == CV_32F)
        بازگشت (MahalanobisImplFunc)MahalanobisImpl<اعشاری>;
    اگر (depth == CV_64F)
        بازگشت (MahalanobisImplFunc)MahalanobisImpl<دوگانه>;
    CV_Assert(0 && "Not supported");
}


#اگر !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> ایستا تهی
MulTransposedR(ثابت Mat& srcmat, ثابت Mat& dstmat, ثابت Mat& deltamat, دوگانه scale)
{
    صحیح i, j, k;
    ثابت sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ثابت dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/اندازه(src[0]);
    size_t dststep = dstmat.step/اندازه(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/اندازه(delta[0]) : 0;
    صحیح delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    صحیح buf_size = size.height*اندازه(dT);
    AutoBuffer<uchar> buf;

    اگر( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    اگر( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        برای( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#اگر CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    اگر( !delta )
        برای( i = 0; i < size.width; i++, tdst += dststep )
        {
            برای( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            برای( j = i; j <= size.width - 4; j += 4 )
            {
#اگر CV_SIMD128_64F
                اگر (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ثابت دوگانه *tsrc = (دوگانه*)(src + j);

                    برای( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((دوگانه)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((دوگانه*)(tdst+j), v_mul(s0, v_scale));
                    v_store((دوگانه*)(tdst+j+2), v_mul(s1, v_scale));
                } وگرنه
#endif
                {
                    دوگانه s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ثابت sT *tsrc = src + j;

                    برای( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        دوگانه a = col_buf[k];
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

            برای( ; j < size.width; j++ )
            {
                دوگانه s0 = 0;
                ثابت sT *tsrc = src + j;

                برای( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (دوگانه)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    وگرنه
        برای( i = 0; i < size.width; i++, tdst += dststep )
        {
            اگر( !delta_buf )
                برای( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            وگرنه
                برای( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            برای( j = i; j <= size.width - 4; j += 4 )
            {
#اگر CV_SIMD128_64F
                اگر (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ثابت دوگانه *tsrc = (دوگانه*)(src + j);
                    ثابت دوگانه *d = (دوگانه*)(delta_buf ? delta_buf : delta + j);

                    برای( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((دوگانه)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((دوگانه*)(tdst+j), v_mul(s0, v_scale));
                    v_store((دوگانه*)(tdst+j+2), v_mul(s1, v_scale));
                }
                وگرنه
#endif

                {
                    دوگانه s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ثابت sT *tsrc = src + j;
                    ثابت dT *d = delta_buf ? delta_buf : delta + j;

                    برای( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        دوگانه a = col_buf[k];
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

            برای( ; j < size.width; j++ )
            {
                دوگانه s0 = 0;
                ثابت sT *tsrc = src + j;
                ثابت dT *d = delta_buf ? delta_buf : delta + j;

                برای( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (دوگانه)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> ایستا تهی
MulTransposedL(ثابت Mat& srcmat, ثابت Mat& dstmat, ثابت Mat& deltamat, دوگانه scale)
{
    صحیح i, j, k;
    ثابت sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ثابت dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/اندازه(src[0]);
    size_t dststep = dstmat.step/اندازه(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/اندازه(delta[0]) : 0;
    صحیح delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    اگر( !delta )
        برای( i = 0; i < size.height; i++, tdst += dststep )
            برای( j = i; j < size.height; j++ )
            {
                دوگانه s = 0;
                ثابت sT *tsrc1 = src + i*srcstep;
                ثابت sT *tsrc2 = src + j*srcstep;
#اگر CV_SIMD128_64F
                اگر (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ثابت دوگانه *v_tsrc1 = (دوگانه *)(tsrc1);
                    ثابت دوگانه *v_tsrc2 = (دوگانه *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    برای( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                وگرنه
#endif
                {
                    برای( k = 0; k <= size.width - 4; k += 4 )
                        s += (دوگانه)tsrc1[k]*tsrc2[k] + (دوگانه)tsrc1[k+1]*tsrc2[k+1] +
                             (دوگانه)tsrc1[k+2]*tsrc2[k+2] + (دوگانه)tsrc1[k+3]*tsrc2[k+3];
                }
                برای( ; k < size.width; k++ )
                    s += (دوگانه)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    وگرنه
    {
        dT delta_buf[4];
        صحیح delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*اندازه(dT));
        dT* row_buf = (dT*)buf.data();

        برای( i = 0; i < size.height; i++, tdst += dststep )
        {
            ثابت sT *tsrc1 = src + i*srcstep;
            ثابت dT *tdelta1 = delta + i*deltastep;

            اگر( delta_cols < size.width )
                برای( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            وگرنه
                برای( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            برای( j = i; j < size.height; j++ )
            {
                دوگانه s = 0;
                ثابت sT *tsrc2 = src + j*srcstep;
                ثابت dT *tdelta2 = delta + j*deltastep;
                اگر( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#اگر CV_SIMD128_64F
                اگر (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    ثابت دوگانه *v_tsrc2 = (دوگانه *)(tsrc2);
                    ثابت دوگانه *v_tdelta2 = (دوگانه *)(tdelta2);
                    ثابت دوگانه *v_row_buf = (دوگانه *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    برای( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (ثابت dT *)(v_tdelta2);
                }
                وگرنه
#endif
                {
                    برای( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (دوگانه)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (دوگانه)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (دوگانه)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (دوگانه)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                برای( ; k < size.width; k++, tdelta2++ )
                    s += (دوگانه)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(صحیح stype, صحیح dtype, منطقی ata)
{
    MulTransposedFunc func = پوچ;
    اگر (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,اعشاری>
                   : MulTransposedL<uchar,اعشاری>;
    }
    وگرنه اگر (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,دوگانه>
                   : MulTransposedL<uchar,دوگانه>;
    }
    وگرنه اگر(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,اعشاری>
                   : MulTransposedL<ushort,اعشاری>;
    }
    وگرنه اگر(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,دوگانه>
                   : MulTransposedL<ushort,دوگانه>;
    }
    وگرنه اگر(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,اعشاری>
                   : MulTransposedL<short,اعشاری>;
    }
    وگرنه اگر(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,دوگانه>
                   : MulTransposedL<short,دوگانه>;
    }
    وگرنه اگر(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<اعشاری,اعشاری>
                   : MulTransposedL<اعشاری,اعشاری>;
    }
    وگرنه اگر(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<اعشاری,دوگانه>
                   : MulTransposedL<اعشاری,دوگانه>;
    }
    وگرنه اگر(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<دوگانه,دوگانه>
                   : MulTransposedL<دوگانه,دوگانه>;
    }
    CV_Assert(func && "Not supported");
    بازگشت func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> ایستا درونخطی
دوگانه dotProd_(ثابت T* src1, ثابت T* src2, صحیح len)
{
    صحیح i = 0;
    دوگانه result = 0;

#اگر CV_ENABLE_UNROLLED
    برای( ; i <= len - 4; i += 4 )
        result += (دوگانه)src1[i]*src2[i] + (دوگانه)src1[i+1]*src2[i+1] +
            (دوگانه)src1[i+2]*src2[i+2] + (دوگانه)src1[i+3]*src2[i+3];
#endif
    برای( ; i < len; i++ )
        result += (دوگانه)src1[i]*src2[i];

    بازگشت result;
}


دوگانه dotProd_8u(ثابت uchar* src1, ثابت uchar* src2, صحیح len)
{
    دوگانه r = 0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    وقتی (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        ثابت صحیح cWidth = VTraits<v_uint16>::vlanes();

        صحیح j = 0;
        برای (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        برای (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (دوگانه)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    بازگشت r + dotProd_(src1, src2, len - i);
}


دوگانه dotProd_8s(ثابت schar* src1, ثابت schar* src2, صحیح len)
{
    دوگانه r = 0.0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    وقتی (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        ثابت صحیح cWidth = VTraits<v_int16>::vlanes();

        صحیح j = 0;
        برای (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        برای (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (دوگانه)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    بازگشت r + dotProd_(src1, src2, len - i);
}

دوگانه dotProd_16u(ثابت ushort* src1, ثابت ushort* src2, صحیح len)
{
    دوگانه r = 0.0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    وقتی (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        ثابت صحیح cWidth = VTraits<v_uint16>::vlanes();

        صحیح j = 0;
        برای (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (دوگانه)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    بازگشت r + dotProd_(src1, src2, len - i);
}

دوگانه dotProd_16s(ثابت short* src1, ثابت short* src2, صحیح len)
{
    دوگانه r = 0.0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    وقتی (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        ثابت صحیح cWidth = VTraits<v_int16>::vlanes();

        صحیح j = 0;
        برای (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (دوگانه)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    بازگشت r + dotProd_(src1, src2, len - i);
}

دوگانه dotProd_32s(ثابت صحیح* src1, ثابت صحیح* src2, صحیح len)
{
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    دوگانه r = .0;
    صحیح i = 0;
    ثابت صحیح step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#اگر CV_SIMD_WIDTH == 16
    ثابت صحیح wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    برای (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    برای (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    بازگشت r + dotProd_(src1, src2, len - i);
#وگرنه
    بازگشت dotProd_(src1, src2, len);
#endif
}

دوگانه dotProd_32f(ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح len)
{
    دوگانه r = 0.0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    وقتی (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        صحیح j = 0;
        صحیح cWidth = VTraits<v_float32>::vlanes();

#اگر CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        برای (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
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

        برای (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    بازگشت r + dotProd_(src1, src2, len - i);
}

دوگانه dotProd_64f(ثابت دوگانه* src1, ثابت دوگانه* src2, صحیح len)
{
    بازگشت dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
