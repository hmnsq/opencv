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
#अन्तर्भाव "precomp.hpp"

#यदि 0
#define dprintf(x) printf x
#define print_matrix(x) print(x)
#अन्यथा
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

नामाकाश cv
{

वर्ग DownhillSolverImpl CV_FINAL : सार्वजनिक DownhillSolver
{
सार्वजनिक:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        _step=Mat_<द्विगुण>();
    }

    शून्य getInitStep(OutputArray step) ध्रुव CV_OVERRIDE { _step.copyTo(step); }
    शून्य setInitStep(निवेश_Array step) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = step.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.cols, m.rows));
        यदि( m.rows == 1 )
            m.copyTo(_step);
        अन्यथा
            transpose(m, _step);
    }

    Ptr<MinProblemSolver::Function> getFunction() ध्रुव CV_OVERRIDE { प्रत्यागम _Function; }

    शून्य setFunction(ध्रुव Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    TermCriteria getTermCriteria() ध्रुव CV_OVERRIDE { प्रत्यागम _termcrit; }

    शून्य setTermCriteria( ध्रुव TermCriteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (TermCriteria::MAX_ITER + TermCriteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_गिनती > 0 );
        _termcrit=termcrit;
    }

    द्विगुण minimize( निवेश_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(_step.cols, _step.rows) == 1 &&
                  std::max(_step.cols, _step.rows) >= 2 &&
                  _step.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_गिनती,_termcrit.epsilon));
        dprintf(("step\n"));
        print_matrix(_step);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, _step);
        पूर्णाङ्क गिनती_2 = 0;
        द्विगुण res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          गिनती_2, _termcrit.max_गिनती);
        dprintf(("%d iterations done\n",गिनती_2));

        यदि( !x.empty() )
        {
            Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<द्विगुण>());
            simplex_0m.convertTo(x, x.type());
        }
        अन्यथा
        {
            पूर्णाङ्क x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.row(0).convertTo(x_, x_type);
        }
        प्रत्यागम res;
    }
रक्षित:
    Ptr<MinProblemSolver::Function> _Function;
    TermCriteria _termcrit;
    Mat _step;

    अन्तःस्थ शून्य updateCoordSum(ध्रुव Mat& p, Mat& coord_sum)
    {
        पूर्णाङ्क i, j, m = p.rows, n = p.cols;
        द्विगुण* coord_sum_ = coord_sum.ptr<द्विगुण>();
        CV_Assert( coord_sum.cols == n && coord_sum.rows == 1 );

        प्रति( j = 0; j < n; j++ )
            coord_sum_[j] = 0.;

        प्रति( i = 0; i < m; i++ )
        {
            ध्रुव द्विगुण* p_i = p.ptr<द्विगुण>(i);
            प्रति( j = 0; j < n; j++ )
                coord_sum_[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_matrix(coord_sum);

    }

    अन्तःस्थ शून्य createInitialSimplex( ध्रुव Mat& x0, Mat& simplex, Mat& step )
    {
        पूर्णाङ्क i, j, ndim = step.cols;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        यदि( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.cols == 1 && x.rows == ndim) || (x.cols == ndim && x.rows == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<द्विगुण>());

        x.convertTo(simplex_0m, CV_64F);
        द्विगुण* simplex_0 = simplex.ptr<द्विगुण>();
        ध्रुव द्विगुण* step_ = step.ptr<द्विगुण>();
        प्रति( i = 1; i <= ndim; i++ )
        {
            द्विगुण* simplex_i = simplex.ptr<द्विगुण>(i);
            प्रति( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*step_[i-1];
        }
        प्रति( j = 0; j < ndim; j++ )
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
    द्विगुण innerDownhillSimplex( Mat& p, द्विगुण MinRange, द्विगुण MinError, पूर्णाङ्क& fcount, पूर्णाङ्क nmax )
    {
        पूर्णाङ्क i, j, ndim = p.cols;
        Mat coord_sum(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        द्विगुण* y_ = y.ptr<द्विगुण>();

        fcount = ndim+1;
        प्रति( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<द्विगुण>(i));

        updateCoordSum(p, coord_sum);

        प्रति (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            पूर्णाङ्क ilo = 0, ihi, inhi;
            यदि( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            अन्यथा
            {
                ihi = 1; inhi = 0;
            }
            प्रति( i = 0; i <= ndim; i++ )
            {
                द्विगुण yval = y_[i];
                यदि (yval <= y_[ilo])
                    ilo = i;
                यदि (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                अन्यथा यदि (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            यदि( ilo == inhi || ilo == ihi )
            {
                प्रति( i = 0; i <= ndim; i++ )
                {
                    द्विगुण yval = y_[i];
                    यदि( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        भङ्ग;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_matrix(y);

            // check stop criterion
            द्विगुण error = fabs(y_[ihi] - y_[ilo]);
            द्विगुण range = 0;
            प्रति( j = 0; j < ndim; j++ )
            {
                द्विगुण minval, maxval;
                minval = maxval = p.at<द्विगुण>(0, j);
                प्रति( i = 1; i <= ndim; i++ )
                {
                    द्विगुण pval = p.at<द्विगुण>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            यदि( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                प्रति( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<द्विगुण>(0, j), p.at<द्विगुण>(ilo, j));
                }
                भङ्ग;
            }

            द्विगुण y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            द्विगुण alpha = -1.0;
            द्विगुण y_alpha = tryNewPoint(p, coord_sum, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_matrix(buf);

            यदि( y_alpha < y_nhi )
            {
                यदि( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    द्विगुण beta = -2.0;
                    द्विगुण y_beta = tryNewPoint(p, coord_sum, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_matrix(buf);
                    यदि( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replacePoint(p, coord_sum, y, ihi, alpha, y_alpha);
            }
            अन्यथा
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                द्विगुण gamma = 0.5;
                द्विगुण y_gamma = tryNewPoint(p, coord_sum, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_matrix(buf);
                यदि( y_gamma < y_hi )
                    replacePoint(p, coord_sum, y, ihi, gamma, y_gamma);
                अन्यथा
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    प्रति( i = 0; i <= ndim; i++ )
                    {
                        यदि (i != ilo)
                        {
                            प्रति( j = 0; j < ndim; j++ )
                                p.at<द्विगुण>(i, j) = 0.5*(p.at<द्विगुण>(i, j) + p.at<द्विगुण>(ilo, j));
                            y_[i] = calc_f(p.ptr<द्विगुण>(i));
                        }
                    }
                    fcount += ndim;
                    updateCoordSum(p, coord_sum);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_matrix(p);
        }
        प्रत्यागम y_[0];
    }

    अन्तःस्थ द्विगुण calc_f(ध्रुव द्विगुण* ptr)
    {
        द्विगुण res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        प्रत्यागम res;
    }

    द्विगुण tryNewPoint( Mat& p, Mat& coord_sum, पूर्णाङ्क ihi, द्विगुण alpha_, Mat& ptry, पूर्णाङ्क& fcount )
    {
        पूर्णाङ्क j, ndim = p.cols;

        द्विगुण alpha = (1.0 - alpha_)/ndim;
        द्विगुण beta = alpha - alpha_;
        द्विगुण* p_ihi = p.ptr<द्विगुण>(ihi);
        द्विगुण* ptry_ = ptry.ptr<द्विगुण>();
        द्विगुण* coord_sum_ = coord_sum.ptr<द्विगुण>();

        प्रति( j = 0; j < ndim; j++ )
            ptry_[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;

        fcount++;
        प्रत्यागम calc_f(ptry_);
    }

    शून्य replacePoint( Mat& p, Mat& coord_sum, Mat& y, पूर्णाङ्क ihi, द्विगुण alpha_, द्विगुण ytry )
    {
        पूर्णाङ्क j, ndim = p.cols;

        द्विगुण alpha = (1.0 - alpha_)/ndim;
        द्विगुण beta = alpha - alpha_;
        द्विगुण* p_ihi = p.ptr<द्विगुण>(ihi);
        द्विगुण* coord_sum_ = coord_sum.ptr<द्विगुण>();

        प्रति( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_sum_[j]*alpha - p_ihi[j]*beta;
        y.at<द्विगुण>(ihi) = ytry;
        updateCoordSum(p, coord_sum);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( ध्रुव Ptr<MinProblemSolver::Function>& f,
                                            निवेश_Array initStep, TermCriteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->setFunction(f);
    DS->setInitStep(initStep);
    DS->setTermCriteria(termcrit);
    प्रत्यागम DS;
}

}
