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

#اگر_تعریف_شدہ HAVE_EIGEN
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
#اگر_ختم

نامخلا cv
{

صحیح LU(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    واپس hal::LU32f(A, astep, m, b, bstep, n);
}

صحیح LU(دوہرا* A, size_t astep, صحیح m, دوہرا* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    واپس hal::LU64f(A, astep, m, b, bstep, n);
}

منطقی Cholesky(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    واپس hal::Cholesky32f(A, astep, m, b, bstep, n);
}

منطقی Cholesky(دوہرا* A, size_t astep, صحیح m, دوہرا* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    واپس hal::Cholesky64f(A, astep, m, b, bstep, n);
}

سانچہ<قسم_نام _Tp> ساکن اندرونی _Tp hypot(_Tp a, _Tp b)
{
    a = std::مطلق_قدر(a);
    b = std::مطلق_قدر(b);
    اگر( a > b )
    {
        b /= a;
        واپس a*std::جذر(1 + b*b);
    }
    اگر( b > 0 )
    {
        a /= b;
        واپس b*std::جذر(1 + a*a);
    }
    واپس 0;
}


سانچہ<قسم_نام _Tp> منطقی
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, صحیح n, uchar* buf )
{
    ثابت _Tp eps = std::numeric_limits<_Tp>::epsilon();
    صحیح i, j, k, m;

    astep /= حجم(A[0]);
    اگر( V )
    {
        vstep /= حجم(V[0]);
        برائے( i = 0; i < n; i++ )
        {
            برائے( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    صحیح iters, maxIters = n*n*30;

    صحیح* indR = (صحیح*)alignPtr(buf, حجم(صحیح));
    صحیح* indC = indR + n;
    _Tp mv = (_Tp)0;

    برائے( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        اگر( k < n - 1 )
        {
            برائے( m = k+1, mv = std::مطلق_قدر(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::مطلق_قدر(A[astep*k+i]);
                اگر( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        اگر( k > 0 )
        {
            برائے( m = 0, mv = std::مطلق_قدر(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::مطلق_قدر(A[astep*i+k]);
                اگر( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    اگر( n > 1 ) برائے( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        برائے( k = 0, mv = std::مطلق_قدر(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::مطلق_قدر(A[astep*i + indR[i]]);
            اگر( mv < val )
                mv = val, k = i;
        }
        صحیح l = indR[k];
        برائے( i = 1; i < n; i++ )
        {
            _Tp val = std::مطلق_قدر(A[astep*indC[i] + i]);
            اگر( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        اگر( std::مطلق_قدر(p) <= eps )
            توڑو;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::مطلق_قدر(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        اگر( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#تعریف_منسوخ rotate
#تعریف_کرو rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        برائے( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        برائے( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        برائے( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        اگر( V )
            برائے( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#تعریف_منسوخ rotate

        برائے( j = 0; j < 2; j++ )
        {
            صحیح idx = j == 0 ? k : l;
            اگر( idx < n - 1 )
            {
                برائے( m = idx+1, mv = std::مطلق_قدر(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::مطلق_قدر(A[astep*idx+i]);
                    اگر( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            اگر( idx > 0 )
            {
                برائے( m = 0, mv = std::مطلق_قدر(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::مطلق_قدر(A[astep*i+idx]);
                    اگر( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    برائے( k = 0; k < n-1; k++ )
    {
        m = k;
        برائے( i = k+1; i < n; i++ )
        {
            اگر( W[m] < W[i] )
                m = i;
        }
        اگر( k != m )
        {
            std::swap(W[m], W[k]);
            اگر( V )
                برائے( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    واپس سچ;
}

ساکن منطقی Jacobi( اعشاری* S, size_t sstep, اعشاری* e, اعشاری* E, size_t estep, صحیح n, uchar* buf )
{
    واپس JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

ساکن منطقی Jacobi( دوہرا* S, size_t sstep, دوہرا* e, دوہرا* E, size_t estep, صحیح n, uchar* buf )
{
    واپس JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


سانچہ<قسم_نام T> ساخت VBLAS
{
    صحیح dot(ثابت T*, ثابت T*, صحیح, T*) ثابت { واپس 0; }
    صحیح givens(T*, T*, صحیح, T, T) ثابت { واپس 0; }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
سانچہ<> اندرونی صحیح VBLAS<اعشاری>::dot(ثابت اعشاری* a, ثابت اعشاری* b, صحیح n, اعشاری* نتیجہ) ثابت
{
    اگر( n < 2*VTraits<v_float32>::vlanes() )
        واپس 0;
    صحیح k = 0;
    v_float32 s0 = vx_setzero_f32();
    برائے( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_بھرو(a + k);
        v_float32 b0 = vx_بھرو(b + k);

        s0 = v_جوڑو(s0, v_mul(a0, b0));
    }
    *نتیجہ = v_reduce_مجموع(s0);
    vx_cleanup();
    واپس k;
}


سانچہ<> اندرونی صحیح VBLAS<اعشاری>::givens(اعشاری* a, اعشاری* b, صحیح n, اعشاری c, اعشاری s) ثابت
{
    اگر( n < VTraits<v_float32>::vlanes())
        واپس 0;
    صحیح k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    برائے( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_بھرو(a + k);
        v_float32 b0 = vx_بھرو(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    واپس k;
}


#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
سانچہ<> اندرونی صحیح VBLAS<دوہرا>::dot(ثابت دوہرا* a, ثابت دوہرا* b, صحیح n, دوہرا* نتیجہ) ثابت
{
    اگر( n < 2*VTraits<v_float64>::vlanes() )
        واپس 0;
    صحیح k = 0;
    v_float64 s0 = vx_setzero_f64();
    برائے( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_بھرو(a + k);
        v_float64 b0 = vx_بھرو(b + k);

        s0 = v_جوڑو(s0, v_mul(a0, b0));
    }
    *نتیجہ = v_reduce_مجموع(s0);
    vx_cleanup();
    واپس k;
}


سانچہ<> اندرونی صحیح VBLAS<دوہرا>::givens(دوہرا* a, دوہرا* b, صحیح n, دوہرا c, دوہرا s) ثابت
{
    صحیح k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    برائے( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_بھرو(a + k);
        v_float64 b0 = vx_بھرو(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    واپس k;
}


#اگر_ختم // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#اگر_ختم // (CV_SIMD || CV_SIMD_SCALABLE)

سانچہ<قسم_نام _Tp> خالی
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               صحیح m, صحیح n, صحیح n1, دوہرا minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    Auto_بفر<دوہرا> Wbuf(n);
    دوہرا* W = Wbuf.ڈیٹا();
    صحیح i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    دوہرا sd;
    astep /= حجم(At[0]);
    vstep /= حجم(Vt[0]);

    برائے( i = 0; i < n; i++ )
    {
        برائے( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (دوہرا)t*t;
        }
        W[i] = sd;

        اگر( Vt )
        {
            برائے( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    برائے( iter = 0; iter < max_iter; iter++ )
    {
        منطقی بدلا = جھوٹ;

        برائے( i = 0; i < n-1; i++ )
            برائے( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                دوہرا a = W[i], p = 0, b = W[j];

                برائے( k = 0; k < m; k++ )
                    p += (دوہرا)Ai[k]*Aj[k];

                اگر( std::مطلق_قدر(p) <= eps*std::جذر((دوہرا)a*b) )
                    جاری;

                p *= 2;
                دوہرا beta = a - b, gamma = hypot((دوہرا)p, beta);
                اگر( beta < 0 )
                {
                    دوہرا delta = (gamma - beta)*0.5;
                    s = (_Tp)std::جذر(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                ورنہ
                {
                    c = (_Tp)std::جذر((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                برائے( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (دوہرا)t0*t0; b += (دوہرا)t1*t1;
                }
                W[i] = a; W[j] = b;

                بدلا = سچ;

                اگر( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    برائے( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        اگر( !بدلا )
            توڑو;
    }

    برائے( i = 0; i < n; i++ )
    {
        برائے( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (دوہرا)t*t;
        }
        W[i] = std::جذر(sd);
    }

    برائے( i = 0; i < n-1; i++ )
    {
        j = i;
        برائے( k = i+1; k < n; k++ )
        {
            اگر( W[j] < W[k] )
                j = k;
        }
        اگر( i != j )
        {
            std::swap(W[i], W[j]);
            اگر( Vt )
            {
                برائے( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                برائے( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    برائے( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    اگر( !Vt )
        واپس;

    RNG rng(0x12345678);
    برائے( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        برائے( صحیح ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ثابت _Tp val0 = (_Tp)(1./m);
            برائے( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            برائے( iter = 0; iter < 2; iter++ )
            {
                برائے( j = 0; j < i; j++ )
                {
                    sd = 0;
                    برائے( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    برائے( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::مطلق_قدر(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    برائے( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            برائے( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (دوہرا)t*t;
            }
            sd = std::جذر(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        برائے( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


ساکن خالی JacobiSVD(اعشاری* At, size_t astep, اعشاری* W, اعشاری* Vt, size_t vstep, صحیح m, صحیح n, صحیح n1=-1)
{
    hal::SVD32f(At, astep, W, تہی, astep, Vt, vstep, m, n, n1);
}

ساکن خالی JacobiSVD(دوہرا* At, size_t astep, دوہرا* W, دوہرا* Vt, size_t vstep, صحیح m, صحیح n, صحیح n1=-1)
{
    hal::SVD64f(At, astep, W, تہی, astep, Vt, vstep, m, n, n1);
}

سانچہ <قسم_نام fptype> ساکن اندرونی صحیح
decodeSVDParameters(ثابت fptype* U, ثابت fptype* Vt, صحیح m, صحیح n, صحیح n1)
{
    صحیح halSVDFlag = 0;
    اگر(Vt == تہی)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    ورنہ اگر(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        اگر(U == تہی)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    ورنہ اگر(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_بھرا_UV;
        اگر(U == تہی)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    واپس halSVDFlag;
}

خالی hal::SVD32f(اعشاری* At, size_t astep, اعشاری* W, اعشاری* U, size_t ustep, اعشاری* Vt, size_t vstep, صحیح m, صحیح n, صحیح n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

خالی hal::SVD64f(دوہرا* At, size_t astep, دوہرا* W, دوہرا* U, size_t ustep, دوہرا* Vt, size_t vstep, صحیح m, صحیح n, صحیح n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
سانچہ<قسم_نام T1, قسم_نام T2, قسم_نام T3> ساکن خالی
MatrAXPY( صحیح m, صحیح n, ثابت T1* x, صحیح dx,
         ثابت T2* a, صحیح inca, T3* y, صحیح dy )
{
    صحیح i;
    برائے( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        صحیح j = 0;
         #اگر CV_ENABLE_UNROLLED
        برائے(; j <= n - 4; j += 4 )
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
        #اگر_ختم
        برائے( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

سانچہ<قسم_نام T> ساکن خالی
SVBkSbImpl_( صحیح m, صحیح n, ثابت T* w, صحیح incw,
       ثابت T* u, صحیح ldu, منطقی uT,
       ثابت T* v, صحیح ldv, منطقی vT,
       ثابت T* b, صحیح ldb, صحیح nb,
       T* x, صحیح ldx, دوہرا* بفر, T eps )
{
    دوہرا حد = 0;
    صحیح udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    صحیح vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    صحیح i, j, nm = std::min(m, n);

    اگر( !b )
        nb = m;

    برائے( i = 0; i < n; i++ )
        برائے( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    برائے( i = 0; i < nm; i++ )
        حد += w[i*incw];
    حد *= eps;

    // v * inv(w) * uT * b
    برائے( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        دوہرا wi = w[i*incw];
        اگر( (دوہرا)std::مطلق_قدر(wi) <= حد )
            جاری;
        wi = 1/wi;

        اگر( nb == 1 )
        {
            دوہرا s = 0;
            اگر( b )
                برائے( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            ورنہ
                s = u[0];
            s *= wi;

            برائے( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        ورنہ
        {
            اگر( b )
            {
                برائے( j = 0; j < nb; j++ )
                    بفر[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, بفر, 0 );
                برائے( j = 0; j < nb; j++ )
                    بفر[j] *= wi;
            }
            ورنہ
            {
                برائے( j = 0; j < nb; j++ )
                    بفر[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, بفر, 0, v, vdelta1, x, ldx );
        }
    }
}

ساکن خالی
SVBkSb( صحیح m, صحیح n, ثابت اعشاری* w, size_t wstep,
        ثابت اعشاری* u, size_t ustep, منطقی uT,
        ثابت اعشاری* v, size_t vstep, منطقی vT,
        ثابت اعشاری* b, size_t bstep, صحیح nb,
        اعشاری* x, size_t xstep, uchar* بفر )
{
    SVBkSbImpl_(m, n, w, wstep ? (صحیح)(wstep/حجم(w[0])) : 1,
                u, (صحیح)(ustep/حجم(u[0])), uT,
                v, (صحیح)(vstep/حجم(v[0])), vT,
                b, (صحیح)(bstep/حجم(b[0])), nb,
                x, (صحیح)(xstep/حجم(x[0])),
                (دوہرا*)alignPtr(بفر, حجم(دوہرا)), (اعشاری)(DBL_EPSILON*2) );
}

ساکن خالی
SVBkSb( صحیح m, صحیح n, ثابت دوہرا* w, size_t wstep,
       ثابت دوہرا* u, size_t ustep, منطقی uT,
       ثابت دوہرا* v, size_t vstep, منطقی vT,
       ثابت دوہرا* b, size_t bstep, صحیح nb,
       دوہرا* x, size_t xstep, uchar* بفر )
{
    SVBkSbImpl_(m, n, w, wstep ? (صحیح)(wstep/حجم(w[0])) : 1,
                u, (صحیح)(ustep/حجم(u[0])), uT,
                v, (صحیح)(vstep/حجم(v[0])), vT,
                b, (صحیح)(bstep/حجم(b[0])), nb,
                x, (صحیح)(xstep/حجم(x[0])),
                (دوہرا*)alignPtr(بفر, حجم(دوہرا)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#تعریف_کرو det2(m)   ((دوہرا)m(0,0)*m(1,1) - (دوہرا)m(0,1)*m(1,0))
#تعریف_کرو det3(m)   (m(0,0)*((دوہرا)m(1,1)*m(2,2) - (دوہرا)m(1,2)*m(2,1)) -  \
                   m(0,1)*((دوہرا)m(1,0)*m(2,2) - (دوہرا)m(1,2)*m(2,0)) +  \
                   m(0,2)*((دوہرا)m(1,0)*m(2,1) - (دوہرا)m(1,1)*m(2,0)))

دوہرا مقطع( داخلہ_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    دوہرا نتیجہ = 0;
    صحیح type = mat.type(), صفیں = mat.صفیں;
    size_t قدم = mat.قدم;
    ثابت uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.صفیں == mat.ستون_تعداد && (type == CV_32F || type == CV_64F));

    #تعریف_کرو Mf(y, x) ((اعشاری*)(m + y*قدم))[x]
    #تعریف_کرو Md(y, x) ((دوہرا*)(m + y*قدم))[x]

    اگر( type == CV_32F )
    {
        اگر( صفیں == 2 )
            نتیجہ = det2(Mf);
        ورنہ اگر( صفیں == 3 )
            نتیجہ = det3(Mf);
        ورنہ اگر( صفیں == 1 )
            نتیجہ = Mf(0,0);
        ورنہ
        {
            size_t buf_سائز = صفیں*صفیں*حجم(اعشاری);
            Auto_بفر<uchar> بفر(buf_سائز);
            Mat a(صفیں, صفیں, CV_32F, بفر.ڈیٹا());
            mat.copy_مقصد(a);

            نتیجہ = hal::LU32f(a.ptr<اعشاری>(), a.قدم, صفیں, 0, 0, 0);
            اگر( نتیجہ )
            {
                برائے( صحیح i = 0; i < صفیں; i++ )
                    نتیجہ *= a.at<اعشاری>(i,i);
            }
        }
    }
    ورنہ
    {
        اگر( صفیں == 2 )
            نتیجہ = det2(Md);
        ورنہ اگر( صفیں == 3 )
            نتیجہ = det3(Md);
        ورنہ اگر( صفیں == 1 )
            نتیجہ = Md(0,0);
        ورنہ
        {
            size_t buf_سائز = صفیں*صفیں*حجم(دوہرا);
            Auto_بفر<uchar> بفر(buf_سائز);
            Mat a(صفیں, صفیں, CV_64F, بفر.ڈیٹا());
            mat.copy_مقصد(a);

            نتیجہ = hal::LU64f(a.ptr<دوہرا>(), a.قدم, صفیں, 0, 0, 0);
            اگر( نتیجہ )
            {
                برائے( صحیح i = 0; i < صفیں; i++ )
                    نتیجہ *= a.at<دوہرا>(i,i);
            }
        }
    }

    #تعریف_منسوخ Mf
    #تعریف_منسوخ Md

    واپس نتیجہ;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#تعریف_کرو Sf( y, x ) ((اعشاری*)(srcdata + y*srcstep))[x]
#تعریف_کرو Sd( y, x ) ((دوہرا*)(srcdata + y*srcstep))[x]
#تعریف_کرو Df( y, x ) ((اعشاری*)(dstdata + y*dststep))[x]
#تعریف_کرو Dd( y, x ) ((دوہرا*)(dstdata + y*dststep))[x]

دوہرا invert( داخلہ_Array _src, OutputArray _dst, صحیح طریقہ )
{
    CV_INSTRUMENT_REGION();

    منطقی نتیجہ = جھوٹ;
    Mat src = _src.getMat();
    صحیح type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_سائز(type);
    صحیح m = src.صفیں, n = src.ستون_تعداد;

    اگر( طریقہ == DECOMP_SVD )
    {
        صحیح nm = std::min(m, n);

        Auto_بفر<uchar> _buf((m*nm + nm + nm*n)*esz + حجم(دوہرا));
        uchar* buf = alignPtr((uchar*)_buf.ڈیٹا(), (صحیح)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::عقب_Subst(w, u, vt, Mat(), _dst);
        واپس type == CV_32F ?
            (w.ptr<اعشاری>()[0] >= FLT_EPSILON ?
             w.ptr<اعشاری>()[n-1]/w.ptr<اعشاری>()[0] : 0) :
            (w.ptr<دوہرا>()[0] >= DBL_EPSILON ?
             w.ptr<دوہرا>()[n-1]/w.ptr<دوہرا>()[0] : 0);
    }

    CV_Assert( m == n );

    اگر( طریقہ == DECOMP_EIG )
    {
        Auto_بفر<uchar> _buf((n*n*2 + n)*esz + حجم(دوہرا));
        uchar* buf = alignPtr((uchar*)_buf.ڈیٹا(), (صحیح)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::عقب_Subst(w, u, vt, Mat(), _dst);
        واپس type == CV_32F ?
        (w.ptr<اعشاری>()[0] >= FLT_EPSILON ?
         w.ptr<اعشاری>()[n-1]/w.ptr<اعشاری>()[0] : 0) :
        (w.ptr<دوہرا>()[0] >= DBL_EPSILON ?
         w.ptr<دوہرا>()[n-1]/w.ptr<دوہرا>()[0] : 0);
    }

    CV_Assert( طریقہ == DECOMP_LU || طریقہ == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    اگر( n <= 3 )
    {
        ثابت uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.قدم;
        size_t dststep = dst.قدم;

        اگر( n == 2 )
        {
            اگر( type == CV_32FC1 )
            {
                دوہرا d = det2(Sf);
                اگر( d != 0. )
                {
                    نتیجہ = سچ;
                    d = 1./d;
                #اگر CV_SIMD128
                    ثابت اعشاری d_32f = (اعشاری)d;
                    ثابت v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_بھرو_halves((ثابت اعشاری *)srcdata, (ثابت اعشاری *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_نکالو<3>(s0, v_combine_کم(v_rotate_دایاں<1>(s0), s0));
                    v_store_کم((اعشاری*)dstdata, s0);
                    v_store_بلند((اعشاری*)(dstdata + dststep), s0);
                #ورنہ
                    دوہرا t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (اعشاری)t0;
                    Df(0,0) = (اعشاری)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (اعشاری)t0;
                    Df(1,0) = (اعشاری)t1;
                #اگر_ختم
                }
            }
            ورنہ
            {
                دوہرا d = det2(Sd);
                اگر( d != 0. )
                {
                    نتیجہ = سچ;
                    d = 1./d;
                #اگر CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_بھرو((ثابت دوہرا *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_بھرو((ثابت دوہرا *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_نکالو<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<دوہرا>(0), v_نکالو<1>(s0, s1));//12
                    v_store((دوہرا*)dstdata, v_combine_کم(sm, ss));//31
                    v_store((دوہرا*)(dstdata + dststep), v_combine_بلند(ss, sm));//20
                #ورنہ
                    دوہرا t0, t1;
                    t0 = Sd(0,0)*d;
                    t1 = Sd(1,1)*d;
                    Dd(1,1) = t0;
                    Dd(0,0) = t1;
                    t0 = -Sd(0,1)*d;
                    t1 = -Sd(1,0)*d;
                    Dd(0,1) = t0;
                    Dd(1,0) = t1;
                #اگر_ختم
                }
            }
        }
        ورنہ اگر( n == 3 )
        {
            اگر( type == CV_32FC1 )
            {
                دوہرا d = det3(Sf);

                اگر( d != 0. )
                {
                    دوہرا t[12];

                    نتیجہ = سچ;
                    d = 1./d;
                    t[0] = (((دوہرا)Sf(1,1) * Sf(2,2) - (دوہرا)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((دوہرا)Sf(0,2) * Sf(2,1) - (دوہرا)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((دوہرا)Sf(0,1) * Sf(1,2) - (دوہرا)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((دوہرا)Sf(1,2) * Sf(2,0) - (دوہرا)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((دوہرا)Sf(0,0) * Sf(2,2) - (دوہرا)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((دوہرا)Sf(0,2) * Sf(1,0) - (دوہرا)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((دوہرا)Sf(1,0) * Sf(2,1) - (دوہرا)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((دوہرا)Sf(0,1) * Sf(2,0) - (دوہرا)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((دوہرا)Sf(0,0) * Sf(1,1) - (دوہرا)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (اعشاری)t[0]; Df(0,1) = (اعشاری)t[1]; Df(0,2) = (اعشاری)t[2];
                    Df(1,0) = (اعشاری)t[3]; Df(1,1) = (اعشاری)t[4]; Df(1,2) = (اعشاری)t[5];
                    Df(2,0) = (اعشاری)t[6]; Df(2,1) = (اعشاری)t[7]; Df(2,2) = (اعشاری)t[8];
                }
            }
            ورنہ
            {
                دوہرا d = det3(Sd);
                اگر( d != 0. )
                {
                    نتیجہ = سچ;
                    d = 1./d;
                    دوہرا t[9];

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
        ورنہ
        {
            CV_Assert( n == 1 );

            اگر( type == CV_32FC1 )
            {
                دوہرا d = Sf(0,0);
                اگر( d != 0. )
                {
                    نتیجہ = سچ;
                    Df(0,0) = (اعشاری)(1./d);
                }
            }
            ورنہ
            {
                دوہرا d = Sd(0,0);
                اگر( d != 0. )
                {
                    نتیجہ = سچ;
                    Dd(0,0) = 1./d;
                }
            }
        }
        اگر( !نتیجہ )
            dst = Scalar(0);
        واپس نتیجہ;
    }

   صحیح elem_سائز_2 = CV_ELEM_سائز(type);
    Auto_بفر<uchar> buf(n*n*elem_سائز_2);
    Mat src1(n, n, type, buf.ڈیٹا());
    src.copy_مقصد(src1);
    سیٹ_شناختی_عنصر(dst);

    اگر( طریقہ == DECOMP_LU && type == CV_32F )
        نتیجہ = hal::LU32f(src1.ptr<اعشاری>(), src1.قدم, n, dst.ptr<اعشاری>(), dst.قدم, n) != 0;
    ورنہ اگر( طریقہ == DECOMP_LU && type == CV_64F )
        نتیجہ = hal::LU64f(src1.ptr<دوہرا>(), src1.قدم, n, dst.ptr<دوہرا>(), dst.قدم, n) != 0;
    ورنہ اگر( طریقہ == DECOMP_CHOLESKY && type == CV_32F )
        نتیجہ = hal::Cholesky32f(src1.ptr<اعشاری>(), src1.قدم, n, dst.ptr<اعشاری>(), dst.قدم, n);
    ورنہ
        نتیجہ = hal::Cholesky64f(src1.ptr<دوہرا>(), src1.قدم, n, dst.ptr<دوہرا>(), dst.قدم, n);

    اگر( !نتیجہ )
        dst = Scalar(0);

    واپس نتیجہ;
}

UMat UMat::inv(صحیح طریقہ) ثابت
{
    UMat m;
    invert(*یہ, m, طریقہ);
    واپس m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

منطقی solve( داخلہ_Array _src, داخلہ_Array _src2arg, OutputArray _dst, صحیح طریقہ )
{
    CV_INSTRUMENT_REGION();

    منطقی نتیجہ = سچ;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    صحیح type = src.type();
    منطقی is_معمول = (طریقہ & DECOMP_معمول) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    طریقہ &= ~DECOMP_معمول;
    CV_Check(طریقہ, طریقہ == DECOMP_LU || طریقہ == DECOMP_SVD || طریقہ == DECOMP_EIG ||
                     طریقہ == DECOMP_CHOLESKY || طریقہ == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (طریقہ != DECOMP_LU && طریقہ != DECOMP_CHOLESKY) ||
        is_معمول || src.صفیں == src.ستون_تعداد );

    // check case of a single equation and small matrix
    اگر( (طریقہ == DECOMP_LU || طریقہ == DECOMP_CHOLESKY) && !is_معمول &&
        src.صفیں <= 3 && src.صفیں == src.ستون_تعداد && _src2.ستون_تعداد == 1 )
    {
        _dst.create( src.ستون_تعداد, _src2.ستون_تعداد, src.type() );
        Mat dst = _dst.getMat();

        #تعریف_کرو bf(y) ((اعشاری*)(bdata + y*src2step))[0]
        #تعریف_کرو bd(y) ((دوہرا*)(bdata + y*src2step))[0]

        ثابت uchar* srcdata = src.ptr();
        ثابت uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.قدم;
        size_t src2step = _src2.قدم;
        size_t dststep = dst.قدم;

        اگر( src.صفیں == 2 )
        {
            اگر( type == CV_32FC1 )
            {
                دوہرا d = det2(Sf);
                اگر( d != 0. )
                {
                    دوہرا t;
                    d = 1./d;
                    t = (اعشاری)(((دوہرا)bf(0)*Sf(1,1) - (دوہرا)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (اعشاری)(((دوہرا)bf(1)*Sf(0,0) - (دوہرا)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (اعشاری)t;
                }
                ورنہ
                    نتیجہ = جھوٹ;
            }
            ورنہ
            {
                دوہرا d = det2(Sd);
                اگر( d != 0. )
                {
                    دوہرا t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                ورنہ
                    نتیجہ = جھوٹ;
            }
        }
        ورنہ اگر( src.صفیں == 3 )
        {
            اگر( type == CV_32FC1 )
            {
                دوہرا d = det3(Sf);
                اگر( d != 0. )
                {
                    اعشاری t[3];
                    d = 1./d;

                    t[0] = (اعشاری)(d*
                           (bf(0)*((دوہرا)Sf(1,1)*Sf(2,2) - (دوہرا)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((دوہرا)bf(1)*Sf(2,2) - (دوہرا)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((دوہرا)bf(1)*Sf(2,1) - (دوہرا)Sf(1,1)*bf(2))));

                    t[1] = (اعشاری)(d*
                           (Sf(0,0)*(دوہرا)(bf(1)*Sf(2,2) - (دوہرا)Sf(1,2)*bf(2)) -
                            bf(0)*((دوہرا)Sf(1,0)*Sf(2,2) - (دوہرا)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((دوہرا)Sf(1,0)*bf(2) - (دوہرا)bf(1)*Sf(2,0))));

                    t[2] = (اعشاری)(d*
                           (Sf(0,0)*((دوہرا)Sf(1,1)*bf(2) - (دوہرا)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((دوہرا)Sf(1,0)*bf(2) - (دوہرا)bf(1)*Sf(2,0)) +
                            bf(0)*((دوہرا)Sf(1,0)*Sf(2,1) - (دوہرا)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                ورنہ
                    نتیجہ = جھوٹ;
            }
            ورنہ
            {
                دوہرا d = det3(Sd);
                اگر( d != 0. )
                {
                    دوہرا t[9];

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
                ورنہ
                    نتیجہ = جھوٹ;
            }
        }
        ورنہ
        {
            CV_Assert( src.صفیں == 1 );

            اگر( type == CV_32FC1 )
            {
                دوہرا d = Sf(0,0);
                اگر( d != 0. )
                    Df(0,0) = (اعشاری)(bf(0)/d);
                ورنہ
                    نتیجہ = جھوٹ;
            }
            ورنہ
            {
                دوہرا d = Sd(0,0);
                اگر( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                ورنہ
                    نتیجہ = جھوٹ;
            }
        }
        واپس نتیجہ;
    }

    صحیح m = src.صفیں, m_ = m, n = src.ستون_تعداد, nb = _src2.ستون_تعداد;
    size_t esz = CV_ELEM_سائز(type), bufsize = 0;
    size_t vstep = align_سائز(n*esz, 16);
    size_t astep = طریقہ == DECOMP_SVD && !is_معمول ? align_سائز(m*esz, 16) : vstep;
    Auto_بفر<uchar> بفر;

    Mat src2 = _src2;
    _dst.create( src.ستون_تعداد, src2.ستون_تعداد, src.type() );
    Mat dst = _dst.getMat();

    اگر( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    اگر( m == n )
        is_معمول = جھوٹ;
    ورنہ اگر( is_معمول )
    {
        m_ = n;
        اگر( طریقہ == DECOMP_SVD )
            طریقہ = DECOMP_EIG;
    }

    CV_Assert(m == src2.صفیں);
    size_t asize = astep*(طریقہ == DECOMP_SVD || is_معمول ? n : m);
    bufsize += asize + 32;

    اگر( is_معمول )
        bufsize += n*nb*esz;
    اگر( طریقہ == DECOMP_SVD || طریقہ == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*حجم(دوہرا) + 32;

    بفر.allocate(bufsize);
    uchar* ptr = alignPtr(بفر.ڈیٹا(), 16);

    Mat a(m_, n, type, ptr, astep);

    اگر( is_معمول )
        mulTransposed(src, a, سچ);
    ورنہ اگر( طریقہ != DECOMP_SVD )
        src.copy_مقصد(a);
    ورنہ
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    اگر( !is_معمول )
    {
        اگر( طریقہ == DECOMP_LU || طریقہ == DECOMP_CHOLESKY )
            src2.copy_مقصد(dst);
    }
    ورنہ
    {
        // a'*b
        اگر( طریقہ == DECOMP_LU || طریقہ == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        ورنہ
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    اگر( طریقہ == DECOMP_LU )
    {
        اگر( type == CV_32F )
            نتیجہ = hal::LU32f(a.ptr<اعشاری>(), a.قدم, n, dst.ptr<اعشاری>(), dst.قدم, nb) != 0;
        ورنہ
            نتیجہ = hal::LU64f(a.ptr<دوہرا>(), a.قدم, n, dst.ptr<دوہرا>(), dst.قدم, nb) != 0;
    }
    ورنہ اگر( طریقہ == DECOMP_CHOLESKY )
    {
        اگر( type == CV_32F )
            نتیجہ = hal::Cholesky32f(a.ptr<اعشاری>(), a.قدم, n, dst.ptr<اعشاری>(), dst.قدم, nb);
        ورنہ
            نتیجہ = hal::Cholesky64f(a.ptr<دوہرا>(), a.قدم, n, dst.ptr<دوہرا>(), dst.قدم, nb);
    }
    ورنہ اگر( طریقہ == DECOMP_QR )
    {
        Mat rhsMat;
        اگر( is_معمول || m == n )
        {
            src2.copy_مقصد(dst);
            rhsMat = dst;
        }
        ورنہ
        {
            rhsMat = Mat(m, nb, type);
            src2.copy_مقصد(rhsMat);
        }

        اگر( type == CV_32F )
            نتیجہ = hal::QR32f(a.ptr<اعشاری>(), a.قدم, a.صفیں, a.ستون_تعداد, rhsMat.ستون_تعداد, rhsMat.ptr<اعشاری>(), rhsMat.قدم, تہی) != 0;
        ورنہ
            نتیجہ = hal::QR64f(a.ptr<دوہرا>(), a.قدم, a.صفیں, a.ستون_تعداد, rhsMat.ستون_تعداد, rhsMat.ptr<دوہرا>(), rhsMat.قدم, تہی) != 0;

        اگر (rhsMat.صفیں != dst.صفیں)
            rhsMat.صف_Range_2(0, dst.صفیں).copy_مقصد(dst);
    }
    ورنہ
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        اگر( طریقہ == DECOMP_EIG )
        {
            اگر( type == CV_32F )
                Jacobi(a.ptr<اعشاری>(), a.قدم, w.ptr<اعشاری>(), v.ptr<اعشاری>(), v.قدم, n, ptr);
            ورنہ
                Jacobi(a.ptr<دوہرا>(), a.قدم, w.ptr<دوہرا>(), v.ptr<دوہرا>(), v.قدم, n, ptr);
            u = v;
        }
        ورنہ
        {
            اگر( type == CV_32F )
                JacobiSVD(a.ptr<اعشاری>(), a.قدم, w.ptr<اعشاری>(), v.ptr<اعشاری>(), v.قدم, m_, n);
            ورنہ
                JacobiSVD(a.ptr<دوہرا>(), a.قدم, w.ptr<دوہرا>(), v.ptr<دوہرا>(), v.قدم, m_, n);
            u = a;
        }

        اگر( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<اعشاری>(), 0, u.ptr<اعشاری>(), u.قدم, سچ,
                   v.ptr<اعشاری>(), v.قدم, سچ, src2.ptr<اعشاری>(),
                   src2.قدم, nb, dst.ptr<اعشاری>(), dst.قدم, ptr);
        }
        ورنہ
        {
            SVBkSb(m_, n, w.ptr<دوہرا>(), 0, u.ptr<دوہرا>(), u.قدم, سچ,
                   v.ptr<دوہرا>(), v.قدم, سچ, src2.ptr<دوہرا>(),
                   src2.قدم, nb, dst.ptr<دوہرا>(), dst.قدم, ptr);
        }
        نتیجہ = سچ;
    }

    اگر( !نتیجہ )
        dst = Scalar(0);

    واپس نتیجہ;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

منطقی eigen( داخلہ_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    صحیح type = src.type();
    صحیح n = src.صفیں;

    CV_Assert( src.صفیں == src.ستون_تعداد );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    اگر( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#اگر_تعریف_شدہ HAVE_EIGEN
    ثابت منطقی evecNeeded = _evects.needed();
    ثابت صحیح esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    اگر ( type == CV_64F )
    {
        Eigen::میٹرکس_Xd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::میٹرکس_Xd> es;
        es.compute(src_eig, esOptions);
        اگر ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            اگر ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            واپس سچ;
        }
    } ورنہ { // CV_32F
        Eigen::میٹرکس_Xf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::میٹرکس_Xf> es;
        es.compute(src_eig, esOptions);
        اگر ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            اگر ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            واپس سچ;
        }
    }
    واپس جھوٹ;
#ورنہ

    size_t elem_سائز = src.elem_سائز(), astep = align_سائز(n*elem_سائز, 16);
    Auto_بفر<uchar> buf(n*astep + n*5*elem_سائز + 32);
    uchar* ptr = alignPtr(buf.ڈیٹا(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elem_سائز*n;
    src.copy_مقصد(a);
    منطقی ok = type == CV_32F ?
        Jacobi(a.ptr<اعشاری>(), a.قدم, w.ptr<اعشاری>(), v.ptr<اعشاری>(), v.قدم, n, ptr) :
        Jacobi(a.ptr<دوہرا>(), a.قدم, w.ptr<دوہرا>(), v.ptr<دوہرا>(), v.قدم, n, ptr);

    w.copy_مقصد(_evals);
    واپس ok;
#اگر_ختم
}

ساکن خالی _SVDcompute( داخلہ_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, صحیح flags )
{
    Mat src = _aarr.getMat();
    صحیح m = src.صفیں, n = src.ستون_تعداد;
    صحیح type = src.type();
    منطقی compute_uv = _u.needed() || _vt.needed();
    منطقی بھرا_uv = (flags & SVD::بھرا_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    اگر( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = بھرا_uv = جھوٹ;
    }

    منطقی at = جھوٹ;
    اگر( m < n )
    {
        std::swap(m, n);
        at = سچ;
    }

    صحیح urows = بھرا_uv ? m : n;
    size_t esz = src.elem_سائز(), astep = align_سائز(m*esz, 16), vstep = align_سائز(n*esz, 16);
    Auto_بفر<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.ڈیٹا(), 16);
    Mat عارضی_a(n, m, type, buf, astep);
    Mat عارضی_w(n, 1, type, buf + urows*astep);
    Mat عارضی_u(urows, m, type, buf, astep), عارضی_v;

    اگر( compute_uv )
        عارضی_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    اگر( urows > n )
        عارضی_u = Scalar::all(0);

    اگر( !at )
        transpose(src, عارضی_a);
    ورنہ
        src.copy_مقصد(عارضی_a);

    اگر( type == CV_32F )
    {
        JacobiSVD(عارضی_a.ptr<اعشاری>(), عارضی_u.قدم, عارضی_w.ptr<اعشاری>(),
              عارضی_v.ptr<اعشاری>(), عارضی_v.قدم, m, n, compute_uv ? urows : 0);
    }
    ورنہ
    {
        JacobiSVD(عارضی_a.ptr<دوہرا>(), عارضی_u.قدم, عارضی_w.ptr<دوہرا>(),
              عارضی_v.ptr<دوہرا>(), عارضی_v.قدم, m, n, compute_uv ? urows : 0);
    }
    عارضی_w.copy_مقصد(_w);
    اگر( compute_uv )
    {
        اگر( !at )
        {
            اگر( _u.needed() )
                transpose(عارضی_u, _u);
            اگر( _vt.needed() )
                عارضی_v.copy_مقصد(_vt);
        }
        ورنہ
        {
            اگر( _u.needed() )
                transpose(عارضی_v, _u);
            اگر( _vt.needed() )
                عارضی_u.copy_مقصد(_vt);
        }
    }
}


خالی SVD::compute( داخلہ_Array a, OutputArray w, OutputArray u, OutputArray vt, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

خالی SVD::compute( داخلہ_Array a, OutputArray w, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

خالی SVD::عقب_Subst( داخلہ_Array _w, داخلہ_Array _u, داخلہ_Array _vt,
                     داخلہ_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    صحیح type = w.type(), esz = (صحیح)w.elem_سائز();
    صحیح m = u.صفیں, n = vt.ستون_تعداد, nb = rhs.ڈیٹا ? rhs.ستون_تعداد : m, nm = std::min(m, n);
    size_t wstep = w.صفیں == 1 ? (size_t)esz : w.ستون_تعداد == 1 ? (size_t)w.قدم : (size_t)w.قدم + esz;
    Auto_بفر<uchar> بفر(nb*حجم(دوہرا) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.ڈیٹا && vt.ڈیٹا && w.ڈیٹا );
    CV_Assert( u.ستون_تعداد >= nm && vt.صفیں >= nm &&
              (w.size() == سائز(nm, 1) || w.size() == سائز(1, nm) || w.size() == سائز(vt.صفیں, u.ستون_تعداد)) );
    CV_Assert( rhs.ڈیٹا == 0 || (rhs.type() == type && rhs.صفیں == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    اگر( type == CV_32F )
        SVBkSb(m, n, w.ptr<اعشاری>(), wstep, u.ptr<اعشاری>(), u.قدم, جھوٹ,
               vt.ptr<اعشاری>(), vt.قدم, سچ, rhs.ptr<اعشاری>(), rhs.قدم, nb,
               dst.ptr<اعشاری>(), dst.قدم, بفر.ڈیٹا());
    ورنہ اگر( type == CV_64F )
        SVBkSb(m, n, w.ptr<دوہرا>(), wstep, u.ptr<دوہرا>(), u.قدم, جھوٹ,
               vt.ptr<دوہرا>(), vt.قدم, سچ, rhs.ptr<دوہرا>(), rhs.قدم, nb,
               dst.ptr<دوہرا>(), dst.قدم, بفر.ڈیٹا());
    ورنہ
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::عامل ()(داخلہ_Array a, صحیح flags)
{
    _SVDcompute(a, w, u, vt, flags);
    واپس *یہ;
}


خالی SVD::عقب_Subst( داخلہ_Array rhs, OutputArray dst ) ثابت
{
    عقب_Subst( w, u, vt, rhs, dst );
}

}


خالی cv::SVDecomp(داخلہ_Array src, OutputArray w, OutputArray u, OutputArray vt, صحیح flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

خالی cv::SVBackSubst(داخلہ_Array w, داخلہ_Array u, داخلہ_Array vt, داخلہ_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::عقب_Subst(w, u, vt, rhs, dst);
}
