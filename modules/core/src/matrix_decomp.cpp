// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"

নামাকাশ cv { নামাকাশ hal {

/****************************************************************************************\
*                     LU & Cholesky implementation for small matrices                    *
\****************************************************************************************/

template<typename _Tp> স্থির অন্তঃস্থ পূর্ণাঙ্ক
LUImpl(_Tp* A, size_t astep, পূর্ণাঙ্ক m, _Tp* b, size_t bstep, পূর্ণাঙ্ক n, _Tp eps)
{
    পূর্ণাঙ্ক i, j, k, p = 1;
    astep /= পরিমাণ(A[0]);
    bstep /= পরিমাণ(b[0]);

    প্রতি( i = 0; i < m; i++ )
    {
        k = i;

        প্রতি( j = i+1; j < m; j++ )
            যদি( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) )
                k = j;

        যদি( std::abs(A[k*astep + i]) < eps )
            প্রত্যাগম 0;

        যদি( k != i )
        {
            প্রতি( j = i; j < m; j++ )
                std::swap(A[i*astep + j], A[k*astep + j]);
            যদি( b )
                প্রতি( j = 0; j < n; j++ )
                    std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        প্রতি( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            প্রতি( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            যদি( b )
                প্রতি( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    যদি( b )
    {
        প্রতি( i = m-1; i >= 0; i-- )
            প্রতি( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                প্রতি( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    প্রত্যাগম p;
}


পূর্ণাঙ্ক LU32f(ভিন্ন* A, size_t astep, পূর্ণাঙ্ক m, ভিন্ন* b, size_t bstep, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক output;
    CALL_HAL_RET(LU32f, cv_hal_LU32f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
    প্রত্যাগম output;
}


পূর্ণাঙ্ক LU64f(দ্বিগুণ* A, size_t astep, পূর্ণাঙ্ক m, দ্বিগুণ* b, size_t bstep, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক output;
    CALL_HAL_RET(LU64f, cv_hal_LU64f, output, A, astep, m, b, bstep, n)
    output = LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
    প্রত্যাগম output;
}

template<typename _Tp> স্থির অন্তঃস্থ তার্কিক
CholImpl(_Tp* A, size_t astep, পূর্ণাঙ্ক m, _Tp* b, size_t bstep, পূর্ণাঙ্ক n)
{
    _Tp* L = A;
    পূর্ণাঙ্ক i, j, k;
    দ্বিগুণ s;
    astep /= পরিমাণ(A[0]);
    bstep /= পরিমাণ(b[0]);

    প্রতি( i = 0; i < m; i++ )
    {
        প্রতি( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            প্রতি( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        প্রতি( k = 0; k < j; k++ )
        {
            দ্বিগুণ t = L[i*astep + k];
            s -= t*t;
        }
        যদি( s < std::numeric_limits<_Tp>::epsilon() )
            প্রত্যাগম অসত্য;
        L[i*astep + i] = (_Tp)(1./std::sqrt(s));
    }

    যদি (!b)
    {
        প্রতি( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        প্রত্যাগম সত্য;
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

    প্রতি( i = 0; i < m; i++ )
    {
        প্রতি( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            প্রতি( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    প্রতি( i = m-1; i >= 0; i-- )
    {
        প্রতি( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            প্রতি( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    প্রতি( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    প্রত্যাগম সত্য;
}

তার্কিক Cholesky32f(ভিন্ন* A, size_t astep, পূর্ণাঙ্ক m, ভিন্ন* b, size_t bstep, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    তার্কিক output;
    CALL_HAL_RET(Cholesky32f, cv_hal_Cholesky32f, output, A, astep, m, b, bstep, n)
    প্রত্যাগম CholImpl(A, astep, m, b, bstep, n);
}

তার্কিক Cholesky64f(দ্বিগুণ* A, size_t astep, পূর্ণাঙ্ক m, দ্বিগুণ* b, size_t bstep, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    তার্কিক output;
    CALL_HAL_RET(Cholesky64f, cv_hal_Cholesky64f, output, A, astep, m, b, bstep, n)
    প্রত্যাগম CholImpl(A, astep, m, b, bstep, n);
}

template<typename _Tp> অন্তঃস্থ স্থির পূর্ণাঙ্ক
sign(_Tp x)
{
    যদি (x >= (_Tp)0)
        প্রত্যাগম 1;
    অন্যথা
        প্রত্যাগম -1;
}

template<typename _Tp> স্থির অন্তঃস্থ পূর্ণাঙ্ক
QRImpl(_Tp* A, size_t astep, পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    astep /= পরিমাণ(_Tp);
    bstep /= পরিমাণ(_Tp);

    cv::AutoBuffer<_Tp> buffer;
    size_t buf_size = m ? m + n : hFactors != রিক্ত;
    buffer.allocate(buf_size);
    _Tp* vl = buffer.data();
    যদি (hFactors == রিক্ত)
        hFactors = vl + m;

    প্রতি (পূর্ণাঙ্ক l = 0; l < n; l++)
    {
        //generate vl
        পূর্ণাঙ্ক vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        প্রতি (পূর্ণাঙ্ক i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*std::sqrt(vlNorm);
        vlNorm = std::sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        প্রতি (পূর্ণাঙ্ক i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        প্রতি (পূর্ণাঙ্ক j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            প্রতি (পূর্ণাঙ্ক i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            প্রতি (পূর্ণাঙ্ক i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        প্রতি (পূর্ণাঙ্ক i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    যদি (b)
    {
        //generate new rhs
        প্রতি (পূর্ণাঙ্ক l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            প্রতি (পূর্ণাঙ্ক j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            প্রতি (পূর্ণাঙ্ক j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                প্রতি (পূর্ণাঙ্ক i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                প্রতি (পূর্ণাঙ্ক i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        প্রতি (পূর্ণাঙ্ক i = n - 1; i >= 0; i--)
        {
            প্রতি (পূর্ণাঙ্ক j = n - 1; j > i; j--)
            {
                প্রতি (পূর্ণাঙ্ক p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            যদি (std::abs(A[i*astep + i]) < eps)
                প্রত্যাগম 0;
            প্রতি (পূর্ণাঙ্ক p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    প্রত্যাগম 1;
}

পূর্ণাঙ্ক QR32f(ভিন্ন* A, size_t astep, পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক k, ভিন্ন* b, size_t bstep, ভিন্ন* hFactors)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক output;
    CALL_HAL_RET(QR32f, cv_hal_QR32f, output, A, astep, m, n, k, b, bstep, hFactors);
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, FLT_EPSILON * 10);
    প্রত্যাগম output;
}

পূর্ণাঙ্ক QR64f(দ্বিগুণ* A, size_t astep, পূর্ণাঙ্ক m, পূর্ণাঙ্ক n, পূর্ণাঙ্ক k, দ্বিগুণ* b, size_t bstep, দ্বিগুণ* hFactors)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক output;
    CALL_HAL_RET(QR64f, cv_hal_QR64f, output, A, astep, m, n, k, b, bstep, hFactors)
    output = QRImpl(A, astep, m, n, k, b, bstep, hFactors, DBL_EPSILON * 100);
    প্রত্যাগম output;
}

//=============================================================================
// for compatibility with 3.0

পূর্ণাঙ্ক LU(ভিন্ন* A, size_t astep, পূর্ণাঙ্ক m, ভিন্ন* b, size_t bstep, পূর্ণাঙ্ক n)
{
    প্রত্যাগম LUImpl(A, astep, m, b, bstep, n, FLT_EPSILON*10);
}

পূর্ণাঙ্ক LU(দ্বিগুণ* A, size_t astep, পূর্ণাঙ্ক m, দ্বিগুণ* b, size_t bstep, পূর্ণাঙ্ক n)
{
    প্রত্যাগম LUImpl(A, astep, m, b, bstep, n, DBL_EPSILON*100);
}

তার্কিক Cholesky(ভিন্ন* A, size_t astep, পূর্ণাঙ্ক m, ভিন্ন* b, size_t bstep, পূর্ণাঙ্ক n)
{
    প্রত্যাগম CholImpl(A, astep, m, b, bstep, n);
}

তার্কিক Cholesky(দ্বিগুণ* A, size_t astep, পূর্ণাঙ্ক m, দ্বিগুণ* b, size_t bstep, পূর্ণাঙ্ক n)
{
    প্রত্যাগম CholImpl(A, astep, m, b, bstep, n);
}

}} // cv::hal::
