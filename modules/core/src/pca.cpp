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

#அந்தர்பாவ "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

நாமாகாஶ cv
{

PCA::PCA() {}

PCA::PCA(உள்ளீடு_Array data, உள்ளீடு_Array _mean, முழு flags, முழு maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(உள்ளீடு_Array data, உள்ளீடு_Array _mean, முழு flags, இரட்டை retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(உள்ளீடு_Array _data, உள்ளீடு_Array __mean, முழு flags, முழு maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    முழு covar_flags = cv::COVAR_SCALE;
    முழு len, in_மொத்தம்;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    எனில்( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_மொத்தம் = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    இல்லையெனில்
    {
        len = data.cols;
        in_மொத்தம் = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    முழு மொத்தம்_2 = std::min(len, in_மொத்தம்), out_மொத்தம் = மொத்தம்_2;
    எனில்( maxComponents > 0 )
        out_மொத்தம் = std::min(மொத்தம்_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    எனில்( len <= in_மொத்தம் )
        covar_flags |= cv::COVAR_NORMAL;

    முழு ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( மொத்தம்_2, மொத்தம்_2, ctype );

    எனில்( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    எனில்( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        எனில்( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        இல்லையெனில்
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(மொத்தம்_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        முழு i;
        க்கு( i = 0; i < out_மொத்தம்; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    எனில்( மொத்தம்_2 > out_மொத்தம் )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_மொத்தம்).clone();
        eigenvectors = eigenvectors.rowRange(0,out_மொத்தம்).clone();
    }
    திரும்பு *இது;
}

வெறுமை PCA::லிக(FileStorage& fs ) மாறிலி
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

வெறுமை PCA::பட(மாறிலி File_முனை& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::பட(fn["vectors"], eigenvectors);
    cv::பட(fn["values"], eigenvalues);
    cv::பட(fn["mean"], mean);
}

template <typename T>
முழு computeCumulativeEnergy(மாறிலி Mat& eigenvalues, இரட்டை retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    க்கு(முழு ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        க்கு(முழு im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    முழு L;

    க்கு(L = 0; L < eigenvalues.rows; L++)
    {
        இரட்டை energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        எனில்(energy > retainedVariance)
            நிறுத்து;
    }

    L = std::max(2, L);

    திரும்பு L;
}

PCA& PCA::operator()(உள்ளீடு_Array _data, உள்ளீடு_Array __mean, முழு flags, இரட்டை retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    முழு covar_flags = cv::COVAR_SCALE;
    முழு len, in_மொத்தம்;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    எனில்( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_மொத்தம் = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    இல்லையெனில்
    {
        len = data.cols;
        in_மொத்தம் = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    முழு மொத்தம்_2 = std::min(len, in_மொத்தம்);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    எனில்( len <= in_மொத்தம் )
        covar_flags |= cv::COVAR_NORMAL;

    முழு ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( மொத்தம்_2, மொத்தம்_2, ctype );

    எனில்( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    எனில்( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        எனில்( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        இல்லையெனில்
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(மொத்தம்_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        முழு i;
        க்கு( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    முழு L;
    எனில் (ctype == CV_32F)
        L = computeCumulativeEnergy<தசம>(eigenvalues, retainedVariance);
    இல்லையெனில்
        L = computeCumulativeEnergy<இரட்டை>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    திரும்பு *இது;
}

வெறுமை PCA::project(உள்ளீடு_Array _data, OutputArray result) மாறிலி
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    முழு ctype = mean.type();
    எனில்( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    இல்லையெனில்
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    எனில்( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    இல்லையெனில்
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(உள்ளீடு_Array data) மாறிலி
{
    Mat result;
    project(data, result);
    திரும்பு result;
}

வெறுமை PCA::backProject(உள்ளீடு_Array _data, OutputArray result) மாறிலி
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    எனில்( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    இல்லையெனில்
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(உள்ளீடு_Array data) மாறிலி
{
    Mat result;
    backProject(data, result);
    திரும்பு result;
}

}

வெறுமை cv::PCACompute(உள்ளீடு_Array data, உள்ளீடு_Output_Array mean,
                    OutputArray eigenvectors, முழு maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

வெறுமை cv::PCACompute(உள்ளீடு_Array data, உள்ளீடு_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    முழு maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

வெறுமை cv::PCACompute(உள்ளீடு_Array data, உள்ளீடு_Output_Array mean,
                    OutputArray eigenvectors, இரட்டை retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

வெறுமை cv::PCACompute(உள்ளீடு_Array data, உள்ளீடு_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    இரட்டை retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

வெறுமை cv::PCAProject(உள்ளீடு_Array data, உள்ளீடு_Array mean,
                    உள்ளீடு_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

வெறுமை cv::PCABackProject(உள்ளீடு_Array data, உள்ளீடு_Array mean,
                    உள்ளீடு_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
