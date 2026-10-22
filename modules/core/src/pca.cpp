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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

/****************************************************************************************\
*                                          PCA                                           *
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv
{

PCA::PCA() {}

PCA::PCA(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ, ਨਿਵੇਸ਼_Array ਔਸਤ, ਪੂਰਨ flags, ਪੂਰਨ maxComponents)
{
    ਸੰਚਾਲਕ()(ਅੰਕੜੇ, ਔਸਤ, flags, maxComponents);
}

PCA::PCA(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ, ਨਿਵੇਸ਼_Array ਔਸਤ, ਪੂਰਨ flags, ਦੁੱਗਣਾ retainedVariance)
{
    ਸੰਚਾਲਕ()(ਅੰਕੜੇ, ਔਸਤ, flags, retainedVariance);
}

PCA& PCA::ਸੰਚਾਲਕ()(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ_3, ਨਿਵੇਸ਼_Array __mean, ਪੂਰਨ flags, ਪੂਰਨ maxComponents)
{
    Mat ਅੰਕੜੇ = ਅੰਕੜੇ_3.getMat(), ਔਸਤ = __mean.getMat();
    ਪੂਰਨ covar_flags = cv::COVAR_SCALE;
    ਪੂਰਨ len, ਵਿੱਚ_ਗਿਣਤੀ;
    ਮਾਪ ਔਸਤ_sz;

    CV_Assert( ਅੰਕੜੇ.channels() == 1 );
    ਜੇ( flags & PCA::ਅੰਕੜੇ_AS_ਖਾਨਾ )
    {
        len = ਅੰਕੜੇ.ਪੰਕਤੀਆਂ;
        ਵਿੱਚ_ਗਿਣਤੀ = ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ;
        covar_flags |= cv::COVAR_ਖਾਨੇ_ਗਿਣਤੀ;
        ਔਸਤ_sz = ਮਾਪ(1, len);
    }
    ਨਹੀਂਤਾਂ
    {
        len = ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ;
        ਵਿੱਚ_ਗਿਣਤੀ = ਅੰਕੜੇ.ਪੰਕਤੀਆਂ;
        covar_flags |= cv::COVAR_ਪੰਕਤੀਆਂ;
        ਔਸਤ_sz = ਮਾਪ(len, 1);
    }

    ਪੂਰਨ ਗਿਣਤੀ_3 = std::min(len, ਵਿੱਚ_ਗਿਣਤੀ), ਬਾਹਰਲਾ_ਗਿਣਤੀ = ਗਿਣਤੀ_3;
    ਜੇ( maxComponents > 0 )
        ਬਾਹਰਲਾ_ਗਿਣਤੀ = std::min(ਗਿਣਤੀ_3, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ਜੇ( len <= ਵਿੱਚ_ਗਿਣਤੀ )
        covar_flags |= cv::COVAR_ਸਾਧਾਰਨ;

    ਪੂਰਨ ctype = std::max(CV_32F, ਅੰਕੜੇ.ਡੂੰਘਾਈ());
    ਔਸਤ_2.create( ਔਸਤ_sz, ctype );

    Mat covar( ਗਿਣਤੀ_3, ਗਿਣਤੀ_3, ctype );

    ਜੇ( !ਔਸਤ.empty() )
    {
        CV_Assert( ਔਸਤ.size() == ਔਸਤ_sz );
        ਔਸਤ.convert_ਮੰਜ਼ਿਲ(ਔਸਤ_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ਮੈਟ੍ਰਿਕਸ( ਅੰਕੜੇ, covar, ਔਸਤ_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ਜੇ( !(covar_flags & cv::COVAR_ਸਾਧਾਰਨ) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_ਅੰਕੜੇ, tmp_ਔਸਤ = repeat(ਔਸਤ_2, ਅੰਕੜੇ.ਪੰਕਤੀਆਂ/ਔਸਤ_2.ਪੰਕਤੀਆਂ, ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ/ਔਸਤ_2.ਖਾਨੇ_ਗਿਣਤੀ);
        ਜੇ( ਅੰਕੜੇ.type() != ctype || tmp_ਔਸਤ.ਅੰਕੜੇ == ਔਸਤ_2.ਅੰਕੜੇ )
        {
            ਅੰਕੜੇ.convert_ਮੰਜ਼ਿਲ( tmp_ਅੰਕੜੇ, ctype );
            ਘਟਾਓ( tmp_ਅੰਕੜੇ, tmp_ਔਸਤ, tmp_ਅੰਕੜੇ );
        }
        ਨਹੀਂਤਾਂ
        {
            ਘਟਾਓ( ਅੰਕੜੇ, tmp_ਔਸਤ, tmp_ਔਸਤ );
            tmp_ਅੰਕੜੇ = tmp_ਔਸਤ;
        }

        Mat evects1(ਗਿਣਤੀ_3, len, ctype);
        gemm( eigenvectors, tmp_ਅੰਕੜੇ, 1, Mat(), 0, evects1,
            (flags & PCA::ਅੰਕੜੇ_AS_ਖਾਨਾ) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        ਪੂਰਨ i;
        ਲਈ( i = 0; i < ਬਾਹਰਲਾ_ਗਿਣਤੀ; i++ )
        {
            Mat vec = eigenvectors.ਪੰਕਤੀ(i);
            normalize(vec, vec);
        }
    }

    ਜੇ( ਗਿਣਤੀ_3 > ਬਾਹਰਲਾ_ਗਿਣਤੀ )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.ਪੰਕਤੀ_Range_2(0,ਬਾਹਰਲਾ_ਗਿਣਤੀ).clone();
        eigenvectors = eigenvectors.ਪੰਕਤੀ_Range_2(0,ਬਾਹਰਲਾ_ਗਿਣਤੀ).clone();
    }
    ਵਾਪਸ *ਇਹ;
}

ਖਾਲੀ PCA::ਲਿਖ(FileStorage& fs ) ਅਚਲ
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << ਔਸਤ_2;
}

ਖਾਲੀ PCA::ਪਠ(ਅਚਲ File_ਗੰਢ& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::ਪਠ(fn["vectors"], eigenvectors);
    cv::ਪਠ(fn["values"], eigenvalues);
    cv::ਪਠ(fn["mean"], ਔਸਤ_2);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਪੂਰਨ computeCumulativeEnergy(ਅਚਲ Mat& eigenvalues, ਦੁੱਗਣਾ retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == ਅੰਕੜੇ_Type_2<T>::type );

    Mat g(eigenvalues.size(), ਅੰਕੜੇ_Type_2<T>::type);

    ਲਈ(ਪੂਰਨ ig = 0; ig < g.ਪੰਕਤੀਆਂ; ig++)
    {
        g.at<T>(ig, 0) = 0;
        ਲਈ(ਪੂਰਨ im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    ਪੂਰਨ L;

    ਲਈ(L = 0; L < eigenvalues.ਪੰਕਤੀਆਂ; L++)
    {
        ਦੁੱਗਣਾ energy = g.at<T>(L, 0) / g.at<T>(g.ਪੰਕਤੀਆਂ - 1, 0);
        ਜੇ(energy > retainedVariance)
            ਰੋਕੋ;
    }

    L = std::max(2, L);

    ਵਾਪਸ L;
}

PCA& PCA::ਸੰਚਾਲਕ()(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ_3, ਨਿਵੇਸ਼_Array __mean, ਪੂਰਨ flags, ਦੁੱਗਣਾ retainedVariance)
{
    Mat ਅੰਕੜੇ = ਅੰਕੜੇ_3.getMat(), ਔਸਤ = __mean.getMat();
    ਪੂਰਨ covar_flags = cv::COVAR_SCALE;
    ਪੂਰਨ len, ਵਿੱਚ_ਗਿਣਤੀ;
    ਮਾਪ ਔਸਤ_sz;

    CV_Assert( ਅੰਕੜੇ.channels() == 1 );
    ਜੇ( flags & PCA::ਅੰਕੜੇ_AS_ਖਾਨਾ )
    {
        len = ਅੰਕੜੇ.ਪੰਕਤੀਆਂ;
        ਵਿੱਚ_ਗਿਣਤੀ = ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ;
        covar_flags |= cv::COVAR_ਖਾਨੇ_ਗਿਣਤੀ;
        ਔਸਤ_sz = ਮਾਪ(1, len);
    }
    ਨਹੀਂਤਾਂ
    {
        len = ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ;
        ਵਿੱਚ_ਗਿਣਤੀ = ਅੰਕੜੇ.ਪੰਕਤੀਆਂ;
        covar_flags |= cv::COVAR_ਪੰਕਤੀਆਂ;
        ਔਸਤ_sz = ਮਾਪ(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    ਪੂਰਨ ਗਿਣਤੀ_3 = std::min(len, ਵਿੱਚ_ਗਿਣਤੀ);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    ਜੇ( len <= ਵਿੱਚ_ਗਿਣਤੀ )
        covar_flags |= cv::COVAR_ਸਾਧਾਰਨ;

    ਪੂਰਨ ctype = std::max(CV_32F, ਅੰਕੜੇ.ਡੂੰਘਾਈ());
    ਔਸਤ_2.create( ਔਸਤ_sz, ctype );

    Mat covar( ਗਿਣਤੀ_3, ਗਿਣਤੀ_3, ctype );

    ਜੇ( !ਔਸਤ.empty() )
    {
        CV_Assert( ਔਸਤ.size() == ਔਸਤ_sz );
        ਔਸਤ.convert_ਮੰਜ਼ਿਲ(ਔਸਤ_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ਮੈਟ੍ਰਿਕਸ( ਅੰਕੜੇ, covar, ਔਸਤ_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    ਜੇ( !(covar_flags & cv::COVAR_ਸਾਧਾਰਨ) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_ਅੰਕੜੇ, tmp_ਔਸਤ = repeat(ਔਸਤ_2, ਅੰਕੜੇ.ਪੰਕਤੀਆਂ/ਔਸਤ_2.ਪੰਕਤੀਆਂ, ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ/ਔਸਤ_2.ਖਾਨੇ_ਗਿਣਤੀ);
        ਜੇ( ਅੰਕੜੇ.type() != ctype || tmp_ਔਸਤ.ਅੰਕੜੇ == ਔਸਤ_2.ਅੰਕੜੇ )
        {
            ਅੰਕੜੇ.convert_ਮੰਜ਼ਿਲ( tmp_ਅੰਕੜੇ, ctype );
            ਘਟਾਓ( tmp_ਅੰਕੜੇ, tmp_ਔਸਤ, tmp_ਅੰਕੜੇ );
        }
        ਨਹੀਂਤਾਂ
        {
            ਘਟਾਓ( ਅੰਕੜੇ, tmp_ਔਸਤ, tmp_ਔਸਤ );
            tmp_ਅੰਕੜੇ = tmp_ਔਸਤ;
        }

        Mat evects1(ਗਿਣਤੀ_3, len, ctype);
        gemm( eigenvectors, tmp_ਅੰਕੜੇ, 1, Mat(), 0, evects1,
            (flags & PCA::ਅੰਕੜੇ_AS_ਖਾਨਾ) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        ਪੂਰਨ i;
        ਲਈ( i = 0; i < eigenvectors.ਪੰਕਤੀਆਂ; i++ )
        {
            Mat vec = eigenvectors.ਪੰਕਤੀ(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    ਪੂਰਨ L;
    ਜੇ (ctype == CV_32F)
        L = computeCumulativeEnergy<ਦਸ਼ਮਲਵ>(eigenvalues, retainedVariance);
    ਨਹੀਂਤਾਂ
        L = computeCumulativeEnergy<ਦੁੱਗਣਾ>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.ਪੰਕਤੀ_Range_2(0,L).clone();
    eigenvectors = eigenvectors.ਪੰਕਤੀ_Range_2(0,L).clone();

    ਵਾਪਸ *ਇਹ;
}

ਖਾਲੀ PCA::project(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ_3, OutputArray ਨਤੀਜਾ) ਅਚਲ
{
    Mat ਅੰਕੜੇ = ਅੰਕੜੇ_3.getMat();
    CV_Assert( !ਔਸਤ_2.empty() && !eigenvectors.empty() &&
        ((ਔਸਤ_2.ਪੰਕਤੀਆਂ == 1 && ਔਸਤ_2.ਖਾਨੇ_ਗਿਣਤੀ == ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ) || (ਔਸਤ_2.ਖਾਨੇ_ਗਿਣਤੀ == 1 && ਔਸਤ_2.ਪੰਕਤੀਆਂ == ਅੰਕੜੇ.ਪੰਕਤੀਆਂ)));
    Mat tmp_ਅੰਕੜੇ, tmp_ਔਸਤ = repeat(ਔਸਤ_2, ਅੰਕੜੇ.ਪੰਕਤੀਆਂ/ਔਸਤ_2.ਪੰਕਤੀਆਂ, ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ/ਔਸਤ_2.ਖਾਨੇ_ਗਿਣਤੀ);
    ਪੂਰਨ ctype = ਔਸਤ_2.type();
    ਜੇ( ਅੰਕੜੇ.type() != ctype || tmp_ਔਸਤ.ਅੰਕੜੇ == ਔਸਤ_2.ਅੰਕੜੇ )
    {
        ਅੰਕੜੇ.convert_ਮੰਜ਼ਿਲ( tmp_ਅੰਕੜੇ, ctype );
        ਘਟਾਓ( tmp_ਅੰਕੜੇ, tmp_ਔਸਤ, tmp_ਅੰਕੜੇ );
    }
    ਨਹੀਂਤਾਂ
    {
        ਘਟਾਓ( ਅੰਕੜੇ, tmp_ਔਸਤ, tmp_ਔਸਤ );
        tmp_ਅੰਕੜੇ = tmp_ਔਸਤ;
    }
    ਜੇ( ਔਸਤ_2.ਪੰਕਤੀਆਂ == 1 )
        gemm( tmp_ਅੰਕੜੇ, eigenvectors, 1, Mat(), 0, ਨਤੀਜਾ, GEMM_2_T );
    ਨਹੀਂਤਾਂ
        gemm( eigenvectors, tmp_ਅੰਕੜੇ, 1, Mat(), 0, ਨਤੀਜਾ, 0 );
}

Mat PCA::project(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ) ਅਚਲ
{
    Mat ਨਤੀਜਾ;
    project(ਅੰਕੜੇ, ਨਤੀਜਾ);
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਖਾਲੀ PCA::ਪਿੱਠ_Project(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ_3, OutputArray ਨਤੀਜਾ) ਅਚਲ
{
    Mat ਅੰਕੜੇ = ਅੰਕੜੇ_3.getMat();
    CV_Assert( !ਔਸਤ_2.empty() && !eigenvectors.empty() &&
        ((ਔਸਤ_2.ਪੰਕਤੀਆਂ == 1 && eigenvectors.ਪੰਕਤੀਆਂ == ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ) ||
         (ਔਸਤ_2.ਖਾਨੇ_ਗਿਣਤੀ == 1 && eigenvectors.ਪੰਕਤੀਆਂ == ਅੰਕੜੇ.ਪੰਕਤੀਆਂ)));

    Mat tmp_ਅੰਕੜੇ, tmp_ਔਸਤ;
    ਅੰਕੜੇ.convert_ਮੰਜ਼ਿਲ(tmp_ਅੰਕੜੇ, ਔਸਤ_2.type());
    ਜੇ( ਔਸਤ_2.ਪੰਕਤੀਆਂ == 1 )
    {
        tmp_ਔਸਤ = repeat(ਔਸਤ_2, ਅੰਕੜੇ.ਪੰਕਤੀਆਂ, 1);
        gemm( tmp_ਅੰਕੜੇ, eigenvectors, 1, tmp_ਔਸਤ, 1, ਨਤੀਜਾ, 0 );
    }
    ਨਹੀਂਤਾਂ
    {
        tmp_ਔਸਤ = repeat(ਔਸਤ_2, 1, ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ);
        gemm( eigenvectors, tmp_ਅੰਕੜੇ, 1, tmp_ਔਸਤ, 1, ਨਤੀਜਾ, GEMM_1_T );
    }
}

Mat PCA::ਪਿੱਠ_Project(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ) ਅਚਲ
{
    Mat ਨਤੀਜਾ;
    ਪਿੱਠ_Project(ਅੰਕੜੇ, ਨਤੀਜਾ);
    ਵਾਪਸ ਨਤੀਜਾ;
}

}

ਖਾਲੀ cv::PCACompute(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ, ਨਿਵੇਸ਼_Output_Array ਔਸਤ_2,
                    OutputArray eigenvectors, ਪੂਰਨ maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ਅੰਕੜੇ, ਔਸਤ_2, 0, maxComponents);
    pca.ਔਸਤ_2.copy_ਮੰਜ਼ਿਲ(ਔਸਤ_2);
    pca.eigenvectors.copy_ਮੰਜ਼ਿਲ(eigenvectors);
}

ਖਾਲੀ cv::PCACompute(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ, ਨਿਵੇਸ਼_Output_Array ਔਸਤ_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ਪੂਰਨ maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ਅੰਕੜੇ, ਔਸਤ_2, 0, maxComponents);
    pca.ਔਸਤ_2.copy_ਮੰਜ਼ਿਲ(ਔਸਤ_2);
    pca.eigenvectors.copy_ਮੰਜ਼ਿਲ(eigenvectors);
    pca.eigenvalues.copy_ਮੰਜ਼ਿਲ(eigenvalues);
}

ਖਾਲੀ cv::PCACompute(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ, ਨਿਵੇਸ਼_Output_Array ਔਸਤ_2,
                    OutputArray eigenvectors, ਦੁੱਗਣਾ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ਅੰਕੜੇ, ਔਸਤ_2, 0, retainedVariance);
    pca.ਔਸਤ_2.copy_ਮੰਜ਼ਿਲ(ਔਸਤ_2);
    pca.eigenvectors.copy_ਮੰਜ਼ਿਲ(eigenvectors);
}

ਖਾਲੀ cv::PCACompute(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ, ਨਿਵੇਸ਼_Output_Array ਔਸਤ_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    ਦੁੱਗਣਾ retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ਅੰਕੜੇ, ਔਸਤ_2, 0, retainedVariance);
    pca.ਔਸਤ_2.copy_ਮੰਜ਼ਿਲ(ਔਸਤ_2);
    pca.eigenvectors.copy_ਮੰਜ਼ਿਲ(eigenvectors);
    pca.eigenvalues.copy_ਮੰਜ਼ਿਲ(eigenvalues);
}

ਖਾਲੀ cv::PCAProject(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ, ਨਿਵੇਸ਼_Array ਔਸਤ_2,
                    ਨਿਵੇਸ਼_Array eigenvectors, OutputArray ਨਤੀਜਾ)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.ਔਸਤ_2 = ਔਸਤ_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(ਅੰਕੜੇ, ਨਤੀਜਾ);
}

ਖਾਲੀ cv::PCABackProject(ਨਿਵੇਸ਼_Array ਅੰਕੜੇ, ਨਿਵੇਸ਼_Array ਔਸਤ_2,
                    ਨਿਵੇਸ਼_Array eigenvectors, OutputArray ਨਤੀਜਾ)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.ਔਸਤ_2 = ਔਸਤ_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.ਪਿੱਠ_Project(ਅੰਕੜੇ, ਨਤੀਜਾ);
}
