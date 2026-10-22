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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "hal_internal.hpp"

#ifdef HAVE_LAPACK

#ଅନ୍ତର୍ଭାଵ "opencv_lapack.h"

#ଅନ୍ତର୍ଭାଵ <cmath>
#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <typeinfo>
#ଅନ୍ତର୍ଭାଵ <limits>
#ଅନ୍ତର୍ଭାଵ <complex>
#ଅନ୍ତର୍ଭାଵ <vector>

#define HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#define HAL_GEMM_SMALL_MATRIX_THRESH 100
#define HAL_SVD_SMALL_MATRIX_THRESH 25
#define HAL_QR_SMALL_MATRIX_THRESH 30
#define HAL_LU_SMALL_MATRIX_THRESH 100
#define HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#ଯଦି defined(__clang__) && defined(__has_feature)
#ଯଦି __has_feature(memory_sanitizer)
#ଅନ୍ତର୍ଭାଵ <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) କର { } ଯେତେବେଳେ(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#ଯଦି defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
transpose_square_inplace(fptype *src, size_t src_ld, size_t m)
{
    ପାଇଁ(size_t i = 0; i < m - 1; i++)
        ପାଇଁ(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
transpose(ଅଚଳ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ପାଇଁ(size_t i = 0; i < m; i++)
        ପାଇଁ(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
copy_matrix(ଅଚଳ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ପାଇଁ(size_t i = 0; i < m; i++)
        ପାଇଁ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
set_ମୂଲ୍ୟ_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    ପାଇଁ(size_t i = 0; i < m; i++)
        ପାଇଁ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, size_t a_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, fptype* b, size_t b_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* info)
{
#ଯଦି defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<long> piv_buff(m);
    long lda = (long)(a_step / ଆକାର(fptype));
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
#ଅନ୍ୟଥା
    cv::AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> piv_buff(m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(a_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _m = m, _n = n;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _info = info;
#endif
    auto piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    ଯଦି(b)
    {
        ଯଦି(n == 1 && b_step == ଆକାର(fptype))
        {
            ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
                sgesv_(&_m, &_n, (ଦଶମିକ*)a, &lda, piv, (ଦଶମିକ*)b, &_m, _info);
            ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
                dgesv_(&_m, &_n, (ଦ୍ୱିଗୁଣ*)a, &lda, piv, (ଦ୍ୱିଗୁଣ*)b, &_m, _info);
        }
        ଅନ୍ୟଥା
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(b_step / ଆକାର(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
                sgesv_(&_m, &_n, (ଦଶମିକ*)a, &lda, piv, (ଦଶମିକ*)&tmpB[0], &_m, _info);
            ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
                dgesv_(&_m, &_n, (ଦ୍ୱିଗୁଣ*)a, &lda, piv, (ଦ୍ୱିଗୁଣ*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    ଅନ୍ୟଥା
    {
        ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
            sgetrf_(&_m, &_m, (ଦଶମିକ*)a, &lda, piv, _info);
        ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
            dgetrf_(&_m, &_m, (ଦ୍ୱିଗୁଣ*)a, &lda, piv, _info);
    }

#ଯଦି defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(_info[0]);
#endif
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sign = 0;
    ଯଦି(*info == 0)
    {
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    ଅନ୍ୟଥା
        *info = 0; //in opencv LU function zero means error

    ଫେରନ୍ତୁ retcode;
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
lapack_Cholesky(fptype* a, size_t a_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, fptype* b, size_t b_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ତାର୍କିକ* info)
{
#ଯଦି defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long lapackStatus = 0;
    long lda = (long)(a_step / ଆକାର(fptype));
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _m = m, _n = n;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapackStatus = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(a_step / ଆକାର(fptype));
#endif
    ଅକ୍ଷର L[] = {'L', '\0'};

    ଯଦି(b)
    {
        ଯଦି(n == 1 && b_step == ଆକାର(fptype))
        {
            ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ଦଶମିକ*)a, &lda, (ଦଶମିକ*)b, &_m, &lapackStatus);
            ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ଦ୍ୱିଗୁଣ*)a, &lda, (ଦ୍ୱିଗୁଣ*)b, &_m, &lapackStatus);
        }
        ଅନ୍ୟଥା
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(b_step / ଆକାର(fptype));
            fptype* tmpB = ନୂତନ fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ଦଶମିକ*)a, &lda, (ଦଶମିକ*)tmpB, &_m, &lapackStatus);
            ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ଦ୍ୱିଗୁଣ*)a, &lda, (ଦ୍ୱିଗୁଣ*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    ଅନ୍ୟଥା
    {
        ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (ଦଶମିକ*)a, &lda, &lapackStatus);
        ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (ଦ୍ୱିଗୁଣ*)a, &lda, &lapackStatus);
    }

    ଯଦି(lapackStatus == 0) *info = ସତ୍ୟ;
    ଅନ୍ୟଥା *info = ମିଥ୍ୟା; //in opencv Cholesky function false means error

    ଫେରନ୍ତୁ lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
lapack_SVD(fptype* a, size_t a_step, fptype *w, fptype* u, size_t u_step, fptype* vt, size_t v_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* info)
{
#ଯଦି defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n);
    long _info[1];
    long lda = (long)(a_step / ଆକାର(fptype));
    long ldv = (long)(v_step / ଆକାର(fptype));
    long ldu = (long)(u_step / ଆକାର(fptype));
    long lwork = -1;
    cv::AutoBuffer<long> iworkBuf_(8 * std::min(m, n));
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _m = m, _n = n;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _info = info;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(a_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldv = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(v_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldu = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(u_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lwork = -1;
    cv::AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> iworkBuf_(8 * std::min(m, n));
#endif
    auto iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    ଅକ୍ଷର mode[] = { ' ', '\0'};
    ଯଦି(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    ଅନ୍ୟଥା ଯଦି((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    ଅନ୍ୟଥା ଯଦି((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    ଅନ୍ୟଥା ଯଦି(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    ଯଦି((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (ଦଶମିକ*)a, &lda, (ଦଶମିକ*)w, (ଦଶମିକ*)u, &ldu, (ଦଶମିକ*)vt, &ldv, (ଦଶମିକ*)&work1, &lwork, iworkBuf, _info);
    ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (ଦ୍ୱିଗୁଣ*)a, &lda, (ଦ୍ୱିଗୁଣ*)w, (ଦ୍ୱିଗୁଣ*)u, &ldu, (ଦ୍ୱିଗୁଣ*)vt, &ldv, (ଦ୍ୱିଗୁଣ*)&work1, &lwork, iworkBuf, _info);

    ଯଦି(*info < 0)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), ଆକାର(fptype) * (lwork + 1));

    ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (ଦଶମିକ*)a, &lda, (ଦଶମିକ*)w, (ଦଶମିକ*)u, &ldu, (ଦଶମିକ*)vt, &ldv, (ଦଶମିକ*)&buffer[0], &lwork, iworkBuf, _info);
    ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (ଦ୍ୱିଗୁଣ*)a, &lda, (ଦ୍ୱିଗୁଣ*)w, (ଦ୍ୱିଗୁଣ*)u, &ldu, (ଦ୍ୱିଗୁଣ*)vt, &ldv, (ଦ୍ୱିଗୁଣ*)&buffer[0], &lwork, iworkBuf, _info);

#ଯଦି defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    ଯଦି (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    ଯଦି (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    ଯଦି (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, ଆକାର(fptype) * std::min(m, n));

    ଯଦି(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    ଯଦି((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m; i++)
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    ଯଦି(*info < 0)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ CV_HAL_ERROR_OK;
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
lapack_QR(fptype* a, size_t a_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, fptype* b, size_t b_step, fptype* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* info)
{
#ଯଦି defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    long _m = static_cast<long>(m), _n = static_cast<long>(n), _k = static_cast<long>(k);
    long _info[1];
    long lda = (long)(a_step / ଆକାର(fptype));
    long lwork = -1;
    long ldtmpA;
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _m = m, _n = n, _k = k;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _info = info;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(a_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lwork = -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldtmpA;
#endif

    ଅକ୍ଷର mode[] = { 'N', '\0' };
    ଯଦି(m < n)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    ଯଦି (m == n)
    {
        transpose_square_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    ଅନ୍ୟଥା
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    ଯଦି (b)
    {
        ଯଦି (k == 1 && b_step == ଆକାର(fptype))
        {
            ଯଦି (typeid(fptype) == typeid(ଦଶମିକ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ଦଶମିକ*)tmpA, &ldtmpA, (ଦଶମିକ*)b, &_m, (ଦଶମିକ*)&work1, &lwork, _info);
            ଅନ୍ୟଥା ଯଦି (typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ଦ୍ୱିଗୁଣ*)tmpA, &ldtmpA, (ଦ୍ୱିଗୁଣ*)b, &_m, (ଦ୍ୱିଗୁଣ*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
            ଯଦି (*info < 0)
                ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ଯଦି (typeid(fptype) == typeid(ଦଶମିକ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ଦଶମିକ*)tmpA, &ldtmpA, (ଦଶମିକ*)b, &_m, (ଦଶମିକ*)buffer, &lwork, _info);
            ଅନ୍ୟଥା ଯଦି (typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ଦ୍ୱିଗୁଣ*)tmpA, &ldtmpA, (ଦ୍ୱିଗୁଣ*)b, &_m, (ଦ୍ୱିଗୁଣ*)buffer, &lwork, _info);
        }
        ଅନ୍ୟଥା
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(b_step / ଆକାର(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            ଯଦି (typeid(fptype) == typeid(ଦଶମିକ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ଦଶମିକ*)tmpA, &ldtmpA, (ଦଶମିକ*)tmpB, &_m, (ଦଶମିକ*)&work1, &lwork, _info);
            ଅନ୍ୟଥା ଯଦି (typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ଦ୍ୱିଗୁଣ*)tmpA, &ldtmpA, (ଦ୍ୱିଗୁଣ*)tmpB, &_m, (ଦ୍ୱିଗୁଣ*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
            ଯଦି (*info < 0)
                ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            ଯଦି (typeid(fptype) == typeid(ଦଶମିକ))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (ଦଶମିକ*)tmpA, &ldtmpA, (ଦଶମିକ*)tmpB, &_m, (ଦଶମିକ*)buffer, &lwork, _info);
            ଅନ୍ୟଥା ଯଦି (typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (ଦ୍ୱିଗୁଣ*)tmpA, &ldtmpA, (ଦ୍ୱିଗୁଣ*)tmpB, &_m, (ଦ୍ୱିଗୁଣ*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (typeid(fptype) == typeid(ଦଶମିକ))
            sgeqrf_(&_m, &_n, (ଦଶମିକ*)tmpA, &ldtmpA, (ଦଶମିକ*)dst, (ଦଶମିକ*)&work1, &lwork, _info);
        ଅନ୍ୟଥା ଯଦି (typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
            dgeqrf_(&_m, &_n, (ଦ୍ୱିଗୁଣ*)tmpA, &ldtmpA, (ଦ୍ୱିଗୁଣ*)dst, (ଦ୍ୱିଗୁଣ*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        ଯଦି (*info < 0)
            ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        ଯଦି (typeid(fptype) == typeid(ଦଶମିକ))
            sgeqrf_(&_m, &_n, (ଦଶମିକ*)tmpA, &ldtmpA, (ଦଶମିକ*)dst, (ଦଶମିକ*)buffer, &lwork, _info);
        ଅନ୍ୟଥା ଯଦି (typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
            dgeqrf_(&_m, &_n, (ଦ୍ୱିଗୁଣ*)tmpA, &ldtmpA, (ଦ୍ୱିଗୁଣ*)dst, (ଦ୍ୱିଗୁଣ*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ଯଦି (m == n)
        transpose_square_inplace(a, lda, m);
    ଅନ୍ୟଥା
        transpose(tmpA, m, a, lda, n, m);

#ଯଦି defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(_info[0]);
#endif

    ଯଦି (*info != 0)
        *info = 0;
    ଅନ୍ୟଥା
        *info = 1;

    ଫେରନ୍ତୁ CV_HAL_ERROR_OK;
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
lapack_gemm(ଅଚଳ fptype *src1, size_t src1_step, ଅଚଳ fptype *src2, size_t src2_step, fptype alpha,
            ଅଚଳ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a_m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a_n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା d_n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldsrc1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src1_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldsrc2 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src2_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldsrc3 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src3_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lddst = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(dst_step / ଆକାର(fptype));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    ଯଦି(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ଯଦି(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ଅନ୍ୟଥା
        {
            d_m = a_m;
        }
    }
    ଅନ୍ୟଥା
    {
        transB = CblasNoTrans;
        ଯଦି(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ଅନ୍ୟଥା
        {
            d_m = a_m;
        }
    }

    ଯଦି(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ଅନ୍ୟଥା
    {
        c_m = d_m;
        c_n = d_n;
    }

    ଯଦି(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ଅନ୍ୟଥା
    {
        transA = CblasNoTrans;
    }

    ଯଦି(src3 != dst && beta != 0.0 && src3_step != 0) {
        ଯଦି(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        ଅନ୍ୟଥା
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    ଅନ୍ୟଥା ଯଦି (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଅନ୍ୟଥା ଯଦି(src3_step == 0 && beta != 0.0)
        set_ମୂଲ୍ୟ_2(dst, lddst, (fptype)0.0, d_m, d_n);

    ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ଦଶମିକ)alpha, (ଦଶମିକ*)src1, ldsrc1, (ଦଶମିକ*)src2, ldsrc2, (ଦଶମିକ)beta, (ଦଶମିକ*)dst, lddst);
    ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ଦ୍ୱିଗୁଣ)alpha, (ଦ୍ୱିଗୁଣ*)src1, ldsrc1, (ଦ୍ୱିଗୁଣ*)src2, ldsrc2, (ଦ୍ୱିଗୁଣ)beta, (ଦ୍ୱିଗୁଣ*)dst, lddst);

    ଫେରନ୍ତୁ CV_HAL_ERROR_OK;
}

template <typename fptype> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
lapack_gemm_c(ଅଚଳ fptype *src1, size_t src1_step, ଅଚଳ fptype *src2, size_t src2_step, fptype alpha,
            ଅଚଳ fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a_m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା a_n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା d_n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldsrc1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src1_step / ଆକାର(std::complex<fptype>));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldsrc2 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src2_step / ଆକାର(std::complex<fptype>));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldsrc3 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src3_step / ଆକାର(std::complex<fptype>));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lddst = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(dst_step / ଆକାର(std::complex<fptype>));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    ଯଦି(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ଯଦି(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ଅନ୍ୟଥା
        {
            d_m = a_m;
        }
    }
    ଅନ୍ୟଥା
    {
        transB = CblasNoTrans;
        ଯଦି(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ଅନ୍ୟଥା
        {
            d_m = a_m;
        }
    }

    ଯଦି(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ଅନ୍ୟଥା
    {
        c_m = d_m;
        c_n = d_n;
    }

    ଯଦି(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ଅନ୍ୟଥା
    {
        transA = CblasNoTrans;
    }

    ଯଦି(src3 != dst && beta != 0.0 && src3_step != 0) {
        ଯଦି(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        ଅନ୍ୟଥା
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    ଅନ୍ୟଥା ଯଦି (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଅନ୍ୟଥା ଯଦି(src3_step == 0 && beta != 0.0)
        set_ମୂଲ୍ୟ_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#ଯଦି defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା M = a_m, N = d_n, K = a_n;
    ଯଦି(typeid(fptype) == typeid(ଦଶମିକ)) {
        auto src1_cast = (std::complex<ଦଶମିକ>*)(src1);
        auto src2_cast = (std::complex<ଦଶମିକ>*)(src2);
        auto dst_cast = (std::complex<ଦଶମିକ>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<ଦଶମିକ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ଦଶମିକ>*)&cBeta, dst_cast, ldc);
    }
    ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ)) {
        auto src1_cast = (std::complex<ଦ୍ୱିଗୁଣ>*)(src1);
        auto src2_cast = (std::complex<ଦ୍ୱିଗୁଣ>*)(src2);
        auto dst_cast = (std::complex<ଦ୍ୱିଗୁଣ>*)(dst);
        long lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<ଦ୍ୱିଗୁଣ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ଦ୍ୱିଗୁଣ>*)&cBeta, dst_cast, ldc);
    }
#ଅନ୍ୟଥା
    ଯଦି(typeid(fptype) == typeid(ଦଶମିକ))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ଦଶମିକ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ଦଶମିକ*)src1, ldsrc1, (ଦଶମିକ*)src2, ldsrc2, (ଦଶମିକ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ଦଶମିକ*)dst, lddst);
    ଅନ୍ୟଥା ଯଦି(typeid(fptype) == typeid(ଦ୍ୱିଗୁଣ))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (ଦ୍ୱିଗୁଣ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ଦ୍ୱିଗୁଣ*)src1, ldsrc1, (ଦ୍ୱିଗୁଣ*)src2, ldsrc2, (ଦ୍ୱିଗୁଣ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ଦ୍ୱିଗୁଣ*)dst, lddst);
#endif

    ଫେରନ୍ତୁ CV_HAL_ERROR_OK;
}
ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_LU32f(ଦଶମିକ* a, size_t a_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦଶମିକ* b, size_t b_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* info)
{
    ଯଦି(m < HAL_LU_SMALL_MATRIX_THRESH)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_LU(a, a_step, m, b, b_step, n, info);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_LU64f(ଦ୍ୱିଗୁଣ* a, size_t a_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ* b, size_t b_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* info)
{
    ଯଦି(m < HAL_LU_SMALL_MATRIX_THRESH)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_LU(a, a_step, m, b, b_step, n, info);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_Cholesky32f(ଦଶମିକ* a, size_t a_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦଶମିକ* b, size_t b_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ତାର୍କିକ *info)
{
    ଯଦି(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_Cholesky64f(ଦ୍ୱିଗୁଣ* a, size_t a_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଦ୍ୱିଗୁଣ* b, size_t b_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ତାର୍କିକ *info)
{
    ଯଦି(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_SVD32f(ଦଶମିକ* a, size_t a_step, ଦଶମିକ *w, ଦଶମିକ* u, size_t u_step, ଦଶମିକ* vt, size_t v_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଯଦି(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା info = 0;
    ଫେରନ୍ତୁ lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_SVD64f(ଦ୍ୱିଗୁଣ* a, size_t a_step, ଦ୍ୱିଗୁଣ *w, ଦ୍ୱିଗୁଣ* u, size_t u_step, ଦ୍ୱିଗୁଣ* vt, size_t v_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଯଦି(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା info = 0;
    ଫେରନ୍ତୁ lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_QR32f(ଦଶମିକ* src1, size_t src1_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ଦଶମିକ* src2, size_t src2_step, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* info)
{
    ଯଦି (m < HAL_QR_SMALL_MATRIX_THRESH)
      ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_QR64f(ଦ୍ୱିଗୁଣ* src1, size_t src1_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ଦ୍ୱିଗୁଣ* src2, size_t src2_step, ଦ୍ୱିଗୁଣ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* info)
{
    ଯଦି (m < HAL_QR_SMALL_MATRIX_THRESH)
      ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_gemm32f(ଅଚଳ ଦଶମିକ *src1, size_t src1_step, ଅଚଳ ଦଶମିକ *src2, size_t src2_step, ଦଶମିକ alpha,
                   ଅଚଳ ଦଶମିକ *src3, size_t src3_step, ଦଶମିକ beta, ଦଶମିକ *dst, size_t dst_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଯଦି(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_gemm64f(ଅଚଳ ଦ୍ୱିଗୁଣ *src1, size_t src1_step, ଅଚଳ ଦ୍ୱିଗୁଣ *src2, size_t src2_step, ଦ୍ୱିଗୁଣ alpha,
                   ଅଚଳ ଦ୍ୱିଗୁଣ *src3, size_t src3_step, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ *dst, size_t dst_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଯଦି(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_gemm32fc(ଅଚଳ ଦଶମିକ *src1, size_t src1_step, ଅଚଳ ଦଶମିକ *src2, size_t src2_step, ଦଶମିକ alpha,
                   ଅଚଳ ଦଶମିକ *src3, size_t src3_step, ଦଶମିକ beta, ଦଶମିକ *dst, size_t dst_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଯଦି(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
ପୂର୍ଣ୍ଣସଂଖ୍ୟା lapack_gemm64fc(ଅଚଳ ଦ୍ୱିଗୁଣ *src1, size_t src1_step, ଅଚଳ ଦ୍ୱିଗୁଣ *src2, size_t src2_step, ଦ୍ୱିଗୁଣ alpha,
                   ଅଚଳ ଦ୍ୱିଗୁଣ *src3, size_t src3_step, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ *dst, size_t dst_step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଯଦି(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        ଫେରନ୍ତୁ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ଫେରନ୍ତୁ lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#ଯଦି defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
