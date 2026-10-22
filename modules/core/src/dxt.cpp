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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_core.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "dxt.hpp"
#അന്തര്ഭാവ "dxt.simd.hpp"
#അന്തര്ഭാവ "dxt.simd_declarations.hpp"
#അന്തര്ഭാവ <map>

നാമാകാശ cv
{

#എങ്കിൽ IPP_VERSION_X100 >= 710
#define USE_IPP_DFT 1
#അല്ലെങ്കിൽ
#undef USE_IPP_DFT
#endif

#എങ്കിൽ defined USE_IPP_DFT
#എങ്കിൽ IPP_VERSION_X100 >= 202220
#define IPP_DISABLE_DFT32F ((depth == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#അല്ലെങ്കിൽ
#define IPP_DISABLE_DFT32F അസത്യം
#endif
#endif

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#ifdef HAVE_OPENCL
// factorization used by the OpenCL radix selection
സ്ഥിതം പൂർണ്ണസംഖ്യ
DFTFactorize( പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ* factors )
{
    പൂർണ്ണസംഖ്യ nf = 0, f, i, j;

    എങ്കിൽ( n <= 5 )
    {
        factors[0] = n;
        തിരികെ 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    എങ്കിൽ( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    വേണ്ടി( f = 3; n > 1; )
    {
        പൂർണ്ണസംഖ്യ d = n/f;
        എങ്കിൽ( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        അല്ലെങ്കിൽ
        {
            f += 2;
            എങ്കിൽ( f*f > n )
                നിർത്തുക;
        }
    }

    എങ്കിൽ( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    വേണ്ടി( i = f; i < (nf+f)/2; i++ )
        CV_SWAP( factors[i], factors[nf-i-1+f], j );

    തിരികെ nf;
}
#endif

#ifdef USE_IPP_DFT
സ്ഥിതം IppStatus ippsDFTFwd_CToC( സ്ഥിരം Complex<ദശാംശം>* src, Complex<ദശാംശം>* dst,
                             സ്ഥിരം ശൂന്യം* spec, uchar* buf)
{
    തിരികെ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (സ്ഥിരം Ipp32fc*)src, (Ipp32fc*)dst,
                                 (സ്ഥിരം IppsDFTSpec_C_32fc*)spec, buf);
}

സ്ഥിതം IppStatus ippsDFTFwd_CToC( സ്ഥിരം Complex<ഇരട്ട>* src, Complex<ഇരട്ട>* dst,
                             സ്ഥിരം ശൂന്യം* spec, uchar* buf)
{
    തിരികെ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (സ്ഥിരം Ipp64fc*)src, (Ipp64fc*)dst,
                                 (സ്ഥിരം IppsDFTSpec_C_64fc*)spec, buf);
}

സ്ഥിതം IppStatus ippsDFTInv_CToC( സ്ഥിരം Complex<ദശാംശം>* src, Complex<ദശാംശം>* dst,
                             സ്ഥിരം ശൂന്യം* spec, uchar* buf)
{
    തിരികെ CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (സ്ഥിരം Ipp32fc*)src, (Ipp32fc*)dst,
                                 (സ്ഥിരം IppsDFTSpec_C_32fc*)spec, buf);
}

സ്ഥിതം IppStatus ippsDFTInv_CToC( സ്ഥിരം Complex<ഇരട്ട>* src, Complex<ഇരട്ട>* dst,
                                  സ്ഥിരം ശൂന്യം* spec, uchar* buf)
{
    തിരികെ CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (സ്ഥിരം Ipp64fc*)src, (Ipp64fc*)dst,
                                 (സ്ഥിരം IppsDFTSpec_C_64fc*)spec, buf);
}

സ്ഥിതം IppStatus ippsDFTFwd_RToPack( സ്ഥിരം ദശാംശം* src, ദശാംശം* dst,
                                     സ്ഥിരം ശൂന്യം* spec, uchar* buf)
{
    തിരികെ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (സ്ഥിരം IppsDFTSpec_R_32f*)spec, buf);
}

സ്ഥിതം IppStatus ippsDFTFwd_RToPack( സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst,
                                     സ്ഥിരം ശൂന്യം* spec, uchar* buf)
{
    തിരികെ CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (സ്ഥിരം IppsDFTSpec_R_64f*)spec, buf);
}

സ്ഥിതം IppStatus ippsDFTInv_PackToR( സ്ഥിരം ദശാംശം* src, ദശാംശം* dst,
                                     സ്ഥിരം ശൂന്യം* spec, uchar* buf)
{
    തിരികെ CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_32f, src, dst, (സ്ഥിരം IppsDFTSpec_R_32f*)spec, buf);
}

സ്ഥിതം IppStatus ippsDFTInv_PackToR( സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst,
                                     സ്ഥിരം ശൂന്യം* spec, uchar* buf)
{
    തിരികെ CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_64f, src, dst, (സ്ഥിരം IppsDFTSpec_R_64f*)spec, buf);
}
#endif

ഘടന OcvDftOptions;

തരനിർവചനം ശൂന്യം (*DFTFunc)(സ്ഥിരം OcvDftOptions & c, സ്ഥിരം ശൂന്യം* src, ശൂന്യം* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
ഘടന OcvDftOptions {
    ഇരട്ട scale;
    പൂർണ്ണസംഖ്യ n;

    തർക്കം isInverse;
    തർക്കം isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    തർക്കം useIpp;

#ifdef USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#endif

    സ്ഥിരം DftPlan* plan;
    സ്ഥിരം DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = അസത്യം;
        isComplex = അസത്യം;
        useIpp = അസത്യം;
#ifdef USE_IPP_DFT
        ipp_spec = 0;
        ipp_work = 0;
#endif
        dft_func = 0;
    }
};


/****************************************************************************************\
                     New 1D engine: kernel table, plan builder, driver
\****************************************************************************************/

സ്ഥിതം DFTKernels getDFTKernelsDispatch(പൂർണ്ണസംഖ്യ depth)
{
    CV_CPU_DISPATCH(getDFTKernels_, (depth), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
സ്ഥിതം സ്ഥിരം DFTKernels& getDFTKernels(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം സ്ഥിരം DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    സ്ഥിതം സ്ഥിരം DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    തിരികെ depth == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
ഘടന DftTwiddleGen
{
    പൂർണ്ണസംഖ്യ M, logB, maskB;
    AutoBuffer<ഇരട്ട, 1024> buf;
    ഇരട്ട *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    സ്ഥിതം ശൂന്യം fillTable(ഇരട്ട* re, ഇരട്ട* im, പൂർണ്ണസംഖ്യ എണ്ണം_2, ഇരട്ട wr, ഇരട്ട wi, ഇരട്ട stepAngle)
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i0 = 0; i0 < എണ്ണം_2; i0 += 16)
        {
            ഇരട്ട t0 = stepAngle*i0;
            ഇരട്ട cr = i0 == 0 ? 1 : cos(t0), ci = i0 == 0 ? 0 : sin(t0);
            പൂർണ്ണസംഖ്യ i1 = std::min(i0 + 16, എണ്ണം_2);
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                ഇരട്ട t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    ശൂന്യം init(പൂർണ്ണസംഖ്യ _M)
    {
        M = _M;
        logB = 0;
        ആകുമ്പോൾ ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        പൂർണ്ണസംഖ്യ B = 1 << logB;
        maskB = B - 1;
        പൂർണ്ണസംഖ്യ nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.data(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        ഇരട്ട a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fillTable(lo_re, lo_im, B, cos(a), sin(a), a);
        fillTable(hi_re, hi_im, nhi, cos(a*B), sin(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    ശൂന്യം get(പൂർണ്ണസംഖ്യ m, ഇരട്ട& re, ഇരട്ട& im) സ്ഥിരം
    {
        CV_DbgAssert(m >= 0);
        എങ്കിൽ (m >= M)
            m %= M;
        പൂർണ്ണസംഖ്യ a = m >> logB, b = m & maskB;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

template<typename T>
ശൂന്യം DftPlan::fillTables(size_t ntw, തർക്കം need_rtw, തർക്കം need_dct)
{
    DftPlan& p = *ഇത്;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    പൂർണ്ണസംഖ്യ M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*വലുപ്പം(T) + വലുപ്പം(ഇരട്ട) - 1)/വലുപ്പം(ഇരട്ട) + 1);
    T* cur = (T*)p.tw.data();
    പൂർണ്ണസംഖ്യ maxL = 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < p.nstages; s++)
        എങ്കിൽ (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].span);
    AutoBuffer<ഇരട്ട, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    ഇരട്ട* twd = twd_buf.data();
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        പൂർണ്ണസംഖ്യ r = st.radix, L = st.span, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        പൂർണ്ണസംഖ്യ step = M/(r*L);
        എങ്കിൽ (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr പൂർണ്ണസംഖ്യ K = 8;
            ഇരട്ട cr[K], ci[K], wr, wi;
            gen.get(K*step, wr, wi);
            ഇരട്ട* t1r = twd; ഇരട്ട* t1i = twd + L;      // leg 1 kept in double for the products
            വേണ്ടി (പൂർണ്ണസംഖ്യ j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*step, cr[k], ci[k]);
                പൂർണ്ണസംഖ്യ j1 = std::min(j0 + 16*K, L);
                വേണ്ടി (പൂർണ്ണസംഖ്യ j = j0; j < j1; j += K)
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        ഇരട്ട t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            വേണ്ടി (പൂർണ്ണസംഖ്യ q = 2; q < r; q++)
            {
                സ്ഥിരം T* pr = twr + (q-2)*ts; സ്ഥിരം T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < L; j++)
                {
                    ഇരട്ട ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        അല്ലെങ്കിൽ
        {
            // large odd radix: exact-ish generator lookup per entry
            വേണ്ടി (പൂർണ്ണസംഖ്യ q = 1; q < r; q++)
                വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < L; j++)
                {
                    ഇരട്ട re, im;
                    gen.get(q*j*step, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        എങ്കിൽ (r >= 7)
        {
            പൂർണ്ണസംഖ്യ h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            വേണ്ടി (പൂർണ്ണസംഖ്യ pp = 1; pp <= h; pp++)
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 1; k <= h; k++)
                {
                    ഇരട്ട t = 2*CV_PI*((ഇരട്ട)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)cos(t);
                    sn[(pp-1)*h + (k-1)] = (T)sin(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    എങ്കിൽ (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        പൂർണ്ണസംഖ്യ step = M/n;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k <= nc; k++)
        {
            ഇരട്ട re, im;
            gen.get(k*step, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    എങ്കിൽ (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        ഇരട്ട scale = (p.kind == DFT_KIND_DCT ? 2 : 1)*std::sqrt(1./(2*n));
        പൂർണ്ണസംഖ്യ step = M/(4*n);
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k <= nc; k++)
        {
            ഇരട്ട re, im;
            gen.get(k*step, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
ശൂന്യം DftPlan::build(പൂർണ്ണസംഖ്യ _kind, പൂർണ്ണസംഖ്യ _n, പൂർണ്ണസംഖ്യ _depth, പൂർണ്ണസംഖ്യ _vl)
{
    DftPlan& p = *ഇത്;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(_depth == CV_32F || _depth == CV_64F);
    തർക്കം real_kind = _kind != DFT_KIND_C2C;
    തർക്കം dct_kind = _kind == DFT_KIND_DCT || _kind == DFT_KIND_IDCT;
    എങ്കിൽ (dct_kind)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.kind = _kind; p.n = _n; p.depth = _depth; p.vl = _vl;
    p.nc = real_kind && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = depth == CV_32F ? വലുപ്പം(ദശാംശം) : വലുപ്പം(ഇരട്ട);
    p.real_നിവേശം = kind == DFT_KIND_R2C && (n & 1) != 0;
    p.need_tmp = kind == DFT_KIND_C2R || kind == DFT_KIND_DCT || kind == DFT_KIND_IDCT;
    p.pingpong = അസത്യം;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    പൂർണ്ണസംഖ്യ k2 = 0, rest = nc;
    ആകുമ്പോൾ ((rest & 1) == 0) { rest >>= 1; k2++; }
    പൂർണ്ണസംഖ്യ r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.first_radix = r0;
    പൂർണ്ണസംഖ്യ radices[DFT_MAX_STAGES], nst = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    ആകുമ്പോൾ (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    ആകുമ്പോൾ (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    വേണ്ടി (പൂർണ്ണസംഖ്യ f = 7; rest > 1; f += 2)
    {
        എങ്കിൽ (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            നിർത്തുക;
        }
        ആകുമ്പോൾ (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    പൂർണ്ണസംഖ്യ L = r0, max_h = 0;
    size_t ntw = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        പൂർണ്ണസംഖ്യ r = radices[s];
        st.radix = r; st.span = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        എങ്കിൽ (vl > 1 && L >= vl)
        {
            st.mode = DFT_STAGE_FULL;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            എങ്കിൽ (L % vl != 0)
                p.pingpong = സത്യം;   // back-off clamp => overlapping stores => must be out-of-place
        }
        അല്ലെങ്കിൽ
        {
            st.mode = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        എങ്കിൽ (r >= 7)
        {
            പൂർണ്ണസംഖ്യ h = (r - 1)/2;
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
    പൂർണ്ണസംഖ്യ seq[DFT_MAX_STAGES + 1], nseq = 0;
    എങ്കിൽ (r0 > 1) seq[nseq++] = r0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < nst; s++) seq[nseq++] = radices[s];
    പൂർണ്ണസംഖ്യ digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        പൂർണ്ണസംഖ്യ Ls = 1;
        വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    എങ്കിൽ (r0 == 1)
    {
        p.itab.allocate(nc*2);
        പൂർണ്ണസംഖ്യ* it_all = p.itab.data();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        പൂർണ്ണസംഖ്യ n_in = std::min(nseq, 2);
        പൂർണ്ണസംഖ്യ r_0 = n_in > 0 ? seq[0] : 1, w_0 = n_in > 0 ? weights[0] : 0;
        പൂർണ്ണസംഖ്യ r_1 = n_in > 1 ? seq[1] : 1, w_1 = n_in > 1 ? weights[1] : 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ pos = 0, rev_hi = 0; pos < nc; )
        {
            വേണ്ടി (പൂർണ്ണസംഖ്യ d1 = 0; d1 < r_1; d1++)
            {
                പൂർണ്ണസംഖ്യ rev = rev_hi + d1*w_1;
                പൂർണ്ണസംഖ്യ* it = it_all + pos*2;
                എങ്കിൽ (p.real_നിവേശം)
                {
                    വേണ്ടി (പൂർണ്ണസംഖ്യ d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                അല്ലെങ്കിൽ
                {
                    വേണ്ടി (പൂർണ്ണസംഖ്യ d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            വേണ്ടി (പൂർണ്ണസംഖ്യ s = n_in; s < nseq; s++)
            {
                rev_hi += weights[s];
                എങ്കിൽ (++digits[s] < seq[s])
                    നിർത്തുക;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        പൂർണ്ണസംഖ്യ M = nc/r0;
        p.itab.allocate(M);
        പൂർണ്ണസംഖ്യ* gtab = p.itab.data();
        പൂർണ്ണസംഖ്യ gw[DFT_MAX_STAGES + 1];
        {
            പൂർണ്ണസംഖ്യ Ls = 1;
            വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        പൂർണ്ണസംഖ്യ sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        പൂർണ്ണസംഖ്യ r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        പൂർണ്ണസംഖ്യ r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ base = 0, g_hi = 0; base < M; )
        {
            വേണ്ടി (പൂർണ്ണസംഖ്യ db = 0; db < r_b; db++)
            {
                പൂർണ്ണസംഖ്യ g = (g_hi + db*g_b)*r0;
                വേണ്ടി (പൂർണ്ണസംഖ്യ da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[base++] = g;
            }
            വേണ്ടി (പൂർണ്ണസംഖ്യ s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                എങ്കിൽ (++digits[s] < seq[s])
                    നിർത്തുക;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    തർക്കം need_rtw = real_kind && n % 2 == 0;
    എങ്കിൽ (need_rtw) ntw += 2*(size_t)(nc + 1);
    എങ്കിൽ (dct_kind) ntw += 2*(size_t)(nc + 1);
    എങ്കിൽ (depth == CV_32F)
        fillTables<ദശാംശം>(ntw, need_rtw, dct_kind);
    അല്ലെങ്കിൽ
        fillTables<ഇരട്ട>(ntw, need_rtw, dct_kind);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_bytes = 2*(size_t)nc*esz;
    തർക്കം two_pairs = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_pairs ? pair_bytes : 0;
    p.scratch_ofs = pair_bytes*(two_pairs ? 2 : 1);
    size_t scratch_bytes = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_bytes = p.scratch_ofs + scratch_bytes + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
സ്ഥിതം ശൂന്യം runDft(സ്ഥിരം OcvDftOptions& c, സ്ഥിരം ശൂന്യം* src, size_t sstep, ശൂന്യം* dst, size_t dstep,
                   തർക്കം complex_io)
{
    സ്ഥിരം DftPlan& p = *c.plan;
    സ്ഥിരം DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.depth == CV_32F ? വലുപ്പം(ദശാംശം) : വലുപ്പം(ഇരട്ട);
    uchar* base = alignPtr(c.workspace, 64);
    uchar* re0 = base;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = base + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = base + p.scratch_ofs;

    സ്ഥിരം ശൂന്യം* asrc = src;
    size_t astep = sstep;
    തർക്കം conj = അസത്യം;
    എങ്കിൽ (p.kind == DFT_KIND_C2R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = സത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (p.kind == DFT_KIND_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = സത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (p.kind == DFT_KIND_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (p.kind == DFT_KIND_C2C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.first_radix == 1);

    DftPreprocFunc pre = p.first_radix == 1 ? k.preprocRadix0 : p.first_radix == 2 ? k.preprocRadix2 :
                         p.first_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < p.nstages; i++)
    {
        സ്ഥിരം DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        എങ്കിൽ (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        അല്ലെങ്കിൽ
            fn(st, sre, sim, sre, sim, scratch);
    }

    ഇരട്ട scale = c.scale;
    തിരഞ്ഞെടുപ്പ് (p.kind)
    {
    അവസ്ഥ DFT_KIND_C2C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        നിർത്തുക;
    അവസ്ഥ DFT_KIND_R2C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        നിർത്തുക;
    അവസ്ഥ DFT_KIND_C2R:
        എങ്കിൽ (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        അല്ലെങ്കിൽ
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        നിർത്തുക;
    അവസ്ഥ DFT_KIND_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        CV_Assert(p.kind == DFT_KIND_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
template<typename T> സ്ഥിതം ശൂന്യം
DFT(സ്ഥിരം OcvDftOptions & c, സ്ഥിരം Complex<T>* src, Complex<T>* dst)
{
    എങ്കിൽ( c.useIpp )
    {
#ifdef USE_IPP_DFT
        എങ്കിൽ( !c.isInverse )
        {
            എങ്കിൽ (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                തിരികെ;
            }
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                തിരികെ;
            }
        }
        setIppErrorStatus();
#endif
    }

    runDft(c, src, 1, dst, 1, അസത്യം);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
template<typename T> സ്ഥിതം ശൂന്യം
RealDFT(സ്ഥിരം OcvDftOptions & c, സ്ഥിരം T* src, T* dst)
{
    പൂർണ്ണസംഖ്യ complex_output = c.isComplex;

    എങ്കിൽ( c.useIpp )
    {
#ifdef USE_IPP_DFT
        പൂർണ്ണസംഖ്യ n = c.n;
        T* ipp_dst = dst + complex_output;
        എങ്കിൽ (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            എങ്കിൽ( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                എങ്കിൽ( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_ADD(CV_IMPL_IPP);
            തിരികെ;
        }
        setIppErrorStatus();
#endif
    }
    runDft(c, src, 1, dst, 1, complex_output != 0);
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
template<typename T> സ്ഥിതം ശൂന്യം
CCSIDFT(സ്ഥിരം OcvDftOptions & c, സ്ഥിരം T* src, T* dst)
{
    പൂർണ്ണസംഖ്യ complex_നിവേശം = c.isComplex;

    എങ്കിൽ( complex_നിവേശം )
        CV_Assert( src != dst );
    എങ്കിൽ( c.useIpp )
    {
#ifdef USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        സ്ഥിരം T* ipp_src = src;
        T save_s1 = 0;
        എങ്കിൽ( complex_നിവേശം )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        IppStatus status = ippsDFTInv_PackToR( ipp_src, dst, c.ipp_spec, c.ipp_work );
        എങ്കിൽ( complex_നിവേശം )
            ((T*)src)[1] = save_s1;
        എങ്കിൽ( status >= 0 )
        {
            CV_IMPL_ADD(CV_IMPL_IPP);
            തിരികെ;
        }
        setIppErrorStatus();
#endif
    }
    runDft(c, src, 1, dst, 1, complex_നിവേശം != 0);
}

സ്ഥിതം ശൂന്യം
CopyColumn( സ്ഥിരം uchar* _src, size_t src_step,
            uchar* _dst, size_t dst_step,
            പൂർണ്ണസംഖ്യ len, size_t elem_size )
{
    പൂർണ്ണസംഖ്യ i, t0, t1;
    സ്ഥിരം പൂർണ്ണസംഖ്യ* src = (സ്ഥിരം പൂർണ്ണസംഖ്യ*)_src;
    പൂർണ്ണസംഖ്യ* dst = (പൂർണ്ണസംഖ്യ*)_dst;
    src_step /= വലുപ്പം(src[0]);
    dst_step /= വലുപ്പം(dst[0]);

    എങ്കിൽ( elem_size == വലുപ്പം(പൂർണ്ണസംഖ്യ) )
    {
        വേണ്ടി( i = 0; i < len; i++, src += src_step, dst += dst_step )
            dst[0] = src[0];
    }
    അല്ലെങ്കിൽ എങ്കിൽ( elem_size == വലുപ്പം(പൂർണ്ണസംഖ്യ)*2 )
    {
        വേണ്ടി( i = 0; i < len; i++, src += src_step, dst += dst_step )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( elem_size == വലുപ്പം(പൂർണ്ണസംഖ്യ)*4 )
    {
        വേണ്ടി( i = 0; i < len; i++, src += src_step, dst += dst_step )
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
template<typename T> സ്ഥിതം വരിയിൽ ശൂന്യം
copyFromColumns_( സ്ഥിരം uchar* src, size_t src_step, T* dst, പൂർണ്ണസംഖ്യ ncols, പൂർണ്ണസംഖ്യ len )
{
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++, src += src_step )
    {
        സ്ഥിരം T* s = (സ്ഥിരം T*)src;
        വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

സ്ഥിതം ശൂന്യം
CopyFromColumns( സ്ഥിരം uchar* src, size_t src_step, uchar* dst, പൂർണ്ണസംഖ്യ ncols, പൂർണ്ണസംഖ്യ len, size_t elem_size )
{
    എങ്കിൽ( elem_size == വലുപ്പം(പൂർണ്ണസംഖ്യ) )
        copyFromColumns_(src, src_step, (പൂർണ്ണസംഖ്യ*)dst, ncols, len);
    അല്ലെങ്കിൽ എങ്കിൽ( elem_size == വലുപ്പം(int64) )
        copyFromColumns_(src, src_step, (int64*)dst, ncols, len);
    അല്ലെങ്കിൽ
    {
        CV_Assert( elem_size == വലുപ്പം(int64)*2 );
        copyFromColumns_(src, src_step, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
template<typename T> സ്ഥിതം വരിയിൽ ശൂന്യം
copyToColumns_( സ്ഥിരം T* src, uchar* dst, size_t dst_step, പൂർണ്ണസംഖ്യ ncols, പൂർണ്ണസംഖ്യ len )
{
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++, dst += dst_step )
    {
        T* d = (T*)dst;
        വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

സ്ഥിതം ശൂന്യം
CopyToColumns( സ്ഥിരം uchar* src, uchar* dst, size_t dst_step, പൂർണ്ണസംഖ്യ ncols, പൂർണ്ണസംഖ്യ len, size_t elem_size )
{
    എങ്കിൽ( elem_size == വലുപ്പം(പൂർണ്ണസംഖ്യ) )
        copyToColumns_((സ്ഥിരം പൂർണ്ണസംഖ്യ*)src, dst, dst_step, ncols, len);
    അല്ലെങ്കിൽ എങ്കിൽ( elem_size == വലുപ്പം(int64) )
        copyToColumns_((സ്ഥിരം int64*)src, dst, dst_step, ncols, len);
    അല്ലെങ്കിൽ
    {
        CV_Assert( elem_size == വലുപ്പം(int64)*2 );
        copyToColumns_((സ്ഥിരം Vec<int64, 2>*)src, dst, dst_step, ncols, len);
    }
}

സ്ഥിതം ശൂന്യം
ExpandCCS( uchar* _ptr, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ elem_size )
{
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ( elem_size == (പൂർണ്ണസംഖ്യ)വലുപ്പം(ദശാംശം) )
    {
        ദശാംശം* p = (ദശാംശം*)_ptr;
        വേണ്ടി( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        എങ്കിൽ( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        വേണ്ടി( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    അല്ലെങ്കിൽ
    {
        ഇരട്ട* p = (ഇരട്ട*)_ptr;
        വേണ്ടി( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        എങ്കിൽ( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        വേണ്ടി( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

template<typename T, ശൂന്യം (*fn)(സ്ഥിരം OcvDftOptions&, സ്ഥിരം T*, T*)>
സ്ഥിതം ശൂന്യം dftWrap(സ്ഥിരം OcvDftOptions & c, സ്ഥിരം ശൂന്യം* src, ശൂന്യം* dst)
{
    fn(c, (സ്ഥിരം T*)src, (T*)dst);
}

}

#ifdef USE_IPP_DFT
തരനിർവചനം IppStatus (CV_STDCALL* IppDFTGetSizeFunc)(പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, IppHintAlgorithm, പൂർണ്ണസംഖ്യ*, പൂർണ്ണസംഖ്യ*, പൂർണ്ണസംഖ്യ*);
തരനിർവചനം IppStatus (CV_STDCALL* IppDFTInitFunc)(പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, IppHintAlgorithm, ശൂന്യം*, uchar*);

template<typename SpecType, IppStatus (CV_STDCALL *init_fn)(പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, IppHintAlgorithm, SpecType*, Ipp8u*)>
സ്ഥിതം IppStatus CV_STDCALL ippDFTInitWrap(പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ flags, IppHintAlgorithm hint, ശൂന്യം* spec, Ipp8u* initbuf)
{
    തിരികെ init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#endif

നാമാകാശ cv
{
#എങ്കിൽ defined USE_IPP_DFT

തരനിർവചനം IppStatus (CV_STDCALL* ippiDFT_C_Func)(സ്ഥിരം Ipp32fc*, പൂർണ്ണസംഖ്യ, Ipp32fc*, പൂർണ്ണസംഖ്യ, സ്ഥിരം IppiDFTSpec_C_32fc*, Ipp8u*);
തരനിർവചനം IppStatus (CV_STDCALL* ippiDFT_R_Func)(സ്ഥിരം Ipp32f* , പൂർണ്ണസംഖ്യ, Ipp32f* , പൂർണ്ണസംഖ്യ, സ്ഥിരം IppiDFTSpec_R_32f* , Ipp8u*);

template <typename Dft>
വർഗ്ഗം Dft_C_IPPLoop_Invoker : പൊതു ParallelLoopBody
{
പൊതു:

    Dft_C_IPPLoop_Invoker(സ്ഥിരം uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, പൂർണ്ണസംഖ്യ _width,
                          സ്ഥിരം Dft& _ippidft, പൂർണ്ണസംഖ്യ _norm_flag, തർക്കം *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = സത്യം;
    }

    മായാ ശൂന്യം operator()(സ്ഥിരം Range& range) സ്ഥിരം CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* pBuffer = 0;
        Ipp8u* pMemInit= 0;
        പൂർണ്ണസംഖ്യ sizeBuffer=0;
        പൂർണ്ണസംഖ്യ sizeSpec=0;
        പൂർണ്ണസംഖ്യ sizeInit=0;

        IppiSize srcRoiSize = {width, 1};

        status = ippiDFTGetSize_C_32fc(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
        എങ്കിൽ ( status < 0 )
        {
            *ok = അസത്യം;
            തിരികെ;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( sizeSpec );

        എങ്കിൽ ( sizeInit > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

        എങ്കിൽ ( sizeBuffer > 0 )
            pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

        status = ippiDFTInit_C_32fc( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        എങ്കിൽ ( sizeInit > 0 )
            ippFree( pMemInit );

        എങ്കിൽ ( status < 0 )
        {
            ippFree( pDFTSpec );
            എങ്കിൽ ( sizeBuffer > 0 )
                ippFree( pBuffer );
            *ok = അസത്യം;
            തിരികെ;
        }

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = range.start; i < range.end; ++i)
            എങ്കിൽ(!ippidft((Ipp32fc*)(src + src_step * i), src_step, (Ipp32fc*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)pBuffer))
            {
                *ok = അസത്യം;
            }

        എങ്കിൽ ( sizeBuffer > 0 )
            ippFree( pBuffer );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

സ്വകാര്യ:
    സ്ഥിരം uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    പൂർണ്ണസംഖ്യ width;
    സ്ഥിരം Dft& ippidft;
    പൂർണ്ണസംഖ്യ norm_flag;
    തർക്കം *ok;

    സ്ഥിരം Dft_C_IPPLoop_Invoker& operator= (സ്ഥിരം Dft_C_IPPLoop_Invoker&);
};

template <typename Dft>
വർഗ്ഗം Dft_R_IPPLoop_Invoker : പൊതു ParallelLoopBody
{
പൊതു:

    Dft_R_IPPLoop_Invoker(സ്ഥിരം uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, പൂർണ്ണസംഖ്യ _width,
                          സ്ഥിരം Dft& _ippidft, പൂർണ്ണസംഖ്യ _norm_flag, തർക്കം *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = സത്യം;
    }

    മായാ ശൂന്യം operator()(സ്ഥിരം Range& range) സ്ഥിരം CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* pBuffer = 0;
        Ipp8u* pMemInit= 0;
        പൂർണ്ണസംഖ്യ sizeBuffer=0;
        പൂർണ്ണസംഖ്യ sizeSpec=0;
        പൂർണ്ണസംഖ്യ sizeInit=0;

        IppiSize srcRoiSize = {width, 1};

        status = ippiDFTGetSize_R_32f(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
        എങ്കിൽ ( status < 0 )
        {
            *ok = അസത്യം;
            തിരികെ;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( sizeSpec );

        എങ്കിൽ ( sizeInit > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

        എങ്കിൽ ( sizeBuffer > 0 )
            pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

        status = ippiDFTInit_R_32f( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        എങ്കിൽ ( sizeInit > 0 )
            ippFree( pMemInit );

        എങ്കിൽ ( status < 0 )
        {
            ippFree( pDFTSpec );
            എങ്കിൽ ( sizeBuffer > 0 )
                ippFree( pBuffer );
            *ok = അസത്യം;
            തിരികെ;
        }

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = range.start; i < range.end; ++i)
            എങ്കിൽ(!ippidft((ദശാംശം*)(src + src_step * i), src_step, (ദശാംശം*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)pBuffer))
            {
                *ok = അസത്യം;
            }

        എങ്കിൽ ( sizeBuffer > 0 )
            ippFree( pBuffer );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

സ്വകാര്യ:
    സ്ഥിരം uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    പൂർണ്ണസംഖ്യ width;
    സ്ഥിരം Dft& ippidft;
    പൂർണ്ണസംഖ്യ norm_flag;
    തർക്കം *ok;

    സ്ഥിരം Dft_R_IPPLoop_Invoker& operator= (സ്ഥിരം Dft_R_IPPLoop_Invoker&);
};

template <typename Dft>
തർക്കം Dft_C_IPPLoop(സ്ഥിരം uchar * src, size_t src_step, uchar * dst, size_t dst_step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, സ്ഥിരം Dft& ippidft, പൂർണ്ണസംഖ്യ norm_flag)
{
    തർക്കം ok;
    parallel_for_(Range(0, height), Dft_C_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(ഇരട്ട)(1<<16) );
    തിരികെ ok;
}

template <typename Dft>
തർക്കം Dft_R_IPPLoop(സ്ഥിരം uchar * src, size_t src_step, uchar * dst, size_t dst_step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, സ്ഥിരം Dft& ippidft, പൂർണ്ണസംഖ്യ norm_flag)
{
    തർക്കം ok;
    parallel_for_(Range(0, height), Dft_R_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(ഇരട്ട)(1<<16) );
    തിരികെ ok;
}

ഘടന IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    തർക്കം operator()(സ്ഥിരം Ipp32fc* src, size_t srcStep, Ipp32fc* dst, size_t dstStep, സ്ഥിരം IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* pBuffer) സ്ഥിരം
    {
        തിരികെ ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<പൂർണ്ണസംഖ്യ>(srcStep), dst, static_cast<പൂർണ്ണസംഖ്യ>(dstStep), pDFTSpec, pBuffer) >= 0 : അസത്യം;
    }
സ്വകാര്യ:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

ഘടന IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippiDFT_PackToR_32f_C1R(_func){}

    തർക്കം operator()(സ്ഥിരം Ipp32f* src, size_t srcStep, Ipp32f* dst, size_t dstStep, സ്ഥിരം IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* pBuffer) സ്ഥിരം
    {
        തിരികെ ippiDFT_PackToR_32f_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_PackToR_32f_C1R, src, static_cast<പൂർണ്ണസംഖ്യ>(srcStep), dst, static_cast<പൂർണ്ണസംഖ്യ>(dstStep), pDFTSpec, pBuffer) >= 0 : അസത്യം;
    }
സ്വകാര്യ:
    ippiDFT_R_Func ippiDFT_PackToR_32f_C1R;
};

സ്ഥിതം തർക്കം ippi_DFT_C_32F(സ്ഥിരം uchar * src, size_t src_step, uchar * dst, size_t dst_step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, തർക്കം inv, പൂർണ്ണസംഖ്യ norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* pBuffer = 0;
    Ipp8u* pMemInit= 0;
    പൂർണ്ണസംഖ്യ sizeBuffer=0;
    പൂർണ്ണസംഖ്യ sizeSpec=0;
    പൂർണ്ണസംഖ്യ sizeInit=0;

    IppiSize srcRoiSize = {width, height};

    status = ippiDFTGetSize_C_32fc(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
    എങ്കിൽ ( status < 0 )
        തിരികെ അസത്യം;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( sizeSpec );

    എങ്കിൽ ( sizeInit > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

    എങ്കിൽ ( sizeBuffer > 0 )
        pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

    status = ippiDFTInit_C_32fc( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    എങ്കിൽ ( sizeInit > 0 )
        ippFree( pMemInit );

    എങ്കിൽ ( status < 0 )
    {
        ippFree( pDFTSpec );
        എങ്കിൽ ( sizeBuffer > 0 )
            ippFree( pBuffer );
        തിരികെ അസത്യം;
    }

    എങ്കിൽ (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<പൂർണ്ണസംഖ്യ>(src_step), (Ipp32fc*)dst, static_cast<പൂർണ്ണസംഖ്യ>(dst_step), pDFTSpec, pBuffer);
    അല്ലെങ്കിൽ
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<പൂർണ്ണസംഖ്യ>(src_step), (Ipp32fc*)dst, static_cast<പൂർണ്ണസംഖ്യ>(dst_step), pDFTSpec, pBuffer);

    എങ്കിൽ ( sizeBuffer > 0 )
        ippFree( pBuffer );

    ippFree( pDFTSpec );

    എങ്കിൽ(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
}

സ്ഥിതം തർക്കം ippi_DFT_R_32F(സ്ഥിരം uchar * src, size_t src_step, uchar * dst, size_t dst_step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, തർക്കം inv, പൂർണ്ണസംഖ്യ norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* pBuffer = 0;
    Ipp8u* pMemInit= 0;
    പൂർണ്ണസംഖ്യ sizeBuffer=0;
    പൂർണ്ണസംഖ്യ sizeSpec=0;
    പൂർണ്ണസംഖ്യ sizeInit=0;

    IppiSize srcRoiSize = {width, height};

    status = ippiDFTGetSize_R_32f(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
    എങ്കിൽ ( status < 0 )
        തിരികെ അസത്യം;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( sizeSpec );

    എങ്കിൽ ( sizeInit > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

    എങ്കിൽ ( sizeBuffer > 0 )
        pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

    status = ippiDFTInit_R_32f( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    എങ്കിൽ ( sizeInit > 0 )
        ippFree( pMemInit );

    എങ്കിൽ ( status < 0 )
    {
        ippFree( pDFTSpec );
        എങ്കിൽ ( sizeBuffer > 0 )
            ippFree( pBuffer );
        തിരികെ അസത്യം;
    }

    എങ്കിൽ (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (ദശാംശം*)src, static_cast<പൂർണ്ണസംഖ്യ>(src_step), (ദശാംശം*)dst, static_cast<പൂർണ്ണസംഖ്യ>(dst_step), pDFTSpec, pBuffer);
    അല്ലെങ്കിൽ
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_PackToR_32f_C1R, (ദശാംശം*)src, static_cast<പൂർണ്ണസംഖ്യ>(src_step), (ദശാംശം*)dst, static_cast<പൂർണ്ണസംഖ്യ>(dst_step), pDFTSpec, pBuffer);

    എങ്കിൽ ( sizeBuffer > 0 )
        ippFree( pBuffer );

    ippFree( pDFTSpec );

    എങ്കിൽ(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
}

#endif
}

#ifdef HAVE_OPENCL

നാമാകാശ cv
{

എണ്ണൽ FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

ഘടന OCL_FftPlan
{
സ്വകാര്യ:
    UMat twiddles;
    String buildOptions;
    പൂർണ്ണസംഖ്യ thread_എണ്ണം;
    പൂർണ്ണസംഖ്യ dft_size;
    പൂർണ്ണസംഖ്യ dft_depth;
    തർക്കം status;

പൊതു:
    OCL_FftPlan(പൂർണ്ണസംഖ്യ _size, പൂർണ്ണസംഖ്യ _depth) : dft_size(_size), dft_depth(_depth), status(സത്യം)
    {
        CV_Assert( dft_depth == CV_32F || dft_depth == CV_64F );

        പൂർണ്ണസംഖ്യ min_radix;
        std::vector<പൂർണ്ണസംഖ്യ> radixes, blocks;
        ocl_getRadixes(dft_size, radixes, blocks, min_radix);
        thread_എണ്ണം = dft_size / min_radix;

        എങ്കിൽ (thread_എണ്ണം > (പൂർണ്ണസംഖ്യ) ocl::Device::getDefault().maxWorkGroupSize())
        {
            status = അസത്യം;
            തിരികെ;
        }

        // generate string with radix calls
        String radix_processing;
        പൂർണ്ണസംഖ്യ n = 1, twiddle_size = 0;
        വേണ്ടി (size_t i=0; i<radixes.size(); i++)
        {
            പൂർണ്ണസംഖ്യ radix = radixes[i], block = blocks[i];
            എങ്കിൽ (block > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, block, twiddle_size, n, dft_size/radix);
            അല്ലെങ്കിൽ
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_size, n, dft_size/radix);
            twiddle_size += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_size, CV_MAKE_TYPE(dft_depth, 2));
        എങ്കിൽ (dft_depth == CV_32F)
            fillRadixTable<ദശാംശം>(twiddles, radixes);
        അല്ലെങ്കിൽ
            fillRadixTable<ഇരട്ട>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_size, min_radix, ocl::typeToStr(dft_depth), ocl::typeToStr(CV_MAKE_TYPE(dft_depth, 2)),
                              dft_depth == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    തർക്കം enqueueTransform(നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ അക്കം_dfts, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ fftType, തർക്കം rows = സത്യം) സ്ഥിരം
    {
        എങ്കിൽ (!status)
            തിരികെ അസത്യം;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        തർക്കം is1d = (flags & DFT_ROWS) != 0 || അക്കം_dfts == 1;
        തർക്കം inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        എങ്കിൽ (rows)
        {
            globalsize[0] = thread_എണ്ണം; globalsize[1] = src.rows;
            localsize[0] = thread_എണ്ണം; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            എങ്കിൽ ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        അല്ലെങ്കിൽ
        {
            globalsize[0] = അക്കം_dfts; globalsize[1] = thread_എണ്ണം;
            localsize[0] = 1; localsize[1] = thread_എണ്ണം;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            എങ്കിൽ (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        എങ്കിൽ (!inv)
        {
            എങ്കിൽ ((is1d && src.channels() == 1) || (rows && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ (rows && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            എങ്കിൽ (dst.cols % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        എങ്കിൽ (k.empty())
            തിരികെ അസത്യം;

        k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::ReadOnlyNoSize(twiddles), thread_എണ്ണം, അക്കം_dfts);
        തിരികെ k.run(2, globalsize, localsize, അസത്യം);
    }

സ്വകാര്യ:
    സ്ഥിതം ശൂന്യം ocl_getRadixes(പൂർണ്ണസംഖ്യ cols, std::vector<പൂർണ്ണസംഖ്യ>& radixes, std::vector<പൂർണ്ണസംഖ്യ>& blocks, പൂർണ്ണസംഖ്യ& min_radix)
    {
        പൂർണ്ണസംഖ്യ factors[34];
        പൂർണ്ണസംഖ്യ nf = DFTFactorize(cols, factors);

        പൂർണ്ണസംഖ്യ n = 1;
        പൂർണ്ണസംഖ്യ factor_index = 0;
        min_radix = INT_MAX;

        // 2^n transforms
        എങ്കിൽ ((factors[factor_index] & 1) == 0)
        {
            വേണ്ടി( ; n < factors[factor_index];)
            {
                പൂർണ്ണസംഖ്യ radix = 2, block = 1;
                എങ്കിൽ (8*n <= factors[0])
                    radix = 8;
                അല്ലെങ്കിൽ എങ്കിൽ (4*n <= factors[0])
                {
                    radix = 4;
                    എങ്കിൽ (cols % 12 == 0)
                        block = 3;
                    അല്ലെങ്കിൽ എങ്കിൽ (cols % 8 == 0)
                        block = 2;
                }
                അല്ലെങ്കിൽ
                {
                    എങ്കിൽ (cols % 10 == 0)
                        block = 5;
                    അല്ലെങ്കിൽ എങ്കിൽ (cols % 8 == 0)
                        block = 4;
                    അല്ലെങ്കിൽ എങ്കിൽ (cols % 6 == 0)
                        block = 3;
                    അല്ലെങ്കിൽ എങ്കിൽ (cols % 4 == 0)
                        block = 2;
                }

                radixes.push_back(radix);
                blocks.push_back(block);
                min_radix = min(min_radix, block*radix);
                n *= radix;
            }
            factor_index++;
        }

        // all the other transforms
        വേണ്ടി( ; factor_index < nf; factor_index++)
        {
            പൂർണ്ണസംഖ്യ radix = factors[factor_index], block = 1;
            എങ്കിൽ (radix == 3)
            {
                എങ്കിൽ (cols % 12 == 0)
                    block = 4;
                അല്ലെങ്കിൽ എങ്കിൽ (cols % 9 == 0)
                    block = 3;
                അല്ലെങ്കിൽ എങ്കിൽ (cols % 6 == 0)
                    block = 2;
            }
            അല്ലെങ്കിൽ എങ്കിൽ (radix == 5)
            {
                എങ്കിൽ (cols % 10 == 0)
                    block = 2;
            }
            radixes.push_back(radix);
            blocks.push_back(block);
            min_radix = min(min_radix, block*radix);
        }
    }

    template <typename T>
    സ്ഥിതം ശൂന്യം fillRadixTable(UMat twiddles, സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        പൂർണ്ണസംഖ്യ ptr_index = 0;

        പൂർണ്ണസംഖ്യ n = 1;
        വേണ്ടി (size_t i=0; i<radixes.size(); i++)
        {
            പൂർണ്ണസംഖ്യ radix = radixes[i];
            n *= radix;

            വേണ്ടി (പൂർണ്ണസംഖ്യ j=1; j<radix; j++)
            {
                ഇരട്ട theta = -CV_2PI*j/n;

                വേണ്ടി (പൂർണ്ണസംഖ്യ k=0; k<(n/radix); k++)
                {
                    ptr[ptr_index++] = (T) cos(k*theta);
                    ptr[ptr_index++] = (T) sin(k*theta);
                }
            }
        }
    }
};

വർഗ്ഗം OCL_FftPlanCache
{
പൊതു:
    സ്ഥിതം OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, പുതിയ OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(പൂർണ്ണസംഖ്യ dft_size, പൂർണ്ണസംഖ്യ depth)
    {
        പൂർണ്ണസംഖ്യ താക്കോൽ = (dft_size << 16) | (depth & 0xFFFF);
        std::map<പൂർണ്ണസംഖ്യ, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(താക്കോൽ);
        എങ്കിൽ (f != planStorage.end())
        {
            തിരികെ f->second;
        }
        അല്ലെങ്കിൽ
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(പുതിയ OCL_FftPlan(dft_size, depth));
            planStorage[താക്കോൽ] = newPlan;
            തിരികെ newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

സംരക്ഷിത:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<പൂർണ്ണസംഖ്യ, Ptr<OCL_FftPlan> > planStorage;
};

സ്ഥിതം തർക്കം ocl_dft_rows(നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ nonzero_rows, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ fftType)
{
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.cols(), depth);
    തിരികെ plan->enqueueTransform(_src, _dst, nonzero_rows, flags, fftType, സത്യം);
}

സ്ഥിതം തർക്കം ocl_dft_cols(നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ nonzero_cols, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ fftType)
{
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.rows(), depth);
    തിരികെ plan->enqueueTransform(_src, _dst, nonzero_cols, flags, fftType, അസത്യം);
}

വരിയിൽ FftType determineFFTType(തർക്കം real_നിവേശം, തർക്കം complex_നിവേശം, തർക്കം real_output, തർക്കം complex_output, തർക്കം inv)
{
    // output format is not specified
    എങ്കിൽ (!real_output && !complex_output)
        complex_output = സത്യം;

    // input or output format is ambiguous
    എങ്കിൽ (real_നിവേശം == complex_നിവേശം || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType result = real_നിവേശം ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    എങ്കിൽ (result == C2R && !inv)
        result = C2C;

    // Inverse CCS to Complex not supported
    എങ്കിൽ (result == R2C && inv)
        result = R2R;

    തിരികെ result;
}

സ്ഥിതം തർക്കം ocl_dft(നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ nonzero_rows)
{
    പൂർണ്ണസംഖ്യ type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    Size ssize = _src.size();
    തർക്കം doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    എങ്കിൽ (!(cn == 1 || cn == 2)
        || !(depth == CV_32F || (depth == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        തിരികെ അസത്യം;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    എങ്കിൽ (ssize.area() != getOptimalDFTSize(ssize.area()))
        തിരികെ അസത്യം;

    UMat src = _src.getUMat();
    തർക്കം inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    എങ്കിൽ( nonzero_rows <= 0 || nonzero_rows > _src.rows() )
        nonzero_rows = _src.rows();
    തർക്കം is1d = (flags & DFT_ROWS) != 0 || nonzero_rows == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    എങ്കിൽ (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.createSameSize(src, CV_MAKETYPE(depth, 2));
        output = _dst.getUMat();
    }
    അല്ലെങ്കിൽ
    {
        // real output
        എങ്കിൽ (is1d)
        {
            _dst.createSameSize(src, CV_MAKETYPE(depth, 1));
            output = _dst.getUMat();
        }
        അല്ലെങ്കിൽ
        {
            _dst.createSameSize(src, CV_MAKETYPE(depth, 1));
            output.create(src.size, CV_MAKETYPE(depth, 2));
        }
    }

    തർക്കം result = അസത്യം;
    എങ്കിൽ (!inv)
    {
        പൂർണ്ണസംഖ്യ nonzero_cols = fftType == R2R ? output.cols/2 + 1 : output.cols;
        result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
        എങ്കിൽ (!is1d)
            result = result && ocl_dft_cols(output, _dst, nonzero_cols, flags, fftType);
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (fftType == C2C)
        {
            // complex output
            result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            എങ്കിൽ (!is1d)
                result = result && ocl_dft_cols(output, output, output.cols, flags, fftType);
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ (is1d)
            {
                result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            }
            അല്ലെങ്കിൽ
            {
                പൂർണ്ണസംഖ്യ nonzero_cols = src.cols/2 + 1;
                result = ocl_dft_cols(src, output, nonzero_cols, flags, fftType);
                result = result && ocl_dft_rows(output, _dst, nonzero_rows, flags, fftType);
            }
        }
    }
    തിരികെ result;
}

} // namespace cv;

#endif

#ifdef HAVE_CLAMDFFT

നാമാകാശ cv {

#define CLAMDDFT_Assert(func) \
    { \
        clfftStatus s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

വർഗ്ഗം PlanCache
{
    ഘടന FftPlan
    {
        FftPlan(സ്ഥിരം Size & _dft_size, പൂർണ്ണസംഖ്യ _src_step, പൂർണ്ണസംഖ്യ _dst_step, തർക്കം _doubleFP, തർക്കം _inplace, പൂർണ്ണസംഖ്യ _flags, FftType _fftType) :
            dft_size(_dft_size), src_step(_src_step), dst_step(_dst_step),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            തർക്കം dft_inverse = (flags & DFT_INVERSE) != 0;
            തർക്കം dft_scale = (flags & DFT_SCALE) != 0;
            തർക്കം dft_rows = (flags & DFT_ROWS) != 0;

            clfftLayout inLayout = CLFFT_REAL, outLayout = CLFFT_REAL;
            clfftDim dim = dft_size.height == 1 || dft_rows ? CLFFT_1D : CLFFT_2D;

            size_t batchSize = dft_rows ? dft_size.height : 1;
            size_t clLengthsIn[3] = { (size_t)dft_size.width, dft_rows ? 1 : (size_t)dft_size.height, 1 };
            size_t clStridesIn[3] = { 1, 1, 1 };
            size_t clStridesOut[3]  = { 1, 1, 1 };
            പൂർണ്ണസംഖ്യ elemSize = doubleFP ? വലുപ്പം(ഇരട്ട) : വലുപ്പം(ദശാംശം);

            തിരഞ്ഞെടുപ്പ് (fftType)
            {
            അവസ്ഥ C2C:
                inLayout = CLFFT_COMPLEX_INTERLEAVED;
                outLayout = CLFFT_COMPLEX_INTERLEAVED;
                clStridesIn[1] = src_step / (elemSize << 1);
                clStridesOut[1] = dst_step / (elemSize << 1);
                നിർത്തുക;
            അവസ്ഥ R2C:
                inLayout = CLFFT_REAL;
                outLayout = CLFFT_HERMITIAN_INTERLEAVED;
                clStridesIn[1] = src_step / elemSize;
                clStridesOut[1] = dst_step / (elemSize << 1);
                നിർത്തുക;
            അവസ്ഥ C2R:
                inLayout = CLFFT_HERMITIAN_INTERLEAVED;
                outLayout = CLFFT_REAL;
                clStridesIn[1] = src_step / (elemSize << 1);
                clStridesOut[1] = dst_step / elemSize;
                നിർത്തുക;
            അവസ്ഥ R2R:
            സ്ഥിരസ്ഥിതി:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                നിർത്തുക;
            }

            clStridesIn[2] = dft_rows ? clStridesIn[1] : dft_size.width * clStridesIn[1];
            clStridesOut[2] = dft_rows ? clStridesOut[1] : dft_size.width * clStridesOut[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, clLengthsIn))

            // setting plan properties
            CLAMDDFT_Assert(clfftSetPlanPrecision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfftSetResultLocation(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfftSetLayout(plHandle, inLayout, outLayout))
            CLAMDDFT_Assert(clfftSetPlanBatchSize(plHandle, batchSize))
            CLAMDDFT_Assert(clfftSetPlanInStride(plHandle, dim, clStridesIn))
            CLAMDDFT_Assert(clfftSetPlanOutStride(plHandle, dim, clStridesOut))
            CLAMDDFT_Assert(clfftSetPlanDistance(plHandle, clStridesIn[dim], clStridesOut[dim]))

            ദശാംശം scale = dft_scale ? 1.0f / (dft_rows ? dft_size.width : dft_size.area()) : 1.0f;
            CLAMDDFT_Assert(clfftSetPlanScale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &queue, ശൂന്യ, ശൂന്യ))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        friend വർഗ്ഗം PlanCache;

    സ്വകാര്യ:
        Size dft_size;
        പൂർണ്ണസംഖ്യ src_step, dst_step;
        തർക്കം doubleFP;
        തർക്കം inplace;
        പൂർണ്ണസംഖ്യ flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

പൊതു:
    സ്ഥിതം PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, പുതിയ PlanCache())
    }

    clfftPlanHandle getPlanHandle(സ്ഥിരം Size & dft_size, പൂർണ്ണസംഖ്യ src_step, പൂർണ്ണസംഖ്യ dst_step, തർക്കം doubleFP,
                                  തർക്കം inplace, പൂർണ്ണസംഖ്യ flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        വേണ്ടി (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            സ്ഥിരം FftPlan * സ്ഥിരം plan = planStorage[i];

            എങ്കിൽ (plan->dft_size == dft_size &&
                plan->flags == flags &&
                plan->src_step == src_step &&
                plan->dst_step == dst_step &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                എങ്കിൽ (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    നിർത്തുക;
                }

                തിരികെ plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(പുതിയ FftPlan(dft_size, src_step, dst_step, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        തിരികെ newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

സംരക്ഷിത:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

ബാഹ്യ "C" {

സ്ഥിതം ശൂന്യം CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, ശൂന്യം *p)
{
    UMatData * u = (UMatData *)p;

    എങ്കിൽ( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

സ്ഥിതം തർക്കം ocl_dft_amdfft(നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ flags)
{
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    Size ssize = _src.size();

    തർക്കം doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    എങ്കിൽ ( (!doubleSupport && depth == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        തിരികെ അസത്യം;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    എങ്കിൽ (ssize.area() != getOptimalDFTSize(ssize.area()))
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ dst_complex_നിവേശം = cn == 2 ? 1 : 0;
    തർക്കം dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    പൂർണ്ണസംഖ്യ dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    തർക്കം dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_നിവേശം << 0 | dft_complex_output << 1);

    തിരഞ്ഞെടുപ്പ് (fftType)
    {
    അവസ്ഥ C2C:
        _dst.create(ssize.height, ssize.width, CV_MAKE_TYPE(depth, 2));
        നിർത്തുക;
    അവസ്ഥ R2C: // TODO implement it if possible
    അവസ്ഥ C2R: // TODO implement it if possible
    അവസ്ഥ R2R: // AMD Fft does not support this type
    സ്ഥിരസ്ഥിതി:
        തിരികെ അസത്യം;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    എങ്കിൽ (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        തിരികെ അസത്യം;
    തർക്കം inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (പൂർണ്ണസംഖ്യ)src.step, (പൂർണ്ണസംഖ്യ)dst.step,
                          depth == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t bufferSize = 0;
    CLAMDDFT_Assert(clfftGetTmpBufSize(plHandle, &bufferSize))
    UMat tmpBuffer(1, (പൂർണ്ണസംഖ്യ)bufferSize, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfftEnqueueTransform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &queue, 0, ശൂന്യ, &e,
                                          &srcarg, &dstarg, (cl_mem)tmpBuffer.handle(ACCESS_RW)))

    tmpBuffer.addref();
    clSetEventCallback(e, CL_COMPLETE, oclCleanupCallback, tmpBuffer.u);
    തിരികെ സത്യം;
}

#undef DFT_ASSERT

}

#endif // HAVE_CLAMDFFT

നാമാകാശ cv
{

template <typename T>
സ്ഥിതം ശൂന്യം complementComplex(T * ptr, size_t step, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = step/വലുപ്പം(p0[0]);
    വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        വേണ്ടി( പൂർണ്ണസംഖ്യ j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

സ്ഥിതം ശൂന്യം complementComplexOutput(പൂർണ്ണസംഖ്യ depth, uchar * ptr, size_t step, പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ dft_dims)
{
    എങ്കിൽ( depth == CV_32F )
        complementComplex((ദശാംശം*)ptr, step, എണ്ണം_2, len, dft_dims);
    അല്ലെങ്കിൽ
        complementComplex((ഇരട്ട*)ptr, step, എണ്ണം_2, len, dft_dims);
}

എണ്ണൽ DftMode {
    InvalidDft = 0,
    FwdRealToCCS,
    FwdRealToComplex,
    FwdComplexToComplex,
    InvCCSToReal,
    InvComplexToReal,
    InvComplexToComplex,
};

എണ്ണൽ DftDims {
    InvalidDim = 0,
    OneDim,
    OneDimColWise,
    TwoDims
};

വരിയിൽ സ്ഥിരം അക്ഷരം * modeName(DftMode m)
{
    തിരഞ്ഞെടുപ്പ് (m)
    {
    അവസ്ഥ InvalidDft: തിരികെ "InvalidDft";
    അവസ്ഥ FwdRealToCCS: തിരികെ "FwdRealToCCS";
    അവസ്ഥ FwdRealToComplex: തിരികെ "FwdRealToComplex";
    അവസ്ഥ FwdComplexToComplex: തിരികെ "FwdComplexToComplex";
    അവസ്ഥ InvCCSToReal: തിരികെ "InvCCSToReal";
    അവസ്ഥ InvComplexToReal: തിരികെ "InvComplexToReal";
    അവസ്ഥ InvComplexToComplex: തിരികെ "InvComplexToComplex";
    }
    തിരികെ 0;
}

വരിയിൽ സ്ഥിരം അക്ഷരം * dimsName(DftDims d)
{
    തിരഞ്ഞെടുപ്പ് (d)
    {
    അവസ്ഥ InvalidDim: തിരികെ "InvalidDim";
    അവസ്ഥ OneDim: തിരികെ "OneDim";
    അവസ്ഥ OneDimColWise: തിരികെ "OneDimColWise";
    അവസ്ഥ TwoDims: തിരികെ "TwoDims";
    };
    തിരികെ 0;
}

template <typename T>
വരിയിൽ തർക്കം isInv(T mode)
{
    തിരഞ്ഞെടുപ്പ് ((DftMode)mode)
    {
        അവസ്ഥ InvCCSToReal:
        അവസ്ഥ InvComplexToReal:
        അവസ്ഥ InvComplexToComplex: തിരികെ സത്യം;
        സ്ഥിരസ്ഥിതി: തിരികെ അസത്യം;
    }
}

വരിയിൽ DftMode determineMode(തർക്കം inv, പൂർണ്ണസംഖ്യ cn1, പൂർണ്ണസംഖ്യ cn2)
{
    എങ്കിൽ (!inv)
    {
        എങ്കിൽ (cn1 == 1 && cn2 == 1)
            തിരികെ FwdRealToCCS;
        അല്ലെങ്കിൽ എങ്കിൽ (cn1 == 1 && cn2 == 2)
            തിരികെ FwdRealToComplex;
        അല്ലെങ്കിൽ എങ്കിൽ (cn1 == 2 && cn2 == 2)
            തിരികെ FwdComplexToComplex;
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (cn1 == 1 && cn2 == 1)
            തിരികെ InvCCSToReal;
        അല്ലെങ്കിൽ എങ്കിൽ (cn1 == 2 && cn2 == 1)
            തിരികെ InvComplexToReal;
        അല്ലെങ്കിൽ എങ്കിൽ (cn1 == 2 && cn2 == 2)
            തിരികെ InvComplexToComplex;
    }
    തിരികെ InvalidDft;
}


വരിയിൽ DftDims determineDims(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, തർക്കം isRowWise, തർക്കം isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    എങ്കിൽ (isRowWise)
        തിരികെ OneDim;
    എങ്കിൽ (cols == 1 && rows > 1) // one-column-shaped input
    {
        എങ്കിൽ (isContinuous)
            തിരികെ OneDim;
        അല്ലെങ്കിൽ
            തിരികെ OneDimColWise;
    }
    എങ്കിൽ (rows == 1)
        തിരികെ OneDim;
    എങ്കിൽ (cols > 1 && rows > 1)
        തിരികെ TwoDims;
    തിരികെ InvalidDim;
}

വർഗ്ഗം OcvDftBasicImpl CV_FINAL : പൊതു hal::DFT1D
{
പൊതു:
    OcvDftOptions opt;
    // the plan is built once in init(), the workspace is allocated once here and reused by
    // every apply() (zero allocations per transform)
    DftPlan plan;
    AutoBuffer<uchar> ws;
#ifdef USE_IPP_DFT
    AutoBuffer<uchar> ippbuf;
    AutoBuffer<uchar> ippworkbuf;
    size_t ipp_worksize;
#endif

പൊതു:
    OcvDftBasicImpl()
    {
#ifdef USE_IPP_DFT
        ipp_worksize = 0;
#endif
    }
    ശൂന്യം init(പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ flags, തർക്കം *needBuffer)
    {
        പൂർണ്ണസംഖ്യ stage = (flags & CV_HAL_DFT_STAGE_COLS) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        തർക്കം real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        തർക്കം needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = അസത്യം;
    #ifdef USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        എങ്കിൽ( CV_IPP_CHECK_COND && (opt.n*എണ്ണം_2 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            പൂർണ്ണസംഖ്യ ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            പൂർണ്ണസംഖ്യ specsize=0, initsize=0, worksize=0;
            IppDFTGetSizeFunc getSizeFunc = 0;
            IppDFTInitFunc initFunc = 0;

            എങ്കിൽ( real_transform && stage == 0 )
            {
                എങ്കിൽ( depth == CV_32F )
                {
                    getSizeFunc = ippsDFTGetSize_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                അല്ലെങ്കിൽ
                {
                    getSizeFunc = ippsDFTGetSize_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            അല്ലെങ്കിൽ
            {
                എങ്കിൽ( depth == CV_32F )
                {
                    getSizeFunc = ippsDFTGetSize_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                അല്ലെങ്കിൽ
                {
                    getSizeFunc = ippsDFTGetSize_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            എങ്കിൽ( getSizeFunc(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                എങ്കിൽ( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = സത്യം;
            }
            അല്ലെങ്കിൽ
                setIppErrorStatus();
        }
    #endif

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            സ്ഥിരം DFTKernels& kernels = getDFTKernels(depth);
            പൂർണ്ണസംഖ്യ kind = stage == 0 && real_transform ? (opt.isInverse ? DFT_KIND_C2R : DFT_KIND_R2C)
                                                    : DFT_KIND_C2C;
            plan.build(kind, len, depth, kernels.vlanes);
            ws.allocate(plan.ws_bytes);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.data();
            എങ്കിൽ (needBuffer)
                *needBuffer = അസത്യം;   // the engine supports in-place for every kind
        }

        {
            സ്ഥിതം DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<ദശാംശം>>,
                dftWrap<ദശാംശം, RealDFT<ദശാംശം>>,
                dftWrap<ദശാംശം, CCSIDFT<ദശാംശം>>,
                dftWrap<Complexd, DFT<ഇരട്ട>>,
                dftWrap<ഇരട്ട, RealDFT<ഇരട്ട>>,
                dftWrap<ഇരട്ട, CCSIDFT<ഇരട്ട>>
            };
            പൂർണ്ണസംഖ്യ idx = 0;
            എങ്കിൽ (stage == 0)
            {
                എങ്കിൽ (real_transform)
                {
                    എങ്കിൽ (!opt.isInverse)
                        idx = 1;
                    അല്ലെങ്കിൽ
                        idx = 2;
                }
            }
            എങ്കിൽ (depth == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        എങ്കിൽ(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            പൂർണ്ണസംഖ്യ row_എണ്ണം = എണ്ണം_2;
            എങ്കിൽ (stage == 0 && (flags & CV_HAL_DFT_ROWS) != 0)
                row_എണ്ണം = 1;
            opt.scale = 1./(len * row_എണ്ണം);
        }
    }

    ശൂന്യം apply(സ്ഥിരം uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspaceSize() സ്ഥിരം
    {
        size_t sz = opt.plan ? plan.ws_bytes : 0;
#ifdef USE_IPP_DFT
        എങ്കിൽ (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#endif
        തിരികെ sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    ശൂന്യം applyWithWorkspace(സ്ഥിരം uchar *src, uchar *dst, uchar* workspace) സ്ഥിരം
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#ifdef USE_IPP_DFT
        എങ്കിൽ (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#endif
        opt_.dft_func(opt_, src, dst);
    }

    ശൂന്യം സ്വതന്ത്രം() {}
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
സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ DFT_COLS_BATCH_MIN = 4;
സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ DFT_COLS_BATCH_MAX = 16;
സ്ഥിതം സ്ഥിരം size_t DFT_COLS_BATCH_BYTES = 1 << 17;

സ്ഥിതം പൂർണ്ണസംഖ്യ dftColsBatch(പൂർണ്ണസംഖ്യ len, size_t complex_elem_size)
{
    പൂർണ്ണസംഖ്യ batch = (പൂർണ്ണസംഖ്യ)(DFT_COLS_BATCH_BYTES/((size_t)len*complex_elem_size));
    തിരികെ std::max(DFT_COLS_BATCH_MIN, std::min(DFT_COLS_BATCH_MAX, batch));
}

സ്ഥിതം ഇരട്ട dftParallelStripes(പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ len)
{
    സ്ഥിരം ഇരട്ട quantum = 0.5e6;
    തിരികെ (ഇരട്ട)എണ്ണം_2*len*std::max(std::log((ഇരട്ട)len)/std::log(2.), 1.)/quantum;
}

വർഗ്ഗം OcvDftImpl CV_FINAL : പൊതു hal::DFT2D
{
സംരക്ഷിത:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    സ്ഥിരം OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    സ്ഥിരം OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    തർക്കം needBufferA;
    തർക്കം needBufferB;
    തർക്കം inv;
    പൂർണ്ണസംഖ്യ width;
    പൂർണ്ണസംഖ്യ height;
    DftMode mode;
    പൂർണ്ണസംഖ്യ elem_size;
    പൂർണ്ണസംഖ്യ complex_elem_size;
    പൂർണ്ണസംഖ്യ depth;
    തർക്കം real_transform;
    പൂർണ്ണസംഖ്യ nonzero_rows;
    തർക്കം isRowTransform;
    തർക്കം isScaled;
    std::vector<പൂർണ്ണസംഖ്യ> stages;
    തർക്കം useIpp;
    പൂർണ്ണസംഖ്യ src_channels;
    പൂർണ്ണസംഖ്യ dst_channels;

    AutoBuffer<uchar> tmp_bufA;
    AutoBuffer<uchar> tmp_bufB;
    AutoBuffer<uchar> buf0;
    AutoBuffer<uchar> buf1;

പൊതു:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        needBufferA = അസത്യം;
        needBufferB = അസത്യം;
        inv = അസത്യം;
        width = 0;
        height = 0;
        mode = InvalidDft;
        elem_size = 0;
        complex_elem_size = 0;
        depth = 0;
        real_transform = അസത്യം;
        nonzero_rows = 0;
        isRowTransform = അസത്യം;
        isScaled = അസത്യം;
        useIpp = അസത്യം;
        src_channels = 0;
        dst_channels = 0;
    }

    ശൂന്യം init(പൂർണ്ണസംഖ്യ _width, പൂർണ്ണസംഖ്യ _height, പൂർണ്ണസംഖ്യ _depth, പൂർണ്ണസംഖ്യ _src_channels, പൂർണ്ണസംഖ്യ _dst_channels, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ _nonzero_rows)
    {
        തർക്കം isComplex = _src_channels != _dst_channels;
        nonzero_rows = _nonzero_rows;
        width = _width;
        height = _height;
        depth = _depth;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        തർക്കം isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        തർക്കം isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        തർക്കം isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        mode = determineMode(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        needBufferA = അസത്യം;
        needBufferB = അസത്യം;
        real_transform = (mode != FwdComplexToComplex && mode != InvComplexToComplex);

        elem_size = (depth == CV_32F) ? വലുപ്പം(ദശാംശം) : വലുപ്പം(ഇരട്ട);
        complex_elem_size = elem_size * 2;
        എങ്കിൽ( !real_transform )
            elem_size = complex_elem_size;

#എങ്കിൽ defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            എങ്കിൽ (nonzero_rows == 0 && depth == CV_32F && ((width * height)>(പൂർണ്ണസംഖ്യ)(1<<6)))
            {
                എങ്കിൽ (mode == FwdComplexToComplex || mode == InvComplexToComplex || mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    useIpp = സത്യം;
                    തിരികെ;
                }
            }
        }
#endif

        DftDims dims = determineDims(height, width, isRowTransform, isContinuous);
        എങ്കിൽ (dims == TwoDims)
        {
            stages.resize(2);
            എങ്കിൽ (mode == InvCCSToReal || mode == InvComplexToReal)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            അല്ലെങ്കിൽ
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        അല്ലെങ്കിൽ
        {
            stages.resize(1);
            എങ്കിൽ (dims == OneDimColWise)
                stages[0] = 1;
            അല്ലെങ്കിൽ
                stages[0] = 0;
        }

        വേണ്ടി(uint stageIndex = 0; stageIndex < stages.size(); ++stageIndex)
        {
            എങ്കിൽ (stageIndex == 1)
            {
                isInplace = സത്യം;
                isComplex = അസത്യം;
            }

            പൂർണ്ണസംഖ്യ stage = stages[stageIndex];
            തർക്കം isLastStage = (stageIndex + 1 == stages.size());

            പൂർണ്ണസംഖ്യ len, എണ്ണം_2;

            പൂർണ്ണസംഖ്യ f = 0;
            എങ്കിൽ (inv)
                f |= CV_HAL_DFT_INVERSE;
            എങ്കിൽ (isScaled)
                f |= CV_HAL_DFT_SCALE;
            എങ്കിൽ (isRowTransform)
                f |= CV_HAL_DFT_ROWS;
            എങ്കിൽ (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            എങ്കിൽ (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            എങ്കിൽ (!isLastStage)
                f |= CV_HAL_DFT_TWO_STAGE;

            എങ്കിൽ( stage == 0 ) // row-wise transform
            {
                എങ്കിൽ (width == 1 && !isRowTransform )
                {
                    len = height;
                    എണ്ണം_2 = width;
                }
                അല്ലെങ്കിൽ
                {
                    len = width;
                    എണ്ണം_2 = height;
                }
                needBufferA = isInplace;
                contextA = hal::DFT1D::create(len, എണ്ണം_2, depth, f, &needBufferA);
                basicA = dynamic_cast<സ്ഥിരം OcvDftBasicImpl*>(contextA.get());
                എങ്കിൽ (needBufferA)
                    tmp_bufA.allocate(len * complex_elem_size);
            }
            അല്ലെങ്കിൽ
            {
                len = height;
                എണ്ണം_2 = width;
                f |= CV_HAL_DFT_STAGE_COLS;
                needBufferB = isInplace;
                contextB = hal::DFT1D::create(len, എണ്ണം_2, depth, f, &needBufferB);
                basicB = dynamic_cast<സ്ഥിരം OcvDftBasicImpl*>(contextB.get());
                എങ്കിൽ (needBufferB)
                    tmp_bufB.allocate(len * complex_elem_size);

                buf0.allocate(len * complex_elem_size);
                buf1.allocate(len * complex_elem_size);
            }
        }
    }

    ശൂന്യം apply(സ്ഥിരം uchar * src, size_t src_step, uchar * dst, size_t dst_step) CV_OVERRIDE
    {
#എങ്കിൽ defined USE_IPP_DFT
        എങ്കിൽ (useIpp)
        {
            പൂർണ്ണസംഖ്യ ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            എങ്കിൽ (!isRowTransform)
            {
                എങ്കിൽ (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    എങ്കിൽ (ippi_DFT_C_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        തിരികെ;
                    }
                    setIppErrorStatus();
                }
                അല്ലെങ്കിൽ എങ്കിൽ (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    എങ്കിൽ (ippi_DFT_R_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        തിരികെ;
                    }
                    setIppErrorStatus();
                }
            }
            അല്ലെങ്കിൽ
            {
                എങ്കിൽ (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    എങ്കിൽ (Dft_C_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        തിരികെ;
                    }
                    setIppErrorStatus();
                }
                അല്ലെങ്കിൽ എങ്കിൽ (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippiDFTInv_PackToR_32f_C1R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    എങ്കിൽ (Dft_R_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        തിരികെ;
                    }
                    setIppErrorStatus();
                }
            }
            തിരികെ;
        }
#endif

        വേണ്ടി(uint stageIndex = 0; stageIndex < stages.size(); ++stageIndex)
        {
            പൂർണ്ണസംഖ്യ stage_src_channels = src_channels;
            പൂർണ്ണസംഖ്യ stage_dst_channels = dst_channels;

            എങ്കിൽ (stageIndex == 1)
            {
                src = dst;
                src_step = dst_step;
                stage_src_channels = stage_dst_channels;
            }

            പൂർണ്ണസംഖ്യ stage = stages[stageIndex];
            തർക്കം isLastStage = (stageIndex + 1 == stages.size());
            തർക്കം isComplex = stage_src_channels != stage_dst_channels;

            എങ്കിൽ( stage == 0 )
                rowDft(src, src_step, dst, dst_step, isComplex, isLastStage);
            അല്ലെങ്കിൽ
                colDft(src, src_step, dst, dst_step, stage_src_channels, stage_dst_channels, isLastStage);
        }
    }

സംരക്ഷിത:

    ശൂന്യം rowDft(സ്ഥിരം uchar* src_data, size_t src_step, uchar* dst_data, size_t dst_step, തർക്കം isComplex, തർക്കം isLastStage)
    {
        പൂർണ്ണസംഖ്യ len, എണ്ണം_2;
        എങ്കിൽ (width == 1 && !isRowTransform )
        {
            len = height;
            എണ്ണം_2 = width;
        }
        അല്ലെങ്കിൽ
        {
            len = width;
            എണ്ണം_2 = height;
        }
        പൂർണ്ണസംഖ്യ dptr_offset = 0;
        പൂർണ്ണസംഖ്യ dst_full_len = len*elem_size;

        എങ്കിൽ( needBufferA )
        {
            എങ്കിൽ (mode == FwdRealToCCS && (len & 1) && len > 1)
                dptr_offset = elem_size;
        }

        എങ്കിൽ( !inv && isComplex )
            dst_full_len += (len & 1) ? elem_size : complex_elem_size;

        പൂർണ്ണസംഖ്യ nz = nonzero_rows;
        എങ്കിൽ( nz <= 0 || nz > എണ്ണം_2 )
            nz = എണ്ണം_2;

        size_t ws_size = basicA ? basicA->workspaceSize() : 0;
        ഇരട്ട nstripes = dftParallelStripes(nz, len);
        എങ്കിൽ( ws_size > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            സ്ഥിരം OcvDftBasicImpl* ctx = basicA;
            auto processRows = [&, ctx](സ്ഥിരം Range& range)
            {
                AutoBuffer<uchar> ws(ws_size);
                AutoBuffer<uchar> rowbuf(needBufferA ? len * complex_elem_size : 0);
                വേണ്ടി( പൂർണ്ണസംഖ്യ i = range.start; i < range.end; i++ )
                {
                    സ്ഥിരം uchar* sptr = src_data + src_step * i;
                    uchar* dptr0 = dst_data + dst_step * i;
                    uchar* dptr = needBufferA ? rowbuf.data() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.data());
                    എങ്കിൽ( needBufferA )
                        memcpy( dptr0, dptr + dptr_offset, dst_full_len );
                }
            };
            parallel_for_(Range(0, nz), processRows, nstripes);
        }
        അല്ലെങ്കിൽ
        {
            വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nz; i++ )
            {
                സ്ഥിരം uchar* sptr = src_data + src_step * i;
                uchar* dptr0 = dst_data + dst_step * i;
                uchar* dptr = dptr0;

                എങ്കിൽ( needBufferA )
                    dptr = tmp_bufA.data();

                contextA->apply(sptr, dptr);

                എങ്കിൽ( needBufferA )
                    memcpy( dptr0, dptr + dptr_offset, dst_full_len );
            }
        }

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = nz; i < എണ്ണം_2; i++ )
        {
            uchar* dptr0 = dst_data + dst_step * i;
            memset( dptr0, 0, dst_full_len );
        }
        എങ്കിൽ(isLastStage &&  mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_data, dst_step, len, nz, 1);
    }

    ശൂന്യം colDft(സ്ഥിരം uchar* src_data, size_t src_step, uchar* dst_data, size_t dst_step, പൂർണ്ണസംഖ്യ stage_src_channels, പൂർണ്ണസംഖ്യ stage_dst_channels, തർക്കം isLastStage)
    {
        പൂർണ്ണസംഖ്യ len = height;
        പൂർണ്ണസംഖ്യ എണ്ണം_2 = width;
        പൂർണ്ണസംഖ്യ a = 0, b = എണ്ണം_2;
        uchar *dbuf0, *dbuf1;
        സ്ഥിരം uchar* sptr0 = src_data;
        uchar* dptr0 = dst_data;

        dbuf0 = buf0.data(), dbuf1 = buf1.data();

        എങ്കിൽ( needBufferB )
        {
            dbuf1 = tmp_bufB.data();
            dbuf0 = buf1.data();
        }

        എങ്കിൽ( real_transform )
        {
            പൂർണ്ണസംഖ്യ even;
            a = 1;
            even = (എണ്ണം_2 & 1) == 0;
            b = (എണ്ണം_2+1)/2;
            എങ്കിൽ( !inv )
            {
                memset( buf0.data(), 0, len*complex_elem_size );
                CopyColumn( sptr0, src_step, buf0.data(), complex_elem_size, len, elem_size );
                sptr0 += stage_dst_channels*elem_size;
                എങ്കിൽ( even )
                {
                    memset( buf1.data(), 0, len*complex_elem_size );
                    CopyColumn( sptr0 + (എണ്ണം_2-2)*elem_size, src_step,
                                buf1.data(), complex_elem_size, len, elem_size );
                }
            }
            അല്ലെങ്കിൽ എങ്കിൽ( stage_src_channels == 1 )
            {
                CopyColumn( sptr0, src_step, buf0.data(), elem_size, len, elem_size );
                ExpandCCS( buf0.data(), len, elem_size );
                എങ്കിൽ( even )
                {
                    CopyColumn( sptr0 + (എണ്ണം_2-1)*elem_size, src_step,
                                buf1.data(), elem_size, len, elem_size );
                    ExpandCCS( buf1.data(), len, elem_size );
                }
                sptr0 += elem_size;
            }
            അല്ലെങ്കിൽ
            {
                CopyColumn( sptr0, src_step, buf0.data(), complex_elem_size, len, complex_elem_size );
                എങ്കിൽ( even )
                {
                    CopyColumn( sptr0 + b*complex_elem_size, src_step,
                                   buf1.data(), complex_elem_size, len, complex_elem_size );
                }
                sptr0 += complex_elem_size;
            }

            എങ്കിൽ( even )
                contextB->apply(buf1.data(), dbuf1);
            contextB->apply(buf0.data(), dbuf0);

            എങ്കിൽ( stage_dst_channels == 1 )
            {
                എങ്കിൽ( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    memcpy( dbuf0 + elem_size, dbuf0, elem_size );
                    CopyColumn( dbuf0 + elem_size, elem_size, dptr0,
                                   dst_step, len, elem_size );
                    എങ്കിൽ( even )
                    {
                        memcpy( dbuf1 + elem_size, dbuf1, elem_size );
                        CopyColumn( dbuf1 + elem_size, elem_size,
                                       dptr0 + (എണ്ണം_2-1)*elem_size,
                                       dst_step, len, elem_size );
                    }
                    dptr0 += elem_size;
                }
                അല്ലെങ്കിൽ
                {
                    // copy the real part of the complex vector to the first/last column
                    CopyColumn( dbuf0, complex_elem_size, dptr0, dst_step, len, elem_size );
                    എങ്കിൽ( even )
                        CopyColumn( dbuf1, complex_elem_size, dptr0 + (എണ്ണം_2-1)*elem_size,
                                       dst_step, len, elem_size );
                    dptr0 += elem_size;
                }
            }
            അല്ലെങ്കിൽ
            {
                CV_Assert( !inv );
                CopyColumn( dbuf0, complex_elem_size, dptr0,
                               dst_step, len, complex_elem_size );
                എങ്കിൽ( even )
                    CopyColumn( dbuf1, complex_elem_size,
                                   dptr0 + b*complex_elem_size,
                                   dst_step, len, complex_elem_size );
                dptr0 += complex_elem_size;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        പൂർണ്ണസംഖ്യ ncols = b - a;
        സ്ഥിരം പൂർണ്ണസംഖ്യ cols_batch = dftColsBatch(len, complex_elem_size);
        പൂർണ്ണസംഖ്യ nbatches = (ncols + cols_batch - 1)/cols_batch;
        size_t ws_size = basicB ? basicB->workspaceSize() : 0;
        size_t colbuf_size = (size_t)len*complex_elem_size;
        സ്ഥിരം OcvDftBasicImpl* ctx = basicB;
        auto processBatches = [&, ctx](സ്ഥിരം Range& range)
        {
            AutoBuffer<uchar> ws(ws_size);
            AutoBuffer<uchar> cbuf(colbuf_size*cols_batch*(needBufferB ? 2 : 1));
            uchar* cbuf0 = cbuf.data();
            uchar* cdbuf0 = needBufferB ? cbuf0 + colbuf_size*cols_batch : cbuf0;
            വേണ്ടി( പൂർണ്ണസംഖ്യ bi = range.start; bi < range.end; bi++ )
            {
                പൂർണ്ണസംഖ്യ c0 = bi*cols_batch;
                പൂർണ്ണസംഖ്യ nc = std::min(cols_batch, ncols - c0);
                സ്ഥിരം uchar* sptr = sptr0 + (size_t)c0*complex_elem_size;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_size;
                CopyFromColumns( sptr, src_step, cbuf0, nc, len, complex_elem_size );
                വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < nc; k++ )
                {
                    എങ്കിൽ( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_size, cdbuf0 + k*colbuf_size, ws.data());
                    അല്ലെങ്കിൽ
                        contextB->apply(cbuf0 + k*colbuf_size, cdbuf0 + k*colbuf_size);
                }
                CopyToColumns( cdbuf0, dptr, dst_step, nc, len, complex_elem_size );
            }
        };
        ഇരട്ട nstripes = dftParallelStripes(ncols, len);
        എങ്കിൽ( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        അല്ലെങ്കിൽ
            processBatches(Range(0, nbatches));
        എങ്കിൽ(isLastStage && mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_data, dst_step, എണ്ണം_2, len, 2);
    }
};

ഘടന ReplacementDFT1D : പൊതു hal::DFT1D
{
    cvhalDFT *context;
    തർക്കം isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(അസത്യം) {}
    തർക്കം init(പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ flags, തർക്കം *needBuffer)
    {
        പൂർണ്ണസംഖ്യ res = cv_hal_dftInit1D(&context, len, എണ്ണം_2, depth, flags, needBuffer);
        isInitialized = (res == CV_HAL_ERROR_OK);
        തിരികെ isInitialized;
    }
    ശൂന്യം apply(സ്ഥിരം uchar *src, uchar *dst) CV_OVERRIDE
    {
        എങ്കിൽ (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        എങ്കിൽ (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

ഘടന ReplacementDFT2D : പൊതു hal::DFT2D
{
    cvhalDFT *context;
    തർക്കം isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(അസത്യം) {}
    തർക്കം init(പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ depth,
              പൂർണ്ണസംഖ്യ src_channels, പൂർണ്ണസംഖ്യ dst_channels,
              പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ nonzero_rows)
    {
        പൂർണ്ണസംഖ്യ res = cv_hal_dftInit2D(&context, width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        isInitialized = (res == CV_HAL_ERROR_OK);
        തിരികെ isInitialized;
    }
    ശൂന്യം apply(സ്ഥിരം uchar *src, size_t src_step, uchar *dst, size_t dst_step) CV_OVERRIDE
    {
        എങ്കിൽ (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_step, dst, dst_step);
        }
    }
    ~ReplacementDFT2D()
    {
        എങ്കിൽ (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

നാമാകാശ hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ flags, തർക്കം *needBuffer)
{
    {
        ReplacementDFT1D *impl = പുതിയ ReplacementDFT1D();
        എങ്കിൽ (impl->init(len, എണ്ണം_2, depth, flags, needBuffer))
        {
            തിരികെ Ptr<DFT1D>(impl);
        }
        delete impl;
    }
    {
        OcvDftBasicImpl *impl = പുതിയ OcvDftBasicImpl();
        impl->init(len, എണ്ണം_2, depth, flags, needBuffer);
        തിരികെ Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ depth,
                         പൂർണ്ണസംഖ്യ src_channels, പൂർണ്ണസംഖ്യ dst_channels,
                         പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ nonzero_rows)
{
    {
        ReplacementDFT2D *impl = പുതിയ ReplacementDFT2D();
        എങ്കിൽ (impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows))
        {
            തിരികെ Ptr<DFT2D>(impl);
        }
        delete impl;
    }
    {
        എങ്കിൽ(width == 1 && nonzero_rows > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = പുതിയ OcvDftImpl();
        impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        തിരികെ Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


ശൂന്യം cv::dft( നിവേശം_Array _src0, OutputArray _dst, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ nonzero_rows )
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CLAMDFFT
    CV_OCL_RUN(ocl::haveAmdFft() && ocl::Device::getDefault().type() != ocl::Device::TYPE_CPU &&
            _dst.isUMat() && _src0.dims() <= 2 && nonzero_rows == 0,
               ocl_dft_amdfft(_src0, _dst, flags))
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN(_dst.isUMat() && _src0.dims() <= 2,
               ocl_dft(_src0, _dst, flags, nonzero_rows))
#endif

    Mat src0 = _src0.getMat(), src = src0;
    എങ്കിൽ (src.empty())
    {
        _dst.release();
        തിരികെ;
    }
    തർക്കം inv = (flags & DFT_INVERSE) != 0;
    പൂർണ്ണസംഖ്യ type = src.type();
    പൂർണ്ണസംഖ്യ depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_നിവേശം) && src.channels() != 2) );

    എങ്കിൽ( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.createSameSize( src, CV_MAKETYPE(depth, 2) );
    അല്ലെങ്കിൽ എങ്കിൽ( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.createSameSize( src, depth );
    അല്ലെങ്കിൽ
        _dst.createSameSize( src, type );

    Mat dst = _dst.getMat();

    പൂർണ്ണസംഖ്യ f = 0;
    എങ്കിൽ (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    എങ്കിൽ (inv)
        f |= CV_HAL_DFT_INVERSE;
    എങ്കിൽ (flags & DFT_ROWS)
        f |= CV_HAL_DFT_ROWS;
    എങ്കിൽ (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    എങ്കിൽ (src.data == dst.data)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.cols, src.rows, depth, src.channels(), dst.channels(), f, nonzero_rows);
    c->apply(src.data, src.step, dst.data, dst.step);
}


ശൂന്യം cv::idft( നിവേശം_Array src, OutputArray dst, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ nonzero_rows )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_rows );
}

#ifdef HAVE_OPENCL

നാമാകാശ cv {

സ്ഥിതം തർക്കം ocl_mulSpectrums( നിവേശം_Array _srcA, നിവേശം_Array _srcB,
                              OutputArray _dst, പൂർണ്ണസംഖ്യ flags, തർക്കം conjB )
{
    പൂർണ്ണസംഖ്യ atype = _srcA.type(), btype = _srcB.type(),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    എങ്കിൽ ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        തിരികെ അസത്യം;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.createSameSize(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    k.args(ocl::KernelArg::ReadOnlyNoSize(A), ocl::KernelArg::ReadOnlyNoSize(B),
           ocl::KernelArg::WriteOnly(dst), rowsPerWI);

    size_t globalsize[2] = { (size_t)asize.width, ((size_t)asize.height + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

}

#endif

നാമാകാശ {

#define VAL(buf, elem) (((T*)((അക്ഷരം*)data ## buf + (step ## buf * (elem))))[0])
#define MUL_SPECTRUMS_COL(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    വേണ്ടി (size_t j = 1; j <= rows - 2; j += 2) \
    { \
        ഇരട്ട a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        ഇരട്ട b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        എങ്കിൽ (conjB) b_im = -b_im; \
        ഇരട്ട c_re = a_re * b_re - a_im * b_im; \
        ഇരട്ട c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    എങ്കിൽ ((rows & 1) == 0) \
        VAL(C, rows-1) = VAL(A, rows-1) * VAL(B, rows-1)

template <typename T, തർക്കം conjB> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_processCol_noinplace(സ്ഥിരം T* dataA, സ്ഥിരം T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows)
{
    MUL_SPECTRUMS_COL(A, B, C);
}

template <typename T, തർക്കം conjB> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_processCol_inplaceA(സ്ഥിരം T* dataB, T* dataAC, size_t stepB, size_t stepAC, size_t rows)
{
    MUL_SPECTRUMS_COL(AC, B, AC);
}
template <typename T, തർക്കം conjB, തർക്കം inplaceA> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_processCol(സ്ഥിരം T* dataA, സ്ഥിരം T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows)
{
    എങ്കിൽ (inplaceA)
        mulSpectrums_processCol_inplaceA<T, conjB>(dataB, dataC, stepB, stepC, rows);
    അല്ലെങ്കിൽ
        mulSpectrums_processCol_noinplace<T, conjB>(dataA, dataB, dataC, stepA, stepB, stepC, rows);
}
#undef MUL_SPECTRUMS_COL
#undef VAL

template <typename T, തർക്കം conjB, തർക്കം inplaceA> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_processCols(സ്ഥിരം T* dataA, സ്ഥിരം T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols)
{
    mulSpectrums_processCol<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows);
    എങ്കിൽ ((cols & 1) == 0)
    {
        mulSpectrums_processCol<T, conjB, inplaceA>(dataA + cols - 1, dataB + cols - 1, dataC + cols - 1, stepA, stepB, stepC, rows);
    }
}

#define VAL(buf, elem) (data ## buf[(elem)])
#define MUL_SPECTRUMS_ROW(A, B, C) \
    വേണ്ടി (size_t j = j0; j < j1; j += 2) \
    { \
        ഇരട്ട a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        ഇരട്ട b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        എങ്കിൽ (conjB) b_im = -b_im; \
        ഇരട്ട c_re = a_re * b_re - a_im * b_im; \
        ഇരട്ട c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
template <typename T, തർക്കം conjB> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_processRow_noinplace(സ്ഥിരം T* dataA, സ്ഥിരം T* dataB, T* dataC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(A, B, C);
}
template <typename T, തർക്കം conjB> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_processRow_inplaceA(സ്ഥിരം T* dataB, T* dataAC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(AC, B, AC);
}
template <typename T, തർക്കം conjB, തർക്കം inplaceA> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_processRow(സ്ഥിരം T* dataA, സ്ഥിരം T* dataB, T* dataC, size_t j0, size_t j1)
{
    എങ്കിൽ (inplaceA)
        mulSpectrums_processRow_inplaceA<T, conjB>(dataB, dataC, j0, j1);
    അല്ലെങ്കിൽ
        mulSpectrums_processRow_noinplace<T, conjB>(dataA, dataB, dataC, j0, j1);
}
#undef MUL_SPECTRUMS_ROW
#undef VAL

template <typename T, തർക്കം conjB, തർക്കം inplaceA> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_processRows(സ്ഥിരം T* dataA, സ്ഥിരം T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, തർക്കം is_1d_CN1)
{
    ആകുമ്പോൾ (rows-- > 0)
    {
        എങ്കിൽ (is_1d_CN1)
            dataC[0] = dataA[0]*dataB[0];
        mulSpectrums_processRow<T, conjB, inplaceA>(dataA, dataB, dataC, j0, j1);
        എങ്കിൽ (is_1d_CN1 && (cols & 1) == 0)
            dataC[j1] = dataA[j1]*dataB[j1];

        dataA = (സ്ഥിരം T*)(((അക്ഷരം*)dataA) + stepA);
        dataB = (സ്ഥിരം T*)(((അക്ഷരം*)dataB) + stepB);
        dataC =       (T*)(((അക്ഷരം*)dataC) + stepC);
    }
}


template <typename T, തർക്കം conjB, തർക്കം inplaceA> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_Impl_(സ്ഥിരം T* dataA, സ്ഥിരം T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, തർക്കം is_1d, തർക്കം isCN1)
{
    എങ്കിൽ (!is_1d && isCN1)
    {
        mulSpectrums_processCols<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols);
    }
    mulSpectrums_processRows<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d && isCN1);
}
template <typename T, തർക്കം conjB> സ്ഥിതം വരിയിൽ
ശൂന്യം mulSpectrums_Impl(സ്ഥിരം T* dataA, സ്ഥിരം T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, തർക്കം is_1d, തർക്കം isCN1)
{
    എങ്കിൽ (dataA == dataC)
        mulSpectrums_Impl_<T, conjB, സത്യം>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
    അല്ലെങ്കിൽ
        mulSpectrums_Impl_<T, conjB, അസത്യം>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
}

} // namespace

ശൂന്യം cv::mulSpectrums( നിവേശം_Array _srcA, നിവേശം_Array _srcB,
                       OutputArray _dst, പൂർണ്ണസംഖ്യ flags, തർക്കം conjB )
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        തിരികെ;
    }

    പൂർണ്ണസംഖ്യ type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_RUN(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    പൂർണ്ണസംഖ്യ depth = srcA.depth(), cn = srcA.channels();
    size_t rows = srcA.rows, cols = srcA.cols;

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    എങ്കിൽ (dst.data == srcB.data)
        srcB = srcB.clone(); // workaround for B only

    തർക്കം is_1d = (flags & DFT_ROWS)
        || (rows == 1)
        || (cols == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    എങ്കിൽ( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    തർക്കം isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = cols*cn - (((cols & 1) == 0 && cn == 1) ? 1 : 0);

    എങ്കിൽ (depth == CV_32F)
    {
        സ്ഥിരം ദശാംശം* dataA = srcA.ptr<ദശാംശം>();
        സ്ഥിരം ദശാംശം* dataB = srcB.ptr<ദശാംശം>();
        ദശാംശം* dataC = dst.ptr<ദശാംശം>();
        എങ്കിൽ (!conjB)
            mulSpectrums_Impl<ദശാംശം, അസത്യം>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        അല്ലെങ്കിൽ
            mulSpectrums_Impl<ദശാംശം, സത്യം>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം ഇരട്ട* dataA = srcA.ptr<ഇരട്ട>();
        സ്ഥിരം ഇരട്ട* dataB = srcB.ptr<ഇരട്ട>();
        ഇരട്ട* dataC = dst.ptr<ഇരട്ട>();
        എങ്കിൽ (!conjB)
            mulSpectrums_Impl<ഇരട്ട, അസത്യം>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        അല്ലെങ്കിൽ
            mulSpectrums_Impl<ഇരട്ട, സത്യം>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
}

ശൂന്യം cv::divSpectrums( നിവേശം_Array _srcA, നിവേശം_Array _srcB, OutputArray _dst, പൂർണ്ണസംഖ്യ flags, തർക്കം conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    എങ്കിൽ (srcA.empty() && srcB.empty())
    {
        _dst.release();
        തിരികെ;
    }
    പൂർണ്ണസംഖ്യ depth = srcA.depth(), cn = srcA.channels(), type = srcA.type();
    പൂർണ്ണസംഖ്യ rows = srcA.rows, cols = srcA.cols;
    പൂർണ്ണസംഖ്യ j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.data != srcA.data); // non-inplace check
    CV_Assert(dst.data != srcB.data); // non-inplace check

    തർക്കം is_1d = (flags & DFT_ROWS) || (rows == 1 || (cols == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    എങ്കിൽ( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    പൂർണ്ണസംഖ്യ ncols = cols*cn;
    പൂർണ്ണസംഖ്യ j0 = cn == 1;
    പൂർണ്ണസംഖ്യ j1 = ncols - (cols % 2 == 0 && cn == 1);

    എങ്കിൽ( depth == CV_32F )
    {
        സ്ഥിരം ദശാംശം* dataA = srcA.ptr<ദശാംശം>();
        സ്ഥിരം ദശാംശം* dataB = srcB.ptr<ദശാംശം>();
        ദശാംശം* dataC = dst.ptr<ദശാംശം>();
        ദശാംശം eps = FLT_EPSILON; // prevent div0 problems

        size_t stepA = srcA.step/വലുപ്പം(dataA[0]);
        size_t stepB = srcB.step/വലുപ്പം(dataB[0]);
        size_t stepC = dst.step/വലുപ്പം(dataC[0]);

        എങ്കിൽ( !is_1d && cn == 1 )
        {
            വേണ്ടി( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                എങ്കിൽ( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0] / (dataB[0] + eps);
                എങ്കിൽ( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA] / (dataB[(rows-1)*stepB] + eps);
                എങ്കിൽ( !conjB )
                    വേണ്ടി( j = 1; j <= rows - 2; j += 2 )
                    {
                        ഇരട്ട denom = (ഇരട്ട)dataB[j*stepB]*dataB[j*stepB] +
                        (ഇരട്ട)dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + (ഇരട്ട)eps;

                        ഇരട്ട re = (ഇരട്ട)dataA[j*stepA]*dataB[j*stepB] +
                        (ഇരട്ട)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        ഇരട്ട im = (ഇരട്ട)dataA[(j+1)*stepA]*dataB[j*stepB] -
                        (ഇരട്ട)dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = (ദശാംശം)(re / denom);
                        dataC[(j+1)*stepC] = (ദശാംശം)(im / denom);
                    }
                    അല്ലെങ്കിൽ
                        വേണ്ടി( j = 1; j <= rows - 2; j += 2 )
                        {

                            ഇരട്ട denom = (ഇരട്ട)dataB[j*stepB]*dataB[j*stepB] +
                            (ഇരട്ട)dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + (ഇരട്ട)eps;

                            ഇരട്ട re = (ഇരട്ട)dataA[j*stepA]*dataB[j*stepB] -
                            (ഇരട്ട)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                            ഇരട്ട im = (ഇരട്ട)dataA[(j+1)*stepA]*dataB[j*stepB] +
                            (ഇരട്ട)dataA[j*stepA]*dataB[(j+1)*stepB];

                            dataC[j*stepC] = (ദശാംശം)(re / denom);
                            dataC[(j+1)*stepC] = (ദശാംശം)(im / denom);
                        }
                    എങ്കിൽ( k == 1 )
                        dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        വേണ്ടി( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            എങ്കിൽ( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0] / (dataB[0] + eps);
                എങ്കിൽ( cols % 2 == 0 )
                    dataC[j1] = dataA[j1] / (dataB[j1] + eps);
            }

            എങ്കിൽ( !conjB )
                വേണ്ടി( j = j0; j < j1; j += 2 )
                {
                    ഇരട്ട denom = (ഇരട്ട)dataB[j]*dataB[j] + (ഇരട്ട)dataB[j+1]*dataB[j+1] + (ഇരട്ട)eps;
                    ഇരട്ട re = (ഇരട്ട)dataA[j]*dataB[j] + (ഇരട്ട)dataA[j+1]*dataB[j+1];
                    ഇരട്ട im = (ഇരട്ട)dataA[j+1]*dataB[j] - (ഇരട്ട)dataA[j]*dataB[j+1];
                    dataC[j] = (ദശാംശം)(re / denom);
                    dataC[j+1] = (ദശാംശം)(im / denom);
                }
                അല്ലെങ്കിൽ
                    വേണ്ടി( j = j0; j < j1; j += 2 )
                    {
                        ഇരട്ട denom = (ഇരട്ട)dataB[j]*dataB[j] + (ഇരട്ട)dataB[j+1]*dataB[j+1] + (ഇരട്ട)eps;
                        ഇരട്ട re = (ഇരട്ട)dataA[j]*dataB[j] - (ഇരട്ട)dataA[j+1]*dataB[j+1];
                        ഇരട്ട im = (ഇരട്ട)dataA[j+1]*dataB[j] + (ഇരട്ട)dataA[j]*dataB[j+1];
                        dataC[j] = (ദശാംശം)(re / denom);
                        dataC[j+1] = (ദശാംശം)(im / denom);
                    }
        }
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം ഇരട്ട* dataA = srcA.ptr<ഇരട്ട>();
        സ്ഥിരം ഇരട്ട* dataB = srcB.ptr<ഇരട്ട>();
        ഇരട്ട* dataC = dst.ptr<ഇരട്ട>();
        ഇരട്ട eps = DBL_EPSILON; // prevent div0 problems

        size_t stepA = srcA.step/വലുപ്പം(dataA[0]);
        size_t stepB = srcB.step/വലുപ്പം(dataB[0]);
        size_t stepC = dst.step/വലുപ്പം(dataC[0]);

        എങ്കിൽ( !is_1d && cn == 1 )
        {
            വേണ്ടി( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                എങ്കിൽ( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0] / (dataB[0] + eps);
                എങ്കിൽ( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA] / (dataB[(rows-1)*stepB] + eps);
                എങ്കിൽ( !conjB )
                    വേണ്ടി( j = 1; j <= rows - 2; j += 2 )
                    {
                        ഇരട്ട denom = dataB[j*stepB]*dataB[j*stepB] +
                        dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + eps;

                        ഇരട്ട re = dataA[j*stepA]*dataB[j*stepB] +
                        dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        ഇരട്ട im = dataA[(j+1)*stepA]*dataB[j*stepB] -
                        dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = re / denom;
                        dataC[(j+1)*stepC] = im / denom;
                    }
                    അല്ലെങ്കിൽ
                        വേണ്ടി( j = 1; j <= rows - 2; j += 2 )
                        {
                            ഇരട്ട denom = dataB[j*stepB]*dataB[j*stepB] +
                            dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + eps;

                            ഇരട്ട re = dataA[j*stepA]*dataB[j*stepB] -
                            dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                            ഇരട്ട im = dataA[(j+1)*stepA]*dataB[j*stepB] +
                            dataA[j*stepA]*dataB[(j+1)*stepB];

                            dataC[j*stepC] = re / denom;
                            dataC[(j+1)*stepC] = im / denom;
                        }
                    എങ്കിൽ( k == 1 )
                        dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        വേണ്ടി( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            എങ്കിൽ( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0] / (dataB[0] + eps);
                എങ്കിൽ( cols % 2 == 0 )
                    dataC[j1] = dataA[j1] / (dataB[j1] + eps);
            }

            എങ്കിൽ( !conjB )
                വേണ്ടി( j = j0; j < j1; j += 2 )
                {
                    ഇരട്ട denom = dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps;
                    ഇരട്ട re = dataA[j]*dataB[j] + dataA[j+1]*dataB[j+1];
                    ഇരട്ട im = dataA[j+1]*dataB[j] - dataA[j]*dataB[j+1];
                    dataC[j] = re / denom;
                    dataC[j+1] = im / denom;
                }
                അല്ലെങ്കിൽ
                    വേണ്ടി( j = j0; j < j1; j += 2 )
                    {
                        ഇരട്ട denom = dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps;
                        ഇരട്ട re = dataA[j]*dataB[j] - dataA[j+1]*dataB[j+1];
                        ഇരട്ട im = dataA[j+1]*dataB[j] + dataA[j]*dataB[j+1];
                        dataC[j] = re / denom;
                        dataC[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

നാമാകാശ cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
template<typename T> സ്ഥിതം ശൂന്യം
DCT( സ്ഥിരം OcvDftOptions & c, സ്ഥിരം T* src, size_t src_step, T* dst, size_t dst_step )
{
    എങ്കിൽ( c.n == 1 )
    {
        dst[0] = src[0];
        തിരികെ;
    }
    runDft(c, src, src_step/വലുപ്പം(T), dst, dst_step/വലുപ്പം(T), അസത്യം);
}

template<typename T> സ്ഥിതം ശൂന്യം
IDCT( സ്ഥിരം OcvDftOptions & c, സ്ഥിരം T* src, size_t src_step, T* dst, size_t dst_step )
{
    എങ്കിൽ( c.n == 1 )
    {
        dst[0] = src[0];
        തിരികെ;
    }
    runDft(c, src, src_step/വലുപ്പം(T), dst, dst_step/വലുപ്പം(T), അസത്യം);
}

തരനിർവചനം ശൂന്യം (*DCTFunc)(സ്ഥിരം OcvDftOptions & c, സ്ഥിരം ശൂന്യം* src, size_t src_step,
                        ശൂന്യം* dst, size_t dst_step);

template<typename T, ശൂന്യം (*fn)(സ്ഥിരം OcvDftOptions&, സ്ഥിരം T*, size_t, T*, size_t)>
സ്ഥിതം ശൂന്യം dctWrap(സ്ഥിരം OcvDftOptions & c, സ്ഥിരം ശൂന്യം* src, size_t src_step, ശൂന്യം* dst, size_t dst_step)
{
    fn(c, (സ്ഥിരം T*)src, src_step, (T*)dst, dst_step);
}

}

#ifdef HAVE_IPP
നാമാകാശ cv
{

#എങ്കിൽ IPP_VERSION_X100 >= 900
തരനിർവചനം IppStatus (CV_STDCALL * ippiDCTFunc)(സ്ഥിരം Ipp32f* pSrc, പൂർണ്ണസംഖ്യ srcStep, Ipp32f* pDst, പൂർണ്ണസംഖ്യ dstStep, സ്ഥിരം ശൂന്യം* pDCTSpec, Ipp8u* pBuffer);
തരനിർവചനം IppStatus (CV_STDCALL * ippiDCTInit)(ശൂന്യം* pDCTSpec, IppiSize roiSize, Ipp8u* pMemInit );
തരനിർവചനം IppStatus (CV_STDCALL * ippiDCTGetSize)(IppiSize roiSize, പൂർണ്ണസംഖ്യ* pSizeSpec, പൂർണ്ണസംഖ്യ* pSizeInit, പൂർണ്ണസംഖ്യ* pSizeBuf);

template<typename SpecType, IppStatus (CV_STDCALL *fn)(സ്ഥിരം Ipp32f*, പൂർണ്ണസംഖ്യ, Ipp32f*, പൂർണ്ണസംഖ്യ, സ്ഥിരം SpecType*, Ipp8u*)>
സ്ഥിതം IppStatus CV_STDCALL ippiDCTWrap(സ്ഥിരം Ipp32f* pSrc, പൂർണ്ണസംഖ്യ srcStep, Ipp32f* pDst, പൂർണ്ണസംഖ്യ dstStep, സ്ഥിരം ശൂന്യം* pDCTSpec, Ipp8u* pBuffer)
{
    തിരികെ fn(pSrc, srcStep, pDst, dstStep, (സ്ഥിരം SpecType*)pDCTSpec, pBuffer);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*, IppiSize, Ipp8u*)>
സ്ഥിതം IppStatus CV_STDCALL ippiDCTInitWrap(ശൂന്യം* pDCTSpec, IppiSize roiSize, Ipp8u* pMemInit)
{
    തിരികെ fn((SpecType*)pDCTSpec, roiSize, pMemInit);
}

template<IppStatus (CV_STDCALL *fn)(IppiSize, പൂർണ്ണസംഖ്യ*, പൂർണ്ണസംഖ്യ*, പൂർണ്ണസംഖ്യ*)>
സ്ഥിതം IppStatus CV_STDCALL ippiDCTGetSizeWrap(IppiSize roiSize, പൂർണ്ണസംഖ്യ* pSizeSpec, പൂർണ്ണസംഖ്യ* pSizeInit, പൂർണ്ണസംഖ്യ* pSizeBuf)
{
    തിരികെ fn(roiSize, pSizeSpec, pSizeInit, pSizeBuf);
}

#അല്ലെങ്കിലെങ്കിൽ IPP_VERSION_X100 >= 700
തരനിർവചനം IppStatus (CV_STDCALL * ippiDCTFunc)(സ്ഥിരം Ipp32f*, പൂർണ്ണസംഖ്യ, Ipp32f*, പൂർണ്ണസംഖ്യ, സ്ഥിരം ശൂന്യം*, Ipp8u*);
തരനിർവചനം IppStatus (CV_STDCALL * ippiDCTInitAlloc)(ശൂന്യം**, IppiSize, IppHintAlgorithm);
തരനിർവചനം IppStatus (CV_STDCALL * ippiDCTFree)(ശൂന്യം* pDCTSpec);
തരനിർവചനം IppStatus (CV_STDCALL * ippiDCTGetBufSize)(സ്ഥിരം ശൂന്യം*, പൂർണ്ണസംഖ്യ*);

template<typename SpecType, IppStatus (CV_STDCALL *fn)(സ്ഥിരം Ipp32f*, പൂർണ്ണസംഖ്യ, Ipp32f*, പൂർണ്ണസംഖ്യ, സ്ഥിരം SpecType*, Ipp8u*)>
സ്ഥിതം IppStatus CV_STDCALL ippiDCTWrap(സ്ഥിരം Ipp32f* pSrc, പൂർണ്ണസംഖ്യ srcStep, Ipp32f* pDst, പൂർണ്ണസംഖ്യ dstStep, സ്ഥിരം ശൂന്യം* pDCTSpec, Ipp8u* pBuffer)
{
    തിരികെ fn(pSrc, srcStep, pDst, dstStep, (സ്ഥിരം SpecType*)pDCTSpec, pBuffer);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType**, IppiSize, IppHintAlgorithm)>
സ്ഥിതം IppStatus CV_STDCALL ippiDCTInitAllocWrap(ശൂന്യം** pDCTSpec, IppiSize roiSize, IppHintAlgorithm hint)
{
    തിരികെ fn((SpecType**)pDCTSpec, roiSize, hint);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*)>
സ്ഥിതം IppStatus CV_STDCALL ippiDCTFreeWrap(ശൂന്യം* pDCTSpec)
{
    തിരികെ fn((SpecType*)pDCTSpec);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(സ്ഥിരം SpecType*, പൂർണ്ണസംഖ്യ*)>
സ്ഥിതം IppStatus CV_STDCALL ippiDCTGetBufSizeWrap(സ്ഥിരം ശൂന്യം* pDCTSpec, പൂർണ്ണസംഖ്യ* pSize)
{
    തിരികെ fn((സ്ഥിരം SpecType*)pDCTSpec, pSize);
}
#endif

വർഗ്ഗം DctIPPLoop_Invoker : പൊതു ParallelLoopBody
{
പൊതു:
    DctIPPLoop_Invoker(സ്ഥിരം uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, പൂർണ്ണസംഖ്യ _width, തർക്കം _inv, തർക്കം *_ok) :
        ParallelLoopBody(), src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width), inv(_inv), ok(_ok)
    {
        *ok = സത്യം;
    }

    മായാ ശൂന്യം operator()(സ്ഥിരം Range& range) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(*ok == അസത്യം)
            തിരികെ;

#എങ്കിൽ IPP_VERSION_X100 >= 900
        IppiSize srcRoiSize = {width, 1};

        പൂർണ്ണസംഖ്യ specSize    = 0;
        പൂർണ്ണസംഖ്യ initSize    = 0;
        പൂർണ്ണസംഖ്യ bufferSize  = 0;

        Ipp8u* pDCTSpec = ശൂന്യ;
        Ipp8u* pBuffer  = ശൂന്യ;
        Ipp8u* pInitBuf = ശൂന്യ;

        #define IPP_RETURN              \
            എങ്കിൽ(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            എങ്കിൽ(pBuffer)                 \
                ippFree(pBuffer);       \
            എങ്കിൽ(pInitBuf)                \
                ippFree(pInitBuf);      \
            തിരികെ;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippiDCTGetSize  ippDctGetSize  = inv ? ippiDCTGetSizeWrap<ippiDCTInvGetSize_32f> : ippiDCTGetSizeWrap<ippiDCTFwdGetSize_32f>;

        എങ്കിൽ(ippDctGetSize(srcRoiSize, &specSize, &initSize, &bufferSize) < 0)
        {
            *ok = അസത്യം;
            തിരികെ;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(specSize);
        എങ്കിൽ(!pDCTSpec && specSize)
        {
            *ok = അസത്യം;
            തിരികെ;
        }

        pBuffer  = (Ipp8u*)CV_IPP_MALLOC(bufferSize);
        എങ്കിൽ(!pBuffer && bufferSize)
        {
            *ok = അസത്യം;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(initSize);
        എങ്കിൽ(!pInitBuf && initSize)
        {
            *ok = അസത്യം;
            IPP_RETURN
        }

        എങ്കിൽ(ippDctInit(pDCTSpec, srcRoiSize, pInitBuf) < 0)
        {
            *ok = അസത്യം;
            IPP_RETURN
        }

        വേണ്ടി(പൂർണ്ണസംഖ്യ i = range.start; i < range.end; ++i)
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (ദശാംശം*)(src + src_step * i), static_cast<പൂർണ്ണസംഖ്യ>(src_step), (ദശാംശം*)(dst + dst_step * i), static_cast<പൂർണ്ണസംഖ്യ>(dst_step), pDCTSpec, pBuffer) < 0)
            {
                *ok = അസത്യം;
                IPP_RETURN
            }
        }
        IPP_RETURN
#undef IPP_RETURN
#അല്ലെങ്കിലെങ്കിൽ IPP_VERSION_X100 >= 700
        ശൂന്യം* pDCTSpec;
        AutoBuffer<uchar> buf;
        uchar* pBuffer = 0;
        പൂർണ്ണസംഖ്യ bufSize=0;

        IppiSize srcRoiSize = {width, 1};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippiDCTGetBufSize ippGetBufSize = inv ? ippiDCTGetBufSizeWrap<IppiDCTInvSpec_32f, ippiDCTInvGetBufSize_32f> : ippiDCTGetBufSizeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdGetBufSize_32f>;

        എങ്കിൽ (ippInitAlloc(&pDCTSpec, srcRoiSize, ippAlgHintNone)>=0 && ippGetBufSize(pDCTSpec, &bufSize)>=0)
        {
            buf.allocate( bufSize );
            pBuffer = (uchar*)buf;

            വേണ്ടി( പൂർണ്ണസംഖ്യ i = range.start; i < range.end; ++i)
            {
                എങ്കിൽ(ippDctFun((ദശാംശം*)(src + src_step * i), static_cast<പൂർണ്ണസംഖ്യ>(src_step), (ദശാംശം*)(dst + dst_step * i), static_cast<പൂർണ്ണസംഖ്യ>(dst_step), pDCTSpec, (Ipp8u*)pBuffer) < 0)
                {
                    *ok = അസത്യം;
                    നിർത്തുക;
                }
            }
        }
        അല്ലെങ്കിൽ
            *ok = അസത്യം;

        എങ്കിൽ (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END
#അല്ലെങ്കിൽ
        CV_UNUSED(range);
        *ok = അസത്യം;
#endif
    }

സ്വകാര്യ:
    സ്ഥിരം uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    പൂർണ്ണസംഖ്യ width;
    തർക്കം inv;
    തർക്കം *ok;
};

സ്ഥിതം തർക്കം DctIPPLoop(സ്ഥിരം uchar * src, size_t src_step, uchar * dst, size_t dst_step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, തർക്കം inv)
{
    തർക്കം ok;
    parallel_for_(Range(0, height), DctIPPLoop_Invoker(src, src_step, dst, dst_step, width, inv, &ok), height/(ഇരട്ട)(1<<4) );
    തിരികെ ok;
}

സ്ഥിതം തർക്കം ippi_DCT_32f(സ്ഥിരം uchar * src, size_t src_step, uchar * dst, size_t dst_step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, തർക്കം inv, തർക്കം row)
{
    CV_INSTRUMENT_REGION_IPP();

    എങ്കിൽ(row)
        തിരികെ DctIPPLoop(src, src_step, dst, dst_step, width, height, inv);
    അല്ലെങ്കിൽ
    {
#എങ്കിൽ IPP_VERSION_X100 >= 900
        IppiSize srcRoiSize = {width, height};

        പൂർണ്ണസംഖ്യ specSize    = 0;
        പൂർണ്ണസംഖ്യ initSize    = 0;
        പൂർണ്ണസംഖ്യ bufferSize  = 0;

        Ipp8u* pDCTSpec = ശൂന്യ;
        Ipp8u* pBuffer  = ശൂന്യ;
        Ipp8u* pInitBuf = ശൂന്യ;

        #define IPP_RELEASE             \
            എങ്കിൽ(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            എങ്കിൽ(pBuffer)                 \
                ippFree(pBuffer);       \
            എങ്കിൽ(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippiDCTGetSize  ippDctGetSize  = inv ? ippiDCTGetSizeWrap<ippiDCTInvGetSize_32f> : ippiDCTGetSizeWrap<ippiDCTFwdGetSize_32f>;

        എങ്കിൽ(ippDctGetSize(srcRoiSize, &specSize, &initSize, &bufferSize) < 0)
            തിരികെ അസത്യം;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(specSize);
        എങ്കിൽ(!pDCTSpec && specSize)
            തിരികെ അസത്യം;

        pBuffer  = (Ipp8u*)CV_IPP_MALLOC(bufferSize);
        എങ്കിൽ(!pBuffer && bufferSize)
        {
            IPP_RELEASE
            തിരികെ അസത്യം;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(initSize);
        എങ്കിൽ(!pInitBuf && initSize)
        {
            IPP_RELEASE
            തിരികെ അസത്യം;
        }

        എങ്കിൽ(ippDctInit(pDCTSpec, srcRoiSize, pInitBuf) < 0)
        {
            IPP_RELEASE
            തിരികെ അസത്യം;
        }

        എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (ദശാംശം*)src, static_cast<പൂർണ്ണസംഖ്യ>(src_step), (ദശാംശം*)dst, static_cast<പൂർണ്ണസംഖ്യ>(dst_step), pDCTSpec, pBuffer) < 0)
        {
            IPP_RELEASE
            തിരികെ അസത്യം;
        }

        IPP_RELEASE
        തിരികെ സത്യം;
#undef IPP_RELEASE
#അല്ലെങ്കിലെങ്കിൽ IPP_VERSION_X100 >= 700
        IppStatus status;
        ശൂന്യം* pDCTSpec;
        AutoBuffer<uchar> buf;
        uchar* pBuffer = 0;
        പൂർണ്ണസംഖ്യ bufSize=0;

        IppiSize srcRoiSize = {width, height};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippiDCTGetBufSize ippGetBufSize = inv ? ippiDCTGetBufSizeWrap<IppiDCTInvSpec_32f, ippiDCTInvGetBufSize_32f> : ippiDCTGetBufSizeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdGetBufSize_32f>;

        status = ippStsErr;

        എങ്കിൽ (ippInitAlloc(&pDCTSpec, srcRoiSize, ippAlgHintNone)>=0 && ippGetBufSize(pDCTSpec, &bufSize)>=0)
        {
            buf.allocate( bufSize );
            pBuffer = (uchar*)buf;

            status = ippDctFun((ദശാംശം*)src, static_cast<പൂർണ്ണസംഖ്യ>(src_step), (ദശാംശം*)dst, static_cast<പൂർണ്ണസംഖ്യ>(dst_step), pDCTSpec, (Ipp8u*)pBuffer);
        }

        എങ്കിൽ (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END

        തിരികെ status >= 0;
#അല്ലെങ്കിൽ
        CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(inv); CV_UNUSED(row);
        തിരികെ അസത്യം;
#endif
    }
}
}
#endif

നാമാകാശ cv {

വർഗ്ഗം OcvDctImpl CV_FINAL : പൊതു hal::DCT2D
{
പൊതു:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    AutoBuffer<uchar> ws;
    തർക്കം isRowTransform;
    തർക്കം isInverse;
    തർക്കം isContinuous;
    പൂർണ്ണസംഖ്യ start_stage;
    പൂർണ്ണസംഖ്യ end_stage;
    പൂർണ്ണസംഖ്യ width;
    പൂർണ്ണസംഖ്യ height;
    പൂർണ്ണസംഖ്യ depth;

    ശൂന്യം init(പൂർണ്ണസംഖ്യ _width, പൂർണ്ണസംഖ്യ _height, പൂർണ്ണസംഖ്യ _depth, പൂർണ്ണസംഖ്യ flags)
    {
        width = _width;
        height = _height;
        depth = _depth;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        സ്ഥിതം DCTFunc dct_tbl[4] =
        {
            dctWrap<ദശാംശം, DCT<ദശാംശം>>,
            dctWrap<ദശാംശം, IDCT<ദശാംശം>>,
            dctWrap<ഇരട്ട, DCT<ഇരട്ട>>,
            dctWrap<ഇരട്ട, IDCT<ഇരട്ട>>
        };
        dct_func = dct_tbl[(പൂർണ്ണസംഖ്യ)isInverse + (depth == CV_64F)*2];
        opt.isComplex = അസത്യം;
        opt.isInverse = അസത്യം;
        opt.scale = 1.;

        എങ്കിൽ (isRowTransform || height == 1 || (width == 1 && isContinuous))
        {
            start_stage = end_stage = 0;
        }
        അല്ലെങ്കിൽ
        {
            start_stage = (width == 1);
            end_stage = 1;
        }
    }
    ശൂന്യം apply(സ്ഥിരം uchar *src, size_t src_step, uchar *dst, size_t dst_step) CV_OVERRIDE
    {
        CV_IPP_RUN(IPP_VERSION_X100 >= 700 && depth == CV_32F, ippi_DCT_32f(src, src_step, dst, dst_step, width, height, isInverse, isRowTransform))

        പൂർണ്ണസംഖ്യ prev_len = 0;
        പൂർണ്ണസംഖ്യ elem_size = (depth == CV_32F) ? വലുപ്പം(ദശാംശം) : വലുപ്പം(ഇരട്ട);
        സ്ഥിരം DFTKernels& kernels = getDFTKernels(depth);

        വേണ്ടി(പൂർണ്ണസംഖ്യ stage = start_stage ; stage <= end_stage; stage++ )
        {
            സ്ഥിരം uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            പൂർണ്ണസംഖ്യ len, എണ്ണം_2;

            എങ്കിൽ( stage == 0 )
            {
                len = width;
                എണ്ണം_2 = height;
                എങ്കിൽ( len == 1 && !isRowTransform )
                {
                    len = height;
                    എണ്ണം_2 = 1;
                }
                sstep0 = src_step;
                dstep0 = dst_step;
                sstep1 = dstep1 = elem_size;
            }
            അല്ലെങ്കിൽ
            {
                len = height;
                എണ്ണം_2 = width;
                sstep1 = src_step;
                dstep1 = dst_step;
                sstep0 = dstep0 = elem_size;
            }

            opt.n = len;

            എങ്കിൽ( len != prev_len )
            {
                എങ്കിൽ( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                എങ്കിൽ( len > 1 )
                {
                    plan.build(isInverse ? DFT_KIND_IDCT : DFT_KIND_DCT, len, depth, kernels.vlanes);
                    ws.allocate(plan.ws_bytes);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = ws.data();
                prev_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            ഇരട്ട nstripes = dftParallelStripes(എണ്ണം_2, len);
            എങ്കിൽ( len > 1 && എണ്ണം_2 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                auto process_വരികൾ = [&](സ്ഥിരം Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    AutoBuffer<uchar> ws_(plan.ws_bytes);
                    opt_.workspace = ws_.data();
                    വേണ്ടി( പൂർണ്ണസംഖ്യ i = range.start; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, എണ്ണം_2), process_വരികൾ, nstripes);
            }
            അല്ലെങ്കിൽ
            {
                വേണ്ടി(unsigned i = 0; i < static_cast<unsigned>(എണ്ണം_2); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_step = dst_step;
        }
    }
};

ഘടന ReplacementDCT2D : പൊതു hal::DCT2D
{
    cvhalDFT *context;
    തർക്കം isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(അസത്യം) {}
    തർക്കം init(പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ flags)
    {
        പൂർണ്ണസംഖ്യ res = cv_hal_dctInit2D(&context, width, height, depth, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        തിരികെ isInitialized;
    }
    ശൂന്യം apply(സ്ഥിരം uchar *src_data, size_t src_step, uchar *dst_data, size_t dst_step) CV_OVERRIDE
    {
        എങ്കിൽ (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_data, src_step, dst_data, dst_step);
        }
    }
    ~ReplacementDCT2D()
    {
        എങ്കിൽ (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

നാമാകാശ hal {

Ptr<DCT2D> DCT2D::create(പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ flags)
{
    {
        ReplacementDCT2D *impl = പുതിയ ReplacementDCT2D();
        എങ്കിൽ (impl->init(width, height, depth, flags))
        {
            തിരികെ Ptr<DCT2D>(impl);
        }
        delete impl;
    }
    {
        OcvDctImpl *impl = പുതിയ OcvDctImpl();
        impl->init(width, height, depth, flags);
        തിരികെ Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

ശൂന്യം cv::dct( നിവേശം_Array _src0, OutputArray _dst, പൂർണ്ണസംഖ്യ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    എങ്കിൽ (src.empty())
    {
        _dst.release();
        തിരികെ;
    }
    പൂർണ്ണസംഖ്യ type = src.type(), depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.rows, src.cols, type );
    Mat dst = _dst.getMat();

    പൂർണ്ണസംഖ്യ f = 0;
    എങ്കിൽ ((flags & DFT_ROWS) != 0)
        f |= CV_HAL_DFT_ROWS;
    എങ്കിൽ ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    എങ്കിൽ (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.cols, src.rows, depth, f);
    c->apply(src.data, src.step, dst.data, dst.step);
}


ശൂന്യം cv::idct( നിവേശം_Array src, OutputArray dst, പൂർണ്ണസംഖ്യ flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

നാമാകാശ cv
{

സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ optimalDFTSizeTab[] = {
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

പൂർണ്ണസംഖ്യ cv::getOptimalDFTSize( പൂർണ്ണസംഖ്യ size0 )
{
    പൂർണ്ണസംഖ്യ a = 0, b = വലുപ്പം(optimalDFTSizeTab)/വലുപ്പം(optimalDFTSizeTab[0]) - 1;
    എങ്കിൽ( (unsigned)size0 >= (unsigned)optimalDFTSizeTab[b] )
        തിരികെ -1;

    ആകുമ്പോൾ( a < b )
    {
        പൂർണ്ണസംഖ്യ c = (a + b) >> 1;
        എങ്കിൽ( size0 <= optimalDFTSizeTab[c] )
            b = c;
        അല്ലെങ്കിൽ
            a = c+1;
    }

    തിരികെ optimalDFTSizeTab[b];
}
/* End of file. */
