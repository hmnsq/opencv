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

نامخلا cv
{

PCA::PCA() {}

PCA::PCA(داخلہ_Array ڈیٹا, داخلہ_Array اوسط, صحیح flags, صحیح maxComponents)
{
    عامل()(ڈیٹا, اوسط, flags, maxComponents);
}

PCA::PCA(داخلہ_Array ڈیٹا, داخلہ_Array اوسط, صحیح flags, دوہرا retainedVariance)
{
    عامل()(ڈیٹا, اوسط, flags, retainedVariance);
}

PCA& PCA::عامل()(داخلہ_Array ڈیٹا_3, داخلہ_Array __mean, صحیح flags, صحیح maxComponents)
{
    Mat ڈیٹا = ڈیٹا_3.getMat(), اوسط = __mean.getMat();
    صحیح covar_flags = cv::COVAR_SCALE;
    صحیح len, اندر_گنتی;
    سائز اوسط_sz;

    CV_Assert( ڈیٹا.channels() == 1 );
    اگر( flags & PCA::ڈیٹا_AS_ستون )
    {
        len = ڈیٹا.صفیں;
        اندر_گنتی = ڈیٹا.ستون_تعداد;
        covar_flags |= cv::COVAR_ستون_تعداد;
        اوسط_sz = سائز(1, len);
    }
    ورنہ
    {
        len = ڈیٹا.ستون_تعداد;
        اندر_گنتی = ڈیٹا.صفیں;
        covar_flags |= cv::COVAR_صفیں;
        اوسط_sz = سائز(len, 1);
    }

    صحیح گنتی_2 = std::min(len, اندر_گنتی), برآمد_گنتی = گنتی_2;
    اگر( maxComponents > 0 )
        برآمد_گنتی = std::min(گنتی_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    اگر( len <= اندر_گنتی )
        covar_flags |= cv::COVAR_معمول;

    صحیح ctype = std::max(CV_32F, ڈیٹا.گہرائی());
    اوسط_2.create( اوسط_sz, ctype );

    Mat covar( گنتی_2, گنتی_2, ctype );

    اگر( !اوسط.empty() )
    {
        CV_Assert( اوسط.size() == اوسط_sz );
        اوسط.convert_مقصد(اوسط_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_میٹرکس( ڈیٹا, covar, اوسط_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    اگر( !(covar_flags & cv::COVAR_معمول) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_ڈیٹا, tmp_اوسط = repeat(اوسط_2, ڈیٹا.صفیں/اوسط_2.صفیں, ڈیٹا.ستون_تعداد/اوسط_2.ستون_تعداد);
        اگر( ڈیٹا.type() != ctype || tmp_اوسط.ڈیٹا == اوسط_2.ڈیٹا )
        {
            ڈیٹا.convert_مقصد( tmp_ڈیٹا, ctype );
            تفریق( tmp_ڈیٹا, tmp_اوسط, tmp_ڈیٹا );
        }
        ورنہ
        {
            تفریق( ڈیٹا, tmp_اوسط, tmp_اوسط );
            tmp_ڈیٹا = tmp_اوسط;
        }

        Mat evects1(گنتی_2, len, ctype);
        gemm( eigenvectors, tmp_ڈیٹا, 1, Mat(), 0, evects1,
            (flags & PCA::ڈیٹا_AS_ستون) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        صحیح i;
        برائے( i = 0; i < برآمد_گنتی; i++ )
        {
            Mat vec = eigenvectors.صف(i);
            normalize(vec, vec);
        }
    }

    اگر( گنتی_2 > برآمد_گنتی )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.صف_Range_2(0,برآمد_گنتی).clone();
        eigenvectors = eigenvectors.صف_Range_2(0,برآمد_گنتی).clone();
    }
    واپس *یہ;
}

خالی PCA::اكتب(FileStorage& fs ) ثابت
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << اوسط_2;
}

خالی PCA::اقرأ(ثابت File_گرہ& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::اقرأ(fn["vectors"], eigenvectors);
    cv::اقرأ(fn["values"], eigenvalues);
    cv::اقرأ(fn["mean"], اوسط_2);
}

سانچہ <قسم_نام T>
صحیح computeCumulativeEnergy(ثابت Mat& eigenvalues, دوہرا retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == ڈیٹا_Type_2<T>::type );

    Mat g(eigenvalues.size(), ڈیٹا_Type_2<T>::type);

    برائے(صحیح ig = 0; ig < g.صفیں; ig++)
    {
        g.at<T>(ig, 0) = 0;
        برائے(صحیح im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    صحیح L;

    برائے(L = 0; L < eigenvalues.صفیں; L++)
    {
        دوہرا energy = g.at<T>(L, 0) / g.at<T>(g.صفیں - 1, 0);
        اگر(energy > retainedVariance)
            توڑو;
    }

    L = std::max(2, L);

    واپس L;
}

PCA& PCA::عامل()(داخلہ_Array ڈیٹا_3, داخلہ_Array __mean, صحیح flags, دوہرا retainedVariance)
{
    Mat ڈیٹا = ڈیٹا_3.getMat(), اوسط = __mean.getMat();
    صحیح covar_flags = cv::COVAR_SCALE;
    صحیح len, اندر_گنتی;
    سائز اوسط_sz;

    CV_Assert( ڈیٹا.channels() == 1 );
    اگر( flags & PCA::ڈیٹا_AS_ستون )
    {
        len = ڈیٹا.صفیں;
        اندر_گنتی = ڈیٹا.ستون_تعداد;
        covar_flags |= cv::COVAR_ستون_تعداد;
        اوسط_sz = سائز(1, len);
    }
    ورنہ
    {
        len = ڈیٹا.ستون_تعداد;
        اندر_گنتی = ڈیٹا.صفیں;
        covar_flags |= cv::COVAR_صفیں;
        اوسط_sz = سائز(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    صحیح گنتی_2 = std::min(len, اندر_گنتی);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    اگر( len <= اندر_گنتی )
        covar_flags |= cv::COVAR_معمول;

    صحیح ctype = std::max(CV_32F, ڈیٹا.گہرائی());
    اوسط_2.create( اوسط_sz, ctype );

    Mat covar( گنتی_2, گنتی_2, ctype );

    اگر( !اوسط.empty() )
    {
        CV_Assert( اوسط.size() == اوسط_sz );
        اوسط.convert_مقصد(اوسط_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_میٹرکس( ڈیٹا, covar, اوسط_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    اگر( !(covar_flags & cv::COVAR_معمول) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_ڈیٹا, tmp_اوسط = repeat(اوسط_2, ڈیٹا.صفیں/اوسط_2.صفیں, ڈیٹا.ستون_تعداد/اوسط_2.ستون_تعداد);
        اگر( ڈیٹا.type() != ctype || tmp_اوسط.ڈیٹا == اوسط_2.ڈیٹا )
        {
            ڈیٹا.convert_مقصد( tmp_ڈیٹا, ctype );
            تفریق( tmp_ڈیٹا, tmp_اوسط, tmp_ڈیٹا );
        }
        ورنہ
        {
            تفریق( ڈیٹا, tmp_اوسط, tmp_اوسط );
            tmp_ڈیٹا = tmp_اوسط;
        }

        Mat evects1(گنتی_2, len, ctype);
        gemm( eigenvectors, tmp_ڈیٹا, 1, Mat(), 0, evects1,
            (flags & PCA::ڈیٹا_AS_ستون) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        صحیح i;
        برائے( i = 0; i < eigenvectors.صفیں; i++ )
        {
            Mat vec = eigenvectors.صف(i);
            normalize(vec, vec);
        }
    }

    // compute the cumulative energy content for each eigenvector
    صحیح L;
    اگر (ctype == CV_32F)
        L = computeCumulativeEnergy<اعشاری>(eigenvalues, retainedVariance);
    ورنہ
        L = computeCumulativeEnergy<دوہرا>(eigenvalues, retainedVariance);

    // use clone() to physically copy the data and thus deallocate the original matrices
    eigenvalues = eigenvalues.صف_Range_2(0,L).clone();
    eigenvectors = eigenvectors.صف_Range_2(0,L).clone();

    واپس *یہ;
}

خالی PCA::project(داخلہ_Array ڈیٹا_3, OutputArray نتیجہ) ثابت
{
    Mat ڈیٹا = ڈیٹا_3.getMat();
    CV_Assert( !اوسط_2.empty() && !eigenvectors.empty() &&
        ((اوسط_2.صفیں == 1 && اوسط_2.ستون_تعداد == ڈیٹا.ستون_تعداد) || (اوسط_2.ستون_تعداد == 1 && اوسط_2.صفیں == ڈیٹا.صفیں)));
    Mat tmp_ڈیٹا, tmp_اوسط = repeat(اوسط_2, ڈیٹا.صفیں/اوسط_2.صفیں, ڈیٹا.ستون_تعداد/اوسط_2.ستون_تعداد);
    صحیح ctype = اوسط_2.type();
    اگر( ڈیٹا.type() != ctype || tmp_اوسط.ڈیٹا == اوسط_2.ڈیٹا )
    {
        ڈیٹا.convert_مقصد( tmp_ڈیٹا, ctype );
        تفریق( tmp_ڈیٹا, tmp_اوسط, tmp_ڈیٹا );
    }
    ورنہ
    {
        تفریق( ڈیٹا, tmp_اوسط, tmp_اوسط );
        tmp_ڈیٹا = tmp_اوسط;
    }
    اگر( اوسط_2.صفیں == 1 )
        gemm( tmp_ڈیٹا, eigenvectors, 1, Mat(), 0, نتیجہ, GEMM_2_T );
    ورنہ
        gemm( eigenvectors, tmp_ڈیٹا, 1, Mat(), 0, نتیجہ, 0 );
}

Mat PCA::project(داخلہ_Array ڈیٹا) ثابت
{
    Mat نتیجہ;
    project(ڈیٹا, نتیجہ);
    واپس نتیجہ;
}

خالی PCA::عقب_Project(داخلہ_Array ڈیٹا_3, OutputArray نتیجہ) ثابت
{
    Mat ڈیٹا = ڈیٹا_3.getMat();
    CV_Assert( !اوسط_2.empty() && !eigenvectors.empty() &&
        ((اوسط_2.صفیں == 1 && eigenvectors.صفیں == ڈیٹا.ستون_تعداد) ||
         (اوسط_2.ستون_تعداد == 1 && eigenvectors.صفیں == ڈیٹا.صفیں)));

    Mat tmp_ڈیٹا, tmp_اوسط;
    ڈیٹا.convert_مقصد(tmp_ڈیٹا, اوسط_2.type());
    اگر( اوسط_2.صفیں == 1 )
    {
        tmp_اوسط = repeat(اوسط_2, ڈیٹا.صفیں, 1);
        gemm( tmp_ڈیٹا, eigenvectors, 1, tmp_اوسط, 1, نتیجہ, 0 );
    }
    ورنہ
    {
        tmp_اوسط = repeat(اوسط_2, 1, ڈیٹا.ستون_تعداد);
        gemm( eigenvectors, tmp_ڈیٹا, 1, tmp_اوسط, 1, نتیجہ, GEMM_1_T );
    }
}

Mat PCA::عقب_Project(داخلہ_Array ڈیٹا) ثابت
{
    Mat نتیجہ;
    عقب_Project(ڈیٹا, نتیجہ);
    واپس نتیجہ;
}

}

خالی cv::PCACompute(داخلہ_Array ڈیٹا, داخلہ_Output_Array اوسط_2,
                    OutputArray eigenvectors, صحیح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ڈیٹا, اوسط_2, 0, maxComponents);
    pca.اوسط_2.copy_مقصد(اوسط_2);
    pca.eigenvectors.copy_مقصد(eigenvectors);
}

خالی cv::PCACompute(داخلہ_Array ڈیٹا, داخلہ_Output_Array اوسط_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    صحیح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ڈیٹا, اوسط_2, 0, maxComponents);
    pca.اوسط_2.copy_مقصد(اوسط_2);
    pca.eigenvectors.copy_مقصد(eigenvectors);
    pca.eigenvalues.copy_مقصد(eigenvalues);
}

خالی cv::PCACompute(داخلہ_Array ڈیٹا, داخلہ_Output_Array اوسط_2,
                    OutputArray eigenvectors, دوہرا retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ڈیٹا, اوسط_2, 0, retainedVariance);
    pca.اوسط_2.copy_مقصد(اوسط_2);
    pca.eigenvectors.copy_مقصد(eigenvectors);
}

خالی cv::PCACompute(داخلہ_Array ڈیٹا, داخلہ_Output_Array اوسط_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    دوہرا retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(ڈیٹا, اوسط_2, 0, retainedVariance);
    pca.اوسط_2.copy_مقصد(اوسط_2);
    pca.eigenvectors.copy_مقصد(eigenvectors);
    pca.eigenvalues.copy_مقصد(eigenvalues);
}

خالی cv::PCAProject(داخلہ_Array ڈیٹا, داخلہ_Array اوسط_2,
                    داخلہ_Array eigenvectors, OutputArray نتیجہ)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.اوسط_2 = اوسط_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(ڈیٹا, نتیجہ);
}

خالی cv::PCABackProject(داخلہ_Array ڈیٹا, داخلہ_Array اوسط_2,
                    داخلہ_Array eigenvectors, OutputArray نتیجہ)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.اوسط_2 = اوسط_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.عقب_Project(ڈیٹا, نتیجہ);
}
