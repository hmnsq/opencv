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

#تعریف_کرو dprintf(x)
#تعریف_کرو print_میٹرکس(x)

نامخلا cv
{
    دوہرا MinProblemSolver::Function::getGradientEps() ثابت { واپس 1e-3; }
    خالی MinProblemSolver::Function::getGradient(ثابت دوہرا* x, دوہرا* grad)
    {
        دوہرا eps = getGradientEps();
        صحیح i, n = getDims();
        Auto_بفر<دوہرا> x_buf(n);
        دوہرا* x_ = x_buf.ڈیٹا();
        برائے( i = 0; i < n; i++ )
            x_[i] = x[i];
        برائے( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            دوہرا y1 = calc(x_);
            x_[i] = x[i] - eps;
            دوہرا y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#تعریف_کرو SEC_طریقہ_ITERATIONS 4
#تعریف_کرو INITIAL_SEC_طریقہ_SIGMA 0.1
    جماعت ConjGradSolverImpl CV_FINAL : عوامی ConjGradSolver
    {
    عوامی:
        Ptr<Function> getFunction() ثابت CV_OVERRIDE;
        خالی سیٹ_Function(ثابت Ptr<Function>& f) CV_OVERRIDE;
        رکن_Criteria get_رکن_Criteria() ثابت CV_OVERRIDE;
        ConjGradSolverImpl();
        خالی سیٹ_رکن_Criteria(ثابت رکن_Criteria& termcrit) CV_OVERRIDE;
        دوہرا minimize(داخلہ_Output_Array x) CV_OVERRIDE;
    محفوظ:
        Ptr<MinProblemSolver::Function> _Function;
        رکن_Criteria _termcrit;
        Mat_<دوہرا> d,r,buf_x,r_old;
        Mat_<دوہرا> minimize_On_The_سطر_buf1,minimize_On_The_سطر_buf2;
    نجی:
        ساکن خالی minimize_On_The_سطر(Ptr<MinProblemSolver::Function> _f,Mat_<دوہرا>& x,ثابت Mat_<دوہرا>& d,Mat_<دوہرا>& buf1,Mat_<دوہرا>& buf2);
    };

    خالی ConjGradSolverImpl::minimize_On_The_سطر(Ptr<MinProblemSolver::Function> _f,Mat_<دوہرا>& x,ثابت Mat_<دوہرا>& d,Mat_<دوہرا>& buf1,
            Mat_<دوہرا>& buf2){
        دوہرا sigma=INITIAL_SEC_طریقہ_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_میٹرکس(x);
        dprintf(("d:\n"));
        print_میٹرکس(d);

        برائے(صحیح i=0;i<SEC_طریقہ_ITERATIONS;i++){
            _f->getGradient((دوہرا*)x.ڈیٹا,(دوہرا*)buf1.ڈیٹا);
            dprintf(("buf1:\n"));
            print_میٹرکس(buf1);
            x=x+sigma*d;
            _f->getGradient((دوہرا*)x.ڈیٹا,(دوہرا*)buf2.ڈیٹا);
            dprintf(("buf2:\n"));
            print_میٹرکس(buf2);
            دوہرا d1=buf1.dot(d), d2=buf2.dot(d);
            اگر((d1-d2)==0){
                توڑو;
            }
            دوہرا alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_میٹرکس(x);
    }

    دوہرا ConjGradSolverImpl::minimize(داخلہ_Output_Array x){
        CV_Assert(_Function.empty()==جھوٹ);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_گنتی,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.صفیں,x_mat.ستون_تعداد)==1);
        صحیح ndim=MAX(x_mat.صفیں,x_mat.ستون_تعداد);
        CV_Assert(x_mat.type()==CV_64FC1);

        اگر(d.ستون_تعداد!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_سطر_buf1.create(1,ndim);
            minimize_On_The_سطر_buf2.create(1,ndim);
        }

        Mat_<دوہرا> proxy_x;
        اگر(x_mat.صفیں>1){
            buf_x.create(1,ndim);
            Mat_<دوہرا> proxy(ndim,1,buf_x.ptr<دوہرا>());
            x_mat.copy_مقصد(proxy);
            proxy_x=buf_x;
        }ورنہ{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<دوہرا>(),d.ptr<دوہرا>());
        d*=-1.0;
        d.copy_مقصد(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_میٹرکس(proxy_x);
        dprintf(("d first time\n"));print_میٹرکس(d);
        dprintf(("r\n"));print_میٹرکس(r);

        برائے(صحیح گنتی_2=0;گنتی_2<_termcrit.max_گنتی;گنتی_2++){
            minimize_On_The_سطر(_Function,proxy_x,d,minimize_On_The_سطر_buf1,minimize_On_The_سطر_buf2);
            r.copy_مقصد(r_old);
            _Function->getGradient(proxy_x.ptr<دوہرا>(),r.ptr<دوہرا>());
            r*=-1.0;
            دوہرا r_norm_sq=norm(r);
            اگر(_termcrit.type==(رکن_Criteria::MAX_ITER+رکن_Criteria::EPS) && r_norm_sq<_termcrit.epsilon){
                توڑو;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            دوہرا beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        اگر(x_mat.صفیں>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<دوہرا>()).copy_مقصد(x);
        }
        واپس _Function->calc(proxy_x.ptr<دوہرا>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()ثابت{
        واپس _Function;
    }
    خالی ConjGradSolverImpl::سیٹ_Function(ثابت Ptr<Function>& f){
        _Function=f;
    }
    رکن_Criteria ConjGradSolverImpl::get_رکن_Criteria()ثابت{
        واپس _termcrit;
    }
    خالی ConjGradSolverImpl::سیٹ_رکن_Criteria(ثابت رکن_Criteria& termcrit){
        CV_Assert((termcrit.type==(رکن_Criteria::MAX_ITER+رکن_Criteria::EPS) && termcrit.epsilon>0 && termcrit.max_گنتی>0) ||
                ((termcrit.type==رکن_Criteria::MAX_ITER) && termcrit.max_گنتی>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(ثابت Ptr<MinProblemSolver::Function>& f, رکن_Criteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->سیٹ_Function(f);
        CG->سیٹ_رکن_Criteria(termcrit);
        واپس CG;
    }
}
