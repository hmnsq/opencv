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

#གལ་སྲིད defined(__clang__) && defined(__has_feature)
#གལ་སྲིད __has_feature(memory_sanitizer)
#include <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) do { } སྐབས(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#གལ་སྲིད defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> static anta_hstha void
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    དོན་ལུ(size_t i = 0; i < m - 1; i++)
        དོན་ལུ(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> static anta_hstha void
transpose(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    དོན་ལུ(size_t i = 0; i < m; i++)
        དོན་ལུ(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> static anta_hstha void
copy_matrix(const fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    དོན་ལུ(size_t i = 0; i < m; i++)
        དོན་ལུ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> static anta_hstha void
set_value(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    དོན་ལུ(size_t i = 0; i < m; i++)
        དོན་ལུ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> static anta_hstha ཧྲིལ
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, ཧྲིལ m, fptype* b, size_t b_step, ཧྲིལ n, ཧྲིལ* info)
{
#གལ་སྲིད defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / parimaa_na(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#དེ་མིན
    cv::AutoBuffer<ཧྲིལ> piv_buff(m);
    ཧྲིལ lda = (ཧྲིལ)(a_step / parimaa_na(fptype));
    ཧྲིལ _m = m, _n = n;
    ཧྲིལ* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    གལ་སྲིད(b)
    {
        གལ་སྲིད(n == 1 && b_step == parimaa_na(fptype))
        {
            གལ་སྲིད(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)b, &_m, _info);
            དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
                dgesv_(&_m, &_n, (dviguu_na*)a, &lda, piv, (dviguu_na*)b, &_m, _info);
        }
        དེ་མིན
        {
            ཧྲིལ ldb = (ཧྲིལ)(b_step / parimaa_na(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            གལ་སྲིད(typeid(fptype) == typeid(float))
                sgesv_(&_m, &_n, (float*)a, &lda, piv, (float*)&tmpB[0], &_m, _info);
            དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
                dgesv_(&_m, &_n, (dviguu_na*)a, &lda, piv, (dviguu_na*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    དེ་མིན
    {
        གལ་སྲིད(typeid(fptype) == typeid(float))
            sgetrf_(&_m, &_m, (float*)a, &lda, piv, _info);
        དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
            dgetrf_(&_m, &_m, (dviguu_na*)a, &lda, piv, _info);
    }

#གལ་སྲིད defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ཧྲིལ>(_info[0]);
#endif
    ཧྲིལ retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    ཧྲིལ sign = 0;
    གལ་སྲིད(*info == 0)
    {
        དོན་ལུ(ཧྲིལ i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    དེ་མིན
        *info = 0; //in opencv LU function zero means error

    ལོག retcode;
}

template <typename fptype> static anta_hstha ཧྲིལ
lapack_Cholesky(fptype* a, size_t a_step, ཧྲིལ m, fptype* b, size_t b_step, ཧྲིལ n, bool* info)
{
#གལ་སྲིད defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / parimaa_na(fptype));
#དེ་མིན
    ཧྲིལ _m = m, _n = n;
    ཧྲིལ lapackStatus = 0;
    ཧྲིལ lda = (ཧྲིལ)(a_step / parimaa_na(fptype));
#endif
    var_na L[] = {'L', '\0'};

    གལ་སྲིད(b)
    {
        གལ་སྲིད(n == 1 && b_step == parimaa_na(fptype))
        {
            གལ་སྲིད(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)b, &_m, &lapackStatus);
            དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)b, &_m, &lapackStatus);
        }
        དེ་མིན
        {
            ཧྲིལ ldb = (ཧྲིལ)(b_step / parimaa_na(fptype));
            fptype* tmpB = new fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            གལ་སྲིད(typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (float*)a, &lda, (float*)tmpB, &_m, &lapackStatus);
            དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    དེ་མིན
    {
        གལ་སྲིད(typeid(fptype) == typeid(float))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (float*)a, &lda, &lapackStatus);
        དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (dviguu_na*)a, &lda, &lapackStatus);
    }

    གལ་སྲིད(lapackStatus == 0) *info = true;
    དེ་མིན *info = false; //in opencv Cholesky function false means error

    ལོག lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> static anta_hstha ཧྲིལ
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ flags, ཧྲིལ* info)
{
#གལ་སྲིད defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / parimaa_na(fptype));
    long ldv = (long)(v_step / parimaa_na(fptype));
    long ldu = (long)(u_step / parimaa_na(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#དེ་མིན
    ཧྲིལ _m = m, _n = n;
    ཧྲིལ* _info = info;
    ཧྲིལ lda = (ཧྲིལ)(a_step / parimaa_na(fptype));
    ཧྲིལ ldv = (ཧྲིལ)(v_step / parimaa_na(fptype));
    ཧྲིལ ldu = (ཧྲིལ)(u_step / parimaa_na(fptype));
    ཧྲིལ lwork = -1;
    cv::AutoBuffer<ཧྲིལ> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    var_na mode[] = { ' ', '\0'};
    གལ་སྲིད(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    དེ་མིན གལ་སྲིད((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    དེ་མིན གལ་སྲིད((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    དེ་མིན གལ་སྲིད(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    གལ་སྲིད((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    གལ་སྲིད(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&work1, &lwork, iworkBuf, _info);
    དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)w, (dviguu_na*)u, &ldu, (dviguu_na*)vt, &ldv, (dviguu_na*)&work1, &lwork, iworkBuf, _info);

    གལ་སྲིད(*info < 0)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (ཧྲིལ)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), parimaa_na(fptype) * (lwork + 1));

    གལ་སྲིད(typeid(fptype) == typeid(float))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (float*)a, &lda, (float*)w, (float*)u, &ldu, (float*)vt, &ldv, (float*)&buffer[0], &lwork, iworkBuf, _info);
    དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (dviguu_na*)a, &lda, (dviguu_na*)w, (dviguu_na*)u, &ldu, (dviguu_na*)vt, &ldv, (dviguu_na*)&buffer[0], &lwork, iworkBuf, _info);

#གལ་སྲིད defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ཧྲིལ>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    གལ་སྲིད (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    གལ་སྲིད (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    གལ་སྲིད (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, parimaa_na(fptype) * std::min(m, n));

    གལ་སྲིད(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    གལ་སྲིད((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        དོན་ལུ(ཧྲིལ i = 0; i < m; i++)
            དོན་ལུ(ཧྲིལ j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    གལ་སྲིད(*info < 0)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha ཧྲིལ
lapack_QR(fptype* a, size_t a_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ k, fptype* b, size_t b_step, fptype* dst, ཧྲིལ* info)
{
#གལ་སྲིད defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / parimaa_na(fptype));
    long lwork = -1;
    long ldtmpA;
#དེ་མིན
    ཧྲིལ _m = m, _n = n, _k = k;
    ཧྲིལ* _info = info;
    ཧྲིལ lda = (ཧྲིལ)(a_step / parimaa_na(fptype));
    ཧྲིལ lwork = -1;
    ཧྲིལ ldtmpA;
#endif

    var_na mode[] = { 'N', '\0' };
    གལ་སྲིད(m < n)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    གལ་སྲིད (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    དེ་མིན
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    གལ་སྲིད (b)
    {
        གལ་སྲིད (k == 1 && b_step == parimaa_na(fptype))
        {
            གལ་སྲིད (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)&work1, &lwork, _info);
            དེ་མིན གལ་སྲིད (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)b, &_m, (dviguu_na*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(ཧྲིལ));
            གལ་སྲིད (*info < 0)
                ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            གལ་སྲིད (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)b, &_m, (float*)buffer, &lwork, _info);
            དེ་མིན གལ་སྲིད (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)b, &_m, (dviguu_na*)buffer, &lwork, _info);
        }
        དེ་མིན
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            ཧྲིལ ldb = (ཧྲིལ)(b_step / parimaa_na(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            གལ་སྲིད (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)&work1, &lwork, _info);
            དེ་མིན གལ་སྲིད (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)tmpB, &_m, (dviguu_na*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(ཧྲིལ));
            གལ་སྲིད (*info < 0)
                ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            གལ་སྲིད (typeid(fptype) == typeid(float))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (float*)tmpA, &ldtmpA, (float*)tmpB, &_m, (float*)buffer, &lwork, _info);
            དེ་མིན གལ་སྲིད (typeid(fptype) == typeid(dviguu_na))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)tmpB, &_m, (dviguu_na*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    དེ་མིན
    {
        གལ་སྲིད (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)&work1, &lwork, _info);
        དེ་མིན གལ་སྲིད (typeid(fptype) == typeid(dviguu_na))
            dgeqrf_(&_m, &_n, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)dst, (dviguu_na*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(ཧྲིལ));
        གལ་སྲིད (*info < 0)
            ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        གལ་སྲིད (typeid(fptype) == typeid(float))
            sgeqrf_(&_m, &_n, (float*)tmpA, &ldtmpA, (float*)dst, (float*)buffer, &lwork, _info);
        དེ་མིན གལ་སྲིད (typeid(fptype) == typeid(dviguu_na))
            dgeqrf_(&_m, &_n, (dviguu_na*)tmpA, &ldtmpA, (dviguu_na*)dst, (dviguu_na*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, parimaa_na(ཧྲིལ));
    གལ་སྲིད (m == n)
        transpose_square_inplace(a, lda, m);
    དེ་མིན
        transpose(tmpA, m, a, lda, n, m);

#གལ་སྲིད defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ཧྲིལ>(_info[0]);
#endif

    གལ་སྲིད (*info != 0)
        *info = 0;
    དེ་མིན
        *info = 1;

    ལོག CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha ཧྲིལ
lapack_gemm(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ཧྲིལ a_m, ཧྲིལ a_n, ཧྲིལ d_n, ཧྲིལ flags)
{
    ཧྲིལ ldsrc1 = (ཧྲིལ)(src1_step / parimaa_na(fptype));
    ཧྲིལ ldsrc2 = (ཧྲིལ)(src2_step / parimaa_na(fptype));
    ཧྲིལ ldsrc3 = (ཧྲིལ)(src3_step / parimaa_na(fptype));
    ཧྲིལ lddst = (ཧྲིལ)(dst_step / parimaa_na(fptype));
    ཧྲིལ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    གལ་སྲིད(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        གལ་སྲིད(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        དེ་མིན
        {
            d_m = a_m;
        }
    }
    དེ་མིན
    {
        transB = CblasNoTrans;
        གལ་སྲིད(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        དེ་མིན
        {
            d_m = a_m;
        }
    }

    གལ་སྲིད(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    དེ་མིན
    {
        c_m = d_m;
        c_n = d_n;
    }

    གལ་སྲིད(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    དེ་མིན
    {
        transA = CblasNoTrans;
    }

    གལ་སྲིད(src3 != dst && beta != 0.0 && src3_step != 0) {
        གལ་སྲིད(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        དེ་མིན
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    དེ་མིན གལ་སྲིད (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    དེ་མིན གལ་སྲིད(src3_step == 0 && beta != 0.0)
        set_value(dst, lddst, (fptype)0.0, d_m, d_n);

    གལ་སྲིད(typeid(fptype) == typeid(float))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float)alpha, (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float)beta, (float*)dst, lddst);
    དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (dviguu_na)alpha, (dviguu_na*)src1, ldsrc1, (dviguu_na*)src2, ldsrc2, (dviguu_na)beta, (dviguu_na*)dst, lddst);

    ལོག CV_HAL_ERROR_OK;
}

template <typename fptype> static anta_hstha ཧྲིལ
lapack_gemm_c(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
            const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ཧྲིལ a_m, ཧྲིལ a_n, ཧྲིལ d_n, ཧྲིལ flags)
{
    ཧྲིལ ldsrc1 = (ཧྲིལ)(src1_step / parimaa_na(std::complex<fptype>));
    ཧྲིལ ldsrc2 = (ཧྲིལ)(src2_step / parimaa_na(std::complex<fptype>));
    ཧྲིལ ldsrc3 = (ཧྲིལ)(src3_step / parimaa_na(std::complex<fptype>));
    ཧྲིལ lddst = (ཧྲིལ)(dst_step / parimaa_na(std::complex<fptype>));
    ཧྲིལ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    གལ་སྲིད(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        གལ་སྲིད(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        དེ་མིན
        {
            d_m = a_m;
        }
    }
    དེ་མིན
    {
        transB = CblasNoTrans;
        གལ་སྲིད(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        དེ་མིན
        {
            d_m = a_m;
        }
    }

    གལ་སྲིད(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    དེ་མིན
    {
        c_m = d_m;
        c_n = d_n;
    }

    གལ་སྲིད(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    དེ་མིན
    {
        transA = CblasNoTrans;
    }

    གལ་སྲིད(src3 != dst && beta != 0.0 && src3_step != 0) {
        གལ་སྲིད(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        དེ་མིན
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    དེ་མིན གལ་སྲིད (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    དེ་མིན གལ་སྲིད(src3_step == 0 && beta != 0.0)
        set_value((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#གལ་སྲིད defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ཧྲིལ M = a_m, N = d_n, K = a_n;
    གལ་སྲིད(typeid(fptype) == typeid(float)) {
        auto src1_cast = (std::complex<float>*)(src1);
        auto src2_cast = (std::complex<float>*)(src2);
        auto dst_cast = (std::complex<float>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<float>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<float>*)&cBeta, dst_cast, ldc);
    }
    དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na)) {
        auto src1_cast = (std::complex<dviguu_na>*)(src1);
        auto src2_cast = (std::complex<dviguu_na>*)(src2);
        auto dst_cast = (std::complex<dviguu_na>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<dviguu_na>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<dviguu_na>*)&cBeta, dst_cast, ldc);
    }
#དེ་མིན
    གལ་སྲིད(typeid(fptype) == typeid(float))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (float*)reinterpret_cast<fptype(&)[2]>(cAlpha), (float*)src1, ldsrc1, (float*)src2, ldsrc2, (float*)reinterpret_cast<fptype(&)[2]>(cBeta), (float*)dst, lddst);
    དེ་མིན གལ་སྲིད(typeid(fptype) == typeid(dviguu_na))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (dviguu_na*)reinterpret_cast<fptype(&)[2]>(cAlpha), (dviguu_na*)src1, ldsrc1, (dviguu_na*)src2, ldsrc2, (dviguu_na*)reinterpret_cast<fptype(&)[2]>(cBeta), (dviguu_na*)dst, lddst);
#endif

    ལོག CV_HAL_ERROR_OK;
}
ཧྲིལ lapack_LU32f(float* a, size_t a_step, ཧྲིལ m, float* b, size_t b_step, ཧྲིལ n, ཧྲིལ* info)
{
    གལ་སྲིད(m < HAL_LU_SMALL_MATRIX_THRESH)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_LU(a, a_step, m, b, b_step, n, info);
}

ཧྲིལ lapack_LU64f(dviguu_na* a, size_t a_step, ཧྲིལ m, dviguu_na* b, size_t b_step, ཧྲིལ n, ཧྲིལ* info)
{
    གལ་སྲིད(m < HAL_LU_SMALL_MATRIX_THRESH)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_LU(a, a_step, m, b, b_step, n, info);
}

ཧྲིལ lapack_Cholesky32f(float* a, size_t a_step, ཧྲིལ m, float* b, size_t b_step, ཧྲིལ n, bool *info)
{
    གལ་སྲིད(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

ཧྲིལ lapack_Cholesky64f(dviguu_na* a, size_t a_step, ཧྲིལ m, dviguu_na* b, size_t b_step, ཧྲིལ n, bool *info)
{
    གལ་སྲིད(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

ཧྲིལ lapack_SVD32f(float* a, size_t a_step, float *w, float* u, size_t u_step, float* vt, size_t v_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ flags)
{
    གལ་སྲིད(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ཧྲིལ info = 0;
    ལོག lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

ཧྲིལ lapack_SVD64f(dviguu_na* a, size_t a_step, dviguu_na *w, dviguu_na* u, size_t u_step, dviguu_na* vt, size_t v_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ flags)
{
    གལ་སྲིད(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ཧྲིལ info = 0;
    ལོག lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

ཧྲིལ lapack_QR32f(float* src1, size_t src1_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ k, float* src2, size_t src2_step, float* dst, ཧྲིལ* info)
{
    གལ་སྲིད (m < HAL_QR_SMALL_MATRIX_THRESH)
      ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

ཧྲིལ lapack_QR64f(dviguu_na* src1, size_t src1_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ k, dviguu_na* src2, size_t src2_step, dviguu_na* dst, ཧྲིལ* info)
{
    གལ་སྲིད (m < HAL_QR_SMALL_MATRIX_THRESH)
      ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

ཧྲིལ lapack_gemm32f(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ k, ཧྲིལ flags)
{
    གལ་སྲིད(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

ཧྲིལ lapack_gemm64f(const dviguu_na *src1, size_t src1_step, const dviguu_na *src2, size_t src2_step, dviguu_na alpha,
                   const dviguu_na *src3, size_t src3_step, dviguu_na beta, dviguu_na *dst, size_t dst_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ k, ཧྲིལ flags)
{
    གལ་སྲིད(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

ཧྲིལ lapack_gemm32fc(const float *src1, size_t src1_step, const float *src2, size_t src2_step, float alpha,
                   const float *src3, size_t src3_step, float beta, float *dst, size_t dst_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ k, ཧྲིལ flags)
{
    གལ་སྲིད(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
ཧྲིལ lapack_gemm64fc(const dviguu_na *src1, size_t src1_step, const dviguu_na *src2, size_t src2_step, dviguu_na alpha,
                   const dviguu_na *src3, size_t src3_step, dviguu_na beta, dviguu_na *dst, size_t dst_step, ཧྲིལ m, ཧྲིལ n, ཧྲིལ k, ཧྲིལ flags)
{
    གལ་སྲིད(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ལོག CV_HAL_ERROR_NOT_IMPLEMENTED;
    ལོག lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#གལ་སྲིད defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
