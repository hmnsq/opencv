// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"

namespace cv { namespace hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> static anta_hstha puur_naa_nka
LUImpl(_Tp* A, size_t astep, puur_naa_nka m, _Tp* b, size_t bstep, puur_naa_nka n, _Tp eps)
{
    puur_naa_nka i, j, k, p = 1;
    astep /= parimaa_na(A[0]);
    bstep /= parimaa_na(b[0]);

    for( i = 0; i < m; i++ )
    {
        k = i;

        for( j = i+1; j < m; j++ )
            ᮚᮦᮔ᮪( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        ᮚᮦᮔ᮪( std::abs(A[k*astep + i]) < eps )
            ᮝᮜᮤ 0;

        ᮚᮦᮔ᮪( k != i )
        {
            for( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ᮚᮦᮔ᮪( b )
                for( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        for( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            for( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ᮚᮦᮔ᮪( b )
                for( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ᮚᮦᮔ᮪( b )
    {
        for( i = m-1; i >= 0; i-- )
            for( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                for( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    ᮝᮜᮤ p;
}


puur_naa_nka LU32f(float* A, size_t astep, puur_naa_nka m, float* b, size_t bstep, puur_naa_nka n)
{
    CV_INSTRUMENT_REGION();

    puur_naa_nka output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    ᮝᮜᮤ output;
}


puur_naa_nka LU64f(dviguu_na* A, size_t astep, puur_naa_nka m, dviguu_na* b, size_t bstep, puur_naa_nka n)
{
    CV_INSTRUMENT_REGION();

    puur_naa_nka output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    ᮝᮜᮤ output;
}

template<typename _Tp> static anta_hstha bool
CholImpl(_Tp* A, size_t astep, puur_naa_nka m, _Tp* b, size_t bstep, puur_naa_nka n)
{
    _Tp* L = A;
    puur_naa_nka i, j, k;
    dviguu_na s;
    astep /= parimaa_na(A[0]);
    bstep /= parimaa_na(b[0]);

    for( i = 0; i < m; i++ )
    {
        for( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            for( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        for( k = 0; k < j; k++ )
        {
            dviguu_na t = L[i*astep + k];
            s -= t*t;
        }
        ᮚᮦᮔ᮪( s < std::numeric_limits<_Tp>::epsilon() )
            ᮝᮜᮤ false;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    ᮚᮦᮔ᮪ (!b)
    {
        for( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        ᮝᮜᮤ true;
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

    for( i = 0; i < m; i++ )
    {
        for( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            for( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    for( i = m-1; i >= 0; i-- )
    {
        for( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            for( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    for( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    ᮝᮜᮤ true;
}

bool Cholesky32f(float* A, size_t astep, puur_naa_nka m, float* b, size_t bstep, puur_naa_nka n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    ᮝᮜᮤ CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky64f(dviguu_na* A, size_t astep, puur_naa_nka m, dviguu_na* b, size_t bstep, puur_naa_nka n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    ᮝᮜᮤ CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> anta_hstha static puur_naa_nka
sign(_Tp x)
{
    ᮚᮦᮔ᮪ (x >= (_Tp)0)
        ᮝᮜᮤ 1;
    ᮜᮤᮚ
        ᮝᮜᮤ -1;
}

template<typename _Tp> static anta_hstha puur_naa_nka
QRImpl(_Tp* A, size_t astep, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= parimaa_na(_Tp);
    bstep /= parimaa_na(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != NULL;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    ᮚᮦᮔ᮪ (hFactors == NULL)
        hFactors = vl + m;

    for (puur_naa_nka l = 0; l < n; l++)
    {
        //generate vl
        puur_naa_nka vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        for (puur_naa_nka i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        for (puur_naa_nka i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        for (puur_naa_nka j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            for (puur_naa_nka i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            for (puur_naa_nka i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        for (puur_naa_nka i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ᮚᮦᮔ᮪ (b)
    {
        //generate new rhs
        for (puur_naa_nka l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            for (puur_naa_nka j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            for (puur_naa_nka j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                for (puur_naa_nka i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                for (puur_naa_nka i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        for (puur_naa_nka i = n - 1; i >= 0; i--)
        {
            for (puur_naa_nka j = n - 1; j > i; j--)
            {
                for (puur_naa_nka p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ᮚᮦᮔ᮪ (std::abs(A[i*astep + i]) < eps)
                ᮝᮜᮤ 0;
            for (puur_naa_nka p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    ᮝᮜᮤ 1;
}

puur_naa_nka QR32f(float* A, size_t astep, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, float* b, size_t bstep, float* hFactors)
{
    CV_INSTRUMENT_REGION();

    puur_naa_nka output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    ᮝᮜᮤ output;
}

puur_naa_nka QR64f(dviguu_na* A, size_t astep, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, dviguu_na* b, size_t bstep, dviguu_na* hFactors)
{
    CV_INSTRUMENT_REGION();

    puur_naa_nka output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    ᮝᮜᮤ output;
}

//=============================================================================
// for compatibility with 3.0

puur_naa_nka LU(float* A, size_t astep, puur_naa_nka m, float* b, size_t bstep, puur_naa_nka n)
{
    ᮝᮜᮤ LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

puur_naa_nka LU(dviguu_na* A, size_t astep, puur_naa_nka m, dviguu_na* b, size_t bstep, puur_naa_nka n)
{
    ᮝᮜᮤ LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

bool Cholesky(float* A, size_t astep, puur_naa_nka m, float* b, size_t bstep, puur_naa_nka n)
{
    ᮝᮜᮤ CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky(dviguu_na* A, size_t astep, puur_naa_nka m, dviguu_na* b, size_t bstep, puur_naa_nka n)
{
    ᮝᮜᮤ CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
