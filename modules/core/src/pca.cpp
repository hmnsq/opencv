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

#समावेश "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

namespace cv
{

PCA::PCA() {}

PCA::PCA(निवेश_Array तथ्य, निवेश_Array _mean, पूर्णांक flags, पूर्णांक maxComponents)
{
    चालक()(तथ्य, _mean, flags, maxComponents);
}

PCA::PCA(निवेश_Array तथ्य, निवेश_Array _mean, पूर्णांक flags, द्विगुन retainedVariance)
{
    चालक()(तथ्य, _mean, flags, retainedVariance);
}

PCA& PCA::चालक()(निवेश_Array तथ्य_3, निवेश_Array __mean, पूर्णांक flags, पूर्णांक maxComponents)
{
    Mat तथ्य = तथ्य_3.getMat(), _mean = __mean.getMat();
    पूर्णांक covar_flags = cv::COVAR_SCALE;
    पूर्णांक len, in_गिनती;
    आकार mean_sz;

    CV_Assert( तथ्य.channels() == 1 );
    अगर( flags & PCA::तथ्य_AS_COL )
    {
        len = तथ्य.rows;
        in_गिनती = तथ्य.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = आकार(1, len);
    }
    अन्यथा
    {
        len = तथ्य.cols;
        in_गिनती = तथ्य.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = आकार(len, 1);
    }

    पूर्णांक गिनती_2 = std::min(len, in_गिनती), out_गिनती = गिनती_2;
    अगर( maxComponents > 0 )
        out_गिनती = std::min(गिनती_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    अगर( len <= in_गिनती )
        covar_flags |= cv::COVAR_NORMAL;

    पूर्णांक ctype = std::max(CV_32F, तथ्य.depth());
    mean.create( mean_sz, ctype );

    Mat covar( गिनती_2, गिनती_2, ctype );

    अगर( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( तथ्य, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    अगर( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_तथ्य, tmp_mean = repeat(mean, तथ्य.rows/mean.rows, तथ्य.cols/mean.cols);
        अगर( तथ्य.type() != ctype || tmp_mean.तथ्य == mean.तथ्य )
        {
            तथ्य.convertTo( tmp_तथ्य, ctype );
            subtract( tmp_तथ्य, tmp_mean, tmp_तथ्य );
        }
        अन्यथा
        {
            subtract( तथ्य, tmp_mean, tmp_mean );
            tmp_तथ्य = tmp_mean;
        }

        Mat evects1(गिनती_2, len, ctype);
        gemm( eigenvectors, tmp_तथ्य, 1, Mat(), 0, evects1,
            (flags & PCA::तथ्य_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        पूर्णांक i;
        क्रम( i = 0; i < out_गिनती; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    अगर( गिनती_2 > out_गिनती )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,out_गिनती).clone();
        eigenvectors = eigenvectors.rowRange(0,out_गिनती).clone();
    }
    वापस *यह;
}

व्योम PCA::ग_लिखो(FileStorage& fs ) स्थिर
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << mean;
}

व्योम PCA::पढ़ो(स्थिर File_गाँठ& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::पढ़ो(fn["vectors"], eigenvectors);
    cv::पढ़ो(fn["values"], eigenvalues);
    cv::पढ़ो(fn["mean"], mean);
}

ढाँचा <typename T>
पूर्णांक computeCumulativeEnergy(स्थिर Mat& eigenvalues, द्विगुन retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == तथ्य_Type_2<T>::type );

    Mat g(eigenvalues.size(), तथ्य_Type_2<T>::type);

    क्रम(पूर्णांक ig = 0; ig < g.rows; ig++)
    {
        g.at<T>(ig, 0) = 0;
        क्रम(पूर्णांक im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    पूर्णांक L;

    क्रम(L = 0; L < eigenvalues.rows; L++)
    {
        द्विगुन energy = g.at<T>(L, 0) / g.at<T>(g.rows - 1, 0);
        अगर(energy > retainedVariance)
            अवरोध;
    }

    L = std::max(2, L);

    वापस L;
}

PCA& PCA::चालक()(निवेश_Array तथ्य_3, निवेश_Array __mean, पूर्णांक flags, द्विगुन retainedVariance)
{
    Mat तथ्य = तथ्य_3.getMat(), _mean = __mean.getMat();
    पूर्णांक covar_flags = cv::COVAR_SCALE;
    पूर्णांक len, in_गिनती;
    आकार mean_sz;

    CV_Assert( तथ्य.channels() == 1 );
    अगर( flags & PCA::तथ्य_AS_COL )
    {
        len = तथ्य.rows;
        in_गिनती = तथ्य.cols;
        covar_flags |= cv::COVAR_COLS;
        mean_sz = आकार(1, len);
    }
    अन्यथा
    {
        len = तथ्य.cols;
        in_गिनती = तथ्य.rows;
        covar_flags |= cv::COVAR_ROWS;
        mean_sz = आकार(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    पूर्णांक गिनती_2 = std::min(len, in_गिनती);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    अगर( len <= in_गिनती )
        covar_flags |= cv::COVAR_NORMAL;

    पूर्णांक ctype = std::max(CV_32F, तथ्य.depth());
    mean.create( mean_sz, ctype );

    Mat covar( गिनती_2, गिनती_2, ctype );

    अगर( !_mean.empty() )
    {
        CV_Assert( _mean.size() == mean_sz );
        _mean.convertTo(mean, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( तथ्य, covar, mean, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    अगर( !(covar_flags & cv::COVAR_NORMAL) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_तथ्य, tmp_mean = repeat(mean, तथ्य.rows/mean.rows, तथ्य.cols/mean.cols);
        अगर( तथ्य.type() != ctype || tmp_mean.तथ्य == mean.तथ्य )
        {
            तथ्य.convertTo( tmp_तथ्य, ctype );
            subtract( tmp_तथ्य, tmp_mean, tmp_तथ्य );
        }
        अन्यथा
        {
            subtract( तथ्य, tmp_mean, tmp_mean );
            tmp_तथ्य = tmp_mean;
        }

        Mat evects1(गिनती_2, len, ctype);
        gemm( eigenvectors, tmp_तथ्य, 1, Mat(), 0, evects1,
            (flags & PCA::तथ्य_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        पूर्णांक i;
        क्रम( i = 0; i < eigenvectors.rows; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    पूर्णांक L;
    अगर (ctype == CV_32F)
        L = computeCumulativeEnergy<भग्न>(eigenvalues, retainedVariance);
    अन्यथा
        L = computeCumulativeEnergy<द्विगुन>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    वापस *यह;
}

व्योम PCA::project(निवेश_Array तथ्य_3, OutputArray परिणाम) स्थिर
{
    Mat तथ्य = तथ्य_3.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && mean.cols == तथ्य.cols) || (mean.cols == 1 && mean.rows == तथ्य.rows)));
    Mat tmp_तथ्य, tmp_mean = repeat(mean, तथ्य.rows/mean.rows, तथ्य.cols/mean.cols);
    पूर्णांक ctype = mean.type();
    अगर( तथ्य.type() != ctype || tmp_mean.तथ्य == mean.तथ्य )
    {
        तथ्य.convertTo( tmp_तथ्य, ctype );
        subtract( tmp_तथ्य, tmp_mean, tmp_तथ्य );
    }
    अन्यथा
    {
        subtract( तथ्य, tmp_mean, tmp_mean );
        tmp_तथ्य = tmp_mean;
    }
    अगर( mean.rows == 1 )
        gemm( tmp_तथ्य, eigenvectors, 1, Mat(), 0, परिणाम, GEMM_2_T );
    अन्यथा
        gemm( eigenvectors, tmp_तथ्य, 1, Mat(), 0, परिणाम, 0 );
}

Mat PCA::project(निवेश_Array तथ्य) स्थिर
{
    Mat परिणाम;
    project(तथ्य, परिणाम);
    वापस परिणाम;
}

व्योम PCA::backProject(निवेश_Array तथ्य_3, OutputArray परिणाम) स्थिर
{
    Mat तथ्य = तथ्य_3.getMat();
    CV_Assert( !mean.empty() && !eigenvectors.empty() &&
        ((mean.rows == 1 && eigenvectors.rows == तथ्य.cols) ||
         (mean.cols == 1 && eigenvectors.rows == तथ्य.rows)));

    Mat tmp_तथ्य, tmp_mean;
    तथ्य.convertTo(tmp_तथ्य, mean.type());
    अगर( mean.rows == 1 )
    {
        tmp_mean = repeat(mean, तथ्य.rows, 1);
        gemm( tmp_तथ्य, eigenvectors, 1, tmp_mean, 1, परिणाम, 0 );
    }
    अन्यथा
    {
        tmp_mean = repeat(mean, 1, तथ्य.cols);
        gemm( eigenvectors, tmp_तथ्य, 1, tmp_mean, 1, परिणाम, GEMM_1_T );
    }
}

Mat PCA::backProject(निवेश_Array तथ्य) स्थिर
{
    Mat परिणाम;
    backProject(तथ्य, परिणाम);
    वापस परिणाम;
}

}

व्योम cv::PCACompute(निवेश_Array तथ्य, निवेश_Output_Array mean,
                    OutputArray eigenvectors, पूर्णांक maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(तथ्य, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

व्योम cv::PCACompute(निवेश_Array तथ्य, निवेश_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    पूर्णांक maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(तथ्य, mean, 0, maxComponents);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

व्योम cv::PCACompute(निवेश_Array तथ्य, निवेश_Output_Array mean,
                    OutputArray eigenvectors, द्विगुन retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(तथ्य, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
}

व्योम cv::PCACompute(निवेश_Array तथ्य, निवेश_Output_Array mean,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    द्विगुन retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(तथ्य, mean, 0, retainedVariance);
    pca.mean.copyTo(mean);
    pca.eigenvectors.copyTo(eigenvectors);
    pca.eigenvalues.copyTo(eigenvalues);
}

व्योम cv::PCAProject(निवेश_Array तथ्य, निवेश_Array mean,
                    निवेश_Array eigenvectors, OutputArray परिणाम)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(तथ्य, परिणाम);
}

व्योम cv::PCABackProject(निवेश_Array तथ्य, निवेश_Array mean,
                    निवेश_Array eigenvectors, OutputArray परिणाम)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.mean = mean.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.backProject(तथ्य, परिणाम);
}
