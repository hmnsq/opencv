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
#समावेश "precomp.hpp"

#अगर 0
#घोषणा dprintf(x) म_लिखो x
#घोषणा print_matrix(x) print(x)
#अन्यथा
#घोषणा dprintf(x)
#घोषणा print_matrix(x)
#पूर्ण_अगर

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

श्रेणी DownhillSolverImpl CV_FINAL : खुला DownhillSolver
{
खुला:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        _step=Mat_<द्विगुन>();
    }

    व्योम getInitStep(OutputArray step) स्थिर CV_OVERRIDE { _step.copyTo(step); }
    व्योम setInitStep(निवेश_Array step) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = step.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.cols, m.rows));
        अगर( m.rows == 1 )
            m.copyTo(_step);
        अन्यथा
            transpose(m, _step);
    }

    Ptr<MinProblemSolver::Function> getFunction() स्थिर CV_OVERRIDE { वापस _Function; }

    व्योम setFunction(स्थिर Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    TermCriteria getTermCriteria() स्थिर CV_OVERRIDE { वापस _termcrit; }

    व्योम setTermCriteria( स्थिर TermCriteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (TermCriteria::MAX_ITER + TermCriteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_गिनती > 0 );
        _termcrit=termcrit;
    }

    द्विगुन minimize( निवेश_Output_Array x_ ) CV_OVERRIDE
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
        पूर्णांक गिनती_2 = 0;
        द्विगुन res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          गिनती_2, _termcrit.max_गिनती);
        dprintf(("%d iterations done\n",गिनती_2));

        अगर( !x.empty() )
        {
            Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<द्विगुन>());
            simplex_0m.convertTo(x, x.type());
        }
        अन्यथा
        {
            पूर्णांक x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.row(0).convertTo(x_, x_type);
        }
        वापस res;
    }
रक्षित:
    Ptr<MinProblemSolver::Function> _Function;
    TermCriteria _termcrit;
    Mat _step;

    अंतरभूत व्योम update_Coord_योग(स्थिर Mat& p, Mat& coord_योग)
    {
        पूर्णांक i, j, m = p.rows, n = p.cols;
        द्विगुन* coord_योग_2 = coord_योग.ptr<द्विगुन>();
        CV_Assert( coord_योग.cols == n && coord_योग.rows == 1 );

        क्रम( j = 0; j < n; j++ )
            coord_योग_2[j] = 0.;

        क्रम( i = 0; i < m; i++ )
        {
            स्थिर द्विगुन* p_i = p.ptr<द्विगुन>(i);
            क्रम( j = 0; j < n; j++ )
                coord_योग_2[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_matrix(coord_योग);

    }

    अंतरभूत व्योम createInitialSimplex( स्थिर Mat& x0, Mat& simplex, Mat& step )
    {
        पूर्णांक i, j, ndim = step.cols;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        अगर( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.cols == 1 && x.rows == ndim) || (x.cols == ndim && x.rows == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.rows, x.cols, CV_64F, simplex.ptr<द्विगुन>());

        x.convertTo(simplex_0m, CV_64F);
        द्विगुन* simplex_0 = simplex.ptr<द्विगुन>();
        स्थिर द्विगुन* step_ = step.ptr<द्विगुन>();
        क्रम( i = 1; i <= ndim; i++ )
        {
            द्विगुन* simplex_i = simplex.ptr<द्विगुन>(i);
            क्रम( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*step_[i-1];
        }
        क्रम( j = 0; j < ndim; j++ )
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
    द्विगुन innerDownhillSimplex( Mat& p, द्विगुन MinRange, द्विगुन MinError, पूर्णांक& fcount, पूर्णांक nmax )
    {
        पूर्णांक i, j, ndim = p.cols;
        Mat coord_योग(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        द्विगुन* y_ = y.ptr<द्विगुन>();

        fcount = ndim+1;
        क्रम( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<द्विगुन>(i));

        update_Coord_योग(p, coord_योग);

        क्रम (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            पूर्णांक ilo = 0, ihi, inhi;
            अगर( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            अन्यथा
            {
                ihi = 1; inhi = 0;
            }
            क्रम( i = 0; i <= ndim; i++ )
            {
                द्विगुन yval = y_[i];
                अगर (yval <= y_[ilo])
                    ilo = i;
                अगर (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                अन्यथा अगर (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            अगर( ilo == inhi || ilo == ihi )
            {
                क्रम( i = 0; i <= ndim; i++ )
                {
                    द्विगुन yval = y_[i];
                    अगर( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        अवरोध;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_matrix(y);

            // check stop criterion
            द्विगुन error = भ_असल(y_[ihi] - y_[ilo]);
            द्विगुन range = 0;
            क्रम( j = 0; j < ndim; j++ )
            {
                द्विगुन minval, maxval;
                minval = maxval = p.at<द्विगुन>(0, j);
                क्रम( i = 1; i <= ndim; i++ )
                {
                    द्विगुन pval = p.at<द्विगुन>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, भ_असल(maxval - minval));
            }

            अगर( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                क्रम( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<द्विगुन>(0, j), p.at<द्विगुन>(ilo, j));
                }
                अवरोध;
            }

            द्विगुन y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            द्विगुन alpha = -1.0;
            द्विगुन y_alpha = tryNewPoint(p, coord_योग, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_matrix(buf);

            अगर( y_alpha < y_nhi )
            {
                अगर( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    द्विगुन beta = -2.0;
                    द्विगुन y_beta = tryNewPoint(p, coord_योग, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_matrix(buf);
                    अगर( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replacePoint(p, coord_योग, y, ihi, alpha, y_alpha);
            }
            अन्यथा
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                द्विगुन gamma = 0.5;
                द्विगुन y_gamma = tryNewPoint(p, coord_योग, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_matrix(buf);
                अगर( y_gamma < y_hi )
                    replacePoint(p, coord_योग, y, ihi, gamma, y_gamma);
                अन्यथा
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    क्रम( i = 0; i <= ndim; i++ )
                    {
                        अगर (i != ilo)
                        {
                            क्रम( j = 0; j < ndim; j++ )
                                p.at<द्विगुन>(i, j) = 0.5*(p.at<द्विगुन>(i, j) + p.at<द्विगुन>(ilo, j));
                            y_[i] = calc_f(p.ptr<द्विगुन>(i));
                        }
                    }
                    fcount += ndim;
                    update_Coord_योग(p, coord_योग);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_matrix(p);
        }
        वापस y_[0];
    }

    अंतरभूत द्विगुन calc_f(स्थिर द्विगुन* ptr)
    {
        द्विगुन res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        वापस res;
    }

    द्विगुन tryNewPoint( Mat& p, Mat& coord_योग, पूर्णांक ihi, द्विगुन alpha_, Mat& ptry, पूर्णांक& fcount )
    {
        पूर्णांक j, ndim = p.cols;

        द्विगुन alpha = (1.0 - alpha_)/ndim;
        द्विगुन beta = alpha - alpha_;
        द्विगुन* p_ihi = p.ptr<द्विगुन>(ihi);
        द्विगुन* ptry_ = ptry.ptr<द्विगुन>();
        द्विगुन* coord_योग_2 = coord_योग.ptr<द्विगुन>();

        क्रम( j = 0; j < ndim; j++ )
            ptry_[j] = coord_योग_2[j]*alpha - p_ihi[j]*beta;

        fcount++;
        वापस calc_f(ptry_);
    }

    व्योम replacePoint( Mat& p, Mat& coord_योग, Mat& y, पूर्णांक ihi, द्विगुन alpha_, द्विगुन ytry )
    {
        पूर्णांक j, ndim = p.cols;

        द्विगुन alpha = (1.0 - alpha_)/ndim;
        द्विगुन beta = alpha - alpha_;
        द्विगुन* p_ihi = p.ptr<द्विगुन>(ihi);
        द्विगुन* coord_योग_2 = coord_योग.ptr<द्विगुन>();

        क्रम( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_योग_2[j]*alpha - p_ihi[j]*beta;
        y.at<द्विगुन>(ihi) = ytry;
        update_Coord_योग(p, coord_योग);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( स्थिर Ptr<MinProblemSolver::Function>& f,
                                            निवेश_Array initStep, TermCriteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->setFunction(f);
    DS->setInitStep(initStep);
    DS->setTermCriteria(termcrit);
    वापस DS;
}

}
