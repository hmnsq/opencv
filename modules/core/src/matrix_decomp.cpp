// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> static inline цео
LUImpl(_Tp* A, size_t astep, цео m, _Tp* b, size_t bstep, цео n, _Tp eps)
{
    цео i, j, k, p = 1;
    astep /= sizeof(A[0]);
    bstep /= sizeof(b[0]);

    за( i = 0; i < m; i++ )
    {
        k = i;

        за( j = i+1; j < m; j++ )
            ако( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        ако( std::abs(A[k*astep + i]) < eps )
            врати 0;

        ако( k != i )
        {
            за( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ако( b )
                за( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        за( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            за( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ако( b )
                за( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ако( b )
    {
        за( i = m-1; i >= 0; i-- )
            за( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                за( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    врати p;
}


цео LU32f(float* A, size_t astep, цео m, float* b, size_t bstep, цео n)
{
    CV_INSTRUMENT_REGION();

    цео output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    врати output;
}


цео LU64f(double* A, size_t astep, цео m, double* b, size_t bstep, цео n)
{
    CV_INSTRUMENT_REGION();

    цео output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    врати output;
}

template<typename _Tp> static inline bool
CholImpl(_Tp* A, size_t astep, цео m, _Tp* b, size_t bstep, цео n)
{
    _Tp* L = A;
    цео i, j, k;
    double s;
    astep /= sizeof(A[0]);
    bstep /= sizeof(b[0]);

    за( i = 0; i < m; i++ )
    {
        за( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            за( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        за( k = 0; k < j; k++ )
        {
            double t = L[i*astep + k];
            s -= t*t;
        }
        ако( s < std::numeric_limits<_Tp>::epsilon() )
            врати false;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    ако (!b)
    {
        за( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        врати true;
   }

    // LLt x = b
    // 1: L y = b
    // 2. Lt x = y

    /*
     [ L00             ]  y0   b0
     [ L10 L11         ]  y1 = b1
     [ L20 L21 L22     ]  y2   b2
     [ L30 L31 L32 L33 ]  y3   b3

     [ L00 L10 L20 L30 ]  x0   y0
     [     L11 L21 L31 ]  x1 = y1
     [         L22 L32 ]  x2   y2
     [             L33 ]  x3   y3
     */

    за( i = 0; i < m; i++ )
    {
        за( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            за( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    за( i = m-1; i >= 0; i-- )
    {
        за( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            за( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    за( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    врати true;
}

bool Cholesky32f(float* A, size_t astep, цео m, float* b, size_t bstep, цео n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    врати CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky64f(double* A, size_t astep, цео m, double* b, size_t bstep, цео n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    врати CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> inline static цео
sign(_Tp x)
{
    ако (x >= (_Tp)0)
        врати 1;
    инако
        врати -1;
}

template<typename _Tp> static inline цео
QRImpl(_Tp* A, size_t astep, цео m, цео n, цео k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= sizeof(_Tp);
    bstep /= sizeof(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != NULL;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    ако (hFactors == NULL)
        hFactors = vl + m;

    за (цео l = 0; l < n; l++)
    {
        //generate vl
        цео vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        за (цео i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        за (цео i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        за (цео j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            за (цео i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            за (цео i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        за (цео i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ако (b)
    {
        //generate new rhs
        за (цео l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            за (цео j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            за (цео j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                за (цео i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                за (цео i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        за (цео i = n - 1; i >= 0; i--)
        {
            за (цео j = n - 1; j > i; j--)
            {
                за (цео p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ако (std::abs(A[i*astep + i]) < eps)
                врати 0;
            за (цео p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    врати 1;
}

цео QR32f(float* A, size_t astep, цео m, цео n, цео k, float* b, size_t bstep, float* hFactors)
{
    CV_INSTRUMENT_REGION();

    цео output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    врати output;
}

цео QR64f(double* A, size_t astep, цео m, цео n, цео k, double* b, size_t bstep, double* hFactors)
{
    CV_INSTRUMENT_REGION();

    цео output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    врати output;
}

//=============================================================================
// for compatibility with 3.0

цео LU(float* A, size_t astep, цео m, float* b, size_t bstep, цео n)
{
    врати LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

цео LU(double* A, size_t astep, цео m, double* b, size_t bstep, цео n)
{
    врати LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

bool Cholesky(float* A, size_t astep, цео m, float* b, size_t bstep, цео n)
{
    врати CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky(double* A, size_t astep, цео m, double* b, size_t bstep, цео n)
{
    врати CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
