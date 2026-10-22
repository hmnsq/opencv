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

#اگر_تعریف_شدہ HAVE_LAPACK

#شامل "opencv_lapack.h"

#شامل <cmath>
#شامل <algorithm>
#شامل <typeinfo>
#شامل <limits>
#شامل <complex>
#شامل <vector>

#تعریف_کرو HAL_GEMM_SMALL_COMPLEX_میٹرکس_THRESH 100
#تعریف_کرو HAL_GEMM_SMALL_میٹرکس_THRESH 100
#تعریف_کرو HAL_SVD_SMALL_میٹرکس_THRESH 25
#تعریف_کرو HAL_QR_SMALL_میٹرکس_THRESH 30
#تعریف_کرو HAL_LU_SMALL_میٹرکس_THRESH 100
#تعریف_کرو HAL_CHOLESKY_SMALL_میٹرکس_THRESH 100

#اگر defined(__clang__) && defined(__has_feature)
#اگر __has_feature(حافظہ_sanitizer)
#شامل <sanitizer/msan_interface.h>
#تعریف_کرو CV_ANNOTATE_حافظہ_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#تعریف_کرو CV_ANNOTATE_NO_SANITIZE_حافظہ __attribute__((no_sanitize("memory")))
#اگر_ختم
#اگر_ختم
#اگر_غیر_تعریف_شدہ CV_ANNOTATE_حافظہ_IS_INITIALIZED
#تعریف_کرو CV_ANNOTATE_حافظہ_IS_INITIALIZED(address, size) کرو { } جب(0)
#تعریف_کرو CV_ANNOTATE_NO_SANITIZE_حافظہ
#اگر_ختم

#اگر defined(__APPLE__) && defined(__clang__)
#ہدایت clang diagnostic push
#ہدایت clang diagnostic ignored "-Wdeprecated-declarations"
#اگر_ختم

//lapack stores matrices in column-major order so transposing is needed everywhere
سانچہ <قسم_نام fptype> ساکن اندرونی خالی
transpose_مربع_inplace(fptype *src, size_t src_ld, size_t m)
{
    برائے(size_t i = 0; i < m - 1; i++)
        برائے(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

سانچہ <قسم_نام fptype> ساکن اندرونی خالی
transpose(ثابت fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    برائے(size_t i = 0; i < m; i++)
        برائے(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

سانچہ <قسم_نام fptype> ساکن اندرونی خالی
copy_میٹرکس(ثابت fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    برائے(size_t i = 0; i < m; i++)
        برائے(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

سانچہ <قسم_نام fptype> ساکن اندرونی خالی
سیٹ_قدر_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    برائے(size_t i = 0; i < m; i++)
        برائے(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
سانچہ <قسم_نام fptype> ساکن اندرونی صحیح
CV_ANNOTATE_NO_SANITIZE_حافظہ lapack_LU(fptype* a, size_t a_قدم, صحیح m, fptype* b, size_t b_قدم, صحیح n, صحیح* info)
{
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::Auto_بفر<طویل> piv_buff(m);
    طویل lda = (طویل)(a_قدم / حجم(fptype));
    طویل _m = static_cast<طویل>(m), _n = static_cast<طویل>(n);
    طویل _info[1];
#ورنہ
    cv::Auto_بفر<صحیح> piv_buff(m);
    صحیح lda = (صحیح)(a_قدم / حجم(fptype));
    صحیح _m = m, _n = n;
    صحیح* _info = info;
#اگر_ختم
    خودکار piv = piv_buff.ڈیٹا();

    transpose_مربع_inplace(a, lda, m);

    اگر(b)
    {
        اگر(n == 1 && b_قدم == حجم(fptype))
        {
            اگر(typeid(fptype) == typeid(اعشاری))
                sgesv_(&_m, &_n, (اعشاری*)a, &lda, piv, (اعشاری*)b, &_m, _info);
            ورنہ اگر(typeid(fptype) == typeid(دوہرا))
                dgesv_(&_m, &_n, (دوہرا*)a, &lda, piv, (دوہرا*)b, &_m, _info);
        }
        ورنہ
        {
            صحیح ldb = (صحیح)(b_قدم / حجم(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            اگر(typeid(fptype) == typeid(اعشاری))
                sgesv_(&_m, &_n, (اعشاری*)a, &lda, piv, (اعشاری*)&tmpB[0], &_m, _info);
            ورنہ اگر(typeid(fptype) == typeid(دوہرا))
                dgesv_(&_m, &_n, (دوہرا*)a, &lda, piv, (دوہرا*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    ورنہ
    {
        اگر(typeid(fptype) == typeid(اعشاری))
            sgetrf_(&_m, &_m, (اعشاری*)a, &lda, piv, _info);
        ورنہ اگر(typeid(fptype) == typeid(دوہرا))
            dgetrf_(&_m, &_m, (دوہرا*)a, &lda, piv, _info);
    }

#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحیح>(_info[0]);
#اگر_ختم
    صحیح retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    صحیح sign = 0;
    اگر(*info == 0)
    {
        برائے(صحیح i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    ورنہ
        *info = 0; //in opencv LU function zero means error

    واپس retcode;
}

سانچہ <قسم_نام fptype> ساکن اندرونی صحیح
lapack_Cholesky(fptype* a, size_t a_قدم, صحیح m, fptype* b, size_t b_قدم, صحیح n, منطقی* info)
{
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    طویل _m = static_cast<طویل>(m), _n = static_cast<طویل>(n);
    طویل lapack_حالت = 0;
    طویل lda = (طویل)(a_قدم / حجم(fptype));
#ورنہ
    صحیح _m = m, _n = n;
    صحیح lapack_حالت = 0;
    صحیح lda = (صحیح)(a_قدم / حجم(fptype));
#اگر_ختم
    حرف L[] = {'L', '\0'};

    اگر(b)
    {
        اگر(n == 1 && b_قدم == حجم(fptype))
        {
            اگر(typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (اعشاری*)a, &lda, (اعشاری*)b, &_m, &lapack_حالت);
            ورنہ اگر(typeid(fptype) == typeid(دوہرا))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (دوہرا*)a, &lda, (دوہرا*)b, &_m, &lapack_حالت);
        }
        ورنہ
        {
            صحیح ldb = (صحیح)(b_قدم / حجم(fptype));
            fptype* tmpB = نیا fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            اگر(typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (اعشاری*)a, &lda, (اعشاری*)tmpB, &_m, &lapack_حالت);
            ورنہ اگر(typeid(fptype) == typeid(دوہرا))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (دوہرا*)a, &lda, (دوہرا*)tmpB, &_m, &lapack_حالت);

            transpose(tmpB, m, b, ldb, n, m);
            حذف[] tmpB;
        }
    }
    ورنہ
    {
        اگر(typeid(fptype) == typeid(اعشاری))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (اعشاری*)a, &lda, &lapack_حالت);
        ورنہ اگر(typeid(fptype) == typeid(دوہرا))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (دوہرا*)a, &lda, &lapack_حالت);
    }

    اگر(lapack_حالت == 0) *info = سچ;
    ورنہ *info = جھوٹ; //in opencv Cholesky function false means error

    واپس lapack_حالت >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

سانچہ <قسم_نام fptype> ساکن اندرونی صحیح
lapack_SVD(fptype* a, size_t a_قدم, fptype *w, fptype* u, size_t u_قدم, fptype* vt, size_t v_قدم, صحیح m, صحیح n, صحیح flags, صحیح* info)
{
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    طویل _m = static_cast<طویل>(m), _n = static_cast<طویل>(n);
    طویل _info[1];
    طویل lda = (طویل)(a_قدم / حجم(fptype));
    طویل ldv = (طویل)(v_قدم / حجم(fptype));
    طویل ldu = (طویل)(u_قدم / حجم(fptype));
    طویل lwork = -1;
    cv::Auto_بفر<طویل> iworkBuf_(8 * std::min(m, n));
#ورنہ
    صحیح _m = m, _n = n;
    صحیح* _info = info;
    صحیح lda = (صحیح)(a_قدم / حجم(fptype));
    صحیح ldv = (صحیح)(v_قدم / حجم(fptype));
    صحیح ldu = (صحیح)(u_قدم / حجم(fptype));
    صحیح lwork = -1;
    cv::Auto_بفر<صحیح> iworkBuf_(8 * std::min(m, n));
#اگر_ختم
    خودکار iworkBuf = iworkBuf_.ڈیٹا();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    حرف منوال[] = { ' ', '\0'};
    اگر(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        منوال[0] = 'N';
    }
    ورنہ اگر((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        منوال[0] = 'O';
    ورنہ اگر((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        منوال[0] = 'S';
    ورنہ اگر(flags & CV_HAL_SVD_بھرا_UV) //full SVD, U stored in u or in a
        منوال[0] = 'A';

    اگر((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_بھرا_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    اگر(typeid(fptype) == typeid(اعشاری))
        OCV_LAPACK_FUNC(sgesdd)(منوال, &_m, &_n, (اعشاری*)a, &lda, (اعشاری*)w, (اعشاری*)u, &ldu, (اعشاری*)vt, &ldv, (اعشاری*)&work1, &lwork, iworkBuf, _info);
    ورنہ اگر(typeid(fptype) == typeid(دوہرا))
        OCV_LAPACK_FUNC(dgesdd)(منوال, &_m, &_n, (دوہرا*)a, &lda, (دوہرا*)w, (دوہرا*)u, &ldu, (دوہرا*)vt, &ldv, (دوہرا*)&work1, &lwork, iworkBuf, _info);

    اگر(*info < 0)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (صحیح)round(work1); //optimal buffer size
    std::vector<fptype> بفر(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_حافظہ_IS_INITIALIZED(بفر.ڈیٹا(), حجم(fptype) * (lwork + 1));

    اگر(typeid(fptype) == typeid(اعشاری))
        OCV_LAPACK_FUNC(sgesdd)(منوال, &_m, &_n, (اعشاری*)a, &lda, (اعشاری*)w, (اعشاری*)u, &ldu, (اعشاری*)vt, &ldv, (اعشاری*)&بفر[0], &lwork, iworkBuf, _info);
    ورنہ اگر(typeid(fptype) == typeid(دوہرا))
        OCV_LAPACK_FUNC(dgesdd)(منوال, &_m, &_n, (دوہرا*)a, &lda, (دوہرا*)w, (دوہرا*)u, &ldu, (دوہرا*)vt, &ldv, (دوہرا*)&بفر[0], &lwork, iworkBuf, _info);

#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحیح>(_info[0]);
#اگر_ختم

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_حافظہ_IS_INITIALIZED(a, a_قدم * n);
    اگر (u)
      CV_ANNOTATE_حافظہ_IS_INITIALIZED(u, u_قدم * m);
    اگر (vt)
      CV_ANNOTATE_حافظہ_IS_INITIALIZED(vt, v_قدم * n);
    اگر (w)
      CV_ANNOTATE_حافظہ_IS_INITIALIZED(w, حجم(fptype) * std::min(m, n));

    اگر(!(flags & CV_HAL_SVD_NO_UV))
        transpose_مربع_inplace(vt, ldv, n);

    اگر((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_بھرا_UV))
    {
        برائے(صحیح i = 0; i < m; i++)
            برائے(صحیح j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    اگر(*info < 0)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس CV_HAL_ERROR_OK;
}

سانچہ <قسم_نام fptype> ساکن اندرونی صحیح
lapack_QR(fptype* a, size_t a_قدم, صحیح m, صحیح n, صحیح k, fptype* b, size_t b_قدم, fptype* dst, صحیح* info)
{
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    طویل _m = static_cast<طویل>(m), _n = static_cast<طویل>(n), _k = static_cast<طویل>(k);
    طویل _info[1];
    طویل lda = (طویل)(a_قدم / حجم(fptype));
    طویل lwork = -1;
    طویل ldtmpA;
#ورنہ
    صحیح _m = m, _n = n, _k = k;
    صحیح* _info = info;
    صحیح lda = (صحیح)(a_قدم / حجم(fptype));
    صحیح lwork = -1;
    صحیح ldtmpA;
#اگر_ختم

    حرف منوال[] = { 'N', '\0' };
    اگر(m < n)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    اگر (m == n)
    {
        transpose_مربع_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    ورنہ
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    اگر (b)
    {
        اگر (k == 1 && b_قدم == حجم(fptype))
        {
            اگر (typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sgels)(منوال, &_m, &_n, &_k, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)b, &_m, (اعشاری*)&work1, &lwork, _info);
            ورنہ اگر (typeid(fptype) == typeid(دوہرا))
                OCV_LAPACK_FUNC(dgels)(منوال, &_m, &_n, &_k, (دوہرا*)tmpA, &ldtmpA, (دوہرا*)b, &_m, (دوہرا*)&work1, &lwork, _info);

            CV_ANNOTATE_حافظہ_IS_INITIALIZED(info, حجم(صحیح));
            اگر (*info < 0)
                واپس CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_باری(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* بفر = &workBufMemHolder.front();

            اگر (typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sgels)(منوال, &_m, &_n, &_k, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)b, &_m, (اعشاری*)بفر, &lwork, _info);
            ورنہ اگر (typeid(fptype) == typeid(دوہرا))
                OCV_LAPACK_FUNC(dgels)(منوال, &_m, &_n, &_k, (دوہرا*)tmpA, &ldtmpA, (دوہرا*)b, &_m, (دوہرا*)بفر, &lwork, _info);
        }
        ورنہ
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            صحیح ldb = (صحیح)(b_قدم / حجم(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            اگر (typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sgels)(منوال, &_m, &_n, &_k, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)tmpB, &_m, (اعشاری*)&work1, &lwork, _info);
            ورنہ اگر (typeid(fptype) == typeid(دوہرا))
                OCV_LAPACK_FUNC(dgels)(منوال, &_m, &_n, &_k, (دوہرا*)tmpA, &ldtmpA, (دوہرا*)tmpB, &_m, (دوہرا*)&work1, &lwork, _info);

            CV_ANNOTATE_حافظہ_IS_INITIALIZED(info, حجم(صحیح));
            اگر (*info < 0)
                واپس CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_باری(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* بفر = &workBufMemHolder.front();

            اگر (typeid(fptype) == typeid(اعشاری))
                OCV_LAPACK_FUNC(sgels)(منوال, &_m, &_n, &_k, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)tmpB, &_m, (اعشاری*)بفر, &lwork, _info);
            ورنہ اگر (typeid(fptype) == typeid(دوہرا))
                OCV_LAPACK_FUNC(dgels)(منوال, &_m, &_n, &_k, (دوہرا*)tmpA, &ldtmpA, (دوہرا*)tmpB, &_m, (دوہرا*)بفر, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    ورنہ
    {
        اگر (typeid(fptype) == typeid(اعشاری))
            sgeqrf_(&_m, &_n, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)dst, (اعشاری*)&work1, &lwork, _info);
        ورنہ اگر (typeid(fptype) == typeid(دوہرا))
            dgeqrf_(&_m, &_n, (دوہرا*)tmpA, &ldtmpA, (دوہرا*)dst, (دوہرا*)&work1, &lwork, _info);

        CV_ANNOTATE_حافظہ_IS_INITIALIZED(info, حجم(صحیح));
        اگر (*info < 0)
            واپس CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cv_باری(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* بفر = &workBufMemHolder.front();

        اگر (typeid(fptype) == typeid(اعشاری))
            sgeqrf_(&_m, &_n, (اعشاری*)tmpA, &ldtmpA, (اعشاری*)dst, (اعشاری*)بفر, &lwork, _info);
        ورنہ اگر (typeid(fptype) == typeid(دوہرا))
            dgeqrf_(&_m, &_n, (دوہرا*)tmpA, &ldtmpA, (دوہرا*)dst, (دوہرا*)بفر, &lwork, _info);
    }

    CV_ANNOTATE_حافظہ_IS_INITIALIZED(info, حجم(صحیح));
    اگر (m == n)
        transpose_مربع_inplace(a, lda, m);
    ورنہ
        transpose(tmpA, m, a, lda, n, m);

#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحیح>(_info[0]);
#اگر_ختم

    اگر (*info != 0)
        *info = 0;
    ورنہ
        *info = 1;

    واپس CV_HAL_ERROR_OK;
}

سانچہ <قسم_نام fptype> ساکن اندرونی صحیح
lapack_gemm(ثابت fptype *src1, size_t src1_قدم, ثابت fptype *src2, size_t src2_قدم, fptype alpha,
            ثابت fptype *src3, size_t src3_قدم, fptype beta, fptype *dst, size_t dst_قدم, صحیح a_m, صحیح a_n, صحیح d_n, صحیح flags)
{
    صحیح ldsrc1 = (صحیح)(src1_قدم / حجم(fptype));
    صحیح ldsrc2 = (صحیح)(src2_قدم / حجم(fptype));
    صحیح ldsrc3 = (صحیح)(src3_قدم / حجم(fptype));
    صحیح lddst = (صحیح)(dst_قدم / حجم(fptype));
    صحیح c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    اگر(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        اگر(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ورنہ
        {
            d_m = a_m;
        }
    }
    ورنہ
    {
        transB = CblasNoTrans;
        اگر(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ورنہ
        {
            d_m = a_m;
        }
    }

    اگر(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ورنہ
    {
        c_m = d_m;
        c_n = d_n;
    }

    اگر(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ورنہ
    {
        transA = CblasNoTrans;
    }

    اگر(src3 != dst && beta != 0.0 && src3_قدم != 0) {
        اگر(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        ورنہ
            copy_میٹرکس(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    ورنہ اگر (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    ورنہ اگر(src3_قدم == 0 && beta != 0.0)
        سیٹ_قدر_2(dst, lddst, (fptype)0.0, d_m, d_n);

    اگر(typeid(fptype) == typeid(اعشاری))
        cblas_sgemm(Cblas_صف_Major, transA, transB, a_m, d_n, a_n, (اعشاری)alpha, (اعشاری*)src1, ldsrc1, (اعشاری*)src2, ldsrc2, (اعشاری)beta, (اعشاری*)dst, lddst);
    ورنہ اگر(typeid(fptype) == typeid(دوہرا))
        cblas_dgemm(Cblas_صف_Major, transA, transB, a_m, d_n, a_n, (دوہرا)alpha, (دوہرا*)src1, ldsrc1, (دوہرا*)src2, ldsrc2, (دوہرا)beta, (دوہرا*)dst, lddst);

    واپس CV_HAL_ERROR_OK;
}

سانچہ <قسم_نام fptype> ساکن اندرونی صحیح
lapack_gemm_c(ثابت fptype *src1, size_t src1_قدم, ثابت fptype *src2, size_t src2_قدم, fptype alpha,
            ثابت fptype *src3, size_t src3_قدم, fptype beta, fptype *dst, size_t dst_قدم, صحیح a_m, صحیح a_n, صحیح d_n, صحیح flags)
{
    صحیح ldsrc1 = (صحیح)(src1_قدم / حجم(std::complex<fptype>));
    صحیح ldsrc2 = (صحیح)(src2_قدم / حجم(std::complex<fptype>));
    صحیح ldsrc3 = (صحیح)(src3_قدم / حجم(std::complex<fptype>));
    صحیح lddst = (صحیح)(dst_قدم / حجم(std::complex<fptype>));
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
        ورنہ
        {
            d_m = a_m;
        }
    }
    ورنہ
    {
        transB = CblasNoTrans;
        اگر(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        ورنہ
        {
            d_m = a_m;
        }
    }

    اگر(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    ورنہ
    {
        c_m = d_m;
        c_n = d_n;
    }

    اگر(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    ورنہ
    {
        transA = CblasNoTrans;
    }

    اگر(src3 != dst && beta != 0.0 && src3_قدم != 0) {
        اگر(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        ورنہ
            copy_میٹرکس((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    ورنہ اگر (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    ورنہ اگر(src3_قدم == 0 && beta != 0.0)
        سیٹ_قدر_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#اگر defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    صحیح M = a_m, N = d_n, K = a_n;
    اگر(typeid(fptype) == typeid(اعشاری)) {
        خودکار src1_cast = (std::complex<اعشاری>*)(src1);
        خودکار src2_cast = (std::complex<اعشاری>*)(src2);
        خودکار dst_cast = (std::complex<اعشاری>*)(dst);
        طویل lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(Cblas_صف_Major, transA, transB, M, N, K, (std::complex<اعشاری>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<اعشاری>*)&cBeta, dst_cast, ldc);
    }
    ورنہ اگر(typeid(fptype) == typeid(دوہرا)) {
        خودکار src1_cast = (std::complex<دوہرا>*)(src1);
        خودکار src2_cast = (std::complex<دوہرا>*)(src2);
        خودکار dst_cast = (std::complex<دوہرا>*)(dst);
        طویل lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(Cblas_صف_Major, transA, transB, M, N, K, (std::complex<دوہرا>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<دوہرا>*)&cBeta, dst_cast, ldc);
    }
#ورنہ
    اگر(typeid(fptype) == typeid(اعشاری))
        cblas_cgemm(Cblas_صف_Major, transA, transB, a_m, d_n, a_n, (اعشاری*)reinterpret_cast<fptype(&)[2]>(cAlpha), (اعشاری*)src1, ldsrc1, (اعشاری*)src2, ldsrc2, (اعشاری*)reinterpret_cast<fptype(&)[2]>(cBeta), (اعشاری*)dst, lddst);
    ورنہ اگر(typeid(fptype) == typeid(دوہرا))
        cblas_zgemm(Cblas_صف_Major, transA, transB, a_m, d_n, a_n, (دوہرا*)reinterpret_cast<fptype(&)[2]>(cAlpha), (دوہرا*)src1, ldsrc1, (دوہرا*)src2, ldsrc2, (دوہرا*)reinterpret_cast<fptype(&)[2]>(cBeta), (دوہرا*)dst, lddst);
#اگر_ختم

    واپس CV_HAL_ERROR_OK;
}
صحیح lapack_LU32f(اعشاری* a, size_t a_قدم, صحیح m, اعشاری* b, size_t b_قدم, صحیح n, صحیح* info)
{
    اگر(m < HAL_LU_SMALL_میٹرکس_THRESH)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_LU(a, a_قدم, m, b, b_قدم, n, info);
}

صحیح lapack_LU64f(دوہرا* a, size_t a_قدم, صحیح m, دوہرا* b, size_t b_قدم, صحیح n, صحیح* info)
{
    اگر(m < HAL_LU_SMALL_میٹرکس_THRESH)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_LU(a, a_قدم, m, b, b_قدم, n, info);
}

صحیح lapack_Cholesky32f(اعشاری* a, size_t a_قدم, صحیح m, اعشاری* b, size_t b_قدم, صحیح n, منطقی *info)
{
    اگر(m < HAL_CHOLESKY_SMALL_میٹرکس_THRESH)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_Cholesky(a, a_قدم, m, b, b_قدم, n, info);
}

صحیح lapack_Cholesky64f(دوہرا* a, size_t a_قدم, صحیح m, دوہرا* b, size_t b_قدم, صحیح n, منطقی *info)
{
    اگر(m < HAL_CHOLESKY_SMALL_میٹرکس_THRESH)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_Cholesky(a, a_قدم, m, b, b_قدم, n, info);
}

صحیح lapack_SVD32f(اعشاری* a, size_t a_قدم, اعشاری *w, اعشاری* u, size_t u_قدم, اعشاری* vt, size_t v_قدم, صحیح m, صحیح n, صحیح flags)
{
    اگر(m < HAL_SVD_SMALL_میٹرکس_THRESH || n <= 0)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    صحیح info = 0;
    واپس lapack_SVD(a, a_قدم, w, u, u_قدم, vt, v_قدم, m, n, flags, &info);
}

صحیح lapack_SVD64f(دوہرا* a, size_t a_قدم, دوہرا *w, دوہرا* u, size_t u_قدم, دوہرا* vt, size_t v_قدم, صحیح m, صحیح n, صحیح flags)
{
    اگر(m < HAL_SVD_SMALL_میٹرکس_THRESH || n <= 0)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    صحیح info = 0;
    واپس lapack_SVD(a, a_قدم, w, u, u_قدم, vt, v_قدم, m, n, flags, &info);
}

صحیح lapack_QR32f(اعشاری* src1, size_t src1_قدم, صحیح m, صحیح n, صحیح k, اعشاری* src2, size_t src2_قدم, اعشاری* dst, صحیح* info)
{
    اگر (m < HAL_QR_SMALL_میٹرکس_THRESH)
      واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_QR(src1, src1_قدم, m, n, k, src2, src2_قدم, dst, info);
}

صحیح lapack_QR64f(دوہرا* src1, size_t src1_قدم, صحیح m, صحیح n, صحیح k, دوہرا* src2, size_t src2_قدم, دوہرا* dst, صحیح* info)
{
    اگر (m < HAL_QR_SMALL_میٹرکس_THRESH)
      واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_QR(src1, src1_قدم, m, n, k, src2, src2_قدم, dst, info);
}

صحیح lapack_gemm32f(ثابت اعشاری *src1, size_t src1_قدم, ثابت اعشاری *src2, size_t src2_قدم, اعشاری alpha,
                   ثابت اعشاری *src3, size_t src3_قدم, اعشاری beta, اعشاری *dst, size_t dst_قدم, صحیح m, صحیح n, صحیح k, صحیح flags)
{
    اگر(m < HAL_GEMM_SMALL_میٹرکس_THRESH)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_gemm(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m, n, k, flags);
}

صحیح lapack_gemm64f(ثابت دوہرا *src1, size_t src1_قدم, ثابت دوہرا *src2, size_t src2_قدم, دوہرا alpha,
                   ثابت دوہرا *src3, size_t src3_قدم, دوہرا beta, دوہرا *dst, size_t dst_قدم, صحیح m, صحیح n, صحیح k, صحیح flags)
{
    اگر(m < HAL_GEMM_SMALL_میٹرکس_THRESH)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_gemm(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m, n, k, flags);
}

صحیح lapack_gemm32fc(ثابت اعشاری *src1, size_t src1_قدم, ثابت اعشاری *src2, size_t src2_قدم, اعشاری alpha,
                   ثابت اعشاری *src3, size_t src3_قدم, اعشاری beta, اعشاری *dst, size_t dst_قدم, صحیح m, صحیح n, صحیح k, صحیح flags)
{
    اگر(m < HAL_GEMM_SMALL_COMPLEX_میٹرکس_THRESH)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_gemm_c(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m, n, k, flags);
}
صحیح lapack_gemm64fc(ثابت دوہرا *src1, size_t src1_قدم, ثابت دوہرا *src2, size_t src2_قدم, دوہرا alpha,
                   ثابت دوہرا *src3, size_t src3_قدم, دوہرا beta, دوہرا *dst, size_t dst_قدم, صحیح m, صحیح n, صحیح k, صحیح flags)
{
    اگر(m < HAL_GEMM_SMALL_COMPLEX_میٹرکس_THRESH)
        واپس CV_HAL_ERROR_NOT_IMPLEMENTED;
    واپس lapack_gemm_c(src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m, n, k, flags);
}

#اگر defined(__APPLE__) && defined(__clang__)
#ہدایت clang diagnostic pop
#اگر_ختم

#اگر_ختم //HAVE_LAPACK
