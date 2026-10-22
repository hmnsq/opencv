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

#અન્તર્ભાવ "precomp.hpp"

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
#જો defined(__i386__) || defined(_M_IX86) || (defined(__APPLE__) && defined(__x86_64__))
#define CV_GEMM_SIMD_ENABLED 0
#નહીંતર
#define CV_GEMM_SIMD_ENABLED 1
#endif
#endif

નામાકાશ cv {

// forward declarations
પ્રકારવ્યાખ્યા શૂન્ય (*TransformFunc)(અચલ શૂન્ય* src, શૂન્ય* dst, અચલ શૂન્ય* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn);
પ્રકારવ્યાખ્યા શૂન્ય (*ScaleAddFunc)(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, શૂન્ય* dst, પૂર્ણાંક len, અચલ શૂન્ય* alpha);
પ્રકારવ્યાખ્યા શૂન્ય (*MulTransposedFunc)(અચલ Mat& src, અચલ/*preallocated*/ Mat& dst, અચલ Mat& delta, દ્વિગુણ scale);
પ્રકારવ્યાખ્યા દ્વિગુણ (*MahalanobisImplFunc)(અચલ Mat& v1, અચલ Mat& v2, અચલ Mat& icovar, દ્વિગુણ *diff_buffer /*[len]*/, પૂર્ણાંક len /*=v1.total()*/);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations

શૂન્ય gemm32f(અચલ દશાંશ* src1, size_t src1_step, અચલ દશાંશ* src2, size_t src2_step,
             દશાંશ alpha, અચલ દશાંશ* src3, size_t src3_step, દશાંશ beta, દશાંશ* dst, size_t dst_step,
             પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags);
શૂન્ય gemm64f(અચલ દ્વિગુણ* src1, size_t src1_step, અચલ દ્વિગુણ* src2, size_t src2_step,
             દ્વિગુણ alpha, અચલ દ્વિગુણ* src3, size_t src3_step, દ્વિગુણ beta, દ્વિગુણ* dst, size_t dst_step,
             પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags);
શૂન્ય gemm32fc(અચલ દશાંશ* src1, size_t src1_step, અચલ દશાંશ* src2, size_t src2_step,
              દશાંશ alpha, અચલ દશાંશ* src3, size_t src3_step, દશાંશ beta, દશાંશ* dst, size_t dst_step,
              પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags);
શૂન્ય gemm64fc(અચલ દ્વિગુણ* src1, size_t src1_step, અચલ દ્વિગુણ* src2, size_t src2_step,
              દ્વિગુણ alpha, અચલ દ્વિગુણ* src3, size_t src3_step, દ્વિગુણ beta, દ્વિગુણ* dst, size_t dst_step,
              પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags);

TransformFunc getTransformFunc(પૂર્ણાંક depth);
TransformFunc getDiagTransformFunc(પૂર્ણાંક depth);
TransformFunc getPerspectiveTransform(પૂર્ણાંક depth);

ScaleAddFunc getScaleAddFunc(પૂર્ણાંક depth);

MahalanobisImplFunc getMahalanobisImplFunc(પૂર્ણાંક depth);

MulTransposedFunc getMulTransposedFunc(પૂર્ણાંક stype, પૂર્ણાંક dtype, તાર્કિક ata);

દ્વિગુણ dotProd_8u(અચલ uchar* src1, અચલ uchar* src2, પૂર્ણાંક len);
દ્વિગુણ dotProd_8s(અચલ schar* src1, અચલ schar* src2, પૂર્ણાંક len);
દ્વિગુણ dotProd_16u(અચલ ushort* src1, અચલ ushort* src2, પૂર્ણાંક len);
દ્વિગુણ dotProd_16s(અચલ short* src1, અચલ short* src2, પૂર્ણાંક len);
દ્વિગુણ dotProd_32s(અચલ પૂર્ણાંક* src1, અચલ પૂર્ણાંક* src2, પૂર્ણાંક len);
દ્વિગુણ dotProd_32f(અચલ દશાંશ* src1, અચલ દશાંશ* src2, પૂર્ણાંક len);
દ્વિગુણ dotProd_64f(અચલ દ્વિગુણ* src1, અચલ દ્વિગુણ* src2, પૂર્ણાંક len);



#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

#જો !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

સ્થિર શૂન્ય
GEMM_CopyBlock( અચલ uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                Size size, size_t pix_size )
{
    પૂર્ણાંક j;
    size.width *= (પૂર્ણાંક)(pix_size / કદ(પૂર્ણાંક));

    માટે( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #જો CV_ENABLE_UNROLLED
        માટે( ; j <= size.width - 4; j += 4 )
        {
            પૂર્ણાંક t0 = ((અચલ પૂર્ણાંક*)src)[j];
            પૂર્ણાંક t1 = ((અચલ પૂર્ણાંક*)src)[j+1];
            ((પૂર્ણાંક*)dst)[j] = t0;
            ((પૂર્ણાંક*)dst)[j+1] = t1;
            t0 = ((અચલ પૂર્ણાંક*)src)[j+2];
            t1 = ((અચલ પૂર્ણાંક*)src)[j+3];
            ((પૂર્ણાંક*)dst)[j+2] = t0;
            ((પૂર્ણાંક*)dst)[j+3] = t1;
        }
        #endif
        માટે( ; j < size.width; j++ )
            ((પૂર્ણાંક*)dst)[j] = ((અચલ પૂર્ણાંક*)src)[j];
    }
}


સ્થિર શૂન્ય
GEMM_TransposeBlock( અચલ uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     Size size, size_t pix_size )
{
    પૂર્ણાંક i, j;
    માટે( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        અચલ uchar* _src = src;
        પસંદગી( pix_size )
        {
        સ્થિતિ કદ(પૂર્ણાંક):
            માટે( j = 0; j < size.height; j++, _src += src_step )
                ((પૂર્ણાંક*)dst)[j] = ((પૂર્ણાંક*)_src)[0];
            તોડો;
        સ્થિતિ કદ(પૂર્ણાંક)*2:
            માટે( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                પૂર્ણાંક t0 = ((પૂર્ણાંક*)_src)[0];
                પૂર્ણાંક t1 = ((પૂર્ણાંક*)_src)[1];
                ((પૂર્ણાંક*)dst)[j] = t0;
                ((પૂર્ણાંક*)dst)[j+1] = t1;
            }
            તોડો;
        સ્થિતિ કદ(પૂર્ણાંક)*4:
            માટે( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                પૂર્ણાંક t0 = ((પૂર્ણાંક*)_src)[0];
                પૂર્ણાંક t1 = ((પૂર્ણાંક*)_src)[1];
                ((પૂર્ણાંક*)dst)[j] = t0;
                ((પૂર્ણાંક*)dst)[j+1] = t1;
                t0 = ((પૂર્ણાંક*)_src)[2];
                t1 = ((પૂર્ણાંક*)_src)[3];
                ((પૂર્ણાંક*)dst)[j+2] = t0;
                ((પૂર્ણાંક*)dst)[j+3] = t1;
            }
            તોડો;
        મૂળભૂત:
            CV_Assert(0);
            પાછા;
        }
    }
}

#જો (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST> સંરચના VecTraits;

template<> સંરચના VecTraits<દશાંશ> {
    પ્રકારવ્યાખ્યા v_float32 VT;
    સ્થિર અંતઃરેખ VT zero() { પાછા vx_setzero_f32(); }
    સ્થિર અંતઃરેખ VT load(અચલ દશાંશ* p) { પાછા vx_load(p); }
    સ્થિર અંતઃરેખ VT setall(દશાંશ v) { પાછા vx_setall_f32(v); }
};
#endif

#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> સંરચના VecTraits<દ્વિગુણ> {
    પ્રકારવ્યાખ્યા v_float64 VT;
    સ્થિર અંતઃરેખ VT zero() { પાછા vx_setzero_f64(); }
    સ્થિર અંતઃરેખ VT load(અચલ દ્વિગુણ* p) { પાછા vx_load(p); }
    સ્થિર અંતઃરેખ VT setall(દ્વિગુણ v) { પાછા vx_setall_f64(v); }
};
#endif

#જો (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
સ્થિર અંતઃરેખ ST simdDotProduct(અચલ ST* a, અચલ ST* b, પૂર્ણાંક n, પૂર્ણાંક& k)
{
    પ્રકારવ્યાખ્યા typename VecTraits<ST>::VT VecT;
    અચલ પૂર્ણાંક vlanes = VTraits<VecT>::vlanes();
    અચલ પૂર્ણાંક stride4 = vlanes * 4;
    VecT vs0 = VecTraits<ST>::zero();
    VecT vs1 = VecTraits<ST>::zero();
    VecT vs2 = VecTraits<ST>::zero();
    VecT vs3 = VecTraits<ST>::zero();
    માટે( ; k <= n - stride4; k += stride4 )
    {
        vs0 = v_muladd(VecTraits<ST>::load(a + k),              VecTraits<ST>::load(b + k),              vs0);
        vs1 = v_muladd(VecTraits<ST>::load(a + k + vlanes),     VecTraits<ST>::load(b + k + vlanes),     vs1);
        vs2 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 2), VecTraits<ST>::load(b + k + vlanes * 2), vs2);
        vs3 = v_muladd(VecTraits<ST>::load(a + k + vlanes * 3), VecTraits<ST>::load(b + k + vlanes * 3), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    માટે( ; k <= n - vlanes; k += vlanes )
        vs0 = v_muladd(VecTraits<ST>::load(a + k), VecTraits<ST>::load(b + k), vs0);
    પાછા v_reduce_sum(vs0);
}
#endif

#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
// Float input, double accumulation dot product (for 32FC1 where WT=double)
સ્થિર અંતઃરેખ દ્વિગુણ simdDotProduct_f32f64(અચલ દશાંશ* a, અચલ દશાંશ* b, પૂર્ણાંક n, પૂર્ણાંક& k)
{
    અચલ પૂર્ણાંક vlanes_f32 = VTraits<v_float32>::vlanes();
    અચલ પૂર્ણાંક stride2 = vlanes_f32 * 2;
    v_float64 vs0 = vx_setzero_f64();
    v_float64 vs1 = vx_setzero_f64();
    v_float64 vs2 = vx_setzero_f64();
    v_float64 vs3 = vx_setzero_f64();
    માટે( ; k <= n - stride2; k += stride2 )
    {
        v_float32 va0 = vx_load(a + k);
        v_float32 vb0 = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va0), v_cvt_f64(vb0), vs0);
        vs1 = v_muladd(v_cvt_f64_ઉપલું(va0), v_cvt_f64_ઉપલું(vb0), vs1);

        v_float32 va1 = vx_load(a + k + vlanes_f32);
        v_float32 vb1 = vx_load(b + k + vlanes_f32);
        vs2 = v_muladd(v_cvt_f64(va1), v_cvt_f64(vb1), vs2);
        vs3 = v_muladd(v_cvt_f64_ઉપલું(va1), v_cvt_f64_ઉપલું(vb1), vs3);
    }
    vs0 = v_add(v_add(vs0, vs1), v_add(vs2, vs3));
    માટે( ; k <= n - vlanes_f32; k += vlanes_f32 )
    {
        v_float32 va = vx_load(a + k);
        v_float32 vb = vx_load(b + k);
        vs0 = v_muladd(v_cvt_f64(va), v_cvt_f64(vb), vs0);
        vs0 = v_muladd(v_cvt_f64_ઉપલું(va), v_cvt_f64_ઉપલું(vb), vs0);
    }
    પાછા v_reduce_sum(vs0);
}

// Float input, double accumulation GEMM k-outer/j-inner (for 32FC1 where WT=double)
સ્થિર અંતઃરેખ શૂન્ય simdGEMM_kj_f32f64(
    અચલ દશાંશ* a_data, અચલ દશાંશ* _b_data, size_t b_step,
    અચલ દશાંશ* _c_data, size_t c_step1,
    દશાંશ* d_data,
    પૂર્ણાંક n, પૂર્ણાંક m,
    દ્વિગુણ alpha, દ્વિગુણ beta,
    પૂર્ણાંક& j)
{
    અચલ પૂર્ણાંક vlanes_f32 = VTraits<v_float32>::vlanes();
    અચલ પૂર્ણાંક vlanes_f64 = VTraits<v_float64>::vlanes();

    જો( m >= 2 * vlanes_f32 && n >= 64 )
    {
        // k-outer / j-inner with double accumulation
        અચલ પૂર્ણાંક max_m = 1600 / (પૂર્ણાંક)કદ(દશાંશ);
        cv::AutoBuffer<દ્વિગુણ> _s_buf(max_m);
        દ્વિગુણ* s_buf = _s_buf.data();

        {
            v_float64 vz = vx_setzero_f64();
            માટે( j = 0; j <= m - vlanes_f64; j += vlanes_f64 )
                v_store(s_buf + j, vz);
            માટે( ; j < m; j++ )
                s_buf[j] = 0.0;
        }

        {
            અચલ દશાંશ* b_ptr = _b_data;
            માટે( પૂર્ણાંક k = 0; k < n; k++, b_ptr += b_step )
            {
                v_float64 va = vx_setall_f64((દ્વિગુણ)a_data[k]);
                j = 0;
                માટે( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float32 vb = vx_load(b_ptr + j);
                    v_float64 vb_lo = v_cvt_f64(vb);
                    v_float64 vb_hi = v_cvt_f64_ઉપલું(vb);
                    v_float64 vd_lo = vx_load(s_buf + j);
                    v_float64 vd_hi = vx_load(s_buf + j + vlanes_f64);
                    vd_lo = v_muladd(va, vb_lo, vd_lo);
                    vd_hi = v_muladd(va, vb_hi, vd_hi);
                    v_store(s_buf + j, vd_lo);
                    v_store(s_buf + j + vlanes_f64, vd_hi);
                }
                દ્વિગુણ a_val = (દ્વિગુણ)a_data[k];
                માટે( ; j < m; j++ )
                    s_buf[j] += a_val * (દ્વિગુણ)b_ptr[j];
            }
        }

        {
            v_float64 valpha = vx_setall_f64(alpha);
            અચલ દશાંશ* c_data = _c_data;
            j = 0;
            જો( !_c_data )
            {
                માટે( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                માટે( ; j < m; j++ )
                    d_data[j] = (દશાંશ)(s_buf[j] * alpha);
            }
            નહીંતર જો( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                માટે( ; j <= m - vlanes_f32; j += vlanes_f32 )
                {
                    v_float64 vs_lo = v_mul(vx_load(s_buf + j), valpha);
                    v_float64 vs_hi = v_mul(vx_load(s_buf + j + vlanes_f64), valpha);
                    v_float32 vc = vx_load(c_data + j);
                    vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                    vs_hi = v_muladd(v_cvt_f64_ઉપલું(vc), vbeta, vs_hi);
                    v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
                }
                માટે( ; j < m; j++ )
                    d_data[j] = (દશાંશ)(s_buf[j] * alpha + (દ્વિગુણ)c_data[j] * beta);
            }
            નહીંતર
            {
                માટે( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = (દશાંશ)(s_buf[j] * alpha + (દ્વિગુણ)c_data[0] * beta);
            }
        }
        j = m;
    }
    નહીંતર જો( vlanes_f32 * (પૂર્ણાંક)કદ(દશાંશ) >= 32 )
    {
        // j-outer with SIMD: double accumulation per vlanes_f64 group
        અચલ દશાંશ* c_data = _c_data;
        માટે( ; j <= m - vlanes_f32; j += vlanes_f32, c_data += vlanes_f32 * c_step1 )
        {
            અચલ દશાંશ* b = _b_data + j;
            v_float64 vs_lo = vx_setzero_f64();
            v_float64 vs_hi = vx_setzero_f64();
            માટે( પૂર્ણાંક k = 0; k < n; k++, b += b_step )
            {
                v_float64 va = vx_setall_f64((દ્વિગુણ)a_data[k]);
                v_float32 vb = vx_load(b);
                vs_lo = v_muladd(va, v_cvt_f64(vb), vs_lo);
                vs_hi = v_muladd(va, v_cvt_f64_ઉપલું(vb), vs_hi);
            }
            v_float64 valpha = vx_setall_f64(alpha);
            vs_lo = v_mul(vs_lo, valpha);
            vs_hi = v_mul(vs_hi, valpha);
            જો( !_c_data )
            {
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            નહીંતર જો( c_step1 == 1 )
            {
                v_float64 vbeta = vx_setall_f64(beta);
                v_float32 vc = vx_load(c_data);
                vs_lo = v_muladd(v_cvt_f64(vc), vbeta, vs_lo);
                vs_hi = v_muladd(v_cvt_f64_ઉપલું(vc), vbeta, vs_hi);
                v_store(d_data + j, v_cvt_f32(vs_lo, vs_hi));
            }
            નહીંતર
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) દશાંશ buf[VTraits<v_float32>::max_nlanes];
                v_store_aligned(buf, v_cvt_f32(vs_lo, vs_hi));
                માટે( પૂર્ણાંક jj = 0; jj < vlanes_f32; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (દશાંશ)beta;
            }
        }
    }
}
#endif

#જો (CV_SIMD || CV_SIMD_SCALABLE)
template<typename ST>
સ્થિર અંતઃરેખ શૂન્ય simdGEMM_kj(
    અચલ ST* a_data, અચલ ST* _b_data, size_t b_step,
    અચલ ST* _c_data, size_t c_step1,
    ST* d_data,
    પૂર્ણાંક n, પૂર્ણાંક m,
    દ્વિગુણ alpha, દ્વિગુણ beta,
    પૂર્ણાંક& j)
{
    પ્રકારવ્યાખ્યા typename VecTraits<ST>::VT VecT;
    અચલ પૂર્ણાંક vlanes = VTraits<VecT>::vlanes();

    જો( m >= 2 * vlanes && n >= 64 )
    {
        // k-outer / j-inner SIMD: sequential B-row access, good for large n
        અચલ પૂર્ણાંક max_m = 1600 / (પૂર્ણાંક)કદ(ST);
        CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST s_buf[max_m];

        {
            VecT vz = VecTraits<ST>::zero();
            માટે( j = 0; j <= m - vlanes; j += vlanes )
                v_store_aligned(s_buf + j, vz);
            માટે( ; j < m; j++ )
                s_buf[j] = (ST)0;
        }

        {
            અચલ ST* b_ptr = _b_data;
            માટે( પૂર્ણાંક k = 0; k < n; k++, b_ptr += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                j = 0;
                માટે( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vd = vx_load_aligned(s_buf + j);
                    VecT vb = VecTraits<ST>::load(b_ptr + j);
                    vd = v_muladd(va, vb, vd);
                    v_store_aligned(s_buf + j, vd);
                }
                ST a_val = a_data[k];
                માટે( ; j < m; j++ )
                    s_buf[j] += a_val * b_ptr[j];
            }
        }

        {
            VecT valpha = VecTraits<ST>::setall((ST)alpha);
            અચલ ST* c_data = _c_data;
            j = 0;
            જો( !_c_data )
            {
                માટે( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    v_store(d_data + j, vs);
                }
                માટે( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha;
            }
            નહીંતર જો( c_step1 == 1 )
            {
                VecT vbeta = VecTraits<ST>::setall((ST)beta);
                માટે( ; j <= m - vlanes; j += vlanes )
                {
                    VecT vs = v_mul(vx_load_aligned(s_buf + j), valpha);
                    VecT vc = VecTraits<ST>::load(c_data + j);
                    vs = v_muladd(vc, vbeta, vs);
                    v_store(d_data + j, vs);
                }
                માટે( ; j < m; j++ )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[j] * (ST)beta;
            }
            નહીંતર
            {
                માટે( j = 0; j < m; j++, c_data += c_step1 )
                    d_data[j] = s_buf[j] * (ST)alpha + c_data[0] * (ST)beta;
            }
        }
        j = m; // all columns handled
    }
    નહીંતર જો( vlanes * (પૂર્ણાંક)કદ(ST) >= 32 )
    {
        // j-outer with SIMD: process vlanes columns per group
        // (skip on narrow SIMD where 4-column scalar is faster)
        VecT valpha = VecTraits<ST>::setall((ST)alpha);
        અચલ ST* c_data = _c_data;
        માટે( ; j <= m - vlanes; j += vlanes, c_data += vlanes * c_step1 )
        {
            અચલ ST* b = _b_data + j;
            VecT vs = VecTraits<ST>::zero();
            માટે( પૂર્ણાંક k = 0; k < n; k++, b += b_step )
            {
                VecT va = VecTraits<ST>::setall(a_data[k]);
                VecT vb = VecTraits<ST>::load(b);
                vs = v_muladd(va, vb, vs);
            }
            vs = v_mul(vs, valpha);
            જો( !_c_data )
            {
                v_store(d_data + j, vs);
            }
            નહીંતર જો( c_step1 == 1 )
            {
                VecT vc = VecTraits<ST>::load(c_data);
                vs = v_muladd(vc, VecTraits<ST>::setall((ST)beta), vs);
                v_store(d_data + j, vs);
            }
            નહીંતર
            {
                CV_DECL_ALIGNED(CV_SIMD_WIDTH) ST buf[VTraits<VecT>::max_nlanes];
                v_store_aligned(buf, vs);
                માટે( પૂર્ણાંક jj = 0; jj < vlanes; jj++ )
                    d_data[j + jj] = buf[jj] + c_data[jj * c_step1] * (ST)beta;
            }
        }
        // Remaining columns handled by 4-column scalar in caller
    }
}
#endif


template<typename T, typename WT> સ્થિર શૂન્ય
GEMMSingleMul( અચલ T* a_data, size_t a_step,
               અચલ T* b_data, size_t b_step,
               અચલ T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               Size a_size, Size d_size,
               દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    પૂર્ણાંક i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    અચલ T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= કદ(a_data[0]);
    b_step /= કદ(b_data[0]);
    c_step /= કદ(c_data[0]);
    d_step /= કદ(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    જો( !c_data )
        c_step0 = c_step1 = 0;
    નહીંતર જો( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    નહીંતર
        c_step0 = 1, c_step1 = c_step;

    જો( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        જો( a_step > 1 && n > 1 )
        {
            _a_buf.allocate(n);
            a_buf = _a_buf.data();
        }
    }

    જો( n == 1 ) /* external product */
    {
        cv::AutoBuffer<T> _b_buf;
        T* b_buf = 0;

        જો( a_step > 1 && a_size.height > 1 )
        {
            _a_buf.allocate(drows);
            a_buf = _a_buf.data();
            માટે( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        જો( b_step > 1 )
        {
            _b_buf.allocate(d_size.width);
            b_buf = _b_buf.data();
            માટે( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        માટે( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            માટે( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                જો( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                નહીંતર
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            માટે( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                જો( !c_data )
                    d_data[j] = T(s0);
                નહીંતર
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    નહીંતર જો( flags & GEMM_2_T ) /* A * Bt */
    {
        માટે( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            જો( a_buf )
            {
                માટે( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            માટે( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0);
                k = 0;
#જો CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                જો( કદ(WT) == કદ(દ્વિગુણ) )
                {
                    જો( કદ(T) == કદ(દ્વિગુણ) )
                        s0 = (WT)simdDotProduct((અચલ દ્વિગુણ*)a_data, (અચલ દ્વિગુણ*)b_data, n, k);
                    નહીંતર જો( કદ(T) == કદ(દશાંશ) )
                        s0 = (WT)simdDotProduct_f32f64((અચલ દશાંશ*)a_data, (અચલ દશાંશ*)b_data, n, k);
                }
                નહીંતર
#endif
                {
                    WT s1(0), s2(0), s3(0);
                     #જો CV_ENABLE_UNROLLED
                    માટે( ; k <= n - 4; k += 4 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                        s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                        s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                    }
                    #endif
                    s0 += s1 + s2 + s3;
                }
                માટે( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = s0*alpha;

                જો( !c_data )
                    d_data[j] = T(s0);
                નહીંતર
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    નહીંતર જો( d_size.width*કદ(d_data[0]) <= 1600 )
    {
        માટે( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            જો( a_buf )
            {
                માટે( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            j = 0;
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            જો( કદ(WT) == કદ(દ્વિગુણ) )
            {
                જો( કદ(T) == કદ(દ્વિગુણ) )
                    simdGEMM_kj((અચલ દ્વિગુણ*)a_data, (અચલ દ્વિગુણ*)_b_data, b_step,
                                        (અચલ દ્વિગુણ*)_c_data, c_step1,
                                (દ્વિગુણ*)d_data, n, m, alpha, beta, j);
                નહીંતર જો( કદ(T) == કદ(દશાંશ) )
                    simdGEMM_kj_f32f64((અચલ દશાંશ*)a_data, (અચલ દશાંશ*)_b_data, b_step,
                                        (અચલ દશાંશ*)_c_data, c_step1,
                                (દશાંશ*)d_data, n, m, alpha, beta, j);
            }
#endif
            c_data = _c_data + j * c_step1;
            // 4-column j-outer with register accumulators for remaining columns
            માટે( ; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                અચલ T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                માટે( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                જો( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                નહીંતર
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            માટે( ; j < m; j++, c_data += c_step1 )
            {
                અચલ T* b = _b_data + j;
                WT s0(0);

                માટે( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                જો( !c_data )
                    d_data[j] = T(s0);
                નહીંતર
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    નહીંતર
    {
        cv::AutoBuffer<WT> _d_buf(m);
        WT* d_buf = _d_buf.data();

        માટે( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            જો( a_buf )
            {
                માટે( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            માટે( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            માટે( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
                જો( કદ(WT) == કદ(દ્વિગુણ) )
                {
                    જો( કદ(T) == કદ(દ્વિગુણ) )
                    {
                        અચલ પૂર્ણાંક vlanes = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((અચલ દ્વિગુણ*)&al));
                        માટે( ; j <= m - vlanes; j += vlanes )
                        {
                            v_float64 vd = vx_load((અચલ દ્વિગુણ*)(d_buf + j));
                            v_float64 vb = vx_load((અચલ દ્વિગુણ*)(b_data + j));
                            vd = v_muladd(vb, val, vd);
                            v_store((દ્વિગુણ*)(d_buf + j), vd);
                        }
                    }
                    નહીંતર
                    {
                        અચલ પૂર્ણાંક vlanes_f32 = VTraits<v_float32>::vlanes();
                        અચલ પૂર્ણાંક vlanes_f64 = VTraits<v_float64>::vlanes();
                        v_float64 val = vx_setall_f64(*((અચલ દ્વિગુણ*)&al));
                        માટે( ; j <= m - vlanes_f32; j += vlanes_f32 )
                        {
                            v_float32 vb = vx_load((અચલ દશાંશ*)(b_data + j));
                            v_float64 vb_lo = v_cvt_f64(vb);
                            v_float64 vb_hi = v_cvt_f64_ઉપલું(vb);
                            v_float64 vd_lo = vx_load((અચલ દ્વિગુણ*)(d_buf + j));
                            v_float64 vd_hi = vx_load((અચલ દ્વિગુણ*)(d_buf + j + vlanes_f64));
                            vd_lo = v_muladd(vb_lo, val, vd_lo);
                            vd_hi = v_muladd(vb_hi, val, vd_hi);
                            v_store((દ્વિગુણ*)(d_buf + j), vd_lo);
                            v_store((દ્વિગુણ*)(d_buf + j + vlanes_f64), vd_hi);
                        }
                    }
                }
#endif
                માટે( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            જો( !c_data )
                માટે( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            નહીંતર
                માટે( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
    }
}
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<typename WT>
સ્થિર અંતઃરેખ શૂન્ય setDoubleDataZero(WT* d_data, પૂર્ણાંક m, પૂર્ણાંક vlanes)
{
    v_float64 vz = vx_setzero_f64();
    પૂર્ણાંક j = 0;
    માટે( ; j <= m - vlanes; j += vlanes )
        v_store((દ્વિગુણ*)(d_data + j), vz);
    માટે( ; j < m; j++ )
        d_data[j] = WT(0);
}

template<typename ST>
સ્થિર અંતઃરેખ શૂન્ય simdMulAdd(અચલ ST* b, દ્વિગુણ* d, પૂર્ણાંક m, દ્વિગુણ a_val);

template<>
અંતઃરેખ શૂન્ય simdMulAdd<દશાંશ>(
    અચલ દશાંશ* b, દ્વિગુણ* d, પૂર્ણાંક m, દ્વિગુણ a_val)
{
    અચલ પૂર્ણાંક vlanes_f32 = VTraits<v_float32>::vlanes();
    અચલ પૂર્ણાંક vlanes_f64 = VTraits<v_float64>::vlanes();
    v_float64 va = vx_setall_f64(a_val);
    પૂર્ણાંક j = 0;
    માટે( ; j <= m - vlanes_f32; j += vlanes_f32 )
    {
        v_float32 vb    = vx_load(b + j);
        v_float64 vb_lo = v_cvt_f64(vb);
        v_float64 vb_hi = v_cvt_f64_ઉપલું(vb);
        v_float64 vd_lo = vx_load(d + j);
        v_float64 vd_hi = vx_load(d + j + vlanes_f64);
        vd_lo = v_muladd(va, vb_lo, vd_lo);
        vd_hi = v_muladd(va, vb_hi, vd_hi);
        v_store(d + j, vd_lo);
        v_store(d + j + vlanes_f64, vd_hi);
    }
    માટે( ; j < m; j++ )
        d[j] += a_val * (દ્વિગુણ)b[j];
}

template<>
અંતઃરેખ શૂન્ય simdMulAdd<દ્વિગુણ>(
    અચલ દ્વિગુણ* b, દ્વિગુણ* d, પૂર્ણાંક m, દ્વિગુણ a_val)
{
    અચલ પૂર્ણાંક vlanes = VTraits<v_float64>::vlanes();
    અચલ પૂર્ણાંક stride2 = vlanes * 2;
    v_float64 va = vx_setall_f64(a_val);
    પૂર્ણાંક j = 0;
    માટે( ; j <= m - stride2; j += stride2 )
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
    માટે( ; j <= m - vlanes; j += vlanes )
    {
        v_float64 vd = vx_load(d + j);
        v_float64 vb = vx_load(b + j);
        vd = v_muladd(va, vb, vd);
        v_store(d + j, vd);
    }
    માટે( ; j < m; j++ )
        d[j] += a_val * b[j];
}

template<typename ST>
સ્થિર અંતઃરેખ શૂન્ય simdBlockMul_kj(
    અચલ ST* a_data, અચલ ST* b_data, size_t b_step,
    દ્વિગુણ* d_data, પૂર્ણાંક n, પૂર્ણાંક m, તાર્કિક do_acc)
{
    અચલ પૂર્ણાંક vlanes = VTraits<v_float64>::vlanes();
    જો( !do_acc )
        setDoubleDataZero(d_data, m, vlanes);
    માટે( પૂર્ણાંક k = 0; k < n; k++, b_data += b_step )
        simdMulAdd(b_data, d_data, m, (દ્વિગુણ)a_data[k]);
}
#endif // CV_SIMD_64F || CV_SIMD_SCALABLE_64F

template<typename T, typename WT> સ્થિર શૂન્ય
GEMMBlockMul( અચલ T* a_data, size_t a_step,
              અચલ T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              Size a_size, Size d_size, પૂર્ણાંક flags )
{
    પૂર્ણાંક i, j, k, n = a_size.width, m = d_size.width;
    અચલ T *_a_data = a_data, *_b_data = b_data;
    cv::AutoBuffer<T> _a_buf;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    પૂર્ણાંક do_acc = flags & 16;

    a_step /= કદ(a_data[0]);
    b_step /= કદ(b_data[0]);
    d_step /= કદ(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    જો( flags & GEMM_1_T )
    {
        CV_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf.allocate(n);
        a_buf = _a_buf.data();
    }

    જો( flags & GEMM_2_T )
    {
        /* second operand is transposed */
        માટે( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            જો( a_buf )
            {
                માટે( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            માટે( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j] : WT(0);
                k = 0;
#જો CV_SIMD_64F && CV_GEMM_SIMD_ENABLED
                જો( કદ(WT) == કદ(દ્વિગુણ) )
                {
                    જો( કદ(T) == કદ(દ્વિગુણ) )
                        s0 += (WT)simdDotProduct((અચલ દ્વિગુણ*)a_data, (અચલ દ્વિગુણ*)b_data, n, k);
                    નહીંતર જો( કદ(T) == કદ(દશાંશ) )
                        s0 += (WT)simdDotProduct_f32f64((અચલ દશાંશ*)a_data, (અચલ દશાંશ*)b_data, n, k);
                }
                નહીંતર
#endif
                {
                    WT s1(0);
                    માટે( ; k <= n - 2; k += 2 )
                    {
                        s0 += WT(a_data[k])*WT(b_data[k]);
                        s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    }
                    s0 += s1;
                }
                માટે( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0;
            }
        }
    }
    નહીંતર
    {
        માટે( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            જો( a_buf )
            {
                માટે( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F) && CV_GEMM_SIMD_ENABLED
            જો( કદ(WT) == કદ(દ્વિગુણ) )
            {
                જો( કદ(T) == કદ(દ્વિગુણ) )
                    simdBlockMul_kj(
                        (અચલ દ્વિગુણ*)a_data, (અચલ દ્વિગુણ*)b_data, b_step,
                        (દ્વિગુણ*)d_data, n, m, do_acc != 0);
                નહીંતર
                    simdBlockMul_kj(
                        (અચલ દશાંશ*)a_data, (અચલ દશાંશ*)b_data, b_step,
                        (દ્વિગુણ*)d_data, n, m, do_acc != 0);
            }
            નહીંતર
#endif
            {
                માટે( j = 0; j <= m - 4; j += 4 )
                {
                    WT s0, s1, s2, s3;
                    અચલ T* b = b_data + j;

                    જો( do_acc )
                    {
                        s0 = d_data[j]; s1 = d_data[j+1];
                        s2 = d_data[j+2]; s3 = d_data[j+3];
                    }
                    નહીંતર
                        s0 = s1 = s2 = s3 = WT(0);

                    માટે( k = 0; k < n; k++, b += b_step )
                    {
                        WT a(a_data[k]);
                        s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                        s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                    }

                    d_data[j] = s0; d_data[j+1] = s1;
                    d_data[j+2] = s2; d_data[j+3] = s3;
                }

                માટે( ; j < m; j++ )
                {
                    અચલ T* b = b_data + j;
                    WT s0 = do_acc ? d_data[j] : WT(0);

                    માટે( k = 0; k < n; k++, b += b_step )
                        s0 += WT(a_data[k]) * WT(b[0]);

                    d_data[j] = s0;
                }
            }
        }
    }
}


template<typename T, typename WT> સ્થિર શૂન્ય
GEMMStore( અચલ T* c_data, size_t c_step,
           અચલ WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, Size d_size,
           દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    અચલ T* _c_data = c_data;
    પૂર્ણાંક j;
    size_t c_step0, c_step1;

    c_step /= કદ(c_data[0]);
    d_buf_step /= કદ(d_buf[0]);
    d_step /= કદ(d_data[0]);

    જો( !c_data )
        c_step0 = c_step1 = 0;
    નહીંતર જો( !(flags & GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    નહીંતર
        c_step0 = 1, c_step1 = c_step;

    માટે( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        જો( _c_data )
        {
            c_data = _c_data;
            j=0;
             #જો CV_ENABLE_UNROLLED
            માટે(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
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
            માટે( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = alpha*d_buf[j];
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        નહીંતર
        {
            j = 0;
             #જો CV_ENABLE_UNROLLED
            માટે( ; j <= d_size.width - 4; j += 4 )
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
            માટે( ; j < d_size.width; j++ )
                d_data[j] = T(alpha*d_buf[j]);
        }
    }
}


પ્રકારવ્યાખ્યા શૂન્ય (*GEMMSingleMulFunc)( અચલ શૂન્ય* src1, size_t step1,
                   અચલ શૂન્ય* src2, size_t step2, અચલ શૂન્ય* src3, size_t step3,
                   શૂન્ય* dst, size_t dststep, Size srcsize, Size dstsize,
                   દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags );

પ્રકારવ્યાખ્યા શૂન્ય (*GEMMBlockMulFunc)( અચલ શૂન્ય* src1, size_t step1,
                   અચલ શૂન્ય* src2, size_t step2, શૂન્ય* dst, size_t dststep,
                   Size srcsize, Size dstsize, પૂર્ણાંક flags );

પ્રકારવ્યાખ્યા શૂન્ય (*GEMMStoreFunc)( અચલ શૂન્ય* src1, size_t step1,
                   અચલ શૂન્ય* src2, size_t step2, શૂન્ય* dst, size_t dststep,
                   Size dstsize, દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags );

template<typename T, typename WT, શૂન્ય (*fn)(અચલ T*, size_t, અચલ T*, size_t, અચલ T*, size_t, T*, size_t, Size, Size, દ્વિગુણ, દ્વિગુણ, પૂર્ણાંક)>
સ્થિર શૂન્ય gemmSingleMulWrap( અચલ શૂન્ય* src1, size_t step1,
                   અચલ શૂન્ય* src2, size_t step2, અચલ શૂન્ય* src3, size_t step3,
                   શૂન્ય* dst, size_t dststep, Size srcsize, Size dstsize,
                   દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    fn((અચલ T*)src1, step1, (અચલ T*)src2, step2, (અચલ T*)src3, step3, (T*)dst, dststep, srcsize, dstsize, alpha, beta, flags);
}

template<typename T, typename WT, શૂન્ય (*fn)(અચલ T*, size_t, અચલ T*, size_t, WT*, size_t, Size, Size, પૂર્ણાંક)>
સ્થિર શૂન્ય gemmBlockMulWrap( અચલ શૂન્ય* src1, size_t step1,
                   અચલ શૂન્ય* src2, size_t step2, શૂન્ય* dst, size_t dststep,
                   Size srcsize, Size dstsize, પૂર્ણાંક flags )
{
    fn((અચલ T*)src1, step1, (અચલ T*)src2, step2, (WT*)dst, dststep, srcsize, dstsize, flags);
}

template<typename T, typename WT, શૂન્ય (*fn)(અચલ T*, size_t, અચલ WT*, size_t, T*, size_t, Size, દ્વિગુણ, દ્વિગુણ, પૂર્ણાંક)>
સ્થિર શૂન્ય gemmStoreWrap( અચલ શૂન્ય* src1, size_t step1,
                   અચલ શૂન્ય* src2, size_t step2, શૂન્ય* dst, size_t dststep,
                   Size dstsize, દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    fn((અચલ T*)src1, step1, (અચલ WT*)src2, step2, (T*)dst, dststep, dstsize, alpha, beta, flags);
}

સ્થિર શૂન્ય GEMMSingleMul_32f( અચલ દશાંશ* a_data, size_t a_step,
              અચલ દશાંશ* b_data, size_t b_step,
              અચલ દશાંશ* c_data, size_t c_step,
              દશાંશ* d_data, size_t d_step,
              Size a_size, Size d_size,
              દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    GEMMSingleMul<દશાંશ,દ્વિગુણ>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

સ્થિર શૂન્ય GEMMSingleMul_64f( અચલ દ્વિગુણ* a_data, size_t a_step,
                              અચલ દ્વિગુણ* b_data, size_t b_step,
                              અચલ દ્વિગુણ* c_data, size_t c_step,
                              દ્વિગુણ* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    GEMMSingleMul<દ્વિગુણ,દ્વિગુણ>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}


સ્થિર શૂન્ય GEMMSingleMul_32fc( અચલ Complexf* a_data, size_t a_step,
                              અચલ Complexf* b_data, size_t b_step,
                              અચલ Complexf* c_data, size_t c_step,
                              Complexf* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    GEMMSingleMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

સ્થિર શૂન્ય GEMMSingleMul_64fc( અચલ Complexd* a_data, size_t a_step,
                              અચલ Complexd* b_data, size_t b_step,
                              અચલ Complexd* c_data, size_t c_step,
                              Complexd* d_data, size_t d_step,
                              Size a_size, Size d_size,
                              દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    GEMMSingleMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

સ્થિર શૂન્ય GEMMBlockMul_32f( અચલ દશાંશ* a_data, size_t a_step,
             અચલ દશાંશ* b_data, size_t b_step,
             દ્વિગુણ* d_data, size_t d_step,
             Size a_size, Size d_size, પૂર્ણાંક flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


સ્થિર શૂન્ય GEMMBlockMul_64f( અચલ દ્વિગુણ* a_data, size_t a_step,
                             અચલ દ્વિગુણ* b_data, size_t b_step,
                             દ્વિગુણ* d_data, size_t d_step,
                             Size a_size, Size d_size, પૂર્ણાંક flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


સ્થિર શૂન્ય GEMMBlockMul_32fc( અચલ Complexf* a_data, size_t a_step,
                             અચલ Complexf* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, પૂર્ણાંક flags )
{
    GEMMBlockMul<Complexf,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


સ્થિર શૂન્ય GEMMBlockMul_64fc( અચલ Complexd* a_data, size_t a_step,
                             અચલ Complexd* b_data, size_t b_step,
                             Complexd* d_data, size_t d_step,
                             Size a_size, Size d_size, પૂર્ણાંક flags )
{
    GEMMBlockMul<Complexd,Complexd>(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


સ્થિર શૂન્ય GEMMStore_32f( અચલ દશાંશ* c_data, size_t c_step,
          અચલ દ્વિગુણ* d_buf, size_t d_buf_step,
          દશાંશ* d_data, size_t d_step, Size d_size,
          દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


સ્થિર શૂન્ય GEMMStore_64f( અચલ દ્વિગુણ* c_data, size_t c_step,
                      અચલ દ્વિગુણ* d_buf, size_t d_buf_step,
                      દ્વિગુણ* d_data, size_t d_step, Size d_size,
                      દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


સ્થિર શૂન્ય GEMMStore_32fc( અચલ Complexf* c_data, size_t c_step,
                          અચલ Complexd* d_buf, size_t d_buf_step,
                          Complexf* d_data, size_t d_step, Size d_size,
                          દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


સ્થિર શૂન્ય GEMMStore_64fc( અચલ Complexd* c_data, size_t c_step,
                          અચલ Complexd* d_buf, size_t d_buf_step,
                          Complexd* d_data, size_t d_step, Size d_size,
                          દ્વિગુણ alpha, દ્વિગુણ beta, પૂર્ણાંક flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

સ્થિર શૂન્ય gemmImpl( Mat A, Mat B, દ્વિગુણ alpha,
           Mat C, દ્વિગુણ beta, Mat D, પૂર્ણાંક flags )
{
    CV_INSTRUMENT_REGION();

    અચલ પૂર્ણાંક block_lin_size = 128;
    અચલ પૂર્ણાંક block_size = block_lin_size * block_lin_size;

    સ્થિર દ્વિગુણ zero[] = {0,0,0,0};
    સ્થિર દશાંશ zerof[] = {0,0,0,0};

    Size a_size = A.size(), d_size;
    પૂર્ણાંક i, len = 0, type = A.type();

    પસંદગી( flags & (GEMM_1_T|GEMM_2_T) )
    {
    સ્થિતિ 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        તોડો;
    સ્થિતિ 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        તોડો;
    સ્થિતિ 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        તોડો;
    સ્થિતિ 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        તોડો;
    }

    જો( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        જો( type == CV_32F )
        {
            દશાંશ* d = D.ptr<દશાંશ>();
            અચલ દશાંશ *a = A.ptr<દશાંશ>(),
                        *b = B.ptr<દશાંશ>(),
                        *c = (અચલ દશાંશ*)C.data;
            size_t d_step = D.step/કદ(d[0]),
                a_step = A.step/કદ(a[0]),
                b_step = B.step/કદ(b[0]),
                c_step = C.data ? C.step/કદ(c[0]) : 0;

            જો( !c )
                c = zerof;

            પસંદગી( len )
            {
            સ્થિતિ 2:
                જો( len == d_size.width && b != d )
                {
                    માટે( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        દશાંશ t0 = a[0]*b[0] + a[1]*b[b_step];
                        દશાંશ t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (દશાંશ)(t0*alpha + c[0]*beta);
                        d[1] = (દશાંશ)(t1*alpha + c[1]*beta);
                    }
                }
                નહીંતર જો( a != d )
                {
                    પૂર્ણાંક c_step0 = 1;
                    જો( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    માટે( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        દશાંશ t0 = a[0]*b[0] + a[1]*b[b_step];
                        દશાંશ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (દશાંશ)(t0*alpha + c[0]*beta);
                        d[d_step] = (દશાંશ)(t1*alpha + c[c_step]*beta);
                    }
                }
                નહીંતર
                    તોડો;
                પાછા;
            સ્થિતિ 3:
                જો( len == d_size.width && b != d )
                {
                    માટે( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        દશાંશ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        દશાંશ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        દશાંશ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (દશાંશ)(t0*alpha + c[0]*beta);
                        d[1] = (દશાંશ)(t1*alpha + c[1]*beta);
                        d[2] = (દશાંશ)(t2*alpha + c[2]*beta);
                    }
                }
                નહીંતર જો( a != d )
                {
                    પૂર્ણાંક c_step0 = 1;
                    જો( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    માટે( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        દશાંશ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        દશાંશ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        દશાંશ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (દશાંશ)(t0*alpha + c[0]*beta);
                        d[d_step] = (દશાંશ)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (દશાંશ)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                નહીંતર
                    તોડો;
                પાછા;
            સ્થિતિ 4:
                જો( len == d_size.width && b != d )
                {
                    માટે( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        દશાંશ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        દશાંશ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        દશાંશ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        દશાંશ t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (દશાંશ)(t0*alpha + c[0]*beta);
                        d[1] = (દશાંશ)(t1*alpha + c[1]*beta);
                        d[2] = (દશાંશ)(t2*alpha + c[2]*beta);
                        d[3] = (દશાંશ)(t3*alpha + c[3]*beta);
                    }
                }
                નહીંતર જો( len <= 16 && a != d )
                {
                    પૂર્ણાંક c_step0 = 1;
                    જો( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    માટે( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        દશાંશ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        દશાંશ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        દશાંશ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        દશાંશ t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (દશાંશ)(t0*alpha + c[0]*beta);
                        d[d_step] = (દશાંશ)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (દશાંશ)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (દશાંશ)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                નહીંતર
                    તોડો;
                પાછા;
            }
        }

        જો( type == CV_64F )
        {
            દ્વિગુણ* d = D.ptr<દ્વિગુણ>();
            અચલ દ્વિગુણ *a = A.ptr<દ્વિગુણ>(),
                         *b = B.ptr<દ્વિગુણ>(),
                         *c = (અચલ દ્વિગુણ*)C.data;
            size_t d_step = D.step/કદ(d[0]),
                a_step = A.step/કદ(a[0]),
                b_step = B.step/કદ(b[0]),
                c_step = C.data ? C.step/કદ(c[0]) : 0;
            જો( !c )
                c = zero;

            પસંદગી( len )
            {
            સ્થિતિ 2:
                જો( len == d_size.width && b != d )
                {
                    માટે( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        દ્વિગુણ t0 = a[0]*b[0] + a[1]*b[b_step];
                        દ્વિગુણ t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                નહીંતર જો( a != d )
                {
                    પૂર્ણાંક c_step0 = 1;
                    જો( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    માટે( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        દ્વિગુણ t0 = a[0]*b[0] + a[1]*b[b_step];
                        દ્વિગુણ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                નહીંતર
                    તોડો;
                પાછા;
            સ્થિતિ 3:
                જો( len == d_size.width && b != d )
                {
                    માટે( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        દ્વિગુણ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        દ્વિગુણ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        દ્વિગુણ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                નહીંતર જો( a != d )
                {
                    પૂર્ણાંક c_step0 = 1;
                    જો( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    માટે( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        દ્વિગુણ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        દ્વિગુણ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        દ્વિગુણ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                નહીંતર
                    તોડો;
                પાછા;
            સ્થિતિ 4:
                જો( len == d_size.width && b != d )
                {
                    માટે( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        દ્વિગુણ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        દ્વિગુણ t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        દ્વિગુણ t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        દ્વિગુણ t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                નહીંતર જો( d_size.width <= 16 && a != d )
                {
                    પૂર્ણાંક c_step0 = 1;
                    જો( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    માટે( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        દ્વિગુણ t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        દ્વિગુણ t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        દ્વિગુણ t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        દ્વિગુણ t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                નહીંતર
                    તોડો;
                પાછા;
            }
        }
    }

    {
    size_t b_step = B.step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    Mat *matD = &D;
    અચલ uchar* Cdata = C.data;
    size_t Cstep = C.data ? (size_t)C.step : 0;
    AutoBuffer<uchar> buf;

    જો( type == CV_32FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<દશાંશ, દ્વિગુણ, GEMMSingleMul_32f>;
        blockMulFunc = gemmBlockMulWrap<દશાંશ, દ્વિગુણ, GEMMBlockMul_32f>;
        storeFunc = gemmStoreWrap<દશાંશ, દ્વિગુણ, GEMMStore_32f>;
    }
    નહીંતર જો( type == CV_64FC1 )
    {
        singleMulFunc = gemmSingleMulWrap<દ્વિગુણ, દ્વિગુણ, GEMMSingleMul_64f>;
        blockMulFunc = gemmBlockMulWrap<દ્વિગુણ, દ્વિગુણ, GEMMBlockMul_64f>;
        storeFunc = gemmStoreWrap<દ્વિગુણ, દ્વિગુણ, GEMMStore_64f>;
    }
    નહીંતર જો( type == CV_32FC2 )
    {
        singleMulFunc = gemmSingleMulWrap<Complexf, Complexd, GEMMSingleMul_32fc>;
        blockMulFunc = gemmBlockMulWrap<Complexf, Complexd, GEMMBlockMul_32fc>;
        storeFunc = gemmStoreWrap<Complexf, Complexd, GEMMStore_32fc>;
    }
    નહીંતર
    {
        CV_Assert( type == CV_64FC2 );
        singleMulFunc = gemmSingleMulWrap<Complexd, Complexd, GEMMSingleMul_64fc>;
        blockMulFunc = gemmBlockMulWrap<Complexd, Complexd, GEMMBlockMul_64fc>;
        storeFunc = gemmStoreWrap<Complexd, Complexd, GEMMStore_64fc>;
    }

    જો( (d_size.width == 1 || len == 1) && !(flags & GEMM_2_T) && B.isContinuous() )
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
    else*/ જો( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A.ptr(), A.step, B.ptr(), b_step, Cdata, Cstep,
                       matD->ptr(), matD->step, a_size, d_size, alpha, beta, flags );
    }
    નહીંતર
    {
        પૂર્ણાંક is_a_t = flags & GEMM_1_T;
        પૂર્ણાંક is_b_t = flags & GEMM_2_T;
        પૂર્ણાંક elem_size = CV_ELEM_SIZE(type);
        પૂર્ણાંક dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        પૂર્ણાંક j, k, di = 0, dj = 0, dk = 0;
        પૂર્ણાંક dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        પૂર્ણાંક work_elem_size = elem_size << (CV_MAT_DEPTH(type) == CV_32F ? 1 : 0);

        જો( !is_a_t )
            a_step0 = A.step, a_step1 = elem_size;
        નહીંતર
            a_step0 = elem_size, a_step1 = A.step;

        જો( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        નહીંતર
            b_step0 = elem_size, b_step1 = b_step;

        જો( C.empty() )
        {
            c_step0 = c_step1 = 0;
            flags &= ~GEMM_3_T;
        }
        નહીંતર જો( !(flags & GEMM_3_T) )
            c_step0 = C.step, c_step1 = elem_size;
        નહીંતર
            c_step0 = elem_size, c_step1 = C.step;

        dm0 = std::min( block_lin_size, d_size.height );
        dn0 = std::min( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = std::min( dk0_1, dk0_2 );
        dk0 = std::min( dk0, len );
        જો( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        જો( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        જો( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~GEMM_1_T;
        }

        buf.allocate(d_buf_size + b_buf_size + a_buf_size);
        d_buf = buf.data();
        b_buf = d_buf + d_buf_size;

        જો( is_a_t )
            a_buf = b_buf + b_buf_size;

        માટે( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            જો( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            માટે( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->ptr() + i*matD->step + j*elem_size;
                અચલ uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                જો( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                જો( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                માટે( k = 0; k < len; k += dk )
                {
                    અચલ uchar* _a = A.ptr() + i*a_step0 + k*a_step1;
                    size_t _a_step = A.step;
                    અચલ uchar* _b = B.ptr() + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    Size a_bl_size;

                    dk = dk0;
                    જો( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    જો( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    નહીંતર
                        a_bl_size.width = di, a_bl_size.height = dk;

                    જો( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A.step, a_buf, _a_step, a_bl_size, elem_size );
                        std::swap( a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    જો( dj < d_size.width )
                    {
                        Size b_size;
                        જો( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        નહીંતર
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    જો( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, Size(dj,di), flags );
                    નહીંતર
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, Size(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                જો( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->ptr(i) + j*elem_size,
                               matD->step, Size(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>અંતઃરેખ સ્થિર શૂન્ય
callGemmImpl(અચલ fptype *src1, size_t src1_step, અચલ fptype *src2, size_t src2_step, fptype alpha,
          અચલ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags, પૂર્ણાંક type)
{
    CV_StaticAssert(GEMM_1_T == CV_HAL_GEMM_1_T, "Incompatible GEMM_1_T flag in HAL");
    CV_StaticAssert(GEMM_2_T == CV_HAL_GEMM_2_T, "Incompatible GEMM_2_T flag in HAL");
    CV_StaticAssert(GEMM_3_T == CV_HAL_GEMM_3_T, "Incompatible GEMM_3_T flag in HAL");

    પૂર્ણાંક b_m, b_n, c_m, c_n, m_d;

    જો(flags & GEMM_2_T)
    {
        b_m = n_d;
        જો(flags & GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        નહીંતર
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    નહીંતર
    {
        b_n = n_d;
        જો(flags & GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        નહીંતર
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    જો(flags & GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    નહીંતર
    {
        c_m = m_d;
        c_n = n_d;
    }

    Mat A, B, C;
    જો(src1 != ખાલી)
        A = Mat(m_a, n_a, type, (શૂન્ય*)src1, src1_step);
    જો(src2 != ખાલી)
        B = Mat(b_m, b_n, type, (શૂન્ય*)src2, src2_step);
    જો(src3 != ખાલી && beta != 0.0)
        C = Mat(c_m, c_n, type, (શૂન્ય*)src3, src3_step);
    Mat D(m_d, n_d, type, (શૂન્ય*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}

શૂન્ય gemm32f(અચલ દશાંશ* src1, size_t src1_step, અચલ દશાંશ* src2, size_t src2_step,
             દશાંશ alpha, અચલ દશાંશ* src3, size_t src3_step, દશાંશ beta, દશાંશ* dst, size_t dst_step,
             પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32F);
}

શૂન્ય gemm64f(અચલ દ્વિગુણ* src1, size_t src1_step, અચલ દ્વિગુણ* src2, size_t src2_step,
             દ્વિગુણ alpha, અચલ દ્વિગુણ* src3, size_t src3_step, દ્વિગુણ beta, દ્વિગુણ* dst, size_t dst_step,
             પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64F);
}

શૂન્ય gemm32fc(અચલ દશાંશ* src1, size_t src1_step, અચલ દશાંશ* src2, size_t src2_step,
              દશાંશ alpha, અચલ દશાંશ* src3, size_t src3_step, દશાંશ beta, દશાંશ* dst, size_t dst_step,
              પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_32FC2);
}

શૂન્ય gemm64fc(અચલ દ્વિગુણ* src1, size_t src1_step, અચલ દ્વિગુણ* src2, size_t src2_step,
              દ્વિગુણ alpha, અચલ દ્વિગુણ* src3, size_t src3_step, દ્વિગુણ beta, દ્વિગુણ* dst, size_t dst_step,
              પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CV_64FC2);
}

#endif // !defined(CV_GEMM_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

template<typename T, typename WT> સ્થિર શૂન્ય
transform_( અચલ T* src, T* dst, અચલ WT* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn )
{
    પૂર્ણાંક x;

    જો( scn == 2 && dcn == 2 )
    {
        માટે( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = saturate_cast<T>(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    નહીંતર જો( scn == 3 && dcn == 3 )
    {
        માટે( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = saturate_cast<T>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = saturate_cast<T>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = saturate_cast<T>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    નહીંતર જો( scn == 3 && dcn == 1 )
    {
        માટે( x = 0; x < len; x++, src += 3 )
            dst[x] = saturate_cast<T>(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    નહીંતર જો( scn == 4 && dcn == 4 )
    {
        માટે( x = 0; x < len*4; x += 4 )
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
    નહીંતર
    {
        માટે( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            અચલ WT* _m = m;
            પૂર્ણાંક j, k;
            માટે( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                માટે( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = saturate_cast<T>(s);
            }
        }
    }
}

સ્થિર શૂન્ય
transform_8u( અચલ uchar* src, uchar* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn )
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક BITS = 10, SCALE = 1 << BITS;
    અચલ દશાંશ MAX_M = (દશાંશ)(1 << (15 - BITS));

    જો( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        અચલ પૂર્ણાંક nChannels = 3;

        સંઘ {
            short s[6];
            પૂર્ણાંક p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        પૂર્ણાંક m32[] = {saturate_cast<પૂર્ણાંક>(m[ 2] * SCALE), saturate_cast<પૂર્ણાંક>(m[ 3] * SCALE),
                     saturate_cast<પૂર્ણાંક>(m[ 6] * SCALE), saturate_cast<પૂર્ણાંક>(m[ 7] * SCALE),
                     saturate_cast<પૂર્ણાંક>(m[10] * SCALE), saturate_cast<પૂર્ણાંક>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        પૂર્ણાંક x = 0;
        માટે (; x <= (len - VTraits<v_uint8>::vlanes()) * nChannels; x += VTraits<v_uint8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<પૂર્ણાંક>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<પૂર્ણાંક>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<પૂર્ણાંક>((m[11] + 0.5f)*SCALE);
        માટે( ; x < len * nChannels; x += nChannels )
        {
            પૂર્ણાંક v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            uchar t1 = saturate_cast<uchar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            uchar t2 = saturate_cast<uchar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
transform_16u( અચલ ushort* src, ushort* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn )
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો( scn == 3 && dcn == 3 )
    {
        પૂર્ણાંક x = 0;
#જો CV_SIMD_WIDTH > 16
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
        માટે (; x <= (len - VTraits<v_uint16>::vlanes())*3; x +=  VTraits<v_uint16>::vlanes()*3)
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
#જો CV_SIMD128
        v_float32x4 _m0l(m[0], m[4], m[ 8], 0.f);
        v_float32x4 _m1l(m[1], m[5], m[ 9], 0.f);
        v_float32x4 _m2l(m[2], m[6], m[10], 0.f);
        v_float32x4 _m3l(m[3] - 32768.f, m[7] - 32768.f, m[11] - 32768.f, 0.f);
        v_float32x4 _m0h = v_rotate_ડાબું<1>(_m0l);
        v_float32x4 _m1h = v_rotate_ડાબું<1>(_m1l);
        v_float32x4 _m2h = v_rotate_ડાબું<1>(_m2l);
        v_float32x4 _m3h = v_rotate_ડાબું<1>(_m3l);
        v_int16x8 _delta(0, -32768, -32768, -32768, -32768, -32768, -32768, 0);
        માટે( ; x <= len*3 - VTraits<v_uint16x8>::vlanes(); x += 3*VTraits<v_uint16x8>::vlanes()/4 )
            v_store(dst + x, v_rotate_જમણું<1>(v_reinterpret_as_u16(v_add_wrap(v_pack(
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x    ))), _m0h, _m1h, _m2h, _m3h)),
                             v_round(v_matmuladd(v_cvt_f32(v_reinterpret_as_s32(v_load_expand(src + x + 3))), _m0l, _m1l, _m2l, _m3l))), _delta))));
#endif //CV_SIMD128
        માટે( ; x < len * 3; x += 3 )
        {
            દશાંશ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            ushort t0 = saturate_cast<ushort>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            ushort t1 = saturate_cast<ushort>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            ushort t2 = saturate_cast<ushort>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
transform_32f( અચલ દશાંશ* src, દશાંશ* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn )
{
// Disabled for RISC-V Vector (scalable), because of:
// 1. Both gcc and clang can autovectorize this, with better performance than using Universal intrinsic.
#જો (CV_SIMD || CV_SIMD_SCALABLE) && !defined(__aarch64__) && !defined(_M_ARM64) && !defined(_M_ARM64EC) && !(CV_TRY_RVV && CV_RVV)
    પૂર્ણાંક x = 0;
    જો( scn == 3 && dcn == 3 )
    {
        પૂર્ણાંક idx[VTraits<v_float32>::max_nlanes/2];
        માટે( પૂર્ણાંક i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
        {
            idx[i] = 3*i;
            idx[i + VTraits<v_float32>::vlanes()/4] = 0;
        }
        દશાંશ _m[] = { m[0], m[4], m[ 8], 0.f,
                       m[1], m[5], m[ 9], 0.f,
                       m[2], m[6], m[10], 0.f,
                       m[3], m[7], m[11], 0.f };
        v_float32 m0 = vx_lut_quads(_m     , idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m1 = vx_lut_quads(_m +  4, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m2 = vx_lut_quads(_m +  8, idx + VTraits<v_float32>::vlanes()/4);
        v_float32 m3 = vx_lut_quads(_m + 12, idx + VTraits<v_float32>::vlanes()/4);
        માટે( ; x <= len*3 - VTraits<v_float32>::vlanes(); x += 3*VTraits<v_float32>::vlanes()/4 )
            v_store(dst + x, v_pack_triplets(v_matmuladd(vx_lut_quads(src + x, idx), m0, m1, m2, m3)));
        માટે( ; x < len*3; x += 3 )
        {
            દશાંશ v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            દશાંશ t0 = saturate_cast<દશાંશ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]);
            દશાંશ t1 = saturate_cast<દશાંશ>(m[4]*v0 + m[5]*v1 + m[ 6]*v2 + m[ 7]);
            દશાંશ t2 = saturate_cast<દશાંશ>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        પાછા;
    }

    જો( scn == 4 && dcn == 4 )
    {
#જો CV_SIMD_WIDTH > 16
        પૂર્ણાંક idx[VTraits<v_float32>::max_nlanes/4];
        માટે( પૂર્ણાંક i = 0; i < VTraits<v_float32>::vlanes()/4; i++ )
            idx[i] = 0;
        દશાંશ _m[] = { m[4], m[9], m[14], m[19] };
        v_float32 m0 = vx_lut_quads(m   , idx);
        v_float32 m1 = vx_lut_quads(m+ 5, idx);
        v_float32 m2 = vx_lut_quads(m+10, idx);
        v_float32 m3 = vx_lut_quads(m+15, idx);
        v_float32 m4 = vx_lut_quads(_m, idx);
        માટે( ; x <= len*4 - VTraits<v_float32>::vlanes(); x += VTraits<v_float32>::vlanes() )
        {
            v_float32 v_src = vx_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, m0), v_mul(v_src, m1), v_mul(v_src, m2), v_mul(v_src, m3)), m4));
        }
#endif
#જો CV_SIMD128
        v_float32x4 _m0 = v_load(m     );
        v_float32x4 _m1 = v_load(m +  5);
        v_float32x4 _m2 = v_load(m + 10);
        v_float32x4 _m3 = v_load(m + 15);
        v_float32x4 _m4(m[4], m[9], m[14], m[19]);
        માટે( ; x < len*4; x += VTraits<v_float32x4>::vlanes() )
        {
            v_float32x4 v_src = v_load(src + x);
            v_store(dst + x, v_add(v_reduce_sum4(v_mul(v_src, _m0), v_mul(v_src, _m1), v_mul(v_src, _m2), v_mul(v_src, _m3)), _m4));
        }
#નહીંતર // CV_SIMD_WIDTH >= 16 && !CV_SIMD128
        માટે( ; x < len*4; x += 4 )
        {
            દશાંશ v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            દશાંશ t0 = saturate_cast<દશાંશ>(m[0]*v0 + m[1]*v1 + m[ 2]*v2 + m[ 3]*v3 + m[ 4]);
            દશાંશ t1 = saturate_cast<દશાંશ>(m[5]*v0 + m[6]*v1 + m[ 7]*v2 + m[ 8]*v3 + m[ 9]);
            દશાંશ t2 = saturate_cast<દશાંશ>(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            દશાંશ t3 = saturate_cast<દશાંશ>(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2; dst[x+3] = t3;
        }
#endif
        vx_cleanup();
        પાછા;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


સ્થિર શૂન્ય
transform_8s(અચલ schar* src, schar* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક BITS = 10, SCALE = 1 << BITS;
    અચલ દશાંશ MAX_M = (દશાંશ)(1 << (15 - BITS));

    જો( scn == 3 && dcn == 3 &&
        std::abs(m[0]) < MAX_M && std::abs(m[1]) < MAX_M && std::abs(m[ 2]) < MAX_M*256 && std::abs(m[ 3]) < MAX_M*256 &&
        std::abs(m[4]) < MAX_M && std::abs(m[5]) < MAX_M && std::abs(m[ 6]) < MAX_M*256 && std::abs(m[ 7]) < MAX_M*256 &&
        std::abs(m[8]) < MAX_M && std::abs(m[9]) < MAX_M && std::abs(m[10]) < MAX_M*256 && std::abs(m[11]) < MAX_M*256 )
    {
        અચલ પૂર્ણાંક nChannels = 3;

        સંઘ {
            short s[6];
            પૂર્ણાંક p[3];
        } m16;
        m16.s[0] = saturate_cast<short>(m[0] * SCALE); m16.s[1] = saturate_cast<short>(m[1] * SCALE);
        m16.s[2] = saturate_cast<short>(m[4] * SCALE); m16.s[3] = saturate_cast<short>(m[5] * SCALE);
        m16.s[4] = saturate_cast<short>(m[8] * SCALE); m16.s[5] = saturate_cast<short>(m[9] * SCALE);
        પૂર્ણાંક m32[] = {saturate_cast<પૂર્ણાંક>(m[ 2] * SCALE), saturate_cast<પૂર્ણાંક>(m[ 3] * SCALE),
                     saturate_cast<પૂર્ણાંક>(m[ 6] * SCALE), saturate_cast<પૂર્ણાંક>(m[ 7] * SCALE),
                     saturate_cast<પૂર્ણાંક>(m[10] * SCALE), saturate_cast<પૂર્ણાંક>(m[11] * SCALE)};
        v_int16 m01 = v_reinterpret_as_s16(vx_setall_s32(m16.p[0]));
        v_int32 m2 = vx_setall_s32(m32[0]);
        v_int32 m3 = vx_setall_s32(m32[1]);
        v_int16 m45 = v_reinterpret_as_s16(vx_setall_s32(m16.p[1]));
        v_int32 m6 = vx_setall_s32(m32[2]);
        v_int32 m7 = vx_setall_s32(m32[3]);
        v_int16 m89 = v_reinterpret_as_s16(vx_setall_s32(m16.p[2]));
        v_int32 m10 = vx_setall_s32(m32[4]);
        v_int32 m11 = vx_setall_s32(m32[5]);
        પૂર્ણાંક x = 0;
        માટે (; x <= (len - VTraits<v_int8>::vlanes()) * nChannels; x += VTraits<v_int8>::vlanes() * nChannels)
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
        m32[1] = saturate_cast<પૂર્ણાંક>((m[3] + 0.5f)*SCALE);
        m32[3] = saturate_cast<પૂર્ણાંક>((m[7] + 0.5f)*SCALE);
        m32[5] = saturate_cast<પૂર્ણાંક>((m[11] + 0.5f)*SCALE);
        માટે( ; x < len * nChannels; x += nChannels )
        {
            પૂર્ણાંક v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            schar t0 = saturate_cast<schar>((m16.s[0] * v0 + m16.s[1] * v1 + m32[0] * v2 + m32[1]) >> BITS);
            schar t1 = saturate_cast<schar>((m16.s[2] * v0 + m16.s[3] * v1 + m32[2] * v2 + m32[3]) >> BITS);
            schar t2 = saturate_cast<schar>((m16.s[4] * v0 + m16.s[5] * v1 + m32[4] * v2 + m32[5]) >> BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
transform_16s(અચલ short* src, short* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

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
        માટે (; x <= (len - VTraits<v_int16>::vlanes())*3; x +=  VTraits<v_int16>::vlanes()*3)
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
        માટે( ; x < len * 3; x += 3 )
        {
            દશાંશ v0 = src[x], v1 = src[x + 1], v2 = src[x + 2];
            short t0 = saturate_cast<short>(m[0] * v0 + m[1] * v1 + m[ 2] * v2 + m[ 3]);
            short t1 = saturate_cast<short>(m[4] * v0 + m[5] * v1 + m[ 6] * v2 + m[ 7]);
            short t2 = saturate_cast<short>(m[8] * v0 + m[9] * v1 + m[10] * v2 + m[11]);
            dst[x] = t0; dst[x + 1] = t1; dst[x + 2] = t2;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
transform_32s(અચલ પૂર્ણાંક* src, પૂર્ણાંક* dst, અચલ દ્વિગુણ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    જો( scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

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
        માટે (; x <= (len - VTraits<v_int32>::vlanes()) * 3; x += VTraits<v_int32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ઉપલું(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ઉપલું(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ઉપલું(r), rl = v_cvt_f64(r);

            v_int32 db, dg, dr;
            db = v_round(v_fma(bl, m0, v_fma(gl, m1, v_fma(rl,  m2,  m3))),
                         v_fma(bh, m0, v_fma(gh, m1, v_fma(rh,  m2,  m3))));
            dg = v_round(v_fma(bl, m4, v_fma(gl, m5, v_fma(rl,  m6,  m7))),
                         v_fma(bh, m4, v_fma(gh, m5, v_fma(rh,  m6,  m7))));
            dr = v_round(v_fma(bl, m8, v_fma(gl, m9, v_fma(rl, m10, m11))),
                         v_fma(bh, m8, v_fma(gh, m9, v_fma(rh, m10, m11))));

            v_store_interleave(dst + x, db, dg, dr);
        }
        માટે (; x < len * 3; x += 3) {
            દ્વિગુણ b = src[x], g = src[x + 1], r = src[x + 2];
            પૂર્ણાંક db = saturate_cast<પૂર્ણાંક>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            પૂર્ણાંક dg = saturate_cast<પૂર્ણાંક>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            પૂર્ણાંક dr = saturate_cast<પૂર્ણાંક>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
transform_64f(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, અચલ દ્વિગુણ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    જો( scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

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
        માટે (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db, dg, dr;
            db = v_fma(b, m0, v_fma(g, m1, v_fma(r,  m2,  m3)));
            dg = v_fma(b, m4, v_fma(g, m5, v_fma(r,  m6,  m7)));
            dr = v_fma(b, m8, v_fma(g, m9, v_fma(r, m10, m11)));

            v_store_interleave(dst + x, db, dg, dr);
        }
        માટે (; x < len * 3; x += 3) {
            દ્વિગુણ b = src[x], g = src[x + 1], r = src[x + 2];
            દ્વિગુણ db = saturate_cast<દ્વિગુણ>(m[0] * b + m[1] * g + m[ 2] * r + m[ 3]);
            દ્વિગુણ dg = saturate_cast<દ્વિગુણ>(m[4] * b + m[5] * g + m[ 6] * r + m[ 7]);
            દ્વિગુણ dr = saturate_cast<દ્વિગુણ>(m[8] * b + m[9] * g + m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> સ્થિર શૂન્ય
diagtransform_( અચલ T* src, T* dst, અચલ WT* m, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક )
{
    પૂર્ણાંક x;

    જો( cn == 2 )
    {
        માટે( x = 0; x < len*2; x += 2 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[2]);
            T t1 = saturate_cast<T>(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    નહીંતર જો( cn == 3 )
    {
        માટે( x = 0; x < len*3; x += 3 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[3]);
            T t1 = saturate_cast<T>(m[5]*src[x+1] + m[7]);
            T t2 = saturate_cast<T>(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    નહીંતર જો( cn == 4 )
    {
        માટે( x = 0; x < len*4; x += 4 )
        {
            T t0 = saturate_cast<T>(m[0]*src[x] + m[4]);
            T t1 = saturate_cast<T>(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = saturate_cast<T>(m[12]*src[x+2] + m[14]);
            t1 = saturate_cast<T>(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    નહીંતર
    {
        માટે( x = 0; x < len; x++, src += cn, dst += cn )
        {
            અચલ WT* _m = m;
            માટે( પૂર્ણાંક j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = saturate_cast<T>(src[j]*_m[j] + _m[cn]);
        }
    }
}

સ્થિર શૂન્ય
diagtransform_8u(અચલ uchar* src, uchar* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        માટે (; x <= (len - VTraits<v_uint8>::vlanes()) * 3; x += VTraits<v_uint8>::vlanes() * 3) {
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
        માટે (; x < len * 3; x += 3) {
            પૂર્ણાંક b = src[x], g = src[x + 1], r = src[x + 2];
            uchar db = saturate_cast<uchar>(m[ 0] * b + m[ 3]);
            uchar dg = saturate_cast<uchar>(m[ 5] * g + m[ 7]);
            uchar dr = saturate_cast<uchar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
diagtransform_8s(અચલ schar* src, schar* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
    #જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        માટે (; x <= (len - VTraits<v_int8>::vlanes()) * 3; x += VTraits<v_int8>::vlanes() * 3) {
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
        માટે (; x < len * 3; x += 3) {
            પૂર્ણાંક b = src[x], g = src[x + 1], r = src[x + 2];
            schar db = saturate_cast<schar>(m[ 0] * b + m[ 3]);
            schar dg = saturate_cast<schar>(m[ 5] * g + m[ 7]);
            schar dr = saturate_cast<schar>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
diagtransform_16u(અચલ ushort* src, ushort* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        માટે (; x <= (len - VTraits<v_uint16>::vlanes()) * 3; x += VTraits<v_uint16>::vlanes() * 3) {
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
        માટે (; x < len * 3; x += 3) {
            પૂર્ણાંક b = src[x], g = src[x + 1], r = src[x + 2];
            ushort db = saturate_cast<ushort>(m[ 0] * b + m[ 3]);
            ushort dg = saturate_cast<ushort>(m[ 5] * g + m[ 7]);
            ushort dr = saturate_cast<ushort>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
diagtransform_16s(અચલ short* src, short* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        માટે (; x <= (len - VTraits<v_int16>::vlanes()) * 3; x += VTraits<v_int16>::vlanes() * 3) {
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
        માટે (; x < len * 3; x += 3) {
            પૂર્ણાંક b = src[x], g = src[x + 1], r = src[x + 2];
            short db = saturate_cast<short>(m[ 0] * b + m[ 3]);
            short dg = saturate_cast<short>(m[ 5] * g + m[ 7]);
            short dr = saturate_cast<short>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
diagtransform_32s(અચલ પૂર્ણાંક* src, પૂર્ણાંક* dst, અચલ દ્વિગુણ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    જો (scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        માટે (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_int32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);
            v_float64 bh = v_cvt_f64_ઉપલું(b), bl = v_cvt_f64(b);
            v_float64 gh = v_cvt_f64_ઉપલું(g), gl = v_cvt_f64(g);
            v_float64 rh = v_cvt_f64_ઉપલું(r), rl = v_cvt_f64(r);

            v_int32 db = v_round(v_fma( m0, bl,  m3),
                                 v_fma( m0, bh,  m3));
            v_int32 dg = v_round(v_fma( m5, gl,  m7),
                                 v_fma( m5, gh,  m7));
            v_int32 dr = v_round(v_fma(m10, rl, m11),
                                 v_fma(m10, rh, m11));

            v_store_interleave(dst + x, db, dg, dr);
        }
        માટે (; x < len * 3; x += 3) {
            પૂર્ણાંક b = src[x], g = src[x + 1], r = src[x + 2];
            પૂર્ણાંક db = saturate_cast<પૂર્ણાંક>(m[ 0] * b + m[ 3]);
            પૂર્ણાંક dg = saturate_cast<પૂર્ણાંક>(m[ 5] * g + m[ 7]);
            પૂર્ણાંક dr = saturate_cast<પૂર્ણાંક>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
diagtransform_32f(અચલ દશાંશ* src, દશાંશ* dst, અચલ દશાંશ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

        v_float32 m0  = vx_setall_f32(m[ 0]);
        v_float32 m3  = vx_setall_f32(m[ 3]);
        v_float32 m5  = vx_setall_f32(m[ 5]);
        v_float32 m7  = vx_setall_f32(m[ 7]);
        v_float32 m10 = vx_setall_f32(m[10]);
        v_float32 m11 = vx_setall_f32(m[11]);
        માટે (; x <= (len - VTraits<v_float32>::vlanes()) * 3; x += VTraits<v_float32>::vlanes() * 3) {
            v_float32 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float32 db = v_fma( m0, b,  m3);
            v_float32 dg = v_fma( m5, g,  m7);
            v_float32 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        માટે (; x < len * 3; x += 3) {
            દશાંશ b = src[x], g = src[x + 1], r = src[x + 2];
            દશાંશ db = saturate_cast<દશાંશ>(m[ 0] * b + m[ 3]);
            દશાંશ dg = saturate_cast<દશાંશ>(m[ 5] * g + m[ 7]);
            દશાંશ dr = saturate_cast<દશાંશ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}

સ્થિર શૂન્ય
diagtransform_64f(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, અચલ દ્વિગુણ* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    જો (scn == 3 && dcn == 3) {
        પૂર્ણાંક x = 0;

        v_float64 m0  = vx_setall_f64(m[ 0]);
        v_float64 m3  = vx_setall_f64(m[ 3]);
        v_float64 m5  = vx_setall_f64(m[ 5]);
        v_float64 m7  = vx_setall_f64(m[ 7]);
        v_float64 m10 = vx_setall_f64(m[10]);
        v_float64 m11 = vx_setall_f64(m[11]);
        માટે (; x <= (len - VTraits<v_float64>::vlanes()) * 3; x += VTraits<v_float64>::vlanes() * 3) {
            v_float64 b, g, r;
            v_load_deinterleave(src + x, b, g, r);

            v_float64 db = v_fma( m0, b,  m3);
            v_float64 dg = v_fma( m5, g,  m7);
            v_float64 dr = v_fma(m10, r, m11);

            v_store_interleave(dst + x, db, dg, dr);
        }
        માટે (; x < len * 3; x += 3) {
            દ્વિગુણ b = src[x], g = src[x + 1], r = src[x + 2];
            દ્વિગુણ db = saturate_cast<દ્વિગુણ>(m[ 0] * b + m[ 3]);
            દ્વિગુણ dg = saturate_cast<દ્વિગુણ>(m[ 5] * g + m[ 7]);
            દ્વિગુણ dr = saturate_cast<દ્વિગુણ>(m[10] * r + m[11]);
            dst[x] = db; dst[x + 1] = dg; dst[x + 2] = dr;
        }
        vx_cleanup();
        પાછા;
    }
#endif

    diagtransform_(src, dst, m, len, scn, dcn);
}


template<typename T, typename WT, શૂન્ય (*fn)(અચલ T*, T*, અચલ WT*, પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક)>
સ્થિર શૂન્ય transformWrap(અચલ શૂન્ય* src, શૂન્ય* dst, અચલ શૂન્ય* m, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn)
{
    fn((અચલ T*)src, (T*)dst, (અચલ WT*)m, len, scn, dcn);
}

TransformFunc getTransformFunc(પૂર્ણાંક depth)
{
    સ્થિર TransformFunc transformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, દશાંશ, transform_8u>, transformWrap<schar, દશાંશ, transform_8s>, transformWrap<ushort, દશાંશ, transform_16u>,
        transformWrap<short, દશાંશ, transform_16s>, transformWrap<પૂર્ણાંક, દ્વિગુણ, transform_32s>, transformWrap<દશાંશ, દશાંશ, transform_32f>,
        transformWrap<દ્વિગુણ, દ્વિગુણ, transform_64f>, 0
    };

    પાછા transformTab[depth];
}

TransformFunc getDiagTransformFunc(પૂર્ણાંક depth)
{
    સ્થિર TransformFunc diagTransformTab[CV_DEPTH_MAX] =
    {
        transformWrap<uchar, દશાંશ, diagtransform_8u>, transformWrap<schar, દશાંશ, diagtransform_8s>, transformWrap<ushort, દશાંશ, diagtransform_16u>,
        transformWrap<short, દશાંશ, diagtransform_16s>, transformWrap<પૂર્ણાંક, દ્વિગુણ, diagtransform_32s>, transformWrap<દશાંશ, દશાંશ, diagtransform_32f>,
        transformWrap<દ્વિગુણ, દ્વિગુણ, diagtransform_64f>, 0
    };

    પાછા diagTransformTab[depth];
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

template<typename T> સ્થિર શૂન્ય
perspectiveTransform_( અચલ શૂન્ય* _src, શૂન્ય* _dst, અચલ શૂન્ય* _m_ptr, પૂર્ણાંક len, પૂર્ણાંક scn, પૂર્ણાંક dcn )
{
    અચલ T* src = (અચલ T*)_src;
    T* dst = (T*)_dst;
    અચલ દ્વિગુણ* m = (અચલ દ્વિગુણ*)_m_ptr;
    અચલ દ્વિગુણ eps = FLT_EPSILON;
    પૂર્ણાંક i;

    જો( scn == 2 && dcn == 2 )
    {
        માટે( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            દ્વિગુણ w = x*m[6] + y*m[7] + m[8];

            જો( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            નહીંતર
                dst[i] = dst[i+1] = (T)0;
        }
    }
    નહીંતર જો( scn == 3 && dcn == 3 )
    {
        માટે( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            દ્વિગુણ w = x*m[12] + y*m[13] + z*m[14] + m[15];

            જો( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            નહીંતર
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    નહીંતર જો( scn == 3 && dcn == 2 )
    {
        માટે( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            દ્વિગુણ w = x*m[8] + y*m[9] + z*m[10] + m[11];

            જો( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            નહીંતર
                dst[0] = dst[1] = (T)0;
        }
    }
    નહીંતર
    {
        માટે( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            અચલ દ્વિગુણ* _m = m + dcn*(scn + 1);
            દ્વિગુણ w = _m[scn];
            પૂર્ણાંક j, k;
            માટે( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            જો( fabs(w) > eps )
            {
                _m = m;
                માટે( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    દ્વિગુણ s = _m[scn];
                    માટે( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            નહીંતર
                માટે( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}

TransformFunc getPerspectiveTransform(પૂર્ણાંક depth)
{
    જો (depth == CV_32F)
        પાછા perspectiveTransform_<દશાંશ>;
    જો (depth == CV_64F)
        પાછા perspectiveTransform_<દ્વિગુણ>;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

સ્થિર શૂન્ય scaleAdd_32f(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, શૂન્ય* _dst,
                         પૂર્ણાંક len, અચલ શૂન્ય* _alpha)
{
    અચલ દશાંશ* src1 = (અચલ દશાંશ*)_src1;
    અચલ દશાંશ* src2 = (અચલ દશાંશ*)_src2;
    દશાંશ* dst = (દશાંશ*)_dst;
    દશાંશ alpha = *(અચલ દશાંશ*)_alpha;
    પૂર્ણાંક i = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    v_float32 v_alpha = vx_setall_f32(alpha);
    અચલ પૂર્ણાંક cWidth = VTraits<v_float32>::vlanes();
    માટે (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), v_alpha, vx_load(src2 + i)));
    vx_cleanup();
#endif
    માટે (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}


સ્થિર શૂન્ય scaleAdd_64f(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, શૂન્ય* _dst,
                         પૂર્ણાંક len, અચલ શૂન્ય* _alpha)
{
    અચલ દ્વિગુણ* src1 = (અચલ દ્વિગુણ*)_src1;
    અચલ દ્વિગુણ* src2 = (અચલ દ્વિગુણ*)_src2;
    દ્વિગુણ* dst = (દ્વિગુણ*)_dst;
    દ્વિગુણ alpha = *(અચલ દ્વિગુણ*)_alpha;
    પૂર્ણાંક i = 0;
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    v_float64 a2 = vx_setall_f64(alpha);
    અચલ પૂર્ણાંક cWidth = VTraits<v_float64>::vlanes();
    માટે (; i <= len - cWidth; i += cWidth)
        v_store(dst + i, v_muladd(vx_load(src1 + i), a2, vx_load(src2 + i)));
    vx_cleanup();
#endif
    માટે (; i < len; i++)
        dst[i] = src1[i] * alpha + src2[i];
}

ScaleAddFunc getScaleAddFunc(પૂર્ણાંક depth)
{
    જો (depth == CV_32F)
        પાછા scaleAdd_32f;
    જો (depth == CV_64F)
        પાછા scaleAdd_64f;
    CV_Assert(0 && "Not supported");
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

template<typename T> સ્થિર અંતઃરેખ
દ્વિગુણ MahalanobisImpl(અચલ Mat& v1, અચલ Mat& v2, અચલ Mat& icovar, દ્વિગુણ *diff_buffer /*[len]*/, પૂર્ણાંક len /*=v1.total()*/)
{
    CV_INSTRUMENT_REGION();

    Size sz = v1.size();
    દ્વિગુણ result = 0;

    sz.width *= v1.channels();
    જો (v1.isContinuous() && v2.isContinuous())
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    {
        અચલ T* src1 = v1.ptr<T>();
        અચલ T* src2 = v2.ptr<T>();
        size_t step1 = v1.step/કદ(src1[0]);
        size_t step2 = v2.step/કદ(src2[0]);
        દ્વિગુણ* diff = diff_buffer;
        અચલ T* mat = icovar.ptr<T>();
        size_t matstep = icovar.step/કદ(mat[0]);

        માટે (; sz.height--; src1 += step1, src2 += step2, diff += sz.width)
        {
            માટે (પૂર્ણાંક i = 0; i < sz.width; i++)
                diff[i] = src1[i] - src2[i];
        }

        diff = diff_buffer;
        માટે (પૂર્ણાંક i = 0; i < len; i++, mat += matstep)
        {
            દ્વિગુણ row_sum = 0;
            પૂર્ણાંક j = 0;
#જો CV_ENABLE_UNROLLED
            માટે(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
#endif
            માટે (; j < len; j++)
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    પાછા result;
}

MahalanobisImplFunc getMahalanobisImplFunc(પૂર્ણાંક depth)
{
    જો (depth == CV_32F)
        પાછા (MahalanobisImplFunc)MahalanobisImpl<દશાંશ>;
    જો (depth == CV_64F)
        પાછા (MahalanobisImplFunc)MahalanobisImpl<દ્વિગુણ>;
    CV_Assert(0 && "Not supported");
}


#જો !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)
/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

template<typename sT, typename dT> સ્થિર શૂન્ય
MulTransposedR(અચલ Mat& srcmat, અચલ Mat& dstmat, અચલ Mat& deltamat, દ્વિગુણ scale)
{
    પૂર્ણાંક i, j, k;
    અચલ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    અચલ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/કદ(src[0]);
    size_t dststep = dstmat.step/કદ(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/કદ(delta[0]) : 0;
    પૂર્ણાંક delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    પૂર્ણાંક buf_size = size.height*કદ(dT);
    AutoBuffer<uchar> buf;

    જો( delta && delta_cols < size.width )
    {
        CV_Assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf.allocate(buf_size);
    col_buf = (dT*)buf.data();

    જો( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        માટે( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

#જો CV_SIMD128_64F
    v_float64x2 v_scale = v_setall_f64(scale);
#endif

    જો( !delta )
        માટે( i = 0; i < size.width; i++, tdst += dststep )
        {
            માટે( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            માટે( j = i; j <= size.width - 4; j += 4 )
            {
#જો CV_SIMD128_64F
                જો (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    અચલ દ્વિગુણ *tsrc = (દ્વિગુણ*)(src + j);

                    માટે( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        v_float64x2 a = v_setall_f64((દ્વિગુણ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_load(tsrc + 0)));
                        s1 = v_add(s1, v_mul(a, v_load(tsrc + 2)));
                    }

                    v_store((દ્વિગુણ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((દ્વિગુણ*)(tdst+j+2), v_mul(s1, v_scale));
                } નહીંતર
#endif
                {
                    દ્વિગુણ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    અચલ sT *tsrc = src + j;

                    માટે( k = 0; k < size.height; k++, tsrc += srcstep )
                    {
                        દ્વિગુણ a = col_buf[k];
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

            માટે( ; j < size.width; j++ )
            {
                દ્વિગુણ s0 = 0;
                અચલ sT *tsrc = src + j;

                માટે( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (દ્વિગુણ)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    નહીંતર
        માટે( i = 0; i < size.width; i++, tdst += dststep )
        {
            જો( !delta_buf )
                માટે( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            નહીંતર
                માટે( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            માટે( j = i; j <= size.width - 4; j += 4 )
            {
#જો CV_SIMD128_64F
                જો (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    v_float64x2 s0 = v_setzero_f64(), s1 = v_setzero_f64();
                    અચલ દ્વિગુણ *tsrc = (દ્વિગુણ*)(src + j);
                    અચલ દ્વિગુણ *d = (દ્વિગુણ*)(delta_buf ? delta_buf : delta + j);

                    માટે( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        v_float64x2 a = v_setall_f64((દ્વિગુણ)col_buf[k]);
                        s0 = v_add(s0, v_mul(a, v_sub(v_load(tsrc + 0), v_load(d + 0))));
                        s1 = v_add(s1, v_mul(a, v_sub(v_load(tsrc + 2), v_load(d + 2))));
                    }

                    v_store((દ્વિગુણ*)(tdst+j), v_mul(s0, v_scale));
                    v_store((દ્વિગુણ*)(tdst+j+2), v_mul(s1, v_scale));
                }
                નહીંતર
#endif

                {
                    દ્વિગુણ s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    અચલ sT *tsrc = src + j;
                    અચલ dT *d = delta_buf ? delta_buf : delta + j;

                    માટે( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    {
                        દ્વિગુણ a = col_buf[k];
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

            માટે( ; j < size.width; j++ )
            {
                દ્વિગુણ s0 = 0;
                અચલ sT *tsrc = src + j;
                અચલ dT *d = delta_buf ? delta_buf : delta + j;

                માટે( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (દ્વિગુણ)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
}


template<typename sT, typename dT> સ્થિર શૂન્ય
MulTransposedL(અચલ Mat& srcmat, અચલ Mat& dstmat, અચલ Mat& deltamat, દ્વિગુણ scale)
{
    પૂર્ણાંક i, j, k;
    અચલ sT* src = srcmat.ptr<sT>();
    dT* dst = (dT*)dstmat.ptr<dT>();
    અચલ dT* delta = deltamat.ptr<dT>();
    size_t srcstep = srcmat.step/કદ(src[0]);
    size_t dststep = dstmat.step/કદ(dst[0]);
    size_t deltastep = deltamat.rows > 1 ? deltamat.step/કદ(delta[0]) : 0;
    પૂર્ણાંક delta_cols = deltamat.cols;
    Size size = srcmat.size();
    dT* tdst = dst;

    જો( !delta )
        માટે( i = 0; i < size.height; i++, tdst += dststep )
            માટે( j = i; j < size.height; j++ )
            {
                દ્વિગુણ s = 0;
                અચલ sT *tsrc1 = src + i*srcstep;
                અચલ sT *tsrc2 = src + j*srcstep;
#જો CV_SIMD128_64F
                જો (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    અચલ દ્વિગુણ *v_tsrc1 = (દ્વિગુણ *)(tsrc1);
                    અચલ દ્વિગુણ *v_tsrc2 = (દ્વિગુણ *)(tsrc2);
                    v_float64x2 v_s = v_setzero_f64();

                    માટે( k = 0; k <= size.width - 4; k += 4 )
                        v_s = v_add(v_s, v_add(v_mul(v_load(v_tsrc1 + k), v_load(v_tsrc2 + k)), v_mul(v_load(v_tsrc1 + k + 2), v_load(v_tsrc2 + k + 2))));
                    s += v_reduce_sum(v_s);
                }
                નહીંતર
#endif
                {
                    માટે( k = 0; k <= size.width - 4; k += 4 )
                        s += (દ્વિગુણ)tsrc1[k]*tsrc2[k] + (દ્વિગુણ)tsrc1[k+1]*tsrc2[k+1] +
                             (દ્વિગુણ)tsrc1[k+2]*tsrc2[k+2] + (દ્વિગુણ)tsrc1[k+3]*tsrc2[k+3];
                }
                માટે( ; k < size.width; k++ )
                    s += (દ્વિગુણ)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    નહીંતર
    {
        dT delta_buf[4];
        પૂર્ણાંક delta_shift = delta_cols == size.width ? 4 : 0;
        AutoBuffer<uchar> buf(size.width*કદ(dT));
        dT* row_buf = (dT*)buf.data();

        માટે( i = 0; i < size.height; i++, tdst += dststep )
        {
            અચલ sT *tsrc1 = src + i*srcstep;
            અચલ dT *tdelta1 = delta + i*deltastep;

            જો( delta_cols < size.width )
                માટે( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            નહીંતર
                માટે( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            માટે( j = i; j < size.height; j++ )
            {
                દ્વિગુણ s = 0;
                અચલ sT *tsrc2 = src + j*srcstep;
                અચલ dT *tdelta2 = delta + j*deltastep;
                જો( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
#જો CV_SIMD128_64F
                જો (DataType<sT>::depth == CV_64F && DataType<dT>::depth == CV_64F)
                {
                    અચલ દ્વિગુણ *v_tsrc2 = (દ્વિગુણ *)(tsrc2);
                    અચલ દ્વિગુણ *v_tdelta2 = (દ્વિગુણ *)(tdelta2);
                    અચલ દ્વિગુણ *v_row_buf = (દ્વિગુણ *)(row_buf);
                    v_float64x2 v_s = v_setzero_f64();

                    માટે( k = 0; k <= size.width - 4; k += 4, v_tdelta2 += delta_shift )
                        v_s = v_add(v_s, v_add(v_mul(v_sub(v_load(v_tsrc2 + k), v_load(v_tdelta2)), v_load(v_row_buf + k)), v_mul(v_sub(v_load(v_tsrc2 + k + 2), v_load(v_tdelta2 + 2)), v_load(v_row_buf + k + 2))));
                    s += v_reduce_sum(v_s);

                    tdelta2 = (અચલ dT *)(v_tdelta2);
                }
                નહીંતર
#endif
                {
                    માટે( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                        s += (દ્વિગુણ)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                             (દ્વિગુણ)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                             (દ્વિગુણ)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                             (દ્વિગુણ)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                }
                માટે( ; k < size.width; k++, tdelta2++ )
                    s += (દ્વિગુણ)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

MulTransposedFunc getMulTransposedFunc(પૂર્ણાંક stype, પૂર્ણાંક dtype, તાર્કિક ata)
{
    MulTransposedFunc func = ખાલી;
    જો (stype == CV_8U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<uchar,દશાંશ>
                   : MulTransposedL<uchar,દશાંશ>;
    }
    નહીંતર જો (stype == CV_8U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<uchar,દ્વિગુણ>
                   : MulTransposedL<uchar,દ્વિગુણ>;
    }
    નહીંતર જો(stype == CV_16U && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<ushort,દશાંશ>
                   : MulTransposedL<ushort,દશાંશ>;
    }
    નહીંતર જો(stype == CV_16U && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<ushort,દ્વિગુણ>
                   : MulTransposedL<ushort,દ્વિગુણ>;
    }
    નહીંતર જો(stype == CV_16S && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<short,દશાંશ>
                   : MulTransposedL<short,દશાંશ>;
    }
    નહીંતર જો(stype == CV_16S && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<short,દ્વિગુણ>
                   : MulTransposedL<short,દ્વિગુણ>;
    }
    નહીંતર જો(stype == CV_32F && dtype == CV_32F)
    {
        func = ata ? MulTransposedR<દશાંશ,દશાંશ>
                   : MulTransposedL<દશાંશ,દશાંશ>;
    }
    નહીંતર જો(stype == CV_32F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<દશાંશ,દ્વિગુણ>
                   : MulTransposedL<દશાંશ,દ્વિગુણ>;
    }
    નહીંતર જો(stype == CV_64F && dtype == CV_64F)
    {
        func = ata ? MulTransposedR<દ્વિગુણ,દ્વિગુણ>
                   : MulTransposedL<દ્વિગુણ,દ્વિગુણ>;
    }
    CV_Assert(func && "Not supported");
    પાછા func;
}
#endif // !defined(CV_MULTRANSPOSED_BASELINE_ONLY) || defined(CV_CPU_BASELINE_MODE)


/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

template<typename T> સ્થિર અંતઃરેખ
દ્વિગુણ dotProd_(અચલ T* src1, અચલ T* src2, પૂર્ણાંક len)
{
    પૂર્ણાંક i = 0;
    દ્વિગુણ result = 0;

#જો CV_ENABLE_UNROLLED
    માટે( ; i <= len - 4; i += 4 )
        result += (દ્વિગુણ)src1[i]*src2[i] + (દ્વિગુણ)src1[i+1]*src2[i+1] +
            (દ્વિગુણ)src1[i+2]*src2[i+2] + (દ્વિગુણ)src1[i+3]*src2[i+3];
#endif
    માટે( ; i < len; i++ )
        result += (દ્વિગુણ)src1[i]*src2[i];

    પાછા result;
}


દ્વિગુણ dotProd_8u(અચલ uchar* src1, અચલ uchar* src2, પૂર્ણાંક len)
{
    દ્વિગુણ r = 0;
    પૂર્ણાંક i = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    પૂર્ણાંક len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 15), blockSize;

    જ્યારે (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint32 v_sum = vx_setzero_u32();
        અચલ પૂર્ણાંક cWidth = VTraits<v_uint16>::vlanes();

        પૂર્ણાંક j = 0;
        માટે (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_uint8 v_src1 = vx_load(src1 + j);
            v_uint8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        માટે (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src10 = v_reinterpret_as_s16(vx_load_expand(src1 + j));
            v_int16 v_src20 = v_reinterpret_as_s16(vx_load_expand(src2 + j));
            v_sum = v_add(v_sum, v_reinterpret_as_u32(v_dotprod_fast(v_src10, v_src20)));
        }
        r += (દ્વિગુણ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    પાછા r + dotProd_(src1, src2, len - i);
}


દ્વિગુણ dotProd_8s(અચલ schar* src1, અચલ schar* src2, પૂર્ણાંક len)
{
    દ્વિગુણ r = 0.0;
    પૂર્ણાંક i = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    પૂર્ણાંક len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 14), blockSize;

    જ્યારે (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int32 v_sum = vx_setzero_s32();
        અચલ પૂર્ણાંક cWidth = VTraits<v_int16>::vlanes();

        પૂર્ણાંક j = 0;
        માટે (; j <= blockSize - cWidth * 2; j += cWidth * 2)
        {
            v_int8 v_src1 = vx_load(src1 + j);
            v_int8 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }

        માટે (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load_expand(src1 + j);
            v_int16 v_src2 = vx_load_expand(src2 + j);
            v_sum = v_dotprod_fast(v_src1, v_src2, v_sum);
        }
        r += (દ્વિગુણ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif

    પાછા r + dotProd_(src1, src2, len - i);
}

દ્વિગુણ dotProd_16u(અચલ ushort* src1, અચલ ushort* src2, પૂર્ણાંક len)
{
    દ્વિગુણ r = 0.0;
    પૂર્ણાંક i = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    પૂર્ણાંક len0 = len & -VTraits<v_uint16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    જ્યારે (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_uint64 v_sum = vx_setzero_u64();
        અચલ પૂર્ણાંક cWidth = VTraits<v_uint16>::vlanes();

        પૂર્ણાંક j = 0;
        માટે (; j <= blockSize - cWidth; j += cWidth)
        {
            v_uint16 v_src1 = vx_load(src1 + j);
            v_uint16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (દ્વિગુણ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    પાછા r + dotProd_(src1, src2, len - i);
}

દ્વિગુણ dotProd_16s(અચલ short* src1, અચલ short* src2, પૂર્ણાંક len)
{
    દ્વિગુણ r = 0.0;
    પૂર્ણાંક i = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    પૂર્ણાંક len0 = len & -VTraits<v_int16>::vlanes(), blockSize0 = (1 << 24), blockSize;

    જ્યારે (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_int64 v_sum = vx_setzero_s64();
        અચલ પૂર્ણાંક cWidth = VTraits<v_int16>::vlanes();

        પૂર્ણાંક j = 0;
        માટે (; j <= blockSize - cWidth; j += cWidth)
        {
            v_int16 v_src1 = vx_load(src1 + j);
            v_int16 v_src2 = vx_load(src2 + j);
            v_sum = v_dotprod_expand_fast(v_src1, v_src2, v_sum);
        }
        r += (દ્વિગુણ)v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    પાછા r + dotProd_(src1, src2, len - i);
}

દ્વિગુણ dotProd_32s(અચલ પૂર્ણાંક* src1, અચલ પૂર્ણાંક* src2, પૂર્ણાંક len)
{
#જો CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    દ્વિગુણ r = .0;
    પૂર્ણાંક i = 0;
    અચલ પૂર્ણાંક step  = VTraits<v_int32>::vlanes();
    v_float64 v_sum0 = vx_setzero_f64();
#જો CV_SIMD_WIDTH == 16
    અચલ પૂર્ણાંક wstep = step * 2;
    v_float64 v_sum1 = vx_setzero_f64();
    માટે (; i <= len - wstep; i += wstep, src1 += wstep, src2 += wstep)
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
    માટે (; i <= len - step; i += step, src1 += step, src2 += step)
    {
        v_int32 v_src1 = vx_load(src1);
        v_int32 v_src2 = vx_load(src2);
        v_sum0 = v_dotprod_expand_fast(v_src1, v_src2, v_sum0);
    }
    r = v_reduce_sum(v_sum0);
    vx_cleanup();
    પાછા r + dotProd_(src1, src2, len - i);
#નહીંતર
    પાછા dotProd_(src1, src2, len);
#endif
}

દ્વિગુણ dotProd_32f(અચલ દશાંશ* src1, અચલ દશાંશ* src2, પૂર્ણાંક len)
{
    દ્વિગુણ r = 0.0;
    પૂર્ણાંક i = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    પૂર્ણાંક len0 = len & -VTraits<v_float32>::vlanes(), blockSize0 = (1 << 13), blockSize;

    જ્યારે (i < len0)
    {
        blockSize = std::min(len0 - i, blockSize0);
        v_float32 v_sum = vx_setzero_f32();

        પૂર્ણાંક j = 0;
        પૂર્ણાંક cWidth = VTraits<v_float32>::vlanes();

#જો CV_ENABLE_UNROLLED
        v_float32 v_sum1 = vx_setzero_f32();
        v_float32 v_sum2 = vx_setzero_f32();
        v_float32 v_sum3 = vx_setzero_f32();

        માટે (; j <= blockSize - (cWidth * 4); j += (cWidth * 4))
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

        માટે (; j <= blockSize - cWidth; j += cWidth)
            v_sum = v_muladd(vx_load(src1 + j), vx_load(src2 + j), v_sum);

        r += v_reduce_sum(v_sum);

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
    vx_cleanup();
#endif
    પાછા r + dotProd_(src1, src2, len - i);
}

દ્વિગુણ dotProd_64f(અચલ દ્વિગુણ* src1, અચલ દ્વિગુણ* src2, પૂર્ણાંક len)
{
    પાછા dotProd_(src1, src2, len);
}

#endif
CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace
