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

PCA::PCA(إدخال_Array data, إدخال_Array _mean, صحيح flags, صحيح maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(إدخال_Array data, إدخال_Array _mean, صحيح flags, مزدوج retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(إدخال_Array _data, إدخال_Array __mean, صحيح flags, صحيح maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, in_تعداد;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    إذا( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_تعداد = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    وإلا
    {
        len = data.cols;
        in_تعداد = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    صحيح تعداد_3 = std::min(len, in_تعداد), out_تعداد = تعداد_3;
    إذا( maxComponents > 0 )
        out_تعداد = std::min(تعداد_3, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= in_تعداد )
        covar_flags |= cv::COVAR_NORMAL;

    صحيح ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( تعداد_3, تعداد_3, ctype );

    إذا( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        إذا( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        وإلا
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(تعداد_3, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        صحيح i;
        لكل( i = 0; i < out_تعداد; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    إذا( تعداد_3 > out_تعداد )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_تعداد).clone();
        eigenvectors = eigenvectors.rowRange(0,out_تعداد).clone();
    }
    إرجاع *هذا;
}

فراغ PCA::اكتب(FileStorage& fs ) ثابت
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

فراغ PCA::اقرأ(ثابت File_عقدة& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::اقرأ(fn["vectors"], eigenvectors);
    cv::اقرأ(fn["values"], eigenvalues);
    cv::اقرأ(fn["mean"], mean);
}

template <typename T>
صحيح computeCumulativeEnergy(ثابت Mat& eigenvalues, مزدوج retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    لكل(صحيح ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        لكل(صحيح im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    صحيح L;

    لكل(L = 0; L < eigenvalues.rows; L++)
    {
        مزدوج energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        إذا(energy > retainedVariance)
            اكسر;
    }

    L = std::max(2, L);

    إرجاع L;
}

PCA& PCA::operator()(إدخال_Array _data, إدخال_Array __mean, صحيح flags, مزدوج retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, in_تعداد;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    إذا( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_تعداد = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    وإلا
    {
        len = data.cols;
        in_تعداد = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    صحيح تعداد_3 = std::min(len, in_تعداد);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= in_تعداد )
        covar_flags |= cv::COVAR_NORMAL;

    صحيح ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( تعداد_3, تعداد_3, ctype );

    إذا( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        إذا( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        وإلا
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(تعداد_3, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        صحيح i;
        لكل( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
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
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    إرجاع *هذا;
}

فراغ PCA::project(إدخال_Array _data, OutputArray result) ثابت
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    صحيح ctype = mean.type();
    إذا( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    وإلا
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    إذا( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    وإلا
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(إدخال_Array data) ثابت
{
    Mat result;
    project(data, result);
    إرجاع result;
}

فراغ PCA::backProject(إدخال_Array _data, OutputArray result) ثابت
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    إذا( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    وإلا
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(إدخال_Array data) ثابت
{
    Mat result;
    backProject(data, result);
    إرجاع result;
}

}

فراغ cv::PCACompute(إدخال_Array data, إدخال_Output_Array mean,
                    OutputArray eigenvectors, صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

فراغ cv::PCACompute(إدخال_Array data, إدخال_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

فراغ cv::PCACompute(إدخال_Array data, إدخال_Output_Array mean,
                    OutputArray eigenvectors, مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

فراغ cv::PCACompute(إدخال_Array data, إدخال_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

فراغ cv::PCAProject(إدخال_Array data, إدخال_Array mean,
                    إدخال_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

فراغ cv::PCABackProject(إدخال_Array data, إدخال_Array mean,
                    إدخال_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
