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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "hal_internal.hpp"

#ifdef HAVE_LAPACK

#अन्तर्भाव "opencv_lapack.h"

#अन्तर्भाव <cmath>
#अन्तर्भाव <algorithm>
#अन्तर्भाव <typeinfo>
#अन्तर्भाव <limits>
#अन्तर्भाव <complex>
#अन्तर्भाव <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#सचे defined(__clang__) && defined(__has_feature)
#सचे __has_feature(memory_sanitizer)
#अन्तर्भाव <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) करोहि { } याव(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#सचे defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> स्थिर अन्तःस्थ शून्य
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    अत्थाय(size_t i = 0; i < m - 1; i++)
        अत्थाय(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> स्थिर अन्तःस्थ शून्य
transpose(ध्रुव fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    अत्थाय(size_t i = 0; i < m; i++)
        अत्थाय(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> स्थिर अन्तःस्थ शून्य
copy_matrix(ध्रुव fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    अत्थाय(size_t i = 0; i < m; i++)
        अत्थाय(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> स्थिर अन्तःस्थ शून्य
set_मानम्_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    अत्थाय(size_t i = 0; i < m; i++)
        अत्थाय(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> स्थिर अन्तःस्थ पूर्णाङ्क
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, पूर्णाङ्क m, fptype* b, size_t b_step, पूर्णाङ्क n, पूर्णाङ्क* info)
{
#सचे defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / परिमाण(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#अञ्ञथा
    cv::AutoBuffer<पूर्णाङ्क> piv_buff(m);
    पूर्णाङ्क lda = (पूर्णाङ्क)(a_step / परिमाण(fptype));
    पूर्णाङ्क _m = m, _n = n;
    पूर्णाङ्क* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    सचे(b)
    {
        सचे(n == 1 && b_step == परिमाण(fptype))
        {
            सचे(typeid(fptype) == typeid(भिन्न))
                sgesv_(&_m, &_n, (भिन्न*)a, &lda, piv, (भिन्न*)b, &_m, _info);
            अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
                dgesv_(&_m, &_n, (द्विगुण*)a, &lda, piv, (द्विगुण*)b, &_m, _info);
        }
        अञ्ञथा
        {
            पूर्णाङ्क ldb = (पूर्णाङ्क)(b_step / परिमाण(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            सचे(typeid(fptype) == typeid(भिन्न))
                sgesv_(&_m, &_n, (भिन्न*)a, &lda, piv, (भिन्न*)&tmpB[0], &_m, _info);
            अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
                dgesv_(&_m, &_n, (द्विगुण*)a, &lda, piv, (द्विगुण*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    अञ्ञथा
    {
        सचे(typeid(fptype) == typeid(भिन्न))
            sgetrf_(&_m, &_m, (भिन्न*)a, &lda, piv, _info);
        अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
            dgetrf_(&_m, &_m, (द्विगुण*)a, &lda, piv, _info);
    }

#सचे defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णाङ्क>(_info[0]);
#endif
    पूर्णाङ्क retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    पूर्णाङ्क sign = 0;
    सचे(*info == 0)
    {
        अत्थाय(पूर्णाङ्क i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    अञ्ञथा
        *info = 0; //in opencv LU function zero means error

    पटिनिवत्त retcode;
}

template <typename fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_Cholesky(fptype* a, size_t a_step, पूर्णाङ्क m, fptype* b, size_t b_step, पूर्णाङ्क n, तार्किक* info)
{
#सचे defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / परिमाण(fptype));
#अञ्ञथा
    पूर्णाङ्क _m = m, _n = n;
    पूर्णाङ्क lapackStatus = 0;
    पूर्णाङ्क lda = (पूर्णाङ्क)(a_step / परिमाण(fptype));
#endif
    वर्ण L[] = {'L', '\0'};

    सचे(b)
    {
        सचे(n == 1 && b_step == परिमाण(fptype))
        {
            सचे(typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (भिन्न*)a, &lda, (भिन्न*)b, &_m, &lapackStatus);
            अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)b, &_m, &lapackStatus);
        }
        अञ्ञथा
        {
            पूर्णाङ्क ldb = (पूर्णाङ्क)(b_step / परिमाण(fptype));
            fptype* tmpB = नूतन fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            सचे(typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (भिन्न*)a, &lda, (भिन्न*)tmpB, &_m, &lapackStatus);
            अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    अञ्ञथा
    {
        सचे(typeid(fptype) == typeid(भिन्न))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (भिन्न*)a, &lda, &lapackStatus);
        अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (द्विगुण*)a, &lda, &lapackStatus);
    }

    सचे(lapackStatus == 0) *info = सच्च;
    अञ्ञथा *info = मिच्छा; //in opencv Cholesky function false means error

    पटिनिवत्त lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags, पूर्णाङ्क* info)
{
#सचे defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / परिमाण(fptype));
    long ldv = (long)(v_step / परिमाण(fptype));
    long ldu = (long)(u_step / परिमाण(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#अञ्ञथा
    पूर्णाङ्क _m = m, _n = n;
    पूर्णाङ्क* _info = info;
    पूर्णाङ्क lda = (पूर्णाङ्क)(a_step / परिमाण(fptype));
    पूर्णाङ्क ldv = (पूर्णाङ्क)(v_step / परिमाण(fptype));
    पूर्णाङ्क ldu = (पूर्णाङ्क)(u_step / परिमाण(fptype));
    पूर्णाङ्क lwork = -1;
    cv::AutoBuffer<पूर्णाङ्क> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    वर्ण mode[] = { ' ', '\0'};
    सचे(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    अञ्ञथा सचे((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    अञ्ञथा सचे((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    अञ्ञथा सचे(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    सचे((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    सचे(typeid(fptype) == typeid(भिन्न))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (भिन्न*)a, &lda, (भिन्न*)w, (भिन्न*)u, &ldu, (भिन्न*)vt, &ldv, (भिन्न*)&work1, &lwork, iworkBuf, _info);
    अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)w, (द्विगुण*)u, &ldu, (द्विगुण*)vt, &ldv, (द्विगुण*)&work1, &lwork, iworkBuf, _info);

    सचे(*info < 0)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (पूर्णाङ्क)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), परिमाण(fptype) * (lwork + 1));

    सचे(typeid(fptype) == typeid(भिन्न))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (भिन्न*)a, &lda, (भिन्न*)w, (भिन्न*)u, &ldu, (भिन्न*)vt, &ldv, (भिन्न*)&buffer[0], &lwork, iworkBuf, _info);
    अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)w, (द्विगुण*)u, &ldu, (द्विगुण*)vt, &ldv, (द्विगुण*)&buffer[0], &lwork, iworkBuf, _info);

#सचे defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णाङ्क>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    सचे (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    सचे (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    सचे (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, परिमाण(fptype) * std::min(m, n));

    सचे(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    सचे((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        अत्थाय(पूर्णाङ्क i = 0; i < m; i++)
            अत्थाय(पूर्णाङ्क j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    सचे(*info < 0)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त CV_HAL_ERROR_OK;
}

template <typename fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_QR(fptype* a, size_t a_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, fptype* b, size_t b_step, fptype* dst, पूर्णाङ्क* info)
{
#सचे defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / परिमाण(fptype));
    long lwork = -1;
    long ldtmpA;
#अञ्ञथा
    पूर्णाङ्क _m = m, _n = n, _k = k;
    पूर्णाङ्क* _info = info;
    पूर्णाङ्क lda = (पूर्णाङ्क)(a_step / परिमाण(fptype));
    पूर्णाङ्क lwork = -1;
    पूर्णाङ्क ldtmpA;
#endif

    वर्ण mode[] = { 'N', '\0' };
    सचे(m < n)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    सचे (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    अञ्ञथा
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    सचे (b)
    {
        सचे (k == 1 && b_step == परिमाण(fptype))
        {
            सचे (typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)b, &_m, (भिन्न*)&work1, &lwork, _info);
            अञ्ञथा सचे (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)b, &_m, (द्विगुण*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, परिमाण(पूर्णाङ्क));
            सचे (*info < 0)
                पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            सचे (typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)b, &_m, (भिन्न*)buffer, &lwork, _info);
            अञ्ञथा सचे (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)b, &_m, (द्विगुण*)buffer, &lwork, _info);
        }
        अञ्ञथा
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            पूर्णाङ्क ldb = (पूर्णाङ्क)(b_step / परिमाण(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            सचे (typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)tmpB, &_m, (भिन्न*)&work1, &lwork, _info);
            अञ्ञथा सचे (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)tmpB, &_m, (द्विगुण*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, परिमाण(पूर्णाङ्क));
            सचे (*info < 0)
                पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            सचे (typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)tmpB, &_m, (भिन्न*)buffer, &lwork, _info);
            अञ्ञथा सचे (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)tmpB, &_m, (द्विगुण*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    अञ्ञथा
    {
        सचे (typeid(fptype) == typeid(भिन्न))
            sgeqrf_(&_m, &_n, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)dst, (भिन्न*)&work1, &lwork, _info);
        अञ्ञथा सचे (typeid(fptype) == typeid(द्विगुण))
            dgeqrf_(&_m, &_n, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)dst, (द्विगुण*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, परिमाण(पूर्णाङ्क));
        सचे (*info < 0)
            पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        सचे (typeid(fptype) == typeid(भिन्न))
            sgeqrf_(&_m, &_n, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)dst, (भिन्न*)buffer, &lwork, _info);
        अञ्ञथा सचे (typeid(fptype) == typeid(द्विगुण))
            dgeqrf_(&_m, &_n, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)dst, (द्विगुण*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, परिमाण(पूर्णाङ्क));
    सचे (m == n)
        transpose_square_inplace(a, lda, m);
    अञ्ञथा
        transpose(tmpA, m, a, lda, n, m);

#सचे defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णाङ्क>(_info[0]);
#endif

    सचे (*info != 0)
        *info = 0;
    अञ्ञथा
        *info = 1;

    पटिनिवत्त CV_HAL_ERROR_OK;
}

template <typename fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_gemm(ध्रुव fptype *src1, size_t src1_step, ध्रुव fptype *src2, size_t src2_step, fptype alpha,
            ध्रुव fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, पूर्णाङ्क a_m, पूर्णाङ्क a_n, पूर्णाङ्क d_n, पूर्णाङ्क flags)
{
    पूर्णाङ्क ldsrc1 = (पूर्णाङ्क)(src1_step / परिमाण(fptype));
    पूर्णाङ्क ldsrc2 = (पूर्णाङ्क)(src2_step / परिमाण(fptype));
    पूर्णाङ्क ldsrc3 = (पूर्णाङ्क)(src3_step / परिमाण(fptype));
    पूर्णाङ्क lddst = (पूर्णाङ्क)(dst_step / परिमाण(fptype));
    पूर्णाङ्क c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    सचे(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        सचे(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अञ्ञथा
        {
            d_m = a_m;
        }
    }
    अञ्ञथा
    {
        transB = CblasNoTrans;
        सचे(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अञ्ञथा
        {
            d_m = a_m;
        }
    }

    सचे(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    अञ्ञथा
    {
        c_m = d_m;
        c_n = d_n;
    }

    सचे(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    अञ्ञथा
    {
        transA = CblasNoTrans;
    }

    सचे(src3 != dst && beta != 0.0 && src3_step != 0) {
        सचे(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        अञ्ञथा
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    अञ्ञथा सचे (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    अञ्ञथा सचे(src3_step == 0 && beta != 0.0)
        set_मानम्_2(dst, lddst, (fptype)0.0, d_m, d_n);

    सचे(typeid(fptype) == typeid(भिन्न))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (भिन्न)alpha, (भिन्न*)src1, ldsrc1, (भिन्न*)src2, ldsrc2, (भिन्न)beta, (भिन्न*)dst, lddst);
    अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (द्विगुण)alpha, (द्विगुण*)src1, ldsrc1, (द्विगुण*)src2, ldsrc2, (द्विगुण)beta, (द्विगुण*)dst, lddst);

    पटिनिवत्त CV_HAL_ERROR_OK;
}

template <typename fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_gemm_c(ध्रुव fptype *src1, size_t src1_step, ध्रुव fptype *src2, size_t src2_step, fptype alpha,
            ध्रुव fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, पूर्णाङ्क a_m, पूर्णाङ्क a_n, पूर्णाङ्क d_n, पूर्णाङ्क flags)
{
    पूर्णाङ्क ldsrc1 = (पूर्णाङ्क)(src1_step / परिमाण(std::complex<fptype>));
    पूर्णाङ्क ldsrc2 = (पूर्णाङ्क)(src2_step / परिमाण(std::complex<fptype>));
    पूर्णाङ्क ldsrc3 = (पूर्णाङ्क)(src3_step / परिमाण(std::complex<fptype>));
    पूर्णाङ्क lddst = (पूर्णाङ्क)(dst_step / परिमाण(std::complex<fptype>));
    पूर्णाङ्क c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    सचे(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        सचे(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अञ्ञथा
        {
            d_m = a_m;
        }
    }
    अञ्ञथा
    {
        transB = CblasNoTrans;
        सचे(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अञ्ञथा
        {
            d_m = a_m;
        }
    }

    सचे(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    अञ्ञथा
    {
        c_m = d_m;
        c_n = d_n;
    }

    सचे(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    अञ्ञथा
    {
        transA = CblasNoTrans;
    }

    सचे(src3 != dst && beta != 0.0 && src3_step != 0) {
        सचे(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        अञ्ञथा
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    अञ्ञथा सचे (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    अञ्ञथा सचे(src3_step == 0 && beta != 0.0)
        set_मानम्_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#सचे defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    पूर्णाङ्क M = a_m, N = d_n, K = a_n;
    सचे(typeid(fptype) == typeid(भिन्न)) {
        auto src1_cast = (std::complex<भिन्न>*)(src1);
        auto src2_cast = (std::complex<भिन्न>*)(src2);
        auto dst_cast = (std::complex<भिन्न>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<भिन्न>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<भिन्न>*)&cBeta, dst_cast, ldc);
    }
    अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण)) {
        auto src1_cast = (std::complex<द्विगुण>*)(src1);
        auto src2_cast = (std::complex<द्विगुण>*)(src2);
        auto dst_cast = (std::complex<द्विगुण>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<द्विगुण>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<द्विगुण>*)&cBeta, dst_cast, ldc);
    }
#अञ्ञथा
    सचे(typeid(fptype) == typeid(भिन्न))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (भिन्न*)reinterpret_cast<fptype(&)[2]>(cAlpha), (भिन्न*)src1, ldsrc1, (भिन्न*)src2, ldsrc2, (भिन्न*)reinterpret_cast<fptype(&)[2]>(cBeta), (भिन्न*)dst, lddst);
    अञ्ञथा सचे(typeid(fptype) == typeid(द्विगुण))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (द्विगुण*)reinterpret_cast<fptype(&)[2]>(cAlpha), (द्विगुण*)src1, ldsrc1, (द्विगुण*)src2, ldsrc2, (द्विगुण*)reinterpret_cast<fptype(&)[2]>(cBeta), (द्विगुण*)dst, lddst);
#endif

    पटिनिवत्त CV_HAL_ERROR_OK;
}
पूर्णाङ्क lapack_LU32f(भिन्न* a, size_t a_step, पूर्णाङ्क m, भिन्न* b, size_t b_step, पूर्णाङ्क n, पूर्णाङ्क* info)
{
    सचे(m < HAL_LU_SMALL_MATRIX_THRESH)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_LU(a, a_step, m, b, b_step, n, info);
}

पूर्णाङ्क lapack_LU64f(द्विगुण* a, size_t a_step, पूर्णाङ्क m, द्विगुण* b, size_t b_step, पूर्णाङ्क n, पूर्णाङ्क* info)
{
    सचे(m < HAL_LU_SMALL_MATRIX_THRESH)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_LU(a, a_step, m, b, b_step, n, info);
}

पूर्णाङ्क lapack_Cholesky32f(भिन्न* a, size_t a_step, पूर्णाङ्क m, भिन्न* b, size_t b_step, पूर्णाङ्क n, तार्किक *info)
{
    सचे(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

पूर्णाङ्क lapack_Cholesky64f(द्विगुण* a, size_t a_step, पूर्णाङ्क m, द्विगुण* b, size_t b_step, पूर्णाङ्क n, तार्किक *info)
{
    सचे(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

पूर्णाङ्क lapack_SVD32f(भिन्न* a, size_t a_step, भिन्न *w, भिन्न* u, size_t u_step, भिन्न* vt, size_t v_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags)
{
    सचे(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पूर्णाङ्क info = 0;
    पटिनिवत्त lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

पूर्णाङ्क lapack_SVD64f(द्विगुण* a, size_t a_step, द्विगुण *w, द्विगुण* u, size_t u_step, द्विगुण* vt, size_t v_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags)
{
    सचे(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पूर्णाङ्क info = 0;
    पटिनिवत्त lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

पूर्णाङ्क lapack_QR32f(भिन्न* src1, size_t src1_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, भिन्न* src2, size_t src2_step, भिन्न* dst, पूर्णाङ्क* info)
{
    सचे (m < HAL_QR_SMALL_MATRIX_THRESH)
      पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

पूर्णाङ्क lapack_QR64f(द्विगुण* src1, size_t src1_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, द्विगुण* src2, size_t src2_step, द्विगुण* dst, पूर्णाङ्क* info)
{
    सचे (m < HAL_QR_SMALL_MATRIX_THRESH)
      पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

पूर्णाङ्क lapack_gemm32f(ध्रुव भिन्न *src1, size_t src1_step, ध्रुव भिन्न *src2, size_t src2_step, भिन्न alpha,
                   ध्रुव भिन्न *src3, size_t src3_step, भिन्न beta, भिन्न *dst, size_t dst_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags)
{
    सचे(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

पूर्णाङ्क lapack_gemm64f(ध्रुव द्विगुण *src1, size_t src1_step, ध्रुव द्विगुण *src2, size_t src2_step, द्विगुण alpha,
                   ध्रुव द्विगुण *src3, size_t src3_step, द्विगुण beta, द्विगुण *dst, size_t dst_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags)
{
    सचे(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

पूर्णाङ्क lapack_gemm32fc(ध्रुव भिन्न *src1, size_t src1_step, ध्रुव भिन्न *src2, size_t src2_step, भिन्न alpha,
                   ध्रुव भिन्न *src3, size_t src3_step, भिन्न beta, भिन्न *dst, size_t dst_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags)
{
    सचे(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
पूर्णाङ्क lapack_gemm64fc(ध्रुव द्विगुण *src1, size_t src1_step, ध्रुव द्विगुण *src2, size_t src2_step, द्विगुण alpha,
                   ध्रुव द्विगुण *src3, size_t src3_step, द्विगुण beta, द्विगुण *dst, size_t dst_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags)
{
    सचे(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        पटिनिवत्त CV_HAL_ERROR_NOT_IMPLEMENTED;
    पटिनिवत्त lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#सचे defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
