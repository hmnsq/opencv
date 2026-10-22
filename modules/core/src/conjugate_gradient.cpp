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

#अन्तर्भाव "precomp.hpp"

#परिभाषय dprintf(x)
#परिभाषय print_matrix(x)

नामाकाश cv
{
    द्विगुण MinProblemSolver::Function::getGradientEps() ध्रुव { प्रत्यागम 1e-3; }
    शून्य MinProblemSolver::Function::getGradient(ध्रुव द्विगुण* x, द्विगुण* grad)
    {
        द्विगुण eps = getGradientEps();
        पूर्णाङ्क i, n = getDims();
        Auto_बफर<द्विगुण> x_buf(n);
        द्विगुण* x_ = x_buf.दत्त();
        प्रति( i = 0; i < n; i++ )
            x_[i] = x[i];
        प्रति( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            द्विगुण y1 = calc(x_);
            x_[i] = x[i] - eps;
            द्विगुण y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#परिभाषय SEC_विधानम्_ITERATIONS 4
#परिभाषय INITIAL_SEC_विधानम्_SIGMA 0.1
    वर्ग ConjGradSolverImpl CV_FINAL : सार्वजनिक ConjGradSolver
    {
    सार्वजनिक:
        Ptr<Function> getFunction() ध्रुव CV_OVERRIDE;
        शून्य समुच्चयः_Function(ध्रुव Ptr<Function>& f) CV_OVERRIDE;
        पदांशः_Criteria get_पदांशः_Criteria() ध्रुव CV_OVERRIDE;
        ConjGradSolverImpl();
        शून्य समुच्चयः_पदांशः_Criteria(ध्रुव पदांशः_Criteria& termcrit) CV_OVERRIDE;
        द्विगुण minimize(निवेशः_Output_Array x) CV_OVERRIDE;
    रक्षित:
        Ptr<MinProblemSolver::Function> _Function;
        पदांशः_Criteria _termcrit;
        Mat_<द्विगुण> d,r,buf_x,r_old;
        Mat_<द्विगुण> minimize_On_The_पङ्क्तिः_buf1,minimize_On_The_पङ्क्तिः_buf2;
    गुप्त:
        स्थिर शून्य minimize_On_The_पङ्क्तिः(Ptr<MinProblemSolver::Function> _f,Mat_<द्विगुण>& x,ध्रुव Mat_<द्विगुण>& d,Mat_<द्विगुण>& buf1,Mat_<द्विगुण>& buf2);
    };

    शून्य ConjGradSolverImpl::minimize_On_The_पङ्क्तिः(Ptr<MinProblemSolver::Function> _f,Mat_<द्विगुण>& x,ध्रुव Mat_<द्विगुण>& d,Mat_<द्विगुण>& buf1,
            Mat_<द्विगुण>& buf2){
        द्विगुण sigma=INITIAL_SEC_विधानम्_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_matrix(x);
        dprintf(("d:\n"));
        print_matrix(d);

        प्रति(पूर्णाङ्क i=0;i<SEC_विधानम्_ITERATIONS;i++){
            _f->getGradient((द्विगुण*)x.दत्त,(द्विगुण*)buf1.दत्त);
            dprintf(("buf1:\n"));
            print_matrix(buf1);
            x=x+sigma*d;
            _f->getGradient((द्विगुण*)x.दत्त,(द्विगुण*)buf2.दत्त);
            dprintf(("buf2:\n"));
            print_matrix(buf2);
            द्विगुण d1=buf1.dot(d), d2=buf2.dot(d);
            यदि((d1-d2)==0){
                भङ्ग;
            }
            द्विगुण alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_matrix(x);
    }

    द्विगुण ConjGradSolverImpl::minimize(निवेशः_Output_Array x){
        CV_Assert(_Function.empty()==असत्य);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_गणनम्,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.पङ्क्तिकाः,x_mat.स्तम्भसङ्ख्या)==1);
        पूर्णाङ्क ndim=MAX(x_mat.पङ्क्तिकाः,x_mat.स्तम्भसङ्ख्या);
        CV_Assert(x_mat.type()==CV_64FC1);

        यदि(d.स्तम्भसङ्ख्या!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_पङ्क्तिः_buf1.create(1,ndim);
            minimize_On_The_पङ्क्तिः_buf2.create(1,ndim);
        }

        Mat_<द्विगुण> proxy_x;
        यदि(x_mat.पङ्क्तिकाः>1){
            buf_x.create(1,ndim);
            Mat_<द्विगुण> proxy(ndim,1,buf_x.ptr<द्विगुण>());
            x_mat.copy_गन्तव्यम्(proxy);
            proxy_x=buf_x;
        }अन्यथा{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<द्विगुण>(),d.ptr<द्विगुण>());
        d*=-1.0;
        d.copy_गन्तव्यम्(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_matrix(proxy_x);
        dprintf(("d first time\n"));print_matrix(d);
        dprintf(("r\n"));print_matrix(r);

        प्रति(पूर्णाङ्क गणनम्_2=0;गणनम्_2<_termcrit.max_गणनम्;गणनम्_2++){
            minimize_On_The_पङ्क्तिः(_Function,proxy_x,d,minimize_On_The_पङ्क्तिः_buf1,minimize_On_The_पङ्क्तिः_buf2);
            r.copy_गन्तव्यम्(r_old);
            _Function->getGradient(proxy_x.ptr<द्विगुण>(),r.ptr<द्विगुण>());
            r*=-1.0;
            द्विगुण r_norm_sq=norm(r);
            यदि(_termcrit.type==(पदांशः_Criteria::MAX_ITER+पदांशः_Criteria::EPS) && r_norm_sq<_termcrit.epsilon){
                भङ्ग;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            द्विगुण beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        यदि(x_mat.पङ्क्तिकाः>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<द्विगुण>()).copy_गन्तव्यम्(x);
        }
        प्रत्यागम _Function->calc(proxy_x.ptr<द्विगुण>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()ध्रुव{
        प्रत्यागम _Function;
    }
    शून्य ConjGradSolverImpl::समुच्चयः_Function(ध्रुव Ptr<Function>& f){
        _Function=f;
    }
    पदांशः_Criteria ConjGradSolverImpl::get_पदांशः_Criteria()ध्रुव{
        प्रत्यागम _termcrit;
    }
    शून्य ConjGradSolverImpl::समुच्चयः_पदांशः_Criteria(ध्रुव पदांशः_Criteria& termcrit){
        CV_Assert((termcrit.type==(पदांशः_Criteria::MAX_ITER+पदांशः_Criteria::EPS) && termcrit.epsilon>0 && termcrit.max_गणनम्>0) ||
                ((termcrit.type==पदांशः_Criteria::MAX_ITER) && termcrit.max_गणनम्>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(ध्रुव Ptr<MinProblemSolver::Function>& f, पदांशः_Criteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->समुच्चयः_Function(f);
        CG->समुच्चयः_पदांशः_Criteria(termcrit);
        प्रत्यागम CG;
    }
}
