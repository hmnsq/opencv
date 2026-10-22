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
#define dprintf(x) printf x
#define print_matrix(x) print(x)
#وگرنه
#define dprintf(x)
#define print_matrix(x)
#endif

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

فضاینام cv
{

رده DownhillSolverImpl CV_FINAL : عمومی DownhillSolver
{
عمومی:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        _step=Mat_<دوگانه>();
    }

    تهی getInitStep(OutputArray step) ثابت CV_OVERRIDE { _step.copyTo(step); }
    تهی setInitStep(درون_داد_Array step) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = step.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.cols, m.rows));
        اگر( m.rows == 1 )
            m.copyTo(_step);
        وگرنه
            transpose(m, _step);
    }

    Ptr<MinProblemSolver::Function> getFunction() ثابت CV_OVERRIDE { بازگشت _Function; }

    تهی setFunction(ثابت Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    TermCriteria getTermCriteria() ثابت CV_OVERRIDE { بازگشت _termcrit; }

    تهی setTermCriteria( ثابت TermCriteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (TermCriteria::MAX_ITER + TermCriteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_شمار > 0 );
        _termcrit=termcrit;
    }

    دوگانه minimize( درون_داد_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(_step.cols, _step.rows) == 1 &&
                  std::max(_step.cols, _step.rows) >= 2 &&
                  _step.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_شمار,_termcrit.epsilon));
        dprintf(("step\n"));
        print_matrix(_step);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, _step);
        صحیح شمار_2 = 0;
        دوگانه res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          شمار_2, _termcrit.max_شمار);
        dprintf(("%d iterations done\n",شمار_2));

        اگر( !x.empty() )
        {
            Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<دوگانه>());
            simplex_0m.convertTo(x, x.type());
        }
        وگرنه
        {
            صحیح x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.row(0).convertTo(x_, x_type);
        }
        بازگشت res;
    }
محافظتشده:
    Ptr<MinProblemSolver::Function> _Function;
    TermCriteria _termcrit;
    Mat _step;

    درونخطی تهی updateCoordSum(ثابت Mat& p, Mat& coord_sum)
    {
        صحیح i, j, m = p.rows, n = p.cols;
        دوگانه* coord_sum_ = coord_sum.ptr<دوگانه>();
        CV_Assert( coord_sum.cols == n && coord_sum.rows == 1 );

        برای( j = 0; j < n; j++ )
            coord_sum_[j] = 0.;

        برای( i = 0; i < m; i++ )
        {
            ثابت دوگانه* p_i = p.ptr<دوگانه>(i);
            برای( j = 0; j < n; j++ )
                coord_sum_[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_matrix(coord_sum);

    }

    درونخطی تهی createInitialSimplex( ثابت Mat& x0, Mat& simplex, Mat& step )
    {
        صحیح i, j, ndim = step.cols;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        اگر( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.cols == 1 && x.rows == ndim) || (x.cols == ndim && x.rows == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<دوگانه>());

        x.convertTo(simplex_0m, CV_64F);
        دوگانه* simplex_0 = simplex.ptr<دوگانه>();
        ثابت دوگانه* step_ = step.ptr<دوگانه>();
        برای( i = 1; i <= ndim; i++ )
        {
            دوگانه* simplex_i = simplex.ptr<دوگانه>(i);
            برای( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*step_[i-1];
        }
        برای( j = 0; j < ndim; j++ )
            simplex_0[j] -= 0.5*step_[j];

        dprintf(("\nthis is simplex\n"));
        print_matrix(simplex);
    }

    /*
     Performs the actual minimization of MinProblemSolver::Function f (after the initialization was done)

     The matrix p[ndim+1][1..ndim] represents ndim+1 vertices that
     form a simplex - each row is an ndim vector.
     On output, fcount gives the number of function evaluations taken.
    */
    دوگانه innerDownhillSimplex( Mat& p, دوگانه MinRange, دوگانه MinError, صحیح& fcount, صحیح nmax )
    {
        صحیح i, j, ndim = p.cols;
        Mat coord_sum(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        دوگانه* y_ = y.ptr<دوگانه>();

        fcount = ndim+1;
        برای( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<دوگانه>(i));

        updateCoordSum(p, coord_sum);

        برای (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            صحیح ilo = 0, ihi, inhi;
            اگر( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            وگرنه
            {
                ihi = 1; inhi = 0;
            }
            برای( i = 0; i <= ndim; i++ )
            {
                دوگانه yval = y_[i];
                اگر (yval <= y_[ilo])
                    ilo = i;
                اگر (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                وگرنه اگر (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            اگر( ilo == inhi || ilo == ihi )
            {
                برای( i = 0; i <= ndim; i++ )
                {
                    دوگانه yval = y_[i];
                    اگر( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        بشکن;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_matrix(y);

            // check stop criterion
            دوگانه error = fabs(y_[ihi] - y_[ilo]);
            دوگانه range = 0;
            برای( j = 0; j < ndim; j++ )
            {
                دوگانه minval, maxval;
                minval = maxval = p.at<دوگانه>(0, j);
                برای( i = 1; i <= ndim; i++ )
                {
                    دوگانه pval = p.at<دوگانه>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            اگر( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                برای( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<دوگانه>(0, j), p.at<دوگانه>(ilo, j));
                }
                بشکن;
            }

            دوگانه y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            دوگانه alpha = -1.0;
            دوگانه y_alpha = tryNewPoint(p, coord_sum, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_matrix(buf);

            اگر( y_alpha < y_nhi )
            {
                اگر( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    دوگانه beta = -2.0;
                    دوگانه y_beta = tryNewPoint(p, coord_sum, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_matrix(buf);
                    اگر( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replacePoint(p, coord_sum, y, ihi, alpha, y_alpha);
            }
            وگرنه
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                دوگانه gamma = 0.5;
                دوگانه y_gamma = tryNewPoint(p, coord_sum, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_matrix(buf);
                اگر( y_gamma < y_hi )
                    replacePoint(p, coord_sum, y, ihi, gamma, y_gamma);
                وگرنه
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    برای( i = 0; i <= ndim; i++ )
                    {
                        اگر (i != ilo)
                        {
                            برای( j = 0; j < ndim; j++ )
                                p.at<دوگانه>(i, j) = 0.5*(p.at<دوگانه>(i, j) + p.at<دوگانه>(ilo, j));
                            y_[i] = calc_f(p.ptr<دوگانه>(i));
                        }
                    }
                    fcount += ndim;
                    updateCoordSum(p, coord_sum);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_matrix(p);
        }
        بازگشت y_[0];
    }

    درونخطی دوگانه calc_f(ثابت دوگانه* ptr)
    {
        دوگانه res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        بازگشت res;
    }

    دوگانه tryNewPoint( Mat& p, Mat& coord_sum, صحیح ihi, دوگانه alpha_, Mat& ptry, صحیح& fcount )
    {
        صحیح j, ndim = p.cols;

        دوگانه alpha = (1.0 - alpha_)/ndim;
        دوگانه beta = alpha - alpha_;
        دوگانه* p_ihi = p.ptr<دوگانه>(ihi);
        دوگانه* ptry_ = ptry.ptr<دوگانه>();
        دوگانه* coord_sum_ = coord_sum.ptr<دوگانه>();

        برای( j = 0; j < ndim; j++ )
            ptry_[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;

        fcount++;
        بازگشت calc_f(ptry_);
    }

    تهی replacePoint( Mat& p, Mat& coord_sum, Mat& y, صحیح ihi, دوگانه alpha_, دوگانه ytry )
    {
        صحیح j, ndim = p.cols;

        دوگانه alpha = (1.0 - alpha_)/ndim;
        دوگانه beta = alpha - alpha_;
        دوگانه* p_ihi = p.ptr<دوگانه>(ihi);
        دوگانه* coord_sum_ = coord_sum.ptr<دوگانه>();

        برای( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;
        y.at<دوگانه>(ihi) = ytry;
        updateCoordSum(p, coord_sum);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( ثابت Ptr<MinProblemSolver::Function>& f,
                                            درون_داد_Array initStep, TermCriteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->setFunction(f);
    DS->setInitStep(initStep);
    DS->setTermCriteria(termcrit);
    بازگشت DS;
}

}
