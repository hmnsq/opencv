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

PCA::PCA(درون_داد_Array داده, درون_داد_Array میانگین, صحيح flags, صحيح maxComponents)
{
    عملگر()(داده, میانگین, flags, maxComponents);
}

PCA::PCA(درون_داد_Array داده, درون_داد_Array میانگین, صحيح flags, مزدوج retainedVariance)
{
    عملگر()(داده, میانگین, flags, retainedVariance);
}

PCA& PCA::عملگر()(درون_داد_Array داده_3, درون_داد_Array __mean, صحيح flags, صحيح maxComponents)
{
    Mat داده = داده_3.getMat(), میانگین = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, درون_شمار;
    اندازه میانگین_sz;

    CV_Assert( داده.channels() == 1 );
    إذا( flags & PCA::داده_AS_ستون )
    {
        len = داده.ردیفها;
        درون_شمار = داده.تعداد_ستون;
        covar_flags |= cv::COVAR_تعداد_ستون;
        میانگین_sz = اندازه(1, len);
    }
    وإلا
    {
        len = داده.تعداد_ستون;
        درون_شمار = داده.ردیفها;
        covar_flags |= cv::COVAR_ردیفها;
        میانگین_sz = اندازه(len, 1);
    }

    صحيح شمار_2 = std::min(len, درون_شمار), بیرون_شمار = شمار_2;
    إذا( maxComponents > 0 )
        بیرون_شمار = std::min(شمار_2, maxComponents);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= درون_شمار )
        covar_flags |= cv::COVAR_نرمال;

    صحيح ctype = std::max(CV_32F, داده.عمق());
    میانگین_2.create( میانگین_sz, ctype );

    Mat covar( شمار_2, شمار_2, ctype );

    إذا( !میانگین.empty() )
    {
        CV_Assert( میانگین.size() == میانگین_sz );
        میانگین.convert_مقصد(میانگین_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ماتریس( داده, covar, میانگین_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_نرمال) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_داده, tmp_میانگین = repeat(میانگین_2, داده.ردیفها/میانگین_2.ردیفها, داده.تعداد_ستون/میانگین_2.تعداد_ستون);
        إذا( داده.type() != ctype || tmp_میانگین.داده == میانگین_2.داده )
        {
            داده.convert_مقصد( tmp_داده, ctype );
            تفریق( tmp_داده, tmp_میانگین, tmp_داده );
        }
        وإلا
        {
            تفریق( داده, tmp_میانگین, tmp_میانگین );
            tmp_داده = tmp_میانگین;
        }

        Mat evects1(شمار_2, len, ctype);
        gemm( eigenvectors, tmp_داده, 1, Mat(), 0, evects1,
            (flags & PCA::داده_AS_ستون) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize eigenvectors
        صحيح i;
        لكل( i = 0; i < بیرون_شمار; i++ )
        {
            Mat vec = eigenvectors.ردیف(i);
            normalize(vec, vec);
        }
    }

    إذا( شمار_2 > بیرون_شمار )
    {
        // use clone() to physically copy the data and thus deallocate the original matrices
        eigenvalues = eigenvalues.ردیف_Range_2(0,بیرون_شمار).clone();
        eigenvectors = eigenvectors.ردیف_Range_2(0,بیرون_شمار).clone();
    }
    أرجع *هذا;
}

فراغ PCA::اكتب(FileStorage& fs ) ثابت
{
    CV_Assert( fs.isOpened() );

    fs << "name" << "PCA";
    fs << "vectors" << eigenvectors;
    fs << "values" << eigenvalues;
    fs << "mean" << میانگین_2;
}

فراغ PCA::اقرأ(ثابت File_گره& fn)
{
    CV_Assert( !fn.empty() );
    CV_Assert( (String)fn["name"] == "PCA" );

    cv::اقرأ(fn["vectors"], eigenvectors);
    cv::اقرأ(fn["values"], eigenvalues);
    cv::اقرأ(fn["mean"], میانگین_2);
}

قالب <نام_نوع T>
صحيح computeCumulativeEnergy(ثابت Mat& eigenvalues, مزدوج retainedVariance)
{
    CV_DbgAssert( eigenvalues.type() == داده_Type_2<T>::type );

    Mat g(eigenvalues.size(), داده_Type_2<T>::type);

    لكل(صحيح ig = 0; ig < g.ردیفها; ig++)
    {
        g.at<T>(ig, 0) = 0;
        لكل(صحيح im = 0; im <= ig; im++)
        {
            g.at<T>(ig,0) += eigenvalues.at<T>(im,0);
        }
    }

    صحيح L;

    لكل(L = 0; L < eigenvalues.ردیفها; L++)
    {
        مزدوج energy = g.at<T>(L, 0) / g.at<T>(g.ردیفها - 1, 0);
        إذا(energy > retainedVariance)
            اكسر;
    }

    L = std::max(2, L);

    أرجع L;
}

PCA& PCA::عملگر()(درون_داد_Array داده_3, درون_داد_Array __mean, صحيح flags, مزدوج retainedVariance)
{
    Mat داده = داده_3.getMat(), میانگین = __mean.getMat();
    صحيح covar_flags = cv::COVAR_SCALE;
    صحيح len, درون_شمار;
    اندازه میانگین_sz;

    CV_Assert( داده.channels() == 1 );
    إذا( flags & PCA::داده_AS_ستون )
    {
        len = داده.ردیفها;
        درون_شمار = داده.تعداد_ستون;
        covar_flags |= cv::COVAR_تعداد_ستون;
        میانگین_sz = اندازه(1, len);
    }
    وإلا
    {
        len = داده.تعداد_ستون;
        درون_شمار = داده.ردیفها;
        covar_flags |= cv::COVAR_ردیفها;
        میانگین_sz = اندازه(len, 1);
    }

    CV_Assert( retainedVariance > 0 && retainedVariance <= 1 );

    صحيح شمار_2 = std::min(len, درون_شمار);

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    إذا( len <= درون_شمار )
        covar_flags |= cv::COVAR_نرمال;

    صحيح ctype = std::max(CV_32F, داده.عمق());
    میانگین_2.create( میانگین_sz, ctype );

    Mat covar( شمار_2, شمار_2, ctype );

    إذا( !میانگین.empty() )
    {
        CV_Assert( میانگین.size() == میانگین_sz );
        میانگین.convert_مقصد(میانگین_2, ctype);
        covar_flags |= cv::COVAR_USE_AVG;
    }

    calc_Covar_ماتریس( داده, covar, میانگین_2, covar_flags, ctype );
    eigen( covar, eigenvalues, eigenvectors );

    إذا( !(covar_flags & cv::COVAR_نرمال) )
    {
        // PCA::DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // PCA::DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        Mat tmp_داده, tmp_میانگین = repeat(میانگین_2, داده.ردیفها/میانگین_2.ردیفها, داده.تعداد_ستون/میانگین_2.تعداد_ستون);
        إذا( داده.type() != ctype || tmp_میانگین.داده == میانگین_2.داده )
        {
            داده.convert_مقصد( tmp_داده, ctype );
            تفریق( tmp_داده, tmp_میانگین, tmp_داده );
        }
        وإلا
        {
            تفریق( داده, tmp_میانگین, tmp_میانگین );
            tmp_داده = tmp_میانگین;
        }

        Mat evects1(شمار_2, len, ctype);
        gemm( eigenvectors, tmp_داده, 1, Mat(), 0, evects1,
            (flags & PCA::داده_AS_ستون) ? cv::GEMM_2_T : 0);
        eigenvectors = evects1;

        // normalize all eigenvectors
        صحيح i;
        لكل( i = 0; i < eigenvectors.ردیفها; i++ )
        {
            Mat vec = eigenvectors.ردیف(i);
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
    eigenvalues = eigenvalues.ردیف_Range_2(0,L).clone();
    eigenvectors = eigenvectors.ردیف_Range_2(0,L).clone();

    أرجع *هذا;
}

فراغ PCA::project(درون_داد_Array داده_3, OutputArray نتیجه) ثابت
{
    Mat داده = داده_3.getMat();
    CV_Assert( !میانگین_2.empty() && !eigenvectors.empty() &&
        ((میانگین_2.ردیفها == 1 && میانگین_2.تعداد_ستون == داده.تعداد_ستون) || (میانگین_2.تعداد_ستون == 1 && میانگین_2.ردیفها == داده.ردیفها)));
    Mat tmp_داده, tmp_میانگین = repeat(میانگین_2, داده.ردیفها/میانگین_2.ردیفها, داده.تعداد_ستون/میانگین_2.تعداد_ستون);
    صحيح ctype = میانگین_2.type();
    إذا( داده.type() != ctype || tmp_میانگین.داده == میانگین_2.داده )
    {
        داده.convert_مقصد( tmp_داده, ctype );
        تفریق( tmp_داده, tmp_میانگین, tmp_داده );
    }
    وإلا
    {
        تفریق( داده, tmp_میانگین, tmp_میانگین );
        tmp_داده = tmp_میانگین;
    }
    إذا( میانگین_2.ردیفها == 1 )
        gemm( tmp_داده, eigenvectors, 1, Mat(), 0, نتیجه, GEMM_2_T );
    وإلا
        gemm( eigenvectors, tmp_داده, 1, Mat(), 0, نتیجه, 0 );
}

Mat PCA::project(درون_داد_Array داده) ثابت
{
    Mat نتیجه;
    project(داده, نتیجه);
    أرجع نتیجه;
}

فراغ PCA::پشت_Project(درون_داد_Array داده_3, OutputArray نتیجه) ثابت
{
    Mat داده = داده_3.getMat();
    CV_Assert( !میانگین_2.empty() && !eigenvectors.empty() &&
        ((میانگین_2.ردیفها == 1 && eigenvectors.ردیفها == داده.تعداد_ستون) ||
         (میانگین_2.تعداد_ستون == 1 && eigenvectors.ردیفها == داده.ردیفها)));

    Mat tmp_داده, tmp_میانگین;
    داده.convert_مقصد(tmp_داده, میانگین_2.type());
    إذا( میانگین_2.ردیفها == 1 )
    {
        tmp_میانگین = repeat(میانگین_2, داده.ردیفها, 1);
        gemm( tmp_داده, eigenvectors, 1, tmp_میانگین, 1, نتیجه, 0 );
    }
    وإلا
    {
        tmp_میانگین = repeat(میانگین_2, 1, داده.تعداد_ستون);
        gemm( eigenvectors, tmp_داده, 1, tmp_میانگین, 1, نتیجه, GEMM_1_T );
    }
}

Mat PCA::پشت_Project(درون_داد_Array داده) ثابت
{
    Mat نتیجه;
    پشت_Project(داده, نتیجه);
    أرجع نتیجه;
}

}

فراغ cv::PCACompute(درون_داد_Array داده, درون_داد_Output_Array میانگین_2,
                    OutputArray eigenvectors, صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(داده, میانگین_2, 0, maxComponents);
    pca.میانگین_2.copy_مقصد(میانگین_2);
    pca.eigenvectors.copy_مقصد(eigenvectors);
}

فراغ cv::PCACompute(درون_داد_Array داده, درون_داد_Output_Array میانگین_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    صحيح maxComponents)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(داده, میانگین_2, 0, maxComponents);
    pca.میانگین_2.copy_مقصد(میانگین_2);
    pca.eigenvectors.copy_مقصد(eigenvectors);
    pca.eigenvalues.copy_مقصد(eigenvalues);
}

فراغ cv::PCACompute(درون_داد_Array داده, درون_داد_Output_Array میانگین_2,
                    OutputArray eigenvectors, مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(داده, میانگین_2, 0, retainedVariance);
    pca.میانگین_2.copy_مقصد(میانگین_2);
    pca.eigenvectors.copy_مقصد(eigenvectors);
}

فراغ cv::PCACompute(درون_داد_Array داده, درون_داد_Output_Array میانگین_2,
                    OutputArray eigenvectors, OutputArray eigenvalues,
                    مزدوج retainedVariance)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca(داده, میانگین_2, 0, retainedVariance);
    pca.میانگین_2.copy_مقصد(میانگین_2);
    pca.eigenvectors.copy_مقصد(eigenvectors);
    pca.eigenvalues.copy_مقصد(eigenvalues);
}

فراغ cv::PCAProject(درون_داد_Array داده, درون_داد_Array میانگین_2,
                    درون_داد_Array eigenvectors, OutputArray نتیجه)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.میانگین_2 = میانگین_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.project(داده, نتیجه);
}

فراغ cv::PCABackProject(درون_داد_Array داده, درون_داد_Array میانگین_2,
                    درون_داد_Array eigenvectors, OutputArray نتیجه)
{
    CV_INSTRUMENT_REGION();

    PCA pca;
    pca.میانگین_2 = میانگین_2.getMat();
    pca.eigenvectors = eigenvectors.getMat();
    pca.پشت_Project(داده, نتیجه);
}
