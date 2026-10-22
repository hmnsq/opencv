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
#  만약 defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  만약 defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

namespace cv
{

정수 LU(float* A, size_t astep, 정수 m, float* b, size_t bstep, 정수 n)
{
    CV_INSTRUMENT_REGION();

    반환 hal::LU32f(A, astep, m, b, bstep, n);
}

정수 LU(double* A, size_t astep, 정수 m, double* b, size_t bstep, 정수 n)
{
    CV_INSTRUMENT_REGION();

    반환 hal::LU64f(A, astep, m, b, bstep, n);
}

bool Cholesky(float* A, size_t astep, 정수 m, float* b, size_t bstep, 정수 n)
{
    CV_INSTRUMENT_REGION();

    반환 hal::Cholesky32f(A, astep, m, b, bstep, n);
}

bool Cholesky(double* A, size_t astep, 정수 m, double* b, size_t bstep, 정수 n)
{
    CV_INSTRUMENT_REGION();

    반환 hal::Cholesky64f(A, astep, m, b, bstep, n);
}

template<typename _Tp> static inline _Tp hypot(_Tp a, _Tp b)
{
    a = std::abs(a);
    b = std::abs(b);
    만약( a > b )
    {
        b /= a;
        반환 a*std::sqrt(1 + b*b);
    }
    만약( b > 0 )
    {
        a /= b;
        반환 b*std::sqrt(1 + a*a);
    }
    반환 0;
}


template<typename _Tp> bool
JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, 정수 n, uchar* buf )
{
    const _Tp eps = std::numeric_limits<_Tp>::epsilon();
    정수 i, j, k, m;

    astep /= sizeof(A[0]);
    만약( V )
    {
        vstep /= sizeof(V[0]);
        위해( i = 0; i < n; i++ )
        {
            위해( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    정수 iters, maxIters = n*n*30;

    정수* indR = (정수*)alignPtr(buf, sizeof(정수));
    정수* indC = indR + n;
    _Tp mv = (_Tp)0;

    위해( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        만약( k < n - 1 )
        {
            위해( m = k+1, mv = std::abs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::abs(A[astep*k+i]);
                만약( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        만약( k > 0 )
        {
            위해( m = 0, mv = std::abs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::abs(A[astep*i+k]);
                만약( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    만약( n > 1 ) 위해( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        위해( k = 0, mv = std::abs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::abs(A[astep*i + indR[i]]);
            만약( mv < val )
                mv = val, k = i;
        }
        정수 l = indR[k];
        위해( i = 1; i < n; i++ )
        {
            _Tp val = std::abs(A[astep*indC[i] + i]);
            만약( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        만약( std::abs(p) <= eps )
            break;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::abs(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        만약( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        위해( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        위해( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        위해( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        만약( V )
            위해( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        위해( j = 0; j < 2; j++ )
        {
            정수 idx = j == 0 ? k : l;
            만약( idx < n - 1 )
            {
                위해( m = idx+1, mv = std::abs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::abs(A[astep*idx+i]);
                    만약( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            만약( idx > 0 )
            {
                위해( m = 0, mv = std::abs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::abs(A[astep*i+idx]);
                    만약( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    위해( k = 0; k < n-1; k++ )
    {
        m = k;
        위해( i = k+1; i < n; i++ )
        {
            만약( W[m] < W[i] )
                m = i;
        }
        만약( k != m )
        {
            std::swap(W[m], W[k]);
            만약( V )
                위해( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    반환 true;
}

static bool Jacobi( float* S, size_t sstep, float* e, float* E, size_t estep, 정수 n, uchar* buf )
{
    반환 JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

static bool Jacobi( double* S, size_t sstep, double* e, double* E, size_t estep, 정수 n, uchar* buf )
{
    반환 JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


template<typename T> struct VBLAS
{
    정수 dot(const T*, const T*, 정수, T*) const { 반환 0; }
    정수 givens(T*, T*, 정수, T, T) const { 반환 0; }
};

#만약 (CV_SIMD || CV_SIMD_SCALABLE)
template<> inline 정수 VBLAS<float>::dot(const float* a, const float* b, 정수 n, float* result) const
{
    만약( n < 2*VTraits<v_float32>::vlanes() )
        반환 0;
    정수 k = 0;
    v_float32 s0 = vx_setzero_f32();
    위해( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    반환 k;
}


template<> inline 정수 VBLAS<float>::givens(float* a, float* b, 정수 n, float c, float s) const
{
    만약( n < VTraits<v_float32>::vlanes())
        반환 0;
    정수 k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    위해( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    반환 k;
}


#만약 (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template<> inline 정수 VBLAS<double>::dot(const double* a, const double* b, 정수 n, double* result) const
{
    만약( n < 2*VTraits<v_float64>::vlanes() )
        반환 0;
    정수 k = 0;
    v_float64 s0 = vx_setzero_f64();
    위해( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    반환 k;
}


template<> inline 정수 VBLAS<double>::givens(double* a, double* b, 정수 n, double c, double s) const
{
    정수 k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    위해( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    반환 k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

template<typename _Tp> void
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               정수 m, 정수 n, 정수 n1, double minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<double> Wbuf(n);
    double* W = Wbuf.data();
    정수 i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    double sd;
    astep /= sizeof(At[0]);
    vstep /= sizeof(Vt[0]);

    위해( i = 0; i < n; i++ )
    {
        위해( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (double)t*t;
        }
        W[i] = sd;

        만약( Vt )
        {
            위해( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    위해( iter = 0; iter < max_iter; iter++ )
    {
        bool changed = false;

        위해( i = 0; i < n-1; i++ )
            위해( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                double a = W[i], p = 0, b = W[j];

                위해( k = 0; k < m; k++ )
                    p += (double)Ai[k]*Aj[k];

                만약( std::abs(p) <= eps*std::sqrt((double)a*b) )
                    continue;

                p *= 2;
                double beta = a - b, gamma = hypot((double)p, beta);
                만약( beta < 0 )
                {
                    double delta = (gamma - beta)*0.5;
                    s = (_Tp)std::sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                아니면
                {
                    c = (_Tp)std::sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                위해( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (double)t0*t0; b += (double)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = true;

                만약( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    위해( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        만약( !changed )
            break;
    }

    위해( i = 0; i < n; i++ )
    {
        위해( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (double)t*t;
        }
        W[i] = std::sqrt(sd);
    }

    위해( i = 0; i < n-1; i++ )
    {
        j = i;
        위해( k = i+1; k < n; k++ )
        {
            만약( W[j] < W[k] )
                j = k;
        }
        만약( i != j )
        {
            std::swap(W[i], W[j]);
            만약( Vt )
            {
                위해( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                위해( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    위해( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    만약( !Vt )
        반환;

    RNG rng(0x12345678);
    위해( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        위해( 정수 ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            const _Tp val0 = (_Tp)(1./m);
            위해( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            위해( iter = 0; iter < 2; iter++ )
            {
                위해( j = 0; j < i; j++ )
                {
                    sd = 0;
                    위해( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    위해( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    위해( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            위해( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (double)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        위해( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


static void JacobiSVD(float* At, size_t astep, float* W, float* Vt, size_t vstep, 정수 m, 정수 n, 정수 n1=-1)
{
    hal::SVD32f(At, astep, W, NULL, astep, Vt, vstep, m, n, n1);
}

static void JacobiSVD(double* At, size_t astep, double* W, double* Vt, size_t vstep, 정수 m, 정수 n, 정수 n1=-1)
{
    hal::SVD64f(At, astep, W, NULL, astep, Vt, vstep, m, n, n1);
}

template <typename fptype> static inline 정수
decodeSVDParameters(const fptype* U, const fptype* Vt, 정수 m, 정수 n, 정수 n1)
{
    정수 halSVDFlag = 0;
    만약(Vt == NULL)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    아니면 만약(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        만약(U == NULL)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    아니면 만약(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        만약(U == NULL)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    반환 halSVDFlag;
}

void hal::SVD32f(float* At, size_t astep, float* W, float* U, size_t ustep, float* Vt, size_t vstep, 정수 m, 정수 n, 정수 n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}

void hal::SVD64f(double* At, size_t astep, double* W, double* U, size_t ustep, double* Vt, size_t vstep, 정수 m, 정수 n, 정수 n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
template<typename T1, typename T2, typename T3> static void
MatrAXPY( 정수 m, 정수 n, const T1* x, 정수 dx,
         const T2* a, 정수 inca, T3* y, 정수 dy )
{
    정수 i;
    위해( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        정수 j = 0;
         #만약 CV_ENABLE_UNROLLED
        위해(; j <= n - 4; j += 4 )
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
        위해( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

template<typename T> static void
SVBkSbImpl_( 정수 m, 정수 n, const T* w, 정수 incw,
       const T* u, 정수 ldu, bool uT,
       const T* v, 정수 ldv, bool vT,
       const T* b, 정수 ldb, 정수 nb,
       T* x, 정수 ldx, double* buffer, T eps )
{
    double threshold = 0;
    정수 udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    정수 vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    정수 i, j, nm = std::min(m, n);

    만약( !b )
        nb = m;

    위해( i = 0; i < n; i++ )
        위해( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    위해( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    위해( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        double wi = w[i*incw];
        만약( (double)std::abs(wi) <= threshold )
            continue;
        wi = 1/wi;

        만약( nb == 1 )
        {
            double s = 0;
            만약( b )
                위해( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            아니면
                s = u[0];
            s *= wi;

            위해( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        아니면
        {
            만약( b )
            {
                위해( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                위해( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            아니면
            {
                위해( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

static void
SVBkSb( 정수 m, 정수 n, const float* w, size_t wstep,
        const float* u, size_t ustep, bool uT,
        const float* v, size_t vstep, bool vT,
        const float* b, size_t bstep, 정수 nb,
        float* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (정수)(wstep/sizeof(w[0])) : 1,
                u, (정수)(ustep/sizeof(u[0])), uT,
                v, (정수)(vstep/sizeof(v[0])), vT,
                b, (정수)(bstep/sizeof(b[0])), nb,
                x, (정수)(xstep/sizeof(x[0])),
                (double*)alignPtr(buffer, sizeof(double)), (float)(DBL_EPSILON*2) );
}

static void
SVBkSb( 정수 m, 정수 n, const double* w, size_t wstep,
       const double* u, size_t ustep, bool uT,
       const double* v, size_t vstep, bool vT,
       const double* b, size_t bstep, 정수 nb,
       double* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (정수)(wstep/sizeof(w[0])) : 1,
                u, (정수)(ustep/sizeof(u[0])), uT,
                v, (정수)(vstep/sizeof(v[0])), vT,
                b, (정수)(bstep/sizeof(b[0])), nb,
                x, (정수)(xstep/sizeof(x[0])),
                (double*)alignPtr(buffer, sizeof(double)), DBL_EPSILON*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((double)m(0,0)*m(1,1) - (double)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((double)m(1,1)*m(2,2) - (double)m(1,2)*m(2,1)) -  \
                   m(0,1)*((double)m(1,0)*m(2,2) - (double)m(1,2)*m(2,0)) +  \
                   m(0,2)*((double)m(1,0)*m(2,1) - (double)m(1,1)*m(2,0)))

double determinant( InputArray _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    double result = 0;
    정수 type = mat.type(), rows = mat.rows;
    size_t step = mat.step;
    const uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((float*)(m + y*step))[x]
    #define Md(y, x) ((double*)(m + y*step))[x]

    만약( type == CV_32F )
    {
        만약( rows == 2 )
            result = det2(Mf);
        아니면 만약( rows == 3 )
            result = det3(Mf);
        아니면 만약( rows == 1 )
            result = Mf(0,0);
        아니면
        {
            size_t bufSize = rows*rows*sizeof(float);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<float>(), a.step, rows, 0, 0, 0);
            만약( result )
            {
                위해( 정수 i = 0; i < rows; i++ )
                    result *= a.at<float>(i,i);
            }
        }
    }
    아니면
    {
        만약( rows == 2 )
            result = det2(Md);
        아니면 만약( rows == 3 )
            result = det3(Md);
        아니면 만약( rows == 1 )
            result = Md(0,0);
        아니면
        {
            size_t bufSize = rows*rows*sizeof(double);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<double>(), a.step, rows, 0, 0, 0);
            만약( result )
            {
                위해( 정수 i = 0; i < rows; i++ )
                    result *= a.at<double>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    반환 result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((float*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((double*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((float*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((double*)(dstdata + y*dststep))[x]

double invert( InputArray _src, OutputArray _dst, 정수 method )
{
    CV_INSTRUMENT_REGION();

    bool result = false;
    Mat src = _src.getMat();
    정수 type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    size_t esz = CV_ELEM_SIZE(type);
    정수 m = src.rows, n = src.cols;

    만약( method == DECOMP_SVD )
    {
        정수 nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + sizeof(double));
        uchar* buf = alignPtr((uchar*)_buf.data(), (정수)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        반환 type == CV_32F ?
            (w.ptr<float>()[0] >= FLT_EPSILON ?
             w.ptr<float>()[n-1]/w.ptr<float>()[0] : 0) :
            (w.ptr<double>()[0] >= DBL_EPSILON ?
             w.ptr<double>()[n-1]/w.ptr<double>()[0] : 0);
    }

    CV_Assert( m == n );

    만약( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + sizeof(double));
        uchar* buf = alignPtr((uchar*)_buf.data(), (정수)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        반환 type == CV_32F ?
        (w.ptr<float>()[0] >= FLT_EPSILON ?
         w.ptr<float>()[n-1]/w.ptr<float>()[0] : 0) :
        (w.ptr<double>()[0] >= DBL_EPSILON ?
         w.ptr<double>()[n-1]/w.ptr<double>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    만약( n <= 3 )
    {
        const uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t dststep = dst.step;

        만약( n == 2 )
        {
            만약( type == CV_32FC1 )
            {
                double d = det2(Sf);
                만약( d != 0. )
                {
                    result = true;
                    d = 1./d;
                #만약 CV_SIMD128
                    const float d_32f = (float)d;
                    const v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((const float *)srcdata, (const float *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_low(v_rotate_right<1>(s0), s0));
                    v_store_low((float*)dstdata, s0);
                    v_store_high((float*)(dstdata + dststep), s0);
                #아니면
                    double t0, t1;
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
            아니면
            {
                double d = det2(Sd);
                만약( d != 0. )
                {
                    result = true;
                    d = 1./d;
                #만약 CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((const double *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((const double *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<double>(0), v_extract<1>(s0, s1));//12
                    v_store((double*)dstdata, v_combine_low(sm, ss));//31
                    v_store((double*)(dstdata + dststep), v_combine_high(ss, sm));//20
                #아니면
                    double t0, t1;
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
        아니면 만약( n == 3 )
        {
            만약( type == CV_32FC1 )
            {
                double d = det3(Sf);

                만약( d != 0. )
                {
                    double t[12];

                    result = true;
                    d = 1./d;
                    t[0] = (((double)Sf(1,1) * Sf(2,2) - (double)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((double)Sf(0,2) * Sf(2,1) - (double)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((double)Sf(0,1) * Sf(1,2) - (double)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((double)Sf(1,2) * Sf(2,0) - (double)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((double)Sf(0,0) * Sf(2,2) - (double)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((double)Sf(0,2) * Sf(1,0) - (double)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((double)Sf(1,0) * Sf(2,1) - (double)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((double)Sf(0,1) * Sf(2,0) - (double)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((double)Sf(0,0) * Sf(1,1) - (double)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (float)t[0]; Df(0,1) = (float)t[1]; Df(0,2) = (float)t[2];
                    Df(1,0) = (float)t[3]; Df(1,1) = (float)t[4]; Df(1,2) = (float)t[5];
                    Df(2,0) = (float)t[6]; Df(2,1) = (float)t[7]; Df(2,2) = (float)t[8];
                }
            }
            아니면
            {
                double d = det3(Sd);
                만약( d != 0. )
                {
                    result = true;
                    d = 1./d;
                    double t[9];

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
        아니면
        {
            CV_Assert( n == 1 );

            만약( type == CV_32FC1 )
            {
                double d = Sf(0,0);
                만약( d != 0. )
                {
                    result = true;
                    Df(0,0) = (float)(1./d);
                }
            }
            아니면
            {
                double d = Sd(0,0);
                만약( d != 0. )
                {
                    result = true;
                    Dd(0,0) = 1./d;
                }
            }
        }
        만약( !result )
            dst = Scalar(0);
        반환 result;
    }

   정수 elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    만약( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<float>(), src1.step, n, dst.ptr<float>(), dst.step, n) != 0;
    아니면 만약( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<double>(), src1.step, n, dst.ptr<double>(), dst.step, n) != 0;
    아니면 만약( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<float>(), src1.step, n, dst.ptr<float>(), dst.step, n);
    아니면
        result = hal::Cholesky64f(src1.ptr<double>(), src1.step, n, dst.ptr<double>(), dst.step, n);

    만약( !result )
        dst = Scalar(0);

    반환 result;
}

UMat UMat::inv(정수 method) const
{
    UMat m;
    invert(*this, m, method);
    반환 m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

bool solve( InputArray _src, InputArray _src2arg, OutputArray _dst, 정수 method )
{
    CV_INSTRUMENT_REGION();

    bool result = true;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    정수 type = src.type();
    bool is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    만약( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((float*)(bdata + y*src2step))[0]
        #define bd(y) ((double*)(bdata + y*src2step))[0]

        const uchar* srcdata = src.ptr();
        const uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        size_t srcstep = src.step;
        size_t src2step = _src2.step;
        size_t dststep = dst.step;

        만약( src.rows == 2 )
        {
            만약( type == CV_32FC1 )
            {
                double d = det2(Sf);
                만약( d != 0. )
                {
                    double t;
                    d = 1./d;
                    t = (float)(((double)bf(0)*Sf(1,1) - (double)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (float)(((double)bf(1)*Sf(0,0) - (double)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (float)t;
                }
                아니면
                    result = false;
            }
            아니면
            {
                double d = det2(Sd);
                만약( d != 0. )
                {
                    double t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                아니면
                    result = false;
            }
        }
        아니면 만약( src.rows == 3 )
        {
            만약( type == CV_32FC1 )
            {
                double d = det3(Sf);
                만약( d != 0. )
                {
                    float t[3];
                    d = 1./d;

                    t[0] = (float)(d*
                           (bf(0)*((double)Sf(1,1)*Sf(2,2) - (double)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((double)bf(1)*Sf(2,2) - (double)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((double)bf(1)*Sf(2,1) - (double)Sf(1,1)*bf(2))));

                    t[1] = (float)(d*
                           (Sf(0,0)*(double)(bf(1)*Sf(2,2) - (double)Sf(1,2)*bf(2)) -
                            bf(0)*((double)Sf(1,0)*Sf(2,2) - (double)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((double)Sf(1,0)*bf(2) - (double)bf(1)*Sf(2,0))));

                    t[2] = (float)(d*
                           (Sf(0,0)*((double)Sf(1,1)*bf(2) - (double)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((double)Sf(1,0)*bf(2) - (double)bf(1)*Sf(2,0)) +
                            bf(0)*((double)Sf(1,0)*Sf(2,1) - (double)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                아니면
                    result = false;
            }
            아니면
            {
                double d = det3(Sd);
                만약( d != 0. )
                {
                    double t[9];

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
                아니면
                    result = false;
            }
        }
        아니면
        {
            CV_Assert( src.rows == 1 );

            만약( type == CV_32FC1 )
            {
                double d = Sf(0,0);
                만약( d != 0. )
                    Df(0,0) = (float)(bf(0)/d);
                아니면
                    result = false;
            }
            아니면
            {
                double d = Sd(0,0);
                만약( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                아니면
                    result = false;
            }
        }
        반환 result;
    }

    정수 m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    size_t esz = CV_ELEM_SIZE(type), bufsize = 0;
    size_t vstep = alignSize(n*esz, 16);
    size_t astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    만약( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    만약( m == n )
        is_normal = false;
    아니면 만약( is_normal )
    {
        m_ = n;
        만약( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    size_t asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    만약( is_normal )
        bufsize += n*nb*esz;
    만약( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*sizeof(double) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    만약( is_normal )
        mulTransposed(src, a, true);
    아니면 만약( method != DECOMP_SVD )
        src.copyTo(a);
    아니면
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    만약( !is_normal )
    {
        만약( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    아니면
    {
        // a'*b
        만약( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        아니면
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    만약( method == DECOMP_LU )
    {
        만약( type == CV_32F )
            result = hal::LU32f(a.ptr<float>(), a.step, n, dst.ptr<float>(), dst.step, nb) != 0;
        아니면
            result = hal::LU64f(a.ptr<double>(), a.step, n, dst.ptr<double>(), dst.step, nb) != 0;
    }
    아니면 만약( method == DECOMP_CHOLESKY )
    {
        만약( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<float>(), a.step, n, dst.ptr<float>(), dst.step, nb);
        아니면
            result = hal::Cholesky64f(a.ptr<double>(), a.step, n, dst.ptr<double>(), dst.step, nb);
    }
    아니면 만약( method == DECOMP_QR )
    {
        Mat rhsMat;
        만약( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        아니면
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        만약( type == CV_32F )
            result = hal::QR32f(a.ptr<float>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<float>(), rhsMat.step, NULL) != 0;
        아니면
            result = hal::QR64f(a.ptr<double>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<double>(), rhsMat.step, NULL) != 0;

        만약 (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    아니면
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        만약( method == DECOMP_EIG )
        {
            만약( type == CV_32F )
                Jacobi(a.ptr<float>(), a.step, w.ptr<float>(), v.ptr<float>(), v.step, n, ptr);
            아니면
                Jacobi(a.ptr<double>(), a.step, w.ptr<double>(), v.ptr<double>(), v.step, n, ptr);
            u = v;
        }
        아니면
        {
            만약( type == CV_32F )
                JacobiSVD(a.ptr<float>(), a.step, w.ptr<float>(), v.ptr<float>(), v.step, m_, n);
            아니면
                JacobiSVD(a.ptr<double>(), a.step, w.ptr<double>(), v.ptr<double>(), v.step, m_, n);
            u = a;
        }

        만약( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<float>(), 0, u.ptr<float>(), u.step, true,
                   v.ptr<float>(), v.step, true, src2.ptr<float>(),
                   src2.step, nb, dst.ptr<float>(), dst.step, ptr);
        }
        아니면
        {
            SVBkSb(m_, n, w.ptr<double>(), 0, u.ptr<double>(), u.step, true,
                   v.ptr<double>(), v.step, true, src2.ptr<double>(),
                   src2.step, nb, dst.ptr<double>(), dst.step, ptr);
        }
        result = true;
    }

    만약( !result )
        dst = Scalar(0);

    반환 result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

bool eigen( InputArray _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    정수 type = src.type();
    정수 n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    만약( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    const bool evecNeeded = _evects.needed();
    const 정수 esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    만약 ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        만약 ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            만약 ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            반환 true;
        }
    } 아니면 { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        만약 ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            만약 ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            반환 true;
        }
    }
    반환 false;
#아니면

    size_t elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    bool ok = type == CV_32F ?
        Jacobi(a.ptr<float>(), a.step, w.ptr<float>(), v.ptr<float>(), v.step, n, ptr) :
        Jacobi(a.ptr<double>(), a.step, w.ptr<double>(), v.ptr<double>(), v.step, n, ptr);

    w.copyTo(_evals);
    반환 ok;
#endif
}

static void _SVDcompute( InputArray _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, 정수 flags )
{
    Mat src = _aarr.getMat();
    정수 m = src.rows, n = src.cols;
    정수 type = src.type();
    bool compute_uv = _u.needed() || _vt.needed();
    bool full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    만약( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = false;
    }

    bool at = false;
    만약( m < n )
    {
        std::swap(m, n);
        at = true;
    }

    정수 urows = full_uv ? m : n;
    size_t esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    만약( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    만약( urows > n )
        temp_u = Scalar::all(0);

    만약( !at )
        transpose(src, temp_a);
    아니면
        src.copyTo(temp_a);

    만약( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<float>(), temp_u.step, temp_w.ptr<float>(),
              temp_v.ptr<float>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    아니면
    {
        JacobiSVD(temp_a.ptr<double>(), temp_u.step, temp_w.ptr<double>(),
              temp_v.ptr<double>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    만약( compute_uv )
    {
        만약( !at )
        {
            만약( _u.needed() )
                transpose(temp_u, _u);
            만약( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        아니면
        {
            만약( _u.needed() )
                transpose(temp_v, _u);
            만약( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


void SVD::compute( InputArray a, OutputArray w, OutputArray u, OutputArray vt, 정수 flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

void SVD::compute( InputArray a, OutputArray w, 정수 flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

void SVD::backSubst( InputArray _w, InputArray _u, InputArray _vt,
                     InputArray _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    정수 type = w.type(), esz = (정수)w.elemSize();
    정수 m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    size_t wstep = w.rows == 1 ? (size_t)esz : w.cols == 1 ? (size_t)w.step : (size_t)w.step + esz;
    AutoBuffer<uchar> buffer(nb*sizeof(double) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    만약( type == CV_32F )
        SVBkSb(m, n, w.ptr<float>(), wstep, u.ptr<float>(), u.step, false,
               vt.ptr<float>(), vt.step, true, rhs.ptr<float>(), rhs.step, nb,
               dst.ptr<float>(), dst.step, buffer.data());
    아니면 만약( type == CV_64F )
        SVBkSb(m, n, w.ptr<double>(), wstep, u.ptr<double>(), u.step, false,
               vt.ptr<double>(), vt.step, true, rhs.ptr<double>(), rhs.step, nb,
               dst.ptr<double>(), dst.step, buffer.data());
    아니면
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::operator ()(InputArray a, 정수 flags)
{
    _SVDcompute(a, w, u, vt, flags);
    반환 *this;
}


void SVD::backSubst( InputArray rhs, OutputArray dst ) const
{
    backSubst( w, u, vt, rhs, dst );
}

}


void cv::SVDecomp(InputArray src, OutputArray w, OutputArray u, OutputArray vt, 정수 flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

void cv::SVBackSubst(InputArray w, InputArray u, InputArray vt, InputArray rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
