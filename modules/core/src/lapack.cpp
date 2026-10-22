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

#समावेश "precomp.hpp"
#समावेश <limits>

#अगर_घोषित HAVE_EIGEN
#  अगर defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  अगर defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#पूर्ण_अगर

namespace cv
{

पूर्णांक LU(भग्न* A, माप_प्रकार astep, पूर्णांक m, भग्न* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    वापस hal::LU32f(A, astep, m, b, bstep, n);
}

पूर्णांक LU(द्विगुन* A, माप_प्रकार astep, पूर्णांक m, द्विगुन* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    वापस hal::LU64f(A, astep, m, b, bstep, n);
}

bool Cholesky(भग्न* A, माप_प्रकार astep, पूर्णांक m, भग्न* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    वापस hal::Cholesky32f(A, astep, m, b, bstep, n);
}

bool Cholesky(द्विगुन* A, माप_प्रकार astep, पूर्णांक m, द्विगुन* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    वापस hal::Cholesky64f(A, astep, m, b, bstep, n);
}

ढाँचा<typename _Tp> अटल अंतरभूत _Tp hypot(_Tp a, _Tp b)
{
    a = std::असल(a);
    b = std::असल(b);
    अगर( a > b )
    {
        b /= a;
        वापस a*std::वर्ग_मूल(1 + b*b);
    }
    अगर( b > 0 )
    {
        a /= b;
        वापस b*std::वर्ग_मूल(1 + a*a);
    }
    वापस 0;
}


ढाँचा<typename _Tp> bool
JacobiImpl_( _Tp* A, माप_प्रकार astep, _Tp* W, _Tp* V, माप_प्रकार vstep, पूर्णांक n, uchar* buf )
{
    स्थिर _Tp eps = std::numeric_limits<_Tp>::epsilon();
    पूर्णांक i, j, k, m;

    astep /= माप(A[0]);
    अगर( V )
    {
        vstep /= माप(V[0]);
        क्रम( i = 0; i < n; i++ )
        {
            क्रम( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    पूर्णांक iters, maxIters = n*n*30;

    पूर्णांक* indR = (पूर्णांक*)alignPtr(buf, माप(पूर्णांक));
    पूर्णांक* indC = indR + n;
    _Tp mv = (_Tp)0;

    क्रम( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        अगर( k < n - 1 )
        {
            क्रम( m = k+1, mv = std::असल(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::असल(A[astep*k+i]);
                अगर( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        अगर( k > 0 )
        {
            क्रम( m = 0, mv = std::असल(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::असल(A[astep*i+k]);
                अगर( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    अगर( n > 1 ) क्रम( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        क्रम( k = 0, mv = std::असल(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::असल(A[astep*i + indR[i]]);
            अगर( mv < val )
                mv = val, k = i;
        }
        पूर्णांक l = indR[k];
        क्रम( i = 1; i < n; i++ )
        {
            _Tp val = std::असल(A[astep*indC[i] + i]);
            अगर( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        अगर( std::असल(p) <= eps )
            अवरोध;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::असल(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        अगर( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#अघोषित rotate
#घोषणा rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        क्रम( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        क्रम( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        क्रम( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        अगर( V )
            क्रम( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#अघोषित rotate

        क्रम( j = 0; j < 2; j++ )
        {
            पूर्णांक idx = j == 0 ? k : l;
            अगर( idx < n - 1 )
            {
                क्रम( m = idx+1, mv = std::असल(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::असल(A[astep*idx+i]);
                    अगर( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            अगर( idx > 0 )
            {
                क्रम( m = 0, mv = std::असल(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::असल(A[astep*i+idx]);
                    अगर( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    क्रम( k = 0; k < n-1; k++ )
    {
        m = k;
        क्रम( i = k+1; i < n; i++ )
        {
            अगर( W[m] < W[i] )
                m = i;
        }
        अगर( k != m )
        {
            std::swap(W[m], W[k]);
            अगर( V )
                क्रम( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    वापस true;
}

अटल bool Jacobi( भग्न* S, माप_प्रकार sstep, भग्न* e, भग्न* E, माप_प्रकार estep, पूर्णांक n, uchar* buf )
{
    वापस JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

अटल bool Jacobi( द्विगुन* S, माप_प्रकार sstep, द्विगुन* e, द्विगुन* E, माप_प्रकार estep, पूर्णांक n, uchar* buf )
{
    वापस JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


ढाँचा<typename T> काष्ठा VBLAS
{
    पूर्णांक dot(स्थिर T*, स्थिर T*, पूर्णांक, T*) स्थिर { वापस 0; }
    पूर्णांक givens(T*, T*, पूर्णांक, T, T) स्थिर { वापस 0; }
};

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<> अंतरभूत पूर्णांक VBLAS<भग्न>::dot(स्थिर भग्न* a, स्थिर भग्न* b, पूर्णांक n, भग्न* परिणाम) स्थिर
{
    अगर( n < 2*VTraits<v_float32>::vlanes() )
        वापस 0;
    पूर्णांक k = 0;
    v_float32 s0 = vx_setzero_f32();
    क्रम( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *परिणाम = v_reduce_योग(s0);
    vx_cleanup();
    वापस k;
}


ढाँचा<> अंतरभूत पूर्णांक VBLAS<भग्न>::givens(भग्न* a, भग्न* b, पूर्णांक n, भग्न c, भग्न s) स्थिर
{
    अगर( n < VTraits<v_float32>::vlanes())
        वापस 0;
    पूर्णांक k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    क्रम( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    वापस k;
}


#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ढाँचा<> अंतरभूत पूर्णांक VBLAS<द्विगुन>::dot(स्थिर द्विगुन* a, स्थिर द्विगुन* b, पूर्णांक n, द्विगुन* परिणाम) स्थिर
{
    अगर( n < 2*VTraits<v_float64>::vlanes() )
        वापस 0;
    पूर्णांक k = 0;
    v_float64 s0 = vx_setzero_f64();
    क्रम( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *परिणाम = v_reduce_योग(s0);
    vx_cleanup();
    वापस k;
}


ढाँचा<> अंतरभूत पूर्णांक VBLAS<द्विगुन>::givens(द्विगुन* a, द्विगुन* b, पूर्णांक n, द्विगुन c, द्विगुन s) स्थिर
{
    पूर्णांक k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    क्रम( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    वापस k;
}


#पूर्ण_अगर // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#पूर्ण_अगर // (CV_SIMD || CV_SIMD_SCALABLE)

ढाँचा<typename _Tp> व्योम
JacobiSVDImpl_(_Tp* At, माप_प्रकार astep, _Tp* _W, _Tp* Vt, माप_प्रकार vstep,
               पूर्णांक m, पूर्णांक n, पूर्णांक n1, द्विगुन minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    Auto_बफर<द्विगुन> Wbuf(n);
    द्विगुन* W = Wbuf.तथ्य();
    पूर्णांक i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    द्विगुन sd;
    astep /= माप(At[0]);
    vstep /= माप(Vt[0]);

    क्रम( i = 0; i < n; i++ )
    {
        क्रम( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (द्विगुन)t*t;
        }
        W[i] = sd;

        अगर( Vt )
        {
            क्रम( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    क्रम( iter = 0; iter < max_iter; iter++ )
    {
        bool changed = false;

        क्रम( i = 0; i < n-1; i++ )
            क्रम( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                द्विगुन a = W[i], p = 0, b = W[j];

                क्रम( k = 0; k < m; k++ )
                    p += (द्विगुन)Ai[k]*Aj[k];

                अगर( std::असल(p) <= eps*std::वर्ग_मूल((द्विगुन)a*b) )
                    जारी;

                p *= 2;
                द्विगुन beta = a - b, gamma = hypot((द्विगुन)p, beta);
                अगर( beta < 0 )
                {
                    द्विगुन delta = (gamma - beta)*0.5;
                    s = (_Tp)std::वर्ग_मूल(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                अन्यथा
                {
                    c = (_Tp)std::वर्ग_मूल((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                क्रम( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (द्विगुन)t0*t0; b += (द्विगुन)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = true;

                अगर( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    क्रम( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        अगर( !changed )
            अवरोध;
    }

    क्रम( i = 0; i < n; i++ )
    {
        क्रम( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (द्विगुन)t*t;
        }
        W[i] = std::वर्ग_मूल(sd);
    }

    क्रम( i = 0; i < n-1; i++ )
    {
        j = i;
        क्रम( k = i+1; k < n; k++ )
        {
            अगर( W[j] < W[k] )
                j = k;
        }
        अगर( i != j )
        {
            std::swap(W[i], W[j]);
            अगर( Vt )
            {
                क्रम( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                क्रम( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    क्रम( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    अगर( !Vt )
        वापस;

    RNG rng(0x12345678);
    क्रम( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        क्रम( पूर्णांक ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            स्थिर _Tp val0 = (_Tp)(1./m);
            क्रम( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            क्रम( iter = 0; iter < 2; iter++ )
            {
                क्रम( j = 0; j < i; j++ )
                {
                    sd = 0;
                    क्रम( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    क्रम( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::असल(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    क्रम( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            क्रम( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (द्विगुन)t*t;
            }
            sd = std::वर्ग_मूल(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        क्रम( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


अटल व्योम JacobiSVD(भग्न* At, माप_प्रकार astep, भग्न* W, भग्न* Vt, माप_प्रकार vstep, पूर्णांक m, पूर्णांक n, पूर्णांक n1=-1)
{
    hal::SVD32f(At, astep, W, शून्य, astep, Vt, vstep, m, n, n1);
}

अटल व्योम JacobiSVD(द्विगुन* At, माप_प्रकार astep, द्विगुन* W, द्विगुन* Vt, माप_प्रकार vstep, पूर्णांक m, पूर्णांक n, पूर्णांक n1=-1)
{
    hal::SVD64f(At, astep, W, शून्य, astep, Vt, vstep, m, n, n1);
}

ढाँचा <typename fptype> अटल अंतरभूत पूर्णांक
decodeSVDParameters(स्थिर fptype* U, स्थिर fptype* Vt, पूर्णांक m, पूर्णांक n, पूर्णांक n1)
{
    पूर्णांक halSVDFlag = 0;
    अगर(Vt == शून्य)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    अन्यथा अगर(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        अगर(U == शून्य)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    अन्यथा अगर(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        अगर(U == शून्य)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    वापस halSVDFlag;
}

व्योम hal::SVD32f(भग्न* At, माप_प्रकार astep, भग्न* W, भग्न* U, माप_प्रकार ustep, भग्न* Vt, माप_प्रकार vstep, पूर्णांक m, पूर्णांक n, पूर्णांक n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, भग्न_न्यून, भग्न_अंतर*2);
}

व्योम hal::SVD64f(द्विगुन* At, माप_प्रकार astep, द्विगुन* W, द्विगुन* U, माप_प्रकार ustep, द्विगुन* Vt, माप_प्रकार vstep, पूर्णांक m, पूर्णांक n, पूर्णांक n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, द्विग_न्यून, द्विग_अंतर*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
ढाँचा<typename T1, typename T2, typename T3> अटल व्योम
MatrAXPY( पूर्णांक m, पूर्णांक n, स्थिर T1* x, पूर्णांक dx,
         स्थिर T2* a, पूर्णांक inca, T3* y, पूर्णांक dy )
{
    पूर्णांक i;
    क्रम( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        पूर्णांक j = 0;
         #अगर CV_ENABLE_UNROLLED
        क्रम(; j <= n - 4; j += 4 )
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
        #पूर्ण_अगर
        क्रम( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

ढाँचा<typename T> अटल व्योम
SVBkSbImpl_( पूर्णांक m, पूर्णांक n, स्थिर T* w, पूर्णांक incw,
       स्थिर T* u, पूर्णांक ldu, bool uT,
       स्थिर T* v, पूर्णांक ldv, bool vT,
       स्थिर T* b, पूर्णांक ldb, पूर्णांक nb,
       T* x, पूर्णांक ldx, द्विगुन* बफर, T eps )
{
    द्विगुन threshold = 0;
    पूर्णांक udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    पूर्णांक vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    पूर्णांक i, j, nm = std::min(m, n);

    अगर( !b )
        nb = m;

    क्रम( i = 0; i < n; i++ )
        क्रम( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    क्रम( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    क्रम( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        द्विगुन wi = w[i*incw];
        अगर( (द्विगुन)std::असल(wi) <= threshold )
            जारी;
        wi = 1/wi;

        अगर( nb == 1 )
        {
            द्विगुन s = 0;
            अगर( b )
                क्रम( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            अन्यथा
                s = u[0];
            s *= wi;

            क्रम( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        अन्यथा
        {
            अगर( b )
            {
                क्रम( j = 0; j < nb; j++ )
                    बफर[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, बफर, 0 );
                क्रम( j = 0; j < nb; j++ )
                    बफर[j] *= wi;
            }
            अन्यथा
            {
                क्रम( j = 0; j < nb; j++ )
                    बफर[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, बफर, 0, v, vdelta1, x, ldx );
        }
    }
}

अटल व्योम
SVBkSb( पूर्णांक m, पूर्णांक n, स्थिर भग्न* w, माप_प्रकार wstep,
        स्थिर भग्न* u, माप_प्रकार ustep, bool uT,
        स्थिर भग्न* v, माप_प्रकार vstep, bool vT,
        स्थिर भग्न* b, माप_प्रकार bstep, पूर्णांक nb,
        भग्न* x, माप_प्रकार xstep, uchar* बफर )
{
    SVBkSbImpl_(m, n, w, wstep ? (पूर्णांक)(wstep/माप(w[0])) : 1,
                u, (पूर्णांक)(ustep/माप(u[0])), uT,
                v, (पूर्णांक)(vstep/माप(v[0])), vT,
                b, (पूर्णांक)(bstep/माप(b[0])), nb,
                x, (पूर्णांक)(xstep/माप(x[0])),
                (द्विगुन*)alignPtr(बफर, माप(द्विगुन)), (भग्न)(द्विग_अंतर*2) );
}

अटल व्योम
SVBkSb( पूर्णांक m, पूर्णांक n, स्थिर द्विगुन* w, माप_प्रकार wstep,
       स्थिर द्विगुन* u, माप_प्रकार ustep, bool uT,
       स्थिर द्विगुन* v, माप_प्रकार vstep, bool vT,
       स्थिर द्विगुन* b, माप_प्रकार bstep, पूर्णांक nb,
       द्विगुन* x, माप_प्रकार xstep, uchar* बफर )
{
    SVBkSbImpl_(m, n, w, wstep ? (पूर्णांक)(wstep/माप(w[0])) : 1,
                u, (पूर्णांक)(ustep/माप(u[0])), uT,
                v, (पूर्णांक)(vstep/माप(v[0])), vT,
                b, (पूर्णांक)(bstep/माप(b[0])), nb,
                x, (पूर्णांक)(xstep/माप(x[0])),
                (द्विगुन*)alignPtr(बफर, माप(द्विगुन)), द्विग_अंतर*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#घोषणा det2(m)   ((द्विगुन)m(0,0)*m(1,1) - (द्विगुन)m(0,1)*m(1,0))
#घोषणा det3(m)   (m(0,0)*((द्विगुन)m(1,1)*m(2,2) - (द्विगुन)m(1,2)*m(2,1)) -  \
                   m(0,1)*((द्विगुन)m(1,0)*m(2,2) - (द्विगुन)m(1,2)*m(2,0)) +  \
                   m(0,2)*((द्विगुन)m(1,0)*m(2,1) - (द्विगुन)m(1,1)*m(2,0)))

द्विगुन determinant( निवेश_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    द्विगुन परिणाम = 0;
    पूर्णांक type = mat.type(), rows = mat.rows;
    माप_प्रकार step = mat.step;
    स्थिर uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #घोषणा Mf(y, x) ((भग्न*)(m + y*step))[x]
    #घोषणा Md(y, x) ((द्विगुन*)(m + y*step))[x]

    अगर( type == CV_32F )
    {
        अगर( rows == 2 )
            परिणाम = det2(Mf);
        अन्यथा अगर( rows == 3 )
            परिणाम = det3(Mf);
        अन्यथा अगर( rows == 1 )
            परिणाम = Mf(0,0);
        अन्यथा
        {
            माप_प्रकार buf_आकार = rows*rows*माप(भग्न);
            Auto_बफर<uchar> बफर(buf_आकार);
            Mat a(rows, rows, CV_32F, बफर.तथ्य());
            mat.copyTo(a);

            परिणाम = hal::LU32f(a.ptr<भग्न>(), a.step, rows, 0, 0, 0);
            अगर( परिणाम )
            {
                क्रम( पूर्णांक i = 0; i < rows; i++ )
                    परिणाम *= a.at<भग्न>(i,i);
            }
        }
    }
    अन्यथा
    {
        अगर( rows == 2 )
            परिणाम = det2(Md);
        अन्यथा अगर( rows == 3 )
            परिणाम = det3(Md);
        अन्यथा अगर( rows == 1 )
            परिणाम = Md(0,0);
        अन्यथा
        {
            माप_प्रकार buf_आकार = rows*rows*माप(द्विगुन);
            Auto_बफर<uchar> बफर(buf_आकार);
            Mat a(rows, rows, CV_64F, बफर.तथ्य());
            mat.copyTo(a);

            परिणाम = hal::LU64f(a.ptr<द्विगुन>(), a.step, rows, 0, 0, 0);
            अगर( परिणाम )
            {
                क्रम( पूर्णांक i = 0; i < rows; i++ )
                    परिणाम *= a.at<द्विगुन>(i,i);
            }
        }
    }

    #अघोषित Mf
    #अघोषित Md

    वापस परिणाम;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#घोषणा Sf( y, x ) ((भग्न*)(srcdata + y*srcstep))[x]
#घोषणा Sd( y, x ) ((द्विगुन*)(srcdata + y*srcstep))[x]
#घोषणा Df( y, x ) ((भग्न*)(dstdata + y*dststep))[x]
#घोषणा Dd( y, x ) ((द्विगुन*)(dstdata + y*dststep))[x]

द्विगुन invert( निवेश_Array _src, OutputArray _dst, पूर्णांक method )
{
    CV_INSTRUMENT_REGION();

    bool परिणाम = false;
    Mat src = _src.getMat();
    पूर्णांक type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    माप_प्रकार esz = CV_ELEM_आकार(type);
    पूर्णांक m = src.rows, n = src.cols;

    अगर( method == DECOMP_SVD )
    {
        पूर्णांक nm = std::min(m, n);

        Auto_बफर<uchar> _buf((m*nm + nm + nm*n)*esz + माप(द्विगुन));
        uchar* buf = alignPtr((uchar*)_buf.तथ्य(), (पूर्णांक)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        वापस type == CV_32F ?
            (w.ptr<भग्न>()[0] >= भग्न_अंतर ?
             w.ptr<भग्न>()[n-1]/w.ptr<भग्न>()[0] : 0) :
            (w.ptr<द्विगुन>()[0] >= द्विग_अंतर ?
             w.ptr<द्विगुन>()[n-1]/w.ptr<द्विगुन>()[0] : 0);
    }

    CV_Assert( m == n );

    अगर( method == DECOMP_EIG )
    {
        Auto_बफर<uchar> _buf((n*n*2 + n)*esz + माप(द्विगुन));
        uchar* buf = alignPtr((uchar*)_buf.तथ्य(), (पूर्णांक)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        वापस type == CV_32F ?
        (w.ptr<भग्न>()[0] >= भग्न_अंतर ?
         w.ptr<भग्न>()[n-1]/w.ptr<भग्न>()[0] : 0) :
        (w.ptr<द्विगुन>()[0] >= द्विग_अंतर ?
         w.ptr<द्विगुन>()[n-1]/w.ptr<द्विगुन>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    अगर( n <= 3 )
    {
        स्थिर uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        माप_प्रकार srcstep = src.step;
        माप_प्रकार dststep = dst.step;

        अगर( n == 2 )
        {
            अगर( type == CV_32FC1 )
            {
                द्विगुन d = det2(Sf);
                अगर( d != 0. )
                {
                    परिणाम = true;
                    d = 1./d;
                #अगर CV_SIMD128
                    स्थिर भग्न d_32f = (भग्न)d;
                    स्थिर v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((स्थिर भग्न *)srcdata, (स्थिर भग्न *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_निचला(v_rotate_दायाँ<1>(s0), s0));
                    v_store_निचला((भग्न*)dstdata, s0);
                    v_store_ऊपरी((भग्न*)(dstdata + dststep), s0);
                #अन्यथा
                    द्विगुन t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (भग्न)t0;
                    Df(0,0) = (भग्न)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (भग्न)t0;
                    Df(1,0) = (भग्न)t1;
                #पूर्ण_अगर
                }
            }
            अन्यथा
            {
                द्विगुन d = det2(Sd);
                अगर( d != 0. )
                {
                    परिणाम = true;
                    d = 1./d;
                #अगर CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((स्थिर द्विगुन *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((स्थिर द्विगुन *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<द्विगुन>(0), v_extract<1>(s0, s1));//12
                    v_store((द्विगुन*)dstdata, v_combine_निचला(sm, ss));//31
                    v_store((द्विगुन*)(dstdata + dststep), v_combine_ऊपरी(ss, sm));//20
                #अन्यथा
                    द्विगुन t0, t1;
                    t0 = Sd(0,0)*d;
                    t1 = Sd(1,1)*d;
                    Dd(1,1) = t0;
                    Dd(0,0) = t1;
                    t0 = -Sd(0,1)*d;
                    t1 = -Sd(1,0)*d;
                    Dd(0,1) = t0;
                    Dd(1,0) = t1;
                #पूर्ण_अगर
                }
            }
        }
        अन्यथा अगर( n == 3 )
        {
            अगर( type == CV_32FC1 )
            {
                द्विगुन d = det3(Sf);

                अगर( d != 0. )
                {
                    द्विगुन t[12];

                    परिणाम = true;
                    d = 1./d;
                    t[0] = (((द्विगुन)Sf(1,1) * Sf(2,2) - (द्विगुन)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((द्विगुन)Sf(0,2) * Sf(2,1) - (द्विगुन)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((द्विगुन)Sf(0,1) * Sf(1,2) - (द्विगुन)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((द्विगुन)Sf(1,2) * Sf(2,0) - (द्विगुन)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((द्विगुन)Sf(0,0) * Sf(2,2) - (द्विगुन)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((द्विगुन)Sf(0,2) * Sf(1,0) - (द्विगुन)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((द्विगुन)Sf(1,0) * Sf(2,1) - (द्विगुन)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((द्विगुन)Sf(0,1) * Sf(2,0) - (द्विगुन)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((द्विगुन)Sf(0,0) * Sf(1,1) - (द्विगुन)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (भग्न)t[0]; Df(0,1) = (भग्न)t[1]; Df(0,2) = (भग्न)t[2];
                    Df(1,0) = (भग्न)t[3]; Df(1,1) = (भग्न)t[4]; Df(1,2) = (भग्न)t[5];
                    Df(2,0) = (भग्न)t[6]; Df(2,1) = (भग्न)t[7]; Df(2,2) = (भग्न)t[8];
                }
            }
            अन्यथा
            {
                द्विगुन d = det3(Sd);
                अगर( d != 0. )
                {
                    परिणाम = true;
                    d = 1./d;
                    द्विगुन t[9];

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

            अगर( type == CV_32FC1 )
            {
                द्विगुन d = Sf(0,0);
                अगर( d != 0. )
                {
                    परिणाम = true;
                    Df(0,0) = (भग्न)(1./d);
                }
            }
            अन्यथा
            {
                द्विगुन d = Sd(0,0);
                अगर( d != 0. )
                {
                    परिणाम = true;
                    Dd(0,0) = 1./d;
                }
            }
        }
        अगर( !परिणाम )
            dst = Scalar(0);
        वापस परिणाम;
    }

   पूर्णांक elem_आकार_2 = CV_ELEM_आकार(type);
    Auto_बफर<uchar> buf(n*n*elem_आकार_2);
    Mat src1(n, n, type, buf.तथ्य());
    src.copyTo(src1);
    setIdentity(dst);

    अगर( method == DECOMP_LU && type == CV_32F )
        परिणाम = hal::LU32f(src1.ptr<भग्न>(), src1.step, n, dst.ptr<भग्न>(), dst.step, n) != 0;
    अन्यथा अगर( method == DECOMP_LU && type == CV_64F )
        परिणाम = hal::LU64f(src1.ptr<द्विगुन>(), src1.step, n, dst.ptr<द्विगुन>(), dst.step, n) != 0;
    अन्यथा अगर( method == DECOMP_CHOLESKY && type == CV_32F )
        परिणाम = hal::Cholesky32f(src1.ptr<भग्न>(), src1.step, n, dst.ptr<भग्न>(), dst.step, n);
    अन्यथा
        परिणाम = hal::Cholesky64f(src1.ptr<द्विगुन>(), src1.step, n, dst.ptr<द्विगुन>(), dst.step, n);

    अगर( !परिणाम )
        dst = Scalar(0);

    वापस परिणाम;
}

UMat UMat::inv(पूर्णांक method) स्थिर
{
    UMat m;
    invert(*यह, m, method);
    वापस m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

bool solve( निवेश_Array _src, निवेश_Array _src2arg, OutputArray _dst, पूर्णांक method )
{
    CV_INSTRUMENT_REGION();

    bool परिणाम = true;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    पूर्णांक type = src.type();
    bool is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    अगर( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #घोषणा bf(y) ((भग्न*)(bdata + y*src2step))[0]
        #घोषणा bd(y) ((द्विगुन*)(bdata + y*src2step))[0]

        स्थिर uchar* srcdata = src.ptr();
        स्थिर uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        माप_प्रकार srcstep = src.step;
        माप_प्रकार src2step = _src2.step;
        माप_प्रकार dststep = dst.step;

        अगर( src.rows == 2 )
        {
            अगर( type == CV_32FC1 )
            {
                द्विगुन d = det2(Sf);
                अगर( d != 0. )
                {
                    द्विगुन t;
                    d = 1./d;
                    t = (भग्न)(((द्विगुन)bf(0)*Sf(1,1) - (द्विगुन)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (भग्न)(((द्विगुन)bf(1)*Sf(0,0) - (द्विगुन)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (भग्न)t;
                }
                अन्यथा
                    परिणाम = false;
            }
            अन्यथा
            {
                द्विगुन d = det2(Sd);
                अगर( d != 0. )
                {
                    द्विगुन t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                अन्यथा
                    परिणाम = false;
            }
        }
        अन्यथा अगर( src.rows == 3 )
        {
            अगर( type == CV_32FC1 )
            {
                द्विगुन d = det3(Sf);
                अगर( d != 0. )
                {
                    भग्न t[3];
                    d = 1./d;

                    t[0] = (भग्न)(d*
                           (bf(0)*((द्विगुन)Sf(1,1)*Sf(2,2) - (द्विगुन)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((द्विगुन)bf(1)*Sf(2,2) - (द्विगुन)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((द्विगुन)bf(1)*Sf(2,1) - (द्विगुन)Sf(1,1)*bf(2))));

                    t[1] = (भग्न)(d*
                           (Sf(0,0)*(द्विगुन)(bf(1)*Sf(2,2) - (द्विगुन)Sf(1,2)*bf(2)) -
                            bf(0)*((द्विगुन)Sf(1,0)*Sf(2,2) - (द्विगुन)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((द्विगुन)Sf(1,0)*bf(2) - (द्विगुन)bf(1)*Sf(2,0))));

                    t[2] = (भग्न)(d*
                           (Sf(0,0)*((द्विगुन)Sf(1,1)*bf(2) - (द्विगुन)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((द्विगुन)Sf(1,0)*bf(2) - (द्विगुन)bf(1)*Sf(2,0)) +
                            bf(0)*((द्विगुन)Sf(1,0)*Sf(2,1) - (द्विगुन)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                अन्यथा
                    परिणाम = false;
            }
            अन्यथा
            {
                द्विगुन d = det3(Sd);
                अगर( d != 0. )
                {
                    द्विगुन t[9];

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
                    परिणाम = false;
            }
        }
        अन्यथा
        {
            CV_Assert( src.rows == 1 );

            अगर( type == CV_32FC1 )
            {
                द्विगुन d = Sf(0,0);
                अगर( d != 0. )
                    Df(0,0) = (भग्न)(bf(0)/d);
                अन्यथा
                    परिणाम = false;
            }
            अन्यथा
            {
                द्विगुन d = Sd(0,0);
                अगर( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                अन्यथा
                    परिणाम = false;
            }
        }
        वापस परिणाम;
    }

    पूर्णांक m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    माप_प्रकार esz = CV_ELEM_आकार(type), bufsize = 0;
    माप_प्रकार vstep = align_आकार(n*esz, 16);
    माप_प्रकार astep = method == DECOMP_SVD && !is_normal ? align_आकार(m*esz, 16) : vstep;
    Auto_बफर<uchar> बफर;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    अगर( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    अगर( m == n )
        is_normal = false;
    अन्यथा अगर( is_normal )
    {
        m_ = n;
        अगर( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    माप_प्रकार asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    अगर( is_normal )
        bufsize += n*nb*esz;
    अगर( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*माप(द्विगुन) + 32;

    बफर.allocate(bufsize);
    uchar* ptr = alignPtr(बफर.तथ्य(), 16);

    Mat a(m_, n, type, ptr, astep);

    अगर( is_normal )
        mulTransposed(src, a, true);
    अन्यथा अगर( method != DECOMP_SVD )
        src.copyTo(a);
    अन्यथा
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    अगर( !is_normal )
    {
        अगर( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    अन्यथा
    {
        // a'*b
        अगर( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        अन्यथा
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    अगर( method == DECOMP_LU )
    {
        अगर( type == CV_32F )
            परिणाम = hal::LU32f(a.ptr<भग्न>(), a.step, n, dst.ptr<भग्न>(), dst.step, nb) != 0;
        अन्यथा
            परिणाम = hal::LU64f(a.ptr<द्विगुन>(), a.step, n, dst.ptr<द्विगुन>(), dst.step, nb) != 0;
    }
    अन्यथा अगर( method == DECOMP_CHOLESKY )
    {
        अगर( type == CV_32F )
            परिणाम = hal::Cholesky32f(a.ptr<भग्न>(), a.step, n, dst.ptr<भग्न>(), dst.step, nb);
        अन्यथा
            परिणाम = hal::Cholesky64f(a.ptr<द्विगुन>(), a.step, n, dst.ptr<द्विगुन>(), dst.step, nb);
    }
    अन्यथा अगर( method == DECOMP_QR )
    {
        Mat rhsMat;
        अगर( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        अन्यथा
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        अगर( type == CV_32F )
            परिणाम = hal::QR32f(a.ptr<भग्न>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<भग्न>(), rhsMat.step, शून्य) != 0;
        अन्यथा
            परिणाम = hal::QR64f(a.ptr<द्विगुन>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<द्विगुन>(), rhsMat.step, शून्य) != 0;

        अगर (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    अन्यथा
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        अगर( method == DECOMP_EIG )
        {
            अगर( type == CV_32F )
                Jacobi(a.ptr<भग्न>(), a.step, w.ptr<भग्न>(), v.ptr<भग्न>(), v.step, n, ptr);
            अन्यथा
                Jacobi(a.ptr<द्विगुन>(), a.step, w.ptr<द्विगुन>(), v.ptr<द्विगुन>(), v.step, n, ptr);
            u = v;
        }
        अन्यथा
        {
            अगर( type == CV_32F )
                JacobiSVD(a.ptr<भग्न>(), a.step, w.ptr<भग्न>(), v.ptr<भग्न>(), v.step, m_, n);
            अन्यथा
                JacobiSVD(a.ptr<द्विगुन>(), a.step, w.ptr<द्विगुन>(), v.ptr<द्विगुन>(), v.step, m_, n);
            u = a;
        }

        अगर( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<भग्न>(), 0, u.ptr<भग्न>(), u.step, true,
                   v.ptr<भग्न>(), v.step, true, src2.ptr<भग्न>(),
                   src2.step, nb, dst.ptr<भग्न>(), dst.step, ptr);
        }
        अन्यथा
        {
            SVBkSb(m_, n, w.ptr<द्विगुन>(), 0, u.ptr<द्विगुन>(), u.step, true,
                   v.ptr<द्विगुन>(), v.step, true, src2.ptr<द्विगुन>(),
                   src2.step, nb, dst.ptr<द्विगुन>(), dst.step, ptr);
        }
        परिणाम = true;
    }

    अगर( !परिणाम )
        dst = Scalar(0);

    वापस परिणाम;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

bool eigen( निवेश_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    पूर्णांक type = src.type();
    पूर्णांक n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    अगर( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#अगर_घोषित HAVE_EIGEN
    स्थिर bool evecNeeded = _evects.needed();
    स्थिर पूर्णांक esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    अगर ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        अगर ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            अगर ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            वापस true;
        }
    } अन्यथा { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        अगर ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            अगर ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            वापस true;
        }
    }
    वापस false;
#अन्यथा

    माप_प्रकार elem_आकार = src.elem_आकार(), astep = align_आकार(n*elem_आकार, 16);
    Auto_बफर<uchar> buf(n*astep + n*5*elem_आकार + 32);
    uchar* ptr = alignPtr(buf.तथ्य(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elem_आकार*n;
    src.copyTo(a);
    bool ok = type == CV_32F ?
        Jacobi(a.ptr<भग्न>(), a.step, w.ptr<भग्न>(), v.ptr<भग्न>(), v.step, n, ptr) :
        Jacobi(a.ptr<द्विगुन>(), a.step, w.ptr<द्विगुन>(), v.ptr<द्विगुन>(), v.step, n, ptr);

    w.copyTo(_evals);
    वापस ok;
#पूर्ण_अगर
}

अटल व्योम _SVDcompute( निवेश_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, पूर्णांक flags )
{
    Mat src = _aarr.getMat();
    पूर्णांक m = src.rows, n = src.cols;
    पूर्णांक type = src.type();
    bool compute_uv = _u.needed() || _vt.needed();
    bool full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    अगर( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = false;
    }

    bool at = false;
    अगर( m < n )
    {
        std::swap(m, n);
        at = true;
    }

    पूर्णांक urows = full_uv ? m : n;
    माप_प्रकार esz = src.elem_आकार(), astep = align_आकार(m*esz, 16), vstep = align_आकार(n*esz, 16);
    Auto_बफर<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.तथ्य(), 16);
    Mat अस्थायी_a(n, m, type, buf, astep);
    Mat अस्थायी_w(n, 1, type, buf + urows*astep);
    Mat अस्थायी_u(urows, m, type, buf, astep), अस्थायी_v;

    अगर( compute_uv )
        अस्थायी_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    अगर( urows > n )
        अस्थायी_u = Scalar::all(0);

    अगर( !at )
        transpose(src, अस्थायी_a);
    अन्यथा
        src.copyTo(अस्थायी_a);

    अगर( type == CV_32F )
    {
        JacobiSVD(अस्थायी_a.ptr<भग्न>(), अस्थायी_u.step, अस्थायी_w.ptr<भग्न>(),
              अस्थायी_v.ptr<भग्न>(), अस्थायी_v.step, m, n, compute_uv ? urows : 0);
    }
    अन्यथा
    {
        JacobiSVD(अस्थायी_a.ptr<द्विगुन>(), अस्थायी_u.step, अस्थायी_w.ptr<द्विगुन>(),
              अस्थायी_v.ptr<द्विगुन>(), अस्थायी_v.step, m, n, compute_uv ? urows : 0);
    }
    अस्थायी_w.copyTo(_w);
    अगर( compute_uv )
    {
        अगर( !at )
        {
            अगर( _u.needed() )
                transpose(अस्थायी_u, _u);
            अगर( _vt.needed() )
                अस्थायी_v.copyTo(_vt);
        }
        अन्यथा
        {
            अगर( _u.needed() )
                transpose(अस्थायी_v, _u);
            अगर( _vt.needed() )
                अस्थायी_u.copyTo(_vt);
        }
    }
}


व्योम SVD::compute( निवेश_Array a, OutputArray w, OutputArray u, OutputArray vt, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

व्योम SVD::compute( निवेश_Array a, OutputArray w, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

व्योम SVD::backSubst( निवेश_Array _w, निवेश_Array _u, निवेश_Array _vt,
                     निवेश_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    पूर्णांक type = w.type(), esz = (पूर्णांक)w.elem_आकार();
    पूर्णांक m = u.rows, n = vt.cols, nb = rhs.तथ्य ? rhs.cols : m, nm = std::min(m, n);
    माप_प्रकार wstep = w.rows == 1 ? (माप_प्रकार)esz : w.cols == 1 ? (माप_प्रकार)w.step : (माप_प्रकार)w.step + esz;
    Auto_बफर<uchar> बफर(nb*माप(द्विगुन) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.तथ्य && vt.तथ्य && w.तथ्य );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == आकार(nm, 1) || w.size() == आकार(1, nm) || w.size() == आकार(vt.rows, u.cols)) );
    CV_Assert( rhs.तथ्य == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    अगर( type == CV_32F )
        SVBkSb(m, n, w.ptr<भग्न>(), wstep, u.ptr<भग्न>(), u.step, false,
               vt.ptr<भग्न>(), vt.step, true, rhs.ptr<भग्न>(), rhs.step, nb,
               dst.ptr<भग्न>(), dst.step, बफर.तथ्य());
    अन्यथा अगर( type == CV_64F )
        SVBkSb(m, n, w.ptr<द्विगुन>(), wstep, u.ptr<द्विगुन>(), u.step, false,
               vt.ptr<द्विगुन>(), vt.step, true, rhs.ptr<द्विगुन>(), rhs.step, nb,
               dst.ptr<द्विगुन>(), dst.step, बफर.तथ्य());
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::चालक ()(निवेश_Array a, पूर्णांक flags)
{
    _SVDcompute(a, w, u, vt, flags);
    वापस *यह;
}


व्योम SVD::backSubst( निवेश_Array rhs, OutputArray dst ) स्थिर
{
    backSubst( w, u, vt, rhs, dst );
}

}


व्योम cv::SVDecomp(निवेश_Array src, OutputArray w, OutputArray u, OutputArray vt, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

व्योम cv::SVBackSubst(निवेश_Array w, निवेश_Array u, निवेश_Array vt, निवेश_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
