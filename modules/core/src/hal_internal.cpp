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

#якщо defined(__clang__) && defined(__has_feature)
#якщо __has_feature(memory_sanitizer)
#include <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) do { } поки(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#якщо defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> static inline void
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    для(size_t i = 0; i < m - 1; i++)
        для(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> static inline void
transpose(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    для(size_t i = 0; i < m; i++)
        для(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> static inline void
copy_matrix(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    для(size_t i = 0; i < m; i++)
        для(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> static inline void
set_value(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    для(size_t i = 0; i < m; i++)
        для(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> static inline ціле
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, ціле m, fptype* b, size_t b_step, ціле n, ціле* info)
{
#якщо defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / sizeof(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#інакше
    cv::AutoBuffer<ціле> piv_buff(m);
    ціле lda = (ціле)(a_step / sizeof(fptype));
    ціле _m = m, _n = n;
    ціле* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    якщо(b)
    {
        якщо(n == 1 && b_step == sizeof(fptype))
        {
            якщо(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)b, &_m, _info);
            інакше якщо(typeid(fptype) == typeid(double))
                dgesv_(&_m, &_n, (double*)a, &lda, piv, (double*)b, &_m, _info);
        }
        інакше
        {
            ціле ldb = (ціле)(b_step / sizeof(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            якщо(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)&tmpB[0], &_m, _info);
            інакше якщо(typeid(fptype) == typeid(double))
                dgesv_(&_m, &_n, (double*)a, &lda, piv, (double*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    інакше
    {
        якщо(typeid(fptype) == typeid(float))
            sgetrf_(&_m, &_m, (float*)a, &lda, piv, _info);
        інакше якщо(typeid(fptype) == typeid(double))
            dgetrf_(&_m, &_m, (double*)a, &lda, piv, _info);
    }

#якщо defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ціле>(_info[0]);
#endif
    ціле retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    ціле sign = 0;
    якщо(*info == 0)
    {
        для(ціле i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    інакше
        *info = 0; //in opencv LU function zero means error

    повернути retcode;
}

template <typename fptype> static inline ціле
lapack_Cholesky(fptype* a, size_t a_step, ціле m, fptype* b, size_t b_step, ціле n, bool* info)
{
#якщо defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / sizeof(fptype));
#інакше
    ціле _m = m, _n = n;
    ціле lapackStatus = 0;
    ціле lda = (ціле)(a_step / sizeof(fptype));
#endif
    char L[] = {'L', '\0'};

    якщо(b)
    {
        якщо(n == 1 && b_step == sizeof(fptype))
        {
            якщо(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)b, &_m, &lapackStatus);
            інакше якщо(typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (double*)a, &lda, (double*)b, &_m, &lapackStatus);
        }
        інакше
        {
            ціле ldb = (ціле)(b_step / sizeof(fptype));
            fptype* tmpB = new fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            якщо(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)tmpB, &_m, &lapackStatus);
            інакше якщо(typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (double*)a, &lda, (double*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    інакше
    {
        якщо(typeid(fptype) == typeid(float))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (float*)a, &lda, &lapackStatus);
        інакше якщо(typeid(fptype) == typeid(double))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (double*)a, &lda, &lapackStatus);
    }

    якщо(lapackStatus == 0) *info = true;
    інакше *info = false; //in opencv Cholesky function false means error

    повернути lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> static inline ціле
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, ціле m, ціле n, ціле flags, ціле* info)
{
#якщо defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / sizeof(fptype));
    long ldv = (long)(v_step / sizeof(fptype));
    long ldu = (long)(u_step / sizeof(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#інакше
    ціле _m = m, _n = n;
    ціле* _info = info;
    ціле lda = (ціле)(a_step / sizeof(fptype));
    ціле ldv = (ціле)(v_step / sizeof(fptype));
    ціле ldu = (ціле)(u_step / sizeof(fptype));
    ціле lwork = -1;
    cv::AutoBuffer<ціле> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    char mode[] = { ' ', '\0'};
    якщо(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    інакше якщо((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    інакше якщо((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    інакше якщо(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    якщо((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    якщо(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&work1, &lwork, iworkBuf, _info);
    інакше якщо(typeid(fptype) == typeid(double))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (double*)a, &lda, (double*)w, (double*)u, &ldu, (double*)vt, &ldv, (double*)&work1, &lwork, iworkBuf, _info);

    якщо(*info < 0)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (ціле)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), sizeof(fptype) * (lwork + 1));

    якщо(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&buffer[0], &lwork, iworkBuf, _info);
    інакше якщо(typeid(fptype) == typeid(double))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (double*)a, &lda, (double*)w, (double*)u, &ldu, (double*)vt, &ldv, (double*)&buffer[0], &lwork, iworkBuf, _info);

#якщо defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ціле>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    якщо (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    якщо (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    якщо (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, sizeof(fptype) * std::min(m, n));

    якщо(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    якщо((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        для(ціле i = 0; i < m; i++)
            для(ціле j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    якщо(*info < 0)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути CV_HAL_ERROR_OK;
}

template <typename fptype> static inline ціле
lapack_QR(fptype* a, size_t a_step, ціле m, ціле n, ціле k, fptype* b, size_t b_step, fptype* dst, ціле* info)
{
#якщо defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / sizeof(fptype));
    long lwork = -1;
    long ldtmpA;
#інакше
    ціле _m = m, _n = n, _k = k;
    ціле* _info = info;
    ціле lda = (ціле)(a_step / sizeof(fptype));
    ціле lwork = -1;
    ціле ldtmpA;
#endif

    char mode[] = { 'N', '\0' };
    якщо(m < n)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    якщо (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    інакше
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    якщо (b)
    {
        якщо (k == 1 && b_step == sizeof(fptype))
        {
            якщо (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)&work1, &lwork, _info);
            інакше якщо (typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (double*)tmpA, &ldtmpA, (double*)b, &_m, (double*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, sizeof(ціле));
            якщо (*info < 0)
                повернути CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            якщо (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)buffer, &lwork, _info);
            інакше якщо (typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (double*)tmpA, &ldtmpA, (double*)b, &_m, (double*)buffer, &lwork, _info);
        }
        інакше
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            ціле ldb = (ціле)(b_step / sizeof(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            якщо (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)&work1, &lwork, _info);
            інакше якщо (typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (double*)tmpA, &ldtmpA, (double*)tmpB, &_m, (double*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, sizeof(ціле));
            якщо (*info < 0)
                повернути CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            якщо (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)buffer, &lwork, _info);
            інакше якщо (typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (double*)tmpA, &ldtmpA, (double*)tmpB, &_m, (double*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    інакше
    {
        якщо (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)&work1, &lwork, _info);
        інакше якщо (typeid(fptype) == typeid(double))
            dgeqrf_(&_m, &_n, (double*)tmpA, &ldtmpA, (double*)dst, (double*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, sizeof(ціле));
        якщо (*info < 0)
            повернути CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        якщо (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)buffer, &lwork, _info);
        інакше якщо (typeid(fptype) == typeid(double))
            dgeqrf_(&_m, &_n, (double*)tmpA, &ldtmpA, (double*)dst, (double*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, sizeof(ціле));
    якщо (m == n)
        transpose_square_inplace(a, lda, m);
    інакше
        transpose(tmpA, m, a, lda, n, m);

#якщо defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ціле>(_info[0]);
#endif

    якщо (*info != 0)
        *info = 0;
    інакше
        *info = 1;

    повернути CV_HAL_ERROR_OK;
}

template <typename fptype> static inline ціле
lapack_gemm(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ціле a_m, ціле a_n, ціле d_n, ціле flags)
{
    ціле ldsrc1 = (ціле)(src1_step / sizeof(fptype));
    ціле ldsrc2 = (ціле)(src2_step / sizeof(fptype));
    ціле ldsrc3 = (ціле)(src3_step / sizeof(fptype));
    ціле lddst = (ціле)(dst_step / sizeof(fptype));
    ціле c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    якщо(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        якщо(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        інакше
        {
            d_m = a_m;
        }
    }
    інакше
    {
        transB = CblasNoTrans;
        якщо(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        інакше
        {
            d_m = a_m;
        }
    }

    якщо(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    інакше
    {
        c_m = d_m;
        c_n = d_n;
    }

    якщо(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    інакше
    {
        transA = CblasNoTrans;
    }

    якщо(src3 != dst && beta != 0.0 && src3_step != 0) {
        якщо(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        інакше
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    інакше якщо (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    інакше якщо(src3_step == 0 && beta != 0.0)
        set_value(dst, lddst, (fptype)0.0, d_m, d_n);

    якщо(typeid(fptype) == typeid(float))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float)alpha, (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float)beta, (float*)dst, lddst);
    інакше якщо(typeid(fptype) == typeid(double))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (double)alpha, (double*)src1, ldsrc1, (double*)src2, ldsrc2, (double)beta, (double*)dst, lddst);

    повернути CV_HAL_ERROR_OK;
}

template <typename fptype> static inline ціле
lapack_gemm_c(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ціле a_m, ціле a_n, ціле d_n, ціле flags)
{
    ціле ldsrc1 = (ціле)(src1_step / sizeof(std::complex<fptype>));
    ціле ldsrc2 = (ціле)(src2_step / sizeof(std::complex<fptype>));
    ціле ldsrc3 = (ціле)(src3_step / sizeof(std::complex<fptype>));
    ціле lddst = (ціле)(dst_step / sizeof(std::complex<fptype>));
    ціле c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    якщо(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        якщо(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        інакше
        {
            d_m = a_m;
        }
    }
    інакше
    {
        transB = CblasNoTrans;
        якщо(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        інакше
        {
            d_m = a_m;
        }
    }

    якщо(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    інакше
    {
        c_m = d_m;
        c_n = d_n;
    }

    якщо(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    інакше
    {
        transA = CblasNoTrans;
    }

    якщо(src3 != dst && beta != 0.0 && src3_step != 0) {
        якщо(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        інакше
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    інакше якщо (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    інакше якщо(src3_step == 0 && beta != 0.0)
        set_value((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#якщо defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ціле M = a_m, N = d_n, K = a_n;
    якщо(typeid(fptype) == typeid(float)) {
        auto src1_cast = (std::complex<float>*)(src1);
        auto src2_cast = (std::complex<float>*)(src2);
        auto dst_cast = (std::complex<float>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<float>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<float>*)&cBeta, dst_cast, ldc);
    }
    інакше якщо(typeid(fptype) == typeid(double)) {
        auto src1_cast = (std::complex<double>*)(src1);
        auto src2_cast = (std::complex<double>*)(src2);
        auto dst_cast = (std::complex<double>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<double>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<double>*)&cBeta, dst_cast, ldc);
    }
#інакше
    якщо(typeid(fptype) == typeid(float))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float*)reinterpret_cast<fptype(&)[2]>(cAlpha), (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float*)reinterpret_cast<fptype(&)[2]>(cBeta), (float*)dst, lddst);
    інакше якщо(typeid(fptype) == typeid(double))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (double*)reinterpret_cast<fptype(&)[2]>(cAlpha), (double*)src1, ldsrc1, (double*)src2, ldsrc2, (double*)reinterpret_cast<fptype(&)[2]>(cBeta), (double*)dst, lddst);
#endif

    повернути CV_HAL_ERROR_OK;
}
ціле lapack_LU32f(float* a, size_t a_step, ціле m, float* b, size_t b_step, ціле n, ціле* info)
{
    якщо(m < HAL_LU_SMALL_MATRIX_THRESH)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_LU(a, a_step, m, b, b_step, n, info);
}

ціле lapack_LU64f(double* a, size_t a_step, ціле m, double* b, size_t b_step, ціле n, ціле* info)
{
    якщо(m < HAL_LU_SMALL_MATRIX_THRESH)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_LU(a, a_step, m, b, b_step, n, info);
}

ціле lapack_Cholesky32f(float* a, size_t a_step, ціле m, float* b, size_t b_step, ціле n, bool *info)
{
    якщо(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

ціле lapack_Cholesky64f(double* a, size_t a_step, ціле m, double* b, size_t b_step, ціле n, bool *info)
{
    якщо(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

ціле lapack_SVD32f(float* a, size_t a_step, float *w, float* u, size_t u_step, float* vt, size_t v_step, ціле m, ціле n, ціле flags)
{
    якщо(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    ціле info = 0;
    повернути lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

ціле lapack_SVD64f(double* a, size_t a_step, double *w, double* u, size_t u_step, double* vt, size_t v_step, ціле m, ціле n, ціле flags)
{
    якщо(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    ціле info = 0;
    повернути lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

ціле lapack_QR32f(float* src1, size_t src1_step, ціле m, ціле n, ціле k, float* src2, size_t src2_step, float* dst, ціле* info)
{
    якщо (m < HAL_QR_SMALL_MATRIX_THRESH)
      повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

ціле lapack_QR64f(double* src1, size_t src1_step, ціле m, ціле n, ціле k, double* src2, size_t src2_step, double* dst, ціле* info)
{
    якщо (m < HAL_QR_SMALL_MATRIX_THRESH)
      повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

ціле lapack_gemm32f(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, ціле m, ціле n, ціле k, ціле flags)
{
    якщо(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

ціле lapack_gemm64f(const double *src1, size_t src1_step, const double *src2, size_t src2_step, double alpha,
                   const double *src3, size_t src3_step, double beta, double *dst, size_t dst_step, ціле m, ціле n, ціле k, ціле flags)
{
    якщо(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

ціле lapack_gemm32fc(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, ціле m, ціле n, ціле k, ціле flags)
{
    якщо(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
ціле lapack_gemm64fc(const double *src1, size_t src1_step, const double *src2, size_t src2_step, double alpha,
                   const double *src3, size_t src3_step, double beta, double *dst, size_t dst_step, ціле m, ціле n, ціле k, ціле flags)
{
    якщо(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        повернути CV_HAL_ERROR_NOT_IMPLEMENTED;
    повернути lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#якщо defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
