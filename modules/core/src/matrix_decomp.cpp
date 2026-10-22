// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"

namespace cv { namespace hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

ढाँचा<typename _Tp> अटल अंतरभूत पूर्णांक
LUImpl(_Tp* A, माप_प्रकार astep, पूर्णांक m, _Tp* b, माप_प्रकार bstep, पूर्णांक n, _Tp eps)
{
    पूर्णांक i, j, k, p = 1;
    astep /= माप(A[0]);
    bstep /= माप(b[0]);

    क्रम( i = 0; i < m; i++ )
    {
        k = i;

        क्रम( j = i+1; j < m; j++ )
            अगर( std::असल(A[j*astep + i]) > std::असल(A[k*astep + i]) )
                k = j;

        अगर( std::असल(A[k*astep + i]) < eps )
            वापस 0;

        अगर( k != i )
        {
            क्रम( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            अगर( b )
                क्रम( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        क्रम( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            क्रम( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            अगर( b )
                क्रम( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    अगर( b )
    {
        क्रम( i = m-1; i >= 0; i-- )
            क्रम( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                क्रम( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    वापस p;
}


पूर्णांक LU32f(भग्न* A, माप_प्रकार astep, पूर्णांक m, भग्न* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, भग्न_अंतर*10);
    वापस output;
}


पूर्णांक LU64f(द्विगुन* A, माप_प्रकार astep, पूर्णांक m, द्विगुन* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, द्विग_अंतर*100);
    वापस output;
}

ढाँचा<typename _Tp> अटल अंतरभूत bool
CholImpl(_Tp* A, माप_प्रकार astep, पूर्णांक m, _Tp* b, माप_प्रकार bstep, पूर्णांक n)
{
    _Tp* L = A;
    पूर्णांक i, j, k;
    द्विगुन s;
    astep /= माप(A[0]);
    bstep /= माप(b[0]);

    क्रम( i = 0; i < m; i++ )
    {
        क्रम( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            क्रम( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        क्रम( k = 0; k < j; k++ )
        {
            द्विगुन t = L[i*astep + k];
            s -= t*t;
        }
        अगर( s < std::numeric_limits<_Tp>::epsilon() )
            वापस false;
        L[i*astep + i] = (_Tp)(1./std::वर्ग_मूल(s));
    }

    अगर (!b)
    {
        क्रम( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        वापस true;
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

    क्रम( i = 0; i < m; i++ )
    {
        क्रम( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            क्रम( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    क्रम( i = m-1; i >= 0; i-- )
    {
        क्रम( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            क्रम( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    क्रम( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    वापस true;
}

bool Cholesky32f(भग्न* A, माप_प्रकार astep, पूर्णांक m, भग्न* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    वापस CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky64f(द्विगुन* A, माप_प्रकार astep, पूर्णांक m, द्विगुन* b, माप_प्रकार bstep, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    bool output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    वापस CholImpl(A, astep, m, b, bstep, n);
}

ढाँचा<typename _Tp> अंतरभूत अटल पूर्णांक
sign(_Tp x)
{
    अगर (x >= (_Tp)0)
        वापस 1;
    अन्यथा
        वापस -1;
}

ढाँचा<typename _Tp> अटल अंतरभूत पूर्णांक
QRImpl(_Tp* A, माप_प्रकार astep, पूर्णांक m, पूर्णांक n, पूर्णांक k, _Tp* b, माप_प्रकार bstep, _Tp* hFactors, _Tp eps)
{
    astep /= माप(_Tp);
    bstep /= माप(_Tp);

    cv::Auto_बफर<_Tp> बफर;
    माप_प्रकार buf_आकार_2 = m ? m + n : hFactors != शून्य;
    बफर.allocate(buf_आकार_2);
    _Tp* vl = बफर.तथ्य();
    अगर (hFactors == शून्य)
        hFactors = vl + m;

    क्रम (पूर्णांक l = 0; l < n; l++)
    {
        //generate vl
        पूर्णांक vl_आकार = m - l;
        _Tp vlNorm = (_Tp)0;
        क्रम (पूर्णांक i = 0; i < vl_आकार; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::वर्ग_मूल(vlNorm);
        vlNorm = std::वर्ग_मूल(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        क्रम (पूर्णांक i = 0; i < vl_आकार; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        क्रम (पूर्णांक j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            क्रम (पूर्णांक i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            क्रम (पूर्णांक i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        क्रम (पूर्णांक i = 1; i < vl_आकार; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    अगर (b)
    {
        //generate new rhs
        क्रम (पूर्णांक l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            क्रम (पूर्णांक j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            क्रम (पूर्णांक j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                क्रम (पूर्णांक i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                क्रम (पूर्णांक i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        क्रम (पूर्णांक i = n - 1; i >= 0; i--)
        {
            क्रम (पूर्णांक j = n - 1; j > i; j--)
            {
                क्रम (पूर्णांक p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            अगर (std::असल(A[i*astep + i]) < eps)
                वापस 0;
            क्रम (पूर्णांक p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    वापस 1;
}

पूर्णांक QR32f(भग्न* A, माप_प्रकार astep, पूर्णांक m, पूर्णांक n, पूर्णांक k, भग्न* b, माप_प्रकार bstep, भग्न* hFactors)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, भग्न_अंतर * 10);
    वापस output;
}

पूर्णांक QR64f(द्विगुन* A, माप_प्रकार astep, पूर्णांक m, पूर्णांक n, पूर्णांक k, द्विगुन* b, माप_प्रकार bstep, द्विगुन* hFactors)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, द्विग_अंतर * 100);
    वापस output;
}

//=============================================================================
// for compatibility with 3.0

पूर्णांक LU(भग्न* A, माप_प्रकार astep, पूर्णांक m, भग्न* b, माप_प्रकार bstep, पूर्णांक n)
{
    वापस LUImpl(A, astep, m, b, bstep, n, भग्न_अंतर*10);
}

पूर्णांक LU(द्विगुन* A, माप_प्रकार astep, पूर्णांक m, द्विगुन* b, माप_प्रकार bstep, पूर्णांक n)
{
    वापस LUImpl(A, astep, m, b, bstep, n, द्विग_अंतर*100);
}

bool Cholesky(भग्न* A, माप_प्रकार astep, पूर्णांक m, भग्न* b, माप_प्रकार bstep, पूर्णांक n)
{
    वापस CholImpl(A, astep, m, b, bstep, n);
}

bool Cholesky(द्विगुन* A, माप_प्रकार astep, पूर्णांक m, द्विगुन* b, माप_प्रकार bstep, पूर्णांक n)
{
    वापस CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
