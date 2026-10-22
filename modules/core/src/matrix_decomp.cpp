// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"

نطاق cv { نطاق hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

کينډۍ<ډول_نوم _Tp> ساكن مضمن صحيح
LUImpl(_Tp* A, size_t astep, صحيح m, _Tp* b, size_t bstep, صحيح n, _Tp eps)
{
    صحيح i, j, k, p = 1;
    astep /= حجم(A[0]);
    bstep /= حجم(b[0]);

    لكل( i = 0; i < m; i++ )
    {
        k = i;

        لكل( j = i+1; j < m; j++ )
            إذا( std::مطلق_قیمت(A[j*astep + i]) > std::مطلق_قیمت(A[k*astep + i]) )
                k = j;

        إذا( std::مطلق_قیمت(A[k*astep + i]) < eps )
            أرجع 0;

        إذا( k != i )
        {
            لكل( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            إذا( b )
                لكل( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        لكل( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            لكل( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            إذا( b )
                لكل( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    إذا( b )
    {
        لكل( i = m-1; i >= 0; i-- )
            لكل( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                لكل( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    أرجع p;
}


صحيح LU32f(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    صحيح output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    أرجع output;
}


صحيح LU64f(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    صحيح output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    أرجع output;
}

کينډۍ<ډول_نوم _Tp> ساكن مضمن منطقي
CholImpl(_Tp* A, size_t astep, صحيح m, _Tp* b, size_t bstep, صحيح n)
{
    _Tp* L = A;
    صحيح i, j, k;
    مزدوج s;
    astep /= حجم(A[0]);
    bstep /= حجم(b[0]);

    لكل( i = 0; i < m; i++ )
    {
        لكل( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            لكل( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        لكل( k = 0; k < j; k++ )
        {
            مزدوج t = L[i*astep + k];
            s -= t*t;
        }
        إذا( s < std::numeric_limits<_Tp>::epsilon() )
            أرجع خطأ;
        L[i*astep + i] = (_Tp)(1./std::جذر(s));
    }

    إذا (!b)
    {
        لكل( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        أرجع ریښتیا;
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

    لكل( i = 0; i < m; i++ )
    {
        لكل( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            لكل( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    لكل( i = m-1; i >= 0; i-- )
    {
        لكل( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            لكل( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    لكل( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    أرجع ریښتیا;
}

منطقي Cholesky32f(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    منطقي output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    أرجع CholImpl(A, astep, m, b, bstep, n);
}

منطقي Cholesky64f(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    منطقي output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    أرجع CholImpl(A, astep, m, b, bstep, n);
}

کينډۍ<ډول_نوم _Tp> مضمن ساكن صحيح
sign(_Tp x)
{
    إذا (x >= (_Tp)0)
        أرجع 1;
    وإلا
        أرجع -1;
}

کينډۍ<ډول_نوم _Tp> ساكن مضمن صحيح
QRImpl(_Tp* A, size_t astep, صحيح m, صحيح n, صحيح k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= حجم(_Tp);
    bstep /= حجم(_Tp);

    cv::Auto_بفر<_Tp> بفر;
    size_t buf_اندازه_2 = m ? m + n : hFactors != عدم;
    بفر.allocate(buf_اندازه_2);
    _Tp* vl = بفر.معلومات();
    إذا (hFactors == عدم)
        hFactors = vl + m;

    لكل (صحيح l = 0; l < n; l++)
    {
        //generate vl
        صحيح vl_اندازه = m - l;
        _Tp vlNorm = (_Tp)0;
        لكل (صحيح i = 0; i < vl_اندازه; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::جذر(vlNorm);
        vlNorm = std::جذر(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        لكل (صحيح i = 0; i < vl_اندازه; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        لكل (صحيح j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            لكل (صحيح i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            لكل (صحيح i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        لكل (صحيح i = 1; i < vl_اندازه; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    إذا (b)
    {
        //generate new rhs
        لكل (صحيح l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            لكل (صحيح j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            لكل (صحيح j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                لكل (صحيح i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                لكل (صحيح i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        لكل (صحيح i = n - 1; i >= 0; i--)
        {
            لكل (صحيح j = n - 1; j > i; j--)
            {
                لكل (صحيح p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            إذا (std::مطلق_قیمت(A[i*astep + i]) < eps)
                أرجع 0;
            لكل (صحيح p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    أرجع 1;
}

صحيح QR32f(عائم* A, size_t astep, صحيح m, صحيح n, صحيح k, عائم* b, size_t bstep, عائم* hFactors)
{
    CV_INSTRUMENT_REGION();

    صحيح output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    أرجع output;
}

صحيح QR64f(مزدوج* A, size_t astep, صحيح m, صحيح n, صحيح k, مزدوج* b, size_t bstep, مزدوج* hFactors)
{
    CV_INSTRUMENT_REGION();

    صحيح output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    أرجع output;
}

//=============================================================================
// for compatibility with 3.0

صحيح LU(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    أرجع LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

صحيح LU(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    أرجع LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

منطقي Cholesky(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    أرجع CholImpl(A, astep, m, b, bstep, n);
}

منطقي Cholesky(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    أرجع CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
