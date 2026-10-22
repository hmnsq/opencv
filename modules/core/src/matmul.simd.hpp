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

#جيڪڏهن_وصف_ٿيل HAVE_LAPACK
#وصف_ڪريو CV_GEMM_BASELINE_ONLY
#جيڪڏهن_پورو
#وصف_ڪريو CV_MAHALANOBIS_BASELINE_ONLY
#وصف_ڪريو CV_MULTRANSPOSED_BASELINE_ONLY

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
#جيڪڏهن_وصف_نه_ٿيل CV_GEMM_SIMD_ENABLED
#إذا defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#وصف_ڪريو CV_GEMM_SIMD_ENABLED 0
#وإلا
#وصف_ڪريو CV_GEMM_SIMD_ENABLED 1
#جيڪڏهن_پورو
#جيڪڏهن_پورو

نطاق cv {

// forward declarations
تعريفنوع فراغ (*TransformFunc)(ثابت فراغ* src, فراغ* dst, ثابت فراغ* m, صحيح len, صحيح scn, صحيح dcn);
تعريفنوع فراغ (*Scale_جوڙيو_Func)(ثابت فراغ* src1, ثابت فراغ* src2, فراغ* dst, صحيح len, ثابت فراغ* alpha);
تعريفنوع فراغ (*MulTransposedFunc)(ثابت Mat& src, ثابت/*preallocated*/ Mat& dst, ثابت Mat& delta, مزدوج scale);
تعريفنوع مزدوج (*MahalanobisImplFunc)(ثابت Mat& v1, ثابت Mat& v2, ثابت Mat& icovar, مزدوج *diff_بفر /*[len]*/, صحيح len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

فراغ gemm32f(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
             عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags);
فراغ gemm64f(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags);
فراغ gemm32fc(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
              عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags);
فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags);

TransformFunc getTransformFunc(صحيح اونهائي);
TransformFunc getDiagTransformFunc(صحيح اونهائي);
TransformFunc getPerspectiveTransform(صحيح اونهائي);

Scale_جوڙيو_Func get_Scale_جوڙيو_Func(صحيح اونهائي);

MahalanobisImplFunc getMahalanobisImplFunc(صحيح اونهائي);

MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata);

مزدوج dotProd_8u(ثابت uchar* src1, ثابت uchar* src2, صحيح len);
مزدوج dotProd_8s(ثابت schar* src1, ثابت schar* src2, صحيح len);
مزدوج dotProd_16u(ثابت ushort* src1, ثابت ushort* src2, صحيح len);
مزدوج dotProd_16s(ثابت ننڍو* src1, ثابت ننڍو* src2, صحيح len);
مزدوج dotProd_32s(ثابت صحيح* src1, ثابت صحيح* src2, صحيح len);
مزدوج dotProd_32f(ثابت عائم* src1, ثابت عائم* src2, صحيح len);
مزدوج dotProd_64f(ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح len);



#جيڪڏهن_وصف_نه_ٿيل CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#إذا !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_گهڻائي)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

ساكن فراغ
GEMM_Copy_بلاڪ( ثابت uchar* src, size_t src_قدم,
                uchar* dst, size_t dst_قدم,
                سائيز size, size_t pix_سائيز )
{
    صحيح j;
    size.ويڪر *= (صحيح)(pix_سائيز / حجم(صحيح));

    لكل( ; size.اوچائي--; src += src_قدم, dst += dst_قدم )
    {
        j=0;
         #إذا CV_ENABLE_UNROLLED
        لكل( ; j <= size.ويڪر - 4; j += 4 )
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
        #جيڪڏهن_پورو
        لكل( ; j < size.ويڪر; j++ )
            ((صحيح*)dst)[j] = ((ثابت صحيح*)src)[j];
    }
}


ساكن فراغ
GEMM_Transpose_بلاڪ( ثابت uchar* src, size_t src_قدم,
                     uchar* dst, size_t dst_قدم,
                     سائيز size, size_t pix_سائيز )
{
    صحيح i, j;
    لكل( i = 0; i < size.ويڪر; i++, dst += dst_قدم, src += pix_سائيز )
    {
        ثابت uchar* _src = src;
        بدل( pix_سائيز )
        {
        حالة حجم(صحيح):
            لكل( j = 0; j < size.اوچائي; j++, _src += src_قدم )
                ((صحيح*)dst)[j] = ((صحيح*)_src)[0];
            اكسر;
        حالة حجم(صحيح)*2:
            لكل( j = 0; j < size.اوچائي*2; j += 2, _src += src_قدم )
            {
                صحيح t0 = ((صحيح*)_src)[0];
                صحيح t1 = ((صحيح*)_src)[1];
                ((صحيح*)dst)[j] = t0;
                ((صحيح*)dst)[j+1] = t1;
            }
            اكسر;
        حالة حجم(صحيح)*4:
            لكل( j = 0; j < size.اوچائي*4; j += 4, _src += src_قدم )
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
            أرجع;
        }
    }
}

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
سانچو<قسم_نالو ST> بنية VecTraits;

سانچو<> بنية VecTraits<عائم> {
    تعريفنوع v_float32 VT;
    ساكن مضمن VT zero() { أرجع vx_setzero_f32(); }
    ساكن مضمن VT load(ثابت عائم* p) { أرجع vx_ڀريو(p); }
    ساكن مضمن VT setall(عائم v) { أرجع vx_setall_f32(v); }
};
#جيڪڏهن_پورو

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
سانچو<> بنية VecTraits<مزدوج> {
    تعريفنوع v_float64 VT;
    ساكن مضمن VT zero() { أرجع vx_setzero_f64(); }
    ساكن مضمن VT load(ثابت مزدوج* p) { أرجع vx_ڀريو(p); }
    ساكن مضمن VT setall(مزدوج v) { أرجع vx_setall_f64(v); }
};
#جيڪڏهن_پورو

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
سانچو<قسم_نالو ST>
ساكن مضمن ST simd_Dot_حاصل_ضرب(ثابت ST* a, ثابت ST* b, صحيح n, صحيح& k)
{
    تعريفنوع قسم_نالو VecTraits<ST>::VT VecT;
    ثابت صحيح vlanes = VTraits<VecT>::vlanes();
    ثابت صحيح stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    لكل( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_جوڙيو(v_جوڙيو(vs0, vs1), v_جوڙيو(vs2, vs3));
    لكل( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    أرجع v_reduce_جوڙ(vs0);
}
#جيڪڏهن_پورو

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
ساكن مضمن مزدوج simd_Dot_حاصل_ضرب_f32f64(ثابت عائم* a, ثابت عائم* b, صحيح n, صحيح& k)
{
    ثابت صحيح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحيح stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    لكل( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_ڀريو(a + k);
        v_float32 vb0 = vx_ڀريو(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_مٿاهون(va0), v_cvt_f64_مٿاهون(vb0), vs1);

        v_float32 va1 = vx_ڀريو(a + k + vlanes_f32);
        v_float32 vb1 = vx_ڀريو(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_مٿاهون(va1), v_cvt_f64_مٿاهون(vb1), vs3);
    }
    vs0 = v_جوڙيو(v_جوڙيو(vs0, vs1), v_جوڙيو(vs2, vs3));
    لكل( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_ڀريو(a + k);
        v_float32 vb = vx_ڀريو(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_مٿاهون(va), v_cvt_f64_مٿاهون(vb), vs0);
    }
    أرجع v_reduce_جوڙ(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
ساكن مضمن فراغ simdGEMM_kj_f32f64(
    ثابت عائم* a_معلومات, ثابت عائم* b_معلومات_2, size_t b_قدم,
    ثابت عائم* c_معلومات, size_t c_step1,
    عائم* d_معلومات,
    صحيح n, صحيح m,
    مزدوج alpha, مزدوج beta,
    صحيح& j)
{
    ثابت صحيح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحيح vlanes_f64 = VTraits<v_float64>::vlanes();

    إذا( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ثابت صحيح max_m = 1600 / (صحيح)حجم(عائم);
        cv::Auto_بفر<مزدوج> _s_buf(max_m);
        مزدوج* s_buf = _s_buf.معلومات();

        {
            v_float64 vz = vx_setzero_f64();
            لكل( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            لكل( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ثابت عائم* b_ptr = b_معلومات_2;
            لكل( صحيح k = 0; k < n; k++, b_ptr += b_قدم )
            {
                v_float64 va = vx_setall_f64((مزدوج)a_معلومات[k]);
                j = 0;
                لكل( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_ڀريو(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_مٿاهون(vb);
                    v_float64 vd_lo = vx_ڀريو(s_buf + j);
                    v_float64 vd_hi = vx_ڀريو(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                مزدوج a_val = (مزدوج)a_معلومات[k];
                لكل( ; j < m; j++ )
                    s_buf[j] += a_val * (مزدوج)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ثابت عائم* c_معلومات_2 = c_معلومات;
            j = 0;
            إذا( !c_معلومات )
            {
                لكل( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_ڀريو(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_ڀريو(s_buf + j + vlanes_f64), valpha);
                    v_store(d_معلومات + j, v_cvt_f32(vs_lo, vs_hi));
                }
                لكل( ; j < m; j++ )
                    d_معلومات[j] = (عائم)(s_buf[j] * alpha);
            }
            وإلا إذا( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                لكل( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_ڀريو(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_ڀريو(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_ڀريو(c_معلومات_2 + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_مٿاهون(vc), vbeta, vs_hi);
                    v_store(d_معلومات + j, v_cvt_f32(vs_lo, vs_hi));
                }
                لكل( ; j < m; j++ )
                    d_معلومات[j] = (عائم)(s_buf[j] * alpha + (مزدوج)c_معلومات_2[j] * beta);
            }
            وإلا
            {
                لكل( j = 0; j < m; j++, c_معلومات_2 += c_step1 )
                    d_معلومات[j] = (عائم)(s_buf[j] * alpha + (مزدوج)c_معلومات_2[0] * beta);
            }
        }
        j = m;
    }
    وإلا إذا( vlanes_f32 * (صحيح)حجم(عائم) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ثابت عائم* c_معلومات_2 = c_معلومات;
        لكل( ; j <= m - vlanes_f32; j += vlanes_f32, c_معلومات_2 += vlanes_f32 * c_step1 )
        {
            ثابت عائم* b = b_معلومات_2 + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            لكل( صحيح k = 0; k < n; k++, b += b_قدم )
            {
                v_float64 va = vx_setall_f64((مزدوج)a_معلومات[k]);
                v_float32 vb = vx_ڀريو(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_مٿاهون(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            إذا( !c_معلومات )
            {
                v_store(d_معلومات + j, v_cvt_f32(vs_lo, vs_hi));
            }
            وإلا إذا( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_ڀريو(c_معلومات_2);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_مٿاهون(vc), vbeta, vs_hi);
                v_store(d_معلومات + j, v_cvt_f32(vs_lo, vs_hi));
            }
            وإلا
            {
                CV_DECL_ALIGNED(CV_SIMD_ويڪر) عائم buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                لكل( صحيح jj = 0; jj < vlanes_f32; jj++ )
                    d_معلومات[j + jj] = buf[jj] + c_معلومات_2[jj * c_step1] * (عائم)beta;
            }
        }
    }
}
#جيڪڏهن_پورو

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
سانچو<قسم_نالو ST>
ساكن مضمن فراغ simdGEMM_kj(
    ثابت ST* a_معلومات, ثابت ST* b_معلومات_2, size_t b_قدم,
    ثابت ST* c_معلومات, size_t c_step1,
    ST* d_معلومات,
    صحيح n, صحيح m,
    مزدوج alpha, مزدوج beta,
    صحيح& j)
{
    تعريفنوع قسم_نالو VecTraits<ST>::VT VecT;
    ثابت صحيح vlanes = VTraits<VecT>::vlanes();

    إذا( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ثابت صحيح max_m = 1600 / (صحيح)حجم(ST);
        CV_DECL_ALIGNED(CV_SIMD_ويڪر) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            لكل( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            لكل( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ثابت ST* b_ptr = b_معلومات_2;
            لكل( صحيح k = 0; k < n; k++, b_ptr += b_قدم )
            {
                VecT va = VecTraits<ST>::setall(a_معلومات[k]);
                j = 0;
                لكل( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_ڀريو_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_معلومات[k];
                لكل( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ثابت ST* c_معلومات_2 = c_معلومات;
            j = 0;
            إذا( !c_معلومات )
            {
                لكل( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_ڀريو_aligned(s_buf + j), valpha);
                    v_store(d_معلومات + j, vs);
                }
                لكل( ; j < m; j++ )
                    d_معلومات[j] = s_buf[j] * (ST)alpha;
            }
            وإلا إذا( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                لكل( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_ڀريو_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_معلومات_2 + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_معلومات + j, vs);
                }
                لكل( ; j < m; j++ )
                    d_معلومات[j] = s_buf[j] * (ST)alpha + c_معلومات_2[j] * (ST)beta;
            }
            وإلا
            {
                لكل( j = 0; j < m; j++, c_معلومات_2 += c_step1 )
                    d_معلومات[j] = s_buf[j] * (ST)alpha + c_معلومات_2[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    وإلا إذا( vlanes * (صحيح)حجم(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ثابت ST* c_معلومات_2 = c_معلومات;
        لكل( ; j <= m - vlanes; j += vlanes, c_معلومات_2 += vlanes * c_step1 )
        {
            ثابت ST* b = b_معلومات_2 + j;
            VecT vs = VecTraits<ST>::zero();
            لكل( صحيح k = 0; k < n; k++, b += b_قدم )
            {
                VecT va = VecTraits<ST>::setall(a_معلومات[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            إذا( !c_معلومات )
            {
                v_store(d_معلومات + j, vs);
            }
            وإلا إذا( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_معلومات_2);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_معلومات + j, vs);
            }
            وإلا
            {
                CV_DECL_ALIGNED(CV_SIMD_ويڪر) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                لكل( صحيح jj = 0; jj < vlanes; jj++ )
                    d_معلومات[j + jj] = buf[jj] + c_معلومات_2[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#جيڪڏهن_پورو


سانچو<قسم_نالو T, قسم_نالو WT> ساكن فراغ
GEMMSingleMul( ثابت T* a_معلومات, size_t a_قدم,
               ثابت T* b_معلومات, size_t b_قدم,
               ثابت T* c_معلومات_2, size_t c_قدم,
               T* d_معلومات, size_t d_قدم,
               سائيز a_سائيز, سائيز d_سائيز,
               مزدوج alpha, مزدوج beta, صحيح flags )
{
    صحيح i, j, k, n = a_سائيز.ويڪر, m = d_سائيز.ويڪر, drows = d_سائيز.اوچائي;
    ثابت T *a_معلومات_2 = a_معلومات, *b_معلومات_2 = b_معلومات, *c_معلومات = c_معلومات_2;
    cv::Auto_بفر<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_قدم;

    a_قدم /= حجم(a_معلومات[0]);
    b_قدم /= حجم(b_معلومات[0]);
    c_قدم /= حجم(c_معلومات_2[0]);
    d_قدم /= حجم(d_معلومات[0]);
    a_step0 = a_قدم;
    a_step1 = 1;

    إذا( !c_معلومات_2 )
        c_step0 = c_step1 = 0;
    وإلا إذا( !(flags & GEMM_3_T) )
        c_step0 = c_قدم, c_step1 = 1;
    وإلا
        c_step0 = 1, c_step1 = c_قدم;

    إذا( flags & GEMM_1_T )
    {
        CV_مٽا_سٽا( a_step0, a_step1, t_قدم );
        n = a_سائيز.اوچائي;
        إذا( a_قدم > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.معلومات();
        }
    }

    إذا( n == 1 ) /* external product */
    {
        cv::Auto_بفر<T> _b_buf;
        T* b_buf = 0;

        إذا( a_قدم > 1 && a_سائيز.اوچائي > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.معلومات();
            لكل( k = 0; k < drows; k++ )
                a_buf[k] = a_معلومات[a_قدم*k];
            a_معلومات = a_buf;
        }

        إذا( b_قدم > 1 )
        {
            _b_buf.allocate(d_سائيز.ويڪر);
            b_buf = _b_buf.معلومات();
            لكل( j = 0; j < d_سائيز.ويڪر; j++ )
                b_buf[j] = b_معلومات[j*b_قدم];
            b_معلومات = b_buf;
        }

        لكل( i = 0; i < drows; i++, c_معلومات += c_step0, d_معلومات += d_قدم )
        {
            WT al = WT(a_معلومات[i])*alpha;
            c_معلومات_2 = c_معلومات;
            لكل( j = 0; j <= d_سائيز.ويڪر - 2; j += 2, c_معلومات_2 += 2*c_step1 )
            {
                WT s0 = al*WT(b_معلومات[j]);
                WT s1 = al*WT(b_معلومات[j+1]);
                إذا( !c_معلومات_2 )
                {
                    d_معلومات[j] = T(s0);
                    d_معلومات[j+1] = T(s1);
                }
                وإلا
                {
                    d_معلومات[j] = T(s0 + WT(c_معلومات_2[0])*beta);
                    d_معلومات[j+1] = T(s1 + WT(c_معلومات_2[c_step1])*beta);
                }
            }

            لكل( ; j < d_سائيز.ويڪر; j++, c_معلومات_2 += c_step1 )
            {
                WT s0 = al*WT(b_معلومات[j]);
                إذا( !c_معلومات_2 )
                    d_معلومات[j] = T(s0);
                وإلا
                    d_معلومات[j] = T(s0 + WT(c_معلومات_2[0])*beta);
            }
        }
    }
    وإلا إذا( flags & GEMM_2_T ) /* A * Bt */
    {
        لكل( i = 0; i < drows; i++, a_معلومات_2 += a_step0, c_معلومات += c_step0, d_معلومات += d_قدم )
        {
            a_معلومات = a_معلومات_2;
            b_معلومات = b_معلومات_2;
            c_معلومات_2 = c_معلومات;

            إذا( a_buf )
            {
                لكل( k = 0; k < n; k++ )
                    a_buf[k] = a_معلومات[a_step1*k];
                a_معلومات = a_buf;
            }

            لكل( j = 0; j < d_سائيز.ويڪر; j++, b_معلومات += b_قدم,
                                               c_معلومات_2 += c_step1 )
            {
                WT s0(0);
                k = 0;
#إذا CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                إذا( حجم(WT) == حجم(مزدوج) )
                {
                    إذا( حجم(T) == حجم(مزدوج) )
                        s0 = (WT)simd_Dot_حاصل_ضرب((ثابت مزدوج*)a_معلومات, (ثابت مزدوج*)b_معلومات, n, k);
                    وإلا إذا( حجم(T) == حجم(عائم) )
                        s0 = (WT)simd_Dot_حاصل_ضرب_f32f64((ثابت عائم*)a_معلومات, (ثابت عائم*)b_معلومات, n, k);
                }
                وإلا
#جيڪڏهن_پورو
                {
                    WT s1(0), s2(0), s3(0);
                     #إذا CV_ENABLE_UNROLLED
                    لكل( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_معلومات[k])*WT(b_معلومات[k]);
                        s1 += WT(a_معلومات[k+1])*WT(b_معلومات[k+1]);
                        s2 += WT(a_معلومات[k+2])*WT(b_معلومات[k+2]);
                        s3 += WT(a_معلومات[k+3])*WT(b_معلومات[k+3]);
                    }
                    #جيڪڏهن_پورو
                    s0 += s1 + s2 + s3;
                }
                لكل( ; k < n; k++ )
                    s0 += WT(a_معلومات[k])*WT(b_معلومات[k]);
                s0 = s0*alpha;

                إذا( !c_معلومات_2 )
                    d_معلومات[j] = T(s0);
                وإلا
                    d_معلومات[j] = T(s0 + WT(c_معلومات_2[0])*beta);
            }
        }
    }
    وإلا إذا( d_سائيز.ويڪر*حجم(d_معلومات[0]) <= 1600 )
    {
        لكل( i = 0; i < drows; i++, a_معلومات_2 += a_step0,
                                    c_معلومات += c_step0,
                                    d_معلومات += d_قدم )
        {
            a_معلومات = a_معلومات_2, c_معلومات_2 = c_معلومات;

            إذا( a_buf )
            {
                لكل( k = 0; k < n; k++ )
                    a_buf[k] = a_معلومات[a_step1*k];
                a_معلومات = a_buf;
            }

            j = 0;
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            إذا( حجم(WT) == حجم(مزدوج) )
            {
                إذا( حجم(T) == حجم(مزدوج) )
                    simdGEMM_kj((ثابت مزدوج*)a_معلومات, (ثابت مزدوج*)b_معلومات_2, b_قدم,
                                        (ثابت مزدوج*)c_معلومات, c_step1,
                                (مزدوج*)d_معلومات, n, m, alpha, beta, j);
                وإلا إذا( حجم(T) == حجم(عائم) )
                    simdGEMM_kj_f32f64((ثابت عائم*)a_معلومات, (ثابت عائم*)b_معلومات_2, b_قدم,
                                        (ثابت عائم*)c_معلومات, c_step1,
                                (عائم*)d_معلومات, n, m, alpha, beta, j);
            }
#جيڪڏهن_پورو
            c_معلومات_2 = c_معلومات + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            لكل( ; j <= m - 4; j += 4, c_معلومات_2 += 4*c_step1 )
            {
                ثابت T* b = b_معلومات_2 + j;
                WT s0(0), s1(0), s2(0), s3(0);

                لكل( k = 0; k < n; k++, b += b_قدم )
                {
                    WT a(a_معلومات[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                إذا( !c_معلومات_2 )
                {
                    d_معلومات[j] = T(s0*alpha);
                    d_معلومات[j+1] = T(s1*alpha);
                    d_معلومات[j+2] = T(s2*alpha);
                    d_معلومات[j+3] = T(s3*alpha);
                }
                وإلا
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_معلومات[j] = T(s0 + WT(c_معلومات_2[0])*beta);
                    d_معلومات[j+1] = T(s1 + WT(c_معلومات_2[c_step1])*beta);
                    d_معلومات[j+2] = T(s2 + WT(c_معلومات_2[c_step1*2])*beta);
                    d_معلومات[j+3] = T(s3 + WT(c_معلومات_2[c_step1*3])*beta);
                }
            }

            لكل( ; j < m; j++, c_معلومات_2 += c_step1 )
            {
                ثابت T* b = b_معلومات_2 + j;
                WT s0(0);

                لكل( k = 0; k < n; k++, b += b_قدم )
                    s0 += WT(a_معلومات[k]) * WT(b[0]);

                s0 = s0*alpha;
                إذا( !c_معلومات_2 )
                    d_معلومات[j] = T(s0);
                وإلا
                    d_معلومات[j] = T(s0 + WT(c_معلومات_2[0])*beta);
            }
        }
    }
    وإلا
    {
        cv::Auto_بفر<WT> _d_buf(m);
        WT* d_buf = _d_buf.معلومات();

        لكل( i = 0; i < drows; i++, a_معلومات_2 += a_step0, c_معلومات += c_step0, d_معلومات += d_قدم )
        {
            a_معلومات = a_معلومات_2;
            b_معلومات = b_معلومات_2;
            c_معلومات_2 = c_معلومات;

            إذا( a_buf )
            {
                لكل( k = 0; k < n; k++ )
                    a_buf[k] = a_معلومات_2[a_step1*k];
                a_معلومات = a_buf;
            }

            لكل( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            لكل( k = 0; k < n; k++, b_معلومات += b_قدم )
            {
                WT al(a_معلومات[k]);
                j=0;
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                إذا( حجم(WT) == حجم(مزدوج) )
                {
                    إذا( حجم(T) == حجم(مزدوج) )
                    {
                        ثابت صحيح vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ثابت مزدوج*)&al));
                        لكل( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_ڀريو((ثابت مزدوج*)(d_buf + j));
                            v_float64 vb = vx_ڀريو((ثابت مزدوج*)(b_معلومات + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((مزدوج*)(d_buf + j), vd);
                        }
                    }
                    وإلا
                    {
                        ثابت صحيح vlanes_f32 = VTraits<v_float32>::vlanes();
                        ثابت صحيح vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ثابت مزدوج*)&al));
                        لكل( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_ڀريو((ثابت عائم*)(b_معلومات + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_مٿاهون(vb);
                            v_float64 vd_lo = vx_ڀريو((ثابت مزدوج*)(d_buf + j));
                            v_float64 vd_hi = vx_ڀريو((ثابت مزدوج*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((مزدوج*)(d_buf + j), vd_lo);
                            v_store((مزدوج*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#جيڪڏهن_پورو
                لكل( ; j < m; j++ )
                    d_buf[j] += WT(b_معلومات[j])*al;
            }

            إذا( !c_معلومات_2 )
                لكل( j = 0; j < m; j++ )
                    d_معلومات[j] = T(d_buf[j]*alpha);
            وإلا
                لكل( j = 0; j < m; j++, c_معلومات_2 += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_معلومات[j] = T(t + WT(c_معلومات_2[0])*beta);
                }
        }
    }
}
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
سانچو<قسم_نالو WT>
ساكن مضمن فراغ سيٽ_Double_معلومات_Zero(WT* d_معلومات, صحيح m, صحيح vlanes)
{
    v_float64 vz = vx_setzero_f64();
    صحيح j = 0;
    لكل( ; j <= m - vlanes; j += vlanes )
        v_store((مزدوج*)(d_معلومات + j), vz);
    لكل( ; j < m; j++ )
        d_معلومات[j] = WT(0);
}

سانچو<قسم_نالو ST>
ساكن مضمن فراغ simd_Mul_جوڙيو(ثابت ST* b, مزدوج* d, صحيح m, مزدوج a_val);

سانچو<>
مضمن فراغ simd_Mul_جوڙيو<عائم>(
    ثابت عائم* b, مزدوج* d, صحيح m, مزدوج a_val)
{
    ثابت صحيح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحيح vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    صحيح j = 0;
    لكل( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_ڀريو(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_مٿاهون(vb);
        v_float64 vd_lo = vx_ڀريو(d + j);
        v_float64 vd_hi = vx_ڀريو(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    لكل( ; j < m; j++ )
        d[j] += a_val * (مزدوج)b[j];
}

سانچو<>
مضمن فراغ simd_Mul_جوڙيو<مزدوج>(
    ثابت مزدوج* b, مزدوج* d, صحيح m, مزدوج a_val)
{
    ثابت صحيح vlanes = VTraits<v_float64>::vlanes();
    ثابت صحيح stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    صحيح j = 0;
    لكل( ; j <= m - stride2; j += stride2 )
    {
        v_float64 vd0 = vx_ڀريو(d + j);
        v_float64 vb0 = vx_ڀريو(b + j);
        v_float64 vd1 = vx_ڀريو(d + j + vlanes);
        v_float64 vb1 = vx_ڀريو(b + j + vlanes);
        vd0 = v_muladd(va, vb0, vd0);
        vd1 = v_muladd(va, vb1, vd1);
        v_store(d + j, vd0);
        v_store(d + j + vlanes, vd1);
    }
    لكل( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_ڀريو(d + j);
        v_float64 vb = vx_ڀريو(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    لكل( ; j < m; j++ )
        d[j] += a_val * b[j];
}

سانچو<قسم_نالو ST>
ساكن مضمن فراغ simd_بلاڪ_Mul_kj(
    ثابت ST* a_معلومات, ثابت ST* b_معلومات, size_t b_قدم,
    مزدوج* d_معلومات, صحيح n, صحيح m, منطقي do_acc)
{
    ثابت صحيح vlanes = VTraits<v_float64>::vlanes();
    إذا( !do_acc )
        سيٽ_Double_معلومات_Zero(d_معلومات, m, vlanes);
    لكل( صحيح k = 0; k < n; k++, b_معلومات += b_قدم )
        simd_Mul_جوڙيو(b_معلومات, d_معلومات, m, (مزدوج)a_معلومات[k]);
}
#جيڪڏهن_پورو // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

سانچو<قسم_نالو T, قسم_نالو WT> ساكن فراغ
GEMMBlockMul( ثابت T* a_معلومات, size_t a_قدم,
              ثابت T* b_معلومات, size_t b_قدم,
              WT* d_معلومات, size_t d_قدم,
              سائيز a_سائيز, سائيز d_سائيز, صحيح flags )
{
    صحيح i, j, k, n = a_سائيز.ويڪر, m = d_سائيز.ويڪر;
    ثابت T *a_معلومات_2 = a_معلومات, *b_معلومات_2 = b_معلومات;
    cv::Auto_بفر<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_قدم;
    صحيح do_acc = flags & 16;

    a_قدم /= حجم(a_معلومات[0]);
    b_قدم /= حجم(b_معلومات[0]);
    d_قدم /= حجم(d_معلومات[0]);

    a_step0 = a_قدم;
    a_step1 = 1;

    إذا( flags & GEMM_1_T )
    {
        CV_مٽا_سٽا( a_step0, a_step1, t_قدم );
        n = a_سائيز.اوچائي;
        _a_buf.allocate(n);
        a_buf = _a_buf.معلومات();
    }

    إذا( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        لكل( i = 0; i < d_سائيز.اوچائي; i++, a_معلومات_2 += a_step0, d_معلومات += d_قدم )
        {
            a_معلومات = a_معلومات_2; b_معلومات = b_معلومات_2;

            إذا( a_buf )
            {
                لكل( k = 0; k < n; k++ )
                    a_buf[k] = a_معلومات[a_step1*k];
                a_معلومات = a_buf;
            }

            لكل( j = 0; j < d_سائيز.ويڪر; j++, b_معلومات += b_قدم )
            {
                WT s0 = do_acc ? d_معلومات[j] : WT(0);
                k = 0;
#إذا CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                إذا( حجم(WT) == حجم(مزدوج) )
                {
                    إذا( حجم(T) == حجم(مزدوج) )
                        s0 += (WT)simd_Dot_حاصل_ضرب((ثابت مزدوج*)a_معلومات, (ثابت مزدوج*)b_معلومات, n, k);
                    وإلا إذا( حجم(T) == حجم(عائم) )
                        s0 += (WT)simd_Dot_حاصل_ضرب_f32f64((ثابت عائم*)a_معلومات, (ثابت عائم*)b_معلومات, n, k);
                }
                وإلا
#جيڪڏهن_پورو
                {
                    WT s1(0);
                    لكل( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_معلومات[k])*WT(b_معلومات[k]);
                        s1 += WT(a_معلومات[k+1])*WT(b_معلومات[k+1]);
                    }
                    s0 += s1;
                }
                لكل( ; k < n; k++ )
                    s0 += WT(a_معلومات[k])*WT(b_معلومات[k]);

                d_معلومات[j] = s0;
            }
        }
    }
    وإلا
    {
        لكل( i = 0; i < d_سائيز.اوچائي; i++, a_معلومات_2 += a_step0, d_معلومات += d_قدم )
        {
            a_معلومات = a_معلومات_2, b_معلومات = b_معلومات_2;

            إذا( a_buf )
            {
                لكل( k = 0; k < n; k++ )
                    a_buf[k] = a_معلومات[a_step1*k];
                a_معلومات = a_buf;
            }
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            إذا( حجم(WT) == حجم(مزدوج) )
            {
                إذا( حجم(T) == حجم(مزدوج) )
                    simd_بلاڪ_Mul_kj(
                        (ثابت مزدوج*)a_معلومات, (ثابت مزدوج*)b_معلومات, b_قدم,
                        (مزدوج*)d_معلومات, n, m, do_acc != 0);
                وإلا
                    simd_بلاڪ_Mul_kj(
                        (ثابت عائم*)a_معلومات, (ثابت عائم*)b_معلومات, b_قدم,
                        (مزدوج*)d_معلومات, n, m, do_acc != 0);
            }
            وإلا
#جيڪڏهن_پورو
            {
                لكل( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ثابت T* b = b_معلومات + j;

                    إذا( do_acc )
                    {
                        s0 = d_معلومات[j]; s1 = d_معلومات[j+1];
                        s2 = d_معلومات[j+2]; s3 = d_معلومات[j+3];
                    }
                    وإلا
                        s0 = s1 = s2 = s3 = WT(0);

                    لكل( k = 0; k < n; k++, b += b_قدم )
                    {
                        WT a(a_معلومات[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_معلومات[j] = s0; d_معلومات[j+1] = s1;
                    d_معلومات[j+2] = s2; d_معلومات[j+3] = s3;
                }

                لكل( ; j < m; j++ )
                {
                    ثابت T* b = b_معلومات + j;
                    WT s0 = do_acc ? d_معلومات[j] : WT(0);

                    لكل( k = 0; k < n; k++, b += b_قدم )
                        s0 += WT(a_معلومات[k]) * WT(b[0]);

                    d_معلومات[j] = s0;
                }
            }
        }
    }
}


سانچو<قسم_نالو T, قسم_نالو WT> ساكن فراغ
GEMMStore( ثابت T* c_معلومات_2, size_t c_قدم,
           ثابت WT* d_buf, size_t d_buf_قدم,
           T* d_معلومات, size_t d_قدم, سائيز d_سائيز,
           مزدوج alpha, مزدوج beta, صحيح flags )
{
    ثابت T* c_معلومات = c_معلومات_2;
    صحيح j;
    size_t c_step0, c_step1;

    c_قدم /= حجم(c_معلومات_2[0]);
    d_buf_قدم /= حجم(d_buf[0]);
    d_قدم /= حجم(d_معلومات[0]);

    إذا( !c_معلومات_2 )
        c_step0 = c_step1 = 0;
    وإلا إذا( !(flags & GEMM_3_T) )
        c_step0 = c_قدم, c_step1 = 1;
    وإلا
        c_step0 = 1, c_step1 = c_قدم;

    لكل( ; d_سائيز.اوچائي--; c_معلومات += c_step0, d_buf += d_buf_قدم, d_معلومات += d_قدم )
    {
        إذا( c_معلومات )
        {
            c_معلومات_2 = c_معلومات;
            j=0;
             #إذا CV_ENABLE_UNROLLED
            لكل(; j <= d_سائيز.ويڪر - 4; j += 4, c_معلومات_2 += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_معلومات_2[0]);
                t1 += beta*WT(c_معلومات_2[c_step1]);
                d_معلومات[j] = T(t0);
                d_معلومات[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_معلومات_2[c_step1*2]);
                t1 += beta*WT(c_معلومات_2[c_step1*3]);
                d_معلومات[j+2] = T(t0);
                d_معلومات[j+3] = T(t1);
            }
            #جيڪڏهن_پورو
            لكل( ; j < d_سائيز.ويڪر; j++, c_معلومات_2 += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_معلومات[j] = T(t0 + WT(c_معلومات_2[0])*beta);
            }
        }
        وإلا
        {
            j = 0;
             #إذا CV_ENABLE_UNROLLED
            لكل( ; j <= d_سائيز.ويڪر - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_معلومات[j] = T(t0);
                d_معلومات[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_معلومات[j+2] = T(t0);
                d_معلومات[j+3] = T(t1);
            }
            #جيڪڏهن_پورو
            لكل( ; j < d_سائيز.ويڪر; j++ )
                d_معلومات[j] = T(alpha*d_buf[j]);
        }
    }
}


تعريفنوع فراغ (*GEMMSingleMulFunc)( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, ثابت فراغ* src3, size_t step3,
                   فراغ* dst, size_t dststep, سائيز srcsize, سائيز dstsize,
                   مزدوج alpha, مزدوج beta, صحيح flags );

تعريفنوع فراغ (*GEMMBlockMulFunc)( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, فراغ* dst, size_t dststep,
                   سائيز srcsize, سائيز dstsize, صحيح flags );

تعريفنوع فراغ (*GEMMStoreFunc)( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, فراغ* dst, size_t dststep,
                   سائيز dstsize, مزدوج alpha, مزدوج beta, صحيح flags );

سانچو<قسم_نالو T, قسم_نالو WT, فراغ (*fn)(ثابت T*, size_t, ثابت T*, size_t, ثابت T*, size_t, T*, size_t, سائيز, سائيز, مزدوج, مزدوج, صحيح)>
ساكن فراغ gemmSingleMulWrap( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, ثابت فراغ* src3, size_t step3,
                   فراغ* dst, size_t dststep, سائيز srcsize, سائيز dstsize,
                   مزدوج alpha, مزدوج beta, صحيح flags )
{
    fn((ثابت T*)src1, step1, (ثابت T*)src2, step2, (ثابت T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

سانچو<قسم_نالو T, قسم_نالو WT, فراغ (*fn)(ثابت T*, size_t, ثابت T*, size_t, WT*, size_t, سائيز, سائيز, صحيح)>
ساكن فراغ gemm_بلاڪ_Mul_Wrap( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, فراغ* dst, size_t dststep,
                   سائيز srcsize, سائيز dstsize, صحيح flags )
{
    fn((ثابت T*)src1, step1, (ثابت T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

سانچو<قسم_نالو T, قسم_نالو WT, فراغ (*fn)(ثابت T*, size_t, ثابت WT*, size_t, T*, size_t, سائيز, مزدوج, مزدوج, صحيح)>
ساكن فراغ gemmStoreWrap( ثابت فراغ* src1, size_t step1,
                   ثابت فراغ* src2, size_t step2, فراغ* dst, size_t dststep,
                   سائيز dstsize, مزدوج alpha, مزدوج beta, صحيح flags )
{
    fn((ثابت T*)src1, step1, (ثابت WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

ساكن فراغ GEMMSingleMul_32f( ثابت عائم* a_معلومات, size_t a_قدم,
              ثابت عائم* b_معلومات, size_t b_قدم,
              ثابت عائم* c_معلومات_2, size_t c_قدم,
              عائم* d_معلومات, size_t d_قدم,
              سائيز a_سائيز, سائيز d_سائيز,
              مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMSingleMul<عائم,مزدوج>(a_معلومات, a_قدم, b_معلومات, b_قدم, c_معلومات_2,
                                c_قدم, d_معلومات, d_قدم, a_سائيز, d_سائيز,
                                alpha, beta, flags);
}

ساكن فراغ GEMMSingleMul_64f( ثابت مزدوج* a_معلومات, size_t a_قدم,
                              ثابت مزدوج* b_معلومات, size_t b_قدم,
                              ثابت مزدوج* c_معلومات_2, size_t c_قدم,
                              مزدوج* d_معلومات, size_t d_قدم,
                              سائيز a_سائيز, سائيز d_سائيز,
                              مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMSingleMul<مزدوج,مزدوج>(a_معلومات, a_قدم, b_معلومات, b_قدم, c_معلومات_2,
                                c_قدم, d_معلومات, d_قدم, a_سائيز, d_سائيز,
                                alpha, beta, flags);
}


ساكن فراغ GEMMSingleMul_32fc( ثابت Complexf* a_معلومات, size_t a_قدم,
                              ثابت Complexf* b_معلومات, size_t b_قدم,
                              ثابت Complexf* c_معلومات_2, size_t c_قدم,
                              Complexf* d_معلومات, size_t d_قدم,
                              سائيز a_سائيز, سائيز d_سائيز,
                              مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_معلومات, a_قدم, b_معلومات, b_قدم, c_معلومات_2,
                                c_قدم, d_معلومات, d_قدم, a_سائيز, d_سائيز,
                                alpha, beta, flags);
}

ساكن فراغ GEMMSingleMul_64fc( ثابت Complexd* a_معلومات, size_t a_قدم,
                              ثابت Complexd* b_معلومات, size_t b_قدم,
                              ثابت Complexd* c_معلومات_2, size_t c_قدم,
                              Complexd* d_معلومات, size_t d_قدم,
                              سائيز a_سائيز, سائيز d_سائيز,
                              مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_معلومات, a_قدم, b_معلومات, b_قدم, c_معلومات_2,
                                 c_قدم, d_معلومات, d_قدم, a_سائيز, d_سائيز,
                                 alpha, beta, flags);
}

ساكن فراغ GEMMBlockMul_32f( ثابت عائم* a_معلومات, size_t a_قدم,
             ثابت عائم* b_معلومات, size_t b_قدم,
             مزدوج* d_معلومات, size_t d_قدم,
             سائيز a_سائيز, سائيز d_سائيز, صحيح flags )
{
    GEMMBlockMul(a_معلومات, a_قدم, b_معلومات, b_قدم, d_معلومات, d_قدم, a_سائيز, d_سائيز, flags);
}


ساكن فراغ GEMMBlockMul_64f( ثابت مزدوج* a_معلومات, size_t a_قدم,
                             ثابت مزدوج* b_معلومات, size_t b_قدم,
                             مزدوج* d_معلومات, size_t d_قدم,
                             سائيز a_سائيز, سائيز d_سائيز, صحيح flags )
{
    GEMMBlockMul(a_معلومات, a_قدم, b_معلومات, b_قدم, d_معلومات, d_قدم, a_سائيز, d_سائيز, flags);
}


ساكن فراغ GEMMBlockMul_32fc( ثابت Complexf* a_معلومات, size_t a_قدم,
                             ثابت Complexf* b_معلومات, size_t b_قدم,
                             Complexd* d_معلومات, size_t d_قدم,
                             سائيز a_سائيز, سائيز d_سائيز, صحيح flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_معلومات, a_قدم, b_معلومات, b_قدم, d_معلومات, d_قدم, a_سائيز, d_سائيز, flags);
}


ساكن فراغ GEMMBlockMul_64fc( ثابت Complexd* a_معلومات, size_t a_قدم,
                             ثابت Complexd* b_معلومات, size_t b_قدم,
                             Complexd* d_معلومات, size_t d_قدم,
                             سائيز a_سائيز, سائيز d_سائيز, صحيح flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_معلومات, a_قدم, b_معلومات, b_قدم, d_معلومات, d_قدم, a_سائيز, d_سائيز, flags);
}


ساكن فراغ GEMMStore_32f( ثابت عائم* c_معلومات_2, size_t c_قدم,
          ثابت مزدوج* d_buf, size_t d_buf_قدم,
          عائم* d_معلومات, size_t d_قدم, سائيز d_سائيز,
          مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMStore(c_معلومات_2, c_قدم, d_buf, d_buf_قدم, d_معلومات, d_قدم, d_سائيز, alpha, beta, flags);
}


ساكن فراغ GEMMStore_64f( ثابت مزدوج* c_معلومات_2, size_t c_قدم,
                      ثابت مزدوج* d_buf, size_t d_buf_قدم,
                      مزدوج* d_معلومات, size_t d_قدم, سائيز d_سائيز,
                      مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMStore(c_معلومات_2, c_قدم, d_buf, d_buf_قدم, d_معلومات, d_قدم, d_سائيز, alpha, beta, flags);
}


ساكن فراغ GEMMStore_32fc( ثابت Complexf* c_معلومات_2, size_t c_قدم,
                          ثابت Complexd* d_buf, size_t d_buf_قدم,
                          Complexf* d_معلومات, size_t d_قدم, سائيز d_سائيز,
                          مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMStore(c_معلومات_2, c_قدم, d_buf, d_buf_قدم, d_معلومات, d_قدم, d_سائيز, alpha, beta, flags);
}


ساكن فراغ GEMMStore_64fc( ثابت Complexd* c_معلومات_2, size_t c_قدم,
                          ثابت Complexd* d_buf, size_t d_buf_قدم,
                          Complexd* d_معلومات, size_t d_قدم, سائيز d_سائيز,
                          مزدوج alpha, مزدوج beta, صحيح flags )
{
    GEMMStore(c_معلومات_2, c_قدم, d_buf, d_buf_قدم, d_معلومات, d_قدم, d_سائيز, alpha, beta, flags);
}

ساكن فراغ gemmImpl( Mat A, Mat B, مزدوج alpha,
           Mat C, مزدوج beta, Mat D, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح بلاڪ_lin_سائيز = 128;
    ثابت صحيح بلاڪ_سائيز_3 = بلاڪ_lin_سائيز * بلاڪ_lin_سائيز;

    ساكن مزدوج zero[] = {0,0,0,0};
    ساكن عائم zerof[] = {0,0,0,0};

    سائيز a_سائيز = A.size(), d_سائيز;
    صحيح i, len = 0, type = A.type();

    بدل( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالة 0:
        d_سائيز = سائيز( B.کانن_ڳڻپ, a_سائيز.اوچائي );
        len = B.صفون;
        اكسر;
    حالة 1:
        d_سائيز = سائيز( B.کانن_ڳڻپ, a_سائيز.ويڪر );
        len = B.صفون;
        اكسر;
    حالة 2:
        d_سائيز = سائيز( B.صفون, a_سائيز.اوچائي );
        len = B.کانن_ڳڻپ;
        اكسر;
    حالة 3:
        d_سائيز = سائيز( B.صفون, a_سائيز.ويڪر );
        len = B.کانن_ڳڻپ;
        اكسر;
    }

    إذا( flags == 0 && 2 <= len && len <= 4 && (len == d_سائيز.ويڪر || len == d_سائيز.اوچائي) )
    {
        إذا( type == CV_32F )
        {
            عائم* d = D.ptr<عائم>();
            ثابت عائم *a = A.ptr<عائم>(),
                        *b = B.ptr<عائم>(),
                        *c = (ثابت عائم*)C.معلومات;
            size_t d_قدم = D.قدم/حجم(d[0]),
                a_قدم = A.قدم/حجم(a[0]),
                b_قدم = B.قدم/حجم(b[0]),
                c_قدم = C.معلومات ? C.قدم/حجم(c[0]) : 0;

            إذا( !c )
                c = zerof;

            بدل( len )
            {
            حالة 2:
                إذا( len == d_سائيز.ويڪر && b != d )
                {
                    لكل( i = 0; i < d_سائيز.اوچائي; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_قدم];
                        عائم t1 = a[0]*b[1] + a[1]*b[b_قدم+1];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[1] = (عائم)(t1*alpha + c[1]*beta);
                    }
                }
                وإلا إذا( a != d )
                {
                    صحيح c_step0 = 1;
                    إذا( c == zerof )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    لكل( i = 0; i < d_سائيز.ويڪر; i++, d++, b++, c += c_step0 )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_قدم];
                        عائم t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[d_قدم] = (عائم)(t1*alpha + c[c_قدم]*beta);
                    }
                }
                وإلا
                    اكسر;
                أرجع;
            حالة 3:
                إذا( len == d_سائيز.ويڪر && b != d )
                {
                    لكل( i = 0; i < d_سائيز.اوچائي; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2];
                        عائم t1 = a[0]*b[1] + a[1]*b[b_قدم+1] + a[2]*b[b_قدم*2+1];
                        عائم t2 = a[0]*b[2] + a[1]*b[b_قدم+2] + a[2]*b[b_قدم*2+2];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[1] = (عائم)(t1*alpha + c[1]*beta);
                        d[2] = (عائم)(t2*alpha + c[2]*beta);
                    }
                }
                وإلا إذا( a != d )
                {
                    صحيح c_step0 = 1;
                    إذا( c == zerof )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    لكل( i = 0; i < d_سائيز.ويڪر; i++, d++, b++, c += c_step0 )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2];
                        عائم t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم] + a[a_قدم+2]*b[b_قدم*2];
                        عائم t2 = a[a_قدم*2]*b[0] + a[a_قدم*2+1]*b[b_قدم] + a[a_قدم*2+2]*b[b_قدم*2];

                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[d_قدم] = (عائم)(t1*alpha + c[c_قدم]*beta);
                        d[d_قدم*2] = (عائم)(t2*alpha + c[c_قدم*2]*beta);
                    }
                }
                وإلا
                    اكسر;
                أرجع;
            حالة 4:
                إذا( len == d_سائيز.ويڪر && b != d )
                {
                    لكل( i = 0; i < d_سائيز.اوچائي; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2] + a[3]*b[b_قدم*3];
                        عائم t1 = a[0]*b[1] + a[1]*b[b_قدم+1] + a[2]*b[b_قدم*2+1] + a[3]*b[b_قدم*3+1];
                        عائم t2 = a[0]*b[2] + a[1]*b[b_قدم+2] + a[2]*b[b_قدم*2+2] + a[3]*b[b_قدم*3+2];
                        عائم t3 = a[0]*b[3] + a[1]*b[b_قدم+3] + a[2]*b[b_قدم*2+3] + a[3]*b[b_قدم*3+3];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[1] = (عائم)(t1*alpha + c[1]*beta);
                        d[2] = (عائم)(t2*alpha + c[2]*beta);
                        d[3] = (عائم)(t3*alpha + c[3]*beta);
                    }
                }
                وإلا إذا( len <= 16 && a != d )
                {
                    صحيح c_step0 = 1;
                    إذا( c == zerof )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    لكل( i = 0; i < d_سائيز.ويڪر; i++, d++, b++, c += c_step0 )
                    {
                        عائم t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2] + a[3]*b[b_قدم*3];
                        عائم t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم] +
                                   a[a_قدم+2]*b[b_قدم*2] + a[a_قدم+3]*b[b_قدم*3];
                        عائم t2 = a[a_قدم*2]*b[0] + a[a_قدم*2+1]*b[b_قدم] +
                                   a[a_قدم*2+2]*b[b_قدم*2] + a[a_قدم*2+3]*b[b_قدم*3];
                        عائم t3 = a[a_قدم*3]*b[0] + a[a_قدم*3+1]*b[b_قدم] +
                                   a[a_قدم*3+2]*b[b_قدم*2] + a[a_قدم*3+3]*b[b_قدم*3];
                        d[0] = (عائم)(t0*alpha + c[0]*beta);
                        d[d_قدم] = (عائم)(t1*alpha + c[c_قدم]*beta);
                        d[d_قدم*2] = (عائم)(t2*alpha + c[c_قدم*2]*beta);
                        d[d_قدم*3] = (عائم)(t3*alpha + c[c_قدم*3]*beta);
                    }
                }
                وإلا
                    اكسر;
                أرجع;
            }
        }

        إذا( type == CV_64F )
        {
            مزدوج* d = D.ptr<مزدوج>();
            ثابت مزدوج *a = A.ptr<مزدوج>(),
                         *b = B.ptr<مزدوج>(),
                         *c = (ثابت مزدوج*)C.معلومات;
            size_t d_قدم = D.قدم/حجم(d[0]),
                a_قدم = A.قدم/حجم(a[0]),
                b_قدم = B.قدم/حجم(b[0]),
                c_قدم = C.معلومات ? C.قدم/حجم(c[0]) : 0;
            إذا( !c )
                c = zero;

            بدل( len )
            {
            حالة 2:
                إذا( len == d_سائيز.ويڪر && b != d )
                {
                    لكل( i = 0; i < d_سائيز.اوچائي; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_قدم];
                        مزدوج t1 = a[0]*b[1] + a[1]*b[b_قدم+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                وإلا إذا( a != d )
                {
                    صحيح c_step0 = 1;
                    إذا( c == zero )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    لكل( i = 0; i < d_سائيز.ويڪر; i++, d++, b++, c += c_step0 )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_قدم];
                        مزدوج t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_قدم] = t1*alpha + c[c_قدم]*beta;
                    }
                }
                وإلا
                    اكسر;
                أرجع;
            حالة 3:
                إذا( len == d_سائيز.ويڪر && b != d )
                {
                    لكل( i = 0; i < d_سائيز.اوچائي; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2];
                        مزدوج t1 = a[0]*b[1] + a[1]*b[b_قدم+1] + a[2]*b[b_قدم*2+1];
                        مزدوج t2 = a[0]*b[2] + a[1]*b[b_قدم+2] + a[2]*b[b_قدم*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                وإلا إذا( a != d )
                {
                    صحيح c_step0 = 1;
                    إذا( c == zero )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    لكل( i = 0; i < d_سائيز.ويڪر; i++, d++, b++, c += c_step0 )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2];
                        مزدوج t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم] + a[a_قدم+2]*b[b_قدم*2];
                        مزدوج t2 = a[a_قدم*2]*b[0] + a[a_قدم*2+1]*b[b_قدم] + a[a_قدم*2+2]*b[b_قدم*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_قدم] = t1*alpha + c[c_قدم]*beta;
                        d[d_قدم*2] = t2*alpha + c[c_قدم*2]*beta;
                    }
                }
                وإلا
                    اكسر;
                أرجع;
            حالة 4:
                إذا( len == d_سائيز.ويڪر && b != d )
                {
                    لكل( i = 0; i < d_سائيز.اوچائي; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2] + a[3]*b[b_قدم*3];
                        مزدوج t1 = a[0]*b[1] + a[1]*b[b_قدم+1] + a[2]*b[b_قدم*2+1] + a[3]*b[b_قدم*3+1];
                        مزدوج t2 = a[0]*b[2] + a[1]*b[b_قدم+2] + a[2]*b[b_قدم*2+2] + a[3]*b[b_قدم*3+2];
                        مزدوج t3 = a[0]*b[3] + a[1]*b[b_قدم+3] + a[2]*b[b_قدم*2+3] + a[3]*b[b_قدم*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                وإلا إذا( d_سائيز.ويڪر <= 16 && a != d )
                {
                    صحيح c_step0 = 1;
                    إذا( c == zero )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    لكل( i = 0; i < d_سائيز.ويڪر; i++, d++, b++, c += c_step0 )
                    {
                        مزدوج t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2] + a[3]*b[b_قدم*3];
                        مزدوج t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم] +
                                    a[a_قدم+2]*b[b_قدم*2] + a[a_قدم+3]*b[b_قدم*3];
                        مزدوج t2 = a[a_قدم*2]*b[0] + a[a_قدم*2+1]*b[b_قدم] +
                                    a[a_قدم*2+2]*b[b_قدم*2] + a[a_قدم*2+3]*b[b_قدم*3];
                        مزدوج t3 = a[a_قدم*3]*b[0] + a[a_قدم*3+1]*b[b_قدم] +
                                    a[a_قدم*3+2]*b[b_قدم*2] + a[a_قدم*3+3]*b[b_قدم*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_قدم] = t1*alpha + c[c_قدم]*beta;
                        d[d_قدم*2] = t2*alpha + c[c_قدم*2]*beta;
                        d[d_قدم*3] = t3*alpha + c[c_قدم*3]*beta;
                    }
                }
                وإلا
                    اكسر;
                أرجع;
            }
        }
    }

    {
    size_t b_قدم = B.قدم;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc بلاڪ_Mul_Func;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ثابت uchar* Cdata = C.معلومات;
    size_t Cstep = C.معلومات ? (size_t)C.قدم : 0;
    Auto_بفر<uchar> buf;

    إذا( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<عائم, مزدوج, GEMMSingleMul_32f>;
        بلاڪ_Mul_Func = gemm_بلاڪ_Mul_Wrap<عائم, مزدوج, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<عائم, مزدوج, GEMMStore_32f>;
    }
    وإلا إذا( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<مزدوج, مزدوج, GEMMSingleMul_64f>;
        بلاڪ_Mul_Func = gemm_بلاڪ_Mul_Wrap<مزدوج, مزدوج, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<مزدوج, مزدوج, GEMMStore_64f>;
    }
    وإلا إذا( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        بلاڪ_Mul_Func = gemm_بلاڪ_Mul_Wrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    وإلا
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        بلاڪ_Mul_Func = gemm_بلاڪ_Mul_Wrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    إذا( (d_سائيز.ويڪر == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_قدم = d_سائيز.ويڪر == 1 ? 0 : CV_ELEM_سائيز(type);
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
    else*/ إذا( ((d_سائيز.اوچائي <= بلاڪ_lin_سائيز/2 || d_سائيز.ويڪر <= بلاڪ_lin_سائيز/2) &&
        len <= 10000) || len <= 10 ||
        (d_سائيز.ويڪر <= بلاڪ_lin_سائيز &&
        d_سائيز.اوچائي <= بلاڪ_lin_سائيز && len <= بلاڪ_lin_سائيز) )
    {
        singleMulFunc( A.ptr(), A.قدم, B.ptr(), b_قدم, Cdata, Cstep,
                       matD->ptr(), matD->قدم, a_سائيز, d_سائيز, alpha, beta, flags );
    }
    وإلا
    {
        صحيح is_a_t = flags & GEMM_1_T;
        صحيح is_b_t = flags & GEMM_2_T;
        صحيح elem_سائيز_2 = CV_ELEM_سائيز(type);
        صحيح dk0_1, dk0_2;
        size_t a_buf_سائيز = 0, b_buf_سائيز, d_buf_سائيز;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        صحيح j, k, di = 0, dj = 0, dk = 0;
        صحيح dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        صحيح work_elem_سائيز = elem_سائيز_2 << (CV_MAT_اونهائي(type) == CV_32F ? 1 : 0);

        إذا( !is_a_t )
            a_step0 = A.قدم, a_step1 = elem_سائيز_2;
        وإلا
            a_step0 = elem_سائيز_2, a_step1 = A.قدم;

        إذا( !is_b_t )
            b_step0 = b_قدم, b_step1 = elem_سائيز_2;
        وإلا
            b_step0 = elem_سائيز_2, b_step1 = b_قدم;

        إذا( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        وإلا إذا( !(flags & GEMM_3_T) )
            c_step0 = C.قدم, c_step1 = elem_سائيز_2;
        وإلا
            c_step0 = elem_سائيز_2, c_step1 = C.قدم;

        dm0 = std::min( بلاڪ_lin_سائيز, d_سائيز.اوچائي );
        dn0 = std::min( بلاڪ_lin_سائيز, d_سائيز.ويڪر );
        dk0_1 = بلاڪ_سائيز_3 / dm0;
        dk0_2 = بلاڪ_سائيز_3 / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        إذا( dk0*dm0 > بلاڪ_سائيز_3 )
            dm0 = بلاڪ_سائيز_3 / dk0;
        إذا( dk0*dn0 > بلاڪ_سائيز_3 )
            dn0 = بلاڪ_سائيز_3 / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_سائيز = (size_t)(dk0+dk0/8+1)*dk0_1*elem_سائيز_2;
        d_buf_سائيز = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_سائيز;

        إذا( is_a_t )
        {
            a_buf_سائيز = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_سائيز_2;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_سائيز + b_buf_سائيز + a_buf_سائيز);
        d_buf = buf.معلومات();
        b_buf = d_buf + d_buf_سائيز;

        إذا( is_a_t )
            a_buf = b_buf + b_buf_سائيز;

        لكل( i = 0; i < d_سائيز.اوچائي; i += di )
        {
            di = dm0;
            إذا( i + di >= d_سائيز.اوچائي || 8*(i + di) + di > 8*d_سائيز.اوچائي )
                di = d_سائيز.اوچائي - i;

            لكل( j = 0; j < d_سائيز.ويڪر; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->قدم + j*elem_سائيز_2;
                ثابت uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t d_قدم_2 = matD->قدم;
                dj = dn0;

                إذا( j + dj >= d_سائيز.ويڪر || 8*(j + dj) + dj > 8*d_سائيز.ويڪر )
                    dj = d_سائيز.ويڪر - j;

                flags &= 15;
                إذا( dk0 < len )
                {
                    _d = d_buf;
                    d_قدم_2 = dj*work_elem_سائيز;
                }

                لكل( k = 0; k < len; k += dk )
                {
                    ثابت uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t a_قدم_2 = A.قدم;
                    ثابت uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t b_قدم_2 = b_قدم;
                    سائيز a_bl_سائيز;

                    dk = dk0;
                    إذا( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    إذا( !is_a_t )
                        a_bl_سائيز.ويڪر = dk, a_bl_سائيز.اوچائي = di;
                    وإلا
                        a_bl_سائيز.ويڪر = di, a_bl_سائيز.اوچائي = dk;

                    إذا( a_buf && is_a_t )
                    {
                        a_قدم_2 = dk*elem_سائيز_2;
                        GEMM_Transpose_بلاڪ( _a, A.قدم, a_buf, a_قدم_2, a_bl_سائيز, elem_سائيز_2 );
                        std::swap( a_bl_سائيز.ويڪر, a_bl_سائيز.اوچائي );
                        _a = a_buf;
                    }

                    إذا( dj < d_سائيز.ويڪر )
                    {
                        سائيز b_سائيز;
                        إذا( !is_b_t )
                            b_سائيز.ويڪر = dj, b_سائيز.اوچائي = dk;
                        وإلا
                            b_سائيز.ويڪر = dk, b_سائيز.اوچائي = dj;

                        b_قدم_2 = b_سائيز.ويڪر*elem_سائيز_2;
                        GEMM_Copy_بلاڪ( _b, b_قدم, b_buf, b_قدم_2, b_سائيز, elem_سائيز_2 );
                        _b = b_buf;
                    }

                    إذا( dk0 < len )
                        بلاڪ_Mul_Func( _a, a_قدم_2, _b, b_قدم_2, _d, d_قدم_2,
                                      a_bl_سائيز, سائيز(dj,di), flags );
                    وإلا
                        singleMulFunc( _a, a_قدم_2, _b, b_قدم_2, _c, Cstep,
                                       _d, d_قدم_2, a_bl_سائيز, سائيز(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                إذا( dk0 < len )
                    storeFunc( _c, Cstep, _d, d_قدم_2,
                               matD->ptr(i) + j*elem_سائيز_2,
                               matD->قدم, سائيز(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

سانچو <قسم_نالو fptype>مضمن ساكن فراغ
callGemmImpl(ثابت fptype *src1, size_t src1_قدم, ثابت fptype *src2, size_t src2_قدم, fptype alpha,
          ثابت fptype *src3, size_t src3_قدم, fptype beta, fptype *dst, size_t dst_قدم, صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags, صحيح type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    صحيح b_m, b_n, c_m, c_n, m_d;

    إذا(flags & GEMM_2_T)
    {
        b_m = n_d;
        إذا(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        وإلا
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    وإلا
    {
        b_n = n_d;
        إذا(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        وإلا
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    إذا(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    وإلا
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    إذا(src1 != عدم)
        A = Mat(m_a, n_a, type, (فراغ*)src1, src1_قدم);
    إذا(src2 != عدم)
        B = Mat(b_m, b_n, type, (فراغ*)src2, src2_قدم);
    إذا(src3 != عدم && beta != 0.0)
        C = Mat(c_m, c_n, type, (فراغ*)src3, src3_قدم);
    Mat D(m_d, n_d, type, (فراغ*)dst, dst_قدم);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

فراغ gemm32f(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
             عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags, CV_32F);
}

فراغ gemm64f(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags, CV_64F);
}

فراغ gemm32fc(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
              عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags, CV_32FC2);
}

فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags, CV_64FC2);
}

#جيڪڏهن_پورو // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

سانچو<قسم_نالو T, قسم_نالو WT> ساكن فراغ
transform_( ثابت T* src, T* dst, ثابت WT* m, صحيح len, صحيح scn, صحيح dcn )
{
    صحيح x;

    إذا( scn == 2 && dcn == 2 )
    {
        لكل( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    وإلا إذا( scn == 3 && dcn == 3 )
    {
        لكل( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    وإلا إذا( scn == 3 && dcn == 1 )
    {
        لكل( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    وإلا إذا( scn == 4 && dcn == 4 )
    {
        لكل( x = 0; x < len*4; x += 4 )
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
    وإلا
    {
        لكل( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ثابت WT* _m = m;
            صحيح j, k;
            لكل( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                لكل( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

ساكن فراغ
transform_8u( ثابت uchar* src, uchar* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn )
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح بٽ = 10, SCALE = 1 << بٽ;
    ثابت عائم MAX_M = (عائم)(1 << (15 - بٽ));

    إذا( scn == 3 && dcn == 3 &&
        std::مطلق_قدر(m[0]) < MAX_M && std::مطلق_قدر(m[1]) < MAX_M && std::مطلق_قدر(m[ 2]) < MAX_M*256 && std::مطلق_قدر(m[ 3]) < MAX_M*256 &&
        std::مطلق_قدر(m[4]) < MAX_M && std::مطلق_قدر(m[5]) < MAX_M && std::مطلق_قدر(m[ 6]) < MAX_M*256 && std::مطلق_قدر(m[ 7]) < MAX_M*256 &&
        std::مطلق_قدر(m[8]) < MAX_M && std::مطلق_قدر(m[9]) < MAX_M && std::مطلق_قدر(m[10]) < MAX_M*256 && std::مطلق_قدر(m[11]) < MAX_M*256 )
    {
        ثابت صحيح nChannels = 3;

        اتحاد {
            ننڍو s[6];
            صحيح p[3];
        } m16;
        m16.s[0] = saturate_cast<ننڍو>(m[0] * SCALE); m16.s[1] = saturate_cast<ننڍو>(m[1] * SCALE);
        m16.s[2] = saturate_cast<ننڍو>(m[4] * SCALE); m16.s[3] = saturate_cast<ننڍو>(m[5] * SCALE);
        m16.s[4] = saturate_cast<ننڍو>(m[8] * SCALE); m16.s[5] = saturate_cast<ننڍو>(m[9] * SCALE);
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
        لكل (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
        {
            v_uint8 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
            v_uint8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_uint16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_uint16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack_u(dbl, dbh), v_pack_u(dgl, dgh), v_pack_u(drl, drh));
        }
        m32[1] = saturate_cast<صحيح>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<صحيح>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<صحيح>((m[11] + 0.5f)*SCALE);
        لكل( ; x < len * nChannels; x += nChannels )
        {
            صحيح v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> بٽ);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> بٽ);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> بٽ);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_16u( ثابت ushort* src, ushort* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn )
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا( scn == 3 && dcn == 3 )
    {
        صحيح x = 0;
#إذا CV_SIMD_ويڪر > 16
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
        لكل (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
        {
            v_uint16 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
            v_uint32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_جوڙيو_wrap(v_pack(v_ڦيرو(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m2,  m3)))),
                                   v_ڦيرو(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m2,  m3))))), delta);
            dg = v_جوڙيو_wrap(v_pack(v_ڦيرو(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m6,  m7)))),
                                   v_ڦيرو(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m6,  m7))))), delta);
            dr = v_جوڙيو_wrap(v_pack(v_ڦيرو(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)), m10, m11)))),
                                   v_ڦيرو(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)), m10, m11))))), delta);
            v_store_interleave(dst + x, v_reinterpret_as_u16(db), v_reinterpret_as_u16(dg), v_reinterpret_as_u16(dr));
        }
#جيڪڏهن_پورو
#إذا CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_کاٻو<1>(_m0l);
        v_float32x4 _m1h = v_rotate_کاٻو<1>(_m1l);
        v_float32x4 _m2h = v_rotate_کاٻو<1>(_m2l);
        v_float32x4 _m3h = v_rotate_کاٻو<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        لكل( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_ساڄو<1>(v_reinterpret_as_u16(v_جوڙيو_wrap(v_pack(
                             v_ڦيرو(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_ڀريو_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_ڦيرو(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_ڀريو_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#جيڪڏهن_پورو //CV_SIMD128
        لكل( ; x < len * 3; x += 3 )
        {
            عائم v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_32f( ثابت عائم* src, عائم* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#إذا (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    صحيح x = 0;
    إذا( scn == 3 && dcn == 3 )
    {
        صحيح idx[VTraits<v_float32>::max_nlanes/2];
        لكل( صحيح i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
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
        لكل( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        لكل( ; x < len*3; x += 3 )
        {
            عائم v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            عائم t0 = saturate_cast<عائم>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            عائم t1 = saturate_cast<عائم>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            عائم t2 = saturate_cast<عائم>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        أرجع;
    }

    إذا( scn == 4 && dcn == 4 )
    {
#إذا CV_SIMD_ويڪر > 16
        صحيح idx[VTraits<v_float32>::max_nlanes/4];
        لكل( صحيح i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        عائم _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        لكل( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_ڀريو(src + x);
            v_store(dst + x, v_جوڙيو(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#جيڪڏهن_پورو
#إذا CV_SIMD128
        v_float32x4 _m0 = v_ڀريو(m     );
        v_float32x4 _m1 = v_ڀريو(m +  5);
        v_float32x4 _m2 = v_ڀريو(m + 10);
        v_float32x4 _m3 = v_ڀريو(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        لكل( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_ڀريو(src + x);
            v_store(dst + x, v_جوڙيو(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#وإلا // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        لكل( ; x < len*4; x += 4 )
        {
            عائم v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            عائم t0 = saturate_cast<عائم>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            عائم t1 = saturate_cast<عائم>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            عائم t2 = saturate_cast<عائم>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            عائم t3 = saturate_cast<عائم>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#جيڪڏهن_پورو
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    transform_(src, dst, m, len, scn, dcn);
}


ساكن فراغ
transform_8s(ثابت schar* src, schar* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح بٽ = 10, SCALE = 1 << بٽ;
    ثابت عائم MAX_M = (عائم)(1 << (15 - بٽ));

    إذا( scn == 3 && dcn == 3 &&
        std::مطلق_قدر(m[0]) < MAX_M && std::مطلق_قدر(m[1]) < MAX_M && std::مطلق_قدر(m[ 2]) < MAX_M*256 && std::مطلق_قدر(m[ 3]) < MAX_M*256 &&
        std::مطلق_قدر(m[4]) < MAX_M && std::مطلق_قدر(m[5]) < MAX_M && std::مطلق_قدر(m[ 6]) < MAX_M*256 && std::مطلق_قدر(m[ 7]) < MAX_M*256 &&
        std::مطلق_قدر(m[8]) < MAX_M && std::مطلق_قدر(m[9]) < MAX_M && std::مطلق_قدر(m[10]) < MAX_M*256 && std::مطلق_قدر(m[11]) < MAX_M*256 )
    {
        ثابت صحيح nChannels = 3;

        اتحاد {
            ننڍو s[6];
            صحيح p[3];
        } m16;
        m16.s[0] = saturate_cast<ننڍو>(m[0] * SCALE); m16.s[1] = saturate_cast<ننڍو>(m[1] * SCALE);
        m16.s[2] = saturate_cast<ننڍو>(m[4] * SCALE); m16.s[3] = saturate_cast<ننڍو>(m[5] * SCALE);
        m16.s[4] = saturate_cast<ننڍو>(m[8] * SCALE); m16.s[5] = saturate_cast<ننڍو>(m[9] * SCALE);
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
        لكل (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
        {
            v_int8 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
            v_int8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_int16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_int16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<بٽ>(v_جوڙيو(v_جوڙيو(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_جوڙيو(v_جوڙيو(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack(dbl, dbh), v_pack(dgl, dgh), v_pack(drl, drh));
        }
        m32[1] = saturate_cast<صحيح>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<صحيح>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<صحيح>((m[11] + 0.5f)*SCALE);
        لكل( ; x < len * nChannels; x += nChannels )
        {
            صحيح v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> بٽ);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> بٽ);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> بٽ);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_16s(ثابت ننڍو* src, ننڍو* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (scn == 3 && dcn == 3) {
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
        لكل (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
        {
            v_int16 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
            v_int32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_pack(v_ڦيرو(v_muladd(v_cvt_f32(bl), m0, v_muladd(v_cvt_f32(gl), m1, v_muladd(v_cvt_f32(rl),  m2,  m3)))),
                        v_ڦيرو(v_muladd(v_cvt_f32(bh), m0, v_muladd(v_cvt_f32(gh), m1, v_muladd(v_cvt_f32(rh),  m2,  m3)))));
            dg = v_pack(v_ڦيرو(v_muladd(v_cvt_f32(bl), m4, v_muladd(v_cvt_f32(gl), m5, v_muladd(v_cvt_f32(rl),  m6,  m7)))),
                        v_ڦيرو(v_muladd(v_cvt_f32(bh), m4, v_muladd(v_cvt_f32(gh), m5, v_muladd(v_cvt_f32(rh),  m6,  m7)))));
            dr = v_pack(v_ڦيرو(v_muladd(v_cvt_f32(bl), m8, v_muladd(v_cvt_f32(gl), m9, v_muladd(v_cvt_f32(rl), m10, m11)))),
                        v_ڦيرو(v_muladd(v_cvt_f32(bh), m8, v_muladd(v_cvt_f32(gh), m9, v_muladd(v_cvt_f32(rh), m10, m11)))));
            v_store_interleave(dst + x, db, dg, dr);
        }
        لكل( ; x < len * 3; x += 3 )
        {
            عائم v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ننڍو t0 = saturate_cast<ننڍو>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ننڍو t1 = saturate_cast<ننڍو>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ننڍو t2 = saturate_cast<ننڍو>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_32s(ثابت صحيح* src, صحيح* dst, ثابت مزدوج* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    إذا( scn == 3 && dcn == 3) {
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
        لكل (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_مٿاهون(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_مٿاهون(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_مٿاهون(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_ڦيرو(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_ڦيرو(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_ڦيرو(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        لكل (; x < len * 3; x += 3) {
            مزدوج b = src[x], g = src[x + 1], r = src[x + 2];
            صحيح db = saturate_cast<صحيح>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            صحيح dg = saturate_cast<صحيح>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            صحيح dr = saturate_cast<صحيح>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    transform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
transform_64f(ثابت مزدوج* src, مزدوج* dst, ثابت مزدوج* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    إذا( scn == 3 && dcn == 3) {
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
        لكل (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        لكل (; x < len * 3; x += 3) {
            مزدوج b = src[x], g = src[x + 1], r = src[x + 2];
            مزدوج db = saturate_cast<مزدوج>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            مزدوج dg = saturate_cast<مزدوج>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            مزدوج dr = saturate_cast<مزدوج>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    transform_(src, dst, m, len, scn, dcn);
}

سانچو<قسم_نالو T, قسم_نالو WT> ساكن فراغ
diagtransform_( ثابت T* src, T* dst, ثابت WT* m, صحيح len, صحيح cn, صحيح )
{
    صحيح x;

    إذا( cn == 2 )
    {
        لكل( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    وإلا إذا( cn == 3 )
    {
        لكل( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    وإلا إذا( cn == 4 )
    {
        لكل( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    وإلا
    {
        لكل( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ثابت WT* _m = m;
            لكل( صحيح j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

ساكن فراغ
diagtransform_8u(ثابت uchar* src, uchar* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        لكل (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
            v_uint8 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack_u(v_ڦيرو(db_f32_l0), v_ڦيرو(db_f32_l1)), v_pack_u(v_ڦيرو(db_f32_h0), v_ڦيرو(db_f32_h1))),
                               v_pack(v_pack_u(v_ڦيرو(dg_f32_l0), v_ڦيرو(dg_f32_l1)), v_pack_u(v_ڦيرو(dg_f32_h0), v_ڦيرو(dg_f32_h1))),
                               v_pack(v_pack_u(v_ڦيرو(dr_f32_l0), v_ڦيرو(dr_f32_l1)), v_pack_u(v_ڦيرو(dr_f32_h0), v_ڦيرو(dr_f32_h1))));
        }
        لكل (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_8s(ثابت schar* src, schar* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
    #إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        لكل (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
            v_int8 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack(v_ڦيرو(db_f32_l0), v_ڦيرو(db_f32_l1)), v_pack(v_ڦيرو(db_f32_h0), v_ڦيرو(db_f32_h1))),
                               v_pack(v_pack(v_ڦيرو(dg_f32_l0), v_ڦيرو(dg_f32_l1)), v_pack(v_ڦيرو(dg_f32_h0), v_ڦيرو(dg_f32_h1))),
                               v_pack(v_pack(v_ڦيرو(dr_f32_l0), v_ڦيرو(dr_f32_l1)), v_pack(v_ڦيرو(dr_f32_h0), v_ڦيرو(dr_f32_h1))));
        }
        لكل (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_16u(ثابت ushort* src, ushort* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        لكل (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
            v_uint16 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
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
                               v_pack_u(v_ڦيرو(db_f32_l), v_ڦيرو(db_f32_h)),
                               v_pack_u(v_ڦيرو(dg_f32_l), v_ڦيرو(dg_f32_h)),
                               v_pack_u(v_ڦيرو(dr_f32_l), v_ڦيرو(dr_f32_h)));
        }
        لكل (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_16s(ثابت ننڍو* src, ننڍو* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        لكل (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
            v_int16 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
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
                               v_pack(v_ڦيرو(db_f32_l), v_ڦيرو(db_f32_h)),
                               v_pack(v_ڦيرو(dg_f32_l), v_ڦيرو(dg_f32_h)),
                               v_pack(v_ڦيرو(dr_f32_l), v_ڦيرو(dr_f32_h)));
        }
        لكل (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            ننڍو db = saturate_cast<ننڍو>(m[ 0] * b + m[ 3]);
            ننڍو dg = saturate_cast<ننڍو>(m[ 5] * g + m[ 7]);
            ننڍو dr = saturate_cast<ننڍو>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_32s(ثابت صحيح* src, صحيح* dst, ثابت مزدوج* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    إذا (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        لكل (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_مٿاهون(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_مٿاهون(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_مٿاهون(r), rl = v_cvt_f64(r);

            v_int32 db = v_ڦيرو(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_ڦيرو(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_ڦيرو(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        لكل (; x < len * 3; x += 3) {
            صحيح b = src[x], g = src[x + 1], r = src[x + 2];
            صحيح db = saturate_cast<صحيح>(m[ 0] * b + m[ 3]);
            صحيح dg = saturate_cast<صحيح>(m[ 5] * g + m[ 7]);
            صحيح dr = saturate_cast<صحيح>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_32f(ثابت عائم* src, عائم* dst, ثابت عائم* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        لكل (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        لكل (; x < len * 3; x += 3) {
            عائم b = src[x], g = src[x + 1], r = src[x + 2];
            عائم db = saturate_cast<عائم>(m[ 0] * b + m[ 3]);
            عائم dg = saturate_cast<عائم>(m[ 5] * g + m[ 7]);
            عائم dr = saturate_cast<عائم>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساكن فراغ
diagtransform_64f(ثابت مزدوج* src, مزدوج* dst, ثابت مزدوج* m, صحيح len, صحيح scn, صحيح dcn)
{
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    إذا (scn == 3 && dcn == 3) {
        صحيح x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        لكل (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_ڀريو_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        لكل (; x < len * 3; x += 3) {
            مزدوج b = src[x], g = src[x + 1], r = src[x + 2];
            مزدوج db = saturate_cast<مزدوج>(m[ 0] * b + m[ 3]);
            مزدوج dg = saturate_cast<مزدوج>(m[ 5] * g + m[ 7]);
            مزدوج dr = saturate_cast<مزدوج>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        أرجع;
    }
#جيڪڏهن_پورو

    diagtransform_(src, dst, m, len, scn, dcn);
}


سانچو<قسم_نالو T, قسم_نالو WT, فراغ (*fn)(ثابت T*, T*, ثابت WT*, صحيح, صحيح, صحيح)>
ساكن فراغ transformWrap(ثابت فراغ* src, فراغ* dst, ثابت فراغ* m, صحيح len, صحيح scn, صحيح dcn)
{
    fn((ثابت T*)src, (T*)dst, (ثابت WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(صحيح اونهائي)
{
    ساكن TransformFunc transformTab[CV_اونهائي_MAX] =
    {
        transformWrap<uchar, عائم, transform_8u>, transformWrap<schar, عائم, transform_8s>, transformWrap<ushort, عائم, transform_16u>,
        transformWrap<ننڍو, عائم, transform_16s>, transformWrap<صحيح, مزدوج, transform_32s>, transformWrap<عائم, عائم, transform_32f>,
        transformWrap<مزدوج, مزدوج, transform_64f>, 0
    };

    أرجع transformTab[اونهائي];
}

TransformFunc getDiagTransformFunc(صحيح اونهائي)
{
    ساكن TransformFunc diagTransformTab[CV_اونهائي_MAX] =
    {
        transformWrap<uchar, عائم, diagtransform_8u>, transformWrap<schar, عائم, diagtransform_8s>, transformWrap<ushort, عائم, diagtransform_16u>,
        transformWrap<ننڍو, عائم, diagtransform_16s>, transformWrap<صحيح, مزدوج, diagtransform_32s>, transformWrap<عائم, عائم, diagtransform_32f>,
        transformWrap<مزدوج, مزدوج, diagtransform_64f>, 0
    };

    أرجع diagTransformTab[اونهائي];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

سانچو<قسم_نالو T> ساكن فراغ
perspectiveTransform_( ثابت فراغ* _src, فراغ* _dst, ثابت فراغ* _m_ptr, صحيح len, صحيح scn, صحيح dcn )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    ثابت مزدوج* m = (ثابت مزدوج*)_m_ptr;
    ثابت مزدوج eps = FLT_EPSILON;
    صحيح i;

    إذا( scn == 2 && dcn == 2 )
    {
        لكل( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            مزدوج w = x*m[6] + y*m[7] + m[8];

            إذا( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            وإلا
                dst[i] = dst[i+1] = (T)0;
        }
    }
    وإلا إذا( scn == 3 && dcn == 3 )
    {
        لكل( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            مزدوج w = x*m[12] + y*m[13] + z*m[14] + m[15];

            إذا( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            وإلا
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    وإلا إذا( scn == 3 && dcn == 2 )
    {
        لكل( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            مزدوج w = x*m[8] + y*m[9] + z*m[10] + m[11];

            إذا( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            وإلا
                dst[0] = dst[1] = (T)0;
        }
    }
    وإلا
    {
        لكل( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ثابت مزدوج* _m = m + dcn*(scn + 1);
            مزدوج w = _m[scn];
            صحيح j, k;
            لكل( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            إذا( fabs(w) > eps )
            {
                _m = m;
                لكل( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    مزدوج s = _m[scn];
                    لكل( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            وإلا
                لكل( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(صحيح اونهائي)
{
    إذا (اونهائي == CV_32F)
        أرجع perspectiveTransform_<عائم>;
    إذا (اونهائي == CV_64F)
        أرجع perspectiveTransform_<مزدوج>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

ساكن فراغ scale_جوڙيو_32f(ثابت فراغ* _src1, ثابت فراغ* _src2, فراغ* _dst,
                         صحيح len, ثابت فراغ* _alpha)
{
    ثابت عائم* src1 = (ثابت عائم*)_src1;
    ثابت عائم* src2 = (ثابت عائم*)_src2;
    عائم* dst = (عائم*)_dst;
    عائم alpha = *(ثابت عائم*)_alpha;
    صحيح i = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ثابت صحيح c_ويڪر = VTraits<v_float32>::vlanes();
    لكل (; i <= len - c_ويڪر; i += c_ويڪر)
        v_store(dst + i, v_muladd(vx_ڀريو(src1 + i), v_alpha, vx_ڀريو(src2 + i)));
    vx_cleanup();
#جيڪڏهن_پورو
    لكل (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


ساكن فراغ scale_جوڙيو_64f(ثابت فراغ* _src1, ثابت فراغ* _src2, فراغ* _dst,
                         صحيح len, ثابت فراغ* _alpha)
{
    ثابت مزدوج* src1 = (ثابت مزدوج*)_src1;
    ثابت مزدوج* src2 = (ثابت مزدوج*)_src2;
    مزدوج* dst = (مزدوج*)_dst;
    مزدوج alpha = *(ثابت مزدوج*)_alpha;
    صحيح i = 0;
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ثابت صحيح c_ويڪر = VTraits<v_float64>::vlanes();
    لكل (; i <= len - c_ويڪر; i += c_ويڪر)
        v_store(dst + i, v_muladd(vx_ڀريو(src1 + i), a2, vx_ڀريو(src2 + i)));
    vx_cleanup();
#جيڪڏهن_پورو
    لكل (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

Scale_جوڙيو_Func get_Scale_جوڙيو_Func(صحيح اونهائي)
{
    إذا (اونهائي == CV_32F)
        أرجع scale_جوڙيو_32f;
    إذا (اونهائي == CV_64F)
        أرجع scale_جوڙيو_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

سانچو<قسم_نالو T> ساكن مضمن
مزدوج MahalanobisImpl(ثابت Mat& v1, ثابت Mat& v2, ثابت Mat& icovar, مزدوج *diff_بفر /*[len]*/, صحيح len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    سائيز sz = v1.size();
    مزدوج نتيجو = 0;

    sz.ويڪر *= v1.channels();
    إذا (v1.isContinuous() && v2.isContinuous())
    {
        sz.ويڪر *= sz.اوچائي;
        sz.اوچائي = 1;
    }

    {
        ثابت T* src1 = v1.ptr<T>();
        ثابت T* src2 = v2.ptr<T>();
        size_t step1 = v1.قدم/حجم(src1[0]);
        size_t step2 = v2.قدم/حجم(src2[0]);
        مزدوج* diff = diff_بفر;
        ثابت T* mat = icovar.ptr<T>();
        size_t matstep = icovar.قدم/حجم(mat[0]);

        لكل (; sz.اوچائي--; src1 += step1, src2 += step2, diff += sz.ويڪر)
        {
            لكل (صحيح i = 0; i < sz.ويڪر; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_بفر;
        لكل (صحيح i = 0; i < len; i++, mat += matstep)
        {
            مزدوج صف_جوڙ = 0;
            صحيح j = 0;
#إذا CV_ENABLE_UNROLLED
            لكل(; j <= len - 4; j += 4 )
                صف_جوڙ += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#جيڪڏهن_پورو
            لكل (; j < len; j++)
                صف_جوڙ += diff[j]*mat[j];
            نتيجو += صف_جوڙ * diff[i];
        }
    }
    أرجع نتيجو;
}

MahalanobisImplFunc getMahalanobisImplFunc(صحيح اونهائي)
{
    إذا (اونهائي == CV_32F)
        أرجع (MahalanobisImplFunc)MahalanobisImpl<عائم>;
    إذا (اونهائي == CV_64F)
        أرجع (MahalanobisImplFunc)MahalanobisImpl<مزدوج>;
    CV_Assert(0 && "Not supported");
}


#إذا !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_گهڻائي)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

سانچو<قسم_نالو sT, قسم_نالو dT> ساكن فراغ
MulTransposedR(ثابت Mat& srcmat, ثابت Mat& dstmat, ثابت Mat& deltamat, مزدوج scale)
{
    صحيح i, j, k;
    ثابت sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ثابت dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.قدم/حجم(src[0]);
    size_t dststep = dstmat.قدم/حجم(dst[0]);
    size_t deltastep = deltamat.صفون > 1 ? deltamat.قدم/حجم(delta[0]) : 0;
    صحيح delta_کانن_ڳڻپ = deltamat.کانن_ڳڻپ;
    سائيز size = srcmat.size();
    dT* tdst = dst;
    dT* کانو_buf = 0;
    dT* delta_buf = 0;
    صحيح buf_سائيز_2 = size.اوچائي*حجم(dT);
    Auto_بفر<uchar> buf;

    إذا( delta && delta_کانن_ڳڻپ < size.ويڪر )
    {
        CV_Assert( delta_کانن_ڳڻپ == 1 );
        buf_سائيز_2 *= 5;
    }
    buf.allocate(buf_سائيز_2);
    کانو_buf = (dT*)buf.معلومات();

    إذا( delta && delta_کانن_ڳڻپ < size.ويڪر )
    {
        delta_buf = کانو_buf + size.اوچائي;
        لكل( i = 0; i < size.اوچائي; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#إذا CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#جيڪڏهن_پورو

    إذا( !delta )
        لكل( i = 0; i < size.ويڪر; i++, tdst += dststep )
        {
            لكل( k = 0; k < size.اوچائي; k++ )
                کانو_buf[k] = src[k*srcstep+i];

            لكل( j = i; j <= size.ويڪر - 4; j += 4 )
            {
#إذا CV_SIMD128_64F
                إذا (معلومات_Type_2<sT>::اونهائي == CV_64F && معلومات_Type_2<dT>::اونهائي == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ثابت مزدوج *tsrc = (مزدوج*)(src + j);

                    لكل( k = 0; k < size.اوچائي; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((مزدوج)کانو_buf[k]);
                        s0 = v_جوڙيو(s0, v_mul(a, v_ڀريو(tsrc + 0)));
                        s1 = v_جوڙيو(s1, v_mul(a, v_ڀريو(tsrc + 2)));
                    }

                    v_store((مزدوج*)(tdst+j), v_mul(s0, v_scale));
                    v_store((مزدوج*)(tdst+j+2), v_mul(s1, v_scale));
                } وإلا
#جيڪڏهن_پورو
                {
                    مزدوج s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ثابت sT *tsrc = src + j;

                    لكل( k = 0; k < size.اوچائي; k++, tsrc += srcstep )
                    {
                        مزدوج a = کانو_buf[k];
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

            لكل( ; j < size.ويڪر; j++ )
            {
                مزدوج s0 = 0;
                ثابت sT *tsrc = src + j;

                لكل( k = 0; k < size.اوچائي; k++, tsrc += srcstep )
                    s0 += (مزدوج)کانو_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    وإلا
        لكل( i = 0; i < size.ويڪر; i++, tdst += dststep )
        {
            إذا( !delta_buf )
                لكل( k = 0; k < size.اوچائي; k++ )
                    کانو_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            وإلا
                لكل( k = 0; k < size.اوچائي; k++ )
                    کانو_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            لكل( j = i; j <= size.ويڪر - 4; j += 4 )
            {
#إذا CV_SIMD128_64F
                إذا (معلومات_Type_2<sT>::اونهائي == CV_64F && معلومات_Type_2<dT>::اونهائي == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ثابت مزدوج *tsrc = (مزدوج*)(src + j);
                    ثابت مزدوج *d = (مزدوج*)(delta_buf ? delta_buf : delta + j);

                    لكل( k = 0; k < size.اوچائي; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((مزدوج)کانو_buf[k]);
                        s0 = v_جوڙيو(s0, v_mul(a, v_sub(v_ڀريو(tsrc + 0), v_ڀريو(d + 0))));
                        s1 = v_جوڙيو(s1, v_mul(a, v_sub(v_ڀريو(tsrc + 2), v_ڀريو(d + 2))));
                    }

                    v_store((مزدوج*)(tdst+j), v_mul(s0, v_scale));
                    v_store((مزدوج*)(tdst+j+2), v_mul(s1, v_scale));
                }
                وإلا
#جيڪڏهن_پورو

                {
                    مزدوج s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ثابت sT *tsrc = src + j;
                    ثابت dT *d = delta_buf ? delta_buf : delta + j;

                    لكل( k = 0; k < size.اوچائي; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        مزدوج a = کانو_buf[k];
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

            لكل( ; j < size.ويڪر; j++ )
            {
                مزدوج s0 = 0;
                ثابت sT *tsrc = src + j;
                ثابت dT *d = delta_buf ? delta_buf : delta + j;

                لكل( k = 0; k < size.اوچائي; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (مزدوج)کانو_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


سانچو<قسم_نالو sT, قسم_نالو dT> ساكن فراغ
MulTransposedL(ثابت Mat& srcmat, ثابت Mat& dstmat, ثابت Mat& deltamat, مزدوج scale)
{
    صحيح i, j, k;
    ثابت sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ثابت dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.قدم/حجم(src[0]);
    size_t dststep = dstmat.قدم/حجم(dst[0]);
    size_t deltastep = deltamat.صفون > 1 ? deltamat.قدم/حجم(delta[0]) : 0;
    صحيح delta_کانن_ڳڻپ = deltamat.کانن_ڳڻپ;
    سائيز size = srcmat.size();
    dT* tdst = dst;

    إذا( !delta )
        لكل( i = 0; i < size.اوچائي; i++, tdst += dststep )
            لكل( j = i; j < size.اوچائي; j++ )
            {
                مزدوج s = 0;
                ثابت sT *tsrc1 = src + i*srcstep;
                ثابت sT *tsrc2 = src + j*srcstep;
#إذا CV_SIMD128_64F
                إذا (معلومات_Type_2<sT>::اونهائي == CV_64F && معلومات_Type_2<dT>::اونهائي == CV_64F)
                {
                    ثابت مزدوج *v_tsrc1 = (مزدوج *)(tsrc1);
                    ثابت مزدوج *v_tsrc2 = (مزدوج *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    لكل( k = 0; k <= size.ويڪر - 4; k += 4 )
                        v_s = v_جوڙيو(v_s, v_جوڙيو(v_mul(v_ڀريو(v_tsrc1 + k), v_ڀريو(v_tsrc2 + k)), v_mul(v_ڀريو(v_tsrc1 + k + 2), v_ڀريو(v_tsrc2 + k + 2))));
                    s += v_reduce_جوڙ(v_s);
                }
                وإلا
#جيڪڏهن_پورو
                {
                    لكل( k = 0; k <= size.ويڪر - 4; k += 4 )
                        s += (مزدوج)tsrc1[k]*tsrc2[k] + (مزدوج)tsrc1[k+1]*tsrc2[k+1] +
                             (مزدوج)tsrc1[k+2]*tsrc2[k+2] + (مزدوج)tsrc1[k+3]*tsrc2[k+3];
                }
                لكل( ; k < size.ويڪر; k++ )
                    s += (مزدوج)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    وإلا
    {
        dT delta_buf[4];
        صحيح delta_سرڪايو = delta_کانن_ڳڻپ == size.ويڪر ? 4 : 0;
        Auto_بفر<uchar> buf(size.ويڪر*حجم(dT));
        dT* صف_buf = (dT*)buf.معلومات();

        لكل( i = 0; i < size.اوچائي; i++, tdst += dststep )
        {
            ثابت sT *tsrc1 = src + i*srcstep;
            ثابت dT *tdelta1 = delta + i*deltastep;

            إذا( delta_کانن_ڳڻپ < size.ويڪر )
                لكل( k = 0; k < size.ويڪر; k++ )
                    صف_buf[k] = tsrc1[k] - tdelta1[0];
            وإلا
                لكل( k = 0; k < size.ويڪر; k++ )
                    صف_buf[k] = tsrc1[k] - tdelta1[k];

            لكل( j = i; j < size.اوچائي; j++ )
            {
                مزدوج s = 0;
                ثابت sT *tsrc2 = src + j*srcstep;
                ثابت dT *tdelta2 = delta + j*deltastep;
                إذا( delta_کانن_ڳڻپ < size.ويڪر )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#إذا CV_SIMD128_64F
                إذا (معلومات_Type_2<sT>::اونهائي == CV_64F && معلومات_Type_2<dT>::اونهائي == CV_64F)
                {
                    ثابت مزدوج *v_tsrc2 = (مزدوج *)(tsrc2);
                    ثابت مزدوج *v_tdelta2 = (مزدوج *)(tdelta2);
                    ثابت مزدوج *v_صف_buf = (مزدوج *)(صف_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    لكل( k = 0; k <= size.ويڪر - 4; k += 4, v_tdelta2 += delta_سرڪايو )
                        v_s = v_جوڙيو(v_s, v_جوڙيو(v_mul(v_sub(v_ڀريو(v_tsrc2 + k), v_ڀريو(v_tdelta2)), v_ڀريو(v_صف_buf + k)), v_mul(v_sub(v_ڀريو(v_tsrc2 + k + 2), v_ڀريو(v_tdelta2 + 2)), v_ڀريو(v_صف_buf + k + 2))));
                    s += v_reduce_جوڙ(v_s);

                    tdelta2 = (ثابت dT *)(v_tdelta2);
                }
                وإلا
#جيڪڏهن_پورو
                {
                    لكل( k = 0; k <= size.ويڪر-4; k += 4, tdelta2 += delta_سرڪايو )
                        s += (مزدوج)صف_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (مزدوج)صف_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (مزدوج)صف_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (مزدوج)صف_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                لكل( ; k < size.ويڪر; k++, tdelta2++ )
                    s += (مزدوج)صف_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata)
{
    MulTransposedFunc func = عدم;
    إذا (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,عائم>
                   : MulTransposedL<uchar,عائم>;
    }
    وإلا إذا (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,مزدوج>
                   : MulTransposedL<uchar,مزدوج>;
    }
    وإلا إذا(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,عائم>
                   : MulTransposedL<ushort,عائم>;
    }
    وإلا إذا(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,مزدوج>
                   : MulTransposedL<ushort,مزدوج>;
    }
    وإلا إذا(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ننڍو,عائم>
                   : MulTransposedL<ننڍو,عائم>;
    }
    وإلا إذا(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ننڍو,مزدوج>
                   : MulTransposedL<ننڍو,مزدوج>;
    }
    وإلا إذا(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<عائم,عائم>
                   : MulTransposedL<عائم,عائم>;
    }
    وإلا إذا(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<عائم,مزدوج>
                   : MulTransposedL<عائم,مزدوج>;
    }
    وإلا إذا(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<مزدوج,مزدوج>
                   : MulTransposedL<مزدوج,مزدوج>;
    }
    CV_Assert(func && "Not supported");
    أرجع func;
}
#جيڪڏهن_پورو // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

سانچو<قسم_نالو T> ساكن مضمن
مزدوج dotProd_(ثابت T* src1, ثابت T* src2, صحيح len)
{
    صحيح i = 0;
    مزدوج نتيجو = 0;

#إذا CV_ENABLE_UNROLLED
    لكل( ; i <= len - 4; i += 4 )
        نتيجو += (مزدوج)src1[i]*src2[i] + (مزدوج)src1[i+1]*src2[i+1] +
            (مزدوج)src1[i+2]*src2[i+2] + (مزدوج)src1[i+3]*src2[i+3];
#جيڪڏهن_پورو
    لكل( ; i < len; i++ )
        نتيجو += (مزدوج)src1[i]*src2[i];

    أرجع نتيجو;
}


مزدوج dotProd_8u(ثابت uchar* src1, ثابت uchar* src2, صحيح len)
{
    مزدوج r = 0;
    صحيح i = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_uint16>::vlanes(), بلاڪ_Size0 = (1 << 15), بلاڪ_سائيز_2;

    طالما (i < len0)
    {
        بلاڪ_سائيز_2 = std::min(len0 - i, بلاڪ_Size0);
        v_uint32 v_جوڙ = vx_setzero_u32();
        ثابت صحيح c_ويڪر = VTraits<v_uint16>::vlanes();

        صحيح j = 0;
        لكل (; j <= بلاڪ_سائيز_2 - c_ويڪر * 2; j += c_ويڪر * 2)
        {
            v_uint8 v_src1 = vx_ڀريو(src1 + j);
            v_uint8 v_src2 = vx_ڀريو(src2 + j);
            v_جوڙ = v_dotprod_expand_fast(v_src1, v_src2, v_جوڙ);
        }

        لكل (; j <= بلاڪ_سائيز_2 - c_ويڪر; j += c_ويڪر)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_ڀريو_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_ڀريو_expand(src2 + j));
            v_جوڙ = v_جوڙيو(v_جوڙ, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (مزدوج)v_reduce_جوڙ(v_جوڙ);

        src1 += بلاڪ_سائيز_2;
        src2 += بلاڪ_سائيز_2;
        i += بلاڪ_سائيز_2;
    }
    vx_cleanup();
#جيڪڏهن_پورو
    أرجع r + dotProd_(src1, src2, len - i);
}


مزدوج dotProd_8s(ثابت schar* src1, ثابت schar* src2, صحيح len)
{
    مزدوج r = 0.0;
    صحيح i = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_int16>::vlanes(), بلاڪ_Size0 = (1 << 14), بلاڪ_سائيز_2;

    طالما (i < len0)
    {
        بلاڪ_سائيز_2 = std::min(len0 - i, بلاڪ_Size0);
        v_int32 v_جوڙ = vx_setzero_s32();
        ثابت صحيح c_ويڪر = VTraits<v_int16>::vlanes();

        صحيح j = 0;
        لكل (; j <= بلاڪ_سائيز_2 - c_ويڪر * 2; j += c_ويڪر * 2)
        {
            v_int8 v_src1 = vx_ڀريو(src1 + j);
            v_int8 v_src2 = vx_ڀريو(src2 + j);
            v_جوڙ = v_dotprod_expand_fast(v_src1, v_src2, v_جوڙ);
        }

        لكل (; j <= بلاڪ_سائيز_2 - c_ويڪر; j += c_ويڪر)
        {
            v_int16 v_src1 = vx_ڀريو_expand(src1 + j);
            v_int16 v_src2 = vx_ڀريو_expand(src2 + j);
            v_جوڙ = v_dotprod_fast(v_src1, v_src2, v_جوڙ);
        }
        r += (مزدوج)v_reduce_جوڙ(v_جوڙ);

        src1 += بلاڪ_سائيز_2;
        src2 += بلاڪ_سائيز_2;
        i += بلاڪ_سائيز_2;
    }
    vx_cleanup();
#جيڪڏهن_پورو

    أرجع r + dotProd_(src1, src2, len - i);
}

مزدوج dotProd_16u(ثابت ushort* src1, ثابت ushort* src2, صحيح len)
{
    مزدوج r = 0.0;
    صحيح i = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_uint16>::vlanes(), بلاڪ_Size0 = (1 << 24), بلاڪ_سائيز_2;

    طالما (i < len0)
    {
        بلاڪ_سائيز_2 = std::min(len0 - i, بلاڪ_Size0);
        v_uint64 v_جوڙ = vx_setzero_u64();
        ثابت صحيح c_ويڪر = VTraits<v_uint16>::vlanes();

        صحيح j = 0;
        لكل (; j <= بلاڪ_سائيز_2 - c_ويڪر; j += c_ويڪر)
        {
            v_uint16 v_src1 = vx_ڀريو(src1 + j);
            v_uint16 v_src2 = vx_ڀريو(src2 + j);
            v_جوڙ = v_dotprod_expand_fast(v_src1, v_src2, v_جوڙ);
        }
        r += (مزدوج)v_reduce_جوڙ(v_جوڙ);

        src1 += بلاڪ_سائيز_2;
        src2 += بلاڪ_سائيز_2;
        i += بلاڪ_سائيز_2;
    }
    vx_cleanup();
#جيڪڏهن_پورو
    أرجع r + dotProd_(src1, src2, len - i);
}

مزدوج dotProd_16s(ثابت ننڍو* src1, ثابت ننڍو* src2, صحيح len)
{
    مزدوج r = 0.0;
    صحيح i = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_int16>::vlanes(), بلاڪ_Size0 = (1 << 24), بلاڪ_سائيز_2;

    طالما (i < len0)
    {
        بلاڪ_سائيز_2 = std::min(len0 - i, بلاڪ_Size0);
        v_int64 v_جوڙ = vx_setzero_s64();
        ثابت صحيح c_ويڪر = VTraits<v_int16>::vlanes();

        صحيح j = 0;
        لكل (; j <= بلاڪ_سائيز_2 - c_ويڪر; j += c_ويڪر)
        {
            v_int16 v_src1 = vx_ڀريو(src1 + j);
            v_int16 v_src2 = vx_ڀريو(src2 + j);
            v_جوڙ = v_dotprod_expand_fast(v_src1, v_src2, v_جوڙ);
        }
        r += (مزدوج)v_reduce_جوڙ(v_جوڙ);

        src1 += بلاڪ_سائيز_2;
        src2 += بلاڪ_سائيز_2;
        i += بلاڪ_سائيز_2;
    }
    vx_cleanup();
#جيڪڏهن_پورو
    أرجع r + dotProd_(src1, src2, len - i);
}

مزدوج dotProd_32s(ثابت صحيح* src1, ثابت صحيح* src2, صحيح len)
{
#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    مزدوج r = .0;
    صحيح i = 0;
    ثابت صحيح قدم  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#إذا CV_SIMD_ويڪر == 16
    ثابت صحيح wstep = قدم * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    لكل (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
    {
        v_int32 v_src10 = vx_ڀريو(src1);
        v_int32 v_src20 = vx_ڀريو(src2);
        v_int32 v_src11 = vx_ڀريو(src1 + قدم);
        v_int32 v_src21 = vx_ڀريو(src2 + قدم);
        v_sum0 = v_dotprod_expand_fast(v_src10, v_src20, v_sum0);
        v_sum1 = v_dotprod_expand_fast(v_src11, v_src21, v_sum1);
    }
    v_sum0 = v_جوڙيو(v_sum0, v_sum1);
#جيڪڏهن_پورو
    لكل (; i <= len - قدم; i += قدم, src1 += قدم, src2 += قدم)
    {
        v_int32 v_src1 = vx_ڀريو(src1);
        v_int32 v_src2 = vx_ڀريو(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_جوڙ(v_sum0);
    vx_cleanup();
    أرجع r + dotProd_(src1, src2, len - i);
#وإلا
    أرجع dotProd_(src1, src2, len);
#جيڪڏهن_پورو
}

مزدوج dotProd_32f(ثابت عائم* src1, ثابت عائم* src2, صحيح len)
{
    مزدوج r = 0.0;
    صحيح i = 0;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    صحيح len0 = len & -VTraits<v_float32>::vlanes(), بلاڪ_Size0 = (1 << 13), بلاڪ_سائيز_2;

    طالما (i < len0)
    {
        بلاڪ_سائيز_2 = std::min(len0 - i, بلاڪ_Size0);
        v_float32 v_جوڙ = vx_setzero_f32();

        صحيح j = 0;
        صحيح c_ويڪر = VTraits<v_float32>::vlanes();

#إذا CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        لكل (; j <= بلاڪ_سائيز_2 - (c_ويڪر * 4); j += (c_ويڪر * 4))
        {
            v_جوڙ  = v_muladd(vx_ڀريو(src1 + j),
                              vx_ڀريو(src2 + j), v_جوڙ);
            v_sum1 = v_muladd(vx_ڀريو(src1 + j + c_ويڪر),
                              vx_ڀريو(src2 + j + c_ويڪر), v_sum1);
            v_sum2 = v_muladd(vx_ڀريو(src1 + j + (c_ويڪر * 2)),
                              vx_ڀريو(src2 + j + (c_ويڪر * 2)), v_sum2);
            v_sum3 = v_muladd(vx_ڀريو(src1 + j + (c_ويڪر * 3)),
                              vx_ڀريو(src2 + j + (c_ويڪر * 3)), v_sum3);
        }

        v_جوڙ = v_جوڙيو(v_جوڙ, v_جوڙيو(v_جوڙيو(v_sum1, v_sum2), v_sum3));
#جيڪڏهن_پورو

        لكل (; j <= بلاڪ_سائيز_2 - c_ويڪر; j += c_ويڪر)
            v_جوڙ = v_muladd(vx_ڀريو(src1 + j), vx_ڀريو(src2 + j), v_جوڙ);

        r += v_reduce_جوڙ(v_جوڙ);

        src1 += بلاڪ_سائيز_2;
        src2 += بلاڪ_سائيز_2;
        i += بلاڪ_سائيز_2;
    }
    vx_cleanup();
#جيڪڏهن_پورو
    أرجع r + dotProd_(src1, src2, len - i);
}

مزدوج dotProd_64f(ثابت مزدوج* src1, ثابت مزدوج* src2, صحيح len)
{
    أرجع dotProd_(src1, src2, len);
}

#جيڪڏهن_پورو
CV_CPU_OPTIMIZATION_NAMESPACE_پڇاڙي
} // namespace
