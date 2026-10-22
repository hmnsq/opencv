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

#समावेश "precomp.hpp"

#अगर_घोषित HAVE_LAPACK
#घोषणा CV_GEMM_BASELINE_ONLY
#पूर्ण_अगर
#घोषणा CV_MAHALANOBIS_BASELINE_ONLY
#घोषणा CV_MULTRANSPOSED_BASELINE_ONLY

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
#अगर_अघोषित CV_GEMM_SIMD_ENABLED
#अगर defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#घोषणा CV_GEMM_SIMD_ENABLED 0
#अन्यथा
#घोषणा CV_GEMM_SIMD_ENABLED 1
#पूर्ण_अगर
#पूर्ण_अगर

namespace cv {

// forward declarations
प्रकार व्योम (*TransformFunc)(स्थिर व्योम* src, व्योम* dst, स्थिर व्योम* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn);
प्रकार व्योम (*ScaleAddFunc)(स्थिर व्योम* src1, स्थिर व्योम* src2, व्योम* dst, पूर्णांक len, स्थिर व्योम* alpha);
प्रकार व्योम (*MulTransposedFunc)(स्थिर Mat& src, स्थिर/*preallocated*/ Mat& dst, स्थिर Mat& delta, द्विगुन scale);
प्रकार द्विगुन (*MahalanobisImplFunc)(स्थिर Mat& v1, स्थिर Mat& v2, स्थिर Mat& icovar, द्विगुन *diff_बफर /*[len]*/, पूर्णांक len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

व्योम gemm32f(स्थिर भग्न* src1, माप_प्रकार src1_step, स्थिर भग्न* src2, माप_प्रकार src2_step,
             भग्न alpha, स्थिर भग्न* src3, माप_प्रकार src3_step, भग्न beta, भग्न* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags);
व्योम gemm64f(स्थिर द्विगुन* src1, माप_प्रकार src1_step, स्थिर द्विगुन* src2, माप_प्रकार src2_step,
             द्विगुन alpha, स्थिर द्विगुन* src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags);
व्योम gemm32fc(स्थिर भग्न* src1, माप_प्रकार src1_step, स्थिर भग्न* src2, माप_प्रकार src2_step,
              भग्न alpha, स्थिर भग्न* src3, माप_प्रकार src3_step, भग्न beta, भग्न* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags);
व्योम gemm64fc(स्थिर द्विगुन* src1, माप_प्रकार src1_step, स्थिर द्विगुन* src2, माप_प्रकार src2_step,
              द्विगुन alpha, स्थिर द्विगुन* src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags);

TransformFunc getTransformFunc(पूर्णांक depth);
TransformFunc getDiagTransformFunc(पूर्णांक depth);
TransformFunc getPerspectiveTransform(पूर्णांक depth);

ScaleAddFunc getScaleAddFunc(पूर्णांक depth);

MahalanobisImplFunc getMahalanobisImplFunc(पूर्णांक depth);

MulTransposedFunc getMulTransposedFunc(पूर्णांक stype, पूर्णांक dtype, bool ata);

द्विगुन dotProd_8u(स्थिर uchar* src1, स्थिर uchar* src2, पूर्णांक len);
द्विगुन dotProd_8s(स्थिर schar* src1, स्थिर schar* src2, पूर्णांक len);
द्विगुन dotProd_16u(स्थिर ushort* src1, स्थिर ushort* src2, पूर्णांक len);
द्विगुन dotProd_16s(स्थिर लघु* src1, स्थिर लघु* src2, पूर्णांक len);
द्विगुन dotProd_32s(स्थिर पूर्णांक* src1, स्थिर पूर्णांक* src2, पूर्णांक len);
द्विगुन dotProd_32f(स्थिर भग्न* src1, स्थिर भग्न* src2, पूर्णांक len);
द्विगुन dotProd_64f(स्थिर द्विगुन* src1, स्थिर द्विगुन* src2, पूर्णांक len);



#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#अगर !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

अटल व्योम
GEMM_CopyBlock( स्थिर uchar* src, माप_प्रकार src_step,
                uchar* dst, माप_प्रकार dst_step,
                आकार size, माप_प्रकार pix_आकार )
{
    पूर्णांक j;
    size.width *= (पूर्णांक)(pix_आकार / माप(पूर्णांक));

    क्रम( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #अगर CV_ENABLE_UNROLLED
        क्रम( ; j <= size.width - 4; j += 4 )
        {
            पूर्णांक t0 = ((स्थिर पूर्णांक*)src)[j];
            पूर्णांक t1 = ((स्थिर पूर्णांक*)src)[j+1];
            ((पूर्णांक*)dst)[j] = t0;
            ((पूर्णांक*)dst)[j+1] = t1;
            t0 = ((स्थिर पूर्णांक*)src)[j+2];
            t1 = ((स्थिर पूर्णांक*)src)[j+3];
            ((पूर्णांक*)dst)[j+2] = t0;
            ((पूर्णांक*)dst)[j+3] = t1;
        }
        #पूर्ण_अगर
        क्रम( ; j < size.width; j++ )
            ((पूर्णांक*)dst)[j] = ((स्थिर पूर्णांक*)src)[j];
    }
}


अटल व्योम
GEMM_TransposeBlock( स्थिर uchar* src, माप_प्रकार src_step,
                     uchar* dst, माप_प्रकार dst_step,
                     आकार size, माप_प्रकार pix_आकार )
{
    पूर्णांक i, j;
    क्रम( i = 0; i < size.width; i++, dst += dst_step, src += pix_आकार )
    {
        स्थिर uchar* _src = src;
        चयन( pix_आकार )
        {
        हाल माप(पूर्णांक):
            क्रम( j = 0; j < size.height; j++, _src += src_step )
                ((पूर्णांक*)dst)[j] = ((पूर्णांक*)_src)[0];
            अवरोध;
        हाल माप(पूर्णांक)*2:
            क्रम( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                पूर्णांक t0 = ((पूर्णांक*)_src)[0];
                पूर्णांक t1 = ((पूर्णांक*)_src)[1];
                ((पूर्णांक*)dst)[j] = t0;
                ((पूर्णांक*)dst)[j+1] = t1;
            }
            अवरोध;
        हाल माप(पूर्णांक)*4:
            क्रम( j = 0; j < size.height*4; j += 4, _src += src_step )
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
            अवरोध;
        शेष:
            CV_Assert(0);
            वापस;
        }
    }
}

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<typename ST> काष्ठा VecTraits;

ढाँचा<> काष्ठा VecTraits<भग्न> {
    प्रकार v_float32 VT;
    अटल अंतरभूत VT zero() { वापस vx_setzero_f32(); }
    अटल अंतरभूत VT load(स्थिर भग्न* p) { वापस vx_load(p); }
    अटल अंतरभूत VT setall(भग्न v) { वापस vx_setall_f32(v); }
};
#पूर्ण_अगर

#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ढाँचा<> काष्ठा VecTraits<द्विगुन> {
    प्रकार v_float64 VT;
    अटल अंतरभूत VT zero() { वापस vx_setzero_f64(); }
    अटल अंतरभूत VT load(स्थिर द्विगुन* p) { वापस vx_load(p); }
    अटल अंतरभूत VT setall(द्विगुन v) { वापस vx_setall_f64(v); }
};
#पूर्ण_अगर

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<typename ST>
अटल अंतरभूत ST simdDotProduct(स्थिर ST* a, स्थिर ST* b, पूर्णांक n, पूर्णांक& k)
{
    प्रकार typename VecTraits<ST>::VT VecT;
    स्थिर पूर्णांक vlanes = VTraits<VecT>::vlanes();
    स्थिर पूर्णांक stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    क्रम( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    क्रम( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    वापस v_reduce_योग(vs0);
}
#पूर्ण_अगर

#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
अटल अंतरभूत द्विगुन simdDotProduct_f32f64(स्थिर भग्न* a, स्थिर भग्न* b, पूर्णांक n, पूर्णांक& k)
{
    स्थिर पूर्णांक vlanes_f32 = VTraits<v_float32>::vlanes();
    स्थिर पूर्णांक stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    क्रम( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_ऊपरी(va0), v_cvt_f64_ऊपरी(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_ऊपरी(va1), v_cvt_f64_ऊपरी(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    क्रम( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_ऊपरी(va), v_cvt_f64_ऊपरी(vb), vs0);
    }
    वापस v_reduce_योग(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
अटल अंतरभूत व्योम simdGEMM_kj_f32f64(
    स्थिर भग्न* a_तथ्य, स्थिर भग्न* b_तथ्य_2, माप_प्रकार b_step,
    स्थिर भग्न* c_तथ्य, माप_प्रकार c_step1,
    भग्न* d_तथ्य,
    पूर्णांक n, पूर्णांक m,
    द्विगुन alpha, द्विगुन beta,
    पूर्णांक& j)
{
    स्थिर पूर्णांक vlanes_f32 = VTraits<v_float32>::vlanes();
    स्थिर पूर्णांक vlanes_f64 = VTraits<v_float64>::vlanes();

    अगर( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        स्थिर पूर्णांक max_m = 1600 / (पूर्णांक)माप(भग्न);
        cv::Auto_बफर<द्विगुन> _s_buf(max_m);
        द्विगुन* s_buf = _s_buf.तथ्य();

        {
            v_float64 vz = vx_setzero_f64();
            क्रम( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            क्रम( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            स्थिर भग्न* b_ptr = b_तथ्य_2;
            क्रम( पूर्णांक k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((द्विगुन)a_तथ्य[k]);
                j = 0;
                क्रम( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_ऊपरी(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                द्विगुन a_val = (द्विगुन)a_तथ्य[k];
                क्रम( ; j < m; j++ )
                    s_buf[j] += a_val * (द्विगुन)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            स्थिर भग्न* c_तथ्य_2 = c_तथ्य;
            j = 0;
            अगर( !c_तथ्य )
            {
                क्रम( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_तथ्य + j, v_cvt_f32(vs_lo, vs_hi));
                }
                क्रम( ; j < m; j++ )
                    d_तथ्य[j] = (भग्न)(s_buf[j] * alpha);
            }
            अन्यथा अगर( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                क्रम( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_तथ्य_2 + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_ऊपरी(vc), vbeta, vs_hi);
                    v_store(d_तथ्य + j, v_cvt_f32(vs_lo, vs_hi));
                }
                क्रम( ; j < m; j++ )
                    d_तथ्य[j] = (भग्न)(s_buf[j] * alpha + (द्विगुन)c_तथ्य_2[j] * beta);
            }
            अन्यथा
            {
                क्रम( j = 0; j < m; j++, c_तथ्य_2 += c_step1 )
                    d_तथ्य[j] = (भग्न)(s_buf[j] * alpha + (द्विगुन)c_तथ्य_2[0] * beta);
            }
        }
        j = m;
    }
    अन्यथा अगर( vlanes_f32 * (पूर्णांक)माप(भग्न) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        स्थिर भग्न* c_तथ्य_2 = c_तथ्य;
        क्रम( ; j <= m - vlanes_f32; j += vlanes_f32, c_तथ्य_2 += vlanes_f32 * c_step1 )
        {
            स्थिर भग्न* b = b_तथ्य_2 + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            क्रम( पूर्णांक k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((द्विगुन)a_तथ्य[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_ऊपरी(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            अगर( !c_तथ्य )
            {
                v_store(d_तथ्य + j, v_cvt_f32(vs_lo, vs_hi));
            }
            अन्यथा अगर( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_तथ्य_2);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_ऊपरी(vc), vbeta, vs_hi);
                v_store(d_तथ्य + j, v_cvt_f32(vs_lo, vs_hi));
            }
            अन्यथा
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) भग्न buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                क्रम( पूर्णांक jj = 0; jj < vlanes_f32; jj++ )
                    d_तथ्य[j + jj] = buf[jj] + c_तथ्य_2[jj * c_step1] * (भग्न)beta;
            }
        }
    }
}
#पूर्ण_अगर

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<typename ST>
अटल अंतरभूत व्योम simdGEMM_kj(
    स्थिर ST* a_तथ्य, स्थिर ST* b_तथ्य_2, माप_प्रकार b_step,
    स्थिर ST* c_तथ्य, माप_प्रकार c_step1,
    ST* d_तथ्य,
    पूर्णांक n, पूर्णांक m,
    द्विगुन alpha, द्विगुन beta,
    पूर्णांक& j)
{
    प्रकार typename VecTraits<ST>::VT VecT;
    स्थिर पूर्णांक vlanes = VTraits<VecT>::vlanes();

    अगर( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        स्थिर पूर्णांक max_m = 1600 / (पूर्णांक)माप(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            क्रम( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            क्रम( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            स्थिर ST* b_ptr = b_तथ्य_2;
            क्रम( पूर्णांक k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_तथ्य[k]);
                j = 0;
                क्रम( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_तथ्य[k];
                क्रम( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            स्थिर ST* c_तथ्य_2 = c_तथ्य;
            j = 0;
            अगर( !c_तथ्य )
            {
                क्रम( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_तथ्य + j, vs);
                }
                क्रम( ; j < m; j++ )
                    d_तथ्य[j] = s_buf[j] * (ST)alpha;
            }
            अन्यथा अगर( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                क्रम( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_तथ्य_2 + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_तथ्य + j, vs);
                }
                क्रम( ; j < m; j++ )
                    d_तथ्य[j] = s_buf[j] * (ST)alpha + c_तथ्य_2[j] * (ST)beta;
            }
            अन्यथा
            {
                क्रम( j = 0; j < m; j++, c_तथ्य_2 += c_step1 )
                    d_तथ्य[j] = s_buf[j] * (ST)alpha + c_तथ्य_2[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    अन्यथा अगर( vlanes * (पूर्णांक)माप(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        स्थिर ST* c_तथ्य_2 = c_तथ्य;
        क्रम( ; j <= m - vlanes; j += vlanes, c_तथ्य_2 += vlanes * c_step1 )
        {
            स्थिर ST* b = b_तथ्य_2 + j;
            VecT vs = VecTraits<ST>::zero();
            क्रम( पूर्णांक k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_तथ्य[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            अगर( !c_तथ्य )
            {
                v_store(d_तथ्य + j, vs);
            }
            अन्यथा अगर( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_तथ्य_2);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_तथ्य + j, vs);
            }
            अन्यथा
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                क्रम( पूर्णांक jj = 0; jj < vlanes; jj++ )
                    d_तथ्य[j + jj] = buf[jj] + c_तथ्य_2[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#पूर्ण_अगर


ढाँचा<typename T, typename WT> अटल व्योम
GEMMSingleMul( स्थिर T* a_तथ्य, माप_प्रकार a_step,
               स्थिर T* b_तथ्य, माप_प्रकार b_step,
               स्थिर T* c_तथ्य_2, माप_प्रकार c_step,
               T* d_तथ्य, माप_प्रकार d_step,
               आकार a_आकार, आकार d_आकार,
               द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    पूर्णांक i, j, k, n = a_आकार.width, m = d_आकार.width, drows = d_आकार.height;
    स्थिर T *a_तथ्य_2 = a_तथ्य, *b_तथ्य_2 = b_तथ्य, *c_तथ्य = c_तथ्य_2;
    cv::Auto_बफर<T> _a_buf;
    T* a_buf = 0;
    माप_प्रकार a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= माप(a_तथ्य[0]);
    b_step /= माप(b_तथ्य[0]);
    c_step /= माप(c_तथ्य_2[0]);
    d_step /= माप(d_तथ्य[0]);
    a_step0 = a_step;
    a_step1 = 1;

    अगर( !c_तथ्य_2 )
        c_step0 = c_step1 = 0;
    अन्यथा अगर( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    अन्यथा
        c_step0 = 1, c_step1 = c_step;

    अगर( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_आकार.height;
        अगर( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.तथ्य();
        }
    }

    अगर( n == 1 ) /* external product */
    {
        cv::Auto_बफर<T> _b_buf;
        T* b_buf = 0;

        अगर( a_step > 1 && a_आकार.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.तथ्य();
            क्रम( k = 0; k < drows; k++ )
                a_buf[k] = a_तथ्य[a_step*k];
            a_तथ्य = a_buf;
        }

        अगर( b_step > 1 )
        {
            _b_buf.allocate(d_आकार.width);
            b_buf = _b_buf.तथ्य();
            क्रम( j = 0; j < d_आकार.width; j++ )
                b_buf[j] = b_तथ्य[j*b_step];
            b_तथ्य = b_buf;
        }

        क्रम( i = 0; i < drows; i++, c_तथ्य += c_step0, d_तथ्य += d_step )
        {
            WT al = WT(a_तथ्य[i])*alpha;
            c_तथ्य_2 = c_तथ्य;
            क्रम( j = 0; j <= d_आकार.width - 2; j += 2, c_तथ्य_2 += 2*c_step1 )
            {
                WT s0 = al*WT(b_तथ्य[j]);
                WT s1 = al*WT(b_तथ्य[j+1]);
                अगर( !c_तथ्य_2 )
                {
                    d_तथ्य[j] = T(s0);
                    d_तथ्य[j+1] = T(s1);
                }
                अन्यथा
                {
                    d_तथ्य[j] = T(s0 + WT(c_तथ्य_2[0])*beta);
                    d_तथ्य[j+1] = T(s1 + WT(c_तथ्य_2[c_step1])*beta);
                }
            }

            क्रम( ; j < d_आकार.width; j++, c_तथ्य_2 += c_step1 )
            {
                WT s0 = al*WT(b_तथ्य[j]);
                अगर( !c_तथ्य_2 )
                    d_तथ्य[j] = T(s0);
                अन्यथा
                    d_तथ्य[j] = T(s0 + WT(c_तथ्य_2[0])*beta);
            }
        }
    }
    अन्यथा अगर( flags & GEMM_2_T ) /* A * Bt */
    {
        क्रम( i = 0; i < drows; i++, a_तथ्य_2 += a_step0, c_तथ्य += c_step0, d_तथ्य += d_step )
        {
            a_तथ्य = a_तथ्य_2;
            b_तथ्य = b_तथ्य_2;
            c_तथ्य_2 = c_तथ्य;

            अगर( a_buf )
            {
                क्रम( k = 0; k < n; k++ )
                    a_buf[k] = a_तथ्य[a_step1*k];
                a_तथ्य = a_buf;
            }

            क्रम( j = 0; j < d_आकार.width; j++, b_तथ्य += b_step,
                                               c_तथ्य_2 += c_step1 )
            {
                WT s0(0);
                k = 0;
#अगर CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                अगर( माप(WT) == माप(द्विगुन) )
                {
                    अगर( माप(T) == माप(द्विगुन) )
                        s0 = (WT)simdDotProduct((स्थिर द्विगुन*)a_तथ्य, (स्थिर द्विगुन*)b_तथ्य, n, k);
                    अन्यथा अगर( माप(T) == माप(भग्न) )
                        s0 = (WT)simdDotProduct_f32f64((स्थिर भग्न*)a_तथ्य, (स्थिर भग्न*)b_तथ्य, n, k);
                }
                अन्यथा
#पूर्ण_अगर
                {
                    WT s1(0), s2(0), s3(0);
                     #अगर CV_ENABLE_UNROLLED
                    क्रम( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_तथ्य[k])*WT(b_तथ्य[k]);
                        s1 += WT(a_तथ्य[k+1])*WT(b_तथ्य[k+1]);
                        s2 += WT(a_तथ्य[k+2])*WT(b_तथ्य[k+2]);
                        s3 += WT(a_तथ्य[k+3])*WT(b_तथ्य[k+3]);
                    }
                    #पूर्ण_अगर
                    s0 += s1 + s2 + s3;
                }
                क्रम( ; k < n; k++ )
                    s0 += WT(a_तथ्य[k])*WT(b_तथ्य[k]);
                s0 = s0*alpha;

                अगर( !c_तथ्य_2 )
                    d_तथ्य[j] = T(s0);
                अन्यथा
                    d_तथ्य[j] = T(s0 + WT(c_तथ्य_2[0])*beta);
            }
        }
    }
    अन्यथा अगर( d_आकार.width*माप(d_तथ्य[0]) <= 1600 )
    {
        क्रम( i = 0; i < drows; i++, a_तथ्य_2 += a_step0,
                                    c_तथ्य += c_step0,
                                    d_तथ्य += d_step )
        {
            a_तथ्य = a_तथ्य_2, c_तथ्य_2 = c_तथ्य;

            अगर( a_buf )
            {
                क्रम( k = 0; k < n; k++ )
                    a_buf[k] = a_तथ्य[a_step1*k];
                a_तथ्य = a_buf;
            }

            j = 0;
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            अगर( माप(WT) == माप(द्विगुन) )
            {
                अगर( माप(T) == माप(द्विगुन) )
                    simdGEMM_kj((स्थिर द्विगुन*)a_तथ्य, (स्थिर द्विगुन*)b_तथ्य_2, b_step,
                                        (स्थिर द्विगुन*)c_तथ्य, c_step1,
                                (द्विगुन*)d_तथ्य, n, m, alpha, beta, j);
                अन्यथा अगर( माप(T) == माप(भग्न) )
                    simdGEMM_kj_f32f64((स्थिर भग्न*)a_तथ्य, (स्थिर भग्न*)b_तथ्य_2, b_step,
                                        (स्थिर भग्न*)c_तथ्य, c_step1,
                                (भग्न*)d_तथ्य, n, m, alpha, beta, j);
            }
#पूर्ण_अगर
            c_तथ्य_2 = c_तथ्य + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            क्रम( ; j <= m - 4; j += 4, c_तथ्य_2 += 4*c_step1 )
            {
                स्थिर T* b = b_तथ्य_2 + j;
                WT s0(0), s1(0), s2(0), s3(0);

                क्रम( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_तथ्य[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                अगर( !c_तथ्य_2 )
                {
                    d_तथ्य[j] = T(s0*alpha);
                    d_तथ्य[j+1] = T(s1*alpha);
                    d_तथ्य[j+2] = T(s2*alpha);
                    d_तथ्य[j+3] = T(s3*alpha);
                }
                अन्यथा
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_तथ्य[j] = T(s0 + WT(c_तथ्य_2[0])*beta);
                    d_तथ्य[j+1] = T(s1 + WT(c_तथ्य_2[c_step1])*beta);
                    d_तथ्य[j+2] = T(s2 + WT(c_तथ्य_2[c_step1*2])*beta);
                    d_तथ्य[j+3] = T(s3 + WT(c_तथ्य_2[c_step1*3])*beta);
                }
            }

            क्रम( ; j < m; j++, c_तथ्य_2 += c_step1 )
            {
                स्थिर T* b = b_तथ्य_2 + j;
                WT s0(0);

                क्रम( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_तथ्य[k]) * WT(b[0]);

                s0 = s0*alpha;
                अगर( !c_तथ्य_2 )
                    d_तथ्य[j] = T(s0);
                अन्यथा
                    d_तथ्य[j] = T(s0 + WT(c_तथ्य_2[0])*beta);
            }
        }
    }
    अन्यथा
    {
        cv::Auto_बफर<WT> _d_buf(m);
        WT* d_buf = _d_buf.तथ्य();

        क्रम( i = 0; i < drows; i++, a_तथ्य_2 += a_step0, c_तथ्य += c_step0, d_तथ्य += d_step )
        {
            a_तथ्य = a_तथ्य_2;
            b_तथ्य = b_तथ्य_2;
            c_तथ्य_2 = c_तथ्य;

            अगर( a_buf )
            {
                क्रम( k = 0; k < n; k++ )
                    a_buf[k] = a_तथ्य_2[a_step1*k];
                a_तथ्य = a_buf;
            }

            क्रम( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            क्रम( k = 0; k < n; k++, b_तथ्य += b_step )
            {
                WT al(a_तथ्य[k]);
                j=0;
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                अगर( माप(WT) == माप(द्विगुन) )
                {
                    अगर( माप(T) == माप(द्विगुन) )
                    {
                        स्थिर पूर्णांक vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((स्थिर द्विगुन*)&al));
                        क्रम( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((स्थिर द्विगुन*)(d_buf + j));
                            v_float64 vb = vx_load((स्थिर द्विगुन*)(b_तथ्य + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((द्विगुन*)(d_buf + j), vd);
                        }
                    }
                    अन्यथा
                    {
                        स्थिर पूर्णांक vlanes_f32 = VTraits<v_float32>::vlanes();
                        स्थिर पूर्णांक vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((स्थिर द्विगुन*)&al));
                        क्रम( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((स्थिर भग्न*)(b_तथ्य + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_ऊपरी(vb);
                            v_float64 vd_lo = vx_load((स्थिर द्विगुन*)(d_buf + j));
                            v_float64 vd_hi = vx_load((स्थिर द्विगुन*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((द्विगुन*)(d_buf + j), vd_lo);
                            v_store((द्विगुन*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#पूर्ण_अगर
                क्रम( ; j < m; j++ )
                    d_buf[j] += WT(b_तथ्य[j])*al;
            }

            अगर( !c_तथ्य_2 )
                क्रम( j = 0; j < m; j++ )
                    d_तथ्य[j] = T(d_buf[j]*alpha);
            अन्यथा
                क्रम( j = 0; j < m; j++, c_तथ्य_2 += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_तथ्य[j] = T(t + WT(c_तथ्य_2[0])*beta);
                }
        }
    }
}
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ढाँचा<typename WT>
अटल अंतरभूत व्योम set_Double_तथ्य_Zero(WT* d_तथ्य, पूर्णांक m, पूर्णांक vlanes)
{
    v_float64 vz = vx_setzero_f64();
    पूर्णांक j = 0;
    क्रम( ; j <= m - vlanes; j += vlanes )
        v_store((द्विगुन*)(d_तथ्य + j), vz);
    क्रम( ; j < m; j++ )
        d_तथ्य[j] = WT(0);
}

ढाँचा<typename ST>
अटल अंतरभूत व्योम simdMulAdd(स्थिर ST* b, द्विगुन* d, पूर्णांक m, द्विगुन a_val);

ढाँचा<>
अंतरभूत व्योम simdMulAdd<भग्न>(
    स्थिर भग्न* b, द्विगुन* d, पूर्णांक m, द्विगुन a_val)
{
    स्थिर पूर्णांक vlanes_f32 = VTraits<v_float32>::vlanes();
    स्थिर पूर्णांक vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    पूर्णांक j = 0;
    क्रम( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_ऊपरी(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    क्रम( ; j < m; j++ )
        d[j] += a_val * (द्विगुन)b[j];
}

ढाँचा<>
अंतरभूत व्योम simdMulAdd<द्विगुन>(
    स्थिर द्विगुन* b, द्विगुन* d, पूर्णांक m, द्विगुन a_val)
{
    स्थिर पूर्णांक vlanes = VTraits<v_float64>::vlanes();
    स्थिर पूर्णांक stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    पूर्णांक j = 0;
    क्रम( ; j <= m - stride2; j += stride2 )
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
    क्रम( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    क्रम( ; j < m; j++ )
        d[j] += a_val * b[j];
}

ढाँचा<typename ST>
अटल अंतरभूत व्योम simdBlockMul_kj(
    स्थिर ST* a_तथ्य, स्थिर ST* b_तथ्य, माप_प्रकार b_step,
    द्विगुन* d_तथ्य, पूर्णांक n, पूर्णांक m, bool do_acc)
{
    स्थिर पूर्णांक vlanes = VTraits<v_float64>::vlanes();
    अगर( !do_acc )
        set_Double_तथ्य_Zero(d_तथ्य, m, vlanes);
    क्रम( पूर्णांक k = 0; k < n; k++, b_तथ्य += b_step )
        simdMulAdd(b_तथ्य, d_तथ्य, m, (द्विगुन)a_तथ्य[k]);
}
#पूर्ण_अगर // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

ढाँचा<typename T, typename WT> अटल व्योम
GEMMBlockMul( स्थिर T* a_तथ्य, माप_प्रकार a_step,
              स्थिर T* b_तथ्य, माप_प्रकार b_step,
              WT* d_तथ्य, माप_प्रकार d_step,
              आकार a_आकार, आकार d_आकार, पूर्णांक flags )
{
    पूर्णांक i, j, k, n = a_आकार.width, m = d_आकार.width;
    स्थिर T *a_तथ्य_2 = a_तथ्य, *b_तथ्य_2 = b_तथ्य;
    cv::Auto_बफर<T> _a_buf;
    T* a_buf = 0;
    माप_प्रकार a_step0, a_step1, t_step;
    पूर्णांक do_acc = flags & 16;

    a_step /= माप(a_तथ्य[0]);
    b_step /= माप(b_तथ्य[0]);
    d_step /= माप(d_तथ्य[0]);

    a_step0 = a_step;
    a_step1 = 1;

    अगर( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_आकार.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.तथ्य();
    }

    अगर( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        क्रम( i = 0; i < d_आकार.height; i++, a_तथ्य_2 += a_step0, d_तथ्य += d_step )
        {
            a_तथ्य = a_तथ्य_2; b_तथ्य = b_तथ्य_2;

            अगर( a_buf )
            {
                क्रम( k = 0; k < n; k++ )
                    a_buf[k] = a_तथ्य[a_step1*k];
                a_तथ्य = a_buf;
            }

            क्रम( j = 0; j < d_आकार.width; j++, b_तथ्य += b_step )
            {
                WT s0 = do_acc ? d_तथ्य[j] : WT(0);
                k = 0;
#अगर CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                अगर( माप(WT) == माप(द्विगुन) )
                {
                    अगर( माप(T) == माप(द्विगुन) )
                        s0 += (WT)simdDotProduct((स्थिर द्विगुन*)a_तथ्य, (स्थिर द्विगुन*)b_तथ्य, n, k);
                    अन्यथा अगर( माप(T) == माप(भग्न) )
                        s0 += (WT)simdDotProduct_f32f64((स्थिर भग्न*)a_तथ्य, (स्थिर भग्न*)b_तथ्य, n, k);
                }
                अन्यथा
#पूर्ण_अगर
                {
                    WT s1(0);
                    क्रम( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_तथ्य[k])*WT(b_तथ्य[k]);
                        s1 += WT(a_तथ्य[k+1])*WT(b_तथ्य[k+1]);
                    }
                    s0 += s1;
                }
                क्रम( ; k < n; k++ )
                    s0 += WT(a_तथ्य[k])*WT(b_तथ्य[k]);

                d_तथ्य[j] = s0;
            }
        }
    }
    अन्यथा
    {
        क्रम( i = 0; i < d_आकार.height; i++, a_तथ्य_2 += a_step0, d_तथ्य += d_step )
        {
            a_तथ्य = a_तथ्य_2, b_तथ्य = b_तथ्य_2;

            अगर( a_buf )
            {
                क्रम( k = 0; k < n; k++ )
                    a_buf[k] = a_तथ्य[a_step1*k];
                a_तथ्य = a_buf;
            }
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            अगर( माप(WT) == माप(द्विगुन) )
            {
                अगर( माप(T) == माप(द्विगुन) )
                    simdBlockMul_kj(
                        (स्थिर द्विगुन*)a_तथ्य, (स्थिर द्विगुन*)b_तथ्य, b_step,
                        (द्विगुन*)d_तथ्य, n, m, do_acc != 0);
                अन्यथा
                    simdBlockMul_kj(
                        (स्थिर भग्न*)a_तथ्य, (स्थिर भग्न*)b_तथ्य, b_step,
                        (द्विगुन*)d_तथ्य, n, m, do_acc != 0);
            }
            अन्यथा
#पूर्ण_अगर
            {
                क्रम( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    स्थिर T* b = b_तथ्य + j;

                    अगर( do_acc )
                    {
                        s0 = d_तथ्य[j]; s1 = d_तथ्य[j+1];
                        s2 = d_तथ्य[j+2]; s3 = d_तथ्य[j+3];
                    }
                    अन्यथा
                        s0 = s1 = s2 = s3 = WT(0);

                    क्रम( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_तथ्य[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_तथ्य[j] = s0; d_तथ्य[j+1] = s1;
                    d_तथ्य[j+2] = s2; d_तथ्य[j+3] = s3;
                }

                क्रम( ; j < m; j++ )
                {
                    स्थिर T* b = b_तथ्य + j;
                    WT s0 = do_acc ? d_तथ्य[j] : WT(0);

                    क्रम( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_तथ्य[k]) * WT(b[0]);

                    d_तथ्य[j] = s0;
                }
            }
        }
    }
}


ढाँचा<typename T, typename WT> अटल व्योम
GEMMStore( स्थिर T* c_तथ्य_2, माप_प्रकार c_step,
           स्थिर WT* d_buf, माप_प्रकार d_buf_step,
           T* d_तथ्य, माप_प्रकार d_step, आकार d_आकार,
           द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    स्थिर T* c_तथ्य = c_तथ्य_2;
    पूर्णांक j;
    माप_प्रकार c_step0, c_step1;

    c_step /= माप(c_तथ्य_2[0]);
    d_buf_step /= माप(d_buf[0]);
    d_step /= माप(d_तथ्य[0]);

    अगर( !c_तथ्य_2 )
        c_step0 = c_step1 = 0;
    अन्यथा अगर( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    अन्यथा
        c_step0 = 1, c_step1 = c_step;

    क्रम( ; d_आकार.height--; c_तथ्य += c_step0, d_buf += d_buf_step, d_तथ्य += d_step )
    {
        अगर( c_तथ्य )
        {
            c_तथ्य_2 = c_तथ्य;
            j=0;
             #अगर CV_ENABLE_UNROLLED
            क्रम(; j <= d_आकार.width - 4; j += 4, c_तथ्य_2 += 4*c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                t0 += beta*WT(c_तथ्य_2[0]);
                t1 += beta*WT(c_तथ्य_2[c_step1]);
                d_तथ्य[j] = T(t0);
                d_तथ्य[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                t0 += beta*WT(c_तथ्य_2[c_step1*2]);
                t1 += beta*WT(c_तथ्य_2[c_step1*3]);
                d_तथ्य[j+2] = T(t0);
                d_तथ्य[j+3] = T(t1);
            }
            #पूर्ण_अगर
            क्रम( ; j < d_आकार.width; j++, c_तथ्य_2 += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_तथ्य[j] = T(t0 + WT(c_तथ्य_2[0])*beta);
            }
        }
        अन्यथा
        {
            j = 0;
             #अगर CV_ENABLE_UNROLLED
            क्रम( ; j <= d_आकार.width - 4; j += 4 )
            {
                WT t0 = alpha*d_buf[j];
                WT t1 = alpha*d_buf[j+1];
                d_तथ्य[j] = T(t0);
                d_तथ्य[j+1] = T(t1);
                t0 = alpha*d_buf[j+2];
                t1 = alpha*d_buf[j+3];
                d_तथ्य[j+2] = T(t0);
                d_तथ्य[j+3] = T(t1);
            }
            #पूर्ण_अगर
            क्रम( ; j < d_आकार.width; j++ )
                d_तथ्य[j] = T(alpha*d_buf[j]);
        }
    }
}


प्रकार व्योम (*GEMMSingleMulFunc)( स्थिर व्योम* src1, माप_प्रकार step1,
                   स्थिर व्योम* src2, माप_प्रकार step2, स्थिर व्योम* src3, माप_प्रकार step3,
                   व्योम* dst, माप_प्रकार dststep, आकार srcsize, आकार dstsize,
                   द्विगुन alpha, द्विगुन beta, पूर्णांक flags );

प्रकार व्योम (*GEMMBlockMulFunc)( स्थिर व्योम* src1, माप_प्रकार step1,
                   स्थिर व्योम* src2, माप_प्रकार step2, व्योम* dst, माप_प्रकार dststep,
                   आकार srcsize, आकार dstsize, पूर्णांक flags );

प्रकार व्योम (*GEMMStoreFunc)( स्थिर व्योम* src1, माप_प्रकार step1,
                   स्थिर व्योम* src2, माप_प्रकार step2, व्योम* dst, माप_प्रकार dststep,
                   आकार dstsize, द्विगुन alpha, द्विगुन beta, पूर्णांक flags );

ढाँचा<typename T, typename WT, व्योम (*fn)(स्थिर T*, माप_प्रकार, स्थिर T*, माप_प्रकार, स्थिर T*, माप_प्रकार, T*, माप_प्रकार, आकार, आकार, द्विगुन, द्विगुन, पूर्णांक)>
अटल व्योम gemmSingleMulWrap( स्थिर व्योम* src1, माप_प्रकार step1,
                   स्थिर व्योम* src2, माप_प्रकार step2, स्थिर व्योम* src3, माप_प्रकार step3,
                   व्योम* dst, माप_प्रकार dststep, आकार srcsize, आकार dstsize,
                   द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    fn((स्थिर T*)src1, step1, (स्थिर T*)src2, step2, (स्थिर T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

ढाँचा<typename T, typename WT, व्योम (*fn)(स्थिर T*, माप_प्रकार, स्थिर T*, माप_प्रकार, WT*, माप_प्रकार, आकार, आकार, पूर्णांक)>
अटल व्योम gemmBlockMulWrap( स्थिर व्योम* src1, माप_प्रकार step1,
                   स्थिर व्योम* src2, माप_प्रकार step2, व्योम* dst, माप_प्रकार dststep,
                   आकार srcsize, आकार dstsize, पूर्णांक flags )
{
    fn((स्थिर T*)src1, step1, (स्थिर T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

ढाँचा<typename T, typename WT, व्योम (*fn)(स्थिर T*, माप_प्रकार, स्थिर WT*, माप_प्रकार, T*, माप_प्रकार, आकार, द्विगुन, द्विगुन, पूर्णांक)>
अटल व्योम gemmStoreWrap( स्थिर व्योम* src1, माप_प्रकार step1,
                   स्थिर व्योम* src2, माप_प्रकार step2, व्योम* dst, माप_प्रकार dststep,
                   आकार dstsize, द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    fn((स्थिर T*)src1, step1, (स्थिर WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

अटल व्योम GEMMSingleMul_32f( स्थिर भग्न* a_तथ्य, माप_प्रकार a_step,
              स्थिर भग्न* b_तथ्य, माप_प्रकार b_step,
              स्थिर भग्न* c_तथ्य_2, माप_प्रकार c_step,
              भग्न* d_तथ्य, माप_प्रकार d_step,
              आकार a_आकार, आकार d_आकार,
              द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    GEMMSingleMul<भग्न,द्विगुन>(a_तथ्य, a_step, b_तथ्य, b_step, c_तथ्य_2,
                                c_step, d_तथ्य, d_step, a_आकार, d_आकार,
                                alpha, beta, flags);
}

अटल व्योम GEMMSingleMul_64f( स्थिर द्विगुन* a_तथ्य, माप_प्रकार a_step,
                              स्थिर द्विगुन* b_तथ्य, माप_प्रकार b_step,
                              स्थिर द्विगुन* c_तथ्य_2, माप_प्रकार c_step,
                              द्विगुन* d_तथ्य, माप_प्रकार d_step,
                              आकार a_आकार, आकार d_आकार,
                              द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    GEMMSingleMul<द्विगुन,द्विगुन>(a_तथ्य, a_step, b_तथ्य, b_step, c_तथ्य_2,
                                c_step, d_तथ्य, d_step, a_आकार, d_आकार,
                                alpha, beta, flags);
}


अटल व्योम GEMMSingleMul_32fc( स्थिर Complexf* a_तथ्य, माप_प्रकार a_step,
                              स्थिर Complexf* b_तथ्य, माप_प्रकार b_step,
                              स्थिर Complexf* c_तथ्य_2, माप_प्रकार c_step,
                              Complexf* d_तथ्य, माप_प्रकार d_step,
                              आकार a_आकार, आकार d_आकार,
                              द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_तथ्य, a_step, b_तथ्य, b_step, c_तथ्य_2,
                                c_step, d_तथ्य, d_step, a_आकार, d_आकार,
                                alpha, beta, flags);
}

अटल व्योम GEMMSingleMul_64fc( स्थिर Complexd* a_तथ्य, माप_प्रकार a_step,
                              स्थिर Complexd* b_तथ्य, माप_प्रकार b_step,
                              स्थिर Complexd* c_तथ्य_2, माप_प्रकार c_step,
                              Complexd* d_तथ्य, माप_प्रकार d_step,
                              आकार a_आकार, आकार d_आकार,
                              द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_तथ्य, a_step, b_तथ्य, b_step, c_तथ्य_2,
                                 c_step, d_तथ्य, d_step, a_आकार, d_आकार,
                                 alpha, beta, flags);
}

अटल व्योम GEMMBlockMul_32f( स्थिर भग्न* a_तथ्य, माप_प्रकार a_step,
             स्थिर भग्न* b_तथ्य, माप_प्रकार b_step,
             द्विगुन* d_तथ्य, माप_प्रकार d_step,
             आकार a_आकार, आकार d_आकार, पूर्णांक flags )
{
    GEMMBlockMul(a_तथ्य, a_step, b_तथ्य, b_step, d_तथ्य, d_step, a_आकार, d_आकार, flags);
}


अटल व्योम GEMMBlockMul_64f( स्थिर द्विगुन* a_तथ्य, माप_प्रकार a_step,
                             स्थिर द्विगुन* b_तथ्य, माप_प्रकार b_step,
                             द्विगुन* d_तथ्य, माप_प्रकार d_step,
                             आकार a_आकार, आकार d_आकार, पूर्णांक flags )
{
    GEMMBlockMul(a_तथ्य, a_step, b_तथ्य, b_step, d_तथ्य, d_step, a_आकार, d_आकार, flags);
}


अटल व्योम GEMMBlockMul_32fc( स्थिर Complexf* a_तथ्य, माप_प्रकार a_step,
                             स्थिर Complexf* b_तथ्य, माप_प्रकार b_step,
                             Complexd* d_तथ्य, माप_प्रकार d_step,
                             आकार a_आकार, आकार d_आकार, पूर्णांक flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_तथ्य, a_step, b_तथ्य, b_step, d_तथ्य, d_step, a_आकार, d_आकार, flags);
}


अटल व्योम GEMMBlockMul_64fc( स्थिर Complexd* a_तथ्य, माप_प्रकार a_step,
                             स्थिर Complexd* b_तथ्य, माप_प्रकार b_step,
                             Complexd* d_तथ्य, माप_प्रकार d_step,
                             आकार a_आकार, आकार d_आकार, पूर्णांक flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_तथ्य, a_step, b_तथ्य, b_step, d_तथ्य, d_step, a_आकार, d_आकार, flags);
}


अटल व्योम GEMMStore_32f( स्थिर भग्न* c_तथ्य_2, माप_प्रकार c_step,
          स्थिर द्विगुन* d_buf, माप_प्रकार d_buf_step,
          भग्न* d_तथ्य, माप_प्रकार d_step, आकार d_आकार,
          द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    GEMMStore(c_तथ्य_2, c_step, d_buf, d_buf_step, d_तथ्य, d_step, d_आकार, alpha, beta, flags);
}


अटल व्योम GEMMStore_64f( स्थिर द्विगुन* c_तथ्य_2, माप_प्रकार c_step,
                      स्थिर द्विगुन* d_buf, माप_प्रकार d_buf_step,
                      द्विगुन* d_तथ्य, माप_प्रकार d_step, आकार d_आकार,
                      द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    GEMMStore(c_तथ्य_2, c_step, d_buf, d_buf_step, d_तथ्य, d_step, d_आकार, alpha, beta, flags);
}


अटल व्योम GEMMStore_32fc( स्थिर Complexf* c_तथ्य_2, माप_प्रकार c_step,
                          स्थिर Complexd* d_buf, माप_प्रकार d_buf_step,
                          Complexf* d_तथ्य, माप_प्रकार d_step, आकार d_आकार,
                          द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    GEMMStore(c_तथ्य_2, c_step, d_buf, d_buf_step, d_तथ्य, d_step, d_आकार, alpha, beta, flags);
}


अटल व्योम GEMMStore_64fc( स्थिर Complexd* c_तथ्य_2, माप_प्रकार c_step,
                          स्थिर Complexd* d_buf, माप_प्रकार d_buf_step,
                          Complexd* d_तथ्य, माप_प्रकार d_step, आकार d_आकार,
                          द्विगुन alpha, द्विगुन beta, पूर्णांक flags )
{
    GEMMStore(c_तथ्य_2, c_step, d_buf, d_buf_step, d_तथ्य, d_step, d_आकार, alpha, beta, flags);
}

अटल व्योम gemmImpl( Mat A, Mat B, द्विगुन alpha,
           Mat C, द्विगुन beta, Mat D, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    स्थिर पूर्णांक block_lin_आकार = 128;
    स्थिर पूर्णांक block_आकार_2 = block_lin_आकार * block_lin_आकार;

    अटल द्विगुन zero[] = {0,0,0,0};
    अटल भग्न zerof[] = {0,0,0,0};

    आकार a_आकार = A.size(), d_आकार;
    पूर्णांक i, len = 0, type = A.type();

    चयन( flags & (GEMM_1_T|GEMM_2_T) )
    {
    हाल 0:
        d_आकार = आकार( B.cols, a_आकार.height );
        len = B.rows;
        अवरोध;
    हाल 1:
        d_आकार = आकार( B.cols, a_आकार.width );
        len = B.rows;
        अवरोध;
    हाल 2:
        d_आकार = आकार( B.rows, a_आकार.height );
        len = B.cols;
        अवरोध;
    हाल 3:
        d_आकार = आकार( B.rows, a_आकार.width );
        len = B.cols;
        अवरोध;
    }

    अगर( flags == 0 && 2 <= len && len <= 4 && (len == d_आकार.width || len == d_आकार.height) )
    {
        अगर( type == CV_32F )
        {
            भग्न* d = D.ptr<भग्न>();
            स्थिर भग्न *a = A.ptr<भग्न>(),
                        *b = B.ptr<भग्न>(),
                        *c = (स्थिर भग्न*)C.तथ्य;
            माप_प्रकार d_step = D.step/माप(d[0]),
                a_step = A.step/माप(a[0]),
                b_step = B.step/माप(b[0]),
                c_step = C.तथ्य ? C.step/माप(c[0]) : 0;

            अगर( !c )
                c = zerof;

            चयन( len )
            {
            हाल 2:
                अगर( len == d_आकार.width && b != d )
                {
                    क्रम( i = 0; i < d_आकार.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        भग्न t0 = a[0]*b[0] + a[1]*b[b_step];
                        भग्न t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (भग्न)(t0*alpha + c[0]*beta);
                        d[1] = (भग्न)(t1*alpha + c[1]*beta);
                    }
                }
                अन्यथा अगर( a != d )
                {
                    पूर्णांक c_step0 = 1;
                    अगर( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    क्रम( i = 0; i < d_आकार.width; i++, d++, b++, c += c_step0 )
                    {
                        भग्न t0 = a[0]*b[0] + a[1]*b[b_step];
                        भग्न t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (भग्न)(t0*alpha + c[0]*beta);
                        d[d_step] = (भग्न)(t1*alpha + c[c_step]*beta);
                    }
                }
                अन्यथा
                    अवरोध;
                वापस;
            हाल 3:
                अगर( len == d_आकार.width && b != d )
                {
                    क्रम( i = 0; i < d_आकार.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        भग्न t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        भग्न t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        भग्न t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (भग्न)(t0*alpha + c[0]*beta);
                        d[1] = (भग्न)(t1*alpha + c[1]*beta);
                        d[2] = (भग्न)(t2*alpha + c[2]*beta);
                    }
                }
                अन्यथा अगर( a != d )
                {
                    पूर्णांक c_step0 = 1;
                    अगर( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    क्रम( i = 0; i < d_आकार.width; i++, d++, b++, c += c_step0 )
                    {
                        भग्न t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        भग्न t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        भग्न t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (भग्न)(t0*alpha + c[0]*beta);
                        d[d_step] = (भग्न)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (भग्न)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                अन्यथा
                    अवरोध;
                वापस;
            हाल 4:
                अगर( len == d_आकार.width && b != d )
                {
                    क्रम( i = 0; i < d_आकार.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        भग्न t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        भग्न t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        भग्न t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        भग्न t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (भग्न)(t0*alpha + c[0]*beta);
                        d[1] = (भग्न)(t1*alpha + c[1]*beta);
                        d[2] = (भग्न)(t2*alpha + c[2]*beta);
                        d[3] = (भग्न)(t3*alpha + c[3]*beta);
                    }
                }
                अन्यथा अगर( len <= 16 && a != d )
                {
                    पूर्णांक c_step0 = 1;
                    अगर( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    क्रम( i = 0; i < d_आकार.width; i++, d++, b++, c += c_step0 )
                    {
                        भग्न t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        भग्न t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        भग्न t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        भग्न t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (भग्न)(t0*alpha + c[0]*beta);
                        d[d_step] = (भग्न)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (भग्न)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (भग्न)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                अन्यथा
                    अवरोध;
                वापस;
            }
        }

        अगर( type == CV_64F )
        {
            द्विगुन* d = D.ptr<द्विगुन>();
            स्थिर द्विगुन *a = A.ptr<द्विगुन>(),
                         *b = B.ptr<द्विगुन>(),
                         *c = (स्थिर द्विगुन*)C.तथ्य;
            माप_प्रकार d_step = D.step/माप(d[0]),
                a_step = A.step/माप(a[0]),
                b_step = B.step/माप(b[0]),
                c_step = C.तथ्य ? C.step/माप(c[0]) : 0;
            अगर( !c )
                c = zero;

            चयन( len )
            {
            हाल 2:
                अगर( len == d_आकार.width && b != d )
                {
                    क्रम( i = 0; i < d_आकार.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        द्विगुन t0 = a[0]*b[0] + a[1]*b[b_step];
                        द्विगुन t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                अन्यथा अगर( a != d )
                {
                    पूर्णांक c_step0 = 1;
                    अगर( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    क्रम( i = 0; i < d_आकार.width; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुन t0 = a[0]*b[0] + a[1]*b[b_step];
                        द्विगुन t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                अन्यथा
                    अवरोध;
                वापस;
            हाल 3:
                अगर( len == d_आकार.width && b != d )
                {
                    क्रम( i = 0; i < d_आकार.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        द्विगुन t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        द्विगुन t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        द्विगुन t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                अन्यथा अगर( a != d )
                {
                    पूर्णांक c_step0 = 1;
                    अगर( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    क्रम( i = 0; i < d_आकार.width; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुन t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        द्विगुन t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        द्विगुन t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                अन्यथा
                    अवरोध;
                वापस;
            हाल 4:
                अगर( len == d_आकार.width && b != d )
                {
                    क्रम( i = 0; i < d_आकार.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        द्विगुन t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        द्विगुन t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        द्विगुन t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        द्विगुन t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                अन्यथा अगर( d_आकार.width <= 16 && a != d )
                {
                    पूर्णांक c_step0 = 1;
                    अगर( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    क्रम( i = 0; i < d_आकार.width; i++, d++, b++, c += c_step0 )
                    {
                        द्विगुन t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        द्विगुन t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        द्विगुन t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        द्विगुन t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                अन्यथा
                    अवरोध;
                वापस;
            }
        }
    }

    {
    माप_प्रकार b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    स्थिर uchar* Cdata = C.तथ्य;
    माप_प्रकार Cstep = C.तथ्य ? (माप_प्रकार)C.step : 0;
    Auto_बफर<uchar> buf;

    अगर( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<भग्न, द्विगुन, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<भग्न, द्विगुन, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<भग्न, द्विगुन, GEMMStore_32f>;
    }
    अन्यथा अगर( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<द्विगुन, द्विगुन, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<द्विगुन, द्विगुन, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<द्विगुन, द्विगुन, GEMMStore_64f>;
    }
    अन्यथा अगर( type == CV_32FC2 )
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

    अगर( (d_आकार.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
    {
        b_step = d_आकार.width == 1 ? 0 : CV_ELEM_आकार(type);
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
    else*/ अगर( ((d_आकार.height <= block_lin_आकार/2 || d_आकार.width <= block_lin_आकार/2) &&
        len <= 10000) || len <= 10 ||
        (d_आकार.width <= block_lin_आकार &&
        d_आकार.height <= block_lin_आकार && len <= block_lin_आकार) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_आकार, d_आकार, alpha, beta, flags );
    }
    अन्यथा
    {
        पूर्णांक is_a_t = flags & GEMM_1_T;
        पूर्णांक is_b_t = flags & GEMM_2_T;
        पूर्णांक elem_आकार_2 = CV_ELEM_आकार(type);
        पूर्णांक dk0_1, dk0_2;
        माप_प्रकार a_buf_आकार = 0, b_buf_आकार, d_buf_आकार;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        पूर्णांक j, k, di = 0, dj = 0, dk = 0;
        पूर्णांक dm0, dn0, dk0;
        माप_प्रकार a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        पूर्णांक work_elem_आकार = elem_आकार_2 << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        अगर( !is_a_t )
            a_step0 = A.step, a_step1 = elem_आकार_2;
        अन्यथा
            a_step0 = elem_आकार_2, a_step1 = A.step;

        अगर( !is_b_t )
            b_step0 = b_step, b_step1 = elem_आकार_2;
        अन्यथा
            b_step0 = elem_आकार_2, b_step1 = b_step;

        अगर( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        अन्यथा अगर( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_आकार_2;
        अन्यथा
            c_step0 = elem_आकार_2, c_step1 = C.step;

        dm0 = std::min( block_lin_आकार, d_आकार.height );
        dn0 = std::min( block_lin_आकार, d_आकार.width );
        dk0_1 = block_आकार_2 / dm0;
        dk0_2 = block_आकार_2 / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        अगर( dk0*dm0 > block_आकार_2 )
            dm0 = block_आकार_2 / dk0;
        अगर( dk0*dn0 > block_आकार_2 )
            dn0 = block_आकार_2 / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_आकार = (माप_प्रकार)(dk0+dk0/8+1)*dk0_1*elem_आकार_2;
        d_buf_आकार = (माप_प्रकार)(dk0+dk0/8+1)*dk0_1*work_elem_आकार;

        अगर( is_a_t )
        {
            a_buf_आकार = (माप_प्रकार)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_आकार_2;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_आकार + b_buf_आकार + a_buf_आकार);
        d_buf = buf.तथ्य();
        b_buf = d_buf + d_buf_आकार;

        अगर( is_a_t )
            a_buf = b_buf + b_buf_आकार;

        क्रम( i = 0; i < d_आकार.height; i += di )
        {
            di = dm0;
            अगर( i + di >= d_आकार.height || 8*(i + di) + di > 8*d_आकार.height )
                di = d_आकार.height - i;

            क्रम( j = 0; j < d_आकार.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_आकार_2;
                स्थिर uchar* _c = Cdata + i*c_step0 + j*c_step1;
                माप_प्रकार _d_step = matD->step;
                dj = dn0;

                अगर( j + dj >= d_आकार.width || 8*(j + dj) + dj > 8*d_आकार.width )
                    dj = d_आकार.width - j;

                flags &= 15;
                अगर( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_आकार;
                }

                क्रम( k = 0; k < len; k += dk )
                {
                    स्थिर uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    माप_प्रकार _a_step = A.step;
                    स्थिर uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    माप_प्रकार _b_step = b_step;
                    आकार a_bl_आकार;

                    dk = dk0;
                    अगर( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    अगर( !is_a_t )
                        a_bl_आकार.width = dk, a_bl_आकार.height = di;
                    अन्यथा
                        a_bl_आकार.width = di, a_bl_आकार.height = dk;

                    अगर( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_आकार_2;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_आकार, elem_आकार_2 );
                        std::swap( a_bl_आकार.width, a_bl_आकार.height );
                        _a = a_buf;
                    }

                    अगर( dj < d_आकार.width )
                    {
                        आकार b_आकार;
                        अगर( !is_b_t )
                            b_आकार.width = dj, b_आकार.height = dk;
                        अन्यथा
                            b_आकार.width = dk, b_आकार.height = dj;

                        _b_step = b_आकार.width*elem_आकार_2;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_आकार, elem_आकार_2 );
                        _b = b_buf;
                    }

                    अगर( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_आकार, आकार(dj,di), flags );
                    अन्यथा
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_आकार, आकार(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                अगर( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_आकार_2,
                               matD->step, आकार(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

ढाँचा <typename fptype>अंतरभूत अटल व्योम
callGemmImpl(स्थिर fptype *src1, माप_प्रकार src1_step, स्थिर fptype *src2, माप_प्रकार src2_step, fptype alpha,
          स्थिर fptype *src3, माप_प्रकार src3_step, fptype beta, fptype *dst, माप_प्रकार dst_step, पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags, पूर्णांक type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    पूर्णांक b_m, b_n, c_m, c_n, m_d;

    अगर(flags & GEMM_2_T)
    {
        b_m = n_d;
        अगर(flags & GEMM_1_T )
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
        अगर(flags & GEMM_1_T )
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

    अगर(flags & GEMM_3_T)
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
    अगर(src1 != शून्य)
        A = Mat(m_a, n_a, type, (व्योम*)src1, src1_step);
    अगर(src2 != शून्य)
        B = Mat(b_m, b_n, type, (व्योम*)src2, src2_step);
    अगर(src3 != शून्य && beta != 0.0)
        C = Mat(c_m, c_n, type, (व्योम*)src3, src3_step);
    Mat D(m_d, n_d, type, (व्योम*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

व्योम gemm32f(स्थिर भग्न* src1, माप_प्रकार src1_step, स्थिर भग्न* src2, माप_प्रकार src2_step,
             भग्न alpha, स्थिर भग्न* src3, माप_प्रकार src3_step, भग्न beta, भग्न* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

व्योम gemm64f(स्थिर द्विगुन* src1, माप_प्रकार src1_step, स्थिर द्विगुन* src2, माप_प्रकार src2_step,
             द्विगुन alpha, स्थिर द्विगुन* src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

व्योम gemm32fc(स्थिर भग्न* src1, माप_प्रकार src1_step, स्थिर भग्न* src2, माप_प्रकार src2_step,
              भग्न alpha, स्थिर भग्न* src3, माप_प्रकार src3_step, भग्न beta, भग्न* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

व्योम gemm64fc(स्थिर द्विगुन* src1, माप_प्रकार src1_step, स्थिर द्विगुन* src2, माप_प्रकार src2_step,
              द्विगुन alpha, स्थिर द्विगुन* src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#पूर्ण_अगर // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ढाँचा<typename T, typename WT> अटल व्योम
transform_( स्थिर T* src, T* dst, स्थिर WT* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
    पूर्णांक x;

    अगर( scn == 2 && dcn == 2 )
    {
        क्रम( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    अन्यथा अगर( scn == 3 && dcn == 3 )
    {
        क्रम( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    अन्यथा अगर( scn == 3 && dcn == 1 )
    {
        क्रम( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    अन्यथा अगर( scn == 4 && dcn == 4 )
    {
        क्रम( x = 0; x < len*4; x += 4 )
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
        क्रम( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            स्थिर WT* _m = m;
            पूर्णांक j, k;
            क्रम( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                क्रम( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

अटल व्योम
transform_8u( स्थिर uchar* src, uchar* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक BITS = 10, SCALE = 1 << BITS;
    स्थिर भग्न MAX_M = (भग्न)(1 << (15 - BITS));

    अगर( scn == 3 && dcn == 3 &&
        std::असल(m[0]) < MAX_M && std::असल(m[1]) < MAX_M && std::असल(m[ 2]) < MAX_M*256 && std::असल(m[ 3]) < MAX_M*256 &&
        std::असल(m[4]) < MAX_M && std::असल(m[5]) < MAX_M && std::असल(m[ 6]) < MAX_M*256 && std::असल(m[ 7]) < MAX_M*256 &&
        std::असल(m[8]) < MAX_M && std::असल(m[9]) < MAX_M && std::असल(m[10]) < MAX_M*256 && std::असल(m[11]) < MAX_M*256 )
    {
        स्थिर पूर्णांक nChannels = 3;

        जोड़ {
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
        क्रम (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        क्रम( ; x < len * nChannels; x += nChannels )
        {
            पूर्णांक v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    transform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
transform_16u( स्थिर ushort* src, ushort* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर( scn == 3 && dcn == 3 )
    {
        पूर्णांक x = 0;
#अगर CV_SIMD_WIDTH > 16
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
        क्रम (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#पूर्ण_अगर
#अगर CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_बायाँ<1>(_m0l);
        v_float32x4 _m1h = v_rotate_बायाँ<1>(_m1l);
        v_float32x4 _m2h = v_rotate_बायाँ<1>(_m2l);
        v_float32x4 _m3h = v_rotate_बायाँ<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        क्रम( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_दायाँ<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#पूर्ण_अगर //CV_SIMD128
        क्रम( ; x < len * 3; x += 3 )
        {
            भग्न v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    transform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
transform_32f( स्थिर भग्न* src, भग्न* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#अगर (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    पूर्णांक x = 0;
    अगर( scn == 3 && dcn == 3 )
    {
        पूर्णांक idx[VTraits<v_float32>::max_nlanes/2];
        क्रम( पूर्णांक i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        भग्न _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        क्रम( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        क्रम( ; x < len*3; x += 3 )
        {
            भग्न v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            भग्न t0 = saturate_cast<भग्न>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            भग्न t1 = saturate_cast<भग्न>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            भग्न t2 = saturate_cast<भग्न>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        वापस;
    }

    अगर( scn == 4 && dcn == 4 )
    {
#अगर CV_SIMD_WIDTH > 16
        पूर्णांक idx[VTraits<v_float32>::max_nlanes/4];
        क्रम( पूर्णांक i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        भग्न _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        क्रम( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#पूर्ण_अगर
#अगर CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        क्रम( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#अन्यथा // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        क्रम( ; x < len*4; x += 4 )
        {
            भग्न v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            भग्न t0 = saturate_cast<भग्न>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            भग्न t1 = saturate_cast<भग्न>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            भग्न t2 = saturate_cast<भग्न>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            भग्न t3 = saturate_cast<भग्न>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#पूर्ण_अगर
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    transform_(src, dst, m, len, scn, dcn);
}


अटल व्योम
transform_8s(स्थिर schar* src, schar* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक BITS = 10, SCALE = 1 << BITS;
    स्थिर भग्न MAX_M = (भग्न)(1 << (15 - BITS));

    अगर( scn == 3 && dcn == 3 &&
        std::असल(m[0]) < MAX_M && std::असल(m[1]) < MAX_M && std::असल(m[ 2]) < MAX_M*256 && std::असल(m[ 3]) < MAX_M*256 &&
        std::असल(m[4]) < MAX_M && std::असल(m[5]) < MAX_M && std::असल(m[ 6]) < MAX_M*256 && std::असल(m[ 7]) < MAX_M*256 &&
        std::असल(m[8]) < MAX_M && std::असल(m[9]) < MAX_M && std::असल(m[10]) < MAX_M*256 && std::असल(m[11]) < MAX_M*256 )
    {
        स्थिर पूर्णांक nChannels = 3;

        जोड़ {
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
        क्रम (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        क्रम( ; x < len * nChannels; x += nChannels )
        {
            पूर्णांक v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    transform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
transform_16s(स्थिर लघु* src, लघु* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर (scn == 3 && dcn == 3) {
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
        क्रम (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        क्रम( ; x < len * 3; x += 3 )
        {
            भग्न v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            लघु t0 = saturate_cast<लघु>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            लघु t1 = saturate_cast<लघु>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            लघु t2 = saturate_cast<लघु>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    transform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
transform_32s(स्थिर पूर्णांक* src, पूर्णांक* dst, स्थिर द्विगुन* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    अगर( scn == 3 && dcn == 3) {
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
        क्रम (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ऊपरी(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ऊपरी(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ऊपरी(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        क्रम (; x < len * 3; x += 3) {
            द्विगुन b = src[x], g = src[x + 1], r = src[x + 2];
            पूर्णांक db = saturate_cast<पूर्णांक>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            पूर्णांक dg = saturate_cast<पूर्णांक>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            पूर्णांक dr = saturate_cast<पूर्णांक>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    transform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
transform_64f(स्थिर द्विगुन* src, द्विगुन* dst, स्थिर द्विगुन* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    अगर( scn == 3 && dcn == 3) {
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
        क्रम (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        क्रम (; x < len * 3; x += 3) {
            द्विगुन b = src[x], g = src[x + 1], r = src[x + 2];
            द्विगुन db = saturate_cast<द्विगुन>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            द्विगुन dg = saturate_cast<द्विगुन>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            द्विगुन dr = saturate_cast<द्विगुन>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    transform_(src, dst, m, len, scn, dcn);
}

ढाँचा<typename T, typename WT> अटल व्योम
diagtransform_( स्थिर T* src, T* dst, स्थिर WT* m, पूर्णांक len, पूर्णांक cn, पूर्णांक )
{
    पूर्णांक x;

    अगर( cn == 2 )
    {
        क्रम( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    अन्यथा अगर( cn == 3 )
    {
        क्रम( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    अन्यथा अगर( cn == 4 )
    {
        क्रम( x = 0; x < len*4; x += 4 )
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
        क्रम( x = 0; x < len; x++, src += cn, dst += cn )
        {
            स्थिर WT* _m = m;
            क्रम( पूर्णांक j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

अटल व्योम
diagtransform_8u(स्थिर uchar* src, uchar* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        क्रम (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        क्रम (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    diagtransform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
diagtransform_8s(स्थिर schar* src, schar* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
    #अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        क्रम (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        क्रम (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    diagtransform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
diagtransform_16u(स्थिर ushort* src, ushort* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        क्रम (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        क्रम (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    diagtransform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
diagtransform_16s(स्थिर लघु* src, लघु* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        क्रम (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        क्रम (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            लघु db = saturate_cast<लघु>(m[ 0] * b + m[ 3]);
            लघु dg = saturate_cast<लघु>(m[ 5] * g + m[ 7]);
            लघु dr = saturate_cast<लघु>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    diagtransform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
diagtransform_32s(स्थिर पूर्णांक* src, पूर्णांक* dst, स्थिर द्विगुन* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    अगर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        क्रम (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ऊपरी(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ऊपरी(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ऊपरी(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        क्रम (; x < len * 3; x += 3) {
            पूर्णांक b = src[x], g = src[x + 1], r = src[x + 2];
            पूर्णांक db = saturate_cast<पूर्णांक>(m[ 0] * b + m[ 3]);
            पूर्णांक dg = saturate_cast<पूर्णांक>(m[ 5] * g + m[ 7]);
            पूर्णांक dr = saturate_cast<पूर्णांक>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    diagtransform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
diagtransform_32f(स्थिर भग्न* src, भग्न* dst, स्थिर भग्न* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    अगर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        क्रम (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        क्रम (; x < len * 3; x += 3) {
            भग्न b = src[x], g = src[x + 1], r = src[x + 2];
            भग्न db = saturate_cast<भग्न>(m[ 0] * b + m[ 3]);
            भग्न dg = saturate_cast<भग्न>(m[ 5] * g + m[ 7]);
            भग्न dr = saturate_cast<भग्न>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    diagtransform_(src, dst, m, len, scn, dcn);
}

अटल व्योम
diagtransform_64f(स्थिर द्विगुन* src, द्विगुन* dst, स्थिर द्विगुन* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    अगर (scn == 3 && dcn == 3) {
        पूर्णांक x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        क्रम (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        क्रम (; x < len * 3; x += 3) {
            द्विगुन b = src[x], g = src[x + 1], r = src[x + 2];
            द्विगुन db = saturate_cast<द्विगुन>(m[ 0] * b + m[ 3]);
            द्विगुन dg = saturate_cast<द्विगुन>(m[ 5] * g + m[ 7]);
            द्विगुन dr = saturate_cast<द्विगुन>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        वापस;
    }
#पूर्ण_अगर

    diagtransform_(src, dst, m, len, scn, dcn);
}


ढाँचा<typename T, typename WT, व्योम (*fn)(स्थिर T*, T*, स्थिर WT*, पूर्णांक, पूर्णांक, पूर्णांक)>
अटल व्योम transformWrap(स्थिर व्योम* src, व्योम* dst, स्थिर व्योम* m, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn)
{
    fn((स्थिर T*)src, (T*)dst, (स्थिर WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(पूर्णांक depth)
{
    अटल TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, भग्न, transform_8u>, transformWrap<schar, भग्न, transform_8s>, transformWrap<ushort, भग्न, transform_16u>,
        transformWrap<लघु, भग्न, transform_16s>, transformWrap<पूर्णांक, द्विगुन, transform_32s>, transformWrap<भग्न, भग्न, transform_32f>,
        transformWrap<द्विगुन, द्विगुन, transform_64f>, 0
    };

    वापस transformTab[depth];
}

TransformFunc getDiagTransformFunc(पूर्णांक depth)
{
    अटल TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, भग्न, diagtransform_8u>, transformWrap<schar, भग्न, diagtransform_8s>, transformWrap<ushort, भग्न, diagtransform_16u>,
        transformWrap<लघु, भग्न, diagtransform_16s>, transformWrap<पूर्णांक, द्विगुन, diagtransform_32s>, transformWrap<भग्न, भग्न, diagtransform_32f>,
        transformWrap<द्विगुन, द्विगुन, diagtransform_64f>, 0
    };

    वापस diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ढाँचा<typename T> अटल व्योम
perspectiveTransform_( स्थिर व्योम* _src, व्योम* _dst, स्थिर व्योम* _m_ptr, पूर्णांक len, पूर्णांक scn, पूर्णांक dcn )
{
    स्थिर T* src = (स्थिर T*)_src;
    T* dst = (T*)_dst;
    स्थिर द्विगुन* m = (स्थिर द्विगुन*)_m_ptr;
    स्थिर द्विगुन eps = भग्न_अंतर;
    पूर्णांक i;

    अगर( scn == 2 && dcn == 2 )
    {
        क्रम( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            द्विगुन w = x*m[6] + y*m[7] + m[8];

            अगर( भ_असल(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            अन्यथा
                dst[i] = dst[i+1] = (T)0;
        }
    }
    अन्यथा अगर( scn == 3 && dcn == 3 )
    {
        क्रम( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            द्विगुन w = x*m[12] + y*m[13] + z*m[14] + m[15];

            अगर( भ_असल(w) > eps )
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
    अन्यथा अगर( scn == 3 && dcn == 2 )
    {
        क्रम( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            द्विगुन w = x*m[8] + y*m[9] + z*m[10] + m[11];

            अगर( भ_असल(w) > eps )
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
        क्रम( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            स्थिर द्विगुन* _m = m + dcn*(scn + 1);
            द्विगुन w = _m[scn];
            पूर्णांक j, k;
            क्रम( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            अगर( भ_असल(w) > eps )
            {
                _m = m;
                क्रम( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    द्विगुन s = _m[scn];
                    क्रम( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            अन्यथा
                क्रम( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(पूर्णांक depth)
{
    अगर (depth == CV_32F)
        वापस perspectiveTransform_<भग्न>;
    अगर (depth == CV_64F)
        वापस perspectiveTransform_<द्विगुन>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

अटल व्योम scaleAdd_32f(स्थिर व्योम* _src1, स्थिर व्योम* _src2, व्योम* _dst,
                         पूर्णांक len, स्थिर व्योम* _alpha)
{
    स्थिर भग्न* src1 = (स्थिर भग्न*)_src1;
    स्थिर भग्न* src2 = (स्थिर भग्न*)_src2;
    भग्न* dst = (भग्न*)_dst;
    भग्न alpha = *(स्थिर भग्न*)_alpha;
    पूर्णांक i = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    स्थिर पूर्णांक cWidth = VTraits<v_float32>::vlanes();
    क्रम (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#पूर्ण_अगर
    क्रम (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


अटल व्योम scaleAdd_64f(स्थिर व्योम* _src1, स्थिर व्योम* _src2, व्योम* _dst,
                         पूर्णांक len, स्थिर व्योम* _alpha)
{
    स्थिर द्विगुन* src1 = (स्थिर द्विगुन*)_src1;
    स्थिर द्विगुन* src2 = (स्थिर द्विगुन*)_src2;
    द्विगुन* dst = (द्विगुन*)_dst;
    द्विगुन alpha = *(स्थिर द्विगुन*)_alpha;
    पूर्णांक i = 0;
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    स्थिर पूर्णांक cWidth = VTraits<v_float64>::vlanes();
    क्रम (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#पूर्ण_अगर
    क्रम (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(पूर्णांक depth)
{
    अगर (depth == CV_32F)
        वापस scaleAdd_32f;
    अगर (depth == CV_64F)
        वापस scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ढाँचा<typename T> अटल अंतरभूत
द्विगुन MahalanobisImpl(स्थिर Mat& v1, स्थिर Mat& v2, स्थिर Mat& icovar, द्विगुन *diff_बफर /*[len]*/, पूर्णांक len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    आकार sz = v1.size();
    द्विगुन परिणाम = 0;

    sz.width *= v1.channels();
    अगर (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        स्थिर T* src1 = v1.ptr<T>();
        स्थिर T* src2 = v2.ptr<T>();
        माप_प्रकार step1 = v1.step/माप(src1[0]);
        माप_प्रकार step2 = v2.step/माप(src2[0]);
        द्विगुन* diff = diff_बफर;
        स्थिर T* mat = icovar.ptr<T>();
        माप_प्रकार matstep = icovar.step/माप(mat[0]);

        क्रम (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            क्रम (पूर्णांक i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_बफर;
        क्रम (पूर्णांक i = 0; i < len; i++, mat += matstep)
        {
            द्विगुन row_योग = 0;
            पूर्णांक j = 0;
#अगर CV_ENABLE_UNROLLED
            क्रम(; j <= len - 4; j += 4 )
                row_योग += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#पूर्ण_अगर
            क्रम (; j < len; j++)
                row_योग += diff[j]*mat[j];
            परिणाम += row_योग * diff[i];
        }
    }
    वापस परिणाम;
}

MahalanobisImplFunc getMahalanobisImplFunc(पूर्णांक depth)
{
    अगर (depth == CV_32F)
        वापस (MahalanobisImplFunc)MahalanobisImpl<भग्न>;
    अगर (depth == CV_64F)
        वापस (MahalanobisImplFunc)MahalanobisImpl<द्विगुन>;
    CV_Assert(0 && "Not supported");
}


#अगर !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ढाँचा<typename sT, typename dT> अटल व्योम
MulTransposedR(स्थिर Mat& srcmat, स्थिर Mat& dstmat, स्थिर Mat& deltamat, द्विगुन scale)
{
    पूर्णांक i, j, k;
    स्थिर sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    स्थिर dT* delta = deltamat.ptr<dT>();
    माप_प्रकार srcstep = srcmat.step/माप(src[0]);
    माप_प्रकार dststep = dstmat.step/माप(dst[0]);
    माप_प्रकार deltastep = deltamat.rows > 1 ? deltamat.step/माप(delta[0]) : 0;
    पूर्णांक delta_cols = deltamat.cols;
    आकार size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    पूर्णांक buf_आकार_2 = size.height*माप(dT);
    Auto_बफर<uchar> buf;

    अगर( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_आकार_2 *= 5;
    }
    buf.allocate(buf_आकार_2);
    col_buf = (dT*)buf.तथ्य();

    अगर( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        क्रम( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#अगर CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#पूर्ण_अगर

    अगर( !delta )
        क्रम( i = 0; i < size.width; i++, tdst += dststep )
        {
            क्रम( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            क्रम( j = i; j <= size.width - 4; j += 4 )
            {
#अगर CV_SIMD128_64F
                अगर (तथ्य_Type_2<sT>::depth == CV_64F && तथ्य_Type_2<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    स्थिर द्विगुन *tsrc = (द्विगुन*)(src + j);

                    क्रम( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((द्विगुन)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((द्विगुन*)(tdst+j), v_mul(s0, v_scale));
                    v_store((द्विगुन*)(tdst+j+2), v_mul(s1, v_scale));
                } अन्यथा
#पूर्ण_अगर
                {
                    द्विगुन s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    स्थिर sT *tsrc = src + j;

                    क्रम( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        द्विगुन a = col_buf[k];
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

            क्रम( ; j < size.width; j++ )
            {
                द्विगुन s0 = 0;
                स्थिर sT *tsrc = src + j;

                क्रम( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (द्विगुन)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    अन्यथा
        क्रम( i = 0; i < size.width; i++, tdst += dststep )
        {
            अगर( !delta_buf )
                क्रम( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            अन्यथा
                क्रम( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            क्रम( j = i; j <= size.width - 4; j += 4 )
            {
#अगर CV_SIMD128_64F
                अगर (तथ्य_Type_2<sT>::depth == CV_64F && तथ्य_Type_2<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    स्थिर द्विगुन *tsrc = (द्विगुन*)(src + j);
                    स्थिर द्विगुन *d = (द्विगुन*)(delta_buf ? delta_buf : delta + j);

                    क्रम( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((द्विगुन)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((द्विगुन*)(tdst+j), v_mul(s0, v_scale));
                    v_store((द्विगुन*)(tdst+j+2), v_mul(s1, v_scale));
                }
                अन्यथा
#पूर्ण_अगर

                {
                    द्विगुन s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    स्थिर sT *tsrc = src + j;
                    स्थिर dT *d = delta_buf ? delta_buf : delta + j;

                    क्रम( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        द्विगुन a = col_buf[k];
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

            क्रम( ; j < size.width; j++ )
            {
                द्विगुन s0 = 0;
                स्थिर sT *tsrc = src + j;
                स्थिर dT *d = delta_buf ? delta_buf : delta + j;

                क्रम( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (द्विगुन)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


ढाँचा<typename sT, typename dT> अटल व्योम
MulTransposedL(स्थिर Mat& srcmat, स्थिर Mat& dstmat, स्थिर Mat& deltamat, द्विगुन scale)
{
    पूर्णांक i, j, k;
    स्थिर sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    स्थिर dT* delta = deltamat.ptr<dT>();
    माप_प्रकार srcstep = srcmat.step/माप(src[0]);
    माप_प्रकार dststep = dstmat.step/माप(dst[0]);
    माप_प्रकार deltastep = deltamat.rows > 1 ? deltamat.step/माप(delta[0]) : 0;
    पूर्णांक delta_cols = deltamat.cols;
    आकार size = srcmat.size();
    dT* tdst = dst;

    अगर( !delta )
        क्रम( i = 0; i < size.height; i++, tdst += dststep )
            क्रम( j = i; j < size.height; j++ )
            {
                द्विगुन s = 0;
                स्थिर sT *tsrc1 = src + i*srcstep;
                स्थिर sT *tsrc2 = src + j*srcstep;
#अगर CV_SIMD128_64F
                अगर (तथ्य_Type_2<sT>::depth == CV_64F && तथ्य_Type_2<dT>::depth == CV_64F)
                {
                    स्थिर द्विगुन *v_tsrc1 = (द्विगुन *)(tsrc1);
                    स्थिर द्विगुन *v_tsrc2 = (द्विगुन *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    क्रम( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_योग(v_s);
                }
                अन्यथा
#पूर्ण_अगर
                {
                    क्रम( k = 0; k <= size.width - 4; k += 4 )
                        s += (द्विगुन)tsrc1[k]*tsrc2[k] + (द्विगुन)tsrc1[k+1]*tsrc2[k+1] +
                             (द्विगुन)tsrc1[k+2]*tsrc2[k+2] + (द्विगुन)tsrc1[k+3]*tsrc2[k+3];
                }
                क्रम( ; k < size.width; k++ )
                    s += (द्विगुन)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    अन्यथा
    {
        dT delta_buf[4];
        पूर्णांक delta_shift = delta_cols == size.width ? 4 : 0;
        Auto_बफर<uchar> buf(size.width*माप(dT));
        dT* row_buf = (dT*)buf.तथ्य();

        क्रम( i = 0; i < size.height; i++, tdst += dststep )
        {
            स्थिर sT *tsrc1 = src + i*srcstep;
            स्थिर dT *tdelta1 = delta + i*deltastep;

            अगर( delta_cols < size.width )
                क्रम( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            अन्यथा
                क्रम( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            क्रम( j = i; j < size.height; j++ )
            {
                द्विगुन s = 0;
                स्थिर sT *tsrc2 = src + j*srcstep;
                स्थिर dT *tdelta2 = delta + j*deltastep;
                अगर( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#अगर CV_SIMD128_64F
                अगर (तथ्य_Type_2<sT>::depth == CV_64F && तथ्य_Type_2<dT>::depth == CV_64F)
                {
                    स्थिर द्विगुन *v_tsrc2 = (द्विगुन *)(tsrc2);
                    स्थिर द्विगुन *v_tdelta2 = (द्विगुन *)(tdelta2);
                    स्थिर द्विगुन *v_row_buf = (द्विगुन *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    क्रम( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_योग(v_s);

                    tdelta2 = (स्थिर dT *)(v_tdelta2);
                }
                अन्यथा
#पूर्ण_अगर
                {
                    क्रम( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (द्विगुन)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (द्विगुन)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (द्विगुन)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (द्विगुन)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                क्रम( ; k < size.width; k++, tdelta2++ )
                    s += (द्विगुन)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(पूर्णांक stype, पूर्णांक dtype, bool ata)
{
    MulTransposedFunc func = शून्य;
    अगर (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,भग्न>
                   : MulTransposedL<uchar,भग्न>;
    }
    अन्यथा अगर (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,द्विगुन>
                   : MulTransposedL<uchar,द्विगुन>;
    }
    अन्यथा अगर(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,भग्न>
                   : MulTransposedL<ushort,भग्न>;
    }
    अन्यथा अगर(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,द्विगुन>
                   : MulTransposedL<ushort,द्विगुन>;
    }
    अन्यथा अगर(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<लघु,भग्न>
                   : MulTransposedL<लघु,भग्न>;
    }
    अन्यथा अगर(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<लघु,द्विगुन>
                   : MulTransposedL<लघु,द्विगुन>;
    }
    अन्यथा अगर(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<भग्न,भग्न>
                   : MulTransposedL<भग्न,भग्न>;
    }
    अन्यथा अगर(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<भग्न,द्विगुन>
                   : MulTransposedL<भग्न,द्विगुन>;
    }
    अन्यथा अगर(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<द्विगुन,द्विगुन>
                   : MulTransposedL<द्विगुन,द्विगुन>;
    }
    CV_Assert(func && "Not supported");
    वापस func;
}
#पूर्ण_अगर // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ढाँचा<typename T> अटल अंतरभूत
द्विगुन dotProd_(स्थिर T* src1, स्थिर T* src2, पूर्णांक len)
{
    पूर्णांक i = 0;
    द्विगुन परिणाम = 0;

#अगर CV_ENABLE_UNROLLED
    क्रम( ; i <= len - 4; i += 4 )
        परिणाम += (द्विगुन)src1[i]*src2[i] + (द्विगुन)src1[i+1]*src2[i+1] +
            (द्विगुन)src1[i+2]*src2[i+2] + (द्विगुन)src1[i+3]*src2[i+3];
#पूर्ण_अगर
    क्रम( ; i < len; i++ )
        परिणाम += (द्विगुन)src1[i]*src2[i];

    वापस परिणाम;
}


द्विगुन dotProd_8u(स्थिर uchar* src1, स्थिर uchar* src2, पूर्णांक len)
{
    द्विगुन r = 0;
    पूर्णांक i = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), block_आकार;

    जबतक (i < len0)
    {
        block_आकार = std::min(len0 - i, blockSize0);
        v_uint32 v_योग = vx_setzero_u32();
        स्थिर पूर्णांक cWidth = VTraits<v_uint16>::vlanes();

        पूर्णांक j = 0;
        क्रम (; j <= block_आकार - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_योग = v_dotprod_expand_fast(v_src1, v_src2, v_योग);
        }

        क्रम (; j <= block_आकार - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_योग = v_add(v_योग, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (द्विगुन)v_reduce_योग(v_योग);

        src1 += block_आकार;
        src2 += block_आकार;
        i += block_आकार;
    }
    vx_cleanup();
#पूर्ण_अगर
    वापस r + dotProd_(src1, src2, len - i);
}


द्विगुन dotProd_8s(स्थिर schar* src1, स्थिर schar* src2, पूर्णांक len)
{
    द्विगुन r = 0.0;
    पूर्णांक i = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), block_आकार;

    जबतक (i < len0)
    {
        block_आकार = std::min(len0 - i, blockSize0);
        v_int32 v_योग = vx_setzero_s32();
        स्थिर पूर्णांक cWidth = VTraits<v_int16>::vlanes();

        पूर्णांक j = 0;
        क्रम (; j <= block_आकार - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_योग = v_dotprod_expand_fast(v_src1, v_src2, v_योग);
        }

        क्रम (; j <= block_आकार - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_योग = v_dotprod_fast(v_src1, v_src2, v_योग);
        }
        r += (द्विगुन)v_reduce_योग(v_योग);

        src1 += block_आकार;
        src2 += block_आकार;
        i += block_आकार;
    }
    vx_cleanup();
#पूर्ण_अगर

    वापस r + dotProd_(src1, src2, len - i);
}

द्विगुन dotProd_16u(स्थिर ushort* src1, स्थिर ushort* src2, पूर्णांक len)
{
    द्विगुन r = 0.0;
    पूर्णांक i = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), block_आकार;

    जबतक (i < len0)
    {
        block_आकार = std::min(len0 - i, blockSize0);
        v_uint64 v_योग = vx_setzero_u64();
        स्थिर पूर्णांक cWidth = VTraits<v_uint16>::vlanes();

        पूर्णांक j = 0;
        क्रम (; j <= block_आकार - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_योग = v_dotprod_expand_fast(v_src1, v_src2, v_योग);
        }
        r += (द्विगुन)v_reduce_योग(v_योग);

        src1 += block_आकार;
        src2 += block_आकार;
        i += block_आकार;
    }
    vx_cleanup();
#पूर्ण_अगर
    वापस r + dotProd_(src1, src2, len - i);
}

द्विगुन dotProd_16s(स्थिर लघु* src1, स्थिर लघु* src2, पूर्णांक len)
{
    द्विगुन r = 0.0;
    पूर्णांक i = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), block_आकार;

    जबतक (i < len0)
    {
        block_आकार = std::min(len0 - i, blockSize0);
        v_int64 v_योग = vx_setzero_s64();
        स्थिर पूर्णांक cWidth = VTraits<v_int16>::vlanes();

        पूर्णांक j = 0;
        क्रम (; j <= block_आकार - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_योग = v_dotprod_expand_fast(v_src1, v_src2, v_योग);
        }
        r += (द्विगुन)v_reduce_योग(v_योग);

        src1 += block_आकार;
        src2 += block_आकार;
        i += block_आकार;
    }
    vx_cleanup();
#पूर्ण_अगर
    वापस r + dotProd_(src1, src2, len - i);
}

द्विगुन dotProd_32s(स्थिर पूर्णांक* src1, स्थिर पूर्णांक* src2, पूर्णांक len)
{
#अगर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    द्विगुन r = .0;
    पूर्णांक i = 0;
    स्थिर पूर्णांक step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#अगर CV_SIMD_WIDTH == 16
    स्थिर पूर्णांक wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    क्रम (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
    {
        v_int32 v_src10 = vx_load(src1);
        v_int32 v_src20 = vx_load(src2);
        v_int32 v_src11 = vx_load(src1 + step);
        v_int32 v_src21 = vx_load(src2 + step);
        v_sum0 = v_dotprod_expand_fast(v_src10, v_src20, v_sum0);
        v_sum1 = v_dotprod_expand_fast(v_src11, v_src21, v_sum1);
    }
    v_sum0 = v_add(v_sum0, v_sum1);
#पूर्ण_अगर
    क्रम (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_योग(v_sum0);
    vx_cleanup();
    वापस r + dotProd_(src1, src2, len - i);
#अन्यथा
    वापस dotProd_(src1, src2, len);
#पूर्ण_अगर
}

द्विगुन dotProd_32f(स्थिर भग्न* src1, स्थिर भग्न* src2, पूर्णांक len)
{
    द्विगुन r = 0.0;
    पूर्णांक i = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    पूर्णांक len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), block_आकार;

    जबतक (i < len0)
    {
        block_आकार = std::min(len0 - i, blockSize0);
        v_float32 v_योग = vx_setzero_f32();

        पूर्णांक j = 0;
        पूर्णांक cWidth = VTraits<v_float32>::vlanes();

#अगर CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        क्रम (; j <= block_आकार - (cWidth * 4); j += (cWidth * 4))
        {
            v_योग  = v_muladd(vx_load(src1 + j),
                              vx_load(src2 + j), v_योग);
            v_sum1 = v_muladd(vx_load(src1 + j + cWidth),
                              vx_load(src2 + j + cWidth), v_sum1);
            v_sum2 = v_muladd(vx_load(src1 + j + (cWidth * 2)),
                              vx_load(src2 + j + (cWidth * 2)), v_sum2);
            v_sum3 = v_muladd(vx_load(src1 + j + (cWidth * 3)),
                              vx_load(src2 + j + (cWidth * 3)), v_sum3);
        }

        v_योग = v_add(v_योग, v_add(v_add(v_sum1, v_sum2), v_sum3));
#पूर्ण_अगर

        क्रम (; j <= block_आकार - cWidth; j += cWidth)
            v_योग = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_योग);

        r += v_reduce_योग(v_योग);

        src1 += block_आकार;
        src2 += block_आकार;
        i += block_आकार;
    }
    vx_cleanup();
#पूर्ण_अगर
    वापस r + dotProd_(src1, src2, len - i);
}

द्विगुन dotProd_64f(स्थिर द्विगुन* src1, स्थिर द्विगुन* src2, पूर्णांक len)
{
    वापस dotProd_(src1, src2, len);
}

#पूर्ण_अगर
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
