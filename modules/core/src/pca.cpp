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

PCA::PCA(निवेशः_Array दत्त, निवेशः_Array माध्यम्, पूर्णाङ्क flags, पूर्णाङ्क maxComponents)
{
    संक्रियकः()(दत्त, माध्यम्, flags, maxComponents);
}

PCA::PCA(निवेशः_Array दत्त, निवेशः_Array माध्यम्, पूर्णाङ्क flags, द्विगुण retainedVariance)
{
    संक्रियकः()(दत्त, माध्यम्, flags, retainedVariance);
}

PCA& PCA::संक्रियकः()(निवेशः_Array दत्त_3, निवेशः_Array __mean, पूर्णाङ्क flags, पूर्णाङ्क maxComponents)
{
    Mat दत्त = दत्त_3.getMat(), माध्यम् = __mean.getMat();
    पूर्णाङ्क covar_flags = cv::COVAR_SCALE;
    पूर्णाङ्क len, अन्तर्_गणनम्;
    आकार माध्यम्_sz;

    CV_Assert( दत्त.channels() == 1 );
    यदि( flags & PCA::दत्त_AS_COL )
    {
        len = दत्त.पङ्क्तिकाः;
        अन्तर्_गणनम् = दत्त.स्तम्भसङ्ख्या;
        covar_flags |= cv::COVAR_स्तम्भसङ्ख्या;
        माध्यम्_sz = आकार(1, len);
    }
    अन्यथा
    {
        len = दत्त.स्तम्भसङ्ख्या;
        अन्तर्_गणनम् = दत्त.पङ्क्तिकाः;
        covar_flags |= cv::COVAR_पङ्क्तिकाः;
        माध्यम्_sz = आकार(len, 1);
    }

    पूर्णाङ्क गणनम्_2 = std::min(len, अन्तर्_गणनम्), बहिर्गमनम्_गणनम् = गणनम्_2;
    यदि( maxComponents > 0 )
        बहिर्गमनम्_गणनम् = std::min(गणनम्_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    यदि( len <= अन्तर्_गणनम् )
        covar_flags |= cv::COVAR_प्रसामान्यम्;

    पूर्णाङ्क ctype = std::max(CV_32F, दत्त.गभीरता());
    माध्यम्_2.create( माध्यम्_sz, ctype );

    Mat covar( गणनम्_2, गणनम्_2, ctype );

    यदि( !माध्यम्.empty() )
    {
        CV_Assert( माध्यम्.size() == माध्यम्_sz );
        माध्यम्.convert_गन्तव्यम्(माध्यम्_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( दत्त, covar, माध्यम्_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    यदि( !(covar_flags & cv::COVAR_प्रसामान्यम्) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_दत्त, tmp_माध्यम् = repeat(माध्यम्_2, दत्त.पङ्क्तिकाः/माध्यम्_2.पङ्क्तिकाः, दत्त.स्तम्भसङ्ख्या/माध्यम्_2.स्तम्भसङ्ख्या);
        यदि( दत्त.type() != ctype || tmp_माध्यम्.दत्त == माध्यम्_2.दत्त )
        {
            दत्त.convert_गन्तव्यम्( tmp_दत्त, ctype );
            व्यवकलय( tmp_दत्त, tmp_माध्यम्, tmp_दत्त );
        }
        अन्यथा
        {
            व्यवकलय( दत्त, tmp_माध्यम्, tmp_माध्यम् );
            tmp_दत्त = tmp_माध्यम्;
        }

        Mat evects1(गणनम्_2, len, ctype);
        gemm( eigenvectors, tmp_दत्त, 1, Mat(), 0, evects1,
            (flags & PCA::दत्त_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        पूर्णाङ्क i;
        प्रति( i = 0; i < बहिर्गमनम्_गणनम्; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    यदि( गणनम्_2 > बहिर्गमनम्_गणनम् )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.rowRange(0,बहिर्गमनम्_गणनम्).clone();
        eigenvectors = eigenvectors.rowRange(0,बहिर्गमनम्_गणनम्).clone();
    }
    प्रत्यागम *एतत्;
}

शून्य PCA::लिख(FileStorage& fs ) ध्रुव
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << माध्यम्_2;
}

शून्य PCA::पठ(ध्रुव File_ग्रन्थिः& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::पठ(fn["vectors"], eigenvectors);
    cv::पठ(fn["values"], eigenvalues);
    cv::पठ(fn["mean"], माध्यम्_2);
}

प्रतिरूपम् <प्रकारनाम T>
पूर्णाङ्क computeCumulativeEnergy(ध्रुव Mat& eigenvalues, द्विगुण retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == दत्त_Type_2<T>::type );

    Mat g(eigenvalues.size(), दत्त_Type_2<T>::type);

    प्रति(पूर्णाङ्क ig = 0; ig < g.पङ्क्तिकाः; ig++)
    {
        g.at<T>(ig, 0) = 0;
        प्रति(पूर्णाङ्क im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    पूर्णाङ्क L;

    प्रति(L = 0; L < eigenvalues.पङ्क्तिकाः; L++)
    {
        द्विगुण energy = g.at<T>(L, 0) / g.at<T>(g.पङ्क्तिकाः - 1, 0);
        यदि(energy > retainedVariance)
            भङ्ग;
    }

    L = std::max(2, L);

    प्रत्यागम L;
}

PCA& PCA::संक्रियकः()(निवेशः_Array दत्त_3, निवेशः_Array __mean, पूर्णाङ्क flags, द्विगुण retainedVariance)
{
    Mat दत्त = दत्त_3.getMat(), माध्यम् = __mean.getMat();
    पूर्णाङ्क covar_flags = cv::COVAR_SCALE;
    पूर्णाङ्क len, अन्तर्_गणनम्;
    आकार माध्यम्_sz;

    CV_Assert( दत्त.channels() == 1 );
    यदि( flags & PCA::दत्त_AS_COL )
    {
        len = दत्त.पङ्क्तिकाः;
        अन्तर्_गणनम् = दत्त.स्तम्भसङ्ख्या;
        covar_flags |= cv::COVAR_स्तम्भसङ्ख्या;
        माध्यम्_sz = आकार(1, len);
    }
    अन्यथा
    {
        len = दत्त.स्तम्भसङ्ख्या;
        अन्तर्_गणनम् = दत्त.पङ्क्तिकाः;
        covar_flags |= cv::COVAR_पङ्क्तिकाः;
        माध्यम्_sz = आकार(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    पूर्णाङ्क गणनम्_2 = std::min(len, अन्तर्_गणनम्);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    यदि( len <= अन्तर्_गणनम् )
        covar_flags |= cv::COVAR_प्रसामान्यम्;

    पूर्णाङ्क ctype = std::max(CV_32F, दत्त.गभीरता());
    माध्यम्_2.create( माध्यम्_sz, ctype );

    Mat covar( गणनम्_2, गणनम्_2, ctype );

    यदि( !माध्यम्.empty() )
    {
        CV_Assert( माध्यम्.size() == माध्यम्_sz );
        माध्यम्.convert_गन्तव्यम्(माध्यम्_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calcCovarMatrix( दत्त, covar, माध्यम्_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    यदि( !(covar_flags & cv::COVAR_प्रसामान्यम्) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_दत्त, tmp_माध्यम् = repeat(माध्यम्_2, दत्त.पङ्क्तिकाः/माध्यम्_2.पङ्क्तिकाः, दत्त.स्तम्भसङ्ख्या/माध्यम्_2.स्तम्भसङ्ख्या);
        यदि( दत्त.type() != ctype || tmp_माध्यम्.दत्त == माध्यम्_2.दत्त )
        {
            दत्त.convert_गन्तव्यम्( tmp_दत्त, ctype );
            व्यवकलय( tmp_दत्त, tmp_माध्यम्, tmp_दत्त );
        }
        अन्यथा
        {
            व्यवकलय( दत्त, tmp_माध्यम्, tmp_माध्यम् );
            tmp_दत्त = tmp_माध्यम्;
        }

        Mat evects1(गणनम्_2, len, ctype);
        gemm( eigenvectors, tmp_दत्त, 1, Mat(), 0, evects1,
            (flags & PCA::दत्त_AS_COL) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        पूर्णाङ्क i;
        प्रति( i = 0; i < eigenvectors.पङ्क्तिकाः; i++ )
        {
            Mat vec = eigenvectors.row(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    पूर्णाङ्क L;
    यदि (ctype == CV_32F)
        L = computeCumulativeEnergy<भिन्न>(eigenvalues, retainedVariance);
    अन्यथा
        L = computeCumulativeEnergy<द्विगुण>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.rowRange(0,L).clone();
    eigenvectors = eigenvectors.rowRange(0,L).clone();

    प्रत्यागम *एतत्;
}

शून्य PCA::project(निवेशः_Array दत्त_3, OutputArray परिणाम) ध्रुव
{
    Mat दत्त = दत्त_3.getMat();
    CV_Assert( !माध्यम्_2.empty() && !eigenvectors.empty() &&
        ((माध्यम्_2.पङ्क्तिकाः == 1 && माध्यम्_2.स्तम्भसङ्ख्या == दत्त.स्तम्भसङ्ख्या) || (माध्यम्_2.स्तम्भसङ्ख्या == 1 && माध्यम्_2.पङ्क्तिकाः == दत्त.पङ्क्तिकाः)));
    Mat tmp_दत्त, tmp_माध्यम् = repeat(माध्यम्_2, दत्त.पङ्क्तिकाः/माध्यम्_2.पङ्क्तिकाः, दत्त.स्तम्भसङ्ख्या/माध्यम्_2.स्तम्भसङ्ख्या);
    पूर्णाङ्क ctype = माध्यम्_2.type();
    यदि( दत्त.type() != ctype || tmp_माध्यम्.दत्त == माध्यम्_2.दत्त )
    {
        दत्त.convert_गन्तव्यम्( tmp_दत्त, ctype );
        व्यवकलय( tmp_दत्त, tmp_माध्यम्, tmp_दत्त );
    }
    अन्यथा
    {
        व्यवकलय( दत्त, tmp_माध्यम्, tmp_माध्यम् );
        tmp_दत्त = tmp_माध्यम्;
    }
    यदि( माध्यम्_2.पङ्क्तिकाः == 1 )
        gemm( tmp_दत्त, eigenvectors, 1, Mat(), 0, परिणाम, GEMM_2_T );
    अन्यथा
        gemm( eigenvectors, tmp_दत्त, 1, Mat(), 0, परिणाम, 0 );
}

Mat PCA::project(निवेशः_Array दत्त) ध्रुव
{
    Mat परिणाम;
    project(दत्त, परिणाम);
    प्रत्यागम परिणाम;
}

शून्य PCA::पृष्ठभागः_Project(निवेशः_Array दत्त_3, OutputArray परिणाम) ध्रुव
{
    Mat दत्त = दत्त_3.getMat();
    CV_Assert( !माध्यम्_2.empty() && !eigenvectors.empty() &&
        ((माध्यम्_2.पङ्क्तिकाः == 1 && eigenvectors.पङ्क्तिकाः == दत्त.स्तम्भसङ्ख्या) ||
         (माध्यम्_2.स्तम्भसङ्ख्या == 1 && eigenvectors.पङ्क्तिकाः == दत्त.पङ्क्तिकाः)));

    Mat tmp_दत्त, tmp_माध्यम्;
    दत्त.convert_गन्तव्यम्(tmp_दत्त, माध्यम्_2.type());
    यदि( माध्यम्_2.पङ्क्तिकाः == 1 )
    {
        tmp_माध्यम् = repeat(माध्यम्_2, दत्त.पङ्क्तिकाः, 1);
        gemm( tmp_दत्त, eigenvectors, 1, tmp_माध्यम्, 1, परिणाम, 0 );
    }
    अन्यथा
    {
        tmp_माध्यम् = repeat(माध्यम्_2, 1, दत्त.स्तम्भसङ्ख्या);
        gemm( eigenvectors, tmp_दत्त, 1, tmp_माध्यम्, 1, परिणाम, GEMM_1_T );
    }
}

Mat PCA::पृष्ठभागः_Project(निवेशः_Array दत्त) ध्रुव
{
    Mat परिणाम;
    पृष्ठभागः_Project(दत्त, परिणाम);
    प्रत्यागम परिणाम;
}

}

शून्य cv::PCACompute(निवेशः_Array दत्त, निवेशः_Output_Array माध्यम्_2,
                    OutputArray eigenvectors, पूर्णाङ्क maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(दत्त, माध्यम्_2, 0, maxComponents);
    pca.माध्यम्_2.copy_गन्तव्यम्(माध्यम्_2);
    pca.eigenvectors.copy_गन्तव्यम्(eigenvectors);
}

शून्य cv::PCACompute(निवेशः_Array दत्त, निवेशः_Output_Array माध्यम्_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    पूर्णाङ्क maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(दत्त, माध्यम्_2, 0, maxComponents);
    pca.माध्यम्_2.copy_गन्तव्यम्(माध्यम्_2);
    pca.eigenvectors.copy_गन्तव्यम्(eigenvectors);
    pca.eigenvalues.copy_गन्तव्यम्(eigenvalues);
}

शून्य cv::PCACompute(निवेशः_Array दत्त, निवेशः_Output_Array माध्यम्_2,
                    OutputArray eigenvectors, द्विगुण retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(दत्त, माध्यम्_2, 0, retainedVariance);
    pca.माध्यम्_2.copy_गन्तव्यम्(माध्यम्_2);
    pca.eigenvectors.copy_गन्तव्यम्(eigenvectors);
}

शून्य cv::PCACompute(निवेशः_Array दत्त, निवेशः_Output_Array माध्यम्_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    द्विगुण retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(दत्त, माध्यम्_2, 0, retainedVariance);
    pca.माध्यम्_2.copy_गन्तव्यम्(माध्यम्_2);
    pca.eigenvectors.copy_गन्तव्यम्(eigenvectors);
    pca.eigenvalues.copy_गन्तव्यम्(eigenvalues);
}

शून्य cv::PCAProject(निवेशः_Array दत्त, निवेशः_Array माध्यम्_2,
                    निवेशः_Array eigenvectors, OutputArray परिणाम)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.माध्यम्_2 = माध्यम्_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(दत्त, परिणाम);
}

शून्य cv::PCABackProject(निवेशः_Array दत्त, निवेशः_Array माध्यम्_2,
                    निवेशः_Array eigenvectors, OutputArray परिणाम)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.माध्यम्_2 = माध्यम्_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.पृष्ठभागः_Project(दत्त, परिणाम);
}
