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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dprintf(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ print_ਮੈਟ੍ਰਿਕਸ(x)

ਨਾਮਾਕਾਸ਼ cv
{
    ਦੁੱਗਣਾ MinProblemSolver::Function::getGradientEps() ਅਚਲ { ਵਾਪਸ 1e-3; }
    ਖਾਲੀ MinProblemSolver::Function::getGradient(ਅਚਲ ਦੁੱਗਣਾ* x, ਦੁੱਗਣਾ* grad)
    {
        ਦੁੱਗਣਾ eps = getGradientEps();
        ਪੂਰਨ i, n = getDims();
        Auto_ਬਫਰ<ਦੁੱਗਣਾ> x_buf(n);
        ਦੁੱਗਣਾ* x_ = x_buf.ਅੰਕੜੇ();
        ਲਈ( i = 0; i < n; i++ )
            x_[i] = x[i];
        ਲਈ( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            ਦੁੱਗਣਾ y1 = calc(x_);
            x_[i] = x[i] - eps;
            ਦੁੱਗਣਾ y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SEC_ਵਿਧੀ_ITERATIONS 4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INITIAL_SEC_ਵਿਧੀ_SIGMA 0.1
    ਵਰਗ ConjGradSolverImpl CV_FINAL : ਜਨਤਕ ConjGradSolver
    {
    ਜਨਤਕ:
        Ptr<Function> getFunction() ਅਚਲ CV_OVERRIDE;
        ਖਾਲੀ ਸੈੱਟ_Function(ਅਚਲ Ptr<Function>& f) CV_OVERRIDE;
        ਪਦ_Criteria get_ਪਦ_Criteria() ਅਚਲ CV_OVERRIDE;
        ConjGradSolverImpl();
        ਖਾਲੀ ਸੈੱਟ_ਪਦ_Criteria(ਅਚਲ ਪਦ_Criteria& termcrit) CV_OVERRIDE;
        ਦੁੱਗਣਾ minimize(ਨਿਵੇਸ਼_Output_Array x) CV_OVERRIDE;
    ਸੁਰੱਖਿਅਤ:
        Ptr<MinProblemSolver::Function> _Function;
        ਪਦ_Criteria _termcrit;
        Mat_<ਦੁੱਗਣਾ> d,r,buf_x,r_old;
        Mat_<ਦੁੱਗਣਾ> minimize_On_The_ਸਤਰ_buf1,minimize_On_The_ਸਤਰ_buf2;
    ਨਿਜੀ:
        ਸਥਿਰ ਖਾਲੀ minimize_On_The_ਸਤਰ(Ptr<MinProblemSolver::Function> _f,Mat_<ਦੁੱਗਣਾ>& x,ਅਚਲ Mat_<ਦੁੱਗਣਾ>& d,Mat_<ਦੁੱਗਣਾ>& buf1,Mat_<ਦੁੱਗਣਾ>& buf2);
    };

    ਖਾਲੀ ConjGradSolverImpl::minimize_On_The_ਸਤਰ(Ptr<MinProblemSolver::Function> _f,Mat_<ਦੁੱਗਣਾ>& x,ਅਚਲ Mat_<ਦੁੱਗਣਾ>& d,Mat_<ਦੁੱਗਣਾ>& buf1,
            Mat_<ਦੁੱਗਣਾ>& buf2){
        ਦੁੱਗਣਾ sigma=INITIAL_SEC_ਵਿਧੀ_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_ਮੈਟ੍ਰਿਕਸ(x);
        dprintf(("d:\n"));
        print_ਮੈਟ੍ਰਿਕਸ(d);

        ਲਈ(ਪੂਰਨ i=0;i<SEC_ਵਿਧੀ_ITERATIONS;i++){
            _f->getGradient((ਦੁੱਗਣਾ*)x.ਅੰਕੜੇ,(ਦੁੱਗਣਾ*)buf1.ਅੰਕੜੇ);
            dprintf(("buf1:\n"));
            print_ਮੈਟ੍ਰਿਕਸ(buf1);
            x=x+sigma*d;
            _f->getGradient((ਦੁੱਗਣਾ*)x.ਅੰਕੜੇ,(ਦੁੱਗਣਾ*)buf2.ਅੰਕੜੇ);
            dprintf(("buf2:\n"));
            print_ਮੈਟ੍ਰਿਕਸ(buf2);
            ਦੁੱਗਣਾ d1=buf1.dot(d), d2=buf2.dot(d);
            ਜੇ((d1-d2)==0){
                ਰੋਕੋ;
            }
            ਦੁੱਗਣਾ alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_ਮੈਟ੍ਰਿਕਸ(x);
    }

    ਦੁੱਗਣਾ ConjGradSolverImpl::minimize(ਨਿਵੇਸ਼_Output_Array x){
        CV_Assert(_Function.empty()==ਝੂਠ);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_ਗਿਣਤੀ,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.ਪੰਕਤੀਆਂ,x_mat.ਖਾਨੇ_ਗਿਣਤੀ)==1);
        ਪੂਰਨ ndim=MAX(x_mat.ਪੰਕਤੀਆਂ,x_mat.ਖਾਨੇ_ਗਿਣਤੀ);
        CV_Assert(x_mat.type()==CV_64FC1);

        ਜੇ(d.ਖਾਨੇ_ਗਿਣਤੀ!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_ਸਤਰ_buf1.create(1,ndim);
            minimize_On_The_ਸਤਰ_buf2.create(1,ndim);
        }

        Mat_<ਦੁੱਗਣਾ> proxy_x;
        ਜੇ(x_mat.ਪੰਕਤੀਆਂ>1){
            buf_x.create(1,ndim);
            Mat_<ਦੁੱਗਣਾ> proxy(ndim,1,buf_x.ptr<ਦੁੱਗਣਾ>());
            x_mat.copy_ਮੰਜ਼ਿਲ(proxy);
            proxy_x=buf_x;
        }ਨਹੀਂਤਾਂ{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<ਦੁੱਗਣਾ>(),d.ptr<ਦੁੱਗਣਾ>());
        d*=-1.0;
        d.copy_ਮੰਜ਼ਿਲ(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_ਮੈਟ੍ਰਿਕਸ(proxy_x);
        dprintf(("d first time\n"));print_ਮੈਟ੍ਰਿਕਸ(d);
        dprintf(("r\n"));print_ਮੈਟ੍ਰਿਕਸ(r);

        ਲਈ(ਪੂਰਨ ਗਿਣਤੀ_3=0;ਗਿਣਤੀ_3<_termcrit.max_ਗਿਣਤੀ;ਗਿਣਤੀ_3++){
            minimize_On_The_ਸਤਰ(_Function,proxy_x,d,minimize_On_The_ਸਤਰ_buf1,minimize_On_The_ਸਤਰ_buf2);
            r.copy_ਮੰਜ਼ਿਲ(r_old);
            _Function->getGradient(proxy_x.ptr<ਦੁੱਗਣਾ>(),r.ptr<ਦੁੱਗਣਾ>());
            r*=-1.0;
            ਦੁੱਗਣਾ r_norm_sq=norm(r);
            ਜੇ(_termcrit.type==(ਪਦ_Criteria::MAX_ITER+ਪਦ_Criteria::EPS) && r_norm_sq<_termcrit.epsilon){
                ਰੋਕੋ;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            ਦੁੱਗਣਾ beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        ਜੇ(x_mat.ਪੰਕਤੀਆਂ>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<ਦੁੱਗਣਾ>()).copy_ਮੰਜ਼ਿਲ(x);
        }
        ਵਾਪਸ _Function->calc(proxy_x.ptr<ਦੁੱਗਣਾ>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()ਅਚਲ{
        ਵਾਪਸ _Function;
    }
    ਖਾਲੀ ConjGradSolverImpl::ਸੈੱਟ_Function(ਅਚਲ Ptr<Function>& f){
        _Function=f;
    }
    ਪਦ_Criteria ConjGradSolverImpl::get_ਪਦ_Criteria()ਅਚਲ{
        ਵਾਪਸ _termcrit;
    }
    ਖਾਲੀ ConjGradSolverImpl::ਸੈੱਟ_ਪਦ_Criteria(ਅਚਲ ਪਦ_Criteria& termcrit){
        CV_Assert((termcrit.type==(ਪਦ_Criteria::MAX_ITER+ਪਦ_Criteria::EPS) && termcrit.epsilon>0 && termcrit.max_ਗਿਣਤੀ>0) ||
                ((termcrit.type==ਪਦ_Criteria::MAX_ITER) && termcrit.max_ਗਿਣਤੀ>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(ਅਚਲ Ptr<MinProblemSolver::Function>& f, ਪਦ_Criteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->ਸੈੱਟ_Function(f);
        CG->ਸੈੱਟ_ਪਦ_Criteria(termcrit);
        ਵਾਪਸ CG;
    }
}
