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

PCA::PCA(داخلا_Array معلومات, داخلا_Array سراسري, صحيح flags, صحيح maxComponents)
{
    عامل()(معلومات, سراسري, flags, maxComponents);
}

PCA::PCA(داخلا_Array معلومات, داخلا_Array سراسري, صحيح flags, مزدوج retainedVariance)
{
    عامل()(معلومات, سراسري, flags, retainedVariance);
}

PCA& PCA::عامل()(داخلا_Array معلومات_3, داخلا_Array __mean, صحيح flags, صحيح maxComponents)
{
    Mat معلومات = معلومات_3.getMat(), سراسري = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, اندريون_ڳڻپ;
    سائيز سراسري_sz;

    CV_Assert( معلومات.channels() == 1 );
    إذا( flags & PCA::معلومات_AS_کانو )
    {
        len = معلومات.صفون;
        اندريون_ڳڻپ = معلومات.کانن_ڳڻپ;
        covar_flags |= cv::COVAR_کانن_ڳڻپ;
        سراسري_sz = سائيز(1, len);
    }
    وإلا
    {
        len = معلومات.کانن_ڳڻپ;
        اندريون_ڳڻپ = معلومات.صفون;
        covar_flags |= cv::COVAR_صفون;
        سراسري_sz = سائيز(len, 1);
    }

    صحيح ڳڻپ_2 = std::min(len, اندريون_ڳڻپ), ٻاهريون_ڳڻپ = ڳڻپ_2;
    إذا( maxComponents > 0 )
        ٻاهريون_ڳڻپ = std::min(ڳڻپ_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= اندريون_ڳڻپ )
        covar_flags |= cv::COVAR_عام;

    صحيح ctype = std::max(CV_32F, معلومات.اونهائي());
    سراسري_2.create( سراسري_sz, ctype );

    Mat covar( ڳڻپ_2, ڳڻپ_2, ctype );

    إذا( !سراسري.empty() )
    {
        CV_Assert( سراسري.size() == سراسري_sz );
        سراسري.convert_منزل(سراسري_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ميٽرڪس( معلومات, covar, سراسري_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_عام) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_معلومات, tmp_سراسري = repeat(سراسري_2, معلومات.صفون/سراسري_2.صفون, معلومات.کانن_ڳڻپ/سراسري_2.کانن_ڳڻپ);
        إذا( معلومات.type() != ctype || tmp_سراسري.معلومات == سراسري_2.معلومات )
        {
            معلومات.convert_منزل( tmp_معلومات, ctype );
            ڪٽيو( tmp_معلومات, tmp_سراسري, tmp_معلومات );
        }
        وإلا
        {
            ڪٽيو( معلومات, tmp_سراسري, tmp_سراسري );
            tmp_معلومات = tmp_سراسري;
        }

        Mat evects1(ڳڻپ_2, len, ctype);
        gemm( eigenvectors, tmp_معلومات, 1, Mat(), 0, evects1,
            (flags & PCA::معلومات_AS_کانو) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        صحيح i;
        لكل( i = 0; i < ٻاهريون_ڳڻپ; i++ )
        {
            Mat vec = eigenvectors.صف(i);
            normalize(vec, vec);
        }
    }

    إذا( ڳڻپ_2 > ٻاهريون_ڳڻپ )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.صف_Range_2(0,ٻاهريون_ڳڻپ).clone();
        eigenvectors = eigenvectors.صف_Range_2(0,ٻاهريون_ڳڻپ).clone();
    }
    أرجع *هذا;
}

فراغ PCA::اكتب(FileStorage& fs ) ثابت
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << سراسري_2;
}

فراغ PCA::اقرأ(ثابت File_ڳنڍ& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::اقرأ(fn["vectors"], eigenvectors);
    cv::اقرأ(fn["values"], eigenvalues);
    cv::اقرأ(fn["mean"], سراسري_2);
}

سانچو <قسم_نالو T>
صحيح computeCumulativeEnergy(ثابت Mat& eigenvalues, مزدوج retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == معلومات_Type_2<T>::type );

    Mat g(eigenvalues.size(), معلومات_Type_2<T>::type);

    لكل(صحيح ig = 0; ig < g.صفون; ig++)
    {
        g.at<T>(ig, 0) = 0;
        لكل(صحيح im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    صحيح L;

    لكل(L = 0; L < eigenvalues.صفون; L++)
    {
        مزدوج energy = g.at<T>(L, 0) / g.at<T>(g.صفون - 1, 0);
        إذا(energy > retainedVariance)
            اكسر;
    }

    L = std::max(2, L);

    أرجع L;
}

PCA& PCA::عامل()(داخلا_Array معلومات_3, داخلا_Array __mean, صحيح flags, مزدوج retainedVariance)
{
    Mat معلومات = معلومات_3.getMat(), سراسري = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, اندريون_ڳڻپ;
    سائيز سراسري_sz;

    CV_Assert( معلومات.channels() == 1 );
    إذا( flags & PCA::معلومات_AS_کانو )
    {
        len = معلومات.صفون;
        اندريون_ڳڻپ = معلومات.کانن_ڳڻپ;
        covar_flags |= cv::COVAR_کانن_ڳڻپ;
        سراسري_sz = سائيز(1, len);
    }
    وإلا
    {
        len = معلومات.کانن_ڳڻپ;
        اندريون_ڳڻپ = معلومات.صفون;
        covar_flags |= cv::COVAR_صفون;
        سراسري_sz = سائيز(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    صحيح ڳڻپ_2 = std::min(len, اندريون_ڳڻپ);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= اندريون_ڳڻپ )
        covar_flags |= cv::COVAR_عام;

    صحيح ctype = std::max(CV_32F, معلومات.اونهائي());
    سراسري_2.create( سراسري_sz, ctype );

    Mat covar( ڳڻپ_2, ڳڻپ_2, ctype );

    إذا( !سراسري.empty() )
    {
        CV_Assert( سراسري.size() == سراسري_sz );
        سراسري.convert_منزل(سراسري_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ميٽرڪس( معلومات, covar, سراسري_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_عام) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_معلومات, tmp_سراسري = repeat(سراسري_2, معلومات.صفون/سراسري_2.صفون, معلومات.کانن_ڳڻپ/سراسري_2.کانن_ڳڻپ);
        إذا( معلومات.type() != ctype || tmp_سراسري.معلومات == سراسري_2.معلومات )
        {
            معلومات.convert_منزل( tmp_معلومات, ctype );
            ڪٽيو( tmp_معلومات, tmp_سراسري, tmp_معلومات );
        }
        وإلا
        {
            ڪٽيو( معلومات, tmp_سراسري, tmp_سراسري );
            tmp_معلومات = tmp_سراسري;
        }

        Mat evects1(ڳڻپ_2, len, ctype);
        gemm( eigenvectors, tmp_معلومات, 1, Mat(), 0, evects1,
            (flags & PCA::معلومات_AS_کانو) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        صحيح i;
        لكل( i = 0; i < eigenvectors.صفون; i++ )
        {
            Mat vec = eigenvectors.صف(i);
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
    eigenvalues = eigenvalues.صف_Range_2(0,L).clone();
    eigenvectors = eigenvectors.صف_Range_2(0,L).clone();

    أرجع *هذا;
}

فراغ PCA::project(داخلا_Array معلومات_3, OutputArray نتيجو) ثابت
{
    Mat معلومات = معلومات_3.getMat();
    CV_Assert( !سراسري_2.empty() && !eigenvectors.empty() &&
        ((سراسري_2.صفون == 1 && سراسري_2.کانن_ڳڻپ == معلومات.کانن_ڳڻپ) || (سراسري_2.کانن_ڳڻپ == 1 && سراسري_2.صفون == معلومات.صفون)));
    Mat tmp_معلومات, tmp_سراسري = repeat(سراسري_2, معلومات.صفون/سراسري_2.صفون, معلومات.کانن_ڳڻپ/سراسري_2.کانن_ڳڻپ);
    صحيح ctype = سراسري_2.type();
    إذا( معلومات.type() != ctype || tmp_سراسري.معلومات == سراسري_2.معلومات )
    {
        معلومات.convert_منزل( tmp_معلومات, ctype );
        ڪٽيو( tmp_معلومات, tmp_سراسري, tmp_معلومات );
    }
    وإلا
    {
        ڪٽيو( معلومات, tmp_سراسري, tmp_سراسري );
        tmp_معلومات = tmp_سراسري;
    }
    إذا( سراسري_2.صفون == 1 )
        gemm( tmp_معلومات, eigenvectors, 1, Mat(), 0, نتيجو, GEMM_2_T );
    وإلا
        gemm( eigenvectors, tmp_معلومات, 1, Mat(), 0, نتيجو, 0 );
}

Mat PCA::project(داخلا_Array معلومات) ثابت
{
    Mat نتيجو;
    project(معلومات, نتيجو);
    أرجع نتيجو;
}

فراغ PCA::پٺ_Project(داخلا_Array معلومات_3, OutputArray نتيجو) ثابت
{
    Mat معلومات = معلومات_3.getMat();
    CV_Assert( !سراسري_2.empty() && !eigenvectors.empty() &&
        ((سراسري_2.صفون == 1 && eigenvectors.صفون == معلومات.کانن_ڳڻپ) ||
         (سراسري_2.کانن_ڳڻپ == 1 && eigenvectors.صفون == معلومات.صفون)));

    Mat tmp_معلومات, tmp_سراسري;
    معلومات.convert_منزل(tmp_معلومات, سراسري_2.type());
    إذا( سراسري_2.صفون == 1 )
    {
        tmp_سراسري = repeat(سراسري_2, معلومات.صفون, 1);
        gemm( tmp_معلومات, eigenvectors, 1, tmp_سراسري, 1, نتيجو, 0 );
    }
    وإلا
    {
        tmp_سراسري = repeat(سراسري_2, 1, معلومات.کانن_ڳڻپ);
        gemm( eigenvectors, tmp_معلومات, 1, tmp_سراسري, 1, نتيجو, GEMM_1_T );
    }
}

Mat PCA::پٺ_Project(داخلا_Array معلومات) ثابت
{
    Mat نتيجو;
    پٺ_Project(معلومات, نتيجو);
    أرجع نتيجو;
}

}

فراغ cv::PCACompute(داخلا_Array معلومات, داخلا_Output_Array سراسري_2,
                    OutputArray eigenvectors, صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(معلومات, سراسري_2, 0, maxComponents);
    pca.سراسري_2.copy_منزل(سراسري_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
}

فراغ cv::PCACompute(داخلا_Array معلومات, داخلا_Output_Array سراسري_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(معلومات, سراسري_2, 0, maxComponents);
    pca.سراسري_2.copy_منزل(سراسري_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
    pca.eigenvalues.copy_منزل(eigenvalues);
}

فراغ cv::PCACompute(داخلا_Array معلومات, داخلا_Output_Array سراسري_2,
                    OutputArray eigenvectors, مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(معلومات, سراسري_2, 0, retainedVariance);
    pca.سراسري_2.copy_منزل(سراسري_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
}

فراغ cv::PCACompute(داخلا_Array معلومات, داخلا_Output_Array سراسري_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(معلومات, سراسري_2, 0, retainedVariance);
    pca.سراسري_2.copy_منزل(سراسري_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
    pca.eigenvalues.copy_منزل(eigenvalues);
}

فراغ cv::PCAProject(داخلا_Array معلومات, داخلا_Array سراسري_2,
                    داخلا_Array eigenvectors, OutputArray نتيجو)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.سراسري_2 = سراسري_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(معلومات, نتيجو);
}

فراغ cv::PCABackProject(داخلا_Array معلومات, داخلا_Array سراسري_2,
                    داخلا_Array eigenvectors, OutputArray نتيجو)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.سراسري_2 = سراسري_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.پٺ_Project(معلومات, نتيجو);
}
