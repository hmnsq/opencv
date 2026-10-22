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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_core.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "dxt.hpp"
#অন্তর্ভাব "dxt.simd.hpp"
#অন্তর্ভাব "dxt.simd_declarations.hpp"
#অন্তর্ভাব <map>

নামাকাশ cv
{

#যদি IPP_VERSION_X100 >= 710
#define USE_IPP_DFT 1
#অন্যথা
#undef USE_IPP_DFT
#endif

#যদি defined USE_IPP_DFT
#যদি IPP_VERSION_X100 >= 202220
#define IPP_DISABLE_DFT32F ((depth == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#অন্যথা
#define IPP_DISABLE_DFT32F মিথ্যা
#endif
#endif

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#ifdef HAVE_OPENCL
// factorization used by the OpenCL radix selection
স্থির পূর্ণসংখ্যা
DFTFactorize( পূর্ণসংখ্যা n, পূর্ণসংখ্যা* factors )
{
    পূর্ণসংখ্যা nf = 0, f, i, j;

    যদি( n <= 5 )
    {
        factors[0] = n;
        ফেরত 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    যদি( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    জন্য( f = 3; n > 1; )
    {
        পূর্ণসংখ্যা d = n/f;
        যদি( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        অন্যথা
        {
            f += 2;
            যদি( f*f > n )
                বিরতি;
        }
    }

    যদি( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    জন্য( i = f; i < (nf+f)/2; i++ )
        CV_SWAP( factors[i], factors[nf-i-1+f], j );

    ফেরত nf;
}
#endif

#ifdef USE_IPP_DFT
স্থির IppStatus ippsDFTFwd_CToC( ধ্রুবক Complex<দশমিক>* src, Complex<দশমিক>* dst,
                             ধ্রুবক শূন্য* spec, uchar* buf)
{
    ফেরত CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (ধ্রুবক Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ধ্রুবক IppsDFTSpec_C_32fc*)spec, buf);
}

স্থির IppStatus ippsDFTFwd_CToC( ধ্রুবক Complex<দ্বিগুণ>* src, Complex<দ্বিগুণ>* dst,
                             ধ্রুবক শূন্য* spec, uchar* buf)
{
    ফেরত CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (ধ্রুবক Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ধ্রুবক IppsDFTSpec_C_64fc*)spec, buf);
}

স্থির IppStatus ippsDFTInv_CToC( ধ্রুবক Complex<দশমিক>* src, Complex<দশমিক>* dst,
                             ধ্রুবক শূন্য* spec, uchar* buf)
{
    ফেরত CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (ধ্রুবক Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ধ্রুবক IppsDFTSpec_C_32fc*)spec, buf);
}

স্থির IppStatus ippsDFTInv_CToC( ধ্রুবক Complex<দ্বিগুণ>* src, Complex<দ্বিগুণ>* dst,
                                  ধ্রুবক শূন্য* spec, uchar* buf)
{
    ফেরত CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (ধ্রুবক Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ধ্রুবক IppsDFTSpec_C_64fc*)spec, buf);
}

স্থির IppStatus ippsDFTFwd_RToPack( ধ্রুবক দশমিক* src, দশমিক* dst,
                                     ধ্রুবক শূন্য* spec, uchar* buf)
{
    ফেরত CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (ধ্রুবক IppsDFTSpec_R_32f*)spec, buf);
}

স্থির IppStatus ippsDFTFwd_RToPack( ধ্রুবক দ্বিগুণ* src, দ্বিগুণ* dst,
                                     ধ্রুবক শূন্য* spec, uchar* buf)
{
    ফেরত CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (ধ্রুবক IppsDFTSpec_R_64f*)spec, buf);
}

স্থির IppStatus ippsDFTInv_PackToR( ধ্রুবক দশমিক* src, দশমিক* dst,
                                     ধ্রুবক শূন্য* spec, uchar* buf)
{
    ফেরত CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_32f, src, dst, (ধ্রুবক IppsDFTSpec_R_32f*)spec, buf);
}

স্থির IppStatus ippsDFTInv_PackToR( ধ্রুবক দ্বিগুণ* src, দ্বিগুণ* dst,
                                     ধ্রুবক শূন্য* spec, uchar* buf)
{
    ফেরত CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_64f, src, dst, (ধ্রুবক IppsDFTSpec_R_64f*)spec, buf);
}
#endif

কাঠামো OcvDftOptions;

প্রকারলেখ শূন্য (*DFTFunc)(ধ্রুবক OcvDftOptions & c, ধ্রুবক শূন্য* src, শূন্য* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
কাঠামো OcvDftOptions {
    দ্বিগুণ scale;
    পূর্ণসংখ্যা n;

    যৌক্তিক isInverse;
    যৌক্তিক isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    যৌক্তিক useIpp;

#ifdef USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#endif

    ধ্রুবক DftPlan* plan;
    ধ্রুবক DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = মিথ্যা;
        isComplex = মিথ্যা;
        useIpp = মিথ্যা;
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

স্থির DFTKernels getDFTKernelsDispatch(পূর্ণসংখ্যা depth)
{
    CV_CPU_DISPATCH(getDFTKernels_, (depth), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
স্থির ধ্রুবক DFTKernels& getDFTKernels(পূর্ণসংখ্যা depth)
{
    স্থির ধ্রুবক DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    স্থির ধ্রুবক DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    ফেরত depth == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
কাঠামো DftTwiddleGen
{
    পূর্ণসংখ্যা M, logB, maskB;
    Auto_বাফার<দ্বিগুণ, 1024> buf;
    দ্বিগুণ *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    স্থির শূন্য fillTable(দ্বিগুণ* re, দ্বিগুণ* im, পূর্ণসংখ্যা গুনতি_2, দ্বিগুণ wr, দ্বিগুণ wi, দ্বিগুণ stepAngle)
    {
        জন্য (পূর্ণসংখ্যা i0 = 0; i0 < গুনতি_2; i0 += 16)
        {
            দ্বিগুণ t0 = stepAngle*i0;
            দ্বিগুণ cr = i0 == 0 ? 1 : cos(t0), ci = i0 == 0 ? 0 : sin(t0);
            পূর্ণসংখ্যা i1 = std::min(i0 + 16, গুনতি_2);
            জন্য (পূর্ণসংখ্যা i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                দ্বিগুণ t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    শূন্য init(পূর্ণসংখ্যা _M)
    {
        M = _M;
        logB = 0;
        যখন ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        পূর্ণসংখ্যা B = 1 << logB;
        maskB = B - 1;
        পূর্ণসংখ্যা nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.তথ্য(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        দ্বিগুণ a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fillTable(lo_re, lo_im, B, cos(a), sin(a), a);
        fillTable(hi_re, hi_im, nhi, cos(a*B), sin(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    শূন্য get(পূর্ণসংখ্যা m, দ্বিগুণ& re, দ্বিগুণ& im) ধ্রুবক
    {
        CV_DbgAssert(m >= 0);
        যদি (m >= M)
            m %= M;
        পূর্ণসংখ্যা a = m >> logB, b = m & maskB;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

template<typename T>
শূন্য DftPlan::fillTables(size_t ntw, যৌক্তিক need_rtw, যৌক্তিক need_dct)
{
    DftPlan& p = *এই;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    পূর্ণসংখ্যা M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*আকার(T) + আকার(দ্বিগুণ) - 1)/আকার(দ্বিগুণ) + 1);
    T* cur = (T*)p.tw.তথ্য();
    পূর্ণসংখ্যা maxL = 1;
    জন্য (পূর্ণসংখ্যা s = 0; s < p.nstages; s++)
        যদি (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].span);
    Auto_বাফার<দ্বিগুণ, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    দ্বিগুণ* twd = twd_buf.তথ্য();
    জন্য (পূর্ণসংখ্যা s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        পূর্ণসংখ্যা r = st.radix, L = st.span, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        পূর্ণসংখ্যা step = M/(r*L);
        যদি (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr পূর্ণসংখ্যা K = 8;
            দ্বিগুণ cr[K], ci[K], wr, wi;
            gen.get(K*step, wr, wi);
            দ্বিগুণ* t1r = twd; দ্বিগুণ* t1i = twd + L;      // leg 1 kept in double for the products
            জন্য (পূর্ণসংখ্যা j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*step, cr[k], ci[k]);
                পূর্ণসংখ্যা j1 = std::min(j0 + 16*K, L);
                জন্য (পূর্ণসংখ্যা j = j0; j < j1; j += K)
                    জন্য (পূর্ণসংখ্যা k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        দ্বিগুণ t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            জন্য (পূর্ণসংখ্যা j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            জন্য (পূর্ণসংখ্যা q = 2; q < r; q++)
            {
                ধ্রুবক T* pr = twr + (q-2)*ts; ধ্রুবক T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                জন্য (পূর্ণসংখ্যা j = 0; j < L; j++)
                {
                    দ্বিগুণ ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        অন্যথা
        {
            // large odd radix: exact-ish generator lookup per entry
            জন্য (পূর্ণসংখ্যা q = 1; q < r; q++)
                জন্য (পূর্ণসংখ্যা j = 0; j < L; j++)
                {
                    দ্বিগুণ re, im;
                    gen.get(q*j*step, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        যদি (r >= 7)
        {
            পূর্ণসংখ্যা h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            জন্য (পূর্ণসংখ্যা pp = 1; pp <= h; pp++)
                জন্য (পূর্ণসংখ্যা k = 1; k <= h; k++)
                {
                    দ্বিগুণ t = 2*CV_PI*((দ্বিগুণ)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)cos(t);
                    sn[(pp-1)*h + (k-1)] = (T)sin(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    যদি (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        পূর্ণসংখ্যা step = M/n;
        জন্য (পূর্ণসংখ্যা k = 0; k <= nc; k++)
        {
            দ্বিগুণ re, im;
            gen.get(k*step, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    যদি (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        দ্বিগুণ scale = (p.kind == DFT_KIND_DCT ? 2 : 1)*std::sqrt(1./(2*n));
        পূর্ণসংখ্যা step = M/(4*n);
        জন্য (পূর্ণসংখ্যা k = 0; k <= nc; k++)
        {
            দ্বিগুণ re, im;
            gen.get(k*step, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
শূন্য DftPlan::build(পূর্ণসংখ্যা _kind, পূর্ণসংখ্যা _n, পূর্ণসংখ্যা _depth, পূর্ণসংখ্যা _vl)
{
    DftPlan& p = *এই;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(_depth == CV_32F || _depth == CV_64F);
    যৌক্তিক real_kind = _kind != DFT_KIND_C2C;
    যৌক্তিক dct_kind = _kind == DFT_KIND_DCT || _kind == DFT_KIND_IDCT;
    যদি (dct_kind)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.kind = _kind; p.n = _n; p.depth = _depth; p.vl = _vl;
    p.nc = real_kind && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = depth == CV_32F ? আকার(দশমিক) : আকার(দ্বিগুণ);
    p.real_প্রবেশ = kind == DFT_KIND_R2C && (n & 1) != 0;
    p.need_tmp = kind == DFT_KIND_C2R || kind == DFT_KIND_DCT || kind == DFT_KIND_IDCT;
    p.pingpong = মিথ্যা;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    পূর্ণসংখ্যা k2 = 0, rest = nc;
    যখন ((rest & 1) == 0) { rest >>= 1; k2++; }
    পূর্ণসংখ্যা r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.first_radix = r0;
    পূর্ণসংখ্যা radices[DFT_MAX_STAGES], nst = 0;
    জন্য (পূর্ণসংখ্যা m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    যখন (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    যখন (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    জন্য (পূর্ণসংখ্যা f = 7; rest > 1; f += 2)
    {
        যদি (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            বিরতি;
        }
        যখন (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    পূর্ণসংখ্যা L = r0, max_h = 0;
    size_t ntw = 0;
    জন্য (পূর্ণসংখ্যা s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        পূর্ণসংখ্যা r = radices[s];
        st.radix = r; st.span = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        যদি (vl > 1 && L >= vl)
        {
            st.mode = DFT_STAGE_FULL;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            যদি (L % vl != 0)
                p.pingpong = সত্য;   // back-off clamp => overlapping stores => must be out-of-place
        }
        অন্যথা
        {
            st.mode = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        যদি (r >= 7)
        {
            পূর্ণসংখ্যা h = (r - 1)/2;
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
    পূর্ণসংখ্যা seq[DFT_MAX_STAGES + 1], nseq = 0;
    যদি (r0 > 1) seq[nseq++] = r0;
    জন্য (পূর্ণসংখ্যা s = 0; s < nst; s++) seq[nseq++] = radices[s];
    পূর্ণসংখ্যা digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        পূর্ণসংখ্যা Ls = 1;
        জন্য (পূর্ণসংখ্যা s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    যদি (r0 == 1)
    {
        p.itab.allocate(nc*2);
        পূর্ণসংখ্যা* it_all = p.itab.তথ্য();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        পূর্ণসংখ্যা n_in = std::min(nseq, 2);
        পূর্ণসংখ্যা r_0 = n_in > 0 ? seq[0] : 1, w_0 = n_in > 0 ? weights[0] : 0;
        পূর্ণসংখ্যা r_1 = n_in > 1 ? seq[1] : 1, w_1 = n_in > 1 ? weights[1] : 0;
        জন্য (পূর্ণসংখ্যা pos = 0, rev_hi = 0; pos < nc; )
        {
            জন্য (পূর্ণসংখ্যা d1 = 0; d1 < r_1; d1++)
            {
                পূর্ণসংখ্যা rev = rev_hi + d1*w_1;
                পূর্ণসংখ্যা* it = it_all + pos*2;
                যদি (p.real_প্রবেশ)
                {
                    জন্য (পূর্ণসংখ্যা d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                অন্যথা
                {
                    জন্য (পূর্ণসংখ্যা d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            জন্য (পূর্ণসংখ্যা s = n_in; s < nseq; s++)
            {
                rev_hi += weights[s];
                যদি (++digits[s] < seq[s])
                    বিরতি;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    অন্যথা
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        পূর্ণসংখ্যা M = nc/r0;
        p.itab.allocate(M);
        পূর্ণসংখ্যা* gtab = p.itab.তথ্য();
        পূর্ণসংখ্যা gw[DFT_MAX_STAGES + 1];
        {
            পূর্ণসংখ্যা Ls = 1;
            জন্য (পূর্ণসংখ্যা s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        পূর্ণসংখ্যা sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        পূর্ণসংখ্যা r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        পূর্ণসংখ্যা r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        জন্য (পূর্ণসংখ্যা base = 0, g_hi = 0; base < M; )
        {
            জন্য (পূর্ণসংখ্যা db = 0; db < r_b; db++)
            {
                পূর্ণসংখ্যা g = (g_hi + db*g_b)*r0;
                জন্য (পূর্ণসংখ্যা da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[base++] = g;
            }
            জন্য (পূর্ণসংখ্যা s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                যদি (++digits[s] < seq[s])
                    বিরতি;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    যৌক্তিক need_rtw = real_kind && n % 2 == 0;
    যদি (need_rtw) ntw += 2*(size_t)(nc + 1);
    যদি (dct_kind) ntw += 2*(size_t)(nc + 1);
    যদি (depth == CV_32F)
        fillTables<দশমিক>(ntw, need_rtw, dct_kind);
    অন্যথা
        fillTables<দ্বিগুণ>(ntw, need_rtw, dct_kind);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_bytes = 2*(size_t)nc*esz;
    যৌক্তিক two_pairs = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_pairs ? pair_bytes : 0;
    p.scratch_ofs = pair_bytes*(two_pairs ? 2 : 1);
    size_t scratch_bytes = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_bytes = p.scratch_ofs + scratch_bytes + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
স্থির শূন্য runDft(ধ্রুবক OcvDftOptions& c, ধ্রুবক শূন্য* src, size_t sstep, শূন্য* dst, size_t dstep,
                   যৌক্তিক complex_io)
{
    ধ্রুবক DftPlan& p = *c.plan;
    ধ্রুবক DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.depth == CV_32F ? আকার(দশমিক) : আকার(দ্বিগুণ);
    uchar* base = alignPtr(c.workspace, 64);
    uchar* re0 = base;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = base + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = base + p.scratch_ofs;

    ধ্রুবক শূন্য* asrc = src;
    size_t astep = sstep;
    যৌক্তিক conj = মিথ্যা;
    যদি (p.kind == DFT_KIND_C2R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = সত্য;
    }
    অন্যথা যদি (p.kind == DFT_KIND_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = সত্য;
    }
    অন্যথা যদি (p.kind == DFT_KIND_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    অন্যথা যদি (p.kind == DFT_KIND_C2C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.first_radix == 1);

    DftPreprocFunc pre = p.first_radix == 1 ? k.preprocRadix0 : p.first_radix == 2 ? k.preprocRadix2 :
                         p.first_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    জন্য (পূর্ণসংখ্যা i = 0; i < p.nstages; i++)
    {
        ধ্রুবক DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        যদি (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        অন্যথা
            fn(st, sre, sim, sre, sim, scratch);
    }

    দ্বিগুণ scale = c.scale;
    নির্বাচন (p.kind)
    {
    ক্ষেত্র DFT_KIND_C2C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        বিরতি;
    ক্ষেত্র DFT_KIND_R2C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        বিরতি;
    ক্ষেত্র DFT_KIND_C2R:
        যদি (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        অন্যথা
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        বিরতি;
    ক্ষেত্র DFT_KIND_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        বিরতি;
    পূর্বনির্ধারিত:
        CV_Assert(p.kind == DFT_KIND_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
template<typename T> স্থির শূন্য
DFT(ধ্রুবক OcvDftOptions & c, ধ্রুবক Complex<T>* src, Complex<T>* dst)
{
    যদি( c.useIpp )
    {
#ifdef USE_IPP_DFT
        যদি( !c.isInverse )
        {
            যদি (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                ফেরত;
            }
        }
        অন্যথা
        {
            যদি (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                ফেরত;
            }
        }
        setIppErrorStatus();
#endif
    }

    runDft(c, src, 1, dst, 1, মিথ্যা);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
template<typename T> স্থির শূন্য
RealDFT(ধ্রুবক OcvDftOptions & c, ধ্রুবক T* src, T* dst)
{
    পূর্ণসংখ্যা complex_output = c.isComplex;

    যদি( c.useIpp )
    {
#ifdef USE_IPP_DFT
        পূর্ণসংখ্যা n = c.n;
        T* ipp_dst = dst + complex_output;
        যদি (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            যদি( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                যদি( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_ADD(CV_IMPL_IPP);
            ফেরত;
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
template<typename T> স্থির শূন্য
CCSIDFT(ধ্রুবক OcvDftOptions & c, ধ্রুবক T* src, T* dst)
{
    পূর্ণসংখ্যা complex_প্রবেশ = c.isComplex;

    যদি( complex_প্রবেশ )
        CV_Assert( src != dst );
    যদি( c.useIpp )
    {
#ifdef USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        ধ্রুবক T* ipp_src = src;
        T save_s1 = 0;
        যদি( complex_প্রবেশ )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        IppStatus status = ippsDFTInv_PackToR( ipp_src, dst, c.ipp_spec, c.ipp_work );
        যদি( complex_প্রবেশ )
            ((T*)src)[1] = save_s1;
        যদি( status >= 0 )
        {
            CV_IMPL_ADD(CV_IMPL_IPP);
            ফেরত;
        }
        setIppErrorStatus();
#endif
    }
    runDft(c, src, 1, dst, 1, complex_প্রবেশ != 0);
}

স্থির শূন্য
CopyColumn( ধ্রুবক uchar* _src, size_t src_step,
            uchar* _dst, size_t dst_step,
            পূর্ণসংখ্যা len, size_t elem_আকার_2 )
{
    পূর্ণসংখ্যা i, t0, t1;
    ধ্রুবক পূর্ণসংখ্যা* src = (ধ্রুবক পূর্ণসংখ্যা*)_src;
    পূর্ণসংখ্যা* dst = (পূর্ণসংখ্যা*)_dst;
    src_step /= আকার(src[0]);
    dst_step /= আকার(dst[0]);

    যদি( elem_আকার_2 == আকার(পূর্ণসংখ্যা) )
    {
        জন্য( i = 0; i < len; i++, src += src_step, dst += dst_step )
            dst[0] = src[0];
    }
    অন্যথা যদি( elem_আকার_2 == আকার(পূর্ণসংখ্যা)*2 )
    {
        জন্য( i = 0; i < len; i++, src += src_step, dst += dst_step )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    অন্যথা যদি( elem_আকার_2 == আকার(পূর্ণসংখ্যা)*4 )
    {
        জন্য( i = 0; i < len; i++, src += src_step, dst += dst_step )
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
template<typename T> স্থির অন্তঃরেখ শূন্য
copyFromColumns_( ধ্রুবক uchar* src, size_t src_step, T* dst, পূর্ণসংখ্যা ncols, পূর্ণসংখ্যা len )
{
    জন্য( পূর্ণসংখ্যা i = 0; i < len; i++, src += src_step )
    {
        ধ্রুবক T* s = (ধ্রুবক T*)src;
        জন্য( পূর্ণসংখ্যা k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

স্থির শূন্য
CopyFromColumns( ধ্রুবক uchar* src, size_t src_step, uchar* dst, পূর্ণসংখ্যা ncols, পূর্ণসংখ্যা len, size_t elem_আকার_2 )
{
    যদি( elem_আকার_2 == আকার(পূর্ণসংখ্যা) )
        copyFromColumns_(src, src_step, (পূর্ণসংখ্যা*)dst, ncols, len);
    অন্যথা যদি( elem_আকার_2 == আকার(int64) )
        copyFromColumns_(src, src_step, (int64*)dst, ncols, len);
    অন্যথা
    {
        CV_Assert( elem_আকার_2 == আকার(int64)*2 );
        copyFromColumns_(src, src_step, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
template<typename T> স্থির অন্তঃরেখ শূন্য
copyToColumns_( ধ্রুবক T* src, uchar* dst, size_t dst_step, পূর্ণসংখ্যা ncols, পূর্ণসংখ্যা len )
{
    জন্য( পূর্ণসংখ্যা i = 0; i < len; i++, dst += dst_step )
    {
        T* d = (T*)dst;
        জন্য( পূর্ণসংখ্যা k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

স্থির শূন্য
CopyToColumns( ধ্রুবক uchar* src, uchar* dst, size_t dst_step, পূর্ণসংখ্যা ncols, পূর্ণসংখ্যা len, size_t elem_আকার_2 )
{
    যদি( elem_আকার_2 == আকার(পূর্ণসংখ্যা) )
        copyToColumns_((ধ্রুবক পূর্ণসংখ্যা*)src, dst, dst_step, ncols, len);
    অন্যথা যদি( elem_আকার_2 == আকার(int64) )
        copyToColumns_((ধ্রুবক int64*)src, dst, dst_step, ncols, len);
    অন্যথা
    {
        CV_Assert( elem_আকার_2 == আকার(int64)*2 );
        copyToColumns_((ধ্রুবক Vec<int64, 2>*)src, dst, dst_step, ncols, len);
    }
}

স্থির শূন্য
ExpandCCS( uchar* _ptr, পূর্ণসংখ্যা n, পূর্ণসংখ্যা elem_আকার_2 )
{
    পূর্ণসংখ্যা i;
    যদি( elem_আকার_2 == (পূর্ণসংখ্যা)আকার(দশমিক) )
    {
        দশমিক* p = (দশমিক*)_ptr;
        জন্য( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        যদি( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        জন্য( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    অন্যথা
    {
        দ্বিগুণ* p = (দ্বিগুণ*)_ptr;
        জন্য( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        যদি( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        জন্য( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

template<typename T, শূন্য (*fn)(ধ্রুবক OcvDftOptions&, ধ্রুবক T*, T*)>
স্থির শূন্য dftWrap(ধ্রুবক OcvDftOptions & c, ধ্রুবক শূন্য* src, শূন্য* dst)
{
    fn(c, (ধ্রুবক T*)src, (T*)dst);
}

}

#ifdef USE_IPP_DFT
প্রকারলেখ IppStatus (CV_STDCALL* Ipp_DFTGet_আকার_Func)(পূর্ণসংখ্যা, পূর্ণসংখ্যা, IppHintAlgorithm, পূর্ণসংখ্যা*, পূর্ণসংখ্যা*, পূর্ণসংখ্যা*);
প্রকারলেখ IppStatus (CV_STDCALL* IppDFTInitFunc)(পূর্ণসংখ্যা, পূর্ণসংখ্যা, IppHintAlgorithm, শূন্য*, uchar*);

template<typename SpecType, IppStatus (CV_STDCALL *init_fn)(পূর্ণসংখ্যা, পূর্ণসংখ্যা, IppHintAlgorithm, SpecType*, Ipp8u*)>
স্থির IppStatus CV_STDCALL ippDFTInitWrap(পূর্ণসংখ্যা n, পূর্ণসংখ্যা flags, IppHintAlgorithm hint, শূন্য* spec, Ipp8u* initbuf)
{
    ফেরত init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#endif

নামাকাশ cv
{
#যদি defined USE_IPP_DFT

প্রকারলেখ IppStatus (CV_STDCALL* ippiDFT_C_Func)(ধ্রুবক Ipp32fc*, পূর্ণসংখ্যা, Ipp32fc*, পূর্ণসংখ্যা, ধ্রুবক IppiDFTSpec_C_32fc*, Ipp8u*);
প্রকারলেখ IppStatus (CV_STDCALL* ippiDFT_R_Func)(ধ্রুবক Ipp32f* , পূর্ণসংখ্যা, Ipp32f* , পূর্ণসংখ্যা, ধ্রুবক IppiDFTSpec_R_32f* , Ipp8u*);

template <typename Dft>
শ্রেণী Dft_C_IPPLoop_Invoker : সর্বজনীন ParallelLoopBody
{
সর্বজনীন:

    Dft_C_IPPLoop_Invoker(ধ্রুবক uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, পূর্ণসংখ্যা _width,
                          ধ্রুবক Dft& _ippidft, পূর্ণসংখ্যা _norm_flag, যৌক্তিক *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = সত্য;
    }

    আভাসী শূন্য operator()(ধ্রুবক Range& range) ধ্রুবক CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* p_বাফার = 0;
        Ipp8u* pMemInit= 0;
        পূর্ণসংখ্যা আকার_বাফার=0;
        পূর্ণসংখ্যা আকার_Spec=0;
        পূর্ণসংখ্যা আকার_Init=0;

        Ippi_আকার src_Roi_আকার = {width, 1};

        status = ippi_DFTGet_আকার_C_32fc(src_Roi_আকার, norm_flag, ippAlgHintNone, &আকার_Spec, &আকার_Init, &আকার_বাফার );
        যদি ( status < 0 )
        {
            *ok = মিথ্যা;
            ফেরত;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( আকার_Spec );

        যদি ( আকার_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( আকার_Init );

        যদি ( আকার_বাফার > 0 )
            p_বাফার = (Ipp8u*)CV_IPP_MALLOC( আকার_বাফার );

        status = ippiDFTInit_C_32fc( src_Roi_আকার, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        যদি ( আকার_Init > 0 )
            ippFree( pMemInit );

        যদি ( status < 0 )
        {
            ippFree( pDFTSpec );
            যদি ( আকার_বাফার > 0 )
                ippFree( p_বাফার );
            *ok = মিথ্যা;
            ফেরত;
        }

        জন্য( পূর্ণসংখ্যা i = range.start; i < range.end; ++i)
            যদি(!ippidft((Ipp32fc*)(src + src_step * i), src_step, (Ipp32fc*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)p_বাফার))
            {
                *ok = মিথ্যা;
            }

        যদি ( আকার_বাফার > 0 )
            ippFree( p_বাফার );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

ব্যক্তিগত:
    ধ্রুবক uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    পূর্ণসংখ্যা width;
    ধ্রুবক Dft& ippidft;
    পূর্ণসংখ্যা norm_flag;
    যৌক্তিক *ok;

    ধ্রুবক Dft_C_IPPLoop_Invoker& operator= (ধ্রুবক Dft_C_IPPLoop_Invoker&);
};

template <typename Dft>
শ্রেণী Dft_R_IPPLoop_Invoker : সর্বজনীন ParallelLoopBody
{
সর্বজনীন:

    Dft_R_IPPLoop_Invoker(ধ্রুবক uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, পূর্ণসংখ্যা _width,
                          ধ্রুবক Dft& _ippidft, পূর্ণসংখ্যা _norm_flag, যৌক্তিক *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = সত্য;
    }

    আভাসী শূন্য operator()(ধ্রুবক Range& range) ধ্রুবক CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* p_বাফার = 0;
        Ipp8u* pMemInit= 0;
        পূর্ণসংখ্যা আকার_বাফার=0;
        পূর্ণসংখ্যা আকার_Spec=0;
        পূর্ণসংখ্যা আকার_Init=0;

        Ippi_আকার src_Roi_আকার = {width, 1};

        status = ippi_DFTGet_আকার_R_32f(src_Roi_আকার, norm_flag, ippAlgHintNone, &আকার_Spec, &আকার_Init, &আকার_বাফার );
        যদি ( status < 0 )
        {
            *ok = মিথ্যা;
            ফেরত;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( আকার_Spec );

        যদি ( আকার_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( আকার_Init );

        যদি ( আকার_বাফার > 0 )
            p_বাফার = (Ipp8u*)CV_IPP_MALLOC( আকার_বাফার );

        status = ippiDFTInit_R_32f( src_Roi_আকার, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        যদি ( আকার_Init > 0 )
            ippFree( pMemInit );

        যদি ( status < 0 )
        {
            ippFree( pDFTSpec );
            যদি ( আকার_বাফার > 0 )
                ippFree( p_বাফার );
            *ok = মিথ্যা;
            ফেরত;
        }

        জন্য( পূর্ণসংখ্যা i = range.start; i < range.end; ++i)
            যদি(!ippidft((দশমিক*)(src + src_step * i), src_step, (দশমিক*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)p_বাফার))
            {
                *ok = মিথ্যা;
            }

        যদি ( আকার_বাফার > 0 )
            ippFree( p_বাফার );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

ব্যক্তিগত:
    ধ্রুবক uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    পূর্ণসংখ্যা width;
    ধ্রুবক Dft& ippidft;
    পূর্ণসংখ্যা norm_flag;
    যৌক্তিক *ok;

    ধ্রুবক Dft_R_IPPLoop_Invoker& operator= (ধ্রুবক Dft_R_IPPLoop_Invoker&);
};

template <typename Dft>
যৌক্তিক Dft_C_IPPLoop(ধ্রুবক uchar * src, size_t src_step, uchar * dst, size_t dst_step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, ধ্রুবক Dft& ippidft, পূর্ণসংখ্যা norm_flag)
{
    যৌক্তিক ok;
    parallel_for_(Range(0, height), Dft_C_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(দ্বিগুণ)(1<<16) );
    ফেরত ok;
}

template <typename Dft>
যৌক্তিক Dft_R_IPPLoop(ধ্রুবক uchar * src, size_t src_step, uchar * dst, size_t dst_step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, ধ্রুবক Dft& ippidft, পূর্ণসংখ্যা norm_flag)
{
    যৌক্তিক ok;
    parallel_for_(Range(0, height), Dft_R_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(দ্বিগুণ)(1<<16) );
    ফেরত ok;
}

কাঠামো IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    যৌক্তিক operator()(ধ্রুবক Ipp32fc* src, size_t srcStep, Ipp32fc* dst, size_t dstStep, ধ্রুবক IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* p_বাফার) ধ্রুবক
    {
        ফেরত ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<পূর্ণসংখ্যা>(srcStep), dst, static_cast<পূর্ণসংখ্যা>(dstStep), pDFTSpec, p_বাফার) >= 0 : মিথ্যা;
    }
ব্যক্তিগত:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

কাঠামো IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippiDFT_PackToR_32f_C1R(_func){}

    যৌক্তিক operator()(ধ্রুবক Ipp32f* src, size_t srcStep, Ipp32f* dst, size_t dstStep, ধ্রুবক IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* p_বাফার) ধ্রুবক
    {
        ফেরত ippiDFT_PackToR_32f_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_PackToR_32f_C1R, src, static_cast<পূর্ণসংখ্যা>(srcStep), dst, static_cast<পূর্ণসংখ্যা>(dstStep), pDFTSpec, p_বাফার) >= 0 : মিথ্যা;
    }
ব্যক্তিগত:
    ippiDFT_R_Func ippiDFT_PackToR_32f_C1R;
};

স্থির যৌক্তিক ippi_DFT_C_32F(ধ্রুবক uchar * src, size_t src_step, uchar * dst, size_t dst_step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, যৌক্তিক inv, পূর্ণসংখ্যা norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* p_বাফার = 0;
    Ipp8u* pMemInit= 0;
    পূর্ণসংখ্যা আকার_বাফার=0;
    পূর্ণসংখ্যা আকার_Spec=0;
    পূর্ণসংখ্যা আকার_Init=0;

    Ippi_আকার src_Roi_আকার = {width, height};

    status = ippi_DFTGet_আকার_C_32fc(src_Roi_আকার, norm_flag, ippAlgHintNone, &আকার_Spec, &আকার_Init, &আকার_বাফার );
    যদি ( status < 0 )
        ফেরত মিথ্যা;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( আকার_Spec );

    যদি ( আকার_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( আকার_Init );

    যদি ( আকার_বাফার > 0 )
        p_বাফার = (Ipp8u*)CV_IPP_MALLOC( আকার_বাফার );

    status = ippiDFTInit_C_32fc( src_Roi_আকার, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    যদি ( আকার_Init > 0 )
        ippFree( pMemInit );

    যদি ( status < 0 )
    {
        ippFree( pDFTSpec );
        যদি ( আকার_বাফার > 0 )
            ippFree( p_বাফার );
        ফেরত মিথ্যা;
    }

    যদি (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<পূর্ণসংখ্যা>(src_step), (Ipp32fc*)dst, static_cast<পূর্ণসংখ্যা>(dst_step), pDFTSpec, p_বাফার);
    অন্যথা
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<পূর্ণসংখ্যা>(src_step), (Ipp32fc*)dst, static_cast<পূর্ণসংখ্যা>(dst_step), pDFTSpec, p_বাফার);

    যদি ( আকার_বাফার > 0 )
        ippFree( p_বাফার );

    ippFree( pDFTSpec );

    যদি(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        ফেরত সত্য;
    }
    ফেরত মিথ্যা;
}

স্থির যৌক্তিক ippi_DFT_R_32F(ধ্রুবক uchar * src, size_t src_step, uchar * dst, size_t dst_step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, যৌক্তিক inv, পূর্ণসংখ্যা norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* p_বাফার = 0;
    Ipp8u* pMemInit= 0;
    পূর্ণসংখ্যা আকার_বাফার=0;
    পূর্ণসংখ্যা আকার_Spec=0;
    পূর্ণসংখ্যা আকার_Init=0;

    Ippi_আকার src_Roi_আকার = {width, height};

    status = ippi_DFTGet_আকার_R_32f(src_Roi_আকার, norm_flag, ippAlgHintNone, &আকার_Spec, &আকার_Init, &আকার_বাফার );
    যদি ( status < 0 )
        ফেরত মিথ্যা;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( আকার_Spec );

    যদি ( আকার_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( আকার_Init );

    যদি ( আকার_বাফার > 0 )
        p_বাফার = (Ipp8u*)CV_IPP_MALLOC( আকার_বাফার );

    status = ippiDFTInit_R_32f( src_Roi_আকার, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    যদি ( আকার_Init > 0 )
        ippFree( pMemInit );

    যদি ( status < 0 )
    {
        ippFree( pDFTSpec );
        যদি ( আকার_বাফার > 0 )
            ippFree( p_বাফার );
        ফেরত মিথ্যা;
    }

    যদি (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (দশমিক*)src, static_cast<পূর্ণসংখ্যা>(src_step), (দশমিক*)dst, static_cast<পূর্ণসংখ্যা>(dst_step), pDFTSpec, p_বাফার);
    অন্যথা
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_PackToR_32f_C1R, (দশমিক*)src, static_cast<পূর্ণসংখ্যা>(src_step), (দশমিক*)dst, static_cast<পূর্ণসংখ্যা>(dst_step), pDFTSpec, p_বাফার);

    যদি ( আকার_বাফার > 0 )
        ippFree( p_বাফার );

    ippFree( pDFTSpec );

    যদি(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        ফেরত সত্য;
    }
    ফেরত মিথ্যা;
}

#endif
}

#ifdef HAVE_OPENCL

নামাকাশ cv
{

গণনা FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

কাঠামো OCL_FftPlan
{
ব্যক্তিগত:
    UMat twiddles;
    String buildOptions;
    পূর্ণসংখ্যা thread_গুনতি;
    পূর্ণসংখ্যা dft_আকার;
    পূর্ণসংখ্যা dft_depth;
    যৌক্তিক status;

সর্বজনীন:
    OCL_FftPlan(পূর্ণসংখ্যা আকার_5, পূর্ণসংখ্যা _depth) : dft_আকার(আকার_5), dft_depth(_depth), status(সত্য)
    {
        CV_Assert( dft_depth == CV_32F || dft_depth == CV_64F );

        পূর্ণসংখ্যা min_radix;
        std::vector<পূর্ণসংখ্যা> radixes, blocks;
        ocl_getRadixes(dft_আকার, radixes, blocks, min_radix);
        thread_গুনতি = dft_আকার / min_radix;

        যদি (thread_গুনতি > (পূর্ণসংখ্যা) ocl::Device::getDefault().max_Work_Group_আকার())
        {
            status = মিথ্যা;
            ফেরত;
        }

        // generate string with radix calls
        String radix_processing;
        পূর্ণসংখ্যা n = 1, twiddle_আকার = 0;
        জন্য (size_t i=0; i<radixes.size(); i++)
        {
            পূর্ণসংখ্যা radix = radixes[i], block = blocks[i];
            যদি (block > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, block, twiddle_আকার, n, dft_আকার/radix);
            অন্যথা
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_আকার, n, dft_আকার/radix);
            twiddle_আকার += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_আকার, CV_MAKE_TYPE(dft_depth, 2));
        যদি (dft_depth == CV_32F)
            fillRadixTable<দশমিক>(twiddles, radixes);
        অন্যথা
            fillRadixTable<দ্বিগুণ>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_আকার, min_radix, ocl::typeToStr(dft_depth), ocl::typeToStr(CV_MAKE_TYPE(dft_depth, 2)),
                              dft_depth == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    যৌক্তিক enqueueTransform(প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা অঙ্ক_dfts, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা fftType, যৌক্তিক rows = সত্য) ধ্রুবক
    {
        যদি (!status)
            ফেরত মিথ্যা;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        যৌক্তিক is1d = (flags & DFT_ROWS) != 0 || অঙ্ক_dfts == 1;
        যৌক্তিক inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        যদি (rows)
        {
            globalsize[0] = thread_গুনতি; globalsize[1] = src.rows;
            localsize[0] = thread_গুনতি; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            যদি ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        অন্যথা
        {
            globalsize[0] = অঙ্ক_dfts; globalsize[1] = thread_গুনতি;
            localsize[0] = 1; localsize[1] = thread_গুনতি;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            যদি (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        যদি (!inv)
        {
            যদি ((is1d && src.channels() == 1) || (rows && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        অন্যথা
        {
            যদি (rows && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            যদি (dst.cols % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        যদি (k.empty())
            ফেরত মিথ্যা;

        k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::Read_Only_No_আকার(twiddles), thread_গুনতি, অঙ্ক_dfts);
        ফেরত k.run(2, globalsize, localsize, মিথ্যা);
    }

ব্যক্তিগত:
    স্থির শূন্য ocl_getRadixes(পূর্ণসংখ্যা cols, std::vector<পূর্ণসংখ্যা>& radixes, std::vector<পূর্ণসংখ্যা>& blocks, পূর্ণসংখ্যা& min_radix)
    {
        পূর্ণসংখ্যা factors[34];
        পূর্ণসংখ্যা nf = DFTFactorize(cols, factors);

        পূর্ণসংখ্যা n = 1;
        পূর্ণসংখ্যা factor_সূচক = 0;
        min_radix = INT_MAX;

        // 2^n transforms
        যদি ((factors[factor_সূচক] & 1) == 0)
        {
            জন্য( ; n < factors[factor_সূচক];)
            {
                পূর্ণসংখ্যা radix = 2, block = 1;
                যদি (8*n <= factors[0])
                    radix = 8;
                অন্যথা যদি (4*n <= factors[0])
                {
                    radix = 4;
                    যদি (cols % 12 == 0)
                        block = 3;
                    অন্যথা যদি (cols % 8 == 0)
                        block = 2;
                }
                অন্যথা
                {
                    যদি (cols % 10 == 0)
                        block = 5;
                    অন্যথা যদি (cols % 8 == 0)
                        block = 4;
                    অন্যথা যদি (cols % 6 == 0)
                        block = 3;
                    অন্যথা যদি (cols % 4 == 0)
                        block = 2;
                }

                radixes.push_back(radix);
                blocks.push_back(block);
                min_radix = min(min_radix, block*radix);
                n *= radix;
            }
            factor_সূচক++;
        }

        // all the other transforms
        জন্য( ; factor_সূচক < nf; factor_সূচক++)
        {
            পূর্ণসংখ্যা radix = factors[factor_সূচক], block = 1;
            যদি (radix == 3)
            {
                যদি (cols % 12 == 0)
                    block = 4;
                অন্যথা যদি (cols % 9 == 0)
                    block = 3;
                অন্যথা যদি (cols % 6 == 0)
                    block = 2;
            }
            অন্যথা যদি (radix == 5)
            {
                যদি (cols % 10 == 0)
                    block = 2;
            }
            radixes.push_back(radix);
            blocks.push_back(block);
            min_radix = min(min_radix, block*radix);
        }
    }

    template <typename T>
    স্থির শূন্য fillRadixTable(UMat twiddles, ধ্রুবক std::vector<পূর্ণসংখ্যা>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        পূর্ণসংখ্যা ptr_সূচক = 0;

        পূর্ণসংখ্যা n = 1;
        জন্য (size_t i=0; i<radixes.size(); i++)
        {
            পূর্ণসংখ্যা radix = radixes[i];
            n *= radix;

            জন্য (পূর্ণসংখ্যা j=1; j<radix; j++)
            {
                দ্বিগুণ theta = -CV_2PI*j/n;

                জন্য (পূর্ণসংখ্যা k=0; k<(n/radix); k++)
                {
                    ptr[ptr_সূচক++] = (T) cos(k*theta);
                    ptr[ptr_সূচক++] = (T) sin(k*theta);
                }
            }
        }
    }
};

শ্রেণী OCL_FftPlanCache
{
সর্বজনীন:
    স্থির OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, নতুন OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(পূর্ণসংখ্যা dft_আকার, পূর্ণসংখ্যা depth)
    {
        পূর্ণসংখ্যা চাবি = (dft_আকার << 16) | (depth & 0xFFFF);
        std::map<পূর্ণসংখ্যা, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(চাবি);
        যদি (f != planStorage.end())
        {
            ফেরত f->second;
        }
        অন্যথা
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(নতুন OCL_FftPlan(dft_আকার, depth));
            planStorage[চাবি] = newPlan;
            ফেরত newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

সুরক্ষিত:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<পূর্ণসংখ্যা, Ptr<OCL_FftPlan> > planStorage;
};

স্থির যৌক্তিক ocl_dft_rows(প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা nonzero_rows, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা fftType)
{
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.cols(), depth);
    ফেরত plan->enqueueTransform(_src, _dst, nonzero_rows, flags, fftType, সত্য);
}

স্থির যৌক্তিক ocl_dft_cols(প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা nonzero_cols, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা fftType)
{
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.rows(), depth);
    ফেরত plan->enqueueTransform(_src, _dst, nonzero_cols, flags, fftType, মিথ্যা);
}

অন্তঃরেখ FftType determineFFTType(যৌক্তিক real_প্রবেশ, যৌক্তিক complex_প্রবেশ, যৌক্তিক real_output, যৌক্তিক complex_output, যৌক্তিক inv)
{
    // output format is not specified
    যদি (!real_output && !complex_output)
        complex_output = সত্য;

    // input or output format is ambiguous
    যদি (real_প্রবেশ == complex_প্রবেশ || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType ফলাফল = real_প্রবেশ ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    যদি (ফলাফল == C2R && !inv)
        ফলাফল = C2C;

    // Inverse CCS to Complex not supported
    যদি (ফলাফল == R2C && inv)
        ফলাফল = R2R;

    ফেরত ফলাফল;
}

স্থির যৌক্তিক ocl_dft(প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা nonzero_rows)
{
    পূর্ণসংখ্যা type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    আকার_2 ssize = _src.size();
    যৌক্তিক doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    যদি (!(cn == 1 || cn == 2)
        || !(depth == CV_32F || (depth == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        ফেরত মিথ্যা;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    যদি (ssize.area() != getOptimalDFTSize(ssize.area()))
        ফেরত মিথ্যা;

    UMat src = _src.getUMat();
    যৌক্তিক inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    যদি( nonzero_rows <= 0 || nonzero_rows > _src.rows() )
        nonzero_rows = _src.rows();
    যৌক্তিক is1d = (flags & DFT_ROWS) != 0 || nonzero_rows == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    যদি (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.create_Same_আকার(src, CV_MAKETYPE(depth, 2));
        output = _dst.getUMat();
    }
    অন্যথা
    {
        // real output
        যদি (is1d)
        {
            _dst.create_Same_আকার(src, CV_MAKETYPE(depth, 1));
            output = _dst.getUMat();
        }
        অন্যথা
        {
            _dst.create_Same_আকার(src, CV_MAKETYPE(depth, 1));
            output.create(src.size, CV_MAKETYPE(depth, 2));
        }
    }

    যৌক্তিক ফলাফল = মিথ্যা;
    যদি (!inv)
    {
        পূর্ণসংখ্যা nonzero_cols = fftType == R2R ? output.cols/2 + 1 : output.cols;
        ফলাফল = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
        যদি (!is1d)
            ফলাফল = ফলাফল && ocl_dft_cols(output, _dst, nonzero_cols, flags, fftType);
    }
    অন্যথা
    {
        যদি (fftType == C2C)
        {
            // complex output
            ফলাফল = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            যদি (!is1d)
                ফলাফল = ফলাফল && ocl_dft_cols(output, output, output.cols, flags, fftType);
        }
        অন্যথা
        {
            যদি (is1d)
            {
                ফলাফল = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            }
            অন্যথা
            {
                পূর্ণসংখ্যা nonzero_cols = src.cols/2 + 1;
                ফলাফল = ocl_dft_cols(src, output, nonzero_cols, flags, fftType);
                ফলাফল = ফলাফল && ocl_dft_rows(output, _dst, nonzero_rows, flags, fftType);
            }
        }
    }
    ফেরত ফলাফল;
}

} // namespace cv;

#endif

#ifdef HAVE_CLAMDFFT

নামাকাশ cv {

#define CLAMDDFT_Assert(func) \
    { \
        clfftStatus s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

শ্রেণী PlanCache
{
    কাঠামো FftPlan
    {
        FftPlan(ধ্রুবক আকার_2 & dft_আকার_2, পূর্ণসংখ্যা _src_step, পূর্ণসংখ্যা _dst_step, যৌক্তিক _doubleFP, যৌক্তিক _inplace, পূর্ণসংখ্যা _flags, FftType _fftType) :
            dft_আকার(dft_আকার_2), src_step(_src_step), dst_step(_dst_step),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            যৌক্তিক dft_inverse = (flags & DFT_INVERSE) != 0;
            যৌক্তিক dft_scale = (flags & DFT_SCALE) != 0;
            যৌক্তিক dft_rows = (flags & DFT_ROWS) != 0;

            clfftLayout inLayout = CLFFT_REAL, outLayout = CLFFT_REAL;
            clfftDim dim = dft_আকার.height == 1 || dft_rows ? CLFFT_1D : CLFFT_2D;

            size_t batch_আকার = dft_rows ? dft_আকার.height : 1;
            size_t clLengthsIn[3] = { (size_t)dft_আকার.width, dft_rows ? 1 : (size_t)dft_আকার.height, 1 };
            size_t clStridesIn[3] = { 1, 1, 1 };
            size_t clStridesOut[3]  = { 1, 1, 1 };
            পূর্ণসংখ্যা elem_আকার = doubleFP ? আকার(দ্বিগুণ) : আকার(দশমিক);

            নির্বাচন (fftType)
            {
            ক্ষেত্র C2C:
                inLayout = CLFFT_COMPLEX_INTERLEAVED;
                outLayout = CLFFT_COMPLEX_INTERLEAVED;
                clStridesIn[1] = src_step / (elem_আকার << 1);
                clStridesOut[1] = dst_step / (elem_আকার << 1);
                বিরতি;
            ক্ষেত্র R2C:
                inLayout = CLFFT_REAL;
                outLayout = CLFFT_HERMITIAN_INTERLEAVED;
                clStridesIn[1] = src_step / elem_আকার;
                clStridesOut[1] = dst_step / (elem_আকার << 1);
                বিরতি;
            ক্ষেত্র C2R:
                inLayout = CLFFT_HERMITIAN_INTERLEAVED;
                outLayout = CLFFT_REAL;
                clStridesIn[1] = src_step / (elem_আকার << 1);
                clStridesOut[1] = dst_step / elem_আকার;
                বিরতি;
            ক্ষেত্র R2R:
            পূর্বনির্ধারিত:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                বিরতি;
            }

            clStridesIn[2] = dft_rows ? clStridesIn[1] : dft_আকার.width * clStridesIn[1];
            clStridesOut[2] = dft_rows ? clStridesOut[1] : dft_আকার.width * clStridesOut[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, clLengthsIn))

            // setting plan properties
            CLAMDDFT_Assert(clfftSetPlanPrecision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfft_Set_ফলাফল_Location(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfftSetLayout(plHandle, inLayout, outLayout))
            CLAMDDFT_Assert(clfft_Set_Plan_Batch_আকার(plHandle, batch_আকার))
            CLAMDDFT_Assert(clfftSetPlanInStride(plHandle, dim, clStridesIn))
            CLAMDDFT_Assert(clfftSetPlanOutStride(plHandle, dim, clStridesOut))
            CLAMDDFT_Assert(clfftSetPlanDistance(plHandle, clStridesIn[dim], clStridesOut[dim]))

            দশমিক scale = dft_scale ? 1.0f / (dft_rows ? dft_আকার.width : dft_আকার.area()) : 1.0f;
            CLAMDDFT_Assert(clfftSetPlanScale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &queue, ফাঁকা, ফাঁকা))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        friend শ্রেণী PlanCache;

    ব্যক্তিগত:
        আকার_2 dft_আকার;
        পূর্ণসংখ্যা src_step, dst_step;
        যৌক্তিক doubleFP;
        যৌক্তিক inplace;
        পূর্ণসংখ্যা flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

সর্বজনীন:
    স্থির PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, নতুন PlanCache())
    }

    clfftPlanHandle getPlanHandle(ধ্রুবক আকার_2 & dft_আকার, পূর্ণসংখ্যা src_step, পূর্ণসংখ্যা dst_step, যৌক্তিক doubleFP,
                                  যৌক্তিক inplace, পূর্ণসংখ্যা flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        জন্য (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            ধ্রুবক FftPlan * ধ্রুবক plan = planStorage[i];

            যদি (plan->dft_আকার == dft_আকার &&
                plan->flags == flags &&
                plan->src_step == src_step &&
                plan->dst_step == dst_step &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                যদি (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    বিরতি;
                }

                ফেরত plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(নতুন FftPlan(dft_আকার, src_step, dst_step, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        ফেরত newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

সুরক্ষিত:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

বাহ্যিক "C" {

স্থির শূন্য CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, শূন্য *p)
{
    UMat_তথ্য * u = (UMat_তথ্য *)p;

    যদি( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

স্থির যৌক্তিক ocl_dft_amdfft(প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা flags)
{
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    আকার_2 ssize = _src.size();

    যৌক্তিক doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    যদি ( (!doubleSupport && depth == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        ফেরত মিথ্যা;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    যদি (ssize.area() != getOptimalDFTSize(ssize.area()))
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা dst_complex_প্রবেশ = cn == 2 ? 1 : 0;
    যৌক্তিক dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    পূর্ণসংখ্যা dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    যৌক্তিক dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_প্রবেশ << 0 | dft_complex_output << 1);

    নির্বাচন (fftType)
    {
    ক্ষেত্র C2C:
        _dst.create(ssize.height, ssize.width, CV_MAKE_TYPE(depth, 2));
        বিরতি;
    ক্ষেত্র R2C: // TODO implement it if possible
    ক্ষেত্র C2R: // TODO implement it if possible
    ক্ষেত্র R2R: // AMD Fft does not support this type
    পূর্বনির্ধারিত:
        ফেরত মিথ্যা;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    যদি (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        ফেরত মিথ্যা;
    যৌক্তিক inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (পূর্ণসংখ্যা)src.step, (পূর্ণসংখ্যা)dst.step,
                          depth == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t বাফার_আকার = 0;
    CLAMDDFT_Assert(clfft_Get_Tmp_Buf_আকার(plHandle, &বাফার_আকার))
    UMat tmp_বাফার(1, (পূর্ণসংখ্যা)বাফার_আকার, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfftEnqueueTransform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &queue, 0, ফাঁকা, &e,
                                          &srcarg, &dstarg, (cl_mem)tmp_বাফার.handle(ACCESS_RW)))

    tmp_বাফার.addref();
    clSetEventCallback(e, CL_COMPLETE, oclCleanupCallback, tmp_বাফার.u);
    ফেরত সত্য;
}

#undef DFT_ASSERT

}

#endif // HAVE_CLAMDFFT

নামাকাশ cv
{

template <typename T>
স্থির শূন্য complementComplex(T * ptr, size_t step, পূর্ণসংখ্যা n, পূর্ণসংখ্যা len, পূর্ণসংখ্যা dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = step/আকার(p0[0]);
    জন্য(পূর্ণসংখ্যা i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        জন্য( পূর্ণসংখ্যা j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

স্থির শূন্য complementComplexOutput(পূর্ণসংখ্যা depth, uchar * ptr, size_t step, পূর্ণসংখ্যা গুনতি_2, পূর্ণসংখ্যা len, পূর্ণসংখ্যা dft_dims)
{
    যদি( depth == CV_32F )
        complementComplex((দশমিক*)ptr, step, গুনতি_2, len, dft_dims);
    অন্যথা
        complementComplex((দ্বিগুণ*)ptr, step, গুনতি_2, len, dft_dims);
}

গণনা DftMode {
    InvalidDft = 0,
    FwdRealToCCS,
    FwdRealToComplex,
    FwdComplexToComplex,
    InvCCSToReal,
    InvComplexToReal,
    InvComplexToComplex,
};

গণনা DftDims {
    InvalidDim = 0,
    OneDim,
    OneDimColWise,
    TwoDims
};

অন্তঃরেখ ধ্রুবক অক্ষর * modeName(DftMode m)
{
    নির্বাচন (m)
    {
    ক্ষেত্র InvalidDft: ফেরত "InvalidDft";
    ক্ষেত্র FwdRealToCCS: ফেরত "FwdRealToCCS";
    ক্ষেত্র FwdRealToComplex: ফেরত "FwdRealToComplex";
    ক্ষেত্র FwdComplexToComplex: ফেরত "FwdComplexToComplex";
    ক্ষেত্র InvCCSToReal: ফেরত "InvCCSToReal";
    ক্ষেত্র InvComplexToReal: ফেরত "InvComplexToReal";
    ক্ষেত্র InvComplexToComplex: ফেরত "InvComplexToComplex";
    }
    ফেরত 0;
}

অন্তঃরেখ ধ্রুবক অক্ষর * dimsName(DftDims d)
{
    নির্বাচন (d)
    {
    ক্ষেত্র InvalidDim: ফেরত "InvalidDim";
    ক্ষেত্র OneDim: ফেরত "OneDim";
    ক্ষেত্র OneDimColWise: ফেরত "OneDimColWise";
    ক্ষেত্র TwoDims: ফেরত "TwoDims";
    };
    ফেরত 0;
}

template <typename T>
অন্তঃরেখ যৌক্তিক isInv(T mode)
{
    নির্বাচন ((DftMode)mode)
    {
        ক্ষেত্র InvCCSToReal:
        ক্ষেত্র InvComplexToReal:
        ক্ষেত্র InvComplexToComplex: ফেরত সত্য;
        পূর্বনির্ধারিত: ফেরত মিথ্যা;
    }
}

অন্তঃরেখ DftMode determineMode(যৌক্তিক inv, পূর্ণসংখ্যা cn1, পূর্ণসংখ্যা cn2)
{
    যদি (!inv)
    {
        যদি (cn1 == 1 && cn2 == 1)
            ফেরত FwdRealToCCS;
        অন্যথা যদি (cn1 == 1 && cn2 == 2)
            ফেরত FwdRealToComplex;
        অন্যথা যদি (cn1 == 2 && cn2 == 2)
            ফেরত FwdComplexToComplex;
    }
    অন্যথা
    {
        যদি (cn1 == 1 && cn2 == 1)
            ফেরত InvCCSToReal;
        অন্যথা যদি (cn1 == 2 && cn2 == 1)
            ফেরত InvComplexToReal;
        অন্যথা যদি (cn1 == 2 && cn2 == 2)
            ফেরত InvComplexToComplex;
    }
    ফেরত InvalidDft;
}


অন্তঃরেখ DftDims determineDims(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, যৌক্তিক isRowWise, যৌক্তিক isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    যদি (isRowWise)
        ফেরত OneDim;
    যদি (cols == 1 && rows > 1) // one-column-shaped input
    {
        যদি (isContinuous)
            ফেরত OneDim;
        অন্যথা
            ফেরত OneDimColWise;
    }
    যদি (rows == 1)
        ফেরত OneDim;
    যদি (cols > 1 && rows > 1)
        ফেরত TwoDims;
    ফেরত InvalidDim;
}

শ্রেণী OcvDftBasicImpl CV_FINAL : সর্বজনীন hal::DFT1D
{
সর্বজনীন:
    OcvDftOptions opt;
    // the plan is built once in init(), the workspace is allocated once here and reused by
    // every apply() (zero allocations per transform)
    DftPlan plan;
    Auto_বাফার<uchar> ws;
#ifdef USE_IPP_DFT
    Auto_বাফার<uchar> ippbuf;
    Auto_বাফার<uchar> ippworkbuf;
    size_t ipp_worksize;
#endif

সর্বজনীন:
    OcvDftBasicImpl()
    {
#ifdef USE_IPP_DFT
        ipp_worksize = 0;
#endif
    }
    শূন্য init(পূর্ণসংখ্যা len, পূর্ণসংখ্যা গুনতি_2, পূর্ণসংখ্যা depth, পূর্ণসংখ্যা flags, যৌক্তিক *need_বাফার)
    {
        পূর্ণসংখ্যা stage = (flags & CV_HAL_DFT_STAGE_COLS) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        যৌক্তিক real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        যৌক্তিক needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = মিথ্যা;
    #ifdef USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        যদি( CV_IPP_CHECK_COND && (opt.n*গুনতি_2 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            পূর্ণসংখ্যা ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            পূর্ণসংখ্যা specsize=0, initsize=0, worksize=0;
            Ipp_DFTGet_আকার_Func get_আকার_Func = 0;
            IppDFTInitFunc initFunc = 0;

            যদি( real_transform && stage == 0 )
            {
                যদি( depth == CV_32F )
                {
                    get_আকার_Func = ipps_DFTGet_আকার_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                অন্যথা
                {
                    get_আকার_Func = ipps_DFTGet_আকার_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            অন্যথা
            {
                যদি( depth == CV_32F )
                {
                    get_আকার_Func = ipps_DFTGet_আকার_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                অন্যথা
                {
                    get_আকার_Func = ipps_DFTGet_আকার_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            যদি( get_আকার_Func(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                যদি( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = সত্য;
            }
            অন্যথা
                setIppErrorStatus();
        }
    #endif

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            ধ্রুবক DFTKernels& kernels = getDFTKernels(depth);
            পূর্ণসংখ্যা kind = stage == 0 && real_transform ? (opt.isInverse ? DFT_KIND_C2R : DFT_KIND_R2C)
                                                    : DFT_KIND_C2C;
            plan.build(kind, len, depth, kernels.vlanes);
            ws.allocate(plan.ws_bytes);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.তথ্য();
            যদি (need_বাফার)
                *need_বাফার = মিথ্যা;   // the engine supports in-place for every kind
        }

        {
            স্থির DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<দশমিক>>,
                dftWrap<দশমিক, RealDFT<দশমিক>>,
                dftWrap<দশমিক, CCSIDFT<দশমিক>>,
                dftWrap<Complexd, DFT<দ্বিগুণ>>,
                dftWrap<দ্বিগুণ, RealDFT<দ্বিগুণ>>,
                dftWrap<দ্বিগুণ, CCSIDFT<দ্বিগুণ>>
            };
            পূর্ণসংখ্যা idx = 0;
            যদি (stage == 0)
            {
                যদি (real_transform)
                {
                    যদি (!opt.isInverse)
                        idx = 1;
                    অন্যথা
                        idx = 2;
                }
            }
            যদি (depth == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        যদি(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            পূর্ণসংখ্যা row_গুনতি = গুনতি_2;
            যদি (stage == 0 && (flags & CV_HAL_DFT_ROWS) != 0)
                row_গুনতি = 1;
            opt.scale = 1./(len * row_গুনতি);
        }
    }

    শূন্য apply(ধ্রুবক uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspace_আকার() ধ্রুবক
    {
        size_t sz = opt.plan ? plan.ws_bytes : 0;
#ifdef USE_IPP_DFT
        যদি (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#endif
        ফেরত sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    শূন্য applyWithWorkspace(ধ্রুবক uchar *src, uchar *dst, uchar* workspace) ধ্রুবক
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#ifdef USE_IPP_DFT
        যদি (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#endif
        opt_.dft_func(opt_, src, dst);
    }

    শূন্য মুক্ত() {}
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
স্থির ধ্রুবক পূর্ণসংখ্যা DFT_COLS_BATCH_MIN = 4;
স্থির ধ্রুবক পূর্ণসংখ্যা DFT_COLS_BATCH_MAX = 16;
স্থির ধ্রুবক size_t DFT_COLS_BATCH_BYTES = 1 << 17;

স্থির পূর্ণসংখ্যা dftColsBatch(পূর্ণসংখ্যা len, size_t complex_elem_আকার)
{
    পূর্ণসংখ্যা batch = (পূর্ণসংখ্যা)(DFT_COLS_BATCH_BYTES/((size_t)len*complex_elem_আকার));
    ফেরত std::max(DFT_COLS_BATCH_MIN, std::min(DFT_COLS_BATCH_MAX, batch));
}

স্থির দ্বিগুণ dftParallelStripes(পূর্ণসংখ্যা গুনতি_2, পূর্ণসংখ্যা len)
{
    ধ্রুবক দ্বিগুণ quantum = 0.5e6;
    ফেরত (দ্বিগুণ)গুনতি_2*len*std::max(std::log((দ্বিগুণ)len)/std::log(2.), 1.)/quantum;
}

শ্রেণী OcvDftImpl CV_FINAL : সর্বজনীন hal::DFT2D
{
সুরক্ষিত:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    ধ্রুবক OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    ধ্রুবক OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    যৌক্তিক need_বাফার_A;
    যৌক্তিক need_বাফার_B;
    যৌক্তিক inv;
    পূর্ণসংখ্যা width;
    পূর্ণসংখ্যা height;
    DftMode mode;
    পূর্ণসংখ্যা elem_আকার_2;
    পূর্ণসংখ্যা complex_elem_আকার;
    পূর্ণসংখ্যা depth;
    যৌক্তিক real_transform;
    পূর্ণসংখ্যা nonzero_rows;
    যৌক্তিক isRowTransform;
    যৌক্তিক isScaled;
    std::vector<পূর্ণসংখ্যা> stages;
    যৌক্তিক useIpp;
    পূর্ণসংখ্যা src_channels;
    পূর্ণসংখ্যা dst_channels;

    Auto_বাফার<uchar> tmp_bufA;
    Auto_বাফার<uchar> tmp_bufB;
    Auto_বাফার<uchar> buf0;
    Auto_বাফার<uchar> buf1;

সর্বজনীন:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        need_বাফার_A = মিথ্যা;
        need_বাফার_B = মিথ্যা;
        inv = মিথ্যা;
        width = 0;
        height = 0;
        mode = InvalidDft;
        elem_আকার_2 = 0;
        complex_elem_আকার = 0;
        depth = 0;
        real_transform = মিথ্যা;
        nonzero_rows = 0;
        isRowTransform = মিথ্যা;
        isScaled = মিথ্যা;
        useIpp = মিথ্যা;
        src_channels = 0;
        dst_channels = 0;
    }

    শূন্য init(পূর্ণসংখ্যা _width, পূর্ণসংখ্যা _height, পূর্ণসংখ্যা _depth, পূর্ণসংখ্যা _src_channels, পূর্ণসংখ্যা _dst_channels, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা _nonzero_rows)
    {
        যৌক্তিক isComplex = _src_channels != _dst_channels;
        nonzero_rows = _nonzero_rows;
        width = _width;
        height = _height;
        depth = _depth;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        যৌক্তিক isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        যৌক্তিক isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        যৌক্তিক isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        mode = determineMode(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        need_বাফার_A = মিথ্যা;
        need_বাফার_B = মিথ্যা;
        real_transform = (mode != FwdComplexToComplex && mode != InvComplexToComplex);

        elem_আকার_2 = (depth == CV_32F) ? আকার(দশমিক) : আকার(দ্বিগুণ);
        complex_elem_আকার = elem_আকার_2 * 2;
        যদি( !real_transform )
            elem_আকার_2 = complex_elem_আকার;

#যদি defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            যদি (nonzero_rows == 0 && depth == CV_32F && ((width * height)>(পূর্ণসংখ্যা)(1<<6)))
            {
                যদি (mode == FwdComplexToComplex || mode == InvComplexToComplex || mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    useIpp = সত্য;
                    ফেরত;
                }
            }
        }
#endif

        DftDims dims = determineDims(height, width, isRowTransform, isContinuous);
        যদি (dims == TwoDims)
        {
            stages.resize(2);
            যদি (mode == InvCCSToReal || mode == InvComplexToReal)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            অন্যথা
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        অন্যথা
        {
            stages.resize(1);
            যদি (dims == OneDimColWise)
                stages[0] = 1;
            অন্যথা
                stages[0] = 0;
        }

        জন্য(uint stage_সূচক = 0; stage_সূচক < stages.size(); ++stage_সূচক)
        {
            যদি (stage_সূচক == 1)
            {
                isInplace = সত্য;
                isComplex = মিথ্যা;
            }

            পূর্ণসংখ্যা stage = stages[stage_সূচক];
            যৌক্তিক isLastStage = (stage_সূচক + 1 == stages.size());

            পূর্ণসংখ্যা len, গুনতি_2;

            পূর্ণসংখ্যা f = 0;
            যদি (inv)
                f |= CV_HAL_DFT_INVERSE;
            যদি (isScaled)
                f |= CV_HAL_DFT_SCALE;
            যদি (isRowTransform)
                f |= CV_HAL_DFT_ROWS;
            যদি (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            যদি (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            যদি (!isLastStage)
                f |= CV_HAL_DFT_TWO_STAGE;

            যদি( stage == 0 ) // row-wise transform
            {
                যদি (width == 1 && !isRowTransform )
                {
                    len = height;
                    গুনতি_2 = width;
                }
                অন্যথা
                {
                    len = width;
                    গুনতি_2 = height;
                }
                need_বাফার_A = isInplace;
                contextA = hal::DFT1D::create(len, গুনতি_2, depth, f, &need_বাফার_A);
                basicA = dynamic_cast<ধ্রুবক OcvDftBasicImpl*>(contextA.get());
                যদি (need_বাফার_A)
                    tmp_bufA.allocate(len * complex_elem_আকার);
            }
            অন্যথা
            {
                len = height;
                গুনতি_2 = width;
                f |= CV_HAL_DFT_STAGE_COLS;
                need_বাফার_B = isInplace;
                contextB = hal::DFT1D::create(len, গুনতি_2, depth, f, &need_বাফার_B);
                basicB = dynamic_cast<ধ্রুবক OcvDftBasicImpl*>(contextB.get());
                যদি (need_বাফার_B)
                    tmp_bufB.allocate(len * complex_elem_আকার);

                buf0.allocate(len * complex_elem_আকার);
                buf1.allocate(len * complex_elem_আকার);
            }
        }
    }

    শূন্য apply(ধ্রুবক uchar * src, size_t src_step, uchar * dst, size_t dst_step) CV_OVERRIDE
    {
#যদি defined USE_IPP_DFT
        যদি (useIpp)
        {
            পূর্ণসংখ্যা ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            যদি (!isRowTransform)
            {
                যদি (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    যদি (ippi_DFT_C_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        ফেরত;
                    }
                    setIppErrorStatus();
                }
                অন্যথা যদি (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    যদি (ippi_DFT_R_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        ফেরত;
                    }
                    setIppErrorStatus();
                }
            }
            অন্যথা
            {
                যদি (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    যদি (Dft_C_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        ফেরত;
                    }
                    setIppErrorStatus();
                }
                অন্যথা যদি (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippiDFTInv_PackToR_32f_C1R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    যদি (Dft_R_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        ফেরত;
                    }
                    setIppErrorStatus();
                }
            }
            ফেরত;
        }
#endif

        জন্য(uint stage_সূচক = 0; stage_সূচক < stages.size(); ++stage_সূচক)
        {
            পূর্ণসংখ্যা stage_src_channels = src_channels;
            পূর্ণসংখ্যা stage_dst_channels = dst_channels;

            যদি (stage_সূচক == 1)
            {
                src = dst;
                src_step = dst_step;
                stage_src_channels = stage_dst_channels;
            }

            পূর্ণসংখ্যা stage = stages[stage_সূচক];
            যৌক্তিক isLastStage = (stage_সূচক + 1 == stages.size());
            যৌক্তিক isComplex = stage_src_channels != stage_dst_channels;

            যদি( stage == 0 )
                rowDft(src, src_step, dst, dst_step, isComplex, isLastStage);
            অন্যথা
                colDft(src, src_step, dst, dst_step, stage_src_channels, stage_dst_channels, isLastStage);
        }
    }

সুরক্ষিত:

    শূন্য rowDft(ধ্রুবক uchar* src_তথ্য, size_t src_step, uchar* dst_তথ্য, size_t dst_step, যৌক্তিক isComplex, যৌক্তিক isLastStage)
    {
        পূর্ণসংখ্যা len, গুনতি_2;
        যদি (width == 1 && !isRowTransform )
        {
            len = height;
            গুনতি_2 = width;
        }
        অন্যথা
        {
            len = width;
            গুনতি_2 = height;
        }
        পূর্ণসংখ্যা dptr_offset = 0;
        পূর্ণসংখ্যা dst_full_len = len*elem_আকার_2;

        যদি( need_বাফার_A )
        {
            যদি (mode == FwdRealToCCS && (len & 1) && len > 1)
                dptr_offset = elem_আকার_2;
        }

        যদি( !inv && isComplex )
            dst_full_len += (len & 1) ? elem_আকার_2 : complex_elem_আকার;

        পূর্ণসংখ্যা nz = nonzero_rows;
        যদি( nz <= 0 || nz > গুনতি_2 )
            nz = গুনতি_2;

        size_t ws_আকার = basicA ? basicA->workspace_আকার() : 0;
        দ্বিগুণ nstripes = dftParallelStripes(nz, len);
        যদি( ws_আকার > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            ধ্রুবক OcvDftBasicImpl* ctx = basicA;
            auto processRows = [&, ctx](ধ্রুবক Range& range)
            {
                Auto_বাফার<uchar> ws(ws_আকার);
                Auto_বাফার<uchar> rowbuf(need_বাফার_A ? len * complex_elem_আকার : 0);
                জন্য( পূর্ণসংখ্যা i = range.start; i < range.end; i++ )
                {
                    ধ্রুবক uchar* sptr = src_তথ্য + src_step * i;
                    uchar* dptr0 = dst_তথ্য + dst_step * i;
                    uchar* dptr = need_বাফার_A ? rowbuf.তথ্য() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.তথ্য());
                    যদি( need_বাফার_A )
                        memcpy( dptr0, dptr + dptr_offset, dst_full_len );
                }
            };
            parallel_for_(Range(0, nz), processRows, nstripes);
        }
        অন্যথা
        {
            জন্য( পূর্ণসংখ্যা i = 0; i < nz; i++ )
            {
                ধ্রুবক uchar* sptr = src_তথ্য + src_step * i;
                uchar* dptr0 = dst_তথ্য + dst_step * i;
                uchar* dptr = dptr0;

                যদি( need_বাফার_A )
                    dptr = tmp_bufA.তথ্য();

                contextA->apply(sptr, dptr);

                যদি( need_বাফার_A )
                    memcpy( dptr0, dptr + dptr_offset, dst_full_len );
            }
        }

        জন্য( পূর্ণসংখ্যা i = nz; i < গুনতি_2; i++ )
        {
            uchar* dptr0 = dst_তথ্য + dst_step * i;
            memset( dptr0, 0, dst_full_len );
        }
        যদি(isLastStage &&  mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_তথ্য, dst_step, len, nz, 1);
    }

    শূন্য colDft(ধ্রুবক uchar* src_তথ্য, size_t src_step, uchar* dst_তথ্য, size_t dst_step, পূর্ণসংখ্যা stage_src_channels, পূর্ণসংখ্যা stage_dst_channels, যৌক্তিক isLastStage)
    {
        পূর্ণসংখ্যা len = height;
        পূর্ণসংখ্যা গুনতি_2 = width;
        পূর্ণসংখ্যা a = 0, b = গুনতি_2;
        uchar *dbuf0, *dbuf1;
        ধ্রুবক uchar* sptr0 = src_তথ্য;
        uchar* dptr0 = dst_তথ্য;

        dbuf0 = buf0.তথ্য(), dbuf1 = buf1.তথ্য();

        যদি( need_বাফার_B )
        {
            dbuf1 = tmp_bufB.তথ্য();
            dbuf0 = buf1.তথ্য();
        }

        যদি( real_transform )
        {
            পূর্ণসংখ্যা even;
            a = 1;
            even = (গুনতি_2 & 1) == 0;
            b = (গুনতি_2+1)/2;
            যদি( !inv )
            {
                memset( buf0.তথ্য(), 0, len*complex_elem_আকার );
                CopyColumn( sptr0, src_step, buf0.তথ্য(), complex_elem_আকার, len, elem_আকার_2 );
                sptr0 += stage_dst_channels*elem_আকার_2;
                যদি( even )
                {
                    memset( buf1.তথ্য(), 0, len*complex_elem_আকার );
                    CopyColumn( sptr0 + (গুনতি_2-2)*elem_আকার_2, src_step,
                                buf1.তথ্য(), complex_elem_আকার, len, elem_আকার_2 );
                }
            }
            অন্যথা যদি( stage_src_channels == 1 )
            {
                CopyColumn( sptr0, src_step, buf0.তথ্য(), elem_আকার_2, len, elem_আকার_2 );
                ExpandCCS( buf0.তথ্য(), len, elem_আকার_2 );
                যদি( even )
                {
                    CopyColumn( sptr0 + (গুনতি_2-1)*elem_আকার_2, src_step,
                                buf1.তথ্য(), elem_আকার_2, len, elem_আকার_2 );
                    ExpandCCS( buf1.তথ্য(), len, elem_আকার_2 );
                }
                sptr0 += elem_আকার_2;
            }
            অন্যথা
            {
                CopyColumn( sptr0, src_step, buf0.তথ্য(), complex_elem_আকার, len, complex_elem_আকার );
                যদি( even )
                {
                    CopyColumn( sptr0 + b*complex_elem_আকার, src_step,
                                   buf1.তথ্য(), complex_elem_আকার, len, complex_elem_আকার );
                }
                sptr0 += complex_elem_আকার;
            }

            যদি( even )
                contextB->apply(buf1.তথ্য(), dbuf1);
            contextB->apply(buf0.তথ্য(), dbuf0);

            যদি( stage_dst_channels == 1 )
            {
                যদি( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    memcpy( dbuf0 + elem_আকার_2, dbuf0, elem_আকার_2 );
                    CopyColumn( dbuf0 + elem_আকার_2, elem_আকার_2, dptr0,
                                   dst_step, len, elem_আকার_2 );
                    যদি( even )
                    {
                        memcpy( dbuf1 + elem_আকার_2, dbuf1, elem_আকার_2 );
                        CopyColumn( dbuf1 + elem_আকার_2, elem_আকার_2,
                                       dptr0 + (গুনতি_2-1)*elem_আকার_2,
                                       dst_step, len, elem_আকার_2 );
                    }
                    dptr0 += elem_আকার_2;
                }
                অন্যথা
                {
                    // copy the real part of the complex vector to the first/last column
                    CopyColumn( dbuf0, complex_elem_আকার, dptr0, dst_step, len, elem_আকার_2 );
                    যদি( even )
                        CopyColumn( dbuf1, complex_elem_আকার, dptr0 + (গুনতি_2-1)*elem_আকার_2,
                                       dst_step, len, elem_আকার_2 );
                    dptr0 += elem_আকার_2;
                }
            }
            অন্যথা
            {
                CV_Assert( !inv );
                CopyColumn( dbuf0, complex_elem_আকার, dptr0,
                               dst_step, len, complex_elem_আকার );
                যদি( even )
                    CopyColumn( dbuf1, complex_elem_আকার,
                                   dptr0 + b*complex_elem_আকার,
                                   dst_step, len, complex_elem_আকার );
                dptr0 += complex_elem_আকার;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        পূর্ণসংখ্যা ncols = b - a;
        ধ্রুবক পূর্ণসংখ্যা cols_batch = dftColsBatch(len, complex_elem_আকার);
        পূর্ণসংখ্যা nbatches = (ncols + cols_batch - 1)/cols_batch;
        size_t ws_আকার = basicB ? basicB->workspace_আকার() : 0;
        size_t colbuf_আকার = (size_t)len*complex_elem_আকার;
        ধ্রুবক OcvDftBasicImpl* ctx = basicB;
        auto processBatches = [&, ctx](ধ্রুবক Range& range)
        {
            Auto_বাফার<uchar> ws(ws_আকার);
            Auto_বাফার<uchar> cbuf(colbuf_আকার*cols_batch*(need_বাফার_B ? 2 : 1));
            uchar* cbuf0 = cbuf.তথ্য();
            uchar* cdbuf0 = need_বাফার_B ? cbuf0 + colbuf_আকার*cols_batch : cbuf0;
            জন্য( পূর্ণসংখ্যা bi = range.start; bi < range.end; bi++ )
            {
                পূর্ণসংখ্যা c0 = bi*cols_batch;
                পূর্ণসংখ্যা nc = std::min(cols_batch, ncols - c0);
                ধ্রুবক uchar* sptr = sptr0 + (size_t)c0*complex_elem_আকার;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_আকার;
                CopyFromColumns( sptr, src_step, cbuf0, nc, len, complex_elem_আকার );
                জন্য( পূর্ণসংখ্যা k = 0; k < nc; k++ )
                {
                    যদি( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_আকার, cdbuf0 + k*colbuf_আকার, ws.তথ্য());
                    অন্যথা
                        contextB->apply(cbuf0 + k*colbuf_আকার, cdbuf0 + k*colbuf_আকার);
                }
                CopyToColumns( cdbuf0, dptr, dst_step, nc, len, complex_elem_আকার );
            }
        };
        দ্বিগুণ nstripes = dftParallelStripes(ncols, len);
        যদি( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        অন্যথা
            processBatches(Range(0, nbatches));
        যদি(isLastStage && mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_তথ্য, dst_step, গুনতি_2, len, 2);
    }
};

কাঠামো ReplacementDFT1D : সর্বজনীন hal::DFT1D
{
    cvhalDFT *context;
    যৌক্তিক isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(মিথ্যা) {}
    যৌক্তিক init(পূর্ণসংখ্যা len, পূর্ণসংখ্যা গুনতি_2, পূর্ণসংখ্যা depth, পূর্ণসংখ্যা flags, যৌক্তিক *need_বাফার)
    {
        পূর্ণসংখ্যা res = cv_hal_dftInit1D(&context, len, গুনতি_2, depth, flags, need_বাফার);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ফেরত isInitialized;
    }
    শূন্য apply(ধ্রুবক uchar *src, uchar *dst) CV_OVERRIDE
    {
        যদি (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        যদি (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

কাঠামো ReplacementDFT2D : সর্বজনীন hal::DFT2D
{
    cvhalDFT *context;
    যৌক্তিক isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(মিথ্যা) {}
    যৌক্তিক init(পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, পূর্ণসংখ্যা depth,
              পূর্ণসংখ্যা src_channels, পূর্ণসংখ্যা dst_channels,
              পূর্ণসংখ্যা flags, পূর্ণসংখ্যা nonzero_rows)
    {
        পূর্ণসংখ্যা res = cv_hal_dftInit2D(&context, width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ফেরত isInitialized;
    }
    শূন্য apply(ধ্রুবক uchar *src, size_t src_step, uchar *dst, size_t dst_step) CV_OVERRIDE
    {
        যদি (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_step, dst, dst_step);
        }
    }
    ~ReplacementDFT2D()
    {
        যদি (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

নামাকাশ hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(পূর্ণসংখ্যা len, পূর্ণসংখ্যা গুনতি_2, পূর্ণসংখ্যা depth, পূর্ণসংখ্যা flags, যৌক্তিক *need_বাফার)
{
    {
        ReplacementDFT1D *impl = নতুন ReplacementDFT1D();
        যদি (impl->init(len, গুনতি_2, depth, flags, need_বাফার))
        {
            ফেরত Ptr<DFT1D>(impl);
        }
        delete impl;
    }
    {
        OcvDftBasicImpl *impl = নতুন OcvDftBasicImpl();
        impl->init(len, গুনতি_2, depth, flags, need_বাফার);
        ফেরত Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, পূর্ণসংখ্যা depth,
                         পূর্ণসংখ্যা src_channels, পূর্ণসংখ্যা dst_channels,
                         পূর্ণসংখ্যা flags, পূর্ণসংখ্যা nonzero_rows)
{
    {
        ReplacementDFT2D *impl = নতুন ReplacementDFT2D();
        যদি (impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows))
        {
            ফেরত Ptr<DFT2D>(impl);
        }
        delete impl;
    }
    {
        যদি(width == 1 && nonzero_rows > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = নতুন OcvDftImpl();
        impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        ফেরত Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


শূন্য cv::dft( প্রবেশ_Array _src0, OutputArray _dst, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা nonzero_rows )
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
    যদি (src.empty())
    {
        _dst.release();
        ফেরত;
    }
    যৌক্তিক inv = (flags & DFT_INVERSE) != 0;
    পূর্ণসংখ্যা type = src.type();
    পূর্ণসংখ্যা depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_প্রবেশ) && src.channels() != 2) );

    যদি( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.create_Same_আকার( src, CV_MAKETYPE(depth, 2) );
    অন্যথা যদি( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.create_Same_আকার( src, depth );
    অন্যথা
        _dst.create_Same_আকার( src, type );

    Mat dst = _dst.getMat();

    পূর্ণসংখ্যা f = 0;
    যদি (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    যদি (inv)
        f |= CV_HAL_DFT_INVERSE;
    যদি (flags & DFT_ROWS)
        f |= CV_HAL_DFT_ROWS;
    যদি (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    যদি (src.তথ্য == dst.তথ্য)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.cols, src.rows, depth, src.channels(), dst.channels(), f, nonzero_rows);
    c->apply(src.তথ্য, src.step, dst.তথ্য, dst.step);
}


শূন্য cv::idft( প্রবেশ_Array src, OutputArray dst, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা nonzero_rows )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_rows );
}

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির যৌক্তিক ocl_mulSpectrums( প্রবেশ_Array _srcA, প্রবেশ_Array _srcB,
                              OutputArray _dst, পূর্ণসংখ্যা flags, যৌক্তিক conjB )
{
    পূর্ণসংখ্যা atype = _srcA.type(), btype = _srcB.type(),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    আকার_2 asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    যদি ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        ফেরত মিথ্যা;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.create_Same_আকার(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    যদি (k.empty())
        ফেরত মিথ্যা;

    k.args(ocl::KernelArg::Read_Only_No_আকার(A), ocl::KernelArg::Read_Only_No_আকার(B),
           ocl::KernelArg::WriteOnly(dst), rowsPerWI);

    size_t globalsize[2] = { (size_t)asize.width, ((size_t)asize.height + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

}

#endif

নামাকাশ {

#define VAL(buf, elem) (((T*)((অক্ষর*)তথ্য ## buf + (step ## buf * (elem))))[0])
#define MUL_SPECTRUMS_COL(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    জন্য (size_t j = 1; j <= rows - 2; j += 2) \
    { \
        দ্বিগুণ a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        দ্বিগুণ b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        যদি (conjB) b_im = -b_im; \
        দ্বিগুণ c_re = a_re * b_re - a_im * b_im; \
        দ্বিগুণ c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    যদি ((rows & 1) == 0) \
        VAL(C, rows-1) = VAL(A, rows-1) * VAL(B, rows-1)

template <typename T, যৌক্তিক conjB> স্থির অন্তঃরেখ
শূন্য mulSpectrums_processCol_noinplace(ধ্রুবক T* তথ্য_A, ধ্রুবক T* তথ্য_B, T* তথ্য_C, size_t stepA, size_t stepB, size_t stepC, size_t rows)
{
    MUL_SPECTRUMS_COL(A, B, C);
}

template <typename T, যৌক্তিক conjB> স্থির অন্তঃরেখ
শূন্য mulSpectrums_processCol_inplaceA(ধ্রুবক T* তথ্য_B, T* তথ্য_AC, size_t stepB, size_t stepAC, size_t rows)
{
    MUL_SPECTRUMS_COL(AC, B, AC);
}
template <typename T, যৌক্তিক conjB, যৌক্তিক inplaceA> স্থির অন্তঃরেখ
শূন্য mulSpectrums_processCol(ধ্রুবক T* তথ্য_A, ধ্রুবক T* তথ্য_B, T* তথ্য_C, size_t stepA, size_t stepB, size_t stepC, size_t rows)
{
    যদি (inplaceA)
        mulSpectrums_processCol_inplaceA<T, conjB>(তথ্য_B, তথ্য_C, stepB, stepC, rows);
    অন্যথা
        mulSpectrums_processCol_noinplace<T, conjB>(তথ্য_A, তথ্য_B, তথ্য_C, stepA, stepB, stepC, rows);
}
#undef MUL_SPECTRUMS_COL
#undef VAL

template <typename T, যৌক্তিক conjB, যৌক্তিক inplaceA> স্থির অন্তঃরেখ
শূন্য mulSpectrums_processCols(ধ্রুবক T* তথ্য_A, ধ্রুবক T* তথ্য_B, T* তথ্য_C, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols)
{
    mulSpectrums_processCol<T, conjB, inplaceA>(তথ্য_A, তথ্য_B, তথ্য_C, stepA, stepB, stepC, rows);
    যদি ((cols & 1) == 0)
    {
        mulSpectrums_processCol<T, conjB, inplaceA>(তথ্য_A + cols - 1, তথ্য_B + cols - 1, তথ্য_C + cols - 1, stepA, stepB, stepC, rows);
    }
}

#define VAL(buf, elem) (তথ্য ## buf[(elem)])
#define MUL_SPECTRUMS_ROW(A, B, C) \
    জন্য (size_t j = j0; j < j1; j += 2) \
    { \
        দ্বিগুণ a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        দ্বিগুণ b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        যদি (conjB) b_im = -b_im; \
        দ্বিগুণ c_re = a_re * b_re - a_im * b_im; \
        দ্বিগুণ c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
template <typename T, যৌক্তিক conjB> স্থির অন্তঃরেখ
শূন্য mulSpectrums_processRow_noinplace(ধ্রুবক T* তথ্য_A, ধ্রুবক T* তথ্য_B, T* তথ্য_C, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(A, B, C);
}
template <typename T, যৌক্তিক conjB> স্থির অন্তঃরেখ
শূন্য mulSpectrums_processRow_inplaceA(ধ্রুবক T* তথ্য_B, T* তথ্য_AC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(AC, B, AC);
}
template <typename T, যৌক্তিক conjB, যৌক্তিক inplaceA> স্থির অন্তঃরেখ
শূন্য mulSpectrums_processRow(ধ্রুবক T* তথ্য_A, ধ্রুবক T* তথ্য_B, T* তথ্য_C, size_t j0, size_t j1)
{
    যদি (inplaceA)
        mulSpectrums_processRow_inplaceA<T, conjB>(তথ্য_B, তথ্য_C, j0, j1);
    অন্যথা
        mulSpectrums_processRow_noinplace<T, conjB>(তথ্য_A, তথ্য_B, তথ্য_C, j0, j1);
}
#undef MUL_SPECTRUMS_ROW
#undef VAL

template <typename T, যৌক্তিক conjB, যৌক্তিক inplaceA> স্থির অন্তঃরেখ
শূন্য mulSpectrums_processRows(ধ্রুবক T* তথ্য_A, ধ্রুবক T* তথ্য_B, T* তথ্য_C, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, যৌক্তিক is_1d_CN1)
{
    যখন (rows-- > 0)
    {
        যদি (is_1d_CN1)
            তথ্য_C[0] = তথ্য_A[0]*তথ্য_B[0];
        mulSpectrums_processRow<T, conjB, inplaceA>(তথ্য_A, তথ্য_B, তথ্য_C, j0, j1);
        যদি (is_1d_CN1 && (cols & 1) == 0)
            তথ্য_C[j1] = তথ্য_A[j1]*তথ্য_B[j1];

        তথ্য_A = (ধ্রুবক T*)(((অক্ষর*)তথ্য_A) + stepA);
        তথ্য_B = (ধ্রুবক T*)(((অক্ষর*)তথ্য_B) + stepB);
        তথ্য_C =       (T*)(((অক্ষর*)তথ্য_C) + stepC);
    }
}


template <typename T, যৌক্তিক conjB, যৌক্তিক inplaceA> স্থির অন্তঃরেখ
শূন্য mulSpectrums_Impl_(ধ্রুবক T* তথ্য_A, ধ্রুবক T* তথ্য_B, T* তথ্য_C, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, যৌক্তিক is_1d, যৌক্তিক isCN1)
{
    যদি (!is_1d && isCN1)
    {
        mulSpectrums_processCols<T, conjB, inplaceA>(তথ্য_A, তথ্য_B, তথ্য_C, stepA, stepB, stepC, rows, cols);
    }
    mulSpectrums_processRows<T, conjB, inplaceA>(তথ্য_A, তথ্য_B, তথ্য_C, stepA, stepB, stepC, rows, cols, j0, j1, is_1d && isCN1);
}
template <typename T, যৌক্তিক conjB> স্থির অন্তঃরেখ
শূন্য mulSpectrums_Impl(ধ্রুবক T* তথ্য_A, ধ্রুবক T* তথ্য_B, T* তথ্য_C, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, যৌক্তিক is_1d, যৌক্তিক isCN1)
{
    যদি (তথ্য_A == তথ্য_C)
        mulSpectrums_Impl_<T, conjB, সত্য>(তথ্য_A, তথ্য_B, তথ্য_C, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
    অন্যথা
        mulSpectrums_Impl_<T, conjB, মিথ্যা>(তথ্য_A, তথ্য_B, তথ্য_C, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
}

} // namespace

শূন্য cv::mulSpectrums( প্রবেশ_Array _srcA, প্রবেশ_Array _srcB,
                       OutputArray _dst, পূর্ণসংখ্যা flags, যৌক্তিক conjB )
{
    CV_INSTRUMENT_REGION();

    যদি (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        ফেরত;
    }

    পূর্ণসংখ্যা type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_RUN(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    পূর্ণসংখ্যা depth = srcA.depth(), cn = srcA.channels();
    size_t rows = srcA.rows, cols = srcA.cols;

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    যদি (dst.তথ্য == srcB.তথ্য)
        srcB = srcB.clone(); // workaround for B only

    যৌক্তিক is_1d = (flags & DFT_ROWS)
        || (rows == 1)
        || (cols == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    যদি( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    যৌক্তিক isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = cols*cn - (((cols & 1) == 0 && cn == 1) ? 1 : 0);

    যদি (depth == CV_32F)
    {
        ধ্রুবক দশমিক* তথ্য_A = srcA.ptr<দশমিক>();
        ধ্রুবক দশমিক* তথ্য_B = srcB.ptr<দশমিক>();
        দশমিক* তথ্য_C = dst.ptr<দশমিক>();
        যদি (!conjB)
            mulSpectrums_Impl<দশমিক, মিথ্যা>(তথ্য_A, তথ্য_B, তথ্য_C, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        অন্যথা
            mulSpectrums_Impl<দশমিক, সত্য>(তথ্য_A, তথ্য_B, তথ্য_C, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
    অন্যথা
    {
        ধ্রুবক দ্বিগুণ* তথ্য_A = srcA.ptr<দ্বিগুণ>();
        ধ্রুবক দ্বিগুণ* তথ্য_B = srcB.ptr<দ্বিগুণ>();
        দ্বিগুণ* তথ্য_C = dst.ptr<দ্বিগুণ>();
        যদি (!conjB)
            mulSpectrums_Impl<দ্বিগুণ, মিথ্যা>(তথ্য_A, তথ্য_B, তথ্য_C, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        অন্যথা
            mulSpectrums_Impl<দ্বিগুণ, সত্য>(তথ্য_A, তথ্য_B, তথ্য_C, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
}

শূন্য cv::divSpectrums( প্রবেশ_Array _srcA, প্রবেশ_Array _srcB, OutputArray _dst, পূর্ণসংখ্যা flags, যৌক্তিক conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    যদি (srcA.empty() && srcB.empty())
    {
        _dst.release();
        ফেরত;
    }
    পূর্ণসংখ্যা depth = srcA.depth(), cn = srcA.channels(), type = srcA.type();
    পূর্ণসংখ্যা rows = srcA.rows, cols = srcA.cols;
    পূর্ণসংখ্যা j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.তথ্য != srcA.তথ্য); // non-inplace check
    CV_Assert(dst.তথ্য != srcB.তথ্য); // non-inplace check

    যৌক্তিক is_1d = (flags & DFT_ROWS) || (rows == 1 || (cols == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    যদি( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    পূর্ণসংখ্যা ncols = cols*cn;
    পূর্ণসংখ্যা j0 = cn == 1;
    পূর্ণসংখ্যা j1 = ncols - (cols % 2 == 0 && cn == 1);

    যদি( depth == CV_32F )
    {
        ধ্রুবক দশমিক* তথ্য_A = srcA.ptr<দশমিক>();
        ধ্রুবক দশমিক* তথ্য_B = srcB.ptr<দশমিক>();
        দশমিক* তথ্য_C = dst.ptr<দশমিক>();
        দশমিক eps = FLT_EPSILON; // prevent div0 problems

        size_t stepA = srcA.step/আকার(তথ্য_A[0]);
        size_t stepB = srcB.step/আকার(তথ্য_B[0]);
        size_t stepC = dst.step/আকার(তথ্য_C[0]);

        যদি( !is_1d && cn == 1 )
        {
            জন্য( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                যদি( k == 1 )
                    তথ্য_A += cols - 1, তথ্য_B += cols - 1, তথ্য_C += cols - 1;
                তথ্য_C[0] = তথ্য_A[0] / (তথ্য_B[0] + eps);
                যদি( rows % 2 == 0 )
                    তথ্য_C[(rows-1)*stepC] = তথ্য_A[(rows-1)*stepA] / (তথ্য_B[(rows-1)*stepB] + eps);
                যদি( !conjB )
                    জন্য( j = 1; j <= rows - 2; j += 2 )
                    {
                        দ্বিগুণ denom = (দ্বিগুণ)তথ্য_B[j*stepB]*তথ্য_B[j*stepB] +
                        (দ্বিগুণ)তথ্য_B[(j+1)*stepB]*তথ্য_B[(j+1)*stepB] + (দ্বিগুণ)eps;

                        দ্বিগুণ re = (দ্বিগুণ)তথ্য_A[j*stepA]*তথ্য_B[j*stepB] +
                        (দ্বিগুণ)তথ্য_A[(j+1)*stepA]*তথ্য_B[(j+1)*stepB];

                        দ্বিগুণ im = (দ্বিগুণ)তথ্য_A[(j+1)*stepA]*তথ্য_B[j*stepB] -
                        (দ্বিগুণ)তথ্য_A[j*stepA]*তথ্য_B[(j+1)*stepB];

                        তথ্য_C[j*stepC] = (দশমিক)(re / denom);
                        তথ্য_C[(j+1)*stepC] = (দশমিক)(im / denom);
                    }
                    অন্যথা
                        জন্য( j = 1; j <= rows - 2; j += 2 )
                        {

                            দ্বিগুণ denom = (দ্বিগুণ)তথ্য_B[j*stepB]*তথ্য_B[j*stepB] +
                            (দ্বিগুণ)তথ্য_B[(j+1)*stepB]*তথ্য_B[(j+1)*stepB] + (দ্বিগুণ)eps;

                            দ্বিগুণ re = (দ্বিগুণ)তথ্য_A[j*stepA]*তথ্য_B[j*stepB] -
                            (দ্বিগুণ)তথ্য_A[(j+1)*stepA]*তথ্য_B[(j+1)*stepB];

                            দ্বিগুণ im = (দ্বিগুণ)তথ্য_A[(j+1)*stepA]*তথ্য_B[j*stepB] +
                            (দ্বিগুণ)তথ্য_A[j*stepA]*তথ্য_B[(j+1)*stepB];

                            তথ্য_C[j*stepC] = (দশমিক)(re / denom);
                            তথ্য_C[(j+1)*stepC] = (দশমিক)(im / denom);
                        }
                    যদি( k == 1 )
                        তথ্য_A -= cols - 1, তথ্য_B -= cols - 1, তথ্য_C -= cols - 1;
            }
        }

        জন্য( ; rows--; তথ্য_A += stepA, তথ্য_B += stepB, তথ্য_C += stepC )
        {
            যদি( is_1d && cn == 1 )
            {
                তথ্য_C[0] = তথ্য_A[0] / (তথ্য_B[0] + eps);
                যদি( cols % 2 == 0 )
                    তথ্য_C[j1] = তথ্য_A[j1] / (তথ্য_B[j1] + eps);
            }

            যদি( !conjB )
                জন্য( j = j0; j < j1; j += 2 )
                {
                    দ্বিগুণ denom = (দ্বিগুণ)তথ্য_B[j]*তথ্য_B[j] + (দ্বিগুণ)তথ্য_B[j+1]*তথ্য_B[j+1] + (দ্বিগুণ)eps;
                    দ্বিগুণ re = (দ্বিগুণ)তথ্য_A[j]*তথ্য_B[j] + (দ্বিগুণ)তথ্য_A[j+1]*তথ্য_B[j+1];
                    দ্বিগুণ im = (দ্বিগুণ)তথ্য_A[j+1]*তথ্য_B[j] - (দ্বিগুণ)তথ্য_A[j]*তথ্য_B[j+1];
                    তথ্য_C[j] = (দশমিক)(re / denom);
                    তথ্য_C[j+1] = (দশমিক)(im / denom);
                }
                অন্যথা
                    জন্য( j = j0; j < j1; j += 2 )
                    {
                        দ্বিগুণ denom = (দ্বিগুণ)তথ্য_B[j]*তথ্য_B[j] + (দ্বিগুণ)তথ্য_B[j+1]*তথ্য_B[j+1] + (দ্বিগুণ)eps;
                        দ্বিগুণ re = (দ্বিগুণ)তথ্য_A[j]*তথ্য_B[j] - (দ্বিগুণ)তথ্য_A[j+1]*তথ্য_B[j+1];
                        দ্বিগুণ im = (দ্বিগুণ)তথ্য_A[j+1]*তথ্য_B[j] + (দ্বিগুণ)তথ্য_A[j]*তথ্য_B[j+1];
                        তথ্য_C[j] = (দশমিক)(re / denom);
                        তথ্য_C[j+1] = (দশমিক)(im / denom);
                    }
        }
    }
    অন্যথা
    {
        ধ্রুবক দ্বিগুণ* তথ্য_A = srcA.ptr<দ্বিগুণ>();
        ধ্রুবক দ্বিগুণ* তথ্য_B = srcB.ptr<দ্বিগুণ>();
        দ্বিগুণ* তথ্য_C = dst.ptr<দ্বিগুণ>();
        দ্বিগুণ eps = DBL_EPSILON; // prevent div0 problems

        size_t stepA = srcA.step/আকার(তথ্য_A[0]);
        size_t stepB = srcB.step/আকার(তথ্য_B[0]);
        size_t stepC = dst.step/আকার(তথ্য_C[0]);

        যদি( !is_1d && cn == 1 )
        {
            জন্য( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                যদি( k == 1 )
                    তথ্য_A += cols - 1, তথ্য_B += cols - 1, তথ্য_C += cols - 1;
                তথ্য_C[0] = তথ্য_A[0] / (তথ্য_B[0] + eps);
                যদি( rows % 2 == 0 )
                    তথ্য_C[(rows-1)*stepC] = তথ্য_A[(rows-1)*stepA] / (তথ্য_B[(rows-1)*stepB] + eps);
                যদি( !conjB )
                    জন্য( j = 1; j <= rows - 2; j += 2 )
                    {
                        দ্বিগুণ denom = তথ্য_B[j*stepB]*তথ্য_B[j*stepB] +
                        তথ্য_B[(j+1)*stepB]*তথ্য_B[(j+1)*stepB] + eps;

                        দ্বিগুণ re = তথ্য_A[j*stepA]*তথ্য_B[j*stepB] +
                        তথ্য_A[(j+1)*stepA]*তথ্য_B[(j+1)*stepB];

                        দ্বিগুণ im = তথ্য_A[(j+1)*stepA]*তথ্য_B[j*stepB] -
                        তথ্য_A[j*stepA]*তথ্য_B[(j+1)*stepB];

                        তথ্য_C[j*stepC] = re / denom;
                        তথ্য_C[(j+1)*stepC] = im / denom;
                    }
                    অন্যথা
                        জন্য( j = 1; j <= rows - 2; j += 2 )
                        {
                            দ্বিগুণ denom = তথ্য_B[j*stepB]*তথ্য_B[j*stepB] +
                            তথ্য_B[(j+1)*stepB]*তথ্য_B[(j+1)*stepB] + eps;

                            দ্বিগুণ re = তথ্য_A[j*stepA]*তথ্য_B[j*stepB] -
                            তথ্য_A[(j+1)*stepA]*তথ্য_B[(j+1)*stepB];

                            দ্বিগুণ im = তথ্য_A[(j+1)*stepA]*তথ্য_B[j*stepB] +
                            তথ্য_A[j*stepA]*তথ্য_B[(j+1)*stepB];

                            তথ্য_C[j*stepC] = re / denom;
                            তথ্য_C[(j+1)*stepC] = im / denom;
                        }
                    যদি( k == 1 )
                        তথ্য_A -= cols - 1, তথ্য_B -= cols - 1, তথ্য_C -= cols - 1;
            }
        }

        জন্য( ; rows--; তথ্য_A += stepA, তথ্য_B += stepB, তথ্য_C += stepC )
        {
            যদি( is_1d && cn == 1 )
            {
                তথ্য_C[0] = তথ্য_A[0] / (তথ্য_B[0] + eps);
                যদি( cols % 2 == 0 )
                    তথ্য_C[j1] = তথ্য_A[j1] / (তথ্য_B[j1] + eps);
            }

            যদি( !conjB )
                জন্য( j = j0; j < j1; j += 2 )
                {
                    দ্বিগুণ denom = তথ্য_B[j]*তথ্য_B[j] + তথ্য_B[j+1]*তথ্য_B[j+1] + eps;
                    দ্বিগুণ re = তথ্য_A[j]*তথ্য_B[j] + তথ্য_A[j+1]*তথ্য_B[j+1];
                    দ্বিগুণ im = তথ্য_A[j+1]*তথ্য_B[j] - তথ্য_A[j]*তথ্য_B[j+1];
                    তথ্য_C[j] = re / denom;
                    তথ্য_C[j+1] = im / denom;
                }
                অন্যথা
                    জন্য( j = j0; j < j1; j += 2 )
                    {
                        দ্বিগুণ denom = তথ্য_B[j]*তথ্য_B[j] + তথ্য_B[j+1]*তথ্য_B[j+1] + eps;
                        দ্বিগুণ re = তথ্য_A[j]*তথ্য_B[j] - তথ্য_A[j+1]*তথ্য_B[j+1];
                        দ্বিগুণ im = তথ্য_A[j+1]*তথ্য_B[j] + তথ্য_A[j]*তথ্য_B[j+1];
                        তথ্য_C[j] = re / denom;
                        তথ্য_C[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

নামাকাশ cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
template<typename T> স্থির শূন্য
DCT( ধ্রুবক OcvDftOptions & c, ধ্রুবক T* src, size_t src_step, T* dst, size_t dst_step )
{
    যদি( c.n == 1 )
    {
        dst[0] = src[0];
        ফেরত;
    }
    runDft(c, src, src_step/আকার(T), dst, dst_step/আকার(T), মিথ্যা);
}

template<typename T> স্থির শূন্য
IDCT( ধ্রুবক OcvDftOptions & c, ধ্রুবক T* src, size_t src_step, T* dst, size_t dst_step )
{
    যদি( c.n == 1 )
    {
        dst[0] = src[0];
        ফেরত;
    }
    runDft(c, src, src_step/আকার(T), dst, dst_step/আকার(T), মিথ্যা);
}

প্রকারলেখ শূন্য (*DCTFunc)(ধ্রুবক OcvDftOptions & c, ধ্রুবক শূন্য* src, size_t src_step,
                        শূন্য* dst, size_t dst_step);

template<typename T, শূন্য (*fn)(ধ্রুবক OcvDftOptions&, ধ্রুবক T*, size_t, T*, size_t)>
স্থির শূন্য dctWrap(ধ্রুবক OcvDftOptions & c, ধ্রুবক শূন্য* src, size_t src_step, শূন্য* dst, size_t dst_step)
{
    fn(c, (ধ্রুবক T*)src, src_step, (T*)dst, dst_step);
}

}

#ifdef HAVE_IPP
নামাকাশ cv
{

#যদি IPP_VERSION_X100 >= 900
প্রকারলেখ IppStatus (CV_STDCALL * ippiDCTFunc)(ধ্রুবক Ipp32f* pSrc, পূর্ণসংখ্যা srcStep, Ipp32f* pDst, পূর্ণসংখ্যা dstStep, ধ্রুবক শূন্য* pDCTSpec, Ipp8u* p_বাফার);
প্রকারলেখ IppStatus (CV_STDCALL * ippiDCTInit)(শূন্য* pDCTSpec, Ippi_আকার roi_আকার, Ipp8u* pMemInit );
প্রকারলেখ IppStatus (CV_STDCALL * ippi_DCTGet_আকার)(Ippi_আকার roi_আকার, পূর্ণসংখ্যা* p_আকার_Spec, পূর্ণসংখ্যা* p_আকার_Init, পূর্ণসংখ্যা* p_আকার_Buf);

template<typename SpecType, IppStatus (CV_STDCALL *fn)(ধ্রুবক Ipp32f*, পূর্ণসংখ্যা, Ipp32f*, পূর্ণসংখ্যা, ধ্রুবক SpecType*, Ipp8u*)>
স্থির IppStatus CV_STDCALL ippiDCTWrap(ধ্রুবক Ipp32f* pSrc, পূর্ণসংখ্যা srcStep, Ipp32f* pDst, পূর্ণসংখ্যা dstStep, ধ্রুবক শূন্য* pDCTSpec, Ipp8u* p_বাফার)
{
    ফেরত fn(pSrc, srcStep, pDst, dstStep, (ধ্রুবক SpecType*)pDCTSpec, p_বাফার);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*, Ippi_আকার, Ipp8u*)>
স্থির IppStatus CV_STDCALL ippiDCTInitWrap(শূন্য* pDCTSpec, Ippi_আকার roi_আকার, Ipp8u* pMemInit)
{
    ফেরত fn((SpecType*)pDCTSpec, roi_আকার, pMemInit);
}

template<IppStatus (CV_STDCALL *fn)(Ippi_আকার, পূর্ণসংখ্যা*, পূর্ণসংখ্যা*, পূর্ণসংখ্যা*)>
স্থির IppStatus CV_STDCALL ippi_DCTGet_আকার_Wrap(Ippi_আকার roi_আকার, পূর্ণসংখ্যা* p_আকার_Spec, পূর্ণসংখ্যা* p_আকার_Init, পূর্ণসংখ্যা* p_আকার_Buf)
{
    ফেরত fn(roi_আকার, p_আকার_Spec, p_আকার_Init, p_আকার_Buf);
}

#নতুবা_যদি IPP_VERSION_X100 >= 700
প্রকারলেখ IppStatus (CV_STDCALL * ippiDCTFunc)(ধ্রুবক Ipp32f*, পূর্ণসংখ্যা, Ipp32f*, পূর্ণসংখ্যা, ধ্রুবক শূন্য*, Ipp8u*);
প্রকারলেখ IppStatus (CV_STDCALL * ippiDCTInitAlloc)(শূন্য**, Ippi_আকার, IppHintAlgorithm);
প্রকারলেখ IppStatus (CV_STDCALL * ippiDCTFree)(শূন্য* pDCTSpec);
প্রকারলেখ IppStatus (CV_STDCALL * ippi_DCTGet_Buf_আকার)(ধ্রুবক শূন্য*, পূর্ণসংখ্যা*);

template<typename SpecType, IppStatus (CV_STDCALL *fn)(ধ্রুবক Ipp32f*, পূর্ণসংখ্যা, Ipp32f*, পূর্ণসংখ্যা, ধ্রুবক SpecType*, Ipp8u*)>
স্থির IppStatus CV_STDCALL ippiDCTWrap(ধ্রুবক Ipp32f* pSrc, পূর্ণসংখ্যা srcStep, Ipp32f* pDst, পূর্ণসংখ্যা dstStep, ধ্রুবক শূন্য* pDCTSpec, Ipp8u* p_বাফার)
{
    ফেরত fn(pSrc, srcStep, pDst, dstStep, (ধ্রুবক SpecType*)pDCTSpec, p_বাফার);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType**, Ippi_আকার, IppHintAlgorithm)>
স্থির IppStatus CV_STDCALL ippiDCTInitAllocWrap(শূন্য** pDCTSpec, Ippi_আকার roi_আকার, IppHintAlgorithm hint)
{
    ফেরত fn((SpecType**)pDCTSpec, roi_আকার, hint);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*)>
স্থির IppStatus CV_STDCALL ippiDCTFreeWrap(শূন্য* pDCTSpec)
{
    ফেরত fn((SpecType*)pDCTSpec);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(ধ্রুবক SpecType*, পূর্ণসংখ্যা*)>
স্থির IppStatus CV_STDCALL ippi_DCTGet_Buf_আকার_Wrap(ধ্রুবক শূন্য* pDCTSpec, পূর্ণসংখ্যা* p_আকার)
{
    ফেরত fn((ধ্রুবক SpecType*)pDCTSpec, p_আকার);
}
#endif

শ্রেণী DctIPPLoop_Invoker : সর্বজনীন ParallelLoopBody
{
সর্বজনীন:
    DctIPPLoop_Invoker(ধ্রুবক uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, পূর্ণসংখ্যা _width, যৌক্তিক _inv, যৌক্তিক *_ok) :
        ParallelLoopBody(), src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width), inv(_inv), ok(_ok)
    {
        *ok = সত্য;
    }

    আভাসী শূন্য operator()(ধ্রুবক Range& range) ধ্রুবক CV_OVERRIDE
    {
        যদি(*ok == মিথ্যা)
            ফেরত;

#যদি IPP_VERSION_X100 >= 900
        Ippi_আকার src_Roi_আকার = {width, 1};

        পূর্ণসংখ্যা spec_আকার    = 0;
        পূর্ণসংখ্যা init_আকার    = 0;
        পূর্ণসংখ্যা বাফার_আকার  = 0;

        Ipp8u* pDCTSpec = ফাঁকা;
        Ipp8u* p_বাফার  = ফাঁকা;
        Ipp8u* pInitBuf = ফাঁকা;

        #define IPP_RETURN              \
            যদি(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            যদি(p_বাফার)                 \
                ippFree(p_বাফার);       \
            যদি(pInitBuf)                \
                ippFree(pInitBuf);      \
            ফেরত;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_আকার  ipp_Dct_Get_আকার  = inv ? ippi_DCTGet_আকার_Wrap<ippi_DCTInv_Get_আকার_32f> : ippi_DCTGet_আকার_Wrap<ippi_DCTFwd_Get_আকার_32f>;

        যদি(ipp_Dct_Get_আকার(src_Roi_আকার, &spec_আকার, &init_আকার, &বাফার_আকার) < 0)
        {
            *ok = মিথ্যা;
            ফেরত;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_আকার);
        যদি(!pDCTSpec && spec_আকার)
        {
            *ok = মিথ্যা;
            ফেরত;
        }

        p_বাফার  = (Ipp8u*)CV_IPP_MALLOC(বাফার_আকার);
        যদি(!p_বাফার && বাফার_আকার)
        {
            *ok = মিথ্যা;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_আকার);
        যদি(!pInitBuf && init_আকার)
        {
            *ok = মিথ্যা;
            IPP_RETURN
        }

        যদি(ippDctInit(pDCTSpec, src_Roi_আকার, pInitBuf) < 0)
        {
            *ok = মিথ্যা;
            IPP_RETURN
        }

        জন্য(পূর্ণসংখ্যা i = range.start; i < range.end; ++i)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (দশমিক*)(src + src_step * i), static_cast<পূর্ণসংখ্যা>(src_step), (দশমিক*)(dst + dst_step * i), static_cast<পূর্ণসংখ্যা>(dst_step), pDCTSpec, p_বাফার) < 0)
            {
                *ok = মিথ্যা;
                IPP_RETURN
            }
        }
        IPP_RETURN
#undef IPP_RETURN
#নতুবা_যদি IPP_VERSION_X100 >= 700
        শূন্য* pDCTSpec;
        Auto_বাফার<uchar> buf;
        uchar* p_বাফার = 0;
        পূর্ণসংখ্যা buf_আকার=0;

        Ippi_আকার src_Roi_আকার = {width, 1};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_আকার ipp_Get_Buf_আকার = inv ? ippi_DCTGet_Buf_আকার_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_আকার_32f> : ippi_DCTGet_Buf_আকার_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_আকার_32f>;

        যদি (ippInitAlloc(&pDCTSpec, src_Roi_আকার, ippAlgHintNone)>=0 && ipp_Get_Buf_আকার(pDCTSpec, &buf_আকার)>=0)
        {
            buf.allocate( buf_আকার );
            p_বাফার = (uchar*)buf;

            জন্য( পূর্ণসংখ্যা i = range.start; i < range.end; ++i)
            {
                যদি(ippDctFun((দশমিক*)(src + src_step * i), static_cast<পূর্ণসংখ্যা>(src_step), (দশমিক*)(dst + dst_step * i), static_cast<পূর্ণসংখ্যা>(dst_step), pDCTSpec, (Ipp8u*)p_বাফার) < 0)
                {
                    *ok = মিথ্যা;
                    বিরতি;
                }
            }
        }
        অন্যথা
            *ok = মিথ্যা;

        যদি (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END
#অন্যথা
        CV_UNUSED(range);
        *ok = মিথ্যা;
#endif
    }

ব্যক্তিগত:
    ধ্রুবক uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    পূর্ণসংখ্যা width;
    যৌক্তিক inv;
    যৌক্তিক *ok;
};

স্থির যৌক্তিক DctIPPLoop(ধ্রুবক uchar * src, size_t src_step, uchar * dst, size_t dst_step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, যৌক্তিক inv)
{
    যৌক্তিক ok;
    parallel_for_(Range(0, height), DctIPPLoop_Invoker(src, src_step, dst, dst_step, width, inv, &ok), height/(দ্বিগুণ)(1<<4) );
    ফেরত ok;
}

স্থির যৌক্তিক ippi_DCT_32f(ধ্রুবক uchar * src, size_t src_step, uchar * dst, size_t dst_step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, যৌক্তিক inv, যৌক্তিক row)
{
    CV_INSTRUMENT_REGION_IPP();

    যদি(row)
        ফেরত DctIPPLoop(src, src_step, dst, dst_step, width, height, inv);
    অন্যথা
    {
#যদি IPP_VERSION_X100 >= 900
        Ippi_আকার src_Roi_আকার = {width, height};

        পূর্ণসংখ্যা spec_আকার    = 0;
        পূর্ণসংখ্যা init_আকার    = 0;
        পূর্ণসংখ্যা বাফার_আকার  = 0;

        Ipp8u* pDCTSpec = ফাঁকা;
        Ipp8u* p_বাফার  = ফাঁকা;
        Ipp8u* pInitBuf = ফাঁকা;

        #define IPP_RELEASE             \
            যদি(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            যদি(p_বাফার)                 \
                ippFree(p_বাফার);       \
            যদি(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_আকার  ipp_Dct_Get_আকার  = inv ? ippi_DCTGet_আকার_Wrap<ippi_DCTInv_Get_আকার_32f> : ippi_DCTGet_আকার_Wrap<ippi_DCTFwd_Get_আকার_32f>;

        যদি(ipp_Dct_Get_আকার(src_Roi_আকার, &spec_আকার, &init_আকার, &বাফার_আকার) < 0)
            ফেরত মিথ্যা;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_আকার);
        যদি(!pDCTSpec && spec_আকার)
            ফেরত মিথ্যা;

        p_বাফার  = (Ipp8u*)CV_IPP_MALLOC(বাফার_আকার);
        যদি(!p_বাফার && বাফার_আকার)
        {
            IPP_RELEASE
            ফেরত মিথ্যা;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_আকার);
        যদি(!pInitBuf && init_আকার)
        {
            IPP_RELEASE
            ফেরত মিথ্যা;
        }

        যদি(ippDctInit(pDCTSpec, src_Roi_আকার, pInitBuf) < 0)
        {
            IPP_RELEASE
            ফেরত মিথ্যা;
        }

        যদি(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (দশমিক*)src, static_cast<পূর্ণসংখ্যা>(src_step), (দশমিক*)dst, static_cast<পূর্ণসংখ্যা>(dst_step), pDCTSpec, p_বাফার) < 0)
        {
            IPP_RELEASE
            ফেরত মিথ্যা;
        }

        IPP_RELEASE
        ফেরত সত্য;
#undef IPP_RELEASE
#নতুবা_যদি IPP_VERSION_X100 >= 700
        IppStatus status;
        শূন্য* pDCTSpec;
        Auto_বাফার<uchar> buf;
        uchar* p_বাফার = 0;
        পূর্ণসংখ্যা buf_আকার=0;

        Ippi_আকার src_Roi_আকার = {width, height};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_আকার ipp_Get_Buf_আকার = inv ? ippi_DCTGet_Buf_আকার_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_আকার_32f> : ippi_DCTGet_Buf_আকার_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_আকার_32f>;

        status = ippStsErr;

        যদি (ippInitAlloc(&pDCTSpec, src_Roi_আকার, ippAlgHintNone)>=0 && ipp_Get_Buf_আকার(pDCTSpec, &buf_আকার)>=0)
        {
            buf.allocate( buf_আকার );
            p_বাফার = (uchar*)buf;

            status = ippDctFun((দশমিক*)src, static_cast<পূর্ণসংখ্যা>(src_step), (দশমিক*)dst, static_cast<পূর্ণসংখ্যা>(dst_step), pDCTSpec, (Ipp8u*)p_বাফার);
        }

        যদি (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END

        ফেরত status >= 0;
#অন্যথা
        CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(inv); CV_UNUSED(row);
        ফেরত মিথ্যা;
#endif
    }
}
}
#endif

নামাকাশ cv {

শ্রেণী OcvDctImpl CV_FINAL : সর্বজনীন hal::DCT2D
{
সর্বজনীন:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    Auto_বাফার<uchar> ws;
    যৌক্তিক isRowTransform;
    যৌক্তিক isInverse;
    যৌক্তিক isContinuous;
    পূর্ণসংখ্যা start_stage;
    পূর্ণসংখ্যা end_stage;
    পূর্ণসংখ্যা width;
    পূর্ণসংখ্যা height;
    পূর্ণসংখ্যা depth;

    শূন্য init(পূর্ণসংখ্যা _width, পূর্ণসংখ্যা _height, পূর্ণসংখ্যা _depth, পূর্ণসংখ্যা flags)
    {
        width = _width;
        height = _height;
        depth = _depth;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        স্থির DCTFunc dct_tbl[4] =
        {
            dctWrap<দশমিক, DCT<দশমিক>>,
            dctWrap<দশমিক, IDCT<দশমিক>>,
            dctWrap<দ্বিগুণ, DCT<দ্বিগুণ>>,
            dctWrap<দ্বিগুণ, IDCT<দ্বিগুণ>>
        };
        dct_func = dct_tbl[(পূর্ণসংখ্যা)isInverse + (depth == CV_64F)*2];
        opt.isComplex = মিথ্যা;
        opt.isInverse = মিথ্যা;
        opt.scale = 1.;

        যদি (isRowTransform || height == 1 || (width == 1 && isContinuous))
        {
            start_stage = end_stage = 0;
        }
        অন্যথা
        {
            start_stage = (width == 1);
            end_stage = 1;
        }
    }
    শূন্য apply(ধ্রুবক uchar *src, size_t src_step, uchar *dst, size_t dst_step) CV_OVERRIDE
    {
        CV_IPP_RUN(IPP_VERSION_X100 >= 700 && depth == CV_32F, ippi_DCT_32f(src, src_step, dst, dst_step, width, height, isInverse, isRowTransform))

        পূর্ণসংখ্যা পূর্ব_len = 0;
        পূর্ণসংখ্যা elem_আকার_2 = (depth == CV_32F) ? আকার(দশমিক) : আকার(দ্বিগুণ);
        ধ্রুবক DFTKernels& kernels = getDFTKernels(depth);

        জন্য(পূর্ণসংখ্যা stage = start_stage ; stage <= end_stage; stage++ )
        {
            ধ্রুবক uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            পূর্ণসংখ্যা len, গুনতি_2;

            যদি( stage == 0 )
            {
                len = width;
                গুনতি_2 = height;
                যদি( len == 1 && !isRowTransform )
                {
                    len = height;
                    গুনতি_2 = 1;
                }
                sstep0 = src_step;
                dstep0 = dst_step;
                sstep1 = dstep1 = elem_আকার_2;
            }
            অন্যথা
            {
                len = height;
                গুনতি_2 = width;
                sstep1 = src_step;
                dstep1 = dst_step;
                sstep0 = dstep0 = elem_আকার_2;
            }

            opt.n = len;

            যদি( len != পূর্ব_len )
            {
                যদি( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                যদি( len > 1 )
                {
                    plan.build(isInverse ? DFT_KIND_IDCT : DFT_KIND_DCT, len, depth, kernels.vlanes);
                    ws.allocate(plan.ws_bytes);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = ws.তথ্য();
                পূর্ব_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            দ্বিগুণ nstripes = dftParallelStripes(গুনতি_2, len);
            যদি( len > 1 && গুনতি_2 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                auto process_সারিগুলি = [&](ধ্রুবক Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    Auto_বাফার<uchar> ws_(plan.ws_bytes);
                    opt_.workspace = ws_.তথ্য();
                    জন্য( পূর্ণসংখ্যা i = range.start; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, গুনতি_2), process_সারিগুলি, nstripes);
            }
            অন্যথা
            {
                জন্য(unsigned i = 0; i < static_cast<unsigned>(গুনতি_2); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_step = dst_step;
        }
    }
};

কাঠামো ReplacementDCT2D : সর্বজনীন hal::DCT2D
{
    cvhalDFT *context;
    যৌক্তিক isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(মিথ্যা) {}
    যৌক্তিক init(পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, পূর্ণসংখ্যা depth, পূর্ণসংখ্যা flags)
    {
        পূর্ণসংখ্যা res = cv_hal_dctInit2D(&context, width, height, depth, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        ফেরত isInitialized;
    }
    শূন্য apply(ধ্রুবক uchar *src_তথ্য, size_t src_step, uchar *dst_তথ্য, size_t dst_step) CV_OVERRIDE
    {
        যদি (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_তথ্য, src_step, dst_তথ্য, dst_step);
        }
    }
    ~ReplacementDCT2D()
    {
        যদি (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

নামাকাশ hal {

Ptr<DCT2D> DCT2D::create(পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, পূর্ণসংখ্যা depth, পূর্ণসংখ্যা flags)
{
    {
        ReplacementDCT2D *impl = নতুন ReplacementDCT2D();
        যদি (impl->init(width, height, depth, flags))
        {
            ফেরত Ptr<DCT2D>(impl);
        }
        delete impl;
    }
    {
        OcvDctImpl *impl = নতুন OcvDctImpl();
        impl->init(width, height, depth, flags);
        ফেরত Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

শূন্য cv::dct( প্রবেশ_Array _src0, OutputArray _dst, পূর্ণসংখ্যা flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    যদি (src.empty())
    {
        _dst.release();
        ফেরত;
    }
    পূর্ণসংখ্যা type = src.type(), depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.rows, src.cols, type );
    Mat dst = _dst.getMat();

    পূর্ণসংখ্যা f = 0;
    যদি ((flags & DFT_ROWS) != 0)
        f |= CV_HAL_DFT_ROWS;
    যদি ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    যদি (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.cols, src.rows, depth, f);
    c->apply(src.তথ্য, src.step, dst.তথ্য, dst.step);
}


শূন্য cv::idct( প্রবেশ_Array src, OutputArray dst, পূর্ণসংখ্যা flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

নামাকাশ cv
{

স্থির ধ্রুবক পূর্ণসংখ্যা optimalDFTSizeTab[] = {
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

পূর্ণসংখ্যা cv::getOptimalDFTSize( পূর্ণসংখ্যা size0 )
{
    পূর্ণসংখ্যা a = 0, b = আকার(optimalDFTSizeTab)/আকার(optimalDFTSizeTab[0]) - 1;
    যদি( (unsigned)size0 >= (unsigned)optimalDFTSizeTab[b] )
        ফেরত -1;

    যখন( a < b )
    {
        পূর্ণসংখ্যা c = (a + b) >> 1;
        যদি( size0 <= optimalDFTSizeTab[c] )
            b = c;
        অন্যথা
            a = c+1;
    }

    ফেরত optimalDFTSizeTab[b];
}
/* End of file. */
