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

#ถ้า defined(__clang__) && defined(__has_feature)
#ถ้า __has_feature(memory_sanitizer)
#include <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) do { } ขณะ(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#ถ้า defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> static anta_hstha void
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    สำหรับ(size_t i = 0; i < m - 1; i++)
        สำหรับ(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> static anta_hstha void
transpose(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    สำหรับ(size_t i = 0; i < m; i++)
        สำหรับ(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> static anta_hstha void
copy_matrix(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    สำหรับ(size_t i = 0; i < m; i++)
        สำหรับ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> static anta_hstha void
set_value(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    สำหรับ(size_t i = 0; i < m; i++)
        สำหรับ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> static anta_hstha จำนวนเต็ม
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, จำนวนเต็ม m, fptype* b, size_t b_step, จำนวนเต็ม n, จำนวนเต็ม* info)
{
#ถ้า defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / parimaa_na(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#มิฉะนั้น
    cv::AutoBuffer<จำนวนเต็ม> piv_buff(m);
    จำนวนเต็ม lda = (จำนวนเต็ม)(a_step / parimaa_na(fptype));
    จำนวนเต็ม _m = m, _n = n;
    จำนวนเต็ม* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    ถ้า(b)
    {
        ถ้า(n == 1 && b_step == parimaa_na(fptype))
        {
            ถ้า(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)b, &_m, _info);
            มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
                dgesv_(&_m, &_n, (dviguu_na*)a, &lda, piv, (dviguu_na*)b, &_m, _info);
        }
        มิฉะนั้น
        {
            จำนวนเต็ม ldb = (จำนวนเต็ม)(b_step / parimaa_na(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            ถ้า(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)&tmpB[0], &_m, _info);
            มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
                dgesv_(&_m, &_n, (dviguu_na*)a, &lda, piv, (dviguu_na*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    มิฉะนั้น
    {
        ถ้า(typeid(fptype) == typeid(float))
            sgetrf_(&_m, &_m, (float*)a, &lda, piv, _info);
        มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
            dgetrf_(&_m, &_m, (dviguu_na*)a, &lda, piv, _info);
    }

#ถ้า defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<จำนวนเต็ม>(_info[0]);
#endif
    จำนวนเต็ม retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    จำนวนเต็ม sign = 0;
    ถ้า(*info == 0)
    {
        สำหรับ(จำนวนเต็ม i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    มิฉะนั้น
        *info = 0; //in opencv LU function zero means error

    คืน retcode;
}

template <typename fptype> static anta_hstha จำนวนเต็ม
lapack_Cholesky(fptype* a, size_t a_step, จำนวนเต็ม m, fptype* b, size_t b_step, จำนวนเต็ม n, bool* info)
{
#ถ้า defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / parimaa_na(fptype));
#มิฉะนั้น
    จำนวนเต็ม _m = m, _n = n;
    จำนวนเต็ม lapackStatus = 0;
    จำนวนเต็ม lda = (จำนวนเต็ม)(a_step / parimaa_na(fptype));
#endif
    var_na L[] = {'L', '\0'};

    ถ้า(b)
    {
        ถ้า(n == 1 && b_step == parimaa_na(fptype))
        {
            ถ้า(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)b, &_m, &lapackStatus);
            มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)b, &_m, &lapackStatus);
        }
        มิฉะนั้น
        {
            จำนวนเต็ม ldb = (จำนวนเต็ม)(b_step / parimaa_na(fptype));
            fptype* tmpB = new fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            ถ้า(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)tmpB, &_m, &lapackStatus);
            มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    มิฉะนั้น
    {
        ถ้า(typeid(fptype) == typeid(float))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (float*)a, &lda, &lapackStatus);
        มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (dviguu_na*)a, &lda, &lapackStatus);
    }

    ถ้า(lapackStatus == 0) *info = true;
    มิฉะนั้น *info = false; //in opencv Cholesky function false means error

    คืน lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> static anta_hstha จำนวนเต็ม
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม flags, จำนวนเต็ม* info)
{
#ถ้า defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / parimaa_na(fptype));
    long ldv = (long)(v_step / parimaa_na(fptype));
    long ldu = (long)(u_step / parimaa_na(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#มิฉะนั้น
    จำนวนเต็ม _m = m, _n = n;
    จำนวนเต็ม* _info = info;
    จำนวนเต็ม lda = (จำนวนเต็ม)(a_step / parimaa_na(fptype));
    จำนวนเต็ม ldv = (จำนวนเต็ม)(v_step / parimaa_na(fptype));
    จำนวนเต็ม ldu = (จำนวนเต็ม)(u_step / parimaa_na(fptype));
    จำนวนเต็ม lwork = -1;
    cv::AutoBuffer<จำนวนเต็ม> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    var_na mode[] = { ' ', '\0'};
    ถ้า(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    มิฉะนั้น ถ้า((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    มิฉะนั้น ถ้า((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    มิฉะนั้น ถ้า(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    ถ้า((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    ถ้า(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&work1, &lwork, iworkBuf, _info);
    มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)w, (dviguu_na*)u, &ldu, (dviguu_na*)vt, &ldv, (dviguu_na*)&work1, &lwork, iworkBuf, _info);

    ถ้า(*info < 0)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (จำนวนเต็ม)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), parimaa_na(fptype) * (lwork + 1));

    ถ้า(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&buffer[0], &lwork, iworkBuf, _info);
    มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)w, (dviguu_na*)u, &ldu, (dviguu_na*)vt, &ldv, (dviguu_na*)&buffer[0], &lwork, iworkBuf, _info);

#ถ้า defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<จำนวนเต็ม>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    ถ้า (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    ถ้า (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    ถ้า (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, parimaa_na(fptype) * std::min(m, n));

    ถ้า(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    ถ้า((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        สำหรับ(จำนวนเต็ม i = 0; i < m; i++)
            สำหรับ(จำนวนเต็ม j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    ถ้า(*info < 0)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha จำนวนเต็ม
lapack_QR(fptype* a, size_t a_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, fptype* b, size_t b_step, fptype* dst, จำนวนเต็ม* info)
{
#ถ้า defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / parimaa_na(fptype));
    long lwork = -1;
    long ldtmpA;
#มิฉะนั้น
    จำนวนเต็ม _m = m, _n = n, _k = k;
    จำนวนเต็ม* _info = info;
    จำนวนเต็ม lda = (จำนวนเต็ม)(a_step / parimaa_na(fptype));
    จำนวนเต็ม lwork = -1;
    จำนวนเต็ม ldtmpA;
#endif

    var_na mode[] = { 'N', '\0' };
    ถ้า(m < n)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    ถ้า (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    มิฉะนั้น
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    ถ้า (b)
    {
        ถ้า (k == 1 && b_step == parimaa_na(fptype))
        {
            ถ้า (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)&work1, &lwork, _info);
            มิฉะนั้น ถ้า (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)b, &_m, (dviguu_na*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(จำนวนเต็ม));
            ถ้า (*info < 0)
                คืน CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ถ้า (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)buffer, &lwork, _info);
            มิฉะนั้น ถ้า (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)b, &_m, (dviguu_na*)buffer, &lwork, _info);
        }
        มิฉะนั้น
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            จำนวนเต็ม ldb = (จำนวนเต็ม)(b_step / parimaa_na(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            ถ้า (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)&work1, &lwork, _info);
            มิฉะนั้น ถ้า (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)tmpB, &_m, (dviguu_na*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(จำนวนเต็ม));
            ถ้า (*info < 0)
                คืน CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ถ้า (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)buffer, &lwork, _info);
            มิฉะนั้น ถ้า (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)tmpB, &_m, (dviguu_na*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    มิฉะนั้น
    {
        ถ้า (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)&work1, &lwork, _info);
        มิฉะนั้น ถ้า (typeid(fptype) == typeid(dviguu_na))
            dgeqrf_(&_m, &_n, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)dst, (dviguu_na*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(จำนวนเต็ม));
        ถ้า (*info < 0)
            คืน CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        ถ้า (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)buffer, &lwork, _info);
        มิฉะนั้น ถ้า (typeid(fptype) == typeid(dviguu_na))
            dgeqrf_(&_m, &_n, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)dst, (dviguu_na*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(จำนวนเต็ม));
    ถ้า (m == n)
        transpose_square_inplace(a, lda, m);
    มิฉะนั้น
        transpose(tmpA, m, a, lda, n, m);

#ถ้า defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<จำนวนเต็ม>(_info[0]);
#endif

    ถ้า (*info != 0)
        *info = 0;
    มิฉะนั้น
        *info = 1;

    คืน CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha จำนวนเต็ม
lapack_gemm(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, จำนวนเต็ม a_m, จำนวนเต็ม a_n, จำนวนเต็ม d_n, จำนวนเต็ม flags)
{
    จำนวนเต็ม ldsrc1 = (จำนวนเต็ม)(src1_step / parimaa_na(fptype));
    จำนวนเต็ม ldsrc2 = (จำนวนเต็ม)(src2_step / parimaa_na(fptype));
    จำนวนเต็ม ldsrc3 = (จำนวนเต็ม)(src3_step / parimaa_na(fptype));
    จำนวนเต็ม lddst = (จำนวนเต็ม)(dst_step / parimaa_na(fptype));
    จำนวนเต็ม c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    ถ้า(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ถ้า(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        มิฉะนั้น
        {
            d_m = a_m;
        }
    }
    มิฉะนั้น
    {
        transB = CblasNoTrans;
        ถ้า(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        มิฉะนั้น
        {
            d_m = a_m;
        }
    }

    ถ้า(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    มิฉะนั้น
    {
        c_m = d_m;
        c_n = d_n;
    }

    ถ้า(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    มิฉะนั้น
    {
        transA = CblasNoTrans;
    }

    ถ้า(src3 != dst && beta != 0.0 && src3_step != 0) {
        ถ้า(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        มิฉะนั้น
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    มิฉะนั้น ถ้า (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    มิฉะนั้น ถ้า(src3_step == 0 && beta != 0.0)
        set_value(dst, lddst, (fptype)0.0, d_m, d_n);

    ถ้า(typeid(fptype) == typeid(float))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float)alpha, (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float)beta, (float*)dst, lddst);
    มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (dviguu_na)alpha, (dviguu_na*)src1, ldsrc1, (dviguu_na*)src2, ldsrc2, (dviguu_na)beta, (dviguu_na*)dst, lddst);

    คืน CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha จำนวนเต็ม
lapack_gemm_c(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, จำนวนเต็ม a_m, จำนวนเต็ม a_n, จำนวนเต็ม d_n, จำนวนเต็ม flags)
{
    จำนวนเต็ม ldsrc1 = (จำนวนเต็ม)(src1_step / parimaa_na(std::complex<fptype>));
    จำนวนเต็ม ldsrc2 = (จำนวนเต็ม)(src2_step / parimaa_na(std::complex<fptype>));
    จำนวนเต็ม ldsrc3 = (จำนวนเต็ม)(src3_step / parimaa_na(std::complex<fptype>));
    จำนวนเต็ม lddst = (จำนวนเต็ม)(dst_step / parimaa_na(std::complex<fptype>));
    จำนวนเต็ม c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    ถ้า(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ถ้า(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        มิฉะนั้น
        {
            d_m = a_m;
        }
    }
    มิฉะนั้น
    {
        transB = CblasNoTrans;
        ถ้า(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        มิฉะนั้น
        {
            d_m = a_m;
        }
    }

    ถ้า(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    มิฉะนั้น
    {
        c_m = d_m;
        c_n = d_n;
    }

    ถ้า(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    มิฉะนั้น
    {
        transA = CblasNoTrans;
    }

    ถ้า(src3 != dst && beta != 0.0 && src3_step != 0) {
        ถ้า(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        มิฉะนั้น
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    มิฉะนั้น ถ้า (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    มิฉะนั้น ถ้า(src3_step == 0 && beta != 0.0)
        set_value((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#ถ้า defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    จำนวนเต็ม M = a_m, N = d_n, K = a_n;
    ถ้า(typeid(fptype) == typeid(float)) {
        auto src1_cast = (std::complex<float>*)(src1);
        auto src2_cast = (std::complex<float>*)(src2);
        auto dst_cast = (std::complex<float>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<float>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<float>*)&cBeta, dst_cast, ldc);
    }
    มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na)) {
        auto src1_cast = (std::complex<dviguu_na>*)(src1);
        auto src2_cast = (std::complex<dviguu_na>*)(src2);
        auto dst_cast = (std::complex<dviguu_na>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<dviguu_na>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<dviguu_na>*)&cBeta, dst_cast, ldc);
    }
#มิฉะนั้น
    ถ้า(typeid(fptype) == typeid(float))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float*)reinterpret_cast<fptype(&)[2]>(cAlpha), (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float*)reinterpret_cast<fptype(&)[2]>(cBeta), (float*)dst, lddst);
    มิฉะนั้น ถ้า(typeid(fptype) == typeid(dviguu_na))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (dviguu_na*)reinterpret_cast<fptype(&)[2]>(cAlpha), (dviguu_na*)src1, ldsrc1, (dviguu_na*)src2, ldsrc2, (dviguu_na*)reinterpret_cast<fptype(&)[2]>(cBeta), (dviguu_na*)dst, lddst);
#endif

    คืน CV_HAL_ERROR_OK;
}
จำนวนเต็ม lapack_LU32f(float* a, size_t a_step, จำนวนเต็ม m, float* b, size_t b_step, จำนวนเต็ม n, จำนวนเต็ม* info)
{
    ถ้า(m < HAL_LU_SMALL_MATRIX_THRESH)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_LU(a, a_step, m, b, b_step, n, info);
}

จำนวนเต็ม lapack_LU64f(dviguu_na* a, size_t a_step, จำนวนเต็ม m, dviguu_na* b, size_t b_step, จำนวนเต็ม n, จำนวนเต็ม* info)
{
    ถ้า(m < HAL_LU_SMALL_MATRIX_THRESH)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_LU(a, a_step, m, b, b_step, n, info);
}

จำนวนเต็ม lapack_Cholesky32f(float* a, size_t a_step, จำนวนเต็ม m, float* b, size_t b_step, จำนวนเต็ม n, bool *info)
{
    ถ้า(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

จำนวนเต็ม lapack_Cholesky64f(dviguu_na* a, size_t a_step, จำนวนเต็ม m, dviguu_na* b, size_t b_step, จำนวนเต็ม n, bool *info)
{
    ถ้า(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

จำนวนเต็ม lapack_SVD32f(float* a, size_t a_step, float *w, float* u, size_t u_step, float* vt, size_t v_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม flags)
{
    ถ้า(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    จำนวนเต็ม info = 0;
    คืน lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

จำนวนเต็ม lapack_SVD64f(dviguu_na* a, size_t a_step, dviguu_na *w, dviguu_na* u, size_t u_step, dviguu_na* vt, size_t v_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม flags)
{
    ถ้า(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    จำนวนเต็ม info = 0;
    คืน lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

จำนวนเต็ม lapack_QR32f(float* src1, size_t src1_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, float* src2, size_t src2_step, float* dst, จำนวนเต็ม* info)
{
    ถ้า (m < HAL_QR_SMALL_MATRIX_THRESH)
      คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

จำนวนเต็ม lapack_QR64f(dviguu_na* src1, size_t src1_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, dviguu_na* src2, size_t src2_step, dviguu_na* dst, จำนวนเต็ม* info)
{
    ถ้า (m < HAL_QR_SMALL_MATRIX_THRESH)
      คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

จำนวนเต็ม lapack_gemm32f(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, จำนวนเต็ม flags)
{
    ถ้า(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

จำนวนเต็ม lapack_gemm64f(const dviguu_na *src1, size_t src1_step, const dviguu_na *src2, size_t src2_step, dviguu_na alpha,
                   const dviguu_na *src3, size_t src3_step, dviguu_na beta, dviguu_na *dst, size_t dst_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, จำนวนเต็ม flags)
{
    ถ้า(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

จำนวนเต็ม lapack_gemm32fc(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, จำนวนเต็ม flags)
{
    ถ้า(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
จำนวนเต็ม lapack_gemm64fc(const dviguu_na *src1, size_t src1_step, const dviguu_na *src2, size_t src2_step, dviguu_na alpha,
                   const dviguu_na *src3, size_t src3_step, dviguu_na beta, dviguu_na *dst, size_t dst_step, จำนวนเต็ม m, จำนวนเต็ม n, จำนวนเต็ม k, จำนวนเต็ม flags)
{
    ถ้า(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        คืน CV_HAL_ERROR_NOT_IMPLEMENTED;
    คืน lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#ถ้า defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
