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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ <limits>

#ifdef HAVE_EIGEN
#  જો defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  જો defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

નામાકાશ cv
{

પૂર્ણાંક LU(દશાંશ* A, size_t astep, પૂર્ણાંક m, દશાંશ* b, size_t bstep, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    પાછા hal::LU32f(A, astep, m, b, bstep, n);
}

પૂર્ણાંક LU(દ્વિગુણ* A, size_t astep, પૂર્ણાંક m, દ્વિગુણ* b, size_t bstep, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    પાછા hal::LU64f(A, astep, m, b, bstep, n);
}

તાર્કિક Cholesky(દશાંશ* A, size_t astep, પૂર્ણાંક m, દશાંશ* b, size_t bstep, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    પાછા hal::Cholesky32f(A, astep, m, b, bstep, n);
}

તાર્કિક Cholesky(દ્વિગુણ* A, size_t astep, પૂર્ણાંક m, દ્વિગુણ* b, size_t bstep, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    પાછા hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> સ્થિર અંતઃરેખ _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    જો( a > b )
    {
        b /= a;
        પાછા a*std::sqrt(1 + b*b);
    }
    જો( b > 0 )
    {
        a /= b;
        પાછા b*std::sqrt(1 + a*a);
    }
    પાછા 0;
}


template<typename _Tp> તાર્કિક
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, પૂર્ણાંક n, uchar* buf )
{
    અચલ _Tp eps = std::numeric_limits<_Tp>::epsilon();
    પૂર્ણાંક i, j, k, m;

    astep /= કદ(A[0]);
    જો( V )
    {
        vstep /= કદ(V[0]);
        માટે( i = 0; i < n; i++ )
        {
            માટે( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    પૂર્ણાંક iters, maxIters = n*n*30;

    પૂર્ણાંક* indR = (પૂર્ણાંક*)alignPtr(buf, કદ(પૂર્ણાંક));
    પૂર્ણાંક* indC = indR + n;
    _Tp mv = (_Tp)0;

    માટે( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        જો( k < n - 1 )
        {
            માટે( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                જો( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        જો( k > 0 )
        {
            માટે( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                જો( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    જો( n > 1 ) માટે( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        માટે( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            જો( mv < val )
                mv = val, k = i;
        }
        પૂર્ણાંક l = indR[k];
        માટે( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            જો( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        જો( std::abs(p) <= eps )
            તોડો;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        જો( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        માટે( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        માટે( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        માટે( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        જો( V )
            માટે( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        માટે( j = 0; j < 2; j++ )
        {
            પૂર્ણાંક idx = j == 0 ? k : l;
            જો( idx < n - 1 )
            {
                માટે( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    જો( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            જો( idx > 0 )
            {
                માટે( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    જો( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    માટે( k = 0; k < n-1; k++ )
    {
        m = k;
        માટે( i = k+1; i < n; i++ )
        {
            જો( W[m] < W[i] )
                m = i;
        }
        જો( k != m )
        {
            std::swap(W[m], W[k]);
            જો( V )
                માટે( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    પાછા સાચું;
}

સ્થિર તાર્કિક Jacobi( દશાંશ* S, size_t sstep, દશાંશ* e, દશાંશ* E, size_t estep, પૂર્ણાંક n, uchar* buf )
{
    પાછા JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

સ્થિર તાર્કિક Jacobi( દ્વિગુણ* S, size_t sstep, દ્વિગુણ* e, દ્વિગુણ* E, size_t estep, પૂર્ણાંક n, uchar* buf )
{
    પાછા JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> સંરચના VBLAS
{
    પૂર્ણાંક dot(અચલ T*, અચલ T*, પૂર્ણાંક, T*) અચલ { પાછા 0; }
    પૂર્ણાંક givens(T*, T*, પૂર્ણાંક, T, T) અચલ { પાછા 0; }
};

#જો (CV_SIMD || CV_SIMD_SCALABLE)
template<> અંતઃરેખ પૂર્ણાંક VBLAS<દશાંશ>::dot(અચલ દશાંશ* a, અચલ દશાંશ* b, પૂર્ણાંક n, દશાંશ* result) અચલ
{
    જો( n < 2*VTraits<v_float32>::vlanes() )
        પાછા 0;
    પૂર્ણાંક k = 0;
    v_float32 s0 = vx_setzero_f32();
    માટે( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    પાછા k;
}


template<> અંતઃરેખ પૂર્ણાંક VBLAS<દશાંશ>::givens(દશાંશ* a, દશાંશ* b, પૂર્ણાંક n, દશાંશ c, દશાંશ s) અચલ
{
    જો( n < VTraits<v_float32>::vlanes())
        પાછા 0;
    પૂર્ણાંક k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    માટે( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    પાછા k;
}


#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> અંતઃરેખ પૂર્ણાંક VBLAS<દ્વિગુણ>::dot(અચલ દ્વિગુણ* a, અચલ દ્વિગુણ* b, પૂર્ણાંક n, દ્વિગુણ* result) અચલ
{
    જો( n < 2*VTraits<v_float64>::vlanes() )
        પાછા 0;
    પૂર્ણાંક k = 0;
    v_float64 s0 = vx_setzero_f64();
    માટે( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    પાછા k;
}


template<> અંતઃરેખ પૂર્ણાંક VBLAS<દ્વિગુણ>::givens(દ્વિગુણ* a, દ્વિગુણ* b, પૂર્ણાંક n, દ્વિગુણ c, દ્વિગુણ s) અચલ
{
    પૂર્ણાંક k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    માટે( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    પાછા k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> શૂન્ય
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક n1, દ્વિગુણ minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<દ્વિગુણ> Wbuf(n);
    દ્વિગુણ* W = Wbuf.data();
    પૂર્ણાંક i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    દ્વિગુણ sd;
    astep /= કદ(At[0]);
    vstep /= કદ(Vt[0]);

    માટે( i = 0; i < n; i++ )
    {
        માટે( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (દ્વિગુણ)t*t;
        }
        W[i] = sd;

        જો( Vt )
        {
            માટે( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    માટે( iter = 0; iter < max_iter; iter++ )
    {
        તાર્કિક changed = ખોટું;

        માટે( i = 0; i < n-1; i++ )
            માટે( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                દ્વિગુણ a = W[i], p = 0, b = W[j];

                માટે( k = 0; k < m; k++ )
                    p += (દ્વિગુણ)Ai[k]*Aj[k];

                જો( std::abs(p) <= eps*std::sqrt((દ્વિગુણ)a*b) )
                    ચાલુ;

                p *= 2;
                દ્વિગુણ beta = a - b, gamma = hypot((દ્વિગુણ)p, beta);
                જો( beta < 0 )
                {
                    દ્વિગુણ delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                નહીંતર
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                માટે( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (દ્વિગુણ)t0*t0; b += (દ્વિગુણ)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = સાચું;

                જો( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    માટે( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        જો( !changed )
            તોડો;
    }

    માટે( i = 0; i < n; i++ )
    {
        માટે( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (દ્વિગુણ)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    માટે( i = 0; i < n-1; i++ )
    {
        j = i;
        માટે( k = i+1; k < n; k++ )
        {
            જો( W[j] < W[k] )
                j = k;
        }
        જો( i != j )
        {
            std::swap(W[i], W[j]);
            જો( Vt )
            {
                માટે( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                માટે( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    માટે( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    જો( !Vt )
        પાછા;

    RNG rng(0x12345678);
    માટે( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        માટે( પૂર્ણાંક ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            અચલ _Tp val0 = (_Tp)(1./m);
            માટે( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            માટે( iter = 0; iter < 2; iter++ )
            {
                માટે( j = 0; j < i; j++ )
                {
                    sd = 0;
                    માટે( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    માટે( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    માટે( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            માટે( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (દ્વિગુણ)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        માટે( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


સ્થિર શૂન્ય JacobiSVD(દશાંશ* At, size_t astep, દશાંશ* W, દશાંશ* Vt, size_t vstep, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક n1=-1)
{
    hal::SVD32f(At, astep, W, ખાલી, astep, Vt, vstep, m, n, n1);
}

સ્થિર શૂન્ય JacobiSVD(દ્વિગુણ* At, size_t astep, દ્વિગુણ* W, દ્વિગુણ* Vt, size_t vstep, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક n1=-1)
{
    hal::SVD64f(At, astep, W, ખાલી, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> સ્થિર અંતઃરેખ પૂર્ણાંક
decodeSVDParameters(અચલ fptype* U, અચલ fptype* Vt, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક n1)
{
    પૂર્ણાંક halSVDFlag = 0;
    જો(Vt == ખાલી)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    નહીંતર જો(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        જો(U == ખાલી)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    નહીંતર જો(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        જો(U == ખાલી)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    પાછા halSVDFlag;
}

શૂન્ય hal::SVD32f(દશાંશ* At, size_t astep, દશાંશ* W, દશાંશ* U, size_t ustep, દશાંશ* Vt, size_t vstep, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

શૂન્ય hal::SVD64f(દ્વિગુણ* At, size_t astep, દ્વિગુણ* W, દ્વિગુણ* U, size_t ustep, દ્વિગુણ* Vt, size_t vstep, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> સ્થિર શૂન્ય
MatrAXPY( પૂર્ણાંક m, પૂર્ણાંક n, અચલ T1* x, પૂર્ણાંક dx,
         અચલ T2* a, પૂર્ણાંક inca, T3* y, પૂર્ણાંક dy )
{
    પૂર્ણાંક i;
    માટે( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        પૂર્ણાંક j = 0;
         #જો CV_ENABLE_UNROLLED
        માટે(; j <= n - 4; j += 4 )
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
        #endif
        માટે( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> સ્થિર શૂન્ય
SVBkSbImpl_( પૂર્ણાંક m, પૂર્ણાંક n, અચલ T* w, પૂર્ણાંક incw,
       અચલ T* u, પૂર્ણાંક ldu, તાર્કિક uT,
       અચલ T* v, પૂર્ણાંક ldv, તાર્કિક vT,
       અચલ T* b, પૂર્ણાંક ldb, પૂર્ણાંક nb,
       T* x, પૂર્ણાંક ldx, દ્વિગુણ* buffer, T eps )
{
    દ્વિગુણ threshold = 0;
    પૂર્ણાંક udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    પૂર્ણાંક vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    પૂર્ણાંક i, j, nm = std::min(m, n);

    જો( !b )
        nb = m;

    માટે( i = 0; i < n; i++ )
        માટે( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    માટે( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    માટે( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        દ્વિગુણ wi = w[i*incw];
        જો( (દ્વિગુણ)std::abs(wi) <= threshold )
            ચાલુ;
        wi = 1/wi;

        જો( nb == 1 )
        {
            દ્વિગુણ s = 0;
            જો( b )
                માટે( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            નહીંતર
                s = u[0];
            s *= wi;

            માટે( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        નહીંતર
        {
            જો( b )
            {
                માટે( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                માટે( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            નહીંતર
            {
                માટે( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

સ્થિર શૂન્ય
SVBkSb( પૂર્ણાંક m, પૂર્ણાંક n, અચલ દશાંશ* w, size_t wstep,
        અચલ દશાંશ* u, size_t ustep, તાર્કિક uT,
        અચલ દશાંશ* v, size_t vstep, તાર્કિક vT,
        અચલ દશાંશ* b, size_t bstep, પૂર્ણાંક nb,
        દશાંશ* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (પૂર્ણાંક)(wstep/કદ(w[0])) : 1,
                u, (પૂર્ણાંક)(ustep/કદ(u[0])), uT,
                v, (પૂર્ણાંક)(vstep/કદ(v[0])), vT,
                b, (પૂર્ણાંક)(bstep/કદ(b[0])), nb,
                x, (પૂર્ણાંક)(xstep/કદ(x[0])),
                (દ્વિગુણ*)alignPtr(buffer, કદ(દ્વિગુણ)), (દશાંશ)(DBL_EPSILON*2) );
}

સ્થિર શૂન્ય
SVBkSb( પૂર્ણાંક m, પૂર્ણાંક n, અચલ દ્વિગુણ* w, size_t wstep,
       અચલ દ્વિગુણ* u, size_t ustep, તાર્કિક uT,
       અચલ દ્વિગુણ* v, size_t vstep, તાર્કિક vT,
       અચલ દ્વિગુણ* b, size_t bstep, પૂર્ણાંક nb,
       દ્વિગુણ* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (પૂર્ણાંક)(wstep/કદ(w[0])) : 1,
                u, (પૂર્ણાંક)(ustep/કદ(u[0])), uT,
                v, (પૂર્ણાંક)(vstep/કદ(v[0])), vT,
                b, (પૂર્ણાંક)(bstep/કદ(b[0])), nb,
                x, (પૂર્ણાંક)(xstep/કદ(x[0])),
                (દ્વિગુણ*)alignPtr(buffer, કદ(દ્વિગુણ)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((દ્વિગુણ)m(0,0)*m(1,1) - (દ્વિગુણ)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((દ્વિગુણ)m(1,1)*m(2,2) - (દ્વિગુણ)m(1,2)*m(2,1)) -  \
                   m(0,1)*((દ્વિગુણ)m(1,0)*m(2,2) - (દ્વિગુણ)m(1,2)*m(2,0)) +  \
                   m(0,2)*((દ્વિગુણ)m(1,0)*m(2,1) - (દ્વિગુણ)m(1,1)*m(2,0)))

દ્વિગુણ determinant( આગત_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    દ્વિગુણ result = 0;
    પૂર્ણાંક type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    અચલ uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((દશાંશ*)(m + y*step))[x]
    #define Md(y, x) ((દ્વિગુણ*)(m + y*step))[x]

    જો( type == CV_32F )
    {
        જો( rows == 2 )
            result = det2(Mf);
        નહીંતર જો( rows == 3 )
            result = det3(Mf);
        નહીંતર જો( rows == 1 )
            result = Mf(0,0);
        નહીંતર
        {
            size_t bufSize = rows*rows*કદ(દશાંશ);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<દશાંશ>(), a.step, rows, 0, 0, 0);
            જો( result )
            {
                માટે( પૂર્ણાંક i = 0; i < rows; i++ )
                    result *= a.at<દશાંશ>(i,i);
            }
        }
    }
    નહીંતર
    {
        જો( rows == 2 )
            result = det2(Md);
        નહીંતર જો( rows == 3 )
            result = det3(Md);
        નહીંતર જો( rows == 1 )
            result = Md(0,0);
        નહીંતર
        {
            size_t bufSize = rows*rows*કદ(દ્વિગુણ);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<દ્વિગુણ>(), a.step, rows, 0, 0, 0);
            જો( result )
            {
                માટે( પૂર્ણાંક i = 0; i < rows; i++ )
                    result *= a.at<દ્વિગુણ>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    પાછા result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((દશાંશ*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((દ્વિગુણ*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((દશાંશ*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((દ્વિગુણ*)(dstdata + y*dststep))[x]

દ્વિગુણ invert( આગત_Array _src, OutputArray _dst, પૂર્ણાંક method )
{
    CV_INSTRUMENT_REGION();

    તાર્કિક result = ખોટું;
    Mat src = _src.getMat();
    પૂર્ણાંક type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    પૂર્ણાંક m = src.rows, n = src.cols;

    જો( method == DECOMP_SVD )
    {
        પૂર્ણાંક nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + કદ(દ્વિગુણ));
        uchar* buf = alignPtr((uchar*)_buf.data(), (પૂર્ણાંક)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        પાછા type == CV_32F ?
            (w.ptr<દશાંશ>()[0] >= FLT_EPSILON ?
             w.ptr<દશાંશ>()[n-1]/w.ptr<દશાંશ>()[0] : 0) :
            (w.ptr<દ્વિગુણ>()[0] >= DBL_EPSILON ?
             w.ptr<દ્વિગુણ>()[n-1]/w.ptr<દ્વિગુણ>()[0] : 0);
    }

    CV_Assert( m == n );

    જો( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + કદ(દ્વિગુણ));
        uchar* buf = alignPtr((uchar*)_buf.data(), (પૂર્ણાંક)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        પાછા type == CV_32F ?
        (w.ptr<દશાંશ>()[0] >= FLT_EPSILON ?
         w.ptr<દશાંશ>()[n-1]/w.ptr<દશાંશ>()[0] : 0) :
        (w.ptr<દ્વિગુણ>()[0] >= DBL_EPSILON ?
         w.ptr<દ્વિગુણ>()[n-1]/w.ptr<દ્વિગુણ>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    જો( n <= 3 )
    {
        અચલ uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        જો( n == 2 )
        {
            જો( type == CV_32FC1 )
            {
                દ્વિગુણ d = det2(Sf);
                જો( d != 0. )
                {
                    result = સાચું;
                    d = 1./d;
                #જો CV_SIMD128
                    અચલ દશાંશ d_32f = (દશાંશ)d;
                    અચલ v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((અચલ દશાંશ *)srcdata, (અચલ દશાંશ *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_નીચલું(v_rotate_જમણું<1>(s0), s0));
                    v_store_નીચલું((દશાંશ*)dstdata, s0);
                    v_store_ઉપલું((દશાંશ*)(dstdata + dststep), s0);
                #નહીંતર
                    દ્વિગુણ t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (દશાંશ)t0;
                    Df(0,0) = (દશાંશ)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (દશાંશ)t0;
                    Df(1,0) = (દશાંશ)t1;
                #endif
                }
            }
            નહીંતર
            {
                દ્વિગુણ d = det2(Sd);
                જો( d != 0. )
                {
                    result = સાચું;
                    d = 1./d;
                #જો CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((અચલ દ્વિગુણ *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((અચલ દ્વિગુણ *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<દ્વિગુણ>(0), v_extract<1>(s0, s1));//12
                    v_store((દ્વિગુણ*)dstdata, v_combine_નીચલું(sm, ss));//31
                    v_store((દ્વિગુણ*)(dstdata + dststep), v_combine_ઉપલું(ss, sm));//20
                #નહીંતર
                    દ્વિગુણ t0, t1;
                    t0 = Sd(0,0)*d;
                    t1 = Sd(1,1)*d;
                    Dd(1,1) = t0;
                    Dd(0,0) = t1;
                    t0 = -Sd(0,1)*d;
                    t1 = -Sd(1,0)*d;
                    Dd(0,1) = t0;
                    Dd(1,0) = t1;
                #endif
                }
            }
        }
        નહીંતર જો( n == 3 )
        {
            જો( type == CV_32FC1 )
            {
                દ્વિગુણ d = det3(Sf);

                જો( d != 0. )
                {
                    દ્વિગુણ t[12];

                    result = સાચું;
                    d = 1./d;
                    t[0] = (((દ્વિગુણ)Sf(1,1) * Sf(2,2) - (દ્વિગુણ)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((દ્વિગુણ)Sf(0,2) * Sf(2,1) - (દ્વિગુણ)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((દ્વિગુણ)Sf(0,1) * Sf(1,2) - (દ્વિગુણ)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((દ્વિગુણ)Sf(1,2) * Sf(2,0) - (દ્વિગુણ)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((દ્વિગુણ)Sf(0,0) * Sf(2,2) - (દ્વિગુણ)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((દ્વિગુણ)Sf(0,2) * Sf(1,0) - (દ્વિગુણ)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((દ્વિગુણ)Sf(1,0) * Sf(2,1) - (દ્વિગુણ)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((દ્વિગુણ)Sf(0,1) * Sf(2,0) - (દ્વિગુણ)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((દ્વિગુણ)Sf(0,0) * Sf(1,1) - (દ્વિગુણ)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (દશાંશ)t[0]; Df(0,1) = (દશાંશ)t[1]; Df(0,2) = (દશાંશ)t[2];
                    Df(1,0) = (દશાંશ)t[3]; Df(1,1) = (દશાંશ)t[4]; Df(1,2) = (દશાંશ)t[5];
                    Df(2,0) = (દશાંશ)t[6]; Df(2,1) = (દશાંશ)t[7]; Df(2,2) = (દશાંશ)t[8];
                }
            }
            નહીંતર
            {
                દ્વિગુણ d = det3(Sd);
                જો( d != 0. )
                {
                    result = સાચું;
                    d = 1./d;
                    દ્વિગુણ t[9];

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
        નહીંતર
        {
            CV_Assert( n == 1 );

            જો( type == CV_32FC1 )
            {
                દ્વિગુણ d = Sf(0,0);
                જો( d != 0. )
                {
                    result = સાચું;
                    Df(0,0) = (દશાંશ)(1./d);
                }
            }
            નહીંતર
            {
                દ્વિગુણ d = Sd(0,0);
                જો( d != 0. )
                {
                    result = સાચું;
                    Dd(0,0) = 1./d;
                }
            }
        }
        જો( !result )
            dst = Scalar(0);
        પાછા result;
    }

   પૂર્ણાંક elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    જો( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<દશાંશ>(), src1.step, n, dst.ptr<દશાંશ>(), dst.step, n) != 0;
    નહીંતર જો( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<દ્વિગુણ>(), src1.step, n, dst.ptr<દ્વિગુણ>(), dst.step, n) != 0;
    નહીંતર જો( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<દશાંશ>(), src1.step, n, dst.ptr<દશાંશ>(), dst.step, n);
    નહીંતર
        result = hal::Cholesky64f(src1.ptr<દ્વિગુણ>(), src1.step, n, dst.ptr<દ્વિગુણ>(), dst.step, n);

    જો( !result )
        dst = Scalar(0);

    પાછા result;
}

UMat UMat::inv(પૂર્ણાંક method) અચલ
{
    UMat m;
    invert(*આ, m, method);
    પાછા m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

તાર્કિક solve( આગત_Array _src, આગત_Array _src2arg, OutputArray _dst, પૂર્ણાંક method )
{
    CV_INSTRUMENT_REGION();

    તાર્કિક result = સાચું;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    પૂર્ણાંક type = src.type();
    તાર્કિક is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    જો( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((દશાંશ*)(bdata + y*src2step))[0]
        #define bd(y) ((દ્વિગુણ*)(bdata + y*src2step))[0]

        અચલ uchar* srcdata = src.ptr();
        અચલ uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        જો( src.rows == 2 )
        {
            જો( type == CV_32FC1 )
            {
                દ્વિગુણ d = det2(Sf);
                જો( d != 0. )
                {
                    દ્વિગુણ t;
                    d = 1./d;
                    t = (દશાંશ)(((દ્વિગુણ)bf(0)*Sf(1,1) - (દ્વિગુણ)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (દશાંશ)(((દ્વિગુણ)bf(1)*Sf(0,0) - (દ્વિગુણ)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (દશાંશ)t;
                }
                નહીંતર
                    result = ખોટું;
            }
            નહીંતર
            {
                દ્વિગુણ d = det2(Sd);
                જો( d != 0. )
                {
                    દ્વિગુણ t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                નહીંતર
                    result = ખોટું;
            }
        }
        નહીંતર જો( src.rows == 3 )
        {
            જો( type == CV_32FC1 )
            {
                દ્વિગુણ d = det3(Sf);
                જો( d != 0. )
                {
                    દશાંશ t[3];
                    d = 1./d;

                    t[0] = (દશાંશ)(d*
                           (bf(0)*((દ્વિગુણ)Sf(1,1)*Sf(2,2) - (દ્વિગુણ)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((દ્વિગુણ)bf(1)*Sf(2,2) - (દ્વિગુણ)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((દ્વિગુણ)bf(1)*Sf(2,1) - (દ્વિગુણ)Sf(1,1)*bf(2))));

                    t[1] = (દશાંશ)(d*
                           (Sf(0,0)*(દ્વિગુણ)(bf(1)*Sf(2,2) - (દ્વિગુણ)Sf(1,2)*bf(2)) -
                            bf(0)*((દ્વિગુણ)Sf(1,0)*Sf(2,2) - (દ્વિગુણ)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((દ્વિગુણ)Sf(1,0)*bf(2) - (દ્વિગુણ)bf(1)*Sf(2,0))));

                    t[2] = (દશાંશ)(d*
                           (Sf(0,0)*((દ્વિગુણ)Sf(1,1)*bf(2) - (દ્વિગુણ)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((દ્વિગુણ)Sf(1,0)*bf(2) - (દ્વિગુણ)bf(1)*Sf(2,0)) +
                            bf(0)*((દ્વિગુણ)Sf(1,0)*Sf(2,1) - (દ્વિગુણ)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                નહીંતર
                    result = ખોટું;
            }
            નહીંતર
            {
                દ્વિગુણ d = det3(Sd);
                જો( d != 0. )
                {
                    દ્વિગુણ t[9];

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
                નહીંતર
                    result = ખોટું;
            }
        }
        નહીંતર
        {
            CV_Assert( src.rows == 1 );

            જો( type == CV_32FC1 )
            {
                દ્વિગુણ d = Sf(0,0);
                જો( d != 0. )
                    Df(0,0) = (દશાંશ)(bf(0)/d);
                નહીંતર
                    result = ખોટું;
            }
            નહીંતર
            {
                દ્વિગુણ d = Sd(0,0);
                જો( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                નહીંતર
                    result = ખોટું;
            }
        }
        પાછા result;
    }

    પૂર્ણાંક m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    જો( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    જો( m == n )
        is_normal = ખોટું;
    નહીંતર જો( is_normal )
    {
        m_ = n;
        જો( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    જો( is_normal )
        bufsize += n*nb*esz;
    જો( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*કદ(દ્વિગુણ) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    જો( is_normal )
        mulTransposed(src, a, સાચું);
    નહીંતર જો( method != DECOMP_SVD )
        src.copyTo(a);
    નહીંતર
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    જો( !is_normal )
    {
        જો( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    નહીંતર
    {
        // a'*b
        જો( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        નહીંતર
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    જો( method == DECOMP_LU )
    {
        જો( type == CV_32F )
            result = hal::LU32f(a.ptr<દશાંશ>(), a.step, n, dst.ptr<દશાંશ>(), dst.step, nb) != 0;
        નહીંતર
            result = hal::LU64f(a.ptr<દ્વિગુણ>(), a.step, n, dst.ptr<દ્વિગુણ>(), dst.step, nb) != 0;
    }
    નહીંતર જો( method == DECOMP_CHOLESKY )
    {
        જો( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<દશાંશ>(), a.step, n, dst.ptr<દશાંશ>(), dst.step, nb);
        નહીંતર
            result = hal::Cholesky64f(a.ptr<દ્વિગુણ>(), a.step, n, dst.ptr<દ્વિગુણ>(), dst.step, nb);
    }
    નહીંતર જો( method == DECOMP_QR )
    {
        Mat rhsMat;
        જો( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        નહીંતર
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        જો( type == CV_32F )
            result = hal::QR32f(a.ptr<દશાંશ>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<દશાંશ>(), rhsMat.step, ખાલી) != 0;
        નહીંતર
            result = hal::QR64f(a.ptr<દ્વિગુણ>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<દ્વિગુણ>(), rhsMat.step, ખાલી) != 0;

        જો (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    નહીંતર
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        જો( method == DECOMP_EIG )
        {
            જો( type == CV_32F )
                Jacobi(a.ptr<દશાંશ>(), a.step, w.ptr<દશાંશ>(), v.ptr<દશાંશ>(), v.step, n, ptr);
            નહીંતર
                Jacobi(a.ptr<દ્વિગુણ>(), a.step, w.ptr<દ્વિગુણ>(), v.ptr<દ્વિગુણ>(), v.step, n, ptr);
            u = v;
        }
        નહીંતર
        {
            જો( type == CV_32F )
                JacobiSVD(a.ptr<દશાંશ>(), a.step, w.ptr<દશાંશ>(), v.ptr<દશાંશ>(), v.step, m_, n);
            નહીંતર
                JacobiSVD(a.ptr<દ્વિગુણ>(), a.step, w.ptr<દ્વિગુણ>(), v.ptr<દ્વિગુણ>(), v.step, m_, n);
            u = a;
        }

        જો( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<દશાંશ>(), 0, u.ptr<દશાંશ>(), u.step, સાચું,
                   v.ptr<દશાંશ>(), v.step, સાચું, src2.ptr<દશાંશ>(),
                   src2.step, nb, dst.ptr<દશાંશ>(), dst.step, ptr);
        }
        નહીંતર
        {
            SVBkSb(m_, n, w.ptr<દ્વિગુણ>(), 0, u.ptr<દ્વિગુણ>(), u.step, સાચું,
                   v.ptr<દ્વિગુણ>(), v.step, સાચું, src2.ptr<દ્વિગુણ>(),
                   src2.step, nb, dst.ptr<દ્વિગુણ>(), dst.step, ptr);
        }
        result = સાચું;
    }

    જો( !result )
        dst = Scalar(0);

    પાછા result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

તાર્કિક eigen( આગત_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    પૂર્ણાંક type = src.type();
    પૂર્ણાંક n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    જો( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    અચલ તાર્કિક evecNeeded = _evects.needed();
    અચલ પૂર્ણાંક esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    જો ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        જો ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            જો ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            પાછા સાચું;
        }
    } નહીંતર { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        જો ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            જો ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            પાછા સાચું;
        }
    }
    પાછા ખોટું;
#નહીંતર

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    તાર્કિક ok = type == CV_32F ?
        Jacobi(a.ptr<દશાંશ>(), a.step, w.ptr<દશાંશ>(), v.ptr<દશાંશ>(), v.step, n, ptr) :
        Jacobi(a.ptr<દ્વિગુણ>(), a.step, w.ptr<દ્વિગુણ>(), v.ptr<દ્વિગુણ>(), v.step, n, ptr);

    w.copyTo(_evals);
    પાછા ok;
#endif
}

સ્થિર શૂન્ય _SVDcompute( આગત_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, પૂર્ણાંક flags )
{
    Mat src = _aarr.getMat();
    પૂર્ણાંક m = src.rows, n = src.cols;
    પૂર્ણાંક type = src.type();
    તાર્કિક compute_uv = _u.needed() || _vt.needed();
    તાર્કિક full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    જો( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = ખોટું;
    }

    તાર્કિક at = ખોટું;
    જો( m < n )
    {
        std::swap(m, n);
        at = સાચું;
    }

    પૂર્ણાંક urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    જો( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    જો( urows > n )
        temp_u = Scalar::all(0);

    જો( !at )
        transpose(src, temp_a);
    નહીંતર
        src.copyTo(temp_a);

    જો( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<દશાંશ>(), temp_u.step, temp_w.ptr<દશાંશ>(),
              temp_v.ptr<દશાંશ>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    નહીંતર
    {
        JacobiSVD(temp_a.ptr<દ્વિગુણ>(), temp_u.step, temp_w.ptr<દ્વિગુણ>(),
              temp_v.ptr<દ્વિગુણ>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    જો( compute_uv )
    {
        જો( !at )
        {
            જો( _u.needed() )
                transpose(temp_u, _u);
            જો( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        નહીંતર
        {
            જો( _u.needed() )
                transpose(temp_v, _u);
            જો( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


શૂન્ય SVD::compute( આગત_Array a, OutputArray w, OutputArray u, OutputArray vt, પૂર્ણાંક flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

શૂન્ય SVD::compute( આગત_Array a, OutputArray w, પૂર્ણાંક flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

શૂન્ય SVD::backSubst( આગત_Array _w, આગત_Array _u, આગત_Array _vt,
                     આગત_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    પૂર્ણાંક type = w.type(), esz = (પૂર્ણાંક)w.elemSize();
    પૂર્ણાંક m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*કદ(દ્વિગુણ) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    જો( type == CV_32F )
        SVBkSb(m, n, w.ptr<દશાંશ>(), wstep, u.ptr<દશાંશ>(), u.step, ખોટું,
               vt.ptr<દશાંશ>(), vt.step, સાચું, rhs.ptr<દશાંશ>(), rhs.step, nb,
               dst.ptr<દશાંશ>(), dst.step, buffer.data());
    નહીંતર જો( type == CV_64F )
        SVBkSb(m, n, w.ptr<દ્વિગુણ>(), wstep, u.ptr<દ્વિગુણ>(), u.step, ખોટું,
               vt.ptr<દ્વિગુણ>(), vt.step, સાચું, rhs.ptr<દ્વિગુણ>(), rhs.step, nb,
               dst.ptr<દ્વિગુણ>(), dst.step, buffer.data());
    નહીંતર
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(આગત_Array a, પૂર્ણાંક flags)
{
    _SVDcompute(a, w, u, vt, flags);
    પાછા *આ;
}


શૂન્ય SVD::backSubst( આગત_Array rhs, OutputArray dst ) અચલ
{
    backSubst( w, u, vt, rhs, dst );
}

}


શૂન્ય cv::SVDecomp(આગત_Array src, OutputArray w, OutputArray u, OutputArray vt, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

શૂન્ય cv::SVBackSubst(આગત_Array w, આગત_Array u, આગત_Array vt, આગત_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
