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
#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <climits>
#অন্তর্ভাব <algorithm>

#define dprintf(x)
#define print_matrix(x)

নামাকাশ cv
{

using std::vector;

#ifdef ALEX_DEBUG
স্থির শূন্য print_simplex_state(ধ্রুব Mat& c,ধ্রুব Mat& b,দ্বিগুণ v,ধ্রুব std::vector<পূর্ণাঙ্ক> N,ধ্রুব std::vector<পূর্ণাঙ্ক> B){
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
#অন্যথা
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
স্থির পূর্ণাঙ্ক initialize_simplex(Mat_<দ্বিগুণ>& c, Mat_<দ্বিগুণ>& b,দ্বিগুণ& v,vector<পূর্ণাঙ্ক>& N,vector<পূর্ণাঙ্ক>& B,vector<unsigned পূর্ণাঙ্ক>& indexToRow);
স্থির অন্তঃস্থ শূন্য pivot(Mat_<দ্বিগুণ>& c,Mat_<দ্বিগুণ>& b,দ্বিগুণ& v,vector<পূর্ণাঙ্ক>& N,vector<পূর্ণাঙ্ক>& B,পূর্ণাঙ্ক leaving_index,
        পূর্ণাঙ্ক entering_index,vector<unsigned পূর্ণাঙ্ক>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
স্থির পূর্ণাঙ্ক inner_simplex(Mat_<দ্বিগুণ>& c, Mat_<দ্বিগুণ>& b,দ্বিগুণ& v,vector<পূর্ণাঙ্ক>& N,vector<পূর্ণাঙ্ক>& B,vector<unsigned পূর্ণাঙ্ক>& indexToRow);
স্থির শূন্য swap_columns(Mat_<দ্বিগুণ>& A,পূর্ণাঙ্ক col1,পূর্ণাঙ্ক col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
পূর্ণাঙ্ক solveLP(প্রবেশ_Array Func_, প্রবেশ_Array Constr_, OutputArray z_, দ্বিগুণ constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    যদি (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<দ্বিগুণ> bigC=Mat_<দ্বিগুণ>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<দ্বিগুণ>(Constr.rows,Constr.cols+1);
    যদি(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }অন্যথা{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    দ্বিগুণ v=0;
    vector<পূর্ণাঙ্ক> N,B;
    vector<unsigned পূর্ণাঙ্ক> indexToRow;

    যদি(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        প্রত্যাগম SOLVELP_UNFEASIBLE;
    }
    Mat_<দ্বিগুণ> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    পূর্ণাঙ্ক res=0;
    যদি((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        প্রত্যাগম SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<দ্বিগুণ> it=z.begin<দ্বিগুণ>();
    unsigned পূর্ণাঙ্ক nsize = (unsigned পূর্ণাঙ্ক)N.size();
    প্রতি(পূর্ণাঙ্ক i=1;i<=c.cols;i++,it++){
        যদি(indexToRow[i]<nsize){
            *it=0;
        }অন্যথা{
            *it=b.at<দ্বিগুণ>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    দ্বিগুণ min_মান = 0.0;
    minMaxIdx(constr_check, &min_মান);
    যদি (min_মান < -constr_eps)
    {
        প্রত্যাগম SOLVELP_LOST;
    }

    প্রত্যাগম res;
}

পূর্ণাঙ্ক solveLP(প্রবেশ_Array Func, প্রবেশ_Array Constr, OutputArray z)
{
    প্রত্যাগম solveLP(Func, Constr, z, 1e-12);
}

স্থির পূর্ণাঙ্ক initialize_simplex(Mat_<দ্বিগুণ>& c, Mat_<দ্বিগুণ>& b,দ্বিগুণ& v,vector<পূর্ণাঙ্ক>& N,vector<পূর্ণাঙ্ক>& B,vector<unsigned পূর্ণাঙ্ক>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    প্রতি (std::vector<পূর্ণাঙ্ক>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(পূর্ণাঙ্ক)N.size();
    প্রতি (std::vector<পূর্ণাঙ্ক>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    প্রতি (std::vector<unsigned পূর্ণাঙ্ক>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    পূর্ণাঙ্ক k=0;
    {
        দ্বিগুণ min=DBL_MAX;
        প্রতি(পূর্ণাঙ্ক i=0;i<b.rows;i++){
            যদি(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    যদি(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        প্রতি (std::vector<unsigned পূর্ণাঙ্ক>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        প্রত্যাগম 0;
    }

    Mat_<দ্বিগুণ> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    প্রতি(পূর্ণাঙ্ক i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned পূর্ণাঙ্ক nsize = (unsigned পূর্ণাঙ্ক)N.size();
    যদি(indexToRow[0]>=nsize){
        পূর্ণাঙ্ক iterator_offset=indexToRow[0]-nsize;
        যদি(b(iterator_offset,b.cols-1)>0){
            প্রত্যাগম SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<পূর্ণাঙ্ক>::iterator iterator;
    {
        পূর্ণাঙ্ক iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(পূর্ণাঙ্ক,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    প্রতি(পূর্ণাঙ্ক I=1;I<old_c.cols;I++){
        যদি(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            পূর্ণাঙ্ক iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }অন্যথা{
            dprintf(("I=%d from basic\n",I));
            পূর্ণাঙ্ক iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    প্রতি (std::vector<unsigned পূর্ণাঙ্ক>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক inner_simplex(Mat_<দ্বিগুণ>& c, Mat_<দ্বিগুণ>& b,দ্বিগুণ& v,vector<পূর্ণাঙ্ক>& N,vector<পূর্ণাঙ্ক>& B,vector<unsigned পূর্ণাঙ্ক>& indexToRow){

    প্রতি(;;){
        MatIterator_<দ্বিগুণ> pos_ptr;
        পূর্ণাঙ্ক e=-1,pos_ctr=0,min_var=INT_MAX;
        তার্কিক all_nonzero=সত্য;
        প্রতি(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            যদি(*pos_ptr==0){
                all_nonzero=অসত্য;
            }
            যদি(*pos_ptr>0){
                যদি(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        যদি(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            যদি(all_nonzero==সত্য){
                প্রত্যাগম SOLVELP_SINGLE;
            }অন্যথা{
                প্রত্যাগম SOLVELP_MULTI;
            }
        }

        পূর্ণাঙ্ক l=-1;
        min_var=INT_MAX;
        দ্বিগুণ min=DBL_MAX;
        পূর্ণাঙ্ক row_it=0;
        MatIterator_<দ্বিগুণ> min_row_ptr=b.begin();
        প্রতি(MatIterator_<দ্বিগুণ> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            দ্বিগুণ myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            যদি((myite=it[e])>0){
                দ্বিগুণ val=it[b.cols-1]/myite;
                যদি(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        যদি(l==-1){
            প্রত্যাগম SOLVELP_UNBOUNDED;
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

স্থির অন্তঃস্থ শূন্য pivot(Mat_<দ্বিগুণ>& c,Mat_<দ্বিগুণ>& b,দ্বিগুণ& v,vector<পূর্ণাঙ্ক>& N,vector<পূর্ণাঙ্ক>& B,
        পূর্ণাঙ্ক leaving_index,পূর্ণাঙ্ক entering_index,vector<unsigned পূর্ণাঙ্ক>& indexToRow){
    দ্বিগুণ Coef=b(leaving_index,entering_index);
    প্রতি(পূর্ণাঙ্ক i=0;i<b.cols;i++){
        যদি(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }অন্যথা{
            b(leaving_index,i)/=Coef;
        }
    }

    প্রতি(পূর্ণাঙ্ক i=0;i<b.rows;i++){
        যদি(i!=leaving_index){
            দ্বিগুণ coef=b(i,entering_index);
            প্রতি(পূর্ণাঙ্ক j=0;j<b.cols;j++){
                যদি(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }অন্যথা{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    প্রতি(পূর্ণাঙ্ক i=0;i<(b.cols-1);i++){
        যদি(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }অন্যথা{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(পূর্ণাঙ্ক,N[entering_index],B[leaving_index]);
    SWAP(পূর্ণাঙ্ক,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

স্থির অন্তঃস্থ শূন্য swap_columns(Mat_<দ্বিগুণ>& A,পূর্ণাঙ্ক col1,পূর্ণাঙ্ক col2){
    প্রতি(পূর্ণাঙ্ক i=0;i<A.rows;i++){
        দ্বিগুণ tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
