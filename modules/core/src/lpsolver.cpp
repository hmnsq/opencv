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

#define dprintf(x)
#define print_matrix(x)

نطاق cv
{

using std::vector;

#ifdef ALEX_DEBUG
ساكن فراغ print_simplex_state(ثابت Mat& c,ثابت Mat& b,مزدوج v,ثابت std::vector<صحيح> N,ثابت std::vector<صحيح> B){
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
#אחרת
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ساكن صحيح initialize_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<unsigned صحيح>& indexToRow);
ساكن مضمن فراغ pivot(Mat_<مزدوج>& c,Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,صحيح leaving_index,
        صحيح entering_index,vector<unsigned صحيح>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ساكن صحيح inner_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<unsigned صحيح>& indexToRow);
ساكن فراغ swap_columns(Mat_<مزدوج>& A,صحيح col1,صحيح col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
صحيح solveLP(InputArray Func_, InputArray Constr_, OutputArray z_, مزدوج constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    אם (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<مزدوج> bigC=Mat_<مزدوج>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<مزدوج>(Constr.rows,Constr.cols+1);
    אם(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }אחרת{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    مزدوج v=0;
    vector<صحيح> N,B;
    vector<unsigned صحيح> indexToRow;

    אם(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        החזר SOLVELP_UNFEASIBLE;
    }
    Mat_<مزدوج> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    صحيح res=0;
    אם((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        החזר SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<مزدوج> it=z.begin<مزدوج>();
    unsigned صحيح nsize = (unsigned صحيح)N.size();
    עבור(صحيح i=1;i<=c.cols;i++,it++){
        אם(indexToRow[i]<nsize){
            *it=0;
        }אחרת{
            *it=b.at<مزدوج>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    مزدوج min_value = 0.0;
    minMaxIdx(constr_check, &min_value);
    אם (min_value < -constr_eps)
    {
        החזר SOLVELP_LOST;
    }

    החזר res;
}

صحيح solveLP(InputArray Func, InputArray Constr, OutputArray z)
{
    החזר solveLP(Func, Constr, z, 1e-12);
}

ساكن صحيح initialize_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<unsigned صحيح>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    עבור (std::vector<صحيح>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(صحيح)N.size();
    עבור (std::vector<صحيح>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    עבור (std::vector<unsigned صحيح>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    صحيح k=0;
    {
        مزدوج min=DBL_MAX;
        עבור(صحيح i=0;i<b.rows;i++){
            אם(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    אם(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        עבור (std::vector<unsigned صحيح>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        החזר 0;
    }

    Mat_<مزدوج> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    עבור(صحيح i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned صحيح nsize = (unsigned صحيح)N.size();
    אם(indexToRow[0]>=nsize){
        صحيح iterator_offset=indexToRow[0]-nsize;
        אם(b(iterator_offset,b.cols-1)>0){
            החזר SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<صحيح>::iterator iterator;
    {
        صحيح iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(صحيح,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    עבור(صحيح I=1;I<old_c.cols;I++){
        אם(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            صحيح iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }אחרת{
            dprintf(("I=%d from basic\n",I));
            صحيح iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    עבור (std::vector<unsigned صحيح>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    החזר 0;
}

ساكن صحيح inner_simplex(Mat_<مزدوج>& c, Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,vector<unsigned صحيح>& indexToRow){

    עבור(;;){
        MatIterator_<مزدوج> pos_ptr;
        صحيح e=-1,pos_ctr=0,min_var=INT_MAX;
        منطقي all_nonzero=אמת;
        עבור(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            אם(*pos_ptr==0){
                all_nonzero=שקר;
            }
            אם(*pos_ptr>0){
                אם(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        אם(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            אם(all_nonzero==אמת){
                החזר SOLVELP_SINGLE;
            }אחרת{
                החזר SOLVELP_MULTI;
            }
        }

        صحيح l=-1;
        min_var=INT_MAX;
        مزدوج min=DBL_MAX;
        صحيح row_it=0;
        MatIterator_<مزدوج> min_row_ptr=b.begin();
        עבור(MatIterator_<مزدوج> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            مزدوج myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            אם((myite=it[e])>0){
                مزدوج val=it[b.cols-1]/myite;
                אם(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        אם(l==-1){
            החזר SOLVELP_UNBOUNDED;
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

ساكن مضمن فراغ pivot(Mat_<مزدوج>& c,Mat_<مزدوج>& b,مزدوج& v,vector<صحيح>& N,vector<صحيح>& B,
        صحيح leaving_index,صحيح entering_index,vector<unsigned صحيح>& indexToRow){
    مزدوج Coef=b(leaving_index,entering_index);
    עבור(صحيح i=0;i<b.cols;i++){
        אם(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }אחרת{
            b(leaving_index,i)/=Coef;
        }
    }

    עבור(صحيح i=0;i<b.rows;i++){
        אם(i!=leaving_index){
            مزدوج coef=b(i,entering_index);
            עבור(صحيح j=0;j<b.cols;j++){
                אם(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }אחרת{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    עבור(صحيح i=0;i<(b.cols-1);i++){
        אם(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }אחרת{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(صحيح,N[entering_index],B[leaving_index]);
    SWAP(صحيح,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

ساكن مضمن فراغ swap_columns(Mat_<مزدوج>& A,صحيح col1,صحيح col2){
    עבור(صحيح i=0;i<A.rows;i++){
        مزدوج tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
