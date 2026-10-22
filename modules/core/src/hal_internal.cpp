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

#ضمن "precomp.hpp"
#ضمن "hal_internal.hpp"

#ifdef HAVE_LAPACK

#ضمن "opencv_lapack.h"

#ضمن <cmath>
#ضمن <algorithm>
#ضمن <typeinfo>
#ضمن <limits>
#ضمن <complex>
#ضمن <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#إذا defined(__clang__) && defined(__has_feature)
#إذا __has_feature(memory_sanitizer)
#ضمن <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) افعل { } طالما(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#إذا defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> ساكن مضمن فراغ
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    لكل(size_t i = 0; i < m - 1; i++)
        لكل(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> ساكن مضمن فراغ
transpose(ثابت fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    لكل(size_t i = 0; i < m; i++)
        لكل(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> ساكن مضمن فراغ
copy_matrix(ثابت fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    لكل(size_t i = 0; i < m; i++)
        لكل(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> ساكن مضمن فراغ
set_نيلاي_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    لكل(size_t i = 0; i < m; i++)
        لكل(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> ساكن مضمن صحيح
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, صحيح m, fptype* b, size_t b_step, صحيح n, صحيح* info)
{
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / حجم(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#وإلا
    cv::AutoBuffer<صحيح> piv_buff(m);
    صحيح lda = (صحيح)(a_step / حجم(fptype));
    صحيح _m = m, _n = n;
    صحيح* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    إذا(b)
    {
        إذا(n == 1 && b_step == حجم(fptype))
        {
            إذا(typeid(fptype) == typeid(عائم))
                sgesv_(&_m, &_n, (عائم*)a, &lda, piv, (عائم*)b, &_m, _info);
            وإلا إذا(typeid(fptype) == typeid(مزدوج))
                dgesv_(&_m, &_n, (مزدوج*)a, &lda, piv, (مزدوج*)b, &_m, _info);
        }
        وإلا
        {
            صحيح ldb = (صحيح)(b_step / حجم(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            إذا(typeid(fptype) == typeid(عائم))
                sgesv_(&_m, &_n, (عائم*)a, &lda, piv, (عائم*)&tmpB[0], &_m, _info);
            وإلا إذا(typeid(fptype) == typeid(مزدوج))
                dgesv_(&_m, &_n, (مزدوج*)a, &lda, piv, (مزدوج*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    وإلا
    {
        إذا(typeid(fptype) == typeid(عائم))
            sgetrf_(&_m, &_m, (عائم*)a, &lda, piv, _info);
        وإلا إذا(typeid(fptype) == typeid(مزدوج))
            dgetrf_(&_m, &_m, (مزدوج*)a, &lda, piv, _info);
    }

#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحيح>(_info[0]);
#endif
    صحيح retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    صحيح sign = 0;
    إذا(*info == 0)
    {
        لكل(صحيح i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    وإلا
        *info = 0; //in opencv LU function zero means error

    أرجع retcode;
}

template <typename fptype> ساكن مضمن صحيح
lapack_Cholesky(fptype* a, size_t a_step, صحيح m, fptype* b, size_t b_step, صحيح n, منطقي* info)
{
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / حجم(fptype));
#وإلا
    صحيح _m = m, _n = n;
    صحيح lapackStatus = 0;
    صحيح lda = (صحيح)(a_step / حجم(fptype));
#endif
    حرف L[] = {'L', '\0'};

    إذا(b)
    {
        إذا(n == 1 && b_step == حجم(fptype))
        {
            إذا(typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (عائم*)a, &lda, (عائم*)b, &_m, &lapackStatus);
            وإلا إذا(typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (مزدوج*)a, &lda, (مزدوج*)b, &_m, &lapackStatus);
        }
        وإلا
        {
            صحيح ldb = (صحيح)(b_step / حجم(fptype));
            fptype* tmpB = جديد fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            إذا(typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (عائم*)a, &lda, (عائم*)tmpB, &_m, &lapackStatus);
            وإلا إذا(typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (مزدوج*)a, &lda, (مزدوج*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    وإلا
    {
        إذا(typeid(fptype) == typeid(عائم))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (عائم*)a, &lda, &lapackStatus);
        وإلا إذا(typeid(fptype) == typeid(مزدوج))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (مزدوج*)a, &lda, &lapackStatus);
    }

    إذا(lapackStatus == 0) *info = true;
    وإلا *info = خطأ; //in opencv Cholesky function false means error

    أرجع lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> ساكن مضمن صحيح
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, صحيح m, صحيح n, صحيح flags, صحيح* info)
{
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / حجم(fptype));
    long ldv = (long)(v_step / حجم(fptype));
    long ldu = (long)(u_step / حجم(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#وإلا
    صحيح _m = m, _n = n;
    صحيح* _info = info;
    صحيح lda = (صحيح)(a_step / حجم(fptype));
    صحيح ldv = (صحيح)(v_step / حجم(fptype));
    صحيح ldu = (صحيح)(u_step / حجم(fptype));
    صحيح lwork = -1;
    cv::AutoBuffer<صحيح> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    حرف mode[] = { ' ', '\0'};
    إذا(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    وإلا إذا((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    وإلا إذا((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    وإلا إذا(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    إذا((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    إذا(typeid(fptype) == typeid(عائم))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (عائم*)a, &lda, (عائم*)w, (عائم*)u, &ldu, (عائم*)vt, &ldv, (عائم*)&work1, &lwork, iworkBuf, _info);
    وإلا إذا(typeid(fptype) == typeid(مزدوج))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (مزدوج*)a, &lda, (مزدوج*)w, (مزدوج*)u, &ldu, (مزدوج*)vt, &ldv, (مزدوج*)&work1, &lwork, iworkBuf, _info);

    إذا(*info < 0)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (صحيح)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), حجم(fptype) * (lwork + 1));

    إذا(typeid(fptype) == typeid(عائم))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (عائم*)a, &lda, (عائم*)w, (عائم*)u, &ldu, (عائم*)vt, &ldv, (عائم*)&buffer[0], &lwork, iworkBuf, _info);
    وإلا إذا(typeid(fptype) == typeid(مزدوج))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (مزدوج*)a, &lda, (مزدوج*)w, (مزدوج*)u, &ldu, (مزدوج*)vt, &ldv, (مزدوج*)&buffer[0], &lwork, iworkBuf, _info);

#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحيح>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    إذا (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    إذا (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    إذا (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, حجم(fptype) * std::min(m, n));

    إذا(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    إذا((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        لكل(صحيح i = 0; i < m; i++)
            لكل(صحيح j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    إذا(*info < 0)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع CV_HAL_ERROR_OK;
}

template <typename fptype> ساكن مضمن صحيح
lapack_QR(fptype* a, size_t a_step, صحيح m, صحيح n, صحيح k, fptype* b, size_t b_step, fptype* dst, صحيح* info)
{
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / حجم(fptype));
    long lwork = -1;
    long ldtmpA;
#وإلا
    صحيح _m = m, _n = n, _k = k;
    صحيح* _info = info;
    صحيح lda = (صحيح)(a_step / حجم(fptype));
    صحيح lwork = -1;
    صحيح ldtmpA;
#endif

    حرف mode[] = { 'N', '\0' };
    إذا(m < n)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    إذا (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    وإلا
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    إذا (b)
    {
        إذا (k == 1 && b_step == حجم(fptype))
        {
            إذا (typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (عائم*)tmpA, &ldtmpA, (عائم*)b, &_m, (عائم*)&work1, &lwork, _info);
            وإلا إذا (typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)b, &_m, (مزدوج*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, حجم(صحيح));
            إذا (*info < 0)
                أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            إذا (typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (عائم*)tmpA, &ldtmpA, (عائم*)b, &_m, (عائم*)buffer, &lwork, _info);
            وإلا إذا (typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)b, &_m, (مزدوج*)buffer, &lwork, _info);
        }
        وإلا
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            صحيح ldb = (صحيح)(b_step / حجم(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            إذا (typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (عائم*)tmpA, &ldtmpA, (عائم*)tmpB, &_m, (عائم*)&work1, &lwork, _info);
            وإلا إذا (typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)tmpB, &_m, (مزدوج*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, حجم(صحيح));
            إذا (*info < 0)
                أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            إذا (typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (عائم*)tmpA, &ldtmpA, (عائم*)tmpB, &_m, (عائم*)buffer, &lwork, _info);
            وإلا إذا (typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)tmpB, &_m, (مزدوج*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    وإلا
    {
        إذا (typeid(fptype) == typeid(عائم))
            sgeqrf_(&_m, &_n, (عائم*)tmpA, &ldtmpA, (عائم*)dst, (عائم*)&work1, &lwork, _info);
        وإلا إذا (typeid(fptype) == typeid(مزدوج))
            dgeqrf_(&_m, &_n, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)dst, (مزدوج*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, حجم(صحيح));
        إذا (*info < 0)
            أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        إذا (typeid(fptype) == typeid(عائم))
            sgeqrf_(&_m, &_n, (عائم*)tmpA, &ldtmpA, (عائم*)dst, (عائم*)buffer, &lwork, _info);
        وإلا إذا (typeid(fptype) == typeid(مزدوج))
            dgeqrf_(&_m, &_n, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)dst, (مزدوج*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, حجم(صحيح));
    إذا (m == n)
        transpose_square_inplace(a, lda, m);
    وإلا
        transpose(tmpA, m, a, lda, n, m);

#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحيح>(_info[0]);
#endif

    إذا (*info != 0)
        *info = 0;
    وإلا
        *info = 1;

    أرجع CV_HAL_ERROR_OK;
}

template <typename fptype> ساكن مضمن صحيح
lapack_gemm(ثابت fptype *src1, size_t src1_step, ثابت fptype *src2, size_t src2_step, fptype alpha,
            ثابت fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, صحيح a_m, صحيح a_n, صحيح d_n, صحيح flags)
{
    صحيح ldsrc1 = (صحيح)(src1_step / حجم(fptype));
    صحيح ldsrc2 = (صحيح)(src2_step / حجم(fptype));
    صحيح ldsrc3 = (صحيح)(src3_step / حجم(fptype));
    صحيح lddst = (صحيح)(dst_step / حجم(fptype));
    صحيح c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    إذا(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        إذا(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        وإلا
        {
            d_m = a_m;
        }
    }
    وإلا
    {
        transB = CblasNoTrans;
        إذا(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        وإلا
        {
            d_m = a_m;
        }
    }

    إذا(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    وإلا
    {
        c_m = d_m;
        c_n = d_n;
    }

    إذا(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    وإلا
    {
        transA = CblasNoTrans;
    }

    إذا(src3 != dst && beta != 0.0 && src3_step != 0) {
        إذا(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        وإلا
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    وإلا إذا (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    وإلا إذا(src3_step == 0 && beta != 0.0)
        set_نيلاي_2(dst, lddst, (fptype)0.0, d_m, d_n);

    إذا(typeid(fptype) == typeid(عائم))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (عائم)alpha, (عائم*)src1, ldsrc1, (عائم*)src2, ldsrc2, (عائم)beta, (عائم*)dst, lddst);
    وإلا إذا(typeid(fptype) == typeid(مزدوج))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (مزدوج)alpha, (مزدوج*)src1, ldsrc1, (مزدوج*)src2, ldsrc2, (مزدوج)beta, (مزدوج*)dst, lddst);

    أرجع CV_HAL_ERROR_OK;
}

template <typename fptype> ساكن مضمن صحيح
lapack_gemm_c(ثابت fptype *src1, size_t src1_step, ثابت fptype *src2, size_t src2_step, fptype alpha,
            ثابت fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, صحيح a_m, صحيح a_n, صحيح d_n, صحيح flags)
{
    صحيح ldsrc1 = (صحيح)(src1_step / حجم(std::complex<fptype>));
    صحيح ldsrc2 = (صحيح)(src2_step / حجم(std::complex<fptype>));
    صحيح ldsrc3 = (صحيح)(src3_step / حجم(std::complex<fptype>));
    صحيح lddst = (صحيح)(dst_step / حجم(std::complex<fptype>));
    صحيح c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    إذا(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        إذا(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        وإلا
        {
            d_m = a_m;
        }
    }
    وإلا
    {
        transB = CblasNoTrans;
        إذا(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        وإلا
        {
            d_m = a_m;
        }
    }

    إذا(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    وإلا
    {
        c_m = d_m;
        c_n = d_n;
    }

    إذا(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    وإلا
    {
        transA = CblasNoTrans;
    }

    إذا(src3 != dst && beta != 0.0 && src3_step != 0) {
        إذا(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        وإلا
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    وإلا إذا (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    وإلا إذا(src3_step == 0 && beta != 0.0)
        set_نيلاي_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    صحيح M = a_m, N = d_n, K = a_n;
    إذا(typeid(fptype) == typeid(عائم)) {
        auto src1_cast = (std::complex<عائم>*)(src1);
        auto src2_cast = (std::complex<عائم>*)(src2);
        auto dst_cast = (std::complex<عائم>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<عائم>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<عائم>*)&cBeta, dst_cast, ldc);
    }
    وإلا إذا(typeid(fptype) == typeid(مزدوج)) {
        auto src1_cast = (std::complex<مزدوج>*)(src1);
        auto src2_cast = (std::complex<مزدوج>*)(src2);
        auto dst_cast = (std::complex<مزدوج>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<مزدوج>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<مزدوج>*)&cBeta, dst_cast, ldc);
    }
#وإلا
    إذا(typeid(fptype) == typeid(عائم))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (عائم*)reinterpret_cast<fptype(&)[2]>(cAlpha), (عائم*)src1, ldsrc1, (عائم*)src2, ldsrc2, (عائم*)reinterpret_cast<fptype(&)[2]>(cBeta), (عائم*)dst, lddst);
    وإلا إذا(typeid(fptype) == typeid(مزدوج))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (مزدوج*)reinterpret_cast<fptype(&)[2]>(cAlpha), (مزدوج*)src1, ldsrc1, (مزدوج*)src2, ldsrc2, (مزدوج*)reinterpret_cast<fptype(&)[2]>(cBeta), (مزدوج*)dst, lddst);
#endif

    أرجع CV_HAL_ERROR_OK;
}
صحيح lapack_LU32f(عائم* a, size_t a_step, صحيح m, عائم* b, size_t b_step, صحيح n, صحيح* info)
{
    إذا(m < HAL_LU_SMALL_MATRIX_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_LU(a, a_step, m, b, b_step, n, info);
}

صحيح lapack_LU64f(مزدوج* a, size_t a_step, صحيح m, مزدوج* b, size_t b_step, صحيح n, صحيح* info)
{
    إذا(m < HAL_LU_SMALL_MATRIX_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_LU(a, a_step, m, b, b_step, n, info);
}

صحيح lapack_Cholesky32f(عائم* a, size_t a_step, صحيح m, عائم* b, size_t b_step, صحيح n, منطقي *info)
{
    إذا(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

صحيح lapack_Cholesky64f(مزدوج* a, size_t a_step, صحيح m, مزدوج* b, size_t b_step, صحيح n, منطقي *info)
{
    إذا(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

صحيح lapack_SVD32f(عائم* a, size_t a_step, عائم *w, عائم* u, size_t u_step, عائم* vt, size_t v_step, صحيح m, صحيح n, صحيح flags)
{
    إذا(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    صحيح info = 0;
    أرجع lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

صحيح lapack_SVD64f(مزدوج* a, size_t a_step, مزدوج *w, مزدوج* u, size_t u_step, مزدوج* vt, size_t v_step, صحيح m, صحيح n, صحيح flags)
{
    إذا(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    صحيح info = 0;
    أرجع lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

صحيح lapack_QR32f(عائم* src1, size_t src1_step, صحيح m, صحيح n, صحيح k, عائم* src2, size_t src2_step, عائم* dst, صحيح* info)
{
    إذا (m < HAL_QR_SMALL_MATRIX_THRESH)
      أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

صحيح lapack_QR64f(مزدوج* src1, size_t src1_step, صحيح m, صحيح n, صحيح k, مزدوج* src2, size_t src2_step, مزدوج* dst, صحيح* info)
{
    إذا (m < HAL_QR_SMALL_MATRIX_THRESH)
      أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

صحيح lapack_gemm32f(ثابت عائم *src1, size_t src1_step, ثابت عائم *src2, size_t src2_step, عائم alpha,
                   ثابت عائم *src3, size_t src3_step, عائم beta, عائم *dst, size_t dst_step, صحيح m, صحيح n, صحيح k, صحيح flags)
{
    إذا(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

صحيح lapack_gemm64f(ثابت مزدوج *src1, size_t src1_step, ثابت مزدوج *src2, size_t src2_step, مزدوج alpha,
                   ثابت مزدوج *src3, size_t src3_step, مزدوج beta, مزدوج *dst, size_t dst_step, صحيح m, صحيح n, صحيح k, صحيح flags)
{
    إذا(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

صحيح lapack_gemm32fc(ثابت عائم *src1, size_t src1_step, ثابت عائم *src2, size_t src2_step, عائم alpha,
                   ثابت عائم *src3, size_t src3_step, عائم beta, عائم *dst, size_t dst_step, صحيح m, صحيح n, صحيح k, صحيح flags)
{
    إذا(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
صحيح lapack_gemm64fc(ثابت مزدوج *src1, size_t src1_step, ثابت مزدوج *src2, size_t src2_step, مزدوج alpha,
                   ثابت مزدوج *src3, size_t src3_step, مزدوج beta, مزدوج *dst, size_t dst_step, صحيح m, صحيح n, صحيح k, صحيح flags)
{
    إذا(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#إذا defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
