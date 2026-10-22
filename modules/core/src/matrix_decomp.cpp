// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"

نطاق cv { نطاق hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> ساكن مضمن صحيح
LUImpl(_Tp* A, size_t astep, صحيح m, _Tp* b, size_t bstep, صحيح n, _Tp eps)
{
    صحيح i, j, k, p = 1;
    astep /= حجم(A[0]);
    bstep /= حجم(b[0]);

    עבור( i = 0; i < m; i++ )
    {
        k = i;

        עבור( j = i+1; j < m; j++ )
            אם( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        אם( std::abs(A[k*astep + i]) < eps )
            החזר 0;

        אם( k != i )
        {
            עבור( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            אם( b )
                עבור( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        עבור( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            עבור( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            אם( b )
                עבור( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    אם( b )
    {
        עבור( i = m-1; i >= 0; i-- )
            עבור( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                עבור( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    החזר p;
}


صحيح LU32f(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    صحيح output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    החזר output;
}


صحيح LU64f(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    صحيح output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    החזר output;
}

template<typename _Tp> ساكن مضمن منطقي
CholImpl(_Tp* A, size_t astep, صحيح m, _Tp* b, size_t bstep, صحيح n)
{
    _Tp* L = A;
    صحيح i, j, k;
    مزدوج s;
    astep /= حجم(A[0]);
    bstep /= حجم(b[0]);

    עבור( i = 0; i < m; i++ )
    {
        עבור( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            עבור( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        עבור( k = 0; k < j; k++ )
        {
            مزدوج t = L[i*astep + k];
            s -= t*t;
        }
        אם( s < std::numeric_limits<_Tp>::epsilon() )
            החזר שקר;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    אם (!b)
    {
        עבור( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        החזר אמת;
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

    עבור( i = 0; i < m; i++ )
    {
        עבור( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            עבור( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    עבור( i = m-1; i >= 0; i-- )
    {
        עבור( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            עבור( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    עבור( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    החזר אמת;
}

منطقي Cholesky32f(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    منطقي output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    החזר CholImpl(A, astep, m, b, bstep, n);
}

منطقي Cholesky64f(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    CV_INSTRUMENT_REGION();

    منطقي output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    החזר CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> مضمن ساكن صحيح
sign(_Tp x)
{
    אם (x >= (_Tp)0)
        החזר 1;
    אחרת
        החזר -1;
}

template<typename _Tp> ساكن مضمن صحيح
QRImpl(_Tp* A, size_t astep, صحيح m, صحيح n, صحيح k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= حجم(_Tp);
    bstep /= حجم(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != عدم;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    אם (hFactors == عدم)
        hFactors = vl + m;

    עבור (صحيح l = 0; l < n; l++)
    {
        //generate vl
        صحيح vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        עבור (صحيح i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        עבור (صحيح i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        עבור (صحيح j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            עבור (صحيح i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            עבור (صحيح i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        עבור (صحيح i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    אם (b)
    {
        //generate new rhs
        עבור (صحيح l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            עבור (صحيح j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            עבור (صحيح j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                עבור (صحيح i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                עבור (صحيح i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        עבור (صحيح i = n - 1; i >= 0; i--)
        {
            עבור (صحيح j = n - 1; j > i; j--)
            {
                עבור (صحيح p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            אם (std::abs(A[i*astep + i]) < eps)
                החזר 0;
            עבור (صحيح p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    החזר 1;
}

صحيح QR32f(عائم* A, size_t astep, صحيح m, صحيح n, صحيح k, عائم* b, size_t bstep, عائم* hFactors)
{
    CV_INSTRUMENT_REGION();

    صحيح output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    החזר output;
}

صحيح QR64f(مزدوج* A, size_t astep, صحيح m, صحيح n, صحيح k, مزدوج* b, size_t bstep, مزدوج* hFactors)
{
    CV_INSTRUMENT_REGION();

    صحيح output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    החזר output;
}

//=============================================================================
// for compatibility with 3.0

صحيح LU(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    החזר LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

صحيح LU(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    החזר LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

منطقي Cholesky(عائم* A, size_t astep, صحيح m, عائم* b, size_t bstep, صحيح n)
{
    החזר CholImpl(A, astep, m, b, bstep, n);
}

منطقي Cholesky(مزدوج* A, size_t astep, صحيح m, مزدوج* b, size_t bstep, صحيح n)
{
    החזר CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
