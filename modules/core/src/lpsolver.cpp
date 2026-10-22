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
#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ <climits>
#అన్తర్భావ <algorithm>

#నిర్వచించు dprintf(x)
#నిర్వచించు print_మాతృక(x)

నామాకాశ cv
{

ఉపయోగించి std::vector;

#నిర్వచితమైతే ALEX_DEBUG
స్థిర శూన్య print_simplex_state(స్థిరరాశి Mat& c,స్థిరరాశి Mat& b,ద్విగుణ v,స్థిరరాశి std::vector<పూర్ణాంకం> N,స్థిరరాశి std::vector<పూర్ణాంకం> B){
    రాయి("\tprint simplex state\n");

    రాయి("v=%g\n",v);

    రాయి("here c goes\n");
    print_మాతృక(c);

    రాయి("non-basic: ");
    print(Mat(N));
    రాయి("\n");

    రాయి("here b goes\n");
    print_మాతృక(b);
    రాయి("basic: ");

    print(Mat(B));
    రాయి("\n");
}
#లేకపోతే
#నిర్వచించు print_simplex_state(c,b,v,N,B)
#అయితే_ముగింపు

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
స్థిర పూర్ణాంకం initialize_simplex(Mat_<ద్విగుణ>& c, Mat_<ద్విగుణ>& b,ద్విగుణ& v,vector<పూర్ణాంకం>& N,vector<పూర్ణాంకం>& B,vector<గుర్తులేని పూర్ణాంకం>& సూచీ_గమ్యం_అడ్డువరుస);
స్థిర అంతఃవరుస శూన్య ఇరుసు(Mat_<ద్విగుణ>& c,Mat_<ద్విగుణ>& b,ద్విగుణ& v,vector<పూర్ణాంకం>& N,vector<పూర్ణాంకం>& B,పూర్ణాంకం leaving_సూచీ,
        పూర్ణాంకం entering_సూచీ,vector<గుర్తులేని పూర్ణాంకం>& సూచీ_గమ్యం_అడ్డువరుస);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
స్థిర పూర్ణాంకం inner_simplex(Mat_<ద్విగుణ>& c, Mat_<ద్విగుణ>& b,ద్విగుణ& v,vector<పూర్ణాంకం>& N,vector<పూర్ణాంకం>& B,vector<గుర్తులేని పూర్ణాంకం>& సూచీ_గమ్యం_అడ్డువరుస);
స్థిర శూన్య మార్పిడి_columns(Mat_<ద్విగుణ>& A,పూర్ణాంకం col1,పూర్ణాంకం col2);
#నిర్వచించు మార్పిడి(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
పూర్ణాంకం solveLP(ఆదానం_Array Func_, ఆదానం_Array Constr_, OutputArray z_, ద్విగుణ constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.అడ్డువరుసలు()==1 && (Constr_.నిలువుల_సంఖ్య()-Func_.నిలువుల_సంఖ్య()==1))||
            (Func_.నిలువుల_సంఖ్య()==1 && (Constr_.నిలువుల_సంఖ్య()-Func_.అడ్డువరుసలు()==1)));
    ఒకవేళ (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<ద్విగుణ> bigC=Mat_<ద్విగుణ>(1,(Func.అడ్డువరుసలు==1?Func.నిలువుల_సంఖ్య:Func.అడ్డువరుసలు)+1),
        bigB=Mat_<ద్విగుణ>(Constr.అడ్డువరుసలు,Constr.నిలువుల_సంఖ్య+1);
    ఒకవేళ(Func.అడ్డువరుసలు==1){
        Func.convert_గమ్యం(bigC.నిలువు_Range_2(1,bigC.నిలువుల_సంఖ్య),CV_64FC1);
    }లేకపోతే{
        Mat FuncT=Func.t();
        FuncT.convert_గమ్యం(bigC.నిలువు_Range_2(1,bigC.నిలువుల_సంఖ్య),CV_64FC1);
    }
    Constr.convert_గమ్యం(bigB.నిలువు_Range_2(1,bigB.నిలువుల_సంఖ్య),CV_64FC1);
    ద్విగుణ v=0;
    vector<పూర్ణాంకం> N,B;
    vector<గుర్తులేని పూర్ణాంకం> సూచీ_గమ్యం_అడ్డువరుస;

    ఒకవేళ(initialize_simplex(bigC,bigB,v,N,B,సూచీ_గమ్యం_అడ్డువరుస)==SOLVELP_UNFEASIBLE){
        తిరిగి SOLVELP_UNFEASIBLE;
    }
    Mat_<ద్విగుణ> c=bigC.నిలువు_Range_2(1,bigC.నిలువుల_సంఖ్య),
        b=bigB.నిలువు_Range_2(1,bigB.నిలువుల_సంఖ్య);

    పూర్ణాంకం res=0;
    ఒకవేళ((res=inner_simplex(c,b,v,N,B,సూచీ_గమ్యం_అడ్డువరుస))==SOLVELP_UNBOUNDED){
        తిరిగి SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.నిలువుల_సంఖ్య,1,CV_64FC1);
    MatIterator_<ద్విగుణ> it=z.begin<ద్విగుణ>();
    గుర్తులేని పూర్ణాంకం nsize = (గుర్తులేని పూర్ణాంకం)N.size();
    కొరకు(పూర్ణాంకం i=1;i<=c.నిలువుల_సంఖ్య;i++,it++){
        ఒకవేళ(సూచీ_గమ్యం_అడ్డువరుస[i]<nsize){
            *it=0;
        }లేకపోతే{
            *it=b.at<ద్విగుణ>(సూచీ_గమ్యం_అడ్డువరుస[i]-nsize,b.నిలువుల_సంఖ్య-1);
        }
    }

    z.copy_గమ్యం(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.నిలువుల_సంఖ్య - 1, Constr.అడ్డువరుసలు)) * z;
    Mat constr_check = Constr.నిలువు(Constr.నిలువుల_సంఖ్య - 1) - prod;
    ద్విగుణ min_విలువ = 0.0;
    minMaxIdx(constr_check, &min_విలువ);
    ఒకవేళ (min_విలువ < -constr_eps)
    {
        తిరిగి SOLVELP_కోల్పోయిన;
    }

    తిరిగి res;
}

పూర్ణాంకం solveLP(ఆదానం_Array Func, ఆదానం_Array Constr, OutputArray z)
{
    తిరిగి solveLP(Func, Constr, z, 1e-12);
}

స్థిర పూర్ణాంకం initialize_simplex(Mat_<ద్విగుణ>& c, Mat_<ద్విగుణ>& b,ద్విగుణ& v,vector<పూర్ణాంకం>& N,vector<పూర్ణాంకం>& B,vector<గుర్తులేని పూర్ణాంకం>& సూచీ_గమ్యం_అడ్డువరుస){
    N.resize(c.నిలువుల_సంఖ్య);
    N[0]=0;
    కొరకు (std::vector<పూర్ణాంకం>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.అడ్డువరుసలు);
    B[0]=(పూర్ణాంకం)N.size();
    కొరకు (std::vector<పూర్ణాంకం>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    సూచీ_గమ్యం_అడ్డువరుస.resize(c.నిలువుల_సంఖ్య+b.అడ్డువరుసలు);
    సూచీ_గమ్యం_అడ్డువరుస[0]=0;
    కొరకు (std::vector<గుర్తులేని పూర్ణాంకం>::iterator it = సూచీ_గమ్యం_అడ్డువరుస.begin()+1 ; it != సూచీ_గమ్యం_అడ్డువరుస.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    పూర్ణాంకం k=0;
    {
        ద్విగుణ min=DBL_MAX;
        కొరకు(పూర్ణాంకం i=0;i<b.అడ్డువరుసలు;i++){
            ఒకవేళ(b(i,b.నిలువుల_సంఖ్య-1)<min){
                min=b(i,b.నిలువుల_సంఖ్య-1);
                k=i;
            }
        }
    }

    ఒకవేళ(b(k,b.నిలువుల_సంఖ్య-1)>=0){
        N.erase(N.begin());
        కొరకు (std::vector<గుర్తులేని పూర్ణాంకం>::iterator it = సూచీ_గమ్యం_అడ్డువరుస.begin()+1 ; it != సూచీ_గమ్యం_అడ్డువరుస.end(); ++it){
            --(*it);
        }
        తిరిగి 0;
    }

    Mat_<ద్విగుణ> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    కొరకు(పూర్ణాంకం i=0;i<b.అడ్డువరుసలు;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    ఇరుసు(c,b,v,N,B,k,0,సూచీ_గమ్యం_అడ్డువరుస);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,సూచీ_గమ్యం_అడ్డువరుస);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    గుర్తులేని పూర్ణాంకం nsize = (గుర్తులేని పూర్ణాంకం)N.size();
    ఒకవేళ(సూచీ_గమ్యం_అడ్డువరుస[0]>=nsize){
        పూర్ణాంకం iterator_స్థానభ్రంశం=సూచీ_గమ్యం_అడ్డువరుస[0]-nsize;
        ఒకవేళ(b(iterator_స్థానభ్రంశం,b.నిలువుల_సంఖ్య-1)>0){
            తిరిగి SOLVELP_UNFEASIBLE;
        }
        ఇరుసు(c,b,v,N,B,iterator_స్థానభ్రంశం,0,సూచీ_గమ్యం_అడ్డువరుస);
    }

    vector<పూర్ణాంకం>::iterator iterator;
    {
        పూర్ణాంకం iterator_స్థానభ్రంశం=సూచీ_గమ్యం_అడ్డువరుస[0];
        iterator=N.begin()+iterator_స్థానభ్రంశం;
        std::iter_swap(iterator,N.begin());
        మార్పిడి(పూర్ణాంకం,సూచీ_గమ్యం_అడ్డువరుస[*iterator],సూచీ_గమ్యం_అడ్డువరుస[0]);
        మార్పిడి_columns(c,iterator_స్థానభ్రంశం,0);
        మార్పిడి_columns(b,iterator_స్థానభ్రంశం,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    కొరకు(పూర్ణాంకం I=1;I<old_c.నిలువుల_సంఖ్య;I++){
        ఒకవేళ(సూచీ_గమ్యం_అడ్డువరుస[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            పూర్ణాంకం iterator_స్థానభ్రంశం=సూచీ_గమ్యం_అడ్డువరుస[I];
            c(0,iterator_స్థానభ్రంశం)+=old_c(0,I);
            print_మాతృక(c);
        }లేకపోతే{
            dprintf(("I=%d from basic\n",I));
            పూర్ణాంకం iterator_స్థానభ్రంశం=సూచీ_గమ్యం_అడ్డువరుస[I]-nsize;
            c-=old_c(0,I)*b.అడ్డువరుస(iterator_స్థానభ్రంశం).నిలువు_Range_2(0,b.నిలువుల_సంఖ్య-1);
            v+=old_c(0,I)*b(iterator_స్థానభ్రంశం,b.నిలువుల_సంఖ్య-1);
            print_మాతృక(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    కొరకు (std::vector<గుర్తులేని పూర్ణాంకం>::iterator it = సూచీ_గమ్యం_అడ్డువరుస.begin()+1 ; it != సూచీ_గమ్యం_అడ్డువరుస.end(); ++it){
        --(*it);
    }
    తిరిగి 0;
}

స్థిర పూర్ణాంకం inner_simplex(Mat_<ద్విగుణ>& c, Mat_<ద్విగుణ>& b,ద్విగుణ& v,vector<పూర్ణాంకం>& N,vector<పూర్ణాంకం>& B,vector<గుర్తులేని పూర్ణాంకం>& సూచీ_గమ్యం_అడ్డువరుస){

    కొరకు(;;){
        MatIterator_<ద్విగుణ> pos_ptr;
        పూర్ణాంకం e=-1,pos_ctr=0,min_var=గరిష్ఠ_పూర్ణాంకం;
        తార్కిక all_nonzero=నిజం;
        కొరకు(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            ఒకవేళ(*pos_ptr==0){
                all_nonzero=అబద్ధం;
            }
            ఒకవేళ(*pos_ptr>0){
                ఒకవేళ(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        ఒకవేళ(e==-1){
            dprintf(("hello from e==-1\n"));
            print_మాతృక(c);
            ఒకవేళ(all_nonzero==నిజం){
                తిరిగి SOLVELP_SINGLE;
            }లేకపోతే{
                తిరిగి SOLVELP_MULTI;
            }
        }

        పూర్ణాంకం l=-1;
        min_var=గరిష్ఠ_పూర్ణాంకం;
        ద్విగుణ min=DBL_MAX;
        పూర్ణాంకం అడ్డువరుస_it=0;
        MatIterator_<ద్విగుణ> min_అడ్డువరుస_ptr=b.begin();
        కొరకు(MatIterator_<ద్విగుణ> it=b.begin();it!=b.end();it+=b.నిలువుల_సంఖ్య,అడ్డువరుస_it++){
            ద్విగుణ myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            ఒకవేళ((myite=it[e])>0){
                ద్విగుణ val=it[b.నిలువుల_సంఖ్య-1]/myite;
                ఒకవేళ(val<min || (val==min && B[అడ్డువరుస_it]<min_var)){
                    min_var=B[అడ్డువరుస_it];
                    min_అడ్డువరుస_ptr=it;
                    min=val;
                    l=అడ్డువరుస_it;
                }
            }
        }
        ఒకవేళ(l==-1){
            తిరిగి SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        ఇరుసు(c,b,v,N,B,l,e,సూచీ_గమ్యం_అడ్డువరుస);

        dprintf(("objective, v=%g\n",v));
        print_మాతృక(c);
        dprintf(("constraints\n"));
        print_మాతృక(b);
        dprintf(("non-basic: "));
        print_మాతృక(Mat(N));
        dprintf(("basic: "));
        print_మాతృక(Mat(B));
    }
}

స్థిర అంతఃవరుస శూన్య ఇరుసు(Mat_<ద్విగుణ>& c,Mat_<ద్విగుణ>& b,ద్విగుణ& v,vector<పూర్ణాంకం>& N,vector<పూర్ణాంకం>& B,
        పూర్ణాంకం leaving_సూచీ,పూర్ణాంకం entering_సూచీ,vector<గుర్తులేని పూర్ణాంకం>& సూచీ_గమ్యం_అడ్డువరుస){
    ద్విగుణ Coef=b(leaving_సూచీ,entering_సూచీ);
    కొరకు(పూర్ణాంకం i=0;i<b.నిలువుల_సంఖ్య;i++){
        ఒకవేళ(i==entering_సూచీ){
            b(leaving_సూచీ,i)=1/Coef;
        }లేకపోతే{
            b(leaving_సూచీ,i)/=Coef;
        }
    }

    కొరకు(పూర్ణాంకం i=0;i<b.అడ్డువరుసలు;i++){
        ఒకవేళ(i!=leaving_సూచీ){
            ద్విగుణ coef=b(i,entering_సూచీ);
            కొరకు(పూర్ణాంకం j=0;j<b.నిలువుల_సంఖ్య;j++){
                ఒకవేళ(j==entering_సూచీ){
                    b(i,j)=-coef*b(leaving_సూచీ,j);
                }లేకపోతే{
                    b(i,j)-=(coef*b(leaving_సూచీ,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_సూచీ);
    కొరకు(పూర్ణాంకం i=0;i<(b.నిలువుల_సంఖ్య-1);i++){
        ఒకవేళ(i==entering_సూచీ){
            c(0,i)=-Coef*b(leaving_సూచీ,i);
        }లేకపోతే{
            c(0,i)-=Coef*b(leaving_సూచీ,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_సూచీ,b.నిలువుల_సంఖ్య-1);

    మార్పిడి(పూర్ణాంకం,N[entering_సూచీ],B[leaving_సూచీ]);
    మార్పిడి(పూర్ణాంకం,సూచీ_గమ్యం_అడ్డువరుస[N[entering_సూచీ]],సూచీ_గమ్యం_అడ్డువరుస[B[leaving_సూచీ]]);
}

స్థిర అంతఃవరుస శూన్య మార్పిడి_columns(Mat_<ద్విగుణ>& A,పూర్ణాంకం col1,పూర్ణాంకం col2){
    కొరకు(పూర్ణాంకం i=0;i<A.అడ్డువరుసలు;i++){
        ద్విగుణ tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
