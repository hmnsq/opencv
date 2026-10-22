/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
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
//   * The name of the copyright holders may not be used to endorse or promote products
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
#అన్తర్భావ <limits>

#నిర్వచితమైతే HAVE_EIGEN
#  ఒకవేళ defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  ఒకవేళ defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#అయితే_ముగింపు

నామాకాశ cv
{

పూర్ణాంకం LU(దశాంశం* A, size_t astep, పూర్ణాంకం m, దశాంశం* b, size_t bstep, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    తిరిగి hal::LU32f(A, astep, m, b, bstep, n);
}

పూర్ణాంకం LU(ద్విగుణ* A, size_t astep, పూర్ణాంకం m, ద్విగుణ* b, size_t bstep, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    తిరిగి hal::LU64f(A, astep, m, b, bstep, n);
}

తార్కిక Cholesky(దశాంశం* A, size_t astep, పూర్ణాంకం m, దశాంశం* b, size_t bstep, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    తిరిగి hal::Cholesky32f(A, astep, m, b, bstep, n);
}

తార్కిక Cholesky(ద్విగుణ* A, size_t astep, పూర్ణాంకం m, ద్విగుణ* b, size_t bstep, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    తిరిగి hal::Cholesky64f(A, astep, m, b, bstep, n);
}

మూస<రకం_పేరు _Tp> స్థిర అంతఃవరుస _Tp hypot(_Tp a, _Tp b)
{
    a = std::నిరపేక్ష(a);
    b = std::నిరపేక్ష(b);
    ఒకవేళ( a > b )
    {
        b /= a;
        తిరిగి a*std::వర్గమూలం(1 + b*b);
    }
    ఒకవేళ( b > 0 )
    {
        a /= b;
        తిరిగి b*std::వర్గమూలం(1 + a*a);
    }
    తిరిగి 0;
}


మూస<రకం_పేరు _Tp> తార్కిక
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, పూర్ణాంకం n, uchar* buf )
{
    స్థిరరాశి _Tp eps = std::numeric_limits<_Tp>::epsilon();
    పూర్ణాంకం i, j, k, m;

    astep /= పరిమాణం(A[0]);
    ఒకవేళ( V )
    {
        vstep /= పరిమాణం(V[0]);
        కొరకు( i = 0; i < n; i++ )
        {
            కొరకు( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    పూర్ణాంకం iters, maxIters = n*n*30;

    పూర్ణాంకం* indR = (పూర్ణాంకం*)alignPtr(buf, పరిమాణం(పూర్ణాంకం));
    పూర్ణాంకం* indC = indR + n;
    _Tp mv = (_Tp)0;

    కొరకు( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        ఒకవేళ( k < n - 1 )
        {
            కొరకు( m = k+1, mv = std::నిరపేక్ష(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::నిరపేక్ష(A[astep*k+i]);
                ఒకవేళ( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        ఒకవేళ( k > 0 )
        {
            కొరకు( m = 0, mv = std::నిరపేక్ష(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::నిరపేక్ష(A[astep*i+k]);
                ఒకవేళ( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    ఒకవేళ( n > 1 ) కొరకు( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        కొరకు( k = 0, mv = std::నిరపేక్ష(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::నిరపేక్ష(A[astep*i + indR[i]]);
            ఒకవేళ( mv < val )
                mv = val, k = i;
        }
        పూర్ణాంకం l = indR[k];
        కొరకు( i = 1; i < n; i++ )
        {
            _Tp val = std::నిరపేక్ష(A[astep*indC[i] + i]);
            ఒకవేళ( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        ఒకవేళ( std::నిరపేక్ష(p) <= eps )
            ఆపు;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::నిరపేక్ష(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        ఒకవేళ( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#నిర్వచనం_రద్దు rotate
#నిర్వచించు rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        కొరకు( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        కొరకు( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        కొరకు( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        ఒకవేళ( V )
            కొరకు( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#నిర్వచనం_రద్దు rotate

        కొరకు( j = 0; j < 2; j++ )
        {
            పూర్ణాంకం idx = j == 0 ? k : l;
            ఒకవేళ( idx < n - 1 )
            {
                కొరకు( m = idx+1, mv = std::నిరపేక్ష(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::నిరపేక్ష(A[astep*idx+i]);
                    ఒకవేళ( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            ఒకవేళ( idx > 0 )
            {
                కొరకు( m = 0, mv = std::నిరపేక్ష(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::నిరపేక్ష(A[astep*i+idx]);
                    ఒకవేళ( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    కొరకు( k = 0; k < n-1; k++ )
    {
        m = k;
        కొరకు( i = k+1; i < n; i++ )
        {
            ఒకవేళ( W[m] < W[i] )
                m = i;
        }
        ఒకవేళ( k != m )
        {
            std::swap(W[m], W[k]);
            ఒకవేళ( V )
                కొరకు( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    తిరిగి నిజం;
}

స్థిర తార్కిక Jacobi( దశాంశం* S, size_t sstep, దశాంశం* e, దశాంశం* E, size_t estep, పూర్ణాంకం n, uchar* buf )
{
    తిరిగి JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

స్థిర తార్కిక Jacobi( ద్విగుణ* S, size_t sstep, ద్విగుణ* e, ద్విగుణ* E, size_t estep, పూర్ణాంకం n, uchar* buf )
{
    తిరిగి JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


మూస<రకం_పేరు T> నిర్మాణం VBLAS
{
    పూర్ణాంకం dot(స్థిరరాశి T*, స్థిరరాశి T*, పూర్ణాంకం, T*) స్థిరరాశి { తిరిగి 0; }
    పూర్ణాంకం givens(T*, T*, పూర్ణాంకం, T, T) స్థిరరాశి { తిరిగి 0; }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
మూస<> అంతఃవరుస పూర్ణాంకం VBLAS<దశాంశం>::dot(స్థిరరాశి దశాంశం* a, స్థిరరాశి దశాంశం* b, పూర్ణాంకం n, దశాంశం* ఫలితం) స్థిరరాశి
{
    ఒకవేళ( n < 2*VTraits<v_float32>::vlanes() )
        తిరిగి 0;
    పూర్ణాంకం k = 0;
    v_float32 s0 = vx_setzero_f32();
    కొరకు( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_ఎక్కించు(a + k);
        v_float32 b0 = vx_ఎక్కించు(b + k);

        s0 = v_కూడు(s0, v_mul(a0, b0));
    }
    *ఫలితం = v_reduce_కూడిక(s0);
    vx_cleanup();
    తిరిగి k;
}


మూస<> అంతఃవరుస పూర్ణాంకం VBLAS<దశాంశం>::givens(దశాంశం* a, దశాంశం* b, పూర్ణాంకం n, దశాంశం c, దశాంశం s) స్థిరరాశి
{
    ఒకవేళ( n < VTraits<v_float32>::vlanes())
        తిరిగి 0;
    పూర్ణాంకం k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    కొరకు( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_ఎక్కించు(a + k);
        v_float32 b0 = vx_ఎక్కించు(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    తిరిగి k;
}


#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
మూస<> అంతఃవరుస పూర్ణాంకం VBLAS<ద్విగుణ>::dot(స్థిరరాశి ద్విగుణ* a, స్థిరరాశి ద్విగుణ* b, పూర్ణాంకం n, ద్విగుణ* ఫలితం) స్థిరరాశి
{
    ఒకవేళ( n < 2*VTraits<v_float64>::vlanes() )
        తిరిగి 0;
    పూర్ణాంకం k = 0;
    v_float64 s0 = vx_setzero_f64();
    కొరకు( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_ఎక్కించు(a + k);
        v_float64 b0 = vx_ఎక్కించు(b + k);

        s0 = v_కూడు(s0, v_mul(a0, b0));
    }
    *ఫలితం = v_reduce_కూడిక(s0);
    vx_cleanup();
    తిరిగి k;
}


మూస<> అంతఃవరుస పూర్ణాంకం VBLAS<ద్విగుణ>::givens(ద్విగుణ* a, ద్విగుణ* b, పూర్ణాంకం n, ద్విగుణ c, ద్విగుణ s) స్థిరరాశి
{
    పూర్ణాంకం k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    కొరకు( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_ఎక్కించు(a + k);
        v_float64 b0 = vx_ఎక్కించు(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    తిరిగి k;
}


#అయితే_ముగింపు // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#అయితే_ముగింపు // (CV_SIMD || CV_SIMD_SCALABLE)

మూస<రకం_పేరు _Tp> శూన్య
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం n1, ద్విగుణ minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    Auto_బఫరు<ద్విగుణ> Wbuf(n);
    ద్విగుణ* W = Wbuf.దత్తాంశం();
    పూర్ణాంకం i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    ద్విగుణ sd;
    astep /= పరిమాణం(At[0]);
    vstep /= పరిమాణం(Vt[0]);

    కొరకు( i = 0; i < n; i++ )
    {
        కొరకు( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ద్విగుణ)t*t;
        }
        W[i] = sd;

        ఒకవేళ( Vt )
        {
            కొరకు( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    కొరకు( iter = 0; iter < max_iter; iter++ )
    {
        తార్కిక మారింది = అబద్ధం;

        కొరకు( i = 0; i < n-1; i++ )
            కొరకు( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                ద్విగుణ a = W[i], p = 0, b = W[j];

                కొరకు( k = 0; k < m; k++ )
                    p += (ద్విగుణ)Ai[k]*Aj[k];

                ఒకవేళ( std::నిరపేక్ష(p) <= eps*std::వర్గమూలం((ద్విగుణ)a*b) )
                    కొనసాగు;

                p *= 2;
                ద్విగుణ beta = a - b, gamma = hypot((ద్విగుణ)p, beta);
                ఒకవేళ( beta < 0 )
                {
                    ద్విగుణ delta = (gamma - beta)*0.5;
                    s = (_Tp)std::వర్గమూలం(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                లేకపోతే
                {
                    c = (_Tp)std::వర్గమూలం((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                కొరకు( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (ద్విగుణ)t0*t0; b += (ద్విగుణ)t1*t1;
                }
                W[i] = a; W[j] = b;

                మారింది = నిజం;

                ఒకవేళ( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    కొరకు( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        ఒకవేళ( !మారింది )
            ఆపు;
    }

    కొరకు( i = 0; i < n; i++ )
    {
        కొరకు( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ద్విగుణ)t*t;
        }
        W[i] = std::వర్గమూలం(sd);
    }

    కొరకు( i = 0; i < n-1; i++ )
    {
        j = i;
        కొరకు( k = i+1; k < n; k++ )
        {
            ఒకవేళ( W[j] < W[k] )
                j = k;
        }
        ఒకవేళ( i != j )
        {
            std::swap(W[i], W[j]);
            ఒకవేళ( Vt )
            {
                కొరకు( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                కొరకు( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    కొరకు( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    ఒకవేళ( !Vt )
        తిరిగి;

    RNG rng(0x12345678);
    కొరకు( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        కొరకు( పూర్ణాంకం ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            స్థిరరాశి _Tp val0 = (_Tp)(1./m);
            కొరకు( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            కొరకు( iter = 0; iter < 2; iter++ )
            {
                కొరకు( j = 0; j < i; j++ )
                {
                    sd = 0;
                    కొరకు( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    కొరకు( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::నిరపేక్ష(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    కొరకు( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            కొరకు( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (ద్విగుణ)t*t;
            }
            sd = std::వర్గమూలం(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        కొరకు( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


స్థిర శూన్య JacobiSVD(దశాంశం* At, size_t astep, దశాంశం* W, దశాంశం* Vt, size_t vstep, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం n1=-1)
{
    hal::SVD32f(At, astep, W, ఖాళీ, astep, Vt, vstep, m, n, n1);
}

స్థిర శూన్య JacobiSVD(ద్విగుణ* At, size_t astep, ద్విగుణ* W, ద్విగుణ* Vt, size_t vstep, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం n1=-1)
{
    hal::SVD64f(At, astep, W, ఖాళీ, astep, Vt, vstep, m, n, n1);
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస పూర్ణాంకం
decodeSVDParameters(స్థిరరాశి fptype* U, స్థిరరాశి fptype* Vt, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం n1)
{
    పూర్ణాంకం halSVDFlag = 0;
    ఒకవేళ(Vt == ఖాళీ)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    లేకపోతే ఒకవేళ(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        ఒకవేళ(U == ఖాళీ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    లేకపోతే ఒకవేళ(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_నిండిన_UV;
        ఒకవేళ(U == ఖాళీ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    తిరిగి halSVDFlag;
}

శూన్య hal::SVD32f(దశాంశం* At, size_t astep, దశాంశం* W, దశాంశం* U, size_t ustep, దశాంశం* Vt, size_t vstep, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

శూన్య hal::SVD64f(ద్విగుణ* At, size_t astep, ద్విగుణ* W, ద్విగుణ* U, size_t ustep, ద్విగుణ* Vt, size_t vstep, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
మూస<రకం_పేరు T1, రకం_పేరు T2, రకం_పేరు T3> స్థిర శూన్య
MatrAXPY( పూర్ణాంకం m, పూర్ణాంకం n, స్థిరరాశి T1* x, పూర్ణాంకం dx,
         స్థిరరాశి T2* a, పూర్ణాంకం inca, T3* y, పూర్ణాంకం dy )
{
    పూర్ణాంకం i;
    కొరకు( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        పూర్ణాంకం j = 0;
         #ఒకవేళ CV_ENABLE_UNROLLED
        కొరకు(; j <= n - 4; j += 4 )
        {
            T3 t0 = (T3)(y[j]   + s*x[j]);
            T3 t1 = (T3)(y[j+1] + s*x[j+1]);
            y[j]   = t0;
            y[j+1] = t1;
            t0 = (T3)(y[j+2] + s*x[j+2]);
            t1 = (T3)(y[j+3] + s*x[j+3]);
            y[j+2] = t0;
            y[j+3] = t1;
        }
        #అయితే_ముగింపు
        కొరకు( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

మూస<రకం_పేరు T> స్థిర శూన్య
SVBkSbImpl_( పూర్ణాంకం m, పూర్ణాంకం n, స్థిరరాశి T* w, పూర్ణాంకం incw,
       స్థిరరాశి T* u, పూర్ణాంకం ldu, తార్కిక uT,
       స్థిరరాశి T* v, పూర్ణాంకం ldv, తార్కిక vT,
       స్థిరరాశి T* b, పూర్ణాంకం ldb, పూర్ణాంకం nb,
       T* x, పూర్ణాంకం ldx, ద్విగుణ* బఫరు, T eps )
{
    ద్విగుణ పరిమితి = 0;
    పూర్ణాంకం udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    పూర్ణాంకం vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    పూర్ణాంకం i, j, nm = std::min(m, n);

    ఒకవేళ( !b )
        nb = m;

    కొరకు( i = 0; i < n; i++ )
        కొరకు( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    కొరకు( i = 0; i < nm; i++ )
        పరిమితి += w[i*incw];
    పరిమితి *= eps;

    // v * inv(w) * uT * b
    కొరకు( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        ద్విగుణ wi = w[i*incw];
        ఒకవేళ( (ద్విగుణ)std::నిరపేక్ష(wi) <= పరిమితి )
            కొనసాగు;
        wi = 1/wi;

        ఒకవేళ( nb == 1 )
        {
            ద్విగుణ s = 0;
            ఒకవేళ( b )
                కొరకు( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            లేకపోతే
                s = u[0];
            s *= wi;

            కొరకు( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        లేకపోతే
        {
            ఒకవేళ( b )
            {
                కొరకు( j = 0; j < nb; j++ )
                    బఫరు[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, బఫరు, 0 );
                కొరకు( j = 0; j < nb; j++ )
                    బఫరు[j] *= wi;
            }
            లేకపోతే
            {
                కొరకు( j = 0; j < nb; j++ )
                    బఫరు[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, బఫరు, 0, v, vdelta1, x, ldx );
        }
    }
}

స్థిర శూన్య
SVBkSb( పూర్ణాంకం m, పూర్ణాంకం n, స్థిరరాశి దశాంశం* w, size_t wstep,
        స్థిరరాశి దశాంశం* u, size_t ustep, తార్కిక uT,
        స్థిరరాశి దశాంశం* v, size_t vstep, తార్కిక vT,
        స్థిరరాశి దశాంశం* b, size_t bstep, పూర్ణాంకం nb,
        దశాంశం* x, size_t xstep, uchar* బఫరు )
{
    SVBkSbImpl_(m, n, w, wstep ? (పూర్ణాంకం)(wstep/పరిమాణం(w[0])) : 1,
                u, (పూర్ణాంకం)(ustep/పరిమాణం(u[0])), uT,
                v, (పూర్ణాంకం)(vstep/పరిమాణం(v[0])), vT,
                b, (పూర్ణాంకం)(bstep/పరిమాణం(b[0])), nb,
                x, (పూర్ణాంకం)(xstep/పరిమాణం(x[0])),
                (ద్విగుణ*)alignPtr(బఫరు, పరిమాణం(ద్విగుణ)), (దశాంశం)(DBL_EPSILON*2) );
}

స్థిర శూన్య
SVBkSb( పూర్ణాంకం m, పూర్ణాంకం n, స్థిరరాశి ద్విగుణ* w, size_t wstep,
       స్థిరరాశి ద్విగుణ* u, size_t ustep, తార్కిక uT,
       స్థిరరాశి ద్విగుణ* v, size_t vstep, తార్కిక vT,
       స్థిరరాశి ద్విగుణ* b, size_t bstep, పూర్ణాంకం nb,
       ద్విగుణ* x, size_t xstep, uchar* బఫరు )
{
    SVBkSbImpl_(m, n, w, wstep ? (పూర్ణాంకం)(wstep/పరిమాణం(w[0])) : 1,
                u, (పూర్ణాంకం)(ustep/పరిమాణం(u[0])), uT,
                v, (పూర్ణాంకం)(vstep/పరిమాణం(v[0])), vT,
                b, (పూర్ణాంకం)(bstep/పరిమాణం(b[0])), nb,
                x, (పూర్ణాంకం)(xstep/పరిమాణం(x[0])),
                (ద్విగుణ*)alignPtr(బఫరు, పరిమాణం(ద్విగుణ)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#నిర్వచించు det2(m)   ((ద్విగుణ)m(0,0)*m(1,1) - (ద్విగుణ)m(0,1)*m(1,0))
#నిర్వచించు det3(m)   (m(0,0)*((ద్విగుణ)m(1,1)*m(2,2) - (ద్విగుణ)m(1,2)*m(2,1)) -  \
                   m(0,1)*((ద్విగుణ)m(1,0)*m(2,2) - (ద్విగుణ)m(1,2)*m(2,0)) +  \
                   m(0,2)*((ద్విగుణ)m(1,0)*m(2,1) - (ద్విగుణ)m(1,1)*m(2,0)))

ద్విగుణ నిర్ధారకం( ఆదానం_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    ద్విగుణ ఫలితం = 0;
    పూర్ణాంకం type = mat.type(), అడ్డువరుసలు = mat.అడ్డువరుసలు;
    size_t అడుగు = mat.అడుగు;
    స్థిరరాశి uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.అడ్డువరుసలు == mat.నిలువుల_సంఖ్య && (type == CV_32F || type == CV_64F));

    #నిర్వచించు Mf(y, x) ((దశాంశం*)(m + y*అడుగు))[x]
    #నిర్వచించు Md(y, x) ((ద్విగుణ*)(m + y*అడుగు))[x]

    ఒకవేళ( type == CV_32F )
    {
        ఒకవేళ( అడ్డువరుసలు == 2 )
            ఫలితం = det2(Mf);
        లేకపోతే ఒకవేళ( అడ్డువరుసలు == 3 )
            ఫలితం = det3(Mf);
        లేకపోతే ఒకవేళ( అడ్డువరుసలు == 1 )
            ఫలితం = Mf(0,0);
        లేకపోతే
        {
            size_t buf_కొలత = అడ్డువరుసలు*అడ్డువరుసలు*పరిమాణం(దశాంశం);
            Auto_బఫరు<uchar> బఫరు(buf_కొలత);
            Mat a(అడ్డువరుసలు, అడ్డువరుసలు, CV_32F, బఫరు.దత్తాంశం());
            mat.copy_గమ్యం(a);

            ఫలితం = hal::LU32f(a.ptr<దశాంశం>(), a.అడుగు, అడ్డువరుసలు, 0, 0, 0);
            ఒకవేళ( ఫలితం )
            {
                కొరకు( పూర్ణాంకం i = 0; i < అడ్డువరుసలు; i++ )
                    ఫలితం *= a.at<దశాంశం>(i,i);
            }
        }
    }
    లేకపోతే
    {
        ఒకవేళ( అడ్డువరుసలు == 2 )
            ఫలితం = det2(Md);
        లేకపోతే ఒకవేళ( అడ్డువరుసలు == 3 )
            ఫలితం = det3(Md);
        లేకపోతే ఒకవేళ( అడ్డువరుసలు == 1 )
            ఫలితం = Md(0,0);
        లేకపోతే
        {
            size_t buf_కొలత = అడ్డువరుసలు*అడ్డువరుసలు*పరిమాణం(ద్విగుణ);
            Auto_బఫరు<uchar> బఫరు(buf_కొలత);
            Mat a(అడ్డువరుసలు, అడ్డువరుసలు, CV_64F, బఫరు.దత్తాంశం());
            mat.copy_గమ్యం(a);

            ఫలితం = hal::LU64f(a.ptr<ద్విగుణ>(), a.అడుగు, అడ్డువరుసలు, 0, 0, 0);
            ఒకవేళ( ఫలితం )
            {
                కొరకు( పూర్ణాంకం i = 0; i < అడ్డువరుసలు; i++ )
                    ఫలితం *= a.at<ద్విగుణ>(i,i);
            }
        }
    }

    #నిర్వచనం_రద్దు Mf
    #నిర్వచనం_రద్దు Md

    తిరిగి ఫలితం;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#నిర్వచించు Sf( y, x ) ((దశాంశం*)(srcdata + y*srcstep))[x]
#నిర్వచించు Sd( y, x ) ((ద్విగుణ*)(srcdata + y*srcstep))[x]
#నిర్వచించు Df( y, x ) ((దశాంశం*)(dstdata + y*dststep))[x]
#నిర్వచించు Dd( y, x ) ((ద్విగుణ*)(dstdata + y*dststep))[x]

ద్విగుణ invert( ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం పద్ధతి )
{
    CV_INSTRUMENT_REGION();

    తార్కిక ఫలితం = అబద్ధం;
    Mat src = _src.getMat();
    పూర్ణాంకం type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_కొలత(type);
    పూర్ణాంకం m = src.అడ్డువరుసలు, n = src.నిలువుల_సంఖ్య;

    ఒకవేళ( పద్ధతి == DECOMP_SVD )
    {
        పూర్ణాంకం nm = std::min(m, n);

        Auto_బఫరు<uchar> _buf((m*nm + nm + nm*n)*esz + పరిమాణం(ద్విగుణ));
        uchar* buf = alignPtr((uchar*)_buf.దత్తాంశం(), (పూర్ణాంకం)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::వెనుక_Subst(w, u, vt, Mat(), _dst);
        తిరిగి type == CV_32F ?
            (w.ptr<దశాంశం>()[0] >= FLT_EPSILON ?
             w.ptr<దశాంశం>()[n-1]/w.ptr<దశాంశం>()[0] : 0) :
            (w.ptr<ద్విగుణ>()[0] >= DBL_EPSILON ?
             w.ptr<ద్విగుణ>()[n-1]/w.ptr<ద్విగుణ>()[0] : 0);
    }

    CV_Assert( m == n );

    ఒకవేళ( పద్ధతి == DECOMP_EIG )
    {
        Auto_బఫరు<uchar> _buf((n*n*2 + n)*esz + పరిమాణం(ద్విగుణ));
        uchar* buf = alignPtr((uchar*)_buf.దత్తాంశం(), (పూర్ణాంకం)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::వెనుక_Subst(w, u, vt, Mat(), _dst);
        తిరిగి type == CV_32F ?
        (w.ptr<దశాంశం>()[0] >= FLT_EPSILON ?
         w.ptr<దశాంశం>()[n-1]/w.ptr<దశాంశం>()[0] : 0) :
        (w.ptr<ద్విగుణ>()[0] >= DBL_EPSILON ?
         w.ptr<ద్విగుణ>()[n-1]/w.ptr<ద్విగుణ>()[0] : 0);
    }

    CV_Assert( పద్ధతి == DECOMP_LU || పద్ధతి == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    ఒకవేళ( n <= 3 )
    {
        స్థిరరాశి uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.అడుగు;
        size_t dststep = dst.అడుగు;

        ఒకవేళ( n == 2 )
        {
            ఒకవేళ( type == CV_32FC1 )
            {
                ద్విగుణ d = det2(Sf);
                ఒకవేళ( d != 0. )
                {
                    ఫలితం = నిజం;
                    d = 1./d;
                #ఒకవేళ CV_SIMD128
                    స్థిరరాశి దశాంశం d_32f = (దశాంశం)d;
                    స్థిరరాశి v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_ఎక్కించు_halves((స్థిరరాశి దశాంశం *)srcdata, (స్థిరరాశి దశాంశం *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_వెలికితీయి<3>(s0, v_combine_దిగువ(v_rotate_కుడి<1>(s0), s0));
                    v_store_దిగువ((దశాంశం*)dstdata, s0);
                    v_store_ఎగువ((దశాంశం*)(dstdata + dststep), s0);
                #లేకపోతే
                    ద్విగుణ t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (దశాంశం)t0;
                    Df(0,0) = (దశాంశం)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (దశాంశం)t0;
                    Df(1,0) = (దశాంశం)t1;
                #అయితే_ముగింపు
                }
            }
            లేకపోతే
            {
                ద్విగుణ d = det2(Sd);
                ఒకవేళ( d != 0. )
                {
                    ఫలితం = నిజం;
                    d = 1./d;
                #ఒకవేళ CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_ఎక్కించు((స్థిరరాశి ద్విగుణ *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_ఎక్కించు((స్థిరరాశి ద్విగుణ *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_వెలికితీయి<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<ద్విగుణ>(0), v_వెలికితీయి<1>(s0, s1));//12
                    v_store((ద్విగుణ*)dstdata, v_combine_దిగువ(sm, ss));//31
                    v_store((ద్విగుణ*)(dstdata + dststep), v_combine_ఎగువ(ss, sm));//20
                #లేకపోతే
                    ద్విగుణ t0, t1;
                    t0 = Sd(0,0)*d;
                    t1 = Sd(1,1)*d;
                    Dd(1,1) = t0;
                    Dd(0,0) = t1;
                    t0 = -Sd(0,1)*d;
                    t1 = -Sd(1,0)*d;
                    Dd(0,1) = t0;
                    Dd(1,0) = t1;
                #అయితే_ముగింపు
                }
            }
        }
        లేకపోతే ఒకవేళ( n == 3 )
        {
            ఒకవేళ( type == CV_32FC1 )
            {
                ద్విగుణ d = det3(Sf);

                ఒకవేళ( d != 0. )
                {
                    ద్విగుణ t[12];

                    ఫలితం = నిజం;
                    d = 1./d;
                    t[0] = (((ద్విగుణ)Sf(1,1) * Sf(2,2) - (ద్విగుణ)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((ద్విగుణ)Sf(0,2) * Sf(2,1) - (ద్విగుణ)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((ద్విగుణ)Sf(0,1) * Sf(1,2) - (ద్విగుణ)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((ద్విగుణ)Sf(1,2) * Sf(2,0) - (ద్విగుణ)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((ద్విగుణ)Sf(0,0) * Sf(2,2) - (ద్విగుణ)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((ద్విగుణ)Sf(0,2) * Sf(1,0) - (ద్విగుణ)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((ద్విగుణ)Sf(1,0) * Sf(2,1) - (ద్విగుణ)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((ద్విగుణ)Sf(0,1) * Sf(2,0) - (ద్విగుణ)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((ద్విగుణ)Sf(0,0) * Sf(1,1) - (ద్విగుణ)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (దశాంశం)t[0]; Df(0,1) = (దశాంశం)t[1]; Df(0,2) = (దశాంశం)t[2];
                    Df(1,0) = (దశాంశం)t[3]; Df(1,1) = (దశాంశం)t[4]; Df(1,2) = (దశాంశం)t[5];
                    Df(2,0) = (దశాంశం)t[6]; Df(2,1) = (దశాంశం)t[7]; Df(2,2) = (దశాంశం)t[8];
                }
            }
            లేకపోతే
            {
                ద్విగుణ d = det3(Sd);
                ఒకవేళ( d != 0. )
                {
                    ఫలితం = నిజం;
                    d = 1./d;
                    ద్విగుణ t[9];

                    t[0] = (Sd(1,1) * Sd(2,2) - Sd(1,2) * Sd(2,1)) * d;
                    t[1] = (Sd(0,2) * Sd(2,1) - Sd(0,1) * Sd(2,2)) * d;
                    t[2] = (Sd(0,1) * Sd(1,2) - Sd(0,2) * Sd(1,1)) * d;

                    t[3] = (Sd(1,2) * Sd(2,0) - Sd(1,0) * Sd(2,2)) * d;
                    t[4] = (Sd(0,0) * Sd(2,2) - Sd(0,2) * Sd(2,0)) * d;
                    t[5] = (Sd(0,2) * Sd(1,0) - Sd(0,0) * Sd(1,2)) * d;

                    t[6] = (Sd(1,0) * Sd(2,1) - Sd(1,1) * Sd(2,0)) * d;
                    t[7] = (Sd(0,1) * Sd(2,0) - Sd(0,0) * Sd(2,1)) * d;
                    t[8] = (Sd(0,0) * Sd(1,1) - Sd(0,1) * Sd(1,0)) * d;

                    Dd(0,0) = t[0]; Dd(0,1) = t[1]; Dd(0,2) = t[2];
                    Dd(1,0) = t[3]; Dd(1,1) = t[4]; Dd(1,2) = t[5];
                    Dd(2,0) = t[6]; Dd(2,1) = t[7]; Dd(2,2) = t[8];
                }
            }
        }
        లేకపోతే
        {
            CV_Assert( n == 1 );

            ఒకవేళ( type == CV_32FC1 )
            {
                ద్విగుణ d = Sf(0,0);
                ఒకవేళ( d != 0. )
                {
                    ఫలితం = నిజం;
                    Df(0,0) = (దశాంశం)(1./d);
                }
            }
            లేకపోతే
            {
                ద్విగుణ d = Sd(0,0);
                ఒకవేళ( d != 0. )
                {
                    ఫలితం = నిజం;
                    Dd(0,0) = 1./d;
                }
            }
        }
        ఒకవేళ( !ఫలితం )
            dst = Scalar(0);
        తిరిగి ఫలితం;
    }

   పూర్ణాంకం elem_కొలత_2 = CV_ELEM_కొలత(type);
    Auto_బఫరు<uchar> buf(n*n*elem_కొలత_2);
    Mat src1(n, n, type, buf.దత్తాంశం());
    src.copy_గమ్యం(src1);
    సమితి_తటస్థం(dst);

    ఒకవేళ( పద్ధతి == DECOMP_LU && type == CV_32F )
        ఫలితం = hal::LU32f(src1.ptr<దశాంశం>(), src1.అడుగు, n, dst.ptr<దశాంశం>(), dst.అడుగు, n) != 0;
    లేకపోతే ఒకవేళ( పద్ధతి == DECOMP_LU && type == CV_64F )
        ఫలితం = hal::LU64f(src1.ptr<ద్విగుణ>(), src1.అడుగు, n, dst.ptr<ద్విగుణ>(), dst.అడుగు, n) != 0;
    లేకపోతే ఒకవేళ( పద్ధతి == DECOMP_CHOLESKY && type == CV_32F )
        ఫలితం = hal::Cholesky32f(src1.ptr<దశాంశం>(), src1.అడుగు, n, dst.ptr<దశాంశం>(), dst.అడుగు, n);
    లేకపోతే
        ఫలితం = hal::Cholesky64f(src1.ptr<ద్విగుణ>(), src1.అడుగు, n, dst.ptr<ద్విగుణ>(), dst.అడుగు, n);

    ఒకవేళ( !ఫలితం )
        dst = Scalar(0);

    తిరిగి ఫలితం;
}

UMat UMat::inv(పూర్ణాంకం పద్ధతి) స్థిరరాశి
{
    UMat m;
    invert(*ఇది, m, పద్ధతి);
    తిరిగి m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

తార్కిక solve( ఆదానం_Array _src, ఆదానం_Array _src2arg, OutputArray _dst, పూర్ణాంకం పద్ధతి )
{
    CV_INSTRUMENT_REGION();

    తార్కిక ఫలితం = నిజం;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    పూర్ణాంకం type = src.type();
    తార్కిక is_సాధారణ = (పద్ధతి & DECOMP_సాధారణ) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    పద్ధతి &= ~DECOMP_సాధారణ;
    CV_Check(పద్ధతి, పద్ధతి == DECOMP_LU || పద్ధతి == DECOMP_SVD || పద్ధతి == DECOMP_EIG ||
                     పద్ధతి == DECOMP_CHOLESKY || పద్ధతి == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (పద్ధతి != DECOMP_LU && పద్ధతి != DECOMP_CHOLESKY) ||
        is_సాధారణ || src.అడ్డువరుసలు == src.నిలువుల_సంఖ్య );

    // check case of a single equation and small matrix
    ఒకవేళ( (పద్ధతి == DECOMP_LU || పద్ధతి == DECOMP_CHOLESKY) && !is_సాధారణ &&
        src.అడ్డువరుసలు <= 3 && src.అడ్డువరుసలు == src.నిలువుల_సంఖ్య && _src2.నిలువుల_సంఖ్య == 1 )
    {
        _dst.create( src.నిలువుల_సంఖ్య, _src2.నిలువుల_సంఖ్య, src.type() );
        Mat dst = _dst.getMat();

        #నిర్వచించు bf(y) ((దశాంశం*)(bdata + y*src2step))[0]
        #నిర్వచించు bd(y) ((ద్విగుణ*)(bdata + y*src2step))[0]

        స్థిరరాశి uchar* srcdata = src.ptr();
        స్థిరరాశి uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.అడుగు;
        size_t src2step = _src2.అడుగు;
        size_t dststep = dst.అడుగు;

        ఒకవేళ( src.అడ్డువరుసలు == 2 )
        {
            ఒకవేళ( type == CV_32FC1 )
            {
                ద్విగుణ d = det2(Sf);
                ఒకవేళ( d != 0. )
                {
                    ద్విగుణ t;
                    d = 1./d;
                    t = (దశాంశం)(((ద్విగుణ)bf(0)*Sf(1,1) - (ద్విగుణ)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (దశాంశం)(((ద్విగుణ)bf(1)*Sf(0,0) - (ద్విగుణ)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (దశాంశం)t;
                }
                లేకపోతే
                    ఫలితం = అబద్ధం;
            }
            లేకపోతే
            {
                ద్విగుణ d = det2(Sd);
                ఒకవేళ( d != 0. )
                {
                    ద్విగుణ t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                లేకపోతే
                    ఫలితం = అబద్ధం;
            }
        }
        లేకపోతే ఒకవేళ( src.అడ్డువరుసలు == 3 )
        {
            ఒకవేళ( type == CV_32FC1 )
            {
                ద్విగుణ d = det3(Sf);
                ఒకవేళ( d != 0. )
                {
                    దశాంశం t[3];
                    d = 1./d;

                    t[0] = (దశాంశం)(d*
                           (bf(0)*((ద్విగుణ)Sf(1,1)*Sf(2,2) - (ద్విగుణ)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((ద్విగుణ)bf(1)*Sf(2,2) - (ద్విగుణ)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((ద్విగుణ)bf(1)*Sf(2,1) - (ద్విగుణ)Sf(1,1)*bf(2))));

                    t[1] = (దశాంశం)(d*
                           (Sf(0,0)*(ద్విగుణ)(bf(1)*Sf(2,2) - (ద్విగుణ)Sf(1,2)*bf(2)) -
                            bf(0)*((ద్విగుణ)Sf(1,0)*Sf(2,2) - (ద్విగుణ)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((ద్విగుణ)Sf(1,0)*bf(2) - (ద్విగుణ)bf(1)*Sf(2,0))));

                    t[2] = (దశాంశం)(d*
                           (Sf(0,0)*((ద్విగుణ)Sf(1,1)*bf(2) - (ద్విగుణ)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((ద్విగుణ)Sf(1,0)*bf(2) - (ద్విగుణ)bf(1)*Sf(2,0)) +
                            bf(0)*((ద్విగుణ)Sf(1,0)*Sf(2,1) - (ద్విగుణ)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                లేకపోతే
                    ఫలితం = అబద్ధం;
            }
            లేకపోతే
            {
                ద్విగుణ d = det3(Sd);
                ఒకవేళ( d != 0. )
                {
                    ద్విగుణ t[9];

                    d = 1./d;

                    t[0] = ((Sd(1,1) * Sd(2,2) - Sd(1,2) * Sd(2,1))*bd(0) +
                            (Sd(0,2) * Sd(2,1) - Sd(0,1) * Sd(2,2))*bd(1) +
                            (Sd(0,1) * Sd(1,2) - Sd(0,2) * Sd(1,1))*bd(2))*d;

                    t[1] = ((Sd(1,2) * Sd(2,0) - Sd(1,0) * Sd(2,2))*bd(0) +
                            (Sd(0,0) * Sd(2,2) - Sd(0,2) * Sd(2,0))*bd(1) +
                            (Sd(0,2) * Sd(1,0) - Sd(0,0) * Sd(1,2))*bd(2))*d;

                    t[2] = ((Sd(1,0) * Sd(2,1) - Sd(1,1) * Sd(2,0))*bd(0) +
                            (Sd(0,1) * Sd(2,0) - Sd(0,0) * Sd(2,1))*bd(1) +
                            (Sd(0,0) * Sd(1,1) - Sd(0,1) * Sd(1,0))*bd(2))*d;

                    Dd(0,0) = t[0];
                    Dd(1,0) = t[1];
                    Dd(2,0) = t[2];
                }
                లేకపోతే
                    ఫలితం = అబద్ధం;
            }
        }
        లేకపోతే
        {
            CV_Assert( src.అడ్డువరుసలు == 1 );

            ఒకవేళ( type == CV_32FC1 )
            {
                ద్విగుణ d = Sf(0,0);
                ఒకవేళ( d != 0. )
                    Df(0,0) = (దశాంశం)(bf(0)/d);
                లేకపోతే
                    ఫలితం = అబద్ధం;
            }
            లేకపోతే
            {
                ద్విగుణ d = Sd(0,0);
                ఒకవేళ( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                లేకపోతే
                    ఫలితం = అబద్ధం;
            }
        }
        తిరిగి ఫలితం;
    }

    పూర్ణాంకం m = src.అడ్డువరుసలు, m_ = m, n = src.నిలువుల_సంఖ్య, nb = _src2.నిలువుల_సంఖ్య;
    size_t esz = CV_ELEM_కొలత(type), bufsize = 0;
    size_t vstep = align_కొలత(n*esz, 16);
    size_t astep = పద్ధతి == DECOMP_SVD && !is_సాధారణ ? align_కొలత(m*esz, 16) : vstep;
    Auto_బఫరు<uchar> బఫరు;

    Mat src2 = _src2;
    _dst.create( src.నిలువుల_సంఖ్య, src2.నిలువుల_సంఖ్య, src.type() );
    Mat dst = _dst.getMat();

    ఒకవేళ( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    ఒకవేళ( m == n )
        is_సాధారణ = అబద్ధం;
    లేకపోతే ఒకవేళ( is_సాధారణ )
    {
        m_ = n;
        ఒకవేళ( పద్ధతి == DECOMP_SVD )
            పద్ధతి = DECOMP_EIG;
    }

    CV_Assert(m == src2.అడ్డువరుసలు);
    size_t asize = astep*(పద్ధతి == DECOMP_SVD || is_సాధారణ ? n : m);
    bufsize += asize + 32;

    ఒకవేళ( is_సాధారణ )
        bufsize += n*nb*esz;
    ఒకవేళ( పద్ధతి == DECOMP_SVD || పద్ధతి == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*పరిమాణం(ద్విగుణ) + 32;

    బఫరు.allocate(bufsize);
    uchar* ptr = alignPtr(బఫరు.దత్తాంశం(), 16);

    Mat a(m_, n, type, ptr, astep);

    ఒకవేళ( is_సాధారణ )
        mulTransposed(src, a, నిజం);
    లేకపోతే ఒకవేళ( పద్ధతి != DECOMP_SVD )
        src.copy_గమ్యం(a);
    లేకపోతే
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    ఒకవేళ( !is_సాధారణ )
    {
        ఒకవేళ( పద్ధతి == DECOMP_LU || పద్ధతి == DECOMP_CHOLESKY )
            src2.copy_గమ్యం(dst);
    }
    లేకపోతే
    {
        // a'*b
        ఒకవేళ( పద్ధతి == DECOMP_LU || పద్ధతి == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        లేకపోతే
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    ఒకవేళ( పద్ధతి == DECOMP_LU )
    {
        ఒకవేళ( type == CV_32F )
            ఫలితం = hal::LU32f(a.ptr<దశాంశం>(), a.అడుగు, n, dst.ptr<దశాంశం>(), dst.అడుగు, nb) != 0;
        లేకపోతే
            ఫలితం = hal::LU64f(a.ptr<ద్విగుణ>(), a.అడుగు, n, dst.ptr<ద్విగుణ>(), dst.అడుగు, nb) != 0;
    }
    లేకపోతే ఒకవేళ( పద్ధతి == DECOMP_CHOLESKY )
    {
        ఒకవేళ( type == CV_32F )
            ఫలితం = hal::Cholesky32f(a.ptr<దశాంశం>(), a.అడుగు, n, dst.ptr<దశాంశం>(), dst.అడుగు, nb);
        లేకపోతే
            ఫలితం = hal::Cholesky64f(a.ptr<ద్విగుణ>(), a.అడుగు, n, dst.ptr<ద్విగుణ>(), dst.అడుగు, nb);
    }
    లేకపోతే ఒకవేళ( పద్ధతి == DECOMP_QR )
    {
        Mat rhsMat;
        ఒకవేళ( is_సాధారణ || m == n )
        {
            src2.copy_గమ్యం(dst);
            rhsMat = dst;
        }
        లేకపోతే
        {
            rhsMat = Mat(m, nb, type);
            src2.copy_గమ్యం(rhsMat);
        }

        ఒకవేళ( type == CV_32F )
            ఫలితం = hal::QR32f(a.ptr<దశాంశం>(), a.అడుగు, a.అడ్డువరుసలు, a.నిలువుల_సంఖ్య, rhsMat.నిలువుల_సంఖ్య, rhsMat.ptr<దశాంశం>(), rhsMat.అడుగు, ఖాళీ) != 0;
        లేకపోతే
            ఫలితం = hal::QR64f(a.ptr<ద్విగుణ>(), a.అడుగు, a.అడ్డువరుసలు, a.నిలువుల_సంఖ్య, rhsMat.నిలువుల_సంఖ్య, rhsMat.ptr<ద్విగుణ>(), rhsMat.అడుగు, ఖాళీ) != 0;

        ఒకవేళ (rhsMat.అడ్డువరుసలు != dst.అడ్డువరుసలు)
            rhsMat.అడ్డువరుస_Range_2(0, dst.అడ్డువరుసలు).copy_గమ్యం(dst);
    }
    లేకపోతే
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        ఒకవేళ( పద్ధతి == DECOMP_EIG )
        {
            ఒకవేళ( type == CV_32F )
                Jacobi(a.ptr<దశాంశం>(), a.అడుగు, w.ptr<దశాంశం>(), v.ptr<దశాంశం>(), v.అడుగు, n, ptr);
            లేకపోతే
                Jacobi(a.ptr<ద్విగుణ>(), a.అడుగు, w.ptr<ద్విగుణ>(), v.ptr<ద్విగుణ>(), v.అడుగు, n, ptr);
            u = v;
        }
        లేకపోతే
        {
            ఒకవేళ( type == CV_32F )
                JacobiSVD(a.ptr<దశాంశం>(), a.అడుగు, w.ptr<దశాంశం>(), v.ptr<దశాంశం>(), v.అడుగు, m_, n);
            లేకపోతే
                JacobiSVD(a.ptr<ద్విగుణ>(), a.అడుగు, w.ptr<ద్విగుణ>(), v.ptr<ద్విగుణ>(), v.అడుగు, m_, n);
            u = a;
        }

        ఒకవేళ( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<దశాంశం>(), 0, u.ptr<దశాంశం>(), u.అడుగు, నిజం,
                   v.ptr<దశాంశం>(), v.అడుగు, నిజం, src2.ptr<దశాంశం>(),
                   src2.అడుగు, nb, dst.ptr<దశాంశం>(), dst.అడుగు, ptr);
        }
        లేకపోతే
        {
            SVBkSb(m_, n, w.ptr<ద్విగుణ>(), 0, u.ptr<ద్విగుణ>(), u.అడుగు, నిజం,
                   v.ptr<ద్విగుణ>(), v.అడుగు, నిజం, src2.ptr<ద్విగుణ>(),
                   src2.అడుగు, nb, dst.ptr<ద్విగుణ>(), dst.అడుగు, ptr);
        }
        ఫలితం = నిజం;
    }

    ఒకవేళ( !ఫలితం )
        dst = Scalar(0);

    తిరిగి ఫలితం;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

తార్కిక eigen( ఆదానం_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    పూర్ణాంకం type = src.type();
    పూర్ణాంకం n = src.అడ్డువరుసలు;

    CV_Assert( src.అడ్డువరుసలు == src.నిలువుల_సంఖ్య );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    ఒకవేళ( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#నిర్వచితమైతే HAVE_EIGEN
    స్థిరరాశి తార్కిక evecNeeded = _evects.needed();
    స్థిరరాశి పూర్ణాంకం esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    ఒకవేళ ( type == CV_64F )
    {
        Eigen::మాతృక_Xd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::మాతృక_Xd> es;
        es.compute(src_eig, esOptions);
        ఒకవేళ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ఒకవేళ ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            తిరిగి నిజం;
        }
    } లేకపోతే { // CV_32F
        Eigen::మాతృక_Xf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::మాతృక_Xf> es;
        es.compute(src_eig, esOptions);
        ఒకవేళ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ఒకవేళ ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            తిరిగి నిజం;
        }
    }
    తిరిగి అబద్ధం;
#లేకపోతే

    size_t elem_కొలత = src.elem_కొలత(), astep = align_కొలత(n*elem_కొలత, 16);
    Auto_బఫరు<uchar> buf(n*astep + n*5*elem_కొలత + 32);
    uchar* ptr = alignPtr(buf.దత్తాంశం(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elem_కొలత*n;
    src.copy_గమ్యం(a);
    తార్కిక ok = type == CV_32F ?
        Jacobi(a.ptr<దశాంశం>(), a.అడుగు, w.ptr<దశాంశం>(), v.ptr<దశాంశం>(), v.అడుగు, n, ptr) :
        Jacobi(a.ptr<ద్విగుణ>(), a.అడుగు, w.ptr<ద్విగుణ>(), v.ptr<ద్విగుణ>(), v.అడుగు, n, ptr);

    w.copy_గమ్యం(_evals);
    తిరిగి ok;
#అయితే_ముగింపు
}

స్థిర శూన్య _SVDcompute( ఆదానం_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, పూర్ణాంకం flags )
{
    Mat src = _aarr.getMat();
    పూర్ణాంకం m = src.అడ్డువరుసలు, n = src.నిలువుల_సంఖ్య;
    పూర్ణాంకం type = src.type();
    తార్కిక compute_uv = _u.needed() || _vt.needed();
    తార్కిక నిండిన_uv = (flags & SVD::నిండిన_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    ఒకవేళ( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = నిండిన_uv = అబద్ధం;
    }

    తార్కిక at = అబద్ధం;
    ఒకవేళ( m < n )
    {
        std::swap(m, n);
        at = నిజం;
    }

    పూర్ణాంకం urows = నిండిన_uv ? m : n;
    size_t esz = src.elem_కొలత(), astep = align_కొలత(m*esz, 16), vstep = align_కొలత(n*esz, 16);
    Auto_బఫరు<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.దత్తాంశం(), 16);
    Mat తాత్కాలిక_a(n, m, type, buf, astep);
    Mat తాత్కాలిక_w(n, 1, type, buf + urows*astep);
    Mat తాత్కాలిక_u(urows, m, type, buf, astep), తాత్కాలిక_v;

    ఒకవేళ( compute_uv )
        తాత్కాలిక_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    ఒకవేళ( urows > n )
        తాత్కాలిక_u = Scalar::all(0);

    ఒకవేళ( !at )
        transpose(src, తాత్కాలిక_a);
    లేకపోతే
        src.copy_గమ్యం(తాత్కాలిక_a);

    ఒకవేళ( type == CV_32F )
    {
        JacobiSVD(తాత్కాలిక_a.ptr<దశాంశం>(), తాత్కాలిక_u.అడుగు, తాత్కాలిక_w.ptr<దశాంశం>(),
              తాత్కాలిక_v.ptr<దశాంశం>(), తాత్కాలిక_v.అడుగు, m, n, compute_uv ? urows : 0);
    }
    లేకపోతే
    {
        JacobiSVD(తాత్కాలిక_a.ptr<ద్విగుణ>(), తాత్కాలిక_u.అడుగు, తాత్కాలిక_w.ptr<ద్విగుణ>(),
              తాత్కాలిక_v.ptr<ద్విగుణ>(), తాత్కాలిక_v.అడుగు, m, n, compute_uv ? urows : 0);
    }
    తాత్కాలిక_w.copy_గమ్యం(_w);
    ఒకవేళ( compute_uv )
    {
        ఒకవేళ( !at )
        {
            ఒకవేళ( _u.needed() )
                transpose(తాత్కాలిక_u, _u);
            ఒకవేళ( _vt.needed() )
                తాత్కాలిక_v.copy_గమ్యం(_vt);
        }
        లేకపోతే
        {
            ఒకవేళ( _u.needed() )
                transpose(తాత్కాలిక_v, _u);
            ఒకవేళ( _vt.needed() )
                తాత్కాలిక_u.copy_గమ్యం(_vt);
        }
    }
}


శూన్య SVD::compute( ఆదానం_Array a, OutputArray w, OutputArray u, OutputArray vt, పూర్ణాంకం flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

శూన్య SVD::compute( ఆదానం_Array a, OutputArray w, పూర్ణాంకం flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

శూన్య SVD::వెనుక_Subst( ఆదానం_Array _w, ఆదానం_Array _u, ఆదానం_Array _vt,
                     ఆదానం_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    పూర్ణాంకం type = w.type(), esz = (పూర్ణాంకం)w.elem_కొలత();
    పూర్ణాంకం m = u.అడ్డువరుసలు, n = vt.నిలువుల_సంఖ్య, nb = rhs.దత్తాంశం ? rhs.నిలువుల_సంఖ్య : m, nm = std::min(m, n);
    size_t wstep = w.అడ్డువరుసలు == 1 ? (size_t)esz : w.నిలువుల_సంఖ్య == 1 ? (size_t)w.అడుగు : (size_t)w.అడుగు + esz;
    Auto_బఫరు<uchar> బఫరు(nb*పరిమాణం(ద్విగుణ) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.దత్తాంశం && vt.దత్తాంశం && w.దత్తాంశం );
    CV_Assert( u.నిలువుల_సంఖ్య >= nm && vt.అడ్డువరుసలు >= nm &&
              (w.size() == కొలత(nm, 1) || w.size() == కొలత(1, nm) || w.size() == కొలత(vt.అడ్డువరుసలు, u.నిలువుల_సంఖ్య)) );
    CV_Assert( rhs.దత్తాంశం == 0 || (rhs.type() == type && rhs.అడ్డువరుసలు == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    ఒకవేళ( type == CV_32F )
        SVBkSb(m, n, w.ptr<దశాంశం>(), wstep, u.ptr<దశాంశం>(), u.అడుగు, అబద్ధం,
               vt.ptr<దశాంశం>(), vt.అడుగు, నిజం, rhs.ptr<దశాంశం>(), rhs.అడుగు, nb,
               dst.ptr<దశాంశం>(), dst.అడుగు, బఫరు.దత్తాంశం());
    లేకపోతే ఒకవేళ( type == CV_64F )
        SVBkSb(m, n, w.ptr<ద్విగుణ>(), wstep, u.ptr<ద్విగుణ>(), u.అడుగు, అబద్ధం,
               vt.ptr<ద్విగుణ>(), vt.అడుగు, నిజం, rhs.ptr<ద్విగుణ>(), rhs.అడుగు, nb,
               dst.ptr<ద్విగుణ>(), dst.అడుగు, బఫరు.దత్తాంశం());
    లేకపోతే
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::క్రియాకారకం ()(ఆదానం_Array a, పూర్ణాంకం flags)
{
    _SVDcompute(a, w, u, vt, flags);
    తిరిగి *ఇది;
}


శూన్య SVD::వెనుక_Subst( ఆదానం_Array rhs, OutputArray dst ) స్థిరరాశి
{
    వెనుక_Subst( w, u, vt, rhs, dst );
}

}


శూన్య cv::SVDecomp(ఆదానం_Array src, OutputArray w, OutputArray u, OutputArray vt, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

శూన్య cv::SVBackSubst(ఆదానం_Array w, ఆదానం_Array u, ఆదానం_Array vt, ఆదానం_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::వెనుక_Subst(w, u, vt, rhs, dst);
}
