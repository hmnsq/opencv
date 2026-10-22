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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#define dprintf(x)
#define print_matrix(x)

ନାମାକାଶ cv
{
    ଦ୍ୱିଗୁଣ MinProblemSolver::Function::getGradientEps() ଅଚଳ { ଫେରନ୍ତୁ 1e-3; }
    ଶୂନ୍ୟ MinProblemSolver::Function::getGradient(ଅଚଳ ଦ୍ୱିଗୁଣ* x, ଦ୍ୱିଗୁଣ* grad)
    {
        ଦ୍ୱିଗୁଣ eps = getGradientEps();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, n = getDims();
        AutoBuffer<ଦ୍ୱିଗୁଣ> x_buf(n);
        ଦ୍ୱିଗୁଣ* x_ = x_buf.data();
        ପାଇଁ( i = 0; i < n; i++ )
            x_[i] = x[i];
        ପାଇଁ( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            ଦ୍ୱିଗୁଣ y1 = calc(x_);
            x_[i] = x[i] - eps;
            ଦ୍ୱିଗୁଣ y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#define SEC_METHOD_ITERATIONS 4
#define INITIAL_SEC_METHOD_SIGMA 0.1
    ଶ୍ରେଣୀ ConjGradSolverImpl CV_FINAL : ସର୍ବସାଧାରଣ ConjGradSolver
    {
    ସର୍ବସାଧାରଣ:
        Ptr<Function> getFunction() ଅଚଳ CV_OVERRIDE;
        ଶୂନ୍ୟ setFunction(ଅଚଳ Ptr<Function>& f) CV_OVERRIDE;
        TermCriteria getTermCriteria() ଅଚଳ CV_OVERRIDE;
        ConjGradSolverImpl();
        ଶୂନ୍ୟ setTermCriteria(ଅଚଳ TermCriteria& termcrit) CV_OVERRIDE;
        ଦ୍ୱିଗୁଣ minimize(ନିବେଶ_Output_Array x) CV_OVERRIDE;
    ସୁରକ୍ଷିତ:
        Ptr<MinProblemSolver::Function> _Function;
        TermCriteria _termcrit;
        Mat_<ଦ୍ୱିଗୁଣ> d,r,buf_x,r_old;
        Mat_<ଦ୍ୱିଗୁଣ> minimize_On_The_ଧାଡ଼ି_buf1,minimize_On_The_ଧାଡ଼ି_buf2;
    ବ୍ୟକ୍ତିଗତ:
        ସ୍ଥିର ଶୂନ୍ୟ minimize_On_The_ଧାଡ଼ି(Ptr<MinProblemSolver::Function> _f,Mat_<ଦ୍ୱିଗୁଣ>& x,ଅଚଳ Mat_<ଦ୍ୱିଗୁଣ>& d,Mat_<ଦ୍ୱିଗୁଣ>& buf1,Mat_<ଦ୍ୱିଗୁଣ>& buf2);
    };

    ଶୂନ୍ୟ ConjGradSolverImpl::minimize_On_The_ଧାଡ଼ି(Ptr<MinProblemSolver::Function> _f,Mat_<ଦ୍ୱିଗୁଣ>& x,ଅଚଳ Mat_<ଦ୍ୱିଗୁଣ>& d,Mat_<ଦ୍ୱିଗୁଣ>& buf1,
            Mat_<ଦ୍ୱିଗୁଣ>& buf2){
        ଦ୍ୱିଗୁଣ sigma=INITIAL_SEC_METHOD_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_matrix(x);
        dprintf(("d:\n"));
        print_matrix(d);

        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0;i<SEC_METHOD_ITERATIONS;i++){
            _f->getGradient((ଦ୍ୱିଗୁଣ*)x.data,(ଦ୍ୱିଗୁଣ*)buf1.data);
            dprintf(("buf1:\n"));
            print_matrix(buf1);
            x=x+sigma*d;
            _f->getGradient((ଦ୍ୱିଗୁଣ*)x.data,(ଦ୍ୱିଗୁଣ*)buf2.data);
            dprintf(("buf2:\n"));
            print_matrix(buf2);
            ଦ୍ୱିଗୁଣ d1=buf1.dot(d), d2=buf2.dot(d);
            ଯଦି((d1-d2)==0){
                ଭାଙ୍ଗ;
            }
            ଦ୍ୱିଗୁଣ alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_matrix(x);
    }

    ଦ୍ୱିଗୁଣ ConjGradSolverImpl::minimize(ନିବେଶ_Output_Array x){
        CV_Assert(_Function.empty()==ମିଥ୍ୟା);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_ଗଣନା,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.rows,x_mat.cols)==1);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndim=MAX(x_mat.rows,x_mat.cols);
        CV_Assert(x_mat.type()==CV_64FC1);

        ଯଦି(d.cols!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_ଧାଡ଼ି_buf1.create(1,ndim);
            minimize_On_The_ଧାଡ଼ି_buf2.create(1,ndim);
        }

        Mat_<ଦ୍ୱିଗୁଣ> proxy_x;
        ଯଦି(x_mat.rows>1){
            buf_x.create(1,ndim);
            Mat_<ଦ୍ୱିଗୁଣ> proxy(ndim,1,buf_x.ptr<ଦ୍ୱିଗୁଣ>());
            x_mat.copyTo(proxy);
            proxy_x=buf_x;
        }ଅନ୍ୟଥା{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<ଦ୍ୱିଗୁଣ>(),d.ptr<ଦ୍ୱିଗୁଣ>());
        d*=-1.0;
        d.copyTo(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_matrix(proxy_x);
        dprintf(("d first time\n"));print_matrix(d);
        dprintf(("r\n"));print_matrix(r);

        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3=0;ଗଣନା_3<_termcrit.max_ଗଣନା;ଗଣନା_3++){
            minimize_On_The_ଧାଡ଼ି(_Function,proxy_x,d,minimize_On_The_ଧାଡ଼ି_buf1,minimize_On_The_ଧାଡ଼ି_buf2);
            r.copyTo(r_old);
            _Function->getGradient(proxy_x.ptr<ଦ୍ୱିଗୁଣ>(),r.ptr<ଦ୍ୱିଗୁଣ>());
            r*=-1.0;
            ଦ୍ୱିଗୁଣ r_norm_sq=norm(r);
            ଯଦି(_termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && r_norm_sq<_termcrit.epsilon){
                ଭାଙ୍ଗ;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            ଦ୍ୱିଗୁଣ beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        ଯଦି(x_mat.rows>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<ଦ୍ୱିଗୁଣ>()).copyTo(x);
        }
        ଫେରନ୍ତୁ _Function->calc(proxy_x.ptr<ଦ୍ୱିଗୁଣ>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()ଅଚଳ{
        ଫେରନ୍ତୁ _Function;
    }
    ଶୂନ୍ୟ ConjGradSolverImpl::setFunction(ଅଚଳ Ptr<Function>& f){
        _Function=f;
    }
    TermCriteria ConjGradSolverImpl::getTermCriteria()ଅଚଳ{
        ଫେରନ୍ତୁ _termcrit;
    }
    ଶୂନ୍ୟ ConjGradSolverImpl::setTermCriteria(ଅଚଳ TermCriteria& termcrit){
        CV_Assert((termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && termcrit.epsilon>0 && termcrit.max_ଗଣନା>0) ||
                ((termcrit.type==TermCriteria::MAX_ITER) && termcrit.max_ଗଣନା>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(ଅଚଳ Ptr<MinProblemSolver::Function>& f, TermCriteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->setFunction(f);
        CG->setTermCriteria(termcrit);
        ଫେରନ୍ତୁ CG;
    }
}
