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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "hal_internal.hpp"

#ifdef HAVE_LAPACK

#അന്തര്ഭാവ "opencv_lapack.h"

#അന്തര്ഭാവ <cmath>
#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <typeinfo>
#അന്തര്ഭാവ <limits>
#അന്തര്ഭാവ <complex>
#അന്തര്ഭാവ <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#എങ്കിൽ defined(__clang__) && defined(__has_feature)
#എങ്കിൽ __has_feature(memory_sanitizer)
#അന്തര്ഭാവ <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) ചെയ്യുക { } ആകുമ്പോൾ(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#എങ്കിൽ defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> സ്ഥിതം വരിയിൽ ശൂന്യം
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    വേണ്ടി(size_t i = 0; i < m - 1; i++)
        വേണ്ടി(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> സ്ഥിതം വരിയിൽ ശൂന്യം
transpose(സ്ഥിരം fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    വേണ്ടി(size_t i = 0; i < m; i++)
        വേണ്ടി(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> സ്ഥിതം വരിയിൽ ശൂന്യം
copy_matrix(സ്ഥിരം fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    വേണ്ടി(size_t i = 0; i < m; i++)
        വേണ്ടി(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> സ്ഥിതം വരിയിൽ ശൂന്യം
set_മൂല്യം_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    വേണ്ടി(size_t i = 0; i < m; i++)
        വേണ്ടി(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, പൂർണ്ണസംഖ്യ m, fptype* b, size_t b_step, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ* info)
{
#എങ്കിൽ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / വലുപ്പം(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#അല്ലെങ്കിൽ
    cv::AutoBuffer<പൂർണ്ണസംഖ്യ> piv_buff(m);
    പൂർണ്ണസംഖ്യ lda = (പൂർണ്ണസംഖ്യ)(a_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ _m = m, _n = n;
    പൂർണ്ണസംഖ്യ* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    എങ്കിൽ(b)
    {
        എങ്കിൽ(n == 1 && b_step == വലുപ്പം(fptype))
        {
            എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
                sgesv_(&_m, &_n, (ദശാംശം*)a, &lda, piv, (ദശാംശം*)b, &_m, _info);
            അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
                dgesv_(&_m, &_n, (ഇരട്ട*)a, &lda, piv, (ഇരട്ട*)b, &_m, _info);
        }
        അല്ലെങ്കിൽ
        {
            പൂർണ്ണസംഖ്യ ldb = (പൂർണ്ണസംഖ്യ)(b_step / വലുപ്പം(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
                sgesv_(&_m, &_n, (ദശാംശം*)a, &lda, piv, (ദശാംശം*)&tmpB[0], &_m, _info);
            അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
                dgesv_(&_m, &_n, (ഇരട്ട*)a, &lda, piv, (ഇരട്ട*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
            sgetrf_(&_m, &_m, (ദശാംശം*)a, &lda, piv, _info);
        അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
            dgetrf_(&_m, &_m, (ഇരട്ട*)a, &lda, piv, _info);
    }

#എങ്കിൽ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<പൂർണ്ണസംഖ്യ>(_info[0]);
#endif
    പൂർണ്ണസംഖ്യ retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    പൂർണ്ണസംഖ്യ sign = 0;
    എങ്കിൽ(*info == 0)
    {
        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    അല്ലെങ്കിൽ
        *info = 0; //in opencv LU function zero means error

    തിരികെ retcode;
}

template <typename fptype> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
lapack_Cholesky(fptype* a, size_t a_step, പൂർണ്ണസംഖ്യ m, fptype* b, size_t b_step, പൂർണ്ണസംഖ്യ n, തർക്കം* info)
{
#എങ്കിൽ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / വലുപ്പം(fptype));
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ _m = m, _n = n;
    പൂർണ്ണസംഖ്യ lapackStatus = 0;
    പൂർണ്ണസംഖ്യ lda = (പൂർണ്ണസംഖ്യ)(a_step / വലുപ്പം(fptype));
#endif
    അക്ഷരം L[] = {'L', '\0'};

    എങ്കിൽ(b)
    {
        എങ്കിൽ(n == 1 && b_step == വലുപ്പം(fptype))
        {
            എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ദശാംശം*)a, &lda, (ദശാംശം*)b, &_m, &lapackStatus);
            അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ഇരട്ട*)a, &lda, (ഇരട്ട*)b, &_m, &lapackStatus);
        }
        അല്ലെങ്കിൽ
        {
            പൂർണ്ണസംഖ്യ ldb = (പൂർണ്ണസംഖ്യ)(b_step / വലുപ്പം(fptype));
            fptype* tmpB = പുതിയ fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ദശാംശം*)a, &lda, (ദശാംശം*)tmpB, &_m, &lapackStatus);
            അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ഇരട്ട*)a, &lda, (ഇരട്ട*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (ദശാംശം*)a, &lda, &lapackStatus);
        അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (ഇരട്ട*)a, &lda, &lapackStatus);
    }

    എങ്കിൽ(lapackStatus == 0) *info = സത്യം;
    അല്ലെങ്കിൽ *info = അസത്യം; //in opencv Cholesky function false means error

    തിരികെ lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ* info)
{
#എങ്കിൽ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / വലുപ്പം(fptype));
    long ldv = (long)(v_step / വലുപ്പം(fptype));
    long ldu = (long)(u_step / വലുപ്പം(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ _m = m, _n = n;
    പൂർണ്ണസംഖ്യ* _info = info;
    പൂർണ്ണസംഖ്യ lda = (പൂർണ്ണസംഖ്യ)(a_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ ldv = (പൂർണ്ണസംഖ്യ)(v_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ ldu = (പൂർണ്ണസംഖ്യ)(u_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ lwork = -1;
    cv::AutoBuffer<പൂർണ്ണസംഖ്യ> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    അക്ഷരം mode[] = { ' ', '\0'};
    എങ്കിൽ(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    അല്ലെങ്കിൽ എങ്കിൽ((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    അല്ലെങ്കിൽ എങ്കിൽ((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    അല്ലെങ്കിൽ എങ്കിൽ(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    എങ്കിൽ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (ദശാംശം*)a, &lda, (ദശാംശം*)w, (ദശാംശം*)u, &ldu, (ദശാംശം*)vt, &ldv, (ദശാംശം*)&work1, &lwork, iworkBuf, _info);
    അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (ഇരട്ട*)a, &lda, (ഇരട്ട*)w, (ഇരട്ട*)u, &ldu, (ഇരട്ട*)vt, &ldv, (ഇരട്ട*)&work1, &lwork, iworkBuf, _info);

    എങ്കിൽ(*info < 0)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (പൂർണ്ണസംഖ്യ)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), വലുപ്പം(fptype) * (lwork + 1));

    എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (ദശാംശം*)a, &lda, (ദശാംശം*)w, (ദശാംശം*)u, &ldu, (ദശാംശം*)vt, &ldv, (ദശാംശം*)&buffer[0], &lwork, iworkBuf, _info);
    അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (ഇരട്ട*)a, &lda, (ഇരട്ട*)w, (ഇരട്ട*)u, &ldu, (ഇരട്ട*)vt, &ldv, (ഇരട്ട*)&buffer[0], &lwork, iworkBuf, _info);

#എങ്കിൽ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<പൂർണ്ണസംഖ്യ>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    എങ്കിൽ (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    എങ്കിൽ (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    എങ്കിൽ (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, വലുപ്പം(fptype) * std::min(m, n));

    എങ്കിൽ(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    എങ്കിൽ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < m; i++)
            വേണ്ടി(പൂർണ്ണസംഖ്യ j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    എങ്കിൽ(*info < 0)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ CV_HAL_ERROR_OK;
}

template <typename fptype> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
lapack_QR(fptype* a, size_t a_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, fptype* b, size_t b_step, fptype* dst, പൂർണ്ണസംഖ്യ* info)
{
#എങ്കിൽ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / വലുപ്പം(fptype));
    long lwork = -1;
    long ldtmpA;
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ _m = m, _n = n, _k = k;
    പൂർണ്ണസംഖ്യ* _info = info;
    പൂർണ്ണസംഖ്യ lda = (പൂർണ്ണസംഖ്യ)(a_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ lwork = -1;
    പൂർണ്ണസംഖ്യ ldtmpA;
#endif

    അക്ഷരം mode[] = { 'N', '\0' };
    എങ്കിൽ(m < n)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    എങ്കിൽ (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    അല്ലെങ്കിൽ
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    എങ്കിൽ (b)
    {
        എങ്കിൽ (k == 1 && b_step == വലുപ്പം(fptype))
        {
            എങ്കിൽ (typeid(fptype) == typeid(ദശാംശം))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ദശാംശം*)tmpA, &ldtmpA, (ദശാംശം*)b, &_m, (ദശാംശം*)&work1, &lwork, _info);
            അല്ലെങ്കിൽ എങ്കിൽ (typeid(fptype) == typeid(ഇരട്ട))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ഇരട്ട*)tmpA, &ldtmpA, (ഇരട്ട*)b, &_m, (ഇരട്ട*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, വലുപ്പം(പൂർണ്ണസംഖ്യ));
            എങ്കിൽ (*info < 0)
                തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            എങ്കിൽ (typeid(fptype) == typeid(ദശാംശം))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ദശാംശം*)tmpA, &ldtmpA, (ദശാംശം*)b, &_m, (ദശാംശം*)buffer, &lwork, _info);
            അല്ലെങ്കിൽ എങ്കിൽ (typeid(fptype) == typeid(ഇരട്ട))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ഇരട്ട*)tmpA, &ldtmpA, (ഇരട്ട*)b, &_m, (ഇരട്ട*)buffer, &lwork, _info);
        }
        അല്ലെങ്കിൽ
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            പൂർണ്ണസംഖ്യ ldb = (പൂർണ്ണസംഖ്യ)(b_step / വലുപ്പം(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            എങ്കിൽ (typeid(fptype) == typeid(ദശാംശം))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ദശാംശം*)tmpA, &ldtmpA, (ദശാംശം*)tmpB, &_m, (ദശാംശം*)&work1, &lwork, _info);
            അല്ലെങ്കിൽ എങ്കിൽ (typeid(fptype) == typeid(ഇരട്ട))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ഇരട്ട*)tmpA, &ldtmpA, (ഇരട്ട*)tmpB, &_m, (ഇരട്ട*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, വലുപ്പം(പൂർണ്ണസംഖ്യ));
            എങ്കിൽ (*info < 0)
                തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            എങ്കിൽ (typeid(fptype) == typeid(ദശാംശം))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ദശാംശം*)tmpA, &ldtmpA, (ദശാംശം*)tmpB, &_m, (ദശാംശം*)buffer, &lwork, _info);
            അല്ലെങ്കിൽ എങ്കിൽ (typeid(fptype) == typeid(ഇരട്ട))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ഇരട്ട*)tmpA, &ldtmpA, (ഇരട്ട*)tmpB, &_m, (ഇരട്ട*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (typeid(fptype) == typeid(ദശാംശം))
            sgeqrf_(&_m, &_n, (ദശാംശം*)tmpA, &ldtmpA, (ദശാംശം*)dst, (ദശാംശം*)&work1, &lwork, _info);
        അല്ലെങ്കിൽ എങ്കിൽ (typeid(fptype) == typeid(ഇരട്ട))
            dgeqrf_(&_m, &_n, (ഇരട്ട*)tmpA, &ldtmpA, (ഇരട്ട*)dst, (ഇരട്ട*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, വലുപ്പം(പൂർണ്ണസംഖ്യ));
        എങ്കിൽ (*info < 0)
            തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        എങ്കിൽ (typeid(fptype) == typeid(ദശാംശം))
            sgeqrf_(&_m, &_n, (ദശാംശം*)tmpA, &ldtmpA, (ദശാംശം*)dst, (ദശാംശം*)buffer, &lwork, _info);
        അല്ലെങ്കിൽ എങ്കിൽ (typeid(fptype) == typeid(ഇരട്ട))
            dgeqrf_(&_m, &_n, (ഇരട്ട*)tmpA, &ldtmpA, (ഇരട്ട*)dst, (ഇരട്ട*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    എങ്കിൽ (m == n)
        transpose_square_inplace(a, lda, m);
    അല്ലെങ്കിൽ
        transpose(tmpA, m, a, lda, n, m);

#എങ്കിൽ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<പൂർണ്ണസംഖ്യ>(_info[0]);
#endif

    എങ്കിൽ (*info != 0)
        *info = 0;
    അല്ലെങ്കിൽ
        *info = 1;

    തിരികെ CV_HAL_ERROR_OK;
}

template <typename fptype> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
lapack_gemm(സ്ഥിരം fptype *src1, size_t src1_step, സ്ഥിരം fptype *src2, size_t src2_step, fptype alpha,
            സ്ഥിരം fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, പൂർണ്ണസംഖ്യ a_m, പൂർണ്ണസംഖ്യ a_n, പൂർണ്ണസംഖ്യ d_n, പൂർണ്ണസംഖ്യ flags)
{
    പൂർണ്ണസംഖ്യ ldsrc1 = (പൂർണ്ണസംഖ്യ)(src1_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ ldsrc2 = (പൂർണ്ണസംഖ്യ)(src2_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ ldsrc3 = (പൂർണ്ണസംഖ്യ)(src3_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ lddst = (പൂർണ്ണസംഖ്യ)(dst_step / വലുപ്പം(fptype));
    പൂർണ്ണസംഖ്യ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    എങ്കിൽ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        എങ്കിൽ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        അല്ലെങ്കിൽ
        {
            d_m = a_m;
        }
    }
    അല്ലെങ്കിൽ
    {
        transB = CblasNoTrans;
        എങ്കിൽ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        അല്ലെങ്കിൽ
        {
            d_m = a_m;
        }
    }

    എങ്കിൽ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    അല്ലെങ്കിൽ
    {
        c_m = d_m;
        c_n = d_n;
    }

    എങ്കിൽ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    അല്ലെങ്കിൽ
    {
        transA = CblasNoTrans;
    }

    എങ്കിൽ(src3 != dst && beta != 0.0 && src3_step != 0) {
        എങ്കിൽ(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        അല്ലെങ്കിൽ
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    അല്ലെങ്കിൽ എങ്കിൽ(src3_step == 0 && beta != 0.0)
        set_മൂല്യം_2(dst, lddst, (fptype)0.0, d_m, d_n);

    എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ദശാംശം)alpha, (ദശാംശം*)src1, ldsrc1, (ദശാംശം*)src2, ldsrc2, (ദശാംശം)beta, (ദശാംശം*)dst, lddst);
    അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ഇരട്ട)alpha, (ഇരട്ട*)src1, ldsrc1, (ഇരട്ട*)src2, ldsrc2, (ഇരട്ട)beta, (ഇരട്ട*)dst, lddst);

    തിരികെ CV_HAL_ERROR_OK;
}

template <typename fptype> സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
lapack_gemm_c(സ്ഥിരം fptype *src1, size_t src1_step, സ്ഥിരം fptype *src2, size_t src2_step, fptype alpha,
            സ്ഥിരം fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, പൂർണ്ണസംഖ്യ a_m, പൂർണ്ണസംഖ്യ a_n, പൂർണ്ണസംഖ്യ d_n, പൂർണ്ണസംഖ്യ flags)
{
    പൂർണ്ണസംഖ്യ ldsrc1 = (പൂർണ്ണസംഖ്യ)(src1_step / വലുപ്പം(std::complex<fptype>));
    പൂർണ്ണസംഖ്യ ldsrc2 = (പൂർണ്ണസംഖ്യ)(src2_step / വലുപ്പം(std::complex<fptype>));
    പൂർണ്ണസംഖ്യ ldsrc3 = (പൂർണ്ണസംഖ്യ)(src3_step / വലുപ്പം(std::complex<fptype>));
    പൂർണ്ണസംഖ്യ lddst = (പൂർണ്ണസംഖ്യ)(dst_step / വലുപ്പം(std::complex<fptype>));
    പൂർണ്ണസംഖ്യ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    എങ്കിൽ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        എങ്കിൽ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        അല്ലെങ്കിൽ
        {
            d_m = a_m;
        }
    }
    അല്ലെങ്കിൽ
    {
        transB = CblasNoTrans;
        എങ്കിൽ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        അല്ലെങ്കിൽ
        {
            d_m = a_m;
        }
    }

    എങ്കിൽ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    അല്ലെങ്കിൽ
    {
        c_m = d_m;
        c_n = d_n;
    }

    എങ്കിൽ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    അല്ലെങ്കിൽ
    {
        transA = CblasNoTrans;
    }

    എങ്കിൽ(src3 != dst && beta != 0.0 && src3_step != 0) {
        എങ്കിൽ(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        അല്ലെങ്കിൽ
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    അല്ലെങ്കിൽ എങ്കിൽ(src3_step == 0 && beta != 0.0)
        set_മൂല്യം_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#എങ്കിൽ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    പൂർണ്ണസംഖ്യ M = a_m, N = d_n, K = a_n;
    എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം)) {
        auto src1_cast = (std::complex<ദശാംശം>*)(src1);
        auto src2_cast = (std::complex<ദശാംശം>*)(src2);
        auto dst_cast = (std::complex<ദശാംശം>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<ദശാംശം>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ദശാംശം>*)&cBeta, dst_cast, ldc);
    }
    അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട)) {
        auto src1_cast = (std::complex<ഇരട്ട>*)(src1);
        auto src2_cast = (std::complex<ഇരട്ട>*)(src2);
        auto dst_cast = (std::complex<ഇരട്ട>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<ഇരട്ട>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ഇരട്ട>*)&cBeta, dst_cast, ldc);
    }
#അല്ലെങ്കിൽ
    എങ്കിൽ(typeid(fptype) == typeid(ദശാംശം))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ദശാംശം*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ദശാംശം*)src1, ldsrc1, (ദശാംശം*)src2, ldsrc2, (ദശാംശം*)reinterpret_cast<fptype(&)[2]>(cBeta), (ദശാംശം*)dst, lddst);
    അല്ലെങ്കിൽ എങ്കിൽ(typeid(fptype) == typeid(ഇരട്ട))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ഇരട്ട*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ഇരട്ട*)src1, ldsrc1, (ഇരട്ട*)src2, ldsrc2, (ഇരട്ട*)reinterpret_cast<fptype(&)[2]>(cBeta), (ഇരട്ട*)dst, lddst);
#endif

    തിരികെ CV_HAL_ERROR_OK;
}
പൂർണ്ണസംഖ്യ lapack_LU32f(ദശാംശം* a, size_t a_step, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t b_step, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ* info)
{
    എങ്കിൽ(m < HAL_LU_SMALL_MATRIX_THRESH)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_LU(a, a_step, m, b, b_step, n, info);
}

പൂർണ്ണസംഖ്യ lapack_LU64f(ഇരട്ട* a, size_t a_step, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t b_step, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ* info)
{
    എങ്കിൽ(m < HAL_LU_SMALL_MATRIX_THRESH)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_LU(a, a_step, m, b, b_step, n, info);
}

പൂർണ്ണസംഖ്യ lapack_Cholesky32f(ദശാംശം* a, size_t a_step, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t b_step, പൂർണ്ണസംഖ്യ n, തർക്കം *info)
{
    എങ്കിൽ(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

പൂർണ്ണസംഖ്യ lapack_Cholesky64f(ഇരട്ട* a, size_t a_step, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t b_step, പൂർണ്ണസംഖ്യ n, തർക്കം *info)
{
    എങ്കിൽ(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

പൂർണ്ണസംഖ്യ lapack_SVD32f(ദശാംശം* a, size_t a_step, ദശാംശം *w, ദശാംശം* u, size_t u_step, ദശാംശം* vt, size_t v_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ flags)
{
    എങ്കിൽ(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    പൂർണ്ണസംഖ്യ info = 0;
    തിരികെ lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

പൂർണ്ണസംഖ്യ lapack_SVD64f(ഇരട്ട* a, size_t a_step, ഇരട്ട *w, ഇരട്ട* u, size_t u_step, ഇരട്ട* vt, size_t v_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ flags)
{
    എങ്കിൽ(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    പൂർണ്ണസംഖ്യ info = 0;
    തിരികെ lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

പൂർണ്ണസംഖ്യ lapack_QR32f(ദശാംശം* src1, size_t src1_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, ദശാംശം* src2, size_t src2_step, ദശാംശം* dst, പൂർണ്ണസംഖ്യ* info)
{
    എങ്കിൽ (m < HAL_QR_SMALL_MATRIX_THRESH)
      തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

പൂർണ്ണസംഖ്യ lapack_QR64f(ഇരട്ട* src1, size_t src1_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, ഇരട്ട* src2, size_t src2_step, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ* info)
{
    എങ്കിൽ (m < HAL_QR_SMALL_MATRIX_THRESH)
      തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

പൂർണ്ണസംഖ്യ lapack_gemm32f(സ്ഥിരം ദശാംശം *src1, size_t src1_step, സ്ഥിരം ദശാംശം *src2, size_t src2_step, ദശാംശം alpha,
                   സ്ഥിരം ദശാംശം *src3, size_t src3_step, ദശാംശം beta, ദശാംശം *dst, size_t dst_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ flags)
{
    എങ്കിൽ(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

പൂർണ്ണസംഖ്യ lapack_gemm64f(സ്ഥിരം ഇരട്ട *src1, size_t src1_step, സ്ഥിരം ഇരട്ട *src2, size_t src2_step, ഇരട്ട alpha,
                   സ്ഥിരം ഇരട്ട *src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട *dst, size_t dst_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ flags)
{
    എങ്കിൽ(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

പൂർണ്ണസംഖ്യ lapack_gemm32fc(സ്ഥിരം ദശാംശം *src1, size_t src1_step, സ്ഥിരം ദശാംശം *src2, size_t src2_step, ദശാംശം alpha,
                   സ്ഥിരം ദശാംശം *src3, size_t src3_step, ദശാംശം beta, ദശാംശം *dst, size_t dst_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ flags)
{
    എങ്കിൽ(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
പൂർണ്ണസംഖ്യ lapack_gemm64fc(സ്ഥിരം ഇരട്ട *src1, size_t src1_step, സ്ഥിരം ഇരട്ട *src2, size_t src2_step, ഇരട്ട alpha,
                   സ്ഥിരം ഇരട്ട *src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട *dst, size_t dst_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ flags)
{
    എങ്കിൽ(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        തിരികെ CV_HAL_ERROR_NOT_IMPLEMENTED;
    തിരികെ lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#എങ്കിൽ defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
