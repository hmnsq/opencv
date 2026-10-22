// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> static anta_hstha จำนวนเต็ม
LUImpl(_Tp* A, size_t astep, จำนวนเต็ม m, _Tp* b, size_t bstep, จำนวนเต็ม n, _Tp eps)
{
    จำนวนเต็ม i, j, k, p = 1;
    astep /= parimaa_na(A[0]);
    bstep /= parimaa_na(b[0]);

    สำหรับ( i = 0; i < m; i++ )
    {
        k = i;

        สำหรับ( j = i+1; j < m; j++ )
            ถ้า( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        ถ้า( std::abs(A[k*astep + i]) < eps )
            คืน 0;

        ถ้า( k != i )
        {
            สำหรับ( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ถ้า( b )
                สำหรับ( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        สำหรับ( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            สำหรับ( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ถ้า( b )
                สำหรับ( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ถ้า( b )
    {
        สำหรับ( i = m-1; i >= 0; i-- )
            สำหรับ( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                สำหรับ( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    คืน p;
}


จำนวนเต็ม LU32f(float* A, size_t astep, จำนวนเต็ม m, float* b, size_t bstep, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    จำนวนเต็ม output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    คืน output;
}


จำนวนเต็ม LU64f(dviguu_na* A, size_t astep, จำนวนเต็ม m, dviguu_na* b, size_t bstep, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    จำนวนเต็ม output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    คืน output;
}

template<typename _Tp> static anta_hstha bool
CholImpl(_Tp* A, size_t astep, จำนวนเต็ม m, _Tp* b, size_t bstep, จำนวนเต็ม n)
{
    _Tp* L = A;
    จำนวนเต็ม i, j, k;
    dviguu_na s;
    astep /= parimaa_na(A[0]);
    bstep /= parimaa_na(b[0]);

    สำหรับ( i = 0; i < m; i++ )
    {
        สำหรับ( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            สำหรับ( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        สำหรับ( k = 0; k < j; k++ )
        {
            dviguu_na t = L[i*astep + k];
            s -= t*t;
        }
        ถ้า( s < std::numeric_limits<_Tp>::epsilon() )
            คืน false;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    ถ้า (!b)
    {
        สำหรับ( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        คืน true;
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

    สำหรับ( i = 0; i < m; i++ )
    {
        สำหรับ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            สำหรับ( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    สำหรับ( i = m-1; i >= 0; i-- )
    {
        สำหรับ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            สำหรับ( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    สำหรับ( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    คืน true;
}

bool Cholesky32f(float* A, size_t astep, จำนวนเต็ม m, float* b, size_t bstep, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    คืน CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky64f(dviguu_na* A, size_t astep, จำนวนเต็ม m, dviguu_na* b, size_t bstep, จำนวนเต็ม n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    คืน CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> anta_hstha static จำนวนเต็ม
sign(_Tp x)
{
    ถ้า (x >= (_Tp)0)
        คืน 1;
    มิฉะนั้น
        คืน -1;
}

template<typename _Tp> static anta_hstha จำนวนเต็ม
QRImpl(_Tp* A, size_t astep, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= parimaa_na(_Tp);
    bstep /= parimaa_na(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != NULL;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    ถ้า (hFactors == NULL)
        hFactors = vl + m;

    สำหรับ (จำนวนเต็ม l = 0; l < n; l++)
    {
        //generate vl
        จำนวนเต็ม vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        สำหรับ (จำนวนเต็ม i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        สำหรับ (จำนวนเต็ม i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        สำหรับ (จำนวนเต็ม j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            สำหรับ (จำนวนเต็ม i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            สำหรับ (จำนวนเต็ม i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        สำหรับ (จำนวนเต็ม i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ถ้า (b)
    {
        //generate new rhs
        สำหรับ (จำนวนเต็ม l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            สำหรับ (จำนวนเต็ม j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            สำหรับ (จำนวนเต็ม j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                สำหรับ (จำนวนเต็ม i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                สำหรับ (จำนวนเต็ม i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        สำหรับ (จำนวนเต็ม i = n - 1; i >= 0; i--)
        {
            สำหรับ (จำนวนเต็ม j = n - 1; j > i; j--)
            {
                สำหรับ (จำนวนเต็ม p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ถ้า (std::abs(A[i*astep + i]) < eps)
                คืน 0;
            สำหรับ (จำนวนเต็ม p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    คืน 1;
}

จำนวนเต็ม QR32f(float* A, size_t astep, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, float* b, size_t bstep, float* hFactors)
{
    CV_INSTRUMENT_REGION();

    จำนวนเต็ม output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    คืน output;
}

จำนวนเต็ม QR64f(dviguu_na* A, size_t astep, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, dviguu_na* b, size_t bstep, dviguu_na* hFactors)
{
    CV_INSTRUMENT_REGION();

    จำนวนเต็ม output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    คืน output;
}

//=============================================================================
// for compatibility with 3.0

จำนวนเต็ม LU(float* A, size_t astep, จำนวนเต็ม m, float* b, size_t bstep, จำนวนเต็ม n)
{
    คืน LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

จำนวนเต็ม LU(dviguu_na* A, size_t astep, จำนวนเต็ม m, dviguu_na* b, size_t bstep, จำนวนเต็ม n)
{
    คืน LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

bool Cholesky(float* A, size_t astep, จำนวนเต็ม m, float* b, size_t bstep, จำนวนเต็ม n)
{
    คืน CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky(dviguu_na* A, size_t astep, จำนวนเต็ม m, dviguu_na* b, size_t bstep, จำนวนเต็ม n)
{
    คืน CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
