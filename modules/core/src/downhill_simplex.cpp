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
#অন্তর্ভাব "precomp.hpp"

#যদি 0
#define dprintf(x) printf x
#define print_matrix(x) print(x)
#অন্যথা
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

নামাকাশ cv
{

শ্রেণী DownhillSolverImpl CV_FINAL : সর্বজনীন DownhillSolver
{
সর্বজনীন:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        _step=Mat_<দ্বিগুণ>();
    }

    শূন্য getInitStep(OutputArray step) ধ্রুবক CV_OVERRIDE { _step.copyTo(step); }
    শূন্য setInitStep(প্রবেশ_Array step) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = step.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.cols, m.rows));
        যদি( m.rows == 1 )
            m.copyTo(_step);
        অন্যথা
            transpose(m, _step);
    }

    Ptr<MinProblemSolver::Function> getFunction() ধ্রুবক CV_OVERRIDE { ফেরত _Function; }

    শূন্য setFunction(ধ্রুবক Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    TermCriteria getTermCriteria() ধ্রুবক CV_OVERRIDE { ফেরত _termcrit; }

    শূন্য setTermCriteria( ধ্রুবক TermCriteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (TermCriteria::MAX_ITER + TermCriteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_গুনতি > 0 );
        _termcrit=termcrit;
    }

    দ্বিগুণ minimize( প্রবেশ_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(_step.cols, _step.rows) == 1 &&
                  std::max(_step.cols, _step.rows) >= 2 &&
                  _step.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_গুনতি,_termcrit.epsilon));
        dprintf(("step\n"));
        print_matrix(_step);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, _step);
        পূর্ণসংখ্যা গুনতি_2 = 0;
        দ্বিগুণ res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          গুনতি_2, _termcrit.max_গুনতি);
        dprintf(("%d iterations done\n",গুনতি_2));

        যদি( !x.empty() )
        {
            Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<দ্বিগুণ>());
            simplex_0m.convertTo(x, x.type());
        }
        অন্যথা
        {
            পূর্ণসংখ্যা x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.row(0).convertTo(x_, x_type);
        }
        ফেরত res;
    }
সুরক্ষিত:
    Ptr<MinProblemSolver::Function> _Function;
    TermCriteria _termcrit;
    Mat _step;

    অন্তঃরেখ শূন্য update_Coord_যোগ(ধ্রুবক Mat& p, Mat& coord_যোগ)
    {
        পূর্ণসংখ্যা i, j, m = p.rows, n = p.cols;
        দ্বিগুণ* coord_যোগ_2 = coord_যোগ.ptr<দ্বিগুণ>();
        CV_Assert( coord_যোগ.cols == n && coord_যোগ.rows == 1 );

        জন্য( j = 0; j < n; j++ )
            coord_যোগ_2[j] = 0.;

        জন্য( i = 0; i < m; i++ )
        {
            ধ্রুবক দ্বিগুণ* p_i = p.ptr<দ্বিগুণ>(i);
            জন্য( j = 0; j < n; j++ )
                coord_যোগ_2[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_matrix(coord_যোগ);

    }

    অন্তঃরেখ শূন্য createInitialSimplex( ধ্রুবক Mat& x0, Mat& simplex, Mat& step )
    {
        পূর্ণসংখ্যা i, j, ndim = step.cols;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        যদি( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.cols == 1 && x.rows == ndim) || (x.cols == ndim && x.rows == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<দ্বিগুণ>());

        x.convertTo(simplex_0m, CV_64F);
        দ্বিগুণ* simplex_0 = simplex.ptr<দ্বিগুণ>();
        ধ্রুবক দ্বিগুণ* step_ = step.ptr<দ্বিগুণ>();
        জন্য( i = 1; i <= ndim; i++ )
        {
            দ্বিগুণ* simplex_i = simplex.ptr<দ্বিগুণ>(i);
            জন্য( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*step_[i-1];
        }
        জন্য( j = 0; j < ndim; j++ )
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
    দ্বিগুণ innerDownhillSimplex( Mat& p, দ্বিগুণ MinRange, দ্বিগুণ MinError, পূর্ণসংখ্যা& fcount, পূর্ণসংখ্যা nmax )
    {
        পূর্ণসংখ্যা i, j, ndim = p.cols;
        Mat coord_যোগ(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        দ্বিগুণ* y_ = y.ptr<দ্বিগুণ>();

        fcount = ndim+1;
        জন্য( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<দ্বিগুণ>(i));

        update_Coord_যোগ(p, coord_যোগ);

        জন্য (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            পূর্ণসংখ্যা ilo = 0, ihi, inhi;
            যদি( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            অন্যথা
            {
                ihi = 1; inhi = 0;
            }
            জন্য( i = 0; i <= ndim; i++ )
            {
                দ্বিগুণ yval = y_[i];
                যদি (yval <= y_[ilo])
                    ilo = i;
                যদি (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                অন্যথা যদি (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            যদি( ilo == inhi || ilo == ihi )
            {
                জন্য( i = 0; i <= ndim; i++ )
                {
                    দ্বিগুণ yval = y_[i];
                    যদি( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        বিরতি;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_matrix(y);

            // check stop criterion
            দ্বিগুণ error = fabs(y_[ihi] - y_[ilo]);
            দ্বিগুণ range = 0;
            জন্য( j = 0; j < ndim; j++ )
            {
                দ্বিগুণ minval, maxval;
                minval = maxval = p.at<দ্বিগুণ>(0, j);
                জন্য( i = 1; i <= ndim; i++ )
                {
                    দ্বিগুণ pval = p.at<দ্বিগুণ>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            যদি( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                জন্য( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<দ্বিগুণ>(0, j), p.at<দ্বিগুণ>(ilo, j));
                }
                বিরতি;
            }

            দ্বিগুণ y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            দ্বিগুণ alpha = -1.0;
            দ্বিগুণ y_alpha = tryNewPoint(p, coord_যোগ, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_matrix(buf);

            যদি( y_alpha < y_nhi )
            {
                যদি( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    দ্বিগুণ beta = -2.0;
                    দ্বিগুণ y_beta = tryNewPoint(p, coord_যোগ, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_matrix(buf);
                    যদি( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replacePoint(p, coord_যোগ, y, ihi, alpha, y_alpha);
            }
            অন্যথা
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                দ্বিগুণ gamma = 0.5;
                দ্বিগুণ y_gamma = tryNewPoint(p, coord_যোগ, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_matrix(buf);
                যদি( y_gamma < y_hi )
                    replacePoint(p, coord_যোগ, y, ihi, gamma, y_gamma);
                অন্যথা
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    জন্য( i = 0; i <= ndim; i++ )
                    {
                        যদি (i != ilo)
                        {
                            জন্য( j = 0; j < ndim; j++ )
                                p.at<দ্বিগুণ>(i, j) = 0.5*(p.at<দ্বিগুণ>(i, j) + p.at<দ্বিগুণ>(ilo, j));
                            y_[i] = calc_f(p.ptr<দ্বিগুণ>(i));
                        }
                    }
                    fcount += ndim;
                    update_Coord_যোগ(p, coord_যোগ);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_matrix(p);
        }
        ফেরত y_[0];
    }

    অন্তঃরেখ দ্বিগুণ calc_f(ধ্রুবক দ্বিগুণ* ptr)
    {
        দ্বিগুণ res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        ফেরত res;
    }

    দ্বিগুণ tryNewPoint( Mat& p, Mat& coord_যোগ, পূর্ণসংখ্যা ihi, দ্বিগুণ alpha_, Mat& ptry, পূর্ণসংখ্যা& fcount )
    {
        পূর্ণসংখ্যা j, ndim = p.cols;

        দ্বিগুণ alpha = (1.0 - alpha_)/ndim;
        দ্বিগুণ beta = alpha - alpha_;
        দ্বিগুণ* p_ihi = p.ptr<দ্বিগুণ>(ihi);
        দ্বিগুণ* ptry_ = ptry.ptr<দ্বিগুণ>();
        দ্বিগুণ* coord_যোগ_2 = coord_যোগ.ptr<দ্বিগুণ>();

        জন্য( j = 0; j < ndim; j++ )
            ptry_[j] = coord_যোগ_2[j]*alpha - p_ihi[j]*beta;

        fcount++;
        ফেরত calc_f(ptry_);
    }

    শূন্য replacePoint( Mat& p, Mat& coord_যোগ, Mat& y, পূর্ণসংখ্যা ihi, দ্বিগুণ alpha_, দ্বিগুণ ytry )
    {
        পূর্ণসংখ্যা j, ndim = p.cols;

        দ্বিগুণ alpha = (1.0 - alpha_)/ndim;
        দ্বিগুণ beta = alpha - alpha_;
        দ্বিগুণ* p_ihi = p.ptr<দ্বিগুণ>(ihi);
        দ্বিগুণ* coord_যোগ_2 = coord_যোগ.ptr<দ্বিগুণ>();

        জন্য( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_যোগ_2[j]*alpha - p_ihi[j]*beta;
        y.at<দ্বিগুণ>(ihi) = ytry;
        update_Coord_যোগ(p, coord_যোগ);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( ধ্রুবক Ptr<MinProblemSolver::Function>& f,
                                            প্রবেশ_Array initStep, TermCriteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->setFunction(f);
    DS->setInitStep(initStep);
    DS->setTermCriteria(termcrit);
    ফেরত DS;
}

}
