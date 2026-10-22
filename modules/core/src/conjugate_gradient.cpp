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

#घोषणा dprintf(x)
#घोषणा print_matrix(x)

namespace cv
{
    द्विगुन MinProblemSolver::Function::getGradientEps() स्थिर { वापस 1e-3; }
    व्योम MinProblemSolver::Function::getGradient(स्थिर द्विगुन* x, द्विगुन* grad)
    {
        द्विगुन eps = getGradientEps();
        पूर्णांक i, n = getDims();
        Auto_बफर<द्विगुन> x_buf(n);
        द्विगुन* x_ = x_buf.तथ्य();
        क्रम( i = 0; i < n; i++ )
            x_[i] = x[i];
        क्रम( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            द्विगुन y1 = calc(x_);
            x_[i] = x[i] - eps;
            द्विगुन y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#घोषणा SEC_METHOD_ITERATIONS 4
#घोषणा INITIAL_SEC_METHOD_SIGMA 0.1
    श्रेणी ConjGradSolverImpl CV_FINAL : खुला ConjGradSolver
    {
    खुला:
        Ptr<Function> getFunction() स्थिर CV_OVERRIDE;
        व्योम setFunction(स्थिर Ptr<Function>& f) CV_OVERRIDE;
        TermCriteria getTermCriteria() स्थिर CV_OVERRIDE;
        ConjGradSolverImpl();
        व्योम setTermCriteria(स्थिर TermCriteria& termcrit) CV_OVERRIDE;
        द्विगुन minimize(निवेश_Output_Array x) CV_OVERRIDE;
    रक्षित:
        Ptr<MinProblemSolver::Function> _Function;
        TermCriteria _termcrit;
        Mat_<द्विगुन> d,r,buf_x,r_old;
        Mat_<द्विगुन> minimize_On_The_पंक्ति_buf1,minimize_On_The_पंक्ति_buf2;
    निजी:
        अटल व्योम minimize_On_The_पंक्ति(Ptr<MinProblemSolver::Function> _f,Mat_<द्विगुन>& x,स्थिर Mat_<द्विगुन>& d,Mat_<द्विगुन>& buf1,Mat_<द्विगुन>& buf2);
    };

    व्योम ConjGradSolverImpl::minimize_On_The_पंक्ति(Ptr<MinProblemSolver::Function> _f,Mat_<द्विगुन>& x,स्थिर Mat_<द्विगुन>& d,Mat_<द्विगुन>& buf1,
            Mat_<द्विगुन>& buf2){
        द्विगुन sigma=INITIAL_SEC_METHOD_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_matrix(x);
        dprintf(("d:\n"));
        print_matrix(d);

        क्रम(पूर्णांक i=0;i<SEC_METHOD_ITERATIONS;i++){
            _f->getGradient((द्विगुन*)x.तथ्य,(द्विगुन*)buf1.तथ्य);
            dprintf(("buf1:\n"));
            print_matrix(buf1);
            x=x+sigma*d;
            _f->getGradient((द्विगुन*)x.तथ्य,(द्विगुन*)buf2.तथ्य);
            dprintf(("buf2:\n"));
            print_matrix(buf2);
            द्विगुन d1=buf1.dot(d), d2=buf2.dot(d);
            अगर((d1-d2)==0){
                अवरोध;
            }
            द्विगुन alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_matrix(x);
    }

    द्विगुन ConjGradSolverImpl::minimize(निवेश_Output_Array x){
        CV_Assert(_Function.empty()==false);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_गिनती,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.rows,x_mat.cols)==1);
        पूर्णांक ndim=MAX(x_mat.rows,x_mat.cols);
        CV_Assert(x_mat.type()==CV_64FC1);

        अगर(d.cols!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_पंक्ति_buf1.create(1,ndim);
            minimize_On_The_पंक्ति_buf2.create(1,ndim);
        }

        Mat_<द्विगुन> proxy_x;
        अगर(x_mat.rows>1){
            buf_x.create(1,ndim);
            Mat_<द्विगुन> proxy(ndim,1,buf_x.ptr<द्विगुन>());
            x_mat.copyTo(proxy);
            proxy_x=buf_x;
        }अन्यथा{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<द्विगुन>(),d.ptr<द्विगुन>());
        d*=-1.0;
        d.copyTo(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_matrix(proxy_x);
        dprintf(("d first time\n"));print_matrix(d);
        dprintf(("r\n"));print_matrix(r);

        क्रम(पूर्णांक गिनती_2=0;गिनती_2<_termcrit.max_गिनती;गिनती_2++){
            minimize_On_The_पंक्ति(_Function,proxy_x,d,minimize_On_The_पंक्ति_buf1,minimize_On_The_पंक्ति_buf2);
            r.copyTo(r_old);
            _Function->getGradient(proxy_x.ptr<द्विगुन>(),r.ptr<द्विगुन>());
            r*=-1.0;
            द्विगुन r_norm_sq=norm(r);
            अगर(_termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && r_norm_sq<_termcrit.epsilon){
                अवरोध;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            द्विगुन beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        अगर(x_mat.rows>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<द्विगुन>()).copyTo(x);
        }
        वापस _Function->calc(proxy_x.ptr<द्विगुन>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()स्थिर{
        वापस _Function;
    }
    व्योम ConjGradSolverImpl::setFunction(स्थिर Ptr<Function>& f){
        _Function=f;
    }
    TermCriteria ConjGradSolverImpl::getTermCriteria()स्थिर{
        वापस _termcrit;
    }
    व्योम ConjGradSolverImpl::setTermCriteria(स्थिर TermCriteria& termcrit){
        CV_Assert((termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && termcrit.epsilon>0 && termcrit.max_गिनती>0) ||
                ((termcrit.type==TermCriteria::MAX_ITER) && termcrit.max_गिनती>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(स्थिर Ptr<MinProblemSolver::Function>& f, TermCriteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->setFunction(f);
        CG->setTermCriteria(termcrit);
        वापस CG;
    }
}
