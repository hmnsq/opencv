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
// Copyright (C) 2015, Itseez Inc., all rights reserved.
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
#అన్తర్భావ "hal_internal.hpp"

#నిర్వచితమైతే HAVE_LAPACK

#అన్తర్భావ "opencv_lapack.h"

#అన్తర్భావ <cmath>
#అన్తర్భావ <algorithm>
#అన్తర్భావ <typeinfo>
#అన్తర్భావ <limits>
#అన్తర్భావ <complex>
#అన్తర్భావ <vector>

#నిర్వచించు HAL_GEMM_SMALL_COMPLEX_మాతృక_THRESH 100
#నిర్వచించు HAL_GEMM_SMALL_మాతృక_THRESH 100
#నిర్వచించు HAL_SVD_SMALL_మాతృక_THRESH 25
#నిర్వచించు HAL_QR_SMALL_మాతృక_THRESH 30
#నిర్వచించు HAL_LU_SMALL_మాతృక_THRESH 100
#నిర్వచించు HAL_CHOLESKY_SMALL_మాతృక_THRESH 100

#ఒకవేళ defined(__clang__) && defined(__has_feature)
#ఒకవేళ __has_feature(జ్ఞాపకం_sanitizer)
#అన్తర్భావ <sanitizer/msan_interface.h>
#నిర్వచించు CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#నిర్వచించు CV_ANNOTATE_NO_SANITIZE_జ్ఞాపకం __attribute__((no_sanitize("memory")))
#అయితే_ముగింపు
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED
#నిర్వచించు CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(address, size) చేయి { } అయితే(0)
#నిర్వచించు CV_ANNOTATE_NO_SANITIZE_జ్ఞాపకం
#అయితే_ముగింపు

#ఒకవేళ defined(__APPLE__) && defined(__clang__)
#సూచన clang diagnostic push
#సూచన clang diagnostic ignored "-Wdeprecated-declarations"
#అయితే_ముగింపు

//lapack stores matrices in column-major order so transposing is needed everywhere
మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస శూన్య
transpose_వర్గం_inplace(fptype *src, size_t src_ld, size_t m)
{
    కొరకు(size_t i = 0; i < m - 1; i++)
        కొరకు(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస శూన్య
transpose(స్థిరరాశి fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    కొరకు(size_t i = 0; i < m; i++)
        కొరకు(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస శూన్య
copy_మాతృక(స్థిరరాశి fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    కొరకు(size_t i = 0; i < m; i++)
        కొరకు(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస శూన్య
సమితి_విలువ_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    కొరకు(size_t i = 0; i < m; i++)
        కొరకు(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస పూర్ణాంకం
CV_ANNOTATE_NO_SANITIZE_జ్ఞాపకం lapack_LU(fptype* a, size_t a_అడుగు, పూర్ణాంకం m, fptype* b, size_t b_అడుగు, పూర్ణాంకం n, పూర్ణాంకం* info)
{
#ఒకవేళ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::Auto_బఫరు<దీర్ఘ> piv_buff(m);
    దీర్ఘ lda = (దీర్ఘ)(a_అడుగు / పరిమాణం(fptype));
    దీర్ఘ _m = static_cast<దీర్ఘ>(m), _n = static_cast<దీర్ఘ>(n);
    దీర్ఘ _info[1];
#లేకపోతే
    cv::Auto_బఫరు<పూర్ణాంకం> piv_buff(m);
    పూర్ణాంకం lda = (పూర్ణాంకం)(a_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం _m = m, _n = n;
    పూర్ణాంకం* _info = info;
#అయితే_ముగింపు
    స్వయంచాలక piv = piv_buff.దత్తాంశం();

    transpose_వర్గం_inplace(a, lda, m);

    ఒకవేళ(b)
    {
        ఒకవేళ(n == 1 && b_అడుగు == పరిమాణం(fptype))
        {
            ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
                sgesv_(&_m, &_n, (దశాంశం*)a, &lda, piv, (దశాంశం*)b, &_m, _info);
            లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
                dgesv_(&_m, &_n, (ద్విగుణ*)a, &lda, piv, (ద్విగుణ*)b, &_m, _info);
        }
        లేకపోతే
        {
            పూర్ణాంకం ldb = (పూర్ణాంకం)(b_అడుగు / పరిమాణం(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
                sgesv_(&_m, &_n, (దశాంశం*)a, &lda, piv, (దశాంశం*)&tmpB[0], &_m, _info);
            లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
                dgesv_(&_m, &_n, (ద్విగుణ*)a, &lda, piv, (ద్విగుణ*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    లేకపోతే
    {
        ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
            sgetrf_(&_m, &_m, (దశాంశం*)a, &lda, piv, _info);
        లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
            dgetrf_(&_m, &_m, (ద్విగుణ*)a, &lda, piv, _info);
    }

#ఒకవేళ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<పూర్ణాంకం>(_info[0]);
#అయితే_ముగింపు
    పూర్ణాంకం retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    పూర్ణాంకం sign = 0;
    ఒకవేళ(*info == 0)
    {
        కొరకు(పూర్ణాంకం i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    లేకపోతే
        *info = 0; //in opencv LU function zero means error

    తిరిగి retcode;
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస పూర్ణాంకం
lapack_Cholesky(fptype* a, size_t a_అడుగు, పూర్ణాంకం m, fptype* b, size_t b_అడుగు, పూర్ణాంకం n, తార్కిక* info)
{
#ఒకవేళ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    దీర్ఘ _m = static_cast<దీర్ఘ>(m), _n = static_cast<దీర్ఘ>(n);
    దీర్ఘ lapack_స్థితి = 0;
    దీర్ఘ lda = (దీర్ఘ)(a_అడుగు / పరిమాణం(fptype));
#లేకపోతే
    పూర్ణాంకం _m = m, _n = n;
    పూర్ణాంకం lapack_స్థితి = 0;
    పూర్ణాంకం lda = (పూర్ణాంకం)(a_అడుగు / పరిమాణం(fptype));
#అయితే_ముగింపు
    అక్షరం L[] = {'L', '\0'};

    ఒకవేళ(b)
    {
        ఒకవేళ(n == 1 && b_అడుగు == పరిమాణం(fptype))
        {
            ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (దశాంశం*)a, &lda, (దశాంశం*)b, &_m, &lapack_స్థితి);
            లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ద్విగుణ*)a, &lda, (ద్విగుణ*)b, &_m, &lapack_స్థితి);
        }
        లేకపోతే
        {
            పూర్ణాంకం ldb = (పూర్ణాంకం)(b_అడుగు / పరిమాణం(fptype));
            fptype* tmpB = కొత్త fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (దశాంశం*)a, &lda, (దశాంశం*)tmpB, &_m, &lapack_స్థితి);
            లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ద్విగుణ*)a, &lda, (ద్విగుణ*)tmpB, &_m, &lapack_స్థితి);

            transpose(tmpB, m, b, ldb, n, m);
            తొలగించు[] tmpB;
        }
    }
    లేకపోతే
    {
        ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (దశాంశం*)a, &lda, &lapack_స్థితి);
        లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (ద్విగుణ*)a, &lda, &lapack_స్థితి);
    }

    ఒకవేళ(lapack_స్థితి == 0) *info = నిజం;
    లేకపోతే *info = అబద్ధం; //in opencv Cholesky function false means error

    తిరిగి lapack_స్థితి >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస పూర్ణాంకం
lapack_SVD(fptype* a, size_t a_అడుగు, fptype *w, fptype* u, size_t u_అడుగు, fptype* vt, size_t v_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం flags, పూర్ణాంకం* info)
{
#ఒకవేళ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    దీర్ఘ _m = static_cast<దీర్ఘ>(m), _n = static_cast<దీర్ఘ>(n);
    దీర్ఘ _info[1];
    దీర్ఘ lda = (దీర్ఘ)(a_అడుగు / పరిమాణం(fptype));
    దీర్ఘ ldv = (దీర్ఘ)(v_అడుగు / పరిమాణం(fptype));
    దీర్ఘ ldu = (దీర్ఘ)(u_అడుగు / పరిమాణం(fptype));
    దీర్ఘ lwork = -1;
    cv::Auto_బఫరు<దీర్ఘ> iworkBuf_(8 * std::min(m, n));
#లేకపోతే
    పూర్ణాంకం _m = m, _n = n;
    పూర్ణాంకం* _info = info;
    పూర్ణాంకం lda = (పూర్ణాంకం)(a_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం ldv = (పూర్ణాంకం)(v_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం ldu = (పూర్ణాంకం)(u_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం lwork = -1;
    cv::Auto_బఫరు<పూర్ణాంకం> iworkBuf_(8 * std::min(m, n));
#అయితే_ముగింపు
    స్వయంచాలక iworkBuf = iworkBuf_.దత్తాంశం();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    అక్షరం బహుళకం[] = { ' ', '\0'};
    ఒకవేళ(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        బహుళకం[0] = 'N';
    }
    లేకపోతే ఒకవేళ((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        బహుళకం[0] = 'O';
    లేకపోతే ఒకవేళ((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        బహుళకం[0] = 'S';
    లేకపోతే ఒకవేళ(flags & CV_HAL_SVD_నిండిన_UV) //full SVD, U stored in u or in a
        బహుళకం[0] = 'A';

    ఒకవేళ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_నిండిన_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
        OCV_LAPACK_FUNC(sgesdd)(బహుళకం, &_m, &_n, (దశాంశం*)a, &lda, (దశాంశం*)w, (దశాంశం*)u, &ldu, (దశాంశం*)vt, &ldv, (దశాంశం*)&work1, &lwork, iworkBuf, _info);
    లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
        OCV_LAPACK_FUNC(dgesdd)(బహుళకం, &_m, &_n, (ద్విగుణ*)a, &lda, (ద్విగుణ*)w, (ద్విగుణ*)u, &ldu, (ద్విగుణ*)vt, &ldv, (ద్విగుణ*)&work1, &lwork, iworkBuf, _info);

    ఒకవేళ(*info < 0)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (పూర్ణాంకం)round(work1); //optimal buffer size
    std::vector<fptype> బఫరు(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(బఫరు.దత్తాంశం(), పరిమాణం(fptype) * (lwork + 1));

    ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
        OCV_LAPACK_FUNC(sgesdd)(బహుళకం, &_m, &_n, (దశాంశం*)a, &lda, (దశాంశం*)w, (దశాంశం*)u, &ldu, (దశాంశం*)vt, &ldv, (దశాంశం*)&బఫరు[0], &lwork, iworkBuf, _info);
    లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
        OCV_LAPACK_FUNC(dgesdd)(బహుళకం, &_m, &_n, (ద్విగుణ*)a, &lda, (ద్విగుణ*)w, (ద్విగుణ*)u, &ldu, (ద్విగుణ*)vt, &ldv, (ద్విగుణ*)&బఫరు[0], &lwork, iworkBuf, _info);

#ఒకవేళ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<పూర్ణాంకం>(_info[0]);
#అయితే_ముగింపు

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(a, a_అడుగు * n);
    ఒకవేళ (u)
      CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(u, u_అడుగు * m);
    ఒకవేళ (vt)
      CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(vt, v_అడుగు * n);
    ఒకవేళ (w)
      CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(w, పరిమాణం(fptype) * std::min(m, n));

    ఒకవేళ(!(flags & CV_HAL_SVD_NO_UV))
        transpose_వర్గం_inplace(vt, ldv, n);

    ఒకవేళ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_నిండిన_UV))
    {
        కొరకు(పూర్ణాంకం i = 0; i < m; i++)
            కొరకు(పూర్ణాంకం j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    ఒకవేళ(*info < 0)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి CV_HAL_ERROR_OK;
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస పూర్ణాంకం
lapack_QR(fptype* a, size_t a_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, fptype* b, size_t b_అడుగు, fptype* dst, పూర్ణాంకం* info)
{
#ఒకవేళ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    దీర్ఘ _m = static_cast<దీర్ఘ>(m), _n = static_cast<దీర్ఘ>(n), _k = static_cast<దీర్ఘ>(k);
    దీర్ఘ _info[1];
    దీర్ఘ lda = (దీర్ఘ)(a_అడుగు / పరిమాణం(fptype));
    దీర్ఘ lwork = -1;
    దీర్ఘ ldtmpA;
#లేకపోతే
    పూర్ణాంకం _m = m, _n = n, _k = k;
    పూర్ణాంకం* _info = info;
    పూర్ణాంకం lda = (పూర్ణాంకం)(a_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం lwork = -1;
    పూర్ణాంకం ldtmpA;
#అయితే_ముగింపు

    అక్షరం బహుళకం[] = { 'N', '\0' };
    ఒకవేళ(m < n)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    ఒకవేళ (m == n)
    {
        transpose_వర్గం_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    లేకపోతే
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    ఒకవేళ (b)
    {
        ఒకవేళ (k == 1 && b_అడుగు == పరిమాణం(fptype))
        {
            ఒకవేళ (typeid(fptype) == typeid(దశాంశం))
                OCV_LAPACK_FUNC(sgels)(బహుళకం, &_m, &_n, &_k, (దశాంశం*)tmpA, &ldtmpA, (దశాంశం*)b, &_m, (దశాంశం*)&work1, &lwork, _info);
            లేకపోతే ఒకవేళ (typeid(fptype) == typeid(ద్విగుణ))
                OCV_LAPACK_FUNC(dgels)(బహుళకం, &_m, &_n, &_k, (ద్విగుణ*)tmpA, &ldtmpA, (ద్విగుణ*)b, &_m, (ద్విగుణ*)&work1, &lwork, _info);

            CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(info, పరిమాణం(పూర్ణాంకం));
            ఒకవేళ (*info < 0)
                తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_విడత(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* బఫరు = &workBufMemHolder.front();

            ఒకవేళ (typeid(fptype) == typeid(దశాంశం))
                OCV_LAPACK_FUNC(sgels)(బహుళకం, &_m, &_n, &_k, (దశాంశం*)tmpA, &ldtmpA, (దశాంశం*)b, &_m, (దశాంశం*)బఫరు, &lwork, _info);
            లేకపోతే ఒకవేళ (typeid(fptype) == typeid(ద్విగుణ))
                OCV_LAPACK_FUNC(dgels)(బహుళకం, &_m, &_n, &_k, (ద్విగుణ*)tmpA, &ldtmpA, (ద్విగుణ*)b, &_m, (ద్విగుణ*)బఫరు, &lwork, _info);
        }
        లేకపోతే
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            పూర్ణాంకం ldb = (పూర్ణాంకం)(b_అడుగు / పరిమాణం(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            ఒకవేళ (typeid(fptype) == typeid(దశాంశం))
                OCV_LAPACK_FUNC(sgels)(బహుళకం, &_m, &_n, &_k, (దశాంశం*)tmpA, &ldtmpA, (దశాంశం*)tmpB, &_m, (దశాంశం*)&work1, &lwork, _info);
            లేకపోతే ఒకవేళ (typeid(fptype) == typeid(ద్విగుణ))
                OCV_LAPACK_FUNC(dgels)(బహుళకం, &_m, &_n, &_k, (ద్విగుణ*)tmpA, &ldtmpA, (ద్విగుణ*)tmpB, &_m, (ద్విగుణ*)&work1, &lwork, _info);

            CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(info, పరిమాణం(పూర్ణాంకం));
            ఒకవేళ (*info < 0)
                తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_విడత(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* బఫరు = &workBufMemHolder.front();

            ఒకవేళ (typeid(fptype) == typeid(దశాంశం))
                OCV_LAPACK_FUNC(sgels)(బహుళకం, &_m, &_n, &_k, (దశాంశం*)tmpA, &ldtmpA, (దశాంశం*)tmpB, &_m, (దశాంశం*)బఫరు, &lwork, _info);
            లేకపోతే ఒకవేళ (typeid(fptype) == typeid(ద్విగుణ))
                OCV_LAPACK_FUNC(dgels)(బహుళకం, &_m, &_n, &_k, (ద్విగుణ*)tmpA, &ldtmpA, (ద్విగుణ*)tmpB, &_m, (ద్విగుణ*)బఫరు, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    లేకపోతే
    {
        ఒకవేళ (typeid(fptype) == typeid(దశాంశం))
            sgeqrf_(&_m, &_n, (దశాంశం*)tmpA, &ldtmpA, (దశాంశం*)dst, (దశాంశం*)&work1, &lwork, _info);
        లేకపోతే ఒకవేళ (typeid(fptype) == typeid(ద్విగుణ))
            dgeqrf_(&_m, &_n, (ద్విగుణ*)tmpA, &ldtmpA, (ద్విగుణ*)dst, (ద్విగుణ*)&work1, &lwork, _info);

        CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(info, పరిమాణం(పూర్ణాంకం));
        ఒకవేళ (*info < 0)
            తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cv_విడత(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* బఫరు = &workBufMemHolder.front();

        ఒకవేళ (typeid(fptype) == typeid(దశాంశం))
            sgeqrf_(&_m, &_n, (దశాంశం*)tmpA, &ldtmpA, (దశాంశం*)dst, (దశాంశం*)బఫరు, &lwork, _info);
        లేకపోతే ఒకవేళ (typeid(fptype) == typeid(ద్విగుణ))
            dgeqrf_(&_m, &_n, (ద్విగుణ*)tmpA, &ldtmpA, (ద్విగుణ*)dst, (ద్విగుణ*)బఫరు, &lwork, _info);
    }

    CV_ANNOTATE_జ్ఞాపకం_IS_INITIALIZED(info, పరిమాణం(పూర్ణాంకం));
    ఒకవేళ (m == n)
        transpose_వర్గం_inplace(a, lda, m);
    లేకపోతే
        transpose(tmpA, m, a, lda, n, m);

#ఒకవేళ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<పూర్ణాంకం>(_info[0]);
#అయితే_ముగింపు

    ఒకవేళ (*info != 0)
        *info = 0;
    లేకపోతే
        *info = 1;

    తిరిగి CV_HAL_ERROR_OK;
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస పూర్ణాంకం
lapack_gemm(స్థిరరాశి fptype *src1, size_t src1_అడుగు, స్థిరరాశి fptype *src2, size_t src2_అడుగు, fptype alpha,
            స్థిరరాశి fptype *src3, size_t src3_అడుగు, fptype beta, fptype *dst, size_t dst_అడుగు, పూర్ణాంకం a_m, పూర్ణాంకం a_n, పూర్ణాంకం d_n, పూర్ణాంకం flags)
{
    పూర్ణాంకం ldsrc1 = (పూర్ణాంకం)(src1_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం ldsrc2 = (పూర్ణాంకం)(src2_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం ldsrc3 = (పూర్ణాంకం)(src3_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం lddst = (పూర్ణాంకం)(dst_అడుగు / పరిమాణం(fptype));
    పూర్ణాంకం c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    ఒకవేళ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ఒకవేళ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        లేకపోతే
        {
            d_m = a_m;
        }
    }
    లేకపోతే
    {
        transB = CblasNoTrans;
        ఒకవేళ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        లేకపోతే
        {
            d_m = a_m;
        }
    }

    ఒకవేళ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    లేకపోతే
    {
        c_m = d_m;
        c_n = d_n;
    }

    ఒకవేళ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    లేకపోతే
    {
        transA = CblasNoTrans;
    }

    ఒకవేళ(src3 != dst && beta != 0.0 && src3_అడుగు != 0) {
        ఒకవేళ(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        లేకపోతే
            copy_మాతృక(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    లేకపోతే ఒకవేళ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    లేకపోతే ఒకవేళ(src3_అడుగు == 0 && beta != 0.0)
        సమితి_విలువ_2(dst, lddst, (fptype)0.0, d_m, d_n);

    ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
        cblas_sgemm(Cblas_అడ్డువరుస_Major, transA, transB, a_m, d_n, a_n, (దశాంశం)alpha, (దశాంశం*)src1, ldsrc1, (దశాంశం*)src2, ldsrc2, (దశాంశం)beta, (దశాంశం*)dst, lddst);
    లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
        cblas_dgemm(Cblas_అడ్డువరుస_Major, transA, transB, a_m, d_n, a_n, (ద్విగుణ)alpha, (ద్విగుణ*)src1, ldsrc1, (ద్విగుణ*)src2, ldsrc2, (ద్విగుణ)beta, (ద్విగుణ*)dst, lddst);

    తిరిగి CV_HAL_ERROR_OK;
}

మూస <రకం_పేరు fptype> స్థిర అంతఃవరుస పూర్ణాంకం
lapack_gemm_c(స్థిరరాశి fptype *src1, size_t src1_అడుగు, స్థిరరాశి fptype *src2, size_t src2_అడుగు, fptype alpha,
            స్థిరరాశి fptype *src3, size_t src3_అడుగు, fptype beta, fptype *dst, size_t dst_అడుగు, పూర్ణాంకం a_m, పూర్ణాంకం a_n, పూర్ణాంకం d_n, పూర్ణాంకం flags)
{
    పూర్ణాంకం ldsrc1 = (పూర్ణాంకం)(src1_అడుగు / పరిమాణం(std::complex<fptype>));
    పూర్ణాంకం ldsrc2 = (పూర్ణాంకం)(src2_అడుగు / పరిమాణం(std::complex<fptype>));
    పూర్ణాంకం ldsrc3 = (పూర్ణాంకం)(src3_అడుగు / పరిమాణం(std::complex<fptype>));
    పూర్ణాంకం lddst = (పూర్ణాంకం)(dst_అడుగు / పరిమాణం(std::complex<fptype>));
    పూర్ణాంకం c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    ఒకవేళ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ఒకవేళ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        లేకపోతే
        {
            d_m = a_m;
        }
    }
    లేకపోతే
    {
        transB = CblasNoTrans;
        ఒకవేళ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        లేకపోతే
        {
            d_m = a_m;
        }
    }

    ఒకవేళ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    లేకపోతే
    {
        c_m = d_m;
        c_n = d_n;
    }

    ఒకవేళ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    లేకపోతే
    {
        transA = CblasNoTrans;
    }

    ఒకవేళ(src3 != dst && beta != 0.0 && src3_అడుగు != 0) {
        ఒకవేళ(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        లేకపోతే
            copy_మాతృక((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    లేకపోతే ఒకవేళ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    లేకపోతే ఒకవేళ(src3_అడుగు == 0 && beta != 0.0)
        సమితి_విలువ_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#ఒకవేళ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    పూర్ణాంకం M = a_m, N = d_n, K = a_n;
    ఒకవేళ(typeid(fptype) == typeid(దశాంశం)) {
        స్వయంచాలక src1_cast = (std::complex<దశాంశం>*)(src1);
        స్వయంచాలక src2_cast = (std::complex<దశాంశం>*)(src2);
        స్వయంచాలక dst_cast = (std::complex<దశాంశం>*)(dst);
        దీర్ఘ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(Cblas_అడ్డువరుస_Major, transA, transB, M, N, K, (std::complex<దశాంశం>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<దశాంశం>*)&cBeta, dst_cast, ldc);
    }
    లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ)) {
        స్వయంచాలక src1_cast = (std::complex<ద్విగుణ>*)(src1);
        స్వయంచాలక src2_cast = (std::complex<ద్విగుణ>*)(src2);
        స్వయంచాలక dst_cast = (std::complex<ద్విగుణ>*)(dst);
        దీర్ఘ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(Cblas_అడ్డువరుస_Major, transA, transB, M, N, K, (std::complex<ద్విగుణ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ద్విగుణ>*)&cBeta, dst_cast, ldc);
    }
#లేకపోతే
    ఒకవేళ(typeid(fptype) == typeid(దశాంశం))
        cblas_cgemm(Cblas_అడ్డువరుస_Major, transA, transB, a_m, d_n, a_n, (దశాంశం*)reinterpret_cast<fptype(&)[2]>(cAlpha), (దశాంశం*)src1, ldsrc1, (దశాంశం*)src2, ldsrc2, (దశాంశం*)reinterpret_cast<fptype(&)[2]>(cBeta), (దశాంశం*)dst, lddst);
    లేకపోతే ఒకవేళ(typeid(fptype) == typeid(ద్విగుణ))
        cblas_zgemm(Cblas_అడ్డువరుస_Major, transA, transB, a_m, d_n, a_n, (ద్విగుణ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ద్విగుణ*)src1, ldsrc1, (ద్విగుణ*)src2, ldsrc2, (ద్విగుణ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ద్విగుణ*)dst, lddst);
#అయితే_ముగింపు

    తిరిగి CV_HAL_ERROR_OK;
}
పూర్ణాంకం lapack_LU32f(దశాంశం* a, size_t a_అడుగు, పూర్ణాంకం m, దశాంశం* b, size_t b_అడుగు, పూర్ణాంకం n, పూర్ణాంకం* info)
{
    ఒకవేళ(m < HAL_LU_SMALL_మాతృక_THRESH)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_LU(a, a_అడుగు, m, b, b_అడుగు, n, info);
}

పూర్ణాంకం lapack_LU64f(ద్విగుణ* a, size_t a_అడుగు, పూర్ణాంకం m, ద్విగుణ* b, size_t b_అడుగు, పూర్ణాంకం n, పూర్ణాంకం* info)
{
    ఒకవేళ(m < HAL_LU_SMALL_మాతృక_THRESH)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_LU(a, a_అడుగు, m, b, b_అడుగు, n, info);
}

పూర్ణాంకం lapack_Cholesky32f(దశాంశం* a, size_t a_అడుగు, పూర్ణాంకం m, దశాంశం* b, size_t b_అడుగు, పూర్ణాంకం n, తార్కిక *info)
{
    ఒకవేళ(m < HAL_CHOLESKY_SMALL_మాతృక_THRESH)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_Cholesky(a, a_అడుగు, m, b, b_అడుగు, n, info);
}

పూర్ణాంకం lapack_Cholesky64f(ద్విగుణ* a, size_t a_అడుగు, పూర్ణాంకం m, ద్విగుణ* b, size_t b_అడుగు, పూర్ణాంకం n, తార్కిక *info)
{
    ఒకవేళ(m < HAL_CHOLESKY_SMALL_మాతృక_THRESH)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_Cholesky(a, a_అడుగు, m, b, b_అడుగు, n, info);
}

పూర్ణాంకం lapack_SVD32f(దశాంశం* a, size_t a_అడుగు, దశాంశం *w, దశాంశం* u, size_t u_అడుగు, దశాంశం* vt, size_t v_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం flags)
{
    ఒకవేళ(m < HAL_SVD_SMALL_మాతృక_THRESH || n <= 0)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    పూర్ణాంకం info = 0;
    తిరిగి lapack_SVD(a, a_అడుగు, w, u, u_అడుగు, vt, v_అడుగు, m, n, flags, &info);
}

పూర్ణాంకం lapack_SVD64f(ద్విగుణ* a, size_t a_అడుగు, ద్విగుణ *w, ద్విగుణ* u, size_t u_అడుగు, ద్విగుణ* vt, size_t v_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం flags)
{
    ఒకవేళ(m < HAL_SVD_SMALL_మాతృక_THRESH || n <= 0)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    పూర్ణాంకం info = 0;
    తిరిగి lapack_SVD(a, a_అడుగు, w, u, u_అడుగు, vt, v_అడుగు, m, n, flags, &info);
}

పూర్ణాంకం lapack_QR32f(దశాంశం* src1, size_t src1_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, దశాంశం* src2, size_t src2_అడుగు, దశాంశం* dst, పూర్ణాంకం* info)
{
    ఒకవేళ (m < HAL_QR_SMALL_మాతృక_THRESH)
      తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_QR(src1, src1_అడుగు, m, n, k, src2, src2_అడుగు, dst, info);
}

పూర్ణాంకం lapack_QR64f(ద్విగుణ* src1, size_t src1_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, ద్విగుణ* src2, size_t src2_అడుగు, ద్విగుణ* dst, పూర్ణాంకం* info)
{
    ఒకవేళ (m < HAL_QR_SMALL_మాతృక_THRESH)
      తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_QR(src1, src1_అడుగు, m, n, k, src2, src2_అడుగు, dst, info);
}

పూర్ణాంకం lapack_gemm32f(స్థిరరాశి దశాంశం *src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2, size_t src2_అడుగు, దశాంశం alpha,
                   స్థిరరాశి దశాంశం *src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం *dst, size_t dst_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags)
{
    ఒకవేళ(m < HAL_GEMM_SMALL_మాతృక_THRESH)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_gemm(src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m, n, k, flags);
}

పూర్ణాంకం lapack_gemm64f(స్థిరరాశి ద్విగుణ *src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2, size_t src2_అడుగు, ద్విగుణ alpha,
                   స్థిరరాశి ద్విగుణ *src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ *dst, size_t dst_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags)
{
    ఒకవేళ(m < HAL_GEMM_SMALL_మాతృక_THRESH)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_gemm(src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m, n, k, flags);
}

పూర్ణాంకం lapack_gemm32fc(స్థిరరాశి దశాంశం *src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2, size_t src2_అడుగు, దశాంశం alpha,
                   స్థిరరాశి దశాంశం *src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం *dst, size_t dst_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags)
{
    ఒకవేళ(m < HAL_GEMM_SMALL_COMPLEX_మాతృక_THRESH)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_gemm_c(src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m, n, k, flags);
}
పూర్ణాంకం lapack_gemm64fc(స్థిరరాశి ద్విగుణ *src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2, size_t src2_అడుగు, ద్విగుణ alpha,
                   స్థిరరాశి ద్విగుణ *src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ *dst, size_t dst_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags)
{
    ఒకవేళ(m < HAL_GEMM_SMALL_COMPLEX_మాతృక_THRESH)
        తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED;
    తిరిగి lapack_gemm_c(src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m, n, k, flags);
}

#ఒకవేళ defined(__APPLE__) && defined(__clang__)
#సూచన clang diagnostic pop
#అయితే_ముగింపు

#అయితే_ముగింపు //HAVE_LAPACK
