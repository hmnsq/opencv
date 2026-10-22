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

#અન્તર્ભાવ "precomp.hpp"

#define dprintf(x)
#define print_matrix(x)

નામાકાશ cv
{
    દ્વિગુણ MinProblemSolver::Function::getGradientEps() અચલ { પાછા 1e-3; }
    શૂન્ય MinProblemSolver::Function::getGradient(અચલ દ્વિગુણ* x, દ્વિગુણ* grad)
    {
        દ્વિગુણ eps = getGradientEps();
        પૂર્ણાંક i, n = getDims();
        AutoBuffer<દ્વિગુણ> x_buf(n);
        દ્વિગુણ* x_ = x_buf.data();
        માટે( i = 0; i < n; i++ )
            x_[i] = x[i];
        માટે( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            દ્વિગુણ y1 = calc(x_);
            x_[i] = x[i] - eps;
            દ્વિગુણ y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#define SEC_METHOD_ITERATIONS 4
#define INITIAL_SEC_METHOD_SIGMA 0.1
    વર્ગ ConjGradSolverImpl CV_FINAL : જાહેર ConjGradSolver
    {
    જાહેર:
        Ptr<Function> getFunction() અચલ CV_OVERRIDE;
        શૂન્ય setFunction(અચલ Ptr<Function>& f) CV_OVERRIDE;
        TermCriteria getTermCriteria() અચલ CV_OVERRIDE;
        ConjGradSolverImpl();
        શૂન્ય setTermCriteria(અચલ TermCriteria& termcrit) CV_OVERRIDE;
        દ્વિગુણ minimize(આગત_Output_Array x) CV_OVERRIDE;
    સુરક્ષિત:
        Ptr<MinProblemSolver::Function> _Function;
        TermCriteria _termcrit;
        Mat_<દ્વિગુણ> d,r,buf_x,r_old;
        Mat_<દ્વિગુણ> minimize_On_The_લીટી_buf1,minimize_On_The_લીટી_buf2;
    ખાનગી:
        સ્થિર શૂન્ય minimize_On_The_લીટી(Ptr<MinProblemSolver::Function> _f,Mat_<દ્વિગુણ>& x,અચલ Mat_<દ્વિગુણ>& d,Mat_<દ્વિગુણ>& buf1,Mat_<દ્વિગુણ>& buf2);
    };

    શૂન્ય ConjGradSolverImpl::minimize_On_The_લીટી(Ptr<MinProblemSolver::Function> _f,Mat_<દ્વિગુણ>& x,અચલ Mat_<દ્વિગુણ>& d,Mat_<દ્વિગુણ>& buf1,
            Mat_<દ્વિગુણ>& buf2){
        દ્વિગુણ sigma=INITIAL_SEC_METHOD_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_matrix(x);
        dprintf(("d:\n"));
        print_matrix(d);

        માટે(પૂર્ણાંક i=0;i<SEC_METHOD_ITERATIONS;i++){
            _f->getGradient((દ્વિગુણ*)x.data,(દ્વિગુણ*)buf1.data);
            dprintf(("buf1:\n"));
            print_matrix(buf1);
            x=x+sigma*d;
            _f->getGradient((દ્વિગુણ*)x.data,(દ્વિગુણ*)buf2.data);
            dprintf(("buf2:\n"));
            print_matrix(buf2);
            દ્વિગુણ d1=buf1.dot(d), d2=buf2.dot(d);
            જો((d1-d2)==0){
                તોડો;
            }
            દ્વિગુણ alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_matrix(x);
    }

    દ્વિગુણ ConjGradSolverImpl::minimize(આગત_Output_Array x){
        CV_Assert(_Function.empty()==ખોટું);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_ગણતરી,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.rows,x_mat.cols)==1);
        પૂર્ણાંક ndim=MAX(x_mat.rows,x_mat.cols);
        CV_Assert(x_mat.type()==CV_64FC1);

        જો(d.cols!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_લીટી_buf1.create(1,ndim);
            minimize_On_The_લીટી_buf2.create(1,ndim);
        }

        Mat_<દ્વિગુણ> proxy_x;
        જો(x_mat.rows>1){
            buf_x.create(1,ndim);
            Mat_<દ્વિગુણ> proxy(ndim,1,buf_x.ptr<દ્વિગુણ>());
            x_mat.copyTo(proxy);
            proxy_x=buf_x;
        }નહીંતર{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<દ્વિગુણ>(),d.ptr<દ્વિગુણ>());
        d*=-1.0;
        d.copyTo(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_matrix(proxy_x);
        dprintf(("d first time\n"));print_matrix(d);
        dprintf(("r\n"));print_matrix(r);

        માટે(પૂર્ણાંક ગણતરી_2=0;ગણતરી_2<_termcrit.max_ગણતરી;ગણતરી_2++){
            minimize_On_The_લીટી(_Function,proxy_x,d,minimize_On_The_લીટી_buf1,minimize_On_The_લીટી_buf2);
            r.copyTo(r_old);
            _Function->getGradient(proxy_x.ptr<દ્વિગુણ>(),r.ptr<દ્વિગુણ>());
            r*=-1.0;
            દ્વિગુણ r_norm_sq=norm(r);
            જો(_termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && r_norm_sq<_termcrit.epsilon){
                તોડો;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            દ્વિગુણ beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        જો(x_mat.rows>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<દ્વિગુણ>()).copyTo(x);
        }
        પાછા _Function->calc(proxy_x.ptr<દ્વિગુણ>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()અચલ{
        પાછા _Function;
    }
    શૂન્ય ConjGradSolverImpl::setFunction(અચલ Ptr<Function>& f){
        _Function=f;
    }
    TermCriteria ConjGradSolverImpl::getTermCriteria()અચલ{
        પાછા _termcrit;
    }
    શૂન્ય ConjGradSolverImpl::setTermCriteria(અચલ TermCriteria& termcrit){
        CV_Assert((termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && termcrit.epsilon>0 && termcrit.max_ગણતરી>0) ||
                ((termcrit.type==TermCriteria::MAX_ITER) && termcrit.max_ગણતરી>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(અચલ Ptr<MinProblemSolver::Function>& f, TermCriteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->setFunction(f);
        CG->setTermCriteria(termcrit);
        પાછા CG;
    }
}
