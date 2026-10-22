// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> static inline ціле
LUImpl(_Tp* A, size_t astep, ціле m, _Tp* b, size_t bstep, ціле n, _Tp eps)
{
    ціле i, j, k, p = 1;
    astep /= sizeof(A[0]);
    bstep /= sizeof(b[0]);

    для( i = 0; i < m; i++ )
    {
        k = i;

        для( j = i+1; j < m; j++ )
            якщо( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        якщо( std::abs(A[k*astep + i]) < eps )
            повернути 0;

        якщо( k != i )
        {
            для( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            якщо( b )
                для( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        для( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            для( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            якщо( b )
                для( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    якщо( b )
    {
        для( i = m-1; i >= 0; i-- )
            для( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                для( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    повернути p;
}


ціле LU32f(float* A, size_t astep, ціле m, float* b, size_t bstep, ціле n)
{
    CV_INSTRUMENT_REGION();

    ціле output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    повернути output;
}


ціле LU64f(double* A, size_t astep, ціле m, double* b, size_t bstep, ціле n)
{
    CV_INSTRUMENT_REGION();

    ціле output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    повернути output;
}

template<typename _Tp> static inline bool
CholImpl(_Tp* A, size_t astep, ціле m, _Tp* b, size_t bstep, ціле n)
{
    _Tp* L = A;
    ціле i, j, k;
    double s;
    astep /= sizeof(A[0]);
    bstep /= sizeof(b[0]);

    для( i = 0; i < m; i++ )
    {
        для( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            для( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        для( k = 0; k < j; k++ )
        {
            double t = L[i*astep + k];
            s -= t*t;
        }
        якщо( s < std::numeric_limits<_Tp>::epsilon() )
            повернути false;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    якщо (!b)
    {
        для( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        повернути true;
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

    для( i = 0; i < m; i++ )
    {
        для( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            для( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    для( i = m-1; i >= 0; i-- )
    {
        для( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            для( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    для( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    повернути true;
}

bool Cholesky32f(float* A, size_t astep, ціле m, float* b, size_t bstep, ціле n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    повернути CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky64f(double* A, size_t astep, ціле m, double* b, size_t bstep, ціле n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    повернути CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> inline static ціле
sign(_Tp x)
{
    якщо (x >= (_Tp)0)
        повернути 1;
    інакше
        повернути -1;
}

template<typename _Tp> static inline ціле
QRImpl(_Tp* A, size_t astep, ціле m, ціле n, ціле k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= sizeof(_Tp);
    bstep /= sizeof(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != NULL;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    якщо (hFactors == NULL)
        hFactors = vl + m;

    для (ціле l = 0; l < n; l++)
    {
        //generate vl
        ціле vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        для (ціле i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        для (ціле i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        для (ціле j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            для (ціле i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            для (ціле i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        для (ціле i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    якщо (b)
    {
        //generate new rhs
        для (ціле l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            для (ціле j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            для (ціле j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                для (ціле i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                для (ціле i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        для (ціле i = n - 1; i >= 0; i--)
        {
            для (ціле j = n - 1; j > i; j--)
            {
                для (ціле p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            якщо (std::abs(A[i*astep + i]) < eps)
                повернути 0;
            для (ціле p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    повернути 1;
}

ціле QR32f(float* A, size_t astep, ціле m, ціле n, ціле k, float* b, size_t bstep, float* hFactors)
{
    CV_INSTRUMENT_REGION();

    ціле output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    повернути output;
}

ціле QR64f(double* A, size_t astep, ціле m, ціле n, ціле k, double* b, size_t bstep, double* hFactors)
{
    CV_INSTRUMENT_REGION();

    ціле output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    повернути output;
}

//=============================================================================
// for compatibility with 3.0

ціле LU(float* A, size_t astep, ціле m, float* b, size_t bstep, ціле n)
{
    повернути LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

ціле LU(double* A, size_t astep, ціле m, double* b, size_t bstep, ціле n)
{
    повернути LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

bool Cholesky(float* A, size_t astep, ціле m, float* b, size_t bstep, ціле n)
{
    повернути CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky(double* A, size_t astep, ціле m, double* b, size_t bstep, ціле n)
{
    повернути CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
