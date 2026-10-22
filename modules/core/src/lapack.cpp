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

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ <limits>

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

পূর্ণাঙ্ক LU(ভিন্ন* A, size_t astep, পূর্ণাঙ্ক m, ভিন্ন* b, size_t bstep, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    ঘূৰাই hal::LU32f(A, astep, m, b, bstep, n);
}

পূর্ণাঙ্ক LU(দ্ৱিগুণ* A, size_t astep, পূর্ণাঙ্ক m, দ্ৱিগুণ* b, size_t bstep, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    ঘূৰাই hal::LU64f(A, astep, m, b, bstep, n);
}

তার্কিক Cholesky(ভিন্ন* A, size_t astep, পূর্ণাঙ্ক m, ভিন্ন* b, size_t bstep, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    ঘূৰাই hal::Cholesky32f(A, astep, m, b, bstep, n);
}

তার্কিক Cholesky(দ্ৱিগুণ* A, size_t astep, পূর্ণাঙ্ক m, দ্ৱিগুণ* b, size_t bstep, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    ঘূৰাই hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> স্থির অন্তঃস্থ _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    যদি( a > b )
    {
        b /= a;
        ঘূৰাই a*std::sqrt(1 + b*b);
    }
    যদি( b > 0 )
    {
        a /= b;
        ঘূৰাই b*std::sqrt(1 + a*a);
    }
    ঘূৰাই 0;
}


template<typename _Tp> তার্কিক
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, পূর্ণাঙ্ক n, uchar* buf )
{
    ধ্রুৱ _Tp eps = std::numeric_limits<_Tp>::epsilon();
    পূর্ণাঙ্ক i, j, k, m;

    astep /= পরিমাণ(A[0]);
    যদি( V )
    {
        vstep /= পরিমাণ(V[0]);
        বাবে( i = 0; i < n; i++ )
        {
            বাবে( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    পূর্ণাঙ্ক iters, maxIters = n*n*30;

    পূর্ণাঙ্ক* indR = (পূর্ণাঙ্ক*)alignPtr(buf, পরিমাণ(পূর্ণাঙ্ক));
    পূর্ণাঙ্ক* indC = indR + n;
    _Tp mv = (_Tp)0;

    বাবে( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        যদি( k < n - 1 )
        {
            বাবে( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                যদি( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        যদি( k > 0 )
        {
            বাবে( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                যদি( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    যদি( n > 1 ) বাবে( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        বাবে( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            যদি( mv < val )
                mv = val, k = i;
        }
        পূর্ণাঙ্ক l = indR[k];
        বাবে( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            যদি( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        যদি( std::abs(p) <= eps )
            ভঙ্গ;
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
        বাবে( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        বাবে( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        বাবে( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        যদি( V )
            বাবে( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        বাবে( j = 0; j < 2; j++ )
        {
            পূর্ণাঙ্ক idx = j == 0 ? k : l;
            যদি( idx < n - 1 )
            {
                বাবে( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    যদি( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            যদি( idx > 0 )
            {
                বাবে( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
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
    বাবে( k = 0; k < n-1; k++ )
    {
        m = k;
        বাবে( i = k+1; i < n; i++ )
        {
            যদি( W[m] < W[i] )
                m = i;
        }
        যদি( k != m )
        {
            std::swap(W[m], W[k]);
            যদি( V )
                বাবে( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    ঘূৰাই সঁচা;
}

স্থির তার্কিক Jacobi( ভিন্ন* S, size_t sstep, ভিন্ন* e, ভিন্ন* E, size_t estep, পূর্ণাঙ্ক n, uchar* buf )
{
    ঘূৰাই JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

স্থির তার্কিক Jacobi( দ্ৱিগুণ* S, size_t sstep, দ্ৱিগুণ* e, দ্ৱিগুণ* E, size_t estep, পূর্ণাঙ্ক n, uchar* buf )
{
    ঘূৰাই JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> সংরচনা VBLAS
{
    পূর্ণাঙ্ক dot(ধ্রুৱ T*, ধ্রুৱ T*, পূর্ণাঙ্ক, T*) ধ্রুৱ { ঘূৰাই 0; }
    পূর্ণাঙ্ক givens(T*, T*, পূর্ণাঙ্ক, T, T) ধ্রুৱ { ঘূৰাই 0; }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<> অন্তঃস্থ পূর্ণাঙ্ক VBLAS<ভিন্ন>::dot(ধ্রুৱ ভিন্ন* a, ধ্রুৱ ভিন্ন* b, পূর্ণাঙ্ক n, ভিন্ন* result) ধ্রুৱ
{
    যদি( n < 2*VTraits<v_float32>::vlanes() )
        ঘূৰাই 0;
    পূর্ণাঙ্ক k = 0;
    v_float32 s0 = vx_setzero_f32();
    বাবে( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    ঘূৰাই k;
}


template<> অন্তঃস্থ পূর্ণাঙ্ক VBLAS<ভিন্ন>::givens(ভিন্ন* a, ভিন্ন* b, পূর্ণাঙ্ক n, ভিন্ন c, ভিন্ন s) ধ্রুৱ
{
    যদি( n < VTraits<v_float32>::vlanes())
        ঘূৰাই 0;
    পূর্ণাঙ্ক k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    বাবে( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ঘূৰাই k;
}


#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> অন্তঃস্থ পূর্ণাঙ্ক VBLAS<দ্ৱিগুণ>::dot(ধ্রুৱ দ্ৱিগুণ* a, ধ্রুৱ দ্ৱিগুণ* b, পূর্ণাঙ্ক n, দ্ৱিগুণ* result) ধ্রুৱ
{
    যদি( n < 2*VTraits<v_float64>::vlanes() )
        ঘূৰাই 0;
    পূর্ণাঙ্ক k = 0;
    v_float64 s0 = vx_setzero_f64();
    বাবে( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    ঘূৰাই k;
}


template<> অন্তঃস্থ পূর্ণাঙ্ক VBLAS<দ্ৱিগুণ>::givens(দ্ৱিগুণ* a, দ্ৱিগুণ* b, পূর্ণাঙ্ক n, দ্ৱিগুণ c, দ্ৱিগুণ s) ধ্রুৱ
{
    পূর্ণাঙ্ক k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    বাবে( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    ঘূৰাই k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> শূন্য
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক n1, দ্ৱিগুণ minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<দ্ৱিগুণ> Wbuf(n);
    দ্ৱিগুণ* W = Wbuf.data();
    পূর্ণাঙ্ক i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    দ্ৱিগুণ sd;
    astep /= পরিমাণ(At[0]);
    vstep /= পরিমাণ(Vt[0]);

    বাবে( i = 0; i < n; i++ )
    {
        বাবে( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (দ্ৱিগুণ)t*t;
        }
        W[i] = sd;

        যদি( Vt )
        {
            বাবে( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    বাবে( iter = 0; iter < max_iter; iter++ )
    {
        তার্কিক changed = মিছা;

        বাবে( i = 0; i < n-1; i++ )
            বাবে( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                দ্ৱিগুণ a = W[i], p = 0, b = W[j];

                বাবে( k = 0; k < m; k++ )
                    p += (দ্ৱিগুণ)Ai[k]*Aj[k];

                যদি( std::abs(p) <= eps*std::sqrt((দ্ৱিগুণ)a*b) )
                    অনুৱর্ত;

                p *= 2;
                দ্ৱিগুণ beta = a - b, gamma = hypot((দ্ৱিগুণ)p, beta);
                যদি( beta < 0 )
                {
                    দ্ৱিগুণ delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                অন্যথা
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                বাবে( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (দ্ৱিগুণ)t0*t0; b += (দ্ৱিগুণ)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = সঁচা;

                যদি( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    বাবে( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        যদি( !changed )
            ভঙ্গ;
    }

    বাবে( i = 0; i < n; i++ )
    {
        বাবে( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (দ্ৱিগুণ)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    বাবে( i = 0; i < n-1; i++ )
    {
        j = i;
        বাবে( k = i+1; k < n; k++ )
        {
            যদি( W[j] < W[k] )
                j = k;
        }
        যদি( i != j )
        {
            std::swap(W[i], W[j]);
            যদি( Vt )
            {
                বাবে( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                বাবে( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    বাবে( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    যদি( !Vt )
        ঘূৰাই;

    RNG rng(0x12345678);
    বাবে( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        বাবে( পূর্ণাঙ্ক ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ধ্রুৱ _Tp val0 = (_Tp)(1./m);
            বাবে( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            বাবে( iter = 0; iter < 2; iter++ )
            {
                বাবে( j = 0; j < i; j++ )
                {
                    sd = 0;
                    বাবে( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    বাবে( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    বাবে( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            বাবে( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (দ্ৱিগুণ)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        বাবে( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


স্থির শূন্য JacobiSVD(ভিন্ন* At, size_t astep, ভিন্ন* W, ভিন্ন* Vt, size_t vstep, পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক n1=-1)
{
    hal::SVD32f(At, astep, W, রিক্ত, astep, Vt, vstep, m, n, n1);
}

স্থির শূন্য JacobiSVD(দ্ৱিগুণ* At, size_t astep, দ্ৱিগুণ* W, দ্ৱিগুণ* Vt, size_t vstep, পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক n1=-1)
{
    hal::SVD64f(At, astep, W, রিক্ত, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> স্থির অন্তঃস্থ পূর্ণাঙ্ক
decodeSVDParameters(ধ্রুৱ fptype* U, ধ্রুৱ fptype* Vt, পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক n1)
{
    পূর্ণাঙ্ক halSVDFlag = 0;
    যদি(Vt == রিক্ত)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    অন্যথা যদি(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        যদি(U == রিক্ত)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    অন্যথা যদি(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        যদি(U == রিক্ত)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ঘূৰাই halSVDFlag;
}

শূন্য hal::SVD32f(ভিন্ন* At, size_t astep, ভিন্ন* W, ভিন্ন* U, size_t ustep, ভিন্ন* Vt, size_t vstep, পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

শূন্য hal::SVD64f(দ্ৱিগুণ* At, size_t astep, দ্ৱিগুণ* W, দ্ৱিগুণ* U, size_t ustep, দ্ৱিগুণ* Vt, size_t vstep, পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> স্থির শূন্য
MatrAXPY( পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, ধ্রুৱ T1* x, পূর্ণাঙ্ক dx,
         ধ্রুৱ T2* a, পূর্ণাঙ্ক inca, T3* y, পূর্ণাঙ্ক dy )
{
    পূর্ণাঙ্ক i;
    বাবে( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        পূর্ণাঙ্ক j = 0;
         #যদি CV_ENABLE_UNROLLED
        বাবে(; j <= n - 4; j += 4 )
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
        বাবে( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> স্থির শূন্য
SVBkSbImpl_( পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, ধ্রুৱ T* w, পূর্ণাঙ্ক incw,
       ধ্রুৱ T* u, পূর্ণাঙ্ক ldu, তার্কিক uT,
       ধ্রুৱ T* v, পূর্ণাঙ্ক ldv, তার্কিক vT,
       ধ্রুৱ T* b, পূর্ণাঙ্ক ldb, পূর্ণাঙ্ক nb,
       T* x, পূর্ণাঙ্ক ldx, দ্ৱিগুণ* buffer, T eps )
{
    দ্ৱিগুণ threshold = 0;
    পূর্ণাঙ্ক udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    পূর্ণাঙ্ক vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    পূর্ণাঙ্ক i, j, nm = std::min(m, n);

    যদি( !b )
        nb = m;

    বাবে( i = 0; i < n; i++ )
        বাবে( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    বাবে( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    বাবে( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        দ্ৱিগুণ wi = w[i*incw];
        যদি( (দ্ৱিগুণ)std::abs(wi) <= threshold )
            অনুৱর্ত;
        wi = 1/wi;

        যদি( nb == 1 )
        {
            দ্ৱিগুণ s = 0;
            যদি( b )
                বাবে( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            অন্যথা
                s = u[0];
            s *= wi;

            বাবে( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        অন্যথা
        {
            যদি( b )
            {
                বাবে( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                বাবে( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            অন্যথা
            {
                বাবে( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

স্থির শূন্য
SVBkSb( পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, ধ্রুৱ ভিন্ন* w, size_t wstep,
        ধ্রুৱ ভিন্ন* u, size_t ustep, তার্কিক uT,
        ধ্রুৱ ভিন্ন* v, size_t vstep, তার্কিক vT,
        ধ্রুৱ ভিন্ন* b, size_t bstep, পূর্ণাঙ্ক nb,
        ভিন্ন* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (পূর্ণাঙ্ক)(wstep/পরিমাণ(w[0])) : 1,
                u, (পূর্ণাঙ্ক)(ustep/পরিমাণ(u[0])), uT,
                v, (পূর্ণাঙ্ক)(vstep/পরিমাণ(v[0])), vT,
                b, (পূর্ণাঙ্ক)(bstep/পরিমাণ(b[0])), nb,
                x, (পূর্ণাঙ্ক)(xstep/পরিমাণ(x[0])),
                (দ্ৱিগুণ*)alignPtr(buffer, পরিমাণ(দ্ৱিগুণ)), (ভিন্ন)(DBL_EPSILON*2) );
}

স্থির শূন্য
SVBkSb( পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, ধ্রুৱ দ্ৱিগুণ* w, size_t wstep,
       ধ্রুৱ দ্ৱিগুণ* u, size_t ustep, তার্কিক uT,
       ধ্রুৱ দ্ৱিগুণ* v, size_t vstep, তার্কিক vT,
       ধ্রুৱ দ্ৱিগুণ* b, size_t bstep, পূর্ণাঙ্ক nb,
       দ্ৱিগুণ* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (পূর্ণাঙ্ক)(wstep/পরিমাণ(w[0])) : 1,
                u, (পূর্ণাঙ্ক)(ustep/পরিমাণ(u[0])), uT,
                v, (পূর্ণাঙ্ক)(vstep/পরিমাণ(v[0])), vT,
                b, (পূর্ণাঙ্ক)(bstep/পরিমাণ(b[0])), nb,
                x, (পূর্ণাঙ্ক)(xstep/পরিমাণ(x[0])),
                (দ্ৱিগুণ*)alignPtr(buffer, পরিমাণ(দ্ৱিগুণ)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((দ্ৱিগুণ)m(0,0)*m(1,1) - (দ্ৱিগুণ)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((দ্ৱিগুণ)m(1,1)*m(2,2) - (দ্ৱিগুণ)m(1,2)*m(2,1)) -  \
                   m(0,1)*((দ্ৱিগুণ)m(1,0)*m(2,2) - (দ্ৱিগুণ)m(1,2)*m(2,0)) +  \
                   m(0,2)*((দ্ৱিগুণ)m(1,0)*m(2,1) - (দ্ৱিগুণ)m(1,1)*m(2,0)))

দ্ৱিগুণ determinant( প্ৰবেশ_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    দ্ৱিগুণ result = 0;
    পূর্ণাঙ্ক type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    ধ্রুৱ uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((ভিন্ন*)(m + y*step))[x]
    #define Md(y, x) ((দ্ৱিগুণ*)(m + y*step))[x]

    যদি( type == CV_32F )
    {
        যদি( rows == 2 )
            result = det2(Mf);
        অন্যথা যদি( rows == 3 )
            result = det3(Mf);
        অন্যথা যদি( rows == 1 )
            result = Mf(0,0);
        অন্যথা
        {
            size_t bufSize = rows*rows*পরিমাণ(ভিন্ন);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<ভিন্ন>(), a.step, rows, 0, 0, 0);
            যদি( result )
            {
                বাবে( পূর্ণাঙ্ক i = 0; i < rows; i++ )
                    result *= a.at<ভিন্ন>(i,i);
            }
        }
    }
    অন্যথা
    {
        যদি( rows == 2 )
            result = det2(Md);
        অন্যথা যদি( rows == 3 )
            result = det3(Md);
        অন্যথা যদি( rows == 1 )
            result = Md(0,0);
        অন্যথা
        {
            size_t bufSize = rows*rows*পরিমাণ(দ্ৱিগুণ);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<দ্ৱিগুণ>(), a.step, rows, 0, 0, 0);
            যদি( result )
            {
                বাবে( পূর্ণাঙ্ক i = 0; i < rows; i++ )
                    result *= a.at<দ্ৱিগুণ>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    ঘূৰাই result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((ভিন্ন*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((দ্ৱিগুণ*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((ভিন্ন*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((দ্ৱিগুণ*)(dstdata + y*dststep))[x]

দ্ৱিগুণ invert( প্ৰবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক method )
{
    CV_INSTRUMENT_REGION();

    তার্কিক result = মিছা;
    Mat src = _src.getMat();
    পূর্ণাঙ্ক type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    পূর্ণাঙ্ক m = src.rows, n = src.cols;

    যদি( method == DECOMP_SVD )
    {
        পূর্ণাঙ্ক nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + পরিমাণ(দ্ৱিগুণ));
        uchar* buf = alignPtr((uchar*)_buf.data(), (পূর্ণাঙ্ক)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        ঘূৰাই type == CV_32F ?
            (w.ptr<ভিন্ন>()[0] >= FLT_EPSILON ?
             w.ptr<ভিন্ন>()[n-1]/w.ptr<ভিন্ন>()[0] : 0) :
            (w.ptr<দ্ৱিগুণ>()[0] >= DBL_EPSILON ?
             w.ptr<দ্ৱিগুণ>()[n-1]/w.ptr<দ্ৱিগুণ>()[0] : 0);
    }

    CV_Assert( m == n );

    যদি( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + পরিমাণ(দ্ৱিগুণ));
        uchar* buf = alignPtr((uchar*)_buf.data(), (পূর্ণাঙ্ক)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        ঘূৰাই type == CV_32F ?
        (w.ptr<ভিন্ন>()[0] >= FLT_EPSILON ?
         w.ptr<ভিন্ন>()[n-1]/w.ptr<ভিন্ন>()[0] : 0) :
        (w.ptr<দ্ৱিগুণ>()[0] >= DBL_EPSILON ?
         w.ptr<দ্ৱিগুণ>()[n-1]/w.ptr<দ্ৱিগুণ>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    যদি( n <= 3 )
    {
        ধ্রুৱ uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        যদি( n == 2 )
        {
            যদি( type == CV_32FC1 )
            {
                দ্ৱিগুণ d = det2(Sf);
                যদি( d != 0. )
                {
                    result = সঁচা;
                    d = 1./d;
                #যদি CV_SIMD128
                    ধ্রুৱ ভিন্ন d_32f = (ভিন্ন)d;
                    ধ্রুৱ v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((ধ্রুৱ ভিন্ন *)srcdata, (ধ্রুৱ ভিন্ন *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_নিচেৰ(v_rotate_ডান<1>(s0), s0));
                    v_store_নিচেৰ((ভিন্ন*)dstdata, s0);
                    v_store_উপৰেৰ((ভিন্ন*)(dstdata + dststep), s0);
                #অন্যথা
                    দ্ৱিগুণ t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (ভিন্ন)t0;
                    Df(0,0) = (ভিন্ন)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (ভিন্ন)t0;
                    Df(1,0) = (ভিন্ন)t1;
                #endif
                }
            }
            অন্যথা
            {
                দ্ৱিগুণ d = det2(Sd);
                যদি( d != 0. )
                {
                    result = সঁচা;
                    d = 1./d;
                #যদি CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((ধ্রুৱ দ্ৱিগুণ *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((ধ্রুৱ দ্ৱিগুণ *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<দ্ৱিগুণ>(0), v_extract<1>(s0, s1));//12
                    v_store((দ্ৱিগুণ*)dstdata, v_combine_নিচেৰ(sm, ss));//31
                    v_store((দ্ৱিগুণ*)(dstdata + dststep), v_combine_উপৰেৰ(ss, sm));//20
                #অন্যথা
                    দ্ৱিগুণ t0, t1;
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
                দ্ৱিগুণ d = det3(Sf);

                যদি( d != 0. )
                {
                    দ্ৱিগুণ t[12];

                    result = সঁচা;
                    d = 1./d;
                    t[0] = (((দ্ৱিগুণ)Sf(1,1) * Sf(2,2) - (দ্ৱিগুণ)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((দ্ৱিগুণ)Sf(0,2) * Sf(2,1) - (দ্ৱিগুণ)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((দ্ৱিগুণ)Sf(0,1) * Sf(1,2) - (দ্ৱিগুণ)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((দ্ৱিগুণ)Sf(1,2) * Sf(2,0) - (দ্ৱিগুণ)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((দ্ৱিগুণ)Sf(0,0) * Sf(2,2) - (দ্ৱিগুণ)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((দ্ৱিগুণ)Sf(0,2) * Sf(1,0) - (দ্ৱিগুণ)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((দ্ৱিগুণ)Sf(1,0) * Sf(2,1) - (দ্ৱিগুণ)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((দ্ৱিগুণ)Sf(0,1) * Sf(2,0) - (দ্ৱিগুণ)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((দ্ৱিগুণ)Sf(0,0) * Sf(1,1) - (দ্ৱিগুণ)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (ভিন্ন)t[0]; Df(0,1) = (ভিন্ন)t[1]; Df(0,2) = (ভিন্ন)t[2];
                    Df(1,0) = (ভিন্ন)t[3]; Df(1,1) = (ভিন্ন)t[4]; Df(1,2) = (ভিন্ন)t[5];
                    Df(2,0) = (ভিন্ন)t[6]; Df(2,1) = (ভিন্ন)t[7]; Df(2,2) = (ভিন্ন)t[8];
                }
            }
            অন্যথা
            {
                দ্ৱিগুণ d = det3(Sd);
                যদি( d != 0. )
                {
                    result = সঁচা;
                    d = 1./d;
                    দ্ৱিগুণ t[9];

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
                দ্ৱিগুণ d = Sf(0,0);
                যদি( d != 0. )
                {
                    result = সঁচা;
                    Df(0,0) = (ভিন্ন)(1./d);
                }
            }
            অন্যথা
            {
                দ্ৱিগুণ d = Sd(0,0);
                যদি( d != 0. )
                {
                    result = সঁচা;
                    Dd(0,0) = 1./d;
                }
            }
        }
        যদি( !result )
            dst = Scalar(0);
        ঘূৰাই result;
    }

   পূর্ণাঙ্ক elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    যদি( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<ভিন্ন>(), src1.step, n, dst.ptr<ভিন্ন>(), dst.step, n) != 0;
    অন্যথা যদি( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<দ্ৱিগুণ>(), src1.step, n, dst.ptr<দ্ৱিগুণ>(), dst.step, n) != 0;
    অন্যথা যদি( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<ভিন্ন>(), src1.step, n, dst.ptr<ভিন্ন>(), dst.step, n);
    অন্যথা
        result = hal::Cholesky64f(src1.ptr<দ্ৱিগুণ>(), src1.step, n, dst.ptr<দ্ৱিগুণ>(), dst.step, n);

    যদি( !result )
        dst = Scalar(0);

    ঘূৰাই result;
}

UMat UMat::inv(পূর্ণাঙ্ক method) ধ্রুৱ
{
    UMat m;
    invert(*এতত্, m, method);
    ঘূৰাই m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

তার্কিক solve( প্ৰবেশ_Array _src, প্ৰবেশ_Array _src2arg, OutputArray _dst, পূর্ণাঙ্ক method )
{
    CV_INSTRUMENT_REGION();

    তার্কিক result = সঁচা;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    পূর্ণাঙ্ক type = src.type();
    তার্কিক is_normal = (method & DECOMP_NORMAL) != 0;

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

        #define bf(y) ((ভিন্ন*)(bdata + y*src2step))[0]
        #define bd(y) ((দ্ৱিগুণ*)(bdata + y*src2step))[0]

        ধ্রুৱ uchar* srcdata = src.ptr();
        ধ্রুৱ uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        যদি( src.rows == 2 )
        {
            যদি( type == CV_32FC1 )
            {
                দ্ৱিগুণ d = det2(Sf);
                যদি( d != 0. )
                {
                    দ্ৱিগুণ t;
                    d = 1./d;
                    t = (ভিন্ন)(((দ্ৱিগুণ)bf(0)*Sf(1,1) - (দ্ৱিগুণ)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (ভিন্ন)(((দ্ৱিগুণ)bf(1)*Sf(0,0) - (দ্ৱিগুণ)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (ভিন্ন)t;
                }
                অন্যথা
                    result = মিছা;
            }
            অন্যথা
            {
                দ্ৱিগুণ d = det2(Sd);
                যদি( d != 0. )
                {
                    দ্ৱিগুণ t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                অন্যথা
                    result = মিছা;
            }
        }
        অন্যথা যদি( src.rows == 3 )
        {
            যদি( type == CV_32FC1 )
            {
                দ্ৱিগুণ d = det3(Sf);
                যদি( d != 0. )
                {
                    ভিন্ন t[3];
                    d = 1./d;

                    t[0] = (ভিন্ন)(d*
                           (bf(0)*((দ্ৱিগুণ)Sf(1,1)*Sf(2,2) - (দ্ৱিগুণ)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((দ্ৱিগুণ)bf(1)*Sf(2,2) - (দ্ৱিগুণ)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((দ্ৱিগুণ)bf(1)*Sf(2,1) - (দ্ৱিগুণ)Sf(1,1)*bf(2))));

                    t[1] = (ভিন্ন)(d*
                           (Sf(0,0)*(দ্ৱিগুণ)(bf(1)*Sf(2,2) - (দ্ৱিগুণ)Sf(1,2)*bf(2)) -
                            bf(0)*((দ্ৱিগুণ)Sf(1,0)*Sf(2,2) - (দ্ৱিগুণ)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((দ্ৱিগুণ)Sf(1,0)*bf(2) - (দ্ৱিগুণ)bf(1)*Sf(2,0))));

                    t[2] = (ভিন্ন)(d*
                           (Sf(0,0)*((দ্ৱিগুণ)Sf(1,1)*bf(2) - (দ্ৱিগুণ)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((দ্ৱিগুণ)Sf(1,0)*bf(2) - (দ্ৱিগুণ)bf(1)*Sf(2,0)) +
                            bf(0)*((দ্ৱিগুণ)Sf(1,0)*Sf(2,1) - (দ্ৱিগুণ)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                অন্যথা
                    result = মিছা;
            }
            অন্যথা
            {
                দ্ৱিগুণ d = det3(Sd);
                যদি( d != 0. )
                {
                    দ্ৱিগুণ t[9];

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
                    result = মিছা;
            }
        }
        অন্যথা
        {
            CV_Assert( src.rows == 1 );

            যদি( type == CV_32FC1 )
            {
                দ্ৱিগুণ d = Sf(0,0);
                যদি( d != 0. )
                    Df(0,0) = (ভিন্ন)(bf(0)/d);
                অন্যথা
                    result = মিছা;
            }
            অন্যথা
            {
                দ্ৱিগুণ d = Sd(0,0);
                যদি( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                অন্যথা
                    result = মিছা;
            }
        }
        ঘূৰাই result;
    }

    পূর্ণাঙ্ক m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    যদি( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    যদি( m == n )
        is_normal = মিছা;
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
        bufsize += n*5*esz + n*vstep + nb*পরিমাণ(দ্ৱিগুণ) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    যদি( is_normal )
        mulTransposed(src, a, সঁচা);
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
            result = hal::LU32f(a.ptr<ভিন্ন>(), a.step, n, dst.ptr<ভিন্ন>(), dst.step, nb) != 0;
        অন্যথা
            result = hal::LU64f(a.ptr<দ্ৱিগুণ>(), a.step, n, dst.ptr<দ্ৱিগুণ>(), dst.step, nb) != 0;
    }
    অন্যথা যদি( method == DECOMP_CHOLESKY )
    {
        যদি( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<ভিন্ন>(), a.step, n, dst.ptr<ভিন্ন>(), dst.step, nb);
        অন্যথা
            result = hal::Cholesky64f(a.ptr<দ্ৱিগুণ>(), a.step, n, dst.ptr<দ্ৱিগুণ>(), dst.step, nb);
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
            result = hal::QR32f(a.ptr<ভিন্ন>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<ভিন্ন>(), rhsMat.step, রিক্ত) != 0;
        অন্যথা
            result = hal::QR64f(a.ptr<দ্ৱিগুণ>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<দ্ৱিগুণ>(), rhsMat.step, রিক্ত) != 0;

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
                Jacobi(a.ptr<ভিন্ন>(), a.step, w.ptr<ভিন্ন>(), v.ptr<ভিন্ন>(), v.step, n, ptr);
            অন্যথা
                Jacobi(a.ptr<দ্ৱিগুণ>(), a.step, w.ptr<দ্ৱিগুণ>(), v.ptr<দ্ৱিগুণ>(), v.step, n, ptr);
            u = v;
        }
        অন্যথা
        {
            যদি( type == CV_32F )
                JacobiSVD(a.ptr<ভিন্ন>(), a.step, w.ptr<ভিন্ন>(), v.ptr<ভিন্ন>(), v.step, m_, n);
            অন্যথা
                JacobiSVD(a.ptr<দ্ৱিগুণ>(), a.step, w.ptr<দ্ৱিগুণ>(), v.ptr<দ্ৱিগুণ>(), v.step, m_, n);
            u = a;
        }

        যদি( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<ভিন্ন>(), 0, u.ptr<ভিন্ন>(), u.step, সঁচা,
                   v.ptr<ভিন্ন>(), v.step, সঁচা, src2.ptr<ভিন্ন>(),
                   src2.step, nb, dst.ptr<ভিন্ন>(), dst.step, ptr);
        }
        অন্যথা
        {
            SVBkSb(m_, n, w.ptr<দ্ৱিগুণ>(), 0, u.ptr<দ্ৱিগুণ>(), u.step, সঁচা,
                   v.ptr<দ্ৱিগুণ>(), v.step, সঁচা, src2.ptr<দ্ৱিগুণ>(),
                   src2.step, nb, dst.ptr<দ্ৱিগুণ>(), dst.step, ptr);
        }
        result = সঁচা;
    }

    যদি( !result )
        dst = Scalar(0);

    ঘূৰাই result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

তার্কিক eigen( প্ৰবেশ_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    পূর্ণাঙ্ক type = src.type();
    পূর্ণাঙ্ক n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    যদি( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    ধ্রুৱ তার্কিক evecNeeded = _evects.needed();
    ধ্রুৱ পূর্ণাঙ্ক esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
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
            ঘূৰাই সঁচা;
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
            ঘূৰাই সঁচা;
        }
    }
    ঘূৰাই মিছা;
#অন্যথা

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    তার্কিক ok = type == CV_32F ?
        Jacobi(a.ptr<ভিন্ন>(), a.step, w.ptr<ভিন্ন>(), v.ptr<ভিন্ন>(), v.step, n, ptr) :
        Jacobi(a.ptr<দ্ৱিগুণ>(), a.step, w.ptr<দ্ৱিগুণ>(), v.ptr<দ্ৱিগুণ>(), v.step, n, ptr);

    w.copyTo(_evals);
    ঘূৰাই ok;
#endif
}

স্থির শূন্য _SVDcompute( প্ৰবেশ_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, পূর্ণাঙ্ক flags )
{
    Mat src = _aarr.getMat();
    পূর্ণাঙ্ক m = src.rows, n = src.cols;
    পূর্ণাঙ্ক type = src.type();
    তার্কিক compute_uv = _u.needed() || _vt.needed();
    তার্কিক full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    যদি( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = মিছা;
    }

    তার্কিক at = মিছা;
    যদি( m < n )
    {
        std::swap(m, n);
        at = সঁচা;
    }

    পূর্ণাঙ্ক urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    যদি( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    যদি( urows > n )
        temp_u = Scalar::all(0);

    যদি( !at )
        transpose(src, temp_a);
    অন্যথা
        src.copyTo(temp_a);

    যদি( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<ভিন্ন>(), temp_u.step, temp_w.ptr<ভিন্ন>(),
              temp_v.ptr<ভিন্ন>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    অন্যথা
    {
        JacobiSVD(temp_a.ptr<দ্ৱিগুণ>(), temp_u.step, temp_w.ptr<দ্ৱিগুণ>(),
              temp_v.ptr<দ্ৱিগুণ>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    যদি( compute_uv )
    {
        যদি( !at )
        {
            যদি( _u.needed() )
                transpose(temp_u, _u);
            যদি( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        অন্যথা
        {
            যদি( _u.needed() )
                transpose(temp_v, _u);
            যদি( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


শূন্য SVD::compute( প্ৰবেশ_Array a, OutputArray w, OutputArray u, OutputArray vt, পূর্ণাঙ্ক flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

শূন্য SVD::compute( প্ৰবেশ_Array a, OutputArray w, পূর্ণাঙ্ক flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

শূন্য SVD::backSubst( প্ৰবেশ_Array _w, প্ৰবেশ_Array _u, প্ৰবেশ_Array _vt,
                     প্ৰবেশ_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    পূর্ণাঙ্ক type = w.type(), esz = (পূর্ণাঙ্ক)w.elemSize();
    পূর্ণাঙ্ক m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*পরিমাণ(দ্ৱিগুণ) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    যদি( type == CV_32F )
        SVBkSb(m, n, w.ptr<ভিন্ন>(), wstep, u.ptr<ভিন্ন>(), u.step, মিছা,
               vt.ptr<ভিন্ন>(), vt.step, সঁচা, rhs.ptr<ভিন্ন>(), rhs.step, nb,
               dst.ptr<ভিন্ন>(), dst.step, buffer.data());
    অন্যথা যদি( type == CV_64F )
        SVBkSb(m, n, w.ptr<দ্ৱিগুণ>(), wstep, u.ptr<দ্ৱিগুণ>(), u.step, মিছা,
               vt.ptr<দ্ৱিগুণ>(), vt.step, সঁচা, rhs.ptr<দ্ৱিগুণ>(), rhs.step, nb,
               dst.ptr<দ্ৱিগুণ>(), dst.step, buffer.data());
    অন্যথা
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(প্ৰবেশ_Array a, পূর্ণাঙ্ক flags)
{
    _SVDcompute(a, w, u, vt, flags);
    ঘূৰাই *এতত্;
}


শূন্য SVD::backSubst( প্ৰবেশ_Array rhs, OutputArray dst ) ধ্রুৱ
{
    backSubst( w, u, vt, rhs, dst );
}

}


শূন্য cv::SVDecomp(প্ৰবেশ_Array src, OutputArray w, OutputArray u, OutputArray vt, পূর্ণাঙ্ক flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

শূন্য cv::SVBackSubst(প্ৰবেশ_Array w, প্ৰবেশ_Array u, প্ৰবেশ_Array vt, প্ৰবেশ_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
