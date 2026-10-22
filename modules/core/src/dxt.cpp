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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_core.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "dxt.hpp"
#அந்தர்பாவ "dxt.simd.hpp"
#அந்தர்பாவ "dxt.simd_declarations.hpp"
#அந்தர்பாவ <map>

நாமாகாஶ cv
{

#எனில் IPP_VERSION_X100 >= 710
#define USE_IPP_DFT 1
#இல்லையெனில்
#undef USE_IPP_DFT
#endif

#எனில் defined USE_IPP_DFT
#எனில் IPP_VERSION_X100 >= 202220
#define IPP_DISABLE_DFT32F ((depth == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#இல்லையெனில்
#define IPP_DISABLE_DFT32F பொய்
#endif
#endif

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#ifdef HAVE_OPENCL
// factorization used by the OpenCL radix selection
நிலையான முழு
DFTFactorize( முழு n, முழு* factors )
{
    முழு nf = 0, f, i, j;

    எனில்( n <= 5 )
    {
        factors[0] = n;
        திரும்பு 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    எனில்( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    க்கு( f = 3; n > 1; )
    {
        முழு d = n/f;
        எனில்( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        இல்லையெனில்
        {
            f += 2;
            எனில்( f*f > n )
                நிறுத்து;
        }
    }

    எனில்( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    க்கு( i = f; i < (nf+f)/2; i++ )
        CV_SWAP( factors[i], factors[nf-i-1+f], j );

    திரும்பு nf;
}
#endif

#ifdef USE_IPP_DFT
நிலையான IppStatus ippsDFTFwd_CToC( மாறிலி Complex<தசம>* src, Complex<தசம>* dst,
                             மாறிலி வெறுமை* spec, uchar* buf)
{
    திரும்பு CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (மாறிலி Ipp32fc*)src, (Ipp32fc*)dst,
                                 (மாறிலி IppsDFTSpec_C_32fc*)spec, buf);
}

நிலையான IppStatus ippsDFTFwd_CToC( மாறிலி Complex<இரட்டை>* src, Complex<இரட்டை>* dst,
                             மாறிலி வெறுமை* spec, uchar* buf)
{
    திரும்பு CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (மாறிலி Ipp64fc*)src, (Ipp64fc*)dst,
                                 (மாறிலி IppsDFTSpec_C_64fc*)spec, buf);
}

நிலையான IppStatus ippsDFTInv_CToC( மாறிலி Complex<தசம>* src, Complex<தசம>* dst,
                             மாறிலி வெறுமை* spec, uchar* buf)
{
    திரும்பு CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (மாறிலி Ipp32fc*)src, (Ipp32fc*)dst,
                                 (மாறிலி IppsDFTSpec_C_32fc*)spec, buf);
}

நிலையான IppStatus ippsDFTInv_CToC( மாறிலி Complex<இரட்டை>* src, Complex<இரட்டை>* dst,
                                  மாறிலி வெறுமை* spec, uchar* buf)
{
    திரும்பு CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (மாறிலி Ipp64fc*)src, (Ipp64fc*)dst,
                                 (மாறிலி IppsDFTSpec_C_64fc*)spec, buf);
}

நிலையான IppStatus ippsDFTFwd_RToPack( மாறிலி தசம* src, தசம* dst,
                                     மாறிலி வெறுமை* spec, uchar* buf)
{
    திரும்பு CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (மாறிலி IppsDFTSpec_R_32f*)spec, buf);
}

நிலையான IppStatus ippsDFTFwd_RToPack( மாறிலி இரட்டை* src, இரட்டை* dst,
                                     மாறிலி வெறுமை* spec, uchar* buf)
{
    திரும்பு CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (மாறிலி IppsDFTSpec_R_64f*)spec, buf);
}

நிலையான IppStatus ippsDFTInv_PackToR( மாறிலி தசம* src, தசம* dst,
                                     மாறிலி வெறுமை* spec, uchar* buf)
{
    திரும்பு CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_32f, src, dst, (மாறிலி IppsDFTSpec_R_32f*)spec, buf);
}

நிலையான IppStatus ippsDFTInv_PackToR( மாறிலி இரட்டை* src, இரட்டை* dst,
                                     மாறிலி வெறுமை* spec, uchar* buf)
{
    திரும்பு CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_64f, src, dst, (மாறிலி IppsDFTSpec_R_64f*)spec, buf);
}
#endif

கட்டமைப்பு OcvDftOptions;

வகைவரை வெறுமை (*DFTFunc)(மாறிலி OcvDftOptions & c, மாறிலி வெறுமை* src, வெறுமை* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
கட்டமைப்பு OcvDftOptions {
    இரட்டை scale;
    முழு n;

    தருக்க isInverse;
    தருக்க isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    தருக்க useIpp;

#ifdef USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#endif

    மாறிலி DftPlan* plan;
    மாறிலி DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = பொய்;
        isComplex = பொய்;
        useIpp = பொய்;
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

நிலையான DFTKernels getDFTKernelsDispatch(முழு depth)
{
    CV_CPU_DISPATCH(getDFTKernels_, (depth), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
நிலையான மாறிலி DFTKernels& getDFTKernels(முழு depth)
{
    நிலையான மாறிலி DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    நிலையான மாறிலி DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    திரும்பு depth == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
கட்டமைப்பு DftTwiddleGen
{
    முழு M, logB, maskB;
    AutoBuffer<இரட்டை, 1024> buf;
    இரட்டை *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    நிலையான வெறுமை fillTable(இரட்டை* re, இரட்டை* im, முழு மொத்தம்_2, இரட்டை wr, இரட்டை wi, இரட்டை stepAngle)
    {
        க்கு (முழு i0 = 0; i0 < மொத்தம்_2; i0 += 16)
        {
            இரட்டை t0 = stepAngle*i0;
            இரட்டை cr = i0 == 0 ? 1 : cos(t0), ci = i0 == 0 ? 0 : sin(t0);
            முழு i1 = std::min(i0 + 16, மொத்தம்_2);
            க்கு (முழு i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                இரட்டை t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    வெறுமை init(முழு _M)
    {
        M = _M;
        logB = 0;
        போது ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        முழு B = 1 << logB;
        maskB = B - 1;
        முழு nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.data(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        இரட்டை a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fillTable(lo_re, lo_im, B, cos(a), sin(a), a);
        fillTable(hi_re, hi_im, nhi, cos(a*B), sin(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    வெறுமை get(முழு m, இரட்டை& re, இரட்டை& im) மாறிலி
    {
        CV_DbgAssert(m >= 0);
        எனில் (m >= M)
            m %= M;
        முழு a = m >> logB, b = m & maskB;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

template<typename T>
வெறுமை DftPlan::fillTables(size_t ntw, தருக்க need_rtw, தருக்க need_dct)
{
    DftPlan& p = *இது;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    முழு M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*அளவு(T) + அளவு(இரட்டை) - 1)/அளவு(இரட்டை) + 1);
    T* cur = (T*)p.tw.data();
    முழு maxL = 1;
    க்கு (முழு s = 0; s < p.nstages; s++)
        எனில் (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].span);
    AutoBuffer<இரட்டை, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    இரட்டை* twd = twd_buf.data();
    க்கு (முழு s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        முழு r = st.radix, L = st.span, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        முழு step = M/(r*L);
        எனில் (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr முழு K = 8;
            இரட்டை cr[K], ci[K], wr, wi;
            gen.get(K*step, wr, wi);
            இரட்டை* t1r = twd; இரட்டை* t1i = twd + L;      // leg 1 kept in double for the products
            க்கு (முழு j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                க்கு (முழு k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*step, cr[k], ci[k]);
                முழு j1 = std::min(j0 + 16*K, L);
                க்கு (முழு j = j0; j < j1; j += K)
                    க்கு (முழு k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        இரட்டை t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            க்கு (முழு j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            க்கு (முழு q = 2; q < r; q++)
            {
                மாறிலி T* pr = twr + (q-2)*ts; மாறிலி T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                க்கு (முழு j = 0; j < L; j++)
                {
                    இரட்டை ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        இல்லையெனில்
        {
            // large odd radix: exact-ish generator lookup per entry
            க்கு (முழு q = 1; q < r; q++)
                க்கு (முழு j = 0; j < L; j++)
                {
                    இரட்டை re, im;
                    gen.get(q*j*step, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        எனில் (r >= 7)
        {
            முழு h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            க்கு (முழு pp = 1; pp <= h; pp++)
                க்கு (முழு k = 1; k <= h; k++)
                {
                    இரட்டை t = 2*CV_PI*((இரட்டை)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)cos(t);
                    sn[(pp-1)*h + (k-1)] = (T)sin(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    எனில் (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        முழு step = M/n;
        க்கு (முழு k = 0; k <= nc; k++)
        {
            இரட்டை re, im;
            gen.get(k*step, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    எனில் (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        இரட்டை scale = (p.kind == DFT_KIND_DCT ? 2 : 1)*std::sqrt(1./(2*n));
        முழு step = M/(4*n);
        க்கு (முழு k = 0; k <= nc; k++)
        {
            இரட்டை re, im;
            gen.get(k*step, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
வெறுமை DftPlan::build(முழு _kind, முழு _n, முழு _depth, முழு _vl)
{
    DftPlan& p = *இது;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(_depth == CV_32F || _depth == CV_64F);
    தருக்க real_kind = _kind != DFT_KIND_C2C;
    தருக்க dct_kind = _kind == DFT_KIND_DCT || _kind == DFT_KIND_IDCT;
    எனில் (dct_kind)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.kind = _kind; p.n = _n; p.depth = _depth; p.vl = _vl;
    p.nc = real_kind && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = depth == CV_32F ? அளவு(தசம) : அளவு(இரட்டை);
    p.real_உள்ளீடு = kind == DFT_KIND_R2C && (n & 1) != 0;
    p.need_tmp = kind == DFT_KIND_C2R || kind == DFT_KIND_DCT || kind == DFT_KIND_IDCT;
    p.pingpong = பொய்;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    முழு k2 = 0, rest = nc;
    போது ((rest & 1) == 0) { rest >>= 1; k2++; }
    முழு r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.first_radix = r0;
    முழு radices[DFT_MAX_STAGES], nst = 0;
    க்கு (முழு m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    போது (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    போது (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    க்கு (முழு f = 7; rest > 1; f += 2)
    {
        எனில் (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            நிறுத்து;
        }
        போது (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    முழு L = r0, max_h = 0;
    size_t ntw = 0;
    க்கு (முழு s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        முழு r = radices[s];
        st.radix = r; st.span = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        எனில் (vl > 1 && L >= vl)
        {
            st.mode = DFT_STAGE_FULL;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            எனில் (L % vl != 0)
                p.pingpong = உண்மை;   // back-off clamp => overlapping stores => must be out-of-place
        }
        இல்லையெனில்
        {
            st.mode = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        எனில் (r >= 7)
        {
            முழு h = (r - 1)/2;
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
    முழு seq[DFT_MAX_STAGES + 1], nseq = 0;
    எனில் (r0 > 1) seq[nseq++] = r0;
    க்கு (முழு s = 0; s < nst; s++) seq[nseq++] = radices[s];
    முழு digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        முழு Ls = 1;
        க்கு (முழு s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    எனில் (r0 == 1)
    {
        p.itab.allocate(nc*2);
        முழு* it_all = p.itab.data();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        முழு n_in = std::min(nseq, 2);
        முழு r_0 = n_in > 0 ? seq[0] : 1, w_0 = n_in > 0 ? weights[0] : 0;
        முழு r_1 = n_in > 1 ? seq[1] : 1, w_1 = n_in > 1 ? weights[1] : 0;
        க்கு (முழு pos = 0, rev_hi = 0; pos < nc; )
        {
            க்கு (முழு d1 = 0; d1 < r_1; d1++)
            {
                முழு rev = rev_hi + d1*w_1;
                முழு* it = it_all + pos*2;
                எனில் (p.real_உள்ளீடு)
                {
                    க்கு (முழு d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                இல்லையெனில்
                {
                    க்கு (முழு d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            க்கு (முழு s = n_in; s < nseq; s++)
            {
                rev_hi += weights[s];
                எனில் (++digits[s] < seq[s])
                    நிறுத்து;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    இல்லையெனில்
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        முழு M = nc/r0;
        p.itab.allocate(M);
        முழு* gtab = p.itab.data();
        முழு gw[DFT_MAX_STAGES + 1];
        {
            முழு Ls = 1;
            க்கு (முழு s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        முழு sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        முழு r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        முழு r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        க்கு (முழு base = 0, g_hi = 0; base < M; )
        {
            க்கு (முழு db = 0; db < r_b; db++)
            {
                முழு g = (g_hi + db*g_b)*r0;
                க்கு (முழு da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[base++] = g;
            }
            க்கு (முழு s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                எனில் (++digits[s] < seq[s])
                    நிறுத்து;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    தருக்க need_rtw = real_kind && n % 2 == 0;
    எனில் (need_rtw) ntw += 2*(size_t)(nc + 1);
    எனில் (dct_kind) ntw += 2*(size_t)(nc + 1);
    எனில் (depth == CV_32F)
        fillTables<தசம>(ntw, need_rtw, dct_kind);
    இல்லையெனில்
        fillTables<இரட்டை>(ntw, need_rtw, dct_kind);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_bytes = 2*(size_t)nc*esz;
    தருக்க two_pairs = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_pairs ? pair_bytes : 0;
    p.scratch_ofs = pair_bytes*(two_pairs ? 2 : 1);
    size_t scratch_bytes = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_bytes = p.scratch_ofs + scratch_bytes + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
நிலையான வெறுமை runDft(மாறிலி OcvDftOptions& c, மாறிலி வெறுமை* src, size_t sstep, வெறுமை* dst, size_t dstep,
                   தருக்க complex_io)
{
    மாறிலி DftPlan& p = *c.plan;
    மாறிலி DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.depth == CV_32F ? அளவு(தசம) : அளவு(இரட்டை);
    uchar* base = alignPtr(c.workspace, 64);
    uchar* re0 = base;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = base + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = base + p.scratch_ofs;

    மாறிலி வெறுமை* asrc = src;
    size_t astep = sstep;
    தருக்க conj = பொய்;
    எனில் (p.kind == DFT_KIND_C2R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = உண்மை;
    }
    இல்லையெனில் எனில் (p.kind == DFT_KIND_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = உண்மை;
    }
    இல்லையெனில் எனில் (p.kind == DFT_KIND_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    இல்லையெனில் எனில் (p.kind == DFT_KIND_C2C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.first_radix == 1);

    DftPreprocFunc pre = p.first_radix == 1 ? k.preprocRadix0 : p.first_radix == 2 ? k.preprocRadix2 :
                         p.first_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    க்கு (முழு i = 0; i < p.nstages; i++)
    {
        மாறிலி DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        எனில் (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        இல்லையெனில்
            fn(st, sre, sim, sre, sim, scratch);
    }

    இரட்டை scale = c.scale;
    தேர்வு (p.kind)
    {
    நிலை DFT_KIND_C2C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        நிறுத்து;
    நிலை DFT_KIND_R2C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        நிறுத்து;
    நிலை DFT_KIND_C2R:
        எனில் (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        இல்லையெனில்
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        நிறுத்து;
    நிலை DFT_KIND_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        நிறுத்து;
    இயல்பு:
        CV_Assert(p.kind == DFT_KIND_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
template<typename T> நிலையான வெறுமை
DFT(மாறிலி OcvDftOptions & c, மாறிலி Complex<T>* src, Complex<T>* dst)
{
    எனில்( c.useIpp )
    {
#ifdef USE_IPP_DFT
        எனில்( !c.isInverse )
        {
            எனில் (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                திரும்பு;
            }
        }
        இல்லையெனில்
        {
            எனில் (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                திரும்பு;
            }
        }
        setIppErrorStatus();
#endif
    }

    runDft(c, src, 1, dst, 1, பொய்);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
template<typename T> நிலையான வெறுமை
RealDFT(மாறிலி OcvDftOptions & c, மாறிலி T* src, T* dst)
{
    முழு complex_output = c.isComplex;

    எனில்( c.useIpp )
    {
#ifdef USE_IPP_DFT
        முழு n = c.n;
        T* ipp_dst = dst + complex_output;
        எனில் (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            எனில்( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                எனில்( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_ADD(CV_IMPL_IPP);
            திரும்பு;
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
template<typename T> நிலையான வெறுமை
CCSIDFT(மாறிலி OcvDftOptions & c, மாறிலி T* src, T* dst)
{
    முழு complex_உள்ளீடு = c.isComplex;

    எனில்( complex_உள்ளீடு )
        CV_Assert( src != dst );
    எனில்( c.useIpp )
    {
#ifdef USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        மாறிலி T* ipp_src = src;
        T save_s1 = 0;
        எனில்( complex_உள்ளீடு )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        IppStatus status = ippsDFTInv_PackToR( ipp_src, dst, c.ipp_spec, c.ipp_work );
        எனில்( complex_உள்ளீடு )
            ((T*)src)[1] = save_s1;
        எனில்( status >= 0 )
        {
            CV_IMPL_ADD(CV_IMPL_IPP);
            திரும்பு;
        }
        setIppErrorStatus();
#endif
    }
    runDft(c, src, 1, dst, 1, complex_உள்ளீடு != 0);
}

நிலையான வெறுமை
CopyColumn( மாறிலி uchar* _src, size_t src_step,
            uchar* _dst, size_t dst_step,
            முழு len, size_t elem_size )
{
    முழு i, t0, t1;
    மாறிலி முழு* src = (மாறிலி முழு*)_src;
    முழு* dst = (முழு*)_dst;
    src_step /= அளவு(src[0]);
    dst_step /= அளவு(dst[0]);

    எனில்( elem_size == அளவு(முழு) )
    {
        க்கு( i = 0; i < len; i++, src += src_step, dst += dst_step )
            dst[0] = src[0];
    }
    இல்லையெனில் எனில்( elem_size == அளவு(முழு)*2 )
    {
        க்கு( i = 0; i < len; i++, src += src_step, dst += dst_step )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    இல்லையெனில் எனில்( elem_size == அளவு(முழு)*4 )
    {
        க்கு( i = 0; i < len; i++, src += src_step, dst += dst_step )
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
template<typename T> நிலையான உள்வரி வெறுமை
copyFromColumns_( மாறிலி uchar* src, size_t src_step, T* dst, முழு ncols, முழு len )
{
    க்கு( முழு i = 0; i < len; i++, src += src_step )
    {
        மாறிலி T* s = (மாறிலி T*)src;
        க்கு( முழு k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

நிலையான வெறுமை
CopyFromColumns( மாறிலி uchar* src, size_t src_step, uchar* dst, முழு ncols, முழு len, size_t elem_size )
{
    எனில்( elem_size == அளவு(முழு) )
        copyFromColumns_(src, src_step, (முழு*)dst, ncols, len);
    இல்லையெனில் எனில்( elem_size == அளவு(int64) )
        copyFromColumns_(src, src_step, (int64*)dst, ncols, len);
    இல்லையெனில்
    {
        CV_Assert( elem_size == அளவு(int64)*2 );
        copyFromColumns_(src, src_step, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
template<typename T> நிலையான உள்வரி வெறுமை
copyToColumns_( மாறிலி T* src, uchar* dst, size_t dst_step, முழு ncols, முழு len )
{
    க்கு( முழு i = 0; i < len; i++, dst += dst_step )
    {
        T* d = (T*)dst;
        க்கு( முழு k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

நிலையான வெறுமை
CopyToColumns( மாறிலி uchar* src, uchar* dst, size_t dst_step, முழு ncols, முழு len, size_t elem_size )
{
    எனில்( elem_size == அளவு(முழு) )
        copyToColumns_((மாறிலி முழு*)src, dst, dst_step, ncols, len);
    இல்லையெனில் எனில்( elem_size == அளவு(int64) )
        copyToColumns_((மாறிலி int64*)src, dst, dst_step, ncols, len);
    இல்லையெனில்
    {
        CV_Assert( elem_size == அளவு(int64)*2 );
        copyToColumns_((மாறிலி Vec<int64, 2>*)src, dst, dst_step, ncols, len);
    }
}

நிலையான வெறுமை
ExpandCCS( uchar* _ptr, முழு n, முழு elem_size )
{
    முழு i;
    எனில்( elem_size == (முழு)அளவு(தசம) )
    {
        தசம* p = (தசம*)_ptr;
        க்கு( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        எனில்( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        க்கு( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    இல்லையெனில்
    {
        இரட்டை* p = (இரட்டை*)_ptr;
        க்கு( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        எனில்( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        க்கு( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

template<typename T, வெறுமை (*fn)(மாறிலி OcvDftOptions&, மாறிலி T*, T*)>
நிலையான வெறுமை dftWrap(மாறிலி OcvDftOptions & c, மாறிலி வெறுமை* src, வெறுமை* dst)
{
    fn(c, (மாறிலி T*)src, (T*)dst);
}

}

#ifdef USE_IPP_DFT
வகைவரை IppStatus (CV_STDCALL* IppDFTGetSizeFunc)(முழு, முழு, IppHintAlgorithm, முழு*, முழு*, முழு*);
வகைவரை IppStatus (CV_STDCALL* IppDFTInitFunc)(முழு, முழு, IppHintAlgorithm, வெறுமை*, uchar*);

template<typename SpecType, IppStatus (CV_STDCALL *init_fn)(முழு, முழு, IppHintAlgorithm, SpecType*, Ipp8u*)>
நிலையான IppStatus CV_STDCALL ippDFTInitWrap(முழு n, முழு flags, IppHintAlgorithm hint, வெறுமை* spec, Ipp8u* initbuf)
{
    திரும்பு init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#endif

நாமாகாஶ cv
{
#எனில் defined USE_IPP_DFT

வகைவரை IppStatus (CV_STDCALL* ippiDFT_C_Func)(மாறிலி Ipp32fc*, முழு, Ipp32fc*, முழு, மாறிலி IppiDFTSpec_C_32fc*, Ipp8u*);
வகைவரை IppStatus (CV_STDCALL* ippiDFT_R_Func)(மாறிலி Ipp32f* , முழு, Ipp32f* , முழு, மாறிலி IppiDFTSpec_R_32f* , Ipp8u*);

template <typename Dft>
வகுப்பு Dft_C_IPPLoop_Invoker : பொது ParallelLoopBody
{
பொது:

    Dft_C_IPPLoop_Invoker(மாறிலி uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, முழு _width,
                          மாறிலி Dft& _ippidft, முழு _norm_flag, தருக்க *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = உண்மை;
    }

    மெய்நிகர் வெறுமை operator()(மாறிலி Range& range) மாறிலி CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* pBuffer = 0;
        Ipp8u* pMemInit= 0;
        முழு sizeBuffer=0;
        முழு sizeSpec=0;
        முழு sizeInit=0;

        IppiSize srcRoiSize = {width, 1};

        status = ippiDFTGetSize_C_32fc(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
        எனில் ( status < 0 )
        {
            *ok = பொய்;
            திரும்பு;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( sizeSpec );

        எனில் ( sizeInit > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

        எனில் ( sizeBuffer > 0 )
            pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

        status = ippiDFTInit_C_32fc( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        எனில் ( sizeInit > 0 )
            ippFree( pMemInit );

        எனில் ( status < 0 )
        {
            ippFree( pDFTSpec );
            எனில் ( sizeBuffer > 0 )
                ippFree( pBuffer );
            *ok = பொய்;
            திரும்பு;
        }

        க்கு( முழு i = range.start; i < range.end; ++i)
            எனில்(!ippidft((Ipp32fc*)(src + src_step * i), src_step, (Ipp32fc*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)pBuffer))
            {
                *ok = பொய்;
            }

        எனில் ( sizeBuffer > 0 )
            ippFree( pBuffer );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

தனிப்பட்ட:
    மாறிலி uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    முழு width;
    மாறிலி Dft& ippidft;
    முழு norm_flag;
    தருக்க *ok;

    மாறிலி Dft_C_IPPLoop_Invoker& operator= (மாறிலி Dft_C_IPPLoop_Invoker&);
};

template <typename Dft>
வகுப்பு Dft_R_IPPLoop_Invoker : பொது ParallelLoopBody
{
பொது:

    Dft_R_IPPLoop_Invoker(மாறிலி uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, முழு _width,
                          மாறிலி Dft& _ippidft, முழு _norm_flag, தருக்க *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = உண்மை;
    }

    மெய்நிகர் வெறுமை operator()(மாறிலி Range& range) மாறிலி CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* pBuffer = 0;
        Ipp8u* pMemInit= 0;
        முழு sizeBuffer=0;
        முழு sizeSpec=0;
        முழு sizeInit=0;

        IppiSize srcRoiSize = {width, 1};

        status = ippiDFTGetSize_R_32f(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
        எனில் ( status < 0 )
        {
            *ok = பொய்;
            திரும்பு;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( sizeSpec );

        எனில் ( sizeInit > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

        எனில் ( sizeBuffer > 0 )
            pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

        status = ippiDFTInit_R_32f( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        எனில் ( sizeInit > 0 )
            ippFree( pMemInit );

        எனில் ( status < 0 )
        {
            ippFree( pDFTSpec );
            எனில் ( sizeBuffer > 0 )
                ippFree( pBuffer );
            *ok = பொய்;
            திரும்பு;
        }

        க்கு( முழு i = range.start; i < range.end; ++i)
            எனில்(!ippidft((தசம*)(src + src_step * i), src_step, (தசம*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)pBuffer))
            {
                *ok = பொய்;
            }

        எனில் ( sizeBuffer > 0 )
            ippFree( pBuffer );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

தனிப்பட்ட:
    மாறிலி uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    முழு width;
    மாறிலி Dft& ippidft;
    முழு norm_flag;
    தருக்க *ok;

    மாறிலி Dft_R_IPPLoop_Invoker& operator= (மாறிலி Dft_R_IPPLoop_Invoker&);
};

template <typename Dft>
தருக்க Dft_C_IPPLoop(மாறிலி uchar * src, size_t src_step, uchar * dst, size_t dst_step, முழு width, முழு height, மாறிலி Dft& ippidft, முழு norm_flag)
{
    தருக்க ok;
    parallel_for_(Range(0, height), Dft_C_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(இரட்டை)(1<<16) );
    திரும்பு ok;
}

template <typename Dft>
தருக்க Dft_R_IPPLoop(மாறிலி uchar * src, size_t src_step, uchar * dst, size_t dst_step, முழு width, முழு height, மாறிலி Dft& ippidft, முழு norm_flag)
{
    தருக்க ok;
    parallel_for_(Range(0, height), Dft_R_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(இரட்டை)(1<<16) );
    திரும்பு ok;
}

கட்டமைப்பு IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    தருக்க operator()(மாறிலி Ipp32fc* src, size_t srcStep, Ipp32fc* dst, size_t dstStep, மாறிலி IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* pBuffer) மாறிலி
    {
        திரும்பு ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<முழு>(srcStep), dst, static_cast<முழு>(dstStep), pDFTSpec, pBuffer) >= 0 : பொய்;
    }
தனிப்பட்ட:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

கட்டமைப்பு IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippiDFT_PackToR_32f_C1R(_func){}

    தருக்க operator()(மாறிலி Ipp32f* src, size_t srcStep, Ipp32f* dst, size_t dstStep, மாறிலி IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* pBuffer) மாறிலி
    {
        திரும்பு ippiDFT_PackToR_32f_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_PackToR_32f_C1R, src, static_cast<முழு>(srcStep), dst, static_cast<முழு>(dstStep), pDFTSpec, pBuffer) >= 0 : பொய்;
    }
தனிப்பட்ட:
    ippiDFT_R_Func ippiDFT_PackToR_32f_C1R;
};

நிலையான தருக்க ippi_DFT_C_32F(மாறிலி uchar * src, size_t src_step, uchar * dst, size_t dst_step, முழு width, முழு height, தருக்க inv, முழு norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* pBuffer = 0;
    Ipp8u* pMemInit= 0;
    முழு sizeBuffer=0;
    முழு sizeSpec=0;
    முழு sizeInit=0;

    IppiSize srcRoiSize = {width, height};

    status = ippiDFTGetSize_C_32fc(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
    எனில் ( status < 0 )
        திரும்பு பொய்;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( sizeSpec );

    எனில் ( sizeInit > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

    எனில் ( sizeBuffer > 0 )
        pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

    status = ippiDFTInit_C_32fc( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    எனில் ( sizeInit > 0 )
        ippFree( pMemInit );

    எனில் ( status < 0 )
    {
        ippFree( pDFTSpec );
        எனில் ( sizeBuffer > 0 )
            ippFree( pBuffer );
        திரும்பு பொய்;
    }

    எனில் (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<முழு>(src_step), (Ipp32fc*)dst, static_cast<முழு>(dst_step), pDFTSpec, pBuffer);
    இல்லையெனில்
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<முழு>(src_step), (Ipp32fc*)dst, static_cast<முழு>(dst_step), pDFTSpec, pBuffer);

    எனில் ( sizeBuffer > 0 )
        ippFree( pBuffer );

    ippFree( pDFTSpec );

    எனில்(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        திரும்பு உண்மை;
    }
    திரும்பு பொய்;
}

நிலையான தருக்க ippi_DFT_R_32F(மாறிலி uchar * src, size_t src_step, uchar * dst, size_t dst_step, முழு width, முழு height, தருக்க inv, முழு norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* pBuffer = 0;
    Ipp8u* pMemInit= 0;
    முழு sizeBuffer=0;
    முழு sizeSpec=0;
    முழு sizeInit=0;

    IppiSize srcRoiSize = {width, height};

    status = ippiDFTGetSize_R_32f(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
    எனில் ( status < 0 )
        திரும்பு பொய்;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( sizeSpec );

    எனில் ( sizeInit > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

    எனில் ( sizeBuffer > 0 )
        pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

    status = ippiDFTInit_R_32f( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    எனில் ( sizeInit > 0 )
        ippFree( pMemInit );

    எனில் ( status < 0 )
    {
        ippFree( pDFTSpec );
        எனில் ( sizeBuffer > 0 )
            ippFree( pBuffer );
        திரும்பு பொய்;
    }

    எனில் (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (தசம*)src, static_cast<முழு>(src_step), (தசம*)dst, static_cast<முழு>(dst_step), pDFTSpec, pBuffer);
    இல்லையெனில்
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_PackToR_32f_C1R, (தசம*)src, static_cast<முழு>(src_step), (தசம*)dst, static_cast<முழு>(dst_step), pDFTSpec, pBuffer);

    எனில் ( sizeBuffer > 0 )
        ippFree( pBuffer );

    ippFree( pDFTSpec );

    எனில்(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        திரும்பு உண்மை;
    }
    திரும்பு பொய்;
}

#endif
}

#ifdef HAVE_OPENCL

நாமாகாஶ cv
{

எண்ணிக்கை FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

கட்டமைப்பு OCL_FftPlan
{
தனிப்பட்ட:
    UMat twiddles;
    String buildOptions;
    முழு thread_மொத்தம்;
    முழு dft_size;
    முழு dft_depth;
    தருக்க status;

பொது:
    OCL_FftPlan(முழு _size, முழு _depth) : dft_size(_size), dft_depth(_depth), status(உண்மை)
    {
        CV_Assert( dft_depth == CV_32F || dft_depth == CV_64F );

        முழு min_radix;
        std::vector<முழு> radixes, blocks;
        ocl_getRadixes(dft_size, radixes, blocks, min_radix);
        thread_மொத்தம் = dft_size / min_radix;

        எனில் (thread_மொத்தம் > (முழு) ocl::Device::getDefault().maxWorkGroupSize())
        {
            status = பொய்;
            திரும்பு;
        }

        // generate string with radix calls
        String radix_processing;
        முழு n = 1, twiddle_size = 0;
        க்கு (size_t i=0; i<radixes.size(); i++)
        {
            முழு radix = radixes[i], block = blocks[i];
            எனில் (block > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, block, twiddle_size, n, dft_size/radix);
            இல்லையெனில்
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_size, n, dft_size/radix);
            twiddle_size += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_size, CV_MAKE_TYPE(dft_depth, 2));
        எனில் (dft_depth == CV_32F)
            fillRadixTable<தசம>(twiddles, radixes);
        இல்லையெனில்
            fillRadixTable<இரட்டை>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_size, min_radix, ocl::typeToStr(dft_depth), ocl::typeToStr(CV_MAKE_TYPE(dft_depth, 2)),
                              dft_depth == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    தருக்க enqueueTransform(உள்ளீடு_Array _src, OutputArray _dst, முழு இலக்கம்_dfts, முழு flags, முழு fftType, தருக்க rows = உண்மை) மாறிலி
    {
        எனில் (!status)
            திரும்பு பொய்;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        தருக்க is1d = (flags & DFT_ROWS) != 0 || இலக்கம்_dfts == 1;
        தருக்க inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        எனில் (rows)
        {
            globalsize[0] = thread_மொத்தம்; globalsize[1] = src.rows;
            localsize[0] = thread_மொத்தம்; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            எனில் ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        இல்லையெனில்
        {
            globalsize[0] = இலக்கம்_dfts; globalsize[1] = thread_மொத்தம்;
            localsize[0] = 1; localsize[1] = thread_மொத்தம்;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            எனில் (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        எனில் (!inv)
        {
            எனில் ((is1d && src.channels() == 1) || (rows && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        இல்லையெனில்
        {
            எனில் (rows && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            எனில் (dst.cols % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        எனில் (k.empty())
            திரும்பு பொய்;

        k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::ReadOnlyNoSize(twiddles), thread_மொத்தம், இலக்கம்_dfts);
        திரும்பு k.run(2, globalsize, localsize, பொய்);
    }

தனிப்பட்ட:
    நிலையான வெறுமை ocl_getRadixes(முழு cols, std::vector<முழு>& radixes, std::vector<முழு>& blocks, முழு& min_radix)
    {
        முழு factors[34];
        முழு nf = DFTFactorize(cols, factors);

        முழு n = 1;
        முழு factor_index = 0;
        min_radix = INT_MAX;

        // 2^n transforms
        எனில் ((factors[factor_index] & 1) == 0)
        {
            க்கு( ; n < factors[factor_index];)
            {
                முழு radix = 2, block = 1;
                எனில் (8*n <= factors[0])
                    radix = 8;
                இல்லையெனில் எனில் (4*n <= factors[0])
                {
                    radix = 4;
                    எனில் (cols % 12 == 0)
                        block = 3;
                    இல்லையெனில் எனில் (cols % 8 == 0)
                        block = 2;
                }
                இல்லையெனில்
                {
                    எனில் (cols % 10 == 0)
                        block = 5;
                    இல்லையெனில் எனில் (cols % 8 == 0)
                        block = 4;
                    இல்லையெனில் எனில் (cols % 6 == 0)
                        block = 3;
                    இல்லையெனில் எனில் (cols % 4 == 0)
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
        க்கு( ; factor_index < nf; factor_index++)
        {
            முழு radix = factors[factor_index], block = 1;
            எனில் (radix == 3)
            {
                எனில் (cols % 12 == 0)
                    block = 4;
                இல்லையெனில் எனில் (cols % 9 == 0)
                    block = 3;
                இல்லையெனில் எனில் (cols % 6 == 0)
                    block = 2;
            }
            இல்லையெனில் எனில் (radix == 5)
            {
                எனில் (cols % 10 == 0)
                    block = 2;
            }
            radixes.push_back(radix);
            blocks.push_back(block);
            min_radix = min(min_radix, block*radix);
        }
    }

    template <typename T>
    நிலையான வெறுமை fillRadixTable(UMat twiddles, மாறிலி std::vector<முழு>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        முழு ptr_index = 0;

        முழு n = 1;
        க்கு (size_t i=0; i<radixes.size(); i++)
        {
            முழு radix = radixes[i];
            n *= radix;

            க்கு (முழு j=1; j<radix; j++)
            {
                இரட்டை theta = -CV_2PI*j/n;

                க்கு (முழு k=0; k<(n/radix); k++)
                {
                    ptr[ptr_index++] = (T) cos(k*theta);
                    ptr[ptr_index++] = (T) sin(k*theta);
                }
            }
        }
    }
};

வகுப்பு OCL_FftPlanCache
{
பொது:
    நிலையான OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, புதிய OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(முழு dft_size, முழு depth)
    {
        முழு சாவி = (dft_size << 16) | (depth & 0xFFFF);
        std::map<முழு, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(சாவி);
        எனில் (f != planStorage.end())
        {
            திரும்பு f->second;
        }
        இல்லையெனில்
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(புதிய OCL_FftPlan(dft_size, depth));
            planStorage[சாவி] = newPlan;
            திரும்பு newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

பாதுகாக்கப்பட்ட:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<முழு, Ptr<OCL_FftPlan> > planStorage;
};

நிலையான தருக்க ocl_dft_rows(உள்ளீடு_Array _src, OutputArray _dst, முழு nonzero_rows, முழு flags, முழு fftType)
{
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.cols(), depth);
    திரும்பு plan->enqueueTransform(_src, _dst, nonzero_rows, flags, fftType, உண்மை);
}

நிலையான தருக்க ocl_dft_cols(உள்ளீடு_Array _src, OutputArray _dst, முழு nonzero_cols, முழு flags, முழு fftType)
{
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.rows(), depth);
    திரும்பு plan->enqueueTransform(_src, _dst, nonzero_cols, flags, fftType, பொய்);
}

உள்வரி FftType determineFFTType(தருக்க real_உள்ளீடு, தருக்க complex_உள்ளீடு, தருக்க real_output, தருக்க complex_output, தருக்க inv)
{
    // output format is not specified
    எனில் (!real_output && !complex_output)
        complex_output = உண்மை;

    // input or output format is ambiguous
    எனில் (real_உள்ளீடு == complex_உள்ளீடு || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType result = real_உள்ளீடு ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    எனில் (result == C2R && !inv)
        result = C2C;

    // Inverse CCS to Complex not supported
    எனில் (result == R2C && inv)
        result = R2R;

    திரும்பு result;
}

நிலையான தருக்க ocl_dft(உள்ளீடு_Array _src, OutputArray _dst, முழு flags, முழு nonzero_rows)
{
    முழு type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    Size ssize = _src.size();
    தருக்க doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    எனில் (!(cn == 1 || cn == 2)
        || !(depth == CV_32F || (depth == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        திரும்பு பொய்;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    எனில் (ssize.area() != getOptimalDFTSize(ssize.area()))
        திரும்பு பொய்;

    UMat src = _src.getUMat();
    தருக்க inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    எனில்( nonzero_rows <= 0 || nonzero_rows > _src.rows() )
        nonzero_rows = _src.rows();
    தருக்க is1d = (flags & DFT_ROWS) != 0 || nonzero_rows == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    எனில் (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.createSameSize(src, CV_MAKETYPE(depth, 2));
        output = _dst.getUMat();
    }
    இல்லையெனில்
    {
        // real output
        எனில் (is1d)
        {
            _dst.createSameSize(src, CV_MAKETYPE(depth, 1));
            output = _dst.getUMat();
        }
        இல்லையெனில்
        {
            _dst.createSameSize(src, CV_MAKETYPE(depth, 1));
            output.create(src.size, CV_MAKETYPE(depth, 2));
        }
    }

    தருக்க result = பொய்;
    எனில் (!inv)
    {
        முழு nonzero_cols = fftType == R2R ? output.cols/2 + 1 : output.cols;
        result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
        எனில் (!is1d)
            result = result && ocl_dft_cols(output, _dst, nonzero_cols, flags, fftType);
    }
    இல்லையெனில்
    {
        எனில் (fftType == C2C)
        {
            // complex output
            result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            எனில் (!is1d)
                result = result && ocl_dft_cols(output, output, output.cols, flags, fftType);
        }
        இல்லையெனில்
        {
            எனில் (is1d)
            {
                result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            }
            இல்லையெனில்
            {
                முழு nonzero_cols = src.cols/2 + 1;
                result = ocl_dft_cols(src, output, nonzero_cols, flags, fftType);
                result = result && ocl_dft_rows(output, _dst, nonzero_rows, flags, fftType);
            }
        }
    }
    திரும்பு result;
}

} // namespace cv;

#endif

#ifdef HAVE_CLAMDFFT

நாமாகாஶ cv {

#define CLAMDDFT_Assert(func) \
    { \
        clfftStatus s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

வகுப்பு PlanCache
{
    கட்டமைப்பு FftPlan
    {
        FftPlan(மாறிலி Size & _dft_size, முழு _src_step, முழு _dst_step, தருக்க _doubleFP, தருக்க _inplace, முழு _flags, FftType _fftType) :
            dft_size(_dft_size), src_step(_src_step), dst_step(_dst_step),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            தருக்க dft_inverse = (flags & DFT_INVERSE) != 0;
            தருக்க dft_scale = (flags & DFT_SCALE) != 0;
            தருக்க dft_rows = (flags & DFT_ROWS) != 0;

            clfftLayout inLayout = CLFFT_REAL, outLayout = CLFFT_REAL;
            clfftDim dim = dft_size.height == 1 || dft_rows ? CLFFT_1D : CLFFT_2D;

            size_t batchSize = dft_rows ? dft_size.height : 1;
            size_t clLengthsIn[3] = { (size_t)dft_size.width, dft_rows ? 1 : (size_t)dft_size.height, 1 };
            size_t clStridesIn[3] = { 1, 1, 1 };
            size_t clStridesOut[3]  = { 1, 1, 1 };
            முழு elemSize = doubleFP ? அளவு(இரட்டை) : அளவு(தசம);

            தேர்வு (fftType)
            {
            நிலை C2C:
                inLayout = CLFFT_COMPLEX_INTERLEAVED;
                outLayout = CLFFT_COMPLEX_INTERLEAVED;
                clStridesIn[1] = src_step / (elemSize << 1);
                clStridesOut[1] = dst_step / (elemSize << 1);
                நிறுத்து;
            நிலை R2C:
                inLayout = CLFFT_REAL;
                outLayout = CLFFT_HERMITIAN_INTERLEAVED;
                clStridesIn[1] = src_step / elemSize;
                clStridesOut[1] = dst_step / (elemSize << 1);
                நிறுத்து;
            நிலை C2R:
                inLayout = CLFFT_HERMITIAN_INTERLEAVED;
                outLayout = CLFFT_REAL;
                clStridesIn[1] = src_step / (elemSize << 1);
                clStridesOut[1] = dst_step / elemSize;
                நிறுத்து;
            நிலை R2R:
            இயல்பு:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                நிறுத்து;
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

            தசம scale = dft_scale ? 1.0f / (dft_rows ? dft_size.width : dft_size.area()) : 1.0f;
            CLAMDDFT_Assert(clfftSetPlanScale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &queue, வெற்று, வெற்று))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        friend வகுப்பு PlanCache;

    தனிப்பட்ட:
        Size dft_size;
        முழு src_step, dst_step;
        தருக்க doubleFP;
        தருக்க inplace;
        முழு flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

பொது:
    நிலையான PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, புதிய PlanCache())
    }

    clfftPlanHandle getPlanHandle(மாறிலி Size & dft_size, முழு src_step, முழு dst_step, தருக்க doubleFP,
                                  தருக்க inplace, முழு flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        க்கு (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            மாறிலி FftPlan * மாறிலி plan = planStorage[i];

            எனில் (plan->dft_size == dft_size &&
                plan->flags == flags &&
                plan->src_step == src_step &&
                plan->dst_step == dst_step &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                எனில் (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    நிறுத்து;
                }

                திரும்பு plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(புதிய FftPlan(dft_size, src_step, dst_step, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        திரும்பு newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

பாதுகாக்கப்பட்ட:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

வெளி "C" {

நிலையான வெறுமை CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, வெறுமை *p)
{
    UMatData * u = (UMatData *)p;

    எனில்( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

நிலையான தருக்க ocl_dft_amdfft(உள்ளீடு_Array _src, OutputArray _dst, முழு flags)
{
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    Size ssize = _src.size();

    தருக்க doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    எனில் ( (!doubleSupport && depth == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        திரும்பு பொய்;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    எனில் (ssize.area() != getOptimalDFTSize(ssize.area()))
        திரும்பு பொய்;

    முழு dst_complex_உள்ளீடு = cn == 2 ? 1 : 0;
    தருக்க dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    முழு dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    தருக்க dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_உள்ளீடு << 0 | dft_complex_output << 1);

    தேர்வு (fftType)
    {
    நிலை C2C:
        _dst.create(ssize.height, ssize.width, CV_MAKE_TYPE(depth, 2));
        நிறுத்து;
    நிலை R2C: // TODO implement it if possible
    நிலை C2R: // TODO implement it if possible
    நிலை R2R: // AMD Fft does not support this type
    இயல்பு:
        திரும்பு பொய்;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    எனில் (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        திரும்பு பொய்;
    தருக்க inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (முழு)src.step, (முழு)dst.step,
                          depth == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t bufferSize = 0;
    CLAMDDFT_Assert(clfftGetTmpBufSize(plHandle, &bufferSize))
    UMat tmpBuffer(1, (முழு)bufferSize, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfftEnqueueTransform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &queue, 0, வெற்று, &e,
                                          &srcarg, &dstarg, (cl_mem)tmpBuffer.handle(ACCESS_RW)))

    tmpBuffer.addref();
    clSetEventCallback(e, CL_COMPLETE, oclCleanupCallback, tmpBuffer.u);
    திரும்பு உண்மை;
}

#undef DFT_ASSERT

}

#endif // HAVE_CLAMDFFT

நாமாகாஶ cv
{

template <typename T>
நிலையான வெறுமை complementComplex(T * ptr, size_t step, முழு n, முழு len, முழு dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = step/அளவு(p0[0]);
    க்கு(முழு i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        க்கு( முழு j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

நிலையான வெறுமை complementComplexOutput(முழு depth, uchar * ptr, size_t step, முழு மொத்தம்_2, முழு len, முழு dft_dims)
{
    எனில்( depth == CV_32F )
        complementComplex((தசம*)ptr, step, மொத்தம்_2, len, dft_dims);
    இல்லையெனில்
        complementComplex((இரட்டை*)ptr, step, மொத்தம்_2, len, dft_dims);
}

எண்ணிக்கை DftMode {
    InvalidDft = 0,
    FwdRealToCCS,
    FwdRealToComplex,
    FwdComplexToComplex,
    InvCCSToReal,
    InvComplexToReal,
    InvComplexToComplex,
};

எண்ணிக்கை DftDims {
    InvalidDim = 0,
    OneDim,
    OneDimColWise,
    TwoDims
};

உள்வரி மாறிலி எழுத்து * modeName(DftMode m)
{
    தேர்வு (m)
    {
    நிலை InvalidDft: திரும்பு "InvalidDft";
    நிலை FwdRealToCCS: திரும்பு "FwdRealToCCS";
    நிலை FwdRealToComplex: திரும்பு "FwdRealToComplex";
    நிலை FwdComplexToComplex: திரும்பு "FwdComplexToComplex";
    நிலை InvCCSToReal: திரும்பு "InvCCSToReal";
    நிலை InvComplexToReal: திரும்பு "InvComplexToReal";
    நிலை InvComplexToComplex: திரும்பு "InvComplexToComplex";
    }
    திரும்பு 0;
}

உள்வரி மாறிலி எழுத்து * dimsName(DftDims d)
{
    தேர்வு (d)
    {
    நிலை InvalidDim: திரும்பு "InvalidDim";
    நிலை OneDim: திரும்பு "OneDim";
    நிலை OneDimColWise: திரும்பு "OneDimColWise";
    நிலை TwoDims: திரும்பு "TwoDims";
    };
    திரும்பு 0;
}

template <typename T>
உள்வரி தருக்க isInv(T mode)
{
    தேர்வு ((DftMode)mode)
    {
        நிலை InvCCSToReal:
        நிலை InvComplexToReal:
        நிலை InvComplexToComplex: திரும்பு உண்மை;
        இயல்பு: திரும்பு பொய்;
    }
}

உள்வரி DftMode determineMode(தருக்க inv, முழு cn1, முழு cn2)
{
    எனில் (!inv)
    {
        எனில் (cn1 == 1 && cn2 == 1)
            திரும்பு FwdRealToCCS;
        இல்லையெனில் எனில் (cn1 == 1 && cn2 == 2)
            திரும்பு FwdRealToComplex;
        இல்லையெனில் எனில் (cn1 == 2 && cn2 == 2)
            திரும்பு FwdComplexToComplex;
    }
    இல்லையெனில்
    {
        எனில் (cn1 == 1 && cn2 == 1)
            திரும்பு InvCCSToReal;
        இல்லையெனில் எனில் (cn1 == 2 && cn2 == 1)
            திரும்பு InvComplexToReal;
        இல்லையெனில் எனில் (cn1 == 2 && cn2 == 2)
            திரும்பு InvComplexToComplex;
    }
    திரும்பு InvalidDft;
}


உள்வரி DftDims determineDims(முழு rows, முழு cols, தருக்க isRowWise, தருக்க isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    எனில் (isRowWise)
        திரும்பு OneDim;
    எனில் (cols == 1 && rows > 1) // one-column-shaped input
    {
        எனில் (isContinuous)
            திரும்பு OneDim;
        இல்லையெனில்
            திரும்பு OneDimColWise;
    }
    எனில் (rows == 1)
        திரும்பு OneDim;
    எனில் (cols > 1 && rows > 1)
        திரும்பு TwoDims;
    திரும்பு InvalidDim;
}

வகுப்பு OcvDftBasicImpl CV_FINAL : பொது hal::DFT1D
{
பொது:
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

பொது:
    OcvDftBasicImpl()
    {
#ifdef USE_IPP_DFT
        ipp_worksize = 0;
#endif
    }
    வெறுமை init(முழு len, முழு மொத்தம்_2, முழு depth, முழு flags, தருக்க *needBuffer)
    {
        முழு stage = (flags & CV_HAL_DFT_STAGE_COLS) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        தருக்க real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        தருக்க needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = பொய்;
    #ifdef USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        எனில்( CV_IPP_CHECK_COND && (opt.n*மொத்தம்_2 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            முழு ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            முழு specsize=0, initsize=0, worksize=0;
            IppDFTGetSizeFunc getSizeFunc = 0;
            IppDFTInitFunc initFunc = 0;

            எனில்( real_transform && stage == 0 )
            {
                எனில்( depth == CV_32F )
                {
                    getSizeFunc = ippsDFTGetSize_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                இல்லையெனில்
                {
                    getSizeFunc = ippsDFTGetSize_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            இல்லையெனில்
            {
                எனில்( depth == CV_32F )
                {
                    getSizeFunc = ippsDFTGetSize_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                இல்லையெனில்
                {
                    getSizeFunc = ippsDFTGetSize_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            எனில்( getSizeFunc(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                எனில்( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = உண்மை;
            }
            இல்லையெனில்
                setIppErrorStatus();
        }
    #endif

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            மாறிலி DFTKernels& kernels = getDFTKernels(depth);
            முழு kind = stage == 0 && real_transform ? (opt.isInverse ? DFT_KIND_C2R : DFT_KIND_R2C)
                                                    : DFT_KIND_C2C;
            plan.build(kind, len, depth, kernels.vlanes);
            ws.allocate(plan.ws_bytes);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.data();
            எனில் (needBuffer)
                *needBuffer = பொய்;   // the engine supports in-place for every kind
        }

        {
            நிலையான DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<தசம>>,
                dftWrap<தசம, RealDFT<தசம>>,
                dftWrap<தசம, CCSIDFT<தசம>>,
                dftWrap<Complexd, DFT<இரட்டை>>,
                dftWrap<இரட்டை, RealDFT<இரட்டை>>,
                dftWrap<இரட்டை, CCSIDFT<இரட்டை>>
            };
            முழு idx = 0;
            எனில் (stage == 0)
            {
                எனில் (real_transform)
                {
                    எனில் (!opt.isInverse)
                        idx = 1;
                    இல்லையெனில்
                        idx = 2;
                }
            }
            எனில் (depth == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        எனில்(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            முழு row_மொத்தம் = மொத்தம்_2;
            எனில் (stage == 0 && (flags & CV_HAL_DFT_ROWS) != 0)
                row_மொத்தம் = 1;
            opt.scale = 1./(len * row_மொத்தம்);
        }
    }

    வெறுமை apply(மாறிலி uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspaceSize() மாறிலி
    {
        size_t sz = opt.plan ? plan.ws_bytes : 0;
#ifdef USE_IPP_DFT
        எனில் (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#endif
        திரும்பு sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    வெறுமை applyWithWorkspace(மாறிலி uchar *src, uchar *dst, uchar* workspace) மாறிலி
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#ifdef USE_IPP_DFT
        எனில் (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#endif
        opt_.dft_func(opt_, src, dst);
    }

    வெறுமை விடுவி() {}
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
நிலையான மாறிலி முழு DFT_COLS_BATCH_MIN = 4;
நிலையான மாறிலி முழு DFT_COLS_BATCH_MAX = 16;
நிலையான மாறிலி size_t DFT_COLS_BATCH_BYTES = 1 << 17;

நிலையான முழு dftColsBatch(முழு len, size_t complex_elem_size)
{
    முழு batch = (முழு)(DFT_COLS_BATCH_BYTES/((size_t)len*complex_elem_size));
    திரும்பு std::max(DFT_COLS_BATCH_MIN, std::min(DFT_COLS_BATCH_MAX, batch));
}

நிலையான இரட்டை dftParallelStripes(முழு மொத்தம்_2, முழு len)
{
    மாறிலி இரட்டை quantum = 0.5e6;
    திரும்பு (இரட்டை)மொத்தம்_2*len*std::max(std::log((இரட்டை)len)/std::log(2.), 1.)/quantum;
}

வகுப்பு OcvDftImpl CV_FINAL : பொது hal::DFT2D
{
பாதுகாக்கப்பட்ட:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    மாறிலி OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    மாறிலி OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    தருக்க needBufferA;
    தருக்க needBufferB;
    தருக்க inv;
    முழு width;
    முழு height;
    DftMode mode;
    முழு elem_size;
    முழு complex_elem_size;
    முழு depth;
    தருக்க real_transform;
    முழு nonzero_rows;
    தருக்க isRowTransform;
    தருக்க isScaled;
    std::vector<முழு> stages;
    தருக்க useIpp;
    முழு src_channels;
    முழு dst_channels;

    AutoBuffer<uchar> tmp_bufA;
    AutoBuffer<uchar> tmp_bufB;
    AutoBuffer<uchar> buf0;
    AutoBuffer<uchar> buf1;

பொது:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        needBufferA = பொய்;
        needBufferB = பொய்;
        inv = பொய்;
        width = 0;
        height = 0;
        mode = InvalidDft;
        elem_size = 0;
        complex_elem_size = 0;
        depth = 0;
        real_transform = பொய்;
        nonzero_rows = 0;
        isRowTransform = பொய்;
        isScaled = பொய்;
        useIpp = பொய்;
        src_channels = 0;
        dst_channels = 0;
    }

    வெறுமை init(முழு _width, முழு _height, முழு _depth, முழு _src_channels, முழு _dst_channels, முழு flags, முழு _nonzero_rows)
    {
        தருக்க isComplex = _src_channels != _dst_channels;
        nonzero_rows = _nonzero_rows;
        width = _width;
        height = _height;
        depth = _depth;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        தருக்க isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        தருக்க isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        தருக்க isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        mode = determineMode(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        needBufferA = பொய்;
        needBufferB = பொய்;
        real_transform = (mode != FwdComplexToComplex && mode != InvComplexToComplex);

        elem_size = (depth == CV_32F) ? அளவு(தசம) : அளவு(இரட்டை);
        complex_elem_size = elem_size * 2;
        எனில்( !real_transform )
            elem_size = complex_elem_size;

#எனில் defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            எனில் (nonzero_rows == 0 && depth == CV_32F && ((width * height)>(முழு)(1<<6)))
            {
                எனில் (mode == FwdComplexToComplex || mode == InvComplexToComplex || mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    useIpp = உண்மை;
                    திரும்பு;
                }
            }
        }
#endif

        DftDims dims = determineDims(height, width, isRowTransform, isContinuous);
        எனில் (dims == TwoDims)
        {
            stages.resize(2);
            எனில் (mode == InvCCSToReal || mode == InvComplexToReal)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            இல்லையெனில்
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        இல்லையெனில்
        {
            stages.resize(1);
            எனில் (dims == OneDimColWise)
                stages[0] = 1;
            இல்லையெனில்
                stages[0] = 0;
        }

        க்கு(uint stageIndex = 0; stageIndex < stages.size(); ++stageIndex)
        {
            எனில் (stageIndex == 1)
            {
                isInplace = உண்மை;
                isComplex = பொய்;
            }

            முழு stage = stages[stageIndex];
            தருக்க isLastStage = (stageIndex + 1 == stages.size());

            முழு len, மொத்தம்_2;

            முழு f = 0;
            எனில் (inv)
                f |= CV_HAL_DFT_INVERSE;
            எனில் (isScaled)
                f |= CV_HAL_DFT_SCALE;
            எனில் (isRowTransform)
                f |= CV_HAL_DFT_ROWS;
            எனில் (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            எனில் (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            எனில் (!isLastStage)
                f |= CV_HAL_DFT_TWO_STAGE;

            எனில்( stage == 0 ) // row-wise transform
            {
                எனில் (width == 1 && !isRowTransform )
                {
                    len = height;
                    மொத்தம்_2 = width;
                }
                இல்லையெனில்
                {
                    len = width;
                    மொத்தம்_2 = height;
                }
                needBufferA = isInplace;
                contextA = hal::DFT1D::create(len, மொத்தம்_2, depth, f, &needBufferA);
                basicA = dynamic_cast<மாறிலி OcvDftBasicImpl*>(contextA.get());
                எனில் (needBufferA)
                    tmp_bufA.allocate(len * complex_elem_size);
            }
            இல்லையெனில்
            {
                len = height;
                மொத்தம்_2 = width;
                f |= CV_HAL_DFT_STAGE_COLS;
                needBufferB = isInplace;
                contextB = hal::DFT1D::create(len, மொத்தம்_2, depth, f, &needBufferB);
                basicB = dynamic_cast<மாறிலி OcvDftBasicImpl*>(contextB.get());
                எனில் (needBufferB)
                    tmp_bufB.allocate(len * complex_elem_size);

                buf0.allocate(len * complex_elem_size);
                buf1.allocate(len * complex_elem_size);
            }
        }
    }

    வெறுமை apply(மாறிலி uchar * src, size_t src_step, uchar * dst, size_t dst_step) CV_OVERRIDE
    {
#எனில் defined USE_IPP_DFT
        எனில் (useIpp)
        {
            முழு ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            எனில் (!isRowTransform)
            {
                எனில் (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    எனில் (ippi_DFT_C_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        திரும்பு;
                    }
                    setIppErrorStatus();
                }
                இல்லையெனில் எனில் (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    எனில் (ippi_DFT_R_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        திரும்பு;
                    }
                    setIppErrorStatus();
                }
            }
            இல்லையெனில்
            {
                எனில் (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    எனில் (Dft_C_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        திரும்பு;
                    }
                    setIppErrorStatus();
                }
                இல்லையெனில் எனில் (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippiDFTInv_PackToR_32f_C1R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    எனில் (Dft_R_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        திரும்பு;
                    }
                    setIppErrorStatus();
                }
            }
            திரும்பு;
        }
#endif

        க்கு(uint stageIndex = 0; stageIndex < stages.size(); ++stageIndex)
        {
            முழு stage_src_channels = src_channels;
            முழு stage_dst_channels = dst_channels;

            எனில் (stageIndex == 1)
            {
                src = dst;
                src_step = dst_step;
                stage_src_channels = stage_dst_channels;
            }

            முழு stage = stages[stageIndex];
            தருக்க isLastStage = (stageIndex + 1 == stages.size());
            தருக்க isComplex = stage_src_channels != stage_dst_channels;

            எனில்( stage == 0 )
                rowDft(src, src_step, dst, dst_step, isComplex, isLastStage);
            இல்லையெனில்
                colDft(src, src_step, dst, dst_step, stage_src_channels, stage_dst_channels, isLastStage);
        }
    }

பாதுகாக்கப்பட்ட:

    வெறுமை rowDft(மாறிலி uchar* src_data, size_t src_step, uchar* dst_data, size_t dst_step, தருக்க isComplex, தருக்க isLastStage)
    {
        முழு len, மொத்தம்_2;
        எனில் (width == 1 && !isRowTransform )
        {
            len = height;
            மொத்தம்_2 = width;
        }
        இல்லையெனில்
        {
            len = width;
            மொத்தம்_2 = height;
        }
        முழு dptr_offset = 0;
        முழு dst_full_len = len*elem_size;

        எனில்( needBufferA )
        {
            எனில் (mode == FwdRealToCCS && (len & 1) && len > 1)
                dptr_offset = elem_size;
        }

        எனில்( !inv && isComplex )
            dst_full_len += (len & 1) ? elem_size : complex_elem_size;

        முழு nz = nonzero_rows;
        எனில்( nz <= 0 || nz > மொத்தம்_2 )
            nz = மொத்தம்_2;

        size_t ws_size = basicA ? basicA->workspaceSize() : 0;
        இரட்டை nstripes = dftParallelStripes(nz, len);
        எனில்( ws_size > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            மாறிலி OcvDftBasicImpl* ctx = basicA;
            auto processRows = [&, ctx](மாறிலி Range& range)
            {
                AutoBuffer<uchar> ws(ws_size);
                AutoBuffer<uchar> rowbuf(needBufferA ? len * complex_elem_size : 0);
                க்கு( முழு i = range.start; i < range.end; i++ )
                {
                    மாறிலி uchar* sptr = src_data + src_step * i;
                    uchar* dptr0 = dst_data + dst_step * i;
                    uchar* dptr = needBufferA ? rowbuf.data() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.data());
                    எனில்( needBufferA )
                        memcpy( dptr0, dptr + dptr_offset, dst_full_len );
                }
            };
            parallel_for_(Range(0, nz), processRows, nstripes);
        }
        இல்லையெனில்
        {
            க்கு( முழு i = 0; i < nz; i++ )
            {
                மாறிலி uchar* sptr = src_data + src_step * i;
                uchar* dptr0 = dst_data + dst_step * i;
                uchar* dptr = dptr0;

                எனில்( needBufferA )
                    dptr = tmp_bufA.data();

                contextA->apply(sptr, dptr);

                எனில்( needBufferA )
                    memcpy( dptr0, dptr + dptr_offset, dst_full_len );
            }
        }

        க்கு( முழு i = nz; i < மொத்தம்_2; i++ )
        {
            uchar* dptr0 = dst_data + dst_step * i;
            memset( dptr0, 0, dst_full_len );
        }
        எனில்(isLastStage &&  mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_data, dst_step, len, nz, 1);
    }

    வெறுமை colDft(மாறிலி uchar* src_data, size_t src_step, uchar* dst_data, size_t dst_step, முழு stage_src_channels, முழு stage_dst_channels, தருக்க isLastStage)
    {
        முழு len = height;
        முழு மொத்தம்_2 = width;
        முழு a = 0, b = மொத்தம்_2;
        uchar *dbuf0, *dbuf1;
        மாறிலி uchar* sptr0 = src_data;
        uchar* dptr0 = dst_data;

        dbuf0 = buf0.data(), dbuf1 = buf1.data();

        எனில்( needBufferB )
        {
            dbuf1 = tmp_bufB.data();
            dbuf0 = buf1.data();
        }

        எனில்( real_transform )
        {
            முழு even;
            a = 1;
            even = (மொத்தம்_2 & 1) == 0;
            b = (மொத்தம்_2+1)/2;
            எனில்( !inv )
            {
                memset( buf0.data(), 0, len*complex_elem_size );
                CopyColumn( sptr0, src_step, buf0.data(), complex_elem_size, len, elem_size );
                sptr0 += stage_dst_channels*elem_size;
                எனில்( even )
                {
                    memset( buf1.data(), 0, len*complex_elem_size );
                    CopyColumn( sptr0 + (மொத்தம்_2-2)*elem_size, src_step,
                                buf1.data(), complex_elem_size, len, elem_size );
                }
            }
            இல்லையெனில் எனில்( stage_src_channels == 1 )
            {
                CopyColumn( sptr0, src_step, buf0.data(), elem_size, len, elem_size );
                ExpandCCS( buf0.data(), len, elem_size );
                எனில்( even )
                {
                    CopyColumn( sptr0 + (மொத்தம்_2-1)*elem_size, src_step,
                                buf1.data(), elem_size, len, elem_size );
                    ExpandCCS( buf1.data(), len, elem_size );
                }
                sptr0 += elem_size;
            }
            இல்லையெனில்
            {
                CopyColumn( sptr0, src_step, buf0.data(), complex_elem_size, len, complex_elem_size );
                எனில்( even )
                {
                    CopyColumn( sptr0 + b*complex_elem_size, src_step,
                                   buf1.data(), complex_elem_size, len, complex_elem_size );
                }
                sptr0 += complex_elem_size;
            }

            எனில்( even )
                contextB->apply(buf1.data(), dbuf1);
            contextB->apply(buf0.data(), dbuf0);

            எனில்( stage_dst_channels == 1 )
            {
                எனில்( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    memcpy( dbuf0 + elem_size, dbuf0, elem_size );
                    CopyColumn( dbuf0 + elem_size, elem_size, dptr0,
                                   dst_step, len, elem_size );
                    எனில்( even )
                    {
                        memcpy( dbuf1 + elem_size, dbuf1, elem_size );
                        CopyColumn( dbuf1 + elem_size, elem_size,
                                       dptr0 + (மொத்தம்_2-1)*elem_size,
                                       dst_step, len, elem_size );
                    }
                    dptr0 += elem_size;
                }
                இல்லையெனில்
                {
                    // copy the real part of the complex vector to the first/last column
                    CopyColumn( dbuf0, complex_elem_size, dptr0, dst_step, len, elem_size );
                    எனில்( even )
                        CopyColumn( dbuf1, complex_elem_size, dptr0 + (மொத்தம்_2-1)*elem_size,
                                       dst_step, len, elem_size );
                    dptr0 += elem_size;
                }
            }
            இல்லையெனில்
            {
                CV_Assert( !inv );
                CopyColumn( dbuf0, complex_elem_size, dptr0,
                               dst_step, len, complex_elem_size );
                எனில்( even )
                    CopyColumn( dbuf1, complex_elem_size,
                                   dptr0 + b*complex_elem_size,
                                   dst_step, len, complex_elem_size );
                dptr0 += complex_elem_size;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        முழு ncols = b - a;
        மாறிலி முழு cols_batch = dftColsBatch(len, complex_elem_size);
        முழு nbatches = (ncols + cols_batch - 1)/cols_batch;
        size_t ws_size = basicB ? basicB->workspaceSize() : 0;
        size_t colbuf_size = (size_t)len*complex_elem_size;
        மாறிலி OcvDftBasicImpl* ctx = basicB;
        auto processBatches = [&, ctx](மாறிலி Range& range)
        {
            AutoBuffer<uchar> ws(ws_size);
            AutoBuffer<uchar> cbuf(colbuf_size*cols_batch*(needBufferB ? 2 : 1));
            uchar* cbuf0 = cbuf.data();
            uchar* cdbuf0 = needBufferB ? cbuf0 + colbuf_size*cols_batch : cbuf0;
            க்கு( முழு bi = range.start; bi < range.end; bi++ )
            {
                முழு c0 = bi*cols_batch;
                முழு nc = std::min(cols_batch, ncols - c0);
                மாறிலி uchar* sptr = sptr0 + (size_t)c0*complex_elem_size;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_size;
                CopyFromColumns( sptr, src_step, cbuf0, nc, len, complex_elem_size );
                க்கு( முழு k = 0; k < nc; k++ )
                {
                    எனில்( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_size, cdbuf0 + k*colbuf_size, ws.data());
                    இல்லையெனில்
                        contextB->apply(cbuf0 + k*colbuf_size, cdbuf0 + k*colbuf_size);
                }
                CopyToColumns( cdbuf0, dptr, dst_step, nc, len, complex_elem_size );
            }
        };
        இரட்டை nstripes = dftParallelStripes(ncols, len);
        எனில்( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        இல்லையெனில்
            processBatches(Range(0, nbatches));
        எனில்(isLastStage && mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_data, dst_step, மொத்தம்_2, len, 2);
    }
};

கட்டமைப்பு ReplacementDFT1D : பொது hal::DFT1D
{
    cvhalDFT *context;
    தருக்க isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(பொய்) {}
    தருக்க init(முழு len, முழு மொத்தம்_2, முழு depth, முழு flags, தருக்க *needBuffer)
    {
        முழு res = cv_hal_dftInit1D(&context, len, மொத்தம்_2, depth, flags, needBuffer);
        isInitialized = (res == CV_HAL_ERROR_OK);
        திரும்பு isInitialized;
    }
    வெறுமை apply(மாறிலி uchar *src, uchar *dst) CV_OVERRIDE
    {
        எனில் (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        எனில் (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

கட்டமைப்பு ReplacementDFT2D : பொது hal::DFT2D
{
    cvhalDFT *context;
    தருக்க isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(பொய்) {}
    தருக்க init(முழு width, முழு height, முழு depth,
              முழு src_channels, முழு dst_channels,
              முழு flags, முழு nonzero_rows)
    {
        முழு res = cv_hal_dftInit2D(&context, width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        isInitialized = (res == CV_HAL_ERROR_OK);
        திரும்பு isInitialized;
    }
    வெறுமை apply(மாறிலி uchar *src, size_t src_step, uchar *dst, size_t dst_step) CV_OVERRIDE
    {
        எனில் (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_step, dst, dst_step);
        }
    }
    ~ReplacementDFT2D()
    {
        எனில் (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

நாமாகாஶ hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(முழு len, முழு மொத்தம்_2, முழு depth, முழு flags, தருக்க *needBuffer)
{
    {
        ReplacementDFT1D *impl = புதிய ReplacementDFT1D();
        எனில் (impl->init(len, மொத்தம்_2, depth, flags, needBuffer))
        {
            திரும்பு Ptr<DFT1D>(impl);
        }
        delete impl;
    }
    {
        OcvDftBasicImpl *impl = புதிய OcvDftBasicImpl();
        impl->init(len, மொத்தம்_2, depth, flags, needBuffer);
        திரும்பு Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(முழு width, முழு height, முழு depth,
                         முழு src_channels, முழு dst_channels,
                         முழு flags, முழு nonzero_rows)
{
    {
        ReplacementDFT2D *impl = புதிய ReplacementDFT2D();
        எனில் (impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows))
        {
            திரும்பு Ptr<DFT2D>(impl);
        }
        delete impl;
    }
    {
        எனில்(width == 1 && nonzero_rows > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = புதிய OcvDftImpl();
        impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        திரும்பு Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


வெறுமை cv::dft( உள்ளீடு_Array _src0, OutputArray _dst, முழு flags, முழு nonzero_rows )
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
    எனில் (src.empty())
    {
        _dst.release();
        திரும்பு;
    }
    தருக்க inv = (flags & DFT_INVERSE) != 0;
    முழு type = src.type();
    முழு depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_உள்ளீடு) && src.channels() != 2) );

    எனில்( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.createSameSize( src, CV_MAKETYPE(depth, 2) );
    இல்லையெனில் எனில்( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.createSameSize( src, depth );
    இல்லையெனில்
        _dst.createSameSize( src, type );

    Mat dst = _dst.getMat();

    முழு f = 0;
    எனில் (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    எனில் (inv)
        f |= CV_HAL_DFT_INVERSE;
    எனில் (flags & DFT_ROWS)
        f |= CV_HAL_DFT_ROWS;
    எனில் (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    எனில் (src.data == dst.data)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.cols, src.rows, depth, src.channels(), dst.channels(), f, nonzero_rows);
    c->apply(src.data, src.step, dst.data, dst.step);
}


வெறுமை cv::idft( உள்ளீடு_Array src, OutputArray dst, முழு flags, முழு nonzero_rows )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_rows );
}

#ifdef HAVE_OPENCL

நாமாகாஶ cv {

நிலையான தருக்க ocl_mulSpectrums( உள்ளீடு_Array _srcA, உள்ளீடு_Array _srcB,
                              OutputArray _dst, முழு flags, தருக்க conjB )
{
    முழு atype = _srcA.type(), btype = _srcB.type(),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    எனில் ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        திரும்பு பொய்;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.createSameSize(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    எனில் (k.empty())
        திரும்பு பொய்;

    k.args(ocl::KernelArg::ReadOnlyNoSize(A), ocl::KernelArg::ReadOnlyNoSize(B),
           ocl::KernelArg::WriteOnly(dst), rowsPerWI);

    size_t globalsize[2] = { (size_t)asize.width, ((size_t)asize.height + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

}

#endif

நாமாகாஶ {

#define VAL(buf, elem) (((T*)((எழுத்து*)data ## buf + (step ## buf * (elem))))[0])
#define MUL_SPECTRUMS_COL(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    க்கு (size_t j = 1; j <= rows - 2; j += 2) \
    { \
        இரட்டை a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        இரட்டை b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        எனில் (conjB) b_im = -b_im; \
        இரட்டை c_re = a_re * b_re - a_im * b_im; \
        இரட்டை c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    எனில் ((rows & 1) == 0) \
        VAL(C, rows-1) = VAL(A, rows-1) * VAL(B, rows-1)

template <typename T, தருக்க conjB> நிலையான உள்வரி
வெறுமை mulSpectrums_processCol_noinplace(மாறிலி T* dataA, மாறிலி T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows)
{
    MUL_SPECTRUMS_COL(A, B, C);
}

template <typename T, தருக்க conjB> நிலையான உள்வரி
வெறுமை mulSpectrums_processCol_inplaceA(மாறிலி T* dataB, T* dataAC, size_t stepB, size_t stepAC, size_t rows)
{
    MUL_SPECTRUMS_COL(AC, B, AC);
}
template <typename T, தருக்க conjB, தருக்க inplaceA> நிலையான உள்வரி
வெறுமை mulSpectrums_processCol(மாறிலி T* dataA, மாறிலி T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows)
{
    எனில் (inplaceA)
        mulSpectrums_processCol_inplaceA<T, conjB>(dataB, dataC, stepB, stepC, rows);
    இல்லையெனில்
        mulSpectrums_processCol_noinplace<T, conjB>(dataA, dataB, dataC, stepA, stepB, stepC, rows);
}
#undef MUL_SPECTRUMS_COL
#undef VAL

template <typename T, தருக்க conjB, தருக்க inplaceA> நிலையான உள்வரி
வெறுமை mulSpectrums_processCols(மாறிலி T* dataA, மாறிலி T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols)
{
    mulSpectrums_processCol<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows);
    எனில் ((cols & 1) == 0)
    {
        mulSpectrums_processCol<T, conjB, inplaceA>(dataA + cols - 1, dataB + cols - 1, dataC + cols - 1, stepA, stepB, stepC, rows);
    }
}

#define VAL(buf, elem) (data ## buf[(elem)])
#define MUL_SPECTRUMS_ROW(A, B, C) \
    க்கு (size_t j = j0; j < j1; j += 2) \
    { \
        இரட்டை a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        இரட்டை b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        எனில் (conjB) b_im = -b_im; \
        இரட்டை c_re = a_re * b_re - a_im * b_im; \
        இரட்டை c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
template <typename T, தருக்க conjB> நிலையான உள்வரி
வெறுமை mulSpectrums_processRow_noinplace(மாறிலி T* dataA, மாறிலி T* dataB, T* dataC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(A, B, C);
}
template <typename T, தருக்க conjB> நிலையான உள்வரி
வெறுமை mulSpectrums_processRow_inplaceA(மாறிலி T* dataB, T* dataAC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(AC, B, AC);
}
template <typename T, தருக்க conjB, தருக்க inplaceA> நிலையான உள்வரி
வெறுமை mulSpectrums_processRow(மாறிலி T* dataA, மாறிலி T* dataB, T* dataC, size_t j0, size_t j1)
{
    எனில் (inplaceA)
        mulSpectrums_processRow_inplaceA<T, conjB>(dataB, dataC, j0, j1);
    இல்லையெனில்
        mulSpectrums_processRow_noinplace<T, conjB>(dataA, dataB, dataC, j0, j1);
}
#undef MUL_SPECTRUMS_ROW
#undef VAL

template <typename T, தருக்க conjB, தருக்க inplaceA> நிலையான உள்வரி
வெறுமை mulSpectrums_processRows(மாறிலி T* dataA, மாறிலி T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, தருக்க is_1d_CN1)
{
    போது (rows-- > 0)
    {
        எனில் (is_1d_CN1)
            dataC[0] = dataA[0]*dataB[0];
        mulSpectrums_processRow<T, conjB, inplaceA>(dataA, dataB, dataC, j0, j1);
        எனில் (is_1d_CN1 && (cols & 1) == 0)
            dataC[j1] = dataA[j1]*dataB[j1];

        dataA = (மாறிலி T*)(((எழுத்து*)dataA) + stepA);
        dataB = (மாறிலி T*)(((எழுத்து*)dataB) + stepB);
        dataC =       (T*)(((எழுத்து*)dataC) + stepC);
    }
}


template <typename T, தருக்க conjB, தருக்க inplaceA> நிலையான உள்வரி
வெறுமை mulSpectrums_Impl_(மாறிலி T* dataA, மாறிலி T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, தருக்க is_1d, தருக்க isCN1)
{
    எனில் (!is_1d && isCN1)
    {
        mulSpectrums_processCols<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols);
    }
    mulSpectrums_processRows<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d && isCN1);
}
template <typename T, தருக்க conjB> நிலையான உள்வரி
வெறுமை mulSpectrums_Impl(மாறிலி T* dataA, மாறிலி T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, தருக்க is_1d, தருக்க isCN1)
{
    எனில் (dataA == dataC)
        mulSpectrums_Impl_<T, conjB, உண்மை>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
    இல்லையெனில்
        mulSpectrums_Impl_<T, conjB, பொய்>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
}

} // namespace

வெறுமை cv::mulSpectrums( உள்ளீடு_Array _srcA, உள்ளீடு_Array _srcB,
                       OutputArray _dst, முழு flags, தருக்க conjB )
{
    CV_INSTRUMENT_REGION();

    எனில் (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        திரும்பு;
    }

    முழு type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_RUN(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    முழு depth = srcA.depth(), cn = srcA.channels();
    size_t rows = srcA.rows, cols = srcA.cols;

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    எனில் (dst.data == srcB.data)
        srcB = srcB.clone(); // workaround for B only

    தருக்க is_1d = (flags & DFT_ROWS)
        || (rows == 1)
        || (cols == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    எனில்( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    தருக்க isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = cols*cn - (((cols & 1) == 0 && cn == 1) ? 1 : 0);

    எனில் (depth == CV_32F)
    {
        மாறிலி தசம* dataA = srcA.ptr<தசம>();
        மாறிலி தசம* dataB = srcB.ptr<தசம>();
        தசம* dataC = dst.ptr<தசம>();
        எனில் (!conjB)
            mulSpectrums_Impl<தசம, பொய்>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        இல்லையெனில்
            mulSpectrums_Impl<தசம, உண்மை>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
    இல்லையெனில்
    {
        மாறிலி இரட்டை* dataA = srcA.ptr<இரட்டை>();
        மாறிலி இரட்டை* dataB = srcB.ptr<இரட்டை>();
        இரட்டை* dataC = dst.ptr<இரட்டை>();
        எனில் (!conjB)
            mulSpectrums_Impl<இரட்டை, பொய்>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        இல்லையெனில்
            mulSpectrums_Impl<இரட்டை, உண்மை>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
}

வெறுமை cv::divSpectrums( உள்ளீடு_Array _srcA, உள்ளீடு_Array _srcB, OutputArray _dst, முழு flags, தருக்க conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    எனில் (srcA.empty() && srcB.empty())
    {
        _dst.release();
        திரும்பு;
    }
    முழு depth = srcA.depth(), cn = srcA.channels(), type = srcA.type();
    முழு rows = srcA.rows, cols = srcA.cols;
    முழு j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.data != srcA.data); // non-inplace check
    CV_Assert(dst.data != srcB.data); // non-inplace check

    தருக்க is_1d = (flags & DFT_ROWS) || (rows == 1 || (cols == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    எனில்( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    முழு ncols = cols*cn;
    முழு j0 = cn == 1;
    முழு j1 = ncols - (cols % 2 == 0 && cn == 1);

    எனில்( depth == CV_32F )
    {
        மாறிலி தசம* dataA = srcA.ptr<தசம>();
        மாறிலி தசம* dataB = srcB.ptr<தசம>();
        தசம* dataC = dst.ptr<தசம>();
        தசம eps = FLT_EPSILON; // prevent div0 problems

        size_t stepA = srcA.step/அளவு(dataA[0]);
        size_t stepB = srcB.step/அளவு(dataB[0]);
        size_t stepC = dst.step/அளவு(dataC[0]);

        எனில்( !is_1d && cn == 1 )
        {
            க்கு( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                எனில்( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0] / (dataB[0] + eps);
                எனில்( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA] / (dataB[(rows-1)*stepB] + eps);
                எனில்( !conjB )
                    க்கு( j = 1; j <= rows - 2; j += 2 )
                    {
                        இரட்டை denom = (இரட்டை)dataB[j*stepB]*dataB[j*stepB] +
                        (இரட்டை)dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + (இரட்டை)eps;

                        இரட்டை re = (இரட்டை)dataA[j*stepA]*dataB[j*stepB] +
                        (இரட்டை)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        இரட்டை im = (இரட்டை)dataA[(j+1)*stepA]*dataB[j*stepB] -
                        (இரட்டை)dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = (தசம)(re / denom);
                        dataC[(j+1)*stepC] = (தசம)(im / denom);
                    }
                    இல்லையெனில்
                        க்கு( j = 1; j <= rows - 2; j += 2 )
                        {

                            இரட்டை denom = (இரட்டை)dataB[j*stepB]*dataB[j*stepB] +
                            (இரட்டை)dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + (இரட்டை)eps;

                            இரட்டை re = (இரட்டை)dataA[j*stepA]*dataB[j*stepB] -
                            (இரட்டை)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                            இரட்டை im = (இரட்டை)dataA[(j+1)*stepA]*dataB[j*stepB] +
                            (இரட்டை)dataA[j*stepA]*dataB[(j+1)*stepB];

                            dataC[j*stepC] = (தசம)(re / denom);
                            dataC[(j+1)*stepC] = (தசம)(im / denom);
                        }
                    எனில்( k == 1 )
                        dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        க்கு( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            எனில்( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0] / (dataB[0] + eps);
                எனில்( cols % 2 == 0 )
                    dataC[j1] = dataA[j1] / (dataB[j1] + eps);
            }

            எனில்( !conjB )
                க்கு( j = j0; j < j1; j += 2 )
                {
                    இரட்டை denom = (இரட்டை)dataB[j]*dataB[j] + (இரட்டை)dataB[j+1]*dataB[j+1] + (இரட்டை)eps;
                    இரட்டை re = (இரட்டை)dataA[j]*dataB[j] + (இரட்டை)dataA[j+1]*dataB[j+1];
                    இரட்டை im = (இரட்டை)dataA[j+1]*dataB[j] - (இரட்டை)dataA[j]*dataB[j+1];
                    dataC[j] = (தசம)(re / denom);
                    dataC[j+1] = (தசம)(im / denom);
                }
                இல்லையெனில்
                    க்கு( j = j0; j < j1; j += 2 )
                    {
                        இரட்டை denom = (இரட்டை)dataB[j]*dataB[j] + (இரட்டை)dataB[j+1]*dataB[j+1] + (இரட்டை)eps;
                        இரட்டை re = (இரட்டை)dataA[j]*dataB[j] - (இரட்டை)dataA[j+1]*dataB[j+1];
                        இரட்டை im = (இரட்டை)dataA[j+1]*dataB[j] + (இரட்டை)dataA[j]*dataB[j+1];
                        dataC[j] = (தசம)(re / denom);
                        dataC[j+1] = (தசம)(im / denom);
                    }
        }
    }
    இல்லையெனில்
    {
        மாறிலி இரட்டை* dataA = srcA.ptr<இரட்டை>();
        மாறிலி இரட்டை* dataB = srcB.ptr<இரட்டை>();
        இரட்டை* dataC = dst.ptr<இரட்டை>();
        இரட்டை eps = DBL_EPSILON; // prevent div0 problems

        size_t stepA = srcA.step/அளவு(dataA[0]);
        size_t stepB = srcB.step/அளவு(dataB[0]);
        size_t stepC = dst.step/அளவு(dataC[0]);

        எனில்( !is_1d && cn == 1 )
        {
            க்கு( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                எனில்( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0] / (dataB[0] + eps);
                எனில்( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA] / (dataB[(rows-1)*stepB] + eps);
                எனில்( !conjB )
                    க்கு( j = 1; j <= rows - 2; j += 2 )
                    {
                        இரட்டை denom = dataB[j*stepB]*dataB[j*stepB] +
                        dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + eps;

                        இரட்டை re = dataA[j*stepA]*dataB[j*stepB] +
                        dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        இரட்டை im = dataA[(j+1)*stepA]*dataB[j*stepB] -
                        dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = re / denom;
                        dataC[(j+1)*stepC] = im / denom;
                    }
                    இல்லையெனில்
                        க்கு( j = 1; j <= rows - 2; j += 2 )
                        {
                            இரட்டை denom = dataB[j*stepB]*dataB[j*stepB] +
                            dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + eps;

                            இரட்டை re = dataA[j*stepA]*dataB[j*stepB] -
                            dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                            இரட்டை im = dataA[(j+1)*stepA]*dataB[j*stepB] +
                            dataA[j*stepA]*dataB[(j+1)*stepB];

                            dataC[j*stepC] = re / denom;
                            dataC[(j+1)*stepC] = im / denom;
                        }
                    எனில்( k == 1 )
                        dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        க்கு( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            எனில்( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0] / (dataB[0] + eps);
                எனில்( cols % 2 == 0 )
                    dataC[j1] = dataA[j1] / (dataB[j1] + eps);
            }

            எனில்( !conjB )
                க்கு( j = j0; j < j1; j += 2 )
                {
                    இரட்டை denom = dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps;
                    இரட்டை re = dataA[j]*dataB[j] + dataA[j+1]*dataB[j+1];
                    இரட்டை im = dataA[j+1]*dataB[j] - dataA[j]*dataB[j+1];
                    dataC[j] = re / denom;
                    dataC[j+1] = im / denom;
                }
                இல்லையெனில்
                    க்கு( j = j0; j < j1; j += 2 )
                    {
                        இரட்டை denom = dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps;
                        இரட்டை re = dataA[j]*dataB[j] - dataA[j+1]*dataB[j+1];
                        இரட்டை im = dataA[j+1]*dataB[j] + dataA[j]*dataB[j+1];
                        dataC[j] = re / denom;
                        dataC[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

நாமாகாஶ cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
template<typename T> நிலையான வெறுமை
DCT( மாறிலி OcvDftOptions & c, மாறிலி T* src, size_t src_step, T* dst, size_t dst_step )
{
    எனில்( c.n == 1 )
    {
        dst[0] = src[0];
        திரும்பு;
    }
    runDft(c, src, src_step/அளவு(T), dst, dst_step/அளவு(T), பொய்);
}

template<typename T> நிலையான வெறுமை
IDCT( மாறிலி OcvDftOptions & c, மாறிலி T* src, size_t src_step, T* dst, size_t dst_step )
{
    எனில்( c.n == 1 )
    {
        dst[0] = src[0];
        திரும்பு;
    }
    runDft(c, src, src_step/அளவு(T), dst, dst_step/அளவு(T), பொய்);
}

வகைவரை வெறுமை (*DCTFunc)(மாறிலி OcvDftOptions & c, மாறிலி வெறுமை* src, size_t src_step,
                        வெறுமை* dst, size_t dst_step);

template<typename T, வெறுமை (*fn)(மாறிலி OcvDftOptions&, மாறிலி T*, size_t, T*, size_t)>
நிலையான வெறுமை dctWrap(மாறிலி OcvDftOptions & c, மாறிலி வெறுமை* src, size_t src_step, வெறுமை* dst, size_t dst_step)
{
    fn(c, (மாறிலி T*)src, src_step, (T*)dst, dst_step);
}

}

#ifdef HAVE_IPP
நாமாகாஶ cv
{

#எனில் IPP_VERSION_X100 >= 900
வகைவரை IppStatus (CV_STDCALL * ippiDCTFunc)(மாறிலி Ipp32f* pSrc, முழு srcStep, Ipp32f* pDst, முழு dstStep, மாறிலி வெறுமை* pDCTSpec, Ipp8u* pBuffer);
வகைவரை IppStatus (CV_STDCALL * ippiDCTInit)(வெறுமை* pDCTSpec, IppiSize roiSize, Ipp8u* pMemInit );
வகைவரை IppStatus (CV_STDCALL * ippiDCTGetSize)(IppiSize roiSize, முழு* pSizeSpec, முழு* pSizeInit, முழு* pSizeBuf);

template<typename SpecType, IppStatus (CV_STDCALL *fn)(மாறிலி Ipp32f*, முழு, Ipp32f*, முழு, மாறிலி SpecType*, Ipp8u*)>
நிலையான IppStatus CV_STDCALL ippiDCTWrap(மாறிலி Ipp32f* pSrc, முழு srcStep, Ipp32f* pDst, முழு dstStep, மாறிலி வெறுமை* pDCTSpec, Ipp8u* pBuffer)
{
    திரும்பு fn(pSrc, srcStep, pDst, dstStep, (மாறிலி SpecType*)pDCTSpec, pBuffer);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*, IppiSize, Ipp8u*)>
நிலையான IppStatus CV_STDCALL ippiDCTInitWrap(வெறுமை* pDCTSpec, IppiSize roiSize, Ipp8u* pMemInit)
{
    திரும்பு fn((SpecType*)pDCTSpec, roiSize, pMemInit);
}

template<IppStatus (CV_STDCALL *fn)(IppiSize, முழு*, முழு*, முழு*)>
நிலையான IppStatus CV_STDCALL ippiDCTGetSizeWrap(IppiSize roiSize, முழு* pSizeSpec, முழு* pSizeInit, முழு* pSizeBuf)
{
    திரும்பு fn(roiSize, pSizeSpec, pSizeInit, pSizeBuf);
}

#அல்லதெனில் IPP_VERSION_X100 >= 700
வகைவரை IppStatus (CV_STDCALL * ippiDCTFunc)(மாறிலி Ipp32f*, முழு, Ipp32f*, முழு, மாறிலி வெறுமை*, Ipp8u*);
வகைவரை IppStatus (CV_STDCALL * ippiDCTInitAlloc)(வெறுமை**, IppiSize, IppHintAlgorithm);
வகைவரை IppStatus (CV_STDCALL * ippiDCTFree)(வெறுமை* pDCTSpec);
வகைவரை IppStatus (CV_STDCALL * ippiDCTGetBufSize)(மாறிலி வெறுமை*, முழு*);

template<typename SpecType, IppStatus (CV_STDCALL *fn)(மாறிலி Ipp32f*, முழு, Ipp32f*, முழு, மாறிலி SpecType*, Ipp8u*)>
நிலையான IppStatus CV_STDCALL ippiDCTWrap(மாறிலி Ipp32f* pSrc, முழு srcStep, Ipp32f* pDst, முழு dstStep, மாறிலி வெறுமை* pDCTSpec, Ipp8u* pBuffer)
{
    திரும்பு fn(pSrc, srcStep, pDst, dstStep, (மாறிலி SpecType*)pDCTSpec, pBuffer);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType**, IppiSize, IppHintAlgorithm)>
நிலையான IppStatus CV_STDCALL ippiDCTInitAllocWrap(வெறுமை** pDCTSpec, IppiSize roiSize, IppHintAlgorithm hint)
{
    திரும்பு fn((SpecType**)pDCTSpec, roiSize, hint);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*)>
நிலையான IppStatus CV_STDCALL ippiDCTFreeWrap(வெறுமை* pDCTSpec)
{
    திரும்பு fn((SpecType*)pDCTSpec);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(மாறிலி SpecType*, முழு*)>
நிலையான IppStatus CV_STDCALL ippiDCTGetBufSizeWrap(மாறிலி வெறுமை* pDCTSpec, முழு* pSize)
{
    திரும்பு fn((மாறிலி SpecType*)pDCTSpec, pSize);
}
#endif

வகுப்பு DctIPPLoop_Invoker : பொது ParallelLoopBody
{
பொது:
    DctIPPLoop_Invoker(மாறிலி uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, முழு _width, தருக்க _inv, தருக்க *_ok) :
        ParallelLoopBody(), src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width), inv(_inv), ok(_ok)
    {
        *ok = உண்மை;
    }

    மெய்நிகர் வெறுமை operator()(மாறிலி Range& range) மாறிலி CV_OVERRIDE
    {
        எனில்(*ok == பொய்)
            திரும்பு;

#எனில் IPP_VERSION_X100 >= 900
        IppiSize srcRoiSize = {width, 1};

        முழு specSize    = 0;
        முழு initSize    = 0;
        முழு bufferSize  = 0;

        Ipp8u* pDCTSpec = வெற்று;
        Ipp8u* pBuffer  = வெற்று;
        Ipp8u* pInitBuf = வெற்று;

        #define IPP_RETURN              \
            எனில்(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            எனில்(pBuffer)                 \
                ippFree(pBuffer);       \
            எனில்(pInitBuf)                \
                ippFree(pInitBuf);      \
            திரும்பு;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippiDCTGetSize  ippDctGetSize  = inv ? ippiDCTGetSizeWrap<ippiDCTInvGetSize_32f> : ippiDCTGetSizeWrap<ippiDCTFwdGetSize_32f>;

        எனில்(ippDctGetSize(srcRoiSize, &specSize, &initSize, &bufferSize) < 0)
        {
            *ok = பொய்;
            திரும்பு;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(specSize);
        எனில்(!pDCTSpec && specSize)
        {
            *ok = பொய்;
            திரும்பு;
        }

        pBuffer  = (Ipp8u*)CV_IPP_MALLOC(bufferSize);
        எனில்(!pBuffer && bufferSize)
        {
            *ok = பொய்;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(initSize);
        எனில்(!pInitBuf && initSize)
        {
            *ok = பொய்;
            IPP_RETURN
        }

        எனில்(ippDctInit(pDCTSpec, srcRoiSize, pInitBuf) < 0)
        {
            *ok = பொய்;
            IPP_RETURN
        }

        க்கு(முழு i = range.start; i < range.end; ++i)
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (தசம*)(src + src_step * i), static_cast<முழு>(src_step), (தசம*)(dst + dst_step * i), static_cast<முழு>(dst_step), pDCTSpec, pBuffer) < 0)
            {
                *ok = பொய்;
                IPP_RETURN
            }
        }
        IPP_RETURN
#undef IPP_RETURN
#அல்லதெனில் IPP_VERSION_X100 >= 700
        வெறுமை* pDCTSpec;
        AutoBuffer<uchar> buf;
        uchar* pBuffer = 0;
        முழு bufSize=0;

        IppiSize srcRoiSize = {width, 1};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippiDCTGetBufSize ippGetBufSize = inv ? ippiDCTGetBufSizeWrap<IppiDCTInvSpec_32f, ippiDCTInvGetBufSize_32f> : ippiDCTGetBufSizeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdGetBufSize_32f>;

        எனில் (ippInitAlloc(&pDCTSpec, srcRoiSize, ippAlgHintNone)>=0 && ippGetBufSize(pDCTSpec, &bufSize)>=0)
        {
            buf.allocate( bufSize );
            pBuffer = (uchar*)buf;

            க்கு( முழு i = range.start; i < range.end; ++i)
            {
                எனில்(ippDctFun((தசம*)(src + src_step * i), static_cast<முழு>(src_step), (தசம*)(dst + dst_step * i), static_cast<முழு>(dst_step), pDCTSpec, (Ipp8u*)pBuffer) < 0)
                {
                    *ok = பொய்;
                    நிறுத்து;
                }
            }
        }
        இல்லையெனில்
            *ok = பொய்;

        எனில் (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END
#இல்லையெனில்
        CV_UNUSED(range);
        *ok = பொய்;
#endif
    }

தனிப்பட்ட:
    மாறிலி uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    முழு width;
    தருக்க inv;
    தருக்க *ok;
};

நிலையான தருக்க DctIPPLoop(மாறிலி uchar * src, size_t src_step, uchar * dst, size_t dst_step, முழு width, முழு height, தருக்க inv)
{
    தருக்க ok;
    parallel_for_(Range(0, height), DctIPPLoop_Invoker(src, src_step, dst, dst_step, width, inv, &ok), height/(இரட்டை)(1<<4) );
    திரும்பு ok;
}

நிலையான தருக்க ippi_DCT_32f(மாறிலி uchar * src, size_t src_step, uchar * dst, size_t dst_step, முழு width, முழு height, தருக்க inv, தருக்க row)
{
    CV_INSTRUMENT_REGION_IPP();

    எனில்(row)
        திரும்பு DctIPPLoop(src, src_step, dst, dst_step, width, height, inv);
    இல்லையெனில்
    {
#எனில் IPP_VERSION_X100 >= 900
        IppiSize srcRoiSize = {width, height};

        முழு specSize    = 0;
        முழு initSize    = 0;
        முழு bufferSize  = 0;

        Ipp8u* pDCTSpec = வெற்று;
        Ipp8u* pBuffer  = வெற்று;
        Ipp8u* pInitBuf = வெற்று;

        #define IPP_RELEASE             \
            எனில்(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            எனில்(pBuffer)                 \
                ippFree(pBuffer);       \
            எனில்(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippiDCTGetSize  ippDctGetSize  = inv ? ippiDCTGetSizeWrap<ippiDCTInvGetSize_32f> : ippiDCTGetSizeWrap<ippiDCTFwdGetSize_32f>;

        எனில்(ippDctGetSize(srcRoiSize, &specSize, &initSize, &bufferSize) < 0)
            திரும்பு பொய்;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(specSize);
        எனில்(!pDCTSpec && specSize)
            திரும்பு பொய்;

        pBuffer  = (Ipp8u*)CV_IPP_MALLOC(bufferSize);
        எனில்(!pBuffer && bufferSize)
        {
            IPP_RELEASE
            திரும்பு பொய்;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(initSize);
        எனில்(!pInitBuf && initSize)
        {
            IPP_RELEASE
            திரும்பு பொய்;
        }

        எனில்(ippDctInit(pDCTSpec, srcRoiSize, pInitBuf) < 0)
        {
            IPP_RELEASE
            திரும்பு பொய்;
        }

        எனில்(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (தசம*)src, static_cast<முழு>(src_step), (தசம*)dst, static_cast<முழு>(dst_step), pDCTSpec, pBuffer) < 0)
        {
            IPP_RELEASE
            திரும்பு பொய்;
        }

        IPP_RELEASE
        திரும்பு உண்மை;
#undef IPP_RELEASE
#அல்லதெனில் IPP_VERSION_X100 >= 700
        IppStatus status;
        வெறுமை* pDCTSpec;
        AutoBuffer<uchar> buf;
        uchar* pBuffer = 0;
        முழு bufSize=0;

        IppiSize srcRoiSize = {width, height};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippiDCTGetBufSize ippGetBufSize = inv ? ippiDCTGetBufSizeWrap<IppiDCTInvSpec_32f, ippiDCTInvGetBufSize_32f> : ippiDCTGetBufSizeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdGetBufSize_32f>;

        status = ippStsErr;

        எனில் (ippInitAlloc(&pDCTSpec, srcRoiSize, ippAlgHintNone)>=0 && ippGetBufSize(pDCTSpec, &bufSize)>=0)
        {
            buf.allocate( bufSize );
            pBuffer = (uchar*)buf;

            status = ippDctFun((தசம*)src, static_cast<முழு>(src_step), (தசம*)dst, static_cast<முழு>(dst_step), pDCTSpec, (Ipp8u*)pBuffer);
        }

        எனில் (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END

        திரும்பு status >= 0;
#இல்லையெனில்
        CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(inv); CV_UNUSED(row);
        திரும்பு பொய்;
#endif
    }
}
}
#endif

நாமாகாஶ cv {

வகுப்பு OcvDctImpl CV_FINAL : பொது hal::DCT2D
{
பொது:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    AutoBuffer<uchar> ws;
    தருக்க isRowTransform;
    தருக்க isInverse;
    தருக்க isContinuous;
    முழு start_stage;
    முழு end_stage;
    முழு width;
    முழு height;
    முழு depth;

    வெறுமை init(முழு _width, முழு _height, முழு _depth, முழு flags)
    {
        width = _width;
        height = _height;
        depth = _depth;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        நிலையான DCTFunc dct_tbl[4] =
        {
            dctWrap<தசம, DCT<தசம>>,
            dctWrap<தசம, IDCT<தசம>>,
            dctWrap<இரட்டை, DCT<இரட்டை>>,
            dctWrap<இரட்டை, IDCT<இரட்டை>>
        };
        dct_func = dct_tbl[(முழு)isInverse + (depth == CV_64F)*2];
        opt.isComplex = பொய்;
        opt.isInverse = பொய்;
        opt.scale = 1.;

        எனில் (isRowTransform || height == 1 || (width == 1 && isContinuous))
        {
            start_stage = end_stage = 0;
        }
        இல்லையெனில்
        {
            start_stage = (width == 1);
            end_stage = 1;
        }
    }
    வெறுமை apply(மாறிலி uchar *src, size_t src_step, uchar *dst, size_t dst_step) CV_OVERRIDE
    {
        CV_IPP_RUN(IPP_VERSION_X100 >= 700 && depth == CV_32F, ippi_DCT_32f(src, src_step, dst, dst_step, width, height, isInverse, isRowTransform))

        முழு prev_len = 0;
        முழு elem_size = (depth == CV_32F) ? அளவு(தசம) : அளவு(இரட்டை);
        மாறிலி DFTKernels& kernels = getDFTKernels(depth);

        க்கு(முழு stage = start_stage ; stage <= end_stage; stage++ )
        {
            மாறிலி uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            முழு len, மொத்தம்_2;

            எனில்( stage == 0 )
            {
                len = width;
                மொத்தம்_2 = height;
                எனில்( len == 1 && !isRowTransform )
                {
                    len = height;
                    மொத்தம்_2 = 1;
                }
                sstep0 = src_step;
                dstep0 = dst_step;
                sstep1 = dstep1 = elem_size;
            }
            இல்லையெனில்
            {
                len = height;
                மொத்தம்_2 = width;
                sstep1 = src_step;
                dstep1 = dst_step;
                sstep0 = dstep0 = elem_size;
            }

            opt.n = len;

            எனில்( len != prev_len )
            {
                எனில்( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                எனில்( len > 1 )
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
            இரட்டை nstripes = dftParallelStripes(மொத்தம்_2, len);
            எனில்( len > 1 && மொத்தம்_2 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                auto process_வரிகள் = [&](மாறிலி Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    AutoBuffer<uchar> ws_(plan.ws_bytes);
                    opt_.workspace = ws_.data();
                    க்கு( முழு i = range.start; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, மொத்தம்_2), process_வரிகள், nstripes);
            }
            இல்லையெனில்
            {
                க்கு(unsigned i = 0; i < static_cast<unsigned>(மொத்தம்_2); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_step = dst_step;
        }
    }
};

கட்டமைப்பு ReplacementDCT2D : பொது hal::DCT2D
{
    cvhalDFT *context;
    தருக்க isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(பொய்) {}
    தருக்க init(முழு width, முழு height, முழு depth, முழு flags)
    {
        முழு res = cv_hal_dctInit2D(&context, width, height, depth, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        திரும்பு isInitialized;
    }
    வெறுமை apply(மாறிலி uchar *src_data, size_t src_step, uchar *dst_data, size_t dst_step) CV_OVERRIDE
    {
        எனில் (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_data, src_step, dst_data, dst_step);
        }
    }
    ~ReplacementDCT2D()
    {
        எனில் (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

நாமாகாஶ hal {

Ptr<DCT2D> DCT2D::create(முழு width, முழு height, முழு depth, முழு flags)
{
    {
        ReplacementDCT2D *impl = புதிய ReplacementDCT2D();
        எனில் (impl->init(width, height, depth, flags))
        {
            திரும்பு Ptr<DCT2D>(impl);
        }
        delete impl;
    }
    {
        OcvDctImpl *impl = புதிய OcvDctImpl();
        impl->init(width, height, depth, flags);
        திரும்பு Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

வெறுமை cv::dct( உள்ளீடு_Array _src0, OutputArray _dst, முழு flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    எனில் (src.empty())
    {
        _dst.release();
        திரும்பு;
    }
    முழு type = src.type(), depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.rows, src.cols, type );
    Mat dst = _dst.getMat();

    முழு f = 0;
    எனில் ((flags & DFT_ROWS) != 0)
        f |= CV_HAL_DFT_ROWS;
    எனில் ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    எனில் (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.cols, src.rows, depth, f);
    c->apply(src.data, src.step, dst.data, dst.step);
}


வெறுமை cv::idct( உள்ளீடு_Array src, OutputArray dst, முழு flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

நாமாகாஶ cv
{

நிலையான மாறிலி முழு optimalDFTSizeTab[] = {
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

முழு cv::getOptimalDFTSize( முழு size0 )
{
    முழு a = 0, b = அளவு(optimalDFTSizeTab)/அளவு(optimalDFTSizeTab[0]) - 1;
    எனில்( (unsigned)size0 >= (unsigned)optimalDFTSizeTab[b] )
        திரும்பு -1;

    போது( a < b )
    {
        முழு c = (a + b) >> 1;
        எனில்( size0 <= optimalDFTSizeTab[c] )
            b = c;
        இல்லையெனில்
            a = c+1;
    }

    திரும்பு optimalDFTSizeTab[b];
}
/* End of file. */
