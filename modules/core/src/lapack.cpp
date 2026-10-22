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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <limits>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EIGEN
#  ਜੇ defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  ਜੇ defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv
{

ਪੂਰਨ LU(ਦਸ਼ਮਲਵ* A, size_t astep, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t bstep, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਵਾਪਸ hal::LU32f(A, astep, m, b, bstep, n);
}

ਪੂਰਨ LU(ਦੁੱਗਣਾ* A, size_t astep, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t bstep, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਵਾਪਸ hal::LU64f(A, astep, m, b, bstep, n);
}

ਤਾਰਕਿਕ Cholesky(ਦਸ਼ਮਲਵ* A, size_t astep, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t bstep, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਵਾਪਸ hal::Cholesky32f(A, astep, m, b, bstep, n);
}

ਤਾਰਕਿਕ Cholesky(ਦੁੱਗਣਾ* A, size_t astep, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t bstep, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਵਾਪਸ hal::Cholesky64f(A, astep, m, b, bstep, n);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਸਥਿਰ ਸਤਰਅੰਦਰ _Tp hypot(_Tp a, _Tp b)
{
    a = std::ਨਿਰਪੇਖ(a);
    b = std::ਨਿਰਪੇਖ(b);
    ਜੇ( a > b )
    {
        b /= a;
        ਵਾਪਸ a*std::ਵਰਗਮੂਲ(1 + b*b);
    }
    ਜੇ( b > 0 )
    {
        a /= b;
        ਵਾਪਸ b*std::ਵਰਗਮੂਲ(1 + a*a);
    }
    ਵਾਪਸ 0;
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਤਾਰਕਿਕ
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, ਪੂਰਨ n, uchar* buf )
{
    ਅਚਲ _Tp eps = std::numeric_limits<_Tp>::epsilon();
    ਪੂਰਨ i, j, k, m;

    astep /= ਆਕਾਰ(A[0]);
    ਜੇ( V )
    {
        vstep /= ਆਕਾਰ(V[0]);
        ਲਈ( i = 0; i < n; i++ )
        {
            ਲਈ( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    ਪੂਰਨ iters, maxIters = n*n*30;

    ਪੂਰਨ* indR = (ਪੂਰਨ*)alignPtr(buf, ਆਕਾਰ(ਪੂਰਨ));
    ਪੂਰਨ* indC = indR + n;
    _Tp mv = (_Tp)0;

    ਲਈ( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        ਜੇ( k < n - 1 )
        {
            ਲਈ( m = k+1, mv = std::ਨਿਰਪੇਖ(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::ਨਿਰਪੇਖ(A[astep*k+i]);
                ਜੇ( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        ਜੇ( k > 0 )
        {
            ਲਈ( m = 0, mv = std::ਨਿਰਪੇਖ(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::ਨਿਰਪੇਖ(A[astep*i+k]);
                ਜੇ( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    ਜੇ( n > 1 ) ਲਈ( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        ਲਈ( k = 0, mv = std::ਨਿਰਪੇਖ(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::ਨਿਰਪੇਖ(A[astep*i + indR[i]]);
            ਜੇ( mv < val )
                mv = val, k = i;
        }
        ਪੂਰਨ l = indR[k];
        ਲਈ( i = 1; i < n; i++ )
        {
            _Tp val = std::ਨਿਰਪੇਖ(A[astep*indC[i] + i]);
            ਜੇ( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        ਜੇ( std::ਨਿਰਪੇਖ(p) <= eps )
            ਰੋਕੋ;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::ਨਿਰਪੇਖ(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        ਜੇ( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ rotate
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        ਲਈ( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        ਲਈ( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        ਲਈ( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        ਜੇ( V )
            ਲਈ( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ rotate

        ਲਈ( j = 0; j < 2; j++ )
        {
            ਪੂਰਨ idx = j == 0 ? k : l;
            ਜੇ( idx < n - 1 )
            {
                ਲਈ( m = idx+1, mv = std::ਨਿਰਪੇਖ(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::ਨਿਰਪੇਖ(A[astep*idx+i]);
                    ਜੇ( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            ਜੇ( idx > 0 )
            {
                ਲਈ( m = 0, mv = std::ਨਿਰਪੇਖ(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::ਨਿਰਪੇਖ(A[astep*i+idx]);
                    ਜੇ( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    ਲਈ( k = 0; k < n-1; k++ )
    {
        m = k;
        ਲਈ( i = k+1; i < n; i++ )
        {
            ਜੇ( W[m] < W[i] )
                m = i;
        }
        ਜੇ( k != m )
        {
            std::swap(W[m], W[k]);
            ਜੇ( V )
                ਲਈ( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ Jacobi( ਦਸ਼ਮਲਵ* S, size_t sstep, ਦਸ਼ਮਲਵ* e, ਦਸ਼ਮਲਵ* E, size_t estep, ਪੂਰਨ n, uchar* buf )
{
    ਵਾਪਸ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

ਸਥਿਰ ਤਾਰਕਿਕ Jacobi( ਦੁੱਗਣਾ* S, size_t sstep, ਦੁੱਗਣਾ* e, ਦੁੱਗਣਾ* E, size_t estep, ਪੂਰਨ n, uchar* buf )
{
    ਵਾਪਸ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ VBLAS
{
    ਪੂਰਨ dot(ਅਚਲ T*, ਅਚਲ T*, ਪੂਰਨ, T*) ਅਚਲ { ਵਾਪਸ 0; }
    ਪੂਰਨ givens(T*, T*, ਪੂਰਨ, T, T) ਅਚਲ { ਵਾਪਸ 0; }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਾਂਚਾ<> ਸਤਰਅੰਦਰ ਪੂਰਨ VBLAS<ਦਸ਼ਮਲਵ>::dot(ਅਚਲ ਦਸ਼ਮਲਵ* a, ਅਚਲ ਦਸ਼ਮਲਵ* b, ਪੂਰਨ n, ਦਸ਼ਮਲਵ* ਨਤੀਜਾ) ਅਚਲ
{
    ਜੇ( n < 2*VTraits<v_float32>::vlanes() )
        ਵਾਪਸ 0;
    ਪੂਰਨ k = 0;
    v_float32 s0 = vx_setzero_f32();
    ਲਈ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_ਲੱਦੋ(a + k);
        v_float32 b0 = vx_ਲੱਦੋ(b + k);

        s0 = v_ਜੋੜੋ(s0, v_mul(a0, b0));
    }
    *ਨਤੀਜਾ = v_reduce_ਜੋੜ(s0);
    vx_cleanup();
    ਵਾਪਸ k;
}


ਸਾਂਚਾ<> ਸਤਰਅੰਦਰ ਪੂਰਨ VBLAS<ਦਸ਼ਮਲਵ>::givens(ਦਸ਼ਮਲਵ* a, ਦਸ਼ਮਲਵ* b, ਪੂਰਨ n, ਦਸ਼ਮਲਵ c, ਦਸ਼ਮਲਵ s) ਅਚਲ
{
    ਜੇ( n < VTraits<v_float32>::vlanes())
        ਵਾਪਸ 0;
    ਪੂਰਨ k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    ਲਈ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_ਲੱਦੋ(a + k);
        v_float32 b0 = vx_ਲੱਦੋ(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ਵਾਪਸ k;
}


#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ਸਾਂਚਾ<> ਸਤਰਅੰਦਰ ਪੂਰਨ VBLAS<ਦੁੱਗਣਾ>::dot(ਅਚਲ ਦੁੱਗਣਾ* a, ਅਚਲ ਦੁੱਗਣਾ* b, ਪੂਰਨ n, ਦੁੱਗਣਾ* ਨਤੀਜਾ) ਅਚਲ
{
    ਜੇ( n < 2*VTraits<v_float64>::vlanes() )
        ਵਾਪਸ 0;
    ਪੂਰਨ k = 0;
    v_float64 s0 = vx_setzero_f64();
    ਲਈ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_ਲੱਦੋ(a + k);
        v_float64 b0 = vx_ਲੱਦੋ(b + k);

        s0 = v_ਜੋੜੋ(s0, v_mul(a0, b0));
    }
    *ਨਤੀਜਾ = v_reduce_ਜੋੜ(s0);
    vx_cleanup();
    ਵਾਪਸ k;
}


ਸਾਂਚਾ<> ਸਤਰਅੰਦਰ ਪੂਰਨ VBLAS<ਦੁੱਗਣਾ>::givens(ਦੁੱਗਣਾ* a, ਦੁੱਗਣਾ* b, ਪੂਰਨ n, ਦੁੱਗਣਾ c, ਦੁੱਗਣਾ s) ਅਚਲ
{
    ਪੂਰਨ k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    ਲਈ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_ਲੱਦੋ(a + k);
        v_float64 b0 = vx_ਲੱਦੋ(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ਵਾਪਸ k;
}


#ਜੇ_ਅੰਤ // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#ਜੇ_ਅੰਤ // (CV_SIMD || CV_SIMD_SCALABLE)

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਖਾਲੀ
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ n1, ਦੁੱਗਣਾ minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    Auto_ਬਫਰ<ਦੁੱਗਣਾ> Wbuf(n);
    ਦੁੱਗਣਾ* W = Wbuf.ਅੰਕੜੇ();
    ਪੂਰਨ i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    ਦੁੱਗਣਾ sd;
    astep /= ਆਕਾਰ(At[0]);
    vstep /= ਆਕਾਰ(Vt[0]);

    ਲਈ( i = 0; i < n; i++ )
    {
        ਲਈ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ਦੁੱਗਣਾ)t*t;
        }
        W[i] = sd;

        ਜੇ( Vt )
        {
            ਲਈ( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    ਲਈ( iter = 0; iter < max_iter; iter++ )
    {
        ਤਾਰਕਿਕ ਬਦਲਿਆ = ਝੂਠ;

        ਲਈ( i = 0; i < n-1; i++ )
            ਲਈ( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                ਦੁੱਗਣਾ a = W[i], p = 0, b = W[j];

                ਲਈ( k = 0; k < m; k++ )
                    p += (ਦੁੱਗਣਾ)Ai[k]*Aj[k];

                ਜੇ( std::ਨਿਰਪੇਖ(p) <= eps*std::ਵਰਗਮੂਲ((ਦੁੱਗਣਾ)a*b) )
                    ਜਾਰੀ;

                p *= 2;
                ਦੁੱਗਣਾ beta = a - b, gamma = hypot((ਦੁੱਗਣਾ)p, beta);
                ਜੇ( beta < 0 )
                {
                    ਦੁੱਗਣਾ delta = (gamma - beta)*0.5;
                    s = (_Tp)std::ਵਰਗਮੂਲ(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                ਨਹੀਂਤਾਂ
                {
                    c = (_Tp)std::ਵਰਗਮੂਲ((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                ਲਈ( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (ਦੁੱਗਣਾ)t0*t0; b += (ਦੁੱਗਣਾ)t1*t1;
                }
                W[i] = a; W[j] = b;

                ਬਦਲਿਆ = ਸੱਚ;

                ਜੇ( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    ਲਈ( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        ਜੇ( !ਬਦਲਿਆ )
            ਰੋਕੋ;
    }

    ਲਈ( i = 0; i < n; i++ )
    {
        ਲਈ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ਦੁੱਗਣਾ)t*t;
        }
        W[i] = std::ਵਰਗਮੂਲ(sd);
    }

    ਲਈ( i = 0; i < n-1; i++ )
    {
        j = i;
        ਲਈ( k = i+1; k < n; k++ )
        {
            ਜੇ( W[j] < W[k] )
                j = k;
        }
        ਜੇ( i != j )
        {
            std::swap(W[i], W[j]);
            ਜੇ( Vt )
            {
                ਲਈ( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                ਲਈ( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    ਲਈ( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    ਜੇ( !Vt )
        ਵਾਪਸ;

    RNG rng(0x12345678);
    ਲਈ( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        ਲਈ( ਪੂਰਨ ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ਅਚਲ _Tp val0 = (_Tp)(1./m);
            ਲਈ( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            ਲਈ( iter = 0; iter < 2; iter++ )
            {
                ਲਈ( j = 0; j < i; j++ )
                {
                    sd = 0;
                    ਲਈ( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    ਲਈ( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::ਨਿਰਪੇਖ(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    ਲਈ( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            ਲਈ( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (ਦੁੱਗਣਾ)t*t;
            }
            sd = std::ਵਰਗਮੂਲ(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        ਲਈ( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


ਸਥਿਰ ਖਾਲੀ JacobiSVD(ਦਸ਼ਮਲਵ* At, size_t astep, ਦਸ਼ਮਲਵ* W, ਦਸ਼ਮਲਵ* Vt, size_t vstep, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ n1=-1)
{
    hal::SVD32f(At, astep, W, ਨਿਰਾ, astep, Vt, vstep, m, n, n1);
}

ਸਥਿਰ ਖਾਲੀ JacobiSVD(ਦੁੱਗਣਾ* At, size_t astep, ਦੁੱਗਣਾ* W, ਦੁੱਗਣਾ* Vt, size_t vstep, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ n1=-1)
{
    hal::SVD64f(At, astep, W, ਨਿਰਾ, astep, Vt, vstep, m, n, n1);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
decodeSVDParameters(ਅਚਲ fptype* U, ਅਚਲ fptype* Vt, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ n1)
{
    ਪੂਰਨ halSVDFlag = 0;
    ਜੇ(Vt == ਨਿਰਾ)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    ਨਹੀਂਤਾਂ ਜੇ(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        ਜੇ(U == ਨਿਰਾ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ਨਹੀਂਤਾਂ ਜੇ(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_ਭਰਿਆ_UV;
        ਜੇ(U == ਨਿਰਾ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ਵਾਪਸ halSVDFlag;
}

ਖਾਲੀ hal::SVD32f(ਦਸ਼ਮਲਵ* At, size_t astep, ਦਸ਼ਮਲਵ* W, ਦਸ਼ਮਲਵ* U, size_t ustep, ਦਸ਼ਮਲਵ* Vt, size_t vstep, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

ਖਾਲੀ hal::SVD64f(ਦੁੱਗਣਾ* At, size_t astep, ਦੁੱਗਣਾ* W, ਦੁੱਗਣਾ* U, size_t ustep, ਦੁੱਗਣਾ* Vt, size_t vstep, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2, ਕਿਸਮ_ਨਾਂ T3> ਸਥਿਰ ਖਾਲੀ
MatrAXPY( ਪੂਰਨ m, ਪੂਰਨ n, ਅਚਲ T1* x, ਪੂਰਨ dx,
         ਅਚਲ T2* a, ਪੂਰਨ inca, T3* y, ਪੂਰਨ dy )
{
    ਪੂਰਨ i;
    ਲਈ( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        ਪੂਰਨ j = 0;
         #ਜੇ CV_ENABLE_UNROLLED
        ਲਈ(; j <= n - 4; j += 4 )
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
        #ਜੇ_ਅੰਤ
        ਲਈ( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
SVBkSbImpl_( ਪੂਰਨ m, ਪੂਰਨ n, ਅਚਲ T* w, ਪੂਰਨ incw,
       ਅਚਲ T* u, ਪੂਰਨ ldu, ਤਾਰਕਿਕ uT,
       ਅਚਲ T* v, ਪੂਰਨ ldv, ਤਾਰਕਿਕ vT,
       ਅਚਲ T* b, ਪੂਰਨ ldb, ਪੂਰਨ nb,
       T* x, ਪੂਰਨ ldx, ਦੁੱਗਣਾ* ਬਫਰ, T eps )
{
    ਦੁੱਗਣਾ ਹੱਦ = 0;
    ਪੂਰਨ udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    ਪੂਰਨ vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    ਪੂਰਨ i, j, nm = std::min(m, n);

    ਜੇ( !b )
        nb = m;

    ਲਈ( i = 0; i < n; i++ )
        ਲਈ( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    ਲਈ( i = 0; i < nm; i++ )
        ਹੱਦ += w[i*incw];
    ਹੱਦ *= eps;

    // v * inv(w) * uT * b
    ਲਈ( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        ਦੁੱਗਣਾ wi = w[i*incw];
        ਜੇ( (ਦੁੱਗਣਾ)std::ਨਿਰਪੇਖ(wi) <= ਹੱਦ )
            ਜਾਰੀ;
        wi = 1/wi;

        ਜੇ( nb == 1 )
        {
            ਦੁੱਗਣਾ s = 0;
            ਜੇ( b )
                ਲਈ( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            ਨਹੀਂਤਾਂ
                s = u[0];
            s *= wi;

            ਲਈ( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ( b )
            {
                ਲਈ( j = 0; j < nb; j++ )
                    ਬਫਰ[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, ਬਫਰ, 0 );
                ਲਈ( j = 0; j < nb; j++ )
                    ਬਫਰ[j] *= wi;
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ( j = 0; j < nb; j++ )
                    ਬਫਰ[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, ਬਫਰ, 0, v, vdelta1, x, ldx );
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
SVBkSb( ਪੂਰਨ m, ਪੂਰਨ n, ਅਚਲ ਦਸ਼ਮਲਵ* w, size_t wstep,
        ਅਚਲ ਦਸ਼ਮਲਵ* u, size_t ustep, ਤਾਰਕਿਕ uT,
        ਅਚਲ ਦਸ਼ਮਲਵ* v, size_t vstep, ਤਾਰਕਿਕ vT,
        ਅਚਲ ਦਸ਼ਮਲਵ* b, size_t bstep, ਪੂਰਨ nb,
        ਦਸ਼ਮਲਵ* x, size_t xstep, uchar* ਬਫਰ )
{
    SVBkSbImpl_(m, n, w, wstep ? (ਪੂਰਨ)(wstep/ਆਕਾਰ(w[0])) : 1,
                u, (ਪੂਰਨ)(ustep/ਆਕਾਰ(u[0])), uT,
                v, (ਪੂਰਨ)(vstep/ਆਕਾਰ(v[0])), vT,
                b, (ਪੂਰਨ)(bstep/ਆਕਾਰ(b[0])), nb,
                x, (ਪੂਰਨ)(xstep/ਆਕਾਰ(x[0])),
                (ਦੁੱਗਣਾ*)alignPtr(ਬਫਰ, ਆਕਾਰ(ਦੁੱਗਣਾ)), (ਦਸ਼ਮਲਵ)(DBL_EPSILON*2) );
}

ਸਥਿਰ ਖਾਲੀ
SVBkSb( ਪੂਰਨ m, ਪੂਰਨ n, ਅਚਲ ਦੁੱਗਣਾ* w, size_t wstep,
       ਅਚਲ ਦੁੱਗਣਾ* u, size_t ustep, ਤਾਰਕਿਕ uT,
       ਅਚਲ ਦੁੱਗਣਾ* v, size_t vstep, ਤਾਰਕਿਕ vT,
       ਅਚਲ ਦੁੱਗਣਾ* b, size_t bstep, ਪੂਰਨ nb,
       ਦੁੱਗਣਾ* x, size_t xstep, uchar* ਬਫਰ )
{
    SVBkSbImpl_(m, n, w, wstep ? (ਪੂਰਨ)(wstep/ਆਕਾਰ(w[0])) : 1,
                u, (ਪੂਰਨ)(ustep/ਆਕਾਰ(u[0])), uT,
                v, (ਪੂਰਨ)(vstep/ਆਕਾਰ(v[0])), vT,
                b, (ਪੂਰਨ)(bstep/ਆਕਾਰ(b[0])), nb,
                x, (ਪੂਰਨ)(xstep/ਆਕਾਰ(x[0])),
                (ਦੁੱਗਣਾ*)alignPtr(ਬਫਰ, ਆਕਾਰ(ਦੁੱਗਣਾ)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ det2(m)   ((ਦੁੱਗਣਾ)m(0,0)*m(1,1) - (ਦੁੱਗਣਾ)m(0,1)*m(1,0))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ det3(m)   (m(0,0)*((ਦੁੱਗਣਾ)m(1,1)*m(2,2) - (ਦੁੱਗਣਾ)m(1,2)*m(2,1)) -  \
                   m(0,1)*((ਦੁੱਗਣਾ)m(1,0)*m(2,2) - (ਦੁੱਗਣਾ)m(1,2)*m(2,0)) +  \
                   m(0,2)*((ਦੁੱਗਣਾ)m(1,0)*m(2,1) - (ਦੁੱਗਣਾ)m(1,1)*m(2,0)))

ਦੁੱਗਣਾ ਨਿਰਧਾਰਕ( ਨਿਵੇਸ਼_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    ਦੁੱਗਣਾ ਨਤੀਜਾ = 0;
    ਪੂਰਨ type = mat.type(), ਪੰਕਤੀਆਂ = mat.ਪੰਕਤੀਆਂ;
    size_t ਕਦਮ = mat.ਕਦਮ;
    ਅਚਲ uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.ਪੰਕਤੀਆਂ == mat.ਖਾਨੇ_ਗਿਣਤੀ && (type == CV_32F || type == CV_64F));

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Mf(y, x) ((ਦਸ਼ਮਲਵ*)(m + y*ਕਦਮ))[x]
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Md(y, x) ((ਦੁੱਗਣਾ*)(m + y*ਕਦਮ))[x]

    ਜੇ( type == CV_32F )
    {
        ਜੇ( ਪੰਕਤੀਆਂ == 2 )
            ਨਤੀਜਾ = det2(Mf);
        ਨਹੀਂਤਾਂ ਜੇ( ਪੰਕਤੀਆਂ == 3 )
            ਨਤੀਜਾ = det3(Mf);
        ਨਹੀਂਤਾਂ ਜੇ( ਪੰਕਤੀਆਂ == 1 )
            ਨਤੀਜਾ = Mf(0,0);
        ਨਹੀਂਤਾਂ
        {
            size_t buf_ਮਾਪ = ਪੰਕਤੀਆਂ*ਪੰਕਤੀਆਂ*ਆਕਾਰ(ਦਸ਼ਮਲਵ);
            Auto_ਬਫਰ<uchar> ਬਫਰ(buf_ਮਾਪ);
            Mat a(ਪੰਕਤੀਆਂ, ਪੰਕਤੀਆਂ, CV_32F, ਬਫਰ.ਅੰਕੜੇ());
            mat.copy_ਮੰਜ਼ਿਲ(a);

            ਨਤੀਜਾ = hal::LU32f(a.ptr<ਦਸ਼ਮਲਵ>(), a.ਕਦਮ, ਪੰਕਤੀਆਂ, 0, 0, 0);
            ਜੇ( ਨਤੀਜਾ )
            {
                ਲਈ( ਪੂਰਨ i = 0; i < ਪੰਕਤੀਆਂ; i++ )
                    ਨਤੀਜਾ *= a.at<ਦਸ਼ਮਲਵ>(i,i);
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ( ਪੰਕਤੀਆਂ == 2 )
            ਨਤੀਜਾ = det2(Md);
        ਨਹੀਂਤਾਂ ਜੇ( ਪੰਕਤੀਆਂ == 3 )
            ਨਤੀਜਾ = det3(Md);
        ਨਹੀਂਤਾਂ ਜੇ( ਪੰਕਤੀਆਂ == 1 )
            ਨਤੀਜਾ = Md(0,0);
        ਨਹੀਂਤਾਂ
        {
            size_t buf_ਮਾਪ = ਪੰਕਤੀਆਂ*ਪੰਕਤੀਆਂ*ਆਕਾਰ(ਦੁੱਗਣਾ);
            Auto_ਬਫਰ<uchar> ਬਫਰ(buf_ਮਾਪ);
            Mat a(ਪੰਕਤੀਆਂ, ਪੰਕਤੀਆਂ, CV_64F, ਬਫਰ.ਅੰਕੜੇ());
            mat.copy_ਮੰਜ਼ਿਲ(a);

            ਨਤੀਜਾ = hal::LU64f(a.ptr<ਦੁੱਗਣਾ>(), a.ਕਦਮ, ਪੰਕਤੀਆਂ, 0, 0, 0);
            ਜੇ( ਨਤੀਜਾ )
            {
                ਲਈ( ਪੂਰਨ i = 0; i < ਪੰਕਤੀਆਂ; i++ )
                    ਨਤੀਜਾ *= a.at<ਦੁੱਗਣਾ>(i,i);
            }
        }
    }

    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ Mf
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ Md

    ਵਾਪਸ ਨਤੀਜਾ;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Sf( y, x ) ((ਦਸ਼ਮਲਵ*)(srcdata + y*srcstep))[x]
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Sd( y, x ) ((ਦੁੱਗਣਾ*)(srcdata + y*srcstep))[x]
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Df( y, x ) ((ਦਸ਼ਮਲਵ*)(dstdata + y*dststep))[x]
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Dd( y, x ) ((ਦੁੱਗਣਾ*)(dstdata + y*dststep))[x]

ਦੁੱਗਣਾ invert( ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ ਵਿਧੀ )
{
    CV_INSTRUMENT_REGION();

    ਤਾਰਕਿਕ ਨਤੀਜਾ = ਝੂਠ;
    Mat src = _src.getMat();
    ਪੂਰਨ type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_ਮਾਪ(type);
    ਪੂਰਨ m = src.ਪੰਕਤੀਆਂ, n = src.ਖਾਨੇ_ਗਿਣਤੀ;

    ਜੇ( ਵਿਧੀ == DECOMP_SVD )
    {
        ਪੂਰਨ nm = std::min(m, n);

        Auto_ਬਫਰ<uchar> _buf((m*nm + nm + nm*n)*esz + ਆਕਾਰ(ਦੁੱਗਣਾ));
        uchar* buf = alignPtr((uchar*)_buf.ਅੰਕੜੇ(), (ਪੂਰਨ)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::ਪਿੱਠ_Subst(w, u, vt, Mat(), _dst);
        ਵਾਪਸ type == CV_32F ?
            (w.ptr<ਦਸ਼ਮਲਵ>()[0] >= FLT_EPSILON ?
             w.ptr<ਦਸ਼ਮਲਵ>()[n-1]/w.ptr<ਦਸ਼ਮਲਵ>()[0] : 0) :
            (w.ptr<ਦੁੱਗਣਾ>()[0] >= DBL_EPSILON ?
             w.ptr<ਦੁੱਗਣਾ>()[n-1]/w.ptr<ਦੁੱਗਣਾ>()[0] : 0);
    }

    CV_Assert( m == n );

    ਜੇ( ਵਿਧੀ == DECOMP_EIG )
    {
        Auto_ਬਫਰ<uchar> _buf((n*n*2 + n)*esz + ਆਕਾਰ(ਦੁੱਗਣਾ));
        uchar* buf = alignPtr((uchar*)_buf.ਅੰਕੜੇ(), (ਪੂਰਨ)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::ਪਿੱਠ_Subst(w, u, vt, Mat(), _dst);
        ਵਾਪਸ type == CV_32F ?
        (w.ptr<ਦਸ਼ਮਲਵ>()[0] >= FLT_EPSILON ?
         w.ptr<ਦਸ਼ਮਲਵ>()[n-1]/w.ptr<ਦਸ਼ਮਲਵ>()[0] : 0) :
        (w.ptr<ਦੁੱਗਣਾ>()[0] >= DBL_EPSILON ?
         w.ptr<ਦੁੱਗਣਾ>()[n-1]/w.ptr<ਦੁੱਗਣਾ>()[0] : 0);
    }

    CV_Assert( ਵਿਧੀ == DECOMP_LU || ਵਿਧੀ == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    ਜੇ( n <= 3 )
    {
        ਅਚਲ uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.ਕਦਮ;
        size_t dststep = dst.ਕਦਮ;

        ਜੇ( n == 2 )
        {
            ਜੇ( type == CV_32FC1 )
            {
                ਦੁੱਗਣਾ d = det2(Sf);
                ਜੇ( d != 0. )
                {
                    ਨਤੀਜਾ = ਸੱਚ;
                    d = 1./d;
                #ਜੇ CV_SIMD128
                    ਅਚਲ ਦਸ਼ਮਲਵ d_32f = (ਦਸ਼ਮਲਵ)d;
                    ਅਚਲ v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_ਲੱਦੋ_halves((ਅਚਲ ਦਸ਼ਮਲਵ *)srcdata, (ਅਚਲ ਦਸ਼ਮਲਵ *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_ਕੱਢੋ<3>(s0, v_combine_ਨੀਵਾਂ(v_rotate_ਸੱਜਾ<1>(s0), s0));
                    v_store_ਨੀਵਾਂ((ਦਸ਼ਮਲਵ*)dstdata, s0);
                    v_store_ਉੱਚਾ((ਦਸ਼ਮਲਵ*)(dstdata + dststep), s0);
                #ਨਹੀਂਤਾਂ
                    ਦੁੱਗਣਾ t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (ਦਸ਼ਮਲਵ)t0;
                    Df(0,0) = (ਦਸ਼ਮਲਵ)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (ਦਸ਼ਮਲਵ)t0;
                    Df(1,0) = (ਦਸ਼ਮਲਵ)t1;
                #ਜੇ_ਅੰਤ
                }
            }
            ਨਹੀਂਤਾਂ
            {
                ਦੁੱਗਣਾ d = det2(Sd);
                ਜੇ( d != 0. )
                {
                    ਨਤੀਜਾ = ਸੱਚ;
                    d = 1./d;
                #ਜੇ CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_ਲੱਦੋ((ਅਚਲ ਦੁੱਗਣਾ *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_ਲੱਦੋ((ਅਚਲ ਦੁੱਗਣਾ *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_ਕੱਢੋ<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<ਦੁੱਗਣਾ>(0), v_ਕੱਢੋ<1>(s0, s1));//12
                    v_store((ਦੁੱਗਣਾ*)dstdata, v_combine_ਨੀਵਾਂ(sm, ss));//31
                    v_store((ਦੁੱਗਣਾ*)(dstdata + dststep), v_combine_ਉੱਚਾ(ss, sm));//20
                #ਨਹੀਂਤਾਂ
                    ਦੁੱਗਣਾ t0, t1;
                    t0 = Sd(0,0)*d;
                    t1 = Sd(1,1)*d;
                    Dd(1,1) = t0;
                    Dd(0,0) = t1;
                    t0 = -Sd(0,1)*d;
                    t1 = -Sd(1,0)*d;
                    Dd(0,1) = t0;
                    Dd(1,0) = t1;
                #ਜੇ_ਅੰਤ
                }
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ( n == 3 )
        {
            ਜੇ( type == CV_32FC1 )
            {
                ਦੁੱਗਣਾ d = det3(Sf);

                ਜੇ( d != 0. )
                {
                    ਦੁੱਗਣਾ t[12];

                    ਨਤੀਜਾ = ਸੱਚ;
                    d = 1./d;
                    t[0] = (((ਦੁੱਗਣਾ)Sf(1,1) * Sf(2,2) - (ਦੁੱਗਣਾ)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((ਦੁੱਗਣਾ)Sf(0,2) * Sf(2,1) - (ਦੁੱਗਣਾ)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((ਦੁੱਗਣਾ)Sf(0,1) * Sf(1,2) - (ਦੁੱਗਣਾ)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((ਦੁੱਗਣਾ)Sf(1,2) * Sf(2,0) - (ਦੁੱਗਣਾ)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((ਦੁੱਗਣਾ)Sf(0,0) * Sf(2,2) - (ਦੁੱਗਣਾ)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((ਦੁੱਗਣਾ)Sf(0,2) * Sf(1,0) - (ਦੁੱਗਣਾ)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((ਦੁੱਗਣਾ)Sf(1,0) * Sf(2,1) - (ਦੁੱਗਣਾ)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((ਦੁੱਗਣਾ)Sf(0,1) * Sf(2,0) - (ਦੁੱਗਣਾ)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((ਦੁੱਗਣਾ)Sf(0,0) * Sf(1,1) - (ਦੁੱਗਣਾ)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (ਦਸ਼ਮਲਵ)t[0]; Df(0,1) = (ਦਸ਼ਮਲਵ)t[1]; Df(0,2) = (ਦਸ਼ਮਲਵ)t[2];
                    Df(1,0) = (ਦਸ਼ਮਲਵ)t[3]; Df(1,1) = (ਦਸ਼ਮਲਵ)t[4]; Df(1,2) = (ਦਸ਼ਮਲਵ)t[5];
                    Df(2,0) = (ਦਸ਼ਮਲਵ)t[6]; Df(2,1) = (ਦਸ਼ਮਲਵ)t[7]; Df(2,2) = (ਦਸ਼ਮਲਵ)t[8];
                }
            }
            ਨਹੀਂਤਾਂ
            {
                ਦੁੱਗਣਾ d = det3(Sd);
                ਜੇ( d != 0. )
                {
                    ਨਤੀਜਾ = ਸੱਚ;
                    d = 1./d;
                    ਦੁੱਗਣਾ t[9];

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
        ਨਹੀਂਤਾਂ
        {
            CV_Assert( n == 1 );

            ਜੇ( type == CV_32FC1 )
            {
                ਦੁੱਗਣਾ d = Sf(0,0);
                ਜੇ( d != 0. )
                {
                    ਨਤੀਜਾ = ਸੱਚ;
                    Df(0,0) = (ਦਸ਼ਮਲਵ)(1./d);
                }
            }
            ਨਹੀਂਤਾਂ
            {
                ਦੁੱਗਣਾ d = Sd(0,0);
                ਜੇ( d != 0. )
                {
                    ਨਤੀਜਾ = ਸੱਚ;
                    Dd(0,0) = 1./d;
                }
            }
        }
        ਜੇ( !ਨਤੀਜਾ )
            dst = Scalar(0);
        ਵਾਪਸ ਨਤੀਜਾ;
    }

   ਪੂਰਨ elem_ਮਾਪ_2 = CV_ELEM_ਮਾਪ(type);
    Auto_ਬਫਰ<uchar> buf(n*n*elem_ਮਾਪ_2);
    Mat src1(n, n, type, buf.ਅੰਕੜੇ());
    src.copy_ਮੰਜ਼ਿਲ(src1);
    ਸੈੱਟ_ਪਛਾਣ_ਤੱਤ(dst);

    ਜੇ( ਵਿਧੀ == DECOMP_LU && type == CV_32F )
        ਨਤੀਜਾ = hal::LU32f(src1.ptr<ਦਸ਼ਮਲਵ>(), src1.ਕਦਮ, n, dst.ptr<ਦਸ਼ਮਲਵ>(), dst.ਕਦਮ, n) != 0;
    ਨਹੀਂਤਾਂ ਜੇ( ਵਿਧੀ == DECOMP_LU && type == CV_64F )
        ਨਤੀਜਾ = hal::LU64f(src1.ptr<ਦੁੱਗਣਾ>(), src1.ਕਦਮ, n, dst.ptr<ਦੁੱਗਣਾ>(), dst.ਕਦਮ, n) != 0;
    ਨਹੀਂਤਾਂ ਜੇ( ਵਿਧੀ == DECOMP_CHOLESKY && type == CV_32F )
        ਨਤੀਜਾ = hal::Cholesky32f(src1.ptr<ਦਸ਼ਮਲਵ>(), src1.ਕਦਮ, n, dst.ptr<ਦਸ਼ਮਲਵ>(), dst.ਕਦਮ, n);
    ਨਹੀਂਤਾਂ
        ਨਤੀਜਾ = hal::Cholesky64f(src1.ptr<ਦੁੱਗਣਾ>(), src1.ਕਦਮ, n, dst.ptr<ਦੁੱਗਣਾ>(), dst.ਕਦਮ, n);

    ਜੇ( !ਨਤੀਜਾ )
        dst = Scalar(0);

    ਵਾਪਸ ਨਤੀਜਾ;
}

UMat UMat::inv(ਪੂਰਨ ਵਿਧੀ) ਅਚਲ
{
    UMat m;
    invert(*ਇਹ, m, ਵਿਧੀ);
    ਵਾਪਸ m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

ਤਾਰਕਿਕ solve( ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Array _src2arg, OutputArray _dst, ਪੂਰਨ ਵਿਧੀ )
{
    CV_INSTRUMENT_REGION();

    ਤਾਰਕਿਕ ਨਤੀਜਾ = ਸੱਚ;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    ਪੂਰਨ type = src.type();
    ਤਾਰਕਿਕ is_ਸਾਧਾਰਨ = (ਵਿਧੀ & DECOMP_ਸਾਧਾਰਨ) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    ਵਿਧੀ &= ~DECOMP_ਸਾਧਾਰਨ;
    CV_Check(ਵਿਧੀ, ਵਿਧੀ == DECOMP_LU || ਵਿਧੀ == DECOMP_SVD || ਵਿਧੀ == DECOMP_EIG ||
                     ਵਿਧੀ == DECOMP_CHOLESKY || ਵਿਧੀ == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (ਵਿਧੀ != DECOMP_LU && ਵਿਧੀ != DECOMP_CHOLESKY) ||
        is_ਸਾਧਾਰਨ || src.ਪੰਕਤੀਆਂ == src.ਖਾਨੇ_ਗਿਣਤੀ );

    // check case of a single equation and small matrix
    ਜੇ( (ਵਿਧੀ == DECOMP_LU || ਵਿਧੀ == DECOMP_CHOLESKY) && !is_ਸਾਧਾਰਨ &&
        src.ਪੰਕਤੀਆਂ <= 3 && src.ਪੰਕਤੀਆਂ == src.ਖਾਨੇ_ਗਿਣਤੀ && _src2.ਖਾਨੇ_ਗਿਣਤੀ == 1 )
    {
        _dst.create( src.ਖਾਨੇ_ਗਿਣਤੀ, _src2.ਖਾਨੇ_ਗਿਣਤੀ, src.type() );
        Mat dst = _dst.getMat();

        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ bf(y) ((ਦਸ਼ਮਲਵ*)(bdata + y*src2step))[0]
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ bd(y) ((ਦੁੱਗਣਾ*)(bdata + y*src2step))[0]

        ਅਚਲ uchar* srcdata = src.ptr();
        ਅਚਲ uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.ਕਦਮ;
        size_t src2step = _src2.ਕਦਮ;
        size_t dststep = dst.ਕਦਮ;

        ਜੇ( src.ਪੰਕਤੀਆਂ == 2 )
        {
            ਜੇ( type == CV_32FC1 )
            {
                ਦੁੱਗਣਾ d = det2(Sf);
                ਜੇ( d != 0. )
                {
                    ਦੁੱਗਣਾ t;
                    d = 1./d;
                    t = (ਦਸ਼ਮਲਵ)(((ਦੁੱਗਣਾ)bf(0)*Sf(1,1) - (ਦੁੱਗਣਾ)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (ਦਸ਼ਮਲਵ)(((ਦੁੱਗਣਾ)bf(1)*Sf(0,0) - (ਦੁੱਗਣਾ)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (ਦਸ਼ਮਲਵ)t;
                }
                ਨਹੀਂਤਾਂ
                    ਨਤੀਜਾ = ਝੂਠ;
            }
            ਨਹੀਂਤਾਂ
            {
                ਦੁੱਗਣਾ d = det2(Sd);
                ਜੇ( d != 0. )
                {
                    ਦੁੱਗਣਾ t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                ਨਹੀਂਤਾਂ
                    ਨਤੀਜਾ = ਝੂਠ;
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ( src.ਪੰਕਤੀਆਂ == 3 )
        {
            ਜੇ( type == CV_32FC1 )
            {
                ਦੁੱਗਣਾ d = det3(Sf);
                ਜੇ( d != 0. )
                {
                    ਦਸ਼ਮਲਵ t[3];
                    d = 1./d;

                    t[0] = (ਦਸ਼ਮਲਵ)(d*
                           (bf(0)*((ਦੁੱਗਣਾ)Sf(1,1)*Sf(2,2) - (ਦੁੱਗਣਾ)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((ਦੁੱਗਣਾ)bf(1)*Sf(2,2) - (ਦੁੱਗਣਾ)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((ਦੁੱਗਣਾ)bf(1)*Sf(2,1) - (ਦੁੱਗਣਾ)Sf(1,1)*bf(2))));

                    t[1] = (ਦਸ਼ਮਲਵ)(d*
                           (Sf(0,0)*(ਦੁੱਗਣਾ)(bf(1)*Sf(2,2) - (ਦੁੱਗਣਾ)Sf(1,2)*bf(2)) -
                            bf(0)*((ਦੁੱਗਣਾ)Sf(1,0)*Sf(2,2) - (ਦੁੱਗਣਾ)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((ਦੁੱਗਣਾ)Sf(1,0)*bf(2) - (ਦੁੱਗਣਾ)bf(1)*Sf(2,0))));

                    t[2] = (ਦਸ਼ਮਲਵ)(d*
                           (Sf(0,0)*((ਦੁੱਗਣਾ)Sf(1,1)*bf(2) - (ਦੁੱਗਣਾ)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((ਦੁੱਗਣਾ)Sf(1,0)*bf(2) - (ਦੁੱਗਣਾ)bf(1)*Sf(2,0)) +
                            bf(0)*((ਦੁੱਗਣਾ)Sf(1,0)*Sf(2,1) - (ਦੁੱਗਣਾ)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                ਨਹੀਂਤਾਂ
                    ਨਤੀਜਾ = ਝੂਠ;
            }
            ਨਹੀਂਤਾਂ
            {
                ਦੁੱਗਣਾ d = det3(Sd);
                ਜੇ( d != 0. )
                {
                    ਦੁੱਗਣਾ t[9];

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
                ਨਹੀਂਤਾਂ
                    ਨਤੀਜਾ = ਝੂਠ;
            }
        }
        ਨਹੀਂਤਾਂ
        {
            CV_Assert( src.ਪੰਕਤੀਆਂ == 1 );

            ਜੇ( type == CV_32FC1 )
            {
                ਦੁੱਗਣਾ d = Sf(0,0);
                ਜੇ( d != 0. )
                    Df(0,0) = (ਦਸ਼ਮਲਵ)(bf(0)/d);
                ਨਹੀਂਤਾਂ
                    ਨਤੀਜਾ = ਝੂਠ;
            }
            ਨਹੀਂਤਾਂ
            {
                ਦੁੱਗਣਾ d = Sd(0,0);
                ਜੇ( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                ਨਹੀਂਤਾਂ
                    ਨਤੀਜਾ = ਝੂਠ;
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਪੂਰਨ m = src.ਪੰਕਤੀਆਂ, m_ = m, n = src.ਖਾਨੇ_ਗਿਣਤੀ, nb = _src2.ਖਾਨੇ_ਗਿਣਤੀ;
    size_t esz = CV_ELEM_ਮਾਪ(type), bufsize = 0;
    size_t vstep = align_ਮਾਪ(n*esz, 16);
    size_t astep = ਵਿਧੀ == DECOMP_SVD && !is_ਸਾਧਾਰਨ ? align_ਮਾਪ(m*esz, 16) : vstep;
    Auto_ਬਫਰ<uchar> ਬਫਰ;

    Mat src2 = _src2;
    _dst.create( src.ਖਾਨੇ_ਗਿਣਤੀ, src2.ਖਾਨੇ_ਗਿਣਤੀ, src.type() );
    Mat dst = _dst.getMat();

    ਜੇ( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    ਜੇ( m == n )
        is_ਸਾਧਾਰਨ = ਝੂਠ;
    ਨਹੀਂਤਾਂ ਜੇ( is_ਸਾਧਾਰਨ )
    {
        m_ = n;
        ਜੇ( ਵਿਧੀ == DECOMP_SVD )
            ਵਿਧੀ = DECOMP_EIG;
    }

    CV_Assert(m == src2.ਪੰਕਤੀਆਂ);
    size_t asize = astep*(ਵਿਧੀ == DECOMP_SVD || is_ਸਾਧਾਰਨ ? n : m);
    bufsize += asize + 32;

    ਜੇ( is_ਸਾਧਾਰਨ )
        bufsize += n*nb*esz;
    ਜੇ( ਵਿਧੀ == DECOMP_SVD || ਵਿਧੀ == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*ਆਕਾਰ(ਦੁੱਗਣਾ) + 32;

    ਬਫਰ.allocate(bufsize);
    uchar* ptr = alignPtr(ਬਫਰ.ਅੰਕੜੇ(), 16);

    Mat a(m_, n, type, ptr, astep);

    ਜੇ( is_ਸਾਧਾਰਨ )
        mulTransposed(src, a, ਸੱਚ);
    ਨਹੀਂਤਾਂ ਜੇ( ਵਿਧੀ != DECOMP_SVD )
        src.copy_ਮੰਜ਼ਿਲ(a);
    ਨਹੀਂਤਾਂ
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    ਜੇ( !is_ਸਾਧਾਰਨ )
    {
        ਜੇ( ਵਿਧੀ == DECOMP_LU || ਵਿਧੀ == DECOMP_CHOLESKY )
            src2.copy_ਮੰਜ਼ਿਲ(dst);
    }
    ਨਹੀਂਤਾਂ
    {
        // a'*b
        ਜੇ( ਵਿਧੀ == DECOMP_LU || ਵਿਧੀ == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        ਨਹੀਂਤਾਂ
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    ਜੇ( ਵਿਧੀ == DECOMP_LU )
    {
        ਜੇ( type == CV_32F )
            ਨਤੀਜਾ = hal::LU32f(a.ptr<ਦਸ਼ਮਲਵ>(), a.ਕਦਮ, n, dst.ptr<ਦਸ਼ਮਲਵ>(), dst.ਕਦਮ, nb) != 0;
        ਨਹੀਂਤਾਂ
            ਨਤੀਜਾ = hal::LU64f(a.ptr<ਦੁੱਗਣਾ>(), a.ਕਦਮ, n, dst.ptr<ਦੁੱਗਣਾ>(), dst.ਕਦਮ, nb) != 0;
    }
    ਨਹੀਂਤਾਂ ਜੇ( ਵਿਧੀ == DECOMP_CHOLESKY )
    {
        ਜੇ( type == CV_32F )
            ਨਤੀਜਾ = hal::Cholesky32f(a.ptr<ਦਸ਼ਮਲਵ>(), a.ਕਦਮ, n, dst.ptr<ਦਸ਼ਮਲਵ>(), dst.ਕਦਮ, nb);
        ਨਹੀਂਤਾਂ
            ਨਤੀਜਾ = hal::Cholesky64f(a.ptr<ਦੁੱਗਣਾ>(), a.ਕਦਮ, n, dst.ptr<ਦੁੱਗਣਾ>(), dst.ਕਦਮ, nb);
    }
    ਨਹੀਂਤਾਂ ਜੇ( ਵਿਧੀ == DECOMP_QR )
    {
        Mat rhsMat;
        ਜੇ( is_ਸਾਧਾਰਨ || m == n )
        {
            src2.copy_ਮੰਜ਼ਿਲ(dst);
            rhsMat = dst;
        }
        ਨਹੀਂਤਾਂ
        {
            rhsMat = Mat(m, nb, type);
            src2.copy_ਮੰਜ਼ਿਲ(rhsMat);
        }

        ਜੇ( type == CV_32F )
            ਨਤੀਜਾ = hal::QR32f(a.ptr<ਦਸ਼ਮਲਵ>(), a.ਕਦਮ, a.ਪੰਕਤੀਆਂ, a.ਖਾਨੇ_ਗਿਣਤੀ, rhsMat.ਖਾਨੇ_ਗਿਣਤੀ, rhsMat.ptr<ਦਸ਼ਮਲਵ>(), rhsMat.ਕਦਮ, ਨਿਰਾ) != 0;
        ਨਹੀਂਤਾਂ
            ਨਤੀਜਾ = hal::QR64f(a.ptr<ਦੁੱਗਣਾ>(), a.ਕਦਮ, a.ਪੰਕਤੀਆਂ, a.ਖਾਨੇ_ਗਿਣਤੀ, rhsMat.ਖਾਨੇ_ਗਿਣਤੀ, rhsMat.ptr<ਦੁੱਗਣਾ>(), rhsMat.ਕਦਮ, ਨਿਰਾ) != 0;

        ਜੇ (rhsMat.ਪੰਕਤੀਆਂ != dst.ਪੰਕਤੀਆਂ)
            rhsMat.ਪੰਕਤੀ_Range_2(0, dst.ਪੰਕਤੀਆਂ).copy_ਮੰਜ਼ਿਲ(dst);
    }
    ਨਹੀਂਤਾਂ
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        ਜੇ( ਵਿਧੀ == DECOMP_EIG )
        {
            ਜੇ( type == CV_32F )
                Jacobi(a.ptr<ਦਸ਼ਮਲਵ>(), a.ਕਦਮ, w.ptr<ਦਸ਼ਮਲਵ>(), v.ptr<ਦਸ਼ਮਲਵ>(), v.ਕਦਮ, n, ptr);
            ਨਹੀਂਤਾਂ
                Jacobi(a.ptr<ਦੁੱਗਣਾ>(), a.ਕਦਮ, w.ptr<ਦੁੱਗਣਾ>(), v.ptr<ਦੁੱਗਣਾ>(), v.ਕਦਮ, n, ptr);
            u = v;
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ( type == CV_32F )
                JacobiSVD(a.ptr<ਦਸ਼ਮਲਵ>(), a.ਕਦਮ, w.ptr<ਦਸ਼ਮਲਵ>(), v.ptr<ਦਸ਼ਮਲਵ>(), v.ਕਦਮ, m_, n);
            ਨਹੀਂਤਾਂ
                JacobiSVD(a.ptr<ਦੁੱਗਣਾ>(), a.ਕਦਮ, w.ptr<ਦੁੱਗਣਾ>(), v.ptr<ਦੁੱਗਣਾ>(), v.ਕਦਮ, m_, n);
            u = a;
        }

        ਜੇ( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<ਦਸ਼ਮਲਵ>(), 0, u.ptr<ਦਸ਼ਮਲਵ>(), u.ਕਦਮ, ਸੱਚ,
                   v.ptr<ਦਸ਼ਮਲਵ>(), v.ਕਦਮ, ਸੱਚ, src2.ptr<ਦਸ਼ਮਲਵ>(),
                   src2.ਕਦਮ, nb, dst.ptr<ਦਸ਼ਮਲਵ>(), dst.ਕਦਮ, ptr);
        }
        ਨਹੀਂਤਾਂ
        {
            SVBkSb(m_, n, w.ptr<ਦੁੱਗਣਾ>(), 0, u.ptr<ਦੁੱਗਣਾ>(), u.ਕਦਮ, ਸੱਚ,
                   v.ptr<ਦੁੱਗਣਾ>(), v.ਕਦਮ, ਸੱਚ, src2.ptr<ਦੁੱਗਣਾ>(),
                   src2.ਕਦਮ, nb, dst.ptr<ਦੁੱਗਣਾ>(), dst.ਕਦਮ, ptr);
        }
        ਨਤੀਜਾ = ਸੱਚ;
    }

    ਜੇ( !ਨਤੀਜਾ )
        dst = Scalar(0);

    ਵਾਪਸ ਨਤੀਜਾ;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

ਤਾਰਕਿਕ eigen( ਨਿਵੇਸ਼_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ਪੂਰਨ type = src.type();
    ਪੂਰਨ n = src.ਪੰਕਤੀਆਂ;

    CV_Assert( src.ਪੰਕਤੀਆਂ == src.ਖਾਨੇ_ਗਿਣਤੀ );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    ਜੇ( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EIGEN
    ਅਚਲ ਤਾਰਕਿਕ evecNeeded = _evects.needed();
    ਅਚਲ ਪੂਰਨ esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    ਜੇ ( type == CV_64F )
    {
        Eigen::ਮੈਟ੍ਰਿਕਸ_Xd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::ਮੈਟ੍ਰਿਕਸ_Xd> es;
        es.compute(src_eig, esOptions);
        ਜੇ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ਜੇ ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ਵਾਪਸ ਸੱਚ;
        }
    } ਨਹੀਂਤਾਂ { // CV_32F
        Eigen::ਮੈਟ੍ਰਿਕਸ_Xf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::ਮੈਟ੍ਰਿਕਸ_Xf> es;
        es.compute(src_eig, esOptions);
        ਜੇ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ਜੇ ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ਵਾਪਸ ਸੱਚ;
        }
    }
    ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ

    size_t elem_ਮਾਪ = src.elem_ਮਾਪ(), astep = align_ਮਾਪ(n*elem_ਮਾਪ, 16);
    Auto_ਬਫਰ<uchar> buf(n*astep + n*5*elem_ਮਾਪ + 32);
    uchar* ptr = alignPtr(buf.ਅੰਕੜੇ(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elem_ਮਾਪ*n;
    src.copy_ਮੰਜ਼ਿਲ(a);
    ਤਾਰਕਿਕ ok = type == CV_32F ?
        Jacobi(a.ptr<ਦਸ਼ਮਲਵ>(), a.ਕਦਮ, w.ptr<ਦਸ਼ਮਲਵ>(), v.ptr<ਦਸ਼ਮਲਵ>(), v.ਕਦਮ, n, ptr) :
        Jacobi(a.ptr<ਦੁੱਗਣਾ>(), a.ਕਦਮ, w.ptr<ਦੁੱਗਣਾ>(), v.ptr<ਦੁੱਗਣਾ>(), v.ਕਦਮ, n, ptr);

    w.copy_ਮੰਜ਼ਿਲ(_evals);
    ਵਾਪਸ ok;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ _SVDcompute( ਨਿਵੇਸ਼_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, ਪੂਰਨ flags )
{
    Mat src = _aarr.getMat();
    ਪੂਰਨ m = src.ਪੰਕਤੀਆਂ, n = src.ਖਾਨੇ_ਗਿਣਤੀ;
    ਪੂਰਨ type = src.type();
    ਤਾਰਕਿਕ compute_uv = _u.needed() || _vt.needed();
    ਤਾਰਕਿਕ ਭਰਿਆ_uv = (flags & SVD::ਭਰਿਆ_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    ਜੇ( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = ਭਰਿਆ_uv = ਝੂਠ;
    }

    ਤਾਰਕਿਕ at = ਝੂਠ;
    ਜੇ( m < n )
    {
        std::swap(m, n);
        at = ਸੱਚ;
    }

    ਪੂਰਨ urows = ਭਰਿਆ_uv ? m : n;
    size_t esz = src.elem_ਮਾਪ(), astep = align_ਮਾਪ(m*esz, 16), vstep = align_ਮਾਪ(n*esz, 16);
    Auto_ਬਫਰ<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.ਅੰਕੜੇ(), 16);
    Mat ਆਰਜ਼ੀ_a(n, m, type, buf, astep);
    Mat ਆਰਜ਼ੀ_w(n, 1, type, buf + urows*astep);
    Mat ਆਰਜ਼ੀ_u(urows, m, type, buf, astep), ਆਰਜ਼ੀ_v;

    ਜੇ( compute_uv )
        ਆਰਜ਼ੀ_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    ਜੇ( urows > n )
        ਆਰਜ਼ੀ_u = Scalar::all(0);

    ਜੇ( !at )
        transpose(src, ਆਰਜ਼ੀ_a);
    ਨਹੀਂਤਾਂ
        src.copy_ਮੰਜ਼ਿਲ(ਆਰਜ਼ੀ_a);

    ਜੇ( type == CV_32F )
    {
        JacobiSVD(ਆਰਜ਼ੀ_a.ptr<ਦਸ਼ਮਲਵ>(), ਆਰਜ਼ੀ_u.ਕਦਮ, ਆਰਜ਼ੀ_w.ptr<ਦਸ਼ਮਲਵ>(),
              ਆਰਜ਼ੀ_v.ptr<ਦਸ਼ਮਲਵ>(), ਆਰਜ਼ੀ_v.ਕਦਮ, m, n, compute_uv ? urows : 0);
    }
    ਨਹੀਂਤਾਂ
    {
        JacobiSVD(ਆਰਜ਼ੀ_a.ptr<ਦੁੱਗਣਾ>(), ਆਰਜ਼ੀ_u.ਕਦਮ, ਆਰਜ਼ੀ_w.ptr<ਦੁੱਗਣਾ>(),
              ਆਰਜ਼ੀ_v.ptr<ਦੁੱਗਣਾ>(), ਆਰਜ਼ੀ_v.ਕਦਮ, m, n, compute_uv ? urows : 0);
    }
    ਆਰਜ਼ੀ_w.copy_ਮੰਜ਼ਿਲ(_w);
    ਜੇ( compute_uv )
    {
        ਜੇ( !at )
        {
            ਜੇ( _u.needed() )
                transpose(ਆਰਜ਼ੀ_u, _u);
            ਜੇ( _vt.needed() )
                ਆਰਜ਼ੀ_v.copy_ਮੰਜ਼ਿਲ(_vt);
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ( _u.needed() )
                transpose(ਆਰਜ਼ੀ_v, _u);
            ਜੇ( _vt.needed() )
                ਆਰਜ਼ੀ_u.copy_ਮੰਜ਼ਿਲ(_vt);
        }
    }
}


ਖਾਲੀ SVD::compute( ਨਿਵੇਸ਼_Array a, OutputArray w, OutputArray u, OutputArray vt, ਪੂਰਨ flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

ਖਾਲੀ SVD::compute( ਨਿਵੇਸ਼_Array a, OutputArray w, ਪੂਰਨ flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

ਖਾਲੀ SVD::ਪਿੱਠ_Subst( ਨਿਵੇਸ਼_Array _w, ਨਿਵੇਸ਼_Array _u, ਨਿਵੇਸ਼_Array _vt,
                     ਨਿਵੇਸ਼_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    ਪੂਰਨ type = w.type(), esz = (ਪੂਰਨ)w.elem_ਮਾਪ();
    ਪੂਰਨ m = u.ਪੰਕਤੀਆਂ, n = vt.ਖਾਨੇ_ਗਿਣਤੀ, nb = rhs.ਅੰਕੜੇ ? rhs.ਖਾਨੇ_ਗਿਣਤੀ : m, nm = std::min(m, n);
    size_t wstep = w.ਪੰਕਤੀਆਂ == 1 ? (size_t)esz : w.ਖਾਨੇ_ਗਿਣਤੀ == 1 ? (size_t)w.ਕਦਮ : (size_t)w.ਕਦਮ + esz;
    Auto_ਬਫਰ<uchar> ਬਫਰ(nb*ਆਕਾਰ(ਦੁੱਗਣਾ) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.ਅੰਕੜੇ && vt.ਅੰਕੜੇ && w.ਅੰਕੜੇ );
    CV_Assert( u.ਖਾਨੇ_ਗਿਣਤੀ >= nm && vt.ਪੰਕਤੀਆਂ >= nm &&
              (w.size() == ਮਾਪ(nm, 1) || w.size() == ਮਾਪ(1, nm) || w.size() == ਮਾਪ(vt.ਪੰਕਤੀਆਂ, u.ਖਾਨੇ_ਗਿਣਤੀ)) );
    CV_Assert( rhs.ਅੰਕੜੇ == 0 || (rhs.type() == type && rhs.ਪੰਕਤੀਆਂ == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    ਜੇ( type == CV_32F )
        SVBkSb(m, n, w.ptr<ਦਸ਼ਮਲਵ>(), wstep, u.ptr<ਦਸ਼ਮਲਵ>(), u.ਕਦਮ, ਝੂਠ,
               vt.ptr<ਦਸ਼ਮਲਵ>(), vt.ਕਦਮ, ਸੱਚ, rhs.ptr<ਦਸ਼ਮਲਵ>(), rhs.ਕਦਮ, nb,
               dst.ptr<ਦਸ਼ਮਲਵ>(), dst.ਕਦਮ, ਬਫਰ.ਅੰਕੜੇ());
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_64F )
        SVBkSb(m, n, w.ptr<ਦੁੱਗਣਾ>(), wstep, u.ptr<ਦੁੱਗਣਾ>(), u.ਕਦਮ, ਝੂਠ,
               vt.ptr<ਦੁੱਗਣਾ>(), vt.ਕਦਮ, ਸੱਚ, rhs.ptr<ਦੁੱਗਣਾ>(), rhs.ਕਦਮ, nb,
               dst.ptr<ਦੁੱਗਣਾ>(), dst.ਕਦਮ, ਬਫਰ.ਅੰਕੜੇ());
    ਨਹੀਂਤਾਂ
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::ਸੰਚਾਲਕ ()(ਨਿਵੇਸ਼_Array a, ਪੂਰਨ flags)
{
    _SVDcompute(a, w, u, vt, flags);
    ਵਾਪਸ *ਇਹ;
}


ਖਾਲੀ SVD::ਪਿੱਠ_Subst( ਨਿਵੇਸ਼_Array rhs, OutputArray dst ) ਅਚਲ
{
    ਪਿੱਠ_Subst( w, u, vt, rhs, dst );
}

}


ਖਾਲੀ cv::SVDecomp(ਨਿਵੇਸ਼_Array src, OutputArray w, OutputArray u, OutputArray vt, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

ਖਾਲੀ cv::SVBackSubst(ਨਿਵੇਸ਼_Array w, ਨਿਵੇਸ਼_Array u, ਨਿਵੇਸ਼_Array vt, ਨਿਵੇਸ਼_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::ਪਿੱਠ_Subst(w, u, vt, rhs, dst);
}
