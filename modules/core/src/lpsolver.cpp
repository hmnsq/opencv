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

#تعریف_کرو dprintf(x)
#تعریف_کرو print_میٹرکس(x)

نطاق cv
{

ورتوں std::vector;

#جے_تعریف_شدہ ALEX_DEBUG
ساكن فراغ print_simplex_state(ثابت Mat& c,ثابت Mat& b,مزدوج v,ثابت std::vector<صحيح> N,ثابت std::vector<صحيح> B){
    لکھو("\tprint simplex state\n");

    لکھو("v=%g\n",v);

    لکھو("here c goes\n");
    print_میٹرکس(c);

    لکھو("non-basic: ");
    print(Mat(N));
    لکھو("\n");

    لکھو("here b goes\n");
    print_میٹرکس(b);
    لکھو("basic: ");

    print(Mat(B));
    لکھو("\n");
}
#وإلا
#تعریف_کرو print_simplex_state(c,b,v,N,B)
#جے_انت

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ساكن صحيح initialize_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بنانشانی صحيح>& اشاریہ_منزل_صف);
ساكن مضمن فراغ دھرا(Mat_<مزدوج>& c,Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,صحيح leaving_اشاریہ,
        صحيح entering_اشاریہ,vector<بنانشانی صحيح>& اشاریہ_منزل_صف);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ساكن صحيح inner_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بنانشانی صحيح>& اشاریہ_منزل_صف);
ساكن فراغ وٹاندرا_columns(Mat_<مزدوج>& A,صحيح col1,صحيح col2);
#تعریف_کرو وٹاندرا(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
صحيح solveLP(داخلہ_Array Func_, داخلہ_Array Constr_, OutputArray z_, مزدوج constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.صفاں()==1 && (Constr_.خانے_گنتی()-Func_.خانے_گنتی()==1))||
            (Func_.خانے_گنتی()==1 && (Constr_.خانے_گنتی()-Func_.صفاں()==1)));
    إذا (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<مزدوج> bigC=Mat_<مزدوج>(1,(Func.صفاں==1?Func.خانے_گنتی:Func.صفاں)+1),
        bigB=Mat_<مزدوج>(Constr.صفاں,Constr.خانے_گنتی+1);
    إذا(Func.صفاں==1){
        Func.convert_منزل(bigC.خانہ_Range_2(1,bigC.خانے_گنتی),CV_64FC1);
    }وإلا{
        Mat FuncT=Func.t();
        FuncT.convert_منزل(bigC.خانہ_Range_2(1,bigC.خانے_گنتی),CV_64FC1);
    }
    Constr.convert_منزل(bigB.خانہ_Range_2(1,bigB.خانے_گنتی),CV_64FC1);
    مزدوج v=0;
    vector<صحيح> N,B;
    vector<بنانشانی صحيح> اشاریہ_منزل_صف;

    إذا(initialize_simplex(bigC,bigB,v,N,B,اشاریہ_منزل_صف)==SOLVELP_UNFEASIBLE){
        أرجع SOLVELP_UNFEASIBLE;
    }
    Mat_<مزدوج> c=bigC.خانہ_Range_2(1,bigC.خانے_گنتی),
        b=bigB.خانہ_Range_2(1,bigB.خانے_گنتی);

    صحيح res=0;
    إذا((res=inner_simplex(c,b,v,N,B,اشاریہ_منزل_صف))==SOLVELP_UNBOUNDED){
        أرجع SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.خانے_گنتی,1,CV_64FC1);
    MatIterator_<مزدوج> it=z.begin<مزدوج>();
    بنانشانی صحيح nsize = (بنانشانی صحيح)N.size();
    لكل(صحيح i=1;i<=c.خانے_گنتی;i++,it++){
        إذا(اشاریہ_منزل_صف[i]<nsize){
            *it=0;
        }وإلا{
            *it=b.at<مزدوج>(اشاریہ_منزل_صف[i]-nsize,b.خانے_گنتی-1);
        }
    }

    z.copy_منزل(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.خانے_گنتی - 1, Constr.صفاں)) * z;
    Mat constr_check = Constr.خانہ(Constr.خانے_گنتی - 1) - prod;
    مزدوج min_قدر = 0.0;
    minMaxIdx(constr_check, &min_قدر);
    إذا (min_قدر < -constr_eps)
    {
        أرجع SOLVELP_گواچا;
    }

    أرجع res;
}

صحيح solveLP(داخلہ_Array Func, داخلہ_Array Constr, OutputArray z)
{
    أرجع solveLP(Func, Constr, z, 1e-12);
}

ساكن صحيح initialize_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بنانشانی صحيح>& اشاریہ_منزل_صف){
    N.resize(c.خانے_گنتی);
    N[0]=0;
    لكل (std::vector<صحيح>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.صفاں);
    B[0]=(صحيح)N.size();
    لكل (std::vector<صحيح>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    اشاریہ_منزل_صف.resize(c.خانے_گنتی+b.صفاں);
    اشاریہ_منزل_صف[0]=0;
    لكل (std::vector<بنانشانی صحيح>::iterator it = اشاریہ_منزل_صف.begin()+1 ; it != اشاریہ_منزل_صف.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    صحيح k=0;
    {
        مزدوج min=DBL_MAX;
        لكل(صحيح i=0;i<b.صفاں;i++){
            إذا(b(i,b.خانے_گنتی-1)<min){
                min=b(i,b.خانے_گنتی-1);
                k=i;
            }
        }
    }

    إذا(b(k,b.خانے_گنتی-1)>=0){
        N.erase(N.begin());
        لكل (std::vector<بنانشانی صحيح>::iterator it = اشاریہ_منزل_صف.begin()+1 ; it != اشاریہ_منزل_صف.end(); ++it){
            --(*it);
        }
        أرجع 0;
    }

    Mat_<مزدوج> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    لكل(صحيح i=0;i<b.صفاں;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    دھرا(c,b,v,N,B,k,0,اشاریہ_منزل_صف);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,اشاریہ_منزل_صف);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    بنانشانی صحيح nsize = (بنانشانی صحيح)N.size();
    إذا(اشاریہ_منزل_صف[0]>=nsize){
        صحيح iterator_وتھ=اشاریہ_منزل_صف[0]-nsize;
        إذا(b(iterator_وتھ,b.خانے_گنتی-1)>0){
            أرجع SOLVELP_UNFEASIBLE;
        }
        دھرا(c,b,v,N,B,iterator_وتھ,0,اشاریہ_منزل_صف);
    }

    vector<صحيح>::iterator iterator;
    {
        صحيح iterator_وتھ=اشاریہ_منزل_صف[0];
        iterator=N.begin()+iterator_وتھ;
        std::iter_swap(iterator,N.begin());
        وٹاندرا(صحيح,اشاریہ_منزل_صف[*iterator],اشاریہ_منزل_صف[0]);
        وٹاندرا_columns(c,iterator_وتھ,0);
        وٹاندرا_columns(b,iterator_وتھ,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    لكل(صحيح I=1;I<old_c.خانے_گنتی;I++){
        إذا(اشاریہ_منزل_صف[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            صحيح iterator_وتھ=اشاریہ_منزل_صف[I];
            c(0,iterator_وتھ)+=old_c(0,I);
            print_میٹرکس(c);
        }وإلا{
            dprintf(("I=%d from basic\n",I));
            صحيح iterator_وتھ=اشاریہ_منزل_صف[I]-nsize;
            c-=old_c(0,I)*b.صف(iterator_وتھ).خانہ_Range_2(0,b.خانے_گنتی-1);
            v+=old_c(0,I)*b(iterator_وتھ,b.خانے_گنتی-1);
            print_میٹرکس(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    لكل (std::vector<بنانشانی صحيح>::iterator it = اشاریہ_منزل_صف.begin()+1 ; it != اشاریہ_منزل_صف.end(); ++it){
        --(*it);
    }
    أرجع 0;
}

ساكن صحيح inner_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<بنانشانی صحيح>& اشاریہ_منزل_صف){

    لكل(;;){
        MatIterator_<مزدوج> pos_ptr;
        صحيح e=-1,pos_ctr=0,min_var=ودھ_توں_ودھ_عدد;
        منطقي all_nonzero=سچ;
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
            print_میٹرکس(c);
            إذا(all_nonzero==سچ){
                أرجع SOLVELP_SINGLE;
            }وإلا{
                أرجع SOLVELP_MULTI;
            }
        }

        صحيح l=-1;
        min_var=ودھ_توں_ودھ_عدد;
        مزدوج min=DBL_MAX;
        صحيح صف_it=0;
        MatIterator_<مزدوج> min_صف_ptr=b.begin();
        لكل(MatIterator_<مزدوج> it=b.begin();it!=b.end();it+=b.خانے_گنتی,صف_it++){
            مزدوج myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            إذا((myite=it[e])>0){
                مزدوج val=it[b.خانے_گنتی-1]/myite;
                إذا(val<min || (val==min && B[صف_it]<min_var)){
                    min_var=B[صف_it];
                    min_صف_ptr=it;
                    min=val;
                    l=صف_it;
                }
            }
        }
        إذا(l==-1){
            أرجع SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        دھرا(c,b,v,N,B,l,e,اشاریہ_منزل_صف);

        dprintf(("objective, v=%g\n",v));
        print_میٹرکس(c);
        dprintf(("constraints\n"));
        print_میٹرکس(b);
        dprintf(("non-basic: "));
        print_میٹرکس(Mat(N));
        dprintf(("basic: "));
        print_میٹرکس(Mat(B));
    }
}

ساكن مضمن فراغ دھرا(Mat_<مزدوج>& c,Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,
        صحيح leaving_اشاریہ,صحيح entering_اشاریہ,vector<بنانشانی صحيح>& اشاریہ_منزل_صف){
    مزدوج Coef=b(leaving_اشاریہ,entering_اشاریہ);
    لكل(صحيح i=0;i<b.خانے_گنتی;i++){
        إذا(i==entering_اشاریہ){
            b(leaving_اشاریہ,i)=1/Coef;
        }وإلا{
            b(leaving_اشاریہ,i)/=Coef;
        }
    }

    لكل(صحيح i=0;i<b.صفاں;i++){
        إذا(i!=leaving_اشاریہ){
            مزدوج coef=b(i,entering_اشاریہ);
            لكل(صحيح j=0;j<b.خانے_گنتی;j++){
                إذا(j==entering_اشاریہ){
                    b(i,j)=-coef*b(leaving_اشاریہ,j);
                }وإلا{
                    b(i,j)-=(coef*b(leaving_اشاریہ,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_اشاریہ);
    لكل(صحيح i=0;i<(b.خانے_گنتی-1);i++){
        إذا(i==entering_اشاریہ){
            c(0,i)=-Coef*b(leaving_اشاریہ,i);
        }وإلا{
            c(0,i)-=Coef*b(leaving_اشاریہ,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_اشاریہ,b.خانے_گنتی-1);

    وٹاندرا(صحيح,N[entering_اشاریہ],B[leaving_اشاریہ]);
    وٹاندرا(صحيح,اشاریہ_منزل_صف[N[entering_اشاریہ]],اشاریہ_منزل_صف[B[leaving_اشاریہ]]);
}

ساكن مضمن فراغ وٹاندرا_columns(Mat_<مزدوج>& A,صحيح col1,صحيح col2){
    لكل(صحيح i=0;i<A.صفاں;i++){
        مزدوج tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
