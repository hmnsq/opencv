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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "hal_internal.hpp"

#ifdef HAVE_LAPACK

#ಅನ್ತರ್ಭಾವ "opencv_lapack.h"

#ಅನ್ತರ್ಭಾವ <cmath>
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <typeinfo>
#ಅನ್ತರ್ಭಾವ <limits>
#ಅನ್ತರ್ಭಾವ <complex>
#ಅನ್ತರ್ಭಾವ <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#ಯದಿ defined(__clang__) && defined(__has_feature)
#ಯದಿ __has_feature(memory_sanitizer)
#ಅನ್ತರ್ಭಾವ <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) ಕುರು { } ಯಾವತ್(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#ಯದಿ defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    ಪ್ರತಿ(size_t i = 0; i < m - 1; i++)
        ಪ್ರತಿ(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
transpose(ಧ್ರುವ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ಪ್ರತಿ(size_t i = 0; i < m; i++)
        ಪ್ರತಿ(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
copy_matrix(ಧ್ರುವ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ಪ್ರತಿ(size_t i = 0; i < m; i++)
        ಪ್ರತಿ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
set_ಮೌಲ್ಯ_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    ಪ್ರತಿ(size_t i = 0; i < m; i++)
        ಪ್ರತಿ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, fptype* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ* info)
{
#ಯದಿ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / ಪರಿಮಾಣ(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#ಅನ್ಯಥಾ
    cv::AutoBuffer<ಪೂರ್ಣಾಙ್ಕ> piv_buff(m);
    ಪೂರ್ಣಾಙ್ಕ lda = (ಪೂರ್ಣಾಙ್ಕ)(a_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ _m = m, _n = n;
    ಪೂರ್ಣಾಙ್ಕ* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    ಯದಿ(b)
    {
        ಯದಿ(n == 1 && b_step == ಪರಿಮಾಣ(fptype))
        {
            ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
                sgesv_(&_m, &_n, (ಭಿನ್ನ*)a, &lda, piv, (ಭಿನ್ನ*)b, &_m, _info);
            ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                dgesv_(&_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, piv, (ದ್ವಿಗುಣ*)b, &_m, _info);
        }
        ಅನ್ಯಥಾ
        {
            ಪೂರ್ಣಾಙ್ಕ ldb = (ಪೂರ್ಣಾಙ್ಕ)(b_step / ಪರಿಮಾಣ(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
                sgesv_(&_m, &_n, (ಭಿನ್ನ*)a, &lda, piv, (ಭಿನ್ನ*)&tmpB[0], &_m, _info);
            ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                dgesv_(&_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, piv, (ದ್ವಿಗುಣ*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
            sgetrf_(&_m, &_m, (ಭಿನ್ನ*)a, &lda, piv, _info);
        ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
            dgetrf_(&_m, &_m, (ದ್ವಿಗುಣ*)a, &lda, piv, _info);
    }

#ಯದಿ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ಪೂರ್ಣಾಙ್ಕ>(_info[0]);
#endif
    ಪೂರ್ಣಾಙ್ಕ retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    ಪೂರ್ಣಾಙ್ಕ sign = 0;
    ಯದಿ(*info == 0)
    {
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    ಅನ್ಯಥಾ
        *info = 0; //in opencv LU function zero means error

    ಪ್ರತ್ಯಾಗಮ retcode;
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
lapack_Cholesky(fptype* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, fptype* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ತಾರ್ಕಿಕ* info)
{
#ಯದಿ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / ಪರಿಮಾಣ(fptype));
#ಅನ್ಯಥಾ
    ಪೂರ್ಣಾಙ್ಕ _m = m, _n = n;
    ಪೂರ್ಣಾಙ್ಕ lapackStatus = 0;
    ಪೂರ್ಣಾಙ್ಕ lda = (ಪೂರ್ಣಾಙ್ಕ)(a_step / ಪರಿಮಾಣ(fptype));
#endif
    ವರ್ಣ L[] = {'L', '\0'};

    ಯದಿ(b)
    {
        ಯದಿ(n == 1 && b_step == ಪರಿಮಾಣ(fptype))
        {
            ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ಭಿನ್ನ*)a, &lda, (ಭಿನ್ನ*)b, &_m, &lapackStatus);
            ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, (ದ್ವಿಗುಣ*)b, &_m, &lapackStatus);
        }
        ಅನ್ಯಥಾ
        {
            ಪೂರ್ಣಾಙ್ಕ ldb = (ಪೂರ್ಣಾಙ್ಕ)(b_step / ಪರಿಮಾಣ(fptype));
            fptype* tmpB = ನೂತನ fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ಭಿನ್ನ*)a, &lda, (ಭಿನ್ನ*)tmpB, &_m, &lapackStatus);
            ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, (ದ್ವಿಗುಣ*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (ಭಿನ್ನ*)a, &lda, &lapackStatus);
        ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (ದ್ವಿಗುಣ*)a, &lda, &lapackStatus);
    }

    ಯದಿ(lapackStatus == 0) *info = ಸತ್ಯ;
    ಅನ್ಯಥಾ *info = ಅಸತ್ಯ; //in opencv Cholesky function false means error

    ಪ್ರತ್ಯಾಗಮ lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ* info)
{
#ಯದಿ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / ಪರಿಮಾಣ(fptype));
    long ldv = (long)(v_step / ಪರಿಮಾಣ(fptype));
    long ldu = (long)(u_step / ಪರಿಮಾಣ(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#ಅನ್ಯಥಾ
    ಪೂರ್ಣಾಙ್ಕ _m = m, _n = n;
    ಪೂರ್ಣಾಙ್ಕ* _info = info;
    ಪೂರ್ಣಾಙ್ಕ lda = (ಪೂರ್ಣಾಙ್ಕ)(a_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ ldv = (ಪೂರ್ಣಾಙ್ಕ)(v_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ ldu = (ಪೂರ್ಣಾಙ್ಕ)(u_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ lwork = -1;
    cv::AutoBuffer<ಪೂರ್ಣಾಙ್ಕ> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    ವರ್ಣ mode[] = { ' ', '\0'};
    ಯದಿ(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    ಅನ್ಯಥಾ ಯದಿ((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    ಅನ್ಯಥಾ ಯದಿ((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    ಅನ್ಯಥಾ ಯದಿ(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    ಯದಿ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (ಭಿನ್ನ*)a, &lda, (ಭಿನ್ನ*)w, (ಭಿನ್ನ*)u, &ldu, (ಭಿನ್ನ*)vt, &ldv, (ಭಿನ್ನ*)&work1, &lwork, iworkBuf, _info);
    ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, (ದ್ವಿಗುಣ*)w, (ದ್ವಿಗುಣ*)u, &ldu, (ದ್ವಿಗುಣ*)vt, &ldv, (ದ್ವಿಗುಣ*)&work1, &lwork, iworkBuf, _info);

    ಯದಿ(*info < 0)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (ಪೂರ್ಣಾಙ್ಕ)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), ಪರಿಮಾಣ(fptype) * (lwork + 1));

    ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (ಭಿನ್ನ*)a, &lda, (ಭಿನ್ನ*)w, (ಭಿನ್ನ*)u, &ldu, (ಭಿನ್ನ*)vt, &ldv, (ಭಿನ್ನ*)&buffer[0], &lwork, iworkBuf, _info);
    ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (ದ್ವಿಗುಣ*)a, &lda, (ದ್ವಿಗುಣ*)w, (ದ್ವಿಗುಣ*)u, &ldu, (ದ್ವಿಗುಣ*)vt, &ldv, (ದ್ವಿಗುಣ*)&buffer[0], &lwork, iworkBuf, _info);

#ಯದಿ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ಪೂರ್ಣಾಙ್ಕ>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    ಯದಿ (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    ಯದಿ (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    ಯದಿ (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, ಪರಿಮಾಣ(fptype) * std::min(m, n));

    ಯದಿ(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    ಯದಿ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < m; i++)
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    ಯದಿ(*info < 0)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_OK;
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
lapack_QR(fptype* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, fptype* b, size_t b_step, fptype* dst, ಪೂರ್ಣಾಙ್ಕ* info)
{
#ಯದಿ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / ಪರಿಮಾಣ(fptype));
    long lwork = -1;
    long ldtmpA;
#ಅನ್ಯಥಾ
    ಪೂರ್ಣಾಙ್ಕ _m = m, _n = n, _k = k;
    ಪೂರ್ಣಾಙ್ಕ* _info = info;
    ಪೂರ್ಣಾಙ್ಕ lda = (ಪೂರ್ಣಾಙ್ಕ)(a_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ lwork = -1;
    ಪೂರ್ಣಾಙ್ಕ ldtmpA;
#endif

    ವರ್ಣ mode[] = { 'N', '\0' };
    ಯದಿ(m < n)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    ಯದಿ (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    ಅನ್ಯಥಾ
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    ಯದಿ (b)
    {
        ಯದಿ (k == 1 && b_step == ಪರಿಮಾಣ(fptype))
        {
            ಯದಿ (typeid(fptype) == typeid(ಭಿನ್ನ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ಭಿನ್ನ*)tmpA, &ldtmpA, (ಭಿನ್ನ*)b, &_m, (ಭಿನ್ನ*)&work1, &lwork, _info);
            ಅನ್ಯಥಾ ಯದಿ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)b, &_m, (ದ್ವಿಗುಣ*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
            ಯದಿ (*info < 0)
                ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ಯದಿ (typeid(fptype) == typeid(ಭಿನ್ನ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ಭಿನ್ನ*)tmpA, &ldtmpA, (ಭಿನ್ನ*)b, &_m, (ಭಿನ್ನ*)buffer, &lwork, _info);
            ಅನ್ಯಥಾ ಯದಿ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)b, &_m, (ದ್ವಿಗುಣ*)buffer, &lwork, _info);
        }
        ಅನ್ಯಥಾ
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            ಪೂರ್ಣಾಙ್ಕ ldb = (ಪೂರ್ಣಾಙ್ಕ)(b_step / ಪರಿಮಾಣ(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            ಯದಿ (typeid(fptype) == typeid(ಭಿನ್ನ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ಭಿನ್ನ*)tmpA, &ldtmpA, (ಭಿನ್ನ*)tmpB, &_m, (ಭಿನ್ನ*)&work1, &lwork, _info);
            ಅನ್ಯಥಾ ಯದಿ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)tmpB, &_m, (ದ್ವಿಗುಣ*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
            ಯದಿ (*info < 0)
                ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ಯದಿ (typeid(fptype) == typeid(ಭಿನ್ನ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ಭಿನ್ನ*)tmpA, &ldtmpA, (ಭಿನ್ನ*)tmpB, &_m, (ಭಿನ್ನ*)buffer, &lwork, _info);
            ಅನ್ಯಥಾ ಯದಿ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)tmpB, &_m, (ದ್ವಿಗುಣ*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (typeid(fptype) == typeid(ಭಿನ್ನ))
            sgeqrf_(&_m, &_n, (ಭಿನ್ನ*)tmpA, &ldtmpA, (ಭಿನ್ನ*)dst, (ಭಿನ್ನ*)&work1, &lwork, _info);
        ಅನ್ಯಥಾ ಯದಿ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
            dgeqrf_(&_m, &_n, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)dst, (ದ್ವಿಗುಣ*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
        ಯದಿ (*info < 0)
            ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        ಯದಿ (typeid(fptype) == typeid(ಭಿನ್ನ))
            sgeqrf_(&_m, &_n, (ಭಿನ್ನ*)tmpA, &ldtmpA, (ಭಿನ್ನ*)dst, (ಭಿನ್ನ*)buffer, &lwork, _info);
        ಅನ್ಯಥಾ ಯದಿ (typeid(fptype) == typeid(ದ್ವಿಗುಣ))
            dgeqrf_(&_m, &_n, (ದ್ವಿಗುಣ*)tmpA, &ldtmpA, (ದ್ವಿಗುಣ*)dst, (ದ್ವಿಗುಣ*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    ಯದಿ (m == n)
        transpose_square_inplace(a, lda, m);
    ಅನ್ಯಥಾ
        transpose(tmpA, m, a, lda, n, m);

#ಯದಿ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ಪೂರ್ಣಾಙ್ಕ>(_info[0]);
#endif

    ಯದಿ (*info != 0)
        *info = 0;
    ಅನ್ಯಥಾ
        *info = 1;

    ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_OK;
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
lapack_gemm(ಧ್ರುವ fptype *src1, size_t src1_step, ಧ್ರುವ fptype *src2, size_t src2_step, fptype alpha,
            ಧ್ರುವ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ಪೂರ್ಣಾಙ್ಕ a_m, ಪೂರ್ಣಾಙ್ಕ a_n, ಪೂರ್ಣಾಙ್ಕ d_n, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಪೂರ್ಣಾಙ್ಕ ldsrc1 = (ಪೂರ್ಣಾಙ್ಕ)(src1_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ ldsrc2 = (ಪೂರ್ಣಾಙ್ಕ)(src2_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ ldsrc3 = (ಪೂರ್ಣಾಙ್ಕ)(src3_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ lddst = (ಪೂರ್ಣಾಙ್ಕ)(dst_step / ಪರಿಮಾಣ(fptype));
    ಪೂರ್ಣಾಙ್ಕ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    ಯದಿ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ಯದಿ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ಅನ್ಯಥಾ
        {
            d_m = a_m;
        }
    }
    ಅನ್ಯಥಾ
    {
        transB = CblasNoTrans;
        ಯದಿ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ಅನ್ಯಥಾ
        {
            d_m = a_m;
        }
    }

    ಯದಿ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ಅನ್ಯಥಾ
    {
        c_m = d_m;
        c_n = d_n;
    }

    ಯದಿ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ಅನ್ಯಥಾ
    {
        transA = CblasNoTrans;
    }

    ಯದಿ(src3 != dst && beta != 0.0 && src3_step != 0) {
        ಯದಿ(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        ಅನ್ಯಥಾ
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    ಅನ್ಯಥಾ ಯದಿ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಅನ್ಯಥಾ ಯದಿ(src3_step == 0 && beta != 0.0)
        set_ಮೌಲ್ಯ_2(dst, lddst, (fptype)0.0, d_m, d_n);

    ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ಭಿನ್ನ)alpha, (ಭಿನ್ನ*)src1, ldsrc1, (ಭಿನ್ನ*)src2, ldsrc2, (ಭಿನ್ನ)beta, (ಭಿನ್ನ*)dst, lddst);
    ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ದ್ವಿಗುಣ)alpha, (ದ್ವಿಗುಣ*)src1, ldsrc1, (ದ್ವಿಗುಣ*)src2, ldsrc2, (ದ್ವಿಗುಣ)beta, (ದ್ವಿಗುಣ*)dst, lddst);

    ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_OK;
}

template <typename fptype> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
lapack_gemm_c(ಧ್ರುವ fptype *src1, size_t src1_step, ಧ್ರುವ fptype *src2, size_t src2_step, fptype alpha,
            ಧ್ರುವ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ಪೂರ್ಣಾಙ್ಕ a_m, ಪೂರ್ಣಾಙ್ಕ a_n, ಪೂರ್ಣಾಙ್ಕ d_n, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಪೂರ್ಣಾಙ್ಕ ldsrc1 = (ಪೂರ್ಣಾಙ್ಕ)(src1_step / ಪರಿಮಾಣ(std::complex<fptype>));
    ಪೂರ್ಣಾಙ್ಕ ldsrc2 = (ಪೂರ್ಣಾಙ್ಕ)(src2_step / ಪರಿಮಾಣ(std::complex<fptype>));
    ಪೂರ್ಣಾಙ್ಕ ldsrc3 = (ಪೂರ್ಣಾಙ್ಕ)(src3_step / ಪರಿಮಾಣ(std::complex<fptype>));
    ಪೂರ್ಣಾಙ್ಕ lddst = (ಪೂರ್ಣಾಙ್ಕ)(dst_step / ಪರಿಮಾಣ(std::complex<fptype>));
    ಪೂರ್ಣಾಙ್ಕ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    ಯದಿ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ಯದಿ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ಅನ್ಯಥಾ
        {
            d_m = a_m;
        }
    }
    ಅನ್ಯಥಾ
    {
        transB = CblasNoTrans;
        ಯದಿ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ಅನ್ಯಥಾ
        {
            d_m = a_m;
        }
    }

    ಯದಿ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ಅನ್ಯಥಾ
    {
        c_m = d_m;
        c_n = d_n;
    }

    ಯದಿ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ಅನ್ಯಥಾ
    {
        transA = CblasNoTrans;
    }

    ಯದಿ(src3 != dst && beta != 0.0 && src3_step != 0) {
        ಯದಿ(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        ಅನ್ಯಥಾ
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    ಅನ್ಯಥಾ ಯದಿ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಅನ್ಯಥಾ ಯದಿ(src3_step == 0 && beta != 0.0)
        set_ಮೌಲ್ಯ_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#ಯದಿ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ಪೂರ್ಣಾಙ್ಕ M = a_m, N = d_n, K = a_n;
    ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ)) {
        auto src1_cast = (std::complex<ಭಿನ್ನ>*)(src1);
        auto src2_cast = (std::complex<ಭಿನ್ನ>*)(src2);
        auto dst_cast = (std::complex<ಭಿನ್ನ>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<ಭಿನ್ನ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ಭಿನ್ನ>*)&cBeta, dst_cast, ldc);
    }
    ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ)) {
        auto src1_cast = (std::complex<ದ್ವಿಗುಣ>*)(src1);
        auto src2_cast = (std::complex<ದ್ವಿಗುಣ>*)(src2);
        auto dst_cast = (std::complex<ದ್ವಿಗುಣ>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<ದ್ವಿಗುಣ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ದ್ವಿಗುಣ>*)&cBeta, dst_cast, ldc);
    }
#ಅನ್ಯಥಾ
    ಯದಿ(typeid(fptype) == typeid(ಭಿನ್ನ))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ಭಿನ್ನ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ಭಿನ್ನ*)src1, ldsrc1, (ಭಿನ್ನ*)src2, ldsrc2, (ಭಿನ್ನ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ಭಿನ್ನ*)dst, lddst);
    ಅನ್ಯಥಾ ಯದಿ(typeid(fptype) == typeid(ದ್ವಿಗುಣ))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ದ್ವಿಗುಣ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ದ್ವಿಗುಣ*)src1, ldsrc1, (ದ್ವಿಗುಣ*)src2, ldsrc2, (ದ್ವಿಗುಣ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ದ್ವಿಗುಣ*)dst, lddst);
#endif

    ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_OK;
}
ಪೂರ್ಣಾಙ್ಕ lapack_LU32f(ಭಿನ್ನ* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ* info)
{
    ಯದಿ(m < HAL_LU_SMALL_MATRIX_THRESH)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_LU(a, a_step, m, b, b_step, n, info);
}

ಪೂರ್ಣಾಙ್ಕ lapack_LU64f(ದ್ವಿಗುಣ* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ* info)
{
    ಯದಿ(m < HAL_LU_SMALL_MATRIX_THRESH)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_LU(a, a_step, m, b, b_step, n, info);
}

ಪೂರ್ಣಾಙ್ಕ lapack_Cholesky32f(ಭಿನ್ನ* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ತಾರ್ಕಿಕ *info)
{
    ಯದಿ(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

ಪೂರ್ಣಾಙ್ಕ lapack_Cholesky64f(ದ್ವಿಗುಣ* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ತಾರ್ಕಿಕ *info)
{
    ಯದಿ(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

ಪೂರ್ಣಾಙ್ಕ lapack_SVD32f(ಭಿನ್ನ* a, size_t a_step, ಭಿನ್ನ *w, ಭಿನ್ನ* u, size_t u_step, ಭಿನ್ನ* vt, size_t v_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಯದಿ(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪೂರ್ಣಾಙ್ಕ info = 0;
    ಪ್ರತ್ಯಾಗಮ lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

ಪೂರ್ಣಾಙ್ಕ lapack_SVD64f(ದ್ವಿಗುಣ* a, size_t a_step, ದ್ವಿಗುಣ *w, ದ್ವಿಗುಣ* u, size_t u_step, ದ್ವಿಗುಣ* vt, size_t v_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಯದಿ(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪೂರ್ಣಾಙ್ಕ info = 0;
    ಪ್ರತ್ಯಾಗಮ lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

ಪೂರ್ಣಾಙ್ಕ lapack_QR32f(ಭಿನ್ನ* src1, size_t src1_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಭಿನ್ನ* src2, size_t src2_step, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ* info)
{
    ಯದಿ (m < HAL_QR_SMALL_MATRIX_THRESH)
      ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

ಪೂರ್ಣಾಙ್ಕ lapack_QR64f(ದ್ವಿಗುಣ* src1, size_t src1_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ದ್ವಿಗುಣ* src2, size_t src2_step, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ* info)
{
    ಯದಿ (m < HAL_QR_SMALL_MATRIX_THRESH)
      ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

ಪೂರ್ಣಾಙ್ಕ lapack_gemm32f(ಧ್ರುವ ಭಿನ್ನ *src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ *src2, size_t src2_step, ಭಿನ್ನ alpha,
                   ಧ್ರುವ ಭಿನ್ನ *src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ *dst, size_t dst_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಯದಿ(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

ಪೂರ್ಣಾಙ್ಕ lapack_gemm64f(ಧ್ರುವ ದ್ವಿಗುಣ *src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ *src2, size_t src2_step, ದ್ವಿಗುಣ alpha,
                   ಧ್ರುವ ದ್ವಿಗುಣ *src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ *dst, size_t dst_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಯದಿ(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

ಪೂರ್ಣಾಙ್ಕ lapack_gemm32fc(ಧ್ರುವ ಭಿನ್ನ *src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ *src2, size_t src2_step, ಭಿನ್ನ alpha,
                   ಧ್ರುವ ಭಿನ್ನ *src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ *dst, size_t dst_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಯದಿ(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
ಪೂರ್ಣಾಙ್ಕ lapack_gemm64fc(ಧ್ರುವ ದ್ವಿಗುಣ *src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ *src2, size_t src2_step, ದ್ವಿಗುಣ alpha,
                   ಧ್ರುವ ದ್ವಿಗುಣ *src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ *dst, size_t dst_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಯದಿ(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ಪ್ರತ್ಯಾಗಮ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ಪ್ರತ್ಯಾಗಮ lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#ಯದಿ defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
