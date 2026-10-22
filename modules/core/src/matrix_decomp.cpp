// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"

నామాకాశ cv { నామాకాశ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

మూస<రకం_పేరు _Tp> స్థిర అంతఃవరుస పూర్ణాంకం
LUImpl(_Tp* A, size_t astep, పూర్ణాంకం m, _Tp* b, size_t bstep, పూర్ణాంకం n, _Tp eps)
{
    పూర్ణాంకం i, j, k, p = 1;
    astep /= పరిమాణం(A[0]);
    bstep /= పరిమాణం(b[0]);

    కొరకు( i = 0; i < m; i++ )
    {
        k = i;

        కొరకు( j = i+1; j < m; j++ )
            ఒకవేళ( std::నిరపేక్ష(A[j*astep + i]) > std::నిరపేక్ష(A[k*astep + i]) )
                k = j;

        ఒకవేళ( std::నిరపేక్ష(A[k*astep + i]) < eps )
            తిరిగి 0;

        ఒకవేళ( k != i )
        {
            కొరకు( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            ఒకవేళ( b )
                కొరకు( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        కొరకు( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            కొరకు( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            ఒకవేళ( b )
                కొరకు( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    ఒకవేళ( b )
    {
        కొరకు( i = m-1; i >= 0; i-- )
            కొరకు( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                కొరకు( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    తిరిగి p;
}


పూర్ణాంకం LU32f(దశాంశం* A, size_t astep, పూర్ణాంకం m, దశాంశం* b, size_t bstep, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    తిరిగి output;
}


పూర్ణాంకం LU64f(ద్విగుణ* A, size_t astep, పూర్ణాంకం m, ద్విగుణ* b, size_t bstep, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    తిరిగి output;
}

మూస<రకం_పేరు _Tp> స్థిర అంతఃవరుస తార్కిక
CholImpl(_Tp* A, size_t astep, పూర్ణాంకం m, _Tp* b, size_t bstep, పూర్ణాంకం n)
{
    _Tp* L = A;
    పూర్ణాంకం i, j, k;
    ద్విగుణ s;
    astep /= పరిమాణం(A[0]);
    bstep /= పరిమాణం(b[0]);

    కొరకు( i = 0; i < m; i++ )
    {
        కొరకు( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            కొరకు( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        కొరకు( k = 0; k < j; k++ )
        {
            ద్విగుణ t = L[i*astep + k];
            s -= t*t;
        }
        ఒకవేళ( s < std::numeric_limits<_Tp>::epsilon() )
            తిరిగి అబద్ధం;
        L[i*astep + i] = (_Tp)(1./std::వర్గమూలం(s));
    }

    ఒకవేళ (!b)
    {
        కొరకు( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        తిరిగి నిజం;
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

    కొరకు( i = 0; i < m; i++ )
    {
        కొరకు( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            కొరకు( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    కొరకు( i = m-1; i >= 0; i-- )
    {
        కొరకు( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            కొరకు( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    కొరకు( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    తిరిగి నిజం;
}

తార్కిక Cholesky32f(దశాంశం* A, size_t astep, పూర్ణాంకం m, దశాంశం* b, size_t bstep, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    తార్కిక output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    తిరిగి CholImpl(A, astep, m, b, bstep, n);
}

తార్కిక Cholesky64f(ద్విగుణ* A, size_t astep, పూర్ణాంకం m, ద్విగుణ* b, size_t bstep, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    తార్కిక output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    తిరిగి CholImpl(A, astep, m, b, bstep, n);
}

మూస<రకం_పేరు _Tp> అంతఃవరుస స్థిర పూర్ణాంకం
sign(_Tp x)
{
    ఒకవేళ (x >= (_Tp)0)
        తిరిగి 1;
    లేకపోతే
        తిరిగి -1;
}

మూస<రకం_పేరు _Tp> స్థిర అంతఃవరుస పూర్ణాంకం
QRImpl(_Tp* A, size_t astep, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= పరిమాణం(_Tp);
    bstep /= పరిమాణం(_Tp);

    cv::Auto_బఫరు<_Tp> బఫరు;
    size_t buf_కొలత_2 = m ? m + n : hFactors != ఖాళీ;
    బఫరు.allocate(buf_కొలత_2);
    _Tp* vl = బఫరు.దత్తాంశం();
    ఒకవేళ (hFactors == ఖాళీ)
        hFactors = vl + m;

    కొరకు (పూర్ణాంకం l = 0; l < n; l++)
    {
        //generate vl
        పూర్ణాంకం vl_కొలత = m - l;
        _Tp vlNorm = (_Tp)0;
        కొరకు (పూర్ణాంకం i = 0; i < vl_కొలత; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::వర్గమూలం(vlNorm);
        vlNorm = std::వర్గమూలం(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        కొరకు (పూర్ణాంకం i = 0; i < vl_కొలత; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        కొరకు (పూర్ణాంకం j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            కొరకు (పూర్ణాంకం i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            కొరకు (పూర్ణాంకం i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        కొరకు (పూర్ణాంకం i = 1; i < vl_కొలత; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    ఒకవేళ (b)
    {
        //generate new rhs
        కొరకు (పూర్ణాంకం l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            కొరకు (పూర్ణాంకం j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            కొరకు (పూర్ణాంకం j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                కొరకు (పూర్ణాంకం i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                కొరకు (పూర్ణాంకం i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        కొరకు (పూర్ణాంకం i = n - 1; i >= 0; i--)
        {
            కొరకు (పూర్ణాంకం j = n - 1; j > i; j--)
            {
                కొరకు (పూర్ణాంకం p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            ఒకవేళ (std::నిరపేక్ష(A[i*astep + i]) < eps)
                తిరిగి 0;
            కొరకు (పూర్ణాంకం p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    తిరిగి 1;
}

పూర్ణాంకం QR32f(దశాంశం* A, size_t astep, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, దశాంశం* b, size_t bstep, దశాంశం* hFactors)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    తిరిగి output;
}

పూర్ణాంకం QR64f(ద్విగుణ* A, size_t astep, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, ద్విగుణ* b, size_t bstep, ద్విగుణ* hFactors)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    తిరిగి output;
}

//=============================================================================
// for compatibility with 3.0

పూర్ణాంకం LU(దశాంశం* A, size_t astep, పూర్ణాంకం m, దశాంశం* b, size_t bstep, పూర్ణాంకం n)
{
    తిరిగి LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

పూర్ణాంకం LU(ద్విగుణ* A, size_t astep, పూర్ణాంకం m, ద్విగుణ* b, size_t bstep, పూర్ణాంకం n)
{
    తిరిగి LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

తార్కిక Cholesky(దశాంశం* A, size_t astep, పూర్ణాంకం m, దశాంశం* b, size_t bstep, పూర్ణాంకం n)
{
    తిరిగి CholImpl(A, astep, m, b, bstep, n);
}

తార్కిక Cholesky(ద్విగుణ* A, size_t astep, పూర్ణాంకం m, ద్విగుణ* b, size_t bstep, పూర్ణాంకం n)
{
    తిరిగి CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
