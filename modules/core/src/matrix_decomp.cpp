// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"

নামাকাশ cv { নামাকাশ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> স্থির অন্তঃরেখ পূর্ণসংখ্যা
LUImpl(_Tp* A, size_t astep, পূর্ণসংখ্যা m, _Tp* b, size_t bstep, পূর্ণসংখ্যা n, _Tp eps)
{
    পূর্ণসংখ্যা i, j, k, p = 1;
    astep /= আকার(A[0]);
    bstep /= আকার(b[0]);

    জন্য( i = 0; i < m; i++ )
    {
        k = i;

        জন্য( j = i+1; j < m; j++ )
            যদি( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        যদি( std::abs(A[k*astep + i]) < eps )
            ফেরত 0;

        যদি( k != i )
        {
            জন্য( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            যদি( b )
                জন্য( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        জন্য( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            জন্য( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            যদি( b )
                জন্য( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    যদি( b )
    {
        জন্য( i = m-1; i >= 0; i-- )
            জন্য( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                জন্য( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    ফেরত p;
}


পূর্ণসংখ্যা LU32f(দশমিক* A, size_t astep, পূর্ণসংখ্যা m, দশমিক* b, size_t bstep, পূর্ণসংখ্যা n)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    ফেরত output;
}


পূর্ণসংখ্যা LU64f(দ্বিগুণ* A, size_t astep, পূর্ণসংখ্যা m, দ্বিগুণ* b, size_t bstep, পূর্ণসংখ্যা n)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    ফেরত output;
}

template<typename _Tp> স্থির অন্তঃরেখ যৌক্তিক
CholImpl(_Tp* A, size_t astep, পূর্ণসংখ্যা m, _Tp* b, size_t bstep, পূর্ণসংখ্যা n)
{
    _Tp* L = A;
    পূর্ণসংখ্যা i, j, k;
    দ্বিগুণ s;
    astep /= আকার(A[0]);
    bstep /= আকার(b[0]);

    জন্য( i = 0; i < m; i++ )
    {
        জন্য( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            জন্য( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        জন্য( k = 0; k < j; k++ )
        {
            দ্বিগুণ t = L[i*astep + k];
            s -= t*t;
        }
        যদি( s < std::numeric_limits<_Tp>::epsilon() )
            ফেরত মিথ্যা;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    যদি (!b)
    {
        জন্য( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        ফেরত সত্য;
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

    জন্য( i = 0; i < m; i++ )
    {
        জন্য( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            জন্য( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    জন্য( i = m-1; i >= 0; i-- )
    {
        জন্য( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            জন্য( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    জন্য( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    ফেরত সত্য;
}

যৌক্তিক Cholesky32f(দশমিক* A, size_t astep, পূর্ণসংখ্যা m, দশমিক* b, size_t bstep, পূর্ণসংখ্যা n)
{
    CV_INSTRUMENT_REGION();

    যৌক্তিক output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    ফেরত CholImpl(A, astep, m, b, bstep, n);
}

যৌক্তিক Cholesky64f(দ্বিগুণ* A, size_t astep, পূর্ণসংখ্যা m, দ্বিগুণ* b, size_t bstep, পূর্ণসংখ্যা n)
{
    CV_INSTRUMENT_REGION();

    যৌক্তিক output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    ফেরত CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> অন্তঃরেখ স্থির পূর্ণসংখ্যা
sign(_Tp x)
{
    যদি (x >= (_Tp)0)
        ফেরত 1;
    অন্যথা
        ফেরত -1;
}

template<typename _Tp> স্থির অন্তঃরেখ পূর্ণসংখ্যা
QRImpl(_Tp* A, size_t astep, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= আকার(_Tp);
    bstep /= আকার(_Tp);

    cv::Auto_বাফার<_Tp> বাফার;
    size_t buf_আকার_2 = m ? m + n : hFactors != ফাঁকা;
    বাফার.allocate(buf_আকার_2);
    _Tp* vl = বাফার.তথ্য();
    যদি (hFactors == ফাঁকা)
        hFactors = vl + m;

    জন্য (পূর্ণসংখ্যা l = 0; l < n; l++)
    {
        //generate vl
        পূর্ণসংখ্যা vl_আকার = m - l;
        _Tp vlNorm = (_Tp)0;
        জন্য (পূর্ণসংখ্যা i = 0; i < vl_আকার; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        জন্য (পূর্ণসংখ্যা i = 0; i < vl_আকার; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        জন্য (পূর্ণসংখ্যা j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            জন্য (পূর্ণসংখ্যা i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            জন্য (পূর্ণসংখ্যা i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        জন্য (পূর্ণসংখ্যা i = 1; i < vl_আকার; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    যদি (b)
    {
        //generate new rhs
        জন্য (পূর্ণসংখ্যা l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            জন্য (পূর্ণসংখ্যা j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            জন্য (পূর্ণসংখ্যা j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                জন্য (পূর্ণসংখ্যা i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                জন্য (পূর্ণসংখ্যা i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        জন্য (পূর্ণসংখ্যা i = n - 1; i >= 0; i--)
        {
            জন্য (পূর্ণসংখ্যা j = n - 1; j > i; j--)
            {
                জন্য (পূর্ণসংখ্যা p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            যদি (std::abs(A[i*astep + i]) < eps)
                ফেরত 0;
            জন্য (পূর্ণসংখ্যা p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    ফেরত 1;
}

পূর্ণসংখ্যা QR32f(দশমিক* A, size_t astep, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, দশমিক* b, size_t bstep, দশমিক* hFactors)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    ফেরত output;
}

পূর্ণসংখ্যা QR64f(দ্বিগুণ* A, size_t astep, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, দ্বিগুণ* b, size_t bstep, দ্বিগুণ* hFactors)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    ফেরত output;
}

//=============================================================================
// for compatibility with 3.0

পূর্ণসংখ্যা LU(দশমিক* A, size_t astep, পূর্ণসংখ্যা m, দশমিক* b, size_t bstep, পূর্ণসংখ্যা n)
{
    ফেরত LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

পূর্ণসংখ্যা LU(দ্বিগুণ* A, size_t astep, পূর্ণসংখ্যা m, দ্বিগুণ* b, size_t bstep, পূর্ণসংখ্যা n)
{
    ফেরত LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

যৌক্তিক Cholesky(দশমিক* A, size_t astep, পূর্ণসংখ্যা m, দশমিক* b, size_t bstep, পূর্ণসংখ্যা n)
{
    ফেরত CholImpl(A, astep, m, b, bstep, n);
}

যৌক্তিক Cholesky(দ্বিগুণ* A, size_t astep, পূর্ণসংখ্যা m, দ্বিগুণ* b, size_t bstep, পূর্ণসংখ্যা n)
{
    ফেরত CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
