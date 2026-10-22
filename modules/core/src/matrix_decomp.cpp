// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"

فضاینام cv { فضاینام hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> ایستا درونخطی صحیح
LUImpl(_Tp* A, size_t astep, صحیح m, _Tp* b, size_t bstep, صحیح n, _Tp eps)
{
    صحیح i, j, k, p = 1;
    astep /= اندازه(A[0]);
    bstep /= اندازه(b[0]);

    برای( i = 0; i < m; i++ )
    {
        k = i;

        برای( j = i+1; j < m; j++ )
            اگر( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        اگر( std::abs(A[k*astep + i]) < eps )
            بازگشت 0;

        اگر( k != i )
        {
            برای( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            اگر( b )
                برای( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        برای( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            برای( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            اگر( b )
                برای( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    اگر( b )
    {
        برای( i = m-1; i >= 0; i-- )
            برای( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                برای( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    بازگشت p;
}


صحیح LU32f(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    صحیح output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    بازگشت output;
}


صحیح LU64f(دوگانه* A, size_t astep, صحیح m, دوگانه* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    صحیح output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    بازگشت output;
}

template<typename _Tp> ایستا درونخطی منطقی
CholImpl(_Tp* A, size_t astep, صحیح m, _Tp* b, size_t bstep, صحیح n)
{
    _Tp* L = A;
    صحیح i, j, k;
    دوگانه s;
    astep /= اندازه(A[0]);
    bstep /= اندازه(b[0]);

    برای( i = 0; i < m; i++ )
    {
        برای( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            برای( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        برای( k = 0; k < j; k++ )
        {
            دوگانه t = L[i*astep + k];
            s -= t*t;
        }
        اگر( s < std::numeric_limits<_Tp>::epsilon() )
            بازگشت نادرست;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    اگر (!b)
    {
        برای( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        بازگشت درست;
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

    برای( i = 0; i < m; i++ )
    {
        برای( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            برای( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    برای( i = m-1; i >= 0; i-- )
    {
        برای( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            برای( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    برای( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    بازگشت درست;
}

منطقی Cholesky32f(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    منطقی output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    بازگشت CholImpl(A, astep, m, b, bstep, n);
}

منطقی Cholesky64f(دوگانه* A, size_t astep, صحیح m, دوگانه* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    منطقی output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    بازگشت CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> درونخطی ایستا صحیح
sign(_Tp x)
{
    اگر (x >= (_Tp)0)
        بازگشت 1;
    وگرنه
        بازگشت -1;
}

template<typename _Tp> ایستا درونخطی صحیح
QRImpl(_Tp* A, size_t astep, صحیح m, صحیح n, صحیح k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= اندازه(_Tp);
    bstep /= اندازه(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != پوچ;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    اگر (hFactors == پوچ)
        hFactors = vl + m;

    برای (صحیح l = 0; l < n; l++)
    {
        //generate vl
        صحیح vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        برای (صحیح i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        برای (صحیح i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        برای (صحیح j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            برای (صحیح i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            برای (صحیح i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        برای (صحیح i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    اگر (b)
    {
        //generate new rhs
        برای (صحیح l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            برای (صحیح j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            برای (صحیح j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                برای (صحیح i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                برای (صحیح i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        برای (صحیح i = n - 1; i >= 0; i--)
        {
            برای (صحیح j = n - 1; j > i; j--)
            {
                برای (صحیح p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            اگر (std::abs(A[i*astep + i]) < eps)
                بازگشت 0;
            برای (صحیح p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    بازگشت 1;
}

صحیح QR32f(اعشاری* A, size_t astep, صحیح m, صحیح n, صحیح k, اعشاری* b, size_t bstep, اعشاری* hFactors)
{
    CV_INSTRUMENT_REGION();

    صحیح output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    بازگشت output;
}

صحیح QR64f(دوگانه* A, size_t astep, صحیح m, صحیح n, صحیح k, دوگانه* b, size_t bstep, دوگانه* hFactors)
{
    CV_INSTRUMENT_REGION();

    صحیح output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    بازگشت output;
}

//=============================================================================
// for compatibility with 3.0

صحیح LU(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    بازگشت LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

صحیح LU(دوگانه* A, size_t astep, صحیح m, دوگانه* b, size_t bstep, صحیح n)
{
    بازگشت LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

منطقی Cholesky(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    بازگشت CholImpl(A, astep, m, b, bstep, n);
}

منطقی Cholesky(دوگانه* A, size_t astep, صحیح m, دوگانه* b, size_t bstep, صحیح n)
{
    بازگشت CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
