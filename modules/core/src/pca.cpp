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

#અન્તર્ભાવ "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

નામાકાશ cv
{

PCA::PCA() {}

PCA::PCA(આગત_Array data, આગત_Array _mean, પૂર્ણાંક flags, પૂર્ણાંક maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(આગત_Array data, આગત_Array _mean, પૂર્ણાંક flags, દ્વિગુણ retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(આગત_Array _data, આગત_Array __mean, પૂર્ણાંક flags, પૂર્ણાંક maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    પૂર્ણાંક covar_flags = cv::COVAR_SCALE;
    પૂર્ણાંક len, in_ગણતરી;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    જો( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_ગણતરી = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    નહીંતર
    {
        len = data.cols;
        in_ગણતરી = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    પૂર્ણાંક ગણતરી_2 = std::min(len, in_ગણતરી), out_ગણતરી = ગણતરી_2;
    જો( maxComponents > 0 )
        out_ગણતરી = std::min(ગણતરી_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    જો( len <= in_ગણતરી )
        covar_flags |= cv::COVAR_NORMAL;

    પૂર્ણાંક ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( ગણતરી_2, ગણતરી_2, ctype );

    જો( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    જો( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        જો( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        નહીંતર
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(ગણતરી_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        પૂર્ણાંક i;
        માટે( i = 0; i < out_ગણતરી; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    જો( ગણતરી_2 > out_ગણતરી )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_ગણતરી).clone();
        eigenvectors = eigenvectors.rowRange(0,out_ગણતરી).clone();
    }
    પાછા *આ;
}

શૂન્ય PCA::લિખ(FileStorage& fs ) અચલ
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

શૂન્ય PCA::પઠ(અચલ File_ગાંઠ& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::પઠ(fn["vectors"], eigenvectors);
    cv::પઠ(fn["values"], eigenvalues);
    cv::પઠ(fn["mean"], mean);
}

template <typename T>
પૂર્ણાંક computeCumulativeEnergy(અચલ Mat& eigenvalues, દ્વિગુણ retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    માટે(પૂર્ણાંક ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        માટે(પૂર્ણાંક im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    પૂર્ણાંક L;

    માટે(L = 0; L < eigenvalues.rows; L++)
    {
        દ્વિગુણ energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        જો(energy > retainedVariance)
            તોડો;
    }

    L = std::max(2, L);

    પાછા L;
}

PCA& PCA::operator()(આગત_Array _data, આગત_Array __mean, પૂર્ણાંક flags, દ્વિગુણ retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    પૂર્ણાંક covar_flags = cv::COVAR_SCALE;
    પૂર્ણાંક len, in_ગણતરી;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    જો( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_ગણતરી = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    નહીંતર
    {
        len = data.cols;
        in_ગણતરી = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    પૂર્ણાંક ગણતરી_2 = std::min(len, in_ગણતરી);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    જો( len <= in_ગણતરી )
        covar_flags |= cv::COVAR_NORMAL;

    પૂર્ણાંક ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( ગણતરી_2, ગણતરી_2, ctype );

    જો( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    જો( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        જો( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        નહીંતર
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(ગણતરી_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        પૂર્ણાંક i;
        માટે( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    પૂર્ણાંક L;
    જો (ctype == CV_32F)
        L = computeCumulativeEnergy<દશાંશ>(eigenvalues, retainedVariance);
    નહીંતર
        L = computeCumulativeEnergy<દ્વિગુણ>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    પાછા *આ;
}

શૂન્ય PCA::project(આગત_Array _data, OutputArray result) અચલ
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    પૂર્ણાંક ctype = mean.type();
    જો( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    નહીંતર
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    જો( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    નહીંતર
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(આગત_Array data) અચલ
{
    Mat result;
    project(data, result);
    પાછા result;
}

શૂન્ય PCA::backProject(આગત_Array _data, OutputArray result) અચલ
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    જો( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    નહીંતર
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(આગત_Array data) અચલ
{
    Mat result;
    backProject(data, result);
    પાછા result;
}

}

શૂન્ય cv::PCACompute(આગત_Array data, આગત_Output_Array mean,
                    OutputArray eigenvectors, પૂર્ણાંક maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

શૂન્ય cv::PCACompute(આગત_Array data, આગત_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    પૂર્ણાંક maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

શૂન્ય cv::PCACompute(આગત_Array data, આગત_Output_Array mean,
                    OutputArray eigenvectors, દ્વિગુણ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

શૂન્ય cv::PCACompute(આગત_Array data, આગત_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    દ્વિગુણ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

શૂન્ય cv::PCAProject(આગત_Array data, આગત_Array mean,
                    આગત_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

શૂન્ય cv::PCABackProject(આગત_Array data, આગત_Array mean,
                    આગત_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
