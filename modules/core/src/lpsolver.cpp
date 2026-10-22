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

#تعریف_کړه dprintf(x)
#تعریف_کړه print_ماتریکس(x)

نطاق cv
{

کارول std::vector;

#که_تعریف_شوی ALEX_DEBUG
ساكن فراغ print_simplex_state(ثابت Mat& c,ثابت Mat& b,مزدوج v,ثابت std::vector<صحيح> N,ثابت std::vector<صحيح> B){
    چاپ_کړه("\tprint simplex state\n");

    چاپ_کړه("v=%g\n",v);

    چاپ_کړه("here c goes\n");
    print_ماتریکس(c);

    چاپ_کړه("non-basic: ");
    print(Mat(N));
    چاپ_کړه("\n");

    چاپ_کړه("here b goes\n");
    print_ماتریکس(b);
    چاپ_کړه("basic: ");

    print(Mat(B));
    چاپ_کړه("\n");
}
#وإلا
#تعریف_کړه print_simplex_state(c,b,v,N,B)
#د_که_پای

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ساكن صحيح initialize_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بینښه صحيح>& شاخص_منزل_لیکه);
ساكن مضمن فراغ محور(Mat_<مزدوج>& c,Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,صحيح leaving_شاخص,
        صحيح entering_شاخص,vector<بینښه صحيح>& شاخص_منزل_لیکه);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ساكن صحيح inner_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بینښه صحيح>& شاخص_منزل_لیکه);
ساكن فراغ بدلون_columns(Mat_<مزدوج>& A,صحيح col1,صحيح col2);
#تعریف_کړه بدلون(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
صحيح solveLP(ننوت_Array Func_, ننوت_Array Constr_, OutputArray z_, مزدوج constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.لیکې()==1 && (Constr_.ستون_شمېر()-Func_.ستون_شمېر()==1))||
            (Func_.ستون_شمېر()==1 && (Constr_.ستون_شمېر()-Func_.لیکې()==1)));
    إذا (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<مزدوج> bigC=Mat_<مزدوج>(1,(Func.لیکې==1?Func.ستون_شمېر:Func.لیکې)+1),
        bigB=Mat_<مزدوج>(Constr.لیکې,Constr.ستون_شمېر+1);
    إذا(Func.لیکې==1){
        Func.convert_منزل(bigC.ستون_Range_2(1,bigC.ستون_شمېر),CV_64FC1);
    }وإلا{
        Mat FuncT=Func.t();
        FuncT.convert_منزل(bigC.ستون_Range_2(1,bigC.ستون_شمېر),CV_64FC1);
    }
    Constr.convert_منزل(bigB.ستون_Range_2(1,bigB.ستون_شمېر),CV_64FC1);
    مزدوج v=0;
    vector<صحيح> N,B;
    vector<بینښه صحيح> شاخص_منزل_لیکه;

    إذا(initialize_simplex(bigC,bigB,v,N,B,شاخص_منزل_لیکه)==SOLVELP_UNFEASIBLE){
        أرجع SOLVELP_UNFEASIBLE;
    }
    Mat_<مزدوج> c=bigC.ستون_Range_2(1,bigC.ستون_شمېر),
        b=bigB.ستون_Range_2(1,bigB.ستون_شمېر);

    صحيح res=0;
    إذا((res=inner_simplex(c,b,v,N,B,شاخص_منزل_لیکه))==SOLVELP_UNBOUNDED){
        أرجع SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.ستون_شمېر,1,CV_64FC1);
    MatIterator_<مزدوج> it=z.begin<مزدوج>();
    بینښه صحيح nsize = (بینښه صحيح)N.size();
    لكل(صحيح i=1;i<=c.ستون_شمېر;i++,it++){
        إذا(شاخص_منزل_لیکه[i]<nsize){
            *it=0;
        }وإلا{
            *it=b.at<مزدوج>(شاخص_منزل_لیکه[i]-nsize,b.ستون_شمېر-1);
        }
    }

    z.copy_منزل(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.ستون_شمېر - 1, Constr.لیکې)) * z;
    Mat constr_check = Constr.ستون(Constr.ستون_شمېر - 1) - prod;
    مزدوج min_ارزښت = 0.0;
    minMaxIdx(constr_check, &min_ارزښت);
    إذا (min_ارزښت < -constr_eps)
    {
        أرجع SOLVELP_ورک;
    }

    أرجع res;
}

صحيح solveLP(ننوت_Array Func, ننوت_Array Constr, OutputArray z)
{
    أرجع solveLP(Func, Constr, z, 1e-12);
}

ساكن صحيح initialize_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بینښه صحيح>& شاخص_منزل_لیکه){
    N.resize(c.ستون_شمېر);
    N[0]=0;
    لكل (std::vector<صحيح>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.لیکې);
    B[0]=(صحيح)N.size();
    لكل (std::vector<صحيح>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    شاخص_منزل_لیکه.resize(c.ستون_شمېر+b.لیکې);
    شاخص_منزل_لیکه[0]=0;
    لكل (std::vector<بینښه صحيح>::iterator it = شاخص_منزل_لیکه.begin()+1 ; it != شاخص_منزل_لیکه.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    صحيح k=0;
    {
        مزدوج min=DBL_MAX;
        لكل(صحيح i=0;i<b.لیکې;i++){
            إذا(b(i,b.ستون_شمېر-1)<min){
                min=b(i,b.ستون_شمېر-1);
                k=i;
            }
        }
    }

    إذا(b(k,b.ستون_شمېر-1)>=0){
        N.erase(N.begin());
        لكل (std::vector<بینښه صحيح>::iterator it = شاخص_منزل_لیکه.begin()+1 ; it != شاخص_منزل_لیکه.end(); ++it){
            --(*it);
        }
        أرجع 0;
    }

    Mat_<مزدوج> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    لكل(صحيح i=0;i<b.لیکې;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    محور(c,b,v,N,B,k,0,شاخص_منزل_لیکه);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,شاخص_منزل_لیکه);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    بینښه صحيح nsize = (بینښه صحيح)N.size();
    إذا(شاخص_منزل_لیکه[0]>=nsize){
        صحيح iterator_دننه_واټن=شاخص_منزل_لیکه[0]-nsize;
        إذا(b(iterator_دننه_واټن,b.ستون_شمېر-1)>0){
            أرجع SOLVELP_UNFEASIBLE;
        }
        محور(c,b,v,N,B,iterator_دننه_واټن,0,شاخص_منزل_لیکه);
    }

    vector<صحيح>::iterator iterator;
    {
        صحيح iterator_دننه_واټن=شاخص_منزل_لیکه[0];
        iterator=N.begin()+iterator_دننه_واټن;
        std::iter_swap(iterator,N.begin());
        بدلون(صحيح,شاخص_منزل_لیکه[*iterator],شاخص_منزل_لیکه[0]);
        بدلون_columns(c,iterator_دننه_واټن,0);
        بدلون_columns(b,iterator_دننه_واټن,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    لكل(صحيح I=1;I<old_c.ستون_شمېر;I++){
        إذا(شاخص_منزل_لیکه[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            صحيح iterator_دننه_واټن=شاخص_منزل_لیکه[I];
            c(0,iterator_دننه_واټن)+=old_c(0,I);
            print_ماتریکس(c);
        }وإلا{
            dprintf(("I=%d from basic\n",I));
            صحيح iterator_دننه_واټن=شاخص_منزل_لیکه[I]-nsize;
            c-=old_c(0,I)*b.لیکه(iterator_دننه_واټن).ستون_Range_2(0,b.ستون_شمېر-1);
            v+=old_c(0,I)*b(iterator_دننه_واټن,b.ستون_شمېر-1);
            print_ماتریکس(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    لكل (std::vector<بینښه صحيح>::iterator it = شاخص_منزل_لیکه.begin()+1 ; it != شاخص_منزل_لیکه.end(); ++it){
        --(*it);
    }
    أرجع 0;
}

ساكن صحيح inner_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بینښه صحيح>& شاخص_منزل_لیکه){

    لكل(;;){
        MatIterator_<مزدوج> pos_ptr;
        صحيح e=-1,pos_ctr=0,min_var=لوی_عدد;
        منطقي all_nonzero=ریښتیا;
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
            print_ماتریکس(c);
            إذا(all_nonzero==ریښتیا){
                أرجع SOLVELP_SINGLE;
            }وإلا{
                أرجع SOLVELP_MULTI;
            }
        }

        صحيح l=-1;
        min_var=لوی_عدد;
        مزدوج min=DBL_MAX;
        صحيح لیکه_it=0;
        MatIterator_<مزدوج> min_لیکه_ptr=b.begin();
        لكل(MatIterator_<مزدوج> it=b.begin();it!=b.end();it+=b.ستون_شمېر,لیکه_it++){
            مزدوج myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            إذا((myite=it[e])>0){
                مزدوج val=it[b.ستون_شمېر-1]/myite;
                إذا(val<min || (val==min && B[لیکه_it]<min_var)){
                    min_var=B[لیکه_it];
                    min_لیکه_ptr=it;
                    min=val;
                    l=لیکه_it;
                }
            }
        }
        إذا(l==-1){
            أرجع SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        محور(c,b,v,N,B,l,e,شاخص_منزل_لیکه);

        dprintf(("objective, v=%g\n",v));
        print_ماتریکس(c);
        dprintf(("constraints\n"));
        print_ماتریکس(b);
        dprintf(("non-basic: "));
        print_ماتریکس(Mat(N));
        dprintf(("basic: "));
        print_ماتریکس(Mat(B));
    }
}

ساكن مضمن فراغ محور(Mat_<مزدوج>& c,Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,
        صحيح leaving_شاخص,صحيح entering_شاخص,vector<بینښه صحيح>& شاخص_منزل_لیکه){
    مزدوج Coef=b(leaving_شاخص,entering_شاخص);
    لكل(صحيح i=0;i<b.ستون_شمېر;i++){
        إذا(i==entering_شاخص){
            b(leaving_شاخص,i)=1/Coef;
        }وإلا{
            b(leaving_شاخص,i)/=Coef;
        }
    }

    لكل(صحيح i=0;i<b.لیکې;i++){
        إذا(i!=leaving_شاخص){
            مزدوج coef=b(i,entering_شاخص);
            لكل(صحيح j=0;j<b.ستون_شمېر;j++){
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
    لكل(صحيح i=0;i<(b.ستون_شمېر-1);i++){
        إذا(i==entering_شاخص){
            c(0,i)=-Coef*b(leaving_شاخص,i);
        }وإلا{
            c(0,i)-=Coef*b(leaving_شاخص,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_شاخص,b.ستون_شمېر-1);

    بدلون(صحيح,N[entering_شاخص],B[leaving_شاخص]);
    بدلون(صحيح,شاخص_منزل_لیکه[N[entering_شاخص]],شاخص_منزل_لیکه[B[leaving_شاخص]]);
}

ساكن مضمن فراغ بدلون_columns(Mat_<مزدوج>& A,صحيح col1,صحيح col2){
    لكل(صحيح i=0;i<A.لیکې;i++){
        مزدوج tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
