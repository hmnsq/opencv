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

PCA::PCA(داخلہ_Array انکڑے, داخلہ_Array اوسط, صحيح flags, صحيح maxComponents)
{
    عامل()(انکڑے, اوسط, flags, maxComponents);
}

PCA::PCA(داخلہ_Array انکڑے, داخلہ_Array اوسط, صحيح flags, مزدوج retainedVariance)
{
    عامل()(انکڑے, اوسط, flags, retainedVariance);
}

PCA& PCA::عامل()(داخلہ_Array انکڑے_3, داخلہ_Array __mean, صحيح flags, صحيح maxComponents)
{
    Mat انکڑے = انکڑے_3.getMat(), اوسط = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, وچ_گنتی;
    ناپ اوسط_sz;

    CV_Assert( انکڑے.channels() == 1 );
    إذا( flags & PCA::انکڑے_AS_خانہ )
    {
        len = انکڑے.صفاں;
        وچ_گنتی = انکڑے.خانے_گنتی;
        covar_flags |= cv::COVAR_خانے_گنتی;
        اوسط_sz = ناپ(1, len);
    }
    وإلا
    {
        len = انکڑے.خانے_گنتی;
        وچ_گنتی = انکڑے.صفاں;
        covar_flags |= cv::COVAR_صفاں;
        اوسط_sz = ناپ(len, 1);
    }

    صحيح گنتی_2 = std::min(len, وچ_گنتی), باہرلا_گنتی = گنتی_2;
    إذا( maxComponents > 0 )
        باہرلا_گنتی = std::min(گنتی_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= وچ_گنتی )
        covar_flags |= cv::COVAR_عام;

    صحيح ctype = std::max(CV_32F, انکڑے.ڈونگھائی());
    اوسط_2.create( اوسط_sz, ctype );

    Mat covar( گنتی_2, گنتی_2, ctype );

    إذا( !اوسط.empty() )
    {
        CV_Assert( اوسط.size() == اوسط_sz );
        اوسط.convert_منزل(اوسط_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_میٹرکس( انکڑے, covar, اوسط_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_عام) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_انکڑے, tmp_اوسط = repeat(اوسط_2, انکڑے.صفاں/اوسط_2.صفاں, انکڑے.خانے_گنتی/اوسط_2.خانے_گنتی);
        إذا( انکڑے.type() != ctype || tmp_اوسط.انکڑے == اوسط_2.انکڑے )
        {
            انکڑے.convert_منزل( tmp_انکڑے, ctype );
            گھٹاؤ( tmp_انکڑے, tmp_اوسط, tmp_انکڑے );
        }
        وإلا
        {
            گھٹاؤ( انکڑے, tmp_اوسط, tmp_اوسط );
            tmp_انکڑے = tmp_اوسط;
        }

        Mat evects1(گنتی_2, len, ctype);
        gemm( eigenvectors, tmp_انکڑے, 1, Mat(), 0, evects1,
            (flags & PCA::انکڑے_AS_خانہ) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        صحيح i;
        لكل( i = 0; i < باہرلا_گنتی; i++ )
        {
            Mat vec = eigenvectors.صف(i);
            normalize(vec, vec);
        }
    }

    إذا( گنتی_2 > باہرلا_گنتی )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.صف_Range_2(0,باہرلا_گنتی).clone();
        eigenvectors = eigenvectors.صف_Range_2(0,باہرلا_گنتی).clone();
    }
    أرجع *هذا;
}

فراغ PCA::اكتب(FileStorage& fs ) ثابت
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << اوسط_2;
}

فراغ PCA::اقرأ(ثابت File_گرہ& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::اقرأ(fn["vectors"], eigenvectors);
    cv::اقرأ(fn["values"], eigenvalues);
    cv::اقرأ(fn["mean"], اوسط_2);
}

سانچا <قسم_ناں T>
صحيح computeCumulativeEnergy(ثابت Mat& eigenvalues, مزدوج retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == انکڑے_Type_2<T>::type );

    Mat g(eigenvalues.size(), انکڑے_Type_2<T>::type);

    لكل(صحيح ig = 0; ig < g.صفاں; ig++)
    {
        g.at<T>(ig, 0) = 0;
        لكل(صحيح im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    صحيح L;

    لكل(L = 0; L < eigenvalues.صفاں; L++)
    {
        مزدوج energy = g.at<T>(L, 0) / g.at<T>(g.صفاں - 1, 0);
        إذا(energy > retainedVariance)
            اكسر;
    }

    L = std::max(2, L);

    أرجع L;
}

PCA& PCA::عامل()(داخلہ_Array انکڑے_3, داخلہ_Array __mean, صحيح flags, مزدوج retainedVariance)
{
    Mat انکڑے = انکڑے_3.getMat(), اوسط = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, وچ_گنتی;
    ناپ اوسط_sz;

    CV_Assert( انکڑے.channels() == 1 );
    إذا( flags & PCA::انکڑے_AS_خانہ )
    {
        len = انکڑے.صفاں;
        وچ_گنتی = انکڑے.خانے_گنتی;
        covar_flags |= cv::COVAR_خانے_گنتی;
        اوسط_sz = ناپ(1, len);
    }
    وإلا
    {
        len = انکڑے.خانے_گنتی;
        وچ_گنتی = انکڑے.صفاں;
        covar_flags |= cv::COVAR_صفاں;
        اوسط_sz = ناپ(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    صحيح گنتی_2 = std::min(len, وچ_گنتی);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= وچ_گنتی )
        covar_flags |= cv::COVAR_عام;

    صحيح ctype = std::max(CV_32F, انکڑے.ڈونگھائی());
    اوسط_2.create( اوسط_sz, ctype );

    Mat covar( گنتی_2, گنتی_2, ctype );

    إذا( !اوسط.empty() )
    {
        CV_Assert( اوسط.size() == اوسط_sz );
        اوسط.convert_منزل(اوسط_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_میٹرکس( انکڑے, covar, اوسط_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_عام) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_انکڑے, tmp_اوسط = repeat(اوسط_2, انکڑے.صفاں/اوسط_2.صفاں, انکڑے.خانے_گنتی/اوسط_2.خانے_گنتی);
        إذا( انکڑے.type() != ctype || tmp_اوسط.انکڑے == اوسط_2.انکڑے )
        {
            انکڑے.convert_منزل( tmp_انکڑے, ctype );
            گھٹاؤ( tmp_انکڑے, tmp_اوسط, tmp_انکڑے );
        }
        وإلا
        {
            گھٹاؤ( انکڑے, tmp_اوسط, tmp_اوسط );
            tmp_انکڑے = tmp_اوسط;
        }

        Mat evects1(گنتی_2, len, ctype);
        gemm( eigenvectors, tmp_انکڑے, 1, Mat(), 0, evects1,
            (flags & PCA::انکڑے_AS_خانہ) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        صحيح i;
        لكل( i = 0; i < eigenvectors.صفاں; i++ )
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

فراغ PCA::project(داخلہ_Array انکڑے_3, OutputArray نتیجہ) ثابت
{
    Mat انکڑے = انکڑے_3.getMat();
    CV_Assert( !اوسط_2.empty() && !eigenvectors.empty() &&
        ((اوسط_2.صفاں == 1 && اوسط_2.خانے_گنتی == انکڑے.خانے_گنتی) || (اوسط_2.خانے_گنتی == 1 && اوسط_2.صفاں == انکڑے.صفاں)));
    Mat tmp_انکڑے, tmp_اوسط = repeat(اوسط_2, انکڑے.صفاں/اوسط_2.صفاں, انکڑے.خانے_گنتی/اوسط_2.خانے_گنتی);
    صحيح ctype = اوسط_2.type();
    إذا( انکڑے.type() != ctype || tmp_اوسط.انکڑے == اوسط_2.انکڑے )
    {
        انکڑے.convert_منزل( tmp_انکڑے, ctype );
        گھٹاؤ( tmp_انکڑے, tmp_اوسط, tmp_انکڑے );
    }
    وإلا
    {
        گھٹاؤ( انکڑے, tmp_اوسط, tmp_اوسط );
        tmp_انکڑے = tmp_اوسط;
    }
    إذا( اوسط_2.صفاں == 1 )
        gemm( tmp_انکڑے, eigenvectors, 1, Mat(), 0, نتیجہ, GEMM_2_T );
    وإلا
        gemm( eigenvectors, tmp_انکڑے, 1, Mat(), 0, نتیجہ, 0 );
}

Mat PCA::project(داخلہ_Array انکڑے) ثابت
{
    Mat نتیجہ;
    project(انکڑے, نتیجہ);
    أرجع نتیجہ;
}

فراغ PCA::پٹھ_Project(داخلہ_Array انکڑے_3, OutputArray نتیجہ) ثابت
{
    Mat انکڑے = انکڑے_3.getMat();
    CV_Assert( !اوسط_2.empty() && !eigenvectors.empty() &&
        ((اوسط_2.صفاں == 1 && eigenvectors.صفاں == انکڑے.خانے_گنتی) ||
         (اوسط_2.خانے_گنتی == 1 && eigenvectors.صفاں == انکڑے.صفاں)));

    Mat tmp_انکڑے, tmp_اوسط;
    انکڑے.convert_منزل(tmp_انکڑے, اوسط_2.type());
    إذا( اوسط_2.صفاں == 1 )
    {
        tmp_اوسط = repeat(اوسط_2, انکڑے.صفاں, 1);
        gemm( tmp_انکڑے, eigenvectors, 1, tmp_اوسط, 1, نتیجہ, 0 );
    }
    وإلا
    {
        tmp_اوسط = repeat(اوسط_2, 1, انکڑے.خانے_گنتی);
        gemm( eigenvectors, tmp_انکڑے, 1, tmp_اوسط, 1, نتیجہ, GEMM_1_T );
    }
}

Mat PCA::پٹھ_Project(داخلہ_Array انکڑے) ثابت
{
    Mat نتیجہ;
    پٹھ_Project(انکڑے, نتیجہ);
    أرجع نتیجہ;
}

}

فراغ cv::PCACompute(داخلہ_Array انکڑے, داخلہ_Output_Array اوسط_2,
                    OutputArray eigenvectors, صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(انکڑے, اوسط_2, 0, maxComponents);
    pca.اوسط_2.copy_منزل(اوسط_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
}

فراغ cv::PCACompute(داخلہ_Array انکڑے, داخلہ_Output_Array اوسط_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(انکڑے, اوسط_2, 0, maxComponents);
    pca.اوسط_2.copy_منزل(اوسط_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
    pca.eigenvalues.copy_منزل(eigenvalues);
}

فراغ cv::PCACompute(داخلہ_Array انکڑے, داخلہ_Output_Array اوسط_2,
                    OutputArray eigenvectors, مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(انکڑے, اوسط_2, 0, retainedVariance);
    pca.اوسط_2.copy_منزل(اوسط_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
}

فراغ cv::PCACompute(داخلہ_Array انکڑے, داخلہ_Output_Array اوسط_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(انکڑے, اوسط_2, 0, retainedVariance);
    pca.اوسط_2.copy_منزل(اوسط_2);
    pca.eigenvectors.copy_منزل(eigenvectors);
    pca.eigenvalues.copy_منزل(eigenvalues);
}

فراغ cv::PCAProject(داخلہ_Array انکڑے, داخلہ_Array اوسط_2,
                    داخلہ_Array eigenvectors, OutputArray نتیجہ)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.اوسط_2 = اوسط_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(انکڑے, نتیجہ);
}

فراغ cv::PCABackProject(داخلہ_Array انکڑے, داخلہ_Array اوسط_2,
                    داخلہ_Array eigenvectors, OutputArray نتیجہ)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.اوسط_2 = اوسط_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.پٹھ_Project(انکڑے, نتیجہ);
}
