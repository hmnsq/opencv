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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "hal_internal.hpp"

#ifdef HAVE_LAPACK

#અન્તર્ભાવ "opencv_lapack.h"

#અન્તર્ભાવ <cmath>
#અન્તર્ભાવ <algorithm>
#અન્તર્ભાવ <typeinfo>
#અન્તર્ભાવ <limits>
#અન્તર્ભાવ <complex>
#અન્તર્ભાવ <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#જો defined(__clang__) && defined(__has_feature)
#જો __has_feature(memory_sanitizer)
#અન્તર્ભાવ <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) કરો { } જ્યારે(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#જો defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> સ્થિર અંતઃરેખ શૂન્ય
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    માટે(size_t i = 0; i < m - 1; i++)
        માટે(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> સ્થિર અંતઃરેખ શૂન્ય
transpose(અચલ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    માટે(size_t i = 0; i < m; i++)
        માટે(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> સ્થિર અંતઃરેખ શૂન્ય
copy_matrix(અચલ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    માટે(size_t i = 0; i < m; i++)
        માટે(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> સ્થિર અંતઃરેખ શૂન્ય
set_કિંમત_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    માટે(size_t i = 0; i < m; i++)
        માટે(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> સ્થિર અંતઃરેખ પૂર્ણાંક
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, પૂર્ણાંક m, fptype* b, size_t b_step, પૂર્ણાંક n, પૂર્ણાંક* info)
{
#જો defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / કદ(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#નહીંતર
    cv::AutoBuffer<પૂર્ણાંક> piv_buff(m);
    પૂર્ણાંક lda = (પૂર્ણાંક)(a_step / કદ(fptype));
    પૂર્ણાંક _m = m, _n = n;
    પૂર્ણાંક* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    જો(b)
    {
        જો(n == 1 && b_step == કદ(fptype))
        {
            જો(typeid(fptype) == typeid(દશાંશ))
                sgesv_(&_m, &_n, (દશાંશ*)a, &lda, piv, (દશાંશ*)b, &_m, _info);
            નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
                dgesv_(&_m, &_n, (દ્વિગુણ*)a, &lda, piv, (દ્વિગુણ*)b, &_m, _info);
        }
        નહીંતર
        {
            પૂર્ણાંક ldb = (પૂર્ણાંક)(b_step / કદ(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            જો(typeid(fptype) == typeid(દશાંશ))
                sgesv_(&_m, &_n, (દશાંશ*)a, &lda, piv, (દશાંશ*)&tmpB[0], &_m, _info);
            નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
                dgesv_(&_m, &_n, (દ્વિગુણ*)a, &lda, piv, (દ્વિગુણ*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    નહીંતર
    {
        જો(typeid(fptype) == typeid(દશાંશ))
            sgetrf_(&_m, &_m, (દશાંશ*)a, &lda, piv, _info);
        નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
            dgetrf_(&_m, &_m, (દ્વિગુણ*)a, &lda, piv, _info);
    }

#જો defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<પૂર્ણાંક>(_info[0]);
#endif
    પૂર્ણાંક retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    પૂર્ણાંક sign = 0;
    જો(*info == 0)
    {
        માટે(પૂર્ણાંક i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    નહીંતર
        *info = 0; //in opencv LU function zero means error

    પાછા retcode;
}

template <typename fptype> સ્થિર અંતઃરેખ પૂર્ણાંક
lapack_Cholesky(fptype* a, size_t a_step, પૂર્ણાંક m, fptype* b, size_t b_step, પૂર્ણાંક n, તાર્કિક* info)
{
#જો defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / કદ(fptype));
#નહીંતર
    પૂર્ણાંક _m = m, _n = n;
    પૂર્ણાંક lapackStatus = 0;
    પૂર્ણાંક lda = (પૂર્ણાંક)(a_step / કદ(fptype));
#endif
    અક્ષર L[] = {'L', '\0'};

    જો(b)
    {
        જો(n == 1 && b_step == કદ(fptype))
        {
            જો(typeid(fptype) == typeid(દશાંશ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (દશાંશ*)a, &lda, (દશાંશ*)b, &_m, &lapackStatus);
            નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (દ્વિગુણ*)a, &lda, (દ્વિગુણ*)b, &_m, &lapackStatus);
        }
        નહીંતર
        {
            પૂર્ણાંક ldb = (પૂર્ણાંક)(b_step / કદ(fptype));
            fptype* tmpB = નવું fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            જો(typeid(fptype) == typeid(દશાંશ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (દશાંશ*)a, &lda, (દશાંશ*)tmpB, &_m, &lapackStatus);
            નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (દ્વિગુણ*)a, &lda, (દ્વિગુણ*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    નહીંતર
    {
        જો(typeid(fptype) == typeid(દશાંશ))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (દશાંશ*)a, &lda, &lapackStatus);
        નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (દ્વિગુણ*)a, &lda, &lapackStatus);
    }

    જો(lapackStatus == 0) *info = સાચું;
    નહીંતર *info = ખોટું; //in opencv Cholesky function false means error

    પાછા lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> સ્થિર અંતઃરેખ પૂર્ણાંક
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક flags, પૂર્ણાંક* info)
{
#જો defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / કદ(fptype));
    long ldv = (long)(v_step / કદ(fptype));
    long ldu = (long)(u_step / કદ(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#નહીંતર
    પૂર્ણાંક _m = m, _n = n;
    પૂર્ણાંક* _info = info;
    પૂર્ણાંક lda = (પૂર્ણાંક)(a_step / કદ(fptype));
    પૂર્ણાંક ldv = (પૂર્ણાંક)(v_step / કદ(fptype));
    પૂર્ણાંક ldu = (પૂર્ણાંક)(u_step / કદ(fptype));
    પૂર્ણાંક lwork = -1;
    cv::AutoBuffer<પૂર્ણાંક> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    અક્ષર mode[] = { ' ', '\0'};
    જો(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    નહીંતર જો((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    નહીંતર જો((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    નહીંતર જો(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    જો((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    જો(typeid(fptype) == typeid(દશાંશ))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (દશાંશ*)a, &lda, (દશાંશ*)w, (દશાંશ*)u, &ldu, (દશાંશ*)vt, &ldv, (દશાંશ*)&work1, &lwork, iworkBuf, _info);
    નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (દ્વિગુણ*)a, &lda, (દ્વિગુણ*)w, (દ્વિગુણ*)u, &ldu, (દ્વિગુણ*)vt, &ldv, (દ્વિગુણ*)&work1, &lwork, iworkBuf, _info);

    જો(*info < 0)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (પૂર્ણાંક)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), કદ(fptype) * (lwork + 1));

    જો(typeid(fptype) == typeid(દશાંશ))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (દશાંશ*)a, &lda, (દશાંશ*)w, (દશાંશ*)u, &ldu, (દશાંશ*)vt, &ldv, (દશાંશ*)&buffer[0], &lwork, iworkBuf, _info);
    નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (દ્વિગુણ*)a, &lda, (દ્વિગુણ*)w, (દ્વિગુણ*)u, &ldu, (દ્વિગુણ*)vt, &ldv, (દ્વિગુણ*)&buffer[0], &lwork, iworkBuf, _info);

#જો defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<પૂર્ણાંક>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    જો (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    જો (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    જો (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, કદ(fptype) * std::min(m, n));

    જો(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    જો((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        માટે(પૂર્ણાંક i = 0; i < m; i++)
            માટે(પૂર્ણાંક j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    જો(*info < 0)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા CV_HAL_ERROR_OK;
}

template <typename fptype> સ્થિર અંતઃરેખ પૂર્ણાંક
lapack_QR(fptype* a, size_t a_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, fptype* b, size_t b_step, fptype* dst, પૂર્ણાંક* info)
{
#જો defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / કદ(fptype));
    long lwork = -1;
    long ldtmpA;
#નહીંતર
    પૂર્ણાંક _m = m, _n = n, _k = k;
    પૂર્ણાંક* _info = info;
    પૂર્ણાંક lda = (પૂર્ણાંક)(a_step / કદ(fptype));
    પૂર્ણાંક lwork = -1;
    પૂર્ણાંક ldtmpA;
#endif

    અક્ષર mode[] = { 'N', '\0' };
    જો(m < n)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    જો (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    નહીંતર
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    જો (b)
    {
        જો (k == 1 && b_step == કદ(fptype))
        {
            જો (typeid(fptype) == typeid(દશાંશ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (દશાંશ*)tmpA, &ldtmpA, (દશાંશ*)b, &_m, (દશાંશ*)&work1, &lwork, _info);
            નહીંતર જો (typeid(fptype) == typeid(દ્વિગુણ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (દ્વિગુણ*)tmpA, &ldtmpA, (દ્વિગુણ*)b, &_m, (દ્વિગુણ*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, કદ(પૂર્ણાંક));
            જો (*info < 0)
                પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            જો (typeid(fptype) == typeid(દશાંશ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (દશાંશ*)tmpA, &ldtmpA, (દશાંશ*)b, &_m, (દશાંશ*)buffer, &lwork, _info);
            નહીંતર જો (typeid(fptype) == typeid(દ્વિગુણ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (દ્વિગુણ*)tmpA, &ldtmpA, (દ્વિગુણ*)b, &_m, (દ્વિગુણ*)buffer, &lwork, _info);
        }
        નહીંતર
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            પૂર્ણાંક ldb = (પૂર્ણાંક)(b_step / કદ(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            જો (typeid(fptype) == typeid(દશાંશ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (દશાંશ*)tmpA, &ldtmpA, (દશાંશ*)tmpB, &_m, (દશાંશ*)&work1, &lwork, _info);
            નહીંતર જો (typeid(fptype) == typeid(દ્વિગુણ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (દ્વિગુણ*)tmpA, &ldtmpA, (દ્વિગુણ*)tmpB, &_m, (દ્વિગુણ*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, કદ(પૂર્ણાંક));
            જો (*info < 0)
                પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            જો (typeid(fptype) == typeid(દશાંશ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (દશાંશ*)tmpA, &ldtmpA, (દશાંશ*)tmpB, &_m, (દશાંશ*)buffer, &lwork, _info);
            નહીંતર જો (typeid(fptype) == typeid(દ્વિગુણ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (દ્વિગુણ*)tmpA, &ldtmpA, (દ્વિગુણ*)tmpB, &_m, (દ્વિગુણ*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    નહીંતર
    {
        જો (typeid(fptype) == typeid(દશાંશ))
            sgeqrf_(&_m, &_n, (દશાંશ*)tmpA, &ldtmpA, (દશાંશ*)dst, (દશાંશ*)&work1, &lwork, _info);
        નહીંતર જો (typeid(fptype) == typeid(દ્વિગુણ))
            dgeqrf_(&_m, &_n, (દ્વિગુણ*)tmpA, &ldtmpA, (દ્વિગુણ*)dst, (દ્વિગુણ*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, કદ(પૂર્ણાંક));
        જો (*info < 0)
            પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        જો (typeid(fptype) == typeid(દશાંશ))
            sgeqrf_(&_m, &_n, (દશાંશ*)tmpA, &ldtmpA, (દશાંશ*)dst, (દશાંશ*)buffer, &lwork, _info);
        નહીંતર જો (typeid(fptype) == typeid(દ્વિગુણ))
            dgeqrf_(&_m, &_n, (દ્વિગુણ*)tmpA, &ldtmpA, (દ્વિગુણ*)dst, (દ્વિગુણ*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, કદ(પૂર્ણાંક));
    જો (m == n)
        transpose_square_inplace(a, lda, m);
    નહીંતર
        transpose(tmpA, m, a, lda, n, m);

#જો defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<પૂર્ણાંક>(_info[0]);
#endif

    જો (*info != 0)
        *info = 0;
    નહીંતર
        *info = 1;

    પાછા CV_HAL_ERROR_OK;
}

template <typename fptype> સ્થિર અંતઃરેખ પૂર્ણાંક
lapack_gemm(અચલ fptype *src1, size_t src1_step, અચલ fptype *src2, size_t src2_step, fptype alpha,
            અચલ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, પૂર્ણાંક a_m, પૂર્ણાંક a_n, પૂર્ણાંક d_n, પૂર્ણાંક flags)
{
    પૂર્ણાંક ldsrc1 = (પૂર્ણાંક)(src1_step / કદ(fptype));
    પૂર્ણાંક ldsrc2 = (પૂર્ણાંક)(src2_step / કદ(fptype));
    પૂર્ણાંક ldsrc3 = (પૂર્ણાંક)(src3_step / કદ(fptype));
    પૂર્ણાંક lddst = (પૂર્ણાંક)(dst_step / કદ(fptype));
    પૂર્ણાંક c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    જો(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        જો(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        નહીંતર
        {
            d_m = a_m;
        }
    }
    નહીંતર
    {
        transB = CblasNoTrans;
        જો(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        નહીંતર
        {
            d_m = a_m;
        }
    }

    જો(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    નહીંતર
    {
        c_m = d_m;
        c_n = d_n;
    }

    જો(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    નહીંતર
    {
        transA = CblasNoTrans;
    }

    જો(src3 != dst && beta != 0.0 && src3_step != 0) {
        જો(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        નહીંતર
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    નહીંતર જો (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    નહીંતર જો(src3_step == 0 && beta != 0.0)
        set_કિંમત_2(dst, lddst, (fptype)0.0, d_m, d_n);

    જો(typeid(fptype) == typeid(દશાંશ))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (દશાંશ)alpha, (દશાંશ*)src1, ldsrc1, (દશાંશ*)src2, ldsrc2, (દશાંશ)beta, (દશાંશ*)dst, lddst);
    નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (દ્વિગુણ)alpha, (દ્વિગુણ*)src1, ldsrc1, (દ્વિગુણ*)src2, ldsrc2, (દ્વિગુણ)beta, (દ્વિગુણ*)dst, lddst);

    પાછા CV_HAL_ERROR_OK;
}

template <typename fptype> સ્થિર અંતઃરેખ પૂર્ણાંક
lapack_gemm_c(અચલ fptype *src1, size_t src1_step, અચલ fptype *src2, size_t src2_step, fptype alpha,
            અચલ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, પૂર્ણાંક a_m, પૂર્ણાંક a_n, પૂર્ણાંક d_n, પૂર્ણાંક flags)
{
    પૂર્ણાંક ldsrc1 = (પૂર્ણાંક)(src1_step / કદ(std::complex<fptype>));
    પૂર્ણાંક ldsrc2 = (પૂર્ણાંક)(src2_step / કદ(std::complex<fptype>));
    પૂર્ણાંક ldsrc3 = (પૂર્ણાંક)(src3_step / કદ(std::complex<fptype>));
    પૂર્ણાંક lddst = (પૂર્ણાંક)(dst_step / કદ(std::complex<fptype>));
    પૂર્ણાંક c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    જો(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        જો(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        નહીંતર
        {
            d_m = a_m;
        }
    }
    નહીંતર
    {
        transB = CblasNoTrans;
        જો(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        નહીંતર
        {
            d_m = a_m;
        }
    }

    જો(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    નહીંતર
    {
        c_m = d_m;
        c_n = d_n;
    }

    જો(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    નહીંતર
    {
        transA = CblasNoTrans;
    }

    જો(src3 != dst && beta != 0.0 && src3_step != 0) {
        જો(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        નહીંતર
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    નહીંતર જો (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    નહીંતર જો(src3_step == 0 && beta != 0.0)
        set_કિંમત_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#જો defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    પૂર્ણાંક M = a_m, N = d_n, K = a_n;
    જો(typeid(fptype) == typeid(દશાંશ)) {
        auto src1_cast = (std::complex<દશાંશ>*)(src1);
        auto src2_cast = (std::complex<દશાંશ>*)(src2);
        auto dst_cast = (std::complex<દશાંશ>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<દશાંશ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<દશાંશ>*)&cBeta, dst_cast, ldc);
    }
    નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ)) {
        auto src1_cast = (std::complex<દ્વિગુણ>*)(src1);
        auto src2_cast = (std::complex<દ્વિગુણ>*)(src2);
        auto dst_cast = (std::complex<દ્વિગુણ>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<દ્વિગુણ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<દ્વિગુણ>*)&cBeta, dst_cast, ldc);
    }
#નહીંતર
    જો(typeid(fptype) == typeid(દશાંશ))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (દશાંશ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (દશાંશ*)src1, ldsrc1, (દશાંશ*)src2, ldsrc2, (દશાંશ*)reinterpret_cast<fptype(&)[2]>(cBeta), (દશાંશ*)dst, lddst);
    નહીંતર જો(typeid(fptype) == typeid(દ્વિગુણ))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (દ્વિગુણ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (દ્વિગુણ*)src1, ldsrc1, (દ્વિગુણ*)src2, ldsrc2, (દ્વિગુણ*)reinterpret_cast<fptype(&)[2]>(cBeta), (દ્વિગુણ*)dst, lddst);
#endif

    પાછા CV_HAL_ERROR_OK;
}
પૂર્ણાંક lapack_LU32f(દશાંશ* a, size_t a_step, પૂર્ણાંક m, દશાંશ* b, size_t b_step, પૂર્ણાંક n, પૂર્ણાંક* info)
{
    જો(m < HAL_LU_SMALL_MATRIX_THRESH)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_LU(a, a_step, m, b, b_step, n, info);
}

પૂર્ણાંક lapack_LU64f(દ્વિગુણ* a, size_t a_step, પૂર્ણાંક m, દ્વિગુણ* b, size_t b_step, પૂર્ણાંક n, પૂર્ણાંક* info)
{
    જો(m < HAL_LU_SMALL_MATRIX_THRESH)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_LU(a, a_step, m, b, b_step, n, info);
}

પૂર્ણાંક lapack_Cholesky32f(દશાંશ* a, size_t a_step, પૂર્ણાંક m, દશાંશ* b, size_t b_step, પૂર્ણાંક n, તાર્કિક *info)
{
    જો(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

પૂર્ણાંક lapack_Cholesky64f(દ્વિગુણ* a, size_t a_step, પૂર્ણાંક m, દ્વિગુણ* b, size_t b_step, પૂર્ણાંક n, તાર્કિક *info)
{
    જો(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

પૂર્ણાંક lapack_SVD32f(દશાંશ* a, size_t a_step, દશાંશ *w, દશાંશ* u, size_t u_step, દશાંશ* vt, size_t v_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક flags)
{
    જો(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પૂર્ણાંક info = 0;
    પાછા lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

પૂર્ણાંક lapack_SVD64f(દ્વિગુણ* a, size_t a_step, દ્વિગુણ *w, દ્વિગુણ* u, size_t u_step, દ્વિગુણ* vt, size_t v_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક flags)
{
    જો(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પૂર્ણાંક info = 0;
    પાછા lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

પૂર્ણાંક lapack_QR32f(દશાંશ* src1, size_t src1_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, દશાંશ* src2, size_t src2_step, દશાંશ* dst, પૂર્ણાંક* info)
{
    જો (m < HAL_QR_SMALL_MATRIX_THRESH)
      પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

પૂર્ણાંક lapack_QR64f(દ્વિગુણ* src1, size_t src1_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, દ્વિગુણ* src2, size_t src2_step, દ્વિગુણ* dst, પૂર્ણાંક* info)
{
    જો (m < HAL_QR_SMALL_MATRIX_THRESH)
      પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

પૂર્ણાંક lapack_gemm32f(અચલ દશાંશ *src1, size_t src1_step, અચલ દશાંશ *src2, size_t src2_step, દશાંશ alpha,
                   અચલ દશાંશ *src3, size_t src3_step, દશાંશ beta, દશાંશ *dst, size_t dst_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, પૂર્ણાંક flags)
{
    જો(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

પૂર્ણાંક lapack_gemm64f(અચલ દ્વિગુણ *src1, size_t src1_step, અચલ દ્વિગુણ *src2, size_t src2_step, દ્વિગુણ alpha,
                   અચલ દ્વિગુણ *src3, size_t src3_step, દ્વિગુણ beta, દ્વિગુણ *dst, size_t dst_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, પૂર્ણાંક flags)
{
    જો(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

પૂર્ણાંક lapack_gemm32fc(અચલ દશાંશ *src1, size_t src1_step, અચલ દશાંશ *src2, size_t src2_step, દશાંશ alpha,
                   અચલ દશાંશ *src3, size_t src3_step, દશાંશ beta, દશાંશ *dst, size_t dst_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, પૂર્ણાંક flags)
{
    જો(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
પૂર્ણાંક lapack_gemm64fc(અચલ દ્વિગુણ *src1, size_t src1_step, અચલ દ્વિગુણ *src2, size_t src2_step, દ્વિગુણ alpha,
                   અચલ દ્વિગુણ *src3, size_t src3_step, દ્વિગુણ beta, દ્વિગુણ *dst, size_t dst_step, પૂર્ણાંક m, પૂર્ણાંક n, પૂર્ણાંક k, પૂર્ણાંક flags)
{
    જો(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        પાછા CV_HAL_ERROR_NOT_IMPLEMENTED;
    પાછા lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#જો defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
