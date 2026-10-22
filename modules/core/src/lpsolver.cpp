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
static void print_simplex_state(const Mat& c,const Mat& b,dviguu_na v,const std::vector<จำนวนเต็ม> N,const std::vector<จำนวนเต็ม> B){
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
#มิฉะนั้น
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
static จำนวนเต็ม initialize_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<จำนวนเต็ม>& N,vector<จำนวนเต็ม>& B,vector<unsigned จำนวนเต็ม>& indexToRow);
static anta_hstha void pivot(Mat_<dviguu_na>& c,Mat_<dviguu_na>& b,dviguu_na& v,vector<จำนวนเต็ม>& N,vector<จำนวนเต็ม>& B,จำนวนเต็ม leaving_index,
        จำนวนเต็ม entering_index,vector<unsigned จำนวนเต็ม>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
static จำนวนเต็ม inner_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<จำนวนเต็ม>& N,vector<จำนวนเต็ม>& B,vector<unsigned จำนวนเต็ม>& indexToRow);
static void swap_columns(Mat_<dviguu_na>& A,จำนวนเต็ม col1,จำนวนเต็ม col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
จำนวนเต็ม solveLP(InputArray Func_, InputArray Constr_, OutputArray z_, dviguu_na constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    ถ้า (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<dviguu_na> bigC=Mat_<dviguu_na>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<dviguu_na>(Constr.rows,Constr.cols+1);
    ถ้า(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }มิฉะนั้น{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    dviguu_na v=0;
    vector<จำนวนเต็ม> N,B;
    vector<unsigned จำนวนเต็ม> indexToRow;

    ถ้า(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        คืน SOLVELP_UNFEASIBLE;
    }
    Mat_<dviguu_na> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    จำนวนเต็ม res=0;
    ถ้า((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        คืน SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<dviguu_na> it=z.begin<dviguu_na>();
    unsigned จำนวนเต็ม nsize = (unsigned จำนวนเต็ม)N.size();
    สำหรับ(จำนวนเต็ม i=1;i<=c.cols;i++,it++){
        ถ้า(indexToRow[i]<nsize){
            *it=0;
        }มิฉะนั้น{
            *it=b.at<dviguu_na>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    dviguu_na min_value = 0.0;
    minMaxIdx(constr_check, &min_value);
    ถ้า (min_value < -constr_eps)
    {
        คืน SOLVELP_LOST;
    }

    คืน res;
}

จำนวนเต็ม solveLP(InputArray Func, InputArray Constr, OutputArray z)
{
    คืน solveLP(Func, Constr, z, 1e-12);
}

static จำนวนเต็ม initialize_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<จำนวนเต็ม>& N,vector<จำนวนเต็ม>& B,vector<unsigned จำนวนเต็ม>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    สำหรับ (std::vector<จำนวนเต็ม>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(จำนวนเต็ม)N.size();
    สำหรับ (std::vector<จำนวนเต็ม>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    สำหรับ (std::vector<unsigned จำนวนเต็ม>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    จำนวนเต็ม k=0;
    {
        dviguu_na min=DBL_MAX;
        สำหรับ(จำนวนเต็ม i=0;i<b.rows;i++){
            ถ้า(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    ถ้า(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        สำหรับ (std::vector<unsigned จำนวนเต็ม>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        คืน 0;
    }

    Mat_<dviguu_na> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    สำหรับ(จำนวนเต็ม i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned จำนวนเต็ม nsize = (unsigned จำนวนเต็ม)N.size();
    ถ้า(indexToRow[0]>=nsize){
        จำนวนเต็ม iterator_offset=indexToRow[0]-nsize;
        ถ้า(b(iterator_offset,b.cols-1)>0){
            คืน SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<จำนวนเต็ม>::iterator iterator;
    {
        จำนวนเต็ม iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(จำนวนเต็ม,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    สำหรับ(จำนวนเต็ม I=1;I<old_c.cols;I++){
        ถ้า(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            จำนวนเต็ม iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }มิฉะนั้น{
            dprintf(("I=%d from basic\n",I));
            จำนวนเต็ม iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    สำหรับ (std::vector<unsigned จำนวนเต็ม>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    คืน 0;
}

static จำนวนเต็ม inner_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<จำนวนเต็ม>& N,vector<จำนวนเต็ม>& B,vector<unsigned จำนวนเต็ม>& indexToRow){

    สำหรับ(;;){
        MatIterator_<dviguu_na> pos_ptr;
        จำนวนเต็ม e=-1,pos_ctr=0,min_var=INT_MAX;
        bool all_nonzero=true;
        สำหรับ(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            ถ้า(*pos_ptr==0){
                all_nonzero=false;
            }
            ถ้า(*pos_ptr>0){
                ถ้า(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        ถ้า(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            ถ้า(all_nonzero==true){
                คืน SOLVELP_SINGLE;
            }มิฉะนั้น{
                คืน SOLVELP_MULTI;
            }
        }

        จำนวนเต็ม l=-1;
        min_var=INT_MAX;
        dviguu_na min=DBL_MAX;
        จำนวนเต็ม row_it=0;
        MatIterator_<dviguu_na> min_row_ptr=b.begin();
        สำหรับ(MatIterator_<dviguu_na> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            dviguu_na myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            ถ้า((myite=it[e])>0){
                dviguu_na val=it[b.cols-1]/myite;
                ถ้า(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        ถ้า(l==-1){
            คืน SOLVELP_UNBOUNDED;
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

static anta_hstha void pivot(Mat_<dviguu_na>& c,Mat_<dviguu_na>& b,dviguu_na& v,vector<จำนวนเต็ม>& N,vector<จำนวนเต็ม>& B,
        จำนวนเต็ม leaving_index,จำนวนเต็ม entering_index,vector<unsigned จำนวนเต็ม>& indexToRow){
    dviguu_na Coef=b(leaving_index,entering_index);
    สำหรับ(จำนวนเต็ม i=0;i<b.cols;i++){
        ถ้า(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }มิฉะนั้น{
            b(leaving_index,i)/=Coef;
        }
    }

    สำหรับ(จำนวนเต็ม i=0;i<b.rows;i++){
        ถ้า(i!=leaving_index){
            dviguu_na coef=b(i,entering_index);
            สำหรับ(จำนวนเต็ม j=0;j<b.cols;j++){
                ถ้า(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }มิฉะนั้น{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    สำหรับ(จำนวนเต็ม i=0;i<(b.cols-1);i++){
        ถ้า(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }มิฉะนั้น{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(จำนวนเต็ม,N[entering_index],B[leaving_index]);
    SWAP(จำนวนเต็ม,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

static anta_hstha void swap_columns(Mat_<dviguu_na>& A,จำนวนเต็ม col1,จำนวนเต็ม col2){
    สำหรับ(จำนวนเต็ม i=0;i<A.rows;i++){
        dviguu_na tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
