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
static void print_simplex_state(const Mat& c,const Mat& b,dviguu_na v,const std::vector<puur_naa_nka> N,const std::vector<puur_naa_nka> B){
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
#ᮜᮤᮚ
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
static puur_naa_nka initialize_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<puur_naa_nka>& N,vector<puur_naa_nka>& B,vector<unsigned puur_naa_nka>& indexToRow);
static anta_hstha void pivot(Mat_<dviguu_na>& c,Mat_<dviguu_na>& b,dviguu_na& v,vector<puur_naa_nka>& N,vector<puur_naa_nka>& B,puur_naa_nka leaving_index,
        puur_naa_nka entering_index,vector<unsigned puur_naa_nka>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
static puur_naa_nka inner_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<puur_naa_nka>& N,vector<puur_naa_nka>& B,vector<unsigned puur_naa_nka>& indexToRow);
static void swap_columns(Mat_<dviguu_na>& A,puur_naa_nka col1,puur_naa_nka col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
puur_naa_nka solveLP(InputArray Func_, InputArray Constr_, OutputArray z_, dviguu_na constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    ᮚᮦᮔ᮪ (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<dviguu_na> bigC=Mat_<dviguu_na>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<dviguu_na>(Constr.rows,Constr.cols+1);
    ᮚᮦᮔ᮪(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }ᮜᮤᮚ{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    dviguu_na v=0;
    vector<puur_naa_nka> N,B;
    vector<unsigned puur_naa_nka> indexToRow;

    ᮚᮦᮔ᮪(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        ᮝᮜᮤ SOLVELP_UNFEASIBLE;
    }
    Mat_<dviguu_na> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    puur_naa_nka res=0;
    ᮚᮦᮔ᮪((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        ᮝᮜᮤ SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<dviguu_na> it=z.begin<dviguu_na>();
    unsigned puur_naa_nka nsize = (unsigned puur_naa_nka)N.size();
    for(puur_naa_nka i=1;i<=c.cols;i++,it++){
        ᮚᮦᮔ᮪(indexToRow[i]<nsize){
            *it=0;
        }ᮜᮤᮚ{
            *it=b.at<dviguu_na>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    dviguu_na min_value = 0.0;
    minMaxIdx(constr_check, &min_value);
    ᮚᮦᮔ᮪ (min_value < -constr_eps)
    {
        ᮝᮜᮤ SOLVELP_LOST;
    }

    ᮝᮜᮤ res;
}

puur_naa_nka solveLP(InputArray Func, InputArray Constr, OutputArray z)
{
    ᮝᮜᮤ solveLP(Func, Constr, z, 1e-12);
}

static puur_naa_nka initialize_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<puur_naa_nka>& N,vector<puur_naa_nka>& B,vector<unsigned puur_naa_nka>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    for (std::vector<puur_naa_nka>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(puur_naa_nka)N.size();
    for (std::vector<puur_naa_nka>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    for (std::vector<unsigned puur_naa_nka>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    puur_naa_nka k=0;
    {
        dviguu_na min=DBL_MAX;
        for(puur_naa_nka i=0;i<b.rows;i++){
            ᮚᮦᮔ᮪(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    ᮚᮦᮔ᮪(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        for (std::vector<unsigned puur_naa_nka>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        ᮝᮜᮤ 0;
    }

    Mat_<dviguu_na> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    for(puur_naa_nka i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned puur_naa_nka nsize = (unsigned puur_naa_nka)N.size();
    ᮚᮦᮔ᮪(indexToRow[0]>=nsize){
        puur_naa_nka iterator_offset=indexToRow[0]-nsize;
        ᮚᮦᮔ᮪(b(iterator_offset,b.cols-1)>0){
            ᮝᮜᮤ SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<puur_naa_nka>::iterator iterator;
    {
        puur_naa_nka iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(puur_naa_nka,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    for(puur_naa_nka I=1;I<old_c.cols;I++){
        ᮚᮦᮔ᮪(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            puur_naa_nka iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }ᮜᮤᮚ{
            dprintf(("I=%d from basic\n",I));
            puur_naa_nka iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    for (std::vector<unsigned puur_naa_nka>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    ᮝᮜᮤ 0;
}

static puur_naa_nka inner_simplex(Mat_<dviguu_na>& c, Mat_<dviguu_na>& b,dviguu_na& v,vector<puur_naa_nka>& N,vector<puur_naa_nka>& B,vector<unsigned puur_naa_nka>& indexToRow){

    for(;;){
        MatIterator_<dviguu_na> pos_ptr;
        puur_naa_nka e=-1,pos_ctr=0,min_var=INT_MAX;
        bool all_nonzero=true;
        for(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            ᮚᮦᮔ᮪(*pos_ptr==0){
                all_nonzero=false;
            }
            ᮚᮦᮔ᮪(*pos_ptr>0){
                ᮚᮦᮔ᮪(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        ᮚᮦᮔ᮪(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            ᮚᮦᮔ᮪(all_nonzero==true){
                ᮝᮜᮤ SOLVELP_SINGLE;
            }ᮜᮤᮚ{
                ᮝᮜᮤ SOLVELP_MULTI;
            }
        }

        puur_naa_nka l=-1;
        min_var=INT_MAX;
        dviguu_na min=DBL_MAX;
        puur_naa_nka row_it=0;
        MatIterator_<dviguu_na> min_row_ptr=b.begin();
        for(MatIterator_<dviguu_na> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            dviguu_na myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            ᮚᮦᮔ᮪((myite=it[e])>0){
                dviguu_na val=it[b.cols-1]/myite;
                ᮚᮦᮔ᮪(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        ᮚᮦᮔ᮪(l==-1){
            ᮝᮜᮤ SOLVELP_UNBOUNDED;
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

static anta_hstha void pivot(Mat_<dviguu_na>& c,Mat_<dviguu_na>& b,dviguu_na& v,vector<puur_naa_nka>& N,vector<puur_naa_nka>& B,
        puur_naa_nka leaving_index,puur_naa_nka entering_index,vector<unsigned puur_naa_nka>& indexToRow){
    dviguu_na Coef=b(leaving_index,entering_index);
    for(puur_naa_nka i=0;i<b.cols;i++){
        ᮚᮦᮔ᮪(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }ᮜᮤᮚ{
            b(leaving_index,i)/=Coef;
        }
    }

    for(puur_naa_nka i=0;i<b.rows;i++){
        ᮚᮦᮔ᮪(i!=leaving_index){
            dviguu_na coef=b(i,entering_index);
            for(puur_naa_nka j=0;j<b.cols;j++){
                ᮚᮦᮔ᮪(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }ᮜᮤᮚ{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    for(puur_naa_nka i=0;i<(b.cols-1);i++){
        ᮚᮦᮔ᮪(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }ᮜᮤᮚ{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(puur_naa_nka,N[entering_index],B[leaving_index]);
    SWAP(puur_naa_nka,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

static anta_hstha void swap_columns(Mat_<dviguu_na>& A,puur_naa_nka col1,puur_naa_nka col2){
    for(puur_naa_nka i=0;i<A.rows;i++){
        dviguu_na tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
