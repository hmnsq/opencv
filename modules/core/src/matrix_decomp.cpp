// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
LUImpl(_Tp* A, size_t astep, ਪੂਰਨ m, _Tp* b, size_t bstep, ਪੂਰਨ n, _Tp eps)
{
    ਪੂਰਨ i, j, k, p = 1;
    astep /= ਆਕਾਰ(A[0]);
    bstep /= ਆਕਾਰ(b[0]);

    ਲਈ( i = 0; i < m; i++ )
    {
        k = i;

        ਲਈ( j = i+1; j < m; j++ )
            ਜੇ( std::ਨਿਰਪੇਖ(A[j*astep + i]) > std::ਨਿਰਪੇਖ(A[k*astep + i]) )
                k = j;

        ਜੇ( std::ਨਿਰਪੇਖ(A[k*astep + i]) < eps )
            ਵਾਪਸ 0;

        ਜੇ( k != i )
        {
            ਲਈ( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ਜੇ( b )
                ਲਈ( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        ਲਈ( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            ਲਈ( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ਜੇ( b )
                ਲਈ( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ਜੇ( b )
    {
        ਲਈ( i = m-1; i >= 0; i-- )
            ਲਈ( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                ਲਈ( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    ਵਾਪਸ p;
}


ਪੂਰਨ LU32f(ਦਸ਼ਮਲਵ* A, size_t astep, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t bstep, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    ਵਾਪਸ output;
}


ਪੂਰਨ LU64f(ਦੁੱਗਣਾ* A, size_t astep, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t bstep, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    ਵਾਪਸ output;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
CholImpl(_Tp* A, size_t astep, ਪੂਰਨ m, _Tp* b, size_t bstep, ਪੂਰਨ n)
{
    _Tp* L = A;
    ਪੂਰਨ i, j, k;
    ਦੁੱਗਣਾ s;
    astep /= ਆਕਾਰ(A[0]);
    bstep /= ਆਕਾਰ(b[0]);

    ਲਈ( i = 0; i < m; i++ )
    {
        ਲਈ( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            ਲਈ( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        ਲਈ( k = 0; k < j; k++ )
        {
            ਦੁੱਗਣਾ t = L[i*astep + k];
            s -= t*t;
        }
        ਜੇ( s < std::numeric_limits<_Tp>::epsilon() )
            ਵਾਪਸ ਝੂਠ;
        L[i*astep + i] = (_Tp)(1./std::ਵਰਗਮੂਲ(s));
    }

    ਜੇ (!b)
    {
        ਲਈ( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        ਵਾਪਸ ਸੱਚ;
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

    ਲਈ( i = 0; i < m; i++ )
    {
        ਲਈ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ਲਈ( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    ਲਈ( i = m-1; i >= 0; i-- )
    {
        ਲਈ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ਲਈ( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    ਲਈ( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ Cholesky32f(ਦਸ਼ਮਲਵ* A, size_t astep, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t bstep, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਤਾਰਕਿਕ output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    ਵਾਪਸ CholImpl(A, astep, m, b, bstep, n);
}

ਤਾਰਕਿਕ Cholesky64f(ਦੁੱਗਣਾ* A, size_t astep, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t bstep, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਤਾਰਕਿਕ output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    ਵਾਪਸ CholImpl(A, astep, m, b, bstep, n);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਸਤਰਅੰਦਰ ਸਥਿਰ ਪੂਰਨ
sign(_Tp x)
{
    ਜੇ (x >= (_Tp)0)
        ਵਾਪਸ 1;
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ -1;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
QRImpl(_Tp* A, size_t astep, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= ਆਕਾਰ(_Tp);
    bstep /= ਆਕਾਰ(_Tp);

    cv::Auto_ਬਫਰ<_Tp> ਬਫਰ;
    size_t buf_ਮਾਪ_2 = m ? m + n : hFactors != ਨਿਰਾ;
    ਬਫਰ.allocate(buf_ਮਾਪ_2);
    _Tp* vl = ਬਫਰ.ਅੰਕੜੇ();
    ਜੇ (hFactors == ਨਿਰਾ)
        hFactors = vl + m;

    ਲਈ (ਪੂਰਨ l = 0; l < n; l++)
    {
        //generate vl
        ਪੂਰਨ vl_ਮਾਪ = m - l;
        _Tp vlNorm = (_Tp)0;
        ਲਈ (ਪੂਰਨ i = 0; i < vl_ਮਾਪ; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::ਵਰਗਮੂਲ(vlNorm);
        vlNorm = std::ਵਰਗਮੂਲ(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        ਲਈ (ਪੂਰਨ i = 0; i < vl_ਮਾਪ; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        ਲਈ (ਪੂਰਨ j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            ਲਈ (ਪੂਰਨ i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            ਲਈ (ਪੂਰਨ i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        ਲਈ (ਪੂਰਨ i = 1; i < vl_ਮਾਪ; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ਜੇ (b)
    {
        //generate new rhs
        ਲਈ (ਪੂਰਨ l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            ਲਈ (ਪੂਰਨ j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            ਲਈ (ਪੂਰਨ j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                ਲਈ (ਪੂਰਨ i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                ਲਈ (ਪੂਰਨ i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        ਲਈ (ਪੂਰਨ i = n - 1; i >= 0; i--)
        {
            ਲਈ (ਪੂਰਨ j = n - 1; j > i; j--)
            {
                ਲਈ (ਪੂਰਨ p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ਜੇ (std::ਨਿਰਪੇਖ(A[i*astep + i]) < eps)
                ਵਾਪਸ 0;
            ਲਈ (ਪੂਰਨ p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    ਵਾਪਸ 1;
}

ਪੂਰਨ QR32f(ਦਸ਼ਮਲਵ* A, size_t astep, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਦਸ਼ਮਲਵ* b, size_t bstep, ਦਸ਼ਮਲਵ* hFactors)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    ਵਾਪਸ output;
}

ਪੂਰਨ QR64f(ਦੁੱਗਣਾ* A, size_t astep, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਦੁੱਗਣਾ* b, size_t bstep, ਦੁੱਗਣਾ* hFactors)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    ਵਾਪਸ output;
}

//=============================================================================
// for compatibility with 3.0

ਪੂਰਨ LU(ਦਸ਼ਮਲਵ* A, size_t astep, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t bstep, ਪੂਰਨ n)
{
    ਵਾਪਸ LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

ਪੂਰਨ LU(ਦੁੱਗਣਾ* A, size_t astep, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t bstep, ਪੂਰਨ n)
{
    ਵਾਪਸ LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

ਤਾਰਕਿਕ Cholesky(ਦਸ਼ਮਲਵ* A, size_t astep, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t bstep, ਪੂਰਨ n)
{
    ਵਾਪਸ CholImpl(A, astep, m, b, bstep, n);
}

ਤਾਰਕਿਕ Cholesky(ਦੁੱਗਣਾ* A, size_t astep, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t bstep, ਪੂਰਨ n)
{
    ਵਾਪਸ CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
