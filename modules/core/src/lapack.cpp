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

#ضمن "precomp.hpp"
#ضمن <limits>

#اگر_تعریف_شده HAVE_EIGEN
#  إذا defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  إذا defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#پایان_اگر

نطاق cv
{

صحيح LU(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    أرجع hal::LU32f(A, astep, m, b, bstep, n);
}

صحيح LU(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    أرجع hal::LU64f(A, astep, m, b, bstep, n);
}

منطقي Cholesky(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    أرجع hal::Cholesky32f(A, astep, m, b, bstep, n);
}

منطقي Cholesky(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    أرجع hal::Cholesky64f(A, astep, m, b, bstep, n);
}

قالب<نام_نوع _Tp> ساكن مضمن _Tp hypot(_Tp a, _Tp b)
{
    a = std::قدر_مطلق(a);
    b = std::قدر_مطلق(b);
    إذا( a > b )
    {
        b /= a;
        أرجع a*std::جذر(1 + b*b);
    }
    إذا( b > 0 )
    {
        a /= b;
        أرجع b*std::جذر(1 + a*a);
    }
    أرجع 0;
}


قالب<نام_نوع _Tp> منطقي
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, صحيح n, uchar* buf )
{
    ثابت _Tp eps = std::numeric_limits<_Tp>::epsilon();
    صحيح i, j, k, m;

    astep /= حجم(A[0]);
    إذا( V )
    {
        vstep /= حجم(V[0]);
        لكل( i = 0; i < n; i++ )
        {
            لكل( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    صحيح iters, maxIters = n*n*30;

    صحيح* indR = (صحيح*)alignPtr(buf, حجم(صحيح));
    صحيح* indC = indR + n;
    _Tp mv = (_Tp)0;

    لكل( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        إذا( k < n - 1 )
        {
            لكل( m = k+1, mv = std::قدر_مطلق(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::قدر_مطلق(A[astep*k+i]);
                إذا( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        إذا( k > 0 )
        {
            لكل( m = 0, mv = std::قدر_مطلق(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::قدر_مطلق(A[astep*i+k]);
                إذا( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    إذا( n > 1 ) لكل( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        لكل( k = 0, mv = std::قدر_مطلق(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::قدر_مطلق(A[astep*i + indR[i]]);
            إذا( mv < val )
                mv = val, k = i;
        }
        صحيح l = indR[k];
        لكل( i = 1; i < n; i++ )
        {
            _Tp val = std::قدر_مطلق(A[astep*indC[i] + i]);
            إذا( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        إذا( std::قدر_مطلق(p) <= eps )
            اكسر;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::قدر_مطلق(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        إذا( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#لغو_تعریف rotate
#تعریف_کن rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        لكل( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        لكل( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        لكل( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        إذا( V )
            لكل( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#لغو_تعریف rotate

        لكل( j = 0; j < 2; j++ )
        {
            صحيح idx = j == 0 ? k : l;
            إذا( idx < n - 1 )
            {
                لكل( m = idx+1, mv = std::قدر_مطلق(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::قدر_مطلق(A[astep*idx+i]);
                    إذا( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            إذا( idx > 0 )
            {
                لكل( m = 0, mv = std::قدر_مطلق(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::قدر_مطلق(A[astep*i+idx]);
                    إذا( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    لكل( k = 0; k < n-1; k++ )
    {
        m = k;
        لكل( i = k+1; i < n; i++ )
        {
            إذا( W[m] < W[i] )
                m = i;
        }
        إذا( k != m )
        {
            std::swap(W[m], W[k]);
            إذا( V )
                لكل( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    أرجع درست;
}

ساكن منطقي Jacobi( عائم* S, size_t sstep, عائم* e, عائم* E, size_t estep, صحيح n, uchar* buf )
{
    أرجع JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

ساكن منطقي Jacobi( مزدوج* S, size_t sstep, مزدوج* e, مزدوج* E, size_t estep, صحيح n, uchar* buf )
{
    أرجع JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


قالب<نام_نوع T> بنية VBLAS
{
    صحيح dot(ثابت T*, ثابت T*, صحيح, T*) ثابت { أرجع 0; }
    صحيح givens(T*, T*, صحيح, T, T) ثابت { أرجع 0; }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
قالب<> مضمن صحيح VBLAS<عائم>::dot(ثابت عائم* a, ثابت عائم* b, صحيح n, عائم* نتیجه) ثابت
{
    إذا( n < 2*VTraits<v_float32>::vlanes() )
        أرجع 0;
    صحيح k = 0;
    v_float32 s0 = vx_setzero_f32();
    لكل( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_بارگیری(a + k);
        v_float32 b0 = vx_بارگیری(b + k);

        s0 = v_جمع_کن(s0, v_mul(a0, b0));
    }
    *نتیجه = v_reduce_جمع(s0);
    vx_cleanup();
    أرجع k;
}


قالب<> مضمن صحيح VBLAS<عائم>::givens(عائم* a, عائم* b, صحيح n, عائم c, عائم s) ثابت
{
    إذا( n < VTraits<v_float32>::vlanes())
        أرجع 0;
    صحيح k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    لكل( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_بارگیری(a + k);
        v_float32 b0 = vx_بارگیری(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    أرجع k;
}


#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
قالب<> مضمن صحيح VBLAS<مزدوج>::dot(ثابت مزدوج* a, ثابت مزدوج* b, صحيح n, مزدوج* نتیجه) ثابت
{
    إذا( n < 2*VTraits<v_float64>::vlanes() )
        أرجع 0;
    صحيح k = 0;
    v_float64 s0 = vx_setzero_f64();
    لكل( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_بارگیری(a + k);
        v_float64 b0 = vx_بارگیری(b + k);

        s0 = v_جمع_کن(s0, v_mul(a0, b0));
    }
    *نتیجه = v_reduce_جمع(s0);
    vx_cleanup();
    أرجع k;
}


قالب<> مضمن صحيح VBLAS<مزدوج>::givens(مزدوج* a, مزدوج* b, صحيح n, مزدوج c, مزدوج s) ثابت
{
    صحيح k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    لكل( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_بارگیری(a + k);
        v_float64 b0 = vx_بارگیری(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    أرجع k;
}


#پایان_اگر // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#پایان_اگر // (CV_SIMD || CV_SIMD_SCALABLE)

قالب<نام_نوع _Tp> فراغ
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               صحيح m, صحيح n, صحيح n1, مزدوج minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    Auto_میانگیر<مزدوج> Wbuf(n);
    مزدوج* W = Wbuf.داده();
    صحيح i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    مزدوج sd;
    astep /= حجم(At[0]);
    vstep /= حجم(Vt[0]);

    لكل( i = 0; i < n; i++ )
    {
        لكل( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (مزدوج)t*t;
        }
        W[i] = sd;

        إذا( Vt )
        {
            لكل( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    لكل( iter = 0; iter < max_iter; iter++ )
    {
        منطقي تغییریافته = خطأ;

        لكل( i = 0; i < n-1; i++ )
            لكل( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                مزدوج a = W[i], p = 0, b = W[j];

                لكل( k = 0; k < m; k++ )
                    p += (مزدوج)Ai[k]*Aj[k];

                إذا( std::قدر_مطلق(p) <= eps*std::جذر((مزدوج)a*b) )
                    استمر;

                p *= 2;
                مزدوج beta = a - b, gamma = hypot((مزدوج)p, beta);
                إذا( beta < 0 )
                {
                    مزدوج delta = (gamma - beta)*0.5;
                    s = (_Tp)std::جذر(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                وإلا
                {
                    c = (_Tp)std::جذر((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                لكل( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (مزدوج)t0*t0; b += (مزدوج)t1*t1;
                }
                W[i] = a; W[j] = b;

                تغییریافته = درست;

                إذا( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    لكل( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        إذا( !تغییریافته )
            اكسر;
    }

    لكل( i = 0; i < n; i++ )
    {
        لكل( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (مزدوج)t*t;
        }
        W[i] = std::جذر(sd);
    }

    لكل( i = 0; i < n-1; i++ )
    {
        j = i;
        لكل( k = i+1; k < n; k++ )
        {
            إذا( W[j] < W[k] )
                j = k;
        }
        إذا( i != j )
        {
            std::swap(W[i], W[j]);
            إذا( Vt )
            {
                لكل( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                لكل( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    لكل( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    إذا( !Vt )
        أرجع;

    RNG rng(0x12345678);
    لكل( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        لكل( صحيح ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ثابت _Tp val0 = (_Tp)(1./m);
            لكل( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            لكل( iter = 0; iter < 2; iter++ )
            {
                لكل( j = 0; j < i; j++ )
                {
                    sd = 0;
                    لكل( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    لكل( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::قدر_مطلق(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    لكل( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            لكل( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (مزدوج)t*t;
            }
            sd = std::جذر(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        لكل( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


ساكن فراغ JacobiSVD(عائم* At, size_t astep, عائم* W, عائم* Vt, size_t vstep, صحيح m, صحيح n, صحيح n1=-1)
{
    hal::SVD32f(At, astep, W, عدم, astep, Vt, vstep, m, n, n1);
}

ساكن فراغ JacobiSVD(مزدوج* At, size_t astep, مزدوج* W, مزدوج* Vt, size_t vstep, صحيح m, صحيح n, صحيح n1=-1)
{
    hal::SVD64f(At, astep, W, عدم, astep, Vt, vstep, m, n, n1);
}

قالب <نام_نوع fptype> ساكن مضمن صحيح
decodeSVDParameters(ثابت fptype* U, ثابت fptype* Vt, صحيح m, صحيح n, صحيح n1)
{
    صحيح halSVDFlag = 0;
    إذا(Vt == عدم)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    وإلا إذا(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        إذا(U == عدم)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    وإلا إذا(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_پر_UV;
        إذا(U == عدم)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    أرجع halSVDFlag;
}

فراغ hal::SVD32f(عائم* At, size_t astep, عائم* W, عائم* U, size_t ustep, عائم* Vt, size_t vstep, صحيح m, صحيح n, صحيح n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

فراغ hal::SVD64f(مزدوج* At, size_t astep, مزدوج* W, مزدوج* U, size_t ustep, مزدوج* Vt, size_t vstep, صحيح m, صحيح n, صحيح n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
قالب<نام_نوع T1, نام_نوع T2, نام_نوع T3> ساكن فراغ
MatrAXPY( صحيح m, صحيح n, ثابت T1* x, صحيح dx,
         ثابت T2* a, صحيح inca, T3* y, صحيح dy )
{
    صحيح i;
    لكل( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        صحيح j = 0;
         #إذا CV_ENABLE_UNROLLED
        لكل(; j <= n - 4; j += 4 )
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
        #پایان_اگر
        لكل( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

قالب<نام_نوع T> ساكن فراغ
SVBkSbImpl_( صحيح m, صحيح n, ثابت T* w, صحيح incw,
       ثابت T* u, صحيح ldu, منطقي uT,
       ثابت T* v, صحيح ldv, منطقي vT,
       ثابت T* b, صحيح ldb, صحيح nb,
       T* x, صحيح ldx, مزدوج* میانگیر, T eps )
{
    مزدوج آستانه = 0;
    صحيح udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    صحيح vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    صحيح i, j, nm = std::min(m, n);

    إذا( !b )
        nb = m;

    لكل( i = 0; i < n; i++ )
        لكل( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    لكل( i = 0; i < nm; i++ )
        آستانه += w[i*incw];
    آستانه *= eps;

    // v * inv(w) * uT * b
    لكل( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        مزدوج wi = w[i*incw];
        إذا( (مزدوج)std::قدر_مطلق(wi) <= آستانه )
            استمر;
        wi = 1/wi;

        إذا( nb == 1 )
        {
            مزدوج s = 0;
            إذا( b )
                لكل( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            وإلا
                s = u[0];
            s *= wi;

            لكل( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        وإلا
        {
            إذا( b )
            {
                لكل( j = 0; j < nb; j++ )
                    میانگیر[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, میانگیر, 0 );
                لكل( j = 0; j < nb; j++ )
                    میانگیر[j] *= wi;
            }
            وإلا
            {
                لكل( j = 0; j < nb; j++ )
                    میانگیر[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, میانگیر, 0, v, vdelta1, x, ldx );
        }
    }
}

ساكن فراغ
SVBkSb( صحيح m, صحيح n, ثابت عائم* w, size_t wstep,
        ثابت عائم* u, size_t ustep, منطقي uT,
        ثابت عائم* v, size_t vstep, منطقي vT,
        ثابت عائم* b, size_t bstep, صحيح nb,
        عائم* x, size_t xstep, uchar* میانگیر )
{
    SVBkSbImpl_(m, n, w, wstep ? (صحيح)(wstep/حجم(w[0])) : 1,
                u, (صحيح)(ustep/حجم(u[0])), uT,
                v, (صحيح)(vstep/حجم(v[0])), vT,
                b, (صحيح)(bstep/حجم(b[0])), nb,
                x, (صحيح)(xstep/حجم(x[0])),
                (مزدوج*)alignPtr(میانگیر, حجم(مزدوج)), (عائم)(DBL_EPSILON*2) );
}

ساكن فراغ
SVBkSb( صحيح m, صحيح n, ثابت مزدوج* w, size_t wstep,
       ثابت مزدوج* u, size_t ustep, منطقي uT,
       ثابت مزدوج* v, size_t vstep, منطقي vT,
       ثابت مزدوج* b, size_t bstep, صحيح nb,
       مزدوج* x, size_t xstep, uchar* میانگیر )
{
    SVBkSbImpl_(m, n, w, wstep ? (صحيح)(wstep/حجم(w[0])) : 1,
                u, (صحيح)(ustep/حجم(u[0])), uT,
                v, (صحيح)(vstep/حجم(v[0])), vT,
                b, (صحيح)(bstep/حجم(b[0])), nb,
                x, (صحيح)(xstep/حجم(x[0])),
                (مزدوج*)alignPtr(میانگیر, حجم(مزدوج)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#تعریف_کن det2(m)   ((مزدوج)m(0,0)*m(1,1) - (مزدوج)m(0,1)*m(1,0))
#تعریف_کن det3(m)   (m(0,0)*((مزدوج)m(1,1)*m(2,2) - (مزدوج)m(1,2)*m(2,1)) -  \
                   m(0,1)*((مزدوج)m(1,0)*m(2,2) - (مزدوج)m(1,2)*m(2,0)) +  \
                   m(0,2)*((مزدوج)m(1,0)*m(2,1) - (مزدوج)m(1,1)*m(2,0)))

مزدوج دترمینان( درون_داد_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    مزدوج نتیجه = 0;
    صحيح type = mat.type(), ردیفها = mat.ردیفها;
    size_t گام = mat.گام;
    ثابت uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.ردیفها == mat.تعداد_ستون && (type == CV_32F || type == CV_64F));

    #تعریف_کن Mf(y, x) ((عائم*)(m + y*گام))[x]
    #تعریف_کن Md(y, x) ((مزدوج*)(m + y*گام))[x]

    إذا( type == CV_32F )
    {
        إذا( ردیفها == 2 )
            نتیجه = det2(Mf);
        وإلا إذا( ردیفها == 3 )
            نتیجه = det3(Mf);
        وإلا إذا( ردیفها == 1 )
            نتیجه = Mf(0,0);
        وإلا
        {
            size_t buf_اندازه = ردیفها*ردیفها*حجم(عائم);
            Auto_میانگیر<uchar> میانگیر(buf_اندازه);
            Mat a(ردیفها, ردیفها, CV_32F, میانگیر.داده());
            mat.copy_مقصد(a);

            نتیجه = hal::LU32f(a.ptr<عائم>(), a.گام, ردیفها, 0, 0, 0);
            إذا( نتیجه )
            {
                لكل( صحيح i = 0; i < ردیفها; i++ )
                    نتیجه *= a.at<عائم>(i,i);
            }
        }
    }
    وإلا
    {
        إذا( ردیفها == 2 )
            نتیجه = det2(Md);
        وإلا إذا( ردیفها == 3 )
            نتیجه = det3(Md);
        وإلا إذا( ردیفها == 1 )
            نتیجه = Md(0,0);
        وإلا
        {
            size_t buf_اندازه = ردیفها*ردیفها*حجم(مزدوج);
            Auto_میانگیر<uchar> میانگیر(buf_اندازه);
            Mat a(ردیفها, ردیفها, CV_64F, میانگیر.داده());
            mat.copy_مقصد(a);

            نتیجه = hal::LU64f(a.ptr<مزدوج>(), a.گام, ردیفها, 0, 0, 0);
            إذا( نتیجه )
            {
                لكل( صحيح i = 0; i < ردیفها; i++ )
                    نتیجه *= a.at<مزدوج>(i,i);
            }
        }
    }

    #لغو_تعریف Mf
    #لغو_تعریف Md

    أرجع نتیجه;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#تعریف_کن Sf( y, x ) ((عائم*)(srcdata + y*srcstep))[x]
#تعریف_کن Sd( y, x ) ((مزدوج*)(srcdata + y*srcstep))[x]
#تعریف_کن Df( y, x ) ((عائم*)(dstdata + y*dststep))[x]
#تعریف_کن Dd( y, x ) ((مزدوج*)(dstdata + y*dststep))[x]

مزدوج invert( درون_داد_Array _src, OutputArray _dst, صحيح روش )
{
    CV_INSTRUMENT_REGION();

    منطقي نتیجه = خطأ;
    Mat src = _src.getMat();
    صحيح type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_اندازه(type);
    صحيح m = src.ردیفها, n = src.تعداد_ستون;

    إذا( روش == DECOMP_SVD )
    {
        صحيح nm = std::min(m, n);

        Auto_میانگیر<uchar> _buf((m*nm + nm + nm*n)*esz + حجم(مزدوج));
        uchar* buf = alignPtr((uchar*)_buf.داده(), (صحيح)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::پشت_Subst(w, u, vt, Mat(), _dst);
        أرجع type == CV_32F ?
            (w.ptr<عائم>()[0] >= FLT_EPSILON ?
             w.ptr<عائم>()[n-1]/w.ptr<عائم>()[0] : 0) :
            (w.ptr<مزدوج>()[0] >= DBL_EPSILON ?
             w.ptr<مزدوج>()[n-1]/w.ptr<مزدوج>()[0] : 0);
    }

    CV_Assert( m == n );

    إذا( روش == DECOMP_EIG )
    {
        Auto_میانگیر<uchar> _buf((n*n*2 + n)*esz + حجم(مزدوج));
        uchar* buf = alignPtr((uchar*)_buf.داده(), (صحيح)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::پشت_Subst(w, u, vt, Mat(), _dst);
        أرجع type == CV_32F ?
        (w.ptr<عائم>()[0] >= FLT_EPSILON ?
         w.ptr<عائم>()[n-1]/w.ptr<عائم>()[0] : 0) :
        (w.ptr<مزدوج>()[0] >= DBL_EPSILON ?
         w.ptr<مزدوج>()[n-1]/w.ptr<مزدوج>()[0] : 0);
    }

    CV_Assert( روش == DECOMP_LU || روش == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    إذا( n <= 3 )
    {
        ثابت uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.گام;
        size_t dststep = dst.گام;

        إذا( n == 2 )
        {
            إذا( type == CV_32FC1 )
            {
                مزدوج d = det2(Sf);
                إذا( d != 0. )
                {
                    نتیجه = درست;
                    d = 1./d;
                #إذا CV_SIMD128
                    ثابت عائم d_32f = (عائم)d;
                    ثابت v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_بارگیری_halves((ثابت عائم *)srcdata, (ثابت عائم *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_استخراج<3>(s0, v_combine_کم(v_rotate_راست<1>(s0), s0));
                    v_store_کم((عائم*)dstdata, s0);
                    v_store_بالا((عائم*)(dstdata + dststep), s0);
                #وإلا
                    مزدوج t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (عائم)t0;
                    Df(0,0) = (عائم)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (عائم)t0;
                    Df(1,0) = (عائم)t1;
                #پایان_اگر
                }
            }
            وإلا
            {
                مزدوج d = det2(Sd);
                إذا( d != 0. )
                {
                    نتیجه = درست;
                    d = 1./d;
                #إذا CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_بارگیری((ثابت مزدوج *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_بارگیری((ثابت مزدوج *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_استخراج<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<مزدوج>(0), v_استخراج<1>(s0, s1));//12
                    v_store((مزدوج*)dstdata, v_combine_کم(sm, ss));//31
                    v_store((مزدوج*)(dstdata + dststep), v_combine_بالا(ss, sm));//20
                #وإلا
                    مزدوج t0, t1;
                    t0 = Sd(0,0)*d;
                    t1 = Sd(1,1)*d;
                    Dd(1,1) = t0;
                    Dd(0,0) = t1;
                    t0 = -Sd(0,1)*d;
                    t1 = -Sd(1,0)*d;
                    Dd(0,1) = t0;
                    Dd(1,0) = t1;
                #پایان_اگر
                }
            }
        }
        وإلا إذا( n == 3 )
        {
            إذا( type == CV_32FC1 )
            {
                مزدوج d = det3(Sf);

                إذا( d != 0. )
                {
                    مزدوج t[12];

                    نتیجه = درست;
                    d = 1./d;
                    t[0] = (((مزدوج)Sf(1,1) * Sf(2,2) - (مزدوج)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((مزدوج)Sf(0,2) * Sf(2,1) - (مزدوج)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((مزدوج)Sf(0,1) * Sf(1,2) - (مزدوج)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((مزدوج)Sf(1,2) * Sf(2,0) - (مزدوج)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((مزدوج)Sf(0,0) * Sf(2,2) - (مزدوج)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((مزدوج)Sf(0,2) * Sf(1,0) - (مزدوج)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((مزدوج)Sf(1,0) * Sf(2,1) - (مزدوج)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((مزدوج)Sf(0,1) * Sf(2,0) - (مزدوج)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((مزدوج)Sf(0,0) * Sf(1,1) - (مزدوج)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (عائم)t[0]; Df(0,1) = (عائم)t[1]; Df(0,2) = (عائم)t[2];
                    Df(1,0) = (عائم)t[3]; Df(1,1) = (عائم)t[4]; Df(1,2) = (عائم)t[5];
                    Df(2,0) = (عائم)t[6]; Df(2,1) = (عائم)t[7]; Df(2,2) = (عائم)t[8];
                }
            }
            وإلا
            {
                مزدوج d = det3(Sd);
                إذا( d != 0. )
                {
                    نتیجه = درست;
                    d = 1./d;
                    مزدوج t[9];

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
        وإلا
        {
            CV_Assert( n == 1 );

            إذا( type == CV_32FC1 )
            {
                مزدوج d = Sf(0,0);
                إذا( d != 0. )
                {
                    نتیجه = درست;
                    Df(0,0) = (عائم)(1./d);
                }
            }
            وإلا
            {
                مزدوج d = Sd(0,0);
                إذا( d != 0. )
                {
                    نتیجه = درست;
                    Dd(0,0) = 1./d;
                }
            }
        }
        إذا( !نتیجه )
            dst = Scalar(0);
        أرجع نتیجه;
    }

   صحيح elem_اندازه_2 = CV_ELEM_اندازه(type);
    Auto_میانگیر<uchar> buf(n*n*elem_اندازه_2);
    Mat src1(n, n, type, buf.داده());
    src.copy_مقصد(src1);
    مجموعه_عنصر_خنثی(dst);

    إذا( روش == DECOMP_LU && type == CV_32F )
        نتیجه = hal::LU32f(src1.ptr<عائم>(), src1.گام, n, dst.ptr<عائم>(), dst.گام, n) != 0;
    وإلا إذا( روش == DECOMP_LU && type == CV_64F )
        نتیجه = hal::LU64f(src1.ptr<مزدوج>(), src1.گام, n, dst.ptr<مزدوج>(), dst.گام, n) != 0;
    وإلا إذا( روش == DECOMP_CHOLESKY && type == CV_32F )
        نتیجه = hal::Cholesky32f(src1.ptr<عائم>(), src1.گام, n, dst.ptr<عائم>(), dst.گام, n);
    وإلا
        نتیجه = hal::Cholesky64f(src1.ptr<مزدوج>(), src1.گام, n, dst.ptr<مزدوج>(), dst.گام, n);

    إذا( !نتیجه )
        dst = Scalar(0);

    أرجع نتیجه;
}

UMat UMat::inv(صحيح روش) ثابت
{
    UMat m;
    invert(*هذا, m, روش);
    أرجع m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

منطقي solve( درون_داد_Array _src, درون_داد_Array _src2arg, OutputArray _dst, صحيح روش )
{
    CV_INSTRUMENT_REGION();

    منطقي نتیجه = درست;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    صحيح type = src.type();
    منطقي is_نرمال = (روش & DECOMP_نرمال) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    روش &= ~DECOMP_نرمال;
    CV_Check(روش, روش == DECOMP_LU || روش == DECOMP_SVD || روش == DECOMP_EIG ||
                     روش == DECOMP_CHOLESKY || روش == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (روش != DECOMP_LU && روش != DECOMP_CHOLESKY) ||
        is_نرمال || src.ردیفها == src.تعداد_ستون );

    // check case of a single equation and small matrix
    إذا( (روش == DECOMP_LU || روش == DECOMP_CHOLESKY) && !is_نرمال &&
        src.ردیفها <= 3 && src.ردیفها == src.تعداد_ستون && _src2.تعداد_ستون == 1 )
    {
        _dst.create( src.تعداد_ستون, _src2.تعداد_ستون, src.type() );
        Mat dst = _dst.getMat();

        #تعریف_کن bf(y) ((عائم*)(bdata + y*src2step))[0]
        #تعریف_کن bd(y) ((مزدوج*)(bdata + y*src2step))[0]

        ثابت uchar* srcdata = src.ptr();
        ثابت uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.گام;
        size_t src2step = _src2.گام;
        size_t dststep = dst.گام;

        إذا( src.ردیفها == 2 )
        {
            إذا( type == CV_32FC1 )
            {
                مزدوج d = det2(Sf);
                إذا( d != 0. )
                {
                    مزدوج t;
                    d = 1./d;
                    t = (عائم)(((مزدوج)bf(0)*Sf(1,1) - (مزدوج)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (عائم)(((مزدوج)bf(1)*Sf(0,0) - (مزدوج)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (عائم)t;
                }
                وإلا
                    نتیجه = خطأ;
            }
            وإلا
            {
                مزدوج d = det2(Sd);
                إذا( d != 0. )
                {
                    مزدوج t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                وإلا
                    نتیجه = خطأ;
            }
        }
        وإلا إذا( src.ردیفها == 3 )
        {
            إذا( type == CV_32FC1 )
            {
                مزدوج d = det3(Sf);
                إذا( d != 0. )
                {
                    عائم t[3];
                    d = 1./d;

                    t[0] = (عائم)(d*
                           (bf(0)*((مزدوج)Sf(1,1)*Sf(2,2) - (مزدوج)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((مزدوج)bf(1)*Sf(2,2) - (مزدوج)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((مزدوج)bf(1)*Sf(2,1) - (مزدوج)Sf(1,1)*bf(2))));

                    t[1] = (عائم)(d*
                           (Sf(0,0)*(مزدوج)(bf(1)*Sf(2,2) - (مزدوج)Sf(1,2)*bf(2)) -
                            bf(0)*((مزدوج)Sf(1,0)*Sf(2,2) - (مزدوج)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((مزدوج)Sf(1,0)*bf(2) - (مزدوج)bf(1)*Sf(2,0))));

                    t[2] = (عائم)(d*
                           (Sf(0,0)*((مزدوج)Sf(1,1)*bf(2) - (مزدوج)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((مزدوج)Sf(1,0)*bf(2) - (مزدوج)bf(1)*Sf(2,0)) +
                            bf(0)*((مزدوج)Sf(1,0)*Sf(2,1) - (مزدوج)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                وإلا
                    نتیجه = خطأ;
            }
            وإلا
            {
                مزدوج d = det3(Sd);
                إذا( d != 0. )
                {
                    مزدوج t[9];

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
                وإلا
                    نتیجه = خطأ;
            }
        }
        وإلا
        {
            CV_Assert( src.ردیفها == 1 );

            إذا( type == CV_32FC1 )
            {
                مزدوج d = Sf(0,0);
                إذا( d != 0. )
                    Df(0,0) = (عائم)(bf(0)/d);
                وإلا
                    نتیجه = خطأ;
            }
            وإلا
            {
                مزدوج d = Sd(0,0);
                إذا( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                وإلا
                    نتیجه = خطأ;
            }
        }
        أرجع نتیجه;
    }

    صحيح m = src.ردیفها, m_ = m, n = src.تعداد_ستون, nb = _src2.تعداد_ستون;
    size_t esz = CV_ELEM_اندازه(type), bufsize = 0;
    size_t vstep = align_اندازه(n*esz, 16);
    size_t astep = روش == DECOMP_SVD && !is_نرمال ? align_اندازه(m*esz, 16) : vstep;
    Auto_میانگیر<uchar> میانگیر;

    Mat src2 = _src2;
    _dst.create( src.تعداد_ستون, src2.تعداد_ستون, src.type() );
    Mat dst = _dst.getMat();

    إذا( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    إذا( m == n )
        is_نرمال = خطأ;
    وإلا إذا( is_نرمال )
    {
        m_ = n;
        إذا( روش == DECOMP_SVD )
            روش = DECOMP_EIG;
    }

    CV_Assert(m == src2.ردیفها);
    size_t asize = astep*(روش == DECOMP_SVD || is_نرمال ? n : m);
    bufsize += asize + 32;

    إذا( is_نرمال )
        bufsize += n*nb*esz;
    إذا( روش == DECOMP_SVD || روش == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*حجم(مزدوج) + 32;

    میانگیر.allocate(bufsize);
    uchar* ptr = alignPtr(میانگیر.داده(), 16);

    Mat a(m_, n, type, ptr, astep);

    إذا( is_نرمال )
        mulTransposed(src, a, درست);
    وإلا إذا( روش != DECOMP_SVD )
        src.copy_مقصد(a);
    وإلا
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    إذا( !is_نرمال )
    {
        إذا( روش == DECOMP_LU || روش == DECOMP_CHOLESKY )
            src2.copy_مقصد(dst);
    }
    وإلا
    {
        // a'*b
        إذا( روش == DECOMP_LU || روش == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        وإلا
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    إذا( روش == DECOMP_LU )
    {
        إذا( type == CV_32F )
            نتیجه = hal::LU32f(a.ptr<عائم>(), a.گام, n, dst.ptr<عائم>(), dst.گام, nb) != 0;
        وإلا
            نتیجه = hal::LU64f(a.ptr<مزدوج>(), a.گام, n, dst.ptr<مزدوج>(), dst.گام, nb) != 0;
    }
    وإلا إذا( روش == DECOMP_CHOLESKY )
    {
        إذا( type == CV_32F )
            نتیجه = hal::Cholesky32f(a.ptr<عائم>(), a.گام, n, dst.ptr<عائم>(), dst.گام, nb);
        وإلا
            نتیجه = hal::Cholesky64f(a.ptr<مزدوج>(), a.گام, n, dst.ptr<مزدوج>(), dst.گام, nb);
    }
    وإلا إذا( روش == DECOMP_QR )
    {
        Mat rhsMat;
        إذا( is_نرمال || m == n )
        {
            src2.copy_مقصد(dst);
            rhsMat = dst;
        }
        وإلا
        {
            rhsMat = Mat(m, nb, type);
            src2.copy_مقصد(rhsMat);
        }

        إذا( type == CV_32F )
            نتیجه = hal::QR32f(a.ptr<عائم>(), a.گام, a.ردیفها, a.تعداد_ستون, rhsMat.تعداد_ستون, rhsMat.ptr<عائم>(), rhsMat.گام, عدم) != 0;
        وإلا
            نتیجه = hal::QR64f(a.ptr<مزدوج>(), a.گام, a.ردیفها, a.تعداد_ستون, rhsMat.تعداد_ستون, rhsMat.ptr<مزدوج>(), rhsMat.گام, عدم) != 0;

        إذا (rhsMat.ردیفها != dst.ردیفها)
            rhsMat.ردیف_Range_2(0, dst.ردیفها).copy_مقصد(dst);
    }
    وإلا
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        إذا( روش == DECOMP_EIG )
        {
            إذا( type == CV_32F )
                Jacobi(a.ptr<عائم>(), a.گام, w.ptr<عائم>(), v.ptr<عائم>(), v.گام, n, ptr);
            وإلا
                Jacobi(a.ptr<مزدوج>(), a.گام, w.ptr<مزدوج>(), v.ptr<مزدوج>(), v.گام, n, ptr);
            u = v;
        }
        وإلا
        {
            إذا( type == CV_32F )
                JacobiSVD(a.ptr<عائم>(), a.گام, w.ptr<عائم>(), v.ptr<عائم>(), v.گام, m_, n);
            وإلا
                JacobiSVD(a.ptr<مزدوج>(), a.گام, w.ptr<مزدوج>(), v.ptr<مزدوج>(), v.گام, m_, n);
            u = a;
        }

        إذا( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<عائم>(), 0, u.ptr<عائم>(), u.گام, درست,
                   v.ptr<عائم>(), v.گام, درست, src2.ptr<عائم>(),
                   src2.گام, nb, dst.ptr<عائم>(), dst.گام, ptr);
        }
        وإلا
        {
            SVBkSb(m_, n, w.ptr<مزدوج>(), 0, u.ptr<مزدوج>(), u.گام, درست,
                   v.ptr<مزدوج>(), v.گام, درست, src2.ptr<مزدوج>(),
                   src2.گام, nb, dst.ptr<مزدوج>(), dst.گام, ptr);
        }
        نتیجه = درست;
    }

    إذا( !نتیجه )
        dst = Scalar(0);

    أرجع نتیجه;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

منطقي eigen( درون_داد_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    صحيح type = src.type();
    صحيح n = src.ردیفها;

    CV_Assert( src.ردیفها == src.تعداد_ستون );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    إذا( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#اگر_تعریف_شده HAVE_EIGEN
    ثابت منطقي evecNeeded = _evects.needed();
    ثابت صحيح esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    إذا ( type == CV_64F )
    {
        Eigen::ماتریس_Xd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::ماتریس_Xd> es;
        es.compute(src_eig, esOptions);
        إذا ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            إذا ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            أرجع درست;
        }
    } وإلا { // CV_32F
        Eigen::ماتریس_Xf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::ماتریس_Xf> es;
        es.compute(src_eig, esOptions);
        إذا ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            إذا ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            أرجع درست;
        }
    }
    أرجع خطأ;
#وإلا

    size_t elem_اندازه = src.elem_اندازه(), astep = align_اندازه(n*elem_اندازه, 16);
    Auto_میانگیر<uchar> buf(n*astep + n*5*elem_اندازه + 32);
    uchar* ptr = alignPtr(buf.داده(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elem_اندازه*n;
    src.copy_مقصد(a);
    منطقي ok = type == CV_32F ?
        Jacobi(a.ptr<عائم>(), a.گام, w.ptr<عائم>(), v.ptr<عائم>(), v.گام, n, ptr) :
        Jacobi(a.ptr<مزدوج>(), a.گام, w.ptr<مزدوج>(), v.ptr<مزدوج>(), v.گام, n, ptr);

    w.copy_مقصد(_evals);
    أرجع ok;
#پایان_اگر
}

ساكن فراغ _SVDcompute( درون_داد_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, صحيح flags )
{
    Mat src = _aarr.getMat();
    صحيح m = src.ردیفها, n = src.تعداد_ستون;
    صحيح type = src.type();
    منطقي compute_uv = _u.needed() || _vt.needed();
    منطقي پر_uv = (flags & SVD::پر_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    إذا( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = پر_uv = خطأ;
    }

    منطقي at = خطأ;
    إذا( m < n )
    {
        std::swap(m, n);
        at = درست;
    }

    صحيح urows = پر_uv ? m : n;
    size_t esz = src.elem_اندازه(), astep = align_اندازه(m*esz, 16), vstep = align_اندازه(n*esz, 16);
    Auto_میانگیر<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.داده(), 16);
    Mat موقت_a(n, m, type, buf, astep);
    Mat موقت_w(n, 1, type, buf + urows*astep);
    Mat موقت_u(urows, m, type, buf, astep), موقت_v;

    إذا( compute_uv )
        موقت_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    إذا( urows > n )
        موقت_u = Scalar::all(0);

    إذا( !at )
        transpose(src, موقت_a);
    وإلا
        src.copy_مقصد(موقت_a);

    إذا( type == CV_32F )
    {
        JacobiSVD(موقت_a.ptr<عائم>(), موقت_u.گام, موقت_w.ptr<عائم>(),
              موقت_v.ptr<عائم>(), موقت_v.گام, m, n, compute_uv ? urows : 0);
    }
    وإلا
    {
        JacobiSVD(موقت_a.ptr<مزدوج>(), موقت_u.گام, موقت_w.ptr<مزدوج>(),
              موقت_v.ptr<مزدوج>(), موقت_v.گام, m, n, compute_uv ? urows : 0);
    }
    موقت_w.copy_مقصد(_w);
    إذا( compute_uv )
    {
        إذا( !at )
        {
            إذا( _u.needed() )
                transpose(موقت_u, _u);
            إذا( _vt.needed() )
                موقت_v.copy_مقصد(_vt);
        }
        وإلا
        {
            إذا( _u.needed() )
                transpose(موقت_v, _u);
            إذا( _vt.needed() )
                موقت_u.copy_مقصد(_vt);
        }
    }
}


فراغ SVD::compute( درون_داد_Array a, OutputArray w, OutputArray u, OutputArray vt, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

فراغ SVD::compute( درون_داد_Array a, OutputArray w, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

فراغ SVD::پشت_Subst( درون_داد_Array _w, درون_داد_Array _u, درون_داد_Array _vt,
                     درون_داد_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    صحيح type = w.type(), esz = (صحيح)w.elem_اندازه();
    صحيح m = u.ردیفها, n = vt.تعداد_ستون, nb = rhs.داده ? rhs.تعداد_ستون : m, nm = std::min(m, n);
    size_t wstep = w.ردیفها == 1 ? (size_t)esz : w.تعداد_ستون == 1 ? (size_t)w.گام : (size_t)w.گام + esz;
    Auto_میانگیر<uchar> میانگیر(nb*حجم(مزدوج) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.داده && vt.داده && w.داده );
    CV_Assert( u.تعداد_ستون >= nm && vt.ردیفها >= nm &&
              (w.size() == اندازه(nm, 1) || w.size() == اندازه(1, nm) || w.size() == اندازه(vt.ردیفها, u.تعداد_ستون)) );
    CV_Assert( rhs.داده == 0 || (rhs.type() == type && rhs.ردیفها == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    إذا( type == CV_32F )
        SVBkSb(m, n, w.ptr<عائم>(), wstep, u.ptr<عائم>(), u.گام, خطأ,
               vt.ptr<عائم>(), vt.گام, درست, rhs.ptr<عائم>(), rhs.گام, nb,
               dst.ptr<عائم>(), dst.گام, میانگیر.داده());
    وإلا إذا( type == CV_64F )
        SVBkSb(m, n, w.ptr<مزدوج>(), wstep, u.ptr<مزدوج>(), u.گام, خطأ,
               vt.ptr<مزدوج>(), vt.گام, درست, rhs.ptr<مزدوج>(), rhs.گام, nb,
               dst.ptr<مزدوج>(), dst.گام, میانگیر.داده());
    وإلا
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::عملگر ()(درون_داد_Array a, صحيح flags)
{
    _SVDcompute(a, w, u, vt, flags);
    أرجع *هذا;
}


فراغ SVD::پشت_Subst( درون_داد_Array rhs, OutputArray dst ) ثابت
{
    پشت_Subst( w, u, vt, rhs, dst );
}

}


فراغ cv::SVDecomp(درون_داد_Array src, OutputArray w, OutputArray u, OutputArray vt, صحيح flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

فراغ cv::SVBackSubst(درون_داد_Array w, درون_داد_Array u, درون_داد_Array vt, درون_داد_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::پشت_Subst(w, u, vt, rhs, dst);
}
