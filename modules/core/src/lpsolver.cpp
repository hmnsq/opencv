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
#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <climits>
#अन्तर्भाव <algorithm>

#define dprintf(x)
#define print_matrix(x)

नामाकाश cv
{

using std::vector;

#ifdef ALEX_DEBUG
स्थिर शून्य print_simplex_state(ध्रुव Mat& c,ध्रुव Mat& b,द्विगुण v,ध्रुव std::vector<पूर्णाङ्क> N,ध्रुव std::vector<पूर्णाङ्क> B){
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
#अण्णहा
#define print_simplex_state(c,b,v,N,B)
#endif

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
स्थिर पूर्णाङ्क initialize_simplex(Mat_<द्विगुण>& c, Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,vector<unsigned पूर्णाङ्क>& indexToRow);
स्थिर अन्तःस्थ शून्य pivot(Mat_<द्विगुण>& c,Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,पूर्णाङ्क leaving_index,
        पूर्णाङ्क entering_index,vector<unsigned पूर्णाङ्क>& indexToRow);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
स्थिर पूर्णाङ्क inner_simplex(Mat_<द्विगुण>& c, Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,vector<unsigned पूर्णाङ्क>& indexToRow);
स्थिर शून्य swap_columns(Mat_<द्विगुण>& A,पूर्णाङ्क col1,पूर्णाङ्क col2);
#define SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
पूर्णाङ्क solveLP(निवेशः_Array Func_, निवेशः_Array Constr_, OutputArray z_, द्विगुण constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.rows()==1 && (Constr_.cols()-Func_.cols()==1))||
            (Func_.cols()==1 && (Constr_.cols()-Func_.rows()==1)));
    जइ (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<द्विगुण> bigC=Mat_<द्विगुण>(1,(Func.rows==1?Func.cols:Func.rows)+1),
        bigB=Mat_<द्विगुण>(Constr.rows,Constr.cols+1);
    जइ(Func.rows==1){
        Func.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }अण्णहा{
        Mat FuncT=Func.t();
        FuncT.convertTo(bigC.colRange(1,bigC.cols),CV_64FC1);
    }
    Constr.convertTo(bigB.colRange(1,bigB.cols),CV_64FC1);
    द्विगुण v=0;
    vector<पूर्णाङ्क> N,B;
    vector<unsigned पूर्णाङ्क> indexToRow;

    जइ(initialize_simplex(bigC,bigB,v,N,B,indexToRow)==SOLVELP_UNFEASIBLE){
        प्रत्यागम SOLVELP_UNFEASIBLE;
    }
    Mat_<द्विगुण> c=bigC.colRange(1,bigC.cols),
        b=bigB.colRange(1,bigB.cols);

    पूर्णाङ्क res=0;
    जइ((res=inner_simplex(c,b,v,N,B,indexToRow))==SOLVELP_UNBOUNDED){
        प्रत्यागम SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.cols,1,CV_64FC1);
    MatIterator_<द्विगुण> it=z.begin<द्विगुण>();
    unsigned पूर्णाङ्क nsize = (unsigned पूर्णाङ्क)N.size();
    प्रति(पूर्णाङ्क i=1;i<=c.cols;i++,it++){
        जइ(indexToRow[i]<nsize){
            *it=0;
        }अण्णहा{
            *it=b.at<द्विगुण>(indexToRow[i]-nsize,b.cols-1);
        }
    }

    z.copyTo(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.cols - 1, Constr.rows)) * z;
    Mat constr_check = Constr.col(Constr.cols - 1) - prod;
    द्विगुण min_मानम् = 0.0;
    minMaxIdx(constr_check, &min_मानम्);
    जइ (min_मानम् < -constr_eps)
    {
        प्रत्यागम SOLVELP_LOST;
    }

    प्रत्यागम res;
}

पूर्णाङ्क solveLP(निवेशः_Array Func, निवेशः_Array Constr, OutputArray z)
{
    प्रत्यागम solveLP(Func, Constr, z, 1e-12);
}

स्थिर पूर्णाङ्क initialize_simplex(Mat_<द्विगुण>& c, Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,vector<unsigned पूर्णाङ्क>& indexToRow){
    N.resize(c.cols);
    N[0]=0;
    प्रति (std::vector<पूर्णाङ्क>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.rows);
    B[0]=(पूर्णाङ्क)N.size();
    प्रति (std::vector<पूर्णाङ्क>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    indexToRow.resize(c.cols+b.rows);
    indexToRow[0]=0;
    प्रति (std::vector<unsigned पूर्णाङ्क>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    पूर्णाङ्क k=0;
    {
        द्विगुण min=DBL_MAX;
        प्रति(पूर्णाङ्क i=0;i<b.rows;i++){
            जइ(b(i,b.cols-1)<min){
                min=b(i,b.cols-1);
                k=i;
            }
        }
    }

    जइ(b(k,b.cols-1)>=0){
        N.erase(N.begin());
        प्रति (std::vector<unsigned पूर्णाङ्क>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
            --(*it);
        }
        प्रत्यागम 0;
    }

    Mat_<द्विगुण> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    प्रति(पूर्णाङ्क i=0;i<b.rows;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,indexToRow);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,indexToRow);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    unsigned पूर्णाङ्क nsize = (unsigned पूर्णाङ्क)N.size();
    जइ(indexToRow[0]>=nsize){
        पूर्णाङ्क iterator_offset=indexToRow[0]-nsize;
        जइ(b(iterator_offset,b.cols-1)>0){
            प्रत्यागम SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_offset,0,indexToRow);
    }

    vector<पूर्णाङ्क>::iterator iterator;
    {
        पूर्णाङ्क iterator_offset=indexToRow[0];
        iterator=N.begin()+iterator_offset;
        std::iter_swap(iterator,N.begin());
        SWAP(पूर्णाङ्क,indexToRow[*iterator],indexToRow[0]);
        swap_columns(c,iterator_offset,0);
        swap_columns(b,iterator_offset,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    प्रति(पूर्णाङ्क I=1;I<old_c.cols;I++){
        जइ(indexToRow[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            पूर्णाङ्क iterator_offset=indexToRow[I];
            c(0,iterator_offset)+=old_c(0,I);
            print_matrix(c);
        }अण्णहा{
            dprintf(("I=%d from basic\n",I));
            पूर्णाङ्क iterator_offset=indexToRow[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_offset).colRange(0,b.cols-1);
            v+=old_c(0,I)*b(iterator_offset,b.cols-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    प्रति (std::vector<unsigned पूर्णाङ्क>::iterator it = indexToRow.begin()+1 ; it != indexToRow.end(); ++it){
        --(*it);
    }
    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क inner_simplex(Mat_<द्विगुण>& c, Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,vector<unsigned पूर्णाङ्क>& indexToRow){

    प्रति(;;){
        MatIterator_<द्विगुण> pos_ptr;
        पूर्णाङ्क e=-1,pos_ctr=0,min_var=INT_MAX;
        तार्किक all_nonzero=सच्च;
        प्रति(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            जइ(*pos_ptr==0){
                all_nonzero=मिच्छ;
            }
            जइ(*pos_ptr>0){
                जइ(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        जइ(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            जइ(all_nonzero==सच्च){
                प्रत्यागम SOLVELP_SINGLE;
            }अण्णहा{
                प्रत्यागम SOLVELP_MULTI;
            }
        }

        पूर्णाङ्क l=-1;
        min_var=INT_MAX;
        द्विगुण min=DBL_MAX;
        पूर्णाङ्क row_it=0;
        MatIterator_<द्विगुण> min_row_ptr=b.begin();
        प्रति(MatIterator_<द्विगुण> it=b.begin();it!=b.end();it+=b.cols,row_it++){
            द्विगुण myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            जइ((myite=it[e])>0){
                द्विगुण val=it[b.cols-1]/myite;
                जइ(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        जइ(l==-1){
            प्रत्यागम SOLVELP_UNBOUNDED;
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

स्थिर अन्तःस्थ शून्य pivot(Mat_<द्विगुण>& c,Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,
        पूर्णाङ्क leaving_index,पूर्णाङ्क entering_index,vector<unsigned पूर्णाङ्क>& indexToRow){
    द्विगुण Coef=b(leaving_index,entering_index);
    प्रति(पूर्णाङ्क i=0;i<b.cols;i++){
        जइ(i==entering_index){
            b(leaving_index,i)=1/Coef;
        }अण्णहा{
            b(leaving_index,i)/=Coef;
        }
    }

    प्रति(पूर्णाङ्क i=0;i<b.rows;i++){
        जइ(i!=leaving_index){
            द्विगुण coef=b(i,entering_index);
            प्रति(पूर्णाङ्क j=0;j<b.cols;j++){
                जइ(j==entering_index){
                    b(i,j)=-coef*b(leaving_index,j);
                }अण्णहा{
                    b(i,j)-=(coef*b(leaving_index,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_index);
    प्रति(पूर्णाङ्क i=0;i<(b.cols-1);i++){
        जइ(i==entering_index){
            c(0,i)=-Coef*b(leaving_index,i);
        }अण्णहा{
            c(0,i)-=Coef*b(leaving_index,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_index,b.cols-1);

    SWAP(पूर्णाङ्क,N[entering_index],B[leaving_index]);
    SWAP(पूर्णाङ्क,indexToRow[N[entering_index]],indexToRow[B[leaving_index]]);
}

स्थिर अन्तःस्थ शून्य swap_columns(Mat_<द्विगुण>& A,पूर्णाङ्क col1,पूर्णाङ्क col2){
    प्रति(पूर्णाङ्क i=0;i<A.rows;i++){
        द्विगुण tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
