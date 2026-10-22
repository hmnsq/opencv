// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"

நாமாகாஶ cv { நாமாகாஶ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> நிலையான உள்வரி முழு
LUImpl(_Tp* A, size_t astep, முழு m, _Tp* b, size_t bstep, முழு n, _Tp eps)
{
    முழு i, j, k, p = 1;
    astep /= அளவு(A[0]);
    bstep /= அளவு(b[0]);

    க்கு( i = 0; i < m; i++ )
    {
        k = i;

        க்கு( j = i+1; j < m; j++ )
            எனில்( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        எனில்( std::abs(A[k*astep + i]) < eps )
            திரும்பு 0;

        எனில்( k != i )
        {
            க்கு( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            எனில்( b )
                க்கு( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        க்கு( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            க்கு( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            எனில்( b )
                க்கு( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    எனில்( b )
    {
        க்கு( i = m-1; i >= 0; i-- )
            க்கு( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                க்கு( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    திரும்பு p;
}


முழு LU32f(தசம* A, size_t astep, முழு m, தசம* b, size_t bstep, முழு n)
{
    CV_INSTRUMENT_REGION();

    முழு output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    திரும்பு output;
}


முழு LU64f(இரட்டை* A, size_t astep, முழு m, இரட்டை* b, size_t bstep, முழு n)
{
    CV_INSTRUMENT_REGION();

    முழு output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    திரும்பு output;
}

template<typename _Tp> நிலையான உள்வரி தருக்க
CholImpl(_Tp* A, size_t astep, முழு m, _Tp* b, size_t bstep, முழு n)
{
    _Tp* L = A;
    முழு i, j, k;
    இரட்டை s;
    astep /= அளவு(A[0]);
    bstep /= அளவு(b[0]);

    க்கு( i = 0; i < m; i++ )
    {
        க்கு( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            க்கு( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        க்கு( k = 0; k < j; k++ )
        {
            இரட்டை t = L[i*astep + k];
            s -= t*t;
        }
        எனில்( s < std::numeric_limits<_Tp>::epsilon() )
            திரும்பு பொய்;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    எனில் (!b)
    {
        க்கு( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        திரும்பு உண்மை;
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

    க்கு( i = 0; i < m; i++ )
    {
        க்கு( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            க்கு( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    க்கு( i = m-1; i >= 0; i-- )
    {
        க்கு( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            க்கு( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    க்கு( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    திரும்பு உண்மை;
}

தருக்க Cholesky32f(தசம* A, size_t astep, முழு m, தசம* b, size_t bstep, முழு n)
{
    CV_INSTRUMENT_REGION();

    தருக்க output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    திரும்பு CholImpl(A, astep, m, b, bstep, n);
}

தருக்க Cholesky64f(இரட்டை* A, size_t astep, முழு m, இரட்டை* b, size_t bstep, முழு n)
{
    CV_INSTRUMENT_REGION();

    தருக்க output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    திரும்பு CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> உள்வரி நிலையான முழு
sign(_Tp x)
{
    எனில் (x >= (_Tp)0)
        திரும்பு 1;
    இல்லையெனில்
        திரும்பு -1;
}

template<typename _Tp> நிலையான உள்வரி முழு
QRImpl(_Tp* A, size_t astep, முழு m, முழு n, முழு k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= அளவு(_Tp);
    bstep /= அளவு(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != வெற்று;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    எனில் (hFactors == வெற்று)
        hFactors = vl + m;

    க்கு (முழு l = 0; l < n; l++)
    {
        //generate vl
        முழு vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        க்கு (முழு i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        க்கு (முழு i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        க்கு (முழு j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            க்கு (முழு i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            க்கு (முழு i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        க்கு (முழு i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    எனில் (b)
    {
        //generate new rhs
        க்கு (முழு l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            க்கு (முழு j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            க்கு (முழு j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                க்கு (முழு i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                க்கு (முழு i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        க்கு (முழு i = n - 1; i >= 0; i--)
        {
            க்கு (முழு j = n - 1; j > i; j--)
            {
                க்கு (முழு p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            எனில் (std::abs(A[i*astep + i]) < eps)
                திரும்பு 0;
            க்கு (முழு p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    திரும்பு 1;
}

முழு QR32f(தசம* A, size_t astep, முழு m, முழு n, முழு k, தசம* b, size_t bstep, தசம* hFactors)
{
    CV_INSTRUMENT_REGION();

    முழு output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    திரும்பு output;
}

முழு QR64f(இரட்டை* A, size_t astep, முழு m, முழு n, முழு k, இரட்டை* b, size_t bstep, இரட்டை* hFactors)
{
    CV_INSTRUMENT_REGION();

    முழு output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    திரும்பு output;
}

//=============================================================================
// for compatibility with 3.0

முழு LU(தசம* A, size_t astep, முழு m, தசம* b, size_t bstep, முழு n)
{
    திரும்பு LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

முழு LU(இரட்டை* A, size_t astep, முழு m, இரட்டை* b, size_t bstep, முழு n)
{
    திரும்பு LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

தருக்க Cholesky(தசம* A, size_t astep, முழு m, தசம* b, size_t bstep, முழு n)
{
    திரும்பு CholImpl(A, astep, m, b, bstep, n);
}

தருக்க Cholesky(இரட்டை* A, size_t astep, முழு m, இரட்டை* b, size_t bstep, முழு n)
{
    திரும்பு CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
