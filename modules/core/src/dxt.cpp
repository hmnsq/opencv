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

#ضمن "precomp.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_core.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "dxt.hpp"
#ضمن "dxt.simd.hpp"
#ضمن "dxt.simd_declarations.hpp"
#ضمن <map>

نطاق cv
{

#𐡄𐡍 IPP_VERSION_X100 >= 710
#define USE_IPP_DFT 1
#وإلا
#undef USE_IPP_DFT
#endif

#𐡄𐡍 defined USE_IPP_DFT
#𐡄𐡍 IPP_VERSION_X100 >= 202220
#define IPP_DISABLE_DFT32F ((depth == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#وإلا
#define IPP_DISABLE_DFT32F خطأ
#endif
#endif

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#ifdef HAVE_OPENCL
// factorization used by the OpenCL radix selection
ساكن صحيح
DFTFactorize( صحيح n, صحيح* factors )
{
    صحيح nf = 0, f, i, j;

    𐡄𐡍( n <= 5 )
    {
        factors[0] = n;
        أرجع 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    𐡄𐡍( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    لكل( f = 3; n > 1; )
    {
        صحيح d = n/f;
        𐡄𐡍( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        وإلا
        {
            f += 2;
            𐡄𐡍( f*f > n )
                اكسر;
        }
    }

    𐡄𐡍( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    لكل( i = f; i < (nf+f)/2; i++ )
        CV_SWAP( factors[i], factors[nf-i-1+f], j );

    أرجع nf;
}
#endif

#ifdef USE_IPP_DFT
ساكن IppStatus ippsDFTFwd_CToC( ثابت Complex<عائم>* src, Complex<عائم>* dst,
                             ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (ثابت Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ثابت IppsDFTSpec_C_32fc*)spec, buf);
}

ساكن IppStatus ippsDFTFwd_CToC( ثابت Complex<مزدوج>* src, Complex<مزدوج>* dst,
                             ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (ثابت Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ثابت IppsDFTSpec_C_64fc*)spec, buf);
}

ساكن IppStatus ippsDFTInv_CToC( ثابت Complex<عائم>* src, Complex<عائم>* dst,
                             ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (ثابت Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ثابت IppsDFTSpec_C_32fc*)spec, buf);
}

ساكن IppStatus ippsDFTInv_CToC( ثابت Complex<مزدوج>* src, Complex<مزدوج>* dst,
                                  ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (ثابت Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ثابت IppsDFTSpec_C_64fc*)spec, buf);
}

ساكن IppStatus ippsDFTFwd_RToPack( ثابت عائم* src, عائم* dst,
                                     ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (ثابت IppsDFTSpec_R_32f*)spec, buf);
}

ساكن IppStatus ippsDFTFwd_RToPack( ثابت مزدوج* src, مزدوج* dst,
                                     ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (ثابت IppsDFTSpec_R_64f*)spec, buf);
}

ساكن IppStatus ippsDFTInv_PackToR( ثابت عائم* src, عائم* dst,
                                     ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_32f, src, dst, (ثابت IppsDFTSpec_R_32f*)spec, buf);
}

ساكن IppStatus ippsDFTInv_PackToR( ثابت مزدوج* src, مزدوج* dst,
                                     ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_64f, src, dst, (ثابت IppsDFTSpec_R_64f*)spec, buf);
}
#endif

بنية OcvDftOptions;

تعريفنوع فراغ (*DFTFunc)(ثابت OcvDftOptions & c, ثابت فراغ* src, فراغ* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
بنية OcvDftOptions {
    مزدوج scale;
    صحيح n;

    منطقي isInverse;
    منطقي isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    منطقي useIpp;

#ifdef USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#endif

    ثابت DftPlan* plan;
    ثابت DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = خطأ;
        isComplex = خطأ;
        useIpp = خطأ;
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

ساكن DFTKernels getDFTKernelsDispatch(صحيح depth)
{
    CV_CPU_DISPATCH(getDFTKernels_, (depth), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
ساكن ثابت DFTKernels& getDFTKernels(صحيح depth)
{
    ساكن ثابت DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    ساكن ثابت DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    أرجع depth == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
بنية DftTwiddleGen
{
    صحيح M, logB, maskB;
    AutoBuffer<مزدوج, 1024> buf;
    مزدوج *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    ساكن فراغ fillTable(مزدوج* re, مزدوج* im, صحيح count, مزدوج wr, مزدوج wi, مزدوج stepAngle)
    {
        لكل (صحيح i0 = 0; i0 < count; i0 += 16)
        {
            مزدوج t0 = stepAngle*i0;
            مزدوج cr = i0 == 0 ? 1 : cos(t0), ci = i0 == 0 ? 0 : sin(t0);
            صحيح i1 = std::min(i0 + 16, count);
            لكل (صحيح i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                مزدوج t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    فراغ init(صحيح _M)
    {
        M = _M;
        logB = 0;
        طالما ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        صحيح B = 1 << logB;
        maskB = B - 1;
        صحيح nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.data(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        مزدوج a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fillTable(lo_re, lo_im, B, cos(a), sin(a), a);
        fillTable(hi_re, hi_im, nhi, cos(a*B), sin(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    فراغ get(صحيح m, مزدوج& re, مزدوج& im) ثابت
    {
        CV_DbgAssert(m >= 0);
        𐡄𐡍 (m >= M)
            m %= M;
        صحيح a = m >> logB, b = m & maskB;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

template<typename T>
فراغ DftPlan::fillTables(size_t ntw, منطقي need_rtw, منطقي need_dct)
{
    DftPlan& p = *هذا;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    صحيح M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*حجم(T) + حجم(مزدوج) - 1)/حجم(مزدوج) + 1);
    T* cur = (T*)p.tw.data();
    صحيح maxL = 1;
    لكل (صحيح s = 0; s < p.nstages; s++)
        𐡄𐡍 (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].span);
    AutoBuffer<مزدوج, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    مزدوج* twd = twd_buf.data();
    لكل (صحيح s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        صحيح r = st.radix, L = st.span, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        صحيح step = M/(r*L);
        𐡄𐡍 (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr صحيح K = 8;
            مزدوج cr[K], ci[K], wr, wi;
            gen.get(K*step, wr, wi);
            مزدوج* t1r = twd; مزدوج* t1i = twd + L;      // leg 1 kept in double for the products
            لكل (صحيح j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                لكل (صحيح k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*step, cr[k], ci[k]);
                صحيح j1 = std::min(j0 + 16*K, L);
                لكل (صحيح j = j0; j < j1; j += K)
                    لكل (صحيح k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        مزدوج t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            لكل (صحيح j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            لكل (صحيح q = 2; q < r; q++)
            {
                ثابت T* pr = twr + (q-2)*ts; ثابت T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                لكل (صحيح j = 0; j < L; j++)
                {
                    مزدوج ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        وإلا
        {
            // large odd radix: exact-ish generator lookup per entry
            لكل (صحيح q = 1; q < r; q++)
                لكل (صحيح j = 0; j < L; j++)
                {
                    مزدوج re, im;
                    gen.get(q*j*step, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        𐡄𐡍 (r >= 7)
        {
            صحيح h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            لكل (صحيح pp = 1; pp <= h; pp++)
                لكل (صحيح k = 1; k <= h; k++)
                {
                    مزدوج t = 2*CV_PI*((مزدوج)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)cos(t);
                    sn[(pp-1)*h + (k-1)] = (T)sin(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    𐡄𐡍 (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        صحيح step = M/n;
        لكل (صحيح k = 0; k <= nc; k++)
        {
            مزدوج re, im;
            gen.get(k*step, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    𐡄𐡍 (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        مزدوج scale = (p.kind == DFT_KIND_DCT ? 2 : 1)*std::sqrt(1./(2*n));
        صحيح step = M/(4*n);
        لكل (صحيح k = 0; k <= nc; k++)
        {
            مزدوج re, im;
            gen.get(k*step, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
فراغ DftPlan::build(صحيح _kind, صحيح _n, صحيح _depth, صحيح _vl)
{
    DftPlan& p = *هذا;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(_depth == CV_32F || _depth == CV_64F);
    منطقي real_kind = _kind != DFT_KIND_C2C;
    منطقي dct_kind = _kind == DFT_KIND_DCT || _kind == DFT_KIND_IDCT;
    𐡄𐡍 (dct_kind)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.kind = _kind; p.n = _n; p.depth = _depth; p.vl = _vl;
    p.nc = real_kind && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = depth == CV_32F ? حجم(عائم) : حجم(مزدوج);
    p.real_input = kind == DFT_KIND_R2C && (n & 1) != 0;
    p.need_tmp = kind == DFT_KIND_C2R || kind == DFT_KIND_DCT || kind == DFT_KIND_IDCT;
    p.pingpong = خطأ;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    صحيح k2 = 0, rest = nc;
    طالما ((rest & 1) == 0) { rest >>= 1; k2++; }
    صحيح r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.first_radix = r0;
    صحيح radices[DFT_MAX_STAGES], nst = 0;
    لكل (صحيح m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    طالما (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    طالما (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    لكل (صحيح f = 7; rest > 1; f += 2)
    {
        𐡄𐡍 (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            اكسر;
        }
        طالما (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    صحيح L = r0, max_h = 0;
    size_t ntw = 0;
    لكل (صحيح s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        صحيح r = radices[s];
        st.radix = r; st.span = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        𐡄𐡍 (vl > 1 && L >= vl)
        {
            st.mode = DFT_STAGE_FULL;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            𐡄𐡍 (L % vl != 0)
                p.pingpong = 𐡒𐡔𐡈;   // back-off clamp => overlapping stores => must be out-of-place
        }
        وإلا
        {
            st.mode = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        𐡄𐡍 (r >= 7)
        {
            صحيح h = (r - 1)/2;
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
    صحيح seq[DFT_MAX_STAGES + 1], nseq = 0;
    𐡄𐡍 (r0 > 1) seq[nseq++] = r0;
    لكل (صحيح s = 0; s < nst; s++) seq[nseq++] = radices[s];
    صحيح digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        صحيح Ls = 1;
        لكل (صحيح s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    𐡄𐡍 (r0 == 1)
    {
        p.itab.allocate(nc*2);
        صحيح* it_all = p.itab.data();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        صحيح n_in = std::min(nseq, 2);
        صحيح r_0 = n_in > 0 ? seq[0] : 1, w_0 = n_in > 0 ? weights[0] : 0;
        صحيح r_1 = n_in > 1 ? seq[1] : 1, w_1 = n_in > 1 ? weights[1] : 0;
        لكل (صحيح pos = 0, rev_hi = 0; pos < nc; )
        {
            لكل (صحيح d1 = 0; d1 < r_1; d1++)
            {
                صحيح rev = rev_hi + d1*w_1;
                صحيح* it = it_all + pos*2;
                𐡄𐡍 (p.real_input)
                {
                    لكل (صحيح d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                وإلا
                {
                    لكل (صحيح d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            لكل (صحيح s = n_in; s < nseq; s++)
            {
                rev_hi += weights[s];
                𐡄𐡍 (++digits[s] < seq[s])
                    اكسر;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    وإلا
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        صحيح M = nc/r0;
        p.itab.allocate(M);
        صحيح* gtab = p.itab.data();
        صحيح gw[DFT_MAX_STAGES + 1];
        {
            صحيح Ls = 1;
            لكل (صحيح s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        صحيح sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        صحيح r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        صحيح r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        لكل (صحيح base = 0, g_hi = 0; base < M; )
        {
            لكل (صحيح db = 0; db < r_b; db++)
            {
                صحيح g = (g_hi + db*g_b)*r0;
                لكل (صحيح da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[base++] = g;
            }
            لكل (صحيح s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                𐡄𐡍 (++digits[s] < seq[s])
                    اكسر;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    منطقي need_rtw = real_kind && n % 2 == 0;
    𐡄𐡍 (need_rtw) ntw += 2*(size_t)(nc + 1);
    𐡄𐡍 (dct_kind) ntw += 2*(size_t)(nc + 1);
    𐡄𐡍 (depth == CV_32F)
        fillTables<عائم>(ntw, need_rtw, dct_kind);
    وإلا
        fillTables<مزدوج>(ntw, need_rtw, dct_kind);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_bytes = 2*(size_t)nc*esz;
    منطقي two_pairs = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_pairs ? pair_bytes : 0;
    p.scratch_ofs = pair_bytes*(two_pairs ? 2 : 1);
    size_t scratch_bytes = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_bytes = p.scratch_ofs + scratch_bytes + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
ساكن فراغ runDft(ثابت OcvDftOptions& c, ثابت فراغ* src, size_t sstep, فراغ* dst, size_t dstep,
                   منطقي complex_io)
{
    ثابت DftPlan& p = *c.plan;
    ثابت DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.depth == CV_32F ? حجم(عائم) : حجم(مزدوج);
    uchar* base = alignPtr(c.workspace, 64);
    uchar* re0 = base;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = base + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = base + p.scratch_ofs;

    ثابت فراغ* asrc = src;
    size_t astep = sstep;
    منطقي conj = خطأ;
    𐡄𐡍 (p.kind == DFT_KIND_C2R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = 𐡒𐡔𐡈;
    }
    وإلا 𐡄𐡍 (p.kind == DFT_KIND_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = 𐡒𐡔𐡈;
    }
    وإلا 𐡄𐡍 (p.kind == DFT_KIND_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    وإلا 𐡄𐡍 (p.kind == DFT_KIND_C2C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.first_radix == 1);

    DftPreprocFunc pre = p.first_radix == 1 ? k.preprocRadix0 : p.first_radix == 2 ? k.preprocRadix2 :
                         p.first_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    لكل (صحيح i = 0; i < p.nstages; i++)
    {
        ثابت DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        𐡄𐡍 (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        وإلا
            fn(st, sre, sim, sre, sim, scratch);
    }

    مزدوج scale = c.scale;
    بدل (p.kind)
    {
    حالة DFT_KIND_C2C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        اكسر;
    حالة DFT_KIND_R2C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        اكسر;
    حالة DFT_KIND_C2R:
        𐡄𐡍 (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        وإلا
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        اكسر;
    حالة DFT_KIND_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        اكسر;
    افتراضي:
        CV_Assert(p.kind == DFT_KIND_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
template<typename T> ساكن فراغ
DFT(ثابت OcvDftOptions & c, ثابت Complex<T>* src, Complex<T>* dst)
{
    𐡄𐡍( c.useIpp )
    {
#ifdef USE_IPP_DFT
        𐡄𐡍( !c.isInverse )
        {
            𐡄𐡍 (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                أرجع;
            }
        }
        وإلا
        {
            𐡄𐡍 (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                أرجع;
            }
        }
        setIppErrorStatus();
#endif
    }

    runDft(c, src, 1, dst, 1, خطأ);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
template<typename T> ساكن فراغ
RealDFT(ثابت OcvDftOptions & c, ثابت T* src, T* dst)
{
    صحيح complex_output = c.isComplex;

    𐡄𐡍( c.useIpp )
    {
#ifdef USE_IPP_DFT
        صحيح n = c.n;
        T* ipp_dst = dst + complex_output;
        𐡄𐡍 (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            𐡄𐡍( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                𐡄𐡍( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_ADD(CV_IMPL_IPP);
            أرجع;
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
template<typename T> ساكن فراغ
CCSIDFT(ثابت OcvDftOptions & c, ثابت T* src, T* dst)
{
    صحيح complex_input = c.isComplex;

    𐡄𐡍( complex_input )
        CV_Assert( src != dst );
    𐡄𐡍( c.useIpp )
    {
#ifdef USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        ثابت T* ipp_src = src;
        T save_s1 = 0;
        𐡄𐡍( complex_input )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        IppStatus status = ippsDFTInv_PackToR( ipp_src, dst, c.ipp_spec, c.ipp_work );
        𐡄𐡍( complex_input )
            ((T*)src)[1] = save_s1;
        𐡄𐡍( status >= 0 )
        {
            CV_IMPL_ADD(CV_IMPL_IPP);
            أرجع;
        }
        setIppErrorStatus();
#endif
    }
    runDft(c, src, 1, dst, 1, complex_input != 0);
}

ساكن فراغ
CopyColumn( ثابت uchar* _src, size_t src_step,
            uchar* _dst, size_t dst_step,
            صحيح len, size_t elem_size )
{
    صحيح i, t0, t1;
    ثابت صحيح* src = (ثابت صحيح*)_src;
    صحيح* dst = (صحيح*)_dst;
    src_step /= حجم(src[0]);
    dst_step /= حجم(dst[0]);

    𐡄𐡍( elem_size == حجم(صحيح) )
    {
        لكل( i = 0; i < len; i++, src += src_step, dst += dst_step )
            dst[0] = src[0];
    }
    وإلا 𐡄𐡍( elem_size == حجم(صحيح)*2 )
    {
        لكل( i = 0; i < len; i++, src += src_step, dst += dst_step )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    وإلا 𐡄𐡍( elem_size == حجم(صحيح)*4 )
    {
        لكل( i = 0; i < len; i++, src += src_step, dst += dst_step )
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
template<typename T> ساكن مضمن فراغ
copyFromColumns_( ثابت uchar* src, size_t src_step, T* dst, صحيح ncols, صحيح len )
{
    لكل( صحيح i = 0; i < len; i++, src += src_step )
    {
        ثابت T* s = (ثابت T*)src;
        لكل( صحيح k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

ساكن فراغ
CopyFromColumns( ثابت uchar* src, size_t src_step, uchar* dst, صحيح ncols, صحيح len, size_t elem_size )
{
    𐡄𐡍( elem_size == حجم(صحيح) )
        copyFromColumns_(src, src_step, (صحيح*)dst, ncols, len);
    وإلا 𐡄𐡍( elem_size == حجم(int64) )
        copyFromColumns_(src, src_step, (int64*)dst, ncols, len);
    وإلا
    {
        CV_Assert( elem_size == حجم(int64)*2 );
        copyFromColumns_(src, src_step, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
template<typename T> ساكن مضمن فراغ
copyToColumns_( ثابت T* src, uchar* dst, size_t dst_step, صحيح ncols, صحيح len )
{
    لكل( صحيح i = 0; i < len; i++, dst += dst_step )
    {
        T* d = (T*)dst;
        لكل( صحيح k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

ساكن فراغ
CopyToColumns( ثابت uchar* src, uchar* dst, size_t dst_step, صحيح ncols, صحيح len, size_t elem_size )
{
    𐡄𐡍( elem_size == حجم(صحيح) )
        copyToColumns_((ثابت صحيح*)src, dst, dst_step, ncols, len);
    وإلا 𐡄𐡍( elem_size == حجم(int64) )
        copyToColumns_((ثابت int64*)src, dst, dst_step, ncols, len);
    وإلا
    {
        CV_Assert( elem_size == حجم(int64)*2 );
        copyToColumns_((ثابت Vec<int64, 2>*)src, dst, dst_step, ncols, len);
    }
}

ساكن فراغ
ExpandCCS( uchar* _ptr, صحيح n, صحيح elem_size )
{
    صحيح i;
    𐡄𐡍( elem_size == (صحيح)حجم(عائم) )
    {
        عائم* p = (عائم*)_ptr;
        لكل( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        𐡄𐡍( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        لكل( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    وإلا
    {
        مزدوج* p = (مزدوج*)_ptr;
        لكل( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        𐡄𐡍( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        لكل( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

template<typename T, فراغ (*fn)(ثابت OcvDftOptions&, ثابت T*, T*)>
ساكن فراغ dftWrap(ثابت OcvDftOptions & c, ثابت فراغ* src, فراغ* dst)
{
    fn(c, (ثابت T*)src, (T*)dst);
}

}

#ifdef USE_IPP_DFT
تعريفنوع IppStatus (CV_STDCALL* IppDFTGetSizeFunc)(صحيح, صحيح, IppHintAlgorithm, صحيح*, صحيح*, صحيح*);
تعريفنوع IppStatus (CV_STDCALL* IppDFTInitFunc)(صحيح, صحيح, IppHintAlgorithm, فراغ*, uchar*);

template<typename SpecType, IppStatus (CV_STDCALL *init_fn)(صحيح, صحيح, IppHintAlgorithm, SpecType*, Ipp8u*)>
ساكن IppStatus CV_STDCALL ippDFTInitWrap(صحيح n, صحيح flags, IppHintAlgorithm hint, فراغ* spec, Ipp8u* initbuf)
{
    أرجع init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#endif

نطاق cv
{
#𐡄𐡍 defined USE_IPP_DFT

تعريفنوع IppStatus (CV_STDCALL* ippiDFT_C_Func)(ثابت Ipp32fc*, صحيح, Ipp32fc*, صحيح, ثابت IppiDFTSpec_C_32fc*, Ipp8u*);
تعريفنوع IppStatus (CV_STDCALL* ippiDFT_R_Func)(ثابت Ipp32f* , صحيح, Ipp32f* , صحيح, ثابت IppiDFTSpec_R_32f* , Ipp8u*);

template <typename Dft>
صنف Dft_C_IPPLoop_Invoker : عام ParallelLoopBody
{
عام:

    Dft_C_IPPLoop_Invoker(ثابت uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, صحيح _width,
                          ثابت Dft& _ippidft, صحيح _norm_flag, منطقي *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = 𐡒𐡔𐡈;
    }

    virtual فراغ operator()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* pBuffer = 0;
        Ipp8u* pMemInit= 0;
        صحيح sizeBuffer=0;
        صحيح sizeSpec=0;
        صحيح sizeInit=0;

        IppiSize srcRoiSize = {width, 1};

        status = ippiDFTGetSize_C_32fc(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
        𐡄𐡍 ( status < 0 )
        {
            *ok = خطأ;
            أرجع;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( sizeSpec );

        𐡄𐡍 ( sizeInit > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

        𐡄𐡍 ( sizeBuffer > 0 )
            pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

        status = ippiDFTInit_C_32fc( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        𐡄𐡍 ( sizeInit > 0 )
            ippFree( pMemInit );

        𐡄𐡍 ( status < 0 )
        {
            ippFree( pDFTSpec );
            𐡄𐡍 ( sizeBuffer > 0 )
                ippFree( pBuffer );
            *ok = خطأ;
            أرجع;
        }

        لكل( صحيح i = range.start; i < range.end; ++i)
            𐡄𐡍(!ippidft((Ipp32fc*)(src + src_step * i), src_step, (Ipp32fc*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)pBuffer))
            {
                *ok = خطأ;
            }

        𐡄𐡍 ( sizeBuffer > 0 )
            ippFree( pBuffer );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

خاص:
    ثابت uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    صحيح width;
    ثابت Dft& ippidft;
    صحيح norm_flag;
    منطقي *ok;

    ثابت Dft_C_IPPLoop_Invoker& operator= (ثابت Dft_C_IPPLoop_Invoker&);
};

template <typename Dft>
صنف Dft_R_IPPLoop_Invoker : عام ParallelLoopBody
{
عام:

    Dft_R_IPPLoop_Invoker(ثابت uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, صحيح _width,
                          ثابت Dft& _ippidft, صحيح _norm_flag, منطقي *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = 𐡒𐡔𐡈;
    }

    virtual فراغ operator()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* pBuffer = 0;
        Ipp8u* pMemInit= 0;
        صحيح sizeBuffer=0;
        صحيح sizeSpec=0;
        صحيح sizeInit=0;

        IppiSize srcRoiSize = {width, 1};

        status = ippiDFTGetSize_R_32f(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
        𐡄𐡍 ( status < 0 )
        {
            *ok = خطأ;
            أرجع;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( sizeSpec );

        𐡄𐡍 ( sizeInit > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

        𐡄𐡍 ( sizeBuffer > 0 )
            pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

        status = ippiDFTInit_R_32f( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        𐡄𐡍 ( sizeInit > 0 )
            ippFree( pMemInit );

        𐡄𐡍 ( status < 0 )
        {
            ippFree( pDFTSpec );
            𐡄𐡍 ( sizeBuffer > 0 )
                ippFree( pBuffer );
            *ok = خطأ;
            أرجع;
        }

        لكل( صحيح i = range.start; i < range.end; ++i)
            𐡄𐡍(!ippidft((عائم*)(src + src_step * i), src_step, (عائم*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)pBuffer))
            {
                *ok = خطأ;
            }

        𐡄𐡍 ( sizeBuffer > 0 )
            ippFree( pBuffer );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

خاص:
    ثابت uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    صحيح width;
    ثابت Dft& ippidft;
    صحيح norm_flag;
    منطقي *ok;

    ثابت Dft_R_IPPLoop_Invoker& operator= (ثابت Dft_R_IPPLoop_Invoker&);
};

template <typename Dft>
منطقي Dft_C_IPPLoop(ثابت uchar * src, size_t src_step, uchar * dst, size_t dst_step, صحيح width, صحيح height, ثابت Dft& ippidft, صحيح norm_flag)
{
    منطقي ok;
    parallel_for_(Range(0, height), Dft_C_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(مزدوج)(1<<16) );
    أرجع ok;
}

template <typename Dft>
منطقي Dft_R_IPPLoop(ثابت uchar * src, size_t src_step, uchar * dst, size_t dst_step, صحيح width, صحيح height, ثابت Dft& ippidft, صحيح norm_flag)
{
    منطقي ok;
    parallel_for_(Range(0, height), Dft_R_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(مزدوج)(1<<16) );
    أرجع ok;
}

بنية IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    منطقي operator()(ثابت Ipp32fc* src, size_t srcStep, Ipp32fc* dst, size_t dstStep, ثابت IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* pBuffer) ثابت
    {
        أرجع ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<صحيح>(srcStep), dst, static_cast<صحيح>(dstStep), pDFTSpec, pBuffer) >= 0 : خطأ;
    }
خاص:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

بنية IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippiDFT_PackToR_32f_C1R(_func){}

    منطقي operator()(ثابت Ipp32f* src, size_t srcStep, Ipp32f* dst, size_t dstStep, ثابت IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* pBuffer) ثابت
    {
        أرجع ippiDFT_PackToR_32f_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_PackToR_32f_C1R, src, static_cast<صحيح>(srcStep), dst, static_cast<صحيح>(dstStep), pDFTSpec, pBuffer) >= 0 : خطأ;
    }
خاص:
    ippiDFT_R_Func ippiDFT_PackToR_32f_C1R;
};

ساكن منطقي ippi_DFT_C_32F(ثابت uchar * src, size_t src_step, uchar * dst, size_t dst_step, صحيح width, صحيح height, منطقي inv, صحيح norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* pBuffer = 0;
    Ipp8u* pMemInit= 0;
    صحيح sizeBuffer=0;
    صحيح sizeSpec=0;
    صحيح sizeInit=0;

    IppiSize srcRoiSize = {width, height};

    status = ippiDFTGetSize_C_32fc(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
    𐡄𐡍 ( status < 0 )
        أرجع خطأ;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( sizeSpec );

    𐡄𐡍 ( sizeInit > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

    𐡄𐡍 ( sizeBuffer > 0 )
        pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

    status = ippiDFTInit_C_32fc( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    𐡄𐡍 ( sizeInit > 0 )
        ippFree( pMemInit );

    𐡄𐡍 ( status < 0 )
    {
        ippFree( pDFTSpec );
        𐡄𐡍 ( sizeBuffer > 0 )
            ippFree( pBuffer );
        أرجع خطأ;
    }

    𐡄𐡍 (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<صحيح>(src_step), (Ipp32fc*)dst, static_cast<صحيح>(dst_step), pDFTSpec, pBuffer);
    وإلا
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<صحيح>(src_step), (Ipp32fc*)dst, static_cast<صحيح>(dst_step), pDFTSpec, pBuffer);

    𐡄𐡍 ( sizeBuffer > 0 )
        ippFree( pBuffer );

    ippFree( pDFTSpec );

    𐡄𐡍(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        أرجع 𐡒𐡔𐡈;
    }
    أرجع خطأ;
}

ساكن منطقي ippi_DFT_R_32F(ثابت uchar * src, size_t src_step, uchar * dst, size_t dst_step, صحيح width, صحيح height, منطقي inv, صحيح norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* pBuffer = 0;
    Ipp8u* pMemInit= 0;
    صحيح sizeBuffer=0;
    صحيح sizeSpec=0;
    صحيح sizeInit=0;

    IppiSize srcRoiSize = {width, height};

    status = ippiDFTGetSize_R_32f(srcRoiSize, norm_flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer );
    𐡄𐡍 ( status < 0 )
        أرجع خطأ;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( sizeSpec );

    𐡄𐡍 ( sizeInit > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( sizeInit );

    𐡄𐡍 ( sizeBuffer > 0 )
        pBuffer = (Ipp8u*)CV_IPP_MALLOC( sizeBuffer );

    status = ippiDFTInit_R_32f( srcRoiSize, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    𐡄𐡍 ( sizeInit > 0 )
        ippFree( pMemInit );

    𐡄𐡍 ( status < 0 )
    {
        ippFree( pDFTSpec );
        𐡄𐡍 ( sizeBuffer > 0 )
            ippFree( pBuffer );
        أرجع خطأ;
    }

    𐡄𐡍 (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (عائم*)src, static_cast<صحيح>(src_step), (عائم*)dst, static_cast<صحيح>(dst_step), pDFTSpec, pBuffer);
    وإلا
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_PackToR_32f_C1R, (عائم*)src, static_cast<صحيح>(src_step), (عائم*)dst, static_cast<صحيح>(dst_step), pDFTSpec, pBuffer);

    𐡄𐡍 ( sizeBuffer > 0 )
        ippFree( pBuffer );

    ippFree( pDFTSpec );

    𐡄𐡍(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        أرجع 𐡒𐡔𐡈;
    }
    أرجع خطأ;
}

#endif
}

#ifdef HAVE_OPENCL

نطاق cv
{

تعداد FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

بنية OCL_FftPlan
{
خاص:
    UMat twiddles;
    String buildOptions;
    صحيح thread_count;
    صحيح dft_size;
    صحيح dft_depth;
    منطقي status;

عام:
    OCL_FftPlan(صحيح _size, صحيح _depth) : dft_size(_size), dft_depth(_depth), status(𐡒𐡔𐡈)
    {
        CV_Assert( dft_depth == CV_32F || dft_depth == CV_64F );

        صحيح min_radix;
        std::vector<صحيح> radixes, blocks;
        ocl_getRadixes(dft_size, radixes, blocks, min_radix);
        thread_count = dft_size / min_radix;

        𐡄𐡍 (thread_count > (صحيح) ocl::Device::getDefault().maxWorkGroupSize())
        {
            status = خطأ;
            أرجع;
        }

        // generate string with radix calls
        String radix_processing;
        صحيح n = 1, twiddle_size = 0;
        لكل (size_t i=0; i<radixes.size(); i++)
        {
            صحيح radix = radixes[i], block = blocks[i];
            𐡄𐡍 (block > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, block, twiddle_size, n, dft_size/radix);
            وإلا
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_size, n, dft_size/radix);
            twiddle_size += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_size, CV_MAKE_TYPE(dft_depth, 2));
        𐡄𐡍 (dft_depth == CV_32F)
            fillRadixTable<عائم>(twiddles, radixes);
        وإلا
            fillRadixTable<مزدوج>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_size, min_radix, ocl::typeToStr(dft_depth), ocl::typeToStr(CV_MAKE_TYPE(dft_depth, 2)),
                              dft_depth == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    منطقي enqueueTransform(InputArray _src, OutputArray _dst, صحيح num_dfts, صحيح flags, صحيح fftType, منطقي rows = 𐡒𐡔𐡈) ثابت
    {
        𐡄𐡍 (!status)
            أرجع خطأ;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        منطقي is1d = (flags & DFT_ROWS) != 0 || num_dfts == 1;
        منطقي inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        𐡄𐡍 (rows)
        {
            globalsize[0] = thread_count; globalsize[1] = src.rows;
            localsize[0] = thread_count; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            𐡄𐡍 ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        وإلا
        {
            globalsize[0] = num_dfts; globalsize[1] = thread_count;
            localsize[0] = 1; localsize[1] = thread_count;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            𐡄𐡍 (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        𐡄𐡍 (!inv)
        {
            𐡄𐡍 ((is1d && src.channels() == 1) || (rows && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        وإلا
        {
            𐡄𐡍 (rows && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            𐡄𐡍 (dst.cols % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        𐡄𐡍 (k.empty())
            أرجع خطأ;

        k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::ReadOnlyNoSize(twiddles), thread_count, num_dfts);
        أرجع k.run(2, globalsize, localsize, خطأ);
    }

خاص:
    ساكن فراغ ocl_getRadixes(صحيح cols, std::vector<صحيح>& radixes, std::vector<صحيح>& blocks, صحيح& min_radix)
    {
        صحيح factors[34];
        صحيح nf = DFTFactorize(cols, factors);

        صحيح n = 1;
        صحيح factor_index = 0;
        min_radix = INT_MAX;

        // 2^n transforms
        𐡄𐡍 ((factors[factor_index] & 1) == 0)
        {
            لكل( ; n < factors[factor_index];)
            {
                صحيح radix = 2, block = 1;
                𐡄𐡍 (8*n <= factors[0])
                    radix = 8;
                وإلا 𐡄𐡍 (4*n <= factors[0])
                {
                    radix = 4;
                    𐡄𐡍 (cols % 12 == 0)
                        block = 3;
                    وإلا 𐡄𐡍 (cols % 8 == 0)
                        block = 2;
                }
                وإلا
                {
                    𐡄𐡍 (cols % 10 == 0)
                        block = 5;
                    وإلا 𐡄𐡍 (cols % 8 == 0)
                        block = 4;
                    وإلا 𐡄𐡍 (cols % 6 == 0)
                        block = 3;
                    وإلا 𐡄𐡍 (cols % 4 == 0)
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
        لكل( ; factor_index < nf; factor_index++)
        {
            صحيح radix = factors[factor_index], block = 1;
            𐡄𐡍 (radix == 3)
            {
                𐡄𐡍 (cols % 12 == 0)
                    block = 4;
                وإلا 𐡄𐡍 (cols % 9 == 0)
                    block = 3;
                وإلا 𐡄𐡍 (cols % 6 == 0)
                    block = 2;
            }
            وإلا 𐡄𐡍 (radix == 5)
            {
                𐡄𐡍 (cols % 10 == 0)
                    block = 2;
            }
            radixes.push_back(radix);
            blocks.push_back(block);
            min_radix = min(min_radix, block*radix);
        }
    }

    template <typename T>
    ساكن فراغ fillRadixTable(UMat twiddles, ثابت std::vector<صحيح>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        صحيح ptr_index = 0;

        صحيح n = 1;
        لكل (size_t i=0; i<radixes.size(); i++)
        {
            صحيح radix = radixes[i];
            n *= radix;

            لكل (صحيح j=1; j<radix; j++)
            {
                مزدوج theta = -CV_2PI*j/n;

                لكل (صحيح k=0; k<(n/radix); k++)
                {
                    ptr[ptr_index++] = (T) cos(k*theta);
                    ptr[ptr_index++] = (T) sin(k*theta);
                }
            }
        }
    }
};

صنف OCL_FftPlanCache
{
عام:
    ساكن OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, جديد OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(صحيح dft_size, صحيح depth)
    {
        صحيح key = (dft_size << 16) | (depth & 0xFFFF);
        std::map<صحيح, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(key);
        𐡄𐡍 (f != planStorage.end())
        {
            أرجع f->second;
        }
        وإلا
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(جديد OCL_FftPlan(dft_size, depth));
            planStorage[key] = newPlan;
            أرجع newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

محمي:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<صحيح, Ptr<OCL_FftPlan> > planStorage;
};

ساكن منطقي ocl_dft_rows(InputArray _src, OutputArray _dst, صحيح nonzero_rows, صحيح flags, صحيح fftType)
{
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.cols(), depth);
    أرجع plan->enqueueTransform(_src, _dst, nonzero_rows, flags, fftType, 𐡒𐡔𐡈);
}

ساكن منطقي ocl_dft_cols(InputArray _src, OutputArray _dst, صحيح nonzero_cols, صحيح flags, صحيح fftType)
{
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.rows(), depth);
    أرجع plan->enqueueTransform(_src, _dst, nonzero_cols, flags, fftType, خطأ);
}

مضمن FftType determineFFTType(منطقي real_input, منطقي complex_input, منطقي real_output, منطقي complex_output, منطقي inv)
{
    // output format is not specified
    𐡄𐡍 (!real_output && !complex_output)
        complex_output = 𐡒𐡔𐡈;

    // input or output format is ambiguous
    𐡄𐡍 (real_input == complex_input || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType result = real_input ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    𐡄𐡍 (result == C2R && !inv)
        result = C2C;

    // Inverse CCS to Complex not supported
    𐡄𐡍 (result == R2C && inv)
        result = R2R;

    أرجع result;
}

ساكن منطقي ocl_dft(InputArray _src, OutputArray _dst, صحيح flags, صحيح nonzero_rows)
{
    صحيح type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    Size ssize = _src.size();
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    𐡄𐡍 (!(cn == 1 || cn == 2)
        || !(depth == CV_32F || (depth == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        أرجع خطأ;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    𐡄𐡍 (ssize.area() != getOptimalDFTSize(ssize.area()))
        أرجع خطأ;

    UMat src = _src.getUMat();
    منطقي inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    𐡄𐡍( nonzero_rows <= 0 || nonzero_rows > _src.rows() )
        nonzero_rows = _src.rows();
    منطقي is1d = (flags & DFT_ROWS) != 0 || nonzero_rows == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    𐡄𐡍 (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.createSameSize(src, CV_MAKETYPE(depth, 2));
        output = _dst.getUMat();
    }
    وإلا
    {
        // real output
        𐡄𐡍 (is1d)
        {
            _dst.createSameSize(src, CV_MAKETYPE(depth, 1));
            output = _dst.getUMat();
        }
        وإلا
        {
            _dst.createSameSize(src, CV_MAKETYPE(depth, 1));
            output.create(src.size, CV_MAKETYPE(depth, 2));
        }
    }

    منطقي result = خطأ;
    𐡄𐡍 (!inv)
    {
        صحيح nonzero_cols = fftType == R2R ? output.cols/2 + 1 : output.cols;
        result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
        𐡄𐡍 (!is1d)
            result = result && ocl_dft_cols(output, _dst, nonzero_cols, flags, fftType);
    }
    وإلا
    {
        𐡄𐡍 (fftType == C2C)
        {
            // complex output
            result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            𐡄𐡍 (!is1d)
                result = result && ocl_dft_cols(output, output, output.cols, flags, fftType);
        }
        وإلا
        {
            𐡄𐡍 (is1d)
            {
                result = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            }
            وإلا
            {
                صحيح nonzero_cols = src.cols/2 + 1;
                result = ocl_dft_cols(src, output, nonzero_cols, flags, fftType);
                result = result && ocl_dft_rows(output, _dst, nonzero_rows, flags, fftType);
            }
        }
    }
    أرجع result;
}

} // namespace cv;

#endif

#ifdef HAVE_CLAMDFFT

نطاق cv {

#define CLAMDDFT_Assert(func) \
    { \
        clfftStatus s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

صنف PlanCache
{
    بنية FftPlan
    {
        FftPlan(ثابت Size & _dft_size, صحيح _src_step, صحيح _dst_step, منطقي _doubleFP, منطقي _inplace, صحيح _flags, FftType _fftType) :
            dft_size(_dft_size), src_step(_src_step), dst_step(_dst_step),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            منطقي dft_inverse = (flags & DFT_INVERSE) != 0;
            منطقي dft_scale = (flags & DFT_SCALE) != 0;
            منطقي dft_rows = (flags & DFT_ROWS) != 0;

            clfftLayout inLayout = CLFFT_REAL, outLayout = CLFFT_REAL;
            clfftDim dim = dft_size.height == 1 || dft_rows ? CLFFT_1D : CLFFT_2D;

            size_t batchSize = dft_rows ? dft_size.height : 1;
            size_t clLengthsIn[3] = { (size_t)dft_size.width, dft_rows ? 1 : (size_t)dft_size.height, 1 };
            size_t clStridesIn[3] = { 1, 1, 1 };
            size_t clStridesOut[3]  = { 1, 1, 1 };
            صحيح elemSize = doubleFP ? حجم(مزدوج) : حجم(عائم);

            بدل (fftType)
            {
            حالة C2C:
                inLayout = CLFFT_COMPLEX_INTERLEAVED;
                outLayout = CLFFT_COMPLEX_INTERLEAVED;
                clStridesIn[1] = src_step / (elemSize << 1);
                clStridesOut[1] = dst_step / (elemSize << 1);
                اكسر;
            حالة R2C:
                inLayout = CLFFT_REAL;
                outLayout = CLFFT_HERMITIAN_INTERLEAVED;
                clStridesIn[1] = src_step / elemSize;
                clStridesOut[1] = dst_step / (elemSize << 1);
                اكسر;
            حالة C2R:
                inLayout = CLFFT_HERMITIAN_INTERLEAVED;
                outLayout = CLFFT_REAL;
                clStridesIn[1] = src_step / (elemSize << 1);
                clStridesOut[1] = dst_step / elemSize;
                اكسر;
            حالة R2R:
            افتراضي:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                اكسر;
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

            عائم scale = dft_scale ? 1.0f / (dft_rows ? dft_size.width : dft_size.area()) : 1.0f;
            CLAMDDFT_Assert(clfftSetPlanScale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &queue, عدم, عدم))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        friend صنف PlanCache;

    خاص:
        Size dft_size;
        صحيح src_step, dst_step;
        منطقي doubleFP;
        منطقي inplace;
        صحيح flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

عام:
    ساكن PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, جديد PlanCache())
    }

    clfftPlanHandle getPlanHandle(ثابت Size & dft_size, صحيح src_step, صحيح dst_step, منطقي doubleFP,
                                  منطقي inplace, صحيح flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        لكل (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            ثابت FftPlan * ثابت plan = planStorage[i];

            𐡄𐡍 (plan->dft_size == dft_size &&
                plan->flags == flags &&
                plan->src_step == src_step &&
                plan->dst_step == dst_step &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                𐡄𐡍 (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    اكسر;
                }

                أرجع plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(جديد FftPlan(dft_size, src_step, dst_step, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        أرجع newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

محمي:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

خارجي "C" {

ساكن فراغ CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, فراغ *p)
{
    UMatData * u = (UMatData *)p;

    𐡄𐡍( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

ساكن منطقي ocl_dft_amdfft(InputArray _src, OutputArray _dst, صحيح flags)
{
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    Size ssize = _src.size();

    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    𐡄𐡍 ( (!doubleSupport && depth == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        أرجع خطأ;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    𐡄𐡍 (ssize.area() != getOptimalDFTSize(ssize.area()))
        أرجع خطأ;

    صحيح dst_complex_input = cn == 2 ? 1 : 0;
    منطقي dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    صحيح dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    منطقي dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_input << 0 | dft_complex_output << 1);

    بدل (fftType)
    {
    حالة C2C:
        _dst.create(ssize.height, ssize.width, CV_MAKE_TYPE(depth, 2));
        اكسر;
    حالة R2C: // TODO implement it if possible
    حالة C2R: // TODO implement it if possible
    حالة R2R: // AMD Fft does not support this type
    افتراضي:
        أرجع خطأ;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    𐡄𐡍 (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        أرجع خطأ;
    منطقي inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (صحيح)src.step, (صحيح)dst.step,
                          depth == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t bufferSize = 0;
    CLAMDDFT_Assert(clfftGetTmpBufSize(plHandle, &bufferSize))
    UMat tmpBuffer(1, (صحيح)bufferSize, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfftEnqueueTransform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &queue, 0, عدم, &e,
                                          &srcarg, &dstarg, (cl_mem)tmpBuffer.handle(ACCESS_RW)))

    tmpBuffer.addref();
    clSetEventCallback(e, CL_COMPLETE, oclCleanupCallback, tmpBuffer.u);
    أرجع 𐡒𐡔𐡈;
}

#undef DFT_ASSERT

}

#endif // HAVE_CLAMDFFT

نطاق cv
{

template <typename T>
ساكن فراغ complementComplex(T * ptr, size_t step, صحيح n, صحيح len, صحيح dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = step/حجم(p0[0]);
    لكل(صحيح i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        لكل( صحيح j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

ساكن فراغ complementComplexOutput(صحيح depth, uchar * ptr, size_t step, صحيح count, صحيح len, صحيح dft_dims)
{
    𐡄𐡍( depth == CV_32F )
        complementComplex((عائم*)ptr, step, count, len, dft_dims);
    وإلا
        complementComplex((مزدوج*)ptr, step, count, len, dft_dims);
}

تعداد DftMode {
    InvalidDft = 0,
    FwdRealToCCS,
    FwdRealToComplex,
    FwdComplexToComplex,
    InvCCSToReal,
    InvComplexToReal,
    InvComplexToComplex,
};

تعداد DftDims {
    InvalidDim = 0,
    OneDim,
    OneDimColWise,
    TwoDims
};

مضمن ثابت حرف * modeName(DftMode m)
{
    بدل (m)
    {
    حالة InvalidDft: أرجع "InvalidDft";
    حالة FwdRealToCCS: أرجع "FwdRealToCCS";
    حالة FwdRealToComplex: أرجع "FwdRealToComplex";
    حالة FwdComplexToComplex: أرجع "FwdComplexToComplex";
    حالة InvCCSToReal: أرجع "InvCCSToReal";
    حالة InvComplexToReal: أرجع "InvComplexToReal";
    حالة InvComplexToComplex: أرجع "InvComplexToComplex";
    }
    أرجع 0;
}

مضمن ثابت حرف * dimsName(DftDims d)
{
    بدل (d)
    {
    حالة InvalidDim: أرجع "InvalidDim";
    حالة OneDim: أرجع "OneDim";
    حالة OneDimColWise: أرجع "OneDimColWise";
    حالة TwoDims: أرجع "TwoDims";
    };
    أرجع 0;
}

template <typename T>
مضمن منطقي isInv(T mode)
{
    بدل ((DftMode)mode)
    {
        حالة InvCCSToReal:
        حالة InvComplexToReal:
        حالة InvComplexToComplex: أرجع 𐡒𐡔𐡈;
        افتراضي: أرجع خطأ;
    }
}

مضمن DftMode determineMode(منطقي inv, صحيح cn1, صحيح cn2)
{
    𐡄𐡍 (!inv)
    {
        𐡄𐡍 (cn1 == 1 && cn2 == 1)
            أرجع FwdRealToCCS;
        وإلا 𐡄𐡍 (cn1 == 1 && cn2 == 2)
            أرجع FwdRealToComplex;
        وإلا 𐡄𐡍 (cn1 == 2 && cn2 == 2)
            أرجع FwdComplexToComplex;
    }
    وإلا
    {
        𐡄𐡍 (cn1 == 1 && cn2 == 1)
            أرجع InvCCSToReal;
        وإلا 𐡄𐡍 (cn1 == 2 && cn2 == 1)
            أرجع InvComplexToReal;
        وإلا 𐡄𐡍 (cn1 == 2 && cn2 == 2)
            أرجع InvComplexToComplex;
    }
    أرجع InvalidDft;
}


مضمن DftDims determineDims(صحيح rows, صحيح cols, منطقي isRowWise, منطقي isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    𐡄𐡍 (isRowWise)
        أرجع OneDim;
    𐡄𐡍 (cols == 1 && rows > 1) // one-column-shaped input
    {
        𐡄𐡍 (isContinuous)
            أرجع OneDim;
        وإلا
            أرجع OneDimColWise;
    }
    𐡄𐡍 (rows == 1)
        أرجع OneDim;
    𐡄𐡍 (cols > 1 && rows > 1)
        أرجع TwoDims;
    أرجع InvalidDim;
}

صنف OcvDftBasicImpl CV_FINAL : عام hal::DFT1D
{
عام:
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

عام:
    OcvDftBasicImpl()
    {
#ifdef USE_IPP_DFT
        ipp_worksize = 0;
#endif
    }
    فراغ init(صحيح len, صحيح count, صحيح depth, صحيح flags, منطقي *needBuffer)
    {
        صحيح stage = (flags & CV_HAL_DFT_STAGE_COLS) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        منطقي real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        منطقي needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = خطأ;
    #ifdef USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        𐡄𐡍( CV_IPP_CHECK_COND && (opt.n*count >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            صحيح ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            صحيح specsize=0, initsize=0, worksize=0;
            IppDFTGetSizeFunc getSizeFunc = 0;
            IppDFTInitFunc initFunc = 0;

            𐡄𐡍( real_transform && stage == 0 )
            {
                𐡄𐡍( depth == CV_32F )
                {
                    getSizeFunc = ippsDFTGetSize_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                وإلا
                {
                    getSizeFunc = ippsDFTGetSize_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            وإلا
            {
                𐡄𐡍( depth == CV_32F )
                {
                    getSizeFunc = ippsDFTGetSize_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                وإلا
                {
                    getSizeFunc = ippsDFTGetSize_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            𐡄𐡍( getSizeFunc(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                𐡄𐡍( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = 𐡒𐡔𐡈;
            }
            وإلا
                setIppErrorStatus();
        }
    #endif

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            ثابت DFTKernels& kernels = getDFTKernels(depth);
            صحيح kind = stage == 0 && real_transform ? (opt.isInverse ? DFT_KIND_C2R : DFT_KIND_R2C)
                                                    : DFT_KIND_C2C;
            plan.build(kind, len, depth, kernels.vlanes);
            ws.allocate(plan.ws_bytes);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.data();
            𐡄𐡍 (needBuffer)
                *needBuffer = خطأ;   // the engine supports in-place for every kind
        }

        {
            ساكن DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<عائم>>,
                dftWrap<عائم, RealDFT<عائم>>,
                dftWrap<عائم, CCSIDFT<عائم>>,
                dftWrap<Complexd, DFT<مزدوج>>,
                dftWrap<مزدوج, RealDFT<مزدوج>>,
                dftWrap<مزدوج, CCSIDFT<مزدوج>>
            };
            صحيح idx = 0;
            𐡄𐡍 (stage == 0)
            {
                𐡄𐡍 (real_transform)
                {
                    𐡄𐡍 (!opt.isInverse)
                        idx = 1;
                    وإلا
                        idx = 2;
                }
            }
            𐡄𐡍 (depth == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        𐡄𐡍(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            صحيح rowCount = count;
            𐡄𐡍 (stage == 0 && (flags & CV_HAL_DFT_ROWS) != 0)
                rowCount = 1;
            opt.scale = 1./(len * rowCount);
        }
    }

    فراغ apply(ثابت uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspaceSize() ثابت
    {
        size_t sz = opt.plan ? plan.ws_bytes : 0;
#ifdef USE_IPP_DFT
        𐡄𐡍 (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#endif
        أرجع sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    فراغ applyWithWorkspace(ثابت uchar *src, uchar *dst, uchar* workspace) ثابت
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#ifdef USE_IPP_DFT
        𐡄𐡍 (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#endif
        opt_.dft_func(opt_, src, dst);
    }

    فراغ حرر() {}
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
ساكن ثابت صحيح DFT_COLS_BATCH_MIN = 4;
ساكن ثابت صحيح DFT_COLS_BATCH_MAX = 16;
ساكن ثابت size_t DFT_COLS_BATCH_BYTES = 1 << 17;

ساكن صحيح dftColsBatch(صحيح len, size_t complex_elem_size)
{
    صحيح batch = (صحيح)(DFT_COLS_BATCH_BYTES/((size_t)len*complex_elem_size));
    أرجع std::max(DFT_COLS_BATCH_MIN, std::min(DFT_COLS_BATCH_MAX, batch));
}

ساكن مزدوج dftParallelStripes(صحيح count, صحيح len)
{
    ثابت مزدوج quantum = 0.5e6;
    أرجع (مزدوج)count*len*std::max(std::log((مزدوج)len)/std::log(2.), 1.)/quantum;
}

صنف OcvDftImpl CV_FINAL : عام hal::DFT2D
{
محمي:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    ثابت OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    ثابت OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    منطقي needBufferA;
    منطقي needBufferB;
    منطقي inv;
    صحيح width;
    صحيح height;
    DftMode mode;
    صحيح elem_size;
    صحيح complex_elem_size;
    صحيح depth;
    منطقي real_transform;
    صحيح nonzero_rows;
    منطقي isRowTransform;
    منطقي isScaled;
    std::vector<صحيح> stages;
    منطقي useIpp;
    صحيح src_channels;
    صحيح dst_channels;

    AutoBuffer<uchar> tmp_bufA;
    AutoBuffer<uchar> tmp_bufB;
    AutoBuffer<uchar> buf0;
    AutoBuffer<uchar> buf1;

عام:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        needBufferA = خطأ;
        needBufferB = خطأ;
        inv = خطأ;
        width = 0;
        height = 0;
        mode = InvalidDft;
        elem_size = 0;
        complex_elem_size = 0;
        depth = 0;
        real_transform = خطأ;
        nonzero_rows = 0;
        isRowTransform = خطأ;
        isScaled = خطأ;
        useIpp = خطأ;
        src_channels = 0;
        dst_channels = 0;
    }

    فراغ init(صحيح _width, صحيح _height, صحيح _depth, صحيح _src_channels, صحيح _dst_channels, صحيح flags, صحيح _nonzero_rows)
    {
        منطقي isComplex = _src_channels != _dst_channels;
        nonzero_rows = _nonzero_rows;
        width = _width;
        height = _height;
        depth = _depth;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        منطقي isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        منطقي isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        منطقي isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        mode = determineMode(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        needBufferA = خطأ;
        needBufferB = خطأ;
        real_transform = (mode != FwdComplexToComplex && mode != InvComplexToComplex);

        elem_size = (depth == CV_32F) ? حجم(عائم) : حجم(مزدوج);
        complex_elem_size = elem_size * 2;
        𐡄𐡍( !real_transform )
            elem_size = complex_elem_size;

#𐡄𐡍 defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            𐡄𐡍 (nonzero_rows == 0 && depth == CV_32F && ((width * height)>(صحيح)(1<<6)))
            {
                𐡄𐡍 (mode == FwdComplexToComplex || mode == InvComplexToComplex || mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    useIpp = 𐡒𐡔𐡈;
                    أرجع;
                }
            }
        }
#endif

        DftDims dims = determineDims(height, width, isRowTransform, isContinuous);
        𐡄𐡍 (dims == TwoDims)
        {
            stages.resize(2);
            𐡄𐡍 (mode == InvCCSToReal || mode == InvComplexToReal)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            وإلا
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        وإلا
        {
            stages.resize(1);
            𐡄𐡍 (dims == OneDimColWise)
                stages[0] = 1;
            وإلا
                stages[0] = 0;
        }

        لكل(uint stageIndex = 0; stageIndex < stages.size(); ++stageIndex)
        {
            𐡄𐡍 (stageIndex == 1)
            {
                isInplace = 𐡒𐡔𐡈;
                isComplex = خطأ;
            }

            صحيح stage = stages[stageIndex];
            منطقي isLastStage = (stageIndex + 1 == stages.size());

            صحيح len, count;

            صحيح f = 0;
            𐡄𐡍 (inv)
                f |= CV_HAL_DFT_INVERSE;
            𐡄𐡍 (isScaled)
                f |= CV_HAL_DFT_SCALE;
            𐡄𐡍 (isRowTransform)
                f |= CV_HAL_DFT_ROWS;
            𐡄𐡍 (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            𐡄𐡍 (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            𐡄𐡍 (!isLastStage)
                f |= CV_HAL_DFT_TWO_STAGE;

            𐡄𐡍( stage == 0 ) // row-wise transform
            {
                𐡄𐡍 (width == 1 && !isRowTransform )
                {
                    len = height;
                    count = width;
                }
                وإلا
                {
                    len = width;
                    count = height;
                }
                needBufferA = isInplace;
                contextA = hal::DFT1D::create(len, count, depth, f, &needBufferA);
                basicA = dynamic_cast<ثابت OcvDftBasicImpl*>(contextA.get());
                𐡄𐡍 (needBufferA)
                    tmp_bufA.allocate(len * complex_elem_size);
            }
            وإلا
            {
                len = height;
                count = width;
                f |= CV_HAL_DFT_STAGE_COLS;
                needBufferB = isInplace;
                contextB = hal::DFT1D::create(len, count, depth, f, &needBufferB);
                basicB = dynamic_cast<ثابت OcvDftBasicImpl*>(contextB.get());
                𐡄𐡍 (needBufferB)
                    tmp_bufB.allocate(len * complex_elem_size);

                buf0.allocate(len * complex_elem_size);
                buf1.allocate(len * complex_elem_size);
            }
        }
    }

    فراغ apply(ثابت uchar * src, size_t src_step, uchar * dst, size_t dst_step) CV_OVERRIDE
    {
#𐡄𐡍 defined USE_IPP_DFT
        𐡄𐡍 (useIpp)
        {
            صحيح ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            𐡄𐡍 (!isRowTransform)
            {
                𐡄𐡍 (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    𐡄𐡍 (ippi_DFT_C_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        أرجع;
                    }
                    setIppErrorStatus();
                }
                وإلا 𐡄𐡍 (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    𐡄𐡍 (ippi_DFT_R_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        أرجع;
                    }
                    setIppErrorStatus();
                }
            }
            وإلا
            {
                𐡄𐡍 (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    𐡄𐡍 (Dft_C_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        أرجع;
                    }
                    setIppErrorStatus();
                }
                وإلا 𐡄𐡍 (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippiDFTInv_PackToR_32f_C1R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    𐡄𐡍 (Dft_R_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        أرجع;
                    }
                    setIppErrorStatus();
                }
            }
            أرجع;
        }
#endif

        لكل(uint stageIndex = 0; stageIndex < stages.size(); ++stageIndex)
        {
            صحيح stage_src_channels = src_channels;
            صحيح stage_dst_channels = dst_channels;

            𐡄𐡍 (stageIndex == 1)
            {
                src = dst;
                src_step = dst_step;
                stage_src_channels = stage_dst_channels;
            }

            صحيح stage = stages[stageIndex];
            منطقي isLastStage = (stageIndex + 1 == stages.size());
            منطقي isComplex = stage_src_channels != stage_dst_channels;

            𐡄𐡍( stage == 0 )
                rowDft(src, src_step, dst, dst_step, isComplex, isLastStage);
            وإلا
                colDft(src, src_step, dst, dst_step, stage_src_channels, stage_dst_channels, isLastStage);
        }
    }

محمي:

    فراغ rowDft(ثابت uchar* src_data, size_t src_step, uchar* dst_data, size_t dst_step, منطقي isComplex, منطقي isLastStage)
    {
        صحيح len, count;
        𐡄𐡍 (width == 1 && !isRowTransform )
        {
            len = height;
            count = width;
        }
        وإلا
        {
            len = width;
            count = height;
        }
        صحيح dptr_offset = 0;
        صحيح dst_full_len = len*elem_size;

        𐡄𐡍( needBufferA )
        {
            𐡄𐡍 (mode == FwdRealToCCS && (len & 1) && len > 1)
                dptr_offset = elem_size;
        }

        𐡄𐡍( !inv && isComplex )
            dst_full_len += (len & 1) ? elem_size : complex_elem_size;

        صحيح nz = nonzero_rows;
        𐡄𐡍( nz <= 0 || nz > count )
            nz = count;

        size_t ws_size = basicA ? basicA->workspaceSize() : 0;
        مزدوج nstripes = dftParallelStripes(nz, len);
        𐡄𐡍( ws_size > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            ثابت OcvDftBasicImpl* ctx = basicA;
            auto processRows = [&, ctx](ثابت Range& range)
            {
                AutoBuffer<uchar> ws(ws_size);
                AutoBuffer<uchar> rowbuf(needBufferA ? len * complex_elem_size : 0);
                لكل( صحيح i = range.start; i < range.end; i++ )
                {
                    ثابت uchar* sptr = src_data + src_step * i;
                    uchar* dptr0 = dst_data + dst_step * i;
                    uchar* dptr = needBufferA ? rowbuf.data() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.data());
                    𐡄𐡍( needBufferA )
                        memcpy( dptr0, dptr + dptr_offset, dst_full_len );
                }
            };
            parallel_for_(Range(0, nz), processRows, nstripes);
        }
        وإلا
        {
            لكل( صحيح i = 0; i < nz; i++ )
            {
                ثابت uchar* sptr = src_data + src_step * i;
                uchar* dptr0 = dst_data + dst_step * i;
                uchar* dptr = dptr0;

                𐡄𐡍( needBufferA )
                    dptr = tmp_bufA.data();

                contextA->apply(sptr, dptr);

                𐡄𐡍( needBufferA )
                    memcpy( dptr0, dptr + dptr_offset, dst_full_len );
            }
        }

        لكل( صحيح i = nz; i < count; i++ )
        {
            uchar* dptr0 = dst_data + dst_step * i;
            memset( dptr0, 0, dst_full_len );
        }
        𐡄𐡍(isLastStage &&  mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_data, dst_step, len, nz, 1);
    }

    فراغ colDft(ثابت uchar* src_data, size_t src_step, uchar* dst_data, size_t dst_step, صحيح stage_src_channels, صحيح stage_dst_channels, منطقي isLastStage)
    {
        صحيح len = height;
        صحيح count = width;
        صحيح a = 0, b = count;
        uchar *dbuf0, *dbuf1;
        ثابت uchar* sptr0 = src_data;
        uchar* dptr0 = dst_data;

        dbuf0 = buf0.data(), dbuf1 = buf1.data();

        𐡄𐡍( needBufferB )
        {
            dbuf1 = tmp_bufB.data();
            dbuf0 = buf1.data();
        }

        𐡄𐡍( real_transform )
        {
            صحيح even;
            a = 1;
            even = (count & 1) == 0;
            b = (count+1)/2;
            𐡄𐡍( !inv )
            {
                memset( buf0.data(), 0, len*complex_elem_size );
                CopyColumn( sptr0, src_step, buf0.data(), complex_elem_size, len, elem_size );
                sptr0 += stage_dst_channels*elem_size;
                𐡄𐡍( even )
                {
                    memset( buf1.data(), 0, len*complex_elem_size );
                    CopyColumn( sptr0 + (count-2)*elem_size, src_step,
                                buf1.data(), complex_elem_size, len, elem_size );
                }
            }
            وإلا 𐡄𐡍( stage_src_channels == 1 )
            {
                CopyColumn( sptr0, src_step, buf0.data(), elem_size, len, elem_size );
                ExpandCCS( buf0.data(), len, elem_size );
                𐡄𐡍( even )
                {
                    CopyColumn( sptr0 + (count-1)*elem_size, src_step,
                                buf1.data(), elem_size, len, elem_size );
                    ExpandCCS( buf1.data(), len, elem_size );
                }
                sptr0 += elem_size;
            }
            وإلا
            {
                CopyColumn( sptr0, src_step, buf0.data(), complex_elem_size, len, complex_elem_size );
                𐡄𐡍( even )
                {
                    CopyColumn( sptr0 + b*complex_elem_size, src_step,
                                   buf1.data(), complex_elem_size, len, complex_elem_size );
                }
                sptr0 += complex_elem_size;
            }

            𐡄𐡍( even )
                contextB->apply(buf1.data(), dbuf1);
            contextB->apply(buf0.data(), dbuf0);

            𐡄𐡍( stage_dst_channels == 1 )
            {
                𐡄𐡍( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    memcpy( dbuf0 + elem_size, dbuf0, elem_size );
                    CopyColumn( dbuf0 + elem_size, elem_size, dptr0,
                                   dst_step, len, elem_size );
                    𐡄𐡍( even )
                    {
                        memcpy( dbuf1 + elem_size, dbuf1, elem_size );
                        CopyColumn( dbuf1 + elem_size, elem_size,
                                       dptr0 + (count-1)*elem_size,
                                       dst_step, len, elem_size );
                    }
                    dptr0 += elem_size;
                }
                وإلا
                {
                    // copy the real part of the complex vector to the first/last column
                    CopyColumn( dbuf0, complex_elem_size, dptr0, dst_step, len, elem_size );
                    𐡄𐡍( even )
                        CopyColumn( dbuf1, complex_elem_size, dptr0 + (count-1)*elem_size,
                                       dst_step, len, elem_size );
                    dptr0 += elem_size;
                }
            }
            وإلا
            {
                CV_Assert( !inv );
                CopyColumn( dbuf0, complex_elem_size, dptr0,
                               dst_step, len, complex_elem_size );
                𐡄𐡍( even )
                    CopyColumn( dbuf1, complex_elem_size,
                                   dptr0 + b*complex_elem_size,
                                   dst_step, len, complex_elem_size );
                dptr0 += complex_elem_size;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        صحيح ncols = b - a;
        ثابت صحيح cols_batch = dftColsBatch(len, complex_elem_size);
        صحيح nbatches = (ncols + cols_batch - 1)/cols_batch;
        size_t ws_size = basicB ? basicB->workspaceSize() : 0;
        size_t colbuf_size = (size_t)len*complex_elem_size;
        ثابت OcvDftBasicImpl* ctx = basicB;
        auto processBatches = [&, ctx](ثابت Range& range)
        {
            AutoBuffer<uchar> ws(ws_size);
            AutoBuffer<uchar> cbuf(colbuf_size*cols_batch*(needBufferB ? 2 : 1));
            uchar* cbuf0 = cbuf.data();
            uchar* cdbuf0 = needBufferB ? cbuf0 + colbuf_size*cols_batch : cbuf0;
            لكل( صحيح bi = range.start; bi < range.end; bi++ )
            {
                صحيح c0 = bi*cols_batch;
                صحيح nc = std::min(cols_batch, ncols - c0);
                ثابت uchar* sptr = sptr0 + (size_t)c0*complex_elem_size;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_size;
                CopyFromColumns( sptr, src_step, cbuf0, nc, len, complex_elem_size );
                لكل( صحيح k = 0; k < nc; k++ )
                {
                    𐡄𐡍( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_size, cdbuf0 + k*colbuf_size, ws.data());
                    وإلا
                        contextB->apply(cbuf0 + k*colbuf_size, cdbuf0 + k*colbuf_size);
                }
                CopyToColumns( cdbuf0, dptr, dst_step, nc, len, complex_elem_size );
            }
        };
        مزدوج nstripes = dftParallelStripes(ncols, len);
        𐡄𐡍( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        وإلا
            processBatches(Range(0, nbatches));
        𐡄𐡍(isLastStage && mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_data, dst_step, count, len, 2);
    }
};

بنية ReplacementDFT1D : عام hal::DFT1D
{
    cvhalDFT *context;
    منطقي isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(خطأ) {}
    منطقي init(صحيح len, صحيح count, صحيح depth, صحيح flags, منطقي *needBuffer)
    {
        صحيح res = cv_hal_dftInit1D(&context, len, count, depth, flags, needBuffer);
        isInitialized = (res == CV_HAL_ERROR_OK);
        أرجع isInitialized;
    }
    فراغ apply(ثابت uchar *src, uchar *dst) CV_OVERRIDE
    {
        𐡄𐡍 (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        𐡄𐡍 (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

بنية ReplacementDFT2D : عام hal::DFT2D
{
    cvhalDFT *context;
    منطقي isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(خطأ) {}
    منطقي init(صحيح width, صحيح height, صحيح depth,
              صحيح src_channels, صحيح dst_channels,
              صحيح flags, صحيح nonzero_rows)
    {
        صحيح res = cv_hal_dftInit2D(&context, width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        isInitialized = (res == CV_HAL_ERROR_OK);
        أرجع isInitialized;
    }
    فراغ apply(ثابت uchar *src, size_t src_step, uchar *dst, size_t dst_step) CV_OVERRIDE
    {
        𐡄𐡍 (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_step, dst, dst_step);
        }
    }
    ~ReplacementDFT2D()
    {
        𐡄𐡍 (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

نطاق hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(صحيح len, صحيح count, صحيح depth, صحيح flags, منطقي *needBuffer)
{
    {
        ReplacementDFT1D *impl = جديد ReplacementDFT1D();
        𐡄𐡍 (impl->init(len, count, depth, flags, needBuffer))
        {
            أرجع Ptr<DFT1D>(impl);
        }
        delete impl;
    }
    {
        OcvDftBasicImpl *impl = جديد OcvDftBasicImpl();
        impl->init(len, count, depth, flags, needBuffer);
        أرجع Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(صحيح width, صحيح height, صحيح depth,
                         صحيح src_channels, صحيح dst_channels,
                         صحيح flags, صحيح nonzero_rows)
{
    {
        ReplacementDFT2D *impl = جديد ReplacementDFT2D();
        𐡄𐡍 (impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows))
        {
            أرجع Ptr<DFT2D>(impl);
        }
        delete impl;
    }
    {
        𐡄𐡍(width == 1 && nonzero_rows > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = جديد OcvDftImpl();
        impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        أرجع Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


فراغ cv::dft( InputArray _src0, OutputArray _dst, صحيح flags, صحيح nonzero_rows )
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
    𐡄𐡍 (src.empty())
    {
        _dst.release();
        أرجع;
    }
    منطقي inv = (flags & DFT_INVERSE) != 0;
    صحيح type = src.type();
    صحيح depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_INPUT) && src.channels() != 2) );

    𐡄𐡍( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.createSameSize( src, CV_MAKETYPE(depth, 2) );
    وإلا 𐡄𐡍( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.createSameSize( src, depth );
    وإلا
        _dst.createSameSize( src, type );

    Mat dst = _dst.getMat();

    صحيح f = 0;
    𐡄𐡍 (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    𐡄𐡍 (inv)
        f |= CV_HAL_DFT_INVERSE;
    𐡄𐡍 (flags & DFT_ROWS)
        f |= CV_HAL_DFT_ROWS;
    𐡄𐡍 (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    𐡄𐡍 (src.data == dst.data)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.cols, src.rows, depth, src.channels(), dst.channels(), f, nonzero_rows);
    c->apply(src.data, src.step, dst.data, dst.step);
}


فراغ cv::idft( InputArray src, OutputArray dst, صحيح flags, صحيح nonzero_rows )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_rows );
}

#ifdef HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_mulSpectrums( InputArray _srcA, InputArray _srcB,
                              OutputArray _dst, صحيح flags, منطقي conjB )
{
    صحيح atype = _srcA.type(), btype = _srcB.type(),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    𐡄𐡍 ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        أرجع خطأ;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.createSameSize(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    𐡄𐡍 (k.empty())
        أرجع خطأ;

    k.args(ocl::KernelArg::ReadOnlyNoSize(A), ocl::KernelArg::ReadOnlyNoSize(B),
           ocl::KernelArg::WriteOnly(dst), rowsPerWI);

    size_t globalsize[2] = { (size_t)asize.width, ((size_t)asize.height + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, عدم, خطأ);
}

}

#endif

نطاق {

#define VAL(buf, elem) (((T*)((حرف*)data ## buf + (step ## buf * (elem))))[0])
#define MUL_SPECTRUMS_COL(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    لكل (size_t j = 1; j <= rows - 2; j += 2) \
    { \
        مزدوج a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        مزدوج b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        𐡄𐡍 (conjB) b_im = -b_im; \
        مزدوج c_re = a_re * b_re - a_im * b_im; \
        مزدوج c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    𐡄𐡍 ((rows & 1) == 0) \
        VAL(C, rows-1) = VAL(A, rows-1) * VAL(B, rows-1)

template <typename T, منطقي conjB> ساكن مضمن
فراغ mulSpectrums_processCol_noinplace(ثابت T* dataA, ثابت T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows)
{
    MUL_SPECTRUMS_COL(A, B, C);
}

template <typename T, منطقي conjB> ساكن مضمن
فراغ mulSpectrums_processCol_inplaceA(ثابت T* dataB, T* dataAC, size_t stepB, size_t stepAC, size_t rows)
{
    MUL_SPECTRUMS_COL(AC, B, AC);
}
template <typename T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mulSpectrums_processCol(ثابت T* dataA, ثابت T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows)
{
    𐡄𐡍 (inplaceA)
        mulSpectrums_processCol_inplaceA<T, conjB>(dataB, dataC, stepB, stepC, rows);
    وإلا
        mulSpectrums_processCol_noinplace<T, conjB>(dataA, dataB, dataC, stepA, stepB, stepC, rows);
}
#undef MUL_SPECTRUMS_COL
#undef VAL

template <typename T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mulSpectrums_processCols(ثابت T* dataA, ثابت T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols)
{
    mulSpectrums_processCol<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows);
    𐡄𐡍 ((cols & 1) == 0)
    {
        mulSpectrums_processCol<T, conjB, inplaceA>(dataA + cols - 1, dataB + cols - 1, dataC + cols - 1, stepA, stepB, stepC, rows);
    }
}

#define VAL(buf, elem) (data ## buf[(elem)])
#define MUL_SPECTRUMS_ROW(A, B, C) \
    لكل (size_t j = j0; j < j1; j += 2) \
    { \
        مزدوج a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        مزدوج b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        𐡄𐡍 (conjB) b_im = -b_im; \
        مزدوج c_re = a_re * b_re - a_im * b_im; \
        مزدوج c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
template <typename T, منطقي conjB> ساكن مضمن
فراغ mulSpectrums_processRow_noinplace(ثابت T* dataA, ثابت T* dataB, T* dataC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(A, B, C);
}
template <typename T, منطقي conjB> ساكن مضمن
فراغ mulSpectrums_processRow_inplaceA(ثابت T* dataB, T* dataAC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(AC, B, AC);
}
template <typename T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mulSpectrums_processRow(ثابت T* dataA, ثابت T* dataB, T* dataC, size_t j0, size_t j1)
{
    𐡄𐡍 (inplaceA)
        mulSpectrums_processRow_inplaceA<T, conjB>(dataB, dataC, j0, j1);
    وإلا
        mulSpectrums_processRow_noinplace<T, conjB>(dataA, dataB, dataC, j0, j1);
}
#undef MUL_SPECTRUMS_ROW
#undef VAL

template <typename T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mulSpectrums_processRows(ثابت T* dataA, ثابت T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, منطقي is_1d_CN1)
{
    طالما (rows-- > 0)
    {
        𐡄𐡍 (is_1d_CN1)
            dataC[0] = dataA[0]*dataB[0];
        mulSpectrums_processRow<T, conjB, inplaceA>(dataA, dataB, dataC, j0, j1);
        𐡄𐡍 (is_1d_CN1 && (cols & 1) == 0)
            dataC[j1] = dataA[j1]*dataB[j1];

        dataA = (ثابت T*)(((حرف*)dataA) + stepA);
        dataB = (ثابت T*)(((حرف*)dataB) + stepB);
        dataC =       (T*)(((حرف*)dataC) + stepC);
    }
}


template <typename T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mulSpectrums_Impl_(ثابت T* dataA, ثابت T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, منطقي is_1d, منطقي isCN1)
{
    𐡄𐡍 (!is_1d && isCN1)
    {
        mulSpectrums_processCols<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols);
    }
    mulSpectrums_processRows<T, conjB, inplaceA>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d && isCN1);
}
template <typename T, منطقي conjB> ساكن مضمن
فراغ mulSpectrums_Impl(ثابت T* dataA, ثابت T* dataB, T* dataC, size_t stepA, size_t stepB, size_t stepC, size_t rows, size_t cols, size_t j0, size_t j1, منطقي is_1d, منطقي isCN1)
{
    𐡄𐡍 (dataA == dataC)
        mulSpectrums_Impl_<T, conjB, 𐡒𐡔𐡈>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
    وإلا
        mulSpectrums_Impl_<T, conjB, خطأ>(dataA, dataB, dataC, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
}

} // namespace

فراغ cv::mulSpectrums( InputArray _srcA, InputArray _srcB,
                       OutputArray _dst, صحيح flags, منطقي conjB )
{
    CV_INSTRUMENT_REGION();

    𐡄𐡍 (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        أرجع;
    }

    صحيح type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_RUN(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    صحيح depth = srcA.depth(), cn = srcA.channels();
    size_t rows = srcA.rows, cols = srcA.cols;

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    𐡄𐡍 (dst.data == srcB.data)
        srcB = srcB.clone(); // workaround for B only

    منطقي is_1d = (flags & DFT_ROWS)
        || (rows == 1)
        || (cols == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    𐡄𐡍( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    منطقي isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = cols*cn - (((cols & 1) == 0 && cn == 1) ? 1 : 0);

    𐡄𐡍 (depth == CV_32F)
    {
        ثابت عائم* dataA = srcA.ptr<عائم>();
        ثابت عائم* dataB = srcB.ptr<عائم>();
        عائم* dataC = dst.ptr<عائم>();
        𐡄𐡍 (!conjB)
            mulSpectrums_Impl<عائم, خطأ>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        وإلا
            mulSpectrums_Impl<عائم, 𐡒𐡔𐡈>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
    وإلا
    {
        ثابت مزدوج* dataA = srcA.ptr<مزدوج>();
        ثابت مزدوج* dataB = srcB.ptr<مزدوج>();
        مزدوج* dataC = dst.ptr<مزدوج>();
        𐡄𐡍 (!conjB)
            mulSpectrums_Impl<مزدوج, خطأ>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        وإلا
            mulSpectrums_Impl<مزدوج, 𐡒𐡔𐡈>(dataA, dataB, dataC, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
}

فراغ cv::divSpectrums( InputArray _srcA, InputArray _srcB, OutputArray _dst, صحيح flags, منطقي conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    𐡄𐡍 (srcA.empty() && srcB.empty())
    {
        _dst.release();
        أرجع;
    }
    صحيح depth = srcA.depth(), cn = srcA.channels(), type = srcA.type();
    صحيح rows = srcA.rows, cols = srcA.cols;
    صحيح j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.data != srcA.data); // non-inplace check
    CV_Assert(dst.data != srcB.data); // non-inplace check

    منطقي is_1d = (flags & DFT_ROWS) || (rows == 1 || (cols == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    𐡄𐡍( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    صحيح ncols = cols*cn;
    صحيح j0 = cn == 1;
    صحيح j1 = ncols - (cols % 2 == 0 && cn == 1);

    𐡄𐡍( depth == CV_32F )
    {
        ثابت عائم* dataA = srcA.ptr<عائم>();
        ثابت عائم* dataB = srcB.ptr<عائم>();
        عائم* dataC = dst.ptr<عائم>();
        عائم eps = FLT_EPSILON; // prevent div0 problems

        size_t stepA = srcA.step/حجم(dataA[0]);
        size_t stepB = srcB.step/حجم(dataB[0]);
        size_t stepC = dst.step/حجم(dataC[0]);

        𐡄𐡍( !is_1d && cn == 1 )
        {
            لكل( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                𐡄𐡍( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0] / (dataB[0] + eps);
                𐡄𐡍( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA] / (dataB[(rows-1)*stepB] + eps);
                𐡄𐡍( !conjB )
                    لكل( j = 1; j <= rows - 2; j += 2 )
                    {
                        مزدوج denom = (مزدوج)dataB[j*stepB]*dataB[j*stepB] +
                        (مزدوج)dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + (مزدوج)eps;

                        مزدوج re = (مزدوج)dataA[j*stepA]*dataB[j*stepB] +
                        (مزدوج)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        مزدوج im = (مزدوج)dataA[(j+1)*stepA]*dataB[j*stepB] -
                        (مزدوج)dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = (عائم)(re / denom);
                        dataC[(j+1)*stepC] = (عائم)(im / denom);
                    }
                    وإلا
                        لكل( j = 1; j <= rows - 2; j += 2 )
                        {

                            مزدوج denom = (مزدوج)dataB[j*stepB]*dataB[j*stepB] +
                            (مزدوج)dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + (مزدوج)eps;

                            مزدوج re = (مزدوج)dataA[j*stepA]*dataB[j*stepB] -
                            (مزدوج)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                            مزدوج im = (مزدوج)dataA[(j+1)*stepA]*dataB[j*stepB] +
                            (مزدوج)dataA[j*stepA]*dataB[(j+1)*stepB];

                            dataC[j*stepC] = (عائم)(re / denom);
                            dataC[(j+1)*stepC] = (عائم)(im / denom);
                        }
                    𐡄𐡍( k == 1 )
                        dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        لكل( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            𐡄𐡍( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0] / (dataB[0] + eps);
                𐡄𐡍( cols % 2 == 0 )
                    dataC[j1] = dataA[j1] / (dataB[j1] + eps);
            }

            𐡄𐡍( !conjB )
                لكل( j = j0; j < j1; j += 2 )
                {
                    مزدوج denom = (مزدوج)dataB[j]*dataB[j] + (مزدوج)dataB[j+1]*dataB[j+1] + (مزدوج)eps;
                    مزدوج re = (مزدوج)dataA[j]*dataB[j] + (مزدوج)dataA[j+1]*dataB[j+1];
                    مزدوج im = (مزدوج)dataA[j+1]*dataB[j] - (مزدوج)dataA[j]*dataB[j+1];
                    dataC[j] = (عائم)(re / denom);
                    dataC[j+1] = (عائم)(im / denom);
                }
                وإلا
                    لكل( j = j0; j < j1; j += 2 )
                    {
                        مزدوج denom = (مزدوج)dataB[j]*dataB[j] + (مزدوج)dataB[j+1]*dataB[j+1] + (مزدوج)eps;
                        مزدوج re = (مزدوج)dataA[j]*dataB[j] - (مزدوج)dataA[j+1]*dataB[j+1];
                        مزدوج im = (مزدوج)dataA[j+1]*dataB[j] + (مزدوج)dataA[j]*dataB[j+1];
                        dataC[j] = (عائم)(re / denom);
                        dataC[j+1] = (عائم)(im / denom);
                    }
        }
    }
    وإلا
    {
        ثابت مزدوج* dataA = srcA.ptr<مزدوج>();
        ثابت مزدوج* dataB = srcB.ptr<مزدوج>();
        مزدوج* dataC = dst.ptr<مزدوج>();
        مزدوج eps = DBL_EPSILON; // prevent div0 problems

        size_t stepA = srcA.step/حجم(dataA[0]);
        size_t stepB = srcB.step/حجم(dataB[0]);
        size_t stepC = dst.step/حجم(dataC[0]);

        𐡄𐡍( !is_1d && cn == 1 )
        {
            لكل( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                𐡄𐡍( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0] / (dataB[0] + eps);
                𐡄𐡍( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA] / (dataB[(rows-1)*stepB] + eps);
                𐡄𐡍( !conjB )
                    لكل( j = 1; j <= rows - 2; j += 2 )
                    {
                        مزدوج denom = dataB[j*stepB]*dataB[j*stepB] +
                        dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + eps;

                        مزدوج re = dataA[j*stepA]*dataB[j*stepB] +
                        dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                        مزدوج im = dataA[(j+1)*stepA]*dataB[j*stepB] -
                        dataA[j*stepA]*dataB[(j+1)*stepB];

                        dataC[j*stepC] = re / denom;
                        dataC[(j+1)*stepC] = im / denom;
                    }
                    وإلا
                        لكل( j = 1; j <= rows - 2; j += 2 )
                        {
                            مزدوج denom = dataB[j*stepB]*dataB[j*stepB] +
                            dataB[(j+1)*stepB]*dataB[(j+1)*stepB] + eps;

                            مزدوج re = dataA[j*stepA]*dataB[j*stepB] -
                            dataA[(j+1)*stepA]*dataB[(j+1)*stepB];

                            مزدوج im = dataA[(j+1)*stepA]*dataB[j*stepB] +
                            dataA[j*stepA]*dataB[(j+1)*stepB];

                            dataC[j*stepC] = re / denom;
                            dataC[(j+1)*stepC] = im / denom;
                        }
                    𐡄𐡍( k == 1 )
                        dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        لكل( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            𐡄𐡍( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0] / (dataB[0] + eps);
                𐡄𐡍( cols % 2 == 0 )
                    dataC[j1] = dataA[j1] / (dataB[j1] + eps);
            }

            𐡄𐡍( !conjB )
                لكل( j = j0; j < j1; j += 2 )
                {
                    مزدوج denom = dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps;
                    مزدوج re = dataA[j]*dataB[j] + dataA[j+1]*dataB[j+1];
                    مزدوج im = dataA[j+1]*dataB[j] - dataA[j]*dataB[j+1];
                    dataC[j] = re / denom;
                    dataC[j+1] = im / denom;
                }
                وإلا
                    لكل( j = j0; j < j1; j += 2 )
                    {
                        مزدوج denom = dataB[j]*dataB[j] + dataB[j+1]*dataB[j+1] + eps;
                        مزدوج re = dataA[j]*dataB[j] - dataA[j+1]*dataB[j+1];
                        مزدوج im = dataA[j+1]*dataB[j] + dataA[j]*dataB[j+1];
                        dataC[j] = re / denom;
                        dataC[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

نطاق cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
template<typename T> ساكن فراغ
DCT( ثابت OcvDftOptions & c, ثابت T* src, size_t src_step, T* dst, size_t dst_step )
{
    𐡄𐡍( c.n == 1 )
    {
        dst[0] = src[0];
        أرجع;
    }
    runDft(c, src, src_step/حجم(T), dst, dst_step/حجم(T), خطأ);
}

template<typename T> ساكن فراغ
IDCT( ثابت OcvDftOptions & c, ثابت T* src, size_t src_step, T* dst, size_t dst_step )
{
    𐡄𐡍( c.n == 1 )
    {
        dst[0] = src[0];
        أرجع;
    }
    runDft(c, src, src_step/حجم(T), dst, dst_step/حجم(T), خطأ);
}

تعريفنوع فراغ (*DCTFunc)(ثابت OcvDftOptions & c, ثابت فراغ* src, size_t src_step,
                        فراغ* dst, size_t dst_step);

template<typename T, فراغ (*fn)(ثابت OcvDftOptions&, ثابت T*, size_t, T*, size_t)>
ساكن فراغ dctWrap(ثابت OcvDftOptions & c, ثابت فراغ* src, size_t src_step, فراغ* dst, size_t dst_step)
{
    fn(c, (ثابت T*)src, src_step, (T*)dst, dst_step);
}

}

#ifdef HAVE_IPP
نطاق cv
{

#𐡄𐡍 IPP_VERSION_X100 >= 900
تعريفنوع IppStatus (CV_STDCALL * ippiDCTFunc)(ثابت Ipp32f* pSrc, صحيح srcStep, Ipp32f* pDst, صحيح dstStep, ثابت فراغ* pDCTSpec, Ipp8u* pBuffer);
تعريفنوع IppStatus (CV_STDCALL * ippiDCTInit)(فراغ* pDCTSpec, IppiSize roiSize, Ipp8u* pMemInit );
تعريفنوع IppStatus (CV_STDCALL * ippiDCTGetSize)(IppiSize roiSize, صحيح* pSizeSpec, صحيح* pSizeInit, صحيح* pSizeBuf);

template<typename SpecType, IppStatus (CV_STDCALL *fn)(ثابت Ipp32f*, صحيح, Ipp32f*, صحيح, ثابت SpecType*, Ipp8u*)>
ساكن IppStatus CV_STDCALL ippiDCTWrap(ثابت Ipp32f* pSrc, صحيح srcStep, Ipp32f* pDst, صحيح dstStep, ثابت فراغ* pDCTSpec, Ipp8u* pBuffer)
{
    أرجع fn(pSrc, srcStep, pDst, dstStep, (ثابت SpecType*)pDCTSpec, pBuffer);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*, IppiSize, Ipp8u*)>
ساكن IppStatus CV_STDCALL ippiDCTInitWrap(فراغ* pDCTSpec, IppiSize roiSize, Ipp8u* pMemInit)
{
    أرجع fn((SpecType*)pDCTSpec, roiSize, pMemInit);
}

template<IppStatus (CV_STDCALL *fn)(IppiSize, صحيح*, صحيح*, صحيح*)>
ساكن IppStatus CV_STDCALL ippiDCTGetSizeWrap(IppiSize roiSize, صحيح* pSizeSpec, صحيح* pSizeInit, صحيح* pSizeBuf)
{
    أرجع fn(roiSize, pSizeSpec, pSizeInit, pSizeBuf);
}

#وإلاإذا IPP_VERSION_X100 >= 700
تعريفنوع IppStatus (CV_STDCALL * ippiDCTFunc)(ثابت Ipp32f*, صحيح, Ipp32f*, صحيح, ثابت فراغ*, Ipp8u*);
تعريفنوع IppStatus (CV_STDCALL * ippiDCTInitAlloc)(فراغ**, IppiSize, IppHintAlgorithm);
تعريفنوع IppStatus (CV_STDCALL * ippiDCTFree)(فراغ* pDCTSpec);
تعريفنوع IppStatus (CV_STDCALL * ippiDCTGetBufSize)(ثابت فراغ*, صحيح*);

template<typename SpecType, IppStatus (CV_STDCALL *fn)(ثابت Ipp32f*, صحيح, Ipp32f*, صحيح, ثابت SpecType*, Ipp8u*)>
ساكن IppStatus CV_STDCALL ippiDCTWrap(ثابت Ipp32f* pSrc, صحيح srcStep, Ipp32f* pDst, صحيح dstStep, ثابت فراغ* pDCTSpec, Ipp8u* pBuffer)
{
    أرجع fn(pSrc, srcStep, pDst, dstStep, (ثابت SpecType*)pDCTSpec, pBuffer);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType**, IppiSize, IppHintAlgorithm)>
ساكن IppStatus CV_STDCALL ippiDCTInitAllocWrap(فراغ** pDCTSpec, IppiSize roiSize, IppHintAlgorithm hint)
{
    أرجع fn((SpecType**)pDCTSpec, roiSize, hint);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*)>
ساكن IppStatus CV_STDCALL ippiDCTFreeWrap(فراغ* pDCTSpec)
{
    أرجع fn((SpecType*)pDCTSpec);
}

template<typename SpecType, IppStatus (CV_STDCALL *fn)(ثابت SpecType*, صحيح*)>
ساكن IppStatus CV_STDCALL ippiDCTGetBufSizeWrap(ثابت فراغ* pDCTSpec, صحيح* pSize)
{
    أرجع fn((ثابت SpecType*)pDCTSpec, pSize);
}
#endif

صنف DctIPPLoop_Invoker : عام ParallelLoopBody
{
عام:
    DctIPPLoop_Invoker(ثابت uchar * _src, size_t _src_step, uchar * _dst, size_t _dst_step, صحيح _width, منطقي _inv, منطقي *_ok) :
        ParallelLoopBody(), src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width), inv(_inv), ok(_ok)
    {
        *ok = 𐡒𐡔𐡈;
    }

    virtual فراغ operator()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        𐡄𐡍(*ok == خطأ)
            أرجع;

#𐡄𐡍 IPP_VERSION_X100 >= 900
        IppiSize srcRoiSize = {width, 1};

        صحيح specSize    = 0;
        صحيح initSize    = 0;
        صحيح bufferSize  = 0;

        Ipp8u* pDCTSpec = عدم;
        Ipp8u* pBuffer  = عدم;
        Ipp8u* pInitBuf = عدم;

        #define IPP_RETURN              \
            𐡄𐡍(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            𐡄𐡍(pBuffer)                 \
                ippFree(pBuffer);       \
            𐡄𐡍(pInitBuf)                \
                ippFree(pInitBuf);      \
            أرجع;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippiDCTGetSize  ippDctGetSize  = inv ? ippiDCTGetSizeWrap<ippiDCTInvGetSize_32f> : ippiDCTGetSizeWrap<ippiDCTFwdGetSize_32f>;

        𐡄𐡍(ippDctGetSize(srcRoiSize, &specSize, &initSize, &bufferSize) < 0)
        {
            *ok = خطأ;
            أرجع;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(specSize);
        𐡄𐡍(!pDCTSpec && specSize)
        {
            *ok = خطأ;
            أرجع;
        }

        pBuffer  = (Ipp8u*)CV_IPP_MALLOC(bufferSize);
        𐡄𐡍(!pBuffer && bufferSize)
        {
            *ok = خطأ;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(initSize);
        𐡄𐡍(!pInitBuf && initSize)
        {
            *ok = خطأ;
            IPP_RETURN
        }

        𐡄𐡍(ippDctInit(pDCTSpec, srcRoiSize, pInitBuf) < 0)
        {
            *ok = خطأ;
            IPP_RETURN
        }

        لكل(صحيح i = range.start; i < range.end; ++i)
        {
            𐡄𐡍(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (عائم*)(src + src_step * i), static_cast<صحيح>(src_step), (عائم*)(dst + dst_step * i), static_cast<صحيح>(dst_step), pDCTSpec, pBuffer) < 0)
            {
                *ok = خطأ;
                IPP_RETURN
            }
        }
        IPP_RETURN
#undef IPP_RETURN
#وإلاإذا IPP_VERSION_X100 >= 700
        فراغ* pDCTSpec;
        AutoBuffer<uchar> buf;
        uchar* pBuffer = 0;
        صحيح bufSize=0;

        IppiSize srcRoiSize = {width, 1};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippiDCTGetBufSize ippGetBufSize = inv ? ippiDCTGetBufSizeWrap<IppiDCTInvSpec_32f, ippiDCTInvGetBufSize_32f> : ippiDCTGetBufSizeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdGetBufSize_32f>;

        𐡄𐡍 (ippInitAlloc(&pDCTSpec, srcRoiSize, ippAlgHintNone)>=0 && ippGetBufSize(pDCTSpec, &bufSize)>=0)
        {
            buf.allocate( bufSize );
            pBuffer = (uchar*)buf;

            لكل( صحيح i = range.start; i < range.end; ++i)
            {
                𐡄𐡍(ippDctFun((عائم*)(src + src_step * i), static_cast<صحيح>(src_step), (عائم*)(dst + dst_step * i), static_cast<صحيح>(dst_step), pDCTSpec, (Ipp8u*)pBuffer) < 0)
                {
                    *ok = خطأ;
                    اكسر;
                }
            }
        }
        وإلا
            *ok = خطأ;

        𐡄𐡍 (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END
#وإلا
        CV_UNUSED(range);
        *ok = خطأ;
#endif
    }

خاص:
    ثابت uchar * src;
    size_t src_step;
    uchar * dst;
    size_t dst_step;
    صحيح width;
    منطقي inv;
    منطقي *ok;
};

ساكن منطقي DctIPPLoop(ثابت uchar * src, size_t src_step, uchar * dst, size_t dst_step, صحيح width, صحيح height, منطقي inv)
{
    منطقي ok;
    parallel_for_(Range(0, height), DctIPPLoop_Invoker(src, src_step, dst, dst_step, width, inv, &ok), height/(مزدوج)(1<<4) );
    أرجع ok;
}

ساكن منطقي ippi_DCT_32f(ثابت uchar * src, size_t src_step, uchar * dst, size_t dst_step, صحيح width, صحيح height, منطقي inv, منطقي row)
{
    CV_INSTRUMENT_REGION_IPP();

    𐡄𐡍(row)
        أرجع DctIPPLoop(src, src_step, dst, dst_step, width, height, inv);
    وإلا
    {
#𐡄𐡍 IPP_VERSION_X100 >= 900
        IppiSize srcRoiSize = {width, height};

        صحيح specSize    = 0;
        صحيح initSize    = 0;
        صحيح bufferSize  = 0;

        Ipp8u* pDCTSpec = عدم;
        Ipp8u* pBuffer  = عدم;
        Ipp8u* pInitBuf = عدم;

        #define IPP_RELEASE             \
            𐡄𐡍(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            𐡄𐡍(pBuffer)                 \
                ippFree(pBuffer);       \
            𐡄𐡍(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippiDCTGetSize  ippDctGetSize  = inv ? ippiDCTGetSizeWrap<ippiDCTInvGetSize_32f> : ippiDCTGetSizeWrap<ippiDCTFwdGetSize_32f>;

        𐡄𐡍(ippDctGetSize(srcRoiSize, &specSize, &initSize, &bufferSize) < 0)
            أرجع خطأ;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(specSize);
        𐡄𐡍(!pDCTSpec && specSize)
            أرجع خطأ;

        pBuffer  = (Ipp8u*)CV_IPP_MALLOC(bufferSize);
        𐡄𐡍(!pBuffer && bufferSize)
        {
            IPP_RELEASE
            أرجع خطأ;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(initSize);
        𐡄𐡍(!pInitBuf && initSize)
        {
            IPP_RELEASE
            أرجع خطأ;
        }

        𐡄𐡍(ippDctInit(pDCTSpec, srcRoiSize, pInitBuf) < 0)
        {
            IPP_RELEASE
            أرجع خطأ;
        }

        𐡄𐡍(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (عائم*)src, static_cast<صحيح>(src_step), (عائم*)dst, static_cast<صحيح>(dst_step), pDCTSpec, pBuffer) < 0)
        {
            IPP_RELEASE
            أرجع خطأ;
        }

        IPP_RELEASE
        أرجع 𐡒𐡔𐡈;
#undef IPP_RELEASE
#وإلاإذا IPP_VERSION_X100 >= 700
        IppStatus status;
        فراغ* pDCTSpec;
        AutoBuffer<uchar> buf;
        uchar* pBuffer = 0;
        صحيح bufSize=0;

        IppiSize srcRoiSize = {width, height};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippiDCTGetBufSize ippGetBufSize = inv ? ippiDCTGetBufSizeWrap<IppiDCTInvSpec_32f, ippiDCTInvGetBufSize_32f> : ippiDCTGetBufSizeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdGetBufSize_32f>;

        status = ippStsErr;

        𐡄𐡍 (ippInitAlloc(&pDCTSpec, srcRoiSize, ippAlgHintNone)>=0 && ippGetBufSize(pDCTSpec, &bufSize)>=0)
        {
            buf.allocate( bufSize );
            pBuffer = (uchar*)buf;

            status = ippDctFun((عائم*)src, static_cast<صحيح>(src_step), (عائم*)dst, static_cast<صحيح>(dst_step), pDCTSpec, (Ipp8u*)pBuffer);
        }

        𐡄𐡍 (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END

        أرجع status >= 0;
#وإلا
        CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(inv); CV_UNUSED(row);
        أرجع خطأ;
#endif
    }
}
}
#endif

نطاق cv {

صنف OcvDctImpl CV_FINAL : عام hal::DCT2D
{
عام:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    AutoBuffer<uchar> ws;
    منطقي isRowTransform;
    منطقي isInverse;
    منطقي isContinuous;
    صحيح start_stage;
    صحيح end_stage;
    صحيح width;
    صحيح height;
    صحيح depth;

    فراغ init(صحيح _width, صحيح _height, صحيح _depth, صحيح flags)
    {
        width = _width;
        height = _height;
        depth = _depth;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        ساكن DCTFunc dct_tbl[4] =
        {
            dctWrap<عائم, DCT<عائم>>,
            dctWrap<عائم, IDCT<عائم>>,
            dctWrap<مزدوج, DCT<مزدوج>>,
            dctWrap<مزدوج, IDCT<مزدوج>>
        };
        dct_func = dct_tbl[(صحيح)isInverse + (depth == CV_64F)*2];
        opt.isComplex = خطأ;
        opt.isInverse = خطأ;
        opt.scale = 1.;

        𐡄𐡍 (isRowTransform || height == 1 || (width == 1 && isContinuous))
        {
            start_stage = end_stage = 0;
        }
        وإلا
        {
            start_stage = (width == 1);
            end_stage = 1;
        }
    }
    فراغ apply(ثابت uchar *src, size_t src_step, uchar *dst, size_t dst_step) CV_OVERRIDE
    {
        CV_IPP_RUN(IPP_VERSION_X100 >= 700 && depth == CV_32F, ippi_DCT_32f(src, src_step, dst, dst_step, width, height, isInverse, isRowTransform))

        صحيح prev_len = 0;
        صحيح elem_size = (depth == CV_32F) ? حجم(عائم) : حجم(مزدوج);
        ثابت DFTKernels& kernels = getDFTKernels(depth);

        لكل(صحيح stage = start_stage ; stage <= end_stage; stage++ )
        {
            ثابت uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            صحيح len, count;

            𐡄𐡍( stage == 0 )
            {
                len = width;
                count = height;
                𐡄𐡍( len == 1 && !isRowTransform )
                {
                    len = height;
                    count = 1;
                }
                sstep0 = src_step;
                dstep0 = dst_step;
                sstep1 = dstep1 = elem_size;
            }
            وإلا
            {
                len = height;
                count = width;
                sstep1 = src_step;
                dstep1 = dst_step;
                sstep0 = dstep0 = elem_size;
            }

            opt.n = len;

            𐡄𐡍( len != prev_len )
            {
                𐡄𐡍( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                𐡄𐡍( len > 1 )
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
            مزدوج nstripes = dftParallelStripes(count, len);
            𐡄𐡍( len > 1 && count > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                auto processLines = [&](ثابت Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    AutoBuffer<uchar> ws_(plan.ws_bytes);
                    opt_.workspace = ws_.data();
                    لكل( صحيح i = range.start; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, count), processLines, nstripes);
            }
            وإلا
            {
                لكل(unsigned i = 0; i < static_cast<unsigned>(count); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_step = dst_step;
        }
    }
};

بنية ReplacementDCT2D : عام hal::DCT2D
{
    cvhalDFT *context;
    منطقي isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(خطأ) {}
    منطقي init(صحيح width, صحيح height, صحيح depth, صحيح flags)
    {
        صحيح res = cv_hal_dctInit2D(&context, width, height, depth, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        أرجع isInitialized;
    }
    فراغ apply(ثابت uchar *src_data, size_t src_step, uchar *dst_data, size_t dst_step) CV_OVERRIDE
    {
        𐡄𐡍 (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_data, src_step, dst_data, dst_step);
        }
    }
    ~ReplacementDCT2D()
    {
        𐡄𐡍 (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

نطاق hal {

Ptr<DCT2D> DCT2D::create(صحيح width, صحيح height, صحيح depth, صحيح flags)
{
    {
        ReplacementDCT2D *impl = جديد ReplacementDCT2D();
        𐡄𐡍 (impl->init(width, height, depth, flags))
        {
            أرجع Ptr<DCT2D>(impl);
        }
        delete impl;
    }
    {
        OcvDctImpl *impl = جديد OcvDctImpl();
        impl->init(width, height, depth, flags);
        أرجع Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

فراغ cv::dct( InputArray _src0, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    𐡄𐡍 (src.empty())
    {
        _dst.release();
        أرجع;
    }
    صحيح type = src.type(), depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.rows, src.cols, type );
    Mat dst = _dst.getMat();

    صحيح f = 0;
    𐡄𐡍 ((flags & DFT_ROWS) != 0)
        f |= CV_HAL_DFT_ROWS;
    𐡄𐡍 ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    𐡄𐡍 (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.cols, src.rows, depth, f);
    c->apply(src.data, src.step, dst.data, dst.step);
}


فراغ cv::idct( InputArray src, OutputArray dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

نطاق cv
{

ساكن ثابت صحيح optimalDFTSizeTab[] = {
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

صحيح cv::getOptimalDFTSize( صحيح size0 )
{
    صحيح a = 0, b = حجم(optimalDFTSizeTab)/حجم(optimalDFTSizeTab[0]) - 1;
    𐡄𐡍( (unsigned)size0 >= (unsigned)optimalDFTSizeTab[b] )
        أرجع -1;

    طالما( a < b )
    {
        صحيح c = (a + b) >> 1;
        𐡄𐡍( size0 <= optimalDFTSizeTab[c] )
            b = c;
        وإلا
            a = c+1;
    }

    أرجع optimalDFTSizeTab[b];
}
/* End of file. */
