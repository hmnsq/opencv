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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{

PCA::PCA() {}

PCA::PCA(ಒಳಹರಿವು_Array data, ಒಳಹರಿವು_Array _mean, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(ಒಳಹರಿವು_Array data, ಒಳಹರಿವು_Array _mean, ಪೂರ್ಣಾಙ್ಕ flags, ದ್ವಿಗುಣ retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(ಒಳಹರಿವು_Array _data, ಒಳಹರಿವು_Array __mean, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    ಪೂರ್ಣಾಙ್ಕ covar_flags = cv::COVAR_SCALE;
    ಪೂರ್ಣಾಙ್ಕ len, in_ಎಣಿಕೆ;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    ಯದಿ( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_ಎಣಿಕೆ = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    ಅನ್ಯಥಾ
    {
        len = data.cols;
        in_ಎಣಿಕೆ = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2 = std::min(len, in_ಎಣಿಕೆ), out_ಎಣಿಕೆ = ಎಣಿಕೆ_2;
    ಯದಿ( maxComponents > 0 )
        out_ಎಣಿಕೆ = std::min(ಎಣಿಕೆ_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ಯದಿ( len <= in_ಎಣಿಕೆ )
        covar_flags |= cv::COVAR_NORMAL;

    ಪೂರ್ಣಾಙ್ಕ ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( ಎಣಿಕೆ_2, ಎಣಿಕೆ_2, ctype );

    ಯದಿ( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ಯದಿ( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        ಯದಿ( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        ಅನ್ಯಥಾ
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(ಎಣಿಕೆ_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ( i = 0; i < out_ಎಣಿಕೆ; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    ಯದಿ( ಎಣಿಕೆ_2 > out_ಎಣಿಕೆ )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_ಎಣಿಕೆ).clone();
        eigenvectors = eigenvectors.rowRange(0,out_ಎಣಿಕೆ).clone();
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಶೂನ್ಯ PCA::ಲಿಖ(FileStorage& fs ) ಧ್ರುವ
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

ಶೂನ್ಯ PCA::ಪಠ(ಧ್ರುವ File_ಗಂಟು& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::ಪಠ(fn["vectors"], eigenvectors);
    cv::ಪಠ(fn["values"], eigenvalues);
    cv::ಪಠ(fn["mean"], mean);
}

template <typename T>
ಪೂರ್ಣಾಙ್ಕ computeCumulativeEnergy(ಧ್ರುವ Mat& eigenvalues, ದ್ವಿಗುಣ retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    ಪೂರ್ಣಾಙ್ಕ L;

    ಪ್ರತಿ(L = 0; L < eigenvalues.rows; L++)
    {
        ದ್ವಿಗುಣ energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        ಯದಿ(energy > retainedVariance)
            ಭಙ್ಗ;
    }

    L = std::max(2, L);

    ಪ್ರತ್ಯಾಗಮ L;
}

PCA& PCA::operator()(ಒಳಹರಿವು_Array _data, ಒಳಹರಿವು_Array __mean, ಪೂರ್ಣಾಙ್ಕ flags, ದ್ವಿಗುಣ retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    ಪೂರ್ಣಾಙ್ಕ covar_flags = cv::COVAR_SCALE;
    ಪೂರ್ಣಾಙ್ಕ len, in_ಎಣಿಕೆ;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    ಯದಿ( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_ಎಣಿಕೆ = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    ಅನ್ಯಥಾ
    {
        len = data.cols;
        in_ಎಣಿಕೆ = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2 = std::min(len, in_ಎಣಿಕೆ);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ಯದಿ( len <= in_ಎಣಿಕೆ )
        covar_flags |= cv::COVAR_NORMAL;

    ಪೂರ್ಣಾಙ್ಕ ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( ಎಣಿಕೆ_2, ಎಣಿಕೆ_2, ctype );

    ಯದಿ( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ಯದಿ( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        ಯದಿ( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        ಅನ್ಯಥಾ
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(ಎಣಿಕೆ_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    ಪೂರ್ಣಾಙ್ಕ L;
    ಯದಿ (ctype == CV_32F)
        L = computeCumulativeEnergy<ಭಿನ್ನ>(eigenvalues, retainedVariance);
    ಅನ್ಯಥಾ
        L = computeCumulativeEnergy<ದ್ವಿಗುಣ>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಶೂನ್ಯ PCA::project(ಒಳಹರಿವು_Array _data, OutputArray result) ಧ್ರುವ
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    ಪೂರ್ಣಾಙ್ಕ ctype = mean.type();
    ಯದಿ( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    ಅನ್ಯಥಾ
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    ಯದಿ( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    ಅನ್ಯಥಾ
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(ಒಳಹರಿವು_Array data) ಧ್ರುವ
{
    Mat result;
    project(data, result);
    ಪ್ರತ್ಯಾಗಮ result;
}

ಶೂನ್ಯ PCA::backProject(ಒಳಹರಿವು_Array _data, OutputArray result) ಧ್ರುವ
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    ಯದಿ( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    ಅನ್ಯಥಾ
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(ಒಳಹರಿವು_Array data) ಧ್ರುವ
{
    Mat result;
    backProject(data, result);
    ಪ್ರತ್ಯಾಗಮ result;
}

}

ಶೂನ್ಯ cv::PCACompute(ಒಳಹರಿವು_Array data, ಒಳಹರಿವು_Output_Array mean,
                    OutputArray eigenvectors, ಪೂರ್ಣಾಙ್ಕ maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

ಶೂನ್ಯ cv::PCACompute(ಒಳಹರಿವು_Array data, ಒಳಹರಿವು_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ಪೂರ್ಣಾಙ್ಕ maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

ಶೂನ್ಯ cv::PCACompute(ಒಳಹರಿವು_Array data, ಒಳಹರಿವು_Output_Array mean,
                    OutputArray eigenvectors, ದ್ವಿಗುಣ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

ಶೂನ್ಯ cv::PCACompute(ಒಳಹರಿವು_Array data, ಒಳಹರಿವು_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ದ್ವಿಗುಣ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

ಶೂನ್ಯ cv::PCAProject(ಒಳಹರಿವು_Array data, ಒಳಹರಿವು_Array mean,
                    ಒಳಹರಿವು_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

ಶೂನ್ಯ cv::PCABackProject(ಒಳಹರಿವು_Array data, ಒಳಹರಿವು_Array mean,
                    ಒಳಹರಿವು_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
