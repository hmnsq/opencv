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

#اگر_تعریف_شدہ HAVE_LAPACK
#تعریف_کرو CV_GEMM_BASELINE_ONLY
#اگر_ختم
#تعریف_کرو CV_MAHALANOBIS_BASELINE_ONLY
#تعریف_کرو CV_MULTRANSPOSED_BASELINE_ONLY

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
#اگر_غیر_تعریف_شدہ CV_GEMM_SIMD_ENABLED
#اگر defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#تعریف_کرو CV_GEMM_SIMD_ENABLED 0
#ورنہ
#تعریف_کرو CV_GEMM_SIMD_ENABLED 1
#اگر_ختم
#اگر_ختم

نامخلا cv {

// forward declarations
قسمتعریف خالی (*TransformFunc)(ثابت خالی* src, خالی* dst, ثابت خالی* m, صحیح len, صحیح scn, صحیح dcn);
قسمتعریف خالی (*Scale_جوڑو_Func)(ثابت خالی* src1, ثابت خالی* src2, خالی* dst, صحیح len, ثابت خالی* alpha);
قسمتعریف خالی (*MulTransposedFunc)(ثابت Mat& src, ثابت/*preallocated*/ Mat& dst, ثابت Mat& delta, دوہرا scale);
قسمتعریف دوہرا (*MahalanobisImplFunc)(ثابت Mat& v1, ثابت Mat& v2, ثابت Mat& icovar, دوہرا *diff_بفر /*[len]*/, صحیح len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

خالی gemm32f(ثابت اعشاری* src1, size_t src1_قدم, ثابت اعشاری* src2, size_t src2_قدم,
             اعشاری alpha, ثابت اعشاری* src3, size_t src3_قدم, اعشاری beta, اعشاری* dst, size_t dst_قدم,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags);
خالی gemm64f(ثابت دوہرا* src1, size_t src1_قدم, ثابت دوہرا* src2, size_t src2_قدم,
             دوہرا alpha, ثابت دوہرا* src3, size_t src3_قدم, دوہرا beta, دوہرا* dst, size_t dst_قدم,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags);
خالی gemm32fc(ثابت اعشاری* src1, size_t src1_قدم, ثابت اعشاری* src2, size_t src2_قدم,
              اعشاری alpha, ثابت اعشاری* src3, size_t src3_قدم, اعشاری beta, اعشاری* dst, size_t dst_قدم,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags);
خالی gemm64fc(ثابت دوہرا* src1, size_t src1_قدم, ثابت دوہرا* src2, size_t src2_قدم,
              دوہرا alpha, ثابت دوہرا* src3, size_t src3_قدم, دوہرا beta, دوہرا* dst, size_t dst_قدم,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags);

TransformFunc getTransformFunc(صحیح گہرائی);
TransformFunc getDiagTransformFunc(صحیح گہرائی);
TransformFunc getPerspectiveTransform(صحیح گہرائی);

Scale_جوڑو_Func get_Scale_جوڑو_Func(صحیح گہرائی);

MahalanobisImplFunc getMahalanobisImplFunc(صحیح گہرائی);

MulTransposedFunc getMulTransposedFunc(صحیح stype, صحیح dtype, منطقی ata);

دوہرا dotProd_8u(ثابت uchar* src1, ثابت uchar* src2, صحیح len);
دوہرا dotProd_8s(ثابت schar* src1, ثابت schar* src2, صحیح len);
دوہرا dotProd_16u(ثابت ushort* src1, ثابت ushort* src2, صحیح len);
دوہرا dotProd_16s(ثابت مختصر* src1, ثابت مختصر* src2, صحیح len);
دوہرا dotProd_32s(ثابت صحیح* src1, ثابت صحیح* src2, صحیح len);
دوہرا dotProd_32f(ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح len);
دوہرا dotProd_64f(ثابت دوہرا* src1, ثابت دوہرا* src2, صحیح len);



#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#اگر !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_منوال)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

ساکن خالی
GEMM_Copy_بلاک( ثابت uchar* src, size_t src_قدم,
                uchar* dst, size_t dst_قدم,
                سائز size, size_t pix_سائز )
{
    صحیح j;
    size.چوڑائی *= (صحیح)(pix_سائز / حجم(صحیح));

    برائے( ; size.اونچائی--; src += src_قدم, dst += dst_قدم )
    {
        j=0;
         #اگر CV_ENABLE_UNROLLED
        برائے( ; j <= size.چوڑائی - 4; j += 4 )
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
        #اگر_ختم
        برائے( ; j < size.چوڑائی; j++ )
            ((صحیح*)dst)[j] = ((ثابت صحیح*)src)[j];
    }
}


ساکن خالی
GEMM_Transpose_بلاک( ثابت uchar* src, size_t src_قدم,
                     uchar* dst, size_t dst_قدم,
                     سائز size, size_t pix_سائز )
{
    صحیح i, j;
    برائے( i = 0; i < size.چوڑائی; i++, dst += dst_قدم, src += pix_سائز )
    {
        ثابت uchar* _src = src;
        انتخاب( pix_سائز )
        {
        حالت حجم(صحیح):
            برائے( j = 0; j < size.اونچائی; j++, _src += src_قدم )
                ((صحیح*)dst)[j] = ((صحیح*)_src)[0];
            توڑو;
        حالت حجم(صحیح)*2:
            برائے( j = 0; j < size.اونچائی*2; j += 2, _src += src_قدم )
            {
                صحیح t0 = ((صحیح*)_src)[0];
                صحیح t1 = ((صحیح*)_src)[1];
                ((صحیح*)dst)[j] = t0;
                ((صحیح*)dst)[j+1] = t1;
            }
            توڑو;
        حالت حجم(صحیح)*4:
            برائے( j = 0; j < size.اونچائی*4; j += 4, _src += src_قدم )
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
            توڑو;
        طےشدہ:
            CV_Assert(0);
            واپس;
        }
    }
}

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
سانچہ<قسم_نام ST> ساخت VecTraits;

سانچہ<> ساخت VecTraits<اعشاری> {
    قسمتعریف v_float32 VT;
    ساکن اندرونی VT zero() { واپس vx_setzero_f32(); }
    ساکن اندرونی VT load(ثابت اعشاری* p) { واپس vx_بھرو(p); }
    ساکن اندرونی VT setall(اعشاری v) { واپس vx_setall_f32(v); }
};
#اگر_ختم

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
سانچہ<> ساخت VecTraits<دوہرا> {
    قسمتعریف v_float64 VT;
    ساکن اندرونی VT zero() { واپس vx_setzero_f64(); }
    ساکن اندرونی VT load(ثابت دوہرا* p) { واپس vx_بھرو(p); }
    ساکن اندرونی VT setall(دوہرا v) { واپس vx_setall_f64(v); }
};
#اگر_ختم

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
سانچہ<قسم_نام ST>
ساکن اندرونی ST simd_Dot_حاصل_ضرب(ثابت ST* a, ثابت ST* b, صحیح n, صحیح& k)
{
    قسمتعریف قسم_نام VecTraits<ST>::VT VecT;
    ثابت صحیح vlanes = VTraits<VecT>::vlanes();
    ثابت صحیح stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    برائے( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_جوڑو(v_جوڑو(vs0, vs1), v_جوڑو(vs2, vs3));
    برائے( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    واپس v_reduce_مجموع(vs0);
}
#اگر_ختم

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
ساکن اندرونی دوہرا simd_Dot_حاصل_ضرب_f32f64(ثابت اعشاری* a, ثابت اعشاری* b, صحیح n, صحیح& k)
{
    ثابت صحیح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحیح stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    برائے( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_بھرو(a + k);
        v_float32 vb0 = vx_بھرو(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_بلند(va0), v_cvt_f64_بلند(vb0), vs1);

        v_float32 va1 = vx_بھرو(a + k + vlanes_f32);
        v_float32 vb1 = vx_بھرو(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_بلند(va1), v_cvt_f64_بلند(vb1), vs3);
    }
    vs0 = v_جوڑو(v_جوڑو(vs0, vs1), v_جوڑو(vs2, vs3));
    برائے( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_بھرو(a + k);
        v_float32 vb = vx_بھرو(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_بلند(va), v_cvt_f64_بلند(vb), vs0);
    }
    واپس v_reduce_مجموع(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
ساکن اندرونی خالی simdGEMM_kj_f32f64(
    ثابت اعشاری* a_ڈیٹا, ثابت اعشاری* b_ڈیٹا_2, size_t b_قدم,
    ثابت اعشاری* c_ڈیٹا, size_t c_step1,
    اعشاری* d_ڈیٹا,
    صحیح n, صحیح m,
    دوہرا alpha, دوہرا beta,
    صحیح& j)
{
    ثابت صحیح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحیح vlanes_f64 = VTraits<v_float64>::vlanes();

    اگر( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ثابت صحیح max_m = 1600 / (صحیح)حجم(اعشاری);
        cv::Auto_بفر<دوہرا> _s_buf(max_m);
        دوہرا* s_buf = _s_buf.ڈیٹا();

        {
            v_float64 vz = vx_setzero_f64();
            برائے( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            برائے( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ثابت اعشاری* b_ptr = b_ڈیٹا_2;
            برائے( صحیح k = 0; k < n; k++, b_ptr += b_قدم )
            {
                v_float64 va = vx_setall_f64((دوہرا)a_ڈیٹا[k]);
                j = 0;
                برائے( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_بھرو(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_بلند(vb);
                    v_float64 vd_lo = vx_بھرو(s_buf + j);
                    v_float64 vd_hi = vx_بھرو(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                دوہرا a_val = (دوہرا)a_ڈیٹا[k];
                برائے( ; j < m; j++ )
                    s_buf[j] += a_val * (دوہرا)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ثابت اعشاری* c_ڈیٹا_2 = c_ڈیٹا;
            j = 0;
            اگر( !c_ڈیٹا )
            {
                برائے( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_بھرو(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_بھرو(s_buf + j + vlanes_f64), valpha);
                    v_store(d_ڈیٹا + j, v_cvt_f32(vs_lo, vs_hi));
                }
                برائے( ; j < m; j++ )
                    d_ڈیٹا[j] = (اعشاری)(s_buf[j] * alpha);
            }
            ورنہ اگر( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                برائے( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_بھرو(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_بھرو(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_بھرو(c_ڈیٹا_2 + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_بلند(vc), vbeta, vs_hi);
                    v_store(d_ڈیٹا + j, v_cvt_f32(vs_lo, vs_hi));
                }
                برائے( ; j < m; j++ )
                    d_ڈیٹا[j] = (اعشاری)(s_buf[j] * alpha + (دوہرا)c_ڈیٹا_2[j] * beta);
            }
            ورنہ
            {
                برائے( j = 0; j < m; j++, c_ڈیٹا_2 += c_step1 )
                    d_ڈیٹا[j] = (اعشاری)(s_buf[j] * alpha + (دوہرا)c_ڈیٹا_2[0] * beta);
            }
        }
        j = m;
    }
    ورنہ اگر( vlanes_f32 * (صحیح)حجم(اعشاری) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ثابت اعشاری* c_ڈیٹا_2 = c_ڈیٹا;
        برائے( ; j <= m - vlanes_f32; j += vlanes_f32, c_ڈیٹا_2 += vlanes_f32 * c_step1 )
        {
            ثابت اعشاری* b = b_ڈیٹا_2 + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            برائے( صحیح k = 0; k < n; k++, b += b_قدم )
            {
                v_float64 va = vx_setall_f64((دوہرا)a_ڈیٹا[k]);
                v_float32 vb = vx_بھرو(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_بلند(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            اگر( !c_ڈیٹا )
            {
                v_store(d_ڈیٹا + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ورنہ اگر( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_بھرو(c_ڈیٹا_2);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_بلند(vc), vbeta, vs_hi);
                v_store(d_ڈیٹا + j, v_cvt_f32(vs_lo, vs_hi));
            }
            ورنہ
            {
                CV_DECL_ALIGNED(CV_SIMD_چوڑائی) اعشاری buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                برائے( صحیح jj = 0; jj < vlanes_f32; jj++ )
                    d_ڈیٹا[j + jj] = buf[jj] + c_ڈیٹا_2[jj * c_step1] * (اعشاری)beta;
            }
        }
    }
}
#اگر_ختم

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
سانچہ<قسم_نام ST>
ساکن اندرونی خالی simdGEMM_kj(
    ثابت ST* a_ڈیٹا, ثابت ST* b_ڈیٹا_2, size_t b_قدم,
    ثابت ST* c_ڈیٹا, size_t c_step1,
    ST* d_ڈیٹا,
    صحیح n, صحیح m,
    دوہرا alpha, دوہرا beta,
    صحیح& j)
{
    قسمتعریف قسم_نام VecTraits<ST>::VT VecT;
    ثابت صحیح vlanes = VTraits<VecT>::vlanes();

    اگر( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ثابت صحیح max_m = 1600 / (صحیح)حجم(ST);
        CV_DECL_ALIGNED(CV_SIMD_چوڑائی) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            برائے( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            برائے( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ثابت ST* b_ptr = b_ڈیٹا_2;
            برائے( صحیح k = 0; k < n; k++, b_ptr += b_قدم )
            {
                VecT va = VecTraits<ST>::setall(a_ڈیٹا[k]);
                j = 0;
                برائے( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_بھرو_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_ڈیٹا[k];
                برائے( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ثابت ST* c_ڈیٹا_2 = c_ڈیٹا;
            j = 0;
            اگر( !c_ڈیٹا )
            {
                برائے( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_بھرو_aligned(s_buf + j), valpha);
                    v_store(d_ڈیٹا + j, vs);
                }
                برائے( ; j < m; j++ )
                    d_ڈیٹا[j] = s_buf[j] * (ST)alpha;
            }
            ورنہ اگر( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                برائے( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_بھرو_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_ڈیٹا_2 + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_ڈیٹا + j, vs);
                }
                برائے( ; j < m; j++ )
                    d_ڈیٹا[j] = s_buf[j] * (ST)alpha + c_ڈیٹا_2[j] * (ST)beta;
            }
            ورنہ
            {
                برائے( j = 0; j < m; j++, c_ڈیٹا_2 += c_step1 )
                    d_ڈیٹا[j] = s_buf[j] * (ST)alpha + c_ڈیٹا_2[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    ورنہ اگر( vlanes * (صحیح)حجم(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ثابت ST* c_ڈیٹا_2 = c_ڈیٹا;
        برائے( ; j <= m - vlanes; j += vlanes, c_ڈیٹا_2 += vlanes * c_step1 )
        {
            ثابت ST* b = b_ڈیٹا_2 + j;
            VecT vs = VecTraits<ST>::zero();
            برائے( صحیح k = 0; k < n; k++, b += b_قدم )
            {
                VecT va = VecTraits<ST>::setall(a_ڈیٹا[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            اگر( !c_ڈیٹا )
            {
                v_store(d_ڈیٹا + j, vs);
            }
            ورنہ اگر( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_ڈیٹا_2);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_ڈیٹا + j, vs);
            }
            ورنہ
            {
                CV_DECL_ALIGNED(CV_SIMD_چوڑائی) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                برائے( صحیح jj = 0; jj < vlanes; jj++ )
                    d_ڈیٹا[j + jj] = buf[jj] + c_ڈیٹا_2[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#اگر_ختم


سانچہ<قسم_نام T, قسم_نام WT> ساکن خالی
GEMMSingleMul( ثابت T* a_ڈیٹا, size_t a_قدم,
               ثابت T* b_ڈیٹا, size_t b_قدم,
               ثابت T* c_ڈیٹا_2, size_t c_قدم,
               T* d_ڈیٹا, size_t d_قدم,
               سائز a_سائز, سائز d_سائز,
               دوہرا alpha, دوہرا beta, صحیح flags )
{
    صحیح i, j, k, n = a_سائز.چوڑائی, m = d_سائز.چوڑائی, drows = d_سائز.اونچائی;
    ثابت T *a_ڈیٹا_2 = a_ڈیٹا, *b_ڈیٹا_2 = b_ڈیٹا, *c_ڈیٹا = c_ڈیٹا_2;
    cv::Auto_بفر<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_قدم;

    a_قدم /= حجم(a_ڈیٹا[0]);
    b_قدم /= حجم(b_ڈیٹا[0]);
    c_قدم /= حجم(c_ڈیٹا_2[0]);
    d_قدم /= حجم(d_ڈیٹا[0]);
    a_step0 = a_قدم;
    a_step1 = 1;

    اگر( !c_ڈیٹا_2 )
        c_step0 = c_step1 = 0;
    ورنہ اگر( !(flags & GEMM_3_T) )
        c_step0 = c_قدم, c_step1 = 1;
    ورنہ
        c_step0 = 1, c_step1 = c_قدم;

    اگر( flags & GEMM_1_T )
    {
        CV_تبادلہ( a_step0, a_step1, t_قدم );
        n = a_سائز.اونچائی;
        اگر( a_قدم > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.ڈیٹا();
        }
    }

    اگر( n == 1 ) /* external product */
    {
        cv::Auto_بفر<T> _b_buf;
        T* b_buf = 0;

        اگر( a_قدم > 1 && a_سائز.اونچائی > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.ڈیٹا();
            برائے( k = 0; k < drows; k++ )
                a_buf[k] = a_ڈیٹا[a_قدم*k];
            a_ڈیٹا = a_buf;
        }

        اگر( b_قدم > 1 )
        {
            _b_buf.allocate(d_سائز.چوڑائی);
            b_buf = _b_buf.ڈیٹا();
            برائے( j = 0; j < d_سائز.چوڑائی; j++ )
                b_buf[j] = b_ڈیٹا[j*b_قدم];
            b_ڈیٹا = b_buf;
        }

        برائے( i = 0; i < drows; i++, c_ڈیٹا += c_step0, d_ڈیٹا += d_قدم )
        {
            WT al = WT(a_ڈیٹا[i])*alpha;
            c_ڈیٹا_2 = c_ڈیٹا;
            برائے( j = 0; j <= d_سائز.چوڑائی - 2; j += 2, c_ڈیٹا_2 += 2*c_step1 )
            {
                WT s0 = al*WT(b_ڈیٹا[j]);
                WT s1 = al*WT(b_ڈیٹا[j+1]);
                اگر( !c_ڈیٹا_2 )
                {
                    d_ڈیٹا[j] = T(s0);
                    d_ڈیٹا[j+1] = T(s1);
                }
                ورنہ
                {
                    d_ڈیٹا[j] = T(s0 + WT(c_ڈیٹا_2[0])*beta);
                    d_ڈیٹا[j+1] = T(s1 + WT(c_ڈیٹا_2[c_step1])*beta);
                }
            }

            برائے( ; j < d_سائز.چوڑائی; j++, c_ڈیٹا_2 += c_step1 )
            {
                WT s0 = al*WT(b_ڈیٹا[j]);
                اگر( !c_ڈیٹا_2 )
                    d_ڈیٹا[j] = T(s0);
                ورنہ
                    d_ڈیٹا[j] = T(s0 + WT(c_ڈیٹا_2[0])*beta);
            }
        }
    }
    ورنہ اگر( flags & GEMM_2_T ) /* A * Bt */
    {
        برائے( i = 0; i < drows; i++, a_ڈیٹا_2 += a_step0, c_ڈیٹا += c_step0, d_ڈیٹا += d_قدم )
        {
            a_ڈیٹا = a_ڈیٹا_2;
            b_ڈیٹا = b_ڈیٹا_2;
            c_ڈیٹا_2 = c_ڈیٹا;

            اگر( a_buf )
            {
                برائے( k = 0; k < n; k++ )
                    a_buf[k] = a_ڈیٹا[a_step1*k];
                a_ڈیٹا = a_buf;
            }

            برائے( j = 0; j < d_سائز.چوڑائی; j++, b_ڈیٹا += b_قدم,
                                               c_ڈیٹا_2 += c_step1 )
            {
                WT s0(0);
                k = 0;
#اگر CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                اگر( حجم(WT) == حجم(دوہرا) )
                {
                    اگر( حجم(T) == حجم(دوہرا) )
                        s0 = (WT)simd_Dot_حاصل_ضرب((ثابت دوہرا*)a_ڈیٹا, (ثابت دوہرا*)b_ڈیٹا, n, k);
                    ورنہ اگر( حجم(T) == حجم(اعشاری) )
                        s0 = (WT)simd_Dot_حاصل_ضرب_f32f64((ثابت اعشاری*)a_ڈیٹا, (ثابت اعشاری*)b_ڈیٹا, n, k);
                }
                ورنہ
#اگر_ختم
                {
                    WT s1(0), s2(0), s3(0);
                     #اگر CV_ENABLE_UNROLLED
                    برائے( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_ڈیٹا[k])*WT(b_ڈیٹا[k]);
                        s1 += WT(a_ڈیٹا[k+1])*WT(b_ڈیٹا[k+1]);
                        s2 += WT(a_ڈیٹا[k+2])*WT(b_ڈیٹا[k+2]);
                        s3 += WT(a_ڈیٹا[k+3])*WT(b_ڈیٹا[k+3]);
                    }
                    #اگر_ختم
                    s0 += s1 + s2 + s3;
                }
                برائے( ; k < n; k++ )
                    s0 += WT(a_ڈیٹا[k])*WT(b_ڈیٹا[k]);
                s0 = s0*alpha;

                اگر( !c_ڈیٹا_2 )
                    d_ڈیٹا[j] = T(s0);
                ورنہ
                    d_ڈیٹا[j] = T(s0 + WT(c_ڈیٹا_2[0])*beta);
            }
        }
    }
    ورنہ اگر( d_سائز.چوڑائی*حجم(d_ڈیٹا[0]) <= 1600 )
    {
        برائے( i = 0; i < drows; i++, a_ڈیٹا_2 += a_step0,
                                    c_ڈیٹا += c_step0,
                                    d_ڈیٹا += d_قدم )
        {
            a_ڈیٹا = a_ڈیٹا_2, c_ڈیٹا_2 = c_ڈیٹا;

            اگر( a_buf )
            {
                برائے( k = 0; k < n; k++ )
                    a_buf[k] = a_ڈیٹا[a_step1*k];
                a_ڈیٹا = a_buf;
            }

            j = 0;
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            اگر( حجم(WT) == حجم(دوہرا) )
            {
                اگر( حجم(T) == حجم(دوہرا) )
                    simdGEMM_kj((ثابت دوہرا*)a_ڈیٹا, (ثابت دوہرا*)b_ڈیٹا_2, b_قدم,
                                        (ثابت دوہرا*)c_ڈیٹا, c_step1,
                                (دوہرا*)d_ڈیٹا, n, m, alpha, beta, j);
                ورنہ اگر( حجم(T) == حجم(اعشاری) )
                    simdGEMM_kj_f32f64((ثابت اعشاری*)a_ڈیٹا, (ثابت اعشاری*)b_ڈیٹا_2, b_قدم,
                                        (ثابت اعشاری*)c_ڈیٹا, c_step1,
                                (اعشاری*)d_ڈیٹا, n, m, alpha, beta, j);
            }
#اگر_ختم
            c_ڈیٹا_2 = c_ڈیٹا + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            برائے( ; j <= m - 4; j += 4, c_ڈیٹا_2 += 4*c_step1 )
            {
                ثابت T* b = b_ڈیٹا_2 + j;
                WT s0(0), s1(0), s2(0), s3(0);

                برائے( k = 0; k < n; k++, b += b_قدم )
                {
                    WT a(a_ڈیٹا[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                اگر( !c_ڈیٹا_2 )
                {
                    d_ڈیٹا[j] = T(s0*alpha);
                    d_ڈیٹا[j+1] = T(s1*alpha);
                    d_ڈیٹا[j+2] = T(s2*alpha);
                    d_ڈیٹا[j+3] = T(s3*alpha);
                }
                ورنہ
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_ڈیٹا[j] = T(s0 + WT(c_ڈیٹا_2[0])*beta);
                    d_ڈیٹا[j+1] = T(s1 + WT(c_ڈیٹا_2[c_step1])*beta);
                    d_ڈیٹا[j+2] = T(s2 + WT(c_ڈیٹا_2[c_step1*2])*beta);
                    d_ڈیٹا[j+3] = T(s3 + WT(c_ڈیٹا_2[c_step1*3])*beta);
                }
            }

            برائے( ; j < m; j++, c_ڈیٹا_2 += c_step1 )
            {
                ثابت T* b = b_ڈیٹا_2 + j;
                WT s0(0);

                برائے( k = 0; k < n; k++, b += b_قدم )
                    s0 += WT(a_ڈیٹا[k]) * WT(b[0]);

                s0 = s0*alpha;
                اگر( !c_ڈیٹا_2 )
                    d_ڈیٹا[j] = T(s0);
                ورنہ
                    d_ڈیٹا[j] = T(s0 + WT(c_ڈیٹا_2[0])*beta);
            }
        }
    }
    ورنہ
    {
        cv::Auto_بفر<WT> _d_buf(m);
        WT* d_buf = _d_buf.ڈیٹا();

        برائے( i = 0; i < drows; i++, a_ڈیٹا_2 += a_step0, c_ڈیٹا += c_step0, d_ڈیٹا += d_قدم )
        {
            a_ڈیٹا = a_ڈیٹا_2;
            b_ڈیٹا = b_ڈیٹا_2;
            c_ڈیٹا_2 = c_ڈیٹا;

            اگر( a_buf )
            {
                برائے( k = 0; k < n; k++ )
                    a_buf[k] = a_ڈیٹا_2[a_step1*k];
                a_ڈیٹا = a_buf;
            }

            برائے( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            برائے( k = 0; k < n; k++, b_ڈیٹا += b_قدم )
            {
                WT al(a_ڈیٹا[k]);
                j=0;
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                اگر( حجم(WT) == حجم(دوہرا) )
                {
                    اگر( حجم(T) == حجم(دوہرا) )
                    {
                        ثابت صحیح vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ثابت دوہرا*)&al));
                        برائے( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_بھرو((ثابت دوہرا*)(d_buf + j));
                            v_float64 vb = vx_بھرو((ثابت دوہرا*)(b_ڈیٹا + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((دوہرا*)(d_buf + j), vd);
                        }
                    }
                    ورنہ
                    {
                        ثابت صحیح vlanes_f32 = VTraits<v_float32>::vlanes();
                        ثابت صحیح vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ثابت دوہرا*)&al));
                        برائے( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_بھرو((ثابت اعشاری*)(b_ڈیٹا + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_بلند(vb);
                            v_float64 vd_lo = vx_بھرو((ثابت دوہرا*)(d_buf + j));
                            v_float64 vd_hi = vx_بھرو((ثابت دوہرا*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((دوہرا*)(d_buf + j), vd_lo);
                            v_store((دوہرا*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#اگر_ختم
                برائے( ; j < m; j++ )
                    d_buf[j] += WT(b_ڈیٹا[j])*al;
            }

            اگر( !c_ڈیٹا_2 )
                برائے( j = 0; j < m; j++ )
                    d_ڈیٹا[j] = T(d_buf[j]*alpha);
            ورنہ
                برائے( j = 0; j < m; j++, c_ڈیٹا_2 += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_ڈیٹا[j] = T(t + WT(c_ڈیٹا_2[0])*beta);
                }
        }
    }
}
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
سانچہ<قسم_نام WT>
ساکن اندرونی خالی سیٹ_Double_ڈیٹا_Zero(WT* d_ڈیٹا, صحیح m, صحیح vlanes)
{
    v_float64 vz = vx_setzero_f64();
    صحیح j = 0;
    برائے( ; j <= m - vlanes; j += vlanes )
        v_store((دوہرا*)(d_ڈیٹا + j), vz);
    برائے( ; j < m; j++ )
        d_ڈیٹا[j] = WT(0);
}

سانچہ<قسم_نام ST>
ساکن اندرونی خالی simd_Mul_جوڑو(ثابت ST* b, دوہرا* d, صحیح m, دوہرا a_val);

سانچہ<>
اندرونی خالی simd_Mul_جوڑو<اعشاری>(
    ثابت اعشاری* b, دوہرا* d, صحیح m, دوہرا a_val)
{
    ثابت صحیح vlanes_f32 = VTraits<v_float32>::vlanes();
    ثابت صحیح vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    صحیح j = 0;
    برائے( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_بھرو(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_بلند(vb);
        v_float64 vd_lo = vx_بھرو(d + j);
        v_float64 vd_hi = vx_بھرو(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    برائے( ; j < m; j++ )
        d[j] += a_val * (دوہرا)b[j];
}

سانچہ<>
اندرونی خالی simd_Mul_جوڑو<دوہرا>(
    ثابت دوہرا* b, دوہرا* d, صحیح m, دوہرا a_val)
{
    ثابت صحیح vlanes = VTraits<v_float64>::vlanes();
    ثابت صحیح stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    صحیح j = 0;
    برائے( ; j <= m - stride2; j += stride2 )
    {
        v_float64 vd0 = vx_بھرو(d + j);
        v_float64 vb0 = vx_بھرو(b + j);
        v_float64 vd1 = vx_بھرو(d + j + vlanes);
        v_float64 vb1 = vx_بھرو(b + j + vlanes);
        vd0 = v_muladd(va, vb0, vd0);
        vd1 = v_muladd(va, vb1, vd1);
        v_store(d + j, vd0);
        v_store(d + j + vlanes, vd1);
    }
    برائے( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_بھرو(d + j);
        v_float64 vb = vx_بھرو(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    برائے( ; j < m; j++ )
        d[j] += a_val * b[j];
}

سانچہ<قسم_نام ST>
ساکن اندرونی خالی simd_بلاک_Mul_kj(
    ثابت ST* a_ڈیٹا, ثابت ST* b_ڈیٹا, size_t b_قدم,
    دوہرا* d_ڈیٹا, صحیح n, صحیح m, منطقی do_acc)
{
    ثابت صحیح vlanes = VTraits<v_float64>::vlanes();
    اگر( !do_acc )
        سیٹ_Double_ڈیٹا_Zero(d_ڈیٹا, m, vlanes);
    برائے( صحیح k = 0; k < n; k++, b_ڈیٹا += b_قدم )
        simd_Mul_جوڑو(b_ڈیٹا, d_ڈیٹا, m, (دوہرا)a_ڈیٹا[k]);
}
#اگر_ختم // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

سانچہ<قسم_نام T, قسم_نام WT> ساکن خالی
GEMMBlockMul( ثابت T* a_ڈیٹا, size_t a_قدم,
              ثابت T* b_ڈیٹا, size_t b_قدم,
              WT* d_ڈیٹا, size_t d_قدم,
              سائز a_سائز, سائز d_سائز, صحیح flags )
{
    صحیح i, j, k, n = a_سائز.چوڑائی, m = d_سائز.چوڑائی;
    ثابت T *a_ڈیٹا_2 = a_ڈیٹا, *b_ڈیٹا_2 = b_ڈیٹا;
    cv::Auto_بفر<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_قدم;
    صحیح do_acc = flags & 16;

    a_قدم /= حجم(a_ڈیٹا[0]);
    b_قدم /= حجم(b_ڈیٹا[0]);
    d_قدم /= حجم(d_ڈیٹا[0]);

    a_step0 = a_قدم;
    a_step1 = 1;

    اگر( flags & GEMM_1_T )
    {
        CV_تبادلہ( a_step0, a_step1, t_قدم );
        n = a_سائز.اونچائی;
        _a_buf.allocate(n);
        a_buf = _a_buf.ڈیٹا();
    }

    اگر( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        برائے( i = 0; i < d_سائز.اونچائی; i++, a_ڈیٹا_2 += a_step0, d_ڈیٹا += d_قدم )
        {
            a_ڈیٹا = a_ڈیٹا_2; b_ڈیٹا = b_ڈیٹا_2;

            اگر( a_buf )
            {
                برائے( k = 0; k < n; k++ )
                    a_buf[k] = a_ڈیٹا[a_step1*k];
                a_ڈیٹا = a_buf;
            }

            برائے( j = 0; j < d_سائز.چوڑائی; j++, b_ڈیٹا += b_قدم )
            {
                WT s0 = do_acc ? d_ڈیٹا[j] : WT(0);
                k = 0;
#اگر CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                اگر( حجم(WT) == حجم(دوہرا) )
                {
                    اگر( حجم(T) == حجم(دوہرا) )
                        s0 += (WT)simd_Dot_حاصل_ضرب((ثابت دوہرا*)a_ڈیٹا, (ثابت دوہرا*)b_ڈیٹا, n, k);
                    ورنہ اگر( حجم(T) == حجم(اعشاری) )
                        s0 += (WT)simd_Dot_حاصل_ضرب_f32f64((ثابت اعشاری*)a_ڈیٹا, (ثابت اعشاری*)b_ڈیٹا, n, k);
                }
                ورنہ
#اگر_ختم
                {
                    WT s1(0);
                    برائے( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_ڈیٹا[k])*WT(b_ڈیٹا[k]);
                        s1 += WT(a_ڈیٹا[k+1])*WT(b_ڈیٹا[k+1]);
                    }
                    s0 += s1;
                }
                برائے( ; k < n; k++ )
                    s0 += WT(a_ڈیٹا[k])*WT(b_ڈیٹا[k]);

                d_ڈیٹا[j] = s0;
            }
        }
    }
    ورنہ
    {
        برائے( i = 0; i < d_سائز.اونچائی; i++, a_ڈیٹا_2 += a_step0, d_ڈیٹا += d_قدم )
        {
            a_ڈیٹا = a_ڈیٹا_2, b_ڈیٹا = b_ڈیٹا_2;

            اگر( a_buf )
            {
                برائے( k = 0; k < n; k++ )
                    a_buf[k] = a_ڈیٹا[a_step1*k];
                a_ڈیٹا = a_buf;
            }
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            اگر( حجم(WT) == حجم(دوہرا) )
            {
                اگر( حجم(T) == حجم(دوہرا) )
                    simd_بلاک_Mul_kj(
                        (ثابت دوہرا*)a_ڈیٹا, (ثابت دوہرا*)b_ڈیٹا, b_قدم,
                        (دوہرا*)d_ڈیٹا, n, m, do_acc != 0);
                ورنہ
                    simd_بلاک_Mul_kj(
                        (ثابت اعشاری*)a_ڈیٹا, (ثابت اعشاری*)b_ڈیٹا, b_قدم,
                        (دوہرا*)d_ڈیٹا, n, m, do_acc != 0);
            }
            ورنہ
#اگر_ختم
            {
                برائے( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ثابت T* b = b_ڈیٹا + j;

                    اگر( do_acc )
                    {
                        s0 = d_ڈیٹا[j]; s1 = d_ڈیٹا[j+1];
                        s2 = d_ڈیٹا[j+2]; s3 = d_ڈیٹا[j+3];
                    }
                    ورنہ
                        s0 = s1 = s2 = s3 = WT(0);

                    برائے( k = 0; k < n; k++, b += b_قدم )
                    {
                        WT a(a_ڈیٹا[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_ڈیٹا[j] = s0; d_ڈیٹا[j+1] = s1;
                    d_ڈیٹا[j+2] = s2; d_ڈیٹا[j+3] = s3;
                }

                برائے( ; j < m; j++ )
                {
                    ثابت T* b = b_ڈیٹا + j;
                    WT s0 = do_acc ? d_ڈیٹا[j] : WT(0);

                    برائے( k = 0; k < n; k++, b += b_قدم )
                        s0 += WT(a_ڈیٹا[k]) * WT(b[0]);

                    d_ڈیٹا[j] = s0;
                }
            }
        }
    }
}


سانچہ<قسم_نام T, قسم_نام WT> ساکن خالی
GEMMStore( ثابت T* c_ڈیٹا_2, size_t c_قدم,
           ثابت WT* d_buf, size_t d_buf_قدم,
           T* d_ڈیٹا, size_t d_قدم, سائز d_سائز,
           دوہرا alpha, دوہرا beta, صحیح flags )
{
    ثابت T* c_ڈیٹا = c_ڈیٹا_2;
    صحیح j;
    size_t c_step0, c_step1;

    c_قدم /= حجم(c_ڈیٹا_2[0]);
    d_buf_قدم /= حجم(d_buf[0]);
    d_قدم /= حجم(d_ڈیٹا[0]);

    اگر( !c_ڈیٹا_2 )
        c_step0 = c_step1 = 0;
    ورنہ اگر( !(flags & GEMM_3_T) )
        c_step0 = c_قدم, c_step1 = 1;
    ورنہ
        c_step0 = 1, c_step1 = c_قدم;

    برائے( ; d_سائز.اونچائی--; c_ڈیٹا += c_step0, d_buf += d_buf_قدم, d_ڈیٹا += d_قدم )
    {
        اگر( c_ڈیٹا )
        {
            c_ڈیٹا_2 = c_ڈیٹا;
            j=0;
             #اگر CV_ENABLE_UNROLLED
            برائے(; j <= d_سائز.چوڑائی - 4; j += 4, c_ڈیٹا_2 += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_ڈیٹا_2[0]);
                t1 += beta*WT(c_ڈیٹا_2[c_step1]);
                d_ڈیٹا[j] = T(t0);
                d_ڈیٹا[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_ڈیٹا_2[c_step1*2]);
                t1 += beta*WT(c_ڈیٹا_2[c_step1*3]);
                d_ڈیٹا[j+2] = T(t0);
                d_ڈیٹا[j+3] = T(t1);
            }
            #اگر_ختم
            برائے( ; j < d_سائز.چوڑائی; j++, c_ڈیٹا_2 += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_ڈیٹا[j] = T(t0 + WT(c_ڈیٹا_2[0])*beta);
            }
        }
        ورنہ
        {
            j = 0;
             #اگر CV_ENABLE_UNROLLED
            برائے( ; j <= d_سائز.چوڑائی - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_ڈیٹا[j] = T(t0);
                d_ڈیٹا[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_ڈیٹا[j+2] = T(t0);
                d_ڈیٹا[j+3] = T(t1);
            }
            #اگر_ختم
            برائے( ; j < d_سائز.چوڑائی; j++ )
                d_ڈیٹا[j] = T(alpha*d_buf[j]);
        }
    }
}


قسمتعریف خالی (*GEMMSingleMulFunc)( ثابت خالی* src1, size_t step1,
                   ثابت خالی* src2, size_t step2, ثابت خالی* src3, size_t step3,
                   خالی* dst, size_t dststep, سائز srcsize, سائز dstsize,
                   دوہرا alpha, دوہرا beta, صحیح flags );

قسمتعریف خالی (*GEMMBlockMulFunc)( ثابت خالی* src1, size_t step1,
                   ثابت خالی* src2, size_t step2, خالی* dst, size_t dststep,
                   سائز srcsize, سائز dstsize, صحیح flags );

قسمتعریف خالی (*GEMMStoreFunc)( ثابت خالی* src1, size_t step1,
                   ثابت خالی* src2, size_t step2, خالی* dst, size_t dststep,
                   سائز dstsize, دوہرا alpha, دوہرا beta, صحیح flags );

سانچہ<قسم_نام T, قسم_نام WT, خالی (*fn)(ثابت T*, size_t, ثابت T*, size_t, ثابت T*, size_t, T*, size_t, سائز, سائز, دوہرا, دوہرا, صحیح)>
ساکن خالی gemmSingleMulWrap( ثابت خالی* src1, size_t step1,
                   ثابت خالی* src2, size_t step2, ثابت خالی* src3, size_t step3,
                   خالی* dst, size_t dststep, سائز srcsize, سائز dstsize,
                   دوہرا alpha, دوہرا beta, صحیح flags )
{
    fn((ثابت T*)src1, step1, (ثابت T*)src2, step2, (ثابت T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

سانچہ<قسم_نام T, قسم_نام WT, خالی (*fn)(ثابت T*, size_t, ثابت T*, size_t, WT*, size_t, سائز, سائز, صحیح)>
ساکن خالی gemm_بلاک_Mul_Wrap( ثابت خالی* src1, size_t step1,
                   ثابت خالی* src2, size_t step2, خالی* dst, size_t dststep,
                   سائز srcsize, سائز dstsize, صحیح flags )
{
    fn((ثابت T*)src1, step1, (ثابت T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

سانچہ<قسم_نام T, قسم_نام WT, خالی (*fn)(ثابت T*, size_t, ثابت WT*, size_t, T*, size_t, سائز, دوہرا, دوہرا, صحیح)>
ساکن خالی gemmStoreWrap( ثابت خالی* src1, size_t step1,
                   ثابت خالی* src2, size_t step2, خالی* dst, size_t dststep,
                   سائز dstsize, دوہرا alpha, دوہرا beta, صحیح flags )
{
    fn((ثابت T*)src1, step1, (ثابت WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

ساکن خالی GEMMSingleMul_32f( ثابت اعشاری* a_ڈیٹا, size_t a_قدم,
              ثابت اعشاری* b_ڈیٹا, size_t b_قدم,
              ثابت اعشاری* c_ڈیٹا_2, size_t c_قدم,
              اعشاری* d_ڈیٹا, size_t d_قدم,
              سائز a_سائز, سائز d_سائز,
              دوہرا alpha, دوہرا beta, صحیح flags )
{
    GEMMSingleMul<اعشاری,دوہرا>(a_ڈیٹا, a_قدم, b_ڈیٹا, b_قدم, c_ڈیٹا_2,
                                c_قدم, d_ڈیٹا, d_قدم, a_سائز, d_سائز,
                                alpha, beta, flags);
}

ساکن خالی GEMMSingleMul_64f( ثابت دوہرا* a_ڈیٹا, size_t a_قدم,
                              ثابت دوہرا* b_ڈیٹا, size_t b_قدم,
                              ثابت دوہرا* c_ڈیٹا_2, size_t c_قدم,
                              دوہرا* d_ڈیٹا, size_t d_قدم,
                              سائز a_سائز, سائز d_سائز,
                              دوہرا alpha, دوہرا beta, صحیح flags )
{
    GEMMSingleMul<دوہرا,دوہرا>(a_ڈیٹا, a_قدم, b_ڈیٹا, b_قدم, c_ڈیٹا_2,
                                c_قدم, d_ڈیٹا, d_قدم, a_سائز, d_سائز,
                                alpha, beta, flags);
}


ساکن خالی GEMMSingleMul_32fc( ثابت Complexf* a_ڈیٹا, size_t a_قدم,
                              ثابت Complexf* b_ڈیٹا, size_t b_قدم,
                              ثابت Complexf* c_ڈیٹا_2, size_t c_قدم,
                              Complexf* d_ڈیٹا, size_t d_قدم,
                              سائز a_سائز, سائز d_سائز,
                              دوہرا alpha, دوہرا beta, صحیح flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_ڈیٹا, a_قدم, b_ڈیٹا, b_قدم, c_ڈیٹا_2,
                                c_قدم, d_ڈیٹا, d_قدم, a_سائز, d_سائز,
                                alpha, beta, flags);
}

ساکن خالی GEMMSingleMul_64fc( ثابت Complexd* a_ڈیٹا, size_t a_قدم,
                              ثابت Complexd* b_ڈیٹا, size_t b_قدم,
                              ثابت Complexd* c_ڈیٹا_2, size_t c_قدم,
                              Complexd* d_ڈیٹا, size_t d_قدم,
                              سائز a_سائز, سائز d_سائز,
                              دوہرا alpha, دوہرا beta, صحیح flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_ڈیٹا, a_قدم, b_ڈیٹا, b_قدم, c_ڈیٹا_2,
                                 c_قدم, d_ڈیٹا, d_قدم, a_سائز, d_سائز,
                                 alpha, beta, flags);
}

ساکن خالی GEMMBlockMul_32f( ثابت اعشاری* a_ڈیٹا, size_t a_قدم,
             ثابت اعشاری* b_ڈیٹا, size_t b_قدم,
             دوہرا* d_ڈیٹا, size_t d_قدم,
             سائز a_سائز, سائز d_سائز, صحیح flags )
{
    GEMMBlockMul(a_ڈیٹا, a_قدم, b_ڈیٹا, b_قدم, d_ڈیٹا, d_قدم, a_سائز, d_سائز, flags);
}


ساکن خالی GEMMBlockMul_64f( ثابت دوہرا* a_ڈیٹا, size_t a_قدم,
                             ثابت دوہرا* b_ڈیٹا, size_t b_قدم,
                             دوہرا* d_ڈیٹا, size_t d_قدم,
                             سائز a_سائز, سائز d_سائز, صحیح flags )
{
    GEMMBlockMul(a_ڈیٹا, a_قدم, b_ڈیٹا, b_قدم, d_ڈیٹا, d_قدم, a_سائز, d_سائز, flags);
}


ساکن خالی GEMMBlockMul_32fc( ثابت Complexf* a_ڈیٹا, size_t a_قدم,
                             ثابت Complexf* b_ڈیٹا, size_t b_قدم,
                             Complexd* d_ڈیٹا, size_t d_قدم,
                             سائز a_سائز, سائز d_سائز, صحیح flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_ڈیٹا, a_قدم, b_ڈیٹا, b_قدم, d_ڈیٹا, d_قدم, a_سائز, d_سائز, flags);
}


ساکن خالی GEMMBlockMul_64fc( ثابت Complexd* a_ڈیٹا, size_t a_قدم,
                             ثابت Complexd* b_ڈیٹا, size_t b_قدم,
                             Complexd* d_ڈیٹا, size_t d_قدم,
                             سائز a_سائز, سائز d_سائز, صحیح flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_ڈیٹا, a_قدم, b_ڈیٹا, b_قدم, d_ڈیٹا, d_قدم, a_سائز, d_سائز, flags);
}


ساکن خالی GEMMStore_32f( ثابت اعشاری* c_ڈیٹا_2, size_t c_قدم,
          ثابت دوہرا* d_buf, size_t d_buf_قدم,
          اعشاری* d_ڈیٹا, size_t d_قدم, سائز d_سائز,
          دوہرا alpha, دوہرا beta, صحیح flags )
{
    GEMMStore(c_ڈیٹا_2, c_قدم, d_buf, d_buf_قدم, d_ڈیٹا, d_قدم, d_سائز, alpha, beta, flags);
}


ساکن خالی GEMMStore_64f( ثابت دوہرا* c_ڈیٹا_2, size_t c_قدم,
                      ثابت دوہرا* d_buf, size_t d_buf_قدم,
                      دوہرا* d_ڈیٹا, size_t d_قدم, سائز d_سائز,
                      دوہرا alpha, دوہرا beta, صحیح flags )
{
    GEMMStore(c_ڈیٹا_2, c_قدم, d_buf, d_buf_قدم, d_ڈیٹا, d_قدم, d_سائز, alpha, beta, flags);
}


ساکن خالی GEMMStore_32fc( ثابت Complexf* c_ڈیٹا_2, size_t c_قدم,
                          ثابت Complexd* d_buf, size_t d_buf_قدم,
                          Complexf* d_ڈیٹا, size_t d_قدم, سائز d_سائز,
                          دوہرا alpha, دوہرا beta, صحیح flags )
{
    GEMMStore(c_ڈیٹا_2, c_قدم, d_buf, d_buf_قدم, d_ڈیٹا, d_قدم, d_سائز, alpha, beta, flags);
}


ساکن خالی GEMMStore_64fc( ثابت Complexd* c_ڈیٹا_2, size_t c_قدم,
                          ثابت Complexd* d_buf, size_t d_buf_قدم,
                          Complexd* d_ڈیٹا, size_t d_قدم, سائز d_سائز,
                          دوہرا alpha, دوہرا beta, صحیح flags )
{
    GEMMStore(c_ڈیٹا_2, c_قدم, d_buf, d_buf_قدم, d_ڈیٹا, d_قدم, d_سائز, alpha, beta, flags);
}

ساکن خالی gemmImpl( Mat A, Mat B, دوہرا alpha,
           Mat C, دوہرا beta, Mat D, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح بلاک_lin_سائز = 128;
    ثابت صحیح بلاک_سائز_3 = بلاک_lin_سائز * بلاک_lin_سائز;

    ساکن دوہرا zero[] = {0,0,0,0};
    ساکن اعشاری zerof[] = {0,0,0,0};

    سائز a_سائز = A.size(), d_سائز;
    صحیح i, len = 0, type = A.type();

    انتخاب( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالت 0:
        d_سائز = سائز( B.ستون_تعداد, a_سائز.اونچائی );
        len = B.صفیں;
        توڑو;
    حالت 1:
        d_سائز = سائز( B.ستون_تعداد, a_سائز.چوڑائی );
        len = B.صفیں;
        توڑو;
    حالت 2:
        d_سائز = سائز( B.صفیں, a_سائز.اونچائی );
        len = B.ستون_تعداد;
        توڑو;
    حالت 3:
        d_سائز = سائز( B.صفیں, a_سائز.چوڑائی );
        len = B.ستون_تعداد;
        توڑو;
    }

    اگر( flags == 0 && 2 <= len && len <= 4 && (len == d_سائز.چوڑائی || len == d_سائز.اونچائی) )
    {
        اگر( type == CV_32F )
        {
            اعشاری* d = D.ptr<اعشاری>();
            ثابت اعشاری *a = A.ptr<اعشاری>(),
                        *b = B.ptr<اعشاری>(),
                        *c = (ثابت اعشاری*)C.ڈیٹا;
            size_t d_قدم = D.قدم/حجم(d[0]),
                a_قدم = A.قدم/حجم(a[0]),
                b_قدم = B.قدم/حجم(b[0]),
                c_قدم = C.ڈیٹا ? C.قدم/حجم(c[0]) : 0;

            اگر( !c )
                c = zerof;

            انتخاب( len )
            {
            حالت 2:
                اگر( len == d_سائز.چوڑائی && b != d )
                {
                    برائے( i = 0; i < d_سائز.اونچائی; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_قدم];
                        اعشاری t1 = a[0]*b[1] + a[1]*b[b_قدم+1];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[1] = (اعشاری)(t1*alpha + c[1]*beta);
                    }
                }
                ورنہ اگر( a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zerof )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    برائے( i = 0; i < d_سائز.چوڑائی; i++, d++, b++, c += c_step0 )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_قدم];
                        اعشاری t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[d_قدم] = (اعشاری)(t1*alpha + c[c_قدم]*beta);
                    }
                }
                ورنہ
                    توڑو;
                واپس;
            حالت 3:
                اگر( len == d_سائز.چوڑائی && b != d )
                {
                    برائے( i = 0; i < d_سائز.اونچائی; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2];
                        اعشاری t1 = a[0]*b[1] + a[1]*b[b_قدم+1] + a[2]*b[b_قدم*2+1];
                        اعشاری t2 = a[0]*b[2] + a[1]*b[b_قدم+2] + a[2]*b[b_قدم*2+2];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[1] = (اعشاری)(t1*alpha + c[1]*beta);
                        d[2] = (اعشاری)(t2*alpha + c[2]*beta);
                    }
                }
                ورنہ اگر( a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zerof )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    برائے( i = 0; i < d_سائز.چوڑائی; i++, d++, b++, c += c_step0 )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2];
                        اعشاری t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم] + a[a_قدم+2]*b[b_قدم*2];
                        اعشاری t2 = a[a_قدم*2]*b[0] + a[a_قدم*2+1]*b[b_قدم] + a[a_قدم*2+2]*b[b_قدم*2];

                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[d_قدم] = (اعشاری)(t1*alpha + c[c_قدم]*beta);
                        d[d_قدم*2] = (اعشاری)(t2*alpha + c[c_قدم*2]*beta);
                    }
                }
                ورنہ
                    توڑو;
                واپس;
            حالت 4:
                اگر( len == d_سائز.چوڑائی && b != d )
                {
                    برائے( i = 0; i < d_سائز.اونچائی; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2] + a[3]*b[b_قدم*3];
                        اعشاری t1 = a[0]*b[1] + a[1]*b[b_قدم+1] + a[2]*b[b_قدم*2+1] + a[3]*b[b_قدم*3+1];
                        اعشاری t2 = a[0]*b[2] + a[1]*b[b_قدم+2] + a[2]*b[b_قدم*2+2] + a[3]*b[b_قدم*3+2];
                        اعشاری t3 = a[0]*b[3] + a[1]*b[b_قدم+3] + a[2]*b[b_قدم*2+3] + a[3]*b[b_قدم*3+3];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[1] = (اعشاری)(t1*alpha + c[1]*beta);
                        d[2] = (اعشاری)(t2*alpha + c[2]*beta);
                        d[3] = (اعشاری)(t3*alpha + c[3]*beta);
                    }
                }
                ورنہ اگر( len <= 16 && a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zerof )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    برائے( i = 0; i < d_سائز.چوڑائی; i++, d++, b++, c += c_step0 )
                    {
                        اعشاری t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2] + a[3]*b[b_قدم*3];
                        اعشاری t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم] +
                                   a[a_قدم+2]*b[b_قدم*2] + a[a_قدم+3]*b[b_قدم*3];
                        اعشاری t2 = a[a_قدم*2]*b[0] + a[a_قدم*2+1]*b[b_قدم] +
                                   a[a_قدم*2+2]*b[b_قدم*2] + a[a_قدم*2+3]*b[b_قدم*3];
                        اعشاری t3 = a[a_قدم*3]*b[0] + a[a_قدم*3+1]*b[b_قدم] +
                                   a[a_قدم*3+2]*b[b_قدم*2] + a[a_قدم*3+3]*b[b_قدم*3];
                        d[0] = (اعشاری)(t0*alpha + c[0]*beta);
                        d[d_قدم] = (اعشاری)(t1*alpha + c[c_قدم]*beta);
                        d[d_قدم*2] = (اعشاری)(t2*alpha + c[c_قدم*2]*beta);
                        d[d_قدم*3] = (اعشاری)(t3*alpha + c[c_قدم*3]*beta);
                    }
                }
                ورنہ
                    توڑو;
                واپس;
            }
        }

        اگر( type == CV_64F )
        {
            دوہرا* d = D.ptr<دوہرا>();
            ثابت دوہرا *a = A.ptr<دوہرا>(),
                         *b = B.ptr<دوہرا>(),
                         *c = (ثابت دوہرا*)C.ڈیٹا;
            size_t d_قدم = D.قدم/حجم(d[0]),
                a_قدم = A.قدم/حجم(a[0]),
                b_قدم = B.قدم/حجم(b[0]),
                c_قدم = C.ڈیٹا ? C.قدم/حجم(c[0]) : 0;
            اگر( !c )
                c = zero;

            انتخاب( len )
            {
            حالت 2:
                اگر( len == d_سائز.چوڑائی && b != d )
                {
                    برائے( i = 0; i < d_سائز.اونچائی; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        دوہرا t0 = a[0]*b[0] + a[1]*b[b_قدم];
                        دوہرا t1 = a[0]*b[1] + a[1]*b[b_قدم+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                ورنہ اگر( a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zero )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    برائے( i = 0; i < d_سائز.چوڑائی; i++, d++, b++, c += c_step0 )
                    {
                        دوہرا t0 = a[0]*b[0] + a[1]*b[b_قدم];
                        دوہرا t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_قدم] = t1*alpha + c[c_قدم]*beta;
                    }
                }
                ورنہ
                    توڑو;
                واپس;
            حالت 3:
                اگر( len == d_سائز.چوڑائی && b != d )
                {
                    برائے( i = 0; i < d_سائز.اونچائی; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        دوہرا t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2];
                        دوہرا t1 = a[0]*b[1] + a[1]*b[b_قدم+1] + a[2]*b[b_قدم*2+1];
                        دوہرا t2 = a[0]*b[2] + a[1]*b[b_قدم+2] + a[2]*b[b_قدم*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                ورنہ اگر( a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zero )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    برائے( i = 0; i < d_سائز.چوڑائی; i++, d++, b++, c += c_step0 )
                    {
                        دوہرا t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2];
                        دوہرا t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم] + a[a_قدم+2]*b[b_قدم*2];
                        دوہرا t2 = a[a_قدم*2]*b[0] + a[a_قدم*2+1]*b[b_قدم] + a[a_قدم*2+2]*b[b_قدم*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_قدم] = t1*alpha + c[c_قدم]*beta;
                        d[d_قدم*2] = t2*alpha + c[c_قدم*2]*beta;
                    }
                }
                ورنہ
                    توڑو;
                واپس;
            حالت 4:
                اگر( len == d_سائز.چوڑائی && b != d )
                {
                    برائے( i = 0; i < d_سائز.اونچائی; i++, d += d_قدم, a += a_قدم, c += c_قدم )
                    {
                        دوہرا t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2] + a[3]*b[b_قدم*3];
                        دوہرا t1 = a[0]*b[1] + a[1]*b[b_قدم+1] + a[2]*b[b_قدم*2+1] + a[3]*b[b_قدم*3+1];
                        دوہرا t2 = a[0]*b[2] + a[1]*b[b_قدم+2] + a[2]*b[b_قدم*2+2] + a[3]*b[b_قدم*3+2];
                        دوہرا t3 = a[0]*b[3] + a[1]*b[b_قدم+3] + a[2]*b[b_قدم*2+3] + a[3]*b[b_قدم*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                ورنہ اگر( d_سائز.چوڑائی <= 16 && a != d )
                {
                    صحیح c_step0 = 1;
                    اگر( c == zero )
                    {
                        c_step0 = 0;
                        c_قدم = 1;
                    }

                    برائے( i = 0; i < d_سائز.چوڑائی; i++, d++, b++, c += c_step0 )
                    {
                        دوہرا t0 = a[0]*b[0] + a[1]*b[b_قدم] + a[2]*b[b_قدم*2] + a[3]*b[b_قدم*3];
                        دوہرا t1 = a[a_قدم]*b[0] + a[a_قدم+1]*b[b_قدم] +
                                    a[a_قدم+2]*b[b_قدم*2] + a[a_قدم+3]*b[b_قدم*3];
                        دوہرا t2 = a[a_قدم*2]*b[0] + a[a_قدم*2+1]*b[b_قدم] +
                                    a[a_قدم*2+2]*b[b_قدم*2] + a[a_قدم*2+3]*b[b_قدم*3];
                        دوہرا t3 = a[a_قدم*3]*b[0] + a[a_قدم*3+1]*b[b_قدم] +
                                    a[a_قدم*3+2]*b[b_قدم*2] + a[a_قدم*3+3]*b[b_قدم*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_قدم] = t1*alpha + c[c_قدم]*beta;
                        d[d_قدم*2] = t2*alpha + c[c_قدم*2]*beta;
                        d[d_قدم*3] = t3*alpha + c[c_قدم*3]*beta;
                    }
                }
                ورنہ
                    توڑو;
                واپس;
            }
        }
    }

    {
    size_t b_قدم = B.قدم;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc بلاک_Mul_Func;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ثابت uchar* Cdata = C.ڈیٹا;
    size_t Cstep = C.ڈیٹا ? (size_t)C.قدم : 0;
    Auto_بفر<uchar> buf;

    اگر( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<اعشاری, دوہرا, GEMMSingleMul_32f>;
        بلاک_Mul_Func = gemm_بلاک_Mul_Wrap<اعشاری, دوہرا, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<اعشاری, دوہرا, GEMMStore_32f>;
    }
    ورنہ اگر( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<دوہرا, دوہرا, GEMMSingleMul_64f>;
        بلاک_Mul_Func = gemm_بلاک_Mul_Wrap<دوہرا, دوہرا, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<دوہرا, دوہرا, GEMMStore_64f>;
    }
    ورنہ اگر( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        بلاک_Mul_Func = gemm_بلاک_Mul_Wrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    ورنہ
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        بلاک_Mul_Func = gemm_بلاک_Mul_Wrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    اگر( (d_سائز.چوڑائی == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_قدم = d_سائز.چوڑائی == 1 ? 0 : CV_ELEM_سائز(type);
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
    else*/ اگر( ((d_سائز.اونچائی <= بلاک_lin_سائز/2 || d_سائز.چوڑائی <= بلاک_lin_سائز/2) &&
        len <= 10000) || len <= 10 ||
        (d_سائز.چوڑائی <= بلاک_lin_سائز &&
        d_سائز.اونچائی <= بلاک_lin_سائز && len <= بلاک_lin_سائز) )
    {
        singleMulFunc( A.ptr(), A.قدم, B.ptr(), b_قدم, Cdata, Cstep,
                       matD->ptr(), matD->قدم, a_سائز, d_سائز, alpha, beta, flags );
    }
    ورنہ
    {
        صحیح is_a_t = flags & GEMM_1_T;
        صحیح is_b_t = flags & GEMM_2_T;
        صحیح elem_سائز_2 = CV_ELEM_سائز(type);
        صحیح dk0_1, dk0_2;
        size_t a_buf_سائز = 0, b_buf_سائز, d_buf_سائز;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        صحیح j, k, di = 0, dj = 0, dk = 0;
        صحیح dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        صحیح work_elem_سائز = elem_سائز_2 << (CV_MAT_گہرائی(type) == CV_32F ? 1 : 0);

        اگر( !is_a_t )
            a_step0 = A.قدم, a_step1 = elem_سائز_2;
        ورنہ
            a_step0 = elem_سائز_2, a_step1 = A.قدم;

        اگر( !is_b_t )
            b_step0 = b_قدم, b_step1 = elem_سائز_2;
        ورنہ
            b_step0 = elem_سائز_2, b_step1 = b_قدم;

        اگر( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        ورنہ اگر( !(flags & GEMM_3_T) )
            c_step0 = C.قدم, c_step1 = elem_سائز_2;
        ورنہ
            c_step0 = elem_سائز_2, c_step1 = C.قدم;

        dm0 = std::min( بلاک_lin_سائز, d_سائز.اونچائی );
        dn0 = std::min( بلاک_lin_سائز, d_سائز.چوڑائی );
        dk0_1 = بلاک_سائز_3 / dm0;
        dk0_2 = بلاک_سائز_3 / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        اگر( dk0*dm0 > بلاک_سائز_3 )
            dm0 = بلاک_سائز_3 / dk0;
        اگر( dk0*dn0 > بلاک_سائز_3 )
            dn0 = بلاک_سائز_3 / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_سائز = (size_t)(dk0+dk0/8+1)*dk0_1*elem_سائز_2;
        d_buf_سائز = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_سائز;

        اگر( is_a_t )
        {
            a_buf_سائز = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_سائز_2;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_سائز + b_buf_سائز + a_buf_سائز);
        d_buf = buf.ڈیٹا();
        b_buf = d_buf + d_buf_سائز;

        اگر( is_a_t )
            a_buf = b_buf + b_buf_سائز;

        برائے( i = 0; i < d_سائز.اونچائی; i += di )
        {
            di = dm0;
            اگر( i + di >= d_سائز.اونچائی || 8*(i + di) + di > 8*d_سائز.اونچائی )
                di = d_سائز.اونچائی - i;

            برائے( j = 0; j < d_سائز.چوڑائی; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->قدم + j*elem_سائز_2;
                ثابت uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t d_قدم_2 = matD->قدم;
                dj = dn0;

                اگر( j + dj >= d_سائز.چوڑائی || 8*(j + dj) + dj > 8*d_سائز.چوڑائی )
                    dj = d_سائز.چوڑائی - j;

                flags &= 15;
                اگر( dk0 < len )
                {
                    _d = d_buf;
                    d_قدم_2 = dj*work_elem_سائز;
                }

                برائے( k = 0; k < len; k += dk )
                {
                    ثابت uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t a_قدم_2 = A.قدم;
                    ثابت uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t b_قدم_2 = b_قدم;
                    سائز a_bl_سائز;

                    dk = dk0;
                    اگر( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    اگر( !is_a_t )
                        a_bl_سائز.چوڑائی = dk, a_bl_سائز.اونچائی = di;
                    ورنہ
                        a_bl_سائز.چوڑائی = di, a_bl_سائز.اونچائی = dk;

                    اگر( a_buf && is_a_t )
                    {
                        a_قدم_2 = dk*elem_سائز_2;
                        GEMM_Transpose_بلاک( _a, A.قدم, a_buf, a_قدم_2, a_bl_سائز, elem_سائز_2 );
                        std::swap( a_bl_سائز.چوڑائی, a_bl_سائز.اونچائی );
                        _a = a_buf;
                    }

                    اگر( dj < d_سائز.چوڑائی )
                    {
                        سائز b_سائز;
                        اگر( !is_b_t )
                            b_سائز.چوڑائی = dj, b_سائز.اونچائی = dk;
                        ورنہ
                            b_سائز.چوڑائی = dk, b_سائز.اونچائی = dj;

                        b_قدم_2 = b_سائز.چوڑائی*elem_سائز_2;
                        GEMM_Copy_بلاک( _b, b_قدم, b_buf, b_قدم_2, b_سائز, elem_سائز_2 );
                        _b = b_buf;
                    }

                    اگر( dk0 < len )
                        بلاک_Mul_Func( _a, a_قدم_2, _b, b_قدم_2, _d, d_قدم_2,
                                      a_bl_سائز, سائز(dj,di), flags );
                    ورنہ
                        singleMulFunc( _a, a_قدم_2, _b, b_قدم_2, _c, Cstep,
                                       _d, d_قدم_2, a_bl_سائز, سائز(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                اگر( dk0 < len )
                    storeFunc( _c, Cstep, _d, d_قدم_2,
                               matD->ptr(i) + j*elem_سائز_2,
                               matD->قدم, سائز(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

سانچہ <قسم_نام fptype>اندرونی ساکن خالی
callGemmImpl(ثابت fptype *src1, size_t src1_قدم, ثابت fptype *src2, size_t src2_قدم, fptype alpha,
          ثابت fptype *src3, size_t src3_قدم, fptype beta, fptype *dst, size_t dst_قدم, صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags, صحیح type)
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
        ورنہ
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    ورنہ
    {
        b_n = n_d;
        اگر(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        ورنہ
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
    ورنہ
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    اگر(src1 != تہی)
        A = Mat(m_a, n_a, type, (خالی*)src1, src1_قدم);
    اگر(src2 != تہی)
        B = Mat(b_m, b_n, type, (خالی*)src2, src2_قدم);
    اگر(src3 != تہی && beta != 0.0)
        C = Mat(c_m, c_n, type, (خالی*)src3, src3_قدم);
    Mat D(m_d, n_d, type, (خالی*)dst, dst_قدم);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

خالی gemm32f(ثابت اعشاری* src1, size_t src1_قدم, ثابت اعشاری* src2, size_t src2_قدم,
             اعشاری alpha, ثابت اعشاری* src3, size_t src3_قدم, اعشاری beta, اعشاری* dst, size_t dst_قدم,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags, CV_32F);
}

خالی gemm64f(ثابت دوہرا* src1, size_t src1_قدم, ثابت دوہرا* src2, size_t src2_قدم,
             دوہرا alpha, ثابت دوہرا* src3, size_t src3_قدم, دوہرا beta, دوہرا* dst, size_t dst_قدم,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags, CV_64F);
}

خالی gemm32fc(ثابت اعشاری* src1, size_t src1_قدم, ثابت اعشاری* src2, size_t src2_قدم,
              اعشاری alpha, ثابت اعشاری* src3, size_t src3_قدم, اعشاری beta, اعشاری* dst, size_t dst_قدم,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags, CV_32FC2);
}

خالی gemm64fc(ثابت دوہرا* src1, size_t src1_قدم, ثابت دوہرا* src2, size_t src2_قدم,
              دوہرا alpha, ثابت دوہرا* src3, size_t src3_قدم, دوہرا beta, دوہرا* dst, size_t dst_قدم,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags, CV_64FC2);
}

#اگر_ختم // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

سانچہ<قسم_نام T, قسم_نام WT> ساکن خالی
transform_( ثابت T* src, T* dst, ثابت WT* m, صحیح len, صحیح scn, صحیح dcn )
{
    صحیح x;

    اگر( scn == 2 && dcn == 2 )
    {
        برائے( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ورنہ اگر( scn == 3 && dcn == 3 )
    {
        برائے( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ورنہ اگر( scn == 3 && dcn == 1 )
    {
        برائے( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    ورنہ اگر( scn == 4 && dcn == 4 )
    {
        برائے( x = 0; x < len*4; x += 4 )
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
    ورنہ
    {
        برائے( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ثابت WT* _m = m;
            صحیح j, k;
            برائے( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                برائے( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

ساکن خالی
transform_8u( ثابت uchar* src, uchar* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn )
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح بٹ = 10, SCALE = 1 << بٹ;
    ثابت اعشاری MAX_M = (اعشاری)(1 << (15 - بٹ));

    اگر( scn == 3 && dcn == 3 &&
        std::مطلق_قدر(m[0]) < MAX_M && std::مطلق_قدر(m[1]) < MAX_M && std::مطلق_قدر(m[ 2]) < MAX_M*256 && std::مطلق_قدر(m[ 3]) < MAX_M*256 &&
        std::مطلق_قدر(m[4]) < MAX_M && std::مطلق_قدر(m[5]) < MAX_M && std::مطلق_قدر(m[ 6]) < MAX_M*256 && std::مطلق_قدر(m[ 7]) < MAX_M*256 &&
        std::مطلق_قدر(m[8]) < MAX_M && std::مطلق_قدر(m[9]) < MAX_M && std::مطلق_قدر(m[10]) < MAX_M*256 && std::مطلق_قدر(m[11]) < MAX_M*256 )
    {
        ثابت صحیح nChannels = 3;

        اتحاد {
            مختصر s[6];
            صحیح p[3];
        } m16;
        m16.s[0] = saturate_cast<مختصر>(m[0] * SCALE); m16.s[1] = saturate_cast<مختصر>(m[1] * SCALE);
        m16.s[2] = saturate_cast<مختصر>(m[4] * SCALE); m16.s[3] = saturate_cast<مختصر>(m[5] * SCALE);
        m16.s[4] = saturate_cast<مختصر>(m[8] * SCALE); m16.s[5] = saturate_cast<مختصر>(m[9] * SCALE);
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
        برائے (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
        {
            v_uint8 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
            v_uint8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_uint16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_uint16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_جوڑو(v_جوڑو(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack_u(dbl, dbh), v_pack_u(dgl, dgh), v_pack_u(drl, drh));
        }
        m32[1] = saturate_cast<صحیح>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<صحیح>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<صحیح>((m[11] + 0.5f)*SCALE);
        برائے( ; x < len * nChannels; x += nChannels )
        {
            صحیح v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> بٹ);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> بٹ);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> بٹ);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    transform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
transform_16u( ثابت ushort* src, ushort* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn )
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر( scn == 3 && dcn == 3 )
    {
        صحیح x = 0;
#اگر CV_SIMD_چوڑائی > 16
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
        برائے (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
        {
            v_uint16 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
            v_uint32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_جوڑو_wrap(v_pack(v_باری(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m2,  m3)))),
                                   v_باری(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m2,  m3))))), delta);
            dg = v_جوڑو_wrap(v_pack(v_باری(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m6,  m7)))),
                                   v_باری(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m6,  m7))))), delta);
            dr = v_جوڑو_wrap(v_pack(v_باری(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)), m10, m11)))),
                                   v_باری(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)), m10, m11))))), delta);
            v_store_interleave(dst + x, v_reinterpret_as_u16(db), v_reinterpret_as_u16(dg), v_reinterpret_as_u16(dr));
        }
#اگر_ختم
#اگر CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_بایاں<1>(_m0l);
        v_float32x4 _m1h = v_rotate_بایاں<1>(_m1l);
        v_float32x4 _m2h = v_rotate_بایاں<1>(_m2l);
        v_float32x4 _m3h = v_rotate_بایاں<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        برائے( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_دایاں<1>(v_reinterpret_as_u16(v_جوڑو_wrap(v_pack(
                             v_باری(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_بھرو_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_باری(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_بھرو_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#اگر_ختم //CV_SIMD128
        برائے( ; x < len * 3; x += 3 )
        {
            اعشاری v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    transform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
transform_32f( ثابت اعشاری* src, اعشاری* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#اگر (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    صحیح x = 0;
    اگر( scn == 3 && dcn == 3 )
    {
        صحیح idx[VTraits<v_float32>::max_nlanes/2];
        برائے( صحیح i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
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
        برائے( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        برائے( ; x < len*3; x += 3 )
        {
            اعشاری v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            اعشاری t0 = saturate_cast<اعشاری>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            اعشاری t1 = saturate_cast<اعشاری>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            اعشاری t2 = saturate_cast<اعشاری>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        واپس;
    }

    اگر( scn == 4 && dcn == 4 )
    {
#اگر CV_SIMD_چوڑائی > 16
        صحیح idx[VTraits<v_float32>::max_nlanes/4];
        برائے( صحیح i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        اعشاری _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        برائے( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_بھرو(src + x);
            v_store(dst + x, v_جوڑو(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#اگر_ختم
#اگر CV_SIMD128
        v_float32x4 _m0 = v_بھرو(m     );
        v_float32x4 _m1 = v_بھرو(m +  5);
        v_float32x4 _m2 = v_بھرو(m + 10);
        v_float32x4 _m3 = v_بھرو(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        برائے( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_بھرو(src + x);
            v_store(dst + x, v_جوڑو(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#ورنہ // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        برائے( ; x < len*4; x += 4 )
        {
            اعشاری v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            اعشاری t0 = saturate_cast<اعشاری>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            اعشاری t1 = saturate_cast<اعشاری>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            اعشاری t2 = saturate_cast<اعشاری>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            اعشاری t3 = saturate_cast<اعشاری>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#اگر_ختم
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    transform_(src, dst, m, len, scn, dcn);
}


ساکن خالی
transform_8s(ثابت schar* src, schar* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح بٹ = 10, SCALE = 1 << بٹ;
    ثابت اعشاری MAX_M = (اعشاری)(1 << (15 - بٹ));

    اگر( scn == 3 && dcn == 3 &&
        std::مطلق_قدر(m[0]) < MAX_M && std::مطلق_قدر(m[1]) < MAX_M && std::مطلق_قدر(m[ 2]) < MAX_M*256 && std::مطلق_قدر(m[ 3]) < MAX_M*256 &&
        std::مطلق_قدر(m[4]) < MAX_M && std::مطلق_قدر(m[5]) < MAX_M && std::مطلق_قدر(m[ 6]) < MAX_M*256 && std::مطلق_قدر(m[ 7]) < MAX_M*256 &&
        std::مطلق_قدر(m[8]) < MAX_M && std::مطلق_قدر(m[9]) < MAX_M && std::مطلق_قدر(m[10]) < MAX_M*256 && std::مطلق_قدر(m[11]) < MAX_M*256 )
    {
        ثابت صحیح nChannels = 3;

        اتحاد {
            مختصر s[6];
            صحیح p[3];
        } m16;
        m16.s[0] = saturate_cast<مختصر>(m[0] * SCALE); m16.s[1] = saturate_cast<مختصر>(m[1] * SCALE);
        m16.s[2] = saturate_cast<مختصر>(m[4] * SCALE); m16.s[3] = saturate_cast<مختصر>(m[5] * SCALE);
        m16.s[4] = saturate_cast<مختصر>(m[8] * SCALE); m16.s[5] = saturate_cast<مختصر>(m[9] * SCALE);
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
        برائے (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
        {
            v_int8 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
            v_int8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_int16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_int16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_جوڑو(v_جوڑو(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_جوڑو(v_جوڑو(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_جوڑو(v_جوڑو(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_جوڑو(v_جوڑو(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_جوڑو(v_جوڑو(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<بٹ>(v_جوڑو(v_جوڑو(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_جوڑو(v_جوڑو(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack(dbl, dbh), v_pack(dgl, dgh), v_pack(drl, drh));
        }
        m32[1] = saturate_cast<صحیح>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<صحیح>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<صحیح>((m[11] + 0.5f)*SCALE);
        برائے( ; x < len * nChannels; x += nChannels )
        {
            صحیح v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> بٹ);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> بٹ);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> بٹ);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    transform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
transform_16s(ثابت مختصر* src, مختصر* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
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
        برائے (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
        {
            v_int16 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
            v_int32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_pack(v_باری(v_muladd(v_cvt_f32(bl), m0, v_muladd(v_cvt_f32(gl), m1, v_muladd(v_cvt_f32(rl),  m2,  m3)))),
                        v_باری(v_muladd(v_cvt_f32(bh), m0, v_muladd(v_cvt_f32(gh), m1, v_muladd(v_cvt_f32(rh),  m2,  m3)))));
            dg = v_pack(v_باری(v_muladd(v_cvt_f32(bl), m4, v_muladd(v_cvt_f32(gl), m5, v_muladd(v_cvt_f32(rl),  m6,  m7)))),
                        v_باری(v_muladd(v_cvt_f32(bh), m4, v_muladd(v_cvt_f32(gh), m5, v_muladd(v_cvt_f32(rh),  m6,  m7)))));
            dr = v_pack(v_باری(v_muladd(v_cvt_f32(bl), m8, v_muladd(v_cvt_f32(gl), m9, v_muladd(v_cvt_f32(rl), m10, m11)))),
                        v_باری(v_muladd(v_cvt_f32(bh), m8, v_muladd(v_cvt_f32(gh), m9, v_muladd(v_cvt_f32(rh), m10, m11)))));
            v_store_interleave(dst + x, db, dg, dr);
        }
        برائے( ; x < len * 3; x += 3 )
        {
            اعشاری v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            مختصر t0 = saturate_cast<مختصر>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            مختصر t1 = saturate_cast<مختصر>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            مختصر t2 = saturate_cast<مختصر>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    transform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
transform_32s(ثابت صحیح* src, صحیح* dst, ثابت دوہرا* m, صحیح len, صحیح scn, صحیح dcn)
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
        برائے (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_بلند(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_بلند(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_بلند(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_باری(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_باری(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_باری(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        برائے (; x < len * 3; x += 3) {
            دوہرا b = src[x], g = src[x + 1], r = src[x + 2];
            صحیح db = saturate_cast<صحیح>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            صحیح dg = saturate_cast<صحیح>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            صحیح dr = saturate_cast<صحیح>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    transform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
transform_64f(ثابت دوہرا* src, دوہرا* dst, ثابت دوہرا* m, صحیح len, صحیح scn, صحیح dcn)
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
        برائے (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        برائے (; x < len * 3; x += 3) {
            دوہرا b = src[x], g = src[x + 1], r = src[x + 2];
            دوہرا db = saturate_cast<دوہرا>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            دوہرا dg = saturate_cast<دوہرا>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            دوہرا dr = saturate_cast<دوہرا>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    transform_(src, dst, m, len, scn, dcn);
}

سانچہ<قسم_نام T, قسم_نام WT> ساکن خالی
diagtransform_( ثابت T* src, T* dst, ثابت WT* m, صحیح len, صحیح cn, صحیح )
{
    صحیح x;

    اگر( cn == 2 )
    {
        برائے( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    ورنہ اگر( cn == 3 )
    {
        برائے( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    ورنہ اگر( cn == 4 )
    {
        برائے( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    ورنہ
    {
        برائے( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ثابت WT* _m = m;
            برائے( صحیح j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

ساکن خالی
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
        برائے (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
            v_uint8 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack_u(v_باری(db_f32_l0), v_باری(db_f32_l1)), v_pack_u(v_باری(db_f32_h0), v_باری(db_f32_h1))),
                               v_pack(v_pack_u(v_باری(dg_f32_l0), v_باری(dg_f32_l1)), v_pack_u(v_باری(dg_f32_h0), v_باری(dg_f32_h1))),
                               v_pack(v_pack_u(v_باری(dr_f32_l0), v_باری(dr_f32_l1)), v_pack_u(v_باری(dr_f32_h0), v_باری(dr_f32_h1))));
        }
        برائے (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
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
        برائے (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
            v_int8 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack(v_باری(db_f32_l0), v_باری(db_f32_l1)), v_pack(v_باری(db_f32_h0), v_باری(db_f32_h1))),
                               v_pack(v_pack(v_باری(dg_f32_l0), v_باری(dg_f32_l1)), v_pack(v_باری(dg_f32_h0), v_باری(dg_f32_h1))),
                               v_pack(v_pack(v_باری(dr_f32_l0), v_باری(dr_f32_l1)), v_pack(v_باری(dr_f32_h0), v_باری(dr_f32_h1))));
        }
        برائے (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
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
        برائے (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
            v_uint16 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
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
                               v_pack_u(v_باری(db_f32_l), v_باری(db_f32_h)),
                               v_pack_u(v_باری(dg_f32_l), v_باری(dg_f32_h)),
                               v_pack_u(v_باری(dr_f32_l), v_باری(dr_f32_h)));
        }
        برائے (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
diagtransform_16s(ثابت مختصر* src, مختصر* dst, ثابت اعشاری* m, صحیح len, صحیح scn, صحیح dcn)
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
        برائے (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
            v_int16 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
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
                               v_pack(v_باری(db_f32_l), v_باری(db_f32_h)),
                               v_pack(v_باری(dg_f32_l), v_باری(dg_f32_h)),
                               v_pack(v_باری(dr_f32_l), v_باری(dr_f32_h)));
        }
        برائے (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            مختصر db = saturate_cast<مختصر>(m[ 0] * b + m[ 3]);
            مختصر dg = saturate_cast<مختصر>(m[ 5] * g + m[ 7]);
            مختصر dr = saturate_cast<مختصر>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
diagtransform_32s(ثابت صحیح* src, صحیح* dst, ثابت دوہرا* m, صحیح len, صحیح scn, صحیح dcn)
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
        برائے (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_بلند(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_بلند(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_بلند(r), rl = v_cvt_f64(r);

            v_int32 db = v_باری(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_باری(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_باری(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        برائے (; x < len * 3; x += 3) {
            صحیح b = src[x], g = src[x + 1], r = src[x + 2];
            صحیح db = saturate_cast<صحیح>(m[ 0] * b + m[ 3]);
            صحیح dg = saturate_cast<صحیح>(m[ 5] * g + m[ 7]);
            صحیح dr = saturate_cast<صحیح>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
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
        برائے (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        برائے (; x < len * 3; x += 3) {
            اعشاری b = src[x], g = src[x + 1], r = src[x + 2];
            اعشاری db = saturate_cast<اعشاری>(m[ 0] * b + m[ 3]);
            اعشاری dg = saturate_cast<اعشاری>(m[ 5] * g + m[ 7]);
            اعشاری dr = saturate_cast<اعشاری>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    diagtransform_(src, dst, m, len, scn, dcn);
}

ساکن خالی
diagtransform_64f(ثابت دوہرا* src, دوہرا* dst, ثابت دوہرا* m, صحیح len, صحیح scn, صحیح dcn)
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
        برائے (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_بھرو_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        برائے (; x < len * 3; x += 3) {
            دوہرا b = src[x], g = src[x + 1], r = src[x + 2];
            دوہرا db = saturate_cast<دوہرا>(m[ 0] * b + m[ 3]);
            دوہرا dg = saturate_cast<دوہرا>(m[ 5] * g + m[ 7]);
            دوہرا dr = saturate_cast<دوہرا>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        واپس;
    }
#اگر_ختم

    diagtransform_(src, dst, m, len, scn, dcn);
}


سانچہ<قسم_نام T, قسم_نام WT, خالی (*fn)(ثابت T*, T*, ثابت WT*, صحیح, صحیح, صحیح)>
ساکن خالی transformWrap(ثابت خالی* src, خالی* dst, ثابت خالی* m, صحیح len, صحیح scn, صحیح dcn)
{
    fn((ثابت T*)src, (T*)dst, (ثابت WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(صحیح گہرائی)
{
    ساکن TransformFunc transformTab[CV_گہرائی_MAX] =
    {
        transformWrap<uchar, اعشاری, transform_8u>, transformWrap<schar, اعشاری, transform_8s>, transformWrap<ushort, اعشاری, transform_16u>,
        transformWrap<مختصر, اعشاری, transform_16s>, transformWrap<صحیح, دوہرا, transform_32s>, transformWrap<اعشاری, اعشاری, transform_32f>,
        transformWrap<دوہرا, دوہرا, transform_64f>, 0
    };

    واپس transformTab[گہرائی];
}

TransformFunc getDiagTransformFunc(صحیح گہرائی)
{
    ساکن TransformFunc diagTransformTab[CV_گہرائی_MAX] =
    {
        transformWrap<uchar, اعشاری, diagtransform_8u>, transformWrap<schar, اعشاری, diagtransform_8s>, transformWrap<ushort, اعشاری, diagtransform_16u>,
        transformWrap<مختصر, اعشاری, diagtransform_16s>, transformWrap<صحیح, دوہرا, diagtransform_32s>, transformWrap<اعشاری, اعشاری, diagtransform_32f>,
        transformWrap<دوہرا, دوہرا, diagtransform_64f>, 0
    };

    واپس diagTransformTab[گہرائی];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

سانچہ<قسم_نام T> ساکن خالی
perspectiveTransform_( ثابت خالی* _src, خالی* _dst, ثابت خالی* _m_ptr, صحیح len, صحیح scn, صحیح dcn )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    ثابت دوہرا* m = (ثابت دوہرا*)_m_ptr;
    ثابت دوہرا eps = FLT_EPSILON;
    صحیح i;

    اگر( scn == 2 && dcn == 2 )
    {
        برائے( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            دوہرا w = x*m[6] + y*m[7] + m[8];

            اگر( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            ورنہ
                dst[i] = dst[i+1] = (T)0;
        }
    }
    ورنہ اگر( scn == 3 && dcn == 3 )
    {
        برائے( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            دوہرا w = x*m[12] + y*m[13] + z*m[14] + m[15];

            اگر( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            ورنہ
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    ورنہ اگر( scn == 3 && dcn == 2 )
    {
        برائے( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            دوہرا w = x*m[8] + y*m[9] + z*m[10] + m[11];

            اگر( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            ورنہ
                dst[0] = dst[1] = (T)0;
        }
    }
    ورنہ
    {
        برائے( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ثابت دوہرا* _m = m + dcn*(scn + 1);
            دوہرا w = _m[scn];
            صحیح j, k;
            برائے( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            اگر( fabs(w) > eps )
            {
                _m = m;
                برائے( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    دوہرا s = _m[scn];
                    برائے( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            ورنہ
                برائے( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(صحیح گہرائی)
{
    اگر (گہرائی == CV_32F)
        واپس perspectiveTransform_<اعشاری>;
    اگر (گہرائی == CV_64F)
        واپس perspectiveTransform_<دوہرا>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

ساکن خالی scale_جوڑو_32f(ثابت خالی* _src1, ثابت خالی* _src2, خالی* _dst,
                         صحیح len, ثابت خالی* _alpha)
{
    ثابت اعشاری* src1 = (ثابت اعشاری*)_src1;
    ثابت اعشاری* src2 = (ثابت اعشاری*)_src2;
    اعشاری* dst = (اعشاری*)_dst;
    اعشاری alpha = *(ثابت اعشاری*)_alpha;
    صحیح i = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ثابت صحیح c_چوڑائی = VTraits<v_float32>::vlanes();
    برائے (; i <= len - c_چوڑائی; i += c_چوڑائی)
        v_store(dst + i, v_muladd(vx_بھرو(src1 + i), v_alpha, vx_بھرو(src2 + i)));
    vx_cleanup();
#اگر_ختم
    برائے (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


ساکن خالی scale_جوڑو_64f(ثابت خالی* _src1, ثابت خالی* _src2, خالی* _dst,
                         صحیح len, ثابت خالی* _alpha)
{
    ثابت دوہرا* src1 = (ثابت دوہرا*)_src1;
    ثابت دوہرا* src2 = (ثابت دوہرا*)_src2;
    دوہرا* dst = (دوہرا*)_dst;
    دوہرا alpha = *(ثابت دوہرا*)_alpha;
    صحیح i = 0;
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ثابت صحیح c_چوڑائی = VTraits<v_float64>::vlanes();
    برائے (; i <= len - c_چوڑائی; i += c_چوڑائی)
        v_store(dst + i, v_muladd(vx_بھرو(src1 + i), a2, vx_بھرو(src2 + i)));
    vx_cleanup();
#اگر_ختم
    برائے (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

Scale_جوڑو_Func get_Scale_جوڑو_Func(صحیح گہرائی)
{
    اگر (گہرائی == CV_32F)
        واپس scale_جوڑو_32f;
    اگر (گہرائی == CV_64F)
        واپس scale_جوڑو_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

سانچہ<قسم_نام T> ساکن اندرونی
دوہرا MahalanobisImpl(ثابت Mat& v1, ثابت Mat& v2, ثابت Mat& icovar, دوہرا *diff_بفر /*[len]*/, صحیح len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    سائز sz = v1.size();
    دوہرا نتیجہ = 0;

    sz.چوڑائی *= v1.channels();
    اگر (v1.isContinuous() && v2.isContinuous())
    {
        sz.چوڑائی *= sz.اونچائی;
        sz.اونچائی = 1;
    }

    {
        ثابت T* src1 = v1.ptr<T>();
        ثابت T* src2 = v2.ptr<T>();
        size_t step1 = v1.قدم/حجم(src1[0]);
        size_t step2 = v2.قدم/حجم(src2[0]);
        دوہرا* diff = diff_بفر;
        ثابت T* mat = icovar.ptr<T>();
        size_t matstep = icovar.قدم/حجم(mat[0]);

        برائے (; sz.اونچائی--; src1 += step1, src2 += step2, diff += sz.چوڑائی)
        {
            برائے (صحیح i = 0; i < sz.چوڑائی; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_بفر;
        برائے (صحیح i = 0; i < len; i++, mat += matstep)
        {
            دوہرا صف_مجموع = 0;
            صحیح j = 0;
#اگر CV_ENABLE_UNROLLED
            برائے(; j <= len - 4; j += 4 )
                صف_مجموع += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#اگر_ختم
            برائے (; j < len; j++)
                صف_مجموع += diff[j]*mat[j];
            نتیجہ += صف_مجموع * diff[i];
        }
    }
    واپس نتیجہ;
}

MahalanobisImplFunc getMahalanobisImplFunc(صحیح گہرائی)
{
    اگر (گہرائی == CV_32F)
        واپس (MahalanobisImplFunc)MahalanobisImpl<اعشاری>;
    اگر (گہرائی == CV_64F)
        واپس (MahalanobisImplFunc)MahalanobisImpl<دوہرا>;
    CV_Assert(0 && "Not supported");
}


#اگر !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_منوال)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

سانچہ<قسم_نام sT, قسم_نام dT> ساکن خالی
MulTransposedR(ثابت Mat& srcmat, ثابت Mat& dstmat, ثابت Mat& deltamat, دوہرا scale)
{
    صحیح i, j, k;
    ثابت sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ثابت dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.قدم/حجم(src[0]);
    size_t dststep = dstmat.قدم/حجم(dst[0]);
    size_t deltastep = deltamat.صفیں > 1 ? deltamat.قدم/حجم(delta[0]) : 0;
    صحیح delta_ستون_تعداد = deltamat.ستون_تعداد;
    سائز size = srcmat.size();
    dT* tdst = dst;
    dT* ستون_buf = 0;
    dT* delta_buf = 0;
    صحیح buf_سائز_2 = size.اونچائی*حجم(dT);
    Auto_بفر<uchar> buf;

    اگر( delta && delta_ستون_تعداد < size.چوڑائی )
    {
        CV_Assert( delta_ستون_تعداد == 1 );
        buf_سائز_2 *= 5;
    }
    buf.allocate(buf_سائز_2);
    ستون_buf = (dT*)buf.ڈیٹا();

    اگر( delta && delta_ستون_تعداد < size.چوڑائی )
    {
        delta_buf = ستون_buf + size.اونچائی;
        برائے( i = 0; i < size.اونچائی; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#اگر CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#اگر_ختم

    اگر( !delta )
        برائے( i = 0; i < size.چوڑائی; i++, tdst += dststep )
        {
            برائے( k = 0; k < size.اونچائی; k++ )
                ستون_buf[k] = src[k*srcstep+i];

            برائے( j = i; j <= size.چوڑائی - 4; j += 4 )
            {
#اگر CV_SIMD128_64F
                اگر (ڈیٹا_Type_2<sT>::گہرائی == CV_64F && ڈیٹا_Type_2<dT>::گہرائی == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ثابت دوہرا *tsrc = (دوہرا*)(src + j);

                    برائے( k = 0; k < size.اونچائی; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((دوہرا)ستون_buf[k]);
                        s0 = v_جوڑو(s0, v_mul(a, v_بھرو(tsrc + 0)));
                        s1 = v_جوڑو(s1, v_mul(a, v_بھرو(tsrc + 2)));
                    }

                    v_store((دوہرا*)(tdst+j), v_mul(s0, v_scale));
                    v_store((دوہرا*)(tdst+j+2), v_mul(s1, v_scale));
                } ورنہ
#اگر_ختم
                {
                    دوہرا s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ثابت sT *tsrc = src + j;

                    برائے( k = 0; k < size.اونچائی; k++, tsrc += srcstep )
                    {
                        دوہرا a = ستون_buf[k];
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

            برائے( ; j < size.چوڑائی; j++ )
            {
                دوہرا s0 = 0;
                ثابت sT *tsrc = src + j;

                برائے( k = 0; k < size.اونچائی; k++, tsrc += srcstep )
                    s0 += (دوہرا)ستون_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    ورنہ
        برائے( i = 0; i < size.چوڑائی; i++, tdst += dststep )
        {
            اگر( !delta_buf )
                برائے( k = 0; k < size.اونچائی; k++ )
                    ستون_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            ورنہ
                برائے( k = 0; k < size.اونچائی; k++ )
                    ستون_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            برائے( j = i; j <= size.چوڑائی - 4; j += 4 )
            {
#اگر CV_SIMD128_64F
                اگر (ڈیٹا_Type_2<sT>::گہرائی == CV_64F && ڈیٹا_Type_2<dT>::گہرائی == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ثابت دوہرا *tsrc = (دوہرا*)(src + j);
                    ثابت دوہرا *d = (دوہرا*)(delta_buf ? delta_buf : delta + j);

                    برائے( k = 0; k < size.اونچائی; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((دوہرا)ستون_buf[k]);
                        s0 = v_جوڑو(s0, v_mul(a, v_sub(v_بھرو(tsrc + 0), v_بھرو(d + 0))));
                        s1 = v_جوڑو(s1, v_mul(a, v_sub(v_بھرو(tsrc + 2), v_بھرو(d + 2))));
                    }

                    v_store((دوہرا*)(tdst+j), v_mul(s0, v_scale));
                    v_store((دوہرا*)(tdst+j+2), v_mul(s1, v_scale));
                }
                ورنہ
#اگر_ختم

                {
                    دوہرا s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ثابت sT *tsrc = src + j;
                    ثابت dT *d = delta_buf ? delta_buf : delta + j;

                    برائے( k = 0; k < size.اونچائی; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        دوہرا a = ستون_buf[k];
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

            برائے( ; j < size.چوڑائی; j++ )
            {
                دوہرا s0 = 0;
                ثابت sT *tsrc = src + j;
                ثابت dT *d = delta_buf ? delta_buf : delta + j;

                برائے( k = 0; k < size.اونچائی; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (دوہرا)ستون_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


سانچہ<قسم_نام sT, قسم_نام dT> ساکن خالی
MulTransposedL(ثابت Mat& srcmat, ثابت Mat& dstmat, ثابت Mat& deltamat, دوہرا scale)
{
    صحیح i, j, k;
    ثابت sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ثابت dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.قدم/حجم(src[0]);
    size_t dststep = dstmat.قدم/حجم(dst[0]);
    size_t deltastep = deltamat.صفیں > 1 ? deltamat.قدم/حجم(delta[0]) : 0;
    صحیح delta_ستون_تعداد = deltamat.ستون_تعداد;
    سائز size = srcmat.size();
    dT* tdst = dst;

    اگر( !delta )
        برائے( i = 0; i < size.اونچائی; i++, tdst += dststep )
            برائے( j = i; j < size.اونچائی; j++ )
            {
                دوہرا s = 0;
                ثابت sT *tsrc1 = src + i*srcstep;
                ثابت sT *tsrc2 = src + j*srcstep;
#اگر CV_SIMD128_64F
                اگر (ڈیٹا_Type_2<sT>::گہرائی == CV_64F && ڈیٹا_Type_2<dT>::گہرائی == CV_64F)
                {
                    ثابت دوہرا *v_tsrc1 = (دوہرا *)(tsrc1);
                    ثابت دوہرا *v_tsrc2 = (دوہرا *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    برائے( k = 0; k <= size.چوڑائی - 4; k += 4 )
                        v_s = v_جوڑو(v_s, v_جوڑو(v_mul(v_بھرو(v_tsrc1 + k), v_بھرو(v_tsrc2 + k)), v_mul(v_بھرو(v_tsrc1 + k + 2), v_بھرو(v_tsrc2 + k + 2))));
                    s += v_reduce_مجموع(v_s);
                }
                ورنہ
#اگر_ختم
                {
                    برائے( k = 0; k <= size.چوڑائی - 4; k += 4 )
                        s += (دوہرا)tsrc1[k]*tsrc2[k] + (دوہرا)tsrc1[k+1]*tsrc2[k+1] +
                             (دوہرا)tsrc1[k+2]*tsrc2[k+2] + (دوہرا)tsrc1[k+3]*tsrc2[k+3];
                }
                برائے( ; k < size.چوڑائی; k++ )
                    s += (دوہرا)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    ورنہ
    {
        dT delta_buf[4];
        صحیح delta_سرکاؤ = delta_ستون_تعداد == size.چوڑائی ? 4 : 0;
        Auto_بفر<uchar> buf(size.چوڑائی*حجم(dT));
        dT* صف_buf = (dT*)buf.ڈیٹا();

        برائے( i = 0; i < size.اونچائی; i++, tdst += dststep )
        {
            ثابت sT *tsrc1 = src + i*srcstep;
            ثابت dT *tdelta1 = delta + i*deltastep;

            اگر( delta_ستون_تعداد < size.چوڑائی )
                برائے( k = 0; k < size.چوڑائی; k++ )
                    صف_buf[k] = tsrc1[k] - tdelta1[0];
            ورنہ
                برائے( k = 0; k < size.چوڑائی; k++ )
                    صف_buf[k] = tsrc1[k] - tdelta1[k];

            برائے( j = i; j < size.اونچائی; j++ )
            {
                دوہرا s = 0;
                ثابت sT *tsrc2 = src + j*srcstep;
                ثابت dT *tdelta2 = delta + j*deltastep;
                اگر( delta_ستون_تعداد < size.چوڑائی )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#اگر CV_SIMD128_64F
                اگر (ڈیٹا_Type_2<sT>::گہرائی == CV_64F && ڈیٹا_Type_2<dT>::گہرائی == CV_64F)
                {
                    ثابت دوہرا *v_tsrc2 = (دوہرا *)(tsrc2);
                    ثابت دوہرا *v_tdelta2 = (دوہرا *)(tdelta2);
                    ثابت دوہرا *v_صف_buf = (دوہرا *)(صف_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    برائے( k = 0; k <= size.چوڑائی - 4; k += 4, v_tdelta2 += delta_سرکاؤ )
                        v_s = v_جوڑو(v_s, v_جوڑو(v_mul(v_sub(v_بھرو(v_tsrc2 + k), v_بھرو(v_tdelta2)), v_بھرو(v_صف_buf + k)), v_mul(v_sub(v_بھرو(v_tsrc2 + k + 2), v_بھرو(v_tdelta2 + 2)), v_بھرو(v_صف_buf + k + 2))));
                    s += v_reduce_مجموع(v_s);

                    tdelta2 = (ثابت dT *)(v_tdelta2);
                }
                ورنہ
#اگر_ختم
                {
                    برائے( k = 0; k <= size.چوڑائی-4; k += 4, tdelta2 += delta_سرکاؤ )
                        s += (دوہرا)صف_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (دوہرا)صف_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (دوہرا)صف_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (دوہرا)صف_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                برائے( ; k < size.چوڑائی; k++, tdelta2++ )
                    s += (دوہرا)صف_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(صحیح stype, صحیح dtype, منطقی ata)
{
    MulTransposedFunc func = تہی;
    اگر (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,اعشاری>
                   : MulTransposedL<uchar,اعشاری>;
    }
    ورنہ اگر (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,دوہرا>
                   : MulTransposedL<uchar,دوہرا>;
    }
    ورنہ اگر(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,اعشاری>
                   : MulTransposedL<ushort,اعشاری>;
    }
    ورنہ اگر(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,دوہرا>
                   : MulTransposedL<ushort,دوہرا>;
    }
    ورنہ اگر(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<مختصر,اعشاری>
                   : MulTransposedL<مختصر,اعشاری>;
    }
    ورنہ اگر(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<مختصر,دوہرا>
                   : MulTransposedL<مختصر,دوہرا>;
    }
    ورنہ اگر(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<اعشاری,اعشاری>
                   : MulTransposedL<اعشاری,اعشاری>;
    }
    ورنہ اگر(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<اعشاری,دوہرا>
                   : MulTransposedL<اعشاری,دوہرا>;
    }
    ورنہ اگر(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<دوہرا,دوہرا>
                   : MulTransposedL<دوہرا,دوہرا>;
    }
    CV_Assert(func && "Not supported");
    واپس func;
}
#اگر_ختم // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

سانچہ<قسم_نام T> ساکن اندرونی
دوہرا dotProd_(ثابت T* src1, ثابت T* src2, صحیح len)
{
    صحیح i = 0;
    دوہرا نتیجہ = 0;

#اگر CV_ENABLE_UNROLLED
    برائے( ; i <= len - 4; i += 4 )
        نتیجہ += (دوہرا)src1[i]*src2[i] + (دوہرا)src1[i+1]*src2[i+1] +
            (دوہرا)src1[i+2]*src2[i+2] + (دوہرا)src1[i+3]*src2[i+3];
#اگر_ختم
    برائے( ; i < len; i++ )
        نتیجہ += (دوہرا)src1[i]*src2[i];

    واپس نتیجہ;
}


دوہرا dotProd_8u(ثابت uchar* src1, ثابت uchar* src2, صحیح len)
{
    دوہرا r = 0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_uint16>::vlanes(), بلاک_Size0 = (1 << 15), بلاک_سائز_2;

    جب (i < len0)
    {
        بلاک_سائز_2 = std::min(len0 - i, بلاک_Size0);
        v_uint32 v_مجموع = vx_setzero_u32();
        ثابت صحیح c_چوڑائی = VTraits<v_uint16>::vlanes();

        صحیح j = 0;
        برائے (; j <= بلاک_سائز_2 - c_چوڑائی * 2; j += c_چوڑائی * 2)
        {
            v_uint8 v_src1 = vx_بھرو(src1 + j);
            v_uint8 v_src2 = vx_بھرو(src2 + j);
            v_مجموع = v_dotprod_expand_fast(v_src1, v_src2, v_مجموع);
        }

        برائے (; j <= بلاک_سائز_2 - c_چوڑائی; j += c_چوڑائی)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_بھرو_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_بھرو_expand(src2 + j));
            v_مجموع = v_جوڑو(v_مجموع, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (دوہرا)v_reduce_مجموع(v_مجموع);

        src1 += بلاک_سائز_2;
        src2 += بلاک_سائز_2;
        i += بلاک_سائز_2;
    }
    vx_cleanup();
#اگر_ختم
    واپس r + dotProd_(src1, src2, len - i);
}


دوہرا dotProd_8s(ثابت schar* src1, ثابت schar* src2, صحیح len)
{
    دوہرا r = 0.0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_int16>::vlanes(), بلاک_Size0 = (1 << 14), بلاک_سائز_2;

    جب (i < len0)
    {
        بلاک_سائز_2 = std::min(len0 - i, بلاک_Size0);
        v_int32 v_مجموع = vx_setzero_s32();
        ثابت صحیح c_چوڑائی = VTraits<v_int16>::vlanes();

        صحیح j = 0;
        برائے (; j <= بلاک_سائز_2 - c_چوڑائی * 2; j += c_چوڑائی * 2)
        {
            v_int8 v_src1 = vx_بھرو(src1 + j);
            v_int8 v_src2 = vx_بھرو(src2 + j);
            v_مجموع = v_dotprod_expand_fast(v_src1, v_src2, v_مجموع);
        }

        برائے (; j <= بلاک_سائز_2 - c_چوڑائی; j += c_چوڑائی)
        {
            v_int16 v_src1 = vx_بھرو_expand(src1 + j);
            v_int16 v_src2 = vx_بھرو_expand(src2 + j);
            v_مجموع = v_dotprod_fast(v_src1, v_src2, v_مجموع);
        }
        r += (دوہرا)v_reduce_مجموع(v_مجموع);

        src1 += بلاک_سائز_2;
        src2 += بلاک_سائز_2;
        i += بلاک_سائز_2;
    }
    vx_cleanup();
#اگر_ختم

    واپس r + dotProd_(src1, src2, len - i);
}

دوہرا dotProd_16u(ثابت ushort* src1, ثابت ushort* src2, صحیح len)
{
    دوہرا r = 0.0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_uint16>::vlanes(), بلاک_Size0 = (1 << 24), بلاک_سائز_2;

    جب (i < len0)
    {
        بلاک_سائز_2 = std::min(len0 - i, بلاک_Size0);
        v_uint64 v_مجموع = vx_setzero_u64();
        ثابت صحیح c_چوڑائی = VTraits<v_uint16>::vlanes();

        صحیح j = 0;
        برائے (; j <= بلاک_سائز_2 - c_چوڑائی; j += c_چوڑائی)
        {
            v_uint16 v_src1 = vx_بھرو(src1 + j);
            v_uint16 v_src2 = vx_بھرو(src2 + j);
            v_مجموع = v_dotprod_expand_fast(v_src1, v_src2, v_مجموع);
        }
        r += (دوہرا)v_reduce_مجموع(v_مجموع);

        src1 += بلاک_سائز_2;
        src2 += بلاک_سائز_2;
        i += بلاک_سائز_2;
    }
    vx_cleanup();
#اگر_ختم
    واپس r + dotProd_(src1, src2, len - i);
}

دوہرا dotProd_16s(ثابت مختصر* src1, ثابت مختصر* src2, صحیح len)
{
    دوہرا r = 0.0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_int16>::vlanes(), بلاک_Size0 = (1 << 24), بلاک_سائز_2;

    جب (i < len0)
    {
        بلاک_سائز_2 = std::min(len0 - i, بلاک_Size0);
        v_int64 v_مجموع = vx_setzero_s64();
        ثابت صحیح c_چوڑائی = VTraits<v_int16>::vlanes();

        صحیح j = 0;
        برائے (; j <= بلاک_سائز_2 - c_چوڑائی; j += c_چوڑائی)
        {
            v_int16 v_src1 = vx_بھرو(src1 + j);
            v_int16 v_src2 = vx_بھرو(src2 + j);
            v_مجموع = v_dotprod_expand_fast(v_src1, v_src2, v_مجموع);
        }
        r += (دوہرا)v_reduce_مجموع(v_مجموع);

        src1 += بلاک_سائز_2;
        src2 += بلاک_سائز_2;
        i += بلاک_سائز_2;
    }
    vx_cleanup();
#اگر_ختم
    واپس r + dotProd_(src1, src2, len - i);
}

دوہرا dotProd_32s(ثابت صحیح* src1, ثابت صحیح* src2, صحیح len)
{
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    دوہرا r = .0;
    صحیح i = 0;
    ثابت صحیح قدم  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#اگر CV_SIMD_چوڑائی == 16
    ثابت صحیح wstep = قدم * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    برائے (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
    {
        v_int32 v_src10 = vx_بھرو(src1);
        v_int32 v_src20 = vx_بھرو(src2);
        v_int32 v_src11 = vx_بھرو(src1 + قدم);
        v_int32 v_src21 = vx_بھرو(src2 + قدم);
        v_sum0 = v_dotprod_expand_fast(v_src10, v_src20, v_sum0);
        v_sum1 = v_dotprod_expand_fast(v_src11, v_src21, v_sum1);
    }
    v_sum0 = v_جوڑو(v_sum0, v_sum1);
#اگر_ختم
    برائے (; i <= len - قدم; i += قدم, src1 += قدم, src2 += قدم)
    {
        v_int32 v_src1 = vx_بھرو(src1);
        v_int32 v_src2 = vx_بھرو(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_مجموع(v_sum0);
    vx_cleanup();
    واپس r + dotProd_(src1, src2, len - i);
#ورنہ
    واپس dotProd_(src1, src2, len);
#اگر_ختم
}

دوہرا dotProd_32f(ثابت اعشاری* src1, ثابت اعشاری* src2, صحیح len)
{
    دوہرا r = 0.0;
    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    صحیح len0 = len & -VTraits<v_float32>::vlanes(), بلاک_Size0 = (1 << 13), بلاک_سائز_2;

    جب (i < len0)
    {
        بلاک_سائز_2 = std::min(len0 - i, بلاک_Size0);
        v_float32 v_مجموع = vx_setzero_f32();

        صحیح j = 0;
        صحیح c_چوڑائی = VTraits<v_float32>::vlanes();

#اگر CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        برائے (; j <= بلاک_سائز_2 - (c_چوڑائی * 4); j += (c_چوڑائی * 4))
        {
            v_مجموع  = v_muladd(vx_بھرو(src1 + j),
                              vx_بھرو(src2 + j), v_مجموع);
            v_sum1 = v_muladd(vx_بھرو(src1 + j + c_چوڑائی),
                              vx_بھرو(src2 + j + c_چوڑائی), v_sum1);
            v_sum2 = v_muladd(vx_بھرو(src1 + j + (c_چوڑائی * 2)),
                              vx_بھرو(src2 + j + (c_چوڑائی * 2)), v_sum2);
            v_sum3 = v_muladd(vx_بھرو(src1 + j + (c_چوڑائی * 3)),
                              vx_بھرو(src2 + j + (c_چوڑائی * 3)), v_sum3);
        }

        v_مجموع = v_جوڑو(v_مجموع, v_جوڑو(v_جوڑو(v_sum1, v_sum2), v_sum3));
#اگر_ختم

        برائے (; j <= بلاک_سائز_2 - c_چوڑائی; j += c_چوڑائی)
            v_مجموع = v_muladd(vx_بھرو(src1 + j), vx_بھرو(src2 + j), v_مجموع);

        r += v_reduce_مجموع(v_مجموع);

        src1 += بلاک_سائز_2;
        src2 += بلاک_سائز_2;
        i += بلاک_سائز_2;
    }
    vx_cleanup();
#اگر_ختم
    واپس r + dotProd_(src1, src2, len - i);
}

دوہرا dotProd_64f(ثابت دوہرا* src1, ثابت دوہرا* src2, صحیح len)
{
    واپس dotProd_(src1, src2, len);
}

#اگر_ختم
CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
} // namespace
