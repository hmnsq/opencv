// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
LUImpl(_Tp* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, _Tp* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n, _Tp eps)
{
    ಪೂರ್ಣಾಙ್ಕ i, j, k, p = 1;
    astep /= ಪರಿಮಾಣ(A[0]);
    bstep /= ಪರಿಮಾಣ(b[0]);

    ಪ್ರತಿ( i = 0; i < m; i++ )
    {
        k = i;

        ಪ್ರತಿ( j = i+1; j < m; j++ )
            ಯದಿ( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        ಯದಿ( std::abs(A[k*astep + i]) < eps )
            ಪ್ರತ್ಯಾಗಮ 0;

        ಯದಿ( k != i )
        {
            ಪ್ರತಿ( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ಯದಿ( b )
                ಪ್ರತಿ( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        ಪ್ರತಿ( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            ಪ್ರತಿ( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ಯದಿ( b )
                ಪ್ರತಿ( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ಯದಿ( b )
    {
        ಪ್ರತಿ( i = m-1; i >= 0; i-- )
            ಪ್ರತಿ( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                ಪ್ರತಿ( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    ಪ್ರತ್ಯಾಗಮ p;
}


ಪೂರ್ಣಾಙ್ಕ LU32f(ಭಿನ್ನ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    ಪ್ರತ್ಯಾಗಮ output;
}


ಪೂರ್ಣಾಙ್ಕ LU64f(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    ಪ್ರತ್ಯಾಗಮ output;
}

template<typename _Tp> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
CholImpl(_Tp* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, _Tp* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    _Tp* L = A;
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ದ್ವಿಗುಣ s;
    astep /= ಪರಿಮಾಣ(A[0]);
    bstep /= ಪರಿಮಾಣ(b[0]);

    ಪ್ರತಿ( i = 0; i < m; i++ )
    {
        ಪ್ರತಿ( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            ಪ್ರತಿ( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        ಪ್ರತಿ( k = 0; k < j; k++ )
        {
            ದ್ವಿಗುಣ t = L[i*astep + k];
            s -= t*t;
        }
        ಯದಿ( s < std::numeric_limits<_Tp>::epsilon() )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    ಯದಿ (!b)
    {
        ಪ್ರತಿ( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
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

    ಪ್ರತಿ( i = 0; i < m; i++ )
    {
        ಪ್ರತಿ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ಪ್ರತಿ( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    ಪ್ರತಿ( i = m-1; i >= 0; i-- )
    {
        ಪ್ರತಿ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ಪ್ರತಿ( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    ಪ್ರತಿ( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ Cholesky32f(ಭಿನ್ನ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    ಪ್ರತ್ಯಾಗಮ CholImpl(A, astep, m, b, bstep, n);
}

ತಾರ್ಕಿಕ Cholesky64f(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    ಪ್ರತ್ಯಾಗಮ CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
sign(_Tp x)
{
    ಯದಿ (x >= (_Tp)0)
        ಪ್ರತ್ಯಾಗಮ 1;
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ -1;
}

template<typename _Tp> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
QRImpl(_Tp* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= ಪರಿಮಾಣ(_Tp);
    bstep /= ಪರಿಮಾಣ(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != ರಿಕ್ತ;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    ಯದಿ (hFactors == ರಿಕ್ತ)
        hFactors = vl + m;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ l = 0; l < n; l++)
    {
        //generate vl
        ಪೂರ್ಣಾಙ್ಕ vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ಯದಿ (b)
    {
        //generate new rhs
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = n - 1; i >= 0; i--)
        {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = n - 1; j > i; j--)
            {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ಯದಿ (std::abs(A[i*astep + i]) < eps)
                ಪ್ರತ್ಯಾಗಮ 0;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ QR32f(ಭಿನ್ನ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಭಿನ್ನ* b, size_t bstep, ಭಿನ್ನ* hFactors)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    ಪ್ರತ್ಯಾಗಮ output;
}

ಪೂರ್ಣಾಙ್ಕ QR64f(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ದ್ವಿಗುಣ* b, size_t bstep, ದ್ವಿಗುಣ* hFactors)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    ಪ್ರತ್ಯಾಗಮ output;
}

//=============================================================================
// for compatibility with 3.0

ಪೂರ್ಣಾಙ್ಕ LU(ಭಿನ್ನ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪ್ರತ್ಯಾಗಮ LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

ಪೂರ್ಣಾಙ್ಕ LU(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪ್ರತ್ಯಾಗಮ LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

ತಾರ್ಕಿಕ Cholesky(ಭಿನ್ನ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪ್ರತ್ಯಾಗಮ CholImpl(A, astep, m, b, bstep, n);
}

ತಾರ್ಕಿಕ Cholesky(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪ್ರತ್ಯಾಗಮ CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
