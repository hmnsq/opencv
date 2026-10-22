// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv { નામાકાશ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> સ્થિર અંતઃરેખ પૂર્ણાંક
LUImpl(_Tp* A, size_t astep, પૂર્ણાંક m, _Tp* b, size_t bstep, પૂર્ણાંક n, _Tp eps)
{
    પૂર્ણાંક i, j, k, p = 1;
    astep /= કદ(A[0]);
    bstep /= કદ(b[0]);

    માટે( i = 0; i < m; i++ )
    {
        k = i;

        માટે( j = i+1; j < m; j++ )
            જો( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        જો( std::abs(A[k*astep + i]) < eps )
            પાછા 0;

        જો( k != i )
        {
            માટે( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            જો( b )
                માટે( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        માટે( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            માટે( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            જો( b )
                માટે( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    જો( b )
    {
        માટે( i = m-1; i >= 0; i-- )
            માટે( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                માટે( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    પાછા p;
}


પૂર્ણાંક LU32f(દશાંશ* A, size_t astep, પૂર્ણાંક m, દશાંશ* b, size_t bstep, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    પાછા output;
}


પૂર્ણાંક LU64f(દ્વિગુણ* A, size_t astep, પૂર્ણાંક m, દ્વિગુણ* b, size_t bstep, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    પાછા output;
}

template<typename _Tp> સ્થિર અંતઃરેખ તાર્કિક
CholImpl(_Tp* A, size_t astep, પૂર્ણાંક m, _Tp* b, size_t bstep, પૂર્ણાંક n)
{
    _Tp* L = A;
    પૂર્ણાંક i, j, k;
    દ્વિગુણ s;
    astep /= કદ(A[0]);
    bstep /= કદ(b[0]);

    માટે( i = 0; i < m; i++ )
    {
        માટે( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            માટે( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        માટે( k = 0; k < j; k++ )
        {
            દ્વિગુણ t = L[i*astep + k];
            s -= t*t;
        }
        જો( s < std::numeric_limits<_Tp>::epsilon() )
            પાછા ખોટું;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    જો (!b)
    {
        માટે( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        પાછા સાચું;
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

    માટે( i = 0; i < m; i++ )
    {
        માટે( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            માટે( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    માટે( i = m-1; i >= 0; i-- )
    {
        માટે( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            માટે( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    માટે( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    પાછા સાચું;
}

તાર્કિક Cholesky32f(દશાંશ* A, size_t astep, પૂર્ણાંક m, દશાંશ* b, size_t bstep, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    તાર્કિક output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    પાછા CholImpl(A, astep, m, b, bstep, n);
}

તાર્કિક Cholesky64f(દ્વિગુણ* A, size_t astep, પૂર્ણાંક m, દ્વિગુણ* b, size_t bstep, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    તાર્કિક output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    પાછા CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> અંતઃરેખ સ્થિર પૂર્ણાંક
sign(_Tp x)
{
    જો (x >= (_Tp)0)
        પાછા 1;
    નહીંતર
        પાછા -1;
}

template<typename _Tp> સ્થિર અંતઃરેખ પૂર્ણાંક
QRImpl(_Tp* A, size_t astep, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= કદ(_Tp);
    bstep /= કદ(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != ખાલી;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    જો (hFactors == ખાલી)
        hFactors = vl + m;

    માટે (પૂર્ણાંક l = 0; l < n; l++)
    {
        //generate vl
        પૂર્ણાંક vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        માટે (પૂર્ણાંક i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        માટે (પૂર્ણાંક i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        માટે (પૂર્ણાંક j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            માટે (પૂર્ણાંક i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            માટે (પૂર્ણાંક i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        માટે (પૂર્ણાંક i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    જો (b)
    {
        //generate new rhs
        માટે (પૂર્ણાંક l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            માટે (પૂર્ણાંક j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            માટે (પૂર્ણાંક j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                માટે (પૂર્ણાંક i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                માટે (પૂર્ણાંક i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        માટે (પૂર્ણાંક i = n - 1; i >= 0; i--)
        {
            માટે (પૂર્ણાંક j = n - 1; j > i; j--)
            {
                માટે (પૂર્ણાંક p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            જો (std::abs(A[i*astep + i]) < eps)
                પાછા 0;
            માટે (પૂર્ણાંક p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    પાછા 1;
}

પૂર્ણાંક QR32f(દશાંશ* A, size_t astep, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, દશાંશ* b, size_t bstep, દશાંશ* hFactors)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    પાછા output;
}

પૂર્ણાંક QR64f(દ્વિગુણ* A, size_t astep, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, દ્વિગુણ* b, size_t bstep, દ્વિગુણ* hFactors)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    પાછા output;
}

//=============================================================================
// for compatibility with 3.0

પૂર્ણાંક LU(દશાંશ* A, size_t astep, પૂર્ણાંક m, દશાંશ* b, size_t bstep, પૂર્ણાંક n)
{
    પાછા LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

પૂર્ણાંક LU(દ્વિગુણ* A, size_t astep, પૂર્ણાંક m, દ્વિગુણ* b, size_t bstep, પૂર્ણાંક n)
{
    પાછા LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

તાર્કિક Cholesky(દશાંશ* A, size_t astep, પૂર્ણાંક m, દશાંશ* b, size_t bstep, પૂર્ણાંક n)
{
    પાછા CholImpl(A, astep, m, b, bstep, n);
}

તાર્કિક Cholesky(દ્વિગુણ* A, size_t astep, પૂર્ણાંક m, દ્વિગુણ* b, size_t bstep, પૂર્ણાંક n)
{
    પાછા CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
