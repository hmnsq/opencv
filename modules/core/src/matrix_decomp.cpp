// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"

നാമാകാശ cv { നാമാകാശ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
LUImpl(_Tp* A, size_t astep, പൂർണ്ണസംഖ്യ m, _Tp* b, size_t bstep, പൂർണ്ണസംഖ്യ n, _Tp eps)
{
    പൂർണ്ണസംഖ്യ i, j, k, p = 1;
    astep /= വലുപ്പം(A[0]);
    bstep /= വലുപ്പം(b[0]);

    വേണ്ടി( i = 0; i < m; i++ )
    {
        k = i;

        വേണ്ടി( j = i+1; j < m; j++ )
            എങ്കിൽ( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        എങ്കിൽ( std::abs(A[k*astep + i]) < eps )
            തിരികെ 0;

        എങ്കിൽ( k != i )
        {
            വേണ്ടി( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            എങ്കിൽ( b )
                വേണ്ടി( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        വേണ്ടി( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            വേണ്ടി( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            എങ്കിൽ( b )
                വേണ്ടി( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    എങ്കിൽ( b )
    {
        വേണ്ടി( i = m-1; i >= 0; i-- )
            വേണ്ടി( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                വേണ്ടി( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    തിരികെ p;
}


പൂർണ്ണസംഖ്യ LU32f(ദശാംശം* A, size_t astep, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    തിരികെ output;
}


പൂർണ്ണസംഖ്യ LU64f(ഇരട്ട* A, size_t astep, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    തിരികെ output;
}

template<typename _Tp> സ്ഥിതം വരിയിൽ തർക്കം
CholImpl(_Tp* A, size_t astep, പൂർണ്ണസംഖ്യ m, _Tp* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    _Tp* L = A;
    പൂർണ്ണസംഖ്യ i, j, k;
    ഇരട്ട s;
    astep /= വലുപ്പം(A[0]);
    bstep /= വലുപ്പം(b[0]);

    വേണ്ടി( i = 0; i < m; i++ )
    {
        വേണ്ടി( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            വേണ്ടി( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        വേണ്ടി( k = 0; k < j; k++ )
        {
            ഇരട്ട t = L[i*astep + k];
            s -= t*t;
        }
        എങ്കിൽ( s < std::numeric_limits<_Tp>::epsilon() )
            തിരികെ അസത്യം;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    എങ്കിൽ (!b)
    {
        വേണ്ടി( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        തിരികെ സത്യം;
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

    വേണ്ടി( i = 0; i < m; i++ )
    {
        വേണ്ടി( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            വേണ്ടി( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    വേണ്ടി( i = m-1; i >= 0; i-- )
    {
        വേണ്ടി( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            വേണ്ടി( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    വേണ്ടി( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    തിരികെ സത്യം;
}

തർക്കം Cholesky32f(ദശാംശം* A, size_t astep, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    തർക്കം output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    തിരികെ CholImpl(A, astep, m, b, bstep, n);
}

തർക്കം Cholesky64f(ഇരട്ട* A, size_t astep, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    തർക്കം output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    തിരികെ CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> വരിയിൽ സ്ഥിതം പൂർണ്ണസംഖ്യ
sign(_Tp x)
{
    എങ്കിൽ (x >= (_Tp)0)
        തിരികെ 1;
    അല്ലെങ്കിൽ
        തിരികെ -1;
}

template<typename _Tp> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
QRImpl(_Tp* A, size_t astep, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= വലുപ്പം(_Tp);
    bstep /= വലുപ്പം(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != ശൂന്യ;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    എങ്കിൽ (hFactors == ശൂന്യ)
        hFactors = vl + m;

    വേണ്ടി (പൂർണ്ണസംഖ്യ l = 0; l < n; l++)
    {
        //generate vl
        പൂർണ്ണസംഖ്യ vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            വേണ്ടി (പൂർണ്ണസംഖ്യ i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    എങ്കിൽ (b)
    {
        //generate new rhs
        വേണ്ടി (പൂർണ്ണസംഖ്യ l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                വേണ്ടി (പൂർണ്ണസംഖ്യ i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = n - 1; i >= 0; i--)
        {
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = n - 1; j > i; j--)
            {
                വേണ്ടി (പൂർണ്ണസംഖ്യ p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            എങ്കിൽ (std::abs(A[i*astep + i]) < eps)
                തിരികെ 0;
            വേണ്ടി (പൂർണ്ണസംഖ്യ p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    തിരികെ 1;
}

പൂർണ്ണസംഖ്യ QR32f(ദശാംശം* A, size_t astep, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, ദശാംശം* b, size_t bstep, ദശാംശം* hFactors)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    തിരികെ output;
}

പൂർണ്ണസംഖ്യ QR64f(ഇരട്ട* A, size_t astep, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, ഇരട്ട* b, size_t bstep, ഇരട്ട* hFactors)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    തിരികെ output;
}

//=============================================================================
// for compatibility with 3.0

പൂർണ്ണസംഖ്യ LU(ദശാംശം* A, size_t astep, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    തിരികെ LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

പൂർണ്ണസംഖ്യ LU(ഇരട്ട* A, size_t astep, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    തിരികെ LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

തർക്കം Cholesky(ദശാംശം* A, size_t astep, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    തിരികെ CholImpl(A, astep, m, b, bstep, n);
}

തർക്കം Cholesky(ഇരട്ട* A, size_t astep, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t bstep, പൂർണ്ണസംഖ്യ n)
{
    തിരികെ CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
