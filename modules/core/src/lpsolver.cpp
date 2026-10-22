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
#ضمن <climits>
#ضمن <algorithm>

#تعریف_کن dprintf(x)
#تعریف_کن print_ماتریس(x)

نطاق cv
{

استفاده std::vector;

#اگر_تعریف_شده ALEX_DEBUG
ساكن فراغ print_simplex_state(ثابت Mat& c,ثابت Mat& b,مزدوج v,ثابت std::vector<صحيح> N,ثابت std::vector<صحيح> B){
    چاپ_کن("\tprint simplex state\n");

    چاپ_کن("v=%g\n",v);

    چاپ_کن("here c goes\n");
    print_ماتریس(c);

    چاپ_کن("non-basic: ");
    print(Mat(N));
    چاپ_کن("\n");

    چاپ_کن("here b goes\n");
    print_ماتریس(b);
    چاپ_کن("basic: ");

    print(Mat(B));
    چاپ_کن("\n");
}
#وإلا
#تعریف_کن print_simplex_state(c,b,v,N,B)
#پایان_اگر

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ساكن صحيح initialize_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بیعلامت صحيح>& شاخص_مقصد_ردیف);
ساكن مضمن فراغ محور(Mat_<مزدوج>& c,Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,صحيح leaving_شاخص,
        صحيح entering_شاخص,vector<بیعلامت صحيح>& شاخص_مقصد_ردیف);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ساكن صحيح inner_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بیعلامت صحيح>& شاخص_مقصد_ردیف);
ساكن فراغ جابجا_columns(Mat_<مزدوج>& A,صحيح col1,صحيح col2);
#تعریف_کن جابجا(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
صحيح solveLP(درون_داد_Array Func_, درون_داد_Array Constr_, OutputArray z_, مزدوج constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.ردیفها()==1 && (Constr_.تعداد_ستون()-Func_.تعداد_ستون()==1))||
            (Func_.تعداد_ستون()==1 && (Constr_.تعداد_ستون()-Func_.ردیفها()==1)));
    إذا (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<مزدوج> bigC=Mat_<مزدوج>(1,(Func.ردیفها==1?Func.تعداد_ستون:Func.ردیفها)+1),
        bigB=Mat_<مزدوج>(Constr.ردیفها,Constr.تعداد_ستون+1);
    إذا(Func.ردیفها==1){
        Func.convert_مقصد(bigC.ستون_Range_2(1,bigC.تعداد_ستون),CV_64FC1);
    }وإلا{
        Mat FuncT=Func.t();
        FuncT.convert_مقصد(bigC.ستون_Range_2(1,bigC.تعداد_ستون),CV_64FC1);
    }
    Constr.convert_مقصد(bigB.ستون_Range_2(1,bigB.تعداد_ستون),CV_64FC1);
    مزدوج v=0;
    vector<صحيح> N,B;
    vector<بیعلامت صحيح> شاخص_مقصد_ردیف;

    إذا(initialize_simplex(bigC,bigB,v,N,B,شاخص_مقصد_ردیف)==SOLVELP_UNFEASIBLE){
        أرجع SOLVELP_UNFEASIBLE;
    }
    Mat_<مزدوج> c=bigC.ستون_Range_2(1,bigC.تعداد_ستون),
        b=bigB.ستون_Range_2(1,bigB.تعداد_ستون);

    صحيح res=0;
    إذا((res=inner_simplex(c,b,v,N,B,شاخص_مقصد_ردیف))==SOLVELP_UNBOUNDED){
        أرجع SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.تعداد_ستون,1,CV_64FC1);
    MatIterator_<مزدوج> it=z.begin<مزدوج>();
    بیعلامت صحيح nsize = (بیعلامت صحيح)N.size();
    لكل(صحيح i=1;i<=c.تعداد_ستون;i++,it++){
        إذا(شاخص_مقصد_ردیف[i]<nsize){
            *it=0;
        }وإلا{
            *it=b.at<مزدوج>(شاخص_مقصد_ردیف[i]-nsize,b.تعداد_ستون-1);
        }
    }

    z.copy_مقصد(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.تعداد_ستون - 1, Constr.ردیفها)) * z;
    Mat constr_check = Constr.ستون(Constr.تعداد_ستون - 1) - prod;
    مزدوج min_مقدار = 0.0;
    minMaxIdx(constr_check, &min_مقدار);
    إذا (min_مقدار < -constr_eps)
    {
        أرجع SOLVELP_گمشده;
    }

    أرجع res;
}

صحيح solveLP(درون_داد_Array Func, درون_داد_Array Constr, OutputArray z)
{
    أرجع solveLP(Func, Constr, z, 1e-12);
}

ساكن صحيح initialize_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بیعلامت صحيح>& شاخص_مقصد_ردیف){
    N.resize(c.تعداد_ستون);
    N[0]=0;
    لكل (std::vector<صحيح>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.ردیفها);
    B[0]=(صحيح)N.size();
    لكل (std::vector<صحيح>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    شاخص_مقصد_ردیف.resize(c.تعداد_ستون+b.ردیفها);
    شاخص_مقصد_ردیف[0]=0;
    لكل (std::vector<بیعلامت صحيح>::iterator it = شاخص_مقصد_ردیف.begin()+1 ; it != شاخص_مقصد_ردیف.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    صحيح k=0;
    {
        مزدوج min=DBL_MAX;
        لكل(صحيح i=0;i<b.ردیفها;i++){
            إذا(b(i,b.تعداد_ستون-1)<min){
                min=b(i,b.تعداد_ستون-1);
                k=i;
            }
        }
    }

    إذا(b(k,b.تعداد_ستون-1)>=0){
        N.erase(N.begin());
        لكل (std::vector<بیعلامت صحيح>::iterator it = شاخص_مقصد_ردیف.begin()+1 ; it != شاخص_مقصد_ردیف.end(); ++it){
            --(*it);
        }
        أرجع 0;
    }

    Mat_<مزدوج> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    لكل(صحيح i=0;i<b.ردیفها;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    محور(c,b,v,N,B,k,0,شاخص_مقصد_ردیف);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,شاخص_مقصد_ردیف);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    بیعلامت صحيح nsize = (بیعلامت صحيح)N.size();
    إذا(شاخص_مقصد_ردیف[0]>=nsize){
        صحيح iterator_جابه_جایی=شاخص_مقصد_ردیف[0]-nsize;
        إذا(b(iterator_جابه_جایی,b.تعداد_ستون-1)>0){
            أرجع SOLVELP_UNFEASIBLE;
        }
        محور(c,b,v,N,B,iterator_جابه_جایی,0,شاخص_مقصد_ردیف);
    }

    vector<صحيح>::iterator iterator;
    {
        صحيح iterator_جابه_جایی=شاخص_مقصد_ردیف[0];
        iterator=N.begin()+iterator_جابه_جایی;
        std::iter_swap(iterator,N.begin());
        جابجا(صحيح,شاخص_مقصد_ردیف[*iterator],شاخص_مقصد_ردیف[0]);
        جابجا_columns(c,iterator_جابه_جایی,0);
        جابجا_columns(b,iterator_جابه_جایی,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    لكل(صحيح I=1;I<old_c.تعداد_ستون;I++){
        إذا(شاخص_مقصد_ردیف[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            صحيح iterator_جابه_جایی=شاخص_مقصد_ردیف[I];
            c(0,iterator_جابه_جایی)+=old_c(0,I);
            print_ماتریس(c);
        }وإلا{
            dprintf(("I=%d from basic\n",I));
            صحيح iterator_جابه_جایی=شاخص_مقصد_ردیف[I]-nsize;
            c-=old_c(0,I)*b.ردیف(iterator_جابه_جایی).ستون_Range_2(0,b.تعداد_ستون-1);
            v+=old_c(0,I)*b(iterator_جابه_جایی,b.تعداد_ستون-1);
            print_ماتریس(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    لكل (std::vector<بیعلامت صحيح>::iterator it = شاخص_مقصد_ردیف.begin()+1 ; it != شاخص_مقصد_ردیف.end(); ++it){
        --(*it);
    }
    أرجع 0;
}

ساكن صحيح inner_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بیعلامت صحيح>& شاخص_مقصد_ردیف){

    لكل(;;){
        MatIterator_<مزدوج> pos_ptr;
        صحيح e=-1,pos_ctr=0,min_var=بیشترین_صحیح;
        منطقي all_nonzero=درست;
        لكل(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            إذا(*pos_ptr==0){
                all_nonzero=خطأ;
            }
            إذا(*pos_ptr>0){
                إذا(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        إذا(e==-1){
            dprintf(("hello from e==-1\n"));
            print_ماتریس(c);
            إذا(all_nonzero==درست){
                أرجع SOLVELP_SINGLE;
            }وإلا{
                أرجع SOLVELP_MULTI;
            }
        }

        صحيح l=-1;
        min_var=بیشترین_صحیح;
        مزدوج min=DBL_MAX;
        صحيح ردیف_it=0;
        MatIterator_<مزدوج> min_ردیف_ptr=b.begin();
        لكل(MatIterator_<مزدوج> it=b.begin();it!=b.end();it+=b.تعداد_ستون,ردیف_it++){
            مزدوج myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            إذا((myite=it[e])>0){
                مزدوج val=it[b.تعداد_ستون-1]/myite;
                إذا(val<min || (val==min && B[ردیف_it]<min_var)){
                    min_var=B[ردیف_it];
                    min_ردیف_ptr=it;
                    min=val;
                    l=ردیف_it;
                }
            }
        }
        إذا(l==-1){
            أرجع SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        محور(c,b,v,N,B,l,e,شاخص_مقصد_ردیف);

        dprintf(("objective, v=%g\n",v));
        print_ماتریس(c);
        dprintf(("constraints\n"));
        print_ماتریس(b);
        dprintf(("non-basic: "));
        print_ماتریس(Mat(N));
        dprintf(("basic: "));
        print_ماتریس(Mat(B));
    }
}

ساكن مضمن فراغ محور(Mat_<مزدوج>& c,Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,
        صحيح leaving_شاخص,صحيح entering_شاخص,vector<بیعلامت صحيح>& شاخص_مقصد_ردیف){
    مزدوج Coef=b(leaving_شاخص,entering_شاخص);
    لكل(صحيح i=0;i<b.تعداد_ستون;i++){
        إذا(i==entering_شاخص){
            b(leaving_شاخص,i)=1/Coef;
        }وإلا{
            b(leaving_شاخص,i)/=Coef;
        }
    }

    لكل(صحيح i=0;i<b.ردیفها;i++){
        إذا(i!=leaving_شاخص){
            مزدوج coef=b(i,entering_شاخص);
            لكل(صحيح j=0;j<b.تعداد_ستون;j++){
                إذا(j==entering_شاخص){
                    b(i,j)=-coef*b(leaving_شاخص,j);
                }وإلا{
                    b(i,j)-=(coef*b(leaving_شاخص,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_شاخص);
    لكل(صحيح i=0;i<(b.تعداد_ستون-1);i++){
        إذا(i==entering_شاخص){
            c(0,i)=-Coef*b(leaving_شاخص,i);
        }وإلا{
            c(0,i)-=Coef*b(leaving_شاخص,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_شاخص,b.تعداد_ستون-1);

    جابجا(صحيح,N[entering_شاخص],B[leaving_شاخص]);
    جابجا(صحيح,شاخص_مقصد_ردیف[N[entering_شاخص]],شاخص_مقصد_ردیف[B[leaving_شاخص]]);
}

ساكن مضمن فراغ جابجا_columns(Mat_<مزدوج>& A,صحيح col1,صحيح col2){
    لكل(صحيح i=0;i<A.ردیفها;i++){
        مزدوج tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
