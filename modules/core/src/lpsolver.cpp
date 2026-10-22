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
#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <climits>
#അന്തര്ഭാവ <algorithm>

#define dprintf(x)
#define print_matrix(x)

നാമാകാശ cv
{

using std::vector;

#ifdef ALEX_DEBUG
സ്ഥിതം ശൂന്യം print_simplex_state(സ്ഥിരം Mat& c,സ്ഥിരം Mat& b,ഇരട്ട v,സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ> N,സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ> B){
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
#അല്ലെങ്കിൽ
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ initialize_simplex(Mat_<ഇരട്ട>& c, Mat_<ഇരട്ട>& b,ഇരട്ട& v,vector<പൂർണ്ണസംഖ്യ>& N,vector<പൂർണ്ണസംഖ്യ>& B,vector<unsigned പൂർണ്ണസംഖ്യ>& indexToRow);
സ്ഥിതം വരിയിൽ ശൂന്യം pivot(Mat_<ഇരട്ട>& c,Mat_<ഇരട്ട>& b,ഇരട്ട& v,vector<പൂർണ്ണസംഖ്യ>& N,vector<പൂർണ്ണസംഖ്യ>& B,പൂർണ്ണസംഖ്യ leaving_index,
        പൂർണ്ണസംഖ്യ entering_index,vector<unsigned പൂർണ്ണസംഖ്യ>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ inner_simplex(Mat_<ഇരട്ട>& c, Mat_<ഇരട്ട>& b,ഇരട്ട& v,vector<പൂർണ്ണസംഖ്യ>& N,vector<പൂർണ്ണസംഖ്യ>& B,vector<unsigned പൂർണ്ണസംഖ്യ>& indexToRow);
സ്ഥിതം ശൂന്യം swap_columns(Mat_<ഇരട്ട>& A,പൂർണ്ണസംഖ്യ col1,പൂർണ്ണസംഖ്യ col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
പൂർണ്ണസംഖ്യ solveLP(നിവേശം_Array Func_, നിവേശം_Array Constr_, OutputArray z_, ഇരട്ട constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    എങ്കിൽ (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<ഇരട്ട> bigC=Mat_<ഇരട്ട>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<ഇരട്ട>(Constr.rows,Constr.cols+1);
    എങ്കിൽ(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }അല്ലെങ്കിൽ{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    ഇരട്ട v=0;
    vector<പൂർണ്ണസംഖ്യ> N,B;
    vector<unsigned പൂർണ്ണസംഖ്യ> indexToRow;

    എങ്കിൽ(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        തിരികെ SOLVELP_UNFEASIBLE;
    }
    Mat_<ഇരട്ട> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    പൂർണ്ണസംഖ്യ res=0;
    എങ്കിൽ((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        തിരികെ SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<ഇരട്ട> it=z.begin<ഇരട്ട>();
    unsigned പൂർണ്ണസംഖ്യ nsize = (unsigned പൂർണ്ണസംഖ്യ)N.size();
    വേണ്ടി(പൂർണ്ണസംഖ്യ i=1;i<=c.cols;i++,it++){
        എങ്കിൽ(indexToRow[i]<nsize){
            *it=0;
        }അല്ലെങ്കിൽ{
            *it=b.at<ഇരട്ട>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    ഇരട്ട min_മൂല്യം = 0.0;
    minMaxIdx(constr_check, &min_മൂല്യം);
    എങ്കിൽ (min_മൂല്യം < -constr_eps)
    {
        തിരികെ SOLVELP_LOST;
    }

    തിരികെ res;
}

പൂർണ്ണസംഖ്യ solveLP(നിവേശം_Array Func, നിവേശം_Array Constr, OutputArray z)
{
    തിരികെ solveLP(Func, Constr, z, 1e-12);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ initialize_simplex(Mat_<ഇരട്ട>& c, Mat_<ഇരട്ട>& b,ഇരട്ട& v,vector<പൂർണ്ണസംഖ്യ>& N,vector<പൂർണ്ണസംഖ്യ>& B,vector<unsigned പൂർണ്ണസംഖ്യ>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    വേണ്ടി (std::vector<പൂർണ്ണസംഖ്യ>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(പൂർണ്ണസംഖ്യ)N.size();
    വേണ്ടി (std::vector<പൂർണ്ണസംഖ്യ>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    വേണ്ടി (std::vector<unsigned പൂർണ്ണസംഖ്യ>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    പൂർണ്ണസംഖ്യ k=0;
    {
        ഇരട്ട min=DBL_MAX;
        വേണ്ടി(പൂർണ്ണസംഖ്യ i=0;i<b.rows;i++){
            എങ്കിൽ(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    എങ്കിൽ(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        വേണ്ടി (std::vector<unsigned പൂർണ്ണസംഖ്യ>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        തിരികെ 0;
    }

    Mat_<ഇരട്ട> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    വേണ്ടി(പൂർണ്ണസംഖ്യ i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned പൂർണ്ണസംഖ്യ nsize = (unsigned പൂർണ്ണസംഖ്യ)N.size();
    എങ്കിൽ(indexToRow[0]>=nsize){
        പൂർണ്ണസംഖ്യ iterator_offset=indexToRow[0]-nsize;
        എങ്കിൽ(b(iterator_offset,b.cols-1)>0){
            തിരികെ SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<പൂർണ്ണസംഖ്യ>::iterator iterator;
    {
        പൂർണ്ണസംഖ്യ iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(പൂർണ്ണസംഖ്യ,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    വേണ്ടി(പൂർണ്ണസംഖ്യ I=1;I<old_c.cols;I++){
        എങ്കിൽ(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            പൂർണ്ണസംഖ്യ iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }അല്ലെങ്കിൽ{
            dprintf(("I=%d from basic\n",I));
            പൂർണ്ണസംഖ്യ iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    വേണ്ടി (std::vector<unsigned പൂർണ്ണസംഖ്യ>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ inner_simplex(Mat_<ഇരട്ട>& c, Mat_<ഇരട്ട>& b,ഇരട്ട& v,vector<പൂർണ്ണസംഖ്യ>& N,vector<പൂർണ്ണസംഖ്യ>& B,vector<unsigned പൂർണ്ണസംഖ്യ>& indexToRow){

    വേണ്ടി(;;){
        MatIterator_<ഇരട്ട> pos_ptr;
        പൂർണ്ണസംഖ്യ e=-1,pos_ctr=0,min_var=INT_MAX;
        തർക്കം all_nonzero=സത്യം;
        വേണ്ടി(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            എങ്കിൽ(*pos_ptr==0){
                all_nonzero=അസത്യം;
            }
            എങ്കിൽ(*pos_ptr>0){
                എങ്കിൽ(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        എങ്കിൽ(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            എങ്കിൽ(all_nonzero==സത്യം){
                തിരികെ SOLVELP_SINGLE;
            }അല്ലെങ്കിൽ{
                തിരികെ SOLVELP_MULTI;
            }
        }

        പൂർണ്ണസംഖ്യ l=-1;
        min_var=INT_MAX;
        ഇരട്ട min=DBL_MAX;
        പൂർണ്ണസംഖ്യ row_it=0;
        MatIterator_<ഇരട്ട> min_row_ptr=b.begin();
        വേണ്ടി(MatIterator_<ഇരട്ട> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            ഇരട്ട myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            എങ്കിൽ((myite=it[e])>0){
                ഇരട്ട val=it[b.cols-1]/myite;
                എങ്കിൽ(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        എങ്കിൽ(l==-1){
            തിരികെ SOLVELP_UNBOUNDED;
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

സ്ഥിതം വരിയിൽ ശൂന്യം pivot(Mat_<ഇരട്ട>& c,Mat_<ഇരട്ട>& b,ഇരട്ട& v,vector<പൂർണ്ണസംഖ്യ>& N,vector<പൂർണ്ണസംഖ്യ>& B,
        പൂർണ്ണസംഖ്യ leaving_index,പൂർണ്ണസംഖ്യ entering_index,vector<unsigned പൂർണ്ണസംഖ്യ>& indexToRow){
    ഇരട്ട Coef=b(leaving_index,entering_index);
    വേണ്ടി(പൂർണ്ണസംഖ്യ i=0;i<b.cols;i++){
        എങ്കിൽ(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }അല്ലെങ്കിൽ{
            b(leaving_index,i)/=Coef;
        }
    }

    വേണ്ടി(പൂർണ്ണസംഖ്യ i=0;i<b.rows;i++){
        എങ്കിൽ(i!=leaving_index){
            ഇരട്ട coef=b(i,entering_index);
            വേണ്ടി(പൂർണ്ണസംഖ്യ j=0;j<b.cols;j++){
                എങ്കിൽ(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }അല്ലെങ്കിൽ{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    വേണ്ടി(പൂർണ്ണസംഖ്യ i=0;i<(b.cols-1);i++){
        എങ്കിൽ(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }അല്ലെങ്കിൽ{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(പൂർണ്ണസംഖ്യ,N[entering_index],B[leaving_index]);
    SWAP(പൂർണ്ണസംഖ്യ,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

സ്ഥിതം വരിയിൽ ശൂന്യം swap_columns(Mat_<ഇരട്ട>& A,പൂർണ്ണസംഖ്യ col1,പൂർണ്ണസംഖ്യ col2){
    വേണ്ടി(പൂർണ്ണസംഖ്യ i=0;i<A.rows;i++){
        ഇരട്ട tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
