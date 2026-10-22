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

#define dprintf(x)
#define print_matrix(x)

نطاق cv
{
    مزدوج MinProblemSolver::Function::getGradientEps() ثابت { החזר 1e-3; }
    فراغ MinProblemSolver::Function::getGradient(ثابت مزدوج* x, مزدوج* grad)
    {
        مزدوج eps = getGradientEps();
        صحيح i, n = getDims();
        AutoBuffer<مزدوج> x_buf(n);
        مزدوج* x_ = x_buf.data();
        עבור( i = 0; i < n; i++ )
            x_[i] = x[i];
        עבור( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            مزدوج y1 = calc(x_);
            x_[i] = x[i] - eps;
            مزدوج y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#define SEC_METHOD_ITERATIONS 4
#define INITIAL_SEC_METHOD_SIGMA 0.1
    מחלקה ConjGradSolverImpl CV_FINAL : عام ConjGradSolver
    {
    عام:
        Ptr<Function> getFunction() ثابت CV_OVERRIDE;
        فراغ setFunction(ثابت Ptr<Function>& f) CV_OVERRIDE;
        TermCriteria getTermCriteria() ثابت CV_OVERRIDE;
        ConjGradSolverImpl();
        فراغ setTermCriteria(ثابت TermCriteria& termcrit) CV_OVERRIDE;
        مزدوج minimize(InputOutputArray x) CV_OVERRIDE;
    محمي:
        Ptr<MinProblemSolver::Function> _Function;
        TermCriteria _termcrit;
        Mat_<مزدوج> d,r,buf_x,r_old;
        Mat_<مزدوج> minimizeOnTheLine_buf1,minimizeOnTheLine_buf2;
    خاص:
        ساكن فراغ minimizeOnTheLine(Ptr<MinProblemSolver::Function> _f,Mat_<مزدوج>& x,ثابت Mat_<مزدوج>& d,Mat_<مزدوج>& buf1,Mat_<مزدوج>& buf2);
    };

    فراغ ConjGradSolverImpl::minimizeOnTheLine(Ptr<MinProblemSolver::Function> _f,Mat_<مزدوج>& x,ثابت Mat_<مزدوج>& d,Mat_<مزدوج>& buf1,
            Mat_<مزدوج>& buf2){
        مزدوج sigma=INITIAL_SEC_METHOD_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_matrix(x);
        dprintf(("d:\n"));
        print_matrix(d);

        עבור(صحيح i=0;i<SEC_METHOD_ITERATIONS;i++){
            _f->getGradient((مزدوج*)x.data,(مزدوج*)buf1.data);
            dprintf(("buf1:\n"));
            print_matrix(buf1);
            x=x+sigma*d;
            _f->getGradient((مزدوج*)x.data,(مزدوج*)buf2.data);
            dprintf(("buf2:\n"));
            print_matrix(buf2);
            مزدوج d1=buf1.dot(d), d2=buf2.dot(d);
            אם((d1-d2)==0){
                اكسر;
            }
            مزدوج alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_matrix(x);
    }

    مزدوج ConjGradSolverImpl::minimize(InputOutputArray x){
        CV_Assert(_Function.empty()==שקר);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.maxCount,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.rows,x_mat.cols)==1);
        صحيح ndim=MAX(x_mat.rows,x_mat.cols);
        CV_Assert(x_mat.type()==CV_64FC1);

        אם(d.cols!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimizeOnTheLine_buf1.create(1,ndim);
            minimizeOnTheLine_buf2.create(1,ndim);
        }

        Mat_<مزدوج> proxy_x;
        אם(x_mat.rows>1){
            buf_x.create(1,ndim);
            Mat_<مزدوج> proxy(ndim,1,buf_x.ptr<مزدوج>());
            x_mat.copyTo(proxy);
            proxy_x=buf_x;
        }אחרת{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<مزدوج>(),d.ptr<مزدوج>());
        d*=-1.0;
        d.copyTo(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_matrix(proxy_x);
        dprintf(("d first time\n"));print_matrix(d);
        dprintf(("r\n"));print_matrix(r);

        עבור(صحيح count=0;count<_termcrit.maxCount;count++){
            minimizeOnTheLine(_Function,proxy_x,d,minimizeOnTheLine_buf1,minimizeOnTheLine_buf2);
            r.copyTo(r_old);
            _Function->getGradient(proxy_x.ptr<مزدوج>(),r.ptr<مزدوج>());
            r*=-1.0;
            مزدوج r_norm_sq=norm(r);
            אם(_termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && r_norm_sq<_termcrit.epsilon){
                اكسر;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            مزدوج beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        אם(x_mat.rows>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<مزدوج>()).copyTo(x);
        }
        החזר _Function->calc(proxy_x.ptr<مزدوج>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()ثابت{
        החזר _Function;
    }
    فراغ ConjGradSolverImpl::setFunction(ثابت Ptr<Function>& f){
        _Function=f;
    }
    TermCriteria ConjGradSolverImpl::getTermCriteria()ثابت{
        החזר _termcrit;
    }
    فراغ ConjGradSolverImpl::setTermCriteria(ثابت TermCriteria& termcrit){
        CV_Assert((termcrit.type==(TermCriteria::MAX_ITER+TermCriteria::EPS) && termcrit.epsilon>0 && termcrit.maxCount>0) ||
                ((termcrit.type==TermCriteria::MAX_ITER) && termcrit.maxCount>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(ثابت Ptr<MinProblemSolver::Function>& f, TermCriteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->setFunction(f);
        CG->setTermCriteria(termcrit);
        החזר CG;
    }
}
