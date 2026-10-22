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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <limits>

#ifdef HAVE_EIGEN
#  എങ്കിൽ defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  എങ്കിൽ defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

നാമാകാശ cv
{

പൂർണ്ണസംഖ്യ LU(ദശാംശം* A, size_t astep, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    തിരികെ hal::LU32f(A, astep, m, b, bstep, n);
}

പൂർണ്ണസംഖ്യ LU(ഇരട്ട* A, size_t astep, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    തിരികെ hal::LU64f(A, astep, m, b, bstep, n);
}

തർക്കം Cholesky(ദശാംശം* A, size_t astep, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    തിരികെ hal::Cholesky32f(A, astep, m, b, bstep, n);
}

തർക്കം Cholesky(ഇരട്ട* A, size_t astep, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    തിരികെ hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> സ്ഥിതം വരിയിൽ _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    എങ്കിൽ( a > b )
    {
        b /= a;
        തിരികെ a*std::sqrt(1 + b*b);
    }
    എങ്കിൽ( b > 0 )
    {
        a /= b;
        തിരികെ b*std::sqrt(1 + a*a);
    }
    തിരികെ 0;
}


template<typename _Tp> തർക്കം
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, പൂർണ്ണസംഖ്യ n, uchar* buf )
{
    സ്ഥിരം _Tp eps = std::numeric_limits<_Tp>::epsilon();
    പൂർണ്ണസംഖ്യ i, j, k, m;

    astep /= വലുപ്പം(A[0]);
    എങ്കിൽ( V )
    {
        vstep /= വലുപ്പം(V[0]);
        വേണ്ടി( i = 0; i < n; i++ )
        {
            വേണ്ടി( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    പൂർണ്ണസംഖ്യ iters, maxIters = n*n*30;

    പൂർണ്ണസംഖ്യ* indR = (പൂർണ്ണസംഖ്യ*)alignPtr(buf, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    പൂർണ്ണസംഖ്യ* indC = indR + n;
    _Tp mv = (_Tp)0;

    വേണ്ടി( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        എങ്കിൽ( k < n - 1 )
        {
            വേണ്ടി( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                എങ്കിൽ( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        എങ്കിൽ( k > 0 )
        {
            വേണ്ടി( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                എങ്കിൽ( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    എങ്കിൽ( n > 1 ) വേണ്ടി( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        വേണ്ടി( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            എങ്കിൽ( mv < val )
                mv = val, k = i;
        }
        പൂർണ്ണസംഖ്യ l = indR[k];
        വേണ്ടി( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            എങ്കിൽ( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        എങ്കിൽ( std::abs(p) <= eps )
            നിർത്തുക;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        എങ്കിൽ( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        വേണ്ടി( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        വേണ്ടി( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        വേണ്ടി( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        എങ്കിൽ( V )
            വേണ്ടി( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        വേണ്ടി( j = 0; j < 2; j++ )
        {
            പൂർണ്ണസംഖ്യ idx = j == 0 ? k : l;
            എങ്കിൽ( idx < n - 1 )
            {
                വേണ്ടി( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    എങ്കിൽ( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            എങ്കിൽ( idx > 0 )
            {
                വേണ്ടി( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    എങ്കിൽ( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    വേണ്ടി( k = 0; k < n-1; k++ )
    {
        m = k;
        വേണ്ടി( i = k+1; i < n; i++ )
        {
            എങ്കിൽ( W[m] < W[i] )
                m = i;
        }
        എങ്കിൽ( k != m )
        {
            std::swap(W[m], W[k]);
            എങ്കിൽ( V )
                വേണ്ടി( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം Jacobi( ദശാംശം* S, size_t sstep, ദശാംശം* e, ദശാംശം* E, size_t estep, പൂർണ്ണസംഖ്യ n, uchar* buf )
{
    തിരികെ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

സ്ഥിതം തർക്കം Jacobi( ഇരട്ട* S, size_t sstep, ഇരട്ട* e, ഇരട്ട* E, size_t estep, പൂർണ്ണസംഖ്യ n, uchar* buf )
{
    തിരികെ JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> ഘടന VBLAS
{
    പൂർണ്ണസംഖ്യ dot(സ്ഥിരം T*, സ്ഥിരം T*, പൂർണ്ണസംഖ്യ, T*) സ്ഥിരം { തിരികെ 0; }
    പൂർണ്ണസംഖ്യ givens(T*, T*, പൂർണ്ണസംഖ്യ, T, T) സ്ഥിരം { തിരികെ 0; }
};

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
template<> വരിയിൽ പൂർണ്ണസംഖ്യ VBLAS<ദശാംശം>::dot(സ്ഥിരം ദശാംശം* a, സ്ഥിരം ദശാംശം* b, പൂർണ്ണസംഖ്യ n, ദശാംശം* result) സ്ഥിരം
{
    എങ്കിൽ( n < 2*VTraits<v_float32>::vlanes() )
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ k = 0;
    v_float32 s0 = vx_setzero_f32();
    വേണ്ടി( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    തിരികെ k;
}


template<> വരിയിൽ പൂർണ്ണസംഖ്യ VBLAS<ദശാംശം>::givens(ദശാംശം* a, ദശാംശം* b, പൂർണ്ണസംഖ്യ n, ദശാംശം c, ദശാംശം s) സ്ഥിരം
{
    എങ്കിൽ( n < VTraits<v_float32>::vlanes())
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    വേണ്ടി( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    തിരികെ k;
}


#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> വരിയിൽ പൂർണ്ണസംഖ്യ VBLAS<ഇരട്ട>::dot(സ്ഥിരം ഇരട്ട* a, സ്ഥിരം ഇരട്ട* b, പൂർണ്ണസംഖ്യ n, ഇരട്ട* result) സ്ഥിരം
{
    എങ്കിൽ( n < 2*VTraits<v_float64>::vlanes() )
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ k = 0;
    v_float64 s0 = vx_setzero_f64();
    വേണ്ടി( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    തിരികെ k;
}


template<> വരിയിൽ പൂർണ്ണസംഖ്യ VBLAS<ഇരട്ട>::givens(ഇരട്ട* a, ഇരട്ട* b, പൂർണ്ണസംഖ്യ n, ഇരട്ട c, ഇരട്ട s) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    വേണ്ടി( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    തിരികെ k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> ശൂന്യം
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ n1, ഇരട്ട minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<ഇരട്ട> Wbuf(n);
    ഇരട്ട* W = Wbuf.data();
    പൂർണ്ണസംഖ്യ i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    ഇരട്ട sd;
    astep /= വലുപ്പം(At[0]);
    vstep /= വലുപ്പം(Vt[0]);

    വേണ്ടി( i = 0; i < n; i++ )
    {
        വേണ്ടി( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ഇരട്ട)t*t;
        }
        W[i] = sd;

        എങ്കിൽ( Vt )
        {
            വേണ്ടി( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    വേണ്ടി( iter = 0; iter < max_iter; iter++ )
    {
        തർക്കം changed = അസത്യം;

        വേണ്ടി( i = 0; i < n-1; i++ )
            വേണ്ടി( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                ഇരട്ട a = W[i], p = 0, b = W[j];

                വേണ്ടി( k = 0; k < m; k++ )
                    p += (ഇരട്ട)Ai[k]*Aj[k];

                എങ്കിൽ( std::abs(p) <= eps*std::sqrt((ഇരട്ട)a*b) )
                    തുടരുക;

                p *= 2;
                ഇരട്ട beta = a - b, gamma = hypot((ഇരട്ട)p, beta);
                എങ്കിൽ( beta < 0 )
                {
                    ഇരട്ട delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                അല്ലെങ്കിൽ
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                വേണ്ടി( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (ഇരട്ട)t0*t0; b += (ഇരട്ട)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = സത്യം;

                എങ്കിൽ( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    വേണ്ടി( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        എങ്കിൽ( !changed )
            നിർത്തുക;
    }

    വേണ്ടി( i = 0; i < n; i++ )
    {
        വേണ്ടി( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (ഇരട്ട)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    വേണ്ടി( i = 0; i < n-1; i++ )
    {
        j = i;
        വേണ്ടി( k = i+1; k < n; k++ )
        {
            എങ്കിൽ( W[j] < W[k] )
                j = k;
        }
        എങ്കിൽ( i != j )
        {
            std::swap(W[i], W[j]);
            എങ്കിൽ( Vt )
            {
                വേണ്ടി( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                വേണ്ടി( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    വേണ്ടി( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    എങ്കിൽ( !Vt )
        തിരികെ;

    RNG rng(0x12345678);
    വേണ്ടി( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        വേണ്ടി( പൂർണ്ണസംഖ്യ ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            സ്ഥിരം _Tp val0 = (_Tp)(1./m);
            വേണ്ടി( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            വേണ്ടി( iter = 0; iter < 2; iter++ )
            {
                വേണ്ടി( j = 0; j < i; j++ )
                {
                    sd = 0;
                    വേണ്ടി( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    വേണ്ടി( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    വേണ്ടി( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            വേണ്ടി( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (ഇരട്ട)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        വേണ്ടി( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


സ്ഥിതം ശൂന്യം JacobiSVD(ദശാംശം* At, size_t astep, ദശാംശം* W, ദശാംശം* Vt, size_t vstep, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ n1=-1)
{
    hal::SVD32f(At, astep, W, ശൂന്യ, astep, Vt, vstep, m, n, n1);
}

സ്ഥിതം ശൂന്യം JacobiSVD(ഇരട്ട* At, size_t astep, ഇരട്ട* W, ഇരട്ട* Vt, size_t vstep, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ n1=-1)
{
    hal::SVD64f(At, astep, W, ശൂന്യ, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
decodeSVDParameters(സ്ഥിരം fptype* U, സ്ഥിരം fptype* Vt, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ n1)
{
    പൂർണ്ണസംഖ്യ halSVDFlag = 0;
    എങ്കിൽ(Vt == ശൂന്യ)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    അല്ലെങ്കിൽ എങ്കിൽ(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        എങ്കിൽ(U == ശൂന്യ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        എങ്കിൽ(U == ശൂന്യ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    തിരികെ halSVDFlag;
}

ശൂന്യം hal::SVD32f(ദശാംശം* At, size_t astep, ദശാംശം* W, ദശാംശം* U, size_t ustep, ദശാംശം* Vt, size_t vstep, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

ശൂന്യം hal::SVD64f(ഇരട്ട* At, size_t astep, ഇരട്ട* W, ഇരട്ട* U, size_t ustep, ഇരട്ട* Vt, size_t vstep, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> സ്ഥിതം ശൂന്യം
MatrAXPY( പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, സ്ഥിരം T1* x, പൂർണ്ണസംഖ്യ dx,
         സ്ഥിരം T2* a, പൂർണ്ണസംഖ്യ inca, T3* y, പൂർണ്ണസംഖ്യ dy )
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        പൂർണ്ണസംഖ്യ j = 0;
         #എങ്കിൽ CV_ENABLE_UNROLLED
        വേണ്ടി(; j <= n - 4; j += 4 )
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
        വേണ്ടി( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> സ്ഥിതം ശൂന്യം
SVBkSbImpl_( പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, സ്ഥിരം T* w, പൂർണ്ണസംഖ്യ incw,
       സ്ഥിരം T* u, പൂർണ്ണസംഖ്യ ldu, തർക്കം uT,
       സ്ഥിരം T* v, പൂർണ്ണസംഖ്യ ldv, തർക്കം vT,
       സ്ഥിരം T* b, പൂർണ്ണസംഖ്യ ldb, പൂർണ്ണസംഖ്യ nb,
       T* x, പൂർണ്ണസംഖ്യ ldx, ഇരട്ട* buffer, T eps )
{
    ഇരട്ട threshold = 0;
    പൂർണ്ണസംഖ്യ udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    പൂർണ്ണസംഖ്യ vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    പൂർണ്ണസംഖ്യ i, j, nm = std::min(m, n);

    എങ്കിൽ( !b )
        nb = m;

    വേണ്ടി( i = 0; i < n; i++ )
        വേണ്ടി( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    വേണ്ടി( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    വേണ്ടി( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        ഇരട്ട wi = w[i*incw];
        എങ്കിൽ( (ഇരട്ട)std::abs(wi) <= threshold )
            തുടരുക;
        wi = 1/wi;

        എങ്കിൽ( nb == 1 )
        {
            ഇരട്ട s = 0;
            എങ്കിൽ( b )
                വേണ്ടി( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            അല്ലെങ്കിൽ
                s = u[0];
            s *= wi;

            വേണ്ടി( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ( b )
            {
                വേണ്ടി( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                വേണ്ടി( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            അല്ലെങ്കിൽ
            {
                വേണ്ടി( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

സ്ഥിതം ശൂന്യം
SVBkSb( പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, സ്ഥിരം ദശാംശം* w, size_t wstep,
        സ്ഥിരം ദശാംശം* u, size_t ustep, തർക്കം uT,
        സ്ഥിരം ദശാംശം* v, size_t vstep, തർക്കം vT,
        സ്ഥിരം ദശാംശം* b, size_t bstep, പൂർണ്ണസംഖ്യ nb,
        ദശാംശം* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (പൂർണ്ണസംഖ്യ)(wstep/വലുപ്പം(w[0])) : 1,
                u, (പൂർണ്ണസംഖ്യ)(ustep/വലുപ്പം(u[0])), uT,
                v, (പൂർണ്ണസംഖ്യ)(vstep/വലുപ്പം(v[0])), vT,
                b, (പൂർണ്ണസംഖ്യ)(bstep/വലുപ്പം(b[0])), nb,
                x, (പൂർണ്ണസംഖ്യ)(xstep/വലുപ്പം(x[0])),
                (ഇരട്ട*)alignPtr(buffer, വലുപ്പം(ഇരട്ട)), (ദശാംശം)(DBL_EPSILON*2) );
}

സ്ഥിതം ശൂന്യം
SVBkSb( പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, സ്ഥിരം ഇരട്ട* w, size_t wstep,
       സ്ഥിരം ഇരട്ട* u, size_t ustep, തർക്കം uT,
       സ്ഥിരം ഇരട്ട* v, size_t vstep, തർക്കം vT,
       സ്ഥിരം ഇരട്ട* b, size_t bstep, പൂർണ്ണസംഖ്യ nb,
       ഇരട്ട* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (പൂർണ്ണസംഖ്യ)(wstep/വലുപ്പം(w[0])) : 1,
                u, (പൂർണ്ണസംഖ്യ)(ustep/വലുപ്പം(u[0])), uT,
                v, (പൂർണ്ണസംഖ്യ)(vstep/വലുപ്പം(v[0])), vT,
                b, (പൂർണ്ണസംഖ്യ)(bstep/വലുപ്പം(b[0])), nb,
                x, (പൂർണ്ണസംഖ്യ)(xstep/വലുപ്പം(x[0])),
                (ഇരട്ട*)alignPtr(buffer, വലുപ്പം(ഇരട്ട)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((ഇരട്ട)m(0,0)*m(1,1) - (ഇരട്ട)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((ഇരട്ട)m(1,1)*m(2,2) - (ഇരട്ട)m(1,2)*m(2,1)) -  \
                   m(0,1)*((ഇരട്ട)m(1,0)*m(2,2) - (ഇരട്ട)m(1,2)*m(2,0)) +  \
                   m(0,2)*((ഇരട്ട)m(1,0)*m(2,1) - (ഇരട്ട)m(1,1)*m(2,0)))

ഇരട്ട determinant( നിവേശം_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    ഇരട്ട result = 0;
    പൂർണ്ണസംഖ്യ type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    സ്ഥിരം uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((ദശാംശം*)(m + y*step))[x]
    #define Md(y, x) ((ഇരട്ട*)(m + y*step))[x]

    എങ്കിൽ( type == CV_32F )
    {
        എങ്കിൽ( rows == 2 )
            result = det2(Mf);
        അല്ലെങ്കിൽ എങ്കിൽ( rows == 3 )
            result = det3(Mf);
        അല്ലെങ്കിൽ എങ്കിൽ( rows == 1 )
            result = Mf(0,0);
        അല്ലെങ്കിൽ
        {
            size_t bufSize = rows*rows*വലുപ്പം(ദശാംശം);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<ദശാംശം>(), a.step, rows, 0, 0, 0);
            എങ്കിൽ( result )
            {
                വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < rows; i++ )
                    result *= a.at<ദശാംശം>(i,i);
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ( rows == 2 )
            result = det2(Md);
        അല്ലെങ്കിൽ എങ്കിൽ( rows == 3 )
            result = det3(Md);
        അല്ലെങ്കിൽ എങ്കിൽ( rows == 1 )
            result = Md(0,0);
        അല്ലെങ്കിൽ
        {
            size_t bufSize = rows*rows*വലുപ്പം(ഇരട്ട);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<ഇരട്ട>(), a.step, rows, 0, 0, 0);
            എങ്കിൽ( result )
            {
                വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < rows; i++ )
                    result *= a.at<ഇരട്ട>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    തിരികെ result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((ദശാംശം*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((ഇരട്ട*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((ദശാംശം*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((ഇരട്ട*)(dstdata + y*dststep))[x]

ഇരട്ട invert( നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ method )
{
    CV_INSTRUMENT_REGION();

    തർക്കം result = അസത്യം;
    Mat src = _src.getMat();
    പൂർണ്ണസംഖ്യ type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    പൂർണ്ണസംഖ്യ m = src.rows, n = src.cols;

    എങ്കിൽ( method == DECOMP_SVD )
    {
        പൂർണ്ണസംഖ്യ nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + വലുപ്പം(ഇരട്ട));
        uchar* buf = alignPtr((uchar*)_buf.data(), (പൂർണ്ണസംഖ്യ)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        തിരികെ type == CV_32F ?
            (w.ptr<ദശാംശം>()[0] >= FLT_EPSILON ?
             w.ptr<ദശാംശം>()[n-1]/w.ptr<ദശാംശം>()[0] : 0) :
            (w.ptr<ഇരട്ട>()[0] >= DBL_EPSILON ?
             w.ptr<ഇരട്ട>()[n-1]/w.ptr<ഇരട്ട>()[0] : 0);
    }

    CV_Assert( m == n );

    എങ്കിൽ( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + വലുപ്പം(ഇരട്ട));
        uchar* buf = alignPtr((uchar*)_buf.data(), (പൂർണ്ണസംഖ്യ)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        തിരികെ type == CV_32F ?
        (w.ptr<ദശാംശം>()[0] >= FLT_EPSILON ?
         w.ptr<ദശാംശം>()[n-1]/w.ptr<ദശാംശം>()[0] : 0) :
        (w.ptr<ഇരട്ട>()[0] >= DBL_EPSILON ?
         w.ptr<ഇരട്ട>()[n-1]/w.ptr<ഇരട്ട>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    എങ്കിൽ( n <= 3 )
    {
        സ്ഥിരം uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        എങ്കിൽ( n == 2 )
        {
            എങ്കിൽ( type == CV_32FC1 )
            {
                ഇരട്ട d = det2(Sf);
                എങ്കിൽ( d != 0. )
                {
                    result = സത്യം;
                    d = 1./d;
                #എങ്കിൽ CV_SIMD128
                    സ്ഥിരം ദശാംശം d_32f = (ദശാംശം)d;
                    സ്ഥിരം v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((സ്ഥിരം ദശാംശം *)srcdata, (സ്ഥിരം ദശാംശം *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_താഴെയുള്ള(v_rotate_വലത്<1>(s0), s0));
                    v_store_താഴെയുള്ള((ദശാംശം*)dstdata, s0);
                    v_store_മുകളിലെ((ദശാംശം*)(dstdata + dststep), s0);
                #അല്ലെങ്കിൽ
                    ഇരട്ട t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (ദശാംശം)t0;
                    Df(0,0) = (ദശാംശം)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (ദശാംശം)t0;
                    Df(1,0) = (ദശാംശം)t1;
                #endif
                }
            }
            അല്ലെങ്കിൽ
            {
                ഇരട്ട d = det2(Sd);
                എങ്കിൽ( d != 0. )
                {
                    result = സത്യം;
                    d = 1./d;
                #എങ്കിൽ CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((സ്ഥിരം ഇരട്ട *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((സ്ഥിരം ഇരട്ട *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<ഇരട്ട>(0), v_extract<1>(s0, s1));//12
                    v_store((ഇരട്ട*)dstdata, v_combine_താഴെയുള്ള(sm, ss));//31
                    v_store((ഇരട്ട*)(dstdata + dststep), v_combine_മുകളിലെ(ss, sm));//20
                #അല്ലെങ്കിൽ
                    ഇരട്ട t0, t1;
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
        അല്ലെങ്കിൽ എങ്കിൽ( n == 3 )
        {
            എങ്കിൽ( type == CV_32FC1 )
            {
                ഇരട്ട d = det3(Sf);

                എങ്കിൽ( d != 0. )
                {
                    ഇരട്ട t[12];

                    result = സത്യം;
                    d = 1./d;
                    t[0] = (((ഇരട്ട)Sf(1,1) * Sf(2,2) - (ഇരട്ട)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((ഇരട്ട)Sf(0,2) * Sf(2,1) - (ഇരട്ട)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((ഇരട്ട)Sf(0,1) * Sf(1,2) - (ഇരട്ട)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((ഇരട്ട)Sf(1,2) * Sf(2,0) - (ഇരട്ട)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((ഇരട്ട)Sf(0,0) * Sf(2,2) - (ഇരട്ട)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((ഇരട്ട)Sf(0,2) * Sf(1,0) - (ഇരട്ട)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((ഇരട്ട)Sf(1,0) * Sf(2,1) - (ഇരട്ട)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((ഇരട്ട)Sf(0,1) * Sf(2,0) - (ഇരട്ട)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((ഇരട്ട)Sf(0,0) * Sf(1,1) - (ഇരട്ട)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (ദശാംശം)t[0]; Df(0,1) = (ദശാംശം)t[1]; Df(0,2) = (ദശാംശം)t[2];
                    Df(1,0) = (ദശാംശം)t[3]; Df(1,1) = (ദശാംശം)t[4]; Df(1,2) = (ദശാംശം)t[5];
                    Df(2,0) = (ദശാംശം)t[6]; Df(2,1) = (ദശാംശം)t[7]; Df(2,2) = (ദശാംശം)t[8];
                }
            }
            അല്ലെങ്കിൽ
            {
                ഇരട്ട d = det3(Sd);
                എങ്കിൽ( d != 0. )
                {
                    result = സത്യം;
                    d = 1./d;
                    ഇരട്ട t[9];

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
        അല്ലെങ്കിൽ
        {
            CV_Assert( n == 1 );

            എങ്കിൽ( type == CV_32FC1 )
            {
                ഇരട്ട d = Sf(0,0);
                എങ്കിൽ( d != 0. )
                {
                    result = സത്യം;
                    Df(0,0) = (ദശാംശം)(1./d);
                }
            }
            അല്ലെങ്കിൽ
            {
                ഇരട്ട d = Sd(0,0);
                എങ്കിൽ( d != 0. )
                {
                    result = സത്യം;
                    Dd(0,0) = 1./d;
                }
            }
        }
        എങ്കിൽ( !result )
            dst = Scalar(0);
        തിരികെ result;
    }

   പൂർണ്ണസംഖ്യ elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    എങ്കിൽ( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<ദശാംശം>(), src1.step, n, dst.ptr<ദശാംശം>(), dst.step, n) != 0;
    അല്ലെങ്കിൽ എങ്കിൽ( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<ഇരട്ട>(), src1.step, n, dst.ptr<ഇരട്ട>(), dst.step, n) != 0;
    അല്ലെങ്കിൽ എങ്കിൽ( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<ദശാംശം>(), src1.step, n, dst.ptr<ദശാംശം>(), dst.step, n);
    അല്ലെങ്കിൽ
        result = hal::Cholesky64f(src1.ptr<ഇരട്ട>(), src1.step, n, dst.ptr<ഇരട്ട>(), dst.step, n);

    എങ്കിൽ( !result )
        dst = Scalar(0);

    തിരികെ result;
}

UMat UMat::inv(പൂർണ്ണസംഖ്യ method) സ്ഥിരം
{
    UMat m;
    invert(*ഇത്, m, method);
    തിരികെ m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

തർക്കം solve( നിവേശം_Array _src, നിവേശം_Array _src2arg, OutputArray _dst, പൂർണ്ണസംഖ്യ method )
{
    CV_INSTRUMENT_REGION();

    തർക്കം result = സത്യം;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    പൂർണ്ണസംഖ്യ type = src.type();
    തർക്കം is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    എങ്കിൽ( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((ദശാംശം*)(bdata + y*src2step))[0]
        #define bd(y) ((ഇരട്ട*)(bdata + y*src2step))[0]

        സ്ഥിരം uchar* srcdata = src.ptr();
        സ്ഥിരം uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        എങ്കിൽ( src.rows == 2 )
        {
            എങ്കിൽ( type == CV_32FC1 )
            {
                ഇരട്ട d = det2(Sf);
                എങ്കിൽ( d != 0. )
                {
                    ഇരട്ട t;
                    d = 1./d;
                    t = (ദശാംശം)(((ഇരട്ട)bf(0)*Sf(1,1) - (ഇരട്ട)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (ദശാംശം)(((ഇരട്ട)bf(1)*Sf(0,0) - (ഇരട്ട)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (ദശാംശം)t;
                }
                അല്ലെങ്കിൽ
                    result = അസത്യം;
            }
            അല്ലെങ്കിൽ
            {
                ഇരട്ട d = det2(Sd);
                എങ്കിൽ( d != 0. )
                {
                    ഇരട്ട t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                അല്ലെങ്കിൽ
                    result = അസത്യം;
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ( src.rows == 3 )
        {
            എങ്കിൽ( type == CV_32FC1 )
            {
                ഇരട്ട d = det3(Sf);
                എങ്കിൽ( d != 0. )
                {
                    ദശാംശം t[3];
                    d = 1./d;

                    t[0] = (ദശാംശം)(d*
                           (bf(0)*((ഇരട്ട)Sf(1,1)*Sf(2,2) - (ഇരട്ട)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((ഇരട്ട)bf(1)*Sf(2,2) - (ഇരട്ട)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((ഇരട്ട)bf(1)*Sf(2,1) - (ഇരട്ട)Sf(1,1)*bf(2))));

                    t[1] = (ദശാംശം)(d*
                           (Sf(0,0)*(ഇരട്ട)(bf(1)*Sf(2,2) - (ഇരട്ട)Sf(1,2)*bf(2)) -
                            bf(0)*((ഇരട്ട)Sf(1,0)*Sf(2,2) - (ഇരട്ട)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((ഇരട്ട)Sf(1,0)*bf(2) - (ഇരട്ട)bf(1)*Sf(2,0))));

                    t[2] = (ദശാംശം)(d*
                           (Sf(0,0)*((ഇരട്ട)Sf(1,1)*bf(2) - (ഇരട്ട)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((ഇരട്ട)Sf(1,0)*bf(2) - (ഇരട്ട)bf(1)*Sf(2,0)) +
                            bf(0)*((ഇരട്ട)Sf(1,0)*Sf(2,1) - (ഇരട്ട)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                അല്ലെങ്കിൽ
                    result = അസത്യം;
            }
            അല്ലെങ്കിൽ
            {
                ഇരട്ട d = det3(Sd);
                എങ്കിൽ( d != 0. )
                {
                    ഇരട്ട t[9];

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
                അല്ലെങ്കിൽ
                    result = അസത്യം;
            }
        }
        അല്ലെങ്കിൽ
        {
            CV_Assert( src.rows == 1 );

            എങ്കിൽ( type == CV_32FC1 )
            {
                ഇരട്ട d = Sf(0,0);
                എങ്കിൽ( d != 0. )
                    Df(0,0) = (ദശാംശം)(bf(0)/d);
                അല്ലെങ്കിൽ
                    result = അസത്യം;
            }
            അല്ലെങ്കിൽ
            {
                ഇരട്ട d = Sd(0,0);
                എങ്കിൽ( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                അല്ലെങ്കിൽ
                    result = അസത്യം;
            }
        }
        തിരികെ result;
    }

    പൂർണ്ണസംഖ്യ m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    എങ്കിൽ( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    എങ്കിൽ( m == n )
        is_normal = അസത്യം;
    അല്ലെങ്കിൽ എങ്കിൽ( is_normal )
    {
        m_ = n;
        എങ്കിൽ( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    എങ്കിൽ( is_normal )
        bufsize += n*nb*esz;
    എങ്കിൽ( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*വലുപ്പം(ഇരട്ട) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    എങ്കിൽ( is_normal )
        mulTransposed(src, a, സത്യം);
    അല്ലെങ്കിൽ എങ്കിൽ( method != DECOMP_SVD )
        src.copyTo(a);
    അല്ലെങ്കിൽ
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    എങ്കിൽ( !is_normal )
    {
        എങ്കിൽ( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    അല്ലെങ്കിൽ
    {
        // a'*b
        എങ്കിൽ( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        അല്ലെങ്കിൽ
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    എങ്കിൽ( method == DECOMP_LU )
    {
        എങ്കിൽ( type == CV_32F )
            result = hal::LU32f(a.ptr<ദശാംശം>(), a.step, n, dst.ptr<ദശാംശം>(), dst.step, nb) != 0;
        അല്ലെങ്കിൽ
            result = hal::LU64f(a.ptr<ഇരട്ട>(), a.step, n, dst.ptr<ഇരട്ട>(), dst.step, nb) != 0;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( method == DECOMP_CHOLESKY )
    {
        എങ്കിൽ( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<ദശാംശം>(), a.step, n, dst.ptr<ദശാംശം>(), dst.step, nb);
        അല്ലെങ്കിൽ
            result = hal::Cholesky64f(a.ptr<ഇരട്ട>(), a.step, n, dst.ptr<ഇരട്ട>(), dst.step, nb);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( method == DECOMP_QR )
    {
        Mat rhsMat;
        എങ്കിൽ( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        അല്ലെങ്കിൽ
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        എങ്കിൽ( type == CV_32F )
            result = hal::QR32f(a.ptr<ദശാംശം>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<ദശാംശം>(), rhsMat.step, ശൂന്യ) != 0;
        അല്ലെങ്കിൽ
            result = hal::QR64f(a.ptr<ഇരട്ട>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<ഇരട്ട>(), rhsMat.step, ശൂന്യ) != 0;

        എങ്കിൽ (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    അല്ലെങ്കിൽ
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        എങ്കിൽ( method == DECOMP_EIG )
        {
            എങ്കിൽ( type == CV_32F )
                Jacobi(a.ptr<ദശാംശം>(), a.step, w.ptr<ദശാംശം>(), v.ptr<ദശാംശം>(), v.step, n, ptr);
            അല്ലെങ്കിൽ
                Jacobi(a.ptr<ഇരട്ട>(), a.step, w.ptr<ഇരട്ട>(), v.ptr<ഇരട്ട>(), v.step, n, ptr);
            u = v;
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ( type == CV_32F )
                JacobiSVD(a.ptr<ദശാംശം>(), a.step, w.ptr<ദശാംശം>(), v.ptr<ദശാംശം>(), v.step, m_, n);
            അല്ലെങ്കിൽ
                JacobiSVD(a.ptr<ഇരട്ട>(), a.step, w.ptr<ഇരട്ട>(), v.ptr<ഇരട്ട>(), v.step, m_, n);
            u = a;
        }

        എങ്കിൽ( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<ദശാംശം>(), 0, u.ptr<ദശാംശം>(), u.step, സത്യം,
                   v.ptr<ദശാംശം>(), v.step, സത്യം, src2.ptr<ദശാംശം>(),
                   src2.step, nb, dst.ptr<ദശാംശം>(), dst.step, ptr);
        }
        അല്ലെങ്കിൽ
        {
            SVBkSb(m_, n, w.ptr<ഇരട്ട>(), 0, u.ptr<ഇരട്ട>(), u.step, സത്യം,
                   v.ptr<ഇരട്ട>(), v.step, സത്യം, src2.ptr<ഇരട്ട>(),
                   src2.step, nb, dst.ptr<ഇരട്ട>(), dst.step, ptr);
        }
        result = സത്യം;
    }

    എങ്കിൽ( !result )
        dst = Scalar(0);

    തിരികെ result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

തർക്കം eigen( നിവേശം_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    പൂർണ്ണസംഖ്യ type = src.type();
    പൂർണ്ണസംഖ്യ n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    എങ്കിൽ( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    സ്ഥിരം തർക്കം evecNeeded = _evects.needed();
    സ്ഥിരം പൂർണ്ണസംഖ്യ esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    എങ്കിൽ ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        എങ്കിൽ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            എങ്കിൽ ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            തിരികെ സത്യം;
        }
    } അല്ലെങ്കിൽ { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        എങ്കിൽ ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            എങ്കിൽ ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            തിരികെ സത്യം;
        }
    }
    തിരികെ അസത്യം;
#അല്ലെങ്കിൽ

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    തർക്കം ok = type == CV_32F ?
        Jacobi(a.ptr<ദശാംശം>(), a.step, w.ptr<ദശാംശം>(), v.ptr<ദശാംശം>(), v.step, n, ptr) :
        Jacobi(a.ptr<ഇരട്ട>(), a.step, w.ptr<ഇരട്ട>(), v.ptr<ഇരട്ട>(), v.step, n, ptr);

    w.copyTo(_evals);
    തിരികെ ok;
#endif
}

സ്ഥിതം ശൂന്യം _SVDcompute( നിവേശം_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, പൂർണ്ണസംഖ്യ flags )
{
    Mat src = _aarr.getMat();
    പൂർണ്ണസംഖ്യ m = src.rows, n = src.cols;
    പൂർണ്ണസംഖ്യ type = src.type();
    തർക്കം compute_uv = _u.needed() || _vt.needed();
    തർക്കം full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    എങ്കിൽ( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = അസത്യം;
    }

    തർക്കം at = അസത്യം;
    എങ്കിൽ( m < n )
    {
        std::swap(m, n);
        at = സത്യം;
    }

    പൂർണ്ണസംഖ്യ urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    എങ്കിൽ( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    എങ്കിൽ( urows > n )
        temp_u = Scalar::all(0);

    എങ്കിൽ( !at )
        transpose(src, temp_a);
    അല്ലെങ്കിൽ
        src.copyTo(temp_a);

    എങ്കിൽ( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<ദശാംശം>(), temp_u.step, temp_w.ptr<ദശാംശം>(),
              temp_v.ptr<ദശാംശം>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    അല്ലെങ്കിൽ
    {
        JacobiSVD(temp_a.ptr<ഇരട്ട>(), temp_u.step, temp_w.ptr<ഇരട്ട>(),
              temp_v.ptr<ഇരട്ട>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    എങ്കിൽ( compute_uv )
    {
        എങ്കിൽ( !at )
        {
            എങ്കിൽ( _u.needed() )
                transpose(temp_u, _u);
            എങ്കിൽ( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ( _u.needed() )
                transpose(temp_v, _u);
            എങ്കിൽ( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


ശൂന്യം SVD::compute( നിവേശം_Array a, OutputArray w, OutputArray u, OutputArray vt, പൂർണ്ണസംഖ്യ flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

ശൂന്യം SVD::compute( നിവേശം_Array a, OutputArray w, പൂർണ്ണസംഖ്യ flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

ശൂന്യം SVD::backSubst( നിവേശം_Array _w, നിവേശം_Array _u, നിവേശം_Array _vt,
                     നിവേശം_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    പൂർണ്ണസംഖ്യ type = w.type(), esz = (പൂർണ്ണസംഖ്യ)w.elemSize();
    പൂർണ്ണസംഖ്യ m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*വലുപ്പം(ഇരട്ട) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    എങ്കിൽ( type == CV_32F )
        SVBkSb(m, n, w.ptr<ദശാംശം>(), wstep, u.ptr<ദശാംശം>(), u.step, അസത്യം,
               vt.ptr<ദശാംശം>(), vt.step, സത്യം, rhs.ptr<ദശാംശം>(), rhs.step, nb,
               dst.ptr<ദശാംശം>(), dst.step, buffer.data());
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_64F )
        SVBkSb(m, n, w.ptr<ഇരട്ട>(), wstep, u.ptr<ഇരട്ട>(), u.step, അസത്യം,
               vt.ptr<ഇരട്ട>(), vt.step, സത്യം, rhs.ptr<ഇരട്ട>(), rhs.step, nb,
               dst.ptr<ഇരട്ട>(), dst.step, buffer.data());
    അല്ലെങ്കിൽ
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(നിവേശം_Array a, പൂർണ്ണസംഖ്യ flags)
{
    _SVDcompute(a, w, u, vt, flags);
    തിരികെ *ഇത്;
}


ശൂന്യം SVD::backSubst( നിവേശം_Array rhs, OutputArray dst ) സ്ഥിരം
{
    backSubst( w, u, vt, rhs, dst );
}

}


ശൂന്യം cv::SVDecomp(നിവേശം_Array src, OutputArray w, OutputArray u, OutputArray vt, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

ശൂന്യം cv::SVBackSubst(നിവേശം_Array w, നിവേശം_Array u, നിവേശം_Array vt, നിവേശം_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
