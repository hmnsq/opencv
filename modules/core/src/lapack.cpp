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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <limits>

#ifdef HAVE_EIGEN
#  যদি defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  যদি defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

নামাকাশ cv
{

পূর্ণসংখ্যা LU(দশমিক* A, size_t astep, পূর্ণসংখ্যা m, দশমিক* b, size_t bstep, পূর্ণসংখ্যা n)
{
    CV_INSTRUMENT_REGION();

    ফেরত hal::LU32f(A, astep, m, b, bstep, n);
}

পূর্ণসংখ্যা LU(দ্বিগুণ* A, size_t astep, পূর্ণসংখ্যা m, দ্বিগুণ* b, size_t bstep, পূর্ণসংখ্যা n)
{
    CV_INSTRUMENT_REGION();

    ফেরত hal::LU64f(A, astep, m, b, bstep, n);
}

যৌক্তিক Cholesky(দশমিক* A, size_t astep, পূর্ণসংখ্যা m, দশমিক* b, size_t bstep, পূর্ণসংখ্যা n)
{
    CV_INSTRUMENT_REGION();

    ফেরত hal::Cholesky32f(A, astep, m, b, bstep, n);
}

যৌক্তিক Cholesky(দ্বিগুণ* A, size_t astep, পূর্ণসংখ্যা m, দ্বিগুণ* b, size_t bstep, পূর্ণসংখ্যা n)
{
    CV_INSTRUMENT_REGION();

    ফেরত hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> স্থির অন্তঃরেখ _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    যদি( a > b )
    {
        b /= a;
        ফেরত a*std::sqrt(1 + b*b);
    }
    যদি( b > 0 )
    {
        a /= b;
        ফেরত b*std::sqrt(1 + a*a);
    }
    ফেরত 0;
}


template<typename _Tp> যৌক্তিক
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, পূর্ণসংখ্যা n, uchar* buf )
{
    ধ্রুবক _Tp eps = std::numeric_limits<_Tp>::epsilon();
    পূর্ণসংখ্যা i, j, k, m;

    astep /= আকার(A[0]);
    যদি( V )
    {
        vstep /= আকার(V[0]);
        জন্য( i = 0; i < n; i++ )
        {
            জন্য( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    পূর্ণসংখ্যা iters, maxIters = n*n*30;

    পূর্ণসংখ্যা* indR = (পূর্ণসংখ্যা*)alignPtr(buf, আকার(পূর্ণসংখ্যা));
    পূর্ণসংখ্যা* indC = indR + n;
    _Tp mv = (_Tp)0;

    জন্য( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        যদি( k < n - 1 )
        {
            জন্য( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                যদি( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        যদি( k > 0 )
        {
            জন্য( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                যদি( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    যদি( n > 1 ) জন্য( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        জন্য( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            যদি( mv < val )
                mv = val, k = i;
        }
        পূর্ণসংখ্যা l = indR[k];
        জন্য( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            যদি( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        যদি( std::abs(p) <= eps )
            বিরতি;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        যদি( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        জন্য( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        জন্য( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        জন্য( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        যদি( V )
            জন্য( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        জন্য( j = 0; j < 2; j++ )
        {
            পূর্ণসংখ্যা idx = j == 0 ? k : l;
            যদি( idx < n - 1 )
            {
                জন্য( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    যদি( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            যদি( idx > 0 )
            {
                জন্য( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    যদি( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    জন্য( k = 0; k < n-1; k++ )
    {
        m = k;
        জন্য( i = k+1; i < n; i++ )
        {
            যদি( W[m] < W[i] )
                m = i;
        }
        যদি( k != m )
        {
            std::swap(W[m], W[k]);
            যদি( V )
                জন্য( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    ফেরত সত্য;
}

স্থির যৌক্তিক Jacobi( দশমিক* S, size_t sstep, দশমিক* e, দশমিক* E, size_t estep, পূর্ণসংখ্যা n, uchar* buf )
{
    ফেরত JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

স্থির যৌক্তিক Jacobi( দ্বিগুণ* S, size_t sstep, দ্বিগুণ* e, দ্বিগুণ* E, size_t estep, পূর্ণসংখ্যা n, uchar* buf )
{
    ফেরত JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> কাঠামো VBLAS
{
    পূর্ণসংখ্যা dot(ধ্রুবক T*, ধ্রুবক T*, পূর্ণসংখ্যা, T*) ধ্রুবক { ফেরত 0; }
    পূর্ণসংখ্যা givens(T*, T*, পূর্ণসংখ্যা, T, T) ধ্রুবক { ফেরত 0; }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<> অন্তঃরেখ পূর্ণসংখ্যা VBLAS<দশমিক>::dot(ধ্রুবক দশমিক* a, ধ্রুবক দশমিক* b, পূর্ণসংখ্যা n, দশমিক* ফলাফল) ধ্রুবক
{
    যদি( n < 2*VTraits<v_float32>::vlanes() )
        ফেরত 0;
    পূর্ণসংখ্যা k = 0;
    v_float32 s0 = vx_setzero_f32();
    জন্য( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *ফলাফল = v_reduce_যোগ(s0);
    vx_cleanup();
    ফেরত k;
}


template<> অন্তঃরেখ পূর্ণসংখ্যা VBLAS<দশমিক>::givens(দশমিক* a, দশমিক* b, পূর্ণসংখ্যা n, দশমিক c, দশমিক s) ধ্রুবক
{
    যদি( n < VTraits<v_float32>::vlanes())
        ফেরত 0;
    পূর্ণসংখ্যা k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    জন্য( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ফেরত k;
}


#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> অন্তঃরেখ পূর্ণসংখ্যা VBLAS<দ্বিগুণ>::dot(ধ্রুবক দ্বিগুণ* a, ধ্রুবক দ্বিগুণ* b, পূর্ণসংখ্যা n, দ্বিগুণ* ফলাফল) ধ্রুবক
{
    যদি( n < 2*VTraits<v_float64>::vlanes() )
        ফেরত 0;
    পূর্ণসংখ্যা k = 0;
    v_float64 s0 = vx_setzero_f64();
    জন্য( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *ফলাফল = v_reduce_যোগ(s0);
    vx_cleanup();
    ফেরত k;
}


template<> অন্তঃরেখ পূর্ণসংখ্যা VBLAS<দ্বিগুণ>::givens(দ্বিগুণ* a, দ্বিগুণ* b, পূর্ণসংখ্যা n, দ্বিগুণ c, দ্বিগুণ s) ধ্রুবক
{
    পূর্ণসংখ্যা k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    জন্য( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ফেরত k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> শূন্য
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা n1, দ্বিগুণ minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    Auto_বাফার<দ্বিগুণ> Wbuf(n);
    দ্বিগুণ* W = Wbuf.তথ্য();
    পূর্ণসংখ্যা i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    দ্বিগুণ sd;
    astep /= আকার(At[0]);
    vstep /= আকার(Vt[0]);

    জন্য( i = 0; i < n; i++ )
    {
        জন্য( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (দ্বিগুণ)t*t;
        }
        W[i] = sd;

        যদি( Vt )
        {
            জন্য( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    জন্য( iter = 0; iter < max_iter; iter++ )
    {
        যৌক্তিক changed = মিথ্যা;

        জন্য( i = 0; i < n-1; i++ )
            জন্য( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                দ্বিগুণ a = W[i], p = 0, b = W[j];

                জন্য( k = 0; k < m; k++ )
                    p += (দ্বিগুণ)Ai[k]*Aj[k];

                যদি( std::abs(p) <= eps*std::sqrt((দ্বিগুণ)a*b) )
                    চালু;

                p *= 2;
                দ্বিগুণ beta = a - b, gamma = hypot((দ্বিগুণ)p, beta);
                যদি( beta < 0 )
                {
                    দ্বিগুণ delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                অন্যথা
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                জন্য( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (দ্বিগুণ)t0*t0; b += (দ্বিগুণ)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = সত্য;

                যদি( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    জন্য( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        যদি( !changed )
            বিরতি;
    }

    জন্য( i = 0; i < n; i++ )
    {
        জন্য( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (দ্বিগুণ)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    জন্য( i = 0; i < n-1; i++ )
    {
        j = i;
        জন্য( k = i+1; k < n; k++ )
        {
            যদি( W[j] < W[k] )
                j = k;
        }
        যদি( i != j )
        {
            std::swap(W[i], W[j]);
            যদি( Vt )
            {
                জন্য( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                জন্য( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    জন্য( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    যদি( !Vt )
        ফেরত;

    RNG rng(0x12345678);
    জন্য( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        জন্য( পূর্ণসংখ্যা ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ধ্রুবক _Tp val0 = (_Tp)(1./m);
            জন্য( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            জন্য( iter = 0; iter < 2; iter++ )
            {
                জন্য( j = 0; j < i; j++ )
                {
                    sd = 0;
                    জন্য( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    জন্য( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    জন্য( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            জন্য( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (দ্বিগুণ)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        জন্য( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


স্থির শূন্য JacobiSVD(দশমিক* At, size_t astep, দশমিক* W, দশমিক* Vt, size_t vstep, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা n1=-1)
{
    hal::SVD32f(At, astep, W, ফাঁকা, astep, Vt, vstep, m, n, n1);
}

স্থির শূন্য JacobiSVD(দ্বিগুণ* At, size_t astep, দ্বিগুণ* W, দ্বিগুণ* Vt, size_t vstep, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা n1=-1)
{
    hal::SVD64f(At, astep, W, ফাঁকা, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> স্থির অন্তঃরেখ পূর্ণসংখ্যা
decodeSVDParameters(ধ্রুবক fptype* U, ধ্রুবক fptype* Vt, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা n1)
{
    পূর্ণসংখ্যা halSVDFlag = 0;
    যদি(Vt == ফাঁকা)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    অন্যথা যদি(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        যদি(U == ফাঁকা)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    অন্যথা যদি(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        যদি(U == ফাঁকা)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ফেরত halSVDFlag;
}

শূন্য hal::SVD32f(দশমিক* At, size_t astep, দশমিক* W, দশমিক* U, size_t ustep, দশমিক* Vt, size_t vstep, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

শূন্য hal::SVD64f(দ্বিগুণ* At, size_t astep, দ্বিগুণ* W, দ্বিগুণ* U, size_t ustep, দ্বিগুণ* Vt, size_t vstep, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> স্থির শূন্য
MatrAXPY( পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, ধ্রুবক T1* x, পূর্ণসংখ্যা dx,
         ধ্রুবক T2* a, পূর্ণসংখ্যা inca, T3* y, পূর্ণসংখ্যা dy )
{
    পূর্ণসংখ্যা i;
    জন্য( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        পূর্ণসংখ্যা j = 0;
         #যদি CV_ENABLE_UNROLLED
        জন্য(; j <= n - 4; j += 4 )
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
        জন্য( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> স্থির শূন্য
SVBkSbImpl_( পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, ধ্রুবক T* w, পূর্ণসংখ্যা incw,
       ধ্রুবক T* u, পূর্ণসংখ্যা ldu, যৌক্তিক uT,
       ধ্রুবক T* v, পূর্ণসংখ্যা ldv, যৌক্তিক vT,
       ধ্রুবক T* b, পূর্ণসংখ্যা ldb, পূর্ণসংখ্যা nb,
       T* x, পূর্ণসংখ্যা ldx, দ্বিগুণ* বাফার, T eps )
{
    দ্বিগুণ threshold = 0;
    পূর্ণসংখ্যা udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    পূর্ণসংখ্যা vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    পূর্ণসংখ্যা i, j, nm = std::min(m, n);

    যদি( !b )
        nb = m;

    জন্য( i = 0; i < n; i++ )
        জন্য( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    জন্য( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    জন্য( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        দ্বিগুণ wi = w[i*incw];
        যদি( (দ্বিগুণ)std::abs(wi) <= threshold )
            চালু;
        wi = 1/wi;

        যদি( nb == 1 )
        {
            দ্বিগুণ s = 0;
            যদি( b )
                জন্য( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            অন্যথা
                s = u[0];
            s *= wi;

            জন্য( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        অন্যথা
        {
            যদি( b )
            {
                জন্য( j = 0; j < nb; j++ )
                    বাফার[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, বাফার, 0 );
                জন্য( j = 0; j < nb; j++ )
                    বাফার[j] *= wi;
            }
            অন্যথা
            {
                জন্য( j = 0; j < nb; j++ )
                    বাফার[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, বাফার, 0, v, vdelta1, x, ldx );
        }
    }
}

স্থির শূন্য
SVBkSb( পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, ধ্রুবক দশমিক* w, size_t wstep,
        ধ্রুবক দশমিক* u, size_t ustep, যৌক্তিক uT,
        ধ্রুবক দশমিক* v, size_t vstep, যৌক্তিক vT,
        ধ্রুবক দশমিক* b, size_t bstep, পূর্ণসংখ্যা nb,
        দশমিক* x, size_t xstep, uchar* বাফার )
{
    SVBkSbImpl_(m, n, w, wstep ? (পূর্ণসংখ্যা)(wstep/আকার(w[0])) : 1,
                u, (পূর্ণসংখ্যা)(ustep/আকার(u[0])), uT,
                v, (পূর্ণসংখ্যা)(vstep/আকার(v[0])), vT,
                b, (পূর্ণসংখ্যা)(bstep/আকার(b[0])), nb,
                x, (পূর্ণসংখ্যা)(xstep/আকার(x[0])),
                (দ্বিগুণ*)alignPtr(বাফার, আকার(দ্বিগুণ)), (দশমিক)(DBL_EPSILON*2) );
}

স্থির শূন্য
SVBkSb( পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, ধ্রুবক দ্বিগুণ* w, size_t wstep,
       ধ্রুবক দ্বিগুণ* u, size_t ustep, যৌক্তিক uT,
       ধ্রুবক দ্বিগুণ* v, size_t vstep, যৌক্তিক vT,
       ধ্রুবক দ্বিগুণ* b, size_t bstep, পূর্ণসংখ্যা nb,
       দ্বিগুণ* x, size_t xstep, uchar* বাফার )
{
    SVBkSbImpl_(m, n, w, wstep ? (পূর্ণসংখ্যা)(wstep/আকার(w[0])) : 1,
                u, (পূর্ণসংখ্যা)(ustep/আকার(u[0])), uT,
                v, (পূর্ণসংখ্যা)(vstep/আকার(v[0])), vT,
                b, (পূর্ণসংখ্যা)(bstep/আকার(b[0])), nb,
                x, (পূর্ণসংখ্যা)(xstep/আকার(x[0])),
                (দ্বিগুণ*)alignPtr(বাফার, আকার(দ্বিগুণ)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((দ্বিগুণ)m(0,0)*m(1,1) - (দ্বিগুণ)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((দ্বিগুণ)m(1,1)*m(2,2) - (দ্বিগুণ)m(1,2)*m(2,1)) -  \
                   m(0,1)*((দ্বিগুণ)m(1,0)*m(2,2) - (দ্বিগুণ)m(1,2)*m(2,0)) +  \
                   m(0,2)*((দ্বিগুণ)m(1,0)*m(2,1) - (দ্বিগুণ)m(1,1)*m(2,0)))

দ্বিগুণ determinant( প্রবেশ_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    দ্বিগুণ ফলাফল = 0;
    পূর্ণসংখ্যা type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    ধ্রুবক uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((দশমিক*)(m + y*step))[x]
    #define Md(y, x) ((দ্বিগুণ*)(m + y*step))[x]

    যদি( type == CV_32F )
    {
        যদি( rows == 2 )
            ফলাফল = det2(Mf);
        অন্যথা যদি( rows == 3 )
            ফলাফল = det3(Mf);
        অন্যথা যদি( rows == 1 )
            ফলাফল = Mf(0,0);
        অন্যথা
        {
            size_t buf_আকার = rows*rows*আকার(দশমিক);
            Auto_বাফার<uchar> বাফার(buf_আকার);
            Mat a(rows, rows, CV_32F, বাফার.তথ্য());
            mat.copyTo(a);

            ফলাফল = hal::LU32f(a.ptr<দশমিক>(), a.step, rows, 0, 0, 0);
            যদি( ফলাফল )
            {
                জন্য( পূর্ণসংখ্যা i = 0; i < rows; i++ )
                    ফলাফল *= a.at<দশমিক>(i,i);
            }
        }
    }
    অন্যথা
    {
        যদি( rows == 2 )
            ফলাফল = det2(Md);
        অন্যথা যদি( rows == 3 )
            ফলাফল = det3(Md);
        অন্যথা যদি( rows == 1 )
            ফলাফল = Md(0,0);
        অন্যথা
        {
            size_t buf_আকার = rows*rows*আকার(দ্বিগুণ);
            Auto_বাফার<uchar> বাফার(buf_আকার);
            Mat a(rows, rows, CV_64F, বাফার.তথ্য());
            mat.copyTo(a);

            ফলাফল = hal::LU64f(a.ptr<দ্বিগুণ>(), a.step, rows, 0, 0, 0);
            যদি( ফলাফল )
            {
                জন্য( পূর্ণসংখ্যা i = 0; i < rows; i++ )
                    ফলাফল *= a.at<দ্বিগুণ>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    ফেরত ফলাফল;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((দশমিক*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((দ্বিগুণ*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((দশমিক*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((দ্বিগুণ*)(dstdata + y*dststep))[x]

দ্বিগুণ invert( প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা method )
{
    CV_INSTRUMENT_REGION();

    যৌক্তিক ফলাফল = মিথ্যা;
    Mat src = _src.getMat();
    পূর্ণসংখ্যা type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_আকার(type);
    পূর্ণসংখ্যা m = src.rows, n = src.cols;

    যদি( method == DECOMP_SVD )
    {
        পূর্ণসংখ্যা nm = std::min(m, n);

        Auto_বাফার<uchar> _buf((m*nm + nm + nm*n)*esz + আকার(দ্বিগুণ));
        uchar* buf = alignPtr((uchar*)_buf.তথ্য(), (পূর্ণসংখ্যা)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        ফেরত type == CV_32F ?
            (w.ptr<দশমিক>()[0] >= FLT_EPSILON ?
             w.ptr<দশমিক>()[n-1]/w.ptr<দশমিক>()[0] : 0) :
            (w.ptr<দ্বিগুণ>()[0] >= DBL_EPSILON ?
             w.ptr<দ্বিগুণ>()[n-1]/w.ptr<দ্বিগুণ>()[0] : 0);
    }

    CV_Assert( m == n );

    যদি( method == DECOMP_EIG )
    {
        Auto_বাফার<uchar> _buf((n*n*2 + n)*esz + আকার(দ্বিগুণ));
        uchar* buf = alignPtr((uchar*)_buf.তথ্য(), (পূর্ণসংখ্যা)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        ফেরত type == CV_32F ?
        (w.ptr<দশমিক>()[0] >= FLT_EPSILON ?
         w.ptr<দশমিক>()[n-1]/w.ptr<দশমিক>()[0] : 0) :
        (w.ptr<দ্বিগুণ>()[0] >= DBL_EPSILON ?
         w.ptr<দ্বিগুণ>()[n-1]/w.ptr<দ্বিগুণ>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    যদি( n <= 3 )
    {
        ধ্রুবক uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        যদি( n == 2 )
        {
            যদি( type == CV_32FC1 )
            {
                দ্বিগুণ d = det2(Sf);
                যদি( d != 0. )
                {
                    ফলাফল = সত্য;
                    d = 1./d;
                #যদি CV_SIMD128
                    ধ্রুবক দশমিক d_32f = (দশমিক)d;
                    ধ্রুবক v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((ধ্রুবক দশমিক *)srcdata, (ধ্রুবক দশমিক *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_নিচের(v_rotate_ডান<1>(s0), s0));
                    v_store_নিচের((দশমিক*)dstdata, s0);
                    v_store_উপরের((দশমিক*)(dstdata + dststep), s0);
                #অন্যথা
                    দ্বিগুণ t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (দশমিক)t0;
                    Df(0,0) = (দশমিক)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (দশমিক)t0;
                    Df(1,0) = (দশমিক)t1;
                #endif
                }
            }
            অন্যথা
            {
                দ্বিগুণ d = det2(Sd);
                যদি( d != 0. )
                {
                    ফলাফল = সত্য;
                    d = 1./d;
                #যদি CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((ধ্রুবক দ্বিগুণ *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((ধ্রুবক দ্বিগুণ *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<দ্বিগুণ>(0), v_extract<1>(s0, s1));//12
                    v_store((দ্বিগুণ*)dstdata, v_combine_নিচের(sm, ss));//31
                    v_store((দ্বিগুণ*)(dstdata + dststep), v_combine_উপরের(ss, sm));//20
                #অন্যথা
                    দ্বিগুণ t0, t1;
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
        অন্যথা যদি( n == 3 )
        {
            যদি( type == CV_32FC1 )
            {
                দ্বিগুণ d = det3(Sf);

                যদি( d != 0. )
                {
                    দ্বিগুণ t[12];

                    ফলাফল = সত্য;
                    d = 1./d;
                    t[0] = (((দ্বিগুণ)Sf(1,1) * Sf(2,2) - (দ্বিগুণ)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((দ্বিগুণ)Sf(0,2) * Sf(2,1) - (দ্বিগুণ)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((দ্বিগুণ)Sf(0,1) * Sf(1,2) - (দ্বিগুণ)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((দ্বিগুণ)Sf(1,2) * Sf(2,0) - (দ্বিগুণ)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((দ্বিগুণ)Sf(0,0) * Sf(2,2) - (দ্বিগুণ)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((দ্বিগুণ)Sf(0,2) * Sf(1,0) - (দ্বিগুণ)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((দ্বিগুণ)Sf(1,0) * Sf(2,1) - (দ্বিগুণ)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((দ্বিগুণ)Sf(0,1) * Sf(2,0) - (দ্বিগুণ)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((দ্বিগুণ)Sf(0,0) * Sf(1,1) - (দ্বিগুণ)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (দশমিক)t[0]; Df(0,1) = (দশমিক)t[1]; Df(0,2) = (দশমিক)t[2];
                    Df(1,0) = (দশমিক)t[3]; Df(1,1) = (দশমিক)t[4]; Df(1,2) = (দশমিক)t[5];
                    Df(2,0) = (দশমিক)t[6]; Df(2,1) = (দশমিক)t[7]; Df(2,2) = (দশমিক)t[8];
                }
            }
            অন্যথা
            {
                দ্বিগুণ d = det3(Sd);
                যদি( d != 0. )
                {
                    ফলাফল = সত্য;
                    d = 1./d;
                    দ্বিগুণ t[9];

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
        অন্যথা
        {
            CV_Assert( n == 1 );

            যদি( type == CV_32FC1 )
            {
                দ্বিগুণ d = Sf(0,0);
                যদি( d != 0. )
                {
                    ফলাফল = সত্য;
                    Df(0,0) = (দশমিক)(1./d);
                }
            }
            অন্যথা
            {
                দ্বিগুণ d = Sd(0,0);
                যদি( d != 0. )
                {
                    ফলাফল = সত্য;
                    Dd(0,0) = 1./d;
                }
            }
        }
        যদি( !ফলাফল )
            dst = Scalar(0);
        ফেরত ফলাফল;
    }

   পূর্ণসংখ্যা elem_আকার_2 = CV_ELEM_আকার(type);
    Auto_বাফার<uchar> buf(n*n*elem_আকার_2);
    Mat src1(n, n, type, buf.তথ্য());
    src.copyTo(src1);
    setIdentity(dst);

    যদি( method == DECOMP_LU && type == CV_32F )
        ফলাফল = hal::LU32f(src1.ptr<দশমিক>(), src1.step, n, dst.ptr<দশমিক>(), dst.step, n) != 0;
    অন্যথা যদি( method == DECOMP_LU && type == CV_64F )
        ফলাফল = hal::LU64f(src1.ptr<দ্বিগুণ>(), src1.step, n, dst.ptr<দ্বিগুণ>(), dst.step, n) != 0;
    অন্যথা যদি( method == DECOMP_CHOLESKY && type == CV_32F )
        ফলাফল = hal::Cholesky32f(src1.ptr<দশমিক>(), src1.step, n, dst.ptr<দশমিক>(), dst.step, n);
    অন্যথা
        ফলাফল = hal::Cholesky64f(src1.ptr<দ্বিগুণ>(), src1.step, n, dst.ptr<দ্বিগুণ>(), dst.step, n);

    যদি( !ফলাফল )
        dst = Scalar(0);

    ফেরত ফলাফল;
}

UMat UMat::inv(পূর্ণসংখ্যা method) ধ্রুবক
{
    UMat m;
    invert(*এই, m, method);
    ফেরত m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

যৌক্তিক solve( প্রবেশ_Array _src, প্রবেশ_Array _src2arg, OutputArray _dst, পূর্ণসংখ্যা method )
{
    CV_INSTRUMENT_REGION();

    যৌক্তিক ফলাফল = সত্য;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    পূর্ণসংখ্যা type = src.type();
    যৌক্তিক is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    যদি( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((দশমিক*)(bdata + y*src2step))[0]
        #define bd(y) ((দ্বিগুণ*)(bdata + y*src2step))[0]

        ধ্রুবক uchar* srcdata = src.ptr();
        ধ্রুবক uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        যদি( src.rows == 2 )
        {
            যদি( type == CV_32FC1 )
            {
                দ্বিগুণ d = det2(Sf);
                যদি( d != 0. )
                {
                    দ্বিগুণ t;
                    d = 1./d;
                    t = (দশমিক)(((দ্বিগুণ)bf(0)*Sf(1,1) - (দ্বিগুণ)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (দশমিক)(((দ্বিগুণ)bf(1)*Sf(0,0) - (দ্বিগুণ)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (দশমিক)t;
                }
                অন্যথা
                    ফলাফল = মিথ্যা;
            }
            অন্যথা
            {
                দ্বিগুণ d = det2(Sd);
                যদি( d != 0. )
                {
                    দ্বিগুণ t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                অন্যথা
                    ফলাফল = মিথ্যা;
            }
        }
        অন্যথা যদি( src.rows == 3 )
        {
            যদি( type == CV_32FC1 )
            {
                দ্বিগুণ d = det3(Sf);
                যদি( d != 0. )
                {
                    দশমিক t[3];
                    d = 1./d;

                    t[0] = (দশমিক)(d*
                           (bf(0)*((দ্বিগুণ)Sf(1,1)*Sf(2,2) - (দ্বিগুণ)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((দ্বিগুণ)bf(1)*Sf(2,2) - (দ্বিগুণ)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((দ্বিগুণ)bf(1)*Sf(2,1) - (দ্বিগুণ)Sf(1,1)*bf(2))));

                    t[1] = (দশমিক)(d*
                           (Sf(0,0)*(দ্বিগুণ)(bf(1)*Sf(2,2) - (দ্বিগুণ)Sf(1,2)*bf(2)) -
                            bf(0)*((দ্বিগুণ)Sf(1,0)*Sf(2,2) - (দ্বিগুণ)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((দ্বিগুণ)Sf(1,0)*bf(2) - (দ্বিগুণ)bf(1)*Sf(2,0))));

                    t[2] = (দশমিক)(d*
                           (Sf(0,0)*((দ্বিগুণ)Sf(1,1)*bf(2) - (দ্বিগুণ)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((দ্বিগুণ)Sf(1,0)*bf(2) - (দ্বিগুণ)bf(1)*Sf(2,0)) +
                            bf(0)*((দ্বিগুণ)Sf(1,0)*Sf(2,1) - (দ্বিগুণ)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                অন্যথা
                    ফলাফল = মিথ্যা;
            }
            অন্যথা
            {
                দ্বিগুণ d = det3(Sd);
                যদি( d != 0. )
                {
                    দ্বিগুণ t[9];

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
                অন্যথা
                    ফলাফল = মিথ্যা;
            }
        }
        অন্যথা
        {
            CV_Assert( src.rows == 1 );

            যদি( type == CV_32FC1 )
            {
                দ্বিগুণ d = Sf(0,0);
                যদি( d != 0. )
                    Df(0,0) = (দশমিক)(bf(0)/d);
                অন্যথা
                    ফলাফল = মিথ্যা;
            }
            অন্যথা
            {
                দ্বিগুণ d = Sd(0,0);
                যদি( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                অন্যথা
                    ফলাফল = মিথ্যা;
            }
        }
        ফেরত ফলাফল;
    }

    পূর্ণসংখ্যা m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_আকার(type), bufsize = 0;
    size_t vstep = align_আকার(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? align_আকার(m*esz, 16) : vstep;
    Auto_বাফার<uchar> বাফার;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    যদি( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    যদি( m == n )
        is_normal = মিথ্যা;
    অন্যথা যদি( is_normal )
    {
        m_ = n;
        যদি( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    যদি( is_normal )
        bufsize += n*nb*esz;
    যদি( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*আকার(দ্বিগুণ) + 32;

    বাফার.allocate(bufsize);
    uchar* ptr = alignPtr(বাফার.তথ্য(), 16);

    Mat a(m_, n, type, ptr, astep);

    যদি( is_normal )
        mulTransposed(src, a, সত্য);
    অন্যথা যদি( method != DECOMP_SVD )
        src.copyTo(a);
    অন্যথা
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    যদি( !is_normal )
    {
        যদি( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    অন্যথা
    {
        // a'*b
        যদি( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        অন্যথা
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    যদি( method == DECOMP_LU )
    {
        যদি( type == CV_32F )
            ফলাফল = hal::LU32f(a.ptr<দশমিক>(), a.step, n, dst.ptr<দশমিক>(), dst.step, nb) != 0;
        অন্যথা
            ফলাফল = hal::LU64f(a.ptr<দ্বিগুণ>(), a.step, n, dst.ptr<দ্বিগুণ>(), dst.step, nb) != 0;
    }
    অন্যথা যদি( method == DECOMP_CHOLESKY )
    {
        যদি( type == CV_32F )
            ফলাফল = hal::Cholesky32f(a.ptr<দশমিক>(), a.step, n, dst.ptr<দশমিক>(), dst.step, nb);
        অন্যথা
            ফলাফল = hal::Cholesky64f(a.ptr<দ্বিগুণ>(), a.step, n, dst.ptr<দ্বিগুণ>(), dst.step, nb);
    }
    অন্যথা যদি( method == DECOMP_QR )
    {
        Mat rhsMat;
        যদি( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        অন্যথা
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        যদি( type == CV_32F )
            ফলাফল = hal::QR32f(a.ptr<দশমিক>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<দশমিক>(), rhsMat.step, ফাঁকা) != 0;
        অন্যথা
            ফলাফল = hal::QR64f(a.ptr<দ্বিগুণ>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<দ্বিগুণ>(), rhsMat.step, ফাঁকা) != 0;

        যদি (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    অন্যথা
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        যদি( method == DECOMP_EIG )
        {
            যদি( type == CV_32F )
                Jacobi(a.ptr<দশমিক>(), a.step, w.ptr<দশমিক>(), v.ptr<দশমিক>(), v.step, n, ptr);
            অন্যথা
                Jacobi(a.ptr<দ্বিগুণ>(), a.step, w.ptr<দ্বিগুণ>(), v.ptr<দ্বিগুণ>(), v.step, n, ptr);
            u = v;
        }
        অন্যথা
        {
            যদি( type == CV_32F )
                JacobiSVD(a.ptr<দশমিক>(), a.step, w.ptr<দশমিক>(), v.ptr<দশমিক>(), v.step, m_, n);
            অন্যথা
                JacobiSVD(a.ptr<দ্বিগুণ>(), a.step, w.ptr<দ্বিগুণ>(), v.ptr<দ্বিগুণ>(), v.step, m_, n);
            u = a;
        }

        যদি( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<দশমিক>(), 0, u.ptr<দশমিক>(), u.step, সত্য,
                   v.ptr<দশমিক>(), v.step, সত্য, src2.ptr<দশমিক>(),
                   src2.step, nb, dst.ptr<দশমিক>(), dst.step, ptr);
        }
        অন্যথা
        {
            SVBkSb(m_, n, w.ptr<দ্বিগুণ>(), 0, u.ptr<দ্বিগুণ>(), u.step, সত্য,
                   v.ptr<দ্বিগুণ>(), v.step, সত্য, src2.ptr<দ্বিগুণ>(),
                   src2.step, nb, dst.ptr<দ্বিগুণ>(), dst.step, ptr);
        }
        ফলাফল = সত্য;
    }

    যদি( !ফলাফল )
        dst = Scalar(0);

    ফেরত ফলাফল;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

যৌক্তিক eigen( প্রবেশ_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    পূর্ণসংখ্যা type = src.type();
    পূর্ণসংখ্যা n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    যদি( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    ধ্রুবক যৌক্তিক evecNeeded = _evects.needed();
    ধ্রুবক পূর্ণসংখ্যা esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    যদি ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        যদি ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            যদি ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ফেরত সত্য;
        }
    } অন্যথা { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        যদি ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            যদি ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            ফেরত সত্য;
        }
    }
    ফেরত মিথ্যা;
#অন্যথা

    size_t elem_আকার = src.elem_আকার(), astep = align_আকার(n*elem_আকার, 16);
    Auto_বাফার<uchar> buf(n*astep + n*5*elem_আকার + 32);
    uchar* ptr = alignPtr(buf.তথ্য(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elem_আকার*n;
    src.copyTo(a);
    যৌক্তিক ok = type == CV_32F ?
        Jacobi(a.ptr<দশমিক>(), a.step, w.ptr<দশমিক>(), v.ptr<দশমিক>(), v.step, n, ptr) :
        Jacobi(a.ptr<দ্বিগুণ>(), a.step, w.ptr<দ্বিগুণ>(), v.ptr<দ্বিগুণ>(), v.step, n, ptr);

    w.copyTo(_evals);
    ফেরত ok;
#endif
}

স্থির শূন্য _SVDcompute( প্রবেশ_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, পূর্ণসংখ্যা flags )
{
    Mat src = _aarr.getMat();
    পূর্ণসংখ্যা m = src.rows, n = src.cols;
    পূর্ণসংখ্যা type = src.type();
    যৌক্তিক compute_uv = _u.needed() || _vt.needed();
    যৌক্তিক full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    যদি( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = মিথ্যা;
    }

    যৌক্তিক at = মিথ্যা;
    যদি( m < n )
    {
        std::swap(m, n);
        at = সত্য;
    }

    পূর্ণসংখ্যা urows = full_uv ? m : n;
    size_t esz = src.elem_আকার(), astep = align_আকার(m*esz, 16), vstep = align_আকার(n*esz, 16);
    Auto_বাফার<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.তথ্য(), 16);
    Mat অস্থায়ী_a(n, m, type, buf, astep);
    Mat অস্থায়ী_w(n, 1, type, buf + urows*astep);
    Mat অস্থায়ী_u(urows, m, type, buf, astep), অস্থায়ী_v;

    যদি( compute_uv )
        অস্থায়ী_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    যদি( urows > n )
        অস্থায়ী_u = Scalar::all(0);

    যদি( !at )
        transpose(src, অস্থায়ী_a);
    অন্যথা
        src.copyTo(অস্থায়ী_a);

    যদি( type == CV_32F )
    {
        JacobiSVD(অস্থায়ী_a.ptr<দশমিক>(), অস্থায়ী_u.step, অস্থায়ী_w.ptr<দশমিক>(),
              অস্থায়ী_v.ptr<দশমিক>(), অস্থায়ী_v.step, m, n, compute_uv ? urows : 0);
    }
    অন্যথা
    {
        JacobiSVD(অস্থায়ী_a.ptr<দ্বিগুণ>(), অস্থায়ী_u.step, অস্থায়ী_w.ptr<দ্বিগুণ>(),
              অস্থায়ী_v.ptr<দ্বিগুণ>(), অস্থায়ী_v.step, m, n, compute_uv ? urows : 0);
    }
    অস্থায়ী_w.copyTo(_w);
    যদি( compute_uv )
    {
        যদি( !at )
        {
            যদি( _u.needed() )
                transpose(অস্থায়ী_u, _u);
            যদি( _vt.needed() )
                অস্থায়ী_v.copyTo(_vt);
        }
        অন্যথা
        {
            যদি( _u.needed() )
                transpose(অস্থায়ী_v, _u);
            যদি( _vt.needed() )
                অস্থায়ী_u.copyTo(_vt);
        }
    }
}


শূন্য SVD::compute( প্রবেশ_Array a, OutputArray w, OutputArray u, OutputArray vt, পূর্ণসংখ্যা flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

শূন্য SVD::compute( প্রবেশ_Array a, OutputArray w, পূর্ণসংখ্যা flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

শূন্য SVD::backSubst( প্রবেশ_Array _w, প্রবেশ_Array _u, প্রবেশ_Array _vt,
                     প্রবেশ_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    পূর্ণসংখ্যা type = w.type(), esz = (পূর্ণসংখ্যা)w.elem_আকার();
    পূর্ণসংখ্যা m = u.rows, n = vt.cols, nb = rhs.তথ্য ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    Auto_বাফার<uchar> বাফার(nb*আকার(দ্বিগুণ) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.তথ্য && vt.তথ্য && w.তথ্য );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == আকার_2(nm, 1) || w.size() == আকার_2(1, nm) || w.size() == আকার_2(vt.rows, u.cols)) );
    CV_Assert( rhs.তথ্য == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    যদি( type == CV_32F )
        SVBkSb(m, n, w.ptr<দশমিক>(), wstep, u.ptr<দশমিক>(), u.step, মিথ্যা,
               vt.ptr<দশমিক>(), vt.step, সত্য, rhs.ptr<দশমিক>(), rhs.step, nb,
               dst.ptr<দশমিক>(), dst.step, বাফার.তথ্য());
    অন্যথা যদি( type == CV_64F )
        SVBkSb(m, n, w.ptr<দ্বিগুণ>(), wstep, u.ptr<দ্বিগুণ>(), u.step, মিথ্যা,
               vt.ptr<দ্বিগুণ>(), vt.step, সত্য, rhs.ptr<দ্বিগুণ>(), rhs.step, nb,
               dst.ptr<দ্বিগুণ>(), dst.step, বাফার.তথ্য());
    অন্যথা
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(প্রবেশ_Array a, পূর্ণসংখ্যা flags)
{
    _SVDcompute(a, w, u, vt, flags);
    ফেরত *এই;
}


শূন্য SVD::backSubst( প্রবেশ_Array rhs, OutputArray dst ) ধ্রুবক
{
    backSubst( w, u, vt, rhs, dst );
}

}


শূন্য cv::SVDecomp(প্রবেশ_Array src, OutputArray w, OutputArray u, OutputArray vt, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

শূন্য cv::SVBackSubst(প্রবেশ_Array w, প্রবেশ_Array u, প্রবেশ_Array vt, প্রবেশ_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
