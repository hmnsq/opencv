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

#include "precomp.hpp"
#include "hal_internal.hpp"

#ifdef HAVE_LAPACK

#include "opencv_lapack.h"

#include <cmath>
#include <algorithm>
#include <typeinfo>
#include <limits>
#include <complex>
#include <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#ꦪꦼꦤ꧀ defined(__clang__) && defined(__has_feature)
#ꦪꦼꦤ꧀ __has_feature(memory_sanitizer)
#include <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) do { } ꦱꦼꦢꦼꦭ(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#ꦪꦼꦤ꧀ defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> static anta_hstha void
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    ꦏꦁꦒꦺꦴ(size_t i = 0; i < m - 1; i++)
        ꦏꦁꦒꦺꦴ(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> static anta_hstha void
transpose(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ꦏꦁꦒꦺꦴ(size_t i = 0; i < m; i++)
        ꦏꦁꦒꦺꦴ(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> static anta_hstha void
copy_matrix(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ꦏꦁꦒꦺꦴ(size_t i = 0; i < m; i++)
        ꦏꦁꦒꦺꦴ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> static anta_hstha void
set_value(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    ꦏꦁꦒꦺꦴ(size_t i = 0; i < m; i++)
        ꦏꦁꦒꦺꦴ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> static anta_hstha puur_naa_nka
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, puur_naa_nka m, fptype* b, size_t b_step, puur_naa_nka n, puur_naa_nka* info)
{
#ꦪꦼꦤ꧀ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / parimaa_na(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#ꦭꦶꦪ
    cv::AutoBuffer<puur_naa_nka> piv_buff(m);
    puur_naa_nka lda = (puur_naa_nka)(a_step / parimaa_na(fptype));
    puur_naa_nka _m = m, _n = n;
    puur_naa_nka* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    ꦪꦼꦤ꧀(b)
    {
        ꦪꦼꦤ꧀(n == 1 && b_step == parimaa_na(fptype))
        {
            ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)b, &_m, _info);
            ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
                dgesv_(&_m, &_n, (dviguu_na*)a, &lda, piv, (dviguu_na*)b, &_m, _info);
        }
        ꦭꦶꦪ
        {
            puur_naa_nka ldb = (puur_naa_nka)(b_step / parimaa_na(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)&tmpB[0], &_m, _info);
            ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
                dgesv_(&_m, &_n, (dviguu_na*)a, &lda, piv, (dviguu_na*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    ꦭꦶꦪ
    {
        ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
            sgetrf_(&_m, &_m, (float*)a, &lda, piv, _info);
        ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
            dgetrf_(&_m, &_m, (dviguu_na*)a, &lda, piv, _info);
    }

#ꦪꦼꦤ꧀ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<puur_naa_nka>(_info[0]);
#endif
    puur_naa_nka retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    puur_naa_nka sign = 0;
    ꦪꦼꦤ꧀(*info == 0)
    {
        ꦏꦁꦒꦺꦴ(puur_naa_nka i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    ꦭꦶꦪ
        *info = 0; //in opencv LU function zero means error

    ꦮꦭꦶ retcode;
}

template <typename fptype> static anta_hstha puur_naa_nka
lapack_Cholesky(fptype* a, size_t a_step, puur_naa_nka m, fptype* b, size_t b_step, puur_naa_nka n, bool* info)
{
#ꦪꦼꦤ꧀ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / parimaa_na(fptype));
#ꦭꦶꦪ
    puur_naa_nka _m = m, _n = n;
    puur_naa_nka lapackStatus = 0;
    puur_naa_nka lda = (puur_naa_nka)(a_step / parimaa_na(fptype));
#endif
    var_na L[] = {'L', '\0'};

    ꦪꦼꦤ꧀(b)
    {
        ꦪꦼꦤ꧀(n == 1 && b_step == parimaa_na(fptype))
        {
            ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)b, &_m, &lapackStatus);
            ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)b, &_m, &lapackStatus);
        }
        ꦭꦶꦪ
        {
            puur_naa_nka ldb = (puur_naa_nka)(b_step / parimaa_na(fptype));
            fptype* tmpB = new fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)tmpB, &_m, &lapackStatus);
            ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    ꦭꦶꦪ
    {
        ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (float*)a, &lda, &lapackStatus);
        ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (dviguu_na*)a, &lda, &lapackStatus);
    }

    ꦪꦼꦤ꧀(lapackStatus == 0) *info = true;
    ꦭꦶꦪ *info = false; //in opencv Cholesky function false means error

    ꦮꦭꦶ lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> static anta_hstha puur_naa_nka
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka flags, puur_naa_nka* info)
{
#ꦪꦼꦤ꧀ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / parimaa_na(fptype));
    long ldv = (long)(v_step / parimaa_na(fptype));
    long ldu = (long)(u_step / parimaa_na(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#ꦭꦶꦪ
    puur_naa_nka _m = m, _n = n;
    puur_naa_nka* _info = info;
    puur_naa_nka lda = (puur_naa_nka)(a_step / parimaa_na(fptype));
    puur_naa_nka ldv = (puur_naa_nka)(v_step / parimaa_na(fptype));
    puur_naa_nka ldu = (puur_naa_nka)(u_step / parimaa_na(fptype));
    puur_naa_nka lwork = -1;
    cv::AutoBuffer<puur_naa_nka> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    var_na mode[] = { ' ', '\0'};
    ꦪꦼꦤ꧀(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    ꦭꦶꦪ ꦪꦼꦤ꧀((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    ꦭꦶꦪ ꦪꦼꦤ꧀(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    ꦪꦼꦤ꧀((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&work1, &lwork, iworkBuf, _info);
    ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)w, (dviguu_na*)u, &ldu, (dviguu_na*)vt, &ldv, (dviguu_na*)&work1, &lwork, iworkBuf, _info);

    ꦪꦼꦤ꧀(*info < 0)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (puur_naa_nka)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), parimaa_na(fptype) * (lwork + 1));

    ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&buffer[0], &lwork, iworkBuf, _info);
    ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)w, (dviguu_na*)u, &ldu, (dviguu_na*)vt, &ldv, (dviguu_na*)&buffer[0], &lwork, iworkBuf, _info);

#ꦪꦼꦤ꧀ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<puur_naa_nka>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    ꦪꦼꦤ꧀ (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    ꦪꦼꦤ꧀ (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    ꦪꦼꦤ꧀ (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, parimaa_na(fptype) * std::min(m, n));

    ꦪꦼꦤ꧀(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    ꦪꦼꦤ꧀((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        ꦏꦁꦒꦺꦴ(puur_naa_nka i = 0; i < m; i++)
            ꦏꦁꦒꦺꦴ(puur_naa_nka j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    ꦪꦼꦤ꧀(*info < 0)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha puur_naa_nka
lapack_QR(fptype* a, size_t a_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, fptype* b, size_t b_step, fptype* dst, puur_naa_nka* info)
{
#ꦪꦼꦤ꧀ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / parimaa_na(fptype));
    long lwork = -1;
    long ldtmpA;
#ꦭꦶꦪ
    puur_naa_nka _m = m, _n = n, _k = k;
    puur_naa_nka* _info = info;
    puur_naa_nka lda = (puur_naa_nka)(a_step / parimaa_na(fptype));
    puur_naa_nka lwork = -1;
    puur_naa_nka ldtmpA;
#endif

    var_na mode[] = { 'N', '\0' };
    ꦪꦼꦤ꧀(m < n)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    ꦪꦼꦤ꧀ (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    ꦭꦶꦪ
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    ꦪꦼꦤ꧀ (b)
    {
        ꦪꦼꦤ꧀ (k == 1 && b_step == parimaa_na(fptype))
        {
            ꦪꦼꦤ꧀ (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)&work1, &lwork, _info);
            ꦭꦶꦪ ꦪꦼꦤ꧀ (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)b, &_m, (dviguu_na*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(puur_naa_nka));
            ꦪꦼꦤ꧀ (*info < 0)
                ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ꦪꦼꦤ꧀ (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)buffer, &lwork, _info);
            ꦭꦶꦪ ꦪꦼꦤ꧀ (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)b, &_m, (dviguu_na*)buffer, &lwork, _info);
        }
        ꦭꦶꦪ
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            puur_naa_nka ldb = (puur_naa_nka)(b_step / parimaa_na(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            ꦪꦼꦤ꧀ (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)&work1, &lwork, _info);
            ꦭꦶꦪ ꦪꦼꦤ꧀ (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)tmpB, &_m, (dviguu_na*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(puur_naa_nka));
            ꦪꦼꦤ꧀ (*info < 0)
                ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ꦪꦼꦤ꧀ (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)buffer, &lwork, _info);
            ꦭꦶꦪ ꦪꦼꦤ꧀ (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)tmpB, &_m, (dviguu_na*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    ꦭꦶꦪ
    {
        ꦪꦼꦤ꧀ (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)&work1, &lwork, _info);
        ꦭꦶꦪ ꦪꦼꦤ꧀ (typeid(fptype) == typeid(dviguu_na))
            dgeqrf_(&_m, &_n, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)dst, (dviguu_na*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(puur_naa_nka));
        ꦪꦼꦤ꧀ (*info < 0)
            ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        ꦪꦼꦤ꧀ (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)buffer, &lwork, _info);
        ꦭꦶꦪ ꦪꦼꦤ꧀ (typeid(fptype) == typeid(dviguu_na))
            dgeqrf_(&_m, &_n, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)dst, (dviguu_na*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(puur_naa_nka));
    ꦪꦼꦤ꧀ (m == n)
        transpose_square_inplace(a, lda, m);
    ꦭꦶꦪ
        transpose(tmpA, m, a, lda, n, m);

#ꦪꦼꦤ꧀ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<puur_naa_nka>(_info[0]);
#endif

    ꦪꦼꦤ꧀ (*info != 0)
        *info = 0;
    ꦭꦶꦪ
        *info = 1;

    ꦮꦭꦶ CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha puur_naa_nka
lapack_gemm(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, puur_naa_nka a_m, puur_naa_nka a_n, puur_naa_nka d_n, puur_naa_nka flags)
{
    puur_naa_nka ldsrc1 = (puur_naa_nka)(src1_step / parimaa_na(fptype));
    puur_naa_nka ldsrc2 = (puur_naa_nka)(src2_step / parimaa_na(fptype));
    puur_naa_nka ldsrc3 = (puur_naa_nka)(src3_step / parimaa_na(fptype));
    puur_naa_nka lddst = (puur_naa_nka)(dst_step / parimaa_na(fptype));
    puur_naa_nka c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ꦭꦶꦪ
        {
            d_m = a_m;
        }
    }
    ꦭꦶꦪ
    {
        transB = CblasNoTrans;
        ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ꦭꦶꦪ
        {
            d_m = a_m;
        }
    }

    ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ꦭꦶꦪ
    {
        c_m = d_m;
        c_n = d_n;
    }

    ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ꦭꦶꦪ
    {
        transA = CblasNoTrans;
    }

    ꦪꦼꦤ꧀(src3 != dst && beta != 0.0 && src3_step != 0) {
        ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        ꦭꦶꦪ
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦭꦶꦪ ꦪꦼꦤ꧀(src3_step == 0 && beta != 0.0)
        set_value(dst, lddst, (fptype)0.0, d_m, d_n);

    ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float)alpha, (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float)beta, (float*)dst, lddst);
    ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (dviguu_na)alpha, (dviguu_na*)src1, ldsrc1, (dviguu_na*)src2, ldsrc2, (dviguu_na)beta, (dviguu_na*)dst, lddst);

    ꦮꦭꦶ CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha puur_naa_nka
lapack_gemm_c(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, puur_naa_nka a_m, puur_naa_nka a_n, puur_naa_nka d_n, puur_naa_nka flags)
{
    puur_naa_nka ldsrc1 = (puur_naa_nka)(src1_step / parimaa_na(std::complex<fptype>));
    puur_naa_nka ldsrc2 = (puur_naa_nka)(src2_step / parimaa_na(std::complex<fptype>));
    puur_naa_nka ldsrc3 = (puur_naa_nka)(src3_step / parimaa_na(std::complex<fptype>));
    puur_naa_nka lddst = (puur_naa_nka)(dst_step / parimaa_na(std::complex<fptype>));
    puur_naa_nka c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ꦭꦶꦪ
        {
            d_m = a_m;
        }
    }
    ꦭꦶꦪ
    {
        transB = CblasNoTrans;
        ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ꦭꦶꦪ
        {
            d_m = a_m;
        }
    }

    ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ꦭꦶꦪ
    {
        c_m = d_m;
        c_n = d_n;
    }

    ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ꦭꦶꦪ
    {
        transA = CblasNoTrans;
    }

    ꦪꦼꦤ꧀(src3 != dst && beta != 0.0 && src3_step != 0) {
        ꦪꦼꦤ꧀(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        ꦭꦶꦪ
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦭꦶꦪ ꦪꦼꦤ꧀(src3_step == 0 && beta != 0.0)
        set_value((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#ꦪꦼꦤ꧀ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    puur_naa_nka M = a_m, N = d_n, K = a_n;
    ꦪꦼꦤ꧀(typeid(fptype) == typeid(float)) {
        auto src1_cast = (std::complex<float>*)(src1);
        auto src2_cast = (std::complex<float>*)(src2);
        auto dst_cast = (std::complex<float>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<float>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<float>*)&cBeta, dst_cast, ldc);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na)) {
        auto src1_cast = (std::complex<dviguu_na>*)(src1);
        auto src2_cast = (std::complex<dviguu_na>*)(src2);
        auto dst_cast = (std::complex<dviguu_na>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<dviguu_na>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<dviguu_na>*)&cBeta, dst_cast, ldc);
    }
#ꦭꦶꦪ
    ꦪꦼꦤ꧀(typeid(fptype) == typeid(float))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float*)reinterpret_cast<fptype(&)[2]>(cAlpha), (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float*)reinterpret_cast<fptype(&)[2]>(cBeta), (float*)dst, lddst);
    ꦭꦶꦪ ꦪꦼꦤ꧀(typeid(fptype) == typeid(dviguu_na))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (dviguu_na*)reinterpret_cast<fptype(&)[2]>(cAlpha), (dviguu_na*)src1, ldsrc1, (dviguu_na*)src2, ldsrc2, (dviguu_na*)reinterpret_cast<fptype(&)[2]>(cBeta), (dviguu_na*)dst, lddst);
#endif

    ꦮꦭꦶ CV_HAL_ERROR_OK;
}
puur_naa_nka lapack_LU32f(float* a, size_t a_step, puur_naa_nka m, float* b, size_t b_step, puur_naa_nka n, puur_naa_nka* info)
{
    ꦪꦼꦤ꧀(m < HAL_LU_SMALL_MATRIX_THRESH)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_LU(a, a_step, m, b, b_step, n, info);
}

puur_naa_nka lapack_LU64f(dviguu_na* a, size_t a_step, puur_naa_nka m, dviguu_na* b, size_t b_step, puur_naa_nka n, puur_naa_nka* info)
{
    ꦪꦼꦤ꧀(m < HAL_LU_SMALL_MATRIX_THRESH)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_LU(a, a_step, m, b, b_step, n, info);
}

puur_naa_nka lapack_Cholesky32f(float* a, size_t a_step, puur_naa_nka m, float* b, size_t b_step, puur_naa_nka n, bool *info)
{
    ꦪꦼꦤ꧀(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

puur_naa_nka lapack_Cholesky64f(dviguu_na* a, size_t a_step, puur_naa_nka m, dviguu_na* b, size_t b_step, puur_naa_nka n, bool *info)
{
    ꦪꦼꦤ꧀(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

puur_naa_nka lapack_SVD32f(float* a, size_t a_step, float *w, float* u, size_t u_step, float* vt, size_t v_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka flags)
{
    ꦪꦼꦤ꧀(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    puur_naa_nka info = 0;
    ꦮꦭꦶ lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

puur_naa_nka lapack_SVD64f(dviguu_na* a, size_t a_step, dviguu_na *w, dviguu_na* u, size_t u_step, dviguu_na* vt, size_t v_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka flags)
{
    ꦪꦼꦤ꧀(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    puur_naa_nka info = 0;
    ꦮꦭꦶ lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

puur_naa_nka lapack_QR32f(float* src1, size_t src1_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, float* src2, size_t src2_step, float* dst, puur_naa_nka* info)
{
    ꦪꦼꦤ꧀ (m < HAL_QR_SMALL_MATRIX_THRESH)
      ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

puur_naa_nka lapack_QR64f(dviguu_na* src1, size_t src1_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, dviguu_na* src2, size_t src2_step, dviguu_na* dst, puur_naa_nka* info)
{
    ꦪꦼꦤ꧀ (m < HAL_QR_SMALL_MATRIX_THRESH)
      ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

puur_naa_nka lapack_gemm32f(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, puur_naa_nka flags)
{
    ꦪꦼꦤ꧀(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

puur_naa_nka lapack_gemm64f(const dviguu_na *src1, size_t src1_step, const dviguu_na *src2, size_t src2_step, dviguu_na alpha,
                   const dviguu_na *src3, size_t src3_step, dviguu_na beta, dviguu_na *dst, size_t dst_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, puur_naa_nka flags)
{
    ꦪꦼꦤ꧀(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

puur_naa_nka lapack_gemm32fc(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, puur_naa_nka flags)
{
    ꦪꦼꦤ꧀(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
puur_naa_nka lapack_gemm64fc(const dviguu_na *src1, size_t src1_step, const dviguu_na *src2, size_t src2_step, dviguu_na alpha,
                   const dviguu_na *src3, size_t src3_step, dviguu_na beta, dviguu_na *dst, size_t dst_step, puur_naa_nka m, puur_naa_nka n, puur_naa_nka k, puur_naa_nka flags)
{
    ꦪꦼꦤ꧀(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ꦮꦭꦶ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ꦮꦭꦶ lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#ꦪꦼꦤ꧀ defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
