/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the OpenCV Foundation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#شامل "precomp.hpp"

#اگر 0
#تعریف_کرو dprintf(x) لکھو x
#تعریف_کرو print_میٹرکس(x) print(x)
#ورنہ
#تعریف_کرو dprintf(x)
#تعریف_کرو print_میٹرکس(x)
#اگر_ختم

/*

****Error Message********************************************************************************************************************

Downhill Simplex method in OpenCV dev 3.0.0 getting this error:

OpenCV Error: Assertion failed (dims <= 2 && data && (unsigned)i0 < (unsigned)(s ize.p[0] * size.p[1])
&& elemSize() == (((((DataType<_Tp>::type) & ((512 - 1) << 3)) >> 3) + 1) << ((((sizeof(size_t)/4+1)16384|0x3a50)
>> ((DataType<_Tp>::typ e) & ((1 << 3) - 1))2) & 3))) in Mat::at,
file C:\builds\master_PackSlave-w in32-vc12-shared\opencv\modules\core\include\opencv2/core/mat.inl.hpp, line 893

****Problem and Possible Fix*********************************************************************************************************

DownhillSolverImpl::innerDownhillSimplex something looks broken here:
Mat_<double> coord_sum(1,ndim,0.0),buf(1,ndim,0.0),y(1,ndim,0.0);
fcount = 0;
for(i=0;i<ndim+1;++i)
{
y(i) = f->calc(p[i]);
}

y has only ndim elements, while the loop goes over ndim+1

Edited the following for possible fix:

Replaced y(1,ndim,0.0) ------> y(1,ndim+1,0.0)

***********************************************************************************************************************************

The code below was used in testing the source code.
Created by @SareeAlnaghy

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <opencv2\optim\optim.hpp>

using namespace std;
using namespace cv;

void test(Ptr<optim::DownhillSolver> MinProblemSolver, Ptr<optim::MinProblemSolver::Function> ptr_F, Mat &P, Mat &step)
{
try{

MinProblemSolver->setFunction(ptr_F);
MinProblemSolver->setInitStep(step);
double res = MinProblemSolver->minimize(P);

cout << "res " << res << endl;
}
catch (exception e)
{
cerr << "Error:: " << e.what() << endl;
}
}

int main()
{

class DistanceToLines :public optim::MinProblemSolver::Function {
public:
double calc(const double* x)const{

return x[0] * x[0] + x[1] * x[1];

}
};

Mat P = Mat_<double>({1, 2}, {1.0, 1.0});
Mat step = Mat_<double>({2, 1}, {-0.5, 0.5});

Ptr<optim::MinProblemSolver::Function> ptr_F(new DistanceToLines());
Ptr<optim::DownhillSolver> MinProblemSolver = optim::createDownhillSolver();

test(MinProblemSolver, ptr_F, P, step);

system("pause");
return 0;
}

****Suggestion for improving Simplex implementation***************************************************************************************

Currently the downhilll simplex method outputs the function value that is minimized. It should also return the coordinate points where the
function is minimized. This is very useful in many applications such as using back projection methods to find a point of intersection of
multiple lines in three dimensions as not all lines intersect in three dimensions.

*/

نامخلا cv
{

جماعت DownhillSolverImpl CV_FINAL : عوامی DownhillSolver
{
عوامی:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        قدم_3=Mat_<دوہرا>();
    }

    خالی get_Init_قدم(OutputArray قدم) ثابت CV_OVERRIDE { قدم_3.copy_مقصد(قدم); }
    خالی سیٹ_Init_قدم(داخلہ_Array قدم) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = قدم.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.ستون_تعداد, m.صفیں));
        اگر( m.صفیں == 1 )
            m.copy_مقصد(قدم_3);
        ورنہ
            transpose(m, قدم_3);
    }

    Ptr<MinProblemSolver::Function> getFunction() ثابت CV_OVERRIDE { واپس _Function; }

    خالی سیٹ_Function(ثابت Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    رکن_Criteria get_رکن_Criteria() ثابت CV_OVERRIDE { واپس _termcrit; }

    خالی سیٹ_رکن_Criteria( ثابت رکن_Criteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (رکن_Criteria::MAX_ITER + رکن_Criteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_گنتی > 0 );
        _termcrit=termcrit;
    }

    دوہرا minimize( داخلہ_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(قدم_3.ستون_تعداد, قدم_3.صفیں) == 1 &&
                  std::max(قدم_3.ستون_تعداد, قدم_3.صفیں) >= 2 &&
                  قدم_3.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_گنتی,_termcrit.epsilon));
        dprintf(("step\n"));
        print_میٹرکس(قدم_3);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, قدم_3);
        صحیح گنتی_2 = 0;
        دوہرا res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          گنتی_2, _termcrit.max_گنتی);
        dprintf(("%d iterations done\n",گنتی_2));

        اگر( !x.empty() )
        {
            Mat simplex_0m(x.صفیں, x.ستون_تعداد, CV_64F, simplex.ptr<دوہرا>());
            simplex_0m.convert_مقصد(x, x.type());
        }
        ورنہ
        {
            صحیح x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.صف(0).convert_مقصد(x_, x_type);
        }
        واپس res;
    }
محفوظ:
    Ptr<MinProblemSolver::Function> _Function;
    رکن_Criteria _termcrit;
    Mat قدم_3;

    اندرونی خالی update_Coord_مجموع(ثابت Mat& p, Mat& coord_مجموع)
    {
        صحیح i, j, m = p.صفیں, n = p.ستون_تعداد;
        دوہرا* coord_مجموع_2 = coord_مجموع.ptr<دوہرا>();
        CV_Assert( coord_مجموع.ستون_تعداد == n && coord_مجموع.صفیں == 1 );

        برائے( j = 0; j < n; j++ )
            coord_مجموع_2[j] = 0.;

        برائے( i = 0; i < m; i++ )
        {
            ثابت دوہرا* p_i = p.ptr<دوہرا>(i);
            برائے( j = 0; j < n; j++ )
                coord_مجموع_2[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_میٹرکس(coord_مجموع);

    }

    اندرونی خالی createInitialSimplex( ثابت Mat& x0, Mat& simplex, Mat& قدم )
    {
        صحیح i, j, ndim = قدم.ستون_تعداد;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        اگر( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.ستون_تعداد == 1 && x.صفیں == ndim) || (x.ستون_تعداد == ndim && x.صفیں == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.صفیں, x.ستون_تعداد, CV_64F, simplex.ptr<دوہرا>());

        x.convert_مقصد(simplex_0m, CV_64F);
        دوہرا* simplex_0 = simplex.ptr<دوہرا>();
        ثابت دوہرا* قدم_2 = قدم.ptr<دوہرا>();
        برائے( i = 1; i <= ndim; i++ )
        {
            دوہرا* simplex_i = simplex.ptr<دوہرا>(i);
            برائے( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*قدم_2[i-1];
        }
        برائے( j = 0; j < ndim; j++ )
            simplex_0[j] -= 0.5*قدم_2[j];

        dprintf(("\nthis is simplex\n"));
        print_میٹرکس(simplex);
    }

    /*
     Performs the actual minimization of MinProblemSolver::Function f (after the initialization was done)

     The matrix p[ndim+1][1..ndim] represents ndim+1 vertices that
     form a simplex - each row is an ndim vector.
     On output, fcount gives the number of function evaluations taken.
    */
    دوہرا innerDownhillSimplex( Mat& p, دوہرا MinRange, دوہرا MinError, صحیح& fcount, صحیح nmax )
    {
        صحیح i, j, ndim = p.ستون_تعداد;
        Mat coord_مجموع(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        دوہرا* y_ = y.ptr<دوہرا>();

        fcount = ndim+1;
        برائے( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<دوہرا>(i));

        update_Coord_مجموع(p, coord_مجموع);

        برائے (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            صحیح ilo = 0, ihi, inhi;
            اگر( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            ورنہ
            {
                ihi = 1; inhi = 0;
            }
            برائے( i = 0; i <= ndim; i++ )
            {
                دوہرا yval = y_[i];
                اگر (yval <= y_[ilo])
                    ilo = i;
                اگر (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                ورنہ اگر (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            اگر( ilo == inhi || ilo == ihi )
            {
                برائے( i = 0; i <= ndim; i++ )
                {
                    دوہرا yval = y_[i];
                    اگر( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        توڑو;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_میٹرکس(y);

            // check stop criterion
            دوہرا error = fabs(y_[ihi] - y_[ilo]);
            دوہرا range = 0;
            برائے( j = 0; j < ndim; j++ )
            {
                دوہرا minval, maxval;
                minval = maxval = p.at<دوہرا>(0, j);
                برائے( i = 1; i <= ndim; i++ )
                {
                    دوہرا pval = p.at<دوہرا>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            اگر( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                برائے( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<دوہرا>(0, j), p.at<دوہرا>(ilo, j));
                }
                توڑو;
            }

            دوہرا y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            دوہرا alpha = -1.0;
            دوہرا y_alpha = try_New_نقطہ(p, coord_مجموع, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_میٹرکس(buf);

            اگر( y_alpha < y_nhi )
            {
                اگر( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    دوہرا beta = -2.0;
                    دوہرا y_beta = try_New_نقطہ(p, coord_مجموع, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_میٹرکس(buf);
                    اگر( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replace_نقطہ(p, coord_مجموع, y, ihi, alpha, y_alpha);
            }
            ورنہ
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                دوہرا gamma = 0.5;
                دوہرا y_gamma = try_New_نقطہ(p, coord_مجموع, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_میٹرکس(buf);
                اگر( y_gamma < y_hi )
                    replace_نقطہ(p, coord_مجموع, y, ihi, gamma, y_gamma);
                ورنہ
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    برائے( i = 0; i <= ndim; i++ )
                    {
                        اگر (i != ilo)
                        {
                            برائے( j = 0; j < ndim; j++ )
                                p.at<دوہرا>(i, j) = 0.5*(p.at<دوہرا>(i, j) + p.at<دوہرا>(ilo, j));
                            y_[i] = calc_f(p.ptr<دوہرا>(i));
                        }
                    }
                    fcount += ndim;
                    update_Coord_مجموع(p, coord_مجموع);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_میٹرکس(p);
        }
        واپس y_[0];
    }

    اندرونی دوہرا calc_f(ثابت دوہرا* ptr)
    {
        دوہرا res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        واپس res;
    }

    دوہرا try_New_نقطہ( Mat& p, Mat& coord_مجموع, صحیح ihi, دوہرا alpha_, Mat& ptry, صحیح& fcount )
    {
        صحیح j, ndim = p.ستون_تعداد;

        دوہرا alpha = (1.0 - alpha_)/ndim;
        دوہرا beta = alpha - alpha_;
        دوہرا* p_ihi = p.ptr<دوہرا>(ihi);
        دوہرا* ptry_ = ptry.ptr<دوہرا>();
        دوہرا* coord_مجموع_2 = coord_مجموع.ptr<دوہرا>();

        برائے( j = 0; j < ndim; j++ )
            ptry_[j] = coord_مجموع_2[j]*alpha - p_ihi[j]*beta;

        fcount++;
        واپس calc_f(ptry_);
    }

    خالی replace_نقطہ( Mat& p, Mat& coord_مجموع, Mat& y, صحیح ihi, دوہرا alpha_, دوہرا ytry )
    {
        صحیح j, ndim = p.ستون_تعداد;

        دوہرا alpha = (1.0 - alpha_)/ndim;
        دوہرا beta = alpha - alpha_;
        دوہرا* p_ihi = p.ptr<دوہرا>(ihi);
        دوہرا* coord_مجموع_2 = coord_مجموع.ptr<دوہرا>();

        برائے( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_مجموع_2[j]*alpha - p_ihi[j]*beta;
        y.at<دوہرا>(ihi) = ytry;
        update_Coord_مجموع(p, coord_مجموع);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( ثابت Ptr<MinProblemSolver::Function>& f,
                                            داخلہ_Array init_قدم, رکن_Criteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->سیٹ_Function(f);
    DS->سیٹ_Init_قدم(init_قدم);
    DS->سیٹ_رکن_Criteria(termcrit);
    واپس DS;
}

}
