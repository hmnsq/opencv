// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

ନାମାକାଶ cv { ନାମାକାଶ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
LUImpl(_Tp* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, _Tp* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, _Tp eps)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k, p = 1;
    astep /= ଆକାର(A[0]);
    bstep /= ଆକାର(b[0]);

    ପାଇଁ( i = 0; i < m; i++ )
    {
        k = i;

        ପାଇଁ( j = i+1; j < m; j++ )
            ଯଦି( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        ଯଦି( std::abs(A[k*astep + i]) < eps )
            ଫେରନ୍ତୁ 0;

        ଯଦି( k != i )
        {
            ପାଇଁ( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ଯଦି( b )
                ପାଇଁ( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        ପାଇଁ( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            ପାଇଁ( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ଯଦି( b )
                ପାଇଁ( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ଯଦି( b )
    {
        ପାଇଁ( i = m-1; i >= 0; i-- )
            ପାଇଁ( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                ପାଇଁ( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    ଫେରନ୍ତୁ p;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା LU32f(ଦଶମିକ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦଶମିକ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    ଫେରନ୍ତୁ output;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା LU64f(ଦ୍ୱିଗୁଣ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    ଫେରନ୍ତୁ output;
}

template<typename _Tp> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
CholImpl(_Tp* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, _Tp* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    _Tp* L = A;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ଦ୍ୱିଗୁଣ s;
    astep /= ଆକାର(A[0]);
    bstep /= ଆକାର(b[0]);

    ପାଇଁ( i = 0; i < m; i++ )
    {
        ପାଇଁ( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            ପାଇଁ( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        ପାଇଁ( k = 0; k < j; k++ )
        {
            ଦ୍ୱିଗୁଣ t = L[i*astep + k];
            s -= t*t;
        }
        ଯଦି( s < std::numeric_limits<_Tp>::epsilon() )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    ଯଦି (!b)
    {
        ପାଇଁ( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        ଫେରନ୍ତୁ ସତ୍ୟ;
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

    ପାଇଁ( i = 0; i < m; i++ )
    {
        ପାଇଁ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ପାଇଁ( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    ପାଇଁ( i = m-1; i >= 0; i-- )
    {
        ପାଇଁ( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            ପାଇଁ( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    ପାଇଁ( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ Cholesky32f(ଦଶମିକ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦଶମିକ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    ତାର୍କିକ output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    ଫେରନ୍ତୁ CholImpl(A, astep, m, b, bstep, n);
}

ତାର୍କିକ Cholesky64f(ଦ୍ୱିଗୁଣ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_INSTRUMENT_REGION();

    ତାର୍କିକ output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    ଫେରନ୍ତୁ CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> ଧାଡ଼ିଭିତରେ ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sign(_Tp x)
{
    ଯଦି (x >= (_Tp)0)
        ଫେରନ୍ତୁ 1;
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ -1;
}

template<typename _Tp> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
QRImpl(_Tp* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= ଆକାର(_Tp);
    bstep /= ଆକାର(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != ଖାଲି;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    ଯଦି (hFactors == ଖାଲି)
        hFactors = vl + m;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = 0; l < n; l++)
    {
        //generate vl
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ଯଦି (b)
    {
        //generate new rhs
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = n - 1; i >= 0; i--)
        {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = n - 1; j > i; j--)
            {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ଯଦି (std::abs(A[i*astep + i]) < eps)
                ଫେରନ୍ତୁ 0;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା QR32f(ଦଶମିକ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ଦଶମିକ* b, size_t bstep, ଦଶମିକ* hFactors)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    ଫେରନ୍ତୁ output;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା QR64f(ଦ୍ୱିଗୁଣ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ଦ୍ୱିଗୁଣ* b, size_t bstep, ଦ୍ୱିଗୁଣ* hFactors)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    ଫେରନ୍ତୁ output;
}

//=============================================================================
// for compatibility with 3.0

ପୂର୍ଣ୍ଣସଂଖ୍ୟା LU(ଦଶମିକ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦଶମିକ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଫେରନ୍ତୁ LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା LU(ଦ୍ୱିଗୁଣ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଫେରନ୍ତୁ LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

ତାର୍କିକ Cholesky(ଦଶମିକ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦଶମିକ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଫେରନ୍ତୁ CholImpl(A, astep, m, b, bstep, n);
}

ତାର୍କିକ Cholesky(ଦ୍ୱିଗୁଣ* A, size_t astep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ* b, size_t bstep, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଫେରନ୍ତୁ CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
