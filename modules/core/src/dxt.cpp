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

#إذا IPP_نسخو_X100 >= 710
#وصف_ڪريو USE_IPP_DFT 1
#وإلا
#وصف_رد USE_IPP_DFT
#جيڪڏهن_پورو

#إذا defined USE_IPP_DFT
#إذا IPP_نسخو_X100 >= 202220
#وصف_ڪريو IPP_DISABLE_DFT32F ((اونهائي == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#وإلا
#وصف_ڪريو IPP_DISABLE_DFT32F خطأ
#جيڪڏهن_پورو
#جيڪڏهن_پورو

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
// factorization used by the OpenCL radix selection
ساكن صحيح
DFTFactorize( صحيح n, صحيح* factors )
{
    صحيح nf = 0, f, i, j;

    إذا( n <= 5 )
    {
        factors[0] = n;
        أرجع 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    إذا( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    لكل( f = 3; n > 1; )
    {
        صحيح d = n/f;
        إذا( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        وإلا
        {
            f += 2;
            إذا( f*f > n )
                اكسر;
        }
    }

    إذا( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    لكل( i = f; i < (nf+f)/2; i++ )
        CV_مٽا_سٽا( factors[i], factors[nf-i-1+f], j );

    أرجع nf;
}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
ساكن Ipp_حالت ippsDFTFwd_CToC( ثابت Complex<عائم>* src, Complex<عائم>* dst,
                             ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (ثابت Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ثابت IppsDFTSpec_C_32fc*)spec, buf);
}

ساكن Ipp_حالت ippsDFTFwd_CToC( ثابت Complex<مزدوج>* src, Complex<مزدوج>* dst,
                             ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (ثابت Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ثابت IppsDFTSpec_C_64fc*)spec, buf);
}

ساكن Ipp_حالت ippsDFTInv_CToC( ثابت Complex<عائم>* src, Complex<عائم>* dst,
                             ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (ثابت Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ثابت IppsDFTSpec_C_32fc*)spec, buf);
}

ساكن Ipp_حالت ippsDFTInv_CToC( ثابت Complex<مزدوج>* src, Complex<مزدوج>* dst,
                                  ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (ثابت Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ثابت IppsDFTSpec_C_64fc*)spec, buf);
}

ساكن Ipp_حالت ippsDFTFwd_RToPack( ثابت عائم* src, عائم* dst,
                                     ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (ثابت IppsDFTSpec_R_32f*)spec, buf);
}

ساكن Ipp_حالت ippsDFTFwd_RToPack( ثابت مزدوج* src, مزدوج* dst,
                                     ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (ثابت IppsDFTSpec_R_64f*)spec, buf);
}

ساكن Ipp_حالت ipps_DFTInv_Pack_منزل_R( ثابت عائم* src, عائم* dst,
                                     ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_منزل_R_32f, src, dst, (ثابت IppsDFTSpec_R_32f*)spec, buf);
}

ساكن Ipp_حالت ipps_DFTInv_Pack_منزل_R( ثابت مزدوج* src, مزدوج* dst,
                                     ثابت فراغ* spec, uchar* buf)
{
    أرجع CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_منزل_R_64f, src, dst, (ثابت IppsDFTSpec_R_64f*)spec, buf);
}
#جيڪڏهن_پورو

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

#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#جيڪڏهن_پورو

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
#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
        ipp_spec = 0;
        ipp_work = 0;
#جيڪڏهن_پورو
        dft_func = 0;
    }
};


/****************************************************************************************\
                     New 1D engine: kernel table, plan builder, driver
\****************************************************************************************/

ساكن DFTKernels getDFTKernelsDispatch(صحيح اونهائي)
{
    CV_CPU_DISPATCH(getDFTKernels_, (اونهائي), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
ساكن ثابت DFTKernels& getDFTKernels(صحيح اونهائي)
{
    ساكن ثابت DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    ساكن ثابت DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    أرجع اونهائي == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
بنية DftTwiddleGen
{
    صحيح M, logB, نقاب_B;
    Auto_بفر<مزدوج, 1024> buf;
    مزدوج *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    ساكن فراغ fill_جدول(مزدوج* re, مزدوج* im, صحيح ڳڻپ_2, مزدوج wr, مزدوج wi, مزدوج قدم_Angle)
    {
        لكل (صحيح i0 = 0; i0 < ڳڻپ_2; i0 += 16)
        {
            مزدوج t0 = قدم_Angle*i0;
            مزدوج cr = i0 == 0 ? 1 : جيب_تمام(t0), ci = i0 == 0 ? 0 : جيب(t0);
            صحيح i1 = std::min(i0 + 16, ڳڻپ_2);
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
        نقاب_B = B - 1;
        صحيح nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.معلومات(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        مزدوج a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fill_جدول(lo_re, lo_im, B, جيب_تمام(a), جيب(a), a);
        fill_جدول(hi_re, hi_im, nhi, جيب_تمام(a*B), جيب(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    فراغ get(صحيح m, مزدوج& re, مزدوج& im) ثابت
    {
        CV_DbgAssert(m >= 0);
        إذا (m >= M)
            m %= M;
        صحيح a = m >> logB, b = m & نقاب_B;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

سانچو<قسم_نالو T>
فراغ DftPlan::fillTables(size_t ntw, منطقي need_rtw, منطقي need_dct)
{
    DftPlan& p = *هذا;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    صحيح M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*حجم(T) + حجم(مزدوج) - 1)/حجم(مزدوج) + 1);
    T* cur = (T*)p.tw.معلومات();
    صحيح maxL = 1;
    لكل (صحيح s = 0; s < p.nstages; s++)
        إذا (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].وسعت);
    Auto_بفر<مزدوج, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    مزدوج* twd = twd_buf.معلومات();
    لكل (صحيح s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        صحيح r = st.radix, L = st.وسعت, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        صحيح قدم = M/(r*L);
        إذا (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr صحيح K = 8;
            مزدوج cr[K], ci[K], wr, wi;
            gen.get(K*قدم, wr, wi);
            مزدوج* t1r = twd; مزدوج* t1i = twd + L;      // leg 1 kept in double for the products
            لكل (صحيح j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                لكل (صحيح k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*قدم, cr[k], ci[k]);
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
                    gen.get(q*j*قدم, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        إذا (r >= 7)
        {
            صحيح h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            لكل (صحيح pp = 1; pp <= h; pp++)
                لكل (صحيح k = 1; k <= h; k++)
                {
                    مزدوج t = 2*CV_PI*((مزدوج)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)جيب_تمام(t);
                    sn[(pp-1)*h + (k-1)] = (T)جيب(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    إذا (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        صحيح قدم = M/n;
        لكل (صحيح k = 0; k <= nc; k++)
        {
            مزدوج re, im;
            gen.get(k*قدم, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    إذا (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        مزدوج scale = (p.قسم == DFT_قسم_DCT ? 2 : 1)*std::جذر(1./(2*n));
        صحيح قدم = M/(4*n);
        لكل (صحيح k = 0; k <= nc; k++)
        {
            مزدوج re, im;
            gen.get(k*قدم, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
فراغ DftPlan::build(صحيح قسم_2, صحيح _n, صحيح اونهائي_2, صحيح _vl)
{
    DftPlan& p = *هذا;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(اونهائي_2 == CV_32F || اونهائي_2 == CV_64F);
    منطقي real_قسم = قسم_2 != DFT_قسم_C2_C;
    منطقي dct_قسم = قسم_2 == DFT_قسم_DCT || قسم_2 == DFT_قسم_IDCT;
    إذا (dct_قسم)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.قسم = قسم_2; p.n = _n; p.اونهائي = اونهائي_2; p.vl = _vl;
    p.nc = real_قسم && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = اونهائي == CV_32F ? حجم(عائم) : حجم(مزدوج);
    p.real_داخلا = قسم == DFT_قسم_R2_C && (n & 1) != 0;
    p.need_tmp = قسم == DFT_قسم_C2_R || قسم == DFT_قسم_DCT || قسم == DFT_قسم_IDCT;
    p.pingpong = خطأ;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    صحيح k2 = 0, rest = nc;
    طالما ((rest & 1) == 0) { rest >>= 1; k2++; }
    صحيح r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.پهريون_radix = r0;
    صحيح radices[DFT_MAX_STAGES], nst = 0;
    لكل (صحيح m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    طالما (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    طالما (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    لكل (صحيح f = 7; rest > 1; f += 2)
    {
        إذا (f*f > rest)
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
        st.radix = r; st.وسعت = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        إذا (vl > 1 && L >= vl)
        {
            st.گهڻائي = DFT_STAGE_ڀريل;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            إذا (L % vl != 0)
                p.pingpong = سچ;   // back-off clamp => overlapping stores => must be out-of-place
        }
        وإلا
        {
            st.گهڻائي = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        إذا (r >= 7)
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
    إذا (r0 > 1) seq[nseq++] = r0;
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
    إذا (r0 == 1)
    {
        p.itab.allocate(nc*2);
        صحيح* it_all = p.itab.معلومات();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        صحيح n_اندريون = std::min(nseq, 2);
        صحيح r_0 = n_اندريون > 0 ? seq[0] : 1, w_0 = n_اندريون > 0 ? weights[0] : 0;
        صحيح r_1 = n_اندريون > 1 ? seq[1] : 1, w_1 = n_اندريون > 1 ? weights[1] : 0;
        لكل (صحيح pos = 0, rev_hi = 0; pos < nc; )
        {
            لكل (صحيح d1 = 0; d1 < r_1; d1++)
            {
                صحيح rev = rev_hi + d1*w_1;
                صحيح* it = it_all + pos*2;
                إذا (p.real_داخلا)
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
            لكل (صحيح s = n_اندريون; s < nseq; s++)
            {
                rev_hi += weights[s];
                إذا (++digits[s] < seq[s])
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
        صحيح* gtab = p.itab.معلومات();
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
        لكل (صحيح بنياد = 0, g_hi = 0; بنياد < M; )
        {
            لكل (صحيح db = 0; db < r_b; db++)
            {
                صحيح g = (g_hi + db*g_b)*r0;
                لكل (صحيح da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[بنياد++] = g;
            }
            لكل (صحيح s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                إذا (++digits[s] < seq[s])
                    اكسر;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    منطقي need_rtw = real_قسم && n % 2 == 0;
    إذا (need_rtw) ntw += 2*(size_t)(nc + 1);
    إذا (dct_قسم) ntw += 2*(size_t)(nc + 1);
    إذا (اونهائي == CV_32F)
        fillTables<عائم>(ntw, need_rtw, dct_قسم);
    وإلا
        fillTables<مزدوج>(ntw, need_rtw, dct_قسم);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_بائيٽون = 2*(size_t)nc*esz;
    منطقي two_جوڙا = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_جوڙا ? pair_بائيٽون : 0;
    p.scratch_ofs = pair_بائيٽون*(two_جوڙا ? 2 : 1);
    size_t scratch_بائيٽون = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_بائيٽون = p.scratch_ofs + scratch_بائيٽون + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
ساكن فراغ لاڳيتو_Dft(ثابت OcvDftOptions& c, ثابت فراغ* src, size_t sstep, فراغ* dst, size_t dstep,
                   منطقي complex_io)
{
    ثابت DftPlan& p = *c.plan;
    ثابت DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.اونهائي == CV_32F ? حجم(عائم) : حجم(مزدوج);
    uchar* بنياد = alignPtr(c.workspace, 64);
    uchar* re0 = بنياد;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = بنياد + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = بنياد + p.scratch_ofs;

    ثابت فراغ* asrc = src;
    size_t astep = sstep;
    منطقي conj = خطأ;
    إذا (p.قسم == DFT_قسم_C2_R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = سچ;
    }
    وإلا إذا (p.قسم == DFT_قسم_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = سچ;
    }
    وإلا إذا (p.قسم == DFT_قسم_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    وإلا إذا (p.قسم == DFT_قسم_C2_C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.پهريون_radix == 1);

    DftPreprocFunc pre = p.پهريون_radix == 1 ? k.preprocRadix0 : p.پهريون_radix == 2 ? k.preprocRadix2 :
                         p.پهريون_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    لكل (صحيح i = 0; i < p.nstages; i++)
    {
        ثابت DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        إذا (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        وإلا
            fn(st, sre, sim, sre, sim, scratch);
    }

    مزدوج scale = c.scale;
    بدل (p.قسم)
    {
    حالة DFT_قسم_C2_C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        اكسر;
    حالة DFT_قسم_R2_C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        اكسر;
    حالة DFT_قسم_C2_R:
        إذا (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        وإلا
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        اكسر;
    حالة DFT_قسم_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        اكسر;
    افتراضي:
        CV_Assert(p.قسم == DFT_قسم_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
سانچو<قسم_نالو T> ساكن فراغ
DFT(ثابت OcvDftOptions & c, ثابت Complex<T>* src, Complex<T>* dst)
{
    إذا( c.useIpp )
    {
#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
        إذا( !c.isInverse )
        {
            إذا (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_جوڙيو(CV_IMPL_IPP);
                أرجع;
            }
        }
        وإلا
        {
            إذا (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_جوڙيو(CV_IMPL_IPP);
                أرجع;
            }
        }
        سيٽ_Ipp_Error_حالت();
#جيڪڏهن_پورو
    }

    لاڳيتو_Dft(c, src, 1, dst, 1, خطأ);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
سانچو<قسم_نالو T> ساكن فراغ
RealDFT(ثابت OcvDftOptions & c, ثابت T* src, T* dst)
{
    صحيح complex_output = c.isComplex;

    إذا( c.useIpp )
    {
#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
        صحيح n = c.n;
        T* ipp_dst = dst + complex_output;
        إذا (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            إذا( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                إذا( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_جوڙيو(CV_IMPL_IPP);
            أرجع;
        }
        سيٽ_Ipp_Error_حالت();
#جيڪڏهن_پورو
    }
    لاڳيتو_Dft(c, src, 1, dst, 1, complex_output != 0);
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
سانچو<قسم_نالو T> ساكن فراغ
CCSIDFT(ثابت OcvDftOptions & c, ثابت T* src, T* dst)
{
    صحيح complex_داخلا = c.isComplex;

    إذا( complex_داخلا )
        CV_Assert( src != dst );
    إذا( c.useIpp )
    {
#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        ثابت T* ipp_src = src;
        T save_s1 = 0;
        إذا( complex_داخلا )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        Ipp_حالت حالت = ipps_DFTInv_Pack_منزل_R( ipp_src, dst, c.ipp_spec, c.ipp_work );
        إذا( complex_داخلا )
            ((T*)src)[1] = save_s1;
        إذا( حالت >= 0 )
        {
            CV_IMPL_جوڙيو(CV_IMPL_IPP);
            أرجع;
        }
        سيٽ_Ipp_Error_حالت();
#جيڪڏهن_پورو
    }
    لاڳيتو_Dft(c, src, 1, dst, 1, complex_داخلا != 0);
}

ساكن فراغ
Copy_ڪالم( ثابت uchar* _src, size_t src_قدم,
            uchar* _dst, size_t dst_قدم,
            صحيح len, size_t elem_سائيز_2 )
{
    صحيح i, t0, t1;
    ثابت صحيح* src = (ثابت صحيح*)_src;
    صحيح* dst = (صحيح*)_dst;
    src_قدم /= حجم(src[0]);
    dst_قدم /= حجم(dst[0]);

    إذا( elem_سائيز_2 == حجم(صحيح) )
    {
        لكل( i = 0; i < len; i++, src += src_قدم, dst += dst_قدم )
            dst[0] = src[0];
    }
    وإلا إذا( elem_سائيز_2 == حجم(صحيح)*2 )
    {
        لكل( i = 0; i < len; i++, src += src_قدم, dst += dst_قدم )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    وإلا إذا( elem_سائيز_2 == حجم(صحيح)*4 )
    {
        لكل( i = 0; i < len; i++, src += src_قدم, dst += dst_قدم )
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
سانچو<قسم_نالو T> ساكن مضمن فراغ
copyFromColumns_( ثابت uchar* src, size_t src_قدم, T* dst, صحيح ncols, صحيح len )
{
    لكل( صحيح i = 0; i < len; i++, src += src_قدم )
    {
        ثابت T* s = (ثابت T*)src;
        لكل( صحيح k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

ساكن فراغ
CopyFromColumns( ثابت uchar* src, size_t src_قدم, uchar* dst, صحيح ncols, صحيح len, size_t elem_سائيز_2 )
{
    إذا( elem_سائيز_2 == حجم(صحيح) )
        copyFromColumns_(src, src_قدم, (صحيح*)dst, ncols, len);
    وإلا إذا( elem_سائيز_2 == حجم(int64) )
        copyFromColumns_(src, src_قدم, (int64*)dst, ncols, len);
    وإلا
    {
        CV_Assert( elem_سائيز_2 == حجم(int64)*2 );
        copyFromColumns_(src, src_قدم, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
سانچو<قسم_نالو T> ساكن مضمن فراغ
copy_منزل_Columns( ثابت T* src, uchar* dst, size_t dst_قدم, صحيح ncols, صحيح len )
{
    لكل( صحيح i = 0; i < len; i++, dst += dst_قدم )
    {
        T* d = (T*)dst;
        لكل( صحيح k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

ساكن فراغ
Copy_منزل_Columns( ثابت uchar* src, uchar* dst, size_t dst_قدم, صحيح ncols, صحيح len, size_t elem_سائيز_2 )
{
    إذا( elem_سائيز_2 == حجم(صحيح) )
        copy_منزل_Columns((ثابت صحيح*)src, dst, dst_قدم, ncols, len);
    وإلا إذا( elem_سائيز_2 == حجم(int64) )
        copy_منزل_Columns((ثابت int64*)src, dst, dst_قدم, ncols, len);
    وإلا
    {
        CV_Assert( elem_سائيز_2 == حجم(int64)*2 );
        copy_منزل_Columns((ثابت Vec<int64, 2>*)src, dst, dst_قدم, ncols, len);
    }
}

ساكن فراغ
ExpandCCS( uchar* _ptr, صحيح n, صحيح elem_سائيز_2 )
{
    صحيح i;
    إذا( elem_سائيز_2 == (صحيح)حجم(عائم) )
    {
        عائم* p = (عائم*)_ptr;
        لكل( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        إذا( (n & 1) == 0 )
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
        إذا( (n & 1) == 0 )
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

سانچو<قسم_نالو T, فراغ (*fn)(ثابت OcvDftOptions&, ثابت T*, T*)>
ساكن فراغ dftWrap(ثابت OcvDftOptions & c, ثابت فراغ* src, فراغ* dst)
{
    fn(c, (ثابت T*)src, (T*)dst);
}

}

#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
تعريفنوع Ipp_حالت (CV_STDCALL* Ipp_DFTGet_سائيز_Func)(صحيح, صحيح, IppHintAlgorithm, صحيح*, صحيح*, صحيح*);
تعريفنوع Ipp_حالت (CV_STDCALL* IppDFTInitFunc)(صحيح, صحيح, IppHintAlgorithm, فراغ*, uchar*);

سانچو<قسم_نالو SpecType, Ipp_حالت (CV_STDCALL *init_fn)(صحيح, صحيح, IppHintAlgorithm, SpecType*, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ippDFTInitWrap(صحيح n, صحيح flags, IppHintAlgorithm hint, فراغ* spec, Ipp8u* initbuf)
{
    أرجع init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#جيڪڏهن_پورو

نطاق cv
{
#إذا defined USE_IPP_DFT

تعريفنوع Ipp_حالت (CV_STDCALL* ippiDFT_C_Func)(ثابت Ipp32fc*, صحيح, Ipp32fc*, صحيح, ثابت IppiDFTSpec_C_32fc*, Ipp8u*);
تعريفنوع Ipp_حالت (CV_STDCALL* ippiDFT_R_Func)(ثابت Ipp32f* , صحيح, Ipp32f* , صحيح, ثابت IppiDFTSpec_R_32f* , Ipp8u*);

سانچو <قسم_نالو Dft>
صنف Dft_C_IPPLoop_Invoker : عام ParallelLoopBody
{
عام:

    Dft_C_IPPLoop_Invoker(ثابت uchar * _src, size_t src_قدم_2, uchar * _dst, size_t dst_قدم_2, صحيح ويڪر_3,
                          ثابت Dft& _ippidft, صحيح _norm_flag, منطقي *_ok) :
        ParallelLoopBody(),
        src(_src), src_قدم(src_قدم_2), dst(_dst), dst_قدم(dst_قدم_2), ويڪر(ويڪر_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = سچ;
    }

    مجازي فراغ عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        Ipp_حالت حالت;
        Ipp8u* p_بفر = 0;
        Ipp8u* pMemInit= 0;
        صحيح سائيز_بفر=0;
        صحيح سائيز_Spec=0;
        صحيح سائيز_Init=0;

        Ippi_سائيز src_Roi_سائيز = {ويڪر, 1};

        حالت = ippi_DFTGet_سائيز_C_32fc(src_Roi_سائيز, norm_flag, ippAlgHintNone, &سائيز_Spec, &سائيز_Init, &سائيز_بفر );
        إذا ( حالت < 0 )
        {
            *ok = خطأ;
            أرجع;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( سائيز_Spec );

        إذا ( سائيز_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( سائيز_Init );

        إذا ( سائيز_بفر > 0 )
            p_بفر = (Ipp8u*)CV_IPP_MALLOC( سائيز_بفر );

        حالت = ippiDFTInit_C_32fc( src_Roi_سائيز, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        إذا ( سائيز_Init > 0 )
            ippFree( pMemInit );

        إذا ( حالت < 0 )
        {
            ippFree( pDFTSpec );
            إذا ( سائيز_بفر > 0 )
                ippFree( p_بفر );
            *ok = خطأ;
            أرجع;
        }

        لكل( صحيح i = range.شروعات; i < range.end; ++i)
            إذا(!ippidft((Ipp32fc*)(src + src_قدم * i), src_قدم, (Ipp32fc*)(dst + dst_قدم * i), dst_قدم,
                        pDFTSpec, (Ipp8u*)p_بفر))
            {
                *ok = خطأ;
            }

        إذا ( سائيز_بفر > 0 )
            ippFree( p_بفر );

        ippFree( pDFTSpec );
        CV_IMPL_جوڙيو(CV_IMPL_IPP|CV_IMPL_MT);
    }

خاص:
    ثابت uchar * src;
    size_t src_قدم;
    uchar * dst;
    size_t dst_قدم;
    صحيح ويڪر;
    ثابت Dft& ippidft;
    صحيح norm_flag;
    منطقي *ok;

    ثابت Dft_C_IPPLoop_Invoker& عامل= (ثابت Dft_C_IPPLoop_Invoker&);
};

سانچو <قسم_نالو Dft>
صنف Dft_R_IPPLoop_Invoker : عام ParallelLoopBody
{
عام:

    Dft_R_IPPLoop_Invoker(ثابت uchar * _src, size_t src_قدم_2, uchar * _dst, size_t dst_قدم_2, صحيح ويڪر_3,
                          ثابت Dft& _ippidft, صحيح _norm_flag, منطقي *_ok) :
        ParallelLoopBody(),
        src(_src), src_قدم(src_قدم_2), dst(_dst), dst_قدم(dst_قدم_2), ويڪر(ويڪر_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = سچ;
    }

    مجازي فراغ عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        Ipp_حالت حالت;
        Ipp8u* p_بفر = 0;
        Ipp8u* pMemInit= 0;
        صحيح سائيز_بفر=0;
        صحيح سائيز_Spec=0;
        صحيح سائيز_Init=0;

        Ippi_سائيز src_Roi_سائيز = {ويڪر, 1};

        حالت = ippi_DFTGet_سائيز_R_32f(src_Roi_سائيز, norm_flag, ippAlgHintNone, &سائيز_Spec, &سائيز_Init, &سائيز_بفر );
        إذا ( حالت < 0 )
        {
            *ok = خطأ;
            أرجع;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( سائيز_Spec );

        إذا ( سائيز_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( سائيز_Init );

        إذا ( سائيز_بفر > 0 )
            p_بفر = (Ipp8u*)CV_IPP_MALLOC( سائيز_بفر );

        حالت = ippiDFTInit_R_32f( src_Roi_سائيز, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        إذا ( سائيز_Init > 0 )
            ippFree( pMemInit );

        إذا ( حالت < 0 )
        {
            ippFree( pDFTSpec );
            إذا ( سائيز_بفر > 0 )
                ippFree( p_بفر );
            *ok = خطأ;
            أرجع;
        }

        لكل( صحيح i = range.شروعات; i < range.end; ++i)
            إذا(!ippidft((عائم*)(src + src_قدم * i), src_قدم, (عائم*)(dst + dst_قدم * i), dst_قدم,
                        pDFTSpec, (Ipp8u*)p_بفر))
            {
                *ok = خطأ;
            }

        إذا ( سائيز_بفر > 0 )
            ippFree( p_بفر );

        ippFree( pDFTSpec );
        CV_IMPL_جوڙيو(CV_IMPL_IPP|CV_IMPL_MT);
    }

خاص:
    ثابت uchar * src;
    size_t src_قدم;
    uchar * dst;
    size_t dst_قدم;
    صحيح ويڪر;
    ثابت Dft& ippidft;
    صحيح norm_flag;
    منطقي *ok;

    ثابت Dft_R_IPPLoop_Invoker& عامل= (ثابت Dft_R_IPPLoop_Invoker&);
};

سانچو <قسم_نالو Dft>
منطقي Dft_C_IPPLoop(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحيح ويڪر, صحيح اوچائي, ثابت Dft& ippidft, صحيح norm_flag)
{
    منطقي ok;
    parallel_for_(Range(0, اوچائي), Dft_C_IPPLoop_Invoker<Dft>(src, src_قدم, dst, dst_قدم, ويڪر, ippidft, norm_flag, &ok), (ويڪر * اوچائي)/(مزدوج)(1<<16) );
    أرجع ok;
}

سانچو <قسم_نالو Dft>
منطقي Dft_R_IPPLoop(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحيح ويڪر, صحيح اوچائي, ثابت Dft& ippidft, صحيح norm_flag)
{
    منطقي ok;
    parallel_for_(Range(0, اوچائي), Dft_R_IPPLoop_Invoker<Dft>(src, src_قدم, dst, dst_قدم, ويڪر, ippidft, norm_flag, &ok), (ويڪر * اوچائي)/(مزدوج)(1<<16) );
    أرجع ok;
}

بنية IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    منطقي عامل()(ثابت Ipp32fc* src, size_t src_قدم_3, Ipp32fc* dst, size_t dst_قدم_3, ثابت IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* p_بفر) ثابت
    {
        أرجع ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<صحيح>(src_قدم_3), dst, static_cast<صحيح>(dst_قدم_3), pDFTSpec, p_بفر) >= 0 : خطأ;
    }
خاص:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

بنية IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippi_DFT_Pack_منزل_R_32f_C1_R(_func){}

    منطقي عامل()(ثابت Ipp32f* src, size_t src_قدم_3, Ipp32f* dst, size_t dst_قدم_3, ثابت IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* p_بفر) ثابت
    {
        أرجع ippi_DFT_Pack_منزل_R_32f_C1_R ? CV_INSTRUMENT_FUN_IPP(ippi_DFT_Pack_منزل_R_32f_C1_R, src, static_cast<صحيح>(src_قدم_3), dst, static_cast<صحيح>(dst_قدم_3), pDFTSpec, p_بفر) >= 0 : خطأ;
    }
خاص:
    ippiDFT_R_Func ippi_DFT_Pack_منزل_R_32f_C1_R;
};

ساكن منطقي ippi_DFT_C_32F(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحيح ويڪر, صحيح اوچائي, منطقي inv, صحيح norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_حالت حالت;
    Ipp8u* p_بفر = 0;
    Ipp8u* pMemInit= 0;
    صحيح سائيز_بفر=0;
    صحيح سائيز_Spec=0;
    صحيح سائيز_Init=0;

    Ippi_سائيز src_Roi_سائيز = {ويڪر, اوچائي};

    حالت = ippi_DFTGet_سائيز_C_32fc(src_Roi_سائيز, norm_flag, ippAlgHintNone, &سائيز_Spec, &سائيز_Init, &سائيز_بفر );
    إذا ( حالت < 0 )
        أرجع خطأ;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( سائيز_Spec );

    إذا ( سائيز_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( سائيز_Init );

    إذا ( سائيز_بفر > 0 )
        p_بفر = (Ipp8u*)CV_IPP_MALLOC( سائيز_بفر );

    حالت = ippiDFTInit_C_32fc( src_Roi_سائيز, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    إذا ( سائيز_Init > 0 )
        ippFree( pMemInit );

    إذا ( حالت < 0 )
    {
        ippFree( pDFTSpec );
        إذا ( سائيز_بفر > 0 )
            ippFree( p_بفر );
        أرجع خطأ;
    }

    إذا (!inv)
        حالت = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<صحيح>(src_قدم), (Ipp32fc*)dst, static_cast<صحيح>(dst_قدم), pDFTSpec, p_بفر);
    وإلا
        حالت = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<صحيح>(src_قدم), (Ipp32fc*)dst, static_cast<صحيح>(dst_قدم), pDFTSpec, p_بفر);

    إذا ( سائيز_بفر > 0 )
        ippFree( p_بفر );

    ippFree( pDFTSpec );

    إذا(حالت >= 0)
    {
        CV_IMPL_جوڙيو(CV_IMPL_IPP);
        أرجع سچ;
    }
    أرجع خطأ;
}

ساكن منطقي ippi_DFT_R_32F(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحيح ويڪر, صحيح اوچائي, منطقي inv, صحيح norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_حالت حالت;
    Ipp8u* p_بفر = 0;
    Ipp8u* pMemInit= 0;
    صحيح سائيز_بفر=0;
    صحيح سائيز_Spec=0;
    صحيح سائيز_Init=0;

    Ippi_سائيز src_Roi_سائيز = {ويڪر, اوچائي};

    حالت = ippi_DFTGet_سائيز_R_32f(src_Roi_سائيز, norm_flag, ippAlgHintNone, &سائيز_Spec, &سائيز_Init, &سائيز_بفر );
    إذا ( حالت < 0 )
        أرجع خطأ;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( سائيز_Spec );

    إذا ( سائيز_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( سائيز_Init );

    إذا ( سائيز_بفر > 0 )
        p_بفر = (Ipp8u*)CV_IPP_MALLOC( سائيز_بفر );

    حالت = ippiDFTInit_R_32f( src_Roi_سائيز, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    إذا ( سائيز_Init > 0 )
        ippFree( pMemInit );

    إذا ( حالت < 0 )
    {
        ippFree( pDFTSpec );
        إذا ( سائيز_بفر > 0 )
            ippFree( p_بفر );
        أرجع خطأ;
    }

    إذا (!inv)
        حالت = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (عائم*)src, static_cast<صحيح>(src_قدم), (عائم*)dst, static_cast<صحيح>(dst_قدم), pDFTSpec, p_بفر);
    وإلا
        حالت = CV_INSTRUMENT_FUN_IPP(ippi_DFTInv_Pack_منزل_R_32f_C1_R, (عائم*)src, static_cast<صحيح>(src_قدم), (عائم*)dst, static_cast<صحيح>(dst_قدم), pDFTSpec, p_بفر);

    إذا ( سائيز_بفر > 0 )
        ippFree( p_بفر );

    ippFree( pDFTSpec );

    إذا(حالت >= 0)
    {
        CV_IMPL_جوڙيو(CV_IMPL_IPP);
        أرجع سچ;
    }
    أرجع خطأ;
}

#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

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
    صحيح thread_ڳڻپ;
    صحيح dft_سائيز;
    صحيح dft_اونهائي;
    منطقي حالت;

عام:
    OCL_FftPlan(صحيح سائيز_4, صحيح اونهائي_2) : dft_سائيز(سائيز_4), dft_اونهائي(اونهائي_2), حالت(سچ)
    {
        CV_Assert( dft_اونهائي == CV_32F || dft_اونهائي == CV_64F );

        صحيح min_radix;
        std::vector<صحيح> radixes, بلاڪون;
        ocl_getRadixes(dft_سائيز, radixes, بلاڪون, min_radix);
        thread_ڳڻپ = dft_سائيز / min_radix;

        إذا (thread_ڳڻپ > (صحيح) ocl::Device::getDefault().max_Work_گروهه_سائيز())
        {
            حالت = خطأ;
            أرجع;
        }

        // generate string with radix calls
        String radix_processing;
        صحيح n = 1, twiddle_سائيز = 0;
        لكل (size_t i=0; i<radixes.size(); i++)
        {
            صحيح radix = radixes[i], بلاڪ_2 = بلاڪون[i];
            إذا (بلاڪ_2 > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, بلاڪ_2, twiddle_سائيز, n, dft_سائيز/radix);
            وإلا
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_سائيز, n, dft_سائيز/radix);
            twiddle_سائيز += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_سائيز, CV_MAKE_TYPE(dft_اونهائي, 2));
        إذا (dft_اونهائي == CV_32F)
            fill_Radix_جدول<عائم>(twiddles, radixes);
        وإلا
            fill_Radix_جدول<مزدوج>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_سائيز, min_radix, ocl::type_منزل_Str(dft_اونهائي), ocl::type_منزل_Str(CV_MAKE_TYPE(dft_اونهائي, 2)),
                              dft_اونهائي == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    منطقي قطار_اندر_وجهو_Transform(داخلا_Array _src, OutputArray _dst, صحيح انگ_dfts, صحيح flags, صحيح fftType, منطقي صفون = سچ) ثابت
    {
        إذا (!حالت)
            أرجع خطأ;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        منطقي is1d = (flags & DFT_صفون) != 0 || انگ_dfts == 1;
        منطقي inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        إذا (صفون)
        {
            globalsize[0] = thread_ڳڻپ; globalsize[1] = src.صفون;
            localsize[0] = thread_ڳڻپ; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            إذا ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        وإلا
        {
            globalsize[0] = انگ_dfts; globalsize[1] = thread_ڳڻپ;
            localsize[0] = 1; localsize[1] = thread_ڳڻپ;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            إذا (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        إذا (!inv)
        {
            إذا ((is1d && src.channels() == 1) || (صفون && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        وإلا
        {
            إذا (صفون && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            إذا (dst.کانن_ڳڻپ % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        إذا (k.empty())
            أرجع خطأ;

        k.دليلون(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::Read_Only_No_سائيز(twiddles), thread_ڳڻپ, انگ_dfts);
        أرجع k.لاڳيتو(2, globalsize, localsize, خطأ);
    }

خاص:
    ساكن فراغ ocl_getRadixes(صحيح کانن_ڳڻپ, std::vector<صحيح>& radixes, std::vector<صحيح>& بلاڪون, صحيح& min_radix)
    {
        صحيح factors[34];
        صحيح nf = DFTFactorize(کانن_ڳڻپ, factors);

        صحيح n = 1;
        صحيح ضرب_ڪندڙ_اشاريو = 0;
        min_radix = وڌ_کان_وڌ_عدد;

        // 2^n transforms
        إذا ((factors[ضرب_ڪندڙ_اشاريو] & 1) == 0)
        {
            لكل( ; n < factors[ضرب_ڪندڙ_اشاريو];)
            {
                صحيح radix = 2, بلاڪ_2 = 1;
                إذا (8*n <= factors[0])
                    radix = 8;
                وإلا إذا (4*n <= factors[0])
                {
                    radix = 4;
                    إذا (کانن_ڳڻپ % 12 == 0)
                        بلاڪ_2 = 3;
                    وإلا إذا (کانن_ڳڻپ % 8 == 0)
                        بلاڪ_2 = 2;
                }
                وإلا
                {
                    إذا (کانن_ڳڻپ % 10 == 0)
                        بلاڪ_2 = 5;
                    وإلا إذا (کانن_ڳڻپ % 8 == 0)
                        بلاڪ_2 = 4;
                    وإلا إذا (کانن_ڳڻپ % 6 == 0)
                        بلاڪ_2 = 3;
                    وإلا إذا (کانن_ڳڻپ % 4 == 0)
                        بلاڪ_2 = 2;
                }

                radixes.push_back(radix);
                بلاڪون.push_back(بلاڪ_2);
                min_radix = min(min_radix, بلاڪ_2*radix);
                n *= radix;
            }
            ضرب_ڪندڙ_اشاريو++;
        }

        // all the other transforms
        لكل( ; ضرب_ڪندڙ_اشاريو < nf; ضرب_ڪندڙ_اشاريو++)
        {
            صحيح radix = factors[ضرب_ڪندڙ_اشاريو], بلاڪ_2 = 1;
            إذا (radix == 3)
            {
                إذا (کانن_ڳڻپ % 12 == 0)
                    بلاڪ_2 = 4;
                وإلا إذا (کانن_ڳڻپ % 9 == 0)
                    بلاڪ_2 = 3;
                وإلا إذا (کانن_ڳڻپ % 6 == 0)
                    بلاڪ_2 = 2;
            }
            وإلا إذا (radix == 5)
            {
                إذا (کانن_ڳڻپ % 10 == 0)
                    بلاڪ_2 = 2;
            }
            radixes.push_back(radix);
            بلاڪون.push_back(بلاڪ_2);
            min_radix = min(min_radix, بلاڪ_2*radix);
        }
    }

    سانچو <قسم_نالو T>
    ساكن فراغ fill_Radix_جدول(UMat twiddles, ثابت std::vector<صحيح>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        صحيح ptr_اشاريو = 0;

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
                    ptr[ptr_اشاريو++] = (T) جيب_تمام(k*theta);
                    ptr[ptr_اشاريو++] = (T) جيب(k*theta);
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

    Ptr<OCL_FftPlan> getFftPlan(صحيح dft_سائيز, صحيح اونهائي)
    {
        صحيح ڪنجي = (dft_سائيز << 16) | (اونهائي & 0xFFFF);
        std::map<صحيح, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(ڪنجي);
        إذا (f != planStorage.end())
        {
            أرجع f->second;
        }
        وإلا
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(جديد OCL_FftPlan(dft_سائيز, اونهائي));
            planStorage[ڪنجي] = newPlan;
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

ساكن منطقي ocl_dft_صفون(داخلا_Array _src, OutputArray _dst, صحيح nonzero_صفون, صحيح flags, صحيح fftType)
{
    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.کانن_ڳڻپ(), اونهائي);
    أرجع plan->قطار_اندر_وجهو_Transform(_src, _dst, nonzero_صفون, flags, fftType, سچ);
}

ساكن منطقي ocl_dft_کانن_ڳڻپ(داخلا_Array _src, OutputArray _dst, صحيح nonzero_کانن_ڳڻپ, صحيح flags, صحيح fftType)
{
    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.صفون(), اونهائي);
    أرجع plan->قطار_اندر_وجهو_Transform(_src, _dst, nonzero_کانن_ڳڻپ, flags, fftType, خطأ);
}

مضمن FftType determineFFTType(منطقي real_داخلا, منطقي complex_داخلا, منطقي real_output, منطقي complex_output, منطقي inv)
{
    // output format is not specified
    إذا (!real_output && !complex_output)
        complex_output = سچ;

    // input or output format is ambiguous
    إذا (real_داخلا == complex_داخلا || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType نتيجو = real_داخلا ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    إذا (نتيجو == C2R && !inv)
        نتيجو = C2C;

    // Inverse CCS to Complex not supported
    إذا (نتيجو == R2C && inv)
        نتيجو = R2R;

    أرجع نتيجو;
}

ساكن منطقي ocl_dft(داخلا_Array _src, OutputArray _dst, صحيح flags, صحيح nonzero_صفون)
{
    صحيح type = _src.type(), cn = CV_MAT_CN(type), اونهائي = CV_MAT_اونهائي(type);
    سائيز ssize = _src.size();
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (!(cn == 1 || cn == 2)
        || !(اونهائي == CV_32F || (اونهائي == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        أرجع خطأ;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    إذا (ssize.area() != getOptimalDFTSize(ssize.area()))
        أرجع خطأ;

    UMat src = _src.getUMat();
    منطقي inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    إذا( nonzero_صفون <= 0 || nonzero_صفون > _src.صفون() )
        nonzero_صفون = _src.صفون();
    منطقي is1d = (flags & DFT_صفون) != 0 || nonzero_صفون == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    إذا (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.create_Same_سائيز(src, CV_MAKETYPE(اونهائي, 2));
        output = _dst.getUMat();
    }
    وإلا
    {
        // real output
        إذا (is1d)
        {
            _dst.create_Same_سائيز(src, CV_MAKETYPE(اونهائي, 1));
            output = _dst.getUMat();
        }
        وإلا
        {
            _dst.create_Same_سائيز(src, CV_MAKETYPE(اونهائي, 1));
            output.create(src.size, CV_MAKETYPE(اونهائي, 2));
        }
    }

    منطقي نتيجو = خطأ;
    إذا (!inv)
    {
        صحيح nonzero_کانن_ڳڻپ = fftType == R2R ? output.کانن_ڳڻپ/2 + 1 : output.کانن_ڳڻپ;
        نتيجو = ocl_dft_صفون(src, output, nonzero_صفون, flags, fftType);
        إذا (!is1d)
            نتيجو = نتيجو && ocl_dft_کانن_ڳڻپ(output, _dst, nonzero_کانن_ڳڻپ, flags, fftType);
    }
    وإلا
    {
        إذا (fftType == C2C)
        {
            // complex output
            نتيجو = ocl_dft_صفون(src, output, nonzero_صفون, flags, fftType);
            إذا (!is1d)
                نتيجو = نتيجو && ocl_dft_کانن_ڳڻپ(output, output, output.کانن_ڳڻپ, flags, fftType);
        }
        وإلا
        {
            إذا (is1d)
            {
                نتيجو = ocl_dft_صفون(src, output, nonzero_صفون, flags, fftType);
            }
            وإلا
            {
                صحيح nonzero_کانن_ڳڻپ = src.کانن_ڳڻپ/2 + 1;
                نتيجو = ocl_dft_کانن_ڳڻپ(src, output, nonzero_کانن_ڳڻپ, flags, fftType);
                نتيجو = نتيجو && ocl_dft_صفون(output, _dst, nonzero_صفون, flags, fftType);
            }
        }
    }
    أرجع نتيجو;
}

} // namespace cv;

#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_CLAMDFFT

نطاق cv {

#وصف_ڪريو CLAMDDFT_Assert(func) \
    { \
        clfft_حالت s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

صنف PlanCache
{
    بنية FftPlan
    {
        FftPlan(ثابت سائيز & dft_سائيز_2, صحيح src_قدم_2, صحيح dst_قدم_2, منطقي _doubleFP, منطقي _inplace, صحيح _flags, FftType _fftType) :
            dft_سائيز(dft_سائيز_2), src_قدم(src_قدم_2), dst_قدم(dst_قدم_2),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            منطقي dft_inverse = (flags & DFT_INVERSE) != 0;
            منطقي dft_scale = (flags & DFT_SCALE) != 0;
            منطقي dft_صفون = (flags & DFT_صفون) != 0;

            clfftLayout اندريون_Layout = CLFFT_REAL, ٻاهريون_Layout = CLFFT_REAL;
            clfftDim dim = dft_سائيز.اوچائي == 1 || dft_صفون ? CLFFT_1D : CLFFT_2D;

            size_t batch_سائيز = dft_صفون ? dft_سائيز.اوچائي : 1;
            size_t cl_Lengths_اندريون[3] = { (size_t)dft_سائيز.ويڪر, dft_صفون ? 1 : (size_t)dft_سائيز.اوچائي, 1 };
            size_t cl_Strides_اندريون[3] = { 1, 1, 1 };
            size_t cl_Strides_ٻاهريون[3]  = { 1, 1, 1 };
            صحيح elem_سائيز = doubleFP ? حجم(مزدوج) : حجم(عائم);

            بدل (fftType)
            {
            حالة C2C:
                اندريون_Layout = CLFFT_COMPLEX_INTERLEAVED;
                ٻاهريون_Layout = CLFFT_COMPLEX_INTERLEAVED;
                cl_Strides_اندريون[1] = src_قدم / (elem_سائيز << 1);
                cl_Strides_ٻاهريون[1] = dst_قدم / (elem_سائيز << 1);
                اكسر;
            حالة R2C:
                اندريون_Layout = CLFFT_REAL;
                ٻاهريون_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                cl_Strides_اندريون[1] = src_قدم / elem_سائيز;
                cl_Strides_ٻاهريون[1] = dst_قدم / (elem_سائيز << 1);
                اكسر;
            حالة C2R:
                اندريون_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                ٻاهريون_Layout = CLFFT_REAL;
                cl_Strides_اندريون[1] = src_قدم / (elem_سائيز << 1);
                cl_Strides_ٻاهريون[1] = dst_قدم / elem_سائيز;
                اكسر;
            حالة R2R:
            افتراضي:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                اكسر;
            }

            cl_Strides_اندريون[2] = dft_صفون ? cl_Strides_اندريون[1] : dft_سائيز.ويڪر * cl_Strides_اندريون[1];
            cl_Strides_ٻاهريون[2] = dft_صفون ? cl_Strides_ٻاهريون[1] : dft_سائيز.ويڪر * cl_Strides_ٻاهريون[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, cl_Lengths_اندريون))

            // setting plan properties
            CLAMDDFT_Assert(clfft_سيٽ_Plan_Precision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfft_سيٽ_نتيجو_Location(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfft_سيٽ_Layout(plHandle, اندريون_Layout, ٻاهريون_Layout))
            CLAMDDFT_Assert(clfft_سيٽ_Plan_Batch_سائيز(plHandle, batch_سائيز))
            CLAMDDFT_Assert(clfft_سيٽ_Plan_اندريون_Stride(plHandle, dim, cl_Strides_اندريون))
            CLAMDDFT_Assert(clfft_سيٽ_Plan_ٻاهريون_Stride(plHandle, dim, cl_Strides_ٻاهريون))
            CLAMDDFT_Assert(clfft_سيٽ_Plan_مفاصلو(plHandle, cl_Strides_اندريون[dim], cl_Strides_ٻاهريون[dim]))

            عائم scale = dft_scale ? 1.0f / (dft_صفون ? dft_سائيز.ويڪر : dft_سائيز.area()) : 1.0f;
            CLAMDDFT_Assert(clfft_سيٽ_Plan_Scale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_قطار قطار_2 = (cl_command_قطار)ocl::قطار::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &قطار_2, عدم, عدم))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        دوست صنف PlanCache;

    خاص:
        سائيز dft_سائيز;
        صحيح src_قدم, dst_قدم;
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

    clfftPlanHandle getPlanHandle(ثابت سائيز & dft_سائيز, صحيح src_قدم, صحيح dst_قدم, منطقي doubleFP,
                                  منطقي inplace, صحيح flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        لكل (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            ثابت FftPlan * ثابت plan = planStorage[i];

            إذا (plan->dft_سائيز == dft_سائيز &&
                plan->flags == flags &&
                plan->src_قدم == src_قدم &&
                plan->dst_قدم == dst_قدم &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                إذا (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    اكسر;
                }

                أرجع plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(جديد FftPlan(dft_سائيز, src_قدم, dst_قدم, doubleFP, inplace, flags, fftType));
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
    UMat_معلومات * u = (UMat_معلومات *)p;

    إذا( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

ساكن منطقي ocl_dft_amdfft(داخلا_Array _src, OutputArray _dst, صحيح flags)
{
    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type);
    سائيز ssize = _src.size();

    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    إذا ( (!doubleSupport && اونهائي == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        أرجع خطأ;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    إذا (ssize.area() != getOptimalDFTSize(ssize.area()))
        أرجع خطأ;

    صحيح dst_complex_داخلا = cn == 2 ? 1 : 0;
    منطقي dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    صحيح dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    منطقي dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_داخلا << 0 | dft_complex_output << 1);

    بدل (fftType)
    {
    حالة C2C:
        _dst.create(ssize.اوچائي, ssize.ويڪر, CV_MAKE_TYPE(اونهائي, 2));
        اكسر;
    حالة R2C: // TODO implement it if possible
    حالة C2R: // TODO implement it if possible
    حالة R2R: // AMD Fft does not support this type
    افتراضي:
        أرجع خطأ;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    إذا (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        أرجع خطأ;
    منطقي inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (صحيح)src.قدم, (صحيح)dst.قدم,
                          اونهائي == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t بفر_سائيز = 0;
    CLAMDDFT_Assert(clfft_Get_Tmp_Buf_سائيز(plHandle, &بفر_سائيز))
    UMat tmp_بفر(1, (صحيح)بفر_سائيز, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_قطار قطار_2 = (cl_command_قطار)ocl::قطار::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfft_قطار_اندر_وجهو_Transform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &قطار_2, 0, عدم, &e,
                                          &srcarg, &dstarg, (cl_mem)tmp_بفر.handle(ACCESS_RW)))

    tmp_بفر.addref();
    cl_سيٽ_Event_Callback(e, CL_COMPLETE, oclCleanupCallback, tmp_بفر.u);
    أرجع سچ;
}

#وصف_رد DFT_ASSERT

}

#جيڪڏهن_پورو // HAVE_CLAMDFFT

نطاق cv
{

سانچو <قسم_نالو T>
ساكن فراغ complementComplex(T * ptr, size_t قدم, صحيح n, صحيح len, صحيح dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = قدم/حجم(p0[0]);
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

ساكن فراغ complementComplexOutput(صحيح اونهائي, uchar * ptr, size_t قدم, صحيح ڳڻپ_2, صحيح len, صحيح dft_dims)
{
    إذا( اونهائي == CV_32F )
        complementComplex((عائم*)ptr, قدم, ڳڻپ_2, len, dft_dims);
    وإلا
        complementComplex((مزدوج*)ptr, قدم, ڳڻپ_2, len, dft_dims);
}

تعداد Dft_گهڻائي {
    InvalidDft = 0,
    Fwd_Real_منزل_CCS,
    Fwd_Real_منزل_Complex,
    Fwd_Complex_منزل_Complex,
    InvCCSToReal,
    Inv_Complex_منزل_Real,
    Inv_Complex_منزل_Complex,
};

تعداد DftDims {
    InvalidDim = 0,
    OneDim,
    One_Dim_کانو_Wise,
    TwoDims
};

مضمن ثابت حرف * گهڻائي_Name(Dft_گهڻائي m)
{
    بدل (m)
    {
    حالة InvalidDft: أرجع "InvalidDft";
    حالة Fwd_Real_منزل_CCS: أرجع "FwdRealToCCS";
    حالة Fwd_Real_منزل_Complex: أرجع "FwdRealToComplex";
    حالة Fwd_Complex_منزل_Complex: أرجع "FwdComplexToComplex";
    حالة InvCCSToReal: أرجع "InvCCSToReal";
    حالة Inv_Complex_منزل_Real: أرجع "InvComplexToReal";
    حالة Inv_Complex_منزل_Complex: أرجع "InvComplexToComplex";
    }
    أرجع 0;
}

مضمن ثابت حرف * dimsName(DftDims d)
{
    بدل (d)
    {
    حالة InvalidDim: أرجع "InvalidDim";
    حالة OneDim: أرجع "OneDim";
    حالة One_Dim_کانو_Wise: أرجع "OneDimColWise";
    حالة TwoDims: أرجع "TwoDims";
    };
    أرجع 0;
}

سانچو <قسم_نالو T>
مضمن منطقي isInv(T گهڻائي)
{
    بدل ((Dft_گهڻائي)گهڻائي)
    {
        حالة InvCCSToReal:
        حالة Inv_Complex_منزل_Real:
        حالة Inv_Complex_منزل_Complex: أرجع سچ;
        افتراضي: أرجع خطأ;
    }
}

مضمن Dft_گهڻائي determine_گهڻائي(منطقي inv, صحيح cn1, صحيح cn2)
{
    إذا (!inv)
    {
        إذا (cn1 == 1 && cn2 == 1)
            أرجع Fwd_Real_منزل_CCS;
        وإلا إذا (cn1 == 1 && cn2 == 2)
            أرجع Fwd_Real_منزل_Complex;
        وإلا إذا (cn1 == 2 && cn2 == 2)
            أرجع Fwd_Complex_منزل_Complex;
    }
    وإلا
    {
        إذا (cn1 == 1 && cn2 == 1)
            أرجع InvCCSToReal;
        وإلا إذا (cn1 == 2 && cn2 == 1)
            أرجع Inv_Complex_منزل_Real;
        وإلا إذا (cn1 == 2 && cn2 == 2)
            أرجع Inv_Complex_منزل_Complex;
    }
    أرجع InvalidDft;
}


مضمن DftDims determineDims(صحيح صفون, صحيح کانن_ڳڻپ, منطقي is_صف_Wise, منطقي isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    إذا (is_صف_Wise)
        أرجع OneDim;
    إذا (کانن_ڳڻپ == 1 && صفون > 1) // one-column-shaped input
    {
        إذا (isContinuous)
            أرجع OneDim;
        وإلا
            أرجع One_Dim_کانو_Wise;
    }
    إذا (صفون == 1)
        أرجع OneDim;
    إذا (کانن_ڳڻپ > 1 && صفون > 1)
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
    Auto_بفر<uchar> ws;
#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
    Auto_بفر<uchar> ippbuf;
    Auto_بفر<uchar> ippworkbuf;
    size_t ipp_worksize;
#جيڪڏهن_پورو

عام:
    OcvDftBasicImpl()
    {
#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
        ipp_worksize = 0;
#جيڪڏهن_پورو
    }
    فراغ init(صحيح len, صحيح ڳڻپ_2, صحيح اونهائي, صحيح flags, منطقي *need_بفر)
    {
        صحيح stage = (flags & CV_HAL_DFT_STAGE_کانن_ڳڻپ) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        منطقي real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        منطقي needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = خطأ;
    #جيڪڏهن_وصف_ٿيل USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        إذا( CV_IPP_CHECK_COND && (opt.n*ڳڻپ_2 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            صحيح ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            صحيح specsize=0, initsize=0, worksize=0;
            Ipp_DFTGet_سائيز_Func get_سائيز_Func = 0;
            IppDFTInitFunc initFunc = 0;

            إذا( real_transform && stage == 0 )
            {
                إذا( اونهائي == CV_32F )
                {
                    get_سائيز_Func = ipps_DFTGet_سائيز_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                وإلا
                {
                    get_سائيز_Func = ipps_DFTGet_سائيز_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            وإلا
            {
                إذا( اونهائي == CV_32F )
                {
                    get_سائيز_Func = ipps_DFTGet_سائيز_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                وإلا
                {
                    get_سائيز_Func = ipps_DFTGet_سائيز_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            إذا( get_سائيز_Func(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                إذا( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = سچ;
            }
            وإلا
                سيٽ_Ipp_Error_حالت();
        }
    #جيڪڏهن_پورو

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            ثابت DFTKernels& kernels = getDFTKernels(اونهائي);
            صحيح قسم = stage == 0 && real_transform ? (opt.isInverse ? DFT_قسم_C2_R : DFT_قسم_R2_C)
                                                    : DFT_قسم_C2_C;
            plan.build(قسم, len, اونهائي, kernels.vlanes);
            ws.allocate(plan.ws_بائيٽون);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.معلومات();
            إذا (need_بفر)
                *need_بفر = خطأ;   // the engine supports in-place for every kind
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
            إذا (stage == 0)
            {
                إذا (real_transform)
                {
                    إذا (!opt.isInverse)
                        idx = 1;
                    وإلا
                        idx = 2;
                }
            }
            إذا (اونهائي == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        إذا(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            صحيح صف_ڳڻپ = ڳڻپ_2;
            إذا (stage == 0 && (flags & CV_HAL_DFT_صفون) != 0)
                صف_ڳڻپ = 1;
            opt.scale = 1./(len * صف_ڳڻپ);
        }
    }

    فراغ apply(ثابت uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspace_سائيز() ثابت
    {
        size_t sz = opt.plan ? plan.ws_بائيٽون : 0;
#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
        إذا (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#جيڪڏهن_پورو
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
#جيڪڏهن_وصف_ٿيل USE_IPP_DFT
        إذا (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#جيڪڏهن_پورو
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
ساكن ثابت صحيح DFT_کانن_ڳڻپ_BATCH_MIN = 4;
ساكن ثابت صحيح DFT_کانن_ڳڻپ_BATCH_MAX = 16;
ساكن ثابت size_t DFT_کانن_ڳڻپ_BATCH_بائيٽون = 1 << 17;

ساكن صحيح dft_کانن_ڳڻپ_Batch(صحيح len, size_t complex_elem_سائيز)
{
    صحيح batch = (صحيح)(DFT_کانن_ڳڻپ_BATCH_بائيٽون/((size_t)len*complex_elem_سائيز));
    أرجع std::max(DFT_کانن_ڳڻپ_BATCH_MIN, std::min(DFT_کانن_ڳڻپ_BATCH_MAX, batch));
}

ساكن مزدوج dftParallelStripes(صحيح ڳڻپ_2, صحيح len)
{
    ثابت مزدوج وقتي_حصو = 0.5e6;
    أرجع (مزدوج)ڳڻپ_2*len*std::max(std::لاگرٿم((مزدوج)len)/std::لاگرٿم(2.), 1.)/وقتي_حصو;
}

صنف OcvDftImpl CV_FINAL : عام hal::DFT2D
{
محمي:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    ثابت OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    ثابت OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    منطقي need_بفر_A;
    منطقي need_بفر_B;
    منطقي inv;
    صحيح ويڪر;
    صحيح اوچائي;
    Dft_گهڻائي گهڻائي;
    صحيح elem_سائيز_2;
    صحيح complex_elem_سائيز;
    صحيح اونهائي;
    منطقي real_transform;
    صحيح nonzero_صفون;
    منطقي is_صف_Transform;
    منطقي isScaled;
    std::vector<صحيح> stages;
    منطقي useIpp;
    صحيح src_channels;
    صحيح dst_channels;

    Auto_بفر<uchar> tmp_bufA;
    Auto_بفر<uchar> tmp_bufB;
    Auto_بفر<uchar> buf0;
    Auto_بفر<uchar> buf1;

عام:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        need_بفر_A = خطأ;
        need_بفر_B = خطأ;
        inv = خطأ;
        ويڪر = 0;
        اوچائي = 0;
        گهڻائي = InvalidDft;
        elem_سائيز_2 = 0;
        complex_elem_سائيز = 0;
        اونهائي = 0;
        real_transform = خطأ;
        nonzero_صفون = 0;
        is_صف_Transform = خطأ;
        isScaled = خطأ;
        useIpp = خطأ;
        src_channels = 0;
        dst_channels = 0;
    }

    فراغ init(صحيح ويڪر_3, صحيح اوچائي_3, صحيح اونهائي_2, صحيح _src_channels, صحيح _dst_channels, صحيح flags, صحيح nonzero_صفون_2)
    {
        منطقي isComplex = _src_channels != _dst_channels;
        nonzero_صفون = nonzero_صفون_2;
        ويڪر = ويڪر_3;
        اوچائي = اوچائي_3;
        اونهائي = اونهائي_2;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        منطقي isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        منطقي isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        منطقي isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        گهڻائي = determine_گهڻائي(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        is_صف_Transform = (flags & CV_HAL_DFT_صفون) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        need_بفر_A = خطأ;
        need_بفر_B = خطأ;
        real_transform = (گهڻائي != Fwd_Complex_منزل_Complex && گهڻائي != Inv_Complex_منزل_Complex);

        elem_سائيز_2 = (اونهائي == CV_32F) ? حجم(عائم) : حجم(مزدوج);
        complex_elem_سائيز = elem_سائيز_2 * 2;
        إذا( !real_transform )
            elem_سائيز_2 = complex_elem_سائيز;

#إذا defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            إذا (nonzero_صفون == 0 && اونهائي == CV_32F && ((ويڪر * اوچائي)>(صحيح)(1<<6)))
            {
                إذا (گهڻائي == Fwd_Complex_منزل_Complex || گهڻائي == Inv_Complex_منزل_Complex || گهڻائي == Fwd_Real_منزل_CCS || گهڻائي == InvCCSToReal)
                {
                    useIpp = سچ;
                    أرجع;
                }
            }
        }
#جيڪڏهن_پورو

        DftDims dims = determineDims(اوچائي, ويڪر, is_صف_Transform, isContinuous);
        إذا (dims == TwoDims)
        {
            stages.resize(2);
            إذا (گهڻائي == InvCCSToReal || گهڻائي == Inv_Complex_منزل_Real)
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
            إذا (dims == One_Dim_کانو_Wise)
                stages[0] = 1;
            وإلا
                stages[0] = 0;
        }

        لكل(uint stage_اشاريو = 0; stage_اشاريو < stages.size(); ++stage_اشاريو)
        {
            إذا (stage_اشاريو == 1)
            {
                isInplace = سچ;
                isComplex = خطأ;
            }

            صحيح stage = stages[stage_اشاريو];
            منطقي is_آخري_Stage = (stage_اشاريو + 1 == stages.size());

            صحيح len, ڳڻپ_2;

            صحيح f = 0;
            إذا (inv)
                f |= CV_HAL_DFT_INVERSE;
            إذا (isScaled)
                f |= CV_HAL_DFT_SCALE;
            إذا (is_صف_Transform)
                f |= CV_HAL_DFT_صفون;
            إذا (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            إذا (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            إذا (!is_آخري_Stage)
                f |= CV_HAL_DFT_TWO_STAGE;

            إذا( stage == 0 ) // row-wise transform
            {
                إذا (ويڪر == 1 && !is_صف_Transform )
                {
                    len = اوچائي;
                    ڳڻپ_2 = ويڪر;
                }
                وإلا
                {
                    len = ويڪر;
                    ڳڻپ_2 = اوچائي;
                }
                need_بفر_A = isInplace;
                contextA = hal::DFT1D::create(len, ڳڻپ_2, اونهائي, f, &need_بفر_A);
                basicA = dynamic_cast<ثابت OcvDftBasicImpl*>(contextA.get());
                إذا (need_بفر_A)
                    tmp_bufA.allocate(len * complex_elem_سائيز);
            }
            وإلا
            {
                len = اوچائي;
                ڳڻپ_2 = ويڪر;
                f |= CV_HAL_DFT_STAGE_کانن_ڳڻپ;
                need_بفر_B = isInplace;
                contextB = hal::DFT1D::create(len, ڳڻپ_2, اونهائي, f, &need_بفر_B);
                basicB = dynamic_cast<ثابت OcvDftBasicImpl*>(contextB.get());
                إذا (need_بفر_B)
                    tmp_bufB.allocate(len * complex_elem_سائيز);

                buf0.allocate(len * complex_elem_سائيز);
                buf1.allocate(len * complex_elem_سائيز);
            }
        }
    }

    فراغ apply(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم) CV_OVERRIDE
    {
#إذا defined USE_IPP_DFT
        إذا (useIpp)
        {
            صحيح ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            إذا (!is_صف_Transform)
            {
                إذا (گهڻائي == Fwd_Complex_منزل_Complex || گهڻائي == Inv_Complex_منزل_Complex)
                {
                    إذا (ippi_DFT_C_32F(src, src_قدم, dst, dst_قدم, ويڪر, اوچائي, inv, ipp_norm_flag))
                    {
                        CV_IMPL_جوڙيو(CV_IMPL_IPP);
                        أرجع;
                    }
                    سيٽ_Ipp_Error_حالت();
                }
                وإلا إذا (گهڻائي == Fwd_Real_منزل_CCS || گهڻائي == InvCCSToReal)
                {
                    إذا (ippi_DFT_R_32F(src, src_قدم, dst, dst_قدم, ويڪر, اوچائي, inv, ipp_norm_flag))
                    {
                        CV_IMPL_جوڙيو(CV_IMPL_IPP);
                        أرجع;
                    }
                    سيٽ_Ipp_Error_حالت();
                }
            }
            وإلا
            {
                إذا (گهڻائي == Fwd_Complex_منزل_Complex || گهڻائي == Inv_Complex_منزل_Complex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    إذا (Dft_C_IPPLoop(src, src_قدم, dst, dst_قدم, ويڪر, اوچائي, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_جوڙيو(CV_IMPL_IPP|CV_IMPL_MT);
                        أرجع;
                    }
                    سيٽ_Ipp_Error_حالت();
                }
                وإلا إذا (گهڻائي == Fwd_Real_منزل_CCS || گهڻائي == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippi_DFTInv_Pack_منزل_R_32f_C1_R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    إذا (Dft_R_IPPLoop(src, src_قدم, dst, dst_قدم, ويڪر, اوچائي, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_جوڙيو(CV_IMPL_IPP|CV_IMPL_MT);
                        أرجع;
                    }
                    سيٽ_Ipp_Error_حالت();
                }
            }
            أرجع;
        }
#جيڪڏهن_پورو

        لكل(uint stage_اشاريو = 0; stage_اشاريو < stages.size(); ++stage_اشاريو)
        {
            صحيح stage_src_channels = src_channels;
            صحيح stage_dst_channels = dst_channels;

            إذا (stage_اشاريو == 1)
            {
                src = dst;
                src_قدم = dst_قدم;
                stage_src_channels = stage_dst_channels;
            }

            صحيح stage = stages[stage_اشاريو];
            منطقي is_آخري_Stage = (stage_اشاريو + 1 == stages.size());
            منطقي isComplex = stage_src_channels != stage_dst_channels;

            إذا( stage == 0 )
                صف_Dft(src, src_قدم, dst, dst_قدم, isComplex, is_آخري_Stage);
            وإلا
                کانو_Dft(src, src_قدم, dst, dst_قدم, stage_src_channels, stage_dst_channels, is_آخري_Stage);
        }
    }

محمي:

    فراغ صف_Dft(ثابت uchar* src_معلومات, size_t src_قدم, uchar* dst_معلومات, size_t dst_قدم, منطقي isComplex, منطقي is_آخري_Stage)
    {
        صحيح len, ڳڻپ_2;
        إذا (ويڪر == 1 && !is_صف_Transform )
        {
            len = اوچائي;
            ڳڻپ_2 = ويڪر;
        }
        وإلا
        {
            len = ويڪر;
            ڳڻپ_2 = اوچائي;
        }
        صحيح dptr_وٿي = 0;
        صحيح dst_ڀريل_len = len*elem_سائيز_2;

        إذا( need_بفر_A )
        {
            إذا (گهڻائي == Fwd_Real_منزل_CCS && (len & 1) && len > 1)
                dptr_وٿي = elem_سائيز_2;
        }

        إذا( !inv && isComplex )
            dst_ڀريل_len += (len & 1) ? elem_سائيز_2 : complex_elem_سائيز;

        صحيح nz = nonzero_صفون;
        إذا( nz <= 0 || nz > ڳڻپ_2 )
            nz = ڳڻپ_2;

        size_t ws_سائيز = basicA ? basicA->workspace_سائيز() : 0;
        مزدوج nstripes = dftParallelStripes(nz, len);
        إذا( ws_سائيز > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            ثابت OcvDftBasicImpl* ctx = basicA;
            خودڪار process_صفون = [&, ctx](ثابت Range& range)
            {
                Auto_بفر<uchar> ws(ws_سائيز);
                Auto_بفر<uchar> rowbuf(need_بفر_A ? len * complex_elem_سائيز : 0);
                لكل( صحيح i = range.شروعات; i < range.end; i++ )
                {
                    ثابت uchar* sptr = src_معلومات + src_قدم * i;
                    uchar* dptr0 = dst_معلومات + dst_قدم * i;
                    uchar* dptr = need_بفر_A ? rowbuf.معلومات() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.معلومات());
                    إذا( need_بفر_A )
                        يادگيري_نقل( dptr0, dptr + dptr_وٿي, dst_ڀريل_len );
                }
            };
            parallel_for_(Range(0, nz), process_صفون, nstripes);
        }
        وإلا
        {
            لكل( صحيح i = 0; i < nz; i++ )
            {
                ثابت uchar* sptr = src_معلومات + src_قدم * i;
                uchar* dptr0 = dst_معلومات + dst_قدم * i;
                uchar* dptr = dptr0;

                إذا( need_بفر_A )
                    dptr = tmp_bufA.معلومات();

                contextA->apply(sptr, dptr);

                إذا( need_بفر_A )
                    يادگيري_نقل( dptr0, dptr + dptr_وٿي, dst_ڀريل_len );
            }
        }

        لكل( صحيح i = nz; i < ڳڻپ_2; i++ )
        {
            uchar* dptr0 = dst_معلومات + dst_قدم * i;
            memset( dptr0, 0, dst_ڀريل_len );
        }
        إذا(is_آخري_Stage &&  گهڻائي == Fwd_Real_منزل_Complex)
            complementComplexOutput(اونهائي, dst_معلومات, dst_قدم, len, nz, 1);
    }

    فراغ کانو_Dft(ثابت uchar* src_معلومات, size_t src_قدم, uchar* dst_معلومات, size_t dst_قدم, صحيح stage_src_channels, صحيح stage_dst_channels, منطقي is_آخري_Stage)
    {
        صحيح len = اوچائي;
        صحيح ڳڻپ_2 = ويڪر;
        صحيح a = 0, b = ڳڻپ_2;
        uchar *dbuf0, *dbuf1;
        ثابت uchar* sptr0 = src_معلومات;
        uchar* dptr0 = dst_معلومات;

        dbuf0 = buf0.معلومات(), dbuf1 = buf1.معلومات();

        إذا( need_بفر_B )
        {
            dbuf1 = tmp_bufB.معلومات();
            dbuf0 = buf1.معلومات();
        }

        إذا( real_transform )
        {
            صحيح even;
            a = 1;
            even = (ڳڻپ_2 & 1) == 0;
            b = (ڳڻپ_2+1)/2;
            إذا( !inv )
            {
                memset( buf0.معلومات(), 0, len*complex_elem_سائيز );
                Copy_ڪالم( sptr0, src_قدم, buf0.معلومات(), complex_elem_سائيز, len, elem_سائيز_2 );
                sptr0 += stage_dst_channels*elem_سائيز_2;
                إذا( even )
                {
                    memset( buf1.معلومات(), 0, len*complex_elem_سائيز );
                    Copy_ڪالم( sptr0 + (ڳڻپ_2-2)*elem_سائيز_2, src_قدم,
                                buf1.معلومات(), complex_elem_سائيز, len, elem_سائيز_2 );
                }
            }
            وإلا إذا( stage_src_channels == 1 )
            {
                Copy_ڪالم( sptr0, src_قدم, buf0.معلومات(), elem_سائيز_2, len, elem_سائيز_2 );
                ExpandCCS( buf0.معلومات(), len, elem_سائيز_2 );
                إذا( even )
                {
                    Copy_ڪالم( sptr0 + (ڳڻپ_2-1)*elem_سائيز_2, src_قدم,
                                buf1.معلومات(), elem_سائيز_2, len, elem_سائيز_2 );
                    ExpandCCS( buf1.معلومات(), len, elem_سائيز_2 );
                }
                sptr0 += elem_سائيز_2;
            }
            وإلا
            {
                Copy_ڪالم( sptr0, src_قدم, buf0.معلومات(), complex_elem_سائيز, len, complex_elem_سائيز );
                إذا( even )
                {
                    Copy_ڪالم( sptr0 + b*complex_elem_سائيز, src_قدم,
                                   buf1.معلومات(), complex_elem_سائيز, len, complex_elem_سائيز );
                }
                sptr0 += complex_elem_سائيز;
            }

            إذا( even )
                contextB->apply(buf1.معلومات(), dbuf1);
            contextB->apply(buf0.معلومات(), dbuf0);

            إذا( stage_dst_channels == 1 )
            {
                إذا( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    يادگيري_نقل( dbuf0 + elem_سائيز_2, dbuf0, elem_سائيز_2 );
                    Copy_ڪالم( dbuf0 + elem_سائيز_2, elem_سائيز_2, dptr0,
                                   dst_قدم, len, elem_سائيز_2 );
                    إذا( even )
                    {
                        يادگيري_نقل( dbuf1 + elem_سائيز_2, dbuf1, elem_سائيز_2 );
                        Copy_ڪالم( dbuf1 + elem_سائيز_2, elem_سائيز_2,
                                       dptr0 + (ڳڻپ_2-1)*elem_سائيز_2,
                                       dst_قدم, len, elem_سائيز_2 );
                    }
                    dptr0 += elem_سائيز_2;
                }
                وإلا
                {
                    // copy the real part of the complex vector to the first/last column
                    Copy_ڪالم( dbuf0, complex_elem_سائيز, dptr0, dst_قدم, len, elem_سائيز_2 );
                    إذا( even )
                        Copy_ڪالم( dbuf1, complex_elem_سائيز, dptr0 + (ڳڻپ_2-1)*elem_سائيز_2,
                                       dst_قدم, len, elem_سائيز_2 );
                    dptr0 += elem_سائيز_2;
                }
            }
            وإلا
            {
                CV_Assert( !inv );
                Copy_ڪالم( dbuf0, complex_elem_سائيز, dptr0,
                               dst_قدم, len, complex_elem_سائيز );
                إذا( even )
                    Copy_ڪالم( dbuf1, complex_elem_سائيز,
                                   dptr0 + b*complex_elem_سائيز,
                                   dst_قدم, len, complex_elem_سائيز );
                dptr0 += complex_elem_سائيز;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        صحيح ncols = b - a;
        ثابت صحيح کانن_ڳڻپ_batch = dft_کانن_ڳڻپ_Batch(len, complex_elem_سائيز);
        صحيح nbatches = (ncols + کانن_ڳڻپ_batch - 1)/کانن_ڳڻپ_batch;
        size_t ws_سائيز = basicB ? basicB->workspace_سائيز() : 0;
        size_t colbuf_سائيز = (size_t)len*complex_elem_سائيز;
        ثابت OcvDftBasicImpl* ctx = basicB;
        خودڪار processBatches = [&, ctx](ثابت Range& range)
        {
            Auto_بفر<uchar> ws(ws_سائيز);
            Auto_بفر<uchar> cbuf(colbuf_سائيز*کانن_ڳڻپ_batch*(need_بفر_B ? 2 : 1));
            uchar* cbuf0 = cbuf.معلومات();
            uchar* cdbuf0 = need_بفر_B ? cbuf0 + colbuf_سائيز*کانن_ڳڻپ_batch : cbuf0;
            لكل( صحيح bi = range.شروعات; bi < range.end; bi++ )
            {
                صحيح c0 = bi*کانن_ڳڻپ_batch;
                صحيح nc = std::min(کانن_ڳڻپ_batch, ncols - c0);
                ثابت uchar* sptr = sptr0 + (size_t)c0*complex_elem_سائيز;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_سائيز;
                CopyFromColumns( sptr, src_قدم, cbuf0, nc, len, complex_elem_سائيز );
                لكل( صحيح k = 0; k < nc; k++ )
                {
                    إذا( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_سائيز, cdbuf0 + k*colbuf_سائيز, ws.معلومات());
                    وإلا
                        contextB->apply(cbuf0 + k*colbuf_سائيز, cdbuf0 + k*colbuf_سائيز);
                }
                Copy_منزل_Columns( cdbuf0, dptr, dst_قدم, nc, len, complex_elem_سائيز );
            }
        };
        مزدوج nstripes = dftParallelStripes(ncols, len);
        إذا( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        وإلا
            processBatches(Range(0, nbatches));
        إذا(is_آخري_Stage && گهڻائي == Fwd_Real_منزل_Complex)
            complementComplexOutput(اونهائي, dst_معلومات, dst_قدم, ڳڻپ_2, len, 2);
    }
};

بنية ReplacementDFT1D : عام hal::DFT1D
{
    cvhalDFT *context;
    منطقي isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(خطأ) {}
    منطقي init(صحيح len, صحيح ڳڻپ_2, صحيح اونهائي, صحيح flags, منطقي *need_بفر)
    {
        صحيح res = cv_hal_dftInit1D(&context, len, ڳڻپ_2, اونهائي, flags, need_بفر);
        isInitialized = (res == CV_HAL_ERROR_OK);
        أرجع isInitialized;
    }
    فراغ apply(ثابت uchar *src, uchar *dst) CV_OVERRIDE
    {
        إذا (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        إذا (isInitialized)
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
    منطقي init(صحيح ويڪر, صحيح اوچائي, صحيح اونهائي,
              صحيح src_channels, صحيح dst_channels,
              صحيح flags, صحيح nonzero_صفون)
    {
        صحيح res = cv_hal_dftInit2D(&context, ويڪر, اوچائي, اونهائي, src_channels, dst_channels, flags, nonzero_صفون);
        isInitialized = (res == CV_HAL_ERROR_OK);
        أرجع isInitialized;
    }
    فراغ apply(ثابت uchar *src, size_t src_قدم, uchar *dst, size_t dst_قدم) CV_OVERRIDE
    {
        إذا (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_قدم, dst, dst_قدم);
        }
    }
    ~ReplacementDFT2D()
    {
        إذا (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

نطاق hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(صحيح len, صحيح ڳڻپ_2, صحيح اونهائي, صحيح flags, منطقي *need_بفر)
{
    {
        ReplacementDFT1D *impl = جديد ReplacementDFT1D();
        إذا (impl->init(len, ڳڻپ_2, اونهائي, flags, need_بفر))
        {
            أرجع Ptr<DFT1D>(impl);
        }
        ميٽيو impl;
    }
    {
        OcvDftBasicImpl *impl = جديد OcvDftBasicImpl();
        impl->init(len, ڳڻپ_2, اونهائي, flags, need_بفر);
        أرجع Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(صحيح ويڪر, صحيح اوچائي, صحيح اونهائي,
                         صحيح src_channels, صحيح dst_channels,
                         صحيح flags, صحيح nonzero_صفون)
{
    {
        ReplacementDFT2D *impl = جديد ReplacementDFT2D();
        إذا (impl->init(ويڪر, اوچائي, اونهائي, src_channels, dst_channels, flags, nonzero_صفون))
        {
            أرجع Ptr<DFT2D>(impl);
        }
        ميٽيو impl;
    }
    {
        إذا(ويڪر == 1 && nonzero_صفون > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = جديد OcvDftImpl();
        impl->init(ويڪر, اوچائي, اونهائي, src_channels, dst_channels, flags, nonzero_صفون);
        أرجع Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


فراغ cv::dft( داخلا_Array _src0, OutputArray _dst, صحيح flags, صحيح nonzero_صفون )
{
    CV_INSTRUMENT_REGION();

#جيڪڏهن_وصف_ٿيل HAVE_CLAMDFFT
    CV_OCL_لاڳيتو(ocl::haveAmdFft() && ocl::Device::getDefault().type() != ocl::Device::TYPE_CPU &&
            _dst.isUMat() && _src0.dims() <= 2 && nonzero_صفون == 0,
               ocl_dft_amdfft(_src0, _dst, flags))
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    CV_OCL_لاڳيتو(_dst.isUMat() && _src0.dims() <= 2,
               ocl_dft(_src0, _dst, flags, nonzero_صفون))
#جيڪڏهن_پورو

    Mat src0 = _src0.getMat(), src = src0;
    إذا (src.empty())
    {
        _dst.release();
        أرجع;
    }
    منطقي inv = (flags & DFT_INVERSE) != 0;
    صحيح type = src.type();
    صحيح اونهائي = src.اونهائي();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_داخلا) && src.channels() != 2) );

    إذا( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.create_Same_سائيز( src, CV_MAKETYPE(اونهائي, 2) );
    وإلا إذا( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.create_Same_سائيز( src, اونهائي );
    وإلا
        _dst.create_Same_سائيز( src, type );

    Mat dst = _dst.getMat();

    صحيح f = 0;
    إذا (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    إذا (inv)
        f |= CV_HAL_DFT_INVERSE;
    إذا (flags & DFT_صفون)
        f |= CV_HAL_DFT_صفون;
    إذا (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    إذا (src.معلومات == dst.معلومات)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.کانن_ڳڻپ, src.صفون, اونهائي, src.channels(), dst.channels(), f, nonzero_صفون);
    c->apply(src.معلومات, src.قدم, dst.معلومات, dst.قدم);
}


فراغ cv::idft( داخلا_Array src, OutputArray dst, صحيح flags, صحيح nonzero_صفون )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_صفون );
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_mulSpectrums( داخلا_Array _srcA, داخلا_Array _srcB,
                              OutputArray _dst, صحيح flags, منطقي conjB )
{
    صحيح atype = _srcA.type(), btype = _srcB.type(),
            صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    سائيز asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    إذا ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        أرجع خطأ;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.create_Same_سائيز(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    إذا (k.empty())
        أرجع خطأ;

    k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(A), ocl::KernelArg::Read_Only_No_سائيز(B),
           ocl::KernelArg::WriteOnly(dst), صفون_في_WI);

    size_t globalsize[2] = { (size_t)asize.ويڪر, ((size_t)asize.اوچائي + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

}

#جيڪڏهن_پورو

نطاق {

#وصف_ڪريو VAL(buf, elem) (((T*)((حرف*)معلومات ## buf + (قدم ## buf * (elem))))[0])
#وصف_ڪريو MUL_SPECTRUMS_کانو(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    لكل (size_t j = 1; j <= صفون - 2; j += 2) \
    { \
        مزدوج a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        مزدوج b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        إذا (conjB) b_im = -b_im; \
        مزدوج c_re = a_re * b_re - a_im * b_im; \
        مزدوج c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    إذا ((صفون & 1) == 0) \
        VAL(C, صفون-1) = VAL(A, صفون-1) * VAL(B, صفون-1)

سانچو <قسم_نالو T, منطقي conjB> ساكن مضمن
فراغ mul_Spectrums_process_کانو_noinplace(ثابت T* معلومات_A, ثابت T* معلومات_B, T* معلومات_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفون)
{
    MUL_SPECTRUMS_کانو(A, B, C);
}

سانچو <قسم_نالو T, منطقي conjB> ساكن مضمن
فراغ mul_Spectrums_process_کانو_inplace_A(ثابت T* معلومات_B, T* معلومات_AC, size_t قدم_B, size_t قدم_AC, size_t صفون)
{
    MUL_SPECTRUMS_کانو(AC, B, AC);
}
سانچو <قسم_نالو T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mul_Spectrums_process_کانو(ثابت T* معلومات_A, ثابت T* معلومات_B, T* معلومات_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفون)
{
    إذا (inplaceA)
        mul_Spectrums_process_کانو_inplace_A<T, conjB>(معلومات_B, معلومات_C, قدم_B, قدم_C, صفون);
    وإلا
        mul_Spectrums_process_کانو_noinplace<T, conjB>(معلومات_A, معلومات_B, معلومات_C, قدم_A, قدم_B, قدم_C, صفون);
}
#وصف_رد MUL_SPECTRUMS_کانو
#وصف_رد VAL

سانچو <قسم_نالو T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mul_Spectrums_process_کانن_ڳڻپ(ثابت T* معلومات_A, ثابت T* معلومات_B, T* معلومات_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفون, size_t کانن_ڳڻپ)
{
    mul_Spectrums_process_کانو<T, conjB, inplaceA>(معلومات_A, معلومات_B, معلومات_C, قدم_A, قدم_B, قدم_C, صفون);
    إذا ((کانن_ڳڻپ & 1) == 0)
    {
        mul_Spectrums_process_کانو<T, conjB, inplaceA>(معلومات_A + کانن_ڳڻپ - 1, معلومات_B + کانن_ڳڻپ - 1, معلومات_C + کانن_ڳڻپ - 1, قدم_A, قدم_B, قدم_C, صفون);
    }
}

#وصف_ڪريو VAL(buf, elem) (معلومات ## buf[(elem)])
#وصف_ڪريو MUL_SPECTRUMS_صف(A, B, C) \
    لكل (size_t j = j0; j < j1; j += 2) \
    { \
        مزدوج a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        مزدوج b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        إذا (conjB) b_im = -b_im; \
        مزدوج c_re = a_re * b_re - a_im * b_im; \
        مزدوج c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
سانچو <قسم_نالو T, منطقي conjB> ساكن مضمن
فراغ mul_Spectrums_process_صف_noinplace(ثابت T* معلومات_A, ثابت T* معلومات_B, T* معلومات_C, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_صف(A, B, C);
}
سانچو <قسم_نالو T, منطقي conjB> ساكن مضمن
فراغ mul_Spectrums_process_صف_inplace_A(ثابت T* معلومات_B, T* معلومات_AC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_صف(AC, B, AC);
}
سانچو <قسم_نالو T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mul_Spectrums_process_صف(ثابت T* معلومات_A, ثابت T* معلومات_B, T* معلومات_C, size_t j0, size_t j1)
{
    إذا (inplaceA)
        mul_Spectrums_process_صف_inplace_A<T, conjB>(معلومات_B, معلومات_C, j0, j1);
    وإلا
        mul_Spectrums_process_صف_noinplace<T, conjB>(معلومات_A, معلومات_B, معلومات_C, j0, j1);
}
#وصف_رد MUL_SPECTRUMS_صف
#وصف_رد VAL

سانچو <قسم_نالو T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mul_Spectrums_process_صفون(ثابت T* معلومات_A, ثابت T* معلومات_B, T* معلومات_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفون, size_t کانن_ڳڻپ, size_t j0, size_t j1, منطقي is_1d_CN1)
{
    طالما (صفون-- > 0)
    {
        إذا (is_1d_CN1)
            معلومات_C[0] = معلومات_A[0]*معلومات_B[0];
        mul_Spectrums_process_صف<T, conjB, inplaceA>(معلومات_A, معلومات_B, معلومات_C, j0, j1);
        إذا (is_1d_CN1 && (کانن_ڳڻپ & 1) == 0)
            معلومات_C[j1] = معلومات_A[j1]*معلومات_B[j1];

        معلومات_A = (ثابت T*)(((حرف*)معلومات_A) + قدم_A);
        معلومات_B = (ثابت T*)(((حرف*)معلومات_B) + قدم_B);
        معلومات_C =       (T*)(((حرف*)معلومات_C) + قدم_C);
    }
}


سانچو <قسم_نالو T, منطقي conjB, منطقي inplaceA> ساكن مضمن
فراغ mulSpectrums_Impl_(ثابت T* معلومات_A, ثابت T* معلومات_B, T* معلومات_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفون, size_t کانن_ڳڻپ, size_t j0, size_t j1, منطقي is_1d, منطقي isCN1)
{
    إذا (!is_1d && isCN1)
    {
        mul_Spectrums_process_کانن_ڳڻپ<T, conjB, inplaceA>(معلومات_A, معلومات_B, معلومات_C, قدم_A, قدم_B, قدم_C, صفون, کانن_ڳڻپ);
    }
    mul_Spectrums_process_صفون<T, conjB, inplaceA>(معلومات_A, معلومات_B, معلومات_C, قدم_A, قدم_B, قدم_C, صفون, کانن_ڳڻپ, j0, j1, is_1d && isCN1);
}
سانچو <قسم_نالو T, منطقي conjB> ساكن مضمن
فراغ mulSpectrums_Impl(ثابت T* معلومات_A, ثابت T* معلومات_B, T* معلومات_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفون, size_t کانن_ڳڻپ, size_t j0, size_t j1, منطقي is_1d, منطقي isCN1)
{
    إذا (معلومات_A == معلومات_C)
        mulSpectrums_Impl_<T, conjB, سچ>(معلومات_A, معلومات_B, معلومات_C, قدم_A, قدم_B, قدم_C, صفون, کانن_ڳڻپ, j0, j1, is_1d, isCN1);
    وإلا
        mulSpectrums_Impl_<T, conjB, خطأ>(معلومات_A, معلومات_B, معلومات_C, قدم_A, قدم_B, قدم_C, صفون, کانن_ڳڻپ, j0, j1, is_1d, isCN1);
}

} // namespace

فراغ cv::mulSpectrums( داخلا_Array _srcA, داخلا_Array _srcB,
                       OutputArray _dst, صحيح flags, منطقي conjB )
{
    CV_INSTRUMENT_REGION();

    إذا (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        أرجع;
    }

    صحيح type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_لاڳيتو(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    صحيح اونهائي = srcA.اونهائي(), cn = srcA.channels();
    size_t صفون = srcA.صفون, کانن_ڳڻپ = srcA.کانن_ڳڻپ;

    _dst.create( srcA.صفون, srcA.کانن_ڳڻپ, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    إذا (dst.معلومات == srcB.معلومات)
        srcB = srcB.clone(); // workaround for B only

    منطقي is_1d = (flags & DFT_صفون)
        || (صفون == 1)
        || (کانن_ڳڻپ == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    إذا( is_1d && !(flags & DFT_صفون) )
        کانن_ڳڻپ = کانن_ڳڻپ + صفون - 1, صفون = 1;

    منطقي isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = کانن_ڳڻپ*cn - (((کانن_ڳڻپ & 1) == 0 && cn == 1) ? 1 : 0);

    إذا (اونهائي == CV_32F)
    {
        ثابت عائم* معلومات_A = srcA.ptr<عائم>();
        ثابت عائم* معلومات_B = srcB.ptr<عائم>();
        عائم* معلومات_C = dst.ptr<عائم>();
        إذا (!conjB)
            mulSpectrums_Impl<عائم, خطأ>(معلومات_A, معلومات_B, معلومات_C, srcA.قدم, srcB.قدم, dst.قدم, صفون, کانن_ڳڻپ, j0, j1, is_1d, isCN1);
        وإلا
            mulSpectrums_Impl<عائم, سچ>(معلومات_A, معلومات_B, معلومات_C, srcA.قدم, srcB.قدم, dst.قدم, صفون, کانن_ڳڻپ, j0, j1, is_1d, isCN1);
    }
    وإلا
    {
        ثابت مزدوج* معلومات_A = srcA.ptr<مزدوج>();
        ثابت مزدوج* معلومات_B = srcB.ptr<مزدوج>();
        مزدوج* معلومات_C = dst.ptr<مزدوج>();
        إذا (!conjB)
            mulSpectrums_Impl<مزدوج, خطأ>(معلومات_A, معلومات_B, معلومات_C, srcA.قدم, srcB.قدم, dst.قدم, صفون, کانن_ڳڻپ, j0, j1, is_1d, isCN1);
        وإلا
            mulSpectrums_Impl<مزدوج, سچ>(معلومات_A, معلومات_B, معلومات_C, srcA.قدم, srcB.قدم, dst.قدم, صفون, کانن_ڳڻپ, j0, j1, is_1d, isCN1);
    }
}

فراغ cv::divSpectrums( داخلا_Array _srcA, داخلا_Array _srcB, OutputArray _dst, صحيح flags, منطقي conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    إذا (srcA.empty() && srcB.empty())
    {
        _dst.release();
        أرجع;
    }
    صحيح اونهائي = srcA.اونهائي(), cn = srcA.channels(), type = srcA.type();
    صحيح صفون = srcA.صفون, کانن_ڳڻپ = srcA.کانن_ڳڻپ;
    صحيح j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.صفون, srcA.کانن_ڳڻپ, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.معلومات != srcA.معلومات); // non-inplace check
    CV_Assert(dst.معلومات != srcB.معلومات); // non-inplace check

    منطقي is_1d = (flags & DFT_صفون) || (صفون == 1 || (کانن_ڳڻپ == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    إذا( is_1d && !(flags & DFT_صفون) )
        کانن_ڳڻپ = کانن_ڳڻپ + صفون - 1, صفون = 1;

    صحيح ncols = کانن_ڳڻپ*cn;
    صحيح j0 = cn == 1;
    صحيح j1 = ncols - (کانن_ڳڻپ % 2 == 0 && cn == 1);

    إذا( اونهائي == CV_32F )
    {
        ثابت عائم* معلومات_A = srcA.ptr<عائم>();
        ثابت عائم* معلومات_B = srcB.ptr<عائم>();
        عائم* معلومات_C = dst.ptr<عائم>();
        عائم eps = FLT_EPSILON; // prevent div0 problems

        size_t قدم_A = srcA.قدم/حجم(معلومات_A[0]);
        size_t قدم_B = srcB.قدم/حجم(معلومات_B[0]);
        size_t قدم_C = dst.قدم/حجم(معلومات_C[0]);

        إذا( !is_1d && cn == 1 )
        {
            لكل( k = 0; k < (کانن_ڳڻپ % 2 ? 1 : 2); k++ )
            {
                إذا( k == 1 )
                    معلومات_A += کانن_ڳڻپ - 1, معلومات_B += کانن_ڳڻپ - 1, معلومات_C += کانن_ڳڻپ - 1;
                معلومات_C[0] = معلومات_A[0] / (معلومات_B[0] + eps);
                إذا( صفون % 2 == 0 )
                    معلومات_C[(صفون-1)*قدم_C] = معلومات_A[(صفون-1)*قدم_A] / (معلومات_B[(صفون-1)*قدم_B] + eps);
                إذا( !conjB )
                    لكل( j = 1; j <= صفون - 2; j += 2 )
                    {
                        مزدوج denom = (مزدوج)معلومات_B[j*قدم_B]*معلومات_B[j*قدم_B] +
                        (مزدوج)معلومات_B[(j+1)*قدم_B]*معلومات_B[(j+1)*قدم_B] + (مزدوج)eps;

                        مزدوج re = (مزدوج)معلومات_A[j*قدم_A]*معلومات_B[j*قدم_B] +
                        (مزدوج)معلومات_A[(j+1)*قدم_A]*معلومات_B[(j+1)*قدم_B];

                        مزدوج im = (مزدوج)معلومات_A[(j+1)*قدم_A]*معلومات_B[j*قدم_B] -
                        (مزدوج)معلومات_A[j*قدم_A]*معلومات_B[(j+1)*قدم_B];

                        معلومات_C[j*قدم_C] = (عائم)(re / denom);
                        معلومات_C[(j+1)*قدم_C] = (عائم)(im / denom);
                    }
                    وإلا
                        لكل( j = 1; j <= صفون - 2; j += 2 )
                        {

                            مزدوج denom = (مزدوج)معلومات_B[j*قدم_B]*معلومات_B[j*قدم_B] +
                            (مزدوج)معلومات_B[(j+1)*قدم_B]*معلومات_B[(j+1)*قدم_B] + (مزدوج)eps;

                            مزدوج re = (مزدوج)معلومات_A[j*قدم_A]*معلومات_B[j*قدم_B] -
                            (مزدوج)معلومات_A[(j+1)*قدم_A]*معلومات_B[(j+1)*قدم_B];

                            مزدوج im = (مزدوج)معلومات_A[(j+1)*قدم_A]*معلومات_B[j*قدم_B] +
                            (مزدوج)معلومات_A[j*قدم_A]*معلومات_B[(j+1)*قدم_B];

                            معلومات_C[j*قدم_C] = (عائم)(re / denom);
                            معلومات_C[(j+1)*قدم_C] = (عائم)(im / denom);
                        }
                    إذا( k == 1 )
                        معلومات_A -= کانن_ڳڻپ - 1, معلومات_B -= کانن_ڳڻپ - 1, معلومات_C -= کانن_ڳڻپ - 1;
            }
        }

        لكل( ; صفون--; معلومات_A += قدم_A, معلومات_B += قدم_B, معلومات_C += قدم_C )
        {
            إذا( is_1d && cn == 1 )
            {
                معلومات_C[0] = معلومات_A[0] / (معلومات_B[0] + eps);
                إذا( کانن_ڳڻپ % 2 == 0 )
                    معلومات_C[j1] = معلومات_A[j1] / (معلومات_B[j1] + eps);
            }

            إذا( !conjB )
                لكل( j = j0; j < j1; j += 2 )
                {
                    مزدوج denom = (مزدوج)معلومات_B[j]*معلومات_B[j] + (مزدوج)معلومات_B[j+1]*معلومات_B[j+1] + (مزدوج)eps;
                    مزدوج re = (مزدوج)معلومات_A[j]*معلومات_B[j] + (مزدوج)معلومات_A[j+1]*معلومات_B[j+1];
                    مزدوج im = (مزدوج)معلومات_A[j+1]*معلومات_B[j] - (مزدوج)معلومات_A[j]*معلومات_B[j+1];
                    معلومات_C[j] = (عائم)(re / denom);
                    معلومات_C[j+1] = (عائم)(im / denom);
                }
                وإلا
                    لكل( j = j0; j < j1; j += 2 )
                    {
                        مزدوج denom = (مزدوج)معلومات_B[j]*معلومات_B[j] + (مزدوج)معلومات_B[j+1]*معلومات_B[j+1] + (مزدوج)eps;
                        مزدوج re = (مزدوج)معلومات_A[j]*معلومات_B[j] - (مزدوج)معلومات_A[j+1]*معلومات_B[j+1];
                        مزدوج im = (مزدوج)معلومات_A[j+1]*معلومات_B[j] + (مزدوج)معلومات_A[j]*معلومات_B[j+1];
                        معلومات_C[j] = (عائم)(re / denom);
                        معلومات_C[j+1] = (عائم)(im / denom);
                    }
        }
    }
    وإلا
    {
        ثابت مزدوج* معلومات_A = srcA.ptr<مزدوج>();
        ثابت مزدوج* معلومات_B = srcB.ptr<مزدوج>();
        مزدوج* معلومات_C = dst.ptr<مزدوج>();
        مزدوج eps = DBL_EPSILON; // prevent div0 problems

        size_t قدم_A = srcA.قدم/حجم(معلومات_A[0]);
        size_t قدم_B = srcB.قدم/حجم(معلومات_B[0]);
        size_t قدم_C = dst.قدم/حجم(معلومات_C[0]);

        إذا( !is_1d && cn == 1 )
        {
            لكل( k = 0; k < (کانن_ڳڻپ % 2 ? 1 : 2); k++ )
            {
                إذا( k == 1 )
                    معلومات_A += کانن_ڳڻپ - 1, معلومات_B += کانن_ڳڻپ - 1, معلومات_C += کانن_ڳڻپ - 1;
                معلومات_C[0] = معلومات_A[0] / (معلومات_B[0] + eps);
                إذا( صفون % 2 == 0 )
                    معلومات_C[(صفون-1)*قدم_C] = معلومات_A[(صفون-1)*قدم_A] / (معلومات_B[(صفون-1)*قدم_B] + eps);
                إذا( !conjB )
                    لكل( j = 1; j <= صفون - 2; j += 2 )
                    {
                        مزدوج denom = معلومات_B[j*قدم_B]*معلومات_B[j*قدم_B] +
                        معلومات_B[(j+1)*قدم_B]*معلومات_B[(j+1)*قدم_B] + eps;

                        مزدوج re = معلومات_A[j*قدم_A]*معلومات_B[j*قدم_B] +
                        معلومات_A[(j+1)*قدم_A]*معلومات_B[(j+1)*قدم_B];

                        مزدوج im = معلومات_A[(j+1)*قدم_A]*معلومات_B[j*قدم_B] -
                        معلومات_A[j*قدم_A]*معلومات_B[(j+1)*قدم_B];

                        معلومات_C[j*قدم_C] = re / denom;
                        معلومات_C[(j+1)*قدم_C] = im / denom;
                    }
                    وإلا
                        لكل( j = 1; j <= صفون - 2; j += 2 )
                        {
                            مزدوج denom = معلومات_B[j*قدم_B]*معلومات_B[j*قدم_B] +
                            معلومات_B[(j+1)*قدم_B]*معلومات_B[(j+1)*قدم_B] + eps;

                            مزدوج re = معلومات_A[j*قدم_A]*معلومات_B[j*قدم_B] -
                            معلومات_A[(j+1)*قدم_A]*معلومات_B[(j+1)*قدم_B];

                            مزدوج im = معلومات_A[(j+1)*قدم_A]*معلومات_B[j*قدم_B] +
                            معلومات_A[j*قدم_A]*معلومات_B[(j+1)*قدم_B];

                            معلومات_C[j*قدم_C] = re / denom;
                            معلومات_C[(j+1)*قدم_C] = im / denom;
                        }
                    إذا( k == 1 )
                        معلومات_A -= کانن_ڳڻپ - 1, معلومات_B -= کانن_ڳڻپ - 1, معلومات_C -= کانن_ڳڻپ - 1;
            }
        }

        لكل( ; صفون--; معلومات_A += قدم_A, معلومات_B += قدم_B, معلومات_C += قدم_C )
        {
            إذا( is_1d && cn == 1 )
            {
                معلومات_C[0] = معلومات_A[0] / (معلومات_B[0] + eps);
                إذا( کانن_ڳڻپ % 2 == 0 )
                    معلومات_C[j1] = معلومات_A[j1] / (معلومات_B[j1] + eps);
            }

            إذا( !conjB )
                لكل( j = j0; j < j1; j += 2 )
                {
                    مزدوج denom = معلومات_B[j]*معلومات_B[j] + معلومات_B[j+1]*معلومات_B[j+1] + eps;
                    مزدوج re = معلومات_A[j]*معلومات_B[j] + معلومات_A[j+1]*معلومات_B[j+1];
                    مزدوج im = معلومات_A[j+1]*معلومات_B[j] - معلومات_A[j]*معلومات_B[j+1];
                    معلومات_C[j] = re / denom;
                    معلومات_C[j+1] = im / denom;
                }
                وإلا
                    لكل( j = j0; j < j1; j += 2 )
                    {
                        مزدوج denom = معلومات_B[j]*معلومات_B[j] + معلومات_B[j+1]*معلومات_B[j+1] + eps;
                        مزدوج re = معلومات_A[j]*معلومات_B[j] - معلومات_A[j+1]*معلومات_B[j+1];
                        مزدوج im = معلومات_A[j+1]*معلومات_B[j] + معلومات_A[j]*معلومات_B[j+1];
                        معلومات_C[j] = re / denom;
                        معلومات_C[j+1] = im / denom;
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
سانچو<قسم_نالو T> ساكن فراغ
DCT( ثابت OcvDftOptions & c, ثابت T* src, size_t src_قدم, T* dst, size_t dst_قدم )
{
    إذا( c.n == 1 )
    {
        dst[0] = src[0];
        أرجع;
    }
    لاڳيتو_Dft(c, src, src_قدم/حجم(T), dst, dst_قدم/حجم(T), خطأ);
}

سانچو<قسم_نالو T> ساكن فراغ
IDCT( ثابت OcvDftOptions & c, ثابت T* src, size_t src_قدم, T* dst, size_t dst_قدم )
{
    إذا( c.n == 1 )
    {
        dst[0] = src[0];
        أرجع;
    }
    لاڳيتو_Dft(c, src, src_قدم/حجم(T), dst, dst_قدم/حجم(T), خطأ);
}

تعريفنوع فراغ (*DCTFunc)(ثابت OcvDftOptions & c, ثابت فراغ* src, size_t src_قدم,
                        فراغ* dst, size_t dst_قدم);

سانچو<قسم_نالو T, فراغ (*fn)(ثابت OcvDftOptions&, ثابت T*, size_t, T*, size_t)>
ساكن فراغ dctWrap(ثابت OcvDftOptions & c, ثابت فراغ* src, size_t src_قدم, فراغ* dst, size_t dst_قدم)
{
    fn(c, (ثابت T*)src, src_قدم, (T*)dst, dst_قدم);
}

}

#جيڪڏهن_وصف_ٿيل HAVE_IPP
نطاق cv
{

#إذا IPP_نسخو_X100 >= 900
تعريفنوع Ipp_حالت (CV_STDCALL * ippiDCTFunc)(ثابت Ipp32f* pSrc, صحيح src_قدم_3, Ipp32f* pDst, صحيح dst_قدم_3, ثابت فراغ* pDCTSpec, Ipp8u* p_بفر);
تعريفنوع Ipp_حالت (CV_STDCALL * ippiDCTInit)(فراغ* pDCTSpec, Ippi_سائيز roi_سائيز, Ipp8u* pMemInit );
تعريفنوع Ipp_حالت (CV_STDCALL * ippi_DCTGet_سائيز)(Ippi_سائيز roi_سائيز, صحيح* p_سائيز_Spec, صحيح* p_سائيز_Init, صحيح* p_سائيز_Buf);

سانچو<قسم_نالو SpecType, Ipp_حالت (CV_STDCALL *fn)(ثابت Ipp32f*, صحيح, Ipp32f*, صحيح, ثابت SpecType*, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ippiDCTWrap(ثابت Ipp32f* pSrc, صحيح src_قدم_3, Ipp32f* pDst, صحيح dst_قدم_3, ثابت فراغ* pDCTSpec, Ipp8u* p_بفر)
{
    أرجع fn(pSrc, src_قدم_3, pDst, dst_قدم_3, (ثابت SpecType*)pDCTSpec, p_بفر);
}

سانچو<قسم_نالو SpecType, Ipp_حالت (CV_STDCALL *fn)(SpecType*, Ippi_سائيز, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ippiDCTInitWrap(فراغ* pDCTSpec, Ippi_سائيز roi_سائيز, Ipp8u* pMemInit)
{
    أرجع fn((SpecType*)pDCTSpec, roi_سائيز, pMemInit);
}

سانچو<Ipp_حالت (CV_STDCALL *fn)(Ippi_سائيز, صحيح*, صحيح*, صحيح*)>
ساكن Ipp_حالت CV_STDCALL ippi_DCTGet_سائيز_Wrap(Ippi_سائيز roi_سائيز, صحيح* p_سائيز_Spec, صحيح* p_سائيز_Init, صحيح* p_سائيز_Buf)
{
    أرجع fn(roi_سائيز, p_سائيز_Spec, p_سائيز_Init, p_سائيز_Buf);
}

#وإلاإذا IPP_نسخو_X100 >= 700
تعريفنوع Ipp_حالت (CV_STDCALL * ippiDCTFunc)(ثابت Ipp32f*, صحيح, Ipp32f*, صحيح, ثابت فراغ*, Ipp8u*);
تعريفنوع Ipp_حالت (CV_STDCALL * ippiDCTInitAlloc)(فراغ**, Ippi_سائيز, IppHintAlgorithm);
تعريفنوع Ipp_حالت (CV_STDCALL * ippiDCTFree)(فراغ* pDCTSpec);
تعريفنوع Ipp_حالت (CV_STDCALL * ippi_DCTGet_Buf_سائيز)(ثابت فراغ*, صحيح*);

سانچو<قسم_نالو SpecType, Ipp_حالت (CV_STDCALL *fn)(ثابت Ipp32f*, صحيح, Ipp32f*, صحيح, ثابت SpecType*, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ippiDCTWrap(ثابت Ipp32f* pSrc, صحيح src_قدم_3, Ipp32f* pDst, صحيح dst_قدم_3, ثابت فراغ* pDCTSpec, Ipp8u* p_بفر)
{
    أرجع fn(pSrc, src_قدم_3, pDst, dst_قدم_3, (ثابت SpecType*)pDCTSpec, p_بفر);
}

سانچو<قسم_نالو SpecType, Ipp_حالت (CV_STDCALL *fn)(SpecType**, Ippi_سائيز, IppHintAlgorithm)>
ساكن Ipp_حالت CV_STDCALL ippiDCTInitAllocWrap(فراغ** pDCTSpec, Ippi_سائيز roi_سائيز, IppHintAlgorithm hint)
{
    أرجع fn((SpecType**)pDCTSpec, roi_سائيز, hint);
}

سانچو<قسم_نالو SpecType, Ipp_حالت (CV_STDCALL *fn)(SpecType*)>
ساكن Ipp_حالت CV_STDCALL ippiDCTFreeWrap(فراغ* pDCTSpec)
{
    أرجع fn((SpecType*)pDCTSpec);
}

سانچو<قسم_نالو SpecType, Ipp_حالت (CV_STDCALL *fn)(ثابت SpecType*, صحيح*)>
ساكن Ipp_حالت CV_STDCALL ippi_DCTGet_Buf_سائيز_Wrap(ثابت فراغ* pDCTSpec, صحيح* p_سائيز)
{
    أرجع fn((ثابت SpecType*)pDCTSpec, p_سائيز);
}
#جيڪڏهن_پورو

صنف DctIPPLoop_Invoker : عام ParallelLoopBody
{
عام:
    DctIPPLoop_Invoker(ثابت uchar * _src, size_t src_قدم_2, uchar * _dst, size_t dst_قدم_2, صحيح ويڪر_3, منطقي _inv, منطقي *_ok) :
        ParallelLoopBody(), src(_src), src_قدم(src_قدم_2), dst(_dst), dst_قدم(dst_قدم_2), ويڪر(ويڪر_3), inv(_inv), ok(_ok)
    {
        *ok = سچ;
    }

    مجازي فراغ عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        إذا(*ok == خطأ)
            أرجع;

#إذا IPP_نسخو_X100 >= 900
        Ippi_سائيز src_Roi_سائيز = {ويڪر, 1};

        صحيح spec_سائيز    = 0;
        صحيح init_سائيز    = 0;
        صحيح بفر_سائيز  = 0;

        Ipp8u* pDCTSpec = عدم;
        Ipp8u* p_بفر  = عدم;
        Ipp8u* pInitBuf = عدم;

        #وصف_ڪريو IPP_RETURN              \
            إذا(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            إذا(p_بفر)                 \
                ippFree(p_بفر);       \
            إذا(pInitBuf)                \
                ippFree(pInitBuf);      \
            أرجع;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_سائيز  ipp_Dct_Get_سائيز  = inv ? ippi_DCTGet_سائيز_Wrap<ippi_DCTInv_Get_سائيز_32f> : ippi_DCTGet_سائيز_Wrap<ippi_DCTFwd_Get_سائيز_32f>;

        إذا(ipp_Dct_Get_سائيز(src_Roi_سائيز, &spec_سائيز, &init_سائيز, &بفر_سائيز) < 0)
        {
            *ok = خطأ;
            أرجع;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_سائيز);
        إذا(!pDCTSpec && spec_سائيز)
        {
            *ok = خطأ;
            أرجع;
        }

        p_بفر  = (Ipp8u*)CV_IPP_MALLOC(بفر_سائيز);
        إذا(!p_بفر && بفر_سائيز)
        {
            *ok = خطأ;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_سائيز);
        إذا(!pInitBuf && init_سائيز)
        {
            *ok = خطأ;
            IPP_RETURN
        }

        إذا(ippDctInit(pDCTSpec, src_Roi_سائيز, pInitBuf) < 0)
        {
            *ok = خطأ;
            IPP_RETURN
        }

        لكل(صحيح i = range.شروعات; i < range.end; ++i)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (عائم*)(src + src_قدم * i), static_cast<صحيح>(src_قدم), (عائم*)(dst + dst_قدم * i), static_cast<صحيح>(dst_قدم), pDCTSpec, p_بفر) < 0)
            {
                *ok = خطأ;
                IPP_RETURN
            }
        }
        IPP_RETURN
#وصف_رد IPP_RETURN
#وإلاإذا IPP_نسخو_X100 >= 700
        فراغ* pDCTSpec;
        Auto_بفر<uchar> buf;
        uchar* p_بفر = 0;
        صحيح buf_سائيز=0;

        Ippi_سائيز src_Roi_سائيز = {ويڪر, 1};

        CV_SUPPRESS_DEPRECATED_شروعات

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_سائيز ipp_Get_Buf_سائيز = inv ? ippi_DCTGet_Buf_سائيز_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_سائيز_32f> : ippi_DCTGet_Buf_سائيز_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_سائيز_32f>;

        إذا (ippInitAlloc(&pDCTSpec, src_Roi_سائيز, ippAlgHintNone)>=0 && ipp_Get_Buf_سائيز(pDCTSpec, &buf_سائيز)>=0)
        {
            buf.allocate( buf_سائيز );
            p_بفر = (uchar*)buf;

            لكل( صحيح i = range.شروعات; i < range.end; ++i)
            {
                إذا(ippDctFun((عائم*)(src + src_قدم * i), static_cast<صحيح>(src_قدم), (عائم*)(dst + dst_قدم * i), static_cast<صحيح>(dst_قدم), pDCTSpec, (Ipp8u*)p_بفر) < 0)
                {
                    *ok = خطأ;
                    اكسر;
                }
            }
        }
        وإلا
            *ok = خطأ;

        إذا (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_پڇاڙي
#وإلا
        CV_اڻ_استعمال_ٿيل(range);
        *ok = خطأ;
#جيڪڏهن_پورو
    }

خاص:
    ثابت uchar * src;
    size_t src_قدم;
    uchar * dst;
    size_t dst_قدم;
    صحيح ويڪر;
    منطقي inv;
    منطقي *ok;
};

ساكن منطقي DctIPPLoop(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحيح ويڪر, صحيح اوچائي, منطقي inv)
{
    منطقي ok;
    parallel_for_(Range(0, اوچائي), DctIPPLoop_Invoker(src, src_قدم, dst, dst_قدم, ويڪر, inv, &ok), اوچائي/(مزدوج)(1<<4) );
    أرجع ok;
}

ساكن منطقي ippi_DCT_32f(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحيح ويڪر, صحيح اوچائي, منطقي inv, منطقي صف)
{
    CV_INSTRUMENT_REGION_IPP();

    إذا(صف)
        أرجع DctIPPLoop(src, src_قدم, dst, dst_قدم, ويڪر, اوچائي, inv);
    وإلا
    {
#إذا IPP_نسخو_X100 >= 900
        Ippi_سائيز src_Roi_سائيز = {ويڪر, اوچائي};

        صحيح spec_سائيز    = 0;
        صحيح init_سائيز    = 0;
        صحيح بفر_سائيز  = 0;

        Ipp8u* pDCTSpec = عدم;
        Ipp8u* p_بفر  = عدم;
        Ipp8u* pInitBuf = عدم;

        #وصف_ڪريو IPP_RELEASE             \
            إذا(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            إذا(p_بفر)                 \
                ippFree(p_بفر);       \
            إذا(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_سائيز  ipp_Dct_Get_سائيز  = inv ? ippi_DCTGet_سائيز_Wrap<ippi_DCTInv_Get_سائيز_32f> : ippi_DCTGet_سائيز_Wrap<ippi_DCTFwd_Get_سائيز_32f>;

        إذا(ipp_Dct_Get_سائيز(src_Roi_سائيز, &spec_سائيز, &init_سائيز, &بفر_سائيز) < 0)
            أرجع خطأ;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_سائيز);
        إذا(!pDCTSpec && spec_سائيز)
            أرجع خطأ;

        p_بفر  = (Ipp8u*)CV_IPP_MALLOC(بفر_سائيز);
        إذا(!p_بفر && بفر_سائيز)
        {
            IPP_RELEASE
            أرجع خطأ;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_سائيز);
        إذا(!pInitBuf && init_سائيز)
        {
            IPP_RELEASE
            أرجع خطأ;
        }

        إذا(ippDctInit(pDCTSpec, src_Roi_سائيز, pInitBuf) < 0)
        {
            IPP_RELEASE
            أرجع خطأ;
        }

        إذا(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (عائم*)src, static_cast<صحيح>(src_قدم), (عائم*)dst, static_cast<صحيح>(dst_قدم), pDCTSpec, p_بفر) < 0)
        {
            IPP_RELEASE
            أرجع خطأ;
        }

        IPP_RELEASE
        أرجع سچ;
#وصف_رد IPP_RELEASE
#وإلاإذا IPP_نسخو_X100 >= 700
        Ipp_حالت حالت;
        فراغ* pDCTSpec;
        Auto_بفر<uchar> buf;
        uchar* p_بفر = 0;
        صحيح buf_سائيز=0;

        Ippi_سائيز src_Roi_سائيز = {ويڪر, اوچائي};

        CV_SUPPRESS_DEPRECATED_شروعات

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_سائيز ipp_Get_Buf_سائيز = inv ? ippi_DCTGet_Buf_سائيز_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_سائيز_32f> : ippi_DCTGet_Buf_سائيز_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_سائيز_32f>;

        حالت = ippStsErr;

        إذا (ippInitAlloc(&pDCTSpec, src_Roi_سائيز, ippAlgHintNone)>=0 && ipp_Get_Buf_سائيز(pDCTSpec, &buf_سائيز)>=0)
        {
            buf.allocate( buf_سائيز );
            p_بفر = (uchar*)buf;

            حالت = ippDctFun((عائم*)src, static_cast<صحيح>(src_قدم), (عائم*)dst, static_cast<صحيح>(dst_قدم), pDCTSpec, (Ipp8u*)p_بفر);
        }

        إذا (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_پڇاڙي

        أرجع حالت >= 0;
#وإلا
        CV_اڻ_استعمال_ٿيل(src); CV_اڻ_استعمال_ٿيل(dst); CV_اڻ_استعمال_ٿيل(inv); CV_اڻ_استعمال_ٿيل(صف);
        أرجع خطأ;
#جيڪڏهن_پورو
    }
}
}
#جيڪڏهن_پورو

نطاق cv {

صنف OcvDctImpl CV_FINAL : عام hal::DCT2D
{
عام:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    Auto_بفر<uchar> ws;
    منطقي is_صف_Transform;
    منطقي isInverse;
    منطقي isContinuous;
    صحيح شروعات_stage;
    صحيح پڇاڙي_stage;
    صحيح ويڪر;
    صحيح اوچائي;
    صحيح اونهائي;

    فراغ init(صحيح ويڪر_3, صحيح اوچائي_3, صحيح اونهائي_2, صحيح flags)
    {
        ويڪر = ويڪر_3;
        اوچائي = اوچائي_3;
        اونهائي = اونهائي_2;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        is_صف_Transform = (flags & CV_HAL_DFT_صفون) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        ساكن DCTFunc dct_tbl[4] =
        {
            dctWrap<عائم, DCT<عائم>>,
            dctWrap<عائم, IDCT<عائم>>,
            dctWrap<مزدوج, DCT<مزدوج>>,
            dctWrap<مزدوج, IDCT<مزدوج>>
        };
        dct_func = dct_tbl[(صحيح)isInverse + (اونهائي == CV_64F)*2];
        opt.isComplex = خطأ;
        opt.isInverse = خطأ;
        opt.scale = 1.;

        إذا (is_صف_Transform || اوچائي == 1 || (ويڪر == 1 && isContinuous))
        {
            شروعات_stage = پڇاڙي_stage = 0;
        }
        وإلا
        {
            شروعات_stage = (ويڪر == 1);
            پڇاڙي_stage = 1;
        }
    }
    فراغ apply(ثابت uchar *src, size_t src_قدم, uchar *dst, size_t dst_قدم) CV_OVERRIDE
    {
        CV_IPP_لاڳيتو(IPP_نسخو_X100 >= 700 && اونهائي == CV_32F, ippi_DCT_32f(src, src_قدم, dst, dst_قدم, ويڪر, اوچائي, isInverse, is_صف_Transform))

        صحيح prev_len = 0;
        صحيح elem_سائيز_2 = (اونهائي == CV_32F) ? حجم(عائم) : حجم(مزدوج);
        ثابت DFTKernels& kernels = getDFTKernels(اونهائي);

        لكل(صحيح stage = شروعات_stage ; stage <= پڇاڙي_stage; stage++ )
        {
            ثابت uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            صحيح len, ڳڻپ_2;

            إذا( stage == 0 )
            {
                len = ويڪر;
                ڳڻپ_2 = اوچائي;
                إذا( len == 1 && !is_صف_Transform )
                {
                    len = اوچائي;
                    ڳڻپ_2 = 1;
                }
                sstep0 = src_قدم;
                dstep0 = dst_قدم;
                sstep1 = dstep1 = elem_سائيز_2;
            }
            وإلا
            {
                len = اوچائي;
                ڳڻپ_2 = ويڪر;
                sstep1 = src_قدم;
                dstep1 = dst_قدم;
                sstep0 = dstep0 = elem_سائيز_2;
            }

            opt.n = len;

            إذا( len != prev_len )
            {
                إذا( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                إذا( len > 1 )
                {
                    plan.build(isInverse ? DFT_قسم_IDCT : DFT_قسم_DCT, len, اونهائي, kernels.vlanes);
                    ws.allocate(plan.ws_بائيٽون);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = ws.معلومات();
                prev_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            مزدوج nstripes = dftParallelStripes(ڳڻپ_2, len);
            إذا( len > 1 && ڳڻپ_2 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                خودڪار process_سٽون = [&](ثابت Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    Auto_بفر<uchar> ws_(plan.ws_بائيٽون);
                    opt_.workspace = ws_.معلومات();
                    لكل( صحيح i = range.شروعات; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, ڳڻپ_2), process_سٽون, nstripes);
            }
            وإلا
            {
                لكل(بنانشان i = 0; i < static_cast<بنانشان>(ڳڻپ_2); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_قدم = dst_قدم;
        }
    }
};

بنية ReplacementDCT2D : عام hal::DCT2D
{
    cvhalDFT *context;
    منطقي isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(خطأ) {}
    منطقي init(صحيح ويڪر, صحيح اوچائي, صحيح اونهائي, صحيح flags)
    {
        صحيح res = cv_hal_dctInit2D(&context, ويڪر, اوچائي, اونهائي, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        أرجع isInitialized;
    }
    فراغ apply(ثابت uchar *src_معلومات, size_t src_قدم, uchar *dst_معلومات, size_t dst_قدم) CV_OVERRIDE
    {
        إذا (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_معلومات, src_قدم, dst_معلومات, dst_قدم);
        }
    }
    ~ReplacementDCT2D()
    {
        إذا (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

نطاق hal {

Ptr<DCT2D> DCT2D::create(صحيح ويڪر, صحيح اوچائي, صحيح اونهائي, صحيح flags)
{
    {
        ReplacementDCT2D *impl = جديد ReplacementDCT2D();
        إذا (impl->init(ويڪر, اوچائي, اونهائي, flags))
        {
            أرجع Ptr<DCT2D>(impl);
        }
        ميٽيو impl;
    }
    {
        OcvDctImpl *impl = جديد OcvDctImpl();
        impl->init(ويڪر, اوچائي, اونهائي, flags);
        أرجع Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

فراغ cv::dct( داخلا_Array _src0, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    إذا (src.empty())
    {
        _dst.release();
        أرجع;
    }
    صحيح type = src.type(), اونهائي = src.اونهائي();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.صفون, src.کانن_ڳڻپ, type );
    Mat dst = _dst.getMat();

    صحيح f = 0;
    إذا ((flags & DFT_صفون) != 0)
        f |= CV_HAL_DFT_صفون;
    إذا ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    إذا (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.کانن_ڳڻپ, src.صفون, اونهائي, f);
    c->apply(src.معلومات, src.قدم, dst.معلومات, dst.قدم);
}


فراغ cv::idct( داخلا_Array src, OutputArray dst, صحيح flags )
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
    إذا( (بنانشان)size0 >= (بنانشان)optimalDFTSizeTab[b] )
        أرجع -1;

    طالما( a < b )
    {
        صحيح c = (a + b) >> 1;
        إذا( size0 <= optimalDFTSizeTab[c] )
            b = c;
        وإلا
            a = c+1;
    }

    أرجع optimalDFTSizeTab[b];
}
/* End of file. */
