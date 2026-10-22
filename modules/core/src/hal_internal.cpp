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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "hal_internal.hpp"

#ifdef HAVE_LAPACK

#அந்தர்பாவ "opencv_lapack.h"

#அந்தர்பாவ <cmath>
#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <typeinfo>
#அந்தர்பாவ <limits>
#அந்தர்பாவ <complex>
#அந்தர்பாவ <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#எனில் defined(__clang__) && defined(__has_feature)
#எனில் __has_feature(memory_sanitizer)
#அந்தர்பாவ <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) செய் { } போது(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#எனில் defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> நிலையான உள்வரி வெறுமை
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    க்கு(size_t i = 0; i < m - 1; i++)
        க்கு(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> நிலையான உள்வரி வெறுமை
transpose(மாறிலி fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    க்கு(size_t i = 0; i < m; i++)
        க்கு(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> நிலையான உள்வரி வெறுமை
copy_matrix(மாறிலி fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    க்கு(size_t i = 0; i < m; i++)
        க்கு(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> நிலையான உள்வரி வெறுமை
set_மதிப்பு_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    க்கு(size_t i = 0; i < m; i++)
        க்கு(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> நிலையான உள்வரி முழு
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, முழு m, fptype* b, size_t b_step, முழு n, முழு* info)
{
#எனில் defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / அளவு(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#இல்லையெனில்
    cv::AutoBuffer<முழு> piv_buff(m);
    முழு lda = (முழு)(a_step / அளவு(fptype));
    முழு _m = m, _n = n;
    முழு* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    எனில்(b)
    {
        எனில்(n == 1 && b_step == அளவு(fptype))
        {
            எனில்(typeid(fptype) == typeid(தசம))
                sgesv_(&_m, &_n, (தசம*)a, &lda, piv, (தசம*)b, &_m, _info);
            இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
                dgesv_(&_m, &_n, (இரட்டை*)a, &lda, piv, (இரட்டை*)b, &_m, _info);
        }
        இல்லையெனில்
        {
            முழு ldb = (முழு)(b_step / அளவு(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            எனில்(typeid(fptype) == typeid(தசம))
                sgesv_(&_m, &_n, (தசம*)a, &lda, piv, (தசம*)&tmpB[0], &_m, _info);
            இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
                dgesv_(&_m, &_n, (இரட்டை*)a, &lda, piv, (இரட்டை*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    இல்லையெனில்
    {
        எனில்(typeid(fptype) == typeid(தசம))
            sgetrf_(&_m, &_m, (தசம*)a, &lda, piv, _info);
        இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
            dgetrf_(&_m, &_m, (இரட்டை*)a, &lda, piv, _info);
    }

#எனில் defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<முழு>(_info[0]);
#endif
    முழு retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    முழு sign = 0;
    எனில்(*info == 0)
    {
        க்கு(முழு i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    இல்லையெனில்
        *info = 0; //in opencv LU function zero means error

    திரும்பு retcode;
}

template <typename fptype> நிலையான உள்வரி முழு
lapack_Cholesky(fptype* a, size_t a_step, முழு m, fptype* b, size_t b_step, முழு n, தருக்க* info)
{
#எனில் defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / அளவு(fptype));
#இல்லையெனில்
    முழு _m = m, _n = n;
    முழு lapackStatus = 0;
    முழு lda = (முழு)(a_step / அளவு(fptype));
#endif
    எழுத்து L[] = {'L', '\0'};

    எனில்(b)
    {
        எனில்(n == 1 && b_step == அளவு(fptype))
        {
            எனில்(typeid(fptype) == typeid(தசம))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (தசம*)a, &lda, (தசம*)b, &_m, &lapackStatus);
            இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (இரட்டை*)a, &lda, (இரட்டை*)b, &_m, &lapackStatus);
        }
        இல்லையெனில்
        {
            முழு ldb = (முழு)(b_step / அளவு(fptype));
            fptype* tmpB = புதிய fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            எனில்(typeid(fptype) == typeid(தசம))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (தசம*)a, &lda, (தசம*)tmpB, &_m, &lapackStatus);
            இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (இரட்டை*)a, &lda, (இரட்டை*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    இல்லையெனில்
    {
        எனில்(typeid(fptype) == typeid(தசம))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (தசம*)a, &lda, &lapackStatus);
        இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (இரட்டை*)a, &lda, &lapackStatus);
    }

    எனில்(lapackStatus == 0) *info = உண்மை;
    இல்லையெனில் *info = பொய்; //in opencv Cholesky function false means error

    திரும்பு lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> நிலையான உள்வரி முழு
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, முழு m, முழு n, முழு flags, முழு* info)
{
#எனில் defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / அளவு(fptype));
    long ldv = (long)(v_step / அளவு(fptype));
    long ldu = (long)(u_step / அளவு(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#இல்லையெனில்
    முழு _m = m, _n = n;
    முழு* _info = info;
    முழு lda = (முழு)(a_step / அளவு(fptype));
    முழு ldv = (முழு)(v_step / அளவு(fptype));
    முழு ldu = (முழு)(u_step / அளவு(fptype));
    முழு lwork = -1;
    cv::AutoBuffer<முழு> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    எழுத்து mode[] = { ' ', '\0'};
    எனில்(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    இல்லையெனில் எனில்((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    இல்லையெனில் எனில்((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    இல்லையெனில் எனில்(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    எனில்((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    எனில்(typeid(fptype) == typeid(தசம))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (தசம*)a, &lda, (தசம*)w, (தசம*)u, &ldu, (தசம*)vt, &ldv, (தசம*)&work1, &lwork, iworkBuf, _info);
    இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (இரட்டை*)a, &lda, (இரட்டை*)w, (இரட்டை*)u, &ldu, (இரட்டை*)vt, &ldv, (இரட்டை*)&work1, &lwork, iworkBuf, _info);

    எனில்(*info < 0)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (முழு)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), அளவு(fptype) * (lwork + 1));

    எனில்(typeid(fptype) == typeid(தசம))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (தசம*)a, &lda, (தசம*)w, (தசம*)u, &ldu, (தசம*)vt, &ldv, (தசம*)&buffer[0], &lwork, iworkBuf, _info);
    இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (இரட்டை*)a, &lda, (இரட்டை*)w, (இரட்டை*)u, &ldu, (இரட்டை*)vt, &ldv, (இரட்டை*)&buffer[0], &lwork, iworkBuf, _info);

#எனில் defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<முழு>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    எனில் (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    எனில் (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    எனில் (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, அளவு(fptype) * std::min(m, n));

    எனில்(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    எனில்((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        க்கு(முழு i = 0; i < m; i++)
            க்கு(முழு j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    எனில்(*info < 0)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு CV_HAL_ERROR_OK;
}

template <typename fptype> நிலையான உள்வரி முழு
lapack_QR(fptype* a, size_t a_step, முழு m, முழு n, முழு k, fptype* b, size_t b_step, fptype* dst, முழு* info)
{
#எனில் defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / அளவு(fptype));
    long lwork = -1;
    long ldtmpA;
#இல்லையெனில்
    முழு _m = m, _n = n, _k = k;
    முழு* _info = info;
    முழு lda = (முழு)(a_step / அளவு(fptype));
    முழு lwork = -1;
    முழு ldtmpA;
#endif

    எழுத்து mode[] = { 'N', '\0' };
    எனில்(m < n)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    எனில் (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    இல்லையெனில்
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    எனில் (b)
    {
        எனில் (k == 1 && b_step == அளவு(fptype))
        {
            எனில் (typeid(fptype) == typeid(தசம))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (தசம*)tmpA, &ldtmpA, (தசம*)b, &_m, (தசம*)&work1, &lwork, _info);
            இல்லையெனில் எனில் (typeid(fptype) == typeid(இரட்டை))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (இரட்டை*)tmpA, &ldtmpA, (இரட்டை*)b, &_m, (இரட்டை*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, அளவு(முழு));
            எனில் (*info < 0)
                திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            எனில் (typeid(fptype) == typeid(தசம))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (தசம*)tmpA, &ldtmpA, (தசம*)b, &_m, (தசம*)buffer, &lwork, _info);
            இல்லையெனில் எனில் (typeid(fptype) == typeid(இரட்டை))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (இரட்டை*)tmpA, &ldtmpA, (இரட்டை*)b, &_m, (இரட்டை*)buffer, &lwork, _info);
        }
        இல்லையெனில்
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            முழு ldb = (முழு)(b_step / அளவு(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            எனில் (typeid(fptype) == typeid(தசம))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (தசம*)tmpA, &ldtmpA, (தசம*)tmpB, &_m, (தசம*)&work1, &lwork, _info);
            இல்லையெனில் எனில் (typeid(fptype) == typeid(இரட்டை))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (இரட்டை*)tmpA, &ldtmpA, (இரட்டை*)tmpB, &_m, (இரட்டை*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, அளவு(முழு));
            எனில் (*info < 0)
                திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            எனில் (typeid(fptype) == typeid(தசம))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (தசம*)tmpA, &ldtmpA, (தசம*)tmpB, &_m, (தசம*)buffer, &lwork, _info);
            இல்லையெனில் எனில் (typeid(fptype) == typeid(இரட்டை))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (இரட்டை*)tmpA, &ldtmpA, (இரட்டை*)tmpB, &_m, (இரட்டை*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    இல்லையெனில்
    {
        எனில் (typeid(fptype) == typeid(தசம))
            sgeqrf_(&_m, &_n, (தசம*)tmpA, &ldtmpA, (தசம*)dst, (தசம*)&work1, &lwork, _info);
        இல்லையெனில் எனில் (typeid(fptype) == typeid(இரட்டை))
            dgeqrf_(&_m, &_n, (இரட்டை*)tmpA, &ldtmpA, (இரட்டை*)dst, (இரட்டை*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, அளவு(முழு));
        எனில் (*info < 0)
            திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        எனில் (typeid(fptype) == typeid(தசம))
            sgeqrf_(&_m, &_n, (தசம*)tmpA, &ldtmpA, (தசம*)dst, (தசம*)buffer, &lwork, _info);
        இல்லையெனில் எனில் (typeid(fptype) == typeid(இரட்டை))
            dgeqrf_(&_m, &_n, (இரட்டை*)tmpA, &ldtmpA, (இரட்டை*)dst, (இரட்டை*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, அளவு(முழு));
    எனில் (m == n)
        transpose_square_inplace(a, lda, m);
    இல்லையெனில்
        transpose(tmpA, m, a, lda, n, m);

#எனில் defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<முழு>(_info[0]);
#endif

    எனில் (*info != 0)
        *info = 0;
    இல்லையெனில்
        *info = 1;

    திரும்பு CV_HAL_ERROR_OK;
}

template <typename fptype> நிலையான உள்வரி முழு
lapack_gemm(மாறிலி fptype *src1, size_t src1_step, மாறிலி fptype *src2, size_t src2_step, fptype alpha,
            மாறிலி fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, முழு a_m, முழு a_n, முழு d_n, முழு flags)
{
    முழு ldsrc1 = (முழு)(src1_step / அளவு(fptype));
    முழு ldsrc2 = (முழு)(src2_step / அளவு(fptype));
    முழு ldsrc3 = (முழு)(src3_step / அளவு(fptype));
    முழு lddst = (முழு)(dst_step / அளவு(fptype));
    முழு c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    எனில்(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        எனில்(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        இல்லையெனில்
        {
            d_m = a_m;
        }
    }
    இல்லையெனில்
    {
        transB = CblasNoTrans;
        எனில்(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        இல்லையெனில்
        {
            d_m = a_m;
        }
    }

    எனில்(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    இல்லையெனில்
    {
        c_m = d_m;
        c_n = d_n;
    }

    எனில்(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    இல்லையெனில்
    {
        transA = CblasNoTrans;
    }

    எனில்(src3 != dst && beta != 0.0 && src3_step != 0) {
        எனில்(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        இல்லையெனில்
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    இல்லையெனில் எனில் (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    இல்லையெனில் எனில்(src3_step == 0 && beta != 0.0)
        set_மதிப்பு_2(dst, lddst, (fptype)0.0, d_m, d_n);

    எனில்(typeid(fptype) == typeid(தசம))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (தசம)alpha, (தசம*)src1, ldsrc1, (தசம*)src2, ldsrc2, (தசம)beta, (தசம*)dst, lddst);
    இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (இரட்டை)alpha, (இரட்டை*)src1, ldsrc1, (இரட்டை*)src2, ldsrc2, (இரட்டை)beta, (இரட்டை*)dst, lddst);

    திரும்பு CV_HAL_ERROR_OK;
}

template <typename fptype> நிலையான உள்வரி முழு
lapack_gemm_c(மாறிலி fptype *src1, size_t src1_step, மாறிலி fptype *src2, size_t src2_step, fptype alpha,
            மாறிலி fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, முழு a_m, முழு a_n, முழு d_n, முழு flags)
{
    முழு ldsrc1 = (முழு)(src1_step / அளவு(std::complex<fptype>));
    முழு ldsrc2 = (முழு)(src2_step / அளவு(std::complex<fptype>));
    முழு ldsrc3 = (முழு)(src3_step / அளவு(std::complex<fptype>));
    முழு lddst = (முழு)(dst_step / அளவு(std::complex<fptype>));
    முழு c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    எனில்(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        எனில்(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        இல்லையெனில்
        {
            d_m = a_m;
        }
    }
    இல்லையெனில்
    {
        transB = CblasNoTrans;
        எனில்(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        இல்லையெனில்
        {
            d_m = a_m;
        }
    }

    எனில்(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    இல்லையெனில்
    {
        c_m = d_m;
        c_n = d_n;
    }

    எனில்(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    இல்லையெனில்
    {
        transA = CblasNoTrans;
    }

    எனில்(src3 != dst && beta != 0.0 && src3_step != 0) {
        எனில்(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        இல்லையெனில்
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    இல்லையெனில் எனில் (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    இல்லையெனில் எனில்(src3_step == 0 && beta != 0.0)
        set_மதிப்பு_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#எனில் defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    முழு M = a_m, N = d_n, K = a_n;
    எனில்(typeid(fptype) == typeid(தசம)) {
        auto src1_cast = (std::complex<தசம>*)(src1);
        auto src2_cast = (std::complex<தசம>*)(src2);
        auto dst_cast = (std::complex<தசம>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<தசம>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<தசம>*)&cBeta, dst_cast, ldc);
    }
    இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை)) {
        auto src1_cast = (std::complex<இரட்டை>*)(src1);
        auto src2_cast = (std::complex<இரட்டை>*)(src2);
        auto dst_cast = (std::complex<இரட்டை>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<இரட்டை>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<இரட்டை>*)&cBeta, dst_cast, ldc);
    }
#இல்லையெனில்
    எனில்(typeid(fptype) == typeid(தசம))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (தசம*)reinterpret_cast<fptype(&)[2]>(cAlpha), (தசம*)src1, ldsrc1, (தசம*)src2, ldsrc2, (தசம*)reinterpret_cast<fptype(&)[2]>(cBeta), (தசம*)dst, lddst);
    இல்லையெனில் எனில்(typeid(fptype) == typeid(இரட்டை))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (இரட்டை*)reinterpret_cast<fptype(&)[2]>(cAlpha), (இரட்டை*)src1, ldsrc1, (இரட்டை*)src2, ldsrc2, (இரட்டை*)reinterpret_cast<fptype(&)[2]>(cBeta), (இரட்டை*)dst, lddst);
#endif

    திரும்பு CV_HAL_ERROR_OK;
}
முழு lapack_LU32f(தசம* a, size_t a_step, முழு m, தசம* b, size_t b_step, முழு n, முழு* info)
{
    எனில்(m < HAL_LU_SMALL_MATRIX_THRESH)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_LU(a, a_step, m, b, b_step, n, info);
}

முழு lapack_LU64f(இரட்டை* a, size_t a_step, முழு m, இரட்டை* b, size_t b_step, முழு n, முழு* info)
{
    எனில்(m < HAL_LU_SMALL_MATRIX_THRESH)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_LU(a, a_step, m, b, b_step, n, info);
}

முழு lapack_Cholesky32f(தசம* a, size_t a_step, முழு m, தசம* b, size_t b_step, முழு n, தருக்க *info)
{
    எனில்(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

முழு lapack_Cholesky64f(இரட்டை* a, size_t a_step, முழு m, இரட்டை* b, size_t b_step, முழு n, தருக்க *info)
{
    எனில்(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

முழு lapack_SVD32f(தசம* a, size_t a_step, தசம *w, தசம* u, size_t u_step, தசம* vt, size_t v_step, முழு m, முழு n, முழு flags)
{
    எனில்(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    முழு info = 0;
    திரும்பு lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

முழு lapack_SVD64f(இரட்டை* a, size_t a_step, இரட்டை *w, இரட்டை* u, size_t u_step, இரட்டை* vt, size_t v_step, முழு m, முழு n, முழு flags)
{
    எனில்(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    முழு info = 0;
    திரும்பு lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

முழு lapack_QR32f(தசம* src1, size_t src1_step, முழு m, முழு n, முழு k, தசம* src2, size_t src2_step, தசம* dst, முழு* info)
{
    எனில் (m < HAL_QR_SMALL_MATRIX_THRESH)
      திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

முழு lapack_QR64f(இரட்டை* src1, size_t src1_step, முழு m, முழு n, முழு k, இரட்டை* src2, size_t src2_step, இரட்டை* dst, முழு* info)
{
    எனில் (m < HAL_QR_SMALL_MATRIX_THRESH)
      திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

முழு lapack_gemm32f(மாறிலி தசம *src1, size_t src1_step, மாறிலி தசம *src2, size_t src2_step, தசம alpha,
                   மாறிலி தசம *src3, size_t src3_step, தசம beta, தசம *dst, size_t dst_step, முழு m, முழு n, முழு k, முழு flags)
{
    எனில்(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

முழு lapack_gemm64f(மாறிலி இரட்டை *src1, size_t src1_step, மாறிலி இரட்டை *src2, size_t src2_step, இரட்டை alpha,
                   மாறிலி இரட்டை *src3, size_t src3_step, இரட்டை beta, இரட்டை *dst, size_t dst_step, முழு m, முழு n, முழு k, முழு flags)
{
    எனில்(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

முழு lapack_gemm32fc(மாறிலி தசம *src1, size_t src1_step, மாறிலி தசம *src2, size_t src2_step, தசம alpha,
                   மாறிலி தசம *src3, size_t src3_step, தசம beta, தசம *dst, size_t dst_step, முழு m, முழு n, முழு k, முழு flags)
{
    எனில்(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
முழு lapack_gemm64fc(மாறிலி இரட்டை *src1, size_t src1_step, மாறிலி இரட்டை *src2, size_t src2_step, இரட்டை alpha,
                   மாறிலி இரட்டை *src3, size_t src3_step, இரட்டை beta, இரட்டை *dst, size_t dst_step, முழு m, முழு n, முழு k, முழு flags)
{
    எனில்(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        திரும்பு CV_HAL_ERROR_NOT_IMPLEMENTED;
    திரும்பு lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#எனில் defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
