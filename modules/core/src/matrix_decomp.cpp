// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> static anta_hstha ຈຳນວນເຕັມ
LUImpl(_Tp* A, size_t astep, ຈຳນວນເຕັມ m, _Tp* b, size_t bstep, ຈຳນວນເຕັມ n, _Tp eps)
{
    ຈຳນວນເຕັມ i, j, k, p = 1;
    astep /= parimaa_na(A[0]);
    bstep /= parimaa_na(b[0]);

    ສຳລັບ( i = 0; i < m; i++ )
    {
        k = i;

        ສຳລັບ( j = i+1; j < m; j++ )
            ຖ້າ( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        ຖ້າ( std::abs(A[k*astep + i]) < eps )
            ສົ່ງຄືນ 0;

        ຖ້າ( k != i )
        {
            ສຳລັບ( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ຖ້າ( b )
                ສຳລັບ( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        ສຳລັບ( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            ສຳລັບ( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ຖ້າ( b )
                ສຳລັບ( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ຖ້າ( b )
    {
        ສຳລັບ( i = m-1; i >= 0; i-- )
            ສຳລັບ( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                ສຳລັບ( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    ສົ່ງຄືນ p;
}


ຈຳນວນເຕັມ LU32f(float* A, size_t astep, ຈຳນວນເຕັມ m, float* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    ສົ່ງຄືນ output;
}


ຈຳນວນເຕັມ LU64f(dviguu_na* A, size_t astep, ຈຳນວນເຕັມ m, dviguu_na* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    ສົ່ງຄືນ output;
}

template<typename _Tp> static anta_hstha bool
CholImpl(_Tp* A, size_t astep, ຈຳນວນເຕັມ m, _Tp* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    _Tp* L = A;
    ຈຳນວນເຕັມ i, j, k;
    dviguu_na s;
    astep /= parimaa_na(A[0]);
    bstep /= parimaa_na(b[0]);

    ສຳລັບ( i = 0; i < m; i++ )
    {
        ສຳລັບ( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            ສຳລັບ( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        ສຳລັບ( k = 0; k < j; k++ )
        {
            dviguu_na t = L[i*astep + k];
            s -= t*t;
        }
        ຖ້າ( s < std::numeric_limits<_Tp>::epsilon() )
            ສົ່ງຄືນ false;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    ຖ້າ (!b)
    {
        ສຳລັບ( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        ສົ່ງຄືນ true;
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

    ສຳລັບ( i = 0; i < m; i++ )
    {
        ສຳລັບ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ສຳລັບ( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    ສຳລັບ( i = m-1; i >= 0; i-- )
    {
        ສຳລັບ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ສຳລັບ( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    ສຳລັບ( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    ສົ່ງຄືນ true;
}

bool Cholesky32f(float* A, size_t astep, ຈຳນວນເຕັມ m, float* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    ສົ່ງຄືນ CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky64f(dviguu_na* A, size_t astep, ຈຳນວນເຕັມ m, dviguu_na* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    ສົ່ງຄືນ CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> anta_hstha static ຈຳນວນເຕັມ
sign(_Tp x)
{
    ຖ້າ (x >= (_Tp)0)
        ສົ່ງຄືນ 1;
    ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ -1;
}

template<typename _Tp> static anta_hstha ຈຳນວນເຕັມ
QRImpl(_Tp* A, size_t astep, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= parimaa_na(_Tp);
    bstep /= parimaa_na(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != NULL;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    ຖ້າ (hFactors == NULL)
        hFactors = vl + m;

    ສຳລັບ (ຈຳນວນເຕັມ l = 0; l < n; l++)
    {
        //generate vl
        ຈຳນວນເຕັມ vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        ສຳລັບ (ຈຳນວນເຕັມ j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            ສຳລັບ (ຈຳນວນເຕັມ i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            ສຳລັບ (ຈຳນວນເຕັມ i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        ສຳລັບ (ຈຳນວນເຕັມ i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ຖ້າ (b)
    {
        //generate new rhs
        ສຳລັບ (ຈຳນວນເຕັມ l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            ສຳລັບ (ຈຳນວນເຕັມ j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            ສຳລັບ (ຈຳນວນເຕັມ j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                ສຳລັບ (ຈຳນວນເຕັມ i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                ສຳລັບ (ຈຳນວນເຕັມ i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        ສຳລັບ (ຈຳນວນເຕັມ i = n - 1; i >= 0; i--)
        {
            ສຳລັບ (ຈຳນວນເຕັມ j = n - 1; j > i; j--)
            {
                ສຳລັບ (ຈຳນວນເຕັມ p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ຖ້າ (std::abs(A[i*astep + i]) < eps)
                ສົ່ງຄືນ 0;
            ສຳລັບ (ຈຳນວນເຕັມ p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    ສົ່ງຄືນ 1;
}

ຈຳນວນເຕັມ QR32f(float* A, size_t astep, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ k, float* b, size_t bstep, float* hFactors)
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    ສົ່ງຄືນ output;
}

ຈຳນວນເຕັມ QR64f(dviguu_na* A, size_t astep, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ k, dviguu_na* b, size_t bstep, dviguu_na* hFactors)
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    ສົ່ງຄືນ output;
}

//=============================================================================
// for compatibility with 3.0

ຈຳນວນເຕັມ LU(float* A, size_t astep, ຈຳນວນເຕັມ m, float* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    ສົ່ງຄືນ LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

ຈຳນວນເຕັມ LU(dviguu_na* A, size_t astep, ຈຳນວນເຕັມ m, dviguu_na* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    ສົ່ງຄືນ LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

bool Cholesky(float* A, size_t astep, ຈຳນວນເຕັມ m, float* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    ສົ່ງຄືນ CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky(dviguu_na* A, size_t astep, ຈຳນວນເຕັມ m, dviguu_na* b, size_t bstep, ຈຳນວນເຕັມ n)
{
    ສົ່ງຄືນ CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
