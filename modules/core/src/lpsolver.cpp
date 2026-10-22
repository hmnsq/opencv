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
#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <climits>
#ಅನ್ತರ್ಭಾವ <algorithm>

#define dprintf(x)
#define print_matrix(x)

ನಾಮಾಕಾಶ cv
{

using std::vector;

#ifdef ALEX_DEBUG
ಸ್ಥಿರ ಶೂನ್ಯ print_simplex_state(ಧ್ರುವ Mat& c,ಧ್ರುವ Mat& b,ದ್ವಿಗುಣ v,ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ> N,ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ> B){
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
#ಅನ್ಯಥಾ
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ initialize_simplex(Mat_<ದ್ವಿಗುಣ>& c, Mat_<ದ್ವಿಗುಣ>& b,ದ್ವಿಗುಣ& v,vector<ಪೂರ್ಣಾಙ್ಕ>& N,vector<ಪೂರ್ಣಾಙ್ಕ>& B,vector<unsigned ಪೂರ್ಣಾಙ್ಕ>& indexToRow);
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ pivot(Mat_<ದ್ವಿಗುಣ>& c,Mat_<ದ್ವಿಗುಣ>& b,ದ್ವಿಗುಣ& v,vector<ಪೂರ್ಣಾಙ್ಕ>& N,vector<ಪೂರ್ಣಾಙ್ಕ>& B,ಪೂರ್ಣಾಙ್ಕ leaving_index,
        ಪೂರ್ಣಾಙ್ಕ entering_index,vector<unsigned ಪೂರ್ಣಾಙ್ಕ>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ inner_simplex(Mat_<ದ್ವಿಗುಣ>& c, Mat_<ದ್ವಿಗುಣ>& b,ದ್ವಿಗುಣ& v,vector<ಪೂರ್ಣಾಙ್ಕ>& N,vector<ಪೂರ್ಣಾಙ್ಕ>& B,vector<unsigned ಪೂರ್ಣಾಙ್ಕ>& indexToRow);
ಸ್ಥಿರ ಶೂನ್ಯ swap_columns(Mat_<ದ್ವಿಗುಣ>& A,ಪೂರ್ಣಾಙ್ಕ col1,ಪೂರ್ಣಾಙ್ಕ col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
ಪೂರ್ಣಾಙ್ಕ solveLP(ಒಳಹರಿವು_Array Func_, ಒಳಹರಿವು_Array Constr_, OutputArray z_, ದ್ವಿಗುಣ constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    ಯದಿ (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<ದ್ವಿಗುಣ> bigC=Mat_<ದ್ವಿಗುಣ>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<ದ್ವಿಗುಣ>(Constr.rows,Constr.cols+1);
    ಯದಿ(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }ಅನ್ಯಥಾ{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    ದ್ವಿಗುಣ v=0;
    vector<ಪೂರ್ಣಾಙ್ಕ> N,B;
    vector<unsigned ಪೂರ್ಣಾಙ್ಕ> indexToRow;

    ಯದಿ(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        ಪ್ರತ್ಯಾಗಮ SOLVELP_UNFEASIBLE;
    }
    Mat_<ದ್ವಿಗುಣ> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    ಪೂರ್ಣಾಙ್ಕ res=0;
    ಯದಿ((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        ಪ್ರತ್ಯಾಗಮ SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<ದ್ವಿಗುಣ> it=z.begin<ದ್ವಿಗುಣ>();
    unsigned ಪೂರ್ಣಾಙ್ಕ nsize = (unsigned ಪೂರ್ಣಾಙ್ಕ)N.size();
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=1;i<=c.cols;i++,it++){
        ಯದಿ(indexToRow[i]<nsize){
            *it=0;
        }ಅನ್ಯಥಾ{
            *it=b.at<ದ್ವಿಗುಣ>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    ದ್ವಿಗುಣ min_ಮೌಲ್ಯ = 0.0;
    minMaxIdx(constr_check, &min_ಮೌಲ್ಯ);
    ಯದಿ (min_ಮೌಲ್ಯ < -constr_eps)
    {
        ಪ್ರತ್ಯಾಗಮ SOLVELP_LOST;
    }

    ಪ್ರತ್ಯಾಗಮ res;
}

ಪೂರ್ಣಾಙ್ಕ solveLP(ಒಳಹರಿವು_Array Func, ಒಳಹರಿವು_Array Constr, OutputArray z)
{
    ಪ್ರತ್ಯಾಗಮ solveLP(Func, Constr, z, 1e-12);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ initialize_simplex(Mat_<ದ್ವಿಗುಣ>& c, Mat_<ದ್ವಿಗುಣ>& b,ದ್ವಿಗುಣ& v,vector<ಪೂರ್ಣಾಙ್ಕ>& N,vector<ಪೂರ್ಣಾಙ್ಕ>& B,vector<unsigned ಪೂರ್ಣಾಙ್ಕ>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    ಪ್ರತಿ (std::vector<ಪೂರ್ಣಾಙ್ಕ>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(ಪೂರ್ಣಾಙ್ಕ)N.size();
    ಪ್ರತಿ (std::vector<ಪೂರ್ಣಾಙ್ಕ>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    ಪ್ರತಿ (std::vector<unsigned ಪೂರ್ಣಾಙ್ಕ>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    ಪೂರ್ಣಾಙ್ಕ k=0;
    {
        ದ್ವಿಗುಣ min=DBL_MAX;
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=0;i<b.rows;i++){
            ಯದಿ(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    ಯದಿ(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        ಪ್ರತಿ (std::vector<unsigned ಪೂರ್ಣಾಙ್ಕ>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    Mat_<ದ್ವಿಗುಣ> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned ಪೂರ್ಣಾಙ್ಕ nsize = (unsigned ಪೂರ್ಣಾಙ್ಕ)N.size();
    ಯದಿ(indexToRow[0]>=nsize){
        ಪೂರ್ಣಾಙ್ಕ iterator_offset=indexToRow[0]-nsize;
        ಯದಿ(b(iterator_offset,b.cols-1)>0){
            ಪ್ರತ್ಯಾಗಮ SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<ಪೂರ್ಣಾಙ್ಕ>::iterator iterator;
    {
        ಪೂರ್ಣಾಙ್ಕ iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(ಪೂರ್ಣಾಙ್ಕ,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ I=1;I<old_c.cols;I++){
        ಯದಿ(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            ಪೂರ್ಣಾಙ್ಕ iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }ಅನ್ಯಥಾ{
            dprintf(("I=%d from basic\n",I));
            ಪೂರ್ಣಾಙ್ಕ iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    ಪ್ರತಿ (std::vector<unsigned ಪೂರ್ಣಾಙ್ಕ>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ inner_simplex(Mat_<ದ್ವಿಗುಣ>& c, Mat_<ದ್ವಿಗುಣ>& b,ದ್ವಿಗುಣ& v,vector<ಪೂರ್ಣಾಙ್ಕ>& N,vector<ಪೂರ್ಣಾಙ್ಕ>& B,vector<unsigned ಪೂರ್ಣಾಙ್ಕ>& indexToRow){

    ಪ್ರತಿ(;;){
        MatIterator_<ದ್ವಿಗುಣ> pos_ptr;
        ಪೂರ್ಣಾಙ್ಕ e=-1,pos_ctr=0,min_var=INT_MAX;
        ತಾರ್ಕಿಕ all_nonzero=ಸತ್ಯ;
        ಪ್ರತಿ(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            ಯದಿ(*pos_ptr==0){
                all_nonzero=ಅಸತ್ಯ;
            }
            ಯದಿ(*pos_ptr>0){
                ಯದಿ(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        ಯದಿ(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            ಯದಿ(all_nonzero==ಸತ್ಯ){
                ಪ್ರತ್ಯಾಗಮ SOLVELP_SINGLE;
            }ಅನ್ಯಥಾ{
                ಪ್ರತ್ಯಾಗಮ SOLVELP_MULTI;
            }
        }

        ಪೂರ್ಣಾಙ್ಕ l=-1;
        min_var=INT_MAX;
        ದ್ವಿಗುಣ min=DBL_MAX;
        ಪೂರ್ಣಾಙ್ಕ row_it=0;
        MatIterator_<ದ್ವಿಗುಣ> min_row_ptr=b.begin();
        ಪ್ರತಿ(MatIterator_<ದ್ವಿಗುಣ> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            ದ್ವಿಗುಣ myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            ಯದಿ((myite=it[e])>0){
                ದ್ವಿಗುಣ val=it[b.cols-1]/myite;
                ಯದಿ(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        ಯದಿ(l==-1){
            ಪ್ರತ್ಯಾಗಮ SOLVELP_UNBOUNDED;
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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ pivot(Mat_<ದ್ವಿಗುಣ>& c,Mat_<ದ್ವಿಗುಣ>& b,ದ್ವಿಗುಣ& v,vector<ಪೂರ್ಣಾಙ್ಕ>& N,vector<ಪೂರ್ಣಾಙ್ಕ>& B,
        ಪೂರ್ಣಾಙ್ಕ leaving_index,ಪೂರ್ಣಾಙ್ಕ entering_index,vector<unsigned ಪೂರ್ಣಾಙ್ಕ>& indexToRow){
    ದ್ವಿಗುಣ Coef=b(leaving_index,entering_index);
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=0;i<b.cols;i++){
        ಯದಿ(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }ಅನ್ಯಥಾ{
            b(leaving_index,i)/=Coef;
        }
    }

    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=0;i<b.rows;i++){
        ಯದಿ(i!=leaving_index){
            ದ್ವಿಗುಣ coef=b(i,entering_index);
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j=0;j<b.cols;j++){
                ಯದಿ(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }ಅನ್ಯಥಾ{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=0;i<(b.cols-1);i++){
        ಯದಿ(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }ಅನ್ಯಥಾ{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(ಪೂರ್ಣಾಙ್ಕ,N[entering_index],B[leaving_index]);
    SWAP(ಪೂರ್ಣಾಙ್ಕ,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ swap_columns(Mat_<ದ್ವಿಗುಣ>& A,ಪೂರ್ಣಾಙ್ಕ col1,ಪೂರ್ಣಾಙ್ಕ col2){
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i=0;i<A.rows;i++){
        ದ್ವಿಗುಣ tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
