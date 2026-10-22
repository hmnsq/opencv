// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"

نامخلا cv { نامخلا hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

سانچہ<قسم_نام _Tp> ساکن اندرونی صحیح
LUImpl(_Tp* A, size_t astep, صحیح m, _Tp* b, size_t bstep, صحیح n, _Tp eps)
{
    صحیح i, j, k, p = 1;
    astep /= حجم(A[0]);
    bstep /= حجم(b[0]);

    برائے( i = 0; i < m; i++ )
    {
        k = i;

        برائے( j = i+1; j < m; j++ )
            اگر( std::مطلق_قدر(A[j*astep + i]) > std::مطلق_قدر(A[k*astep + i]) )
                k = j;

        اگر( std::مطلق_قدر(A[k*astep + i]) < eps )
            واپس 0;

        اگر( k != i )
        {
            برائے( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            اگر( b )
                برائے( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        برائے( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            برائے( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            اگر( b )
                برائے( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    اگر( b )
    {
        برائے( i = m-1; i >= 0; i-- )
            برائے( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                برائے( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    واپس p;
}


صحیح LU32f(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    صحیح output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    واپس output;
}


صحیح LU64f(دوہرا* A, size_t astep, صحیح m, دوہرا* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    صحیح output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    واپس output;
}

سانچہ<قسم_نام _Tp> ساکن اندرونی منطقی
CholImpl(_Tp* A, size_t astep, صحیح m, _Tp* b, size_t bstep, صحیح n)
{
    _Tp* L = A;
    صحیح i, j, k;
    دوہرا s;
    astep /= حجم(A[0]);
    bstep /= حجم(b[0]);

    برائے( i = 0; i < m; i++ )
    {
        برائے( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            برائے( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        برائے( k = 0; k < j; k++ )
        {
            دوہرا t = L[i*astep + k];
            s -= t*t;
        }
        اگر( s < std::numeric_limits<_Tp>::epsilon() )
            واپس جھوٹ;
        L[i*astep + i] = (_Tp)(1./std::جذر(s));
    }

    اگر (!b)
    {
        برائے( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        واپس سچ;
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

    برائے( i = 0; i < m; i++ )
    {
        برائے( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            برائے( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    برائے( i = m-1; i >= 0; i-- )
    {
        برائے( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            برائے( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    برائے( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    واپس سچ;
}

منطقی Cholesky32f(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    منطقی output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    واپس CholImpl(A, astep, m, b, bstep, n);
}

منطقی Cholesky64f(دوہرا* A, size_t astep, صحیح m, دوہرا* b, size_t bstep, صحیح n)
{
    CV_INSTRUMENT_REGION();

    منطقی output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    واپس CholImpl(A, astep, m, b, bstep, n);
}

سانچہ<قسم_نام _Tp> اندرونی ساکن صحیح
sign(_Tp x)
{
    اگر (x >= (_Tp)0)
        واپس 1;
    ورنہ
        واپس -1;
}

سانچہ<قسم_نام _Tp> ساکن اندرونی صحیح
QRImpl(_Tp* A, size_t astep, صحیح m, صحیح n, صحیح k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= حجم(_Tp);
    bstep /= حجم(_Tp);

    cv::Auto_بفر<_Tp> بفر;
    size_t buf_سائز_2 = m ? m + n : hFactors != تہی;
    بفر.allocate(buf_سائز_2);
    _Tp* vl = بفر.ڈیٹا();
    اگر (hFactors == تہی)
        hFactors = vl + m;

    برائے (صحیح l = 0; l < n; l++)
    {
        //generate vl
        صحیح vl_سائز = m - l;
        _Tp vlNorm = (_Tp)0;
        برائے (صحیح i = 0; i < vl_سائز; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::جذر(vlNorm);
        vlNorm = std::جذر(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        برائے (صحیح i = 0; i < vl_سائز; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        برائے (صحیح j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            برائے (صحیح i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            برائے (صحیح i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        برائے (صحیح i = 1; i < vl_سائز; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    اگر (b)
    {
        //generate new rhs
        برائے (صحیح l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            برائے (صحیح j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            برائے (صحیح j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                برائے (صحیح i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                برائے (صحیح i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        برائے (صحیح i = n - 1; i >= 0; i--)
        {
            برائے (صحیح j = n - 1; j > i; j--)
            {
                برائے (صحیح p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            اگر (std::مطلق_قدر(A[i*astep + i]) < eps)
                واپس 0;
            برائے (صحیح p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    واپس 1;
}

صحیح QR32f(اعشاری* A, size_t astep, صحیح m, صحیح n, صحیح k, اعشاری* b, size_t bstep, اعشاری* hFactors)
{
    CV_INSTRUMENT_REGION();

    صحیح output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    واپس output;
}

صحیح QR64f(دوہرا* A, size_t astep, صحیح m, صحیح n, صحیح k, دوہرا* b, size_t bstep, دوہرا* hFactors)
{
    CV_INSTRUMENT_REGION();

    صحیح output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    واپس output;
}

//=============================================================================
// for compatibility with 3.0

صحیح LU(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    واپس LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

صحیح LU(دوہرا* A, size_t astep, صحیح m, دوہرا* b, size_t bstep, صحیح n)
{
    واپس LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

منطقی Cholesky(اعشاری* A, size_t astep, صحیح m, اعشاری* b, size_t bstep, صحیح n)
{
    واپس CholImpl(A, astep, m, b, bstep, n);
}

منطقی Cholesky(دوہرا* A, size_t astep, صحیح m, دوہرا* b, size_t bstep, صحیح n)
{
    واپس CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
