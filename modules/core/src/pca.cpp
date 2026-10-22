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

PCA::PCA(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ, ಒಳಹರಿವು_Array ಸರಾಸರಿ, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ maxComponents)
{
    ಕ್ರಿಯಾಕಾರಕ()(ದತ್ತಾಂಶ, ಸರಾಸರಿ, flags, maxComponents);
}

PCA::PCA(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ, ಒಳಹರಿವು_Array ಸರಾಸರಿ, ಪೂರ್ಣಾಂಕ flags, ದ್ವಿಗುಣ retainedVariance)
{
    ಕ್ರಿಯಾಕಾರಕ()(ದತ್ತಾಂಶ, ಸರಾಸರಿ, flags, retainedVariance);
}

PCA& PCA::ಕ್ರಿಯಾಕಾರಕ()(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ_3, ಒಳಹರಿವು_Array __mean, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ maxComponents)
{
    Mat ದತ್ತಾಂಶ = ದತ್ತಾಂಶ_3.getMat(), ಸರಾಸರಿ = __mean.getMat();
    ಪೂರ್ಣಾಂಕ covar_flags = cv::COVAR_SCALE;
    ಪೂರ್ಣಾಂಕ len, ಒಳ_ಎಣಿಕೆ;
    ಗಾತ್ರ_2 ಸರಾಸರಿ_sz;

    CV_Assert( ದತ್ತಾಂಶ.channels() == 1 );
    ಆದರೆ( flags & PCA::ದತ್ತಾಂಶ_AS_ಲಂಬಸಾಲು )
    {
        len = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು;
        ಒಳ_ಎಣಿಕೆ = ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        covar_flags |= cv::COVAR_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ಸರಾಸರಿ_sz = ಗಾತ್ರ_2(1, len);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        len = ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ಒಳ_ಎಣಿಕೆ = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು;
        covar_flags |= cv::COVAR_ಅಡ್ಡಸಾಲುಗಳು;
        ಸರಾಸರಿ_sz = ಗಾತ್ರ_2(len, 1);
    }

    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = std::min(len, ಒಳ_ಎಣಿಕೆ), ಹೊರಹೋಗು_ಎಣಿಕೆ = ಎಣಿಕೆ_3;
    ಆದರೆ( maxComponents > 0 )
        ಹೊರಹೋಗು_ಎಣಿಕೆ = std::min(ಎಣಿಕೆ_3, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ಆದರೆ( len <= ಒಳ_ಎಣಿಕೆ )
        covar_flags |= cv::COVAR_ಸಾಮಾನ್ಯ;

    ಪೂರ್ಣಾಂಕ ctype = std::max(CV_32F, ದತ್ತಾಂಶ.ಆಳ());
    ಸರಾಸರಿ_2.create( ಸರಾಸರಿ_sz, ctype );

    Mat covar( ಎಣಿಕೆ_3, ಎಣಿಕೆ_3, ctype );

    ಆದರೆ( !ಸರಾಸರಿ.empty() )
    {
        CV_Assert( ಸರಾಸರಿ.size() == ಸರಾಸರಿ_sz );
        ಸರಾಸರಿ.convert_ಗಮ್ಯ(ಸರಾಸರಿ_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ಮಾತೃಕೆ( ದತ್ತಾಂಶ, covar, ಸರಾಸರಿ_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ಆದರೆ( !(covar_flags & cv::COVAR_ಸಾಮಾನ್ಯ) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ = repeat(ಸರಾಸರಿ_2, ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು/ಸರಾಸರಿ_2.ಅಡ್ಡಸಾಲುಗಳು, ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/ಸರಾಸರಿ_2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        ಆದರೆ( ದತ್ತಾಂಶ.type() != ctype || tmp_ಸರಾಸರಿ.ದತ್ತಾಂಶ == ಸರಾಸರಿ_2.ದತ್ತಾಂಶ )
        {
            ದತ್ತಾಂಶ.convert_ಗಮ್ಯ( tmp_ದತ್ತಾಂಶ, ctype );
            ಕಳೆ( tmp_ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ, tmp_ದತ್ತಾಂಶ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಕಳೆ( ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ, tmp_ಸರಾಸರಿ );
            tmp_ದತ್ತಾಂಶ = tmp_ಸರಾಸರಿ;
        }

        Mat evects1(ಎಣಿಕೆ_3, len, ctype);
        gemm( eigenvectors, tmp_ದತ್ತಾಂಶ, 1, Mat(), 0, evects1,
            (flags & PCA::ದತ್ತಾಂಶ_AS_ಲಂಬಸಾಲು) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ( i = 0; i < ಹೊರಹೋಗು_ಎಣಿಕೆ; i++ )
        {
            Mat vec = eigenvectors.ಅಡ್ಡಸಾಲು(i);
            normalize(vec, vec);
        }
    }

    ಆದರೆ( ಎಣಿಕೆ_3 > ಹೊರಹೋಗು_ಎಣಿಕೆ )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.ಅಡ್ಡಸಾಲು_Range_2(0,ಹೊರಹೋಗು_ಎಣಿಕೆ).clone();
        eigenvectors = eigenvectors.ಅಡ್ಡಸಾಲು_Range_2(0,ಹೊರಹೋಗು_ಎಣಿಕೆ).clone();
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಶೂನ್ಯ PCA::ಲಿಖ(FileStorage& fs ) ಸ್ಥಿರಾಂಕ
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << ಸರಾಸರಿ_2;
}

ಶೂನ್ಯ PCA::ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::ಪಠ(fn["vectors"], eigenvectors);
    cv::ಪಠ(fn["values"], eigenvalues);
    cv::ಪಠ(fn["mean"], ಸರಾಸರಿ_2);
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಪೂರ್ಣಾಂಕ computeCumulativeEnergy(ಸ್ಥಿರಾಂಕ Mat& eigenvalues, ದ್ವಿಗುಣ retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == ದತ್ತಾಂಶ_Type_2<T>::type );

    Mat g(eigenvalues.size(), ದತ್ತಾಂಶ_Type_2<T>::type);

    ಗಾಗಿ(ಪೂರ್ಣಾಂಕ ig = 0; ig < g.ಅಡ್ಡಸಾಲುಗಳು; ig++)
    {
        g.at<T>(ig, 0) = 0;
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    ಪೂರ್ಣಾಂಕ L;

    ಗಾಗಿ(L = 0; L < eigenvalues.ಅಡ್ಡಸಾಲುಗಳು; L++)
    {
        ದ್ವಿಗುಣ energy = g.at<T>(L, 0) / g.at<T>(g.ಅಡ್ಡಸಾಲುಗಳು - 1, 0);
        ಆದರೆ(energy > retainedVariance)
            ನಿಲ್ಲಿಸು;
    }

    L = std::max(2, L);

    ಹಿಂತಿರುಗಿ L;
}

PCA& PCA::ಕ್ರಿಯಾಕಾರಕ()(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ_3, ಒಳಹರಿವು_Array __mean, ಪೂರ್ಣಾಂಕ flags, ದ್ವಿಗುಣ retainedVariance)
{
    Mat ದತ್ತಾಂಶ = ದತ್ತಾಂಶ_3.getMat(), ಸರಾಸರಿ = __mean.getMat();
    ಪೂರ್ಣಾಂಕ covar_flags = cv::COVAR_SCALE;
    ಪೂರ್ಣಾಂಕ len, ಒಳ_ಎಣಿಕೆ;
    ಗಾತ್ರ_2 ಸರಾಸರಿ_sz;

    CV_Assert( ದತ್ತಾಂಶ.channels() == 1 );
    ಆದರೆ( flags & PCA::ದತ್ತಾಂಶ_AS_ಲಂಬಸಾಲು )
    {
        len = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು;
        ಒಳ_ಎಣಿಕೆ = ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        covar_flags |= cv::COVAR_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ಸರಾಸರಿ_sz = ಗಾತ್ರ_2(1, len);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        len = ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ಒಳ_ಎಣಿಕೆ = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು;
        covar_flags |= cv::COVAR_ಅಡ್ಡಸಾಲುಗಳು;
        ಸರಾಸರಿ_sz = ಗಾತ್ರ_2(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = std::min(len, ಒಳ_ಎಣಿಕೆ);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ಆದರೆ( len <= ಒಳ_ಎಣಿಕೆ )
        covar_flags |= cv::COVAR_ಸಾಮಾನ್ಯ;

    ಪೂರ್ಣಾಂಕ ctype = std::max(CV_32F, ದತ್ತಾಂಶ.ಆಳ());
    ಸರಾಸರಿ_2.create( ಸರಾಸರಿ_sz, ctype );

    Mat covar( ಎಣಿಕೆ_3, ಎಣಿಕೆ_3, ctype );

    ಆದರೆ( !ಸರಾಸರಿ.empty() )
    {
        CV_Assert( ಸರಾಸರಿ.size() == ಸರಾಸರಿ_sz );
        ಸರಾಸರಿ.convert_ಗಮ್ಯ(ಸರಾಸರಿ_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ಮಾತೃಕೆ( ದತ್ತಾಂಶ, covar, ಸರಾಸರಿ_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ಆದರೆ( !(covar_flags & cv::COVAR_ಸಾಮಾನ್ಯ) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ = repeat(ಸರಾಸರಿ_2, ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು/ಸರಾಸರಿ_2.ಅಡ್ಡಸಾಲುಗಳು, ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/ಸರಾಸರಿ_2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        ಆದರೆ( ದತ್ತಾಂಶ.type() != ctype || tmp_ಸರಾಸರಿ.ದತ್ತಾಂಶ == ಸರಾಸರಿ_2.ದತ್ತಾಂಶ )
        {
            ದತ್ತಾಂಶ.convert_ಗಮ್ಯ( tmp_ದತ್ತಾಂಶ, ctype );
            ಕಳೆ( tmp_ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ, tmp_ದತ್ತಾಂಶ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಕಳೆ( ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ, tmp_ಸರಾಸರಿ );
            tmp_ದತ್ತಾಂಶ = tmp_ಸರಾಸರಿ;
        }

        Mat evects1(ಎಣಿಕೆ_3, len, ctype);
        gemm( eigenvectors, tmp_ದತ್ತಾಂಶ, 1, Mat(), 0, evects1,
            (flags & PCA::ದತ್ತಾಂಶ_AS_ಲಂಬಸಾಲು) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ( i = 0; i < eigenvectors.ಅಡ್ಡಸಾಲುಗಳು; i++ )
        {
            Mat vec = eigenvectors.ಅಡ್ಡಸಾಲು(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    ಪೂರ್ಣಾಂಕ L;
    ಆದರೆ (ctype == CV_32F)
        L = computeCumulativeEnergy<ದಶಮಾಂಶ>(eigenvalues, retainedVariance);
    ಇಲ್ಲದಿದ್ದರೆ
        L = computeCumulativeEnergy<ದ್ವಿಗುಣ>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.ಅಡ್ಡಸಾಲು_Range_2(0,L).clone();
    eigenvectors = eigenvectors.ಅಡ್ಡಸಾಲು_Range_2(0,L).clone();

    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಶೂನ್ಯ PCA::project(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ_3, OutputArray ಫಲಿತಾಂಶ) ಸ್ಥಿರಾಂಕ
{
    Mat ದತ್ತಾಂಶ = ದತ್ತಾಂಶ_3.getMat();
    CV_Assert( !ಸರಾಸರಿ_2.empty() && !eigenvectors.empty() &&
        ((ಸರಾಸರಿ_2.ಅಡ್ಡಸಾಲುಗಳು == 1 && ಸರಾಸರಿ_2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) || (ಸರಾಸರಿ_2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 && ಸರಾಸರಿ_2.ಅಡ್ಡಸಾಲುಗಳು == ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು)));
    Mat tmp_ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ = repeat(ಸರಾಸರಿ_2, ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು/ಸರಾಸರಿ_2.ಅಡ್ಡಸಾಲುಗಳು, ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/ಸರಾಸರಿ_2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    ಪೂರ್ಣಾಂಕ ctype = ಸರಾಸರಿ_2.type();
    ಆದರೆ( ದತ್ತಾಂಶ.type() != ctype || tmp_ಸರಾಸರಿ.ದತ್ತಾಂಶ == ಸರಾಸರಿ_2.ದತ್ತಾಂಶ )
    {
        ದತ್ತಾಂಶ.convert_ಗಮ್ಯ( tmp_ದತ್ತಾಂಶ, ctype );
        ಕಳೆ( tmp_ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ, tmp_ದತ್ತಾಂಶ );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಕಳೆ( ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ, tmp_ಸರಾಸರಿ );
        tmp_ದತ್ತಾಂಶ = tmp_ಸರಾಸರಿ;
    }
    ಆದರೆ( ಸರಾಸರಿ_2.ಅಡ್ಡಸಾಲುಗಳು == 1 )
        gemm( tmp_ದತ್ತಾಂಶ, eigenvectors, 1, Mat(), 0, ಫಲಿತಾಂಶ, GEMM_2_T );
    ಇಲ್ಲದಿದ್ದರೆ
        gemm( eigenvectors, tmp_ದತ್ತಾಂಶ, 1, Mat(), 0, ಫಲಿತಾಂಶ, 0 );
}

Mat PCA::project(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ) ಸ್ಥಿರಾಂಕ
{
    Mat ಫಲಿತಾಂಶ;
    project(ದತ್ತಾಂಶ, ಫಲಿತಾಂಶ);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಶೂನ್ಯ PCA::ಹಿಂಭಾಗ_Project(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ_3, OutputArray ಫಲಿತಾಂಶ) ಸ್ಥಿರಾಂಕ
{
    Mat ದತ್ತಾಂಶ = ದತ್ತಾಂಶ_3.getMat();
    CV_Assert( !ಸರಾಸರಿ_2.empty() && !eigenvectors.empty() &&
        ((ಸರಾಸರಿ_2.ಅಡ್ಡಸಾಲುಗಳು == 1 && eigenvectors.ಅಡ್ಡಸಾಲುಗಳು == ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) ||
         (ಸರಾಸರಿ_2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 && eigenvectors.ಅಡ್ಡಸಾಲುಗಳು == ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು)));

    Mat tmp_ದತ್ತಾಂಶ, tmp_ಸರಾಸರಿ;
    ದತ್ತಾಂಶ.convert_ಗಮ್ಯ(tmp_ದತ್ತಾಂಶ, ಸರಾಸರಿ_2.type());
    ಆದರೆ( ಸರಾಸರಿ_2.ಅಡ್ಡಸಾಲುಗಳು == 1 )
    {
        tmp_ಸರಾಸರಿ = repeat(ಸರಾಸರಿ_2, ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು, 1);
        gemm( tmp_ದತ್ತಾಂಶ, eigenvectors, 1, tmp_ಸರಾಸರಿ, 1, ಫಲಿತಾಂಶ, 0 );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        tmp_ಸರಾಸರಿ = repeat(ಸರಾಸರಿ_2, 1, ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        gemm( eigenvectors, tmp_ದತ್ತಾಂಶ, 1, tmp_ಸರಾಸರಿ, 1, ಫಲಿತಾಂಶ, GEMM_1_T );
    }
}

Mat PCA::ಹಿಂಭಾಗ_Project(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ) ಸ್ಥಿರಾಂಕ
{
    Mat ಫಲಿತಾಂಶ;
    ಹಿಂಭಾಗ_Project(ದತ್ತಾಂಶ, ಫಲಿತಾಂಶ);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

}

ಶೂನ್ಯ cv::PCACompute(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ, ಒಳಹರಿವು_Output_Array ಸರಾಸರಿ_2,
                    OutputArray eigenvectors, ಪೂರ್ಣಾಂಕ maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ದತ್ತಾಂಶ, ಸರಾಸರಿ_2, 0, maxComponents);
    pca.ಸರಾಸರಿ_2.copy_ಗಮ್ಯ(ಸರಾಸರಿ_2);
    pca.eigenvectors.copy_ಗಮ್ಯ(eigenvectors);
}

ಶೂನ್ಯ cv::PCACompute(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ, ಒಳಹರಿವು_Output_Array ಸರಾಸರಿ_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ಪೂರ್ಣಾಂಕ maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ದತ್ತಾಂಶ, ಸರಾಸರಿ_2, 0, maxComponents);
    pca.ಸರಾಸರಿ_2.copy_ಗಮ್ಯ(ಸರಾಸರಿ_2);
    pca.eigenvectors.copy_ಗಮ್ಯ(eigenvectors);
    pca.eigenvalues.copy_ಗಮ್ಯ(eigenvalues);
}

ಶೂನ್ಯ cv::PCACompute(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ, ಒಳಹರಿವು_Output_Array ಸರಾಸರಿ_2,
                    OutputArray eigenvectors, ದ್ವಿಗುಣ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ದತ್ತಾಂಶ, ಸರಾಸರಿ_2, 0, retainedVariance);
    pca.ಸರಾಸರಿ_2.copy_ಗಮ್ಯ(ಸರಾಸರಿ_2);
    pca.eigenvectors.copy_ಗಮ್ಯ(eigenvectors);
}

ಶೂನ್ಯ cv::PCACompute(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ, ಒಳಹರಿವು_Output_Array ಸರಾಸರಿ_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ದ್ವಿಗುಣ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ದತ್ತಾಂಶ, ಸರಾಸರಿ_2, 0, retainedVariance);
    pca.ಸರಾಸರಿ_2.copy_ಗಮ್ಯ(ಸರಾಸರಿ_2);
    pca.eigenvectors.copy_ಗಮ್ಯ(eigenvectors);
    pca.eigenvalues.copy_ಗಮ್ಯ(eigenvalues);
}

ಶೂನ್ಯ cv::PCAProject(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ, ಒಳಹರಿವು_Array ಸರಾಸರಿ_2,
                    ಒಳಹರಿವು_Array eigenvectors, OutputArray ಫಲಿತಾಂಶ)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.ಸರಾಸರಿ_2 = ಸರಾಸರಿ_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(ದತ್ತಾಂಶ, ಫಲಿತಾಂಶ);
}

ಶೂನ್ಯ cv::PCABackProject(ಒಳಹರಿವು_Array ದತ್ತಾಂಶ, ಒಳಹರಿವು_Array ಸರಾಸರಿ_2,
                    ಒಳಹರಿವು_Array eigenvectors, OutputArray ಫಲಿತಾಂಶ)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.ಸರಾಸರಿ_2 = ಸರಾಸರಿ_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.ಹಿಂಭಾಗ_Project(ದತ್ತಾಂಶ, ಫಲಿತಾಂಶ);
}
