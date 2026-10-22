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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <limits>

#ifdef HAVE_EIGEN
#  ಯದಿ defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  ಯದಿ defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

ನಾಮಾಕಾಶ cv
{

ಪೂರ್ಣಾಙ್ಕ LU(ಭಿನ್ನ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತ್ಯಾಗಮ hal::LU32f(A, astep, m, b, bstep, n);
}

ಪೂರ್ಣಾಙ್ಕ LU(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತ್ಯಾಗಮ hal::LU64f(A, astep, m, b, bstep, n);
}

ತಾರ್ಕಿಕ Cholesky(ಭಿನ್ನ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತ್ಯಾಗಮ hal::Cholesky32f(A, astep, m, b, bstep, n);
}

ತಾರ್ಕಿಕ Cholesky(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತ್ಯಾಗಮ hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    ಯದಿ( a > b )
    {
        b /= a;
        ಪ್ರತ್ಯಾಗಮ a*std::sqrt(1 + b*b);
    }
    ಯದಿ( b > 0 )
    {
        a /= b;
        ಪ್ರತ್ಯಾಗಮ b*std::sqrt(1 + a*a);
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}


template<typename _Tp> ತಾರ್ಕಿಕ
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, ಪೂರ್ಣಾಙ್ಕ n, uchar* buf )
{
    ಧ್ರುವ _Tp eps = std::numeric_limits<_Tp>::epsilon();
    ಪೂರ್ಣಾಙ್ಕ i, j, k, m;

    astep /= ಪರಿಮಾಣ(A[0]);
    ಯದಿ( V )
    {
        vstep /= ಪರಿಮಾಣ(V[0]);
        ಪ್ರತಿ( i = 0; i < n; i++ )
        {
            ಪ್ರತಿ( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    ಪೂರ್ಣಾಙ್ಕ iters, maxIters = n*n*30;

    ಪೂರ್ಣಾಙ್ಕ* indR = (ಪೂರ್ಣಾಙ್ಕ*)alignPtr(buf, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    ಪೂರ್ಣಾಙ್ಕ* indC = indR + n;
    _Tp mv = (_Tp)0;

    ಪ್ರತಿ( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        ಯದಿ( k < n - 1 )
        {
            ಪ್ರತಿ( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                ಯದಿ( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        ಯದಿ( k > 0 )
        {
            ಪ್ರತಿ( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                ಯದಿ( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    ಯದಿ( n > 1 ) ಪ್ರತಿ( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        ಪ್ರತಿ( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            ಯದಿ( mv < val )
                mv = val, k = i;
        }
        ಪೂರ್ಣಾಙ್ಕ l = indR[k];
        ಪ್ರತಿ( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            ಯದಿ( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        ಯದಿ( std::abs(p) <= eps )
            ಭಙ್ಗ;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        ಯದಿ( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        ಪ್ರತಿ( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        ಪ್ರತಿ( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        ಪ್ರತಿ( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        ಯದಿ( V )
            ಪ್ರತಿ( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        ಪ್ರತಿ( j = 0; j < 2; j++ )
        {
            ಪೂರ್ಣಾಙ್ಕ idx = j == 0 ? k : l;
            ಯದಿ( idx < n - 1 )
            {
                ಪ್ರತಿ( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    ಯದಿ( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            ಯದಿ( idx > 0 )
            {
                ಪ್ರತಿ( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    ಯದಿ( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    ಪ್ರತಿ( k = 0; k < n-1; k++ )
    {
        m = k;
        ಪ್ರತಿ( i = k+1; i < n; i++ )
        {
            ಯದಿ( W[m] < W[i] )
                m = i;
        }
        ಯದಿ( k != m )
        {
            std::swap(W[m], W[k]);
            ಯದಿ( V )
                ಪ್ರತಿ( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ Jacobi( ಭಿನ್ನ* S, size_t sstep, ಭಿನ್ನ* e, ಭಿನ್ನ* E, size_t estep, ಪೂರ್ಣಾಙ್ಕ n, uchar* buf )
{
    ಪ್ರತ್ಯಾಗಮ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ Jacobi( ದ್ವಿಗುಣ* S, size_t sstep, ದ್ವಿಗುಣ* e, ದ್ವಿಗುಣ* E, size_t estep, ಪೂರ್ಣಾಙ್ಕ n, uchar* buf )
{
    ಪ್ರತ್ಯಾಗಮ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> ಸಂರಚನಾ VBLAS
{
    ಪೂರ್ಣಾಙ್ಕ dot(ಧ್ರುವ T*, ಧ್ರುವ T*, ಪೂರ್ಣಾಙ್ಕ, T*) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ 0; }
    ಪೂರ್ಣಾಙ್ಕ givens(T*, T*, ಪೂರ್ಣಾಙ್ಕ, T, T) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ 0; }
};

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
template<> ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ VBLAS<ಭಿನ್ನ>::dot(ಧ್ರುವ ಭಿನ್ನ* a, ಧ್ರುವ ಭಿನ್ನ* b, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ* result) ಧ್ರುವ
{
    ಯದಿ( n < 2*VTraits<v_float32>::vlanes() )
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ k = 0;
    v_float32 s0 = vx_setzero_f32();
    ಪ್ರತಿ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    ಪ್ರತ್ಯಾಗಮ k;
}


template<> ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ VBLAS<ಭಿನ್ನ>::givens(ಭಿನ್ನ* a, ಭಿನ್ನ* b, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ c, ಭಿನ್ನ s) ಧ್ರುವ
{
    ಯದಿ( n < VTraits<v_float32>::vlanes())
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    ಪ್ರತಿ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ಪ್ರತ್ಯಾಗಮ k;
}


#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ VBLAS<ದ್ವಿಗುಣ>::dot(ಧ್ರುವ ದ್ವಿಗುಣ* a, ಧ್ರುವ ದ್ವಿಗುಣ* b, ಪೂರ್ಣಾಙ್ಕ n, ದ್ವಿಗುಣ* result) ಧ್ರುವ
{
    ಯದಿ( n < 2*VTraits<v_float64>::vlanes() )
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ k = 0;
    v_float64 s0 = vx_setzero_f64();
    ಪ್ರತಿ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    ಪ್ರತ್ಯಾಗಮ k;
}


template<> ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ VBLAS<ದ್ವಿಗುಣ>::givens(ದ್ವಿಗುಣ* a, ದ್ವಿಗುಣ* b, ಪೂರ್ಣಾಙ್ಕ n, ದ್ವಿಗುಣ c, ದ್ವಿಗುಣ s) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    ಪ್ರತಿ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ಪ್ರತ್ಯಾಗಮ k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> ಶೂನ್ಯ
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ n1, ದ್ವಿಗುಣ minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<ದ್ವಿಗುಣ> Wbuf(n);
    ದ್ವಿಗುಣ* W = Wbuf.data();
    ಪೂರ್ಣಾಙ್ಕ i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    ದ್ವಿಗುಣ sd;
    astep /= ಪರಿಮಾಣ(At[0]);
    vstep /= ಪರಿಮಾಣ(Vt[0]);

    ಪ್ರತಿ( i = 0; i < n; i++ )
    {
        ಪ್ರತಿ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ದ್ವಿಗುಣ)t*t;
        }
        W[i] = sd;

        ಯದಿ( Vt )
        {
            ಪ್ರತಿ( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    ಪ್ರತಿ( iter = 0; iter < max_iter; iter++ )
    {
        ತಾರ್ಕಿಕ changed = ಅಸತ್ಯ;

        ಪ್ರತಿ( i = 0; i < n-1; i++ )
            ಪ್ರತಿ( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                ದ್ವಿಗುಣ a = W[i], p = 0, b = W[j];

                ಪ್ರತಿ( k = 0; k < m; k++ )
                    p += (ದ್ವಿಗುಣ)Ai[k]*Aj[k];

                ಯದಿ( std::abs(p) <= eps*std::sqrt((ದ್ವಿಗುಣ)a*b) )
                    ಅನುವರ್ತ;

                p *= 2;
                ದ್ವಿಗುಣ beta = a - b, gamma = hypot((ದ್ವಿಗುಣ)p, beta);
                ಯದಿ( beta < 0 )
                {
                    ದ್ವಿಗುಣ delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                ಅನ್ಯಥಾ
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                ಪ್ರತಿ( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (ದ್ವಿಗುಣ)t0*t0; b += (ದ್ವಿಗುಣ)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = ಸತ್ಯ;

                ಯದಿ( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    ಪ್ರತಿ( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        ಯದಿ( !changed )
            ಭಙ್ಗ;
    }

    ಪ್ರತಿ( i = 0; i < n; i++ )
    {
        ಪ್ರತಿ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ದ್ವಿಗುಣ)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    ಪ್ರತಿ( i = 0; i < n-1; i++ )
    {
        j = i;
        ಪ್ರತಿ( k = i+1; k < n; k++ )
        {
            ಯದಿ( W[j] < W[k] )
                j = k;
        }
        ಯದಿ( i != j )
        {
            std::swap(W[i], W[j]);
            ಯದಿ( Vt )
            {
                ಪ್ರತಿ( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                ಪ್ರತಿ( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    ಪ್ರತಿ( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    ಯದಿ( !Vt )
        ಪ್ರತ್ಯಾಗಮ;

    RNG rng(0x12345678);
    ಪ್ರತಿ( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ಧ್ರುವ _Tp val0 = (_Tp)(1./m);
            ಪ್ರತಿ( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            ಪ್ರತಿ( iter = 0; iter < 2; iter++ )
            {
                ಪ್ರತಿ( j = 0; j < i; j++ )
                {
                    sd = 0;
                    ಪ್ರತಿ( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    ಪ್ರತಿ( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    ಪ್ರತಿ( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            ಪ್ರತಿ( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (ದ್ವಿಗುಣ)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        ಪ್ರತಿ( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ JacobiSVD(ಭಿನ್ನ* At, size_t astep, ಭಿನ್ನ* W, ಭಿನ್ನ* Vt, size_t vstep, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ n1=-1)
{
    hal::SVD32f(At, astep, W, ರಿಕ್ತ, astep, Vt, vstep, m, n, n1);
}

ಸ್ಥಿರ ಶೂನ್ಯ JacobiSVD(ದ್ವಿಗುಣ* At, size_t astep, ದ್ವಿಗುಣ* W, ದ್ವಿಗುಣ* Vt, size_t vstep, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ n1=-1)
{
    hal::SVD64f(At, astep, W, ರಿಕ್ತ, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
decodeSVDParameters(ಧ್ರುವ fptype* U, ಧ್ರುವ fptype* Vt, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ n1)
{
    ಪೂರ್ಣಾಙ್ಕ halSVDFlag = 0;
    ಯದಿ(Vt == ರಿಕ್ತ)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    ಅನ್ಯಥಾ ಯದಿ(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        ಯದಿ(U == ರಿಕ್ತ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ಅನ್ಯಥಾ ಯದಿ(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        ಯದಿ(U == ರಿಕ್ತ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ಪ್ರತ್ಯಾಗಮ halSVDFlag;
}

ಶೂನ್ಯ hal::SVD32f(ಭಿನ್ನ* At, size_t astep, ಭಿನ್ನ* W, ಭಿನ್ನ* U, size_t ustep, ಭಿನ್ನ* Vt, size_t vstep, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

ಶೂನ್ಯ hal::SVD64f(ದ್ವಿಗುಣ* At, size_t astep, ದ್ವಿಗುಣ* W, ದ್ವಿಗುಣ* U, size_t ustep, ದ್ವಿಗುಣ* Vt, size_t vstep, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> ಸ್ಥಿರ ಶೂನ್ಯ
MatrAXPY( ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಧ್ರುವ T1* x, ಪೂರ್ಣಾಙ್ಕ dx,
         ಧ್ರುವ T2* a, ಪೂರ್ಣಾಙ್ಕ inca, T3* y, ಪೂರ್ಣಾಙ್ಕ dy )
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        ಪೂರ್ಣಾಙ್ಕ j = 0;
         #ಯದಿ CV_ENABLE_UNROLLED
        ಪ್ರತಿ(; j <= n - 4; j += 4 )
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
        ಪ್ರತಿ( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ
SVBkSbImpl_( ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಧ್ರುವ T* w, ಪೂರ್ಣಾಙ್ಕ incw,
       ಧ್ರುವ T* u, ಪೂರ್ಣಾಙ್ಕ ldu, ತಾರ್ಕಿಕ uT,
       ಧ್ರುವ T* v, ಪೂರ್ಣಾಙ್ಕ ldv, ತಾರ್ಕಿಕ vT,
       ಧ್ರುವ T* b, ಪೂರ್ಣಾಙ್ಕ ldb, ಪೂರ್ಣಾಙ್ಕ nb,
       T* x, ಪೂರ್ಣಾಙ್ಕ ldx, ದ್ವಿಗುಣ* buffer, T eps )
{
    ದ್ವಿಗುಣ threshold = 0;
    ಪೂರ್ಣಾಙ್ಕ udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    ಪೂರ್ಣಾಙ್ಕ vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    ಪೂರ್ಣಾಙ್ಕ i, j, nm = std::min(m, n);

    ಯದಿ( !b )
        nb = m;

    ಪ್ರತಿ( i = 0; i < n; i++ )
        ಪ್ರತಿ( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    ಪ್ರತಿ( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    ಪ್ರತಿ( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        ದ್ವಿಗುಣ wi = w[i*incw];
        ಯದಿ( (ದ್ವಿಗುಣ)std::abs(wi) <= threshold )
            ಅನುವರ್ತ;
        wi = 1/wi;

        ಯದಿ( nb == 1 )
        {
            ದ್ವಿಗುಣ s = 0;
            ಯದಿ( b )
                ಪ್ರತಿ( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            ಅನ್ಯಥಾ
                s = u[0];
            s *= wi;

            ಪ್ರತಿ( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ( b )
            {
                ಪ್ರತಿ( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                ಪ್ರತಿ( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            ಅನ್ಯಥಾ
            {
                ಪ್ರತಿ( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
SVBkSb( ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಧ್ರುವ ಭಿನ್ನ* w, size_t wstep,
        ಧ್ರುವ ಭಿನ್ನ* u, size_t ustep, ತಾರ್ಕಿಕ uT,
        ಧ್ರುವ ಭಿನ್ನ* v, size_t vstep, ತಾರ್ಕಿಕ vT,
        ಧ್ರುವ ಭಿನ್ನ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ nb,
        ಭಿನ್ನ* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (ಪೂರ್ಣಾಙ್ಕ)(wstep/ಪರಿಮಾಣ(w[0])) : 1,
                u, (ಪೂರ್ಣಾಙ್ಕ)(ustep/ಪರಿಮಾಣ(u[0])), uT,
                v, (ಪೂರ್ಣಾಙ್ಕ)(vstep/ಪರಿಮಾಣ(v[0])), vT,
                b, (ಪೂರ್ಣಾಙ್ಕ)(bstep/ಪರಿಮಾಣ(b[0])), nb,
                x, (ಪೂರ್ಣಾಙ್ಕ)(xstep/ಪರಿಮಾಣ(x[0])),
                (ದ್ವಿಗುಣ*)alignPtr(buffer, ಪರಿಮಾಣ(ದ್ವಿಗುಣ)), (ಭಿನ್ನ)(DBL_EPSILON*2) );
}

ಸ್ಥಿರ ಶೂನ್ಯ
SVBkSb( ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಧ್ರುವ ದ್ವಿಗುಣ* w, size_t wstep,
       ಧ್ರುವ ದ್ವಿಗುಣ* u, size_t ustep, ತಾರ್ಕಿಕ uT,
       ಧ್ರುವ ದ್ವಿಗುಣ* v, size_t vstep, ತಾರ್ಕಿಕ vT,
       ಧ್ರುವ ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ nb,
       ದ್ವಿಗುಣ* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (ಪೂರ್ಣಾಙ್ಕ)(wstep/ಪರಿಮಾಣ(w[0])) : 1,
                u, (ಪೂರ್ಣಾಙ್ಕ)(ustep/ಪರಿಮಾಣ(u[0])), uT,
                v, (ಪೂರ್ಣಾಙ್ಕ)(vstep/ಪರಿಮಾಣ(v[0])), vT,
                b, (ಪೂರ್ಣಾಙ್ಕ)(bstep/ಪರಿಮಾಣ(b[0])), nb,
                x, (ಪೂರ್ಣಾಙ್ಕ)(xstep/ಪರಿಮಾಣ(x[0])),
                (ದ್ವಿಗುಣ*)alignPtr(buffer, ಪರಿಮಾಣ(ದ್ವಿಗುಣ)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((ದ್ವಿಗುಣ)m(0,0)*m(1,1) - (ದ್ವಿಗುಣ)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((ದ್ವಿಗುಣ)m(1,1)*m(2,2) - (ದ್ವಿಗುಣ)m(1,2)*m(2,1)) -  \
                   m(0,1)*((ದ್ವಿಗುಣ)m(1,0)*m(2,2) - (ದ್ವಿಗುಣ)m(1,2)*m(2,0)) +  \
                   m(0,2)*((ದ್ವಿಗುಣ)m(1,0)*m(2,1) - (ದ್ವಿಗುಣ)m(1,1)*m(2,0)))

ದ್ವಿಗುಣ determinant( ಒಳಹರಿವು_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    ದ್ವಿಗುಣ result = 0;
    ಪೂರ್ಣಾಙ್ಕ type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    ಧ್ರುವ uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((ಭಿನ್ನ*)(m + y*step))[x]
    #define Md(y, x) ((ದ್ವಿಗುಣ*)(m + y*step))[x]

    ಯದಿ( type == CV_32F )
    {
        ಯದಿ( rows == 2 )
            result = det2(Mf);
        ಅನ್ಯಥಾ ಯದಿ( rows == 3 )
            result = det3(Mf);
        ಅನ್ಯಥಾ ಯದಿ( rows == 1 )
            result = Mf(0,0);
        ಅನ್ಯಥಾ
        {
            size_t bufSize = rows*rows*ಪರಿಮಾಣ(ಭಿನ್ನ);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<ಭಿನ್ನ>(), a.step, rows, 0, 0, 0);
            ಯದಿ( result )
            {
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < rows; i++ )
                    result *= a.at<ಭಿನ್ನ>(i,i);
            }
        }
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ( rows == 2 )
            result = det2(Md);
        ಅನ್ಯಥಾ ಯದಿ( rows == 3 )
            result = det3(Md);
        ಅನ್ಯಥಾ ಯದಿ( rows == 1 )
            result = Md(0,0);
        ಅನ್ಯಥಾ
        {
            size_t bufSize = rows*rows*ಪರಿಮಾಣ(ದ್ವಿಗುಣ);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<ದ್ವಿಗುಣ>(), a.step, rows, 0, 0, 0);
            ಯದಿ( result )
            {
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < rows; i++ )
                    result *= a.at<ದ್ವಿಗುಣ>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    ಪ್ರತ್ಯಾಗಮ result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((ಭಿನ್ನ*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((ದ್ವಿಗುಣ*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((ಭಿನ್ನ*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((ದ್ವಿಗುಣ*)(dstdata + y*dststep))[x]

ದ್ವಿಗುಣ invert( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ method )
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ result = ಅಸತ್ಯ;
    Mat src = _src.getMat();
    ಪೂರ್ಣಾಙ್ಕ type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    ಪೂರ್ಣಾಙ್ಕ m = src.rows, n = src.cols;

    ಯದಿ( method == DECOMP_SVD )
    {
        ಪೂರ್ಣಾಙ್ಕ nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + ಪರಿಮಾಣ(ದ್ವಿಗುಣ));
        uchar* buf = alignPtr((uchar*)_buf.data(), (ಪೂರ್ಣಾಙ್ಕ)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        ಪ್ರತ್ಯಾಗಮ type == CV_32F ?
            (w.ptr<ಭಿನ್ನ>()[0] >= FLT_EPSILON ?
             w.ptr<ಭಿನ್ನ>()[n-1]/w.ptr<ಭಿನ್ನ>()[0] : 0) :
            (w.ptr<ದ್ವಿಗುಣ>()[0] >= DBL_EPSILON ?
             w.ptr<ದ್ವಿಗುಣ>()[n-1]/w.ptr<ದ್ವಿಗುಣ>()[0] : 0);
    }

    CV_Assert( m == n );

    ಯದಿ( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + ಪರಿಮಾಣ(ದ್ವಿಗುಣ));
        uchar* buf = alignPtr((uchar*)_buf.data(), (ಪೂರ್ಣಾಙ್ಕ)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        ಪ್ರತ್ಯಾಗಮ type == CV_32F ?
        (w.ptr<ಭಿನ್ನ>()[0] >= FLT_EPSILON ?
         w.ptr<ಭಿನ್ನ>()[n-1]/w.ptr<ಭಿನ್ನ>()[0] : 0) :
        (w.ptr<ದ್ವಿಗುಣ>()[0] >= DBL_EPSILON ?
         w.ptr<ದ್ವಿಗುಣ>()[n-1]/w.ptr<ದ್ವಿಗುಣ>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    ಯದಿ( n <= 3 )
    {
        ಧ್ರುವ uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        ಯದಿ( n == 2 )
        {
            ಯದಿ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = det2(Sf);
                ಯದಿ( d != 0. )
                {
                    result = ಸತ್ಯ;
                    d = 1./d;
                #ಯದಿ CV_SIMD128
                    ಧ್ರುವ ಭಿನ್ನ d_32f = (ಭಿನ್ನ)d;
                    ಧ್ರುವ v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((ಧ್ರುವ ಭಿನ್ನ *)srcdata, (ಧ್ರುವ ಭಿನ್ನ *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_ಕೆಳಗಿನ(v_rotate_ಬಲ<1>(s0), s0));
                    v_store_ಕೆಳಗಿನ((ಭಿನ್ನ*)dstdata, s0);
                    v_store_ಮೇಲಿನ((ಭಿನ್ನ*)(dstdata + dststep), s0);
                #ಅನ್ಯಥಾ
                    ದ್ವಿಗುಣ t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (ಭಿನ್ನ)t0;
                    Df(0,0) = (ಭಿನ್ನ)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (ಭಿನ್ನ)t0;
                    Df(1,0) = (ಭಿನ್ನ)t1;
                #endif
                }
            }
            ಅನ್ಯಥಾ
            {
                ದ್ವಿಗುಣ d = det2(Sd);
                ಯದಿ( d != 0. )
                {
                    result = ಸತ್ಯ;
                    d = 1./d;
                #ಯದಿ CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((ಧ್ರುವ ದ್ವಿಗುಣ *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((ಧ್ರುವ ದ್ವಿಗುಣ *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<ದ್ವಿಗುಣ>(0), v_extract<1>(s0, s1));//12
                    v_store((ದ್ವಿಗುಣ*)dstdata, v_combine_ಕೆಳಗಿನ(sm, ss));//31
                    v_store((ದ್ವಿಗುಣ*)(dstdata + dststep), v_combine_ಮೇಲಿನ(ss, sm));//20
                #ಅನ್ಯಥಾ
                    ದ್ವಿಗುಣ t0, t1;
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
        ಅನ್ಯಥಾ ಯದಿ( n == 3 )
        {
            ಯದಿ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = det3(Sf);

                ಯದಿ( d != 0. )
                {
                    ದ್ವಿಗುಣ t[12];

                    result = ಸತ್ಯ;
                    d = 1./d;
                    t[0] = (((ದ್ವಿಗುಣ)Sf(1,1) * Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((ದ್ವಿಗುಣ)Sf(0,2) * Sf(2,1) - (ದ್ವಿಗುಣ)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((ದ್ವಿಗುಣ)Sf(0,1) * Sf(1,2) - (ದ್ವಿಗುಣ)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((ದ್ವಿಗುಣ)Sf(1,2) * Sf(2,0) - (ದ್ವಿಗುಣ)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((ದ್ವಿಗುಣ)Sf(0,0) * Sf(2,2) - (ದ್ವಿಗುಣ)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((ದ್ವಿಗುಣ)Sf(0,2) * Sf(1,0) - (ದ್ವಿಗುಣ)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((ದ್ವಿಗುಣ)Sf(1,0) * Sf(2,1) - (ದ್ವಿಗುಣ)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((ದ್ವಿಗುಣ)Sf(0,1) * Sf(2,0) - (ದ್ವಿಗುಣ)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((ದ್ವಿಗುಣ)Sf(0,0) * Sf(1,1) - (ದ್ವಿಗುಣ)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (ಭಿನ್ನ)t[0]; Df(0,1) = (ಭಿನ್ನ)t[1]; Df(0,2) = (ಭಿನ್ನ)t[2];
                    Df(1,0) = (ಭಿನ್ನ)t[3]; Df(1,1) = (ಭಿನ್ನ)t[4]; Df(1,2) = (ಭಿನ್ನ)t[5];
                    Df(2,0) = (ಭಿನ್ನ)t[6]; Df(2,1) = (ಭಿನ್ನ)t[7]; Df(2,2) = (ಭಿನ್ನ)t[8];
                }
            }
            ಅನ್ಯಥಾ
            {
                ದ್ವಿಗುಣ d = det3(Sd);
                ಯದಿ( d != 0. )
                {
                    result = ಸತ್ಯ;
                    d = 1./d;
                    ದ್ವಿಗುಣ t[9];

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
        ಅನ್ಯಥಾ
        {
            CV_Assert( n == 1 );

            ಯದಿ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = Sf(0,0);
                ಯದಿ( d != 0. )
                {
                    result = ಸತ್ಯ;
                    Df(0,0) = (ಭಿನ್ನ)(1./d);
                }
            }
            ಅನ್ಯಥಾ
            {
                ದ್ವಿಗುಣ d = Sd(0,0);
                ಯದಿ( d != 0. )
                {
                    result = ಸತ್ಯ;
                    Dd(0,0) = 1./d;
                }
            }
        }
        ಯದಿ( !result )
            dst = Scalar(0);
        ಪ್ರತ್ಯಾಗಮ result;
    }

   ಪೂರ್ಣಾಙ್ಕ elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    ಯದಿ( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<ಭಿನ್ನ>(), src1.step, n, dst.ptr<ಭಿನ್ನ>(), dst.step, n) != 0;
    ಅನ್ಯಥಾ ಯದಿ( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<ದ್ವಿಗುಣ>(), src1.step, n, dst.ptr<ದ್ವಿಗುಣ>(), dst.step, n) != 0;
    ಅನ್ಯಥಾ ಯದಿ( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<ಭಿನ್ನ>(), src1.step, n, dst.ptr<ಭಿನ್ನ>(), dst.step, n);
    ಅನ್ಯಥಾ
        result = hal::Cholesky64f(src1.ptr<ದ್ವಿಗುಣ>(), src1.step, n, dst.ptr<ದ್ವಿಗುಣ>(), dst.step, n);

    ಯದಿ( !result )
        dst = Scalar(0);

    ಪ್ರತ್ಯಾಗಮ result;
}

UMat UMat::inv(ಪೂರ್ಣಾಙ್ಕ method) ಧ್ರುವ
{
    UMat m;
    invert(*ಏತತ್, m, method);
    ಪ್ರತ್ಯಾಗಮ m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

ತಾರ್ಕಿಕ solve( ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _src2arg, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ method )
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ result = ಸತ್ಯ;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    ಪೂರ್ಣಾಙ್ಕ type = src.type();
    ತಾರ್ಕಿಕ is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    ಯದಿ( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((ಭಿನ್ನ*)(bdata + y*src2step))[0]
        #define bd(y) ((ದ್ವಿಗುಣ*)(bdata + y*src2step))[0]

        ಧ್ರುವ uchar* srcdata = src.ptr();
        ಧ್ರುವ uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        ಯದಿ( src.rows == 2 )
        {
            ಯದಿ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = det2(Sf);
                ಯದಿ( d != 0. )
                {
                    ದ್ವಿಗುಣ t;
                    d = 1./d;
                    t = (ಭಿನ್ನ)(((ದ್ವಿಗುಣ)bf(0)*Sf(1,1) - (ದ್ವಿಗುಣ)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (ಭಿನ್ನ)(((ದ್ವಿಗುಣ)bf(1)*Sf(0,0) - (ದ್ವಿಗುಣ)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (ಭಿನ್ನ)t;
                }
                ಅನ್ಯಥಾ
                    result = ಅಸತ್ಯ;
            }
            ಅನ್ಯಥಾ
            {
                ದ್ವಿಗುಣ d = det2(Sd);
                ಯದಿ( d != 0. )
                {
                    ದ್ವಿಗುಣ t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                ಅನ್ಯಥಾ
                    result = ಅಸತ್ಯ;
            }
        }
        ಅನ್ಯಥಾ ಯದಿ( src.rows == 3 )
        {
            ಯದಿ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = det3(Sf);
                ಯದಿ( d != 0. )
                {
                    ಭಿನ್ನ t[3];
                    d = 1./d;

                    t[0] = (ಭಿನ್ನ)(d*
                           (bf(0)*((ದ್ವಿಗುಣ)Sf(1,1)*Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((ದ್ವಿಗುಣ)bf(1)*Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((ದ್ವಿಗುಣ)bf(1)*Sf(2,1) - (ದ್ವಿಗುಣ)Sf(1,1)*bf(2))));

                    t[1] = (ಭಿನ್ನ)(d*
                           (Sf(0,0)*(ದ್ವಿಗುಣ)(bf(1)*Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2)*bf(2)) -
                            bf(0)*((ದ್ವಿಗುಣ)Sf(1,0)*Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((ದ್ವಿಗುಣ)Sf(1,0)*bf(2) - (ದ್ವಿಗುಣ)bf(1)*Sf(2,0))));

                    t[2] = (ಭಿನ್ನ)(d*
                           (Sf(0,0)*((ದ್ವಿಗುಣ)Sf(1,1)*bf(2) - (ದ್ವಿಗುಣ)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((ದ್ವಿಗುಣ)Sf(1,0)*bf(2) - (ದ್ವಿಗುಣ)bf(1)*Sf(2,0)) +
                            bf(0)*((ದ್ವಿಗುಣ)Sf(1,0)*Sf(2,1) - (ದ್ವಿಗುಣ)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                ಅನ್ಯಥಾ
                    result = ಅಸತ್ಯ;
            }
            ಅನ್ಯಥಾ
            {
                ದ್ವಿಗುಣ d = det3(Sd);
                ಯದಿ( d != 0. )
                {
                    ದ್ವಿಗುಣ t[9];

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
                ಅನ್ಯಥಾ
                    result = ಅಸತ್ಯ;
            }
        }
        ಅನ್ಯಥಾ
        {
            CV_Assert( src.rows == 1 );

            ಯದಿ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = Sf(0,0);
                ಯದಿ( d != 0. )
                    Df(0,0) = (ಭಿನ್ನ)(bf(0)/d);
                ಅನ್ಯಥಾ
                    result = ಅಸತ್ಯ;
            }
            ಅನ್ಯಥಾ
            {
                ದ್ವಿಗುಣ d = Sd(0,0);
                ಯದಿ( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                ಅನ್ಯಥಾ
                    result = ಅಸತ್ಯ;
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }

    ಪೂರ್ಣಾಙ್ಕ m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    ಯದಿ( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    ಯದಿ( m == n )
        is_normal = ಅಸತ್ಯ;
    ಅನ್ಯಥಾ ಯದಿ( is_normal )
    {
        m_ = n;
        ಯದಿ( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    ಯದಿ( is_normal )
        bufsize += n*nb*esz;
    ಯದಿ( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*ಪರಿಮಾಣ(ದ್ವಿಗುಣ) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    ಯದಿ( is_normal )
        mulTransposed(src, a, ಸತ್ಯ);
    ಅನ್ಯಥಾ ಯದಿ( method != DECOMP_SVD )
        src.copyTo(a);
    ಅನ್ಯಥಾ
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    ಯದಿ( !is_normal )
    {
        ಯದಿ( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    ಅನ್ಯಥಾ
    {
        // a'*b
        ಯದಿ( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        ಅನ್ಯಥಾ
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    ಯದಿ( method == DECOMP_LU )
    {
        ಯದಿ( type == CV_32F )
            result = hal::LU32f(a.ptr<ಭಿನ್ನ>(), a.step, n, dst.ptr<ಭಿನ್ನ>(), dst.step, nb) != 0;
        ಅನ್ಯಥಾ
            result = hal::LU64f(a.ptr<ದ್ವಿಗುಣ>(), a.step, n, dst.ptr<ದ್ವಿಗುಣ>(), dst.step, nb) != 0;
    }
    ಅನ್ಯಥಾ ಯದಿ( method == DECOMP_CHOLESKY )
    {
        ಯದಿ( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<ಭಿನ್ನ>(), a.step, n, dst.ptr<ಭಿನ್ನ>(), dst.step, nb);
        ಅನ್ಯಥಾ
            result = hal::Cholesky64f(a.ptr<ದ್ವಿಗುಣ>(), a.step, n, dst.ptr<ದ್ವಿಗುಣ>(), dst.step, nb);
    }
    ಅನ್ಯಥಾ ಯದಿ( method == DECOMP_QR )
    {
        Mat rhsMat;
        ಯದಿ( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        ಅನ್ಯಥಾ
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        ಯದಿ( type == CV_32F )
            result = hal::QR32f(a.ptr<ಭಿನ್ನ>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<ಭಿನ್ನ>(), rhsMat.step, ರಿಕ್ತ) != 0;
        ಅನ್ಯಥಾ
            result = hal::QR64f(a.ptr<ದ್ವಿಗುಣ>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<ದ್ವಿಗುಣ>(), rhsMat.step, ರಿಕ್ತ) != 0;

        ಯದಿ (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    ಅನ್ಯಥಾ
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        ಯದಿ( method == DECOMP_EIG )
        {
            ಯದಿ( type == CV_32F )
                Jacobi(a.ptr<ಭಿನ್ನ>(), a.step, w.ptr<ಭಿನ್ನ>(), v.ptr<ಭಿನ್ನ>(), v.step, n, ptr);
            ಅನ್ಯಥಾ
                Jacobi(a.ptr<ದ್ವಿಗುಣ>(), a.step, w.ptr<ದ್ವಿಗುಣ>(), v.ptr<ದ್ವಿಗುಣ>(), v.step, n, ptr);
            u = v;
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ( type == CV_32F )
                JacobiSVD(a.ptr<ಭಿನ್ನ>(), a.step, w.ptr<ಭಿನ್ನ>(), v.ptr<ಭಿನ್ನ>(), v.step, m_, n);
            ಅನ್ಯಥಾ
                JacobiSVD(a.ptr<ದ್ವಿಗುಣ>(), a.step, w.ptr<ದ್ವಿಗುಣ>(), v.ptr<ದ್ವಿಗುಣ>(), v.step, m_, n);
            u = a;
        }

        ಯದಿ( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<ಭಿನ್ನ>(), 0, u.ptr<ಭಿನ್ನ>(), u.step, ಸತ್ಯ,
                   v.ptr<ಭಿನ್ನ>(), v.step, ಸತ್ಯ, src2.ptr<ಭಿನ್ನ>(),
                   src2.step, nb, dst.ptr<ಭಿನ್ನ>(), dst.step, ptr);
        }
        ಅನ್ಯಥಾ
        {
            SVBkSb(m_, n, w.ptr<ದ್ವಿಗುಣ>(), 0, u.ptr<ದ್ವಿಗುಣ>(), u.step, ಸತ್ಯ,
                   v.ptr<ದ್ವಿಗುಣ>(), v.step, ಸತ್ಯ, src2.ptr<ದ್ವಿಗುಣ>(),
                   src2.step, nb, dst.ptr<ದ್ವಿಗುಣ>(), dst.step, ptr);
        }
        result = ಸತ್ಯ;
    }

    ಯದಿ( !result )
        dst = Scalar(0);

    ಪ್ರತ್ಯಾಗಮ result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

ತಾರ್ಕಿಕ eigen( ಒಳಹರಿವು_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ಪೂರ್ಣಾಙ್ಕ type = src.type();
    ಪೂರ್ಣಾಙ್ಕ n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    ಯದಿ( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    ಧ್ರುವ ತಾರ್ಕಿಕ evecNeeded = _evects.needed();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    ಯದಿ ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        ಯದಿ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ಯದಿ ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
    } ಅನ್ಯಥಾ { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        ಯದಿ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ಯದಿ ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#ಅನ್ಯಥಾ

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    ತಾರ್ಕಿಕ ok = type == CV_32F ?
        Jacobi(a.ptr<ಭಿನ್ನ>(), a.step, w.ptr<ಭಿನ್ನ>(), v.ptr<ಭಿನ್ನ>(), v.step, n, ptr) :
        Jacobi(a.ptr<ದ್ವಿಗುಣ>(), a.step, w.ptr<ದ್ವಿಗುಣ>(), v.ptr<ದ್ವಿಗುಣ>(), v.step, n, ptr);

    w.copyTo(_evals);
    ಪ್ರತ್ಯಾಗಮ ok;
#endif
}

ಸ್ಥಿರ ಶೂನ್ಯ _SVDcompute( ಒಳಹರಿವು_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, ಪೂರ್ಣಾಙ್ಕ flags )
{
    Mat src = _aarr.getMat();
    ಪೂರ್ಣಾಙ್ಕ m = src.rows, n = src.cols;
    ಪೂರ್ಣಾಙ್ಕ type = src.type();
    ತಾರ್ಕಿಕ compute_uv = _u.needed() || _vt.needed();
    ತಾರ್ಕಿಕ full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    ಯದಿ( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = ಅಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ at = ಅಸತ್ಯ;
    ಯದಿ( m < n )
    {
        std::swap(m, n);
        at = ಸತ್ಯ;
    }

    ಪೂರ್ಣಾಙ್ಕ urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    ಯದಿ( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    ಯದಿ( urows > n )
        temp_u = Scalar::all(0);

    ಯದಿ( !at )
        transpose(src, temp_a);
    ಅನ್ಯಥಾ
        src.copyTo(temp_a);

    ಯದಿ( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<ಭಿನ್ನ>(), temp_u.step, temp_w.ptr<ಭಿನ್ನ>(),
              temp_v.ptr<ಭಿನ್ನ>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    ಅನ್ಯಥಾ
    {
        JacobiSVD(temp_a.ptr<ದ್ವಿಗುಣ>(), temp_u.step, temp_w.ptr<ದ್ವಿಗುಣ>(),
              temp_v.ptr<ದ್ವಿಗುಣ>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    ಯದಿ( compute_uv )
    {
        ಯದಿ( !at )
        {
            ಯದಿ( _u.needed() )
                transpose(temp_u, _u);
            ಯದಿ( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ( _u.needed() )
                transpose(temp_v, _u);
            ಯದಿ( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


ಶೂನ್ಯ SVD::compute( ಒಳಹರಿವು_Array a, OutputArray w, OutputArray u, OutputArray vt, ಪೂರ್ಣಾಙ್ಕ flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

ಶೂನ್ಯ SVD::compute( ಒಳಹರಿವು_Array a, OutputArray w, ಪೂರ್ಣಾಙ್ಕ flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

ಶೂನ್ಯ SVD::backSubst( ಒಳಹರಿವು_Array _w, ಒಳಹರಿವು_Array _u, ಒಳಹರಿವು_Array _vt,
                     ಒಳಹರಿವು_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    ಪೂರ್ಣಾಙ್ಕ type = w.type(), esz = (ಪೂರ್ಣಾಙ್ಕ)w.elemSize();
    ಪೂರ್ಣಾಙ್ಕ m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*ಪರಿಮಾಣ(ದ್ವಿಗುಣ) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    ಯದಿ( type == CV_32F )
        SVBkSb(m, n, w.ptr<ಭಿನ್ನ>(), wstep, u.ptr<ಭಿನ್ನ>(), u.step, ಅಸತ್ಯ,
               vt.ptr<ಭಿನ್ನ>(), vt.step, ಸತ್ಯ, rhs.ptr<ಭಿನ್ನ>(), rhs.step, nb,
               dst.ptr<ಭಿನ್ನ>(), dst.step, buffer.data());
    ಅನ್ಯಥಾ ಯದಿ( type == CV_64F )
        SVBkSb(m, n, w.ptr<ದ್ವಿಗುಣ>(), wstep, u.ptr<ದ್ವಿಗುಣ>(), u.step, ಅಸತ್ಯ,
               vt.ptr<ದ್ವಿಗುಣ>(), vt.step, ಸತ್ಯ, rhs.ptr<ದ್ವಿಗುಣ>(), rhs.step, nb,
               dst.ptr<ದ್ವಿಗುಣ>(), dst.step, buffer.data());
    ಅನ್ಯಥಾ
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(ಒಳಹರಿವು_Array a, ಪೂರ್ಣಾಙ್ಕ flags)
{
    _SVDcompute(a, w, u, vt, flags);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}


ಶೂನ್ಯ SVD::backSubst( ಒಳಹರಿವು_Array rhs, OutputArray dst ) ಧ್ರುವ
{
    backSubst( w, u, vt, rhs, dst );
}

}


ಶೂನ್ಯ cv::SVDecomp(ಒಳಹರಿವು_Array src, OutputArray w, OutputArray u, OutputArray vt, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

ಶೂನ್ಯ cv::SVBackSubst(ಒಳಹರಿವು_Array w, ಒಳಹರಿವು_Array u, ಒಳಹರಿವು_Array vt, ಒಳಹರಿವು_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
