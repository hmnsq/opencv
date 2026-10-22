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

#অন্তর্ভাব "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

নামাকাশ cv
{

PCA::PCA() {}

PCA::PCA(প্রবেশ_Array তথ্য, প্রবেশ_Array _mean, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা maxComponents)
{
    operator()(তথ্য, _mean, flags, maxComponents);
}

PCA::PCA(প্রবেশ_Array তথ্য, প্রবেশ_Array _mean, পূর্ণসংখ্যা flags, দ্বিগুণ retainedVariance)
{
    operator()(তথ্য, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(প্রবেশ_Array তথ্য_3, প্রবেশ_Array __mean, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা maxComponents)
{
    Mat তথ্য = তথ্য_3.getMat(), _mean = __mean.getMat();
    পূর্ণসংখ্যা covar_flags = cv::COVAR_SCALE;
    পূর্ণসংখ্যা len, in_গুনতি;
    আকার_2 mean_sz;

    CV_Assert( তথ্য.channels() == 1 );
    যদি( flags & PCA::তথ্য_AS_COL )
    {
        len = তথ্য.rows;
        in_গুনতি = তথ্য.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = আকার_2(1, len);
    }
    অন্যথা
    {
        len = তথ্য.cols;
        in_গুনতি = তথ্য.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = আকার_2(len, 1);
    }

    পূর্ণসংখ্যা গুনতি_2 = std::min(len, in_গুনতি), out_গুনতি = গুনতি_2;
    যদি( maxComponents > 0 )
        out_গুনতি = std::min(গুনতি_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    যদি( len <= in_গুনতি )
        covar_flags |= cv::COVAR_NORMAL;

    পূর্ণসংখ্যা ctype = std::max(CV_32F, তথ্য.depth());
    mean.create( mean_sz, ctype );

    Mat covar( গুনতি_2, গুনতি_2, ctype );

    যদি( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( তথ্য, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    যদি( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_তথ্য, tmp_mean = repeat(mean, তথ্য.rows/mean.rows, তথ্য.cols/mean.cols);
        যদি( তথ্য.type() != ctype || tmp_mean.তথ্য == mean.তথ্য )
        {
            তথ্য.convertTo( tmp_তথ্য, ctype );
            subtract( tmp_তথ্য, tmp_mean, tmp_তথ্য );
        }
        অন্যথা
        {
            subtract( তথ্য, tmp_mean, tmp_mean );
            tmp_তথ্য = tmp_mean;
        }

        Mat evects1(গুনতি_2, len, ctype);
        gemm( eigenvectors, tmp_তথ্য, 1, Mat(), 0, evects1,
            (flags & PCA::তথ্য_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        পূর্ণসংখ্যা i;
        জন্য( i = 0; i < out_গুনতি; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    যদি( গুনতি_2 > out_গুনতি )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_গুনতি).clone();
        eigenvectors = eigenvectors.rowRange(0,out_গুনতি).clone();
    }
    ফেরত *এই;
}

শূন্য PCA::লিখ(FileStorage& fs ) ধ্রুবক
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

শূন্য PCA::পঠ(ধ্রুবক File_গ্রন্থি& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::পঠ(fn["vectors"], eigenvectors);
    cv::পঠ(fn["values"], eigenvalues);
    cv::পঠ(fn["mean"], mean);
}

template <typename T>
পূর্ণসংখ্যা computeCumulativeEnergy(ধ্রুবক Mat& eigenvalues, দ্বিগুণ retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == তথ্য_Type_2<T>::type );

    Mat g(eigenvalues.size(), তথ্য_Type_2<T>::type);

    জন্য(পূর্ণসংখ্যা ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        জন্য(পূর্ণসংখ্যা im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    পূর্ণসংখ্যা L;

    জন্য(L = 0; L < eigenvalues.rows; L++)
    {
        দ্বিগুণ energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        যদি(energy > retainedVariance)
            বিরতি;
    }

    L = std::max(2, L);

    ফেরত L;
}

PCA& PCA::operator()(প্রবেশ_Array তথ্য_3, প্রবেশ_Array __mean, পূর্ণসংখ্যা flags, দ্বিগুণ retainedVariance)
{
    Mat তথ্য = তথ্য_3.getMat(), _mean = __mean.getMat();
    পূর্ণসংখ্যা covar_flags = cv::COVAR_SCALE;
    পূর্ণসংখ্যা len, in_গুনতি;
    আকার_2 mean_sz;

    CV_Assert( তথ্য.channels() == 1 );
    যদি( flags & PCA::তথ্য_AS_COL )
    {
        len = তথ্য.rows;
        in_গুনতি = তথ্য.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = আকার_2(1, len);
    }
    অন্যথা
    {
        len = তথ্য.cols;
        in_গুনতি = তথ্য.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = আকার_2(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    পূর্ণসংখ্যা গুনতি_2 = std::min(len, in_গুনতি);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    যদি( len <= in_গুনতি )
        covar_flags |= cv::COVAR_NORMAL;

    পূর্ণসংখ্যা ctype = std::max(CV_32F, তথ্য.depth());
    mean.create( mean_sz, ctype );

    Mat covar( গুনতি_2, গুনতি_2, ctype );

    যদি( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( তথ্য, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    যদি( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_তথ্য, tmp_mean = repeat(mean, তথ্য.rows/mean.rows, তথ্য.cols/mean.cols);
        যদি( তথ্য.type() != ctype || tmp_mean.তথ্য == mean.তথ্য )
        {
            তথ্য.convertTo( tmp_তথ্য, ctype );
            subtract( tmp_তথ্য, tmp_mean, tmp_তথ্য );
        }
        অন্যথা
        {
            subtract( তথ্য, tmp_mean, tmp_mean );
            tmp_তথ্য = tmp_mean;
        }

        Mat evects1(গুনতি_2, len, ctype);
        gemm( eigenvectors, tmp_তথ্য, 1, Mat(), 0, evects1,
            (flags & PCA::তথ্য_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        পূর্ণসংখ্যা i;
        জন্য( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    পূর্ণসংখ্যা L;
    যদি (ctype == CV_32F)
        L = computeCumulativeEnergy<দশমিক>(eigenvalues, retainedVariance);
    অন্যথা
        L = computeCumulativeEnergy<দ্বিগুণ>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    ফেরত *এই;
}

শূন্য PCA::project(প্রবেশ_Array তথ্য_3, OutputArray ফলাফল) ধ্রুবক
{
    Mat তথ্য = তথ্য_3.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == তথ্য.cols) || (mean.cols == 1 && mean.rows == তথ্য.rows)));
    Mat tmp_তথ্য, tmp_mean = repeat(mean, তথ্য.rows/mean.rows, তথ্য.cols/mean.cols);
    পূর্ণসংখ্যা ctype = mean.type();
    যদি( তথ্য.type() != ctype || tmp_mean.তথ্য == mean.তথ্য )
    {
        তথ্য.convertTo( tmp_তথ্য, ctype );
        subtract( tmp_তথ্য, tmp_mean, tmp_তথ্য );
    }
    অন্যথা
    {
        subtract( তথ্য, tmp_mean, tmp_mean );
        tmp_তথ্য = tmp_mean;
    }
    যদি( mean.rows == 1 )
        gemm( tmp_তথ্য, eigenvectors, 1, Mat(), 0, ফলাফল, GEMM_2_T );
    অন্যথা
        gemm( eigenvectors, tmp_তথ্য, 1, Mat(), 0, ফলাফল, 0 );
}

Mat PCA::project(প্রবেশ_Array তথ্য) ধ্রুবক
{
    Mat ফলাফল;
    project(তথ্য, ফলাফল);
    ফেরত ফলাফল;
}

শূন্য PCA::backProject(প্রবেশ_Array তথ্য_3, OutputArray ফলাফল) ধ্রুবক
{
    Mat তথ্য = তথ্য_3.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == তথ্য.cols) ||
         (mean.cols == 1 && eigenvectors.rows == তথ্য.rows)));

    Mat tmp_তথ্য, tmp_mean;
    তথ্য.convertTo(tmp_তথ্য, mean.type());
    যদি( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, তথ্য.rows, 1);
        gemm( tmp_তথ্য, eigenvectors, 1, tmp_mean, 1, ফলাফল, 0 );
    }
    অন্যথা
    {
        tmp_mean = repeat(mean, 1, তথ্য.cols);
        gemm( eigenvectors, tmp_তথ্য, 1, tmp_mean, 1, ফলাফল, GEMM_1_T );
    }
}

Mat PCA::backProject(প্রবেশ_Array তথ্য) ধ্রুবক
{
    Mat ফলাফল;
    backProject(তথ্য, ফলাফল);
    ফেরত ফলাফল;
}

}

শূন্য cv::PCACompute(প্রবেশ_Array তথ্য, প্রবেশ_Output_Array mean,
                    OutputArray eigenvectors, পূর্ণসংখ্যা maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(তথ্য, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

শূন্য cv::PCACompute(প্রবেশ_Array তথ্য, প্রবেশ_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    পূর্ণসংখ্যা maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(তথ্য, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

শূন্য cv::PCACompute(প্রবেশ_Array তথ্য, প্রবেশ_Output_Array mean,
                    OutputArray eigenvectors, দ্বিগুণ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(তথ্য, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

শূন্য cv::PCACompute(প্রবেশ_Array তথ্য, প্রবেশ_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    দ্বিগুণ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(তথ্য, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

শূন্য cv::PCAProject(প্রবেশ_Array তথ্য, প্রবেশ_Array mean,
                    প্রবেশ_Array eigenvectors, OutputArray ফলাফল)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(তথ্য, ফলাফল);
}

শূন্য cv::PCABackProject(প্রবেশ_Array তথ্য, প্রবেশ_Array mean,
                    প্রবেশ_Array eigenvectors, OutputArray ফলাফল)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(তথ্য, ফলাফল);
}
