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

#समावेश "precomp.hpp"
#समावेश "opencv2/core/opencl/runtime/opencl_clfft.hpp"
#समावेश "opencv2/core/opencl/runtime/opencl_core.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "dxt.hpp"
#समावेश "dxt.simd.hpp"
#समावेश "dxt.simd_declarations.hpp"
#समावेश <map>

namespace cv
{

#अगर IPP_VERSION_X100 >= 710
#घोषणा USE_IPP_DFT 1
#अन्यथा
#अघोषित USE_IPP_DFT
#पूर्ण_अगर

#अगर defined USE_IPP_DFT
#अगर IPP_VERSION_X100 >= 202220
#घोषणा IPP_DISABLE_DFT32F ((depth == CV_32F) && (ippCPUID_AVX512F&cv::ipp::getIppFeatures()))
#अन्यथा
#घोषणा IPP_DISABLE_DFT32F false
#पूर्ण_अगर
#पूर्ण_अगर

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#अगर_घोषित HAVE_OPENCL
// factorization used by the OpenCL radix selection
अटल पूर्णांक
DFTFactorize( पूर्णांक n, पूर्णांक* factors )
{
    पूर्णांक nf = 0, f, i, j;

    अगर( n <= 5 )
    {
        factors[0] = n;
        वापस 1;
    }

    f = (((n - 1)^n)+1) >> 1;
    अगर( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    क्रम( f = 3; n > 1; )
    {
        पूर्णांक d = n/f;
        अगर( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        अन्यथा
        {
            f += 2;
            अगर( f*f > n )
                अवरोध;
        }
    }

    अगर( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    क्रम( i = f; i < (nf+f)/2; i++ )
        CV_SWAP( factors[i], factors[nf-i-1+f], j );

    वापस nf;
}
#पूर्ण_अगर

#अगर_घोषित USE_IPP_DFT
अटल IppStatus ippsDFTFwd_CToC( स्थिर Complex<भग्न>* src, Complex<भग्न>* dst,
                             स्थिर व्योम* spec, uchar* buf)
{
    वापस CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_32fc, (स्थिर Ipp32fc*)src, (Ipp32fc*)dst,
                                 (स्थिर IppsDFTSpec_C_32fc*)spec, buf);
}

अटल IppStatus ippsDFTFwd_CToC( स्थिर Complex<द्विगुन>* src, Complex<द्विगुन>* dst,
                             स्थिर व्योम* spec, uchar* buf)
{
    वापस CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_CToC_64fc, (स्थिर Ipp64fc*)src, (Ipp64fc*)dst,
                                 (स्थिर IppsDFTSpec_C_64fc*)spec, buf);
}

अटल IppStatus ippsDFTInv_CToC( स्थिर Complex<भग्न>* src, Complex<भग्न>* dst,
                             स्थिर व्योम* spec, uchar* buf)
{
    वापस CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_32fc, (स्थिर Ipp32fc*)src, (Ipp32fc*)dst,
                                 (स्थिर IppsDFTSpec_C_32fc*)spec, buf);
}

अटल IppStatus ippsDFTInv_CToC( स्थिर Complex<द्विगुन>* src, Complex<द्विगुन>* dst,
                                  स्थिर व्योम* spec, uchar* buf)
{
    वापस CV_INSTRUMENT_FUN_IPP(ippsDFTInv_CToC_64fc, (स्थिर Ipp64fc*)src, (Ipp64fc*)dst,
                                 (स्थिर IppsDFTSpec_C_64fc*)spec, buf);
}

अटल IppStatus ippsDFTFwd_RToPack( स्थिर भग्न* src, भग्न* dst,
                                     स्थिर व्योम* spec, uchar* buf)
{
    वापस CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_32f, src, dst, (स्थिर IppsDFTSpec_R_32f*)spec, buf);
}

अटल IppStatus ippsDFTFwd_RToPack( स्थिर द्विगुन* src, द्विगुन* dst,
                                     स्थिर व्योम* spec, uchar* buf)
{
    वापस CV_INSTRUMENT_FUN_IPP(ippsDFTFwd_RToPack_64f, src, dst, (स्थिर IppsDFTSpec_R_64f*)spec, buf);
}

अटल IppStatus ippsDFTInv_PackToR( स्थिर भग्न* src, भग्न* dst,
                                     स्थिर व्योम* spec, uchar* buf)
{
    वापस CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_32f, src, dst, (स्थिर IppsDFTSpec_R_32f*)spec, buf);
}

अटल IppStatus ippsDFTInv_PackToR( स्थिर द्विगुन* src, द्विगुन* dst,
                                     स्थिर व्योम* spec, uchar* buf)
{
    वापस CV_INSTRUMENT_FUN_IPP(ippsDFTInv_PackToR_64f, src, dst, (स्थिर IppsDFTSpec_R_64f*)spec, buf);
}
#पूर्ण_अगर

काष्ठा OcvDftOptions;

प्रकार व्योम (*DFTFunc)(स्थिर OcvDftOptions & c, स्थिर व्योम* src, व्योम* dst);

// Per-transform options of the 1D engine: the immutable plan (shared by all rows), the kernel
// table resolved for the current CPU, and the per-executor workspace (see dxt.hpp).
काष्ठा OcvDftOptions {
    द्विगुन scale;
    पूर्णांक n;

    bool isInverse;
    bool isComplex;     // DFT_COMPLEX_OUTPUT (forward) / complex input (inverse) for real transforms

    DFTFunc dft_func;
    bool useIpp;

#अगर_घोषित USE_IPP_DFT
    uchar* ipp_spec;
    uchar* ipp_work;
#पूर्ण_अगर

    स्थिर DftPlan* plan;
    स्थिर DFTKernels* kernels;
    uchar* workspace;

    OcvDftOptions()
    {
        plan = 0;
        kernels = 0;
        workspace = 0;
        scale = 0;
        n = 0;
        isInverse = false;
        isComplex = false;
        useIpp = false;
#अगर_घोषित USE_IPP_DFT
        ipp_spec = 0;
        ipp_work = 0;
#पूर्ण_अगर
        dft_func = 0;
    }
};


/****************************************************************************************\
                     New 1D engine: kernel table, plan builder, driver
\****************************************************************************************/

अटल DFTKernels getDFTKernelsDispatch(पूर्णांक depth)
{
    CV_CPU_DISPATCH(getDFTKernels_, (depth), CV_CPU_DISPATCH_MODES_ALL);
}

// The kernel tables are resolved once for the best available ISA; all further calls go
// through the function pointers with no dispatch overhead.
अटल स्थिर DFTKernels& getDFTKernels(पूर्णांक depth)
{
    अटल स्थिर DFTKernels k32 = getDFTKernelsDispatch(CV_32F);
    अटल स्थिर DFTKernels k64 = getDFTKernelsDispatch(CV_64F);
    वापस depth == CV_32F ? k32 : k64;
}

// exp(-2*pi*i*m/M) in double precision, m in [0, M): m = B*a + b -> hi[a]*lo[b] with B a power of
// two ~ sqrt(M). Both small tables are built by the rotation recurrence, re-seeded with exact
// sin/cos every 16 entries, so the error stays within a few ulps while only ~(B + M/B)/16 + 4
// trigonometric calls are made (4 for small M). Cheap to build, which matters because cv::dft()
// builds a fresh plan on every call.
काष्ठा DftTwiddleGen
{
    पूर्णांक M, logB, maskB;
    Auto_बफर<द्विगुन, 1024> buf;
    द्विगुन *hi_re, *hi_im, *lo_re, *lo_im;

    // re[i] + i*im[i] = w^i, i in [0, count), w = (wr, wi) = exp(i*stepAngle); the rotation
    // recurrence is re-seeded with exact sin/cos every 16 entries
    अटल व्योम fillTable(द्विगुन* re, द्विगुन* im, पूर्णांक गिनती_2, द्विगुन wr, द्विगुन wi, द्विगुन stepAngle)
    {
        क्रम (पूर्णांक i0 = 0; i0 < गिनती_2; i0 += 16)
        {
            द्विगुन t0 = stepAngle*i0;
            द्विगुन cr = i0 == 0 ? 1 : cos(t0), ci = i0 == 0 ? 0 : sin(t0);
            पूर्णांक i1 = std::min(i0 + 16, गिनती_2);
            क्रम (पूर्णांक i = i0; i < i1; i++)
            {
                re[i] = cr; im[i] = ci;
                द्विगुन t = cr*wr - ci*wi;
                ci = cr*wi + ci*wr;
                cr = t;
            }
        }
    }

    व्योम init(पूर्णांक _M)
    {
        M = _M;
        logB = 0;
        जबतक ((1LL << (2*logB)) < M) logB++;   // B = 2^logB >= sqrt(M)
        पूर्णांक B = 1 << logB;
        maskB = B - 1;
        पूर्णांक nhi = (M + B - 1)/B;
        buf.allocate(2*(B + nhi));
        hi_re = buf.तथ्य(); hi_im = hi_re + nhi;
        lo_re = hi_im + nhi; lo_im = lo_re + B;
        द्विगुन a = -2*CV_PI/M;
        // the two step angles are computed exactly (4 trigonometric calls in total for small M)
        fillTable(lo_re, lo_im, B, cos(a), sin(a), a);
        fillTable(hi_re, hi_im, nhi, cos(a*B), sin(a*B), a*B);
    }

    // exp(-2*pi*i*m/M) for any m >= 0 (the angle is periodic: only m mod M matters; callers may
    // ask for m == M, e.g. the rotation step of a chain that covers the whole period)
    व्योम लो(पूर्णांक m, द्विगुन& re, द्विगुन& im) स्थिर
    {
        CV_DbgAssert(m >= 0);
        अगर (m >= M)
            m %= M;
        पूर्णांक a = m >> logB, b = m & maskB;
        re = hi_re[a]*lo_re[b] - hi_im[a]*lo_im[b];
        im = hi_re[a]*lo_im[b] + hi_im[a]*lo_re[b];
    }
};

ढाँचा<typename T>
व्योम DftPlan::fillTables(माप_प्रकार ntw, bool need_rtw, bool need_dct)
{
    DftPlan& p = *यह;
    // all angles come from one base table exp(-2*pi*i*m/M), M a multiple of every angle grid used:
    // C2C: M = nc; real kinds with even n: M = n (=2nc) or 4n for DCT (W_4n^k); odd real: M = n = nc.
    पूर्णांक M = nc*(need_dct ? 8 : need_rtw ? 2 : 1);
    DftTwiddleGen gen;
    gen.init(M);
    p.tw.allocate((ntw*माप(T) + माप(द्विगुन) - 1)/माप(द्विगुन) + 1);
    T* cur = (T*)p.tw.तथ्य();
    पूर्णांक maxL = 1;
    क्रम (पूर्णांक s = 0; s < p.nstages; s++)
        अगर (p.stages[s].radix <= 8) maxL = std::max(maxL, p.stages[s].span);
    Auto_बफर<द्विगुन, 2048> twd_buf(2*maxL);     // leg-1 twiddles of the current stage in double
    द्विगुन* twd = twd_buf.तथ्य();
    क्रम (पूर्णांक s = 0; s < p.nstages; s++)
    {
        DftStage& st = p.stages[s];
        पूर्णांक r = st.radix, L = st.span, ts = st.tw_stride;
        T* twr = cur; T* twi = cur + (r - 1)*ts;
        cur = twi + (r - 1)*ts;
        पूर्णांक step = M/(r*L);
        अगर (r <= 8)
        {
            // Leg 1 (W_{rL}^j) by K independent interleaved rotation recurrences (a single chain is
            // latency-bound), each re-seeded from the exact generator every 16 of its steps; legs
            // q >= 2 as t_q(j) = t_{q-1}(j)*t_1(j), independent per entry (error <= q ulps).
            constexpr पूर्णांक K = 8;
            द्विगुन cr[K], ci[K], wr, wi;
            gen.लो(K*step, wr, wi);
            द्विगुन* t1r = twd; द्विगुन* t1i = twd + L;      // leg 1 kept in double for the products
            क्रम (पूर्णांक j0 = 0; j0 < L; j0 += 16*K)           // blocks of 16 steps per chain
            {
                क्रम (पूर्णांक k = 0; k < K; k++)
                    gen.लो(std::min(j0 + k, L - 1)*step, cr[k], ci[k]);
                पूर्णांक j1 = std::min(j0 + 16*K, L);
                क्रम (पूर्णांक j = j0; j < j1; j += K)
                    क्रम (पूर्णांक k = 0; k < K && j + k < L; k++)
                    {
                        t1r[j+k] = cr[k]; t1i[j+k] = ci[k];
                        द्विगुन t = cr[k]*wr - ci[k]*wi;
                        ci[k] = cr[k]*wi + ci[k]*wr;
                        cr[k] = t;
                    }
            }
            क्रम (पूर्णांक j = 0; j < L; j++)     // separate pass: vectorizes, the recurrence loop does not
            {
                twr[j] = (T)t1r[j]; twi[j] = (T)t1i[j];
            }
            क्रम (पूर्णांक q = 2; q < r; q++)
            {
                स्थिर T* pr = twr + (q-2)*ts; स्थिर T* pi = twi + (q-2)*ts;
                T* tr = twr + (q-1)*ts; T* ti = twi + (q-1)*ts;
                क्रम (पूर्णांक j = 0; j < L; j++)
                {
                    द्विगुन ar = pr[j], ai = pi[j], br = t1r[j], bi = t1i[j];
                    tr[j] = (T)(ar*br - ai*bi);
                    ti[j] = (T)(ar*bi + ai*br);
                }
            }
        }
        अन्यथा
        {
            // large odd radix: exact-ish generator lookup per entry
            क्रम (पूर्णांक q = 1; q < r; q++)
                क्रम (पूर्णांक j = 0; j < L; j++)
                {
                    द्विगुन re, im;
                    gen.लो(q*j*step, re, im);
                    twr[(q-1)*ts + j] = (T)re;
                    twi[(q-1)*ts + j] = (T)im;
                }
        }
        st.tw_re = twr; st.tw_im = twi;
        st.cs = st.sn = 0;
        अगर (r >= 7)
        {
            पूर्णांक h = (r - 1)/2;
            T* cs = cur; T* sn = cur + h*h;
            cur = sn + h*h;
            क्रम (पूर्णांक pp = 1; pp <= h; pp++)
                क्रम (पूर्णांक k = 1; k <= h; k++)
                {
                    द्विगुन t = 2*CV_PI*((द्विगुन)pp*k/r);
                    cs[(pp-1)*h + (k-1)] = (T)cos(t);
                    sn[(pp-1)*h + (k-1)] = (T)sin(t);
                }
            st.cs = cs; st.sn = sn;
        }
    }
    p.rtw_re = p.rtw_im = p.dct_re = p.dct_im = 0;
    अगर (need_rtw)
    {
        // W_n^k, k in [0, nc]
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        पूर्णांक step = M/n;
        क्रम (पूर्णांक k = 0; k <= nc; k++)
        {
            द्विगुन re, im;
            gen.लो(k*step, re, im);
            wr[k] = (T)re; wi[k] = (T)im;
        }
        p.rtw_re = wr; p.rtw_im = wi;
    }
    अगर (need_dct)
    {
        // s*W_4n^k, k in [0, nc]; s = 2*sqrt(1/(2n)) forward, sqrt(1/(2n)) inverse (the original normalization)
        T* wr = cur; T* wi = cur + (nc + 1);
        cur = wi + (nc + 1);
        द्विगुन scale = (p.kind == DFT_KIND_DCT ? 2 : 1)*std::वर्ग_मूल(1./(2*n));
        पूर्णांक step = M/(4*n);
        क्रम (पूर्णांक k = 0; k <= nc; k++)
        {
            द्विगुन re, im;
            gen.लो(k*step, re, im);
            wr[k] = (T)(re*scale); wi[k] = (T)(im*scale);
        }
        p.dct_re = wr; p.dct_im = wi;
    }
}

// See dxt.hpp for the contract (in particular where `vl` must come from).
व्योम DftPlan::build(पूर्णांक _kind, पूर्णांक _n, पूर्णांक _depth, पूर्णांक _vl)
{
    DftPlan& p = *यह;
    CV_Assert(_n >= 1 && _vl >= 1);
    CV_Assert(_depth == CV_32F || _depth == CV_64F);
    bool real_kind = _kind != DFT_KIND_C2C;
    bool dct_kind = _kind == DFT_KIND_DCT || _kind == DFT_KIND_IDCT;
    अगर (dct_kind)
        CV_Assert(_n % 2 == 0);   // cv::dct() supports even n only (n == 1 is handled by the caller)
    p.kind = _kind; p.n = _n; p.depth = _depth; p.vl = _vl;
    p.nc = real_kind && _n % 2 == 0 ? _n/2 : _n;
    CV_Assert(nc < (1 << 28));
    माप_प्रकार esz = depth == CV_32F ? माप(भग्न) : माप(द्विगुन);
    p.real_निवेश = kind == DFT_KIND_R2C && (n & 1) != 0;
    p.need_tmp = kind == DFT_KIND_C2R || kind == DFT_KIND_DCT || kind == DFT_KIND_IDCT;
    p.pingpong = false;

    // factorization: 2^k part first (fused first stage r0 + radix-4 stages), then 3s, 5s, other odd.
    // (A radix-8 middle stage was tried and measured slower on NEON: with per-leg twiddle tables it
    // does not save loads over two radix-4 stages and keeps 16 data vectors live.)
    पूर्णांक k2 = 0, rest = nc;
    जबतक ((rest & 1) == 0) { rest >>= 1; k2++; }
    पूर्णांक r0 = k2 == 0 ? 1 : k2 == 1 ? 2 : (k2 & 1) ? 8 : 4;
    p.first_radix = r0;
    पूर्णांक radices[DFT_MAX_STAGES], nst = 0;
    क्रम (पूर्णांक m = k2 - (r0 == 1 ? 0 : r0 == 2 ? 1 : r0 == 4 ? 2 : 3); m > 0; m -= 2) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 4; }
    rest = nc >> k2;
    जबतक (rest % 3 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 3; rest /= 3; }
    जबतक (rest % 5 == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = 5; rest /= 5; }
    क्रम (पूर्णांक f = 7; rest > 1; f += 2)
    {
        अगर (f*f > rest)
        {
            CV_Assert(nst < DFT_MAX_STAGES);
            radices[nst++] = rest;
            अवरोध;
        }
        जबतक (rest % f == 0) { CV_Assert(nst < DFT_MAX_STAGES); radices[nst++] = f; rest /= f; }
    }
    p.nstages = nst;

    // stages
    पूर्णांक L = r0, max_h = 0;
    माप_प्रकार ntw = 0;
    क्रम (पूर्णांक s = 0; s < nst; s++)
    {
        DftStage& st = p.stages[s];
        पूर्णांक r = radices[s];
        st.radix = r; st.span = L; st.ngroups = nc/(r*L); st.tw_stride = L;
        अगर (vl > 1 && L >= vl)
        {
            st.mode = DFT_STAGE_FULL;
            st.nsteps = st.ngroups*((L + vl - 1)/vl);
            अगर (L % vl != 0)
                p.pingpong = true;   // back-off clamp => overlapping stores => must be out-of-place
        }
        अन्यथा
        {
            st.mode = DFT_STAGE_SCALAR;
            st.nsteps = st.ngroups*L;
        }
        ntw += 2*(माप_प्रकार)(r - 1)*st.tw_stride;
        अगर (r >= 7)
        {
            पूर्णांक h = (r - 1)/2;
            ntw += 2*(माप_प्रकार)h*h;
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
    पूर्णांक seq[DFT_MAX_STAGES + 1], nseq = 0;
    अगर (r0 > 1) seq[nseq++] = r0;
    क्रम (पूर्णांक s = 0; s < nst; s++) seq[nseq++] = radices[s];
    पूर्णांक digits[DFT_MAX_STAGES + 1], weights[DFT_MAX_STAGES + 1];
    {
        पूर्णांक Ls = 1;
        क्रम (पूर्णांक s = 0; s < nseq; s++)
        {
            digits[s] = 0;
            weights[s] = nc/(Ls*seq[s]);
            Ls *= seq[s];
        }
    }
    अगर (r0 == 1)
    {
        p.itab.allocate(nc*2);
        पूर्णांक* it_all = p.itab.तथ्य();
        // the two lowest digits are enumerated by plain nested loops, the higher ones by a counter
        पूर्णांक n_in = std::min(nseq, 2);
        पूर्णांक r_0 = n_in > 0 ? seq[0] : 1, w_0 = n_in > 0 ? weights[0] : 0;
        पूर्णांक r_1 = n_in > 1 ? seq[1] : 1, w_1 = n_in > 1 ? weights[1] : 0;
        क्रम (पूर्णांक pos = 0, rev_hi = 0; pos < nc; )
        {
            क्रम (पूर्णांक d1 = 0; d1 < r_1; d1++)
            {
                पूर्णांक rev = rev_hi + d1*w_1;
                पूर्णांक* it = it_all + pos*2;
                अगर (p.real_निवेश)
                {
                    क्रम (पूर्णांक d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev; it[d0*2+1] = rev;
                    }
                }
                अन्यथा
                {
                    क्रम (पूर्णांक d0 = 0; d0 < r_0; d0++, rev += w_0)
                    {
                        it[d0*2] = rev*2; it[d0*2+1] = rev*2 + 1;
                    }
                }
                pos += r_0;
            }
            क्रम (पूर्णांक s = n_in; s < nseq; s++)
            {
                rev_hi += weights[s];
                अगर (++digits[s] < seq[s])
                    अवरोध;
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
        पूर्णांक M = nc/r0;
        p.itab.allocate(M);
        पूर्णांक* gtab = p.itab.तथ्य();
        पूर्णांक gw[DFT_MAX_STAGES + 1];
        {
            पूर्णांक Ls = 1;
            क्रम (पूर्णांक s = 0; s < nseq; s++)
            {
                gw[s] = Ls/r0;      // valid for s >= 1 (L_s is a multiple of r0)
                Ls *= seq[s];
            }
        }
        पूर्णांक sa = nseq - 1, sb = nseq - 2;                  // fastest and second fastest digits
        पूर्णांक r_a = sa >= 1 ? seq[sa] : 1, g_a = sa >= 1 ? gw[sa] : 0;
        पूर्णांक r_b = sb >= 1 ? seq[sb] : 1, g_b = sb >= 1 ? gw[sb] : 0;
        क्रम (पूर्णांक base = 0, g_hi = 0; base < M; )
        {
            क्रम (पूर्णांक db = 0; db < r_b; db++)
            {
                पूर्णांक g = (g_hi + db*g_b)*r0;
                क्रम (पूर्णांक da = 0; da < r_a; da++, g += g_a*r0)
                    gtab[base++] = g;
            }
            क्रम (पूर्णांक s = sb - 1; s >= 1; s--)
            {
                g_hi += gw[s];
                अगर (++digits[s] < seq[s])
                    अवरोध;
                digits[s] = 0;
                g_hi -= seq[s]*gw[s];
            }
        }
    }

    // twiddle tables
    bool need_rtw = real_kind && n % 2 == 0;
    अगर (need_rtw) ntw += 2*(माप_प्रकार)(nc + 1);
    अगर (dct_kind) ntw += 2*(माप_प्रकार)(nc + 1);
    अगर (depth == CV_32F)
        fillTables<भग्न>(ntw, need_rtw, dct_kind);
    अन्यथा
        fillTables<द्विगुन>(ntw, need_rtw, dct_kind);

    // workspace layout (from the 64-byte aligned base): pair0 (re, im), optional pair1 (re, im) or
    // the interleaved temp, radix-odd scratch (a[h], b[h] complex per lane).
    माप_प्रकार pair_bytes = 2*(माप_प्रकार)nc*esz;
    bool two_pairs = p.pingpong || p.need_tmp;
    p.pair1_ofs = two_pairs ? pair_bytes : 0;
    p.scratch_ofs = pair_bytes*(two_pairs ? 2 : 1);
    माप_प्रकार scratch_bytes = 4*(माप_प्रकार)max_h*std::max(vl, 1)*esz;
    p.ws_bytes = p.scratch_ofs + scratch_bytes + 64;
}

// Runs the transform described by c.plan: A (preproc) -> B stages (ping-pong) -> C (postproc).
// No allocations. sstep/dstep are in elements (used by DCT/IDCT only; 1 otherwise).
अटल व्योम runDft(स्थिर OcvDftOptions& c, स्थिर व्योम* src, माप_प्रकार sstep, व्योम* dst, माप_प्रकार dstep,
                   bool complex_io)
{
    स्थिर DftPlan& p = *c.plan;
    स्थिर DFTKernels& k = *c.kernels;
    CV_DbgAssert(p.vl == k.vlanes);
    माप_प्रकार esz = p.depth == CV_32F ? माप(भग्न) : माप(द्विगुन);
    uchar* base = alignPtr(c.workspace, 64);
    uchar* re0 = base;
    uchar* im0 = re0 + p.nc*esz;
    uchar* re1 = base + p.pair1_ofs;
    uchar* im1 = re1 + p.nc*esz;
    uchar* scratch = base + p.scratch_ofs;

    स्थिर व्योम* asrc = src;
    माप_प्रकार astep = sstep;
    bool conj = false;
    अगर (p.kind == DFT_KIND_C2R)
    {
        k.preprocCCS(p, src, re1, complex_io);
        asrc = re1; astep = 1; conj = true;
    }
    अन्यथा अगर (p.kind == DFT_KIND_IDCT)
    {
        k.preprocIDCT(p, src, sstep, re1);
        asrc = re1; astep = 1; conj = true;
    }
    अन्यथा अगर (p.kind == DFT_KIND_DCT)
    {
        k.preprocDCT(p, src, sstep, re1);
        asrc = re1; astep = 1;
    }
    अन्यथा अगर (p.kind == DFT_KIND_C2C)
        conj = c.isInverse;
    CV_DbgAssert(astep == 1 || p.first_radix == 1);

    DftPreprocFunc pre = p.first_radix == 1 ? k.preprocRadix0 : p.first_radix == 2 ? k.preprocRadix2 :
                         p.first_radix == 4 ? k.preprocRadix4 : k.preprocRadix8;
    pre(p, asrc, astep, re0, im0, conj);

    uchar *sre = re0, *sim = im0, *dre = re1, *dim = im1;
    क्रम (पूर्णांक i = 0; i < p.nstages; i++)
    {
        स्थिर DftStage& st = p.stages[i];
        DftStageFunc fn = st.radix == 4 ? k.radix4 : st.radix == 3 ? k.radix3 :
                          st.radix == 5 ? k.radix5 : k.radixOdd;
        अगर (p.pingpong)
        {
            fn(st, sre, sim, dre, dim, scratch);
            std::swap(sre, dre); std::swap(sim, dim);
        }
        अन्यथा
            fn(st, sre, sim, sre, sim, scratch);
    }

    द्विगुन scale = c.scale;
    चयन (p.kind)
    {
    हाल DFT_KIND_C2C:
        k.postprocDFT(p, sre, sim, dst, scale, c.isInverse ? -scale : scale);
        अवरोध;
    हाल DFT_KIND_R2C:
        k.postprocRealDFT(p, sre, sim, dst, scale, complex_io);
        अवरोध;
    हाल DFT_KIND_C2R:
        अगर (p.nc == p.n)
            k.postprocReal(p, sre, dst, scale);
        अन्यथा
            k.postprocDFT(p, sre, sim, dst, scale, -scale);
        अवरोध;
    हाल DFT_KIND_DCT:
        k.postprocDCT(p, sre, sim, dst, dstep);
        अवरोध;
    शेष:
        CV_Assert(p.kind == DFT_KIND_IDCT);
        k.postprocIDCT(p, sre, sim, dst, dstep);
    }
}

// mixed-radix complex discrete Fourier transform (forward or inverse)
ढाँचा<typename T> अटल व्योम
DFT(स्थिर OcvDftOptions & c, स्थिर Complex<T>* src, Complex<T>* dst)
{
    अगर( c.useIpp )
    {
#अगर_घोषित USE_IPP_DFT
        अगर( !c.isInverse )
        {
            अगर (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                वापस;
            }
        }
        अन्यथा
        {
            अगर (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CV_IMPL_ADD(CV_IMPL_IPP);
                वापस;
            }
        }
        setIppErrorStatus();
#पूर्ण_अगर
    }

    runDft(c, src, 1, dst, 1, false);
}


/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
ढाँचा<typename T> अटल व्योम
RealDFT(स्थिर OcvDftOptions & c, स्थिर T* src, T* dst)
{
    पूर्णांक complex_output = c.isComplex;

    अगर( c.useIpp )
    {
#अगर_घोषित USE_IPP_DFT
        पूर्णांक n = c.n;
        T* ipp_dst = dst + complex_output;
        अगर (ippsDFTFwd_RToPack( src, ipp_dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            अगर( complex_output )
            {
                ipp_dst[-1] = ipp_dst[0];
                ipp_dst[0] = 0;
                अगर( (n & 1) == 0 )
                    ipp_dst[n] = 0;
            }
            CV_IMPL_ADD(CV_IMPL_IPP);
            वापस;
        }
        setIppErrorStatus();
#पूर्ण_अगर
    }
    runDft(c, src, 1, dst, 1, complex_output != 0);
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
ढाँचा<typename T> अटल व्योम
CCSIDFT(स्थिर OcvDftOptions & c, स्थिर T* src, T* dst)
{
    पूर्णांक complex_निवेश = c.isComplex;

    अगर( complex_निवेश )
        CV_Assert( src != dst );
    अगर( c.useIpp )
    {
#अगर_घोषित USE_IPP_DFT
        // IPP expects the packed format; with complex input the packed spectrum is src+1 once
        // im(0) (always zero) is overwritten with re(0)
        स्थिर T* ipp_src = src;
        T save_s1 = 0;
        अगर( complex_निवेश )
        {
            save_s1 = src[1];
            ((T*)src)[1] = src[0];
            ipp_src = src + 1;
        }
        IppStatus status = ippsDFTInv_PackToR( ipp_src, dst, c.ipp_spec, c.ipp_work );
        अगर( complex_निवेश )
            ((T*)src)[1] = save_s1;
        अगर( status >= 0 )
        {
            CV_IMPL_ADD(CV_IMPL_IPP);
            वापस;
        }
        setIppErrorStatus();
#पूर्ण_अगर
    }
    runDft(c, src, 1, dst, 1, complex_निवेश != 0);
}

अटल व्योम
CopyColumn( स्थिर uchar* _src, माप_प्रकार src_step,
            uchar* _dst, माप_प्रकार dst_step,
            पूर्णांक len, माप_प्रकार elem_आकार_2 )
{
    पूर्णांक i, t0, t1;
    स्थिर पूर्णांक* src = (स्थिर पूर्णांक*)_src;
    पूर्णांक* dst = (पूर्णांक*)_dst;
    src_step /= माप(src[0]);
    dst_step /= माप(dst[0]);

    अगर( elem_आकार_2 == माप(पूर्णांक) )
    {
        क्रम( i = 0; i < len; i++, src += src_step, dst += dst_step )
            dst[0] = src[0];
    }
    अन्यथा अगर( elem_आकार_2 == माप(पूर्णांक)*2 )
    {
        क्रम( i = 0; i < len; i++, src += src_step, dst += dst_step )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    अन्यथा अगर( elem_आकार_2 == माप(पूर्णांक)*4 )
    {
        क्रम( i = 0; i < len; i++, src += src_step, dst += dst_step )
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
ढाँचा<typename T> अटल अंतरभूत व्योम
copyFromColumns_( स्थिर uchar* src, माप_प्रकार src_step, T* dst, पूर्णांक ncols, पूर्णांक len )
{
    क्रम( पूर्णांक i = 0; i < len; i++, src += src_step )
    {
        स्थिर T* s = (स्थिर T*)src;
        क्रम( पूर्णांक k = 0; k < ncols; k++ )
            dst[(माप_प्रकार)k*len + i] = s[k];
    }
}

अटल व्योम
CopyFromColumns( स्थिर uchar* src, माप_प्रकार src_step, uchar* dst, पूर्णांक ncols, पूर्णांक len, माप_प्रकार elem_आकार_2 )
{
    अगर( elem_आकार_2 == माप(पूर्णांक) )
        copyFromColumns_(src, src_step, (पूर्णांक*)dst, ncols, len);
    अन्यथा अगर( elem_आकार_2 == माप(int64) )
        copyFromColumns_(src, src_step, (int64*)dst, ncols, len);
    अन्यथा
    {
        CV_Assert( elem_आकार_2 == माप(int64)*2 );
        copyFromColumns_(src, src_step, (Vec<int64, 2>*)dst, ncols, len);
    }
}

// The inverse of CopyFromColumns: `ncols` contiguous column buffers -> `ncols` adjacent columns
// of `dst` (row step in bytes), written row by row.
ढाँचा<typename T> अटल अंतरभूत व्योम
copyToColumns_( स्थिर T* src, uchar* dst, माप_प्रकार dst_step, पूर्णांक ncols, पूर्णांक len )
{
    क्रम( पूर्णांक i = 0; i < len; i++, dst += dst_step )
    {
        T* d = (T*)dst;
        क्रम( पूर्णांक k = 0; k < ncols; k++ )
            d[k] = src[(माप_प्रकार)k*len + i];
    }
}

अटल व्योम
CopyToColumns( स्थिर uchar* src, uchar* dst, माप_प्रकार dst_step, पूर्णांक ncols, पूर्णांक len, माप_प्रकार elem_आकार_2 )
{
    अगर( elem_आकार_2 == माप(पूर्णांक) )
        copyToColumns_((स्थिर पूर्णांक*)src, dst, dst_step, ncols, len);
    अन्यथा अगर( elem_आकार_2 == माप(int64) )
        copyToColumns_((स्थिर int64*)src, dst, dst_step, ncols, len);
    अन्यथा
    {
        CV_Assert( elem_आकार_2 == माप(int64)*2 );
        copyToColumns_((स्थिर Vec<int64, 2>*)src, dst, dst_step, ncols, len);
    }
}

अटल व्योम
ExpandCCS( uchar* _ptr, पूर्णांक n, पूर्णांक elem_आकार_2 )
{
    पूर्णांक i;
    अगर( elem_आकार_2 == (पूर्णांक)माप(भग्न) )
    {
        भग्न* p = (भग्न*)_ptr;
        क्रम( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        अगर( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        क्रम( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
    अन्यथा
    {
        द्विगुन* p = (द्विगुन*)_ptr;
        क्रम( i = 1; i < (n+1)/2; i++ )
        {
            p[(n-i)*2] = p[i*2-1];
            p[(n-i)*2+1] = -p[i*2];
        }
        अगर( (n & 1) == 0 )
        {
            p[n] = p[n-1];
            p[n+1] = 0.f;
            n--;
        }
        क्रम( i = n-1; i > 0; i-- )
            p[i+1] = p[i];
        p[1] = 0.f;
    }
}

ढाँचा<typename T, व्योम (*fn)(स्थिर OcvDftOptions&, स्थिर T*, T*)>
अटल व्योम dftWrap(स्थिर OcvDftOptions & c, स्थिर व्योम* src, व्योम* dst)
{
    fn(c, (स्थिर T*)src, (T*)dst);
}

}

#अगर_घोषित USE_IPP_DFT
प्रकार IppStatus (CV_STDCALL* Ipp_DFTGet_आकार_Func)(पूर्णांक, पूर्णांक, IppHintAlgorithm, पूर्णांक*, पूर्णांक*, पूर्णांक*);
प्रकार IppStatus (CV_STDCALL* IppDFTInitFunc)(पूर्णांक, पूर्णांक, IppHintAlgorithm, व्योम*, uchar*);

ढाँचा<typename SpecType, IppStatus (CV_STDCALL *init_fn)(पूर्णांक, पूर्णांक, IppHintAlgorithm, SpecType*, Ipp8u*)>
अटल IppStatus CV_STDCALL ippDFTInitWrap(पूर्णांक n, पूर्णांक flags, IppHintAlgorithm hint, व्योम* spec, Ipp8u* initbuf)
{
    वापस init_fn(n, flags, hint, (SpecType*)spec, initbuf);
}
#पूर्ण_अगर

namespace cv
{
#अगर defined USE_IPP_DFT

प्रकार IppStatus (CV_STDCALL* ippiDFT_C_Func)(स्थिर Ipp32fc*, पूर्णांक, Ipp32fc*, पूर्णांक, स्थिर IppiDFTSpec_C_32fc*, Ipp8u*);
प्रकार IppStatus (CV_STDCALL* ippiDFT_R_Func)(स्थिर Ipp32f* , पूर्णांक, Ipp32f* , पूर्णांक, स्थिर IppiDFTSpec_R_32f* , Ipp8u*);

ढाँचा <typename Dft>
श्रेणी Dft_C_IPPLoop_Invoker : खुला ParallelLoopBody
{
खुला:

    Dft_C_IPPLoop_Invoker(स्थिर uchar * _src, माप_प्रकार _src_step, uchar * _dst, माप_प्रकार _dst_step, पूर्णांक _width,
                          स्थिर Dft& _ippidft, पूर्णांक _norm_flag, bool *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = true;
    }

    भव व्योम चालक()(स्थिर Range& range) स्थिर CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* p_बफर = 0;
        Ipp8u* pMemInit= 0;
        पूर्णांक आकार_बफर=0;
        पूर्णांक आकार_Spec=0;
        पूर्णांक आकार_Init=0;

        Ippi_आकार src_Roi_आकार = {width, 1};

        status = ippi_DFTGet_आकार_C_32fc(src_Roi_आकार, norm_flag, ippAlgHintNone, &आकार_Spec, &आकार_Init, &आकार_बफर );
        अगर ( status < 0 )
        {
            *ok = false;
            वापस;
        }

        IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( आकार_Spec );

        अगर ( आकार_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( आकार_Init );

        अगर ( आकार_बफर > 0 )
            p_बफर = (Ipp8u*)CV_IPP_MALLOC( आकार_बफर );

        status = ippiDFTInit_C_32fc( src_Roi_आकार, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        अगर ( आकार_Init > 0 )
            ippFree( pMemInit );

        अगर ( status < 0 )
        {
            ippFree( pDFTSpec );
            अगर ( आकार_बफर > 0 )
                ippFree( p_बफर );
            *ok = false;
            वापस;
        }

        क्रम( पूर्णांक i = range.start; i < range.end; ++i)
            अगर(!ippidft((Ipp32fc*)(src + src_step * i), src_step, (Ipp32fc*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)p_बफर))
            {
                *ok = false;
            }

        अगर ( आकार_बफर > 0 )
            ippFree( p_बफर );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

निजी:
    स्थिर uchar * src;
    माप_प्रकार src_step;
    uchar * dst;
    माप_प्रकार dst_step;
    पूर्णांक width;
    स्थिर Dft& ippidft;
    पूर्णांक norm_flag;
    bool *ok;

    स्थिर Dft_C_IPPLoop_Invoker& चालक= (स्थिर Dft_C_IPPLoop_Invoker&);
};

ढाँचा <typename Dft>
श्रेणी Dft_R_IPPLoop_Invoker : खुला ParallelLoopBody
{
खुला:

    Dft_R_IPPLoop_Invoker(स्थिर uchar * _src, माप_प्रकार _src_step, uchar * _dst, माप_प्रकार _dst_step, पूर्णांक _width,
                          स्थिर Dft& _ippidft, पूर्णांक _norm_flag, bool *_ok) :
        ParallelLoopBody(),
        src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width),
        ippidft(_ippidft), norm_flag(_norm_flag), ok(_ok)
    {
        *ok = true;
    }

    भव व्योम चालक()(स्थिर Range& range) स्थिर CV_OVERRIDE
    {
        IppStatus status;
        Ipp8u* p_बफर = 0;
        Ipp8u* pMemInit= 0;
        पूर्णांक आकार_बफर=0;
        पूर्णांक आकार_Spec=0;
        पूर्णांक आकार_Init=0;

        Ippi_आकार src_Roi_आकार = {width, 1};

        status = ippi_DFTGet_आकार_R_32f(src_Roi_आकार, norm_flag, ippAlgHintNone, &आकार_Spec, &आकार_Init, &आकार_बफर );
        अगर ( status < 0 )
        {
            *ok = false;
            वापस;
        }

        IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( आकार_Spec );

        अगर ( आकार_Init > 0 )
            pMemInit = (Ipp8u*)CV_IPP_MALLOC( आकार_Init );

        अगर ( आकार_बफर > 0 )
            p_बफर = (Ipp8u*)CV_IPP_MALLOC( आकार_बफर );

        status = ippiDFTInit_R_32f( src_Roi_आकार, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

        अगर ( आकार_Init > 0 )
            ippFree( pMemInit );

        अगर ( status < 0 )
        {
            ippFree( pDFTSpec );
            अगर ( आकार_बफर > 0 )
                ippFree( p_बफर );
            *ok = false;
            वापस;
        }

        क्रम( पूर्णांक i = range.start; i < range.end; ++i)
            अगर(!ippidft((भग्न*)(src + src_step * i), src_step, (भग्न*)(dst + dst_step * i), dst_step,
                        pDFTSpec, (Ipp8u*)p_बफर))
            {
                *ok = false;
            }

        अगर ( आकार_बफर > 0 )
            ippFree( p_बफर );

        ippFree( pDFTSpec );
        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
    }

निजी:
    स्थिर uchar * src;
    माप_प्रकार src_step;
    uchar * dst;
    माप_प्रकार dst_step;
    पूर्णांक width;
    स्थिर Dft& ippidft;
    पूर्णांक norm_flag;
    bool *ok;

    स्थिर Dft_R_IPPLoop_Invoker& चालक= (स्थिर Dft_R_IPPLoop_Invoker&);
};

ढाँचा <typename Dft>
bool Dft_C_IPPLoop(स्थिर uchar * src, माप_प्रकार src_step, uchar * dst, माप_प्रकार dst_step, पूर्णांक width, पूर्णांक height, स्थिर Dft& ippidft, पूर्णांक norm_flag)
{
    bool ok;
    parallel_for_(Range(0, height), Dft_C_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(द्विगुन)(1<<16) );
    वापस ok;
}

ढाँचा <typename Dft>
bool Dft_R_IPPLoop(स्थिर uchar * src, माप_प्रकार src_step, uchar * dst, माप_प्रकार dst_step, पूर्णांक width, पूर्णांक height, स्थिर Dft& ippidft, पूर्णांक norm_flag)
{
    bool ok;
    parallel_for_(Range(0, height), Dft_R_IPPLoop_Invoker<Dft>(src, src_step, dst, dst_step, width, ippidft, norm_flag, &ok), (width * height)/(द्विगुन)(1<<16) );
    वापस ok;
}

काष्ठा IPPDFT_C_Functor
{
    IPPDFT_C_Functor(ippiDFT_C_Func _func) : ippiDFT_CToC_32fc_C1R(_func){}

    bool चालक()(स्थिर Ipp32fc* src, माप_प्रकार srcStep, Ipp32fc* dst, माप_प्रकार dstStep, स्थिर IppiDFTSpec_C_32fc* pDFTSpec, Ipp8u* p_बफर) स्थिर
    {
        वापस ippiDFT_CToC_32fc_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_CToC_32fc_C1R, src, static_cast<पूर्णांक>(srcStep), dst, static_cast<पूर्णांक>(dstStep), pDFTSpec, p_बफर) >= 0 : false;
    }
निजी:
    ippiDFT_C_Func ippiDFT_CToC_32fc_C1R;
};

काष्ठा IPPDFT_R_Functor
{
    IPPDFT_R_Functor(ippiDFT_R_Func _func) : ippiDFT_PackToR_32f_C1R(_func){}

    bool चालक()(स्थिर Ipp32f* src, माप_प्रकार srcStep, Ipp32f* dst, माप_प्रकार dstStep, स्थिर IppiDFTSpec_R_32f* pDFTSpec, Ipp8u* p_बफर) स्थिर
    {
        वापस ippiDFT_PackToR_32f_C1R ? CV_INSTRUMENT_FUN_IPP(ippiDFT_PackToR_32f_C1R, src, static_cast<पूर्णांक>(srcStep), dst, static_cast<पूर्णांक>(dstStep), pDFTSpec, p_बफर) >= 0 : false;
    }
निजी:
    ippiDFT_R_Func ippiDFT_PackToR_32f_C1R;
};

अटल bool ippi_DFT_C_32F(स्थिर uchar * src, माप_प्रकार src_step, uchar * dst, माप_प्रकार dst_step, पूर्णांक width, पूर्णांक height, bool inv, पूर्णांक norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* p_बफर = 0;
    Ipp8u* pMemInit= 0;
    पूर्णांक आकार_बफर=0;
    पूर्णांक आकार_Spec=0;
    पूर्णांक आकार_Init=0;

    Ippi_आकार src_Roi_आकार = {width, height};

    status = ippi_DFTGet_आकार_C_32fc(src_Roi_आकार, norm_flag, ippAlgHintNone, &आकार_Spec, &आकार_Init, &आकार_बफर );
    अगर ( status < 0 )
        वापस false;

    IppiDFTSpec_C_32fc* pDFTSpec = (IppiDFTSpec_C_32fc*)CV_IPP_MALLOC( आकार_Spec );

    अगर ( आकार_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( आकार_Init );

    अगर ( आकार_बफर > 0 )
        p_बफर = (Ipp8u*)CV_IPP_MALLOC( आकार_बफर );

    status = ippiDFTInit_C_32fc( src_Roi_आकार, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    अगर ( आकार_Init > 0 )
        ippFree( pMemInit );

    अगर ( status < 0 )
    {
        ippFree( pDFTSpec );
        अगर ( आकार_बफर > 0 )
            ippFree( p_बफर );
        वापस false;
    }

    अगर (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<पूर्णांक>(src_step), (Ipp32fc*)dst, static_cast<पूर्णांक>(dst_step), pDFTSpec, p_बफर);
    अन्यथा
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_CToC_32fc_C1R, (Ipp32fc*)src, static_cast<पूर्णांक>(src_step), (Ipp32fc*)dst, static_cast<पूर्णांक>(dst_step), pDFTSpec, p_बफर);

    अगर ( आकार_बफर > 0 )
        ippFree( p_बफर );

    ippFree( pDFTSpec );

    अगर(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        वापस true;
    }
    वापस false;
}

अटल bool ippi_DFT_R_32F(स्थिर uchar * src, माप_प्रकार src_step, uchar * dst, माप_प्रकार dst_step, पूर्णांक width, पूर्णांक height, bool inv, पूर्णांक norm_flag)
{
    CV_INSTRUMENT_REGION_IPP();

    IppStatus status;
    Ipp8u* p_बफर = 0;
    Ipp8u* pMemInit= 0;
    पूर्णांक आकार_बफर=0;
    पूर्णांक आकार_Spec=0;
    पूर्णांक आकार_Init=0;

    Ippi_आकार src_Roi_आकार = {width, height};

    status = ippi_DFTGet_आकार_R_32f(src_Roi_आकार, norm_flag, ippAlgHintNone, &आकार_Spec, &आकार_Init, &आकार_बफर );
    अगर ( status < 0 )
        वापस false;

    IppiDFTSpec_R_32f* pDFTSpec = (IppiDFTSpec_R_32f*)CV_IPP_MALLOC( आकार_Spec );

    अगर ( आकार_Init > 0 )
        pMemInit = (Ipp8u*)CV_IPP_MALLOC( आकार_Init );

    अगर ( आकार_बफर > 0 )
        p_बफर = (Ipp8u*)CV_IPP_MALLOC( आकार_बफर );

    status = ippiDFTInit_R_32f( src_Roi_आकार, norm_flag, ippAlgHintNone, pDFTSpec, pMemInit );

    अगर ( आकार_Init > 0 )
        ippFree( pMemInit );

    अगर ( status < 0 )
    {
        ippFree( pDFTSpec );
        अगर ( आकार_बफर > 0 )
            ippFree( p_बफर );
        वापस false;
    }

    अगर (!inv)
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTFwd_RToPack_32f_C1R, (भग्न*)src, static_cast<पूर्णांक>(src_step), (भग्न*)dst, static_cast<पूर्णांक>(dst_step), pDFTSpec, p_बफर);
    अन्यथा
        status = CV_INSTRUMENT_FUN_IPP(ippiDFTInv_PackToR_32f_C1R, (भग्न*)src, static_cast<पूर्णांक>(src_step), (भग्न*)dst, static_cast<पूर्णांक>(dst_step), pDFTSpec, p_बफर);

    अगर ( आकार_बफर > 0 )
        ippFree( p_बफर );

    ippFree( pDFTSpec );

    अगर(status >= 0)
    {
        CV_IMPL_ADD(CV_IMPL_IPP);
        वापस true;
    }
    वापस false;
}

#पूर्ण_अगर
}

#अगर_घोषित HAVE_OPENCL

namespace cv
{

क्रमागत FftType
{
    R2R = 0, // real to CCS in case forward transform, CCS to real otherwise
    C2R = 1, // complex to real in case inverse transform
    R2C = 2, // real to complex in case forward transform
    C2C = 3  // complex to complex
};

काष्ठा OCL_FftPlan
{
निजी:
    UMat twiddles;
    String buildOptions;
    पूर्णांक thread_गिनती;
    पूर्णांक dft_आकार;
    पूर्णांक dft_depth;
    bool status;

खुला:
    OCL_FftPlan(पूर्णांक आकार_4, पूर्णांक _depth) : dft_आकार(आकार_4), dft_depth(_depth), status(true)
    {
        CV_Assert( dft_depth == CV_32F || dft_depth == CV_64F );

        पूर्णांक min_radix;
        std::vector<पूर्णांक> radixes, blocks;
        ocl_getRadixes(dft_आकार, radixes, blocks, min_radix);
        thread_गिनती = dft_आकार / min_radix;

        अगर (thread_गिनती > (पूर्णांक) ocl::Device::getDefault().max_Work_Group_आकार())
        {
            status = false;
            वापस;
        }

        // generate string with radix calls
        String radix_processing;
        पूर्णांक n = 1, twiddle_आकार = 0;
        क्रम (माप_प्रकार i=0; i<radixes.size(); i++)
        {
            पूर्णांक radix = radixes[i], block = blocks[i];
            अगर (block > 1)
                radix_processing += format("fft_radix%d_B%d(smem,twiddles+%d,ind,%d,%d);", radix, block, twiddle_आकार, n, dft_आकार/radix);
            अन्यथा
                radix_processing += format("fft_radix%d(smem,twiddles+%d,ind,%d,%d);", radix, twiddle_आकार, n, dft_आकार/radix);
            twiddle_आकार += (radix-1)*n;
            n *= radix;
        }

        twiddles.create(1, twiddle_आकार, CV_MAKE_TYPE(dft_depth, 2));
        अगर (dft_depth == CV_32F)
            fillRadixTable<भग्न>(twiddles, radixes);
        अन्यथा
            fillRadixTable<द्विगुन>(twiddles, radixes);

        buildOptions = format("-D LOCAL_SIZE=%d -D kercn=%d -D FT=%s -D CT=%s%s -D RADIX_PROCESS=%s",
                              dft_आकार, min_radix, ocl::typeToStr(dft_depth), ocl::typeToStr(CV_MAKE_TYPE(dft_depth, 2)),
                              dft_depth == CV_64F ? " -D DOUBLE_SUPPORT" : "", radix_processing.c_str());
    }

    bool enqueueTransform(निवेश_Array _src, OutputArray _dst, पूर्णांक अंक_dfts, पूर्णांक flags, पूर्णांक fftType, bool rows = true) स्थिर
    {
        अगर (!status)
            वापस false;

        UMat src = _src.getUMat();
        UMat dst = _dst.getUMat();

        माप_प्रकार globalsize[2];
        माप_प्रकार localsize[2];
        String kernel_name;

        bool is1d = (flags & DFT_ROWS) != 0 || अंक_dfts == 1;
        bool inv = (flags & DFT_INVERSE) != 0;
        String options = buildOptions;

        अगर (rows)
        {
            globalsize[0] = thread_गिनती; globalsize[1] = src.rows;
            localsize[0] = thread_गिनती; localsize[1] = 1;
            kernel_name = !inv ? "fft_multi_radix_rows" : "ifft_multi_radix_rows";
            अगर ((is1d || inv) && (flags & DFT_SCALE))
                options += " -D DFT_SCALE";
        }
        अन्यथा
        {
            globalsize[0] = अंक_dfts; globalsize[1] = thread_गिनती;
            localsize[0] = 1; localsize[1] = thread_गिनती;
            kernel_name = !inv ? "fft_multi_radix_cols" : "ifft_multi_radix_cols";
            अगर (flags & DFT_SCALE)
                options += " -D DFT_SCALE";
        }

        options += src.channels() == 1 ? " -D REAL_INPUT" : " -D COMPLEX_INPUT";
        options += dst.channels() == 1 ? " -D REAL_OUTPUT" : " -D COMPLEX_OUTPUT";
        options += is1d ? " -D IS_1D" : "";

        अगर (!inv)
        {
            अगर ((is1d && src.channels() == 1) || (rows && (fftType == R2R)))
                options += " -D NO_CONJUGATE";
        }
        अन्यथा
        {
            अगर (rows && (fftType == C2R || fftType == R2R))
                options += " -D NO_CONJUGATE";
            अगर (dst.cols % 2 == 0)
                options += " -D EVEN";
        }

        ocl::Kernel k(kernel_name.c_str(), ocl::core::fft_oclsrc, options);
        अगर (k.empty())
            वापस false;

        k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::Read_Only_No_आकार(twiddles), thread_गिनती, अंक_dfts);
        वापस k.run(2, globalsize, localsize, false);
    }

निजी:
    अटल व्योम ocl_getRadixes(पूर्णांक cols, std::vector<पूर्णांक>& radixes, std::vector<पूर्णांक>& blocks, पूर्णांक& min_radix)
    {
        पूर्णांक factors[34];
        पूर्णांक nf = DFTFactorize(cols, factors);

        पूर्णांक n = 1;
        पूर्णांक factor_सूचकांक = 0;
        min_radix = पूर्णांक_उच्च;

        // 2^n transforms
        अगर ((factors[factor_सूचकांक] & 1) == 0)
        {
            क्रम( ; n < factors[factor_सूचकांक];)
            {
                पूर्णांक radix = 2, block = 1;
                अगर (8*n <= factors[0])
                    radix = 8;
                अन्यथा अगर (4*n <= factors[0])
                {
                    radix = 4;
                    अगर (cols % 12 == 0)
                        block = 3;
                    अन्यथा अगर (cols % 8 == 0)
                        block = 2;
                }
                अन्यथा
                {
                    अगर (cols % 10 == 0)
                        block = 5;
                    अन्यथा अगर (cols % 8 == 0)
                        block = 4;
                    अन्यथा अगर (cols % 6 == 0)
                        block = 3;
                    अन्यथा अगर (cols % 4 == 0)
                        block = 2;
                }

                radixes.push_back(radix);
                blocks.push_back(block);
                min_radix = min(min_radix, block*radix);
                n *= radix;
            }
            factor_सूचकांक++;
        }

        // all the other transforms
        क्रम( ; factor_सूचकांक < nf; factor_सूचकांक++)
        {
            पूर्णांक radix = factors[factor_सूचकांक], block = 1;
            अगर (radix == 3)
            {
                अगर (cols % 12 == 0)
                    block = 4;
                अन्यथा अगर (cols % 9 == 0)
                    block = 3;
                अन्यथा अगर (cols % 6 == 0)
                    block = 2;
            }
            अन्यथा अगर (radix == 5)
            {
                अगर (cols % 10 == 0)
                    block = 2;
            }
            radixes.push_back(radix);
            blocks.push_back(block);
            min_radix = min(min_radix, block*radix);
        }
    }

    ढाँचा <typename T>
    अटल व्योम fillRadixTable(UMat twiddles, स्थिर std::vector<पूर्णांक>& radixes)
    {
        Mat tw = twiddles.getMat(ACCESS_WRITE);
        T* ptr = tw.ptr<T>();
        पूर्णांक ptr_सूचकांक = 0;

        पूर्णांक n = 1;
        क्रम (माप_प्रकार i=0; i<radixes.size(); i++)
        {
            पूर्णांक radix = radixes[i];
            n *= radix;

            क्रम (पूर्णांक j=1; j<radix; j++)
            {
                द्विगुन theta = -CV_2PI*j/n;

                क्रम (पूर्णांक k=0; k<(n/radix); k++)
                {
                    ptr[ptr_सूचकांक++] = (T) cos(k*theta);
                    ptr[ptr_सूचकांक++] = (T) sin(k*theta);
                }
            }
        }
    }
};

श्रेणी OCL_FftPlanCache
{
खुला:
    अटल OCL_FftPlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OCL_FftPlanCache, नया OCL_FftPlanCache())
    }

    Ptr<OCL_FftPlan> getFftPlan(पूर्णांक dft_आकार, पूर्णांक depth)
    {
        पूर्णांक कुंजी = (dft_आकार << 16) | (depth & 0xFFFF);
        std::map<पूर्णांक, Ptr<OCL_FftPlan> >::iterator f = planStorage.find(कुंजी);
        अगर (f != planStorage.end())
        {
            वापस f->second;
        }
        अन्यथा
        {
            Ptr<OCL_FftPlan> newPlan = Ptr<OCL_FftPlan>(नया OCL_FftPlan(dft_आकार, depth));
            planStorage[कुंजी] = newPlan;
            वापस newPlan;
        }
    }

    ~OCL_FftPlanCache()
    {
        planStorage.सफा();
    }

रक्षित:
    OCL_FftPlanCache() :
        planStorage()
    {
    }
    std::map<पूर्णांक, Ptr<OCL_FftPlan> > planStorage;
};

अटल bool ocl_dft_rows(निवेश_Array _src, OutputArray _dst, पूर्णांक nonzero_rows, पूर्णांक flags, पूर्णांक fftType)
{
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.cols(), depth);
    वापस plan->enqueueTransform(_src, _dst, nonzero_rows, flags, fftType, true);
}

अटल bool ocl_dft_cols(निवेश_Array _src, OutputArray _dst, पूर्णांक nonzero_cols, पूर्णांक flags, पूर्णांक fftType)
{
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type);
    Ptr<OCL_FftPlan> plan = OCL_FftPlanCache::getInstance().getFftPlan(_src.rows(), depth);
    वापस plan->enqueueTransform(_src, _dst, nonzero_cols, flags, fftType, false);
}

अंतरभूत FftType determineFFTType(bool real_निवेश, bool complex_निवेश, bool real_output, bool complex_output, bool inv)
{
    // output format is not specified
    अगर (!real_output && !complex_output)
        complex_output = true;

    // input or output format is ambiguous
    अगर (real_निवेश == complex_निवेश || real_output == complex_output)
        CV_Error(Error::StsBadArg, "Invalid FFT input or output format");

    FftType परिणाम = real_निवेश ? (real_output ? R2R : R2C) : (real_output ? C2R : C2C);

    // Forward Complex to CCS not supported
    अगर (परिणाम == C2R && !inv)
        परिणाम = C2C;

    // Inverse CCS to Complex not supported
    अगर (परिणाम == R2C && inv)
        परिणाम = R2R;

    वापस परिणाम;
}

अटल bool ocl_dft(निवेश_Array _src, OutputArray _dst, पूर्णांक flags, पूर्णांक nonzero_rows)
{
    पूर्णांक type = _src.type(), cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    आकार ssize = _src.size();
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    अगर (!(cn == 1 || cn == 2)
        || !(depth == CV_32F || (depth == CV_64F && doubleSupport))
        || ((flags & DFT_REAL_OUTPUT) && (flags & DFT_COMPLEX_OUTPUT)))
        वापस false;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    अगर (ssize.area() != getOptimalDFTSize(ssize.area()))
        वापस false;

    UMat src = _src.getUMat();
    bool inv = (flags & DFT_INVERSE) != 0 ? 1 : 0;

    अगर( nonzero_rows <= 0 || nonzero_rows > _src.rows() )
        nonzero_rows = _src.rows();
    bool is1d = (flags & DFT_ROWS) != 0 || nonzero_rows == 1;

    FftType fftType = determineFFTType(cn == 1, cn == 2,
        (flags & DFT_REAL_OUTPUT) != 0, (flags & DFT_COMPLEX_OUTPUT) != 0, inv);

    UMat output;
    अगर (fftType == C2C || fftType == R2C)
    {
        // complex output
        _dst.create_Same_आकार(src, CV_MAKETYPE(depth, 2));
        output = _dst.getUMat();
    }
    अन्यथा
    {
        // real output
        अगर (is1d)
        {
            _dst.create_Same_आकार(src, CV_MAKETYPE(depth, 1));
            output = _dst.getUMat();
        }
        अन्यथा
        {
            _dst.create_Same_आकार(src, CV_MAKETYPE(depth, 1));
            output.create(src.size, CV_MAKETYPE(depth, 2));
        }
    }

    bool परिणाम = false;
    अगर (!inv)
    {
        पूर्णांक nonzero_cols = fftType == R2R ? output.cols/2 + 1 : output.cols;
        परिणाम = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
        अगर (!is1d)
            परिणाम = परिणाम && ocl_dft_cols(output, _dst, nonzero_cols, flags, fftType);
    }
    अन्यथा
    {
        अगर (fftType == C2C)
        {
            // complex output
            परिणाम = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            अगर (!is1d)
                परिणाम = परिणाम && ocl_dft_cols(output, output, output.cols, flags, fftType);
        }
        अन्यथा
        {
            अगर (is1d)
            {
                परिणाम = ocl_dft_rows(src, output, nonzero_rows, flags, fftType);
            }
            अन्यथा
            {
                पूर्णांक nonzero_cols = src.cols/2 + 1;
                परिणाम = ocl_dft_cols(src, output, nonzero_cols, flags, fftType);
                परिणाम = परिणाम && ocl_dft_rows(output, _dst, nonzero_rows, flags, fftType);
            }
        }
    }
    वापस परिणाम;
}

} // namespace cv;

#पूर्ण_अगर

#अगर_घोषित HAVE_CLAMDFFT

namespace cv {

#घोषणा CLAMDDFT_Assert(func) \
    { \
        clfftStatus s = (func); \
        CV_Assert(s == CLFFT_SUCCESS); \
    }

श्रेणी PlanCache
{
    काष्ठा FftPlan
    {
        FftPlan(स्थिर आकार & dft_आकार_2, पूर्णांक _src_step, पूर्णांक _dst_step, bool _doubleFP, bool _inplace, पूर्णांक _flags, FftType _fftType) :
            dft_आकार(dft_आकार_2), src_step(_src_step), dst_step(_dst_step),
            doubleFP(_doubleFP), inplace(_inplace), flags(_flags), fftType(_fftType),
            context((cl_context)ocl::Context::getDefault().ptr()), plHandle(0)
        {
            bool dft_inverse = (flags & DFT_INVERSE) != 0;
            bool dft_scale = (flags & DFT_SCALE) != 0;
            bool dft_rows = (flags & DFT_ROWS) != 0;

            clfftLayout inLayout = CLFFT_REAL, outLayout = CLFFT_REAL;
            clfftDim dim = dft_आकार.height == 1 || dft_rows ? CLFFT_1D : CLFFT_2D;

            माप_प्रकार batch_आकार = dft_rows ? dft_आकार.height : 1;
            माप_प्रकार clLengthsIn[3] = { (माप_प्रकार)dft_आकार.width, dft_rows ? 1 : (माप_प्रकार)dft_आकार.height, 1 };
            माप_प्रकार clStridesIn[3] = { 1, 1, 1 };
            माप_प्रकार clStridesOut[3]  = { 1, 1, 1 };
            पूर्णांक elem_आकार = doubleFP ? माप(द्विगुन) : माप(भग्न);

            चयन (fftType)
            {
            हाल C2C:
                inLayout = CLFFT_COMPLEX_INTERLEAVED;
                outLayout = CLFFT_COMPLEX_INTERLEAVED;
                clStridesIn[1] = src_step / (elem_आकार << 1);
                clStridesOut[1] = dst_step / (elem_आकार << 1);
                अवरोध;
            हाल R2C:
                inLayout = CLFFT_REAL;
                outLayout = CLFFT_HERMITIAN_INTERLEAVED;
                clStridesIn[1] = src_step / elem_आकार;
                clStridesOut[1] = dst_step / (elem_आकार << 1);
                अवरोध;
            हाल C2R:
                inLayout = CLFFT_HERMITIAN_INTERLEAVED;
                outLayout = CLFFT_REAL;
                clStridesIn[1] = src_step / (elem_आकार << 1);
                clStridesOut[1] = dst_step / elem_आकार;
                अवरोध;
            हाल R2R:
            शेष:
                CV_Error(Error::StsNotImplemented, "AMD Fft does not support this type");
                अवरोध;
            }

            clStridesIn[2] = dft_rows ? clStridesIn[1] : dft_आकार.width * clStridesIn[1];
            clStridesOut[2] = dft_rows ? clStridesOut[1] : dft_आकार.width * clStridesOut[1];

            CLAMDDFT_Assert(clfftCreateDefaultPlan(&plHandle, (cl_context)ocl::Context::getDefault().ptr(), dim, clLengthsIn))

            // setting plan properties
            CLAMDDFT_Assert(clfftSetPlanPrecision(plHandle, doubleFP ? CLFFT_DOUBLE : CLFFT_SINGLE));
            CLAMDDFT_Assert(clfft_Set_परिणाम_Location(plHandle, inplace ? CLFFT_INPLACE : CLFFT_OUTOFPLACE))
            CLAMDDFT_Assert(clfftSetLayout(plHandle, inLayout, outLayout))
            CLAMDDFT_Assert(clfft_Set_Plan_Batch_आकार(plHandle, batch_आकार))
            CLAMDDFT_Assert(clfftSetPlanInStride(plHandle, dim, clStridesIn))
            CLAMDDFT_Assert(clfftSetPlanOutStride(plHandle, dim, clStridesOut))
            CLAMDDFT_Assert(clfftSetPlanDistance(plHandle, clStridesIn[dim], clStridesOut[dim]))

            भग्न scale = dft_scale ? 1.0f / (dft_rows ? dft_आकार.width : dft_आकार.area()) : 1.0f;
            CLAMDDFT_Assert(clfftSetPlanScale(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD, scale))

            // ready to bake
            cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
            CLAMDDFT_Assert(clfftBakePlan(plHandle, 1, &queue, शून्य, शून्य))
        }

        ~FftPlan()
        {
            // Do not tear down clFFT.
            // The user application may still use clFFT even after OpenCV is unloaded.
            /*clfftDestroyPlan(&plHandle);*/
        }

        मित्र श्रेणी PlanCache;

    निजी:
        आकार dft_आकार;
        पूर्णांक src_step, dst_step;
        bool doubleFP;
        bool inplace;
        पूर्णांक flags;
        FftType fftType;

        cl_context context;
        clfftPlanHandle plHandle;
    };

खुला:
    अटल PlanCache & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(PlanCache, नया PlanCache())
    }

    clfftPlanHandle getPlanHandle(स्थिर आकार & dft_आकार, पूर्णांक src_step, पूर्णांक dst_step, bool doubleFP,
                                  bool inplace, पूर्णांक flags, FftType fftType)
    {
        cl_context currentContext = (cl_context)ocl::Context::getDefault().ptr();

        क्रम (माप_प्रकार i = 0, size = planStorage.size(); i < size; ++i)
        {
            स्थिर FftPlan * स्थिर plan = planStorage[i];

            अगर (plan->dft_आकार == dft_आकार &&
                plan->flags == flags &&
                plan->src_step == src_step &&
                plan->dst_step == dst_step &&
                plan->doubleFP == doubleFP &&
                plan->fftType == fftType &&
                plan->inplace == inplace)
            {
                अगर (plan->context != currentContext)
                {
                    planStorage.erase(planStorage.begin() + i);
                    अवरोध;
                }

                वापस plan->plHandle;
            }
        }

        // no baked plan is found, so let's create a new one
        Ptr<FftPlan> newPlan = Ptr<FftPlan>(नया FftPlan(dft_आकार, src_step, dst_step, doubleFP, inplace, flags, fftType));
        planStorage.push_back(newPlan);

        वापस newPlan->plHandle;
    }

    ~PlanCache()
    {
        planStorage.सफा();
    }

रक्षित:
    PlanCache() :
        planStorage()
    {
    }

    std::vector<Ptr<FftPlan> > planStorage;
};

बाह्य "C" {

अटल व्योम CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, व्योम *p)
{
    UMat_तथ्य * u = (UMat_तथ्य *)p;

    अगर( u && CV_XADD(&u->urefcount, -1) == 1 )
        u->currAllocator->deallocate(u);
    u = 0;

    clReleaseEvent(e), e = 0;
}

}

अटल bool ocl_dft_amdfft(निवेश_Array _src, OutputArray _dst, पूर्णांक flags)
{
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    आकार ssize = _src.size();

    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    अगर ( (!doubleSupport && depth == CV_64F) ||
         !(type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2) ||
         _src.offset() != 0)
        वापस false;

    // if is not a multiplication of prime numbers { 2, 3, 5 }
    अगर (ssize.area() != getOptimalDFTSize(ssize.area()))
        वापस false;

    पूर्णांक dst_complex_निवेश = cn == 2 ? 1 : 0;
    bool dft_inverse = (flags & DFT_INVERSE) != 0 ? 1 : 0;
    पूर्णांक dft_complex_output = (flags & DFT_COMPLEX_OUTPUT) != 0;
    bool dft_real_output = (flags & DFT_REAL_OUTPUT) != 0;

    CV_Assert(dft_complex_output + dft_real_output < 2);
    FftType fftType = (FftType)(dst_complex_निवेश << 0 | dft_complex_output << 1);

    चयन (fftType)
    {
    हाल C2C:
        _dst.create(ssize.height, ssize.width, CV_MAKE_TYPE(depth, 2));
        अवरोध;
    हाल R2C: // TODO implement it if possible
    हाल C2R: // TODO implement it if possible
    हाल R2R: // AMD Fft does not support this type
    शेष:
        वापस false;
    }

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    अगर (!ocl::internal::isCLBuffer(src) || !ocl::internal::isCLBuffer(dst))
        वापस false;
    bool inplace = src.u == dst.u;

    clfftPlanHandle plHandle = PlanCache::getInstance().
            getPlanHandle(ssize, (पूर्णांक)src.step, (पूर्णांक)dst.step,
                          depth == CV_64F, inplace, flags, fftType);

    // get the bufferSize
    माप_प्रकार बफर_आकार = 0;
    CLAMDDFT_Assert(clfft_Get_Tmp_Buf_आकार(plHandle, &बफर_आकार))
    UMat tmp_बफर(1, (पूर्णांक)बफर_आकार, CV_8UC1);

    cl_mem srcarg = (cl_mem)src.handle(ACCESS_READ);
    cl_mem dstarg = (cl_mem)dst.handle(ACCESS_RW);

    cl_command_queue queue = (cl_command_queue)ocl::Queue::getDefault().ptr();
    cl_event e = 0;

    CLAMDDFT_Assert(clfftEnqueueTransform(plHandle, dft_inverse ? CLFFT_BACKWARD : CLFFT_FORWARD,
                                          1, &queue, 0, शून्य, &e,
                                          &srcarg, &dstarg, (cl_mem)tmp_बफर.handle(ACCESS_RW)))

    tmp_बफर.addref();
    clSetEventCallback(e, CL_COMPLETE, oclCleanupCallback, tmp_बफर.u);
    वापस true;
}

#अघोषित DFT_ASSERT

}

#पूर्ण_अगर // HAVE_CLAMDFFT

namespace cv
{

ढाँचा <typename T>
अटल व्योम complementComplex(T * ptr, माप_प्रकार step, पूर्णांक n, पूर्णांक len, पूर्णांक dft_dims)
{
    T* p0 = (T*)ptr;
    माप_प्रकार dstep = step/माप(p0[0]);
    क्रम(पूर्णांक i = 0; i < len; i++ )
    {
        T* p = p0 + dstep*i;
        T* q = dft_dims == 1 || i == 0 || i*2 == len ? p : p0 + dstep*(len-i);

        क्रम( पूर्णांक j = 1; j < (n+1)/2; j++ )
        {
            p[(n-j)*2] = q[j*2];
            p[(n-j)*2+1] = -q[j*2+1];
        }
    }
}

अटल व्योम complementComplexOutput(पूर्णांक depth, uchar * ptr, माप_प्रकार step, पूर्णांक गिनती_2, पूर्णांक len, पूर्णांक dft_dims)
{
    अगर( depth == CV_32F )
        complementComplex((भग्न*)ptr, step, गिनती_2, len, dft_dims);
    अन्यथा
        complementComplex((द्विगुन*)ptr, step, गिनती_2, len, dft_dims);
}

क्रमागत DftMode {
    InvalidDft = 0,
    FwdRealToCCS,
    FwdRealToComplex,
    FwdComplexToComplex,
    InvCCSToReal,
    InvComplexToReal,
    InvComplexToComplex,
};

क्रमागत DftDims {
    InvalidDim = 0,
    OneDim,
    OneDimColWise,
    TwoDims
};

अंतरभूत स्थिर अक्षर * modeName(DftMode m)
{
    चयन (m)
    {
    हाल InvalidDft: वापस "InvalidDft";
    हाल FwdRealToCCS: वापस "FwdRealToCCS";
    हाल FwdRealToComplex: वापस "FwdRealToComplex";
    हाल FwdComplexToComplex: वापस "FwdComplexToComplex";
    हाल InvCCSToReal: वापस "InvCCSToReal";
    हाल InvComplexToReal: वापस "InvComplexToReal";
    हाल InvComplexToComplex: वापस "InvComplexToComplex";
    }
    वापस 0;
}

अंतरभूत स्थिर अक्षर * dimsName(DftDims d)
{
    चयन (d)
    {
    हाल InvalidDim: वापस "InvalidDim";
    हाल OneDim: वापस "OneDim";
    हाल OneDimColWise: वापस "OneDimColWise";
    हाल TwoDims: वापस "TwoDims";
    };
    वापस 0;
}

ढाँचा <typename T>
अंतरभूत bool isInv(T mode)
{
    चयन ((DftMode)mode)
    {
        हाल InvCCSToReal:
        हाल InvComplexToReal:
        हाल InvComplexToComplex: वापस true;
        शेष: वापस false;
    }
}

अंतरभूत DftMode determineMode(bool inv, पूर्णांक cn1, पूर्णांक cn2)
{
    अगर (!inv)
    {
        अगर (cn1 == 1 && cn2 == 1)
            वापस FwdRealToCCS;
        अन्यथा अगर (cn1 == 1 && cn2 == 2)
            वापस FwdRealToComplex;
        अन्यथा अगर (cn1 == 2 && cn2 == 2)
            वापस FwdComplexToComplex;
    }
    अन्यथा
    {
        अगर (cn1 == 1 && cn2 == 1)
            वापस InvCCSToReal;
        अन्यथा अगर (cn1 == 2 && cn2 == 1)
            वापस InvComplexToReal;
        अन्यथा अगर (cn1 == 2 && cn2 == 2)
            वापस InvComplexToComplex;
    }
    वापस InvalidDft;
}


अंतरभूत DftDims determineDims(पूर्णांक rows, पूर्णांक cols, bool isRowWise, bool isContinuous)
{
    // printf("%d x %d (%d, %d)\n", rows, cols, isRowWise, isContinuous);
    अगर (isRowWise)
        वापस OneDim;
    अगर (cols == 1 && rows > 1) // one-column-shaped input
    {
        अगर (isContinuous)
            वापस OneDim;
        अन्यथा
            वापस OneDimColWise;
    }
    अगर (rows == 1)
        वापस OneDim;
    अगर (cols > 1 && rows > 1)
        वापस TwoDims;
    वापस InvalidDim;
}

श्रेणी OcvDftBasicImpl CV_FINAL : खुला hal::DFT1D
{
खुला:
    OcvDftOptions opt;
    // the plan is built once in init(), the workspace is allocated once here and reused by
    // every apply() (zero allocations per transform)
    DftPlan plan;
    Auto_बफर<uchar> खाली;
#अगर_घोषित USE_IPP_DFT
    Auto_बफर<uchar> ippbuf;
    Auto_बफर<uchar> ippworkbuf;
    माप_प्रकार ipp_worksize;
#पूर्ण_अगर

खुला:
    OcvDftBasicImpl()
    {
#अगर_घोषित USE_IPP_DFT
        ipp_worksize = 0;
#पूर्ण_अगर
    }
    व्योम init(पूर्णांक len, पूर्णांक गिनती_2, पूर्णांक depth, पूर्णांक flags, bool *need_बफर)
    {
        पूर्णांक stage = (flags & CV_HAL_DFT_STAGE_COLS) != 0 ? 1 : 0;
        opt.isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        bool real_transform = (flags & CV_HAL_DFT_REAL_OUTPUT) != 0;
        opt.isComplex = (stage == 0) && (flags & CV_HAL_DFT_COMPLEX_OUTPUT) != 0;
        bool needAnotherStage = (flags & CV_HAL_DFT_TWO_STAGE) != 0;

        opt.scale = 1;
        opt.n = len;

        opt.useIpp = false;
    #अगर_घोषित USE_IPP_DFT
        opt.ipp_spec = 0;
        opt.ipp_work = 0;

        अगर( CV_IPP_CHECK_COND && (opt.n*गिनती_2 >= 64) && !IPP_DISABLE_DFT32F) // use IPP DFT if available
        {
            पूर्णांक ipp_norm_flag = (flags & CV_HAL_DFT_SCALE) == 0 ? 8 : opt.isInverse ? 2 : 1;
            पूर्णांक specsize=0, initsize=0, worksize=0;
            Ipp_DFTGet_आकार_Func get_आकार_Func = 0;
            IppDFTInitFunc initFunc = 0;

            अगर( real_transform && stage == 0 )
            {
                अगर( depth == CV_32F )
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
                अगर( depth == CV_32F )
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
            अगर( get_आकार_Func(opt.n, ipp_norm_flag, ippAlgHintNone, &specsize, &initsize, &worksize) >= 0 )
            {
                ippbuf.allocate(specsize + initsize + 64);
                opt.ipp_spec = alignPtr(&ippbuf[0], 32);
                ippworkbuf.allocate(worksize + 32);
                opt.ipp_work = alignPtr(&ippworkbuf[0], 32);
                ipp_worksize = (माप_प्रकार)worksize;
                uchar* initbuf = alignPtr((uchar*)opt.ipp_spec + specsize, 32);
                अगर( initFunc(opt.n, ipp_norm_flag, ippAlgHintNone, opt.ipp_spec, initbuf) >= 0 )
                    opt.useIpp = true;
            }
            अन्यथा
                setIppErrorStatus();
        }
    #पूर्ण_अगर

        {
            // The plan is built even when IPP is enabled: the IPP calls fall back to the generic
            // code on failure.
            स्थिर DFTKernels& kernels = getDFTKernels(depth);
            पूर्णांक kind = stage == 0 && real_transform ? (opt.isInverse ? DFT_KIND_C2R : DFT_KIND_R2C)
                                                    : DFT_KIND_C2C;
            plan.build(kind, len, depth, kernels.vlanes);
            खाली.allocate(plan.ws_bytes);
            opt.plan = &plan;
            opt.kernels = &kernels;
            opt.workspace = खाली.तथ्य();
            अगर (need_बफर)
                *need_बफर = false;   // the engine supports in-place for every kind
        }

        {
            अटल DFTFunc dft_tbl[6] =
            {
                dftWrap<Complexf, DFT<भग्न>>,
                dftWrap<भग्न, RealDFT<भग्न>>,
                dftWrap<भग्न, CCSIDFT<भग्न>>,
                dftWrap<Complexd, DFT<द्विगुन>>,
                dftWrap<द्विगुन, RealDFT<द्विगुन>>,
                dftWrap<द्विगुन, CCSIDFT<द्विगुन>>
            };
            पूर्णांक idx = 0;
            अगर (stage == 0)
            {
                अगर (real_transform)
                {
                    अगर (!opt.isInverse)
                        idx = 1;
                    अन्यथा
                        idx = 2;
                }
            }
            अगर (depth == CV_64F)
                idx += 3;

            opt.dft_func = dft_tbl[idx];
        }

        अगर(!needAnotherStage && (flags & CV_HAL_DFT_SCALE) != 0)
        {
            पूर्णांक row_गिनती = गिनती_2;
            अगर (stage == 0 && (flags & CV_HAL_DFT_ROWS) != 0)
                row_गिनती = 1;
            opt.scale = 1./(len * row_गिनती);
        }
    }

    व्योम apply(स्थिर uchar *src, uchar *dst) CV_OVERRIDE
    {
        opt.dft_func(opt, src, dst);
    }

    // Size of a private workspace for applyWithWorkspace(). It covers both the engine workspace
    // and, when IPP is used, the IPP work buffer (only one of them is touched per call).
    माप_प्रकार workspace_आकार() स्थिर
    {
        माप_प्रकार sz = opt.plan ? plan.ws_bytes : 0;
#अगर_घोषित USE_IPP_DFT
        अगर (opt.useIpp)
            sz = std::max(sz, ipp_worksize + 64);
#पूर्ण_अगर
        वापस sz;
    }

    // Same as apply(), but with a caller-provided workspace: several threads may run the same
    // transform as long as each of them uses its own workspace. The plan is immutable and the
    // IPP spec is read-only after init, so both are shared; the IPP work buffer is per call and
    // must therefore be private to the thread, too.
    व्योम applyWithWorkspace(स्थिर uchar *src, uchar *dst, uchar* workspace) स्थिर
    {
        OcvDftOptions opt_ = opt;
        opt_.workspace = workspace;
#अगर_घोषित USE_IPP_DFT
        अगर (opt_.useIpp)
            opt_.ipp_work = alignPtr(workspace, 32);
#पूर्ण_अगर
        opt_.dft_func(opt_, src, dst);
    }

    व्योम मुक्त() {}
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
अटल स्थिर पूर्णांक DFT_COLS_BATCH_MIN = 4;
अटल स्थिर पूर्णांक DFT_COLS_BATCH_MAX = 16;
अटल स्थिर माप_प्रकार DFT_COLS_BATCH_BYTES = 1 << 17;

अटल पूर्णांक dftColsBatch(पूर्णांक len, माप_प्रकार complex_elem_आकार)
{
    पूर्णांक batch = (पूर्णांक)(DFT_COLS_BATCH_BYTES/((माप_प्रकार)len*complex_elem_आकार));
    वापस std::max(DFT_COLS_BATCH_MIN, std::min(DFT_COLS_BATCH_MAX, batch));
}

अटल द्विगुन dftParallelStripes(पूर्णांक गिनती_2, पूर्णांक len)
{
    स्थिर द्विगुन quantum = 0.5e6;
    वापस (द्विगुन)गिनती_2*len*std::max(std::log((द्विगुन)len)/std::log(2.), 1.)/quantum;
}

श्रेणी OcvDftImpl CV_FINAL : खुला hal::DFT2D
{
रक्षित:
    Ptr<hal::DFT1D> contextA;
    Ptr<hal::DFT1D> contextB;
    स्थिर OcvDftBasicImpl* basicA;   // contextA/contextB when they are our own implementation
    स्थिर OcvDftBasicImpl* basicB;   // (parallel path); 0 for external HAL contexts
    bool need_बफर_A;
    bool need_बफर_B;
    bool inv;
    पूर्णांक width;
    पूर्णांक height;
    DftMode mode;
    पूर्णांक elem_आकार_2;
    पूर्णांक complex_elem_आकार;
    पूर्णांक depth;
    bool real_transform;
    पूर्णांक nonzero_rows;
    bool isRowTransform;
    bool isScaled;
    std::vector<पूर्णांक> stages;
    bool useIpp;
    पूर्णांक src_channels;
    पूर्णांक dst_channels;

    Auto_बफर<uchar> tmp_bufA;
    Auto_बफर<uchar> tmp_bufB;
    Auto_बफर<uchar> buf0;
    Auto_बफर<uchar> buf1;

खुला:
    OcvDftImpl()
    {
        basicA = basicB = 0;
        need_बफर_A = false;
        need_बफर_B = false;
        inv = false;
        width = 0;
        height = 0;
        mode = InvalidDft;
        elem_आकार_2 = 0;
        complex_elem_आकार = 0;
        depth = 0;
        real_transform = false;
        nonzero_rows = 0;
        isRowTransform = false;
        isScaled = false;
        useIpp = false;
        src_channels = 0;
        dst_channels = 0;
    }

    व्योम init(पूर्णांक _width, पूर्णांक _height, पूर्णांक _depth, पूर्णांक _src_channels, पूर्णांक _dst_channels, पूर्णांक flags, पूर्णांक _nonzero_rows)
    {
        bool isComplex = _src_channels != _dst_channels;
        nonzero_rows = _nonzero_rows;
        width = _width;
        height = _height;
        depth = _depth;
        src_channels = _src_channels;
        dst_channels = _dst_channels;
        bool isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        bool isInplace = (flags & CV_HAL_DFT_IS_INPLACE) != 0;
        bool isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        mode = determineMode(isInverse, _src_channels, _dst_channels);
        inv = isInverse;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isScaled = (flags & CV_HAL_DFT_SCALE) != 0;
        need_बफर_A = false;
        need_बफर_B = false;
        real_transform = (mode != FwdComplexToComplex && mode != InvComplexToComplex);

        elem_आकार_2 = (depth == CV_32F) ? माप(भग्न) : माप(द्विगुन);
        complex_elem_आकार = elem_आकार_2 * 2;
        अगर( !real_transform )
            elem_आकार_2 = complex_elem_आकार;

#अगर defined USE_IPP_DFT
        CV_IPP_CHECK()
        {
            अगर (nonzero_rows == 0 && depth == CV_32F && ((width * height)>(पूर्णांक)(1<<6)))
            {
                अगर (mode == FwdComplexToComplex || mode == InvComplexToComplex || mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    useIpp = true;
                    वापस;
                }
            }
        }
#पूर्ण_अगर

        DftDims dims = determineDims(height, width, isRowTransform, isContinuous);
        अगर (dims == TwoDims)
        {
            stages.resize(2);
            अगर (mode == InvCCSToReal || mode == InvComplexToReal)
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
            अगर (dims == OneDimColWise)
                stages[0] = 1;
            अन्यथा
                stages[0] = 0;
        }

        क्रम(uint stage_सूचकांक = 0; stage_सूचकांक < stages.size(); ++stage_सूचकांक)
        {
            अगर (stage_सूचकांक == 1)
            {
                isInplace = true;
                isComplex = false;
            }

            पूर्णांक stage = stages[stage_सूचकांक];
            bool isLastStage = (stage_सूचकांक + 1 == stages.size());

            पूर्णांक len, गिनती_2;

            पूर्णांक f = 0;
            अगर (inv)
                f |= CV_HAL_DFT_INVERSE;
            अगर (isScaled)
                f |= CV_HAL_DFT_SCALE;
            अगर (isRowTransform)
                f |= CV_HAL_DFT_ROWS;
            अगर (isComplex)
                f |= CV_HAL_DFT_COMPLEX_OUTPUT;
            अगर (real_transform)
                f |= CV_HAL_DFT_REAL_OUTPUT;
            अगर (!isLastStage)
                f |= CV_HAL_DFT_TWO_STAGE;

            अगर( stage == 0 ) // row-wise transform
            {
                अगर (width == 1 && !isRowTransform )
                {
                    len = height;
                    गिनती_2 = width;
                }
                अन्यथा
                {
                    len = width;
                    गिनती_2 = height;
                }
                need_बफर_A = isInplace;
                contextA = hal::DFT1D::create(len, गिनती_2, depth, f, &need_बफर_A);
                basicA = dynamic_cast<स्थिर OcvDftBasicImpl*>(contextA.लो());
                अगर (need_बफर_A)
                    tmp_bufA.allocate(len * complex_elem_आकार);
            }
            अन्यथा
            {
                len = height;
                गिनती_2 = width;
                f |= CV_HAL_DFT_STAGE_COLS;
                need_बफर_B = isInplace;
                contextB = hal::DFT1D::create(len, गिनती_2, depth, f, &need_बफर_B);
                basicB = dynamic_cast<स्थिर OcvDftBasicImpl*>(contextB.लो());
                अगर (need_बफर_B)
                    tmp_bufB.allocate(len * complex_elem_आकार);

                buf0.allocate(len * complex_elem_आकार);
                buf1.allocate(len * complex_elem_आकार);
            }
        }
    }

    व्योम apply(स्थिर uchar * src, माप_प्रकार src_step, uchar * dst, माप_प्रकार dst_step) CV_OVERRIDE
    {
#अगर defined USE_IPP_DFT
        अगर (useIpp)
        {
            पूर्णांक ipp_norm_flag = !isScaled ? 8 : inv ? 2 : 1;
            अगर (!isRowTransform)
            {
                अगर (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    अगर (ippi_DFT_C_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        वापस;
                    }
                    setIppErrorStatus();
                }
                अन्यथा अगर (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    अगर (ippi_DFT_R_32F(src, src_step, dst, dst_step, width, height, inv, ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP);
                        वापस;
                    }
                    setIppErrorStatus();
                }
            }
            अन्यथा
            {
                अगर (mode == FwdComplexToComplex || mode == InvComplexToComplex)
                {
                    ippiDFT_C_Func ippiFunc = inv ? (ippiDFT_C_Func)ippiDFTInv_CToC_32fc_C1R : (ippiDFT_C_Func)ippiDFTFwd_CToC_32fc_C1R;
                    अगर (Dft_C_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_C_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        वापस;
                    }
                    setIppErrorStatus();
                }
                अन्यथा अगर (mode == FwdRealToCCS || mode == InvCCSToReal)
                {
                    ippiDFT_R_Func ippiFunc = inv ? (ippiDFT_R_Func)ippiDFTInv_PackToR_32f_C1R : (ippiDFT_R_Func)ippiDFTFwd_RToPack_32f_C1R;
                    अगर (Dft_R_IPPLoop(src, src_step, dst, dst_step, width, height, IPPDFT_R_Functor(ippiFunc),ipp_norm_flag))
                    {
                        CV_IMPL_ADD(CV_IMPL_IPP|CV_IMPL_MT);
                        वापस;
                    }
                    setIppErrorStatus();
                }
            }
            वापस;
        }
#पूर्ण_अगर

        क्रम(uint stage_सूचकांक = 0; stage_सूचकांक < stages.size(); ++stage_सूचकांक)
        {
            पूर्णांक stage_src_channels = src_channels;
            पूर्णांक stage_dst_channels = dst_channels;

            अगर (stage_सूचकांक == 1)
            {
                src = dst;
                src_step = dst_step;
                stage_src_channels = stage_dst_channels;
            }

            पूर्णांक stage = stages[stage_सूचकांक];
            bool isLastStage = (stage_सूचकांक + 1 == stages.size());
            bool isComplex = stage_src_channels != stage_dst_channels;

            अगर( stage == 0 )
                rowDft(src, src_step, dst, dst_step, isComplex, isLastStage);
            अन्यथा
                colDft(src, src_step, dst, dst_step, stage_src_channels, stage_dst_channels, isLastStage);
        }
    }

रक्षित:

    व्योम rowDft(स्थिर uchar* src_तथ्य, माप_प्रकार src_step, uchar* dst_तथ्य, माप_प्रकार dst_step, bool isComplex, bool isLastStage)
    {
        पूर्णांक len, गिनती_2;
        अगर (width == 1 && !isRowTransform )
        {
            len = height;
            गिनती_2 = width;
        }
        अन्यथा
        {
            len = width;
            गिनती_2 = height;
        }
        पूर्णांक dptr_offset = 0;
        पूर्णांक dst_full_len = len*elem_आकार_2;

        अगर( need_बफर_A )
        {
            अगर (mode == FwdRealToCCS && (len & 1) && len > 1)
                dptr_offset = elem_आकार_2;
        }

        अगर( !inv && isComplex )
            dst_full_len += (len & 1) ? elem_आकार_2 : complex_elem_आकार;

        पूर्णांक nz = nonzero_rows;
        अगर( nz <= 0 || nz > गिनती_2 )
            nz = गिनती_2;

        माप_प्रकार ws_आकार = basicA ? basicA->workspace_आकार() : 0;
        द्विगुन nstripes = dftParallelStripes(nz, len);
        अगर( ws_आकार > 0 && nz > 1 && nstripes >= 1.5 )
        {
            // every worker gets its own workspace (and row buffer) once per range
            स्थिर OcvDftBasicImpl* ctx = basicA;
            स्वतः processRows = [&, ctx](स्थिर Range& range)
            {
                Auto_बफर<uchar> खाली(ws_आकार);
                Auto_बफर<uchar> rowbuf(need_बफर_A ? len * complex_elem_आकार : 0);
                क्रम( पूर्णांक i = range.start; i < range.end; i++ )
                {
                    स्थिर uchar* sptr = src_तथ्य + src_step * i;
                    uchar* dptr0 = dst_तथ्य + dst_step * i;
                    uchar* dptr = need_बफर_A ? rowbuf.तथ्य() : dptr0;
                    ctx->applyWithWorkspace(sptr, dptr, खाली.तथ्य());
                    अगर( need_बफर_A )
                        स_नकल( dptr0, dptr + dptr_offset, dst_full_len );
                }
            };
            parallel_for_(Range(0, nz), processRows, nstripes);
        }
        अन्यथा
        {
            क्रम( पूर्णांक i = 0; i < nz; i++ )
            {
                स्थिर uchar* sptr = src_तथ्य + src_step * i;
                uchar* dptr0 = dst_तथ्य + dst_step * i;
                uchar* dptr = dptr0;

                अगर( need_बफर_A )
                    dptr = tmp_bufA.तथ्य();

                contextA->apply(sptr, dptr);

                अगर( need_बफर_A )
                    स_नकल( dptr0, dptr + dptr_offset, dst_full_len );
            }
        }

        क्रम( पूर्णांक i = nz; i < गिनती_2; i++ )
        {
            uchar* dptr0 = dst_तथ्य + dst_step * i;
            स_रखो( dptr0, 0, dst_full_len );
        }
        अगर(isLastStage &&  mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_तथ्य, dst_step, len, nz, 1);
    }

    व्योम colDft(स्थिर uchar* src_तथ्य, माप_प्रकार src_step, uchar* dst_तथ्य, माप_प्रकार dst_step, पूर्णांक stage_src_channels, पूर्णांक stage_dst_channels, bool isLastStage)
    {
        पूर्णांक len = height;
        पूर्णांक गिनती_2 = width;
        पूर्णांक a = 0, b = गिनती_2;
        uchar *dbuf0, *dbuf1;
        स्थिर uchar* sptr0 = src_तथ्य;
        uchar* dptr0 = dst_तथ्य;

        dbuf0 = buf0.तथ्य(), dbuf1 = buf1.तथ्य();

        अगर( need_बफर_B )
        {
            dbuf1 = tmp_bufB.तथ्य();
            dbuf0 = buf1.तथ्य();
        }

        अगर( real_transform )
        {
            पूर्णांक even;
            a = 1;
            even = (गिनती_2 & 1) == 0;
            b = (गिनती_2+1)/2;
            अगर( !inv )
            {
                स_रखो( buf0.तथ्य(), 0, len*complex_elem_आकार );
                CopyColumn( sptr0, src_step, buf0.तथ्य(), complex_elem_आकार, len, elem_आकार_2 );
                sptr0 += stage_dst_channels*elem_आकार_2;
                अगर( even )
                {
                    स_रखो( buf1.तथ्य(), 0, len*complex_elem_आकार );
                    CopyColumn( sptr0 + (गिनती_2-2)*elem_आकार_2, src_step,
                                buf1.तथ्य(), complex_elem_आकार, len, elem_आकार_2 );
                }
            }
            अन्यथा अगर( stage_src_channels == 1 )
            {
                CopyColumn( sptr0, src_step, buf0.तथ्य(), elem_आकार_2, len, elem_आकार_2 );
                ExpandCCS( buf0.तथ्य(), len, elem_आकार_2 );
                अगर( even )
                {
                    CopyColumn( sptr0 + (गिनती_2-1)*elem_आकार_2, src_step,
                                buf1.तथ्य(), elem_आकार_2, len, elem_आकार_2 );
                    ExpandCCS( buf1.तथ्य(), len, elem_आकार_2 );
                }
                sptr0 += elem_आकार_2;
            }
            अन्यथा
            {
                CopyColumn( sptr0, src_step, buf0.तथ्य(), complex_elem_आकार, len, complex_elem_आकार );
                अगर( even )
                {
                    CopyColumn( sptr0 + b*complex_elem_आकार, src_step,
                                   buf1.तथ्य(), complex_elem_आकार, len, complex_elem_आकार );
                }
                sptr0 += complex_elem_आकार;
            }

            अगर( even )
                contextB->apply(buf1.तथ्य(), dbuf1);
            contextB->apply(buf0.तथ्य(), dbuf0);

            अगर( stage_dst_channels == 1 )
            {
                अगर( !inv )
                {
                    // copy the half of output vector to the first/last column.
                    // before doing that, defgragment the vector
                    स_नकल( dbuf0 + elem_आकार_2, dbuf0, elem_आकार_2 );
                    CopyColumn( dbuf0 + elem_आकार_2, elem_आकार_2, dptr0,
                                   dst_step, len, elem_आकार_2 );
                    अगर( even )
                    {
                        स_नकल( dbuf1 + elem_आकार_2, dbuf1, elem_आकार_2 );
                        CopyColumn( dbuf1 + elem_आकार_2, elem_आकार_2,
                                       dptr0 + (गिनती_2-1)*elem_आकार_2,
                                       dst_step, len, elem_आकार_2 );
                    }
                    dptr0 += elem_आकार_2;
                }
                अन्यथा
                {
                    // copy the real part of the complex vector to the first/last column
                    CopyColumn( dbuf0, complex_elem_आकार, dptr0, dst_step, len, elem_आकार_2 );
                    अगर( even )
                        CopyColumn( dbuf1, complex_elem_आकार, dptr0 + (गिनती_2-1)*elem_आकार_2,
                                       dst_step, len, elem_आकार_2 );
                    dptr0 += elem_आकार_2;
                }
            }
            अन्यथा
            {
                CV_Assert( !inv );
                CopyColumn( dbuf0, complex_elem_आकार, dptr0,
                               dst_step, len, complex_elem_आकार );
                अगर( even )
                    CopyColumn( dbuf1, complex_elem_आकार,
                                   dptr0 + b*complex_elem_आकार,
                                   dst_step, len, complex_elem_आकार );
                dptr0 += complex_elem_आकार;
            }
        }

        // the remaining columns are processed in batches of cols_batch columns: copied row-wise into
        // contiguous per-column buffers, transformed one by one, copied back row-wise
        पूर्णांक ncols = b - a;
        स्थिर पूर्णांक cols_batch = dftColsBatch(len, complex_elem_आकार);
        पूर्णांक nbatches = (ncols + cols_batch - 1)/cols_batch;
        माप_प्रकार ws_आकार = basicB ? basicB->workspace_आकार() : 0;
        माप_प्रकार colbuf_आकार = (माप_प्रकार)len*complex_elem_आकार;
        स्थिर OcvDftBasicImpl* ctx = basicB;
        स्वतः processBatches = [&, ctx](स्थिर Range& range)
        {
            Auto_बफर<uchar> खाली(ws_आकार);
            Auto_बफर<uchar> cbuf(colbuf_आकार*cols_batch*(need_बफर_B ? 2 : 1));
            uchar* cbuf0 = cbuf.तथ्य();
            uchar* cdbuf0 = need_बफर_B ? cbuf0 + colbuf_आकार*cols_batch : cbuf0;
            क्रम( पूर्णांक bi = range.start; bi < range.end; bi++ )
            {
                पूर्णांक c0 = bi*cols_batch;
                पूर्णांक nc = std::min(cols_batch, ncols - c0);
                स्थिर uchar* sptr = sptr0 + (माप_प्रकार)c0*complex_elem_आकार;
                uchar* dptr = dptr0 + (माप_प्रकार)c0*complex_elem_आकार;
                CopyFromColumns( sptr, src_step, cbuf0, nc, len, complex_elem_आकार );
                क्रम( पूर्णांक k = 0; k < nc; k++ )
                {
                    अगर( ctx )
                        ctx->applyWithWorkspace(cbuf0 + k*colbuf_आकार, cdbuf0 + k*colbuf_आकार, खाली.तथ्य());
                    अन्यथा
                        contextB->apply(cbuf0 + k*colbuf_आकार, cdbuf0 + k*colbuf_आकार);
                }
                CopyToColumns( cdbuf0, dptr, dst_step, nc, len, complex_elem_आकार );
            }
        };
        द्विगुन nstripes = dftParallelStripes(ncols, len);
        अगर( ctx && nbatches > 1 && nstripes >= 1.5 )
            parallel_for_(Range(0, nbatches), processBatches, nstripes);
        अन्यथा
            processBatches(Range(0, nbatches));
        अगर(isLastStage && mode == FwdRealToComplex)
            complementComplexOutput(depth, dst_तथ्य, dst_step, गिनती_2, len, 2);
    }
};

काष्ठा ReplacementDFT1D : खुला hal::DFT1D
{
    cvhalDFT *context;
    bool isInitialized;

    ReplacementDFT1D() : context(0), isInitialized(false) {}
    bool init(पूर्णांक len, पूर्णांक गिनती_2, पूर्णांक depth, पूर्णांक flags, bool *need_बफर)
    {
        पूर्णांक res = cv_hal_dftInit1D(&context, len, गिनती_2, depth, flags, need_बफर);
        isInitialized = (res == CV_HAL_ERROR_OK);
        वापस isInitialized;
    }
    व्योम apply(स्थिर uchar *src, uchar *dst) CV_OVERRIDE
    {
        अगर (isInitialized)
        {
            CALL_HAL(dft1D, cv_hal_dft1D, context, src, dst);
        }
    }
    ~ReplacementDFT1D()
    {
        अगर (isInitialized)
        {
            CALL_HAL(dftFree1D, cv_hal_dftFree1D, context);
        }
    }
};

काष्ठा ReplacementDFT2D : खुला hal::DFT2D
{
    cvhalDFT *context;
    bool isInitialized;

    ReplacementDFT2D() : context(0), isInitialized(false) {}
    bool init(पूर्णांक width, पूर्णांक height, पूर्णांक depth,
              पूर्णांक src_channels, पूर्णांक dst_channels,
              पूर्णांक flags, पूर्णांक nonzero_rows)
    {
        पूर्णांक res = cv_hal_dftInit2D(&context, width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        isInitialized = (res == CV_HAL_ERROR_OK);
        वापस isInitialized;
    }
    व्योम apply(स्थिर uchar *src, माप_प्रकार src_step, uchar *dst, माप_प्रकार dst_step) CV_OVERRIDE
    {
        अगर (isInitialized)
        {
            CALL_HAL(dft2D, cv_hal_dft2D, context, src, src_step, dst, dst_step);
        }
    }
    ~ReplacementDFT2D()
    {
        अगर (isInitialized)
        {
            CALL_HAL(dftFree2D, cv_hal_dftFree1D, context);
        }
    }
};

namespace hal {

//================== 1D ======================

Ptr<DFT1D> DFT1D::create(पूर्णांक len, पूर्णांक गिनती_2, पूर्णांक depth, पूर्णांक flags, bool *need_बफर)
{
    {
        ReplacementDFT1D *impl = नया ReplacementDFT1D();
        अगर (impl->init(len, गिनती_2, depth, flags, need_बफर))
        {
            वापस Ptr<DFT1D>(impl);
        }
        मिटाओ impl;
    }
    {
        OcvDftBasicImpl *impl = नया OcvDftBasicImpl();
        impl->init(len, गिनती_2, depth, flags, need_बफर);
        वापस Ptr<DFT1D>(impl);
    }
}

//================== 2D ======================

Ptr<DFT2D> DFT2D::create(पूर्णांक width, पूर्णांक height, पूर्णांक depth,
                         पूर्णांक src_channels, पूर्णांक dst_channels,
                         पूर्णांक flags, पूर्णांक nonzero_rows)
{
    {
        ReplacementDFT2D *impl = नया ReplacementDFT2D();
        अगर (impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows))
        {
            वापस Ptr<DFT2D>(impl);
        }
        मिटाओ impl;
    }
    {
        अगर(width == 1 && nonzero_rows > 0 )
        {
            CV_Error( cv::Error::StsNotImplemented,
            "This mode (using nonzero_rows with a single-column matrix) breaks the function's logic, so it is prohibited.\n"
            "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
        }
        OcvDftImpl *impl = नया OcvDftImpl();
        impl->init(width, height, depth, src_channels, dst_channels, flags, nonzero_rows);
        वापस Ptr<DFT2D>(impl);
    }
}

} // cv::hal::
} // cv::


व्योम cv::dft( निवेश_Array _src0, OutputArray _dst, पूर्णांक flags, पूर्णांक nonzero_rows )
{
    CV_INSTRUMENT_REGION();

#अगर_घोषित HAVE_CLAMDFFT
    CV_OCL_RUN(ocl::haveAmdFft() && ocl::Device::getDefault().type() != ocl::Device::TYPE_CPU &&
            _dst.isUMat() && _src0.dims() <= 2 && nonzero_rows == 0,
               ocl_dft_amdfft(_src0, _dst, flags))
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENCL
    CV_OCL_RUN(_dst.isUMat() && _src0.dims() <= 2,
               ocl_dft(_src0, _dst, flags, nonzero_rows))
#पूर्ण_अगर

    Mat src0 = _src0.getMat(), src = src0;
    अगर (src.empty())
    {
        _dst.release();
        वापस;
    }
    bool inv = (flags & DFT_INVERSE) != 0;
    पूर्णांक type = src.type();
    पूर्णांक depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );

    // Fail if DFT_COMPLEX_INPUT is specified, but src is not 2 channels.
    CV_Assert( !((flags & DFT_COMPLEX_निवेश) && src.channels() != 2) );

    अगर( !inv && src.channels() == 1 && (flags & DFT_COMPLEX_OUTPUT) )
        _dst.create_Same_आकार( src, CV_MAKETYPE(depth, 2) );
    अन्यथा अगर( inv && src.channels() == 2 && (flags & DFT_REAL_OUTPUT) )
        _dst.create_Same_आकार( src, depth );
    अन्यथा
        _dst.create_Same_आकार( src, type );

    Mat dst = _dst.getMat();

    पूर्णांक f = 0;
    अगर (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;
    अगर (inv)
        f |= CV_HAL_DFT_INVERSE;
    अगर (flags & DFT_ROWS)
        f |= CV_HAL_DFT_ROWS;
    अगर (flags & DFT_SCALE)
        f |= CV_HAL_DFT_SCALE;
    अगर (src.तथ्य == dst.तथ्य)
        f |= CV_HAL_DFT_IS_INPLACE;
    Ptr<hal::DFT2D> c = hal::DFT2D::create(src.cols, src.rows, depth, src.channels(), dst.channels(), f, nonzero_rows);
    c->apply(src.तथ्य, src.step, dst.तथ्य, dst.step);
}


व्योम cv::idft( निवेश_Array src, OutputArray dst, पूर्णांक flags, पूर्णांक nonzero_rows )
{
    CV_INSTRUMENT_REGION();

    dft( src, dst, flags | DFT_INVERSE, nonzero_rows );
}

#अगर_घोषित HAVE_OPENCL

namespace cv {

अटल bool ocl_mulSpectrums( निवेश_Array _srcA, निवेश_Array _srcB,
                              OutputArray _dst, पूर्णांक flags, bool conjB )
{
    पूर्णांक atype = _srcA.type(), btype = _srcB.type(),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    आकार asize = _srcA.size(), bsize = _srcB.size();
    CV_Assert(asize == bsize);

    अगर ( !(atype == CV_32FC2 && btype == CV_32FC2) || flags != 0 )
        वापस false;

    UMat A = _srcA.getUMat(), B = _srcB.getUMat();
    CV_Assert(A.size() == B.size());

    _dst.create_Same_आकार(A, atype);
    UMat dst = _dst.getUMat();

    ocl::Kernel k("mulAndScaleSpectrums",
                  ocl::core::mulspectrums_oclsrc,
                  format("%s", conjB ? "-D CONJ" : ""));
    अगर (k.empty())
        वापस false;

    k.args(ocl::KernelArg::Read_Only_No_आकार(A), ocl::KernelArg::Read_Only_No_आकार(B),
           ocl::KernelArg::WriteOnly(dst), rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)asize.width, ((माप_प्रकार)asize.height + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

}

#पूर्ण_अगर

namespace {

#घोषणा VAL(buf, elem) (((T*)((अक्षर*)तथ्य ## buf + (step ## buf * (elem))))[0])
#घोषणा MUL_SPECTRUMS_COL(A, B, C) \
    VAL(C, 0) = VAL(A, 0) * VAL(B, 0); \
    क्रम (माप_प्रकार j = 1; j <= rows - 2; j += 2) \
    { \
        द्विगुन a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        द्विगुन b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        अगर (conjB) b_im = -b_im; \
        द्विगुन c_re = a_re * b_re - a_im * b_im; \
        द्विगुन c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    } \
    अगर ((rows & 1) == 0) \
        VAL(C, rows-1) = VAL(A, rows-1) * VAL(B, rows-1)

ढाँचा <typename T, bool conjB> अटल अंतरभूत
व्योम mulSpectrums_processCol_noinplace(स्थिर T* तथ्य_A, स्थिर T* तथ्य_B, T* तथ्य_C, माप_प्रकार stepA, माप_प्रकार stepB, माप_प्रकार stepC, माप_प्रकार rows)
{
    MUL_SPECTRUMS_COL(A, B, C);
}

ढाँचा <typename T, bool conjB> अटल अंतरभूत
व्योम mulSpectrums_processCol_inplaceA(स्थिर T* तथ्य_B, T* तथ्य_AC, माप_प्रकार stepB, माप_प्रकार stepAC, माप_प्रकार rows)
{
    MUL_SPECTRUMS_COL(AC, B, AC);
}
ढाँचा <typename T, bool conjB, bool inplaceA> अटल अंतरभूत
व्योम mulSpectrums_processCol(स्थिर T* तथ्य_A, स्थिर T* तथ्य_B, T* तथ्य_C, माप_प्रकार stepA, माप_प्रकार stepB, माप_प्रकार stepC, माप_प्रकार rows)
{
    अगर (inplaceA)
        mulSpectrums_processCol_inplaceA<T, conjB>(तथ्य_B, तथ्य_C, stepB, stepC, rows);
    अन्यथा
        mulSpectrums_processCol_noinplace<T, conjB>(तथ्य_A, तथ्य_B, तथ्य_C, stepA, stepB, stepC, rows);
}
#अघोषित MUL_SPECTRUMS_COL
#अघोषित VAL

ढाँचा <typename T, bool conjB, bool inplaceA> अटल अंतरभूत
व्योम mulSpectrums_processCols(स्थिर T* तथ्य_A, स्थिर T* तथ्य_B, T* तथ्य_C, माप_प्रकार stepA, माप_प्रकार stepB, माप_प्रकार stepC, माप_प्रकार rows, माप_प्रकार cols)
{
    mulSpectrums_processCol<T, conjB, inplaceA>(तथ्य_A, तथ्य_B, तथ्य_C, stepA, stepB, stepC, rows);
    अगर ((cols & 1) == 0)
    {
        mulSpectrums_processCol<T, conjB, inplaceA>(तथ्य_A + cols - 1, तथ्य_B + cols - 1, तथ्य_C + cols - 1, stepA, stepB, stepC, rows);
    }
}

#घोषणा VAL(buf, elem) (तथ्य ## buf[(elem)])
#घोषणा MUL_SPECTRUMS_ROW(A, B, C) \
    क्रम (माप_प्रकार j = j0; j < j1; j += 2) \
    { \
        द्विगुन a_re = VAL(A, j), a_im = VAL(A, j + 1); \
        द्विगुन b_re = VAL(B, j), b_im = VAL(B, j + 1); \
        अगर (conjB) b_im = -b_im; \
        द्विगुन c_re = a_re * b_re - a_im * b_im; \
        द्विगुन c_im = a_re * b_im + a_im * b_re; \
        VAL(C, j) = (T)c_re; VAL(C, j + 1) = (T)c_im; \
    }
ढाँचा <typename T, bool conjB> अटल अंतरभूत
व्योम mulSpectrums_processRow_noinplace(स्थिर T* तथ्य_A, स्थिर T* तथ्य_B, T* तथ्य_C, माप_प्रकार j0, माप_प्रकार j1)
{
    MUL_SPECTRUMS_ROW(A, B, C);
}
ढाँचा <typename T, bool conjB> अटल अंतरभूत
व्योम mulSpectrums_processRow_inplaceA(स्थिर T* तथ्य_B, T* तथ्य_AC, माप_प्रकार j0, माप_प्रकार j1)
{
    MUL_SPECTRUMS_ROW(AC, B, AC);
}
ढाँचा <typename T, bool conjB, bool inplaceA> अटल अंतरभूत
व्योम mulSpectrums_processRow(स्थिर T* तथ्य_A, स्थिर T* तथ्य_B, T* तथ्य_C, माप_प्रकार j0, माप_प्रकार j1)
{
    अगर (inplaceA)
        mulSpectrums_processRow_inplaceA<T, conjB>(तथ्य_B, तथ्य_C, j0, j1);
    अन्यथा
        mulSpectrums_processRow_noinplace<T, conjB>(तथ्य_A, तथ्य_B, तथ्य_C, j0, j1);
}
#अघोषित MUL_SPECTRUMS_ROW
#अघोषित VAL

ढाँचा <typename T, bool conjB, bool inplaceA> अटल अंतरभूत
व्योम mulSpectrums_processRows(स्थिर T* तथ्य_A, स्थिर T* तथ्य_B, T* तथ्य_C, माप_प्रकार stepA, माप_प्रकार stepB, माप_प्रकार stepC, माप_प्रकार rows, माप_प्रकार cols, माप_प्रकार j0, माप_प्रकार j1, bool is_1d_CN1)
{
    जबतक (rows-- > 0)
    {
        अगर (is_1d_CN1)
            तथ्य_C[0] = तथ्य_A[0]*तथ्य_B[0];
        mulSpectrums_processRow<T, conjB, inplaceA>(तथ्य_A, तथ्य_B, तथ्य_C, j0, j1);
        अगर (is_1d_CN1 && (cols & 1) == 0)
            तथ्य_C[j1] = तथ्य_A[j1]*तथ्य_B[j1];

        तथ्य_A = (स्थिर T*)(((अक्षर*)तथ्य_A) + stepA);
        तथ्य_B = (स्थिर T*)(((अक्षर*)तथ्य_B) + stepB);
        तथ्य_C =       (T*)(((अक्षर*)तथ्य_C) + stepC);
    }
}


ढाँचा <typename T, bool conjB, bool inplaceA> अटल अंतरभूत
व्योम mulSpectrums_Impl_(स्थिर T* तथ्य_A, स्थिर T* तथ्य_B, T* तथ्य_C, माप_प्रकार stepA, माप_प्रकार stepB, माप_प्रकार stepC, माप_प्रकार rows, माप_प्रकार cols, माप_प्रकार j0, माप_प्रकार j1, bool is_1d, bool isCN1)
{
    अगर (!is_1d && isCN1)
    {
        mulSpectrums_processCols<T, conjB, inplaceA>(तथ्य_A, तथ्य_B, तथ्य_C, stepA, stepB, stepC, rows, cols);
    }
    mulSpectrums_processRows<T, conjB, inplaceA>(तथ्य_A, तथ्य_B, तथ्य_C, stepA, stepB, stepC, rows, cols, j0, j1, is_1d && isCN1);
}
ढाँचा <typename T, bool conjB> अटल अंतरभूत
व्योम mulSpectrums_Impl(स्थिर T* तथ्य_A, स्थिर T* तथ्य_B, T* तथ्य_C, माप_प्रकार stepA, माप_प्रकार stepB, माप_प्रकार stepC, माप_प्रकार rows, माप_प्रकार cols, माप_प्रकार j0, माप_प्रकार j1, bool is_1d, bool isCN1)
{
    अगर (तथ्य_A == तथ्य_C)
        mulSpectrums_Impl_<T, conjB, true>(तथ्य_A, तथ्य_B, तथ्य_C, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
    अन्यथा
        mulSpectrums_Impl_<T, conjB, false>(तथ्य_A, तथ्य_B, तथ्य_C, stepA, stepB, stepC, rows, cols, j0, j1, is_1d, isCN1);
}

} // namespace

व्योम cv::mulSpectrums( निवेश_Array _srcA, निवेश_Array _srcB,
                       OutputArray _dst, पूर्णांक flags, bool conjB )
{
    CV_INSTRUMENT_REGION();

    अगर (_srcA.empty() && _srcB.empty())
    {
        _dst.release();
        वापस;
    }

    पूर्णांक type = _srcA.type();
    CV_Assert( type == _srcB.type() && _srcA.size() == _srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !_srcA.empty() && !_srcB.empty() );

    CV_OCL_RUN(_dst.isUMat() && _srcA.dims() <= 2 && _srcB.dims() <= 2,
            ocl_mulSpectrums(_srcA, _srcB, _dst, flags, conjB))

    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    पूर्णांक depth = srcA.depth(), cn = srcA.channels();
    माप_प्रकार rows = srcA.rows, cols = srcA.cols;

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    // correct inplace support
    // Case 'dst.data == srcA.data' is handled by implementation,
    // because it is used frequently (filter2D, matchTemplate)
    अगर (dst.तथ्य == srcB.तथ्य)
        srcB = srcB.clone(); // workaround for B only

    bool is_1d = (flags & DFT_ROWS)
        || (rows == 1)
        || (cols == 1 && srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous());

    अगर( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    bool isCN1 = cn == 1;
    माप_प्रकार j0 = isCN1 ? 1 : 0;
    माप_प्रकार j1 = cols*cn - (((cols & 1) == 0 && cn == 1) ? 1 : 0);

    अगर (depth == CV_32F)
    {
        स्थिर भग्न* तथ्य_A = srcA.ptr<भग्न>();
        स्थिर भग्न* तथ्य_B = srcB.ptr<भग्न>();
        भग्न* तथ्य_C = dst.ptr<भग्न>();
        अगर (!conjB)
            mulSpectrums_Impl<भग्न, false>(तथ्य_A, तथ्य_B, तथ्य_C, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        अन्यथा
            mulSpectrums_Impl<भग्न, true>(तथ्य_A, तथ्य_B, तथ्य_C, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
    अन्यथा
    {
        स्थिर द्विगुन* तथ्य_A = srcA.ptr<द्विगुन>();
        स्थिर द्विगुन* तथ्य_B = srcB.ptr<द्विगुन>();
        द्विगुन* तथ्य_C = dst.ptr<द्विगुन>();
        अगर (!conjB)
            mulSpectrums_Impl<द्विगुन, false>(तथ्य_A, तथ्य_B, तथ्य_C, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
        अन्यथा
            mulSpectrums_Impl<द्विगुन, true>(तथ्य_A, तथ्य_B, तथ्य_C, srcA.step, srcB.step, dst.step, rows, cols, j0, j1, is_1d, isCN1);
    }
}

व्योम cv::divSpectrums( निवेश_Array _srcA, निवेश_Array _srcB, OutputArray _dst, पूर्णांक flags, bool conjB)
{
    Mat srcA = _srcA.getMat(), srcB = _srcB.getMat();
    अगर (srcA.empty() && srcB.empty())
    {
        _dst.release();
        वापस;
    }
    पूर्णांक depth = srcA.depth(), cn = srcA.channels(), type = srcA.type();
    पूर्णांक rows = srcA.rows, cols = srcA.cols;
    पूर्णांक j, k;

    CV_Assert( type == srcB.type() && srcA.size() == srcB.size() );
    CV_Assert( type == CV_32FC1 || type == CV_32FC2 || type == CV_64FC1 || type == CV_64FC2 );
    CV_Assert( !srcA.empty() && !srcB.empty() );

    _dst.create( srcA.rows, srcA.cols, type );
    Mat dst = _dst.getMat();

    CV_Assert(dst.तथ्य != srcA.तथ्य); // non-inplace check
    CV_Assert(dst.तथ्य != srcB.तथ्य); // non-inplace check

    bool is_1d = (flags & DFT_ROWS) || (rows == 1 || (cols == 1 &&
    srcA.isContinuous() && srcB.isContinuous() && dst.isContinuous()));

    अगर( is_1d && !(flags & DFT_ROWS) )
        cols = cols + rows - 1, rows = 1;

    पूर्णांक ncols = cols*cn;
    पूर्णांक j0 = cn == 1;
    पूर्णांक j1 = ncols - (cols % 2 == 0 && cn == 1);

    अगर( depth == CV_32F )
    {
        स्थिर भग्न* तथ्य_A = srcA.ptr<भग्न>();
        स्थिर भग्न* तथ्य_B = srcB.ptr<भग्न>();
        भग्न* तथ्य_C = dst.ptr<भग्न>();
        भग्न eps = भग्न_अंतर; // prevent div0 problems

        माप_प्रकार stepA = srcA.step/माप(तथ्य_A[0]);
        माप_प्रकार stepB = srcB.step/माप(तथ्य_B[0]);
        माप_प्रकार stepC = dst.step/माप(तथ्य_C[0]);

        अगर( !is_1d && cn == 1 )
        {
            क्रम( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                अगर( k == 1 )
                    तथ्य_A += cols - 1, तथ्य_B += cols - 1, तथ्य_C += cols - 1;
                तथ्य_C[0] = तथ्य_A[0] / (तथ्य_B[0] + eps);
                अगर( rows % 2 == 0 )
                    तथ्य_C[(rows-1)*stepC] = तथ्य_A[(rows-1)*stepA] / (तथ्य_B[(rows-1)*stepB] + eps);
                अगर( !conjB )
                    क्रम( j = 1; j <= rows - 2; j += 2 )
                    {
                        द्विगुन denom = (द्विगुन)तथ्य_B[j*stepB]*तथ्य_B[j*stepB] +
                        (द्विगुन)तथ्य_B[(j+1)*stepB]*तथ्य_B[(j+1)*stepB] + (द्विगुन)eps;

                        द्विगुन re = (द्विगुन)तथ्य_A[j*stepA]*तथ्य_B[j*stepB] +
                        (द्विगुन)तथ्य_A[(j+1)*stepA]*तथ्य_B[(j+1)*stepB];

                        द्विगुन im = (द्विगुन)तथ्य_A[(j+1)*stepA]*तथ्य_B[j*stepB] -
                        (द्विगुन)तथ्य_A[j*stepA]*तथ्य_B[(j+1)*stepB];

                        तथ्य_C[j*stepC] = (भग्न)(re / denom);
                        तथ्य_C[(j+1)*stepC] = (भग्न)(im / denom);
                    }
                    अन्यथा
                        क्रम( j = 1; j <= rows - 2; j += 2 )
                        {

                            द्विगुन denom = (द्विगुन)तथ्य_B[j*stepB]*तथ्य_B[j*stepB] +
                            (द्विगुन)तथ्य_B[(j+1)*stepB]*तथ्य_B[(j+1)*stepB] + (द्विगुन)eps;

                            द्विगुन re = (द्विगुन)तथ्य_A[j*stepA]*तथ्य_B[j*stepB] -
                            (द्विगुन)तथ्य_A[(j+1)*stepA]*तथ्य_B[(j+1)*stepB];

                            द्विगुन im = (द्विगुन)तथ्य_A[(j+1)*stepA]*तथ्य_B[j*stepB] +
                            (द्विगुन)तथ्य_A[j*stepA]*तथ्य_B[(j+1)*stepB];

                            तथ्य_C[j*stepC] = (भग्न)(re / denom);
                            तथ्य_C[(j+1)*stepC] = (भग्न)(im / denom);
                        }
                    अगर( k == 1 )
                        तथ्य_A -= cols - 1, तथ्य_B -= cols - 1, तथ्य_C -= cols - 1;
            }
        }

        क्रम( ; rows--; तथ्य_A += stepA, तथ्य_B += stepB, तथ्य_C += stepC )
        {
            अगर( is_1d && cn == 1 )
            {
                तथ्य_C[0] = तथ्य_A[0] / (तथ्य_B[0] + eps);
                अगर( cols % 2 == 0 )
                    तथ्य_C[j1] = तथ्य_A[j1] / (तथ्य_B[j1] + eps);
            }

            अगर( !conjB )
                क्रम( j = j0; j < j1; j += 2 )
                {
                    द्विगुन denom = (द्विगुन)तथ्य_B[j]*तथ्य_B[j] + (द्विगुन)तथ्य_B[j+1]*तथ्य_B[j+1] + (द्विगुन)eps;
                    द्विगुन re = (द्विगुन)तथ्य_A[j]*तथ्य_B[j] + (द्विगुन)तथ्य_A[j+1]*तथ्य_B[j+1];
                    द्विगुन im = (द्विगुन)तथ्य_A[j+1]*तथ्य_B[j] - (द्विगुन)तथ्य_A[j]*तथ्य_B[j+1];
                    तथ्य_C[j] = (भग्न)(re / denom);
                    तथ्य_C[j+1] = (भग्न)(im / denom);
                }
                अन्यथा
                    क्रम( j = j0; j < j1; j += 2 )
                    {
                        द्विगुन denom = (द्विगुन)तथ्य_B[j]*तथ्य_B[j] + (द्विगुन)तथ्य_B[j+1]*तथ्य_B[j+1] + (द्विगुन)eps;
                        द्विगुन re = (द्विगुन)तथ्य_A[j]*तथ्य_B[j] - (द्विगुन)तथ्य_A[j+1]*तथ्य_B[j+1];
                        द्विगुन im = (द्विगुन)तथ्य_A[j+1]*तथ्य_B[j] + (द्विगुन)तथ्य_A[j]*तथ्य_B[j+1];
                        तथ्य_C[j] = (भग्न)(re / denom);
                        तथ्य_C[j+1] = (भग्न)(im / denom);
                    }
        }
    }
    अन्यथा
    {
        स्थिर द्विगुन* तथ्य_A = srcA.ptr<द्विगुन>();
        स्थिर द्विगुन* तथ्य_B = srcB.ptr<द्विगुन>();
        द्विगुन* तथ्य_C = dst.ptr<द्विगुन>();
        द्विगुन eps = द्विग_अंतर; // prevent div0 problems

        माप_प्रकार stepA = srcA.step/माप(तथ्य_A[0]);
        माप_प्रकार stepB = srcB.step/माप(तथ्य_B[0]);
        माप_प्रकार stepC = dst.step/माप(तथ्य_C[0]);

        अगर( !is_1d && cn == 1 )
        {
            क्रम( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                अगर( k == 1 )
                    तथ्य_A += cols - 1, तथ्य_B += cols - 1, तथ्य_C += cols - 1;
                तथ्य_C[0] = तथ्य_A[0] / (तथ्य_B[0] + eps);
                अगर( rows % 2 == 0 )
                    तथ्य_C[(rows-1)*stepC] = तथ्य_A[(rows-1)*stepA] / (तथ्य_B[(rows-1)*stepB] + eps);
                अगर( !conjB )
                    क्रम( j = 1; j <= rows - 2; j += 2 )
                    {
                        द्विगुन denom = तथ्य_B[j*stepB]*तथ्य_B[j*stepB] +
                        तथ्य_B[(j+1)*stepB]*तथ्य_B[(j+1)*stepB] + eps;

                        द्विगुन re = तथ्य_A[j*stepA]*तथ्य_B[j*stepB] +
                        तथ्य_A[(j+1)*stepA]*तथ्य_B[(j+1)*stepB];

                        द्विगुन im = तथ्य_A[(j+1)*stepA]*तथ्य_B[j*stepB] -
                        तथ्य_A[j*stepA]*तथ्य_B[(j+1)*stepB];

                        तथ्य_C[j*stepC] = re / denom;
                        तथ्य_C[(j+1)*stepC] = im / denom;
                    }
                    अन्यथा
                        क्रम( j = 1; j <= rows - 2; j += 2 )
                        {
                            द्विगुन denom = तथ्य_B[j*stepB]*तथ्य_B[j*stepB] +
                            तथ्य_B[(j+1)*stepB]*तथ्य_B[(j+1)*stepB] + eps;

                            द्विगुन re = तथ्य_A[j*stepA]*तथ्य_B[j*stepB] -
                            तथ्य_A[(j+1)*stepA]*तथ्य_B[(j+1)*stepB];

                            द्विगुन im = तथ्य_A[(j+1)*stepA]*तथ्य_B[j*stepB] +
                            तथ्य_A[j*stepA]*तथ्य_B[(j+1)*stepB];

                            तथ्य_C[j*stepC] = re / denom;
                            तथ्य_C[(j+1)*stepC] = im / denom;
                        }
                    अगर( k == 1 )
                        तथ्य_A -= cols - 1, तथ्य_B -= cols - 1, तथ्य_C -= cols - 1;
            }
        }

        क्रम( ; rows--; तथ्य_A += stepA, तथ्य_B += stepB, तथ्य_C += stepC )
        {
            अगर( is_1d && cn == 1 )
            {
                तथ्य_C[0] = तथ्य_A[0] / (तथ्य_B[0] + eps);
                अगर( cols % 2 == 0 )
                    तथ्य_C[j1] = तथ्य_A[j1] / (तथ्य_B[j1] + eps);
            }

            अगर( !conjB )
                क्रम( j = j0; j < j1; j += 2 )
                {
                    द्विगुन denom = तथ्य_B[j]*तथ्य_B[j] + तथ्य_B[j+1]*तथ्य_B[j+1] + eps;
                    द्विगुन re = तथ्य_A[j]*तथ्य_B[j] + तथ्य_A[j+1]*तथ्य_B[j+1];
                    द्विगुन im = तथ्य_A[j+1]*तथ्य_B[j] - तथ्य_A[j]*तथ्य_B[j+1];
                    तथ्य_C[j] = re / denom;
                    तथ्य_C[j+1] = im / denom;
                }
                अन्यथा
                    क्रम( j = j0; j < j1; j += 2 )
                    {
                        द्विगुन denom = तथ्य_B[j]*तथ्य_B[j] + तथ्य_B[j+1]*तथ्य_B[j+1] + eps;
                        द्विगुन re = तथ्य_A[j]*तथ्य_B[j] - तथ्य_A[j+1]*तथ्य_B[j+1];
                        द्विगुन im = तथ्य_A[j+1]*तथ्य_B[j] + तथ्य_A[j]*तथ्य_B[j+1];
                        तथ्य_C[j] = re / denom;
                        तथ्य_C[j+1] = im / denom;
                    }
        }
    }
}

/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

namespace cv
{

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
// DCT-II / DCT-III through the new engine (plan kinds DFT_KIND_DCT / DFT_KIND_IDCT).
// src_step / dst_step are in bytes.
ढाँचा<typename T> अटल व्योम
DCT( स्थिर OcvDftOptions & c, स्थिर T* src, माप_प्रकार src_step, T* dst, माप_प्रकार dst_step )
{
    अगर( c.n == 1 )
    {
        dst[0] = src[0];
        वापस;
    }
    runDft(c, src, src_step/माप(T), dst, dst_step/माप(T), false);
}

ढाँचा<typename T> अटल व्योम
IDCT( स्थिर OcvDftOptions & c, स्थिर T* src, माप_प्रकार src_step, T* dst, माप_प्रकार dst_step )
{
    अगर( c.n == 1 )
    {
        dst[0] = src[0];
        वापस;
    }
    runDft(c, src, src_step/माप(T), dst, dst_step/माप(T), false);
}

प्रकार व्योम (*DCTFunc)(स्थिर OcvDftOptions & c, स्थिर व्योम* src, माप_प्रकार src_step,
                        व्योम* dst, माप_प्रकार dst_step);

ढाँचा<typename T, व्योम (*fn)(स्थिर OcvDftOptions&, स्थिर T*, माप_प्रकार, T*, माप_प्रकार)>
अटल व्योम dctWrap(स्थिर OcvDftOptions & c, स्थिर व्योम* src, माप_प्रकार src_step, व्योम* dst, माप_प्रकार dst_step)
{
    fn(c, (स्थिर T*)src, src_step, (T*)dst, dst_step);
}

}

#अगर_घोषित HAVE_IPP
namespace cv
{

#अगर IPP_VERSION_X100 >= 900
प्रकार IppStatus (CV_STDCALL * ippiDCTFunc)(स्थिर Ipp32f* pSrc, पूर्णांक srcStep, Ipp32f* pDst, पूर्णांक dstStep, स्थिर व्योम* pDCTSpec, Ipp8u* p_बफर);
प्रकार IppStatus (CV_STDCALL * ippiDCTInit)(व्योम* pDCTSpec, Ippi_आकार roi_आकार, Ipp8u* pMemInit );
प्रकार IppStatus (CV_STDCALL * ippi_DCTGet_आकार)(Ippi_आकार roi_आकार, पूर्णांक* p_आकार_Spec, पूर्णांक* p_आकार_Init, पूर्णांक* p_आकार_Buf);

ढाँचा<typename SpecType, IppStatus (CV_STDCALL *fn)(स्थिर Ipp32f*, पूर्णांक, Ipp32f*, पूर्णांक, स्थिर SpecType*, Ipp8u*)>
अटल IppStatus CV_STDCALL ippiDCTWrap(स्थिर Ipp32f* pSrc, पूर्णांक srcStep, Ipp32f* pDst, पूर्णांक dstStep, स्थिर व्योम* pDCTSpec, Ipp8u* p_बफर)
{
    वापस fn(pSrc, srcStep, pDst, dstStep, (स्थिर SpecType*)pDCTSpec, p_बफर);
}

ढाँचा<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*, Ippi_आकार, Ipp8u*)>
अटल IppStatus CV_STDCALL ippiDCTInitWrap(व्योम* pDCTSpec, Ippi_आकार roi_आकार, Ipp8u* pMemInit)
{
    वापस fn((SpecType*)pDCTSpec, roi_आकार, pMemInit);
}

ढाँचा<IppStatus (CV_STDCALL *fn)(Ippi_आकार, पूर्णांक*, पूर्णांक*, पूर्णांक*)>
अटल IppStatus CV_STDCALL ippi_DCTGet_आकार_Wrap(Ippi_आकार roi_आकार, पूर्णांक* p_आकार_Spec, पूर्णांक* p_आकार_Init, पूर्णांक* p_आकार_Buf)
{
    वापस fn(roi_आकार, p_आकार_Spec, p_आकार_Init, p_आकार_Buf);
}

#या_अगर IPP_VERSION_X100 >= 700
प्रकार IppStatus (CV_STDCALL * ippiDCTFunc)(स्थिर Ipp32f*, पूर्णांक, Ipp32f*, पूर्णांक, स्थिर व्योम*, Ipp8u*);
प्रकार IppStatus (CV_STDCALL * ippiDCTInitAlloc)(व्योम**, Ippi_आकार, IppHintAlgorithm);
प्रकार IppStatus (CV_STDCALL * ippiDCTFree)(व्योम* pDCTSpec);
प्रकार IppStatus (CV_STDCALL * ippi_DCTGet_Buf_आकार)(स्थिर व्योम*, पूर्णांक*);

ढाँचा<typename SpecType, IppStatus (CV_STDCALL *fn)(स्थिर Ipp32f*, पूर्णांक, Ipp32f*, पूर्णांक, स्थिर SpecType*, Ipp8u*)>
अटल IppStatus CV_STDCALL ippiDCTWrap(स्थिर Ipp32f* pSrc, पूर्णांक srcStep, Ipp32f* pDst, पूर्णांक dstStep, स्थिर व्योम* pDCTSpec, Ipp8u* p_बफर)
{
    वापस fn(pSrc, srcStep, pDst, dstStep, (स्थिर SpecType*)pDCTSpec, p_बफर);
}

ढाँचा<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType**, Ippi_आकार, IppHintAlgorithm)>
अटल IppStatus CV_STDCALL ippiDCTInitAllocWrap(व्योम** pDCTSpec, Ippi_आकार roi_आकार, IppHintAlgorithm hint)
{
    वापस fn((SpecType**)pDCTSpec, roi_आकार, hint);
}

ढाँचा<typename SpecType, IppStatus (CV_STDCALL *fn)(SpecType*)>
अटल IppStatus CV_STDCALL ippiDCTFreeWrap(व्योम* pDCTSpec)
{
    वापस fn((SpecType*)pDCTSpec);
}

ढाँचा<typename SpecType, IppStatus (CV_STDCALL *fn)(स्थिर SpecType*, पूर्णांक*)>
अटल IppStatus CV_STDCALL ippi_DCTGet_Buf_आकार_Wrap(स्थिर व्योम* pDCTSpec, पूर्णांक* p_आकार)
{
    वापस fn((स्थिर SpecType*)pDCTSpec, p_आकार);
}
#पूर्ण_अगर

श्रेणी DctIPPLoop_Invoker : खुला ParallelLoopBody
{
खुला:
    DctIPPLoop_Invoker(स्थिर uchar * _src, माप_प्रकार _src_step, uchar * _dst, माप_प्रकार _dst_step, पूर्णांक _width, bool _inv, bool *_ok) :
        ParallelLoopBody(), src(_src), src_step(_src_step), dst(_dst), dst_step(_dst_step), width(_width), inv(_inv), ok(_ok)
    {
        *ok = true;
    }

    भव व्योम चालक()(स्थिर Range& range) स्थिर CV_OVERRIDE
    {
        अगर(*ok == false)
            वापस;

#अगर IPP_VERSION_X100 >= 900
        Ippi_आकार src_Roi_आकार = {width, 1};

        पूर्णांक spec_आकार    = 0;
        पूर्णांक init_आकार    = 0;
        पूर्णांक बफर_आकार  = 0;

        Ipp8u* pDCTSpec = शून्य;
        Ipp8u* p_बफर  = शून्य;
        Ipp8u* pInitBuf = शून्य;

        #घोषणा IPP_RETURN              \
            अगर(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            अगर(p_बफर)                 \
                ippFree(p_बफर);       \
            अगर(pInitBuf)                \
                ippFree(pInitBuf);      \
            वापस;

        ippiDCTFunc     ippiDCT_32f_C1R   = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_आकार  ipp_Dct_Get_आकार  = inv ? ippi_DCTGet_आकार_Wrap<ippi_DCTInv_Get_आकार_32f> : ippi_DCTGet_आकार_Wrap<ippi_DCTFwd_Get_आकार_32f>;

        अगर(ipp_Dct_Get_आकार(src_Roi_आकार, &spec_आकार, &init_आकार, &बफर_आकार) < 0)
        {
            *ok = false;
            वापस;
        }

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_आकार);
        अगर(!pDCTSpec && spec_आकार)
        {
            *ok = false;
            वापस;
        }

        p_बफर  = (Ipp8u*)CV_IPP_MALLOC(बफर_आकार);
        अगर(!p_बफर && बफर_आकार)
        {
            *ok = false;
            IPP_RETURN
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_आकार);
        अगर(!pInitBuf && init_आकार)
        {
            *ok = false;
            IPP_RETURN
        }

        अगर(ippDctInit(pDCTSpec, src_Roi_आकार, pInitBuf) < 0)
        {
            *ok = false;
            IPP_RETURN
        }

        क्रम(पूर्णांक i = range.start; i < range.end; ++i)
        {
            अगर(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (भग्न*)(src + src_step * i), static_cast<पूर्णांक>(src_step), (भग्न*)(dst + dst_step * i), static_cast<पूर्णांक>(dst_step), pDCTSpec, p_बफर) < 0)
            {
                *ok = false;
                IPP_RETURN
            }
        }
        IPP_RETURN
#अघोषित IPP_RETURN
#या_अगर IPP_VERSION_X100 >= 700
        व्योम* pDCTSpec;
        Auto_बफर<uchar> buf;
        uchar* p_बफर = 0;
        पूर्णांक buf_आकार=0;

        Ippi_आकार src_Roi_आकार = {width, 1};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_आकार ipp_Get_Buf_आकार = inv ? ippi_DCTGet_Buf_आकार_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_आकार_32f> : ippi_DCTGet_Buf_आकार_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_आकार_32f>;

        अगर (ippInitAlloc(&pDCTSpec, src_Roi_आकार, ippAlgHintNone)>=0 && ipp_Get_Buf_आकार(pDCTSpec, &buf_आकार)>=0)
        {
            buf.allocate( buf_आकार );
            p_बफर = (uchar*)buf;

            क्रम( पूर्णांक i = range.start; i < range.end; ++i)
            {
                अगर(ippDctFun((भग्न*)(src + src_step * i), static_cast<पूर्णांक>(src_step), (भग्न*)(dst + dst_step * i), static_cast<पूर्णांक>(dst_step), pDCTSpec, (Ipp8u*)p_बफर) < 0)
                {
                    *ok = false;
                    अवरोध;
                }
            }
        }
        अन्यथा
            *ok = false;

        अगर (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END
#अन्यथा
        CV_UNUSED(range);
        *ok = false;
#पूर्ण_अगर
    }

निजी:
    स्थिर uchar * src;
    माप_प्रकार src_step;
    uchar * dst;
    माप_प्रकार dst_step;
    पूर्णांक width;
    bool inv;
    bool *ok;
};

अटल bool DctIPPLoop(स्थिर uchar * src, माप_प्रकार src_step, uchar * dst, माप_प्रकार dst_step, पूर्णांक width, पूर्णांक height, bool inv)
{
    bool ok;
    parallel_for_(Range(0, height), DctIPPLoop_Invoker(src, src_step, dst, dst_step, width, inv, &ok), height/(द्विगुन)(1<<4) );
    वापस ok;
}

अटल bool ippi_DCT_32f(स्थिर uchar * src, माप_प्रकार src_step, uchar * dst, माप_प्रकार dst_step, पूर्णांक width, पूर्णांक height, bool inv, bool row)
{
    CV_INSTRUMENT_REGION_IPP();

    अगर(row)
        वापस DctIPPLoop(src, src_step, dst, dst_step, width, height, inv);
    अन्यथा
    {
#अगर IPP_VERSION_X100 >= 900
        Ippi_आकार src_Roi_आकार = {width, height};

        पूर्णांक spec_आकार    = 0;
        पूर्णांक init_आकार    = 0;
        पूर्णांक बफर_आकार  = 0;

        Ipp8u* pDCTSpec = शून्य;
        Ipp8u* p_बफर  = शून्य;
        Ipp8u* pInitBuf = शून्य;

        #घोषणा IPP_RELEASE             \
            अगर(pDCTSpec)                \
                ippFree(pDCTSpec);      \
            अगर(p_बफर)                 \
                ippFree(p_बफर);       \
            अगर(pInitBuf)                \
                ippFree(pInitBuf);      \

        ippiDCTFunc     ippiDCT_32f_C1R      = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R> : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInit     ippDctInit     = inv ? ippiDCTInitWrap<IppiDCTInvSpec_32f, ippiDCTInvInit_32f> : ippiDCTInitWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInit_32f>;
        ippi_DCTGet_आकार  ipp_Dct_Get_आकार  = inv ? ippi_DCTGet_आकार_Wrap<ippi_DCTInv_Get_आकार_32f> : ippi_DCTGet_आकार_Wrap<ippi_DCTFwd_Get_आकार_32f>;

        अगर(ipp_Dct_Get_आकार(src_Roi_आकार, &spec_आकार, &init_आकार, &बफर_आकार) < 0)
            वापस false;

        pDCTSpec = (Ipp8u*)CV_IPP_MALLOC(spec_आकार);
        अगर(!pDCTSpec && spec_आकार)
            वापस false;

        p_बफर  = (Ipp8u*)CV_IPP_MALLOC(बफर_आकार);
        अगर(!p_बफर && बफर_आकार)
        {
            IPP_RELEASE
            वापस false;
        }
        pInitBuf = (Ipp8u*)CV_IPP_MALLOC(init_आकार);
        अगर(!pInitBuf && init_आकार)
        {
            IPP_RELEASE
            वापस false;
        }

        अगर(ippDctInit(pDCTSpec, src_Roi_आकार, pInitBuf) < 0)
        {
            IPP_RELEASE
            वापस false;
        }

        अगर(CV_INSTRUMENT_FUN_IPP(ippiDCT_32f_C1R, (भग्न*)src, static_cast<पूर्णांक>(src_step), (भग्न*)dst, static_cast<पूर्णांक>(dst_step), pDCTSpec, p_बफर) < 0)
        {
            IPP_RELEASE
            वापस false;
        }

        IPP_RELEASE
        वापस true;
#अघोषित IPP_RELEASE
#या_अगर IPP_VERSION_X100 >= 700
        IppStatus status;
        व्योम* pDCTSpec;
        Auto_बफर<uchar> buf;
        uchar* p_बफर = 0;
        पूर्णांक buf_आकार=0;

        Ippi_आकार src_Roi_आकार = {width, height};

        CV_SUPPRESS_DEPRECATED_START

        ippiDCTFunc ippDctFun           = inv ? ippiDCTWrap<IppiDCTInvSpec_32f, ippiDCTInv_32f_C1R>             : ippiDCTWrap<IppiDCTFwdSpec_32f, ippiDCTFwd_32f_C1R>;
        ippiDCTInitAlloc ippInitAlloc   = inv ? ippiDCTInitAllocWrap<IppiDCTInvSpec_32f, ippiDCTInvInitAlloc_32f>   : ippiDCTInitAllocWrap<IppiDCTFwdSpec_32f, ippiDCTFwdInitAlloc_32f>;
        ippiDCTFree ippFree             = inv ? ippiDCTFreeWrap<IppiDCTInvSpec_32f, ippiDCTInvFree_32f>             : ippiDCTFreeWrap<IppiDCTFwdSpec_32f, ippiDCTFwdFree_32f>;
        ippi_DCTGet_Buf_आकार ipp_Get_Buf_आकार = inv ? ippi_DCTGet_Buf_आकार_Wrap<IppiDCTInvSpec_32f, ippi_DCTInv_Get_Buf_आकार_32f> : ippi_DCTGet_Buf_आकार_Wrap<IppiDCTFwdSpec_32f, ippi_DCTFwd_Get_Buf_आकार_32f>;

        status = ippStsErr;

        अगर (ippInitAlloc(&pDCTSpec, src_Roi_आकार, ippAlgHintNone)>=0 && ipp_Get_Buf_आकार(pDCTSpec, &buf_आकार)>=0)
        {
            buf.allocate( buf_आकार );
            p_बफर = (uchar*)buf;

            status = ippDctFun((भग्न*)src, static_cast<पूर्णांक>(src_step), (भग्न*)dst, static_cast<पूर्णांक>(dst_step), pDCTSpec, (Ipp8u*)p_बफर);
        }

        अगर (pDCTSpec)
            ippFree(pDCTSpec);

        CV_SUPPRESS_DEPRECATED_END

        वापस status >= 0;
#अन्यथा
        CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(inv); CV_UNUSED(row);
        वापस false;
#पूर्ण_अगर
    }
}
}
#पूर्ण_अगर

namespace cv {

श्रेणी OcvDctImpl CV_FINAL : खुला hal::DCT2D
{
खुला:
    OcvDftOptions opt;
    DCTFunc dct_func;
    // per-stage plan + workspace, (re)built once per stage length, never per row
    DftPlan plan;
    Auto_बफर<uchar> खाली;
    bool isRowTransform;
    bool isInverse;
    bool isContinuous;
    पूर्णांक start_stage;
    पूर्णांक end_stage;
    पूर्णांक width;
    पूर्णांक height;
    पूर्णांक depth;

    व्योम init(पूर्णांक _width, पूर्णांक _height, पूर्णांक _depth, पूर्णांक flags)
    {
        width = _width;
        height = _height;
        depth = _depth;
        isInverse = (flags & CV_HAL_DFT_INVERSE) != 0;
        isRowTransform = (flags & CV_HAL_DFT_ROWS) != 0;
        isContinuous = (flags & CV_HAL_DFT_IS_CONTINUOUS) != 0;
        अटल DCTFunc dct_tbl[4] =
        {
            dctWrap<भग्न, DCT<भग्न>>,
            dctWrap<भग्न, IDCT<भग्न>>,
            dctWrap<द्विगुन, DCT<द्विगुन>>,
            dctWrap<द्विगुन, IDCT<द्विगुन>>
        };
        dct_func = dct_tbl[(पूर्णांक)isInverse + (depth == CV_64F)*2];
        opt.isComplex = false;
        opt.isInverse = false;
        opt.scale = 1.;

        अगर (isRowTransform || height == 1 || (width == 1 && isContinuous))
        {
            start_stage = end_stage = 0;
        }
        अन्यथा
        {
            start_stage = (width == 1);
            end_stage = 1;
        }
    }
    व्योम apply(स्थिर uchar *src, माप_प्रकार src_step, uchar *dst, माप_प्रकार dst_step) CV_OVERRIDE
    {
        CV_IPP_RUN(IPP_VERSION_X100 >= 700 && depth == CV_32F, ippi_DCT_32f(src, src_step, dst, dst_step, width, height, isInverse, isRowTransform))

        पूर्णांक पूर्व_len = 0;
        पूर्णांक elem_आकार_2 = (depth == CV_32F) ? माप(भग्न) : माप(द्विगुन);
        स्थिर DFTKernels& kernels = getDFTKernels(depth);

        क्रम(पूर्णांक stage = start_stage ; stage <= end_stage; stage++ )
        {
            स्थिर uchar* sptr = src;
            uchar* dptr = dst;
            माप_प्रकार sstep0, sstep1, dstep0, dstep1;
            पूर्णांक len, गिनती_2;

            अगर( stage == 0 )
            {
                len = width;
                गिनती_2 = height;
                अगर( len == 1 && !isRowTransform )
                {
                    len = height;
                    गिनती_2 = 1;
                }
                sstep0 = src_step;
                dstep0 = dst_step;
                sstep1 = dstep1 = elem_आकार_2;
            }
            अन्यथा
            {
                len = height;
                गिनती_2 = width;
                sstep1 = src_step;
                dstep1 = dst_step;
                sstep0 = dstep0 = elem_आकार_2;
            }

            opt.n = len;

            अगर( len != पूर्व_len )
            {
                अगर( len > 1 && (len & 1) )
                    CV_Error( cv::Error::StsNotImplemented, "Odd-size DCT\'s are not implemented" );
                अगर( len > 1 )
                {
                    plan.build(isInverse ? DFT_KIND_IDCT : DFT_KIND_DCT, len, depth, kernels.vlanes);
                    खाली.allocate(plan.ws_bytes);
                }
                opt.plan = &plan;
                opt.kernels = &kernels;
                opt.workspace = खाली.तथ्य();
                पूर्व_len = len;
            }
            // otherwise reuse the plan built on the previous stage (same length, only the steps differ)
            द्विगुन nstripes = dftParallelStripes(गिनती_2, len);
            अगर( len > 1 && गिनती_2 > 1 && nstripes >= 1.5 )
            {
                // the plan is shared, every worker gets its own workspace once per range
                स्वतः process_पंक्तियाँ = [&](स्थिर Range& range)
                {
                    OcvDftOptions opt_ = opt;
                    Auto_बफर<uchar> ws_(plan.ws_bytes);
                    opt_.workspace = ws_.तथ्य();
                    क्रम( पूर्णांक i = range.start; i < range.end; i++ )
                        dct_func( opt_, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
                };
                parallel_for_(Range(0, गिनती_2), process_पंक्तियाँ, nstripes);
            }
            अन्यथा
            {
                क्रम(अचिन्हित i = 0; i < static_cast<अचिन्हित>(गिनती_2); i++ )
                    dct_func( opt, sptr + i*sstep0, sstep1, dptr + i*dstep0, dstep1 );
            }
            src = dst;
            src_step = dst_step;
        }
    }
};

काष्ठा ReplacementDCT2D : खुला hal::DCT2D
{
    cvhalDFT *context;
    bool isInitialized;

    ReplacementDCT2D() : context(0), isInitialized(false) {}
    bool init(पूर्णांक width, पूर्णांक height, पूर्णांक depth, पूर्णांक flags)
    {
        पूर्णांक res = cv_hal_dctInit2D(&context, width, height, depth, flags);
        isInitialized = (res == CV_HAL_ERROR_OK);
        वापस isInitialized;
    }
    व्योम apply(स्थिर uchar *src_तथ्य, माप_प्रकार src_step, uchar *dst_तथ्य, माप_प्रकार dst_step) CV_OVERRIDE
    {
        अगर (isInitialized)
        {
            CALL_HAL(dct2D, cv_hal_dct2D, context, src_तथ्य, src_step, dst_तथ्य, dst_step);
        }
    }
    ~ReplacementDCT2D()
    {
        अगर (isInitialized)
        {
            CALL_HAL(dctFree2D, cv_hal_dctFree2D, context);
        }
    }
};

namespace hal {

Ptr<DCT2D> DCT2D::create(पूर्णांक width, पूर्णांक height, पूर्णांक depth, पूर्णांक flags)
{
    {
        ReplacementDCT2D *impl = नया ReplacementDCT2D();
        अगर (impl->init(width, height, depth, flags))
        {
            वापस Ptr<DCT2D>(impl);
        }
        मिटाओ impl;
    }
    {
        OcvDctImpl *impl = नया OcvDctImpl();
        impl->init(width, height, depth, flags);
        वापस Ptr<DCT2D>(impl);
    }
}

} // cv::hal::
} // cv::

व्योम cv::dct( निवेश_Array _src0, OutputArray _dst, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    Mat src0 = _src0.getMat(), src = src0;
    अगर (src.empty())
    {
        _dst.release();
        वापस;
    }
    पूर्णांक type = src.type(), depth = src.depth();

    CV_Assert( type == CV_32FC1 || type == CV_64FC1 );
    _dst.create( src.rows, src.cols, type );
    Mat dst = _dst.getMat();

    पूर्णांक f = 0;
    अगर ((flags & DFT_ROWS) != 0)
        f |= CV_HAL_DFT_ROWS;
    अगर ((flags & DCT_INVERSE) != 0)
        f |= CV_HAL_DFT_INVERSE;
    अगर (src.isContinuous() && dst.isContinuous())
        f |= CV_HAL_DFT_IS_CONTINUOUS;

    Ptr<hal::DCT2D> c = hal::DCT2D::create(src.cols, src.rows, depth, f);
    c->apply(src.तथ्य, src.step, dst.तथ्य, dst.step);
}


व्योम cv::idct( निवेश_Array src, OutputArray dst, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    dct( src, dst, flags | DCT_INVERSE );
}

namespace cv
{

अटल स्थिर पूर्णांक optimalDFTSizeTab[] = {
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

पूर्णांक cv::getOptimalDFTSize( पूर्णांक size0 )
{
    पूर्णांक a = 0, b = माप(optimalDFTSizeTab)/माप(optimalDFTSizeTab[0]) - 1;
    अगर( (अचिन्हित)size0 >= (अचिन्हित)optimalDFTSizeTab[b] )
        वापस -1;

    जबतक( a < b )
    {
        पूर्णांक c = (a + b) >> 1;
        अगर( size0 <= optimalDFTSizeTab[c] )
            b = c;
        अन्यथा
            a = c+1;
    }

    वापस optimalDFTSizeTab[b];
}
/* End of file. */
