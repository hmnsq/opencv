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

#شامل "precomp.hpp"
#شامل <limits>

#ifdef HAVE_EIGEN
#  اگر defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  اگر defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

فضاینام cv
{

صحیح LU(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    بازگشت hal::LU32f(A, astep, m, b, bstep, n);
}

صحیح LU(دوگانه* A, size_t astep, صحیح m, دوگانه* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    بازگشت hal::LU64f(A, astep, m, b, bstep, n);
}

منطقی Cholesky(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    بازگشت hal::Cholesky32f(A, astep, m, b, bstep, n);
}

منطقی Cholesky(دوگانه* A, size_t astep, صحیح m, دوگانه* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    بازگشت hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> ایستا درونخطی _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    اگر( a > b )
    {
        b /= a;
        بازگشت a*std::sqrt(1 + b*b);
    }
    اگر( b > 0 )
    {
        a /= b;
        بازگشت b*std::sqrt(1 + a*a);
    }
    بازگشت 0;
}


template<typename _Tp> منطقی
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, صحیح n, uchar* buf )
{
    ثابت _Tp eps = std::numeric_limits<_Tp>::epsilon();
    صحیح i, j, k, m;

    astep /= اندازه(A[0]);
    اگر( V )
    {
        vstep /= اندازه(V[0]);
        برای( i = 0; i < n; i++ )
        {
            برای( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    صحیح iters, maxIters = n*n*30;

    صحیح* indR = (صحیح*)alignPtr(buf, اندازه(صحیح));
    صحیح* indC = indR + n;
    _Tp mv = (_Tp)0;

    برای( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        اگر( k < n - 1 )
        {
            برای( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                اگر( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        اگر( k > 0 )
        {
            برای( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                اگر( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    اگر( n > 1 ) برای( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        برای( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            اگر( mv < val )
                mv = val, k = i;
        }
        صحیح l = indR[k];
        برای( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            اگر( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        اگر( std::abs(p) <= eps )
            بشکن;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        اگر( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        برای( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        برای( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        برای( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        اگر( V )
            برای( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        برای( j = 0; j < 2; j++ )
        {
            صحیح idx = j == 0 ? k : l;
            اگر( idx < n - 1 )
            {
                برای( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    اگر( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            اگر( idx > 0 )
            {
                برای( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    اگر( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    برای( k = 0; k < n-1; k++ )
    {
        m = k;
        برای( i = k+1; i < n; i++ )
        {
            اگر( W[m] < W[i] )
                m = i;
        }
        اگر( k != m )
        {
            std::swap(W[m], W[k]);
            اگر( V )
                برای( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    بازگشت درست;
}

ایستا منطقی Jacobi( اعشاری* S, size_t sstep, اعشاری* e, اعشاری* E, size_t estep, صحیح n, uchar* buf )
{
    بازگشت JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

ایستا منطقی Jacobi( دوگانه* S, size_t sstep, دوگانه* e, دوگانه* E, size_t estep, صحیح n, uchar* buf )
{
    بازگشت JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> ساختار VBLAS
{
    صحیح dot(ثابت T*, ثابت T*, صحیح, T*) ثابت { بازگشت 0; }
    صحیح givens(T*, T*, صحیح, T, T) ثابت { بازگشت 0; }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
template<> درونخطی صحیح VBLAS<اعشاری>::dot(ثابت اعشاری* a, ثابت اعشاری* b, صحیح n, اعشاری* result) ثابت
{
    اگر( n < 2*VTraits<v_float32>::vlanes() )
        بازگشت 0;
    صحیح k = 0;
    v_float32 s0 = vx_setzero_f32();
    برای( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    بازگشت k;
}


template<> درونخطی صحیح VBLAS<اعشاری>::givens(اعشاری* a, اعشاری* b, صحیح n, اعشاری c, اعشاری s) ثابت
{
    اگر( n < VTraits<v_float32>::vlanes())
        بازگشت 0;
    صحیح k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    برای( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    بازگشت k;
}


#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> درونخطی صحیح VBLAS<دوگانه>::dot(ثابت دوگانه* a, ثابت دوگانه* b, صحیح n, دوگانه* result) ثابت
{
    اگر( n < 2*VTraits<v_float64>::vlanes() )
        بازگشت 0;
    صحیح k = 0;
    v_float64 s0 = vx_setzero_f64();
    برای( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    بازگشت k;
}


template<> درونخطی صحیح VBLAS<دوگانه>::givens(دوگانه* a, دوگانه* b, صحیح n, دوگانه c, دوگانه s) ثابت
{
    صحیح k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    برای( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    بازگشت k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> تهی
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               صحیح m, صحیح n, صحیح n1, دوگانه minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<دوگانه> Wbuf(n);
    دوگانه* W = Wbuf.data();
    صحیح i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    دوگانه sd;
    astep /= اندازه(At[0]);
    vstep /= اندازه(Vt[0]);

    برای( i = 0; i < n; i++ )
    {
        برای( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (دوگانه)t*t;
        }
        W[i] = sd;

        اگر( Vt )
        {
            برای( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    برای( iter = 0; iter < max_iter; iter++ )
    {
        منطقی changed = نادرست;

        برای( i = 0; i < n-1; i++ )
            برای( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                دوگانه a = W[i], p = 0, b = W[j];

                برای( k = 0; k < m; k++ )
                    p += (دوگانه)Ai[k]*Aj[k];

                اگر( std::abs(p) <= eps*std::sqrt((دوگانه)a*b) )
                    ادامه;

                p *= 2;
                دوگانه beta = a - b, gamma = hypot((دوگانه)p, beta);
                اگر( beta < 0 )
                {
                    دوگانه delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                وگرنه
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                برای( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (دوگانه)t0*t0; b += (دوگانه)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = درست;

                اگر( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    برای( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        اگر( !changed )
            بشکن;
    }

    برای( i = 0; i < n; i++ )
    {
        برای( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (دوگانه)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    برای( i = 0; i < n-1; i++ )
    {
        j = i;
        برای( k = i+1; k < n; k++ )
        {
            اگر( W[j] < W[k] )
                j = k;
        }
        اگر( i != j )
        {
            std::swap(W[i], W[j]);
            اگر( Vt )
            {
                برای( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                برای( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    برای( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    اگر( !Vt )
        بازگشت;

    RNG rng(0x12345678);
    برای( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        برای( صحیح ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ثابت _Tp val0 = (_Tp)(1./m);
            برای( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            برای( iter = 0; iter < 2; iter++ )
            {
                برای( j = 0; j < i; j++ )
                {
                    sd = 0;
                    برای( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    برای( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    برای( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            برای( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (دوگانه)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        برای( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


ایستا تهی JacobiSVD(اعشاری* At, size_t astep, اعشاری* W, اعشاری* Vt, size_t vstep, صحیح m, صحیح n, صحیح n1=-1)
{
    hal::SVD32f(At, astep, W, پوچ, astep, Vt, vstep, m, n, n1);
}

ایستا تهی JacobiSVD(دوگانه* At, size_t astep, دوگانه* W, دوگانه* Vt, size_t vstep, صحیح m, صحیح n, صحیح n1=-1)
{
    hal::SVD64f(At, astep, W, پوچ, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> ایستا درونخطی صحیح
decodeSVDParameters(ثابت fptype* U, ثابت fptype* Vt, صحیح m, صحیح n, صحیح n1)
{
    صحیح halSVDFlag = 0;
    اگر(Vt == پوچ)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    وگرنه اگر(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        اگر(U == پوچ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    وگرنه اگر(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        اگر(U == پوچ)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    بازگشت halSVDFlag;
}

تهی hal::SVD32f(اعشاری* At, size_t astep, اعشاری* W, اعشاری* U, size_t ustep, اعشاری* Vt, size_t vstep, صحیح m, صحیح n, صحیح n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

تهی hal::SVD64f(دوگانه* At, size_t astep, دوگانه* W, دوگانه* U, size_t ustep, دوگانه* Vt, size_t vstep, صحیح m, صحیح n, صحیح n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> ایستا تهی
MatrAXPY( صحیح m, صحیح n, ثابت T1* x, صحیح dx,
         ثابت T2* a, صحیح inca, T3* y, صحیح dy )
{
    صحیح i;
    برای( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        صحیح j = 0;
         #اگر CV_ENABLE_UNROLLED
        برای(; j <= n - 4; j += 4 )
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
        برای( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> ایستا تهی
SVBkSbImpl_( صحیح m, صحیح n, ثابت T* w, صحیح incw,
       ثابت T* u, صحیح ldu, منطقی uT,
       ثابت T* v, صحیح ldv, منطقی vT,
       ثابت T* b, صحیح ldb, صحیح nb,
       T* x, صحیح ldx, دوگانه* buffer, T eps )
{
    دوگانه threshold = 0;
    صحیح udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    صحیح vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    صحیح i, j, nm = std::min(m, n);

    اگر( !b )
        nb = m;

    برای( i = 0; i < n; i++ )
        برای( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    برای( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    برای( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        دوگانه wi = w[i*incw];
        اگر( (دوگانه)std::abs(wi) <= threshold )
            ادامه;
        wi = 1/wi;

        اگر( nb == 1 )
        {
            دوگانه s = 0;
            اگر( b )
                برای( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            وگرنه
                s = u[0];
            s *= wi;

            برای( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        وگرنه
        {
            اگر( b )
            {
                برای( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                برای( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            وگرنه
            {
                برای( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

ایستا تهی
SVBkSb( صحیح m, صحیح n, ثابت اعشاری* w, size_t wstep,
        ثابت اعشاری* u, size_t ustep, منطقی uT,
        ثابت اعشاری* v, size_t vstep, منطقی vT,
        ثابت اعشاری* b, size_t bstep, صحیح nb,
        اعشاری* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (صحیح)(wstep/اندازه(w[0])) : 1,
                u, (صحیح)(ustep/اندازه(u[0])), uT,
                v, (صحیح)(vstep/اندازه(v[0])), vT,
                b, (صحیح)(bstep/اندازه(b[0])), nb,
                x, (صحیح)(xstep/اندازه(x[0])),
                (دوگانه*)alignPtr(buffer, اندازه(دوگانه)), (اعشاری)(DBL_EPSILON*2) );
}

ایستا تهی
SVBkSb( صحیح m, صحیح n, ثابت دوگانه* w, size_t wstep,
       ثابت دوگانه* u, size_t ustep, منطقی uT,
       ثابت دوگانه* v, size_t vstep, منطقی vT,
       ثابت دوگانه* b, size_t bstep, صحیح nb,
       دوگانه* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (صحیح)(wstep/اندازه(w[0])) : 1,
                u, (صحیح)(ustep/اندازه(u[0])), uT,
                v, (صحیح)(vstep/اندازه(v[0])), vT,
                b, (صحیح)(bstep/اندازه(b[0])), nb,
                x, (صحیح)(xstep/اندازه(x[0])),
                (دوگانه*)alignPtr(buffer, اندازه(دوگانه)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((دوگانه)m(0,0)*m(1,1) - (دوگانه)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((دوگانه)m(1,1)*m(2,2) - (دوگانه)m(1,2)*m(2,1)) -  \
                   m(0,1)*((دوگانه)m(1,0)*m(2,2) - (دوگانه)m(1,2)*m(2,0)) +  \
                   m(0,2)*((دوگانه)m(1,0)*m(2,1) - (دوگانه)m(1,1)*m(2,0)))

دوگانه determinant( درون_داد_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    دوگانه result = 0;
    صحیح type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    ثابت uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((اعشاری*)(m + y*step))[x]
    #define Md(y, x) ((دوگانه*)(m + y*step))[x]

    اگر( type == CV_32F )
    {
        اگر( rows == 2 )
            result = det2(Mf);
        وگرنه اگر( rows == 3 )
            result = det3(Mf);
        وگرنه اگر( rows == 1 )
            result = Mf(0,0);
        وگرنه
        {
            size_t bufSize = rows*rows*اندازه(اعشاری);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<اعشاری>(), a.step, rows, 0, 0, 0);
            اگر( result )
            {
                برای( صحیح i = 0; i < rows; i++ )
                    result *= a.at<اعشاری>(i,i);
            }
        }
    }
    وگرنه
    {
        اگر( rows == 2 )
            result = det2(Md);
        وگرنه اگر( rows == 3 )
            result = det3(Md);
        وگرنه اگر( rows == 1 )
            result = Md(0,0);
        وگرنه
        {
            size_t bufSize = rows*rows*اندازه(دوگانه);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<دوگانه>(), a.step, rows, 0, 0, 0);
            اگر( result )
            {
                برای( صحیح i = 0; i < rows; i++ )
                    result *= a.at<دوگانه>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    بازگشت result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((اعشاری*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((دوگانه*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((اعشاری*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((دوگانه*)(dstdata + y*dststep))[x]

دوگانه invert( درون_داد_Array _src, OutputArray _dst, صحیح method )
{
    CV_INSTRUMENT_REGION();

    منطقی result = نادرست;
    Mat src = _src.getMat();
    صحیح type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    صحیح m = src.rows, n = src.cols;

    اگر( method == DECOMP_SVD )
    {
        صحیح nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + اندازه(دوگانه));
        uchar* buf = alignPtr((uchar*)_buf.data(), (صحیح)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        بازگشت type == CV_32F ?
            (w.ptr<اعشاری>()[0] >= FLT_EPSILON ?
             w.ptr<اعشاری>()[n-1]/w.ptr<اعشاری>()[0] : 0) :
            (w.ptr<دوگانه>()[0] >= DBL_EPSILON ?
             w.ptr<دوگانه>()[n-1]/w.ptr<دوگانه>()[0] : 0);
    }

    CV_Assert( m == n );

    اگر( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + اندازه(دوگانه));
        uchar* buf = alignPtr((uchar*)_buf.data(), (صحیح)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        بازگشت type == CV_32F ?
        (w.ptr<اعشاری>()[0] >= FLT_EPSILON ?
         w.ptr<اعشاری>()[n-1]/w.ptr<اعشاری>()[0] : 0) :
        (w.ptr<دوگانه>()[0] >= DBL_EPSILON ?
         w.ptr<دوگانه>()[n-1]/w.ptr<دوگانه>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    اگر( n <= 3 )
    {
        ثابت uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        اگر( n == 2 )
        {
            اگر( type == CV_32FC1 )
            {
                دوگانه d = det2(Sf);
                اگر( d != 0. )
                {
                    result = درست;
                    d = 1./d;
                #اگر CV_SIMD128
                    ثابت اعشاری d_32f = (اعشاری)d;
                    ثابت v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((ثابت اعشاری *)srcdata, (ثابت اعشاری *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_کم(v_rotate_راست<1>(s0), s0));
                    v_store_کم((اعشاری*)dstdata, s0);
                    v_store_بالا((اعشاری*)(dstdata + dststep), s0);
                #وگرنه
                    دوگانه t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (اعشاری)t0;
                    Df(0,0) = (اعشاری)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (اعشاری)t0;
                    Df(1,0) = (اعشاری)t1;
                #endif
                }
            }
            وگرنه
            {
                دوگانه d = det2(Sd);
                اگر( d != 0. )
                {
                    result = درست;
                    d = 1./d;
                #اگر CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((ثابت دوگانه *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((ثابت دوگانه *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<دوگانه>(0), v_extract<1>(s0, s1));//12
                    v_store((دوگانه*)dstdata, v_combine_کم(sm, ss));//31
                    v_store((دوگانه*)(dstdata + dststep), v_combine_بالا(ss, sm));//20
                #وگرنه
                    دوگانه t0, t1;
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
        وگرنه اگر( n == 3 )
        {
            اگر( type == CV_32FC1 )
            {
                دوگانه d = det3(Sf);

                اگر( d != 0. )
                {
                    دوگانه t[12];

                    result = درست;
                    d = 1./d;
                    t[0] = (((دوگانه)Sf(1,1) * Sf(2,2) - (دوگانه)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((دوگانه)Sf(0,2) * Sf(2,1) - (دوگانه)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((دوگانه)Sf(0,1) * Sf(1,2) - (دوگانه)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((دوگانه)Sf(1,2) * Sf(2,0) - (دوگانه)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((دوگانه)Sf(0,0) * Sf(2,2) - (دوگانه)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((دوگانه)Sf(0,2) * Sf(1,0) - (دوگانه)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((دوگانه)Sf(1,0) * Sf(2,1) - (دوگانه)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((دوگانه)Sf(0,1) * Sf(2,0) - (دوگانه)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((دوگانه)Sf(0,0) * Sf(1,1) - (دوگانه)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (اعشاری)t[0]; Df(0,1) = (اعشاری)t[1]; Df(0,2) = (اعشاری)t[2];
                    Df(1,0) = (اعشاری)t[3]; Df(1,1) = (اعشاری)t[4]; Df(1,2) = (اعشاری)t[5];
                    Df(2,0) = (اعشاری)t[6]; Df(2,1) = (اعشاری)t[7]; Df(2,2) = (اعشاری)t[8];
                }
            }
            وگرنه
            {
                دوگانه d = det3(Sd);
                اگر( d != 0. )
                {
                    result = درست;
                    d = 1./d;
                    دوگانه t[9];

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
        وگرنه
        {
            CV_Assert( n == 1 );

            اگر( type == CV_32FC1 )
            {
                دوگانه d = Sf(0,0);
                اگر( d != 0. )
                {
                    result = درست;
                    Df(0,0) = (اعشاری)(1./d);
                }
            }
            وگرنه
            {
                دوگانه d = Sd(0,0);
                اگر( d != 0. )
                {
                    result = درست;
                    Dd(0,0) = 1./d;
                }
            }
        }
        اگر( !result )
            dst = Scalar(0);
        بازگشت result;
    }

   صحیح elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    اگر( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<اعشاری>(), src1.step, n, dst.ptr<اعشاری>(), dst.step, n) != 0;
    وگرنه اگر( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<دوگانه>(), src1.step, n, dst.ptr<دوگانه>(), dst.step, n) != 0;
    وگرنه اگر( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<اعشاری>(), src1.step, n, dst.ptr<اعشاری>(), dst.step, n);
    وگرنه
        result = hal::Cholesky64f(src1.ptr<دوگانه>(), src1.step, n, dst.ptr<دوگانه>(), dst.step, n);

    اگر( !result )
        dst = Scalar(0);

    بازگشت result;
}

UMat UMat::inv(صحیح method) ثابت
{
    UMat m;
    invert(*این, m, method);
    بازگشت m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

منطقی solve( درون_داد_Array _src, درون_داد_Array _src2arg, OutputArray _dst, صحیح method )
{
    CV_INSTRUMENT_REGION();

    منطقی result = درست;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    صحیح type = src.type();
    منطقی is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    اگر( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((اعشاری*)(bdata + y*src2step))[0]
        #define bd(y) ((دوگانه*)(bdata + y*src2step))[0]

        ثابت uchar* srcdata = src.ptr();
        ثابت uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        اگر( src.rows == 2 )
        {
            اگر( type == CV_32FC1 )
            {
                دوگانه d = det2(Sf);
                اگر( d != 0. )
                {
                    دوگانه t;
                    d = 1./d;
                    t = (اعشاری)(((دوگانه)bf(0)*Sf(1,1) - (دوگانه)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (اعشاری)(((دوگانه)bf(1)*Sf(0,0) - (دوگانه)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (اعشاری)t;
                }
                وگرنه
                    result = نادرست;
            }
            وگرنه
            {
                دوگانه d = det2(Sd);
                اگر( d != 0. )
                {
                    دوگانه t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                وگرنه
                    result = نادرست;
            }
        }
        وگرنه اگر( src.rows == 3 )
        {
            اگر( type == CV_32FC1 )
            {
                دوگانه d = det3(Sf);
                اگر( d != 0. )
                {
                    اعشاری t[3];
                    d = 1./d;

                    t[0] = (اعشاری)(d*
                           (bf(0)*((دوگانه)Sf(1,1)*Sf(2,2) - (دوگانه)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((دوگانه)bf(1)*Sf(2,2) - (دوگانه)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((دوگانه)bf(1)*Sf(2,1) - (دوگانه)Sf(1,1)*bf(2))));

                    t[1] = (اعشاری)(d*
                           (Sf(0,0)*(دوگانه)(bf(1)*Sf(2,2) - (دوگانه)Sf(1,2)*bf(2)) -
                            bf(0)*((دوگانه)Sf(1,0)*Sf(2,2) - (دوگانه)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((دوگانه)Sf(1,0)*bf(2) - (دوگانه)bf(1)*Sf(2,0))));

                    t[2] = (اعشاری)(d*
                           (Sf(0,0)*((دوگانه)Sf(1,1)*bf(2) - (دوگانه)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((دوگانه)Sf(1,0)*bf(2) - (دوگانه)bf(1)*Sf(2,0)) +
                            bf(0)*((دوگانه)Sf(1,0)*Sf(2,1) - (دوگانه)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                وگرنه
                    result = نادرست;
            }
            وگرنه
            {
                دوگانه d = det3(Sd);
                اگر( d != 0. )
                {
                    دوگانه t[9];

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
                وگرنه
                    result = نادرست;
            }
        }
        وگرنه
        {
            CV_Assert( src.rows == 1 );

            اگر( type == CV_32FC1 )
            {
                دوگانه d = Sf(0,0);
                اگر( d != 0. )
                    Df(0,0) = (اعشاری)(bf(0)/d);
                وگرنه
                    result = نادرست;
            }
            وگرنه
            {
                دوگانه d = Sd(0,0);
                اگر( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                وگرنه
                    result = نادرست;
            }
        }
        بازگشت result;
    }

    صحیح m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    اگر( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    اگر( m == n )
        is_normal = نادرست;
    وگرنه اگر( is_normal )
    {
        m_ = n;
        اگر( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    اگر( is_normal )
        bufsize += n*nb*esz;
    اگر( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*اندازه(دوگانه) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    اگر( is_normal )
        mulTransposed(src, a, درست);
    وگرنه اگر( method != DECOMP_SVD )
        src.copyTo(a);
    وگرنه
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    اگر( !is_normal )
    {
        اگر( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    وگرنه
    {
        // a'*b
        اگر( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        وگرنه
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    اگر( method == DECOMP_LU )
    {
        اگر( type == CV_32F )
            result = hal::LU32f(a.ptr<اعشاری>(), a.step, n, dst.ptr<اعشاری>(), dst.step, nb) != 0;
        وگرنه
            result = hal::LU64f(a.ptr<دوگانه>(), a.step, n, dst.ptr<دوگانه>(), dst.step, nb) != 0;
    }
    وگرنه اگر( method == DECOMP_CHOLESKY )
    {
        اگر( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<اعشاری>(), a.step, n, dst.ptr<اعشاری>(), dst.step, nb);
        وگرنه
            result = hal::Cholesky64f(a.ptr<دوگانه>(), a.step, n, dst.ptr<دوگانه>(), dst.step, nb);
    }
    وگرنه اگر( method == DECOMP_QR )
    {
        Mat rhsMat;
        اگر( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        وگرنه
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        اگر( type == CV_32F )
            result = hal::QR32f(a.ptr<اعشاری>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<اعشاری>(), rhsMat.step, پوچ) != 0;
        وگرنه
            result = hal::QR64f(a.ptr<دوگانه>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<دوگانه>(), rhsMat.step, پوچ) != 0;

        اگر (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    وگرنه
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        اگر( method == DECOMP_EIG )
        {
            اگر( type == CV_32F )
                Jacobi(a.ptr<اعشاری>(), a.step, w.ptr<اعشاری>(), v.ptr<اعشاری>(), v.step, n, ptr);
            وگرنه
                Jacobi(a.ptr<دوگانه>(), a.step, w.ptr<دوگانه>(), v.ptr<دوگانه>(), v.step, n, ptr);
            u = v;
        }
        وگرنه
        {
            اگر( type == CV_32F )
                JacobiSVD(a.ptr<اعشاری>(), a.step, w.ptr<اعشاری>(), v.ptr<اعشاری>(), v.step, m_, n);
            وگرنه
                JacobiSVD(a.ptr<دوگانه>(), a.step, w.ptr<دوگانه>(), v.ptr<دوگانه>(), v.step, m_, n);
            u = a;
        }

        اگر( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<اعشاری>(), 0, u.ptr<اعشاری>(), u.step, درست,
                   v.ptr<اعشاری>(), v.step, درست, src2.ptr<اعشاری>(),
                   src2.step, nb, dst.ptr<اعشاری>(), dst.step, ptr);
        }
        وگرنه
        {
            SVBkSb(m_, n, w.ptr<دوگانه>(), 0, u.ptr<دوگانه>(), u.step, درست,
                   v.ptr<دوگانه>(), v.step, درست, src2.ptr<دوگانه>(),
                   src2.step, nb, dst.ptr<دوگانه>(), dst.step, ptr);
        }
        result = درست;
    }

    اگر( !result )
        dst = Scalar(0);

    بازگشت result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

منطقی eigen( درون_داد_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    صحیح type = src.type();
    صحیح n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    اگر( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    ثابت منطقی evecNeeded = _evects.needed();
    ثابت صحیح esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    اگر ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        اگر ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            اگر ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            بازگشت درست;
        }
    } وگرنه { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        اگر ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            اگر ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            بازگشت درست;
        }
    }
    بازگشت نادرست;
#وگرنه

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    منطقی ok = type == CV_32F ?
        Jacobi(a.ptr<اعشاری>(), a.step, w.ptr<اعشاری>(), v.ptr<اعشاری>(), v.step, n, ptr) :
        Jacobi(a.ptr<دوگانه>(), a.step, w.ptr<دوگانه>(), v.ptr<دوگانه>(), v.step, n, ptr);

    w.copyTo(_evals);
    بازگشت ok;
#endif
}

ایستا تهی _SVDcompute( درون_داد_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, صحیح flags )
{
    Mat src = _aarr.getMat();
    صحیح m = src.rows, n = src.cols;
    صحیح type = src.type();
    منطقی compute_uv = _u.needed() || _vt.needed();
    منطقی full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    اگر( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = نادرست;
    }

    منطقی at = نادرست;
    اگر( m < n )
    {
        std::swap(m, n);
        at = درست;
    }

    صحیح urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    اگر( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    اگر( urows > n )
        temp_u = Scalar::all(0);

    اگر( !at )
        transpose(src, temp_a);
    وگرنه
        src.copyTo(temp_a);

    اگر( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<اعشاری>(), temp_u.step, temp_w.ptr<اعشاری>(),
              temp_v.ptr<اعشاری>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    وگرنه
    {
        JacobiSVD(temp_a.ptr<دوگانه>(), temp_u.step, temp_w.ptr<دوگانه>(),
              temp_v.ptr<دوگانه>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    اگر( compute_uv )
    {
        اگر( !at )
        {
            اگر( _u.needed() )
                transpose(temp_u, _u);
            اگر( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        وگرنه
        {
            اگر( _u.needed() )
                transpose(temp_v, _u);
            اگر( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


تهی SVD::compute( درون_داد_Array a, OutputArray w, OutputArray u, OutputArray vt, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

تهی SVD::compute( درون_داد_Array a, OutputArray w, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

تهی SVD::backSubst( درون_داد_Array _w, درون_داد_Array _u, درون_داد_Array _vt,
                     درون_داد_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    صحیح type = w.type(), esz = (صحیح)w.elemSize();
    صحیح m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*اندازه(دوگانه) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    اگر( type == CV_32F )
        SVBkSb(m, n, w.ptr<اعشاری>(), wstep, u.ptr<اعشاری>(), u.step, نادرست,
               vt.ptr<اعشاری>(), vt.step, درست, rhs.ptr<اعشاری>(), rhs.step, nb,
               dst.ptr<اعشاری>(), dst.step, buffer.data());
    وگرنه اگر( type == CV_64F )
        SVBkSb(m, n, w.ptr<دوگانه>(), wstep, u.ptr<دوگانه>(), u.step, نادرست,
               vt.ptr<دوگانه>(), vt.step, درست, rhs.ptr<دوگانه>(), rhs.step, nb,
               dst.ptr<دوگانه>(), dst.step, buffer.data());
    وگرنه
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(درون_داد_Array a, صحیح flags)
{
    _SVDcompute(a, w, u, vt, flags);
    بازگشت *این;
}


تهی SVD::backSubst( درون_داد_Array rhs, OutputArray dst ) ثابت
{
    backSubst( w, u, vt, rhs, dst );
}

}


تهی cv::SVDecomp(درون_داد_Array src, OutputArray w, OutputArray u, OutputArray vt, صحیح flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

تهی cv::SVBackSubst(درون_داد_Array w, درون_داد_Array u, درون_داد_Array vt, درون_داد_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
