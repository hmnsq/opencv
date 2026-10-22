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

#അന്തര്ഭാവ "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

നാമാകാശ cv
{

PCA::PCA() {}

PCA::PCA(നിവേശം_Array data, നിവേശം_Array _mean, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(നിവേശം_Array data, നിവേശം_Array _mean, പൂർണ്ണസംഖ്യ flags, ഇരട്ട retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(നിവേശം_Array _data, നിവേശം_Array __mean, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    പൂർണ്ണസംഖ്യ covar_flags = cv::COVAR_SCALE;
    പൂർണ്ണസംഖ്യ len, in_എണ്ണം;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    എങ്കിൽ( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_എണ്ണം = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    അല്ലെങ്കിൽ
    {
        len = data.cols;
        in_എണ്ണം = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    പൂർണ്ണസംഖ്യ എണ്ണം_2 = std::min(len, in_എണ്ണം), out_എണ്ണം = എണ്ണം_2;
    എങ്കിൽ( maxComponents > 0 )
        out_എണ്ണം = std::min(എണ്ണം_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    എങ്കിൽ( len <= in_എണ്ണം )
        covar_flags |= cv::COVAR_NORMAL;

    പൂർണ്ണസംഖ്യ ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( എണ്ണം_2, എണ്ണം_2, ctype );

    എങ്കിൽ( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    എങ്കിൽ( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        എങ്കിൽ( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        അല്ലെങ്കിൽ
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(എണ്ണം_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി( i = 0; i < out_എണ്ണം; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    എങ്കിൽ( എണ്ണം_2 > out_എണ്ണം )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_എണ്ണം).clone();
        eigenvectors = eigenvectors.rowRange(0,out_എണ്ണം).clone();
    }
    തിരികെ *ഇത്;
}

ശൂന്യം PCA::ലിഖ(FileStorage& fs ) സ്ഥിരം
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

ശൂന്യം PCA::പഠ(സ്ഥിരം File_കണ്ണി& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::പഠ(fn["vectors"], eigenvectors);
    cv::പഠ(fn["values"], eigenvalues);
    cv::പഠ(fn["mean"], mean);
}

template <typename T>
പൂർണ്ണസംഖ്യ computeCumulativeEnergy(സ്ഥിരം Mat& eigenvalues, ഇരട്ട retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    വേണ്ടി(പൂർണ്ണസംഖ്യ ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        വേണ്ടി(പൂർണ്ണസംഖ്യ im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    പൂർണ്ണസംഖ്യ L;

    വേണ്ടി(L = 0; L < eigenvalues.rows; L++)
    {
        ഇരട്ട energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        എങ്കിൽ(energy > retainedVariance)
            നിർത്തുക;
    }

    L = std::max(2, L);

    തിരികെ L;
}

PCA& PCA::operator()(നിവേശം_Array _data, നിവേശം_Array __mean, പൂർണ്ണസംഖ്യ flags, ഇരട്ട retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    പൂർണ്ണസംഖ്യ covar_flags = cv::COVAR_SCALE;
    പൂർണ്ണസംഖ്യ len, in_എണ്ണം;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    എങ്കിൽ( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_എണ്ണം = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    അല്ലെങ്കിൽ
    {
        len = data.cols;
        in_എണ്ണം = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    പൂർണ്ണസംഖ്യ എണ്ണം_2 = std::min(len, in_എണ്ണം);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    എങ്കിൽ( len <= in_എണ്ണം )
        covar_flags |= cv::COVAR_NORMAL;

    പൂർണ്ണസംഖ്യ ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( എണ്ണം_2, എണ്ണം_2, ctype );

    എങ്കിൽ( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    എങ്കിൽ( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        എങ്കിൽ( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        അല്ലെങ്കിൽ
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(എണ്ണം_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    പൂർണ്ണസംഖ്യ L;
    എങ്കിൽ (ctype == CV_32F)
        L = computeCumulativeEnergy<ദശാംശം>(eigenvalues, retainedVariance);
    അല്ലെങ്കിൽ
        L = computeCumulativeEnergy<ഇരട്ട>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    തിരികെ *ഇത്;
}

ശൂന്യം PCA::project(നിവേശം_Array _data, OutputArray result) സ്ഥിരം
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    പൂർണ്ണസംഖ്യ ctype = mean.type();
    എങ്കിൽ( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    അല്ലെങ്കിൽ
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    എങ്കിൽ( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    അല്ലെങ്കിൽ
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(നിവേശം_Array data) സ്ഥിരം
{
    Mat result;
    project(data, result);
    തിരികെ result;
}

ശൂന്യം PCA::backProject(നിവേശം_Array _data, OutputArray result) സ്ഥിരം
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    എങ്കിൽ( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    അല്ലെങ്കിൽ
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(നിവേശം_Array data) സ്ഥിരം
{
    Mat result;
    backProject(data, result);
    തിരികെ result;
}

}

ശൂന്യം cv::PCACompute(നിവേശം_Array data, നിവേശം_Output_Array mean,
                    OutputArray eigenvectors, പൂർണ്ണസംഖ്യ maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

ശൂന്യം cv::PCACompute(നിവേശം_Array data, നിവേശം_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    പൂർണ്ണസംഖ്യ maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

ശൂന്യം cv::PCACompute(നിവേശം_Array data, നിവേശം_Output_Array mean,
                    OutputArray eigenvectors, ഇരട്ട retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

ശൂന്യം cv::PCACompute(നിവേശം_Array data, നിവേശം_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ഇരട്ട retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

ശൂന്യം cv::PCAProject(നിവേശം_Array data, നിവേശം_Array mean,
                    നിവേശം_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

ശൂന്യം cv::PCABackProject(നിവേശം_Array data, നിവേശം_Array mean,
                    നിവേശം_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
