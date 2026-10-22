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
#  यदि defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4701)  // potentially uninitialized local variable
#    pragma warning(disable:4702)  // unreachable code
#    pragma warning(disable:4714)  // const marked as __forceinline not inlined
#  endif
#  include <Eigen/Core>
#  include <Eigen/Eigenvalues>
#  यदि defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#  include "opencv2/core/eigen.hpp"
#endif

नामाकाश cv
{

पूर्णाङ्क LU(भिन्न* A, माप_प्रकार astep, पूर्णाङ्क m, भिन्न* b, माप_प्रकार bstep, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    प्रत्यागम hal::LU32f(A, astep, m, b, bstep, n);
}

पूर्णाङ्क LU(द्विगुण* A, माप_प्रकार astep, पूर्णाङ्क m, द्विगुण* b, माप_प्रकार bstep, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    प्रत्यागम hal::LU64f(A, astep, m, b, bstep, n);
}

तार्किक Cholesky(भिन्न* A, माप_प्रकार astep, पूर्णाङ्क m, भिन्न* b, माप_प्रकार bstep, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    प्रत्यागम hal::Cholesky32f(A, astep, m, b, bstep, n);
}

तार्किक Cholesky(द्विगुण* A, माप_प्रकार astep, पूर्णाङ्क m, द्विगुण* b, माप_प्रकार bstep, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    प्रत्यागम hal::Cholesky64f(A, astep, m, b, bstep, n);
}

ढाँचा<typename _Tp> स्थिर अन्तःस्थ _Tp hypot(_Tp a, _Tp b)
{
    a = std::असल(a);
    b = std::असल(b);
    यदि( a > b )
    {
        b /= a;
        प्रत्यागम a*std::वर्ग_मूल(1 + b*b);
    }
    यदि( b > 0 )
    {
        a /= b;
        प्रत्यागम b*std::वर्ग_मूल(1 + a*a);
    }
    प्रत्यागम 0;
}


ढाँचा<typename _Tp> तार्किक
JacobiImpl_( _Tp* A, माप_प्रकार astep, _Tp* W, _Tp* V, माप_प्रकार vstep, पूर्णाङ्क n, uchar* buf )
{
    ध्रुव _Tp eps = std::numeric_limits<_Tp>::epsilon();
    पूर्णाङ्क i, j, k, m;

    astep /= परिमाण(A[0]);
    यदि( V )
    {
        vstep /= परिमाण(V[0]);
        प्रति( i = 0; i < n; i++ )
        {
            प्रति( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    पूर्णाङ्क iters, maxIters = n*n*30;

    पूर्णाङ्क* indR = (पूर्णाङ्क*)alignPtr(buf, परिमाण(पूर्णाङ्क));
    पूर्णाङ्क* indC = indR + n;
    _Tp mv = (_Tp)0;

    प्रति( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        यदि( k < n - 1 )
        {
            प्रति( m = k+1, mv = std::असल(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = std::असल(A[astep*k+i]);
                यदि( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        यदि( k > 0 )
        {
            प्रति( m = 0, mv = std::असल(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = std::असल(A[astep*i+k]);
                यदि( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    यदि( n > 1 ) प्रति( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        प्रति( k = 0, mv = std::असल(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = std::असल(A[astep*i + indR[i]]);
            यदि( mv < val )
                mv = val, k = i;
        }
        पूर्णाङ्क l = indR[k];
        प्रति( i = 1; i < n; i++ )
        {
            _Tp val = std::असल(A[astep*indC[i] + i]);
            यदि( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        यदि( std::असल(p) <= eps )
            भङ्ग;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = std::असल(y) + hypot(p, y);
        _Tp s = hypot(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        यदि( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        प्रति( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        प्रति( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        प्रति( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        यदि( V )
            प्रति( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        प्रति( j = 0; j < 2; j++ )
        {
            पूर्णाङ्क idx = j == 0 ? k : l;
            यदि( idx < n - 1 )
            {
                प्रति( m = idx+1, mv = std::असल(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = std::असल(A[astep*idx+i]);
                    यदि( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            यदि( idx > 0 )
            {
                प्रति( m = 0, mv = std::असल(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = std::असल(A[astep*i+idx]);
                    यदि( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    प्रति( k = 0; k < n-1; k++ )
    {
        m = k;
        प्रति( i = k+1; i < n; i++ )
        {
            यदि( W[m] < W[i] )
                m = i;
        }
        यदि( k != m )
        {
            std::swap(W[m], W[k]);
            यदि( V )
                प्रति( i = 0; i < n; i++ )
                    std::swap(V[vstep*m + i], V[vstep*k + i]);
        }
    }

    प्रत्यागम सत्य;
}

स्थिर तार्किक Jacobi( भिन्न* S, माप_प्रकार sstep, भिन्न* e, भिन्न* E, माप_प्रकार estep, पूर्णाङ्क n, uchar* buf )
{
    प्रत्यागम JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

स्थिर तार्किक Jacobi( द्विगुण* S, माप_प्रकार sstep, द्विगुण* e, द्विगुण* E, माप_प्रकार estep, पूर्णाङ्क n, uchar* buf )
{
    प्रत्यागम JacobiImpl_(S, sstep, e, E, estep, n, buf);
}


ढाँचा<typename T> संरचना VBLAS
{
    पूर्णाङ्क dot(ध्रुव T*, ध्रुव T*, पूर्णाङ्क, T*) ध्रुव { प्रत्यागम 0; }
    पूर्णाङ्क givens(T*, T*, पूर्णाङ्क, T, T) ध्रुव { प्रत्यागम 0; }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<> अन्तःस्थ पूर्णाङ्क VBLAS<भिन्न>::dot(ध्रुव भिन्न* a, ध्रुव भिन्न* b, पूर्णाङ्क n, भिन्न* result) ध्रुव
{
    यदि( n < 2*VTraits<v_float32>::vlanes() )
        प्रत्यागम 0;
    पूर्णाङ्क k = 0;
    v_float32 s0 = vx_setzero_f32();
    प्रति( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    प्रत्यागम k;
}


ढाँचा<> अन्तःस्थ पूर्णाङ्क VBLAS<भिन्न>::givens(भिन्न* a, भिन्न* b, पूर्णाङ्क n, भिन्न c, भिन्न s) ध्रुव
{
    यदि( n < VTraits<v_float32>::vlanes())
        प्रत्यागम 0;
    पूर्णाङ्क k = 0;
    v_float32 c4 = vx_setall_f32(c), s4 = vx_setall_f32(s);
    v_float32 ns4 = vx_setall_f32(-s);
    प्रति( ; k <= n - VTraits<v_float32>::vlanes(); k += VTraits<v_float32>::vlanes() )
    {
        v_float32 a0 = vx_load(a + k);
        v_float32 b0 = vx_load(b + k);
        v_float32 t0 = v_fma(a0, c4, v_mul(b0, s4));
        v_float32 t1 = v_fma(a0, ns4, v_mul(b0, c4));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    प्रत्यागम k;
}


#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ढाँचा<> अन्तःस्थ पूर्णाङ्क VBLAS<द्विगुण>::dot(ध्रुव द्विगुण* a, ध्रुव द्विगुण* b, पूर्णाङ्क n, द्विगुण* result) ध्रुव
{
    यदि( n < 2*VTraits<v_float64>::vlanes() )
        प्रत्यागम 0;
    पूर्णाङ्क k = 0;
    v_float64 s0 = vx_setzero_f64();
    प्रति( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);

        s0 = v_add(s0, v_mul(a0, b0));
    }
    *result = v_reduce_sum(s0);
    vx_cleanup();
    प्रत्यागम k;
}


ढाँचा<> अन्तःस्थ पूर्णाङ्क VBLAS<द्विगुण>::givens(द्विगुण* a, द्विगुण* b, पूर्णाङ्क n, द्विगुण c, द्विगुण s) ध्रुव
{
    पूर्णाङ्क k = 0;
    v_float64 c2 = vx_setall_f64(c), s2 = vx_setall_f64(s);
    v_float64 ns2 = vx_setall_f64(-s);
    प्रति( ; k <= n - VTraits<v_float64>::vlanes(); k += VTraits<v_float64>::vlanes() )
    {
        v_float64 a0 = vx_load(a + k);
        v_float64 b0 = vx_load(b + k);
        v_float64 t0 = v_fma(a0, c2, v_mul(b0, s2));
        v_float64 t1 = v_fma(a0, ns2, v_mul(b0, c2));
        v_store(a + k, t0);
        v_store(b + k, t1);
    }
    vx_cleanup();
    प्रत्यागम k;
}


#endif // (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
#endif // (CV_SIMD || CV_SIMD_SCALABLE)

ढाँचा<typename _Tp> शून्य
JacobiSVDImpl_(_Tp* At, माप_प्रकार astep, _Tp* _W, _Tp* Vt, माप_प्रकार vstep,
               पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क n1, द्विगुण minval, _Tp eps)
{
    VBLAS<_Tp> vblas;
    AutoBuffer<द्विगुण> Wbuf(n);
    द्विगुण* W = Wbuf.data();
    पूर्णाङ्क i, j, k, iter, max_iter = std::max(m, 30);
    _Tp c, s;
    द्विगुण sd;
    astep /= परिमाण(At[0]);
    vstep /= परिमाण(Vt[0]);

    प्रति( i = 0; i < n; i++ )
    {
        प्रति( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (द्विगुण)t*t;
        }
        W[i] = sd;

        यदि( Vt )
        {
            प्रति( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    प्रति( iter = 0; iter < max_iter; iter++ )
    {
        तार्किक changed = असत्य;

        प्रति( i = 0; i < n-1; i++ )
            प्रति( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                द्विगुण a = W[i], p = 0, b = W[j];

                प्रति( k = 0; k < m; k++ )
                    p += (द्विगुण)Ai[k]*Aj[k];

                यदि( std::असल(p) <= eps*std::वर्ग_मूल((द्विगुण)a*b) )
                    अनुवर्त;

                p *= 2;
                द्विगुण beta = a - b, gamma = hypot((द्विगुण)p, beta);
                यदि( beta < 0 )
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
                प्रति( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (द्विगुण)t0*t0; b += (द्विगुण)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = सत्य;

                यदि( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = vblas.givens(Vi, Vj, n, c, s);

                    प्रति( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        यदि( !changed )
            भङ्ग;
    }

    प्रति( i = 0; i < n; i++ )
    {
        प्रति( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (द्विगुण)t*t;
        }
        W[i] = std::वर्ग_मूल(sd);
    }

    प्रति( i = 0; i < n-1; i++ )
    {
        j = i;
        प्रति( k = i+1; k < n; k++ )
        {
            यदि( W[j] < W[k] )
                j = k;
        }
        यदि( i != j )
        {
            std::swap(W[i], W[j]);
            यदि( Vt )
            {
                प्रति( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                प्रति( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    प्रति( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    यदि( !Vt )
        प्रत्यागम;

    RNG rng(0x12345678);
    प्रति( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        प्रति( पूर्णाङ्क ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            ध्रुव _Tp val0 = (_Tp)(1./m);
            प्रति( k = 0; k < m; k++ )
            {
                _Tp val = (rng.next() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            प्रति( iter = 0; iter < 2; iter++ )
            {
                प्रति( j = 0; j < i; j++ )
                {
                    sd = 0;
                    प्रति( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    प्रति( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::असल(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    प्रति( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            प्रति( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (द्विगुण)t*t;
            }
            sd = std::वर्ग_मूल(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        प्रति( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}


स्थिर शून्य JacobiSVD(भिन्न* At, माप_प्रकार astep, भिन्न* W, भिन्न* Vt, माप_प्रकार vstep, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क n1=-1)
{
    hal::SVD32f(At, astep, W, रिक्त, astep, Vt, vstep, m, n, n1);
}

स्थिर शून्य JacobiSVD(द्विगुण* At, माप_प्रकार astep, द्विगुण* W, द्विगुण* Vt, माप_प्रकार vstep, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क n1=-1)
{
    hal::SVD64f(At, astep, W, रिक्त, astep, Vt, vstep, m, n, n1);
}

ढाँचा <typename fptype> स्थिर अन्तःस्थ पूर्णाङ्क
decodeSVDParameters(ध्रुव fptype* U, ध्रुव fptype* Vt, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क n1)
{
    पूर्णाङ्क halSVDFlag = 0;
    यदि(Vt == रिक्त)
        halSVDFlag = CV_HAL_SVD_NO_UV;
    अन्यथा यदि(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CV_HAL_SVD_SHORT_UV;
        यदि(U == रिक्त)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    अन्यथा यदि(n1 == m)
    {
        halSVDFlag = CV_HAL_SVD_FULL_UV;
        यदि(U == रिक्त)
            halSVDFlag |= CV_HAL_SVD_MODIFY_A;
    }
    प्रत्यागम halSVDFlag;
}

शून्य hal::SVD32f(भिन्न* At, माप_प्रकार astep, भिन्न* W, भिन्न* U, माप_प्रकार ustep, भिन्न* Vt, माप_प्रकार vstep, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, भग्न_न्यून, भग्न_अंतर*2);
}

शून्य hal::SVD64f(द्विगुण* At, माप_प्रकार astep, द्विगुण* W, द्विगुण* U, माप_प्रकार ustep, द्विगुण* Vt, माप_प्रकार vstep, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, द्विग_न्यून, द्विग_अंतर*10);
}

/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
ढाँचा<typename T1, typename T2, typename T3> स्थिर शून्य
MatrAXPY( पूर्णाङ्क m, पूर्णाङ्क n, ध्रुव T1* x, पूर्णाङ्क dx,
         ध्रुव T2* a, पूर्णाङ्क inca, T3* y, पूर्णाङ्क dy )
{
    पूर्णाङ्क i;
    प्रति( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        पूर्णाङ्क j = 0;
         #यदि CV_ENABLE_UNROLLED
        प्रति(; j <= n - 4; j += 4 )
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
        प्रति( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

ढाँचा<typename T> स्थिर शून्य
SVBkSbImpl_( पूर्णाङ्क m, पूर्णाङ्क n, ध्रुव T* w, पूर्णाङ्क incw,
       ध्रुव T* u, पूर्णाङ्क ldu, तार्किक uT,
       ध्रुव T* v, पूर्णाङ्क ldv, तार्किक vT,
       ध्रुव T* b, पूर्णाङ्क ldb, पूर्णाङ्क nb,
       T* x, पूर्णाङ्क ldx, द्विगुण* buffer, T eps )
{
    द्विगुण threshold = 0;
    पूर्णाङ्क udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    पूर्णाङ्क vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    पूर्णाङ्क i, j, nm = std::min(m, n);

    यदि( !b )
        nb = m;

    प्रति( i = 0; i < n; i++ )
        प्रति( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    प्रति( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    प्रति( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        द्विगुण wi = w[i*incw];
        यदि( (द्विगुण)std::असल(wi) <= threshold )
            अनुवर्त;
        wi = 1/wi;

        यदि( nb == 1 )
        {
            द्विगुण s = 0;
            यदि( b )
                प्रति( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            अन्यथा
                s = u[0];
            s *= wi;

            प्रति( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        अन्यथा
        {
            यदि( b )
            {
                प्रति( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                प्रति( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            अन्यथा
            {
                प्रति( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}

स्थिर शून्य
SVBkSb( पूर्णाङ्क m, पूर्णाङ्क n, ध्रुव भिन्न* w, माप_प्रकार wstep,
        ध्रुव भिन्न* u, माप_प्रकार ustep, तार्किक uT,
        ध्रुव भिन्न* v, माप_प्रकार vstep, तार्किक vT,
        ध्रुव भिन्न* b, माप_प्रकार bstep, पूर्णाङ्क nb,
        भिन्न* x, माप_प्रकार xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (पूर्णाङ्क)(wstep/परिमाण(w[0])) : 1,
                u, (पूर्णाङ्क)(ustep/परिमाण(u[0])), uT,
                v, (पूर्णाङ्क)(vstep/परिमाण(v[0])), vT,
                b, (पूर्णाङ्क)(bstep/परिमाण(b[0])), nb,
                x, (पूर्णाङ्क)(xstep/परिमाण(x[0])),
                (द्विगुण*)alignPtr(buffer, परिमाण(द्विगुण)), (भिन्न)(द्विग_अंतर*2) );
}

स्थिर शून्य
SVBkSb( पूर्णाङ्क m, पूर्णाङ्क n, ध्रुव द्विगुण* w, माप_प्रकार wstep,
       ध्रुव द्विगुण* u, माप_प्रकार ustep, तार्किक uT,
       ध्रुव द्विगुण* v, माप_प्रकार vstep, तार्किक vT,
       ध्रुव द्विगुण* b, माप_प्रकार bstep, पूर्णाङ्क nb,
       द्विगुण* x, माप_प्रकार xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (पूर्णाङ्क)(wstep/परिमाण(w[0])) : 1,
                u, (पूर्णाङ्क)(ustep/परिमाण(u[0])), uT,
                v, (पूर्णाङ्क)(vstep/परिमाण(v[0])), vT,
                b, (पूर्णाङ्क)(bstep/परिमाण(b[0])), nb,
                x, (पूर्णाङ्क)(xstep/परिमाण(x[0])),
                (द्विगुण*)alignPtr(buffer, परिमाण(द्विगुण)), द्विग_अंतर*2 );
}

/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define det2(m)   ((द्विगुण)m(0,0)*m(1,1) - (द्विगुण)m(0,1)*m(1,0))
#define det3(m)   (m(0,0)*((द्विगुण)m(1,1)*m(2,2) - (द्विगुण)m(1,2)*m(2,1)) -  \
                   m(0,1)*((द्विगुण)m(1,0)*m(2,2) - (द्विगुण)m(1,2)*m(2,0)) +  \
                   m(0,2)*((द्विगुण)m(1,0)*m(2,1) - (द्विगुण)m(1,1)*m(2,0)))

द्विगुण determinant( निवेश_Array _mat )
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    द्विगुण result = 0;
    पूर्णाङ्क type = mat.type(), rows = mat.rows;
    माप_प्रकार step = mat.step;
    ध्रुव uchar* m = mat.ptr();

    CV_Assert( !mat.empty() );
    CV_Assert( mat.rows == mat.cols && (type == CV_32F || type == CV_64F));

    #define Mf(y, x) ((भिन्न*)(m + y*step))[x]
    #define Md(y, x) ((द्विगुण*)(m + y*step))[x]

    यदि( type == CV_32F )
    {
        यदि( rows == 2 )
            result = det2(Mf);
        अन्यथा यदि( rows == 3 )
            result = det3(Mf);
        अन्यथा यदि( rows == 1 )
            result = Mf(0,0);
        अन्यथा
        {
            माप_प्रकार bufSize = rows*rows*परिमाण(भिन्न);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_32F, buffer.data());
            mat.copyTo(a);

            result = hal::LU32f(a.ptr<भिन्न>(), a.step, rows, 0, 0, 0);
            यदि( result )
            {
                प्रति( पूर्णाङ्क i = 0; i < rows; i++ )
                    result *= a.at<भिन्न>(i,i);
            }
        }
    }
    अन्यथा
    {
        यदि( rows == 2 )
            result = det2(Md);
        अन्यथा यदि( rows == 3 )
            result = det3(Md);
        अन्यथा यदि( rows == 1 )
            result = Md(0,0);
        अन्यथा
        {
            माप_प्रकार bufSize = rows*rows*परिमाण(द्विगुण);
            AutoBuffer<uchar> buffer(bufSize);
            Mat a(rows, rows, CV_64F, buffer.data());
            mat.copyTo(a);

            result = hal::LU64f(a.ptr<द्विगुण>(), a.step, rows, 0, 0, 0);
            यदि( result )
            {
                प्रति( पूर्णाङ्क i = 0; i < rows; i++ )
                    result *= a.at<द्विगुण>(i,i);
            }
        }
    }

    #undef Mf
    #undef Md

    प्रत्यागम result;
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((भिन्न*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((द्विगुण*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((भिन्न*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((द्विगुण*)(dstdata + y*dststep))[x]

द्विगुण invert( निवेश_Array _src, OutputArray _dst, पूर्णाङ्क method )
{
    CV_INSTRUMENT_REGION();

    तार्किक result = असत्य;
    Mat src = _src.getMat();
    पूर्णाङ्क type = src.type();

    CV_Assert(type == CV_32F || type == CV_64F);

    माप_प्रकार esz = CV_ELEM_SIZE(type);
    पूर्णाङ्क m = src.rows, n = src.cols;

    यदि( method == DECOMP_SVD )
    {
        पूर्णाङ्क nm = std::min(m, n);

        AutoBuffer<uchar> _buf((m*nm + nm + nm*n)*esz + परिमाण(द्विगुण));
        uchar* buf = alignPtr((uchar*)_buf.data(), (पूर्णाङ्क)esz);
        Mat u(m, nm, type, buf);
        Mat w(nm, 1, type, u.ptr() + m*nm*esz);
        Mat vt(nm, n, type, w.ptr() + nm*esz);

        SVD::compute(src, w, u, vt);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        प्रत्यागम type == CV_32F ?
            (w.ptr<भिन्न>()[0] >= भग्न_अंतर ?
             w.ptr<भिन्न>()[n-1]/w.ptr<भिन्न>()[0] : 0) :
            (w.ptr<द्विगुण>()[0] >= द्विग_अंतर ?
             w.ptr<द्विगुण>()[n-1]/w.ptr<द्विगुण>()[0] : 0);
    }

    CV_Assert( m == n );

    यदि( method == DECOMP_EIG )
    {
        AutoBuffer<uchar> _buf((n*n*2 + n)*esz + परिमाण(द्विगुण));
        uchar* buf = alignPtr((uchar*)_buf.data(), (पूर्णाङ्क)esz);
        Mat u(n, n, type, buf);
        Mat w(n, 1, type, u.ptr() + n*n*esz);
        Mat vt(n, n, type, w.ptr() + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD::backSubst(w, u, vt, Mat(), _dst);
        प्रत्यागम type == CV_32F ?
        (w.ptr<भिन्न>()[0] >= भग्न_अंतर ?
         w.ptr<भिन्न>()[n-1]/w.ptr<भिन्न>()[0] : 0) :
        (w.ptr<द्विगुण>()[0] >= द्विग_अंतर ?
         w.ptr<द्विगुण>()[n-1]/w.ptr<द्विगुण>()[0] : 0);
    }

    CV_Assert( method == DECOMP_LU || method == DECOMP_CHOLESKY );

    _dst.create( n, n, type );
    Mat dst = _dst.getMat();

    यदि( n <= 3 )
    {
        ध्रुव uchar* srcdata = src.ptr();
        uchar* dstdata = dst.ptr();
        माप_प्रकार srcstep = src.step;
        माप_प्रकार dststep = dst.step;

        यदि( n == 2 )
        {
            यदि( type == CV_32FC1 )
            {
                द्विगुण d = det2(Sf);
                यदि( d != 0. )
                {
                    result = सत्य;
                    d = 1./d;
                #यदि CV_SIMD128
                    ध्रुव भिन्न d_32f = (भिन्न)d;
                    ध्रुव v_float32x4 d_vec(d_32f, -d_32f, -d_32f, d_32f);
                    v_float32x4 s0 = v_mul(v_load_halves((ध्रुव भिन्न *)srcdata, (ध्रुव भिन्न *)(srcdata + srcstep)), d_vec);//0123//3120
                    s0 = v_extract<3>(s0, v_combine_निचला(v_rotate_दायाँ<1>(s0), s0));
                    v_store_निचला((भिन्न*)dstdata, s0);
                    v_store_ऊपरी((भिन्न*)(dstdata + dststep), s0);
                #अन्यथा
                    द्विगुण t0, t1;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (भिन्न)t0;
                    Df(0,0) = (भिन्न)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (भिन्न)t0;
                    Df(1,0) = (भिन्न)t1;
                #endif
                }
            }
            अन्यथा
            {
                द्विगुण d = det2(Sd);
                यदि( d != 0. )
                {
                    result = सत्य;
                    d = 1./d;
                #यदि CV_SIMD128_64F
                    v_float64x2 det = v_setall_f64(d);
                    v_float64x2 s0 = v_mul(v_load((ध्रुव द्विगुण *)srcdata), det);
                    v_float64x2 s1 = v_mul(v_load((ध्रुव द्विगुण *)(srcdata + srcstep)), det);
                    v_float64x2 sm = v_extract<1>(s1, s0);//30
                    v_float64x2 ss = v_sub(v_setall<द्विगुण>(0), v_extract<1>(s0, s1));//12
                    v_store((द्विगुण*)dstdata, v_combine_निचला(sm, ss));//31
                    v_store((द्विगुण*)(dstdata + dststep), v_combine_ऊपरी(ss, sm));//20
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
        अन्यथा यदि( n == 3 )
        {
            यदि( type == CV_32FC1 )
            {
                द्विगुण d = det3(Sf);

                यदि( d != 0. )
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

                    Df(0,0) = (भिन्न)t[0]; Df(0,1) = (भिन्न)t[1]; Df(0,2) = (भिन्न)t[2];
                    Df(1,0) = (भिन्न)t[3]; Df(1,1) = (भिन्न)t[4]; Df(1,2) = (भिन्न)t[5];
                    Df(2,0) = (भिन्न)t[6]; Df(2,1) = (भिन्न)t[7]; Df(2,2) = (भिन्न)t[8];
                }
            }
            अन्यथा
            {
                द्विगुण d = det3(Sd);
                यदि( d != 0. )
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

            यदि( type == CV_32FC1 )
            {
                द्विगुण d = Sf(0,0);
                यदि( d != 0. )
                {
                    result = सत्य;
                    Df(0,0) = (भिन्न)(1./d);
                }
            }
            अन्यथा
            {
                द्विगुण d = Sd(0,0);
                यदि( d != 0. )
                {
                    result = सत्य;
                    Dd(0,0) = 1./d;
                }
            }
        }
        यदि( !result )
            dst = Scalar(0);
        प्रत्यागम result;
    }

   पूर्णाङ्क elem_size = CV_ELEM_SIZE(type);
    AutoBuffer<uchar> buf(n*n*elem_size);
    Mat src1(n, n, type, buf.data());
    src.copyTo(src1);
    setIdentity(dst);

    यदि( method == DECOMP_LU && type == CV_32F )
        result = hal::LU32f(src1.ptr<भिन्न>(), src1.step, n, dst.ptr<भिन्न>(), dst.step, n) != 0;
    अन्यथा यदि( method == DECOMP_LU && type == CV_64F )
        result = hal::LU64f(src1.ptr<द्विगुण>(), src1.step, n, dst.ptr<द्विगुण>(), dst.step, n) != 0;
    अन्यथा यदि( method == DECOMP_CHOLESKY && type == CV_32F )
        result = hal::Cholesky32f(src1.ptr<भिन्न>(), src1.step, n, dst.ptr<भिन्न>(), dst.step, n);
    अन्यथा
        result = hal::Cholesky64f(src1.ptr<द्विगुण>(), src1.step, n, dst.ptr<द्विगुण>(), dst.step, n);

    यदि( !result )
        dst = Scalar(0);

    प्रत्यागम result;
}

UMat UMat::inv(पूर्णाङ्क method) ध्रुव
{
    UMat m;
    invert(*एतत्, m, method);
    प्रत्यागम m;
}


/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

तार्किक solve( निवेश_Array _src, निवेश_Array _src2arg, OutputArray _dst, पूर्णाङ्क method )
{
    CV_INSTRUMENT_REGION();

    तार्किक result = सत्य;
    Mat src = _src.getMat(), _src2 = _src2arg.getMat();
    पूर्णाङ्क type = src.type();
    तार्किक is_normal = (method & DECOMP_NORMAL) != 0;

    CV_Assert( type == _src2.type() && (type == CV_32F || type == CV_64F) );

    method &= ~DECOMP_NORMAL;
    CV_Check(method, method == DECOMP_LU || method == DECOMP_SVD || method == DECOMP_EIG ||
                     method == DECOMP_CHOLESKY || method == DECOMP_QR,
             "Unsupported method, see #DecompTypes");
    CV_Assert( (method != DECOMP_LU && method != DECOMP_CHOLESKY) ||
        is_normal || src.rows == src.cols );

    // check case of a single equation and small matrix
    यदि( (method == DECOMP_LU || method == DECOMP_CHOLESKY) && !is_normal &&
        src.rows <= 3 && src.rows == src.cols && _src2.cols == 1 )
    {
        _dst.create( src.cols, _src2.cols, src.type() );
        Mat dst = _dst.getMat();

        #define bf(y) ((भिन्न*)(bdata + y*src2step))[0]
        #define bd(y) ((द्विगुण*)(bdata + y*src2step))[0]

        ध्रुव uchar* srcdata = src.ptr();
        ध्रुव uchar* bdata = _src2.ptr();
        uchar* dstdata = dst.ptr();
        माप_प्रकार srcstep = src.step;
        माप_प्रकार src2step = _src2.step;
        माप_प्रकार dststep = dst.step;

        यदि( src.rows == 2 )
        {
            यदि( type == CV_32FC1 )
            {
                द्विगुण d = det2(Sf);
                यदि( d != 0. )
                {
                    द्विगुण t;
                    d = 1./d;
                    t = (भिन्न)(((द्विगुण)bf(0)*Sf(1,1) - (द्विगुण)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (भिन्न)(((द्विगुण)bf(1)*Sf(0,0) - (द्विगुण)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (भिन्न)t;
                }
                अन्यथा
                    result = असत्य;
            }
            अन्यथा
            {
                द्विगुण d = det2(Sd);
                यदि( d != 0. )
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
        अन्यथा यदि( src.rows == 3 )
        {
            यदि( type == CV_32FC1 )
            {
                द्विगुण d = det3(Sf);
                यदि( d != 0. )
                {
                    भिन्न t[3];
                    d = 1./d;

                    t[0] = (भिन्न)(d*
                           (bf(0)*((द्विगुण)Sf(1,1)*Sf(2,2) - (द्विगुण)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((द्विगुण)bf(1)*Sf(2,2) - (द्विगुण)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((द्विगुण)bf(1)*Sf(2,1) - (द्विगुण)Sf(1,1)*bf(2))));

                    t[1] = (भिन्न)(d*
                           (Sf(0,0)*(द्विगुण)(bf(1)*Sf(2,2) - (द्विगुण)Sf(1,2)*bf(2)) -
                            bf(0)*((द्विगुण)Sf(1,0)*Sf(2,2) - (द्विगुण)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((द्विगुण)Sf(1,0)*bf(2) - (द्विगुण)bf(1)*Sf(2,0))));

                    t[2] = (भिन्न)(d*
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
                यदि( d != 0. )
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

            यदि( type == CV_32FC1 )
            {
                द्विगुण d = Sf(0,0);
                यदि( d != 0. )
                    Df(0,0) = (भिन्न)(bf(0)/d);
                अन्यथा
                    result = असत्य;
            }
            अन्यथा
            {
                द्विगुण d = Sd(0,0);
                यदि( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                अन्यथा
                    result = असत्य;
            }
        }
        प्रत्यागम result;
    }

    पूर्णाङ्क m = src.rows, m_ = m, n = src.cols, nb = _src2.cols;
    माप_प्रकार esz = CV_ELEM_SIZE(type), bufsize = 0;
    माप_प्रकार vstep = alignSize(n*esz, 16);
    माप_प्रकार astep = method == DECOMP_SVD && !is_normal ? alignSize(m*esz, 16) : vstep;
    AutoBuffer<uchar> buffer;

    Mat src2 = _src2;
    _dst.create( src.cols, src2.cols, src.type() );
    Mat dst = _dst.getMat();

    यदि( m < n )
        CV_Error(cv::Error::StsBadArg, "The function can not solve under-determined linear systems" );

    यदि( m == n )
        is_normal = असत्य;
    अन्यथा यदि( is_normal )
    {
        m_ = n;
        यदि( method == DECOMP_SVD )
            method = DECOMP_EIG;
    }

    CV_Assert(m == src2.rows);
    माप_प्रकार asize = astep*(method == DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    यदि( is_normal )
        bufsize += n*nb*esz;
    यदि( method == DECOMP_SVD || method == DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*परिमाण(द्विगुण) + 32;

    buffer.allocate(bufsize);
    uchar* ptr = alignPtr(buffer.data(), 16);

    Mat a(m_, n, type, ptr, astep);

    यदि( is_normal )
        mulTransposed(src, a, सत्य);
    अन्यथा यदि( method != DECOMP_SVD )
        src.copyTo(a);
    अन्यथा
    {
        a = Mat(n, m_, type, ptr, astep);
        transpose(src, a);
    }
    ptr += asize;

    यदि( !is_normal )
    {
        यदि( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            src2.copyTo(dst);
    }
    अन्यथा
    {
        // a'*b
        यदि( method == DECOMP_LU || method == DECOMP_CHOLESKY )
            gemm( src, src2, 1, Mat(), 0, dst, GEMM_1_T );
        अन्यथा
        {
            Mat tmp(n, nb, type, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, Mat(), 0, tmp, GEMM_1_T );
            src2 = tmp;
        }
    }

    यदि( method == DECOMP_LU )
    {
        यदि( type == CV_32F )
            result = hal::LU32f(a.ptr<भिन्न>(), a.step, n, dst.ptr<भिन्न>(), dst.step, nb) != 0;
        अन्यथा
            result = hal::LU64f(a.ptr<द्विगुण>(), a.step, n, dst.ptr<द्विगुण>(), dst.step, nb) != 0;
    }
    अन्यथा यदि( method == DECOMP_CHOLESKY )
    {
        यदि( type == CV_32F )
            result = hal::Cholesky32f(a.ptr<भिन्न>(), a.step, n, dst.ptr<भिन्न>(), dst.step, nb);
        अन्यथा
            result = hal::Cholesky64f(a.ptr<द्विगुण>(), a.step, n, dst.ptr<द्विगुण>(), dst.step, nb);
    }
    अन्यथा यदि( method == DECOMP_QR )
    {
        Mat rhsMat;
        यदि( is_normal || m == n )
        {
            src2.copyTo(dst);
            rhsMat = dst;
        }
        अन्यथा
        {
            rhsMat = Mat(m, nb, type);
            src2.copyTo(rhsMat);
        }

        यदि( type == CV_32F )
            result = hal::QR32f(a.ptr<भिन्न>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<भिन्न>(), rhsMat.step, रिक्त) != 0;
        अन्यथा
            result = hal::QR64f(a.ptr<द्विगुण>(), a.step, a.rows, a.cols, rhsMat.cols, rhsMat.ptr<द्विगुण>(), rhsMat.step, रिक्त) != 0;

        यदि (rhsMat.rows != dst.rows)
            rhsMat.rowRange(0, dst.rows).copyTo(dst);
    }
    अन्यथा
    {
        ptr = alignPtr(ptr, 16);
        Mat v(n, n, type, ptr, vstep), w(n, 1, type, ptr + vstep*n), u;
        ptr += n*(vstep + esz);

        यदि( method == DECOMP_EIG )
        {
            यदि( type == CV_32F )
                Jacobi(a.ptr<भिन्न>(), a.step, w.ptr<भिन्न>(), v.ptr<भिन्न>(), v.step, n, ptr);
            अन्यथा
                Jacobi(a.ptr<द्विगुण>(), a.step, w.ptr<द्विगुण>(), v.ptr<द्विगुण>(), v.step, n, ptr);
            u = v;
        }
        अन्यथा
        {
            यदि( type == CV_32F )
                JacobiSVD(a.ptr<भिन्न>(), a.step, w.ptr<भिन्न>(), v.ptr<भिन्न>(), v.step, m_, n);
            अन्यथा
                JacobiSVD(a.ptr<द्विगुण>(), a.step, w.ptr<द्विगुण>(), v.ptr<द्विगुण>(), v.step, m_, n);
            u = a;
        }

        यदि( type == CV_32F )
        {
            SVBkSb(m_, n, w.ptr<भिन्न>(), 0, u.ptr<भिन्न>(), u.step, सत्य,
                   v.ptr<भिन्न>(), v.step, सत्य, src2.ptr<भिन्न>(),
                   src2.step, nb, dst.ptr<भिन्न>(), dst.step, ptr);
        }
        अन्यथा
        {
            SVBkSb(m_, n, w.ptr<द्विगुण>(), 0, u.ptr<द्विगुण>(), u.step, सत्य,
                   v.ptr<द्विगुण>(), v.step, सत्य, src2.ptr<द्विगुण>(),
                   src2.step, nb, dst.ptr<द्विगुण>(), dst.step, ptr);
        }
        result = सत्य;
    }

    यदि( !result )
        dst = Scalar(0);

    प्रत्यागम result;
}


/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

तार्किक eigen( निवेश_Array _src, OutputArray _evals, OutputArray _evects )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    पूर्णाङ्क type = src.type();
    पूर्णाङ्क n = src.rows;

    CV_Assert( src.rows == src.cols );
    CV_Assert (type == CV_32F || type == CV_64F);

    Mat v;
    यदि( _evects.needed() )
    {
        _evects.create(n, n, type);
        v = _evects.getMat();
    }

#ifdef HAVE_EIGEN
    ध्रुव तार्किक evecNeeded = _evects.needed();
    ध्रुव पूर्णाङ्क esOptions = evecNeeded ? Eigen::ComputeEigenvectors : Eigen::EigenvaluesOnly;
    _evals.create(n, 1, type);
    Mat evals = _evals.getMat();
    यदि ( type == CV_64F )
    {
        Eigen::MatrixXd src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es;
        es.compute(src_eig, esOptions);
        यदि ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            यदि ( evecNeeded )
            {
                cv::Mat evects = _evects.getMat();
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            प्रत्यागम सत्य;
        }
    } अन्यथा { // CV_32F
        Eigen::MatrixXf src_eig, zeros_eig;
        cv::cv2eigen(src, src_eig);

        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> es;
        es.compute(src_eig, esOptions);
        यदि ( es.info() == Eigen::Success )
        {
            cv::eigen2cv(es.eigenvalues().reverse().eval(), evals);
            यदि ( evecNeeded )
            {
                cv::eigen2cv(es.eigenvectors().rowwise().reverse().transpose().eval(), v);
            }
            प्रत्यागम सत्य;
        }
    }
    प्रत्यागम असत्य;
#अन्यथा

    माप_प्रकार elemSize = src.elemSize(), astep = alignSize(n*elemSize, 16);
    AutoBuffer<uchar> buf(n*astep + n*5*elemSize + 32);
    uchar* ptr = alignPtr(buf.data(), 16);
    Mat a(n, n, type, ptr, astep), w(n, 1, type, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    src.copyTo(a);
    तार्किक ok = type == CV_32F ?
        Jacobi(a.ptr<भिन्न>(), a.step, w.ptr<भिन्न>(), v.ptr<भिन्न>(), v.step, n, ptr) :
        Jacobi(a.ptr<द्विगुण>(), a.step, w.ptr<द्विगुण>(), v.ptr<द्विगुण>(), v.step, n, ptr);

    w.copyTo(_evals);
    प्रत्यागम ok;
#endif
}

स्थिर शून्य _SVDcompute( निवेश_Array _aarr, OutputArray _w,
                         OutputArray _u, OutputArray _vt, पूर्णाङ्क flags )
{
    Mat src = _aarr.getMat();
    पूर्णाङ्क m = src.rows, n = src.cols;
    पूर्णाङ्क type = src.type();
    तार्किक compute_uv = _u.needed() || _vt.needed();
    तार्किक full_uv = (flags & SVD::FULL_UV) != 0;

    CV_Assert( type == CV_32F || type == CV_64F );

    यदि( flags & SVD::NO_UV )
    {
        _u.release();
        _vt.release();
        compute_uv = full_uv = असत्य;
    }

    तार्किक at = असत्य;
    यदि( m < n )
    {
        std::swap(m, n);
        at = सत्य;
    }

    पूर्णाङ्क urows = full_uv ? m : n;
    माप_प्रकार esz = src.elemSize(), astep = alignSize(m*esz, 16), vstep = alignSize(n*esz, 16);
    AutoBuffer<uchar> _buf(urows*astep + n*vstep + n*esz + 32);
    uchar* buf = alignPtr(_buf.data(), 16);
    Mat temp_a(n, m, type, buf, astep);
    Mat temp_w(n, 1, type, buf + urows*astep);
    Mat temp_u(urows, m, type, buf, astep), temp_v;

    यदि( compute_uv )
        temp_v = Mat(n, n, type, alignPtr(buf + urows*astep + n*esz, 16), vstep);

    यदि( urows > n )
        temp_u = Scalar::all(0);

    यदि( !at )
        transpose(src, temp_a);
    अन्यथा
        src.copyTo(temp_a);

    यदि( type == CV_32F )
    {
        JacobiSVD(temp_a.ptr<भिन्न>(), temp_u.step, temp_w.ptr<भिन्न>(),
              temp_v.ptr<भिन्न>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    अन्यथा
    {
        JacobiSVD(temp_a.ptr<द्विगुण>(), temp_u.step, temp_w.ptr<द्विगुण>(),
              temp_v.ptr<द्विगुण>(), temp_v.step, m, n, compute_uv ? urows : 0);
    }
    temp_w.copyTo(_w);
    यदि( compute_uv )
    {
        यदि( !at )
        {
            यदि( _u.needed() )
                transpose(temp_u, _u);
            यदि( _vt.needed() )
                temp_v.copyTo(_vt);
        }
        अन्यथा
        {
            यदि( _u.needed() )
                transpose(temp_v, _u);
            यदि( _vt.needed() )
                temp_u.copyTo(_vt);
        }
    }
}


शून्य SVD::compute( निवेश_Array a, OutputArray w, OutputArray u, OutputArray vt, पूर्णाङ्क flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, u, vt, flags);
}

शून्य SVD::compute( निवेश_Array a, OutputArray w, पूर्णाङ्क flags )
{
    CV_INSTRUMENT_REGION();

    _SVDcompute(a, w, noArray(), noArray(), flags);
}

शून्य SVD::backSubst( निवेश_Array _w, निवेश_Array _u, निवेश_Array _vt,
                     निवेश_Array _rhs, OutputArray _dst )
{
    Mat w = _w.getMat(), u = _u.getMat(), vt = _vt.getMat(), rhs = _rhs.getMat();
    पूर्णाङ्क type = w.type(), esz = (पूर्णाङ्क)w.elemSize();
    पूर्णाङ्क m = u.rows, n = vt.cols, nb = rhs.data ? rhs.cols : m, nm = std::min(m, n);
    माप_प्रकार wstep = w.rows == 1 ? (माप_प्रकार)esz : w.cols == 1 ? (माप_प्रकार)w.step : (माप_प्रकार)w.step + esz;
    AutoBuffer<uchar> buffer(nb*परिमाण(द्विगुण) + 16);
    CV_Assert( w.type() == u.type() && u.type() == vt.type() && u.data && vt.data && w.data );
    CV_Assert( u.cols >= nm && vt.rows >= nm &&
              (w.size() == Size(nm, 1) || w.size() == Size(1, nm) || w.size() == Size(vt.rows, u.cols)) );
    CV_Assert( rhs.data == 0 || (rhs.type() == type && rhs.rows == m) );

    _dst.create( n, nb, type );
    Mat dst = _dst.getMat();
    यदि( type == CV_32F )
        SVBkSb(m, n, w.ptr<भिन्न>(), wstep, u.ptr<भिन्न>(), u.step, असत्य,
               vt.ptr<भिन्न>(), vt.step, सत्य, rhs.ptr<भिन्न>(), rhs.step, nb,
               dst.ptr<भिन्न>(), dst.step, buffer.data());
    अन्यथा यदि( type == CV_64F )
        SVBkSb(m, n, w.ptr<द्विगुण>(), wstep, u.ptr<द्विगुण>(), u.step, असत्य,
               vt.ptr<द्विगुण>(), vt.step, सत्य, rhs.ptr<द्विगुण>(), rhs.step, nb,
               dst.ptr<द्विगुण>(), dst.step, buffer.data());
    अन्यथा
        CV_Error( cv::Error::StsUnsupportedFormat, "" );
}


SVD& SVD::चालक ()(निवेश_Array a, पूर्णाङ्क flags)
{
    _SVDcompute(a, w, u, vt, flags);
    प्रत्यागम *एतत्;
}


शून्य SVD::backSubst( निवेश_Array rhs, OutputArray dst ) ध्रुव
{
    backSubst( w, u, vt, rhs, dst );
}

}


शून्य cv::SVDecomp(निवेश_Array src, OutputArray w, OutputArray u, OutputArray vt, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();

    SVD::compute(src, w, u, vt, flags);
}

शून्य cv::SVBackSubst(निवेश_Array w, निवेश_Array u, निवेश_Array vt, निवेश_Array rhs, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    SVD::backSubst(w, u, vt, rhs, dst);
}
