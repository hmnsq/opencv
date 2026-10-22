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

#define dprintf(x)
#define print_matrix(x)

فضاینام cv
{
    دوگانه MinProblemSolver::Function::getGradientEps() ثابت { بازگشت 1e-3; }
    تهی MinProblemSolver::Function::getGradient(ثابت دوگانه* x, دوگانه* grad)
    {
        دوگانه eps = getGradientEps();
        صحیح i, n = getDims();
        AutoBuffer<دوگانه> x_buf(n);
        دوگانه* x_ = x_buf.data();
        برای( i = 0; i < n; i++ )
            x_[i] = x[i];
        برای( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            دوگانه y1 = calc(x_);
            x_[i] = x[i] - eps;
            دوگانه y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#define SEC_METHOD_ITERATIONS 4
#define INITIAL_SEC_METHOD_SIGMA 0.1
    رده ConjGradSolverImpl CV_FINAL : عمومی ConjGradSolver
    {
    عمومی:
        Ptr<Function> getFunction() ثابت CV_OVERRIDE;
        تهی setFunction(ثابت Ptr<Function>& f) CV_OVERRIDE;
        TermCriteria getTermCriteria() ثابت CV_OVERRIDE;
        ConjGradSolverImpl();
        تهی setTermCriteria(ثابت TermCriteria& termcrit) CV_OVERRIDE;
        دوگانه minimize(درون_داد_Output_Array x) CV_OVERRIDE;
    محافظتشده:
        Ptr<MinProblemSolver::Function> _Function;
        TermCriteria _termcrit;
        Mat_<دوگانه> d,r,buf_x,r_old;
        Mat_<دوگانه> minimize_On_The_سطر_buf1,minimize_On_The_سطر_buf2;
    خصوصی:
        ایستا تهی minimize_On_The_سطر(Ptr<MinProblemSolver::Function> _f,Mat_<دوگانه>& x,ثابت Mat_<دوگانه>& d,Mat_<دوگانه>& buf1,Mat_<دوگانه>& buf2);
    };

    تهی ConjGradSolverImpl::minimize_On_The_سطر(Ptr<MinProblemSolver::Function> _f,Mat_<دوگانه>& x,ثابت Mat_<دوگانه>& d,Mat_<دوگانه>& buf1,
            Mat_<دوگانه>& buf2){
        دوگانه sigma=INITIAL_SEC_METHOD_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_matrix(x);
        dprintf(("d:\n"));
        print_matrix(d);

        برای(صحیح i=0;i<SEC_METHOD_ITERATIONS;i++){
            _f->getGradient((دوگانه*)x.data,(دوگانه*)buf1.data);
            dprintf(("buf1:\n"));
            print_matrix(buf1);
            x=x+sigma*d;
            _f->getGradient((دوگانه*)x.data,(دوگانه*)buf2.data);
            dprintf(("buf2:\n"));
            print_matrix(buf2);
            دوگانه d1=buf1.dot(d), d2=buf2.dot(d);
            اگر((d1-d2)==0){
                بشکن;
            }
            دوگانه alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_matrix(x);
    }

    دوگانه ConjGradSolverImpl::minimize(درون_داد_Output_Array x){
        CV_Assert(_Function.empty()==نادرست);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_شمار,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.rows,x_mat.cols)==1);
        صحیح ndim=MAX(x_mat.rows,x_mat.cols);
        CV_Assert(x_mat.type()==CV_64FC1);

        اگر(d.cols!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_سطر_buf1.create(1,ndim);
            minimize_On_The_سطر_buf2.create(1,ndim);
        }

        Mat_<دوگانه> proxy_x;
        اگر(x_mat.rows>1){
            buf_x.create(1,ndim);
            Mat_<دوگانه> proxy(ndim,1,buf_x.ptr<دوگانه>());
            x_mat.copyTo(proxy);
            proxy_x=buf_x;
        }وگرنه{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<دوگانه>(),d.ptr<دوگانه>());
        d*=-1.0;
        d.copyTo(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_matrix(proxy_x);
        dprintf(("d first time\n"));print_matrix(d);
        dprintf(("r\n"));print_matrix(r);

        برای(صحیح شمار_2=0;شمار_2<_termcrit.max_شمار;شمار_2++){
            minimize_On_The_سطر(_Function,proxy_x,d,minimize_On_The_سطر_buf1,minimize_On_The_سطر_buf2);
            r.copyTo(r_old);
            _Function->getGradient(proxy_x.ptr<دوگانه>(),r.ptr<دوگانه>());
            r*=-1.0;
            دوگانه r_norm_sq=norm(r);
            اگر(_termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && r_norm_sq<_termcrit.epsilon){
                بشکن;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            دوگانه beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        اگر(x_mat.rows>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<دوگانه>()).copyTo(x);
        }
        بازگشت _Function->calc(proxy_x.ptr<دوگانه>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()ثابت{
        بازگشت _Function;
    }
    تهی ConjGradSolverImpl::setFunction(ثابت Ptr<Function>& f){
        _Function=f;
    }
    TermCriteria ConjGradSolverImpl::getTermCriteria()ثابت{
        بازگشت _termcrit;
    }
    تهی ConjGradSolverImpl::setTermCriteria(ثابت TermCriteria& termcrit){
        CV_Assert((termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && termcrit.epsilon>0 && termcrit.max_شمار>0) ||
                ((termcrit.type==TermCriteria::MAX_ITER) && termcrit.max_شمار>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(ثابت Ptr<MinProblemSolver::Function>& f, TermCriteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->setFunction(f);
        CG->setTermCriteria(termcrit);
        بازگشت CG;
    }
}
