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
#include "precomp.hpp"
#include <climits>
#include <algorithm>

#define dprintf(x)
#define print_matrix(x)

namespace cv
{

using std::vector;

#ifdef ALEX_DEBUG
static void print_simplex_state(const Mat& c,const Mat& b,dviguu_na v,const std::vector<පූර්ණ_සංඛ්‍යා> N,const std::vector<පූර්ණ_සංඛ්‍යා> B){
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
#නැතහොත්
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
static පූර්ණ_සංඛ්‍යා initialize_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<පූර්ණ_සංඛ්‍යා>& N,vector<පූර්ණ_සංඛ්‍යා>& B,vector<unsigned පූර්ණ_සංඛ්‍යා>& indexToRow);
static anta_hstha void pivot(Mat_<dviguu_na>& c,Mat_<dviguu_na>& b,dviguu_na& v,vector<පූර්ණ_සංඛ්‍යා>& N,vector<පූර්ණ_සංඛ්‍යා>& B,පූර්ණ_සංඛ්‍යා leaving_index,
        පූර්ණ_සංඛ්‍යා entering_index,vector<unsigned පූර්ණ_සංඛ්‍යා>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
static පූර්ණ_සංඛ්‍යා inner_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<පූර්ණ_සංඛ්‍යා>& N,vector<පූර්ණ_සංඛ්‍යා>& B,vector<unsigned පූර්ණ_සංඛ්‍යා>& indexToRow);
static void swap_columns(Mat_<dviguu_na>& A,පූර්ණ_සංඛ්‍යා col1,පූර්ණ_සංඛ්‍යා col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
පූර්ණ_සංඛ්‍යා solveLP(InputArray Func_, InputArray Constr_, OutputArray z_, dviguu_na constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    නම් (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<dviguu_na> bigC=Mat_<dviguu_na>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<dviguu_na>(Constr.rows,Constr.cols+1);
    නම්(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }නැතහොත්{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    dviguu_na v=0;
    vector<පූර්ණ_සංඛ්‍යා> N,B;
    vector<unsigned පූර්ණ_සංඛ්‍යා> indexToRow;

    නම්(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        ආපසු SOLVELP_UNFEASIBLE;
    }
    Mat_<dviguu_na> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    පූර්ණ_සංඛ්‍යා res=0;
    නම්((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        ආපසු SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<dviguu_na> it=z.begin<dviguu_na>();
    unsigned පූර්ණ_සංඛ්‍යා nsize = (unsigned පූර්ණ_සංඛ්‍යා)N.size();
    සඳහා(පූර්ණ_සංඛ්‍යා i=1;i<=c.cols;i++,it++){
        නම්(indexToRow[i]<nsize){
            *it=0;
        }නැතහොත්{
            *it=b.at<dviguu_na>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    dviguu_na min_value = 0.0;
    minMaxIdx(constr_check, &min_value);
    නම් (min_value < -constr_eps)
    {
        ආපසු SOLVELP_LOST;
    }

    ආපසු res;
}

පූර්ණ_සංඛ්‍යා solveLP(InputArray Func, InputArray Constr, OutputArray z)
{
    ආපසු solveLP(Func, Constr, z, 1e-12);
}

static පූර්ණ_සංඛ්‍යා initialize_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<පූර්ණ_සංඛ්‍යා>& N,vector<පූර්ණ_සංඛ්‍යා>& B,vector<unsigned පූර්ණ_සංඛ්‍යා>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    සඳහා (std::vector<පූර්ණ_සංඛ්‍යා>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(පූර්ණ_සංඛ්‍යා)N.size();
    සඳහා (std::vector<පූර්ණ_සංඛ්‍යා>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    සඳහා (std::vector<unsigned පූර්ණ_සංඛ්‍යා>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    පූර්ණ_සංඛ්‍යා k=0;
    {
        dviguu_na min=DBL_MAX;
        සඳහා(පූර්ණ_සංඛ්‍යා i=0;i<b.rows;i++){
            නම්(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    නම්(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        සඳහා (std::vector<unsigned පූර්ණ_සංඛ්‍යා>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        ආපසු 0;
    }

    Mat_<dviguu_na> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    සඳහා(පූර්ණ_සංඛ්‍යා i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned පූර්ණ_සංඛ්‍යා nsize = (unsigned පූර්ණ_සංඛ්‍යා)N.size();
    නම්(indexToRow[0]>=nsize){
        පූර්ණ_සංඛ්‍යා iterator_offset=indexToRow[0]-nsize;
        නම්(b(iterator_offset,b.cols-1)>0){
            ආපසු SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<පූර්ණ_සංඛ්‍යා>::iterator iterator;
    {
        පූර්ණ_සංඛ්‍යා iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(පූර්ණ_සංඛ්‍යා,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    සඳහා(පූර්ණ_සංඛ්‍යා I=1;I<old_c.cols;I++){
        නම්(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            පූර්ණ_සංඛ්‍යා iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }නැතහොත්{
            dprintf(("I=%d from basic\n",I));
            පූර්ණ_සංඛ්‍යා iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    සඳහා (std::vector<unsigned පූර්ණ_සංඛ්‍යා>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා inner_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<පූර්ණ_සංඛ්‍යා>& N,vector<පූර්ණ_සංඛ්‍යා>& B,vector<unsigned පූර්ණ_සංඛ්‍යා>& indexToRow){

    සඳහා(;;){
        MatIterator_<dviguu_na> pos_ptr;
        පූර්ණ_සංඛ්‍යා e=-1,pos_ctr=0,min_var=INT_MAX;
        bool all_nonzero=true;
        සඳහා(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            නම්(*pos_ptr==0){
                all_nonzero=false;
            }
            නම්(*pos_ptr>0){
                නම්(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        නම්(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            නම්(all_nonzero==true){
                ආපසු SOLVELP_SINGLE;
            }නැතහොත්{
                ආපසු SOLVELP_MULTI;
            }
        }

        පූර්ණ_සංඛ්‍යා l=-1;
        min_var=INT_MAX;
        dviguu_na min=DBL_MAX;
        පූර්ණ_සංඛ්‍යා row_it=0;
        MatIterator_<dviguu_na> min_row_ptr=b.begin();
        සඳහා(MatIterator_<dviguu_na> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            dviguu_na myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            නම්((myite=it[e])>0){
                dviguu_na val=it[b.cols-1]/myite;
                නම්(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        නම්(l==-1){
            ආපසු SOLVELP_UNBOUNDED;
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

static anta_hstha void pivot(Mat_<dviguu_na>& c,Mat_<dviguu_na>& b,dviguu_na& v,vector<පූර්ණ_සංඛ්‍යා>& N,vector<පූර්ණ_සංඛ්‍යා>& B,
        පූර්ණ_සංඛ්‍යා leaving_index,පූර්ණ_සංඛ්‍යා entering_index,vector<unsigned පූර්ණ_සංඛ්‍යා>& indexToRow){
    dviguu_na Coef=b(leaving_index,entering_index);
    සඳහා(පූර්ණ_සංඛ්‍යා i=0;i<b.cols;i++){
        නම්(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }නැතහොත්{
            b(leaving_index,i)/=Coef;
        }
    }

    සඳහා(පූර්ණ_සංඛ්‍යා i=0;i<b.rows;i++){
        නම්(i!=leaving_index){
            dviguu_na coef=b(i,entering_index);
            සඳහා(පූර්ණ_සංඛ්‍යා j=0;j<b.cols;j++){
                නම්(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }නැතහොත්{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    සඳහා(පූර්ණ_සංඛ්‍යා i=0;i<(b.cols-1);i++){
        නම්(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }නැතහොත්{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(පූර්ණ_සංඛ්‍යා,N[entering_index],B[leaving_index]);
    SWAP(පූර්ණ_සංඛ්‍යා,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

static anta_hstha void swap_columns(Mat_<dviguu_na>& A,පූර්ණ_සංඛ්‍යා col1,පූර්ණ_සංඛ්‍යා col2){
    සඳහා(පූර්ණ_සංඛ්‍යා i=0;i<A.rows;i++){
        dviguu_na tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
