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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal_internal.hpp"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LAPACK

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv_lapack.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <typeinfo>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits>
#ਅਨ੍ਤਰ੍ਭਾਵ <complex>
#ਅਨ੍ਤਰ੍ਭਾਵ <vector>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAL_GEMM_SMALL_COMPLEX_ਮੈਟ੍ਰਿਕਸ_THRESH 100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAL_GEMM_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH 100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAL_SVD_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH 25
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAL_QR_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH 30
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAL_LU_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH 100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAL_CHOLESKY_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH 100

#ਜੇ defined(__clang__) && defined(__has_feature)
#ਜੇ __has_feature(ਸਮ੍ਰਿਤੀ_sanitizer)
#ਅਨ੍ਤਰ੍ਭਾਵ <sanitizer/msan_interface.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_ANNOTATE_NO_SANITIZE_ਸਮ੍ਰਿਤੀ __attribute__((no_sanitize("memory")))
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(address, size) ਕਰੋ { } ਜਦੋਂ(0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_ANNOTATE_NO_SANITIZE_ਸਮ੍ਰਿਤੀ
#ਜੇ_ਅੰਤ

#ਜੇ defined(__APPLE__) && defined(__clang__)
#ਹਦਾਇਤ clang diagnostic push
#ਹਦਾਇਤ clang diagnostic ignored "-Wdeprecated-declarations"
#ਜੇ_ਅੰਤ

//lapack stores matrices in column-major order so transposing is needed everywhere
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
transpose_ਵਰਗਫਲ_inplace(fptype *src, size_t src_ld, size_t m)
{
    ਲਈ(size_t i = 0; i < m - 1; i++)
        ਲਈ(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
transpose(ਅਚਲ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ਲਈ(size_t i = 0; i < m; i++)
        ਲਈ(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
copy_ਮੈਟ੍ਰਿਕਸ(ਅਚਲ fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    ਲਈ(size_t i = 0; i < m; i++)
        ਲਈ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਸੈੱਟ_ਮੁੱਲ_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    ਲਈ(size_t i = 0; i < m; i++)
        ਲਈ(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
CV_ANNOTATE_NO_SANITIZE_ਸਮ੍ਰਿਤੀ lapack_LU(fptype* a, size_t a_ਕਦਮ, ਪੂਰਨ m, fptype* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਪੂਰਨ* info)
{
#ਜੇ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::Auto_ਬਫਰ<ਲੰਮਾ> piv_buff(m);
    ਲੰਮਾ lda = (ਲੰਮਾ)(a_ਕਦਮ / ਆਕਾਰ(fptype));
    ਲੰਮਾ _m = static_cast<ਲੰਮਾ>(m), _n = static_cast<ਲੰਮਾ>(n);
    ਲੰਮਾ _info[1];
#ਨਹੀਂਤਾਂ
    cv::Auto_ਬਫਰ<ਪੂਰਨ> piv_buff(m);
    ਪੂਰਨ lda = (ਪੂਰਨ)(a_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ _m = m, _n = n;
    ਪੂਰਨ* _info = info;
#ਜੇ_ਅੰਤ
    ਸਵੈਚਾਲਿਤ piv = piv_buff.ਅੰਕੜੇ();

    transpose_ਵਰਗਫਲ_inplace(a, lda, m);

    ਜੇ(b)
    {
        ਜੇ(n == 1 && b_ਕਦਮ == ਆਕਾਰ(fptype))
        {
            ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
                sgesv_(&_m, &_n, (ਦਸ਼ਮਲਵ*)a, &lda, piv, (ਦਸ਼ਮਲਵ*)b, &_m, _info);
            ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
                dgesv_(&_m, &_n, (ਦੁੱਗਣਾ*)a, &lda, piv, (ਦੁੱਗਣਾ*)b, &_m, _info);
        }
        ਨਹੀਂਤਾਂ
        {
            ਪੂਰਨ ldb = (ਪੂਰਨ)(b_ਕਦਮ / ਆਕਾਰ(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
                sgesv_(&_m, &_n, (ਦਸ਼ਮਲਵ*)a, &lda, piv, (ਦਸ਼ਮਲਵ*)&tmpB[0], &_m, _info);
            ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
                dgesv_(&_m, &_n, (ਦੁੱਗਣਾ*)a, &lda, piv, (ਦੁੱਗਣਾ*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
            sgetrf_(&_m, &_m, (ਦਸ਼ਮਲਵ*)a, &lda, piv, _info);
        ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
            dgetrf_(&_m, &_m, (ਦੁੱਗਣਾ*)a, &lda, piv, _info);
    }

#ਜੇ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ਪੂਰਨ>(_info[0]);
#ਜੇ_ਅੰਤ
    ਪੂਰਨ retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    ਪੂਰਨ sign = 0;
    ਜੇ(*info == 0)
    {
        ਲਈ(ਪੂਰਨ i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    ਨਹੀਂਤਾਂ
        *info = 0; //in opencv LU function zero means error

    ਵਾਪਸ retcode;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
lapack_Cholesky(fptype* a, size_t a_ਕਦਮ, ਪੂਰਨ m, fptype* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਤਾਰਕਿਕ* info)
{
#ਜੇ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ਲੰਮਾ _m = static_cast<ਲੰਮਾ>(m), _n = static_cast<ਲੰਮਾ>(n);
    ਲੰਮਾ lapack_ਸਥਿਤੀ = 0;
    ਲੰਮਾ lda = (ਲੰਮਾ)(a_ਕਦਮ / ਆਕਾਰ(fptype));
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ _m = m, _n = n;
    ਪੂਰਨ lapack_ਸਥਿਤੀ = 0;
    ਪੂਰਨ lda = (ਪੂਰਨ)(a_ਕਦਮ / ਆਕਾਰ(fptype));
#ਜੇ_ਅੰਤ
    ਅੱਖਰ L[] = {'L', '\0'};

    ਜੇ(b)
    {
        ਜੇ(n == 1 && b_ਕਦਮ == ਆਕਾਰ(fptype))
        {
            ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ਦਸ਼ਮਲਵ*)a, &lda, (ਦਸ਼ਮਲਵ*)b, &_m, &lapack_ਸਥਿਤੀ);
            ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ਦੁੱਗਣਾ*)a, &lda, (ਦੁੱਗਣਾ*)b, &_m, &lapack_ਸਥਿਤੀ);
        }
        ਨਹੀਂਤਾਂ
        {
            ਪੂਰਨ ldb = (ਪੂਰਨ)(b_ਕਦਮ / ਆਕਾਰ(fptype));
            fptype* tmpB = ਨਵਾਂ fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (ਦਸ਼ਮਲਵ*)a, &lda, (ਦਸ਼ਮਲਵ*)tmpB, &_m, &lapack_ਸਥਿਤੀ);
            ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (ਦੁੱਗਣਾ*)a, &lda, (ਦੁੱਗਣਾ*)tmpB, &_m, &lapack_ਸਥਿਤੀ);

            transpose(tmpB, m, b, ldb, n, m);
            ਮਿਟਾਓ[] tmpB;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (ਦਸ਼ਮਲਵ*)a, &lda, &lapack_ਸਥਿਤੀ);
        ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (ਦੁੱਗਣਾ*)a, &lda, &lapack_ਸਥਿਤੀ);
    }

    ਜੇ(lapack_ਸਥਿਤੀ == 0) *info = ਸੱਚ;
    ਨਹੀਂਤਾਂ *info = ਝੂਠ; //in opencv Cholesky function false means error

    ਵਾਪਸ lapack_ਸਥਿਤੀ >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
lapack_SVD(fptype* a, size_t a_ਕਦਮ, fptype *w, fptype* u, size_t u_ਕਦਮ, fptype* vt, size_t v_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ flags, ਪੂਰਨ* info)
{
#ਜੇ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ਲੰਮਾ _m = static_cast<ਲੰਮਾ>(m), _n = static_cast<ਲੰਮਾ>(n);
    ਲੰਮਾ _info[1];
    ਲੰਮਾ lda = (ਲੰਮਾ)(a_ਕਦਮ / ਆਕਾਰ(fptype));
    ਲੰਮਾ ldv = (ਲੰਮਾ)(v_ਕਦਮ / ਆਕਾਰ(fptype));
    ਲੰਮਾ ldu = (ਲੰਮਾ)(u_ਕਦਮ / ਆਕਾਰ(fptype));
    ਲੰਮਾ lwork = -1;
    cv::Auto_ਬਫਰ<ਲੰਮਾ> iworkBuf_(8 * std::min(m, n));
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ _m = m, _n = n;
    ਪੂਰਨ* _info = info;
    ਪੂਰਨ lda = (ਪੂਰਨ)(a_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ ldv = (ਪੂਰਨ)(v_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ ldu = (ਪੂਰਨ)(u_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ lwork = -1;
    cv::Auto_ਬਫਰ<ਪੂਰਨ> iworkBuf_(8 * std::min(m, n));
#ਜੇ_ਅੰਤ
    ਸਵੈਚਾਲਿਤ iworkBuf = iworkBuf_.ਅੰਕੜੇ();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    ਅੱਖਰ ਬਹੁਲਕ[] = { ' ', '\0'};
    ਜੇ(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        ਬਹੁਲਕ[0] = 'N';
    }
    ਨਹੀਂਤਾਂ ਜੇ((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        ਬਹੁਲਕ[0] = 'O';
    ਨਹੀਂਤਾਂ ਜੇ((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        ਬਹੁਲਕ[0] = 'S';
    ਨਹੀਂਤਾਂ ਜੇ(flags & CV_HAL_SVD_ਭਰਿਆ_UV) //full SVD, U stored in u or in a
        ਬਹੁਲਕ[0] = 'A';

    ਜੇ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_ਭਰਿਆ_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
        OCV_LAPACK_FUNC(sgesdd)(ਬਹੁਲਕ, &_m, &_n, (ਦਸ਼ਮਲਵ*)a, &lda, (ਦਸ਼ਮਲਵ*)w, (ਦਸ਼ਮਲਵ*)u, &ldu, (ਦਸ਼ਮਲਵ*)vt, &ldv, (ਦਸ਼ਮਲਵ*)&work1, &lwork, iworkBuf, _info);
    ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
        OCV_LAPACK_FUNC(dgesdd)(ਬਹੁਲਕ, &_m, &_n, (ਦੁੱਗਣਾ*)a, &lda, (ਦੁੱਗਣਾ*)w, (ਦੁੱਗਣਾ*)u, &ldu, (ਦੁੱਗਣਾ*)vt, &ldv, (ਦੁੱਗਣਾ*)&work1, &lwork, iworkBuf, _info);

    ਜੇ(*info < 0)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (ਪੂਰਨ)round(work1); //optimal buffer size
    std::vector<fptype> ਬਫਰ(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(ਬਫਰ.ਅੰਕੜੇ(), ਆਕਾਰ(fptype) * (lwork + 1));

    ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
        OCV_LAPACK_FUNC(sgesdd)(ਬਹੁਲਕ, &_m, &_n, (ਦਸ਼ਮਲਵ*)a, &lda, (ਦਸ਼ਮਲਵ*)w, (ਦਸ਼ਮਲਵ*)u, &ldu, (ਦਸ਼ਮਲਵ*)vt, &ldv, (ਦਸ਼ਮਲਵ*)&ਬਫਰ[0], &lwork, iworkBuf, _info);
    ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
        OCV_LAPACK_FUNC(dgesdd)(ਬਹੁਲਕ, &_m, &_n, (ਦੁੱਗਣਾ*)a, &lda, (ਦੁੱਗਣਾ*)w, (ਦੁੱਗਣਾ*)u, &ldu, (ਦੁੱਗਣਾ*)vt, &ldv, (ਦੁੱਗਣਾ*)&ਬਫਰ[0], &lwork, iworkBuf, _info);

#ਜੇ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ਪੂਰਨ>(_info[0]);
#ਜੇ_ਅੰਤ

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(a, a_ਕਦਮ * n);
    ਜੇ (u)
      CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(u, u_ਕਦਮ * m);
    ਜੇ (vt)
      CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(vt, v_ਕਦਮ * n);
    ਜੇ (w)
      CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(w, ਆਕਾਰ(fptype) * std::min(m, n));

    ਜੇ(!(flags & CV_HAL_SVD_NO_UV))
        transpose_ਵਰਗਫਲ_inplace(vt, ldv, n);

    ਜੇ((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_ਭਰਿਆ_UV))
    {
        ਲਈ(ਪੂਰਨ i = 0; i < m; i++)
            ਲਈ(ਪੂਰਨ j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    ਜੇ(*info < 0)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ CV_HAL_ERROR_OK;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
lapack_QR(fptype* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, fptype* b, size_t b_ਕਦਮ, fptype* dst, ਪੂਰਨ* info)
{
#ਜੇ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ਲੰਮਾ _m = static_cast<ਲੰਮਾ>(m), _n = static_cast<ਲੰਮਾ>(n), _k = static_cast<ਲੰਮਾ>(k);
    ਲੰਮਾ _info[1];
    ਲੰਮਾ lda = (ਲੰਮਾ)(a_ਕਦਮ / ਆਕਾਰ(fptype));
    ਲੰਮਾ lwork = -1;
    ਲੰਮਾ ldtmpA;
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ _m = m, _n = n, _k = k;
    ਪੂਰਨ* _info = info;
    ਪੂਰਨ lda = (ਪੂਰਨ)(a_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ lwork = -1;
    ਪੂਰਨ ldtmpA;
#ਜੇ_ਅੰਤ

    ਅੱਖਰ ਬਹੁਲਕ[] = { 'N', '\0' };
    ਜੇ(m < n)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    ਜੇ (m == n)
    {
        transpose_ਵਰਗਫਲ_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    ਨਹੀਂਤਾਂ
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    ਜੇ (b)
    {
        ਜੇ (k == 1 && b_ਕਦਮ == ਆਕਾਰ(fptype))
        {
            ਜੇ (typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
                OCV_LAPACK_FUNC(sgels)(ਬਹੁਲਕ, &_m, &_n, &_k, (ਦਸ਼ਮਲਵ*)tmpA, &ldtmpA, (ਦਸ਼ਮਲਵ*)b, &_m, (ਦਸ਼ਮਲਵ*)&work1, &lwork, _info);
            ਨਹੀਂਤਾਂ ਜੇ (typeid(fptype) == typeid(ਦੁੱਗਣਾ))
                OCV_LAPACK_FUNC(dgels)(ਬਹੁਲਕ, &_m, &_n, &_k, (ਦੁੱਗਣਾ*)tmpA, &ldtmpA, (ਦੁੱਗਣਾ*)b, &_m, (ਦੁੱਗਣਾ*)&work1, &lwork, _info);

            CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(info, ਆਕਾਰ(ਪੂਰਨ));
            ਜੇ (*info < 0)
                ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_ਗੇੜਾ(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* ਬਫਰ = &workBufMemHolder.front();

            ਜੇ (typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
                OCV_LAPACK_FUNC(sgels)(ਬਹੁਲਕ, &_m, &_n, &_k, (ਦਸ਼ਮਲਵ*)tmpA, &ldtmpA, (ਦਸ਼ਮਲਵ*)b, &_m, (ਦਸ਼ਮਲਵ*)ਬਫਰ, &lwork, _info);
            ਨਹੀਂਤਾਂ ਜੇ (typeid(fptype) == typeid(ਦੁੱਗਣਾ))
                OCV_LAPACK_FUNC(dgels)(ਬਹੁਲਕ, &_m, &_n, &_k, (ਦੁੱਗਣਾ*)tmpA, &ldtmpA, (ਦੁੱਗਣਾ*)b, &_m, (ਦੁੱਗਣਾ*)ਬਫਰ, &lwork, _info);
        }
        ਨਹੀਂਤਾਂ
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            ਪੂਰਨ ldb = (ਪੂਰਨ)(b_ਕਦਮ / ਆਕਾਰ(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            ਜੇ (typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
                OCV_LAPACK_FUNC(sgels)(ਬਹੁਲਕ, &_m, &_n, &_k, (ਦਸ਼ਮਲਵ*)tmpA, &ldtmpA, (ਦਸ਼ਮਲਵ*)tmpB, &_m, (ਦਸ਼ਮਲਵ*)&work1, &lwork, _info);
            ਨਹੀਂਤਾਂ ਜੇ (typeid(fptype) == typeid(ਦੁੱਗਣਾ))
                OCV_LAPACK_FUNC(dgels)(ਬਹੁਲਕ, &_m, &_n, &_k, (ਦੁੱਗਣਾ*)tmpA, &ldtmpA, (ਦੁੱਗਣਾ*)tmpB, &_m, (ਦੁੱਗਣਾ*)&work1, &lwork, _info);

            CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(info, ਆਕਾਰ(ਪੂਰਨ));
            ਜੇ (*info < 0)
                ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_ਗੇੜਾ(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* ਬਫਰ = &workBufMemHolder.front();

            ਜੇ (typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
                OCV_LAPACK_FUNC(sgels)(ਬਹੁਲਕ, &_m, &_n, &_k, (ਦਸ਼ਮਲਵ*)tmpA, &ldtmpA, (ਦਸ਼ਮਲਵ*)tmpB, &_m, (ਦਸ਼ਮਲਵ*)ਬਫਰ, &lwork, _info);
            ਨਹੀਂਤਾਂ ਜੇ (typeid(fptype) == typeid(ਦੁੱਗਣਾ))
                OCV_LAPACK_FUNC(dgels)(ਬਹੁਲਕ, &_m, &_n, &_k, (ਦੁੱਗਣਾ*)tmpA, &ldtmpA, (ਦੁੱਗਣਾ*)tmpB, &_m, (ਦੁੱਗਣਾ*)ਬਫਰ, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
            sgeqrf_(&_m, &_n, (ਦਸ਼ਮਲਵ*)tmpA, &ldtmpA, (ਦਸ਼ਮਲਵ*)dst, (ਦਸ਼ਮਲਵ*)&work1, &lwork, _info);
        ਨਹੀਂਤਾਂ ਜੇ (typeid(fptype) == typeid(ਦੁੱਗਣਾ))
            dgeqrf_(&_m, &_n, (ਦੁੱਗਣਾ*)tmpA, &ldtmpA, (ਦੁੱਗਣਾ*)dst, (ਦੁੱਗਣਾ*)&work1, &lwork, _info);

        CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(info, ਆਕਾਰ(ਪੂਰਨ));
        ਜੇ (*info < 0)
            ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cv_ਗੇੜਾ(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* ਬਫਰ = &workBufMemHolder.front();

        ਜੇ (typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
            sgeqrf_(&_m, &_n, (ਦਸ਼ਮਲਵ*)tmpA, &ldtmpA, (ਦਸ਼ਮਲਵ*)dst, (ਦਸ਼ਮਲਵ*)ਬਫਰ, &lwork, _info);
        ਨਹੀਂਤਾਂ ਜੇ (typeid(fptype) == typeid(ਦੁੱਗਣਾ))
            dgeqrf_(&_m, &_n, (ਦੁੱਗਣਾ*)tmpA, &ldtmpA, (ਦੁੱਗਣਾ*)dst, (ਦੁੱਗਣਾ*)ਬਫਰ, &lwork, _info);
    }

    CV_ANNOTATE_ਸਮ੍ਰਿਤੀ_IS_INITIALIZED(info, ਆਕਾਰ(ਪੂਰਨ));
    ਜੇ (m == n)
        transpose_ਵਰਗਫਲ_inplace(a, lda, m);
    ਨਹੀਂਤਾਂ
        transpose(tmpA, m, a, lda, n, m);

#ਜੇ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<ਪੂਰਨ>(_info[0]);
#ਜੇ_ਅੰਤ

    ਜੇ (*info != 0)
        *info = 0;
    ਨਹੀਂਤਾਂ
        *info = 1;

    ਵਾਪਸ CV_HAL_ERROR_OK;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
lapack_gemm(ਅਚਲ fptype *src1, size_t src1_ਕਦਮ, ਅਚਲ fptype *src2, size_t src2_ਕਦਮ, fptype alpha,
            ਅਚਲ fptype *src3, size_t src3_ਕਦਮ, fptype beta, fptype *dst, size_t dst_ਕਦਮ, ਪੂਰਨ a_m, ਪੂਰਨ a_n, ਪੂਰਨ d_n, ਪੂਰਨ flags)
{
    ਪੂਰਨ ldsrc1 = (ਪੂਰਨ)(src1_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ ldsrc2 = (ਪੂਰਨ)(src2_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ ldsrc3 = (ਪੂਰਨ)(src3_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ lddst = (ਪੂਰਨ)(dst_ਕਦਮ / ਆਕਾਰ(fptype));
    ਪੂਰਨ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    ਜੇ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ਜੇ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ਨਹੀਂਤਾਂ
        {
            d_m = a_m;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        transB = CblasNoTrans;
        ਜੇ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ਨਹੀਂਤਾਂ
        {
            d_m = a_m;
        }
    }

    ਜੇ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ਨਹੀਂਤਾਂ
    {
        c_m = d_m;
        c_n = d_n;
    }

    ਜੇ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ਨਹੀਂਤਾਂ
    {
        transA = CblasNoTrans;
    }

    ਜੇ(src3 != dst && beta != 0.0 && src3_ਕਦਮ != 0) {
        ਜੇ(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        ਨਹੀਂਤਾਂ
            copy_ਮੈਟ੍ਰਿਕਸ(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    ਨਹੀਂਤਾਂ ਜੇ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਨਹੀਂਤਾਂ ਜੇ(src3_ਕਦਮ == 0 && beta != 0.0)
        ਸੈੱਟ_ਮੁੱਲ_2(dst, lddst, (fptype)0.0, d_m, d_n);

    ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
        cblas_sgemm(Cblas_ਪੰਕਤੀ_Major, transA, transB, a_m, d_n, a_n, (ਦਸ਼ਮਲਵ)alpha, (ਦਸ਼ਮਲਵ*)src1, ldsrc1, (ਦਸ਼ਮਲਵ*)src2, ldsrc2, (ਦਸ਼ਮਲਵ)beta, (ਦਸ਼ਮਲਵ*)dst, lddst);
    ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
        cblas_dgemm(Cblas_ਪੰਕਤੀ_Major, transA, transB, a_m, d_n, a_n, (ਦੁੱਗਣਾ)alpha, (ਦੁੱਗਣਾ*)src1, ldsrc1, (ਦੁੱਗਣਾ*)src2, ldsrc2, (ਦੁੱਗਣਾ)beta, (ਦੁੱਗਣਾ*)dst, lddst);

    ਵਾਪਸ CV_HAL_ERROR_OK;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ fptype> ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
lapack_gemm_c(ਅਚਲ fptype *src1, size_t src1_ਕਦਮ, ਅਚਲ fptype *src2, size_t src2_ਕਦਮ, fptype alpha,
            ਅਚਲ fptype *src3, size_t src3_ਕਦਮ, fptype beta, fptype *dst, size_t dst_ਕਦਮ, ਪੂਰਨ a_m, ਪੂਰਨ a_n, ਪੂਰਨ d_n, ਪੂਰਨ flags)
{
    ਪੂਰਨ ldsrc1 = (ਪੂਰਨ)(src1_ਕਦਮ / ਆਕਾਰ(std::complex<fptype>));
    ਪੂਰਨ ldsrc2 = (ਪੂਰਨ)(src2_ਕਦਮ / ਆਕਾਰ(std::complex<fptype>));
    ਪੂਰਨ ldsrc3 = (ਪੂਰਨ)(src3_ਕਦਮ / ਆਕਾਰ(std::complex<fptype>));
    ਪੂਰਨ lddst = (ਪੂਰਨ)(dst_ਕਦਮ / ਆਕਾਰ(std::complex<fptype>));
    ਪੂਰਨ c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    ਜੇ(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        ਜੇ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ਨਹੀਂਤਾਂ
        {
            d_m = a_m;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        transB = CblasNoTrans;
        ਜੇ(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ਨਹੀਂਤਾਂ
        {
            d_m = a_m;
        }
    }

    ਜੇ(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ਨਹੀਂਤਾਂ
    {
        c_m = d_m;
        c_n = d_n;
    }

    ਜੇ(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ਨਹੀਂਤਾਂ
    {
        transA = CblasNoTrans;
    }

    ਜੇ(src3 != dst && beta != 0.0 && src3_ਕਦਮ != 0) {
        ਜੇ(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        ਨਹੀਂਤਾਂ
            copy_ਮੈਟ੍ਰਿਕਸ((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    ਨਹੀਂਤਾਂ ਜੇ (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਨਹੀਂਤਾਂ ਜੇ(src3_ਕਦਮ == 0 && beta != 0.0)
        ਸੈੱਟ_ਮੁੱਲ_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#ਜੇ defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    ਪੂਰਨ M = a_m, N = d_n, K = a_n;
    ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ)) {
        ਸਵੈਚਾਲਿਤ src1_cast = (std::complex<ਦਸ਼ਮਲਵ>*)(src1);
        ਸਵੈਚਾਲਿਤ src2_cast = (std::complex<ਦਸ਼ਮਲਵ>*)(src2);
        ਸਵੈਚਾਲਿਤ dst_cast = (std::complex<ਦਸ਼ਮਲਵ>*)(dst);
        ਲੰਮਾ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(Cblas_ਪੰਕਤੀ_Major, transA, transB, M, N, K, (std::complex<ਦਸ਼ਮਲਵ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ਦਸ਼ਮਲਵ>*)&cBeta, dst_cast, ldc);
    }
    ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ)) {
        ਸਵੈਚਾਲਿਤ src1_cast = (std::complex<ਦੁੱਗਣਾ>*)(src1);
        ਸਵੈਚਾਲਿਤ src2_cast = (std::complex<ਦੁੱਗਣਾ>*)(src2);
        ਸਵੈਚਾਲਿਤ dst_cast = (std::complex<ਦੁੱਗਣਾ>*)(dst);
        ਲੰਮਾ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(Cblas_ਪੰਕਤੀ_Major, transA, transB, M, N, K, (std::complex<ਦੁੱਗਣਾ>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<ਦੁੱਗਣਾ>*)&cBeta, dst_cast, ldc);
    }
#ਨਹੀਂਤਾਂ
    ਜੇ(typeid(fptype) == typeid(ਦਸ਼ਮਲਵ))
        cblas_cgemm(Cblas_ਪੰਕਤੀ_Major, transA, transB, a_m, d_n, a_n, (ਦਸ਼ਮਲਵ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ਦਸ਼ਮਲਵ*)src1, ldsrc1, (ਦਸ਼ਮਲਵ*)src2, ldsrc2, (ਦਸ਼ਮਲਵ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ਦਸ਼ਮਲਵ*)dst, lddst);
    ਨਹੀਂਤਾਂ ਜੇ(typeid(fptype) == typeid(ਦੁੱਗਣਾ))
        cblas_zgemm(Cblas_ਪੰਕਤੀ_Major, transA, transB, a_m, d_n, a_n, (ਦੁੱਗਣਾ*)reinterpret_cast<fptype(&)[2]>(cAlpha), (ਦੁੱਗਣਾ*)src1, ldsrc1, (ਦੁੱਗਣਾ*)src2, ldsrc2, (ਦੁੱਗਣਾ*)reinterpret_cast<fptype(&)[2]>(cBeta), (ਦੁੱਗਣਾ*)dst, lddst);
#ਜੇ_ਅੰਤ

    ਵਾਪਸ CV_HAL_ERROR_OK;
}
ਪੂਰਨ lapack_LU32f(ਦਸ਼ਮਲਵ* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਪੂਰਨ* info)
{
    ਜੇ(m < HAL_LU_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_LU(a, a_ਕਦਮ, m, b, b_ਕਦਮ, n, info);
}

ਪੂਰਨ lapack_LU64f(ਦੁੱਗਣਾ* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਪੂਰਨ* info)
{
    ਜੇ(m < HAL_LU_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_LU(a, a_ਕਦਮ, m, b, b_ਕਦਮ, n, info);
}

ਪੂਰਨ lapack_Cholesky32f(ਦਸ਼ਮਲਵ* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਤਾਰਕਿਕ *info)
{
    ਜੇ(m < HAL_CHOLESKY_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_Cholesky(a, a_ਕਦਮ, m, b, b_ਕਦਮ, n, info);
}

ਪੂਰਨ lapack_Cholesky64f(ਦੁੱਗਣਾ* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਤਾਰਕਿਕ *info)
{
    ਜੇ(m < HAL_CHOLESKY_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_Cholesky(a, a_ਕਦਮ, m, b, b_ਕਦਮ, n, info);
}

ਪੂਰਨ lapack_SVD32f(ਦਸ਼ਮਲਵ* a, size_t a_ਕਦਮ, ਦਸ਼ਮਲਵ *w, ਦਸ਼ਮਲਵ* u, size_t u_ਕਦਮ, ਦਸ਼ਮਲਵ* vt, size_t v_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ flags)
{
    ਜੇ(m < HAL_SVD_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH || n <= 0)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਪੂਰਨ info = 0;
    ਵਾਪਸ lapack_SVD(a, a_ਕਦਮ, w, u, u_ਕਦਮ, vt, v_ਕਦਮ, m, n, flags, &info);
}

ਪੂਰਨ lapack_SVD64f(ਦੁੱਗਣਾ* a, size_t a_ਕਦਮ, ਦੁੱਗਣਾ *w, ਦੁੱਗਣਾ* u, size_t u_ਕਦਮ, ਦੁੱਗਣਾ* vt, size_t v_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ flags)
{
    ਜੇ(m < HAL_SVD_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH || n <= 0)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਪੂਰਨ info = 0;
    ਵਾਪਸ lapack_SVD(a, a_ਕਦਮ, w, u, u_ਕਦਮ, vt, v_ਕਦਮ, m, n, flags, &info);
}

ਪੂਰਨ lapack_QR32f(ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ* info)
{
    ਜੇ (m < HAL_QR_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH)
      ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_QR(src1, src1_ਕਦਮ, m, n, k, src2, src2_ਕਦਮ, dst, info);
}

ਪੂਰਨ lapack_QR64f(ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ* info)
{
    ਜੇ (m < HAL_QR_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH)
      ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_QR(src1, src1_ਕਦਮ, m, n, k, src2, src2_ਕਦਮ, dst, info);
}

ਪੂਰਨ lapack_gemm32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ alpha,
                   ਅਚਲ ਦਸ਼ਮਲਵ *src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ *dst, size_t dst_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags)
{
    ਜੇ(m < HAL_GEMM_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_gemm(src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m, n, k, flags);
}

ਪੂਰਨ lapack_gemm64f(ਅਚਲ ਦੁੱਗਣਾ *src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ alpha,
                   ਅਚਲ ਦੁੱਗਣਾ *src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ *dst, size_t dst_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags)
{
    ਜੇ(m < HAL_GEMM_SMALL_ਮੈਟ੍ਰਿਕਸ_THRESH)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_gemm(src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m, n, k, flags);
}

ਪੂਰਨ lapack_gemm32fc(ਅਚਲ ਦਸ਼ਮਲਵ *src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ alpha,
                   ਅਚਲ ਦਸ਼ਮਲਵ *src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ *dst, size_t dst_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags)
{
    ਜੇ(m < HAL_GEMM_SMALL_COMPLEX_ਮੈਟ੍ਰਿਕਸ_THRESH)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_gemm_c(src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m, n, k, flags);
}
ਪੂਰਨ lapack_gemm64fc(ਅਚਲ ਦੁੱਗਣਾ *src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ alpha,
                   ਅਚਲ ਦੁੱਗਣਾ *src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ *dst, size_t dst_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags)
{
    ਜੇ(m < HAL_GEMM_SMALL_COMPLEX_ਮੈਟ੍ਰਿਕਸ_THRESH)
        ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED;
    ਵਾਪਸ lapack_gemm_c(src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m, n, k, flags);
}

#ਜੇ defined(__APPLE__) && defined(__clang__)
#ਹਦਾਇਤ clang diagnostic pop
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ //HAVE_LAPACK
