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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <limits>

#ifdef HAVE_EIGEN
#  ଯଦି defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  ଯଦି defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

ନାମାକାଶ cv
{

ପୂର୍ଣ୍ଣସଂଖ୍ୟା LU(ଦଶମିକ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦଶମିକ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    ଫେରନ୍ତୁ hal::LU32f(A, astep, m, b, bstep, n);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା LU(ଦ୍ୱିଗୁଣ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    ଫେରନ୍ତୁ hal::LU64f(A, astep, m, b, bstep, n);
}

ତାର୍କିକ Cholesky(ଦଶମିକ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦଶମିକ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    ଫେରନ୍ତୁ hal::Cholesky32f(A, astep, m, b, bstep, n);
}

ତାର୍କିକ Cholesky(ଦ୍ୱିଗୁଣ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    ଫେରନ୍ତୁ hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> ସ୍ଥିର ଧାଡ଼ିଭିତରେ _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    ଯଦି( a > b )
    {
        b /= a;
        ଫେରନ୍ତୁ a*std::sqrt(1 + b*b);
    }
    ଯଦି( b > 0 )
    {
        a /= b;
        ଫେରନ୍ତୁ b*std::sqrt(1 + a*a);
    }
    ଫେରନ୍ତୁ 0;
}


template<typename _Tp> ତାର୍କିକ
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, uchar* buf )
{
    ଅଚଳ _Tp eps = std::numeric_limits<_Tp>::epsilon();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k, m;

    astep /= ଆକାର(A[0]);
    ଯଦି( V )
    {
        vstep /= ଆକାର(V[0]);
        ପାଇଁ( i = 0; i < n; i++ )
        {
            ପାଇଁ( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iters, maxIters = n*n*30;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* indR = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)alignPtr(buf, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* indC = indR + n;
    _Tp mv = (_Tp)0;

    ପାଇଁ( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        ଯଦି( k < n - 1 )
        {
            ପାଇଁ( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                ଯଦି( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        ଯଦି( k > 0 )
        {
            ପାଇଁ( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                ଯଦି( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    ଯଦି( n > 1 ) ପାଇଁ( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        ପାଇଁ( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            ଯଦି( mv < val )
                mv = val, k = i;
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = indR[k];
        ପାଇଁ( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            ଯଦି( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        ଯଦି( std::abs(p) <= eps )
            ଭାଙ୍ଗ;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        ଯଦି( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        ପାଇଁ( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        ପାଇଁ( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        ପାଇଁ( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        ଯଦି( V )
            ପାଇଁ( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        ପାଇଁ( j = 0; j < 2; j++ )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = j == 0 ? k : l;
            ଯଦି( idx < n - 1 )
            {
                ପାଇଁ( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    ଯଦି( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            ଯଦି( idx > 0 )
            {
                ପାଇଁ( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    ଯଦି( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    ପାଇଁ( k = 0; k < n-1; k++ )
    {
        m = k;
        ପାଇଁ( i = k+1; i < n; i++ )
        {
            ଯଦି( W[m] < W[i] )
                m = i;
        }
        ଯଦି( k != m )
        {
            std::swap(W[m], W[k]);
            ଯଦି( V )
                ପାଇଁ( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ତାର୍କିକ Jacobi( ଦଶମିକ* S, size_t sstep, ଦଶମିକ* e, ଦଶମିକ* E, size_t estep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, uchar* buf )
{
    ଫେରନ୍ତୁ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

ସ୍ଥିର ତାର୍କିକ Jacobi( ଦ୍ୱିଗୁଣ* S, size_t sstep, ଦ୍ୱିଗୁଣ* e, ଦ୍ୱିଗୁଣ* E, size_t estep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, uchar* buf )
{
    ଫେରନ୍ତୁ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> ସଂରଚନା VBLAS
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dot(ଅଚଳ T*, ଅଚଳ T*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, T*) ଅଚଳ { ଫେରନ୍ତୁ 0; }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା givens(T*, T*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, T, T) ଅଚଳ { ଫେରନ୍ତୁ 0; }
};

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
template<> ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VBLAS<ଦଶମିକ>::dot(ଅଚଳ ଦଶମିକ* a, ଅଚଳ ଦଶମିକ* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ* result) ଅଚଳ
{
    ଯଦି( n < 2*VTraits<v_float32>::vlanes() )
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
    v_float32 s0 = vx_setzero_f32();
    ପାଇଁ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    ଫେରନ୍ତୁ k;
}


template<> ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VBLAS<ଦଶମିକ>::givens(ଦଶମିକ* a, ଦଶମିକ* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ c, ଦଶମିକ s) ଅଚଳ
{
    ଯଦି( n < VTraits<v_float32>::vlanes())
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    ପାଇଁ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ଫେରନ୍ତୁ k;
}


#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VBLAS<ଦ୍ୱିଗୁଣ>::dot(ଅଚଳ ଦ୍ୱିଗୁଣ* a, ଅଚଳ ଦ୍ୱିଗୁଣ* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦ୍ୱିଗୁଣ* result) ଅଚଳ
{
    ଯଦି( n < 2*VTraits<v_float64>::vlanes() )
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
    v_float64 s0 = vx_setzero_f64();
    ପାଇଁ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    ଫେରନ୍ତୁ k;
}


template<> ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VBLAS<ଦ୍ୱିଗୁଣ>::givens(ଦ୍ୱିଗୁଣ* a, ଦ୍ୱିଗୁଣ* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦ୍ୱିଗୁଣ c, ଦ୍ୱିଗୁଣ s) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    ପାଇଁ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ଫେରନ୍ତୁ k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> ଶୂନ୍ୟ
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n1, ଦ୍ୱିଗୁଣ minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<ଦ୍ୱିଗୁଣ> Wbuf(n);
    ଦ୍ୱିଗୁଣ* W = Wbuf.data();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    ଦ୍ୱିଗୁଣ sd;
    astep /= ଆକାର(At[0]);
    vstep /= ଆକାର(Vt[0]);

    ପାଇଁ( i = 0; i < n; i++ )
    {
        ପାଇଁ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ଦ୍ୱିଗୁଣ)t*t;
        }
        W[i] = sd;

        ଯଦି( Vt )
        {
            ପାଇଁ( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    ପାଇଁ( iter = 0; iter < max_iter; iter++ )
    {
        ତାର୍କିକ changed = ମିଥ୍ୟା;

        ପାଇଁ( i = 0; i < n-1; i++ )
            ପାଇଁ( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                ଦ୍ୱିଗୁଣ a = W[i], p = 0, b = W[j];

                ପାଇଁ( k = 0; k < m; k++ )
                    p += (ଦ୍ୱିଗୁଣ)Ai[k]*Aj[k];

                ଯଦି( std::abs(p) <= eps*std::sqrt((ଦ୍ୱିଗୁଣ)a*b) )
                    ଜାରି;

                p *= 2;
                ଦ୍ୱିଗୁଣ beta = a - b, gamma = hypot((ଦ୍ୱିଗୁଣ)p, beta);
                ଯଦି( beta < 0 )
                {
                    ଦ୍ୱିଗୁଣ delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                ଅନ୍ୟଥା
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                ପାଇଁ( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (ଦ୍ୱିଗୁଣ)t0*t0; b += (ଦ୍ୱିଗୁଣ)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = ସତ୍ୟ;

                ଯଦି( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    ପାଇଁ( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        ଯଦି( !changed )
            ଭାଙ୍ଗ;
    }

    ପାଇଁ( i = 0; i < n; i++ )
    {
        ପାଇଁ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ଦ୍ୱିଗୁଣ)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    ପାଇଁ( i = 0; i < n-1; i++ )
    {
        j = i;
        ପାଇଁ( k = i+1; k < n; k++ )
        {
            ଯଦି( W[j] < W[k] )
                j = k;
        }
        ଯଦି( i != j )
        {
            std::swap(W[i], W[j]);
            ଯଦି( Vt )
            {
                ପାଇଁ( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                ପାଇଁ( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    ପାଇଁ( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    ଯଦି( !Vt )
        ଫେରନ୍ତୁ;

    RNG rng(0x12345678);
    ପାଇଁ( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ଅଚଳ _Tp val0 = (_Tp)(1./m);
            ପାଇଁ( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            ପାଇଁ( iter = 0; iter < 2; iter++ )
            {
                ପାଇଁ( j = 0; j < i; j++ )
                {
                    sd = 0;
                    ପାଇଁ( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    ପାଇଁ( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    ପାଇଁ( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            ପାଇଁ( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (ଦ୍ୱିଗୁଣ)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        ପାଇଁ( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


ସ୍ଥିର ଶୂନ୍ୟ JacobiSVD(ଦଶମିକ* At, size_t astep, ଦଶମିକ* W, ଦଶମିକ* Vt, size_t vstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n1=-1)
{
    hal::SVD32f(At, astep, W, ଖାଲି, astep, Vt, vstep, m, n, n1);
}

ସ୍ଥିର ଶୂନ୍ୟ JacobiSVD(ଦ୍ୱିଗୁଣ* At, size_t astep, ଦ୍ୱିଗୁଣ* W, ଦ୍ୱିଗୁଣ* Vt, size_t vstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n1=-1)
{
    hal::SVD64f(At, astep, W, ଖାଲି, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
decodeSVDParameters(ଅଚଳ fptype* U, ଅଚଳ fptype* Vt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n1)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା halSVDFlag = 0;
    ଯଦି(Vt == ଖାଲି)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    ଅନ୍ୟଥା ଯଦି(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        ଯଦି(U == ଖାଲି)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ଅନ୍ୟଥା ଯଦି(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        ଯଦି(U == ଖାଲି)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ଫେରନ୍ତୁ halSVDFlag;
}

ଶୂନ୍ୟ hal::SVD32f(ଦଶମିକ* At, size_t astep, ଦଶମିକ* W, ଦଶମିକ* U, size_t ustep, ଦଶମିକ* Vt, size_t vstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

ଶୂନ୍ୟ hal::SVD64f(ଦ୍ୱିଗୁଣ* At, size_t astep, ଦ୍ୱିଗୁଣ* W, ଦ୍ୱିଗୁଣ* U, size_t ustep, ଦ୍ୱିଗୁଣ* Vt, size_t vstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> ସ୍ଥିର ଶୂନ୍ୟ
MatrAXPY( ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅଚଳ T1* x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx,
         ଅଚଳ T2* a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inca, T3* y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
         #ଯଦି CV_ENABLE_UNROLLED
        ପାଇଁ(; j <= n - 4; j += 4 )
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
        ପାଇଁ( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ
SVBkSbImpl_( ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅଚଳ T* w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା incw,
       ଅଚଳ T* u, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldu, ତାର୍କିକ uT,
       ଅଚଳ T* v, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldv, ତାର୍କିକ vT,
       ଅଚଳ T* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nb,
       T* x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldx, ଦ୍ୱିଗୁଣ* buffer, T eps )
{
    ଦ୍ୱିଗୁଣ threshold = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, nm = std::min(m, n);

    ଯଦି( !b )
        nb = m;

    ପାଇଁ( i = 0; i < n; i++ )
        ପାଇଁ( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    ପାଇଁ( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    ପାଇଁ( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        ଦ୍ୱିଗୁଣ wi = w[i*incw];
        ଯଦି( (ଦ୍ୱିଗୁଣ)std::abs(wi) <= threshold )
            ଜାରି;
        wi = 1/wi;

        ଯଦି( nb == 1 )
        {
            ଦ୍ୱିଗୁଣ s = 0;
            ଯଦି( b )
                ପାଇଁ( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            ଅନ୍ୟଥା
                s = u[0];
            s *= wi;

            ପାଇଁ( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        ଅନ୍ୟଥା
        {
            ଯଦି( b )
            {
                ପାଇଁ( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                ପାଇଁ( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            ଅନ୍ୟଥା
            {
                ପାଇଁ( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
SVBkSb( ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅଚଳ ଦଶମିକ* w, size_t wstep,
        ଅଚଳ ଦଶମିକ* u, size_t ustep, ତାର୍କିକ uT,
        ଅଚଳ ଦଶମିକ* v, size_t vstep, ତାର୍କିକ vT,
        ଅଚଳ ଦଶମିକ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nb,
        ଦଶମିକ* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(wstep/ଆକାର(w[0])) : 1,
                u, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ustep/ଆକାର(u[0])), uT,
                v, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(vstep/ଆକାର(v[0])), vT,
                b, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(bstep/ଆକାର(b[0])), nb,
                x, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(xstep/ଆକାର(x[0])),
                (ଦ୍ୱିଗୁଣ*)alignPtr(buffer, ଆକାର(ଦ୍ୱିଗୁଣ)), (ଦଶମିକ)(DBL_EPSILON*2) );
}

ସ୍ଥିର ଶୂନ୍ୟ
SVBkSb( ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅଚଳ ଦ୍ୱିଗୁଣ* w, size_t wstep,
       ଅଚଳ ଦ୍ୱିଗୁଣ* u, size_t ustep, ତାର୍କିକ uT,
       ଅଚଳ ଦ୍ୱିଗୁଣ* v, size_t vstep, ତାର୍କିକ vT,
       ଅଚଳ ଦ୍ୱିଗୁଣ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nb,
       ଦ୍ୱିଗୁଣ* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(wstep/ଆକାର(w[0])) : 1,
                u, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(ustep/ଆକାର(u[0])), uT,
                v, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(vstep/ଆକାର(v[0])), vT,
                b, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(bstep/ଆକାର(b[0])), nb,
                x, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(xstep/ଆକାର(x[0])),
                (ଦ୍ୱିଗୁଣ*)alignPtr(buffer, ଆକାର(ଦ୍ୱିଗୁଣ)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((ଦ୍ୱିଗୁଣ)m(0,0)*m(1,1) - (ଦ୍ୱିଗୁଣ)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((ଦ୍ୱିଗୁଣ)m(1,1)*m(2,2) - (ଦ୍ୱିଗୁଣ)m(1,2)*m(2,1)) -  \
                   m(0,1)*((ଦ୍ୱିଗୁଣ)m(1,0)*m(2,2) - (ଦ୍ୱିଗୁଣ)m(1,2)*m(2,0)) +  \
                   m(0,2)*((ଦ୍ୱିଗୁଣ)m(1,0)*m(2,1) - (ଦ୍ୱିଗୁଣ)m(1,1)*m(2,0)))

ଦ୍ୱିଗୁଣ determinant( ନିବେଶ_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    ଦ୍ୱିଗୁଣ result = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    ଅଚଳ uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((ଦଶମିକ*)(m + y*step))[x]
    #define Md(y, x) ((ଦ୍ୱିଗୁଣ*)(m + y*step))[x]

    ଯଦି( type == CV_32F )
    {
        ଯଦି( rows == 2 )
            result = det2(Mf);
        ଅନ୍ୟଥା ଯଦି( rows == 3 )
            result = det3(Mf);
        ଅନ୍ୟଥା ଯଦି( rows == 1 )
            result = Mf(0,0);
        ଅନ୍ୟଥା
        {
            size_t bufSize = rows*rows*ଆକାର(ଦଶମିକ);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<ଦଶମିକ>(), a.step, rows, 0, 0, 0);
            ଯଦି( result )
            {
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < rows; i++ )
                    result *= a.at<ଦଶମିକ>(i,i);
            }
        }
    }
    ଅନ୍ୟଥା
    {
        ଯଦି( rows == 2 )
            result = det2(Md);
        ଅନ୍ୟଥା ଯଦି( rows == 3 )
            result = det3(Md);
        ଅନ୍ୟଥା ଯଦି( rows == 1 )
            result = Md(0,0);
        ଅନ୍ୟଥା
        {
            size_t bufSize = rows*rows*ଆକାର(ଦ୍ୱିଗୁଣ);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<ଦ୍ୱିଗୁଣ>(), a.step, rows, 0, 0, 0);
            ଯଦି( result )
            {
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < rows; i++ )
                    result *= a.at<ଦ୍ୱିଗୁଣ>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    ଫେରନ୍ତୁ result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((ଦଶମିକ*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((ଦ୍ୱିଗୁଣ*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((ଦଶମିକ*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((ଦ୍ୱିଗୁଣ*)(dstdata + y*dststep))[x]

ଦ୍ୱିଗୁଣ invert( ନିବେଶ_Array _src, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method )
{
    CV_INSTRUMENT_REGION();

    ତାର୍କିକ result = ମିଥ୍ୟା;
    Mat src = _src.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = src.rows, n = src.cols;

    ଯଦି( method == DECOMP_SVD )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + ଆକାର(ଦ୍ୱିଗୁଣ));
        uchar* buf = alignPtr((uchar*)_buf.data(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        ଫେରନ୍ତୁ type == CV_32F ?
            (w.ptr<ଦଶମିକ>()[0] >= FLT_EPSILON ?
             w.ptr<ଦଶମିକ>()[n-1]/w.ptr<ଦଶମିକ>()[0] : 0) :
            (w.ptr<ଦ୍ୱିଗୁଣ>()[0] >= DBL_EPSILON ?
             w.ptr<ଦ୍ୱିଗୁଣ>()[n-1]/w.ptr<ଦ୍ୱିଗୁଣ>()[0] : 0);
    }

    CV_Assert( m == n );

    ଯଦି( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + ଆକାର(ଦ୍ୱିଗୁଣ));
        uchar* buf = alignPtr((uchar*)_buf.data(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        ଫେରନ୍ତୁ type == CV_32F ?
        (w.ptr<ଦଶମିକ>()[0] >= FLT_EPSILON ?
         w.ptr<ଦଶମିକ>()[n-1]/w.ptr<ଦଶମିକ>()[0] : 0) :
        (w.ptr<ଦ୍ୱିଗୁଣ>()[0] >= DBL_EPSILON ?
         w.ptr<ଦ୍ୱିଗୁଣ>()[n-1]/w.ptr<ଦ୍ୱିଗୁଣ>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    ଯଦି( n <= 3 )
    {
        ଅଚଳ uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        ଯଦି( n == 2 )
        {
            ଯଦି( type == CV_32FC1 )
            {
                ଦ୍ୱିଗୁଣ d = det2(Sf);
                ଯଦି( d != 0. )
                {
                    result = ସତ୍ୟ;
                    d = 1./d;
                #ଯଦି CV_SIMD128
                    ଅଚଳ ଦଶମିକ d_32f = (ଦଶମିକ)d;
                    ଅଚଳ v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((ଅଚଳ ଦଶମିକ *)srcdata, (ଅଚଳ ଦଶମିକ *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_ତଳ(v_rotate_ଡାହାଣ<1>(s0), s0));
                    v_store_ତଳ((ଦଶମିକ*)dstdata, s0);
                    v_store_ଉପର((ଦଶମିକ*)(dstdata + dststep), s0);
                #ଅନ୍ୟଥା
                    ଦ୍ୱିଗୁଣ t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (ଦଶମିକ)t0;
                    Df(0,0) = (ଦଶମିକ)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (ଦଶମିକ)t0;
                    Df(1,0) = (ଦଶମିକ)t1;
                #endif
                }
            }
            ଅନ୍ୟଥା
            {
                ଦ୍ୱିଗୁଣ d = det2(Sd);
                ଯଦି( d != 0. )
                {
                    result = ସତ୍ୟ;
                    d = 1./d;
                #ଯଦି CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((ଅଚଳ ଦ୍ୱିଗୁଣ *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((ଅଚଳ ଦ୍ୱିଗୁଣ *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<ଦ୍ୱିଗୁଣ>(0), v_extract<1>(s0, s1));//12
                    v_store((ଦ୍ୱିଗୁଣ*)dstdata, v_combine_ତଳ(sm, ss));//31
                    v_store((ଦ୍ୱିଗୁଣ*)(dstdata + dststep), v_combine_ଉପର(ss, sm));//20
                #ଅନ୍ୟଥା
                    ଦ୍ୱିଗୁଣ t0, t1;
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
        ଅନ୍ୟଥା ଯଦି( n == 3 )
        {
            ଯଦି( type == CV_32FC1 )
            {
                ଦ୍ୱିଗୁଣ d = det3(Sf);

                ଯଦି( d != 0. )
                {
                    ଦ୍ୱିଗୁଣ t[12];

                    result = ସତ୍ୟ;
                    d = 1./d;
                    t[0] = (((ଦ୍ୱିଗୁଣ)Sf(1,1) * Sf(2,2) - (ଦ୍ୱିଗୁଣ)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((ଦ୍ୱିଗୁଣ)Sf(0,2) * Sf(2,1) - (ଦ୍ୱିଗୁଣ)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((ଦ୍ୱିଗୁଣ)Sf(0,1) * Sf(1,2) - (ଦ୍ୱିଗୁଣ)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((ଦ୍ୱିଗୁଣ)Sf(1,2) * Sf(2,0) - (ଦ୍ୱିଗୁଣ)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((ଦ୍ୱିଗୁଣ)Sf(0,0) * Sf(2,2) - (ଦ୍ୱିଗୁଣ)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((ଦ୍ୱିଗୁଣ)Sf(0,2) * Sf(1,0) - (ଦ୍ୱିଗୁଣ)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((ଦ୍ୱିଗୁଣ)Sf(1,0) * Sf(2,1) - (ଦ୍ୱିଗୁଣ)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((ଦ୍ୱିଗୁଣ)Sf(0,1) * Sf(2,0) - (ଦ୍ୱିଗୁଣ)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((ଦ୍ୱିଗୁଣ)Sf(0,0) * Sf(1,1) - (ଦ୍ୱିଗୁଣ)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (ଦଶମିକ)t[0]; Df(0,1) = (ଦଶମିକ)t[1]; Df(0,2) = (ଦଶମିକ)t[2];
                    Df(1,0) = (ଦଶମିକ)t[3]; Df(1,1) = (ଦଶମିକ)t[4]; Df(1,2) = (ଦଶମିକ)t[5];
                    Df(2,0) = (ଦଶମିକ)t[6]; Df(2,1) = (ଦଶମିକ)t[7]; Df(2,2) = (ଦଶମିକ)t[8];
                }
            }
            ଅନ୍ୟଥା
            {
                ଦ୍ୱିଗୁଣ d = det3(Sd);
                ଯଦି( d != 0. )
                {
                    result = ସତ୍ୟ;
                    d = 1./d;
                    ଦ୍ୱିଗୁଣ t[9];

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
        ଅନ୍ୟଥା
        {
            CV_Assert( n == 1 );

            ଯଦି( type == CV_32FC1 )
            {
                ଦ୍ୱିଗୁଣ d = Sf(0,0);
                ଯଦି( d != 0. )
                {
                    result = ସତ୍ୟ;
                    Df(0,0) = (ଦଶମିକ)(1./d);
                }
            }
            ଅନ୍ୟଥା
            {
                ଦ୍ୱିଗୁଣ d = Sd(0,0);
                ଯଦି( d != 0. )
                {
                    result = ସତ୍ୟ;
                    Dd(0,0) = 1./d;
                }
            }
        }
        ଯଦି( !result )
            dst = Scalar(0);
        ଫେରନ୍ତୁ result;
    }

   ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    ଯଦି( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<ଦଶମିକ>(), src1.step, n, dst.ptr<ଦଶମିକ>(), dst.step, n) != 0;
    ଅନ୍ୟଥା ଯଦି( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<ଦ୍ୱିଗୁଣ>(), src1.step, n, dst.ptr<ଦ୍ୱିଗୁଣ>(), dst.step, n) != 0;
    ଅନ୍ୟଥା ଯଦି( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<ଦଶମିକ>(), src1.step, n, dst.ptr<ଦଶମିକ>(), dst.step, n);
    ଅନ୍ୟଥା
        result = hal::Cholesky64f(src1.ptr<ଦ୍ୱିଗୁଣ>(), src1.step, n, dst.ptr<ଦ୍ୱିଗୁଣ>(), dst.step, n);

    ଯଦି( !result )
        dst = Scalar(0);

    ଫେରନ୍ତୁ result;
}

UMat UMat::inv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା method) ଅଚଳ
{
    UMat m;
    invert(*ଏହି, m, method);
    ଫେରନ୍ତୁ m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

ତାର୍କିକ solve( ନିବେଶ_Array _src, ନିବେଶ_Array _src2arg, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method )
{
    CV_INSTRUMENT_REGION();

    ତାର୍କିକ result = ସତ୍ୟ;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src.type();
    ତାର୍କିକ is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    ଯଦି( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((ଦଶମିକ*)(bdata + y*src2step))[0]
        #define bd(y) ((ଦ୍ୱିଗୁଣ*)(bdata + y*src2step))[0]

        ଅଚଳ uchar* srcdata = src.ptr();
        ଅଚଳ uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        ଯଦି( src.rows == 2 )
        {
            ଯଦି( type == CV_32FC1 )
            {
                ଦ୍ୱିଗୁଣ d = det2(Sf);
                ଯଦି( d != 0. )
                {
                    ଦ୍ୱିଗୁଣ t;
                    d = 1./d;
                    t = (ଦଶମିକ)(((ଦ୍ୱିଗୁଣ)bf(0)*Sf(1,1) - (ଦ୍ୱିଗୁଣ)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (ଦଶମିକ)(((ଦ୍ୱିଗୁଣ)bf(1)*Sf(0,0) - (ଦ୍ୱିଗୁଣ)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (ଦଶମିକ)t;
                }
                ଅନ୍ୟଥା
                    result = ମିଥ୍ୟା;
            }
            ଅନ୍ୟଥା
            {
                ଦ୍ୱିଗୁଣ d = det2(Sd);
                ଯଦି( d != 0. )
                {
                    ଦ୍ୱିଗୁଣ t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                ଅନ୍ୟଥା
                    result = ମିଥ୍ୟା;
            }
        }
        ଅନ୍ୟଥା ଯଦି( src.rows == 3 )
        {
            ଯଦି( type == CV_32FC1 )
            {
                ଦ୍ୱିଗୁଣ d = det3(Sf);
                ଯଦି( d != 0. )
                {
                    ଦଶମିକ t[3];
                    d = 1./d;

                    t[0] = (ଦଶମିକ)(d*
                           (bf(0)*((ଦ୍ୱିଗୁଣ)Sf(1,1)*Sf(2,2) - (ଦ୍ୱିଗୁଣ)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((ଦ୍ୱିଗୁଣ)bf(1)*Sf(2,2) - (ଦ୍ୱିଗୁଣ)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((ଦ୍ୱିଗୁଣ)bf(1)*Sf(2,1) - (ଦ୍ୱିଗୁଣ)Sf(1,1)*bf(2))));

                    t[1] = (ଦଶମିକ)(d*
                           (Sf(0,0)*(ଦ୍ୱିଗୁଣ)(bf(1)*Sf(2,2) - (ଦ୍ୱିଗୁଣ)Sf(1,2)*bf(2)) -
                            bf(0)*((ଦ୍ୱିଗୁଣ)Sf(1,0)*Sf(2,2) - (ଦ୍ୱିଗୁଣ)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((ଦ୍ୱିଗୁଣ)Sf(1,0)*bf(2) - (ଦ୍ୱିଗୁଣ)bf(1)*Sf(2,0))));

                    t[2] = (ଦଶମିକ)(d*
                           (Sf(0,0)*((ଦ୍ୱିଗୁଣ)Sf(1,1)*bf(2) - (ଦ୍ୱିଗୁଣ)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((ଦ୍ୱିଗୁଣ)Sf(1,0)*bf(2) - (ଦ୍ୱିଗୁଣ)bf(1)*Sf(2,0)) +
                            bf(0)*((ଦ୍ୱିଗୁଣ)Sf(1,0)*Sf(2,1) - (ଦ୍ୱିଗୁଣ)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                ଅନ୍ୟଥା
                    result = ମିଥ୍ୟା;
            }
            ଅନ୍ୟଥା
            {
                ଦ୍ୱିଗୁଣ d = det3(Sd);
                ଯଦି( d != 0. )
                {
                    ଦ୍ୱିଗୁଣ t[9];

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
                ଅନ୍ୟଥା
                    result = ମିଥ୍ୟା;
            }
        }
        ଅନ୍ୟଥା
        {
            CV_Assert( src.rows == 1 );

            ଯଦି( type == CV_32FC1 )
            {
                ଦ୍ୱିଗୁଣ d = Sf(0,0);
                ଯଦି( d != 0. )
                    Df(0,0) = (ଦଶମିକ)(bf(0)/d);
                ଅନ୍ୟଥା
                    result = ମିଥ୍ୟା;
            }
            ଅନ୍ୟଥା
            {
                ଦ୍ୱିଗୁଣ d = Sd(0,0);
                ଯଦି( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                ଅନ୍ୟଥା
                    result = ମିଥ୍ୟା;
            }
        }
        ଫେରନ୍ତୁ result;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    ଯଦି( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    ଯଦି( m == n )
        is_normal = ମିଥ୍ୟା;
    ଅନ୍ୟଥା ଯଦି( is_normal )
    {
        m_ = n;
        ଯଦି( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    ଯଦି( is_normal )
        bufsize += n*nb*esz;
    ଯଦି( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*ଆକାର(ଦ୍ୱିଗୁଣ) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    ଯଦି( is_normal )
        mulTransposed(src, a, ସତ୍ୟ);
    ଅନ୍ୟଥା ଯଦି( method != DECOMP_SVD )
        src.copyTo(a);
    ଅନ୍ୟଥା
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    ଯଦି( !is_normal )
    {
        ଯଦି( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    ଅନ୍ୟଥା
    {
        // a'*b
        ଯଦି( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        ଅନ୍ୟଥା
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    ଯଦି( method == DECOMP_LU )
    {
        ଯଦି( type == CV_32F )
            result = hal::LU32f(a.ptr<ଦଶମିକ>(), a.step, n, dst.ptr<ଦଶମିକ>(), dst.step, nb) != 0;
        ଅନ୍ୟଥା
            result = hal::LU64f(a.ptr<ଦ୍ୱିଗୁଣ>(), a.step, n, dst.ptr<ଦ୍ୱିଗୁଣ>(), dst.step, nb) != 0;
    }
    ଅନ୍ୟଥା ଯଦି( method == DECOMP_CHOLESKY )
    {
        ଯଦି( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<ଦଶମିକ>(), a.step, n, dst.ptr<ଦଶମିକ>(), dst.step, nb);
        ଅନ୍ୟଥା
            result = hal::Cholesky64f(a.ptr<ଦ୍ୱିଗୁଣ>(), a.step, n, dst.ptr<ଦ୍ୱିଗୁଣ>(), dst.step, nb);
    }
    ଅନ୍ୟଥା ଯଦି( method == DECOMP_QR )
    {
        Mat rhsMat;
        ଯଦି( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        ଅନ୍ୟଥା
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        ଯଦି( type == CV_32F )
            result = hal::QR32f(a.ptr<ଦଶମିକ>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<ଦଶମିକ>(), rhsMat.step, ଖାଲି) != 0;
        ଅନ୍ୟଥା
            result = hal::QR64f(a.ptr<ଦ୍ୱିଗୁଣ>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<ଦ୍ୱିଗୁଣ>(), rhsMat.step, ଖାଲି) != 0;

        ଯଦି (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    ଅନ୍ୟଥା
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        ଯଦି( method == DECOMP_EIG )
        {
            ଯଦି( type == CV_32F )
                Jacobi(a.ptr<ଦଶମିକ>(), a.step, w.ptr<ଦଶମିକ>(), v.ptr<ଦଶମିକ>(), v.step, n, ptr);
            ଅନ୍ୟଥା
                Jacobi(a.ptr<ଦ୍ୱିଗୁଣ>(), a.step, w.ptr<ଦ୍ୱିଗୁଣ>(), v.ptr<ଦ୍ୱିଗୁଣ>(), v.step, n, ptr);
            u = v;
        }
        ଅନ୍ୟଥା
        {
            ଯଦି( type == CV_32F )
                JacobiSVD(a.ptr<ଦଶମିକ>(), a.step, w.ptr<ଦଶମିକ>(), v.ptr<ଦଶମିକ>(), v.step, m_, n);
            ଅନ୍ୟଥା
                JacobiSVD(a.ptr<ଦ୍ୱିଗୁଣ>(), a.step, w.ptr<ଦ୍ୱିଗୁଣ>(), v.ptr<ଦ୍ୱିଗୁଣ>(), v.step, m_, n);
            u = a;
        }

        ଯଦି( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<ଦଶମିକ>(), 0, u.ptr<ଦଶମିକ>(), u.step, ସତ୍ୟ,
                   v.ptr<ଦଶମିକ>(), v.step, ସତ୍ୟ, src2.ptr<ଦଶମିକ>(),
                   src2.step, nb, dst.ptr<ଦଶମିକ>(), dst.step, ptr);
        }
        ଅନ୍ୟଥା
        {
            SVBkSb(m_, n, w.ptr<ଦ୍ୱିଗୁଣ>(), 0, u.ptr<ଦ୍ୱିଗୁଣ>(), u.step, ସତ୍ୟ,
                   v.ptr<ଦ୍ୱିଗୁଣ>(), v.step, ସତ୍ୟ, src2.ptr<ଦ୍ୱିଗୁଣ>(),
                   src2.step, nb, dst.ptr<ଦ୍ୱିଗୁଣ>(), dst.step, ptr);
        }
        result = ସତ୍ୟ;
    }

    ଯଦି( !result )
        dst = Scalar(0);

    ଫେରନ୍ତୁ result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

ତାର୍କିକ eigen( ନିବେଶ_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    ଯଦି( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    ଅଚଳ ତାର୍କିକ evecNeeded = _evects.needed();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    ଯଦି ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        ଯଦି ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ଯଦି ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
    } ଅନ୍ୟଥା { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        ଯଦି ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ଯଦି ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#ଅନ୍ୟଥା

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    ତାର୍କିକ ok = type == CV_32F ?
        Jacobi(a.ptr<ଦଶମିକ>(), a.step, w.ptr<ଦଶମିକ>(), v.ptr<ଦଶମିକ>(), v.step, n, ptr) :
        Jacobi(a.ptr<ଦ୍ୱିଗୁଣ>(), a.step, w.ptr<ଦ୍ୱିଗୁଣ>(), v.ptr<ଦ୍ୱିଗୁଣ>(), v.step, n, ptr);

    w.copyTo(_evals);
    ଫେରନ୍ତୁ ok;
#endif
}

ସ୍ଥିର ଶୂନ୍ୟ _SVDcompute( ନିବେଶ_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    Mat src = _aarr.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = src.rows, n = src.cols;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src.type();
    ତାର୍କିକ compute_uv = _u.needed() || _vt.needed();
    ତାର୍କିକ full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    ଯଦି( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = ମିଥ୍ୟା;
    }

    ତାର୍କିକ at = ମିଥ୍ୟା;
    ଯଦି( m < n )
    {
        std::swap(m, n);
        at = ସତ୍ୟ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    ଯଦି( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    ଯଦି( urows > n )
        temp_u = Scalar::all(0);

    ଯଦି( !at )
        transpose(src, temp_a);
    ଅନ୍ୟଥା
        src.copyTo(temp_a);

    ଯଦି( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<ଦଶମିକ>(), temp_u.step, temp_w.ptr<ଦଶମିକ>(),
              temp_v.ptr<ଦଶମିକ>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    ଅନ୍ୟଥା
    {
        JacobiSVD(temp_a.ptr<ଦ୍ୱିଗୁଣ>(), temp_u.step, temp_w.ptr<ଦ୍ୱିଗୁଣ>(),
              temp_v.ptr<ଦ୍ୱିଗୁଣ>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    ଯଦି( compute_uv )
    {
        ଯଦି( !at )
        {
            ଯଦି( _u.needed() )
                transpose(temp_u, _u);
            ଯଦି( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        ଅନ୍ୟଥା
        {
            ଯଦି( _u.needed() )
                transpose(temp_v, _u);
            ଯଦି( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


ଶୂନ୍ୟ SVD::compute( ନିବେଶ_Array a, OutputArray w, OutputArray u, OutputArray vt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

ଶୂନ୍ୟ SVD::compute( ନିବେଶ_Array a, OutputArray w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

ଶୂନ୍ୟ SVD::backSubst( ନିବେଶ_Array _w, ନିବେଶ_Array _u, ନିବେଶ_Array _vt,
                     ନିବେଶ_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = w.type(), esz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)w.elemSize();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*ଆକାର(ଦ୍ୱିଗୁଣ) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    ଯଦି( type == CV_32F )
        SVBkSb(m, n, w.ptr<ଦଶମିକ>(), wstep, u.ptr<ଦଶମିକ>(), u.step, ମିଥ୍ୟା,
               vt.ptr<ଦଶମିକ>(), vt.step, ସତ୍ୟ, rhs.ptr<ଦଶମିକ>(), rhs.step, nb,
               dst.ptr<ଦଶମିକ>(), dst.step, buffer.data());
    ଅନ୍ୟଥା ଯଦି( type == CV_64F )
        SVBkSb(m, n, w.ptr<ଦ୍ୱିଗୁଣ>(), wstep, u.ptr<ଦ୍ୱିଗୁଣ>(), u.step, ମିଥ୍ୟା,
               vt.ptr<ଦ୍ୱିଗୁଣ>(), vt.step, ସତ୍ୟ, rhs.ptr<ଦ୍ୱିଗୁଣ>(), rhs.step, nb,
               dst.ptr<ଦ୍ୱିଗୁଣ>(), dst.step, buffer.data());
    ଅନ୍ୟଥା
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(ନିବେଶ_Array a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    _SVDcompute(a, w, u, vt, flags);
    ଫେରନ୍ତୁ *ଏହି;
}


ଶୂନ୍ୟ SVD::backSubst( ନିବେଶ_Array rhs, OutputArray dst ) ଅଚଳ
{
    backSubst( w, u, vt, rhs, dst );
}

}


ଶୂନ୍ୟ cv::SVDecomp(ନିବେଶ_Array src, OutputArray w, OutputArray u, OutputArray vt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

ଶୂନ୍ୟ cv::SVBackSubst(ନିବେଶ_Array w, ନିବେଶ_Array u, ନିବେଶ_Array vt, ନିବେଶ_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
