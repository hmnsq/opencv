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
#అన్తర్భావ "precomp.hpp"

#ఒకవేళ 0
#నిర్వచించు dprintf(x) రాయి x
#నిర్వచించు print_మాతృక(x) print(x)
#లేకపోతే
#నిర్వచించు dprintf(x)
#నిర్వచించు print_మాతృక(x)
#అయితే_ముగింపు

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

నామాకాశ cv
{

తరగతి DownhillSolverImpl CV_FINAL : బహిరంగ DownhillSolver
{
బహిరంగ:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        అడుగు_3=Mat_<ద్విగుణ>();
    }

    శూన్య get_Init_అడుగు(OutputArray అడుగు) స్థిరరాశి CV_OVERRIDE { అడుగు_3.copy_గమ్యం(అడుగు); }
    శూన్య సమితి_Init_అడుగు(ఆదానం_Array అడుగు) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = అడుగు.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.నిలువుల_సంఖ్య, m.అడ్డువరుసలు));
        ఒకవేళ( m.అడ్డువరుసలు == 1 )
            m.copy_గమ్యం(అడుగు_3);
        లేకపోతే
            transpose(m, అడుగు_3);
    }

    Ptr<MinProblemSolver::Function> getFunction() స్థిరరాశి CV_OVERRIDE { తిరిగి _Function; }

    శూన్య సమితి_Function(స్థిరరాశి Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    పదం_Criteria get_పదం_Criteria() స్థిరరాశి CV_OVERRIDE { తిరిగి _termcrit; }

    శూన్య సమితి_పదం_Criteria( స్థిరరాశి పదం_Criteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (పదం_Criteria::MAX_ITER + పదం_Criteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_లెక్క > 0 );
        _termcrit=termcrit;
    }

    ద్విగుణ minimize( ఆదానం_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(అడుగు_3.నిలువుల_సంఖ్య, అడుగు_3.అడ్డువరుసలు) == 1 &&
                  std::max(అడుగు_3.నిలువుల_సంఖ్య, అడుగు_3.అడ్డువరుసలు) >= 2 &&
                  అడుగు_3.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_లెక్క,_termcrit.epsilon));
        dprintf(("step\n"));
        print_మాతృక(అడుగు_3);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, అడుగు_3);
        పూర్ణాంకం లెక్క_2 = 0;
        ద్విగుణ res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          లెక్క_2, _termcrit.max_లెక్క);
        dprintf(("%d iterations done\n",లెక్క_2));

        ఒకవేళ( !x.empty() )
        {
            Mat simplex_0m(x.అడ్డువరుసలు, x.నిలువుల_సంఖ్య, CV_64F, simplex.ptr<ద్విగుణ>());
            simplex_0m.convert_గమ్యం(x, x.type());
        }
        లేకపోతే
        {
            పూర్ణాంకం x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.అడ్డువరుస(0).convert_గమ్యం(x_, x_type);
        }
        తిరిగి res;
    }
రక్షిత:
    Ptr<MinProblemSolver::Function> _Function;
    పదం_Criteria _termcrit;
    Mat అడుగు_3;

    అంతఃవరుస శూన్య update_Coord_కూడిక(స్థిరరాశి Mat& p, Mat& coord_కూడిక)
    {
        పూర్ణాంకం i, j, m = p.అడ్డువరుసలు, n = p.నిలువుల_సంఖ్య;
        ద్విగుణ* coord_కూడిక_2 = coord_కూడిక.ptr<ద్విగుణ>();
        CV_Assert( coord_కూడిక.నిలువుల_సంఖ్య == n && coord_కూడిక.అడ్డువరుసలు == 1 );

        కొరకు( j = 0; j < n; j++ )
            coord_కూడిక_2[j] = 0.;

        కొరకు( i = 0; i < m; i++ )
        {
            స్థిరరాశి ద్విగుణ* p_i = p.ptr<ద్విగుణ>(i);
            కొరకు( j = 0; j < n; j++ )
                coord_కూడిక_2[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_మాతృక(coord_కూడిక);

    }

    అంతఃవరుస శూన్య createInitialSimplex( స్థిరరాశి Mat& x0, Mat& simplex, Mat& అడుగు )
    {
        పూర్ణాంకం i, j, ndim = అడుగు.నిలువుల_సంఖ్య;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        ఒకవేళ( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.నిలువుల_సంఖ్య == 1 && x.అడ్డువరుసలు == ndim) || (x.నిలువుల_సంఖ్య == ndim && x.అడ్డువరుసలు == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.అడ్డువరుసలు, x.నిలువుల_సంఖ్య, CV_64F, simplex.ptr<ద్విగుణ>());

        x.convert_గమ్యం(simplex_0m, CV_64F);
        ద్విగుణ* simplex_0 = simplex.ptr<ద్విగుణ>();
        స్థిరరాశి ద్విగుణ* అడుగు_2 = అడుగు.ptr<ద్విగుణ>();
        కొరకు( i = 1; i <= ndim; i++ )
        {
            ద్విగుణ* simplex_i = simplex.ptr<ద్విగుణ>(i);
            కొరకు( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*అడుగు_2[i-1];
        }
        కొరకు( j = 0; j < ndim; j++ )
            simplex_0[j] -= 0.5*అడుగు_2[j];

        dprintf(("\nthis is simplex\n"));
        print_మాతృక(simplex);
    }

    /*
     Performs the actual minimization of MinProblemSolver::Function f (after the initialization was done)

     The matrix p[ndim+1][1..ndim] represents ndim+1 vertices that
     form a simplex - each row is an ndim vector.
     On output, fcount gives the number of function evaluations taken.
    */
    ద్విగుణ innerDownhillSimplex( Mat& p, ద్విగుణ MinRange, ద్విగుణ MinError, పూర్ణాంకం& fcount, పూర్ణాంకం nmax )
    {
        పూర్ణాంకం i, j, ndim = p.నిలువుల_సంఖ్య;
        Mat coord_కూడిక(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        ద్విగుణ* y_ = y.ptr<ద్విగుణ>();

        fcount = ndim+1;
        కొరకు( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<ద్విగుణ>(i));

        update_Coord_కూడిక(p, coord_కూడిక);

        కొరకు (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            పూర్ణాంకం ilo = 0, ihi, inhi;
            ఒకవేళ( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            లేకపోతే
            {
                ihi = 1; inhi = 0;
            }
            కొరకు( i = 0; i <= ndim; i++ )
            {
                ద్విగుణ yval = y_[i];
                ఒకవేళ (yval <= y_[ilo])
                    ilo = i;
                ఒకవేళ (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                లేకపోతే ఒకవేళ (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            ఒకవేళ( ilo == inhi || ilo == ihi )
            {
                కొరకు( i = 0; i <= ndim; i++ )
                {
                    ద్విగుణ yval = y_[i];
                    ఒకవేళ( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        ఆపు;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_మాతృక(y);

            // check stop criterion
            ద్విగుణ error = fabs(y_[ihi] - y_[ilo]);
            ద్విగుణ range = 0;
            కొరకు( j = 0; j < ndim; j++ )
            {
                ద్విగుణ minval, maxval;
                minval = maxval = p.at<ద్విగుణ>(0, j);
                కొరకు( i = 1; i <= ndim; i++ )
                {
                    ద్విగుణ pval = p.at<ద్విగుణ>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            ఒకవేళ( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                కొరకు( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<ద్విగుణ>(0, j), p.at<ద్విగుణ>(ilo, j));
                }
                ఆపు;
            }

            ద్విగుణ y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            ద్విగుణ alpha = -1.0;
            ద్విగుణ y_alpha = try_New_బిందువు(p, coord_కూడిక, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_మాతృక(buf);

            ఒకవేళ( y_alpha < y_nhi )
            {
                ఒకవేళ( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    ద్విగుణ beta = -2.0;
                    ద్విగుణ y_beta = try_New_బిందువు(p, coord_కూడిక, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_మాతృక(buf);
                    ఒకవేళ( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replace_బిందువు(p, coord_కూడిక, y, ihi, alpha, y_alpha);
            }
            లేకపోతే
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                ద్విగుణ gamma = 0.5;
                ద్విగుణ y_gamma = try_New_బిందువు(p, coord_కూడిక, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_మాతృక(buf);
                ఒకవేళ( y_gamma < y_hi )
                    replace_బిందువు(p, coord_కూడిక, y, ihi, gamma, y_gamma);
                లేకపోతే
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    కొరకు( i = 0; i <= ndim; i++ )
                    {
                        ఒకవేళ (i != ilo)
                        {
                            కొరకు( j = 0; j < ndim; j++ )
                                p.at<ద్విగుణ>(i, j) = 0.5*(p.at<ద్విగుణ>(i, j) + p.at<ద్విగుణ>(ilo, j));
                            y_[i] = calc_f(p.ptr<ద్విగుణ>(i));
                        }
                    }
                    fcount += ndim;
                    update_Coord_కూడిక(p, coord_కూడిక);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_మాతృక(p);
        }
        తిరిగి y_[0];
    }

    అంతఃవరుస ద్విగుణ calc_f(స్థిరరాశి ద్విగుణ* ptr)
    {
        ద్విగుణ res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        తిరిగి res;
    }

    ద్విగుణ try_New_బిందువు( Mat& p, Mat& coord_కూడిక, పూర్ణాంకం ihi, ద్విగుణ alpha_, Mat& ptry, పూర్ణాంకం& fcount )
    {
        పూర్ణాంకం j, ndim = p.నిలువుల_సంఖ్య;

        ద్విగుణ alpha = (1.0 - alpha_)/ndim;
        ద్విగుణ beta = alpha - alpha_;
        ద్విగుణ* p_ihi = p.ptr<ద్విగుణ>(ihi);
        ద్విగుణ* ptry_ = ptry.ptr<ద్విగుణ>();
        ద్విగుణ* coord_కూడిక_2 = coord_కూడిక.ptr<ద్విగుణ>();

        కొరకు( j = 0; j < ndim; j++ )
            ptry_[j] = coord_కూడిక_2[j]*alpha - p_ihi[j]*beta;

        fcount++;
        తిరిగి calc_f(ptry_);
    }

    శూన్య replace_బిందువు( Mat& p, Mat& coord_కూడిక, Mat& y, పూర్ణాంకం ihi, ద్విగుణ alpha_, ద్విగుణ ytry )
    {
        పూర్ణాంకం j, ndim = p.నిలువుల_సంఖ్య;

        ద్విగుణ alpha = (1.0 - alpha_)/ndim;
        ద్విగుణ beta = alpha - alpha_;
        ద్విగుణ* p_ihi = p.ptr<ద్విగుణ>(ihi);
        ద్విగుణ* coord_కూడిక_2 = coord_కూడిక.ptr<ద్విగుణ>();

        కొరకు( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_కూడిక_2[j]*alpha - p_ihi[j]*beta;
        y.at<ద్విగుణ>(ihi) = ytry;
        update_Coord_కూడిక(p, coord_కూడిక);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( స్థిరరాశి Ptr<MinProblemSolver::Function>& f,
                                            ఆదానం_Array init_అడుగు, పదం_Criteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->సమితి_Function(f);
    DS->సమితి_Init_అడుగు(init_అడుగు);
    DS->సమితి_పదం_Criteria(termcrit);
    తిరిగి DS;
}

}
