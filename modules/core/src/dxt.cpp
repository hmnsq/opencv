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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "dxt.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "dxt.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "dxt.simd_declarations.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <map>

ਨਾਮਾਕਾਸ਼ cv
{

#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 710
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_IPP_DFT 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ USE_IPP_DFT
#ਜੇ_ਅੰਤ

#ਜੇ defined USE_IPP_DFT
#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 202220
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IPP_DISABLE_DFT32F ((ਡੂੰਘਾਈ == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IPP_DISABLE_DFT32F ਝੂਠ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
// factorization used by the OpenCL radix selection
ਸਥਿਰ ਪੂਰਨ
DFTFactorize( ਪੂਰਨ n, ਪੂਰਨ* factors )
{
    ਪੂਰਨ nf = 0, f, i, j;

    ਜੇ( n <= 5 )
    {
        factors[0] = n;
        ਵਾਪਸ 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    ਜੇ( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    ਲਈ( f = 3; n > 1; )
    {
        ਪੂਰਨ d = n/f;
        ਜੇ( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        ਨਹੀਂਤਾਂ
        {
            f += 2;
            ਜੇ( f*f > n )
                ਰੋਕੋ;
        }
    }

    ਜੇ( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    ਲਈ( i = f; i < (nf+f)/2; i++ )
        CV_ਅਦਲਾ_ਬਦਲੀ( factors[i], factors[nf-i-1+f], j );

    ਵਾਪਸ nf;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
ਸਥਿਰ Ipp_ਸਥਿਤੀ ippsDFTFwd_CToC( ਅਚਲ Complex<ਦਸ਼ਮਲਵ>* src, Complex<ਦਸ਼ਮਲਵ>* dst,
                             ਅਚਲ ਖਾਲੀ* spec, uchar* buf)
{
    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (ਅਚਲ Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ਅਚਲ IppsDFTSpec_C_32fc*)spec, buf);
}

ਸਥਿਰ Ipp_ਸਥਿਤੀ ippsDFTFwd_CToC( ਅਚਲ Complex<ਦੁੱਗਣਾ>* src, Complex<ਦੁੱਗਣਾ>* dst,
                             ਅਚਲ ਖਾਲੀ* spec, uchar* buf)
{
    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (ਅਚਲ Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ਅਚਲ IppsDFTSpec_C_64fc*)spec, buf);
}

ਸਥਿਰ Ipp_ਸਥਿਤੀ ippsDFTInv_CToC( ਅਚਲ Complex<ਦਸ਼ਮਲਵ>* src, Complex<ਦਸ਼ਮਲਵ>* dst,
                             ਅਚਲ ਖਾਲੀ* spec, uchar* buf)
{
    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (ਅਚਲ Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ਅਚਲ IppsDFTSpec_C_32fc*)spec, buf);
}

ਸਥਿਰ Ipp_ਸਥਿਤੀ ippsDFTInv_CToC( ਅਚਲ Complex<ਦੁੱਗਣਾ>* src, Complex<ਦੁੱਗਣਾ>* dst,
                                  ਅਚਲ ਖਾਲੀ* spec, uchar* buf)
{
    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (ਅਚਲ Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ਅਚਲ IppsDFTSpec_C_64fc*)spec, buf);
}

ਸਥਿਰ Ipp_ਸਥਿਤੀ ippsDFTFwd_RToPack( ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst,
                                     ਅਚਲ ਖਾਲੀ* spec, uchar* buf)
{
    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (ਅਚਲ IppsDFTSpec_R_32f*)spec, buf);
}

ਸਥਿਰ Ipp_ਸਥਿਤੀ ippsDFTFwd_RToPack( ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst,
                                     ਅਚਲ ਖਾਲੀ* spec, uchar* buf)
{
    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (ਅਚਲ IppsDFTSpec_R_64f*)spec, buf);
}

ਸਥਿਰ Ipp_ਸਥਿਤੀ ipps_DFTInv_Pack_ਮੰਜ਼ਿਲ_R( ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst,
                                     ਅਚਲ ਖਾਲੀ* spec, uchar* buf)
{
    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_ਮੰਜ਼ਿਲ_R_32f, src, dst, (ਅਚਲ IppsDFTSpec_R_32f*)spec, buf);
}

ਸਥਿਰ Ipp_ਸਥਿਤੀ ipps_DFTInv_Pack_ਮੰਜ਼ਿਲ_R( ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst,
                                     ਅਚਲ ਖਾਲੀ* spec, uchar* buf)
{
    ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_ਮੰਜ਼ਿਲ_R_64f, src, dst, (ਅਚਲ IppsDFTSpec_R_64f*)spec, buf);
}
#ਜੇ_ਅੰਤ

ਬਣਤਰ OcvDftOptions;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*DFTFunc)(ਅਚਲ OcvDftOptions & c, ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
ਬਣਤਰ OcvDftOptions {
    ਦੁੱਗਣਾ scale;
    ਪੂਰਨ n;

    ਤਾਰਕਿਕ isInverse;
    ਤਾਰਕਿਕ isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    ਤਾਰਕਿਕ useIpp;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#ਜੇ_ਅੰਤ

    ਅਚਲ DftPlan* plan;
    ਅਚਲ DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = ਝੂਠ;
        isComplex = ਝੂਠ;
        useIpp = ਝੂਠ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
        ipp_spec = 0;
        ipp_work = 0;
#ਜੇ_ਅੰਤ
        dft_func = 0;
    }
};


/****************************************************************************************\
                     New 1D engine: kernel table, plan builder, driver
\****************************************************************************************/

ਸਥਿਰ DFTKernels getDFTKernelsDispatch(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_CPU_DISPATCH(getDFTKernels_, (ਡੂੰਘਾਈ), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
ਸਥਿਰ ਅਚਲ DFTKernels& getDFTKernels(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਅਚਲ DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    ਸਥਿਰ ਅਚਲ DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    ਵਾਪਸ ਡੂੰਘਾਈ == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
ਬਣਤਰ DftTwiddleGen
{
    ਪੂਰਨ M, logB, ਨਕਾਬ_B;
    Auto_ਬਫਰ<ਦੁੱਗਣਾ, 1024> buf;
    ਦੁੱਗਣਾ *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    ਸਥਿਰ ਖਾਲੀ fill_ਸਾਰਣੀ(ਦੁੱਗਣਾ* re, ਦੁੱਗਣਾ* im, ਪੂਰਨ ਗਿਣਤੀ_3, ਦੁੱਗਣਾ wr, ਦੁੱਗਣਾ wi, ਦੁੱਗਣਾ ਕਦਮ_Angle)
    {
        ਲਈ (ਪੂਰਨ i0 = 0; i0 < ਗਿਣਤੀ_3; i0 += 16)
        {
            ਦੁੱਗਣਾ t0 = ਕਦਮ_Angle*i0;
            ਦੁੱਗਣਾ cr = i0 == 0 ? 1 : ਕੋਜੀਵਾ(t0), ci = i0 == 0 ? 0 : ਜੀਵਾ(t0);
            ਪੂਰਨ i1 = std::min(i0 + 16, ਗਿਣਤੀ_3);
            ਲਈ (ਪੂਰਨ i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                ਦੁੱਗਣਾ t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    ਖਾਲੀ init(ਪੂਰਨ _M)
    {
        M = _M;
        logB = 0;
        ਜਦੋਂ ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        ਪੂਰਨ B = 1 << logB;
        ਨਕਾਬ_B = B - 1;
        ਪੂਰਨ nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.ਅੰਕੜੇ(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        ਦੁੱਗਣਾ a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fill_ਸਾਰਣੀ(lo_re, lo_im, B, ਕੋਜੀਵਾ(a), ਜੀਵਾ(a), a);
        fill_ਸਾਰਣੀ(hi_re, hi_im, nhi, ਕੋਜੀਵਾ(a*B), ਜੀਵਾ(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    ਖਾਲੀ get(ਪੂਰਨ m, ਦੁੱਗਣਾ& re, ਦੁੱਗਣਾ& im) ਅਚਲ
    {
        CV_DbgAssert(m >= 0);
        ਜੇ (m >= M)
            m %= M;
        ਪੂਰਨ a = m >> logB, b = m & ਨਕਾਬ_B;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਖਾਲੀ DftPlan::fillTables(size_t ntw, ਤਾਰਕਿਕ need_rtw, ਤਾਰਕਿਕ need_dct)
{
    DftPlan& p = *ਇਹ;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    ਪੂਰਨ M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*ਆਕਾਰ(T) + ਆਕਾਰ(ਦੁੱਗਣਾ) - 1)/ਆਕਾਰ(ਦੁੱਗਣਾ) + 1);
    T* cur = (T*)p.tw.ਅੰਕੜੇ();
    ਪੂਰਨ maxL = 1;
    ਲਈ (ਪੂਰਨ s = 0; s < p.nstages; s++)
        ਜੇ (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].ਵਿਸਤਾਰ);
    Auto_ਬਫਰ<ਦੁੱਗਣਾ, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    ਦੁੱਗਣਾ* twd = twd_buf.ਅੰਕੜੇ();
    ਲਈ (ਪੂਰਨ s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        ਪੂਰਨ r = st.radix, L = st.ਵਿਸਤਾਰ, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        ਪੂਰਨ ਕਦਮ = M/(r*L);
        ਜੇ (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr ਪੂਰਨ K = 8;
            ਦੁੱਗਣਾ cr[K], ci[K], wr, wi;
            gen.get(K*ਕਦਮ, wr, wi);
            ਦੁੱਗਣਾ* t1r = twd; ਦੁੱਗਣਾ* t1i = twd + L;      // leg 1 kept in double for the products
            ਲਈ (ਪੂਰਨ j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*ਕਦਮ, cr[k], ci[k]);
                ਪੂਰਨ j1 = std::min(j0 + 16*K, L);
                ਲਈ (ਪੂਰਨ j = j0; j < j1; j += K)
                    ਲਈ (ਪੂਰਨ k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        ਦੁੱਗਣਾ t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            ਲਈ (ਪੂਰਨ j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            ਲਈ (ਪੂਰਨ q = 2; q < r; q++)
            {
                ਅਚਲ T* pr = twr + (q-2)*ts; ਅਚਲ T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                ਲਈ (ਪੂਰਨ j = 0; j < L; j++)
                {
                    ਦੁੱਗਣਾ ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        ਨਹੀਂਤਾਂ
        {
            // large odd radix: exact-ish generator lookup per entry
            ਲਈ (ਪੂਰਨ q = 1; q < r; q++)
                ਲਈ (ਪੂਰਨ j = 0; j < L; j++)
                {
                    ਦੁੱਗਣਾ re, im;
                    gen.get(q*j*ਕਦਮ, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        ਜੇ (r >= 7)
        {
            ਪੂਰਨ h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            ਲਈ (ਪੂਰਨ pp = 1; pp <= h; pp++)
                ਲਈ (ਪੂਰਨ k = 1; k <= h; k++)
                {
                    ਦੁੱਗਣਾ t = 2*CV_PI*((ਦੁੱਗਣਾ)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)ਕੋਜੀਵਾ(t);
                    sn[(pp-1)*h + (k-1)] = (T)ਜੀਵਾ(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    ਜੇ (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        ਪੂਰਨ ਕਦਮ = M/n;
        ਲਈ (ਪੂਰਨ k = 0; k <= nc; k++)
        {
            ਦੁੱਗਣਾ re, im;
            gen.get(k*ਕਦਮ, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    ਜੇ (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        ਦੁੱਗਣਾ scale = (p.ਕਿਸਮ == DFT_ਕਿਸਮ_DCT ? 2 : 1)*std::ਵਰਗਮੂਲ(1./(2*n));
        ਪੂਰਨ ਕਦਮ = M/(4*n);
        ਲਈ (ਪੂਰਨ k = 0; k <= nc; k++)
        {
            ਦੁੱਗਣਾ re, im;
            gen.get(k*ਕਦਮ, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
ਖਾਲੀ DftPlan::build(ਪੂਰਨ ਕਿਸਮ_2, ਪੂਰਨ _n, ਪੂਰਨ ਡੂੰਘਾਈ_2, ਪੂਰਨ _vl)
{
    DftPlan& p = *ਇਹ;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(ਡੂੰਘਾਈ_2 == CV_32F || ਡੂੰਘਾਈ_2 == CV_64F);
    ਤਾਰਕਿਕ real_ਕਿਸਮ = ਕਿਸਮ_2 != DFT_ਕਿਸਮ_C2_C;
    ਤਾਰਕਿਕ dct_ਕਿਸਮ = ਕਿਸਮ_2 == DFT_ਕਿਸਮ_DCT || ਕਿਸਮ_2 == DFT_ਕਿਸਮ_IDCT;
    ਜੇ (dct_ਕਿਸਮ)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.ਕਿਸਮ = ਕਿਸਮ_2; p.n = _n; p.ਡੂੰਘਾਈ = ਡੂੰਘਾਈ_2; p.vl = _vl;
    p.nc = real_ਕਿਸਮ && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = ਡੂੰਘਾਈ == CV_32F ? ਆਕਾਰ(ਦਸ਼ਮਲਵ) : ਆਕਾਰ(ਦੁੱਗਣਾ);
    p.real_ਨਿਵੇਸ਼ = ਕਿਸਮ == DFT_ਕਿਸਮ_R2_C && (n & 1) != 0;
    p.need_tmp = ਕਿਸਮ == DFT_ਕਿਸਮ_C2_R || ਕਿਸਮ == DFT_ਕਿਸਮ_DCT || ਕਿਸਮ == DFT_ਕਿਸਮ_IDCT;
    p.pingpong = ਝੂਠ;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    ਪੂਰਨ k2 = 0, rest = nc;
    ਜਦੋਂ ((rest & 1) == 0) { rest >>= 1; k2++; }
    ਪੂਰਨ r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.ਪਹਿਲਾ_radix = r0;
    ਪੂਰਨ radices[DFT_MAX_STAGES], nst = 0;
    ਲਈ (ਪੂਰਨ m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    ਜਦੋਂ (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    ਜਦੋਂ (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    ਲਈ (ਪੂਰਨ f = 7; rest > 1; f += 2)
    {
        ਜੇ (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            ਰੋਕੋ;
        }
        ਜਦੋਂ (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    ਪੂਰਨ L = r0, max_h = 0;
    size_t ntw = 0;
    ਲਈ (ਪੂਰਨ s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        ਪੂਰਨ r = radices[s];
        st.radix = r; st.ਵਿਸਤਾਰ = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        ਜੇ (vl > 1 && L >= vl)
        {
            st.ਬਹੁਲਕ = DFT_STAGE_ਭਰਿਆ;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            ਜੇ (L % vl != 0)
                p.pingpong = ਸੱਚ;   // back-off clamp => overlapping stores => must be out-of-place
        }
        ਨਹੀਂਤਾਂ
        {
            st.ਬਹੁਲਕ = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        ਜੇ (r >= 7)
        {
            ਪੂਰਨ h = (r - 1)/2;
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
    ਪੂਰਨ seq[DFT_MAX_STAGES + 1], nseq = 0;
    ਜੇ (r0 > 1) seq[nseq++] = r0;
    ਲਈ (ਪੂਰਨ s = 0; s < nst; s++) seq[nseq++] = radices[s];
    ਪੂਰਨ digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        ਪੂਰਨ Ls = 1;
        ਲਈ (ਪੂਰਨ s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    ਜੇ (r0 == 1)
    {
        p.itab.allocate(nc*2);
        ਪੂਰਨ* it_all = p.itab.ਅੰਕੜੇ();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        ਪੂਰਨ n_ਵਿੱਚ = std::min(nseq, 2);
        ਪੂਰਨ r_0 = n_ਵਿੱਚ > 0 ? seq[0] : 1, w_0 = n_ਵਿੱਚ > 0 ? weights[0] : 0;
        ਪੂਰਨ r_1 = n_ਵਿੱਚ > 1 ? seq[1] : 1, w_1 = n_ਵਿੱਚ > 1 ? weights[1] : 0;
        ਲਈ (ਪੂਰਨ pos = 0, rev_hi = 0; pos < nc; )
        {
            ਲਈ (ਪੂਰਨ d1 = 0; d1 < r_1; d1++)
            {
                ਪੂਰਨ rev = rev_hi + d1*w_1;
                ਪੂਰਨ* it = it_all + pos*2;
                ਜੇ (p.real_ਨਿਵੇਸ਼)
                {
                    ਲਈ (ਪੂਰਨ d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    ਲਈ (ਪੂਰਨ d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            ਲਈ (ਪੂਰਨ s = n_ਵਿੱਚ; s < nseq; s++)
            {
                rev_hi += weights[s];
                ਜੇ (++digits[s] < seq[s])
                    ਰੋਕੋ;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        ਪੂਰਨ M = nc/r0;
        p.itab.allocate(M);
        ਪੂਰਨ* gtab = p.itab.ਅੰਕੜੇ();
        ਪੂਰਨ gw[DFT_MAX_STAGES + 1];
        {
            ਪੂਰਨ Ls = 1;
            ਲਈ (ਪੂਰਨ s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        ਪੂਰਨ sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        ਪੂਰਨ r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        ਪੂਰਨ r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        ਲਈ (ਪੂਰਨ ਅਧਾਰ = 0, g_hi = 0; ਅਧਾਰ < M; )
        {
            ਲਈ (ਪੂਰਨ db = 0; db < r_b; db++)
            {
                ਪੂਰਨ g = (g_hi + db*g_b)*r0;
                ਲਈ (ਪੂਰਨ da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[ਅਧਾਰ++] = g;
            }
            ਲਈ (ਪੂਰਨ s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                ਜੇ (++digits[s] < seq[s])
                    ਰੋਕੋ;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    ਤਾਰਕਿਕ need_rtw = real_ਕਿਸਮ && n % 2 == 0;
    ਜੇ (need_rtw) ntw += 2*(size_t)(nc + 1);
    ਜੇ (dct_ਕਿਸਮ) ntw += 2*(size_t)(nc + 1);
    ਜੇ (ਡੂੰਘਾਈ == CV_32F)
        fillTables<ਦਸ਼ਮਲਵ>(ntw, need_rtw, dct_ਕਿਸਮ);
    ਨਹੀਂਤਾਂ
        fillTables<ਦੁੱਗਣਾ>(ntw, need_rtw, dct_ਕਿਸਮ);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_ਬਾਈਟਾਂ = 2*(size_t)nc*esz;
    ਤਾਰਕਿਕ two_ਜੋੜੇ = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_ਜੋੜੇ ? pair_ਬਾਈਟਾਂ : 0;
    p.scratch_ofs = pair_ਬਾਈਟਾਂ*(two_ਜੋੜੇ ? 2 : 1);
    size_t scratch_ਬਾਈਟਾਂ = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_ਬਾਈਟਾਂ = p.scratch_ofs + scratch_ਬਾਈਟਾਂ + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Dft(ਅਚਲ OcvDftOptions& c, ਅਚਲ ਖਾਲੀ* src, size_t sstep, ਖਾਲੀ* dst, size_t dstep,
                   ਤਾਰਕਿਕ complex_io)
{
    ਅਚਲ DftPlan& p = *c.plan;
    ਅਚਲ DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.ਡੂੰਘਾਈ == CV_32F ? ਆਕਾਰ(ਦਸ਼ਮਲਵ) : ਆਕਾਰ(ਦੁੱਗਣਾ);
    uchar* ਅਧਾਰ = alignPtr(c.workspace, 64);
    uchar* re0 = ਅਧਾਰ;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = ਅਧਾਰ + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = ਅਧਾਰ + p.scratch_ofs;

    ਅਚਲ ਖਾਲੀ* asrc = src;
    size_t astep = sstep;
    ਤਾਰਕਿਕ conj = ਝੂਠ;
    ਜੇ (p.ਕਿਸਮ == DFT_ਕਿਸਮ_C2_R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = ਸੱਚ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (p.ਕਿਸਮ == DFT_ਕਿਸਮ_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = ਸੱਚ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (p.ਕਿਸਮ == DFT_ਕਿਸਮ_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    ਨਹੀਂਤਾਂ ਜੇ (p.ਕਿਸਮ == DFT_ਕਿਸਮ_C2_C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.ਪਹਿਲਾ_radix == 1);

    DftPreprocFunc pre = p.ਪਹਿਲਾ_radix == 1 ? k.preprocRadix0 : p.ਪਹਿਲਾ_radix == 2 ? k.preprocRadix2 :
                         p.ਪਹਿਲਾ_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    ਲਈ (ਪੂਰਨ i = 0; i < p.nstages; i++)
    {
        ਅਚਲ DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        ਜੇ (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        ਨਹੀਂਤਾਂ
            fn(st, sre, sim, sre, sim, scratch);
    }

    ਦੁੱਗਣਾ scale = c.scale;
    ਚੋਣ (p.ਕਿਸਮ)
    {
    ਹਾਲਤ DFT_ਕਿਸਮ_C2_C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        ਰੋਕੋ;
    ਹਾਲਤ DFT_ਕਿਸਮ_R2_C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        ਰੋਕੋ;
    ਹਾਲਤ DFT_ਕਿਸਮ_C2_R:
        ਜੇ (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        ਨਹੀਂਤਾਂ
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        ਰੋਕੋ;
    ਹਾਲਤ DFT_ਕਿਸਮ_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        ਰੋਕੋ;
    ਮੂਲ:
        CV_Assert(p.ਕਿਸਮ == DFT_ਕਿਸਮ_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
DFT(ਅਚਲ OcvDftOptions & c, ਅਚਲ Complex<T>* src, Complex<T>* dst)
{
    ਜੇ( c.useIpp )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
        ਜੇ( !c.isInverse )
        {
            ਜੇ (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
                ਵਾਪਸ;
            }
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
                ਵਾਪਸ;
            }
        }
        ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
#ਜੇ_ਅੰਤ
    }

    ਲੜੀਵਾਰ_Dft(c, src, 1, dst, 1, ਝੂਠ);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
RealDFT(ਅਚਲ OcvDftOptions & c, ਅਚਲ T* src, T* dst)
{
    ਪੂਰਨ complex_output = c.isComplex;

    ਜੇ( c.useIpp )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
        ਪੂਰਨ n = c.n;
        T* ipp_dst = dst + complex_output;
        ਜੇ (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            ਜੇ( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                ਜੇ( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
            ਵਾਪਸ;
        }
        ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
#ਜੇ_ਅੰਤ
    }
    ਲੜੀਵਾਰ_Dft(c, src, 1, dst, 1, complex_output != 0);
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
CCSIDFT(ਅਚਲ OcvDftOptions & c, ਅਚਲ T* src, T* dst)
{
    ਪੂਰਨ complex_ਨਿਵੇਸ਼ = c.isComplex;

    ਜੇ( complex_ਨਿਵੇਸ਼ )
        CV_Assert( src != dst );
    ਜੇ( c.useIpp )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        ਅਚਲ T* ipp_src = src;
        T save_s1 = 0;
        ਜੇ( complex_ਨਿਵੇਸ਼ )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        Ipp_ਸਥਿਤੀ ਸਥਿਤੀ = ipps_DFTInv_Pack_ਮੰਜ਼ਿਲ_R( ipp_src, dst, c.ipp_spec, c.ipp_work );
        ਜੇ( complex_ਨਿਵੇਸ਼ )
            ((T*)src)[1] = save_s1;
        ਜੇ( ਸਥਿਤੀ >= 0 )
        {
            CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
            ਵਾਪਸ;
        }
        ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
#ਜੇ_ਅੰਤ
    }
    ਲੜੀਵਾਰ_Dft(c, src, 1, dst, 1, complex_ਨਿਵੇਸ਼ != 0);
}

ਸਥਿਰ ਖਾਲੀ
Copy_ਕਾਲਮ( ਅਚਲ uchar* _src, size_t src_ਕਦਮ,
            uchar* _dst, size_t dst_ਕਦਮ,
            ਪੂਰਨ len, size_t elem_ਮਾਪ_2 )
{
    ਪੂਰਨ i, t0, t1;
    ਅਚਲ ਪੂਰਨ* src = (ਅਚਲ ਪੂਰਨ*)_src;
    ਪੂਰਨ* dst = (ਪੂਰਨ*)_dst;
    src_ਕਦਮ /= ਆਕਾਰ(src[0]);
    dst_ਕਦਮ /= ਆਕਾਰ(dst[0]);

    ਜੇ( elem_ਮਾਪ_2 == ਆਕਾਰ(ਪੂਰਨ) )
    {
        ਲਈ( i = 0; i < len; i++, src += src_ਕਦਮ, dst += dst_ਕਦਮ )
            dst[0] = src[0];
    }
    ਨਹੀਂਤਾਂ ਜੇ( elem_ਮਾਪ_2 == ਆਕਾਰ(ਪੂਰਨ)*2 )
    {
        ਲਈ( i = 0; i < len; i++, src += src_ਕਦਮ, dst += dst_ਕਦਮ )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( elem_ਮਾਪ_2 == ਆਕਾਰ(ਪੂਰਨ)*4 )
    {
        ਲਈ( i = 0; i < len; i++, src += src_ਕਦਮ, dst += dst_ਕਦਮ )
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
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
copyFromColumns_( ਅਚਲ uchar* src, size_t src_ਕਦਮ, T* dst, ਪੂਰਨ ncols, ਪੂਰਨ len )
{
    ਲਈ( ਪੂਰਨ i = 0; i < len; i++, src += src_ਕਦਮ )
    {
        ਅਚਲ T* s = (ਅਚਲ T*)src;
        ਲਈ( ਪੂਰਨ k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

ਸਥਿਰ ਖਾਲੀ
CopyFromColumns( ਅਚਲ uchar* src, size_t src_ਕਦਮ, uchar* dst, ਪੂਰਨ ncols, ਪੂਰਨ len, size_t elem_ਮਾਪ_2 )
{
    ਜੇ( elem_ਮਾਪ_2 == ਆਕਾਰ(ਪੂਰਨ) )
        copyFromColumns_(src, src_ਕਦਮ, (ਪੂਰਨ*)dst, ncols, len);
    ਨਹੀਂਤਾਂ ਜੇ( elem_ਮਾਪ_2 == ਆਕਾਰ(int64) )
        copyFromColumns_(src, src_ਕਦਮ, (int64*)dst, ncols, len);
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( elem_ਮਾਪ_2 == ਆਕਾਰ(int64)*2 );
        copyFromColumns_(src, src_ਕਦਮ, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
copy_ਮੰਜ਼ਿਲ_Columns( ਅਚਲ T* src, uchar* dst, size_t dst_ਕਦਮ, ਪੂਰਨ ncols, ਪੂਰਨ len )
{
    ਲਈ( ਪੂਰਨ i = 0; i < len; i++, dst += dst_ਕਦਮ )
    {
        T* d = (T*)dst;
        ਲਈ( ਪੂਰਨ k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

ਸਥਿਰ ਖਾਲੀ
Copy_ਮੰਜ਼ਿਲ_Columns( ਅਚਲ uchar* src, uchar* dst, size_t dst_ਕਦਮ, ਪੂਰਨ ncols, ਪੂਰਨ len, size_t elem_ਮਾਪ_2 )
{
    ਜੇ( elem_ਮਾਪ_2 == ਆਕਾਰ(ਪੂਰਨ) )
        copy_ਮੰਜ਼ਿਲ_Columns((ਅਚਲ ਪੂਰਨ*)src, dst, dst_ਕਦਮ, ncols, len);
    ਨਹੀਂਤਾਂ ਜੇ( elem_ਮਾਪ_2 == ਆਕਾਰ(int64) )
        copy_ਮੰਜ਼ਿਲ_Columns((ਅਚਲ int64*)src, dst, dst_ਕਦਮ, ncols, len);
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( elem_ਮਾਪ_2 == ਆਕਾਰ(int64)*2 );
        copy_ਮੰਜ਼ਿਲ_Columns((ਅਚਲ Vec<int64, 2>*)src, dst, dst_ਕਦਮ, ncols, len);
    }
}

ਸਥਿਰ ਖਾਲੀ
ExpandCCS( uchar* _ptr, ਪੂਰਨ n, ਪੂਰਨ elem_ਮਾਪ_2 )
{
    ਪੂਰਨ i;
    ਜੇ( elem_ਮਾਪ_2 == (ਪੂਰਨ)ਆਕਾਰ(ਦਸ਼ਮਲਵ) )
    {
        ਦਸ਼ਮਲਵ* p = (ਦਸ਼ਮਲਵ*)_ptr;
        ਲਈ( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        ਜੇ( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        ਲਈ( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    ਨਹੀਂਤਾਂ
    {
        ਦੁੱਗਣਾ* p = (ਦੁੱਗਣਾ*)_ptr;
        ਲਈ( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        ਜੇ( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        ਲਈ( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਖਾਲੀ (*fn)(ਅਚਲ OcvDftOptions&, ਅਚਲ T*, T*)>
ਸਥਿਰ ਖਾਲੀ dftWrap(ਅਚਲ OcvDftOptions & c, ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ* dst)
{
    fn(c, (ਅਚਲ T*)src, (T*)dst);
}

}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL* Ipp_DFTGet_ਮਾਪ_Func)(ਪੂਰਨ, ਪੂਰਨ, IppHintAlgorithm, ਪੂਰਨ*, ਪੂਰਨ*, ਪੂਰਨ*);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL* IppDFTInitFunc)(ਪੂਰਨ, ਪੂਰਨ, IppHintAlgorithm, ਖਾਲੀ*, uchar*);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SpecType, Ipp_ਸਥਿਤੀ (CV_STDCALL *init_fn)(ਪੂਰਨ, ਪੂਰਨ, IppHintAlgorithm, SpecType*, Ipp8u*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ippDFTInitWrap(ਪੂਰਨ n, ਪੂਰਨ flags, IppHintAlgorithm hint, ਖਾਲੀ* spec, Ipp8u* initbuf)
{
    ਵਾਪਸ init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv
{
#ਜੇ defined USE_IPP_DFT

ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL* ippiDFT_C_Func)(ਅਚਲ Ipp32fc*, ਪੂਰਨ, Ipp32fc*, ਪੂਰਨ, ਅਚਲ IppiDFTSpec_C_32fc*, Ipp8u*);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL* ippiDFT_R_Func)(ਅਚਲ Ipp32f* , ਪੂਰਨ, Ipp32f* , ਪੂਰਨ, ਅਚਲ IppiDFTSpec_R_32f* , Ipp8u*);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Dft>
ਵਰਗ Dft_C_IPPLoop_Invoker : ਜਨਤਕ ParallelLoopBody
{
ਜਨਤਕ:

    Dft_C_IPPLoop_Invoker(ਅਚਲ uchar * _src, size_t src_ਕਦਮ_2, uchar * _dst, size_t dst_ਕਦਮ_2, ਪੂਰਨ ਚੌੜਾਈ_3,
                          ਅਚਲ Dft& _ippidft, ਪੂਰਨ _norm_flag, ਤਾਰਕਿਕ *_ok) :
        ParallelLoopBody(),
        src(_src), src_ਕਦਮ(src_ਕਦਮ_2), dst(_dst), dst_ਕਦਮ(dst_ਕਦਮ_2), ਚੌੜਾਈ(ਚੌੜਾਈ_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = ਸੱਚ;
    }

    ਆਭਾਸੀ ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ Range& range) ਅਚਲ CV_OVERRIDE
    {
        Ipp_ਸਥਿਤੀ ਸਥਿਤੀ;
        Ipp8u* p_ਬਫਰ = 0;
        Ipp8u* pMemInit= 0;
        ਪੂਰਨ ਮਾਪ_ਬਫਰ=0;
        ਪੂਰਨ ਮਾਪ_Spec=0;
        ਪੂਰਨ ਮਾਪ_Init=0;

        Ippi_ਮਾਪ src_Roi_ਮਾਪ = {ਚੌੜਾਈ, 1};

        ਸਥਿਤੀ = ippi_DFTGet_ਮਾਪ_C_32fc(src_Roi_ਮਾਪ, norm_flag, ippAlgHintNone, &ਮਾਪ_Spec, &ਮਾਪ_Init, &ਮਾਪ_ਬਫਰ );
        ਜੇ ( ਸਥਿਤੀ < 0 )
        {
            *ok = ਝੂਠ;
            ਵਾਪਸ;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( ਮਾਪ_Spec );

        ਜੇ ( ਮਾਪ_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( ਮਾਪ_Init );

        ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
            p_ਬਫਰ = (Ipp8u*)CV_IPP_MALLOC( ਮਾਪ_ਬਫਰ );

        ਸਥਿਤੀ = ippiDFTInit_C_32fc( src_Roi_ਮਾਪ, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        ਜੇ ( ਮਾਪ_Init > 0 )
            ippFree( pMemInit );

        ਜੇ ( ਸਥਿਤੀ < 0 )
        {
            ippFree( pDFTSpec );
            ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
                ippFree( p_ਬਫਰ );
            *ok = ਝੂਠ;
            ਵਾਪਸ;
        }

        ਲਈ( ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; ++i)
            ਜੇ(!ippidft((Ipp32fc*)(src + src_ਕਦਮ * i), src_ਕਦਮ, (Ipp32fc*)(dst + dst_ਕਦਮ * i), dst_ਕਦਮ,
                        pDFTSpec, (Ipp8u*)p_ਬਫਰ))
            {
                *ok = ਝੂਠ;
            }

        ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
            ippFree( p_ਬਫਰ );

        ippFree( pDFTSpec );
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP|CV_IMPL_MT);
    }

ਨਿਜੀ:
    ਅਚਲ uchar * src;
    size_t src_ਕਦਮ;
    uchar * dst;
    size_t dst_ਕਦਮ;
    ਪੂਰਨ ਚੌੜਾਈ;
    ਅਚਲ Dft& ippidft;
    ਪੂਰਨ norm_flag;
    ਤਾਰਕਿਕ *ok;

    ਅਚਲ Dft_C_IPPLoop_Invoker& ਸੰਚਾਲਕ= (ਅਚਲ Dft_C_IPPLoop_Invoker&);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Dft>
ਵਰਗ Dft_R_IPPLoop_Invoker : ਜਨਤਕ ParallelLoopBody
{
ਜਨਤਕ:

    Dft_R_IPPLoop_Invoker(ਅਚਲ uchar * _src, size_t src_ਕਦਮ_2, uchar * _dst, size_t dst_ਕਦਮ_2, ਪੂਰਨ ਚੌੜਾਈ_3,
                          ਅਚਲ Dft& _ippidft, ਪੂਰਨ _norm_flag, ਤਾਰਕਿਕ *_ok) :
        ParallelLoopBody(),
        src(_src), src_ਕਦਮ(src_ਕਦਮ_2), dst(_dst), dst_ਕਦਮ(dst_ਕਦਮ_2), ਚੌੜਾਈ(ਚੌੜਾਈ_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = ਸੱਚ;
    }

    ਆਭਾਸੀ ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ Range& range) ਅਚਲ CV_OVERRIDE
    {
        Ipp_ਸਥਿਤੀ ਸਥਿਤੀ;
        Ipp8u* p_ਬਫਰ = 0;
        Ipp8u* pMemInit= 0;
        ਪੂਰਨ ਮਾਪ_ਬਫਰ=0;
        ਪੂਰਨ ਮਾਪ_Spec=0;
        ਪੂਰਨ ਮਾਪ_Init=0;

        Ippi_ਮਾਪ src_Roi_ਮਾਪ = {ਚੌੜਾਈ, 1};

        ਸਥਿਤੀ = ippi_DFTGet_ਮਾਪ_R_32f(src_Roi_ਮਾਪ, norm_flag, ippAlgHintNone, &ਮਾਪ_Spec, &ਮਾਪ_Init, &ਮਾਪ_ਬਫਰ );
        ਜੇ ( ਸਥਿਤੀ < 0 )
        {
            *ok = ਝੂਠ;
            ਵਾਪਸ;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( ਮਾਪ_Spec );

        ਜੇ ( ਮਾਪ_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( ਮਾਪ_Init );

        ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
            p_ਬਫਰ = (Ipp8u*)CV_IPP_MALLOC( ਮਾਪ_ਬਫਰ );

        ਸਥਿਤੀ = ippiDFTInit_R_32f( src_Roi_ਮਾਪ, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        ਜੇ ( ਮਾਪ_Init > 0 )
            ippFree( pMemInit );

        ਜੇ ( ਸਥਿਤੀ < 0 )
        {
            ippFree( pDFTSpec );
            ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
                ippFree( p_ਬਫਰ );
            *ok = ਝੂਠ;
            ਵਾਪਸ;
        }

        ਲਈ( ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; ++i)
            ਜੇ(!ippidft((ਦਸ਼ਮਲਵ*)(src + src_ਕਦਮ * i), src_ਕਦਮ, (ਦਸ਼ਮਲਵ*)(dst + dst_ਕਦਮ * i), dst_ਕਦਮ,
                        pDFTSpec, (Ipp8u*)p_ਬਫਰ))
            {
                *ok = ਝੂਠ;
            }

        ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
            ippFree( p_ਬਫਰ );

        ippFree( pDFTSpec );
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP|CV_IMPL_MT);
    }

ਨਿਜੀ:
    ਅਚਲ uchar * src;
    size_t src_ਕਦਮ;
    uchar * dst;
    size_t dst_ਕਦਮ;
    ਪੂਰਨ ਚੌੜਾਈ;
    ਅਚਲ Dft& ippidft;
    ਪੂਰਨ norm_flag;
    ਤਾਰਕਿਕ *ok;

    ਅਚਲ Dft_R_IPPLoop_Invoker& ਸੰਚਾਲਕ= (ਅਚਲ Dft_R_IPPLoop_Invoker&);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Dft>
ਤਾਰਕਿਕ Dft_C_IPPLoop(ਅਚਲ uchar * src, size_t src_ਕਦਮ, uchar * dst, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ Dft& ippidft, ਪੂਰਨ norm_flag)
{
    ਤਾਰਕਿਕ ok;
    parallel_for_(Range(0, ਉਚਾਈ), Dft_C_IPPLoop_Invoker<Dft>(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, ippidft, norm_flag, &ok), (ਚੌੜਾਈ * ਉਚਾਈ)/(ਦੁੱਗਣਾ)(1<<16) );
    ਵਾਪਸ ok;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Dft>
ਤਾਰਕਿਕ Dft_R_IPPLoop(ਅਚਲ uchar * src, size_t src_ਕਦਮ, uchar * dst, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ Dft& ippidft, ਪੂਰਨ norm_flag)
{
    ਤਾਰਕਿਕ ok;
    parallel_for_(Range(0, ਉਚਾਈ), Dft_R_IPPLoop_Invoker<Dft>(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, ippidft, norm_flag, &ok), (ਚੌੜਾਈ * ਉਚਾਈ)/(ਦੁੱਗਣਾ)(1<<16) );
    ਵਾਪਸ ok;
}

ਬਣਤਰ IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ()(ਅਚਲ Ipp32fc* src, size_t src_ਕਦਮ_3, Ipp32fc* dst, size_t dst_ਕਦਮ_3, ਅਚਲ IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* p_ਬਫਰ) ਅਚਲ
    {
        ਵਾਪਸ ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<ਪੂਰਨ>(src_ਕਦਮ_3), dst, static_cast<ਪੂਰਨ>(dst_ਕਦਮ_3), pDFTSpec, p_ਬਫਰ) >= 0 : ਝੂਠ;
    }
ਨਿਜੀ:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

ਬਣਤਰ IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippi_DFT_Pack_ਮੰਜ਼ਿਲ_R_32f_C1_R(_func){}

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ()(ਅਚਲ Ipp32f* src, size_t src_ਕਦਮ_3, Ipp32f* dst, size_t dst_ਕਦਮ_3, ਅਚਲ IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* p_ਬਫਰ) ਅਚਲ
    {
        ਵਾਪਸ ippi_DFT_Pack_ਮੰਜ਼ਿਲ_R_32f_C1_R ? CV_INSTRUMENT_FUN_IPP(ippi_DFT_Pack_ਮੰਜ਼ਿਲ_R_32f_C1_R, src, static_cast<ਪੂਰਨ>(src_ਕਦਮ_3), dst, static_cast<ਪੂਰਨ>(dst_ਕਦਮ_3), pDFTSpec, p_ਬਫਰ) >= 0 : ਝੂਠ;
    }
ਨਿਜੀ:
    ippiDFT_R_Func ippi_DFT_Pack_ਮੰਜ਼ਿਲ_R_32f_C1_R;
};

ਸਥਿਰ ਤਾਰਕਿਕ ippi_DFT_C_32F(ਅਚਲ uchar * src, size_t src_ਕਦਮ, uchar * dst, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਤਾਰਕਿਕ inv, ਪੂਰਨ norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_ਸਥਿਤੀ ਸਥਿਤੀ;
    Ipp8u* p_ਬਫਰ = 0;
    Ipp8u* pMemInit= 0;
    ਪੂਰਨ ਮਾਪ_ਬਫਰ=0;
    ਪੂਰਨ ਮਾਪ_Spec=0;
    ਪੂਰਨ ਮਾਪ_Init=0;

    Ippi_ਮਾਪ src_Roi_ਮਾਪ = {ਚੌੜਾਈ, ਉਚਾਈ};

    ਸਥਿਤੀ = ippi_DFTGet_ਮਾਪ_C_32fc(src_Roi_ਮਾਪ, norm_flag, ippAlgHintNone, &ਮਾਪ_Spec, &ਮਾਪ_Init, &ਮਾਪ_ਬਫਰ );
    ਜੇ ( ਸਥਿਤੀ < 0 )
        ਵਾਪਸ ਝੂਠ;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( ਮਾਪ_Spec );

    ਜੇ ( ਮਾਪ_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( ਮਾਪ_Init );

    ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
        p_ਬਫਰ = (Ipp8u*)CV_IPP_MALLOC( ਮਾਪ_ਬਫਰ );

    ਸਥਿਤੀ = ippiDFTInit_C_32fc( src_Roi_ਮਾਪ, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    ਜੇ ( ਮਾਪ_Init > 0 )
        ippFree( pMemInit );

    ਜੇ ( ਸਥਿਤੀ < 0 )
    {
        ippFree( pDFTSpec );
        ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
            ippFree( p_ਬਫਰ );
        ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (!inv)
        ਸਥਿਤੀ = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<ਪੂਰਨ>(src_ਕਦਮ), (Ipp32fc*)dst, static_cast<ਪੂਰਨ>(dst_ਕਦਮ), pDFTSpec, p_ਬਫਰ);
    ਨਹੀਂਤਾਂ
        ਸਥਿਤੀ = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<ਪੂਰਨ>(src_ਕਦਮ), (Ipp32fc*)dst, static_cast<ਪੂਰਨ>(dst_ਕਦਮ), pDFTSpec, p_ਬਫਰ);

    ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
        ippFree( p_ਬਫਰ );

    ippFree( pDFTSpec );

    ਜੇ(ਸਥਿਤੀ >= 0)
    {
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
        ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਤਾਰਕਿਕ ippi_DFT_R_32F(ਅਚਲ uchar * src, size_t src_ਕਦਮ, uchar * dst, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਤਾਰਕਿਕ inv, ਪੂਰਨ norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_ਸਥਿਤੀ ਸਥਿਤੀ;
    Ipp8u* p_ਬਫਰ = 0;
    Ipp8u* pMemInit= 0;
    ਪੂਰਨ ਮਾਪ_ਬਫਰ=0;
    ਪੂਰਨ ਮਾਪ_Spec=0;
    ਪੂਰਨ ਮਾਪ_Init=0;

    Ippi_ਮਾਪ src_Roi_ਮਾਪ = {ਚੌੜਾਈ, ਉਚਾਈ};

    ਸਥਿਤੀ = ippi_DFTGet_ਮਾਪ_R_32f(src_Roi_ਮਾਪ, norm_flag, ippAlgHintNone, &ਮਾਪ_Spec, &ਮਾਪ_Init, &ਮਾਪ_ਬਫਰ );
    ਜੇ ( ਸਥਿਤੀ < 0 )
        ਵਾਪਸ ਝੂਠ;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( ਮਾਪ_Spec );

    ਜੇ ( ਮਾਪ_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( ਮਾਪ_Init );

    ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
        p_ਬਫਰ = (Ipp8u*)CV_IPP_MALLOC( ਮਾਪ_ਬਫਰ );

    ਸਥਿਤੀ = ippiDFTInit_R_32f( src_Roi_ਮਾਪ, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    ਜੇ ( ਮਾਪ_Init > 0 )
        ippFree( pMemInit );

    ਜੇ ( ਸਥਿਤੀ < 0 )
    {
        ippFree( pDFTSpec );
        ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
            ippFree( p_ਬਫਰ );
        ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (!inv)
        ਸਥਿਤੀ = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (ਦਸ਼ਮਲਵ*)src, static_cast<ਪੂਰਨ>(src_ਕਦਮ), (ਦਸ਼ਮਲਵ*)dst, static_cast<ਪੂਰਨ>(dst_ਕਦਮ), pDFTSpec, p_ਬਫਰ);
    ਨਹੀਂਤਾਂ
        ਸਥਿਤੀ = CV_INSTRUMENT_FUN_IPP(ippi_DFTInv_Pack_ਮੰਜ਼ਿਲ_R_32f_C1_R, (ਦਸ਼ਮਲਵ*)src, static_cast<ਪੂਰਨ>(src_ਕਦਮ), (ਦਸ਼ਮਲਵ*)dst, static_cast<ਪੂਰਨ>(dst_ਕਦਮ), pDFTSpec, p_ਬਫਰ);

    ਜੇ ( ਮਾਪ_ਬਫਰ > 0 )
        ippFree( p_ਬਫਰ );

    ippFree( pDFTSpec );

    ਜੇ(ਸਥਿਤੀ >= 0)
    {
        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
        ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਨਾਮਾਕਾਸ਼ cv
{

ਗਿਣਤੀ FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

ਬਣਤਰ OCL_FftPlan
{
ਨਿਜੀ:
    UMat twiddles;
    String buildOptions;
    ਪੂਰਨ thread_ਗਿਣਤੀ;
    ਪੂਰਨ dft_ਮਾਪ;
    ਪੂਰਨ dft_ਡੂੰਘਾਈ;
    ਤਾਰਕਿਕ ਸਥਿਤੀ;

ਜਨਤਕ:
    OCL_FftPlan(ਪੂਰਨ ਮਾਪ_4, ਪੂਰਨ ਡੂੰਘਾਈ_2) : dft_ਮਾਪ(ਮਾਪ_4), dft_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ_2), ਸਥਿਤੀ(ਸੱਚ)
    {
        CV_Assert( dft_ਡੂੰਘਾਈ == CV_32F || dft_ਡੂੰਘਾਈ == CV_64F );

        ਪੂਰਨ min_radix;
        std::vector<ਪੂਰਨ> radixes, ਖੰਡਾਂ;
        ocl_getRadixes(dft_ਮਾਪ, radixes, ਖੰਡਾਂ, min_radix);
        thread_ਗਿਣਤੀ = dft_ਮਾਪ / min_radix;

        ਜੇ (thread_ਗਿਣਤੀ > (ਪੂਰਨ) ocl::Device::getDefault().max_Work_ਸਮੂਹ_ਮਾਪ())
        {
            ਸਥਿਤੀ = ਝੂਠ;
            ਵਾਪਸ;
        }

        // generate string with radix calls
        String radix_processing;
        ਪੂਰਨ n = 1, twiddle_ਮਾਪ = 0;
        ਲਈ (size_t i=0; i<radixes.size(); i++)
        {
            ਪੂਰਨ radix = radixes[i], ਖੰਡ_2 = ਖੰਡਾਂ[i];
            ਜੇ (ਖੰਡ_2 > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, ਖੰਡ_2, twiddle_ਮਾਪ, n, dft_ਮਾਪ/radix);
            ਨਹੀਂਤਾਂ
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_ਮਾਪ, n, dft_ਮਾਪ/radix);
            twiddle_ਮਾਪ += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_ਮਾਪ, CV_MAKE_TYPE(dft_ਡੂੰਘਾਈ, 2));
        ਜੇ (dft_ਡੂੰਘਾਈ == CV_32F)
            fill_Radix_ਸਾਰਣੀ<ਦਸ਼ਮਲਵ>(twiddles, radixes);
        ਨਹੀਂਤਾਂ
            fill_Radix_ਸਾਰਣੀ<ਦੁੱਗਣਾ>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_ਮਾਪ, min_radix, ocl::type_ਮੰਜ਼ਿਲ_Str(dft_ਡੂੰਘਾਈ), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(dft_ਡੂੰਘਾਈ, 2)),
                              dft_ਡੂੰਘਾਈ == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    ਤਾਰਕਿਕ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Transform(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ ਅੰਕ_dfts, ਪੂਰਨ flags, ਪੂਰਨ fftType, ਤਾਰਕਿਕ ਪੰਕਤੀਆਂ = ਸੱਚ) ਅਚਲ
    {
        ਜੇ (!ਸਥਿਤੀ)
            ਵਾਪਸ ਝੂਠ;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        ਤਾਰਕਿਕ is1d = (flags & DFT_ਪੰਕਤੀਆਂ) != 0 || ਅੰਕ_dfts == 1;
        ਤਾਰਕਿਕ inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        ਜੇ (ਪੰਕਤੀਆਂ)
        {
            globalsize[0] = thread_ਗਿਣਤੀ; globalsize[1] = src.ਪੰਕਤੀਆਂ;
            localsize[0] = thread_ਗਿਣਤੀ; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            ਜੇ ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        ਨਹੀਂਤਾਂ
        {
            globalsize[0] = ਅੰਕ_dfts; globalsize[1] = thread_ਗਿਣਤੀ;
            localsize[0] = 1; localsize[1] = thread_ਗਿਣਤੀ;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            ਜੇ (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        ਜੇ (!inv)
        {
            ਜੇ ((is1d && src.channels() == 1) || (ਪੰਕਤੀਆਂ && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (ਪੰਕਤੀਆਂ && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            ਜੇ (dst.ਖਾਨੇ_ਗਿਣਤੀ % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        ਜੇ (k.empty())
            ਵਾਪਸ ਝੂਠ;

        k.ਦਲੀਲਾਂ(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::Read_Only_No_ਮਾਪ(twiddles), thread_ਗਿਣਤੀ, ਅੰਕ_dfts);
        ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, localsize, ਝੂਠ);
    }

ਨਿਜੀ:
    ਸਥਿਰ ਖਾਲੀ ocl_getRadixes(ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, std::vector<ਪੂਰਨ>& radixes, std::vector<ਪੂਰਨ>& ਖੰਡਾਂ, ਪੂਰਨ& min_radix)
    {
        ਪੂਰਨ factors[34];
        ਪੂਰਨ nf = DFTFactorize(ਖਾਨੇ_ਗਿਣਤੀ, factors);

        ਪੂਰਨ n = 1;
        ਪੂਰਨ ਗੁਣਕ_ਸੂਚਕਾਂਕ = 0;
        min_radix = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;

        // 2^n transforms
        ਜੇ ((factors[ਗੁਣਕ_ਸੂਚਕਾਂਕ] & 1) == 0)
        {
            ਲਈ( ; n < factors[ਗੁਣਕ_ਸੂਚਕਾਂਕ];)
            {
                ਪੂਰਨ radix = 2, ਖੰਡ_2 = 1;
                ਜੇ (8*n <= factors[0])
                    radix = 8;
                ਨਹੀਂਤਾਂ ਜੇ (4*n <= factors[0])
                {
                    radix = 4;
                    ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 12 == 0)
                        ਖੰਡ_2 = 3;
                    ਨਹੀਂਤਾਂ ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 8 == 0)
                        ਖੰਡ_2 = 2;
                }
                ਨਹੀਂਤਾਂ
                {
                    ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 10 == 0)
                        ਖੰਡ_2 = 5;
                    ਨਹੀਂਤਾਂ ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 8 == 0)
                        ਖੰਡ_2 = 4;
                    ਨਹੀਂਤਾਂ ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 6 == 0)
                        ਖੰਡ_2 = 3;
                    ਨਹੀਂਤਾਂ ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 4 == 0)
                        ਖੰਡ_2 = 2;
                }

                radixes.push_back(radix);
                ਖੰਡਾਂ.push_back(ਖੰਡ_2);
                min_radix = min(min_radix, ਖੰਡ_2*radix);
                n *= radix;
            }
            ਗੁਣਕ_ਸੂਚਕਾਂਕ++;
        }

        // all the other transforms
        ਲਈ( ; ਗੁਣਕ_ਸੂਚਕਾਂਕ < nf; ਗੁਣਕ_ਸੂਚਕਾਂਕ++)
        {
            ਪੂਰਨ radix = factors[ਗੁਣਕ_ਸੂਚਕਾਂਕ], ਖੰਡ_2 = 1;
            ਜੇ (radix == 3)
            {
                ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 12 == 0)
                    ਖੰਡ_2 = 4;
                ਨਹੀਂਤਾਂ ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 9 == 0)
                    ਖੰਡ_2 = 3;
                ਨਹੀਂਤਾਂ ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 6 == 0)
                    ਖੰਡ_2 = 2;
            }
            ਨਹੀਂਤਾਂ ਜੇ (radix == 5)
            {
                ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ % 10 == 0)
                    ਖੰਡ_2 = 2;
            }
            radixes.push_back(radix);
            ਖੰਡਾਂ.push_back(ਖੰਡ_2);
            min_radix = min(min_radix, ਖੰਡ_2*radix);
        }
    }

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
    ਸਥਿਰ ਖਾਲੀ fill_Radix_ਸਾਰਣੀ(UMat twiddles, ਅਚਲ std::vector<ਪੂਰਨ>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        ਪੂਰਨ ptr_ਸੂਚਕਾਂਕ = 0;

        ਪੂਰਨ n = 1;
        ਲਈ (size_t i=0; i<radixes.size(); i++)
        {
            ਪੂਰਨ radix = radixes[i];
            n *= radix;

            ਲਈ (ਪੂਰਨ j=1; j<radix; j++)
            {
                ਦੁੱਗਣਾ theta = -CV_2PI*j/n;

                ਲਈ (ਪੂਰਨ k=0; k<(n/radix); k++)
                {
                    ptr[ptr_ਸੂਚਕਾਂਕ++] = (T) ਕੋਜੀਵਾ(k*theta);
                    ptr[ptr_ਸੂਚਕਾਂਕ++] = (T) ਜੀਵਾ(k*theta);
                }
            }
        }
    }
};

ਵਰਗ OCL_FftPlanCache
{
ਜਨਤਕ:
    ਸਥਿਰ OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, ਨਵਾਂ OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(ਪੂਰਨ dft_ਮਾਪ, ਪੂਰਨ ਡੂੰਘਾਈ)
    {
        ਪੂਰਨ ਕੁੰਜੀ = (dft_ਮਾਪ << 16) | (ਡੂੰਘਾਈ & 0xFFFF);
        std::map<ਪੂਰਨ, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(ਕੁੰਜੀ);
        ਜੇ (f != planStorage.end())
        {
            ਵਾਪਸ f->second;
        }
        ਨਹੀਂਤਾਂ
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(ਨਵਾਂ OCL_FftPlan(dft_ਮਾਪ, ਡੂੰਘਾਈ));
            planStorage[ਕੁੰਜੀ] = newPlan;
            ਵਾਪਸ newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

ਸੁਰੱਖਿਅਤ:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<ਪੂਰਨ, Ptr<OCL_FftPlan> > planStorage;
};

ਸਥਿਰ ਤਾਰਕਿਕ ocl_dft_ਪੰਕਤੀਆਂ(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ, ਪੂਰਨ flags, ਪੂਰਨ fftType)
{
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.ਖਾਨੇ_ਗਿਣਤੀ(), ਡੂੰਘਾਈ);
    ਵਾਪਸ plan->ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Transform(_src, _dst, nonzero_ਪੰਕਤੀਆਂ, flags, fftType, ਸੱਚ);
}

ਸਥਿਰ ਤਾਰਕਿਕ ocl_dft_ਖਾਨੇ_ਗਿਣਤੀ(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ nonzero_ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ flags, ਪੂਰਨ fftType)
{
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.ਪੰਕਤੀਆਂ(), ਡੂੰਘਾਈ);
    ਵਾਪਸ plan->ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Transform(_src, _dst, nonzero_ਖਾਨੇ_ਗਿਣਤੀ, flags, fftType, ਝੂਠ);
}

ਸਤਰਅੰਦਰ FftType determineFFTType(ਤਾਰਕਿਕ real_ਨਿਵੇਸ਼, ਤਾਰਕਿਕ complex_ਨਿਵੇਸ਼, ਤਾਰਕਿਕ real_output, ਤਾਰਕਿਕ complex_output, ਤਾਰਕਿਕ inv)
{
    // output format is not specified
    ਜੇ (!real_output && !complex_output)
        complex_output = ਸੱਚ;

    // input or output format is ambiguous
    ਜੇ (real_ਨਿਵੇਸ਼ == complex_ਨਿਵੇਸ਼ || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType ਨਤੀਜਾ = real_ਨਿਵੇਸ਼ ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    ਜੇ (ਨਤੀਜਾ == C2R && !inv)
        ਨਤੀਜਾ = C2C;

    // Inverse CCS to Complex not supported
    ਜੇ (ਨਤੀਜਾ == R2C && inv)
        ਨਤੀਜਾ = R2R;

    ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਤਾਰਕਿਕ ocl_dft(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ flags, ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ)
{
    ਪੂਰਨ type = _src.type(), cn = CV_MAT_CN(type), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਮਾਪ ssize = _src.size();
    ਤਾਰਕਿਕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ਜੇ (!(cn == 1 || cn == 2)
        || !(ਡੂੰਘਾਈ == CV_32F || (ਡੂੰਘਾਈ == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        ਵਾਪਸ ਝੂਠ;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    ਜੇ (ssize.area() != getOptimalDFTSize(ssize.area()))
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat();
    ਤਾਰਕਿਕ inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    ਜੇ( nonzero_ਪੰਕਤੀਆਂ <= 0 || nonzero_ਪੰਕਤੀਆਂ > _src.ਪੰਕਤੀਆਂ() )
        nonzero_ਪੰਕਤੀਆਂ = _src.ਪੰਕਤੀਆਂ();
    ਤਾਰਕਿਕ is1d = (flags & DFT_ਪੰਕਤੀਆਂ) != 0 || nonzero_ਪੰਕਤੀਆਂ == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    ਜੇ (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.create_Same_ਮਾਪ(src, CV_MAKETYPE(ਡੂੰਘਾਈ, 2));
        output = _dst.getUMat();
    }
    ਨਹੀਂਤਾਂ
    {
        // real output
        ਜੇ (is1d)
        {
            _dst.create_Same_ਮਾਪ(src, CV_MAKETYPE(ਡੂੰਘਾਈ, 1));
            output = _dst.getUMat();
        }
        ਨਹੀਂਤਾਂ
        {
            _dst.create_Same_ਮਾਪ(src, CV_MAKETYPE(ਡੂੰਘਾਈ, 1));
            output.create(src.size, CV_MAKETYPE(ਡੂੰਘਾਈ, 2));
        }
    }

    ਤਾਰਕਿਕ ਨਤੀਜਾ = ਝੂਠ;
    ਜੇ (!inv)
    {
        ਪੂਰਨ nonzero_ਖਾਨੇ_ਗਿਣਤੀ = fftType == R2R ? output.ਖਾਨੇ_ਗਿਣਤੀ/2 + 1 : output.ਖਾਨੇ_ਗਿਣਤੀ;
        ਨਤੀਜਾ = ocl_dft_ਪੰਕਤੀਆਂ(src, output, nonzero_ਪੰਕਤੀਆਂ, flags, fftType);
        ਜੇ (!is1d)
            ਨਤੀਜਾ = ਨਤੀਜਾ && ocl_dft_ਖਾਨੇ_ਗਿਣਤੀ(output, _dst, nonzero_ਖਾਨੇ_ਗਿਣਤੀ, flags, fftType);
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (fftType == C2C)
        {
            // complex output
            ਨਤੀਜਾ = ocl_dft_ਪੰਕਤੀਆਂ(src, output, nonzero_ਪੰਕਤੀਆਂ, flags, fftType);
            ਜੇ (!is1d)
                ਨਤੀਜਾ = ਨਤੀਜਾ && ocl_dft_ਖਾਨੇ_ਗਿਣਤੀ(output, output, output.ਖਾਨੇ_ਗਿਣਤੀ, flags, fftType);
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (is1d)
            {
                ਨਤੀਜਾ = ocl_dft_ਪੰਕਤੀਆਂ(src, output, nonzero_ਪੰਕਤੀਆਂ, flags, fftType);
            }
            ਨਹੀਂਤਾਂ
            {
                ਪੂਰਨ nonzero_ਖਾਨੇ_ਗਿਣਤੀ = src.ਖਾਨੇ_ਗਿਣਤੀ/2 + 1;
                ਨਤੀਜਾ = ocl_dft_ਖਾਨੇ_ਗਿਣਤੀ(src, output, nonzero_ਖਾਨੇ_ਗਿਣਤੀ, flags, fftType);
                ਨਤੀਜਾ = ਨਤੀਜਾ && ocl_dft_ਪੰਕਤੀਆਂ(output, _dst, nonzero_ਪੰਕਤੀਆਂ, flags, fftType);
            }
        }
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

} // namespace cv;

#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CLAMDFFT

ਨਾਮਾਕਾਸ਼ cv {

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CLAMDDFT_Assert(func) \
    { \
        clfft_ਸਥਿਤੀ s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

ਵਰਗ PlanCache
{
    ਬਣਤਰ FftPlan
    {
        FftPlan(ਅਚਲ ਮਾਪ & dft_ਮਾਪ_2, ਪੂਰਨ src_ਕਦਮ_2, ਪੂਰਨ dst_ਕਦਮ_2, ਤਾਰਕਿਕ _doubleFP, ਤਾਰਕਿਕ _inplace, ਪੂਰਨ _flags, FftType _fftType) :
            dft_ਮਾਪ(dft_ਮਾਪ_2), src_ਕਦਮ(src_ਕਦਮ_2), dst_ਕਦਮ(dst_ਕਦਮ_2),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            ਤਾਰਕਿਕ dft_inverse = (flags & DFT_INVERSE) != 0;
            ਤਾਰਕਿਕ dft_scale = (flags & DFT_SCALE) != 0;
            ਤਾਰਕਿਕ dft_ਪੰਕਤੀਆਂ = (flags & DFT_ਪੰਕਤੀਆਂ) != 0;

            clfftLayout ਵਿੱਚ_Layout = CLFFT_REAL, ਬਾਹਰਲਾ_Layout = CLFFT_REAL;
            clfftDim dim = dft_ਮਾਪ.ਉਚਾਈ == 1 || dft_ਪੰਕਤੀਆਂ ? CLFFT_1D : CLFFT_2D;

            size_t batch_ਮਾਪ = dft_ਪੰਕਤੀਆਂ ? dft_ਮਾਪ.ਉਚਾਈ : 1;
            size_t cl_Lengths_ਵਿੱਚ[3] = { (size_t)dft_ਮਾਪ.ਚੌੜਾਈ, dft_ਪੰਕਤੀਆਂ ? 1 : (size_t)dft_ਮਾਪ.ਉਚਾਈ, 1 };
            size_t cl_Strides_ਵਿੱਚ[3] = { 1, 1, 1 };
            size_t cl_Strides_ਬਾਹਰਲਾ[3]  = { 1, 1, 1 };
            ਪੂਰਨ elem_ਮਾਪ = doubleFP ? ਆਕਾਰ(ਦੁੱਗਣਾ) : ਆਕਾਰ(ਦਸ਼ਮਲਵ);

            ਚੋਣ (fftType)
            {
            ਹਾਲਤ C2C:
                ਵਿੱਚ_Layout = CLFFT_COMPLEX_INTERLEAVED;
                ਬਾਹਰਲਾ_Layout = CLFFT_COMPLEX_INTERLEAVED;
                cl_Strides_ਵਿੱਚ[1] = src_ਕਦਮ / (elem_ਮਾਪ << 1);
                cl_Strides_ਬਾਹਰਲਾ[1] = dst_ਕਦਮ / (elem_ਮਾਪ << 1);
                ਰੋਕੋ;
            ਹਾਲਤ R2C:
                ਵਿੱਚ_Layout = CLFFT_REAL;
                ਬਾਹਰਲਾ_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                cl_Strides_ਵਿੱਚ[1] = src_ਕਦਮ / elem_ਮਾਪ;
                cl_Strides_ਬਾਹਰਲਾ[1] = dst_ਕਦਮ / (elem_ਮਾਪ << 1);
                ਰੋਕੋ;
            ਹਾਲਤ C2R:
                ਵਿੱਚ_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                ਬਾਹਰਲਾ_Layout = CLFFT_REAL;
                cl_Strides_ਵਿੱਚ[1] = src_ਕਦਮ / (elem_ਮਾਪ << 1);
                cl_Strides_ਬਾਹਰਲਾ[1] = dst_ਕਦਮ / elem_ਮਾਪ;
                ਰੋਕੋ;
            ਹਾਲਤ R2R:
            ਮੂਲ:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                ਰੋਕੋ;
            }

            cl_Strides_ਵਿੱਚ[2] = dft_ਪੰਕਤੀਆਂ ? cl_Strides_ਵਿੱਚ[1] : dft_ਮਾਪ.ਚੌੜਾਈ * cl_Strides_ਵਿੱਚ[1];
            cl_Strides_ਬਾਹਰਲਾ[2] = dft_ਪੰਕਤੀਆਂ ? cl_Strides_ਬਾਹਰਲਾ[1] : dft_ਮਾਪ.ਚੌੜਾਈ * cl_Strides_ਬਾਹਰਲਾ[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, cl_Lengths_ਵਿੱਚ))

            // setting plan properties
            CLAMDDFT_Assert(clfft_ਸੈੱਟ_Plan_Precision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfft_ਸੈੱਟ_ਨਤੀਜਾ_Location(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfft_ਸੈੱਟ_Layout(plHandle, ਵਿੱਚ_Layout, ਬਾਹਰਲਾ_Layout))
            CLAMDDFT_Assert(clfft_ਸੈੱਟ_Plan_Batch_ਮਾਪ(plHandle, batch_ਮਾਪ))
            CLAMDDFT_Assert(clfft_ਸੈੱਟ_Plan_ਵਿੱਚ_Stride(plHandle, dim, cl_Strides_ਵਿੱਚ))
            CLAMDDFT_Assert(clfft_ਸੈੱਟ_Plan_ਬਾਹਰਲਾ_Stride(plHandle, dim, cl_Strides_ਬਾਹਰਲਾ))
            CLAMDDFT_Assert(clfft_ਸੈੱਟ_Plan_ਦੂਰੀ(plHandle, cl_Strides_ਵਿੱਚ[dim], cl_Strides_ਬਾਹਰਲਾ[dim]))

            ਦਸ਼ਮਲਵ scale = dft_scale ? 1.0f / (dft_ਪੰਕਤੀਆਂ ? dft_ਮਾਪ.ਚੌੜਾਈ : dft_ਮਾਪ.area()) : 1.0f;
            CLAMDDFT_Assert(clfft_ਸੈੱਟ_Plan_Scale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_ਕਤਾਰ ਕਤਾਰ_2 = (cl_command_ਕਤਾਰ)ocl::ਕਤਾਰ::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &ਕਤਾਰ_2, ਨਿਰਾ, ਨਿਰਾ))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        ਮਿੱਤਰ ਵਰਗ PlanCache;

    ਨਿਜੀ:
        ਮਾਪ dft_ਮਾਪ;
        ਪੂਰਨ src_ਕਦਮ, dst_ਕਦਮ;
        ਤਾਰਕਿਕ doubleFP;
        ਤਾਰਕਿਕ inplace;
        ਪੂਰਨ flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

ਜਨਤਕ:
    ਸਥਿਰ PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, ਨਵਾਂ PlanCache())
    }

    clfftPlanHandle getPlanHandle(ਅਚਲ ਮਾਪ & dft_ਮਾਪ, ਪੂਰਨ src_ਕਦਮ, ਪੂਰਨ dst_ਕਦਮ, ਤਾਰਕਿਕ doubleFP,
                                  ਤਾਰਕਿਕ inplace, ਪੂਰਨ flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        ਲਈ (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            ਅਚਲ FftPlan * ਅਚਲ plan = planStorage[i];

            ਜੇ (plan->dft_ਮਾਪ == dft_ਮਾਪ &&
                plan->flags == flags &&
                plan->src_ਕਦਮ == src_ਕਦਮ &&
                plan->dst_ਕਦਮ == dst_ਕਦਮ &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                ਜੇ (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    ਰੋਕੋ;
                }

                ਵਾਪਸ plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(ਨਵਾਂ FftPlan(dft_ਮਾਪ, src_ਕਦਮ, dst_ਕਦਮ, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        ਵਾਪਸ newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

ਸੁਰੱਖਿਅਤ:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

ਬਾਹਰੀ "C" {

ਸਥਿਰ ਖਾਲੀ CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, ਖਾਲੀ *p)
{
    UMat_ਅੰਕੜੇ * u = (UMat_ਅੰਕੜੇ *)p;

    ਜੇ( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

ਸਥਿਰ ਤਾਰਕਿਕ ocl_dft_amdfft(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ flags)
{
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type);
    ਮਾਪ ssize = _src.size();

    ਤਾਰਕਿਕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    ਜੇ ( (!doubleSupport && ਡੂੰਘਾਈ == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        ਵਾਪਸ ਝੂਠ;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    ਜੇ (ssize.area() != getOptimalDFTSize(ssize.area()))
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ dst_complex_ਨਿਵੇਸ਼ = cn == 2 ? 1 : 0;
    ਤਾਰਕਿਕ dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    ਪੂਰਨ dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    ਤਾਰਕਿਕ dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_ਨਿਵੇਸ਼ << 0 | dft_complex_output << 1);

    ਚੋਣ (fftType)
    {
    ਹਾਲਤ C2C:
        _dst.create(ssize.ਉਚਾਈ, ssize.ਚੌੜਾਈ, CV_MAKE_TYPE(ਡੂੰਘਾਈ, 2));
        ਰੋਕੋ;
    ਹਾਲਤ R2C: // TODO implement it if possible
    ਹਾਲਤ C2R: // TODO implement it if possible
    ਹਾਲਤ R2R: // AMD Fft does not support this type
    ਮੂਲ:
        ਵਾਪਸ ਝੂਠ;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    ਜੇ (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        ਵਾਪਸ ਝੂਠ;
    ਤਾਰਕਿਕ inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (ਪੂਰਨ)src.ਕਦਮ, (ਪੂਰਨ)dst.ਕਦਮ,
                          ਡੂੰਘਾਈ == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t ਬਫਰ_ਮਾਪ = 0;
    CLAMDDFT_Assert(clfft_Get_Tmp_Buf_ਮਾਪ(plHandle, &ਬਫਰ_ਮਾਪ))
    UMat tmp_ਬਫਰ(1, (ਪੂਰਨ)ਬਫਰ_ਮਾਪ, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_ਕਤਾਰ ਕਤਾਰ_2 = (cl_command_ਕਤਾਰ)ocl::ਕਤਾਰ::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfft_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Transform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &ਕਤਾਰ_2, 0, ਨਿਰਾ, &e,
                                          &srcarg, &dstarg, (cl_mem)tmp_ਬਫਰ.handle(ACCESS_RW)))

    tmp_ਬਫਰ.addref();
    cl_ਸੈੱਟ_Event_Callback(e, CL_COMPLETE, oclCleanupCallback, tmp_ਬਫਰ.u);
    ਵਾਪਸ ਸੱਚ;
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_ASSERT

}

#ਜੇ_ਅੰਤ // HAVE_CLAMDFFT

ਨਾਮਾਕਾਸ਼ cv
{

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਖਾਲੀ complementComplex(T * ptr, size_t ਕਦਮ, ਪੂਰਨ n, ਪੂਰਨ len, ਪੂਰਨ dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = ਕਦਮ/ਆਕਾਰ(p0[0]);
    ਲਈ(ਪੂਰਨ i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        ਲਈ( ਪੂਰਨ j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

ਸਥਿਰ ਖਾਲੀ complementComplexOutput(ਪੂਰਨ ਡੂੰਘਾਈ, uchar * ptr, size_t ਕਦਮ, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ len, ਪੂਰਨ dft_dims)
{
    ਜੇ( ਡੂੰਘਾਈ == CV_32F )
        complementComplex((ਦਸ਼ਮਲਵ*)ptr, ਕਦਮ, ਗਿਣਤੀ_3, len, dft_dims);
    ਨਹੀਂਤਾਂ
        complementComplex((ਦੁੱਗਣਾ*)ptr, ਕਦਮ, ਗਿਣਤੀ_3, len, dft_dims);
}

ਗਿਣਤੀ Dft_ਬਹੁਲਕ {
    InvalidDft = 0,
    Fwd_Real_ਮੰਜ਼ਿਲ_CCS,
    Fwd_Real_ਮੰਜ਼ਿਲ_Complex,
    Fwd_Complex_ਮੰਜ਼ਿਲ_Complex,
    InvCCSToReal,
    Inv_Complex_ਮੰਜ਼ਿਲ_Real,
    Inv_Complex_ਮੰਜ਼ਿਲ_Complex,
};

ਗਿਣਤੀ DftDims {
    InvalidDim = 0,
    OneDim,
    One_Dim_ਖਾਨਾ_Wise,
    TwoDims
};

ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ * ਬਹੁਲਕ_Name(Dft_ਬਹੁਲਕ m)
{
    ਚੋਣ (m)
    {
    ਹਾਲਤ InvalidDft: ਵਾਪਸ "InvalidDft";
    ਹਾਲਤ Fwd_Real_ਮੰਜ਼ਿਲ_CCS: ਵਾਪਸ "FwdRealToCCS";
    ਹਾਲਤ Fwd_Real_ਮੰਜ਼ਿਲ_Complex: ਵਾਪਸ "FwdRealToComplex";
    ਹਾਲਤ Fwd_Complex_ਮੰਜ਼ਿਲ_Complex: ਵਾਪਸ "FwdComplexToComplex";
    ਹਾਲਤ InvCCSToReal: ਵਾਪਸ "InvCCSToReal";
    ਹਾਲਤ Inv_Complex_ਮੰਜ਼ਿਲ_Real: ਵਾਪਸ "InvComplexToReal";
    ਹਾਲਤ Inv_Complex_ਮੰਜ਼ਿਲ_Complex: ਵਾਪਸ "InvComplexToComplex";
    }
    ਵਾਪਸ 0;
}

ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ * dimsName(DftDims d)
{
    ਚੋਣ (d)
    {
    ਹਾਲਤ InvalidDim: ਵਾਪਸ "InvalidDim";
    ਹਾਲਤ OneDim: ਵਾਪਸ "OneDim";
    ਹਾਲਤ One_Dim_ਖਾਨਾ_Wise: ਵਾਪਸ "OneDimColWise";
    ਹਾਲਤ TwoDims: ਵਾਪਸ "TwoDims";
    };
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isInv(T ਬਹੁਲਕ)
{
    ਚੋਣ ((Dft_ਬਹੁਲਕ)ਬਹੁਲਕ)
    {
        ਹਾਲਤ InvCCSToReal:
        ਹਾਲਤ Inv_Complex_ਮੰਜ਼ਿਲ_Real:
        ਹਾਲਤ Inv_Complex_ਮੰਜ਼ਿਲ_Complex: ਵਾਪਸ ਸੱਚ;
        ਮੂਲ: ਵਾਪਸ ਝੂਠ;
    }
}

ਸਤਰਅੰਦਰ Dft_ਬਹੁਲਕ determine_ਬਹੁਲਕ(ਤਾਰਕਿਕ inv, ਪੂਰਨ cn1, ਪੂਰਨ cn2)
{
    ਜੇ (!inv)
    {
        ਜੇ (cn1 == 1 && cn2 == 1)
            ਵਾਪਸ Fwd_Real_ਮੰਜ਼ਿਲ_CCS;
        ਨਹੀਂਤਾਂ ਜੇ (cn1 == 1 && cn2 == 2)
            ਵਾਪਸ Fwd_Real_ਮੰਜ਼ਿਲ_Complex;
        ਨਹੀਂਤਾਂ ਜੇ (cn1 == 2 && cn2 == 2)
            ਵਾਪਸ Fwd_Complex_ਮੰਜ਼ਿਲ_Complex;
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (cn1 == 1 && cn2 == 1)
            ਵਾਪਸ InvCCSToReal;
        ਨਹੀਂਤਾਂ ਜੇ (cn1 == 2 && cn2 == 1)
            ਵਾਪਸ Inv_Complex_ਮੰਜ਼ਿਲ_Real;
        ਨਹੀਂਤਾਂ ਜੇ (cn1 == 2 && cn2 == 2)
            ਵਾਪਸ Inv_Complex_ਮੰਜ਼ਿਲ_Complex;
    }
    ਵਾਪਸ InvalidDft;
}


ਸਤਰਅੰਦਰ DftDims determineDims(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਤਾਰਕਿਕ is_ਪੰਕਤੀ_Wise, ਤਾਰਕਿਕ isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    ਜੇ (is_ਪੰਕਤੀ_Wise)
        ਵਾਪਸ OneDim;
    ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ == 1 && ਪੰਕਤੀਆਂ > 1) // one-column-shaped input
    {
        ਜੇ (isContinuous)
            ਵਾਪਸ OneDim;
        ਨਹੀਂਤਾਂ
            ਵਾਪਸ One_Dim_ਖਾਨਾ_Wise;
    }
    ਜੇ (ਪੰਕਤੀਆਂ == 1)
        ਵਾਪਸ OneDim;
    ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ > 1 && ਪੰਕਤੀਆਂ > 1)
        ਵਾਪਸ TwoDims;
    ਵਾਪਸ InvalidDim;
}

ਵਰਗ OcvDftBasicImpl CV_FINAL : ਜਨਤਕ hal::DFT1D
{
ਜਨਤਕ:
    OcvDftOptions opt;
    // the plan is built once in init(), the workspace is allocated once here and reused by
    // every apply() (zero allocations per transform)
    DftPlan plan;
    Auto_ਬਫਰ<uchar> ws;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
    Auto_ਬਫਰ<uchar> ippbuf;
    Auto_ਬਫਰ<uchar> ippworkbuf;
    size_t ipp_worksize;
#ਜੇ_ਅੰਤ

ਜਨਤਕ:
    OcvDftBasicImpl()
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
        ipp_worksize = 0;
#ਜੇ_ਅੰਤ
    }
    ਖਾਲੀ init(ਪੂਰਨ len, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ flags, ਤਾਰਕਿਕ *need_ਬਫਰ)
    {
        ਪੂਰਨ stage = (flags & CV_HAL_DFT_STAGE_ਖਾਨੇ_ਗਿਣਤੀ) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        ਤਾਰਕਿਕ real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        ਤਾਰਕਿਕ needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = ਝੂਠ;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        ਜੇ( CV_IPP_CHECK_COND && (opt.n*ਗਿਣਤੀ_3 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            ਪੂਰਨ ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            ਪੂਰਨ specsize=0, initsize=0, worksize=0;
            Ipp_DFTGet_ਮਾਪ_Func get_ਮਾਪ_Func = 0;
            IppDFTInitFunc initFunc = 0;

            ਜੇ( real_transform && stage == 0 )
            {
                ਜੇ( ਡੂੰਘਾਈ == CV_32F )
                {
                    get_ਮਾਪ_Func = ipps_DFTGet_ਮਾਪ_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                ਨਹੀਂਤਾਂ
                {
                    get_ਮਾਪ_Func = ipps_DFTGet_ਮਾਪ_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ( ਡੂੰਘਾਈ == CV_32F )
                {
                    get_ਮਾਪ_Func = ipps_DFTGet_ਮਾਪ_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                ਨਹੀਂਤਾਂ
                {
                    get_ਮਾਪ_Func = ipps_DFTGet_ਮਾਪ_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            ਜੇ( get_ਮਾਪ_Func(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                ਜੇ( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = ਸੱਚ;
            }
            ਨਹੀਂਤਾਂ
                ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
        }
    #ਜੇ_ਅੰਤ

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            ਅਚਲ DFTKernels& kernels = getDFTKernels(ਡੂੰਘਾਈ);
            ਪੂਰਨ ਕਿਸਮ = stage == 0 && real_transform ? (opt.isInverse ? DFT_ਕਿਸਮ_C2_R : DFT_ਕਿਸਮ_R2_C)
                                                    : DFT_ਕਿਸਮ_C2_C;
            plan.build(ਕਿਸਮ, len, ਡੂੰਘਾਈ, kernels.vlanes);
            ws.allocate(plan.ws_ਬਾਈਟਾਂ);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.ਅੰਕੜੇ();
            ਜੇ (need_ਬਫਰ)
                *need_ਬਫਰ = ਝੂਠ;   // the engine supports in-place for every kind
        }

        {
            ਸਥਿਰ DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<ਦਸ਼ਮਲਵ>>,
                dftWrap<ਦਸ਼ਮਲਵ, RealDFT<ਦਸ਼ਮਲਵ>>,
                dftWrap<ਦਸ਼ਮਲਵ, CCSIDFT<ਦਸ਼ਮਲਵ>>,
                dftWrap<Complexd, DFT<ਦੁੱਗਣਾ>>,
                dftWrap<ਦੁੱਗਣਾ, RealDFT<ਦੁੱਗਣਾ>>,
                dftWrap<ਦੁੱਗਣਾ, CCSIDFT<ਦੁੱਗਣਾ>>
            };
            ਪੂਰਨ idx = 0;
            ਜੇ (stage == 0)
            {
                ਜੇ (real_transform)
                {
                    ਜੇ (!opt.isInverse)
                        idx = 1;
                    ਨਹੀਂਤਾਂ
                        idx = 2;
                }
            }
            ਜੇ (ਡੂੰਘਾਈ == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        ਜੇ(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            ਪੂਰਨ ਪੰਕਤੀ_ਗਿਣਤੀ = ਗਿਣਤੀ_3;
            ਜੇ (stage == 0 && (flags & CV_HAL_DFT_ਪੰਕਤੀਆਂ) != 0)
                ਪੰਕਤੀ_ਗਿਣਤੀ = 1;
            opt.scale = 1./(len * ਪੰਕਤੀ_ਗਿਣਤੀ);
        }
    }

    ਖਾਲੀ apply(ਅਚਲ uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspace_ਮਾਪ() ਅਚਲ
    {
        size_t sz = opt.plan ? plan.ws_ਬਾਈਟਾਂ : 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
        ਜੇ (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#ਜੇ_ਅੰਤ
        ਵਾਪਸ sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    ਖਾਲੀ applyWithWorkspace(ਅਚਲ uchar *src, uchar *dst, uchar* workspace) ਅਚਲ
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_IPP_DFT
        ਜੇ (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#ਜੇ_ਅੰਤ
        opt_.dft_func(opt_, src, dst);
    }

    ਖਾਲੀ ਮੁਕਤ() {}
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
ਸਥਿਰ ਅਚਲ ਪੂਰਨ DFT_ਖਾਨੇ_ਗਿਣਤੀ_BATCH_MIN = 4;
ਸਥਿਰ ਅਚਲ ਪੂਰਨ DFT_ਖਾਨੇ_ਗਿਣਤੀ_BATCH_MAX = 16;
ਸਥਿਰ ਅਚਲ size_t DFT_ਖਾਨੇ_ਗਿਣਤੀ_BATCH_ਬਾਈਟਾਂ = 1 << 17;

ਸਥਿਰ ਪੂਰਨ dft_ਖਾਨੇ_ਗਿਣਤੀ_Batch(ਪੂਰਨ len, size_t complex_elem_ਮਾਪ)
{
    ਪੂਰਨ batch = (ਪੂਰਨ)(DFT_ਖਾਨੇ_ਗਿਣਤੀ_BATCH_ਬਾਈਟਾਂ/((size_t)len*complex_elem_ਮਾਪ));
    ਵਾਪਸ std::max(DFT_ਖਾਨੇ_ਗਿਣਤੀ_BATCH_MIN, std::min(DFT_ਖਾਨੇ_ਗਿਣਤੀ_BATCH_MAX, batch));
}

ਸਥਿਰ ਦੁੱਗਣਾ dftParallelStripes(ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ len)
{
    ਅਚਲ ਦੁੱਗਣਾ ਸਮਾਂ_ਹਿੱਸਾ = 0.5e6;
    ਵਾਪਸ (ਦੁੱਗਣਾ)ਗਿਣਤੀ_3*len*std::max(std::ਲਘੂਗਣਕ((ਦੁੱਗਣਾ)len)/std::ਲਘੂਗਣਕ(2.), 1.)/ਸਮਾਂ_ਹਿੱਸਾ;
}

ਵਰਗ OcvDftImpl CV_FINAL : ਜਨਤਕ hal::DFT2D
{
ਸੁਰੱਖਿਅਤ:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    ਅਚਲ OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    ਅਚਲ OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    ਤਾਰਕਿਕ need_ਬਫਰ_A;
    ਤਾਰਕਿਕ need_ਬਫਰ_B;
    ਤਾਰਕਿਕ inv;
    ਪੂਰਨ ਚੌੜਾਈ;
    ਪੂਰਨ ਉਚਾਈ;
    Dft_ਬਹੁਲਕ ਬਹੁਲਕ;
    ਪੂਰਨ elem_ਮਾਪ_2;
    ਪੂਰਨ complex_elem_ਮਾਪ;
    ਪੂਰਨ ਡੂੰਘਾਈ;
    ਤਾਰਕਿਕ real_transform;
    ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ;
    ਤਾਰਕਿਕ is_ਪੰਕਤੀ_Transform;
    ਤਾਰਕਿਕ isScaled;
    std::vector<ਪੂਰਨ> stages;
    ਤਾਰਕਿਕ useIpp;
    ਪੂਰਨ src_channels;
    ਪੂਰਨ dst_channels;

    Auto_ਬਫਰ<uchar> tmp_bufA;
    Auto_ਬਫਰ<uchar> tmp_bufB;
    Auto_ਬਫਰ<uchar> buf0;
    Auto_ਬਫਰ<uchar> buf1;

ਜਨਤਕ:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        need_ਬਫਰ_A = ਝੂਠ;
        need_ਬਫਰ_B = ਝੂਠ;
        inv = ਝੂਠ;
        ਚੌੜਾਈ = 0;
        ਉਚਾਈ = 0;
        ਬਹੁਲਕ = InvalidDft;
        elem_ਮਾਪ_2 = 0;
        complex_elem_ਮਾਪ = 0;
        ਡੂੰਘਾਈ = 0;
        real_transform = ਝੂਠ;
        nonzero_ਪੰਕਤੀਆਂ = 0;
        is_ਪੰਕਤੀ_Transform = ਝੂਠ;
        isScaled = ਝੂਠ;
        useIpp = ਝੂਠ;
        src_channels = 0;
        dst_channels = 0;
    }

    ਖਾਲੀ init(ਪੂਰਨ ਚੌੜਾਈ_3, ਪੂਰਨ ਉਚਾਈ_3, ਪੂਰਨ ਡੂੰਘਾਈ_2, ਪੂਰਨ _src_channels, ਪੂਰਨ _dst_channels, ਪੂਰਨ flags, ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ_2)
    {
        ਤਾਰਕਿਕ isComplex = _src_channels != _dst_channels;
        nonzero_ਪੰਕਤੀਆਂ = nonzero_ਪੰਕਤੀਆਂ_2;
        ਚੌੜਾਈ = ਚੌੜਾਈ_3;
        ਉਚਾਈ = ਉਚਾਈ_3;
        ਡੂੰਘਾਈ = ਡੂੰਘਾਈ_2;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        ਤਾਰਕਿਕ isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        ਤਾਰਕਿਕ isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        ਤਾਰਕਿਕ isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        ਬਹੁਲਕ = determine_ਬਹੁਲਕ(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        is_ਪੰਕਤੀ_Transform = (flags & CV_HAL_DFT_ਪੰਕਤੀਆਂ) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        need_ਬਫਰ_A = ਝੂਠ;
        need_ਬਫਰ_B = ਝੂਠ;
        real_transform = (ਬਹੁਲਕ != Fwd_Complex_ਮੰਜ਼ਿਲ_Complex && ਬਹੁਲਕ != Inv_Complex_ਮੰਜ਼ਿਲ_Complex);

        elem_ਮਾਪ_2 = (ਡੂੰਘਾਈ == CV_32F) ? ਆਕਾਰ(ਦਸ਼ਮਲਵ) : ਆਕਾਰ(ਦੁੱਗਣਾ);
        complex_elem_ਮਾਪ = elem_ਮਾਪ_2 * 2;
        ਜੇ( !real_transform )
            elem_ਮਾਪ_2 = complex_elem_ਮਾਪ;

#ਜੇ defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            ਜੇ (nonzero_ਪੰਕਤੀਆਂ == 0 && ਡੂੰਘਾਈ == CV_32F && ((ਚੌੜਾਈ * ਉਚਾਈ)>(ਪੂਰਨ)(1<<6)))
            {
                ਜੇ (ਬਹੁਲਕ == Fwd_Complex_ਮੰਜ਼ਿਲ_Complex || ਬਹੁਲਕ == Inv_Complex_ਮੰਜ਼ਿਲ_Complex || ਬਹੁਲਕ == Fwd_Real_ਮੰਜ਼ਿਲ_CCS || ਬਹੁਲਕ == InvCCSToReal)
                {
                    useIpp = ਸੱਚ;
                    ਵਾਪਸ;
                }
            }
        }
#ਜੇ_ਅੰਤ

        DftDims dims = determineDims(ਉਚਾਈ, ਚੌੜਾਈ, is_ਪੰਕਤੀ_Transform, isContinuous);
        ਜੇ (dims == TwoDims)
        {
            stages.resize(2);
            ਜੇ (ਬਹੁਲਕ == InvCCSToReal || ਬਹੁਲਕ == Inv_Complex_ਮੰਜ਼ਿਲ_Real)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            ਨਹੀਂਤਾਂ
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        ਨਹੀਂਤਾਂ
        {
            stages.resize(1);
            ਜੇ (dims == One_Dim_ਖਾਨਾ_Wise)
                stages[0] = 1;
            ਨਹੀਂਤਾਂ
                stages[0] = 0;
        }

        ਲਈ(uint stage_ਸੂਚਕਾਂਕ = 0; stage_ਸੂਚਕਾਂਕ < stages.size(); ++stage_ਸੂਚਕਾਂਕ)
        {
            ਜੇ (stage_ਸੂਚਕਾਂਕ == 1)
            {
                isInplace = ਸੱਚ;
                isComplex = ਝੂਠ;
            }

            ਪੂਰਨ stage = stages[stage_ਸੂਚਕਾਂਕ];
            ਤਾਰਕਿਕ is_ਆਖਰੀ_Stage = (stage_ਸੂਚਕਾਂਕ + 1 == stages.size());

            ਪੂਰਨ len, ਗਿਣਤੀ_3;

            ਪੂਰਨ f = 0;
            ਜੇ (inv)
                f |= CV_HAL_DFT_INVERSE;
            ਜੇ (isScaled)
                f |= CV_HAL_DFT_SCALE;
            ਜੇ (is_ਪੰਕਤੀ_Transform)
                f |= CV_HAL_DFT_ਪੰਕਤੀਆਂ;
            ਜੇ (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            ਜੇ (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            ਜੇ (!is_ਆਖਰੀ_Stage)
                f |= CV_HAL_DFT_TWO_STAGE;

            ਜੇ( stage == 0 ) // row-wise transform
            {
                ਜੇ (ਚੌੜਾਈ == 1 && !is_ਪੰਕਤੀ_Transform )
                {
                    len = ਉਚਾਈ;
                    ਗਿਣਤੀ_3 = ਚੌੜਾਈ;
                }
                ਨਹੀਂਤਾਂ
                {
                    len = ਚੌੜਾਈ;
                    ਗਿਣਤੀ_3 = ਉਚਾਈ;
                }
                need_ਬਫਰ_A = isInplace;
                contextA = hal::DFT1D::create(len, ਗਿਣਤੀ_3, ਡੂੰਘਾਈ, f, &need_ਬਫਰ_A);
                basicA = dynamic_cast<ਅਚਲ OcvDftBasicImpl*>(contextA.get());
                ਜੇ (need_ਬਫਰ_A)
                    tmp_bufA.allocate(len * complex_elem_ਮਾਪ);
            }
            ਨਹੀਂਤਾਂ
            {
                len = ਉਚਾਈ;
                ਗਿਣਤੀ_3 = ਚੌੜਾਈ;
                f |= CV_HAL_DFT_STAGE_ਖਾਨੇ_ਗਿਣਤੀ;
                need_ਬਫਰ_B = isInplace;
                contextB = hal::DFT1D::create(len, ਗਿਣਤੀ_3, ਡੂੰਘਾਈ, f, &need_ਬਫਰ_B);
                basicB = dynamic_cast<ਅਚਲ OcvDftBasicImpl*>(contextB.get());
                ਜੇ (need_ਬਫਰ_B)
                    tmp_bufB.allocate(len * complex_elem_ਮਾਪ);

                buf0.allocate(len * complex_elem_ਮਾਪ);
                buf1.allocate(len * complex_elem_ਮਾਪ);
            }
        }
    }

    ਖਾਲੀ apply(ਅਚਲ uchar * src, size_t src_ਕਦਮ, uchar * dst, size_t dst_ਕਦਮ) CV_OVERRIDE
    {
#ਜੇ defined USE_IPP_DFT
        ਜੇ (useIpp)
        {
            ਪੂਰਨ ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            ਜੇ (!is_ਪੰਕਤੀ_Transform)
            {
                ਜੇ (ਬਹੁਲਕ == Fwd_Complex_ਮੰਜ਼ਿਲ_Complex || ਬਹੁਲਕ == Inv_Complex_ਮੰਜ਼ਿਲ_Complex)
                {
                    ਜੇ (ippi_DFT_C_32F(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
                        ਵਾਪਸ;
                    }
                    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
                }
                ਨਹੀਂਤਾਂ ਜੇ (ਬਹੁਲਕ == Fwd_Real_ਮੰਜ਼ਿਲ_CCS || ਬਹੁਲਕ == InvCCSToReal)
                {
                    ਜੇ (ippi_DFT_R_32F(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP);
                        ਵਾਪਸ;
                    }
                    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
                }
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ (ਬਹੁਲਕ == Fwd_Complex_ਮੰਜ਼ਿਲ_Complex || ਬਹੁਲਕ == Inv_Complex_ਮੰਜ਼ਿਲ_Complex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    ਜੇ (Dft_C_IPPLoop(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP|CV_IMPL_MT);
                        ਵਾਪਸ;
                    }
                    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
                }
                ਨਹੀਂਤਾਂ ਜੇ (ਬਹੁਲਕ == Fwd_Real_ਮੰਜ਼ਿਲ_CCS || ਬਹੁਲਕ == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippi_DFTInv_Pack_ਮੰਜ਼ਿਲ_R_32f_C1_R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    ਜੇ (Dft_R_IPPLoop(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ਜੋੜੋ(CV_IMPL_IPP|CV_IMPL_MT);
                        ਵਾਪਸ;
                    }
                    ਸੈੱਟ_Ipp_Error_ਸਥਿਤੀ();
                }
            }
            ਵਾਪਸ;
        }
#ਜੇ_ਅੰਤ

        ਲਈ(uint stage_ਸੂਚਕਾਂਕ = 0; stage_ਸੂਚਕਾਂਕ < stages.size(); ++stage_ਸੂਚਕਾਂਕ)
        {
            ਪੂਰਨ stage_src_channels = src_channels;
            ਪੂਰਨ stage_dst_channels = dst_channels;

            ਜੇ (stage_ਸੂਚਕਾਂਕ == 1)
            {
                src = dst;
                src_ਕਦਮ = dst_ਕਦਮ;
                stage_src_channels = stage_dst_channels;
            }

            ਪੂਰਨ stage = stages[stage_ਸੂਚਕਾਂਕ];
            ਤਾਰਕਿਕ is_ਆਖਰੀ_Stage = (stage_ਸੂਚਕਾਂਕ + 1 == stages.size());
            ਤਾਰਕਿਕ isComplex = stage_src_channels != stage_dst_channels;

            ਜੇ( stage == 0 )
                ਪੰਕਤੀ_Dft(src, src_ਕਦਮ, dst, dst_ਕਦਮ, isComplex, is_ਆਖਰੀ_Stage);
            ਨਹੀਂਤਾਂ
                ਖਾਨਾ_Dft(src, src_ਕਦਮ, dst, dst_ਕਦਮ, stage_src_channels, stage_dst_channels, is_ਆਖਰੀ_Stage);
        }
    }

ਸੁਰੱਖਿਅਤ:

    ਖਾਲੀ ਪੰਕਤੀ_Dft(ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਤਾਰਕਿਕ isComplex, ਤਾਰਕਿਕ is_ਆਖਰੀ_Stage)
    {
        ਪੂਰਨ len, ਗਿਣਤੀ_3;
        ਜੇ (ਚੌੜਾਈ == 1 && !is_ਪੰਕਤੀ_Transform )
        {
            len = ਉਚਾਈ;
            ਗਿਣਤੀ_3 = ਚੌੜਾਈ;
        }
        ਨਹੀਂਤਾਂ
        {
            len = ਚੌੜਾਈ;
            ਗਿਣਤੀ_3 = ਉਚਾਈ;
        }
        ਪੂਰਨ dptr_ਵਿੱਥ = 0;
        ਪੂਰਨ dst_ਭਰਿਆ_len = len*elem_ਮਾਪ_2;

        ਜੇ( need_ਬਫਰ_A )
        {
            ਜੇ (ਬਹੁਲਕ == Fwd_Real_ਮੰਜ਼ਿਲ_CCS && (len & 1) && len > 1)
                dptr_ਵਿੱਥ = elem_ਮਾਪ_2;
        }

        ਜੇ( !inv && isComplex )
            dst_ਭਰਿਆ_len += (len & 1) ? elem_ਮਾਪ_2 : complex_elem_ਮਾਪ;

        ਪੂਰਨ nz = nonzero_ਪੰਕਤੀਆਂ;
        ਜੇ( nz <= 0 || nz > ਗਿਣਤੀ_3 )
            nz = ਗਿਣਤੀ_3;

        size_t ws_ਮਾਪ = basicA ? basicA->workspace_ਮਾਪ() : 0;
        ਦੁੱਗਣਾ nstripes = dftParallelStripes(nz, len);
        ਜੇ( ws_ਮਾਪ > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            ਅਚਲ OcvDftBasicImpl* ctx = basicA;
            ਸਵੈਚਾਲਿਤ process_ਪੰਕਤੀਆਂ = [&, ctx](ਅਚਲ Range& range)
            {
                Auto_ਬਫਰ<uchar> ws(ws_ਮਾਪ);
                Auto_ਬਫਰ<uchar> rowbuf(need_ਬਫਰ_A ? len * complex_elem_ਮਾਪ : 0);
                ਲਈ( ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; i++ )
                {
                    ਅਚਲ uchar* sptr = src_ਅੰਕੜੇ + src_ਕਦਮ * i;
                    uchar* dptr0 = dst_ਅੰਕੜੇ + dst_ਕਦਮ * i;
                    uchar* dptr = need_ਬਫਰ_A ? rowbuf.ਅੰਕੜੇ() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.ਅੰਕੜੇ());
                    ਜੇ( need_ਬਫਰ_A )
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ( dptr0, dptr + dptr_ਵਿੱਥ, dst_ਭਰਿਆ_len );
                }
            };
            parallel_for_(Range(0, nz), process_ਪੰਕਤੀਆਂ, nstripes);
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਈ( ਪੂਰਨ i = 0; i < nz; i++ )
            {
                ਅਚਲ uchar* sptr = src_ਅੰਕੜੇ + src_ਕਦਮ * i;
                uchar* dptr0 = dst_ਅੰਕੜੇ + dst_ਕਦਮ * i;
                uchar* dptr = dptr0;

                ਜੇ( need_ਬਫਰ_A )
                    dptr = tmp_bufA.ਅੰਕੜੇ();

                contextA->apply(sptr, dptr);

                ਜੇ( need_ਬਫਰ_A )
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ( dptr0, dptr + dptr_ਵਿੱਥ, dst_ਭਰਿਆ_len );
            }
        }

        ਲਈ( ਪੂਰਨ i = nz; i < ਗਿਣਤੀ_3; i++ )
        {
            uchar* dptr0 = dst_ਅੰਕੜੇ + dst_ਕਦਮ * i;
            memset( dptr0, 0, dst_ਭਰਿਆ_len );
        }
        ਜੇ(is_ਆਖਰੀ_Stage &&  ਬਹੁਲਕ == Fwd_Real_ਮੰਜ਼ਿਲ_Complex)
            complementComplexOutput(ਡੂੰਘਾਈ, dst_ਅੰਕੜੇ, dst_ਕਦਮ, len, nz, 1);
    }

    ਖਾਲੀ ਖਾਨਾ_Dft(ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ stage_src_channels, ਪੂਰਨ stage_dst_channels, ਤਾਰਕਿਕ is_ਆਖਰੀ_Stage)
    {
        ਪੂਰਨ len = ਉਚਾਈ;
        ਪੂਰਨ ਗਿਣਤੀ_3 = ਚੌੜਾਈ;
        ਪੂਰਨ a = 0, b = ਗਿਣਤੀ_3;
        uchar *dbuf0, *dbuf1;
        ਅਚਲ uchar* sptr0 = src_ਅੰਕੜੇ;
        uchar* dptr0 = dst_ਅੰਕੜੇ;

        dbuf0 = buf0.ਅੰਕੜੇ(), dbuf1 = buf1.ਅੰਕੜੇ();

        ਜੇ( need_ਬਫਰ_B )
        {
            dbuf1 = tmp_bufB.ਅੰਕੜੇ();
            dbuf0 = buf1.ਅੰਕੜੇ();
        }

        ਜੇ( real_transform )
        {
            ਪੂਰਨ even;
            a = 1;
            even = (ਗਿਣਤੀ_3 & 1) == 0;
            b = (ਗਿਣਤੀ_3+1)/2;
            ਜੇ( !inv )
            {
                memset( buf0.ਅੰਕੜੇ(), 0, len*complex_elem_ਮਾਪ );
                Copy_ਕਾਲਮ( sptr0, src_ਕਦਮ, buf0.ਅੰਕੜੇ(), complex_elem_ਮਾਪ, len, elem_ਮਾਪ_2 );
                sptr0 += stage_dst_channels*elem_ਮਾਪ_2;
                ਜੇ( even )
                {
                    memset( buf1.ਅੰਕੜੇ(), 0, len*complex_elem_ਮਾਪ );
                    Copy_ਕਾਲਮ( sptr0 + (ਗਿਣਤੀ_3-2)*elem_ਮਾਪ_2, src_ਕਦਮ,
                                buf1.ਅੰਕੜੇ(), complex_elem_ਮਾਪ, len, elem_ਮਾਪ_2 );
                }
            }
            ਨਹੀਂਤਾਂ ਜੇ( stage_src_channels == 1 )
            {
                Copy_ਕਾਲਮ( sptr0, src_ਕਦਮ, buf0.ਅੰਕੜੇ(), elem_ਮਾਪ_2, len, elem_ਮਾਪ_2 );
                ExpandCCS( buf0.ਅੰਕੜੇ(), len, elem_ਮਾਪ_2 );
                ਜੇ( even )
                {
                    Copy_ਕਾਲਮ( sptr0 + (ਗਿਣਤੀ_3-1)*elem_ਮਾਪ_2, src_ਕਦਮ,
                                buf1.ਅੰਕੜੇ(), elem_ਮਾਪ_2, len, elem_ਮਾਪ_2 );
                    ExpandCCS( buf1.ਅੰਕੜੇ(), len, elem_ਮਾਪ_2 );
                }
                sptr0 += elem_ਮਾਪ_2;
            }
            ਨਹੀਂਤਾਂ
            {
                Copy_ਕਾਲਮ( sptr0, src_ਕਦਮ, buf0.ਅੰਕੜੇ(), complex_elem_ਮਾਪ, len, complex_elem_ਮਾਪ );
                ਜੇ( even )
                {
                    Copy_ਕਾਲਮ( sptr0 + b*complex_elem_ਮਾਪ, src_ਕਦਮ,
                                   buf1.ਅੰਕੜੇ(), complex_elem_ਮਾਪ, len, complex_elem_ਮਾਪ );
                }
                sptr0 += complex_elem_ਮਾਪ;
            }

            ਜੇ( even )
                contextB->apply(buf1.ਅੰਕੜੇ(), dbuf1);
            contextB->apply(buf0.ਅੰਕੜੇ(), dbuf0);

            ਜੇ( stage_dst_channels == 1 )
            {
                ਜੇ( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ( dbuf0 + elem_ਮਾਪ_2, dbuf0, elem_ਮਾਪ_2 );
                    Copy_ਕਾਲਮ( dbuf0 + elem_ਮਾਪ_2, elem_ਮਾਪ_2, dptr0,
                                   dst_ਕਦਮ, len, elem_ਮਾਪ_2 );
                    ਜੇ( even )
                    {
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ( dbuf1 + elem_ਮਾਪ_2, dbuf1, elem_ਮਾਪ_2 );
                        Copy_ਕਾਲਮ( dbuf1 + elem_ਮਾਪ_2, elem_ਮਾਪ_2,
                                       dptr0 + (ਗਿਣਤੀ_3-1)*elem_ਮਾਪ_2,
                                       dst_ਕਦਮ, len, elem_ਮਾਪ_2 );
                    }
                    dptr0 += elem_ਮਾਪ_2;
                }
                ਨਹੀਂਤਾਂ
                {
                    // copy the real part of the complex vector to the first/last column
                    Copy_ਕਾਲਮ( dbuf0, complex_elem_ਮਾਪ, dptr0, dst_ਕਦਮ, len, elem_ਮਾਪ_2 );
                    ਜੇ( even )
                        Copy_ਕਾਲਮ( dbuf1, complex_elem_ਮਾਪ, dptr0 + (ਗਿਣਤੀ_3-1)*elem_ਮਾਪ_2,
                                       dst_ਕਦਮ, len, elem_ਮਾਪ_2 );
                    dptr0 += elem_ਮਾਪ_2;
                }
            }
            ਨਹੀਂਤਾਂ
            {
                CV_Assert( !inv );
                Copy_ਕਾਲਮ( dbuf0, complex_elem_ਮਾਪ, dptr0,
                               dst_ਕਦਮ, len, complex_elem_ਮਾਪ );
                ਜੇ( even )
                    Copy_ਕਾਲਮ( dbuf1, complex_elem_ਮਾਪ,
                                   dptr0 + b*complex_elem_ਮਾਪ,
                                   dst_ਕਦਮ, len, complex_elem_ਮਾਪ );
                dptr0 += complex_elem_ਮਾਪ;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        ਪੂਰਨ ncols = b - a;
        ਅਚਲ ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_batch = dft_ਖਾਨੇ_ਗਿਣਤੀ_Batch(len, complex_elem_ਮਾਪ);
        ਪੂਰਨ nbatches = (ncols + ਖਾਨੇ_ਗਿਣਤੀ_batch - 1)/ਖਾਨੇ_ਗਿਣਤੀ_batch;
        size_t ws_ਮਾਪ = basicB ? basicB->workspace_ਮਾਪ() : 0;
        size_t colbuf_ਮਾਪ = (size_t)len*complex_elem_ਮਾਪ;
        ਅਚਲ OcvDftBasicImpl* ctx = basicB;
        ਸਵੈਚਾਲਿਤ processBatches = [&, ctx](ਅਚਲ Range& range)
        {
            Auto_ਬਫਰ<uchar> ws(ws_ਮਾਪ);
            Auto_ਬਫਰ<uchar> cbuf(colbuf_ਮਾਪ*ਖਾਨੇ_ਗਿਣਤੀ_batch*(need_ਬਫਰ_B ? 2 : 1));
            uchar* cbuf0 = cbuf.ਅੰਕੜੇ();
            uchar* cdbuf0 = need_ਬਫਰ_B ? cbuf0 + colbuf_ਮਾਪ*ਖਾਨੇ_ਗਿਣਤੀ_batch : cbuf0;
            ਲਈ( ਪੂਰਨ bi = range.ਸ਼ੁਰੂ; bi < range.end; bi++ )
            {
                ਪੂਰਨ c0 = bi*ਖਾਨੇ_ਗਿਣਤੀ_batch;
                ਪੂਰਨ nc = std::min(ਖਾਨੇ_ਗਿਣਤੀ_batch, ncols - c0);
                ਅਚਲ uchar* sptr = sptr0 + (size_t)c0*complex_elem_ਮਾਪ;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_ਮਾਪ;
                CopyFromColumns( sptr, src_ਕਦਮ, cbuf0, nc, len, complex_elem_ਮਾਪ );
                ਲਈ( ਪੂਰਨ k = 0; k < nc; k++ )
                {
                    ਜੇ( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_ਮਾਪ, cdbuf0 + k*colbuf_ਮਾਪ, ws.ਅੰਕੜੇ());
                    ਨਹੀਂਤਾਂ
                        contextB->apply(cbuf0 + k*colbuf_ਮਾਪ, cdbuf0 + k*colbuf_ਮਾਪ);
                }
                Copy_ਮੰਜ਼ਿਲ_Columns( cdbuf0, dptr, dst_ਕਦਮ, nc, len, complex_elem_ਮਾਪ );
            }
        };
        ਦੁੱਗਣਾ nstripes = dftParallelStripes(ncols, len);
        ਜੇ( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        ਨਹੀਂਤਾਂ
            processBatches(Range(0, nbatches));
        ਜੇ(is_ਆਖਰੀ_Stage && ਬਹੁਲਕ == Fwd_Real_ਮੰਜ਼ਿਲ_Complex)
            complementComplexOutput(ਡੂੰਘਾਈ, dst_ਅੰਕੜੇ, dst_ਕਦਮ, ਗਿਣਤੀ_3, len, 2);
    }
};

ਬਣਤਰ ReplacementDFT1D : ਜਨਤਕ hal::DFT1D
{
    cvhalDFT *context;
    ਤਾਰਕਿਕ isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(ਝੂਠ) {}
    ਤਾਰਕਿਕ init(ਪੂਰਨ len, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ flags, ਤਾਰਕਿਕ *need_ਬਫਰ)
    {
        ਪੂਰਨ res = cv_hal_dftInit1D(&context, len, ਗਿਣਤੀ_3, ਡੂੰਘਾਈ, flags, need_ਬਫਰ);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ਵਾਪਸ isInitialized;
    }
    ਖਾਲੀ apply(ਅਚਲ uchar *src, uchar *dst) CV_OVERRIDE
    {
        ਜੇ (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        ਜੇ (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

ਬਣਤਰ ReplacementDFT2D : ਜਨਤਕ hal::DFT2D
{
    cvhalDFT *context;
    ਤਾਰਕਿਕ isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(ਝੂਠ) {}
    ਤਾਰਕਿਕ init(ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਡੂੰਘਾਈ,
              ਪੂਰਨ src_channels, ਪੂਰਨ dst_channels,
              ਪੂਰਨ flags, ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ)
    {
        ਪੂਰਨ res = cv_hal_dftInit2D(&context, ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, src_channels, dst_channels, flags, nonzero_ਪੰਕਤੀਆਂ);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ਵਾਪਸ isInitialized;
    }
    ਖਾਲੀ apply(ਅਚਲ uchar *src, size_t src_ਕਦਮ, uchar *dst, size_t dst_ਕਦਮ) CV_OVERRIDE
    {
        ਜੇ (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_ਕਦਮ, dst, dst_ਕਦਮ);
        }
    }
    ~ReplacementDFT2D()
    {
        ਜੇ (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

ਨਾਮਾਕਾਸ਼ hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(ਪੂਰਨ len, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ flags, ਤਾਰਕਿਕ *need_ਬਫਰ)
{
    {
        ReplacementDFT1D *impl = ਨਵਾਂ ReplacementDFT1D();
        ਜੇ (impl->init(len, ਗਿਣਤੀ_3, ਡੂੰਘਾਈ, flags, need_ਬਫਰ))
        {
            ਵਾਪਸ Ptr<DFT1D>(impl);
        }
        ਮਿਟਾਓ impl;
    }
    {
        OcvDftBasicImpl *impl = ਨਵਾਂ OcvDftBasicImpl();
        impl->init(len, ਗਿਣਤੀ_3, ਡੂੰਘਾਈ, flags, need_ਬਫਰ);
        ਵਾਪਸ Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਡੂੰਘਾਈ,
                         ਪੂਰਨ src_channels, ਪੂਰਨ dst_channels,
                         ਪੂਰਨ flags, ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ)
{
    {
        ReplacementDFT2D *impl = ਨਵਾਂ ReplacementDFT2D();
        ਜੇ (impl->init(ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, src_channels, dst_channels, flags, nonzero_ਪੰਕਤੀਆਂ))
        {
            ਵਾਪਸ Ptr<DFT2D>(impl);
        }
        ਮਿਟਾਓ impl;
    }
    {
        ਜੇ(ਚੌੜਾਈ == 1 && nonzero_ਪੰਕਤੀਆਂ > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = ਨਵਾਂ OcvDftImpl();
        impl->init(ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, src_channels, dst_channels, flags, nonzero_ਪੰਕਤੀਆਂ);
        ਵਾਪਸ Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


ਖਾਲੀ cv::dft( ਨਿਵੇਸ਼_Array _src0, OutputArray _dst, ਪੂਰਨ flags, ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ )
{
    CV_INSTRUMENT_REGION();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CLAMDFFT
    CV_OCL_ਲੜੀਵਾਰ(ocl::haveAmdFft() && ocl::Device::getDefault().type() != ocl::Device::TYPE_CPU &&
            _dst.isUMat() && _src0.dims() <= 2 && nonzero_ਪੰਕਤੀਆਂ == 0,
               ocl_dft_amdfft(_src0, _dst, flags))
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat() && _src0.dims() <= 2,
               ocl_dft(_src0, _dst, flags, nonzero_ਪੰਕਤੀਆਂ))
#ਜੇ_ਅੰਤ

    Mat src0 = _src0.getMat(), src = src0;
    ਜੇ (src.empty())
    {
        _dst.release();
        ਵਾਪਸ;
    }
    ਤਾਰਕਿਕ inv = (flags & DFT_INVERSE) != 0;
    ਪੂਰਨ type = src.type();
    ਪੂਰਨ ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_ਨਿਵੇਸ਼) && src.channels() != 2) );

    ਜੇ( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.create_Same_ਮਾਪ( src, CV_MAKETYPE(ਡੂੰਘਾਈ, 2) );
    ਨਹੀਂਤਾਂ ਜੇ( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.create_Same_ਮਾਪ( src, ਡੂੰਘਾਈ );
    ਨਹੀਂਤਾਂ
        _dst.create_Same_ਮਾਪ( src, type );

    Mat dst = _dst.getMat();

    ਪੂਰਨ f = 0;
    ਜੇ (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    ਜੇ (inv)
        f |= CV_HAL_DFT_INVERSE;
    ਜੇ (flags & DFT_ਪੰਕਤੀਆਂ)
        f |= CV_HAL_DFT_ਪੰਕਤੀਆਂ;
    ਜੇ (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    ਜੇ (src.ਅੰਕੜੇ == dst.ਅੰਕੜੇ)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, ਡੂੰਘਾਈ, src.channels(), dst.channels(), f, nonzero_ਪੰਕਤੀਆਂ);
    c->apply(src.ਅੰਕੜੇ, src.ਕਦਮ, dst.ਅੰਕੜੇ, dst.ਕਦਮ);
}


ਖਾਲੀ cv::idft( ਨਿਵੇਸ਼_Array src, OutputArray dst, ਪੂਰਨ flags, ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_ਪੰਕਤੀਆਂ );
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਤਾਰਕਿਕ ocl_mulSpectrums( ਨਿਵੇਸ਼_Array _srcA, ਨਿਵੇਸ਼_Array _srcB,
                              OutputArray _dst, ਪੂਰਨ flags, ਤਾਰਕਿਕ conjB )
{
    ਪੂਰਨ atype = _srcA.type(), btype = _srcB.type(),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ਮਾਪ asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    ਜੇ ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        ਵਾਪਸ ਝੂਠ;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.create_Same_ਮਾਪ(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(A), ocl::KernelArg::Read_Only_No_ਮਾਪ(B),
           ocl::KernelArg::WriteOnly(dst), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI);

    size_t globalsize[2] = { (size_t)asize.ਚੌੜਾਈ, ((size_t)asize.ਉਚਾਈ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

}

#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ {

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VAL(buf, elem) (((T*)((ਅੱਖਰ*)ਅੰਕੜੇ ## buf + (ਕਦਮ ## buf * (elem))))[0])
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MUL_SPECTRUMS_ਖਾਨਾ(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    ਲਈ (size_t j = 1; j <= ਪੰਕਤੀਆਂ - 2; j += 2) \
    { \
        ਦੁੱਗਣਾ a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        ਦੁੱਗਣਾ b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        ਜੇ (conjB) b_im = -b_im; \
        ਦੁੱਗਣਾ c_re = a_re * b_re - a_im * b_im; \
        ਦੁੱਗਣਾ c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    ਜੇ ((ਪੰਕਤੀਆਂ & 1) == 0) \
        VAL(C, ਪੰਕਤੀਆਂ-1) = VAL(A, ਪੰਕਤੀਆਂ-1) * VAL(B, ਪੰਕਤੀਆਂ-1)

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mul_Spectrums_process_ਖਾਨਾ_noinplace(ਅਚਲ T* ਅੰਕੜੇ_A, ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_C, size_t ਕਦਮ_A, size_t ਕਦਮ_B, size_t ਕਦਮ_C, size_t ਪੰਕਤੀਆਂ)
{
    MUL_SPECTRUMS_ਖਾਨਾ(A, B, C);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mul_Spectrums_process_ਖਾਨਾ_inplace_A(ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_AC, size_t ਕਦਮ_B, size_t ਕਦਮ_AC, size_t ਪੰਕਤੀਆਂ)
{
    MUL_SPECTRUMS_ਖਾਨਾ(AC, B, AC);
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB, ਤਾਰਕਿਕ inplaceA> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mul_Spectrums_process_ਖਾਨਾ(ਅਚਲ T* ਅੰਕੜੇ_A, ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_C, size_t ਕਦਮ_A, size_t ਕਦਮ_B, size_t ਕਦਮ_C, size_t ਪੰਕਤੀਆਂ)
{
    ਜੇ (inplaceA)
        mul_Spectrums_process_ਖਾਨਾ_inplace_A<T, conjB>(ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, ਕਦਮ_B, ਕਦਮ_C, ਪੰਕਤੀਆਂ);
    ਨਹੀਂਤਾਂ
        mul_Spectrums_process_ਖਾਨਾ_noinplace<T, conjB>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, ਕਦਮ_A, ਕਦਮ_B, ਕਦਮ_C, ਪੰਕਤੀਆਂ);
}
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MUL_SPECTRUMS_ਖਾਨਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ VAL

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB, ਤਾਰਕਿਕ inplaceA> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mul_Spectrums_process_ਖਾਨੇ_ਗਿਣਤੀ(ਅਚਲ T* ਅੰਕੜੇ_A, ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_C, size_t ਕਦਮ_A, size_t ਕਦਮ_B, size_t ਕਦਮ_C, size_t ਪੰਕਤੀਆਂ, size_t ਖਾਨੇ_ਗਿਣਤੀ)
{
    mul_Spectrums_process_ਖਾਨਾ<T, conjB, inplaceA>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, ਕਦਮ_A, ਕਦਮ_B, ਕਦਮ_C, ਪੰਕਤੀਆਂ);
    ਜੇ ((ਖਾਨੇ_ਗਿਣਤੀ & 1) == 0)
    {
        mul_Spectrums_process_ਖਾਨਾ<T, conjB, inplaceA>(ਅੰਕੜੇ_A + ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_B + ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_C + ਖਾਨੇ_ਗਿਣਤੀ - 1, ਕਦਮ_A, ਕਦਮ_B, ਕਦਮ_C, ਪੰਕਤੀਆਂ);
    }
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VAL(buf, elem) (ਅੰਕੜੇ ## buf[(elem)])
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MUL_SPECTRUMS_ਪੰਕਤੀ(A, B, C) \
    ਲਈ (size_t j = j0; j < j1; j += 2) \
    { \
        ਦੁੱਗਣਾ a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        ਦੁੱਗਣਾ b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        ਜੇ (conjB) b_im = -b_im; \
        ਦੁੱਗਣਾ c_re = a_re * b_re - a_im * b_im; \
        ਦੁੱਗਣਾ c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mul_Spectrums_process_ਪੰਕਤੀ_noinplace(ਅਚਲ T* ਅੰਕੜੇ_A, ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_C, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ਪੰਕਤੀ(A, B, C);
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mul_Spectrums_process_ਪੰਕਤੀ_inplace_A(ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_AC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ਪੰਕਤੀ(AC, B, AC);
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB, ਤਾਰਕਿਕ inplaceA> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mul_Spectrums_process_ਪੰਕਤੀ(ਅਚਲ T* ਅੰਕੜੇ_A, ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_C, size_t j0, size_t j1)
{
    ਜੇ (inplaceA)
        mul_Spectrums_process_ਪੰਕਤੀ_inplace_A<T, conjB>(ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, j0, j1);
    ਨਹੀਂਤਾਂ
        mul_Spectrums_process_ਪੰਕਤੀ_noinplace<T, conjB>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, j0, j1);
}
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MUL_SPECTRUMS_ਪੰਕਤੀ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ VAL

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB, ਤਾਰਕਿਕ inplaceA> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mul_Spectrums_process_ਪੰਕਤੀਆਂ(ਅਚਲ T* ਅੰਕੜੇ_A, ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_C, size_t ਕਦਮ_A, size_t ਕਦਮ_B, size_t ਕਦਮ_C, size_t ਪੰਕਤੀਆਂ, size_t ਖਾਨੇ_ਗਿਣਤੀ, size_t j0, size_t j1, ਤਾਰਕਿਕ is_1d_CN1)
{
    ਜਦੋਂ (ਪੰਕਤੀਆਂ-- > 0)
    {
        ਜੇ (is_1d_CN1)
            ਅੰਕੜੇ_C[0] = ਅੰਕੜੇ_A[0]*ਅੰਕੜੇ_B[0];
        mul_Spectrums_process_ਪੰਕਤੀ<T, conjB, inplaceA>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, j0, j1);
        ਜੇ (is_1d_CN1 && (ਖਾਨੇ_ਗਿਣਤੀ & 1) == 0)
            ਅੰਕੜੇ_C[j1] = ਅੰਕੜੇ_A[j1]*ਅੰਕੜੇ_B[j1];

        ਅੰਕੜੇ_A = (ਅਚਲ T*)(((ਅੱਖਰ*)ਅੰਕੜੇ_A) + ਕਦਮ_A);
        ਅੰਕੜੇ_B = (ਅਚਲ T*)(((ਅੱਖਰ*)ਅੰਕੜੇ_B) + ਕਦਮ_B);
        ਅੰਕੜੇ_C =       (T*)(((ਅੱਖਰ*)ਅੰਕੜੇ_C) + ਕਦਮ_C);
    }
}


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB, ਤਾਰਕਿਕ inplaceA> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mulSpectrums_Impl_(ਅਚਲ T* ਅੰਕੜੇ_A, ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_C, size_t ਕਦਮ_A, size_t ਕਦਮ_B, size_t ਕਦਮ_C, size_t ਪੰਕਤੀਆਂ, size_t ਖਾਨੇ_ਗਿਣਤੀ, size_t j0, size_t j1, ਤਾਰਕਿਕ is_1d, ਤਾਰਕਿਕ isCN1)
{
    ਜੇ (!is_1d && isCN1)
    {
        mul_Spectrums_process_ਖਾਨੇ_ਗਿਣਤੀ<T, conjB, inplaceA>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, ਕਦਮ_A, ਕਦਮ_B, ਕਦਮ_C, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ);
    }
    mul_Spectrums_process_ਪੰਕਤੀਆਂ<T, conjB, inplaceA>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, ਕਦਮ_A, ਕਦਮ_B, ਕਦਮ_C, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, j0, j1, is_1d && isCN1);
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਤਾਰਕਿਕ conjB> ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ mulSpectrums_Impl(ਅਚਲ T* ਅੰਕੜੇ_A, ਅਚਲ T* ਅੰਕੜੇ_B, T* ਅੰਕੜੇ_C, size_t ਕਦਮ_A, size_t ਕਦਮ_B, size_t ਕਦਮ_C, size_t ਪੰਕਤੀਆਂ, size_t ਖਾਨੇ_ਗਿਣਤੀ, size_t j0, size_t j1, ਤਾਰਕਿਕ is_1d, ਤਾਰਕਿਕ isCN1)
{
    ਜੇ (ਅੰਕੜੇ_A == ਅੰਕੜੇ_C)
        mulSpectrums_Impl_<T, conjB, ਸੱਚ>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, ਕਦਮ_A, ਕਦਮ_B, ਕਦਮ_C, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, j0, j1, is_1d, isCN1);
    ਨਹੀਂਤਾਂ
        mulSpectrums_Impl_<T, conjB, ਝੂਠ>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, ਕਦਮ_A, ਕਦਮ_B, ਕਦਮ_C, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, j0, j1, is_1d, isCN1);
}

} // namespace

ਖਾਲੀ cv::mulSpectrums( ਨਿਵੇਸ਼_Array _srcA, ਨਿਵੇਸ਼_Array _srcB,
                       OutputArray _dst, ਪੂਰਨ flags, ਤਾਰਕਿਕ conjB )
{
    CV_INSTRUMENT_REGION();

    ਜੇ (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        ਵਾਪਸ;
    }

    ਪੂਰਨ type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    ਪੂਰਨ ਡੂੰਘਾਈ = srcA.ਡੂੰਘਾਈ(), cn = srcA.channels();
    size_t ਪੰਕਤੀਆਂ = srcA.ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ = srcA.ਖਾਨੇ_ਗਿਣਤੀ;

    _dst.create( srcA.ਪੰਕਤੀਆਂ, srcA.ਖਾਨੇ_ਗਿਣਤੀ, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    ਜੇ (dst.ਅੰਕੜੇ == srcB.ਅੰਕੜੇ)
        srcB = srcB.clone(); // workaround for B only

    ਤਾਰਕਿਕ is_1d = (flags & DFT_ਪੰਕਤੀਆਂ)
        || (ਪੰਕਤੀਆਂ == 1)
        || (ਖਾਨੇ_ਗਿਣਤੀ == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    ਜੇ( is_1d && !(flags & DFT_ਪੰਕਤੀਆਂ) )
        ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨੇ_ਗਿਣਤੀ + ਪੰਕਤੀਆਂ - 1, ਪੰਕਤੀਆਂ = 1;

    ਤਾਰਕਿਕ isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = ਖਾਨੇ_ਗਿਣਤੀ*cn - (((ਖਾਨੇ_ਗਿਣਤੀ & 1) == 0 && cn == 1) ? 1 : 0);

    ਜੇ (ਡੂੰਘਾਈ == CV_32F)
    {
        ਅਚਲ ਦਸ਼ਮਲਵ* ਅੰਕੜੇ_A = srcA.ptr<ਦਸ਼ਮਲਵ>();
        ਅਚਲ ਦਸ਼ਮਲਵ* ਅੰਕੜੇ_B = srcB.ptr<ਦਸ਼ਮਲਵ>();
        ਦਸ਼ਮਲਵ* ਅੰਕੜੇ_C = dst.ptr<ਦਸ਼ਮਲਵ>();
        ਜੇ (!conjB)
            mulSpectrums_Impl<ਦਸ਼ਮਲਵ, ਝੂਠ>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, srcA.ਕਦਮ, srcB.ਕਦਮ, dst.ਕਦਮ, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, j0, j1, is_1d, isCN1);
        ਨਹੀਂਤਾਂ
            mulSpectrums_Impl<ਦਸ਼ਮਲਵ, ਸੱਚ>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, srcA.ਕਦਮ, srcB.ਕਦਮ, dst.ਕਦਮ, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, j0, j1, is_1d, isCN1);
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ ਦੁੱਗਣਾ* ਅੰਕੜੇ_A = srcA.ptr<ਦੁੱਗਣਾ>();
        ਅਚਲ ਦੁੱਗਣਾ* ਅੰਕੜੇ_B = srcB.ptr<ਦੁੱਗਣਾ>();
        ਦੁੱਗਣਾ* ਅੰਕੜੇ_C = dst.ptr<ਦੁੱਗਣਾ>();
        ਜੇ (!conjB)
            mulSpectrums_Impl<ਦੁੱਗਣਾ, ਝੂਠ>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, srcA.ਕਦਮ, srcB.ਕਦਮ, dst.ਕਦਮ, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, j0, j1, is_1d, isCN1);
        ਨਹੀਂਤਾਂ
            mulSpectrums_Impl<ਦੁੱਗਣਾ, ਸੱਚ>(ਅੰਕੜੇ_A, ਅੰਕੜੇ_B, ਅੰਕੜੇ_C, srcA.ਕਦਮ, srcB.ਕਦਮ, dst.ਕਦਮ, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, j0, j1, is_1d, isCN1);
    }
}

ਖਾਲੀ cv::divSpectrums( ਨਿਵੇਸ਼_Array _srcA, ਨਿਵੇਸ਼_Array _srcB, OutputArray _dst, ਪੂਰਨ flags, ਤਾਰਕਿਕ conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    ਜੇ (srcA.empty() && srcB.empty())
    {
        _dst.release();
        ਵਾਪਸ;
    }
    ਪੂਰਨ ਡੂੰਘਾਈ = srcA.ਡੂੰਘਾਈ(), cn = srcA.channels(), type = srcA.type();
    ਪੂਰਨ ਪੰਕਤੀਆਂ = srcA.ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ = srcA.ਖਾਨੇ_ਗਿਣਤੀ;
    ਪੂਰਨ j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.ਪੰਕਤੀਆਂ, srcA.ਖਾਨੇ_ਗਿਣਤੀ, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.ਅੰਕੜੇ != srcA.ਅੰਕੜੇ); // non-inplace check
    CV_Assert(dst.ਅੰਕੜੇ != srcB.ਅੰਕੜੇ); // non-inplace check

    ਤਾਰਕਿਕ is_1d = (flags & DFT_ਪੰਕਤੀਆਂ) || (ਪੰਕਤੀਆਂ == 1 || (ਖਾਨੇ_ਗਿਣਤੀ == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    ਜੇ( is_1d && !(flags & DFT_ਪੰਕਤੀਆਂ) )
        ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨੇ_ਗਿਣਤੀ + ਪੰਕਤੀਆਂ - 1, ਪੰਕਤੀਆਂ = 1;

    ਪੂਰਨ ncols = ਖਾਨੇ_ਗਿਣਤੀ*cn;
    ਪੂਰਨ j0 = cn == 1;
    ਪੂਰਨ j1 = ncols - (ਖਾਨੇ_ਗਿਣਤੀ % 2 == 0 && cn == 1);

    ਜੇ( ਡੂੰਘਾਈ == CV_32F )
    {
        ਅਚਲ ਦਸ਼ਮਲਵ* ਅੰਕੜੇ_A = srcA.ptr<ਦਸ਼ਮਲਵ>();
        ਅਚਲ ਦਸ਼ਮਲਵ* ਅੰਕੜੇ_B = srcB.ptr<ਦਸ਼ਮਲਵ>();
        ਦਸ਼ਮਲਵ* ਅੰਕੜੇ_C = dst.ptr<ਦਸ਼ਮਲਵ>();
        ਦਸ਼ਮਲਵ eps = FLT_EPSILON; // prevent div0 problems

        size_t ਕਦਮ_A = srcA.ਕਦਮ/ਆਕਾਰ(ਅੰਕੜੇ_A[0]);
        size_t ਕਦਮ_B = srcB.ਕਦਮ/ਆਕਾਰ(ਅੰਕੜੇ_B[0]);
        size_t ਕਦਮ_C = dst.ਕਦਮ/ਆਕਾਰ(ਅੰਕੜੇ_C[0]);

        ਜੇ( !is_1d && cn == 1 )
        {
            ਲਈ( k = 0; k < (ਖਾਨੇ_ਗਿਣਤੀ % 2 ? 1 : 2); k++ )
            {
                ਜੇ( k == 1 )
                    ਅੰਕੜੇ_A += ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_B += ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_C += ਖਾਨੇ_ਗਿਣਤੀ - 1;
                ਅੰਕੜੇ_C[0] = ਅੰਕੜੇ_A[0] / (ਅੰਕੜੇ_B[0] + eps);
                ਜੇ( ਪੰਕਤੀਆਂ % 2 == 0 )
                    ਅੰਕੜੇ_C[(ਪੰਕਤੀਆਂ-1)*ਕਦਮ_C] = ਅੰਕੜੇ_A[(ਪੰਕਤੀਆਂ-1)*ਕਦਮ_A] / (ਅੰਕੜੇ_B[(ਪੰਕਤੀਆਂ-1)*ਕਦਮ_B] + eps);
                ਜੇ( !conjB )
                    ਲਈ( j = 1; j <= ਪੰਕਤੀਆਂ - 2; j += 2 )
                    {
                        ਦੁੱਗਣਾ denom = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_B[j*ਕਦਮ_B]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] +
                        (ਦੁੱਗਣਾ)ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B] + (ਦੁੱਗਣਾ)eps;

                        ਦੁੱਗਣਾ re = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j*ਕਦਮ_A]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] +
                        (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[(j+1)*ਕਦਮ_A]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B];

                        ਦੁੱਗਣਾ im = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[(j+1)*ਕਦਮ_A]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] -
                        (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j*ਕਦਮ_A]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B];

                        ਅੰਕੜੇ_C[j*ਕਦਮ_C] = (ਦਸ਼ਮਲਵ)(re / denom);
                        ਅੰਕੜੇ_C[(j+1)*ਕਦਮ_C] = (ਦਸ਼ਮਲਵ)(im / denom);
                    }
                    ਨਹੀਂਤਾਂ
                        ਲਈ( j = 1; j <= ਪੰਕਤੀਆਂ - 2; j += 2 )
                        {

                            ਦੁੱਗਣਾ denom = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_B[j*ਕਦਮ_B]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] +
                            (ਦੁੱਗਣਾ)ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B] + (ਦੁੱਗਣਾ)eps;

                            ਦੁੱਗਣਾ re = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j*ਕਦਮ_A]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] -
                            (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[(j+1)*ਕਦਮ_A]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B];

                            ਦੁੱਗਣਾ im = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[(j+1)*ਕਦਮ_A]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] +
                            (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j*ਕਦਮ_A]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B];

                            ਅੰਕੜੇ_C[j*ਕਦਮ_C] = (ਦਸ਼ਮਲਵ)(re / denom);
                            ਅੰਕੜੇ_C[(j+1)*ਕਦਮ_C] = (ਦਸ਼ਮਲਵ)(im / denom);
                        }
                    ਜੇ( k == 1 )
                        ਅੰਕੜੇ_A -= ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_B -= ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_C -= ਖਾਨੇ_ਗਿਣਤੀ - 1;
            }
        }

        ਲਈ( ; ਪੰਕਤੀਆਂ--; ਅੰਕੜੇ_A += ਕਦਮ_A, ਅੰਕੜੇ_B += ਕਦਮ_B, ਅੰਕੜੇ_C += ਕਦਮ_C )
        {
            ਜੇ( is_1d && cn == 1 )
            {
                ਅੰਕੜੇ_C[0] = ਅੰਕੜੇ_A[0] / (ਅੰਕੜੇ_B[0] + eps);
                ਜੇ( ਖਾਨੇ_ਗਿਣਤੀ % 2 == 0 )
                    ਅੰਕੜੇ_C[j1] = ਅੰਕੜੇ_A[j1] / (ਅੰਕੜੇ_B[j1] + eps);
            }

            ਜੇ( !conjB )
                ਲਈ( j = j0; j < j1; j += 2 )
                {
                    ਦੁੱਗਣਾ denom = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_B[j]*ਅੰਕੜੇ_B[j] + (ਦੁੱਗਣਾ)ਅੰਕੜੇ_B[j+1]*ਅੰਕੜੇ_B[j+1] + (ਦੁੱਗਣਾ)eps;
                    ਦੁੱਗਣਾ re = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j]*ਅੰਕੜੇ_B[j] + (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j+1]*ਅੰਕੜੇ_B[j+1];
                    ਦੁੱਗਣਾ im = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j+1]*ਅੰਕੜੇ_B[j] - (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j]*ਅੰਕੜੇ_B[j+1];
                    ਅੰਕੜੇ_C[j] = (ਦਸ਼ਮਲਵ)(re / denom);
                    ਅੰਕੜੇ_C[j+1] = (ਦਸ਼ਮਲਵ)(im / denom);
                }
                ਨਹੀਂਤਾਂ
                    ਲਈ( j = j0; j < j1; j += 2 )
                    {
                        ਦੁੱਗਣਾ denom = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_B[j]*ਅੰਕੜੇ_B[j] + (ਦੁੱਗਣਾ)ਅੰਕੜੇ_B[j+1]*ਅੰਕੜੇ_B[j+1] + (ਦੁੱਗਣਾ)eps;
                        ਦੁੱਗਣਾ re = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j]*ਅੰਕੜੇ_B[j] - (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j+1]*ਅੰਕੜੇ_B[j+1];
                        ਦੁੱਗਣਾ im = (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j+1]*ਅੰਕੜੇ_B[j] + (ਦੁੱਗਣਾ)ਅੰਕੜੇ_A[j]*ਅੰਕੜੇ_B[j+1];
                        ਅੰਕੜੇ_C[j] = (ਦਸ਼ਮਲਵ)(re / denom);
                        ਅੰਕੜੇ_C[j+1] = (ਦਸ਼ਮਲਵ)(im / denom);
                    }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ ਦੁੱਗਣਾ* ਅੰਕੜੇ_A = srcA.ptr<ਦੁੱਗਣਾ>();
        ਅਚਲ ਦੁੱਗਣਾ* ਅੰਕੜੇ_B = srcB.ptr<ਦੁੱਗਣਾ>();
        ਦੁੱਗਣਾ* ਅੰਕੜੇ_C = dst.ptr<ਦੁੱਗਣਾ>();
        ਦੁੱਗਣਾ eps = DBL_EPSILON; // prevent div0 problems

        size_t ਕਦਮ_A = srcA.ਕਦਮ/ਆਕਾਰ(ਅੰਕੜੇ_A[0]);
        size_t ਕਦਮ_B = srcB.ਕਦਮ/ਆਕਾਰ(ਅੰਕੜੇ_B[0]);
        size_t ਕਦਮ_C = dst.ਕਦਮ/ਆਕਾਰ(ਅੰਕੜੇ_C[0]);

        ਜੇ( !is_1d && cn == 1 )
        {
            ਲਈ( k = 0; k < (ਖਾਨੇ_ਗਿਣਤੀ % 2 ? 1 : 2); k++ )
            {
                ਜੇ( k == 1 )
                    ਅੰਕੜੇ_A += ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_B += ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_C += ਖਾਨੇ_ਗਿਣਤੀ - 1;
                ਅੰਕੜੇ_C[0] = ਅੰਕੜੇ_A[0] / (ਅੰਕੜੇ_B[0] + eps);
                ਜੇ( ਪੰਕਤੀਆਂ % 2 == 0 )
                    ਅੰਕੜੇ_C[(ਪੰਕਤੀਆਂ-1)*ਕਦਮ_C] = ਅੰਕੜੇ_A[(ਪੰਕਤੀਆਂ-1)*ਕਦਮ_A] / (ਅੰਕੜੇ_B[(ਪੰਕਤੀਆਂ-1)*ਕਦਮ_B] + eps);
                ਜੇ( !conjB )
                    ਲਈ( j = 1; j <= ਪੰਕਤੀਆਂ - 2; j += 2 )
                    {
                        ਦੁੱਗਣਾ denom = ਅੰਕੜੇ_B[j*ਕਦਮ_B]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] +
                        ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B] + eps;

                        ਦੁੱਗਣਾ re = ਅੰਕੜੇ_A[j*ਕਦਮ_A]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] +
                        ਅੰਕੜੇ_A[(j+1)*ਕਦਮ_A]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B];

                        ਦੁੱਗਣਾ im = ਅੰਕੜੇ_A[(j+1)*ਕਦਮ_A]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] -
                        ਅੰਕੜੇ_A[j*ਕਦਮ_A]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B];

                        ਅੰਕੜੇ_C[j*ਕਦਮ_C] = re / denom;
                        ਅੰਕੜੇ_C[(j+1)*ਕਦਮ_C] = im / denom;
                    }
                    ਨਹੀਂਤਾਂ
                        ਲਈ( j = 1; j <= ਪੰਕਤੀਆਂ - 2; j += 2 )
                        {
                            ਦੁੱਗਣਾ denom = ਅੰਕੜੇ_B[j*ਕਦਮ_B]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] +
                            ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B] + eps;

                            ਦੁੱਗਣਾ re = ਅੰਕੜੇ_A[j*ਕਦਮ_A]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] -
                            ਅੰਕੜੇ_A[(j+1)*ਕਦਮ_A]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B];

                            ਦੁੱਗਣਾ im = ਅੰਕੜੇ_A[(j+1)*ਕਦਮ_A]*ਅੰਕੜੇ_B[j*ਕਦਮ_B] +
                            ਅੰਕੜੇ_A[j*ਕਦਮ_A]*ਅੰਕੜੇ_B[(j+1)*ਕਦਮ_B];

                            ਅੰਕੜੇ_C[j*ਕਦਮ_C] = re / denom;
                            ਅੰਕੜੇ_C[(j+1)*ਕਦਮ_C] = im / denom;
                        }
                    ਜੇ( k == 1 )
                        ਅੰਕੜੇ_A -= ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_B -= ਖਾਨੇ_ਗਿਣਤੀ - 1, ਅੰਕੜੇ_C -= ਖਾਨੇ_ਗਿਣਤੀ - 1;
            }
        }

        ਲਈ( ; ਪੰਕਤੀਆਂ--; ਅੰਕੜੇ_A += ਕਦਮ_A, ਅੰਕੜੇ_B += ਕਦਮ_B, ਅੰਕੜੇ_C += ਕਦਮ_C )
        {
            ਜੇ( is_1d && cn == 1 )
            {
                ਅੰਕੜੇ_C[0] = ਅੰਕੜੇ_A[0] / (ਅੰਕੜੇ_B[0] + eps);
                ਜੇ( ਖਾਨੇ_ਗਿਣਤੀ % 2 == 0 )
                    ਅੰਕੜੇ_C[j1] = ਅੰਕੜੇ_A[j1] / (ਅੰਕੜੇ_B[j1] + eps);
            }

            ਜੇ( !conjB )
                ਲਈ( j = j0; j < j1; j += 2 )
                {
                    ਦੁੱਗਣਾ denom = ਅੰਕੜੇ_B[j]*ਅੰਕੜੇ_B[j] + ਅੰਕੜੇ_B[j+1]*ਅੰਕੜੇ_B[j+1] + eps;
                    ਦੁੱਗਣਾ re = ਅੰਕੜੇ_A[j]*ਅੰਕੜੇ_B[j] + ਅੰਕੜੇ_A[j+1]*ਅੰਕੜੇ_B[j+1];
                    ਦੁੱਗਣਾ im = ਅੰਕੜੇ_A[j+1]*ਅੰਕੜੇ_B[j] - ਅੰਕੜੇ_A[j]*ਅੰਕੜੇ_B[j+1];
                    ਅੰਕੜੇ_C[j] = re / denom;
                    ਅੰਕੜੇ_C[j+1] = im / denom;
                }
                ਨਹੀਂਤਾਂ
                    ਲਈ( j = j0; j < j1; j += 2 )
                    {
                        ਦੁੱਗਣਾ denom = ਅੰਕੜੇ_B[j]*ਅੰਕੜੇ_B[j] + ਅੰਕੜੇ_B[j+1]*ਅੰਕੜੇ_B[j+1] + eps;
                        ਦੁੱਗਣਾ re = ਅੰਕੜੇ_A[j]*ਅੰਕੜੇ_B[j] - ਅੰਕੜੇ_A[j+1]*ਅੰਕੜੇ_B[j+1];
                        ਦੁੱਗਣਾ im = ਅੰਕੜੇ_A[j+1]*ਅੰਕੜੇ_B[j] + ਅੰਕੜੇ_A[j]*ਅੰਕੜੇ_B[j+1];
                        ਅੰਕੜੇ_C[j] = re / denom;
                        ਅੰਕੜੇ_C[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
DCT( ਅਚਲ OcvDftOptions & c, ਅਚਲ T* src, size_t src_ਕਦਮ, T* dst, size_t dst_ਕਦਮ )
{
    ਜੇ( c.n == 1 )
    {
        dst[0] = src[0];
        ਵਾਪਸ;
    }
    ਲੜੀਵਾਰ_Dft(c, src, src_ਕਦਮ/ਆਕਾਰ(T), dst, dst_ਕਦਮ/ਆਕਾਰ(T), ਝੂਠ);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
IDCT( ਅਚਲ OcvDftOptions & c, ਅਚਲ T* src, size_t src_ਕਦਮ, T* dst, size_t dst_ਕਦਮ )
{
    ਜੇ( c.n == 1 )
    {
        dst[0] = src[0];
        ਵਾਪਸ;
    }
    ਲੜੀਵਾਰ_Dft(c, src, src_ਕਦਮ/ਆਕਾਰ(T), dst, dst_ਕਦਮ/ਆਕਾਰ(T), ਝੂਠ);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*DCTFunc)(ਅਚਲ OcvDftOptions & c, ਅਚਲ ਖਾਲੀ* src, size_t src_ਕਦਮ,
                        ਖਾਲੀ* dst, size_t dst_ਕਦਮ);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਖਾਲੀ (*fn)(ਅਚਲ OcvDftOptions&, ਅਚਲ T*, size_t, T*, size_t)>
ਸਥਿਰ ਖਾਲੀ dctWrap(ਅਚਲ OcvDftOptions & c, ਅਚਲ ਖਾਲੀ* src, size_t src_ਕਦਮ, ਖਾਲੀ* dst, size_t dst_ਕਦਮ)
{
    fn(c, (ਅਚਲ T*)src, src_ਕਦਮ, (T*)dst, dst_ਕਦਮ);
}

}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
ਨਾਮਾਕਾਸ਼ cv
{

#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 900
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * ippiDCTFunc)(ਅਚਲ Ipp32f* pSrc, ਪੂਰਨ src_ਕਦਮ_3, Ipp32f* pDst, ਪੂਰਨ dst_ਕਦਮ_3, ਅਚਲ ਖਾਲੀ* pDCTSpec, Ipp8u* p_ਬਫਰ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * ippiDCTInit)(ਖਾਲੀ* pDCTSpec, Ippi_ਮਾਪ roi_ਮਾਪ, Ipp8u* pMemInit );
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * ippi_DCTGet_ਮਾਪ)(Ippi_ਮਾਪ roi_ਮਾਪ, ਪੂਰਨ* p_ਮਾਪ_Spec, ਪੂਰਨ* p_ਮਾਪ_Init, ਪੂਰਨ* p_ਮਾਪ_Buf);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SpecType, Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(ਅਚਲ Ipp32f*, ਪੂਰਨ, Ipp32f*, ਪੂਰਨ, ਅਚਲ SpecType*, Ipp8u*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ippiDCTWrap(ਅਚਲ Ipp32f* pSrc, ਪੂਰਨ src_ਕਦਮ_3, Ipp32f* pDst, ਪੂਰਨ dst_ਕਦਮ_3, ਅਚਲ ਖਾਲੀ* pDCTSpec, Ipp8u* p_ਬਫਰ)
{
    ਵਾਪਸ fn(pSrc, src_ਕਦਮ_3, pDst, dst_ਕਦਮ_3, (ਅਚਲ SpecType*)pDCTSpec, p_ਬਫਰ);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SpecType, Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(SpecType*, Ippi_ਮਾਪ, Ipp8u*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ippiDCTInitWrap(ਖਾਲੀ* pDCTSpec, Ippi_ਮਾਪ roi_ਮਾਪ, Ipp8u* pMemInit)
{
    ਵਾਪਸ fn((SpecType*)pDCTSpec, roi_ਮਾਪ, pMemInit);
}

ਸਾਂਚਾ<Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(Ippi_ਮਾਪ, ਪੂਰਨ*, ਪੂਰਨ*, ਪੂਰਨ*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ippi_DCTGet_ਮਾਪ_Wrap(Ippi_ਮਾਪ roi_ਮਾਪ, ਪੂਰਨ* p_ਮਾਪ_Spec, ਪੂਰਨ* p_ਮਾਪ_Init, ਪੂਰਨ* p_ਮਾਪ_Buf)
{
    ਵਾਪਸ fn(roi_ਮਾਪ, p_ਮਾਪ_Spec, p_ਮਾਪ_Init, p_ਮਾਪ_Buf);
}

#ਜਾਂਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 700
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * ippiDCTFunc)(ਅਚਲ Ipp32f*, ਪੂਰਨ, Ipp32f*, ਪੂਰਨ, ਅਚਲ ਖਾਲੀ*, Ipp8u*);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * ippiDCTInitAlloc)(ਖਾਲੀ**, Ippi_ਮਾਪ, IppHintAlgorithm);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * ippiDCTFree)(ਖਾਲੀ* pDCTSpec);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * ippi_DCTGet_Buf_ਮਾਪ)(ਅਚਲ ਖਾਲੀ*, ਪੂਰਨ*);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SpecType, Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(ਅਚਲ Ipp32f*, ਪੂਰਨ, Ipp32f*, ਪੂਰਨ, ਅਚਲ SpecType*, Ipp8u*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ippiDCTWrap(ਅਚਲ Ipp32f* pSrc, ਪੂਰਨ src_ਕਦਮ_3, Ipp32f* pDst, ਪੂਰਨ dst_ਕਦਮ_3, ਅਚਲ ਖਾਲੀ* pDCTSpec, Ipp8u* p_ਬਫਰ)
{
    ਵਾਪਸ fn(pSrc, src_ਕਦਮ_3, pDst, dst_ਕਦਮ_3, (ਅਚਲ SpecType*)pDCTSpec, p_ਬਫਰ);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SpecType, Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(SpecType**, Ippi_ਮਾਪ, IppHintAlgorithm)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ippiDCTInitAllocWrap(ਖਾਲੀ** pDCTSpec, Ippi_ਮਾਪ roi_ਮਾਪ, IppHintAlgorithm hint)
{
    ਵਾਪਸ fn((SpecType**)pDCTSpec, roi_ਮਾਪ, hint);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SpecType, Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(SpecType*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ippiDCTFreeWrap(ਖਾਲੀ* pDCTSpec)
{
    ਵਾਪਸ fn((SpecType*)pDCTSpec);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SpecType, Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(ਅਚਲ SpecType*, ਪੂਰਨ*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ippi_DCTGet_Buf_ਮਾਪ_Wrap(ਅਚਲ ਖਾਲੀ* pDCTSpec, ਪੂਰਨ* p_ਮਾਪ)
{
    ਵਾਪਸ fn((ਅਚਲ SpecType*)pDCTSpec, p_ਮਾਪ);
}
#ਜੇ_ਅੰਤ

ਵਰਗ DctIPPLoop_Invoker : ਜਨਤਕ ParallelLoopBody
{
ਜਨਤਕ:
    DctIPPLoop_Invoker(ਅਚਲ uchar * _src, size_t src_ਕਦਮ_2, uchar * _dst, size_t dst_ਕਦਮ_2, ਪੂਰਨ ਚੌੜਾਈ_3, ਤਾਰਕਿਕ _inv, ਤਾਰਕਿਕ *_ok) :
        ParallelLoopBody(), src(_src), src_ਕਦਮ(src_ਕਦਮ_2), dst(_dst), dst_ਕਦਮ(dst_ਕਦਮ_2), ਚੌੜਾਈ(ਚੌੜਾਈ_3), inv(_inv), ok(_ok)
    {
        *ok = ਸੱਚ;
    }

    ਆਭਾਸੀ ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ Range& range) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(*ok == ਝੂਠ)
            ਵਾਪਸ;

#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 900
        Ippi_ਮਾਪ src_Roi_ਮਾਪ = {ਚੌੜਾਈ, 1};

        ਪੂਰਨ spec_ਮਾਪ    = 0;
        ਪੂਰਨ init_ਮਾਪ    = 0;
        ਪੂਰਨ ਬਫਰ_ਮਾਪ  = 0;

        Ipp8u* pDCTSpec = ਨਿਰਾ;
        Ipp8u* p_ਬਫਰ  = ਨਿਰਾ;
        Ipp8u* pInitBuf = ਨਿਰਾ;

        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IPP_RETURN              \
            ਜੇ(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            ਜੇ(p_ਬਫਰ)                 \
                ippFree(p_ਬਫਰ);       \
            ਜੇ(pInitBuf)                \
                ippFree(pInitBuf);      \
            ਵਾਪਸ;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_ਮਾਪ  ipp_Dct_Get_ਮਾਪ  = inv ? ippi_DCTGet_ਮਾਪ_Wrap<ippi_DCTInv_Get_ਮਾਪ_32f> : ippi_DCTGet_ਮਾਪ_Wrap<ippi_DCTFwd_Get_ਮਾਪ_32f>;

        ਜੇ(ipp_Dct_Get_ਮਾਪ(src_Roi_ਮਾਪ, &spec_ਮਾਪ, &init_ਮਾਪ, &ਬਫਰ_ਮਾਪ) < 0)
        {
            *ok = ਝੂਠ;
            ਵਾਪਸ;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_ਮਾਪ);
        ਜੇ(!pDCTSpec && spec_ਮਾਪ)
        {
            *ok = ਝੂਠ;
            ਵਾਪਸ;
        }

        p_ਬਫਰ  = (Ipp8u*)CV_IPP_MALLOC(ਬਫਰ_ਮਾਪ);
        ਜੇ(!p_ਬਫਰ && ਬਫਰ_ਮਾਪ)
        {
            *ok = ਝੂਠ;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_ਮਾਪ);
        ਜੇ(!pInitBuf && init_ਮਾਪ)
        {
            *ok = ਝੂਠ;
            IPP_RETURN
        }

        ਜੇ(ippDctInit(pDCTSpec, src_Roi_ਮਾਪ, pInitBuf) < 0)
        {
            *ok = ਝੂਠ;
            IPP_RETURN
        }

        ਲਈ(ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; ++i)
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (ਦਸ਼ਮਲਵ*)(src + src_ਕਦਮ * i), static_cast<ਪੂਰਨ>(src_ਕਦਮ), (ਦਸ਼ਮਲਵ*)(dst + dst_ਕਦਮ * i), static_cast<ਪੂਰਨ>(dst_ਕਦਮ), pDCTSpec, p_ਬਫਰ) < 0)
            {
                *ok = ਝੂਠ;
                IPP_RETURN
            }
        }
        IPP_RETURN
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ IPP_RETURN
#ਜਾਂਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 700
        ਖਾਲੀ* pDCTSpec;
        Auto_ਬਫਰ<uchar> buf;
        uchar* p_ਬਫਰ = 0;
        ਪੂਰਨ buf_ਮਾਪ=0;

        Ippi_ਮਾਪ src_Roi_ਮਾਪ = {ਚੌੜਾਈ, 1};

        CV_SUPPRESS_DEPRECATED_ਸ਼ੁਰੂ

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_ਮਾਪ ipp_Get_Buf_ਮਾਪ = inv ? ippi_DCTGet_Buf_ਮਾਪ_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_ਮਾਪ_32f> : ippi_DCTGet_Buf_ਮਾਪ_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_ਮਾਪ_32f>;

        ਜੇ (ippInitAlloc(&pDCTSpec, src_Roi_ਮਾਪ, ippAlgHintNone)>=0 && ipp_Get_Buf_ਮਾਪ(pDCTSpec, &buf_ਮਾਪ)>=0)
        {
            buf.allocate( buf_ਮਾਪ );
            p_ਬਫਰ = (uchar*)buf;

            ਲਈ( ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; ++i)
            {
                ਜੇ(ippDctFun((ਦਸ਼ਮਲਵ*)(src + src_ਕਦਮ * i), static_cast<ਪੂਰਨ>(src_ਕਦਮ), (ਦਸ਼ਮਲਵ*)(dst + dst_ਕਦਮ * i), static_cast<ਪੂਰਨ>(dst_ਕਦਮ), pDCTSpec, (Ipp8u*)p_ਬਫਰ) < 0)
                {
                    *ok = ਝੂਠ;
                    ਰੋਕੋ;
                }
            }
        }
        ਨਹੀਂਤਾਂ
            *ok = ਝੂਠ;

        ਜੇ (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_ਸਿਰਾ
#ਨਹੀਂਤਾਂ
        CV_ਅਣਵਰਤਿਆ(range);
        *ok = ਝੂਠ;
#ਜੇ_ਅੰਤ
    }

ਨਿਜੀ:
    ਅਚਲ uchar * src;
    size_t src_ਕਦਮ;
    uchar * dst;
    size_t dst_ਕਦਮ;
    ਪੂਰਨ ਚੌੜਾਈ;
    ਤਾਰਕਿਕ inv;
    ਤਾਰਕਿਕ *ok;
};

ਸਥਿਰ ਤਾਰਕਿਕ DctIPPLoop(ਅਚਲ uchar * src, size_t src_ਕਦਮ, uchar * dst, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਤਾਰਕਿਕ inv)
{
    ਤਾਰਕਿਕ ok;
    parallel_for_(Range(0, ਉਚਾਈ), DctIPPLoop_Invoker(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, inv, &ok), ਉਚਾਈ/(ਦੁੱਗਣਾ)(1<<4) );
    ਵਾਪਸ ok;
}

ਸਥਿਰ ਤਾਰਕਿਕ ippi_DCT_32f(ਅਚਲ uchar * src, size_t src_ਕਦਮ, uchar * dst, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਤਾਰਕਿਕ inv, ਤਾਰਕਿਕ ਪੰਕਤੀ)
{
    CV_INSTRUMENT_REGION_IPP();

    ਜੇ(ਪੰਕਤੀ)
        ਵਾਪਸ DctIPPLoop(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ, inv);
    ਨਹੀਂਤਾਂ
    {
#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 900
        Ippi_ਮਾਪ src_Roi_ਮਾਪ = {ਚੌੜਾਈ, ਉਚਾਈ};

        ਪੂਰਨ spec_ਮਾਪ    = 0;
        ਪੂਰਨ init_ਮਾਪ    = 0;
        ਪੂਰਨ ਬਫਰ_ਮਾਪ  = 0;

        Ipp8u* pDCTSpec = ਨਿਰਾ;
        Ipp8u* p_ਬਫਰ  = ਨਿਰਾ;
        Ipp8u* pInitBuf = ਨਿਰਾ;

        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IPP_RELEASE             \
            ਜੇ(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            ਜੇ(p_ਬਫਰ)                 \
                ippFree(p_ਬਫਰ);       \
            ਜੇ(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_ਮਾਪ  ipp_Dct_Get_ਮਾਪ  = inv ? ippi_DCTGet_ਮਾਪ_Wrap<ippi_DCTInv_Get_ਮਾਪ_32f> : ippi_DCTGet_ਮਾਪ_Wrap<ippi_DCTFwd_Get_ਮਾਪ_32f>;

        ਜੇ(ipp_Dct_Get_ਮਾਪ(src_Roi_ਮਾਪ, &spec_ਮਾਪ, &init_ਮਾਪ, &ਬਫਰ_ਮਾਪ) < 0)
            ਵਾਪਸ ਝੂਠ;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_ਮਾਪ);
        ਜੇ(!pDCTSpec && spec_ਮਾਪ)
            ਵਾਪਸ ਝੂਠ;

        p_ਬਫਰ  = (Ipp8u*)CV_IPP_MALLOC(ਬਫਰ_ਮਾਪ);
        ਜੇ(!p_ਬਫਰ && ਬਫਰ_ਮਾਪ)
        {
            IPP_RELEASE
            ਵਾਪਸ ਝੂਠ;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_ਮਾਪ);
        ਜੇ(!pInitBuf && init_ਮਾਪ)
        {
            IPP_RELEASE
            ਵਾਪਸ ਝੂਠ;
        }

        ਜੇ(ippDctInit(pDCTSpec, src_Roi_ਮਾਪ, pInitBuf) < 0)
        {
            IPP_RELEASE
            ਵਾਪਸ ਝੂਠ;
        }

        ਜੇ(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (ਦਸ਼ਮਲਵ*)src, static_cast<ਪੂਰਨ>(src_ਕਦਮ), (ਦਸ਼ਮਲਵ*)dst, static_cast<ਪੂਰਨ>(dst_ਕਦਮ), pDCTSpec, p_ਬਫਰ) < 0)
        {
            IPP_RELEASE
            ਵਾਪਸ ਝੂਠ;
        }

        IPP_RELEASE
        ਵਾਪਸ ਸੱਚ;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ IPP_RELEASE
#ਜਾਂਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 700
        Ipp_ਸਥਿਤੀ ਸਥਿਤੀ;
        ਖਾਲੀ* pDCTSpec;
        Auto_ਬਫਰ<uchar> buf;
        uchar* p_ਬਫਰ = 0;
        ਪੂਰਨ buf_ਮਾਪ=0;

        Ippi_ਮਾਪ src_Roi_ਮਾਪ = {ਚੌੜਾਈ, ਉਚਾਈ};

        CV_SUPPRESS_DEPRECATED_ਸ਼ੁਰੂ

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_ਮਾਪ ipp_Get_Buf_ਮਾਪ = inv ? ippi_DCTGet_Buf_ਮਾਪ_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_ਮਾਪ_32f> : ippi_DCTGet_Buf_ਮਾਪ_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_ਮਾਪ_32f>;

        ਸਥਿਤੀ = ippStsErr;

        ਜੇ (ippInitAlloc(&pDCTSpec, src_Roi_ਮਾਪ, ippAlgHintNone)>=0 && ipp_Get_Buf_ਮਾਪ(pDCTSpec, &buf_ਮਾਪ)>=0)
        {
            buf.allocate( buf_ਮਾਪ );
            p_ਬਫਰ = (uchar*)buf;

            ਸਥਿਤੀ = ippDctFun((ਦਸ਼ਮਲਵ*)src, static_cast<ਪੂਰਨ>(src_ਕਦਮ), (ਦਸ਼ਮਲਵ*)dst, static_cast<ਪੂਰਨ>(dst_ਕਦਮ), pDCTSpec, (Ipp8u*)p_ਬਫਰ);
        }

        ਜੇ (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_ਸਿਰਾ

        ਵਾਪਸ ਸਥਿਤੀ >= 0;
#ਨਹੀਂਤਾਂ
        CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(dst); CV_ਅਣਵਰਤਿਆ(inv); CV_ਅਣਵਰਤਿਆ(ਪੰਕਤੀ);
        ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
    }
}
}
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv {

ਵਰਗ OcvDctImpl CV_FINAL : ਜਨਤਕ hal::DCT2D
{
ਜਨਤਕ:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    Auto_ਬਫਰ<uchar> ws;
    ਤਾਰਕਿਕ is_ਪੰਕਤੀ_Transform;
    ਤਾਰਕਿਕ isInverse;
    ਤਾਰਕਿਕ isContinuous;
    ਪੂਰਨ ਸ਼ੁਰੂ_stage;
    ਪੂਰਨ ਸਿਰਾ_stage;
    ਪੂਰਨ ਚੌੜਾਈ;
    ਪੂਰਨ ਉਚਾਈ;
    ਪੂਰਨ ਡੂੰਘਾਈ;

    ਖਾਲੀ init(ਪੂਰਨ ਚੌੜਾਈ_3, ਪੂਰਨ ਉਚਾਈ_3, ਪੂਰਨ ਡੂੰਘਾਈ_2, ਪੂਰਨ flags)
    {
        ਚੌੜਾਈ = ਚੌੜਾਈ_3;
        ਉਚਾਈ = ਉਚਾਈ_3;
        ਡੂੰਘਾਈ = ਡੂੰਘਾਈ_2;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        is_ਪੰਕਤੀ_Transform = (flags & CV_HAL_DFT_ਪੰਕਤੀਆਂ) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        ਸਥਿਰ DCTFunc dct_tbl[4] =
        {
            dctWrap<ਦਸ਼ਮਲਵ, DCT<ਦਸ਼ਮਲਵ>>,
            dctWrap<ਦਸ਼ਮਲਵ, IDCT<ਦਸ਼ਮਲਵ>>,
            dctWrap<ਦੁੱਗਣਾ, DCT<ਦੁੱਗਣਾ>>,
            dctWrap<ਦੁੱਗਣਾ, IDCT<ਦੁੱਗਣਾ>>
        };
        dct_func = dct_tbl[(ਪੂਰਨ)isInverse + (ਡੂੰਘਾਈ == CV_64F)*2];
        opt.isComplex = ਝੂਠ;
        opt.isInverse = ਝੂਠ;
        opt.scale = 1.;

        ਜੇ (is_ਪੰਕਤੀ_Transform || ਉਚਾਈ == 1 || (ਚੌੜਾਈ == 1 && isContinuous))
        {
            ਸ਼ੁਰੂ_stage = ਸਿਰਾ_stage = 0;
        }
        ਨਹੀਂਤਾਂ
        {
            ਸ਼ੁਰੂ_stage = (ਚੌੜਾਈ == 1);
            ਸਿਰਾ_stage = 1;
        }
    }
    ਖਾਲੀ apply(ਅਚਲ uchar *src, size_t src_ਕਦਮ, uchar *dst, size_t dst_ਕਦਮ) CV_OVERRIDE
    {
        CV_IPP_ਲੜੀਵਾਰ(IPP_ਸੰਸਕਰਣ_X100 >= 700 && ਡੂੰਘਾਈ == CV_32F, ippi_DCT_32f(src, src_ਕਦਮ, dst, dst_ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ, isInverse, is_ਪੰਕਤੀ_Transform))

        ਪੂਰਨ prev_len = 0;
        ਪੂਰਨ elem_ਮਾਪ_2 = (ਡੂੰਘਾਈ == CV_32F) ? ਆਕਾਰ(ਦਸ਼ਮਲਵ) : ਆਕਾਰ(ਦੁੱਗਣਾ);
        ਅਚਲ DFTKernels& kernels = getDFTKernels(ਡੂੰਘਾਈ);

        ਲਈ(ਪੂਰਨ stage = ਸ਼ੁਰੂ_stage ; stage <= ਸਿਰਾ_stage; stage++ )
        {
            ਅਚਲ uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            ਪੂਰਨ len, ਗਿਣਤੀ_3;

            ਜੇ( stage == 0 )
            {
                len = ਚੌੜਾਈ;
                ਗਿਣਤੀ_3 = ਉਚਾਈ;
                ਜੇ( len == 1 && !is_ਪੰਕਤੀ_Transform )
                {
                    len = ਉਚਾਈ;
                    ਗਿਣਤੀ_3 = 1;
                }
                sstep0 = src_ਕਦਮ;
                dstep0 = dst_ਕਦਮ;
                sstep1 = dstep1 = elem_ਮਾਪ_2;
            }
            ਨਹੀਂਤਾਂ
            {
                len = ਉਚਾਈ;
                ਗਿਣਤੀ_3 = ਚੌੜਾਈ;
                sstep1 = src_ਕਦਮ;
                dstep1 = dst_ਕਦਮ;
                sstep0 = dstep0 = elem_ਮਾਪ_2;
            }

            opt.n = len;

            ਜੇ( len != prev_len )
            {
                ਜੇ( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                ਜੇ( len > 1 )
                {
                    plan.build(isInverse ? DFT_ਕਿਸਮ_IDCT : DFT_ਕਿਸਮ_DCT, len, ਡੂੰਘਾਈ, kernels.vlanes);
                    ws.allocate(plan.ws_ਬਾਈਟਾਂ);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = ws.ਅੰਕੜੇ();
                prev_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            ਦੁੱਗਣਾ nstripes = dftParallelStripes(ਗਿਣਤੀ_3, len);
            ਜੇ( len > 1 && ਗਿਣਤੀ_3 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                ਸਵੈਚਾਲਿਤ process_ਸਤਰਾਂ = [&](ਅਚਲ Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    Auto_ਬਫਰ<uchar> ws_(plan.ws_ਬਾਈਟਾਂ);
                    opt_.workspace = ws_.ਅੰਕੜੇ();
                    ਲਈ( ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, ਗਿਣਤੀ_3), process_ਸਤਰਾਂ, nstripes);
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ(ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ>(ਗਿਣਤੀ_3); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_ਕਦਮ = dst_ਕਦਮ;
        }
    }
};

ਬਣਤਰ ReplacementDCT2D : ਜਨਤਕ hal::DCT2D
{
    cvhalDFT *context;
    ਤਾਰਕਿਕ isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(ਝੂਠ) {}
    ਤਾਰਕਿਕ init(ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ flags)
    {
        ਪੂਰਨ res = cv_hal_dctInit2D(&context, ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ਵਾਪਸ isInitialized;
    }
    ਖਾਲੀ apply(ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ) CV_OVERRIDE
    {
        ਜੇ (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_ਅੰਕੜੇ, src_ਕਦਮ, dst_ਅੰਕੜੇ, dst_ਕਦਮ);
        }
    }
    ~ReplacementDCT2D()
    {
        ਜੇ (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

ਨਾਮਾਕਾਸ਼ hal {

Ptr<DCT2D> DCT2D::create(ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ flags)
{
    {
        ReplacementDCT2D *impl = ਨਵਾਂ ReplacementDCT2D();
        ਜੇ (impl->init(ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, flags))
        {
            ਵਾਪਸ Ptr<DCT2D>(impl);
        }
        ਮਿਟਾਓ impl;
    }
    {
        OcvDctImpl *impl = ਨਵਾਂ OcvDctImpl();
        impl->init(ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, flags);
        ਵਾਪਸ Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

ਖਾਲੀ cv::dct( ਨਿਵੇਸ਼_Array _src0, OutputArray _dst, ਪੂਰਨ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    ਜੇ (src.empty())
    {
        _dst.release();
        ਵਾਪਸ;
    }
    ਪੂਰਨ type = src.type(), ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.ਪੰਕਤੀਆਂ, src.ਖਾਨੇ_ਗਿਣਤੀ, type );
    Mat dst = _dst.getMat();

    ਪੂਰਨ f = 0;
    ਜੇ ((flags & DFT_ਪੰਕਤੀਆਂ) != 0)
        f |= CV_HAL_DFT_ਪੰਕਤੀਆਂ;
    ਜੇ ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    ਜੇ (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, ਡੂੰਘਾਈ, f);
    c->apply(src.ਅੰਕੜੇ, src.ਕਦਮ, dst.ਅੰਕੜੇ, dst.ਕਦਮ);
}


ਖਾਲੀ cv::idct( ਨਿਵੇਸ਼_Array src, OutputArray dst, ਪੂਰਨ flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

ਨਾਮਾਕਾਸ਼ cv
{

ਸਥਿਰ ਅਚਲ ਪੂਰਨ optimalDFTSizeTab[] = {
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

ਪੂਰਨ cv::getOptimalDFTSize( ਪੂਰਨ size0 )
{
    ਪੂਰਨ a = 0, b = ਆਕਾਰ(optimalDFTSizeTab)/ਆਕਾਰ(optimalDFTSizeTab[0]) - 1;
    ਜੇ( (ਬਿਨਾਨਿਸ਼ਾਨੀ)size0 >= (ਬਿਨਾਨਿਸ਼ਾਨੀ)optimalDFTSizeTab[b] )
        ਵਾਪਸ -1;

    ਜਦੋਂ( a < b )
    {
        ਪੂਰਨ c = (a + b) >> 1;
        ਜੇ( size0 <= optimalDFTSizeTab[c] )
            b = c;
        ਨਹੀਂਤਾਂ
            a = c+1;
    }

    ਵਾਪਸ optimalDFTSizeTab[b];
}
/* End of file. */
