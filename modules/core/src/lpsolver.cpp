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
#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <climits>
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dprintf(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ print_ਮੈਟ੍ਰਿਕਸ(x)

ਨਾਮਾਕਾਸ਼ cv
{

ਵਰਤੋਂ std::vector;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ALEX_DEBUG
ਸਥਿਰ ਖਾਲੀ print_simplex_state(ਅਚਲ Mat& c,ਅਚਲ Mat& b,ਦੁੱਗਣਾ v,ਅਚਲ std::vector<ਪੂਰਨ> N,ਅਚਲ std::vector<ਪੂਰਨ> B){
    ਲਿਖੋ("\tprint simplex state\n");

    ਲਿਖੋ("v=%g\n",v);

    ਲਿਖੋ("here c goes\n");
    print_ਮੈਟ੍ਰਿਕਸ(c);

    ਲਿਖੋ("non-basic: ");
    print(Mat(N));
    ਲਿਖੋ("\n");

    ਲਿਖੋ("here b goes\n");
    print_ਮੈਟ੍ਰਿਕਸ(b);
    ਲਿਖੋ("basic: ");

    print(Mat(B));
    ਲਿਖੋ("\n");
}
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ print_simplex_state(c,b,v,N,B)
#ਜੇ_ਅੰਤ

/**Due to technical considerations, the format of input b and c is somewhat special:
 *both b and c should be one column bigger than corresponding b and c of linear problem and the leftmost column will be used internally
 by this procedure - it should not be cleaned before the call to procedure and may contain mess after
 it also initializes N and B and does not make any assumptions about their init values
 * @return SOLVELP_UNFEASIBLE if problem is unfeasible, 0 if feasible.
*/
ਸਥਿਰ ਪੂਰਨ initialize_simplex(Mat_<ਦੁੱਗਣਾ>& c, Mat_<ਦੁੱਗਣਾ>& b,ਦੁੱਗਣਾ& v,vector<ਪੂਰਨ>& N,vector<ਪੂਰਨ>& B,vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>& ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ);
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਧੁਰਾ(Mat_<ਦੁੱਗਣਾ>& c,Mat_<ਦੁੱਗਣਾ>& b,ਦੁੱਗਣਾ& v,vector<ਪੂਰਨ>& N,vector<ਪੂਰਨ>& B,ਪੂਰਨ leaving_ਸੂਚਕਾਂਕ,
        ਪੂਰਨ entering_ਸੂਚਕਾਂਕ,vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>& ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ);
/**@return SOLVELP_UNBOUNDED means the problem is unbdd, SOLVELP_MULTI means multiple solutions, SOLVELP_SINGLE means one solution.
 */
ਸਥਿਰ ਪੂਰਨ inner_simplex(Mat_<ਦੁੱਗਣਾ>& c, Mat_<ਦੁੱਗਣਾ>& b,ਦੁੱਗਣਾ& v,vector<ਪੂਰਨ>& N,vector<ਪੂਰਨ>& B,vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>& ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ);
ਸਥਿਰ ਖਾਲੀ ਅਦਲਾ_ਬਦਲੀ_columns(Mat_<ਦੁੱਗਣਾ>& A,ਪੂਰਨ col1,ਪੂਰਨ col2);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅਦਲਾ_ਬਦਲੀ(type,a,b) {type tmp=(a);(a)=(b);(b)=tmp;}

//return codes:-2 (no_sol - unbdd),-1(no_sol - unfsbl), 0(single_sol), 1(multiple_sol=>least_l2_norm)
ਪੂਰਨ solveLP(ਨਿਵੇਸ਼_Array Func_, ਨਿਵੇਸ਼_Array Constr_, OutputArray z_, ਦੁੱਗਣਾ constr_eps)
{
    dprintf(("call to solveLP\n"));

    //sanity check (size, type, no. of channels)
    CV_Assert(Func_.type()==CV_64FC1 || Func_.type()==CV_32FC1);
    CV_Assert(Constr_.type()==CV_64FC1 || Constr_.type()==CV_32FC1);
    CV_Assert((Func_.ਪੰਕਤੀਆਂ()==1 && (Constr_.ਖਾਨੇ_ਗਿਣਤੀ()-Func_.ਖਾਨੇ_ਗਿਣਤੀ()==1))||
            (Func_.ਖਾਨੇ_ਗਿਣਤੀ()==1 && (Constr_.ਖਾਨੇ_ਗਿਣਤੀ()-Func_.ਪੰਕਤੀਆਂ()==1)));
    ਜੇ (z_.fixedType())
        CV_CheckType(z_.type(), z_.type() == CV_64FC1 || z_.type() == CV_32FC1 || z_.type() == CV_32SC1, "");

    Mat Func = Func_.getMat();
    Mat Constr = Constr_.getMat();

    //copy arguments for we will shall modify them
    Mat_<ਦੁੱਗਣਾ> bigC=Mat_<ਦੁੱਗਣਾ>(1,(Func.ਪੰਕਤੀਆਂ==1?Func.ਖਾਨੇ_ਗਿਣਤੀ:Func.ਪੰਕਤੀਆਂ)+1),
        bigB=Mat_<ਦੁੱਗਣਾ>(Constr.ਪੰਕਤੀਆਂ,Constr.ਖਾਨੇ_ਗਿਣਤੀ+1);
    ਜੇ(Func.ਪੰਕਤੀਆਂ==1){
        Func.convert_ਮੰਜ਼ਿਲ(bigC.ਖਾਨਾ_Range_2(1,bigC.ਖਾਨੇ_ਗਿਣਤੀ),CV_64FC1);
    }ਨਹੀਂਤਾਂ{
        Mat FuncT=Func.t();
        FuncT.convert_ਮੰਜ਼ਿਲ(bigC.ਖਾਨਾ_Range_2(1,bigC.ਖਾਨੇ_ਗਿਣਤੀ),CV_64FC1);
    }
    Constr.convert_ਮੰਜ਼ਿਲ(bigB.ਖਾਨਾ_Range_2(1,bigB.ਖਾਨੇ_ਗਿਣਤੀ),CV_64FC1);
    ਦੁੱਗਣਾ v=0;
    vector<ਪੂਰਨ> N,B;
    vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ;

    ਜੇ(initialize_simplex(bigC,bigB,v,N,B,ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ)==SOLVELP_UNFEASIBLE){
        ਵਾਪਸ SOLVELP_UNFEASIBLE;
    }
    Mat_<ਦੁੱਗਣਾ> c=bigC.ਖਾਨਾ_Range_2(1,bigC.ਖਾਨੇ_ਗਿਣਤੀ),
        b=bigB.ਖਾਨਾ_Range_2(1,bigB.ਖਾਨੇ_ਗਿਣਤੀ);

    ਪੂਰਨ res=0;
    ਜੇ((res=inner_simplex(c,b,v,N,B,ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ))==SOLVELP_UNBOUNDED){
        ਵਾਪਸ SOLVELP_UNBOUNDED;
    }

    //return the optimal solution
    Mat z(c.ਖਾਨੇ_ਗਿਣਤੀ,1,CV_64FC1);
    MatIterator_<ਦੁੱਗਣਾ> it=z.begin<ਦੁੱਗਣਾ>();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nsize = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)N.size();
    ਲਈ(ਪੂਰਨ i=1;i<=c.ਖਾਨੇ_ਗਿਣਤੀ;i++,it++){
        ਜੇ(ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[i]<nsize){
            *it=0;
        }ਨਹੀਂਤਾਂ{
            *it=b.at<ਦੁੱਗਣਾ>(ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[i]-nsize,b.ਖਾਨੇ_ਗਿਣਤੀ-1);
        }
    }

    z.copy_ਮੰਜ਼ਿਲ(z_);

    //check constraints feasibility
    Mat prod = Constr(Rect(0, 0, Constr.ਖਾਨੇ_ਗਿਣਤੀ - 1, Constr.ਪੰਕਤੀਆਂ)) * z;
    Mat constr_check = Constr.ਖਾਨਾ(Constr.ਖਾਨੇ_ਗਿਣਤੀ - 1) - prod;
    ਦੁੱਗਣਾ min_ਮੁੱਲ = 0.0;
    minMaxIdx(constr_check, &min_ਮੁੱਲ);
    ਜੇ (min_ਮੁੱਲ < -constr_eps)
    {
        ਵਾਪਸ SOLVELP_ਗੁਆਚਾ;
    }

    ਵਾਪਸ res;
}

ਪੂਰਨ solveLP(ਨਿਵੇਸ਼_Array Func, ਨਿਵੇਸ਼_Array Constr, OutputArray z)
{
    ਵਾਪਸ solveLP(Func, Constr, z, 1e-12);
}

ਸਥਿਰ ਪੂਰਨ initialize_simplex(Mat_<ਦੁੱਗਣਾ>& c, Mat_<ਦੁੱਗਣਾ>& b,ਦੁੱਗਣਾ& v,vector<ਪੂਰਨ>& N,vector<ਪੂਰਨ>& B,vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>& ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ){
    N.resize(c.ਖਾਨੇ_ਗਿਣਤੀ);
    N[0]=0;
    ਲਈ (std::vector<ਪੂਰਨ>::iterator it = N.begin()+1 ; it != N.end(); ++it){
        *it=it[-1]+1;
    }
    B.resize(b.ਪੰਕਤੀਆਂ);
    B[0]=(ਪੂਰਨ)N.size();
    ਲਈ (std::vector<ਪੂਰਨ>::iterator it = B.begin()+1 ; it != B.end(); ++it){
        *it=it[-1]+1;
    }
    ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ.resize(c.ਖਾਨੇ_ਗਿਣਤੀ+b.ਪੰਕਤੀਆਂ);
    ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[0]=0;
    ਲਈ (std::vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>::iterator it = ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ.begin()+1 ; it != ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ.end(); ++it){
        *it=it[-1]+1;
    }
    v=0;

    ਪੂਰਨ k=0;
    {
        ਦੁੱਗਣਾ min=DBL_MAX;
        ਲਈ(ਪੂਰਨ i=0;i<b.ਪੰਕਤੀਆਂ;i++){
            ਜੇ(b(i,b.ਖਾਨੇ_ਗਿਣਤੀ-1)<min){
                min=b(i,b.ਖਾਨੇ_ਗਿਣਤੀ-1);
                k=i;
            }
        }
    }

    ਜੇ(b(k,b.ਖਾਨੇ_ਗਿਣਤੀ-1)>=0){
        N.erase(N.begin());
        ਲਈ (std::vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>::iterator it = ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ.begin()+1 ; it != ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ.end(); ++it){
            --(*it);
        }
        ਵਾਪਸ 0;
    }

    Mat_<ਦੁੱਗਣਾ> old_c=c.clone();
    c=0;
    c(0,0)=-1;
    ਲਈ(ਪੂਰਨ i=0;i<b.ਪੰਕਤੀਆਂ;i++){
        b(i,0)=-1;
    }

    print_simplex_state(c,b,v,N,B);

    dprintf(("\tWE MAKE PIVOT\n"));
    ਧੁਰਾ(c,b,v,N,B,k,0,ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ);

    print_simplex_state(c,b,v,N,B);

    inner_simplex(c,b,v,N,B,ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ);

    dprintf(("\tAFTER INNER_SIMPLEX\n"));
    print_simplex_state(c,b,v,N,B);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nsize = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)N.size();
    ਜੇ(ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[0]>=nsize){
        ਪੂਰਨ iterator_ਵਿੱਥ=ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[0]-nsize;
        ਜੇ(b(iterator_ਵਿੱਥ,b.ਖਾਨੇ_ਗਿਣਤੀ-1)>0){
            ਵਾਪਸ SOLVELP_UNFEASIBLE;
        }
        ਧੁਰਾ(c,b,v,N,B,iterator_ਵਿੱਥ,0,ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ);
    }

    vector<ਪੂਰਨ>::iterator iterator;
    {
        ਪੂਰਨ iterator_ਵਿੱਥ=ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[0];
        iterator=N.begin()+iterator_ਵਿੱਥ;
        std::iter_swap(iterator,N.begin());
        ਅਦਲਾ_ਬਦਲੀ(ਪੂਰਨ,ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[*iterator],ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[0]);
        ਅਦਲਾ_ਬਦਲੀ_columns(c,iterator_ਵਿੱਥ,0);
        ਅਦਲਾ_ਬਦਲੀ_columns(b,iterator_ਵਿੱਥ,0);
    }

    dprintf(("after swaps\n"));
    print_simplex_state(c,b,v,N,B);

    //start from 1, because we ignore x_0
    c=0;
    v=0;
    ਲਈ(ਪੂਰਨ I=1;I<old_c.ਖਾਨੇ_ਗਿਣਤੀ;I++){
        ਜੇ(ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[I]<nsize){
            dprintf(("I=%d from nonbasic\n",I));
            ਪੂਰਨ iterator_ਵਿੱਥ=ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[I];
            c(0,iterator_ਵਿੱਥ)+=old_c(0,I);
            print_ਮੈਟ੍ਰਿਕਸ(c);
        }ਨਹੀਂਤਾਂ{
            dprintf(("I=%d from basic\n",I));
            ਪੂਰਨ iterator_ਵਿੱਥ=ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[I]-nsize;
            c-=old_c(0,I)*b.ਪੰਕਤੀ(iterator_ਵਿੱਥ).ਖਾਨਾ_Range_2(0,b.ਖਾਨੇ_ਗਿਣਤੀ-1);
            v+=old_c(0,I)*b(iterator_ਵਿੱਥ,b.ਖਾਨੇ_ਗਿਣਤੀ-1);
            print_ਮੈਟ੍ਰਿਕਸ(c);
        }
    }

    dprintf(("after restore\n"));
    print_simplex_state(c,b,v,N,B);

    N.erase(N.begin());
    ਲਈ (std::vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>::iterator it = ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ.begin()+1 ; it != ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ.end(); ++it){
        --(*it);
    }
    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ inner_simplex(Mat_<ਦੁੱਗਣਾ>& c, Mat_<ਦੁੱਗਣਾ>& b,ਦੁੱਗਣਾ& v,vector<ਪੂਰਨ>& N,vector<ਪੂਰਨ>& B,vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>& ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ){

    ਲਈ(;;){
        MatIterator_<ਦੁੱਗਣਾ> pos_ptr;
        ਪੂਰਨ e=-1,pos_ctr=0,min_var=ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
        ਤਾਰਕਿਕ all_nonzero=ਸੱਚ;
        ਲਈ(pos_ptr=c.begin();pos_ptr!=c.end();pos_ptr++,pos_ctr++){
            ਜੇ(*pos_ptr==0){
                all_nonzero=ਝੂਠ;
            }
            ਜੇ(*pos_ptr>0){
                ਜੇ(N[pos_ctr]<min_var){
                    e=pos_ctr;
                    min_var=N[pos_ctr];
                }
            }
        }
        ਜੇ(e==-1){
            dprintf(("hello from e==-1\n"));
            print_ਮੈਟ੍ਰਿਕਸ(c);
            ਜੇ(all_nonzero==ਸੱਚ){
                ਵਾਪਸ SOLVELP_SINGLE;
            }ਨਹੀਂਤਾਂ{
                ਵਾਪਸ SOLVELP_MULTI;
            }
        }

        ਪੂਰਨ l=-1;
        min_var=ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
        ਦੁੱਗਣਾ min=DBL_MAX;
        ਪੂਰਨ ਪੰਕਤੀ_it=0;
        MatIterator_<ਦੁੱਗਣਾ> min_ਪੰਕਤੀ_ptr=b.begin();
        ਲਈ(MatIterator_<ਦੁੱਗਣਾ> it=b.begin();it!=b.end();it+=b.ਖਾਨੇ_ਗਿਣਤੀ,ਪੰਕਤੀ_it++){
            ਦੁੱਗਣਾ myite=0;
            //check constraints, select the tightest one, reinforcing Bland's rule
            ਜੇ((myite=it[e])>0){
                ਦੁੱਗਣਾ val=it[b.ਖਾਨੇ_ਗਿਣਤੀ-1]/myite;
                ਜੇ(val<min || (val==min && B[ਪੰਕਤੀ_it]<min_var)){
                    min_var=B[ਪੰਕਤੀ_it];
                    min_ਪੰਕਤੀ_ptr=it;
                    min=val;
                    l=ਪੰਕਤੀ_it;
                }
            }
        }
        ਜੇ(l==-1){
            ਵਾਪਸ SOLVELP_UNBOUNDED;
        }
        dprintf(("the tightest constraint is in row %d with %g\n",l,min));

        ਧੁਰਾ(c,b,v,N,B,l,e,ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ);

        dprintf(("objective, v=%g\n",v));
        print_ਮੈਟ੍ਰਿਕਸ(c);
        dprintf(("constraints\n"));
        print_ਮੈਟ੍ਰਿਕਸ(b);
        dprintf(("non-basic: "));
        print_ਮੈਟ੍ਰਿਕਸ(Mat(N));
        dprintf(("basic: "));
        print_ਮੈਟ੍ਰਿਕਸ(Mat(B));
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਧੁਰਾ(Mat_<ਦੁੱਗਣਾ>& c,Mat_<ਦੁੱਗਣਾ>& b,ਦੁੱਗਣਾ& v,vector<ਪੂਰਨ>& N,vector<ਪੂਰਨ>& B,
        ਪੂਰਨ leaving_ਸੂਚਕਾਂਕ,ਪੂਰਨ entering_ਸੂਚਕਾਂਕ,vector<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>& ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ){
    ਦੁੱਗਣਾ Coef=b(leaving_ਸੂਚਕਾਂਕ,entering_ਸੂਚਕਾਂਕ);
    ਲਈ(ਪੂਰਨ i=0;i<b.ਖਾਨੇ_ਗਿਣਤੀ;i++){
        ਜੇ(i==entering_ਸੂਚਕਾਂਕ){
            b(leaving_ਸੂਚਕਾਂਕ,i)=1/Coef;
        }ਨਹੀਂਤਾਂ{
            b(leaving_ਸੂਚਕਾਂਕ,i)/=Coef;
        }
    }

    ਲਈ(ਪੂਰਨ i=0;i<b.ਪੰਕਤੀਆਂ;i++){
        ਜੇ(i!=leaving_ਸੂਚਕਾਂਕ){
            ਦੁੱਗਣਾ coef=b(i,entering_ਸੂਚਕਾਂਕ);
            ਲਈ(ਪੂਰਨ j=0;j<b.ਖਾਨੇ_ਗਿਣਤੀ;j++){
                ਜੇ(j==entering_ਸੂਚਕਾਂਕ){
                    b(i,j)=-coef*b(leaving_ਸੂਚਕਾਂਕ,j);
                }ਨਹੀਂਤਾਂ{
                    b(i,j)-=(coef*b(leaving_ਸੂਚਕਾਂਕ,j));
                }
            }
        }
    }

    //objective function
    Coef=c(0,entering_ਸੂਚਕਾਂਕ);
    ਲਈ(ਪੂਰਨ i=0;i<(b.ਖਾਨੇ_ਗਿਣਤੀ-1);i++){
        ਜੇ(i==entering_ਸੂਚਕਾਂਕ){
            c(0,i)=-Coef*b(leaving_ਸੂਚਕਾਂਕ,i);
        }ਨਹੀਂਤਾਂ{
            c(0,i)-=Coef*b(leaving_ਸੂਚਕਾਂਕ,i);
        }
    }
    dprintf(("v was %g\n",v));
    v+=Coef*b(leaving_ਸੂਚਕਾਂਕ,b.ਖਾਨੇ_ਗਿਣਤੀ-1);

    ਅਦਲਾ_ਬਦਲੀ(ਪੂਰਨ,N[entering_ਸੂਚਕਾਂਕ],B[leaving_ਸੂਚਕਾਂਕ]);
    ਅਦਲਾ_ਬਦਲੀ(ਪੂਰਨ,ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[N[entering_ਸੂਚਕਾਂਕ]],ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਪੰਕਤੀ[B[leaving_ਸੂਚਕਾਂਕ]]);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਅਦਲਾ_ਬਦਲੀ_columns(Mat_<ਦੁੱਗਣਾ>& A,ਪੂਰਨ col1,ਪੂਰਨ col2){
    ਲਈ(ਪੂਰਨ i=0;i<A.ਪੰਕਤੀਆਂ;i++){
        ਦੁੱਗਣਾ tmp=A(i,col1);
        A(i,col1)=A(i,col2);
        A(i,col2)=tmp;
    }
}
}
