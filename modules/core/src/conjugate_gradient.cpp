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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ವ್ಯಾಖ್ಯಾನಿಸು dprintf(x)
#ವ್ಯಾಖ್ಯಾನಿಸು print_ಮಾತೃಕೆ(x)

ನಾಮಾಕಾಶ cv
{
    ದ್ವಿಗುಣ MinProblemSolver::Function::getGradientEps() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ 1e-3; }
    ಶೂನ್ಯ MinProblemSolver::Function::getGradient(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* x, ದ್ವಿಗುಣ* grad)
    {
        ದ್ವಿಗುಣ eps = getGradientEps();
        ಪೂರ್ಣಾಂಕ i, n = getDims();
        Auto_ಬಫರ್<ದ್ವಿಗುಣ> x_buf(n);
        ದ್ವಿಗುಣ* x_ = x_buf.ದತ್ತಾಂಶ();
        ಗಾಗಿ( i = 0; i < n; i++ )
            x_[i] = x[i];
        ಗಾಗಿ( i = 0; i < n; i++ )
        {
            x_[i] = x[i] + eps;
            ದ್ವಿಗುಣ y1 = calc(x_);
            x_[i] = x[i] - eps;
            ದ್ವಿಗುಣ y0 = calc(x_);
            grad[i] = (y1 - y0)/(2*eps);
            x_[i] = x[i];
        }
    }

#ವ್ಯಾಖ್ಯಾನಿಸು SEC_ವಿಧಾನ_ITERATIONS 4
#ವ್ಯಾಖ್ಯಾನಿಸು INITIAL_SEC_ವಿಧಾನ_SIGMA 0.1
    ವರ್ಗ ConjGradSolverImpl CV_FINAL : ಸಾರ್ವಜನಿಕ ConjGradSolver
    {
    ಸಾರ್ವಜನಿಕ:
        Ptr<Function> getFunction() ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
        ಶೂನ್ಯ ಗಣ_Function(ಸ್ಥಿರಾಂಕ Ptr<Function>& f) CV_OVERRIDE;
        ಪದ_Criteria get_ಪದ_Criteria() ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
        ConjGradSolverImpl();
        ಶೂನ್ಯ ಗಣ_ಪದ_Criteria(ಸ್ಥಿರಾಂಕ ಪದ_Criteria& termcrit) CV_OVERRIDE;
        ದ್ವಿಗುಣ minimize(ಒಳಹರಿವು_Output_Array x) CV_OVERRIDE;
    ಸಂರಕ್ಷಿತ:
        Ptr<MinProblemSolver::Function> _Function;
        ಪದ_Criteria _termcrit;
        Mat_<ದ್ವಿಗುಣ> d,r,buf_x,r_old;
        Mat_<ದ್ವಿಗುಣ> minimize_On_The_ಸಾಲು_buf1,minimize_On_The_ಸಾಲು_buf2;
    ಖಾಸಗಿ:
        ಸ್ಥಿರ ಶೂನ್ಯ minimize_On_The_ಸಾಲು(Ptr<MinProblemSolver::Function> _f,Mat_<ದ್ವಿಗುಣ>& x,ಸ್ಥಿರಾಂಕ Mat_<ದ್ವಿಗುಣ>& d,Mat_<ದ್ವಿಗುಣ>& buf1,Mat_<ದ್ವಿಗುಣ>& buf2);
    };

    ಶೂನ್ಯ ConjGradSolverImpl::minimize_On_The_ಸಾಲು(Ptr<MinProblemSolver::Function> _f,Mat_<ದ್ವಿಗುಣ>& x,ಸ್ಥಿರಾಂಕ Mat_<ದ್ವಿಗುಣ>& d,Mat_<ದ್ವಿಗುಣ>& buf1,
            Mat_<ದ್ವಿಗುಣ>& buf2){
        ದ್ವಿಗುಣ sigma=INITIAL_SEC_ವಿಧಾನ_SIGMA;
        buf1=0.0;
        buf2=0.0;

        dprintf(("before minimizeOnTheLine\n"));
        dprintf(("x:\n"));
        print_ಮಾತೃಕೆ(x);
        dprintf(("d:\n"));
        print_ಮಾತೃಕೆ(d);

        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i=0;i<SEC_ವಿಧಾನ_ITERATIONS;i++){
            _f->getGradient((ದ್ವಿಗುಣ*)x.ದತ್ತಾಂಶ,(ದ್ವಿಗುಣ*)buf1.ದತ್ತಾಂಶ);
            dprintf(("buf1:\n"));
            print_ಮಾತೃಕೆ(buf1);
            x=x+sigma*d;
            _f->getGradient((ದ್ವಿಗುಣ*)x.ದತ್ತಾಂಶ,(ದ್ವಿಗುಣ*)buf2.ದತ್ತಾಂಶ);
            dprintf(("buf2:\n"));
            print_ಮಾತೃಕೆ(buf2);
            ದ್ವಿಗುಣ d1=buf1.dot(d), d2=buf2.dot(d);
            ಆದರೆ((d1-d2)==0){
                ನಿಲ್ಲಿಸು;
            }
            ದ್ವಿಗುಣ alpha=-sigma*d1/(d2-d1);
            dprintf(("(buf2.dot(d)-buf1.dot(d))=%f\nalpha=%f\n",(buf2.dot(d)-buf1.dot(d)),alpha));
            x=x+(alpha-sigma)*d;
            sigma=-alpha;
        }

        dprintf(("after minimizeOnTheLine\n"));
        print_ಮಾತೃಕೆ(x);
    }

    ದ್ವಿಗುಣ ConjGradSolverImpl::minimize(ಒಳಹರಿವು_Output_Array x){
        CV_Assert(_Function.empty()==ಸುಳ್ಳು);
        dprintf(("termcrit:\n\ttype: %d\n\tmaxCount: %d\n\tEPS: %g\n",_termcrit.type,_termcrit.max_ಎಣಿಕೆ,_termcrit.epsilon));

        Mat x_mat=x.getMat();
        CV_Assert(MIN(x_mat.ಅಡ್ಡಸಾಲುಗಳು,x_mat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)==1);
        ಪೂರ್ಣಾಂಕ ndim=MAX(x_mat.ಅಡ್ಡಸಾಲುಗಳು,x_mat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        CV_Assert(x_mat.type()==CV_64FC1);

        ಆದರೆ(d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ!=ndim){
            d.create(1,ndim);
            r.create(1,ndim);
            r_old.create(1,ndim);
            minimize_On_The_ಸಾಲು_buf1.create(1,ndim);
            minimize_On_The_ಸಾಲು_buf2.create(1,ndim);
        }

        Mat_<ದ್ವಿಗುಣ> proxy_x;
        ಆದರೆ(x_mat.ಅಡ್ಡಸಾಲುಗಳು>1){
            buf_x.create(1,ndim);
            Mat_<ದ್ವಿಗುಣ> proxy(ndim,1,buf_x.ptr<ದ್ವಿಗುಣ>());
            x_mat.copy_ಗಮ್ಯ(proxy);
            proxy_x=buf_x;
        }ಇಲ್ಲದಿದ್ದರೆ{
            proxy_x=x_mat;
        }
        _Function->getGradient(proxy_x.ptr<ದ್ವಿಗುಣ>(),d.ptr<ದ್ವಿಗುಣ>());
        d*=-1.0;
        d.copy_ಗಮ್ಯ(r);

        //here everything goes. check that everything is set properly
        dprintf(("proxy_x\n"));print_ಮಾತೃಕೆ(proxy_x);
        dprintf(("d first time\n"));print_ಮಾತೃಕೆ(d);
        dprintf(("r\n"));print_ಮಾತೃಕೆ(r);

        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3=0;ಎಣಿಕೆ_3<_termcrit.max_ಎಣಿಕೆ;ಎಣಿಕೆ_3++){
            minimize_On_The_ಸಾಲು(_Function,proxy_x,d,minimize_On_The_ಸಾಲು_buf1,minimize_On_The_ಸಾಲು_buf2);
            r.copy_ಗಮ್ಯ(r_old);
            _Function->getGradient(proxy_x.ptr<ದ್ವಿಗುಣ>(),r.ptr<ದ್ವಿಗುಣ>());
            r*=-1.0;
            ದ್ವಿಗುಣ r_norm_sq=norm(r);
            ಆದರೆ(_termcrit.type==(ಪದ_Criteria::MAX_ITER+ಪದ_Criteria::EPS) && r_norm_sq<_termcrit.epsilon){
                ನಿಲ್ಲಿಸು;
            }
            r_norm_sq=r_norm_sq*r_norm_sq;
            ದ್ವಿಗುಣ beta=MAX(0.0,(r_norm_sq-r.dot(r_old))/r_norm_sq);
            d=r+beta*d;
        }



        ಆದರೆ(x_mat.ಅಡ್ಡಸಾಲುಗಳು>1){
            Mat(ndim, 1, CV_64F, proxy_x.ptr<ದ್ವಿಗುಣ>()).copy_ಗಮ್ಯ(x);
        }
        ಹಿಂತಿರುಗಿ _Function->calc(proxy_x.ptr<ದ್ವಿಗುಣ>());
    }

    ConjGradSolverImpl::ConjGradSolverImpl(){
        _Function=Ptr<Function>();
    }
    Ptr<MinProblemSolver::Function> ConjGradSolverImpl::getFunction()ಸ್ಥಿರಾಂಕ{
        ಹಿಂತಿರುಗಿ _Function;
    }
    ಶೂನ್ಯ ConjGradSolverImpl::ಗಣ_Function(ಸ್ಥಿರಾಂಕ Ptr<Function>& f){
        _Function=f;
    }
    ಪದ_Criteria ConjGradSolverImpl::get_ಪದ_Criteria()ಸ್ಥಿರಾಂಕ{
        ಹಿಂತಿರುಗಿ _termcrit;
    }
    ಶೂನ್ಯ ConjGradSolverImpl::ಗಣ_ಪದ_Criteria(ಸ್ಥಿರಾಂಕ ಪದ_Criteria& termcrit){
        CV_Assert((termcrit.type==(ಪದ_Criteria::MAX_ITER+ಪದ_Criteria::EPS) && termcrit.epsilon>0 && termcrit.max_ಎಣಿಕೆ>0) ||
                ((termcrit.type==ಪದ_Criteria::MAX_ITER) && termcrit.max_ಎಣಿಕೆ>0));
        _termcrit=termcrit;
    }
    // both minRange & minError are specified by termcrit.epsilon; In addition, user may specify the number of iterations that the algorithm does.
    Ptr<ConjGradSolver> ConjGradSolver::create(ಸ್ಥಿರಾಂಕ Ptr<MinProblemSolver::Function>& f, ಪದ_Criteria termcrit){
        Ptr<ConjGradSolver> CG = makePtr<ConjGradSolverImpl>();
        CG->ಗಣ_Function(f);
        CG->ಗಣ_ಪದ_Criteria(termcrit);
        ಹಿಂತಿರುಗಿ CG;
    }
}
