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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EIGEN
#  ಆದರೆ defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  ಆದರೆ defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv
{

ಪೂರ್ಣಾಂಕ LU(ದಶಮಾಂಶ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಹಿಂತಿರುಗಿ hal::LU32f(A, astep, m, b, bstep, n);
}

ಪೂರ್ಣಾಂಕ LU(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಹಿಂತಿರುಗಿ hal::LU64f(A, astep, m, b, bstep, n);
}

ತಾರ್ಕಿಕ Cholesky(ದಶಮಾಂಶ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಹಿಂತಿರುಗಿ hal::Cholesky32f(A, astep, m, b, bstep, n);
}

ತಾರ್ಕಿಕ Cholesky(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಹಿಂತಿರುಗಿ hal::Cholesky64f(A, astep, m, b, bstep, n);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ಸ್ಥಿರ ಸಾಲೊಳಗೆ _Tp hypot(_Tp a, _Tp b)
{
    a = std::ನಿರಪೇಕ್ಷ(a);
    b = std::ನಿರಪೇಕ್ಷ(b);
    ಆದರೆ( a > b )
    {
        b /= a;
        ಹಿಂತಿರುಗಿ a*std::ವರ್ಗಮೂಲ(1 + b*b);
    }
    ಆದರೆ( b > 0 )
    {
        a /= b;
        ಹಿಂತಿರುಗಿ b*std::ವರ್ಗಮೂಲ(1 + a*a);
    }
    ಹಿಂತಿರುಗಿ 0;
}


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ತಾರ್ಕಿಕ
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, ಪೂರ್ಣಾಂಕ n, uchar* buf )
{
    ಸ್ಥಿರಾಂಕ _Tp eps = std::numeric_limits<_Tp>::epsilon();
    ಪೂರ್ಣಾಂಕ i, j, k, m;

    astep /= ಗಾತ್ರ(A[0]);
    ಆದರೆ( V )
    {
        vstep /= ಗಾತ್ರ(V[0]);
        ಗಾಗಿ( i = 0; i < n; i++ )
        {
            ಗಾಗಿ( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    ಪೂರ್ಣಾಂಕ iters, maxIters = n*n*30;

    ಪೂರ್ಣಾಂಕ* indR = (ಪೂರ್ಣಾಂಕ*)alignPtr(buf, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
    ಪೂರ್ಣಾಂಕ* indC = indR + n;
    _Tp mv = (_Tp)0;

    ಗಾಗಿ( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        ಆದರೆ( k < n - 1 )
        {
            ಗಾಗಿ( m = k+1, mv = std::ನಿರಪೇಕ್ಷ(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::ನಿರಪೇಕ್ಷ(A[astep*k+i]);
                ಆದರೆ( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        ಆದರೆ( k > 0 )
        {
            ಗಾಗಿ( m = 0, mv = std::ನಿರಪೇಕ್ಷ(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::ನಿರಪೇಕ್ಷ(A[astep*i+k]);
                ಆದರೆ( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    ಆದರೆ( n > 1 ) ಗಾಗಿ( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        ಗಾಗಿ( k = 0, mv = std::ನಿರಪೇಕ್ಷ(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::ನಿರಪೇಕ್ಷ(A[astep*i + indR[i]]);
            ಆದರೆ( mv < val )
                mv = val, k = i;
        }
        ಪೂರ್ಣಾಂಕ l = indR[k];
        ಗಾಗಿ( i = 1; i < n; i++ )
        {
            _Tp val = std::ನಿರಪೇಕ್ಷ(A[astep*indC[i] + i]);
            ಆದರೆ( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        ಆದರೆ( std::ನಿರಪೇಕ್ಷ(p) <= eps )
            ನಿಲ್ಲಿಸು;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::ನಿರಪೇಕ್ಷ(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        ಆದರೆ( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು rotate
#ವ್ಯಾಖ್ಯಾನಿಸು rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        ಗಾಗಿ( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        ಗಾಗಿ( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        ಗಾಗಿ( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        ಆದರೆ( V )
            ಗಾಗಿ( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು rotate

        ಗಾಗಿ( j = 0; j < 2; j++ )
        {
            ಪೂರ್ಣಾಂಕ idx = j == 0 ? k : l;
            ಆದರೆ( idx < n - 1 )
            {
                ಗಾಗಿ( m = idx+1, mv = std::ನಿರಪೇಕ್ಷ(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::ನಿರಪೇಕ್ಷ(A[astep*idx+i]);
                    ಆದರೆ( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            ಆದರೆ( idx > 0 )
            {
                ಗಾಗಿ( m = 0, mv = std::ನಿರಪೇಕ್ಷ(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::ನಿರಪೇಕ್ಷ(A[astep*i+idx]);
                    ಆದರೆ( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    ಗಾಗಿ( k = 0; k < n-1; k++ )
    {
        m = k;
        ಗಾಗಿ( i = k+1; i < n; i++ )
        {
            ಆದರೆ( W[m] < W[i] )
                m = i;
        }
        ಆದರೆ( k != m )
        {
            std::swap(W[m], W[k]);
            ಆದರೆ( V )
                ಗಾಗಿ( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ Jacobi( ದಶಮಾಂಶ* S, size_t sstep, ದಶಮಾಂಶ* e, ದಶಮಾಂಶ* E, size_t estep, ಪೂರ್ಣಾಂಕ n, uchar* buf )
{
    ಹಿಂತಿರುಗಿ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ Jacobi( ದ್ವಿಗುಣ* S, size_t sstep, ದ್ವಿಗುಣ* e, ದ್ವಿಗುಣ* E, size_t estep, ಪೂರ್ಣಾಂಕ n, uchar* buf )
{
    ಹಿಂತಿರುಗಿ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ VBLAS
{
    ಪೂರ್ಣಾಂಕ dot(ಸ್ಥಿರಾಂಕ T*, ಸ್ಥಿರಾಂಕ T*, ಪೂರ್ಣಾಂಕ, T*) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ 0; }
    ಪೂರ್ಣಾಂಕ givens(T*, T*, ಪೂರ್ಣಾಂಕ, T, T) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ 0; }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಮಾದರಿ<> ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ VBLAS<ದಶಮಾಂಶ>::dot(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* a, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ* ಫಲಿತಾಂಶ) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( n < 2*VTraits<v_float32>::vlanes() )
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ k = 0;
    v_float32 s0 = vx_setzero_f32();
    ಗಾಗಿ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_ತುಂಬು(a + k);
        v_float32 b0 = vx_ತುಂಬು(b + k);

        s0 = v_ಕೂಡಿಸು(s0, v_mul(a0, b0));
    }
    *ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(s0);
    vx_cleanup();
    ಹಿಂತಿರುಗಿ k;
}


ಮಾದರಿ<> ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ VBLAS<ದಶಮಾಂಶ>::givens(ದಶಮಾಂಶ* a, ದಶಮಾಂಶ* b, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ c, ದಶಮಾಂಶ s) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( n < VTraits<v_float32>::vlanes())
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    ಗಾಗಿ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_ತುಂಬು(a + k);
        v_float32 b0 = vx_ತುಂಬು(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ಹಿಂತಿರುಗಿ k;
}


#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ಮಾದರಿ<> ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ VBLAS<ದ್ವಿಗುಣ>::dot(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* a, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* b, ಪೂರ್ಣಾಂಕ n, ದ್ವಿಗುಣ* ಫಲಿತಾಂಶ) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( n < 2*VTraits<v_float64>::vlanes() )
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ k = 0;
    v_float64 s0 = vx_setzero_f64();
    ಗಾಗಿ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_ತುಂಬು(a + k);
        v_float64 b0 = vx_ತುಂಬು(b + k);

        s0 = v_ಕೂಡಿಸು(s0, v_mul(a0, b0));
    }
    *ಫಲಿತಾಂಶ = v_reduce_ಮೊತ್ತ(s0);
    vx_cleanup();
    ಹಿಂತಿರುಗಿ k;
}


ಮಾದರಿ<> ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ VBLAS<ದ್ವಿಗುಣ>::givens(ದ್ವಿಗುಣ* a, ದ್ವಿಗುಣ* b, ಪೂರ್ಣಾಂಕ n, ದ್ವಿಗುಣ c, ದ್ವಿಗುಣ s) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    ಗಾಗಿ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_ತುಂಬು(a + k);
        v_float64 b0 = vx_ತುಂಬು(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ಹಿಂತಿರುಗಿ k;
}


#ಆದರೆ_ಅಂತ್ಯ // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#ಆದರೆ_ಅಂತ್ಯ // (CV_SIMD || CV_SIMD_SCALABLE)

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ಶೂನ್ಯ
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ n1, ದ್ವಿಗುಣ minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    Auto_ಬಫರ್<ದ್ವಿಗುಣ> Wbuf(n);
    ದ್ವಿಗುಣ* W = Wbuf.ದತ್ತಾಂಶ();
    ಪೂರ್ಣಾಂಕ i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    ದ್ವಿಗುಣ sd;
    astep /= ಗಾತ್ರ(At[0]);
    vstep /= ಗಾತ್ರ(Vt[0]);

    ಗಾಗಿ( i = 0; i < n; i++ )
    {
        ಗಾಗಿ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ದ್ವಿಗುಣ)t*t;
        }
        W[i] = sd;

        ಆದರೆ( Vt )
        {
            ಗಾಗಿ( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    ಗಾಗಿ( iter = 0; iter < max_iter; iter++ )
    {
        ತಾರ್ಕಿಕ ಬದಲಾಯಿತು = ಸುಳ್ಳು;

        ಗಾಗಿ( i = 0; i < n-1; i++ )
            ಗಾಗಿ( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                ದ್ವಿಗುಣ a = W[i], p = 0, b = W[j];

                ಗಾಗಿ( k = 0; k < m; k++ )
                    p += (ದ್ವಿಗುಣ)Ai[k]*Aj[k];

                ಆದರೆ( std::ನಿರಪೇಕ್ಷ(p) <= eps*std::ವರ್ಗಮೂಲ((ದ್ವಿಗುಣ)a*b) )
                    ಮುಂದುವರಿ;

                p *= 2;
                ದ್ವಿಗುಣ beta = a - b, gamma = hypot((ದ್ವಿಗುಣ)p, beta);
                ಆದರೆ( beta < 0 )
                {
                    ದ್ವಿಗುಣ delta = (gamma - beta)*0.5;
                    s = (_Tp)std::ವರ್ಗಮೂಲ(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    c = (_Tp)std::ವರ್ಗಮೂಲ((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                ಗಾಗಿ( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (ದ್ವಿಗುಣ)t0*t0; b += (ದ್ವಿಗುಣ)t1*t1;
                }
                W[i] = a; W[j] = b;

                ಬದಲಾಯಿತು = ನಿಜ;

                ಆದರೆ( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    ಗಾಗಿ( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        ಆದರೆ( !ಬದಲಾಯಿತು )
            ನಿಲ್ಲಿಸು;
    }

    ಗಾಗಿ( i = 0; i < n; i++ )
    {
        ಗಾಗಿ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ದ್ವಿಗುಣ)t*t;
        }
        W[i] = std::ವರ್ಗಮೂಲ(sd);
    }

    ಗಾಗಿ( i = 0; i < n-1; i++ )
    {
        j = i;
        ಗಾಗಿ( k = i+1; k < n; k++ )
        {
            ಆದರೆ( W[j] < W[k] )
                j = k;
        }
        ಆದರೆ( i != j )
        {
            std::swap(W[i], W[j]);
            ಆದರೆ( Vt )
            {
                ಗಾಗಿ( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                ಗಾಗಿ( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    ಗಾಗಿ( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    ಆದರೆ( !Vt )
        ಹಿಂತಿರುಗಿ;

    RNG rng(0x12345678);
    ಗಾಗಿ( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ಸ್ಥಿರಾಂಕ _Tp val0 = (_Tp)(1./m);
            ಗಾಗಿ( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            ಗಾಗಿ( iter = 0; iter < 2; iter++ )
            {
                ಗಾಗಿ( j = 0; j < i; j++ )
                {
                    sd = 0;
                    ಗಾಗಿ( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    ಗಾಗಿ( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::ನಿರಪೇಕ್ಷ(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    ಗಾಗಿ( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            ಗಾಗಿ( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (ದ್ವಿಗುಣ)t*t;
            }
            sd = std::ವರ್ಗಮೂಲ(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        ಗಾಗಿ( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ JacobiSVD(ದಶಮಾಂಶ* At, size_t astep, ದಶಮಾಂಶ* W, ದಶಮಾಂಶ* Vt, size_t vstep, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ n1=-1)
{
    hal::SVD32f(At, astep, W, ಖಾಲಿ, astep, Vt, vstep, m, n, n1);
}

ಸ್ಥಿರ ಶೂನ್ಯ JacobiSVD(ದ್ವಿಗುಣ* At, size_t astep, ದ್ವಿಗುಣ* W, ದ್ವಿಗುಣ* Vt, size_t vstep, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ n1=-1)
{
    hal::SVD64f(At, astep, W, ಖಾಲಿ, astep, Vt, vstep, m, n, n1);
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
decodeSVDParameters(ಸ್ಥಿರಾಂಕ fptype* U, ಸ್ಥಿರಾಂಕ fptype* Vt, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ n1)
{
    ಪೂರ್ಣಾಂಕ halSVDFlag = 0;
    ಆದರೆ(Vt == ಖಾಲಿ)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        ಆದರೆ(U == ಖಾಲಿ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_ತುಂಬಿದ_UV;
        ಆದರೆ(U == ಖಾಲಿ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ಹಿಂತಿರುಗಿ halSVDFlag;
}

ಶೂನ್ಯ hal::SVD32f(ದಶಮಾಂಶ* At, size_t astep, ದಶಮಾಂಶ* W, ದಶಮಾಂಶ* U, size_t ustep, ದಶಮಾಂಶ* Vt, size_t vstep, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

ಶೂನ್ಯ hal::SVD64f(ದ್ವಿಗುಣ* At, size_t astep, ದ್ವಿಗುಣ* W, ದ್ವಿಗುಣ* U, size_t ustep, ದ್ವಿಗುಣ* Vt, size_t vstep, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2, ಬಗೆ_ಹೆಸರು T3> ಸ್ಥಿರ ಶೂನ್ಯ
MatrAXPY( ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಸ್ಥಿರಾಂಕ T1* x, ಪೂರ್ಣಾಂಕ dx,
         ಸ್ಥಿರಾಂಕ T2* a, ಪೂರ್ಣಾಂಕ inca, T3* y, ಪೂರ್ಣಾಂಕ dy )
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        ಪೂರ್ಣಾಂಕ j = 0;
         #ಆದರೆ CV_ENABLE_UNROLLED
        ಗಾಗಿ(; j <= n - 4; j += 4 )
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
        #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
SVBkSbImpl_( ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಸ್ಥಿರಾಂಕ T* w, ಪೂರ್ಣಾಂಕ incw,
       ಸ್ಥಿರಾಂಕ T* u, ಪೂರ್ಣಾಂಕ ldu, ತಾರ್ಕಿಕ uT,
       ಸ್ಥಿರಾಂಕ T* v, ಪೂರ್ಣಾಂಕ ldv, ತಾರ್ಕಿಕ vT,
       ಸ್ಥಿರಾಂಕ T* b, ಪೂರ್ಣಾಂಕ ldb, ಪೂರ್ಣಾಂಕ nb,
       T* x, ಪೂರ್ಣಾಂಕ ldx, ದ್ವಿಗುಣ* ಬಫರ್, T eps )
{
    ದ್ವಿಗುಣ ಮಿತಿ = 0;
    ಪೂರ್ಣಾಂಕ udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    ಪೂರ್ಣಾಂಕ vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    ಪೂರ್ಣಾಂಕ i, j, nm = std::min(m, n);

    ಆದರೆ( !b )
        nb = m;

    ಗಾಗಿ( i = 0; i < n; i++ )
        ಗಾಗಿ( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    ಗಾಗಿ( i = 0; i < nm; i++ )
        ಮಿತಿ += w[i*incw];
    ಮಿತಿ *= eps;

    // v * inv(w) * uT * b
    ಗಾಗಿ( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        ದ್ವಿಗುಣ wi = w[i*incw];
        ಆದರೆ( (ದ್ವಿಗುಣ)std::ನಿರಪೇಕ್ಷ(wi) <= ಮಿತಿ )
            ಮುಂದುವರಿ;
        wi = 1/wi;

        ಆದರೆ( nb == 1 )
        {
            ದ್ವಿಗುಣ s = 0;
            ಆದರೆ( b )
                ಗಾಗಿ( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            ಇಲ್ಲದಿದ್ದರೆ
                s = u[0];
            s *= wi;

            ಗಾಗಿ( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ( b )
            {
                ಗಾಗಿ( j = 0; j < nb; j++ )
                    ಬಫರ್[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, ಬಫರ್, 0 );
                ಗಾಗಿ( j = 0; j < nb; j++ )
                    ಬಫರ್[j] *= wi;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ( j = 0; j < nb; j++ )
                    ಬಫರ್[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, ಬಫರ್, 0, v, vdelta1, x, ldx );
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
SVBkSb( ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* w, size_t wstep,
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* u, size_t ustep, ತಾರ್ಕಿಕ uT,
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* v, size_t vstep, ತಾರ್ಕಿಕ vT,
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* b, size_t bstep, ಪೂರ್ಣಾಂಕ nb,
        ದಶಮಾಂಶ* x, size_t xstep, uchar* ಬಫರ್ )
{
    SVBkSbImpl_(m, n, w, wstep ? (ಪೂರ್ಣಾಂಕ)(wstep/ಗಾತ್ರ(w[0])) : 1,
                u, (ಪೂರ್ಣಾಂಕ)(ustep/ಗಾತ್ರ(u[0])), uT,
                v, (ಪೂರ್ಣಾಂಕ)(vstep/ಗಾತ್ರ(v[0])), vT,
                b, (ಪೂರ್ಣಾಂಕ)(bstep/ಗಾತ್ರ(b[0])), nb,
                x, (ಪೂರ್ಣಾಂಕ)(xstep/ಗಾತ್ರ(x[0])),
                (ದ್ವಿಗುಣ*)alignPtr(ಬಫರ್, ಗಾತ್ರ(ದ್ವಿಗುಣ)), (ದಶಮಾಂಶ)(DBL_EPSILON*2) );
}

ಸ್ಥಿರ ಶೂನ್ಯ
SVBkSb( ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* w, size_t wstep,
       ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* u, size_t ustep, ತಾರ್ಕಿಕ uT,
       ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* v, size_t vstep, ತಾರ್ಕಿಕ vT,
       ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಂಕ nb,
       ದ್ವಿಗುಣ* x, size_t xstep, uchar* ಬಫರ್ )
{
    SVBkSbImpl_(m, n, w, wstep ? (ಪೂರ್ಣಾಂಕ)(wstep/ಗಾತ್ರ(w[0])) : 1,
                u, (ಪೂರ್ಣಾಂಕ)(ustep/ಗಾತ್ರ(u[0])), uT,
                v, (ಪೂರ್ಣಾಂಕ)(vstep/ಗಾತ್ರ(v[0])), vT,
                b, (ಪೂರ್ಣಾಂಕ)(bstep/ಗಾತ್ರ(b[0])), nb,
                x, (ಪೂರ್ಣಾಂಕ)(xstep/ಗಾತ್ರ(x[0])),
                (ದ್ವಿಗುಣ*)alignPtr(ಬಫರ್, ಗಾತ್ರ(ದ್ವಿಗುಣ)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#ವ್ಯಾಖ್ಯಾನಿಸು det2(m)   ((ದ್ವಿಗುಣ)m(0,0)*m(1,1) - (ದ್ವಿಗುಣ)m(0,1)*m(1,0))
#ವ್ಯಾಖ್ಯಾನಿಸು det3(m)   (m(0,0)*((ದ್ವಿಗುಣ)m(1,1)*m(2,2) - (ದ್ವಿಗುಣ)m(1,2)*m(2,1)) -  \
                   m(0,1)*((ದ್ವಿಗುಣ)m(1,0)*m(2,2) - (ದ್ವಿಗುಣ)m(1,2)*m(2,0)) +  \
                   m(0,2)*((ದ್ವಿಗುಣ)m(1,0)*m(2,1) - (ದ್ವಿಗುಣ)m(1,1)*m(2,0)))

ದ್ವಿಗುಣ ನಿರ್ಧಾರಕ( ಒಳಹರಿವು_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = 0;
    ಪೂರ್ಣಾಂಕ type = mat.type(), ಅಡ್ಡಸಾಲುಗಳು = mat.ಅಡ್ಡಸಾಲುಗಳು;
    size_t ಹೆಜ್ಜೆ = mat.ಹೆಜ್ಜೆ;
    ಸ್ಥಿರಾಂಕ uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.ಅಡ್ಡಸಾಲುಗಳು == mat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ && (type == CV_32F || type == CV_64F));

    #ವ್ಯಾಖ್ಯಾನಿಸು Mf(y, x) ((ದಶಮಾಂಶ*)(m + y*ಹೆಜ್ಜೆ))[x]
    #ವ್ಯಾಖ್ಯಾನಿಸು Md(y, x) ((ದ್ವಿಗುಣ*)(m + y*ಹೆಜ್ಜೆ))[x]

    ಆದರೆ( type == CV_32F )
    {
        ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು == 2 )
            ಫಲಿತಾಂಶ = det2(Mf);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು == 3 )
            ಫಲಿತಾಂಶ = det3(Mf);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು == 1 )
            ಫಲಿತಾಂಶ = Mf(0,0);
        ಇಲ್ಲದಿದ್ದರೆ
        {
            size_t buf_ಗಾತ್ರ = ಅಡ್ಡಸಾಲುಗಳು*ಅಡ್ಡಸಾಲುಗಳು*ಗಾತ್ರ(ದಶಮಾಂಶ);
            Auto_ಬಫರ್<uchar> ಬಫರ್(buf_ಗಾತ್ರ);
            Mat a(ಅಡ್ಡಸಾಲುಗಳು, ಅಡ್ಡಸಾಲುಗಳು, CV_32F, ಬಫರ್.ದತ್ತಾಂಶ());
            mat.copy_ಗಮ್ಯ(a);

            ಫಲಿತಾಂಶ = hal::LU32f(a.ptr<ದಶಮಾಂಶ>(), a.ಹೆಜ್ಜೆ, ಅಡ್ಡಸಾಲುಗಳು, 0, 0, 0);
            ಆದರೆ( ಫಲಿತಾಂಶ )
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++ )
                    ಫಲಿತಾಂಶ *= a.at<ದಶಮಾಂಶ>(i,i);
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು == 2 )
            ಫಲಿತಾಂಶ = det2(Md);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು == 3 )
            ಫಲಿತಾಂಶ = det3(Md);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು == 1 )
            ಫಲಿತಾಂಶ = Md(0,0);
        ಇಲ್ಲದಿದ್ದರೆ
        {
            size_t buf_ಗಾತ್ರ = ಅಡ್ಡಸಾಲುಗಳು*ಅಡ್ಡಸಾಲುಗಳು*ಗಾತ್ರ(ದ್ವಿಗುಣ);
            Auto_ಬಫರ್<uchar> ಬಫರ್(buf_ಗಾತ್ರ);
            Mat a(ಅಡ್ಡಸಾಲುಗಳು, ಅಡ್ಡಸಾಲುಗಳು, CV_64F, ಬಫರ್.ದತ್ತಾಂಶ());
            mat.copy_ಗಮ್ಯ(a);

            ಫಲಿತಾಂಶ = hal::LU64f(a.ptr<ದ್ವಿಗುಣ>(), a.ಹೆಜ್ಜೆ, ಅಡ್ಡಸಾಲುಗಳು, 0, 0, 0);
            ಆದರೆ( ಫಲಿತಾಂಶ )
            {
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++ )
                    ಫಲಿತಾಂಶ *= a.at<ದ್ವಿಗುಣ>(i,i);
            }
        }
    }

    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು Mf
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು Md

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#ವ್ಯಾಖ್ಯಾನಿಸು Sf( y, x ) ((ದಶಮಾಂಶ*)(srcdata + y*srcstep))[x]
#ವ್ಯಾಖ್ಯಾನಿಸು Sd( y, x ) ((ದ್ವಿಗುಣ*)(srcdata + y*srcstep))[x]
#ವ್ಯಾಖ್ಯಾನಿಸು Df( y, x ) ((ದಶಮಾಂಶ*)(dstdata + y*dststep))[x]
#ವ್ಯಾಖ್ಯಾನಿಸು Dd( y, x ) ((ದ್ವಿಗುಣ*)(dstdata + y*dststep))[x]

ದ್ವಿಗುಣ invert( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ ವಿಧಾನ )
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
    Mat src = _src.getMat();
    ಪೂರ್ಣಾಂಕ type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_ಗಾತ್ರ(type);
    ಪೂರ್ಣಾಂಕ m = src.ಅಡ್ಡಸಾಲುಗಳು, n = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

    ಆದರೆ( ವಿಧಾನ == DECOMP_SVD )
    {
        ಪೂರ್ಣಾಂಕ nm = std::min(m, n);

        Auto_ಬಫರ್<uchar> _buf((m*nm + nm + nm*n)*esz + ಗಾತ್ರ(ದ್ವಿಗುಣ));
        uchar* buf = alignPtr((uchar*)_buf.ದತ್ತಾಂಶ(), (ಪೂರ್ಣಾಂಕ)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::ಹಿಂಭಾಗ_Subst(w, u, vt, Mat(), _dst);
        ಹಿಂತಿರುಗಿ type == CV_32F ?
            (w.ptr<ದಶಮಾಂಶ>()[0] >= FLT_EPSILON ?
             w.ptr<ದಶಮಾಂಶ>()[n-1]/w.ptr<ದಶಮಾಂಶ>()[0] : 0) :
            (w.ptr<ದ್ವಿಗುಣ>()[0] >= DBL_EPSILON ?
             w.ptr<ದ್ವಿಗುಣ>()[n-1]/w.ptr<ದ್ವಿಗುಣ>()[0] : 0);
    }

    CV_Assert( m == n );

    ಆದರೆ( ವಿಧಾನ == DECOMP_EIG )
    {
        Auto_ಬಫರ್<uchar> _buf((n*n*2 + n)*esz + ಗಾತ್ರ(ದ್ವಿಗುಣ));
        uchar* buf = alignPtr((uchar*)_buf.ದತ್ತಾಂಶ(), (ಪೂರ್ಣಾಂಕ)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::ಹಿಂಭಾಗ_Subst(w, u, vt, Mat(), _dst);
        ಹಿಂತಿರುಗಿ type == CV_32F ?
        (w.ptr<ದಶಮಾಂಶ>()[0] >= FLT_EPSILON ?
         w.ptr<ದಶಮಾಂಶ>()[n-1]/w.ptr<ದಶಮಾಂಶ>()[0] : 0) :
        (w.ptr<ದ್ವಿಗುಣ>()[0] >= DBL_EPSILON ?
         w.ptr<ದ್ವಿಗುಣ>()[n-1]/w.ptr<ದ್ವಿಗುಣ>()[0] : 0);
    }

    CV_Assert( ವಿಧಾನ == DECOMP_LU || ವಿಧಾನ == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    ಆದರೆ( n <= 3 )
    {
        ಸ್ಥಿರಾಂಕ uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.ಹೆಜ್ಜೆ;
        size_t dststep = dst.ಹೆಜ್ಜೆ;

        ಆದರೆ( n == 2 )
        {
            ಆದರೆ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = det2(Sf);
                ಆದರೆ( d != 0. )
                {
                    ಫಲಿತಾಂಶ = ನಿಜ;
                    d = 1./d;
                #ಆದರೆ CV_SIMD128
                    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d_32f = (ದಶಮಾಂಶ)d;
                    ಸ್ಥಿರಾಂಕ v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_ತುಂಬು_halves((ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *)srcdata, (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_ಹೊರತೆಗೆ<3>(s0, v_combine_ಕೆಳಗಿನ(v_rotate_ಬಲ<1>(s0), s0));
                    v_store_ಕೆಳಗಿನ((ದಶಮಾಂಶ*)dstdata, s0);
                    v_store_ಮೇಲಿನ((ದಶಮಾಂಶ*)(dstdata + dststep), s0);
                #ಇಲ್ಲದಿದ್ದರೆ
                    ದ್ವಿಗುಣ t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (ದಶಮಾಂಶ)t0;
                    Df(0,0) = (ದಶಮಾಂಶ)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (ದಶಮಾಂಶ)t0;
                    Df(1,0) = (ದಶಮಾಂಶ)t1;
                #ಆದರೆ_ಅಂತ್ಯ
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ದ್ವಿಗುಣ d = det2(Sd);
                ಆದರೆ( d != 0. )
                {
                    ಫಲಿತಾಂಶ = ನಿಜ;
                    d = 1./d;
                #ಆದರೆ CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_ತುಂಬು((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_ತುಂಬು((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_ಹೊರತೆಗೆ<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<ದ್ವಿಗುಣ>(0), v_ಹೊರತೆಗೆ<1>(s0, s1));//12
                    v_store((ದ್ವಿಗುಣ*)dstdata, v_combine_ಕೆಳಗಿನ(sm, ss));//31
                    v_store((ದ್ವಿಗುಣ*)(dstdata + dststep), v_combine_ಮೇಲಿನ(ss, sm));//20
                #ಇಲ್ಲದಿದ್ದರೆ
                    ದ್ವಿಗುಣ t0, t1;
                    t0 = Sd(0,0)*d;
                    t1 = Sd(1,1)*d;
                    Dd(1,1) = t0;
                    Dd(0,0) = t1;
                    t0 = -Sd(0,1)*d;
                    t1 = -Sd(1,0)*d;
                    Dd(0,1) = t0;
                    Dd(1,0) = t1;
                #ಆದರೆ_ಅಂತ್ಯ
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( n == 3 )
        {
            ಆದರೆ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = det3(Sf);

                ಆದರೆ( d != 0. )
                {
                    ದ್ವಿಗುಣ t[12];

                    ಫಲಿತಾಂಶ = ನಿಜ;
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

                    Df(0,0) = (ದಶಮಾಂಶ)t[0]; Df(0,1) = (ದಶಮಾಂಶ)t[1]; Df(0,2) = (ದಶಮಾಂಶ)t[2];
                    Df(1,0) = (ದಶಮಾಂಶ)t[3]; Df(1,1) = (ದಶಮಾಂಶ)t[4]; Df(1,2) = (ದಶಮಾಂಶ)t[5];
                    Df(2,0) = (ದಶಮಾಂಶ)t[6]; Df(2,1) = (ದಶಮಾಂಶ)t[7]; Df(2,2) = (ದಶಮಾಂಶ)t[8];
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ದ್ವಿಗುಣ d = det3(Sd);
                ಆದರೆ( d != 0. )
                {
                    ಫಲಿತಾಂಶ = ನಿಜ;
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
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_Assert( n == 1 );

            ಆದರೆ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = Sf(0,0);
                ಆದರೆ( d != 0. )
                {
                    ಫಲಿತಾಂಶ = ನಿಜ;
                    Df(0,0) = (ದಶಮಾಂಶ)(1./d);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ದ್ವಿಗುಣ d = Sd(0,0);
                ಆದರೆ( d != 0. )
                {
                    ಫಲಿತಾಂಶ = ನಿಜ;
                    Dd(0,0) = 1./d;
                }
            }
        }
        ಆದರೆ( !ಫಲಿತಾಂಶ )
            dst = Scalar(0);
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

   ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ_2 = CV_ELEM_ಗಾತ್ರ(type);
    Auto_ಬಫರ್<uchar> buf(n*n*elem_ಗಾತ್ರ_2);
    Mat src1(n, n, type, buf.ದತ್ತಾಂಶ());
    src.copy_ಗಮ್ಯ(src1);
    ಗಣ_ತಟಸ್ಥ(dst);

    ಆದರೆ( ವಿಧಾನ == DECOMP_LU && type == CV_32F )
        ಫಲಿತಾಂಶ = hal::LU32f(src1.ptr<ದಶಮಾಂಶ>(), src1.ಹೆಜ್ಜೆ, n, dst.ptr<ದಶಮಾಂಶ>(), dst.ಹೆಜ್ಜೆ, n) != 0;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ವಿಧಾನ == DECOMP_LU && type == CV_64F )
        ಫಲಿತಾಂಶ = hal::LU64f(src1.ptr<ದ್ವಿಗುಣ>(), src1.ಹೆಜ್ಜೆ, n, dst.ptr<ದ್ವಿಗುಣ>(), dst.ಹೆಜ್ಜೆ, n) != 0;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ವಿಧಾನ == DECOMP_CHOLESKY && type == CV_32F )
        ಫಲಿತಾಂಶ = hal::Cholesky32f(src1.ptr<ದಶಮಾಂಶ>(), src1.ಹೆಜ್ಜೆ, n, dst.ptr<ದಶಮಾಂಶ>(), dst.ಹೆಜ್ಜೆ, n);
    ಇಲ್ಲದಿದ್ದರೆ
        ಫಲಿತಾಂಶ = hal::Cholesky64f(src1.ptr<ದ್ವಿಗುಣ>(), src1.ಹೆಜ್ಜೆ, n, dst.ptr<ದ್ವಿಗುಣ>(), dst.ಹೆಜ್ಜೆ, n);

    ಆದರೆ( !ಫಲಿತಾಂಶ )
        dst = Scalar(0);

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

UMat UMat::inv(ಪೂರ್ಣಾಂಕ ವಿಧಾನ) ಸ್ಥಿರಾಂಕ
{
    UMat m;
    invert(*ಇದು, m, ವಿಧಾನ);
    ಹಿಂತಿರುಗಿ m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

ತಾರ್ಕಿಕ solve( ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _src2arg, OutputArray _dst, ಪೂರ್ಣಾಂಕ ವಿಧಾನ )
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = ನಿಜ;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    ಪೂರ್ಣಾಂಕ type = src.type();
    ತಾರ್ಕಿಕ is_ಸಾಮಾನ್ಯ = (ವಿಧಾನ & DECOMP_ಸಾಮಾನ್ಯ) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    ವಿಧಾನ &= ~DECOMP_ಸಾಮಾನ್ಯ;
    CV_Check(ವಿಧಾನ, ವಿಧಾನ == DECOMP_LU || ವಿಧಾನ == DECOMP_SVD || ವಿಧಾನ == DECOMP_EIG ||
                     ವಿಧಾನ == DECOMP_CHOLESKY || ವಿಧಾನ == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (ವಿಧಾನ != DECOMP_LU && ವಿಧಾನ != DECOMP_CHOLESKY) ||
        is_ಸಾಮಾನ್ಯ || src.ಅಡ್ಡಸಾಲುಗಳು == src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );

    // check case of a single equation and small matrix
    ಆದರೆ( (ವಿಧಾನ == DECOMP_LU || ವಿಧಾನ == DECOMP_CHOLESKY) && !is_ಸಾಮಾನ್ಯ &&
        src.ಅಡ್ಡಸಾಲುಗಳು <= 3 && src.ಅಡ್ಡಸಾಲುಗಳು == src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ && _src2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 )
    {
        _dst.create( src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, _src2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.type() );
        Mat dst = _dst.getMat();

        #ವ್ಯಾಖ್ಯಾನಿಸು bf(y) ((ದಶಮಾಂಶ*)(bdata + y*src2step))[0]
        #ವ್ಯಾಖ್ಯಾನಿಸು bd(y) ((ದ್ವಿಗುಣ*)(bdata + y*src2step))[0]

        ಸ್ಥಿರಾಂಕ uchar* srcdata = src.ptr();
        ಸ್ಥಿರಾಂಕ uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.ಹೆಜ್ಜೆ;
        size_t src2step = _src2.ಹೆಜ್ಜೆ;
        size_t dststep = dst.ಹೆಜ್ಜೆ;

        ಆದರೆ( src.ಅಡ್ಡಸಾಲುಗಳು == 2 )
        {
            ಆದರೆ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = det2(Sf);
                ಆದರೆ( d != 0. )
                {
                    ದ್ವಿಗುಣ t;
                    d = 1./d;
                    t = (ದಶಮಾಂಶ)(((ದ್ವಿಗುಣ)bf(0)*Sf(1,1) - (ದ್ವಿಗುಣ)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (ದಶಮಾಂಶ)(((ದ್ವಿಗುಣ)bf(1)*Sf(0,0) - (ದ್ವಿಗುಣ)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (ದಶಮಾಂಶ)t;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ದ್ವಿಗುಣ d = det2(Sd);
                ಆದರೆ( d != 0. )
                {
                    ದ್ವಿಗುಣ t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( src.ಅಡ್ಡಸಾಲುಗಳು == 3 )
        {
            ಆದರೆ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = det3(Sf);
                ಆದರೆ( d != 0. )
                {
                    ದಶಮಾಂಶ t[3];
                    d = 1./d;

                    t[0] = (ದಶಮಾಂಶ)(d*
                           (bf(0)*((ದ್ವಿಗುಣ)Sf(1,1)*Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((ದ್ವಿಗುಣ)bf(1)*Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((ದ್ವಿಗುಣ)bf(1)*Sf(2,1) - (ದ್ವಿಗುಣ)Sf(1,1)*bf(2))));

                    t[1] = (ದಶಮಾಂಶ)(d*
                           (Sf(0,0)*(ದ್ವಿಗುಣ)(bf(1)*Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2)*bf(2)) -
                            bf(0)*((ದ್ವಿಗುಣ)Sf(1,0)*Sf(2,2) - (ದ್ವಿಗುಣ)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((ದ್ವಿಗುಣ)Sf(1,0)*bf(2) - (ದ್ವಿಗುಣ)bf(1)*Sf(2,0))));

                    t[2] = (ದಶಮಾಂಶ)(d*
                           (Sf(0,0)*((ದ್ವಿಗುಣ)Sf(1,1)*bf(2) - (ದ್ವಿಗುಣ)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((ದ್ವಿಗುಣ)Sf(1,0)*bf(2) - (ದ್ವಿಗುಣ)bf(1)*Sf(2,0)) +
                            bf(0)*((ದ್ವಿಗುಣ)Sf(1,0)*Sf(2,1) - (ದ್ವಿಗುಣ)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ದ್ವಿಗುಣ d = det3(Sd);
                ಆದರೆ( d != 0. )
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
                ಇಲ್ಲದಿದ್ದರೆ
                    ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_Assert( src.ಅಡ್ಡಸಾಲುಗಳು == 1 );

            ಆದರೆ( type == CV_32FC1 )
            {
                ದ್ವಿಗುಣ d = Sf(0,0);
                ಆದರೆ( d != 0. )
                    Df(0,0) = (ದಶಮಾಂಶ)(bf(0)/d);
                ಇಲ್ಲದಿದ್ದರೆ
                    ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ದ್ವಿಗುಣ d = Sd(0,0);
                ಆದರೆ( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                ಇಲ್ಲದಿದ್ದರೆ
                    ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    ಪೂರ್ಣಾಂಕ m = src.ಅಡ್ಡಸಾಲುಗಳು, m_ = m, n = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, nb = _src2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    size_t esz = CV_ELEM_ಗಾತ್ರ(type), bufsize = 0;
    size_t vstep = align_ಗಾತ್ರ(n*esz, 16);
    size_t astep = ವಿಧಾನ == DECOMP_SVD && !is_ಸಾಮಾನ್ಯ ? align_ಗಾತ್ರ(m*esz, 16) : vstep;
    Auto_ಬಫರ್<uchar> ಬಫರ್;

    Mat src2 = _src2;
    _dst.create( src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.type() );
    Mat dst = _dst.getMat();

    ಆದರೆ( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    ಆದರೆ( m == n )
        is_ಸಾಮಾನ್ಯ = ಸುಳ್ಳು;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( is_ಸಾಮಾನ್ಯ )
    {
        m_ = n;
        ಆದರೆ( ವಿಧಾನ == DECOMP_SVD )
            ವಿಧಾನ = DECOMP_EIG;
    }

    CV_Assert(m == src2.ಅಡ್ಡಸಾಲುಗಳು);
    size_t asize = astep*(ವಿಧಾನ == DECOMP_SVD || is_ಸಾಮಾನ್ಯ ? n : m);
    bufsize += asize + 32;

    ಆದರೆ( is_ಸಾಮಾನ್ಯ )
        bufsize += n*nb*esz;
    ಆದರೆ( ವಿಧಾನ == DECOMP_SVD || ವಿಧಾನ == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*ಗಾತ್ರ(ದ್ವಿಗುಣ) + 32;

    ಬಫರ್.allocate(bufsize);
    uchar* ptr = alignPtr(ಬಫರ್.ದತ್ತಾಂಶ(), 16);

    Mat a(m_, n, type, ptr, astep);

    ಆದರೆ( is_ಸಾಮಾನ್ಯ )
        mulTransposed(src, a, ನಿಜ);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ವಿಧಾನ != DECOMP_SVD )
        src.copy_ಗಮ್ಯ(a);
    ಇಲ್ಲದಿದ್ದರೆ
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    ಆದರೆ( !is_ಸಾಮಾನ್ಯ )
    {
        ಆದರೆ( ವಿಧಾನ == DECOMP_LU || ವಿಧಾನ == DECOMP_CHOLESKY )
            src2.copy_ಗಮ್ಯ(dst);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // a'*b
        ಆದರೆ( ವಿಧಾನ == DECOMP_LU || ವಿಧಾನ == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        ಇಲ್ಲದಿದ್ದರೆ
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    ಆದರೆ( ವಿಧಾನ == DECOMP_LU )
    {
        ಆದರೆ( type == CV_32F )
            ಫಲಿತಾಂಶ = hal::LU32f(a.ptr<ದಶಮಾಂಶ>(), a.ಹೆಜ್ಜೆ, n, dst.ptr<ದಶಮಾಂಶ>(), dst.ಹೆಜ್ಜೆ, nb) != 0;
        ಇಲ್ಲದಿದ್ದರೆ
            ಫಲಿತಾಂಶ = hal::LU64f(a.ptr<ದ್ವಿಗುಣ>(), a.ಹೆಜ್ಜೆ, n, dst.ptr<ದ್ವಿಗುಣ>(), dst.ಹೆಜ್ಜೆ, nb) != 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ವಿಧಾನ == DECOMP_CHOLESKY )
    {
        ಆದರೆ( type == CV_32F )
            ಫಲಿತಾಂಶ = hal::Cholesky32f(a.ptr<ದಶಮಾಂಶ>(), a.ಹೆಜ್ಜೆ, n, dst.ptr<ದಶಮಾಂಶ>(), dst.ಹೆಜ್ಜೆ, nb);
        ಇಲ್ಲದಿದ್ದರೆ
            ಫಲಿತಾಂಶ = hal::Cholesky64f(a.ptr<ದ್ವಿಗುಣ>(), a.ಹೆಜ್ಜೆ, n, dst.ptr<ದ್ವಿಗುಣ>(), dst.ಹೆಜ್ಜೆ, nb);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ವಿಧಾನ == DECOMP_QR )
    {
        Mat rhsMat;
        ಆದರೆ( is_ಸಾಮಾನ್ಯ || m == n )
        {
            src2.copy_ಗಮ್ಯ(dst);
            rhsMat = dst;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            rhsMat = Mat(m, nb, type);
            src2.copy_ಗಮ್ಯ(rhsMat);
        }

        ಆದರೆ( type == CV_32F )
            ಫಲಿತಾಂಶ = hal::QR32f(a.ptr<ದಶಮಾಂಶ>(), a.ಹೆಜ್ಜೆ, a.ಅಡ್ಡಸಾಲುಗಳು, a.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, rhsMat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, rhsMat.ptr<ದಶಮಾಂಶ>(), rhsMat.ಹೆಜ್ಜೆ, ಖಾಲಿ) != 0;
        ಇಲ್ಲದಿದ್ದರೆ
            ಫಲಿತಾಂಶ = hal::QR64f(a.ptr<ದ್ವಿಗುಣ>(), a.ಹೆಜ್ಜೆ, a.ಅಡ್ಡಸಾಲುಗಳು, a.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, rhsMat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, rhsMat.ptr<ದ್ವಿಗುಣ>(), rhsMat.ಹೆಜ್ಜೆ, ಖಾಲಿ) != 0;

        ಆದರೆ (rhsMat.ಅಡ್ಡಸಾಲುಗಳು != dst.ಅಡ್ಡಸಾಲುಗಳು)
            rhsMat.ಅಡ್ಡಸಾಲು_Range_2(0, dst.ಅಡ್ಡಸಾಲುಗಳು).copy_ಗಮ್ಯ(dst);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        ಆದರೆ( ವಿಧಾನ == DECOMP_EIG )
        {
            ಆದರೆ( type == CV_32F )
                Jacobi(a.ptr<ದಶಮಾಂಶ>(), a.ಹೆಜ್ಜೆ, w.ptr<ದಶಮಾಂಶ>(), v.ptr<ದಶಮಾಂಶ>(), v.ಹೆಜ್ಜೆ, n, ptr);
            ಇಲ್ಲದಿದ್ದರೆ
                Jacobi(a.ptr<ದ್ವಿಗುಣ>(), a.ಹೆಜ್ಜೆ, w.ptr<ದ್ವಿಗುಣ>(), v.ptr<ದ್ವಿಗುಣ>(), v.ಹೆಜ್ಜೆ, n, ptr);
            u = v;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ( type == CV_32F )
                JacobiSVD(a.ptr<ದಶಮಾಂಶ>(), a.ಹೆಜ್ಜೆ, w.ptr<ದಶಮಾಂಶ>(), v.ptr<ದಶಮಾಂಶ>(), v.ಹೆಜ್ಜೆ, m_, n);
            ಇಲ್ಲದಿದ್ದರೆ
                JacobiSVD(a.ptr<ದ್ವಿಗುಣ>(), a.ಹೆಜ್ಜೆ, w.ptr<ದ್ವಿಗುಣ>(), v.ptr<ದ್ವಿಗುಣ>(), v.ಹೆಜ್ಜೆ, m_, n);
            u = a;
        }

        ಆದರೆ( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<ದಶಮಾಂಶ>(), 0, u.ptr<ದಶಮಾಂಶ>(), u.ಹೆಜ್ಜೆ, ನಿಜ,
                   v.ptr<ದಶಮಾಂಶ>(), v.ಹೆಜ್ಜೆ, ನಿಜ, src2.ptr<ದಶಮಾಂಶ>(),
                   src2.ಹೆಜ್ಜೆ, nb, dst.ptr<ದಶಮಾಂಶ>(), dst.ಹೆಜ್ಜೆ, ptr);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            SVBkSb(m_, n, w.ptr<ದ್ವಿಗುಣ>(), 0, u.ptr<ದ್ವಿಗುಣ>(), u.ಹೆಜ್ಜೆ, ನಿಜ,
                   v.ptr<ದ್ವಿಗುಣ>(), v.ಹೆಜ್ಜೆ, ನಿಜ, src2.ptr<ದ್ವಿಗುಣ>(),
                   src2.ಹೆಜ್ಜೆ, nb, dst.ptr<ದ್ವಿಗುಣ>(), dst.ಹೆಜ್ಜೆ, ptr);
        }
        ಫಲಿತಾಂಶ = ನಿಜ;
    }

    ಆದರೆ( !ಫಲಿತಾಂಶ )
        dst = Scalar(0);

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

ತಾರ್ಕಿಕ eigen( ಒಳಹರಿವು_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ಪೂರ್ಣಾಂಕ type = src.type();
    ಪೂರ್ಣಾಂಕ n = src.ಅಡ್ಡಸಾಲುಗಳು;

    CV_Assert( src.ಅಡ್ಡಸಾಲುಗಳು == src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    ಆದರೆ( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EIGEN
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ evecNeeded = _evects.needed();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    ಆದರೆ ( type == CV_64F )
    {
        Eigen::ಮಾತೃಕೆ_Xd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::ಮಾತೃಕೆ_Xd> es;
        es.compute(src_eig, esOptions);
        ಆದರೆ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ಆದರೆ ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
    } ಇಲ್ಲದಿದ್ದರೆ { // CV_32F
        Eigen::ಮಾತೃಕೆ_Xf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::ಮಾತೃಕೆ_Xf> es;
        es.compute(src_eig, esOptions);
        ಆದರೆ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ಆದರೆ ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಇಲ್ಲದಿದ್ದರೆ

    size_t elem_ಗಾತ್ರ = src.elem_ಗಾತ್ರ(), astep = align_ಗಾತ್ರ(n*elem_ಗಾತ್ರ, 16);
    Auto_ಬಫರ್<uchar> buf(n*astep + n*5*elem_ಗಾತ್ರ + 32);
    uchar* ptr = alignPtr(buf.ದತ್ತಾಂಶ(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elem_ಗಾತ್ರ*n;
    src.copy_ಗಮ್ಯ(a);
    ತಾರ್ಕಿಕ ok = type == CV_32F ?
        Jacobi(a.ptr<ದಶಮಾಂಶ>(), a.ಹೆಜ್ಜೆ, w.ptr<ದಶಮಾಂಶ>(), v.ptr<ದಶಮಾಂಶ>(), v.ಹೆಜ್ಜೆ, n, ptr) :
        Jacobi(a.ptr<ದ್ವಿಗುಣ>(), a.ಹೆಜ್ಜೆ, w.ptr<ದ್ವಿಗುಣ>(), v.ptr<ದ್ವಿಗುಣ>(), v.ಹೆಜ್ಜೆ, n, ptr);

    w.copy_ಗಮ್ಯ(_evals);
    ಹಿಂತಿರುಗಿ ok;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ _SVDcompute( ಒಳಹರಿವು_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, ಪೂರ್ಣಾಂಕ flags )
{
    Mat src = _aarr.getMat();
    ಪೂರ್ಣಾಂಕ m = src.ಅಡ್ಡಸಾಲುಗಳು, n = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಪೂರ್ಣಾಂಕ type = src.type();
    ತಾರ್ಕಿಕ compute_uv = _u.needed() || _vt.needed();
    ತಾರ್ಕಿಕ ತುಂಬಿದ_uv = (flags & SVD::ತುಂಬಿದ_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    ಆದರೆ( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = ತುಂಬಿದ_uv = ಸುಳ್ಳು;
    }

    ತಾರ್ಕಿಕ at = ಸುಳ್ಳು;
    ಆದರೆ( m < n )
    {
        std::swap(m, n);
        at = ನಿಜ;
    }

    ಪೂರ್ಣಾಂಕ urows = ತುಂಬಿದ_uv ? m : n;
    size_t esz = src.elem_ಗಾತ್ರ(), astep = align_ಗಾತ್ರ(m*esz, 16), vstep = align_ಗಾತ್ರ(n*esz, 16);
    Auto_ಬಫರ್<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.ದತ್ತಾಂಶ(), 16);
    Mat ತಾತ್ಕಾಲಿಕ_a(n, m, type, buf, astep);
    Mat ತಾತ್ಕಾಲಿಕ_w(n, 1, type, buf + urows*astep);
    Mat ತಾತ್ಕಾಲಿಕ_u(urows, m, type, buf, astep), ತಾತ್ಕಾಲಿಕ_v;

    ಆದರೆ( compute_uv )
        ತಾತ್ಕಾಲಿಕ_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    ಆದರೆ( urows > n )
        ತಾತ್ಕಾಲಿಕ_u = Scalar::all(0);

    ಆದರೆ( !at )
        transpose(src, ತಾತ್ಕಾಲಿಕ_a);
    ಇಲ್ಲದಿದ್ದರೆ
        src.copy_ಗಮ್ಯ(ತಾತ್ಕಾಲಿಕ_a);

    ಆದರೆ( type == CV_32F )
    {
        JacobiSVD(ತಾತ್ಕಾಲಿಕ_a.ptr<ದಶಮಾಂಶ>(), ತಾತ್ಕಾಲಿಕ_u.ಹೆಜ್ಜೆ, ತಾತ್ಕಾಲಿಕ_w.ptr<ದಶಮಾಂಶ>(),
              ತಾತ್ಕಾಲಿಕ_v.ptr<ದಶಮಾಂಶ>(), ತಾತ್ಕಾಲಿಕ_v.ಹೆಜ್ಜೆ, m, n, compute_uv ? urows : 0);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        JacobiSVD(ತಾತ್ಕಾಲಿಕ_a.ptr<ದ್ವಿಗುಣ>(), ತಾತ್ಕಾಲಿಕ_u.ಹೆಜ್ಜೆ, ತಾತ್ಕಾಲಿಕ_w.ptr<ದ್ವಿಗುಣ>(),
              ತಾತ್ಕಾಲಿಕ_v.ptr<ದ್ವಿಗುಣ>(), ತಾತ್ಕಾಲಿಕ_v.ಹೆಜ್ಜೆ, m, n, compute_uv ? urows : 0);
    }
    ತಾತ್ಕಾಲಿಕ_w.copy_ಗಮ್ಯ(_w);
    ಆದರೆ( compute_uv )
    {
        ಆದರೆ( !at )
        {
            ಆದರೆ( _u.needed() )
                transpose(ತಾತ್ಕಾಲಿಕ_u, _u);
            ಆದರೆ( _vt.needed() )
                ತಾತ್ಕಾಲಿಕ_v.copy_ಗಮ್ಯ(_vt);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ( _u.needed() )
                transpose(ತಾತ್ಕಾಲಿಕ_v, _u);
            ಆದರೆ( _vt.needed() )
                ತಾತ್ಕಾಲಿಕ_u.copy_ಗಮ್ಯ(_vt);
        }
    }
}


ಶೂನ್ಯ SVD::compute( ಒಳಹರಿವು_Array a, OutputArray w, OutputArray u, OutputArray vt, ಪೂರ್ಣಾಂಕ flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

ಶೂನ್ಯ SVD::compute( ಒಳಹರಿವು_Array a, OutputArray w, ಪೂರ್ಣಾಂಕ flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

ಶೂನ್ಯ SVD::ಹಿಂಭಾಗ_Subst( ಒಳಹರಿವು_Array _w, ಒಳಹರಿವು_Array _u, ಒಳಹರಿವು_Array _vt,
                     ಒಳಹರಿವು_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    ಪೂರ್ಣಾಂಕ type = w.type(), esz = (ಪೂರ್ಣಾಂಕ)w.elem_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ m = u.ಅಡ್ಡಸಾಲುಗಳು, n = vt.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, nb = rhs.ದತ್ತಾಂಶ ? rhs.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ : m, nm = std::min(m, n);
    size_t wstep = w.ಅಡ್ಡಸಾಲುಗಳು == 1 ? (size_t)esz : w.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 ? (size_t)w.ಹೆಜ್ಜೆ : (size_t)w.ಹೆಜ್ಜೆ + esz;
    Auto_ಬಫರ್<uchar> ಬಫರ್(nb*ಗಾತ್ರ(ದ್ವಿಗುಣ) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.ದತ್ತಾಂಶ && vt.ದತ್ತಾಂಶ && w.ದತ್ತಾಂಶ );
    CV_Assert( u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ >= nm && vt.ಅಡ್ಡಸಾಲುಗಳು >= nm &&
              (w.size() == ಗಾತ್ರ_2(nm, 1) || w.size() == ಗಾತ್ರ_2(1, nm) || w.size() == ಗಾತ್ರ_2(vt.ಅಡ್ಡಸಾಲುಗಳು, u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)) );
    CV_Assert( rhs.ದತ್ತಾಂಶ == 0 || (rhs.type() == type && rhs.ಅಡ್ಡಸಾಲುಗಳು == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    ಆದರೆ( type == CV_32F )
        SVBkSb(m, n, w.ptr<ದಶಮಾಂಶ>(), wstep, u.ptr<ದಶಮಾಂಶ>(), u.ಹೆಜ್ಜೆ, ಸುಳ್ಳು,
               vt.ptr<ದಶಮಾಂಶ>(), vt.ಹೆಜ್ಜೆ, ನಿಜ, rhs.ptr<ದಶಮಾಂಶ>(), rhs.ಹೆಜ್ಜೆ, nb,
               dst.ptr<ದಶಮಾಂಶ>(), dst.ಹೆಜ್ಜೆ, ಬಫರ್.ದತ್ತಾಂಶ());
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_64F )
        SVBkSb(m, n, w.ptr<ದ್ವಿಗುಣ>(), wstep, u.ptr<ದ್ವಿಗುಣ>(), u.ಹೆಜ್ಜೆ, ಸುಳ್ಳು,
               vt.ptr<ದ್ವಿಗುಣ>(), vt.ಹೆಜ್ಜೆ, ನಿಜ, rhs.ptr<ದ್ವಿಗುಣ>(), rhs.ಹೆಜ್ಜೆ, nb,
               dst.ptr<ದ್ವಿಗುಣ>(), dst.ಹೆಜ್ಜೆ, ಬಫರ್.ದತ್ತಾಂಶ());
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::ಕ್ರಿಯಾಕಾರಕ ()(ಒಳಹರಿವು_Array a, ಪೂರ್ಣಾಂಕ flags)
{
    _SVDcompute(a, w, u, vt, flags);
    ಹಿಂತಿರುಗಿ *ಇದು;
}


ಶೂನ್ಯ SVD::ಹಿಂಭಾಗ_Subst( ಒಳಹರಿವು_Array rhs, OutputArray dst ) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂಭಾಗ_Subst( w, u, vt, rhs, dst );
}

}


ಶೂನ್ಯ cv::SVDecomp(ಒಳಹರಿವು_Array src, OutputArray w, OutputArray u, OutputArray vt, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

ಶೂನ್ಯ cv::SVBackSubst(ಒಳಹರಿವು_Array w, ಒಳಹರಿವು_Array u, ಒಳಹರಿವು_Array vt, ಒಳಹರಿವು_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::ಹಿಂಭಾಗ_Subst(w, u, vt, rhs, dst);
}
