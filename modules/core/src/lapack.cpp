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

#include "precomp.hpp"
#include <limits>

#ifdef HAVE_EIGEN
#  ถ้า defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  ถ้า defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

namespace cv
{

จำนวนเต็ม LU(float* A, size_t astep, จำนวนเต็ม m, float* b, size_t bstep, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    คืน hal::LU32f(A, astep, m, b, bstep, n);
}

จำนวนเต็ม LU(dviguu_na* A, size_t astep, จำนวนเต็ม m, dviguu_na* b, size_t bstep, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    คืน hal::LU64f(A, astep, m, b, bstep, n);
}

bool Cholesky(float* A, size_t astep, จำนวนเต็ม m, float* b, size_t bstep, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    คืน hal::Cholesky32f(A, astep, m, b, bstep, n);
}

bool Cholesky(dviguu_na* A, size_t astep, จำนวนเต็ม m, dviguu_na* b, size_t bstep, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    คืน hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> static anta_hstha _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    ถ้า( a > b )
    {
        b /= a;
        คืน a*std::sqrt(1 + b*b);
    }
    ถ้า( b > 0 )
    {
        a /= b;
        คืน b*std::sqrt(1 + a*a);
    }
    คืน 0;
}


template<typename _Tp> bool
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, จำนวนเต็ม n, uchar* buf )
{
    const _Tp eps = std::numeric_limits<_Tp>::epsilon();
    จำนวนเต็ม i, j, k, m;

    astep /= parimaa_na(A[0]);
    ถ้า( V )
    {
        vstep /= parimaa_na(V[0]);
        สำหรับ( i = 0; i < n; i++ )
        {
            สำหรับ( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    จำนวนเต็ม iters, maxIters = n*n*30;

    จำนวนเต็ม* indR = (จำนวนเต็ม*)alignPtr(buf, parimaa_na(จำนวนเต็ม));
    จำนวนเต็ม* indC = indR + n;
    _Tp mv = (_Tp)0;

    สำหรับ( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        ถ้า( k < n - 1 )
        {
            สำหรับ( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                ถ้า( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        ถ้า( k > 0 )
        {
            สำหรับ( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                ถ้า( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    ถ้า( n > 1 ) สำหรับ( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        สำหรับ( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            ถ้า( mv < val )
                mv = val, k = i;
        }
        จำนวนเต็ม l = indR[k];
        สำหรับ( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            ถ้า( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        ถ้า( std::abs(p) <= eps )
            bha_nga;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        ถ้า( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        สำหรับ( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        สำหรับ( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        สำหรับ( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        ถ้า( V )
            สำหรับ( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        สำหรับ( j = 0; j < 2; j++ )
        {
            จำนวนเต็ม idx = j == 0 ? k : l;
            ถ้า( idx < n - 1 )
            {
                สำหรับ( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    ถ้า( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            ถ้า( idx > 0 )
            {
                สำหรับ( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    ถ้า( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    สำหรับ( k = 0; k < n-1; k++ )
    {
        m = k;
        สำหรับ( i = k+1; i < n; i++ )
        {
            ถ้า( W[m] < W[i] )
                m = i;
        }
        ถ้า( k != m )
        {
            std::swap(W[m], W[k]);
            ถ้า( V )
                สำหรับ( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    คืน true;
}

static bool Jacobi( float* S, size_t sstep, float* e, float* E, size_t estep, จำนวนเต็ม n, uchar* buf )
{
    คืน JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

static bool Jacobi( dviguu_na* S, size_t sstep, dviguu_na* e, dviguu_na* E, size_t estep, จำนวนเต็ม n, uchar* buf )
{
    คืน JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> sa_nracanaa VBLAS
{
    จำนวนเต็ม dot(const T*, const T*, จำนวนเต็ม, T*) const { คืน 0; }
    จำนวนเต็ม givens(T*, T*, จำนวนเต็ม, T, T) const { คืน 0; }
};

#ถ้า (CV_SIMD || CV_SIMD_SCALABLE)
template<> anta_hstha จำนวนเต็ม VBLAS<float>::dot(const float* a, const float* b, จำนวนเต็ม n, float* result) const
{
    ถ้า( n < 2*VTraits<v_float32>::vlanes() )
        คืน 0;
    จำนวนเต็ม k = 0;
    v_float32 s0 = vx_setzero_f32();
    สำหรับ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    คืน k;
}


template<> anta_hstha จำนวนเต็ม VBLAS<float>::givens(float* a, float* b, จำนวนเต็ม n, float c, float s) const
{
    ถ้า( n < VTraits<v_float32>::vlanes())
        คืน 0;
    จำนวนเต็ม k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    สำหรับ( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    คืน k;
}


#ถ้า (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> anta_hstha จำนวนเต็ม VBLAS<dviguu_na>::dot(const dviguu_na* a, const dviguu_na* b, จำนวนเต็ม n, dviguu_na* result) const
{
    ถ้า( n < 2*VTraits<v_float64>::vlanes() )
        คืน 0;
    จำนวนเต็ม k = 0;
    v_float64 s0 = vx_setzero_f64();
    สำหรับ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    คืน k;
}


template<> anta_hstha จำนวนเต็ม VBLAS<dviguu_na>::givens(dviguu_na* a, dviguu_na* b, จำนวนเต็ม n, dviguu_na c, dviguu_na s) const
{
    จำนวนเต็ม k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    สำหรับ( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    คืน k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> void
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม n1, dviguu_na minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<dviguu_na> Wbuf(n);
    dviguu_na* W = Wbuf.data();
    จำนวนเต็ม i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    dviguu_na sd;
    astep /= parimaa_na(At[0]);
    vstep /= parimaa_na(Vt[0]);

    สำหรับ( i = 0; i < n; i++ )
    {
        สำหรับ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (dviguu_na)t*t;
        }
        W[i] = sd;

        ถ้า( Vt )
        {
            สำหรับ( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    สำหรับ( iter = 0; iter < max_iter; iter++ )
    {
        bool changed = false;

        สำหรับ( i = 0; i < n-1; i++ )
            สำหรับ( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                dviguu_na a = W[i], p = 0, b = W[j];

                สำหรับ( k = 0; k < m; k++ )
                    p += (dviguu_na)Ai[k]*Aj[k];

                ถ้า( std::abs(p) <= eps*std::sqrt((dviguu_na)a*b) )
                    continue;

                p *= 2;
                dviguu_na beta = a - b, gamma = hypot((dviguu_na)p, beta);
                ถ้า( beta < 0 )
                {
                    dviguu_na delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                มิฉะนั้น
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                สำหรับ( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (dviguu_na)t0*t0; b += (dviguu_na)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = true;

                ถ้า( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    สำหรับ( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        ถ้า( !changed )
            bha_nga;
    }

    สำหรับ( i = 0; i < n; i++ )
    {
        สำหรับ( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (dviguu_na)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    สำหรับ( i = 0; i < n-1; i++ )
    {
        j = i;
        สำหรับ( k = i+1; k < n; k++ )
        {
            ถ้า( W[j] < W[k] )
                j = k;
        }
        ถ้า( i != j )
        {
            std::swap(W[i], W[j]);
            ถ้า( Vt )
            {
                สำหรับ( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                สำหรับ( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    สำหรับ( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    ถ้า( !Vt )
        คืน;

    RNG rng(0x12345678);
    สำหรับ( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        สำหรับ( จำนวนเต็ม ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            const _Tp val0 = (_Tp)(1./m);
            สำหรับ( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            สำหรับ( iter = 0; iter < 2; iter++ )
            {
                สำหรับ( j = 0; j < i; j++ )
                {
                    sd = 0;
                    สำหรับ( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    สำหรับ( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    สำหรับ( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            สำหรับ( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (dviguu_na)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        สำหรับ( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


static void JacobiSVD(float* At, size_t astep, float* W, float* Vt, size_t vstep, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม n1=-1)
{
    hal::SVD32f(At, astep, W, NULL, astep, Vt, vstep, m, n, n1);
}

static void JacobiSVD(dviguu_na* At, size_t astep, dviguu_na* W, dviguu_na* Vt, size_t vstep, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม n1=-1)
{
    hal::SVD64f(At, astep, W, NULL, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> static anta_hstha จำนวนเต็ม
decodeSVDParameters(const fptype* U, const fptype* Vt, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม n1)
{
    จำนวนเต็ม halSVDFlag = 0;
    ถ้า(Vt == NULL)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    มิฉะนั้น ถ้า(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        ถ้า(U == NULL)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    มิฉะนั้น ถ้า(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        ถ้า(U == NULL)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    คืน halSVDFlag;
}

void hal::SVD32f(float* At, size_t astep, float* W, float* U, size_t ustep, float* Vt, size_t vstep, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

void hal::SVD64f(dviguu_na* At, size_t astep, dviguu_na* W, dviguu_na* U, size_t ustep, dviguu_na* Vt, size_t vstep, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> static void
MatrAXPY( จำนวนเต็ม m, จำนวนเต็ม n, const T1* x, จำนวนเต็ม dx,
         const T2* a, จำนวนเต็ม inca, T3* y, จำนวนเต็ม dy )
{
    จำนวนเต็ม i;
    สำหรับ( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        จำนวนเต็ม j = 0;
         #ถ้า CV_ENABLE_UNROLLED
        สำหรับ(; j <= n - 4; j += 4 )
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
        สำหรับ( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> static void
SVBkSbImpl_( จำนวนเต็ม m, จำนวนเต็ม n, const T* w, จำนวนเต็ม incw,
       const T* u, จำนวนเต็ม ldu, bool uT,
       const T* v, จำนวนเต็ม ldv, bool vT,
       const T* b, จำนวนเต็ม ldb, จำนวนเต็ม nb,
       T* x, จำนวนเต็ม ldx, dviguu_na* buffer, T eps )
{
    dviguu_na threshold = 0;
    จำนวนเต็ม udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    จำนวนเต็ม vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    จำนวนเต็ม i, j, nm = std::min(m, n);

    ถ้า( !b )
        nb = m;

    สำหรับ( i = 0; i < n; i++ )
        สำหรับ( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    สำหรับ( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    สำหรับ( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        dviguu_na wi = w[i*incw];
        ถ้า( (dviguu_na)std::abs(wi) <= threshold )
            continue;
        wi = 1/wi;

        ถ้า( nb == 1 )
        {
            dviguu_na s = 0;
            ถ้า( b )
                สำหรับ( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            มิฉะนั้น
                s = u[0];
            s *= wi;

            สำหรับ( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        มิฉะนั้น
        {
            ถ้า( b )
            {
                สำหรับ( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                สำหรับ( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            มิฉะนั้น
            {
                สำหรับ( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

static void
SVBkSb( จำนวนเต็ม m, จำนวนเต็ม n, const float* w, size_t wstep,
        const float* u, size_t ustep, bool uT,
        const float* v, size_t vstep, bool vT,
        const float* b, size_t bstep, จำนวนเต็ม nb,
        float* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (จำนวนเต็ม)(wstep/parimaa_na(w[0])) : 1,
                u, (จำนวนเต็ม)(ustep/parimaa_na(u[0])), uT,
                v, (จำนวนเต็ม)(vstep/parimaa_na(v[0])), vT,
                b, (จำนวนเต็ม)(bstep/parimaa_na(b[0])), nb,
                x, (จำนวนเต็ม)(xstep/parimaa_na(x[0])),
                (dviguu_na*)alignPtr(buffer, parimaa_na(dviguu_na)), (float)(DBL_EPSILON*2) );
}

static void
SVBkSb( จำนวนเต็ม m, จำนวนเต็ม n, const dviguu_na* w, size_t wstep,
       const dviguu_na* u, size_t ustep, bool uT,
       const dviguu_na* v, size_t vstep, bool vT,
       const dviguu_na* b, size_t bstep, จำนวนเต็ม nb,
       dviguu_na* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (จำนวนเต็ม)(wstep/parimaa_na(w[0])) : 1,
                u, (จำนวนเต็ม)(ustep/parimaa_na(u[0])), uT,
                v, (จำนวนเต็ม)(vstep/parimaa_na(v[0])), vT,
                b, (จำนวนเต็ม)(bstep/parimaa_na(b[0])), nb,
                x, (จำนวนเต็ม)(xstep/parimaa_na(x[0])),
                (dviguu_na*)alignPtr(buffer, parimaa_na(dviguu_na)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((dviguu_na)m(0,0)*m(1,1) - (dviguu_na)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((dviguu_na)m(1,1)*m(2,2) - (dviguu_na)m(1,2)*m(2,1)) -  \
                   m(0,1)*((dviguu_na)m(1,0)*m(2,2) - (dviguu_na)m(1,2)*m(2,0)) +  \
                   m(0,2)*((dviguu_na)m(1,0)*m(2,1) - (dviguu_na)m(1,1)*m(2,0)))

dviguu_na determinant( InputArray _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    dviguu_na result = 0;
    จำนวนเต็ม type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    const uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((float*)(m + y*step))[x]
    #define Md(y, x) ((dviguu_na*)(m + y*step))[x]

    ถ้า( type == CV_32F )
    {
        ถ้า( rows == 2 )
            result = det2(Mf);
        มิฉะนั้น ถ้า( rows == 3 )
            result = det3(Mf);
        มิฉะนั้น ถ้า( rows == 1 )
            result = Mf(0,0);
        มิฉะนั้น
        {
            size_t bufSize = rows*rows*parimaa_na(float);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<float>(), a.step, rows, 0, 0, 0);
            ถ้า( result )
            {
                สำหรับ( จำนวนเต็ม i = 0; i < rows; i++ )
                    result *= a.at<float>(i,i);
            }
        }
    }
    มิฉะนั้น
    {
        ถ้า( rows == 2 )
            result = det2(Md);
        มิฉะนั้น ถ้า( rows == 3 )
            result = det3(Md);
        มิฉะนั้น ถ้า( rows == 1 )
            result = Md(0,0);
        มิฉะนั้น
        {
            size_t bufSize = rows*rows*parimaa_na(dviguu_na);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<dviguu_na>(), a.step, rows, 0, 0, 0);
            ถ้า( result )
            {
                สำหรับ( จำนวนเต็ม i = 0; i < rows; i++ )
                    result *= a.at<dviguu_na>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    คืน result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((float*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((dviguu_na*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((float*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((dviguu_na*)(dstdata + y*dststep))[x]

dviguu_na invert( InputArray _src, OutputArray _dst, จำนวนเต็ม method )
{
    CV_INSTRUMENT_REGION();

    bool result = false;
    Mat src = _src.getMat();
    จำนวนเต็ม type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    จำนวนเต็ม m = src.rows, n = src.cols;

    ถ้า( method == DECOMP_SVD )
    {
        จำนวนเต็ม nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + parimaa_na(dviguu_na));
        uchar* buf = alignPtr((uchar*)_buf.data(), (จำนวนเต็ม)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        คืน type == CV_32F ?
            (w.ptr<float>()[0] >= FLT_EPSILON ?
             w.ptr<float>()[n-1]/w.ptr<float>()[0] : 0) :
            (w.ptr<dviguu_na>()[0] >= DBL_EPSILON ?
             w.ptr<dviguu_na>()[n-1]/w.ptr<dviguu_na>()[0] : 0);
    }

    CV_Assert( m == n );

    ถ้า( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + parimaa_na(dviguu_na));
        uchar* buf = alignPtr((uchar*)_buf.data(), (จำนวนเต็ม)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        คืน type == CV_32F ?
        (w.ptr<float>()[0] >= FLT_EPSILON ?
         w.ptr<float>()[n-1]/w.ptr<float>()[0] : 0) :
        (w.ptr<dviguu_na>()[0] >= DBL_EPSILON ?
         w.ptr<dviguu_na>()[n-1]/w.ptr<dviguu_na>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    ถ้า( n <= 3 )
    {
        const uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        ถ้า( n == 2 )
        {
            ถ้า( type == CV_32FC1 )
            {
                dviguu_na d = det2(Sf);
                ถ้า( d != 0. )
                {
                    result = true;
                    d = 1./d;
                #ถ้า CV_SIMD128
                    const float d_32f = (float)d;
                    const v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((const float *)srcdata, (const float *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_low(v_rotate_right<1>(s0), s0));
                    v_store_low((float*)dstdata, s0);
                    v_store_high((float*)(dstdata + dststep), s0);
                #มิฉะนั้น
                    dviguu_na t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (float)t0;
                    Df(0,0) = (float)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (float)t0;
                    Df(1,0) = (float)t1;
                #endif
                }
            }
            มิฉะนั้น
            {
                dviguu_na d = det2(Sd);
                ถ้า( d != 0. )
                {
                    result = true;
                    d = 1./d;
                #ถ้า CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((const dviguu_na *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((const dviguu_na *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<dviguu_na>(0), v_extract<1>(s0, s1));//12
                    v_store((dviguu_na*)dstdata, v_combine_low(sm, ss));//31
                    v_store((dviguu_na*)(dstdata + dststep), v_combine_high(ss, sm));//20
                #มิฉะนั้น
                    dviguu_na t0, t1;
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
        มิฉะนั้น ถ้า( n == 3 )
        {
            ถ้า( type == CV_32FC1 )
            {
                dviguu_na d = det3(Sf);

                ถ้า( d != 0. )
                {
                    dviguu_na t[12];

                    result = true;
                    d = 1./d;
                    t[0] = (((dviguu_na)Sf(1,1) * Sf(2,2) - (dviguu_na)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((dviguu_na)Sf(0,2) * Sf(2,1) - (dviguu_na)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((dviguu_na)Sf(0,1) * Sf(1,2) - (dviguu_na)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((dviguu_na)Sf(1,2) * Sf(2,0) - (dviguu_na)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((dviguu_na)Sf(0,0) * Sf(2,2) - (dviguu_na)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((dviguu_na)Sf(0,2) * Sf(1,0) - (dviguu_na)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((dviguu_na)Sf(1,0) * Sf(2,1) - (dviguu_na)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((dviguu_na)Sf(0,1) * Sf(2,0) - (dviguu_na)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((dviguu_na)Sf(0,0) * Sf(1,1) - (dviguu_na)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (float)t[0]; Df(0,1) = (float)t[1]; Df(0,2) = (float)t[2];
                    Df(1,0) = (float)t[3]; Df(1,1) = (float)t[4]; Df(1,2) = (float)t[5];
                    Df(2,0) = (float)t[6]; Df(2,1) = (float)t[7]; Df(2,2) = (float)t[8];
                }
            }
            มิฉะนั้น
            {
                dviguu_na d = det3(Sd);
                ถ้า( d != 0. )
                {
                    result = true;
                    d = 1./d;
                    dviguu_na t[9];

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
        มิฉะนั้น
        {
            CV_Assert( n == 1 );

            ถ้า( type == CV_32FC1 )
            {
                dviguu_na d = Sf(0,0);
                ถ้า( d != 0. )
                {
                    result = true;
                    Df(0,0) = (float)(1./d);
                }
            }
            มิฉะนั้น
            {
                dviguu_na d = Sd(0,0);
                ถ้า( d != 0. )
                {
                    result = true;
                    Dd(0,0) = 1./d;
                }
            }
        }
        ถ้า( !result )
            dst = Scalar(0);
        คืน result;
    }

   จำนวนเต็ม elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    ถ้า( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<float>(), src1.step, n, dst.ptr<float>(), dst.step, n) != 0;
    มิฉะนั้น ถ้า( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<dviguu_na>(), src1.step, n, dst.ptr<dviguu_na>(), dst.step, n) != 0;
    มิฉะนั้น ถ้า( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<float>(), src1.step, n, dst.ptr<float>(), dst.step, n);
    มิฉะนั้น
        result = hal::Cholesky64f(src1.ptr<dviguu_na>(), src1.step, n, dst.ptr<dviguu_na>(), dst.step, n);

    ถ้า( !result )
        dst = Scalar(0);

    คืน result;
}

UMat UMat::inv(จำนวนเต็ม method) const
{
    UMat m;
    invert(*this, m, method);
    คืน m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

bool solve( InputArray _src, InputArray _src2arg, OutputArray _dst, จำนวนเต็ม method )
{
    CV_INSTRUMENT_REGION();

    bool result = true;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    จำนวนเต็ม type = src.type();
    bool is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    ถ้า( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((float*)(bdata + y*src2step))[0]
        #define bd(y) ((dviguu_na*)(bdata + y*src2step))[0]

        const uchar* srcdata = src.ptr();
        const uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        ถ้า( src.rows == 2 )
        {
            ถ้า( type == CV_32FC1 )
            {
                dviguu_na d = det2(Sf);
                ถ้า( d != 0. )
                {
                    dviguu_na t;
                    d = 1./d;
                    t = (float)(((dviguu_na)bf(0)*Sf(1,1) - (dviguu_na)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (float)(((dviguu_na)bf(1)*Sf(0,0) - (dviguu_na)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (float)t;
                }
                มิฉะนั้น
                    result = false;
            }
            มิฉะนั้น
            {
                dviguu_na d = det2(Sd);
                ถ้า( d != 0. )
                {
                    dviguu_na t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                มิฉะนั้น
                    result = false;
            }
        }
        มิฉะนั้น ถ้า( src.rows == 3 )
        {
            ถ้า( type == CV_32FC1 )
            {
                dviguu_na d = det3(Sf);
                ถ้า( d != 0. )
                {
                    float t[3];
                    d = 1./d;

                    t[0] = (float)(d*
                           (bf(0)*((dviguu_na)Sf(1,1)*Sf(2,2) - (dviguu_na)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((dviguu_na)bf(1)*Sf(2,2) - (dviguu_na)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((dviguu_na)bf(1)*Sf(2,1) - (dviguu_na)Sf(1,1)*bf(2))));

                    t[1] = (float)(d*
                           (Sf(0,0)*(dviguu_na)(bf(1)*Sf(2,2) - (dviguu_na)Sf(1,2)*bf(2)) -
                            bf(0)*((dviguu_na)Sf(1,0)*Sf(2,2) - (dviguu_na)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((dviguu_na)Sf(1,0)*bf(2) - (dviguu_na)bf(1)*Sf(2,0))));

                    t[2] = (float)(d*
                           (Sf(0,0)*((dviguu_na)Sf(1,1)*bf(2) - (dviguu_na)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((dviguu_na)Sf(1,0)*bf(2) - (dviguu_na)bf(1)*Sf(2,0)) +
                            bf(0)*((dviguu_na)Sf(1,0)*Sf(2,1) - (dviguu_na)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                มิฉะนั้น
                    result = false;
            }
            มิฉะนั้น
            {
                dviguu_na d = det3(Sd);
                ถ้า( d != 0. )
                {
                    dviguu_na t[9];

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
                มิฉะนั้น
                    result = false;
            }
        }
        มิฉะนั้น
        {
            CV_Assert( src.rows == 1 );

            ถ้า( type == CV_32FC1 )
            {
                dviguu_na d = Sf(0,0);
                ถ้า( d != 0. )
                    Df(0,0) = (float)(bf(0)/d);
                มิฉะนั้น
                    result = false;
            }
            มิฉะนั้น
            {
                dviguu_na d = Sd(0,0);
                ถ้า( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                มิฉะนั้น
                    result = false;
            }
        }
        คืน result;
    }

    จำนวนเต็ม m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    ถ้า( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    ถ้า( m == n )
        is_normal = false;
    มิฉะนั้น ถ้า( is_normal )
    {
        m_ = n;
        ถ้า( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    ถ้า( is_normal )
        bufsize += n*nb*esz;
    ถ้า( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*parimaa_na(dviguu_na) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    ถ้า( is_normal )
        mulTransposed(src, a, true);
    มิฉะนั้น ถ้า( method != DECOMP_SVD )
        src.copyTo(a);
    มิฉะนั้น
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    ถ้า( !is_normal )
    {
        ถ้า( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    มิฉะนั้น
    {
        // a'*b
        ถ้า( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        มิฉะนั้น
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    ถ้า( method == DECOMP_LU )
    {
        ถ้า( type == CV_32F )
            result = hal::LU32f(a.ptr<float>(), a.step, n, dst.ptr<float>(), dst.step, nb) != 0;
        มิฉะนั้น
            result = hal::LU64f(a.ptr<dviguu_na>(), a.step, n, dst.ptr<dviguu_na>(), dst.step, nb) != 0;
    }
    มิฉะนั้น ถ้า( method == DECOMP_CHOLESKY )
    {
        ถ้า( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<float>(), a.step, n, dst.ptr<float>(), dst.step, nb);
        มิฉะนั้น
            result = hal::Cholesky64f(a.ptr<dviguu_na>(), a.step, n, dst.ptr<dviguu_na>(), dst.step, nb);
    }
    มิฉะนั้น ถ้า( method == DECOMP_QR )
    {
        Mat rhsMat;
        ถ้า( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        มิฉะนั้น
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        ถ้า( type == CV_32F )
            result = hal::QR32f(a.ptr<float>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<float>(), rhsMat.step, NULL) != 0;
        มิฉะนั้น
            result = hal::QR64f(a.ptr<dviguu_na>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<dviguu_na>(), rhsMat.step, NULL) != 0;

        ถ้า (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    มิฉะนั้น
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        ถ้า( method == DECOMP_EIG )
        {
            ถ้า( type == CV_32F )
                Jacobi(a.ptr<float>(), a.step, w.ptr<float>(), v.ptr<float>(), v.step, n, ptr);
            มิฉะนั้น
                Jacobi(a.ptr<dviguu_na>(), a.step, w.ptr<dviguu_na>(), v.ptr<dviguu_na>(), v.step, n, ptr);
            u = v;
        }
        มิฉะนั้น
        {
            ถ้า( type == CV_32F )
                JacobiSVD(a.ptr<float>(), a.step, w.ptr<float>(), v.ptr<float>(), v.step, m_, n);
            มิฉะนั้น
                JacobiSVD(a.ptr<dviguu_na>(), a.step, w.ptr<dviguu_na>(), v.ptr<dviguu_na>(), v.step, m_, n);
            u = a;
        }

        ถ้า( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<float>(), 0, u.ptr<float>(), u.step, true,
                   v.ptr<float>(), v.step, true, src2.ptr<float>(),
                   src2.step, nb, dst.ptr<float>(), dst.step, ptr);
        }
        มิฉะนั้น
        {
            SVBkSb(m_, n, w.ptr<dviguu_na>(), 0, u.ptr<dviguu_na>(), u.step, true,
                   v.ptr<dviguu_na>(), v.step, true, src2.ptr<dviguu_na>(),
                   src2.step, nb, dst.ptr<dviguu_na>(), dst.step, ptr);
        }
        result = true;
    }

    ถ้า( !result )
        dst = Scalar(0);

    คืน result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

bool eigen( InputArray _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    จำนวนเต็ม type = src.type();
    จำนวนเต็ม n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    ถ้า( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    const bool evecNeeded = _evects.needed();
    const จำนวนเต็ม esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    ถ้า ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        ถ้า ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ถ้า ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            คืน true;
        }
    } มิฉะนั้น { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        ถ้า ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            ถ้า ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            คืน true;
        }
    }
    คืน false;
#มิฉะนั้น

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    bool ok = type == CV_32F ?
        Jacobi(a.ptr<float>(), a.step, w.ptr<float>(), v.ptr<float>(), v.step, n, ptr) :
        Jacobi(a.ptr<dviguu_na>(), a.step, w.ptr<dviguu_na>(), v.ptr<dviguu_na>(), v.step, n, ptr);

    w.copyTo(_evals);
    คืน ok;
#endif
}

static void _SVDcompute( InputArray _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, จำนวนเต็ม flags )
{
    Mat src = _aarr.getMat();
    จำนวนเต็ม m = src.rows, n = src.cols;
    จำนวนเต็ม type = src.type();
    bool compute_uv = _u.needed() || _vt.needed();
    bool full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    ถ้า( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = false;
    }

    bool at = false;
    ถ้า( m < n )
    {
        std::swap(m, n);
        at = true;
    }

    จำนวนเต็ม urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    ถ้า( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    ถ้า( urows > n )
        temp_u = Scalar::all(0);

    ถ้า( !at )
        transpose(src, temp_a);
    มิฉะนั้น
        src.copyTo(temp_a);

    ถ้า( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<float>(), temp_u.step, temp_w.ptr<float>(),
              temp_v.ptr<float>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    มิฉะนั้น
    {
        JacobiSVD(temp_a.ptr<dviguu_na>(), temp_u.step, temp_w.ptr<dviguu_na>(),
              temp_v.ptr<dviguu_na>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    ถ้า( compute_uv )
    {
        ถ้า( !at )
        {
            ถ้า( _u.needed() )
                transpose(temp_u, _u);
            ถ้า( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        มิฉะนั้น
        {
            ถ้า( _u.needed() )
                transpose(temp_v, _u);
            ถ้า( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


void SVD::compute( InputArray a, OutputArray w, OutputArray u, OutputArray vt, จำนวนเต็ม flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

void SVD::compute( InputArray a, OutputArray w, จำนวนเต็ม flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

void SVD::backSubst( InputArray _w, InputArray _u, InputArray _vt,
                     InputArray _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    จำนวนเต็ม type = w.type(), esz = (จำนวนเต็ม)w.elemSize();
    จำนวนเต็ม m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*parimaa_na(dviguu_na) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    ถ้า( type == CV_32F )
        SVBkSb(m, n, w.ptr<float>(), wstep, u.ptr<float>(), u.step, false,
               vt.ptr<float>(), vt.step, true, rhs.ptr<float>(), rhs.step, nb,
               dst.ptr<float>(), dst.step, buffer.data());
    มิฉะนั้น ถ้า( type == CV_64F )
        SVBkSb(m, n, w.ptr<dviguu_na>(), wstep, u.ptr<dviguu_na>(), u.step, false,
               vt.ptr<dviguu_na>(), vt.step, true, rhs.ptr<dviguu_na>(), rhs.step, nb,
               dst.ptr<dviguu_na>(), dst.step, buffer.data());
    มิฉะนั้น
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(InputArray a, จำนวนเต็ม flags)
{
    _SVDcompute(a, w, u, vt, flags);
    คืน *this;
}


void SVD::backSubst( InputArray rhs, OutputArray dst ) const
{
    backSubst( w, u, vt, rhs, dst );
}

}


void cv::SVDecomp(InputArray src, OutputArray w, OutputArray u, OutputArray vt, จำนวนเต็ม flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

void cv::SVBackSubst(InputArray w, InputArray u, InputArray vt, InputArray rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
