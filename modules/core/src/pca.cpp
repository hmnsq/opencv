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

PCA::PCA(आदान_Array data, आदान_Array _mean, पूर्णांक flags, पूर्णांक maxComponents)
{
    चालक()(data, _mean, flags, maxComponents);
}

PCA::PCA(आदान_Array data, आदान_Array _mean, पूर्णांक flags, द्विगुण retainedVariance)
{
    चालक()(data, _mean, flags, retainedVariance);
}

PCA& PCA::चालक()(आदान_Array _data, आदान_Array __mean, पूर्णांक flags, पूर्णांक maxComponents)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    पूर्णांक covar_flags = cv::COVAR_SCALE;
    पूर्णांक len, in_मोजणी;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    जर( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_मोजणी = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    अन्यथा
    {
        len = data.cols;
        in_मोजणी = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    पूर्णांक मोजणी_2 = std::min(len, in_मोजणी), out_मोजणी = मोजणी_2;
    जर( maxComponents > 0 )
        out_मोजणी = std::min(मोजणी_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    जर( len <= in_मोजणी )
        covar_flags |= cv::COVAR_NORMAL;

    पूर्णांक ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( मोजणी_2, मोजणी_2, ctype );

    जर( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    जर( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        जर( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        अन्यथा
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(मोजणी_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        पूर्णांक i;
        साठी( i = 0; i < out_मोजणी; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    जर( मोजणी_2 > out_मोजणी )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_मोजणी).clone();
        eigenvectors = eigenvectors.rowRange(0,out_मोजणी).clone();
    }
    परत *हे;
}

शून्य PCA::लिख(FileStorage& fs ) स्थिरांक
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

शून्य PCA::पठ(स्थिरांक File_गाठ& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::पठ(fn["vectors"], eigenvectors);
    cv::पठ(fn["values"], eigenvalues);
    cv::पठ(fn["mean"], mean);
}

ढाँचा <typename T>
पूर्णांक computeCumulativeEnergy(स्थिरांक Mat& eigenvalues, द्विगुण retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == DataType<T>::type );

    Mat g(eigenvalues.size(), DataType<T>::type);

    साठी(पूर्णांक ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        साठी(पूर्णांक im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    पूर्णांक L;

    साठी(L = 0; L < eigenvalues.rows; L++)
    {
        द्विगुण energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        जर(energy > retainedVariance)
            थांबा;
    }

    L = std::max(2, L);

    परत L;
}

PCA& PCA::चालक()(आदान_Array _data, आदान_Array __mean, पूर्णांक flags, द्विगुण retainedVariance)
{
    Mat data = _data.getMat(), _mean = __mean.getMat();
    पूर्णांक covar_flags = cv::COVAR_SCALE;
    पूर्णांक len, in_मोजणी;
    Size mean_sz;

    CV_Assert( data.channels() == 1 );
    जर( flags & PCA::DATA_AS_COL )
    {
        len = data.rows;
        in_मोजणी = data.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = Size(1, len);
    }
    अन्यथा
    {
        len = data.cols;
        in_मोजणी = data.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = Size(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    पूर्णांक मोजणी_2 = std::min(len, in_मोजणी);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    जर( len <= in_मोजणी )
        covar_flags |= cv::COVAR_NORMAL;

    पूर्णांक ctype = std::max(CV_32F, data.depth());
    mean.create( mean_sz, ctype );

    Mat covar( मोजणी_2, मोजणी_2, ctype );

    जर( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( data, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    जर( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
        जर( data.type() != ctype || tmp_mean.data == mean.data )
        {
            data.convertTo( tmp_data, ctype );
            subtract( tmp_data, tmp_mean, tmp_data );
        }
        अन्यथा
        {
            subtract( data, tmp_mean, tmp_mean );
            tmp_data = tmp_mean;
        }

        Mat evects1(मोजणी_2, len, ctype);
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, evects1,
            (flags & PCA::DATA_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        पूर्णांक i;
        साठी( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    पूर्णांक L;
    जर (ctype == CV_32F)
        L = computeCumulativeEnergy<दशांश>(eigenvalues, retainedVariance);
    अन्यथा
        L = computeCumulativeEnergy<द्विगुण>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    परत *हे;
}

शून्य PCA::project(आदान_Array _data, OutputArray result) स्थिरांक
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == data.cols) || (mean.cols == 1 && mean.rows == data.rows)));
    Mat tmp_data, tmp_mean = repeat(mean, data.rows/mean.rows, data.cols/mean.cols);
    पूर्णांक ctype = mean.type();
    जर( data.type() != ctype || tmp_mean.data == mean.data )
    {
        data.convertTo( tmp_data, ctype );
        subtract( tmp_data, tmp_mean, tmp_data );
    }
    अन्यथा
    {
        subtract( data, tmp_mean, tmp_mean );
        tmp_data = tmp_mean;
    }
    जर( mean.rows == 1 )
        gemm( tmp_data, eigenvectors, 1, Mat(), 0, result, GEMM_2_T );
    अन्यथा
        gemm( eigenvectors, tmp_data, 1, Mat(), 0, result, 0 );
}

Mat PCA::project(आदान_Array data) स्थिरांक
{
    Mat result;
    project(data, result);
    परत result;
}

शून्य PCA::backProject(आदान_Array _data, OutputArray result) स्थिरांक
{
    Mat data = _data.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == data.cols) ||
         (mean.cols == 1 && eigenvectors.rows == data.rows)));

    Mat tmp_data, tmp_mean;
    data.convertTo(tmp_data, mean.type());
    जर( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, data.rows, 1);
        gemm( tmp_data, eigenvectors, 1, tmp_mean, 1, result, 0 );
    }
    अन्यथा
    {
        tmp_mean = repeat(mean, 1, data.cols);
        gemm( eigenvectors, tmp_data, 1, tmp_mean, 1, result, GEMM_1_T );
    }
}

Mat PCA::backProject(आदान_Array data) स्थिरांक
{
    Mat result;
    backProject(data, result);
    परत result;
}

}

शून्य cv::PCACompute(आदान_Array data, आदान_Output_Array mean,
                    OutputArray eigenvectors, पूर्णांक maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

शून्य cv::PCACompute(आदान_Array data, आदान_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    पूर्णांक maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

शून्य cv::PCACompute(आदान_Array data, आदान_Output_Array mean,
                    OutputArray eigenvectors, द्विगुण retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(data, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

शून्य cv::PCACompute(आदान_Array data, आदान_Output_Array mean,
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

शून्य cv::PCAProject(आदान_Array data, आदान_Array mean,
                    आदान_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(data, result);
}

शून्य cv::PCABackProject(आदान_Array data, आदान_Array mean,
                    आदान_Array eigenvectors, OutputArray result)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(data, result);
}
