/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
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
//   * The name of Intel Corporation may not be used to endorse or promote products
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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_core.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "dxt.hpp"
#ಅನ್ತರ್ಭಾವ "dxt.simd.hpp"
#ಅನ್ತರ್ಭಾವ "dxt.simd_declarations.hpp"
#ಅನ್ತರ್ಭಾವ <map>

ನಾಮಾಕಾಶ cv
{

#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 710
#ವ್ಯಾಖ್ಯಾನಿಸು USE_IPP_DFT 1
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು USE_IPP_DFT
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined USE_IPP_DFT
#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 202220
#ವ್ಯಾಖ್ಯಾನಿಸು IPP_DISABLE_DFT32F ((ಆಳ == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು IPP_DISABLE_DFT32F ಸುಳ್ಳು
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
// factorization used by the OpenCL radix selection
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
DFTFactorize( ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ* factors )
{
    ಪೂರ್ಣಾಂಕ nf = 0, f, i, j;

    ಆದರೆ( n <= 5 )
    {
        factors[0] = n;
        ಹಿಂತಿರುಗಿ 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    ಆದರೆ( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    ಗಾಗಿ( f = 3; n > 1; )
    {
        ಪೂರ್ಣಾಂಕ d = n/f;
        ಆದರೆ( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            f += 2;
            ಆದರೆ( f*f > n )
                ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    ಗಾಗಿ( i = f; i < (nf+f)/2; i++ )
        CV_ಅದಲುಬದಲು( factors[i], factors[nf-i-1+f], j );

    ಹಿಂತಿರುಗಿ nf;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ ippsDFTFwd_CToC( ಸ್ಥಿರಾಂಕ Complex<ದಶಮಾಂಶ>* src, Complex<ದಶಮಾಂಶ>* dst,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* spec, uchar* buf)
{
    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (ಸ್ಥಿರಾಂಕ Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ಸ್ಥಿರಾಂಕ IppsDFTSpec_C_32fc*)spec, buf);
}

ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ ippsDFTFwd_CToC( ಸ್ಥಿರಾಂಕ Complex<ದ್ವಿಗುಣ>* src, Complex<ದ್ವಿಗುಣ>* dst,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* spec, uchar* buf)
{
    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (ಸ್ಥಿರಾಂಕ Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ಸ್ಥಿರಾಂಕ IppsDFTSpec_C_64fc*)spec, buf);
}

ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ ippsDFTInv_CToC( ಸ್ಥಿರಾಂಕ Complex<ದಶಮಾಂಶ>* src, Complex<ದಶಮಾಂಶ>* dst,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* spec, uchar* buf)
{
    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (ಸ್ಥಿರಾಂಕ Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ಸ್ಥಿರಾಂಕ IppsDFTSpec_C_32fc*)spec, buf);
}

ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ ippsDFTInv_CToC( ಸ್ಥಿರಾಂಕ Complex<ದ್ವಿಗುಣ>* src, Complex<ದ್ವಿಗುಣ>* dst,
                                  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* spec, uchar* buf)
{
    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (ಸ್ಥಿರಾಂಕ Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ಸ್ಥಿರಾಂಕ IppsDFTSpec_C_64fc*)spec, buf);
}

ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ ippsDFTFwd_RToPack( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst,
                                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* spec, uchar* buf)
{
    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (ಸ್ಥಿರಾಂಕ IppsDFTSpec_R_32f*)spec, buf);
}

ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ ippsDFTFwd_RToPack( ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst,
                                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* spec, uchar* buf)
{
    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (ಸ್ಥಿರಾಂಕ IppsDFTSpec_R_64f*)spec, buf);
}

ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ ipps_DFTInv_Pack_ಗಮ್ಯ_R( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst,
                                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* spec, uchar* buf)
{
    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_ಗಮ್ಯ_R_32f, src, dst, (ಸ್ಥಿರಾಂಕ IppsDFTSpec_R_32f*)spec, buf);
}

ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ ipps_DFTInv_Pack_ಗಮ್ಯ_R( ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst,
                                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* spec, uchar* buf)
{
    ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_ಗಮ್ಯ_R_64f, src, dst, (ಸ್ಥಿರಾಂಕ IppsDFTSpec_R_64f*)spec, buf);
}
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ OcvDftOptions;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*DFTFunc)(ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
ರಚನೆ OcvDftOptions {
    ದ್ವಿಗುಣ scale;
    ಪೂರ್ಣಾಂಕ n;

    ತಾರ್ಕಿಕ isInverse;
    ತಾರ್ಕಿಕ isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    ತಾರ್ಕಿಕ useIpp;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#ಆದರೆ_ಅಂತ್ಯ

    ಸ್ಥಿರಾಂಕ DftPlan* plan;
    ಸ್ಥಿರಾಂಕ DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = ಸುಳ್ಳು;
        isComplex = ಸುಳ್ಳು;
        useIpp = ಸುಳ್ಳು;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
        ipp_spec = 0;
        ipp_work = 0;
#ಆದರೆ_ಅಂತ್ಯ
        dft_func = 0;
    }
};


/****************************************************************************************\
                     New 1D engine: kernel table, plan builder, driver
\****************************************************************************************/

ಸ್ಥಿರ DFTKernels getDFTKernelsDispatch(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_CPU_DISPATCH(getDFTKernels_, (ಆಳ), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ DFTKernels& getDFTKernels(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    ಹಿಂತಿರುಗಿ ಆಳ == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
ರಚನೆ DftTwiddleGen
{
    ಪೂರ್ಣಾಂಕ M, logB, ಮುಖವಾಡ_B;
    Auto_ಬಫರ್<ದ್ವಿಗುಣ, 1024> buf;
    ದ್ವಿಗುಣ *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    ಸ್ಥಿರ ಶೂನ್ಯ fill_ಕೋಷ್ಟಕ(ದ್ವಿಗುಣ* re, ದ್ವಿಗುಣ* im, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ದ್ವಿಗುಣ wr, ದ್ವಿಗುಣ wi, ದ್ವಿಗುಣ ಹೆಜ್ಜೆ_Angle)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i0 = 0; i0 < ಎಣಿಕೆ_3; i0 += 16)
        {
            ದ್ವಿಗುಣ t0 = ಹೆಜ್ಜೆ_Angle*i0;
            ದ್ವಿಗುಣ cr = i0 == 0 ? 1 : ಕೋಜ್ಯಾ(t0), ci = i0 == 0 ? 0 : ಜ್ಯಾ(t0);
            ಪೂರ್ಣಾಂಕ i1 = std::min(i0 + 16, ಎಣಿಕೆ_3);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                ದ್ವಿಗುಣ t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    ಶೂನ್ಯ init(ಪೂರ್ಣಾಂಕ _M)
    {
        M = _M;
        logB = 0;
        ಆಗ ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        ಪೂರ್ಣಾಂಕ B = 1 << logB;
        ಮುಖವಾಡ_B = B - 1;
        ಪೂರ್ಣಾಂಕ nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.ದತ್ತಾಂಶ(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        ದ್ವಿಗುಣ a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fill_ಕೋಷ್ಟಕ(lo_re, lo_im, B, ಕೋಜ್ಯಾ(a), ಜ್ಯಾ(a), a);
        fill_ಕೋಷ್ಟಕ(hi_re, hi_im, nhi, ಕೋಜ್ಯಾ(a*B), ಜ್ಯಾ(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    ಶೂನ್ಯ get(ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ& re, ದ್ವಿಗುಣ& im) ಸ್ಥಿರಾಂಕ
    {
        CV_DbgAssert(m >= 0);
        ಆದರೆ (m >= M)
            m %= M;
        ಪೂರ್ಣಾಂಕ a = m >> logB, b = m & ಮುಖವಾಡ_B;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಶೂನ್ಯ DftPlan::fillTables(size_t ntw, ತಾರ್ಕಿಕ need_rtw, ತಾರ್ಕಿಕ need_dct)
{
    DftPlan& p = *ಇದು;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    ಪೂರ್ಣಾಂಕ M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*ಗಾತ್ರ(T) + ಗಾತ್ರ(ದ್ವಿಗುಣ) - 1)/ಗಾತ್ರ(ದ್ವಿಗುಣ) + 1);
    T* cur = (T*)p.tw.ದತ್ತಾಂಶ();
    ಪೂರ್ಣಾಂಕ maxL = 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < p.nstages; s++)
        ಆದರೆ (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].ಹರವು);
    Auto_ಬಫರ್<ದ್ವಿಗುಣ, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    ದ್ವಿಗುಣ* twd = twd_buf.ದತ್ತಾಂಶ();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        ಪೂರ್ಣಾಂಕ r = st.radix, L = st.ಹರವು, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ = M/(r*L);
        ಆದರೆ (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr ಪೂರ್ಣಾಂಕ K = 8;
            ದ್ವಿಗುಣ cr[K], ci[K], wr, wi;
            gen.get(K*ಹೆಜ್ಜೆ, wr, wi);
            ದ್ವಿಗುಣ* t1r = twd; ದ್ವಿಗುಣ* t1i = twd + L;      // leg 1 kept in double for the products
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*ಹೆಜ್ಜೆ, cr[k], ci[k]);
                ಪೂರ್ಣಾಂಕ j1 = std::min(j0 + 16*K, L);
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = j0; j < j1; j += K)
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        ದ್ವಿಗುಣ t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ q = 2; q < r; q++)
            {
                ಸ್ಥಿರಾಂಕ T* pr = twr + (q-2)*ts; ಸ್ಥಿರಾಂಕ T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < L; j++)
                {
                    ದ್ವಿಗುಣ ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            // large odd radix: exact-ish generator lookup per entry
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ q = 1; q < r; q++)
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < L; j++)
                {
                    ದ್ವಿಗುಣ re, im;
                    gen.get(q*j*ಹೆಜ್ಜೆ, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        ಆದರೆ (r >= 7)
        {
            ಪೂರ್ಣಾಂಕ h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ pp = 1; pp <= h; pp++)
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 1; k <= h; k++)
                {
                    ದ್ವಿಗುಣ t = 2*CV_PI*((ದ್ವಿಗುಣ)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)ಕೋಜ್ಯಾ(t);
                    sn[(pp-1)*h + (k-1)] = (T)ಜ್ಯಾ(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    ಆದರೆ (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ = M/n;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k <= nc; k++)
        {
            ದ್ವಿಗುಣ re, im;
            gen.get(k*ಹೆಜ್ಜೆ, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    ಆದರೆ (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        ದ್ವಿಗುಣ scale = (p.ಬಗೆ == DFT_ಬಗೆ_DCT ? 2 : 1)*std::ವರ್ಗಮೂಲ(1./(2*n));
        ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ = M/(4*n);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k <= nc; k++)
        {
            ದ್ವಿಗುಣ re, im;
            gen.get(k*ಹೆಜ್ಜೆ, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
ಶೂನ್ಯ DftPlan::build(ಪೂರ್ಣಾಂಕ ಬಗೆ_2, ಪೂರ್ಣಾಂಕ _n, ಪೂರ್ಣಾಂಕ ಆಳ_2, ಪೂರ್ಣಾಂಕ _vl)
{
    DftPlan& p = *ಇದು;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(ಆಳ_2 == CV_32F || ಆಳ_2 == CV_64F);
    ತಾರ್ಕಿಕ real_ಬಗೆ = ಬಗೆ_2 != DFT_ಬಗೆ_C2_C;
    ತಾರ್ಕಿಕ dct_ಬಗೆ = ಬಗೆ_2 == DFT_ಬಗೆ_DCT || ಬಗೆ_2 == DFT_ಬಗೆ_IDCT;
    ಆದರೆ (dct_ಬಗೆ)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.ಬಗೆ = ಬಗೆ_2; p.n = _n; p.ಆಳ = ಆಳ_2; p.vl = _vl;
    p.nc = real_ಬಗೆ && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = ಆಳ == CV_32F ? ಗಾತ್ರ(ದಶಮಾಂಶ) : ಗಾತ್ರ(ದ್ವಿಗುಣ);
    p.real_ಒಳಹರಿವು = ಬಗೆ == DFT_ಬಗೆ_R2_C && (n & 1) != 0;
    p.need_tmp = ಬಗೆ == DFT_ಬಗೆ_C2_R || ಬಗೆ == DFT_ಬಗೆ_DCT || ಬಗೆ == DFT_ಬಗೆ_IDCT;
    p.pingpong = ಸುಳ್ಳು;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    ಪೂರ್ಣಾಂಕ k2 = 0, rest = nc;
    ಆಗ ((rest & 1) == 0) { rest >>= 1; k2++; }
    ಪೂರ್ಣಾಂಕ r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.ಮೊದಲ_radix = r0;
    ಪೂರ್ಣಾಂಕ radices[DFT_MAX_STAGES], nst = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    ಆಗ (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    ಆಗ (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ f = 7; rest > 1; f += 2)
    {
        ಆದರೆ (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            ನಿಲ್ಲಿಸು;
        }
        ಆಗ (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    ಪೂರ್ಣಾಂಕ L = r0, max_h = 0;
    size_t ntw = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        ಪೂರ್ಣಾಂಕ r = radices[s];
        st.radix = r; st.ಹರವು = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        ಆದರೆ (vl > 1 && L >= vl)
        {
            st.ಬಹುಲಕ = DFT_STAGE_ತುಂಬಿದ;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            ಆದರೆ (L % vl != 0)
                p.pingpong = ನಿಜ;   // back-off clamp => overlapping stores => must be out-of-place
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            st.ಬಹುಲಕ = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        ಆದರೆ (r >= 7)
        {
            ಪೂರ್ಣಾಂಕ h = (r - 1)/2;
            ntw += 2*(size_t)h*h;
            max_h = std::max(max_h, h);
        }
        L *= r;
    }
    CV_Assert(L == nc);

    // digit-reversal: position p = sum_s d_s*L_s holds input element sum_s d_s*M_s, M_s = nc/(L_s*r_s);
    // the radix sequence is r0 (if > 1) followed by the stage radices.
    //  - r0 == 1 (odd nc): itab holds two element indices (re, im) per complex element for the
    //    scalar gather (im index unused when real_input);
    //  - r0 > 1: the r0 inputs of group g are the column x[base(g) + q*nc/r0]; itab[base(g)] = g*r0
    //    tells the transposed first stage where the outputs of column base(g) go.
    ಪೂರ್ಣಾಂಕ seq[DFT_MAX_STAGES + 1], nseq = 0;
    ಆದರೆ (r0 > 1) seq[nseq++] = r0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < nst; s++) seq[nseq++] = radices[s];
    ಪೂರ್ಣಾಂಕ digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        ಪೂರ್ಣಾಂಕ Ls = 1;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    ಆದರೆ (r0 == 1)
    {
        p.itab.allocate(nc*2);
        ಪೂರ್ಣಾಂಕ* it_all = p.itab.ದತ್ತಾಂಶ();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        ಪೂರ್ಣಾಂಕ n_ಒಳ = std::min(nseq, 2);
        ಪೂರ್ಣಾಂಕ r_0 = n_ಒಳ > 0 ? seq[0] : 1, w_0 = n_ಒಳ > 0 ? weights[0] : 0;
        ಪೂರ್ಣಾಂಕ r_1 = n_ಒಳ > 1 ? seq[1] : 1, w_1 = n_ಒಳ > 1 ? weights[1] : 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ pos = 0, rev_hi = 0; pos < nc; )
        {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d1 = 0; d1 < r_1; d1++)
            {
                ಪೂರ್ಣಾಂಕ rev = rev_hi + d1*w_1;
                ಪೂರ್ಣಾಂಕ* it = it_all + pos*2;
                ಆದರೆ (p.real_ಒಳಹರಿವು)
                {
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = n_ಒಳ; s < nseq; s++)
            {
                rev_hi += weights[s];
                ಆದರೆ (++digits[s] < seq[s])
                    ನಿಲ್ಲಿಸು;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        ಪೂರ್ಣಾಂಕ M = nc/r0;
        p.itab.allocate(M);
        ಪೂರ್ಣಾಂಕ* gtab = p.itab.ದತ್ತಾಂಶ();
        ಪೂರ್ಣಾಂಕ gw[DFT_MAX_STAGES + 1];
        {
            ಪೂರ್ಣಾಂಕ Ls = 1;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        ಪೂರ್ಣಾಂಕ sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        ಪೂರ್ಣಾಂಕ r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        ಪೂರ್ಣಾಂಕ r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ತಳ = 0, g_hi = 0; ತಳ < M; )
        {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ db = 0; db < r_b; db++)
            {
                ಪೂರ್ಣಾಂಕ g = (g_hi + db*g_b)*r0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[ತಳ++] = g;
            }
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                ಆದರೆ (++digits[s] < seq[s])
                    ನಿಲ್ಲಿಸು;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    ತಾರ್ಕಿಕ need_rtw = real_ಬಗೆ && n % 2 == 0;
    ಆದರೆ (need_rtw) ntw += 2*(size_t)(nc + 1);
    ಆದರೆ (dct_ಬಗೆ) ntw += 2*(size_t)(nc + 1);
    ಆದರೆ (ಆಳ == CV_32F)
        fillTables<ದಶಮಾಂಶ>(ntw, need_rtw, dct_ಬಗೆ);
    ಇಲ್ಲದಿದ್ದರೆ
        fillTables<ದ್ವಿಗುಣ>(ntw, need_rtw, dct_ಬಗೆ);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_ಬೈಟುಗಳು = 2*(size_t)nc*esz;
    ತಾರ್ಕಿಕ two_ಜೋಡಿಗಳು = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_ಜೋಡಿಗಳು ? pair_ಬೈಟುಗಳು : 0;
    p.scratch_ofs = pair_ಬೈಟುಗಳು*(two_ಜೋಡಿಗಳು ? 2 : 1);
    size_t scratch_ಬೈಟುಗಳು = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_ಬೈಟುಗಳು = p.scratch_ofs + scratch_ಬೈಟುಗಳು + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Dft(ಸ್ಥಿರಾಂಕ OcvDftOptions& c, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t sstep, ಶೂನ್ಯ* dst, size_t dstep,
                   ತಾರ್ಕಿಕ complex_io)
{
    ಸ್ಥಿರಾಂಕ DftPlan& p = *c.plan;
    ಸ್ಥಿರಾಂಕ DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.ಆಳ == CV_32F ? ಗಾತ್ರ(ದಶಮಾಂಶ) : ಗಾತ್ರ(ದ್ವಿಗುಣ);
    uchar* ತಳ = alignPtr(c.workspace, 64);
    uchar* re0 = ತಳ;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = ತಳ + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = ತಳ + p.scratch_ofs;

    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* asrc = src;
    size_t astep = sstep;
    ತಾರ್ಕಿಕ conj = ಸುಳ್ಳು;
    ಆದರೆ (p.ಬಗೆ == DFT_ಬಗೆ_C2_R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = ನಿಜ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p.ಬಗೆ == DFT_ಬಗೆ_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = ನಿಜ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p.ಬಗೆ == DFT_ಬಗೆ_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p.ಬಗೆ == DFT_ಬಗೆ_C2_C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.ಮೊದಲ_radix == 1);

    DftPreprocFunc pre = p.ಮೊದಲ_radix == 1 ? k.preprocRadix0 : p.ಮೊದಲ_radix == 2 ? k.preprocRadix2 :
                         p.ಮೊದಲ_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < p.nstages; i++)
    {
        ಸ್ಥಿರಾಂಕ DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        ಆದರೆ (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        ಇಲ್ಲದಿದ್ದರೆ
            fn(st, sre, sim, sre, sim, scratch);
    }

    ದ್ವಿಗುಣ scale = c.scale;
    ಆಯ್ಕೆ (p.ಬಗೆ)
    {
    ಸ್ಥಿತಿ DFT_ಬಗೆ_C2_C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ DFT_ಬಗೆ_R2_C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ DFT_ಬಗೆ_C2_R:
        ಆದರೆ (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        ಇಲ್ಲದಿದ್ದರೆ
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ DFT_ಬಗೆ_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        CV_Assert(p.ಬಗೆ == DFT_ಬಗೆ_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
DFT(ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ Complex<T>* src, Complex<T>* dst)
{
    ಆದರೆ( c.useIpp )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
        ಆದರೆ( !c.isInverse )
        {
            ಆದರೆ (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
                ಹಿಂತಿರುಗಿ;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
                ಹಿಂತಿರುಗಿ;
            }
        }
        ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಸತತ_Dft(c, src, 1, dst, 1, ಸುಳ್ಳು);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
RealDFT(ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ T* src, T* dst)
{
    ಪೂರ್ಣಾಂಕ complex_output = c.isComplex;

    ಆದರೆ( c.useIpp )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
        ಪೂರ್ಣಾಂಕ n = c.n;
        T* ipp_dst = dst + complex_output;
        ಆದರೆ (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            ಆದರೆ( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                ಆದರೆ( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
            ಹಿಂತಿರುಗಿ;
        }
        ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಸತತ_Dft(c, src, 1, dst, 1, complex_output != 0);
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
CCSIDFT(ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ T* src, T* dst)
{
    ಪೂರ್ಣಾಂಕ complex_ಒಳಹರಿವು = c.isComplex;

    ಆದರೆ( complex_ಒಳಹರಿವು )
        CV_Assert( src != dst );
    ಆದರೆ( c.useIpp )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        ಸ್ಥಿರಾಂಕ T* ipp_src = src;
        T save_s1 = 0;
        ಆದರೆ( complex_ಒಳಹರಿವು )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        Ipp_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = ipps_DFTInv_Pack_ಗಮ್ಯ_R( ipp_src, dst, c.ipp_spec, c.ipp_work );
        ಆದರೆ( complex_ಒಳಹರಿವು )
            ((T*)src)[1] = save_s1;
        ಆದರೆ( ಸ್ಥಿತಿ_2 >= 0 )
        {
            CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
            ಹಿಂತಿರುಗಿ;
        }
        ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಸತತ_Dft(c, src, 1, dst, 1, complex_ಒಳಹರಿವು != 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Copy_ಸ್ತಂಭ( ಸ್ಥಿರಾಂಕ uchar* _src, size_t src_ಹೆಜ್ಜೆ,
            uchar* _dst, size_t dst_ಹೆಜ್ಜೆ,
            ಪೂರ್ಣಾಂಕ len, size_t elem_ಗಾತ್ರ_2 )
{
    ಪೂರ್ಣಾಂಕ i, t0, t1;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src = (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)_src;
    ಪೂರ್ಣಾಂಕ* dst = (ಪೂರ್ಣಾಂಕ*)_dst;
    src_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(src[0]);
    dst_ಹೆಜ್ಜೆ /= ಗಾತ್ರ(dst[0]);

    ಆದರೆ( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) )
    {
        ಗಾಗಿ( i = 0; i < len; i++, src += src_ಹೆಜ್ಜೆ, dst += dst_ಹೆಜ್ಜೆ )
            dst[0] = src[0];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ)*2 )
    {
        ಗಾಗಿ( i = 0; i < len; i++, src += src_ಹೆಜ್ಜೆ, dst += dst_ಹೆಜ್ಜೆ )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ)*4 )
    {
        ಗಾಗಿ( i = 0; i < len; i++, src += src_ಹೆಜ್ಜೆ, dst += dst_ಹೆಜ್ಜೆ )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
            t0 = src[2]; t1 = src[3];
            dst[2] = t0; dst[3] = t1;
        }
    }
}


// Copies `ncols` adjacent columns of `src` (element size elem_size, row step src_step in bytes)
// into `ncols` contiguous column buffers of `len` elements each: dst + k*len*elem_size holds
// column k. Row-wise: each source row is read once, sequentially. (Row steps are only
// guaranteed to be multiples of the element size, hence the byte arithmetic.)
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
copyFromColumns_( ಸ್ಥಿರಾಂಕ uchar* src, size_t src_ಹೆಜ್ಜೆ, T* dst, ಪೂರ್ಣಾಂಕ ncols, ಪೂರ್ಣಾಂಕ len )
{
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++, src += src_ಹೆಜ್ಜೆ )
    {
        ಸ್ಥಿರಾಂಕ T* s = (ಸ್ಥಿರಾಂಕ T*)src;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
CopyFromColumns( ಸ್ಥಿರಾಂಕ uchar* src, size_t src_ಹೆಜ್ಜೆ, uchar* dst, ಪೂರ್ಣಾಂಕ ncols, ಪೂರ್ಣಾಂಕ len, size_t elem_ಗಾತ್ರ_2 )
{
    ಆದರೆ( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) )
        copyFromColumns_(src, src_ಹೆಜ್ಜೆ, (ಪೂರ್ಣಾಂಕ*)dst, ncols, len);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(int64) )
        copyFromColumns_(src, src_ಹೆಜ್ಜೆ, (int64*)dst, ncols, len);
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(int64)*2 );
        copyFromColumns_(src, src_ಹೆಜ್ಜೆ, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
copy_ಗಮ್ಯ_Columns( ಸ್ಥಿರಾಂಕ T* src, uchar* dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ncols, ಪೂರ್ಣಾಂಕ len )
{
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++, dst += dst_ಹೆಜ್ಜೆ )
    {
        T* d = (T*)dst;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
Copy_ಗಮ್ಯ_Columns( ಸ್ಥಿರಾಂಕ uchar* src, uchar* dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ncols, ಪೂರ್ಣಾಂಕ len, size_t elem_ಗಾತ್ರ_2 )
{
    ಆದರೆ( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) )
        copy_ಗಮ್ಯ_Columns((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)src, dst, dst_ಹೆಜ್ಜೆ, ncols, len);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(int64) )
        copy_ಗಮ್ಯ_Columns((ಸ್ಥಿರಾಂಕ int64*)src, dst, dst_ಹೆಜ್ಜೆ, ncols, len);
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( elem_ಗಾತ್ರ_2 == ಗಾತ್ರ(int64)*2 );
        copy_ಗಮ್ಯ_Columns((ಸ್ಥಿರಾಂಕ Vec<int64, 2>*)src, dst, dst_ಹೆಜ್ಜೆ, ncols, len);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
ExpandCCS( uchar* _ptr, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ_2 )
{
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ( elem_ಗಾತ್ರ_2 == (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ದಶಮಾಂಶ) )
    {
        ದಶಮಾಂಶ* p = (ದಶಮಾಂಶ*)_ptr;
        ಗಾಗಿ( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        ಆದರೆ( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        ಗಾಗಿ( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ದ್ವಿಗುಣ* p = (ದ್ವಿಗುಣ*)_ptr;
        ಗಾಗಿ( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        ಆದರೆ( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        ಗಾಗಿ( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ OcvDftOptions&, ಸ್ಥಿರಾಂಕ T*, T*)>
ಸ್ಥಿರ ಶೂನ್ಯ dftWrap(ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst)
{
    fn(c, (ಸ್ಥಿರಾಂಕ T*)src, (T*)dst);
}

}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL* Ipp_DFTGet_ಗಾತ್ರ_Func)(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, IppHintAlgorithm, ಪೂರ್ಣಾಂಕ*, ಪೂರ್ಣಾಂಕ*, ಪೂರ್ಣಾಂಕ*);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL* IppDFTInitFunc)(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, IppHintAlgorithm, ಶೂನ್ಯ*, uchar*);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SpecType, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *init_fn)(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, IppHintAlgorithm, SpecType*, Ipp8u*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ippDFTInitWrap(ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ flags, IppHintAlgorithm hint, ಶೂನ್ಯ* spec, Ipp8u* initbuf)
{
    ಹಿಂತಿರುಗಿ init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv
{
#ಆದರೆ defined USE_IPP_DFT

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL* ippiDFT_C_Func)(ಸ್ಥಿರಾಂಕ Ipp32fc*, ಪೂರ್ಣಾಂಕ, Ipp32fc*, ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ IppiDFTSpec_C_32fc*, Ipp8u*);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL* ippiDFT_R_Func)(ಸ್ಥಿರಾಂಕ Ipp32f* , ಪೂರ್ಣಾಂಕ, Ipp32f* , ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ IppiDFTSpec_R_32f* , Ipp8u*);

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು Dft>
ವರ್ಗ Dft_C_IPPLoop_Invoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:

    Dft_C_IPPLoop_Invoker(ಸ್ಥಿರಾಂಕ uchar * _src, size_t src_ಹೆಜ್ಜೆ_2, uchar * _dst, size_t dst_ಹೆಜ್ಜೆ_2, ಪೂರ್ಣಾಂಕ ಅಗಲ_3,
                          ಸ್ಥಿರಾಂಕ Dft& _ippidft, ಪೂರ್ಣಾಂಕ _norm_flag, ತಾರ್ಕಿಕ *_ok) :
        ParallelLoopBody(),
        src(_src), src_ಹೆಜ್ಜೆ(src_ಹೆಜ್ಜೆ_2), dst(_dst), dst_ಹೆಜ್ಜೆ(dst_ಹೆಜ್ಜೆ_2), ಅಗಲ(ಅಗಲ_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = ನಿಜ;
    }

    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Range& range) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        Ipp_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
        Ipp8u* p_ಬಫರ್ = 0;
        Ipp8u* pMemInit= 0;
        ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_ಬಫರ್=0;
        ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Spec=0;
        ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Init=0;

        Ippi_ಗಾತ್ರ src_Roi_ಗಾತ್ರ = {ಅಗಲ, 1};

        ಸ್ಥಿತಿ_2 = ippi_DFTGet_ಗಾತ್ರ_C_32fc(src_Roi_ಗಾತ್ರ, norm_flag, ippAlgHintNone, &ಗಾತ್ರ_Spec, &ಗಾತ್ರ_Init, &ಗಾತ್ರ_ಬಫರ್ );
        ಆದರೆ ( ಸ್ಥಿತಿ_2 < 0 )
        {
            *ok = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( ಗಾತ್ರ_Spec );

        ಆದರೆ ( ಗಾತ್ರ_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( ಗಾತ್ರ_Init );

        ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
            p_ಬಫರ್ = (Ipp8u*)CV_IPP_MALLOC( ಗಾತ್ರ_ಬಫರ್ );

        ಸ್ಥಿತಿ_2 = ippiDFTInit_C_32fc( src_Roi_ಗಾತ್ರ, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        ಆದರೆ ( ಗಾತ್ರ_Init > 0 )
            ippFree( pMemInit );

        ಆದರೆ ( ಸ್ಥಿತಿ_2 < 0 )
        {
            ippFree( pDFTSpec );
            ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
                ippFree( p_ಬಫರ್ );
            *ok = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; ++i)
            ಆದರೆ(!ippidft((Ipp32fc*)(src + src_ಹೆಜ್ಜೆ * i), src_ಹೆಜ್ಜೆ, (Ipp32fc*)(dst + dst_ಹೆಜ್ಜೆ * i), dst_ಹೆಜ್ಜೆ,
                        pDFTSpec, (Ipp8u*)p_ಬಫರ್))
            {
                *ok = ಸುಳ್ಳು;
            }

        ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
            ippFree( p_ಬಫರ್ );

        ippFree( pDFTSpec );
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP|CV_IMPL_MT);
    }

ಖಾಸಗಿ:
    ಸ್ಥಿರಾಂಕ uchar * src;
    size_t src_ಹೆಜ್ಜೆ;
    uchar * dst;
    size_t dst_ಹೆಜ್ಜೆ;
    ಪೂರ್ಣಾಂಕ ಅಗಲ;
    ಸ್ಥಿರಾಂಕ Dft& ippidft;
    ಪೂರ್ಣಾಂಕ norm_flag;
    ತಾರ್ಕಿಕ *ok;

    ಸ್ಥಿರಾಂಕ Dft_C_IPPLoop_Invoker& ಕ್ರಿಯಾಕಾರಕ= (ಸ್ಥಿರಾಂಕ Dft_C_IPPLoop_Invoker&);
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು Dft>
ವರ್ಗ Dft_R_IPPLoop_Invoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:

    Dft_R_IPPLoop_Invoker(ಸ್ಥಿರಾಂಕ uchar * _src, size_t src_ಹೆಜ್ಜೆ_2, uchar * _dst, size_t dst_ಹೆಜ್ಜೆ_2, ಪೂರ್ಣಾಂಕ ಅಗಲ_3,
                          ಸ್ಥಿರಾಂಕ Dft& _ippidft, ಪೂರ್ಣಾಂಕ _norm_flag, ತಾರ್ಕಿಕ *_ok) :
        ParallelLoopBody(),
        src(_src), src_ಹೆಜ್ಜೆ(src_ಹೆಜ್ಜೆ_2), dst(_dst), dst_ಹೆಜ್ಜೆ(dst_ಹೆಜ್ಜೆ_2), ಅಗಲ(ಅಗಲ_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = ನಿಜ;
    }

    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Range& range) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        Ipp_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
        Ipp8u* p_ಬಫರ್ = 0;
        Ipp8u* pMemInit= 0;
        ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_ಬಫರ್=0;
        ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Spec=0;
        ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Init=0;

        Ippi_ಗಾತ್ರ src_Roi_ಗಾತ್ರ = {ಅಗಲ, 1};

        ಸ್ಥಿತಿ_2 = ippi_DFTGet_ಗಾತ್ರ_R_32f(src_Roi_ಗಾತ್ರ, norm_flag, ippAlgHintNone, &ಗಾತ್ರ_Spec, &ಗಾತ್ರ_Init, &ಗಾತ್ರ_ಬಫರ್ );
        ಆದರೆ ( ಸ್ಥಿತಿ_2 < 0 )
        {
            *ok = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( ಗಾತ್ರ_Spec );

        ಆದರೆ ( ಗಾತ್ರ_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( ಗಾತ್ರ_Init );

        ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
            p_ಬಫರ್ = (Ipp8u*)CV_IPP_MALLOC( ಗಾತ್ರ_ಬಫರ್ );

        ಸ್ಥಿತಿ_2 = ippiDFTInit_R_32f( src_Roi_ಗಾತ್ರ, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        ಆದರೆ ( ಗಾತ್ರ_Init > 0 )
            ippFree( pMemInit );

        ಆದರೆ ( ಸ್ಥಿತಿ_2 < 0 )
        {
            ippFree( pDFTSpec );
            ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
                ippFree( p_ಬಫರ್ );
            *ok = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; ++i)
            ಆದರೆ(!ippidft((ದಶಮಾಂಶ*)(src + src_ಹೆಜ್ಜೆ * i), src_ಹೆಜ್ಜೆ, (ದಶಮಾಂಶ*)(dst + dst_ಹೆಜ್ಜೆ * i), dst_ಹೆಜ್ಜೆ,
                        pDFTSpec, (Ipp8u*)p_ಬಫರ್))
            {
                *ok = ಸುಳ್ಳು;
            }

        ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
            ippFree( p_ಬಫರ್ );

        ippFree( pDFTSpec );
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP|CV_IMPL_MT);
    }

ಖಾಸಗಿ:
    ಸ್ಥಿರಾಂಕ uchar * src;
    size_t src_ಹೆಜ್ಜೆ;
    uchar * dst;
    size_t dst_ಹೆಜ್ಜೆ;
    ಪೂರ್ಣಾಂಕ ಅಗಲ;
    ಸ್ಥಿರಾಂಕ Dft& ippidft;
    ಪೂರ್ಣಾಂಕ norm_flag;
    ತಾರ್ಕಿಕ *ok;

    ಸ್ಥಿರಾಂಕ Dft_R_IPPLoop_Invoker& ಕ್ರಿಯಾಕಾರಕ= (ಸ್ಥಿರಾಂಕ Dft_R_IPPLoop_Invoker&);
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು Dft>
ತಾರ್ಕಿಕ Dft_C_IPPLoop(ಸ್ಥಿರಾಂಕ uchar * src, size_t src_ಹೆಜ್ಜೆ, uchar * dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ Dft& ippidft, ಪೂರ್ಣಾಂಕ norm_flag)
{
    ತಾರ್ಕಿಕ ok;
    parallel_for_(Range(0, ಎತ್ತರ), Dft_C_IPPLoop_Invoker<Dft>(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, ippidft, norm_flag, &ok), (ಅಗಲ * ಎತ್ತರ)/(ದ್ವಿಗುಣ)(1<<16) );
    ಹಿಂತಿರುಗಿ ok;
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು Dft>
ತಾರ್ಕಿಕ Dft_R_IPPLoop(ಸ್ಥಿರಾಂಕ uchar * src, size_t src_ಹೆಜ್ಜೆ, uchar * dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ Dft& ippidft, ಪೂರ್ಣಾಂಕ norm_flag)
{
    ತಾರ್ಕಿಕ ok;
    parallel_for_(Range(0, ಎತ್ತರ), Dft_R_IPPLoop_Invoker<Dft>(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, ippidft, norm_flag, &ok), (ಅಗಲ * ಎತ್ತರ)/(ದ್ವಿಗುಣ)(1<<16) );
    ಹಿಂತಿರುಗಿ ok;
}

ರಚನೆ IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Ipp32fc* src, size_t src_ಹೆಜ್ಜೆ_3, Ipp32fc* dst, size_t dst_ಹೆಜ್ಜೆ_3, ಸ್ಥಿರಾಂಕ IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* p_ಬಫರ್) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ_3), dst, static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ_3), pDFTSpec, p_ಬಫರ್) >= 0 : ಸುಳ್ಳು;
    }
ಖಾಸಗಿ:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

ರಚನೆ IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippi_DFT_Pack_ಗಮ್ಯ_R_32f_C1_R(_func){}

    ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Ipp32f* src, size_t src_ಹೆಜ್ಜೆ_3, Ipp32f* dst, size_t dst_ಹೆಜ್ಜೆ_3, ಸ್ಥಿರಾಂಕ IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* p_ಬಫರ್) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ ippi_DFT_Pack_ಗಮ್ಯ_R_32f_C1_R ? CV_INSTRUMENT_FUN_IPP(ippi_DFT_Pack_ಗಮ್ಯ_R_32f_C1_R, src, static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ_3), dst, static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ_3), pDFTSpec, p_ಬಫರ್) >= 0 : ಸುಳ್ಳು;
    }
ಖಾಸಗಿ:
    ippiDFT_R_Func ippi_DFT_Pack_ಗಮ್ಯ_R_32f_C1_R;
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ ippi_DFT_C_32F(ಸ್ಥಿರಾಂಕ uchar * src, size_t src_ಹೆಜ್ಜೆ, uchar * dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ತಾರ್ಕಿಕ inv, ಪೂರ್ಣಾಂಕ norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
    Ipp8u* p_ಬಫರ್ = 0;
    Ipp8u* pMemInit= 0;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_ಬಫರ್=0;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Spec=0;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Init=0;

    Ippi_ಗಾತ್ರ src_Roi_ಗಾತ್ರ = {ಅಗಲ, ಎತ್ತರ};

    ಸ್ಥಿತಿ_2 = ippi_DFTGet_ಗಾತ್ರ_C_32fc(src_Roi_ಗಾತ್ರ, norm_flag, ippAlgHintNone, &ಗಾತ್ರ_Spec, &ಗಾತ್ರ_Init, &ಗಾತ್ರ_ಬಫರ್ );
    ಆದರೆ ( ಸ್ಥಿತಿ_2 < 0 )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( ಗಾತ್ರ_Spec );

    ಆದರೆ ( ಗಾತ್ರ_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( ಗಾತ್ರ_Init );

    ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
        p_ಬಫರ್ = (Ipp8u*)CV_IPP_MALLOC( ಗಾತ್ರ_ಬಫರ್ );

    ಸ್ಥಿತಿ_2 = ippiDFTInit_C_32fc( src_Roi_ಗಾತ್ರ, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    ಆದರೆ ( ಗಾತ್ರ_Init > 0 )
        ippFree( pMemInit );

    ಆದರೆ ( ಸ್ಥಿತಿ_2 < 0 )
    {
        ippFree( pDFTSpec );
        ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
            ippFree( p_ಬಫರ್ );
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಆದರೆ (!inv)
        ಸ್ಥಿತಿ_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ), (Ipp32fc*)dst, static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ), pDFTSpec, p_ಬಫರ್);
    ಇಲ್ಲದಿದ್ದರೆ
        ಸ್ಥಿತಿ_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ), (Ipp32fc*)dst, static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ), pDFTSpec, p_ಬಫರ್);

    ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
        ippFree( p_ಬಫರ್ );

    ippFree( pDFTSpec );

    ಆದರೆ(ಸ್ಥಿತಿ_2 >= 0)
    {
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ippi_DFT_R_32F(ಸ್ಥಿರಾಂಕ uchar * src, size_t src_ಹೆಜ್ಜೆ, uchar * dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ತಾರ್ಕಿಕ inv, ಪೂರ್ಣಾಂಕ norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
    Ipp8u* p_ಬಫರ್ = 0;
    Ipp8u* pMemInit= 0;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_ಬಫರ್=0;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Spec=0;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Init=0;

    Ippi_ಗಾತ್ರ src_Roi_ಗಾತ್ರ = {ಅಗಲ, ಎತ್ತರ};

    ಸ್ಥಿತಿ_2 = ippi_DFTGet_ಗಾತ್ರ_R_32f(src_Roi_ಗಾತ್ರ, norm_flag, ippAlgHintNone, &ಗಾತ್ರ_Spec, &ಗಾತ್ರ_Init, &ಗಾತ್ರ_ಬಫರ್ );
    ಆದರೆ ( ಸ್ಥಿತಿ_2 < 0 )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( ಗಾತ್ರ_Spec );

    ಆದರೆ ( ಗಾತ್ರ_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( ಗಾತ್ರ_Init );

    ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
        p_ಬಫರ್ = (Ipp8u*)CV_IPP_MALLOC( ಗಾತ್ರ_ಬಫರ್ );

    ಸ್ಥಿತಿ_2 = ippiDFTInit_R_32f( src_Roi_ಗಾತ್ರ, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    ಆದರೆ ( ಗಾತ್ರ_Init > 0 )
        ippFree( pMemInit );

    ಆದರೆ ( ಸ್ಥಿತಿ_2 < 0 )
    {
        ippFree( pDFTSpec );
        ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
            ippFree( p_ಬಫರ್ );
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಆದರೆ (!inv)
        ಸ್ಥಿತಿ_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (ದಶಮಾಂಶ*)src, static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ), (ದಶಮಾಂಶ*)dst, static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ), pDFTSpec, p_ಬಫರ್);
    ಇಲ್ಲದಿದ್ದರೆ
        ಸ್ಥಿತಿ_2 = CV_INSTRUMENT_FUN_IPP(ippi_DFTInv_Pack_ಗಮ್ಯ_R_32f_C1_R, (ದಶಮಾಂಶ*)src, static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ), (ದಶಮಾಂಶ*)dst, static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ), pDFTSpec, p_ಬಫರ್);

    ಆದರೆ ( ಗಾತ್ರ_ಬಫರ್ > 0 )
        ippFree( p_ಬಫರ್ );

    ippFree( pDFTSpec );

    ಆದರೆ(ಸ್ಥಿತಿ_2 >= 0)
    {
        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ನಾಮಾಕಾಶ cv
{

ಎಣಿಕೆ FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

ರಚನೆ OCL_FftPlan
{
ಖಾಸಗಿ:
    UMat twiddles;
    String buildOptions;
    ಪೂರ್ಣಾಂಕ thread_ಎಣಿಕೆ;
    ಪೂರ್ಣಾಂಕ dft_ಗಾತ್ರ;
    ಪೂರ್ಣಾಂಕ dft_ಆಳ;
    ತಾರ್ಕಿಕ ಸ್ಥಿತಿ_2;

ಸಾರ್ವಜನಿಕ:
    OCL_FftPlan(ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_5, ಪೂರ್ಣಾಂಕ ಆಳ_2) : dft_ಗಾತ್ರ(ಗಾತ್ರ_5), dft_ಆಳ(ಆಳ_2), ಸ್ಥಿತಿ_2(ನಿಜ)
    {
        CV_Assert( dft_ಆಳ == CV_32F || dft_ಆಳ == CV_64F );

        ಪೂರ್ಣಾಂಕ min_radix;
        std::vector<ಪೂರ್ಣಾಂಕ> radixes, ಖಂಡಗಳು;
        ocl_getRadixes(dft_ಗಾತ್ರ, radixes, ಖಂಡಗಳು, min_radix);
        thread_ಎಣಿಕೆ = dft_ಗಾತ್ರ / min_radix;

        ಆದರೆ (thread_ಎಣಿಕೆ > (ಪೂರ್ಣಾಂಕ) ocl::Device::getDefault().max_Work_ಗುಂಪು_ಗಾತ್ರ())
        {
            ಸ್ಥಿತಿ_2 = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }

        // generate string with radix calls
        String radix_processing;
        ಪೂರ್ಣಾಂಕ n = 1, twiddle_ಗಾತ್ರ = 0;
        ಗಾಗಿ (size_t i=0; i<radixes.size(); i++)
        {
            ಪೂರ್ಣಾಂಕ radix = radixes[i], ಖಂಡ_2 = ಖಂಡಗಳು[i];
            ಆದರೆ (ಖಂಡ_2 > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, ಖಂಡ_2, twiddle_ಗಾತ್ರ, n, dft_ಗಾತ್ರ/radix);
            ಇಲ್ಲದಿದ್ದರೆ
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_ಗಾತ್ರ, n, dft_ಗಾತ್ರ/radix);
            twiddle_ಗಾತ್ರ += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_ಗಾತ್ರ, CV_MAKE_TYPE(dft_ಆಳ, 2));
        ಆದರೆ (dft_ಆಳ == CV_32F)
            fill_Radix_ಕೋಷ್ಟಕ<ದಶಮಾಂಶ>(twiddles, radixes);
        ಇಲ್ಲದಿದ್ದರೆ
            fill_Radix_ಕೋಷ್ಟಕ<ದ್ವಿಗುಣ>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_ಗಾತ್ರ, min_radix, ocl::type_ಗಮ್ಯ_Str(dft_ಆಳ), ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(dft_ಆಳ, 2)),
                              dft_ಆಳ == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    ತಾರ್ಕಿಕ ಸರತಿಗೆ_ಸೇರಿಸು_Transform(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_dfts, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ fftType, ತಾರ್ಕಿಕ ಅಡ್ಡಸಾಲುಗಳು = ನಿಜ) ಸ್ಥಿರಾಂಕ
    {
        ಆದರೆ (!ಸ್ಥಿತಿ_2)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        ತಾರ್ಕಿಕ is1d = (flags & DFT_ಅಡ್ಡಸಾಲುಗಳು) != 0 || ಅಂಕೆ_dfts == 1;
        ತಾರ್ಕಿಕ inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು)
        {
            globalsize[0] = thread_ಎಣಿಕೆ; globalsize[1] = src.ಅಡ್ಡಸಾಲುಗಳು;
            localsize[0] = thread_ಎಣಿಕೆ; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            ಆದರೆ ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            globalsize[0] = ಅಂಕೆ_dfts; globalsize[1] = thread_ಎಣಿಕೆ;
            localsize[0] = 1; localsize[1] = thread_ಎಣಿಕೆ;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            ಆದರೆ (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        ಆದರೆ (!inv)
        {
            ಆದರೆ ((is1d && src.channels() == 1) || (ಅಡ್ಡಸಾಲುಗಳು && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            ಆದರೆ (dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        ಆದರೆ (k.empty())
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        k.ವಾದಗಳು(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(twiddles), thread_ಎಣಿಕೆ, ಅಂಕೆ_dfts);
        ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, localsize, ಸುಳ್ಳು);
    }

ಖಾಸಗಿ:
    ಸ್ಥಿರ ಶೂನ್ಯ ocl_getRadixes(ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, std::vector<ಪೂರ್ಣಾಂಕ>& radixes, std::vector<ಪೂರ್ಣಾಂಕ>& ಖಂಡಗಳು, ಪೂರ್ಣಾಂಕ& min_radix)
    {
        ಪೂರ್ಣಾಂಕ factors[34];
        ಪೂರ್ಣಾಂಕ nf = DFTFactorize(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, factors);

        ಪೂರ್ಣಾಂಕ n = 1;
        ಪೂರ್ಣಾಂಕ ಗುಣಕ_ಸೂಚ್ಯಂಕ = 0;
        min_radix = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;

        // 2^n transforms
        ಆದರೆ ((factors[ಗುಣಕ_ಸೂಚ್ಯಂಕ] & 1) == 0)
        {
            ಗಾಗಿ( ; n < factors[ಗುಣಕ_ಸೂಚ್ಯಂಕ];)
            {
                ಪೂರ್ಣಾಂಕ radix = 2, ಖಂಡ_2 = 1;
                ಆದರೆ (8*n <= factors[0])
                    radix = 8;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (4*n <= factors[0])
                {
                    radix = 4;
                    ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 12 == 0)
                        ಖಂಡ_2 = 3;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 8 == 0)
                        ಖಂಡ_2 = 2;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 10 == 0)
                        ಖಂಡ_2 = 5;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 8 == 0)
                        ಖಂಡ_2 = 4;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 6 == 0)
                        ಖಂಡ_2 = 3;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 4 == 0)
                        ಖಂಡ_2 = 2;
                }

                radixes.push_back(radix);
                ಖಂಡಗಳು.push_back(ಖಂಡ_2);
                min_radix = min(min_radix, ಖಂಡ_2*radix);
                n *= radix;
            }
            ಗುಣಕ_ಸೂಚ್ಯಂಕ++;
        }

        // all the other transforms
        ಗಾಗಿ( ; ಗುಣಕ_ಸೂಚ್ಯಂಕ < nf; ಗುಣಕ_ಸೂಚ್ಯಂಕ++)
        {
            ಪೂರ್ಣಾಂಕ radix = factors[ಗುಣಕ_ಸೂಚ್ಯಂಕ], ಖಂಡ_2 = 1;
            ಆದರೆ (radix == 3)
            {
                ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 12 == 0)
                    ಖಂಡ_2 = 4;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 9 == 0)
                    ಖಂಡ_2 = 3;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 6 == 0)
                    ಖಂಡ_2 = 2;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (radix == 5)
            {
                ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 10 == 0)
                    ಖಂಡ_2 = 2;
            }
            radixes.push_back(radix);
            ಖಂಡಗಳು.push_back(ಖಂಡ_2);
            min_radix = min(min_radix, ಖಂಡ_2*radix);
        }
    }

    ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
    ಸ್ಥಿರ ಶೂನ್ಯ fill_Radix_ಕೋಷ್ಟಕ(UMat twiddles, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        ಪೂರ್ಣಾಂಕ ptr_ಸೂಚ್ಯಂಕ = 0;

        ಪೂರ್ಣಾಂಕ n = 1;
        ಗಾಗಿ (size_t i=0; i<radixes.size(); i++)
        {
            ಪೂರ್ಣಾಂಕ radix = radixes[i];
            n *= radix;

            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j=1; j<radix; j++)
            {
                ದ್ವಿಗುಣ theta = -CV_2PI*j/n;

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k=0; k<(n/radix); k++)
                {
                    ptr[ptr_ಸೂಚ್ಯಂಕ++] = (T) ಕೋಜ್ಯಾ(k*theta);
                    ptr[ptr_ಸೂಚ್ಯಂಕ++] = (T) ಜ್ಯಾ(k*theta);
                }
            }
        }
    }
};

ವರ್ಗ OCL_FftPlanCache
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, ಹೊಸ OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(ಪೂರ್ಣಾಂಕ dft_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ ಆಳ)
    {
        ಪೂರ್ಣಾಂಕ ಕೀಲಿ = (dft_ಗಾತ್ರ << 16) | (ಆಳ & 0xFFFF);
        std::map<ಪೂರ್ಣಾಂಕ, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(ಕೀಲಿ);
        ಆದರೆ (f != planStorage.end())
        {
            ಹಿಂತಿರುಗಿ f->second;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(ಹೊಸ OCL_FftPlan(dft_ಗಾತ್ರ, ಆಳ));
            planStorage[ಕೀಲಿ] = newPlan;
            ಹಿಂತಿರುಗಿ newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

ಸಂರಕ್ಷಿತ:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<ಪೂರ್ಣಾಂಕ, Ptr<OCL_FftPlan> > planStorage;
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_dft_ಅಡ್ಡಸಾಲುಗಳು(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ fftType)
{
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(), ಆಳ);
    ಹಿಂತಿರುಗಿ plan->ಸರತಿಗೆ_ಸೇರಿಸು_Transform(_src, _dst, nonzero_ಅಡ್ಡಸಾಲುಗಳು, flags, fftType, ನಿಜ);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_dft_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ nonzero_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ fftType)
{
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.ಅಡ್ಡಸಾಲುಗಳು(), ಆಳ);
    ಹಿಂತಿರುಗಿ plan->ಸರತಿಗೆ_ಸೇರಿಸು_Transform(_src, _dst, nonzero_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, flags, fftType, ಸುಳ್ಳು);
}

ಸಾಲೊಳಗೆ FftType determineFFTType(ತಾರ್ಕಿಕ real_ಒಳಹರಿವು, ತಾರ್ಕಿಕ complex_ಒಳಹರಿವು, ತಾರ್ಕಿಕ real_output, ತಾರ್ಕಿಕ complex_output, ತಾರ್ಕಿಕ inv)
{
    // output format is not specified
    ಆದರೆ (!real_output && !complex_output)
        complex_output = ನಿಜ;

    // input or output format is ambiguous
    ಆದರೆ (real_ಒಳಹರಿವು == complex_ಒಳಹರಿವು || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType ಫಲಿತಾಂಶ = real_ಒಳಹರಿವು ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    ಆದರೆ (ಫಲಿತಾಂಶ == C2R && !inv)
        ಫಲಿತಾಂಶ = C2C;

    // Inverse CCS to Complex not supported
    ಆದರೆ (ಫಲಿತಾಂಶ == R2C && inv)
        ಫಲಿತಾಂಶ = R2R;

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_dft(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು)
{
    ಪೂರ್ಣಾಂಕ type = _src.type(), cn = CV_MAT_CN(type), ಆಳ = CV_MAT_ಆಳ(type);
    ಗಾತ್ರ_2 ssize = _src.size();
    ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ಆದರೆ (!(cn == 1 || cn == 2)
        || !(ಆಳ == CV_32F || (ಆಳ == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    ಆದರೆ (ssize.area() != getOptimalDFTSize(ssize.area()))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat();
    ತಾರ್ಕಿಕ inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    ಆದರೆ( nonzero_ಅಡ್ಡಸಾಲುಗಳು <= 0 || nonzero_ಅಡ್ಡಸಾಲುಗಳು > _src.ಅಡ್ಡಸಾಲುಗಳು() )
        nonzero_ಅಡ್ಡಸಾಲುಗಳು = _src.ಅಡ್ಡಸಾಲುಗಳು();
    ತಾರ್ಕಿಕ is1d = (flags & DFT_ಅಡ್ಡಸಾಲುಗಳು) != 0 || nonzero_ಅಡ್ಡಸಾಲುಗಳು == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    ಆದರೆ (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.create_Same_ಗಾತ್ರ(src, CV_MAKETYPE(ಆಳ, 2));
        output = _dst.getUMat();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // real output
        ಆದರೆ (is1d)
        {
            _dst.create_Same_ಗಾತ್ರ(src, CV_MAKETYPE(ಆಳ, 1));
            output = _dst.getUMat();
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            _dst.create_Same_ಗಾತ್ರ(src, CV_MAKETYPE(ಆಳ, 1));
            output.create(src.size, CV_MAKETYPE(ಆಳ, 2));
        }
    }

    ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
    ಆದರೆ (!inv)
    {
        ಪೂರ್ಣಾಂಕ nonzero_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = fftType == R2R ? output.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/2 + 1 : output.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ಫಲಿತಾಂಶ = ocl_dft_ಅಡ್ಡಸಾಲುಗಳು(src, output, nonzero_ಅಡ್ಡಸಾಲುಗಳು, flags, fftType);
        ಆದರೆ (!is1d)
            ಫಲಿತಾಂಶ = ಫಲಿತಾಂಶ && ocl_dft_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(output, _dst, nonzero_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, flags, fftType);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (fftType == C2C)
        {
            // complex output
            ಫಲಿತಾಂಶ = ocl_dft_ಅಡ್ಡಸಾಲುಗಳು(src, output, nonzero_ಅಡ್ಡಸಾಲುಗಳು, flags, fftType);
            ಆದರೆ (!is1d)
                ಫಲಿತಾಂಶ = ಫಲಿತಾಂಶ && ocl_dft_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(output, output, output.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, flags, fftType);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (is1d)
            {
                ಫಲಿತಾಂಶ = ocl_dft_ಅಡ್ಡಸಾಲುಗಳು(src, output, nonzero_ಅಡ್ಡಸಾಲುಗಳು, flags, fftType);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಪೂರ್ಣಾಂಕ nonzero_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/2 + 1;
                ಫಲಿತಾಂಶ = ocl_dft_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(src, output, nonzero_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, flags, fftType);
                ಫಲಿತಾಂಶ = ಫಲಿತಾಂಶ && ocl_dft_ಅಡ್ಡಸಾಲುಗಳು(output, _dst, nonzero_ಅಡ್ಡಸಾಲುಗಳು, flags, fftType);
            }
        }
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

} // namespace cv;

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CLAMDFFT

ನಾಮಾಕಾಶ cv {

#ವ್ಯಾಖ್ಯಾನಿಸು CLAMDDFT_Assert(func) \
    { \
        clfft_ಸ್ಥಿತಿ s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

ವರ್ಗ PlanCache
{
    ರಚನೆ FftPlan
    {
        FftPlan(ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_2 & dft_ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ src_ಹೆಜ್ಜೆ_2, ಪೂರ್ಣಾಂಕ dst_ಹೆಜ್ಜೆ_2, ತಾರ್ಕಿಕ _doubleFP, ತಾರ್ಕಿಕ _inplace, ಪೂರ್ಣಾಂಕ _flags, FftType _fftType) :
            dft_ಗಾತ್ರ(dft_ಗಾತ್ರ_2), src_ಹೆಜ್ಜೆ(src_ಹೆಜ್ಜೆ_2), dst_ಹೆಜ್ಜೆ(dst_ಹೆಜ್ಜೆ_2),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            ತಾರ್ಕಿಕ dft_inverse = (flags & DFT_INVERSE) != 0;
            ತಾರ್ಕಿಕ dft_scale = (flags & DFT_SCALE) != 0;
            ತಾರ್ಕಿಕ dft_ಅಡ್ಡಸಾಲುಗಳು = (flags & DFT_ಅಡ್ಡಸಾಲುಗಳು) != 0;

            clfftLayout ಒಳ_Layout = CLFFT_REAL, ಹೊರಹೋಗು_Layout = CLFFT_REAL;
            clfftDim dim = dft_ಗಾತ್ರ.ಎತ್ತರ == 1 || dft_ಅಡ್ಡಸಾಲುಗಳು ? CLFFT_1D : CLFFT_2D;

            size_t batch_ಗಾತ್ರ = dft_ಅಡ್ಡಸಾಲುಗಳು ? dft_ಗಾತ್ರ.ಎತ್ತರ : 1;
            size_t cl_Lengths_ಒಳ[3] = { (size_t)dft_ಗಾತ್ರ.ಅಗಲ, dft_ಅಡ್ಡಸಾಲುಗಳು ? 1 : (size_t)dft_ಗಾತ್ರ.ಎತ್ತರ, 1 };
            size_t cl_Strides_ಒಳ[3] = { 1, 1, 1 };
            size_t cl_Strides_ಹೊರಹೋಗು[3]  = { 1, 1, 1 };
            ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ = doubleFP ? ಗಾತ್ರ(ದ್ವಿಗುಣ) : ಗಾತ್ರ(ದಶಮಾಂಶ);

            ಆಯ್ಕೆ (fftType)
            {
            ಸ್ಥಿತಿ C2C:
                ಒಳ_Layout = CLFFT_COMPLEX_INTERLEAVED;
                ಹೊರಹೋಗು_Layout = CLFFT_COMPLEX_INTERLEAVED;
                cl_Strides_ಒಳ[1] = src_ಹೆಜ್ಜೆ / (elem_ಗಾತ್ರ << 1);
                cl_Strides_ಹೊರಹೋಗು[1] = dst_ಹೆಜ್ಜೆ / (elem_ಗಾತ್ರ << 1);
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ R2C:
                ಒಳ_Layout = CLFFT_REAL;
                ಹೊರಹೋಗು_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                cl_Strides_ಒಳ[1] = src_ಹೆಜ್ಜೆ / elem_ಗಾತ್ರ;
                cl_Strides_ಹೊರಹೋಗು[1] = dst_ಹೆಜ್ಜೆ / (elem_ಗಾತ್ರ << 1);
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ C2R:
                ಒಳ_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                ಹೊರಹೋಗು_Layout = CLFFT_REAL;
                cl_Strides_ಒಳ[1] = src_ಹೆಜ್ಜೆ / (elem_ಗಾತ್ರ << 1);
                cl_Strides_ಹೊರಹೋಗು[1] = dst_ಹೆಜ್ಜೆ / elem_ಗಾತ್ರ;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ R2R:
            ಪೂರ್ವನಿಯೋಜಿತ:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                ನಿಲ್ಲಿಸು;
            }

            cl_Strides_ಒಳ[2] = dft_ಅಡ್ಡಸಾಲುಗಳು ? cl_Strides_ಒಳ[1] : dft_ಗಾತ್ರ.ಅಗಲ * cl_Strides_ಒಳ[1];
            cl_Strides_ಹೊರಹೋಗು[2] = dft_ಅಡ್ಡಸಾಲುಗಳು ? cl_Strides_ಹೊರಹೋಗು[1] : dft_ಗಾತ್ರ.ಅಗಲ * cl_Strides_ಹೊರಹೋಗು[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, cl_Lengths_ಒಳ))

            // setting plan properties
            CLAMDDFT_Assert(clfft_ಗಣ_Plan_Precision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfft_ಗಣ_ಫಲಿತಾಂಶ_Location(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfft_ಗಣ_Layout(plHandle, ಒಳ_Layout, ಹೊರಹೋಗು_Layout))
            CLAMDDFT_Assert(clfft_ಗಣ_Plan_Batch_ಗಾತ್ರ(plHandle, batch_ಗಾತ್ರ))
            CLAMDDFT_Assert(clfft_ಗಣ_Plan_ಒಳ_Stride(plHandle, dim, cl_Strides_ಒಳ))
            CLAMDDFT_Assert(clfft_ಗಣ_Plan_ಹೊರಹೋಗು_Stride(plHandle, dim, cl_Strides_ಹೊರಹೋಗು))
            CLAMDDFT_Assert(clfft_ಗಣ_Plan_ದೂರ(plHandle, cl_Strides_ಒಳ[dim], cl_Strides_ಹೊರಹೋಗು[dim]))

            ದಶಮಾಂಶ scale = dft_scale ? 1.0f / (dft_ಅಡ್ಡಸಾಲುಗಳು ? dft_ಗಾತ್ರ.ಅಗಲ : dft_ಗಾತ್ರ.area()) : 1.0f;
            CLAMDDFT_Assert(clfft_ಗಣ_Plan_Scale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_ಸರತಿ ಸರತಿ_2 = (cl_command_ಸರತಿ)ocl::ಸರತಿ::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &ಸರತಿ_2, ಖಾಲಿ, ಖಾಲಿ))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        ಮಿತ್ರ ವರ್ಗ PlanCache;

    ಖಾಸಗಿ:
        ಗಾತ್ರ_2 dft_ಗಾತ್ರ;
        ಪೂರ್ಣಾಂಕ src_ಹೆಜ್ಜೆ, dst_ಹೆಜ್ಜೆ;
        ತಾರ್ಕಿಕ doubleFP;
        ತಾರ್ಕಿಕ inplace;
        ಪೂರ್ಣಾಂಕ flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, ಹೊಸ PlanCache())
    }

    clfftPlanHandle getPlanHandle(ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_2 & dft_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ src_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ dst_ಹೆಜ್ಜೆ, ತಾರ್ಕಿಕ doubleFP,
                                  ತಾರ್ಕಿಕ inplace, ಪೂರ್ಣಾಂಕ flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        ಗಾಗಿ (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            ಸ್ಥಿರಾಂಕ FftPlan * ಸ್ಥಿರಾಂಕ plan = planStorage[i];

            ಆದರೆ (plan->dft_ಗಾತ್ರ == dft_ಗಾತ್ರ &&
                plan->flags == flags &&
                plan->src_ಹೆಜ್ಜೆ == src_ಹೆಜ್ಜೆ &&
                plan->dst_ಹೆಜ್ಜೆ == dst_ಹೆಜ್ಜೆ &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                ಆದರೆ (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    ನಿಲ್ಲಿಸು;
                }

                ಹಿಂತಿರುಗಿ plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(ಹೊಸ FftPlan(dft_ಗಾತ್ರ, src_ಹೆಜ್ಜೆ, dst_ಹೆಜ್ಜೆ, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        ಹಿಂತಿರುಗಿ newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

ಸಂರಕ್ಷಿತ:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

ಬಾಹ್ಯ "C" {

ಸ್ಥಿರ ಶೂನ್ಯ CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, ಶೂನ್ಯ *p)
{
    UMat_ದತ್ತಾಂಶ * u = (UMat_ದತ್ತಾಂಶ *)p;

    ಆದರೆ( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_dft_amdfft(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ flags)
{
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type);
    ಗಾತ್ರ_2 ssize = _src.size();

    ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    ಆದರೆ ( (!doubleSupport && ಆಳ == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    ಆದರೆ (ssize.area() != getOptimalDFTSize(ssize.area()))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ dst_complex_ಒಳಹರಿವು = cn == 2 ? 1 : 0;
    ತಾರ್ಕಿಕ dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    ಪೂರ್ಣಾಂಕ dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    ತಾರ್ಕಿಕ dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_ಒಳಹರಿವು << 0 | dft_complex_output << 1);

    ಆಯ್ಕೆ (fftType)
    {
    ಸ್ಥಿತಿ C2C:
        _dst.create(ssize.ಎತ್ತರ, ssize.ಅಗಲ, CV_MAKE_TYPE(ಆಳ, 2));
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ R2C: // TODO implement it if possible
    ಸ್ಥಿತಿ C2R: // TODO implement it if possible
    ಸ್ಥಿತಿ R2R: // AMD Fft does not support this type
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    ಆದರೆ (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ತಾರ್ಕಿಕ inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (ಪೂರ್ಣಾಂಕ)src.ಹೆಜ್ಜೆ, (ಪೂರ್ಣಾಂಕ)dst.ಹೆಜ್ಜೆ,
                          ಆಳ == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t ಬಫರ್_ಗಾತ್ರ = 0;
    CLAMDDFT_Assert(clfft_Get_Tmp_Buf_ಗಾತ್ರ(plHandle, &ಬಫರ್_ಗಾತ್ರ))
    UMat tmp_ಬಫರ್(1, (ಪೂರ್ಣಾಂಕ)ಬಫರ್_ಗಾತ್ರ, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_ಸರತಿ ಸರತಿ_2 = (cl_command_ಸರತಿ)ocl::ಸರತಿ::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfft_ಸರತಿಗೆ_ಸೇರಿಸು_Transform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &ಸರತಿ_2, 0, ಖಾಲಿ, &e,
                                          &srcarg, &dstarg, (cl_mem)tmp_ಬಫರ್.handle(ACCESS_RW)))

    tmp_ಬಫರ್.addref();
    cl_ಗಣ_Event_Callback(e, CL_COMPLETE, oclCleanupCallback, tmp_ಬಫರ್.u);
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DFT_ASSERT

}

#ಆದರೆ_ಅಂತ್ಯ // HAVE_CLAMDFFT

ನಾಮಾಕಾಶ cv
{

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಶೂನ್ಯ complementComplex(T * ptr, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = ಹೆಜ್ಜೆ/ಗಾತ್ರ(p0[0]);
    ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ complementComplexOutput(ಪೂರ್ಣಾಂಕ ಆಳ, uchar * ptr, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ dft_dims)
{
    ಆದರೆ( ಆಳ == CV_32F )
        complementComplex((ದಶಮಾಂಶ*)ptr, ಹೆಜ್ಜೆ, ಎಣಿಕೆ_3, len, dft_dims);
    ಇಲ್ಲದಿದ್ದರೆ
        complementComplex((ದ್ವಿಗುಣ*)ptr, ಹೆಜ್ಜೆ, ಎಣಿಕೆ_3, len, dft_dims);
}

ಎಣಿಕೆ Dft_ಬಹುಲಕ {
    InvalidDft = 0,
    Fwd_Real_ಗಮ್ಯ_CCS,
    Fwd_Real_ಗಮ್ಯ_Complex,
    Fwd_Complex_ಗಮ್ಯ_Complex,
    InvCCSToReal,
    Inv_Complex_ಗಮ್ಯ_Real,
    Inv_Complex_ಗಮ್ಯ_Complex,
};

ಎಣಿಕೆ DftDims {
    InvalidDim = 0,
    OneDim,
    One_Dim_ಲಂಬಸಾಲು_Wise,
    TwoDims
};

ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಬಹುಲಕ_Name(Dft_ಬಹುಲಕ m)
{
    ಆಯ್ಕೆ (m)
    {
    ಸ್ಥಿತಿ InvalidDft: ಹಿಂತಿರುಗಿ "InvalidDft";
    ಸ್ಥಿತಿ Fwd_Real_ಗಮ್ಯ_CCS: ಹಿಂತಿರುಗಿ "FwdRealToCCS";
    ಸ್ಥಿತಿ Fwd_Real_ಗಮ್ಯ_Complex: ಹಿಂತಿರುಗಿ "FwdRealToComplex";
    ಸ್ಥಿತಿ Fwd_Complex_ಗಮ್ಯ_Complex: ಹಿಂತಿರುಗಿ "FwdComplexToComplex";
    ಸ್ಥಿತಿ InvCCSToReal: ಹಿಂತಿರುಗಿ "InvCCSToReal";
    ಸ್ಥಿತಿ Inv_Complex_ಗಮ್ಯ_Real: ಹಿಂತಿರುಗಿ "InvComplexToReal";
    ಸ್ಥಿತಿ Inv_Complex_ಗಮ್ಯ_Complex: ಹಿಂತಿರುಗಿ "InvComplexToComplex";
    }
    ಹಿಂತಿರುಗಿ 0;
}

ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * dimsName(DftDims d)
{
    ಆಯ್ಕೆ (d)
    {
    ಸ್ಥಿತಿ InvalidDim: ಹಿಂತಿರುಗಿ "InvalidDim";
    ಸ್ಥಿತಿ OneDim: ಹಿಂತಿರುಗಿ "OneDim";
    ಸ್ಥಿತಿ One_Dim_ಲಂಬಸಾಲು_Wise: ಹಿಂತಿರುಗಿ "OneDimColWise";
    ಸ್ಥಿತಿ TwoDims: ಹಿಂತಿರುಗಿ "TwoDims";
    };
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isInv(T ಬಹುಲಕ)
{
    ಆಯ್ಕೆ ((Dft_ಬಹುಲಕ)ಬಹುಲಕ)
    {
        ಸ್ಥಿತಿ InvCCSToReal:
        ಸ್ಥಿತಿ Inv_Complex_ಗಮ್ಯ_Real:
        ಸ್ಥಿತಿ Inv_Complex_ಗಮ್ಯ_Complex: ಹಿಂತಿರುಗಿ ನಿಜ;
        ಪೂರ್ವನಿಯೋಜಿತ: ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
}

ಸಾಲೊಳಗೆ Dft_ಬಹುಲಕ determine_ಬಹುಲಕ(ತಾರ್ಕಿಕ inv, ಪೂರ್ಣಾಂಕ cn1, ಪೂರ್ಣಾಂಕ cn2)
{
    ಆದರೆ (!inv)
    {
        ಆದರೆ (cn1 == 1 && cn2 == 1)
            ಹಿಂತಿರುಗಿ Fwd_Real_ಗಮ್ಯ_CCS;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn1 == 1 && cn2 == 2)
            ಹಿಂತಿರುಗಿ Fwd_Real_ಗಮ್ಯ_Complex;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn1 == 2 && cn2 == 2)
            ಹಿಂತಿರುಗಿ Fwd_Complex_ಗಮ್ಯ_Complex;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (cn1 == 1 && cn2 == 1)
            ಹಿಂತಿರುಗಿ InvCCSToReal;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn1 == 2 && cn2 == 1)
            ಹಿಂತಿರುಗಿ Inv_Complex_ಗಮ್ಯ_Real;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn1 == 2 && cn2 == 2)
            ಹಿಂತಿರುಗಿ Inv_Complex_ಗಮ್ಯ_Complex;
    }
    ಹಿಂತಿರುಗಿ InvalidDft;
}


ಸಾಲೊಳಗೆ DftDims determineDims(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ತಾರ್ಕಿಕ is_ಅಡ್ಡಸಾಲು_Wise, ತಾರ್ಕಿಕ isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    ಆದರೆ (is_ಅಡ್ಡಸಾಲು_Wise)
        ಹಿಂತಿರುಗಿ OneDim;
    ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 && ಅಡ್ಡಸಾಲುಗಳು > 1) // one-column-shaped input
    {
        ಆದರೆ (isContinuous)
            ಹಿಂತಿರುಗಿ OneDim;
        ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ One_Dim_ಲಂಬಸಾಲು_Wise;
    }
    ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು == 1)
        ಹಿಂತಿರುಗಿ OneDim;
    ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ > 1 && ಅಡ್ಡಸಾಲುಗಳು > 1)
        ಹಿಂತಿರುಗಿ TwoDims;
    ಹಿಂತಿರುಗಿ InvalidDim;
}

ವರ್ಗ OcvDftBasicImpl CV_FINAL : ಸಾರ್ವಜನಿಕ hal::DFT1D
{
ಸಾರ್ವಜನಿಕ:
    OcvDftOptions opt;
    // the plan is built once in init(), the workspace is allocated once here and reused by
    // every apply() (zero allocations per transform)
    DftPlan plan;
    Auto_ಬಫರ್<uchar> ws;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
    Auto_ಬಫರ್<uchar> ippbuf;
    Auto_ಬಫರ್<uchar> ippworkbuf;
    size_t ipp_worksize;
#ಆದರೆ_ಅಂತ್ಯ

ಸಾರ್ವಜನಿಕ:
    OcvDftBasicImpl()
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
        ipp_worksize = 0;
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಶೂನ್ಯ init(ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ flags, ತಾರ್ಕಿಕ *need_ಬಫರ್)
    {
        ಪೂರ್ಣಾಂಕ stage = (flags & CV_HAL_DFT_STAGE_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        ತಾರ್ಕಿಕ real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        ತಾರ್ಕಿಕ needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = ಸುಳ್ಳು;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        ಆದರೆ( CV_IPP_CHECK_COND && (opt.n*ಎಣಿಕೆ_3 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            ಪೂರ್ಣಾಂಕ ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            ಪೂರ್ಣಾಂಕ specsize=0, initsize=0, worksize=0;
            Ipp_DFTGet_ಗಾತ್ರ_Func get_ಗಾತ್ರ_Func = 0;
            IppDFTInitFunc initFunc = 0;

            ಆದರೆ( real_transform && stage == 0 )
            {
                ಆದರೆ( ಆಳ == CV_32F )
                {
                    get_ಗಾತ್ರ_Func = ipps_DFTGet_ಗಾತ್ರ_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    get_ಗಾತ್ರ_Func = ipps_DFTGet_ಗಾತ್ರ_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ( ಆಳ == CV_32F )
                {
                    get_ಗಾತ್ರ_Func = ipps_DFTGet_ಗಾತ್ರ_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    get_ಗಾತ್ರ_Func = ipps_DFTGet_ಗಾತ್ರ_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            ಆದರೆ( get_ಗಾತ್ರ_Func(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                ಆದರೆ( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = ನಿಜ;
            }
            ಇಲ್ಲದಿದ್ದರೆ
                ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
        }
    #ಆದರೆ_ಅಂತ್ಯ

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            ಸ್ಥಿರಾಂಕ DFTKernels& kernels = getDFTKernels(ಆಳ);
            ಪೂರ್ಣಾಂಕ ಬಗೆ = stage == 0 && real_transform ? (opt.isInverse ? DFT_ಬಗೆ_C2_R : DFT_ಬಗೆ_R2_C)
                                                    : DFT_ಬಗೆ_C2_C;
            plan.build(ಬಗೆ, len, ಆಳ, kernels.vlanes);
            ws.allocate(plan.ws_ಬೈಟುಗಳು);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.ದತ್ತಾಂಶ();
            ಆದರೆ (need_ಬಫರ್)
                *need_ಬಫರ್ = ಸುಳ್ಳು;   // the engine supports in-place for every kind
        }

        {
            ಸ್ಥಿರ DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<ದಶಮಾಂಶ>>,
                dftWrap<ದಶಮಾಂಶ, RealDFT<ದಶಮಾಂಶ>>,
                dftWrap<ದಶಮಾಂಶ, CCSIDFT<ದಶಮಾಂಶ>>,
                dftWrap<Complexd, DFT<ದ್ವಿಗುಣ>>,
                dftWrap<ದ್ವಿಗುಣ, RealDFT<ದ್ವಿಗುಣ>>,
                dftWrap<ದ್ವಿಗುಣ, CCSIDFT<ದ್ವಿಗುಣ>>
            };
            ಪೂರ್ಣಾಂಕ idx = 0;
            ಆದರೆ (stage == 0)
            {
                ಆದರೆ (real_transform)
                {
                    ಆದರೆ (!opt.isInverse)
                        idx = 1;
                    ಇಲ್ಲದಿದ್ದರೆ
                        idx = 2;
                }
            }
            ಆದರೆ (ಆಳ == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        ಆದರೆ(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು_ಎಣಿಕೆ = ಎಣಿಕೆ_3;
            ಆದರೆ (stage == 0 && (flags & CV_HAL_DFT_ಅಡ್ಡಸಾಲುಗಳು) != 0)
                ಅಡ್ಡಸಾಲು_ಎಣಿಕೆ = 1;
            opt.scale = 1./(len * ಅಡ್ಡಸಾಲು_ಎಣಿಕೆ);
        }
    }

    ಶೂನ್ಯ apply(ಸ್ಥಿರಾಂಕ uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspace_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
    {
        size_t sz = opt.plan ? plan.ws_ಬೈಟುಗಳು : 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
        ಆದರೆ (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#ಆದರೆ_ಅಂತ್ಯ
        ಹಿಂತಿರುಗಿ sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    ಶೂನ್ಯ applyWithWorkspace(ಸ್ಥಿರಾಂಕ uchar *src, uchar *dst, uchar* workspace) ಸ್ಥಿರಾಂಕ
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_IPP_DFT
        ಆದರೆ (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#ಆದರೆ_ಅಂತ್ಯ
        opt_.dft_func(opt_, src, dst);
    }

    ಶೂನ್ಯ ಬಿಡುಗಡೆ() {}
};

// 2D transforms (and the multi-row 1D batches) distribute the rows / the column pairs over
// threads. The per-thread state is the 1D workspace (+ the column buffers); the plans are shared.
// The number of stripes given to parallel_for_ is the estimated cost of the pass
// (count * len * log2(len), i.e. N log N) divided by an empirically chosen quantum: a 256x256
// pass gets 1-2 stripes, 512x512 ~5, 1024x1024 ~20.
// The column pass of the 2D transforms copies the columns into contiguous buffers, several at a
// time, row by row: every source/destination row is touched once per batch and the strided
// accesses do not alias in the cache (which they do when columns are copied one or two at a
// time and the row step is a multiple of a page). The batch is sized so that the column buffers
// stay within DFT_COLS_BATCH_BYTES (~L1-resident), between DFT_COLS_BATCH_MIN and _MAX columns.
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ DFT_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_BATCH_MIN = 4;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ DFT_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_BATCH_MAX = 16;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t DFT_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_BATCH_ಬೈಟುಗಳು = 1 << 17;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dft_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_Batch(ಪೂರ್ಣಾಂಕ len, size_t complex_elem_ಗಾತ್ರ)
{
    ಪೂರ್ಣಾಂಕ batch = (ಪೂರ್ಣಾಂಕ)(DFT_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_BATCH_ಬೈಟುಗಳು/((size_t)len*complex_elem_ಗಾತ್ರ));
    ಹಿಂತಿರುಗಿ std::max(DFT_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_BATCH_MIN, std::min(DFT_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_BATCH_MAX, batch));
}

ಸ್ಥಿರ ದ್ವಿಗುಣ dftParallelStripes(ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ len)
{
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ ಕಾಲಾಂಶ = 0.5e6;
    ಹಿಂತಿರುಗಿ (ದ್ವಿಗುಣ)ಎಣಿಕೆ_3*len*std::max(std::ಲಘುಗಣಕ((ದ್ವಿಗುಣ)len)/std::ಲಘುಗಣಕ(2.), 1.)/ಕಾಲಾಂಶ;
}

ವರ್ಗ OcvDftImpl CV_FINAL : ಸಾರ್ವಜನಿಕ hal::DFT2D
{
ಸಂರಕ್ಷಿತ:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    ಸ್ಥಿರಾಂಕ OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    ಸ್ಥಿರಾಂಕ OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    ತಾರ್ಕಿಕ need_ಬಫರ್_A;
    ತಾರ್ಕಿಕ need_ಬಫರ್_B;
    ತಾರ್ಕಿಕ inv;
    ಪೂರ್ಣಾಂಕ ಅಗಲ;
    ಪೂರ್ಣಾಂಕ ಎತ್ತರ;
    Dft_ಬಹುಲಕ ಬಹುಲಕ;
    ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ_2;
    ಪೂರ್ಣಾಂಕ complex_elem_ಗಾತ್ರ;
    ಪೂರ್ಣಾಂಕ ಆಳ;
    ತಾರ್ಕಿಕ real_transform;
    ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು;
    ತಾರ್ಕಿಕ is_ಅಡ್ಡಸಾಲು_Transform;
    ತಾರ್ಕಿಕ isScaled;
    std::vector<ಪೂರ್ಣಾಂಕ> stages;
    ತಾರ್ಕಿಕ useIpp;
    ಪೂರ್ಣಾಂಕ src_channels;
    ಪೂರ್ಣಾಂಕ dst_channels;

    Auto_ಬಫರ್<uchar> tmp_bufA;
    Auto_ಬಫರ್<uchar> tmp_bufB;
    Auto_ಬಫರ್<uchar> buf0;
    Auto_ಬಫರ್<uchar> buf1;

ಸಾರ್ವಜನಿಕ:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        need_ಬಫರ್_A = ಸುಳ್ಳು;
        need_ಬಫರ್_B = ಸುಳ್ಳು;
        inv = ಸುಳ್ಳು;
        ಅಗಲ = 0;
        ಎತ್ತರ = 0;
        ಬಹುಲಕ = InvalidDft;
        elem_ಗಾತ್ರ_2 = 0;
        complex_elem_ಗಾತ್ರ = 0;
        ಆಳ = 0;
        real_transform = ಸುಳ್ಳು;
        nonzero_ಅಡ್ಡಸಾಲುಗಳು = 0;
        is_ಅಡ್ಡಸಾಲು_Transform = ಸುಳ್ಳು;
        isScaled = ಸುಳ್ಳು;
        useIpp = ಸುಳ್ಳು;
        src_channels = 0;
        dst_channels = 0;
    }

    ಶೂನ್ಯ init(ಪೂರ್ಣಾಂಕ ಅಗಲ_3, ಪೂರ್ಣಾಂಕ ಎತ್ತರ_3, ಪೂರ್ಣಾಂಕ ಆಳ_2, ಪೂರ್ಣಾಂಕ _src_channels, ಪೂರ್ಣಾಂಕ _dst_channels, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು_2)
    {
        ತಾರ್ಕಿಕ isComplex = _src_channels != _dst_channels;
        nonzero_ಅಡ್ಡಸಾಲುಗಳು = nonzero_ಅಡ್ಡಸಾಲುಗಳು_2;
        ಅಗಲ = ಅಗಲ_3;
        ಎತ್ತರ = ಎತ್ತರ_3;
        ಆಳ = ಆಳ_2;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        ತಾರ್ಕಿಕ isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        ತಾರ್ಕಿಕ isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        ತಾರ್ಕಿಕ isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        ಬಹುಲಕ = determine_ಬಹುಲಕ(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        is_ಅಡ್ಡಸಾಲು_Transform = (flags & CV_HAL_DFT_ಅಡ್ಡಸಾಲುಗಳು) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        need_ಬಫರ್_A = ಸುಳ್ಳು;
        need_ಬಫರ್_B = ಸುಳ್ಳು;
        real_transform = (ಬಹುಲಕ != Fwd_Complex_ಗಮ್ಯ_Complex && ಬಹುಲಕ != Inv_Complex_ಗಮ್ಯ_Complex);

        elem_ಗಾತ್ರ_2 = (ಆಳ == CV_32F) ? ಗಾತ್ರ(ದಶಮಾಂಶ) : ಗಾತ್ರ(ದ್ವಿಗುಣ);
        complex_elem_ಗಾತ್ರ = elem_ಗಾತ್ರ_2 * 2;
        ಆದರೆ( !real_transform )
            elem_ಗಾತ್ರ_2 = complex_elem_ಗಾತ್ರ;

#ಆದರೆ defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            ಆದರೆ (nonzero_ಅಡ್ಡಸಾಲುಗಳು == 0 && ಆಳ == CV_32F && ((ಅಗಲ * ಎತ್ತರ)>(ಪೂರ್ಣಾಂಕ)(1<<6)))
            {
                ಆದರೆ (ಬಹುಲಕ == Fwd_Complex_ಗಮ್ಯ_Complex || ಬಹುಲಕ == Inv_Complex_ಗಮ್ಯ_Complex || ಬಹುಲಕ == Fwd_Real_ಗಮ್ಯ_CCS || ಬಹುಲಕ == InvCCSToReal)
                {
                    useIpp = ನಿಜ;
                    ಹಿಂತಿರುಗಿ;
                }
            }
        }
#ಆದರೆ_ಅಂತ್ಯ

        DftDims dims = determineDims(ಎತ್ತರ, ಅಗಲ, is_ಅಡ್ಡಸಾಲು_Transform, isContinuous);
        ಆದರೆ (dims == TwoDims)
        {
            stages.resize(2);
            ಆದರೆ (ಬಹುಲಕ == InvCCSToReal || ಬಹುಲಕ == Inv_Complex_ಗಮ್ಯ_Real)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            stages.resize(1);
            ಆದರೆ (dims == One_Dim_ಲಂಬಸಾಲು_Wise)
                stages[0] = 1;
            ಇಲ್ಲದಿದ್ದರೆ
                stages[0] = 0;
        }

        ಗಾಗಿ(uint stage_ಸೂಚ್ಯಂಕ = 0; stage_ಸೂಚ್ಯಂಕ < stages.size(); ++stage_ಸೂಚ್ಯಂಕ)
        {
            ಆದರೆ (stage_ಸೂಚ್ಯಂಕ == 1)
            {
                isInplace = ನಿಜ;
                isComplex = ಸುಳ್ಳು;
            }

            ಪೂರ್ಣಾಂಕ stage = stages[stage_ಸೂಚ್ಯಂಕ];
            ತಾರ್ಕಿಕ is_ಕೊನೆಯ_Stage = (stage_ಸೂಚ್ಯಂಕ + 1 == stages.size());

            ಪೂರ್ಣಾಂಕ len, ಎಣಿಕೆ_3;

            ಪೂರ್ಣಾಂಕ f = 0;
            ಆದರೆ (inv)
                f |= CV_HAL_DFT_INVERSE;
            ಆದರೆ (isScaled)
                f |= CV_HAL_DFT_SCALE;
            ಆದರೆ (is_ಅಡ್ಡಸಾಲು_Transform)
                f |= CV_HAL_DFT_ಅಡ್ಡಸಾಲುಗಳು;
            ಆದರೆ (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            ಆದರೆ (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            ಆದರೆ (!is_ಕೊನೆಯ_Stage)
                f |= CV_HAL_DFT_TWO_STAGE;

            ಆದರೆ( stage == 0 ) // row-wise transform
            {
                ಆದರೆ (ಅಗಲ == 1 && !is_ಅಡ್ಡಸಾಲು_Transform )
                {
                    len = ಎತ್ತರ;
                    ಎಣಿಕೆ_3 = ಅಗಲ;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    len = ಅಗಲ;
                    ಎಣಿಕೆ_3 = ಎತ್ತರ;
                }
                need_ಬಫರ್_A = isInplace;
                contextA = hal::DFT1D::create(len, ಎಣಿಕೆ_3, ಆಳ, f, &need_ಬಫರ್_A);
                basicA = dynamic_cast<ಸ್ಥಿರಾಂಕ OcvDftBasicImpl*>(contextA.get());
                ಆದರೆ (need_ಬಫರ್_A)
                    tmp_bufA.allocate(len * complex_elem_ಗಾತ್ರ);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                len = ಎತ್ತರ;
                ಎಣಿಕೆ_3 = ಅಗಲ;
                f |= CV_HAL_DFT_STAGE_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
                need_ಬಫರ್_B = isInplace;
                contextB = hal::DFT1D::create(len, ಎಣಿಕೆ_3, ಆಳ, f, &need_ಬಫರ್_B);
                basicB = dynamic_cast<ಸ್ಥಿರಾಂಕ OcvDftBasicImpl*>(contextB.get());
                ಆದರೆ (need_ಬಫರ್_B)
                    tmp_bufB.allocate(len * complex_elem_ಗಾತ್ರ);

                buf0.allocate(len * complex_elem_ಗಾತ್ರ);
                buf1.allocate(len * complex_elem_ಗಾತ್ರ);
            }
        }
    }

    ಶೂನ್ಯ apply(ಸ್ಥಿರಾಂಕ uchar * src, size_t src_ಹೆಜ್ಜೆ, uchar * dst, size_t dst_ಹೆಜ್ಜೆ) CV_OVERRIDE
    {
#ಆದರೆ defined USE_IPP_DFT
        ಆದರೆ (useIpp)
        {
            ಪೂರ್ಣಾಂಕ ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            ಆದರೆ (!is_ಅಡ್ಡಸಾಲು_Transform)
            {
                ಆದರೆ (ಬಹುಲಕ == Fwd_Complex_ಗಮ್ಯ_Complex || ಬಹುಲಕ == Inv_Complex_ಗಮ್ಯ_Complex)
                {
                    ಆದರೆ (ippi_DFT_C_32F(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, ಎತ್ತರ, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
                        ಹಿಂತಿರುಗಿ;
                    }
                    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬಹುಲಕ == Fwd_Real_ಗಮ್ಯ_CCS || ಬಹುಲಕ == InvCCSToReal)
                {
                    ಆದರೆ (ippi_DFT_R_32F(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, ಎತ್ತರ, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP);
                        ಹಿಂತಿರುಗಿ;
                    }
                    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ (ಬಹುಲಕ == Fwd_Complex_ಗಮ್ಯ_Complex || ಬಹುಲಕ == Inv_Complex_ಗಮ್ಯ_Complex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    ಆದರೆ (Dft_C_IPPLoop(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, ಎತ್ತರ, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP|CV_IMPL_MT);
                        ಹಿಂತಿರುಗಿ;
                    }
                    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬಹುಲಕ == Fwd_Real_ಗಮ್ಯ_CCS || ಬಹುಲಕ == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippi_DFTInv_Pack_ಗಮ್ಯ_R_32f_C1_R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    ಆದರೆ (Dft_R_IPPLoop(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, ಎತ್ತರ, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ಕೂಡಿಸು(CV_IMPL_IPP|CV_IMPL_MT);
                        ಹಿಂತಿರುಗಿ;
                    }
                    ಗಣ_Ipp_Error_ಸ್ಥಿತಿ();
                }
            }
            ಹಿಂತಿರುಗಿ;
        }
#ಆದರೆ_ಅಂತ್ಯ

        ಗಾಗಿ(uint stage_ಸೂಚ್ಯಂಕ = 0; stage_ಸೂಚ್ಯಂಕ < stages.size(); ++stage_ಸೂಚ್ಯಂಕ)
        {
            ಪೂರ್ಣಾಂಕ stage_src_channels = src_channels;
            ಪೂರ್ಣಾಂಕ stage_dst_channels = dst_channels;

            ಆದರೆ (stage_ಸೂಚ್ಯಂಕ == 1)
            {
                src = dst;
                src_ಹೆಜ್ಜೆ = dst_ಹೆಜ್ಜೆ;
                stage_src_channels = stage_dst_channels;
            }

            ಪೂರ್ಣಾಂಕ stage = stages[stage_ಸೂಚ್ಯಂಕ];
            ತಾರ್ಕಿಕ is_ಕೊನೆಯ_Stage = (stage_ಸೂಚ್ಯಂಕ + 1 == stages.size());
            ತಾರ್ಕಿಕ isComplex = stage_src_channels != stage_dst_channels;

            ಆದರೆ( stage == 0 )
                ಅಡ್ಡಸಾಲು_Dft(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, isComplex, is_ಕೊನೆಯ_Stage);
            ಇಲ್ಲದಿದ್ದರೆ
                ಲಂಬಸಾಲು_Dft(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, stage_src_channels, stage_dst_channels, is_ಕೊನೆಯ_Stage);
        }
    }

ಸಂರಕ್ಷಿತ:

    ಶೂನ್ಯ ಅಡ್ಡಸಾಲು_Dft(ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ತಾರ್ಕಿಕ isComplex, ತಾರ್ಕಿಕ is_ಕೊನೆಯ_Stage)
    {
        ಪೂರ್ಣಾಂಕ len, ಎಣಿಕೆ_3;
        ಆದರೆ (ಅಗಲ == 1 && !is_ಅಡ್ಡಸಾಲು_Transform )
        {
            len = ಎತ್ತರ;
            ಎಣಿಕೆ_3 = ಅಗಲ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            len = ಅಗಲ;
            ಎಣಿಕೆ_3 = ಎತ್ತರ;
        }
        ಪೂರ್ಣಾಂಕ dptr_ಸ್ಥಾನಾಂತರ = 0;
        ಪೂರ್ಣಾಂಕ dst_ತುಂಬಿದ_len = len*elem_ಗಾತ್ರ_2;

        ಆದರೆ( need_ಬಫರ್_A )
        {
            ಆದರೆ (ಬಹುಲಕ == Fwd_Real_ಗಮ್ಯ_CCS && (len & 1) && len > 1)
                dptr_ಸ್ಥಾನಾಂತರ = elem_ಗಾತ್ರ_2;
        }

        ಆದರೆ( !inv && isComplex )
            dst_ತುಂಬಿದ_len += (len & 1) ? elem_ಗಾತ್ರ_2 : complex_elem_ಗಾತ್ರ;

        ಪೂರ್ಣಾಂಕ nz = nonzero_ಅಡ್ಡಸಾಲುಗಳು;
        ಆದರೆ( nz <= 0 || nz > ಎಣಿಕೆ_3 )
            nz = ಎಣಿಕೆ_3;

        size_t ws_ಗಾತ್ರ = basicA ? basicA->workspace_ಗಾತ್ರ() : 0;
        ದ್ವಿಗುಣ nstripes = dftParallelStripes(nz, len);
        ಆದರೆ( ws_ಗಾತ್ರ > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            ಸ್ಥಿರಾಂಕ OcvDftBasicImpl* ctx = basicA;
            ಸ್ವಯಂಚಾಲಿತ process_ಅಡ್ಡಸಾಲುಗಳು = [&, ctx](ಸ್ಥಿರಾಂಕ Range& range)
            {
                Auto_ಬಫರ್<uchar> ws(ws_ಗಾತ್ರ);
                Auto_ಬಫರ್<uchar> rowbuf(need_ಬಫರ್_A ? len * complex_elem_ಗಾತ್ರ : 0);
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; i++ )
                {
                    ಸ್ಥಿರಾಂಕ uchar* sptr = src_ದತ್ತಾಂಶ + src_ಹೆಜ್ಜೆ * i;
                    uchar* dptr0 = dst_ದತ್ತಾಂಶ + dst_ಹೆಜ್ಜೆ * i;
                    uchar* dptr = need_ಬಫರ್_A ? rowbuf.ದತ್ತಾಂಶ() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.ದತ್ತಾಂಶ());
                    ಆದರೆ( need_ಬಫರ್_A )
                        ಸ್ಮರಣೆ_ನಕಲು( dptr0, dptr + dptr_ಸ್ಥಾನಾಂತರ, dst_ತುಂಬಿದ_len );
                }
            };
            parallel_for_(Range(0, nz), process_ಅಡ್ಡಸಾಲುಗಳು, nstripes);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nz; i++ )
            {
                ಸ್ಥಿರಾಂಕ uchar* sptr = src_ದತ್ತಾಂಶ + src_ಹೆಜ್ಜೆ * i;
                uchar* dptr0 = dst_ದತ್ತಾಂಶ + dst_ಹೆಜ್ಜೆ * i;
                uchar* dptr = dptr0;

                ಆದರೆ( need_ಬಫರ್_A )
                    dptr = tmp_bufA.ದತ್ತಾಂಶ();

                contextA->apply(sptr, dptr);

                ಆದರೆ( need_ಬಫರ್_A )
                    ಸ್ಮರಣೆ_ನಕಲು( dptr0, dptr + dptr_ಸ್ಥಾನಾಂತರ, dst_ತುಂಬಿದ_len );
            }
        }

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = nz; i < ಎಣಿಕೆ_3; i++ )
        {
            uchar* dptr0 = dst_ದತ್ತಾಂಶ + dst_ಹೆಜ್ಜೆ * i;
            memset( dptr0, 0, dst_ತುಂಬಿದ_len );
        }
        ಆದರೆ(is_ಕೊನೆಯ_Stage &&  ಬಹುಲಕ == Fwd_Real_ಗಮ್ಯ_Complex)
            complementComplexOutput(ಆಳ, dst_ದತ್ತಾಂಶ, dst_ಹೆಜ್ಜೆ, len, nz, 1);
    }

    ಶೂನ್ಯ ಲಂಬಸಾಲು_Dft(ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ stage_src_channels, ಪೂರ್ಣಾಂಕ stage_dst_channels, ತಾರ್ಕಿಕ is_ಕೊನೆಯ_Stage)
    {
        ಪೂರ್ಣಾಂಕ len = ಎತ್ತರ;
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = ಅಗಲ;
        ಪೂರ್ಣಾಂಕ a = 0, b = ಎಣಿಕೆ_3;
        uchar *dbuf0, *dbuf1;
        ಸ್ಥಿರಾಂಕ uchar* sptr0 = src_ದತ್ತಾಂಶ;
        uchar* dptr0 = dst_ದತ್ತಾಂಶ;

        dbuf0 = buf0.ದತ್ತಾಂಶ(), dbuf1 = buf1.ದತ್ತಾಂಶ();

        ಆದರೆ( need_ಬಫರ್_B )
        {
            dbuf1 = tmp_bufB.ದತ್ತಾಂಶ();
            dbuf0 = buf1.ದತ್ತಾಂಶ();
        }

        ಆದರೆ( real_transform )
        {
            ಪೂರ್ಣಾಂಕ even;
            a = 1;
            even = (ಎಣಿಕೆ_3 & 1) == 0;
            b = (ಎಣಿಕೆ_3+1)/2;
            ಆದರೆ( !inv )
            {
                memset( buf0.ದತ್ತಾಂಶ(), 0, len*complex_elem_ಗಾತ್ರ );
                Copy_ಸ್ತಂಭ( sptr0, src_ಹೆಜ್ಜೆ, buf0.ದತ್ತಾಂಶ(), complex_elem_ಗಾತ್ರ, len, elem_ಗಾತ್ರ_2 );
                sptr0 += stage_dst_channels*elem_ಗಾತ್ರ_2;
                ಆದರೆ( even )
                {
                    memset( buf1.ದತ್ತಾಂಶ(), 0, len*complex_elem_ಗಾತ್ರ );
                    Copy_ಸ್ತಂಭ( sptr0 + (ಎಣಿಕೆ_3-2)*elem_ಗಾತ್ರ_2, src_ಹೆಜ್ಜೆ,
                                buf1.ದತ್ತಾಂಶ(), complex_elem_ಗಾತ್ರ, len, elem_ಗಾತ್ರ_2 );
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( stage_src_channels == 1 )
            {
                Copy_ಸ್ತಂಭ( sptr0, src_ಹೆಜ್ಜೆ, buf0.ದತ್ತಾಂಶ(), elem_ಗಾತ್ರ_2, len, elem_ಗಾತ್ರ_2 );
                ExpandCCS( buf0.ದತ್ತಾಂಶ(), len, elem_ಗಾತ್ರ_2 );
                ಆದರೆ( even )
                {
                    Copy_ಸ್ತಂಭ( sptr0 + (ಎಣಿಕೆ_3-1)*elem_ಗಾತ್ರ_2, src_ಹೆಜ್ಜೆ,
                                buf1.ದತ್ತಾಂಶ(), elem_ಗಾತ್ರ_2, len, elem_ಗಾತ್ರ_2 );
                    ExpandCCS( buf1.ದತ್ತಾಂಶ(), len, elem_ಗಾತ್ರ_2 );
                }
                sptr0 += elem_ಗಾತ್ರ_2;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                Copy_ಸ್ತಂಭ( sptr0, src_ಹೆಜ್ಜೆ, buf0.ದತ್ತಾಂಶ(), complex_elem_ಗಾತ್ರ, len, complex_elem_ಗಾತ್ರ );
                ಆದರೆ( even )
                {
                    Copy_ಸ್ತಂಭ( sptr0 + b*complex_elem_ಗಾತ್ರ, src_ಹೆಜ್ಜೆ,
                                   buf1.ದತ್ತಾಂಶ(), complex_elem_ಗಾತ್ರ, len, complex_elem_ಗಾತ್ರ );
                }
                sptr0 += complex_elem_ಗಾತ್ರ;
            }

            ಆದರೆ( even )
                contextB->apply(buf1.ದತ್ತಾಂಶ(), dbuf1);
            contextB->apply(buf0.ದತ್ತಾಂಶ(), dbuf0);

            ಆದರೆ( stage_dst_channels == 1 )
            {
                ಆದರೆ( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    ಸ್ಮರಣೆ_ನಕಲು( dbuf0 + elem_ಗಾತ್ರ_2, dbuf0, elem_ಗಾತ್ರ_2 );
                    Copy_ಸ್ತಂಭ( dbuf0 + elem_ಗಾತ್ರ_2, elem_ಗಾತ್ರ_2, dptr0,
                                   dst_ಹೆಜ್ಜೆ, len, elem_ಗಾತ್ರ_2 );
                    ಆದರೆ( even )
                    {
                        ಸ್ಮರಣೆ_ನಕಲು( dbuf1 + elem_ಗಾತ್ರ_2, dbuf1, elem_ಗಾತ್ರ_2 );
                        Copy_ಸ್ತಂಭ( dbuf1 + elem_ಗಾತ್ರ_2, elem_ಗಾತ್ರ_2,
                                       dptr0 + (ಎಣಿಕೆ_3-1)*elem_ಗಾತ್ರ_2,
                                       dst_ಹೆಜ್ಜೆ, len, elem_ಗಾತ್ರ_2 );
                    }
                    dptr0 += elem_ಗಾತ್ರ_2;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    // copy the real part of the complex vector to the first/last column
                    Copy_ಸ್ತಂಭ( dbuf0, complex_elem_ಗಾತ್ರ, dptr0, dst_ಹೆಜ್ಜೆ, len, elem_ಗಾತ್ರ_2 );
                    ಆದರೆ( even )
                        Copy_ಸ್ತಂಭ( dbuf1, complex_elem_ಗಾತ್ರ, dptr0 + (ಎಣಿಕೆ_3-1)*elem_ಗಾತ್ರ_2,
                                       dst_ಹೆಜ್ಜೆ, len, elem_ಗಾತ್ರ_2 );
                    dptr0 += elem_ಗಾತ್ರ_2;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_Assert( !inv );
                Copy_ಸ್ತಂಭ( dbuf0, complex_elem_ಗಾತ್ರ, dptr0,
                               dst_ಹೆಜ್ಜೆ, len, complex_elem_ಗಾತ್ರ );
                ಆದರೆ( even )
                    Copy_ಸ್ತಂಭ( dbuf1, complex_elem_ಗಾತ್ರ,
                                   dptr0 + b*complex_elem_ಗಾತ್ರ,
                                   dst_ಹೆಜ್ಜೆ, len, complex_elem_ಗಾತ್ರ );
                dptr0 += complex_elem_ಗಾತ್ರ;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        ಪೂರ್ಣಾಂಕ ncols = b - a;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_batch = dft_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_Batch(len, complex_elem_ಗಾತ್ರ);
        ಪೂರ್ಣಾಂಕ nbatches = (ncols + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_batch - 1)/ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_batch;
        size_t ws_ಗಾತ್ರ = basicB ? basicB->workspace_ಗಾತ್ರ() : 0;
        size_t colbuf_ಗಾತ್ರ = (size_t)len*complex_elem_ಗಾತ್ರ;
        ಸ್ಥಿರಾಂಕ OcvDftBasicImpl* ctx = basicB;
        ಸ್ವಯಂಚಾಲಿತ processBatches = [&, ctx](ಸ್ಥಿರಾಂಕ Range& range)
        {
            Auto_ಬಫರ್<uchar> ws(ws_ಗಾತ್ರ);
            Auto_ಬಫರ್<uchar> cbuf(colbuf_ಗಾತ್ರ*ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_batch*(need_ಬಫರ್_B ? 2 : 1));
            uchar* cbuf0 = cbuf.ದತ್ತಾಂಶ();
            uchar* cdbuf0 = need_ಬಫರ್_B ? cbuf0 + colbuf_ಗಾತ್ರ*ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_batch : cbuf0;
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ bi = range.ಆರಂಭ; bi < range.end; bi++ )
            {
                ಪೂರ್ಣಾಂಕ c0 = bi*ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_batch;
                ಪೂರ್ಣಾಂಕ nc = std::min(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_batch, ncols - c0);
                ಸ್ಥಿರಾಂಕ uchar* sptr = sptr0 + (size_t)c0*complex_elem_ಗಾತ್ರ;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_ಗಾತ್ರ;
                CopyFromColumns( sptr, src_ಹೆಜ್ಜೆ, cbuf0, nc, len, complex_elem_ಗಾತ್ರ );
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < nc; k++ )
                {
                    ಆದರೆ( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_ಗಾತ್ರ, cdbuf0 + k*colbuf_ಗಾತ್ರ, ws.ದತ್ತಾಂಶ());
                    ಇಲ್ಲದಿದ್ದರೆ
                        contextB->apply(cbuf0 + k*colbuf_ಗಾತ್ರ, cdbuf0 + k*colbuf_ಗಾತ್ರ);
                }
                Copy_ಗಮ್ಯ_Columns( cdbuf0, dptr, dst_ಹೆಜ್ಜೆ, nc, len, complex_elem_ಗಾತ್ರ );
            }
        };
        ದ್ವಿಗುಣ nstripes = dftParallelStripes(ncols, len);
        ಆದರೆ( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        ಇಲ್ಲದಿದ್ದರೆ
            processBatches(Range(0, nbatches));
        ಆದರೆ(is_ಕೊನೆಯ_Stage && ಬಹುಲಕ == Fwd_Real_ಗಮ್ಯ_Complex)
            complementComplexOutput(ಆಳ, dst_ದತ್ತಾಂಶ, dst_ಹೆಜ್ಜೆ, ಎಣಿಕೆ_3, len, 2);
    }
};

ರಚನೆ ReplacementDFT1D : ಸಾರ್ವಜನಿಕ hal::DFT1D
{
    cvhalDFT *context;
    ತಾರ್ಕಿಕ isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(ಸುಳ್ಳು) {}
    ತಾರ್ಕಿಕ init(ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ flags, ತಾರ್ಕಿಕ *need_ಬಫರ್)
    {
        ಪೂರ್ಣಾಂಕ res = cv_hal_dftInit1D(&context, len, ಎಣಿಕೆ_3, ಆಳ, flags, need_ಬಫರ್);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ಹಿಂತಿರುಗಿ isInitialized;
    }
    ಶೂನ್ಯ apply(ಸ್ಥಿರಾಂಕ uchar *src, uchar *dst) CV_OVERRIDE
    {
        ಆದರೆ (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        ಆದರೆ (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

ರಚನೆ ReplacementDFT2D : ಸಾರ್ವಜನಿಕ hal::DFT2D
{
    cvhalDFT *context;
    ತಾರ್ಕಿಕ isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(ಸುಳ್ಳು) {}
    ತಾರ್ಕಿಕ init(ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ ಆಳ,
              ಪೂರ್ಣಾಂಕ src_channels, ಪೂರ್ಣಾಂಕ dst_channels,
              ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು)
    {
        ಪೂರ್ಣಾಂಕ res = cv_hal_dftInit2D(&context, ಅಗಲ, ಎತ್ತರ, ಆಳ, src_channels, dst_channels, flags, nonzero_ಅಡ್ಡಸಾಲುಗಳು);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ಹಿಂತಿರುಗಿ isInitialized;
    }
    ಶೂನ್ಯ apply(ಸ್ಥಿರಾಂಕ uchar *src, size_t src_ಹೆಜ್ಜೆ, uchar *dst, size_t dst_ಹೆಜ್ಜೆ) CV_OVERRIDE
    {
        ಆದರೆ (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ);
        }
    }
    ~ReplacementDFT2D()
    {
        ಆದರೆ (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

ನಾಮಾಕಾಶ hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ flags, ತಾರ್ಕಿಕ *need_ಬಫರ್)
{
    {
        ReplacementDFT1D *impl = ಹೊಸ ReplacementDFT1D();
        ಆದರೆ (impl->init(len, ಎಣಿಕೆ_3, ಆಳ, flags, need_ಬಫರ್))
        {
            ಹಿಂತಿರುಗಿ Ptr<DFT1D>(impl);
        }
        ಅಳಿಸು impl;
    }
    {
        OcvDftBasicImpl *impl = ಹೊಸ OcvDftBasicImpl();
        impl->init(len, ಎಣಿಕೆ_3, ಆಳ, flags, need_ಬಫರ್);
        ಹಿಂತಿರುಗಿ Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ ಆಳ,
                         ಪೂರ್ಣಾಂಕ src_channels, ಪೂರ್ಣಾಂಕ dst_channels,
                         ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು)
{
    {
        ReplacementDFT2D *impl = ಹೊಸ ReplacementDFT2D();
        ಆದರೆ (impl->init(ಅಗಲ, ಎತ್ತರ, ಆಳ, src_channels, dst_channels, flags, nonzero_ಅಡ್ಡಸಾಲುಗಳು))
        {
            ಹಿಂತಿರುಗಿ Ptr<DFT2D>(impl);
        }
        ಅಳಿಸು impl;
    }
    {
        ಆದರೆ(ಅಗಲ == 1 && nonzero_ಅಡ್ಡಸಾಲುಗಳು > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = ಹೊಸ OcvDftImpl();
        impl->init(ಅಗಲ, ಎತ್ತರ, ಆಳ, src_channels, dst_channels, flags, nonzero_ಅಡ್ಡಸಾಲುಗಳು);
        ಹಿಂತಿರುಗಿ Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


ಶೂನ್ಯ cv::dft( ಒಳಹರಿವು_Array _src0, OutputArray _dst, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು )
{
    CV_INSTRUMENT_REGION();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CLAMDFFT
    CV_OCL_ಸತತ(ocl::haveAmdFft() && ocl::Device::getDefault().type() != ocl::Device::TYPE_CPU &&
            _dst.isUMat() && _src0.dims() <= 2 && nonzero_ಅಡ್ಡಸಾಲುಗಳು == 0,
               ocl_dft_amdfft(_src0, _dst, flags))
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    CV_OCL_ಸತತ(_dst.isUMat() && _src0.dims() <= 2,
               ocl_dft(_src0, _dst, flags, nonzero_ಅಡ್ಡಸಾಲುಗಳು))
#ಆದರೆ_ಅಂತ್ಯ

    Mat src0 = _src0.getMat(), src = src0;
    ಆದರೆ (src.empty())
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }
    ತಾರ್ಕಿಕ inv = (flags & DFT_INVERSE) != 0;
    ಪೂರ್ಣಾಂಕ type = src.type();
    ಪೂರ್ಣಾಂಕ ಆಳ = src.ಆಳ();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_ಒಳಹರಿವು) && src.channels() != 2) );

    ಆದರೆ( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.create_Same_ಗಾತ್ರ( src, CV_MAKETYPE(ಆಳ, 2) );
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.create_Same_ಗಾತ್ರ( src, ಆಳ );
    ಇಲ್ಲದಿದ್ದರೆ
        _dst.create_Same_ಗಾತ್ರ( src, type );

    Mat dst = _dst.getMat();

    ಪೂರ್ಣಾಂಕ f = 0;
    ಆದರೆ (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    ಆದರೆ (inv)
        f |= CV_HAL_DFT_INVERSE;
    ಆದರೆ (flags & DFT_ಅಡ್ಡಸಾಲುಗಳು)
        f |= CV_HAL_DFT_ಅಡ್ಡಸಾಲುಗಳು;
    ಆದರೆ (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    ಆದರೆ (src.ದತ್ತಾಂಶ == dst.ದತ್ತಾಂಶ)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, ಆಳ, src.channels(), dst.channels(), f, nonzero_ಅಡ್ಡಸಾಲುಗಳು);
    c->apply(src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ);
}


ಶೂನ್ಯ cv::idft( ಒಳಹರಿವು_Array src, OutputArray dst, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_ಅಡ್ಡಸಾಲುಗಳು );
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_mulSpectrums( ಒಳಹರಿವು_Array _srcA, ಒಳಹರಿವು_Array _srcB,
                              OutputArray _dst, ಪೂರ್ಣಾಂಕ flags, ತಾರ್ಕಿಕ conjB )
{
    ಪೂರ್ಣಾಂಕ atype = _srcA.type(), btype = _srcB.type(),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ಗಾತ್ರ_2 asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    ಆದರೆ ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.create_Same_ಗಾತ್ರ(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(A), ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(B),
           ocl::KernelArg::WriteOnly(dst), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI);

    size_t globalsize[2] = { (size_t)asize.ಅಗಲ, ((size_t)asize.ಎತ್ತರ + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

}

#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ {

#ವ್ಯಾಖ್ಯಾನಿಸು VAL(buf, elem) (((T*)((ಅಕ್ಷರ*)ದತ್ತಾಂಶ ## buf + (ಹೆಜ್ಜೆ ## buf * (elem))))[0])
#ವ್ಯಾಖ್ಯಾನಿಸು MUL_SPECTRUMS_ಲಂಬಸಾಲು(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    ಗಾಗಿ (size_t j = 1; j <= ಅಡ್ಡಸಾಲುಗಳು - 2; j += 2) \
    { \
        ದ್ವಿಗುಣ a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        ದ್ವಿಗುಣ b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        ಆದರೆ (conjB) b_im = -b_im; \
        ದ್ವಿಗುಣ c_re = a_re * b_re - a_im * b_im; \
        ದ್ವಿಗುಣ c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    ಆದರೆ ((ಅಡ್ಡಸಾಲುಗಳು & 1) == 0) \
        VAL(C, ಅಡ್ಡಸಾಲುಗಳು-1) = VAL(A, ಅಡ್ಡಸಾಲುಗಳು-1) * VAL(B, ಅಡ್ಡಸಾಲುಗಳು-1)

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mul_Spectrums_process_ಲಂಬಸಾಲು_noinplace(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_A, ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_C, size_t ಹೆಜ್ಜೆ_A, size_t ಹೆಜ್ಜೆ_B, size_t ಹೆಜ್ಜೆ_C, size_t ಅಡ್ಡಸಾಲುಗಳು)
{
    MUL_SPECTRUMS_ಲಂಬಸಾಲು(A, B, C);
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mul_Spectrums_process_ಲಂಬಸಾಲು_inplace_A(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_AC, size_t ಹೆಜ್ಜೆ_B, size_t ಹೆಜ್ಜೆ_AC, size_t ಅಡ್ಡಸಾಲುಗಳು)
{
    MUL_SPECTRUMS_ಲಂಬಸಾಲು(AC, B, AC);
}
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB, ತಾರ್ಕಿಕ inplaceA> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mul_Spectrums_process_ಲಂಬಸಾಲು(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_A, ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_C, size_t ಹೆಜ್ಜೆ_A, size_t ಹೆಜ್ಜೆ_B, size_t ಹೆಜ್ಜೆ_C, size_t ಅಡ್ಡಸಾಲುಗಳು)
{
    ಆದರೆ (inplaceA)
        mul_Spectrums_process_ಲಂಬಸಾಲು_inplace_A<T, conjB>(ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, ಹೆಜ್ಜೆ_B, ಹೆಜ್ಜೆ_C, ಅಡ್ಡಸಾಲುಗಳು);
    ಇಲ್ಲದಿದ್ದರೆ
        mul_Spectrums_process_ಲಂಬಸಾಲು_noinplace<T, conjB>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, ಹೆಜ್ಜೆ_A, ಹೆಜ್ಜೆ_B, ಹೆಜ್ಜೆ_C, ಅಡ್ಡಸಾಲುಗಳು);
}
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MUL_SPECTRUMS_ಲಂಬಸಾಲು
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು VAL

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB, ತಾರ್ಕಿಕ inplaceA> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mul_Spectrums_process_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_A, ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_C, size_t ಹೆಜ್ಜೆ_A, size_t ಹೆಜ್ಜೆ_B, size_t ಹೆಜ್ಜೆ_C, size_t ಅಡ್ಡಸಾಲುಗಳು, size_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
{
    mul_Spectrums_process_ಲಂಬಸಾಲು<T, conjB, inplaceA>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, ಹೆಜ್ಜೆ_A, ಹೆಜ್ಜೆ_B, ಹೆಜ್ಜೆ_C, ಅಡ್ಡಸಾಲುಗಳು);
    ಆದರೆ ((ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ & 1) == 0)
    {
        mul_Spectrums_process_ಲಂಬಸಾಲು<T, conjB, inplaceA>(ದತ್ತಾಂಶ_A + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_B + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_C + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ಹೆಜ್ಜೆ_A, ಹೆಜ್ಜೆ_B, ಹೆಜ್ಜೆ_C, ಅಡ್ಡಸಾಲುಗಳು);
    }
}

#ವ್ಯಾಖ್ಯಾನಿಸು VAL(buf, elem) (ದತ್ತಾಂಶ ## buf[(elem)])
#ವ್ಯಾಖ್ಯಾನಿಸು MUL_SPECTRUMS_ಅಡ್ಡಸಾಲು(A, B, C) \
    ಗಾಗಿ (size_t j = j0; j < j1; j += 2) \
    { \
        ದ್ವಿಗುಣ a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        ದ್ವಿಗುಣ b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        ಆದರೆ (conjB) b_im = -b_im; \
        ದ್ವಿಗುಣ c_re = a_re * b_re - a_im * b_im; \
        ದ್ವಿಗುಣ c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mul_Spectrums_process_ಅಡ್ಡಸಾಲು_noinplace(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_A, ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_C, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ಅಡ್ಡಸಾಲು(A, B, C);
}
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mul_Spectrums_process_ಅಡ್ಡಸಾಲು_inplace_A(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_AC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ಅಡ್ಡಸಾಲು(AC, B, AC);
}
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB, ತಾರ್ಕಿಕ inplaceA> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mul_Spectrums_process_ಅಡ್ಡಸಾಲು(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_A, ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_C, size_t j0, size_t j1)
{
    ಆದರೆ (inplaceA)
        mul_Spectrums_process_ಅಡ್ಡಸಾಲು_inplace_A<T, conjB>(ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, j0, j1);
    ಇಲ್ಲದಿದ್ದರೆ
        mul_Spectrums_process_ಅಡ್ಡಸಾಲು_noinplace<T, conjB>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, j0, j1);
}
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MUL_SPECTRUMS_ಅಡ್ಡಸಾಲು
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು VAL

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB, ತಾರ್ಕಿಕ inplaceA> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mul_Spectrums_process_ಅಡ್ಡಸಾಲುಗಳು(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_A, ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_C, size_t ಹೆಜ್ಜೆ_A, size_t ಹೆಜ್ಜೆ_B, size_t ಹೆಜ್ಜೆ_C, size_t ಅಡ್ಡಸಾಲುಗಳು, size_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, size_t j0, size_t j1, ತಾರ್ಕಿಕ is_1d_CN1)
{
    ಆಗ (ಅಡ್ಡಸಾಲುಗಳು-- > 0)
    {
        ಆದರೆ (is_1d_CN1)
            ದತ್ತಾಂಶ_C[0] = ದತ್ತಾಂಶ_A[0]*ದತ್ತಾಂಶ_B[0];
        mul_Spectrums_process_ಅಡ್ಡಸಾಲು<T, conjB, inplaceA>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, j0, j1);
        ಆದರೆ (is_1d_CN1 && (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ & 1) == 0)
            ದತ್ತಾಂಶ_C[j1] = ದತ್ತಾಂಶ_A[j1]*ದತ್ತಾಂಶ_B[j1];

        ದತ್ತಾಂಶ_A = (ಸ್ಥಿರಾಂಕ T*)(((ಅಕ್ಷರ*)ದತ್ತಾಂಶ_A) + ಹೆಜ್ಜೆ_A);
        ದತ್ತಾಂಶ_B = (ಸ್ಥಿರಾಂಕ T*)(((ಅಕ್ಷರ*)ದತ್ತಾಂಶ_B) + ಹೆಜ್ಜೆ_B);
        ದತ್ತಾಂಶ_C =       (T*)(((ಅಕ್ಷರ*)ದತ್ತಾಂಶ_C) + ಹೆಜ್ಜೆ_C);
    }
}


ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB, ತಾರ್ಕಿಕ inplaceA> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mulSpectrums_Impl_(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_A, ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_C, size_t ಹೆಜ್ಜೆ_A, size_t ಹೆಜ್ಜೆ_B, size_t ಹೆಜ್ಜೆ_C, size_t ಅಡ್ಡಸಾಲುಗಳು, size_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, size_t j0, size_t j1, ತಾರ್ಕಿಕ is_1d, ತಾರ್ಕಿಕ isCN1)
{
    ಆದರೆ (!is_1d && isCN1)
    {
        mul_Spectrums_process_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ<T, conjB, inplaceA>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, ಹೆಜ್ಜೆ_A, ಹೆಜ್ಜೆ_B, ಹೆಜ್ಜೆ_C, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    }
    mul_Spectrums_process_ಅಡ್ಡಸಾಲುಗಳು<T, conjB, inplaceA>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, ಹೆಜ್ಜೆ_A, ಹೆಜ್ಜೆ_B, ಹೆಜ್ಜೆ_C, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, j0, j1, is_1d && isCN1);
}
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ತಾರ್ಕಿಕ conjB> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ mulSpectrums_Impl(ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_A, ಸ್ಥಿರಾಂಕ T* ದತ್ತಾಂಶ_B, T* ದತ್ತಾಂಶ_C, size_t ಹೆಜ್ಜೆ_A, size_t ಹೆಜ್ಜೆ_B, size_t ಹೆಜ್ಜೆ_C, size_t ಅಡ್ಡಸಾಲುಗಳು, size_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, size_t j0, size_t j1, ತಾರ್ಕಿಕ is_1d, ತಾರ್ಕಿಕ isCN1)
{
    ಆದರೆ (ದತ್ತಾಂಶ_A == ದತ್ತಾಂಶ_C)
        mulSpectrums_Impl_<T, conjB, ನಿಜ>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, ಹೆಜ್ಜೆ_A, ಹೆಜ್ಜೆ_B, ಹೆಜ್ಜೆ_C, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, j0, j1, is_1d, isCN1);
    ಇಲ್ಲದಿದ್ದರೆ
        mulSpectrums_Impl_<T, conjB, ಸುಳ್ಳು>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, ಹೆಜ್ಜೆ_A, ಹೆಜ್ಜೆ_B, ಹೆಜ್ಜೆ_C, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, j0, j1, is_1d, isCN1);
}

} // namespace

ಶೂನ್ಯ cv::mulSpectrums( ಒಳಹರಿವು_Array _srcA, ಒಳಹರಿವು_Array _srcB,
                       OutputArray _dst, ಪೂರ್ಣಾಂಕ flags, ತಾರ್ಕಿಕ conjB )
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }

    ಪೂರ್ಣಾಂಕ type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_ಸತತ(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    ಪೂರ್ಣಾಂಕ ಆಳ = srcA.ಆಳ(), cn = srcA.channels();
    size_t ಅಡ್ಡಸಾಲುಗಳು = srcA.ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = srcA.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

    _dst.create( srcA.ಅಡ್ಡಸಾಲುಗಳು, srcA.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    ಆದರೆ (dst.ದತ್ತಾಂಶ == srcB.ದತ್ತಾಂಶ)
        srcB = srcB.clone(); // workaround for B only

    ತಾರ್ಕಿಕ is_1d = (flags & DFT_ಅಡ್ಡಸಾಲುಗಳು)
        || (ಅಡ್ಡಸಾಲುಗಳು == 1)
        || (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    ಆದರೆ( is_1d && !(flags & DFT_ಅಡ್ಡಸಾಲುಗಳು) )
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + ಅಡ್ಡಸಾಲುಗಳು - 1, ಅಡ್ಡಸಾಲುಗಳು = 1;

    ತಾರ್ಕಿಕ isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*cn - (((ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ & 1) == 0 && cn == 1) ? 1 : 0);

    ಆದರೆ (ಆಳ == CV_32F)
    {
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ದತ್ತಾಂಶ_A = srcA.ptr<ದಶಮಾಂಶ>();
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ದತ್ತಾಂಶ_B = srcB.ptr<ದಶಮಾಂಶ>();
        ದಶಮಾಂಶ* ದತ್ತಾಂಶ_C = dst.ptr<ದಶಮಾಂಶ>();
        ಆದರೆ (!conjB)
            mulSpectrums_Impl<ದಶಮಾಂಶ, ಸುಳ್ಳು>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, srcA.ಹೆಜ್ಜೆ, srcB.ಹೆಜ್ಜೆ, dst.ಹೆಜ್ಜೆ, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, j0, j1, is_1d, isCN1);
        ಇಲ್ಲದಿದ್ದರೆ
            mulSpectrums_Impl<ದಶಮಾಂಶ, ನಿಜ>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, srcA.ಹೆಜ್ಜೆ, srcB.ಹೆಜ್ಜೆ, dst.ಹೆಜ್ಜೆ, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, j0, j1, is_1d, isCN1);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ದತ್ತಾಂಶ_A = srcA.ptr<ದ್ವಿಗುಣ>();
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ದತ್ತಾಂಶ_B = srcB.ptr<ದ್ವಿಗುಣ>();
        ದ್ವಿಗುಣ* ದತ್ತಾಂಶ_C = dst.ptr<ದ್ವಿಗುಣ>();
        ಆದರೆ (!conjB)
            mulSpectrums_Impl<ದ್ವಿಗುಣ, ಸುಳ್ಳು>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, srcA.ಹೆಜ್ಜೆ, srcB.ಹೆಜ್ಜೆ, dst.ಹೆಜ್ಜೆ, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, j0, j1, is_1d, isCN1);
        ಇಲ್ಲದಿದ್ದರೆ
            mulSpectrums_Impl<ದ್ವಿಗುಣ, ನಿಜ>(ದತ್ತಾಂಶ_A, ದತ್ತಾಂಶ_B, ದತ್ತಾಂಶ_C, srcA.ಹೆಜ್ಜೆ, srcB.ಹೆಜ್ಜೆ, dst.ಹೆಜ್ಜೆ, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, j0, j1, is_1d, isCN1);
    }
}

ಶೂನ್ಯ cv::divSpectrums( ಒಳಹರಿವು_Array _srcA, ಒಳಹರಿವು_Array _srcB, OutputArray _dst, ಪೂರ್ಣಾಂಕ flags, ತಾರ್ಕಿಕ conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    ಆದರೆ (srcA.empty() && srcB.empty())
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }
    ಪೂರ್ಣಾಂಕ ಆಳ = srcA.ಆಳ(), cn = srcA.channels(), type = srcA.type();
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು = srcA.ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = srcA.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಪೂರ್ಣಾಂಕ j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.ಅಡ್ಡಸಾಲುಗಳು, srcA.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.ದತ್ತಾಂಶ != srcA.ದತ್ತಾಂಶ); // non-inplace check
    CV_Assert(dst.ದತ್ತಾಂಶ != srcB.ದತ್ತಾಂಶ); // non-inplace check

    ತಾರ್ಕಿಕ is_1d = (flags & DFT_ಅಡ್ಡಸಾಲುಗಳು) || (ಅಡ್ಡಸಾಲುಗಳು == 1 || (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    ಆದರೆ( is_1d && !(flags & DFT_ಅಡ್ಡಸಾಲುಗಳು) )
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + ಅಡ್ಡಸಾಲುಗಳು - 1, ಅಡ್ಡಸಾಲುಗಳು = 1;

    ಪೂರ್ಣಾಂಕ ncols = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*cn;
    ಪೂರ್ಣಾಂಕ j0 = cn == 1;
    ಪೂರ್ಣಾಂಕ j1 = ncols - (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 2 == 0 && cn == 1);

    ಆದರೆ( ಆಳ == CV_32F )
    {
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ದತ್ತಾಂಶ_A = srcA.ptr<ದಶಮಾಂಶ>();
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ದತ್ತಾಂಶ_B = srcB.ptr<ದಶಮಾಂಶ>();
        ದಶಮಾಂಶ* ದತ್ತಾಂಶ_C = dst.ptr<ದಶಮಾಂಶ>();
        ದಶಮಾಂಶ eps = FLT_EPSILON; // prevent div0 problems

        size_t ಹೆಜ್ಜೆ_A = srcA.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ದತ್ತಾಂಶ_A[0]);
        size_t ಹೆಜ್ಜೆ_B = srcB.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ದತ್ತಾಂಶ_B[0]);
        size_t ಹೆಜ್ಜೆ_C = dst.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ದತ್ತಾಂಶ_C[0]);

        ಆದರೆ( !is_1d && cn == 1 )
        {
            ಗಾಗಿ( k = 0; k < (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 2 ? 1 : 2); k++ )
            {
                ಆದರೆ( k == 1 )
                    ದತ್ತಾಂಶ_A += ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_B += ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_C += ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1;
                ದತ್ತಾಂಶ_C[0] = ದತ್ತಾಂಶ_A[0] / (ದತ್ತಾಂಶ_B[0] + eps);
                ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು % 2 == 0 )
                    ದತ್ತಾಂಶ_C[(ಅಡ್ಡಸಾಲುಗಳು-1)*ಹೆಜ್ಜೆ_C] = ದತ್ತಾಂಶ_A[(ಅಡ್ಡಸಾಲುಗಳು-1)*ಹೆಜ್ಜೆ_A] / (ದತ್ತಾಂಶ_B[(ಅಡ್ಡಸಾಲುಗಳು-1)*ಹೆಜ್ಜೆ_B] + eps);
                ಆದರೆ( !conjB )
                    ಗಾಗಿ( j = 1; j <= ಅಡ್ಡಸಾಲುಗಳು - 2; j += 2 )
                    {
                        ದ್ವಿಗುಣ denom = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] +
                        (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B] + (ದ್ವಿಗುಣ)eps;

                        ದ್ವಿಗುಣ re = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] +
                        (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[(j+1)*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B];

                        ದ್ವಿಗುಣ im = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[(j+1)*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] -
                        (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B];

                        ದತ್ತಾಂಶ_C[j*ಹೆಜ್ಜೆ_C] = (ದಶಮಾಂಶ)(re / denom);
                        ದತ್ತಾಂಶ_C[(j+1)*ಹೆಜ್ಜೆ_C] = (ದಶಮಾಂಶ)(im / denom);
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                        ಗಾಗಿ( j = 1; j <= ಅಡ್ಡಸಾಲುಗಳು - 2; j += 2 )
                        {

                            ದ್ವಿಗುಣ denom = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] +
                            (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B] + (ದ್ವಿಗುಣ)eps;

                            ದ್ವಿಗುಣ re = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] -
                            (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[(j+1)*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B];

                            ದ್ವಿಗುಣ im = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[(j+1)*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] +
                            (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B];

                            ದತ್ತಾಂಶ_C[j*ಹೆಜ್ಜೆ_C] = (ದಶಮಾಂಶ)(re / denom);
                            ದತ್ತಾಂಶ_C[(j+1)*ಹೆಜ್ಜೆ_C] = (ದಶಮಾಂಶ)(im / denom);
                        }
                    ಆದರೆ( k == 1 )
                        ದತ್ತಾಂಶ_A -= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_B -= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_C -= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1;
            }
        }

        ಗಾಗಿ( ; ಅಡ್ಡಸಾಲುಗಳು--; ದತ್ತಾಂಶ_A += ಹೆಜ್ಜೆ_A, ದತ್ತಾಂಶ_B += ಹೆಜ್ಜೆ_B, ದತ್ತಾಂಶ_C += ಹೆಜ್ಜೆ_C )
        {
            ಆದರೆ( is_1d && cn == 1 )
            {
                ದತ್ತಾಂಶ_C[0] = ದತ್ತಾಂಶ_A[0] / (ದತ್ತಾಂಶ_B[0] + eps);
                ಆದರೆ( ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 2 == 0 )
                    ದತ್ತಾಂಶ_C[j1] = ದತ್ತಾಂಶ_A[j1] / (ದತ್ತಾಂಶ_B[j1] + eps);
            }

            ಆದರೆ( !conjB )
                ಗಾಗಿ( j = j0; j < j1; j += 2 )
                {
                    ದ್ವಿಗುಣ denom = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_B[j]*ದತ್ತಾಂಶ_B[j] + (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_B[j+1]*ದತ್ತಾಂಶ_B[j+1] + (ದ್ವಿಗುಣ)eps;
                    ದ್ವಿಗುಣ re = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j]*ದತ್ತಾಂಶ_B[j] + (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j+1]*ದತ್ತಾಂಶ_B[j+1];
                    ದ್ವಿಗುಣ im = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j+1]*ದತ್ತಾಂಶ_B[j] - (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j]*ದತ್ತಾಂಶ_B[j+1];
                    ದತ್ತಾಂಶ_C[j] = (ದಶಮಾಂಶ)(re / denom);
                    ದತ್ತಾಂಶ_C[j+1] = (ದಶಮಾಂಶ)(im / denom);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ಗಾಗಿ( j = j0; j < j1; j += 2 )
                    {
                        ದ್ವಿಗುಣ denom = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_B[j]*ದತ್ತಾಂಶ_B[j] + (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_B[j+1]*ದತ್ತಾಂಶ_B[j+1] + (ದ್ವಿಗುಣ)eps;
                        ದ್ವಿಗುಣ re = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j]*ದತ್ತಾಂಶ_B[j] - (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j+1]*ದತ್ತಾಂಶ_B[j+1];
                        ದ್ವಿಗುಣ im = (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j+1]*ದತ್ತಾಂಶ_B[j] + (ದ್ವಿಗುಣ)ದತ್ತಾಂಶ_A[j]*ದತ್ತಾಂಶ_B[j+1];
                        ದತ್ತಾಂಶ_C[j] = (ದಶಮಾಂಶ)(re / denom);
                        ದತ್ತಾಂಶ_C[j+1] = (ದಶಮಾಂಶ)(im / denom);
                    }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ದತ್ತಾಂಶ_A = srcA.ptr<ದ್ವಿಗುಣ>();
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ದತ್ತಾಂಶ_B = srcB.ptr<ದ್ವಿಗುಣ>();
        ದ್ವಿಗುಣ* ದತ್ತಾಂಶ_C = dst.ptr<ದ್ವಿಗುಣ>();
        ದ್ವಿಗುಣ eps = DBL_EPSILON; // prevent div0 problems

        size_t ಹೆಜ್ಜೆ_A = srcA.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ದತ್ತಾಂಶ_A[0]);
        size_t ಹೆಜ್ಜೆ_B = srcB.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ದತ್ತಾಂಶ_B[0]);
        size_t ಹೆಜ್ಜೆ_C = dst.ಹೆಜ್ಜೆ/ಗಾತ್ರ(ದತ್ತಾಂಶ_C[0]);

        ಆದರೆ( !is_1d && cn == 1 )
        {
            ಗಾಗಿ( k = 0; k < (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 2 ? 1 : 2); k++ )
            {
                ಆದರೆ( k == 1 )
                    ದತ್ತಾಂಶ_A += ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_B += ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_C += ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1;
                ದತ್ತಾಂಶ_C[0] = ದತ್ತಾಂಶ_A[0] / (ದತ್ತಾಂಶ_B[0] + eps);
                ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು % 2 == 0 )
                    ದತ್ತಾಂಶ_C[(ಅಡ್ಡಸಾಲುಗಳು-1)*ಹೆಜ್ಜೆ_C] = ದತ್ತಾಂಶ_A[(ಅಡ್ಡಸಾಲುಗಳು-1)*ಹೆಜ್ಜೆ_A] / (ದತ್ತಾಂಶ_B[(ಅಡ್ಡಸಾಲುಗಳು-1)*ಹೆಜ್ಜೆ_B] + eps);
                ಆದರೆ( !conjB )
                    ಗಾಗಿ( j = 1; j <= ಅಡ್ಡಸಾಲುಗಳು - 2; j += 2 )
                    {
                        ದ್ವಿಗುಣ denom = ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] +
                        ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B] + eps;

                        ದ್ವಿಗುಣ re = ದತ್ತಾಂಶ_A[j*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] +
                        ದತ್ತಾಂಶ_A[(j+1)*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B];

                        ದ್ವಿಗುಣ im = ದತ್ತಾಂಶ_A[(j+1)*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] -
                        ದತ್ತಾಂಶ_A[j*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B];

                        ದತ್ತಾಂಶ_C[j*ಹೆಜ್ಜೆ_C] = re / denom;
                        ದತ್ತಾಂಶ_C[(j+1)*ಹೆಜ್ಜೆ_C] = im / denom;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                        ಗಾಗಿ( j = 1; j <= ಅಡ್ಡಸಾಲುಗಳು - 2; j += 2 )
                        {
                            ದ್ವಿಗುಣ denom = ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] +
                            ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B] + eps;

                            ದ್ವಿಗುಣ re = ದತ್ತಾಂಶ_A[j*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] -
                            ದತ್ತಾಂಶ_A[(j+1)*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B];

                            ದ್ವಿಗುಣ im = ದತ್ತಾಂಶ_A[(j+1)*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[j*ಹೆಜ್ಜೆ_B] +
                            ದತ್ತಾಂಶ_A[j*ಹೆಜ್ಜೆ_A]*ದತ್ತಾಂಶ_B[(j+1)*ಹೆಜ್ಜೆ_B];

                            ದತ್ತಾಂಶ_C[j*ಹೆಜ್ಜೆ_C] = re / denom;
                            ದತ್ತಾಂಶ_C[(j+1)*ಹೆಜ್ಜೆ_C] = im / denom;
                        }
                    ಆದರೆ( k == 1 )
                        ದತ್ತಾಂಶ_A -= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_B -= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ದತ್ತಾಂಶ_C -= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1;
            }
        }

        ಗಾಗಿ( ; ಅಡ್ಡಸಾಲುಗಳು--; ದತ್ತಾಂಶ_A += ಹೆಜ್ಜೆ_A, ದತ್ತಾಂಶ_B += ಹೆಜ್ಜೆ_B, ದತ್ತಾಂಶ_C += ಹೆಜ್ಜೆ_C )
        {
            ಆದರೆ( is_1d && cn == 1 )
            {
                ದತ್ತಾಂಶ_C[0] = ದತ್ತಾಂಶ_A[0] / (ದತ್ತಾಂಶ_B[0] + eps);
                ಆದರೆ( ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ % 2 == 0 )
                    ದತ್ತಾಂಶ_C[j1] = ದತ್ತಾಂಶ_A[j1] / (ದತ್ತಾಂಶ_B[j1] + eps);
            }

            ಆದರೆ( !conjB )
                ಗಾಗಿ( j = j0; j < j1; j += 2 )
                {
                    ದ್ವಿಗುಣ denom = ದತ್ತಾಂಶ_B[j]*ದತ್ತಾಂಶ_B[j] + ದತ್ತಾಂಶ_B[j+1]*ದತ್ತಾಂಶ_B[j+1] + eps;
                    ದ್ವಿಗುಣ re = ದತ್ತಾಂಶ_A[j]*ದತ್ತಾಂಶ_B[j] + ದತ್ತಾಂಶ_A[j+1]*ದತ್ತಾಂಶ_B[j+1];
                    ದ್ವಿಗುಣ im = ದತ್ತಾಂಶ_A[j+1]*ದತ್ತಾಂಶ_B[j] - ದತ್ತಾಂಶ_A[j]*ದತ್ತಾಂಶ_B[j+1];
                    ದತ್ತಾಂಶ_C[j] = re / denom;
                    ದತ್ತಾಂಶ_C[j+1] = im / denom;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ಗಾಗಿ( j = j0; j < j1; j += 2 )
                    {
                        ದ್ವಿಗುಣ denom = ದತ್ತಾಂಶ_B[j]*ದತ್ತಾಂಶ_B[j] + ದತ್ತಾಂಶ_B[j+1]*ದತ್ತಾಂಶ_B[j+1] + eps;
                        ದ್ವಿಗುಣ re = ದತ್ತಾಂಶ_A[j]*ದತ್ತಾಂಶ_B[j] - ದತ್ತಾಂಶ_A[j+1]*ದತ್ತಾಂಶ_B[j+1];
                        ದ್ವಿಗುಣ im = ದತ್ತಾಂಶ_A[j+1]*ದತ್ತಾಂಶ_B[j] + ದತ್ತಾಂಶ_A[j]*ದತ್ತಾಂಶ_B[j+1];
                        ದತ್ತಾಂಶ_C[j] = re / denom;
                        ದತ್ತಾಂಶ_C[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
DCT( ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ T* src, size_t src_ಹೆಜ್ಜೆ, T* dst, size_t dst_ಹೆಜ್ಜೆ )
{
    ಆದರೆ( c.n == 1 )
    {
        dst[0] = src[0];
        ಹಿಂತಿರುಗಿ;
    }
    ಸತತ_Dft(c, src, src_ಹೆಜ್ಜೆ/ಗಾತ್ರ(T), dst, dst_ಹೆಜ್ಜೆ/ಗಾತ್ರ(T), ಸುಳ್ಳು);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
IDCT( ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ T* src, size_t src_ಹೆಜ್ಜೆ, T* dst, size_t dst_ಹೆಜ್ಜೆ )
{
    ಆದರೆ( c.n == 1 )
    {
        dst[0] = src[0];
        ಹಿಂತಿರುಗಿ;
    }
    ಸತತ_Dft(c, src, src_ಹೆಜ್ಜೆ/ಗಾತ್ರ(T), dst, dst_ಹೆಜ್ಜೆ/ಗಾತ್ರ(T), ಸುಳ್ಳು);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*DCTFunc)(ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t src_ಹೆಜ್ಜೆ,
                        ಶೂನ್ಯ* dst, size_t dst_ಹೆಜ್ಜೆ);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ OcvDftOptions&, ಸ್ಥಿರಾಂಕ T*, size_t, T*, size_t)>
ಸ್ಥಿರ ಶೂನ್ಯ dctWrap(ಸ್ಥಿರಾಂಕ OcvDftOptions & c, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t src_ಹೆಜ್ಜೆ, ಶೂನ್ಯ* dst, size_t dst_ಹೆಜ್ಜೆ)
{
    fn(c, (ಸ್ಥಿರಾಂಕ T*)src, src_ಹೆಜ್ಜೆ, (T*)dst, dst_ಹೆಜ್ಜೆ);
}

}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
ನಾಮಾಕಾಶ cv
{

#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 900
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * ippiDCTFunc)(ಸ್ಥಿರಾಂಕ Ipp32f* pSrc, ಪೂರ್ಣಾಂಕ src_ಹೆಜ್ಜೆ_3, Ipp32f* pDst, ಪೂರ್ಣಾಂಕ dst_ಹೆಜ್ಜೆ_3, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* pDCTSpec, Ipp8u* p_ಬಫರ್);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * ippiDCTInit)(ಶೂನ್ಯ* pDCTSpec, Ippi_ಗಾತ್ರ roi_ಗಾತ್ರ, Ipp8u* pMemInit );
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * ippi_DCTGet_ಗಾತ್ರ)(Ippi_ಗಾತ್ರ roi_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ* p_ಗಾತ್ರ_Spec, ಪೂರ್ಣಾಂಕ* p_ಗಾತ್ರ_Init, ಪೂರ್ಣಾಂಕ* p_ಗಾತ್ರ_Buf);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SpecType, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(ಸ್ಥಿರಾಂಕ Ipp32f*, ಪೂರ್ಣಾಂಕ, Ipp32f*, ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ SpecType*, Ipp8u*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ippiDCTWrap(ಸ್ಥಿರಾಂಕ Ipp32f* pSrc, ಪೂರ್ಣಾಂಕ src_ಹೆಜ್ಜೆ_3, Ipp32f* pDst, ಪೂರ್ಣಾಂಕ dst_ಹೆಜ್ಜೆ_3, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* pDCTSpec, Ipp8u* p_ಬಫರ್)
{
    ಹಿಂತಿರುಗಿ fn(pSrc, src_ಹೆಜ್ಜೆ_3, pDst, dst_ಹೆಜ್ಜೆ_3, (ಸ್ಥಿರಾಂಕ SpecType*)pDCTSpec, p_ಬಫರ್);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SpecType, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(SpecType*, Ippi_ಗಾತ್ರ, Ipp8u*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ippiDCTInitWrap(ಶೂನ್ಯ* pDCTSpec, Ippi_ಗಾತ್ರ roi_ಗಾತ್ರ, Ipp8u* pMemInit)
{
    ಹಿಂತಿರುಗಿ fn((SpecType*)pDCTSpec, roi_ಗಾತ್ರ, pMemInit);
}

ಮಾದರಿ<Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(Ippi_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ*, ಪೂರ್ಣಾಂಕ*, ಪೂರ್ಣಾಂಕ*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ippi_DCTGet_ಗಾತ್ರ_Wrap(Ippi_ಗಾತ್ರ roi_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ* p_ಗಾತ್ರ_Spec, ಪೂರ್ಣಾಂಕ* p_ಗಾತ್ರ_Init, ಪೂರ್ಣಾಂಕ* p_ಗಾತ್ರ_Buf)
{
    ಹಿಂತಿರುಗಿ fn(roi_ಗಾತ್ರ, p_ಗಾತ್ರ_Spec, p_ಗಾತ್ರ_Init, p_ಗಾತ್ರ_Buf);
}

#ಅಥವಾಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 700
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * ippiDCTFunc)(ಸ್ಥಿರಾಂಕ Ipp32f*, ಪೂರ್ಣಾಂಕ, Ipp32f*, ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, Ipp8u*);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * ippiDCTInitAlloc)(ಶೂನ್ಯ**, Ippi_ಗಾತ್ರ, IppHintAlgorithm);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * ippiDCTFree)(ಶೂನ್ಯ* pDCTSpec);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Ipp_ಸ್ಥಿತಿ (CV_STDCALL * ippi_DCTGet_Buf_ಗಾತ್ರ)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, ಪೂರ್ಣಾಂಕ*);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SpecType, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(ಸ್ಥಿರಾಂಕ Ipp32f*, ಪೂರ್ಣಾಂಕ, Ipp32f*, ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ SpecType*, Ipp8u*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ippiDCTWrap(ಸ್ಥಿರಾಂಕ Ipp32f* pSrc, ಪೂರ್ಣಾಂಕ src_ಹೆಜ್ಜೆ_3, Ipp32f* pDst, ಪೂರ್ಣಾಂಕ dst_ಹೆಜ್ಜೆ_3, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* pDCTSpec, Ipp8u* p_ಬಫರ್)
{
    ಹಿಂತಿರುಗಿ fn(pSrc, src_ಹೆಜ್ಜೆ_3, pDst, dst_ಹೆಜ್ಜೆ_3, (ಸ್ಥಿರಾಂಕ SpecType*)pDCTSpec, p_ಬಫರ್);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SpecType, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(SpecType**, Ippi_ಗಾತ್ರ, IppHintAlgorithm)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ippiDCTInitAllocWrap(ಶೂನ್ಯ** pDCTSpec, Ippi_ಗಾತ್ರ roi_ಗಾತ್ರ, IppHintAlgorithm hint)
{
    ಹಿಂತಿರುಗಿ fn((SpecType**)pDCTSpec, roi_ಗಾತ್ರ, hint);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SpecType, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(SpecType*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ippiDCTFreeWrap(ಶೂನ್ಯ* pDCTSpec)
{
    ಹಿಂತಿರುಗಿ fn((SpecType*)pDCTSpec);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SpecType, Ipp_ಸ್ಥಿತಿ (CV_STDCALL *fn)(ಸ್ಥಿರಾಂಕ SpecType*, ಪೂರ್ಣಾಂಕ*)>
ಸ್ಥಿರ Ipp_ಸ್ಥಿತಿ CV_STDCALL ippi_DCTGet_Buf_ಗಾತ್ರ_Wrap(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* pDCTSpec, ಪೂರ್ಣಾಂಕ* p_ಗಾತ್ರ)
{
    ಹಿಂತಿರುಗಿ fn((ಸ್ಥಿರಾಂಕ SpecType*)pDCTSpec, p_ಗಾತ್ರ);
}
#ಆದರೆ_ಅಂತ್ಯ

ವರ್ಗ DctIPPLoop_Invoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
    DctIPPLoop_Invoker(ಸ್ಥಿರಾಂಕ uchar * _src, size_t src_ಹೆಜ್ಜೆ_2, uchar * _dst, size_t dst_ಹೆಜ್ಜೆ_2, ಪೂರ್ಣಾಂಕ ಅಗಲ_3, ತಾರ್ಕಿಕ _inv, ತಾರ್ಕಿಕ *_ok) :
        ParallelLoopBody(), src(_src), src_ಹೆಜ್ಜೆ(src_ಹೆಜ್ಜೆ_2), dst(_dst), dst_ಹೆಜ್ಜೆ(dst_ಹೆಜ್ಜೆ_2), ಅಗಲ(ಅಗಲ_3), inv(_inv), ok(_ok)
    {
        *ok = ನಿಜ;
    }

    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Range& range) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(*ok == ಸುಳ್ಳು)
            ಹಿಂತಿರುಗಿ;

#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 900
        Ippi_ಗಾತ್ರ src_Roi_ಗಾತ್ರ = {ಅಗಲ, 1};

        ಪೂರ್ಣಾಂಕ spec_ಗಾತ್ರ    = 0;
        ಪೂರ್ಣಾಂಕ init_ಗಾತ್ರ    = 0;
        ಪೂರ್ಣಾಂಕ ಬಫರ್_ಗಾತ್ರ  = 0;

        Ipp8u* pDCTSpec = ಖಾಲಿ;
        Ipp8u* p_ಬಫರ್  = ಖಾಲಿ;
        Ipp8u* pInitBuf = ಖಾಲಿ;

        #ವ್ಯಾಖ್ಯಾನಿಸು IPP_RETURN              \
            ಆದರೆ(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            ಆದರೆ(p_ಬಫರ್)                 \
                ippFree(p_ಬಫರ್);       \
            ಆದರೆ(pInitBuf)                \
                ippFree(pInitBuf);      \
            ಹಿಂತಿರುಗಿ;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_ಗಾತ್ರ  ipp_Dct_Get_ಗಾತ್ರ  = inv ? ippi_DCTGet_ಗಾತ್ರ_Wrap<ippi_DCTInv_Get_ಗಾತ್ರ_32f> : ippi_DCTGet_ಗಾತ್ರ_Wrap<ippi_DCTFwd_Get_ಗಾತ್ರ_32f>;

        ಆದರೆ(ipp_Dct_Get_ಗಾತ್ರ(src_Roi_ಗಾತ್ರ, &spec_ಗಾತ್ರ, &init_ಗಾತ್ರ, &ಬಫರ್_ಗಾತ್ರ) < 0)
        {
            *ok = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_ಗಾತ್ರ);
        ಆದರೆ(!pDCTSpec && spec_ಗಾತ್ರ)
        {
            *ok = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }

        p_ಬಫರ್  = (Ipp8u*)CV_IPP_MALLOC(ಬಫರ್_ಗಾತ್ರ);
        ಆದರೆ(!p_ಬಫರ್ && ಬಫರ್_ಗಾತ್ರ)
        {
            *ok = ಸುಳ್ಳು;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_ಗಾತ್ರ);
        ಆದರೆ(!pInitBuf && init_ಗಾತ್ರ)
        {
            *ok = ಸುಳ್ಳು;
            IPP_RETURN
        }

        ಆದರೆ(ippDctInit(pDCTSpec, src_Roi_ಗಾತ್ರ, pInitBuf) < 0)
        {
            *ok = ಸುಳ್ಳು;
            IPP_RETURN
        }

        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; ++i)
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (ದಶಮಾಂಶ*)(src + src_ಹೆಜ್ಜೆ * i), static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ), (ದಶಮಾಂಶ*)(dst + dst_ಹೆಜ್ಜೆ * i), static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ), pDCTSpec, p_ಬಫರ್) < 0)
            {
                *ok = ಸುಳ್ಳು;
                IPP_RETURN
            }
        }
        IPP_RETURN
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು IPP_RETURN
#ಅಥವಾಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 700
        ಶೂನ್ಯ* pDCTSpec;
        Auto_ಬಫರ್<uchar> buf;
        uchar* p_ಬಫರ್ = 0;
        ಪೂರ್ಣಾಂಕ buf_ಗಾತ್ರ=0;

        Ippi_ಗಾತ್ರ src_Roi_ಗಾತ್ರ = {ಅಗಲ, 1};

        CV_SUPPRESS_DEPRECATED_ಆರಂಭ

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_ಗಾತ್ರ ipp_Get_Buf_ಗಾತ್ರ = inv ? ippi_DCTGet_Buf_ಗಾತ್ರ_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_ಗಾತ್ರ_32f> : ippi_DCTGet_Buf_ಗಾತ್ರ_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_ಗಾತ್ರ_32f>;

        ಆದರೆ (ippInitAlloc(&pDCTSpec, src_Roi_ಗಾತ್ರ, ippAlgHintNone)>=0 && ipp_Get_Buf_ಗಾತ್ರ(pDCTSpec, &buf_ಗಾತ್ರ)>=0)
        {
            buf.allocate( buf_ಗಾತ್ರ );
            p_ಬಫರ್ = (uchar*)buf;

            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; ++i)
            {
                ಆದರೆ(ippDctFun((ದಶಮಾಂಶ*)(src + src_ಹೆಜ್ಜೆ * i), static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ), (ದಶಮಾಂಶ*)(dst + dst_ಹೆಜ್ಜೆ * i), static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ), pDCTSpec, (Ipp8u*)p_ಬಫರ್) < 0)
                {
                    *ok = ಸುಳ್ಳು;
                    ನಿಲ್ಲಿಸು;
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
            *ok = ಸುಳ್ಳು;

        ಆದರೆ (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_ಕೊನೆ
#ಇಲ್ಲದಿದ್ದರೆ
        CV_ಬಳಸದ(range);
        *ok = ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
    }

ಖಾಸಗಿ:
    ಸ್ಥಿರಾಂಕ uchar * src;
    size_t src_ಹೆಜ್ಜೆ;
    uchar * dst;
    size_t dst_ಹೆಜ್ಜೆ;
    ಪೂರ್ಣಾಂಕ ಅಗಲ;
    ತಾರ್ಕಿಕ inv;
    ತಾರ್ಕಿಕ *ok;
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ DctIPPLoop(ಸ್ಥಿರಾಂಕ uchar * src, size_t src_ಹೆಜ್ಜೆ, uchar * dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ತಾರ್ಕಿಕ inv)
{
    ತಾರ್ಕಿಕ ok;
    parallel_for_(Range(0, ಎತ್ತರ), DctIPPLoop_Invoker(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, inv, &ok), ಎತ್ತರ/(ದ್ವಿಗುಣ)(1<<4) );
    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ippi_DCT_32f(ಸ್ಥಿರಾಂಕ uchar * src, size_t src_ಹೆಜ್ಜೆ, uchar * dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ತಾರ್ಕಿಕ inv, ತಾರ್ಕಿಕ ಅಡ್ಡಸಾಲು)
{
    CV_INSTRUMENT_REGION_IPP();

    ಆದರೆ(ಅಡ್ಡಸಾಲು)
        ಹಿಂತಿರುಗಿ DctIPPLoop(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, ಎತ್ತರ, inv);
    ಇಲ್ಲದಿದ್ದರೆ
    {
#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 900
        Ippi_ಗಾತ್ರ src_Roi_ಗಾತ್ರ = {ಅಗಲ, ಎತ್ತರ};

        ಪೂರ್ಣಾಂಕ spec_ಗಾತ್ರ    = 0;
        ಪೂರ್ಣಾಂಕ init_ಗಾತ್ರ    = 0;
        ಪೂರ್ಣಾಂಕ ಬಫರ್_ಗಾತ್ರ  = 0;

        Ipp8u* pDCTSpec = ಖಾಲಿ;
        Ipp8u* p_ಬಫರ್  = ಖಾಲಿ;
        Ipp8u* pInitBuf = ಖಾಲಿ;

        #ವ್ಯಾಖ್ಯಾನಿಸು IPP_RELEASE             \
            ಆದರೆ(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            ಆದರೆ(p_ಬಫರ್)                 \
                ippFree(p_ಬಫರ್);       \
            ಆದರೆ(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_ಗಾತ್ರ  ipp_Dct_Get_ಗಾತ್ರ  = inv ? ippi_DCTGet_ಗಾತ್ರ_Wrap<ippi_DCTInv_Get_ಗಾತ್ರ_32f> : ippi_DCTGet_ಗಾತ್ರ_Wrap<ippi_DCTFwd_Get_ಗಾತ್ರ_32f>;

        ಆದರೆ(ipp_Dct_Get_ಗಾತ್ರ(src_Roi_ಗಾತ್ರ, &spec_ಗಾತ್ರ, &init_ಗಾತ್ರ, &ಬಫರ್_ಗಾತ್ರ) < 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_ಗಾತ್ರ);
        ಆದರೆ(!pDCTSpec && spec_ಗಾತ್ರ)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        p_ಬಫರ್  = (Ipp8u*)CV_IPP_MALLOC(ಬಫರ್_ಗಾತ್ರ);
        ಆದರೆ(!p_ಬಫರ್ && ಬಫರ್_ಗಾತ್ರ)
        {
            IPP_RELEASE
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_ಗಾತ್ರ);
        ಆದರೆ(!pInitBuf && init_ಗಾತ್ರ)
        {
            IPP_RELEASE
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }

        ಆದರೆ(ippDctInit(pDCTSpec, src_Roi_ಗಾತ್ರ, pInitBuf) < 0)
        {
            IPP_RELEASE
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }

        ಆದರೆ(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (ದಶಮಾಂಶ*)src, static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ), (ದಶಮಾಂಶ*)dst, static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ), pDCTSpec, p_ಬಫರ್) < 0)
        {
            IPP_RELEASE
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }

        IPP_RELEASE
        ಹಿಂತಿರುಗಿ ನಿಜ;
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು IPP_RELEASE
#ಅಥವಾಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 700
        Ipp_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
        ಶೂನ್ಯ* pDCTSpec;
        Auto_ಬಫರ್<uchar> buf;
        uchar* p_ಬಫರ್ = 0;
        ಪೂರ್ಣಾಂಕ buf_ಗಾತ್ರ=0;

        Ippi_ಗಾತ್ರ src_Roi_ಗಾತ್ರ = {ಅಗಲ, ಎತ್ತರ};

        CV_SUPPRESS_DEPRECATED_ಆರಂಭ

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_ಗಾತ್ರ ipp_Get_Buf_ಗಾತ್ರ = inv ? ippi_DCTGet_Buf_ಗಾತ್ರ_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_ಗಾತ್ರ_32f> : ippi_DCTGet_Buf_ಗಾತ್ರ_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_ಗಾತ್ರ_32f>;

        ಸ್ಥಿತಿ_2 = ippStsErr;

        ಆದರೆ (ippInitAlloc(&pDCTSpec, src_Roi_ಗಾತ್ರ, ippAlgHintNone)>=0 && ipp_Get_Buf_ಗಾತ್ರ(pDCTSpec, &buf_ಗಾತ್ರ)>=0)
        {
            buf.allocate( buf_ಗಾತ್ರ );
            p_ಬಫರ್ = (uchar*)buf;

            ಸ್ಥಿತಿ_2 = ippDctFun((ದಶಮಾಂಶ*)src, static_cast<ಪೂರ್ಣಾಂಕ>(src_ಹೆಜ್ಜೆ), (ದಶಮಾಂಶ*)dst, static_cast<ಪೂರ್ಣಾಂಕ>(dst_ಹೆಜ್ಜೆ), pDCTSpec, (Ipp8u*)p_ಬಫರ್);
        }

        ಆದರೆ (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_ಕೊನೆ

        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 >= 0;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_ಬಳಸದ(src); CV_ಬಳಸದ(dst); CV_ಬಳಸದ(inv); CV_ಬಳಸದ(ಅಡ್ಡಸಾಲು);
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
    }
}
}
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv {

ವರ್ಗ OcvDctImpl CV_FINAL : ಸಾರ್ವಜನಿಕ hal::DCT2D
{
ಸಾರ್ವಜನಿಕ:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    Auto_ಬಫರ್<uchar> ws;
    ತಾರ್ಕಿಕ is_ಅಡ್ಡಸಾಲು_Transform;
    ತಾರ್ಕಿಕ isInverse;
    ತಾರ್ಕಿಕ isContinuous;
    ಪೂರ್ಣಾಂಕ ಆರಂಭ_stage;
    ಪೂರ್ಣಾಂಕ ಕೊನೆ_stage;
    ಪೂರ್ಣಾಂಕ ಅಗಲ;
    ಪೂರ್ಣಾಂಕ ಎತ್ತರ;
    ಪೂರ್ಣಾಂಕ ಆಳ;

    ಶೂನ್ಯ init(ಪೂರ್ಣಾಂಕ ಅಗಲ_3, ಪೂರ್ಣಾಂಕ ಎತ್ತರ_3, ಪೂರ್ಣಾಂಕ ಆಳ_2, ಪೂರ್ಣಾಂಕ flags)
    {
        ಅಗಲ = ಅಗಲ_3;
        ಎತ್ತರ = ಎತ್ತರ_3;
        ಆಳ = ಆಳ_2;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        is_ಅಡ್ಡಸಾಲು_Transform = (flags & CV_HAL_DFT_ಅಡ್ಡಸಾಲುಗಳು) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        ಸ್ಥಿರ DCTFunc dct_tbl[4] =
        {
            dctWrap<ದಶಮಾಂಶ, DCT<ದಶಮಾಂಶ>>,
            dctWrap<ದಶಮಾಂಶ, IDCT<ದಶಮಾಂಶ>>,
            dctWrap<ದ್ವಿಗುಣ, DCT<ದ್ವಿಗುಣ>>,
            dctWrap<ದ್ವಿಗುಣ, IDCT<ದ್ವಿಗುಣ>>
        };
        dct_func = dct_tbl[(ಪೂರ್ಣಾಂಕ)isInverse + (ಆಳ == CV_64F)*2];
        opt.isComplex = ಸುಳ್ಳು;
        opt.isInverse = ಸುಳ್ಳು;
        opt.scale = 1.;

        ಆದರೆ (is_ಅಡ್ಡಸಾಲು_Transform || ಎತ್ತರ == 1 || (ಅಗಲ == 1 && isContinuous))
        {
            ಆರಂಭ_stage = ಕೊನೆ_stage = 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆರಂಭ_stage = (ಅಗಲ == 1);
            ಕೊನೆ_stage = 1;
        }
    }
    ಶೂನ್ಯ apply(ಸ್ಥಿರಾಂಕ uchar *src, size_t src_ಹೆಜ್ಜೆ, uchar *dst, size_t dst_ಹೆಜ್ಜೆ) CV_OVERRIDE
    {
        CV_IPP_ಸತತ(IPP_ಆವೃತ್ತಿ_X100 >= 700 && ಆಳ == CV_32F, ippi_DCT_32f(src, src_ಹೆಜ್ಜೆ, dst, dst_ಹೆಜ್ಜೆ, ಅಗಲ, ಎತ್ತರ, isInverse, is_ಅಡ್ಡಸಾಲು_Transform))

        ಪೂರ್ಣಾಂಕ prev_len = 0;
        ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ_2 = (ಆಳ == CV_32F) ? ಗಾತ್ರ(ದಶಮಾಂಶ) : ಗಾತ್ರ(ದ್ವಿಗುಣ);
        ಸ್ಥಿರಾಂಕ DFTKernels& kernels = getDFTKernels(ಆಳ);

        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ stage = ಆರಂಭ_stage ; stage <= ಕೊನೆ_stage; stage++ )
        {
            ಸ್ಥಿರಾಂಕ uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            ಪೂರ್ಣಾಂಕ len, ಎಣಿಕೆ_3;

            ಆದರೆ( stage == 0 )
            {
                len = ಅಗಲ;
                ಎಣಿಕೆ_3 = ಎತ್ತರ;
                ಆದರೆ( len == 1 && !is_ಅಡ್ಡಸಾಲು_Transform )
                {
                    len = ಎತ್ತರ;
                    ಎಣಿಕೆ_3 = 1;
                }
                sstep0 = src_ಹೆಜ್ಜೆ;
                dstep0 = dst_ಹೆಜ್ಜೆ;
                sstep1 = dstep1 = elem_ಗಾತ್ರ_2;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                len = ಎತ್ತರ;
                ಎಣಿಕೆ_3 = ಅಗಲ;
                sstep1 = src_ಹೆಜ್ಜೆ;
                dstep1 = dst_ಹೆಜ್ಜೆ;
                sstep0 = dstep0 = elem_ಗಾತ್ರ_2;
            }

            opt.n = len;

            ಆದರೆ( len != prev_len )
            {
                ಆದರೆ( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                ಆದರೆ( len > 1 )
                {
                    plan.build(isInverse ? DFT_ಬಗೆ_IDCT : DFT_ಬಗೆ_DCT, len, ಆಳ, kernels.vlanes);
                    ws.allocate(plan.ws_ಬೈಟುಗಳು);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = ws.ದತ್ತಾಂಶ();
                prev_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            ದ್ವಿಗುಣ nstripes = dftParallelStripes(ಎಣಿಕೆ_3, len);
            ಆದರೆ( len > 1 && ಎಣಿಕೆ_3 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                ಸ್ವಯಂಚಾಲಿತ process_ಸಾಲುಗಳು = [&](ಸ್ಥಿರಾಂಕ Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    Auto_ಬಫರ್<uchar> ws_(plan.ws_ಬೈಟುಗಳು);
                    opt_.workspace = ws_.ದತ್ತಾಂಶ();
                    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, ಎಣಿಕೆ_3), process_ಸಾಲುಗಳು, nstripes);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ(ಚಿಹ್ನೆರಹಿತ i = 0; i < static_cast<ಚಿಹ್ನೆರಹಿತ>(ಎಣಿಕೆ_3); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_ಹೆಜ್ಜೆ = dst_ಹೆಜ್ಜೆ;
        }
    }
};

ರಚನೆ ReplacementDCT2D : ಸಾರ್ವಜನಿಕ hal::DCT2D
{
    cvhalDFT *context;
    ತಾರ್ಕಿಕ isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(ಸುಳ್ಳು) {}
    ತಾರ್ಕಿಕ init(ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ flags)
    {
        ಪೂರ್ಣಾಂಕ res = cv_hal_dctInit2D(&context, ಅಗಲ, ಎತ್ತರ, ಆಳ, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ಹಿಂತಿರುಗಿ isInitialized;
    }
    ಶೂನ್ಯ apply(ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ) CV_OVERRIDE
    {
        ಆದರೆ (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_ದತ್ತಾಂಶ, src_ಹೆಜ್ಜೆ, dst_ದತ್ತಾಂಶ, dst_ಹೆಜ್ಜೆ);
        }
    }
    ~ReplacementDCT2D()
    {
        ಆದರೆ (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

ನಾಮಾಕಾಶ hal {

Ptr<DCT2D> DCT2D::create(ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ flags)
{
    {
        ReplacementDCT2D *impl = ಹೊಸ ReplacementDCT2D();
        ಆದರೆ (impl->init(ಅಗಲ, ಎತ್ತರ, ಆಳ, flags))
        {
            ಹಿಂತಿರುಗಿ Ptr<DCT2D>(impl);
        }
        ಅಳಿಸು impl;
    }
    {
        OcvDctImpl *impl = ಹೊಸ OcvDctImpl();
        impl->init(ಅಗಲ, ಎತ್ತರ, ಆಳ, flags);
        ಹಿಂತಿರುಗಿ Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

ಶೂನ್ಯ cv::dct( ಒಳಹರಿವು_Array _src0, OutputArray _dst, ಪೂರ್ಣಾಂಕ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    ಆದರೆ (src.empty())
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }
    ಪೂರ್ಣಾಂಕ type = src.type(), ಆಳ = src.ಆಳ();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.ಅಡ್ಡಸಾಲುಗಳು, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type );
    Mat dst = _dst.getMat();

    ಪೂರ್ಣಾಂಕ f = 0;
    ಆದರೆ ((flags & DFT_ಅಡ್ಡಸಾಲುಗಳು) != 0)
        f |= CV_HAL_DFT_ಅಡ್ಡಸಾಲುಗಳು;
    ಆದರೆ ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    ಆದರೆ (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, ಆಳ, f);
    c->apply(src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ);
}


ಶೂನ್ಯ cv::idct( ಒಳಹರಿವು_Array src, OutputArray dst, ಪೂರ್ಣಾಂಕ flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

ನಾಮಾಕಾಶ cv
{

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ optimalDFTSizeTab[] = {
1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 25, 27, 30, 32, 36, 40, 45, 48,
50, 54, 60, 64, 72, 75, 80, 81, 90, 96, 100, 108, 120, 125, 128, 135, 144, 150, 160,
162, 180, 192, 200, 216, 225, 240, 243, 250, 256, 270, 288, 300, 320, 324, 360, 375,
384, 400, 405, 432, 450, 480, 486, 500, 512, 540, 576, 600, 625, 640, 648, 675, 720,
729, 750, 768, 800, 810, 864, 900, 960, 972, 1000, 1024, 1080, 1125, 1152, 1200,
1215, 1250, 1280, 1296, 1350, 1440, 1458, 1500, 1536, 1600, 1620, 1728, 1800, 1875,
1920, 1944, 2000, 2025, 2048, 2160, 2187, 2250, 2304, 2400, 2430, 2500, 2560, 2592,
2700, 2880, 2916, 3000, 3072, 3125, 3200, 3240, 3375, 3456, 3600, 3645, 3750, 3840,
3888, 4000, 4050, 4096, 4320, 4374, 4500, 4608, 4800, 4860, 5000, 5120, 5184, 5400,
5625, 5760, 5832, 6000, 6075, 6144, 6250, 6400, 6480, 6561, 6750, 6912, 7200, 7290,
7500, 7680, 7776, 8000, 8100, 8192, 8640, 8748, 9000, 9216, 9375, 9600, 9720, 10000,
10125, 10240, 10368, 10800, 10935, 11250, 11520, 11664, 12000, 12150, 12288, 12500,
12800, 12960, 13122, 13500, 13824, 14400, 14580, 15000, 15360, 15552, 15625, 16000,
16200, 16384, 16875, 17280, 17496, 18000, 18225, 18432, 18750, 19200, 19440, 19683,
20000, 20250, 20480, 20736, 21600, 21870, 22500, 23040, 23328, 24000, 24300, 24576,
25000, 25600, 25920, 26244, 27000, 27648, 28125, 28800, 29160, 30000, 30375, 30720,
31104, 31250, 32000, 32400, 32768, 32805, 33750, 34560, 34992, 36000, 36450, 36864,
37500, 38400, 38880, 39366, 40000, 40500, 40960, 41472, 43200, 43740, 45000, 46080,
46656, 46875, 48000, 48600, 49152, 50000, 50625, 51200, 51840, 52488, 54000, 54675,
55296, 56250, 57600, 58320, 59049, 60000, 60750, 61440, 62208, 62500, 64000, 64800,
65536, 65610, 67500, 69120, 69984, 72000, 72900, 73728, 75000, 76800, 77760, 78125,
78732, 80000, 81000, 81920, 82944, 84375, 86400, 87480, 90000, 91125, 92160, 93312,
93750, 96000, 97200, 98304, 98415, 100000, 101250, 102400, 103680, 104976, 108000,
109350, 110592, 112500, 115200, 116640, 118098, 120000, 121500, 122880, 124416, 125000,
128000, 129600, 131072, 131220, 135000, 138240, 139968, 140625, 144000, 145800, 147456,
150000, 151875, 153600, 155520, 156250, 157464, 160000, 162000, 163840, 164025, 165888,
168750, 172800, 174960, 177147, 180000, 182250, 184320, 186624, 187500, 192000, 194400,
196608, 196830, 200000, 202500, 204800, 207360, 209952, 216000, 218700, 221184, 225000,
230400, 233280, 234375, 236196, 240000, 243000, 245760, 248832, 250000, 253125, 256000,
259200, 262144, 262440, 270000, 273375, 276480, 279936, 281250, 288000, 291600, 294912,
295245, 300000, 303750, 307200, 311040, 312500, 314928, 320000, 324000, 327680, 328050,
331776, 337500, 345600, 349920, 354294, 360000, 364500, 368640, 373248, 375000, 384000,
388800, 390625, 393216, 393660, 400000, 405000, 409600, 414720, 419904, 421875, 432000,
437400, 442368, 450000, 455625, 460800, 466560, 468750, 472392, 480000, 486000, 491520,
492075, 497664, 500000, 506250, 512000, 518400, 524288, 524880, 531441, 540000, 546750,
552960, 559872, 562500, 576000, 583200, 589824, 590490, 600000, 607500, 614400, 622080,
625000, 629856, 640000, 648000, 655360, 656100, 663552, 675000, 691200, 699840, 703125,
708588, 720000, 729000, 737280, 746496, 750000, 759375, 768000, 777600, 781250, 786432,
787320, 800000, 810000, 819200, 820125, 829440, 839808, 843750, 864000, 874800, 884736,
885735, 900000, 911250, 921600, 933120, 937500, 944784, 960000, 972000, 983040, 984150,
995328, 1000000, 1012500, 1024000, 1036800, 1048576, 1049760, 1062882, 1080000, 1093500,
1105920, 1119744, 1125000, 1152000, 1166400, 1171875, 1179648, 1180980, 1200000,
1215000, 1228800, 1244160, 1250000, 1259712, 1265625, 1280000, 1296000, 1310720,
1312200, 1327104, 1350000, 1366875, 1382400, 1399680, 1406250, 1417176, 1440000,
1458000, 1474560, 1476225, 1492992, 1500000, 1518750, 1536000, 1555200, 1562500,
1572864, 1574640, 1594323, 1600000, 1620000, 1638400, 1640250, 1658880, 1679616,
1687500, 1728000, 1749600, 1769472, 1771470, 1800000, 1822500, 1843200, 1866240,
1875000, 1889568, 1920000, 1944000, 1953125, 1966080, 1968300, 1990656, 2000000,
2025000, 2048000, 2073600, 2097152, 2099520, 2109375, 2125764, 2160000, 2187000,
2211840, 2239488, 2250000, 2278125, 2304000, 2332800, 2343750, 2359296, 2361960,
2400000, 2430000, 2457600, 2460375, 2488320, 2500000, 2519424, 2531250, 2560000,
2592000, 2621440, 2624400, 2654208, 2657205, 2700000, 2733750, 2764800, 2799360,
2812500, 2834352, 2880000, 2916000, 2949120, 2952450, 2985984, 3000000, 3037500,
3072000, 3110400, 3125000, 3145728, 3149280, 3188646, 3200000, 3240000, 3276800,
3280500, 3317760, 3359232, 3375000, 3456000, 3499200, 3515625, 3538944, 3542940,
3600000, 3645000, 3686400, 3732480, 3750000, 3779136, 3796875, 3840000, 3888000,
3906250, 3932160, 3936600, 3981312, 4000000, 4050000, 4096000, 4100625, 4147200,
4194304, 4199040, 4218750, 4251528, 4320000, 4374000, 4423680, 4428675, 4478976,
4500000, 4556250, 4608000, 4665600, 4687500, 4718592, 4723920, 4782969, 4800000,
4860000, 4915200, 4920750, 4976640, 5000000, 5038848, 5062500, 5120000, 5184000,
5242880, 5248800, 5308416, 5314410, 5400000, 5467500, 5529600, 5598720, 5625000,
5668704, 5760000, 5832000, 5859375, 5898240, 5904900, 5971968, 6000000, 6075000,
6144000, 6220800, 6250000, 6291456, 6298560, 6328125, 6377292, 6400000, 6480000,
6553600, 6561000, 6635520, 6718464, 6750000, 6834375, 6912000, 6998400, 7031250,
7077888, 7085880, 7200000, 7290000, 7372800, 7381125, 7464960, 7500000, 7558272,
7593750, 7680000, 7776000, 7812500, 7864320, 7873200, 7962624, 7971615, 8000000,
8100000, 8192000, 8201250, 8294400, 8388608, 8398080, 8437500, 8503056, 8640000,
8748000, 8847360, 8857350, 8957952, 9000000, 9112500, 9216000, 9331200, 9375000,
9437184, 9447840, 9565938, 9600000, 9720000, 9765625, 9830400, 9841500, 9953280,
10000000, 10077696, 10125000, 10240000, 10368000, 10485760, 10497600, 10546875, 10616832,
10628820, 10800000, 10935000, 11059200, 11197440, 11250000, 11337408, 11390625, 11520000,
11664000, 11718750, 11796480, 11809800, 11943936, 12000000, 12150000, 12288000, 12301875,
12441600, 12500000, 12582912, 12597120, 12656250, 12754584, 12800000, 12960000, 13107200,
13122000, 13271040, 13286025, 13436928, 13500000, 13668750, 13824000, 13996800, 14062500,
14155776, 14171760, 14400000, 14580000, 14745600, 14762250, 14929920, 15000000, 15116544,
15187500, 15360000, 15552000, 15625000, 15728640, 15746400, 15925248, 15943230, 16000000,
16200000, 16384000, 16402500, 16588800, 16777216, 16796160, 16875000, 17006112, 17280000,
17496000, 17578125, 17694720, 17714700, 17915904, 18000000, 18225000, 18432000, 18662400,
18750000, 18874368, 18895680, 18984375, 19131876, 19200000, 19440000, 19531250, 19660800,
19683000, 19906560, 20000000, 20155392, 20250000, 20480000, 20503125, 20736000, 20971520,
20995200, 21093750, 21233664, 21257640, 21600000, 21870000, 22118400, 22143375, 22394880,
22500000, 22674816, 22781250, 23040000, 23328000, 23437500, 23592960, 23619600, 23887872,
23914845, 24000000, 24300000, 24576000, 24603750, 24883200, 25000000, 25165824, 25194240,
25312500, 25509168, 25600000, 25920000, 26214400, 26244000, 26542080, 26572050, 26873856,
27000000, 27337500, 27648000, 27993600, 28125000, 28311552, 28343520, 28800000, 29160000,
29296875, 29491200, 29524500, 29859840, 30000000, 30233088, 30375000, 30720000, 31104000,
31250000, 31457280, 31492800, 31640625, 31850496, 31886460, 32000000, 32400000, 32768000,
32805000, 33177600, 33554432, 33592320, 33750000, 34012224, 34171875, 34560000, 34992000,
35156250, 35389440, 35429400, 35831808, 36000000, 36450000, 36864000, 36905625, 37324800,
37500000, 37748736, 37791360, 37968750, 38263752, 38400000, 38880000, 39062500, 39321600,
39366000, 39813120, 39858075, 40000000, 40310784, 40500000, 40960000, 41006250, 41472000,
41943040, 41990400, 42187500, 42467328, 42515280, 43200000, 43740000, 44236800, 44286750,
44789760, 45000000, 45349632, 45562500, 46080000, 46656000, 46875000, 47185920, 47239200,
47775744, 47829690, 48000000, 48600000, 48828125, 49152000, 49207500, 49766400, 50000000,
50331648, 50388480, 50625000, 51018336, 51200000, 51840000, 52428800, 52488000, 52734375,
53084160, 53144100, 53747712, 54000000, 54675000, 55296000, 55987200, 56250000, 56623104,
56687040, 56953125, 57600000, 58320000, 58593750, 58982400, 59049000, 59719680, 60000000,
60466176, 60750000, 61440000, 61509375, 62208000, 62500000, 62914560, 62985600, 63281250,
63700992, 63772920, 64000000, 64800000, 65536000, 65610000, 66355200, 66430125, 67108864,
67184640, 67500000, 68024448, 68343750, 69120000, 69984000, 70312500, 70778880, 70858800,
71663616, 72000000, 72900000, 73728000, 73811250, 74649600, 75000000, 75497472, 75582720,
75937500, 76527504, 76800000, 77760000, 78125000, 78643200, 78732000, 79626240, 79716150,
80000000, 80621568, 81000000, 81920000, 82012500, 82944000, 83886080, 83980800, 84375000,
84934656, 85030560, 86400000, 87480000, 87890625, 88473600, 88573500, 89579520, 90000000,
90699264, 91125000, 92160000, 93312000, 93750000, 94371840, 94478400, 94921875, 95551488,
95659380, 96000000, 97200000, 97656250, 98304000, 98415000, 99532800, 100000000,
100663296, 100776960, 101250000, 102036672, 102400000, 102515625, 103680000, 104857600,
104976000, 105468750, 106168320, 106288200, 107495424, 108000000, 109350000, 110592000,
110716875, 111974400, 112500000, 113246208, 113374080, 113906250, 115200000, 116640000,
117187500, 117964800, 118098000, 119439360, 119574225, 120000000, 120932352, 121500000,
122880000, 123018750, 124416000, 125000000, 125829120, 125971200, 126562500, 127401984,
127545840, 128000000, 129600000, 131072000, 131220000, 132710400, 132860250, 134217728,
134369280, 135000000, 136048896, 136687500, 138240000, 139968000, 140625000, 141557760,
141717600, 143327232, 144000000, 145800000, 146484375, 147456000, 147622500, 149299200,
150000000, 150994944, 151165440, 151875000, 153055008, 153600000, 155520000, 156250000,
157286400, 157464000, 158203125, 159252480, 159432300, 160000000, 161243136, 162000000,
163840000, 164025000, 165888000, 167772160, 167961600, 168750000, 169869312, 170061120,
170859375, 172800000, 174960000, 175781250, 176947200, 177147000, 179159040, 180000000,
181398528, 182250000, 184320000, 184528125, 186624000, 187500000, 188743680, 188956800,
189843750, 191102976, 191318760, 192000000, 194400000, 195312500, 196608000, 196830000,
199065600, 199290375, 200000000, 201326592, 201553920, 202500000, 204073344, 204800000,
205031250, 207360000, 209715200, 209952000, 210937500, 212336640, 212576400, 214990848,
216000000, 218700000, 221184000, 221433750, 223948800, 225000000, 226492416, 226748160,
227812500, 230400000, 233280000, 234375000, 235929600, 236196000, 238878720, 239148450,
240000000, 241864704, 243000000, 244140625, 245760000, 246037500, 248832000, 250000000,
251658240, 251942400, 253125000, 254803968, 255091680, 256000000, 259200000, 262144000,
262440000, 263671875, 265420800, 265720500, 268435456, 268738560, 270000000, 272097792,
273375000, 276480000, 279936000, 281250000, 283115520, 283435200, 284765625, 286654464,
288000000, 291600000, 292968750, 294912000, 295245000, 298598400, 300000000, 301989888,
302330880, 303750000, 306110016, 307200000, 307546875, 311040000, 312500000, 314572800,
314928000, 316406250, 318504960, 318864600, 320000000, 322486272, 324000000, 327680000,
328050000, 331776000, 332150625, 335544320, 335923200, 337500000, 339738624, 340122240,
341718750, 345600000, 349920000, 351562500, 353894400, 354294000, 358318080, 360000000,
362797056, 364500000, 368640000, 369056250, 373248000, 375000000, 377487360, 377913600,
379687500, 382205952, 382637520, 384000000, 388800000, 390625000, 393216000, 393660000,
398131200, 398580750, 400000000, 402653184, 403107840, 405000000, 408146688, 409600000,
410062500, 414720000, 419430400, 419904000, 421875000, 424673280, 425152800, 429981696,
432000000, 437400000, 439453125, 442368000, 442867500, 447897600, 450000000, 452984832,
453496320, 455625000, 460800000, 466560000, 468750000, 471859200, 472392000, 474609375,
477757440, 478296900, 480000000, 483729408, 486000000, 488281250, 491520000, 492075000,
497664000, 500000000, 503316480, 503884800, 506250000, 509607936, 510183360, 512000000,
512578125, 518400000, 524288000, 524880000, 527343750, 530841600, 531441000, 536870912,
537477120, 540000000, 544195584, 546750000, 552960000, 553584375, 559872000, 562500000,
566231040, 566870400, 569531250, 573308928, 576000000, 583200000, 585937500, 589824000,
590490000, 597196800, 597871125, 600000000, 603979776, 604661760, 607500000, 612220032,
614400000, 615093750, 622080000, 625000000, 629145600, 629856000, 632812500, 637009920,
637729200, 640000000, 644972544, 648000000, 655360000, 656100000, 663552000, 664301250,
671088640, 671846400, 675000000, 679477248, 680244480, 683437500, 691200000, 699840000,
703125000, 707788800, 708588000, 716636160, 720000000, 725594112, 729000000, 732421875,
737280000, 738112500, 746496000, 750000000, 754974720, 755827200, 759375000, 764411904,
765275040, 768000000, 777600000, 781250000, 786432000, 787320000, 791015625, 796262400,
797161500, 800000000, 805306368, 806215680, 810000000, 816293376, 819200000, 820125000,
829440000, 838860800, 839808000, 843750000, 849346560, 850305600, 854296875, 859963392,
864000000, 874800000, 878906250, 884736000, 885735000, 895795200, 900000000, 905969664,
906992640, 911250000, 921600000, 922640625, 933120000, 937500000, 943718400, 944784000,
949218750, 955514880, 956593800, 960000000, 967458816, 972000000, 976562500, 983040000,
984150000, 995328000, 996451875, 1000000000, 1006632960, 1007769600, 1012500000,
1019215872, 1020366720, 1024000000, 1025156250, 1036800000, 1048576000, 1049760000,
1054687500, 1061683200, 1062882000, 1073741824, 1074954240, 1080000000, 1088391168,
1093500000, 1105920000, 1107168750, 1119744000, 1125000000, 1132462080, 1133740800,
1139062500, 1146617856, 1152000000, 1166400000, 1171875000, 1179648000, 1180980000,
1194393600, 1195742250, 1200000000, 1207959552, 1209323520, 1215000000, 1220703125,
1224440064, 1228800000, 1230187500, 1244160000, 1250000000, 1258291200, 1259712000,
1265625000, 1274019840, 1275458400, 1280000000, 1289945088, 1296000000, 1310720000,
1312200000, 1318359375, 1327104000, 1328602500, 1342177280, 1343692800, 1350000000,
1358954496, 1360488960, 1366875000, 1382400000, 1399680000, 1406250000, 1415577600,
1417176000, 1423828125, 1433272320, 1440000000, 1451188224, 1458000000, 1464843750,
1474560000, 1476225000, 1492992000, 1500000000, 1509949440, 1511654400, 1518750000,
1528823808, 1530550080, 1536000000, 1537734375, 1555200000, 1562500000, 1572864000,
1574640000, 1582031250, 1592524800, 1594323000, 1600000000, 1610612736, 1612431360,
1620000000, 1632586752, 1638400000, 1640250000, 1658880000, 1660753125, 1677721600,
1679616000, 1687500000, 1698693120, 1700611200, 1708593750, 1719926784, 1728000000,
1749600000, 1757812500, 1769472000, 1771470000, 1791590400, 1800000000, 1811939328,
1813985280, 1822500000, 1843200000, 1845281250, 1866240000, 1875000000, 1887436800,
1889568000, 1898437500, 1911029760, 1913187600, 1920000000, 1934917632, 1944000000,
1953125000, 1966080000, 1968300000, 1990656000, 1992903750, 2000000000, 2013265920,
2015539200, 2025000000, 2038431744, 2040733440, 2048000000, 2050312500, 2073600000,
2097152000, 2099520000, 2109375000, 2123366400, 2125764000
};

}

ಪೂರ್ಣಾಂಕ cv::getOptimalDFTSize( ಪೂರ್ಣಾಂಕ size0 )
{
    ಪೂರ್ಣಾಂಕ a = 0, b = ಗಾತ್ರ(optimalDFTSizeTab)/ಗಾತ್ರ(optimalDFTSizeTab[0]) - 1;
    ಆದರೆ( (ಚಿಹ್ನೆರಹಿತ)size0 >= (ಚಿಹ್ನೆರಹಿತ)optimalDFTSizeTab[b] )
        ಹಿಂತಿರುಗಿ -1;

    ಆಗ( a < b )
    {
        ಪೂರ್ಣಾಂಕ c = (a + b) >> 1;
        ಆದರೆ( size0 <= optimalDFTSizeTab[c] )
            b = c;
        ಇಲ್ಲದಿದ್ದರೆ
            a = c+1;
    }

    ಹಿಂತಿರುಗಿ optimalDFTSizeTab[b];
}
/* End of file. */
