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

#అన్తర్భావ "precomp.hpp"

#నిర్వచించు dprintf(x)
#నిర్వచించు print_మాతృక(x)

నామాకాశ cv
{
    ద్విగుణ MinProblemSolver::Function::getGradientEps() స్థిరరాశి { తిరిగి 1e-3; }
    శూన్య MinProblemSolver::Function::getGradient(స్థిరరాశి ద్విగుణ* x, ద్విగుణ* grad)
    {
        ద్విగుణ eps = getGradientEps();
        పూర్ణాంకం i, n = getDims();
        Auto_బఫరు<ద్విగుణ> x_buf(n);
        ద్విగుణ* x_ = x_buf.దత్తాంశం();
        కొరకు( i = 0; i < n; i++ )
            x_[i] = x[i];
        కొరకు( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            ద్విగుణ y1 = calc(x_);
            x_[i] = x[i] - eps;
            ద్విగుణ y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#నిర్వచించు SEC_పద్ధతి_ITERATIONS 4
#నిర్వచించు INITIAL_SEC_పద్ధతి_SIGMA 0.1
    తరగతి ConjGradSolverImpl CV_FINAL : బహిరంగ ConjGradSolver
    {
    బహిరంగ:
        Ptr<Function> getFunction() స్థిరరాశి CV_OVERRIDE;
        శూన్య సమితి_Function(స్థిరరాశి Ptr<Function>& f) CV_OVERRIDE;
        పదం_Criteria get_పదం_Criteria() స్థిరరాశి CV_OVERRIDE;
        ConjGradSolverImpl();
        శూన్య సమితి_పదం_Criteria(స్థిరరాశి పదం_Criteria& termcrit) CV_OVERRIDE;
        ద్విగుణ minimize(ఆదానం_Output_Array x) CV_OVERRIDE;
    రక్షిత:
        Ptr<MinProblemSolver::Function> _Function;
        పదం_Criteria _termcrit;
        Mat_<ద్విగుణ> d,r,buf_x,r_old;
        Mat_<ద్విగుణ> minimize_On_The_పంక్తి_buf1,minimize_On_The_పంక్తి_buf2;
    అంతరంగ:
        స్థిర శూన్య minimize_On_The_పంక్తి(Ptr<MinProblemSolver::Function> _f,Mat_<ద్విగుణ>& x,స్థిరరాశి Mat_<ద్విగుణ>& d,Mat_<ద్విగుణ>& buf1,Mat_<ద్విగుణ>& buf2);
    };

    శూన్య ConjGradSolverImpl::minimize_On_The_పంక్తి(Ptr<MinProblemSolver::Function> _f,Mat_<ద్విగుణ>& x,స్థిరరాశి Mat_<ద్విగుణ>& d,Mat_<ద్విగుణ>& buf1,
            Mat_<ద్విగుణ>& buf2){
        ద్విగుణ sigma=INITIAL_SEC_పద్ధతి_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_మాతృక(x);
        dprintf(("d:\n"));
        print_మాతృక(d);

        కొరకు(పూర్ణాంకం i=0;i<SEC_పద్ధతి_ITERATIONS;i++){
            _f->getGradient((ద్విగుణ*)x.దత్తాంశం,(ద్విగుణ*)buf1.దత్తాంశం);
            dprintf(("buf1:\n"));
            print_మాతృక(buf1);
            x=x+sigma*d;
            _f->getGradient((ద్విగుణ*)x.దత్తాంశం,(ద్విగుణ*)buf2.దత్తాంశం);
            dprintf(("buf2:\n"));
            print_మాతృక(buf2);
            ద్విగుణ d1=buf1.dot(d), d2=buf2.dot(d);
            ఒకవేళ((d1-d2)==0){
                ఆపు;
            }
            ద్విగుణ alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_మాతృక(x);
    }

    ద్విగుణ ConjGradSolverImpl::minimize(ఆదానం_Output_Array x){
        CV_Assert(_Function.empty()==అబద్ధం);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_లెక్క,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.అడ్డువరుసలు,x_mat.నిలువుల_సంఖ్య)==1);
        పూర్ణాంకం ndim=MAX(x_mat.అడ్డువరుసలు,x_mat.నిలువుల_సంఖ్య);
        CV_Assert(x_mat.type()==CV_64FC1);

        ఒకవేళ(d.నిలువుల_సంఖ్య!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_పంక్తి_buf1.create(1,ndim);
            minimize_On_The_పంక్తి_buf2.create(1,ndim);
        }

        Mat_<ద్విగుణ> proxy_x;
        ఒకవేళ(x_mat.అడ్డువరుసలు>1){
            buf_x.create(1,ndim);
            Mat_<ద్విగుణ> proxy(ndim,1,buf_x.ptr<ద్విగుణ>());
            x_mat.copy_గమ్యం(proxy);
            proxy_x=buf_x;
        }లేకపోతే{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<ద్విగుణ>(),d.ptr<ద్విగుణ>());
        d*=-1.0;
        d.copy_గమ్యం(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_మాతృక(proxy_x);
        dprintf(("d first time\n"));print_మాతృక(d);
        dprintf(("r\n"));print_మాతృక(r);

        కొరకు(పూర్ణాంకం లెక్క_2=0;లెక్క_2<_termcrit.max_లెక్క;లెక్క_2++){
            minimize_On_The_పంక్తి(_Function,proxy_x,d,minimize_On_The_పంక్తి_buf1,minimize_On_The_పంక్తి_buf2);
            r.copy_గమ్యం(r_old);
            _Function->getGradient(proxy_x.ptr<ద్విగుణ>(),r.ptr<ద్విగుణ>());
            r*=-1.0;
            ద్విగుణ r_norm_sq=norm(r);
            ఒకవేళ(_termcrit.type==(పదం_Criteria::MAX_ITER+పదం_Criteria::EPS) && r_norm_sq<_termcrit.epsilon){
                ఆపు;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            ద్విగుణ beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        ఒకవేళ(x_mat.అడ్డువరుసలు>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<ద్విగుణ>()).copy_గమ్యం(x);
        }
        తిరిగి _Function->calc(proxy_x.ptr<ద్విగుణ>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()స్థిరరాశి{
        తిరిగి _Function;
    }
    శూన్య ConjGradSolverImpl::సమితి_Function(స్థిరరాశి Ptr<Function>& f){
        _Function=f;
    }
    పదం_Criteria ConjGradSolverImpl::get_పదం_Criteria()స్థిరరాశి{
        తిరిగి _termcrit;
    }
    శూన్య ConjGradSolverImpl::సమితి_పదం_Criteria(స్థిరరాశి పదం_Criteria& termcrit){
        CV_Assert((termcrit.type==(పదం_Criteria::MAX_ITER+పదం_Criteria::EPS) && termcrit.epsilon>0 && termcrit.max_లెక్క>0) ||
                ((termcrit.type==పదం_Criteria::MAX_ITER) && termcrit.max_లెక్క>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(స్థిరరాశి Ptr<MinProblemSolver::Function>& f, పదం_Criteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->సమితి_Function(f);
        CG->సమితి_పదం_Criteria(termcrit);
        తిరిగి CG;
    }
}
