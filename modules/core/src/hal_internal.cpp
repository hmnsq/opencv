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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "hal_internal.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LAPACK

#ಅನ್ತರ್ಭಾವ "opencv_lapack.h"

#ಅನ್ತರ್ಭಾವ <cmath>
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <typeinfo>
#ಅನ್ತರ್ಭಾವ <limits>
#ಅನ್ತರ್ಭಾವ <complex>
#ಅನ್ತರ್ಭಾವ <vector>

#ವ್ಯಾಖ್ಯಾನಿಸು HAL_GEMM_SMALL_COMPLEX_ಮಾತೃಕೆ_THRESH 100
#ವ್ಯಾಖ್ಯಾನಿಸು HAL_GEMM_SMALL_ಮಾತೃಕೆ_THRESH 100
#ವ್ಯಾಖ್ಯಾನಿಸು HAL_SVD_SMALL_ಮಾತೃಕೆ_THRESH 25
#ವ್ಯಾಖ್ಯಾನಿಸು HAL_QR_SMALL_ಮಾತೃಕೆ_THRESH 30
#ವ್ಯಾಖ್ಯಾನಿಸು HAL_LU_SMALL_ಮಾತೃಕೆ_THRESH 100
#ವ್ಯಾಖ್ಯಾನಿಸು HAL_CHOLESKY_SMALL_ಮಾತೃಕೆ_THRESH 100

#ಆದರೆ defined(__clang__) && defined(__has_feature)
#ಆದರೆ __has_feature(ಸ್ಮರಣೆ_sanitizer)
#ಅನ್ತರ್ಭಾವ <sanitizer/msan_interface.h>
#ವ್ಯಾಖ್ಯಾನಿಸು CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_ANNOTATE_NO_SANITIZE_ಸ್ಮರಣೆ __attribute__((no_sanitize("memory")))
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED
#ವ್ಯಾಖ್ಯಾನಿಸು CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(address, size) ಮಾಡು { } ಆಗ(0)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_ANNOTATE_NO_SANITIZE_ಸ್ಮರಣೆ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__APPLE__) && defined(__clang__)
#ಸೂಚನೆ clang diagnostic push
#ಸೂಚನೆ clang diagnostic ignored "-Wdeprecated-declarations"
#ಆದರೆ_ಅಂತ್ಯ

//lapack stores matrices in column-major order so transposing is needed everywhere
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
transpose_ವರ್ಗಫಲ_inplace(fptype *src, size_t src_ld, size_t m)
{
    ಗಾಗಿ(size_t i = 0; i < m - 1; i++)
        ಗಾಗಿ(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
transpose(ಸ್ಥಿರಾಂಕ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ಗಾಗಿ(size_t i = 0; i < m; i++)
        ಗಾಗಿ(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
copy_ಮಾತೃಕೆ(ಸ್ಥಿರಾಂಕ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ಗಾಗಿ(size_t i = 0; i < m; i++)
        ಗಾಗಿ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
ಗಣ_ಮೌಲ್ಯ_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    ಗಾಗಿ(size_t i = 0; i < m; i++)
        ಗಾಗಿ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
CV_ANNOTATE_NO_SANITIZE_ಸ್ಮರಣೆ lapack_LU(fptype* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, fptype* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ* info)
{
#ಆದರೆ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::Auto_ಬಫರ್<ದೀರ್ಘ> piv_buff(m);
    ದೀರ್ಘ lda = (ದೀರ್ಘ)(a_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ದೀರ್ಘ _m = static_cast<ದೀರ್ಘ>(m), _n = static_cast<ದೀರ್ಘ>(n);
    ದೀರ್ಘ _info[1];
#ಇಲ್ಲದಿದ್ದರೆ
    cv::Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> piv_buff(m);
    ಪೂರ್ಣಾಂಕ lda = (ಪೂರ್ಣಾಂಕ)(a_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ _m = m, _n = n;
    ಪೂರ್ಣಾಂಕ* _info = info;
#ಆದರೆ_ಅಂತ್ಯ
    ಸ್ವಯಂಚಾಲಿತ piv = piv_buff.ದತ್ತಾಂಶ();

    transpose_ವರ್ಗಫಲ_inplace(a, lda, m);

    ಆದರೆ(b)
    {
        ಆದರೆ(n == 1 && b_ಹೆಜ್ಜೆ == ಗಾತ್ರ(fptype))
        {
            ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
                sgesv_(&_m, &_n, (ದಶಮಾಂಶ*)a, &lda, piv, (ದಶಮಾಂಶ*)b, &_m, _info);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                dgesv_(&_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, piv, (ದ್ವಿಗುಣ*)b, &_m, _info);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಪೂರ್ಣಾಂಕ ldb = (ಪೂರ್ಣಾಂಕ)(b_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
                sgesv_(&_m, &_n, (ದಶಮಾಂಶ*)a, &lda, piv, (ದಶಮಾಂಶ*)&tmpB[0], &_m, _info);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                dgesv_(&_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, piv, (ದ್ವಿಗುಣ*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
            sgetrf_(&_m, &_m, (ದಶಮಾಂಶ*)a, &lda, piv, _info);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
            dgetrf_(&_m, &_m, (ದ್ವಿಗುಣ*)a, &lda, piv, _info);
    }

#ಆದರೆ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ಪೂರ್ಣಾಂಕ>(_info[0]);
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ಣಾಂಕ retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    ಪೂರ್ಣಾಂಕ sign = 0;
    ಆದರೆ(*info == 0)
    {
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        *info = 0; //in opencv LU function zero means error

    ಹಿಂತಿರುಗಿ retcode;
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
lapack_Cholesky(fptype* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, fptype* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ತಾರ್ಕಿಕ* info)
{
#ಆದರೆ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ದೀರ್ಘ _m = static_cast<ದೀರ್ಘ>(m), _n = static_cast<ದೀರ್ಘ>(n);
    ದೀರ್ಘ lapack_ಸ್ಥಿತಿ = 0;
    ದೀರ್ಘ lda = (ದೀರ್ಘ)(a_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ _m = m, _n = n;
    ಪೂರ್ಣಾಂಕ lapack_ಸ್ಥಿತಿ = 0;
    ಪೂರ್ಣಾಂಕ lda = (ಪೂರ್ಣಾಂಕ)(a_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
#ಆದರೆ_ಅಂತ್ಯ
    ಅಕ್ಷರ L[] = {'L', '\0'};

    ಆದರೆ(b)
    {
        ಆದರೆ(n == 1 && b_ಹೆಜ್ಜೆ == ಗಾತ್ರ(fptype))
        {
            ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ದಶಮಾಂಶ*)a, &lda, (ದಶಮಾಂಶ*)b, &_m, &lapack_ಸ್ಥಿತಿ);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, (ದ್ವಿಗುಣ*)b, &_m, &lapack_ಸ್ಥಿತಿ);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಪೂರ್ಣಾಂಕ ldb = (ಪೂರ್ಣಾಂಕ)(b_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
            fptype* tmpB = ಹೊಸ fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ದಶಮಾಂಶ*)a, &lda, (ದಶಮಾಂಶ*)tmpB, &_m, &lapack_ಸ್ಥಿತಿ);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, (ದ್ವಿಗುಣ*)tmpB, &_m, &lapack_ಸ್ಥಿತಿ);

            transpose(tmpB, m, b, ldb, n, m);
            ಅಳಿಸು[] tmpB;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (ದಶಮಾಂಶ*)a, &lda, &lapack_ಸ್ಥಿತಿ);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (ದ್ವಿಗುಣ*)a, &lda, &lapack_ಸ್ಥಿತಿ);
    }

    ಆದರೆ(lapack_ಸ್ಥಿತಿ == 0) *info = ನಿಜ;
    ಇಲ್ಲದಿದ್ದರೆ *info = ಸುಳ್ಳು; //in opencv Cholesky function false means error

    ಹಿಂತಿರುಗಿ lapack_ಸ್ಥಿತಿ >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
lapack_SVD(fptype* a, size_t a_ಹೆಜ್ಜೆ, fptype *w, fptype* u, size_t u_ಹೆಜ್ಜೆ, fptype* vt, size_t v_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ* info)
{
#ಆದರೆ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ದೀರ್ಘ _m = static_cast<ದೀರ್ಘ>(m), _n = static_cast<ದೀರ್ಘ>(n);
    ದೀರ್ಘ _info[1];
    ದೀರ್ಘ lda = (ದೀರ್ಘ)(a_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ದೀರ್ಘ ldv = (ದೀರ್ಘ)(v_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ದೀರ್ಘ ldu = (ದೀರ್ಘ)(u_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ದೀರ್ಘ lwork = -1;
    cv::Auto_ಬಫರ್<ದೀರ್ಘ> iworkBuf_(8 * std::min(m, n));
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ _m = m, _n = n;
    ಪೂರ್ಣಾಂಕ* _info = info;
    ಪೂರ್ಣಾಂಕ lda = (ಪೂರ್ಣಾಂಕ)(a_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ ldv = (ಪೂರ್ಣಾಂಕ)(v_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ ldu = (ಪೂರ್ಣಾಂಕ)(u_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ lwork = -1;
    cv::Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> iworkBuf_(8 * std::min(m, n));
#ಆದರೆ_ಅಂತ್ಯ
    ಸ್ವಯಂಚಾಲಿತ iworkBuf = iworkBuf_.ದತ್ತಾಂಶ();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    ಅಕ್ಷರ ಬಹುಲಕ[] = { ' ', '\0'};
    ಆದರೆ(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        ಬಹುಲಕ[0] = 'N';
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        ಬಹುಲಕ[0] = 'O';
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        ಬಹುಲಕ[0] = 'S';
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(flags & CV_HAL_SVD_ತುಂಬಿದ_UV) //full SVD, U stored in u or in a
        ಬಹುಲಕ[0] = 'A';

    ಆದರೆ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_ತುಂಬಿದ_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
        OCV_LAPACK_FUNC(sgesdd)(ಬಹುಲಕ, &_m, &_n, (ದಶಮಾಂಶ*)a, &lda, (ದಶಮಾಂಶ*)w, (ದಶಮಾಂಶ*)u, &ldu, (ದಶಮಾಂಶ*)vt, &ldv, (ದಶಮಾಂಶ*)&work1, &lwork, iworkBuf, _info);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
        OCV_LAPACK_FUNC(dgesdd)(ಬಹುಲಕ, &_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, (ದ್ವಿಗುಣ*)w, (ದ್ವಿಗುಣ*)u, &ldu, (ದ್ವಿಗುಣ*)vt, &ldv, (ದ್ವಿಗುಣ*)&work1, &lwork, iworkBuf, _info);

    ಆದರೆ(*info < 0)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (ಪೂರ್ಣಾಂಕ)round(work1); //optimal buffer size
    std::vector<fptype> ಬಫರ್(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(ಬಫರ್.ದತ್ತಾಂಶ(), ಗಾತ್ರ(fptype) * (lwork + 1));

    ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
        OCV_LAPACK_FUNC(sgesdd)(ಬಹುಲಕ, &_m, &_n, (ದಶಮಾಂಶ*)a, &lda, (ದಶಮಾಂಶ*)w, (ದಶಮಾಂಶ*)u, &ldu, (ದಶಮಾಂಶ*)vt, &ldv, (ದಶಮಾಂಶ*)&ಬಫರ್[0], &lwork, iworkBuf, _info);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
        OCV_LAPACK_FUNC(dgesdd)(ಬಹುಲಕ, &_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, (ದ್ವಿಗುಣ*)w, (ದ್ವಿಗುಣ*)u, &ldu, (ದ್ವಿಗುಣ*)vt, &ldv, (ದ್ವಿಗುಣ*)&ಬಫರ್[0], &lwork, iworkBuf, _info);

#ಆದರೆ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ಪೂರ್ಣಾಂಕ>(_info[0]);
#ಆದರೆ_ಅಂತ್ಯ

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(a, a_ಹೆಜ್ಜೆ * n);
    ಆದರೆ (u)
      CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(u, u_ಹೆಜ್ಜೆ * m);
    ಆದರೆ (vt)
      CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(vt, v_ಹೆಜ್ಜೆ * n);
    ಆದರೆ (w)
      CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(w, ಗಾತ್ರ(fptype) * std::min(m, n));

    ಆದರೆ(!(flags & CV_HAL_SVD_NO_UV))
        transpose_ವರ್ಗಫಲ_inplace(vt, ldv, n);

    ಆದರೆ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_ತುಂಬಿದ_UV))
    {
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < m; i++)
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    ಆದರೆ(*info < 0)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ CV_HAL_ERROR_OK;
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
lapack_QR(fptype* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, fptype* b, size_t b_ಹೆಜ್ಜೆ, fptype* dst, ಪೂರ್ಣಾಂಕ* info)
{
#ಆದರೆ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ದೀರ್ಘ _m = static_cast<ದೀರ್ಘ>(m), _n = static_cast<ದೀರ್ಘ>(n), _k = static_cast<ದೀರ್ಘ>(k);
    ದೀರ್ಘ _info[1];
    ದೀರ್ಘ lda = (ದೀರ್ಘ)(a_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ದೀರ್ಘ lwork = -1;
    ದೀರ್ಘ ldtmpA;
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ _m = m, _n = n, _k = k;
    ಪೂರ್ಣಾಂಕ* _info = info;
    ಪೂರ್ಣಾಂಕ lda = (ಪೂರ್ಣಾಂಕ)(a_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ lwork = -1;
    ಪೂರ್ಣಾಂಕ ldtmpA;
#ಆದರೆ_ಅಂತ್ಯ

    ಅಕ್ಷರ ಬಹುಲಕ[] = { 'N', '\0' };
    ಆದರೆ(m < n)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    ಆದರೆ (m == n)
    {
        transpose_ವರ್ಗಫಲ_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    ಆದರೆ (b)
    {
        ಆದರೆ (k == 1 && b_ಹೆಜ್ಜೆ == ಗಾತ್ರ(fptype))
        {
            ಆದರೆ (typeid(fptype) == typeid(ದಶಮಾಂಶ))
                OCV_LAPACK_FUNC(sgels)(ಬಹುಲಕ, &_m, &_n, &_k, (ದಶಮಾಂಶ*)tmpA, &ldtmpA, (ದಶಮಾಂಶ*)b, &_m, (ದಶಮಾಂಶ*)&work1, &lwork, _info);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dgels)(ಬಹುಲಕ, &_m, &_n, &_k, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)b, &_m, (ದ್ವಿಗುಣ*)&work1, &lwork, _info);

            CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(info, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
            ಆದರೆ (*info < 0)
                ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_ಸುತ್ತು(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* ಬಫರ್ = &workBufMemHolder.front();

            ಆದರೆ (typeid(fptype) == typeid(ದಶಮಾಂಶ))
                OCV_LAPACK_FUNC(sgels)(ಬಹುಲಕ, &_m, &_n, &_k, (ದಶಮಾಂಶ*)tmpA, &ldtmpA, (ದಶಮಾಂಶ*)b, &_m, (ದಶಮಾಂಶ*)ಬಫರ್, &lwork, _info);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dgels)(ಬಹುಲಕ, &_m, &_n, &_k, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)b, &_m, (ದ್ವಿಗುಣ*)ಬಫರ್, &lwork, _info);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            ಪೂರ್ಣಾಂಕ ldb = (ಪೂರ್ಣಾಂಕ)(b_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            ಆದರೆ (typeid(fptype) == typeid(ದಶಮಾಂಶ))
                OCV_LAPACK_FUNC(sgels)(ಬಹುಲಕ, &_m, &_n, &_k, (ದಶಮಾಂಶ*)tmpA, &ldtmpA, (ದಶಮಾಂಶ*)tmpB, &_m, (ದಶಮಾಂಶ*)&work1, &lwork, _info);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dgels)(ಬಹುಲಕ, &_m, &_n, &_k, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)tmpB, &_m, (ದ್ವಿಗುಣ*)&work1, &lwork, _info);

            CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(info, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
            ಆದರೆ (*info < 0)
                ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_ಸುತ್ತು(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* ಬಫರ್ = &workBufMemHolder.front();

            ಆದರೆ (typeid(fptype) == typeid(ದಶಮಾಂಶ))
                OCV_LAPACK_FUNC(sgels)(ಬಹುಲಕ, &_m, &_n, &_k, (ದಶಮಾಂಶ*)tmpA, &ldtmpA, (ದಶಮಾಂಶ*)tmpB, &_m, (ದಶಮಾಂಶ*)ಬಫರ್, &lwork, _info);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dgels)(ಬಹುಲಕ, &_m, &_n, &_k, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)tmpB, &_m, (ದ್ವಿಗುಣ*)ಬಫರ್, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (typeid(fptype) == typeid(ದಶಮಾಂಶ))
            sgeqrf_(&_m, &_n, (ದಶಮಾಂಶ*)tmpA, &ldtmpA, (ದಶಮಾಂಶ*)dst, (ದಶಮಾಂಶ*)&work1, &lwork, _info);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
            dgeqrf_(&_m, &_n, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)dst, (ದ್ವಿಗುಣ*)&work1, &lwork, _info);

        CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(info, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
        ಆದರೆ (*info < 0)
            ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cv_ಸುತ್ತು(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* ಬಫರ್ = &workBufMemHolder.front();

        ಆದರೆ (typeid(fptype) == typeid(ದಶಮಾಂಶ))
            sgeqrf_(&_m, &_n, (ದಶಮಾಂಶ*)tmpA, &ldtmpA, (ದಶಮಾಂಶ*)dst, (ದಶಮಾಂಶ*)ಬಫರ್, &lwork, _info);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
            dgeqrf_(&_m, &_n, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)dst, (ದ್ವಿಗುಣ*)ಬಫರ್, &lwork, _info);
    }

    CV_ANNOTATE_ಸ್ಮರಣೆ_IS_INITIALIZED(info, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
    ಆದರೆ (m == n)
        transpose_ವರ್ಗಫಲ_inplace(a, lda, m);
    ಇಲ್ಲದಿದ್ದರೆ
        transpose(tmpA, m, a, lda, n, m);

#ಆದರೆ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ಪೂರ್ಣಾಂಕ>(_info[0]);
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (*info != 0)
        *info = 0;
    ಇಲ್ಲದಿದ್ದರೆ
        *info = 1;

    ಹಿಂತಿರುಗಿ CV_HAL_ERROR_OK;
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
lapack_gemm(ಸ್ಥಿರಾಂಕ fptype *src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ fptype *src2, size_t src2_ಹೆಜ್ಜೆ, fptype alpha,
            ಸ್ಥಿರಾಂಕ fptype *src3, size_t src3_ಹೆಜ್ಜೆ, fptype beta, fptype *dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ a_m, ಪೂರ್ಣಾಂಕ a_n, ಪೂರ್ಣಾಂಕ d_n, ಪೂರ್ಣಾಂಕ flags)
{
    ಪೂರ್ಣಾಂಕ ldsrc1 = (ಪೂರ್ಣಾಂಕ)(src1_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ ldsrc2 = (ಪೂರ್ಣಾಂಕ)(src2_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ ldsrc3 = (ಪೂರ್ಣಾಂಕ)(src3_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ lddst = (ಪೂರ್ಣಾಂಕ)(dst_ಹೆಜ್ಜೆ / ಗಾತ್ರ(fptype));
    ಪೂರ್ಣಾಂಕ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    ಆದರೆ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ಆದರೆ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            d_m = a_m;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        transB = CblasNoTrans;
        ಆದರೆ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            d_m = a_m;
        }
    }

    ಆದರೆ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        c_m = d_m;
        c_n = d_n;
    }

    ಆದರೆ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        transA = CblasNoTrans;
    }

    ಆದರೆ(src3 != dst && beta != 0.0 && src3_ಹೆಜ್ಜೆ != 0) {
        ಆದರೆ(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        ಇಲ್ಲದಿದ್ದರೆ
            copy_ಮಾತೃಕೆ(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(src3_ಹೆಜ್ಜೆ == 0 && beta != 0.0)
        ಗಣ_ಮೌಲ್ಯ_2(dst, lddst, (fptype)0.0, d_m, d_n);

    ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
        cblas_sgemm(Cblas_ಅಡ್ಡಸಾಲು_Major, transA, transB, a_m, d_n, a_n, (ದಶಮಾಂಶ)alpha, (ದಶಮಾಂಶ*)src1, ldsrc1, (ದಶಮಾಂಶ*)src2, ldsrc2, (ದಶಮಾಂಶ)beta, (ದಶಮಾಂಶ*)dst, lddst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
        cblas_dgemm(Cblas_ಅಡ್ಡಸಾಲು_Major, transA, transB, a_m, d_n, a_n, (ದ್ವಿಗುಣ)alpha, (ದ್ವಿಗುಣ*)src1, ldsrc1, (ದ್ವಿಗುಣ*)src2, ldsrc2, (ದ್ವಿಗುಣ)beta, (ದ್ವಿಗುಣ*)dst, lddst);

    ಹಿಂತಿರುಗಿ CV_HAL_ERROR_OK;
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು fptype> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
lapack_gemm_c(ಸ್ಥಿರಾಂಕ fptype *src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ fptype *src2, size_t src2_ಹೆಜ್ಜೆ, fptype alpha,
            ಸ್ಥಿರಾಂಕ fptype *src3, size_t src3_ಹೆಜ್ಜೆ, fptype beta, fptype *dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ a_m, ಪೂರ್ಣಾಂಕ a_n, ಪೂರ್ಣಾಂಕ d_n, ಪೂರ್ಣಾಂಕ flags)
{
    ಪೂರ್ಣಾಂಕ ldsrc1 = (ಪೂರ್ಣಾಂಕ)(src1_ಹೆಜ್ಜೆ / ಗಾತ್ರ(std::complex<fptype>));
    ಪೂರ್ಣಾಂಕ ldsrc2 = (ಪೂರ್ಣಾಂಕ)(src2_ಹೆಜ್ಜೆ / ಗಾತ್ರ(std::complex<fptype>));
    ಪೂರ್ಣಾಂಕ ldsrc3 = (ಪೂರ್ಣಾಂಕ)(src3_ಹೆಜ್ಜೆ / ಗಾತ್ರ(std::complex<fptype>));
    ಪೂರ್ಣಾಂಕ lddst = (ಪೂರ್ಣಾಂಕ)(dst_ಹೆಜ್ಜೆ / ಗಾತ್ರ(std::complex<fptype>));
    ಪೂರ್ಣಾಂಕ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    ಆದರೆ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ಆದರೆ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            d_m = a_m;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        transB = CblasNoTrans;
        ಆದರೆ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            d_m = a_m;
        }
    }

    ಆದರೆ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        c_m = d_m;
        c_n = d_n;
    }

    ಆದರೆ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        transA = CblasNoTrans;
    }

    ಆದರೆ(src3 != dst && beta != 0.0 && src3_ಹೆಜ್ಜೆ != 0) {
        ಆದರೆ(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        ಇಲ್ಲದಿದ್ದರೆ
            copy_ಮಾತೃಕೆ((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(src3_ಹೆಜ್ಜೆ == 0 && beta != 0.0)
        ಗಣ_ಮೌಲ್ಯ_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#ಆದರೆ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ಪೂರ್ಣಾಂಕ M = a_m, N = d_n, K = a_n;
    ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ)) {
        ಸ್ವಯಂಚಾಲಿತ src1_cast = (std::complex<ದಶಮಾಂಶ>*)(src1);
        ಸ್ವಯಂಚಾಲಿತ src2_cast = (std::complex<ದಶಮಾಂಶ>*)(src2);
        ಸ್ವಯಂಚಾಲಿತ dst_cast = (std::complex<ದಶಮಾಂಶ>*)(dst);
        ದೀರ್ಘ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(Cblas_ಅಡ್ಡಸಾಲು_Major, transA, transB, M, N, K, (std::complex<ದಶಮಾಂಶ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ದಶಮಾಂಶ>*)&cBeta, dst_cast, ldc);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ)) {
        ಸ್ವಯಂಚಾಲಿತ src1_cast = (std::complex<ದ್ವಿಗುಣ>*)(src1);
        ಸ್ವಯಂಚಾಲಿತ src2_cast = (std::complex<ದ್ವಿಗುಣ>*)(src2);
        ಸ್ವಯಂಚಾಲಿತ dst_cast = (std::complex<ದ್ವಿಗುಣ>*)(dst);
        ದೀರ್ಘ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(Cblas_ಅಡ್ಡಸಾಲು_Major, transA, transB, M, N, K, (std::complex<ದ್ವಿಗುಣ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ದ್ವಿಗುಣ>*)&cBeta, dst_cast, ldc);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ(typeid(fptype) == typeid(ದಶಮಾಂಶ))
        cblas_cgemm(Cblas_ಅಡ್ಡಸಾಲು_Major, transA, transB, a_m, d_n, a_n, (ದಶಮಾಂಶ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ದಶಮಾಂಶ*)src1, ldsrc1, (ದಶಮಾಂಶ*)src2, ldsrc2, (ದಶಮಾಂಶ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ದಶಮಾಂಶ*)dst, lddst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
        cblas_zgemm(Cblas_ಅಡ್ಡಸಾಲು_Major, transA, transB, a_m, d_n, a_n, (ದ್ವಿಗುಣ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ದ್ವಿಗುಣ*)src1, ldsrc1, (ದ್ವಿಗುಣ*)src2, ldsrc2, (ದ್ವಿಗುಣ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ದ್ವಿಗುಣ*)dst, lddst);
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ CV_HAL_ERROR_OK;
}
ಪೂರ್ಣಾಂಕ lapack_LU32f(ದಶಮಾಂಶ* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ* info)
{
    ಆದರೆ(m < HAL_LU_SMALL_ಮಾತೃಕೆ_THRESH)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_LU(a, a_ಹೆಜ್ಜೆ, m, b, b_ಹೆಜ್ಜೆ, n, info);
}

ಪೂರ್ಣಾಂಕ lapack_LU64f(ದ್ವಿಗುಣ* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ* info)
{
    ಆದರೆ(m < HAL_LU_SMALL_ಮಾತೃಕೆ_THRESH)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_LU(a, a_ಹೆಜ್ಜೆ, m, b, b_ಹೆಜ್ಜೆ, n, info);
}

ಪೂರ್ಣಾಂಕ lapack_Cholesky32f(ದಶಮಾಂಶ* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ತಾರ್ಕಿಕ *info)
{
    ಆದರೆ(m < HAL_CHOLESKY_SMALL_ಮಾತೃಕೆ_THRESH)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_Cholesky(a, a_ಹೆಜ್ಜೆ, m, b, b_ಹೆಜ್ಜೆ, n, info);
}

ಪೂರ್ಣಾಂಕ lapack_Cholesky64f(ದ್ವಿಗುಣ* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ತಾರ್ಕಿಕ *info)
{
    ಆದರೆ(m < HAL_CHOLESKY_SMALL_ಮಾತೃಕೆ_THRESH)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_Cholesky(a, a_ಹೆಜ್ಜೆ, m, b, b_ಹೆಜ್ಜೆ, n, info);
}

ಪೂರ್ಣಾಂಕ lapack_SVD32f(ದಶಮಾಂಶ* a, size_t a_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *w, ದಶಮಾಂಶ* u, size_t u_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ* vt, size_t v_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ flags)
{
    ಆದರೆ(m < HAL_SVD_SMALL_ಮಾತೃಕೆ_THRESH || n <= 0)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪೂರ್ಣಾಂಕ info = 0;
    ಹಿಂತಿರುಗಿ lapack_SVD(a, a_ಹೆಜ್ಜೆ, w, u, u_ಹೆಜ್ಜೆ, vt, v_ಹೆಜ್ಜೆ, m, n, flags, &info);
}

ಪೂರ್ಣಾಂಕ lapack_SVD64f(ದ್ವಿಗುಣ* a, size_t a_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *w, ದ್ವಿಗುಣ* u, size_t u_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ* vt, size_t v_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ flags)
{
    ಆದರೆ(m < HAL_SVD_SMALL_ಮಾತೃಕೆ_THRESH || n <= 0)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪೂರ್ಣಾಂಕ info = 0;
    ಹಿಂತಿರುಗಿ lapack_SVD(a, a_ಹೆಜ್ಜೆ, w, u, u_ಹೆಜ್ಜೆ, vt, v_ಹೆಜ್ಜೆ, m, n, flags, &info);
}

ಪೂರ್ಣಾಂಕ lapack_QR32f(ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ* info)
{
    ಆದರೆ (m < HAL_QR_SMALL_ಮಾತೃಕೆ_THRESH)
      ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_QR(src1, src1_ಹೆಜ್ಜೆ, m, n, k, src2, src2_ಹೆಜ್ಜೆ, dst, info);
}

ಪೂರ್ಣಾಂಕ lapack_QR64f(ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ* info)
{
    ಆದರೆ (m < HAL_QR_SMALL_ಮಾತೃಕೆ_THRESH)
      ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_QR(src1, src1_ಹೆಜ್ಜೆ, m, n, k, src2, src2_ಹೆಜ್ಜೆ, dst, info);
}

ಪೂರ್ಣಾಂಕ lapack_gemm32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ alpha,
                   ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ *dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags)
{
    ಆದರೆ(m < HAL_GEMM_SMALL_ಮಾತೃಕೆ_THRESH)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_gemm(src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m, n, k, flags);
}

ಪೂರ್ಣಾಂಕ lapack_gemm64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ alpha,
                   ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ *dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags)
{
    ಆದರೆ(m < HAL_GEMM_SMALL_ಮಾತೃಕೆ_THRESH)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_gemm(src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m, n, k, flags);
}

ಪೂರ್ಣಾಂಕ lapack_gemm32fc(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ alpha,
                   ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ *dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags)
{
    ಆದರೆ(m < HAL_GEMM_SMALL_COMPLEX_ಮಾತೃಕೆ_THRESH)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_gemm_c(src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m, n, k, flags);
}
ಪೂರ್ಣಾಂಕ lapack_gemm64fc(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ alpha,
                   ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ *dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags)
{
    ಆದರೆ(m < HAL_GEMM_SMALL_COMPLEX_ಮಾತೃಕೆ_THRESH)
        ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಹಿಂತಿರುಗಿ lapack_gemm_c(src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m, n, k, flags);
}

#ಆದರೆ defined(__APPLE__) && defined(__clang__)
#ಸೂಚನೆ clang diagnostic pop
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ //HAVE_LAPACK
