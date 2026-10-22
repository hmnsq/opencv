// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
LUImpl(_Tp* A, size_t astep, ಪೂರ್ಣಾಂಕ m, _Tp* b, size_t bstep, ಪೂರ್ಣಾಂಕ n, _Tp eps)
{
    ಪೂರ್ಣಾಂಕ i, j, k, p = 1;
    astep /= ಗಾತ್ರ(A[0]);
    bstep /= ಗಾತ್ರ(b[0]);

    ಗಾಗಿ( i = 0; i < m; i++ )
    {
        k = i;

        ಗಾಗಿ( j = i+1; j < m; j++ )
            ಆದರೆ( std::ನಿರಪೇಕ್ಷ(A[j*astep + i]) > std::ನಿರಪೇಕ್ಷ(A[k*astep + i]) )
                k = j;

        ಆದರೆ( std::ನಿರಪೇಕ್ಷ(A[k*astep + i]) < eps )
            ಹಿಂತಿರುಗಿ 0;

        ಆದರೆ( k != i )
        {
            ಗಾಗಿ( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ಆದರೆ( b )
                ಗಾಗಿ( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        ಗಾಗಿ( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            ಗಾಗಿ( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ಆದರೆ( b )
                ಗಾಗಿ( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ಆದರೆ( b )
    {
        ಗಾಗಿ( i = m-1; i >= 0; i-- )
            ಗಾಗಿ( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                ಗಾಗಿ( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    ಹಿಂತಿರುಗಿ p;
}


ಪೂರ್ಣಾಂಕ LU32f(ದಶಮಾಂಶ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    ಹಿಂತಿರುಗಿ output;
}


ಪೂರ್ಣಾಂಕ LU64f(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    ಹಿಂತಿರುಗಿ output;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ
CholImpl(_Tp* A, size_t astep, ಪೂರ್ಣಾಂಕ m, _Tp* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    _Tp* L = A;
    ಪೂರ್ಣಾಂಕ i, j, k;
    ದ್ವಿಗುಣ s;
    astep /= ಗಾತ್ರ(A[0]);
    bstep /= ಗಾತ್ರ(b[0]);

    ಗಾಗಿ( i = 0; i < m; i++ )
    {
        ಗಾಗಿ( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            ಗಾಗಿ( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        ಗಾಗಿ( k = 0; k < j; k++ )
        {
            ದ್ವಿಗುಣ t = L[i*astep + k];
            s -= t*t;
        }
        ಆದರೆ( s < std::numeric_limits<_Tp>::epsilon() )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        L[i*astep + i] = (_Tp)(1./std::ವರ್ಗಮೂಲ(s));
    }

    ಆದರೆ (!b)
    {
        ಗಾಗಿ( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        ಹಿಂತಿರುಗಿ ನಿಜ;
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

    ಗಾಗಿ( i = 0; i < m; i++ )
    {
        ಗಾಗಿ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ಗಾಗಿ( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    ಗಾಗಿ( i = m-1; i >= 0; i-- )
    {
        ಗಾಗಿ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ಗಾಗಿ( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    ಗಾಗಿ( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ತಾರ್ಕಿಕ Cholesky32f(ದಶಮಾಂಶ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    ಹಿಂತಿರುಗಿ CholImpl(A, astep, m, b, bstep, n);
}

ತಾರ್ಕಿಕ Cholesky64f(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    ಹಿಂತಿರುಗಿ CholImpl(A, astep, m, b, bstep, n);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ಸಾಲೊಳಗೆ ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sign(_Tp x)
{
    ಆದರೆ (x >= (_Tp)0)
        ಹಿಂತಿರುಗಿ 1;
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ -1;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
QRImpl(_Tp* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= ಗಾತ್ರ(_Tp);
    bstep /= ಗಾತ್ರ(_Tp);

    cv::Auto_ಬಫರ್<_Tp> ಬಫರ್;
    size_t buf_ಗಾತ್ರ_2 = m ? m + n : hFactors != ಖಾಲಿ;
    ಬಫರ್.allocate(buf_ಗಾತ್ರ_2);
    _Tp* vl = ಬಫರ್.ದತ್ತಾಂಶ();
    ಆದರೆ (hFactors == ಖಾಲಿ)
        hFactors = vl + m;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ l = 0; l < n; l++)
    {
        //generate vl
        ಪೂರ್ಣಾಂಕ vl_ಗಾತ್ರ = m - l;
        _Tp vlNorm = (_Tp)0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < vl_ಗಾತ್ರ; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::ವರ್ಗಮೂಲ(vlNorm);
        vlNorm = std::ವರ್ಗಮೂಲ(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < vl_ಗಾತ್ರ; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < vl_ಗಾತ್ರ; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ಆದರೆ (b)
    {
        //generate new rhs
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = n - 1; i >= 0; i--)
        {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = n - 1; j > i; j--)
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ಆದರೆ (std::ನಿರಪೇಕ್ಷ(A[i*astep + i]) < eps)
                ಹಿಂತಿರುಗಿ 0;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ QR32f(ದಶಮಾಂಶ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ದಶಮಾಂಶ* b, size_t bstep, ದಶಮಾಂಶ* hFactors)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    ಹಿಂತಿರುಗಿ output;
}

ಪೂರ್ಣಾಂಕ QR64f(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ದ್ವಿಗುಣ* b, size_t bstep, ದ್ವಿಗುಣ* hFactors)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    ಹಿಂತಿರುಗಿ output;
}

//=============================================================================
// for compatibility with 3.0

ಪೂರ್ಣಾಂಕ LU(ದಶಮಾಂಶ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    ಹಿಂತಿರುಗಿ LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

ಪೂರ್ಣಾಂಕ LU(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    ಹಿಂತಿರುಗಿ LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

ತಾರ್ಕಿಕ Cholesky(ದಶಮಾಂಶ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    ಹಿಂತಿರುಗಿ CholImpl(A, astep, m, b, bstep, n);
}

ತಾರ್ಕಿಕ Cholesky(ದ್ವಿಗುಣ* A, size_t astep, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t bstep, ಪೂರ್ಣಾಂಕ n)
{
    ಹಿಂತಿರುಗಿ CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
