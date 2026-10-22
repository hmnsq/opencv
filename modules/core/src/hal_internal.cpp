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

#ако defined(__clang__) && defined(__has_feature)
#ако __has_feature(memory_sanitizer)
#include <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) do { } док(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#ако defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> static inline void
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    за(size_t i = 0; i < m - 1; i++)
        за(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> static inline void
transpose(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    за(size_t i = 0; i < m; i++)
        за(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> static inline void
copy_matrix(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    за(size_t i = 0; i < m; i++)
        за(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> static inline void
set_value(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    за(size_t i = 0; i < m; i++)
        за(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> static inline цео
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, цео m, fptype* b, size_t b_step, цео n, цео* info)
{
#ако defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / sizeof(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#инако
    cv::AutoBuffer<цео> piv_buff(m);
    цео lda = (цео)(a_step / sizeof(fptype));
    цео _m = m, _n = n;
    цео* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    ако(b)
    {
        ако(n == 1 && b_step == sizeof(fptype))
        {
            ако(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)b, &_m, _info);
            инако ако(typeid(fptype) == typeid(double))
                dgesv_(&_m, &_n, (double*)a, &lda, piv, (double*)b, &_m, _info);
        }
        инако
        {
            цео ldb = (цео)(b_step / sizeof(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            ако(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)&tmpB[0], &_m, _info);
            инако ако(typeid(fptype) == typeid(double))
                dgesv_(&_m, &_n, (double*)a, &lda, piv, (double*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    инако
    {
        ако(typeid(fptype) == typeid(float))
            sgetrf_(&_m, &_m, (float*)a, &lda, piv, _info);
        инако ако(typeid(fptype) == typeid(double))
            dgetrf_(&_m, &_m, (double*)a, &lda, piv, _info);
    }

#ако defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<цео>(_info[0]);
#endif
    цео retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    цео sign = 0;
    ако(*info == 0)
    {
        за(цео i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    инако
        *info = 0; //in opencv LU function zero means error

    врати retcode;
}

template <typename fptype> static inline цео
lapack_Cholesky(fptype* a, size_t a_step, цео m, fptype* b, size_t b_step, цео n, bool* info)
{
#ако defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / sizeof(fptype));
#инако
    цео _m = m, _n = n;
    цео lapackStatus = 0;
    цео lda = (цео)(a_step / sizeof(fptype));
#endif
    char L[] = {'L', '\0'};

    ако(b)
    {
        ако(n == 1 && b_step == sizeof(fptype))
        {
            ако(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)b, &_m, &lapackStatus);
            инако ако(typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (double*)a, &lda, (double*)b, &_m, &lapackStatus);
        }
        инако
        {
            цео ldb = (цео)(b_step / sizeof(fptype));
            fptype* tmpB = new fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            ако(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)tmpB, &_m, &lapackStatus);
            инако ако(typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (double*)a, &lda, (double*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    инако
    {
        ако(typeid(fptype) == typeid(float))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (float*)a, &lda, &lapackStatus);
        инако ако(typeid(fptype) == typeid(double))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (double*)a, &lda, &lapackStatus);
    }

    ако(lapackStatus == 0) *info = true;
    инако *info = false; //in opencv Cholesky function false means error

    врати lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> static inline цео
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, цео m, цео n, цео flags, цео* info)
{
#ако defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / sizeof(fptype));
    long ldv = (long)(v_step / sizeof(fptype));
    long ldu = (long)(u_step / sizeof(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#инако
    цео _m = m, _n = n;
    цео* _info = info;
    цео lda = (цео)(a_step / sizeof(fptype));
    цео ldv = (цео)(v_step / sizeof(fptype));
    цео ldu = (цео)(u_step / sizeof(fptype));
    цео lwork = -1;
    cv::AutoBuffer<цео> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    char mode[] = { ' ', '\0'};
    ако(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    инако ако((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    инако ако((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    инако ако(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    ако((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    ако(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&work1, &lwork, iworkBuf, _info);
    инако ако(typeid(fptype) == typeid(double))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (double*)a, &lda, (double*)w, (double*)u, &ldu, (double*)vt, &ldv, (double*)&work1, &lwork, iworkBuf, _info);

    ако(*info < 0)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (цео)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), sizeof(fptype) * (lwork + 1));

    ако(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&buffer[0], &lwork, iworkBuf, _info);
    инако ако(typeid(fptype) == typeid(double))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (double*)a, &lda, (double*)w, (double*)u, &ldu, (double*)vt, &ldv, (double*)&buffer[0], &lwork, iworkBuf, _info);

#ако defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<цео>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    ако (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    ако (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    ако (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, sizeof(fptype) * std::min(m, n));

    ако(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    ако((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        за(цео i = 0; i < m; i++)
            за(цео j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    ако(*info < 0)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати CV_HAL_ERROR_OK;
}

template <typename fptype> static inline цео
lapack_QR(fptype* a, size_t a_step, цео m, цео n, цео k, fptype* b, size_t b_step, fptype* dst, цео* info)
{
#ако defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / sizeof(fptype));
    long lwork = -1;
    long ldtmpA;
#инако
    цео _m = m, _n = n, _k = k;
    цео* _info = info;
    цео lda = (цео)(a_step / sizeof(fptype));
    цео lwork = -1;
    цео ldtmpA;
#endif

    char mode[] = { 'N', '\0' };
    ако(m < n)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    ако (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    инако
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    ако (b)
    {
        ако (k == 1 && b_step == sizeof(fptype))
        {
            ако (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)&work1, &lwork, _info);
            инако ако (typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (double*)tmpA, &ldtmpA, (double*)b, &_m, (double*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, sizeof(цео));
            ако (*info < 0)
                врати CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ако (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)buffer, &lwork, _info);
            инако ако (typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (double*)tmpA, &ldtmpA, (double*)b, &_m, (double*)buffer, &lwork, _info);
        }
        инако
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            цео ldb = (цео)(b_step / sizeof(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            ако (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)&work1, &lwork, _info);
            инако ако (typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (double*)tmpA, &ldtmpA, (double*)tmpB, &_m, (double*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, sizeof(цео));
            ако (*info < 0)
                врати CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ако (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)buffer, &lwork, _info);
            инако ако (typeid(fptype) == typeid(double))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (double*)tmpA, &ldtmpA, (double*)tmpB, &_m, (double*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    инако
    {
        ако (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)&work1, &lwork, _info);
        инако ако (typeid(fptype) == typeid(double))
            dgeqrf_(&_m, &_n, (double*)tmpA, &ldtmpA, (double*)dst, (double*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, sizeof(цео));
        ако (*info < 0)
            врати CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        ако (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)buffer, &lwork, _info);
        инако ако (typeid(fptype) == typeid(double))
            dgeqrf_(&_m, &_n, (double*)tmpA, &ldtmpA, (double*)dst, (double*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, sizeof(цео));
    ако (m == n)
        transpose_square_inplace(a, lda, m);
    инако
        transpose(tmpA, m, a, lda, n, m);

#ако defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<цео>(_info[0]);
#endif

    ако (*info != 0)
        *info = 0;
    инако
        *info = 1;

    врати CV_HAL_ERROR_OK;
}

template <typename fptype> static inline цео
lapack_gemm(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, цео a_m, цео a_n, цео d_n, цео flags)
{
    цео ldsrc1 = (цео)(src1_step / sizeof(fptype));
    цео ldsrc2 = (цео)(src2_step / sizeof(fptype));
    цео ldsrc3 = (цео)(src3_step / sizeof(fptype));
    цео lddst = (цео)(dst_step / sizeof(fptype));
    цео c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    ако(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ако(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        инако
        {
            d_m = a_m;
        }
    }
    инако
    {
        transB = CblasNoTrans;
        ако(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        инако
        {
            d_m = a_m;
        }
    }

    ако(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    инако
    {
        c_m = d_m;
        c_n = d_n;
    }

    ако(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    инако
    {
        transA = CblasNoTrans;
    }

    ако(src3 != dst && beta != 0.0 && src3_step != 0) {
        ако(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        инако
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    инако ако (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    инако ако(src3_step == 0 && beta != 0.0)
        set_value(dst, lddst, (fptype)0.0, d_m, d_n);

    ако(typeid(fptype) == typeid(float))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float)alpha, (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float)beta, (float*)dst, lddst);
    инако ако(typeid(fptype) == typeid(double))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (double)alpha, (double*)src1, ldsrc1, (double*)src2, ldsrc2, (double)beta, (double*)dst, lddst);

    врати CV_HAL_ERROR_OK;
}

template <typename fptype> static inline цео
lapack_gemm_c(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, цео a_m, цео a_n, цео d_n, цео flags)
{
    цео ldsrc1 = (цео)(src1_step / sizeof(std::complex<fptype>));
    цео ldsrc2 = (цео)(src2_step / sizeof(std::complex<fptype>));
    цео ldsrc3 = (цео)(src3_step / sizeof(std::complex<fptype>));
    цео lddst = (цео)(dst_step / sizeof(std::complex<fptype>));
    цео c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    ако(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ако(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        инако
        {
            d_m = a_m;
        }
    }
    инако
    {
        transB = CblasNoTrans;
        ако(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        инако
        {
            d_m = a_m;
        }
    }

    ако(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    инако
    {
        c_m = d_m;
        c_n = d_n;
    }

    ако(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    инако
    {
        transA = CblasNoTrans;
    }

    ако(src3 != dst && beta != 0.0 && src3_step != 0) {
        ако(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        инако
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    инако ако (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    инако ако(src3_step == 0 && beta != 0.0)
        set_value((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#ако defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    цео M = a_m, N = d_n, K = a_n;
    ако(typeid(fptype) == typeid(float)) {
        auto src1_cast = (std::complex<float>*)(src1);
        auto src2_cast = (std::complex<float>*)(src2);
        auto dst_cast = (std::complex<float>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<float>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<float>*)&cBeta, dst_cast, ldc);
    }
    инако ако(typeid(fptype) == typeid(double)) {
        auto src1_cast = (std::complex<double>*)(src1);
        auto src2_cast = (std::complex<double>*)(src2);
        auto dst_cast = (std::complex<double>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<double>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<double>*)&cBeta, dst_cast, ldc);
    }
#инако
    ако(typeid(fptype) == typeid(float))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float*)reinterpret_cast<fptype(&)[2]>(cAlpha), (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float*)reinterpret_cast<fptype(&)[2]>(cBeta), (float*)dst, lddst);
    инако ако(typeid(fptype) == typeid(double))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (double*)reinterpret_cast<fptype(&)[2]>(cAlpha), (double*)src1, ldsrc1, (double*)src2, ldsrc2, (double*)reinterpret_cast<fptype(&)[2]>(cBeta), (double*)dst, lddst);
#endif

    врати CV_HAL_ERROR_OK;
}
цео lapack_LU32f(float* a, size_t a_step, цео m, float* b, size_t b_step, цео n, цео* info)
{
    ако(m < HAL_LU_SMALL_MATRIX_THRESH)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_LU(a, a_step, m, b, b_step, n, info);
}

цео lapack_LU64f(double* a, size_t a_step, цео m, double* b, size_t b_step, цео n, цео* info)
{
    ако(m < HAL_LU_SMALL_MATRIX_THRESH)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_LU(a, a_step, m, b, b_step, n, info);
}

цео lapack_Cholesky32f(float* a, size_t a_step, цео m, float* b, size_t b_step, цео n, bool *info)
{
    ако(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

цео lapack_Cholesky64f(double* a, size_t a_step, цео m, double* b, size_t b_step, цео n, bool *info)
{
    ако(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

цео lapack_SVD32f(float* a, size_t a_step, float *w, float* u, size_t u_step, float* vt, size_t v_step, цео m, цео n, цео flags)
{
    ако(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    цео info = 0;
    врати lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

цео lapack_SVD64f(double* a, size_t a_step, double *w, double* u, size_t u_step, double* vt, size_t v_step, цео m, цео n, цео flags)
{
    ако(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    цео info = 0;
    врати lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

цео lapack_QR32f(float* src1, size_t src1_step, цео m, цео n, цео k, float* src2, size_t src2_step, float* dst, цео* info)
{
    ако (m < HAL_QR_SMALL_MATRIX_THRESH)
      врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

цео lapack_QR64f(double* src1, size_t src1_step, цео m, цео n, цео k, double* src2, size_t src2_step, double* dst, цео* info)
{
    ако (m < HAL_QR_SMALL_MATRIX_THRESH)
      врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

цео lapack_gemm32f(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, цео m, цео n, цео k, цео flags)
{
    ако(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

цео lapack_gemm64f(const double *src1, size_t src1_step, const double *src2, size_t src2_step, double alpha,
                   const double *src3, size_t src3_step, double beta, double *dst, size_t dst_step, цео m, цео n, цео k, цео flags)
{
    ако(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

цео lapack_gemm32fc(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, цео m, цео n, цео k, цео flags)
{
    ако(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
цео lapack_gemm64fc(const double *src1, size_t src1_step, const double *src2, size_t src2_step, double alpha,
                   const double *src3, size_t src3_step, double beta, double *dst, size_t dst_step, цео m, цео n, цео k, цео flags)
{
    ако(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        врати CV_HAL_ERROR_NOT_IMPLEMENTED;
    врати lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#ако defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
