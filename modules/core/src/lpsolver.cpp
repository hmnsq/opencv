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
#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <climits>
#ଅନ୍ତର୍ଭାଵ <algorithm>

#define dprintf(x)
#define print_matrix(x)

ନାମାକାଶ cv
{

using std::vector;

#ifdef ALEX_DEBUG
ସ୍ଥିର ଶୂନ୍ୟ print_simplex_state(ଅଚଳ Mat& c,ଅଚଳ Mat& b,ଦ୍ୱିଗୁଣ v,ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> N,ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> B){
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
#ଅନ୍ୟଥା
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା initialize_simplex(Mat_<ଦ୍ୱିଗୁଣ>& c, Mat_<ଦ୍ୱିଗୁଣ>& b,ଦ୍ୱିଗୁଣ& v,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& N,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& B,vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& indexToRow);
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ pivot(Mat_<ଦ୍ୱିଗୁଣ>& c,Mat_<ଦ୍ୱିଗୁଣ>& b,ଦ୍ୱିଗୁଣ& v,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& N,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& B,ପୂର୍ଣ୍ଣସଂଖ୍ୟା leaving_index,
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା entering_index,vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner_simplex(Mat_<ଦ୍ୱିଗୁଣ>& c, Mat_<ଦ୍ୱିଗୁଣ>& b,ଦ୍ୱିଗୁଣ& v,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& N,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& B,vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& indexToRow);
ସ୍ଥିର ଶୂନ୍ୟ swap_columns(Mat_<ଦ୍ୱିଗୁଣ>& A,ପୂର୍ଣ୍ଣସଂଖ୍ୟା col1,ପୂର୍ଣ୍ଣସଂଖ୍ୟା col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
ପୂର୍ଣ୍ଣସଂଖ୍ୟା solveLP(ନିବେଶ_Array Func_, ନିବେଶ_Array Constr_, OutputArray z_, ଦ୍ୱିଗୁଣ constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    ଯଦି (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<ଦ୍ୱିଗୁଣ> bigC=Mat_<ଦ୍ୱିଗୁଣ>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<ଦ୍ୱିଗୁଣ>(Constr.rows,Constr.cols+1);
    ଯଦି(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }ଅନ୍ୟଥା{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    ଦ୍ୱିଗୁଣ v=0;
    vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> N,B;
    vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> indexToRow;

    ଯଦି(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        ଫେରନ୍ତୁ SOLVELP_UNFEASIBLE;
    }
    Mat_<ଦ୍ୱିଗୁଣ> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା res=0;
    ଯଦି((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        ଫେରନ୍ତୁ SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<ଦ୍ୱିଗୁଣ> it=z.begin<ଦ୍ୱିଗୁଣ>();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsize = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)N.size();
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=1;i<=c.cols;i++,it++){
        ଯଦି(indexToRow[i]<nsize){
            *it=0;
        }ଅନ୍ୟଥା{
            *it=b.at<ଦ୍ୱିଗୁଣ>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    ଦ୍ୱିଗୁଣ min_ମୂଲ୍ୟ = 0.0;
    minMaxIdx(constr_check, &min_ମୂଲ୍ୟ);
    ଯଦି (min_ମୂଲ୍ୟ < -constr_eps)
    {
        ଫେରନ୍ତୁ SOLVELP_LOST;
    }

    ଫେରନ୍ତୁ res;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା solveLP(ନିବେଶ_Array Func, ନିବେଶ_Array Constr, OutputArray z)
{
    ଫେରନ୍ତୁ solveLP(Func, Constr, z, 1e-12);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା initialize_simplex(Mat_<ଦ୍ୱିଗୁଣ>& c, Mat_<ଦ୍ୱିଗୁଣ>& b,ଦ୍ୱିଗୁଣ& v,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& N,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& B,vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    ପାଇଁ (std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)N.size();
    ପାଇଁ (std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    ପାଇଁ (std::vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k=0;
    {
        ଦ୍ୱିଗୁଣ min=DBL_MAX;
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0;i<b.rows;i++){
            ଯଦି(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    ଯଦି(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        ପାଇଁ (std::vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        ଫେରନ୍ତୁ 0;
    }

    Mat_<ଦ୍ୱିଗୁଣ> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsize = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)N.size();
    ଯଦି(indexToRow[0]>=nsize){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା iterator_offset=indexToRow[0]-nsize;
        ଯଦି(b(iterator_offset,b.cols-1)>0){
            ଫେରନ୍ତୁ SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::iterator iterator;
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(ପୂର୍ଣ୍ଣସଂଖ୍ୟା,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା I=1;I<old_c.cols;I++){
        ଯଦି(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }ଅନ୍ୟଥା{
            dprintf(("I=%d from basic\n",I));
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    ପାଇଁ (std::vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner_simplex(Mat_<ଦ୍ୱିଗୁଣ>& c, Mat_<ଦ୍ୱିଗୁଣ>& b,ଦ୍ୱିଗୁଣ& v,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& N,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& B,vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& indexToRow){

    ପାଇଁ(;;){
        MatIterator_<ଦ୍ୱିଗୁଣ> pos_ptr;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା e=-1,pos_ctr=0,min_var=INT_MAX;
        ତାର୍କିକ all_nonzero=ସତ୍ୟ;
        ପାଇଁ(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            ଯଦି(*pos_ptr==0){
                all_nonzero=ମିଥ୍ୟା;
            }
            ଯଦି(*pos_ptr>0){
                ଯଦି(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        ଯଦି(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            ଯଦି(all_nonzero==ସତ୍ୟ){
                ଫେରନ୍ତୁ SOLVELP_SINGLE;
            }ଅନ୍ୟଥା{
                ଫେରନ୍ତୁ SOLVELP_MULTI;
            }
        }

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା l=-1;
        min_var=INT_MAX;
        ଦ୍ୱିଗୁଣ min=DBL_MAX;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା row_it=0;
        MatIterator_<ଦ୍ୱିଗୁଣ> min_row_ptr=b.begin();
        ପାଇଁ(MatIterator_<ଦ୍ୱିଗୁଣ> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            ଦ୍ୱିଗୁଣ myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            ଯଦି((myite=it[e])>0){
                ଦ୍ୱିଗୁଣ val=it[b.cols-1]/myite;
                ଯଦି(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        ଯଦି(l==-1){
            ଫେରନ୍ତୁ SOLVELP_UNBOUNDED;
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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ pivot(Mat_<ଦ୍ୱିଗୁଣ>& c,Mat_<ଦ୍ୱିଗୁଣ>& b,ଦ୍ୱିଗୁଣ& v,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& N,vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& B,
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା leaving_index,ପୂର୍ଣ୍ଣସଂଖ୍ୟା entering_index,vector<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& indexToRow){
    ଦ୍ୱିଗୁଣ Coef=b(leaving_index,entering_index);
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0;i<b.cols;i++){
        ଯଦି(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }ଅନ୍ୟଥା{
            b(leaving_index,i)/=Coef;
        }
    }

    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0;i<b.rows;i++){
        ଯଦି(i!=leaving_index){
            ଦ୍ୱିଗୁଣ coef=b(i,entering_index);
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j=0;j<b.cols;j++){
                ଯଦି(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }ଅନ୍ୟଥା{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0;i<(b.cols-1);i++){
        ଯଦି(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }ଅନ୍ୟଥା{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(ପୂର୍ଣ୍ଣସଂଖ୍ୟା,N[entering_index],B[leaving_index]);
    SWAP(ପୂର୍ଣ୍ଣସଂଖ୍ୟା,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ swap_columns(Mat_<ଦ୍ୱିଗୁଣ>& A,ପୂର୍ଣ୍ଣସଂଖ୍ୟା col1,ପୂର୍ଣ୍ଣସଂଖ୍ୟା col2){
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=0;i<A.rows;i++){
        ଦ୍ୱିଗୁଣ tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
