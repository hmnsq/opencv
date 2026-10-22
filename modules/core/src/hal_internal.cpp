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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "hal_internal.hpp"

#ifdef HAVE_LAPACK

#অন্তর্ভাব "opencv_lapack.h"

#অন্তর্ভাব <cmath>
#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <typeinfo>
#অন্তর্ভাব <limits>
#অন্তর্ভাব <complex>
#অন্তর্ভাব <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#যদি defined(__clang__) && defined(__has_feature)
#যদি __has_feature(memory_sanitizer)
#অন্তর্ভাব <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) করো { } যখন(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#যদি defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> স্থির অন্তঃরেখ শূন্য
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    জন্য(size_t i = 0; i < m - 1; i++)
        জন্য(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> স্থির অন্তঃরেখ শূন্য
transpose(ধ্রুবক fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    জন্য(size_t i = 0; i < m; i++)
        জন্য(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> স্থির অন্তঃরেখ শূন্য
copy_matrix(ধ্রুবক fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    জন্য(size_t i = 0; i < m; i++)
        জন্য(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> স্থির অন্তঃরেখ শূন্য
set_মান_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    জন্য(size_t i = 0; i < m; i++)
        জন্য(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> স্থির অন্তঃরেখ পূর্ণসংখ্যা
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, পূর্ণসংখ্যা m, fptype* b, size_t b_step, পূর্ণসংখ্যা n, পূর্ণসংখ্যা* info)
{
#যদি defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::Auto_বাফার<long> piv_buff(m);
    long lda = (long)(a_step / আকার(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#অন্যথা
    cv::Auto_বাফার<পূর্ণসংখ্যা> piv_buff(m);
    পূর্ণসংখ্যা lda = (পূর্ণসংখ্যা)(a_step / আকার(fptype));
    পূর্ণসংখ্যা _m = m, _n = n;
    পূর্ণসংখ্যা* _info = info;
#endif
    auto piv = piv_buff.তথ্য();

    transpose_square_inplace(a, lda, m);

    যদি(b)
    {
        যদি(n == 1 && b_step == আকার(fptype))
        {
            যদি(typeid(fptype) == typeid(দশমিক))
                sgesv_(&_m, &_n, (দশমিক*)a, &lda, piv, (দশমিক*)b, &_m, _info);
            অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
                dgesv_(&_m, &_n, (দ্বিগুণ*)a, &lda, piv, (দ্বিগুণ*)b, &_m, _info);
        }
        অন্যথা
        {
            পূর্ণসংখ্যা ldb = (পূর্ণসংখ্যা)(b_step / আকার(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            যদি(typeid(fptype) == typeid(দশমিক))
                sgesv_(&_m, &_n, (দশমিক*)a, &lda, piv, (দশমিক*)&tmpB[0], &_m, _info);
            অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
                dgesv_(&_m, &_n, (দ্বিগুণ*)a, &lda, piv, (দ্বিগুণ*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    অন্যথা
    {
        যদি(typeid(fptype) == typeid(দশমিক))
            sgetrf_(&_m, &_m, (দশমিক*)a, &lda, piv, _info);
        অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
            dgetrf_(&_m, &_m, (দ্বিগুণ*)a, &lda, piv, _info);
    }

#যদি defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<পূর্ণসংখ্যা>(_info[0]);
#endif
    পূর্ণসংখ্যা retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    পূর্ণসংখ্যা sign = 0;
    যদি(*info == 0)
    {
        জন্য(পূর্ণসংখ্যা i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    অন্যথা
        *info = 0; //in opencv LU function zero means error

    ফেরত retcode;
}

template <typename fptype> স্থির অন্তঃরেখ পূর্ণসংখ্যা
lapack_Cholesky(fptype* a, size_t a_step, পূর্ণসংখ্যা m, fptype* b, size_t b_step, পূর্ণসংখ্যা n, যৌক্তিক* info)
{
#যদি defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / আকার(fptype));
#অন্যথা
    পূর্ণসংখ্যা _m = m, _n = n;
    পূর্ণসংখ্যা lapackStatus = 0;
    পূর্ণসংখ্যা lda = (পূর্ণসংখ্যা)(a_step / আকার(fptype));
#endif
    অক্ষর L[] = {'L', '\0'};

    যদি(b)
    {
        যদি(n == 1 && b_step == আকার(fptype))
        {
            যদি(typeid(fptype) == typeid(দশমিক))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (দশমিক*)a, &lda, (দশমিক*)b, &_m, &lapackStatus);
            অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (দ্বিগুণ*)a, &lda, (দ্বিগুণ*)b, &_m, &lapackStatus);
        }
        অন্যথা
        {
            পূর্ণসংখ্যা ldb = (পূর্ণসংখ্যা)(b_step / আকার(fptype));
            fptype* tmpB = নতুন fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            যদি(typeid(fptype) == typeid(দশমিক))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (দশমিক*)a, &lda, (দশমিক*)tmpB, &_m, &lapackStatus);
            অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (দ্বিগুণ*)a, &lda, (দ্বিগুণ*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    অন্যথা
    {
        যদি(typeid(fptype) == typeid(দশমিক))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (দশমিক*)a, &lda, &lapackStatus);
        অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (দ্বিগুণ*)a, &lda, &lapackStatus);
    }

    যদি(lapackStatus == 0) *info = সত্য;
    অন্যথা *info = মিথ্যা; //in opencv Cholesky function false means error

    ফেরত lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> স্থির অন্তঃরেখ পূর্ণসংখ্যা
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা* info)
{
#যদি defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / আকার(fptype));
    long ldv = (long)(v_step / আকার(fptype));
    long ldu = (long)(u_step / আকার(fptype));
    long lwork = -1;
    cv::Auto_বাফার<long> iworkBuf_(8 * std::min(m, n));
#অন্যথা
    পূর্ণসংখ্যা _m = m, _n = n;
    পূর্ণসংখ্যা* _info = info;
    পূর্ণসংখ্যা lda = (পূর্ণসংখ্যা)(a_step / আকার(fptype));
    পূর্ণসংখ্যা ldv = (পূর্ণসংখ্যা)(v_step / আকার(fptype));
    পূর্ণসংখ্যা ldu = (পূর্ণসংখ্যা)(u_step / আকার(fptype));
    পূর্ণসংখ্যা lwork = -1;
    cv::Auto_বাফার<পূর্ণসংখ্যা> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.তথ্য();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    অক্ষর mode[] = { ' ', '\0'};
    যদি(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    অন্যথা যদি((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    অন্যথা যদি((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    অন্যথা যদি(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    যদি((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    যদি(typeid(fptype) == typeid(দশমিক))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (দশমিক*)a, &lda, (দশমিক*)w, (দশমিক*)u, &ldu, (দশমিক*)vt, &ldv, (দশমিক*)&work1, &lwork, iworkBuf, _info);
    অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (দ্বিগুণ*)a, &lda, (দ্বিগুণ*)w, (দ্বিগুণ*)u, &ldu, (দ্বিগুণ*)vt, &ldv, (দ্বিগুণ*)&work1, &lwork, iworkBuf, _info);

    যদি(*info < 0)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (পূর্ণসংখ্যা)round(work1); //optimal buffer size
    std::vector<fptype> বাফার(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(বাফার.তথ্য(), আকার(fptype) * (lwork + 1));

    যদি(typeid(fptype) == typeid(দশমিক))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (দশমিক*)a, &lda, (দশমিক*)w, (দশমিক*)u, &ldu, (দশমিক*)vt, &ldv, (দশমিক*)&বাফার[0], &lwork, iworkBuf, _info);
    অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (দ্বিগুণ*)a, &lda, (দ্বিগুণ*)w, (দ্বিগুণ*)u, &ldu, (দ্বিগুণ*)vt, &ldv, (দ্বিগুণ*)&বাফার[0], &lwork, iworkBuf, _info);

#যদি defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<পূর্ণসংখ্যা>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    যদি (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    যদি (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    যদি (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, আকার(fptype) * std::min(m, n));

    যদি(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    যদি((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        জন্য(পূর্ণসংখ্যা i = 0; i < m; i++)
            জন্য(পূর্ণসংখ্যা j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    যদি(*info < 0)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত CV_HAL_ERROR_OK;
}

template <typename fptype> স্থির অন্তঃরেখ পূর্ণসংখ্যা
lapack_QR(fptype* a, size_t a_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, fptype* b, size_t b_step, fptype* dst, পূর্ণসংখ্যা* info)
{
#যদি defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / আকার(fptype));
    long lwork = -1;
    long ldtmpA;
#অন্যথা
    পূর্ণসংখ্যা _m = m, _n = n, _k = k;
    পূর্ণসংখ্যা* _info = info;
    পূর্ণসংখ্যা lda = (পূর্ণসংখ্যা)(a_step / আকার(fptype));
    পূর্ণসংখ্যা lwork = -1;
    পূর্ণসংখ্যা ldtmpA;
#endif

    অক্ষর mode[] = { 'N', '\0' };
    যদি(m < n)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    যদি (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    অন্যথা
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    যদি (b)
    {
        যদি (k == 1 && b_step == আকার(fptype))
        {
            যদি (typeid(fptype) == typeid(দশমিক))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (দশমিক*)tmpA, &ldtmpA, (দশমিক*)b, &_m, (দশমিক*)&work1, &lwork, _info);
            অন্যথা যদি (typeid(fptype) == typeid(দ্বিগুণ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (দ্বিগুণ*)tmpA, &ldtmpA, (দ্বিগুণ*)b, &_m, (দ্বিগুণ*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, আকার(পূর্ণসংখ্যা));
            যদি (*info < 0)
                ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* বাফার = &workBufMemHolder.front();

            যদি (typeid(fptype) == typeid(দশমিক))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (দশমিক*)tmpA, &ldtmpA, (দশমিক*)b, &_m, (দশমিক*)বাফার, &lwork, _info);
            অন্যথা যদি (typeid(fptype) == typeid(দ্বিগুণ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (দ্বিগুণ*)tmpA, &ldtmpA, (দ্বিগুণ*)b, &_m, (দ্বিগুণ*)বাফার, &lwork, _info);
        }
        অন্যথা
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            পূর্ণসংখ্যা ldb = (পূর্ণসংখ্যা)(b_step / আকার(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            যদি (typeid(fptype) == typeid(দশমিক))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (দশমিক*)tmpA, &ldtmpA, (দশমিক*)tmpB, &_m, (দশমিক*)&work1, &lwork, _info);
            অন্যথা যদি (typeid(fptype) == typeid(দ্বিগুণ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (দ্বিগুণ*)tmpA, &ldtmpA, (দ্বিগুণ*)tmpB, &_m, (দ্বিগুণ*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, আকার(পূর্ণসংখ্যা));
            যদি (*info < 0)
                ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* বাফার = &workBufMemHolder.front();

            যদি (typeid(fptype) == typeid(দশমিক))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (দশমিক*)tmpA, &ldtmpA, (দশমিক*)tmpB, &_m, (দশমিক*)বাফার, &lwork, _info);
            অন্যথা যদি (typeid(fptype) == typeid(দ্বিগুণ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (দ্বিগুণ*)tmpA, &ldtmpA, (দ্বিগুণ*)tmpB, &_m, (দ্বিগুণ*)বাফার, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    অন্যথা
    {
        যদি (typeid(fptype) == typeid(দশমিক))
            sgeqrf_(&_m, &_n, (দশমিক*)tmpA, &ldtmpA, (দশমিক*)dst, (দশমিক*)&work1, &lwork, _info);
        অন্যথা যদি (typeid(fptype) == typeid(দ্বিগুণ))
            dgeqrf_(&_m, &_n, (দ্বিগুণ*)tmpA, &ldtmpA, (দ্বিগুণ*)dst, (দ্বিগুণ*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, আকার(পূর্ণসংখ্যা));
        যদি (*info < 0)
            ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* বাফার = &workBufMemHolder.front();

        যদি (typeid(fptype) == typeid(দশমিক))
            sgeqrf_(&_m, &_n, (দশমিক*)tmpA, &ldtmpA, (দশমিক*)dst, (দশমিক*)বাফার, &lwork, _info);
        অন্যথা যদি (typeid(fptype) == typeid(দ্বিগুণ))
            dgeqrf_(&_m, &_n, (দ্বিগুণ*)tmpA, &ldtmpA, (দ্বিগুণ*)dst, (দ্বিগুণ*)বাফার, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, আকার(পূর্ণসংখ্যা));
    যদি (m == n)
        transpose_square_inplace(a, lda, m);
    অন্যথা
        transpose(tmpA, m, a, lda, n, m);

#যদি defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<পূর্ণসংখ্যা>(_info[0]);
#endif

    যদি (*info != 0)
        *info = 0;
    অন্যথা
        *info = 1;

    ফেরত CV_HAL_ERROR_OK;
}

template <typename fptype> স্থির অন্তঃরেখ পূর্ণসংখ্যা
lapack_gemm(ধ্রুবক fptype *src1, size_t src1_step, ধ্রুবক fptype *src2, size_t src2_step, fptype alpha,
            ধ্রুবক fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, পূর্ণসংখ্যা a_m, পূর্ণসংখ্যা a_n, পূর্ণসংখ্যা d_n, পূর্ণসংখ্যা flags)
{
    পূর্ণসংখ্যা ldsrc1 = (পূর্ণসংখ্যা)(src1_step / আকার(fptype));
    পূর্ণসংখ্যা ldsrc2 = (পূর্ণসংখ্যা)(src2_step / আকার(fptype));
    পূর্ণসংখ্যা ldsrc3 = (পূর্ণসংখ্যা)(src3_step / আকার(fptype));
    পূর্ণসংখ্যা lddst = (পূর্ণসংখ্যা)(dst_step / আকার(fptype));
    পূর্ণসংখ্যা c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    যদি(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        যদি(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        অন্যথা
        {
            d_m = a_m;
        }
    }
    অন্যথা
    {
        transB = CblasNoTrans;
        যদি(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        অন্যথা
        {
            d_m = a_m;
        }
    }

    যদি(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    অন্যথা
    {
        c_m = d_m;
        c_n = d_n;
    }

    যদি(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    অন্যথা
    {
        transA = CblasNoTrans;
    }

    যদি(src3 != dst && beta != 0.0 && src3_step != 0) {
        যদি(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        অন্যথা
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    অন্যথা যদি (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    অন্যথা যদি(src3_step == 0 && beta != 0.0)
        set_মান_2(dst, lddst, (fptype)0.0, d_m, d_n);

    যদি(typeid(fptype) == typeid(দশমিক))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (দশমিক)alpha, (দশমিক*)src1, ldsrc1, (দশমিক*)src2, ldsrc2, (দশমিক)beta, (দশমিক*)dst, lddst);
    অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (দ্বিগুণ)alpha, (দ্বিগুণ*)src1, ldsrc1, (দ্বিগুণ*)src2, ldsrc2, (দ্বিগুণ)beta, (দ্বিগুণ*)dst, lddst);

    ফেরত CV_HAL_ERROR_OK;
}

template <typename fptype> স্থির অন্তঃরেখ পূর্ণসংখ্যা
lapack_gemm_c(ধ্রুবক fptype *src1, size_t src1_step, ধ্রুবক fptype *src2, size_t src2_step, fptype alpha,
            ধ্রুবক fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, পূর্ণসংখ্যা a_m, পূর্ণসংখ্যা a_n, পূর্ণসংখ্যা d_n, পূর্ণসংখ্যা flags)
{
    পূর্ণসংখ্যা ldsrc1 = (পূর্ণসংখ্যা)(src1_step / আকার(std::complex<fptype>));
    পূর্ণসংখ্যা ldsrc2 = (পূর্ণসংখ্যা)(src2_step / আকার(std::complex<fptype>));
    পূর্ণসংখ্যা ldsrc3 = (পূর্ণসংখ্যা)(src3_step / আকার(std::complex<fptype>));
    পূর্ণসংখ্যা lddst = (পূর্ণসংখ্যা)(dst_step / আকার(std::complex<fptype>));
    পূর্ণসংখ্যা c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    যদি(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        যদি(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        অন্যথা
        {
            d_m = a_m;
        }
    }
    অন্যথা
    {
        transB = CblasNoTrans;
        যদি(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        অন্যথা
        {
            d_m = a_m;
        }
    }

    যদি(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    অন্যথা
    {
        c_m = d_m;
        c_n = d_n;
    }

    যদি(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    অন্যথা
    {
        transA = CblasNoTrans;
    }

    যদি(src3 != dst && beta != 0.0 && src3_step != 0) {
        যদি(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        অন্যথা
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    অন্যথা যদি (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    অন্যথা যদি(src3_step == 0 && beta != 0.0)
        set_মান_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#যদি defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    পূর্ণসংখ্যা M = a_m, N = d_n, K = a_n;
    যদি(typeid(fptype) == typeid(দশমিক)) {
        auto src1_cast = (std::complex<দশমিক>*)(src1);
        auto src2_cast = (std::complex<দশমিক>*)(src2);
        auto dst_cast = (std::complex<দশমিক>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<দশমিক>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<দশমিক>*)&cBeta, dst_cast, ldc);
    }
    অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ)) {
        auto src1_cast = (std::complex<দ্বিগুণ>*)(src1);
        auto src2_cast = (std::complex<দ্বিগুণ>*)(src2);
        auto dst_cast = (std::complex<দ্বিগুণ>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<দ্বিগুণ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<দ্বিগুণ>*)&cBeta, dst_cast, ldc);
    }
#অন্যথা
    যদি(typeid(fptype) == typeid(দশমিক))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (দশমিক*)reinterpret_cast<fptype(&)[2]>(cAlpha), (দশমিক*)src1, ldsrc1, (দশমিক*)src2, ldsrc2, (দশমিক*)reinterpret_cast<fptype(&)[2]>(cBeta), (দশমিক*)dst, lddst);
    অন্যথা যদি(typeid(fptype) == typeid(দ্বিগুণ))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (দ্বিগুণ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (দ্বিগুণ*)src1, ldsrc1, (দ্বিগুণ*)src2, ldsrc2, (দ্বিগুণ*)reinterpret_cast<fptype(&)[2]>(cBeta), (দ্বিগুণ*)dst, lddst);
#endif

    ফেরত CV_HAL_ERROR_OK;
}
পূর্ণসংখ্যা lapack_LU32f(দশমিক* a, size_t a_step, পূর্ণসংখ্যা m, দশমিক* b, size_t b_step, পূর্ণসংখ্যা n, পূর্ণসংখ্যা* info)
{
    যদি(m < HAL_LU_SMALL_MATRIX_THRESH)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_LU(a, a_step, m, b, b_step, n, info);
}

পূর্ণসংখ্যা lapack_LU64f(দ্বিগুণ* a, size_t a_step, পূর্ণসংখ্যা m, দ্বিগুণ* b, size_t b_step, পূর্ণসংখ্যা n, পূর্ণসংখ্যা* info)
{
    যদি(m < HAL_LU_SMALL_MATRIX_THRESH)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_LU(a, a_step, m, b, b_step, n, info);
}

পূর্ণসংখ্যা lapack_Cholesky32f(দশমিক* a, size_t a_step, পূর্ণসংখ্যা m, দশমিক* b, size_t b_step, পূর্ণসংখ্যা n, যৌক্তিক *info)
{
    যদি(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

পূর্ণসংখ্যা lapack_Cholesky64f(দ্বিগুণ* a, size_t a_step, পূর্ণসংখ্যা m, দ্বিগুণ* b, size_t b_step, পূর্ণসংখ্যা n, যৌক্তিক *info)
{
    যদি(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

পূর্ণসংখ্যা lapack_SVD32f(দশমিক* a, size_t a_step, দশমিক *w, দশমিক* u, size_t u_step, দশমিক* vt, size_t v_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা flags)
{
    যদি(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    পূর্ণসংখ্যা info = 0;
    ফেরত lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

পূর্ণসংখ্যা lapack_SVD64f(দ্বিগুণ* a, size_t a_step, দ্বিগুণ *w, দ্বিগুণ* u, size_t u_step, দ্বিগুণ* vt, size_t v_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা flags)
{
    যদি(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    পূর্ণসংখ্যা info = 0;
    ফেরত lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

পূর্ণসংখ্যা lapack_QR32f(দশমিক* src1, size_t src1_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, দশমিক* src2, size_t src2_step, দশমিক* dst, পূর্ণসংখ্যা* info)
{
    যদি (m < HAL_QR_SMALL_MATRIX_THRESH)
      ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

পূর্ণসংখ্যা lapack_QR64f(দ্বিগুণ* src1, size_t src1_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, দ্বিগুণ* src2, size_t src2_step, দ্বিগুণ* dst, পূর্ণসংখ্যা* info)
{
    যদি (m < HAL_QR_SMALL_MATRIX_THRESH)
      ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

পূর্ণসংখ্যা lapack_gemm32f(ধ্রুবক দশমিক *src1, size_t src1_step, ধ্রুবক দশমিক *src2, size_t src2_step, দশমিক alpha,
                   ধ্রুবক দশমিক *src3, size_t src3_step, দশমিক beta, দশমিক *dst, size_t dst_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, পূর্ণসংখ্যা flags)
{
    যদি(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

পূর্ণসংখ্যা lapack_gemm64f(ধ্রুবক দ্বিগুণ *src1, size_t src1_step, ধ্রুবক দ্বিগুণ *src2, size_t src2_step, দ্বিগুণ alpha,
                   ধ্রুবক দ্বিগুণ *src3, size_t src3_step, দ্বিগুণ beta, দ্বিগুণ *dst, size_t dst_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, পূর্ণসংখ্যা flags)
{
    যদি(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

পূর্ণসংখ্যা lapack_gemm32fc(ধ্রুবক দশমিক *src1, size_t src1_step, ধ্রুবক দশমিক *src2, size_t src2_step, দশমিক alpha,
                   ধ্রুবক দশমিক *src3, size_t src3_step, দশমিক beta, দশমিক *dst, size_t dst_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, পূর্ণসংখ্যা flags)
{
    যদি(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
পূর্ণসংখ্যা lapack_gemm64fc(ধ্রুবক দ্বিগুণ *src1, size_t src1_step, ধ্রুবক দ্বিগুণ *src2, size_t src2_step, দ্বিগুণ alpha,
                   ধ্রুবক দ্বিগুণ *src3, size_t src3_step, দ্বিগুণ beta, দ্বিগুণ *dst, size_t dst_step, পূর্ণসংখ্যা m, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k, পূর্ণসংখ্যা flags)
{
    যদি(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ফেরত CV_HAL_ERROR_NOT_IMPLEMENTED;
    ফেরত lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#যদি defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
