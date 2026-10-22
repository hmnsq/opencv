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
#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਜੇ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dprintf(x) ਲਿਖੋ x
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ print_ਮੈਟ੍ਰਿਕਸ(x) print(x)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dprintf(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ print_ਮੈਟ੍ਰਿਕਸ(x)
#ਜੇ_ਅੰਤ

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

ਨਾਮਾਕਾਸ਼ cv
{

ਵਰਗ DownhillSolverImpl CV_FINAL : ਜਨਤਕ DownhillSolver
{
ਜਨਤਕ:
    DownhillSolverImpl()
    {
        _Function=Ptr<Function>();
        ਕਦਮ_3=Mat_<ਦੁੱਗਣਾ>();
    }

    ਖਾਲੀ get_Init_ਕਦਮ(OutputArray ਕਦਮ) ਅਚਲ CV_OVERRIDE { ਕਦਮ_3.copy_ਮੰਜ਼ਿਲ(ਕਦਮ); }
    ਖਾਲੀ ਸੈੱਟ_Init_ਕਦਮ(ਨਿਵੇਸ਼_Array ਕਦਮ) CV_OVERRIDE
    {
        // set dimensionality and make a deep copy of step
        Mat m = ਕਦਮ.getMat();
        dprintf(("m.cols=%d\nm.rows=%d\n", m.ਖਾਨੇ_ਗਿਣਤੀ, m.ਪੰਕਤੀਆਂ));
        ਜੇ( m.ਪੰਕਤੀਆਂ == 1 )
            m.copy_ਮੰਜ਼ਿਲ(ਕਦਮ_3);
        ਨਹੀਂਤਾਂ
            transpose(m, ਕਦਮ_3);
    }

    Ptr<MinProblemSolver::Function> getFunction() ਅਚਲ CV_OVERRIDE { ਵਾਪਸ _Function; }

    ਖਾਲੀ ਸੈੱਟ_Function(ਅਚਲ Ptr<Function>& f) CV_OVERRIDE { _Function=f; }

    ਪਦ_Criteria get_ਪਦ_Criteria() ਅਚਲ CV_OVERRIDE { ਵਾਪਸ _termcrit; }

    ਖਾਲੀ ਸੈੱਟ_ਪਦ_Criteria( ਅਚਲ ਪਦ_Criteria& termcrit ) CV_OVERRIDE
    {
        CV_Assert( termcrit.type == (ਪਦ_Criteria::MAX_ITER + ਪਦ_Criteria::EPS) &&
                   termcrit.epsilon > 0 &&
                   termcrit.max_ਗਿਣਤੀ > 0 );
        _termcrit=termcrit;
    }

    ਦੁੱਗਣਾ minimize( ਨਿਵੇਸ਼_Output_Array x_ ) CV_OVERRIDE
    {
        dprintf(("hi from minimize\n"));
        CV_Assert( !_Function.empty() );
        CV_Assert( std::min(ਕਦਮ_3.ਖਾਨੇ_ਗਿਣਤੀ, ਕਦਮ_3.ਪੰਕਤੀਆਂ) == 1 &&
                  std::max(ਕਦਮ_3.ਖਾਨੇ_ਗਿਣਤੀ, ਕਦਮ_3.ਪੰਕਤੀਆਂ) >= 2 &&
                  ਕਦਮ_3.type() == CV_64FC1 );
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_ਗਿਣਤੀ,_termcrit.epsilon));
        dprintf(("step\n"));
        print_ਮੈਟ੍ਰਿਕਸ(ਕਦਮ_3);

        Mat x = x_.getMat(), simplex;

        createInitialSimplex(x, simplex, ਕਦਮ_3);
        ਪੂਰਨ ਗਿਣਤੀ_3 = 0;
        ਦੁੱਗਣਾ res = innerDownhillSimplex(simplex,_termcrit.epsilon, _termcrit.epsilon,
                                          ਗਿਣਤੀ_3, _termcrit.max_ਗਿਣਤੀ);
        dprintf(("%d iterations done\n",ਗਿਣਤੀ_3));

        ਜੇ( !x.empty() )
        {
            Mat simplex_0m(x.ਪੰਕਤੀਆਂ, x.ਖਾਨੇ_ਗਿਣਤੀ, CV_64F, simplex.ptr<ਦੁੱਗਣਾ>());
            simplex_0m.convert_ਮੰਜ਼ਿਲ(x, x.type());
        }
        ਨਹੀਂਤਾਂ
        {
            ਪੂਰਨ x_type = x_.fixedType() ? x_.type() : CV_64F;
            simplex.ਪੰਕਤੀ(0).convert_ਮੰਜ਼ਿਲ(x_, x_type);
        }
        ਵਾਪਸ res;
    }
ਸੁਰੱਖਿਅਤ:
    Ptr<MinProblemSolver::Function> _Function;
    ਪਦ_Criteria _termcrit;
    Mat ਕਦਮ_3;

    ਸਤਰਅੰਦਰ ਖਾਲੀ update_Coord_ਜੋੜ(ਅਚਲ Mat& p, Mat& coord_ਜੋੜ)
    {
        ਪੂਰਨ i, j, m = p.ਪੰਕਤੀਆਂ, n = p.ਖਾਨੇ_ਗਿਣਤੀ;
        ਦੁੱਗਣਾ* coord_ਜੋੜ_2 = coord_ਜੋੜ.ptr<ਦੁੱਗਣਾ>();
        CV_Assert( coord_ਜੋੜ.ਖਾਨੇ_ਗਿਣਤੀ == n && coord_ਜੋੜ.ਪੰਕਤੀਆਂ == 1 );

        ਲਈ( j = 0; j < n; j++ )
            coord_ਜੋੜ_2[j] = 0.;

        ਲਈ( i = 0; i < m; i++ )
        {
            ਅਚਲ ਦੁੱਗਣਾ* p_i = p.ptr<ਦੁੱਗਣਾ>(i);
            ਲਈ( j = 0; j < n; j++ )
                coord_ਜੋੜ_2[j] += p_i[j];
        }

        dprintf(("\nupdated coord sum:\n"));
        print_ਮੈਟ੍ਰਿਕਸ(coord_ਜੋੜ);

    }

    ਸਤਰਅੰਦਰ ਖਾਲੀ createInitialSimplex( ਅਚਲ Mat& x0, Mat& simplex, Mat& ਕਦਮ )
    {
        ਪੂਰਨ i, j, ndim = ਕਦਮ.ਖਾਨੇ_ਗਿਣਤੀ;
        CV_Assert( _Function->getDims() == ndim );
        Mat x = x0;
        ਜੇ( x0.empty() )
            x = Mat::zeros(1, ndim, CV_64F);
        CV_Assert( (x.ਖਾਨੇ_ਗਿਣਤੀ == 1 && x.ਪੰਕਤੀਆਂ == ndim) || (x.ਖਾਨੇ_ਗਿਣਤੀ == ndim && x.ਪੰਕਤੀਆਂ == 1) );
        CV_Assert( x.type() == CV_32F || x.type() == CV_64F );

        simplex.create(ndim + 1, ndim, CV_64F);
        Mat simplex_0m(x.ਪੰਕਤੀਆਂ, x.ਖਾਨੇ_ਗਿਣਤੀ, CV_64F, simplex.ptr<ਦੁੱਗਣਾ>());

        x.convert_ਮੰਜ਼ਿਲ(simplex_0m, CV_64F);
        ਦੁੱਗਣਾ* simplex_0 = simplex.ptr<ਦੁੱਗਣਾ>();
        ਅਚਲ ਦੁੱਗਣਾ* ਕਦਮ_2 = ਕਦਮ.ptr<ਦੁੱਗਣਾ>();
        ਲਈ( i = 1; i <= ndim; i++ )
        {
            ਦੁੱਗਣਾ* simplex_i = simplex.ptr<ਦੁੱਗਣਾ>(i);
            ਲਈ( j = 0; j < ndim; j++ )
                simplex_i[j] = simplex_0[j];
            simplex_i[i-1] += 0.5*ਕਦਮ_2[i-1];
        }
        ਲਈ( j = 0; j < ndim; j++ )
            simplex_0[j] -= 0.5*ਕਦਮ_2[j];

        dprintf(("\nthis is simplex\n"));
        print_ਮੈਟ੍ਰਿਕਸ(simplex);
    }

    /*
     Performs the actual minimization of MinProblemSolver::Function f (after the initialization was done)

     The matrix p[ndim+1][1..ndim] represents ndim+1 vertices that
     form a simplex - each row is an ndim vector.
     On output, fcount gives the number of function evaluations taken.
    */
    ਦੁੱਗਣਾ innerDownhillSimplex( Mat& p, ਦੁੱਗਣਾ MinRange, ਦੁੱਗਣਾ MinError, ਪੂਰਨ& fcount, ਪੂਰਨ nmax )
    {
        ਪੂਰਨ i, j, ndim = p.ਖਾਨੇ_ਗਿਣਤੀ;
        Mat coord_ਜੋੜ(1, ndim, CV_64F), buf(1, ndim, CV_64F), y(1, ndim+1, CV_64F);
        ਦੁੱਗਣਾ* y_ = y.ptr<ਦੁੱਗਣਾ>();

        fcount = ndim+1;
        ਲਈ( i = 0; i <= ndim; i++ )
            y_[i] = calc_f(p.ptr<ਦੁੱਗਣਾ>(i));

        update_Coord_ਜੋੜ(p, coord_ਜੋੜ);

        ਲਈ (;;)
        {
            // find highest (worst), next-to-worst, and lowest
            // (best) points by going through all of them.
            ਪੂਰਨ ilo = 0, ihi, inhi;
            ਜੇ( y_[0] > y_[1] )
            {
                ihi = 0; inhi = 1;
            }
            ਨਹੀਂਤਾਂ
            {
                ihi = 1; inhi = 0;
            }
            ਲਈ( i = 0; i <= ndim; i++ )
            {
                ਦੁੱਗਣਾ yval = y_[i];
                ਜੇ (yval <= y_[ilo])
                    ilo = i;
                ਜੇ (yval > y_[ihi])
                {
                    inhi = ihi;
                    ihi = i;
                }
                ਨਹੀਂਤਾਂ ਜੇ (yval > y_[inhi] && i != ihi)
                    inhi = i;
            }
            CV_Assert( ihi != inhi );
            ਜੇ( ilo == inhi || ilo == ihi )
            {
                ਲਈ( i = 0; i <= ndim; i++ )
                {
                    ਦੁੱਗਣਾ yval = y_[i];
                    ਜੇ( yval == y_[ilo] && i != ihi && i != inhi )
                    {
                        ilo = i;
                        ਰੋਕੋ;
                    }
                }
            }
            dprintf(("\nthis is y on iteration %d:\n",fcount));
            print_ਮੈਟ੍ਰਿਕਸ(y);

            // check stop criterion
            ਦੁੱਗਣਾ error = fabs(y_[ihi] - y_[ilo]);
            ਦੁੱਗਣਾ range = 0;
            ਲਈ( j = 0; j < ndim; j++ )
            {
                ਦੁੱਗਣਾ minval, maxval;
                minval = maxval = p.at<ਦੁੱਗਣਾ>(0, j);
                ਲਈ( i = 1; i <= ndim; i++ )
                {
                    ਦੁੱਗਣਾ pval = p.at<ਦੁੱਗਣਾ>(i, j);
                    minval = std::min(minval, pval);
                    maxval = std::max(maxval, pval);
                }
                range = std::max(range, fabs(maxval - minval));
            }

            ਜੇ( range <= MinRange || error <= MinError || fcount >= nmax )
            {
                // Put best point and value in first slot.
                std::swap(y_[0], y_[ilo]);
                ਲਈ( j = 0; j < ndim; j++ )
                {
                    std::swap(p.at<ਦੁੱਗਣਾ>(0, j), p.at<ਦੁੱਗਣਾ>(ilo, j));
                }
                ਰੋਕੋ;
            }

            ਦੁੱਗਣਾ y_lo = y_[ilo], y_nhi = y_[inhi], y_hi = y_[ihi];
            // Begin a new iteration. First, reflect the worst point about the centroid of others
            ਦੁੱਗਣਾ alpha = -1.0;
            ਦੁੱਗਣਾ y_alpha = try_New_ਬਿੰਦੂ(p, coord_ਜੋੜ, ihi, alpha, buf, fcount);

            dprintf(("\ny_lo=%g, y_nhi=%g, y_hi=%g, y_alpha=%g, p_alpha:\n", y_lo, y_nhi, y_hi, y_alpha));
            print_ਮੈਟ੍ਰਿਕਸ(buf);

            ਜੇ( y_alpha < y_nhi )
            {
                ਜੇ( y_alpha < y_lo )
                {
                    // If that's better than the best point, go twice as far in that direction
                    ਦੁੱਗਣਾ beta = -2.0;
                    ਦੁੱਗਣਾ y_beta = try_New_ਬਿੰਦੂ(p, coord_ਜੋੜ, ihi, beta, buf, fcount);
                    dprintf(("\ny_beta=%g, p_beta:\n", y_beta));
                    print_ਮੈਟ੍ਰਿਕਸ(buf);
                    ਜੇ( y_beta < y_alpha )
                    {
                        alpha = beta;
                        y_alpha = y_beta;
                    }
                }
                replace_ਬਿੰਦੂ(p, coord_ਜੋੜ, y, ihi, alpha, y_alpha);
            }
            ਨਹੀਂਤਾਂ
            {
                // The new point is worse than the second-highest,
                // do not go so far in that direction
                ਦੁੱਗਣਾ gamma = 0.5;
                ਦੁੱਗਣਾ y_gamma = try_New_ਬਿੰਦੂ(p, coord_ਜੋੜ, ihi, gamma, buf, fcount);
                dprintf(("\ny_gamma=%g, p_gamma:\n", y_gamma));
                print_ਮੈਟ੍ਰਿਕਸ(buf);
                ਜੇ( y_gamma < y_hi )
                    replace_ਬਿੰਦੂ(p, coord_ਜੋੜ, y, ihi, gamma, y_gamma);
                ਨਹੀਂਤਾਂ
                {
                    // Can't seem to improve things.
                    // Contract the simplex to good point
                    // in hope to find a simplex landscape.
                    ਲਈ( i = 0; i <= ndim; i++ )
                    {
                        ਜੇ (i != ilo)
                        {
                            ਲਈ( j = 0; j < ndim; j++ )
                                p.at<ਦੁੱਗਣਾ>(i, j) = 0.5*(p.at<ਦੁੱਗਣਾ>(i, j) + p.at<ਦੁੱਗਣਾ>(ilo, j));
                            y_[i] = calc_f(p.ptr<ਦੁੱਗਣਾ>(i));
                        }
                    }
                    fcount += ndim;
                    update_Coord_ਜੋੜ(p, coord_ਜੋੜ);
                }
            }
            dprintf(("\nthis is simplex on iteration %d\n",fcount));
            print_ਮੈਟ੍ਰਿਕਸ(p);
        }
        ਵਾਪਸ y_[0];
    }

    ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ calc_f(ਅਚਲ ਦੁੱਗਣਾ* ptr)
    {
        ਦੁੱਗਣਾ res = _Function->calc(ptr);
        CV_Assert( !cvIsNaN(res) && !cvIsInf(res) );
        ਵਾਪਸ res;
    }

    ਦੁੱਗਣਾ try_New_ਬਿੰਦੂ( Mat& p, Mat& coord_ਜੋੜ, ਪੂਰਨ ihi, ਦੁੱਗਣਾ alpha_, Mat& ptry, ਪੂਰਨ& fcount )
    {
        ਪੂਰਨ j, ndim = p.ਖਾਨੇ_ਗਿਣਤੀ;

        ਦੁੱਗਣਾ alpha = (1.0 - alpha_)/ndim;
        ਦੁੱਗਣਾ beta = alpha - alpha_;
        ਦੁੱਗਣਾ* p_ihi = p.ptr<ਦੁੱਗਣਾ>(ihi);
        ਦੁੱਗਣਾ* ptry_ = ptry.ptr<ਦੁੱਗਣਾ>();
        ਦੁੱਗਣਾ* coord_ਜੋੜ_2 = coord_ਜੋੜ.ptr<ਦੁੱਗਣਾ>();

        ਲਈ( j = 0; j < ndim; j++ )
            ptry_[j] = coord_ਜੋੜ_2[j]*alpha - p_ihi[j]*beta;

        fcount++;
        ਵਾਪਸ calc_f(ptry_);
    }

    ਖਾਲੀ replace_ਬਿੰਦੂ( Mat& p, Mat& coord_ਜੋੜ, Mat& y, ਪੂਰਨ ihi, ਦੁੱਗਣਾ alpha_, ਦੁੱਗਣਾ ytry )
    {
        ਪੂਰਨ j, ndim = p.ਖਾਨੇ_ਗਿਣਤੀ;

        ਦੁੱਗਣਾ alpha = (1.0 - alpha_)/ndim;
        ਦੁੱਗਣਾ beta = alpha - alpha_;
        ਦੁੱਗਣਾ* p_ihi = p.ptr<ਦੁੱਗਣਾ>(ihi);
        ਦੁੱਗਣਾ* coord_ਜੋੜ_2 = coord_ਜੋੜ.ptr<ਦੁੱਗਣਾ>();

        ਲਈ( j = 0; j < ndim; j++ )
            p_ihi[j] = coord_ਜੋੜ_2[j]*alpha - p_ihi[j]*beta;
        y.at<ਦੁੱਗਣਾ>(ihi) = ytry;
        update_Coord_ਜੋੜ(p, coord_ਜੋੜ);
    }
};


// both minRange & minError are specified by termcrit.epsilon;
// In addition, user may specify the number of iterations that the algorithm does.
Ptr<DownhillSolver> DownhillSolver::create( ਅਚਲ Ptr<MinProblemSolver::Function>& f,
                                            ਨਿਵੇਸ਼_Array init_ਕਦਮ, ਪਦ_Criteria termcrit )
{
    Ptr<DownhillSolver> DS = makePtr<DownhillSolverImpl>();
    DS->ਸੈੱਟ_Function(f);
    DS->ਸੈੱਟ_Init_ਕਦਮ(init_ਕਦਮ);
    DS->ਸੈੱਟ_ਪਦ_Criteria(termcrit);
    ਵਾਪਸ DS;
}

}
