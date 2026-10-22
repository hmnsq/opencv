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
#समावेश "precomp.hpp"
#समावेश <climits>
#समावेश <algorithm>

#घोषणा dprintf(x)
#घोषणा print_matrix(x)

namespace cv
{

using std::vector;

#अगर_घोषित ALEX_DEBUG
अटल व्योम print_simplex_state(स्थिर Mat& c,स्थिर Mat& b,द्विगुन v,स्थिर std::vector<पूर्णांक> N,स्थिर std::vector<पूर्णांक> B){
    म_लिखो("\tprint simplex state\n");

    म_लिखो("v=%g\n",v);

    म_लिखो("here c goes\n");
    print_matrix(c);

    म_लिखो("non-basic: ");
    print(Mat(N));
    म_लिखो("\n");

    म_लिखो("here b goes\n");
    print_matrix(b);
    म_लिखो("basic: ");

    print(Mat(B));
    म_लिखो("\n");
}
#अन्यथा
#घोषणा print_simplex_state(c,b,v,N,B)
#पूर्ण_अगर

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
अटल पूर्णांक initialize_simplex(Mat_<द्विगुन>& c, Mat_<द्विगुन>& b,द्विगुन& v,vector<पूर्णांक>& N,vector<पूर्णांक>& B,vector<अचिन्हित पूर्णांक>& सूचकांक_To_Row);
अटल अंतरभूत व्योम pivot(Mat_<द्विगुन>& c,Mat_<द्विगुन>& b,द्विगुन& v,vector<पूर्णांक>& N,vector<पूर्णांक>& B,पूर्णांक leaving_सूचकांक,
        पूर्णांक entering_सूचकांक,vector<अचिन्हित पूर्णांक>& सूचकांक_To_Row);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
अटल पूर्णांक inner_simplex(Mat_<द्विगुन>& c, Mat_<द्विगुन>& b,द्विगुन& v,vector<पूर्णांक>& N,vector<पूर्णांक>& B,vector<अचिन्हित पूर्णांक>& सूचकांक_To_Row);
अटल व्योम swap_columns(Mat_<द्विगुन>& A,पूर्णांक col1,पूर्णांक col2);
#घोषणा SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
पूर्णांक solveLP(निवेश_Array Func_, निवेश_Array Constr_, OutputArray z_, द्विगुन constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    अगर (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<द्विगुन> bigC=Mat_<द्विगुन>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<द्विगुन>(Constr.rows,Constr.cols+1);
    अगर(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }अन्यथा{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    द्विगुन v=0;
    vector<पूर्णांक> N,B;
    vector<अचिन्हित पूर्णांक> सूचकांक_To_Row;

    अगर(initialize_simplex(bigC,bigB,v,N,B,सूचकांक_To_Row)==SOLVELP_UNFEASIBLE){
        वापस SOLVELP_UNFEASIBLE;
    }
    Mat_<द्विगुन> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    पूर्णांक res=0;
    अगर((res=inner_simplex(c,b,v,N,B,सूचकांक_To_Row))==SOLVELP_UNBOUNDED){
        वापस SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<द्विगुन> it=z.begin<द्विगुन>();
    अचिन्हित पूर्णांक nsize = (अचिन्हित पूर्णांक)N.size();
    क्रम(पूर्णांक i=1;i<=c.cols;i++,it++){
        अगर(सूचकांक_To_Row[i]<nsize){
            *it=0;
        }अन्यथा{
            *it=b.at<द्विगुन>(सूचकांक_To_Row[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    द्विगुन min_मूल्य = 0.0;
    minMaxIdx(constr_check, &min_मूल्य);
    अगर (min_मूल्य < -constr_eps)
    {
        वापस SOLVELP_LOST;
    }

    वापस res;
}

पूर्णांक solveLP(निवेश_Array Func, निवेश_Array Constr, OutputArray z)
{
    वापस solveLP(Func, Constr, z, 1e-12);
}

अटल पूर्णांक initialize_simplex(Mat_<द्विगुन>& c, Mat_<द्विगुन>& b,द्विगुन& v,vector<पूर्णांक>& N,vector<पूर्णांक>& B,vector<अचिन्हित पूर्णांक>& सूचकांक_To_Row){
    N.resize(c.cols);
    N[0]=0;
    क्रम (std::vector<पूर्णांक>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(पूर्णांक)N.size();
    क्रम (std::vector<पूर्णांक>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    सूचकांक_To_Row.resize(c.cols+b.rows);
    सूचकांक_To_Row[0]=0;
    क्रम (std::vector<अचिन्हित पूर्णांक>::iterator it = सूचकांक_To_Row.begin()+1 ; it != सूचकांक_To_Row.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    पूर्णांक k=0;
    {
        द्विगुन min=द्विग_उच्च;
        क्रम(पूर्णांक i=0;i<b.rows;i++){
            अगर(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    अगर(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        क्रम (std::vector<अचिन्हित पूर्णांक>::iterator it = सूचकांक_To_Row.begin()+1 ; it != सूचकांक_To_Row.end(); ++it){
            --(*it);
        }
        वापस 0;
    }

    Mat_<द्विगुन> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    क्रम(पूर्णांक i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,सूचकांक_To_Row);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,सूचकांक_To_Row);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    अचिन्हित पूर्णांक nsize = (अचिन्हित पूर्णांक)N.size();
    अगर(सूचकांक_To_Row[0]>=nsize){
        पूर्णांक iterator_offset=सूचकांक_To_Row[0]-nsize;
        अगर(b(iterator_offset,b.cols-1)>0){
            वापस SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,सूचकांक_To_Row);
    }

    vector<पूर्णांक>::iterator iterator;
    {
        पूर्णांक iterator_offset=सूचकांक_To_Row[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(पूर्णांक,सूचकांक_To_Row[*iterator],सूचकांक_To_Row[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    क्रम(पूर्णांक I=1;I<old_c.cols;I++){
        अगर(सूचकांक_To_Row[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            पूर्णांक iterator_offset=सूचकांक_To_Row[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }अन्यथा{
            dprintf(("I=%d from basic\n",I));
            पूर्णांक iterator_offset=सूचकांक_To_Row[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    क्रम (std::vector<अचिन्हित पूर्णांक>::iterator it = सूचकांक_To_Row.begin()+1 ; it != सूचकांक_To_Row.end(); ++it){
        --(*it);
    }
    वापस 0;
}

अटल पूर्णांक inner_simplex(Mat_<द्विगुन>& c, Mat_<द्विगुन>& b,द्विगुन& v,vector<पूर्णांक>& N,vector<पूर्णांक>& B,vector<अचिन्हित पूर्णांक>& सूचकांक_To_Row){

    क्रम(;;){
        MatIterator_<द्विगुन> pos_ptr;
        पूर्णांक e=-1,pos_ctr=0,min_var=पूर्णांक_उच्च;
        bool all_nonzero=true;
        क्रम(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            अगर(*pos_ptr==0){
                all_nonzero=false;
            }
            अगर(*pos_ptr>0){
                अगर(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        अगर(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            अगर(all_nonzero==true){
                वापस SOLVELP_SINGLE;
            }अन्यथा{
                वापस SOLVELP_MULTI;
            }
        }

        पूर्णांक l=-1;
        min_var=पूर्णांक_उच्च;
        द्विगुन min=द्विग_उच्च;
        पूर्णांक row_it=0;
        MatIterator_<द्विगुन> min_row_ptr=b.begin();
        क्रम(MatIterator_<द्विगुन> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            द्विगुन myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            अगर((myite=it[e])>0){
                द्विगुन val=it[b.cols-1]/myite;
                अगर(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        अगर(l==-1){
            वापस SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        pivot(c,b,v,N,B,l,e,सूचकांक_To_Row);

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

अटल अंतरभूत व्योम pivot(Mat_<द्विगुन>& c,Mat_<द्विगुन>& b,द्विगुन& v,vector<पूर्णांक>& N,vector<पूर्णांक>& B,
        पूर्णांक leaving_सूचकांक,पूर्णांक entering_सूचकांक,vector<अचिन्हित पूर्णांक>& सूचकांक_To_Row){
    द्विगुन Coef=b(leaving_सूचकांक,entering_सूचकांक);
    क्रम(पूर्णांक i=0;i<b.cols;i++){
        अगर(i==entering_सूचकांक){
            b(leaving_सूचकांक,i)=1/Coef;
        }अन्यथा{
            b(leaving_सूचकांक,i)/=Coef;
        }
    }

    क्रम(पूर्णांक i=0;i<b.rows;i++){
        अगर(i!=leaving_सूचकांक){
            द्विगुन coef=b(i,entering_सूचकांक);
            क्रम(पूर्णांक j=0;j<b.cols;j++){
                अगर(j==entering_सूचकांक){
                    b(i,j)=-coef*b(leaving_सूचकांक,j);
                }अन्यथा{
                    b(i,j)-=(coef*b(leaving_सूचकांक,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_सूचकांक);
    क्रम(पूर्णांक i=0;i<(b.cols-1);i++){
        अगर(i==entering_सूचकांक){
            c(0,i)=-Coef*b(leaving_सूचकांक,i);
        }अन्यथा{
            c(0,i)-=Coef*b(leaving_सूचकांक,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_सूचकांक,b.cols-1);

    SWAP(पूर्णांक,N[entering_सूचकांक],B[leaving_सूचकांक]);
    SWAP(पूर्णांक,सूचकांक_To_Row[N[entering_सूचकांक]],सूचकांक_To_Row[B[leaving_सूचकांक]]);
}

अटल अंतरभूत व्योम swap_columns(Mat_<द्विगुन>& A,पूर्णांक col1,पूर्णांक col2){
    क्रम(पूर्णांक i=0;i<A.rows;i++){
        द्विगुन tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
