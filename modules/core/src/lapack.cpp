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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <limits>

#ifdef HAVE_EIGEN
#  जर defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  जर defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

नामाकाश cv
{

पूर्णांक LU(दशांश* A, माप_प्रकार astep, पूर्णांक m, दशांश* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    परत hal::LU32f(A, astep, m, b, bstep, n);
}

पूर्णांक LU(द्विगुण* A, माप_प्रकार astep, पूर्णांक m, द्विगुण* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    परत hal::LU64f(A, astep, m, b, bstep, n);
}

तार्किक Cholesky(दशांश* A, माप_प्रकार astep, पूर्णांक m, दशांश* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    परत hal::Cholesky32f(A, astep, m, b, bstep, n);
}

तार्किक Cholesky(द्विगुण* A, माप_प्रकार astep, पूर्णांक m, द्विगुण* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    परत hal::Cholesky64f(A, astep, m, b, bstep, n);
}

ढाँचा<typename _Tp> स्थिर ओळीत _Tp hypot(_Tp a, _Tp b)
{
    a = std::असल(a);
    b = std::असल(b);
    जर( a > b )
    {
        b /= a;
        परत a*std::वर्ग_मूल(1 + b*b);
    }
    जर( b > 0 )
    {
        a /= b;
        परत b*std::वर्ग_मूल(1 + a*a);
    }
    परत 0;
}


ढाँचा<typename _Tp> तार्किक
JacobiImpl_( _Tp* A, माप_प्रकार astep, _Tp* W, _Tp* V, माप_प्रकार vstep, पूर्णांक n, uchar* buf )
{
    स्थिरांक _Tp eps = std::numeric_limits<_Tp>::epsilon();
    पूर्णांक i, j, k, m;

    astep /= आकार(A[0]);
    जर( V )
    {
        vstep /= आकार(V[0]);
        साठी( i = 0; i < n; i++ )
        {
            साठी( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    पूर्णांक iters, maxIters = n*n*30;

    पूर्णांक* indR = (पूर्णांक*)alignPtr(buf, आकार(पूर्णांक));
    पूर्णांक* indC = indR + n;
    _Tp mv = (_Tp)0;

    साठी( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        जर( k < n - 1 )
        {
            साठी( m = k+1, mv = std::असल(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::असल(A[astep*k+i]);
                जर( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        जर( k > 0 )
        {
            साठी( m = 0, mv = std::असल(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::असल(A[astep*i+k]);
                जर( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    जर( n > 1 ) साठी( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        साठी( k = 0, mv = std::असल(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::असल(A[astep*i + indR[i]]);
            जर( mv < val )
                mv = val, k = i;
        }
        पूर्णांक l = indR[k];
        साठी( i = 1; i < n; i++ )
        {
            _Tp val = std::असल(A[astep*indC[i] + i]);
            जर( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        जर( std::असल(p) <= eps )
            थांबा;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::असल(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        जर( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        साठी( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        साठी( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        साठी( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        जर( V )
            साठी( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        साठी( j = 0; j < 2; j++ )
        {
            पूर्णांक idx = j == 0 ? k : l;
            जर( idx < n - 1 )
            {
                साठी( m = idx+1, mv = std::असल(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::असल(A[astep*idx+i]);
                    जर( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            जर( idx > 0 )
            {
                साठी( m = 0, mv = std::असल(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::असल(A[astep*i+idx]);
                    जर( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    साठी( k = 0; k < n-1; k++ )
    {
        m = k;
        साठी( i = k+1; i < n; i++ )
        {
            जर( W[m] < W[i] )
                m = i;
        }
        जर( k != m )
        {
            std::swap(W[m], W[k]);
            जर( V )
                साठी( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    परत सत्य;
}

स्थिर तार्किक Jacobi( दशांश* S, माप_प्रकार sstep, दशांश* e, दशांश* E, माप_प्रकार estep, पूर्णांक n, uchar* buf )
{
    परत JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

स्थिर तार्किक Jacobi( द्विगुण* S, माप_प्रकार sstep, द्विगुण* e, द्विगुण* E, माप_प्रकार estep, पूर्णांक n, uchar* buf )
{
    परत JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


ढाँचा<typename T> संरचना VBLAS
{
    पूर्णांक dot(स्थिरांक T*, स्थिरांक T*, पूर्णांक, T*) स्थिरांक { परत 0; }
    पूर्णांक givens(T*, T*, पूर्णांक, T, T) स्थिरांक { परत 0; }
};

#जर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<> ओळीत पूर्णांक VBLAS<दशांश>::dot(स्थिरांक दशांश* a, स्थिरांक दशांश* b, पूर्णांक n, दशांश* result) स्थिरांक
{
    जर( n < 2*VTraits<v_float32>::vlanes() )
        परत 0;
    पूर्णांक k = 0;
    v_float32 s0 = vx_setzero_f32();
    साठी( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    परत k;
}


ढाँचा<> ओळीत पूर्णांक VBLAS<दशांश>::givens(दशांश* a, दशांश* b, पूर्णांक n, दशांश c, दशांश s) स्थिरांक
{
    जर( n < VTraits<v_float32>::vlanes())
        परत 0;
    पूर्णांक k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    साठी( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    परत k;
}


#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ढाँचा<> ओळीत पूर्णांक VBLAS<द्विगुण>::dot(स्थिरांक द्विगुण* a, स्थिरांक द्विगुण* b, पूर्णांक n, द्विगुण* result) स्थिरांक
{
    जर( n < 2*VTraits<v_float64>::vlanes() )
        परत 0;
    पूर्णांक k = 0;
    v_float64 s0 = vx_setzero_f64();
    साठी( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    परत k;
}


ढाँचा<> ओळीत पूर्णांक VBLAS<द्विगुण>::givens(द्विगुण* a, द्विगुण* b, पूर्णांक n, द्विगुण c, द्विगुण s) स्थिरांक
{
    पूर्णांक k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    साठी( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    परत k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

ढाँचा<typename _Tp> शून्य
JacobiSVDImpl_(_Tp* At, माप_प्रकार astep, _Tp* _W, _Tp* Vt, माप_प्रकार vstep,
               पूर्णांक m, पूर्णांक n, पूर्णांक n1, द्विगुण minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<द्विगुण> Wbuf(n);
    द्विगुण* W = Wbuf.data();
    पूर्णांक i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    द्विगुण sd;
    astep /= आकार(At[0]);
    vstep /= आकार(Vt[0]);

    साठी( i = 0; i < n; i++ )
    {
        साठी( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (द्विगुण)t*t;
        }
        W[i] = sd;

        जर( Vt )
        {
            साठी( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    साठी( iter = 0; iter < max_iter; iter++ )
    {
        तार्किक changed = असत्य;

        साठी( i = 0; i < n-1; i++ )
            साठी( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                द्विगुण a = W[i], p = 0, b = W[j];

                साठी( k = 0; k < m; k++ )
                    p += (द्विगुण)Ai[k]*Aj[k];

                जर( std::असल(p) <= eps*std::वर्ग_मूल((द्विगुण)a*b) )
                    सुरूठेवा;

                p *= 2;
                द्विगुण beta = a - b, gamma = hypot((द्विगुण)p, beta);
                जर( beta < 0 )
                {
                    द्विगुण delta = (gamma - beta)*0.5;
                    s = (_Tp)std::वर्ग_मूल(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                अन्यथा
                {
                    c = (_Tp)std::वर्ग_मूल((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                साठी( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (द्विगुण)t0*t0; b += (द्विगुण)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = सत्य;

                जर( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    साठी( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        जर( !changed )
            थांबा;
    }

    साठी( i = 0; i < n; i++ )
    {
        साठी( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (द्विगुण)t*t;
        }
        W[i] = std::वर्ग_मूल(sd);
    }

    साठी( i = 0; i < n-1; i++ )
    {
        j = i;
        साठी( k = i+1; k < n; k++ )
        {
            जर( W[j] < W[k] )
                j = k;
        }
        जर( i != j )
        {
            std::swap(W[i], W[j]);
            जर( Vt )
            {
                साठी( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                साठी( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    साठी( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    जर( !Vt )
        परत;

    RNG rng(0x12345678);
    साठी( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        साठी( पूर्णांक ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            स्थिरांक _Tp val0 = (_Tp)(1./m);
            साठी( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            साठी( iter = 0; iter < 2; iter++ )
            {
                साठी( j = 0; j < i; j++ )
                {
                    sd = 0;
                    साठी( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    साठी( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::असल(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    साठी( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            साठी( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (द्विगुण)t*t;
            }
            sd = std::वर्ग_मूल(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        साठी( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


स्थिर शून्य JacobiSVD(दशांश* At, माप_प्रकार astep, दशांश* W, दशांश* Vt, माप_प्रकार vstep, पूर्णांक m, पूर्णांक n, पूर्णांक n1=-1)
{
    hal::SVD32f(At, astep, W, रिक्त, astep, Vt, vstep, m, n, n1);
}

स्थिर शून्य JacobiSVD(द्विगुण* At, माप_प्रकार astep, द्विगुण* W, द्विगुण* Vt, माप_प्रकार vstep, पूर्णांक m, पूर्णांक n, पूर्णांक n1=-1)
{
    hal::SVD64f(At, astep, W, रिक्त, astep, Vt, vstep, m, n, n1);
}

ढाँचा <typename fptype> स्थिर ओळीत पूर्णांक
decodeSVDParameters(स्थिरांक fptype* U, स्थिरांक fptype* Vt, पूर्णांक m, पूर्णांक n, पूर्णांक n1)
{
    पूर्णांक halSVDFlag = 0;
    जर(Vt == रिक्त)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    अन्यथा जर(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        जर(U == रिक्त)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    अन्यथा जर(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        जर(U == रिक्त)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    परत halSVDFlag;
}

शून्य hal::SVD32f(दशांश* At, माप_प्रकार astep, दशांश* W, दशांश* U, माप_प्रकार ustep, दशांश* Vt, माप_प्रकार vstep, पूर्णांक m, पूर्णांक n, पूर्णांक n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, भग्न_न्यून, भग्न_अंतर*2);
}

शून्य hal::SVD64f(द्विगुण* At, माप_प्रकार astep, द्विगुण* W, द्विगुण* U, माप_प्रकार ustep, द्विगुण* Vt, माप_प्रकार vstep, पूर्णांक m, पूर्णांक n, पूर्णांक n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, द्विग_न्यून, द्विग_अंतर*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
ढाँचा<typename T1, typename T2, typename T3> स्थिर शून्य
MatrAXPY( पूर्णांक m, पूर्णांक n, स्थिरांक T1* x, पूर्णांक dx,
         स्थिरांक T2* a, पूर्णांक inca, T3* y, पूर्णांक dy )
{
    पूर्णांक i;
    साठी( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        पूर्णांक j = 0;
         #जर CV_ENABLE_UNROLLED
        साठी(; j <= n - 4; j += 4 )
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
        साठी( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

ढाँचा<typename T> स्थिर शून्य
SVBkSbImpl_( पूर्णांक m, पूर्णांक n, स्थिरांक T* w, पूर्णांक incw,
       स्थिरांक T* u, पूर्णांक ldu, तार्किक uT,
       स्थिरांक T* v, पूर्णांक ldv, तार्किक vT,
       स्थिरांक T* b, पूर्णांक ldb, पूर्णांक nb,
       T* x, पूर्णांक ldx, द्विगुण* buffer, T eps )
{
    द्विगुण threshold = 0;
    पूर्णांक udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    पूर्णांक vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    पूर्णांक i, j, nm = std::min(m, n);

    जर( !b )
        nb = m;

    साठी( i = 0; i < n; i++ )
        साठी( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    साठी( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    साठी( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        द्विगुण wi = w[i*incw];
        जर( (द्विगुण)std::असल(wi) <= threshold )
            सुरूठेवा;
        wi = 1/wi;

        जर( nb == 1 )
        {
            द्विगुण s = 0;
            जर( b )
                साठी( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            अन्यथा
                s = u[0];
            s *= wi;

            साठी( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        अन्यथा
        {
            जर( b )
            {
                साठी( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                साठी( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            अन्यथा
            {
                साठी( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

स्थिर शून्य
SVBkSb( पूर्णांक m, पूर्णांक n, स्थिरांक दशांश* w, माप_प्रकार wstep,
        स्थिरांक दशांश* u, माप_प्रकार ustep, तार्किक uT,
        स्थिरांक दशांश* v, माप_प्रकार vstep, तार्किक vT,
        स्थिरांक दशांश* b, माप_प्रकार bstep, पूर्णांक nb,
        दशांश* x, माप_प्रकार xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (पूर्णांक)(wstep/आकार(w[0])) : 1,
                u, (पूर्णांक)(ustep/आकार(u[0])), uT,
                v, (पूर्णांक)(vstep/आकार(v[0])), vT,
                b, (पूर्णांक)(bstep/आकार(b[0])), nb,
                x, (पूर्णांक)(xstep/आकार(x[0])),
                (द्विगुण*)alignPtr(buffer, आकार(द्विगुण)), (दशांश)(द्विग_अंतर*2) );
}

स्थिर शून्य
SVBkSb( पूर्णांक m, पूर्णांक n, स्थिरांक द्विगुण* w, माप_प्रकार wstep,
       स्थिरांक द्विगुण* u, माप_प्रकार ustep, तार्किक uT,
       स्थिरांक द्विगुण* v, माप_प्रकार vstep, तार्किक vT,
       स्थिरांक द्विगुण* b, माप_प्रकार bstep, पूर्णांक nb,
       द्विगुण* x, माप_प्रकार xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (पूर्णांक)(wstep/आकार(w[0])) : 1,
                u, (पूर्णांक)(ustep/आकार(u[0])), uT,
                v, (पूर्णांक)(vstep/आकार(v[0])), vT,
                b, (पूर्णांक)(bstep/आकार(b[0])), nb,
                x, (पूर्णांक)(xstep/आकार(x[0])),
                (द्विगुण*)alignPtr(buffer, आकार(द्विगुण)), द्विग_अंतर*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((द्विगुण)m(0,0)*m(1,1) - (द्विगुण)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((द्विगुण)m(1,1)*m(2,2) - (द्विगुण)m(1,2)*m(2,1)) -  \
                   m(0,1)*((द्विगुण)m(1,0)*m(2,2) - (द्विगुण)m(1,2)*m(2,0)) +  \
                   m(0,2)*((द्विगुण)m(1,0)*m(2,1) - (द्विगुण)m(1,1)*m(2,0)))

द्विगुण determinant( आदान_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    द्विगुण result = 0;
    पूर्णांक type = mat.type(), rows = mat.rows;
    माप_प्रकार step = mat.step;
    स्थिरांक uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((दशांश*)(m + y*step))[x]
    #define Md(y, x) ((द्विगुण*)(m + y*step))[x]

    जर( type == CV_32F )
    {
        जर( rows == 2 )
            result = det2(Mf);
        अन्यथा जर( rows == 3 )
            result = det3(Mf);
        अन्यथा जर( rows == 1 )
            result = Mf(0,0);
        अन्यथा
        {
            माप_प्रकार bufSize = rows*rows*आकार(दशांश);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<दशांश>(), a.step, rows, 0, 0, 0);
            जर( result )
            {
                साठी( पूर्णांक i = 0; i < rows; i++ )
                    result *= a.at<दशांश>(i,i);
            }
        }
    }
    अन्यथा
    {
        जर( rows == 2 )
            result = det2(Md);
        अन्यथा जर( rows == 3 )
            result = det3(Md);
        अन्यथा जर( rows == 1 )
            result = Md(0,0);
        अन्यथा
        {
            माप_प्रकार bufSize = rows*rows*आकार(द्विगुण);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<द्विगुण>(), a.step, rows, 0, 0, 0);
            जर( result )
            {
                साठी( पूर्णांक i = 0; i < rows; i++ )
                    result *= a.at<द्विगुण>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    परत result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((दशांश*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((द्विगुण*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((दशांश*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((द्विगुण*)(dstdata + y*dststep))[x]

द्विगुण invert( आदान_Array _src, OutputArray _dst, पूर्णांक method )
{
    CV_INSTRUMENT_REGION();

    तार्किक result = असत्य;
    Mat src = _src.getMat();
    पूर्णांक type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    माप_प्रकार esz = CV_ELEM_SIZE(type);
    पूर्णांक m = src.rows, n = src.cols;

    जर( method == DECOMP_SVD )
    {
        पूर्णांक nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + आकार(द्विगुण));
        uchar* buf = alignPtr((uchar*)_buf.data(), (पूर्णांक)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        परत type == CV_32F ?
            (w.ptr<दशांश>()[0] >= भग्न_अंतर ?
             w.ptr<दशांश>()[n-1]/w.ptr<दशांश>()[0] : 0) :
            (w.ptr<द्विगुण>()[0] >= द्विग_अंतर ?
             w.ptr<द्विगुण>()[n-1]/w.ptr<द्विगुण>()[0] : 0);
    }

    CV_Assert( m == n );

    जर( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + आकार(द्विगुण));
        uchar* buf = alignPtr((uchar*)_buf.data(), (पूर्णांक)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        परत type == CV_32F ?
        (w.ptr<दशांश>()[0] >= भग्न_अंतर ?
         w.ptr<दशांश>()[n-1]/w.ptr<दशांश>()[0] : 0) :
        (w.ptr<द्विगुण>()[0] >= द्विग_अंतर ?
         w.ptr<द्विगुण>()[n-1]/w.ptr<द्विगुण>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    जर( n <= 3 )
    {
        स्थिरांक uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        माप_प्रकार srcstep = src.step;
        माप_प्रकार dststep = dst.step;

        जर( n == 2 )
        {
            जर( type == CV_32FC1 )
            {
                द्विगुण d = det2(Sf);
                जर( d != 0. )
                {
                    result = सत्य;
                    d = 1./d;
                #जर CV_SIMD128
                    स्थिरांक दशांश d_32f = (दशांश)d;
                    स्थिरांक v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((स्थिरांक दशांश *)srcdata, (स्थिरांक दशांश *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_खालचा(v_rotate_उजवा<1>(s0), s0));
                    v_store_खालचा((दशांश*)dstdata, s0);
                    v_store_वरचा((दशांश*)(dstdata + dststep), s0);
                #अन्यथा
                    द्विगुण t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (दशांश)t0;
                    Df(0,0) = (दशांश)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (दशांश)t0;
                    Df(1,0) = (दशांश)t1;
                #endif
                }
            }
            अन्यथा
            {
                द्विगुण d = det2(Sd);
                जर( d != 0. )
                {
                    result = सत्य;
                    d = 1./d;
                #जर CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((स्थिरांक द्विगुण *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((स्थिरांक द्विगुण *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<द्विगुण>(0), v_extract<1>(s0, s1));//12
                    v_store((द्विगुण*)dstdata, v_combine_खालचा(sm, ss));//31
                    v_store((द्विगुण*)(dstdata + dststep), v_combine_वरचा(ss, sm));//20
                #अन्यथा
                    द्विगुण t0, t1;
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
        अन्यथा जर( n == 3 )
        {
            जर( type == CV_32FC1 )
            {
                द्विगुण d = det3(Sf);

                जर( d != 0. )
                {
                    द्विगुण t[12];

                    result = सत्य;
                    d = 1./d;
                    t[0] = (((द्विगुण)Sf(1,1) * Sf(2,2) - (द्विगुण)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((द्विगुण)Sf(0,2) * Sf(2,1) - (द्विगुण)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((द्विगुण)Sf(0,1) * Sf(1,2) - (द्विगुण)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((द्विगुण)Sf(1,2) * Sf(2,0) - (द्विगुण)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((द्विगुण)Sf(0,0) * Sf(2,2) - (द्विगुण)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((द्विगुण)Sf(0,2) * Sf(1,0) - (द्विगुण)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((द्विगुण)Sf(1,0) * Sf(2,1) - (द्विगुण)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((द्विगुण)Sf(0,1) * Sf(2,0) - (द्विगुण)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((द्विगुण)Sf(0,0) * Sf(1,1) - (द्विगुण)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (दशांश)t[0]; Df(0,1) = (दशांश)t[1]; Df(0,2) = (दशांश)t[2];
                    Df(1,0) = (दशांश)t[3]; Df(1,1) = (दशांश)t[4]; Df(1,2) = (दशांश)t[5];
                    Df(2,0) = (दशांश)t[6]; Df(2,1) = (दशांश)t[7]; Df(2,2) = (दशांश)t[8];
                }
            }
            अन्यथा
            {
                द्विगुण d = det3(Sd);
                जर( d != 0. )
                {
                    result = सत्य;
                    d = 1./d;
                    द्विगुण t[9];

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
        अन्यथा
        {
            CV_Assert( n == 1 );

            जर( type == CV_32FC1 )
            {
                द्विगुण d = Sf(0,0);
                जर( d != 0. )
                {
                    result = सत्य;
                    Df(0,0) = (दशांश)(1./d);
                }
            }
            अन्यथा
            {
                द्विगुण d = Sd(0,0);
                जर( d != 0. )
                {
                    result = सत्य;
                    Dd(0,0) = 1./d;
                }
            }
        }
        जर( !result )
            dst = Scalar(0);
        परत result;
    }

   पूर्णांक elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    जर( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<दशांश>(), src1.step, n, dst.ptr<दशांश>(), dst.step, n) != 0;
    अन्यथा जर( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<द्विगुण>(), src1.step, n, dst.ptr<द्विगुण>(), dst.step, n) != 0;
    अन्यथा जर( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<दशांश>(), src1.step, n, dst.ptr<दशांश>(), dst.step, n);
    अन्यथा
        result = hal::Cholesky64f(src1.ptr<द्विगुण>(), src1.step, n, dst.ptr<द्विगुण>(), dst.step, n);

    जर( !result )
        dst = Scalar(0);

    परत result;
}

UMat UMat::inv(पूर्णांक method) स्थिरांक
{
    UMat m;
    invert(*हे, m, method);
    परत m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

तार्किक solve( आदान_Array _src, आदान_Array _src2arg, OutputArray _dst, पूर्णांक method )
{
    CV_INSTRUMENT_REGION();

    तार्किक result = सत्य;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    पूर्णांक type = src.type();
    तार्किक is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    जर( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((दशांश*)(bdata + y*src2step))[0]
        #define bd(y) ((द्विगुण*)(bdata + y*src2step))[0]

        स्थिरांक uchar* srcdata = src.ptr();
        स्थिरांक uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        माप_प्रकार srcstep = src.step;
        माप_प्रकार src2step = _src2.step;
        माप_प्रकार dststep = dst.step;

        जर( src.rows == 2 )
        {
            जर( type == CV_32FC1 )
            {
                द्विगुण d = det2(Sf);
                जर( d != 0. )
                {
                    द्विगुण t;
                    d = 1./d;
                    t = (दशांश)(((द्विगुण)bf(0)*Sf(1,1) - (द्विगुण)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (दशांश)(((द्विगुण)bf(1)*Sf(0,0) - (द्विगुण)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (दशांश)t;
                }
                अन्यथा
                    result = असत्य;
            }
            अन्यथा
            {
                द्विगुण d = det2(Sd);
                जर( d != 0. )
                {
                    द्विगुण t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                अन्यथा
                    result = असत्य;
            }
        }
        अन्यथा जर( src.rows == 3 )
        {
            जर( type == CV_32FC1 )
            {
                द्विगुण d = det3(Sf);
                जर( d != 0. )
                {
                    दशांश t[3];
                    d = 1./d;

                    t[0] = (दशांश)(d*
                           (bf(0)*((द्विगुण)Sf(1,1)*Sf(2,2) - (द्विगुण)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((द्विगुण)bf(1)*Sf(2,2) - (द्विगुण)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((द्विगुण)bf(1)*Sf(2,1) - (द्विगुण)Sf(1,1)*bf(2))));

                    t[1] = (दशांश)(d*
                           (Sf(0,0)*(द्विगुण)(bf(1)*Sf(2,2) - (द्विगुण)Sf(1,2)*bf(2)) -
                            bf(0)*((द्विगुण)Sf(1,0)*Sf(2,2) - (द्विगुण)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((द्विगुण)Sf(1,0)*bf(2) - (द्विगुण)bf(1)*Sf(2,0))));

                    t[2] = (दशांश)(d*
                           (Sf(0,0)*((द्विगुण)Sf(1,1)*bf(2) - (द्विगुण)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((द्विगुण)Sf(1,0)*bf(2) - (द्विगुण)bf(1)*Sf(2,0)) +
                            bf(0)*((द्विगुण)Sf(1,0)*Sf(2,1) - (द्विगुण)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                अन्यथा
                    result = असत्य;
            }
            अन्यथा
            {
                द्विगुण d = det3(Sd);
                जर( d != 0. )
                {
                    द्विगुण t[9];

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
                अन्यथा
                    result = असत्य;
            }
        }
        अन्यथा
        {
            CV_Assert( src.rows == 1 );

            जर( type == CV_32FC1 )
            {
                द्विगुण d = Sf(0,0);
                जर( d != 0. )
                    Df(0,0) = (दशांश)(bf(0)/d);
                अन्यथा
                    result = असत्य;
            }
            अन्यथा
            {
                द्विगुण d = Sd(0,0);
                जर( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                अन्यथा
                    result = असत्य;
            }
        }
        परत result;
    }

    पूर्णांक m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    माप_प्रकार esz = CV_ELEM_SIZE(type), bufsize = 0;
    माप_प्रकार vstep = alignSize(n*esz, 16);
    माप_प्रकार astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    जर( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    जर( m == n )
        is_normal = असत्य;
    अन्यथा जर( is_normal )
    {
        m_ = n;
        जर( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    माप_प्रकार asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    जर( is_normal )
        bufsize += n*nb*esz;
    जर( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*आकार(द्विगुण) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    जर( is_normal )
        mulTransposed(src, a, सत्य);
    अन्यथा जर( method != DECOMP_SVD )
        src.copyTo(a);
    अन्यथा
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    जर( !is_normal )
    {
        जर( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    अन्यथा
    {
        // a'*b
        जर( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        अन्यथा
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    जर( method == DECOMP_LU )
    {
        जर( type == CV_32F )
            result = hal::LU32f(a.ptr<दशांश>(), a.step, n, dst.ptr<दशांश>(), dst.step, nb) != 0;
        अन्यथा
            result = hal::LU64f(a.ptr<द्विगुण>(), a.step, n, dst.ptr<द्विगुण>(), dst.step, nb) != 0;
    }
    अन्यथा जर( method == DECOMP_CHOLESKY )
    {
        जर( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<दशांश>(), a.step, n, dst.ptr<दशांश>(), dst.step, nb);
        अन्यथा
            result = hal::Cholesky64f(a.ptr<द्विगुण>(), a.step, n, dst.ptr<द्विगुण>(), dst.step, nb);
    }
    अन्यथा जर( method == DECOMP_QR )
    {
        Mat rhsMat;
        जर( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        अन्यथा
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        जर( type == CV_32F )
            result = hal::QR32f(a.ptr<दशांश>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<दशांश>(), rhsMat.step, रिक्त) != 0;
        अन्यथा
            result = hal::QR64f(a.ptr<द्विगुण>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<द्विगुण>(), rhsMat.step, रिक्त) != 0;

        जर (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    अन्यथा
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        जर( method == DECOMP_EIG )
        {
            जर( type == CV_32F )
                Jacobi(a.ptr<दशांश>(), a.step, w.ptr<दशांश>(), v.ptr<दशांश>(), v.step, n, ptr);
            अन्यथा
                Jacobi(a.ptr<द्विगुण>(), a.step, w.ptr<द्विगुण>(), v.ptr<द्विगुण>(), v.step, n, ptr);
            u = v;
        }
        अन्यथा
        {
            जर( type == CV_32F )
                JacobiSVD(a.ptr<दशांश>(), a.step, w.ptr<दशांश>(), v.ptr<दशांश>(), v.step, m_, n);
            अन्यथा
                JacobiSVD(a.ptr<द्विगुण>(), a.step, w.ptr<द्विगुण>(), v.ptr<द्विगुण>(), v.step, m_, n);
            u = a;
        }

        जर( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<दशांश>(), 0, u.ptr<दशांश>(), u.step, सत्य,
                   v.ptr<दशांश>(), v.step, सत्य, src2.ptr<दशांश>(),
                   src2.step, nb, dst.ptr<दशांश>(), dst.step, ptr);
        }
        अन्यथा
        {
            SVBkSb(m_, n, w.ptr<द्विगुण>(), 0, u.ptr<द्विगुण>(), u.step, सत्य,
                   v.ptr<द्विगुण>(), v.step, सत्य, src2.ptr<द्विगुण>(),
                   src2.step, nb, dst.ptr<द्विगुण>(), dst.step, ptr);
        }
        result = सत्य;
    }

    जर( !result )
        dst = Scalar(0);

    परत result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

तार्किक eigen( आदान_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    पूर्णांक type = src.type();
    पूर्णांक n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    जर( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    स्थिरांक तार्किक evecNeeded = _evects.needed();
    स्थिरांक पूर्णांक esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    जर ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        जर ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            जर ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            परत सत्य;
        }
    } अन्यथा { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        जर ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            जर ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            परत सत्य;
        }
    }
    परत असत्य;
#अन्यथा

    माप_प्रकार elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    तार्किक ok = type == CV_32F ?
        Jacobi(a.ptr<दशांश>(), a.step, w.ptr<दशांश>(), v.ptr<दशांश>(), v.step, n, ptr) :
        Jacobi(a.ptr<द्विगुण>(), a.step, w.ptr<द्विगुण>(), v.ptr<द्विगुण>(), v.step, n, ptr);

    w.copyTo(_evals);
    परत ok;
#endif
}

स्थिर शून्य _SVDcompute( आदान_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, पूर्णांक flags )
{
    Mat src = _aarr.getMat();
    पूर्णांक m = src.rows, n = src.cols;
    पूर्णांक type = src.type();
    तार्किक compute_uv = _u.needed() || _vt.needed();
    तार्किक full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    जर( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = असत्य;
    }

    तार्किक at = असत्य;
    जर( m < n )
    {
        std::swap(m, n);
        at = सत्य;
    }

    पूर्णांक urows = full_uv ? m : n;
    माप_प्रकार esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    जर( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    जर( urows > n )
        temp_u = Scalar::all(0);

    जर( !at )
        transpose(src, temp_a);
    अन्यथा
        src.copyTo(temp_a);

    जर( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<दशांश>(), temp_u.step, temp_w.ptr<दशांश>(),
              temp_v.ptr<दशांश>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    अन्यथा
    {
        JacobiSVD(temp_a.ptr<द्विगुण>(), temp_u.step, temp_w.ptr<द्विगुण>(),
              temp_v.ptr<द्विगुण>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    जर( compute_uv )
    {
        जर( !at )
        {
            जर( _u.needed() )
                transpose(temp_u, _u);
            जर( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        अन्यथा
        {
            जर( _u.needed() )
                transpose(temp_v, _u);
            जर( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


शून्य SVD::compute( आदान_Array a, OutputArray w, OutputArray u, OutputArray vt, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

शून्य SVD::compute( आदान_Array a, OutputArray w, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

शून्य SVD::backSubst( आदान_Array _w, आदान_Array _u, आदान_Array _vt,
                     आदान_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    पूर्णांक type = w.type(), esz = (पूर्णांक)w.elemSize();
    पूर्णांक m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    माप_प्रकार wstep = w.rows == 1 ? (माप_प्रकार)esz : w.cols == 1 ? (माप_प्रकार)w.step : (माप_प्रकार)w.step + esz;
    AutoBuffer<uchar> buffer(nb*आकार(द्विगुण) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    जर( type == CV_32F )
        SVBkSb(m, n, w.ptr<दशांश>(), wstep, u.ptr<दशांश>(), u.step, असत्य,
               vt.ptr<दशांश>(), vt.step, सत्य, rhs.ptr<दशांश>(), rhs.step, nb,
               dst.ptr<दशांश>(), dst.step, buffer.data());
    अन्यथा जर( type == CV_64F )
        SVBkSb(m, n, w.ptr<द्विगुण>(), wstep, u.ptr<द्विगुण>(), u.step, असत्य,
               vt.ptr<द्विगुण>(), vt.step, सत्य, rhs.ptr<द्विगुण>(), rhs.step, nb,
               dst.ptr<द्विगुण>(), dst.step, buffer.data());
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::चालक ()(आदान_Array a, पूर्णांक flags)
{
    _SVDcompute(a, w, u, vt, flags);
    परत *हे;
}


शून्य SVD::backSubst( आदान_Array rhs, OutputArray dst ) स्थिरांक
{
    backSubst( w, u, vt, rhs, dst );
}

}


शून्य cv::SVDecomp(आदान_Array src, OutputArray w, OutputArray u, OutputArray vt, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

शून्य cv::SVBackSubst(आदान_Array w, आदान_Array u, आदान_Array vt, आदान_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
