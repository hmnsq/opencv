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
#include "precomp.hpp"

#만약 0
#define dprintf(x) printf x
#define print_matrix(x) print(x)
#아니면
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

namespace cv
{

class DownhillSolverImpl CV_FINAL : public DownhillSolver
{
public:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        _step=Mat_<double>();
    }

    void getInitStep(OutputArray step) const CV_OVERRIDE { _step.copyTo(step); }
    void setInitStep(InputArray step) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = step.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.cols, m.rows));
        만약( m.rows == 1 )
            m.copyTo(_step);
        아니면
            transpose(m, _step);
    }

    Ptr<MinProblemSolver::Function> getFunction() const CV_OVERRIDE { 반환 _Function; }

    void setFunction(const Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    TermCriteria getTermCriteria() const CV_OVERRIDE { 반환 _termcrit; }

    void setTermCriteria( const TermCriteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (TermCriteria::MAX_ITER + TermCriteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.maxCount > 0 );
        _termcrit=termcrit;
    }

    double minimize( InputOutputArray x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(_step.cols, _step.rows) == 1 &&
                  std::max(_step.cols, _step.rows) >= 2 &&
                  _step.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.maxCount,_termcrit.epsilon));
        dprintf(("step\n"));
        print_matrix(_step);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, _step);
        정수 count = 0;
        double res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          count, _termcrit.maxCount);
        dprintf(("%d iterations done\n",count));

        만약( !x.empty() )
        {
            Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<double>());
            simplex_0m.convertTo(x, x.type());
        }
        아니면
        {
            정수 x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.row(0).convertTo(x_, x_type);
        }
        반환 res;
    }
protected:
    Ptr<MinProblemSolver::Function> _Function;
    TermCriteria _termcrit;
    Mat _step;

    inline void updateCoordSum(const Mat& p, Mat& coord_sum)
    {
        정수 i, j, m = p.rows, n = p.cols;
        double* coord_sum_ = coord_sum.ptr<double>();
        CV_Assert( coord_sum.cols == n && coord_sum.rows == 1 );

        위해( j = 0; j < n; j++ )
            coord_sum_[j] = 0.;

        위해( i = 0; i < m; i++ )
        {
            const double* p_i = p.ptr<double>(i);
            위해( j = 0; j < n; j++ )
                coord_sum_[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_matrix(coord_sum);

    }

    inline void createInitialSimplex( const Mat& x0, Mat& simplex, Mat& step )
    {
        정수 i, j, ndim = step.cols;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        만약( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.cols == 1 && x.rows == ndim) || (x.cols == ndim && x.rows == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<double>());

        x.convertTo(simplex_0m, CV_64F);
        double* simplex_0 = simplex.ptr<double>();
        const double* step_ = step.ptr<double>();
        위해( i = 1; i <= ndim; i++ )
        {
            double* simplex_i = simplex.ptr<double>(i);
            위해( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*step_[i-1];
        }
        위해( j = 0; j < ndim; j++ )
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
    double innerDownhillSimplex( Mat& p, double MinRange, double MinError, 정수& fcount, 정수 nmax )
    {
        정수 i, j, ndim = p.cols;
        Mat coord_sum(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        double* y_ = y.ptr<double>();

        fcount = ndim+1;
        위해( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<double>(i));

        updateCoordSum(p, coord_sum);

        위해 (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            정수 ilo = 0, ihi, inhi;
            만약( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            아니면
            {
                ihi = 1; inhi = 0;
            }
            위해( i = 0; i <= ndim; i++ )
            {
                double yval = y_[i];
                만약 (yval <= y_[ilo])
                    ilo = i;
                만약 (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                아니면 만약 (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            만약( ilo == inhi || ilo == ihi )
            {
                위해( i = 0; i <= ndim; i++ )
                {
                    double yval = y_[i];
                    만약( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        break;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_matrix(y);

            // check stop criterion
            double error = fabs(y_[ihi] - y_[ilo]);
            double range = 0;
            위해( j = 0; j < ndim; j++ )
            {
                double minval, maxval;
                minval = maxval = p.at<double>(0, j);
                위해( i = 1; i <= ndim; i++ )
                {
                    double pval = p.at<double>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            만약( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                위해( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<double>(0, j), p.at<double>(ilo, j));
                }
                break;
            }

            double y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            double alpha = -1.0;
            double y_alpha = tryNewPoint(p, coord_sum, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_matrix(buf);

            만약( y_alpha < y_nhi )
            {
                만약( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    double beta = -2.0;
                    double y_beta = tryNewPoint(p, coord_sum, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_matrix(buf);
                    만약( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replacePoint(p, coord_sum, y, ihi, alpha, y_alpha);
            }
            아니면
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                double gamma = 0.5;
                double y_gamma = tryNewPoint(p, coord_sum, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_matrix(buf);
                만약( y_gamma < y_hi )
                    replacePoint(p, coord_sum, y, ihi, gamma, y_gamma);
                아니면
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    위해( i = 0; i <= ndim; i++ )
                    {
                        만약 (i != ilo)
                        {
                            위해( j = 0; j < ndim; j++ )
                                p.at<double>(i, j) = 0.5*(p.at<double>(i, j) + p.at<double>(ilo, j));
                            y_[i] = calc_f(p.ptr<double>(i));
                        }
                    }
                    fcount += ndim;
                    updateCoordSum(p, coord_sum);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_matrix(p);
        }
        반환 y_[0];
    }

    inline double calc_f(const double* ptr)
    {
        double res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        반환 res;
    }

    double tryNewPoint( Mat& p, Mat& coord_sum, 정수 ihi, double alpha_, Mat& ptry, 정수& fcount )
    {
        정수 j, ndim = p.cols;

        double alpha = (1.0 - alpha_)/ndim;
        double beta = alpha - alpha_;
        double* p_ihi = p.ptr<double>(ihi);
        double* ptry_ = ptry.ptr<double>();
        double* coord_sum_ = coord_sum.ptr<double>();

        위해( j = 0; j < ndim; j++ )
            ptry_[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;

        fcount++;
        반환 calc_f(ptry_);
    }

    void replacePoint( Mat& p, Mat& coord_sum, Mat& y, 정수 ihi, double alpha_, double ytry )
    {
        정수 j, ndim = p.cols;

        double alpha = (1.0 - alpha_)/ndim;
        double beta = alpha - alpha_;
        double* p_ihi = p.ptr<double>(ihi);
        double* coord_sum_ = coord_sum.ptr<double>();

        위해( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;
        y.at<double>(ihi) = ytry;
        updateCoordSum(p, coord_sum);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( const Ptr<MinProblemSolver::Function>& f,
                                            InputArray initStep, TermCriteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->setFunction(f);
    DS->setInitStep(initStep);
    DS->setTermCriteria(termcrit);
    반환 DS;
}

}
