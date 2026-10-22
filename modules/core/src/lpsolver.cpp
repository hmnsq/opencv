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

#परिभाषय dprintf(x)
#परिभाषय print_matrix(x)

नामाकाश cv
{

उपयुज्य std::vector;

#यदि_परिभाषितम् ALEX_DEBUG
स्थिर शून्य print_simplex_state(ध्रुव Mat& c,ध्रुव Mat& b,द्विगुण v,ध्रुव std::vector<पूर्णाङ्क> N,ध्रुव std::vector<पूर्णाङ्क> B){
    प्रारूप_लिख("\tprint simplex state\n");

    प्रारूप_लिख("v=%g\n",v);

    प्रारूप_लिख("here c goes\n");
    print_matrix(c);

    प्रारूप_लिख("non-basic: ");
    print(Mat(N));
    प्रारूप_लिख("\n");

    प्रारूप_लिख("here b goes\n");
    print_matrix(b);
    प्रारूप_लिख("basic: ");

    print(Mat(B));
    प्रारूप_लिख("\n");
}
#अन्यथा
#परिभाषय print_simplex_state(c,b,v,N,B)
#यदि_अन्तः

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
स्थिर पूर्णाङ्क initialize_simplex(Mat_<द्विगुण>& c, Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,vector<अचिह्नित पूर्णाङ्क>& सूचकांक_गन्तव्यम्_Row);
स्थिर अन्तःस्थ शून्य pivot(Mat_<द्विगुण>& c,Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,पूर्णाङ्क leaving_सूचकांक,
        पूर्णाङ्क entering_सूचकांक,vector<अचिह्नित पूर्णाङ्क>& सूचकांक_गन्तव्यम्_Row);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
स्थिर पूर्णाङ्क inner_simplex(Mat_<द्विगुण>& c, Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,vector<अचिह्नित पूर्णाङ्क>& सूचकांक_गन्तव्यम्_Row);
स्थिर शून्य swap_columns(Mat_<द्विगुण>& A,पूर्णाङ्क col1,पूर्णाङ्क col2);
#परिभाषय SWAP(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
पूर्णाङ्क solveLP(निवेशः_Array Func_, निवेशः_Array Constr_, OutputArray z_, द्विगुण constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.पङ्क्तिकाः()==1 && (Constr_.स्तम्भसङ्ख्या()-Func_.स्तम्भसङ्ख्या()==1))||
            (Func_.स्तम्भसङ्ख्या()==1 && (Constr_.स्तम्भसङ्ख्या()-Func_.पङ्क्तिकाः()==1)));
    यदि (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<द्विगुण> bigC=Mat_<द्विगुण>(1,(Func.पङ्क्तिकाः==1?Func.स्तम्भसङ्ख्या:Func.पङ्क्तिकाः)+1),
        bigB=Mat_<द्विगुण>(Constr.पङ्क्तिकाः,Constr.स्तम्भसङ्ख्या+1);
    यदि(Func.पङ्क्तिकाः==1){
        Func.convert_गन्तव्यम्(bigC.colRange(1,bigC.स्तम्भसङ्ख्या),CV_64FC1);
    }अन्यथा{
        Mat FuncT=Func.t();
        FuncT.convert_गन्तव्यम्(bigC.colRange(1,bigC.स्तम्भसङ्ख्या),CV_64FC1);
    }
    Constr.convert_गन्तव्यम्(bigB.colRange(1,bigB.स्तम्भसङ्ख्या),CV_64FC1);
    द्विगुण v=0;
    vector<पूर्णाङ्क> N,B;
    vector<अचिह्नित पूर्णाङ्क> सूचकांक_गन्तव्यम्_Row;

    यदि(initialize_simplex(bigC,bigB,v,N,B,सूचकांक_गन्तव्यम्_Row)==SOLVELP_UNFEASIBLE){
        प्रत्यागम SOLVELP_UNFEASIBLE;
    }
    Mat_<द्विगुण> c=bigC.colRange(1,bigC.स्तम्भसङ्ख्या),
        b=bigB.colRange(1,bigB.स्तम्भसङ्ख्या);

    पूर्णाङ्क res=0;
    यदि((res=inner_simplex(c,b,v,N,B,सूचकांक_गन्तव्यम्_Row))==SOLVELP_UNBOUNDED){
        प्रत्यागम SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.स्तम्भसङ्ख्या,1,CV_64FC1);
    MatIterator_<द्विगुण> it=z.begin<द्विगुण>();
    अचिह्नित पूर्णाङ्क nsize = (अचिह्नित पूर्णाङ्क)N.size();
    प्रति(पूर्णाङ्क i=1;i<=c.स्तम्भसङ्ख्या;i++,it++){
        यदि(सूचकांक_गन्तव्यम्_Row[i]<nsize){
            *it=0;
        }अन्यथा{
            *it=b.at<द्विगुण>(सूचकांक_गन्तव्यम्_Row[i]-nsize,b.स्तम्भसङ्ख्या-1);
        }
    }

    z.copy_गन्तव्यम्(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.स्तम्भसङ्ख्या - 1, Constr.पङ्क्तिकाः)) * z;
    Mat constr_check = Constr.col(Constr.स्तम्भसङ्ख्या - 1) - prod;
    द्विगुण min_मानम् = 0.0;
    minMaxIdx(constr_check, &min_मानम्);
    यदि (min_मानम् < -constr_eps)
    {
        प्रत्यागम SOLVELP_नष्टम्;
    }

    प्रत्यागम res;
}

पूर्णाङ्क solveLP(निवेशः_Array Func, निवेशः_Array Constr, OutputArray z)
{
    प्रत्यागम solveLP(Func, Constr, z, 1e-12);
}

स्थिर पूर्णाङ्क initialize_simplex(Mat_<द्विगुण>& c, Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,vector<अचिह्नित पूर्णाङ्क>& सूचकांक_गन्तव्यम्_Row){
    N.resize(c.स्तम्भसङ्ख्या);
    N[0]=0;
    प्रति (std::vector<पूर्णाङ्क>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.पङ्क्तिकाः);
    B[0]=(पूर्णाङ्क)N.size();
    प्रति (std::vector<पूर्णाङ्क>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    सूचकांक_गन्तव्यम्_Row.resize(c.स्तम्भसङ्ख्या+b.पङ्क्तिकाः);
    सूचकांक_गन्तव्यम्_Row[0]=0;
    प्रति (std::vector<अचिह्नित पूर्णाङ्क>::iterator it = सूचकांक_गन्तव्यम्_Row.begin()+1 ; it != सूचकांक_गन्तव्यम्_Row.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    पूर्णाङ्क k=0;
    {
        द्विगुण min=DBL_MAX;
        प्रति(पूर्णाङ्क i=0;i<b.पङ्क्तिकाः;i++){
            यदि(b(i,b.स्तम्भसङ्ख्या-1)<min){
                min=b(i,b.स्तम्भसङ्ख्या-1);
                k=i;
            }
        }
    }

    यदि(b(k,b.स्तम्भसङ्ख्या-1)>=0){
        N.erase(N.begin());
        प्रति (std::vector<अचिह्नित पूर्णाङ्क>::iterator it = सूचकांक_गन्तव्यम्_Row.begin()+1 ; it != सूचकांक_गन्तव्यम्_Row.end(); ++it){
            --(*it);
        }
        प्रत्यागम 0;
    }

    Mat_<द्विगुण> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    प्रति(पूर्णाङ्क i=0;i<b.पङ्क्तिकाः;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    pivot(c,b,v,N,B,k,0,सूचकांक_गन्तव्यम्_Row);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,सूचकांक_गन्तव्यम्_Row);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    अचिह्नित पूर्णाङ्क nsize = (अचिह्नित पूर्णाङ्क)N.size();
    यदि(सूचकांक_गन्तव्यम्_Row[0]>=nsize){
        पूर्णाङ्क iterator_विस्थापनम्=सूचकांक_गन्तव्यम्_Row[0]-nsize;
        यदि(b(iterator_विस्थापनम्,b.स्तम्भसङ्ख्या-1)>0){
            प्रत्यागम SOLVELP_UNFEASIBLE;
        }
        pivot(c,b,v,N,B,iterator_विस्थापनम्,0,सूचकांक_गन्तव्यम्_Row);
    }

    vector<पूर्णाङ्क>::iterator iterator;
    {
        पूर्णाङ्क iterator_विस्थापनम्=सूचकांक_गन्तव्यम्_Row[0];
        iterator=N.begin()+iterator_विस्थापनम्;
        std::iter_swap(iterator,N.begin());
        SWAP(पूर्णाङ्क,सूचकांक_गन्तव्यम्_Row[*iterator],सूचकांक_गन्तव्यम्_Row[0]);
        swap_columns(c,iterator_विस्थापनम्,0);
        swap_columns(b,iterator_विस्थापनम्,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    प्रति(पूर्णाङ्क I=1;I<old_c.स्तम्भसङ्ख्या;I++){
        यदि(सूचकांक_गन्तव्यम्_Row[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            पूर्णाङ्क iterator_विस्थापनम्=सूचकांक_गन्तव्यम्_Row[I];
            c(0,iterator_विस्थापनम्)+=old_c(0,I);
            print_matrix(c);
        }अन्यथा{
            dprintf(("I=%d from basic\n",I));
            पूर्णाङ्क iterator_विस्थापनम्=सूचकांक_गन्तव्यम्_Row[I]-nsize;
            c-=old_c(0,I)*b.row(iterator_विस्थापनम्).colRange(0,b.स्तम्भसङ्ख्या-1);
            v+=old_c(0,I)*b(iterator_विस्थापनम्,b.स्तम्भसङ्ख्या-1);
            print_matrix(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    प्रति (std::vector<अचिह्नित पूर्णाङ्क>::iterator it = सूचकांक_गन्तव्यम्_Row.begin()+1 ; it != सूचकांक_गन्तव्यम्_Row.end(); ++it){
        --(*it);
    }
    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क inner_simplex(Mat_<द्विगुण>& c, Mat_<द्विगुण>& b,द्विगुण& v,vector<पूर्णाङ्क>& N,vector<पूर्णाङ्क>& B,vector<अचिह्नित पूर्णाङ्क>& सूचकांक_गन्तव्यम्_Row){

    प्रति(;;){
        MatIterator_<द्विगुण> pos_ptr;
        पूर्णाङ्क e=-1,pos_ctr=0,min_var=अधिकतम_पूर्णाङ्कः;
        तार्किक all_nonzero=सत्य;
        प्रति(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            यदि(*pos_ptr==0){
                all_nonzero=असत्य;
            }
            यदि(*pos_ptr>0){
                यदि(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        यदि(e==-1){
            dprintf(("hello from e==-1\n"));
            print_matrix(c);
            यदि(all_nonzero==सत्य){
                प्रत्यागम SOLVELP_SINGLE;
            }अन्यथा{
                प्रत्यागम SOLVELP_MULTI;
            }
        }

        पूर्णाङ्क l=-1;
        min_var=अधिकतम_पूर्णाङ्कः;
        द्विगुण min=DBL_MAX;
        पूर्णाङ्क row_it=0;
        MatIterator_<द्विगुण> min_row_ptr=b.begin();
        प्रति(MatIterator_<द्विगुण> it=b.begin();it!=b.end();it+=b.स्तम्भसङ्ख्या,row_it++){
            द्विगुण myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            यदि((myite=it[e])>0){
                द्विगुण val=it[b.स्तम्भसङ्ख्या-1]/myite;
                यदि(val<min || (val==min && B[row_it]<min_var)){
                    min_var=B[row_it];
                    min_row_ptr=it;
                    min=val;
                    l=row_it;
                }
            }
        }
        यदि(l==-1){
            प्रत्यागम SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        pivot(c,b,v,N,B,l,e,सूचकांक_गन्तव्यम्_Row);

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
        पूर्णाङ्क leaving_सूचकांक,पूर्णाङ्क entering_सूचकांक,vector<अचिह्नित पूर्णाङ्क>& सूचकांक_गन्तव्यम्_Row){
    द्विगुण Coef=b(leaving_सूचकांक,entering_सूचकांक);
    प्रति(पूर्णाङ्क i=0;i<b.स्तम्भसङ्ख्या;i++){
        यदि(i==entering_सूचकांक){
            b(leaving_सूचकांक,i)=1/Coef;
        }अन्यथा{
            b(leaving_सूचकांक,i)/=Coef;
        }
    }

    प्रति(पूर्णाङ्क i=0;i<b.पङ्क्तिकाः;i++){
        यदि(i!=leaving_सूचकांक){
            द्विगुण coef=b(i,entering_सूचकांक);
            प्रति(पूर्णाङ्क j=0;j<b.स्तम्भसङ्ख्या;j++){
                यदि(j==entering_सूचकांक){
                    b(i,j)=-coef*b(leaving_सूचकांक,j);
                }अन्यथा{
                    b(i,j)-=(coef*b(leaving_सूचकांक,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_सूचकांक);
    प्रति(पूर्णाङ्क i=0;i<(b.स्तम्भसङ्ख्या-1);i++){
        यदि(i==entering_सूचकांक){
            c(0,i)=-Coef*b(leaving_सूचकांक,i);
        }अन्यथा{
            c(0,i)-=Coef*b(leaving_सूचकांक,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_सूचकांक,b.स्तम्भसङ्ख्या-1);

    SWAP(पूर्णाङ्क,N[entering_सूचकांक],B[leaving_सूचकांक]);
    SWAP(पूर्णाङ्क,सूचकांक_गन्तव्यम्_Row[N[entering_सूचकांक]],सूचकांक_गन्तव्यम्_Row[B[leaving_सूचकांक]]);
}

स्थिर अन्तःस्थ शून्य swap_columns(Mat_<द्विगुण>& A,पूर्णाङ्क col1,पूर्णाङ्क col2){
    प्रति(पूर्णाङ्क i=0;i<A.पङ्क्तिकाः;i++){
        द्विगुण tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
