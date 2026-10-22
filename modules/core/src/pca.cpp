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

#అన్తర్భావ "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

నామాకాశ cv
{

PCA::PCA() {}

PCA::PCA(ఆదానం_Array దత్తాంశం, ఆదానం_Array సగటు, పూర్ణాంకం flags, పూర్ణాంకం maxComponents)
{
    క్రియాకారకం()(దత్తాంశం, సగటు, flags, maxComponents);
}

PCA::PCA(ఆదానం_Array దత్తాంశం, ఆదానం_Array సగటు, పూర్ణాంకం flags, ద్విగుణ retainedVariance)
{
    క్రియాకారకం()(దత్తాంశం, సగటు, flags, retainedVariance);
}

PCA& PCA::క్రియాకారకం()(ఆదానం_Array దత్తాంశం_3, ఆదానం_Array __mean, పూర్ణాంకం flags, పూర్ణాంకం maxComponents)
{
    Mat దత్తాంశం = దత్తాంశం_3.getMat(), సగటు = __mean.getMat();
    పూర్ణాంకం covar_flags = cv::COVAR_SCALE;
    పూర్ణాంకం len, లోపల_లెక్క;
    కొలత సగటు_sz;

    CV_Assert( దత్తాంశం.channels() == 1 );
    ఒకవేళ( flags & PCA::దత్తాంశం_AS_నిలువు )
    {
        len = దత్తాంశం.అడ్డువరుసలు;
        లోపల_లెక్క = దత్తాంశం.నిలువుల_సంఖ్య;
        covar_flags |= cv::COVAR_నిలువుల_సంఖ్య;
        సగటు_sz = కొలత(1, len);
    }
    లేకపోతే
    {
        len = దత్తాంశం.నిలువుల_సంఖ్య;
        లోపల_లెక్క = దత్తాంశం.అడ్డువరుసలు;
        covar_flags |= cv::COVAR_అడ్డువరుసలు;
        సగటు_sz = కొలత(len, 1);
    }

    పూర్ణాంకం లెక్క_2 = std::min(len, లోపల_లెక్క), బహిర్గతం_లెక్క = లెక్క_2;
    ఒకవేళ( maxComponents > 0 )
        బహిర్గతం_లెక్క = std::min(లెక్క_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ఒకవేళ( len <= లోపల_లెక్క )
        covar_flags |= cv::COVAR_సాధారణ;

    పూర్ణాంకం ctype = std::max(CV_32F, దత్తాంశం.లోతు());
    సగటు_2.create( సగటు_sz, ctype );

    Mat covar( లెక్క_2, లెక్క_2, ctype );

    ఒకవేళ( !సగటు.empty() )
    {
        CV_Assert( సగటు.size() == సగటు_sz );
        సగటు.convert_గమ్యం(సగటు_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_మాతృక( దత్తాంశం, covar, సగటు_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ఒకవేళ( !(covar_flags & cv::COVAR_సాధారణ) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_దత్తాంశం, tmp_సగటు = repeat(సగటు_2, దత్తాంశం.అడ్డువరుసలు/సగటు_2.అడ్డువరుసలు, దత్తాంశం.నిలువుల_సంఖ్య/సగటు_2.నిలువుల_సంఖ్య);
        ఒకవేళ( దత్తాంశం.type() != ctype || tmp_సగటు.దత్తాంశం == సగటు_2.దత్తాంశం )
        {
            దత్తాంశం.convert_గమ్యం( tmp_దత్తాంశం, ctype );
            వ్యవకలనం( tmp_దత్తాంశం, tmp_సగటు, tmp_దత్తాంశం );
        }
        లేకపోతే
        {
            వ్యవకలనం( దత్తాంశం, tmp_సగటు, tmp_సగటు );
            tmp_దత్తాంశం = tmp_సగటు;
        }

        Mat evects1(లెక్క_2, len, ctype);
        gemm( eigenvectors, tmp_దత్తాంశం, 1, Mat(), 0, evects1,
            (flags & PCA::దత్తాంశం_AS_నిలువు) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        పూర్ణాంకం i;
        కొరకు( i = 0; i < బహిర్గతం_లెక్క; i++ )
        {
            Mat vec = eigenvectors.అడ్డువరుస(i);
            normalize(vec, vec);
        }
    }

    ఒకవేళ( లెక్క_2 > బహిర్గతం_లెక్క )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.అడ్డువరుస_Range_2(0,బహిర్గతం_లెక్క).clone();
        eigenvectors = eigenvectors.అడ్డువరుస_Range_2(0,బహిర్గతం_లెక్క).clone();
    }
    తిరిగి *ఇది;
}

శూన్య PCA::లిఖ(FileStorage& fs ) స్థిరరాశి
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << సగటు_2;
}

శూన్య PCA::పఠ(స్థిరరాశి File_కణుపు& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::పఠ(fn["vectors"], eigenvectors);
    cv::పఠ(fn["values"], eigenvalues);
    cv::పఠ(fn["mean"], సగటు_2);
}

మూస <రకం_పేరు T>
పూర్ణాంకం computeCumulativeEnergy(స్థిరరాశి Mat& eigenvalues, ద్విగుణ retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == దత్తాంశం_Type_2<T>::type );

    Mat g(eigenvalues.size(), దత్తాంశం_Type_2<T>::type);

    కొరకు(పూర్ణాంకం ig = 0; ig < g.అడ్డువరుసలు; ig++)
    {
        g.at<T>(ig, 0) = 0;
        కొరకు(పూర్ణాంకం im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    పూర్ణాంకం L;

    కొరకు(L = 0; L < eigenvalues.అడ్డువరుసలు; L++)
    {
        ద్విగుణ energy = g.at<T>(L, 0) / g.at<T>(g.అడ్డువరుసలు - 1, 0);
        ఒకవేళ(energy > retainedVariance)
            ఆపు;
    }

    L = std::max(2, L);

    తిరిగి L;
}

PCA& PCA::క్రియాకారకం()(ఆదానం_Array దత్తాంశం_3, ఆదానం_Array __mean, పూర్ణాంకం flags, ద్విగుణ retainedVariance)
{
    Mat దత్తాంశం = దత్తాంశం_3.getMat(), సగటు = __mean.getMat();
    పూర్ణాంకం covar_flags = cv::COVAR_SCALE;
    పూర్ణాంకం len, లోపల_లెక్క;
    కొలత సగటు_sz;

    CV_Assert( దత్తాంశం.channels() == 1 );
    ఒకవేళ( flags & PCA::దత్తాంశం_AS_నిలువు )
    {
        len = దత్తాంశం.అడ్డువరుసలు;
        లోపల_లెక్క = దత్తాంశం.నిలువుల_సంఖ్య;
        covar_flags |= cv::COVAR_నిలువుల_సంఖ్య;
        సగటు_sz = కొలత(1, len);
    }
    లేకపోతే
    {
        len = దత్తాంశం.నిలువుల_సంఖ్య;
        లోపల_లెక్క = దత్తాంశం.అడ్డువరుసలు;
        covar_flags |= cv::COVAR_అడ్డువరుసలు;
        సగటు_sz = కొలత(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    పూర్ణాంకం లెక్క_2 = std::min(len, లోపల_లెక్క);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ఒకవేళ( len <= లోపల_లెక్క )
        covar_flags |= cv::COVAR_సాధారణ;

    పూర్ణాంకం ctype = std::max(CV_32F, దత్తాంశం.లోతు());
    సగటు_2.create( సగటు_sz, ctype );

    Mat covar( లెక్క_2, లెక్క_2, ctype );

    ఒకవేళ( !సగటు.empty() )
    {
        CV_Assert( సగటు.size() == సగటు_sz );
        సగటు.convert_గమ్యం(సగటు_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_మాతృక( దత్తాంశం, covar, సగటు_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ఒకవేళ( !(covar_flags & cv::COVAR_సాధారణ) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_దత్తాంశం, tmp_సగటు = repeat(సగటు_2, దత్తాంశం.అడ్డువరుసలు/సగటు_2.అడ్డువరుసలు, దత్తాంశం.నిలువుల_సంఖ్య/సగటు_2.నిలువుల_సంఖ్య);
        ఒకవేళ( దత్తాంశం.type() != ctype || tmp_సగటు.దత్తాంశం == సగటు_2.దత్తాంశం )
        {
            దత్తాంశం.convert_గమ్యం( tmp_దత్తాంశం, ctype );
            వ్యవకలనం( tmp_దత్తాంశం, tmp_సగటు, tmp_దత్తాంశం );
        }
        లేకపోతే
        {
            వ్యవకలనం( దత్తాంశం, tmp_సగటు, tmp_సగటు );
            tmp_దత్తాంశం = tmp_సగటు;
        }

        Mat evects1(లెక్క_2, len, ctype);
        gemm( eigenvectors, tmp_దత్తాంశం, 1, Mat(), 0, evects1,
            (flags & PCA::దత్తాంశం_AS_నిలువు) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        పూర్ణాంకం i;
        కొరకు( i = 0; i < eigenvectors.అడ్డువరుసలు; i++ )
        {
            Mat vec = eigenvectors.అడ్డువరుస(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    పూర్ణాంకం L;
    ఒకవేళ (ctype == CV_32F)
        L = computeCumulativeEnergy<దశాంశం>(eigenvalues, retainedVariance);
    లేకపోతే
        L = computeCumulativeEnergy<ద్విగుణ>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.అడ్డువరుస_Range_2(0,L).clone();
    eigenvectors = eigenvectors.అడ్డువరుస_Range_2(0,L).clone();

    తిరిగి *ఇది;
}

శూన్య PCA::project(ఆదానం_Array దత్తాంశం_3, OutputArray ఫలితం) స్థిరరాశి
{
    Mat దత్తాంశం = దత్తాంశం_3.getMat();
    CV_Assert( !సగటు_2.empty() && !eigenvectors.empty() &&
        ((సగటు_2.అడ్డువరుసలు == 1 && సగటు_2.నిలువుల_సంఖ్య == దత్తాంశం.నిలువుల_సంఖ్య) || (సగటు_2.నిలువుల_సంఖ్య == 1 && సగటు_2.అడ్డువరుసలు == దత్తాంశం.అడ్డువరుసలు)));
    Mat tmp_దత్తాంశం, tmp_సగటు = repeat(సగటు_2, దత్తాంశం.అడ్డువరుసలు/సగటు_2.అడ్డువరుసలు, దత్తాంశం.నిలువుల_సంఖ్య/సగటు_2.నిలువుల_సంఖ్య);
    పూర్ణాంకం ctype = సగటు_2.type();
    ఒకవేళ( దత్తాంశం.type() != ctype || tmp_సగటు.దత్తాంశం == సగటు_2.దత్తాంశం )
    {
        దత్తాంశం.convert_గమ్యం( tmp_దత్తాంశం, ctype );
        వ్యవకలనం( tmp_దత్తాంశం, tmp_సగటు, tmp_దత్తాంశం );
    }
    లేకపోతే
    {
        వ్యవకలనం( దత్తాంశం, tmp_సగటు, tmp_సగటు );
        tmp_దత్తాంశం = tmp_సగటు;
    }
    ఒకవేళ( సగటు_2.అడ్డువరుసలు == 1 )
        gemm( tmp_దత్తాంశం, eigenvectors, 1, Mat(), 0, ఫలితం, GEMM_2_T );
    లేకపోతే
        gemm( eigenvectors, tmp_దత్తాంశం, 1, Mat(), 0, ఫలితం, 0 );
}

Mat PCA::project(ఆదానం_Array దత్తాంశం) స్థిరరాశి
{
    Mat ఫలితం;
    project(దత్తాంశం, ఫలితం);
    తిరిగి ఫలితం;
}

శూన్య PCA::వెనుక_Project(ఆదానం_Array దత్తాంశం_3, OutputArray ఫలితం) స్థిరరాశి
{
    Mat దత్తాంశం = దత్తాంశం_3.getMat();
    CV_Assert( !సగటు_2.empty() && !eigenvectors.empty() &&
        ((సగటు_2.అడ్డువరుసలు == 1 && eigenvectors.అడ్డువరుసలు == దత్తాంశం.నిలువుల_సంఖ్య) ||
         (సగటు_2.నిలువుల_సంఖ్య == 1 && eigenvectors.అడ్డువరుసలు == దత్తాంశం.అడ్డువరుసలు)));

    Mat tmp_దత్తాంశం, tmp_సగటు;
    దత్తాంశం.convert_గమ్యం(tmp_దత్తాంశం, సగటు_2.type());
    ఒకవేళ( సగటు_2.అడ్డువరుసలు == 1 )
    {
        tmp_సగటు = repeat(సగటు_2, దత్తాంశం.అడ్డువరుసలు, 1);
        gemm( tmp_దత్తాంశం, eigenvectors, 1, tmp_సగటు, 1, ఫలితం, 0 );
    }
    లేకపోతే
    {
        tmp_సగటు = repeat(సగటు_2, 1, దత్తాంశం.నిలువుల_సంఖ్య);
        gemm( eigenvectors, tmp_దత్తాంశం, 1, tmp_సగటు, 1, ఫలితం, GEMM_1_T );
    }
}

Mat PCA::వెనుక_Project(ఆదానం_Array దత్తాంశం) స్థిరరాశి
{
    Mat ఫలితం;
    వెనుక_Project(దత్తాంశం, ఫలితం);
    తిరిగి ఫలితం;
}

}

శూన్య cv::PCACompute(ఆదానం_Array దత్తాంశం, ఆదానం_Output_Array సగటు_2,
                    OutputArray eigenvectors, పూర్ణాంకం maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(దత్తాంశం, సగటు_2, 0, maxComponents);
    pca.సగటు_2.copy_గమ్యం(సగటు_2);
    pca.eigenvectors.copy_గమ్యం(eigenvectors);
}

శూన్య cv::PCACompute(ఆదానం_Array దత్తాంశం, ఆదానం_Output_Array సగటు_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    పూర్ణాంకం maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(దత్తాంశం, సగటు_2, 0, maxComponents);
    pca.సగటు_2.copy_గమ్యం(సగటు_2);
    pca.eigenvectors.copy_గమ్యం(eigenvectors);
    pca.eigenvalues.copy_గమ్యం(eigenvalues);
}

శూన్య cv::PCACompute(ఆదానం_Array దత్తాంశం, ఆదానం_Output_Array సగటు_2,
                    OutputArray eigenvectors, ద్విగుణ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(దత్తాంశం, సగటు_2, 0, retainedVariance);
    pca.సగటు_2.copy_గమ్యం(సగటు_2);
    pca.eigenvectors.copy_గమ్యం(eigenvectors);
}

శూన్య cv::PCACompute(ఆదానం_Array దత్తాంశం, ఆదానం_Output_Array సగటు_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ద్విగుణ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(దత్తాంశం, సగటు_2, 0, retainedVariance);
    pca.సగటు_2.copy_గమ్యం(సగటు_2);
    pca.eigenvectors.copy_గమ్యం(eigenvectors);
    pca.eigenvalues.copy_గమ్యం(eigenvalues);
}

శూన్య cv::PCAProject(ఆదానం_Array దత్తాంశం, ఆదానం_Array సగటు_2,
                    ఆదానం_Array eigenvectors, OutputArray ఫలితం)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.సగటు_2 = సగటు_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(దత్తాంశం, ఫలితం);
}

శూన్య cv::PCABackProject(ఆదానం_Array దత్తాంశం, ఆదానం_Array సగటు_2,
                    ఆదానం_Array eigenvectors, OutputArray ఫలితం)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.సగటు_2 = సగటు_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.వెనుక_Project(దత్తాంశం, ఫలితం);
}
