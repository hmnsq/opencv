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

#ضمن "precomp.hpp"

#تعریف_کن dprintf(x)
#تعریف_کن print_ماتریس(x)

نطاق cv
{
    مزدوج MinProblemSolver::Function::getGradientEps() ثابت { أرجع 1e-3; }
    فراغ MinProblemSolver::Function::getGradient(ثابت مزدوج* x, مزدوج* grad)
    {
        مزدوج eps = getGradientEps();
        صحيح i, n = getDims();
        Auto_میانگیر<مزدوج> x_buf(n);
        مزدوج* x_ = x_buf.داده();
        لكل( i = 0; i < n; i++ )
            x_[i] = x[i];
        لكل( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            مزدوج y1 = calc(x_);
            x_[i] = x[i] - eps;
            مزدوج y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#تعریف_کن SEC_روش_ITERATIONS 4
#تعریف_کن INITIAL_SEC_روش_SIGMA 0.1
    صنف ConjGradSolverImpl CV_FINAL : عام ConjGradSolver
    {
    عام:
        Ptr<Function> getFunction() ثابت CV_OVERRIDE;
        فراغ مجموعه_Function(ثابت Ptr<Function>& f) CV_OVERRIDE;
        جمله_Criteria get_جمله_Criteria() ثابت CV_OVERRIDE;
        ConjGradSolverImpl();
        فراغ مجموعه_جمله_Criteria(ثابت جمله_Criteria& termcrit) CV_OVERRIDE;
        مزدوج minimize(درون_داد_Output_Array x) CV_OVERRIDE;
    محمي:
        Ptr<MinProblemSolver::Function> _Function;
        جمله_Criteria _termcrit;
        Mat_<مزدوج> d,r,buf_x,r_old;
        Mat_<مزدوج> minimize_On_The_سطر_buf1,minimize_On_The_سطر_buf2;
    خاص:
        ساكن فراغ minimize_On_The_سطر(Ptr<MinProblemSolver::Function> _f,Mat_<مزدوج>& x,ثابت Mat_<مزدوج>& d,Mat_<مزدوج>& buf1,Mat_<مزدوج>& buf2);
    };

    فراغ ConjGradSolverImpl::minimize_On_The_سطر(Ptr<MinProblemSolver::Function> _f,Mat_<مزدوج>& x,ثابت Mat_<مزدوج>& d,Mat_<مزدوج>& buf1,
            Mat_<مزدوج>& buf2){
        مزدوج sigma=INITIAL_SEC_روش_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_ماتریس(x);
        dprintf(("d:\n"));
        print_ماتریس(d);

        لكل(صحيح i=0;i<SEC_روش_ITERATIONS;i++){
            _f->getGradient((مزدوج*)x.داده,(مزدوج*)buf1.داده);
            dprintf(("buf1:\n"));
            print_ماتریس(buf1);
            x=x+sigma*d;
            _f->getGradient((مزدوج*)x.داده,(مزدوج*)buf2.داده);
            dprintf(("buf2:\n"));
            print_ماتریس(buf2);
            مزدوج d1=buf1.dot(d), d2=buf2.dot(d);
            إذا((d1-d2)==0){
                اكسر;
            }
            مزدوج alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_ماتریس(x);
    }

    مزدوج ConjGradSolverImpl::minimize(درون_داد_Output_Array x){
        CV_Assert(_Function.empty()==خطأ);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_شمار,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.ردیفها,x_mat.تعداد_ستون)==1);
        صحيح ndim=MAX(x_mat.ردیفها,x_mat.تعداد_ستون);
        CV_Assert(x_mat.type()==CV_64FC1);

        إذا(d.تعداد_ستون!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_سطر_buf1.create(1,ndim);
            minimize_On_The_سطر_buf2.create(1,ndim);
        }

        Mat_<مزدوج> proxy_x;
        إذا(x_mat.ردیفها>1){
            buf_x.create(1,ndim);
            Mat_<مزدوج> proxy(ndim,1,buf_x.ptr<مزدوج>());
            x_mat.copy_مقصد(proxy);
            proxy_x=buf_x;
        }وإلا{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<مزدوج>(),d.ptr<مزدوج>());
        d*=-1.0;
        d.copy_مقصد(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_ماتریس(proxy_x);
        dprintf(("d first time\n"));print_ماتریس(d);
        dprintf(("r\n"));print_ماتریس(r);

        لكل(صحيح شمار_2=0;شمار_2<_termcrit.max_شمار;شمار_2++){
            minimize_On_The_سطر(_Function,proxy_x,d,minimize_On_The_سطر_buf1,minimize_On_The_سطر_buf2);
            r.copy_مقصد(r_old);
            _Function->getGradient(proxy_x.ptr<مزدوج>(),r.ptr<مزدوج>());
            r*=-1.0;
            مزدوج r_norm_sq=norm(r);
            إذا(_termcrit.type==(جمله_Criteria::MAX_ITER+جمله_Criteria::EPS) && r_norm_sq<_termcrit.epsilon){
                اكسر;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            مزدوج beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        إذا(x_mat.ردیفها>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<مزدوج>()).copy_مقصد(x);
        }
        أرجع _Function->calc(proxy_x.ptr<مزدوج>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()ثابت{
        أرجع _Function;
    }
    فراغ ConjGradSolverImpl::مجموعه_Function(ثابت Ptr<Function>& f){
        _Function=f;
    }
    جمله_Criteria ConjGradSolverImpl::get_جمله_Criteria()ثابت{
        أرجع _termcrit;
    }
    فراغ ConjGradSolverImpl::مجموعه_جمله_Criteria(ثابت جمله_Criteria& termcrit){
        CV_Assert((termcrit.type==(جمله_Criteria::MAX_ITER+جمله_Criteria::EPS) && termcrit.epsilon>0 && termcrit.max_شمار>0) ||
                ((termcrit.type==جمله_Criteria::MAX_ITER) && termcrit.max_شمار>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(ثابت Ptr<MinProblemSolver::Function>& f, جمله_Criteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->مجموعه_Function(f);
        CG->مجموعه_جمله_Criteria(termcrit);
        أرجع CG;
    }
}
