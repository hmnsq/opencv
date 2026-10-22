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
#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଯଦି 0
#define dprintf(x) printf x
#define print_matrix(x) print(x)
#ଅନ୍ୟଥା
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

ନାମାକାଶ cv
{

ଶ୍ରେଣୀ DownhillSolverImpl CV_FINAL : ସର୍ବସାଧାରଣ DownhillSolver
{
ସର୍ବସାଧାରଣ:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        _step=Mat_<ଦ୍ୱିଗୁଣ>();
    }

    ଶୂନ୍ୟ getInitStep(OutputArray step) ଅଚଳ CV_OVERRIDE { _step.copyTo(step); }
    ଶୂନ୍ୟ setInitStep(ନିବେଶ_Array step) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = step.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.cols, m.rows));
        ଯଦି( m.rows == 1 )
            m.copyTo(_step);
        ଅନ୍ୟଥା
            transpose(m, _step);
    }

    Ptr<MinProblemSolver::Function> getFunction() ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ _Function; }

    ଶୂନ୍ୟ setFunction(ଅଚଳ Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    TermCriteria getTermCriteria() ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ _termcrit; }

    ଶୂନ୍ୟ setTermCriteria( ଅଚଳ TermCriteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (TermCriteria::MAX_ITER + TermCriteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_ଗଣନା > 0 );
        _termcrit=termcrit;
    }

    ଦ୍ୱିଗୁଣ minimize( ନିବେଶ_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(_step.cols, _step.rows) == 1 &&
                  std::max(_step.cols, _step.rows) >= 2 &&
                  _step.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_ଗଣନା,_termcrit.epsilon));
        dprintf(("step\n"));
        print_matrix(_step);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, _step);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = 0;
        ଦ୍ୱିଗୁଣ res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          ଗଣନା_3, _termcrit.max_ଗଣନା);
        dprintf(("%d iterations done\n",ଗଣନା_3));

        ଯଦି( !x.empty() )
        {
            Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<ଦ୍ୱିଗୁଣ>());
            simplex_0m.convertTo(x, x.type());
        }
        ଅନ୍ୟଥା
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.row(0).convertTo(x_, x_type);
        }
        ଫେରନ୍ତୁ res;
    }
ସୁରକ୍ଷିତ:
    Ptr<MinProblemSolver::Function> _Function;
    TermCriteria _termcrit;
    Mat _step;

    ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ updateCoordSum(ଅଚଳ Mat& p, Mat& coord_sum)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, m = p.rows, n = p.cols;
        ଦ୍ୱିଗୁଣ* coord_sum_ = coord_sum.ptr<ଦ୍ୱିଗୁଣ>();
        CV_Assert( coord_sum.cols == n && coord_sum.rows == 1 );

        ପାଇଁ( j = 0; j < n; j++ )
            coord_sum_[j] = 0.;

        ପାଇଁ( i = 0; i < m; i++ )
        {
            ଅଚଳ ଦ୍ୱିଗୁଣ* p_i = p.ptr<ଦ୍ୱିଗୁଣ>(i);
            ପାଇଁ( j = 0; j < n; j++ )
                coord_sum_[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_matrix(coord_sum);

    }

    ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ createInitialSimplex( ଅଚଳ Mat& x0, Mat& simplex, Mat& step )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, ndim = step.cols;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        ଯଦି( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.cols == 1 && x.rows == ndim) || (x.cols == ndim && x.rows == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<ଦ୍ୱିଗୁଣ>());

        x.convertTo(simplex_0m, CV_64F);
        ଦ୍ୱିଗୁଣ* simplex_0 = simplex.ptr<ଦ୍ୱିଗୁଣ>();
        ଅଚଳ ଦ୍ୱିଗୁଣ* step_ = step.ptr<ଦ୍ୱିଗୁଣ>();
        ପାଇଁ( i = 1; i <= ndim; i++ )
        {
            ଦ୍ୱିଗୁଣ* simplex_i = simplex.ptr<ଦ୍ୱିଗୁଣ>(i);
            ପାଇଁ( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*step_[i-1];
        }
        ପାଇଁ( j = 0; j < ndim; j++ )
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
    ଦ୍ୱିଗୁଣ innerDownhillSimplex( Mat& p, ଦ୍ୱିଗୁଣ MinRange, ଦ୍ୱିଗୁଣ MinError, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& fcount, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nmax )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, ndim = p.cols;
        Mat coord_sum(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        ଦ୍ୱିଗୁଣ* y_ = y.ptr<ଦ୍ୱିଗୁଣ>();

        fcount = ndim+1;
        ପାଇଁ( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<ଦ୍ୱିଗୁଣ>(i));

        updateCoordSum(p, coord_sum);

        ପାଇଁ (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ilo = 0, ihi, inhi;
            ଯଦି( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            ଅନ୍ୟଥା
            {
                ihi = 1; inhi = 0;
            }
            ପାଇଁ( i = 0; i <= ndim; i++ )
            {
                ଦ୍ୱିଗୁଣ yval = y_[i];
                ଯଦି (yval <= y_[ilo])
                    ilo = i;
                ଯଦି (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                ଅନ୍ୟଥା ଯଦି (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            ଯଦି( ilo == inhi || ilo == ihi )
            {
                ପାଇଁ( i = 0; i <= ndim; i++ )
                {
                    ଦ୍ୱିଗୁଣ yval = y_[i];
                    ଯଦି( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        ଭାଙ୍ଗ;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_matrix(y);

            // check stop criterion
            ଦ୍ୱିଗୁଣ error = fabs(y_[ihi] - y_[ilo]);
            ଦ୍ୱିଗୁଣ range = 0;
            ପାଇଁ( j = 0; j < ndim; j++ )
            {
                ଦ୍ୱିଗୁଣ minval, maxval;
                minval = maxval = p.at<ଦ୍ୱିଗୁଣ>(0, j);
                ପାଇଁ( i = 1; i <= ndim; i++ )
                {
                    ଦ୍ୱିଗୁଣ pval = p.at<ଦ୍ୱିଗୁଣ>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            ଯଦି( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                ପାଇଁ( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<ଦ୍ୱିଗୁଣ>(0, j), p.at<ଦ୍ୱିଗୁଣ>(ilo, j));
                }
                ଭାଙ୍ଗ;
            }

            ଦ୍ୱିଗୁଣ y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            ଦ୍ୱିଗୁଣ alpha = -1.0;
            ଦ୍ୱିଗୁଣ y_alpha = tryNewPoint(p, coord_sum, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_matrix(buf);

            ଯଦି( y_alpha < y_nhi )
            {
                ଯଦି( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    ଦ୍ୱିଗୁଣ beta = -2.0;
                    ଦ୍ୱିଗୁଣ y_beta = tryNewPoint(p, coord_sum, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_matrix(buf);
                    ଯଦି( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replacePoint(p, coord_sum, y, ihi, alpha, y_alpha);
            }
            ଅନ୍ୟଥା
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                ଦ୍ୱିଗୁଣ gamma = 0.5;
                ଦ୍ୱିଗୁଣ y_gamma = tryNewPoint(p, coord_sum, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_matrix(buf);
                ଯଦି( y_gamma < y_hi )
                    replacePoint(p, coord_sum, y, ihi, gamma, y_gamma);
                ଅନ୍ୟଥା
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    ପାଇଁ( i = 0; i <= ndim; i++ )
                    {
                        ଯଦି (i != ilo)
                        {
                            ପାଇଁ( j = 0; j < ndim; j++ )
                                p.at<ଦ୍ୱିଗୁଣ>(i, j) = 0.5*(p.at<ଦ୍ୱିଗୁଣ>(i, j) + p.at<ଦ୍ୱିଗୁଣ>(ilo, j));
                            y_[i] = calc_f(p.ptr<ଦ୍ୱିଗୁଣ>(i));
                        }
                    }
                    fcount += ndim;
                    updateCoordSum(p, coord_sum);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_matrix(p);
        }
        ଫେରନ୍ତୁ y_[0];
    }

    ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ calc_f(ଅଚଳ ଦ୍ୱିଗୁଣ* ptr)
    {
        ଦ୍ୱିଗୁଣ res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        ଫେରନ୍ତୁ res;
    }

    ଦ୍ୱିଗୁଣ tryNewPoint( Mat& p, Mat& coord_sum, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ihi, ଦ୍ୱିଗୁଣ alpha_, Mat& ptry, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& fcount )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, ndim = p.cols;

        ଦ୍ୱିଗୁଣ alpha = (1.0 - alpha_)/ndim;
        ଦ୍ୱିଗୁଣ beta = alpha - alpha_;
        ଦ୍ୱିଗୁଣ* p_ihi = p.ptr<ଦ୍ୱିଗୁଣ>(ihi);
        ଦ୍ୱିଗୁଣ* ptry_ = ptry.ptr<ଦ୍ୱିଗୁଣ>();
        ଦ୍ୱିଗୁଣ* coord_sum_ = coord_sum.ptr<ଦ୍ୱିଗୁଣ>();

        ପାଇଁ( j = 0; j < ndim; j++ )
            ptry_[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;

        fcount++;
        ଫେରନ୍ତୁ calc_f(ptry_);
    }

    ଶୂନ୍ୟ replacePoint( Mat& p, Mat& coord_sum, Mat& y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ihi, ଦ୍ୱିଗୁଣ alpha_, ଦ୍ୱିଗୁଣ ytry )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, ndim = p.cols;

        ଦ୍ୱିଗୁଣ alpha = (1.0 - alpha_)/ndim;
        ଦ୍ୱିଗୁଣ beta = alpha - alpha_;
        ଦ୍ୱିଗୁଣ* p_ihi = p.ptr<ଦ୍ୱିଗୁଣ>(ihi);
        ଦ୍ୱିଗୁଣ* coord_sum_ = coord_sum.ptr<ଦ୍ୱିଗୁଣ>();

        ପାଇଁ( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;
        y.at<ଦ୍ୱିଗୁଣ>(ihi) = ytry;
        updateCoordSum(p, coord_sum);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( ଅଚଳ Ptr<MinProblemSolver::Function>& f,
                                            ନିବେଶ_Array initStep, TermCriteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->setFunction(f);
    DS->setInitStep(initStep);
    DS->setTermCriteria(termcrit);
    ଫେରନ୍ତୁ DS;
}

}
