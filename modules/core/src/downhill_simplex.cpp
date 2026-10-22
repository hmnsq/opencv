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
#അന്തര്ഭാവ "precomp.hpp"

#എങ്കിൽ 0
#define dprintf(x) printf x
#define print_matrix(x) print(x)
#അല്ലെങ്കിൽ
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

നാമാകാശ cv
{

വർഗ്ഗം DownhillSolverImpl CV_FINAL : പൊതു DownhillSolver
{
പൊതു:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        _step=Mat_<ഇരട്ട>();
    }

    ശൂന്യം getInitStep(OutputArray step) സ്ഥിരം CV_OVERRIDE { _step.copyTo(step); }
    ശൂന്യം setInitStep(നിവേശം_Array step) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = step.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.cols, m.rows));
        എങ്കിൽ( m.rows == 1 )
            m.copyTo(_step);
        അല്ലെങ്കിൽ
            transpose(m, _step);
    }

    Ptr<MinProblemSolver::Function> getFunction() സ്ഥിരം CV_OVERRIDE { തിരികെ _Function; }

    ശൂന്യം setFunction(സ്ഥിരം Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    TermCriteria getTermCriteria() സ്ഥിരം CV_OVERRIDE { തിരികെ _termcrit; }

    ശൂന്യം setTermCriteria( സ്ഥിരം TermCriteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (TermCriteria::MAX_ITER + TermCriteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_എണ്ണം > 0 );
        _termcrit=termcrit;
    }

    ഇരട്ട minimize( നിവേശം_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(_step.cols, _step.rows) == 1 &&
                  std::max(_step.cols, _step.rows) >= 2 &&
                  _step.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_എണ്ണം,_termcrit.epsilon));
        dprintf(("step\n"));
        print_matrix(_step);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, _step);
        പൂർണ്ണസംഖ്യ എണ്ണം_2 = 0;
        ഇരട്ട res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          എണ്ണം_2, _termcrit.max_എണ്ണം);
        dprintf(("%d iterations done\n",എണ്ണം_2));

        എങ്കിൽ( !x.empty() )
        {
            Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<ഇരട്ട>());
            simplex_0m.convertTo(x, x.type());
        }
        അല്ലെങ്കിൽ
        {
            പൂർണ്ണസംഖ്യ x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.row(0).convertTo(x_, x_type);
        }
        തിരികെ res;
    }
സംരക്ഷിത:
    Ptr<MinProblemSolver::Function> _Function;
    TermCriteria _termcrit;
    Mat _step;

    വരിയിൽ ശൂന്യം updateCoordSum(സ്ഥിരം Mat& p, Mat& coord_sum)
    {
        പൂർണ്ണസംഖ്യ i, j, m = p.rows, n = p.cols;
        ഇരട്ട* coord_sum_ = coord_sum.ptr<ഇരട്ട>();
        CV_Assert( coord_sum.cols == n && coord_sum.rows == 1 );

        വേണ്ടി( j = 0; j < n; j++ )
            coord_sum_[j] = 0.;

        വേണ്ടി( i = 0; i < m; i++ )
        {
            സ്ഥിരം ഇരട്ട* p_i = p.ptr<ഇരട്ട>(i);
            വേണ്ടി( j = 0; j < n; j++ )
                coord_sum_[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_matrix(coord_sum);

    }

    വരിയിൽ ശൂന്യം createInitialSimplex( സ്ഥിരം Mat& x0, Mat& simplex, Mat& step )
    {
        പൂർണ്ണസംഖ്യ i, j, ndim = step.cols;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        എങ്കിൽ( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.cols == 1 && x.rows == ndim) || (x.cols == ndim && x.rows == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<ഇരട്ട>());

        x.convertTo(simplex_0m, CV_64F);
        ഇരട്ട* simplex_0 = simplex.ptr<ഇരട്ട>();
        സ്ഥിരം ഇരട്ട* step_ = step.ptr<ഇരട്ട>();
        വേണ്ടി( i = 1; i <= ndim; i++ )
        {
            ഇരട്ട* simplex_i = simplex.ptr<ഇരട്ട>(i);
            വേണ്ടി( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*step_[i-1];
        }
        വേണ്ടി( j = 0; j < ndim; j++ )
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
    ഇരട്ട innerDownhillSimplex( Mat& p, ഇരട്ട MinRange, ഇരട്ട MinError, പൂർണ്ണസംഖ്യ& fcount, പൂർണ്ണസംഖ്യ nmax )
    {
        പൂർണ്ണസംഖ്യ i, j, ndim = p.cols;
        Mat coord_sum(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        ഇരട്ട* y_ = y.ptr<ഇരട്ട>();

        fcount = ndim+1;
        വേണ്ടി( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<ഇരട്ട>(i));

        updateCoordSum(p, coord_sum);

        വേണ്ടി (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            പൂർണ്ണസംഖ്യ ilo = 0, ihi, inhi;
            എങ്കിൽ( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            അല്ലെങ്കിൽ
            {
                ihi = 1; inhi = 0;
            }
            വേണ്ടി( i = 0; i <= ndim; i++ )
            {
                ഇരട്ട yval = y_[i];
                എങ്കിൽ (yval <= y_[ilo])
                    ilo = i;
                എങ്കിൽ (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                അല്ലെങ്കിൽ എങ്കിൽ (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            എങ്കിൽ( ilo == inhi || ilo == ihi )
            {
                വേണ്ടി( i = 0; i <= ndim; i++ )
                {
                    ഇരട്ട yval = y_[i];
                    എങ്കിൽ( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        നിർത്തുക;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_matrix(y);

            // check stop criterion
            ഇരട്ട error = fabs(y_[ihi] - y_[ilo]);
            ഇരട്ട range = 0;
            വേണ്ടി( j = 0; j < ndim; j++ )
            {
                ഇരട്ട minval, maxval;
                minval = maxval = p.at<ഇരട്ട>(0, j);
                വേണ്ടി( i = 1; i <= ndim; i++ )
                {
                    ഇരട്ട pval = p.at<ഇരട്ട>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            എങ്കിൽ( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                വേണ്ടി( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<ഇരട്ട>(0, j), p.at<ഇരട്ട>(ilo, j));
                }
                നിർത്തുക;
            }

            ഇരട്ട y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            ഇരട്ട alpha = -1.0;
            ഇരട്ട y_alpha = tryNewPoint(p, coord_sum, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_matrix(buf);

            എങ്കിൽ( y_alpha < y_nhi )
            {
                എങ്കിൽ( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    ഇരട്ട beta = -2.0;
                    ഇരട്ട y_beta = tryNewPoint(p, coord_sum, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_matrix(buf);
                    എങ്കിൽ( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replacePoint(p, coord_sum, y, ihi, alpha, y_alpha);
            }
            അല്ലെങ്കിൽ
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                ഇരട്ട gamma = 0.5;
                ഇരട്ട y_gamma = tryNewPoint(p, coord_sum, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_matrix(buf);
                എങ്കിൽ( y_gamma < y_hi )
                    replacePoint(p, coord_sum, y, ihi, gamma, y_gamma);
                അല്ലെങ്കിൽ
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    വേണ്ടി( i = 0; i <= ndim; i++ )
                    {
                        എങ്കിൽ (i != ilo)
                        {
                            വേണ്ടി( j = 0; j < ndim; j++ )
                                p.at<ഇരട്ട>(i, j) = 0.5*(p.at<ഇരട്ട>(i, j) + p.at<ഇരട്ട>(ilo, j));
                            y_[i] = calc_f(p.ptr<ഇരട്ട>(i));
                        }
                    }
                    fcount += ndim;
                    updateCoordSum(p, coord_sum);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_matrix(p);
        }
        തിരികെ y_[0];
    }

    വരിയിൽ ഇരട്ട calc_f(സ്ഥിരം ഇരട്ട* ptr)
    {
        ഇരട്ട res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        തിരികെ res;
    }

    ഇരട്ട tryNewPoint( Mat& p, Mat& coord_sum, പൂർണ്ണസംഖ്യ ihi, ഇരട്ട alpha_, Mat& ptry, പൂർണ്ണസംഖ്യ& fcount )
    {
        പൂർണ്ണസംഖ്യ j, ndim = p.cols;

        ഇരട്ട alpha = (1.0 - alpha_)/ndim;
        ഇരട്ട beta = alpha - alpha_;
        ഇരട്ട* p_ihi = p.ptr<ഇരട്ട>(ihi);
        ഇരട്ട* ptry_ = ptry.ptr<ഇരട്ട>();
        ഇരട്ട* coord_sum_ = coord_sum.ptr<ഇരട്ട>();

        വേണ്ടി( j = 0; j < ndim; j++ )
            ptry_[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;

        fcount++;
        തിരികെ calc_f(ptry_);
    }

    ശൂന്യം replacePoint( Mat& p, Mat& coord_sum, Mat& y, പൂർണ്ണസംഖ്യ ihi, ഇരട്ട alpha_, ഇരട്ട ytry )
    {
        പൂർണ്ണസംഖ്യ j, ndim = p.cols;

        ഇരട്ട alpha = (1.0 - alpha_)/ndim;
        ഇരട്ട beta = alpha - alpha_;
        ഇരട്ട* p_ihi = p.ptr<ഇരട്ട>(ihi);
        ഇരട്ട* coord_sum_ = coord_sum.ptr<ഇരട്ട>();

        വേണ്ടി( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;
        y.at<ഇരട്ട>(ihi) = ytry;
        updateCoordSum(p, coord_sum);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( സ്ഥിരം Ptr<MinProblemSolver::Function>& f,
                                            നിവേശം_Array initStep, TermCriteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->setFunction(f);
    DS->setInitStep(initStep);
    DS->setTermCriteria(termcrit);
    തിരികെ DS;
}

}
