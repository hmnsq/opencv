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

#شامل "precomp.hpp"
#شامل "hal_internal.hpp"

#ifdef HAVE_LAPACK

#شامل "opencv_lapack.h"

#شامل <cmath>
#شامل <algorithm>
#شامل <typeinfo>
#شامل <limits>
#شامل <complex>
#شامل <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#اگر defined(__clang__) && defined(__has_feature)
#اگر __has_feature(memory_sanitizer)
#شامل <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) انجامبده { } وقتی(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#اگر defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> ایستا درونخطی تهی
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    برای(size_t i = 0; i < m - 1; i++)
        برای(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> ایستا درونخطی تهی
transpose(ثابت fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    برای(size_t i = 0; i < m; i++)
        برای(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> ایستا درونخطی تهی
copy_matrix(ثابت fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    برای(size_t i = 0; i < m; i++)
        برای(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> ایستا درونخطی تهی
set_مقدار_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    برای(size_t i = 0; i < m; i++)
        برای(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> ایستا درونخطی صحیح
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, صحیح m, fptype* b, size_t b_step, صحیح n, صحیح* info)
{
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / اندازه(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#وگرنه
    cv::AutoBuffer<صحیح> piv_buff(m);
    صحیح lda = (صحیح)(a_step / اندازه(fptype));
    صحیح _m = m, _n = n;
    صحیح* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    اگر(b)
    {
        اگر(n == 1 && b_step == اندازه(fptype))
        {
            اگر(typeid(fptype) == typeid(اعشاری))
                sgesv_(&_m, &_n, (اعشاری*)a, &lda, piv, (اعشاری*)b, &_m, _info);
            وگرنه اگر(typeid(fptype) == typeid(دوگانه))
                dgesv_(&_m, &_n, (دوگانه*)a, &lda, piv, (دوگانه*)b, &_m, _info);
        }
        وگرنه
        {
            صحیح ldb = (صحیح)(b_step / اندازه(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            اگر(typeid(fptype) == typeid(اعشاری))
                sgesv_(&_m, &_n, (اعشاری*)a, &lda, piv, (اعشاری*)&tmpB[0], &_m, _info);
            وگرنه اگر(typeid(fptype) == typeid(دوگانه))
                dgesv_(&_m, &_n, (دوگانه*)a, &lda, piv, (دوگانه*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    وگرنه
    {
        اگر(typeid(fptype) == typeid(اعشاری))
            sgetrf_(&_m, &_m, (اعشاری*)a, &lda, piv, _info);
        وگرنه اگر(typeid(fptype) == typeid(دوگانه))
            dgetrf_(&_m, &_m, (دوگانه*)a, &lda, piv, _info);
    }

#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحیح>(_info[0]);
#endif
    صحیح retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    صحیح sign = 0;
    اگر(*info == 0)
    {
        برای(صحیح i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    وگرنه
        *info = 0; //in opencv LU function zero means error

    بازگشت retcode;
}

template <typename fptype> ایستا درونخطی صحیح
lapack_Cholesky(fptype* a, size_t a_step, صحیح m, fptype* b, size_t b_step, صحیح n, منطقی* info)
{
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / اندازه(fptype));
#وگرنه
    صحیح _m = m, _n = n;
    صحیح lapackStatus = 0;
    صحیح lda = (صحیح)(a_step / اندازه(fptype));
#endif
    نویسه L[] = {'L', '\0'};

    اگر(b)
    {
        اگر(n == 1 && b_step == اندازه(fptype))
        {
            اگر(typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (اعشاری*)a, &lda, (اعشاری*)b, &_m, &lapackStatus);
            وگرنه اگر(typeid(fptype) == typeid(دوگانه))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (دوگانه*)a, &lda, (دوگانه*)b, &_m, &lapackStatus);
        }
        وگرنه
        {
            صحیح ldb = (صحیح)(b_step / اندازه(fptype));
            fptype* tmpB = جدید fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            اگر(typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (اعشاری*)a, &lda, (اعشاری*)tmpB, &_m, &lapackStatus);
            وگرنه اگر(typeid(fptype) == typeid(دوگانه))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (دوگانه*)a, &lda, (دوگانه*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    وگرنه
    {
        اگر(typeid(fptype) == typeid(اعشاری))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (اعشاری*)a, &lda, &lapackStatus);
        وگرنه اگر(typeid(fptype) == typeid(دوگانه))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (دوگانه*)a, &lda, &lapackStatus);
    }

    اگر(lapackStatus == 0) *info = درست;
    وگرنه *info = نادرست; //in opencv Cholesky function false means error

    بازگشت lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> ایستا درونخطی صحیح
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, صحیح m, صحیح n, صحیح flags, صحیح* info)
{
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / اندازه(fptype));
    long ldv = (long)(v_step / اندازه(fptype));
    long ldu = (long)(u_step / اندازه(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#وگرنه
    صحیح _m = m, _n = n;
    صحیح* _info = info;
    صحیح lda = (صحیح)(a_step / اندازه(fptype));
    صحیح ldv = (صحیح)(v_step / اندازه(fptype));
    صحیح ldu = (صحیح)(u_step / اندازه(fptype));
    صحیح lwork = -1;
    cv::AutoBuffer<صحیح> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    نویسه mode[] = { ' ', '\0'};
    اگر(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    وگرنه اگر((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    وگرنه اگر((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    وگرنه اگر(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    اگر((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    اگر(typeid(fptype) == typeid(اعشاری))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (اعشاری*)a, &lda, (اعشاری*)w, (اعشاری*)u, &ldu, (اعشاری*)vt, &ldv, (اعشاری*)&work1, &lwork, iworkBuf, _info);
    وگرنه اگر(typeid(fptype) == typeid(دوگانه))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (دوگانه*)a, &lda, (دوگانه*)w, (دوگانه*)u, &ldu, (دوگانه*)vt, &ldv, (دوگانه*)&work1, &lwork, iworkBuf, _info);

    اگر(*info < 0)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (صحیح)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), اندازه(fptype) * (lwork + 1));

    اگر(typeid(fptype) == typeid(اعشاری))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (اعشاری*)a, &lda, (اعشاری*)w, (اعشاری*)u, &ldu, (اعشاری*)vt, &ldv, (اعشاری*)&buffer[0], &lwork, iworkBuf, _info);
    وگرنه اگر(typeid(fptype) == typeid(دوگانه))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (دوگانه*)a, &lda, (دوگانه*)w, (دوگانه*)u, &ldu, (دوگانه*)vt, &ldv, (دوگانه*)&buffer[0], &lwork, iworkBuf, _info);

#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحیح>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    اگر (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    اگر (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    اگر (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, اندازه(fptype) * std::min(m, n));

    اگر(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    اگر((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        برای(صحیح i = 0; i < m; i++)
            برای(صحیح j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    اگر(*info < 0)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت CV_HAL_ERROR_OK;
}

template <typename fptype> ایستا درونخطی صحیح
lapack_QR(fptype* a, size_t a_step, صحیح m, صحیح n, صحیح k, fptype* b, size_t b_step, fptype* dst, صحیح* info)
{
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / اندازه(fptype));
    long lwork = -1;
    long ldtmpA;
#وگرنه
    صحیح _m = m, _n = n, _k = k;
    صحیح* _info = info;
    صحیح lda = (صحیح)(a_step / اندازه(fptype));
    صحیح lwork = -1;
    صحیح ldtmpA;
#endif

    نویسه mode[] = { 'N', '\0' };
    اگر(m < n)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    اگر (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    وگرنه
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    اگر (b)
    {
        اگر (k == 1 && b_step == اندازه(fptype))
        {
            اگر (typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)b, &_m, (اعشاری*)&work1, &lwork, _info);
            وگرنه اگر (typeid(fptype) == typeid(دوگانه))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (دوگانه*)tmpA, &ldtmpA, (دوگانه*)b, &_m, (دوگانه*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, اندازه(صحیح));
            اگر (*info < 0)
                بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            اگر (typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)b, &_m, (اعشاری*)buffer, &lwork, _info);
            وگرنه اگر (typeid(fptype) == typeid(دوگانه))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (دوگانه*)tmpA, &ldtmpA, (دوگانه*)b, &_m, (دوگانه*)buffer, &lwork, _info);
        }
        وگرنه
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            صحیح ldb = (صحیح)(b_step / اندازه(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            اگر (typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)tmpB, &_m, (اعشاری*)&work1, &lwork, _info);
            وگرنه اگر (typeid(fptype) == typeid(دوگانه))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (دوگانه*)tmpA, &ldtmpA, (دوگانه*)tmpB, &_m, (دوگانه*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, اندازه(صحیح));
            اگر (*info < 0)
                بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            اگر (typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)tmpB, &_m, (اعشاری*)buffer, &lwork, _info);
            وگرنه اگر (typeid(fptype) == typeid(دوگانه))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (دوگانه*)tmpA, &ldtmpA, (دوگانه*)tmpB, &_m, (دوگانه*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    وگرنه
    {
        اگر (typeid(fptype) == typeid(اعشاری))
            sgeqrf_(&_m, &_n, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)dst, (اعشاری*)&work1, &lwork, _info);
        وگرنه اگر (typeid(fptype) == typeid(دوگانه))
            dgeqrf_(&_m, &_n, (دوگانه*)tmpA, &ldtmpA, (دوگانه*)dst, (دوگانه*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, اندازه(صحیح));
        اگر (*info < 0)
            بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        اگر (typeid(fptype) == typeid(اعشاری))
            sgeqrf_(&_m, &_n, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)dst, (اعشاری*)buffer, &lwork, _info);
        وگرنه اگر (typeid(fptype) == typeid(دوگانه))
            dgeqrf_(&_m, &_n, (دوگانه*)tmpA, &ldtmpA, (دوگانه*)dst, (دوگانه*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, اندازه(صحیح));
    اگر (m == n)
        transpose_square_inplace(a, lda, m);
    وگرنه
        transpose(tmpA, m, a, lda, n, m);

#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحیح>(_info[0]);
#endif

    اگر (*info != 0)
        *info = 0;
    وگرنه
        *info = 1;

    بازگشت CV_HAL_ERROR_OK;
}

template <typename fptype> ایستا درونخطی صحیح
lapack_gemm(ثابت fptype *src1, size_t src1_step, ثابت fptype *src2, size_t src2_step, fptype alpha,
            ثابت fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, صحیح a_m, صحیح a_n, صحیح d_n, صحیح flags)
{
    صحیح ldsrc1 = (صحیح)(src1_step / اندازه(fptype));
    صحیح ldsrc2 = (صحیح)(src2_step / اندازه(fptype));
    صحیح ldsrc3 = (صحیح)(src3_step / اندازه(fptype));
    صحیح lddst = (صحیح)(dst_step / اندازه(fptype));
    صحیح c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    اگر(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        اگر(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        وگرنه
        {
            d_m = a_m;
        }
    }
    وگرنه
    {
        transB = CblasNoTrans;
        اگر(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        وگرنه
        {
            d_m = a_m;
        }
    }

    اگر(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    وگرنه
    {
        c_m = d_m;
        c_n = d_n;
    }

    اگر(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    وگرنه
    {
        transA = CblasNoTrans;
    }

    اگر(src3 != dst && beta != 0.0 && src3_step != 0) {
        اگر(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        وگرنه
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    وگرنه اگر (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    وگرنه اگر(src3_step == 0 && beta != 0.0)
        set_مقدار_2(dst, lddst, (fptype)0.0, d_m, d_n);

    اگر(typeid(fptype) == typeid(اعشاری))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (اعشاری)alpha, (اعشاری*)src1, ldsrc1, (اعشاری*)src2, ldsrc2, (اعشاری)beta, (اعشاری*)dst, lddst);
    وگرنه اگر(typeid(fptype) == typeid(دوگانه))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (دوگانه)alpha, (دوگانه*)src1, ldsrc1, (دوگانه*)src2, ldsrc2, (دوگانه)beta, (دوگانه*)dst, lddst);

    بازگشت CV_HAL_ERROR_OK;
}

template <typename fptype> ایستا درونخطی صحیح
lapack_gemm_c(ثابت fptype *src1, size_t src1_step, ثابت fptype *src2, size_t src2_step, fptype alpha,
            ثابت fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, صحیح a_m, صحیح a_n, صحیح d_n, صحیح flags)
{
    صحیح ldsrc1 = (صحیح)(src1_step / اندازه(std::complex<fptype>));
    صحیح ldsrc2 = (صحیح)(src2_step / اندازه(std::complex<fptype>));
    صحیح ldsrc3 = (صحیح)(src3_step / اندازه(std::complex<fptype>));
    صحیح lddst = (صحیح)(dst_step / اندازه(std::complex<fptype>));
    صحیح c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    اگر(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        اگر(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        وگرنه
        {
            d_m = a_m;
        }
    }
    وگرنه
    {
        transB = CblasNoTrans;
        اگر(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        وگرنه
        {
            d_m = a_m;
        }
    }

    اگر(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    وگرنه
    {
        c_m = d_m;
        c_n = d_n;
    }

    اگر(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    وگرنه
    {
        transA = CblasNoTrans;
    }

    اگر(src3 != dst && beta != 0.0 && src3_step != 0) {
        اگر(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        وگرنه
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    وگرنه اگر (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    وگرنه اگر(src3_step == 0 && beta != 0.0)
        set_مقدار_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    صحیح M = a_m, N = d_n, K = a_n;
    اگر(typeid(fptype) == typeid(اعشاری)) {
        auto src1_cast = (std::complex<اعشاری>*)(src1);
        auto src2_cast = (std::complex<اعشاری>*)(src2);
        auto dst_cast = (std::complex<اعشاری>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<اعشاری>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<اعشاری>*)&cBeta, dst_cast, ldc);
    }
    وگرنه اگر(typeid(fptype) == typeid(دوگانه)) {
        auto src1_cast = (std::complex<دوگانه>*)(src1);
        auto src2_cast = (std::complex<دوگانه>*)(src2);
        auto dst_cast = (std::complex<دوگانه>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<دوگانه>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<دوگانه>*)&cBeta, dst_cast, ldc);
    }
#وگرنه
    اگر(typeid(fptype) == typeid(اعشاری))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (اعشاری*)reinterpret_cast<fptype(&)[2]>(cAlpha), (اعشاری*)src1, ldsrc1, (اعشاری*)src2, ldsrc2, (اعشاری*)reinterpret_cast<fptype(&)[2]>(cBeta), (اعشاری*)dst, lddst);
    وگرنه اگر(typeid(fptype) == typeid(دوگانه))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (دوگانه*)reinterpret_cast<fptype(&)[2]>(cAlpha), (دوگانه*)src1, ldsrc1, (دوگانه*)src2, ldsrc2, (دوگانه*)reinterpret_cast<fptype(&)[2]>(cBeta), (دوگانه*)dst, lddst);
#endif

    بازگشت CV_HAL_ERROR_OK;
}
صحیح lapack_LU32f(اعشاری* a, size_t a_step, صحیح m, اعشاری* b, size_t b_step, صحیح n, صحیح* info)
{
    اگر(m < HAL_LU_SMALL_MATRIX_THRESH)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_LU(a, a_step, m, b, b_step, n, info);
}

صحیح lapack_LU64f(دوگانه* a, size_t a_step, صحیح m, دوگانه* b, size_t b_step, صحیح n, صحیح* info)
{
    اگر(m < HAL_LU_SMALL_MATRIX_THRESH)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_LU(a, a_step, m, b, b_step, n, info);
}

صحیح lapack_Cholesky32f(اعشاری* a, size_t a_step, صحیح m, اعشاری* b, size_t b_step, صحیح n, منطقی *info)
{
    اگر(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

صحیح lapack_Cholesky64f(دوگانه* a, size_t a_step, صحیح m, دوگانه* b, size_t b_step, صحیح n, منطقی *info)
{
    اگر(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

صحیح lapack_SVD32f(اعشاری* a, size_t a_step, اعشاری *w, اعشاری* u, size_t u_step, اعشاری* vt, size_t v_step, صحیح m, صحیح n, صحیح flags)
{
    اگر(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    صحیح info = 0;
    بازگشت lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

صحیح lapack_SVD64f(دوگانه* a, size_t a_step, دوگانه *w, دوگانه* u, size_t u_step, دوگانه* vt, size_t v_step, صحیح m, صحیح n, صحیح flags)
{
    اگر(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    صحیح info = 0;
    بازگشت lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

صحیح lapack_QR32f(اعشاری* src1, size_t src1_step, صحیح m, صحیح n, صحیح k, اعشاری* src2, size_t src2_step, اعشاری* dst, صحیح* info)
{
    اگر (m < HAL_QR_SMALL_MATRIX_THRESH)
      بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

صحیح lapack_QR64f(دوگانه* src1, size_t src1_step, صحیح m, صحیح n, صحیح k, دوگانه* src2, size_t src2_step, دوگانه* dst, صحیح* info)
{
    اگر (m < HAL_QR_SMALL_MATRIX_THRESH)
      بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

صحیح lapack_gemm32f(ثابت اعشاری *src1, size_t src1_step, ثابت اعشاری *src2, size_t src2_step, اعشاری alpha,
                   ثابت اعشاری *src3, size_t src3_step, اعشاری beta, اعشاری *dst, size_t dst_step, صحیح m, صحیح n, صحیح k, صحیح flags)
{
    اگر(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

صحیح lapack_gemm64f(ثابت دوگانه *src1, size_t src1_step, ثابت دوگانه *src2, size_t src2_step, دوگانه alpha,
                   ثابت دوگانه *src3, size_t src3_step, دوگانه beta, دوگانه *dst, size_t dst_step, صحیح m, صحیح n, صحیح k, صحیح flags)
{
    اگر(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

صحیح lapack_gemm32fc(ثابت اعشاری *src1, size_t src1_step, ثابت اعشاری *src2, size_t src2_step, اعشاری alpha,
                   ثابت اعشاری *src3, size_t src3_step, اعشاری beta, اعشاری *dst, size_t dst_step, صحیح m, صحیح n, صحیح k, صحیح flags)
{
    اگر(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
صحیح lapack_gemm64fc(ثابت دوگانه *src1, size_t src1_step, ثابت دوگانه *src2, size_t src2_step, دوگانه alpha,
                   ثابت دوگانه *src3, size_t src3_step, دوگانه beta, دوگانه *dst, size_t dst_step, صحیح m, صحیح n, صحیح k, صحیح flags)
{
    اگر(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        بازگشت CV_HAL_ERROR_NOT_IMPLEMENTED;
    بازگشت lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#اگر defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
