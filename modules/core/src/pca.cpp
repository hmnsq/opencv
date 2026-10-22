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

#অন্তর্ভাৱ "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

নামাকাশ cv
{

PCA::PCA() {}

PCA::PCA(প্ৰবেশ_Array data, প্ৰবেশ_Array _mean, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(প্ৰবেশ_Array data, প্ৰবেশ_Array _mean, পূর্ণাঙ্ক flags, দ্ৱিগুণ retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(প্ৰবেশ_Array _data, প্ৰবেশ_Array __mean, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    পূর্ণাঙ্ক covar_flags = cv::COVAR_SCALE;
    পূর্ণাঙ্ক len, in_গুনতি;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    যদি( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_গুনতি = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    অন্যথা
    {
        len = data.cols;
        in_গুনতি = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    পূর্ণাঙ্ক গুনতি_2 = std::min(len, in_গুনতি), out_গুনতি = গুনতি_2;
    যদি( maxComponents > 0 )
        out_গুনতি = std::min(গুনতি_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    যদি( len <= in_গুনতি )
        covar_flags |= cv::COVAR_NORMAL;

    পূর্ণাঙ্ক ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( গুনতি_2, গুনতি_2, ctype );

    যদি( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    যদি( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        যদি( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        অন্যথা
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(গুনতি_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        পূর্ণাঙ্ক i;
        বাবে( i = 0; i < out_গুনতি; i++ )
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
    ঘূৰাই *এতত্;
}

শূন্য PCA::লিখ(FileStorage& fs ) ধ্রুৱ
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

শূন্য PCA::পঠ(ধ্রুৱ File_গ্ৰন্থি& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::পঠ(fn["vectors"], eigenvectors);
    cv::পঠ(fn["values"], eigenvalues);
    cv::পঠ(fn["mean"], mean);
}

template <typename T>
পূর্ণাঙ্ক computeCumulativeEnergy(ধ্রুৱ Mat& eigenvalues, দ্ৱিগুণ retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    বাবে(পূর্ণাঙ্ক ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        বাবে(পূর্ণাঙ্ক im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    পূর্ণাঙ্ক L;

    বাবে(L = 0; L < eigenvalues.rows; L++)
    {
        দ্ৱিগুণ energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        যদি(energy > retainedVariance)
            ভঙ্গ;
    }

    L = std::max(2, L);

    ঘূৰাই L;
}

PCA& PCA::operator()(প্ৰবেশ_Array _data, প্ৰবেশ_Array __mean, পূর্ণাঙ্ক flags, দ্ৱিগুণ retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    পূর্ণাঙ্ক covar_flags = cv::COVAR_SCALE;
    পূর্ণাঙ্ক len, in_গুনতি;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    যদি( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_গুনতি = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    অন্যথা
    {
        len = data.cols;
        in_গুনতি = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    পূর্ণাঙ্ক গুনতি_2 = std::min(len, in_গুনতি);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    যদি( len <= in_গুনতি )
        covar_flags |= cv::COVAR_NORMAL;

    পূর্ণাঙ্ক ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( গুনতি_2, গুনতি_2, ctype );

    যদি( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    যদি( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        যদি( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        অন্যথা
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(গুনতি_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        পূর্ণাঙ্ক i;
        বাবে( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    পূর্ণাঙ্ক L;
    যদি (ctype == CV_32F)
        L = computeCumulativeEnergy<ভিন্ন>(eigenvalues, retainedVariance);
    অন্যথা
        L = computeCumulativeEnergy<দ্ৱিগুণ>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    ঘূৰাই *এতত্;
}

শূন্য PCA::project(প্ৰবেশ_Array _data, OutputArray result) ধ্রুৱ
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    পূর্ণাঙ্ক ctype = mean.type();
    যদি( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    অন্যথা
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    যদি( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    অন্যথা
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(প্ৰবেশ_Array data) ধ্রুৱ
{
    Mat result;
    project(data, result);
    ঘূৰাই result;
}

শূন্য PCA::backProject(প্ৰবেশ_Array _data, OutputArray result) ধ্রুৱ
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    যদি( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    অন্যথা
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(প্ৰবেশ_Array data) ধ্রুৱ
{
    Mat result;
    backProject(data, result);
    ঘূৰাই result;
}

}

শূন্য cv::PCACompute(প্ৰবেশ_Array data, প্ৰবেশ_Output_Array mean,
                    OutputArray eigenvectors, পূর্ণাঙ্ক maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

শূন্য cv::PCACompute(প্ৰবেশ_Array data, প্ৰবেশ_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    পূর্ণাঙ্ক maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

শূন্য cv::PCACompute(প্ৰবেশ_Array data, প্ৰবেশ_Output_Array mean,
                    OutputArray eigenvectors, দ্ৱিগুণ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

শূন্য cv::PCACompute(প্ৰবেশ_Array data, প্ৰবেশ_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    দ্ৱিগুণ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

শূন্য cv::PCAProject(প্ৰবেশ_Array data, প্ৰবেশ_Array mean,
                    প্ৰবেশ_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

শূন্য cv::PCABackProject(প্ৰবেশ_Array data, প্ৰবেশ_Array mean,
                    প্ৰবেশ_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
