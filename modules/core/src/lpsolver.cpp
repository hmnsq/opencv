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
static void print_simplex_state(const Mat& c,const Mat& b,double v,const std::vector<цяло> N,const std::vector<цяло> B){
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
#иначе
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
static цяло initialize_simplex(Mat_<double>& c, Mat_<double>& b,double& v,vector<цяло>& N,vector<цяло>& B,vector<unsigned цяло>& indexToRow);
static inline void pivot(Mat_<double>& c,Mat_<double>& b,double& v,vector<цяло>& N,vector<цяло>& B,цяло leaving_index,
        цяло entering_index,vector<unsigned цяло>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
static цяло inner_simplex(Mat_<double>& c, Mat_<double>& b,double& v,vector<цяло>& N,vector<цяло>& B,vector<unsigned цяло>& indexToRow);
static void swap_columns(Mat_<double>& A,цяло col1,цяло col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
цяло solveLP(InputArray Func_, InputArray Constr_, OutputArray z_, double constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    ако (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<double> bigC=Mat_<double>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<double>(Constr.rows,Constr.cols+1);
    ако(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }иначе{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    double v=0;
    vector<цяло> N,B;
    vector<unsigned цяло> indexToRow;

    ако(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        върни SOLVELP_UNFEASIBLE;
    }
    Mat_<double> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    цяло res=0;
    ако((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        върни SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<double> it=z.begin<double>();
    unsigned цяло nsize = (unsigned цяло)N.size();
    за(цяло i=1;i<=c.cols;i++,it++){
        ако(indexToRow[i]<nsize){
            *it=0;
        }иначе{
            *it=b.at<double>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    double min_value = 0.0;
    minMaxIdx(constr_check, &min_value);
    ако (min_value < -constr_eps)
    {
        върни SOLVELP_LOST;
    }

    върни res;
}

цяло solveLP(InputArray Func, InputArray Constr, OutputArray z)
{
    върни solveLP(Func, Constr, z, 1e-12);
}

static цяло initialize_simplex(Mat_<double>& c, Mat_<double>& b,double& v,vector<цяло>& N,vector<цяло>& B,vector<unsigned цяло>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    за (std::vector<цяло>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(цяло)N.size();
    за (std::vector<цяло>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    за (std::vector<unsigned цяло>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    цяло k=0;
    {
        double min=DBL_MAX;
        за(цяло i=0;i<b.rows;i++){
            ако(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    ако(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        за (std::vector<unsigned цяло>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        върни 0;
    }

    Mat_<double> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    за(цяло i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned цяло nsize = (unsigned цяло)N.size();
    ако(indexToRow[0]>=nsize){
        цяло iterator_offset=indexToRow[0]-nsize;
        ако(b(iterator_offset,b.cols-1)>0){
            върни SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<цяло>::iterator iterator;
    {
        цяло iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(цяло,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    за(цяло I=1;I<old_c.cols;I++){
        ако(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            цяло iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }иначе{
            dprintf(("I=%d from basic\n",I));
            цяло iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    за (std::vector<unsigned цяло>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    върни 0;
}

static цяло inner_simplex(Mat_<double>& c, Mat_<double>& b,double& v,vector<цяло>& N,vector<цяло>& B,vector<unsigned цяло>& indexToRow){

    за(;;){
        MatIterator_<double> pos_ptr;
        цяло e=-1,pos_ctr=0,min_var=INT_MAX;
        bool all_nonzero=true;
        за(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            ако(*pos_ptr==0){
                all_nonzero=false;
            }
            ако(*pos_ptr>0){
                ако(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        ако(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            ако(all_nonzero==true){
                върни SOLVELP_SINGLE;
            }иначе{
                върни SOLVELP_MULTI;
            }
        }

        цяло l=-1;
        min_var=INT_MAX;
        double min=DBL_MAX;
        цяло row_it=0;
        MatIterator_<double> min_row_ptr=b.begin();
        за(MatIterator_<double> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            double myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            ако((myite=it[e])>0){
                double val=it[b.cols-1]/myite;
                ако(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        ако(l==-1){
            върни SOLVELP_UNBOUNDED;
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

static inline void pivot(Mat_<double>& c,Mat_<double>& b,double& v,vector<цяло>& N,vector<цяло>& B,
        цяло leaving_index,цяло entering_index,vector<unsigned цяло>& indexToRow){
    double Coef=b(leaving_index,entering_index);
    за(цяло i=0;i<b.cols;i++){
        ако(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }иначе{
            b(leaving_index,i)/=Coef;
        }
    }

    за(цяло i=0;i<b.rows;i++){
        ако(i!=leaving_index){
            double coef=b(i,entering_index);
            за(цяло j=0;j<b.cols;j++){
                ако(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }иначе{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    за(цяло i=0;i<(b.cols-1);i++){
        ако(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }иначе{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(цяло,N[entering_index],B[leaving_index]);
    SWAP(цяло,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

static inline void swap_columns(Mat_<double>& A,цяло col1,цяло col2){
    за(цяло i=0;i<A.rows;i++){
        double tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
