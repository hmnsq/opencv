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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

ନାମାକାଶ cv
{

PCA::PCA() {}

PCA::PCA(ନିବେଶ_Array data, ନିବେଶ_Array _mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(ନିବେଶ_Array data, ନିବେଶ_Array _mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଦ୍ୱିଗୁଣ retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(ନିବେଶ_Array _data, ନିବେଶ_Array __mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା covar_flags = cv::COVAR_SCALE;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, in_ଗଣନା;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    ଯଦି( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_ଗଣନା = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    ଅନ୍ୟଥା
    {
        len = data.cols;
        in_ଗଣନା = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = std::min(len, in_ଗଣନା), out_ଗଣନା = ଗଣନା_3;
    ଯଦି( maxComponents > 0 )
        out_ଗଣନା = std::min(ଗଣନା_3, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ଯଦି( len <= in_ଗଣନା )
        covar_flags |= cv::COVAR_NORMAL;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( ଗଣନା_3, ଗଣନା_3, ctype );

    ଯଦି( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ଯଦି( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        ଯଦି( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        ଅନ୍ୟଥା
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(ଗଣନା_3, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ( i = 0; i < out_ଗଣନା; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    ଯଦି( ଗଣନା_3 > out_ଗଣନା )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_ଗଣନା).clone();
        eigenvectors = eigenvectors.rowRange(0,out_ଗଣନା).clone();
    }
    ଫେରନ୍ତୁ *ଏହି;
}

ଶୂନ୍ୟ PCA::ଲିଖ(FileStorage& fs ) ଅଚଳ
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

ଶୂନ୍ୟ PCA::ପଠ(ଅଚଳ File_ଗଣ୍ଠି& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::ପଠ(fn["vectors"], eigenvectors);
    cv::ପଠ(fn["values"], eigenvalues);
    cv::ପଠ(fn["mean"], mean);
}

template <typename T>
ପୂର୍ଣ୍ଣସଂଖ୍ୟା computeCumulativeEnergy(ଅଚଳ Mat& eigenvalues, ଦ୍ୱିଗୁଣ retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା L;

    ପାଇଁ(L = 0; L < eigenvalues.rows; L++)
    {
        ଦ୍ୱିଗୁଣ energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        ଯଦି(energy > retainedVariance)
            ଭାଙ୍ଗ;
    }

    L = std::max(2, L);

    ଫେରନ୍ତୁ L;
}

PCA& PCA::operator()(ନିବେଶ_Array _data, ନିବେଶ_Array __mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଦ୍ୱିଗୁଣ retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା covar_flags = cv::COVAR_SCALE;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, in_ଗଣନା;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    ଯଦି( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_ଗଣନା = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    ଅନ୍ୟଥା
    {
        len = data.cols;
        in_ଗଣନା = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = std::min(len, in_ଗଣନା);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ଯଦି( len <= in_ଗଣନା )
        covar_flags |= cv::COVAR_NORMAL;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( ଗଣନା_3, ଗଣନା_3, ctype );

    ଯଦି( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ଯଦି( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        ଯଦି( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        ଅନ୍ୟଥା
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(ଗଣନା_3, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା L;
    ଯଦି (ctype == CV_32F)
        L = computeCumulativeEnergy<ଦଶମିକ>(eigenvalues, retainedVariance);
    ଅନ୍ୟଥା
        L = computeCumulativeEnergy<ଦ୍ୱିଗୁଣ>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    ଫେରନ୍ତୁ *ଏହି;
}

ଶୂନ୍ୟ PCA::project(ନିବେଶ_Array _data, OutputArray result) ଅଚଳ
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ctype = mean.type();
    ଯଦି( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    ଅନ୍ୟଥା
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    ଯଦି( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    ଅନ୍ୟଥା
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(ନିବେଶ_Array data) ଅଚଳ
{
    Mat result;
    project(data, result);
    ଫେରନ୍ତୁ result;
}

ଶୂନ୍ୟ PCA::backProject(ନିବେଶ_Array _data, OutputArray result) ଅଚଳ
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    ଯଦି( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    ଅନ୍ୟଥା
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(ନିବେଶ_Array data) ଅଚଳ
{
    Mat result;
    backProject(data, result);
    ଫେରନ୍ତୁ result;
}

}

ଶୂନ୍ୟ cv::PCACompute(ନିବେଶ_Array data, ନିବେଶ_Output_Array mean,
                    OutputArray eigenvectors, ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

ଶୂନ୍ୟ cv::PCACompute(ନିବେଶ_Array data, ନିବେଶ_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

ଶୂନ୍ୟ cv::PCACompute(ନିବେଶ_Array data, ନିବେଶ_Output_Array mean,
                    OutputArray eigenvectors, ଦ୍ୱିଗୁଣ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

ଶୂନ୍ୟ cv::PCACompute(ନିବେଶ_Array data, ନିବେଶ_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ଦ୍ୱିଗୁଣ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

ଶୂନ୍ୟ cv::PCAProject(ନିବେଶ_Array data, ନିବେଶ_Array mean,
                    ନିବେଶ_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

ଶୂନ୍ୟ cv::PCABackProject(ନିବେଶ_Array data, ନିବେଶ_Array mean,
                    ନିବେଶ_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
