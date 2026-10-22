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

#अन्तर्भाव "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

नामाकाश cv
{

PCA::PCA() {}

PCA::PCA(निवेशः_Array data, निवेशः_Array _mean, पूर्णाङ्क flags, पूर्णाङ्क maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(निवेशः_Array data, निवेशः_Array _mean, पूर्णाङ्क flags, द्विगुण retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(निवेशः_Array _data, निवेशः_Array __mean, पूर्णाङ्क flags, पूर्णाङ्क maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    पूर्णाङ्क covar_flags = cv::COVAR_SCALE;
    पूर्णाङ्क len, in_गणनम्;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    जइ( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_गणनम् = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    अण्णहा
    {
        len = data.cols;
        in_गणनम् = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    पूर्णाङ्क गणनम्_2 = std::min(len, in_गणनम्), out_गणनम् = गणनम्_2;
    जइ( maxComponents > 0 )
        out_गणनम् = std::min(गणनम्_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    जइ( len <= in_गणनम् )
        covar_flags |= cv::COVAR_NORMAL;

    पूर्णाङ्क ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( गणनम्_2, गणनम्_2, ctype );

    जइ( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    जइ( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        जइ( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        अण्णहा
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(गणनम्_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        पूर्णाङ्क i;
        प्रति( i = 0; i < out_गणनम्; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    जइ( गणनम्_2 > out_गणनम् )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_गणनम्).clone();
        eigenvectors = eigenvectors.rowRange(0,out_गणनम्).clone();
    }
    प्रत्यागम *एतत्;
}

शून्य PCA::लिख(FileStorage& fs ) ध्रुव
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

शून्य PCA::पठ(ध्रुव File_ग्रन्थिः& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::पठ(fn["vectors"], eigenvectors);
    cv::पठ(fn["values"], eigenvalues);
    cv::पठ(fn["mean"], mean);
}

template <typename T>
पूर्णाङ्क computeCumulativeEnergy(ध्रुव Mat& eigenvalues, द्विगुण retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    प्रति(पूर्णाङ्क ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        प्रति(पूर्णाङ्क im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    पूर्णाङ्क L;

    प्रति(L = 0; L < eigenvalues.rows; L++)
    {
        द्विगुण energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        जइ(energy > retainedVariance)
            भङ्ग;
    }

    L = std::max(2, L);

    प्रत्यागम L;
}

PCA& PCA::operator()(निवेशः_Array _data, निवेशः_Array __mean, पूर्णाङ्क flags, द्विगुण retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    पूर्णाङ्क covar_flags = cv::COVAR_SCALE;
    पूर्णाङ्क len, in_गणनम्;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    जइ( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_गणनम् = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    अण्णहा
    {
        len = data.cols;
        in_गणनम् = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    पूर्णाङ्क गणनम्_2 = std::min(len, in_गणनम्);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    जइ( len <= in_गणनम् )
        covar_flags |= cv::COVAR_NORMAL;

    पूर्णाङ्क ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( गणनम्_2, गणनम्_2, ctype );

    जइ( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    जइ( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        जइ( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        अण्णहा
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(गणनम्_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        पूर्णाङ्क i;
        प्रति( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    पूर्णाङ्क L;
    जइ (ctype == CV_32F)
        L = computeCumulativeEnergy<भिन्न>(eigenvalues, retainedVariance);
    अण्णहा
        L = computeCumulativeEnergy<द्विगुण>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    प्रत्यागम *एतत्;
}

शून्य PCA::project(निवेशः_Array _data, OutputArray result) ध्रुव
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    पूर्णाङ्क ctype = mean.type();
    जइ( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    अण्णहा
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    जइ( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    अण्णहा
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(निवेशः_Array data) ध्रुव
{
    Mat result;
    project(data, result);
    प्रत्यागम result;
}

शून्य PCA::backProject(निवेशः_Array _data, OutputArray result) ध्रुव
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    जइ( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    अण्णहा
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(निवेशः_Array data) ध्रुव
{
    Mat result;
    backProject(data, result);
    प्रत्यागम result;
}

}

शून्य cv::PCACompute(निवेशः_Array data, निवेशः_Output_Array mean,
                    OutputArray eigenvectors, पूर्णाङ्क maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

शून्य cv::PCACompute(निवेशः_Array data, निवेशः_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    पूर्णाङ्क maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

शून्य cv::PCACompute(निवेशः_Array data, निवेशः_Output_Array mean,
                    OutputArray eigenvectors, द्विगुण retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

शून्य cv::PCACompute(निवेशः_Array data, निवेशः_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    द्विगुण retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

शून्य cv::PCAProject(निवेशः_Array data, निवेशः_Array mean,
                    निवेशः_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

शून्य cv::PCABackProject(निवेशः_Array data, निवेशः_Array mean,
                    निवेशः_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
