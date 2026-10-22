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

#அந்தர்பாவ "precomp.hpp"

#define dprintf(x)
#define print_matrix(x)

நாமாகாஶ cv
{
    இரட்டை MinProblemSolver::Function::getGradientEps() மாறிலி { திரும்பு 1e-3; }
    வெறுமை MinProblemSolver::Function::getGradient(மாறிலி இரட்டை* x, இரட்டை* grad)
    {
        இரட்டை eps = getGradientEps();
        முழு i, n = getDims();
        AutoBuffer<இரட்டை> x_buf(n);
        இரட்டை* x_ = x_buf.data();
        க்கு( i = 0; i < n; i++ )
            x_[i] = x[i];
        க்கு( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            இரட்டை y1 = calc(x_);
            x_[i] = x[i] - eps;
            இரட்டை y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#define SEC_METHOD_ITERATIONS 4
#define INITIAL_SEC_METHOD_SIGMA 0.1
    வகுப்பு ConjGradSolverImpl CV_FINAL : பொது ConjGradSolver
    {
    பொது:
        Ptr<Function> getFunction() மாறிலி CV_OVERRIDE;
        வெறுமை setFunction(மாறிலி Ptr<Function>& f) CV_OVERRIDE;
        TermCriteria getTermCriteria() மாறிலி CV_OVERRIDE;
        ConjGradSolverImpl();
        வெறுமை setTermCriteria(மாறிலி TermCriteria& termcrit) CV_OVERRIDE;
        இரட்டை minimize(உள்ளீடு_Output_Array x) CV_OVERRIDE;
    பாதுகாக்கப்பட்ட:
        Ptr<MinProblemSolver::Function> _Function;
        TermCriteria _termcrit;
        Mat_<இரட்டை> d,r,buf_x,r_old;
        Mat_<இரட்டை> minimize_On_The_வரி_buf1,minimize_On_The_வரி_buf2;
    தனிப்பட்ட:
        நிலையான வெறுமை minimize_On_The_வரி(Ptr<MinProblemSolver::Function> _f,Mat_<இரட்டை>& x,மாறிலி Mat_<இரட்டை>& d,Mat_<இரட்டை>& buf1,Mat_<இரட்டை>& buf2);
    };

    வெறுமை ConjGradSolverImpl::minimize_On_The_வரி(Ptr<MinProblemSolver::Function> _f,Mat_<இரட்டை>& x,மாறிலி Mat_<இரட்டை>& d,Mat_<இரட்டை>& buf1,
            Mat_<இரட்டை>& buf2){
        இரட்டை sigma=INITIAL_SEC_METHOD_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_matrix(x);
        dprintf(("d:\n"));
        print_matrix(d);

        க்கு(முழு i=0;i<SEC_METHOD_ITERATIONS;i++){
            _f->getGradient((இரட்டை*)x.data,(இரட்டை*)buf1.data);
            dprintf(("buf1:\n"));
            print_matrix(buf1);
            x=x+sigma*d;
            _f->getGradient((இரட்டை*)x.data,(இரட்டை*)buf2.data);
            dprintf(("buf2:\n"));
            print_matrix(buf2);
            இரட்டை d1=buf1.dot(d), d2=buf2.dot(d);
            எனில்((d1-d2)==0){
                நிறுத்து;
            }
            இரட்டை alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_matrix(x);
    }

    இரட்டை ConjGradSolverImpl::minimize(உள்ளீடு_Output_Array x){
        CV_Assert(_Function.empty()==பொய்);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_மொத்தம்,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.rows,x_mat.cols)==1);
        முழு ndim=MAX(x_mat.rows,x_mat.cols);
        CV_Assert(x_mat.type()==CV_64FC1);

        எனில்(d.cols!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_வரி_buf1.create(1,ndim);
            minimize_On_The_வரி_buf2.create(1,ndim);
        }

        Mat_<இரட்டை> proxy_x;
        எனில்(x_mat.rows>1){
            buf_x.create(1,ndim);
            Mat_<இரட்டை> proxy(ndim,1,buf_x.ptr<இரட்டை>());
            x_mat.copyTo(proxy);
            proxy_x=buf_x;
        }இல்லையெனில்{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<இரட்டை>(),d.ptr<இரட்டை>());
        d*=-1.0;
        d.copyTo(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_matrix(proxy_x);
        dprintf(("d first time\n"));print_matrix(d);
        dprintf(("r\n"));print_matrix(r);

        க்கு(முழு மொத்தம்_2=0;மொத்தம்_2<_termcrit.max_மொத்தம்;மொத்தம்_2++){
            minimize_On_The_வரி(_Function,proxy_x,d,minimize_On_The_வரி_buf1,minimize_On_The_வரி_buf2);
            r.copyTo(r_old);
            _Function->getGradient(proxy_x.ptr<இரட்டை>(),r.ptr<இரட்டை>());
            r*=-1.0;
            இரட்டை r_norm_sq=norm(r);
            எனில்(_termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && r_norm_sq<_termcrit.epsilon){
                நிறுத்து;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            இரட்டை beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        எனில்(x_mat.rows>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<இரட்டை>()).copyTo(x);
        }
        திரும்பு _Function->calc(proxy_x.ptr<இரட்டை>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()மாறிலி{
        திரும்பு _Function;
    }
    வெறுமை ConjGradSolverImpl::setFunction(மாறிலி Ptr<Function>& f){
        _Function=f;
    }
    TermCriteria ConjGradSolverImpl::getTermCriteria()மாறிலி{
        திரும்பு _termcrit;
    }
    வெறுமை ConjGradSolverImpl::setTermCriteria(மாறிலி TermCriteria& termcrit){
        CV_Assert((termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && termcrit.epsilon>0 && termcrit.max_மொத்தம்>0) ||
                ((termcrit.type==TermCriteria::MAX_ITER) && termcrit.max_மொத்தம்>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(மாறிலி Ptr<MinProblemSolver::Function>& f, TermCriteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->setFunction(f);
        CG->setTermCriteria(termcrit);
        திரும்பு CG;
    }
}
