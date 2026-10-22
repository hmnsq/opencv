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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_core.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "dxt.hpp"
#అన్తర్భావ "dxt.simd.hpp"
#అన్తర్భావ "dxt.simd_declarations.hpp"
#అన్తర్భావ <map>

నామాకాశ cv
{

#ఒకవేళ IPP_సంస్కరణ_X100 >= 710
#నిర్వచించు USE_IPP_DFT 1
#లేకపోతే
#నిర్వచనం_రద్దు USE_IPP_DFT
#అయితే_ముగింపు

#ఒకవేళ defined USE_IPP_DFT
#ఒకవేళ IPP_సంస్కరణ_X100 >= 202220
#నిర్వచించు IPP_DISABLE_DFT32F ((లోతు == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#లేకపోతే
#నిర్వచించు IPP_DISABLE_DFT32F అబద్ధం
#అయితే_ముగింపు
#అయితే_ముగింపు

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#నిర్వచితమైతే HAVE_OPENCL
// factorization used by the OpenCL radix selection
స్థిర పూర్ణాంకం
DFTFactorize( పూర్ణాంకం n, పూర్ణాంకం* factors )
{
    పూర్ణాంకం nf = 0, f, i, j;

    ఒకవేళ( n <= 5 )
    {
        factors[0] = n;
        తిరిగి 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    ఒకవేళ( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    కొరకు( f = 3; n > 1; )
    {
        పూర్ణాంకం d = n/f;
        ఒకవేళ( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        లేకపోతే
        {
            f += 2;
            ఒకవేళ( f*f > n )
                ఆపు;
        }
    }

    ఒకవేళ( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    కొరకు( i = f; i < (nf+f)/2; i++ )
        CV_మార్పిడి( factors[i], factors[nf-i-1+f], j );

    తిరిగి nf;
}
#అయితే_ముగింపు

#నిర్వచితమైతే USE_IPP_DFT
స్థిర Ipp_స్థితి ippsDFTFwd_CToC( స్థిరరాశి Complex<దశాంశం>* src, Complex<దశాంశం>* dst,
                             స్థిరరాశి శూన్య* spec, uchar* buf)
{
    తిరిగి CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (స్థిరరాశి Ipp32fc*)src, (Ipp32fc*)dst,
                                 (స్థిరరాశి IppsDFTSpec_C_32fc*)spec, buf);
}

స్థిర Ipp_స్థితి ippsDFTFwd_CToC( స్థిరరాశి Complex<ద్విగుణ>* src, Complex<ద్విగుణ>* dst,
                             స్థిరరాశి శూన్య* spec, uchar* buf)
{
    తిరిగి CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (స్థిరరాశి Ipp64fc*)src, (Ipp64fc*)dst,
                                 (స్థిరరాశి IppsDFTSpec_C_64fc*)spec, buf);
}

స్థిర Ipp_స్థితి ippsDFTInv_CToC( స్థిరరాశి Complex<దశాంశం>* src, Complex<దశాంశం>* dst,
                             స్థిరరాశి శూన్య* spec, uchar* buf)
{
    తిరిగి CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (స్థిరరాశి Ipp32fc*)src, (Ipp32fc*)dst,
                                 (స్థిరరాశి IppsDFTSpec_C_32fc*)spec, buf);
}

స్థిర Ipp_స్థితి ippsDFTInv_CToC( స్థిరరాశి Complex<ద్విగుణ>* src, Complex<ద్విగుణ>* dst,
                                  స్థిరరాశి శూన్య* spec, uchar* buf)
{
    తిరిగి CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (స్థిరరాశి Ipp64fc*)src, (Ipp64fc*)dst,
                                 (స్థిరరాశి IppsDFTSpec_C_64fc*)spec, buf);
}

స్థిర Ipp_స్థితి ippsDFTFwd_RToPack( స్థిరరాశి దశాంశం* src, దశాంశం* dst,
                                     స్థిరరాశి శూన్య* spec, uchar* buf)
{
    తిరిగి CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (స్థిరరాశి IppsDFTSpec_R_32f*)spec, buf);
}

స్థిర Ipp_స్థితి ippsDFTFwd_RToPack( స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst,
                                     స్థిరరాశి శూన్య* spec, uchar* buf)
{
    తిరిగి CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (స్థిరరాశి IppsDFTSpec_R_64f*)spec, buf);
}

స్థిర Ipp_స్థితి ipps_DFTInv_Pack_గమ్యం_R( స్థిరరాశి దశాంశం* src, దశాంశం* dst,
                                     స్థిరరాశి శూన్య* spec, uchar* buf)
{
    తిరిగి CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_గమ్యం_R_32f, src, dst, (స్థిరరాశి IppsDFTSpec_R_32f*)spec, buf);
}

స్థిర Ipp_స్థితి ipps_DFTInv_Pack_గమ్యం_R( స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst,
                                     స్థిరరాశి శూన్య* spec, uchar* buf)
{
    తిరిగి CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_గమ్యం_R_64f, src, dst, (స్థిరరాశి IppsDFTSpec_R_64f*)spec, buf);
}
#అయితే_ముగింపు

నిర్మాణం OcvDftOptions;

రకనిర్వచనం శూన్య (*DFTFunc)(స్థిరరాశి OcvDftOptions & c, స్థిరరాశి శూన్య* src, శూన్య* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
నిర్మాణం OcvDftOptions {
    ద్విగుణ scale;
    పూర్ణాంకం n;

    తార్కిక isInverse;
    తార్కిక isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    తార్కిక useIpp;

#నిర్వచితమైతే USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#అయితే_ముగింపు

    స్థిరరాశి DftPlan* plan;
    స్థిరరాశి DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = అబద్ధం;
        isComplex = అబద్ధం;
        useIpp = అబద్ధం;
#నిర్వచితమైతే USE_IPP_DFT
        ipp_spec = 0;
        ipp_work = 0;
#అయితే_ముగింపు
        dft_func = 0;
    }
};


/****************************************************************************************\
                     New 1D engine: kernel table, plan builder, driver
\****************************************************************************************/

స్థిర DFTKernels getDFTKernelsDispatch(పూర్ణాంకం లోతు)
{
    CV_CPU_DISPATCH(getDFTKernels_, (లోతు), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
స్థిర స్థిరరాశి DFTKernels& getDFTKernels(పూర్ణాంకం లోతు)
{
    స్థిర స్థిరరాశి DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    స్థిర స్థిరరాశి DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    తిరిగి లోతు == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
నిర్మాణం DftTwiddleGen
{
    పూర్ణాంకం M, logB, ముసుగు_B;
    Auto_బఫరు<ద్విగుణ, 1024> buf;
    ద్విగుణ *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    స్థిర శూన్య fill_పట్టిక(ద్విగుణ* re, ద్విగుణ* im, పూర్ణాంకం లెక్క_2, ద్విగుణ wr, ద్విగుణ wi, ద్విగుణ అడుగు_Angle)
    {
        కొరకు (పూర్ణాంకం i0 = 0; i0 < లెక్క_2; i0 += 16)
        {
            ద్విగుణ t0 = అడుగు_Angle*i0;
            ద్విగుణ cr = i0 == 0 ? 1 : కోజ్యా(t0), ci = i0 == 0 ? 0 : జ్యా(t0);
            పూర్ణాంకం i1 = std::min(i0 + 16, లెక్క_2);
            కొరకు (పూర్ణాంకం i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                ద్విగుణ t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    శూన్య init(పూర్ణాంకం _M)
    {
        M = _M;
        logB = 0;
        అయితే ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        పూర్ణాంకం B = 1 << logB;
        ముసుగు_B = B - 1;
        పూర్ణాంకం nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.దత్తాంశం(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        ద్విగుణ a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fill_పట్టిక(lo_re, lo_im, B, కోజ్యా(a), జ్యా(a), a);
        fill_పట్టిక(hi_re, hi_im, nhi, కోజ్యా(a*B), జ్యా(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    శూన్య get(పూర్ణాంకం m, ద్విగుణ& re, ద్విగుణ& im) స్థిరరాశి
    {
        CV_DbgAssert(m >= 0);
        ఒకవేళ (m >= M)
            m %= M;
        పూర్ణాంకం a = m >> logB, b = m & ముసుగు_B;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

మూస<రకం_పేరు T>
శూన్య DftPlan::fillTables(size_t ntw, తార్కిక need_rtw, తార్కిక need_dct)
{
    DftPlan& p = *ఇది;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    పూర్ణాంకం M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*పరిమాణం(T) + పరిమాణం(ద్విగుణ) - 1)/పరిమాణం(ద్విగుణ) + 1);
    T* cur = (T*)p.tw.దత్తాంశం();
    పూర్ణాంకం maxL = 1;
    కొరకు (పూర్ణాంకం s = 0; s < p.nstages; s++)
        ఒకవేళ (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].పరిధి);
    Auto_బఫరు<ద్విగుణ, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    ద్విగుణ* twd = twd_buf.దత్తాంశం();
    కొరకు (పూర్ణాంకం s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        పూర్ణాంకం r = st.radix, L = st.పరిధి, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        పూర్ణాంకం అడుగు = M/(r*L);
        ఒకవేళ (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr పూర్ణాంకం K = 8;
            ద్విగుణ cr[K], ci[K], wr, wi;
            gen.get(K*అడుగు, wr, wi);
            ద్విగుణ* t1r = twd; ద్విగుణ* t1i = twd + L;      // leg 1 kept in double for the products
            కొరకు (పూర్ణాంకం j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                కొరకు (పూర్ణాంకం k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*అడుగు, cr[k], ci[k]);
                పూర్ణాంకం j1 = std::min(j0 + 16*K, L);
                కొరకు (పూర్ణాంకం j = j0; j < j1; j += K)
                    కొరకు (పూర్ణాంకం k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        ద్విగుణ t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            కొరకు (పూర్ణాంకం j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            కొరకు (పూర్ణాంకం q = 2; q < r; q++)
            {
                స్థిరరాశి T* pr = twr + (q-2)*ts; స్థిరరాశి T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                కొరకు (పూర్ణాంకం j = 0; j < L; j++)
                {
                    ద్విగుణ ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        లేకపోతే
        {
            // large odd radix: exact-ish generator lookup per entry
            కొరకు (పూర్ణాంకం q = 1; q < r; q++)
                కొరకు (పూర్ణాంకం j = 0; j < L; j++)
                {
                    ద్విగుణ re, im;
                    gen.get(q*j*అడుగు, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        ఒకవేళ (r >= 7)
        {
            పూర్ణాంకం h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            కొరకు (పూర్ణాంకం pp = 1; pp <= h; pp++)
                కొరకు (పూర్ణాంకం k = 1; k <= h; k++)
                {
                    ద్విగుణ t = 2*CV_PI*((ద్విగుణ)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)కోజ్యా(t);
                    sn[(pp-1)*h + (k-1)] = (T)జ్యా(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    ఒకవేళ (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        పూర్ణాంకం అడుగు = M/n;
        కొరకు (పూర్ణాంకం k = 0; k <= nc; k++)
        {
            ద్విగుణ re, im;
            gen.get(k*అడుగు, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    ఒకవేళ (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        ద్విగుణ scale = (p.రకం == DFT_రకం_DCT ? 2 : 1)*std::వర్గమూలం(1./(2*n));
        పూర్ణాంకం అడుగు = M/(4*n);
        కొరకు (పూర్ణాంకం k = 0; k <= nc; k++)
        {
            ద్విగుణ re, im;
            gen.get(k*అడుగు, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
శూన్య DftPlan::build(పూర్ణాంకం రకం_2, పూర్ణాంకం _n, పూర్ణాంకం లోతు_2, పూర్ణాంకం _vl)
{
    DftPlan& p = *ఇది;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(లోతు_2 == CV_32F || లోతు_2 == CV_64F);
    తార్కిక real_రకం = రకం_2 != DFT_రకం_C2_C;
    తార్కిక dct_రకం = రకం_2 == DFT_రకం_DCT || రకం_2 == DFT_రకం_IDCT;
    ఒకవేళ (dct_రకం)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.రకం = రకం_2; p.n = _n; p.లోతు = లోతు_2; p.vl = _vl;
    p.nc = real_రకం && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = లోతు == CV_32F ? పరిమాణం(దశాంశం) : పరిమాణం(ద్విగుణ);
    p.real_ఆదానం = రకం == DFT_రకం_R2_C && (n & 1) != 0;
    p.need_tmp = రకం == DFT_రకం_C2_R || రకం == DFT_రకం_DCT || రకం == DFT_రకం_IDCT;
    p.pingpong = అబద్ధం;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    పూర్ణాంకం k2 = 0, rest = nc;
    అయితే ((rest & 1) == 0) { rest >>= 1; k2++; }
    పూర్ణాంకం r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.మొదటి_radix = r0;
    పూర్ణాంకం radices[DFT_MAX_STAGES], nst = 0;
    కొరకు (పూర్ణాంకం m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    అయితే (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    అయితే (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    కొరకు (పూర్ణాంకం f = 7; rest > 1; f += 2)
    {
        ఒకవేళ (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            ఆపు;
        }
        అయితే (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    పూర్ణాంకం L = r0, max_h = 0;
    size_t ntw = 0;
    కొరకు (పూర్ణాంకం s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        పూర్ణాంకం r = radices[s];
        st.radix = r; st.పరిధి = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        ఒకవేళ (vl > 1 && L >= vl)
        {
            st.బహుళకం = DFT_STAGE_నిండిన;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            ఒకవేళ (L % vl != 0)
                p.pingpong = నిజం;   // back-off clamp => overlapping stores => must be out-of-place
        }
        లేకపోతే
        {
            st.బహుళకం = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        ఒకవేళ (r >= 7)
        {
            పూర్ణాంకం h = (r - 1)/2;
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
    పూర్ణాంకం seq[DFT_MAX_STAGES + 1], nseq = 0;
    ఒకవేళ (r0 > 1) seq[nseq++] = r0;
    కొరకు (పూర్ణాంకం s = 0; s < nst; s++) seq[nseq++] = radices[s];
    పూర్ణాంకం digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        పూర్ణాంకం Ls = 1;
        కొరకు (పూర్ణాంకం s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    ఒకవేళ (r0 == 1)
    {
        p.itab.allocate(nc*2);
        పూర్ణాంకం* it_all = p.itab.దత్తాంశం();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        పూర్ణాంకం n_లోపల = std::min(nseq, 2);
        పూర్ణాంకం r_0 = n_లోపల > 0 ? seq[0] : 1, w_0 = n_లోపల > 0 ? weights[0] : 0;
        పూర్ణాంకం r_1 = n_లోపల > 1 ? seq[1] : 1, w_1 = n_లోపల > 1 ? weights[1] : 0;
        కొరకు (పూర్ణాంకం pos = 0, rev_hi = 0; pos < nc; )
        {
            కొరకు (పూర్ణాంకం d1 = 0; d1 < r_1; d1++)
            {
                పూర్ణాంకం rev = rev_hi + d1*w_1;
                పూర్ణాంకం* it = it_all + pos*2;
                ఒకవేళ (p.real_ఆదానం)
                {
                    కొరకు (పూర్ణాంకం d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                లేకపోతే
                {
                    కొరకు (పూర్ణాంకం d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            కొరకు (పూర్ణాంకం s = n_లోపల; s < nseq; s++)
            {
                rev_hi += weights[s];
                ఒకవేళ (++digits[s] < seq[s])
                    ఆపు;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    లేకపోతే
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        పూర్ణాంకం M = nc/r0;
        p.itab.allocate(M);
        పూర్ణాంకం* gtab = p.itab.దత్తాంశం();
        పూర్ణాంకం gw[DFT_MAX_STAGES + 1];
        {
            పూర్ణాంకం Ls = 1;
            కొరకు (పూర్ణాంకం s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        పూర్ణాంకం sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        పూర్ణాంకం r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        పూర్ణాంకం r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        కొరకు (పూర్ణాంకం పునాది = 0, g_hi = 0; పునాది < M; )
        {
            కొరకు (పూర్ణాంకం db = 0; db < r_b; db++)
            {
                పూర్ణాంకం g = (g_hi + db*g_b)*r0;
                కొరకు (పూర్ణాంకం da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[పునాది++] = g;
            }
            కొరకు (పూర్ణాంకం s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                ఒకవేళ (++digits[s] < seq[s])
                    ఆపు;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    తార్కిక need_rtw = real_రకం && n % 2 == 0;
    ఒకవేళ (need_rtw) ntw += 2*(size_t)(nc + 1);
    ఒకవేళ (dct_రకం) ntw += 2*(size_t)(nc + 1);
    ఒకవేళ (లోతు == CV_32F)
        fillTables<దశాంశం>(ntw, need_rtw, dct_రకం);
    లేకపోతే
        fillTables<ద్విగుణ>(ntw, need_rtw, dct_రకం);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_బైట్లు = 2*(size_t)nc*esz;
    తార్కిక two_జతలు = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_జతలు ? pair_బైట్లు : 0;
    p.scratch_ofs = pair_బైట్లు*(two_జతలు ? 2 : 1);
    size_t scratch_బైట్లు = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_బైట్లు = p.scratch_ofs + scratch_బైట్లు + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
స్థిర శూన్య నిరంతరం_Dft(స్థిరరాశి OcvDftOptions& c, స్థిరరాశి శూన్య* src, size_t sstep, శూన్య* dst, size_t dstep,
                   తార్కిక complex_io)
{
    స్థిరరాశి DftPlan& p = *c.plan;
    స్థిరరాశి DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.లోతు == CV_32F ? పరిమాణం(దశాంశం) : పరిమాణం(ద్విగుణ);
    uchar* పునాది = alignPtr(c.workspace, 64);
    uchar* re0 = పునాది;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = పునాది + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = పునాది + p.scratch_ofs;

    స్థిరరాశి శూన్య* asrc = src;
    size_t astep = sstep;
    తార్కిక conj = అబద్ధం;
    ఒకవేళ (p.రకం == DFT_రకం_C2_R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = నిజం;
    }
    లేకపోతే ఒకవేళ (p.రకం == DFT_రకం_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = నిజం;
    }
    లేకపోతే ఒకవేళ (p.రకం == DFT_రకం_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    లేకపోతే ఒకవేళ (p.రకం == DFT_రకం_C2_C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.మొదటి_radix == 1);

    DftPreprocFunc pre = p.మొదటి_radix == 1 ? k.preprocRadix0 : p.మొదటి_radix == 2 ? k.preprocRadix2 :
                         p.మొదటి_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    కొరకు (పూర్ణాంకం i = 0; i < p.nstages; i++)
    {
        స్థిరరాశి DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        ఒకవేళ (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        లేకపోతే
            fn(st, sre, sim, sre, sim, scratch);
    }

    ద్విగుణ scale = c.scale;
    ఎంపిక (p.రకం)
    {
    స్థితి DFT_రకం_C2_C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        ఆపు;
    స్థితి DFT_రకం_R2_C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        ఆపు;
    స్థితి DFT_రకం_C2_R:
        ఒకవేళ (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        లేకపోతే
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        ఆపు;
    స్థితి DFT_రకం_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        ఆపు;
    అప్రమేయ:
        CV_Assert(p.రకం == DFT_రకం_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
మూస<రకం_పేరు T> స్థిర శూన్య
DFT(స్థిరరాశి OcvDftOptions & c, స్థిరరాశి Complex<T>* src, Complex<T>* dst)
{
    ఒకవేళ( c.useIpp )
    {
#నిర్వచితమైతే USE_IPP_DFT
        ఒకవేళ( !c.isInverse )
        {
            ఒకవేళ (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_కూడు(CV_IMPL_IPP);
                తిరిగి;
            }
        }
        లేకపోతే
        {
            ఒకవేళ (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_కూడు(CV_IMPL_IPP);
                తిరిగి;
            }
        }
        సమితి_Ipp_Error_స్థితి();
#అయితే_ముగింపు
    }

    నిరంతరం_Dft(c, src, 1, dst, 1, అబద్ధం);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
మూస<రకం_పేరు T> స్థిర శూన్య
RealDFT(స్థిరరాశి OcvDftOptions & c, స్థిరరాశి T* src, T* dst)
{
    పూర్ణాంకం complex_output = c.isComplex;

    ఒకవేళ( c.useIpp )
    {
#నిర్వచితమైతే USE_IPP_DFT
        పూర్ణాంకం n = c.n;
        T* ipp_dst = dst + complex_output;
        ఒకవేళ (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            ఒకవేళ( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                ఒకవేళ( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_కూడు(CV_IMPL_IPP);
            తిరిగి;
        }
        సమితి_Ipp_Error_స్థితి();
#అయితే_ముగింపు
    }
    నిరంతరం_Dft(c, src, 1, dst, 1, complex_output != 0);
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
మూస<రకం_పేరు T> స్థిర శూన్య
CCSIDFT(స్థిరరాశి OcvDftOptions & c, స్థిరరాశి T* src, T* dst)
{
    పూర్ణాంకం complex_ఆదానం = c.isComplex;

    ఒకవేళ( complex_ఆదానం )
        CV_Assert( src != dst );
    ఒకవేళ( c.useIpp )
    {
#నిర్వచితమైతే USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        స్థిరరాశి T* ipp_src = src;
        T save_s1 = 0;
        ఒకవేళ( complex_ఆదానం )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        Ipp_స్థితి స్థితి_2 = ipps_DFTInv_Pack_గమ్యం_R( ipp_src, dst, c.ipp_spec, c.ipp_work );
        ఒకవేళ( complex_ఆదానం )
            ((T*)src)[1] = save_s1;
        ఒకవేళ( స్థితి_2 >= 0 )
        {
            CV_IMPL_కూడు(CV_IMPL_IPP);
            తిరిగి;
        }
        సమితి_Ipp_Error_స్థితి();
#అయితే_ముగింపు
    }
    నిరంతరం_Dft(c, src, 1, dst, 1, complex_ఆదానం != 0);
}

స్థిర శూన్య
Copy_స్తంభం( స్థిరరాశి uchar* _src, size_t src_అడుగు,
            uchar* _dst, size_t dst_అడుగు,
            పూర్ణాంకం len, size_t elem_కొలత_2 )
{
    పూర్ణాంకం i, t0, t1;
    స్థిరరాశి పూర్ణాంకం* src = (స్థిరరాశి పూర్ణాంకం*)_src;
    పూర్ణాంకం* dst = (పూర్ణాంకం*)_dst;
    src_అడుగు /= పరిమాణం(src[0]);
    dst_అడుగు /= పరిమాణం(dst[0]);

    ఒకవేళ( elem_కొలత_2 == పరిమాణం(పూర్ణాంకం) )
    {
        కొరకు( i = 0; i < len; i++, src += src_అడుగు, dst += dst_అడుగు )
            dst[0] = src[0];
    }
    లేకపోతే ఒకవేళ( elem_కొలత_2 == పరిమాణం(పూర్ణాంకం)*2 )
    {
        కొరకు( i = 0; i < len; i++, src += src_అడుగు, dst += dst_అడుగు )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    లేకపోతే ఒకవేళ( elem_కొలత_2 == పరిమాణం(పూర్ణాంకం)*4 )
    {
        కొరకు( i = 0; i < len; i++, src += src_అడుగు, dst += dst_అడుగు )
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
మూస<రకం_పేరు T> స్థిర అంతఃవరుస శూన్య
copyFromColumns_( స్థిరరాశి uchar* src, size_t src_అడుగు, T* dst, పూర్ణాంకం ncols, పూర్ణాంకం len )
{
    కొరకు( పూర్ణాంకం i = 0; i < len; i++, src += src_అడుగు )
    {
        స్థిరరాశి T* s = (స్థిరరాశి T*)src;
        కొరకు( పూర్ణాంకం k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

స్థిర శూన్య
CopyFromColumns( స్థిరరాశి uchar* src, size_t src_అడుగు, uchar* dst, పూర్ణాంకం ncols, పూర్ణాంకం len, size_t elem_కొలత_2 )
{
    ఒకవేళ( elem_కొలత_2 == పరిమాణం(పూర్ణాంకం) )
        copyFromColumns_(src, src_అడుగు, (పూర్ణాంకం*)dst, ncols, len);
    లేకపోతే ఒకవేళ( elem_కొలత_2 == పరిమాణం(int64) )
        copyFromColumns_(src, src_అడుగు, (int64*)dst, ncols, len);
    లేకపోతే
    {
        CV_Assert( elem_కొలత_2 == పరిమాణం(int64)*2 );
        copyFromColumns_(src, src_అడుగు, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
మూస<రకం_పేరు T> స్థిర అంతఃవరుస శూన్య
copy_గమ్యం_Columns( స్థిరరాశి T* src, uchar* dst, size_t dst_అడుగు, పూర్ణాంకం ncols, పూర్ణాంకం len )
{
    కొరకు( పూర్ణాంకం i = 0; i < len; i++, dst += dst_అడుగు )
    {
        T* d = (T*)dst;
        కొరకు( పూర్ణాంకం k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

స్థిర శూన్య
Copy_గమ్యం_Columns( స్థిరరాశి uchar* src, uchar* dst, size_t dst_అడుగు, పూర్ణాంకం ncols, పూర్ణాంకం len, size_t elem_కొలత_2 )
{
    ఒకవేళ( elem_కొలత_2 == పరిమాణం(పూర్ణాంకం) )
        copy_గమ్యం_Columns((స్థిరరాశి పూర్ణాంకం*)src, dst, dst_అడుగు, ncols, len);
    లేకపోతే ఒకవేళ( elem_కొలత_2 == పరిమాణం(int64) )
        copy_గమ్యం_Columns((స్థిరరాశి int64*)src, dst, dst_అడుగు, ncols, len);
    లేకపోతే
    {
        CV_Assert( elem_కొలత_2 == పరిమాణం(int64)*2 );
        copy_గమ్యం_Columns((స్థిరరాశి Vec<int64, 2>*)src, dst, dst_అడుగు, ncols, len);
    }
}

స్థిర శూన్య
ExpandCCS( uchar* _ptr, పూర్ణాంకం n, పూర్ణాంకం elem_కొలత_2 )
{
    పూర్ణాంకం i;
    ఒకవేళ( elem_కొలత_2 == (పూర్ణాంకం)పరిమాణం(దశాంశం) )
    {
        దశాంశం* p = (దశాంశం*)_ptr;
        కొరకు( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        ఒకవేళ( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        కొరకు( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    లేకపోతే
    {
        ద్విగుణ* p = (ద్విగుణ*)_ptr;
        కొరకు( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        ఒకవేళ( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        కొరకు( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

మూస<రకం_పేరు T, శూన్య (*fn)(స్థిరరాశి OcvDftOptions&, స్థిరరాశి T*, T*)>
స్థిర శూన్య dftWrap(స్థిరరాశి OcvDftOptions & c, స్థిరరాశి శూన్య* src, శూన్య* dst)
{
    fn(c, (స్థిరరాశి T*)src, (T*)dst);
}

}

#నిర్వచితమైతే USE_IPP_DFT
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL* Ipp_DFTGet_కొలత_Func)(పూర్ణాంకం, పూర్ణాంకం, IppHintAlgorithm, పూర్ణాంకం*, పూర్ణాంకం*, పూర్ణాంకం*);
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL* IppDFTInitFunc)(పూర్ణాంకం, పూర్ణాంకం, IppHintAlgorithm, శూన్య*, uchar*);

మూస<రకం_పేరు SpecType, Ipp_స్థితి (CV_STDCALL *init_fn)(పూర్ణాంకం, పూర్ణాంకం, IppHintAlgorithm, SpecType*, Ipp8u*)>
స్థిర Ipp_స్థితి CV_STDCALL ippDFTInitWrap(పూర్ణాంకం n, పూర్ణాంకం flags, IppHintAlgorithm hint, శూన్య* spec, Ipp8u* initbuf)
{
    తిరిగి init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#అయితే_ముగింపు

నామాకాశ cv
{
#ఒకవేళ defined USE_IPP_DFT

రకనిర్వచనం Ipp_స్థితి (CV_STDCALL* ippiDFT_C_Func)(స్థిరరాశి Ipp32fc*, పూర్ణాంకం, Ipp32fc*, పూర్ణాంకం, స్థిరరాశి IppiDFTSpec_C_32fc*, Ipp8u*);
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL* ippiDFT_R_Func)(స్థిరరాశి Ipp32f* , పూర్ణాంకం, Ipp32f* , పూర్ణాంకం, స్థిరరాశి IppiDFTSpec_R_32f* , Ipp8u*);

మూస <రకం_పేరు Dft>
తరగతి Dft_C_IPPLoop_Invoker : బహిరంగ ParallelLoopBody
{
బహిరంగ:

    Dft_C_IPPLoop_Invoker(స్థిరరాశి uchar * _src, size_t src_అడుగు_2, uchar * _dst, size_t dst_అడుగు_2, పూర్ణాంకం వెడల్పు_3,
                          స్థిరరాశి Dft& _ippidft, పూర్ణాంకం _norm_flag, తార్కిక *_ok) :
        ParallelLoopBody(),
        src(_src), src_అడుగు(src_అడుగు_2), dst(_dst), dst_అడుగు(dst_అడుగు_2), వెడల్పు(వెడల్పు_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = నిజం;
    }

    మాయా శూన్య క్రియాకారకం()(స్థిరరాశి Range& range) స్థిరరాశి CV_OVERRIDE
    {
        Ipp_స్థితి స్థితి_2;
        Ipp8u* p_బఫరు = 0;
        Ipp8u* pMemInit= 0;
        పూర్ణాంకం కొలత_బఫరు=0;
        పూర్ణాంకం కొలత_Spec=0;
        పూర్ణాంకం కొలత_Init=0;

        Ippi_కొలత src_Roi_కొలత = {వెడల్పు, 1};

        స్థితి_2 = ippi_DFTGet_కొలత_C_32fc(src_Roi_కొలత, norm_flag, ippAlgHintNone, &కొలత_Spec, &కొలత_Init, &కొలత_బఫరు );
        ఒకవేళ ( స్థితి_2 < 0 )
        {
            *ok = అబద్ధం;
            తిరిగి;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( కొలత_Spec );

        ఒకవేళ ( కొలత_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( కొలత_Init );

        ఒకవేళ ( కొలత_బఫరు > 0 )
            p_బఫరు = (Ipp8u*)CV_IPP_MALLOC( కొలత_బఫరు );

        స్థితి_2 = ippiDFTInit_C_32fc( src_Roi_కొలత, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        ఒకవేళ ( కొలత_Init > 0 )
            ippFree( pMemInit );

        ఒకవేళ ( స్థితి_2 < 0 )
        {
            ippFree( pDFTSpec );
            ఒకవేళ ( కొలత_బఫరు > 0 )
                ippFree( p_బఫరు );
            *ok = అబద్ధం;
            తిరిగి;
        }

        కొరకు( పూర్ణాంకం i = range.ప్రారంభం; i < range.end; ++i)
            ఒకవేళ(!ippidft((Ipp32fc*)(src + src_అడుగు * i), src_అడుగు, (Ipp32fc*)(dst + dst_అడుగు * i), dst_అడుగు,
                        pDFTSpec, (Ipp8u*)p_బఫరు))
            {
                *ok = అబద్ధం;
            }

        ఒకవేళ ( కొలత_బఫరు > 0 )
            ippFree( p_బఫరు );

        ippFree( pDFTSpec );
        CV_IMPL_కూడు(CV_IMPL_IPP|CV_IMPL_MT);
    }

అంతరంగ:
    స్థిరరాశి uchar * src;
    size_t src_అడుగు;
    uchar * dst;
    size_t dst_అడుగు;
    పూర్ణాంకం వెడల్పు;
    స్థిరరాశి Dft& ippidft;
    పూర్ణాంకం norm_flag;
    తార్కిక *ok;

    స్థిరరాశి Dft_C_IPPLoop_Invoker& క్రియాకారకం= (స్థిరరాశి Dft_C_IPPLoop_Invoker&);
};

మూస <రకం_పేరు Dft>
తరగతి Dft_R_IPPLoop_Invoker : బహిరంగ ParallelLoopBody
{
బహిరంగ:

    Dft_R_IPPLoop_Invoker(స్థిరరాశి uchar * _src, size_t src_అడుగు_2, uchar * _dst, size_t dst_అడుగు_2, పూర్ణాంకం వెడల్పు_3,
                          స్థిరరాశి Dft& _ippidft, పూర్ణాంకం _norm_flag, తార్కిక *_ok) :
        ParallelLoopBody(),
        src(_src), src_అడుగు(src_అడుగు_2), dst(_dst), dst_అడుగు(dst_అడుగు_2), వెడల్పు(వెడల్పు_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = నిజం;
    }

    మాయా శూన్య క్రియాకారకం()(స్థిరరాశి Range& range) స్థిరరాశి CV_OVERRIDE
    {
        Ipp_స్థితి స్థితి_2;
        Ipp8u* p_బఫరు = 0;
        Ipp8u* pMemInit= 0;
        పూర్ణాంకం కొలత_బఫరు=0;
        పూర్ణాంకం కొలత_Spec=0;
        పూర్ణాంకం కొలత_Init=0;

        Ippi_కొలత src_Roi_కొలత = {వెడల్పు, 1};

        స్థితి_2 = ippi_DFTGet_కొలత_R_32f(src_Roi_కొలత, norm_flag, ippAlgHintNone, &కొలత_Spec, &కొలత_Init, &కొలత_బఫరు );
        ఒకవేళ ( స్థితి_2 < 0 )
        {
            *ok = అబద్ధం;
            తిరిగి;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( కొలత_Spec );

        ఒకవేళ ( కొలత_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( కొలత_Init );

        ఒకవేళ ( కొలత_బఫరు > 0 )
            p_బఫరు = (Ipp8u*)CV_IPP_MALLOC( కొలత_బఫరు );

        స్థితి_2 = ippiDFTInit_R_32f( src_Roi_కొలత, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        ఒకవేళ ( కొలత_Init > 0 )
            ippFree( pMemInit );

        ఒకవేళ ( స్థితి_2 < 0 )
        {
            ippFree( pDFTSpec );
            ఒకవేళ ( కొలత_బఫరు > 0 )
                ippFree( p_బఫరు );
            *ok = అబద్ధం;
            తిరిగి;
        }

        కొరకు( పూర్ణాంకం i = range.ప్రారంభం; i < range.end; ++i)
            ఒకవేళ(!ippidft((దశాంశం*)(src + src_అడుగు * i), src_అడుగు, (దశాంశం*)(dst + dst_అడుగు * i), dst_అడుగు,
                        pDFTSpec, (Ipp8u*)p_బఫరు))
            {
                *ok = అబద్ధం;
            }

        ఒకవేళ ( కొలత_బఫరు > 0 )
            ippFree( p_బఫరు );

        ippFree( pDFTSpec );
        CV_IMPL_కూడు(CV_IMPL_IPP|CV_IMPL_MT);
    }

అంతరంగ:
    స్థిరరాశి uchar * src;
    size_t src_అడుగు;
    uchar * dst;
    size_t dst_అడుగు;
    పూర్ణాంకం వెడల్పు;
    స్థిరరాశి Dft& ippidft;
    పూర్ణాంకం norm_flag;
    తార్కిక *ok;

    స్థిరరాశి Dft_R_IPPLoop_Invoker& క్రియాకారకం= (స్థిరరాశి Dft_R_IPPLoop_Invoker&);
};

మూస <రకం_పేరు Dft>
తార్కిక Dft_C_IPPLoop(స్థిరరాశి uchar * src, size_t src_అడుగు, uchar * dst, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి Dft& ippidft, పూర్ణాంకం norm_flag)
{
    తార్కిక ok;
    parallel_for_(Range(0, ఎత్తు), Dft_C_IPPLoop_Invoker<Dft>(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, ippidft, norm_flag, &ok), (వెడల్పు * ఎత్తు)/(ద్విగుణ)(1<<16) );
    తిరిగి ok;
}

మూస <రకం_పేరు Dft>
తార్కిక Dft_R_IPPLoop(స్థిరరాశి uchar * src, size_t src_అడుగు, uchar * dst, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి Dft& ippidft, పూర్ణాంకం norm_flag)
{
    తార్కిక ok;
    parallel_for_(Range(0, ఎత్తు), Dft_R_IPPLoop_Invoker<Dft>(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, ippidft, norm_flag, &ok), (వెడల్పు * ఎత్తు)/(ద్విగుణ)(1<<16) );
    తిరిగి ok;
}

నిర్మాణం IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    తార్కిక క్రియాకారకం()(స్థిరరాశి Ipp32fc* src, size_t src_అడుగు_3, Ipp32fc* dst, size_t dst_అడుగు_3, స్థిరరాశి IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* p_బఫరు) స్థిరరాశి
    {
        తిరిగి ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<పూర్ణాంకం>(src_అడుగు_3), dst, static_cast<పూర్ణాంకం>(dst_అడుగు_3), pDFTSpec, p_బఫరు) >= 0 : అబద్ధం;
    }
అంతరంగ:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

నిర్మాణం IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippi_DFT_Pack_గమ్యం_R_32f_C1_R(_func){}

    తార్కిక క్రియాకారకం()(స్థిరరాశి Ipp32f* src, size_t src_అడుగు_3, Ipp32f* dst, size_t dst_అడుగు_3, స్థిరరాశి IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* p_బఫరు) స్థిరరాశి
    {
        తిరిగి ippi_DFT_Pack_గమ్యం_R_32f_C1_R ? CV_INSTRUMENT_FUN_IPP(ippi_DFT_Pack_గమ్యం_R_32f_C1_R, src, static_cast<పూర్ణాంకం>(src_అడుగు_3), dst, static_cast<పూర్ణాంకం>(dst_అడుగు_3), pDFTSpec, p_బఫరు) >= 0 : అబద్ధం;
    }
అంతరంగ:
    ippiDFT_R_Func ippi_DFT_Pack_గమ్యం_R_32f_C1_R;
};

స్థిర తార్కిక ippi_DFT_C_32F(స్థిరరాశి uchar * src, size_t src_అడుగు, uchar * dst, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, తార్కిక inv, పూర్ణాంకం norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_స్థితి స్థితి_2;
    Ipp8u* p_బఫరు = 0;
    Ipp8u* pMemInit= 0;
    పూర్ణాంకం కొలత_బఫరు=0;
    పూర్ణాంకం కొలత_Spec=0;
    పూర్ణాంకం కొలత_Init=0;

    Ippi_కొలత src_Roi_కొలత = {వెడల్పు, ఎత్తు};

    స్థితి_2 = ippi_DFTGet_కొలత_C_32fc(src_Roi_కొలత, norm_flag, ippAlgHintNone, &కొలత_Spec, &కొలత_Init, &కొలత_బఫరు );
    ఒకవేళ ( స్థితి_2 < 0 )
        తిరిగి అబద్ధం;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( కొలత_Spec );

    ఒకవేళ ( కొలత_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( కొలత_Init );

    ఒకవేళ ( కొలత_బఫరు > 0 )
        p_బఫరు = (Ipp8u*)CV_IPP_MALLOC( కొలత_బఫరు );

    స్థితి_2 = ippiDFTInit_C_32fc( src_Roi_కొలత, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    ఒకవేళ ( కొలత_Init > 0 )
        ippFree( pMemInit );

    ఒకవేళ ( స్థితి_2 < 0 )
    {
        ippFree( pDFTSpec );
        ఒకవేళ ( కొలత_బఫరు > 0 )
            ippFree( p_బఫరు );
        తిరిగి అబద్ధం;
    }

    ఒకవేళ (!inv)
        స్థితి_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<పూర్ణాంకం>(src_అడుగు), (Ipp32fc*)dst, static_cast<పూర్ణాంకం>(dst_అడుగు), pDFTSpec, p_బఫరు);
    లేకపోతే
        స్థితి_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<పూర్ణాంకం>(src_అడుగు), (Ipp32fc*)dst, static_cast<పూర్ణాంకం>(dst_అడుగు), pDFTSpec, p_బఫరు);

    ఒకవేళ ( కొలత_బఫరు > 0 )
        ippFree( p_బఫరు );

    ippFree( pDFTSpec );

    ఒకవేళ(స్థితి_2 >= 0)
    {
        CV_IMPL_కూడు(CV_IMPL_IPP);
        తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
}

స్థిర తార్కిక ippi_DFT_R_32F(స్థిరరాశి uchar * src, size_t src_అడుగు, uchar * dst, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, తార్కిక inv, పూర్ణాంకం norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_స్థితి స్థితి_2;
    Ipp8u* p_బఫరు = 0;
    Ipp8u* pMemInit= 0;
    పూర్ణాంకం కొలత_బఫరు=0;
    పూర్ణాంకం కొలత_Spec=0;
    పూర్ణాంకం కొలత_Init=0;

    Ippi_కొలత src_Roi_కొలత = {వెడల్పు, ఎత్తు};

    స్థితి_2 = ippi_DFTGet_కొలత_R_32f(src_Roi_కొలత, norm_flag, ippAlgHintNone, &కొలత_Spec, &కొలత_Init, &కొలత_బఫరు );
    ఒకవేళ ( స్థితి_2 < 0 )
        తిరిగి అబద్ధం;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( కొలత_Spec );

    ఒకవేళ ( కొలత_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( కొలత_Init );

    ఒకవేళ ( కొలత_బఫరు > 0 )
        p_బఫరు = (Ipp8u*)CV_IPP_MALLOC( కొలత_బఫరు );

    స్థితి_2 = ippiDFTInit_R_32f( src_Roi_కొలత, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    ఒకవేళ ( కొలత_Init > 0 )
        ippFree( pMemInit );

    ఒకవేళ ( స్థితి_2 < 0 )
    {
        ippFree( pDFTSpec );
        ఒకవేళ ( కొలత_బఫరు > 0 )
            ippFree( p_బఫరు );
        తిరిగి అబద్ధం;
    }

    ఒకవేళ (!inv)
        స్థితి_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (దశాంశం*)src, static_cast<పూర్ణాంకం>(src_అడుగు), (దశాంశం*)dst, static_cast<పూర్ణాంకం>(dst_అడుగు), pDFTSpec, p_బఫరు);
    లేకపోతే
        స్థితి_2 = CV_INSTRUMENT_FUN_IPP(ippi_DFTInv_Pack_గమ్యం_R_32f_C1_R, (దశాంశం*)src, static_cast<పూర్ణాంకం>(src_అడుగు), (దశాంశం*)dst, static_cast<పూర్ణాంకం>(dst_అడుగు), pDFTSpec, p_బఫరు);

    ఒకవేళ ( కొలత_బఫరు > 0 )
        ippFree( p_బఫరు );

    ippFree( pDFTSpec );

    ఒకవేళ(స్థితి_2 >= 0)
    {
        CV_IMPL_కూడు(CV_IMPL_IPP);
        తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
}

#అయితే_ముగింపు
}

#నిర్వచితమైతే HAVE_OPENCL

నామాకాశ cv
{

గణన FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

నిర్మాణం OCL_FftPlan
{
అంతరంగ:
    UMat twiddles;
    String buildOptions;
    పూర్ణాంకం thread_లెక్క;
    పూర్ణాంకం dft_కొలత;
    పూర్ణాంకం dft_లోతు;
    తార్కిక స్థితి_2;

బహిరంగ:
    OCL_FftPlan(పూర్ణాంకం కొలత_4, పూర్ణాంకం లోతు_2) : dft_కొలత(కొలత_4), dft_లోతు(లోతు_2), స్థితి_2(నిజం)
    {
        CV_Assert( dft_లోతు == CV_32F || dft_లోతు == CV_64F );

        పూర్ణాంకం min_radix;
        std::vector<పూర్ణాంకం> radixes, ఖండాలు;
        ocl_getRadixes(dft_కొలత, radixes, ఖండాలు, min_radix);
        thread_లెక్క = dft_కొలత / min_radix;

        ఒకవేళ (thread_లెక్క > (పూర్ణాంకం) ocl::Device::getDefault().max_Work_సమూహం_కొలత())
        {
            స్థితి_2 = అబద్ధం;
            తిరిగి;
        }

        // generate string with radix calls
        String radix_processing;
        పూర్ణాంకం n = 1, twiddle_కొలత = 0;
        కొరకు (size_t i=0; i<radixes.size(); i++)
        {
            పూర్ణాంకం radix = radixes[i], ఖండం_2 = ఖండాలు[i];
            ఒకవేళ (ఖండం_2 > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, ఖండం_2, twiddle_కొలత, n, dft_కొలత/radix);
            లేకపోతే
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_కొలత, n, dft_కొలత/radix);
            twiddle_కొలత += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_కొలత, CV_MAKE_TYPE(dft_లోతు, 2));
        ఒకవేళ (dft_లోతు == CV_32F)
            fill_Radix_పట్టిక<దశాంశం>(twiddles, radixes);
        లేకపోతే
            fill_Radix_పట్టిక<ద్విగుణ>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_కొలత, min_radix, ocl::type_గమ్యం_Str(dft_లోతు), ocl::type_గమ్యం_Str(CV_MAKE_TYPE(dft_లోతు, 2)),
                              dft_లోతు == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    తార్కిక వరుసలో_చేర్చు_Transform(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం అంకె_dfts, పూర్ణాంకం flags, పూర్ణాంకం fftType, తార్కిక అడ్డువరుసలు = నిజం) స్థిరరాశి
    {
        ఒకవేళ (!స్థితి_2)
            తిరిగి అబద్ధం;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        తార్కిక is1d = (flags & DFT_అడ్డువరుసలు) != 0 || అంకె_dfts == 1;
        తార్కిక inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        ఒకవేళ (అడ్డువరుసలు)
        {
            globalsize[0] = thread_లెక్క; globalsize[1] = src.అడ్డువరుసలు;
            localsize[0] = thread_లెక్క; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            ఒకవేళ ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        లేకపోతే
        {
            globalsize[0] = అంకె_dfts; globalsize[1] = thread_లెక్క;
            localsize[0] = 1; localsize[1] = thread_లెక్క;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            ఒకవేళ (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        ఒకవేళ (!inv)
        {
            ఒకవేళ ((is1d && src.channels() == 1) || (అడ్డువరుసలు && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        లేకపోతే
        {
            ఒకవేళ (అడ్డువరుసలు && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            ఒకవేళ (dst.నిలువుల_సంఖ్య % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        ఒకవేళ (k.empty())
            తిరిగి అబద్ధం;

        k.వాదనలు(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::Read_Only_No_కొలత(twiddles), thread_లెక్క, అంకె_dfts);
        తిరిగి k.నిరంతరం(2, globalsize, localsize, అబద్ధం);
    }

అంతరంగ:
    స్థిర శూన్య ocl_getRadixes(పూర్ణాంకం నిలువుల_సంఖ్య, std::vector<పూర్ణాంకం>& radixes, std::vector<పూర్ణాంకం>& ఖండాలు, పూర్ణాంకం& min_radix)
    {
        పూర్ణాంకం factors[34];
        పూర్ణాంకం nf = DFTFactorize(నిలువుల_సంఖ్య, factors);

        పూర్ణాంకం n = 1;
        పూర్ణాంకం గుణకం_సూచీ = 0;
        min_radix = గరిష్ఠ_పూర్ణాంకం;

        // 2^n transforms
        ఒకవేళ ((factors[గుణకం_సూచీ] & 1) == 0)
        {
            కొరకు( ; n < factors[గుణకం_సూచీ];)
            {
                పూర్ణాంకం radix = 2, ఖండం_2 = 1;
                ఒకవేళ (8*n <= factors[0])
                    radix = 8;
                లేకపోతే ఒకవేళ (4*n <= factors[0])
                {
                    radix = 4;
                    ఒకవేళ (నిలువుల_సంఖ్య % 12 == 0)
                        ఖండం_2 = 3;
                    లేకపోతే ఒకవేళ (నిలువుల_సంఖ్య % 8 == 0)
                        ఖండం_2 = 2;
                }
                లేకపోతే
                {
                    ఒకవేళ (నిలువుల_సంఖ్య % 10 == 0)
                        ఖండం_2 = 5;
                    లేకపోతే ఒకవేళ (నిలువుల_సంఖ్య % 8 == 0)
                        ఖండం_2 = 4;
                    లేకపోతే ఒకవేళ (నిలువుల_సంఖ్య % 6 == 0)
                        ఖండం_2 = 3;
                    లేకపోతే ఒకవేళ (నిలువుల_సంఖ్య % 4 == 0)
                        ఖండం_2 = 2;
                }

                radixes.push_back(radix);
                ఖండాలు.push_back(ఖండం_2);
                min_radix = min(min_radix, ఖండం_2*radix);
                n *= radix;
            }
            గుణకం_సూచీ++;
        }

        // all the other transforms
        కొరకు( ; గుణకం_సూచీ < nf; గుణకం_సూచీ++)
        {
            పూర్ణాంకం radix = factors[గుణకం_సూచీ], ఖండం_2 = 1;
            ఒకవేళ (radix == 3)
            {
                ఒకవేళ (నిలువుల_సంఖ్య % 12 == 0)
                    ఖండం_2 = 4;
                లేకపోతే ఒకవేళ (నిలువుల_సంఖ్య % 9 == 0)
                    ఖండం_2 = 3;
                లేకపోతే ఒకవేళ (నిలువుల_సంఖ్య % 6 == 0)
                    ఖండం_2 = 2;
            }
            లేకపోతే ఒకవేళ (radix == 5)
            {
                ఒకవేళ (నిలువుల_సంఖ్య % 10 == 0)
                    ఖండం_2 = 2;
            }
            radixes.push_back(radix);
            ఖండాలు.push_back(ఖండం_2);
            min_radix = min(min_radix, ఖండం_2*radix);
        }
    }

    మూస <రకం_పేరు T>
    స్థిర శూన్య fill_Radix_పట్టిక(UMat twiddles, స్థిరరాశి std::vector<పూర్ణాంకం>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        పూర్ణాంకం ptr_సూచీ = 0;

        పూర్ణాంకం n = 1;
        కొరకు (size_t i=0; i<radixes.size(); i++)
        {
            పూర్ణాంకం radix = radixes[i];
            n *= radix;

            కొరకు (పూర్ణాంకం j=1; j<radix; j++)
            {
                ద్విగుణ theta = -CV_2PI*j/n;

                కొరకు (పూర్ణాంకం k=0; k<(n/radix); k++)
                {
                    ptr[ptr_సూచీ++] = (T) కోజ్యా(k*theta);
                    ptr[ptr_సూచీ++] = (T) జ్యా(k*theta);
                }
            }
        }
    }
};

తరగతి OCL_FftPlanCache
{
బహిరంగ:
    స్థిర OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, కొత్త OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(పూర్ణాంకం dft_కొలత, పూర్ణాంకం లోతు)
    {
        పూర్ణాంకం కీలకం = (dft_కొలత << 16) | (లోతు & 0xFFFF);
        std::map<పూర్ణాంకం, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(కీలకం);
        ఒకవేళ (f != planStorage.end())
        {
            తిరిగి f->second;
        }
        లేకపోతే
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(కొత్త OCL_FftPlan(dft_కొలత, లోతు));
            planStorage[కీలకం] = newPlan;
            తిరిగి newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

రక్షిత:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<పూర్ణాంకం, Ptr<OCL_FftPlan> > planStorage;
};

స్థిర తార్కిక ocl_dft_అడ్డువరుసలు(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం nonzero_అడ్డువరుసలు, పూర్ణాంకం flags, పూర్ణాంకం fftType)
{
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.నిలువుల_సంఖ్య(), లోతు);
    తిరిగి plan->వరుసలో_చేర్చు_Transform(_src, _dst, nonzero_అడ్డువరుసలు, flags, fftType, నిజం);
}

స్థిర తార్కిక ocl_dft_నిలువుల_సంఖ్య(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం nonzero_నిలువుల_సంఖ్య, పూర్ణాంకం flags, పూర్ణాంకం fftType)
{
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.అడ్డువరుసలు(), లోతు);
    తిరిగి plan->వరుసలో_చేర్చు_Transform(_src, _dst, nonzero_నిలువుల_సంఖ్య, flags, fftType, అబద్ధం);
}

అంతఃవరుస FftType determineFFTType(తార్కిక real_ఆదానం, తార్కిక complex_ఆదానం, తార్కిక real_output, తార్కిక complex_output, తార్కిక inv)
{
    // output format is not specified
    ఒకవేళ (!real_output && !complex_output)
        complex_output = నిజం;

    // input or output format is ambiguous
    ఒకవేళ (real_ఆదానం == complex_ఆదానం || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType ఫలితం = real_ఆదానం ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    ఒకవేళ (ఫలితం == C2R && !inv)
        ఫలితం = C2C;

    // Inverse CCS to Complex not supported
    ఒకవేళ (ఫలితం == R2C && inv)
        ఫలితం = R2R;

    తిరిగి ఫలితం;
}

స్థిర తార్కిక ocl_dft(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం flags, పూర్ణాంకం nonzero_అడ్డువరుసలు)
{
    పూర్ణాంకం type = _src.type(), cn = CV_MAT_CN(type), లోతు = CV_MAT_లోతు(type);
    కొలత ssize = _src.size();
    తార్కిక doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ఒకవేళ (!(cn == 1 || cn == 2)
        || !(లోతు == CV_32F || (లోతు == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        తిరిగి అబద్ధం;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    ఒకవేళ (ssize.area() != getOptimalDFTSize(ssize.area()))
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat();
    తార్కిక inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    ఒకవేళ( nonzero_అడ్డువరుసలు <= 0 || nonzero_అడ్డువరుసలు > _src.అడ్డువరుసలు() )
        nonzero_అడ్డువరుసలు = _src.అడ్డువరుసలు();
    తార్కిక is1d = (flags & DFT_అడ్డువరుసలు) != 0 || nonzero_అడ్డువరుసలు == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    ఒకవేళ (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.create_Same_కొలత(src, CV_MAKETYPE(లోతు, 2));
        output = _dst.getUMat();
    }
    లేకపోతే
    {
        // real output
        ఒకవేళ (is1d)
        {
            _dst.create_Same_కొలత(src, CV_MAKETYPE(లోతు, 1));
            output = _dst.getUMat();
        }
        లేకపోతే
        {
            _dst.create_Same_కొలత(src, CV_MAKETYPE(లోతు, 1));
            output.create(src.size, CV_MAKETYPE(లోతు, 2));
        }
    }

    తార్కిక ఫలితం = అబద్ధం;
    ఒకవేళ (!inv)
    {
        పూర్ణాంకం nonzero_నిలువుల_సంఖ్య = fftType == R2R ? output.నిలువుల_సంఖ్య/2 + 1 : output.నిలువుల_సంఖ్య;
        ఫలితం = ocl_dft_అడ్డువరుసలు(src, output, nonzero_అడ్డువరుసలు, flags, fftType);
        ఒకవేళ (!is1d)
            ఫలితం = ఫలితం && ocl_dft_నిలువుల_సంఖ్య(output, _dst, nonzero_నిలువుల_సంఖ్య, flags, fftType);
    }
    లేకపోతే
    {
        ఒకవేళ (fftType == C2C)
        {
            // complex output
            ఫలితం = ocl_dft_అడ్డువరుసలు(src, output, nonzero_అడ్డువరుసలు, flags, fftType);
            ఒకవేళ (!is1d)
                ఫలితం = ఫలితం && ocl_dft_నిలువుల_సంఖ్య(output, output, output.నిలువుల_సంఖ్య, flags, fftType);
        }
        లేకపోతే
        {
            ఒకవేళ (is1d)
            {
                ఫలితం = ocl_dft_అడ్డువరుసలు(src, output, nonzero_అడ్డువరుసలు, flags, fftType);
            }
            లేకపోతే
            {
                పూర్ణాంకం nonzero_నిలువుల_సంఖ్య = src.నిలువుల_సంఖ్య/2 + 1;
                ఫలితం = ocl_dft_నిలువుల_సంఖ్య(src, output, nonzero_నిలువుల_సంఖ్య, flags, fftType);
                ఫలితం = ఫలితం && ocl_dft_అడ్డువరుసలు(output, _dst, nonzero_అడ్డువరుసలు, flags, fftType);
            }
        }
    }
    తిరిగి ఫలితం;
}

} // namespace cv;

#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_CLAMDFFT

నామాకాశ cv {

#నిర్వచించు CLAMDDFT_Assert(func) \
    { \
        clfft_స్థితి s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

తరగతి PlanCache
{
    నిర్మాణం FftPlan
    {
        FftPlan(స్థిరరాశి కొలత & dft_కొలత_2, పూర్ణాంకం src_అడుగు_2, పూర్ణాంకం dst_అడుగు_2, తార్కిక _doubleFP, తార్కిక _inplace, పూర్ణాంకం _flags, FftType _fftType) :
            dft_కొలత(dft_కొలత_2), src_అడుగు(src_అడుగు_2), dst_అడుగు(dst_అడుగు_2),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            తార్కిక dft_inverse = (flags & DFT_INVERSE) != 0;
            తార్కిక dft_scale = (flags & DFT_SCALE) != 0;
            తార్కిక dft_అడ్డువరుసలు = (flags & DFT_అడ్డువరుసలు) != 0;

            clfftLayout లోపల_Layout = CLFFT_REAL, బహిర్గతం_Layout = CLFFT_REAL;
            clfftDim dim = dft_కొలత.ఎత్తు == 1 || dft_అడ్డువరుసలు ? CLFFT_1D : CLFFT_2D;

            size_t batch_కొలత = dft_అడ్డువరుసలు ? dft_కొలత.ఎత్తు : 1;
            size_t cl_Lengths_లోపల[3] = { (size_t)dft_కొలత.వెడల్పు, dft_అడ్డువరుసలు ? 1 : (size_t)dft_కొలత.ఎత్తు, 1 };
            size_t cl_Strides_లోపల[3] = { 1, 1, 1 };
            size_t cl_Strides_బహిర్గతం[3]  = { 1, 1, 1 };
            పూర్ణాంకం elem_కొలత = doubleFP ? పరిమాణం(ద్విగుణ) : పరిమాణం(దశాంశం);

            ఎంపిక (fftType)
            {
            స్థితి C2C:
                లోపల_Layout = CLFFT_COMPLEX_INTERLEAVED;
                బహిర్గతం_Layout = CLFFT_COMPLEX_INTERLEAVED;
                cl_Strides_లోపల[1] = src_అడుగు / (elem_కొలత << 1);
                cl_Strides_బహిర్గతం[1] = dst_అడుగు / (elem_కొలత << 1);
                ఆపు;
            స్థితి R2C:
                లోపల_Layout = CLFFT_REAL;
                బహిర్గతం_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                cl_Strides_లోపల[1] = src_అడుగు / elem_కొలత;
                cl_Strides_బహిర్గతం[1] = dst_అడుగు / (elem_కొలత << 1);
                ఆపు;
            స్థితి C2R:
                లోపల_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                బహిర్గతం_Layout = CLFFT_REAL;
                cl_Strides_లోపల[1] = src_అడుగు / (elem_కొలత << 1);
                cl_Strides_బహిర్గతం[1] = dst_అడుగు / elem_కొలత;
                ఆపు;
            స్థితి R2R:
            అప్రమేయ:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                ఆపు;
            }

            cl_Strides_లోపల[2] = dft_అడ్డువరుసలు ? cl_Strides_లోపల[1] : dft_కొలత.వెడల్పు * cl_Strides_లోపల[1];
            cl_Strides_బహిర్గతం[2] = dft_అడ్డువరుసలు ? cl_Strides_బహిర్గతం[1] : dft_కొలత.వెడల్పు * cl_Strides_బహిర్గతం[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, cl_Lengths_లోపల))

            // setting plan properties
            CLAMDDFT_Assert(clfft_సమితి_Plan_Precision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfft_సమితి_ఫలితం_Location(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfft_సమితి_Layout(plHandle, లోపల_Layout, బహిర్గతం_Layout))
            CLAMDDFT_Assert(clfft_సమితి_Plan_Batch_కొలత(plHandle, batch_కొలత))
            CLAMDDFT_Assert(clfft_సమితి_Plan_లోపల_Stride(plHandle, dim, cl_Strides_లోపల))
            CLAMDDFT_Assert(clfft_సమితి_Plan_బహిర్గతం_Stride(plHandle, dim, cl_Strides_బహిర్గతం))
            CLAMDDFT_Assert(clfft_సమితి_Plan_దూరం(plHandle, cl_Strides_లోపల[dim], cl_Strides_బహిర్గతం[dim]))

            దశాంశం scale = dft_scale ? 1.0f / (dft_అడ్డువరుసలు ? dft_కొలత.వెడల్పు : dft_కొలత.area()) : 1.0f;
            CLAMDDFT_Assert(clfft_సమితి_Plan_Scale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_వరుస వరుస_2 = (cl_command_వరుస)ocl::వరుస::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &వరుస_2, ఖాళీ, ఖాళీ))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        మిత్ర తరగతి PlanCache;

    అంతరంగ:
        కొలత dft_కొలత;
        పూర్ణాంకం src_అడుగు, dst_అడుగు;
        తార్కిక doubleFP;
        తార్కిక inplace;
        పూర్ణాంకం flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

బహిరంగ:
    స్థిర PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, కొత్త PlanCache())
    }

    clfftPlanHandle getPlanHandle(స్థిరరాశి కొలత & dft_కొలత, పూర్ణాంకం src_అడుగు, పూర్ణాంకం dst_అడుగు, తార్కిక doubleFP,
                                  తార్కిక inplace, పూర్ణాంకం flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        కొరకు (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            స్థిరరాశి FftPlan * స్థిరరాశి plan = planStorage[i];

            ఒకవేళ (plan->dft_కొలత == dft_కొలత &&
                plan->flags == flags &&
                plan->src_అడుగు == src_అడుగు &&
                plan->dst_అడుగు == dst_అడుగు &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                ఒకవేళ (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    ఆపు;
                }

                తిరిగి plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(కొత్త FftPlan(dft_కొలత, src_అడుగు, dst_అడుగు, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        తిరిగి newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

రక్షిత:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

బాహ్య "C" {

స్థిర శూన్య CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, శూన్య *p)
{
    UMat_దత్తాంశం * u = (UMat_దత్తాంశం *)p;

    ఒకవేళ( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

స్థిర తార్కిక ocl_dft_amdfft(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం flags)
{
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type);
    కొలత ssize = _src.size();

    తార్కిక doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    ఒకవేళ ( (!doubleSupport && లోతు == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        తిరిగి అబద్ధం;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    ఒకవేళ (ssize.area() != getOptimalDFTSize(ssize.area()))
        తిరిగి అబద్ధం;

    పూర్ణాంకం dst_complex_ఆదానం = cn == 2 ? 1 : 0;
    తార్కిక dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    పూర్ణాంకం dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    తార్కిక dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_ఆదానం << 0 | dft_complex_output << 1);

    ఎంపిక (fftType)
    {
    స్థితి C2C:
        _dst.create(ssize.ఎత్తు, ssize.వెడల్పు, CV_MAKE_TYPE(లోతు, 2));
        ఆపు;
    స్థితి R2C: // TODO implement it if possible
    స్థితి C2R: // TODO implement it if possible
    స్థితి R2R: // AMD Fft does not support this type
    అప్రమేయ:
        తిరిగి అబద్ధం;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    ఒకవేళ (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        తిరిగి అబద్ధం;
    తార్కిక inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (పూర్ణాంకం)src.అడుగు, (పూర్ణాంకం)dst.అడుగు,
                          లోతు == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t బఫరు_కొలత = 0;
    CLAMDDFT_Assert(clfft_Get_Tmp_Buf_కొలత(plHandle, &బఫరు_కొలత))
    UMat tmp_బఫరు(1, (పూర్ణాంకం)బఫరు_కొలత, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_వరుస వరుస_2 = (cl_command_వరుస)ocl::వరుస::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfft_వరుసలో_చేర్చు_Transform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &వరుస_2, 0, ఖాళీ, &e,
                                          &srcarg, &dstarg, (cl_mem)tmp_బఫరు.handle(ACCESS_RW)))

    tmp_బఫరు.addref();
    cl_సమితి_Event_Callback(e, CL_COMPLETE, oclCleanupCallback, tmp_బఫరు.u);
    తిరిగి నిజం;
}

#నిర్వచనం_రద్దు DFT_ASSERT

}

#అయితే_ముగింపు // HAVE_CLAMDFFT

నామాకాశ cv
{

మూస <రకం_పేరు T>
స్థిర శూన్య complementComplex(T * ptr, size_t అడుగు, పూర్ణాంకం n, పూర్ణాంకం len, పూర్ణాంకం dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = అడుగు/పరిమాణం(p0[0]);
    కొరకు(పూర్ణాంకం i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        కొరకు( పూర్ణాంకం j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

స్థిర శూన్య complementComplexOutput(పూర్ణాంకం లోతు, uchar * ptr, size_t అడుగు, పూర్ణాంకం లెక్క_2, పూర్ణాంకం len, పూర్ణాంకం dft_dims)
{
    ఒకవేళ( లోతు == CV_32F )
        complementComplex((దశాంశం*)ptr, అడుగు, లెక్క_2, len, dft_dims);
    లేకపోతే
        complementComplex((ద్విగుణ*)ptr, అడుగు, లెక్క_2, len, dft_dims);
}

గణన Dft_బహుళకం {
    InvalidDft = 0,
    Fwd_Real_గమ్యం_CCS,
    Fwd_Real_గమ్యం_Complex,
    Fwd_Complex_గమ్యం_Complex,
    InvCCSToReal,
    Inv_Complex_గమ్యం_Real,
    Inv_Complex_గమ్యం_Complex,
};

గణన DftDims {
    InvalidDim = 0,
    OneDim,
    One_Dim_నిలువు_Wise,
    TwoDims
};

అంతఃవరుస స్థిరరాశి అక్షరం * బహుళకం_Name(Dft_బహుళకం m)
{
    ఎంపిక (m)
    {
    స్థితి InvalidDft: తిరిగి "InvalidDft";
    స్థితి Fwd_Real_గమ్యం_CCS: తిరిగి "FwdRealToCCS";
    స్థితి Fwd_Real_గమ్యం_Complex: తిరిగి "FwdRealToComplex";
    స్థితి Fwd_Complex_గమ్యం_Complex: తిరిగి "FwdComplexToComplex";
    స్థితి InvCCSToReal: తిరిగి "InvCCSToReal";
    స్థితి Inv_Complex_గమ్యం_Real: తిరిగి "InvComplexToReal";
    స్థితి Inv_Complex_గమ్యం_Complex: తిరిగి "InvComplexToComplex";
    }
    తిరిగి 0;
}

అంతఃవరుస స్థిరరాశి అక్షరం * dimsName(DftDims d)
{
    ఎంపిక (d)
    {
    స్థితి InvalidDim: తిరిగి "InvalidDim";
    స్థితి OneDim: తిరిగి "OneDim";
    స్థితి One_Dim_నిలువు_Wise: తిరిగి "OneDimColWise";
    స్థితి TwoDims: తిరిగి "TwoDims";
    };
    తిరిగి 0;
}

మూస <రకం_పేరు T>
అంతఃవరుస తార్కిక isInv(T బహుళకం)
{
    ఎంపిక ((Dft_బహుళకం)బహుళకం)
    {
        స్థితి InvCCSToReal:
        స్థితి Inv_Complex_గమ్యం_Real:
        స్థితి Inv_Complex_గమ్యం_Complex: తిరిగి నిజం;
        అప్రమేయ: తిరిగి అబద్ధం;
    }
}

అంతఃవరుస Dft_బహుళకం determine_బహుళకం(తార్కిక inv, పూర్ణాంకం cn1, పూర్ణాంకం cn2)
{
    ఒకవేళ (!inv)
    {
        ఒకవేళ (cn1 == 1 && cn2 == 1)
            తిరిగి Fwd_Real_గమ్యం_CCS;
        లేకపోతే ఒకవేళ (cn1 == 1 && cn2 == 2)
            తిరిగి Fwd_Real_గమ్యం_Complex;
        లేకపోతే ఒకవేళ (cn1 == 2 && cn2 == 2)
            తిరిగి Fwd_Complex_గమ్యం_Complex;
    }
    లేకపోతే
    {
        ఒకవేళ (cn1 == 1 && cn2 == 1)
            తిరిగి InvCCSToReal;
        లేకపోతే ఒకవేళ (cn1 == 2 && cn2 == 1)
            తిరిగి Inv_Complex_గమ్యం_Real;
        లేకపోతే ఒకవేళ (cn1 == 2 && cn2 == 2)
            తిరిగి Inv_Complex_గమ్యం_Complex;
    }
    తిరిగి InvalidDft;
}


అంతఃవరుస DftDims determineDims(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, తార్కిక is_అడ్డువరుస_Wise, తార్కిక isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    ఒకవేళ (is_అడ్డువరుస_Wise)
        తిరిగి OneDim;
    ఒకవేళ (నిలువుల_సంఖ్య == 1 && అడ్డువరుసలు > 1) // one-column-shaped input
    {
        ఒకవేళ (isContinuous)
            తిరిగి OneDim;
        లేకపోతే
            తిరిగి One_Dim_నిలువు_Wise;
    }
    ఒకవేళ (అడ్డువరుసలు == 1)
        తిరిగి OneDim;
    ఒకవేళ (నిలువుల_సంఖ్య > 1 && అడ్డువరుసలు > 1)
        తిరిగి TwoDims;
    తిరిగి InvalidDim;
}

తరగతి OcvDftBasicImpl CV_FINAL : బహిరంగ hal::DFT1D
{
బహిరంగ:
    OcvDftOptions opt;
    // the plan is built once in init(), the workspace is allocated once here and reused by
    // every apply() (zero allocations per transform)
    DftPlan plan;
    Auto_బఫరు<uchar> ws;
#నిర్వచితమైతే USE_IPP_DFT
    Auto_బఫరు<uchar> ippbuf;
    Auto_బఫరు<uchar> ippworkbuf;
    size_t ipp_worksize;
#అయితే_ముగింపు

బహిరంగ:
    OcvDftBasicImpl()
    {
#నిర్వచితమైతే USE_IPP_DFT
        ipp_worksize = 0;
#అయితే_ముగింపు
    }
    శూన్య init(పూర్ణాంకం len, పూర్ణాంకం లెక్క_2, పూర్ణాంకం లోతు, పూర్ణాంకం flags, తార్కిక *need_బఫరు)
    {
        పూర్ణాంకం stage = (flags & CV_HAL_DFT_STAGE_నిలువుల_సంఖ్య) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        తార్కిక real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        తార్కిక needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = అబద్ధం;
    #నిర్వచితమైతే USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        ఒకవేళ( CV_IPP_CHECK_COND && (opt.n*లెక్క_2 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            పూర్ణాంకం ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            పూర్ణాంకం specsize=0, initsize=0, worksize=0;
            Ipp_DFTGet_కొలత_Func get_కొలత_Func = 0;
            IppDFTInitFunc initFunc = 0;

            ఒకవేళ( real_transform && stage == 0 )
            {
                ఒకవేళ( లోతు == CV_32F )
                {
                    get_కొలత_Func = ipps_DFTGet_కొలత_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                లేకపోతే
                {
                    get_కొలత_Func = ipps_DFTGet_కొలత_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            లేకపోతే
            {
                ఒకవేళ( లోతు == CV_32F )
                {
                    get_కొలత_Func = ipps_DFTGet_కొలత_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                లేకపోతే
                {
                    get_కొలత_Func = ipps_DFTGet_కొలత_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            ఒకవేళ( get_కొలత_Func(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                ఒకవేళ( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = నిజం;
            }
            లేకపోతే
                సమితి_Ipp_Error_స్థితి();
        }
    #అయితే_ముగింపు

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            స్థిరరాశి DFTKernels& kernels = getDFTKernels(లోతు);
            పూర్ణాంకం రకం = stage == 0 && real_transform ? (opt.isInverse ? DFT_రకం_C2_R : DFT_రకం_R2_C)
                                                    : DFT_రకం_C2_C;
            plan.build(రకం, len, లోతు, kernels.vlanes);
            ws.allocate(plan.ws_బైట్లు);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.దత్తాంశం();
            ఒకవేళ (need_బఫరు)
                *need_బఫరు = అబద్ధం;   // the engine supports in-place for every kind
        }

        {
            స్థిర DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<దశాంశం>>,
                dftWrap<దశాంశం, RealDFT<దశాంశం>>,
                dftWrap<దశాంశం, CCSIDFT<దశాంశం>>,
                dftWrap<Complexd, DFT<ద్విగుణ>>,
                dftWrap<ద్విగుణ, RealDFT<ద్విగుణ>>,
                dftWrap<ద్విగుణ, CCSIDFT<ద్విగుణ>>
            };
            పూర్ణాంకం idx = 0;
            ఒకవేళ (stage == 0)
            {
                ఒకవేళ (real_transform)
                {
                    ఒకవేళ (!opt.isInverse)
                        idx = 1;
                    లేకపోతే
                        idx = 2;
                }
            }
            ఒకవేళ (లోతు == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        ఒకవేళ(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            పూర్ణాంకం అడ్డువరుస_లెక్క = లెక్క_2;
            ఒకవేళ (stage == 0 && (flags & CV_HAL_DFT_అడ్డువరుసలు) != 0)
                అడ్డువరుస_లెక్క = 1;
            opt.scale = 1./(len * అడ్డువరుస_లెక్క);
        }
    }

    శూన్య apply(స్థిరరాశి uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspace_కొలత() స్థిరరాశి
    {
        size_t sz = opt.plan ? plan.ws_బైట్లు : 0;
#నిర్వచితమైతే USE_IPP_DFT
        ఒకవేళ (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#అయితే_ముగింపు
        తిరిగి sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    శూన్య applyWithWorkspace(స్థిరరాశి uchar *src, uchar *dst, uchar* workspace) స్థిరరాశి
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#నిర్వచితమైతే USE_IPP_DFT
        ఒకవేళ (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#అయితే_ముగింపు
        opt_.dft_func(opt_, src, dst);
    }

    శూన్య విడుదల() {}
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
స్థిర స్థిరరాశి పూర్ణాంకం DFT_నిలువుల_సంఖ్య_BATCH_MIN = 4;
స్థిర స్థిరరాశి పూర్ణాంకం DFT_నిలువుల_సంఖ్య_BATCH_MAX = 16;
స్థిర స్థిరరాశి size_t DFT_నిలువుల_సంఖ్య_BATCH_బైట్లు = 1 << 17;

స్థిర పూర్ణాంకం dft_నిలువుల_సంఖ్య_Batch(పూర్ణాంకం len, size_t complex_elem_కొలత)
{
    పూర్ణాంకం batch = (పూర్ణాంకం)(DFT_నిలువుల_సంఖ్య_BATCH_బైట్లు/((size_t)len*complex_elem_కొలత));
    తిరిగి std::max(DFT_నిలువుల_సంఖ్య_BATCH_MIN, std::min(DFT_నిలువుల_సంఖ్య_BATCH_MAX, batch));
}

స్థిర ద్విగుణ dftParallelStripes(పూర్ణాంకం లెక్క_2, పూర్ణాంకం len)
{
    స్థిరరాశి ద్విగుణ కాలాంశం = 0.5e6;
    తిరిగి (ద్విగుణ)లెక్క_2*len*std::max(std::సంవర్గమానం((ద్విగుణ)len)/std::సంవర్గమానం(2.), 1.)/కాలాంశం;
}

తరగతి OcvDftImpl CV_FINAL : బహిరంగ hal::DFT2D
{
రక్షిత:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    స్థిరరాశి OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    స్థిరరాశి OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    తార్కిక need_బఫరు_A;
    తార్కిక need_బఫరు_B;
    తార్కిక inv;
    పూర్ణాంకం వెడల్పు;
    పూర్ణాంకం ఎత్తు;
    Dft_బహుళకం బహుళకం;
    పూర్ణాంకం elem_కొలత_2;
    పూర్ణాంకం complex_elem_కొలత;
    పూర్ణాంకం లోతు;
    తార్కిక real_transform;
    పూర్ణాంకం nonzero_అడ్డువరుసలు;
    తార్కిక is_అడ్డువరుస_Transform;
    తార్కిక isScaled;
    std::vector<పూర్ణాంకం> stages;
    తార్కిక useIpp;
    పూర్ణాంకం src_channels;
    పూర్ణాంకం dst_channels;

    Auto_బఫరు<uchar> tmp_bufA;
    Auto_బఫరు<uchar> tmp_bufB;
    Auto_బఫరు<uchar> buf0;
    Auto_బఫరు<uchar> buf1;

బహిరంగ:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        need_బఫరు_A = అబద్ధం;
        need_బఫరు_B = అబద్ధం;
        inv = అబద్ధం;
        వెడల్పు = 0;
        ఎత్తు = 0;
        బహుళకం = InvalidDft;
        elem_కొలత_2 = 0;
        complex_elem_కొలత = 0;
        లోతు = 0;
        real_transform = అబద్ధం;
        nonzero_అడ్డువరుసలు = 0;
        is_అడ్డువరుస_Transform = అబద్ధం;
        isScaled = అబద్ధం;
        useIpp = అబద్ధం;
        src_channels = 0;
        dst_channels = 0;
    }

    శూన్య init(పూర్ణాంకం వెడల్పు_3, పూర్ణాంకం ఎత్తు_3, పూర్ణాంకం లోతు_2, పూర్ణాంకం _src_channels, పూర్ణాంకం _dst_channels, పూర్ణాంకం flags, పూర్ణాంకం nonzero_అడ్డువరుసలు_2)
    {
        తార్కిక isComplex = _src_channels != _dst_channels;
        nonzero_అడ్డువరుసలు = nonzero_అడ్డువరుసలు_2;
        వెడల్పు = వెడల్పు_3;
        ఎత్తు = ఎత్తు_3;
        లోతు = లోతు_2;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        తార్కిక isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        తార్కిక isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        తార్కిక isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        బహుళకం = determine_బహుళకం(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        is_అడ్డువరుస_Transform = (flags & CV_HAL_DFT_అడ్డువరుసలు) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        need_బఫరు_A = అబద్ధం;
        need_బఫరు_B = అబద్ధం;
        real_transform = (బహుళకం != Fwd_Complex_గమ్యం_Complex && బహుళకం != Inv_Complex_గమ్యం_Complex);

        elem_కొలత_2 = (లోతు == CV_32F) ? పరిమాణం(దశాంశం) : పరిమాణం(ద్విగుణ);
        complex_elem_కొలత = elem_కొలత_2 * 2;
        ఒకవేళ( !real_transform )
            elem_కొలత_2 = complex_elem_కొలత;

#ఒకవేళ defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            ఒకవేళ (nonzero_అడ్డువరుసలు == 0 && లోతు == CV_32F && ((వెడల్పు * ఎత్తు)>(పూర్ణాంకం)(1<<6)))
            {
                ఒకవేళ (బహుళకం == Fwd_Complex_గమ్యం_Complex || బహుళకం == Inv_Complex_గమ్యం_Complex || బహుళకం == Fwd_Real_గమ్యం_CCS || బహుళకం == InvCCSToReal)
                {
                    useIpp = నిజం;
                    తిరిగి;
                }
            }
        }
#అయితే_ముగింపు

        DftDims dims = determineDims(ఎత్తు, వెడల్పు, is_అడ్డువరుస_Transform, isContinuous);
        ఒకవేళ (dims == TwoDims)
        {
            stages.resize(2);
            ఒకవేళ (బహుళకం == InvCCSToReal || బహుళకం == Inv_Complex_గమ్యం_Real)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            లేకపోతే
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        లేకపోతే
        {
            stages.resize(1);
            ఒకవేళ (dims == One_Dim_నిలువు_Wise)
                stages[0] = 1;
            లేకపోతే
                stages[0] = 0;
        }

        కొరకు(uint stage_సూచీ = 0; stage_సూచీ < stages.size(); ++stage_సూచీ)
        {
            ఒకవేళ (stage_సూచీ == 1)
            {
                isInplace = నిజం;
                isComplex = అబద్ధం;
            }

            పూర్ణాంకం stage = stages[stage_సూచీ];
            తార్కిక is_చివరి_Stage = (stage_సూచీ + 1 == stages.size());

            పూర్ణాంకం len, లెక్క_2;

            పూర్ణాంకం f = 0;
            ఒకవేళ (inv)
                f |= CV_HAL_DFT_INVERSE;
            ఒకవేళ (isScaled)
                f |= CV_HAL_DFT_SCALE;
            ఒకవేళ (is_అడ్డువరుస_Transform)
                f |= CV_HAL_DFT_అడ్డువరుసలు;
            ఒకవేళ (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            ఒకవేళ (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            ఒకవేళ (!is_చివరి_Stage)
                f |= CV_HAL_DFT_TWO_STAGE;

            ఒకవేళ( stage == 0 ) // row-wise transform
            {
                ఒకవేళ (వెడల్పు == 1 && !is_అడ్డువరుస_Transform )
                {
                    len = ఎత్తు;
                    లెక్క_2 = వెడల్పు;
                }
                లేకపోతే
                {
                    len = వెడల్పు;
                    లెక్క_2 = ఎత్తు;
                }
                need_బఫరు_A = isInplace;
                contextA = hal::DFT1D::create(len, లెక్క_2, లోతు, f, &need_బఫరు_A);
                basicA = dynamic_cast<స్థిరరాశి OcvDftBasicImpl*>(contextA.get());
                ఒకవేళ (need_బఫరు_A)
                    tmp_bufA.allocate(len * complex_elem_కొలత);
            }
            లేకపోతే
            {
                len = ఎత్తు;
                లెక్క_2 = వెడల్పు;
                f |= CV_HAL_DFT_STAGE_నిలువుల_సంఖ్య;
                need_బఫరు_B = isInplace;
                contextB = hal::DFT1D::create(len, లెక్క_2, లోతు, f, &need_బఫరు_B);
                basicB = dynamic_cast<స్థిరరాశి OcvDftBasicImpl*>(contextB.get());
                ఒకవేళ (need_బఫరు_B)
                    tmp_bufB.allocate(len * complex_elem_కొలత);

                buf0.allocate(len * complex_elem_కొలత);
                buf1.allocate(len * complex_elem_కొలత);
            }
        }
    }

    శూన్య apply(స్థిరరాశి uchar * src, size_t src_అడుగు, uchar * dst, size_t dst_అడుగు) CV_OVERRIDE
    {
#ఒకవేళ defined USE_IPP_DFT
        ఒకవేళ (useIpp)
        {
            పూర్ణాంకం ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            ఒకవేళ (!is_అడ్డువరుస_Transform)
            {
                ఒకవేళ (బహుళకం == Fwd_Complex_గమ్యం_Complex || బహుళకం == Inv_Complex_గమ్యం_Complex)
                {
                    ఒకవేళ (ippi_DFT_C_32F(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, ఎత్తు, inv, ipp_norm_flag))
                    {
                        CV_IMPL_కూడు(CV_IMPL_IPP);
                        తిరిగి;
                    }
                    సమితి_Ipp_Error_స్థితి();
                }
                లేకపోతే ఒకవేళ (బహుళకం == Fwd_Real_గమ్యం_CCS || బహుళకం == InvCCSToReal)
                {
                    ఒకవేళ (ippi_DFT_R_32F(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, ఎత్తు, inv, ipp_norm_flag))
                    {
                        CV_IMPL_కూడు(CV_IMPL_IPP);
                        తిరిగి;
                    }
                    సమితి_Ipp_Error_స్థితి();
                }
            }
            లేకపోతే
            {
                ఒకవేళ (బహుళకం == Fwd_Complex_గమ్యం_Complex || బహుళకం == Inv_Complex_గమ్యం_Complex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    ఒకవేళ (Dft_C_IPPLoop(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, ఎత్తు, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_కూడు(CV_IMPL_IPP|CV_IMPL_MT);
                        తిరిగి;
                    }
                    సమితి_Ipp_Error_స్థితి();
                }
                లేకపోతే ఒకవేళ (బహుళకం == Fwd_Real_గమ్యం_CCS || బహుళకం == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippi_DFTInv_Pack_గమ్యం_R_32f_C1_R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    ఒకవేళ (Dft_R_IPPLoop(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, ఎత్తు, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_కూడు(CV_IMPL_IPP|CV_IMPL_MT);
                        తిరిగి;
                    }
                    సమితి_Ipp_Error_స్థితి();
                }
            }
            తిరిగి;
        }
#అయితే_ముగింపు

        కొరకు(uint stage_సూచీ = 0; stage_సూచీ < stages.size(); ++stage_సూచీ)
        {
            పూర్ణాంకం stage_src_channels = src_channels;
            పూర్ణాంకం stage_dst_channels = dst_channels;

            ఒకవేళ (stage_సూచీ == 1)
            {
                src = dst;
                src_అడుగు = dst_అడుగు;
                stage_src_channels = stage_dst_channels;
            }

            పూర్ణాంకం stage = stages[stage_సూచీ];
            తార్కిక is_చివరి_Stage = (stage_సూచీ + 1 == stages.size());
            తార్కిక isComplex = stage_src_channels != stage_dst_channels;

            ఒకవేళ( stage == 0 )
                అడ్డువరుస_Dft(src, src_అడుగు, dst, dst_అడుగు, isComplex, is_చివరి_Stage);
            లేకపోతే
                నిలువు_Dft(src, src_అడుగు, dst, dst_అడుగు, stage_src_channels, stage_dst_channels, is_చివరి_Stage);
        }
    }

రక్షిత:

    శూన్య అడ్డువరుస_Dft(స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, uchar* dst_దత్తాంశం, size_t dst_అడుగు, తార్కిక isComplex, తార్కిక is_చివరి_Stage)
    {
        పూర్ణాంకం len, లెక్క_2;
        ఒకవేళ (వెడల్పు == 1 && !is_అడ్డువరుస_Transform )
        {
            len = ఎత్తు;
            లెక్క_2 = వెడల్పు;
        }
        లేకపోతే
        {
            len = వెడల్పు;
            లెక్క_2 = ఎత్తు;
        }
        పూర్ణాంకం dptr_స్థానభ్రంశం = 0;
        పూర్ణాంకం dst_నిండిన_len = len*elem_కొలత_2;

        ఒకవేళ( need_బఫరు_A )
        {
            ఒకవేళ (బహుళకం == Fwd_Real_గమ్యం_CCS && (len & 1) && len > 1)
                dptr_స్థానభ్రంశం = elem_కొలత_2;
        }

        ఒకవేళ( !inv && isComplex )
            dst_నిండిన_len += (len & 1) ? elem_కొలత_2 : complex_elem_కొలత;

        పూర్ణాంకం nz = nonzero_అడ్డువరుసలు;
        ఒకవేళ( nz <= 0 || nz > లెక్క_2 )
            nz = లెక్క_2;

        size_t ws_కొలత = basicA ? basicA->workspace_కొలత() : 0;
        ద్విగుణ nstripes = dftParallelStripes(nz, len);
        ఒకవేళ( ws_కొలత > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            స్థిరరాశి OcvDftBasicImpl* ctx = basicA;
            స్వయంచాలక process_అడ్డువరుసలు = [&, ctx](స్థిరరాశి Range& range)
            {
                Auto_బఫరు<uchar> ws(ws_కొలత);
                Auto_బఫరు<uchar> rowbuf(need_బఫరు_A ? len * complex_elem_కొలత : 0);
                కొరకు( పూర్ణాంకం i = range.ప్రారంభం; i < range.end; i++ )
                {
                    స్థిరరాశి uchar* sptr = src_దత్తాంశం + src_అడుగు * i;
                    uchar* dptr0 = dst_దత్తాంశం + dst_అడుగు * i;
                    uchar* dptr = need_బఫరు_A ? rowbuf.దత్తాంశం() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.దత్తాంశం());
                    ఒకవేళ( need_బఫరు_A )
                        జ్ఞాపకం_నకలు( dptr0, dptr + dptr_స్థానభ్రంశం, dst_నిండిన_len );
                }
            };
            parallel_for_(Range(0, nz), process_అడ్డువరుసలు, nstripes);
        }
        లేకపోతే
        {
            కొరకు( పూర్ణాంకం i = 0; i < nz; i++ )
            {
                స్థిరరాశి uchar* sptr = src_దత్తాంశం + src_అడుగు * i;
                uchar* dptr0 = dst_దత్తాంశం + dst_అడుగు * i;
                uchar* dptr = dptr0;

                ఒకవేళ( need_బఫరు_A )
                    dptr = tmp_bufA.దత్తాంశం();

                contextA->apply(sptr, dptr);

                ఒకవేళ( need_బఫరు_A )
                    జ్ఞాపకం_నకలు( dptr0, dptr + dptr_స్థానభ్రంశం, dst_నిండిన_len );
            }
        }

        కొరకు( పూర్ణాంకం i = nz; i < లెక్క_2; i++ )
        {
            uchar* dptr0 = dst_దత్తాంశం + dst_అడుగు * i;
            memset( dptr0, 0, dst_నిండిన_len );
        }
        ఒకవేళ(is_చివరి_Stage &&  బహుళకం == Fwd_Real_గమ్యం_Complex)
            complementComplexOutput(లోతు, dst_దత్తాంశం, dst_అడుగు, len, nz, 1);
    }

    శూన్య నిలువు_Dft(స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, uchar* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం stage_src_channels, పూర్ణాంకం stage_dst_channels, తార్కిక is_చివరి_Stage)
    {
        పూర్ణాంకం len = ఎత్తు;
        పూర్ణాంకం లెక్క_2 = వెడల్పు;
        పూర్ణాంకం a = 0, b = లెక్క_2;
        uchar *dbuf0, *dbuf1;
        స్థిరరాశి uchar* sptr0 = src_దత్తాంశం;
        uchar* dptr0 = dst_దత్తాంశం;

        dbuf0 = buf0.దత్తాంశం(), dbuf1 = buf1.దత్తాంశం();

        ఒకవేళ( need_బఫరు_B )
        {
            dbuf1 = tmp_bufB.దత్తాంశం();
            dbuf0 = buf1.దత్తాంశం();
        }

        ఒకవేళ( real_transform )
        {
            పూర్ణాంకం even;
            a = 1;
            even = (లెక్క_2 & 1) == 0;
            b = (లెక్క_2+1)/2;
            ఒకవేళ( !inv )
            {
                memset( buf0.దత్తాంశం(), 0, len*complex_elem_కొలత );
                Copy_స్తంభం( sptr0, src_అడుగు, buf0.దత్తాంశం(), complex_elem_కొలత, len, elem_కొలత_2 );
                sptr0 += stage_dst_channels*elem_కొలత_2;
                ఒకవేళ( even )
                {
                    memset( buf1.దత్తాంశం(), 0, len*complex_elem_కొలత );
                    Copy_స్తంభం( sptr0 + (లెక్క_2-2)*elem_కొలత_2, src_అడుగు,
                                buf1.దత్తాంశం(), complex_elem_కొలత, len, elem_కొలత_2 );
                }
            }
            లేకపోతే ఒకవేళ( stage_src_channels == 1 )
            {
                Copy_స్తంభం( sptr0, src_అడుగు, buf0.దత్తాంశం(), elem_కొలత_2, len, elem_కొలత_2 );
                ExpandCCS( buf0.దత్తాంశం(), len, elem_కొలత_2 );
                ఒకవేళ( even )
                {
                    Copy_స్తంభం( sptr0 + (లెక్క_2-1)*elem_కొలత_2, src_అడుగు,
                                buf1.దత్తాంశం(), elem_కొలత_2, len, elem_కొలత_2 );
                    ExpandCCS( buf1.దత్తాంశం(), len, elem_కొలత_2 );
                }
                sptr0 += elem_కొలత_2;
            }
            లేకపోతే
            {
                Copy_స్తంభం( sptr0, src_అడుగు, buf0.దత్తాంశం(), complex_elem_కొలత, len, complex_elem_కొలత );
                ఒకవేళ( even )
                {
                    Copy_స్తంభం( sptr0 + b*complex_elem_కొలత, src_అడుగు,
                                   buf1.దత్తాంశం(), complex_elem_కొలత, len, complex_elem_కొలత );
                }
                sptr0 += complex_elem_కొలత;
            }

            ఒకవేళ( even )
                contextB->apply(buf1.దత్తాంశం(), dbuf1);
            contextB->apply(buf0.దత్తాంశం(), dbuf0);

            ఒకవేళ( stage_dst_channels == 1 )
            {
                ఒకవేళ( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    జ్ఞాపకం_నకలు( dbuf0 + elem_కొలత_2, dbuf0, elem_కొలత_2 );
                    Copy_స్తంభం( dbuf0 + elem_కొలత_2, elem_కొలత_2, dptr0,
                                   dst_అడుగు, len, elem_కొలత_2 );
                    ఒకవేళ( even )
                    {
                        జ్ఞాపకం_నకలు( dbuf1 + elem_కొలత_2, dbuf1, elem_కొలత_2 );
                        Copy_స్తంభం( dbuf1 + elem_కొలత_2, elem_కొలత_2,
                                       dptr0 + (లెక్క_2-1)*elem_కొలత_2,
                                       dst_అడుగు, len, elem_కొలత_2 );
                    }
                    dptr0 += elem_కొలత_2;
                }
                లేకపోతే
                {
                    // copy the real part of the complex vector to the first/last column
                    Copy_స్తంభం( dbuf0, complex_elem_కొలత, dptr0, dst_అడుగు, len, elem_కొలత_2 );
                    ఒకవేళ( even )
                        Copy_స్తంభం( dbuf1, complex_elem_కొలత, dptr0 + (లెక్క_2-1)*elem_కొలత_2,
                                       dst_అడుగు, len, elem_కొలత_2 );
                    dptr0 += elem_కొలత_2;
                }
            }
            లేకపోతే
            {
                CV_Assert( !inv );
                Copy_స్తంభం( dbuf0, complex_elem_కొలత, dptr0,
                               dst_అడుగు, len, complex_elem_కొలత );
                ఒకవేళ( even )
                    Copy_స్తంభం( dbuf1, complex_elem_కొలత,
                                   dptr0 + b*complex_elem_కొలత,
                                   dst_అడుగు, len, complex_elem_కొలత );
                dptr0 += complex_elem_కొలత;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        పూర్ణాంకం ncols = b - a;
        స్థిరరాశి పూర్ణాంకం నిలువుల_సంఖ్య_batch = dft_నిలువుల_సంఖ్య_Batch(len, complex_elem_కొలత);
        పూర్ణాంకం nbatches = (ncols + నిలువుల_సంఖ్య_batch - 1)/నిలువుల_సంఖ్య_batch;
        size_t ws_కొలత = basicB ? basicB->workspace_కొలత() : 0;
        size_t colbuf_కొలత = (size_t)len*complex_elem_కొలత;
        స్థిరరాశి OcvDftBasicImpl* ctx = basicB;
        స్వయంచాలక processBatches = [&, ctx](స్థిరరాశి Range& range)
        {
            Auto_బఫరు<uchar> ws(ws_కొలత);
            Auto_బఫరు<uchar> cbuf(colbuf_కొలత*నిలువుల_సంఖ్య_batch*(need_బఫరు_B ? 2 : 1));
            uchar* cbuf0 = cbuf.దత్తాంశం();
            uchar* cdbuf0 = need_బఫరు_B ? cbuf0 + colbuf_కొలత*నిలువుల_సంఖ్య_batch : cbuf0;
            కొరకు( పూర్ణాంకం bi = range.ప్రారంభం; bi < range.end; bi++ )
            {
                పూర్ణాంకం c0 = bi*నిలువుల_సంఖ్య_batch;
                పూర్ణాంకం nc = std::min(నిలువుల_సంఖ్య_batch, ncols - c0);
                స్థిరరాశి uchar* sptr = sptr0 + (size_t)c0*complex_elem_కొలత;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_కొలత;
                CopyFromColumns( sptr, src_అడుగు, cbuf0, nc, len, complex_elem_కొలత );
                కొరకు( పూర్ణాంకం k = 0; k < nc; k++ )
                {
                    ఒకవేళ( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_కొలత, cdbuf0 + k*colbuf_కొలత, ws.దత్తాంశం());
                    లేకపోతే
                        contextB->apply(cbuf0 + k*colbuf_కొలత, cdbuf0 + k*colbuf_కొలత);
                }
                Copy_గమ్యం_Columns( cdbuf0, dptr, dst_అడుగు, nc, len, complex_elem_కొలత );
            }
        };
        ద్విగుణ nstripes = dftParallelStripes(ncols, len);
        ఒకవేళ( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        లేకపోతే
            processBatches(Range(0, nbatches));
        ఒకవేళ(is_చివరి_Stage && బహుళకం == Fwd_Real_గమ్యం_Complex)
            complementComplexOutput(లోతు, dst_దత్తాంశం, dst_అడుగు, లెక్క_2, len, 2);
    }
};

నిర్మాణం ReplacementDFT1D : బహిరంగ hal::DFT1D
{
    cvhalDFT *context;
    తార్కిక isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(అబద్ధం) {}
    తార్కిక init(పూర్ణాంకం len, పూర్ణాంకం లెక్క_2, పూర్ణాంకం లోతు, పూర్ణాంకం flags, తార్కిక *need_బఫరు)
    {
        పూర్ణాంకం res = cv_hal_dftInit1D(&context, len, లెక్క_2, లోతు, flags, need_బఫరు);
        isInitialized = (res == CV_HAL_ERROR_OK);
        తిరిగి isInitialized;
    }
    శూన్య apply(స్థిరరాశి uchar *src, uchar *dst) CV_OVERRIDE
    {
        ఒకవేళ (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        ఒకవేళ (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

నిర్మాణం ReplacementDFT2D : బహిరంగ hal::DFT2D
{
    cvhalDFT *context;
    తార్కిక isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(అబద్ధం) {}
    తార్కిక init(పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం లోతు,
              పూర్ణాంకం src_channels, పూర్ణాంకం dst_channels,
              పూర్ణాంకం flags, పూర్ణాంకం nonzero_అడ్డువరుసలు)
    {
        పూర్ణాంకం res = cv_hal_dftInit2D(&context, వెడల్పు, ఎత్తు, లోతు, src_channels, dst_channels, flags, nonzero_అడ్డువరుసలు);
        isInitialized = (res == CV_HAL_ERROR_OK);
        తిరిగి isInitialized;
    }
    శూన్య apply(స్థిరరాశి uchar *src, size_t src_అడుగు, uchar *dst, size_t dst_అడుగు) CV_OVERRIDE
    {
        ఒకవేళ (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_అడుగు, dst, dst_అడుగు);
        }
    }
    ~ReplacementDFT2D()
    {
        ఒకవేళ (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

నామాకాశ hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(పూర్ణాంకం len, పూర్ణాంకం లెక్క_2, పూర్ణాంకం లోతు, పూర్ణాంకం flags, తార్కిక *need_బఫరు)
{
    {
        ReplacementDFT1D *impl = కొత్త ReplacementDFT1D();
        ఒకవేళ (impl->init(len, లెక్క_2, లోతు, flags, need_బఫరు))
        {
            తిరిగి Ptr<DFT1D>(impl);
        }
        తొలగించు impl;
    }
    {
        OcvDftBasicImpl *impl = కొత్త OcvDftBasicImpl();
        impl->init(len, లెక్క_2, లోతు, flags, need_బఫరు);
        తిరిగి Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం లోతు,
                         పూర్ణాంకం src_channels, పూర్ణాంకం dst_channels,
                         పూర్ణాంకం flags, పూర్ణాంకం nonzero_అడ్డువరుసలు)
{
    {
        ReplacementDFT2D *impl = కొత్త ReplacementDFT2D();
        ఒకవేళ (impl->init(వెడల్పు, ఎత్తు, లోతు, src_channels, dst_channels, flags, nonzero_అడ్డువరుసలు))
        {
            తిరిగి Ptr<DFT2D>(impl);
        }
        తొలగించు impl;
    }
    {
        ఒకవేళ(వెడల్పు == 1 && nonzero_అడ్డువరుసలు > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = కొత్త OcvDftImpl();
        impl->init(వెడల్పు, ఎత్తు, లోతు, src_channels, dst_channels, flags, nonzero_అడ్డువరుసలు);
        తిరిగి Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


శూన్య cv::dft( ఆదానం_Array _src0, OutputArray _dst, పూర్ణాంకం flags, పూర్ణాంకం nonzero_అడ్డువరుసలు )
{
    CV_INSTRUMENT_REGION();

#నిర్వచితమైతే HAVE_CLAMDFFT
    CV_OCL_నిరంతరం(ocl::haveAmdFft() && ocl::Device::getDefault().type() != ocl::Device::TYPE_CPU &&
            _dst.isUMat() && _src0.dims() <= 2 && nonzero_అడ్డువరుసలు == 0,
               ocl_dft_amdfft(_src0, _dst, flags))
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENCL
    CV_OCL_నిరంతరం(_dst.isUMat() && _src0.dims() <= 2,
               ocl_dft(_src0, _dst, flags, nonzero_అడ్డువరుసలు))
#అయితే_ముగింపు

    Mat src0 = _src0.getMat(), src = src0;
    ఒకవేళ (src.empty())
    {
        _dst.release();
        తిరిగి;
    }
    తార్కిక inv = (flags & DFT_INVERSE) != 0;
    పూర్ణాంకం type = src.type();
    పూర్ణాంకం లోతు = src.లోతు();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_ఆదానం) && src.channels() != 2) );

    ఒకవేళ( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.create_Same_కొలత( src, CV_MAKETYPE(లోతు, 2) );
    లేకపోతే ఒకవేళ( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.create_Same_కొలత( src, లోతు );
    లేకపోతే
        _dst.create_Same_కొలత( src, type );

    Mat dst = _dst.getMat();

    పూర్ణాంకం f = 0;
    ఒకవేళ (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    ఒకవేళ (inv)
        f |= CV_HAL_DFT_INVERSE;
    ఒకవేళ (flags & DFT_అడ్డువరుసలు)
        f |= CV_HAL_DFT_అడ్డువరుసలు;
    ఒకవేళ (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    ఒకవేళ (src.దత్తాంశం == dst.దత్తాంశం)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, లోతు, src.channels(), dst.channels(), f, nonzero_అడ్డువరుసలు);
    c->apply(src.దత్తాంశం, src.అడుగు, dst.దత్తాంశం, dst.అడుగు);
}


శూన్య cv::idft( ఆదానం_Array src, OutputArray dst, పూర్ణాంకం flags, పూర్ణాంకం nonzero_అడ్డువరుసలు )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_అడ్డువరుసలు );
}

#నిర్వచితమైతే HAVE_OPENCL

నామాకాశ cv {

స్థిర తార్కిక ocl_mulSpectrums( ఆదానం_Array _srcA, ఆదానం_Array _srcB,
                              OutputArray _dst, పూర్ణాంకం flags, తార్కిక conjB )
{
    పూర్ణాంకం atype = _srcA.type(), btype = _srcB.type(),
            అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    కొలత asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    ఒకవేళ ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        తిరిగి అబద్ధం;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.create_Same_కొలత(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(A), ocl::KernelArg::Read_Only_No_కొలత(B),
           ocl::KernelArg::WriteOnly(dst), అడ్డువరుసలు_చొప్పున_WI);

    size_t globalsize[2] = { (size_t)asize.వెడల్పు, ((size_t)asize.ఎత్తు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

}

#అయితే_ముగింపు

నామాకాశ {

#నిర్వచించు VAL(buf, elem) (((T*)((అక్షరం*)దత్తాంశం ## buf + (అడుగు ## buf * (elem))))[0])
#నిర్వచించు MUL_SPECTRUMS_నిలువు(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    కొరకు (size_t j = 1; j <= అడ్డువరుసలు - 2; j += 2) \
    { \
        ద్విగుణ a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        ద్విగుణ b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        ఒకవేళ (conjB) b_im = -b_im; \
        ద్విగుణ c_re = a_re * b_re - a_im * b_im; \
        ద్విగుణ c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    ఒకవేళ ((అడ్డువరుసలు & 1) == 0) \
        VAL(C, అడ్డువరుసలు-1) = VAL(A, అడ్డువరుసలు-1) * VAL(B, అడ్డువరుసలు-1)

మూస <రకం_పేరు T, తార్కిక conjB> స్థిర అంతఃవరుస
శూన్య mul_Spectrums_process_నిలువు_noinplace(స్థిరరాశి T* దత్తాంశం_A, స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_C, size_t అడుగు_A, size_t అడుగు_B, size_t అడుగు_C, size_t అడ్డువరుసలు)
{
    MUL_SPECTRUMS_నిలువు(A, B, C);
}

మూస <రకం_పేరు T, తార్కిక conjB> స్థిర అంతఃవరుస
శూన్య mul_Spectrums_process_నిలువు_inplace_A(స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_AC, size_t అడుగు_B, size_t అడుగు_AC, size_t అడ్డువరుసలు)
{
    MUL_SPECTRUMS_నిలువు(AC, B, AC);
}
మూస <రకం_పేరు T, తార్కిక conjB, తార్కిక inplaceA> స్థిర అంతఃవరుస
శూన్య mul_Spectrums_process_నిలువు(స్థిరరాశి T* దత్తాంశం_A, స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_C, size_t అడుగు_A, size_t అడుగు_B, size_t అడుగు_C, size_t అడ్డువరుసలు)
{
    ఒకవేళ (inplaceA)
        mul_Spectrums_process_నిలువు_inplace_A<T, conjB>(దత్తాంశం_B, దత్తాంశం_C, అడుగు_B, అడుగు_C, అడ్డువరుసలు);
    లేకపోతే
        mul_Spectrums_process_నిలువు_noinplace<T, conjB>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, అడుగు_A, అడుగు_B, అడుగు_C, అడ్డువరుసలు);
}
#నిర్వచనం_రద్దు MUL_SPECTRUMS_నిలువు
#నిర్వచనం_రద్దు VAL

మూస <రకం_పేరు T, తార్కిక conjB, తార్కిక inplaceA> స్థిర అంతఃవరుస
శూన్య mul_Spectrums_process_నిలువుల_సంఖ్య(స్థిరరాశి T* దత్తాంశం_A, స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_C, size_t అడుగు_A, size_t అడుగు_B, size_t అడుగు_C, size_t అడ్డువరుసలు, size_t నిలువుల_సంఖ్య)
{
    mul_Spectrums_process_నిలువు<T, conjB, inplaceA>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, అడుగు_A, అడుగు_B, అడుగు_C, అడ్డువరుసలు);
    ఒకవేళ ((నిలువుల_సంఖ్య & 1) == 0)
    {
        mul_Spectrums_process_నిలువు<T, conjB, inplaceA>(దత్తాంశం_A + నిలువుల_సంఖ్య - 1, దత్తాంశం_B + నిలువుల_సంఖ్య - 1, దత్తాంశం_C + నిలువుల_సంఖ్య - 1, అడుగు_A, అడుగు_B, అడుగు_C, అడ్డువరుసలు);
    }
}

#నిర్వచించు VAL(buf, elem) (దత్తాంశం ## buf[(elem)])
#నిర్వచించు MUL_SPECTRUMS_అడ్డువరుస(A, B, C) \
    కొరకు (size_t j = j0; j < j1; j += 2) \
    { \
        ద్విగుణ a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        ద్విగుణ b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        ఒకవేళ (conjB) b_im = -b_im; \
        ద్విగుణ c_re = a_re * b_re - a_im * b_im; \
        ద్విగుణ c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
మూస <రకం_పేరు T, తార్కిక conjB> స్థిర అంతఃవరుస
శూన్య mul_Spectrums_process_అడ్డువరుస_noinplace(స్థిరరాశి T* దత్తాంశం_A, స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_C, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_అడ్డువరుస(A, B, C);
}
మూస <రకం_పేరు T, తార్కిక conjB> స్థిర అంతఃవరుస
శూన్య mul_Spectrums_process_అడ్డువరుస_inplace_A(స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_AC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_అడ్డువరుస(AC, B, AC);
}
మూస <రకం_పేరు T, తార్కిక conjB, తార్కిక inplaceA> స్థిర అంతఃవరుస
శూన్య mul_Spectrums_process_అడ్డువరుస(స్థిరరాశి T* దత్తాంశం_A, స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_C, size_t j0, size_t j1)
{
    ఒకవేళ (inplaceA)
        mul_Spectrums_process_అడ్డువరుస_inplace_A<T, conjB>(దత్తాంశం_B, దత్తాంశం_C, j0, j1);
    లేకపోతే
        mul_Spectrums_process_అడ్డువరుస_noinplace<T, conjB>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, j0, j1);
}
#నిర్వచనం_రద్దు MUL_SPECTRUMS_అడ్డువరుస
#నిర్వచనం_రద్దు VAL

మూస <రకం_పేరు T, తార్కిక conjB, తార్కిక inplaceA> స్థిర అంతఃవరుస
శూన్య mul_Spectrums_process_అడ్డువరుసలు(స్థిరరాశి T* దత్తాంశం_A, స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_C, size_t అడుగు_A, size_t అడుగు_B, size_t అడుగు_C, size_t అడ్డువరుసలు, size_t నిలువుల_సంఖ్య, size_t j0, size_t j1, తార్కిక is_1d_CN1)
{
    అయితే (అడ్డువరుసలు-- > 0)
    {
        ఒకవేళ (is_1d_CN1)
            దత్తాంశం_C[0] = దత్తాంశం_A[0]*దత్తాంశం_B[0];
        mul_Spectrums_process_అడ్డువరుస<T, conjB, inplaceA>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, j0, j1);
        ఒకవేళ (is_1d_CN1 && (నిలువుల_సంఖ్య & 1) == 0)
            దత్తాంశం_C[j1] = దత్తాంశం_A[j1]*దత్తాంశం_B[j1];

        దత్తాంశం_A = (స్థిరరాశి T*)(((అక్షరం*)దత్తాంశం_A) + అడుగు_A);
        దత్తాంశం_B = (స్థిరరాశి T*)(((అక్షరం*)దత్తాంశం_B) + అడుగు_B);
        దత్తాంశం_C =       (T*)(((అక్షరం*)దత్తాంశం_C) + అడుగు_C);
    }
}


మూస <రకం_పేరు T, తార్కిక conjB, తార్కిక inplaceA> స్థిర అంతఃవరుస
శూన్య mulSpectrums_Impl_(స్థిరరాశి T* దత్తాంశం_A, స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_C, size_t అడుగు_A, size_t అడుగు_B, size_t అడుగు_C, size_t అడ్డువరుసలు, size_t నిలువుల_సంఖ్య, size_t j0, size_t j1, తార్కిక is_1d, తార్కిక isCN1)
{
    ఒకవేళ (!is_1d && isCN1)
    {
        mul_Spectrums_process_నిలువుల_సంఖ్య<T, conjB, inplaceA>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, అడుగు_A, అడుగు_B, అడుగు_C, అడ్డువరుసలు, నిలువుల_సంఖ్య);
    }
    mul_Spectrums_process_అడ్డువరుసలు<T, conjB, inplaceA>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, అడుగు_A, అడుగు_B, అడుగు_C, అడ్డువరుసలు, నిలువుల_సంఖ్య, j0, j1, is_1d && isCN1);
}
మూస <రకం_పేరు T, తార్కిక conjB> స్థిర అంతఃవరుస
శూన్య mulSpectrums_Impl(స్థిరరాశి T* దత్తాంశం_A, స్థిరరాశి T* దత్తాంశం_B, T* దత్తాంశం_C, size_t అడుగు_A, size_t అడుగు_B, size_t అడుగు_C, size_t అడ్డువరుసలు, size_t నిలువుల_సంఖ్య, size_t j0, size_t j1, తార్కిక is_1d, తార్కిక isCN1)
{
    ఒకవేళ (దత్తాంశం_A == దత్తాంశం_C)
        mulSpectrums_Impl_<T, conjB, నిజం>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, అడుగు_A, అడుగు_B, అడుగు_C, అడ్డువరుసలు, నిలువుల_సంఖ్య, j0, j1, is_1d, isCN1);
    లేకపోతే
        mulSpectrums_Impl_<T, conjB, అబద్ధం>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, అడుగు_A, అడుగు_B, అడుగు_C, అడ్డువరుసలు, నిలువుల_సంఖ్య, j0, j1, is_1d, isCN1);
}

} // namespace

శూన్య cv::mulSpectrums( ఆదానం_Array _srcA, ఆదానం_Array _srcB,
                       OutputArray _dst, పూర్ణాంకం flags, తార్కిక conjB )
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        తిరిగి;
    }

    పూర్ణాంకం type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_నిరంతరం(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    పూర్ణాంకం లోతు = srcA.లోతు(), cn = srcA.channels();
    size_t అడ్డువరుసలు = srcA.అడ్డువరుసలు, నిలువుల_సంఖ్య = srcA.నిలువుల_సంఖ్య;

    _dst.create( srcA.అడ్డువరుసలు, srcA.నిలువుల_సంఖ్య, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    ఒకవేళ (dst.దత్తాంశం == srcB.దత్తాంశం)
        srcB = srcB.clone(); // workaround for B only

    తార్కిక is_1d = (flags & DFT_అడ్డువరుసలు)
        || (అడ్డువరుసలు == 1)
        || (నిలువుల_సంఖ్య == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    ఒకవేళ( is_1d && !(flags & DFT_అడ్డువరుసలు) )
        నిలువుల_సంఖ్య = నిలువుల_సంఖ్య + అడ్డువరుసలు - 1, అడ్డువరుసలు = 1;

    తార్కిక isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = నిలువుల_సంఖ్య*cn - (((నిలువుల_సంఖ్య & 1) == 0 && cn == 1) ? 1 : 0);

    ఒకవేళ (లోతు == CV_32F)
    {
        స్థిరరాశి దశాంశం* దత్తాంశం_A = srcA.ptr<దశాంశం>();
        స్థిరరాశి దశాంశం* దత్తాంశం_B = srcB.ptr<దశాంశం>();
        దశాంశం* దత్తాంశం_C = dst.ptr<దశాంశం>();
        ఒకవేళ (!conjB)
            mulSpectrums_Impl<దశాంశం, అబద్ధం>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, srcA.అడుగు, srcB.అడుగు, dst.అడుగు, అడ్డువరుసలు, నిలువుల_సంఖ్య, j0, j1, is_1d, isCN1);
        లేకపోతే
            mulSpectrums_Impl<దశాంశం, నిజం>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, srcA.అడుగు, srcB.అడుగు, dst.అడుగు, అడ్డువరుసలు, నిలువుల_సంఖ్య, j0, j1, is_1d, isCN1);
    }
    లేకపోతే
    {
        స్థిరరాశి ద్విగుణ* దత్తాంశం_A = srcA.ptr<ద్విగుణ>();
        స్థిరరాశి ద్విగుణ* దత్తాంశం_B = srcB.ptr<ద్విగుణ>();
        ద్విగుణ* దత్తాంశం_C = dst.ptr<ద్విగుణ>();
        ఒకవేళ (!conjB)
            mulSpectrums_Impl<ద్విగుణ, అబద్ధం>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, srcA.అడుగు, srcB.అడుగు, dst.అడుగు, అడ్డువరుసలు, నిలువుల_సంఖ్య, j0, j1, is_1d, isCN1);
        లేకపోతే
            mulSpectrums_Impl<ద్విగుణ, నిజం>(దత్తాంశం_A, దత్తాంశం_B, దత్తాంశం_C, srcA.అడుగు, srcB.అడుగు, dst.అడుగు, అడ్డువరుసలు, నిలువుల_సంఖ్య, j0, j1, is_1d, isCN1);
    }
}

శూన్య cv::divSpectrums( ఆదానం_Array _srcA, ఆదానం_Array _srcB, OutputArray _dst, పూర్ణాంకం flags, తార్కిక conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    ఒకవేళ (srcA.empty() && srcB.empty())
    {
        _dst.release();
        తిరిగి;
    }
    పూర్ణాంకం లోతు = srcA.లోతు(), cn = srcA.channels(), type = srcA.type();
    పూర్ణాంకం అడ్డువరుసలు = srcA.అడ్డువరుసలు, నిలువుల_సంఖ్య = srcA.నిలువుల_సంఖ్య;
    పూర్ణాంకం j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.అడ్డువరుసలు, srcA.నిలువుల_సంఖ్య, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.దత్తాంశం != srcA.దత్తాంశం); // non-inplace check
    CV_Assert(dst.దత్తాంశం != srcB.దత్తాంశం); // non-inplace check

    తార్కిక is_1d = (flags & DFT_అడ్డువరుసలు) || (అడ్డువరుసలు == 1 || (నిలువుల_సంఖ్య == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    ఒకవేళ( is_1d && !(flags & DFT_అడ్డువరుసలు) )
        నిలువుల_సంఖ్య = నిలువుల_సంఖ్య + అడ్డువరుసలు - 1, అడ్డువరుసలు = 1;

    పూర్ణాంకం ncols = నిలువుల_సంఖ్య*cn;
    పూర్ణాంకం j0 = cn == 1;
    పూర్ణాంకం j1 = ncols - (నిలువుల_సంఖ్య % 2 == 0 && cn == 1);

    ఒకవేళ( లోతు == CV_32F )
    {
        స్థిరరాశి దశాంశం* దత్తాంశం_A = srcA.ptr<దశాంశం>();
        స్థిరరాశి దశాంశం* దత్తాంశం_B = srcB.ptr<దశాంశం>();
        దశాంశం* దత్తాంశం_C = dst.ptr<దశాంశం>();
        దశాంశం eps = FLT_EPSILON; // prevent div0 problems

        size_t అడుగు_A = srcA.అడుగు/పరిమాణం(దత్తాంశం_A[0]);
        size_t అడుగు_B = srcB.అడుగు/పరిమాణం(దత్తాంశం_B[0]);
        size_t అడుగు_C = dst.అడుగు/పరిమాణం(దత్తాంశం_C[0]);

        ఒకవేళ( !is_1d && cn == 1 )
        {
            కొరకు( k = 0; k < (నిలువుల_సంఖ్య % 2 ? 1 : 2); k++ )
            {
                ఒకవేళ( k == 1 )
                    దత్తాంశం_A += నిలువుల_సంఖ్య - 1, దత్తాంశం_B += నిలువుల_సంఖ్య - 1, దత్తాంశం_C += నిలువుల_సంఖ్య - 1;
                దత్తాంశం_C[0] = దత్తాంశం_A[0] / (దత్తాంశం_B[0] + eps);
                ఒకవేళ( అడ్డువరుసలు % 2 == 0 )
                    దత్తాంశం_C[(అడ్డువరుసలు-1)*అడుగు_C] = దత్తాంశం_A[(అడ్డువరుసలు-1)*అడుగు_A] / (దత్తాంశం_B[(అడ్డువరుసలు-1)*అడుగు_B] + eps);
                ఒకవేళ( !conjB )
                    కొరకు( j = 1; j <= అడ్డువరుసలు - 2; j += 2 )
                    {
                        ద్విగుణ denom = (ద్విగుణ)దత్తాంశం_B[j*అడుగు_B]*దత్తాంశం_B[j*అడుగు_B] +
                        (ద్విగుణ)దత్తాంశం_B[(j+1)*అడుగు_B]*దత్తాంశం_B[(j+1)*అడుగు_B] + (ద్విగుణ)eps;

                        ద్విగుణ re = (ద్విగుణ)దత్తాంశం_A[j*అడుగు_A]*దత్తాంశం_B[j*అడుగు_B] +
                        (ద్విగుణ)దత్తాంశం_A[(j+1)*అడుగు_A]*దత్తాంశం_B[(j+1)*అడుగు_B];

                        ద్విగుణ im = (ద్విగుణ)దత్తాంశం_A[(j+1)*అడుగు_A]*దత్తాంశం_B[j*అడుగు_B] -
                        (ద్విగుణ)దత్తాంశం_A[j*అడుగు_A]*దత్తాంశం_B[(j+1)*అడుగు_B];

                        దత్తాంశం_C[j*అడుగు_C] = (దశాంశం)(re / denom);
                        దత్తాంశం_C[(j+1)*అడుగు_C] = (దశాంశం)(im / denom);
                    }
                    లేకపోతే
                        కొరకు( j = 1; j <= అడ్డువరుసలు - 2; j += 2 )
                        {

                            ద్విగుణ denom = (ద్విగుణ)దత్తాంశం_B[j*అడుగు_B]*దత్తాంశం_B[j*అడుగు_B] +
                            (ద్విగుణ)దత్తాంశం_B[(j+1)*అడుగు_B]*దత్తాంశం_B[(j+1)*అడుగు_B] + (ద్విగుణ)eps;

                            ద్విగుణ re = (ద్విగుణ)దత్తాంశం_A[j*అడుగు_A]*దత్తాంశం_B[j*అడుగు_B] -
                            (ద్విగుణ)దత్తాంశం_A[(j+1)*అడుగు_A]*దత్తాంశం_B[(j+1)*అడుగు_B];

                            ద్విగుణ im = (ద్విగుణ)దత్తాంశం_A[(j+1)*అడుగు_A]*దత్తాంశం_B[j*అడుగు_B] +
                            (ద్విగుణ)దత్తాంశం_A[j*అడుగు_A]*దత్తాంశం_B[(j+1)*అడుగు_B];

                            దత్తాంశం_C[j*అడుగు_C] = (దశాంశం)(re / denom);
                            దత్తాంశం_C[(j+1)*అడుగు_C] = (దశాంశం)(im / denom);
                        }
                    ఒకవేళ( k == 1 )
                        దత్తాంశం_A -= నిలువుల_సంఖ్య - 1, దత్తాంశం_B -= నిలువుల_సంఖ్య - 1, దత్తాంశం_C -= నిలువుల_సంఖ్య - 1;
            }
        }

        కొరకు( ; అడ్డువరుసలు--; దత్తాంశం_A += అడుగు_A, దత్తాంశం_B += అడుగు_B, దత్తాంశం_C += అడుగు_C )
        {
            ఒకవేళ( is_1d && cn == 1 )
            {
                దత్తాంశం_C[0] = దత్తాంశం_A[0] / (దత్తాంశం_B[0] + eps);
                ఒకవేళ( నిలువుల_సంఖ్య % 2 == 0 )
                    దత్తాంశం_C[j1] = దత్తాంశం_A[j1] / (దత్తాంశం_B[j1] + eps);
            }

            ఒకవేళ( !conjB )
                కొరకు( j = j0; j < j1; j += 2 )
                {
                    ద్విగుణ denom = (ద్విగుణ)దత్తాంశం_B[j]*దత్తాంశం_B[j] + (ద్విగుణ)దత్తాంశం_B[j+1]*దత్తాంశం_B[j+1] + (ద్విగుణ)eps;
                    ద్విగుణ re = (ద్విగుణ)దత్తాంశం_A[j]*దత్తాంశం_B[j] + (ద్విగుణ)దత్తాంశం_A[j+1]*దత్తాంశం_B[j+1];
                    ద్విగుణ im = (ద్విగుణ)దత్తాంశం_A[j+1]*దత్తాంశం_B[j] - (ద్విగుణ)దత్తాంశం_A[j]*దత్తాంశం_B[j+1];
                    దత్తాంశం_C[j] = (దశాంశం)(re / denom);
                    దత్తాంశం_C[j+1] = (దశాంశం)(im / denom);
                }
                లేకపోతే
                    కొరకు( j = j0; j < j1; j += 2 )
                    {
                        ద్విగుణ denom = (ద్విగుణ)దత్తాంశం_B[j]*దత్తాంశం_B[j] + (ద్విగుణ)దత్తాంశం_B[j+1]*దత్తాంశం_B[j+1] + (ద్విగుణ)eps;
                        ద్విగుణ re = (ద్విగుణ)దత్తాంశం_A[j]*దత్తాంశం_B[j] - (ద్విగుణ)దత్తాంశం_A[j+1]*దత్తాంశం_B[j+1];
                        ద్విగుణ im = (ద్విగుణ)దత్తాంశం_A[j+1]*దత్తాంశం_B[j] + (ద్విగుణ)దత్తాంశం_A[j]*దత్తాంశం_B[j+1];
                        దత్తాంశం_C[j] = (దశాంశం)(re / denom);
                        దత్తాంశం_C[j+1] = (దశాంశం)(im / denom);
                    }
        }
    }
    లేకపోతే
    {
        స్థిరరాశి ద్విగుణ* దత్తాంశం_A = srcA.ptr<ద్విగుణ>();
        స్థిరరాశి ద్విగుణ* దత్తాంశం_B = srcB.ptr<ద్విగుణ>();
        ద్విగుణ* దత్తాంశం_C = dst.ptr<ద్విగుణ>();
        ద్విగుణ eps = DBL_EPSILON; // prevent div0 problems

        size_t అడుగు_A = srcA.అడుగు/పరిమాణం(దత్తాంశం_A[0]);
        size_t అడుగు_B = srcB.అడుగు/పరిమాణం(దత్తాంశం_B[0]);
        size_t అడుగు_C = dst.అడుగు/పరిమాణం(దత్తాంశం_C[0]);

        ఒకవేళ( !is_1d && cn == 1 )
        {
            కొరకు( k = 0; k < (నిలువుల_సంఖ్య % 2 ? 1 : 2); k++ )
            {
                ఒకవేళ( k == 1 )
                    దత్తాంశం_A += నిలువుల_సంఖ్య - 1, దత్తాంశం_B += నిలువుల_సంఖ్య - 1, దత్తాంశం_C += నిలువుల_సంఖ్య - 1;
                దత్తాంశం_C[0] = దత్తాంశం_A[0] / (దత్తాంశం_B[0] + eps);
                ఒకవేళ( అడ్డువరుసలు % 2 == 0 )
                    దత్తాంశం_C[(అడ్డువరుసలు-1)*అడుగు_C] = దత్తాంశం_A[(అడ్డువరుసలు-1)*అడుగు_A] / (దత్తాంశం_B[(అడ్డువరుసలు-1)*అడుగు_B] + eps);
                ఒకవేళ( !conjB )
                    కొరకు( j = 1; j <= అడ్డువరుసలు - 2; j += 2 )
                    {
                        ద్విగుణ denom = దత్తాంశం_B[j*అడుగు_B]*దత్తాంశం_B[j*అడుగు_B] +
                        దత్తాంశం_B[(j+1)*అడుగు_B]*దత్తాంశం_B[(j+1)*అడుగు_B] + eps;

                        ద్విగుణ re = దత్తాంశం_A[j*అడుగు_A]*దత్తాంశం_B[j*అడుగు_B] +
                        దత్తాంశం_A[(j+1)*అడుగు_A]*దత్తాంశం_B[(j+1)*అడుగు_B];

                        ద్విగుణ im = దత్తాంశం_A[(j+1)*అడుగు_A]*దత్తాంశం_B[j*అడుగు_B] -
                        దత్తాంశం_A[j*అడుగు_A]*దత్తాంశం_B[(j+1)*అడుగు_B];

                        దత్తాంశం_C[j*అడుగు_C] = re / denom;
                        దత్తాంశం_C[(j+1)*అడుగు_C] = im / denom;
                    }
                    లేకపోతే
                        కొరకు( j = 1; j <= అడ్డువరుసలు - 2; j += 2 )
                        {
                            ద్విగుణ denom = దత్తాంశం_B[j*అడుగు_B]*దత్తాంశం_B[j*అడుగు_B] +
                            దత్తాంశం_B[(j+1)*అడుగు_B]*దత్తాంశం_B[(j+1)*అడుగు_B] + eps;

                            ద్విగుణ re = దత్తాంశం_A[j*అడుగు_A]*దత్తాంశం_B[j*అడుగు_B] -
                            దత్తాంశం_A[(j+1)*అడుగు_A]*దత్తాంశం_B[(j+1)*అడుగు_B];

                            ద్విగుణ im = దత్తాంశం_A[(j+1)*అడుగు_A]*దత్తాంశం_B[j*అడుగు_B] +
                            దత్తాంశం_A[j*అడుగు_A]*దత్తాంశం_B[(j+1)*అడుగు_B];

                            దత్తాంశం_C[j*అడుగు_C] = re / denom;
                            దత్తాంశం_C[(j+1)*అడుగు_C] = im / denom;
                        }
                    ఒకవేళ( k == 1 )
                        దత్తాంశం_A -= నిలువుల_సంఖ్య - 1, దత్తాంశం_B -= నిలువుల_సంఖ్య - 1, దత్తాంశం_C -= నిలువుల_సంఖ్య - 1;
            }
        }

        కొరకు( ; అడ్డువరుసలు--; దత్తాంశం_A += అడుగు_A, దత్తాంశం_B += అడుగు_B, దత్తాంశం_C += అడుగు_C )
        {
            ఒకవేళ( is_1d && cn == 1 )
            {
                దత్తాంశం_C[0] = దత్తాంశం_A[0] / (దత్తాంశం_B[0] + eps);
                ఒకవేళ( నిలువుల_సంఖ్య % 2 == 0 )
                    దత్తాంశం_C[j1] = దత్తాంశం_A[j1] / (దత్తాంశం_B[j1] + eps);
            }

            ఒకవేళ( !conjB )
                కొరకు( j = j0; j < j1; j += 2 )
                {
                    ద్విగుణ denom = దత్తాంశం_B[j]*దత్తాంశం_B[j] + దత్తాంశం_B[j+1]*దత్తాంశం_B[j+1] + eps;
                    ద్విగుణ re = దత్తాంశం_A[j]*దత్తాంశం_B[j] + దత్తాంశం_A[j+1]*దత్తాంశం_B[j+1];
                    ద్విగుణ im = దత్తాంశం_A[j+1]*దత్తాంశం_B[j] - దత్తాంశం_A[j]*దత్తాంశం_B[j+1];
                    దత్తాంశం_C[j] = re / denom;
                    దత్తాంశం_C[j+1] = im / denom;
                }
                లేకపోతే
                    కొరకు( j = j0; j < j1; j += 2 )
                    {
                        ద్విగుణ denom = దత్తాంశం_B[j]*దత్తాంశం_B[j] + దత్తాంశం_B[j+1]*దత్తాంశం_B[j+1] + eps;
                        ద్విగుణ re = దత్తాంశం_A[j]*దత్తాంశం_B[j] - దత్తాంశం_A[j+1]*దత్తాంశం_B[j+1];
                        ద్విగుణ im = దత్తాంశం_A[j+1]*దత్తాంశం_B[j] + దత్తాంశం_A[j]*దత్తాంశం_B[j+1];
                        దత్తాంశం_C[j] = re / denom;
                        దత్తాంశం_C[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

నామాకాశ cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
మూస<రకం_పేరు T> స్థిర శూన్య
DCT( స్థిరరాశి OcvDftOptions & c, స్థిరరాశి T* src, size_t src_అడుగు, T* dst, size_t dst_అడుగు )
{
    ఒకవేళ( c.n == 1 )
    {
        dst[0] = src[0];
        తిరిగి;
    }
    నిరంతరం_Dft(c, src, src_అడుగు/పరిమాణం(T), dst, dst_అడుగు/పరిమాణం(T), అబద్ధం);
}

మూస<రకం_పేరు T> స్థిర శూన్య
IDCT( స్థిరరాశి OcvDftOptions & c, స్థిరరాశి T* src, size_t src_అడుగు, T* dst, size_t dst_అడుగు )
{
    ఒకవేళ( c.n == 1 )
    {
        dst[0] = src[0];
        తిరిగి;
    }
    నిరంతరం_Dft(c, src, src_అడుగు/పరిమాణం(T), dst, dst_అడుగు/పరిమాణం(T), అబద్ధం);
}

రకనిర్వచనం శూన్య (*DCTFunc)(స్థిరరాశి OcvDftOptions & c, స్థిరరాశి శూన్య* src, size_t src_అడుగు,
                        శూన్య* dst, size_t dst_అడుగు);

మూస<రకం_పేరు T, శూన్య (*fn)(స్థిరరాశి OcvDftOptions&, స్థిరరాశి T*, size_t, T*, size_t)>
స్థిర శూన్య dctWrap(స్థిరరాశి OcvDftOptions & c, స్థిరరాశి శూన్య* src, size_t src_అడుగు, శూన్య* dst, size_t dst_అడుగు)
{
    fn(c, (స్థిరరాశి T*)src, src_అడుగు, (T*)dst, dst_అడుగు);
}

}

#నిర్వచితమైతే HAVE_IPP
నామాకాశ cv
{

#ఒకవేళ IPP_సంస్కరణ_X100 >= 900
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * ippiDCTFunc)(స్థిరరాశి Ipp32f* pSrc, పూర్ణాంకం src_అడుగు_3, Ipp32f* pDst, పూర్ణాంకం dst_అడుగు_3, స్థిరరాశి శూన్య* pDCTSpec, Ipp8u* p_బఫరు);
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * ippiDCTInit)(శూన్య* pDCTSpec, Ippi_కొలత roi_కొలత, Ipp8u* pMemInit );
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * ippi_DCTGet_కొలత)(Ippi_కొలత roi_కొలత, పూర్ణాంకం* p_కొలత_Spec, పూర్ణాంకం* p_కొలత_Init, పూర్ణాంకం* p_కొలత_Buf);

మూస<రకం_పేరు SpecType, Ipp_స్థితి (CV_STDCALL *fn)(స్థిరరాశి Ipp32f*, పూర్ణాంకం, Ipp32f*, పూర్ణాంకం, స్థిరరాశి SpecType*, Ipp8u*)>
స్థిర Ipp_స్థితి CV_STDCALL ippiDCTWrap(స్థిరరాశి Ipp32f* pSrc, పూర్ణాంకం src_అడుగు_3, Ipp32f* pDst, పూర్ణాంకం dst_అడుగు_3, స్థిరరాశి శూన్య* pDCTSpec, Ipp8u* p_బఫరు)
{
    తిరిగి fn(pSrc, src_అడుగు_3, pDst, dst_అడుగు_3, (స్థిరరాశి SpecType*)pDCTSpec, p_బఫరు);
}

మూస<రకం_పేరు SpecType, Ipp_స్థితి (CV_STDCALL *fn)(SpecType*, Ippi_కొలత, Ipp8u*)>
స్థిర Ipp_స్థితి CV_STDCALL ippiDCTInitWrap(శూన్య* pDCTSpec, Ippi_కొలత roi_కొలత, Ipp8u* pMemInit)
{
    తిరిగి fn((SpecType*)pDCTSpec, roi_కొలత, pMemInit);
}

మూస<Ipp_స్థితి (CV_STDCALL *fn)(Ippi_కొలత, పూర్ణాంకం*, పూర్ణాంకం*, పూర్ణాంకం*)>
స్థిర Ipp_స్థితి CV_STDCALL ippi_DCTGet_కొలత_Wrap(Ippi_కొలత roi_కొలత, పూర్ణాంకం* p_కొలత_Spec, పూర్ణాంకం* p_కొలత_Init, పూర్ణాంకం* p_కొలత_Buf)
{
    తిరిగి fn(roi_కొలత, p_కొలత_Spec, p_కొలత_Init, p_కొలత_Buf);
}

#లేదావేళ IPP_సంస్కరణ_X100 >= 700
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * ippiDCTFunc)(స్థిరరాశి Ipp32f*, పూర్ణాంకం, Ipp32f*, పూర్ణాంకం, స్థిరరాశి శూన్య*, Ipp8u*);
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * ippiDCTInitAlloc)(శూన్య**, Ippi_కొలత, IppHintAlgorithm);
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * ippiDCTFree)(శూన్య* pDCTSpec);
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * ippi_DCTGet_Buf_కొలత)(స్థిరరాశి శూన్య*, పూర్ణాంకం*);

మూస<రకం_పేరు SpecType, Ipp_స్థితి (CV_STDCALL *fn)(స్థిరరాశి Ipp32f*, పూర్ణాంకం, Ipp32f*, పూర్ణాంకం, స్థిరరాశి SpecType*, Ipp8u*)>
స్థిర Ipp_స్థితి CV_STDCALL ippiDCTWrap(స్థిరరాశి Ipp32f* pSrc, పూర్ణాంకం src_అడుగు_3, Ipp32f* pDst, పూర్ణాంకం dst_అడుగు_3, స్థిరరాశి శూన్య* pDCTSpec, Ipp8u* p_బఫరు)
{
    తిరిగి fn(pSrc, src_అడుగు_3, pDst, dst_అడుగు_3, (స్థిరరాశి SpecType*)pDCTSpec, p_బఫరు);
}

మూస<రకం_పేరు SpecType, Ipp_స్థితి (CV_STDCALL *fn)(SpecType**, Ippi_కొలత, IppHintAlgorithm)>
స్థిర Ipp_స్థితి CV_STDCALL ippiDCTInitAllocWrap(శూన్య** pDCTSpec, Ippi_కొలత roi_కొలత, IppHintAlgorithm hint)
{
    తిరిగి fn((SpecType**)pDCTSpec, roi_కొలత, hint);
}

మూస<రకం_పేరు SpecType, Ipp_స్థితి (CV_STDCALL *fn)(SpecType*)>
స్థిర Ipp_స్థితి CV_STDCALL ippiDCTFreeWrap(శూన్య* pDCTSpec)
{
    తిరిగి fn((SpecType*)pDCTSpec);
}

మూస<రకం_పేరు SpecType, Ipp_స్థితి (CV_STDCALL *fn)(స్థిరరాశి SpecType*, పూర్ణాంకం*)>
స్థిర Ipp_స్థితి CV_STDCALL ippi_DCTGet_Buf_కొలత_Wrap(స్థిరరాశి శూన్య* pDCTSpec, పూర్ణాంకం* p_కొలత)
{
    తిరిగి fn((స్థిరరాశి SpecType*)pDCTSpec, p_కొలత);
}
#అయితే_ముగింపు

తరగతి DctIPPLoop_Invoker : బహిరంగ ParallelLoopBody
{
బహిరంగ:
    DctIPPLoop_Invoker(స్థిరరాశి uchar * _src, size_t src_అడుగు_2, uchar * _dst, size_t dst_అడుగు_2, పూర్ణాంకం వెడల్పు_3, తార్కిక _inv, తార్కిక *_ok) :
        ParallelLoopBody(), src(_src), src_అడుగు(src_అడుగు_2), dst(_dst), dst_అడుగు(dst_అడుగు_2), వెడల్పు(వెడల్పు_3), inv(_inv), ok(_ok)
    {
        *ok = నిజం;
    }

    మాయా శూన్య క్రియాకారకం()(స్థిరరాశి Range& range) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(*ok == అబద్ధం)
            తిరిగి;

#ఒకవేళ IPP_సంస్కరణ_X100 >= 900
        Ippi_కొలత src_Roi_కొలత = {వెడల్పు, 1};

        పూర్ణాంకం spec_కొలత    = 0;
        పూర్ణాంకం init_కొలత    = 0;
        పూర్ణాంకం బఫరు_కొలత  = 0;

        Ipp8u* pDCTSpec = ఖాళీ;
        Ipp8u* p_బఫరు  = ఖాళీ;
        Ipp8u* pInitBuf = ఖాళీ;

        #నిర్వచించు IPP_RETURN              \
            ఒకవేళ(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            ఒకవేళ(p_బఫరు)                 \
                ippFree(p_బఫరు);       \
            ఒకవేళ(pInitBuf)                \
                ippFree(pInitBuf);      \
            తిరిగి;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_కొలత  ipp_Dct_Get_కొలత  = inv ? ippi_DCTGet_కొలత_Wrap<ippi_DCTInv_Get_కొలత_32f> : ippi_DCTGet_కొలత_Wrap<ippi_DCTFwd_Get_కొలత_32f>;

        ఒకవేళ(ipp_Dct_Get_కొలత(src_Roi_కొలత, &spec_కొలత, &init_కొలత, &బఫరు_కొలత) < 0)
        {
            *ok = అబద్ధం;
            తిరిగి;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_కొలత);
        ఒకవేళ(!pDCTSpec && spec_కొలత)
        {
            *ok = అబద్ధం;
            తిరిగి;
        }

        p_బఫరు  = (Ipp8u*)CV_IPP_MALLOC(బఫరు_కొలత);
        ఒకవేళ(!p_బఫరు && బఫరు_కొలత)
        {
            *ok = అబద్ధం;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_కొలత);
        ఒకవేళ(!pInitBuf && init_కొలత)
        {
            *ok = అబద్ధం;
            IPP_RETURN
        }

        ఒకవేళ(ippDctInit(pDCTSpec, src_Roi_కొలత, pInitBuf) < 0)
        {
            *ok = అబద్ధం;
            IPP_RETURN
        }

        కొరకు(పూర్ణాంకం i = range.ప్రారంభం; i < range.end; ++i)
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (దశాంశం*)(src + src_అడుగు * i), static_cast<పూర్ణాంకం>(src_అడుగు), (దశాంశం*)(dst + dst_అడుగు * i), static_cast<పూర్ణాంకం>(dst_అడుగు), pDCTSpec, p_బఫరు) < 0)
            {
                *ok = అబద్ధం;
                IPP_RETURN
            }
        }
        IPP_RETURN
#నిర్వచనం_రద్దు IPP_RETURN
#లేదావేళ IPP_సంస్కరణ_X100 >= 700
        శూన్య* pDCTSpec;
        Auto_బఫరు<uchar> buf;
        uchar* p_బఫరు = 0;
        పూర్ణాంకం buf_కొలత=0;

        Ippi_కొలత src_Roi_కొలత = {వెడల్పు, 1};

        CV_SUPPRESS_DEPRECATED_ప్రారంభం

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_కొలత ipp_Get_Buf_కొలత = inv ? ippi_DCTGet_Buf_కొలత_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_కొలత_32f> : ippi_DCTGet_Buf_కొలత_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_కొలత_32f>;

        ఒకవేళ (ippInitAlloc(&pDCTSpec, src_Roi_కొలత, ippAlgHintNone)>=0 && ipp_Get_Buf_కొలత(pDCTSpec, &buf_కొలత)>=0)
        {
            buf.allocate( buf_కొలత );
            p_బఫరు = (uchar*)buf;

            కొరకు( పూర్ణాంకం i = range.ప్రారంభం; i < range.end; ++i)
            {
                ఒకవేళ(ippDctFun((దశాంశం*)(src + src_అడుగు * i), static_cast<పూర్ణాంకం>(src_అడుగు), (దశాంశం*)(dst + dst_అడుగు * i), static_cast<పూర్ణాంకం>(dst_అడుగు), pDCTSpec, (Ipp8u*)p_బఫరు) < 0)
                {
                    *ok = అబద్ధం;
                    ఆపు;
                }
            }
        }
        లేకపోతే
            *ok = అబద్ధం;

        ఒకవేళ (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_చివర
#లేకపోతే
        CV_వాడని(range);
        *ok = అబద్ధం;
#అయితే_ముగింపు
    }

అంతరంగ:
    స్థిరరాశి uchar * src;
    size_t src_అడుగు;
    uchar * dst;
    size_t dst_అడుగు;
    పూర్ణాంకం వెడల్పు;
    తార్కిక inv;
    తార్కిక *ok;
};

స్థిర తార్కిక DctIPPLoop(స్థిరరాశి uchar * src, size_t src_అడుగు, uchar * dst, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, తార్కిక inv)
{
    తార్కిక ok;
    parallel_for_(Range(0, ఎత్తు), DctIPPLoop_Invoker(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, inv, &ok), ఎత్తు/(ద్విగుణ)(1<<4) );
    తిరిగి ok;
}

స్థిర తార్కిక ippi_DCT_32f(స్థిరరాశి uchar * src, size_t src_అడుగు, uchar * dst, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, తార్కిక inv, తార్కిక అడ్డువరుస)
{
    CV_INSTRUMENT_REGION_IPP();

    ఒకవేళ(అడ్డువరుస)
        తిరిగి DctIPPLoop(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, ఎత్తు, inv);
    లేకపోతే
    {
#ఒకవేళ IPP_సంస్కరణ_X100 >= 900
        Ippi_కొలత src_Roi_కొలత = {వెడల్పు, ఎత్తు};

        పూర్ణాంకం spec_కొలత    = 0;
        పూర్ణాంకం init_కొలత    = 0;
        పూర్ణాంకం బఫరు_కొలత  = 0;

        Ipp8u* pDCTSpec = ఖాళీ;
        Ipp8u* p_బఫరు  = ఖాళీ;
        Ipp8u* pInitBuf = ఖాళీ;

        #నిర్వచించు IPP_RELEASE             \
            ఒకవేళ(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            ఒకవేళ(p_బఫరు)                 \
                ippFree(p_బఫరు);       \
            ఒకవేళ(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_కొలత  ipp_Dct_Get_కొలత  = inv ? ippi_DCTGet_కొలత_Wrap<ippi_DCTInv_Get_కొలత_32f> : ippi_DCTGet_కొలత_Wrap<ippi_DCTFwd_Get_కొలత_32f>;

        ఒకవేళ(ipp_Dct_Get_కొలత(src_Roi_కొలత, &spec_కొలత, &init_కొలత, &బఫరు_కొలత) < 0)
            తిరిగి అబద్ధం;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_కొలత);
        ఒకవేళ(!pDCTSpec && spec_కొలత)
            తిరిగి అబద్ధం;

        p_బఫరు  = (Ipp8u*)CV_IPP_MALLOC(బఫరు_కొలత);
        ఒకవేళ(!p_బఫరు && బఫరు_కొలత)
        {
            IPP_RELEASE
            తిరిగి అబద్ధం;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_కొలత);
        ఒకవేళ(!pInitBuf && init_కొలత)
        {
            IPP_RELEASE
            తిరిగి అబద్ధం;
        }

        ఒకవేళ(ippDctInit(pDCTSpec, src_Roi_కొలత, pInitBuf) < 0)
        {
            IPP_RELEASE
            తిరిగి అబద్ధం;
        }

        ఒకవేళ(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (దశాంశం*)src, static_cast<పూర్ణాంకం>(src_అడుగు), (దశాంశం*)dst, static_cast<పూర్ణాంకం>(dst_అడుగు), pDCTSpec, p_బఫరు) < 0)
        {
            IPP_RELEASE
            తిరిగి అబద్ధం;
        }

        IPP_RELEASE
        తిరిగి నిజం;
#నిర్వచనం_రద్దు IPP_RELEASE
#లేదావేళ IPP_సంస్కరణ_X100 >= 700
        Ipp_స్థితి స్థితి_2;
        శూన్య* pDCTSpec;
        Auto_బఫరు<uchar> buf;
        uchar* p_బఫరు = 0;
        పూర్ణాంకం buf_కొలత=0;

        Ippi_కొలత src_Roi_కొలత = {వెడల్పు, ఎత్తు};

        CV_SUPPRESS_DEPRECATED_ప్రారంభం

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_కొలత ipp_Get_Buf_కొలత = inv ? ippi_DCTGet_Buf_కొలత_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_కొలత_32f> : ippi_DCTGet_Buf_కొలత_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_కొలత_32f>;

        స్థితి_2 = ippStsErr;

        ఒకవేళ (ippInitAlloc(&pDCTSpec, src_Roi_కొలత, ippAlgHintNone)>=0 && ipp_Get_Buf_కొలత(pDCTSpec, &buf_కొలత)>=0)
        {
            buf.allocate( buf_కొలత );
            p_బఫరు = (uchar*)buf;

            స్థితి_2 = ippDctFun((దశాంశం*)src, static_cast<పూర్ణాంకం>(src_అడుగు), (దశాంశం*)dst, static_cast<పూర్ణాంకం>(dst_అడుగు), pDCTSpec, (Ipp8u*)p_బఫరు);
        }

        ఒకవేళ (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_చివర

        తిరిగి స్థితి_2 >= 0;
#లేకపోతే
        CV_వాడని(src); CV_వాడని(dst); CV_వాడని(inv); CV_వాడని(అడ్డువరుస);
        తిరిగి అబద్ధం;
#అయితే_ముగింపు
    }
}
}
#అయితే_ముగింపు

నామాకాశ cv {

తరగతి OcvDctImpl CV_FINAL : బహిరంగ hal::DCT2D
{
బహిరంగ:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    Auto_బఫరు<uchar> ws;
    తార్కిక is_అడ్డువరుస_Transform;
    తార్కిక isInverse;
    తార్కిక isContinuous;
    పూర్ణాంకం ప్రారంభం_stage;
    పూర్ణాంకం చివర_stage;
    పూర్ణాంకం వెడల్పు;
    పూర్ణాంకం ఎత్తు;
    పూర్ణాంకం లోతు;

    శూన్య init(పూర్ణాంకం వెడల్పు_3, పూర్ణాంకం ఎత్తు_3, పూర్ణాంకం లోతు_2, పూర్ణాంకం flags)
    {
        వెడల్పు = వెడల్పు_3;
        ఎత్తు = ఎత్తు_3;
        లోతు = లోతు_2;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        is_అడ్డువరుస_Transform = (flags & CV_HAL_DFT_అడ్డువరుసలు) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        స్థిర DCTFunc dct_tbl[4] =
        {
            dctWrap<దశాంశం, DCT<దశాంశం>>,
            dctWrap<దశాంశం, IDCT<దశాంశం>>,
            dctWrap<ద్విగుణ, DCT<ద్విగుణ>>,
            dctWrap<ద్విగుణ, IDCT<ద్విగుణ>>
        };
        dct_func = dct_tbl[(పూర్ణాంకం)isInverse + (లోతు == CV_64F)*2];
        opt.isComplex = అబద్ధం;
        opt.isInverse = అబద్ధం;
        opt.scale = 1.;

        ఒకవేళ (is_అడ్డువరుస_Transform || ఎత్తు == 1 || (వెడల్పు == 1 && isContinuous))
        {
            ప్రారంభం_stage = చివర_stage = 0;
        }
        లేకపోతే
        {
            ప్రారంభం_stage = (వెడల్పు == 1);
            చివర_stage = 1;
        }
    }
    శూన్య apply(స్థిరరాశి uchar *src, size_t src_అడుగు, uchar *dst, size_t dst_అడుగు) CV_OVERRIDE
    {
        CV_IPP_నిరంతరం(IPP_సంస్కరణ_X100 >= 700 && లోతు == CV_32F, ippi_DCT_32f(src, src_అడుగు, dst, dst_అడుగు, వెడల్పు, ఎత్తు, isInverse, is_అడ్డువరుస_Transform))

        పూర్ణాంకం prev_len = 0;
        పూర్ణాంకం elem_కొలత_2 = (లోతు == CV_32F) ? పరిమాణం(దశాంశం) : పరిమాణం(ద్విగుణ);
        స్థిరరాశి DFTKernels& kernels = getDFTKernels(లోతు);

        కొరకు(పూర్ణాంకం stage = ప్రారంభం_stage ; stage <= చివర_stage; stage++ )
        {
            స్థిరరాశి uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            పూర్ణాంకం len, లెక్క_2;

            ఒకవేళ( stage == 0 )
            {
                len = వెడల్పు;
                లెక్క_2 = ఎత్తు;
                ఒకవేళ( len == 1 && !is_అడ్డువరుస_Transform )
                {
                    len = ఎత్తు;
                    లెక్క_2 = 1;
                }
                sstep0 = src_అడుగు;
                dstep0 = dst_అడుగు;
                sstep1 = dstep1 = elem_కొలత_2;
            }
            లేకపోతే
            {
                len = ఎత్తు;
                లెక్క_2 = వెడల్పు;
                sstep1 = src_అడుగు;
                dstep1 = dst_అడుగు;
                sstep0 = dstep0 = elem_కొలత_2;
            }

            opt.n = len;

            ఒకవేళ( len != prev_len )
            {
                ఒకవేళ( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                ఒకవేళ( len > 1 )
                {
                    plan.build(isInverse ? DFT_రకం_IDCT : DFT_రకం_DCT, len, లోతు, kernels.vlanes);
                    ws.allocate(plan.ws_బైట్లు);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = ws.దత్తాంశం();
                prev_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            ద్విగుణ nstripes = dftParallelStripes(లెక్క_2, len);
            ఒకవేళ( len > 1 && లెక్క_2 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                స్వయంచాలక process_పంక్తులు = [&](స్థిరరాశి Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    Auto_బఫరు<uchar> ws_(plan.ws_బైట్లు);
                    opt_.workspace = ws_.దత్తాంశం();
                    కొరకు( పూర్ణాంకం i = range.ప్రారంభం; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, లెక్క_2), process_పంక్తులు, nstripes);
            }
            లేకపోతే
            {
                కొరకు(గుర్తులేని i = 0; i < static_cast<గుర్తులేని>(లెక్క_2); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_అడుగు = dst_అడుగు;
        }
    }
};

నిర్మాణం ReplacementDCT2D : బహిరంగ hal::DCT2D
{
    cvhalDFT *context;
    తార్కిక isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(అబద్ధం) {}
    తార్కిక init(పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం లోతు, పూర్ణాంకం flags)
    {
        పూర్ణాంకం res = cv_hal_dctInit2D(&context, వెడల్పు, ఎత్తు, లోతు, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        తిరిగి isInitialized;
    }
    శూన్య apply(స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు) CV_OVERRIDE
    {
        ఒకవేళ (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_దత్తాంశం, src_అడుగు, dst_దత్తాంశం, dst_అడుగు);
        }
    }
    ~ReplacementDCT2D()
    {
        ఒకవేళ (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

నామాకాశ hal {

Ptr<DCT2D> DCT2D::create(పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం లోతు, పూర్ణాంకం flags)
{
    {
        ReplacementDCT2D *impl = కొత్త ReplacementDCT2D();
        ఒకవేళ (impl->init(వెడల్పు, ఎత్తు, లోతు, flags))
        {
            తిరిగి Ptr<DCT2D>(impl);
        }
        తొలగించు impl;
    }
    {
        OcvDctImpl *impl = కొత్త OcvDctImpl();
        impl->init(వెడల్పు, ఎత్తు, లోతు, flags);
        తిరిగి Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

శూన్య cv::dct( ఆదానం_Array _src0, OutputArray _dst, పూర్ణాంకం flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    ఒకవేళ (src.empty())
    {
        _dst.release();
        తిరిగి;
    }
    పూర్ణాంకం type = src.type(), లోతు = src.లోతు();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.అడ్డువరుసలు, src.నిలువుల_సంఖ్య, type );
    Mat dst = _dst.getMat();

    పూర్ణాంకం f = 0;
    ఒకవేళ ((flags & DFT_అడ్డువరుసలు) != 0)
        f |= CV_HAL_DFT_అడ్డువరుసలు;
    ఒకవేళ ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    ఒకవేళ (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, లోతు, f);
    c->apply(src.దత్తాంశం, src.అడుగు, dst.దత్తాంశం, dst.అడుగు);
}


శూన్య cv::idct( ఆదానం_Array src, OutputArray dst, పూర్ణాంకం flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

నామాకాశ cv
{

స్థిర స్థిరరాశి పూర్ణాంకం optimalDFTSizeTab[] = {
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

పూర్ణాంకం cv::getOptimalDFTSize( పూర్ణాంకం size0 )
{
    పూర్ణాంకం a = 0, b = పరిమాణం(optimalDFTSizeTab)/పరిమాణం(optimalDFTSizeTab[0]) - 1;
    ఒకవేళ( (గుర్తులేని)size0 >= (గుర్తులేని)optimalDFTSizeTab[b] )
        తిరిగి -1;

    అయితే( a < b )
    {
        పూర్ణాంకం c = (a + b) >> 1;
        ఒకవేళ( size0 <= optimalDFTSizeTab[c] )
            b = c;
        లేకపోతే
            a = c+1;
    }

    తిరిగి optimalDFTSizeTab[b];
}
/* End of file. */
