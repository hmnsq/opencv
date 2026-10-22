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
#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಆದರೆ 0
#ವ್ಯಾಖ್ಯಾನಿಸು dprintf(x) ಬರೆ x
#ವ್ಯಾಖ್ಯಾನಿಸು print_ಮಾತೃಕೆ(x) print(x)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು dprintf(x)
#ವ್ಯಾಖ್ಯಾನಿಸು print_ಮಾತೃಕೆ(x)
#ಆದರೆ_ಅಂತ್ಯ

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

ನಾಮಾಕಾಶ cv
{

ವರ್ಗ DownhillSolverImpl CV_FINAL : ಸಾರ್ವಜನಿಕ DownhillSolver
{
ಸಾರ್ವಜನಿಕ:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        ಹೆಜ್ಜೆ_3=Mat_<ದ್ವಿಗುಣ>();
    }

    ಶೂನ್ಯ get_Init_ಹೆಜ್ಜೆ(OutputArray ಹೆಜ್ಜೆ) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹೆಜ್ಜೆ_3.copy_ಗಮ್ಯ(ಹೆಜ್ಜೆ); }
    ಶೂನ್ಯ ಗಣ_Init_ಹೆಜ್ಜೆ(ಒಳಹರಿವು_Array ಹೆಜ್ಜೆ) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = ಹೆಜ್ಜೆ.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, m.ಅಡ್ಡಸಾಲುಗಳು));
        ಆದರೆ( m.ಅಡ್ಡಸಾಲುಗಳು == 1 )
            m.copy_ಗಮ್ಯ(ಹೆಜ್ಜೆ_3);
        ಇಲ್ಲದಿದ್ದರೆ
            transpose(m, ಹೆಜ್ಜೆ_3);
    }

    Ptr<MinProblemSolver::Function> getFunction() ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ _Function; }

    ಶೂನ್ಯ ಗಣ_Function(ಸ್ಥಿರಾಂಕ Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    ಪದ_Criteria get_ಪದ_Criteria() ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ _termcrit; }

    ಶೂನ್ಯ ಗಣ_ಪದ_Criteria( ಸ್ಥಿರಾಂಕ ಪದ_Criteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (ಪದ_Criteria::MAX_ITER + ಪದ_Criteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_ಎಣಿಕೆ > 0 );
        _termcrit=termcrit;
    }

    ದ್ವಿಗುಣ minimize( ಒಳಹರಿವು_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(ಹೆಜ್ಜೆ_3.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಹೆಜ್ಜೆ_3.ಅಡ್ಡಸಾಲುಗಳು) == 1 &&
                  std::max(ಹೆಜ್ಜೆ_3.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಹೆಜ್ಜೆ_3.ಅಡ್ಡಸಾಲುಗಳು) >= 2 &&
                  ಹೆಜ್ಜೆ_3.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_ಎಣಿಕೆ,_termcrit.epsilon));
        dprintf(("step\n"));
        print_ಮಾತೃಕೆ(ಹೆಜ್ಜೆ_3);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, ಹೆಜ್ಜೆ_3);
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = 0;
        ದ್ವಿಗುಣ res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          ಎಣಿಕೆ_3, _termcrit.max_ಎಣಿಕೆ);
        dprintf(("%d iterations done\n",ಎಣಿಕೆ_3));

        ಆದರೆ( !x.empty() )
        {
            Mat simplex_0m(x.ಅಡ್ಡಸಾಲುಗಳು, x.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, CV_64F, simplex.ptr<ದ್ವಿಗುಣ>());
            simplex_0m.convert_ಗಮ್ಯ(x, x.type());
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಪೂರ್ಣಾಂಕ x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.ಅಡ್ಡಸಾಲು(0).convert_ಗಮ್ಯ(x_, x_type);
        }
        ಹಿಂತಿರುಗಿ res;
    }
ಸಂರಕ್ಷಿತ:
    Ptr<MinProblemSolver::Function> _Function;
    ಪದ_Criteria _termcrit;
    Mat ಹೆಜ್ಜೆ_3;

    ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_Coord_ಮೊತ್ತ(ಸ್ಥಿರಾಂಕ Mat& p, Mat& coord_ಮೊತ್ತ)
    {
        ಪೂರ್ಣಾಂಕ i, j, m = p.ಅಡ್ಡಸಾಲುಗಳು, n = p.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ದ್ವಿಗುಣ* coord_ಮೊತ್ತ_2 = coord_ಮೊತ್ತ.ptr<ದ್ವಿಗುಣ>();
        CV_Assert( coord_ಮೊತ್ತ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == n && coord_ಮೊತ್ತ.ಅಡ್ಡಸಾಲುಗಳು == 1 );

        ಗಾಗಿ( j = 0; j < n; j++ )
            coord_ಮೊತ್ತ_2[j] = 0.;

        ಗಾಗಿ( i = 0; i < m; i++ )
        {
            ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* p_i = p.ptr<ದ್ವಿಗುಣ>(i);
            ಗಾಗಿ( j = 0; j < n; j++ )
                coord_ಮೊತ್ತ_2[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_ಮಾತೃಕೆ(coord_ಮೊತ್ತ);

    }

    ಸಾಲೊಳಗೆ ಶೂನ್ಯ createInitialSimplex( ಸ್ಥಿರಾಂಕ Mat& x0, Mat& simplex, Mat& ಹೆಜ್ಜೆ )
    {
        ಪೂರ್ಣಾಂಕ i, j, ndim = ಹೆಜ್ಜೆ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        ಆದರೆ( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 && x.ಅಡ್ಡಸಾಲುಗಳು == ndim) || (x.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == ndim && x.ಅಡ್ಡಸಾಲುಗಳು == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.ಅಡ್ಡಸಾಲುಗಳು, x.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, CV_64F, simplex.ptr<ದ್ವಿಗುಣ>());

        x.convert_ಗಮ್ಯ(simplex_0m, CV_64F);
        ದ್ವಿಗುಣ* simplex_0 = simplex.ptr<ದ್ವಿಗುಣ>();
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ಹೆಜ್ಜೆ_2 = ಹೆಜ್ಜೆ.ptr<ದ್ವಿಗುಣ>();
        ಗಾಗಿ( i = 1; i <= ndim; i++ )
        {
            ದ್ವಿಗುಣ* simplex_i = simplex.ptr<ದ್ವಿಗುಣ>(i);
            ಗಾಗಿ( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*ಹೆಜ್ಜೆ_2[i-1];
        }
        ಗಾಗಿ( j = 0; j < ndim; j++ )
            simplex_0[j] -= 0.5*ಹೆಜ್ಜೆ_2[j];

        dprintf(("\nthis is simplex\n"));
        print_ಮಾತೃಕೆ(simplex);
    }

    /*
     Performs the actual minimization of MinProblemSolver::Function f (after the initialization was done)

     The matrix p[ndim+1][1..ndim] represents ndim+1 vertices that
     form a simplex - each row is an ndim vector.
     On output, fcount gives the number of function evaluations taken.
    */
    ದ್ವಿಗುಣ innerDownhillSimplex( Mat& p, ದ್ವಿಗುಣ MinRange, ದ್ವಿಗುಣ MinError, ಪೂರ್ಣಾಂಕ& fcount, ಪೂರ್ಣಾಂಕ nmax )
    {
        ಪೂರ್ಣಾಂಕ i, j, ndim = p.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        Mat coord_ಮೊತ್ತ(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        ದ್ವಿಗುಣ* y_ = y.ptr<ದ್ವಿಗುಣ>();

        fcount = ndim+1;
        ಗಾಗಿ( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<ದ್ವಿಗುಣ>(i));

        update_Coord_ಮೊತ್ತ(p, coord_ಮೊತ್ತ);

        ಗಾಗಿ (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            ಪೂರ್ಣಾಂಕ ilo = 0, ihi, inhi;
            ಆದರೆ( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ihi = 1; inhi = 0;
            }
            ಗಾಗಿ( i = 0; i <= ndim; i++ )
            {
                ದ್ವಿಗುಣ yval = y_[i];
                ಆದರೆ (yval <= y_[ilo])
                    ilo = i;
                ಆದರೆ (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            ಆದರೆ( ilo == inhi || ilo == ihi )
            {
                ಗಾಗಿ( i = 0; i <= ndim; i++ )
                {
                    ದ್ವಿಗುಣ yval = y_[i];
                    ಆದರೆ( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        ನಿಲ್ಲಿಸು;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_ಮಾತೃಕೆ(y);

            // check stop criterion
            ದ್ವಿಗುಣ error = fabs(y_[ihi] - y_[ilo]);
            ದ್ವಿಗುಣ range = 0;
            ಗಾಗಿ( j = 0; j < ndim; j++ )
            {
                ದ್ವಿಗುಣ minval, maxval;
                minval = maxval = p.at<ದ್ವಿಗುಣ>(0, j);
                ಗಾಗಿ( i = 1; i <= ndim; i++ )
                {
                    ದ್ವಿಗುಣ pval = p.at<ದ್ವಿಗುಣ>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            ಆದರೆ( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                ಗಾಗಿ( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<ದ್ವಿಗುಣ>(0, j), p.at<ದ್ವಿಗುಣ>(ilo, j));
                }
                ನಿಲ್ಲಿಸು;
            }

            ದ್ವಿಗುಣ y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            ದ್ವಿಗುಣ alpha = -1.0;
            ದ್ವಿಗುಣ y_alpha = try_New_ಬಿಂದು(p, coord_ಮೊತ್ತ, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_ಮಾತೃಕೆ(buf);

            ಆದರೆ( y_alpha < y_nhi )
            {
                ಆದರೆ( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    ದ್ವಿಗುಣ beta = -2.0;
                    ದ್ವಿಗುಣ y_beta = try_New_ಬಿಂದು(p, coord_ಮೊತ್ತ, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_ಮಾತೃಕೆ(buf);
                    ಆದರೆ( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replace_ಬಿಂದು(p, coord_ಮೊತ್ತ, y, ihi, alpha, y_alpha);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                ದ್ವಿಗುಣ gamma = 0.5;
                ದ್ವಿಗುಣ y_gamma = try_New_ಬಿಂದು(p, coord_ಮೊತ್ತ, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_ಮಾತೃಕೆ(buf);
                ಆದರೆ( y_gamma < y_hi )
                    replace_ಬಿಂದು(p, coord_ಮೊತ್ತ, y, ihi, gamma, y_gamma);
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    ಗಾಗಿ( i = 0; i <= ndim; i++ )
                    {
                        ಆದರೆ (i != ilo)
                        {
                            ಗಾಗಿ( j = 0; j < ndim; j++ )
                                p.at<ದ್ವಿಗುಣ>(i, j) = 0.5*(p.at<ದ್ವಿಗುಣ>(i, j) + p.at<ದ್ವಿಗುಣ>(ilo, j));
                            y_[i] = calc_f(p.ptr<ದ್ವಿಗುಣ>(i));
                        }
                    }
                    fcount += ndim;
                    update_Coord_ಮೊತ್ತ(p, coord_ಮೊತ್ತ);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_ಮಾತೃಕೆ(p);
        }
        ಹಿಂತಿರುಗಿ y_[0];
    }

    ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ calc_f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ptr)
    {
        ದ್ವಿಗುಣ res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        ಹಿಂತಿರುಗಿ res;
    }

    ದ್ವಿಗುಣ try_New_ಬಿಂದು( Mat& p, Mat& coord_ಮೊತ್ತ, ಪೂರ್ಣಾಂಕ ihi, ದ್ವಿಗುಣ alpha_, Mat& ptry, ಪೂರ್ಣಾಂಕ& fcount )
    {
        ಪೂರ್ಣಾಂಕ j, ndim = p.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

        ದ್ವಿಗುಣ alpha = (1.0 - alpha_)/ndim;
        ದ್ವಿಗುಣ beta = alpha - alpha_;
        ದ್ವಿಗುಣ* p_ihi = p.ptr<ದ್ವಿಗುಣ>(ihi);
        ದ್ವಿಗುಣ* ptry_ = ptry.ptr<ದ್ವಿಗುಣ>();
        ದ್ವಿಗುಣ* coord_ಮೊತ್ತ_2 = coord_ಮೊತ್ತ.ptr<ದ್ವಿಗುಣ>();

        ಗಾಗಿ( j = 0; j < ndim; j++ )
            ptry_[j] = coord_ಮೊತ್ತ_2[j]*alpha - p_ihi[j]*beta;

        fcount++;
        ಹಿಂತಿರುಗಿ calc_f(ptry_);
    }

    ಶೂನ್ಯ replace_ಬಿಂದು( Mat& p, Mat& coord_ಮೊತ್ತ, Mat& y, ಪೂರ್ಣಾಂಕ ihi, ದ್ವಿಗುಣ alpha_, ದ್ವಿಗುಣ ytry )
    {
        ಪೂರ್ಣಾಂಕ j, ndim = p.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

        ದ್ವಿಗುಣ alpha = (1.0 - alpha_)/ndim;
        ದ್ವಿಗುಣ beta = alpha - alpha_;
        ದ್ವಿಗುಣ* p_ihi = p.ptr<ದ್ವಿಗುಣ>(ihi);
        ದ್ವಿಗುಣ* coord_ಮೊತ್ತ_2 = coord_ಮೊತ್ತ.ptr<ದ್ವಿಗುಣ>();

        ಗಾಗಿ( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_ಮೊತ್ತ_2[j]*alpha - p_ihi[j]*beta;
        y.at<ದ್ವಿಗುಣ>(ihi) = ytry;
        update_Coord_ಮೊತ್ತ(p, coord_ಮೊತ್ತ);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( ಸ್ಥಿರಾಂಕ Ptr<MinProblemSolver::Function>& f,
                                            ಒಳಹರಿವು_Array init_ಹೆಜ್ಜೆ, ಪದ_Criteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->ಗಣ_Function(f);
    DS->ಗಣ_Init_ಹೆಜ್ಜೆ(init_ಹೆಜ್ಜೆ);
    DS->ಗಣ_ಪದ_Criteria(termcrit);
    ಹಿಂತಿರುಗಿ DS;
}

}
