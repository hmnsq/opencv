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

#define dprintf(x)
#define print_matrix(x)

فضاینام cv
{

using std::vector;

#ifdef ALEX_DEBUG
ایستا تهی print_simplex_state(ثابت Mat& c,ثابت Mat& b,دوگانه v,ثابت std::vector<صحیح> N,ثابت std::vector<صحیح> B){
    printf("\tprint simplex state\n");

    printf("v=%g\n",v);

    printf("here c goes\n");
    print_matrix(c);

    printf("non-basic: ");
    print(Mat(N));
    printf("\n");

    printf("here b goes\n");
    print_matrix(b);
    printf("basic: ");

    print(Mat(B));
    printf("\n");
}
#وگرنه
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ایستا صحیح initialize_simplex(Mat_<دوگانه>& c, Mat_<دوگانه>& b,دوگانه& v,vector<صحیح>& N,vector<صحیح>& B,vector<unsigned صحیح>& indexToRow);
ایستا درونخطی تهی pivot(Mat_<دوگانه>& c,Mat_<دوگانه>& b,دوگانه& v,vector<صحیح>& N,vector<صحیح>& B,صحیح leaving_index,
        صحیح entering_index,vector<unsigned صحیح>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ایستا صحیح inner_simplex(Mat_<دوگانه>& c, Mat_<دوگانه>& b,دوگانه& v,vector<صحیح>& N,vector<صحیح>& B,vector<unsigned صحیح>& indexToRow);
ایستا تهی swap_columns(Mat_<دوگانه>& A,صحیح col1,صحیح col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
صحیح solveLP(درون_داد_Array Func_, درون_داد_Array Constr_, OutputArray z_, دوگانه constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    اگر (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<دوگانه> bigC=Mat_<دوگانه>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<دوگانه>(Constr.rows,Constr.cols+1);
    اگر(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }وگرنه{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    دوگانه v=0;
    vector<صحیح> N,B;
    vector<unsigned صحیح> indexToRow;

    اگر(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        بازگشت SOLVELP_UNFEASIBLE;
    }
    Mat_<دوگانه> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    صحیح res=0;
    اگر((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        بازگشت SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<دوگانه> it=z.begin<دوگانه>();
    unsigned صحیح nsize = (unsigned صحیح)N.size();
    برای(صحیح i=1;i<=c.cols;i++,it++){
        اگر(indexToRow[i]<nsize){
            *it=0;
        }وگرنه{
            *it=b.at<دوگانه>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    دوگانه min_مقدار = 0.0;
    minMaxIdx(constr_check, &min_مقدار);
    اگر (min_مقدار < -constr_eps)
    {
        بازگشت SOLVELP_LOST;
    }

    بازگشت res;
}

صحیح solveLP(درون_داد_Array Func, درون_داد_Array Constr, OutputArray z)
{
    بازگشت solveLP(Func, Constr, z, 1e-12);
}

ایستا صحیح initialize_simplex(Mat_<دوگانه>& c, Mat_<دوگانه>& b,دوگانه& v,vector<صحیح>& N,vector<صحیح>& B,vector<unsigned صحیح>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    برای (std::vector<صحیح>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(صحیح)N.size();
    برای (std::vector<صحیح>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    برای (std::vector<unsigned صحیح>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    صحیح k=0;
    {
        دوگانه min=DBL_MAX;
        برای(صحیح i=0;i<b.rows;i++){
            اگر(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    اگر(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        برای (std::vector<unsigned صحیح>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        بازگشت 0;
    }

    Mat_<دوگانه> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    برای(صحیح i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned صحیح nsize = (unsigned صحیح)N.size();
    اگر(indexToRow[0]>=nsize){
        صحیح iterator_offset=indexToRow[0]-nsize;
        اگر(b(iterator_offset,b.cols-1)>0){
            بازگشت SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<صحیح>::iterator iterator;
    {
        صحیح iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(صحیح,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    برای(صحیح I=1;I<old_c.cols;I++){
        اگر(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            صحیح iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }وگرنه{
            dprintf(("I=%d from basic\n",I));
            صحیح iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    برای (std::vector<unsigned صحیح>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    بازگشت 0;
}

ایستا صحیح inner_simplex(Mat_<دوگانه>& c, Mat_<دوگانه>& b,دوگانه& v,vector<صحیح>& N,vector<صحیح>& B,vector<unsigned صحیح>& indexToRow){

    برای(;;){
        MatIterator_<دوگانه> pos_ptr;
        صحیح e=-1,pos_ctr=0,min_var=INT_MAX;
        منطقی all_nonzero=درست;
        برای(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            اگر(*pos_ptr==0){
                all_nonzero=نادرست;
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
            print_matrix(c);
            اگر(all_nonzero==درست){
                بازگشت SOLVELP_SINGLE;
            }وگرنه{
                بازگشت SOLVELP_MULTI;
            }
        }

        صحیح l=-1;
        min_var=INT_MAX;
        دوگانه min=DBL_MAX;
        صحیح row_it=0;
        MatIterator_<دوگانه> min_row_ptr=b.begin();
        برای(MatIterator_<دوگانه> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            دوگانه myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            اگر((myite=it[e])>0){
                دوگانه val=it[b.cols-1]/myite;
                اگر(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        اگر(l==-1){
            بازگشت SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        pivot(c,b,v,N,B,l,e,indexToRow);

        dprintf(("objective, v=%g\n",v));
        print_matrix(c);
        dprintf(("constraints\n"));
        print_matrix(b);
        dprintf(("non-basic: "));
        print_matrix(Mat(N));
        dprintf(("basic: "));
        print_matrix(Mat(B));
    }
}

ایستا درونخطی تهی pivot(Mat_<دوگانه>& c,Mat_<دوگانه>& b,دوگانه& v,vector<صحیح>& N,vector<صحیح>& B,
        صحیح leaving_index,صحیح entering_index,vector<unsigned صحیح>& indexToRow){
    دوگانه Coef=b(leaving_index,entering_index);
    برای(صحیح i=0;i<b.cols;i++){
        اگر(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }وگرنه{
            b(leaving_index,i)/=Coef;
        }
    }

    برای(صحیح i=0;i<b.rows;i++){
        اگر(i!=leaving_index){
            دوگانه coef=b(i,entering_index);
            برای(صحیح j=0;j<b.cols;j++){
                اگر(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }وگرنه{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    برای(صحیح i=0;i<(b.cols-1);i++){
        اگر(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }وگرنه{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(صحیح,N[entering_index],B[leaving_index]);
    SWAP(صحیح,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

ایستا درونخطی تهی swap_columns(Mat_<دوگانه>& A,صحیح col1,صحیح col2){
    برای(صحیح i=0;i<A.rows;i++){
        دوگانه tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
