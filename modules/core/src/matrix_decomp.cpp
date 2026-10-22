// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"

नामाकाश cv { नामाकाश hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> स्थिर अन्तःस्थ पूर्णाङ्क
LUImpl(_Tp* A, size_t astep, पूर्णाङ्क m, _Tp* b, size_t bstep, पूर्णाङ्क n, _Tp eps)
{
    पूर्णाङ्क i, j, k, p = 1;
    astep /= परिमाण(A[0]);
    bstep /= परिमाण(b[0]);

    प्रति( i = 0; i < m; i++ )
    {
        k = i;

        प्रति( j = i+1; j < m; j++ )
            यदि( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        यदि( std::abs(A[k*astep + i]) < eps )
            प्रत्यागम 0;

        यदि( k != i )
        {
            प्रति( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            यदि( b )
                प्रति( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        प्रति( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            प्रति( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            यदि( b )
                प्रति( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    यदि( b )
    {
        प्रति( i = m-1; i >= 0; i-- )
            प्रति( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                प्रति( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    प्रत्यागम p;
}


पूर्णाङ्क LU32f(भिन्न* A, size_t astep, पूर्णाङ्क m, भिन्न* b, size_t bstep, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    प्रत्यागम output;
}


पूर्णाङ्क LU64f(द्विगुण* A, size_t astep, पूर्णाङ्क m, द्विगुण* b, size_t bstep, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    प्रत्यागम output;
}

template<typename _Tp> स्थिर अन्तःस्थ तार्किक
CholImpl(_Tp* A, size_t astep, पूर्णाङ्क m, _Tp* b, size_t bstep, पूर्णाङ्क n)
{
    _Tp* L = A;
    पूर्णाङ्क i, j, k;
    द्विगुण s;
    astep /= परिमाण(A[0]);
    bstep /= परिमाण(b[0]);

    प्रति( i = 0; i < m; i++ )
    {
        प्रति( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            प्रति( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        प्रति( k = 0; k < j; k++ )
        {
            द्विगुण t = L[i*astep + k];
            s -= t*t;
        }
        यदि( s < std::numeric_limits<_Tp>::epsilon() )
            प्रत्यागम असत्य;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    यदि (!b)
    {
        प्रति( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        प्रत्यागम सत्य;
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

    प्रति( i = 0; i < m; i++ )
    {
        प्रति( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            प्रति( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    प्रति( i = m-1; i >= 0; i-- )
    {
        प्रति( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            प्रति( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    प्रति( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    प्रत्यागम सत्य;
}

तार्किक Cholesky32f(भिन्न* A, size_t astep, पूर्णाङ्क m, भिन्न* b, size_t bstep, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    तार्किक output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    प्रत्यागम CholImpl(A, astep, m, b, bstep, n);
}

तार्किक Cholesky64f(द्विगुण* A, size_t astep, पूर्णाङ्क m, द्विगुण* b, size_t bstep, पूर्णाङ्क n)
{
    CV_INSTRUMENT_REGION();

    तार्किक output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    प्रत्यागम CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> अन्तःस्थ स्थिर पूर्णाङ्क
sign(_Tp x)
{
    यदि (x >= (_Tp)0)
        प्रत्यागम 1;
    अन्यथा
        प्रत्यागम -1;
}

template<typename _Tp> स्थिर अन्तःस्थ पूर्णाङ्क
QRImpl(_Tp* A, size_t astep, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= परिमाण(_Tp);
    bstep /= परिमाण(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != रिक्त;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    यदि (hFactors == रिक्त)
        hFactors = vl + m;

    प्रति (पूर्णाङ्क l = 0; l < n; l++)
    {
        //generate vl
        पूर्णाङ्क vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        प्रति (पूर्णाङ्क i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        प्रति (पूर्णाङ्क i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        प्रति (पूर्णाङ्क j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            प्रति (पूर्णाङ्क i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            प्रति (पूर्णाङ्क i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        प्रति (पूर्णाङ्क i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    यदि (b)
    {
        //generate new rhs
        प्रति (पूर्णाङ्क l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            प्रति (पूर्णाङ्क j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            प्रति (पूर्णाङ्क j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                प्रति (पूर्णाङ्क i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                प्रति (पूर्णाङ्क i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        प्रति (पूर्णाङ्क i = n - 1; i >= 0; i--)
        {
            प्रति (पूर्णाङ्क j = n - 1; j > i; j--)
            {
                प्रति (पूर्णाङ्क p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            यदि (std::abs(A[i*astep + i]) < eps)
                प्रत्यागम 0;
            प्रति (पूर्णाङ्क p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    प्रत्यागम 1;
}

पूर्णाङ्क QR32f(भिन्न* A, size_t astep, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, भिन्न* b, size_t bstep, भिन्न* hFactors)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    प्रत्यागम output;
}

पूर्णाङ्क QR64f(द्विगुण* A, size_t astep, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, द्विगुण* b, size_t bstep, द्विगुण* hFactors)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    प्रत्यागम output;
}

//=============================================================================
// for compatibility with 3.0

पूर्णाङ्क LU(भिन्न* A, size_t astep, पूर्णाङ्क m, भिन्न* b, size_t bstep, पूर्णाङ्क n)
{
    प्रत्यागम LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

पूर्णाङ्क LU(द्विगुण* A, size_t astep, पूर्णाङ्क m, द्विगुण* b, size_t bstep, पूर्णाङ्क n)
{
    प्रत्यागम LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

तार्किक Cholesky(भिन्न* A, size_t astep, पूर्णाङ्क m, भिन्न* b, size_t bstep, पूर्णाङ्क n)
{
    प्रत्यागम CholImpl(A, astep, m, b, bstep, n);
}

तार्किक Cholesky(द्विगुण* A, size_t astep, पूर्णाङ्क m, द्विगुण* b, size_t bstep, पूर्णाङ्क n)
{
    प्रत्यागम CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
