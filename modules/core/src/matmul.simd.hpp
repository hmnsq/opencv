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

#यदि_परिभाषितम् HAVE_LAPACK
#परिभाषय CV_GEMM_BASELINE_ONLY
#यदि_अन्तः
#परिभाषय CV_MAHALANOBIS_BASELINE_ONLY
#परिभाषय CV_MULTRANSPOSED_BASELINE_ONLY

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
#यदि_अपरिभाषितम् CV_GEMM_SIMD_ENABLED
#यदि defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#परिभाषय CV_GEMM_SIMD_ENABLED 0
#अन्यथा
#परिभाषय CV_GEMM_SIMD_ENABLED 1
#यदि_अन्तः
#यदि_अन्तः

नामाकाश cv {

// forward declarations
प्रकारनिर्देश शून्य (*TransformFunc)(ध्रुव शून्य* src, शून्य* dst, ध्रुव शून्य* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn);
प्रकारनिर्देश शून्य (*Scale_सङ्कलय_Func)(ध्रुव शून्य* src1, ध्रुव शून्य* src2, शून्य* dst, पूर्णाङ्क len, ध्रुव शून्य* alpha);
प्रकारनिर्देश शून्य (*MulTransposedFunc)(ध्रुव Mat& src, ध्रुव/*preallocated*/ Mat& dst, ध्रुव Mat& delta, द्विगुण scale);
प्रकारनिर्देश द्विगुण (*MahalanobisImplFunc)(ध्रुव Mat& v1, ध्रुव Mat& v2, ध्रुव Mat& icovar, द्विगुण *diff_बफर /*[len]*/, पूर्णाङ्क len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

शून्य gemm32f(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
             भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
             पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags);
शून्य gemm64f(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
             द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
             पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags);
शून्य gemm32fc(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
              भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
              पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags);
शून्य gemm64fc(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
              द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
              पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags);

TransformFunc getTransformFunc(पूर्णाङ्क गभीरता);
TransformFunc getDiagTransformFunc(पूर्णाङ्क गभीरता);
TransformFunc getPerspectiveTransform(पूर्णाङ्क गभीरता);

Scale_सङ्कलय_Func get_Scale_सङ्कलय_Func(पूर्णाङ्क गभीरता);

MahalanobisImplFunc getMahalanobisImplFunc(पूर्णाङ्क गभीरता);

MulTransposedFunc getMulTransposedFunc(पूर्णाङ्क stype, पूर्णाङ्क dtype, तार्किक ata);

द्विगुण dotProd_8u(ध्रुव uchar* src1, ध्रुव uchar* src2, पूर्णाङ्क len);
द्विगुण dotProd_8s(ध्रुव schar* src1, ध्रुव schar* src2, पूर्णाङ्क len);
द्विगुण dotProd_16u(ध्रुव ushort* src1, ध्रुव ushort* src2, पूर्णाङ्क len);
द्विगुण dotProd_16s(ध्रुव ह्रस्व* src1, ध्रुव ह्रस्व* src2, पूर्णाङ्क len);
द्विगुण dotProd_32s(ध्रुव पूर्णाङ्क* src1, ध्रुव पूर्णाङ्क* src2, पूर्णाङ्क len);
द्विगुण dotProd_32f(ध्रुव भिन्न* src1, ध्रुव भिन्न* src2, पूर्णाङ्क len);
द्विगुण dotProd_64f(ध्रुव द्विगुण* src1, ध्रुव द्विगुण* src2, पूर्णाङ्क len);



#यदि_अपरिभाषितम् CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#यदि !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_बहुलकम्)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

स्थिर शून्य
GEMM_Copy_खण्डम्( ध्रुव uchar* src, size_t src_चरणः,
                uchar* dst, size_t dst_चरणः,
                आकार size, size_t pix_आकार )
{
    पूर्णाङ्क j;
    size.विस्तारः *= (पूर्णाङ्क)(pix_आकार / परिमाण(पूर्णाङ्क));

    प्रति( ; size.height--; src += src_चरणः, dst += dst_चरणः )
    {
        j=0;
         #यदि CV_ENABLE_UNROLLED
        प्रति( ; j <= size.विस्तारः - 4; j += 4 )
        {
            पूर्णाङ्क t0 = ((ध्रुव पूर्णाङ्क*)src)[j];
            पूर्णाङ्क t1 = ((ध्रुव पूर्णाङ्क*)src)[j+1];
            ((पूर्णाङ्क*)dst)[j] = t0;
            ((पूर्णाङ्क*)dst)[j+1] = t1;
            t0 = ((ध्रुव पूर्णाङ्क*)src)[j+2];
            t1 = ((ध्रुव पूर्णाङ्क*)src)[j+3];
            ((पूर्णाङ्क*)dst)[j+2] = t0;
            ((पूर्णाङ्क*)dst)[j+3] = t1;
        }
        #यदि_अन्तः
        प्रति( ; j < size.विस्तारः; j++ )
            ((पूर्णाङ्क*)dst)[j] = ((ध्रुव पूर्णाङ्क*)src)[j];
    }
}


स्थिर शून्य
GEMM_Transpose_खण्डम्( ध्रुव uchar* src, size_t src_चरणः,
                     uchar* dst, size_t dst_चरणः,
                     आकार size, size_t pix_आकार )
{
    पूर्णाङ्क i, j;
    प्रति( i = 0; i < size.विस्तारः; i++, dst += dst_चरणः, src += pix_आकार )
    {
        ध्रुव uchar* _src = src;
        विकल्प( pix_आकार )
        {
        पक्ष परिमाण(पूर्णाङ्क):
            प्रति( j = 0; j < size.height; j++, _src += src_चरणः )
                ((पूर्णाङ्क*)dst)[j] = ((पूर्णाङ्क*)_src)[0];
            भङ्ग;
        पक्ष परिमाण(पूर्णाङ्क)*2:
            प्रति( j = 0; j < size.height*2; j += 2, _src += src_चरणः )
            {
                पूर्णाङ्क t0 = ((पूर्णाङ्क*)_src)[0];
                पूर्णाङ्क t1 = ((पूर्णाङ्क*)_src)[1];
                ((पूर्णाङ्क*)dst)[j] = t0;
                ((पूर्णाङ्क*)dst)[j+1] = t1;
            }
            भङ्ग;
        पक्ष परिमाण(पूर्णाङ्क)*4:
            प्रति( j = 0; j < size.height*4; j += 4, _src += src_चरणः )
            {
                पूर्णाङ्क t0 = ((पूर्णाङ्क*)_src)[0];
                पूर्णाङ्क t1 = ((पूर्णाङ्क*)_src)[1];
                ((पूर्णाङ्क*)dst)[j] = t0;
                ((पूर्णाङ्क*)dst)[j+1] = t1;
                t0 = ((पूर्णाङ्क*)_src)[2];
                t1 = ((पूर्णाङ्क*)_src)[3];
                ((पूर्णाङ्क*)dst)[j+2] = t0;
                ((पूर्णाङ्क*)dst)[j+3] = t1;
            }
            भङ्ग;
        स्वभाव:
            CV_Assert(0);
            प्रत्यागम;
        }
    }
}

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
प्रतिरूपम्<प्रकारनाम ST> संरचना VecTraits;

प्रतिरूपम्<> संरचना VecTraits<भिन्न> {
    प्रकारनिर्देश v_float32 VT;
    स्थिर अन्तःस्थ VT zero() { प्रत्यागम vx_setzero_f32(); }
    स्थिर अन्तःस्थ VT load(ध्रुव भिन्न* p) { प्रत्यागम vx_आरोपय(p); }
    स्थिर अन्तःस्थ VT setall(भिन्न v) { प्रत्यागम vx_setall_f32(v); }
};
#यदि_अन्तः

#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
प्रतिरूपम्<> संरचना VecTraits<द्विगुण> {
    प्रकारनिर्देश v_float64 VT;
    स्थिर अन्तःस्थ VT zero() { प्रत्यागम vx_setzero_f64(); }
    स्थिर अन्तःस्थ VT load(ध्रुव द्विगुण* p) { प्रत्यागम vx_आरोपय(p); }
    स्थिर अन्तःस्थ VT setall(द्विगुण v) { प्रत्यागम vx_setall_f64(v); }
};
#यदि_अन्तः

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
प्रतिरूपम्<प्रकारनाम ST>
स्थिर अन्तःस्थ ST simd_Dot_गुणनफलम्(ध्रुव ST* a, ध्रुव ST* b, पूर्णाङ्क n, पूर्णाङ्क& k)
{
    प्रकारनिर्देश प्रकारनाम VecTraits<ST>::VT VecT;
    ध्रुव पूर्णाङ्क vlanes = VTraits<VecT>::vlanes();
    ध्रुव पूर्णाङ्क stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    प्रति( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_सङ्कलय(v_सङ्कलय(vs0, vs1), v_सङ्कलय(vs2, vs3));
    प्रति( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    प्रत्यागम v_reduce_योग(vs0);
}
#यदि_अन्तः

#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
स्थिर अन्तःस्थ द्विगुण simd_Dot_गुणनफलम्_f32f64(ध्रुव भिन्न* a, ध्रुव भिन्न* b, पूर्णाङ्क n, पूर्णाङ्क& k)
{
    ध्रुव पूर्णाङ्क vlanes_f32 = VTraits<v_float32>::vlanes();
    ध्रुव पूर्णाङ्क stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    प्रति( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_आरोपय(a + k);
        v_float32 vb0 = vx_आरोपय(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_उच्चः(va0), v_cvt_f64_उच्चः(vb0), vs1);

        v_float32 va1 = vx_आरोपय(a + k + vlanes_f32);
        v_float32 vb1 = vx_आरोपय(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_उच्चः(va1), v_cvt_f64_उच्चः(vb1), vs3);
    }
    vs0 = v_सङ्कलय(v_सङ्कलय(vs0, vs1), v_सङ्कलय(vs2, vs3));
    प्रति( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_आरोपय(a + k);
        v_float32 vb = vx_आरोपय(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_उच्चः(va), v_cvt_f64_उच्चः(vb), vs0);
    }
    प्रत्यागम v_reduce_योग(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
स्थिर अन्तःस्थ शून्य simdGEMM_kj_f32f64(
    ध्रुव भिन्न* a_दत्त, ध्रुव भिन्न* b_दत्त_2, size_t b_चरणः,
    ध्रुव भिन्न* c_दत्त, size_t c_step1,
    भिन्न* d_दत्त,
    पूर्णाङ्क n, पूर्णाङ्क m,
    द्विगुण alpha, द्विगुण beta,
    पूर्णाङ्क& j)
{
    ध्रुव पूर्णाङ्क vlanes_f32 = VTraits<v_float32>::vlanes();
    ध्रुव पूर्णाङ्क vlanes_f64 = VTraits<v_float64>::vlanes();

    यदि( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        ध्रुव पूर्णाङ्क max_m = 1600 / (पूर्णाङ्क)परिमाण(भिन्न);
        cv::Auto_बफर<द्विगुण> _s_buf(max_m);
        द्विगुण* s_buf = _s_buf.दत्त();

        {
            v_float64 vz = vx_setzero_f64();
            प्रति( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            प्रति( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            ध्रुव भिन्न* b_ptr = b_दत्त_2;
            प्रति( पूर्णाङ्क k = 0; k < n; k++, b_ptr += b_चरणः )
            {
                v_float64 va = vx_setall_f64((द्विगुण)a_दत्त[k]);
                j = 0;
                प्रति( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_आरोपय(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_उच्चः(vb);
                    v_float64 vd_lo = vx_आरोपय(s_buf + j);
                    v_float64 vd_hi = vx_आरोपय(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                द्विगुण a_val = (द्विगुण)a_दत्त[k];
                प्रति( ; j < m; j++ )
                    s_buf[j] += a_val * (द्विगुण)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            ध्रुव भिन्न* c_दत्त_2 = c_दत्त;
            j = 0;
            यदि( !c_दत्त )
            {
                प्रति( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_आरोपय(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_आरोपय(s_buf + j + vlanes_f64), valpha);
                    v_store(d_दत्त + j, v_cvt_f32(vs_lo, vs_hi));
                }
                प्रति( ; j < m; j++ )
                    d_दत्त[j] = (भिन्न)(s_buf[j] * alpha);
            }
            अन्यथा यदि( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                प्रति( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_आरोपय(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_आरोपय(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_आरोपय(c_दत्त_2 + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_उच्चः(vc), vbeta, vs_hi);
                    v_store(d_दत्त + j, v_cvt_f32(vs_lo, vs_hi));
                }
                प्रति( ; j < m; j++ )
                    d_दत्त[j] = (भिन्न)(s_buf[j] * alpha + (द्विगुण)c_दत्त_2[j] * beta);
            }
            अन्यथा
            {
                प्रति( j = 0; j < m; j++, c_दत्त_2 += c_step1 )
                    d_दत्त[j] = (भिन्न)(s_buf[j] * alpha + (द्विगुण)c_दत्त_2[0] * beta);
            }
        }
        j = m;
    }
    अन्यथा यदि( vlanes_f32 * (पूर्णाङ्क)परिमाण(भिन्न) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        ध्रुव भिन्न* c_दत्त_2 = c_दत्त;
        प्रति( ; j <= m - vlanes_f32; j += vlanes_f32, c_दत्त_2 += vlanes_f32 * c_step1 )
        {
            ध्रुव भिन्न* b = b_दत्त_2 + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            प्रति( पूर्णाङ्क k = 0; k < n; k++, b += b_चरणः )
            {
                v_float64 va = vx_setall_f64((द्विगुण)a_दत्त[k]);
                v_float32 vb = vx_आरोपय(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_उच्चः(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            यदि( !c_दत्त )
            {
                v_store(d_दत्त + j, v_cvt_f32(vs_lo, vs_hi));
            }
            अन्यथा यदि( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_आरोपय(c_दत्त_2);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_उच्चः(vc), vbeta, vs_hi);
                v_store(d_दत्त + j, v_cvt_f32(vs_lo, vs_hi));
            }
            अन्यथा
            {
                CV_DECL_ALIGNED(CV_SIMD_विस्तारः) भिन्न buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                प्रति( पूर्णाङ्क jj = 0; jj < vlanes_f32; jj++ )
                    d_दत्त[j + jj] = buf[jj] + c_दत्त_2[jj * c_step1] * (भिन्न)beta;
            }
        }
    }
}
#यदि_अन्तः

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
प्रतिरूपम्<प्रकारनाम ST>
स्थिर अन्तःस्थ शून्य simdGEMM_kj(
    ध्रुव ST* a_दत्त, ध्रुव ST* b_दत्त_2, size_t b_चरणः,
    ध्रुव ST* c_दत्त, size_t c_step1,
    ST* d_दत्त,
    पूर्णाङ्क n, पूर्णाङ्क m,
    द्विगुण alpha, द्विगुण beta,
    पूर्णाङ्क& j)
{
    प्रकारनिर्देश प्रकारनाम VecTraits<ST>::VT VecT;
    ध्रुव पूर्णाङ्क vlanes = VTraits<VecT>::vlanes();

    यदि( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        ध्रुव पूर्णाङ्क max_m = 1600 / (पूर्णाङ्क)परिमाण(ST);
        CV_DECL_ALIGNED(CV_SIMD_विस्तारः) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            प्रति( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            प्रति( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            ध्रुव ST* b_ptr = b_दत्त_2;
            प्रति( पूर्णाङ्क k = 0; k < n; k++, b_ptr += b_चरणः )
            {
                VecT va = VecTraits<ST>::setall(a_दत्त[k]);
                j = 0;
                प्रति( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_आरोपय_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_दत्त[k];
                प्रति( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            ध्रुव ST* c_दत्त_2 = c_दत्त;
            j = 0;
            यदि( !c_दत्त )
            {
                प्रति( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_आरोपय_aligned(s_buf + j), valpha);
                    v_store(d_दत्त + j, vs);
                }
                प्रति( ; j < m; j++ )
                    d_दत्त[j] = s_buf[j] * (ST)alpha;
            }
            अन्यथा यदि( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                प्रति( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_आरोपय_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_दत्त_2 + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_दत्त + j, vs);
                }
                प्रति( ; j < m; j++ )
                    d_दत्त[j] = s_buf[j] * (ST)alpha + c_दत्त_2[j] * (ST)beta;
            }
            अन्यथा
            {
                प्रति( j = 0; j < m; j++, c_दत्त_2 += c_step1 )
                    d_दत्त[j] = s_buf[j] * (ST)alpha + c_दत्त_2[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    अन्यथा यदि( vlanes * (पूर्णाङ्क)परिमाण(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        ध्रुव ST* c_दत्त_2 = c_दत्त;
        प्रति( ; j <= m - vlanes; j += vlanes, c_दत्त_2 += vlanes * c_step1 )
        {
            ध्रुव ST* b = b_दत्त_2 + j;
            VecT vs = VecTraits<ST>::zero();
            प्रति( पूर्णाङ्क k = 0; k < n; k++, b += b_चरणः )
            {
                VecT va = VecTraits<ST>::setall(a_दत्त[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            यदि( !c_दत्त )
            {
                v_store(d_दत्त + j, vs);
            }
            अन्यथा यदि( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_दत्त_2);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_दत्त + j, vs);
            }
            अन्यथा
            {
                CV_DECL_ALIGNED(CV_SIMD_विस्तारः) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                प्रति( पूर्णाङ्क jj = 0; jj < vlanes; jj++ )
                    d_दत्त[j + jj] = buf[jj] + c_दत्त_2[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#यदि_अन्तः


प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT> स्थिर शून्य
GEMMSingleMul( ध्रुव T* a_दत्त, size_t a_चरणः,
               ध्रुव T* b_दत्त, size_t b_चरणः,
               ध्रुव T* c_दत्त_2, size_t c_चरणः,
               T* d_दत्त, size_t d_चरणः,
               आकार a_आकार, आकार d_आकार,
               द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    पूर्णाङ्क i, j, k, n = a_आकार.विस्तारः, m = d_आकार.विस्तारः, drows = d_आकार.height;
    ध्रुव T *a_दत्त_2 = a_दत्त, *b_दत्त_2 = b_दत्त, *c_दत्त = c_दत्त_2;
    cv::Auto_बफर<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_चरणः;

    a_चरणः /= परिमाण(a_दत्त[0]);
    b_चरणः /= परिमाण(b_दत्त[0]);
    c_चरणः /= परिमाण(c_दत्त_2[0]);
    d_चरणः /= परिमाण(d_दत्त[0]);
    a_step0 = a_चरणः;
    a_step1 = 1;

    यदि( !c_दत्त_2 )
        c_step0 = c_step1 = 0;
    अन्यथा यदि( !(flags & GEMM_3_T) )
        c_step0 = c_चरणः, c_step1 = 1;
    अन्यथा
        c_step0 = 1, c_step1 = c_चरणः;

    यदि( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_चरणः );
        n = a_आकार.height;
        यदि( a_चरणः > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.दत्त();
        }
    }

    यदि( n == 1 ) /* external product */
    {
        cv::Auto_बफर<T> _b_buf;
        T* b_buf = 0;

        यदि( a_चरणः > 1 && a_आकार.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.दत्त();
            प्रति( k = 0; k < drows; k++ )
                a_buf[k] = a_दत्त[a_चरणः*k];
            a_दत्त = a_buf;
        }

        यदि( b_चरणः > 1 )
        {
            _b_buf.allocate(d_आकार.विस्तारः);
            b_buf = _b_buf.दत्त();
            प्रति( j = 0; j < d_आकार.विस्तारः; j++ )
                b_buf[j] = b_दत्त[j*b_चरणः];
            b_दत्त = b_buf;
        }

        प्रति( i = 0; i < drows; i++, c_दत्त += c_step0, d_दत्त += d_चरणः )
        {
            WT al = WT(a_दत्त[i])*alpha;
            c_दत्त_2 = c_दत्त;
            प्रति( j = 0; j <= d_आकार.विस्तारः - 2; j += 2, c_दत्त_2 += 2*c_step1 )
            {
                WT s0 = al*WT(b_दत्त[j]);
                WT s1 = al*WT(b_दत्त[j+1]);
                यदि( !c_दत्त_2 )
                {
                    d_दत्त[j] = T(s0);
                    d_दत्त[j+1] = T(s1);
                }
                अन्यथा
                {
                    d_दत्त[j] = T(s0 + WT(c_दत्त_2[0])*beta);
                    d_दत्त[j+1] = T(s1 + WT(c_दत्त_2[c_step1])*beta);
                }
            }

            प्रति( ; j < d_आकार.विस्तारः; j++, c_दत्त_2 += c_step1 )
            {
                WT s0 = al*WT(b_दत्त[j]);
                यदि( !c_दत्त_2 )
                    d_दत्त[j] = T(s0);
                अन्यथा
                    d_दत्त[j] = T(s0 + WT(c_दत्त_2[0])*beta);
            }
        }
    }
    अन्यथा यदि( flags & GEMM_2_T ) /* A * Bt */
    {
        प्रति( i = 0; i < drows; i++, a_दत्त_2 += a_step0, c_दत्त += c_step0, d_दत्त += d_चरणः )
        {
            a_दत्त = a_दत्त_2;
            b_दत्त = b_दत्त_2;
            c_दत्त_2 = c_दत्त;

            यदि( a_buf )
            {
                प्रति( k = 0; k < n; k++ )
                    a_buf[k] = a_दत्त[a_step1*k];
                a_दत्त = a_buf;
            }

            प्रति( j = 0; j < d_आकार.विस्तारः; j++, b_दत्त += b_चरणः,
                                               c_दत्त_2 += c_step1 )
            {
                WT s0(0);
                k = 0;
#यदि CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                यदि( परिमाण(WT) == परिमाण(द्विगुण) )
                {
                    यदि( परिमाण(T) == परिमाण(द्विगुण) )
                        s0 = (WT)simd_Dot_गुणनफलम्((ध्रुव द्विगुण*)a_दत्त, (ध्रुव द्विगुण*)b_दत्त, n, k);
                    अन्यथा यदि( परिमाण(T) == परिमाण(भिन्न) )
                        s0 = (WT)simd_Dot_गुणनफलम्_f32f64((ध्रुव भिन्न*)a_दत्त, (ध्रुव भिन्न*)b_दत्त, n, k);
                }
                अन्यथा
#यदि_अन्तः
                {
                    WT s1(0), s2(0), s3(0);
                     #यदि CV_ENABLE_UNROLLED
                    प्रति( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_दत्त[k])*WT(b_दत्त[k]);
                        s1 += WT(a_दत्त[k+1])*WT(b_दत्त[k+1]);
                        s2 += WT(a_दत्त[k+2])*WT(b_दत्त[k+2]);
                        s3 += WT(a_दत्त[k+3])*WT(b_दत्त[k+3]);
                    }
                    #यदि_अन्तः
                    s0 += s1 + s2 + s3;
                }
                प्रति( ; k < n; k++ )
                    s0 += WT(a_दत्त[k])*WT(b_दत्त[k]);
                s0 = s0*alpha;

                यदि( !c_दत्त_2 )
                    d_दत्त[j] = T(s0);
                अन्यथा
                    d_दत्त[j] = T(s0 + WT(c_दत्त_2[0])*beta);
            }
        }
    }
    अन्यथा यदि( d_आकार.विस्तारः*परिमाण(d_दत्त[0]) <= 1600 )
    {
        प्रति( i = 0; i < drows; i++, a_दत्त_2 += a_step0,
                                    c_दत्त += c_step0,
                                    d_दत्त += d_चरणः )
        {
            a_दत्त = a_दत्त_2, c_दत्त_2 = c_दत्त;

            यदि( a_buf )
            {
                प्रति( k = 0; k < n; k++ )
                    a_buf[k] = a_दत्त[a_step1*k];
                a_दत्त = a_buf;
            }

            j = 0;
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            यदि( परिमाण(WT) == परिमाण(द्विगुण) )
            {
                यदि( परिमाण(T) == परिमाण(द्विगुण) )
                    simdGEMM_kj((ध्रुव द्विगुण*)a_दत्त, (ध्रुव द्विगुण*)b_दत्त_2, b_चरणः,
                                        (ध्रुव द्विगुण*)c_दत्त, c_step1,
                                (द्विगुण*)d_दत्त, n, m, alpha, beta, j);
                अन्यथा यदि( परिमाण(T) == परिमाण(भिन्न) )
                    simdGEMM_kj_f32f64((ध्रुव भिन्न*)a_दत्त, (ध्रुव भिन्न*)b_दत्त_2, b_चरणः,
                                        (ध्रुव भिन्न*)c_दत्त, c_step1,
                                (भिन्न*)d_दत्त, n, m, alpha, beta, j);
            }
#यदि_अन्तः
            c_दत्त_2 = c_दत्त + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            प्रति( ; j <= m - 4; j += 4, c_दत्त_2 += 4*c_step1 )
            {
                ध्रुव T* b = b_दत्त_2 + j;
                WT s0(0), s1(0), s2(0), s3(0);

                प्रति( k = 0; k < n; k++, b += b_चरणः )
                {
                    WT a(a_दत्त[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                यदि( !c_दत्त_2 )
                {
                    d_दत्त[j] = T(s0*alpha);
                    d_दत्त[j+1] = T(s1*alpha);
                    d_दत्त[j+2] = T(s2*alpha);
                    d_दत्त[j+3] = T(s3*alpha);
                }
                अन्यथा
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_दत्त[j] = T(s0 + WT(c_दत्त_2[0])*beta);
                    d_दत्त[j+1] = T(s1 + WT(c_दत्त_2[c_step1])*beta);
                    d_दत्त[j+2] = T(s2 + WT(c_दत्त_2[c_step1*2])*beta);
                    d_दत्त[j+3] = T(s3 + WT(c_दत्त_2[c_step1*3])*beta);
                }
            }

            प्रति( ; j < m; j++, c_दत्त_2 += c_step1 )
            {
                ध्रुव T* b = b_दत्त_2 + j;
                WT s0(0);

                प्रति( k = 0; k < n; k++, b += b_चरणः )
                    s0 += WT(a_दत्त[k]) * WT(b[0]);

                s0 = s0*alpha;
                यदि( !c_दत्त_2 )
                    d_दत्त[j] = T(s0);
                अन्यथा
                    d_दत्त[j] = T(s0 + WT(c_दत्त_2[0])*beta);
            }
        }
    }
    अन्यथा
    {
        cv::Auto_बफर<WT> _d_buf(m);
        WT* d_buf = _d_buf.दत्त();

        प्रति( i = 0; i < drows; i++, a_दत्त_2 += a_step0, c_दत्त += c_step0, d_दत्त += d_चरणः )
        {
            a_दत्त = a_दत्त_2;
            b_दत्त = b_दत्त_2;
            c_दत्त_2 = c_दत्त;

            यदि( a_buf )
            {
                प्रति( k = 0; k < n; k++ )
                    a_buf[k] = a_दत्त_2[a_step1*k];
                a_दत्त = a_buf;
            }

            प्रति( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            प्रति( k = 0; k < n; k++, b_दत्त += b_चरणः )
            {
                WT al(a_दत्त[k]);
                j=0;
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                यदि( परिमाण(WT) == परिमाण(द्विगुण) )
                {
                    यदि( परिमाण(T) == परिमाण(द्विगुण) )
                    {
                        ध्रुव पूर्णाङ्क vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ध्रुव द्विगुण*)&al));
                        प्रति( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_आरोपय((ध्रुव द्विगुण*)(d_buf + j));
                            v_float64 vb = vx_आरोपय((ध्रुव द्विगुण*)(b_दत्त + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((द्विगुण*)(d_buf + j), vd);
                        }
                    }
                    अन्यथा
                    {
                        ध्रुव पूर्णाङ्क vlanes_f32 = VTraits<v_float32>::vlanes();
                        ध्रुव पूर्णाङ्क vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((ध्रुव द्विगुण*)&al));
                        प्रति( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_आरोपय((ध्रुव भिन्न*)(b_दत्त + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_उच्चः(vb);
                            v_float64 vd_lo = vx_आरोपय((ध्रुव द्विगुण*)(d_buf + j));
                            v_float64 vd_hi = vx_आरोपय((ध्रुव द्विगुण*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((द्विगुण*)(d_buf + j), vd_lo);
                            v_store((द्विगुण*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#यदि_अन्तः
                प्रति( ; j < m; j++ )
                    d_buf[j] += WT(b_दत्त[j])*al;
            }

            यदि( !c_दत्त_2 )
                प्रति( j = 0; j < m; j++ )
                    d_दत्त[j] = T(d_buf[j]*alpha);
            अन्यथा
                प्रति( j = 0; j < m; j++, c_दत्त_2 += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_दत्त[j] = T(t + WT(c_दत्त_2[0])*beta);
                }
        }
    }
}
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
प्रतिरूपम्<प्रकारनाम WT>
स्थिर अन्तःस्थ शून्य समुच्चयः_Double_दत्त_Zero(WT* d_दत्त, पूर्णाङ्क m, पूर्णाङ्क vlanes)
{
    v_float64 vz = vx_setzero_f64();
    पूर्णाङ्क j = 0;
    प्रति( ; j <= m - vlanes; j += vlanes )
        v_store((द्विगुण*)(d_दत्त + j), vz);
    प्रति( ; j < m; j++ )
        d_दत्त[j] = WT(0);
}

प्रतिरूपम्<प्रकारनाम ST>
स्थिर अन्तःस्थ शून्य simd_Mul_सङ्कलय(ध्रुव ST* b, द्विगुण* d, पूर्णाङ्क m, द्विगुण a_val);

प्रतिरूपम्<>
अन्तःस्थ शून्य simd_Mul_सङ्कलय<भिन्न>(
    ध्रुव भिन्न* b, द्विगुण* d, पूर्णाङ्क m, द्विगुण a_val)
{
    ध्रुव पूर्णाङ्क vlanes_f32 = VTraits<v_float32>::vlanes();
    ध्रुव पूर्णाङ्क vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    पूर्णाङ्क j = 0;
    प्रति( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_आरोपय(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_उच्चः(vb);
        v_float64 vd_lo = vx_आरोपय(d + j);
        v_float64 vd_hi = vx_आरोपय(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    प्रति( ; j < m; j++ )
        d[j] += a_val * (द्विगुण)b[j];
}

प्रतिरूपम्<>
अन्तःस्थ शून्य simd_Mul_सङ्कलय<द्विगुण>(
    ध्रुव द्विगुण* b, द्विगुण* d, पूर्णाङ्क m, द्विगुण a_val)
{
    ध्रुव पूर्णाङ्क vlanes = VTraits<v_float64>::vlanes();
    ध्रुव पूर्णाङ्क stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    पूर्णाङ्क j = 0;
    प्रति( ; j <= m - stride2; j += stride2 )
    {
        v_float64 vd0 = vx_आरोपय(d + j);
        v_float64 vb0 = vx_आरोपय(b + j);
        v_float64 vd1 = vx_आरोपय(d + j + vlanes);
        v_float64 vb1 = vx_आरोपय(b + j + vlanes);
        vd0 = v_muladd(va, vb0, vd0);
        vd1 = v_muladd(va, vb1, vd1);
        v_store(d + j, vd0);
        v_store(d + j + vlanes, vd1);
    }
    प्रति( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_आरोपय(d + j);
        v_float64 vb = vx_आरोपय(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    प्रति( ; j < m; j++ )
        d[j] += a_val * b[j];
}

प्रतिरूपम्<प्रकारनाम ST>
स्थिर अन्तःस्थ शून्य simd_खण्डम्_Mul_kj(
    ध्रुव ST* a_दत्त, ध्रुव ST* b_दत्त, size_t b_चरणः,
    द्विगुण* d_दत्त, पूर्णाङ्क n, पूर्णाङ्क m, तार्किक do_acc)
{
    ध्रुव पूर्णाङ्क vlanes = VTraits<v_float64>::vlanes();
    यदि( !do_acc )
        समुच्चयः_Double_दत्त_Zero(d_दत्त, m, vlanes);
    प्रति( पूर्णाङ्क k = 0; k < n; k++, b_दत्त += b_चरणः )
        simd_Mul_सङ्कलय(b_दत्त, d_दत्त, m, (द्विगुण)a_दत्त[k]);
}
#यदि_अन्तः // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT> स्थिर शून्य
GEMMBlockMul( ध्रुव T* a_दत्त, size_t a_चरणः,
              ध्रुव T* b_दत्त, size_t b_चरणः,
              WT* d_दत्त, size_t d_चरणः,
              आकार a_आकार, आकार d_आकार, पूर्णाङ्क flags )
{
    पूर्णाङ्क i, j, k, n = a_आकार.विस्तारः, m = d_आकार.विस्तारः;
    ध्रुव T *a_दत्त_2 = a_दत्त, *b_दत्त_2 = b_दत्त;
    cv::Auto_बफर<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_चरणः;
    पूर्णाङ्क do_acc = flags & 16;

    a_चरणः /= परिमाण(a_दत्त[0]);
    b_चरणः /= परिमाण(b_दत्त[0]);
    d_चरणः /= परिमाण(d_दत्त[0]);

    a_step0 = a_चरणः;
    a_step1 = 1;

    यदि( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_चरणः );
        n = a_आकार.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.दत्त();
    }

    यदि( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        प्रति( i = 0; i < d_आकार.height; i++, a_दत्त_2 += a_step0, d_दत्त += d_चरणः )
        {
            a_दत्त = a_दत्त_2; b_दत्त = b_दत्त_2;

            यदि( a_buf )
            {
                प्रति( k = 0; k < n; k++ )
                    a_buf[k] = a_दत्त[a_step1*k];
                a_दत्त = a_buf;
            }

            प्रति( j = 0; j < d_आकार.विस्तारः; j++, b_दत्त += b_चरणः )
            {
                WT s0 = do_acc ? d_दत्त[j] : WT(0);
                k = 0;
#यदि CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                यदि( परिमाण(WT) == परिमाण(द्विगुण) )
                {
                    यदि( परिमाण(T) == परिमाण(द्विगुण) )
                        s0 += (WT)simd_Dot_गुणनफलम्((ध्रुव द्विगुण*)a_दत्त, (ध्रुव द्विगुण*)b_दत्त, n, k);
                    अन्यथा यदि( परिमाण(T) == परिमाण(भिन्न) )
                        s0 += (WT)simd_Dot_गुणनफलम्_f32f64((ध्रुव भिन्न*)a_दत्त, (ध्रुव भिन्न*)b_दत्त, n, k);
                }
                अन्यथा
#यदि_अन्तः
                {
                    WT s1(0);
                    प्रति( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_दत्त[k])*WT(b_दत्त[k]);
                        s1 += WT(a_दत्त[k+1])*WT(b_दत्त[k+1]);
                    }
                    s0 += s1;
                }
                प्रति( ; k < n; k++ )
                    s0 += WT(a_दत्त[k])*WT(b_दत्त[k]);

                d_दत्त[j] = s0;
            }
        }
    }
    अन्यथा
    {
        प्रति( i = 0; i < d_आकार.height; i++, a_दत्त_2 += a_step0, d_दत्त += d_चरणः )
        {
            a_दत्त = a_दत्त_2, b_दत्त = b_दत्त_2;

            यदि( a_buf )
            {
                प्रति( k = 0; k < n; k++ )
                    a_buf[k] = a_दत्त[a_step1*k];
                a_दत्त = a_buf;
            }
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            यदि( परिमाण(WT) == परिमाण(द्विगुण) )
            {
                यदि( परिमाण(T) == परिमाण(द्विगुण) )
                    simd_खण्डम्_Mul_kj(
                        (ध्रुव द्विगुण*)a_दत्त, (ध्रुव द्विगुण*)b_दत्त, b_चरणः,
                        (द्विगुण*)d_दत्त, n, m, do_acc != 0);
                अन्यथा
                    simd_खण्डम्_Mul_kj(
                        (ध्रुव भिन्न*)a_दत्त, (ध्रुव भिन्न*)b_दत्त, b_चरणः,
                        (द्विगुण*)d_दत्त, n, m, do_acc != 0);
            }
            अन्यथा
#यदि_अन्तः
            {
                प्रति( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    ध्रुव T* b = b_दत्त + j;

                    यदि( do_acc )
                    {
                        s0 = d_दत्त[j]; s1 = d_दत्त[j+1];
                        s2 = d_दत्त[j+2]; s3 = d_दत्त[j+3];
                    }
                    अन्यथा
                        s0 = s1 = s2 = s3 = WT(0);

                    प्रति( k = 0; k < n; k++, b += b_चरणः )
                    {
                        WT a(a_दत्त[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_दत्त[j] = s0; d_दत्त[j+1] = s1;
                    d_दत्त[j+2] = s2; d_दत्त[j+3] = s3;
                }

                प्रति( ; j < m; j++ )
                {
                    ध्रुव T* b = b_दत्त + j;
                    WT s0 = do_acc ? d_दत्त[j] : WT(0);

                    प्रति( k = 0; k < n; k++, b += b_चरणः )
                        s0 += WT(a_दत्त[k]) * WT(b[0]);

                    d_दत्त[j] = s0;
                }
            }
        }
    }
}


प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT> स्थिर शून्य
GEMMStore( ध्रुव T* c_दत्त_2, size_t c_चरणः,
           ध्रुव WT* d_buf, size_t d_buf_चरणः,
           T* d_दत्त, size_t d_चरणः, आकार d_आकार,
           द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    ध्रुव T* c_दत्त = c_दत्त_2;
    पूर्णाङ्क j;
    size_t c_step0, c_step1;

    c_चरणः /= परिमाण(c_दत्त_2[0]);
    d_buf_चरणः /= परिमाण(d_buf[0]);
    d_चरणः /= परिमाण(d_दत्त[0]);

    यदि( !c_दत्त_2 )
        c_step0 = c_step1 = 0;
    अन्यथा यदि( !(flags & GEMM_3_T) )
        c_step0 = c_चरणः, c_step1 = 1;
    अन्यथा
        c_step0 = 1, c_step1 = c_चरणः;

    प्रति( ; d_आकार.height--; c_दत्त += c_step0, d_buf += d_buf_चरणः, d_दत्त += d_चरणः )
    {
        यदि( c_दत्त )
        {
            c_दत्त_2 = c_दत्त;
            j=0;
             #यदि CV_ENABLE_UNROLLED
            प्रति(; j <= d_आकार.विस्तारः - 4; j += 4, c_दत्त_2 += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_दत्त_2[0]);
                t1 += beta*WT(c_दत्त_2[c_step1]);
                d_दत्त[j] = T(t0);
                d_दत्त[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_दत्त_2[c_step1*2]);
                t1 += beta*WT(c_दत्त_2[c_step1*3]);
                d_दत्त[j+2] = T(t0);
                d_दत्त[j+3] = T(t1);
            }
            #यदि_अन्तः
            प्रति( ; j < d_आकार.विस्तारः; j++, c_दत्त_2 += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_दत्त[j] = T(t0 + WT(c_दत्त_2[0])*beta);
            }
        }
        अन्यथा
        {
            j = 0;
             #यदि CV_ENABLE_UNROLLED
            प्रति( ; j <= d_आकार.विस्तारः - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_दत्त[j] = T(t0);
                d_दत्त[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_दत्त[j+2] = T(t0);
                d_दत्त[j+3] = T(t1);
            }
            #यदि_अन्तः
            प्रति( ; j < d_आकार.विस्तारः; j++ )
                d_दत्त[j] = T(alpha*d_buf[j]);
        }
    }
}


प्रकारनिर्देश शून्य (*GEMMSingleMulFunc)( ध्रुव शून्य* src1, size_t step1,
                   ध्रुव शून्य* src2, size_t step2, ध्रुव शून्य* src3, size_t step3,
                   शून्य* dst, size_t dststep, आकार srcsize, आकार dstsize,
                   द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags );

प्रकारनिर्देश शून्य (*GEMMBlockMulFunc)( ध्रुव शून्य* src1, size_t step1,
                   ध्रुव शून्य* src2, size_t step2, शून्य* dst, size_t dststep,
                   आकार srcsize, आकार dstsize, पूर्णाङ्क flags );

प्रकारनिर्देश शून्य (*GEMMStoreFunc)( ध्रुव शून्य* src1, size_t step1,
                   ध्रुव शून्य* src2, size_t step2, शून्य* dst, size_t dststep,
                   आकार dstsize, द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags );

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT, शून्य (*fn)(ध्रुव T*, size_t, ध्रुव T*, size_t, ध्रुव T*, size_t, T*, size_t, आकार, आकार, द्विगुण, द्विगुण, पूर्णाङ्क)>
स्थिर शून्य gemmSingleMulWrap( ध्रुव शून्य* src1, size_t step1,
                   ध्रुव शून्य* src2, size_t step2, ध्रुव शून्य* src3, size_t step3,
                   शून्य* dst, size_t dststep, आकार srcsize, आकार dstsize,
                   द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    fn((ध्रुव T*)src1, step1, (ध्रुव T*)src2, step2, (ध्रुव T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT, शून्य (*fn)(ध्रुव T*, size_t, ध्रुव T*, size_t, WT*, size_t, आकार, आकार, पूर्णाङ्क)>
स्थिर शून्य gemm_खण्डम्_Mul_Wrap( ध्रुव शून्य* src1, size_t step1,
                   ध्रुव शून्य* src2, size_t step2, शून्य* dst, size_t dststep,
                   आकार srcsize, आकार dstsize, पूर्णाङ्क flags )
{
    fn((ध्रुव T*)src1, step1, (ध्रुव T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT, शून्य (*fn)(ध्रुव T*, size_t, ध्रुव WT*, size_t, T*, size_t, आकार, द्विगुण, द्विगुण, पूर्णाङ्क)>
स्थिर शून्य gemmStoreWrap( ध्रुव शून्य* src1, size_t step1,
                   ध्रुव शून्य* src2, size_t step2, शून्य* dst, size_t dststep,
                   आकार dstsize, द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    fn((ध्रुव T*)src1, step1, (ध्रुव WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

स्थिर शून्य GEMMSingleMul_32f( ध्रुव भिन्न* a_दत्त, size_t a_चरणः,
              ध्रुव भिन्न* b_दत्त, size_t b_चरणः,
              ध्रुव भिन्न* c_दत्त_2, size_t c_चरणः,
              भिन्न* d_दत्त, size_t d_चरणः,
              आकार a_आकार, आकार d_आकार,
              द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    GEMMSingleMul<भिन्न,द्विगुण>(a_दत्त, a_चरणः, b_दत्त, b_चरणः, c_दत्त_2,
                                c_चरणः, d_दत्त, d_चरणः, a_आकार, d_आकार,
                                alpha, beta, flags);
}

स्थिर शून्य GEMMSingleMul_64f( ध्रुव द्विगुण* a_दत्त, size_t a_चरणः,
                              ध्रुव द्विगुण* b_दत्त, size_t b_चरणः,
                              ध्रुव द्विगुण* c_दत्त_2, size_t c_चरणः,
                              द्विगुण* d_दत्त, size_t d_चरणः,
                              आकार a_आकार, आकार d_आकार,
                              द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    GEMMSingleMul<द्विगुण,द्विगुण>(a_दत्त, a_चरणः, b_दत्त, b_चरणः, c_दत्त_2,
                                c_चरणः, d_दत्त, d_चरणः, a_आकार, d_आकार,
                                alpha, beta, flags);
}


स्थिर शून्य GEMMSingleMul_32fc( ध्रुव Complexf* a_दत्त, size_t a_चरणः,
                              ध्रुव Complexf* b_दत्त, size_t b_चरणः,
                              ध्रुव Complexf* c_दत्त_2, size_t c_चरणः,
                              Complexf* d_दत्त, size_t d_चरणः,
                              आकार a_आकार, आकार d_आकार,
                              द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_दत्त, a_चरणः, b_दत्त, b_चरणः, c_दत्त_2,
                                c_चरणः, d_दत्त, d_चरणः, a_आकार, d_आकार,
                                alpha, beta, flags);
}

स्थिर शून्य GEMMSingleMul_64fc( ध्रुव Complexd* a_दत्त, size_t a_चरणः,
                              ध्रुव Complexd* b_दत्त, size_t b_चरणः,
                              ध्रुव Complexd* c_दत्त_2, size_t c_चरणः,
                              Complexd* d_दत्त, size_t d_चरणः,
                              आकार a_आकार, आकार d_आकार,
                              द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_दत्त, a_चरणः, b_दत्त, b_चरणः, c_दत्त_2,
                                 c_चरणः, d_दत्त, d_चरणः, a_आकार, d_आकार,
                                 alpha, beta, flags);
}

स्थिर शून्य GEMMBlockMul_32f( ध्रुव भिन्न* a_दत्त, size_t a_चरणः,
             ध्रुव भिन्न* b_दत्त, size_t b_चरणः,
             द्विगुण* d_दत्त, size_t d_चरणः,
             आकार a_आकार, आकार d_आकार, पूर्णाङ्क flags )
{
    GEMMBlockMul(a_दत्त, a_चरणः, b_दत्त, b_चरणः, d_दत्त, d_चरणः, a_आकार, d_आकार, flags);
}


स्थिर शून्य GEMMBlockMul_64f( ध्रुव द्विगुण* a_दत्त, size_t a_चरणः,
                             ध्रुव द्विगुण* b_दत्त, size_t b_चरणः,
                             द्विगुण* d_दत्त, size_t d_चरणः,
                             आकार a_आकार, आकार d_आकार, पूर्णाङ्क flags )
{
    GEMMBlockMul(a_दत्त, a_चरणः, b_दत्त, b_चरणः, d_दत्त, d_चरणः, a_आकार, d_आकार, flags);
}


स्थिर शून्य GEMMBlockMul_32fc( ध्रुव Complexf* a_दत्त, size_t a_चरणः,
                             ध्रुव Complexf* b_दत्त, size_t b_चरणः,
                             Complexd* d_दत्त, size_t d_चरणः,
                             आकार a_आकार, आकार d_आकार, पूर्णाङ्क flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_दत्त, a_चरणः, b_दत्त, b_चरणः, d_दत्त, d_चरणः, a_आकार, d_आकार, flags);
}


स्थिर शून्य GEMMBlockMul_64fc( ध्रुव Complexd* a_दत्त, size_t a_चरणः,
                             ध्रुव Complexd* b_दत्त, size_t b_चरणः,
                             Complexd* d_दत्त, size_t d_चरणः,
                             आकार a_आकार, आकार d_आकार, पूर्णाङ्क flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_दत्त, a_चरणः, b_दत्त, b_चरणः, d_दत्त, d_चरणः, a_आकार, d_आकार, flags);
}


स्थिर शून्य GEMMStore_32f( ध्रुव भिन्न* c_दत्त_2, size_t c_चरणः,
          ध्रुव द्विगुण* d_buf, size_t d_buf_चरणः,
          भिन्न* d_दत्त, size_t d_चरणः, आकार d_आकार,
          द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    GEMMStore(c_दत्त_2, c_चरणः, d_buf, d_buf_चरणः, d_दत्त, d_चरणः, d_आकार, alpha, beta, flags);
}


स्थिर शून्य GEMMStore_64f( ध्रुव द्विगुण* c_दत्त_2, size_t c_चरणः,
                      ध्रुव द्विगुण* d_buf, size_t d_buf_चरणः,
                      द्विगुण* d_दत्त, size_t d_चरणः, आकार d_आकार,
                      द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    GEMMStore(c_दत्त_2, c_चरणः, d_buf, d_buf_चरणः, d_दत्त, d_चरणः, d_आकार, alpha, beta, flags);
}


स्थिर शून्य GEMMStore_32fc( ध्रुव Complexf* c_दत्त_2, size_t c_चरणः,
                          ध्रुव Complexd* d_buf, size_t d_buf_चरणः,
                          Complexf* d_दत्त, size_t d_चरणः, आकार d_आकार,
                          द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    GEMMStore(c_दत्त_2, c_चरणः, d_buf, d_buf_चरणः, d_दत्त, d_चरणः, d_आकार, alpha, beta, flags);
}


स्थिर शून्य GEMMStore_64fc( ध्रुव Complexd* c_दत्त_2, size_t c_चरणः,
                          ध्रुव Complexd* d_buf, size_t d_buf_चरणः,
                          Complexd* d_दत्त, size_t d_चरणः, आकार d_आकार,
                          द्विगुण alpha, द्विगुण beta, पूर्णाङ्क flags )
{
    GEMMStore(c_दत्त_2, c_चरणः, d_buf, d_buf_चरणः, d_दत्त, d_चरणः, d_आकार, alpha, beta, flags);
}

स्थिर शून्य gemmImpl( Mat A, Mat B, द्विगुण alpha,
           Mat C, द्विगुण beta, Mat D, पूर्णाङ्क flags )
{
    CV_INSTRUMENT_REGION();

    ध्रुव पूर्णाङ्क खण्डम्_lin_आकार = 128;
    ध्रुव पूर्णाङ्क खण्डम्_आकार_3 = खण्डम्_lin_आकार * खण्डम्_lin_आकार;

    स्थिर द्विगुण zero[] = {0,0,0,0};
    स्थिर भिन्न zerof[] = {0,0,0,0};

    आकार a_आकार = A.size(), d_आकार;
    पूर्णाङ्क i, len = 0, type = A.type();

    विकल्प( flags & (GEMM_1_T|GEMM_2_T) )
    {
    पक्ष 0:
        d_आकार = आकार( B.स्तम्भसङ्ख्या, a_आकार.height );
        len = B.पङ्क्तिकाः;
        भङ्ग;
    पक्ष 1:
        d_आकार = आकार( B.स्तम्भसङ्ख्या, a_आकार.विस्तारः );
        len = B.पङ्क्तिकाः;
        भङ्ग;
    पक्ष 2:
        d_आकार = आकार( B.पङ्क्तिकाः, a_आकार.height );
        len = B.स्तम्भसङ्ख्या;
        भङ्ग;
    पक्ष 3:
        d_आकार = आकार( B.पङ्क्तिकाः, a_आकार.विस्तारः );
        len = B.स्तम्भसङ्ख्या;
        भङ्ग;
    }

    यदि( flags == 0 && 2 <= len && len <= 4 && (len == d_आकार.विस्तारः || len == d_आकार.height) )
    {
        यदि( type == CV_32F )
        {
            भिन्न* d = D.ptr<भिन्न>();
            ध्रुव भिन्न *a = A.ptr<भिन्न>(),
                        *b = B.ptr<भिन्न>(),
                        *c = (ध्रुव भिन्न*)C.दत्त;
            size_t d_चरणः = D.चरणः/परिमाण(d[0]),
                a_चरणः = A.चरणः/परिमाण(a[0]),
                b_चरणः = B.चरणः/परिमाण(b[0]),
                c_चरणः = C.दत्त ? C.चरणः/परिमाण(c[0]) : 0;

            यदि( !c )
                c = zerof;

            विकल्प( len )
            {
            पक्ष 2:
                यदि( len == d_आकार.विस्तारः && b != d )
                {
                    प्रति( i = 0; i < d_आकार.height; i++, d += d_चरणः, a += a_चरणः, c += c_चरणः )
                    {
                        भिन्न t0 = a[0]*b[0] + a[1]*b[b_चरणः];
                        भिन्न t1 = a[0]*b[1] + a[1]*b[b_चरणः+1];
                        d[0] = (भिन्न)(t0*alpha + c[0]*beta);
                        d[1] = (भिन्न)(t1*alpha + c[1]*beta);
                    }
                }
                अन्यथा यदि( a != d )
                {
                    पूर्णाङ्क c_step0 = 1;
                    यदि( c == zerof )
                    {
                        c_step0 = 0;
                        c_चरणः = 1;
                    }

                    प्रति( i = 0; i < d_आकार.विस्तारः; i++, d++, b++, c += c_step0 )
                    {
                        भिन्न t0 = a[0]*b[0] + a[1]*b[b_चरणः];
                        भिन्न t1 = a[a_चरणः]*b[0] + a[a_चरणः+1]*b[b_चरणः];
                        d[0] = (भिन्न)(t0*alpha + c[0]*beta);
                        d[d_चरणः] = (भिन्न)(t1*alpha + c[c_चरणः]*beta);
                    }
                }
                अन्यथा
                    भङ्ग;
                प्रत्यागम;
            पक्ष 3:
                यदि( len == d_आकार.विस्तारः && b != d )
                {
                    प्रति( i = 0; i < d_आकार.height; i++, d += d_चरणः, a += a_चरणः, c += c_चरणः )
                    {
                        भिन्न t0 = a[0]*b[0] + a[1]*b[b_चरणः] + a[2]*b[b_चरणः*2];
                        भिन्न t1 = a[0]*b[1] + a[1]*b[b_चरणः+1] + a[2]*b[b_चरणः*2+1];
                        भिन्न t2 = a[0]*b[2] + a[1]*b[b_चरणः+2] + a[2]*b[b_चरणः*2+2];
                        d[0] = (भिन्न)(t0*alpha + c[0]*beta);
                        d[1] = (भिन्न)(t1*alpha + c[1]*beta);
                        d[2] = (भिन्न)(t2*alpha + c[2]*beta);
                    }
                }
                अन्यथा यदि( a != d )
                {
                    पूर्णाङ्क c_step0 = 1;
                    यदि( c == zerof )
                    {
                        c_step0 = 0;
                        c_चरणः = 1;
                    }

                    प्रति( i = 0; i < d_आकार.विस्तारः; i++, d++, b++, c += c_step0 )
                    {
                        भिन्न t0 = a[0]*b[0] + a[1]*b[b_चरणः] + a[2]*b[b_चरणः*2];
                        भिन्न t1 = a[a_चरणः]*b[0] + a[a_चरणः+1]*b[b_चरणः] + a[a_चरणः+2]*b[b_चरणः*2];
                        भिन्न t2 = a[a_चरणः*2]*b[0] + a[a_चरणः*2+1]*b[b_चरणः] + a[a_चरणः*2+2]*b[b_चरणः*2];

                        d[0] = (भिन्न)(t0*alpha + c[0]*beta);
                        d[d_चरणः] = (भिन्न)(t1*alpha + c[c_चरणः]*beta);
                        d[d_चरणः*2] = (भिन्न)(t2*alpha + c[c_चरणः*2]*beta);
                    }
                }
                अन्यथा
                    भङ्ग;
                प्रत्यागम;
            पक्ष 4:
                यदि( len == d_आकार.विस्तारः && b != d )
                {
                    प्रति( i = 0; i < d_आकार.height; i++, d += d_चरणः, a += a_चरणः, c += c_चरणः )
                    {
                        भिन्न t0 = a[0]*b[0] + a[1]*b[b_चरणः] + a[2]*b[b_चरणः*2] + a[3]*b[b_चरणः*3];
                        भिन्न t1 = a[0]*b[1] + a[1]*b[b_चरणः+1] + a[2]*b[b_चरणः*2+1] + a[3]*b[b_चरणः*3+1];
                        भिन्न t2 = a[0]*b[2] + a[1]*b[b_चरणः+2] + a[2]*b[b_चरणः*2+2] + a[3]*b[b_चरणः*3+2];
                        भिन्न t3 = a[0]*b[3] + a[1]*b[b_चरणः+3] + a[2]*b[b_चरणः*2+3] + a[3]*b[b_चरणः*3+3];
                        d[0] = (भिन्न)(t0*alpha + c[0]*beta);
                        d[1] = (भिन्न)(t1*alpha + c[1]*beta);
                        d[2] = (भिन्न)(t2*alpha + c[2]*beta);
                        d[3] = (भिन्न)(t3*alpha + c[3]*beta);
                    }
                }
                अन्यथा यदि( len <= 16 && a != d )
                {
                    पूर्णाङ्क c_step0 = 1;
                    यदि( c == zerof )
                    {
                        c_step0 = 0;
                        c_चरणः = 1;
                    }

                    प्रति( i = 0; i < d_आकार.विस्तारः; i++, d++, b++, c += c_step0 )
                    {
                        भिन्न t0 = a[0]*b[0] + a[1]*b[b_चरणः] + a[2]*b[b_चरणः*2] + a[3]*b[b_चरणः*3];
                        भिन्न t1 = a[a_चरणः]*b[0] + a[a_चरणः+1]*b[b_चरणः] +
                                   a[a_चरणः+2]*b[b_चरणः*2] + a[a_चरणः+3]*b[b_चरणः*3];
                        भिन्न t2 = a[a_चरणः*2]*b[0] + a[a_चरणः*2+1]*b[b_चरणः] +
                                   a[a_चरणः*2+2]*b[b_चरणः*2] + a[a_चरणः*2+3]*b[b_चरणः*3];
                        भिन्न t3 = a[a_चरणः*3]*b[0] + a[a_चरणः*3+1]*b[b_चरणः] +
                                   a[a_चरणः*3+2]*b[b_चरणः*2] + a[a_चरणः*3+3]*b[b_चरणः*3];
                        d[0] = (भिन्न)(t0*alpha + c[0]*beta);
                        d[d_चरणः] = (भिन्न)(t1*alpha + c[c_चरणः]*beta);
                        d[d_चरणः*2] = (भिन्न)(t2*alpha + c[c_चरणः*2]*beta);
                        d[d_चरणः*3] = (भिन्न)(t3*alpha + c[c_चरणः*3]*beta);
                    }
                }
                अन्यथा
                    भङ्ग;
                प्रत्यागम;
            }
        }

        यदि( type == CV_64F )
        {
            द्विगुण* d = D.ptr<द्विगुण>();
            ध्रुव द्विगुण *a = A.ptr<द्विगुण>(),
                         *b = B.ptr<द्विगुण>(),
                         *c = (ध्रुव द्विगुण*)C.दत्त;
            size_t d_चरणः = D.चरणः/परिमाण(d[0]),
                a_चरणः = A.चरणः/परिमाण(a[0]),
                b_चरणः = B.चरणः/परिमाण(b[0]),
                c_चरणः = C.दत्त ? C.चरणः/परिमाण(c[0]) : 0;
            यदि( !c )
                c = zero;

            विकल्प( len )
            {
            पक्ष 2:
                यदि( len == d_आकार.विस्तारः && b != d )
                {
                    प्रति( i = 0; i < d_आकार.height; i++, d += d_चरणः, a += a_चरणः, c += c_चरणः )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_चरणः];
                        द्विगुण t1 = a[0]*b[1] + a[1]*b[b_चरणः+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                अन्यथा यदि( a != d )
                {
                    पूर्णाङ्क c_step0 = 1;
                    यदि( c == zero )
                    {
                        c_step0 = 0;
                        c_चरणः = 1;
                    }

                    प्रति( i = 0; i < d_आकार.विस्तारः; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_चरणः];
                        द्विगुण t1 = a[a_चरणः]*b[0] + a[a_चरणः+1]*b[b_चरणः];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_चरणः] = t1*alpha + c[c_चरणः]*beta;
                    }
                }
                अन्यथा
                    भङ्ग;
                प्रत्यागम;
            पक्ष 3:
                यदि( len == d_आकार.विस्तारः && b != d )
                {
                    प्रति( i = 0; i < d_आकार.height; i++, d += d_चरणः, a += a_चरणः, c += c_चरणः )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_चरणः] + a[2]*b[b_चरणः*2];
                        द्विगुण t1 = a[0]*b[1] + a[1]*b[b_चरणः+1] + a[2]*b[b_चरणः*2+1];
                        द्विगुण t2 = a[0]*b[2] + a[1]*b[b_चरणः+2] + a[2]*b[b_चरणः*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                अन्यथा यदि( a != d )
                {
                    पूर्णाङ्क c_step0 = 1;
                    यदि( c == zero )
                    {
                        c_step0 = 0;
                        c_चरणः = 1;
                    }

                    प्रति( i = 0; i < d_आकार.विस्तारः; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_चरणः] + a[2]*b[b_चरणः*2];
                        द्विगुण t1 = a[a_चरणः]*b[0] + a[a_चरणः+1]*b[b_चरणः] + a[a_चरणः+2]*b[b_चरणः*2];
                        द्विगुण t2 = a[a_चरणः*2]*b[0] + a[a_चरणः*2+1]*b[b_चरणः] + a[a_चरणः*2+2]*b[b_चरणः*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_चरणः] = t1*alpha + c[c_चरणः]*beta;
                        d[d_चरणः*2] = t2*alpha + c[c_चरणः*2]*beta;
                    }
                }
                अन्यथा
                    भङ्ग;
                प्रत्यागम;
            पक्ष 4:
                यदि( len == d_आकार.विस्तारः && b != d )
                {
                    प्रति( i = 0; i < d_आकार.height; i++, d += d_चरणः, a += a_चरणः, c += c_चरणः )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_चरणः] + a[2]*b[b_चरणः*2] + a[3]*b[b_चरणः*3];
                        द्विगुण t1 = a[0]*b[1] + a[1]*b[b_चरणः+1] + a[2]*b[b_चरणः*2+1] + a[3]*b[b_चरणः*3+1];
                        द्विगुण t2 = a[0]*b[2] + a[1]*b[b_चरणः+2] + a[2]*b[b_चरणः*2+2] + a[3]*b[b_चरणः*3+2];
                        द्विगुण t3 = a[0]*b[3] + a[1]*b[b_चरणः+3] + a[2]*b[b_चरणः*2+3] + a[3]*b[b_चरणः*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                अन्यथा यदि( d_आकार.विस्तारः <= 16 && a != d )
                {
                    पूर्णाङ्क c_step0 = 1;
                    यदि( c == zero )
                    {
                        c_step0 = 0;
                        c_चरणः = 1;
                    }

                    प्रति( i = 0; i < d_आकार.विस्तारः; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुण t0 = a[0]*b[0] + a[1]*b[b_चरणः] + a[2]*b[b_चरणः*2] + a[3]*b[b_चरणः*3];
                        द्विगुण t1 = a[a_चरणः]*b[0] + a[a_चरणः+1]*b[b_चरणः] +
                                    a[a_चरणः+2]*b[b_चरणः*2] + a[a_चरणः+3]*b[b_चरणः*3];
                        द्विगुण t2 = a[a_चरणः*2]*b[0] + a[a_चरणः*2+1]*b[b_चरणः] +
                                    a[a_चरणः*2+2]*b[b_चरणः*2] + a[a_चरणः*2+3]*b[b_चरणः*3];
                        द्विगुण t3 = a[a_चरणः*3]*b[0] + a[a_चरणः*3+1]*b[b_चरणः] +
                                    a[a_चरणः*3+2]*b[b_चरणः*2] + a[a_चरणः*3+3]*b[b_चरणः*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_चरणः] = t1*alpha + c[c_चरणः]*beta;
                        d[d_चरणः*2] = t2*alpha + c[c_चरणः*2]*beta;
                        d[d_चरणः*3] = t3*alpha + c[c_चरणः*3]*beta;
                    }
                }
                अन्यथा
                    भङ्ग;
                प्रत्यागम;
            }
        }
    }

    {
    size_t b_चरणः = B.चरणः;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc खण्डम्_Mul_Func;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    ध्रुव uchar* Cdata = C.दत्त;
    size_t Cstep = C.दत्त ? (size_t)C.चरणः : 0;
    Auto_बफर<uchar> buf;

    यदि( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<भिन्न, द्विगुण, GEMMSingleMul_32f>;
        खण्डम्_Mul_Func = gemm_खण्डम्_Mul_Wrap<भिन्न, द्विगुण, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<भिन्न, द्विगुण, GEMMStore_32f>;
    }
    अन्यथा यदि( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<द्विगुण, द्विगुण, GEMMSingleMul_64f>;
        खण्डम्_Mul_Func = gemm_खण्डम्_Mul_Wrap<द्विगुण, द्विगुण, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<द्विगुण, द्विगुण, GEMMStore_64f>;
    }
    अन्यथा यदि( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        खण्डम्_Mul_Func = gemm_खण्डम्_Mul_Wrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    अन्यथा
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        खण्डम्_Mul_Func = gemm_खण्डम्_Mul_Wrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    यदि( (d_आकार.विस्तारः == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_चरणः = d_आकार.विस्तारः == 1 ? 0 : CV_ELEM_आकार(type);
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
    else*/ यदि( ((d_आकार.height <= खण्डम्_lin_आकार/2 || d_आकार.विस्तारः <= खण्डम्_lin_आकार/2) &&
        len <= 10000) || len <= 10 ||
        (d_आकार.विस्तारः <= खण्डम्_lin_आकार &&
        d_आकार.height <= खण्डम्_lin_आकार && len <= खण्डम्_lin_आकार) )
    {
        singleMulFunc( A.ptr(), A.चरणः, B.ptr(), b_चरणः, Cdata, Cstep,
                       matD->ptr(), matD->चरणः, a_आकार, d_आकार, alpha, beta, flags );
    }
    अन्यथा
    {
        पूर्णाङ्क is_a_t = flags & GEMM_1_T;
        पूर्णाङ्क is_b_t = flags & GEMM_2_T;
        पूर्णाङ्क elem_आकार_2 = CV_ELEM_आकार(type);
        पूर्णाङ्क dk0_1, dk0_2;
        size_t a_buf_आकार = 0, b_buf_आकार, d_buf_आकार;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        पूर्णाङ्क j, k, di = 0, dj = 0, dk = 0;
        पूर्णाङ्क dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        पूर्णाङ्क work_elem_आकार = elem_आकार_2 << (CV_MAT_गभीरता(type) == CV_32F ? 1 : 0);

        यदि( !is_a_t )
            a_step0 = A.चरणः, a_step1 = elem_आकार_2;
        अन्यथा
            a_step0 = elem_आकार_2, a_step1 = A.चरणः;

        यदि( !is_b_t )
            b_step0 = b_चरणः, b_step1 = elem_आकार_2;
        अन्यथा
            b_step0 = elem_आकार_2, b_step1 = b_चरणः;

        यदि( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        अन्यथा यदि( !(flags & GEMM_3_T) )
            c_step0 = C.चरणः, c_step1 = elem_आकार_2;
        अन्यथा
            c_step0 = elem_आकार_2, c_step1 = C.चरणः;

        dm0 = std::min( खण्डम्_lin_आकार, d_आकार.height );
        dn0 = std::min( खण्डम्_lin_आकार, d_आकार.विस्तारः );
        dk0_1 = खण्डम्_आकार_3 / dm0;
        dk0_2 = खण्डम्_आकार_3 / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        यदि( dk0*dm0 > खण्डम्_आकार_3 )
            dm0 = खण्डम्_आकार_3 / dk0;
        यदि( dk0*dn0 > खण्डम्_आकार_3 )
            dn0 = खण्डम्_आकार_3 / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_आकार = (size_t)(dk0+dk0/8+1)*dk0_1*elem_आकार_2;
        d_buf_आकार = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_आकार;

        यदि( is_a_t )
        {
            a_buf_आकार = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_आकार_2;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_आकार + b_buf_आकार + a_buf_आकार);
        d_buf = buf.दत्त();
        b_buf = d_buf + d_buf_आकार;

        यदि( is_a_t )
            a_buf = b_buf + b_buf_आकार;

        प्रति( i = 0; i < d_आकार.height; i += di )
        {
            di = dm0;
            यदि( i + di >= d_आकार.height || 8*(i + di) + di > 8*d_आकार.height )
                di = d_आकार.height - i;

            प्रति( j = 0; j < d_आकार.विस्तारः; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->चरणः + j*elem_आकार_2;
                ध्रुव uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t d_चरणः_2 = matD->चरणः;
                dj = dn0;

                यदि( j + dj >= d_आकार.विस्तारः || 8*(j + dj) + dj > 8*d_आकार.विस्तारः )
                    dj = d_आकार.विस्तारः - j;

                flags &= 15;
                यदि( dk0 < len )
                {
                    _d = d_buf;
                    d_चरणः_2 = dj*work_elem_आकार;
                }

                प्रति( k = 0; k < len; k += dk )
                {
                    ध्रुव uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t a_चरणः_2 = A.चरणः;
                    ध्रुव uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t b_चरणः_2 = b_चरणः;
                    आकार a_bl_आकार;

                    dk = dk0;
                    यदि( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    यदि( !is_a_t )
                        a_bl_आकार.विस्तारः = dk, a_bl_आकार.height = di;
                    अन्यथा
                        a_bl_आकार.विस्तारः = di, a_bl_आकार.height = dk;

                    यदि( a_buf && is_a_t )
                    {
                        a_चरणः_2 = dk*elem_आकार_2;
                        GEMM_Transpose_खण्डम्( _a, A.चरणः, a_buf, a_चरणः_2, a_bl_आकार, elem_आकार_2 );
                        std::swap( a_bl_आकार.विस्तारः, a_bl_आकार.height );
                        _a = a_buf;
                    }

                    यदि( dj < d_आकार.विस्तारः )
                    {
                        आकार b_आकार;
                        यदि( !is_b_t )
                            b_आकार.विस्तारः = dj, b_आकार.height = dk;
                        अन्यथा
                            b_आकार.विस्तारः = dk, b_आकार.height = dj;

                        b_चरणः_2 = b_आकार.विस्तारः*elem_आकार_2;
                        GEMM_Copy_खण्डम्( _b, b_चरणः, b_buf, b_चरणः_2, b_आकार, elem_आकार_2 );
                        _b = b_buf;
                    }

                    यदि( dk0 < len )
                        खण्डम्_Mul_Func( _a, a_चरणः_2, _b, b_चरणः_2, _d, d_चरणः_2,
                                      a_bl_आकार, आकार(dj,di), flags );
                    अन्यथा
                        singleMulFunc( _a, a_चरणः_2, _b, b_चरणः_2, _c, Cstep,
                                       _d, d_चरणः_2, a_bl_आकार, आकार(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                यदि( dk0 < len )
                    storeFunc( _c, Cstep, _d, d_चरणः_2,
                               matD->ptr(i) + j*elem_आकार_2,
                               matD->चरणः, आकार(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

प्रतिरूपम् <प्रकारनाम fptype>अन्तःस्थ स्थिर शून्य
callGemmImpl(ध्रुव fptype *src1, size_t src1_चरणः, ध्रुव fptype *src2, size_t src2_चरणः, fptype alpha,
          ध्रुव fptype *src3, size_t src3_चरणः, fptype beta, fptype *dst, size_t dst_चरणः, पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags, पूर्णाङ्क type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    पूर्णाङ्क b_m, b_n, c_m, c_n, m_d;

    यदि(flags & GEMM_2_T)
    {
        b_m = n_d;
        यदि(flags & GEMM_1_T )
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
        यदि(flags & GEMM_1_T )
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

    यदि(flags & GEMM_3_T)
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
    यदि(src1 != रिक्त)
        A = Mat(m_a, n_a, type, (शून्य*)src1, src1_चरणः);
    यदि(src2 != रिक्त)
        B = Mat(b_m, b_n, type, (शून्य*)src2, src2_चरणः);
    यदि(src3 != रिक्त && beta != 0.0)
        C = Mat(c_m, c_n, type, (शून्य*)src3, src3_चरणः);
    Mat D(m_d, n_d, type, (शून्य*)dst, dst_चरणः);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

शून्य gemm32f(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
             भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
             पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags, CV_32F);
}

शून्य gemm64f(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
             द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
             पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags, CV_64F);
}

शून्य gemm32fc(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
              भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
              पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags, CV_32FC2);
}

शून्य gemm64fc(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
              द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
              पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags, CV_64FC2);
}

#यदि_अन्तः // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT> स्थिर शून्य
transform_( ध्रुव T* src, T* dst, ध्रुव WT* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn )
{
    पूर्णाङ्क x;

    यदि( scn == 2 && dcn == 2 )
    {
        प्रति( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    अन्यथा यदि( scn == 3 && dcn == 3 )
    {
        प्रति( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    अन्यथा यदि( scn == 3 && dcn == 1 )
    {
        प्रति( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    अन्यथा यदि( scn == 4 && dcn == 4 )
    {
        प्रति( x = 0; x < len*4; x += 4 )
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
        प्रति( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            ध्रुव WT* _m = m;
            पूर्णाङ्क j, k;
            प्रति( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                प्रति( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

स्थिर शून्य
transform_8u( ध्रुव uchar* src, uchar* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn )
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क द्वयङ्काः = 10, SCALE = 1 << द्वयङ्काः;
    ध्रुव भिन्न MAX_M = (भिन्न)(1 << (15 - द्वयङ्काः));

    यदि( scn == 3 && dcn == 3 &&
        std::निरपेक्षम्(m[0]) < MAX_M && std::निरपेक्षम्(m[1]) < MAX_M && std::निरपेक्षम्(m[ 2]) < MAX_M*256 && std::निरपेक्षम्(m[ 3]) < MAX_M*256 &&
        std::निरपेक्षम्(m[4]) < MAX_M && std::निरपेक्षम्(m[5]) < MAX_M && std::निरपेक्षम्(m[ 6]) < MAX_M*256 && std::निरपेक्षम्(m[ 7]) < MAX_M*256 &&
        std::निरपेक्षम्(m[8]) < MAX_M && std::निरपेक्षम्(m[9]) < MAX_M && std::निरपेक्षम्(m[10]) < MAX_M*256 && std::निरपेक्षम्(m[11]) < MAX_M*256 )
    {
        ध्रुव पूर्णाङ्क nChannels = 3;

        सङ्घ {
            ह्रस्व s[6];
            पूर्णाङ्क p[3];
        } m16;
        m16.s[0] = saturate_cast<ह्रस्व>(m[0] * SCALE); m16.s[1] = saturate_cast<ह्रस्व>(m[1] * SCALE);
        m16.s[2] = saturate_cast<ह्रस्व>(m[4] * SCALE); m16.s[3] = saturate_cast<ह्रस्व>(m[5] * SCALE);
        m16.s[4] = saturate_cast<ह्रस्व>(m[8] * SCALE); m16.s[5] = saturate_cast<ह्रस्व>(m[9] * SCALE);
        पूर्णाङ्क m32[] = {saturate_cast<पूर्णाङ्क>(m[ 2] * SCALE), saturate_cast<पूर्णाङ्क>(m[ 3] * SCALE),
                     saturate_cast<पूर्णाङ्क>(m[ 6] * SCALE), saturate_cast<पूर्णाङ्क>(m[ 7] * SCALE),
                     saturate_cast<पूर्णाङ्क>(m[10] * SCALE), saturate_cast<पूर्णाङ्क>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        पूर्णाङ्क x = 0;
        प्रति (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
        {
            v_uint8 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
            v_uint8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_uint16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_uint16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p0), m01), v_mul(p1, m2)), m3),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p2), m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p0), m45), v_mul(p1, m6)), m7),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p2), m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p0), m89), v_mul(p1, m10)), m11),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(v_reinterpret_as_s16(p2), m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack_u(dbl, dbh), v_pack_u(dgl, dgh), v_pack_u(drl, drh));
        }
        m32[1] = saturate_cast<पूर्णाङ्क>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<पूर्णाङ्क>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<पूर्णाङ्क>((m[11] + 0.5f)*SCALE);
        प्रति( ; x < len * nChannels; x += nChannels )
        {
            पूर्णाङ्क v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> द्वयङ्काः);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> द्वयङ्काः);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> द्वयङ्काः);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_16u( ध्रुव ushort* src, ushort* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn )
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि( scn == 3 && dcn == 3 )
    {
        पूर्णाङ्क x = 0;
#यदि CV_SIMD_विस्तारः > 16
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
        प्रति (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
        {
            v_uint16 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
            v_uint32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_सङ्कलय_wrap(v_pack(v_आवर्तनम्(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m2,  m3)))),
                                   v_आवर्तनम्(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m0, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m1, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m2,  m3))))), delta);
            dg = v_सङ्कलय_wrap(v_pack(v_आवर्तनम्(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)),  m6,  m7)))),
                                   v_आवर्तनम्(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m4, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m5, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)),  m6,  m7))))), delta);
            dr = v_सङ्कलय_wrap(v_pack(v_आवर्तनम्(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bl)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gl)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rl)), m10, m11)))),
                                   v_आवर्तनम्(v_muladd(v_cvt_f32(v_reinterpret_as_s32(bh)), m8, v_muladd(v_cvt_f32(v_reinterpret_as_s32(gh)), m9, v_muladd(v_cvt_f32(v_reinterpret_as_s32(rh)), m10, m11))))), delta);
            v_store_interleave(dst + x, v_reinterpret_as_u16(db), v_reinterpret_as_u16(dg), v_reinterpret_as_u16(dr));
        }
#यदि_अन्तः
#यदि CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_वामः<1>(_m0l);
        v_float32x4 _m1h = v_rotate_वामः<1>(_m1l);
        v_float32x4 _m2h = v_rotate_वामः<1>(_m2l);
        v_float32x4 _m3h = v_rotate_वामः<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        प्रति( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_दक्षिणः<1>(v_reinterpret_as_u16(v_सङ्कलय_wrap(v_pack(
                             v_आवर्तनम्(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_आरोपय_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_आवर्तनम्(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_आरोपय_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#यदि_अन्तः //CV_SIMD128
        प्रति( ; x < len * 3; x += 3 )
        {
            भिन्न v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_32f( ध्रुव भिन्न* src, भिन्न* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#यदि (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    पूर्णाङ्क x = 0;
    यदि( scn == 3 && dcn == 3 )
    {
        पूर्णाङ्क idx[VTraits<v_float32>::max_nlanes/2];
        प्रति( पूर्णाङ्क i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        भिन्न _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        प्रति( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        प्रति( ; x < len*3; x += 3 )
        {
            भिन्न v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            भिन्न t0 = saturate_cast<भिन्न>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            भिन्न t1 = saturate_cast<भिन्न>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            भिन्न t2 = saturate_cast<भिन्न>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        प्रत्यागम;
    }

    यदि( scn == 4 && dcn == 4 )
    {
#यदि CV_SIMD_विस्तारः > 16
        पूर्णाङ्क idx[VTraits<v_float32>::max_nlanes/4];
        प्रति( पूर्णाङ्क i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        भिन्न _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        प्रति( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_आरोपय(src + x);
            v_store(dst + x, v_सङ्कलय(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#यदि_अन्तः
#यदि CV_SIMD128
        v_float32x4 _m0 = v_आरोपय(m     );
        v_float32x4 _m1 = v_आरोपय(m +  5);
        v_float32x4 _m2 = v_आरोपय(m + 10);
        v_float32x4 _m3 = v_आरोपय(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        प्रति( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_आरोपय(src + x);
            v_store(dst + x, v_सङ्कलय(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#अन्यथा // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        प्रति( ; x < len*4; x += 4 )
        {
            भिन्न v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            भिन्न t0 = saturate_cast<भिन्न>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            भिन्न t1 = saturate_cast<भिन्न>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            भिन्न t2 = saturate_cast<भिन्न>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            भिन्न t3 = saturate_cast<भिन्न>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#यदि_अन्तः
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    transform_(src, dst, m, len, scn, dcn);
}


स्थिर शून्य
transform_8s(ध्रुव schar* src, schar* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    ध्रुव पूर्णाङ्क द्वयङ्काः = 10, SCALE = 1 << द्वयङ्काः;
    ध्रुव भिन्न MAX_M = (भिन्न)(1 << (15 - द्वयङ्काः));

    यदि( scn == 3 && dcn == 3 &&
        std::निरपेक्षम्(m[0]) < MAX_M && std::निरपेक्षम्(m[1]) < MAX_M && std::निरपेक्षम्(m[ 2]) < MAX_M*256 && std::निरपेक्षम्(m[ 3]) < MAX_M*256 &&
        std::निरपेक्षम्(m[4]) < MAX_M && std::निरपेक्षम्(m[5]) < MAX_M && std::निरपेक्षम्(m[ 6]) < MAX_M*256 && std::निरपेक्षम्(m[ 7]) < MAX_M*256 &&
        std::निरपेक्षम्(m[8]) < MAX_M && std::निरपेक्षम्(m[9]) < MAX_M && std::निरपेक्षम्(m[10]) < MAX_M*256 && std::निरपेक्षम्(m[11]) < MAX_M*256 )
    {
        ध्रुव पूर्णाङ्क nChannels = 3;

        सङ्घ {
            ह्रस्व s[6];
            पूर्णाङ्क p[3];
        } m16;
        m16.s[0] = saturate_cast<ह्रस्व>(m[0] * SCALE); m16.s[1] = saturate_cast<ह्रस्व>(m[1] * SCALE);
        m16.s[2] = saturate_cast<ह्रस्व>(m[4] * SCALE); m16.s[3] = saturate_cast<ह्रस्व>(m[5] * SCALE);
        m16.s[4] = saturate_cast<ह्रस्व>(m[8] * SCALE); m16.s[5] = saturate_cast<ह्रस्व>(m[9] * SCALE);
        पूर्णाङ्क m32[] = {saturate_cast<पूर्णाङ्क>(m[ 2] * SCALE), saturate_cast<पूर्णाङ्क>(m[ 3] * SCALE),
                     saturate_cast<पूर्णाङ्क>(m[ 6] * SCALE), saturate_cast<पूर्णाङ्क>(m[ 7] * SCALE),
                     saturate_cast<पूर्णाङ्क>(m[10] * SCALE), saturate_cast<पूर्णाङ्क>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        पूर्णाङ्क x = 0;
        प्रति (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
        {
            v_int8 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
            v_int8 bgl, bgh;
            v_zip(b, g, bgl, bgh);
            v_int16 rl, rh;
            v_expand(r, rl, rh);

            v_int16 dbl, dbh, dgl, dgh, drl, drh;
            v_int16 p0, p2;
            v_int32 p1, p3;
            v_expand(bgl, p0, p2);
            v_expand(v_reinterpret_as_s16(rl), p1, p3);
            dbl = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgl = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drl = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_expand(bgh, p0, p2);
            v_expand(v_reinterpret_as_s16(rh), p1, p3);
            dbh = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(p0, m01), v_mul(p1, m2)), m3),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(p2, m01), v_mul(p3, m2)), m3));
            dgh = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(p0, m45), v_mul(p1, m6)), m7),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(p2, m45), v_mul(p3, m6)), m7));
            drh = v_rshr_pack<द्वयङ्काः>(v_सङ्कलय(v_सङ्कलय(v_dotprod(p0, m89), v_mul(p1, m10)), m11),
                                    v_सङ्कलय(v_सङ्कलय(v_dotprod(p2, m89), v_mul(p3, m10)), m11));
            v_store_interleave(dst + x, v_pack(dbl, dbh), v_pack(dgl, dgh), v_pack(drl, drh));
        }
        m32[1] = saturate_cast<पूर्णाङ्क>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<पूर्णाङ्क>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<पूर्णाङ्क>((m[11] + 0.5f)*SCALE);
        प्रति( ; x < len * nChannels; x += nChannels )
        {
            पूर्णाङ्क v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> द्वयङ्काः);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> द्वयङ्काः);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> द्वयङ्काः);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_16s(ध्रुव ह्रस्व* src, ह्रस्व* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि (scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

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
        प्रति (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
        {
            v_int16 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
            v_int32 bl, bh, gl, gh, rl, rh;
            v_expand(b, bl, bh);
            v_expand(g, gl, gh);
            v_expand(r, rl, rh);

            v_int16 db, dg, dr;
            db = v_pack(v_आवर्तनम्(v_muladd(v_cvt_f32(bl), m0, v_muladd(v_cvt_f32(gl), m1, v_muladd(v_cvt_f32(rl),  m2,  m3)))),
                        v_आवर्तनम्(v_muladd(v_cvt_f32(bh), m0, v_muladd(v_cvt_f32(gh), m1, v_muladd(v_cvt_f32(rh),  m2,  m3)))));
            dg = v_pack(v_आवर्तनम्(v_muladd(v_cvt_f32(bl), m4, v_muladd(v_cvt_f32(gl), m5, v_muladd(v_cvt_f32(rl),  m6,  m7)))),
                        v_आवर्तनम्(v_muladd(v_cvt_f32(bh), m4, v_muladd(v_cvt_f32(gh), m5, v_muladd(v_cvt_f32(rh),  m6,  m7)))));
            dr = v_pack(v_आवर्तनम्(v_muladd(v_cvt_f32(bl), m8, v_muladd(v_cvt_f32(gl), m9, v_muladd(v_cvt_f32(rl), m10, m11)))),
                        v_आवर्तनम्(v_muladd(v_cvt_f32(bh), m8, v_muladd(v_cvt_f32(gh), m9, v_muladd(v_cvt_f32(rh), m10, m11)))));
            v_store_interleave(dst + x, db, dg, dr);
        }
        प्रति( ; x < len * 3; x += 3 )
        {
            भिन्न v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ह्रस्व t0 = saturate_cast<ह्रस्व>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ह्रस्व t1 = saturate_cast<ह्रस्व>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ह्रस्व t2 = saturate_cast<ह्रस्व>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_32s(ध्रुव पूर्णाङ्क* src, पूर्णाङ्क* dst, ध्रुव द्विगुण* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    यदि( scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

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
        प्रति (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_उच्चः(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_उच्चः(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_उच्चः(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_आवर्तनम्(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_आवर्तनम्(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_आवर्तनम्(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        प्रति (; x < len * 3; x += 3) {
            द्विगुण b = src[x], g = src[x + 1], r = src[x + 2];
            पूर्णाङ्क db = saturate_cast<पूर्णाङ्क>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            पूर्णाङ्क dg = saturate_cast<पूर्णाङ्क>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            पूर्णाङ्क dr = saturate_cast<पूर्णाङ्क>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    transform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
transform_64f(ध्रुव द्विगुण* src, द्विगुण* dst, ध्रुव द्विगुण* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    यदि( scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

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
        प्रति (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        प्रति (; x < len * 3; x += 3) {
            द्विगुण b = src[x], g = src[x + 1], r = src[x + 2];
            द्विगुण db = saturate_cast<द्विगुण>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            द्विगुण dg = saturate_cast<द्विगुण>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            द्विगुण dr = saturate_cast<द्विगुण>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    transform_(src, dst, m, len, scn, dcn);
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT> स्थिर शून्य
diagtransform_( ध्रुव T* src, T* dst, ध्रुव WT* m, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क )
{
    पूर्णाङ्क x;

    यदि( cn == 2 )
    {
        प्रति( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    अन्यथा यदि( cn == 3 )
    {
        प्रति( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    अन्यथा यदि( cn == 4 )
    {
        प्रति( x = 0; x < len*4; x += 4 )
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
        प्रति( x = 0; x < len; x++, src += cn, dst += cn )
        {
            ध्रुव WT* _m = m;
            प्रति( पूर्णाङ्क j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

स्थिर शून्य
diagtransform_8u(ध्रुव uchar* src, uchar* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि (scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        प्रति (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
            v_uint8 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack_u(v_आवर्तनम्(db_f32_l0), v_आवर्तनम्(db_f32_l1)), v_pack_u(v_आवर्तनम्(db_f32_h0), v_आवर्तनम्(db_f32_h1))),
                               v_pack(v_pack_u(v_आवर्तनम्(dg_f32_l0), v_आवर्तनम्(dg_f32_l1)), v_pack_u(v_आवर्तनम्(dg_f32_h0), v_आवर्तनम्(dg_f32_h1))),
                               v_pack(v_pack_u(v_आवर्तनम्(dr_f32_l0), v_आवर्तनम्(dr_f32_l1)), v_pack_u(v_आवर्तनम्(dr_f32_h0), v_आवर्तनम्(dr_f32_h1))));
        }
        प्रति (; x < len * 3; x += 3) {
            पूर्णाङ्क b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_8s(ध्रुव schar* src, schar* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
    #यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि (scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        प्रति (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
            v_int8 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
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
                               v_pack(v_pack(v_आवर्तनम्(db_f32_l0), v_आवर्तनम्(db_f32_l1)), v_pack(v_आवर्तनम्(db_f32_h0), v_आवर्तनम्(db_f32_h1))),
                               v_pack(v_pack(v_आवर्तनम्(dg_f32_l0), v_आवर्तनम्(dg_f32_l1)), v_pack(v_आवर्तनम्(dg_f32_h0), v_आवर्तनम्(dg_f32_h1))),
                               v_pack(v_pack(v_आवर्तनम्(dr_f32_l0), v_आवर्तनम्(dr_f32_l1)), v_pack(v_आवर्तनम्(dr_f32_h0), v_आवर्तनम्(dr_f32_h1))));
        }
        प्रति (; x < len * 3; x += 3) {
            पूर्णाङ्क b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_16u(ध्रुव ushort* src, ushort* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि (scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        प्रति (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
            v_uint16 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
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
                               v_pack_u(v_आवर्तनम्(db_f32_l), v_आवर्तनम्(db_f32_h)),
                               v_pack_u(v_आवर्तनम्(dg_f32_l), v_आवर्तनम्(dg_f32_h)),
                               v_pack_u(v_आवर्तनम्(dr_f32_l), v_आवर्तनम्(dr_f32_h)));
        }
        प्रति (; x < len * 3; x += 3) {
            पूर्णाङ्क b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_16s(ध्रुव ह्रस्व* src, ह्रस्व* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि (scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        प्रति (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
            v_int16 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
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
                               v_pack(v_आवर्तनम्(db_f32_l), v_आवर्तनम्(db_f32_h)),
                               v_pack(v_आवर्तनम्(dg_f32_l), v_आवर्तनम्(dg_f32_h)),
                               v_pack(v_आवर्तनम्(dr_f32_l), v_आवर्तनम्(dr_f32_h)));
        }
        प्रति (; x < len * 3; x += 3) {
            पूर्णाङ्क b = src[x], g = src[x + 1], r = src[x + 2];
            ह्रस्व db = saturate_cast<ह्रस्व>(m[ 0] * b + m[ 3]);
            ह्रस्व dg = saturate_cast<ह्रस्व>(m[ 5] * g + m[ 7]);
            ह्रस्व dr = saturate_cast<ह्रस्व>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_32s(ध्रुव पूर्णाङ्क* src, पूर्णाङ्क* dst, ध्रुव द्विगुण* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    यदि (scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        प्रति (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_उच्चः(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_उच्चः(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_उच्चः(r), rl = v_cvt_f64(r);

            v_int32 db = v_आवर्तनम्(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_आवर्तनम्(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_आवर्तनम्(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        प्रति (; x < len * 3; x += 3) {
            पूर्णाङ्क b = src[x], g = src[x + 1], r = src[x + 2];
            पूर्णाङ्क db = saturate_cast<पूर्णाङ्क>(m[ 0] * b + m[ 3]);
            पूर्णाङ्क dg = saturate_cast<पूर्णाङ्क>(m[ 5] * g + m[ 7]);
            पूर्णाङ्क dr = saturate_cast<पूर्णाङ्क>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_32f(ध्रुव भिन्न* src, भिन्न* dst, ध्रुव भिन्न* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    यदि (scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        प्रति (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        प्रति (; x < len * 3; x += 3) {
            भिन्न b = src[x], g = src[x + 1], r = src[x + 2];
            भिन्न db = saturate_cast<भिन्न>(m[ 0] * b + m[ 3]);
            भिन्न dg = saturate_cast<भिन्न>(m[ 5] * g + m[ 7]);
            भिन्न dr = saturate_cast<भिन्न>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    diagtransform_(src, dst, m, len, scn, dcn);
}

स्थिर शून्य
diagtransform_64f(ध्रुव द्विगुण* src, द्विगुण* dst, ध्रुव द्विगुण* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    यदि (scn == 3 && dcn == 3) {
        पूर्णाङ्क x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        प्रति (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_आरोपय_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        प्रति (; x < len * 3; x += 3) {
            द्विगुण b = src[x], g = src[x + 1], r = src[x + 2];
            द्विगुण db = saturate_cast<द्विगुण>(m[ 0] * b + m[ 3]);
            द्विगुण dg = saturate_cast<द्विगुण>(m[ 5] * g + m[ 7]);
            द्विगुण dr = saturate_cast<द्विगुण>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        प्रत्यागम;
    }
#यदि_अन्तः

    diagtransform_(src, dst, m, len, scn, dcn);
}


प्रतिरूपम्<प्रकारनाम T, प्रकारनाम WT, शून्य (*fn)(ध्रुव T*, T*, ध्रुव WT*, पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर शून्य transformWrap(ध्रुव शून्य* src, शून्य* dst, ध्रुव शून्य* m, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn)
{
    fn((ध्रुव T*)src, (T*)dst, (ध्रुव WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(पूर्णाङ्क गभीरता)
{
    स्थिर TransformFunc transformTab[CV_गभीरता_MAX] =
    {
        transformWrap<uchar, भिन्न, transform_8u>, transformWrap<schar, भिन्न, transform_8s>, transformWrap<ushort, भिन्न, transform_16u>,
        transformWrap<ह्रस्व, भिन्न, transform_16s>, transformWrap<पूर्णाङ्क, द्विगुण, transform_32s>, transformWrap<भिन्न, भिन्न, transform_32f>,
        transformWrap<द्विगुण, द्विगुण, transform_64f>, 0
    };

    प्रत्यागम transformTab[गभीरता];
}

TransformFunc getDiagTransformFunc(पूर्णाङ्क गभीरता)
{
    स्थिर TransformFunc diagTransformTab[CV_गभीरता_MAX] =
    {
        transformWrap<uchar, भिन्न, diagtransform_8u>, transformWrap<schar, भिन्न, diagtransform_8s>, transformWrap<ushort, भिन्न, diagtransform_16u>,
        transformWrap<ह्रस्व, भिन्न, diagtransform_16s>, transformWrap<पूर्णाङ्क, द्विगुण, diagtransform_32s>, transformWrap<भिन्न, भिन्न, diagtransform_32f>,
        transformWrap<द्विगुण, द्विगुण, diagtransform_64f>, 0
    };

    प्रत्यागम diagTransformTab[गभीरता];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
perspectiveTransform_( ध्रुव शून्य* _src, शून्य* _dst, ध्रुव शून्य* _m_ptr, पूर्णाङ्क len, पूर्णाङ्क scn, पूर्णाङ्क dcn )
{
    ध्रुव T* src = (ध्रुव T*)_src;
    T* dst = (T*)_dst;
    ध्रुव द्विगुण* m = (ध्रुव द्विगुण*)_m_ptr;
    ध्रुव द्विगुण eps = FLT_EPSILON;
    पूर्णाङ्क i;

    यदि( scn == 2 && dcn == 2 )
    {
        प्रति( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            द्विगुण w = x*m[6] + y*m[7] + m[8];

            यदि( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            अन्यथा
                dst[i] = dst[i+1] = (T)0;
        }
    }
    अन्यथा यदि( scn == 3 && dcn == 3 )
    {
        प्रति( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            द्विगुण w = x*m[12] + y*m[13] + z*m[14] + m[15];

            यदि( fabs(w) > eps )
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
    अन्यथा यदि( scn == 3 && dcn == 2 )
    {
        प्रति( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            द्विगुण w = x*m[8] + y*m[9] + z*m[10] + m[11];

            यदि( fabs(w) > eps )
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
        प्रति( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            ध्रुव द्विगुण* _m = m + dcn*(scn + 1);
            द्विगुण w = _m[scn];
            पूर्णाङ्क j, k;
            प्रति( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            यदि( fabs(w) > eps )
            {
                _m = m;
                प्रति( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    द्विगुण s = _m[scn];
                    प्रति( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            अन्यथा
                प्रति( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(पूर्णाङ्क गभीरता)
{
    यदि (गभीरता == CV_32F)
        प्रत्यागम perspectiveTransform_<भिन्न>;
    यदि (गभीरता == CV_64F)
        प्रत्यागम perspectiveTransform_<द्विगुण>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

स्थिर शून्य scale_सङ्कलय_32f(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, शून्य* _dst,
                         पूर्णाङ्क len, ध्रुव शून्य* _alpha)
{
    ध्रुव भिन्न* src1 = (ध्रुव भिन्न*)_src1;
    ध्रुव भिन्न* src2 = (ध्रुव भिन्न*)_src2;
    भिन्न* dst = (भिन्न*)_dst;
    भिन्न alpha = *(ध्रुव भिन्न*)_alpha;
    पूर्णाङ्क i = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    ध्रुव पूर्णाङ्क c_विस्तारः = VTraits<v_float32>::vlanes();
    प्रति (; i <= len - c_विस्तारः; i += c_विस्तारः)
        v_store(dst + i, v_muladd(vx_आरोपय(src1 + i), v_alpha, vx_आरोपय(src2 + i)));
    vx_cleanup();
#यदि_अन्तः
    प्रति (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


स्थिर शून्य scale_सङ्कलय_64f(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, शून्य* _dst,
                         पूर्णाङ्क len, ध्रुव शून्य* _alpha)
{
    ध्रुव द्विगुण* src1 = (ध्रुव द्विगुण*)_src1;
    ध्रुव द्विगुण* src2 = (ध्रुव द्विगुण*)_src2;
    द्विगुण* dst = (द्विगुण*)_dst;
    द्विगुण alpha = *(ध्रुव द्विगुण*)_alpha;
    पूर्णाङ्क i = 0;
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    ध्रुव पूर्णाङ्क c_विस्तारः = VTraits<v_float64>::vlanes();
    प्रति (; i <= len - c_विस्तारः; i += c_विस्तारः)
        v_store(dst + i, v_muladd(vx_आरोपय(src1 + i), a2, vx_आरोपय(src2 + i)));
    vx_cleanup();
#यदि_अन्तः
    प्रति (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

Scale_सङ्कलय_Func get_Scale_सङ्कलय_Func(पूर्णाङ्क गभीरता)
{
    यदि (गभीरता == CV_32F)
        प्रत्यागम scale_सङ्कलय_32f;
    यदि (गभीरता == CV_64F)
        प्रत्यागम scale_सङ्कलय_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

प्रतिरूपम्<प्रकारनाम T> स्थिर अन्तःस्थ
द्विगुण MahalanobisImpl(ध्रुव Mat& v1, ध्रुव Mat& v2, ध्रुव Mat& icovar, द्विगुण *diff_बफर /*[len]*/, पूर्णाङ्क len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    आकार sz = v1.size();
    द्विगुण परिणाम = 0;

    sz.विस्तारः *= v1.channels();
    यदि (v1.isContinuous() && v2.isContinuous())
    {
        sz.विस्तारः *= sz.height;
        sz.height = 1;
    }

    {
        ध्रुव T* src1 = v1.ptr<T>();
        ध्रुव T* src2 = v2.ptr<T>();
        size_t step1 = v1.चरणः/परिमाण(src1[0]);
        size_t step2 = v2.चरणः/परिमाण(src2[0]);
        द्विगुण* diff = diff_बफर;
        ध्रुव T* mat = icovar.ptr<T>();
        size_t matstep = icovar.चरणः/परिमाण(mat[0]);

        प्रति (; sz.height--; src1 += step1, src2 += step2, diff += sz.विस्तारः)
        {
            प्रति (पूर्णाङ्क i = 0; i < sz.विस्तारः; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_बफर;
        प्रति (पूर्णाङ्क i = 0; i < len; i++, mat += matstep)
        {
            द्विगुण row_योग = 0;
            पूर्णाङ्क j = 0;
#यदि CV_ENABLE_UNROLLED
            प्रति(; j <= len - 4; j += 4 )
                row_योग += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#यदि_अन्तः
            प्रति (; j < len; j++)
                row_योग += diff[j]*mat[j];
            परिणाम += row_योग * diff[i];
        }
    }
    प्रत्यागम परिणाम;
}

MahalanobisImplFunc getMahalanobisImplFunc(पूर्णाङ्क गभीरता)
{
    यदि (गभीरता == CV_32F)
        प्रत्यागम (MahalanobisImplFunc)MahalanobisImpl<भिन्न>;
    यदि (गभीरता == CV_64F)
        प्रत्यागम (MahalanobisImplFunc)MahalanobisImpl<द्विगुण>;
    CV_Assert(0 && "Not supported");
}


#यदि !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_बहुलकम्)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

प्रतिरूपम्<प्रकारनाम sT, प्रकारनाम dT> स्थिर शून्य
MulTransposedR(ध्रुव Mat& srcmat, ध्रुव Mat& dstmat, ध्रुव Mat& deltamat, द्विगुण scale)
{
    पूर्णाङ्क i, j, k;
    ध्रुव sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ध्रुव dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.चरणः/परिमाण(src[0]);
    size_t dststep = dstmat.चरणः/परिमाण(dst[0]);
    size_t deltastep = deltamat.पङ्क्तिकाः > 1 ? deltamat.चरणः/परिमाण(delta[0]) : 0;
    पूर्णाङ्क delta_स्तम्भसङ्ख्या = deltamat.स्तम्भसङ्ख्या;
    आकार size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    पूर्णाङ्क buf_आकार_2 = size.height*परिमाण(dT);
    Auto_बफर<uchar> buf;

    यदि( delta && delta_स्तम्भसङ्ख्या < size.विस्तारः )
    {
        CV_Assert( delta_स्तम्भसङ्ख्या == 1 );
        buf_आकार_2 *= 5;
    }
    buf.allocate(buf_आकार_2);
    col_buf = (dT*)buf.दत्त();

    यदि( delta && delta_स्तम्भसङ्ख्या < size.विस्तारः )
    {
        delta_buf = col_buf + size.height;
        प्रति( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#यदि CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#यदि_अन्तः

    यदि( !delta )
        प्रति( i = 0; i < size.विस्तारः; i++, tdst += dststep )
        {
            प्रति( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            प्रति( j = i; j <= size.विस्तारः - 4; j += 4 )
            {
#यदि CV_SIMD128_64F
                यदि (दत्त_Type_2<sT>::गभीरता == CV_64F && दत्त_Type_2<dT>::गभीरता == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ध्रुव द्विगुण *tsrc = (द्विगुण*)(src + j);

                    प्रति( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((द्विगुण)col_buf[k]);
                        s0 = v_सङ्कलय(s0, v_mul(a, v_आरोपय(tsrc + 0)));
                        s1 = v_सङ्कलय(s1, v_mul(a, v_आरोपय(tsrc + 2)));
                    }

                    v_store((द्विगुण*)(tdst+j), v_mul(s0, v_scale));
                    v_store((द्विगुण*)(tdst+j+2), v_mul(s1, v_scale));
                } अन्यथा
#यदि_अन्तः
                {
                    द्विगुण s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ध्रुव sT *tsrc = src + j;

                    प्रति( k = 0; k < size.height; k++, tsrc += srcstep )
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

            प्रति( ; j < size.विस्तारः; j++ )
            {
                द्विगुण s0 = 0;
                ध्रुव sT *tsrc = src + j;

                प्रति( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (द्विगुण)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    अन्यथा
        प्रति( i = 0; i < size.विस्तारः; i++, tdst += dststep )
        {
            यदि( !delta_buf )
                प्रति( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            अन्यथा
                प्रति( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            प्रति( j = i; j <= size.विस्तारः - 4; j += 4 )
            {
#यदि CV_SIMD128_64F
                यदि (दत्त_Type_2<sT>::गभीरता == CV_64F && दत्त_Type_2<dT>::गभीरता == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    ध्रुव द्विगुण *tsrc = (द्विगुण*)(src + j);
                    ध्रुव द्विगुण *d = (द्विगुण*)(delta_buf ? delta_buf : delta + j);

                    प्रति( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((द्विगुण)col_buf[k]);
                        s0 = v_सङ्कलय(s0, v_mul(a, v_sub(v_आरोपय(tsrc + 0), v_आरोपय(d + 0))));
                        s1 = v_सङ्कलय(s1, v_mul(a, v_sub(v_आरोपय(tsrc + 2), v_आरोपय(d + 2))));
                    }

                    v_store((द्विगुण*)(tdst+j), v_mul(s0, v_scale));
                    v_store((द्विगुण*)(tdst+j+2), v_mul(s1, v_scale));
                }
                अन्यथा
#यदि_अन्तः

                {
                    द्विगुण s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    ध्रुव sT *tsrc = src + j;
                    ध्रुव dT *d = delta_buf ? delta_buf : delta + j;

                    प्रति( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
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

            प्रति( ; j < size.विस्तारः; j++ )
            {
                द्विगुण s0 = 0;
                ध्रुव sT *tsrc = src + j;
                ध्रुव dT *d = delta_buf ? delta_buf : delta + j;

                प्रति( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (द्विगुण)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


प्रतिरूपम्<प्रकारनाम sT, प्रकारनाम dT> स्थिर शून्य
MulTransposedL(ध्रुव Mat& srcmat, ध्रुव Mat& dstmat, ध्रुव Mat& deltamat, द्विगुण scale)
{
    पूर्णाङ्क i, j, k;
    ध्रुव sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    ध्रुव dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.चरणः/परिमाण(src[0]);
    size_t dststep = dstmat.चरणः/परिमाण(dst[0]);
    size_t deltastep = deltamat.पङ्क्तिकाः > 1 ? deltamat.चरणः/परिमाण(delta[0]) : 0;
    पूर्णाङ्क delta_स्तम्भसङ्ख्या = deltamat.स्तम्भसङ्ख्या;
    आकार size = srcmat.size();
    dT* tdst = dst;

    यदि( !delta )
        प्रति( i = 0; i < size.height; i++, tdst += dststep )
            प्रति( j = i; j < size.height; j++ )
            {
                द्विगुण s = 0;
                ध्रुव sT *tsrc1 = src + i*srcstep;
                ध्रुव sT *tsrc2 = src + j*srcstep;
#यदि CV_SIMD128_64F
                यदि (दत्त_Type_2<sT>::गभीरता == CV_64F && दत्त_Type_2<dT>::गभीरता == CV_64F)
                {
                    ध्रुव द्विगुण *v_tsrc1 = (द्विगुण *)(tsrc1);
                    ध्रुव द्विगुण *v_tsrc2 = (द्विगुण *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    प्रति( k = 0; k <= size.विस्तारः - 4; k += 4 )
                        v_s = v_सङ्कलय(v_s, v_सङ्कलय(v_mul(v_आरोपय(v_tsrc1 + k), v_आरोपय(v_tsrc2 + k)), v_mul(v_आरोपय(v_tsrc1 + k + 2), v_आरोपय(v_tsrc2 + k + 2))));
                    s += v_reduce_योग(v_s);
                }
                अन्यथा
#यदि_अन्तः
                {
                    प्रति( k = 0; k <= size.विस्तारः - 4; k += 4 )
                        s += (द्विगुण)tsrc1[k]*tsrc2[k] + (द्विगुण)tsrc1[k+1]*tsrc2[k+1] +
                             (द्विगुण)tsrc1[k+2]*tsrc2[k+2] + (द्विगुण)tsrc1[k+3]*tsrc2[k+3];
                }
                प्रति( ; k < size.विस्तारः; k++ )
                    s += (द्विगुण)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    अन्यथा
    {
        dT delta_buf[4];
        पूर्णाङ्क delta_सरणम् = delta_स्तम्भसङ्ख्या == size.विस्तारः ? 4 : 0;
        Auto_बफर<uchar> buf(size.विस्तारः*परिमाण(dT));
        dT* row_buf = (dT*)buf.दत्त();

        प्रति( i = 0; i < size.height; i++, tdst += dststep )
        {
            ध्रुव sT *tsrc1 = src + i*srcstep;
            ध्रुव dT *tdelta1 = delta + i*deltastep;

            यदि( delta_स्तम्भसङ्ख्या < size.विस्तारः )
                प्रति( k = 0; k < size.विस्तारः; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            अन्यथा
                प्रति( k = 0; k < size.विस्तारः; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            प्रति( j = i; j < size.height; j++ )
            {
                द्विगुण s = 0;
                ध्रुव sT *tsrc2 = src + j*srcstep;
                ध्रुव dT *tdelta2 = delta + j*deltastep;
                यदि( delta_स्तम्भसङ्ख्या < size.विस्तारः )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#यदि CV_SIMD128_64F
                यदि (दत्त_Type_2<sT>::गभीरता == CV_64F && दत्त_Type_2<dT>::गभीरता == CV_64F)
                {
                    ध्रुव द्विगुण *v_tsrc2 = (द्विगुण *)(tsrc2);
                    ध्रुव द्विगुण *v_tdelta2 = (द्विगुण *)(tdelta2);
                    ध्रुव द्विगुण *v_row_buf = (द्विगुण *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    प्रति( k = 0; k <= size.विस्तारः - 4; k += 4, v_tdelta2 += delta_सरणम् )
                        v_s = v_सङ्कलय(v_s, v_सङ्कलय(v_mul(v_sub(v_आरोपय(v_tsrc2 + k), v_आरोपय(v_tdelta2)), v_आरोपय(v_row_buf + k)), v_mul(v_sub(v_आरोपय(v_tsrc2 + k + 2), v_आरोपय(v_tdelta2 + 2)), v_आरोपय(v_row_buf + k + 2))));
                    s += v_reduce_योग(v_s);

                    tdelta2 = (ध्रुव dT *)(v_tdelta2);
                }
                अन्यथा
#यदि_अन्तः
                {
                    प्रति( k = 0; k <= size.विस्तारः-4; k += 4, tdelta2 += delta_सरणम् )
                        s += (द्विगुण)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (द्विगुण)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (द्विगुण)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (द्विगुण)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                प्रति( ; k < size.विस्तारः; k++, tdelta2++ )
                    s += (द्विगुण)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(पूर्णाङ्क stype, पूर्णाङ्क dtype, तार्किक ata)
{
    MulTransposedFunc func = रिक्त;
    यदि (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,भिन्न>
                   : MulTransposedL<uchar,भिन्न>;
    }
    अन्यथा यदि (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,द्विगुण>
                   : MulTransposedL<uchar,द्विगुण>;
    }
    अन्यथा यदि(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,भिन्न>
                   : MulTransposedL<ushort,भिन्न>;
    }
    अन्यथा यदि(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,द्विगुण>
                   : MulTransposedL<ushort,द्विगुण>;
    }
    अन्यथा यदि(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ह्रस्व,भिन्न>
                   : MulTransposedL<ह्रस्व,भिन्न>;
    }
    अन्यथा यदि(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ह्रस्व,द्विगुण>
                   : MulTransposedL<ह्रस्व,द्विगुण>;
    }
    अन्यथा यदि(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<भिन्न,भिन्न>
                   : MulTransposedL<भिन्न,भिन्न>;
    }
    अन्यथा यदि(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<भिन्न,द्विगुण>
                   : MulTransposedL<भिन्न,द्विगुण>;
    }
    अन्यथा यदि(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<द्विगुण,द्विगुण>
                   : MulTransposedL<द्विगुण,द्विगुण>;
    }
    CV_Assert(func && "Not supported");
    प्रत्यागम func;
}
#यदि_अन्तः // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

प्रतिरूपम्<प्रकारनाम T> स्थिर अन्तःस्थ
द्विगुण dotProd_(ध्रुव T* src1, ध्रुव T* src2, पूर्णाङ्क len)
{
    पूर्णाङ्क i = 0;
    द्विगुण परिणाम = 0;

#यदि CV_ENABLE_UNROLLED
    प्रति( ; i <= len - 4; i += 4 )
        परिणाम += (द्विगुण)src1[i]*src2[i] + (द्विगुण)src1[i+1]*src2[i+1] +
            (द्विगुण)src1[i+2]*src2[i+2] + (द्विगुण)src1[i+3]*src2[i+3];
#यदि_अन्तः
    प्रति( ; i < len; i++ )
        परिणाम += (द्विगुण)src1[i]*src2[i];

    प्रत्यागम परिणाम;
}


द्विगुण dotProd_8u(ध्रुव uchar* src1, ध्रुव uchar* src2, पूर्णाङ्क len)
{
    द्विगुण r = 0;
    पूर्णाङ्क i = 0;

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णाङ्क len0 = len & -VTraits<v_uint16>::vlanes(), खण्डम्_Size0 = (1 << 15), खण्डम्_आकार_2;

    यावत् (i < len0)
    {
        खण्डम्_आकार_2 = std::min(len0 - i, खण्डम्_Size0);
        v_uint32 v_योग = vx_setzero_u32();
        ध्रुव पूर्णाङ्क c_विस्तारः = VTraits<v_uint16>::vlanes();

        पूर्णाङ्क j = 0;
        प्रति (; j <= खण्डम्_आकार_2 - c_विस्तारः * 2; j += c_विस्तारः * 2)
        {
            v_uint8 v_src1 = vx_आरोपय(src1 + j);
            v_uint8 v_src2 = vx_आरोपय(src2 + j);
            v_योग = v_dotprod_expand_fast(v_src1, v_src2, v_योग);
        }

        प्रति (; j <= खण्डम्_आकार_2 - c_विस्तारः; j += c_विस्तारः)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_आरोपय_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_आरोपय_expand(src2 + j));
            v_योग = v_सङ्कलय(v_योग, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (द्विगुण)v_reduce_योग(v_योग);

        src1 += खण्डम्_आकार_2;
        src2 += खण्डम्_आकार_2;
        i += खण्डम्_आकार_2;
    }
    vx_cleanup();
#यदि_अन्तः
    प्रत्यागम r + dotProd_(src1, src2, len - i);
}


द्विगुण dotProd_8s(ध्रुव schar* src1, ध्रुव schar* src2, पूर्णाङ्क len)
{
    द्विगुण r = 0.0;
    पूर्णाङ्क i = 0;

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णाङ्क len0 = len & -VTraits<v_int16>::vlanes(), खण्डम्_Size0 = (1 << 14), खण्डम्_आकार_2;

    यावत् (i < len0)
    {
        खण्डम्_आकार_2 = std::min(len0 - i, खण्डम्_Size0);
        v_int32 v_योग = vx_setzero_s32();
        ध्रुव पूर्णाङ्क c_विस्तारः = VTraits<v_int16>::vlanes();

        पूर्णाङ्क j = 0;
        प्रति (; j <= खण्डम्_आकार_2 - c_विस्तारः * 2; j += c_विस्तारः * 2)
        {
            v_int8 v_src1 = vx_आरोपय(src1 + j);
            v_int8 v_src2 = vx_आरोपय(src2 + j);
            v_योग = v_dotprod_expand_fast(v_src1, v_src2, v_योग);
        }

        प्रति (; j <= खण्डम्_आकार_2 - c_विस्तारः; j += c_विस्तारः)
        {
            v_int16 v_src1 = vx_आरोपय_expand(src1 + j);
            v_int16 v_src2 = vx_आरोपय_expand(src2 + j);
            v_योग = v_dotprod_fast(v_src1, v_src2, v_योग);
        }
        r += (द्विगुण)v_reduce_योग(v_योग);

        src1 += खण्डम्_आकार_2;
        src2 += खण्डम्_आकार_2;
        i += खण्डम्_आकार_2;
    }
    vx_cleanup();
#यदि_अन्तः

    प्रत्यागम r + dotProd_(src1, src2, len - i);
}

द्विगुण dotProd_16u(ध्रुव ushort* src1, ध्रुव ushort* src2, पूर्णाङ्क len)
{
    द्विगुण r = 0.0;
    पूर्णाङ्क i = 0;

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णाङ्क len0 = len & -VTraits<v_uint16>::vlanes(), खण्डम्_Size0 = (1 << 24), खण्डम्_आकार_2;

    यावत् (i < len0)
    {
        खण्डम्_आकार_2 = std::min(len0 - i, खण्डम्_Size0);
        v_uint64 v_योग = vx_setzero_u64();
        ध्रुव पूर्णाङ्क c_विस्तारः = VTraits<v_uint16>::vlanes();

        पूर्णाङ्क j = 0;
        प्रति (; j <= खण्डम्_आकार_2 - c_विस्तारः; j += c_विस्तारः)
        {
            v_uint16 v_src1 = vx_आरोपय(src1 + j);
            v_uint16 v_src2 = vx_आरोपय(src2 + j);
            v_योग = v_dotprod_expand_fast(v_src1, v_src2, v_योग);
        }
        r += (द्विगुण)v_reduce_योग(v_योग);

        src1 += खण्डम्_आकार_2;
        src2 += खण्डम्_आकार_2;
        i += खण्डम्_आकार_2;
    }
    vx_cleanup();
#यदि_अन्तः
    प्रत्यागम r + dotProd_(src1, src2, len - i);
}

द्विगुण dotProd_16s(ध्रुव ह्रस्व* src1, ध्रुव ह्रस्व* src2, पूर्णाङ्क len)
{
    द्विगुण r = 0.0;
    पूर्णाङ्क i = 0;

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णाङ्क len0 = len & -VTraits<v_int16>::vlanes(), खण्डम्_Size0 = (1 << 24), खण्डम्_आकार_2;

    यावत् (i < len0)
    {
        खण्डम्_आकार_2 = std::min(len0 - i, खण्डम्_Size0);
        v_int64 v_योग = vx_setzero_s64();
        ध्रुव पूर्णाङ्क c_विस्तारः = VTraits<v_int16>::vlanes();

        पूर्णाङ्क j = 0;
        प्रति (; j <= खण्डम्_आकार_2 - c_विस्तारः; j += c_विस्तारः)
        {
            v_int16 v_src1 = vx_आरोपय(src1 + j);
            v_int16 v_src2 = vx_आरोपय(src2 + j);
            v_योग = v_dotprod_expand_fast(v_src1, v_src2, v_योग);
        }
        r += (द्विगुण)v_reduce_योग(v_योग);

        src1 += खण्डम्_आकार_2;
        src2 += खण्डम्_आकार_2;
        i += खण्डम्_आकार_2;
    }
    vx_cleanup();
#यदि_अन्तः
    प्रत्यागम r + dotProd_(src1, src2, len - i);
}

द्विगुण dotProd_32s(ध्रुव पूर्णाङ्क* src1, ध्रुव पूर्णाङ्क* src2, पूर्णाङ्क len)
{
#यदि CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    द्विगुण r = .0;
    पूर्णाङ्क i = 0;
    ध्रुव पूर्णाङ्क चरणः  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#यदि CV_SIMD_विस्तारः == 16
    ध्रुव पूर्णाङ्क wstep = चरणः * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    प्रति (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
    {
        v_int32 v_src10 = vx_आरोपय(src1);
        v_int32 v_src20 = vx_आरोपय(src2);
        v_int32 v_src11 = vx_आरोपय(src1 + चरणः);
        v_int32 v_src21 = vx_आरोपय(src2 + चरणः);
        v_sum0 = v_dotprod_expand_fast(v_src10, v_src20, v_sum0);
        v_sum1 = v_dotprod_expand_fast(v_src11, v_src21, v_sum1);
    }
    v_sum0 = v_सङ्कलय(v_sum0, v_sum1);
#यदि_अन्तः
    प्रति (; i <= len - चरणः; i += चरणः, src1 += चरणः, src2 += चरणः)
    {
        v_int32 v_src1 = vx_आरोपय(src1);
        v_int32 v_src2 = vx_आरोपय(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_योग(v_sum0);
    vx_cleanup();
    प्रत्यागम r + dotProd_(src1, src2, len - i);
#अन्यथा
    प्रत्यागम dotProd_(src1, src2, len);
#यदि_अन्तः
}

द्विगुण dotProd_32f(ध्रुव भिन्न* src1, ध्रुव भिन्न* src2, पूर्णाङ्क len)
{
    द्विगुण r = 0.0;
    पूर्णाङ्क i = 0;

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णाङ्क len0 = len & -VTraits<v_float32>::vlanes(), खण्डम्_Size0 = (1 << 13), खण्डम्_आकार_2;

    यावत् (i < len0)
    {
        खण्डम्_आकार_2 = std::min(len0 - i, खण्डम्_Size0);
        v_float32 v_योग = vx_setzero_f32();

        पूर्णाङ्क j = 0;
        पूर्णाङ्क c_विस्तारः = VTraits<v_float32>::vlanes();

#यदि CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        प्रति (; j <= खण्डम्_आकार_2 - (c_विस्तारः * 4); j += (c_विस्तारः * 4))
        {
            v_योग  = v_muladd(vx_आरोपय(src1 + j),
                              vx_आरोपय(src2 + j), v_योग);
            v_sum1 = v_muladd(vx_आरोपय(src1 + j + c_विस्तारः),
                              vx_आरोपय(src2 + j + c_विस्तारः), v_sum1);
            v_sum2 = v_muladd(vx_आरोपय(src1 + j + (c_विस्तारः * 2)),
                              vx_आरोपय(src2 + j + (c_विस्तारः * 2)), v_sum2);
            v_sum3 = v_muladd(vx_आरोपय(src1 + j + (c_विस्तारः * 3)),
                              vx_आरोपय(src2 + j + (c_विस्तारः * 3)), v_sum3);
        }

        v_योग = v_सङ्कलय(v_योग, v_सङ्कलय(v_सङ्कलय(v_sum1, v_sum2), v_sum3));
#यदि_अन्तः

        प्रति (; j <= खण्डम्_आकार_2 - c_विस्तारः; j += c_विस्तारः)
            v_योग = v_muladd(vx_आरोपय(src1 + j), vx_आरोपय(src2 + j), v_योग);

        r += v_reduce_योग(v_योग);

        src1 += खण्डम्_आकार_2;
        src2 += खण्डम्_आकार_2;
        i += खण्डम्_आकार_2;
    }
    vx_cleanup();
#यदि_अन्तः
    प्रत्यागम r + dotProd_(src1, src2, len - i);
}

द्विगुण dotProd_64f(ध्रुव द्विगुण* src1, ध्रुव द्विगुण* src2, पूर्णाङ्क len)
{
    प्रत्यागम dotProd_(src1, src2, len);
}

#यदि_अन्तः
CV_CPU_OPTIMIZATION_NAMESPACE_अन्तः
} // namespace
