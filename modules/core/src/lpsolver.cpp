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
#شامل "precomp.hpp"
#شامل <climits>
#شامل <algorithm>

#تعریف_کرو dprintf(x)
#تعریف_کرو print_میٹرکس(x)

نامخلا cv
{

استعمال std::vector;

#اگر_تعریف_شدہ ALEX_DEBUG
ساکن خالی print_simplex_state(ثابت Mat& c,ثابت Mat& b,دوہرا v,ثابت std::vector<صحیح> N,ثابت std::vector<صحیح> B){
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
#ورنہ
#تعریف_کرو print_simplex_state(c,b,v,N,B)
#اگر_ختم

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ساکن صحیح initialize_simplex(Mat_<دوہرا>& c, Mat_<دوہرا>& b,دوہرا& v,vector<صحیح>& N,vector<صحیح>& B,vector<غیرعلامتی صحیح>& اشاریہ_مقصد_صف);
ساکن اندرونی خالی محور(Mat_<دوہرا>& c,Mat_<دوہرا>& b,دوہرا& v,vector<صحیح>& N,vector<صحیح>& B,صحیح leaving_اشاریہ,
        صحیح entering_اشاریہ,vector<غیرعلامتی صحیح>& اشاریہ_مقصد_صف);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ساکن صحیح inner_simplex(Mat_<دوہرا>& c, Mat_<دوہرا>& b,دوہرا& v,vector<صحیح>& N,vector<صحیح>& B,vector<غیرعلامتی صحیح>& اشاریہ_مقصد_صف);
ساکن خالی تبادلہ_columns(Mat_<دوہرا>& A,صحیح col1,صحیح col2);
#تعریف_کرو تبادلہ(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
صحیح solveLP(داخلہ_Array Func_, داخلہ_Array Constr_, OutputArray z_, دوہرا constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.صفیں()==1 && (Constr_.ستون_تعداد()-Func_.ستون_تعداد()==1))||
            (Func_.ستون_تعداد()==1 && (Constr_.ستون_تعداد()-Func_.صفیں()==1)));
    اگر (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<دوہرا> bigC=Mat_<دوہرا>(1,(Func.صفیں==1?Func.ستون_تعداد:Func.صفیں)+1),
        bigB=Mat_<دوہرا>(Constr.صفیں,Constr.ستون_تعداد+1);
    اگر(Func.صفیں==1){
        Func.convert_مقصد(bigC.ستون_Range_2(1,bigC.ستون_تعداد),CV_64FC1);
    }ورنہ{
        Mat FuncT=Func.t();
        FuncT.convert_مقصد(bigC.ستون_Range_2(1,bigC.ستون_تعداد),CV_64FC1);
    }
    Constr.convert_مقصد(bigB.ستون_Range_2(1,bigB.ستون_تعداد),CV_64FC1);
    دوہرا v=0;
    vector<صحیح> N,B;
    vector<غیرعلامتی صحیح> اشاریہ_مقصد_صف;

    اگر(initialize_simplex(bigC,bigB,v,N,B,اشاریہ_مقصد_صف)==SOLVELP_UNFEASIBLE){
        واپس SOLVELP_UNFEASIBLE;
    }
    Mat_<دوہرا> c=bigC.ستون_Range_2(1,bigC.ستون_تعداد),
        b=bigB.ستون_Range_2(1,bigB.ستون_تعداد);

    صحیح res=0;
    اگر((res=inner_simplex(c,b,v,N,B,اشاریہ_مقصد_صف))==SOLVELP_UNBOUNDED){
        واپس SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.ستون_تعداد,1,CV_64FC1);
    MatIterator_<دوہرا> it=z.begin<دوہرا>();
    غیرعلامتی صحیح nsize = (غیرعلامتی صحیح)N.size();
    برائے(صحیح i=1;i<=c.ستون_تعداد;i++,it++){
        اگر(اشاریہ_مقصد_صف[i]<nsize){
            *it=0;
        }ورنہ{
            *it=b.at<دوہرا>(اشاریہ_مقصد_صف[i]-nsize,b.ستون_تعداد-1);
        }
    }

    z.copy_مقصد(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.ستون_تعداد - 1, Constr.صفیں)) * z;
    Mat constr_check = Constr.ستون(Constr.ستون_تعداد - 1) - prod;
    دوہرا min_قدر = 0.0;
    minMaxIdx(constr_check, &min_قدر);
    اگر (min_قدر < -constr_eps)
    {
        واپس SOLVELP_گمشدہ;
    }

    واپس res;
}

صحیح solveLP(داخلہ_Array Func, داخلہ_Array Constr, OutputArray z)
{
    واپس solveLP(Func, Constr, z, 1e-12);
}

ساکن صحیح initialize_simplex(Mat_<دوہرا>& c, Mat_<دوہرا>& b,دوہرا& v,vector<صحیح>& N,vector<صحیح>& B,vector<غیرعلامتی صحیح>& اشاریہ_مقصد_صف){
    N.resize(c.ستون_تعداد);
    N[0]=0;
    برائے (std::vector<صحیح>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.صفیں);
    B[0]=(صحیح)N.size();
    برائے (std::vector<صحیح>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    اشاریہ_مقصد_صف.resize(c.ستون_تعداد+b.صفیں);
    اشاریہ_مقصد_صف[0]=0;
    برائے (std::vector<غیرعلامتی صحیح>::iterator it = اشاریہ_مقصد_صف.begin()+1 ; it != اشاریہ_مقصد_صف.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    صحیح k=0;
    {
        دوہرا min=DBL_MAX;
        برائے(صحیح i=0;i<b.صفیں;i++){
            اگر(b(i,b.ستون_تعداد-1)<min){
                min=b(i,b.ستون_تعداد-1);
                k=i;
            }
        }
    }

    اگر(b(k,b.ستون_تعداد-1)>=0){
        N.erase(N.begin());
        برائے (std::vector<غیرعلامتی صحیح>::iterator it = اشاریہ_مقصد_صف.begin()+1 ; it != اشاریہ_مقصد_صف.end(); ++it){
            --(*it);
        }
        واپس 0;
    }

    Mat_<دوہرا> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    برائے(صحیح i=0;i<b.صفیں;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    محور(c,b,v,N,B,k,0,اشاریہ_مقصد_صف);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,اشاریہ_مقصد_صف);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    غیرعلامتی صحیح nsize = (غیرعلامتی صحیح)N.size();
    اگر(اشاریہ_مقصد_صف[0]>=nsize){
        صحیح iterator_انحراف=اشاریہ_مقصد_صف[0]-nsize;
        اگر(b(iterator_انحراف,b.ستون_تعداد-1)>0){
            واپس SOLVELP_UNFEASIBLE;
        }
        محور(c,b,v,N,B,iterator_انحراف,0,اشاریہ_مقصد_صف);
    }

    vector<صحیح>::iterator iterator;
    {
        صحیح iterator_انحراف=اشاریہ_مقصد_صف[0];
        iterator=N.begin()+iterator_انحراف;
        std::iter_swap(iterator,N.begin());
        تبادلہ(صحیح,اشاریہ_مقصد_صف[*iterator],اشاریہ_مقصد_صف[0]);
        تبادلہ_columns(c,iterator_انحراف,0);
        تبادلہ_columns(b,iterator_انحراف,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    برائے(صحیح I=1;I<old_c.ستون_تعداد;I++){
        اگر(اشاریہ_مقصد_صف[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            صحیح iterator_انحراف=اشاریہ_مقصد_صف[I];
            c(0,iterator_انحراف)+=old_c(0,I);
            print_میٹرکس(c);
        }ورنہ{
            dprintf(("I=%d from basic\n",I));
            صحیح iterator_انحراف=اشاریہ_مقصد_صف[I]-nsize;
            c-=old_c(0,I)*b.صف(iterator_انحراف).ستون_Range_2(0,b.ستون_تعداد-1);
            v+=old_c(0,I)*b(iterator_انحراف,b.ستون_تعداد-1);
            print_میٹرکس(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    برائے (std::vector<غیرعلامتی صحیح>::iterator it = اشاریہ_مقصد_صف.begin()+1 ; it != اشاریہ_مقصد_صف.end(); ++it){
        --(*it);
    }
    واپس 0;
}

ساکن صحیح inner_simplex(Mat_<دوہرا>& c, Mat_<دوہرا>& b,دوہرا& v,vector<صحیح>& N,vector<صحیح>& B,vector<غیرعلامتی صحیح>& اشاریہ_مقصد_صف){

    برائے(;;){
        MatIterator_<دوہرا> pos_ptr;
        صحیح e=-1,pos_ctr=0,min_var=زیادہ_سے_زیادہ_عدد;
        منطقی all_nonzero=سچ;
        برائے(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            اگر(*pos_ptr==0){
                all_nonzero=جھوٹ;
            }
            اگر(*pos_ptr>0){
                اگر(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        اگر(e==-1){
            dprintf(("hello from e==-1\n"));
            print_میٹرکس(c);
            اگر(all_nonzero==سچ){
                واپس SOLVELP_SINGLE;
            }ورنہ{
                واپس SOLVELP_MULTI;
            }
        }

        صحیح l=-1;
        min_var=زیادہ_سے_زیادہ_عدد;
        دوہرا min=DBL_MAX;
        صحیح صف_it=0;
        MatIterator_<دوہرا> min_صف_ptr=b.begin();
        برائے(MatIterator_<دوہرا> it=b.begin();it!=b.end();it+=b.ستون_تعداد,صف_it++){
            دوہرا myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            اگر((myite=it[e])>0){
                دوہرا val=it[b.ستون_تعداد-1]/myite;
                اگر(val<min || (val==min && B[صف_it]<min_var)){
                    min_var=B[صف_it];
                    min_صف_ptr=it;
                    min=val;
                    l=صف_it;
                }
            }
        }
        اگر(l==-1){
            واپس SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        محور(c,b,v,N,B,l,e,اشاریہ_مقصد_صف);

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

ساکن اندرونی خالی محور(Mat_<دوہرا>& c,Mat_<دوہرا>& b,دوہرا& v,vector<صحیح>& N,vector<صحیح>& B,
        صحیح leaving_اشاریہ,صحیح entering_اشاریہ,vector<غیرعلامتی صحیح>& اشاریہ_مقصد_صف){
    دوہرا Coef=b(leaving_اشاریہ,entering_اشاریہ);
    برائے(صحیح i=0;i<b.ستون_تعداد;i++){
        اگر(i==entering_اشاریہ){
            b(leaving_اشاریہ,i)=1/Coef;
        }ورنہ{
            b(leaving_اشاریہ,i)/=Coef;
        }
    }

    برائے(صحیح i=0;i<b.صفیں;i++){
        اگر(i!=leaving_اشاریہ){
            دوہرا coef=b(i,entering_اشاریہ);
            برائے(صحیح j=0;j<b.ستون_تعداد;j++){
                اگر(j==entering_اشاریہ){
                    b(i,j)=-coef*b(leaving_اشاریہ,j);
                }ورنہ{
                    b(i,j)-=(coef*b(leaving_اشاریہ,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_اشاریہ);
    برائے(صحیح i=0;i<(b.ستون_تعداد-1);i++){
        اگر(i==entering_اشاریہ){
            c(0,i)=-Coef*b(leaving_اشاریہ,i);
        }ورنہ{
            c(0,i)-=Coef*b(leaving_اشاریہ,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_اشاریہ,b.ستون_تعداد-1);

    تبادلہ(صحیح,N[entering_اشاریہ],B[leaving_اشاریہ]);
    تبادلہ(صحیح,اشاریہ_مقصد_صف[N[entering_اشاریہ]],اشاریہ_مقصد_صف[B[leaving_اشاریہ]]);
}

ساکن اندرونی خالی تبادلہ_columns(Mat_<دوہرا>& A,صحیح col1,صحیح col2){
    برائے(صحیح i=0;i<A.صفیں;i++){
        دوہرا tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
