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

#अन्तर्भाव "precomp.hpp"

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
#जर defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#अन्यथा
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

नामाकाश cv {

// forward declarations
प्रकारव्याख्या शून्य (*TransformFunc)(स्थिरांक शून्य* src, शून्य* dst, स्थिरांक शून्य* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn);
प्रकारव्याख्या शून्य (*ScaleAddFunc)(स्थिरांक शून्य* src1, स्थिरांक शून्य* src2, शून्य* dst, पूर्णांक len, स्थिरांक शून्य* alpha);
प्रकारव्याख्या शून्य (*MulTransposedFunc)(स्थिरांक Mat& src, स्थिरांक/*preallocated*/ Mat& dst, स्थिरांक Mat& delta, द्विगुण scale);
प्रकारव्याख्या द्विगुण (*MahalanobisImplFunc)(स्थिरांक Mat& v1, स्थिरांक Mat& v2, स्थिरांक Mat& icovar, द्विगुण *diff_buffer /*[len]*/, पूर्णांक len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

शून्य gemm32f(स्थिरांक दशांश* src1, माप_प्रकार src1_step, स्थिरांक दशांश* src2, माप_प्रकार src2_step,
             दशांश alpha, स्थिरांक दशांश* src3, माप_प्रकार src3_step, दशांश beta, दशांश* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags);
शून्य gemm64f(स्थिरांक द्विगुण* src1, माप_प्रकार src1_step, स्थिरांक द्विगुण* src2, माप_प्रकार src2_step,
             द्विगुण alpha, स्थिरांक द्विगुण* src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags);
शून्य gemm32fc(स्थिरांक दशांश* src1, माप_प्रकार src1_step, स्थिरांक दशांश* src2, माप_प्रकार src2_step,
              दशांश alpha, स्थिरांक दशांश* src3, माप_प्रकार src3_step, दशांश beta, दशांश* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags);
शून्य gemm64fc(स्थिरांक द्विगुण* src1, माप_प्रकार src1_step, स्थिरांक द्विगुण* src2, माप_प्रकार src2_step,
              द्विगुण alpha, स्थिरांक द्विगुण* src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags);

TransformFunc getTransformFunc(पूर्णांक depth);
TransformFunc getDiagTransformFunc(पूर्णांक depth);
TransformFunc getPerspectiveTransform(पूर्णांक depth);

ScaleAddFunc getScaleAddFunc(पूर्णांक depth);

MahalanobisImplFunc getMahalanobisImplFunc(पूर्णांक depth);

MulTransposedFunc getMulTransposedFunc(पूर्णांक stype, पूर्णांक dtype, तार्किक ata);

द्विगुण dotProd_8u(स्थिरांक uchar* src1, स्थिरांक uchar* src2, पूर्णांक len);
द्विगुण dotProd_8s(स्थिरांक schar* src1, स्थिरांक schar* src2, पूर्णांक len);
द्विगुण dotProd_16u(स्थिरांक ushort* src1, स्थिरांक ushort* src2, पूर्णांक len);
द्विगुण dotProd_16s(स्थिरांक लघु* src1, स्थिरांक लघु* src2, पूर्णांक len);
द्विगुण dotProd_32s(स्थिरांक पूर्णांक* src1, स्थिरांक पूर्णांक* src2, पूर्णांक len);
द्विगुण dotProd_32f(स्थिरांक दशांश* src1, स्थिरांक दशांश* src2, पूर्णांक len);
द्विगुण dotProd_64f(स्थिरांक द्विगुण* src1, स्थिरांक द्विगुण* src2, पूर्णांक len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#जर !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

स्थिर शून्य
GEMM_CopyBlock( स्थिरांक uchar* src, माप_प्रकार src_step,
                uchar* dst, माप_प्रकार dst_step,
                Size size, माप_प्रकार pix_size )
{
    पूर्णांक j;
    size.width *= (पूर्णांक)(pix_size / आकार(पूर्णांक));

    साठी( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #जर CV_ENABLE_UNROLLED
        साठी( ; j <= size.width - 4; j += 4 )
        {
            पूर्णांक t0 = ((स्थिरांक पूर्णांक*)src)[j];
            पूर्णांक t1 = ((स्थिरांक पूर्णांक*)src)[j+1];
            ((पूर्णांक*)dst)[j] = t0;
            ((पूर्णांक*)dst)[j+1] = t1;
            t0 = ((स्थिरांक पूर्णांक*)src)[j+2];
            t1 = ((स्थिरांक पूर्णांक*)src)[j+3];
            ((पूर्णांक*)dst)[j+2] = t0;
            ((पूर्णांक*)dst)[j+3] = t1;
        }
        #endif
        साठी( ; j < size.width; j++ )
            ((पूर्णांक*)dst)[j] = ((स्थिरांक पूर्णांक*)src)[j];
    }
}


स्थिर शून्य
GEMM_TransposeBlock( स्थिरांक uchar* src, माप_प्रकार src_step,
                     uchar* dst, माप_प्रकार dst_step,
                     Size size, माप_प्रकार pix_size )
{
    पूर्णांक i, j;
    साठी( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        स्थिरांक uchar* _src = src;
        निवड( pix_size )
        {
        स्थिती आकार(पूर्णांक):
            साठी( j = 0; j < size.height; j++, _src += src_step )
                ((पूर्णांक*)dst)[j] = ((पूर्णांक*)_src)[0];
            थांबा;
        स्थिती आकार(पूर्णांक)*2:
            साठी( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                पूर्णांक t0 = ((पूर्णांक*)_src)[0];
                पूर्णांक t1 = ((पूर्णांक*)_src)[1];
                ((पूर्णांक*)dst)[j] = t0;
                ((पूर्णांक*)dst)[j+1] = t1;
            }
            थांबा;
        स्थिती आकार(पूर्णांक)*4:
            साठी( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                पूर्णांक t0 = ((पूर्णांक*)_src)[0];
                पूर्णांक t1 = ((पूर्णांक*)_src)[1];
                ((पूर्णांक*)dst)[j] = t0;
                ((पूर्णांक*)dst)[j+1] = t1;
                t0 = ((पूर्णांक*)_src)[2];
                t1 = ((पूर्णांक*)_src)[3];
                ((पूर्णांक*)dst)[j+2] = t0;
                ((पूर्णांक*)dst)[j+3] = t1;
            }
            थांबा;
        पूर्वनिर्धारित:
            CV_Assert(0);
            परत;
        }
    }
}

#जर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<typename ST> संरचना VecTraits;

ढाँचा<> संरचना VecTraits<दशांश> {
    प्रकारव्याख्या v_float32 VT;
    स्थिर ओळीत VT zero() { परत vx_setzero_f32(); }
    स्थिर ओळीत VT load(स्थिरांक दशांश* p) { परत vx_load(p); }
    स्थिर ओळीत VT setall(दशांश v) { परत vx_setall_f32(v); }
};
#endif

#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ढाँचा<> संरचना VecTraits<द्विगुण> {
    प्रकारव्याख्या v_float64 VT;
    स्थिर ओळीत VT zero() { परत vx_setzero_f64(); }
    स्थिर ओळीत VT load(स्थिरांक द्विगुण* p) { परत vx_load(p); }
    स्थिर ओळीत VT setall(द्विगुण v) { परत vx_setall_f64(v); }
};
#endif

#जर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<typename ST>
स्थिर ओळीत ST simdDotProduct(स्थिरांक ST* a, स्थिरांक ST* b, पूर्णांक n, पूर्णांक& k)
{
    प्रकारव्याख्या typename VecTraits<ST>::VT VecT;
    स्थिरांक पूर्णांक vlanes = VTraits<VecT>::vlanes();
    स्थिरांक पूर्णांक stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    साठी( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    साठी( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    परत v_reduce_sum(vs0);
}
#endif

#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
स्थिर ओळीत द्विगुण simdDotProduct_f32f64(स्थिरांक दशांश* a, स्थिरांक दशांश* b, पूर्णांक n, पूर्णांक& k)
{
    स्थिरांक पूर्णांक vlanes_f32 = VTraits<v_float32>::vlanes();
    स्थिरांक पूर्णांक stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    साठी( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_वरचा(va0), v_cvt_f64_वरचा(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_वरचा(va1), v_cvt_f64_वरचा(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    साठी( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_वरचा(va), v_cvt_f64_वरचा(vb), vs0);
    }
    परत v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
स्थिर ओळीत शून्य simdGEMM_kj_f32f64(
    स्थिरांक दशांश* a_data, स्थिरांक दशांश* _b_data, माप_प्रकार b_step,
    स्थिरांक दशांश* _c_data, माप_प्रकार c_step1,
    दशांश* d_data,
    पूर्णांक n, पूर्णांक m,
    द्विगुण alpha, द्विगुण beta,
    पूर्णांक& j)
{
    स्थिरांक पूर्णांक vlanes_f32 = VTraits<v_float32>::vlanes();
    स्थिरांक पूर्णांक vlanes_f64 = VTraits<v_float64>::vlanes();

    जर( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        स्थिरांक पूर्णांक max_m = 1600 / (पूर्णांक)आकार(दशांश);
        cv::AutoBuffer<द्विगुण> _s_buf(max_m);
        द्विगुण* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            साठी( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            साठी( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            स्थिरांक दशांश* b_ptr = _b_data;
            साठी( पूर्णांक k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((द्विगुण)a_data[k]);
                j = 0;
                साठी( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_वरचा(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                द्विगुण a_val = (द्विगुण)a_data[k];
                साठी( ; j < m; j++ )
                    s_buf[j] += a_val * (द्विगुण)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            स्थिरांक दशांश* c_data = _c_data;
            j = 0;
            जर( !_c_data )
            {
                साठी( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                साठी( ; j < m; j++ )
                    d_data[j] = (दशांश)(s_buf[j] * alpha);
            }
            अन्यथा जर( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                साठी( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_वरचा(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                साठी( ; j < m; j++ )
                    d_data[j] = (दशांश)(s_buf[j] * alpha + (द्विगुण)c_data[j] * beta);
            }
            अन्यथा
            {
                साठी( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (दशांश)(s_buf[j] * alpha + (द्विगुण)c_data[0] * beta);
            }
        }
        j = m;
    }
    अन्यथा जर( vlanes_f32 * (पूर्णांक)आकार(दशांश) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        स्थिरांक दशांश* c_data = _c_data;
        साठी( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            स्थिरांक दशांश* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            साठी( पूर्णांक k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((द्विगुण)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_वरचा(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            जर( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            अन्यथा जर( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_वरचा(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            अन्यथा
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) दशांश buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                साठी( पूर्णांक jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (दशांश)beta;
            }
        }
    }
}
#endif

#जर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<typename ST>
स्थिर ओळीत शून्य simdGEMM_kj(
    स्थिरांक ST* a_data, स्थिरांक ST* _b_data, माप_प्रकार b_step,
    स्थिरांक ST* _c_data, माप_प्रकार c_step1,
    ST* d_data,
    पूर्णांक n, पूर्णांक m,
    द्विगुण alpha, द्विगुण beta,
    पूर्णांक& j)
{
    प्रकारव्याख्या typename VecTraits<ST>::VT VecT;
    स्थिरांक पूर्णांक vlanes = VTraits<VecT>::vlanes();

    जर( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        स्थिरांक पूर्णांक max_m = 1600 / (पूर्णांक)आकार(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            साठी( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            साठी( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            स्थिरांक ST* b_ptr = _b_data;
            साठी( पूर्णांक k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                साठी( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                साठी( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            स्थिरांक ST* c_data = _c_data;
            j = 0;
            जर( !_c_data )
            {
                साठी( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                साठी( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            अन्यथा जर( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                साठी( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                साठी( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            अन्यथा
            {
                साठी( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    अन्यथा जर( vlanes * (पूर्णांक)आकार(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        स्थिरांक ST* c_data = _c_data;
        साठी( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            स्थिरांक ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            साठी( पूर्णांक k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            जर( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            अन्यथा जर( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            अन्यथा
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                साठी( पूर्णांक jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


ढाँचा<typename T, typename WT> स्थिर शून्य
GEMMSingleMul( स्थिरांक T* a_data, माप_प्रकार a_step,
               स्थिरांक T* b_data, माप_प्रकार b_step,
               स्थिरांक T* c_data, माप_प्रकार c_step,
               T* d_data, माप_प्रकार d_step,
               Size a_size, Size d_size,
               द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    पूर्णांक i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    स्थिरांक T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    माप_प्रकार a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= आकार(a_data[0]);
    b_step /= आकार(b_data[0]);
    c_step /= आकार(c_data[0]);
    d_step /= आकार(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    जर( !c_data )
        c_step0 = c_step1 = 0;
    अन्यथा जर( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    अन्यथा
        c_step0 = 1, c_step1 = c_step;

    जर( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        जर( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    जर( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        जर( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            साठी( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        जर( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            साठी( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        साठी( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            साठी( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                जर( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                अन्यथा
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            साठी( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                जर( !c_data )
                    d_data[j] = T(s0);
                अन्यथा
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    अन्यथा जर( flags & GEMM_2_T ) /* A * Bt */
    {
        साठी( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            जर( a_buf )
            {
                साठी( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            साठी( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#जर CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                जर( आकार(WT) == आकार(द्विगुण) )
                {
                    जर( आकार(T) == आकार(द्विगुण) )
                        s0 = (WT)simdDotProduct((स्थिरांक द्विगुण*)a_data, (स्थिरांक द्विगुण*)b_data, n, k);
                    अन्यथा जर( आकार(T) == आकार(दशांश) )
                        s0 = (WT)simdDotProduct_f32f64((स्थिरांक दशांश*)a_data, (स्थिरांक दशांश*)b_data, n, k);
                }
                अन्यथा
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #जर CV_ENABLE_UNROLLED
                    साठी( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                साठी( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                जर( !c_data )
                    d_data[j] = T(s0);
                अन्यथा
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    अन्यथा जर( d_size.width*आकार(d_data[0]) <= 1600 )
    {
        साठी( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            जर( a_buf )
            {
                साठी( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            जर( आकार(WT) == आकार(द्विगुण) )
            {
                जर( आकार(T) == आकार(द्विगुण) )
                    simdGEMM_kj((स्थिरांक द्विगुण*)a_data, (स्थिरांक द्विगुण*)_b_data, b_step,
                                        (स्थिरांक द्विगुण*)_c_data, c_step1,
                                (द्विगुण*)d_data, n, m, alpha, beta, j);
                अन्यथा जर( आकार(T) == आकार(दशांश) )
                    simdGEMM_kj_f32f64((स्थिरांक दशांश*)a_data, (स्थिरांक दशांश*)_b_data, b_step,
                                        (स्थिरांक दशांश*)_c_data, c_step1,
                                (दशांश*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            साठी( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                स्थिरांक T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                साठी( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                जर( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                अन्यथा
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            साठी( ; j < m; j++, c_data += c_step1 )
            {
                स्थिरांक T* b = _b_data + j;
                WT s0(0);

                साठी( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                जर( !c_data )
                    d_data[j] = T(s0);
                अन्यथा
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    अन्यथा
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        साठी( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            जर( a_buf )
            {
                साठी( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            साठी( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            साठी( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                जर( आकार(WT) == आकार(द्विगुण) )
                {
                    जर( आकार(T) == आकार(द्विगुण) )
                    {
                        स्थिरांक पूर्णांक vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((स्थिरांक द्विगुण*)&al));
                        साठी( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((स्थिरांक द्विगुण*)(d_buf + j));
                            v_float64 vb = vx_load((स्थिरांक द्विगुण*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((द्विगुण*)(d_buf + j), vd);
                        }
                    }
                    अन्यथा
                    {
                        स्थिरांक पूर्णांक vlanes_f32 = VTraits<v_float32>::vlanes();
                        स्थिरांक पूर्णांक vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((स्थिरांक द्विगुण*)&al));
                        साठी( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((स्थिरांक दशांश*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_वरचा(vb);
                            v_float64 vd_lo = vx_load((स्थिरांक द्विगुण*)(d_buf + j));
                            v_float64 vd_hi = vx_load((स्थिरांक द्विगुण*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((द्विगुण*)(d_buf + j), vd_lo);
                            v_store((द्विगुण*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                साठी( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            जर( !c_data )
                साठी( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            अन्यथा
                साठी( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ढाँचा<typename WT>
स्थिर ओळीत शून्य setDoubleDataZero(WT* d_data, पूर्णांक m, पूर्णांक vlanes)
{
    v_float64 vz = vx_setzero_f64();
    पूर्णांक j = 0;
    साठी( ; j <= m - vlanes; j += vlanes )
        v_store((द्विगुण*)(d_data + j), vz);
    साठी( ; j < m; j++ )
        d_data[j] = WT(0);
}

ढाँचा<typename ST>
स्थिर ओळीत शून्य simdMulAdd(स्थिरांक ST* b, द्विगुण* d, पूर्णांक m, द्विगुण a_val);

ढाँचा<>
ओळीत शून्य simdMulAdd<दशांश>(
    स्थिरांक दशांश* b, द्विगुण* d, पूर्णांक m, द्विगुण a_val)
{
    स्थिरांक पूर्णांक vlanes_f32 = VTraits<v_float32>::vlanes();
    स्थिरांक पूर्णांक vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    पूर्णांक j = 0;
    साठी( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_वरचा(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    साठी( ; j < m; j++ )
        d[j] += a_val * (द्विगुण)b[j];
}

ढाँचा<>
ओळीत शून्य simdMulAdd<द्विगुण>(
    स्थिरांक द्विगुण* b, द्विगुण* d, पूर्णांक m, द्विगुण a_val)
{
    स्थिरांक पूर्णांक vlanes = VTraits<v_float64>::vlanes();
    स्थिरांक पूर्णांक stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    पूर्णांक j = 0;
    साठी( ; j <= m - stride2; j += stride2 )
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
    साठी( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    साठी( ; j < m; j++ )
        d[j] += a_val * b[j];
}

ढाँचा<typename ST>
स्थिर ओळीत शून्य simdBlockMul_kj(
    स्थिरांक ST* a_data, स्थिरांक ST* b_data, माप_प्रकार b_step,
    द्विगुण* d_data, पूर्णांक n, पूर्णांक m, तार्किक do_acc)
{
    स्थिरांक पूर्णांक vlanes = VTraits<v_float64>::vlanes();
    जर( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    साठी( पूर्णांक k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (द्विगुण)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

ढाँचा<typename T, typename WT> स्थिर शून्य
GEMMBlockMul( स्थिरांक T* a_data, माप_प्रकार a_step,
              स्थिरांक T* b_data, माप_प्रकार b_step,
              WT* d_data, माप_प्रकार d_step,
              Size a_size, Size d_size, पूर्णांक flags )
{
    पूर्णांक i, j, k, n = a_size.width, m = d_size.width;
    स्थिरांक T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    माप_प्रकार a_step0, a_step1, t_step;
    पूर्णांक do_acc = flags & 16;

    a_step /= आकार(a_data[0]);
    b_step /= आकार(b_data[0]);
    d_step /= आकार(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    जर( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    जर( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        साठी( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            जर( a_buf )
            {
                साठी( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            साठी( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#जर CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                जर( आकार(WT) == आकार(द्विगुण) )
                {
                    जर( आकार(T) == आकार(द्विगुण) )
                        s0 += (WT)simdDotProduct((स्थिरांक द्विगुण*)a_data, (स्थिरांक द्विगुण*)b_data, n, k);
                    अन्यथा जर( आकार(T) == आकार(दशांश) )
                        s0 += (WT)simdDotProduct_f32f64((स्थिरांक दशांश*)a_data, (स्थिरांक दशांश*)b_data, n, k);
                }
                अन्यथा
#endif
                {
                    WT s1(0);
                    साठी( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                साठी( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    अन्यथा
    {
        साठी( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            जर( a_buf )
            {
                साठी( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            जर( आकार(WT) == आकार(द्विगुण) )
            {
                जर( आकार(T) == आकार(द्विगुण) )
                    simdBlockMul_kj(
                        (स्थिरांक द्विगुण*)a_data, (स्थिरांक द्विगुण*)b_data, b_step,
                        (द्विगुण*)d_data, n, m, do_acc != 0);
                अन्यथा
                    simdBlockMul_kj(
                        (स्थिरांक दशांश*)a_data, (स्थिरांक दशांश*)b_data, b_step,
                        (द्विगुण*)d_data, n, m, do_acc != 0);
            }
            अन्यथा
#endif
            {
                साठी( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    स्थिरांक T* b = b_data + j;

                    जर( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    अन्यथा
                        s0 = s1 = s2 = s3 = WT(0);

                    साठी( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                साठी( ; j < m; j++ )
                {
                    स्थिरांक T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    साठी( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


ढाँचा<typename T, typename WT> स्थिर शून्य
GEMMStore( स्थिरांक T* c_data, माप_प्रकार c_step,
           स्थिरांक WT* d_buf, माप_प्रकार d_buf_step,
           T* d_data, माप_प्रकार d_step, Size d_size,
           द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    स्थिरांक T* _c_data = c_data;
    पूर्णांक j;
    माप_प्रकार c_step0, c_step1;

    c_step /= आकार(c_data[0]);
    d_buf_step /= आकार(d_buf[0]);
    d_step /= आकार(d_data[0]);

    जर( !c_data )
        c_step0 = c_step1 = 0;
    अन्यथा जर( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    अन्यथा
        c_step0 = 1, c_step1 = c_step;

    साठी( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        जर( _c_data )
        {
            c_data = _c_data;
            j=0;
             #जर CV_ENABLE_UNROLLED
            साठी(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
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
            साठी( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        अन्यथा
        {
            j = 0;
             #जर CV_ENABLE_UNROLLED
            साठी( ; j <= d_size.width - 4; j += 4 )
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
            साठी( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


प्रकारव्याख्या शून्य (*GEMMSingleMulFunc)( स्थिरांक शून्य* src1, माप_प्रकार step1,
                   स्थिरांक शून्य* src2, माप_प्रकार step2, स्थिरांक शून्य* src3, माप_प्रकार step3,
                   शून्य* dst, माप_प्रकार dststep, Size srcsize, Size dstsize,
                   द्विगुण alpha, द्विगुण beta, पूर्णांक flags );

प्रकारव्याख्या शून्य (*GEMMBlockMulFunc)( स्थिरांक शून्य* src1, माप_प्रकार step1,
                   स्थिरांक शून्य* src2, माप_प्रकार step2, शून्य* dst, माप_प्रकार dststep,
                   Size srcsize, Size dstsize, पूर्णांक flags );

प्रकारव्याख्या शून्य (*GEMMStoreFunc)( स्थिरांक शून्य* src1, माप_प्रकार step1,
                   स्थिरांक शून्य* src2, माप_प्रकार step2, शून्य* dst, माप_प्रकार dststep,
                   Size dstsize, द्विगुण alpha, द्विगुण beta, पूर्णांक flags );

ढाँचा<typename T, typename WT, शून्य (*fn)(स्थिरांक T*, माप_प्रकार, स्थिरांक T*, माप_प्रकार, स्थिरांक T*, माप_प्रकार, T*, माप_प्रकार, Size, Size, द्विगुण, द्विगुण, पूर्णांक)>
स्थिर शून्य gemmSingleMulWrap( स्थिरांक शून्य* src1, माप_प्रकार step1,
                   स्थिरांक शून्य* src2, माप_प्रकार step2, स्थिरांक शून्य* src3, माप_प्रकार step3,
                   शून्य* dst, माप_प्रकार dststep, Size srcsize, Size dstsize,
                   द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    fn((स्थिरांक T*)src1, step1, (स्थिरांक T*)src2, step2, (स्थिरांक T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

ढाँचा<typename T, typename WT, शून्य (*fn)(स्थिरांक T*, माप_प्रकार, स्थिरांक T*, माप_प्रकार, WT*, माप_प्रकार, Size, Size, पूर्णांक)>
स्थिर शून्य gemmBlockMulWrap( स्थिरांक शून्य* src1, माप_प्रकार step1,
                   स्थिरांक शून्य* src2, माप_प्रकार step2, शून्य* dst, माप_प्रकार dststep,
                   Size srcsize, Size dstsize, पूर्णांक flags )
{
    fn((स्थिरांक T*)src1, step1, (स्थिरांक T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

ढाँचा<typename T, typename WT, शून्य (*fn)(स्थिरांक T*, माप_प्रकार, स्थिरांक WT*, माप_प्रकार, T*, माप_प्रकार, Size, द्विगुण, द्विगुण, पूर्णांक)>
स्थिर शून्य gemmStoreWrap( स्थिरांक शून्य* src1, माप_प्रकार step1,
                   स्थिरांक शून्य* src2, माप_प्रकार step2, शून्य* dst, माप_प्रकार dststep,
                   Size dstsize, द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    fn((स्थिरांक T*)src1, step1, (स्थिरांक WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

स्थिर शून्य GEMMSingleMul_32f( स्थिरांक दशांश* a_data, माप_प्रकार a_step,
              स्थिरांक दशांश* b_data, माप_प्रकार b_step,
              स्थिरांक दशांश* c_data, माप_प्रकार c_step,
              दशांश* d_data, माप_प्रकार d_step,
              Size a_size, Size d_size,
              द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    GEMMSingleMul<दशांश,द्विगुण>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

स्थिर शून्य GEMMSingleMul_64f( स्थिरांक द्विगुण* a_data, माप_प्रकार a_step,
                              स्थिरांक द्विगुण* b_data, माप_प्रकार b_step,
                              स्थिरांक द्विगुण* c_data, माप_प्रकार c_step,
                              द्विगुण* d_data, माप_प्रकार d_step,
                              Size a_size, Size d_size,
                              द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    GEMMSingleMul<द्विगुण,द्विगुण>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


स्थिर शून्य GEMMSingleMul_32fc( स्थिरांक Complexf* a_data, माप_प्रकार a_step,
                              स्थिरांक Complexf* b_data, माप_प्रकार b_step,
                              स्थिरांक Complexf* c_data, माप_प्रकार c_step,
                              Complexf* d_data, माप_प्रकार d_step,
                              Size a_size, Size d_size,
                              द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

स्थिर शून्य GEMMSingleMul_64fc( स्थिरांक Complexd* a_data, माप_प्रकार a_step,
                              स्थिरांक Complexd* b_data, माप_प्रकार b_step,
                              स्थिरांक Complexd* c_data, माप_प्रकार c_step,
                              Complexd* d_data, माप_प्रकार d_step,
                              Size a_size, Size d_size,
                              द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

स्थिर शून्य GEMMBlockMul_32f( स्थिरांक दशांश* a_data, माप_प्रकार a_step,
             स्थिरांक दशांश* b_data, माप_प्रकार b_step,
             द्विगुण* d_data, माप_प्रकार d_step,
             Size a_size, Size d_size, पूर्णांक flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


स्थिर शून्य GEMMBlockMul_64f( स्थिरांक द्विगुण* a_data, माप_प्रकार a_step,
                             स्थिरांक द्विगुण* b_data, माप_प्रकार b_step,
                             द्विगुण* d_data, माप_प्रकार d_step,
                             Size a_size, Size d_size, पूर्णांक flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


स्थिर शून्य GEMMBlockMul_32fc( स्थिरांक Complexf* a_data, माप_प्रकार a_step,
                             स्थिरांक Complexf* b_data, माप_प्रकार b_step,
                             Complexd* d_data, माप_प्रकार d_step,
                             Size a_size, Size d_size, पूर्णांक flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


स्थिर शून्य GEMMBlockMul_64fc( स्थिरांक Complexd* a_data, माप_प्रकार a_step,
                             स्थिरांक Complexd* b_data, माप_प्रकार b_step,
                             Complexd* d_data, माप_प्रकार d_step,
                             Size a_size, Size d_size, पूर्णांक flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


स्थिर शून्य GEMMStore_32f( स्थिरांक दशांश* c_data, माप_प्रकार c_step,
          स्थिरांक द्विगुण* d_buf, माप_प्रकार d_buf_step,
          दशांश* d_data, माप_प्रकार d_step, Size d_size,
          द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


स्थिर शून्य GEMMStore_64f( स्थिरांक द्विगुण* c_data, माप_प्रकार c_step,
                      स्थिरांक द्विगुण* d_buf, माप_प्रकार d_buf_step,
                      द्विगुण* d_data, माप_प्रकार d_step, Size d_size,
                      द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


स्थिर शून्य GEMMStore_32fc( स्थिरांक Complexf* c_data, माप_प्रकार c_step,
                          स्थिरांक Complexd* d_buf, माप_प्रकार d_buf_step,
                          Complexf* d_data, माप_प्रकार d_step, Size d_size,
                          द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


स्थिर शून्य GEMMStore_64fc( स्थिरांक Complexd* c_data, माप_प्रकार c_step,
                          स्थिरांक Complexd* d_buf, माप_प्रकार d_buf_step,
                          Complexd* d_data, माप_प्रकार d_step, Size d_size,
                          द्विगुण alpha, द्विगुण beta, पूर्णांक flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

स्थिर शून्य gemmImpl( Mat A, Mat B, द्विगुण alpha,
           Mat C, द्विगुण beta, Mat D, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    स्थिरांक पूर्णांक block_lin_size = 128;
    स्थिरांक पूर्णांक block_size = block_lin_size * block_lin_size;

    स्थिर द्विगुण zero[] = {0,0,0,0};
    स्थिर दशांश zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    पूर्णांक i, len = 0, type = A.type();

    निवड( flags & (GEMM_1_T|GEMM_2_T) )
    {
    स्थिती 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        थांबा;
    स्थिती 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        थांबा;
    स्थिती 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        थांबा;
    स्थिती 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        थांबा;
    }

    जर( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        जर( type == CV_32F )
        {
            दशांश* d = D.ptr<दशांश>();
            स्थिरांक दशांश *a = A.ptr<दशांश>(),
                        *b = B.ptr<दशांश>(),
                        *c = (स्थिरांक दशांश*)C.data;
            माप_प्रकार d_step = D.step/आकार(d[0]),
                a_step = A.step/आकार(a[0]),
                b_step = B.step/आकार(b[0]),
                c_step = C.data ? C.step/आकार(c[0]) : 0;

            जर( !c )
                c = zerof;

            निवड( len )
            {
            स्थिती 2:
                जर( len == d_size.width && b != d )
                {
                    साठी( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        दशांश t0 = a[0]*b[0] + a[1]*b[b_step];
                        दशांश t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (दशांश)(t0*alpha + c[0]*beta);
                        d[1] = (दशांश)(t1*alpha + c[1]*beta);
                    }
                }
                अन्यथा जर( a != d )
                {
                    पूर्णांक c_step0 = 1;
                    जर( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    साठी( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        दशांश t0 = a[0]*b[0] + a[1]*b[b_step];
                        दशांश t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (दशांश)(t0*alpha + c[0]*beta);
                        d[d_step] = (दशांश)(t1*alpha + c[c_step]*beta);
                    }
                }
                अन्यथा
                    थांबा;
                परत;
            स्थिती 3:
                जर( len == d_size.width && b != d )
                {
                    साठी( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        दशांश t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        दशांश t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        दशांश t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (दशांश)(t0*alpha + c[0]*beta);
                        d[1] = (दशांश)(t1*alpha + c[1]*beta);
                        d[2] = (दशांश)(t2*alpha + c[2]*beta);
                    }
                }
                अन्यथा जर( a != d )
                {
                    पूर्णांक c_step0 = 1;
                    जर( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    साठी( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        दशांश t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        दशांश t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        दशांश t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (दशांश)(t0*alpha + c[0]*beta);
                        d[d_step] = (दशांश)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (दशांश)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                अन्यथा
                    थांबा;
                परत;
            स्थिती 4:
                जर( len == d_size.width && b != d )
                {
                    साठी( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        दशांश t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        दशांश t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        दशांश t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        दशांश t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (दशांश)(t0*alpha + c[0]*beta);
                        d[1] = (दशांश)(t1*alpha + c[1]*beta);
                        d[2] = (दशांश)(t2*alpha + c[2]*beta);
                        d[3] = (दशांश)(t3*alpha + c[3]*beta);
                    }
                }
                अन्यथा जर( len <= 16 && a != d )
                {
                    पूर्णांक c_step0 = 1;
                    जर( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    साठी( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        दशांश t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        दशांश t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        दशांश t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        दशांश t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (दशांश)(t0*alpha + c[0]*beta);
                        d[d_step] = (दशांश)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (दशांश)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (दशांश)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                अन्यथा
                    थांबा;
                परत;
            }
        }

        जर( type == CV_64F )
        {
            द्विगुण* d = D.ptr<द्विगुण>();
            स्थिरांक द्विगुण *a = A.ptr<द्विगुण>(),
                         *b = B.ptr<द्विगुण>(),
                         *c = (स्थिरांक द्विगुण*)C.data;
            माप_प्रकार d_step = D.step/आकार(d[0]),
                a_step = A.step/आकार(a[0]),
                b_step = B.step/आकार(b[0]),
                c_step = C.data ? C.step/आकार(c[0]) : 0;
            जर( !c )
                c = zero;

            निवड( len )
            {
            स्थिती 2:
                जर( len == d_size.width && b != d )
                {
                    साठी( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_step];
                        द्विगुण t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                अन्यथा जर( a != d )
                {
                    पूर्णांक c_step0 = 1;
                    जर( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    साठी( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_step];
                        द्विगुण t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                अन्यथा
                    थांबा;
                परत;
            स्थिती 3:
                जर( len == d_size.width && b != d )
                {
                    साठी( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        द्विगुण t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        द्विगुण t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                अन्यथा जर( a != d )
                {
                    पूर्णांक c_step0 = 1;
                    जर( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    साठी( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        द्विगुण t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        द्विगुण t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                अन्यथा
                    थांबा;
                परत;
            स्थिती 4:
                जर( len == d_size.width && b != d )
                {
                    साठी( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        द्विगुण t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        द्विगुण t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        द्विगुण t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                अन्यथा जर( d_size.width <= 16 && a != d )
                {
                    पूर्णांक c_step0 = 1;
                    जर( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    साठी( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        द्विगुण t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        द्विगुण t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        द्विगुण t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                अन्यथा
                    थांबा;
                परत;
            }
        }
    }

    {
    माप_प्रकार b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    स्थिरांक uchar* Cdata = C.data;
    माप_प्रकार Cstep = C.data ? (माप_प्रकार)C.step : 0;
    AutoBuffer<uchar> buf;

    जर( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<दशांश, द्विगुण, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<दशांश, द्विगुण, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<दशांश, द्विगुण, GEMMStore_32f>;
    }
    अन्यथा जर( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<द्विगुण, द्विगुण, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<द्विगुण, द्विगुण, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<द्विगुण, द्विगुण, GEMMStore_64f>;
    }
    अन्यथा जर( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    अन्यथा
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    जर( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
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
    else*/ जर( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    अन्यथा
    {
        पूर्णांक is_a_t = flags & GEMM_1_T;
        पूर्णांक is_b_t = flags & GEMM_2_T;
        पूर्णांक elem_size = CV_ELEM_SIZE(type);
        पूर्णांक dk0_1, dk0_2;
        माप_प्रकार a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        पूर्णांक j, k, di = 0, dj = 0, dk = 0;
        पूर्णांक dm0, dn0, dk0;
        माप_प्रकार a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        पूर्णांक work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        जर( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        अन्यथा
            a_step0 = elem_size, a_step1 = A.step;

        जर( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        अन्यथा
            b_step0 = elem_size, b_step1 = b_step;

        जर( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        अन्यथा जर( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        अन्यथा
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        जर( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        जर( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (माप_प्रकार)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (माप_प्रकार)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        जर( is_a_t )
        {
            a_buf_size = (माप_प्रकार)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        जर( is_a_t )
            a_buf = b_buf + b_buf_size;

        साठी( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            जर( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            साठी( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                स्थिरांक uchar* _c = Cdata + i*c_step0 + j*c_step1;
                माप_प्रकार _d_step = matD->step;
                dj = dn0;

                जर( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                जर( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                साठी( k = 0; k < len; k += dk )
                {
                    स्थिरांक uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    माप_प्रकार _a_step = A.step;
                    स्थिरांक uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    माप_प्रकार _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    जर( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    जर( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    अन्यथा
                        a_bl_size.width = di, a_bl_size.height = dk;

                    जर( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    जर( dj < d_size.width )
                    {
                        Size b_size;
                        जर( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        अन्यथा
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    जर( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    अन्यथा
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                जर( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

ढाँचा <typename fptype>ओळीत स्थिर शून्य
callGemmImpl(स्थिरांक fptype *src1, माप_प्रकार src1_step, स्थिरांक fptype *src2, माप_प्रकार src2_step, fptype alpha,
          स्थिरांक fptype *src3, माप_प्रकार src3_step, fptype beta, fptype *dst, माप_प्रकार dst_step, पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags, पूर्णांक type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    पूर्णांक b_m, b_n, c_m, c_n, m_d;

    जर(flags & GEMM_2_T)
    {
        b_m = n_d;
        जर(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        अन्यथा
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    अन्यथा
    {
        b_n = n_d;
        जर(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        अन्यथा
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    जर(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    अन्यथा
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    जर(src1 != रिक्त)
        A = Mat(m_a, n_a, type, (शून्य*)src1, src1_step);
    जर(src2 != रिक्त)
        B = Mat(b_m, b_n, type, (शून्य*)src2, src2_step);
    जर(src3 != रिक्त && beta != 0.0)
        C = Mat(c_m, c_n, type, (शून्य*)src3, src3_step);
    Mat D(m_d, n_d, type, (शून्य*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

शून्य gemm32f(स्थिरांक दशांश* src1, माप_प्रकार src1_step, स्थिरांक दशांश* src2, माप_प्रकार src2_step,
             दशांश alpha, स्थिरांक दशांश* src3, माप_प्रकार src3_step, दशांश beta, दशांश* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

शून्य gemm64f(स्थिरांक द्विगुण* src1, माप_प्रकार src1_step, स्थिरांक द्विगुण* src2, माप_प्रकार src2_step,
             द्विगुण alpha, स्थिरांक द्विगुण* src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

शून्य gemm32fc(स्थिरांक दशांश* src1, माप_प्रकार src1_step, स्थिरांक दशांश* src2, माप_प्रकार src2_step,
              दशांश alpha, स्थिरांक दशांश* src3, माप_प्रकार src3_step, दशांश beta, दशांश* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

शून्य gemm64fc(स्थिरांक द्विगुण* src1, माप_प्रकार src1_step, स्थिरांक द्विगुण* src2, माप_प्रकार src2_step,
              द्विगुण alpha, स्थिरांक द्विगुण* src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ढाँचा<typename T, typename WT> स्थिर शून्य
transform_( स्थिरांक T* src, T* dst, स्थिरांक WT* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
    पूर्णांक x;

    जर( scn == 2 && dcn == 2 )
    {
        साठी( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    अन्यथा जर( scn == 3 && dcn == 3 )
    {
        साठी( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    अन्यथा जर( scn == 3 && dcn == 1 )
    {
        साठी( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    अन्यथा जर( scn == 4 && dcn == 4 )
    {
        साठी( x = 0; x < len*4; x += 4 )
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
    अन्यथा
    {
        साठी( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            स्थिरांक WT* _m = m;
            पूर्णांक j, k;
            साठी( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                साठी( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

स्थिर शून्य
transform_8u( स्थिरांक uchar* src, uchar* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक BITS = 10, SCALE = 1 << BITS;
    स्थिरांक दशांश MAX_M = (दशांश)(1 << (15 - BITS));

    जर( scn == 3 && dcn == 3 &&
        std::असल(m[0]) < MAX_M && std::असल(m[1]) < MAX_M && std::असल(m[ 2]) < MAX_M*256 && std::असल(m[ 3]) < MAX_M*256 &&
        std::असल(m[4]) < MAX_M && std::असल(m[5]) < MAX_M && std::असल(m[ 6]) < MAX_M*256 && std::असल(m[ 7]) < MAX_M*256 &&
        std::असल(m[8]) < MAX_M && std::असल(m[9]) < MAX_M && std::असल(m[10]) < MAX_M*256 && std::असल(m[11]) < MAX_M*256 )
    {
        स्थिरांक पूर्णांक nChannels = 3;

        संघ {
            लघु s[6];
            पूर्णांक p[3];
        } m16;
        m16.s[0] = saturate_cast<लघु>(m[0] * SCALE); m16.s[1] = saturate_cast<लघु>(m[1] * SCALE);
        m16.s[2] = saturate_cast<लघु>(m[4] * SCALE); m16.s[3] = saturate_cast<लघु>(m[5] * SCALE);
        m16.s[4] = saturate_cast<लघु>(m[8] * SCALE); m16.s[5] = saturate_cast<लघु>(m[9] * SCALE);
        पूर्णांक m32[] = {saturate_cast<पूर्णांक>(m[ 2] * SCALE), saturate_cast<पूर्णांक>(m[ 3] * SCALE),
                     saturate_cast<पूर्णांक>(m[ 6] * SCALE), saturate_cast<पूर्णांक>(m[ 7] * SCALE),
                     saturate_cast<पूर्णांक>(m[10] * SCALE), saturate_cast<पूर्णांक>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        पूर्णांक x = 0;
        साठी (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<पूर्णांक>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<पूर्णांक>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<पूर्णांक>((m[11] + 0.5f)*SCALE);
        साठी( ; x < len * nChannels; x += nChannels )
        {
            पूर्णांक v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        परत;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_16u( स्थिरांक ushort* src, ushort* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर( scn == 3 && dcn == 3 )
    {
        पूर्णांक x = 0;
#जर CV_SIMD_WIDTH > 16
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
        साठी (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#जर CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_डावा<1>(_m0l);
        v_float32x4 _m1h = v_rotate_डावा<1>(_m1l);
        v_float32x4 _m2h = v_rotate_डावा<1>(_m2l);
        v_float32x4 _m3h = v_rotate_डावा<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        साठी( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_उजवा<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        साठी( ; x < len * 3; x += 3 )
        {
            दशांश v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        परत;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_32f( स्थिरांक दशांश* src, दशांश* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#जर (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    पूर्णांक x = 0;
    जर( scn == 3 && dcn == 3 )
    {
        पूर्णांक idx[VTraits<v_float32>::max_nlanes/2];
        साठी( पूर्णांक i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        दशांश _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        साठी( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        साठी( ; x < len*3; x += 3 )
        {
            दशांश v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            दशांश t0 = saturate_cast<दशांश>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            दशांश t1 = saturate_cast<दशांश>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            दशांश t2 = saturate_cast<दशांश>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        परत;
    }

    जर( scn == 4 && dcn == 4 )
    {
#जर CV_SIMD_WIDTH > 16
        पूर्णांक idx[VTraits<v_float32>::max_nlanes/4];
        साठी( पूर्णांक i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        दशांश _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        साठी( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#जर CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        साठी( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#अन्यथा // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        साठी( ; x < len*4; x += 4 )
        {
            दशांश v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            दशांश t0 = saturate_cast<दशांश>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            दशांश t1 = saturate_cast<दशांश>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            दशांश t2 = saturate_cast<दशांश>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            दशांश t3 = saturate_cast<दशांश>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        परत;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


स्थिर शून्य
transform_8s(स्थिरांक schar* src, schar* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक BITS = 10, SCALE = 1 << BITS;
    स्थिरांक दशांश MAX_M = (दशांश)(1 << (15 - BITS));

    जर( scn == 3 && dcn == 3 &&
        std::असल(m[0]) < MAX_M && std::असल(m[1]) < MAX_M && std::असल(m[ 2]) < MAX_M*256 && std::असल(m[ 3]) < MAX_M*256 &&
        std::असल(m[4]) < MAX_M && std::असल(m[5]) < MAX_M && std::असल(m[ 6]) < MAX_M*256 && std::असल(m[ 7]) < MAX_M*256 &&
        std::असल(m[8]) < MAX_M && std::असल(m[9]) < MAX_M && std::असल(m[10]) < MAX_M*256 && std::असल(m[11]) < MAX_M*256 )
    {
        स्थिरांक पूर्णांक nChannels = 3;

        संघ {
            लघु s[6];
            पूर्णांक p[3];
        } m16;
        m16.s[0] = saturate_cast<लघु>(m[0] * SCALE); m16.s[1] = saturate_cast<लघु>(m[1] * SCALE);
        m16.s[2] = saturate_cast<लघु>(m[4] * SCALE); m16.s[3] = saturate_cast<लघु>(m[5] * SCALE);
        m16.s[4] = saturate_cast<लघु>(m[8] * SCALE); m16.s[5] = saturate_cast<लघु>(m[9] * SCALE);
        पूर्णांक m32[] = {saturate_cast<पूर्णांक>(m[ 2] * SCALE), saturate_cast<पूर्णांक>(m[ 3] * SCALE),
                     saturate_cast<पूर्णांक>(m[ 6] * SCALE), saturate_cast<पूर्णांक>(m[ 7] * SCALE),
                     saturate_cast<पूर्णांक>(m[10] * SCALE), saturate_cast<पूर्णांक>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        पूर्णांक x = 0;
        साठी (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<पूर्णांक>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<पूर्णांक>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<पूर्णांक>((m[11] + 0.5f)*SCALE);
        साठी( ; x < len * nChannels; x += nChannels )
        {
            पूर्णांक v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        परत;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_16s(स्थिरांक लघु* src, लघु* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

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
        साठी (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        साठी( ; x < len * 3; x += 3 )
        {
            दशांश v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            लघु t0 = saturate_cast<लघु>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            लघु t1 = saturate_cast<लघु>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            लघु t2 = saturate_cast<लघु>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        परत;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_32s(स्थिरांक पूर्णांक* src, पूर्णांक* dst, स्थिरांक द्विगुण* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    जर( scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

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
        साठी (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_वरचा(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_वरचा(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_वरचा(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        साठी (; x < len * 3; x += 3) {
            द्विगुण b = src[x], g = src[x + 1], r = src[x + 2];
            पूर्णांक db = saturate_cast<पूर्णांक>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            पूर्णांक dg = saturate_cast<पूर्णांक>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            पूर्णांक dr = saturate_cast<पूर्णांक>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_64f(स्थिरांक द्विगुण* src, द्विगुण* dst, स्थिरांक द्विगुण* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    जर( scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

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
        साठी (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        साठी (; x < len * 3; x += 3) {
            द्विगुण b = src[x], g = src[x + 1], r = src[x + 2];
            द्विगुण db = saturate_cast<द्विगुण>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            द्विगुण dg = saturate_cast<द्विगुण>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            द्विगुण dr = saturate_cast<द्विगुण>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

ढाँचा<typename T, typename WT> स्थिर शून्य
diagtransform_( स्थिरांक T* src, T* dst, स्थिरांक WT* m, पूर्णांक len, पूर्णांक cn, पूर्णांक )
{
    पूर्णांक x;

    जर( cn == 2 )
    {
        साठी( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    अन्यथा जर( cn == 3 )
    {
        साठी( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    अन्यथा जर( cn == 4 )
    {
        साठी( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    अन्यथा
    {
        साठी( x = 0; x < len; x++, src += cn, dst += cn )
        {
            स्थिरांक WT* _m = m;
            साठी( पूर्णांक j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

स्थिर शून्य
diagtransform_8u(स्थिरांक uchar* src, uchar* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        साठी (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        साठी (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_8s(स्थिरांक schar* src, schar* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
    #जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        साठी (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        साठी (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_16u(स्थिरांक ushort* src, ushort* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        साठी (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        साठी (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_16s(स्थिरांक लघु* src, लघु* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        साठी (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        साठी (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            लघु db = saturate_cast<लघु>(m[ 0] * b + m[ 3]);
            लघु dg = saturate_cast<लघु>(m[ 5] * g + m[ 7]);
            लघु dr = saturate_cast<लघु>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_32s(स्थिरांक पूर्णांक* src, पूर्णांक* dst, स्थिरांक द्विगुण* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    जर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        साठी (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_वरचा(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_वरचा(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_वरचा(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        साठी (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            पूर्णांक db = saturate_cast<पूर्णांक>(m[ 0] * b + m[ 3]);
            पूर्णांक dg = saturate_cast<पूर्णांक>(m[ 5] * g + m[ 7]);
            पूर्णांक dr = saturate_cast<पूर्णांक>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_32f(स्थिरांक दशांश* src, दशांश* dst, स्थिरांक दशांश* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        साठी (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        साठी (; x < len * 3; x += 3) {
            दशांश b = src[x], g = src[x + 1], r = src[x + 2];
            दशांश db = saturate_cast<दशांश>(m[ 0] * b + m[ 3]);
            दशांश dg = saturate_cast<दशांश>(m[ 5] * g + m[ 7]);
            दशांश dr = saturate_cast<दशांश>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_64f(स्थिरांक द्विगुण* src, द्विगुण* dst, स्थिरांक द्विगुण* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    जर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        साठी (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        साठी (; x < len * 3; x += 3) {
            द्विगुण b = src[x], g = src[x + 1], r = src[x + 2];
            द्विगुण db = saturate_cast<द्विगुण>(m[ 0] * b + m[ 3]);
            द्विगुण dg = saturate_cast<द्विगुण>(m[ 5] * g + m[ 7]);
            द्विगुण dr = saturate_cast<द्विगुण>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        परत;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


ढाँचा<typename T, typename WT, शून्य (*fn)(स्थिरांक T*, T*, स्थिरांक WT*, पूर्णांक, पूर्णांक, पूर्णांक)>
स्थिर शून्य transformWrap(स्थिरांक शून्य* src, शून्य* dst, स्थिरांक शून्य* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
    fn((स्थिरांक T*)src, (T*)dst, (स्थिरांक WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(पूर्णांक depth)
{
    स्थिर TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, दशांश, transform_8u>, transformWrap<schar, दशांश, transform_8s>, transformWrap<ushort, दशांश, transform_16u>,
        transformWrap<लघु, दशांश, transform_16s>, transformWrap<पूर्णांक, द्विगुण, transform_32s>, transformWrap<दशांश, दशांश, transform_32f>,
        transformWrap<द्विगुण, द्विगुण, transform_64f>, 0
    };

    परत transformTab[depth];
}

TransformFunc getDiagTransformFunc(पूर्णांक depth)
{
    स्थिर TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, दशांश, diagtransform_8u>, transformWrap<schar, दशांश, diagtransform_8s>, transformWrap<ushort, दशांश, diagtransform_16u>,
        transformWrap<लघु, दशांश, diagtransform_16s>, transformWrap<पूर्णांक, द्विगुण, diagtransform_32s>, transformWrap<दशांश, दशांश, diagtransform_32f>,
        transformWrap<द्विगुण, द्विगुण, diagtransform_64f>, 0
    };

    परत diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ढाँचा<typename T> स्थिर शून्य
perspectiveTransform_( स्थिरांक शून्य* _src, शून्य* _dst, स्थिरांक शून्य* _m_ptr, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
    स्थिरांक T* src = (स्थिरांक T*)_src;
    T* dst = (T*)_dst;
    स्थिरांक द्विगुण* m = (स्थिरांक द्विगुण*)_m_ptr;
    स्थिरांक द्विगुण eps = भग्न_अंतर;
    पूर्णांक i;

    जर( scn == 2 && dcn == 2 )
    {
        साठी( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            द्विगुण w = x*m[6] + y*m[7] + m[8];

            जर( भ_असल(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            अन्यथा
                dst[i] = dst[i+1] = (T)0;
        }
    }
    अन्यथा जर( scn == 3 && dcn == 3 )
    {
        साठी( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            द्विगुण w = x*m[12] + y*m[13] + z*m[14] + m[15];

            जर( भ_असल(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            अन्यथा
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    अन्यथा जर( scn == 3 && dcn == 2 )
    {
        साठी( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            द्विगुण w = x*m[8] + y*m[9] + z*m[10] + m[11];

            जर( भ_असल(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            अन्यथा
                dst[0] = dst[1] = (T)0;
        }
    }
    अन्यथा
    {
        साठी( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            स्थिरांक द्विगुण* _m = m + dcn*(scn + 1);
            द्विगुण w = _m[scn];
            पूर्णांक j, k;
            साठी( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            जर( भ_असल(w) > eps )
            {
                _m = m;
                साठी( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    द्विगुण s = _m[scn];
                    साठी( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            अन्यथा
                साठी( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(पूर्णांक depth)
{
    जर (depth == CV_32F)
        परत perspectiveTransform_<दशांश>;
    जर (depth == CV_64F)
        परत perspectiveTransform_<द्विगुण>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

स्थिर शून्य scaleAdd_32f(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, शून्य* _dst,
                         पूर्णांक len, स्थिरांक शून्य* _alpha)
{
    स्थिरांक दशांश* src1 = (स्थिरांक दशांश*)_src1;
    स्थिरांक दशांश* src2 = (स्थिरांक दशांश*)_src2;
    दशांश* dst = (दशांश*)_dst;
    दशांश alpha = *(स्थिरांक दशांश*)_alpha;
    पूर्णांक i = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    स्थिरांक पूर्णांक cWidth = VTraits<v_float32>::vlanes();
    साठी (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    साठी (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


स्थिर शून्य scaleAdd_64f(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, शून्य* _dst,
                         पूर्णांक len, स्थिरांक शून्य* _alpha)
{
    स्थिरांक द्विगुण* src1 = (स्थिरांक द्विगुण*)_src1;
    स्थिरांक द्विगुण* src2 = (स्थिरांक द्विगुण*)_src2;
    द्विगुण* dst = (द्विगुण*)_dst;
    द्विगुण alpha = *(स्थिरांक द्विगुण*)_alpha;
    पूर्णांक i = 0;
#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    स्थिरांक पूर्णांक cWidth = VTraits<v_float64>::vlanes();
    साठी (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    साठी (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(पूर्णांक depth)
{
    जर (depth == CV_32F)
        परत scaleAdd_32f;
    जर (depth == CV_64F)
        परत scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ढाँचा<typename T> स्थिर ओळीत
द्विगुण MahalanobisImpl(स्थिरांक Mat& v1, स्थिरांक Mat& v2, स्थिरांक Mat& icovar, द्विगुण *diff_buffer /*[len]*/, पूर्णांक len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    द्विगुण result = 0;

    sz.width *= v1.channels();
    जर (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        स्थिरांक T* src1 = v1.ptr<T>();
        स्थिरांक T* src2 = v2.ptr<T>();
        माप_प्रकार step1 = v1.step/आकार(src1[0]);
        माप_प्रकार step2 = v2.step/आकार(src2[0]);
        द्विगुण* diff = diff_buffer;
        स्थिरांक T* mat = icovar.ptr<T>();
        माप_प्रकार matstep = icovar.step/आकार(mat[0]);

        साठी (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            साठी (पूर्णांक i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        साठी (पूर्णांक i = 0; i < len; i++, mat += matstep)
        {
            द्विगुण row_sum = 0;
            पूर्णांक j = 0;
#जर CV_ENABLE_UNROLLED
            साठी(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            साठी (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    परत result;
}

MahalanobisImplFunc getMahalanobisImplFunc(पूर्णांक depth)
{
    जर (depth == CV_32F)
        परत (MahalanobisImplFunc)MahalanobisImpl<दशांश>;
    जर (depth == CV_64F)
        परत (MahalanobisImplFunc)MahalanobisImpl<द्विगुण>;
    CV_Assert(0 && "Not supported");
}


#जर !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ढाँचा<typename sT, typename dT> स्थिर शून्य
MulTransposedR(स्थिरांक Mat& srcmat, स्थिरांक Mat& dstmat, स्थिरांक Mat& deltamat, द्विगुण scale)
{
    पूर्णांक i, j, k;
    स्थिरांक sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    स्थिरांक dT* delta = deltamat.ptr<dT>();
    माप_प्रकार srcstep = srcmat.step/आकार(src[0]);
    माप_प्रकार dststep = dstmat.step/आकार(dst[0]);
    माप_प्रकार deltastep = deltamat.rows > 1 ? deltamat.step/आकार(delta[0]) : 0;
    पूर्णांक delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    पूर्णांक buf_size = size.height*आकार(dT);
    AutoBuffer<uchar> buf;

    जर( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    जर( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        साठी( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#जर CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    जर( !delta )
        साठी( i = 0; i < size.width; i++, tdst += dststep )
        {
            साठी( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            साठी( j = i; j <= size.width - 4; j += 4 )
            {
#जर CV_SIMD128_64F
                जर (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    स्थिरांक द्विगुण *tsrc = (द्विगुण*)(src + j);

                    साठी( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((द्विगुण)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((द्विगुण*)(tdst+j), v_mul(s0, v_scale));
                    v_store((द्विगुण*)(tdst+j+2), v_mul(s1, v_scale));
                } अन्यथा
#endif
                {
                    द्विगुण s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    स्थिरांक sT *tsrc = src + j;

                    साठी( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        द्विगुण a = col_buf[k];
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

            साठी( ; j < size.width; j++ )
            {
                द्विगुण s0 = 0;
                स्थिरांक sT *tsrc = src + j;

                साठी( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (द्विगुण)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    अन्यथा
        साठी( i = 0; i < size.width; i++, tdst += dststep )
        {
            जर( !delta_buf )
                साठी( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            अन्यथा
                साठी( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            साठी( j = i; j <= size.width - 4; j += 4 )
            {
#जर CV_SIMD128_64F
                जर (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    स्थिरांक द्विगुण *tsrc = (द्विगुण*)(src + j);
                    स्थिरांक द्विगुण *d = (द्विगुण*)(delta_buf ? delta_buf : delta + j);

                    साठी( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((द्विगुण)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((द्विगुण*)(tdst+j), v_mul(s0, v_scale));
                    v_store((द्विगुण*)(tdst+j+2), v_mul(s1, v_scale));
                }
                अन्यथा
#endif

                {
                    द्विगुण s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    स्थिरांक sT *tsrc = src + j;
                    स्थिरांक dT *d = delta_buf ? delta_buf : delta + j;

                    साठी( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        द्विगुण a = col_buf[k];
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

            साठी( ; j < size.width; j++ )
            {
                द्विगुण s0 = 0;
                स्थिरांक sT *tsrc = src + j;
                स्थिरांक dT *d = delta_buf ? delta_buf : delta + j;

                साठी( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (द्विगुण)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


ढाँचा<typename sT, typename dT> स्थिर शून्य
MulTransposedL(स्थिरांक Mat& srcmat, स्थिरांक Mat& dstmat, स्थिरांक Mat& deltamat, द्विगुण scale)
{
    पूर्णांक i, j, k;
    स्थिरांक sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    स्थिरांक dT* delta = deltamat.ptr<dT>();
    माप_प्रकार srcstep = srcmat.step/आकार(src[0]);
    माप_प्रकार dststep = dstmat.step/आकार(dst[0]);
    माप_प्रकार deltastep = deltamat.rows > 1 ? deltamat.step/आकार(delta[0]) : 0;
    पूर्णांक delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    जर( !delta )
        साठी( i = 0; i < size.height; i++, tdst += dststep )
            साठी( j = i; j < size.height; j++ )
            {
                द्विगुण s = 0;
                स्थिरांक sT *tsrc1 = src + i*srcstep;
                स्थिरांक sT *tsrc2 = src + j*srcstep;
#जर CV_SIMD128_64F
                जर (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    स्थिरांक द्विगुण *v_tsrc1 = (द्विगुण *)(tsrc1);
                    स्थिरांक द्विगुण *v_tsrc2 = (द्विगुण *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    साठी( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                अन्यथा
#endif
                {
                    साठी( k = 0; k <= size.width - 4; k += 4 )
                        s += (द्विगुण)tsrc1[k]*tsrc2[k] + (द्विगुण)tsrc1[k+1]*tsrc2[k+1] +
                             (द्विगुण)tsrc1[k+2]*tsrc2[k+2] + (द्विगुण)tsrc1[k+3]*tsrc2[k+3];
                }
                साठी( ; k < size.width; k++ )
                    s += (द्विगुण)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    अन्यथा
    {
        dT delta_buf[4];
        पूर्णांक delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*आकार(dT));
        dT* row_buf = (dT*)buf.data();

        साठी( i = 0; i < size.height; i++, tdst += dststep )
        {
            स्थिरांक sT *tsrc1 = src + i*srcstep;
            स्थिरांक dT *tdelta1 = delta + i*deltastep;

            जर( delta_cols < size.width )
                साठी( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            अन्यथा
                साठी( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            साठी( j = i; j < size.height; j++ )
            {
                द्विगुण s = 0;
                स्थिरांक sT *tsrc2 = src + j*srcstep;
                स्थिरांक dT *tdelta2 = delta + j*deltastep;
                जर( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#जर CV_SIMD128_64F
                जर (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    स्थिरांक द्विगुण *v_tsrc2 = (द्विगुण *)(tsrc2);
                    स्थिरांक द्विगुण *v_tdelta2 = (द्विगुण *)(tdelta2);
                    स्थिरांक द्विगुण *v_row_buf = (द्विगुण *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    साठी( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (स्थिरांक dT *)(v_tdelta2);
                }
                अन्यथा
#endif
                {
                    साठी( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (द्विगुण)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (द्विगुण)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (द्विगुण)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (द्विगुण)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                साठी( ; k < size.width; k++, tdelta2++ )
                    s += (द्विगुण)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(पूर्णांक stype, पूर्णांक dtype, तार्किक ata)
{
    MulTransposedFunc func = रिक्त;
    जर (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,दशांश>
                   : MulTransposedL<uchar,दशांश>;
    }
    अन्यथा जर (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,द्विगुण>
                   : MulTransposedL<uchar,द्विगुण>;
    }
    अन्यथा जर(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,दशांश>
                   : MulTransposedL<ushort,दशांश>;
    }
    अन्यथा जर(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,द्विगुण>
                   : MulTransposedL<ushort,द्विगुण>;
    }
    अन्यथा जर(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<लघु,दशांश>
                   : MulTransposedL<लघु,दशांश>;
    }
    अन्यथा जर(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<लघु,द्विगुण>
                   : MulTransposedL<लघु,द्विगुण>;
    }
    अन्यथा जर(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<दशांश,दशांश>
                   : MulTransposedL<दशांश,दशांश>;
    }
    अन्यथा जर(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<दशांश,द्विगुण>
                   : MulTransposedL<दशांश,द्विगुण>;
    }
    अन्यथा जर(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<द्विगुण,द्विगुण>
                   : MulTransposedL<द्विगुण,द्विगुण>;
    }
    CV_Assert(func && "Not supported");
    परत func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ढाँचा<typename T> स्थिर ओळीत
द्विगुण dotProd_(स्थिरांक T* src1, स्थिरांक T* src2, पूर्णांक len)
{
    पूर्णांक i = 0;
    द्विगुण result = 0;

#जर CV_ENABLE_UNROLLED
    साठी( ; i <= len - 4; i += 4 )
        result += (द्विगुण)src1[i]*src2[i] + (द्विगुण)src1[i+1]*src2[i+1] +
            (द्विगुण)src1[i+2]*src2[i+2] + (द्विगुण)src1[i+3]*src2[i+3];
#endif
    साठी( ; i < len; i++ )
        result += (द्विगुण)src1[i]*src2[i];

    परत result;
}


द्विगुण dotProd_8u(स्थिरांक uchar* src1, स्थिरांक uchar* src2, पूर्णांक len)
{
    द्विगुण r = 0;
    पूर्णांक i = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    पर्यंत (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        स्थिरांक पूर्णांक cWidth = VTraits<v_uint16>::vlanes();

        पूर्णांक j = 0;
        साठी (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        साठी (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (द्विगुण)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    परत r + dotProd_(src1, src2, len - i);
}


द्विगुण dotProd_8s(स्थिरांक schar* src1, स्थिरांक schar* src2, पूर्णांक len)
{
    द्विगुण r = 0.0;
    पूर्णांक i = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    पर्यंत (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        स्थिरांक पूर्णांक cWidth = VTraits<v_int16>::vlanes();

        पूर्णांक j = 0;
        साठी (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        साठी (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (द्विगुण)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    परत r + dotProd_(src1, src2, len - i);
}

द्विगुण dotProd_16u(स्थिरांक ushort* src1, स्थिरांक ushort* src2, पूर्णांक len)
{
    द्विगुण r = 0.0;
    पूर्णांक i = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    पर्यंत (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        स्थिरांक पूर्णांक cWidth = VTraits<v_uint16>::vlanes();

        पूर्णांक j = 0;
        साठी (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (द्विगुण)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    परत r + dotProd_(src1, src2, len - i);
}

द्विगुण dotProd_16s(स्थिरांक लघु* src1, स्थिरांक लघु* src2, पूर्णांक len)
{
    द्विगुण r = 0.0;
    पूर्णांक i = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    पर्यंत (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        स्थिरांक पूर्णांक cWidth = VTraits<v_int16>::vlanes();

        पूर्णांक j = 0;
        साठी (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (द्विगुण)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    परत r + dotProd_(src1, src2, len - i);
}

द्विगुण dotProd_32s(स्थिरांक पूर्णांक* src1, स्थिरांक पूर्णांक* src2, पूर्णांक len)
{
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    द्विगुण r = .0;
    पूर्णांक i = 0;
    स्थिरांक पूर्णांक step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#जर CV_SIMD_WIDTH == 16
    स्थिरांक पूर्णांक wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    साठी (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    साठी (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    परत r + dotProd_(src1, src2, len - i);
#अन्यथा
    परत dotProd_(src1, src2, len);
#endif
}

द्विगुण dotProd_32f(स्थिरांक दशांश* src1, स्थिरांक दशांश* src2, पूर्णांक len)
{
    द्विगुण r = 0.0;
    पूर्णांक i = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    पर्यंत (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        पूर्णांक j = 0;
        पूर्णांक cWidth = VTraits<v_float32>::vlanes();

#जर CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        साठी (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
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

        साठी (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    परत r + dotProd_(src1, src2, len - i);
}

द्विगुण dotProd_64f(स्थिरांक द्विगुण* src1, स्थिरांक द्विगुण* src2, पूर्णांक len)
{
    परत dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
