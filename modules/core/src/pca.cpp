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

#شامل "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

فضاینام cv
{

PCA::PCA() {}

PCA::PCA(درون_داد_Array data, درون_داد_Array _mean, صحیح flags, صحیح maxComponents)
{
    operator()(data, _mean, flags, maxComponents);
}

PCA::PCA(درون_داد_Array data, درون_داد_Array _mean, صحیح flags, دوگانه retainedVariance)
{
    operator()(data, _mean, flags, retainedVariance);
}

PCA& PCA::operator()(درون_داد_Array _data, درون_داد_Array __mean, صحیح flags, صحیح maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    صحیح covar_flags = cv::COVAR_SCALE;
    صحیح len, in_شمار;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    اگر( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_شمار = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    وگرنه
    {
        len = data.cols;
        in_شمار = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    صحیح شمار_2 = std::min(len, in_شمار), out_شمار = شمار_2;
    اگر( maxComponents > 0 )
        out_شمار = std::min(شمار_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    اگر( len <= in_شمار )
        covar_flags |= cv::COVAR_NORMAL;

    صحیح ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( شمار_2, شمار_2, ctype );

    اگر( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    اگر( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        اگر( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        وگرنه
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(شمار_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        صحیح i;
        برای( i = 0; i < out_شمار; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    اگر( شمار_2 > out_شمار )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_شمار).clone();
        eigenvectors = eigenvectors.rowRange(0,out_شمار).clone();
    }
    بازگشت *این;
}

تهی PCA::اكتب(FileStorage& fs ) ثابت
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

تهی PCA::اقرأ(ثابت File_گره& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::اقرأ(fn["vectors"], eigenvectors);
    cv::اقرأ(fn["values"], eigenvalues);
    cv::اقرأ(fn["mean"], mean);
}

template <typename T>
صحیح computeCumulativeEnergy(ثابت Mat& eigenvalues, دوگانه retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    برای(صحیح ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        برای(صحیح im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    صحیح L;

    برای(L = 0; L < eigenvalues.rows; L++)
    {
        دوگانه energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        اگر(energy > retainedVariance)
            بشکن;
    }

    L = std::max(2, L);

    بازگشت L;
}

PCA& PCA::operator()(درون_داد_Array _data, درون_داد_Array __mean, صحیح flags, دوگانه retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    صحیح covar_flags = cv::COVAR_SCALE;
    صحیح len, in_شمار;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    اگر( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_شمار = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    وگرنه
    {
        len = data.cols;
        in_شمار = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    صحیح شمار_2 = std::min(len, in_شمار);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    اگر( len <= in_شمار )
        covar_flags |= cv::COVAR_NORMAL;

    صحیح ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( شمار_2, شمار_2, ctype );

    اگر( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    اگر( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        اگر( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        وگرنه
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(شمار_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        صحیح i;
        برای( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    صحیح L;
    اگر (ctype == CV_32F)
        L = computeCumulativeEnergy<اعشاری>(eigenvalues, retainedVariance);
    وگرنه
        L = computeCumulativeEnergy<دوگانه>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    بازگشت *این;
}

تهی PCA::project(درون_داد_Array _data, OutputArray result) ثابت
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    صحیح ctype = mean.type();
    اگر( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    وگرنه
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    اگر( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    وگرنه
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(درون_داد_Array data) ثابت
{
    Mat result;
    project(data, result);
    بازگشت result;
}

تهی PCA::backProject(درون_داد_Array _data, OutputArray result) ثابت
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    اگر( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    وگرنه
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(درون_داد_Array data) ثابت
{
    Mat result;
    backProject(data, result);
    بازگشت result;
}

}

تهی cv::PCACompute(درون_داد_Array data, درون_داد_Output_Array mean,
                    OutputArray eigenvectors, صحیح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

تهی cv::PCACompute(درون_داد_Array data, درون_داد_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    صحیح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

تهی cv::PCACompute(درون_داد_Array data, درون_داد_Output_Array mean,
                    OutputArray eigenvectors, دوگانه retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

تهی cv::PCACompute(درون_داد_Array data, درون_داد_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    دوگانه retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

تهی cv::PCAProject(درون_داد_Array data, درون_داد_Array mean,
                    درون_داد_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

تهی cv::PCABackProject(درون_داد_Array data, درون_داد_Array mean,
                    درون_داد_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
