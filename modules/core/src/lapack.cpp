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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ <limits>

#ifdef HAVE_EIGEN
#  எனில் defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  எனில் defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

நாமாகாஶ cv
{

முழு LU(தசம* A, size_t astep, முழு m, தசம* b, size_t bstep, முழு n)
{
    CV_INSTRUMENT_REGION();

    திரும்பு hal::LU32f(A, astep, m, b, bstep, n);
}

முழு LU(இரட்டை* A, size_t astep, முழு m, இரட்டை* b, size_t bstep, முழு n)
{
    CV_INSTRUMENT_REGION();

    திரும்பு hal::LU64f(A, astep, m, b, bstep, n);
}

தருக்க Cholesky(தசம* A, size_t astep, முழு m, தசம* b, size_t bstep, முழு n)
{
    CV_INSTRUMENT_REGION();

    திரும்பு hal::Cholesky32f(A, astep, m, b, bstep, n);
}

தருக்க Cholesky(இரட்டை* A, size_t astep, முழு m, இரட்டை* b, size_t bstep, முழு n)
{
    CV_INSTRUMENT_REGION();

    திரும்பு hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> நிலையான உள்வரி _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    எனில்( a > b )
    {
        b /= a;
        திரும்பு a*std::sqrt(1 + b*b);
    }
    எனில்( b > 0 )
    {
        a /= b;
        திரும்பு b*std::sqrt(1 + a*a);
    }
    திரும்பு 0;
}


template<typename _Tp> தருக்க
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, முழு n, uchar* buf )
{
    மாறிலி _Tp eps = std::numeric_limits<_Tp>::epsilon();
    முழு i, j, k, m;

    astep /= அளவு(A[0]);
    எனில்( V )
    {
        vstep /= அளவு(V[0]);
        க்கு( i = 0; i < n; i++ )
        {
            க்கு( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    முழு iters, maxIters = n*n*30;

    முழு* indR = (முழு*)alignPtr(buf, அளவு(முழு));
    முழு* indC = indR + n;
    _Tp mv = (_Tp)0;

    க்கு( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        எனில்( k < n - 1 )
        {
            க்கு( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                எனில்( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        எனில்( k > 0 )
        {
            க்கு( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                எனில்( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    எனில்( n > 1 ) க்கு( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        க்கு( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            எனில்( mv < val )
                mv = val, k = i;
        }
        முழு l = indR[k];
        க்கு( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            எனில்( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        எனில்( std::abs(p) <= eps )
            நிறுத்து;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        எனில்( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        க்கு( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        க்கு( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        க்கு( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        எனில்( V )
            க்கு( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        க்கு( j = 0; j < 2; j++ )
        {
            முழு idx = j == 0 ? k : l;
            எனில்( idx < n - 1 )
            {
                க்கு( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    எனில்( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            எனில்( idx > 0 )
            {
                க்கு( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    எனில்( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    க்கு( k = 0; k < n-1; k++ )
    {
        m = k;
        க்கு( i = k+1; i < n; i++ )
        {
            எனில்( W[m] < W[i] )
                m = i;
        }
        எனில்( k != m )
        {
            std::swap(W[m], W[k]);
            எனில்( V )
                க்கு( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    திரும்பு உண்மை;
}

நிலையான தருக்க Jacobi( தசம* S, size_t sstep, தசம* e, தசம* E, size_t estep, முழு n, uchar* buf )
{
    திரும்பு JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

நிலையான தருக்க Jacobi( இரட்டை* S, size_t sstep, இரட்டை* e, இரட்டை* E, size_t estep, முழு n, uchar* buf )
{
    திரும்பு JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> கட்டமைப்பு VBLAS
{
    முழு dot(மாறிலி T*, மாறிலி T*, முழு, T*) மாறிலி { திரும்பு 0; }
    முழு givens(T*, T*, முழு, T, T) மாறிலி { திரும்பு 0; }
};

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
template<> உள்வரி முழு VBLAS<தசம>::dot(மாறிலி தசம* a, மாறிலி தசம* b, முழு n, தசம* result) மாறிலி
{
    எனில்( n < 2*VTraits<v_float32>::vlanes() )
        திரும்பு 0;
    முழு k = 0;
    v_float32 s0 = vx_setzero_f32();
    க்கு( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    திரும்பு k;
}


template<> உள்வரி முழு VBLAS<தசம>::givens(தசம* a, தசம* b, முழு n, தசம c, தசம s) மாறிலி
{
    எனில்( n < VTraits<v_float32>::vlanes())
        திரும்பு 0;
    முழு k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    க்கு( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    திரும்பு k;
}


#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> உள்வரி முழு VBLAS<இரட்டை>::dot(மாறிலி இரட்டை* a, மாறிலி இரட்டை* b, முழு n, இரட்டை* result) மாறிலி
{
    எனில்( n < 2*VTraits<v_float64>::vlanes() )
        திரும்பு 0;
    முழு k = 0;
    v_float64 s0 = vx_setzero_f64();
    க்கு( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    திரும்பு k;
}


template<> உள்வரி முழு VBLAS<இரட்டை>::givens(இரட்டை* a, இரட்டை* b, முழு n, இரட்டை c, இரட்டை s) மாறிலி
{
    முழு k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    க்கு( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    திரும்பு k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> வெறுமை
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               முழு m, முழு n, முழு n1, இரட்டை minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<இரட்டை> Wbuf(n);
    இரட்டை* W = Wbuf.data();
    முழு i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    இரட்டை sd;
    astep /= அளவு(At[0]);
    vstep /= அளவு(Vt[0]);

    க்கு( i = 0; i < n; i++ )
    {
        க்கு( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (இரட்டை)t*t;
        }
        W[i] = sd;

        எனில்( Vt )
        {
            க்கு( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    க்கு( iter = 0; iter < max_iter; iter++ )
    {
        தருக்க changed = பொய்;

        க்கு( i = 0; i < n-1; i++ )
            க்கு( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                இரட்டை a = W[i], p = 0, b = W[j];

                க்கு( k = 0; k < m; k++ )
                    p += (இரட்டை)Ai[k]*Aj[k];

                எனில்( std::abs(p) <= eps*std::sqrt((இரட்டை)a*b) )
                    தொடர்;

                p *= 2;
                இரட்டை beta = a - b, gamma = hypot((இரட்டை)p, beta);
                எனில்( beta < 0 )
                {
                    இரட்டை delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                இல்லையெனில்
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                க்கு( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (இரட்டை)t0*t0; b += (இரட்டை)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = உண்மை;

                எனில்( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    க்கு( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        எனில்( !changed )
            நிறுத்து;
    }

    க்கு( i = 0; i < n; i++ )
    {
        க்கு( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (இரட்டை)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    க்கு( i = 0; i < n-1; i++ )
    {
        j = i;
        க்கு( k = i+1; k < n; k++ )
        {
            எனில்( W[j] < W[k] )
                j = k;
        }
        எனில்( i != j )
        {
            std::swap(W[i], W[j]);
            எனில்( Vt )
            {
                க்கு( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                க்கு( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    க்கு( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    எனில்( !Vt )
        திரும்பு;

    RNG rng(0x12345678);
    க்கு( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        க்கு( முழு ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            மாறிலி _Tp val0 = (_Tp)(1./m);
            க்கு( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            க்கு( iter = 0; iter < 2; iter++ )
            {
                க்கு( j = 0; j < i; j++ )
                {
                    sd = 0;
                    க்கு( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    க்கு( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    க்கு( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            க்கு( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (இரட்டை)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        க்கு( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


நிலையான வெறுமை JacobiSVD(தசம* At, size_t astep, தசம* W, தசம* Vt, size_t vstep, முழு m, முழு n, முழு n1=-1)
{
    hal::SVD32f(At, astep, W, வெற்று, astep, Vt, vstep, m, n, n1);
}

நிலையான வெறுமை JacobiSVD(இரட்டை* At, size_t astep, இரட்டை* W, இரட்டை* Vt, size_t vstep, முழு m, முழு n, முழு n1=-1)
{
    hal::SVD64f(At, astep, W, வெற்று, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> நிலையான உள்வரி முழு
decodeSVDParameters(மாறிலி fptype* U, மாறிலி fptype* Vt, முழு m, முழு n, முழு n1)
{
    முழு halSVDFlag = 0;
    எனில்(Vt == வெற்று)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    இல்லையெனில் எனில்(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        எனில்(U == வெற்று)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    இல்லையெனில் எனில்(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        எனில்(U == வெற்று)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    திரும்பு halSVDFlag;
}

வெறுமை hal::SVD32f(தசம* At, size_t astep, தசம* W, தசம* U, size_t ustep, தசம* Vt, size_t vstep, முழு m, முழு n, முழு n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

வெறுமை hal::SVD64f(இரட்டை* At, size_t astep, இரட்டை* W, இரட்டை* U, size_t ustep, இரட்டை* Vt, size_t vstep, முழு m, முழு n, முழு n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> நிலையான வெறுமை
MatrAXPY( முழு m, முழு n, மாறிலி T1* x, முழு dx,
         மாறிலி T2* a, முழு inca, T3* y, முழு dy )
{
    முழு i;
    க்கு( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        முழு j = 0;
         #எனில் CV_ENABLE_UNROLLED
        க்கு(; j <= n - 4; j += 4 )
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
        க்கு( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> நிலையான வெறுமை
SVBkSbImpl_( முழு m, முழு n, மாறிலி T* w, முழு incw,
       மாறிலி T* u, முழு ldu, தருக்க uT,
       மாறிலி T* v, முழு ldv, தருக்க vT,
       மாறிலி T* b, முழு ldb, முழு nb,
       T* x, முழு ldx, இரட்டை* buffer, T eps )
{
    இரட்டை threshold = 0;
    முழு udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    முழு vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    முழு i, j, nm = std::min(m, n);

    எனில்( !b )
        nb = m;

    க்கு( i = 0; i < n; i++ )
        க்கு( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    க்கு( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    க்கு( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        இரட்டை wi = w[i*incw];
        எனில்( (இரட்டை)std::abs(wi) <= threshold )
            தொடர்;
        wi = 1/wi;

        எனில்( nb == 1 )
        {
            இரட்டை s = 0;
            எனில்( b )
                க்கு( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            இல்லையெனில்
                s = u[0];
            s *= wi;

            க்கு( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        இல்லையெனில்
        {
            எனில்( b )
            {
                க்கு( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                க்கு( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            இல்லையெனில்
            {
                க்கு( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

நிலையான வெறுமை
SVBkSb( முழு m, முழு n, மாறிலி தசம* w, size_t wstep,
        மாறிலி தசம* u, size_t ustep, தருக்க uT,
        மாறிலி தசம* v, size_t vstep, தருக்க vT,
        மாறிலி தசம* b, size_t bstep, முழு nb,
        தசம* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (முழு)(wstep/அளவு(w[0])) : 1,
                u, (முழு)(ustep/அளவு(u[0])), uT,
                v, (முழு)(vstep/அளவு(v[0])), vT,
                b, (முழு)(bstep/அளவு(b[0])), nb,
                x, (முழு)(xstep/அளவு(x[0])),
                (இரட்டை*)alignPtr(buffer, அளவு(இரட்டை)), (தசம)(DBL_EPSILON*2) );
}

நிலையான வெறுமை
SVBkSb( முழு m, முழு n, மாறிலி இரட்டை* w, size_t wstep,
       மாறிலி இரட்டை* u, size_t ustep, தருக்க uT,
       மாறிலி இரட்டை* v, size_t vstep, தருக்க vT,
       மாறிலி இரட்டை* b, size_t bstep, முழு nb,
       இரட்டை* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (முழு)(wstep/அளவு(w[0])) : 1,
                u, (முழு)(ustep/அளவு(u[0])), uT,
                v, (முழு)(vstep/அளவு(v[0])), vT,
                b, (முழு)(bstep/அளவு(b[0])), nb,
                x, (முழு)(xstep/அளவு(x[0])),
                (இரட்டை*)alignPtr(buffer, அளவு(இரட்டை)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((இரட்டை)m(0,0)*m(1,1) - (இரட்டை)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((இரட்டை)m(1,1)*m(2,2) - (இரட்டை)m(1,2)*m(2,1)) -  \
                   m(0,1)*((இரட்டை)m(1,0)*m(2,2) - (இரட்டை)m(1,2)*m(2,0)) +  \
                   m(0,2)*((இரட்டை)m(1,0)*m(2,1) - (இரட்டை)m(1,1)*m(2,0)))

இரட்டை determinant( உள்ளீடு_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    இரட்டை result = 0;
    முழு type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    மாறிலி uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((தசம*)(m + y*step))[x]
    #define Md(y, x) ((இரட்டை*)(m + y*step))[x]

    எனில்( type == CV_32F )
    {
        எனில்( rows == 2 )
            result = det2(Mf);
        இல்லையெனில் எனில்( rows == 3 )
            result = det3(Mf);
        இல்லையெனில் எனில்( rows == 1 )
            result = Mf(0,0);
        இல்லையெனில்
        {
            size_t bufSize = rows*rows*அளவு(தசம);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<தசம>(), a.step, rows, 0, 0, 0);
            எனில்( result )
            {
                க்கு( முழு i = 0; i < rows; i++ )
                    result *= a.at<தசம>(i,i);
            }
        }
    }
    இல்லையெனில்
    {
        எனில்( rows == 2 )
            result = det2(Md);
        இல்லையெனில் எனில்( rows == 3 )
            result = det3(Md);
        இல்லையெனில் எனில்( rows == 1 )
            result = Md(0,0);
        இல்லையெனில்
        {
            size_t bufSize = rows*rows*அளவு(இரட்டை);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<இரட்டை>(), a.step, rows, 0, 0, 0);
            எனில்( result )
            {
                க்கு( முழு i = 0; i < rows; i++ )
                    result *= a.at<இரட்டை>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    திரும்பு result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((தசம*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((இரட்டை*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((தசம*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((இரட்டை*)(dstdata + y*dststep))[x]

இரட்டை invert( உள்ளீடு_Array _src, OutputArray _dst, முழு method )
{
    CV_INSTRUMENT_REGION();

    தருக்க result = பொய்;
    Mat src = _src.getMat();
    முழு type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    முழு m = src.rows, n = src.cols;

    எனில்( method == DECOMP_SVD )
    {
        முழு nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + அளவு(இரட்டை));
        uchar* buf = alignPtr((uchar*)_buf.data(), (முழு)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        திரும்பு type == CV_32F ?
            (w.ptr<தசம>()[0] >= FLT_EPSILON ?
             w.ptr<தசம>()[n-1]/w.ptr<தசம>()[0] : 0) :
            (w.ptr<இரட்டை>()[0] >= DBL_EPSILON ?
             w.ptr<இரட்டை>()[n-1]/w.ptr<இரட்டை>()[0] : 0);
    }

    CV_Assert( m == n );

    எனில்( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + அளவு(இரட்டை));
        uchar* buf = alignPtr((uchar*)_buf.data(), (முழு)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        திரும்பு type == CV_32F ?
        (w.ptr<தசம>()[0] >= FLT_EPSILON ?
         w.ptr<தசம>()[n-1]/w.ptr<தசம>()[0] : 0) :
        (w.ptr<இரட்டை>()[0] >= DBL_EPSILON ?
         w.ptr<இரட்டை>()[n-1]/w.ptr<இரட்டை>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    எனில்( n <= 3 )
    {
        மாறிலி uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        எனில்( n == 2 )
        {
            எனில்( type == CV_32FC1 )
            {
                இரட்டை d = det2(Sf);
                எனில்( d != 0. )
                {
                    result = உண்மை;
                    d = 1./d;
                #எனில் CV_SIMD128
                    மாறிலி தசம d_32f = (தசம)d;
                    மாறிலி v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((மாறிலி தசம *)srcdata, (மாறிலி தசம *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_கீழ்(v_rotate_வலது<1>(s0), s0));
                    v_store_கீழ்((தசம*)dstdata, s0);
                    v_store_உயர்வு((தசம*)(dstdata + dststep), s0);
                #இல்லையெனில்
                    இரட்டை t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (தசம)t0;
                    Df(0,0) = (தசம)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (தசம)t0;
                    Df(1,0) = (தசம)t1;
                #endif
                }
            }
            இல்லையெனில்
            {
                இரட்டை d = det2(Sd);
                எனில்( d != 0. )
                {
                    result = உண்மை;
                    d = 1./d;
                #எனில் CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((மாறிலி இரட்டை *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((மாறிலி இரட்டை *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<இரட்டை>(0), v_extract<1>(s0, s1));//12
                    v_store((இரட்டை*)dstdata, v_combine_கீழ்(sm, ss));//31
                    v_store((இரட்டை*)(dstdata + dststep), v_combine_உயர்வு(ss, sm));//20
                #இல்லையெனில்
                    இரட்டை t0, t1;
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
        இல்லையெனில் எனில்( n == 3 )
        {
            எனில்( type == CV_32FC1 )
            {
                இரட்டை d = det3(Sf);

                எனில்( d != 0. )
                {
                    இரட்டை t[12];

                    result = உண்மை;
                    d = 1./d;
                    t[0] = (((இரட்டை)Sf(1,1) * Sf(2,2) - (இரட்டை)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((இரட்டை)Sf(0,2) * Sf(2,1) - (இரட்டை)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((இரட்டை)Sf(0,1) * Sf(1,2) - (இரட்டை)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((இரட்டை)Sf(1,2) * Sf(2,0) - (இரட்டை)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((இரட்டை)Sf(0,0) * Sf(2,2) - (இரட்டை)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((இரட்டை)Sf(0,2) * Sf(1,0) - (இரட்டை)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((இரட்டை)Sf(1,0) * Sf(2,1) - (இரட்டை)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((இரட்டை)Sf(0,1) * Sf(2,0) - (இரட்டை)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((இரட்டை)Sf(0,0) * Sf(1,1) - (இரட்டை)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (தசம)t[0]; Df(0,1) = (தசம)t[1]; Df(0,2) = (தசம)t[2];
                    Df(1,0) = (தசம)t[3]; Df(1,1) = (தசம)t[4]; Df(1,2) = (தசம)t[5];
                    Df(2,0) = (தசம)t[6]; Df(2,1) = (தசம)t[7]; Df(2,2) = (தசம)t[8];
                }
            }
            இல்லையெனில்
            {
                இரட்டை d = det3(Sd);
                எனில்( d != 0. )
                {
                    result = உண்மை;
                    d = 1./d;
                    இரட்டை t[9];

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
        இல்லையெனில்
        {
            CV_Assert( n == 1 );

            எனில்( type == CV_32FC1 )
            {
                இரட்டை d = Sf(0,0);
                எனில்( d != 0. )
                {
                    result = உண்மை;
                    Df(0,0) = (தசம)(1./d);
                }
            }
            இல்லையெனில்
            {
                இரட்டை d = Sd(0,0);
                எனில்( d != 0. )
                {
                    result = உண்மை;
                    Dd(0,0) = 1./d;
                }
            }
        }
        எனில்( !result )
            dst = Scalar(0);
        திரும்பு result;
    }

   முழு elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    எனில்( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<தசம>(), src1.step, n, dst.ptr<தசம>(), dst.step, n) != 0;
    இல்லையெனில் எனில்( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<இரட்டை>(), src1.step, n, dst.ptr<இரட்டை>(), dst.step, n) != 0;
    இல்லையெனில் எனில்( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<தசம>(), src1.step, n, dst.ptr<தசம>(), dst.step, n);
    இல்லையெனில்
        result = hal::Cholesky64f(src1.ptr<இரட்டை>(), src1.step, n, dst.ptr<இரட்டை>(), dst.step, n);

    எனில்( !result )
        dst = Scalar(0);

    திரும்பு result;
}

UMat UMat::inv(முழு method) மாறிலி
{
    UMat m;
    invert(*இது, m, method);
    திரும்பு m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

தருக்க solve( உள்ளீடு_Array _src, உள்ளீடு_Array _src2arg, OutputArray _dst, முழு method )
{
    CV_INSTRUMENT_REGION();

    தருக்க result = உண்மை;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    முழு type = src.type();
    தருக்க is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    எனில்( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((தசம*)(bdata + y*src2step))[0]
        #define bd(y) ((இரட்டை*)(bdata + y*src2step))[0]

        மாறிலி uchar* srcdata = src.ptr();
        மாறிலி uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        எனில்( src.rows == 2 )
        {
            எனில்( type == CV_32FC1 )
            {
                இரட்டை d = det2(Sf);
                எனில்( d != 0. )
                {
                    இரட்டை t;
                    d = 1./d;
                    t = (தசம)(((இரட்டை)bf(0)*Sf(1,1) - (இரட்டை)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (தசம)(((இரட்டை)bf(1)*Sf(0,0) - (இரட்டை)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (தசம)t;
                }
                இல்லையெனில்
                    result = பொய்;
            }
            இல்லையெனில்
            {
                இரட்டை d = det2(Sd);
                எனில்( d != 0. )
                {
                    இரட்டை t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                இல்லையெனில்
                    result = பொய்;
            }
        }
        இல்லையெனில் எனில்( src.rows == 3 )
        {
            எனில்( type == CV_32FC1 )
            {
                இரட்டை d = det3(Sf);
                எனில்( d != 0. )
                {
                    தசம t[3];
                    d = 1./d;

                    t[0] = (தசம)(d*
                           (bf(0)*((இரட்டை)Sf(1,1)*Sf(2,2) - (இரட்டை)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((இரட்டை)bf(1)*Sf(2,2) - (இரட்டை)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((இரட்டை)bf(1)*Sf(2,1) - (இரட்டை)Sf(1,1)*bf(2))));

                    t[1] = (தசம)(d*
                           (Sf(0,0)*(இரட்டை)(bf(1)*Sf(2,2) - (இரட்டை)Sf(1,2)*bf(2)) -
                            bf(0)*((இரட்டை)Sf(1,0)*Sf(2,2) - (இரட்டை)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((இரட்டை)Sf(1,0)*bf(2) - (இரட்டை)bf(1)*Sf(2,0))));

                    t[2] = (தசம)(d*
                           (Sf(0,0)*((இரட்டை)Sf(1,1)*bf(2) - (இரட்டை)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((இரட்டை)Sf(1,0)*bf(2) - (இரட்டை)bf(1)*Sf(2,0)) +
                            bf(0)*((இரட்டை)Sf(1,0)*Sf(2,1) - (இரட்டை)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                இல்லையெனில்
                    result = பொய்;
            }
            இல்லையெனில்
            {
                இரட்டை d = det3(Sd);
                எனில்( d != 0. )
                {
                    இரட்டை t[9];

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
                இல்லையெனில்
                    result = பொய்;
            }
        }
        இல்லையெனில்
        {
            CV_Assert( src.rows == 1 );

            எனில்( type == CV_32FC1 )
            {
                இரட்டை d = Sf(0,0);
                எனில்( d != 0. )
                    Df(0,0) = (தசம)(bf(0)/d);
                இல்லையெனில்
                    result = பொய்;
            }
            இல்லையெனில்
            {
                இரட்டை d = Sd(0,0);
                எனில்( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                இல்லையெனில்
                    result = பொய்;
            }
        }
        திரும்பு result;
    }

    முழு m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    எனில்( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    எனில்( m == n )
        is_normal = பொய்;
    இல்லையெனில் எனில்( is_normal )
    {
        m_ = n;
        எனில்( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    எனில்( is_normal )
        bufsize += n*nb*esz;
    எனில்( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*அளவு(இரட்டை) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    எனில்( is_normal )
        mulTransposed(src, a, உண்மை);
    இல்லையெனில் எனில்( method != DECOMP_SVD )
        src.copyTo(a);
    இல்லையெனில்
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    எனில்( !is_normal )
    {
        எனில்( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    இல்லையெனில்
    {
        // a'*b
        எனில்( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        இல்லையெனில்
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    எனில்( method == DECOMP_LU )
    {
        எனில்( type == CV_32F )
            result = hal::LU32f(a.ptr<தசம>(), a.step, n, dst.ptr<தசம>(), dst.step, nb) != 0;
        இல்லையெனில்
            result = hal::LU64f(a.ptr<இரட்டை>(), a.step, n, dst.ptr<இரட்டை>(), dst.step, nb) != 0;
    }
    இல்லையெனில் எனில்( method == DECOMP_CHOLESKY )
    {
        எனில்( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<தசம>(), a.step, n, dst.ptr<தசம>(), dst.step, nb);
        இல்லையெனில்
            result = hal::Cholesky64f(a.ptr<இரட்டை>(), a.step, n, dst.ptr<இரட்டை>(), dst.step, nb);
    }
    இல்லையெனில் எனில்( method == DECOMP_QR )
    {
        Mat rhsMat;
        எனில்( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        இல்லையெனில்
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        எனில்( type == CV_32F )
            result = hal::QR32f(a.ptr<தசம>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<தசம>(), rhsMat.step, வெற்று) != 0;
        இல்லையெனில்
            result = hal::QR64f(a.ptr<இரட்டை>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<இரட்டை>(), rhsMat.step, வெற்று) != 0;

        எனில் (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    இல்லையெனில்
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        எனில்( method == DECOMP_EIG )
        {
            எனில்( type == CV_32F )
                Jacobi(a.ptr<தசம>(), a.step, w.ptr<தசம>(), v.ptr<தசம>(), v.step, n, ptr);
            இல்லையெனில்
                Jacobi(a.ptr<இரட்டை>(), a.step, w.ptr<இரட்டை>(), v.ptr<இரட்டை>(), v.step, n, ptr);
            u = v;
        }
        இல்லையெனில்
        {
            எனில்( type == CV_32F )
                JacobiSVD(a.ptr<தசம>(), a.step, w.ptr<தசம>(), v.ptr<தசம>(), v.step, m_, n);
            இல்லையெனில்
                JacobiSVD(a.ptr<இரட்டை>(), a.step, w.ptr<இரட்டை>(), v.ptr<இரட்டை>(), v.step, m_, n);
            u = a;
        }

        எனில்( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<தசம>(), 0, u.ptr<தசம>(), u.step, உண்மை,
                   v.ptr<தசம>(), v.step, உண்மை, src2.ptr<தசம>(),
                   src2.step, nb, dst.ptr<தசம>(), dst.step, ptr);
        }
        இல்லையெனில்
        {
            SVBkSb(m_, n, w.ptr<இரட்டை>(), 0, u.ptr<இரட்டை>(), u.step, உண்மை,
                   v.ptr<இரட்டை>(), v.step, உண்மை, src2.ptr<இரட்டை>(),
                   src2.step, nb, dst.ptr<இரட்டை>(), dst.step, ptr);
        }
        result = உண்மை;
    }

    எனில்( !result )
        dst = Scalar(0);

    திரும்பு result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

தருக்க eigen( உள்ளீடு_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    முழு type = src.type();
    முழு n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    எனில்( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    மாறிலி தருக்க evecNeeded = _evects.needed();
    மாறிலி முழு esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    எனில் ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        எனில் ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            எனில் ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            திரும்பு உண்மை;
        }
    } இல்லையெனில் { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        எனில் ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            எனில் ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            திரும்பு உண்மை;
        }
    }
    திரும்பு பொய்;
#இல்லையெனில்

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    தருக்க ok = type == CV_32F ?
        Jacobi(a.ptr<தசம>(), a.step, w.ptr<தசம>(), v.ptr<தசம>(), v.step, n, ptr) :
        Jacobi(a.ptr<இரட்டை>(), a.step, w.ptr<இரட்டை>(), v.ptr<இரட்டை>(), v.step, n, ptr);

    w.copyTo(_evals);
    திரும்பு ok;
#endif
}

நிலையான வெறுமை _SVDcompute( உள்ளீடு_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, முழு flags )
{
    Mat src = _aarr.getMat();
    முழு m = src.rows, n = src.cols;
    முழு type = src.type();
    தருக்க compute_uv = _u.needed() || _vt.needed();
    தருக்க full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    எனில்( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = பொய்;
    }

    தருக்க at = பொய்;
    எனில்( m < n )
    {
        std::swap(m, n);
        at = உண்மை;
    }

    முழு urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    எனில்( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    எனில்( urows > n )
        temp_u = Scalar::all(0);

    எனில்( !at )
        transpose(src, temp_a);
    இல்லையெனில்
        src.copyTo(temp_a);

    எனில்( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<தசம>(), temp_u.step, temp_w.ptr<தசம>(),
              temp_v.ptr<தசம>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    இல்லையெனில்
    {
        JacobiSVD(temp_a.ptr<இரட்டை>(), temp_u.step, temp_w.ptr<இரட்டை>(),
              temp_v.ptr<இரட்டை>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    எனில்( compute_uv )
    {
        எனில்( !at )
        {
            எனில்( _u.needed() )
                transpose(temp_u, _u);
            எனில்( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        இல்லையெனில்
        {
            எனில்( _u.needed() )
                transpose(temp_v, _u);
            எனில்( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


வெறுமை SVD::compute( உள்ளீடு_Array a, OutputArray w, OutputArray u, OutputArray vt, முழு flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

வெறுமை SVD::compute( உள்ளீடு_Array a, OutputArray w, முழு flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

வெறுமை SVD::backSubst( உள்ளீடு_Array _w, உள்ளீடு_Array _u, உள்ளீடு_Array _vt,
                     உள்ளீடு_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    முழு type = w.type(), esz = (முழு)w.elemSize();
    முழு m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*அளவு(இரட்டை) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    எனில்( type == CV_32F )
        SVBkSb(m, n, w.ptr<தசம>(), wstep, u.ptr<தசம>(), u.step, பொய்,
               vt.ptr<தசம>(), vt.step, உண்மை, rhs.ptr<தசம>(), rhs.step, nb,
               dst.ptr<தசம>(), dst.step, buffer.data());
    இல்லையெனில் எனில்( type == CV_64F )
        SVBkSb(m, n, w.ptr<இரட்டை>(), wstep, u.ptr<இரட்டை>(), u.step, பொய்,
               vt.ptr<இரட்டை>(), vt.step, உண்மை, rhs.ptr<இரட்டை>(), rhs.step, nb,
               dst.ptr<இரட்டை>(), dst.step, buffer.data());
    இல்லையெனில்
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(உள்ளீடு_Array a, முழு flags)
{
    _SVDcompute(a, w, u, vt, flags);
    திரும்பு *இது;
}


வெறுமை SVD::backSubst( உள்ளீடு_Array rhs, OutputArray dst ) மாறிலி
{
    backSubst( w, u, vt, rhs, dst );
}

}


வெறுமை cv::SVDecomp(உள்ளீடு_Array src, OutputArray w, OutputArray u, OutputArray vt, முழு flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

வெறுமை cv::SVBackSubst(உள்ளீடு_Array w, உள்ளீடு_Array u, உள்ளீடு_Array vt, உள்ளீடு_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
