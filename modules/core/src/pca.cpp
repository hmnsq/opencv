/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
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
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ضمن "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

نطاق cv
{

PCA::PCA() {}

PCA::PCA(ننوت_Array معلومات, ننوت_Array منځنۍ, صحيح flags, صحيح maxComponents)
{
    چلوونکی()(معلومات, منځنۍ, flags, maxComponents);
}

PCA::PCA(ننوت_Array معلومات, ننوت_Array منځنۍ, صحيح flags, مزدوج retainedVariance)
{
    چلوونکی()(معلومات, منځنۍ, flags, retainedVariance);
}

PCA& PCA::چلوونکی()(ننوت_Array معلومات_3, ننوت_Array __mean, صحيح flags, صحيح maxComponents)
{
    Mat معلومات = معلومات_3.getMat(), منځنۍ = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, دننه_شمېر;
    اندازه منځنۍ_sz;

    CV_Assert( معلومات.channels() == 1 );
    إذا( flags & PCA::معلومات_AS_ستون )
    {
        len = معلومات.لیکې;
        دننه_شمېر = معلومات.ستون_شمېر;
        covar_flags |= cv::COVAR_ستون_شمېر;
        منځنۍ_sz = اندازه(1, len);
    }
    وإلا
    {
        len = معلومات.ستون_شمېر;
        دننه_شمېر = معلومات.لیکې;
        covar_flags |= cv::COVAR_لیکې;
        منځنۍ_sz = اندازه(len, 1);
    }

    صحيح شمېر_2 = std::min(len, دننه_شمېر), بهر_شمېر = شمېر_2;
    إذا( maxComponents > 0 )
        بهر_شمېر = std::min(شمېر_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= دننه_شمېر )
        covar_flags |= cv::COVAR_نورمال;

    صحيح ctype = std::max(CV_32F, معلومات.ژوروالی());
    منځنۍ_2.create( منځنۍ_sz, ctype );

    Mat covar( شمېر_2, شمېر_2, ctype );

    إذا( !منځنۍ.empty() )
    {
        CV_Assert( منځنۍ.size() == منځنۍ_sz );
        منځنۍ.convert_منزل(منځنۍ_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ماتریکس( معلومات, covar, منځنۍ_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_نورمال) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_معلومات, tmp_منځنۍ = repeat(منځنۍ_2, معلومات.لیکې/منځنۍ_2.لیکې, معلومات.ستون_شمېر/منځنۍ_2.ستون_شمېر);
        إذا( معلومات.type() != ctype || tmp_منځنۍ.معلومات == منځنۍ_2.معلومات )
        {
            معلومات.convert_منزل( tmp_معلومات, ctype );
            کمول( tmp_معلومات, tmp_منځنۍ, tmp_معلومات );
        }
        وإلا
        {
            کمول( معلومات, tmp_منځنۍ, tmp_منځنۍ );
            tmp_معلومات = tmp_منځنۍ;
        }

        Mat evects1(شمېر_2, len, ctype);
        gemm( eigenvectors, tmp_معلومات, 1, Mat(), 0, evects1,
            (flags & PCA::معلومات_AS_ستون) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        صحيح i;
        لكل( i = 0; i < بهر_شمېر; i++ )
        {
            Mat vec = eigenvectors.لیکه(i);
            normalize(vec, vec);
        }
    }

    إذا( شمېر_2 > بهر_شمېر )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.لیکه_Range_2(0,بهر_شمېر).clone();
        eigenvectors = eigenvectors.لیکه_Range_2(0,بهر_شمېر).clone();
    }
    أرجع *هذا;
}

فراغ PCA::اكتب(FileStorage& fs ) ثابت
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << منځنۍ_2;
}

فراغ PCA::اقرأ(ثابت File_غوټه& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::اقرأ(fn["vectors"], eigenvectors);
    cv::اقرأ(fn["values"], eigenvalues);
    cv::اقرأ(fn["mean"], منځنۍ_2);
}

کينډۍ <ډول_نوم T>
صحيح computeCumulativeEnergy(ثابت Mat& eigenvalues, مزدوج retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == معلومات_Type_2<T>::type );

    Mat g(eigenvalues.size(), معلومات_Type_2<T>::type);

    لكل(صحيح ig = 0; ig < g.لیکې; ig++)
    {
        g.at<T>(ig, 0) = 0;
        لكل(صحيح im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    صحيح L;

    لكل(L = 0; L < eigenvalues.لیکې; L++)
    {
        مزدوج energy = g.at<T>(L, 0) / g.at<T>(g.لیکې - 1, 0);
        إذا(energy > retainedVariance)
            اكسر;
    }

    L = std::max(2, L);

    أرجع L;
}

PCA& PCA::چلوونکی()(ننوت_Array معلومات_3, ننوت_Array __mean, صحيح flags, مزدوج retainedVariance)
{
    Mat معلومات = معلومات_3.getMat(), منځنۍ = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, دننه_شمېر;
    اندازه منځنۍ_sz;

    CV_Assert( معلومات.channels() == 1 );
    إذا( flags & PCA::معلومات_AS_ستون )
    {
        len = معلومات.لیکې;
        دننه_شمېر = معلومات.ستون_شمېر;
        covar_flags |= cv::COVAR_ستون_شمېر;
        منځنۍ_sz = اندازه(1, len);
    }
    وإلا
    {
        len = معلومات.ستون_شمېر;
        دننه_شمېر = معلومات.لیکې;
        covar_flags |= cv::COVAR_لیکې;
        منځنۍ_sz = اندازه(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    صحيح شمېر_2 = std::min(len, دننه_شمېر);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= دننه_شمېر )
        covar_flags |= cv::COVAR_نورمال;

    صحيح ctype = std::max(CV_32F, معلومات.ژوروالی());
    منځنۍ_2.create( منځنۍ_sz, ctype );

    Mat covar( شمېر_2, شمېر_2, ctype );

    إذا( !منځنۍ.empty() )
    {
        CV_Assert( منځنۍ.size() == منځنۍ_sz );
        منځنۍ.convert_منزل(منځنۍ_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ماتریکس( معلومات, covar, منځنۍ_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_نورمال) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_معلومات, tmp_منځنۍ = repeat(منځنۍ_2, معلومات.لیکې/منځنۍ_2.لیکې, معلومات.ستون_شمېر/منځنۍ_2.ستون_شمېر);
        إذا( معلومات.type() != ctype || tmp_منځنۍ.معلومات == منځنۍ_2.معلومات )
        {
            معلومات.convert_منزل( tmp_معلومات, ctype );
            کمول( tmp_معلومات, tmp_منځنۍ, tmp_معلومات );
        }
        وإلا
        {
            کمول( معلومات, tmp_منځنۍ, tmp_منځنۍ );
            tmp_معلومات = tmp_منځنۍ;
        }

        Mat evects1(شمېر_2, len, ctype);
        gemm( eigenvectors, tmp_معلومات, 1, Mat(), 0, evects1,
            (flags & PCA::معلومات_AS_ستون) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        صحيح i;
        لكل( i = 0; i < eigenvectors.لیکې; i++ )
        {
            Mat vec = eigenvectors.لیکه(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    صحيح L;
    إذا (ctype == CV_32F)
        L = computeCumulativeEnergy<عائم>(eigenvalues, retainedVariance);
    وإلا
        L = computeCumulativeEnergy<مزدوج>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.لیکه_Range_2(0,L).clone();
    eigenvectors = eigenvectors.لیکه_Range_2(0,L).clone();

    أرجع *هذا;
}

فراغ PCA::project(ننوت_Array معلومات_3, OutputArray پایله) ثابت
{
    Mat معلومات = معلومات_3.getMat();
    CV_Assert( !منځنۍ_2.empty() && !eigenvectors.empty() &&
        ((منځنۍ_2.لیکې == 1 && منځنۍ_2.ستون_شمېر == معلومات.ستون_شمېر) || (منځنۍ_2.ستون_شمېر == 1 && منځنۍ_2.لیکې == معلومات.لیکې)));
    Mat tmp_معلومات, tmp_منځنۍ = repeat(منځنۍ_2, معلومات.لیکې/منځنۍ_2.لیکې, معلومات.ستون_شمېر/منځنۍ_2.ستون_شمېر);
    صحيح ctype = منځنۍ_2.type();
    إذا( معلومات.type() != ctype || tmp_منځنۍ.معلومات == منځنۍ_2.معلومات )
    {
        معلومات.convert_منزل( tmp_معلومات, ctype );
        کمول( tmp_معلومات, tmp_منځنۍ, tmp_معلومات );
    }
    وإلا
    {
        کمول( معلومات, tmp_منځنۍ, tmp_منځنۍ );
        tmp_معلومات = tmp_منځنۍ;
    }
    إذا( منځنۍ_2.لیکې == 1 )
        gemm( tmp_معلومات, eigenvectors, 1, Mat(), 0, پایله, GEMM_2_T );
    وإلا
        gemm( eigenvectors, tmp_معلومات, 1, Mat(), 0, پایله, 0 );
}

Mat PCA::project(ننوت_Array معلومات) ثابت
{
    Mat پایله;
    project(معلومات, پایله);
    أرجع پایله;
}

فراغ PCA::شا_Project(ننوت_Array معلومات_3, OutputArray پایله) ثابت
{
    Mat معلومات = معلومات_3.getMat();
    CV_Assert( !منځنۍ_2.empty() && !eigenvectors.empty() &&
        ((منځنۍ_2.لیکې == 1 && eigenvectors.لیکې == معلومات.ستون_شمېر) ||
         (منځنۍ_2.ستون_شمېر == 1 && eigenvectors.لیکې == معلومات.لیکې)));

    Mat tmp_معلومات, tmp_منځنۍ;
    معلومات.convert_منزل(tmp_معلومات, منځنۍ_2.type());
    إذا( منځنۍ_2.لیکې == 1 )
    {
        tmp_منځنۍ = repeat(منځنۍ_2, معلومات.لیکې, 1);
        gemm( tmp_معلومات, eigenvectors, 1, tmp_منځنۍ, 1, پایله, 0 );
    }
    وإلا
    {
        tmp_منځنۍ = repeat(منځنۍ_2, 1, معلومات.ستون_شمېر);
        gemm( eigenvectors, tmp_معلومات, 1, tmp_منځنۍ, 1, پایله, GEMM_1_T );
    }
}

Mat PCA::شا_Project(ننوت_Array معلومات) ثابت
{
    Mat پایله;
    شا_Project(معلومات, پایله);
    أرجع پایله;
}

}

فراغ cv::PCACompute(ننوت_Array معلومات, ننوت_Output_Array منځنۍ_2,
                    OutputArray eigenvectors, صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(معلومات, منځنۍ_2, 0, maxComponents);
    pca.منځنۍ_2.copy_منزل(منځنۍ_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
}

فراغ cv::PCACompute(ننوت_Array معلومات, ننوت_Output_Array منځنۍ_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(معلومات, منځنۍ_2, 0, maxComponents);
    pca.منځنۍ_2.copy_منزل(منځنۍ_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
    pca.eigenvalues.copy_منزل(eigenvalues);
}

فراغ cv::PCACompute(ننوت_Array معلومات, ننوت_Output_Array منځنۍ_2,
                    OutputArray eigenvectors, مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(معلومات, منځنۍ_2, 0, retainedVariance);
    pca.منځنۍ_2.copy_منزل(منځنۍ_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
}

فراغ cv::PCACompute(ننوت_Array معلومات, ننوت_Output_Array منځنۍ_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(معلومات, منځنۍ_2, 0, retainedVariance);
    pca.منځنۍ_2.copy_منزل(منځنۍ_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
    pca.eigenvalues.copy_منزل(eigenvalues);
}

فراغ cv::PCAProject(ننوت_Array معلومات, ننوت_Array منځنۍ_2,
                    ننوت_Array eigenvectors, OutputArray پایله)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.منځنۍ_2 = منځنۍ_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(معلومات, پایله);
}

فراغ cv::PCABackProject(ننوت_Array معلومات, ننوت_Array منځنۍ_2,
                    ننوت_Array eigenvectors, OutputArray پایله)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.منځنۍ_2 = منځنۍ_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.شا_Project(معلومات, پایله);
}
