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

#شامل "precomp.hpp"
#شامل "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#شامل "opencv2/core/opencl/runtime/opencl_core.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "dxt.hpp"
#شامل "dxt.simd.hpp"
#شامل "dxt.simd_declarations.hpp"
#شامل <map>

نامخلا cv
{

#اگر IPP_نسخہ_X100 >= 710
#تعریف_کرو USE_IPP_DFT 1
#ورنہ
#تعریف_منسوخ USE_IPP_DFT
#اگر_ختم

#اگر defined USE_IPP_DFT
#اگر IPP_نسخہ_X100 >= 202220
#تعریف_کرو IPP_DISABLE_DFT32F ((گہرائی == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#ورنہ
#تعریف_کرو IPP_DISABLE_DFT32F جھوٹ
#اگر_ختم
#اگر_ختم

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#اگر_تعریف_شدہ HAVE_OPENCL
// factorization used by the OpenCL radix selection
ساکن صحیح
DFTFactorize( صحیح n, صحیح* factors )
{
    صحیح nf = 0, f, i, j;

    اگر( n <= 5 )
    {
        factors[0] = n;
        واپس 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    اگر( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    برائے( f = 3; n > 1; )
    {
        صحیح d = n/f;
        اگر( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        ورنہ
        {
            f += 2;
            اگر( f*f > n )
                توڑو;
        }
    }

    اگر( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    برائے( i = f; i < (nf+f)/2; i++ )
        CV_تبادلہ( factors[i], factors[nf-i-1+f], j );

    واپس nf;
}
#اگر_ختم

#اگر_تعریف_شدہ USE_IPP_DFT
ساکن Ipp_حالت ippsDFTFwd_CToC( ثابت Complex<اعشاری>* src, Complex<اعشاری>* dst,
                             ثابت خالی* spec, uchar* buf)
{
    واپس CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (ثابت Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ثابت IppsDFTSpec_C_32fc*)spec, buf);
}

ساکن Ipp_حالت ippsDFTFwd_CToC( ثابت Complex<دوہرا>* src, Complex<دوہرا>* dst,
                             ثابت خالی* spec, uchar* buf)
{
    واپس CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (ثابت Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ثابت IppsDFTSpec_C_64fc*)spec, buf);
}

ساکن Ipp_حالت ippsDFTInv_CToC( ثابت Complex<اعشاری>* src, Complex<اعشاری>* dst,
                             ثابت خالی* spec, uchar* buf)
{
    واپس CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (ثابت Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ثابت IppsDFTSpec_C_32fc*)spec, buf);
}

ساکن Ipp_حالت ippsDFTInv_CToC( ثابت Complex<دوہرا>* src, Complex<دوہرا>* dst,
                                  ثابت خالی* spec, uchar* buf)
{
    واپس CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (ثابت Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ثابت IppsDFTSpec_C_64fc*)spec, buf);
}

ساکن Ipp_حالت ippsDFTFwd_RToPack( ثابت اعشاری* src, اعشاری* dst,
                                     ثابت خالی* spec, uchar* buf)
{
    واپس CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (ثابت IppsDFTSpec_R_32f*)spec, buf);
}

ساکن Ipp_حالت ippsDFTFwd_RToPack( ثابت دوہرا* src, دوہرا* dst,
                                     ثابت خالی* spec, uchar* buf)
{
    واپس CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (ثابت IppsDFTSpec_R_64f*)spec, buf);
}

ساکن Ipp_حالت ipps_DFTInv_Pack_مقصد_R( ثابت اعشاری* src, اعشاری* dst,
                                     ثابت خالی* spec, uchar* buf)
{
    واپس CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_مقصد_R_32f, src, dst, (ثابت IppsDFTSpec_R_32f*)spec, buf);
}

ساکن Ipp_حالت ipps_DFTInv_Pack_مقصد_R( ثابت دوہرا* src, دوہرا* dst,
                                     ثابت خالی* spec, uchar* buf)
{
    واپس CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_مقصد_R_64f, src, dst, (ثابت IppsDFTSpec_R_64f*)spec, buf);
}
#اگر_ختم

ساخت OcvDftOptions;

قسمتعریف خالی (*DFTFunc)(ثابت OcvDftOptions & c, ثابت خالی* src, خالی* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
ساخت OcvDftOptions {
    دوہرا scale;
    صحیح n;

    منطقی isInverse;
    منطقی isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    منطقی useIpp;

#اگر_تعریف_شدہ USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#اگر_ختم

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
        isInverse = جھوٹ;
        isComplex = جھوٹ;
        useIpp = جھوٹ;
#اگر_تعریف_شدہ USE_IPP_DFT
        ipp_spec = 0;
        ipp_work = 0;
#اگر_ختم
        dft_func = 0;
    }
};


/****************************************************************************************\
                     New 1D engine: kernel table, plan builder, driver
\****************************************************************************************/

ساکن DFTKernels getDFTKernelsDispatch(صحیح گہرائی)
{
    CV_CPU_DISPATCH(getDFTKernels_, (گہرائی), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
ساکن ثابت DFTKernels& getDFTKernels(صحیح گہرائی)
{
    ساکن ثابت DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    ساکن ثابت DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    واپس گہرائی == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
ساخت DftTwiddleGen
{
    صحیح M, logB, نقاب_B;
    Auto_بفر<دوہرا, 1024> buf;
    دوہرا *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    ساکن خالی fill_جدول(دوہرا* re, دوہرا* im, صحیح گنتی_2, دوہرا wr, دوہرا wi, دوہرا قدم_Angle)
    {
        برائے (صحیح i0 = 0; i0 < گنتی_2; i0 += 16)
        {
            دوہرا t0 = قدم_Angle*i0;
            دوہرا cr = i0 == 0 ? 1 : جیب_تمام(t0), ci = i0 == 0 ? 0 : جیب(t0);
            صحیح i1 = std::min(i0 + 16, گنتی_2);
            برائے (صحیح i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                دوہرا t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    خالی init(صحیح _M)
    {
        M = _M;
        logB = 0;
        جب ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        صحیح B = 1 << logB;
        نقاب_B = B - 1;
        صحیح nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.ڈیٹا(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        دوہرا a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fill_جدول(lo_re, lo_im, B, جیب_تمام(a), جیب(a), a);
        fill_جدول(hi_re, hi_im, nhi, جیب_تمام(a*B), جیب(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    خالی get(صحیح m, دوہرا& re, دوہرا& im) ثابت
    {
        CV_DbgAssert(m >= 0);
        اگر (m >= M)
            m %= M;
        صحیح a = m >> logB, b = m & نقاب_B;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

سانچہ<قسم_نام T>
خالی DftPlan::fillTables(size_t ntw, منطقی need_rtw, منطقی need_dct)
{
    DftPlan& p = *یہ;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    صحیح M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*حجم(T) + حجم(دوہرا) - 1)/حجم(دوہرا) + 1);
    T* cur = (T*)p.tw.ڈیٹا();
    صحیح maxL = 1;
    برائے (صحیح s = 0; s < p.nstages; s++)
        اگر (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].وسعت);
    Auto_بفر<دوہرا, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    دوہرا* twd = twd_buf.ڈیٹا();
    برائے (صحیح s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        صحیح r = st.radix, L = st.وسعت, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        صحیح قدم = M/(r*L);
        اگر (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr صحیح K = 8;
            دوہرا cr[K], ci[K], wr, wi;
            gen.get(K*قدم, wr, wi);
            دوہرا* t1r = twd; دوہرا* t1i = twd + L;      // leg 1 kept in double for the products
            برائے (صحیح j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                برائے (صحیح k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*قدم, cr[k], ci[k]);
                صحیح j1 = std::min(j0 + 16*K, L);
                برائے (صحیح j = j0; j < j1; j += K)
                    برائے (صحیح k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        دوہرا t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            برائے (صحیح j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            برائے (صحیح q = 2; q < r; q++)
            {
                ثابت T* pr = twr + (q-2)*ts; ثابت T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                برائے (صحیح j = 0; j < L; j++)
                {
                    دوہرا ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        ورنہ
        {
            // large odd radix: exact-ish generator lookup per entry
            برائے (صحیح q = 1; q < r; q++)
                برائے (صحیح j = 0; j < L; j++)
                {
                    دوہرا re, im;
                    gen.get(q*j*قدم, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        اگر (r >= 7)
        {
            صحیح h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            برائے (صحیح pp = 1; pp <= h; pp++)
                برائے (صحیح k = 1; k <= h; k++)
                {
                    دوہرا t = 2*CV_PI*((دوہرا)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)جیب_تمام(t);
                    sn[(pp-1)*h + (k-1)] = (T)جیب(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    اگر (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        صحیح قدم = M/n;
        برائے (صحیح k = 0; k <= nc; k++)
        {
            دوہرا re, im;
            gen.get(k*قدم, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    اگر (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        دوہرا scale = (p.قسم == DFT_قسم_DCT ? 2 : 1)*std::جذر(1./(2*n));
        صحیح قدم = M/(4*n);
        برائے (صحیح k = 0; k <= nc; k++)
        {
            دوہرا re, im;
            gen.get(k*قدم, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
خالی DftPlan::build(صحیح قسم_2, صحیح _n, صحیح گہرائی_2, صحیح _vl)
{
    DftPlan& p = *یہ;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(گہرائی_2 == CV_32F || گہرائی_2 == CV_64F);
    منطقی real_قسم = قسم_2 != DFT_قسم_C2_C;
    منطقی dct_قسم = قسم_2 == DFT_قسم_DCT || قسم_2 == DFT_قسم_IDCT;
    اگر (dct_قسم)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.قسم = قسم_2; p.n = _n; p.گہرائی = گہرائی_2; p.vl = _vl;
    p.nc = real_قسم && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = گہرائی == CV_32F ? حجم(اعشاری) : حجم(دوہرا);
    p.real_داخلہ = قسم == DFT_قسم_R2_C && (n & 1) != 0;
    p.need_tmp = قسم == DFT_قسم_C2_R || قسم == DFT_قسم_DCT || قسم == DFT_قسم_IDCT;
    p.pingpong = جھوٹ;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    صحیح k2 = 0, rest = nc;
    جب ((rest & 1) == 0) { rest >>= 1; k2++; }
    صحیح r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.پہلا_radix = r0;
    صحیح radices[DFT_MAX_STAGES], nst = 0;
    برائے (صحیح m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    جب (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    جب (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    برائے (صحیح f = 7; rest > 1; f += 2)
    {
        اگر (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            توڑو;
        }
        جب (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    صحیح L = r0, max_h = 0;
    size_t ntw = 0;
    برائے (صحیح s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        صحیح r = radices[s];
        st.radix = r; st.وسعت = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        اگر (vl > 1 && L >= vl)
        {
            st.منوال = DFT_STAGE_بھرا;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            اگر (L % vl != 0)
                p.pingpong = سچ;   // back-off clamp => overlapping stores => must be out-of-place
        }
        ورنہ
        {
            st.منوال = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        اگر (r >= 7)
        {
            صحیح h = (r - 1)/2;
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
    صحیح seq[DFT_MAX_STAGES + 1], nseq = 0;
    اگر (r0 > 1) seq[nseq++] = r0;
    برائے (صحیح s = 0; s < nst; s++) seq[nseq++] = radices[s];
    صحیح digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        صحیح Ls = 1;
        برائے (صحیح s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    اگر (r0 == 1)
    {
        p.itab.allocate(nc*2);
        صحیح* it_all = p.itab.ڈیٹا();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        صحیح n_اندر = std::min(nseq, 2);
        صحیح r_0 = n_اندر > 0 ? seq[0] : 1, w_0 = n_اندر > 0 ? weights[0] : 0;
        صحیح r_1 = n_اندر > 1 ? seq[1] : 1, w_1 = n_اندر > 1 ? weights[1] : 0;
        برائے (صحیح pos = 0, rev_hi = 0; pos < nc; )
        {
            برائے (صحیح d1 = 0; d1 < r_1; d1++)
            {
                صحیح rev = rev_hi + d1*w_1;
                صحیح* it = it_all + pos*2;
                اگر (p.real_داخلہ)
                {
                    برائے (صحیح d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                ورنہ
                {
                    برائے (صحیح d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            برائے (صحیح s = n_اندر; s < nseq; s++)
            {
                rev_hi += weights[s];
                اگر (++digits[s] < seq[s])
                    توڑو;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    ورنہ
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        صحیح M = nc/r0;
        p.itab.allocate(M);
        صحیح* gtab = p.itab.ڈیٹا();
        صحیح gw[DFT_MAX_STAGES + 1];
        {
            صحیح Ls = 1;
            برائے (صحیح s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        صحیح sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        صحیح r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        صحیح r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        برائے (صحیح بنیاد = 0, g_hi = 0; بنیاد < M; )
        {
            برائے (صحیح db = 0; db < r_b; db++)
            {
                صحیح g = (g_hi + db*g_b)*r0;
                برائے (صحیح da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[بنیاد++] = g;
            }
            برائے (صحیح s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                اگر (++digits[s] < seq[s])
                    توڑو;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    منطقی need_rtw = real_قسم && n % 2 == 0;
    اگر (need_rtw) ntw += 2*(size_t)(nc + 1);
    اگر (dct_قسم) ntw += 2*(size_t)(nc + 1);
    اگر (گہرائی == CV_32F)
        fillTables<اعشاری>(ntw, need_rtw, dct_قسم);
    ورنہ
        fillTables<دوہرا>(ntw, need_rtw, dct_قسم);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_بائٹیں = 2*(size_t)nc*esz;
    منطقی two_جوڑے = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_جوڑے ? pair_بائٹیں : 0;
    p.scratch_ofs = pair_بائٹیں*(two_جوڑے ? 2 : 1);
    size_t scratch_بائٹیں = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_بائٹیں = p.scratch_ofs + scratch_بائٹیں + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
ساکن خالی تسلسل_Dft(ثابت OcvDftOptions& c, ثابت خالی* src, size_t sstep, خالی* dst, size_t dstep,
                   منطقی complex_io)
{
    ثابت DftPlan& p = *c.plan;
    ثابت DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.گہرائی == CV_32F ? حجم(اعشاری) : حجم(دوہرا);
    uchar* بنیاد = alignPtr(c.workspace, 64);
    uchar* re0 = بنیاد;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = بنیاد + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = بنیاد + p.scratch_ofs;

    ثابت خالی* asrc = src;
    size_t astep = sstep;
    منطقی conj = جھوٹ;
    اگر (p.قسم == DFT_قسم_C2_R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = سچ;
    }
    ورنہ اگر (p.قسم == DFT_قسم_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = سچ;
    }
    ورنہ اگر (p.قسم == DFT_قسم_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    ورنہ اگر (p.قسم == DFT_قسم_C2_C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.پہلا_radix == 1);

    DftPreprocFunc pre = p.پہلا_radix == 1 ? k.preprocRadix0 : p.پہلا_radix == 2 ? k.preprocRadix2 :
                         p.پہلا_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    برائے (صحیح i = 0; i < p.nstages; i++)
    {
        ثابت DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        اگر (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        ورنہ
            fn(st, sre, sim, sre, sim, scratch);
    }

    دوہرا scale = c.scale;
    انتخاب (p.قسم)
    {
    حالت DFT_قسم_C2_C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        توڑو;
    حالت DFT_قسم_R2_C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        توڑو;
    حالت DFT_قسم_C2_R:
        اگر (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        ورنہ
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        توڑو;
    حالت DFT_قسم_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        توڑو;
    طےشدہ:
        CV_Assert(p.قسم == DFT_قسم_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
سانچہ<قسم_نام T> ساکن خالی
DFT(ثابت OcvDftOptions & c, ثابت Complex<T>* src, Complex<T>* dst)
{
    اگر( c.useIpp )
    {
#اگر_تعریف_شدہ USE_IPP_DFT
        اگر( !c.isInverse )
        {
            اگر (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_جوڑو(CV_IMPL_IPP);
                واپس;
            }
        }
        ورنہ
        {
            اگر (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_جوڑو(CV_IMPL_IPP);
                واپس;
            }
        }
        سیٹ_Ipp_Error_حالت();
#اگر_ختم
    }

    تسلسل_Dft(c, src, 1, dst, 1, جھوٹ);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
سانچہ<قسم_نام T> ساکن خالی
RealDFT(ثابت OcvDftOptions & c, ثابت T* src, T* dst)
{
    صحیح complex_output = c.isComplex;

    اگر( c.useIpp )
    {
#اگر_تعریف_شدہ USE_IPP_DFT
        صحیح n = c.n;
        T* ipp_dst = dst + complex_output;
        اگر (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            اگر( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                اگر( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_جوڑو(CV_IMPL_IPP);
            واپس;
        }
        سیٹ_Ipp_Error_حالت();
#اگر_ختم
    }
    تسلسل_Dft(c, src, 1, dst, 1, complex_output != 0);
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
سانچہ<قسم_نام T> ساکن خالی
CCSIDFT(ثابت OcvDftOptions & c, ثابت T* src, T* dst)
{
    صحیح complex_داخلہ = c.isComplex;

    اگر( complex_داخلہ )
        CV_Assert( src != dst );
    اگر( c.useIpp )
    {
#اگر_تعریف_شدہ USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        ثابت T* ipp_src = src;
        T save_s1 = 0;
        اگر( complex_داخلہ )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        Ipp_حالت حالت_2 = ipps_DFTInv_Pack_مقصد_R( ipp_src, dst, c.ipp_spec, c.ipp_work );
        اگر( complex_داخلہ )
            ((T*)src)[1] = save_s1;
        اگر( حالت_2 >= 0 )
        {
            CV_IMPL_جوڑو(CV_IMPL_IPP);
            واپس;
        }
        سیٹ_Ipp_Error_حالت();
#اگر_ختم
    }
    تسلسل_Dft(c, src, 1, dst, 1, complex_داخلہ != 0);
}

ساکن خالی
Copy_عمود( ثابت uchar* _src, size_t src_قدم,
            uchar* _dst, size_t dst_قدم,
            صحیح len, size_t elem_سائز_2 )
{
    صحیح i, t0, t1;
    ثابت صحیح* src = (ثابت صحیح*)_src;
    صحیح* dst = (صحیح*)_dst;
    src_قدم /= حجم(src[0]);
    dst_قدم /= حجم(dst[0]);

    اگر( elem_سائز_2 == حجم(صحیح) )
    {
        برائے( i = 0; i < len; i++, src += src_قدم, dst += dst_قدم )
            dst[0] = src[0];
    }
    ورنہ اگر( elem_سائز_2 == حجم(صحیح)*2 )
    {
        برائے( i = 0; i < len; i++, src += src_قدم, dst += dst_قدم )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    ورنہ اگر( elem_سائز_2 == حجم(صحیح)*4 )
    {
        برائے( i = 0; i < len; i++, src += src_قدم, dst += dst_قدم )
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
سانچہ<قسم_نام T> ساکن اندرونی خالی
copyFromColumns_( ثابت uchar* src, size_t src_قدم, T* dst, صحیح ncols, صحیح len )
{
    برائے( صحیح i = 0; i < len; i++, src += src_قدم )
    {
        ثابت T* s = (ثابت T*)src;
        برائے( صحیح k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

ساکن خالی
CopyFromColumns( ثابت uchar* src, size_t src_قدم, uchar* dst, صحیح ncols, صحیح len, size_t elem_سائز_2 )
{
    اگر( elem_سائز_2 == حجم(صحیح) )
        copyFromColumns_(src, src_قدم, (صحیح*)dst, ncols, len);
    ورنہ اگر( elem_سائز_2 == حجم(int64) )
        copyFromColumns_(src, src_قدم, (int64*)dst, ncols, len);
    ورنہ
    {
        CV_Assert( elem_سائز_2 == حجم(int64)*2 );
        copyFromColumns_(src, src_قدم, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
سانچہ<قسم_نام T> ساکن اندرونی خالی
copy_مقصد_Columns( ثابت T* src, uchar* dst, size_t dst_قدم, صحیح ncols, صحیح len )
{
    برائے( صحیح i = 0; i < len; i++, dst += dst_قدم )
    {
        T* d = (T*)dst;
        برائے( صحیح k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

ساکن خالی
Copy_مقصد_Columns( ثابت uchar* src, uchar* dst, size_t dst_قدم, صحیح ncols, صحیح len, size_t elem_سائز_2 )
{
    اگر( elem_سائز_2 == حجم(صحیح) )
        copy_مقصد_Columns((ثابت صحیح*)src, dst, dst_قدم, ncols, len);
    ورنہ اگر( elem_سائز_2 == حجم(int64) )
        copy_مقصد_Columns((ثابت int64*)src, dst, dst_قدم, ncols, len);
    ورنہ
    {
        CV_Assert( elem_سائز_2 == حجم(int64)*2 );
        copy_مقصد_Columns((ثابت Vec<int64, 2>*)src, dst, dst_قدم, ncols, len);
    }
}

ساکن خالی
ExpandCCS( uchar* _ptr, صحیح n, صحیح elem_سائز_2 )
{
    صحیح i;
    اگر( elem_سائز_2 == (صحیح)حجم(اعشاری) )
    {
        اعشاری* p = (اعشاری*)_ptr;
        برائے( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        اگر( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        برائے( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    ورنہ
    {
        دوہرا* p = (دوہرا*)_ptr;
        برائے( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        اگر( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        برائے( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

سانچہ<قسم_نام T, خالی (*fn)(ثابت OcvDftOptions&, ثابت T*, T*)>
ساکن خالی dftWrap(ثابت OcvDftOptions & c, ثابت خالی* src, خالی* dst)
{
    fn(c, (ثابت T*)src, (T*)dst);
}

}

#اگر_تعریف_شدہ USE_IPP_DFT
قسمتعریف Ipp_حالت (CV_STDCALL* Ipp_DFTGet_سائز_Func)(صحیح, صحیح, IppHintAlgorithm, صحیح*, صحیح*, صحیح*);
قسمتعریف Ipp_حالت (CV_STDCALL* IppDFTInitFunc)(صحیح, صحیح, IppHintAlgorithm, خالی*, uchar*);

سانچہ<قسم_نام SpecType, Ipp_حالت (CV_STDCALL *init_fn)(صحیح, صحیح, IppHintAlgorithm, SpecType*, Ipp8u*)>
ساکن Ipp_حالت CV_STDCALL ippDFTInitWrap(صحیح n, صحیح flags, IppHintAlgorithm hint, خالی* spec, Ipp8u* initbuf)
{
    واپس init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#اگر_ختم

نامخلا cv
{
#اگر defined USE_IPP_DFT

قسمتعریف Ipp_حالت (CV_STDCALL* ippiDFT_C_Func)(ثابت Ipp32fc*, صحیح, Ipp32fc*, صحیح, ثابت IppiDFTSpec_C_32fc*, Ipp8u*);
قسمتعریف Ipp_حالت (CV_STDCALL* ippiDFT_R_Func)(ثابت Ipp32f* , صحیح, Ipp32f* , صحیح, ثابت IppiDFTSpec_R_32f* , Ipp8u*);

سانچہ <قسم_نام Dft>
جماعت Dft_C_IPPLoop_Invoker : عوامی ParallelLoopBody
{
عوامی:

    Dft_C_IPPLoop_Invoker(ثابت uchar * _src, size_t src_قدم_2, uchar * _dst, size_t dst_قدم_2, صحیح چوڑائی_3,
                          ثابت Dft& _ippidft, صحیح _norm_flag, منطقی *_ok) :
        ParallelLoopBody(),
        src(_src), src_قدم(src_قدم_2), dst(_dst), dst_قدم(dst_قدم_2), چوڑائی(چوڑائی_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = سچ;
    }

    مجازی خالی عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        Ipp_حالت حالت_2;
        Ipp8u* p_بفر = 0;
        Ipp8u* pMemInit= 0;
        صحیح سائز_بفر=0;
        صحیح سائز_Spec=0;
        صحیح سائز_Init=0;

        Ippi_سائز src_Roi_سائز = {چوڑائی, 1};

        حالت_2 = ippi_DFTGet_سائز_C_32fc(src_Roi_سائز, norm_flag, ippAlgHintNone, &سائز_Spec, &سائز_Init, &سائز_بفر );
        اگر ( حالت_2 < 0 )
        {
            *ok = جھوٹ;
            واپس;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( سائز_Spec );

        اگر ( سائز_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( سائز_Init );

        اگر ( سائز_بفر > 0 )
            p_بفر = (Ipp8u*)CV_IPP_MALLOC( سائز_بفر );

        حالت_2 = ippiDFTInit_C_32fc( src_Roi_سائز, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        اگر ( سائز_Init > 0 )
            ippFree( pMemInit );

        اگر ( حالت_2 < 0 )
        {
            ippFree( pDFTSpec );
            اگر ( سائز_بفر > 0 )
                ippFree( p_بفر );
            *ok = جھوٹ;
            واپس;
        }

        برائے( صحیح i = range.آغاز; i < range.end; ++i)
            اگر(!ippidft((Ipp32fc*)(src + src_قدم * i), src_قدم, (Ipp32fc*)(dst + dst_قدم * i), dst_قدم,
                        pDFTSpec, (Ipp8u*)p_بفر))
            {
                *ok = جھوٹ;
            }

        اگر ( سائز_بفر > 0 )
            ippFree( p_بفر );

        ippFree( pDFTSpec );
        CV_IMPL_جوڑو(CV_IMPL_IPP|CV_IMPL_MT);
    }

نجی:
    ثابت uchar * src;
    size_t src_قدم;
    uchar * dst;
    size_t dst_قدم;
    صحیح چوڑائی;
    ثابت Dft& ippidft;
    صحیح norm_flag;
    منطقی *ok;

    ثابت Dft_C_IPPLoop_Invoker& عامل= (ثابت Dft_C_IPPLoop_Invoker&);
};

سانچہ <قسم_نام Dft>
جماعت Dft_R_IPPLoop_Invoker : عوامی ParallelLoopBody
{
عوامی:

    Dft_R_IPPLoop_Invoker(ثابت uchar * _src, size_t src_قدم_2, uchar * _dst, size_t dst_قدم_2, صحیح چوڑائی_3,
                          ثابت Dft& _ippidft, صحیح _norm_flag, منطقی *_ok) :
        ParallelLoopBody(),
        src(_src), src_قدم(src_قدم_2), dst(_dst), dst_قدم(dst_قدم_2), چوڑائی(چوڑائی_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = سچ;
    }

    مجازی خالی عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        Ipp_حالت حالت_2;
        Ipp8u* p_بفر = 0;
        Ipp8u* pMemInit= 0;
        صحیح سائز_بفر=0;
        صحیح سائز_Spec=0;
        صحیح سائز_Init=0;

        Ippi_سائز src_Roi_سائز = {چوڑائی, 1};

        حالت_2 = ippi_DFTGet_سائز_R_32f(src_Roi_سائز, norm_flag, ippAlgHintNone, &سائز_Spec, &سائز_Init, &سائز_بفر );
        اگر ( حالت_2 < 0 )
        {
            *ok = جھوٹ;
            واپس;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( سائز_Spec );

        اگر ( سائز_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( سائز_Init );

        اگر ( سائز_بفر > 0 )
            p_بفر = (Ipp8u*)CV_IPP_MALLOC( سائز_بفر );

        حالت_2 = ippiDFTInit_R_32f( src_Roi_سائز, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        اگر ( سائز_Init > 0 )
            ippFree( pMemInit );

        اگر ( حالت_2 < 0 )
        {
            ippFree( pDFTSpec );
            اگر ( سائز_بفر > 0 )
                ippFree( p_بفر );
            *ok = جھوٹ;
            واپس;
        }

        برائے( صحیح i = range.آغاز; i < range.end; ++i)
            اگر(!ippidft((اعشاری*)(src + src_قدم * i), src_قدم, (اعشاری*)(dst + dst_قدم * i), dst_قدم,
                        pDFTSpec, (Ipp8u*)p_بفر))
            {
                *ok = جھوٹ;
            }

        اگر ( سائز_بفر > 0 )
            ippFree( p_بفر );

        ippFree( pDFTSpec );
        CV_IMPL_جوڑو(CV_IMPL_IPP|CV_IMPL_MT);
    }

نجی:
    ثابت uchar * src;
    size_t src_قدم;
    uchar * dst;
    size_t dst_قدم;
    صحیح چوڑائی;
    ثابت Dft& ippidft;
    صحیح norm_flag;
    منطقی *ok;

    ثابت Dft_R_IPPLoop_Invoker& عامل= (ثابت Dft_R_IPPLoop_Invoker&);
};

سانچہ <قسم_نام Dft>
منطقی Dft_C_IPPLoop(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحیح چوڑائی, صحیح اونچائی, ثابت Dft& ippidft, صحیح norm_flag)
{
    منطقی ok;
    parallel_for_(Range(0, اونچائی), Dft_C_IPPLoop_Invoker<Dft>(src, src_قدم, dst, dst_قدم, چوڑائی, ippidft, norm_flag, &ok), (چوڑائی * اونچائی)/(دوہرا)(1<<16) );
    واپس ok;
}

سانچہ <قسم_نام Dft>
منطقی Dft_R_IPPLoop(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحیح چوڑائی, صحیح اونچائی, ثابت Dft& ippidft, صحیح norm_flag)
{
    منطقی ok;
    parallel_for_(Range(0, اونچائی), Dft_R_IPPLoop_Invoker<Dft>(src, src_قدم, dst, dst_قدم, چوڑائی, ippidft, norm_flag, &ok), (چوڑائی * اونچائی)/(دوہرا)(1<<16) );
    واپس ok;
}

ساخت IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    منطقی عامل()(ثابت Ipp32fc* src, size_t src_قدم_3, Ipp32fc* dst, size_t dst_قدم_3, ثابت IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* p_بفر) ثابت
    {
        واپس ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<صحیح>(src_قدم_3), dst, static_cast<صحیح>(dst_قدم_3), pDFTSpec, p_بفر) >= 0 : جھوٹ;
    }
نجی:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

ساخت IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippi_DFT_Pack_مقصد_R_32f_C1_R(_func){}

    منطقی عامل()(ثابت Ipp32f* src, size_t src_قدم_3, Ipp32f* dst, size_t dst_قدم_3, ثابت IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* p_بفر) ثابت
    {
        واپس ippi_DFT_Pack_مقصد_R_32f_C1_R ? CV_INSTRUMENT_FUN_IPP(ippi_DFT_Pack_مقصد_R_32f_C1_R, src, static_cast<صحیح>(src_قدم_3), dst, static_cast<صحیح>(dst_قدم_3), pDFTSpec, p_بفر) >= 0 : جھوٹ;
    }
نجی:
    ippiDFT_R_Func ippi_DFT_Pack_مقصد_R_32f_C1_R;
};

ساکن منطقی ippi_DFT_C_32F(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحیح چوڑائی, صحیح اونچائی, منطقی inv, صحیح norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_حالت حالت_2;
    Ipp8u* p_بفر = 0;
    Ipp8u* pMemInit= 0;
    صحیح سائز_بفر=0;
    صحیح سائز_Spec=0;
    صحیح سائز_Init=0;

    Ippi_سائز src_Roi_سائز = {چوڑائی, اونچائی};

    حالت_2 = ippi_DFTGet_سائز_C_32fc(src_Roi_سائز, norm_flag, ippAlgHintNone, &سائز_Spec, &سائز_Init, &سائز_بفر );
    اگر ( حالت_2 < 0 )
        واپس جھوٹ;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( سائز_Spec );

    اگر ( سائز_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( سائز_Init );

    اگر ( سائز_بفر > 0 )
        p_بفر = (Ipp8u*)CV_IPP_MALLOC( سائز_بفر );

    حالت_2 = ippiDFTInit_C_32fc( src_Roi_سائز, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    اگر ( سائز_Init > 0 )
        ippFree( pMemInit );

    اگر ( حالت_2 < 0 )
    {
        ippFree( pDFTSpec );
        اگر ( سائز_بفر > 0 )
            ippFree( p_بفر );
        واپس جھوٹ;
    }

    اگر (!inv)
        حالت_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<صحیح>(src_قدم), (Ipp32fc*)dst, static_cast<صحیح>(dst_قدم), pDFTSpec, p_بفر);
    ورنہ
        حالت_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<صحیح>(src_قدم), (Ipp32fc*)dst, static_cast<صحیح>(dst_قدم), pDFTSpec, p_بفر);

    اگر ( سائز_بفر > 0 )
        ippFree( p_بفر );

    ippFree( pDFTSpec );

    اگر(حالت_2 >= 0)
    {
        CV_IMPL_جوڑو(CV_IMPL_IPP);
        واپس سچ;
    }
    واپس جھوٹ;
}

ساکن منطقی ippi_DFT_R_32F(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحیح چوڑائی, صحیح اونچائی, منطقی inv, صحیح norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_حالت حالت_2;
    Ipp8u* p_بفر = 0;
    Ipp8u* pMemInit= 0;
    صحیح سائز_بفر=0;
    صحیح سائز_Spec=0;
    صحیح سائز_Init=0;

    Ippi_سائز src_Roi_سائز = {چوڑائی, اونچائی};

    حالت_2 = ippi_DFTGet_سائز_R_32f(src_Roi_سائز, norm_flag, ippAlgHintNone, &سائز_Spec, &سائز_Init, &سائز_بفر );
    اگر ( حالت_2 < 0 )
        واپس جھوٹ;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( سائز_Spec );

    اگر ( سائز_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( سائز_Init );

    اگر ( سائز_بفر > 0 )
        p_بفر = (Ipp8u*)CV_IPP_MALLOC( سائز_بفر );

    حالت_2 = ippiDFTInit_R_32f( src_Roi_سائز, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    اگر ( سائز_Init > 0 )
        ippFree( pMemInit );

    اگر ( حالت_2 < 0 )
    {
        ippFree( pDFTSpec );
        اگر ( سائز_بفر > 0 )
            ippFree( p_بفر );
        واپس جھوٹ;
    }

    اگر (!inv)
        حالت_2 = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (اعشاری*)src, static_cast<صحیح>(src_قدم), (اعشاری*)dst, static_cast<صحیح>(dst_قدم), pDFTSpec, p_بفر);
    ورنہ
        حالت_2 = CV_INSTRUMENT_FUN_IPP(ippi_DFTInv_Pack_مقصد_R_32f_C1_R, (اعشاری*)src, static_cast<صحیح>(src_قدم), (اعشاری*)dst, static_cast<صحیح>(dst_قدم), pDFTSpec, p_بفر);

    اگر ( سائز_بفر > 0 )
        ippFree( p_بفر );

    ippFree( pDFTSpec );

    اگر(حالت_2 >= 0)
    {
        CV_IMPL_جوڑو(CV_IMPL_IPP);
        واپس سچ;
    }
    واپس جھوٹ;
}

#اگر_ختم
}

#اگر_تعریف_شدہ HAVE_OPENCL

نامخلا cv
{

شمار FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

ساخت OCL_FftPlan
{
نجی:
    UMat twiddles;
    String buildOptions;
    صحیح thread_گنتی;
    صحیح dft_سائز;
    صحیح dft_گہرائی;
    منطقی حالت_2;

عوامی:
    OCL_FftPlan(صحیح سائز_4, صحیح گہرائی_2) : dft_سائز(سائز_4), dft_گہرائی(گہرائی_2), حالت_2(سچ)
    {
        CV_Assert( dft_گہرائی == CV_32F || dft_گہرائی == CV_64F );

        صحیح min_radix;
        std::vector<صحیح> radixes, بلاکیں;
        ocl_getRadixes(dft_سائز, radixes, بلاکیں, min_radix);
        thread_گنتی = dft_سائز / min_radix;

        اگر (thread_گنتی > (صحیح) ocl::Device::getDefault().max_Work_گروہ_سائز())
        {
            حالت_2 = جھوٹ;
            واپس;
        }

        // generate string with radix calls
        String radix_processing;
        صحیح n = 1, twiddle_سائز = 0;
        برائے (size_t i=0; i<radixes.size(); i++)
        {
            صحیح radix = radixes[i], بلاک_2 = بلاکیں[i];
            اگر (بلاک_2 > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, بلاک_2, twiddle_سائز, n, dft_سائز/radix);
            ورنہ
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_سائز, n, dft_سائز/radix);
            twiddle_سائز += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_سائز, CV_MAKE_TYPE(dft_گہرائی, 2));
        اگر (dft_گہرائی == CV_32F)
            fill_Radix_جدول<اعشاری>(twiddles, radixes);
        ورنہ
            fill_Radix_جدول<دوہرا>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_سائز, min_radix, ocl::type_مقصد_Str(dft_گہرائی), ocl::type_مقصد_Str(CV_MAKE_TYPE(dft_گہرائی, 2)),
                              dft_گہرائی == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    منطقی قطار_میں_ڈالو_Transform(داخلہ_Array _src, OutputArray _dst, صحیح عدد_dfts, صحیح flags, صحیح fftType, منطقی صفیں = سچ) ثابت
    {
        اگر (!حالت_2)
            واپس جھوٹ;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        منطقی is1d = (flags & DFT_صفیں) != 0 || عدد_dfts == 1;
        منطقی inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        اگر (صفیں)
        {
            globalsize[0] = thread_گنتی; globalsize[1] = src.صفیں;
            localsize[0] = thread_گنتی; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            اگر ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        ورنہ
        {
            globalsize[0] = عدد_dfts; globalsize[1] = thread_گنتی;
            localsize[0] = 1; localsize[1] = thread_گنتی;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            اگر (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        اگر (!inv)
        {
            اگر ((is1d && src.channels() == 1) || (صفیں && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        ورنہ
        {
            اگر (صفیں && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            اگر (dst.ستون_تعداد % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        اگر (k.empty())
            واپس جھوٹ;

        k.دلائل(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::Read_Only_No_سائز(twiddles), thread_گنتی, عدد_dfts);
        واپس k.تسلسل(2, globalsize, localsize, جھوٹ);
    }

نجی:
    ساکن خالی ocl_getRadixes(صحیح ستون_تعداد, std::vector<صحیح>& radixes, std::vector<صحیح>& بلاکیں, صحیح& min_radix)
    {
        صحیح factors[34];
        صحیح nf = DFTFactorize(ستون_تعداد, factors);

        صحیح n = 1;
        صحیح ضربی_عدد_اشاریہ = 0;
        min_radix = زیادہ_سے_زیادہ_عدد;

        // 2^n transforms
        اگر ((factors[ضربی_عدد_اشاریہ] & 1) == 0)
        {
            برائے( ; n < factors[ضربی_عدد_اشاریہ];)
            {
                صحیح radix = 2, بلاک_2 = 1;
                اگر (8*n <= factors[0])
                    radix = 8;
                ورنہ اگر (4*n <= factors[0])
                {
                    radix = 4;
                    اگر (ستون_تعداد % 12 == 0)
                        بلاک_2 = 3;
                    ورنہ اگر (ستون_تعداد % 8 == 0)
                        بلاک_2 = 2;
                }
                ورنہ
                {
                    اگر (ستون_تعداد % 10 == 0)
                        بلاک_2 = 5;
                    ورنہ اگر (ستون_تعداد % 8 == 0)
                        بلاک_2 = 4;
                    ورنہ اگر (ستون_تعداد % 6 == 0)
                        بلاک_2 = 3;
                    ورنہ اگر (ستون_تعداد % 4 == 0)
                        بلاک_2 = 2;
                }

                radixes.push_back(radix);
                بلاکیں.push_back(بلاک_2);
                min_radix = min(min_radix, بلاک_2*radix);
                n *= radix;
            }
            ضربی_عدد_اشاریہ++;
        }

        // all the other transforms
        برائے( ; ضربی_عدد_اشاریہ < nf; ضربی_عدد_اشاریہ++)
        {
            صحیح radix = factors[ضربی_عدد_اشاریہ], بلاک_2 = 1;
            اگر (radix == 3)
            {
                اگر (ستون_تعداد % 12 == 0)
                    بلاک_2 = 4;
                ورنہ اگر (ستون_تعداد % 9 == 0)
                    بلاک_2 = 3;
                ورنہ اگر (ستون_تعداد % 6 == 0)
                    بلاک_2 = 2;
            }
            ورنہ اگر (radix == 5)
            {
                اگر (ستون_تعداد % 10 == 0)
                    بلاک_2 = 2;
            }
            radixes.push_back(radix);
            بلاکیں.push_back(بلاک_2);
            min_radix = min(min_radix, بلاک_2*radix);
        }
    }

    سانچہ <قسم_نام T>
    ساکن خالی fill_Radix_جدول(UMat twiddles, ثابت std::vector<صحیح>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        صحیح ptr_اشاریہ = 0;

        صحیح n = 1;
        برائے (size_t i=0; i<radixes.size(); i++)
        {
            صحیح radix = radixes[i];
            n *= radix;

            برائے (صحیح j=1; j<radix; j++)
            {
                دوہرا theta = -CV_2PI*j/n;

                برائے (صحیح k=0; k<(n/radix); k++)
                {
                    ptr[ptr_اشاریہ++] = (T) جیب_تمام(k*theta);
                    ptr[ptr_اشاریہ++] = (T) جیب(k*theta);
                }
            }
        }
    }
};

جماعت OCL_FftPlanCache
{
عوامی:
    ساکن OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, نیا OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(صحیح dft_سائز, صحیح گہرائی)
    {
        صحیح کلید = (dft_سائز << 16) | (گہرائی & 0xFFFF);
        std::map<صحیح, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(کلید);
        اگر (f != planStorage.end())
        {
            واپس f->second;
        }
        ورنہ
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(نیا OCL_FftPlan(dft_سائز, گہرائی));
            planStorage[کلید] = newPlan;
            واپس newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

محفوظ:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<صحیح, Ptr<OCL_FftPlan> > planStorage;
};

ساکن منطقی ocl_dft_صفیں(داخلہ_Array _src, OutputArray _dst, صحیح nonzero_صفیں, صحیح flags, صحیح fftType)
{
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.ستون_تعداد(), گہرائی);
    واپس plan->قطار_میں_ڈالو_Transform(_src, _dst, nonzero_صفیں, flags, fftType, سچ);
}

ساکن منطقی ocl_dft_ستون_تعداد(داخلہ_Array _src, OutputArray _dst, صحیح nonzero_ستون_تعداد, صحیح flags, صحیح fftType)
{
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.صفیں(), گہرائی);
    واپس plan->قطار_میں_ڈالو_Transform(_src, _dst, nonzero_ستون_تعداد, flags, fftType, جھوٹ);
}

اندرونی FftType determineFFTType(منطقی real_داخلہ, منطقی complex_داخلہ, منطقی real_output, منطقی complex_output, منطقی inv)
{
    // output format is not specified
    اگر (!real_output && !complex_output)
        complex_output = سچ;

    // input or output format is ambiguous
    اگر (real_داخلہ == complex_داخلہ || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType نتیجہ = real_داخلہ ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    اگر (نتیجہ == C2R && !inv)
        نتیجہ = C2C;

    // Inverse CCS to Complex not supported
    اگر (نتیجہ == R2C && inv)
        نتیجہ = R2R;

    واپس نتیجہ;
}

ساکن منطقی ocl_dft(داخلہ_Array _src, OutputArray _dst, صحیح flags, صحیح nonzero_صفیں)
{
    صحیح type = _src.type(), cn = CV_MAT_CN(type), گہرائی = CV_MAT_گہرائی(type);
    سائز ssize = _src.size();
    منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    اگر (!(cn == 1 || cn == 2)
        || !(گہرائی == CV_32F || (گہرائی == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        واپس جھوٹ;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    اگر (ssize.area() != getOptimalDFTSize(ssize.area()))
        واپس جھوٹ;

    UMat src = _src.getUMat();
    منطقی inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    اگر( nonzero_صفیں <= 0 || nonzero_صفیں > _src.صفیں() )
        nonzero_صفیں = _src.صفیں();
    منطقی is1d = (flags & DFT_صفیں) != 0 || nonzero_صفیں == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    اگر (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.create_Same_سائز(src, CV_MAKETYPE(گہرائی, 2));
        output = _dst.getUMat();
    }
    ورنہ
    {
        // real output
        اگر (is1d)
        {
            _dst.create_Same_سائز(src, CV_MAKETYPE(گہرائی, 1));
            output = _dst.getUMat();
        }
        ورنہ
        {
            _dst.create_Same_سائز(src, CV_MAKETYPE(گہرائی, 1));
            output.create(src.size, CV_MAKETYPE(گہرائی, 2));
        }
    }

    منطقی نتیجہ = جھوٹ;
    اگر (!inv)
    {
        صحیح nonzero_ستون_تعداد = fftType == R2R ? output.ستون_تعداد/2 + 1 : output.ستون_تعداد;
        نتیجہ = ocl_dft_صفیں(src, output, nonzero_صفیں, flags, fftType);
        اگر (!is1d)
            نتیجہ = نتیجہ && ocl_dft_ستون_تعداد(output, _dst, nonzero_ستون_تعداد, flags, fftType);
    }
    ورنہ
    {
        اگر (fftType == C2C)
        {
            // complex output
            نتیجہ = ocl_dft_صفیں(src, output, nonzero_صفیں, flags, fftType);
            اگر (!is1d)
                نتیجہ = نتیجہ && ocl_dft_ستون_تعداد(output, output, output.ستون_تعداد, flags, fftType);
        }
        ورنہ
        {
            اگر (is1d)
            {
                نتیجہ = ocl_dft_صفیں(src, output, nonzero_صفیں, flags, fftType);
            }
            ورنہ
            {
                صحیح nonzero_ستون_تعداد = src.ستون_تعداد/2 + 1;
                نتیجہ = ocl_dft_ستون_تعداد(src, output, nonzero_ستون_تعداد, flags, fftType);
                نتیجہ = نتیجہ && ocl_dft_صفیں(output, _dst, nonzero_صفیں, flags, fftType);
            }
        }
    }
    واپس نتیجہ;
}

} // namespace cv;

#اگر_ختم

#اگر_تعریف_شدہ HAVE_CLAMDFFT

نامخلا cv {

#تعریف_کرو CLAMDDFT_Assert(func) \
    { \
        clfft_حالت s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

جماعت PlanCache
{
    ساخت FftPlan
    {
        FftPlan(ثابت سائز & dft_سائز_2, صحیح src_قدم_2, صحیح dst_قدم_2, منطقی _doubleFP, منطقی _inplace, صحیح _flags, FftType _fftType) :
            dft_سائز(dft_سائز_2), src_قدم(src_قدم_2), dst_قدم(dst_قدم_2),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            منطقی dft_inverse = (flags & DFT_INVERSE) != 0;
            منطقی dft_scale = (flags & DFT_SCALE) != 0;
            منطقی dft_صفیں = (flags & DFT_صفیں) != 0;

            clfftLayout اندر_Layout = CLFFT_REAL, برآمد_Layout = CLFFT_REAL;
            clfftDim dim = dft_سائز.اونچائی == 1 || dft_صفیں ? CLFFT_1D : CLFFT_2D;

            size_t batch_سائز = dft_صفیں ? dft_سائز.اونچائی : 1;
            size_t cl_Lengths_اندر[3] = { (size_t)dft_سائز.چوڑائی, dft_صفیں ? 1 : (size_t)dft_سائز.اونچائی, 1 };
            size_t cl_Strides_اندر[3] = { 1, 1, 1 };
            size_t cl_Strides_برآمد[3]  = { 1, 1, 1 };
            صحیح elem_سائز = doubleFP ? حجم(دوہرا) : حجم(اعشاری);

            انتخاب (fftType)
            {
            حالت C2C:
                اندر_Layout = CLFFT_COMPLEX_INTERLEAVED;
                برآمد_Layout = CLFFT_COMPLEX_INTERLEAVED;
                cl_Strides_اندر[1] = src_قدم / (elem_سائز << 1);
                cl_Strides_برآمد[1] = dst_قدم / (elem_سائز << 1);
                توڑو;
            حالت R2C:
                اندر_Layout = CLFFT_REAL;
                برآمد_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                cl_Strides_اندر[1] = src_قدم / elem_سائز;
                cl_Strides_برآمد[1] = dst_قدم / (elem_سائز << 1);
                توڑو;
            حالت C2R:
                اندر_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                برآمد_Layout = CLFFT_REAL;
                cl_Strides_اندر[1] = src_قدم / (elem_سائز << 1);
                cl_Strides_برآمد[1] = dst_قدم / elem_سائز;
                توڑو;
            حالت R2R:
            طےشدہ:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                توڑو;
            }

            cl_Strides_اندر[2] = dft_صفیں ? cl_Strides_اندر[1] : dft_سائز.چوڑائی * cl_Strides_اندر[1];
            cl_Strides_برآمد[2] = dft_صفیں ? cl_Strides_برآمد[1] : dft_سائز.چوڑائی * cl_Strides_برآمد[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, cl_Lengths_اندر))

            // setting plan properties
            CLAMDDFT_Assert(clfft_سیٹ_Plan_Precision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfft_سیٹ_نتیجہ_Location(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfft_سیٹ_Layout(plHandle, اندر_Layout, برآمد_Layout))
            CLAMDDFT_Assert(clfft_سیٹ_Plan_Batch_سائز(plHandle, batch_سائز))
            CLAMDDFT_Assert(clfft_سیٹ_Plan_اندر_Stride(plHandle, dim, cl_Strides_اندر))
            CLAMDDFT_Assert(clfft_سیٹ_Plan_برآمد_Stride(plHandle, dim, cl_Strides_برآمد))
            CLAMDDFT_Assert(clfft_سیٹ_Plan_فاصلہ(plHandle, cl_Strides_اندر[dim], cl_Strides_برآمد[dim]))

            اعشاری scale = dft_scale ? 1.0f / (dft_صفیں ? dft_سائز.چوڑائی : dft_سائز.area()) : 1.0f;
            CLAMDDFT_Assert(clfft_سیٹ_Plan_Scale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_قطار قطار_2 = (cl_command_قطار)ocl::قطار::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &قطار_2, تہی, تہی))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        دوست جماعت PlanCache;

    نجی:
        سائز dft_سائز;
        صحیح src_قدم, dst_قدم;
        منطقی doubleFP;
        منطقی inplace;
        صحیح flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

عوامی:
    ساکن PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, نیا PlanCache())
    }

    clfftPlanHandle getPlanHandle(ثابت سائز & dft_سائز, صحیح src_قدم, صحیح dst_قدم, منطقی doubleFP,
                                  منطقی inplace, صحیح flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        برائے (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            ثابت FftPlan * ثابت plan = planStorage[i];

            اگر (plan->dft_سائز == dft_سائز &&
                plan->flags == flags &&
                plan->src_قدم == src_قدم &&
                plan->dst_قدم == dst_قدم &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                اگر (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    توڑو;
                }

                واپس plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(نیا FftPlan(dft_سائز, src_قدم, dst_قدم, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        واپس newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

محفوظ:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

خارجی "C" {

ساکن خالی CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, خالی *p)
{
    UMat_ڈیٹا * u = (UMat_ڈیٹا *)p;

    اگر( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

ساکن منطقی ocl_dft_amdfft(داخلہ_Array _src, OutputArray _dst, صحیح flags)
{
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type);
    سائز ssize = _src.size();

    منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    اگر ( (!doubleSupport && گہرائی == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        واپس جھوٹ;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    اگر (ssize.area() != getOptimalDFTSize(ssize.area()))
        واپس جھوٹ;

    صحیح dst_complex_داخلہ = cn == 2 ? 1 : 0;
    منطقی dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    صحیح dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    منطقی dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_داخلہ << 0 | dft_complex_output << 1);

    انتخاب (fftType)
    {
    حالت C2C:
        _dst.create(ssize.اونچائی, ssize.چوڑائی, CV_MAKE_TYPE(گہرائی, 2));
        توڑو;
    حالت R2C: // TODO implement it if possible
    حالت C2R: // TODO implement it if possible
    حالت R2R: // AMD Fft does not support this type
    طےشدہ:
        واپس جھوٹ;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    اگر (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        واپس جھوٹ;
    منطقی inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (صحیح)src.قدم, (صحیح)dst.قدم,
                          گہرائی == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t بفر_سائز = 0;
    CLAMDDFT_Assert(clfft_Get_Tmp_Buf_سائز(plHandle, &بفر_سائز))
    UMat tmp_بفر(1, (صحیح)بفر_سائز, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_قطار قطار_2 = (cl_command_قطار)ocl::قطار::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfft_قطار_میں_ڈالو_Transform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &قطار_2, 0, تہی, &e,
                                          &srcarg, &dstarg, (cl_mem)tmp_بفر.handle(ACCESS_RW)))

    tmp_بفر.addref();
    cl_سیٹ_Event_Callback(e, CL_COMPLETE, oclCleanupCallback, tmp_بفر.u);
    واپس سچ;
}

#تعریف_منسوخ DFT_ASSERT

}

#اگر_ختم // HAVE_CLAMDFFT

نامخلا cv
{

سانچہ <قسم_نام T>
ساکن خالی complementComplex(T * ptr, size_t قدم, صحیح n, صحیح len, صحیح dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = قدم/حجم(p0[0]);
    برائے(صحیح i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        برائے( صحیح j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

ساکن خالی complementComplexOutput(صحیح گہرائی, uchar * ptr, size_t قدم, صحیح گنتی_2, صحیح len, صحیح dft_dims)
{
    اگر( گہرائی == CV_32F )
        complementComplex((اعشاری*)ptr, قدم, گنتی_2, len, dft_dims);
    ورنہ
        complementComplex((دوہرا*)ptr, قدم, گنتی_2, len, dft_dims);
}

شمار Dft_منوال {
    InvalidDft = 0,
    Fwd_Real_مقصد_CCS,
    Fwd_Real_مقصد_Complex,
    Fwd_Complex_مقصد_Complex,
    InvCCSToReal,
    Inv_Complex_مقصد_Real,
    Inv_Complex_مقصد_Complex,
};

شمار DftDims {
    InvalidDim = 0,
    OneDim,
    One_Dim_ستون_Wise,
    TwoDims
};

اندرونی ثابت حرف * منوال_Name(Dft_منوال m)
{
    انتخاب (m)
    {
    حالت InvalidDft: واپس "InvalidDft";
    حالت Fwd_Real_مقصد_CCS: واپس "FwdRealToCCS";
    حالت Fwd_Real_مقصد_Complex: واپس "FwdRealToComplex";
    حالت Fwd_Complex_مقصد_Complex: واپس "FwdComplexToComplex";
    حالت InvCCSToReal: واپس "InvCCSToReal";
    حالت Inv_Complex_مقصد_Real: واپس "InvComplexToReal";
    حالت Inv_Complex_مقصد_Complex: واپس "InvComplexToComplex";
    }
    واپس 0;
}

اندرونی ثابت حرف * dimsName(DftDims d)
{
    انتخاب (d)
    {
    حالت InvalidDim: واپس "InvalidDim";
    حالت OneDim: واپس "OneDim";
    حالت One_Dim_ستون_Wise: واپس "OneDimColWise";
    حالت TwoDims: واپس "TwoDims";
    };
    واپس 0;
}

سانچہ <قسم_نام T>
اندرونی منطقی isInv(T منوال)
{
    انتخاب ((Dft_منوال)منوال)
    {
        حالت InvCCSToReal:
        حالت Inv_Complex_مقصد_Real:
        حالت Inv_Complex_مقصد_Complex: واپس سچ;
        طےشدہ: واپس جھوٹ;
    }
}

اندرونی Dft_منوال determine_منوال(منطقی inv, صحیح cn1, صحیح cn2)
{
    اگر (!inv)
    {
        اگر (cn1 == 1 && cn2 == 1)
            واپس Fwd_Real_مقصد_CCS;
        ورنہ اگر (cn1 == 1 && cn2 == 2)
            واپس Fwd_Real_مقصد_Complex;
        ورنہ اگر (cn1 == 2 && cn2 == 2)
            واپس Fwd_Complex_مقصد_Complex;
    }
    ورنہ
    {
        اگر (cn1 == 1 && cn2 == 1)
            واپس InvCCSToReal;
        ورنہ اگر (cn1 == 2 && cn2 == 1)
            واپس Inv_Complex_مقصد_Real;
        ورنہ اگر (cn1 == 2 && cn2 == 2)
            واپس Inv_Complex_مقصد_Complex;
    }
    واپس InvalidDft;
}


اندرونی DftDims determineDims(صحیح صفیں, صحیح ستون_تعداد, منطقی is_صف_Wise, منطقی isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    اگر (is_صف_Wise)
        واپس OneDim;
    اگر (ستون_تعداد == 1 && صفیں > 1) // one-column-shaped input
    {
        اگر (isContinuous)
            واپس OneDim;
        ورنہ
            واپس One_Dim_ستون_Wise;
    }
    اگر (صفیں == 1)
        واپس OneDim;
    اگر (ستون_تعداد > 1 && صفیں > 1)
        واپس TwoDims;
    واپس InvalidDim;
}

جماعت OcvDftBasicImpl CV_FINAL : عوامی hal::DFT1D
{
عوامی:
    OcvDftOptions opt;
    // the plan is built once in init(), the workspace is allocated once here and reused by
    // every apply() (zero allocations per transform)
    DftPlan plan;
    Auto_بفر<uchar> ws;
#اگر_تعریف_شدہ USE_IPP_DFT
    Auto_بفر<uchar> ippbuf;
    Auto_بفر<uchar> ippworkbuf;
    size_t ipp_worksize;
#اگر_ختم

عوامی:
    OcvDftBasicImpl()
    {
#اگر_تعریف_شدہ USE_IPP_DFT
        ipp_worksize = 0;
#اگر_ختم
    }
    خالی init(صحیح len, صحیح گنتی_2, صحیح گہرائی, صحیح flags, منطقی *need_بفر)
    {
        صحیح stage = (flags & CV_HAL_DFT_STAGE_ستون_تعداد) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        منطقی real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        منطقی needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = جھوٹ;
    #اگر_تعریف_شدہ USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        اگر( CV_IPP_CHECK_COND && (opt.n*گنتی_2 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            صحیح ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            صحیح specsize=0, initsize=0, worksize=0;
            Ipp_DFTGet_سائز_Func get_سائز_Func = 0;
            IppDFTInitFunc initFunc = 0;

            اگر( real_transform && stage == 0 )
            {
                اگر( گہرائی == CV_32F )
                {
                    get_سائز_Func = ipps_DFTGet_سائز_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                ورنہ
                {
                    get_سائز_Func = ipps_DFTGet_سائز_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            ورنہ
            {
                اگر( گہرائی == CV_32F )
                {
                    get_سائز_Func = ipps_DFTGet_سائز_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                ورنہ
                {
                    get_سائز_Func = ipps_DFTGet_سائز_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            اگر( get_سائز_Func(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                اگر( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = سچ;
            }
            ورنہ
                سیٹ_Ipp_Error_حالت();
        }
    #اگر_ختم

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            ثابت DFTKernels& kernels = getDFTKernels(گہرائی);
            صحیح قسم = stage == 0 && real_transform ? (opt.isInverse ? DFT_قسم_C2_R : DFT_قسم_R2_C)
                                                    : DFT_قسم_C2_C;
            plan.build(قسم, len, گہرائی, kernels.vlanes);
            ws.allocate(plan.ws_بائٹیں);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.ڈیٹا();
            اگر (need_بفر)
                *need_بفر = جھوٹ;   // the engine supports in-place for every kind
        }

        {
            ساکن DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<اعشاری>>,
                dftWrap<اعشاری, RealDFT<اعشاری>>,
                dftWrap<اعشاری, CCSIDFT<اعشاری>>,
                dftWrap<Complexd, DFT<دوہرا>>,
                dftWrap<دوہرا, RealDFT<دوہرا>>,
                dftWrap<دوہرا, CCSIDFT<دوہرا>>
            };
            صحیح idx = 0;
            اگر (stage == 0)
            {
                اگر (real_transform)
                {
                    اگر (!opt.isInverse)
                        idx = 1;
                    ورنہ
                        idx = 2;
                }
            }
            اگر (گہرائی == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        اگر(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            صحیح صف_گنتی = گنتی_2;
            اگر (stage == 0 && (flags & CV_HAL_DFT_صفیں) != 0)
                صف_گنتی = 1;
            opt.scale = 1./(len * صف_گنتی);
        }
    }

    خالی apply(ثابت uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspace_سائز() ثابت
    {
        size_t sz = opt.plan ? plan.ws_بائٹیں : 0;
#اگر_تعریف_شدہ USE_IPP_DFT
        اگر (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#اگر_ختم
        واپس sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    خالی applyWithWorkspace(ثابت uchar *src, uchar *dst, uchar* workspace) ثابت
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#اگر_تعریف_شدہ USE_IPP_DFT
        اگر (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#اگر_ختم
        opt_.dft_func(opt_, src, dst);
    }

    خالی آزاد() {}
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
ساکن ثابت صحیح DFT_ستون_تعداد_BATCH_MIN = 4;
ساکن ثابت صحیح DFT_ستون_تعداد_BATCH_MAX = 16;
ساکن ثابت size_t DFT_ستون_تعداد_BATCH_بائٹیں = 1 << 17;

ساکن صحیح dft_ستون_تعداد_Batch(صحیح len, size_t complex_elem_سائز)
{
    صحیح batch = (صحیح)(DFT_ستون_تعداد_BATCH_بائٹیں/((size_t)len*complex_elem_سائز));
    واپس std::max(DFT_ستون_تعداد_BATCH_MIN, std::min(DFT_ستون_تعداد_BATCH_MAX, batch));
}

ساکن دوہرا dftParallelStripes(صحیح گنتی_2, صحیح len)
{
    ثابت دوہرا وقتی_حصہ = 0.5e6;
    واپس (دوہرا)گنتی_2*len*std::max(std::لوگارتھم((دوہرا)len)/std::لوگارتھم(2.), 1.)/وقتی_حصہ;
}

جماعت OcvDftImpl CV_FINAL : عوامی hal::DFT2D
{
محفوظ:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    ثابت OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    ثابت OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    منطقی need_بفر_A;
    منطقی need_بفر_B;
    منطقی inv;
    صحیح چوڑائی;
    صحیح اونچائی;
    Dft_منوال منوال;
    صحیح elem_سائز_2;
    صحیح complex_elem_سائز;
    صحیح گہرائی;
    منطقی real_transform;
    صحیح nonzero_صفیں;
    منطقی is_صف_Transform;
    منطقی isScaled;
    std::vector<صحیح> stages;
    منطقی useIpp;
    صحیح src_channels;
    صحیح dst_channels;

    Auto_بفر<uchar> tmp_bufA;
    Auto_بفر<uchar> tmp_bufB;
    Auto_بفر<uchar> buf0;
    Auto_بفر<uchar> buf1;

عوامی:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        need_بفر_A = جھوٹ;
        need_بفر_B = جھوٹ;
        inv = جھوٹ;
        چوڑائی = 0;
        اونچائی = 0;
        منوال = InvalidDft;
        elem_سائز_2 = 0;
        complex_elem_سائز = 0;
        گہرائی = 0;
        real_transform = جھوٹ;
        nonzero_صفیں = 0;
        is_صف_Transform = جھوٹ;
        isScaled = جھوٹ;
        useIpp = جھوٹ;
        src_channels = 0;
        dst_channels = 0;
    }

    خالی init(صحیح چوڑائی_3, صحیح اونچائی_3, صحیح گہرائی_2, صحیح _src_channels, صحیح _dst_channels, صحیح flags, صحیح nonzero_صفیں_2)
    {
        منطقی isComplex = _src_channels != _dst_channels;
        nonzero_صفیں = nonzero_صفیں_2;
        چوڑائی = چوڑائی_3;
        اونچائی = اونچائی_3;
        گہرائی = گہرائی_2;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        منطقی isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        منطقی isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        منطقی isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        منوال = determine_منوال(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        is_صف_Transform = (flags & CV_HAL_DFT_صفیں) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        need_بفر_A = جھوٹ;
        need_بفر_B = جھوٹ;
        real_transform = (منوال != Fwd_Complex_مقصد_Complex && منوال != Inv_Complex_مقصد_Complex);

        elem_سائز_2 = (گہرائی == CV_32F) ? حجم(اعشاری) : حجم(دوہرا);
        complex_elem_سائز = elem_سائز_2 * 2;
        اگر( !real_transform )
            elem_سائز_2 = complex_elem_سائز;

#اگر defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            اگر (nonzero_صفیں == 0 && گہرائی == CV_32F && ((چوڑائی * اونچائی)>(صحیح)(1<<6)))
            {
                اگر (منوال == Fwd_Complex_مقصد_Complex || منوال == Inv_Complex_مقصد_Complex || منوال == Fwd_Real_مقصد_CCS || منوال == InvCCSToReal)
                {
                    useIpp = سچ;
                    واپس;
                }
            }
        }
#اگر_ختم

        DftDims dims = determineDims(اونچائی, چوڑائی, is_صف_Transform, isContinuous);
        اگر (dims == TwoDims)
        {
            stages.resize(2);
            اگر (منوال == InvCCSToReal || منوال == Inv_Complex_مقصد_Real)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            ورنہ
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        ورنہ
        {
            stages.resize(1);
            اگر (dims == One_Dim_ستون_Wise)
                stages[0] = 1;
            ورنہ
                stages[0] = 0;
        }

        برائے(uint stage_اشاریہ = 0; stage_اشاریہ < stages.size(); ++stage_اشاریہ)
        {
            اگر (stage_اشاریہ == 1)
            {
                isInplace = سچ;
                isComplex = جھوٹ;
            }

            صحیح stage = stages[stage_اشاریہ];
            منطقی is_آخری_Stage = (stage_اشاریہ + 1 == stages.size());

            صحیح len, گنتی_2;

            صحیح f = 0;
            اگر (inv)
                f |= CV_HAL_DFT_INVERSE;
            اگر (isScaled)
                f |= CV_HAL_DFT_SCALE;
            اگر (is_صف_Transform)
                f |= CV_HAL_DFT_صفیں;
            اگر (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            اگر (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            اگر (!is_آخری_Stage)
                f |= CV_HAL_DFT_TWO_STAGE;

            اگر( stage == 0 ) // row-wise transform
            {
                اگر (چوڑائی == 1 && !is_صف_Transform )
                {
                    len = اونچائی;
                    گنتی_2 = چوڑائی;
                }
                ورنہ
                {
                    len = چوڑائی;
                    گنتی_2 = اونچائی;
                }
                need_بفر_A = isInplace;
                contextA = hal::DFT1D::create(len, گنتی_2, گہرائی, f, &need_بفر_A);
                basicA = dynamic_cast<ثابت OcvDftBasicImpl*>(contextA.get());
                اگر (need_بفر_A)
                    tmp_bufA.allocate(len * complex_elem_سائز);
            }
            ورنہ
            {
                len = اونچائی;
                گنتی_2 = چوڑائی;
                f |= CV_HAL_DFT_STAGE_ستون_تعداد;
                need_بفر_B = isInplace;
                contextB = hal::DFT1D::create(len, گنتی_2, گہرائی, f, &need_بفر_B);
                basicB = dynamic_cast<ثابت OcvDftBasicImpl*>(contextB.get());
                اگر (need_بفر_B)
                    tmp_bufB.allocate(len * complex_elem_سائز);

                buf0.allocate(len * complex_elem_سائز);
                buf1.allocate(len * complex_elem_سائز);
            }
        }
    }

    خالی apply(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم) CV_OVERRIDE
    {
#اگر defined USE_IPP_DFT
        اگر (useIpp)
        {
            صحیح ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            اگر (!is_صف_Transform)
            {
                اگر (منوال == Fwd_Complex_مقصد_Complex || منوال == Inv_Complex_مقصد_Complex)
                {
                    اگر (ippi_DFT_C_32F(src, src_قدم, dst, dst_قدم, چوڑائی, اونچائی, inv, ipp_norm_flag))
                    {
                        CV_IMPL_جوڑو(CV_IMPL_IPP);
                        واپس;
                    }
                    سیٹ_Ipp_Error_حالت();
                }
                ورنہ اگر (منوال == Fwd_Real_مقصد_CCS || منوال == InvCCSToReal)
                {
                    اگر (ippi_DFT_R_32F(src, src_قدم, dst, dst_قدم, چوڑائی, اونچائی, inv, ipp_norm_flag))
                    {
                        CV_IMPL_جوڑو(CV_IMPL_IPP);
                        واپس;
                    }
                    سیٹ_Ipp_Error_حالت();
                }
            }
            ورنہ
            {
                اگر (منوال == Fwd_Complex_مقصد_Complex || منوال == Inv_Complex_مقصد_Complex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    اگر (Dft_C_IPPLoop(src, src_قدم, dst, dst_قدم, چوڑائی, اونچائی, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_جوڑو(CV_IMPL_IPP|CV_IMPL_MT);
                        واپس;
                    }
                    سیٹ_Ipp_Error_حالت();
                }
                ورنہ اگر (منوال == Fwd_Real_مقصد_CCS || منوال == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippi_DFTInv_Pack_مقصد_R_32f_C1_R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    اگر (Dft_R_IPPLoop(src, src_قدم, dst, dst_قدم, چوڑائی, اونچائی, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_جوڑو(CV_IMPL_IPP|CV_IMPL_MT);
                        واپس;
                    }
                    سیٹ_Ipp_Error_حالت();
                }
            }
            واپس;
        }
#اگر_ختم

        برائے(uint stage_اشاریہ = 0; stage_اشاریہ < stages.size(); ++stage_اشاریہ)
        {
            صحیح stage_src_channels = src_channels;
            صحیح stage_dst_channels = dst_channels;

            اگر (stage_اشاریہ == 1)
            {
                src = dst;
                src_قدم = dst_قدم;
                stage_src_channels = stage_dst_channels;
            }

            صحیح stage = stages[stage_اشاریہ];
            منطقی is_آخری_Stage = (stage_اشاریہ + 1 == stages.size());
            منطقی isComplex = stage_src_channels != stage_dst_channels;

            اگر( stage == 0 )
                صف_Dft(src, src_قدم, dst, dst_قدم, isComplex, is_آخری_Stage);
            ورنہ
                ستون_Dft(src, src_قدم, dst, dst_قدم, stage_src_channels, stage_dst_channels, is_آخری_Stage);
        }
    }

محفوظ:

    خالی صف_Dft(ثابت uchar* src_ڈیٹا, size_t src_قدم, uchar* dst_ڈیٹا, size_t dst_قدم, منطقی isComplex, منطقی is_آخری_Stage)
    {
        صحیح len, گنتی_2;
        اگر (چوڑائی == 1 && !is_صف_Transform )
        {
            len = اونچائی;
            گنتی_2 = چوڑائی;
        }
        ورنہ
        {
            len = چوڑائی;
            گنتی_2 = اونچائی;
        }
        صحیح dptr_انحراف = 0;
        صحیح dst_بھرا_len = len*elem_سائز_2;

        اگر( need_بفر_A )
        {
            اگر (منوال == Fwd_Real_مقصد_CCS && (len & 1) && len > 1)
                dptr_انحراف = elem_سائز_2;
        }

        اگر( !inv && isComplex )
            dst_بھرا_len += (len & 1) ? elem_سائز_2 : complex_elem_سائز;

        صحیح nz = nonzero_صفیں;
        اگر( nz <= 0 || nz > گنتی_2 )
            nz = گنتی_2;

        size_t ws_سائز = basicA ? basicA->workspace_سائز() : 0;
        دوہرا nstripes = dftParallelStripes(nz, len);
        اگر( ws_سائز > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            ثابت OcvDftBasicImpl* ctx = basicA;
            خودکار process_صفیں = [&, ctx](ثابت Range& range)
            {
                Auto_بفر<uchar> ws(ws_سائز);
                Auto_بفر<uchar> rowbuf(need_بفر_A ? len * complex_elem_سائز : 0);
                برائے( صحیح i = range.آغاز; i < range.end; i++ )
                {
                    ثابت uchar* sptr = src_ڈیٹا + src_قدم * i;
                    uchar* dptr0 = dst_ڈیٹا + dst_قدم * i;
                    uchar* dptr = need_بفر_A ? rowbuf.ڈیٹا() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.ڈیٹا());
                    اگر( need_بفر_A )
                        حافظہ_نقل( dptr0, dptr + dptr_انحراف, dst_بھرا_len );
                }
            };
            parallel_for_(Range(0, nz), process_صفیں, nstripes);
        }
        ورنہ
        {
            برائے( صحیح i = 0; i < nz; i++ )
            {
                ثابت uchar* sptr = src_ڈیٹا + src_قدم * i;
                uchar* dptr0 = dst_ڈیٹا + dst_قدم * i;
                uchar* dptr = dptr0;

                اگر( need_بفر_A )
                    dptr = tmp_bufA.ڈیٹا();

                contextA->apply(sptr, dptr);

                اگر( need_بفر_A )
                    حافظہ_نقل( dptr0, dptr + dptr_انحراف, dst_بھرا_len );
            }
        }

        برائے( صحیح i = nz; i < گنتی_2; i++ )
        {
            uchar* dptr0 = dst_ڈیٹا + dst_قدم * i;
            memset( dptr0, 0, dst_بھرا_len );
        }
        اگر(is_آخری_Stage &&  منوال == Fwd_Real_مقصد_Complex)
            complementComplexOutput(گہرائی, dst_ڈیٹا, dst_قدم, len, nz, 1);
    }

    خالی ستون_Dft(ثابت uchar* src_ڈیٹا, size_t src_قدم, uchar* dst_ڈیٹا, size_t dst_قدم, صحیح stage_src_channels, صحیح stage_dst_channels, منطقی is_آخری_Stage)
    {
        صحیح len = اونچائی;
        صحیح گنتی_2 = چوڑائی;
        صحیح a = 0, b = گنتی_2;
        uchar *dbuf0, *dbuf1;
        ثابت uchar* sptr0 = src_ڈیٹا;
        uchar* dptr0 = dst_ڈیٹا;

        dbuf0 = buf0.ڈیٹا(), dbuf1 = buf1.ڈیٹا();

        اگر( need_بفر_B )
        {
            dbuf1 = tmp_bufB.ڈیٹا();
            dbuf0 = buf1.ڈیٹا();
        }

        اگر( real_transform )
        {
            صحیح even;
            a = 1;
            even = (گنتی_2 & 1) == 0;
            b = (گنتی_2+1)/2;
            اگر( !inv )
            {
                memset( buf0.ڈیٹا(), 0, len*complex_elem_سائز );
                Copy_عمود( sptr0, src_قدم, buf0.ڈیٹا(), complex_elem_سائز, len, elem_سائز_2 );
                sptr0 += stage_dst_channels*elem_سائز_2;
                اگر( even )
                {
                    memset( buf1.ڈیٹا(), 0, len*complex_elem_سائز );
                    Copy_عمود( sptr0 + (گنتی_2-2)*elem_سائز_2, src_قدم,
                                buf1.ڈیٹا(), complex_elem_سائز, len, elem_سائز_2 );
                }
            }
            ورنہ اگر( stage_src_channels == 1 )
            {
                Copy_عمود( sptr0, src_قدم, buf0.ڈیٹا(), elem_سائز_2, len, elem_سائز_2 );
                ExpandCCS( buf0.ڈیٹا(), len, elem_سائز_2 );
                اگر( even )
                {
                    Copy_عمود( sptr0 + (گنتی_2-1)*elem_سائز_2, src_قدم,
                                buf1.ڈیٹا(), elem_سائز_2, len, elem_سائز_2 );
                    ExpandCCS( buf1.ڈیٹا(), len, elem_سائز_2 );
                }
                sptr0 += elem_سائز_2;
            }
            ورنہ
            {
                Copy_عمود( sptr0, src_قدم, buf0.ڈیٹا(), complex_elem_سائز, len, complex_elem_سائز );
                اگر( even )
                {
                    Copy_عمود( sptr0 + b*complex_elem_سائز, src_قدم,
                                   buf1.ڈیٹا(), complex_elem_سائز, len, complex_elem_سائز );
                }
                sptr0 += complex_elem_سائز;
            }

            اگر( even )
                contextB->apply(buf1.ڈیٹا(), dbuf1);
            contextB->apply(buf0.ڈیٹا(), dbuf0);

            اگر( stage_dst_channels == 1 )
            {
                اگر( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    حافظہ_نقل( dbuf0 + elem_سائز_2, dbuf0, elem_سائز_2 );
                    Copy_عمود( dbuf0 + elem_سائز_2, elem_سائز_2, dptr0,
                                   dst_قدم, len, elem_سائز_2 );
                    اگر( even )
                    {
                        حافظہ_نقل( dbuf1 + elem_سائز_2, dbuf1, elem_سائز_2 );
                        Copy_عمود( dbuf1 + elem_سائز_2, elem_سائز_2,
                                       dptr0 + (گنتی_2-1)*elem_سائز_2,
                                       dst_قدم, len, elem_سائز_2 );
                    }
                    dptr0 += elem_سائز_2;
                }
                ورنہ
                {
                    // copy the real part of the complex vector to the first/last column
                    Copy_عمود( dbuf0, complex_elem_سائز, dptr0, dst_قدم, len, elem_سائز_2 );
                    اگر( even )
                        Copy_عمود( dbuf1, complex_elem_سائز, dptr0 + (گنتی_2-1)*elem_سائز_2,
                                       dst_قدم, len, elem_سائز_2 );
                    dptr0 += elem_سائز_2;
                }
            }
            ورنہ
            {
                CV_Assert( !inv );
                Copy_عمود( dbuf0, complex_elem_سائز, dptr0,
                               dst_قدم, len, complex_elem_سائز );
                اگر( even )
                    Copy_عمود( dbuf1, complex_elem_سائز,
                                   dptr0 + b*complex_elem_سائز,
                                   dst_قدم, len, complex_elem_سائز );
                dptr0 += complex_elem_سائز;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        صحیح ncols = b - a;
        ثابت صحیح ستون_تعداد_batch = dft_ستون_تعداد_Batch(len, complex_elem_سائز);
        صحیح nbatches = (ncols + ستون_تعداد_batch - 1)/ستون_تعداد_batch;
        size_t ws_سائز = basicB ? basicB->workspace_سائز() : 0;
        size_t colbuf_سائز = (size_t)len*complex_elem_سائز;
        ثابت OcvDftBasicImpl* ctx = basicB;
        خودکار processBatches = [&, ctx](ثابت Range& range)
        {
            Auto_بفر<uchar> ws(ws_سائز);
            Auto_بفر<uchar> cbuf(colbuf_سائز*ستون_تعداد_batch*(need_بفر_B ? 2 : 1));
            uchar* cbuf0 = cbuf.ڈیٹا();
            uchar* cdbuf0 = need_بفر_B ? cbuf0 + colbuf_سائز*ستون_تعداد_batch : cbuf0;
            برائے( صحیح bi = range.آغاز; bi < range.end; bi++ )
            {
                صحیح c0 = bi*ستون_تعداد_batch;
                صحیح nc = std::min(ستون_تعداد_batch, ncols - c0);
                ثابت uchar* sptr = sptr0 + (size_t)c0*complex_elem_سائز;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_سائز;
                CopyFromColumns( sptr, src_قدم, cbuf0, nc, len, complex_elem_سائز );
                برائے( صحیح k = 0; k < nc; k++ )
                {
                    اگر( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_سائز, cdbuf0 + k*colbuf_سائز, ws.ڈیٹا());
                    ورنہ
                        contextB->apply(cbuf0 + k*colbuf_سائز, cdbuf0 + k*colbuf_سائز);
                }
                Copy_مقصد_Columns( cdbuf0, dptr, dst_قدم, nc, len, complex_elem_سائز );
            }
        };
        دوہرا nstripes = dftParallelStripes(ncols, len);
        اگر( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        ورنہ
            processBatches(Range(0, nbatches));
        اگر(is_آخری_Stage && منوال == Fwd_Real_مقصد_Complex)
            complementComplexOutput(گہرائی, dst_ڈیٹا, dst_قدم, گنتی_2, len, 2);
    }
};

ساخت ReplacementDFT1D : عوامی hal::DFT1D
{
    cvhalDFT *context;
    منطقی isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(جھوٹ) {}
    منطقی init(صحیح len, صحیح گنتی_2, صحیح گہرائی, صحیح flags, منطقی *need_بفر)
    {
        صحیح res = cv_hal_dftInit1D(&context, len, گنتی_2, گہرائی, flags, need_بفر);
        isInitialized = (res == CV_HAL_ERROR_OK);
        واپس isInitialized;
    }
    خالی apply(ثابت uchar *src, uchar *dst) CV_OVERRIDE
    {
        اگر (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        اگر (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

ساخت ReplacementDFT2D : عوامی hal::DFT2D
{
    cvhalDFT *context;
    منطقی isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(جھوٹ) {}
    منطقی init(صحیح چوڑائی, صحیح اونچائی, صحیح گہرائی,
              صحیح src_channels, صحیح dst_channels,
              صحیح flags, صحیح nonzero_صفیں)
    {
        صحیح res = cv_hal_dftInit2D(&context, چوڑائی, اونچائی, گہرائی, src_channels, dst_channels, flags, nonzero_صفیں);
        isInitialized = (res == CV_HAL_ERROR_OK);
        واپس isInitialized;
    }
    خالی apply(ثابت uchar *src, size_t src_قدم, uchar *dst, size_t dst_قدم) CV_OVERRIDE
    {
        اگر (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_قدم, dst, dst_قدم);
        }
    }
    ~ReplacementDFT2D()
    {
        اگر (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

نامخلا hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(صحیح len, صحیح گنتی_2, صحیح گہرائی, صحیح flags, منطقی *need_بفر)
{
    {
        ReplacementDFT1D *impl = نیا ReplacementDFT1D();
        اگر (impl->init(len, گنتی_2, گہرائی, flags, need_بفر))
        {
            واپس Ptr<DFT1D>(impl);
        }
        حذف impl;
    }
    {
        OcvDftBasicImpl *impl = نیا OcvDftBasicImpl();
        impl->init(len, گنتی_2, گہرائی, flags, need_بفر);
        واپس Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(صحیح چوڑائی, صحیح اونچائی, صحیح گہرائی,
                         صحیح src_channels, صحیح dst_channels,
                         صحیح flags, صحیح nonzero_صفیں)
{
    {
        ReplacementDFT2D *impl = نیا ReplacementDFT2D();
        اگر (impl->init(چوڑائی, اونچائی, گہرائی, src_channels, dst_channels, flags, nonzero_صفیں))
        {
            واپس Ptr<DFT2D>(impl);
        }
        حذف impl;
    }
    {
        اگر(چوڑائی == 1 && nonzero_صفیں > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = نیا OcvDftImpl();
        impl->init(چوڑائی, اونچائی, گہرائی, src_channels, dst_channels, flags, nonzero_صفیں);
        واپس Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


خالی cv::dft( داخلہ_Array _src0, OutputArray _dst, صحیح flags, صحیح nonzero_صفیں )
{
    CV_INSTRUMENT_REGION();

#اگر_تعریف_شدہ HAVE_CLAMDFFT
    CV_OCL_تسلسل(ocl::haveAmdFft() && ocl::Device::getDefault().type() != ocl::Device::TYPE_CPU &&
            _dst.isUMat() && _src0.dims() <= 2 && nonzero_صفیں == 0,
               ocl_dft_amdfft(_src0, _dst, flags))
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENCL
    CV_OCL_تسلسل(_dst.isUMat() && _src0.dims() <= 2,
               ocl_dft(_src0, _dst, flags, nonzero_صفیں))
#اگر_ختم

    Mat src0 = _src0.getMat(), src = src0;
    اگر (src.empty())
    {
        _dst.release();
        واپس;
    }
    منطقی inv = (flags & DFT_INVERSE) != 0;
    صحیح type = src.type();
    صحیح گہرائی = src.گہرائی();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_داخلہ) && src.channels() != 2) );

    اگر( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.create_Same_سائز( src, CV_MAKETYPE(گہرائی, 2) );
    ورنہ اگر( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.create_Same_سائز( src, گہرائی );
    ورنہ
        _dst.create_Same_سائز( src, type );

    Mat dst = _dst.getMat();

    صحیح f = 0;
    اگر (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    اگر (inv)
        f |= CV_HAL_DFT_INVERSE;
    اگر (flags & DFT_صفیں)
        f |= CV_HAL_DFT_صفیں;
    اگر (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    اگر (src.ڈیٹا == dst.ڈیٹا)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.ستون_تعداد, src.صفیں, گہرائی, src.channels(), dst.channels(), f, nonzero_صفیں);
    c->apply(src.ڈیٹا, src.قدم, dst.ڈیٹا, dst.قدم);
}


خالی cv::idft( داخلہ_Array src, OutputArray dst, صحیح flags, صحیح nonzero_صفیں )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_صفیں );
}

#اگر_تعریف_شدہ HAVE_OPENCL

نامخلا cv {

ساکن منطقی ocl_mulSpectrums( داخلہ_Array _srcA, داخلہ_Array _srcB,
                              OutputArray _dst, صحیح flags, منطقی conjB )
{
    صحیح atype = _srcA.type(), btype = _srcB.type(),
            صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    سائز asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    اگر ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        واپس جھوٹ;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.create_Same_سائز(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    k.دلائل(ocl::KernelArg::Read_Only_No_سائز(A), ocl::KernelArg::Read_Only_No_سائز(B),
           ocl::KernelArg::WriteOnly(dst), صفیں_فی_WI);

    size_t globalsize[2] = { (size_t)asize.چوڑائی, ((size_t)asize.اونچائی + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

}

#اگر_ختم

نامخلا {

#تعریف_کرو VAL(buf, elem) (((T*)((حرف*)ڈیٹا ## buf + (قدم ## buf * (elem))))[0])
#تعریف_کرو MUL_SPECTRUMS_ستون(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    برائے (size_t j = 1; j <= صفیں - 2; j += 2) \
    { \
        دوہرا a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        دوہرا b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        اگر (conjB) b_im = -b_im; \
        دوہرا c_re = a_re * b_re - a_im * b_im; \
        دوہرا c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    اگر ((صفیں & 1) == 0) \
        VAL(C, صفیں-1) = VAL(A, صفیں-1) * VAL(B, صفیں-1)

سانچہ <قسم_نام T, منطقی conjB> ساکن اندرونی
خالی mul_Spectrums_process_ستون_noinplace(ثابت T* ڈیٹا_A, ثابت T* ڈیٹا_B, T* ڈیٹا_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفیں)
{
    MUL_SPECTRUMS_ستون(A, B, C);
}

سانچہ <قسم_نام T, منطقی conjB> ساکن اندرونی
خالی mul_Spectrums_process_ستون_inplace_A(ثابت T* ڈیٹا_B, T* ڈیٹا_AC, size_t قدم_B, size_t قدم_AC, size_t صفیں)
{
    MUL_SPECTRUMS_ستون(AC, B, AC);
}
سانچہ <قسم_نام T, منطقی conjB, منطقی inplaceA> ساکن اندرونی
خالی mul_Spectrums_process_ستون(ثابت T* ڈیٹا_A, ثابت T* ڈیٹا_B, T* ڈیٹا_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفیں)
{
    اگر (inplaceA)
        mul_Spectrums_process_ستون_inplace_A<T, conjB>(ڈیٹا_B, ڈیٹا_C, قدم_B, قدم_C, صفیں);
    ورنہ
        mul_Spectrums_process_ستون_noinplace<T, conjB>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, قدم_A, قدم_B, قدم_C, صفیں);
}
#تعریف_منسوخ MUL_SPECTRUMS_ستون
#تعریف_منسوخ VAL

سانچہ <قسم_نام T, منطقی conjB, منطقی inplaceA> ساکن اندرونی
خالی mul_Spectrums_process_ستون_تعداد(ثابت T* ڈیٹا_A, ثابت T* ڈیٹا_B, T* ڈیٹا_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفیں, size_t ستون_تعداد)
{
    mul_Spectrums_process_ستون<T, conjB, inplaceA>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, قدم_A, قدم_B, قدم_C, صفیں);
    اگر ((ستون_تعداد & 1) == 0)
    {
        mul_Spectrums_process_ستون<T, conjB, inplaceA>(ڈیٹا_A + ستون_تعداد - 1, ڈیٹا_B + ستون_تعداد - 1, ڈیٹا_C + ستون_تعداد - 1, قدم_A, قدم_B, قدم_C, صفیں);
    }
}

#تعریف_کرو VAL(buf, elem) (ڈیٹا ## buf[(elem)])
#تعریف_کرو MUL_SPECTRUMS_صف(A, B, C) \
    برائے (size_t j = j0; j < j1; j += 2) \
    { \
        دوہرا a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        دوہرا b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        اگر (conjB) b_im = -b_im; \
        دوہرا c_re = a_re * b_re - a_im * b_im; \
        دوہرا c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
سانچہ <قسم_نام T, منطقی conjB> ساکن اندرونی
خالی mul_Spectrums_process_صف_noinplace(ثابت T* ڈیٹا_A, ثابت T* ڈیٹا_B, T* ڈیٹا_C, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_صف(A, B, C);
}
سانچہ <قسم_نام T, منطقی conjB> ساکن اندرونی
خالی mul_Spectrums_process_صف_inplace_A(ثابت T* ڈیٹا_B, T* ڈیٹا_AC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_صف(AC, B, AC);
}
سانچہ <قسم_نام T, منطقی conjB, منطقی inplaceA> ساکن اندرونی
خالی mul_Spectrums_process_صف(ثابت T* ڈیٹا_A, ثابت T* ڈیٹا_B, T* ڈیٹا_C, size_t j0, size_t j1)
{
    اگر (inplaceA)
        mul_Spectrums_process_صف_inplace_A<T, conjB>(ڈیٹا_B, ڈیٹا_C, j0, j1);
    ورنہ
        mul_Spectrums_process_صف_noinplace<T, conjB>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, j0, j1);
}
#تعریف_منسوخ MUL_SPECTRUMS_صف
#تعریف_منسوخ VAL

سانچہ <قسم_نام T, منطقی conjB, منطقی inplaceA> ساکن اندرونی
خالی mul_Spectrums_process_صفیں(ثابت T* ڈیٹا_A, ثابت T* ڈیٹا_B, T* ڈیٹا_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفیں, size_t ستون_تعداد, size_t j0, size_t j1, منطقی is_1d_CN1)
{
    جب (صفیں-- > 0)
    {
        اگر (is_1d_CN1)
            ڈیٹا_C[0] = ڈیٹا_A[0]*ڈیٹا_B[0];
        mul_Spectrums_process_صف<T, conjB, inplaceA>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, j0, j1);
        اگر (is_1d_CN1 && (ستون_تعداد & 1) == 0)
            ڈیٹا_C[j1] = ڈیٹا_A[j1]*ڈیٹا_B[j1];

        ڈیٹا_A = (ثابت T*)(((حرف*)ڈیٹا_A) + قدم_A);
        ڈیٹا_B = (ثابت T*)(((حرف*)ڈیٹا_B) + قدم_B);
        ڈیٹا_C =       (T*)(((حرف*)ڈیٹا_C) + قدم_C);
    }
}


سانچہ <قسم_نام T, منطقی conjB, منطقی inplaceA> ساکن اندرونی
خالی mulSpectrums_Impl_(ثابت T* ڈیٹا_A, ثابت T* ڈیٹا_B, T* ڈیٹا_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفیں, size_t ستون_تعداد, size_t j0, size_t j1, منطقی is_1d, منطقی isCN1)
{
    اگر (!is_1d && isCN1)
    {
        mul_Spectrums_process_ستون_تعداد<T, conjB, inplaceA>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, قدم_A, قدم_B, قدم_C, صفیں, ستون_تعداد);
    }
    mul_Spectrums_process_صفیں<T, conjB, inplaceA>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, قدم_A, قدم_B, قدم_C, صفیں, ستون_تعداد, j0, j1, is_1d && isCN1);
}
سانچہ <قسم_نام T, منطقی conjB> ساکن اندرونی
خالی mulSpectrums_Impl(ثابت T* ڈیٹا_A, ثابت T* ڈیٹا_B, T* ڈیٹا_C, size_t قدم_A, size_t قدم_B, size_t قدم_C, size_t صفیں, size_t ستون_تعداد, size_t j0, size_t j1, منطقی is_1d, منطقی isCN1)
{
    اگر (ڈیٹا_A == ڈیٹا_C)
        mulSpectrums_Impl_<T, conjB, سچ>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, قدم_A, قدم_B, قدم_C, صفیں, ستون_تعداد, j0, j1, is_1d, isCN1);
    ورنہ
        mulSpectrums_Impl_<T, conjB, جھوٹ>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, قدم_A, قدم_B, قدم_C, صفیں, ستون_تعداد, j0, j1, is_1d, isCN1);
}

} // namespace

خالی cv::mulSpectrums( داخلہ_Array _srcA, داخلہ_Array _srcB,
                       OutputArray _dst, صحیح flags, منطقی conjB )
{
    CV_INSTRUMENT_REGION();

    اگر (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        واپس;
    }

    صحیح type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_تسلسل(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    صحیح گہرائی = srcA.گہرائی(), cn = srcA.channels();
    size_t صفیں = srcA.صفیں, ستون_تعداد = srcA.ستون_تعداد;

    _dst.create( srcA.صفیں, srcA.ستون_تعداد, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    اگر (dst.ڈیٹا == srcB.ڈیٹا)
        srcB = srcB.clone(); // workaround for B only

    منطقی is_1d = (flags & DFT_صفیں)
        || (صفیں == 1)
        || (ستون_تعداد == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    اگر( is_1d && !(flags & DFT_صفیں) )
        ستون_تعداد = ستون_تعداد + صفیں - 1, صفیں = 1;

    منطقی isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = ستون_تعداد*cn - (((ستون_تعداد & 1) == 0 && cn == 1) ? 1 : 0);

    اگر (گہرائی == CV_32F)
    {
        ثابت اعشاری* ڈیٹا_A = srcA.ptr<اعشاری>();
        ثابت اعشاری* ڈیٹا_B = srcB.ptr<اعشاری>();
        اعشاری* ڈیٹا_C = dst.ptr<اعشاری>();
        اگر (!conjB)
            mulSpectrums_Impl<اعشاری, جھوٹ>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, srcA.قدم, srcB.قدم, dst.قدم, صفیں, ستون_تعداد, j0, j1, is_1d, isCN1);
        ورنہ
            mulSpectrums_Impl<اعشاری, سچ>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, srcA.قدم, srcB.قدم, dst.قدم, صفیں, ستون_تعداد, j0, j1, is_1d, isCN1);
    }
    ورنہ
    {
        ثابت دوہرا* ڈیٹا_A = srcA.ptr<دوہرا>();
        ثابت دوہرا* ڈیٹا_B = srcB.ptr<دوہرا>();
        دوہرا* ڈیٹا_C = dst.ptr<دوہرا>();
        اگر (!conjB)
            mulSpectrums_Impl<دوہرا, جھوٹ>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, srcA.قدم, srcB.قدم, dst.قدم, صفیں, ستون_تعداد, j0, j1, is_1d, isCN1);
        ورنہ
            mulSpectrums_Impl<دوہرا, سچ>(ڈیٹا_A, ڈیٹا_B, ڈیٹا_C, srcA.قدم, srcB.قدم, dst.قدم, صفیں, ستون_تعداد, j0, j1, is_1d, isCN1);
    }
}

خالی cv::divSpectrums( داخلہ_Array _srcA, داخلہ_Array _srcB, OutputArray _dst, صحیح flags, منطقی conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    اگر (srcA.empty() && srcB.empty())
    {
        _dst.release();
        واپس;
    }
    صحیح گہرائی = srcA.گہرائی(), cn = srcA.channels(), type = srcA.type();
    صحیح صفیں = srcA.صفیں, ستون_تعداد = srcA.ستون_تعداد;
    صحیح j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.صفیں, srcA.ستون_تعداد, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.ڈیٹا != srcA.ڈیٹا); // non-inplace check
    CV_Assert(dst.ڈیٹا != srcB.ڈیٹا); // non-inplace check

    منطقی is_1d = (flags & DFT_صفیں) || (صفیں == 1 || (ستون_تعداد == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    اگر( is_1d && !(flags & DFT_صفیں) )
        ستون_تعداد = ستون_تعداد + صفیں - 1, صفیں = 1;

    صحیح ncols = ستون_تعداد*cn;
    صحیح j0 = cn == 1;
    صحیح j1 = ncols - (ستون_تعداد % 2 == 0 && cn == 1);

    اگر( گہرائی == CV_32F )
    {
        ثابت اعشاری* ڈیٹا_A = srcA.ptr<اعشاری>();
        ثابت اعشاری* ڈیٹا_B = srcB.ptr<اعشاری>();
        اعشاری* ڈیٹا_C = dst.ptr<اعشاری>();
        اعشاری eps = FLT_EPSILON; // prevent div0 problems

        size_t قدم_A = srcA.قدم/حجم(ڈیٹا_A[0]);
        size_t قدم_B = srcB.قدم/حجم(ڈیٹا_B[0]);
        size_t قدم_C = dst.قدم/حجم(ڈیٹا_C[0]);

        اگر( !is_1d && cn == 1 )
        {
            برائے( k = 0; k < (ستون_تعداد % 2 ? 1 : 2); k++ )
            {
                اگر( k == 1 )
                    ڈیٹا_A += ستون_تعداد - 1, ڈیٹا_B += ستون_تعداد - 1, ڈیٹا_C += ستون_تعداد - 1;
                ڈیٹا_C[0] = ڈیٹا_A[0] / (ڈیٹا_B[0] + eps);
                اگر( صفیں % 2 == 0 )
                    ڈیٹا_C[(صفیں-1)*قدم_C] = ڈیٹا_A[(صفیں-1)*قدم_A] / (ڈیٹا_B[(صفیں-1)*قدم_B] + eps);
                اگر( !conjB )
                    برائے( j = 1; j <= صفیں - 2; j += 2 )
                    {
                        دوہرا denom = (دوہرا)ڈیٹا_B[j*قدم_B]*ڈیٹا_B[j*قدم_B] +
                        (دوہرا)ڈیٹا_B[(j+1)*قدم_B]*ڈیٹا_B[(j+1)*قدم_B] + (دوہرا)eps;

                        دوہرا re = (دوہرا)ڈیٹا_A[j*قدم_A]*ڈیٹا_B[j*قدم_B] +
                        (دوہرا)ڈیٹا_A[(j+1)*قدم_A]*ڈیٹا_B[(j+1)*قدم_B];

                        دوہرا im = (دوہرا)ڈیٹا_A[(j+1)*قدم_A]*ڈیٹا_B[j*قدم_B] -
                        (دوہرا)ڈیٹا_A[j*قدم_A]*ڈیٹا_B[(j+1)*قدم_B];

                        ڈیٹا_C[j*قدم_C] = (اعشاری)(re / denom);
                        ڈیٹا_C[(j+1)*قدم_C] = (اعشاری)(im / denom);
                    }
                    ورنہ
                        برائے( j = 1; j <= صفیں - 2; j += 2 )
                        {

                            دوہرا denom = (دوہرا)ڈیٹا_B[j*قدم_B]*ڈیٹا_B[j*قدم_B] +
                            (دوہرا)ڈیٹا_B[(j+1)*قدم_B]*ڈیٹا_B[(j+1)*قدم_B] + (دوہرا)eps;

                            دوہرا re = (دوہرا)ڈیٹا_A[j*قدم_A]*ڈیٹا_B[j*قدم_B] -
                            (دوہرا)ڈیٹا_A[(j+1)*قدم_A]*ڈیٹا_B[(j+1)*قدم_B];

                            دوہرا im = (دوہرا)ڈیٹا_A[(j+1)*قدم_A]*ڈیٹا_B[j*قدم_B] +
                            (دوہرا)ڈیٹا_A[j*قدم_A]*ڈیٹا_B[(j+1)*قدم_B];

                            ڈیٹا_C[j*قدم_C] = (اعشاری)(re / denom);
                            ڈیٹا_C[(j+1)*قدم_C] = (اعشاری)(im / denom);
                        }
                    اگر( k == 1 )
                        ڈیٹا_A -= ستون_تعداد - 1, ڈیٹا_B -= ستون_تعداد - 1, ڈیٹا_C -= ستون_تعداد - 1;
            }
        }

        برائے( ; صفیں--; ڈیٹا_A += قدم_A, ڈیٹا_B += قدم_B, ڈیٹا_C += قدم_C )
        {
            اگر( is_1d && cn == 1 )
            {
                ڈیٹا_C[0] = ڈیٹا_A[0] / (ڈیٹا_B[0] + eps);
                اگر( ستون_تعداد % 2 == 0 )
                    ڈیٹا_C[j1] = ڈیٹا_A[j1] / (ڈیٹا_B[j1] + eps);
            }

            اگر( !conjB )
                برائے( j = j0; j < j1; j += 2 )
                {
                    دوہرا denom = (دوہرا)ڈیٹا_B[j]*ڈیٹا_B[j] + (دوہرا)ڈیٹا_B[j+1]*ڈیٹا_B[j+1] + (دوہرا)eps;
                    دوہرا re = (دوہرا)ڈیٹا_A[j]*ڈیٹا_B[j] + (دوہرا)ڈیٹا_A[j+1]*ڈیٹا_B[j+1];
                    دوہرا im = (دوہرا)ڈیٹا_A[j+1]*ڈیٹا_B[j] - (دوہرا)ڈیٹا_A[j]*ڈیٹا_B[j+1];
                    ڈیٹا_C[j] = (اعشاری)(re / denom);
                    ڈیٹا_C[j+1] = (اعشاری)(im / denom);
                }
                ورنہ
                    برائے( j = j0; j < j1; j += 2 )
                    {
                        دوہرا denom = (دوہرا)ڈیٹا_B[j]*ڈیٹا_B[j] + (دوہرا)ڈیٹا_B[j+1]*ڈیٹا_B[j+1] + (دوہرا)eps;
                        دوہرا re = (دوہرا)ڈیٹا_A[j]*ڈیٹا_B[j] - (دوہرا)ڈیٹا_A[j+1]*ڈیٹا_B[j+1];
                        دوہرا im = (دوہرا)ڈیٹا_A[j+1]*ڈیٹا_B[j] + (دوہرا)ڈیٹا_A[j]*ڈیٹا_B[j+1];
                        ڈیٹا_C[j] = (اعشاری)(re / denom);
                        ڈیٹا_C[j+1] = (اعشاری)(im / denom);
                    }
        }
    }
    ورنہ
    {
        ثابت دوہرا* ڈیٹا_A = srcA.ptr<دوہرا>();
        ثابت دوہرا* ڈیٹا_B = srcB.ptr<دوہرا>();
        دوہرا* ڈیٹا_C = dst.ptr<دوہرا>();
        دوہرا eps = DBL_EPSILON; // prevent div0 problems

        size_t قدم_A = srcA.قدم/حجم(ڈیٹا_A[0]);
        size_t قدم_B = srcB.قدم/حجم(ڈیٹا_B[0]);
        size_t قدم_C = dst.قدم/حجم(ڈیٹا_C[0]);

        اگر( !is_1d && cn == 1 )
        {
            برائے( k = 0; k < (ستون_تعداد % 2 ? 1 : 2); k++ )
            {
                اگر( k == 1 )
                    ڈیٹا_A += ستون_تعداد - 1, ڈیٹا_B += ستون_تعداد - 1, ڈیٹا_C += ستون_تعداد - 1;
                ڈیٹا_C[0] = ڈیٹا_A[0] / (ڈیٹا_B[0] + eps);
                اگر( صفیں % 2 == 0 )
                    ڈیٹا_C[(صفیں-1)*قدم_C] = ڈیٹا_A[(صفیں-1)*قدم_A] / (ڈیٹا_B[(صفیں-1)*قدم_B] + eps);
                اگر( !conjB )
                    برائے( j = 1; j <= صفیں - 2; j += 2 )
                    {
                        دوہرا denom = ڈیٹا_B[j*قدم_B]*ڈیٹا_B[j*قدم_B] +
                        ڈیٹا_B[(j+1)*قدم_B]*ڈیٹا_B[(j+1)*قدم_B] + eps;

                        دوہرا re = ڈیٹا_A[j*قدم_A]*ڈیٹا_B[j*قدم_B] +
                        ڈیٹا_A[(j+1)*قدم_A]*ڈیٹا_B[(j+1)*قدم_B];

                        دوہرا im = ڈیٹا_A[(j+1)*قدم_A]*ڈیٹا_B[j*قدم_B] -
                        ڈیٹا_A[j*قدم_A]*ڈیٹا_B[(j+1)*قدم_B];

                        ڈیٹا_C[j*قدم_C] = re / denom;
                        ڈیٹا_C[(j+1)*قدم_C] = im / denom;
                    }
                    ورنہ
                        برائے( j = 1; j <= صفیں - 2; j += 2 )
                        {
                            دوہرا denom = ڈیٹا_B[j*قدم_B]*ڈیٹا_B[j*قدم_B] +
                            ڈیٹا_B[(j+1)*قدم_B]*ڈیٹا_B[(j+1)*قدم_B] + eps;

                            دوہرا re = ڈیٹا_A[j*قدم_A]*ڈیٹا_B[j*قدم_B] -
                            ڈیٹا_A[(j+1)*قدم_A]*ڈیٹا_B[(j+1)*قدم_B];

                            دوہرا im = ڈیٹا_A[(j+1)*قدم_A]*ڈیٹا_B[j*قدم_B] +
                            ڈیٹا_A[j*قدم_A]*ڈیٹا_B[(j+1)*قدم_B];

                            ڈیٹا_C[j*قدم_C] = re / denom;
                            ڈیٹا_C[(j+1)*قدم_C] = im / denom;
                        }
                    اگر( k == 1 )
                        ڈیٹا_A -= ستون_تعداد - 1, ڈیٹا_B -= ستون_تعداد - 1, ڈیٹا_C -= ستون_تعداد - 1;
            }
        }

        برائے( ; صفیں--; ڈیٹا_A += قدم_A, ڈیٹا_B += قدم_B, ڈیٹا_C += قدم_C )
        {
            اگر( is_1d && cn == 1 )
            {
                ڈیٹا_C[0] = ڈیٹا_A[0] / (ڈیٹا_B[0] + eps);
                اگر( ستون_تعداد % 2 == 0 )
                    ڈیٹا_C[j1] = ڈیٹا_A[j1] / (ڈیٹا_B[j1] + eps);
            }

            اگر( !conjB )
                برائے( j = j0; j < j1; j += 2 )
                {
                    دوہرا denom = ڈیٹا_B[j]*ڈیٹا_B[j] + ڈیٹا_B[j+1]*ڈیٹا_B[j+1] + eps;
                    دوہرا re = ڈیٹا_A[j]*ڈیٹا_B[j] + ڈیٹا_A[j+1]*ڈیٹا_B[j+1];
                    دوہرا im = ڈیٹا_A[j+1]*ڈیٹا_B[j] - ڈیٹا_A[j]*ڈیٹا_B[j+1];
                    ڈیٹا_C[j] = re / denom;
                    ڈیٹا_C[j+1] = im / denom;
                }
                ورنہ
                    برائے( j = j0; j < j1; j += 2 )
                    {
                        دوہرا denom = ڈیٹا_B[j]*ڈیٹا_B[j] + ڈیٹا_B[j+1]*ڈیٹا_B[j+1] + eps;
                        دوہرا re = ڈیٹا_A[j]*ڈیٹا_B[j] - ڈیٹا_A[j+1]*ڈیٹا_B[j+1];
                        دوہرا im = ڈیٹا_A[j+1]*ڈیٹا_B[j] + ڈیٹا_A[j]*ڈیٹا_B[j+1];
                        ڈیٹا_C[j] = re / denom;
                        ڈیٹا_C[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

نامخلا cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
سانچہ<قسم_نام T> ساکن خالی
DCT( ثابت OcvDftOptions & c, ثابت T* src, size_t src_قدم, T* dst, size_t dst_قدم )
{
    اگر( c.n == 1 )
    {
        dst[0] = src[0];
        واپس;
    }
    تسلسل_Dft(c, src, src_قدم/حجم(T), dst, dst_قدم/حجم(T), جھوٹ);
}

سانچہ<قسم_نام T> ساکن خالی
IDCT( ثابت OcvDftOptions & c, ثابت T* src, size_t src_قدم, T* dst, size_t dst_قدم )
{
    اگر( c.n == 1 )
    {
        dst[0] = src[0];
        واپس;
    }
    تسلسل_Dft(c, src, src_قدم/حجم(T), dst, dst_قدم/حجم(T), جھوٹ);
}

قسمتعریف خالی (*DCTFunc)(ثابت OcvDftOptions & c, ثابت خالی* src, size_t src_قدم,
                        خالی* dst, size_t dst_قدم);

سانچہ<قسم_نام T, خالی (*fn)(ثابت OcvDftOptions&, ثابت T*, size_t, T*, size_t)>
ساکن خالی dctWrap(ثابت OcvDftOptions & c, ثابت خالی* src, size_t src_قدم, خالی* dst, size_t dst_قدم)
{
    fn(c, (ثابت T*)src, src_قدم, (T*)dst, dst_قدم);
}

}

#اگر_تعریف_شدہ HAVE_IPP
نامخلا cv
{

#اگر IPP_نسخہ_X100 >= 900
قسمتعریف Ipp_حالت (CV_STDCALL * ippiDCTFunc)(ثابت Ipp32f* pSrc, صحیح src_قدم_3, Ipp32f* pDst, صحیح dst_قدم_3, ثابت خالی* pDCTSpec, Ipp8u* p_بفر);
قسمتعریف Ipp_حالت (CV_STDCALL * ippiDCTInit)(خالی* pDCTSpec, Ippi_سائز roi_سائز, Ipp8u* pMemInit );
قسمتعریف Ipp_حالت (CV_STDCALL * ippi_DCTGet_سائز)(Ippi_سائز roi_سائز, صحیح* p_سائز_Spec, صحیح* p_سائز_Init, صحیح* p_سائز_Buf);

سانچہ<قسم_نام SpecType, Ipp_حالت (CV_STDCALL *fn)(ثابت Ipp32f*, صحیح, Ipp32f*, صحیح, ثابت SpecType*, Ipp8u*)>
ساکن Ipp_حالت CV_STDCALL ippiDCTWrap(ثابت Ipp32f* pSrc, صحیح src_قدم_3, Ipp32f* pDst, صحیح dst_قدم_3, ثابت خالی* pDCTSpec, Ipp8u* p_بفر)
{
    واپس fn(pSrc, src_قدم_3, pDst, dst_قدم_3, (ثابت SpecType*)pDCTSpec, p_بفر);
}

سانچہ<قسم_نام SpecType, Ipp_حالت (CV_STDCALL *fn)(SpecType*, Ippi_سائز, Ipp8u*)>
ساکن Ipp_حالت CV_STDCALL ippiDCTInitWrap(خالی* pDCTSpec, Ippi_سائز roi_سائز, Ipp8u* pMemInit)
{
    واپس fn((SpecType*)pDCTSpec, roi_سائز, pMemInit);
}

سانچہ<Ipp_حالت (CV_STDCALL *fn)(Ippi_سائز, صحیح*, صحیح*, صحیح*)>
ساکن Ipp_حالت CV_STDCALL ippi_DCTGet_سائز_Wrap(Ippi_سائز roi_سائز, صحیح* p_سائز_Spec, صحیح* p_سائز_Init, صحیح* p_سائز_Buf)
{
    واپس fn(roi_سائز, p_سائز_Spec, p_سائز_Init, p_سائز_Buf);
}

#وگرنہ_اگر IPP_نسخہ_X100 >= 700
قسمتعریف Ipp_حالت (CV_STDCALL * ippiDCTFunc)(ثابت Ipp32f*, صحیح, Ipp32f*, صحیح, ثابت خالی*, Ipp8u*);
قسمتعریف Ipp_حالت (CV_STDCALL * ippiDCTInitAlloc)(خالی**, Ippi_سائز, IppHintAlgorithm);
قسمتعریف Ipp_حالت (CV_STDCALL * ippiDCTFree)(خالی* pDCTSpec);
قسمتعریف Ipp_حالت (CV_STDCALL * ippi_DCTGet_Buf_سائز)(ثابت خالی*, صحیح*);

سانچہ<قسم_نام SpecType, Ipp_حالت (CV_STDCALL *fn)(ثابت Ipp32f*, صحیح, Ipp32f*, صحیح, ثابت SpecType*, Ipp8u*)>
ساکن Ipp_حالت CV_STDCALL ippiDCTWrap(ثابت Ipp32f* pSrc, صحیح src_قدم_3, Ipp32f* pDst, صحیح dst_قدم_3, ثابت خالی* pDCTSpec, Ipp8u* p_بفر)
{
    واپس fn(pSrc, src_قدم_3, pDst, dst_قدم_3, (ثابت SpecType*)pDCTSpec, p_بفر);
}

سانچہ<قسم_نام SpecType, Ipp_حالت (CV_STDCALL *fn)(SpecType**, Ippi_سائز, IppHintAlgorithm)>
ساکن Ipp_حالت CV_STDCALL ippiDCTInitAllocWrap(خالی** pDCTSpec, Ippi_سائز roi_سائز, IppHintAlgorithm hint)
{
    واپس fn((SpecType**)pDCTSpec, roi_سائز, hint);
}

سانچہ<قسم_نام SpecType, Ipp_حالت (CV_STDCALL *fn)(SpecType*)>
ساکن Ipp_حالت CV_STDCALL ippiDCTFreeWrap(خالی* pDCTSpec)
{
    واپس fn((SpecType*)pDCTSpec);
}

سانچہ<قسم_نام SpecType, Ipp_حالت (CV_STDCALL *fn)(ثابت SpecType*, صحیح*)>
ساکن Ipp_حالت CV_STDCALL ippi_DCTGet_Buf_سائز_Wrap(ثابت خالی* pDCTSpec, صحیح* p_سائز)
{
    واپس fn((ثابت SpecType*)pDCTSpec, p_سائز);
}
#اگر_ختم

جماعت DctIPPLoop_Invoker : عوامی ParallelLoopBody
{
عوامی:
    DctIPPLoop_Invoker(ثابت uchar * _src, size_t src_قدم_2, uchar * _dst, size_t dst_قدم_2, صحیح چوڑائی_3, منطقی _inv, منطقی *_ok) :
        ParallelLoopBody(), src(_src), src_قدم(src_قدم_2), dst(_dst), dst_قدم(dst_قدم_2), چوڑائی(چوڑائی_3), inv(_inv), ok(_ok)
    {
        *ok = سچ;
    }

    مجازی خالی عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        اگر(*ok == جھوٹ)
            واپس;

#اگر IPP_نسخہ_X100 >= 900
        Ippi_سائز src_Roi_سائز = {چوڑائی, 1};

        صحیح spec_سائز    = 0;
        صحیح init_سائز    = 0;
        صحیح بفر_سائز  = 0;

        Ipp8u* pDCTSpec = تہی;
        Ipp8u* p_بفر  = تہی;
        Ipp8u* pInitBuf = تہی;

        #تعریف_کرو IPP_RETURN              \
            اگر(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            اگر(p_بفر)                 \
                ippFree(p_بفر);       \
            اگر(pInitBuf)                \
                ippFree(pInitBuf);      \
            واپس;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_سائز  ipp_Dct_Get_سائز  = inv ? ippi_DCTGet_سائز_Wrap<ippi_DCTInv_Get_سائز_32f> : ippi_DCTGet_سائز_Wrap<ippi_DCTFwd_Get_سائز_32f>;

        اگر(ipp_Dct_Get_سائز(src_Roi_سائز, &spec_سائز, &init_سائز, &بفر_سائز) < 0)
        {
            *ok = جھوٹ;
            واپس;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_سائز);
        اگر(!pDCTSpec && spec_سائز)
        {
            *ok = جھوٹ;
            واپس;
        }

        p_بفر  = (Ipp8u*)CV_IPP_MALLOC(بفر_سائز);
        اگر(!p_بفر && بفر_سائز)
        {
            *ok = جھوٹ;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_سائز);
        اگر(!pInitBuf && init_سائز)
        {
            *ok = جھوٹ;
            IPP_RETURN
        }

        اگر(ippDctInit(pDCTSpec, src_Roi_سائز, pInitBuf) < 0)
        {
            *ok = جھوٹ;
            IPP_RETURN
        }

        برائے(صحیح i = range.آغاز; i < range.end; ++i)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (اعشاری*)(src + src_قدم * i), static_cast<صحیح>(src_قدم), (اعشاری*)(dst + dst_قدم * i), static_cast<صحیح>(dst_قدم), pDCTSpec, p_بفر) < 0)
            {
                *ok = جھوٹ;
                IPP_RETURN
            }
        }
        IPP_RETURN
#تعریف_منسوخ IPP_RETURN
#وگرنہ_اگر IPP_نسخہ_X100 >= 700
        خالی* pDCTSpec;
        Auto_بفر<uchar> buf;
        uchar* p_بفر = 0;
        صحیح buf_سائز=0;

        Ippi_سائز src_Roi_سائز = {چوڑائی, 1};

        CV_SUPPRESS_DEPRECATED_آغاز

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_سائز ipp_Get_Buf_سائز = inv ? ippi_DCTGet_Buf_سائز_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_سائز_32f> : ippi_DCTGet_Buf_سائز_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_سائز_32f>;

        اگر (ippInitAlloc(&pDCTSpec, src_Roi_سائز, ippAlgHintNone)>=0 && ipp_Get_Buf_سائز(pDCTSpec, &buf_سائز)>=0)
        {
            buf.allocate( buf_سائز );
            p_بفر = (uchar*)buf;

            برائے( صحیح i = range.آغاز; i < range.end; ++i)
            {
                اگر(ippDctFun((اعشاری*)(src + src_قدم * i), static_cast<صحیح>(src_قدم), (اعشاری*)(dst + dst_قدم * i), static_cast<صحیح>(dst_قدم), pDCTSpec, (Ipp8u*)p_بفر) < 0)
                {
                    *ok = جھوٹ;
                    توڑو;
                }
            }
        }
        ورنہ
            *ok = جھوٹ;

        اگر (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_انتہا
#ورنہ
        CV_غیر_مستعمل(range);
        *ok = جھوٹ;
#اگر_ختم
    }

نجی:
    ثابت uchar * src;
    size_t src_قدم;
    uchar * dst;
    size_t dst_قدم;
    صحیح چوڑائی;
    منطقی inv;
    منطقی *ok;
};

ساکن منطقی DctIPPLoop(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحیح چوڑائی, صحیح اونچائی, منطقی inv)
{
    منطقی ok;
    parallel_for_(Range(0, اونچائی), DctIPPLoop_Invoker(src, src_قدم, dst, dst_قدم, چوڑائی, inv, &ok), اونچائی/(دوہرا)(1<<4) );
    واپس ok;
}

ساکن منطقی ippi_DCT_32f(ثابت uchar * src, size_t src_قدم, uchar * dst, size_t dst_قدم, صحیح چوڑائی, صحیح اونچائی, منطقی inv, منطقی صف)
{
    CV_INSTRUMENT_REGION_IPP();

    اگر(صف)
        واپس DctIPPLoop(src, src_قدم, dst, dst_قدم, چوڑائی, اونچائی, inv);
    ورنہ
    {
#اگر IPP_نسخہ_X100 >= 900
        Ippi_سائز src_Roi_سائز = {چوڑائی, اونچائی};

        صحیح spec_سائز    = 0;
        صحیح init_سائز    = 0;
        صحیح بفر_سائز  = 0;

        Ipp8u* pDCTSpec = تہی;
        Ipp8u* p_بفر  = تہی;
        Ipp8u* pInitBuf = تہی;

        #تعریف_کرو IPP_RELEASE             \
            اگر(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            اگر(p_بفر)                 \
                ippFree(p_بفر);       \
            اگر(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_سائز  ipp_Dct_Get_سائز  = inv ? ippi_DCTGet_سائز_Wrap<ippi_DCTInv_Get_سائز_32f> : ippi_DCTGet_سائز_Wrap<ippi_DCTFwd_Get_سائز_32f>;

        اگر(ipp_Dct_Get_سائز(src_Roi_سائز, &spec_سائز, &init_سائز, &بفر_سائز) < 0)
            واپس جھوٹ;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_سائز);
        اگر(!pDCTSpec && spec_سائز)
            واپس جھوٹ;

        p_بفر  = (Ipp8u*)CV_IPP_MALLOC(بفر_سائز);
        اگر(!p_بفر && بفر_سائز)
        {
            IPP_RELEASE
            واپس جھوٹ;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_سائز);
        اگر(!pInitBuf && init_سائز)
        {
            IPP_RELEASE
            واپس جھوٹ;
        }

        اگر(ippDctInit(pDCTSpec, src_Roi_سائز, pInitBuf) < 0)
        {
            IPP_RELEASE
            واپس جھوٹ;
        }

        اگر(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (اعشاری*)src, static_cast<صحیح>(src_قدم), (اعشاری*)dst, static_cast<صحیح>(dst_قدم), pDCTSpec, p_بفر) < 0)
        {
            IPP_RELEASE
            واپس جھوٹ;
        }

        IPP_RELEASE
        واپس سچ;
#تعریف_منسوخ IPP_RELEASE
#وگرنہ_اگر IPP_نسخہ_X100 >= 700
        Ipp_حالت حالت_2;
        خالی* pDCTSpec;
        Auto_بفر<uchar> buf;
        uchar* p_بفر = 0;
        صحیح buf_سائز=0;

        Ippi_سائز src_Roi_سائز = {چوڑائی, اونچائی};

        CV_SUPPRESS_DEPRECATED_آغاز

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_سائز ipp_Get_Buf_سائز = inv ? ippi_DCTGet_Buf_سائز_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_سائز_32f> : ippi_DCTGet_Buf_سائز_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_سائز_32f>;

        حالت_2 = ippStsErr;

        اگر (ippInitAlloc(&pDCTSpec, src_Roi_سائز, ippAlgHintNone)>=0 && ipp_Get_Buf_سائز(pDCTSpec, &buf_سائز)>=0)
        {
            buf.allocate( buf_سائز );
            p_بفر = (uchar*)buf;

            حالت_2 = ippDctFun((اعشاری*)src, static_cast<صحیح>(src_قدم), (اعشاری*)dst, static_cast<صحیح>(dst_قدم), pDCTSpec, (Ipp8u*)p_بفر);
        }

        اگر (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_انتہا

        واپس حالت_2 >= 0;
#ورنہ
        CV_غیر_مستعمل(src); CV_غیر_مستعمل(dst); CV_غیر_مستعمل(inv); CV_غیر_مستعمل(صف);
        واپس جھوٹ;
#اگر_ختم
    }
}
}
#اگر_ختم

نامخلا cv {

جماعت OcvDctImpl CV_FINAL : عوامی hal::DCT2D
{
عوامی:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    Auto_بفر<uchar> ws;
    منطقی is_صف_Transform;
    منطقی isInverse;
    منطقی isContinuous;
    صحیح آغاز_stage;
    صحیح انتہا_stage;
    صحیح چوڑائی;
    صحیح اونچائی;
    صحیح گہرائی;

    خالی init(صحیح چوڑائی_3, صحیح اونچائی_3, صحیح گہرائی_2, صحیح flags)
    {
        چوڑائی = چوڑائی_3;
        اونچائی = اونچائی_3;
        گہرائی = گہرائی_2;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        is_صف_Transform = (flags & CV_HAL_DFT_صفیں) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        ساکن DCTFunc dct_tbl[4] =
        {
            dctWrap<اعشاری, DCT<اعشاری>>,
            dctWrap<اعشاری, IDCT<اعشاری>>,
            dctWrap<دوہرا, DCT<دوہرا>>,
            dctWrap<دوہرا, IDCT<دوہرا>>
        };
        dct_func = dct_tbl[(صحیح)isInverse + (گہرائی == CV_64F)*2];
        opt.isComplex = جھوٹ;
        opt.isInverse = جھوٹ;
        opt.scale = 1.;

        اگر (is_صف_Transform || اونچائی == 1 || (چوڑائی == 1 && isContinuous))
        {
            آغاز_stage = انتہا_stage = 0;
        }
        ورنہ
        {
            آغاز_stage = (چوڑائی == 1);
            انتہا_stage = 1;
        }
    }
    خالی apply(ثابت uchar *src, size_t src_قدم, uchar *dst, size_t dst_قدم) CV_OVERRIDE
    {
        CV_IPP_تسلسل(IPP_نسخہ_X100 >= 700 && گہرائی == CV_32F, ippi_DCT_32f(src, src_قدم, dst, dst_قدم, چوڑائی, اونچائی, isInverse, is_صف_Transform))

        صحیح پچھلا_len = 0;
        صحیح elem_سائز_2 = (گہرائی == CV_32F) ? حجم(اعشاری) : حجم(دوہرا);
        ثابت DFTKernels& kernels = getDFTKernels(گہرائی);

        برائے(صحیح stage = آغاز_stage ; stage <= انتہا_stage; stage++ )
        {
            ثابت uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            صحیح len, گنتی_2;

            اگر( stage == 0 )
            {
                len = چوڑائی;
                گنتی_2 = اونچائی;
                اگر( len == 1 && !is_صف_Transform )
                {
                    len = اونچائی;
                    گنتی_2 = 1;
                }
                sstep0 = src_قدم;
                dstep0 = dst_قدم;
                sstep1 = dstep1 = elem_سائز_2;
            }
            ورنہ
            {
                len = اونچائی;
                گنتی_2 = چوڑائی;
                sstep1 = src_قدم;
                dstep1 = dst_قدم;
                sstep0 = dstep0 = elem_سائز_2;
            }

            opt.n = len;

            اگر( len != پچھلا_len )
            {
                اگر( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                اگر( len > 1 )
                {
                    plan.build(isInverse ? DFT_قسم_IDCT : DFT_قسم_DCT, len, گہرائی, kernels.vlanes);
                    ws.allocate(plan.ws_بائٹیں);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = ws.ڈیٹا();
                پچھلا_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            دوہرا nstripes = dftParallelStripes(گنتی_2, len);
            اگر( len > 1 && گنتی_2 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                خودکار process_سطریں = [&](ثابت Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    Auto_بفر<uchar> ws_(plan.ws_بائٹیں);
                    opt_.workspace = ws_.ڈیٹا();
                    برائے( صحیح i = range.آغاز; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, گنتی_2), process_سطریں, nstripes);
            }
            ورنہ
            {
                برائے(غیرعلامتی i = 0; i < static_cast<غیرعلامتی>(گنتی_2); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_قدم = dst_قدم;
        }
    }
};

ساخت ReplacementDCT2D : عوامی hal::DCT2D
{
    cvhalDFT *context;
    منطقی isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(جھوٹ) {}
    منطقی init(صحیح چوڑائی, صحیح اونچائی, صحیح گہرائی, صحیح flags)
    {
        صحیح res = cv_hal_dctInit2D(&context, چوڑائی, اونچائی, گہرائی, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        واپس isInitialized;
    }
    خالی apply(ثابت uchar *src_ڈیٹا, size_t src_قدم, uchar *dst_ڈیٹا, size_t dst_قدم) CV_OVERRIDE
    {
        اگر (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_ڈیٹا, src_قدم, dst_ڈیٹا, dst_قدم);
        }
    }
    ~ReplacementDCT2D()
    {
        اگر (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

نامخلا hal {

Ptr<DCT2D> DCT2D::create(صحیح چوڑائی, صحیح اونچائی, صحیح گہرائی, صحیح flags)
{
    {
        ReplacementDCT2D *impl = نیا ReplacementDCT2D();
        اگر (impl->init(چوڑائی, اونچائی, گہرائی, flags))
        {
            واپس Ptr<DCT2D>(impl);
        }
        حذف impl;
    }
    {
        OcvDctImpl *impl = نیا OcvDctImpl();
        impl->init(چوڑائی, اونچائی, گہرائی, flags);
        واپس Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

خالی cv::dct( داخلہ_Array _src0, OutputArray _dst, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    اگر (src.empty())
    {
        _dst.release();
        واپس;
    }
    صحیح type = src.type(), گہرائی = src.گہرائی();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.صفیں, src.ستون_تعداد, type );
    Mat dst = _dst.getMat();

    صحیح f = 0;
    اگر ((flags & DFT_صفیں) != 0)
        f |= CV_HAL_DFT_صفیں;
    اگر ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    اگر (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.ستون_تعداد, src.صفیں, گہرائی, f);
    c->apply(src.ڈیٹا, src.قدم, dst.ڈیٹا, dst.قدم);
}


خالی cv::idct( داخلہ_Array src, OutputArray dst, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

نامخلا cv
{

ساکن ثابت صحیح optimalDFTSizeTab[] = {
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

صحیح cv::getOptimalDFTSize( صحیح size0 )
{
    صحیح a = 0, b = حجم(optimalDFTSizeTab)/حجم(optimalDFTSizeTab[0]) - 1;
    اگر( (غیرعلامتی)size0 >= (غیرعلامتی)optimalDFTSizeTab[b] )
        واپس -1;

    جب( a < b )
    {
        صحیح c = (a + b) >> 1;
        اگر( size0 <= optimalDFTSizeTab[c] )
            b = c;
        ورنہ
            a = c+1;
    }

    واپس optimalDFTSizeTab[b];
}
/* End of file. */
