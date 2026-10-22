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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_core.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "dxt.hpp"
#अन्तर्भाव "dxt.simd.hpp"
#अन्तर्भाव "dxt.simd_declarations.hpp"
#अन्तर्भाव <map>

नामाकाश cv
{

#यदि IPP_संस्करणम्_X100 >= 710
#परिभाषय USE_IPP_DFT 1
#अन्यथा
#अपरिभाषय USE_IPP_DFT
#यदि_अन्तः

#यदि defined USE_IPP_DFT
#यदि IPP_संस्करणम्_X100 >= 202220
#परिभाषय IPP_DISABLE_DFT32F ((गभीरता == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#अन्यथा
#परिभाषय IPP_DISABLE_DFT32F असत्य
#यदि_अन्तः
#यदि_अन्तः

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#यदि_परिभाषितम् HAVE_OPENCL
// factorization used by the OpenCL radix selection
स्थिर पूर्णाङ्क
DFTFactorize( पूर्णाङ्क n, पूर्णाङ्क* factors )
{
    पूर्णाङ्क nf = 0, f, i, j;

    यदि( n <= 5 )
    {
        factors[0] = n;
        प्रत्यागम 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    यदि( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    प्रति( f = 3; n > 1; )
    {
        पूर्णाङ्क d = n/f;
        यदि( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        अन्यथा
        {
            f += 2;
            यदि( f*f > n )
                भङ्ग;
        }
    }

    यदि( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    प्रति( i = f; i < (nf+f)/2; i++ )
        CV_SWAP( factors[i], factors[nf-i-1+f], j );

    प्रत्यागम nf;
}
#यदि_अन्तः

#यदि_परिभाषितम् USE_IPP_DFT
स्थिर Ipp_स्थितिः ippsDFTFwd_CToC( ध्रुव Complex<भिन्न>* src, Complex<भिन्न>* dst,
                             ध्रुव शून्य* spec, uchar* buf)
{
    प्रत्यागम CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (ध्रुव Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ध्रुव IppsDFTSpec_C_32fc*)spec, buf);
}

स्थिर Ipp_स्थितिः ippsDFTFwd_CToC( ध्रुव Complex<द्विगुण>* src, Complex<द्विगुण>* dst,
                             ध्रुव शून्य* spec, uchar* buf)
{
    प्रत्यागम CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (ध्रुव Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ध्रुव IppsDFTSpec_C_64fc*)spec, buf);
}

स्थिर Ipp_स्थितिः ippsDFTInv_CToC( ध्रुव Complex<भिन्न>* src, Complex<भिन्न>* dst,
                             ध्रुव शून्य* spec, uchar* buf)
{
    प्रत्यागम CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (ध्रुव Ipp32fc*)src, (Ipp32fc*)dst,
                                 (ध्रुव IppsDFTSpec_C_32fc*)spec, buf);
}

स्थिर Ipp_स्थितिः ippsDFTInv_CToC( ध्रुव Complex<द्विगुण>* src, Complex<द्विगुण>* dst,
                                  ध्रुव शून्य* spec, uchar* buf)
{
    प्रत्यागम CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (ध्रुव Ipp64fc*)src, (Ipp64fc*)dst,
                                 (ध्रुव IppsDFTSpec_C_64fc*)spec, buf);
}

स्थिर Ipp_स्थितिः ippsDFTFwd_RToPack( ध्रुव भिन्न* src, भिन्न* dst,
                                     ध्रुव शून्य* spec, uchar* buf)
{
    प्रत्यागम CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (ध्रुव IppsDFTSpec_R_32f*)spec, buf);
}

स्थिर Ipp_स्थितिः ippsDFTFwd_RToPack( ध्रुव द्विगुण* src, द्विगुण* dst,
                                     ध्रुव शून्य* spec, uchar* buf)
{
    प्रत्यागम CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (ध्रुव IppsDFTSpec_R_64f*)spec, buf);
}

स्थिर Ipp_स्थितिः ipps_DFTInv_Pack_गन्तव्यम्_R( ध्रुव भिन्न* src, भिन्न* dst,
                                     ध्रुव शून्य* spec, uchar* buf)
{
    प्रत्यागम CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_गन्तव्यम्_R_32f, src, dst, (ध्रुव IppsDFTSpec_R_32f*)spec, buf);
}

स्थिर Ipp_स्थितिः ipps_DFTInv_Pack_गन्तव्यम्_R( ध्रुव द्विगुण* src, द्विगुण* dst,
                                     ध्रुव शून्य* spec, uchar* buf)
{
    प्रत्यागम CV_INSTRUMENT_FUN_IPP(ipps_DFTInv_Pack_गन्तव्यम्_R_64f, src, dst, (ध्रुव IppsDFTSpec_R_64f*)spec, buf);
}
#यदि_अन्तः

संरचना OcvDftOptions;

प्रकारनिर्देश शून्य (*DFTFunc)(ध्रुव OcvDftOptions & c, ध्रुव शून्य* src, शून्य* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
संरचना OcvDftOptions {
    द्विगुण scale;
    पूर्णाङ्क n;

    तार्किक isInverse;
    तार्किक isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    तार्किक useIpp;

#यदि_परिभाषितम् USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#यदि_अन्तः

    ध्रुव DftPlan* plan;
    ध्रुव DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = असत्य;
        isComplex = असत्य;
        useIpp = असत्य;
#यदि_परिभाषितम् USE_IPP_DFT
        ipp_spec = 0;
        ipp_work = 0;
#यदि_अन्तः
        dft_func = 0;
    }
};


/****************************************************************************************\
                     New 1D engine: kernel table, plan builder, driver
\****************************************************************************************/

स्थिर DFTKernels getDFTKernelsDispatch(पूर्णाङ्क गभीरता)
{
    CV_CPU_DISPATCH(getDFTKernels_, (गभीरता), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
स्थिर ध्रुव DFTKernels& getDFTKernels(पूर्णाङ्क गभीरता)
{
    स्थिर ध्रुव DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    स्थिर ध्रुव DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    प्रत्यागम गभीरता == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
संरचना DftTwiddleGen
{
    पूर्णाङ्क M, logB, आवरणम्_B;
    Auto_बफर<द्विगुण, 1024> buf;
    द्विगुण *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    स्थिर शून्य fill_सारणी(द्विगुण* re, द्विगुण* im, पूर्णाङ्क गणनम्_2, द्विगुण wr, द्विगुण wi, द्विगुण चरणः_Angle)
    {
        प्रति (पूर्णाङ्क i0 = 0; i0 < गणनम्_2; i0 += 16)
        {
            द्विगुण t0 = चरणः_Angle*i0;
            द्विगुण cr = i0 == 0 ? 1 : कोटिज्या(t0), ci = i0 == 0 ? 0 : ज्या(t0);
            पूर्णाङ्क i1 = std::min(i0 + 16, गणनम्_2);
            प्रति (पूर्णाङ्क i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                द्विगुण t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    शून्य init(पूर्णाङ्क _M)
    {
        M = _M;
        logB = 0;
        यावत् ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        पूर्णाङ्क B = 1 << logB;
        आवरणम्_B = B - 1;
        पूर्णाङ्क nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.दत्त(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        द्विगुण a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fill_सारणी(lo_re, lo_im, B, कोटिज्या(a), ज्या(a), a);
        fill_सारणी(hi_re, hi_im, nhi, कोटिज्या(a*B), ज्या(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    शून्य get(पूर्णाङ्क m, द्विगुण& re, द्विगुण& im) ध्रुव
    {
        CV_DbgAssert(m >= 0);
        यदि (m >= M)
            m %= M;
        पूर्णाङ्क a = m >> logB, b = m & आवरणम्_B;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

प्रतिरूपम्<प्रकारनाम T>
शून्य DftPlan::fillTables(size_t ntw, तार्किक need_rtw, तार्किक need_dct)
{
    DftPlan& p = *एतत्;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    पूर्णाङ्क M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*परिमाण(T) + परिमाण(द्विगुण) - 1)/परिमाण(द्विगुण) + 1);
    T* cur = (T*)p.tw.दत्त();
    पूर्णाङ्क maxL = 1;
    प्रति (पूर्णाङ्क s = 0; s < p.nstages; s++)
        यदि (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].विस्तृतिः);
    Auto_बफर<द्विगुण, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    द्विगुण* twd = twd_buf.दत्त();
    प्रति (पूर्णाङ्क s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        पूर्णाङ्क r = st.radix, L = st.विस्तृतिः, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        पूर्णाङ्क चरणः = M/(r*L);
        यदि (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr पूर्णाङ्क K = 8;
            द्विगुण cr[K], ci[K], wr, wi;
            gen.get(K*चरणः, wr, wi);
            द्विगुण* t1r = twd; द्विगुण* t1i = twd + L;      // leg 1 kept in double for the products
            प्रति (पूर्णाङ्क j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                प्रति (पूर्णाङ्क k = 0; k < K; k++)
                    gen.get(std::min(j0 + k, L - 1)*चरणः, cr[k], ci[k]);
                पूर्णाङ्क j1 = std::min(j0 + 16*K, L);
                प्रति (पूर्णाङ्क j = j0; j < j1; j += K)
                    प्रति (पूर्णाङ्क k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        द्विगुण t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            प्रति (पूर्णाङ्क j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            प्रति (पूर्णाङ्क q = 2; q < r; q++)
            {
                ध्रुव T* pr = twr + (q-2)*ts; ध्रुव T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                प्रति (पूर्णाङ्क j = 0; j < L; j++)
                {
                    द्विगुण ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        अन्यथा
        {
            // large odd radix: exact-ish generator lookup per entry
            प्रति (पूर्णाङ्क q = 1; q < r; q++)
                प्रति (पूर्णाङ्क j = 0; j < L; j++)
                {
                    द्विगुण re, im;
                    gen.get(q*j*चरणः, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        यदि (r >= 7)
        {
            पूर्णाङ्क h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            प्रति (पूर्णाङ्क pp = 1; pp <= h; pp++)
                प्रति (पूर्णाङ्क k = 1; k <= h; k++)
                {
                    द्विगुण t = 2*CV_PI*((द्विगुण)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)कोटिज्या(t);
                    sn[(pp-1)*h + (k-1)] = (T)ज्या(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    यदि (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        पूर्णाङ्क चरणः = M/n;
        प्रति (पूर्णाङ्क k = 0; k <= nc; k++)
        {
            द्विगुण re, im;
            gen.get(k*चरणः, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    यदि (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        द्विगुण scale = (p.विधा == DFT_विधा_DCT ? 2 : 1)*std::वर्गमूलम्(1./(2*n));
        पूर्णाङ्क चरणः = M/(4*n);
        प्रति (पूर्णाङ्क k = 0; k <= nc; k++)
        {
            द्विगुण re, im;
            gen.get(k*चरणः, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
शून्य DftPlan::build(पूर्णाङ्क विधा_2, पूर्णाङ्क _n, पूर्णाङ्क गभीरता_2, पूर्णाङ्क _vl)
{
    DftPlan& p = *एतत्;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(गभीरता_2 == CV_32F || गभीरता_2 == CV_64F);
    तार्किक real_विधा = विधा_2 != DFT_विधा_C2_C;
    तार्किक dct_विधा = विधा_2 == DFT_विधा_DCT || विधा_2 == DFT_विधा_IDCT;
    यदि (dct_विधा)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.विधा = विधा_2; p.n = _n; p.गभीरता = गभीरता_2; p.vl = _vl;
    p.nc = real_विधा && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    size_t esz = गभीरता == CV_32F ? परिमाण(भिन्न) : परिमाण(द्विगुण);
    p.real_निवेशः = विधा == DFT_विधा_R2_C && (n & 1) != 0;
    p.need_tmp = विधा == DFT_विधा_C2_R || विधा == DFT_विधा_DCT || विधा == DFT_विधा_IDCT;
    p.pingpong = असत्य;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    पूर्णाङ्क k2 = 0, rest = nc;
    यावत् ((rest & 1) == 0) { rest >>= 1; k2++; }
    पूर्णाङ्क r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.first_radix = r0;
    पूर्णाङ्क radices[DFT_MAX_STAGES], nst = 0;
    प्रति (पूर्णाङ्क m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    यावत् (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    यावत् (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    प्रति (पूर्णाङ्क f = 7; rest > 1; f += 2)
    {
        यदि (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            भङ्ग;
        }
        यावत् (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    पूर्णाङ्क L = r0, max_h = 0;
    size_t ntw = 0;
    प्रति (पूर्णाङ्क s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        पूर्णाङ्क r = radices[s];
        st.radix = r; st.विस्तृतिः = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        यदि (vl > 1 && L >= vl)
        {
            st.बहुलकम् = DFT_STAGE_पूर्णम्;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            यदि (L % vl != 0)
                p.pingpong = सत्य;   // back-off clamp => overlapping stores => must be out-of-place
        }
        अन्यथा
        {
            st.बहुलकम् = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(size_t)(r - 1)*st.tw_stride;
        यदि (r >= 7)
        {
            पूर्णाङ्क h = (r - 1)/2;
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
    पूर्णाङ्क seq[DFT_MAX_STAGES + 1], nseq = 0;
    यदि (r0 > 1) seq[nseq++] = r0;
    प्रति (पूर्णाङ्क s = 0; s < nst; s++) seq[nseq++] = radices[s];
    पूर्णाङ्क digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        पूर्णाङ्क Ls = 1;
        प्रति (पूर्णाङ्क s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    यदि (r0 == 1)
    {
        p.itab.allocate(nc*2);
        पूर्णाङ्क* it_all = p.itab.दत्त();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        पूर्णाङ्क n_अन्तर् = std::min(nseq, 2);
        पूर्णाङ्क r_0 = n_अन्तर् > 0 ? seq[0] : 1, w_0 = n_अन्तर् > 0 ? weights[0] : 0;
        पूर्णाङ्क r_1 = n_अन्तर् > 1 ? seq[1] : 1, w_1 = n_अन्तर् > 1 ? weights[1] : 0;
        प्रति (पूर्णाङ्क pos = 0, rev_hi = 0; pos < nc; )
        {
            प्रति (पूर्णाङ्क d1 = 0; d1 < r_1; d1++)
            {
                पूर्णाङ्क rev = rev_hi + d1*w_1;
                पूर्णाङ्क* it = it_all + pos*2;
                यदि (p.real_निवेशः)
                {
                    प्रति (पूर्णाङ्क d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                अन्यथा
                {
                    प्रति (पूर्णाङ्क d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            प्रति (पूर्णाङ्क s = n_अन्तर्; s < nseq; s++)
            {
                rev_hi += weights[s];
                यदि (++digits[s] < seq[s])
                    भङ्ग;
                digits[s] = 0;
                rev_hi -= seq[s]*weights[s];
            }
        }
    }
    अन्यथा
    {
        // gtab[base(g)] = g*r0, written in increasing base order (sequential stores): base is
        // counted up with the digits s = nseq-1 (base weight 1) .. 1 (base weight weights[1]),
        // g follows incrementally with the digit weights gw[s] = L_s/r0. The two fastest digits
        // are enumerated by plain nested loops.
        पूर्णाङ्क M = nc/r0;
        p.itab.allocate(M);
        पूर्णाङ्क* gtab = p.itab.दत्त();
        पूर्णाङ्क gw[DFT_MAX_STAGES + 1];
        {
            पूर्णाङ्क Ls = 1;
            प्रति (पूर्णाङ्क s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        पूर्णाङ्क sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        पूर्णाङ्क r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        पूर्णाङ्क r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        प्रति (पूर्णाङ्क आधारः = 0, g_hi = 0; आधारः < M; )
        {
            प्रति (पूर्णाङ्क db = 0; db < r_b; db++)
            {
                पूर्णाङ्क g = (g_hi + db*g_b)*r0;
                प्रति (पूर्णाङ्क da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[आधारः++] = g;
            }
            प्रति (पूर्णाङ्क s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                यदि (++digits[s] < seq[s])
                    भङ्ग;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    तार्किक need_rtw = real_विधा && n % 2 == 0;
    यदि (need_rtw) ntw += 2*(size_t)(nc + 1);
    यदि (dct_विधा) ntw += 2*(size_t)(nc + 1);
    यदि (गभीरता == CV_32F)
        fillTables<भिन्न>(ntw, need_rtw, dct_विधा);
    अन्यथा
        fillTables<द्विगुण>(ntw, need_rtw, dct_विधा);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    size_t pair_अष्टकानि = 2*(size_t)nc*esz;
    तार्किक two_युग्मानि = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_युग्मानि ? pair_अष्टकानि : 0;
    p.scratch_ofs = pair_अष्टकानि*(two_युग्मानि ? 2 : 1);
    size_t scratch_अष्टकानि = 4*(size_t)max_h*std::max(vl, 1)*esz;
    p.ws_अष्टकानि = p.scratch_ofs + scratch_अष्टकानि + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
स्थिर शून्य सततम्_Dft(ध्रुव OcvDftOptions& c, ध्रुव शून्य* src, size_t sstep, शून्य* dst, size_t dstep,
                   तार्किक complex_io)
{
    ध्रुव DftPlan& p = *c.plan;
    ध्रुव DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    size_t esz = p.गभीरता == CV_32F ? परिमाण(भिन्न) : परिमाण(द्विगुण);
    uchar* आधारः = alignPtr(c.workspace, 64);
    uchar* re0 = आधारः;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = आधारः + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = आधारः + p.scratch_ofs;

    ध्रुव शून्य* asrc = src;
    size_t astep = sstep;
    तार्किक conj = असत्य;
    यदि (p.विधा == DFT_विधा_C2_R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = सत्य;
    }
    अन्यथा यदि (p.विधा == DFT_विधा_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = सत्य;
    }
    अन्यथा यदि (p.विधा == DFT_विधा_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    अन्यथा यदि (p.विधा == DFT_विधा_C2_C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.first_radix == 1);

    DftPreprocFunc pre = p.first_radix == 1 ? k.preprocRadix0 : p.first_radix == 2 ? k.preprocRadix2 :
                         p.first_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    प्रति (पूर्णाङ्क i = 0; i < p.nstages; i++)
    {
        ध्रुव DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        यदि (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        अन्यथा
            fn(st, sre, sim, sre, sim, scratch);
    }

    द्विगुण scale = c.scale;
    विकल्प (p.विधा)
    {
    पक्ष DFT_विधा_C2_C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        भङ्ग;
    पक्ष DFT_विधा_R2_C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        भङ्ग;
    पक्ष DFT_विधा_C2_R:
        यदि (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        अन्यथा
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        भङ्ग;
    पक्ष DFT_विधा_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        भङ्ग;
    स्वभाव:
        CV_Assert(p.विधा == DFT_विधा_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
DFT(ध्रुव OcvDftOptions & c, ध्रुव Complex<T>* src, Complex<T>* dst)
{
    यदि( c.useIpp )
    {
#यदि_परिभाषितम् USE_IPP_DFT
        यदि( !c.isInverse )
        {
            यदि (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_सङ्कलय(CV_IMPL_IPP);
                प्रत्यागम;
            }
        }
        अन्यथा
        {
            यदि (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_सङ्कलय(CV_IMPL_IPP);
                प्रत्यागम;
            }
        }
        समुच्चयः_Ipp_Error_स्थितिः();
#यदि_अन्तः
    }

    सततम्_Dft(c, src, 1, dst, 1, असत्य);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
RealDFT(ध्रुव OcvDftOptions & c, ध्रुव T* src, T* dst)
{
    पूर्णाङ्क complex_output = c.isComplex;

    यदि( c.useIpp )
    {
#यदि_परिभाषितम् USE_IPP_DFT
        पूर्णाङ्क n = c.n;
        T* ipp_dst = dst + complex_output;
        यदि (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            यदि( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                यदि( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_सङ्कलय(CV_IMPL_IPP);
            प्रत्यागम;
        }
        समुच्चयः_Ipp_Error_स्थितिः();
#यदि_अन्तः
    }
    सततम्_Dft(c, src, 1, dst, 1, complex_output != 0);
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
CCSIDFT(ध्रुव OcvDftOptions & c, ध्रुव T* src, T* dst)
{
    पूर्णाङ्क complex_निवेशः = c.isComplex;

    यदि( complex_निवेशः )
        CV_Assert( src != dst );
    यदि( c.useIpp )
    {
#यदि_परिभाषितम् USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        ध्रुव T* ipp_src = src;
        T save_s1 = 0;
        यदि( complex_निवेशः )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        Ipp_स्थितिः स्थितिः = ipps_DFTInv_Pack_गन्तव्यम्_R( ipp_src, dst, c.ipp_spec, c.ipp_work );
        यदि( complex_निवेशः )
            ((T*)src)[1] = save_s1;
        यदि( स्थितिः >= 0 )
        {
            CV_IMPL_सङ्कलय(CV_IMPL_IPP);
            प्रत्यागम;
        }
        समुच्चयः_Ipp_Error_स्थितिः();
#यदि_अन्तः
    }
    सततम्_Dft(c, src, 1, dst, 1, complex_निवेशः != 0);
}

स्थिर शून्य
CopyColumn( ध्रुव uchar* _src, size_t src_चरणः,
            uchar* _dst, size_t dst_चरणः,
            पूर्णाङ्क len, size_t elem_आकार_2 )
{
    पूर्णाङ्क i, t0, t1;
    ध्रुव पूर्णाङ्क* src = (ध्रुव पूर्णाङ्क*)_src;
    पूर्णाङ्क* dst = (पूर्णाङ्क*)_dst;
    src_चरणः /= परिमाण(src[0]);
    dst_चरणः /= परिमाण(dst[0]);

    यदि( elem_आकार_2 == परिमाण(पूर्णाङ्क) )
    {
        प्रति( i = 0; i < len; i++, src += src_चरणः, dst += dst_चरणः )
            dst[0] = src[0];
    }
    अन्यथा यदि( elem_आकार_2 == परिमाण(पूर्णाङ्क)*2 )
    {
        प्रति( i = 0; i < len; i++, src += src_चरणः, dst += dst_चरणः )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    अन्यथा यदि( elem_आकार_2 == परिमाण(पूर्णाङ्क)*4 )
    {
        प्रति( i = 0; i < len; i++, src += src_चरणः, dst += dst_चरणः )
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
प्रतिरूपम्<प्रकारनाम T> स्थिर अन्तःस्थ शून्य
copyFromColumns_( ध्रुव uchar* src, size_t src_चरणः, T* dst, पूर्णाङ्क ncols, पूर्णाङ्क len )
{
    प्रति( पूर्णाङ्क i = 0; i < len; i++, src += src_चरणः )
    {
        ध्रुव T* s = (ध्रुव T*)src;
        प्रति( पूर्णाङ्क k = 0; k < ncols; k++ )
            dst[(size_t)k*len + i] = s[k];
    }
}

स्थिर शून्य
CopyFromColumns( ध्रुव uchar* src, size_t src_चरणः, uchar* dst, पूर्णाङ्क ncols, पूर्णाङ्क len, size_t elem_आकार_2 )
{
    यदि( elem_आकार_2 == परिमाण(पूर्णाङ्क) )
        copyFromColumns_(src, src_चरणः, (पूर्णाङ्क*)dst, ncols, len);
    अन्यथा यदि( elem_आकार_2 == परिमाण(int64) )
        copyFromColumns_(src, src_चरणः, (int64*)dst, ncols, len);
    अन्यथा
    {
        CV_Assert( elem_आकार_2 == परिमाण(int64)*2 );
        copyFromColumns_(src, src_चरणः, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
प्रतिरूपम्<प्रकारनाम T> स्थिर अन्तःस्थ शून्य
copy_गन्तव्यम्_Columns( ध्रुव T* src, uchar* dst, size_t dst_चरणः, पूर्णाङ्क ncols, पूर्णाङ्क len )
{
    प्रति( पूर्णाङ्क i = 0; i < len; i++, dst += dst_चरणः )
    {
        T* d = (T*)dst;
        प्रति( पूर्णाङ्क k = 0; k < ncols; k++ )
            d[k] = src[(size_t)k*len + i];
    }
}

स्थिर शून्य
Copy_गन्तव्यम्_Columns( ध्रुव uchar* src, uchar* dst, size_t dst_चरणः, पूर्णाङ्क ncols, पूर्णाङ्क len, size_t elem_आकार_2 )
{
    यदि( elem_आकार_2 == परिमाण(पूर्णाङ्क) )
        copy_गन्तव्यम्_Columns((ध्रुव पूर्णाङ्क*)src, dst, dst_चरणः, ncols, len);
    अन्यथा यदि( elem_आकार_2 == परिमाण(int64) )
        copy_गन्तव्यम्_Columns((ध्रुव int64*)src, dst, dst_चरणः, ncols, len);
    अन्यथा
    {
        CV_Assert( elem_आकार_2 == परिमाण(int64)*2 );
        copy_गन्तव्यम्_Columns((ध्रुव Vec<int64, 2>*)src, dst, dst_चरणः, ncols, len);
    }
}

स्थिर शून्य
ExpandCCS( uchar* _ptr, पूर्णाङ्क n, पूर्णाङ्क elem_आकार_2 )
{
    पूर्णाङ्क i;
    यदि( elem_आकार_2 == (पूर्णाङ्क)परिमाण(भिन्न) )
    {
        भिन्न* p = (भिन्न*)_ptr;
        प्रति( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        यदि( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        प्रति( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    अन्यथा
    {
        द्विगुण* p = (द्विगुण*)_ptr;
        प्रति( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        यदि( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        प्रति( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

प्रतिरूपम्<प्रकारनाम T, शून्य (*fn)(ध्रुव OcvDftOptions&, ध्रुव T*, T*)>
स्थिर शून्य dftWrap(ध्रुव OcvDftOptions & c, ध्रुव शून्य* src, शून्य* dst)
{
    fn(c, (ध्रुव T*)src, (T*)dst);
}

}

#यदि_परिभाषितम् USE_IPP_DFT
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL* Ipp_DFTGet_आकार_Func)(पूर्णाङ्क, पूर्णाङ्क, IppHintAlgorithm, पूर्णाङ्क*, पूर्णाङ्क*, पूर्णाङ्क*);
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL* IppDFTInitFunc)(पूर्णाङ्क, पूर्णाङ्क, IppHintAlgorithm, शून्य*, uchar*);

प्रतिरूपम्<प्रकारनाम SpecType, Ipp_स्थितिः (CV_STDCALL *init_fn)(पूर्णाङ्क, पूर्णाङ्क, IppHintAlgorithm, SpecType*, Ipp8u*)>
स्थिर Ipp_स्थितिः CV_STDCALL ippDFTInitWrap(पूर्णाङ्क n, पूर्णाङ्क flags, IppHintAlgorithm hint, शून्य* spec, Ipp8u* initbuf)
{
    प्रत्यागम init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#यदि_अन्तः

नामाकाश cv
{
#यदि defined USE_IPP_DFT

प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL* ippiDFT_C_Func)(ध्रुव Ipp32fc*, पूर्णाङ्क, Ipp32fc*, पूर्णाङ्क, ध्रुव IppiDFTSpec_C_32fc*, Ipp8u*);
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL* ippiDFT_R_Func)(ध्रुव Ipp32f* , पूर्णाङ्क, Ipp32f* , पूर्णाङ्क, ध्रुव IppiDFTSpec_R_32f* , Ipp8u*);

प्रतिरूपम् <प्रकारनाम Dft>
वर्ग Dft_C_IPPLoop_Invoker : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:

    Dft_C_IPPLoop_Invoker(ध्रुव uchar * _src, size_t src_चरणः_2, uchar * _dst, size_t dst_चरणः_2, पूर्णाङ्क विस्तारः_3,
                          ध्रुव Dft& _ippidft, पूर्णाङ्क _norm_flag, तार्किक *_ok) :
        ParallelLoopBody(),
        src(_src), src_चरणः(src_चरणः_2), dst(_dst), dst_चरणः(dst_चरणः_2), विस्तारः(विस्तारः_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = सत्य;
    }

    आभास शून्य संक्रियकः()(ध्रुव Range& range) ध्रुव CV_OVERRIDE
    {
        Ipp_स्थितिः स्थितिः;
        Ipp8u* p_बफर = 0;
        Ipp8u* pMemInit= 0;
        पूर्णाङ्क आकार_बफर=0;
        पूर्णाङ्क आकार_Spec=0;
        पूर्णाङ्क आकार_Init=0;

        Ippi_आकार src_Roi_आकार = {विस्तारः, 1};

        स्थितिः = ippi_DFTGet_आकार_C_32fc(src_Roi_आकार, norm_flag, ippAlgHintNone, &आकार_Spec, &आकार_Init, &आकार_बफर );
        यदि ( स्थितिः < 0 )
        {
            *ok = असत्य;
            प्रत्यागम;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( आकार_Spec );

        यदि ( आकार_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( आकार_Init );

        यदि ( आकार_बफर > 0 )
            p_बफर = (Ipp8u*)CV_IPP_MALLOC( आकार_बफर );

        स्थितिः = ippiDFTInit_C_32fc( src_Roi_आकार, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        यदि ( आकार_Init > 0 )
            ippFree( pMemInit );

        यदि ( स्थितिः < 0 )
        {
            ippFree( pDFTSpec );
            यदि ( आकार_बफर > 0 )
                ippFree( p_बफर );
            *ok = असत्य;
            प्रत्यागम;
        }

        प्रति( पूर्णाङ्क i = range.start; i < range.end; ++i)
            यदि(!ippidft((Ipp32fc*)(src + src_चरणः * i), src_चरणः, (Ipp32fc*)(dst + dst_चरणः * i), dst_चरणः,
                        pDFTSpec, (Ipp8u*)p_बफर))
            {
                *ok = असत्य;
            }

        यदि ( आकार_बफर > 0 )
            ippFree( p_बफर );

        ippFree( pDFTSpec );
        CV_IMPL_सङ्कलय(CV_IMPL_IPP|CV_IMPL_MT);
    }

गुप्त:
    ध्रुव uchar * src;
    size_t src_चरणः;
    uchar * dst;
    size_t dst_चरणः;
    पूर्णाङ्क विस्तारः;
    ध्रुव Dft& ippidft;
    पूर्णाङ्क norm_flag;
    तार्किक *ok;

    ध्रुव Dft_C_IPPLoop_Invoker& संक्रियकः= (ध्रुव Dft_C_IPPLoop_Invoker&);
};

प्रतिरूपम् <प्रकारनाम Dft>
वर्ग Dft_R_IPPLoop_Invoker : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:

    Dft_R_IPPLoop_Invoker(ध्रुव uchar * _src, size_t src_चरणः_2, uchar * _dst, size_t dst_चरणः_2, पूर्णाङ्क विस्तारः_3,
                          ध्रुव Dft& _ippidft, पूर्णाङ्क _norm_flag, तार्किक *_ok) :
        ParallelLoopBody(),
        src(_src), src_चरणः(src_चरणः_2), dst(_dst), dst_चरणः(dst_चरणः_2), विस्तारः(विस्तारः_3),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = सत्य;
    }

    आभास शून्य संक्रियकः()(ध्रुव Range& range) ध्रुव CV_OVERRIDE
    {
        Ipp_स्थितिः स्थितिः;
        Ipp8u* p_बफर = 0;
        Ipp8u* pMemInit= 0;
        पूर्णाङ्क आकार_बफर=0;
        पूर्णाङ्क आकार_Spec=0;
        पूर्णाङ्क आकार_Init=0;

        Ippi_आकार src_Roi_आकार = {विस्तारः, 1};

        स्थितिः = ippi_DFTGet_आकार_R_32f(src_Roi_आकार, norm_flag, ippAlgHintNone, &आकार_Spec, &आकार_Init, &आकार_बफर );
        यदि ( स्थितिः < 0 )
        {
            *ok = असत्य;
            प्रत्यागम;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( आकार_Spec );

        यदि ( आकार_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( आकार_Init );

        यदि ( आकार_बफर > 0 )
            p_बफर = (Ipp8u*)CV_IPP_MALLOC( आकार_बफर );

        स्थितिः = ippiDFTInit_R_32f( src_Roi_आकार, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        यदि ( आकार_Init > 0 )
            ippFree( pMemInit );

        यदि ( स्थितिः < 0 )
        {
            ippFree( pDFTSpec );
            यदि ( आकार_बफर > 0 )
                ippFree( p_बफर );
            *ok = असत्य;
            प्रत्यागम;
        }

        प्रति( पूर्णाङ्क i = range.start; i < range.end; ++i)
            यदि(!ippidft((भिन्न*)(src + src_चरणः * i), src_चरणः, (भिन्न*)(dst + dst_चरणः * i), dst_चरणः,
                        pDFTSpec, (Ipp8u*)p_बफर))
            {
                *ok = असत्य;
            }

        यदि ( आकार_बफर > 0 )
            ippFree( p_बफर );

        ippFree( pDFTSpec );
        CV_IMPL_सङ्कलय(CV_IMPL_IPP|CV_IMPL_MT);
    }

गुप्त:
    ध्रुव uchar * src;
    size_t src_चरणः;
    uchar * dst;
    size_t dst_चरणः;
    पूर्णाङ्क विस्तारः;
    ध्रुव Dft& ippidft;
    पूर्णाङ्क norm_flag;
    तार्किक *ok;

    ध्रुव Dft_R_IPPLoop_Invoker& संक्रियकः= (ध्रुव Dft_R_IPPLoop_Invoker&);
};

प्रतिरूपम् <प्रकारनाम Dft>
तार्किक Dft_C_IPPLoop(ध्रुव uchar * src, size_t src_चरणः, uchar * dst, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव Dft& ippidft, पूर्णाङ्क norm_flag)
{
    तार्किक ok;
    parallel_for_(Range(0, height), Dft_C_IPPLoop_Invoker<Dft>(src, src_चरणः, dst, dst_चरणः, विस्तारः, ippidft, norm_flag, &ok), (विस्तारः * height)/(द्विगुण)(1<<16) );
    प्रत्यागम ok;
}

प्रतिरूपम् <प्रकारनाम Dft>
तार्किक Dft_R_IPPLoop(ध्रुव uchar * src, size_t src_चरणः, uchar * dst, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव Dft& ippidft, पूर्णाङ्क norm_flag)
{
    तार्किक ok;
    parallel_for_(Range(0, height), Dft_R_IPPLoop_Invoker<Dft>(src, src_चरणः, dst, dst_चरणः, विस्तारः, ippidft, norm_flag, &ok), (विस्तारः * height)/(द्विगुण)(1<<16) );
    प्रत्यागम ok;
}

संरचना IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    तार्किक संक्रियकः()(ध्रुव Ipp32fc* src, size_t src_चरणः_3, Ipp32fc* dst, size_t dst_चरणः_3, ध्रुव IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* p_बफर) ध्रुव
    {
        प्रत्यागम ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<पूर्णाङ्क>(src_चरणः_3), dst, static_cast<पूर्णाङ्क>(dst_चरणः_3), pDFTSpec, p_बफर) >= 0 : असत्य;
    }
गुप्त:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

संरचना IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippi_DFT_Pack_गन्तव्यम्_R_32f_C1_R(_func){}

    तार्किक संक्रियकः()(ध्रुव Ipp32f* src, size_t src_चरणः_3, Ipp32f* dst, size_t dst_चरणः_3, ध्रुव IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* p_बफर) ध्रुव
    {
        प्रत्यागम ippi_DFT_Pack_गन्तव्यम्_R_32f_C1_R ? CV_INSTRUMENT_FUN_IPP(ippi_DFT_Pack_गन्तव्यम्_R_32f_C1_R, src, static_cast<पूर्णाङ्क>(src_चरणः_3), dst, static_cast<पूर्णाङ्क>(dst_चरणः_3), pDFTSpec, p_बफर) >= 0 : असत्य;
    }
गुप्त:
    ippiDFT_R_Func ippi_DFT_Pack_गन्तव्यम्_R_32f_C1_R;
};

स्थिर तार्किक ippi_DFT_C_32F(ध्रुव uchar * src, size_t src_चरणः, uchar * dst, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, तार्किक inv, पूर्णाङ्क norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_स्थितिः स्थितिः;
    Ipp8u* p_बफर = 0;
    Ipp8u* pMemInit= 0;
    पूर्णाङ्क आकार_बफर=0;
    पूर्णाङ्क आकार_Spec=0;
    पूर्णाङ्क आकार_Init=0;

    Ippi_आकार src_Roi_आकार = {विस्तारः, height};

    स्थितिः = ippi_DFTGet_आकार_C_32fc(src_Roi_आकार, norm_flag, ippAlgHintNone, &आकार_Spec, &आकार_Init, &आकार_बफर );
    यदि ( स्थितिः < 0 )
        प्रत्यागम असत्य;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( आकार_Spec );

    यदि ( आकार_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( आकार_Init );

    यदि ( आकार_बफर > 0 )
        p_बफर = (Ipp8u*)CV_IPP_MALLOC( आकार_बफर );

    स्थितिः = ippiDFTInit_C_32fc( src_Roi_आकार, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    यदि ( आकार_Init > 0 )
        ippFree( pMemInit );

    यदि ( स्थितिः < 0 )
    {
        ippFree( pDFTSpec );
        यदि ( आकार_बफर > 0 )
            ippFree( p_बफर );
        प्रत्यागम असत्य;
    }

    यदि (!inv)
        स्थितिः = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<पूर्णाङ्क>(src_चरणः), (Ipp32fc*)dst, static_cast<पूर्णाङ्क>(dst_चरणः), pDFTSpec, p_बफर);
    अन्यथा
        स्थितिः = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<पूर्णाङ्क>(src_चरणः), (Ipp32fc*)dst, static_cast<पूर्णाङ्क>(dst_चरणः), pDFTSpec, p_बफर);

    यदि ( आकार_बफर > 0 )
        ippFree( p_बफर );

    ippFree( pDFTSpec );

    यदि(स्थितिः >= 0)
    {
        CV_IMPL_सङ्कलय(CV_IMPL_IPP);
        प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

स्थिर तार्किक ippi_DFT_R_32F(ध्रुव uchar * src, size_t src_चरणः, uchar * dst, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, तार्किक inv, पूर्णाङ्क norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    Ipp_स्थितिः स्थितिः;
    Ipp8u* p_बफर = 0;
    Ipp8u* pMemInit= 0;
    पूर्णाङ्क आकार_बफर=0;
    पूर्णाङ्क आकार_Spec=0;
    पूर्णाङ्क आकार_Init=0;

    Ippi_आकार src_Roi_आकार = {विस्तारः, height};

    स्थितिः = ippi_DFTGet_आकार_R_32f(src_Roi_आकार, norm_flag, ippAlgHintNone, &आकार_Spec, &आकार_Init, &आकार_बफर );
    यदि ( स्थितिः < 0 )
        प्रत्यागम असत्य;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( आकार_Spec );

    यदि ( आकार_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( आकार_Init );

    यदि ( आकार_बफर > 0 )
        p_बफर = (Ipp8u*)CV_IPP_MALLOC( आकार_बफर );

    स्थितिः = ippiDFTInit_R_32f( src_Roi_आकार, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    यदि ( आकार_Init > 0 )
        ippFree( pMemInit );

    यदि ( स्थितिः < 0 )
    {
        ippFree( pDFTSpec );
        यदि ( आकार_बफर > 0 )
            ippFree( p_बफर );
        प्रत्यागम असत्य;
    }

    यदि (!inv)
        स्थितिः = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (भिन्न*)src, static_cast<पूर्णाङ्क>(src_चरणः), (भिन्न*)dst, static_cast<पूर्णाङ्क>(dst_चरणः), pDFTSpec, p_बफर);
    अन्यथा
        स्थितिः = CV_INSTRUMENT_FUN_IPP(ippi_DFTInv_Pack_गन्तव्यम्_R_32f_C1_R, (भिन्न*)src, static_cast<पूर्णाङ्क>(src_चरणः), (भिन्न*)dst, static_cast<पूर्णाङ्क>(dst_चरणः), pDFTSpec, p_बफर);

    यदि ( आकार_बफर > 0 )
        ippFree( p_बफर );

    ippFree( pDFTSpec );

    यदि(स्थितिः >= 0)
    {
        CV_IMPL_सङ्कलय(CV_IMPL_IPP);
        प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

#यदि_अन्तः
}

#यदि_परिभाषितम् HAVE_OPENCL

नामाकाश cv
{

गणना FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

संरचना OCL_FftPlan
{
गुप्त:
    UMat twiddles;
    String buildOptions;
    पूर्णाङ्क thread_गणनम्;
    पूर्णाङ्क dft_आकार;
    पूर्णाङ्क dft_गभीरता;
    तार्किक स्थितिः;

सार्वजनिक:
    OCL_FftPlan(पूर्णाङ्क आकार_4, पूर्णाङ्क गभीरता_2) : dft_आकार(आकार_4), dft_गभीरता(गभीरता_2), स्थितिः(सत्य)
    {
        CV_Assert( dft_गभीरता == CV_32F || dft_गभीरता == CV_64F );

        पूर्णाङ्क min_radix;
        std::vector<पूर्णाङ्क> radixes, खण्डानि;
        ocl_getRadixes(dft_आकार, radixes, खण्डानि, min_radix);
        thread_गणनम् = dft_आकार / min_radix;

        यदि (thread_गणनम् > (पूर्णाङ्क) ocl::Device::getDefault().max_Work_गणः_आकार())
        {
            स्थितिः = असत्य;
            प्रत्यागम;
        }

        // generate string with radix calls
        String radix_processing;
        पूर्णाङ्क n = 1, twiddle_आकार = 0;
        प्रति (size_t i=0; i<radixes.size(); i++)
        {
            पूर्णाङ्क radix = radixes[i], खण्डम्_2 = खण्डानि[i];
            यदि (खण्डम्_2 > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, खण्डम्_2, twiddle_आकार, n, dft_आकार/radix);
            अन्यथा
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_आकार, n, dft_आकार/radix);
            twiddle_आकार += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_आकार, CV_MAKE_TYPE(dft_गभीरता, 2));
        यदि (dft_गभीरता == CV_32F)
            fill_Radix_सारणी<भिन्न>(twiddles, radixes);
        अन्यथा
            fill_Radix_सारणी<द्विगुण>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_आकार, min_radix, ocl::type_गन्तव्यम्_Str(dft_गभीरता), ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(dft_गभीरता, 2)),
                              dft_गभीरता == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    तार्किक पङ्क्तौ_निक्षिप_Transform(निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क अङ्कः_dfts, पूर्णाङ्क flags, पूर्णाङ्क fftType, तार्किक पङ्क्तिकाः = सत्य) ध्रुव
    {
        यदि (!स्थितिः)
            प्रत्यागम असत्य;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        size_t globalsize[2];
        size_t localsize[2];
        String kernel_name;

        तार्किक is1d = (flags & DFT_पङ्क्तिकाः) != 0 || अङ्कः_dfts == 1;
        तार्किक inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        यदि (पङ्क्तिकाः)
        {
            globalsize[0] = thread_गणनम्; globalsize[1] = src.पङ्क्तिकाः;
            localsize[0] = thread_गणनम्; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            यदि ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        अन्यथा
        {
            globalsize[0] = अङ्कः_dfts; globalsize[1] = thread_गणनम्;
            localsize[0] = 1; localsize[1] = thread_गणनम्;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            यदि (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        यदि (!inv)
        {
            यदि ((is1d && src.channels() == 1) || (पङ्क्तिकाः && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        अन्यथा
        {
            यदि (पङ्क्तिकाः && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            यदि (dst.स्तम्भसङ्ख्या % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        यदि (k.empty())
            प्रत्यागम असत्य;

        k.तर्काः(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::Read_Only_No_आकार(twiddles), thread_गणनम्, अङ्कः_dfts);
        प्रत्यागम k.सततम्(2, globalsize, localsize, असत्य);
    }

गुप्त:
    स्थिर शून्य ocl_getRadixes(पूर्णाङ्क स्तम्भसङ्ख्या, std::vector<पूर्णाङ्क>& radixes, std::vector<पूर्णाङ्क>& खण्डानि, पूर्णाङ्क& min_radix)
    {
        पूर्णाङ्क factors[34];
        पूर्णाङ्क nf = DFTFactorize(स्तम्भसङ्ख्या, factors);

        पूर्णाङ्क n = 1;
        पूर्णाङ्क गुणकः_सूचकांक = 0;
        min_radix = अधिकतम_पूर्णाङ्कः;

        // 2^n transforms
        यदि ((factors[गुणकः_सूचकांक] & 1) == 0)
        {
            प्रति( ; n < factors[गुणकः_सूचकांक];)
            {
                पूर्णाङ्क radix = 2, खण्डम्_2 = 1;
                यदि (8*n <= factors[0])
                    radix = 8;
                अन्यथा यदि (4*n <= factors[0])
                {
                    radix = 4;
                    यदि (स्तम्भसङ्ख्या % 12 == 0)
                        खण्डम्_2 = 3;
                    अन्यथा यदि (स्तम्भसङ्ख्या % 8 == 0)
                        खण्डम्_2 = 2;
                }
                अन्यथा
                {
                    यदि (स्तम्भसङ्ख्या % 10 == 0)
                        खण्डम्_2 = 5;
                    अन्यथा यदि (स्तम्भसङ्ख्या % 8 == 0)
                        खण्डम्_2 = 4;
                    अन्यथा यदि (स्तम्भसङ्ख्या % 6 == 0)
                        खण्डम्_2 = 3;
                    अन्यथा यदि (स्तम्भसङ्ख्या % 4 == 0)
                        खण्डम्_2 = 2;
                }

                radixes.push_back(radix);
                खण्डानि.push_back(खण्डम्_2);
                min_radix = min(min_radix, खण्डम्_2*radix);
                n *= radix;
            }
            गुणकः_सूचकांक++;
        }

        // all the other transforms
        प्रति( ; गुणकः_सूचकांक < nf; गुणकः_सूचकांक++)
        {
            पूर्णाङ्क radix = factors[गुणकः_सूचकांक], खण्डम्_2 = 1;
            यदि (radix == 3)
            {
                यदि (स्तम्भसङ्ख्या % 12 == 0)
                    खण्डम्_2 = 4;
                अन्यथा यदि (स्तम्भसङ्ख्या % 9 == 0)
                    खण्डम्_2 = 3;
                अन्यथा यदि (स्तम्भसङ्ख्या % 6 == 0)
                    खण्डम्_2 = 2;
            }
            अन्यथा यदि (radix == 5)
            {
                यदि (स्तम्भसङ्ख्या % 10 == 0)
                    खण्डम्_2 = 2;
            }
            radixes.push_back(radix);
            खण्डानि.push_back(खण्डम्_2);
            min_radix = min(min_radix, खण्डम्_2*radix);
        }
    }

    प्रतिरूपम् <प्रकारनाम T>
    स्थिर शून्य fill_Radix_सारणी(UMat twiddles, ध्रुव std::vector<पूर्णाङ्क>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        पूर्णाङ्क ptr_सूचकांक = 0;

        पूर्णाङ्क n = 1;
        प्रति (size_t i=0; i<radixes.size(); i++)
        {
            पूर्णाङ्क radix = radixes[i];
            n *= radix;

            प्रति (पूर्णाङ्क j=1; j<radix; j++)
            {
                द्विगुण theta = -CV_2PI*j/n;

                प्रति (पूर्णाङ्क k=0; k<(n/radix); k++)
                {
                    ptr[ptr_सूचकांक++] = (T) कोटिज्या(k*theta);
                    ptr[ptr_सूचकांक++] = (T) ज्या(k*theta);
                }
            }
        }
    }
};

वर्ग OCL_FftPlanCache
{
सार्वजनिक:
    स्थिर OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, नूतन OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(पूर्णाङ्क dft_आकार, पूर्णाङ्क गभीरता)
    {
        पूर्णाङ्क कुञ्चिका = (dft_आकार << 16) | (गभीरता & 0xFFFF);
        std::map<पूर्णाङ्क, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(कुञ्चिका);
        यदि (f != planStorage.end())
        {
            प्रत्यागम f->second;
        }
        अन्यथा
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(नूतन OCL_FftPlan(dft_आकार, गभीरता));
            planStorage[कुञ्चिका] = newPlan;
            प्रत्यागम newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.clear();
    }

रक्षित:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<पूर्णाङ्क, Ptr<OCL_FftPlan> > planStorage;
};

स्थिर तार्किक ocl_dft_पङ्क्तिकाः(निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क nonzero_पङ्क्तिकाः, पूर्णाङ्क flags, पूर्णाङ्क fftType)
{
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.स्तम्भसङ्ख्या(), गभीरता);
    प्रत्यागम plan->पङ्क्तौ_निक्षिप_Transform(_src, _dst, nonzero_पङ्क्तिकाः, flags, fftType, सत्य);
}

स्थिर तार्किक ocl_dft_स्तम्भसङ्ख्या(निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क nonzero_स्तम्भसङ्ख्या, पूर्णाङ्क flags, पूर्णाङ्क fftType)
{
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.पङ्क्तिकाः(), गभीरता);
    प्रत्यागम plan->पङ्क्तौ_निक्षिप_Transform(_src, _dst, nonzero_स्तम्भसङ्ख्या, flags, fftType, असत्य);
}

अन्तःस्थ FftType determineFFTType(तार्किक real_निवेशः, तार्किक complex_निवेशः, तार्किक real_output, तार्किक complex_output, तार्किक inv)
{
    // output format is not specified
    यदि (!real_output && !complex_output)
        complex_output = सत्य;

    // input or output format is ambiguous
    यदि (real_निवेशः == complex_निवेशः || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType परिणाम = real_निवेशः ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    यदि (परिणाम == C2R && !inv)
        परिणाम = C2C;

    // Inverse CCS to Complex not supported
    यदि (परिणाम == R2C && inv)
        परिणाम = R2R;

    प्रत्यागम परिणाम;
}

स्थिर तार्किक ocl_dft(निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क flags, पूर्णाङ्क nonzero_पङ्क्तिकाः)
{
    पूर्णाङ्क type = _src.type(), cn = CV_MAT_CN(type), गभीरता = CV_MAT_गभीरता(type);
    आकार ssize = _src.size();
    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    यदि (!(cn == 1 || cn == 2)
        || !(गभीरता == CV_32F || (गभीरता == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        प्रत्यागम असत्य;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    यदि (ssize.area() != getOptimalDFTSize(ssize.area()))
        प्रत्यागम असत्य;

    UMat src = _src.getUMat();
    तार्किक inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    यदि( nonzero_पङ्क्तिकाः <= 0 || nonzero_पङ्क्तिकाः > _src.पङ्क्तिकाः() )
        nonzero_पङ्क्तिकाः = _src.पङ्क्तिकाः();
    तार्किक is1d = (flags & DFT_पङ्क्तिकाः) != 0 || nonzero_पङ्क्तिकाः == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    यदि (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.create_Same_आकार(src, CV_MAKETYPE(गभीरता, 2));
        output = _dst.getUMat();
    }
    अन्यथा
    {
        // real output
        यदि (is1d)
        {
            _dst.create_Same_आकार(src, CV_MAKETYPE(गभीरता, 1));
            output = _dst.getUMat();
        }
        अन्यथा
        {
            _dst.create_Same_आकार(src, CV_MAKETYPE(गभीरता, 1));
            output.create(src.size, CV_MAKETYPE(गभीरता, 2));
        }
    }

    तार्किक परिणाम = असत्य;
    यदि (!inv)
    {
        पूर्णाङ्क nonzero_स्तम्भसङ्ख्या = fftType == R2R ? output.स्तम्भसङ्ख्या/2 + 1 : output.स्तम्भसङ्ख्या;
        परिणाम = ocl_dft_पङ्क्तिकाः(src, output, nonzero_पङ्क्तिकाः, flags, fftType);
        यदि (!is1d)
            परिणाम = परिणाम && ocl_dft_स्तम्भसङ्ख्या(output, _dst, nonzero_स्तम्भसङ्ख्या, flags, fftType);
    }
    अन्यथा
    {
        यदि (fftType == C2C)
        {
            // complex output
            परिणाम = ocl_dft_पङ्क्तिकाः(src, output, nonzero_पङ्क्तिकाः, flags, fftType);
            यदि (!is1d)
                परिणाम = परिणाम && ocl_dft_स्तम्भसङ्ख्या(output, output, output.स्तम्भसङ्ख्या, flags, fftType);
        }
        अन्यथा
        {
            यदि (is1d)
            {
                परिणाम = ocl_dft_पङ्क्तिकाः(src, output, nonzero_पङ्क्तिकाः, flags, fftType);
            }
            अन्यथा
            {
                पूर्णाङ्क nonzero_स्तम्भसङ्ख्या = src.स्तम्भसङ्ख्या/2 + 1;
                परिणाम = ocl_dft_स्तम्भसङ्ख्या(src, output, nonzero_स्तम्भसङ्ख्या, flags, fftType);
                परिणाम = परिणाम && ocl_dft_पङ्क्तिकाः(output, _dst, nonzero_पङ्क्तिकाः, flags, fftType);
            }
        }
    }
    प्रत्यागम परिणाम;
}

} // namespace cv;

#यदि_अन्तः

#यदि_परिभाषितम् HAVE_CLAMDFFT

नामाकाश cv {

#परिभाषय CLAMDDFT_Assert(func) \
    { \
        clfft_स्थितिः s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

वर्ग PlanCache
{
    संरचना FftPlan
    {
        FftPlan(ध्रुव आकार & dft_आकार_2, पूर्णाङ्क src_चरणः_2, पूर्णाङ्क dst_चरणः_2, तार्किक _doubleFP, तार्किक _inplace, पूर्णाङ्क _flags, FftType _fftType) :
            dft_आकार(dft_आकार_2), src_चरणः(src_चरणः_2), dst_चरणः(dst_चरणः_2),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            तार्किक dft_inverse = (flags & DFT_INVERSE) != 0;
            तार्किक dft_scale = (flags & DFT_SCALE) != 0;
            तार्किक dft_पङ्क्तिकाः = (flags & DFT_पङ्क्तिकाः) != 0;

            clfftLayout अन्तर्_Layout = CLFFT_REAL, बहिर्गमनम्_Layout = CLFFT_REAL;
            clfftDim dim = dft_आकार.height == 1 || dft_पङ्क्तिकाः ? CLFFT_1D : CLFFT_2D;

            size_t batch_आकार = dft_पङ्क्तिकाः ? dft_आकार.height : 1;
            size_t cl_Lengths_अन्तर्[3] = { (size_t)dft_आकार.विस्तारः, dft_पङ्क्तिकाः ? 1 : (size_t)dft_आकार.height, 1 };
            size_t cl_Strides_अन्तर्[3] = { 1, 1, 1 };
            size_t cl_Strides_बहिर्गमनम्[3]  = { 1, 1, 1 };
            पूर्णाङ्क elem_आकार = doubleFP ? परिमाण(द्विगुण) : परिमाण(भिन्न);

            विकल्प (fftType)
            {
            पक्ष C2C:
                अन्तर्_Layout = CLFFT_COMPLEX_INTERLEAVED;
                बहिर्गमनम्_Layout = CLFFT_COMPLEX_INTERLEAVED;
                cl_Strides_अन्तर्[1] = src_चरणः / (elem_आकार << 1);
                cl_Strides_बहिर्गमनम्[1] = dst_चरणः / (elem_आकार << 1);
                भङ्ग;
            पक्ष R2C:
                अन्तर्_Layout = CLFFT_REAL;
                बहिर्गमनम्_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                cl_Strides_अन्तर्[1] = src_चरणः / elem_आकार;
                cl_Strides_बहिर्गमनम्[1] = dst_चरणः / (elem_आकार << 1);
                भङ्ग;
            पक्ष C2R:
                अन्तर्_Layout = CLFFT_HERMITIAN_INTERLEAVED;
                बहिर्गमनम्_Layout = CLFFT_REAL;
                cl_Strides_अन्तर्[1] = src_चरणः / (elem_आकार << 1);
                cl_Strides_बहिर्गमनम्[1] = dst_चरणः / elem_आकार;
                भङ्ग;
            पक्ष R2R:
            स्वभाव:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                भङ्ग;
            }

            cl_Strides_अन्तर्[2] = dft_पङ्क्तिकाः ? cl_Strides_अन्तर्[1] : dft_आकार.विस्तारः * cl_Strides_अन्तर्[1];
            cl_Strides_बहिर्गमनम्[2] = dft_पङ्क्तिकाः ? cl_Strides_बहिर्गमनम्[1] : dft_आकार.विस्तारः * cl_Strides_बहिर्गमनम्[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, cl_Lengths_अन्तर्))

            // setting plan properties
            CLAMDDFT_Assert(clfft_समुच्चयः_Plan_Precision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfft_समुच्चयः_परिणाम_Location(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfft_समुच्चयः_Layout(plHandle, अन्तर्_Layout, बहिर्गमनम्_Layout))
            CLAMDDFT_Assert(clfft_समुच्चयः_Plan_Batch_आकार(plHandle, batch_आकार))
            CLAMDDFT_Assert(clfft_समुच्चयः_Plan_अन्तर्_Stride(plHandle, dim, cl_Strides_अन्तर्))
            CLAMDDFT_Assert(clfft_समुच्चयः_Plan_बहिर्गमनम्_Stride(plHandle, dim, cl_Strides_बहिर्गमनम्))
            CLAMDDFT_Assert(clfft_समुच्चयः_Plan_Distance(plHandle, cl_Strides_अन्तर्[dim], cl_Strides_बहिर्गमनम्[dim]))

            भिन्न scale = dft_scale ? 1.0f / (dft_पङ्क्तिकाः ? dft_आकार.विस्तारः : dft_आकार.area()) : 1.0f;
            CLAMDDFT_Assert(clfft_समुच्चयः_Plan_Scale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &queue, रिक्त, रिक्त))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        मित्रम् वर्ग PlanCache;

    गुप्त:
        आकार dft_आकार;
        पूर्णाङ्क src_चरणः, dst_चरणः;
        तार्किक doubleFP;
        तार्किक inplace;
        पूर्णाङ्क flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

सार्वजनिक:
    स्थिर PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, नूतन PlanCache())
    }

    clfftPlanHandle getPlanHandle(ध्रुव आकार & dft_आकार, पूर्णाङ्क src_चरणः, पूर्णाङ्क dst_चरणः, तार्किक doubleFP,
                                  तार्किक inplace, पूर्णाङ्क flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        प्रति (size_t i = 0, size = planStorage.size(); i < size; ++i)
        {
            ध्रुव FftPlan * ध्रुव plan = planStorage[i];

            यदि (plan->dft_आकार == dft_आकार &&
                plan->flags == flags &&
                plan->src_चरणः == src_चरणः &&
                plan->dst_चरणः == dst_चरणः &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                यदि (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    भङ्ग;
                }

                प्रत्यागम plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(नूतन FftPlan(dft_आकार, src_चरणः, dst_चरणः, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        प्रत्यागम newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.clear();
    }

रक्षित:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

बाह्य "C" {

स्थिर शून्य CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, शून्य *p)
{
    UMat_दत्त * u = (UMat_दत्त *)p;

    यदि( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

स्थिर तार्किक ocl_dft_amdfft(निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क flags)
{
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type);
    आकार ssize = _src.size();

    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    यदि ( (!doubleSupport && गभीरता == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        प्रत्यागम असत्य;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    यदि (ssize.area() != getOptimalDFTSize(ssize.area()))
        प्रत्यागम असत्य;

    पूर्णाङ्क dst_complex_निवेशः = cn == 2 ? 1 : 0;
    तार्किक dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    पूर्णाङ्क dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    तार्किक dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_निवेशः << 0 | dft_complex_output << 1);

    विकल्प (fftType)
    {
    पक्ष C2C:
        _dst.create(ssize.height, ssize.विस्तारः, CV_MAKE_TYPE(गभीरता, 2));
        भङ्ग;
    पक्ष R2C: // TODO implement it if possible
    पक्ष C2R: // TODO implement it if possible
    पक्ष R2R: // AMD Fft does not support this type
    स्वभाव:
        प्रत्यागम असत्य;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    यदि (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        प्रत्यागम असत्य;
    तार्किक inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (पूर्णाङ्क)src.चरणः, (पूर्णाङ्क)dst.चरणः,
                          गभीरता == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    size_t बफर_आकार = 0;
    CLAMDDFT_Assert(clfft_Get_Tmp_Buf_आकार(plHandle, &बफर_आकार))
    UMat tmp_बफर(1, (पूर्णाङ्क)बफर_आकार, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfft_पङ्क्तौ_निक्षिप_Transform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &queue, 0, रिक्त, &e,
                                          &srcarg, &dstarg, (cl_mem)tmp_बफर.handle(ACCESS_RW)))

    tmp_बफर.addref();
    cl_समुच्चयः_Event_Callback(e, CL_COMPLETE, oclCleanupCallback, tmp_बफर.u);
    प्रत्यागम सत्य;
}

#अपरिभाषय DFT_ASSERT

}

#यदि_अन्तः // HAVE_CLAMDFFT

नामाकाश cv
{

प्रतिरूपम् <प्रकारनाम T>
स्थिर शून्य complementComplex(T * ptr, size_t चरणः, पूर्णाङ्क n, पूर्णाङ्क len, पूर्णाङ्क dft_dims)
{
    T* p0 = (T*)ptr;
    size_t dstep = चरणः/परिमाण(p0[0]);
    प्रति(पूर्णाङ्क i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        प्रति( पूर्णाङ्क j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

स्थिर शून्य complementComplexOutput(पूर्णाङ्क गभीरता, uchar * ptr, size_t चरणः, पूर्णाङ्क गणनम्_2, पूर्णाङ्क len, पूर्णाङ्क dft_dims)
{
    यदि( गभीरता == CV_32F )
        complementComplex((भिन्न*)ptr, चरणः, गणनम्_2, len, dft_dims);
    अन्यथा
        complementComplex((द्विगुण*)ptr, चरणः, गणनम्_2, len, dft_dims);
}

गणना Dft_बहुलकम् {
    InvalidDft = 0,
    Fwd_Real_गन्तव्यम्_CCS,
    Fwd_Real_गन्तव्यम्_Complex,
    Fwd_Complex_गन्तव्यम्_Complex,
    InvCCSToReal,
    Inv_Complex_गन्तव्यम्_Real,
    Inv_Complex_गन्तव्यम्_Complex,
};

गणना DftDims {
    InvalidDim = 0,
    OneDim,
    OneDimColWise,
    TwoDims
};

अन्तःस्थ ध्रुव वर्ण * बहुलकम्_Name(Dft_बहुलकम् m)
{
    विकल्प (m)
    {
    पक्ष InvalidDft: प्रत्यागम "InvalidDft";
    पक्ष Fwd_Real_गन्तव्यम्_CCS: प्रत्यागम "FwdRealToCCS";
    पक्ष Fwd_Real_गन्तव्यम्_Complex: प्रत्यागम "FwdRealToComplex";
    पक्ष Fwd_Complex_गन्तव्यम्_Complex: प्रत्यागम "FwdComplexToComplex";
    पक्ष InvCCSToReal: प्रत्यागम "InvCCSToReal";
    पक्ष Inv_Complex_गन्तव्यम्_Real: प्रत्यागम "InvComplexToReal";
    पक्ष Inv_Complex_गन्तव्यम्_Complex: प्रत्यागम "InvComplexToComplex";
    }
    प्रत्यागम 0;
}

अन्तःस्थ ध्रुव वर्ण * dimsName(DftDims d)
{
    विकल्प (d)
    {
    पक्ष InvalidDim: प्रत्यागम "InvalidDim";
    पक्ष OneDim: प्रत्यागम "OneDim";
    पक्ष OneDimColWise: प्रत्यागम "OneDimColWise";
    पक्ष TwoDims: प्रत्यागम "TwoDims";
    };
    प्रत्यागम 0;
}

प्रतिरूपम् <प्रकारनाम T>
अन्तःस्थ तार्किक isInv(T बहुलकम्)
{
    विकल्प ((Dft_बहुलकम्)बहुलकम्)
    {
        पक्ष InvCCSToReal:
        पक्ष Inv_Complex_गन्तव्यम्_Real:
        पक्ष Inv_Complex_गन्तव्यम्_Complex: प्रत्यागम सत्य;
        स्वभाव: प्रत्यागम असत्य;
    }
}

अन्तःस्थ Dft_बहुलकम् determine_बहुलकम्(तार्किक inv, पूर्णाङ्क cn1, पूर्णाङ्क cn2)
{
    यदि (!inv)
    {
        यदि (cn1 == 1 && cn2 == 1)
            प्रत्यागम Fwd_Real_गन्तव्यम्_CCS;
        अन्यथा यदि (cn1 == 1 && cn2 == 2)
            प्रत्यागम Fwd_Real_गन्तव्यम्_Complex;
        अन्यथा यदि (cn1 == 2 && cn2 == 2)
            प्रत्यागम Fwd_Complex_गन्तव्यम्_Complex;
    }
    अन्यथा
    {
        यदि (cn1 == 1 && cn2 == 1)
            प्रत्यागम InvCCSToReal;
        अन्यथा यदि (cn1 == 2 && cn2 == 1)
            प्रत्यागम Inv_Complex_गन्तव्यम्_Real;
        अन्यथा यदि (cn1 == 2 && cn2 == 2)
            प्रत्यागम Inv_Complex_गन्तव्यम्_Complex;
    }
    प्रत्यागम InvalidDft;
}


अन्तःस्थ DftDims determineDims(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, तार्किक isRowWise, तार्किक isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    यदि (isRowWise)
        प्रत्यागम OneDim;
    यदि (स्तम्भसङ्ख्या == 1 && पङ्क्तिकाः > 1) // one-column-shaped input
    {
        यदि (isContinuous)
            प्रत्यागम OneDim;
        अन्यथा
            प्रत्यागम OneDimColWise;
    }
    यदि (पङ्क्तिकाः == 1)
        प्रत्यागम OneDim;
    यदि (स्तम्भसङ्ख्या > 1 && पङ्क्तिकाः > 1)
        प्रत्यागम TwoDims;
    प्रत्यागम InvalidDim;
}

वर्ग OcvDftBasicImpl CV_FINAL : सार्वजनिक hal::DFT1D
{
सार्वजनिक:
    OcvDftOptions opt;
    // the plan is built once in init(), the workspace is allocated once here and reused by
    // every apply() (zero allocations per transform)
    DftPlan plan;
    Auto_बफर<uchar> ws;
#यदि_परिभाषितम् USE_IPP_DFT
    Auto_बफर<uchar> ippbuf;
    Auto_बफर<uchar> ippworkbuf;
    size_t ipp_worksize;
#यदि_अन्तः

सार्वजनिक:
    OcvDftBasicImpl()
    {
#यदि_परिभाषितम् USE_IPP_DFT
        ipp_worksize = 0;
#यदि_अन्तः
    }
    शून्य init(पूर्णाङ्क len, पूर्णाङ्क गणनम्_2, पूर्णाङ्क गभीरता, पूर्णाङ्क flags, तार्किक *need_बफर)
    {
        पूर्णाङ्क stage = (flags & CV_HAL_DFT_STAGE_स्तम्भसङ्ख्या) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        तार्किक real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        तार्किक needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = असत्य;
    #यदि_परिभाषितम् USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        यदि( CV_IPP_CHECK_COND && (opt.n*गणनम्_2 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            पूर्णाङ्क ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            पूर्णाङ्क specsize=0, initsize=0, worksize=0;
            Ipp_DFTGet_आकार_Func get_आकार_Func = 0;
            IppDFTInitFunc initFunc = 0;

            यदि( real_transform && stage == 0 )
            {
                यदि( गभीरता == CV_32F )
                {
                    get_आकार_Func = ipps_DFTGet_आकार_R_32f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_32f, ippsDFTInit_R_32f>;
                }
                अन्यथा
                {
                    get_आकार_Func = ipps_DFTGet_आकार_R_64f;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_R_64f, ippsDFTInit_R_64f>;
                }
            }
            अन्यथा
            {
                यदि( गभीरता == CV_32F )
                {
                    get_आकार_Func = ipps_DFTGet_आकार_C_32fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_32fc, ippsDFTInit_C_32fc>;
                }
                अन्यथा
                {
                    get_आकार_Func = ipps_DFTGet_आकार_C_64fc;
                    initFunc = ippDFTInitWrap<IppsDFTSpec_C_64fc, ippsDFTInit_C_64fc>;
                }
            }
            यदि( get_आकार_Func(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (size_t)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                यदि( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = सत्य;
            }
            अन्यथा
                समुच्चयः_Ipp_Error_स्थितिः();
        }
    #यदि_अन्तः

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            ध्रुव DFTKernels& kernels = getDFTKernels(गभीरता);
            पूर्णाङ्क विधा = stage == 0 && real_transform ? (opt.isInverse ? DFT_विधा_C2_R : DFT_विधा_R2_C)
                                                    : DFT_विधा_C2_C;
            plan.build(विधा, len, गभीरता, kernels.vlanes);
            ws.allocate(plan.ws_अष्टकानि);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = ws.दत्त();
            यदि (need_बफर)
                *need_बफर = असत्य;   // the engine supports in-place for every kind
        }

        {
            स्थिर DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<भिन्न>>,
                dftWrap<भिन्न, RealDFT<भिन्न>>,
                dftWrap<भिन्न, CCSIDFT<भिन्न>>,
                dftWrap<Complexd, DFT<द्विगुण>>,
                dftWrap<द्विगुण, RealDFT<द्विगुण>>,
                dftWrap<द्विगुण, CCSIDFT<द्विगुण>>
            };
            पूर्णाङ्क idx = 0;
            यदि (stage == 0)
            {
                यदि (real_transform)
                {
                    यदि (!opt.isInverse)
                        idx = 1;
                    अन्यथा
                        idx = 2;
                }
            }
            यदि (गभीरता == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        यदि(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            पूर्णाङ्क row_गणनम् = गणनम्_2;
            यदि (stage == 0 && (flags & CV_HAL_DFT_पङ्क्तिकाः) != 0)
                row_गणनम् = 1;
            opt.scale = 1./(len * row_गणनम्);
        }
    }

    शून्य apply(ध्रुव uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    size_t workspace_आकार() ध्रुव
    {
        size_t sz = opt.plan ? plan.ws_अष्टकानि : 0;
#यदि_परिभाषितम् USE_IPP_DFT
        यदि (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#यदि_अन्तः
        प्रत्यागम sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    शून्य applyWithWorkspace(ध्रुव uchar *src, uchar *dst, uchar* workspace) ध्रुव
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#यदि_परिभाषितम् USE_IPP_DFT
        यदि (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#यदि_अन्तः
        opt_.dft_func(opt_, src, dst);
    }

    शून्य मुक्त() {}
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
स्थिर ध्रुव पूर्णाङ्क DFT_स्तम्भसङ्ख्या_BATCH_MIN = 4;
स्थिर ध्रुव पूर्णाङ्क DFT_स्तम्भसङ्ख्या_BATCH_MAX = 16;
स्थिर ध्रुव size_t DFT_स्तम्भसङ्ख्या_BATCH_अष्टकानि = 1 << 17;

स्थिर पूर्णाङ्क dft_स्तम्भसङ्ख्या_Batch(पूर्णाङ्क len, size_t complex_elem_आकार)
{
    पूर्णाङ्क batch = (पूर्णाङ्क)(DFT_स्तम्भसङ्ख्या_BATCH_अष्टकानि/((size_t)len*complex_elem_आकार));
    प्रत्यागम std::max(DFT_स्तम्भसङ्ख्या_BATCH_MIN, std::min(DFT_स्तम्भसङ्ख्या_BATCH_MAX, batch));
}

स्थिर द्विगुण dftParallelStripes(पूर्णाङ्क गणनम्_2, पूर्णाङ्क len)
{
    ध्रुव द्विगुण कालांशः = 0.5e6;
    प्रत्यागम (द्विगुण)गणनम्_2*len*std::max(std::लघुगणकः((द्विगुण)len)/std::लघुगणकः(2.), 1.)/कालांशः;
}

वर्ग OcvDftImpl CV_FINAL : सार्वजनिक hal::DFT2D
{
रक्षित:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    ध्रुव OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    ध्रुव OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    तार्किक need_बफर_A;
    तार्किक need_बफर_B;
    तार्किक inv;
    पूर्णाङ्क विस्तारः;
    पूर्णाङ्क height;
    Dft_बहुलकम् बहुलकम्;
    पूर्णाङ्क elem_आकार_2;
    पूर्णाङ्क complex_elem_आकार;
    पूर्णाङ्क गभीरता;
    तार्किक real_transform;
    पूर्णाङ्क nonzero_पङ्क्तिकाः;
    तार्किक isRowTransform;
    तार्किक isScaled;
    std::vector<पूर्णाङ्क> stages;
    तार्किक useIpp;
    पूर्णाङ्क src_channels;
    पूर्णाङ्क dst_channels;

    Auto_बफर<uchar> tmp_bufA;
    Auto_बफर<uchar> tmp_bufB;
    Auto_बफर<uchar> buf0;
    Auto_बफर<uchar> buf1;

सार्वजनिक:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        need_बफर_A = असत्य;
        need_बफर_B = असत्य;
        inv = असत्य;
        विस्तारः = 0;
        height = 0;
        बहुलकम् = InvalidDft;
        elem_आकार_2 = 0;
        complex_elem_आकार = 0;
        गभीरता = 0;
        real_transform = असत्य;
        nonzero_पङ्क्तिकाः = 0;
        isRowTransform = असत्य;
        isScaled = असत्य;
        useIpp = असत्य;
        src_channels = 0;
        dst_channels = 0;
    }

    शून्य init(पूर्णाङ्क विस्तारः_3, पूर्णाङ्क _height, पूर्णाङ्क गभीरता_2, पूर्णाङ्क _src_channels, पूर्णाङ्क _dst_channels, पूर्णाङ्क flags, पूर्णाङ्क nonzero_पङ्क्तिकाः_2)
    {
        तार्किक isComplex = _src_channels != _dst_channels;
        nonzero_पङ्क्तिकाः = nonzero_पङ्क्तिकाः_2;
        विस्तारः = विस्तारः_3;
        height = _height;
        गभीरता = गभीरता_2;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        तार्किक isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        तार्किक isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        तार्किक isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        बहुलकम् = determine_बहुलकम्(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        isRowTransform = (flags & CV_HAL_DFT_पङ्क्तिकाः) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        need_बफर_A = असत्य;
        need_बफर_B = असत्य;
        real_transform = (बहुलकम् != Fwd_Complex_गन्तव्यम्_Complex && बहुलकम् != Inv_Complex_गन्तव्यम्_Complex);

        elem_आकार_2 = (गभीरता == CV_32F) ? परिमाण(भिन्न) : परिमाण(द्विगुण);
        complex_elem_आकार = elem_आकार_2 * 2;
        यदि( !real_transform )
            elem_आकार_2 = complex_elem_आकार;

#यदि defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            यदि (nonzero_पङ्क्तिकाः == 0 && गभीरता == CV_32F && ((विस्तारः * height)>(पूर्णाङ्क)(1<<6)))
            {
                यदि (बहुलकम् == Fwd_Complex_गन्तव्यम्_Complex || बहुलकम् == Inv_Complex_गन्तव्यम्_Complex || बहुलकम् == Fwd_Real_गन्तव्यम्_CCS || बहुलकम् == InvCCSToReal)
                {
                    useIpp = सत्य;
                    प्रत्यागम;
                }
            }
        }
#यदि_अन्तः

        DftDims dims = determineDims(height, विस्तारः, isRowTransform, isContinuous);
        यदि (dims == TwoDims)
        {
            stages.resize(2);
            यदि (बहुलकम् == InvCCSToReal || बहुलकम् == Inv_Complex_गन्तव्यम्_Real)
            {
                stages[0] = 1;
                stages[1] = 0;
            }
            अन्यथा
            {
                stages[0] = 0;
                stages[1] = 1;
            }
        }
        अन्यथा
        {
            stages.resize(1);
            यदि (dims == OneDimColWise)
                stages[0] = 1;
            अन्यथा
                stages[0] = 0;
        }

        प्रति(uint stage_सूचकांक = 0; stage_सूचकांक < stages.size(); ++stage_सूचकांक)
        {
            यदि (stage_सूचकांक == 1)
            {
                isInplace = सत्य;
                isComplex = असत्य;
            }

            पूर्णाङ्क stage = stages[stage_सूचकांक];
            तार्किक is_चरमः_Stage = (stage_सूचकांक + 1 == stages.size());

            पूर्णाङ्क len, गणनम्_2;

            पूर्णाङ्क f = 0;
            यदि (inv)
                f |= CV_HAL_DFT_INVERSE;
            यदि (isScaled)
                f |= CV_HAL_DFT_SCALE;
            यदि (isRowTransform)
                f |= CV_HAL_DFT_पङ्क्तिकाः;
            यदि (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            यदि (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            यदि (!is_चरमः_Stage)
                f |= CV_HAL_DFT_TWO_STAGE;

            यदि( stage == 0 ) // row-wise transform
            {
                यदि (विस्तारः == 1 && !isRowTransform )
                {
                    len = height;
                    गणनम्_2 = विस्तारः;
                }
                अन्यथा
                {
                    len = विस्तारः;
                    गणनम्_2 = height;
                }
                need_बफर_A = isInplace;
                contextA = hal::DFT1D::create(len, गणनम्_2, गभीरता, f, &need_बफर_A);
                basicA = dynamic_cast<ध्रुव OcvDftBasicImpl*>(contextA.get());
                यदि (need_बफर_A)
                    tmp_bufA.allocate(len * complex_elem_आकार);
            }
            अन्यथा
            {
                len = height;
                गणनम्_2 = विस्तारः;
                f |= CV_HAL_DFT_STAGE_स्तम्भसङ्ख्या;
                need_बफर_B = isInplace;
                contextB = hal::DFT1D::create(len, गणनम्_2, गभीरता, f, &need_बफर_B);
                basicB = dynamic_cast<ध्रुव OcvDftBasicImpl*>(contextB.get());
                यदि (need_बफर_B)
                    tmp_bufB.allocate(len * complex_elem_आकार);

                buf0.allocate(len * complex_elem_आकार);
                buf1.allocate(len * complex_elem_आकार);
            }
        }
    }

    शून्य apply(ध्रुव uchar * src, size_t src_चरणः, uchar * dst, size_t dst_चरणः) CV_OVERRIDE
    {
#यदि defined USE_IPP_DFT
        यदि (useIpp)
        {
            पूर्णाङ्क ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            यदि (!isRowTransform)
            {
                यदि (बहुलकम् == Fwd_Complex_गन्तव्यम्_Complex || बहुलकम् == Inv_Complex_गन्तव्यम्_Complex)
                {
                    यदि (ippi_DFT_C_32F(src, src_चरणः, dst, dst_चरणः, विस्तारः, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_सङ्कलय(CV_IMPL_IPP);
                        प्रत्यागम;
                    }
                    समुच्चयः_Ipp_Error_स्थितिः();
                }
                अन्यथा यदि (बहुलकम् == Fwd_Real_गन्तव्यम्_CCS || बहुलकम् == InvCCSToReal)
                {
                    यदि (ippi_DFT_R_32F(src, src_चरणः, dst, dst_चरणः, विस्तारः, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_सङ्कलय(CV_IMPL_IPP);
                        प्रत्यागम;
                    }
                    समुच्चयः_Ipp_Error_स्थितिः();
                }
            }
            अन्यथा
            {
                यदि (बहुलकम् == Fwd_Complex_गन्तव्यम्_Complex || बहुलकम् == Inv_Complex_गन्तव्यम्_Complex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    यदि (Dft_C_IPPLoop(src, src_चरणः, dst, dst_चरणः, विस्तारः, height, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_सङ्कलय(CV_IMPL_IPP|CV_IMPL_MT);
                        प्रत्यागम;
                    }
                    समुच्चयः_Ipp_Error_स्थितिः();
                }
                अन्यथा यदि (बहुलकम् == Fwd_Real_गन्तव्यम्_CCS || बहुलकम् == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippi_DFTInv_Pack_गन्तव्यम्_R_32f_C1_R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    यदि (Dft_R_IPPLoop(src, src_चरणः, dst, dst_चरणः, विस्तारः, height, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_सङ्कलय(CV_IMPL_IPP|CV_IMPL_MT);
                        प्रत्यागम;
                    }
                    समुच्चयः_Ipp_Error_स्थितिः();
                }
            }
            प्रत्यागम;
        }
#यदि_अन्तः

        प्रति(uint stage_सूचकांक = 0; stage_सूचकांक < stages.size(); ++stage_सूचकांक)
        {
            पूर्णाङ्क stage_src_channels = src_channels;
            पूर्णाङ्क stage_dst_channels = dst_channels;

            यदि (stage_सूचकांक == 1)
            {
                src = dst;
                src_चरणः = dst_चरणः;
                stage_src_channels = stage_dst_channels;
            }

            पूर्णाङ्क stage = stages[stage_सूचकांक];
            तार्किक is_चरमः_Stage = (stage_सूचकांक + 1 == stages.size());
            तार्किक isComplex = stage_src_channels != stage_dst_channels;

            यदि( stage == 0 )
                rowDft(src, src_चरणः, dst, dst_चरणः, isComplex, is_चरमः_Stage);
            अन्यथा
                colDft(src, src_चरणः, dst, dst_चरणः, stage_src_channels, stage_dst_channels, is_चरमः_Stage);
        }
    }

रक्षित:

    शून्य rowDft(ध्रुव uchar* src_दत्त, size_t src_चरणः, uchar* dst_दत्त, size_t dst_चरणः, तार्किक isComplex, तार्किक is_चरमः_Stage)
    {
        पूर्णाङ्क len, गणनम्_2;
        यदि (विस्तारः == 1 && !isRowTransform )
        {
            len = height;
            गणनम्_2 = विस्तारः;
        }
        अन्यथा
        {
            len = विस्तारः;
            गणनम्_2 = height;
        }
        पूर्णाङ्क dptr_विस्थापनम् = 0;
        पूर्णाङ्क dst_पूर्णम्_len = len*elem_आकार_2;

        यदि( need_बफर_A )
        {
            यदि (बहुलकम् == Fwd_Real_गन्तव्यम्_CCS && (len & 1) && len > 1)
                dptr_विस्थापनम् = elem_आकार_2;
        }

        यदि( !inv && isComplex )
            dst_पूर्णम्_len += (len & 1) ? elem_आकार_2 : complex_elem_आकार;

        पूर्णाङ्क nz = nonzero_पङ्क्तिकाः;
        यदि( nz <= 0 || nz > गणनम्_2 )
            nz = गणनम्_2;

        size_t ws_आकार = basicA ? basicA->workspace_आकार() : 0;
        द्विगुण nstripes = dftParallelStripes(nz, len);
        यदि( ws_आकार > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            ध्रुव OcvDftBasicImpl* ctx = basicA;
            स्वचालित process_पङ्क्तिकाः = [&, ctx](ध्रुव Range& range)
            {
                Auto_बफर<uchar> ws(ws_आकार);
                Auto_बफर<uchar> rowbuf(need_बफर_A ? len * complex_elem_आकार : 0);
                प्रति( पूर्णाङ्क i = range.start; i < range.end; i++ )
                {
                    ध्रुव uchar* sptr = src_दत्त + src_चरणः * i;
                    uchar* dptr0 = dst_दत्त + dst_चरणः * i;
                    uchar* dptr = need_बफर_A ? rowbuf.दत्त() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, ws.दत्त());
                    यदि( need_बफर_A )
                        स्मृतिं_प्रतिलिख( dptr0, dptr + dptr_विस्थापनम्, dst_पूर्णम्_len );
                }
            };
            parallel_for_(Range(0, nz), process_पङ्क्तिकाः, nstripes);
        }
        अन्यथा
        {
            प्रति( पूर्णाङ्क i = 0; i < nz; i++ )
            {
                ध्रुव uchar* sptr = src_दत्त + src_चरणः * i;
                uchar* dptr0 = dst_दत्त + dst_चरणः * i;
                uchar* dptr = dptr0;

                यदि( need_बफर_A )
                    dptr = tmp_bufA.दत्त();

                contextA->apply(sptr, dptr);

                यदि( need_बफर_A )
                    स्मृतिं_प्रतिलिख( dptr0, dptr + dptr_विस्थापनम्, dst_पूर्णम्_len );
            }
        }

        प्रति( पूर्णाङ्क i = nz; i < गणनम्_2; i++ )
        {
            uchar* dptr0 = dst_दत्त + dst_चरणः * i;
            स्मृतिं_पूरय( dptr0, 0, dst_पूर्णम्_len );
        }
        यदि(is_चरमः_Stage &&  बहुलकम् == Fwd_Real_गन्तव्यम्_Complex)
            complementComplexOutput(गभीरता, dst_दत्त, dst_चरणः, len, nz, 1);
    }

    शून्य colDft(ध्रुव uchar* src_दत्त, size_t src_चरणः, uchar* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क stage_src_channels, पूर्णाङ्क stage_dst_channels, तार्किक is_चरमः_Stage)
    {
        पूर्णाङ्क len = height;
        पूर्णाङ्क गणनम्_2 = विस्तारः;
        पूर्णाङ्क a = 0, b = गणनम्_2;
        uchar *dbuf0, *dbuf1;
        ध्रुव uchar* sptr0 = src_दत्त;
        uchar* dptr0 = dst_दत्त;

        dbuf0 = buf0.दत्त(), dbuf1 = buf1.दत्त();

        यदि( need_बफर_B )
        {
            dbuf1 = tmp_bufB.दत्त();
            dbuf0 = buf1.दत्त();
        }

        यदि( real_transform )
        {
            पूर्णाङ्क even;
            a = 1;
            even = (गणनम्_2 & 1) == 0;
            b = (गणनम्_2+1)/2;
            यदि( !inv )
            {
                स्मृतिं_पूरय( buf0.दत्त(), 0, len*complex_elem_आकार );
                CopyColumn( sptr0, src_चरणः, buf0.दत्त(), complex_elem_आकार, len, elem_आकार_2 );
                sptr0 += stage_dst_channels*elem_आकार_2;
                यदि( even )
                {
                    स्मृतिं_पूरय( buf1.दत्त(), 0, len*complex_elem_आकार );
                    CopyColumn( sptr0 + (गणनम्_2-2)*elem_आकार_2, src_चरणः,
                                buf1.दत्त(), complex_elem_आकार, len, elem_आकार_2 );
                }
            }
            अन्यथा यदि( stage_src_channels == 1 )
            {
                CopyColumn( sptr0, src_चरणः, buf0.दत्त(), elem_आकार_2, len, elem_आकार_2 );
                ExpandCCS( buf0.दत्त(), len, elem_आकार_2 );
                यदि( even )
                {
                    CopyColumn( sptr0 + (गणनम्_2-1)*elem_आकार_2, src_चरणः,
                                buf1.दत्त(), elem_आकार_2, len, elem_आकार_2 );
                    ExpandCCS( buf1.दत्त(), len, elem_आकार_2 );
                }
                sptr0 += elem_आकार_2;
            }
            अन्यथा
            {
                CopyColumn( sptr0, src_चरणः, buf0.दत्त(), complex_elem_आकार, len, complex_elem_आकार );
                यदि( even )
                {
                    CopyColumn( sptr0 + b*complex_elem_आकार, src_चरणः,
                                   buf1.दत्त(), complex_elem_आकार, len, complex_elem_आकार );
                }
                sptr0 += complex_elem_आकार;
            }

            यदि( even )
                contextB->apply(buf1.दत्त(), dbuf1);
            contextB->apply(buf0.दत्त(), dbuf0);

            यदि( stage_dst_channels == 1 )
            {
                यदि( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    स्मृतिं_प्रतिलिख( dbuf0 + elem_आकार_2, dbuf0, elem_आकार_2 );
                    CopyColumn( dbuf0 + elem_आकार_2, elem_आकार_2, dptr0,
                                   dst_चरणः, len, elem_आकार_2 );
                    यदि( even )
                    {
                        स्मृतिं_प्रतिलिख( dbuf1 + elem_आकार_2, dbuf1, elem_आकार_2 );
                        CopyColumn( dbuf1 + elem_आकार_2, elem_आकार_2,
                                       dptr0 + (गणनम्_2-1)*elem_आकार_2,
                                       dst_चरणः, len, elem_आकार_2 );
                    }
                    dptr0 += elem_आकार_2;
                }
                अन्यथा
                {
                    // copy the real part of the complex vector to the first/last column
                    CopyColumn( dbuf0, complex_elem_आकार, dptr0, dst_चरणः, len, elem_आकार_2 );
                    यदि( even )
                        CopyColumn( dbuf1, complex_elem_आकार, dptr0 + (गणनम्_2-1)*elem_आकार_2,
                                       dst_चरणः, len, elem_आकार_2 );
                    dptr0 += elem_आकार_2;
                }
            }
            अन्यथा
            {
                CV_Assert( !inv );
                CopyColumn( dbuf0, complex_elem_आकार, dptr0,
                               dst_चरणः, len, complex_elem_आकार );
                यदि( even )
                    CopyColumn( dbuf1, complex_elem_आकार,
                                   dptr0 + b*complex_elem_आकार,
                                   dst_चरणः, len, complex_elem_आकार );
                dptr0 += complex_elem_आकार;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        पूर्णाङ्क ncols = b - a;
        ध्रुव पूर्णाङ्क स्तम्भसङ्ख्या_batch = dft_स्तम्भसङ्ख्या_Batch(len, complex_elem_आकार);
        पूर्णाङ्क nbatches = (ncols + स्तम्भसङ्ख्या_batch - 1)/स्तम्भसङ्ख्या_batch;
        size_t ws_आकार = basicB ? basicB->workspace_आकार() : 0;
        size_t colbuf_आकार = (size_t)len*complex_elem_आकार;
        ध्रुव OcvDftBasicImpl* ctx = basicB;
        स्वचालित processBatches = [&, ctx](ध्रुव Range& range)
        {
            Auto_बफर<uchar> ws(ws_आकार);
            Auto_बफर<uchar> cbuf(colbuf_आकार*स्तम्भसङ्ख्या_batch*(need_बफर_B ? 2 : 1));
            uchar* cbuf0 = cbuf.दत्त();
            uchar* cdbuf0 = need_बफर_B ? cbuf0 + colbuf_आकार*स्तम्भसङ्ख्या_batch : cbuf0;
            प्रति( पूर्णाङ्क bi = range.start; bi < range.end; bi++ )
            {
                पूर्णाङ्क c0 = bi*स्तम्भसङ्ख्या_batch;
                पूर्णाङ्क nc = std::min(स्तम्भसङ्ख्या_batch, ncols - c0);
                ध्रुव uchar* sptr = sptr0 + (size_t)c0*complex_elem_आकार;
                uchar* dptr = dptr0 + (size_t)c0*complex_elem_आकार;
                CopyFromColumns( sptr, src_चरणः, cbuf0, nc, len, complex_elem_आकार );
                प्रति( पूर्णाङ्क k = 0; k < nc; k++ )
                {
                    यदि( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_आकार, cdbuf0 + k*colbuf_आकार, ws.दत्त());
                    अन्यथा
                        contextB->apply(cbuf0 + k*colbuf_आकार, cdbuf0 + k*colbuf_आकार);
                }
                Copy_गन्तव्यम्_Columns( cdbuf0, dptr, dst_चरणः, nc, len, complex_elem_आकार );
            }
        };
        द्विगुण nstripes = dftParallelStripes(ncols, len);
        यदि( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        अन्यथा
            processBatches(Range(0, nbatches));
        यदि(is_चरमः_Stage && बहुलकम् == Fwd_Real_गन्तव्यम्_Complex)
            complementComplexOutput(गभीरता, dst_दत्त, dst_चरणः, गणनम्_2, len, 2);
    }
};

संरचना ReplacementDFT1D : सार्वजनिक hal::DFT1D
{
    cvhalDFT *context;
    तार्किक isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(असत्य) {}
    तार्किक init(पूर्णाङ्क len, पूर्णाङ्क गणनम्_2, पूर्णाङ्क गभीरता, पूर्णाङ्क flags, तार्किक *need_बफर)
    {
        पूर्णाङ्क res = cv_hal_dftInit1D(&context, len, गणनम्_2, गभीरता, flags, need_बफर);
        isInitialized = (res == CV_HAL_ERROR_OK);
        प्रत्यागम isInitialized;
    }
    शून्य apply(ध्रुव uchar *src, uchar *dst) CV_OVERRIDE
    {
        यदि (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        यदि (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

संरचना ReplacementDFT2D : सार्वजनिक hal::DFT2D
{
    cvhalDFT *context;
    तार्किक isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(असत्य) {}
    तार्किक init(पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क गभीरता,
              पूर्णाङ्क src_channels, पूर्णाङ्क dst_channels,
              पूर्णाङ्क flags, पूर्णाङ्क nonzero_पङ्क्तिकाः)
    {
        पूर्णाङ्क res = cv_hal_dftInit2D(&context, विस्तारः, height, गभीरता, src_channels, dst_channels, flags, nonzero_पङ्क्तिकाः);
        isInitialized = (res == CV_HAL_ERROR_OK);
        प्रत्यागम isInitialized;
    }
    शून्य apply(ध्रुव uchar *src, size_t src_चरणः, uchar *dst, size_t dst_चरणः) CV_OVERRIDE
    {
        यदि (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_चरणः, dst, dst_चरणः);
        }
    }
    ~ReplacementDFT2D()
    {
        यदि (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

नामाकाश hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(पूर्णाङ्क len, पूर्णाङ्क गणनम्_2, पूर्णाङ्क गभीरता, पूर्णाङ्क flags, तार्किक *need_बफर)
{
    {
        ReplacementDFT1D *impl = नूतन ReplacementDFT1D();
        यदि (impl->init(len, गणनम्_2, गभीरता, flags, need_बफर))
        {
            प्रत्यागम Ptr<DFT1D>(impl);
        }
        विलोपय impl;
    }
    {
        OcvDftBasicImpl *impl = नूतन OcvDftBasicImpl();
        impl->init(len, गणनम्_2, गभीरता, flags, need_बफर);
        प्रत्यागम Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क गभीरता,
                         पूर्णाङ्क src_channels, पूर्णाङ्क dst_channels,
                         पूर्णाङ्क flags, पूर्णाङ्क nonzero_पङ्क्तिकाः)
{
    {
        ReplacementDFT2D *impl = नूतन ReplacementDFT2D();
        यदि (impl->init(विस्तारः, height, गभीरता, src_channels, dst_channels, flags, nonzero_पङ्क्तिकाः))
        {
            प्रत्यागम Ptr<DFT2D>(impl);
        }
        विलोपय impl;
    }
    {
        यदि(विस्तारः == 1 && nonzero_पङ्क्तिकाः > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = नूतन OcvDftImpl();
        impl->init(विस्तारः, height, गभीरता, src_channels, dst_channels, flags, nonzero_पङ्क्तिकाः);
        प्रत्यागम Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


शून्य cv::dft( निवेशः_Array _src0, OutputArray _dst, पूर्णाङ्क flags, पूर्णाङ्क nonzero_पङ्क्तिकाः )
{
    CV_INSTRUMENT_REGION();

#यदि_परिभाषितम् HAVE_CLAMDFFT
    CV_OCL_सततम्(ocl::haveAmdFft() && ocl::Device::getDefault().type() != ocl::Device::TYPE_CPU &&
            _dst.isUMat() && _src0.dims() <= 2 && nonzero_पङ्क्तिकाः == 0,
               ocl_dft_amdfft(_src0, _dst, flags))
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_OPENCL
    CV_OCL_सततम्(_dst.isUMat() && _src0.dims() <= 2,
               ocl_dft(_src0, _dst, flags, nonzero_पङ्क्तिकाः))
#यदि_अन्तः

    Mat src0 = _src0.getMat(), src = src0;
    यदि (src.empty())
    {
        _dst.release();
        प्रत्यागम;
    }
    तार्किक inv = (flags & DFT_INVERSE) != 0;
    पूर्णाङ्क type = src.type();
    पूर्णाङ्क गभीरता = src.गभीरता();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_निवेशः) && src.channels() != 2) );

    यदि( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.create_Same_आकार( src, CV_MAKETYPE(गभीरता, 2) );
    अन्यथा यदि( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.create_Same_आकार( src, गभीरता );
    अन्यथा
        _dst.create_Same_आकार( src, type );

    Mat dst = _dst.getMat();

    पूर्णाङ्क f = 0;
    यदि (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    यदि (inv)
        f |= CV_HAL_DFT_INVERSE;
    यदि (flags & DFT_पङ्क्तिकाः)
        f |= CV_HAL_DFT_पङ्क्तिकाः;
    यदि (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    यदि (src.दत्त == dst.दत्त)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.स्तम्भसङ्ख्या, src.पङ्क्तिकाः, गभीरता, src.channels(), dst.channels(), f, nonzero_पङ्क्तिकाः);
    c->apply(src.दत्त, src.चरणः, dst.दत्त, dst.चरणः);
}


शून्य cv::idft( निवेशः_Array src, OutputArray dst, पूर्णाङ्क flags, पूर्णाङ्क nonzero_पङ्क्तिकाः )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_पङ्क्तिकाः );
}

#यदि_परिभाषितम् HAVE_OPENCL

नामाकाश cv {

स्थिर तार्किक ocl_mulSpectrums( निवेशः_Array _srcA, निवेशः_Array _srcB,
                              OutputArray _dst, पूर्णाङ्क flags, तार्किक conjB )
{
    पूर्णाङ्क atype = _srcA.type(), btype = _srcB.type(),
            पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    आकार asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    यदि ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        प्रत्यागम असत्य;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.create_Same_आकार(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    यदि (k.empty())
        प्रत्यागम असत्य;

    k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(A), ocl::KernelArg::Read_Only_No_आकार(B),
           ocl::KernelArg::WriteOnly(dst), पङ्क्तिकाः_प्रत्येकम्_WI);

    size_t globalsize[2] = { (size_t)asize.विस्तारः, ((size_t)asize.height + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

}

#यदि_अन्तः

नामाकाश {

#परिभाषय VAL(buf, elem) (((T*)((वर्ण*)दत्त ## buf + (चरणः ## buf * (elem))))[0])
#परिभाषय MUL_SPECTRUMS_COL(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    प्रति (size_t j = 1; j <= पङ्क्तिकाः - 2; j += 2) \
    { \
        द्विगुण a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        द्विगुण b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        यदि (conjB) b_im = -b_im; \
        द्विगुण c_re = a_re * b_re - a_im * b_im; \
        द्विगुण c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    यदि ((पङ्क्तिकाः & 1) == 0) \
        VAL(C, पङ्क्तिकाः-1) = VAL(A, पङ्क्तिकाः-1) * VAL(B, पङ्क्तिकाः-1)

प्रतिरूपम् <प्रकारनाम T, तार्किक conjB> स्थिर अन्तःस्थ
शून्य mulSpectrums_processCol_noinplace(ध्रुव T* दत्त_A, ध्रुव T* दत्त_B, T* दत्त_C, size_t चरणः_A, size_t चरणः_B, size_t चरणः_C, size_t पङ्क्तिकाः)
{
    MUL_SPECTRUMS_COL(A, B, C);
}

प्रतिरूपम् <प्रकारनाम T, तार्किक conjB> स्थिर अन्तःस्थ
शून्य mulSpectrums_processCol_inplaceA(ध्रुव T* दत्त_B, T* दत्त_AC, size_t चरणः_B, size_t चरणः_AC, size_t पङ्क्तिकाः)
{
    MUL_SPECTRUMS_COL(AC, B, AC);
}
प्रतिरूपम् <प्रकारनाम T, तार्किक conjB, तार्किक inplaceA> स्थिर अन्तःस्थ
शून्य mulSpectrums_processCol(ध्रुव T* दत्त_A, ध्रुव T* दत्त_B, T* दत्त_C, size_t चरणः_A, size_t चरणः_B, size_t चरणः_C, size_t पङ्क्तिकाः)
{
    यदि (inplaceA)
        mulSpectrums_processCol_inplaceA<T, conjB>(दत्त_B, दत्त_C, चरणः_B, चरणः_C, पङ्क्तिकाः);
    अन्यथा
        mulSpectrums_processCol_noinplace<T, conjB>(दत्त_A, दत्त_B, दत्त_C, चरणः_A, चरणः_B, चरणः_C, पङ्क्तिकाः);
}
#अपरिभाषय MUL_SPECTRUMS_COL
#अपरिभाषय VAL

प्रतिरूपम् <प्रकारनाम T, तार्किक conjB, तार्किक inplaceA> स्थिर अन्तःस्थ
शून्य mul_Spectrums_process_स्तम्भसङ्ख्या(ध्रुव T* दत्त_A, ध्रुव T* दत्त_B, T* दत्त_C, size_t चरणः_A, size_t चरणः_B, size_t चरणः_C, size_t पङ्क्तिकाः, size_t स्तम्भसङ्ख्या)
{
    mulSpectrums_processCol<T, conjB, inplaceA>(दत्त_A, दत्त_B, दत्त_C, चरणः_A, चरणः_B, चरणः_C, पङ्क्तिकाः);
    यदि ((स्तम्भसङ्ख्या & 1) == 0)
    {
        mulSpectrums_processCol<T, conjB, inplaceA>(दत्त_A + स्तम्भसङ्ख्या - 1, दत्त_B + स्तम्भसङ्ख्या - 1, दत्त_C + स्तम्भसङ्ख्या - 1, चरणः_A, चरणः_B, चरणः_C, पङ्क्तिकाः);
    }
}

#परिभाषय VAL(buf, elem) (दत्त ## buf[(elem)])
#परिभाषय MUL_SPECTRUMS_ROW(A, B, C) \
    प्रति (size_t j = j0; j < j1; j += 2) \
    { \
        द्विगुण a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        द्विगुण b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        यदि (conjB) b_im = -b_im; \
        द्विगुण c_re = a_re * b_re - a_im * b_im; \
        द्विगुण c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
प्रतिरूपम् <प्रकारनाम T, तार्किक conjB> स्थिर अन्तःस्थ
शून्य mulSpectrums_processRow_noinplace(ध्रुव T* दत्त_A, ध्रुव T* दत्त_B, T* दत्त_C, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(A, B, C);
}
प्रतिरूपम् <प्रकारनाम T, तार्किक conjB> स्थिर अन्तःस्थ
शून्य mulSpectrums_processRow_inplaceA(ध्रुव T* दत्त_B, T* दत्त_AC, size_t j0, size_t j1)
{
    MUL_SPECTRUMS_ROW(AC, B, AC);
}
प्रतिरूपम् <प्रकारनाम T, तार्किक conjB, तार्किक inplaceA> स्थिर अन्तःस्थ
शून्य mulSpectrums_processRow(ध्रुव T* दत्त_A, ध्रुव T* दत्त_B, T* दत्त_C, size_t j0, size_t j1)
{
    यदि (inplaceA)
        mulSpectrums_processRow_inplaceA<T, conjB>(दत्त_B, दत्त_C, j0, j1);
    अन्यथा
        mulSpectrums_processRow_noinplace<T, conjB>(दत्त_A, दत्त_B, दत्त_C, j0, j1);
}
#अपरिभाषय MUL_SPECTRUMS_ROW
#अपरिभाषय VAL

प्रतिरूपम् <प्रकारनाम T, तार्किक conjB, तार्किक inplaceA> स्थिर अन्तःस्थ
शून्य mul_Spectrums_process_पङ्क्तिकाः(ध्रुव T* दत्त_A, ध्रुव T* दत्त_B, T* दत्त_C, size_t चरणः_A, size_t चरणः_B, size_t चरणः_C, size_t पङ्क्तिकाः, size_t स्तम्भसङ्ख्या, size_t j0, size_t j1, तार्किक is_1d_CN1)
{
    यावत् (पङ्क्तिकाः-- > 0)
    {
        यदि (is_1d_CN1)
            दत्त_C[0] = दत्त_A[0]*दत्त_B[0];
        mulSpectrums_processRow<T, conjB, inplaceA>(दत्त_A, दत्त_B, दत्त_C, j0, j1);
        यदि (is_1d_CN1 && (स्तम्भसङ्ख्या & 1) == 0)
            दत्त_C[j1] = दत्त_A[j1]*दत्त_B[j1];

        दत्त_A = (ध्रुव T*)(((वर्ण*)दत्त_A) + चरणः_A);
        दत्त_B = (ध्रुव T*)(((वर्ण*)दत्त_B) + चरणः_B);
        दत्त_C =       (T*)(((वर्ण*)दत्त_C) + चरणः_C);
    }
}


प्रतिरूपम् <प्रकारनाम T, तार्किक conjB, तार्किक inplaceA> स्थिर अन्तःस्थ
शून्य mulSpectrums_Impl_(ध्रुव T* दत्त_A, ध्रुव T* दत्त_B, T* दत्त_C, size_t चरणः_A, size_t चरणः_B, size_t चरणः_C, size_t पङ्क्तिकाः, size_t स्तम्भसङ्ख्या, size_t j0, size_t j1, तार्किक is_1d, तार्किक isCN1)
{
    यदि (!is_1d && isCN1)
    {
        mul_Spectrums_process_स्तम्भसङ्ख्या<T, conjB, inplaceA>(दत्त_A, दत्त_B, दत्त_C, चरणः_A, चरणः_B, चरणः_C, पङ्क्तिकाः, स्तम्भसङ्ख्या);
    }
    mul_Spectrums_process_पङ्क्तिकाः<T, conjB, inplaceA>(दत्त_A, दत्त_B, दत्त_C, चरणः_A, चरणः_B, चरणः_C, पङ्क्तिकाः, स्तम्भसङ्ख्या, j0, j1, is_1d && isCN1);
}
प्रतिरूपम् <प्रकारनाम T, तार्किक conjB> स्थिर अन्तःस्थ
शून्य mulSpectrums_Impl(ध्रुव T* दत्त_A, ध्रुव T* दत्त_B, T* दत्त_C, size_t चरणः_A, size_t चरणः_B, size_t चरणः_C, size_t पङ्क्तिकाः, size_t स्तम्भसङ्ख्या, size_t j0, size_t j1, तार्किक is_1d, तार्किक isCN1)
{
    यदि (दत्त_A == दत्त_C)
        mulSpectrums_Impl_<T, conjB, सत्य>(दत्त_A, दत्त_B, दत्त_C, चरणः_A, चरणः_B, चरणः_C, पङ्क्तिकाः, स्तम्भसङ्ख्या, j0, j1, is_1d, isCN1);
    अन्यथा
        mulSpectrums_Impl_<T, conjB, असत्य>(दत्त_A, दत्त_B, दत्त_C, चरणः_A, चरणः_B, चरणः_C, पङ्क्तिकाः, स्तम्भसङ्ख्या, j0, j1, is_1d, isCN1);
}

} // namespace

शून्य cv::mulSpectrums( निवेशः_Array _srcA, निवेशः_Array _srcB,
                       OutputArray _dst, पूर्णाङ्क flags, तार्किक conjB )
{
    CV_INSTRUMENT_REGION();

    यदि (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        प्रत्यागम;
    }

    पूर्णाङ्क type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_सततम्(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    पूर्णाङ्क गभीरता = srcA.गभीरता(), cn = srcA.channels();
    size_t पङ्क्तिकाः = srcA.पङ्क्तिकाः, स्तम्भसङ्ख्या = srcA.स्तम्भसङ्ख्या;

    _dst.create( srcA.पङ्क्तिकाः, srcA.स्तम्भसङ्ख्या, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    यदि (dst.दत्त == srcB.दत्त)
        srcB = srcB.clone(); // workaround for B only

    तार्किक is_1d = (flags & DFT_पङ्क्तिकाः)
        || (पङ्क्तिकाः == 1)
        || (स्तम्भसङ्ख्या == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    यदि( is_1d && !(flags & DFT_पङ्क्तिकाः) )
        स्तम्भसङ्ख्या = स्तम्भसङ्ख्या + पङ्क्तिकाः - 1, पङ्क्तिकाः = 1;

    तार्किक isCN1 = cn == 1;
    size_t j0 = isCN1 ? 1 : 0;
    size_t j1 = स्तम्भसङ्ख्या*cn - (((स्तम्भसङ्ख्या & 1) == 0 && cn == 1) ? 1 : 0);

    यदि (गभीरता == CV_32F)
    {
        ध्रुव भिन्न* दत्त_A = srcA.ptr<भिन्न>();
        ध्रुव भिन्न* दत्त_B = srcB.ptr<भिन्न>();
        भिन्न* दत्त_C = dst.ptr<भिन्न>();
        यदि (!conjB)
            mulSpectrums_Impl<भिन्न, असत्य>(दत्त_A, दत्त_B, दत्त_C, srcA.चरणः, srcB.चरणः, dst.चरणः, पङ्क्तिकाः, स्तम्भसङ्ख्या, j0, j1, is_1d, isCN1);
        अन्यथा
            mulSpectrums_Impl<भिन्न, सत्य>(दत्त_A, दत्त_B, दत्त_C, srcA.चरणः, srcB.चरणः, dst.चरणः, पङ्क्तिकाः, स्तम्भसङ्ख्या, j0, j1, is_1d, isCN1);
    }
    अन्यथा
    {
        ध्रुव द्विगुण* दत्त_A = srcA.ptr<द्विगुण>();
        ध्रुव द्विगुण* दत्त_B = srcB.ptr<द्विगुण>();
        द्विगुण* दत्त_C = dst.ptr<द्विगुण>();
        यदि (!conjB)
            mulSpectrums_Impl<द्विगुण, असत्य>(दत्त_A, दत्त_B, दत्त_C, srcA.चरणः, srcB.चरणः, dst.चरणः, पङ्क्तिकाः, स्तम्भसङ्ख्या, j0, j1, is_1d, isCN1);
        अन्यथा
            mulSpectrums_Impl<द्विगुण, सत्य>(दत्त_A, दत्त_B, दत्त_C, srcA.चरणः, srcB.चरणः, dst.चरणः, पङ्क्तिकाः, स्तम्भसङ्ख्या, j0, j1, is_1d, isCN1);
    }
}

शून्य cv::divSpectrums( निवेशः_Array _srcA, निवेशः_Array _srcB, OutputArray _dst, पूर्णाङ्क flags, तार्किक conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    यदि (srcA.empty() && srcB.empty())
    {
        _dst.release();
        प्रत्यागम;
    }
    पूर्णाङ्क गभीरता = srcA.गभीरता(), cn = srcA.channels(), type = srcA.type();
    पूर्णाङ्क पङ्क्तिकाः = srcA.पङ्क्तिकाः, स्तम्भसङ्ख्या = srcA.स्तम्भसङ्ख्या;
    पूर्णाङ्क j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.पङ्क्तिकाः, srcA.स्तम्भसङ्ख्या, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.दत्त != srcA.दत्त); // non-inplace check
    CV_Assert(dst.दत्त != srcB.दत्त); // non-inplace check

    तार्किक is_1d = (flags & DFT_पङ्क्तिकाः) || (पङ्क्तिकाः == 1 || (स्तम्भसङ्ख्या == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    यदि( is_1d && !(flags & DFT_पङ्क्तिकाः) )
        स्तम्भसङ्ख्या = स्तम्भसङ्ख्या + पङ्क्तिकाः - 1, पङ्क्तिकाः = 1;

    पूर्णाङ्क ncols = स्तम्भसङ्ख्या*cn;
    पूर्णाङ्क j0 = cn == 1;
    पूर्णाङ्क j1 = ncols - (स्तम्भसङ्ख्या % 2 == 0 && cn == 1);

    यदि( गभीरता == CV_32F )
    {
        ध्रुव भिन्न* दत्त_A = srcA.ptr<भिन्न>();
        ध्रुव भिन्न* दत्त_B = srcB.ptr<भिन्न>();
        भिन्न* दत्त_C = dst.ptr<भिन्न>();
        भिन्न eps = FLT_EPSILON; // prevent div0 problems

        size_t चरणः_A = srcA.चरणः/परिमाण(दत्त_A[0]);
        size_t चरणः_B = srcB.चरणः/परिमाण(दत्त_B[0]);
        size_t चरणः_C = dst.चरणः/परिमाण(दत्त_C[0]);

        यदि( !is_1d && cn == 1 )
        {
            प्रति( k = 0; k < (स्तम्भसङ्ख्या % 2 ? 1 : 2); k++ )
            {
                यदि( k == 1 )
                    दत्त_A += स्तम्भसङ्ख्या - 1, दत्त_B += स्तम्भसङ्ख्या - 1, दत्त_C += स्तम्भसङ्ख्या - 1;
                दत्त_C[0] = दत्त_A[0] / (दत्त_B[0] + eps);
                यदि( पङ्क्तिकाः % 2 == 0 )
                    दत्त_C[(पङ्क्तिकाः-1)*चरणः_C] = दत्त_A[(पङ्क्तिकाः-1)*चरणः_A] / (दत्त_B[(पङ्क्तिकाः-1)*चरणः_B] + eps);
                यदि( !conjB )
                    प्रति( j = 1; j <= पङ्क्तिकाः - 2; j += 2 )
                    {
                        द्विगुण denom = (द्विगुण)दत्त_B[j*चरणः_B]*दत्त_B[j*चरणः_B] +
                        (द्विगुण)दत्त_B[(j+1)*चरणः_B]*दत्त_B[(j+1)*चरणः_B] + (द्विगुण)eps;

                        द्विगुण re = (द्विगुण)दत्त_A[j*चरणः_A]*दत्त_B[j*चरणः_B] +
                        (द्विगुण)दत्त_A[(j+1)*चरणः_A]*दत्त_B[(j+1)*चरणः_B];

                        द्विगुण im = (द्विगुण)दत्त_A[(j+1)*चरणः_A]*दत्त_B[j*चरणः_B] -
                        (द्विगुण)दत्त_A[j*चरणः_A]*दत्त_B[(j+1)*चरणः_B];

                        दत्त_C[j*चरणः_C] = (भिन्न)(re / denom);
                        दत्त_C[(j+1)*चरणः_C] = (भिन्न)(im / denom);
                    }
                    अन्यथा
                        प्रति( j = 1; j <= पङ्क्तिकाः - 2; j += 2 )
                        {

                            द्विगुण denom = (द्विगुण)दत्त_B[j*चरणः_B]*दत्त_B[j*चरणः_B] +
                            (द्विगुण)दत्त_B[(j+1)*चरणः_B]*दत्त_B[(j+1)*चरणः_B] + (द्विगुण)eps;

                            द्विगुण re = (द्विगुण)दत्त_A[j*चरणः_A]*दत्त_B[j*चरणः_B] -
                            (द्विगुण)दत्त_A[(j+1)*चरणः_A]*दत्त_B[(j+1)*चरणः_B];

                            द्विगुण im = (द्विगुण)दत्त_A[(j+1)*चरणः_A]*दत्त_B[j*चरणः_B] +
                            (द्विगुण)दत्त_A[j*चरणः_A]*दत्त_B[(j+1)*चरणः_B];

                            दत्त_C[j*चरणः_C] = (भिन्न)(re / denom);
                            दत्त_C[(j+1)*चरणः_C] = (भिन्न)(im / denom);
                        }
                    यदि( k == 1 )
                        दत्त_A -= स्तम्भसङ्ख्या - 1, दत्त_B -= स्तम्भसङ्ख्या - 1, दत्त_C -= स्तम्भसङ्ख्या - 1;
            }
        }

        प्रति( ; पङ्क्तिकाः--; दत्त_A += चरणः_A, दत्त_B += चरणः_B, दत्त_C += चरणः_C )
        {
            यदि( is_1d && cn == 1 )
            {
                दत्त_C[0] = दत्त_A[0] / (दत्त_B[0] + eps);
                यदि( स्तम्भसङ्ख्या % 2 == 0 )
                    दत्त_C[j1] = दत्त_A[j1] / (दत्त_B[j1] + eps);
            }

            यदि( !conjB )
                प्रति( j = j0; j < j1; j += 2 )
                {
                    द्विगुण denom = (द्विगुण)दत्त_B[j]*दत्त_B[j] + (द्विगुण)दत्त_B[j+1]*दत्त_B[j+1] + (द्विगुण)eps;
                    द्विगुण re = (द्विगुण)दत्त_A[j]*दत्त_B[j] + (द्विगुण)दत्त_A[j+1]*दत्त_B[j+1];
                    द्विगुण im = (द्विगुण)दत्त_A[j+1]*दत्त_B[j] - (द्विगुण)दत्त_A[j]*दत्त_B[j+1];
                    दत्त_C[j] = (भिन्न)(re / denom);
                    दत्त_C[j+1] = (भिन्न)(im / denom);
                }
                अन्यथा
                    प्रति( j = j0; j < j1; j += 2 )
                    {
                        द्विगुण denom = (द्विगुण)दत्त_B[j]*दत्त_B[j] + (द्विगुण)दत्त_B[j+1]*दत्त_B[j+1] + (द्विगुण)eps;
                        द्विगुण re = (द्विगुण)दत्त_A[j]*दत्त_B[j] - (द्विगुण)दत्त_A[j+1]*दत्त_B[j+1];
                        द्विगुण im = (द्विगुण)दत्त_A[j+1]*दत्त_B[j] + (द्विगुण)दत्त_A[j]*दत्त_B[j+1];
                        दत्त_C[j] = (भिन्न)(re / denom);
                        दत्त_C[j+1] = (भिन्न)(im / denom);
                    }
        }
    }
    अन्यथा
    {
        ध्रुव द्विगुण* दत्त_A = srcA.ptr<द्विगुण>();
        ध्रुव द्विगुण* दत्त_B = srcB.ptr<द्विगुण>();
        द्विगुण* दत्त_C = dst.ptr<द्विगुण>();
        द्विगुण eps = DBL_EPSILON; // prevent div0 problems

        size_t चरणः_A = srcA.चरणः/परिमाण(दत्त_A[0]);
        size_t चरणः_B = srcB.चरणः/परिमाण(दत्त_B[0]);
        size_t चरणः_C = dst.चरणः/परिमाण(दत्त_C[0]);

        यदि( !is_1d && cn == 1 )
        {
            प्रति( k = 0; k < (स्तम्भसङ्ख्या % 2 ? 1 : 2); k++ )
            {
                यदि( k == 1 )
                    दत्त_A += स्तम्भसङ्ख्या - 1, दत्त_B += स्तम्भसङ्ख्या - 1, दत्त_C += स्तम्भसङ्ख्या - 1;
                दत्त_C[0] = दत्त_A[0] / (दत्त_B[0] + eps);
                यदि( पङ्क्तिकाः % 2 == 0 )
                    दत्त_C[(पङ्क्तिकाः-1)*चरणः_C] = दत्त_A[(पङ्क्तिकाः-1)*चरणः_A] / (दत्त_B[(पङ्क्तिकाः-1)*चरणः_B] + eps);
                यदि( !conjB )
                    प्रति( j = 1; j <= पङ्क्तिकाः - 2; j += 2 )
                    {
                        द्विगुण denom = दत्त_B[j*चरणः_B]*दत्त_B[j*चरणः_B] +
                        दत्त_B[(j+1)*चरणः_B]*दत्त_B[(j+1)*चरणः_B] + eps;

                        द्विगुण re = दत्त_A[j*चरणः_A]*दत्त_B[j*चरणः_B] +
                        दत्त_A[(j+1)*चरणः_A]*दत्त_B[(j+1)*चरणः_B];

                        द्विगुण im = दत्त_A[(j+1)*चरणः_A]*दत्त_B[j*चरणः_B] -
                        दत्त_A[j*चरणः_A]*दत्त_B[(j+1)*चरणः_B];

                        दत्त_C[j*चरणः_C] = re / denom;
                        दत्त_C[(j+1)*चरणः_C] = im / denom;
                    }
                    अन्यथा
                        प्रति( j = 1; j <= पङ्क्तिकाः - 2; j += 2 )
                        {
                            द्विगुण denom = दत्त_B[j*चरणः_B]*दत्त_B[j*चरणः_B] +
                            दत्त_B[(j+1)*चरणः_B]*दत्त_B[(j+1)*चरणः_B] + eps;

                            द्विगुण re = दत्त_A[j*चरणः_A]*दत्त_B[j*चरणः_B] -
                            दत्त_A[(j+1)*चरणः_A]*दत्त_B[(j+1)*चरणः_B];

                            द्विगुण im = दत्त_A[(j+1)*चरणः_A]*दत्त_B[j*चरणः_B] +
                            दत्त_A[j*चरणः_A]*दत्त_B[(j+1)*चरणः_B];

                            दत्त_C[j*चरणः_C] = re / denom;
                            दत्त_C[(j+1)*चरणः_C] = im / denom;
                        }
                    यदि( k == 1 )
                        दत्त_A -= स्तम्भसङ्ख्या - 1, दत्त_B -= स्तम्भसङ्ख्या - 1, दत्त_C -= स्तम्भसङ्ख्या - 1;
            }
        }

        प्रति( ; पङ्क्तिकाः--; दत्त_A += चरणः_A, दत्त_B += चरणः_B, दत्त_C += चरणः_C )
        {
            यदि( is_1d && cn == 1 )
            {
                दत्त_C[0] = दत्त_A[0] / (दत्त_B[0] + eps);
                यदि( स्तम्भसङ्ख्या % 2 == 0 )
                    दत्त_C[j1] = दत्त_A[j1] / (दत्त_B[j1] + eps);
            }

            यदि( !conjB )
                प्रति( j = j0; j < j1; j += 2 )
                {
                    द्विगुण denom = दत्त_B[j]*दत्त_B[j] + दत्त_B[j+1]*दत्त_B[j+1] + eps;
                    द्विगुण re = दत्त_A[j]*दत्त_B[j] + दत्त_A[j+1]*दत्त_B[j+1];
                    द्विगुण im = दत्त_A[j+1]*दत्त_B[j] - दत्त_A[j]*दत्त_B[j+1];
                    दत्त_C[j] = re / denom;
                    दत्त_C[j+1] = im / denom;
                }
                अन्यथा
                    प्रति( j = j0; j < j1; j += 2 )
                    {
                        द्विगुण denom = दत्त_B[j]*दत्त_B[j] + दत्त_B[j+1]*दत्त_B[j+1] + eps;
                        द्विगुण re = दत्त_A[j]*दत्त_B[j] - दत्त_A[j+1]*दत्त_B[j+1];
                        द्विगुण im = दत्त_A[j+1]*दत्त_B[j] + दत्त_A[j]*दत्त_B[j+1];
                        दत्त_C[j] = re / denom;
                        दत्त_C[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

नामाकाश cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
DCT( ध्रुव OcvDftOptions & c, ध्रुव T* src, size_t src_चरणः, T* dst, size_t dst_चरणः )
{
    यदि( c.n == 1 )
    {
        dst[0] = src[0];
        प्रत्यागम;
    }
    सततम्_Dft(c, src, src_चरणः/परिमाण(T), dst, dst_चरणः/परिमाण(T), असत्य);
}

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
IDCT( ध्रुव OcvDftOptions & c, ध्रुव T* src, size_t src_चरणः, T* dst, size_t dst_चरणः )
{
    यदि( c.n == 1 )
    {
        dst[0] = src[0];
        प्रत्यागम;
    }
    सततम्_Dft(c, src, src_चरणः/परिमाण(T), dst, dst_चरणः/परिमाण(T), असत्य);
}

प्रकारनिर्देश शून्य (*DCTFunc)(ध्रुव OcvDftOptions & c, ध्रुव शून्य* src, size_t src_चरणः,
                        शून्य* dst, size_t dst_चरणः);

प्रतिरूपम्<प्रकारनाम T, शून्य (*fn)(ध्रुव OcvDftOptions&, ध्रुव T*, size_t, T*, size_t)>
स्थिर शून्य dctWrap(ध्रुव OcvDftOptions & c, ध्रुव शून्य* src, size_t src_चरणः, शून्य* dst, size_t dst_चरणः)
{
    fn(c, (ध्रुव T*)src, src_चरणः, (T*)dst, dst_चरणः);
}

}

#यदि_परिभाषितम् HAVE_IPP
नामाकाश cv
{

#यदि IPP_संस्करणम्_X100 >= 900
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * ippiDCTFunc)(ध्रुव Ipp32f* pSrc, पूर्णाङ्क src_चरणः_3, Ipp32f* pDst, पूर्णाङ्क dst_चरणः_3, ध्रुव शून्य* pDCTSpec, Ipp8u* p_बफर);
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * ippiDCTInit)(शून्य* pDCTSpec, Ippi_आकार roi_आकार, Ipp8u* pMemInit );
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * ippi_DCTGet_आकार)(Ippi_आकार roi_आकार, पूर्णाङ्क* p_आकार_Spec, पूर्णाङ्क* p_आकार_Init, पूर्णाङ्क* p_आकार_Buf);

प्रतिरूपम्<प्रकारनाम SpecType, Ipp_स्थितिः (CV_STDCALL *fn)(ध्रुव Ipp32f*, पूर्णाङ्क, Ipp32f*, पूर्णाङ्क, ध्रुव SpecType*, Ipp8u*)>
स्थिर Ipp_स्थितिः CV_STDCALL ippiDCTWrap(ध्रुव Ipp32f* pSrc, पूर्णाङ्क src_चरणः_3, Ipp32f* pDst, पूर्णाङ्क dst_चरणः_3, ध्रुव शून्य* pDCTSpec, Ipp8u* p_बफर)
{
    प्रत्यागम fn(pSrc, src_चरणः_3, pDst, dst_चरणः_3, (ध्रुव SpecType*)pDCTSpec, p_बफर);
}

प्रतिरूपम्<प्रकारनाम SpecType, Ipp_स्थितिः (CV_STDCALL *fn)(SpecType*, Ippi_आकार, Ipp8u*)>
स्थिर Ipp_स्थितिः CV_STDCALL ippiDCTInitWrap(शून्य* pDCTSpec, Ippi_आकार roi_आकार, Ipp8u* pMemInit)
{
    प्रत्यागम fn((SpecType*)pDCTSpec, roi_आकार, pMemInit);
}

प्रतिरूपम्<Ipp_स्थितिः (CV_STDCALL *fn)(Ippi_आकार, पूर्णाङ्क*, पूर्णाङ्क*, पूर्णाङ्क*)>
स्थिर Ipp_स्थितिः CV_STDCALL ippi_DCTGet_आकार_Wrap(Ippi_आकार roi_आकार, पूर्णाङ्क* p_आकार_Spec, पूर्णाङ्क* p_आकार_Init, पूर्णाङ्क* p_आकार_Buf)
{
    प्रत्यागम fn(roi_आकार, p_आकार_Spec, p_आकार_Init, p_आकार_Buf);
}

#नोचेद्यदि IPP_संस्करणम्_X100 >= 700
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * ippiDCTFunc)(ध्रुव Ipp32f*, पूर्णाङ्क, Ipp32f*, पूर्णाङ्क, ध्रुव शून्य*, Ipp8u*);
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * ippiDCTInitAlloc)(शून्य**, Ippi_आकार, IppHintAlgorithm);
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * ippiDCTFree)(शून्य* pDCTSpec);
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * ippi_DCTGet_Buf_आकार)(ध्रुव शून्य*, पूर्णाङ्क*);

प्रतिरूपम्<प्रकारनाम SpecType, Ipp_स्थितिः (CV_STDCALL *fn)(ध्रुव Ipp32f*, पूर्णाङ्क, Ipp32f*, पूर्णाङ्क, ध्रुव SpecType*, Ipp8u*)>
स्थिर Ipp_स्थितिः CV_STDCALL ippiDCTWrap(ध्रुव Ipp32f* pSrc, पूर्णाङ्क src_चरणः_3, Ipp32f* pDst, पूर्णाङ्क dst_चरणः_3, ध्रुव शून्य* pDCTSpec, Ipp8u* p_बफर)
{
    प्रत्यागम fn(pSrc, src_चरणः_3, pDst, dst_चरणः_3, (ध्रुव SpecType*)pDCTSpec, p_बफर);
}

प्रतिरूपम्<प्रकारनाम SpecType, Ipp_स्थितिः (CV_STDCALL *fn)(SpecType**, Ippi_आकार, IppHintAlgorithm)>
स्थिर Ipp_स्थितिः CV_STDCALL ippiDCTInitAllocWrap(शून्य** pDCTSpec, Ippi_आकार roi_आकार, IppHintAlgorithm hint)
{
    प्रत्यागम fn((SpecType**)pDCTSpec, roi_आकार, hint);
}

प्रतिरूपम्<प्रकारनाम SpecType, Ipp_स्थितिः (CV_STDCALL *fn)(SpecType*)>
स्थिर Ipp_स्थितिः CV_STDCALL ippiDCTFreeWrap(शून्य* pDCTSpec)
{
    प्रत्यागम fn((SpecType*)pDCTSpec);
}

प्रतिरूपम्<प्रकारनाम SpecType, Ipp_स्थितिः (CV_STDCALL *fn)(ध्रुव SpecType*, पूर्णाङ्क*)>
स्थिर Ipp_स्थितिः CV_STDCALL ippi_DCTGet_Buf_आकार_Wrap(ध्रुव शून्य* pDCTSpec, पूर्णाङ्क* p_आकार)
{
    प्रत्यागम fn((ध्रुव SpecType*)pDCTSpec, p_आकार);
}
#यदि_अन्तः

वर्ग DctIPPLoop_Invoker : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    DctIPPLoop_Invoker(ध्रुव uchar * _src, size_t src_चरणः_2, uchar * _dst, size_t dst_चरणः_2, पूर्णाङ्क विस्तारः_3, तार्किक _inv, तार्किक *_ok) :
        ParallelLoopBody(), src(_src), src_चरणः(src_चरणः_2), dst(_dst), dst_चरणः(dst_चरणः_2), विस्तारः(विस्तारः_3), inv(_inv), ok(_ok)
    {
        *ok = सत्य;
    }

    आभास शून्य संक्रियकः()(ध्रुव Range& range) ध्रुव CV_OVERRIDE
    {
        यदि(*ok == असत्य)
            प्रत्यागम;

#यदि IPP_संस्करणम्_X100 >= 900
        Ippi_आकार src_Roi_आकार = {विस्तारः, 1};

        पूर्णाङ्क spec_आकार    = 0;
        पूर्णाङ्क init_आकार    = 0;
        पूर्णाङ्क बफर_आकार  = 0;

        Ipp8u* pDCTSpec = रिक्त;
        Ipp8u* p_बफर  = रिक्त;
        Ipp8u* pInitBuf = रिक्त;

        #परिभाषय IPP_RETURN              \
            यदि(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            यदि(p_बफर)                 \
                ippFree(p_बफर);       \
            यदि(pInitBuf)                \
                ippFree(pInitBuf);      \
            प्रत्यागम;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_आकार  ipp_Dct_Get_आकार  = inv ? ippi_DCTGet_आकार_Wrap<ippi_DCTInv_Get_आकार_32f> : ippi_DCTGet_आकार_Wrap<ippi_DCTFwd_Get_आकार_32f>;

        यदि(ipp_Dct_Get_आकार(src_Roi_आकार, &spec_आकार, &init_आकार, &बफर_आकार) < 0)
        {
            *ok = असत्य;
            प्रत्यागम;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_आकार);
        यदि(!pDCTSpec && spec_आकार)
        {
            *ok = असत्य;
            प्रत्यागम;
        }

        p_बफर  = (Ipp8u*)CV_IPP_MALLOC(बफर_आकार);
        यदि(!p_बफर && बफर_आकार)
        {
            *ok = असत्य;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_आकार);
        यदि(!pInitBuf && init_आकार)
        {
            *ok = असत्य;
            IPP_RETURN
        }

        यदि(ippDctInit(pDCTSpec, src_Roi_आकार, pInitBuf) < 0)
        {
            *ok = असत्य;
            IPP_RETURN
        }

        प्रति(पूर्णाङ्क i = range.start; i < range.end; ++i)
        {
            यदि(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (भिन्न*)(src + src_चरणः * i), static_cast<पूर्णाङ्क>(src_चरणः), (भिन्न*)(dst + dst_चरणः * i), static_cast<पूर्णाङ्क>(dst_चरणः), pDCTSpec, p_बफर) < 0)
            {
                *ok = असत्य;
                IPP_RETURN
            }
        }
        IPP_RETURN
#अपरिभाषय IPP_RETURN
#नोचेद्यदि IPP_संस्करणम्_X100 >= 700
        शून्य* pDCTSpec;
        Auto_बफर<uchar> buf;
        uchar* p_बफर = 0;
        पूर्णाङ्क buf_आकार=0;

        Ippi_आकार src_Roi_आकार = {विस्तारः, 1};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_आकार ipp_Get_Buf_आकार = inv ? ippi_DCTGet_Buf_आकार_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_आकार_32f> : ippi_DCTGet_Buf_आकार_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_आकार_32f>;

        यदि (ippInitAlloc(&pDCTSpec, src_Roi_आकार, ippAlgHintNone)>=0 && ipp_Get_Buf_आकार(pDCTSpec, &buf_आकार)>=0)
        {
            buf.allocate( buf_आकार );
            p_बफर = (uchar*)buf;

            प्रति( पूर्णाङ्क i = range.start; i < range.end; ++i)
            {
                यदि(ippDctFun((भिन्न*)(src + src_चरणः * i), static_cast<पूर्णाङ्क>(src_चरणः), (भिन्न*)(dst + dst_चरणः * i), static_cast<पूर्णाङ्क>(dst_चरणः), pDCTSpec, (Ipp8u*)p_बफर) < 0)
                {
                    *ok = असत्य;
                    भङ्ग;
                }
            }
        }
        अन्यथा
            *ok = असत्य;

        यदि (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_अन्तः
#अन्यथा
        CV_अप्रयुक्तम्(range);
        *ok = असत्य;
#यदि_अन्तः
    }

गुप्त:
    ध्रुव uchar * src;
    size_t src_चरणः;
    uchar * dst;
    size_t dst_चरणः;
    पूर्णाङ्क विस्तारः;
    तार्किक inv;
    तार्किक *ok;
};

स्थिर तार्किक DctIPPLoop(ध्रुव uchar * src, size_t src_चरणः, uchar * dst, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, तार्किक inv)
{
    तार्किक ok;
    parallel_for_(Range(0, height), DctIPPLoop_Invoker(src, src_चरणः, dst, dst_चरणः, विस्तारः, inv, &ok), height/(द्विगुण)(1<<4) );
    प्रत्यागम ok;
}

स्थिर तार्किक ippi_DCT_32f(ध्रुव uchar * src, size_t src_चरणः, uchar * dst, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, तार्किक inv, तार्किक row)
{
    CV_INSTRUMENT_REGION_IPP();

    यदि(row)
        प्रत्यागम DctIPPLoop(src, src_चरणः, dst, dst_चरणः, विस्तारः, height, inv);
    अन्यथा
    {
#यदि IPP_संस्करणम्_X100 >= 900
        Ippi_आकार src_Roi_आकार = {विस्तारः, height};

        पूर्णाङ्क spec_आकार    = 0;
        पूर्णाङ्क init_आकार    = 0;
        पूर्णाङ्क बफर_आकार  = 0;

        Ipp8u* pDCTSpec = रिक्त;
        Ipp8u* p_बफर  = रिक्त;
        Ipp8u* pInitBuf = रिक्त;

        #परिभाषय IPP_RELEASE             \
            यदि(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            यदि(p_बफर)                 \
                ippFree(p_बफर);       \
            यदि(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_आकार  ipp_Dct_Get_आकार  = inv ? ippi_DCTGet_आकार_Wrap<ippi_DCTInv_Get_आकार_32f> : ippi_DCTGet_आकार_Wrap<ippi_DCTFwd_Get_आकार_32f>;

        यदि(ipp_Dct_Get_आकार(src_Roi_आकार, &spec_आकार, &init_आकार, &बफर_आकार) < 0)
            प्रत्यागम असत्य;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_आकार);
        यदि(!pDCTSpec && spec_आकार)
            प्रत्यागम असत्य;

        p_बफर  = (Ipp8u*)CV_IPP_MALLOC(बफर_आकार);
        यदि(!p_बफर && बफर_आकार)
        {
            IPP_RELEASE
            प्रत्यागम असत्य;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_आकार);
        यदि(!pInitBuf && init_आकार)
        {
            IPP_RELEASE
            प्रत्यागम असत्य;
        }

        यदि(ippDctInit(pDCTSpec, src_Roi_आकार, pInitBuf) < 0)
        {
            IPP_RELEASE
            प्रत्यागम असत्य;
        }

        यदि(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (भिन्न*)src, static_cast<पूर्णाङ्क>(src_चरणः), (भिन्न*)dst, static_cast<पूर्णाङ्क>(dst_चरणः), pDCTSpec, p_बफर) < 0)
        {
            IPP_RELEASE
            प्रत्यागम असत्य;
        }

        IPP_RELEASE
        प्रत्यागम सत्य;
#अपरिभाषय IPP_RELEASE
#नोचेद्यदि IPP_संस्करणम्_X100 >= 700
        Ipp_स्थितिः स्थितिः;
        शून्य* pDCTSpec;
        Auto_बफर<uchar> buf;
        uchar* p_बफर = 0;
        पूर्णाङ्क buf_आकार=0;

        Ippi_आकार src_Roi_आकार = {विस्तारः, height};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_आकार ipp_Get_Buf_आकार = inv ? ippi_DCTGet_Buf_आकार_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_आकार_32f> : ippi_DCTGet_Buf_आकार_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_आकार_32f>;

        स्थितिः = ippStsErr;

        यदि (ippInitAlloc(&pDCTSpec, src_Roi_आकार, ippAlgHintNone)>=0 && ipp_Get_Buf_आकार(pDCTSpec, &buf_आकार)>=0)
        {
            buf.allocate( buf_आकार );
            p_बफर = (uchar*)buf;

            स्थितिः = ippDctFun((भिन्न*)src, static_cast<पूर्णाङ्क>(src_चरणः), (भिन्न*)dst, static_cast<पूर्णाङ्क>(dst_चरणः), pDCTSpec, (Ipp8u*)p_बफर);
        }

        यदि (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_अन्तः

        प्रत्यागम स्थितिः >= 0;
#अन्यथा
        CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(dst); CV_अप्रयुक्तम्(inv); CV_अप्रयुक्तम्(row);
        प्रत्यागम असत्य;
#यदि_अन्तः
    }
}
}
#यदि_अन्तः

नामाकाश cv {

वर्ग OcvDctImpl CV_FINAL : सार्वजनिक hal::DCT2D
{
सार्वजनिक:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    Auto_बफर<uchar> ws;
    तार्किक isRowTransform;
    तार्किक isInverse;
    तार्किक isContinuous;
    पूर्णाङ्क start_stage;
    पूर्णाङ्क अन्तः_stage;
    पूर्णाङ्क विस्तारः;
    पूर्णाङ्क height;
    पूर्णाङ्क गभीरता;

    शून्य init(पूर्णाङ्क विस्तारः_3, पूर्णाङ्क _height, पूर्णाङ्क गभीरता_2, पूर्णाङ्क flags)
    {
        विस्तारः = विस्तारः_3;
        height = _height;
        गभीरता = गभीरता_2;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        isRowTransform = (flags & CV_HAL_DFT_पङ्क्तिकाः) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        स्थिर DCTFunc dct_tbl[4] =
        {
            dctWrap<भिन्न, DCT<भिन्न>>,
            dctWrap<भिन्न, IDCT<भिन्न>>,
            dctWrap<द्विगुण, DCT<द्विगुण>>,
            dctWrap<द्विगुण, IDCT<द्विगुण>>
        };
        dct_func = dct_tbl[(पूर्णाङ्क)isInverse + (गभीरता == CV_64F)*2];
        opt.isComplex = असत्य;
        opt.isInverse = असत्य;
        opt.scale = 1.;

        यदि (isRowTransform || height == 1 || (विस्तारः == 1 && isContinuous))
        {
            start_stage = अन्तः_stage = 0;
        }
        अन्यथा
        {
            start_stage = (विस्तारः == 1);
            अन्तः_stage = 1;
        }
    }
    शून्य apply(ध्रुव uchar *src, size_t src_चरणः, uchar *dst, size_t dst_चरणः) CV_OVERRIDE
    {
        CV_IPP_सततम्(IPP_संस्करणम्_X100 >= 700 && गभीरता == CV_32F, ippi_DCT_32f(src, src_चरणः, dst, dst_चरणः, विस्तारः, height, isInverse, isRowTransform))

        पूर्णाङ्क पूर्व_len = 0;
        पूर्णाङ्क elem_आकार_2 = (गभीरता == CV_32F) ? परिमाण(भिन्न) : परिमाण(द्विगुण);
        ध्रुव DFTKernels& kernels = getDFTKernels(गभीरता);

        प्रति(पूर्णाङ्क stage = start_stage ; stage <= अन्तः_stage; stage++ )
        {
            ध्रुव uchar* sptr = src;
            uchar* dptr = dst;
            size_t sstep0, sstep1, dstep0, dstep1;
            पूर्णाङ्क len, गणनम्_2;

            यदि( stage == 0 )
            {
                len = विस्तारः;
                गणनम्_2 = height;
                यदि( len == 1 && !isRowTransform )
                {
                    len = height;
                    गणनम्_2 = 1;
                }
                sstep0 = src_चरणः;
                dstep0 = dst_चरणः;
                sstep1 = dstep1 = elem_आकार_2;
            }
            अन्यथा
            {
                len = height;
                गणनम्_2 = विस्तारः;
                sstep1 = src_चरणः;
                dstep1 = dst_चरणः;
                sstep0 = dstep0 = elem_आकार_2;
            }

            opt.n = len;

            यदि( len != पूर्व_len )
            {
                यदि( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                यदि( len > 1 )
                {
                    plan.build(isInverse ? DFT_विधा_IDCT : DFT_विधा_DCT, len, गभीरता, kernels.vlanes);
                    ws.allocate(plan.ws_अष्टकानि);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = ws.दत्त();
                पूर्व_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            द्विगुण nstripes = dftParallelStripes(गणनम्_2, len);
            यदि( len > 1 && गणनम्_2 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                स्वचालित process_पङ्क्तयः = [&](ध्रुव Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    Auto_बफर<uchar> ws_(plan.ws_अष्टकानि);
                    opt_.workspace = ws_.दत्त();
                    प्रति( पूर्णाङ्क i = range.start; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, गणनम्_2), process_पङ्क्तयः, nstripes);
            }
            अन्यथा
            {
                प्रति(अचिह्नित i = 0; i < static_cast<अचिह्नित>(गणनम्_2); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_चरणः = dst_चरणः;
        }
    }
};

संरचना ReplacementDCT2D : सार्वजनिक hal::DCT2D
{
    cvhalDFT *context;
    तार्किक isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(असत्य) {}
    तार्किक init(पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क गभीरता, पूर्णाङ्क flags)
    {
        पूर्णाङ्क res = cv_hal_dctInit2D(&context, विस्तारः, height, गभीरता, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        प्रत्यागम isInitialized;
    }
    शून्य apply(ध्रुव uchar *src_दत्त, size_t src_चरणः, uchar *dst_दत्त, size_t dst_चरणः) CV_OVERRIDE
    {
        यदि (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_दत्त, src_चरणः, dst_दत्त, dst_चरणः);
        }
    }
    ~ReplacementDCT2D()
    {
        यदि (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

नामाकाश hal {

Ptr<DCT2D> DCT2D::create(पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क गभीरता, पूर्णाङ्क flags)
{
    {
        ReplacementDCT2D *impl = नूतन ReplacementDCT2D();
        यदि (impl->init(विस्तारः, height, गभीरता, flags))
        {
            प्रत्यागम Ptr<DCT2D>(impl);
        }
        विलोपय impl;
    }
    {
        OcvDctImpl *impl = नूतन OcvDctImpl();
        impl->init(विस्तारः, height, गभीरता, flags);
        प्रत्यागम Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

शून्य cv::dct( निवेशः_Array _src0, OutputArray _dst, पूर्णाङ्क flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    यदि (src.empty())
    {
        _dst.release();
        प्रत्यागम;
    }
    पूर्णाङ्क type = src.type(), गभीरता = src.गभीरता();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.पङ्क्तिकाः, src.स्तम्भसङ्ख्या, type );
    Mat dst = _dst.getMat();

    पूर्णाङ्क f = 0;
    यदि ((flags & DFT_पङ्क्तिकाः) != 0)
        f |= CV_HAL_DFT_पङ्क्तिकाः;
    यदि ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    यदि (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.स्तम्भसङ्ख्या, src.पङ्क्तिकाः, गभीरता, f);
    c->apply(src.दत्त, src.चरणः, dst.दत्त, dst.चरणः);
}


शून्य cv::idct( निवेशः_Array src, OutputArray dst, पूर्णाङ्क flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

नामाकाश cv
{

स्थिर ध्रुव पूर्णाङ्क optimalDFTSizeTab[] = {
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

पूर्णाङ्क cv::getOptimalDFTSize( पूर्णाङ्क size0 )
{
    पूर्णाङ्क a = 0, b = परिमाण(optimalDFTSizeTab)/परिमाण(optimalDFTSizeTab[0]) - 1;
    यदि( (अचिह्नित)size0 >= (अचिह्नित)optimalDFTSizeTab[b] )
        प्रत्यागम -1;

    यावत्( a < b )
    {
        पूर्णाङ्क c = (a + b) >> 1;
        यदि( size0 <= optimalDFTSizeTab[c] )
            b = c;
        अन्यथा
            a = c+1;
    }

    प्रत्यागम optimalDFTSizeTab[b];
}
/* End of file. */
