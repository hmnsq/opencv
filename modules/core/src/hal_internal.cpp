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

#که_تعریف_شوی HAVE_LAPACK

#ضمن "opencv_lapack.h"

#ضمن <cmath>
#ضمن <algorithm>
#ضمن <typeinfo>
#ضمن <limits>
#ضمن <complex>
#ضمن <vector>

#تعریف_کړه HAL_GEMM_SMALL_COMPLEX_ماتریکس_THRESH 100
#تعریف_کړه HAL_GEMM_SMALL_ماتریکس_THRESH 100
#تعریف_کړه HAL_SVD_SMALL_ماتریکس_THRESH 25
#تعریف_کړه HAL_QR_SMALL_ماتریکس_THRESH 30
#تعریف_کړه HAL_LU_SMALL_ماتریکس_THRESH 100
#تعریف_کړه HAL_CHOLESKY_SMALL_ماتریکس_THRESH 100

#إذا defined(__clang__) && defined(__has_feature)
#إذا __has_feature(حافظه_sanitizer)
#ضمن <sanitizer/msan_interface.h>
#تعریف_کړه CV_ANNOTATE_حافظه_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#تعریف_کړه CV_ANNOTATE_NO_SANITIZE_حافظه __attribute__((no_sanitize("memory")))
#د_که_پای
#د_که_پای
#که_نه_تعریف_شوی CV_ANNOTATE_حافظه_IS_INITIALIZED
#تعریف_کړه CV_ANNOTATE_حافظه_IS_INITIALIZED(address, size) افعل { } طالما(0)
#تعریف_کړه CV_ANNOTATE_NO_SANITIZE_حافظه
#د_که_پای

#إذا defined(__APPLE__) && defined(__clang__)
#لارښوونه clang diagnostic push
#لارښوونه clang diagnostic ignored "-Wdeprecated-declarations"
#د_که_پای

//lapack stores matrices in column-major order so transposing is needed everywhere
کينډۍ <ډول_نوم fptype> ساكن مضمن فراغ
transpose_مربع_inplace(fptype *src, size_t src_ld, size_t m)
{
    لكل(size_t i = 0; i < m - 1; i++)
        لكل(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

کينډۍ <ډول_نوم fptype> ساكن مضمن فراغ
transpose(ثابت fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    لكل(size_t i = 0; i < m; i++)
        لكل(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

کينډۍ <ډول_نوم fptype> ساكن مضمن فراغ
copy_ماتریکس(ثابت fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    لكل(size_t i = 0; i < m; i++)
        لكل(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

کينډۍ <ډول_نوم fptype> ساكن مضمن فراغ
سېټ_ارزښت_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    لكل(size_t i = 0; i < m; i++)
        لكل(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
کينډۍ <ډول_نوم fptype> ساكن مضمن صحيح
CV_ANNOTATE_NO_SANITIZE_حافظه lapack_LU(fptype* a, size_t a_ګام, صحيح m, fptype* b, size_t b_ګام, صحيح n, صحيح* info)
{
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::Auto_بفر<اوږد> piv_buff(m);
    اوږد lda = (اوږد)(a_ګام / حجم(fptype));
    اوږد _m = static_cast<اوږد>(m), _n = static_cast<اوږد>(n);
    اوږد _info[1];
#وإلا
    cv::Auto_بفر<صحيح> piv_buff(m);
    صحيح lda = (صحيح)(a_ګام / حجم(fptype));
    صحيح _m = m, _n = n;
    صحيح* _info = info;
#د_که_پای
    خپلکاری piv = piv_buff.معلومات();

    transpose_مربع_inplace(a, lda, m);

    إذا(b)
    {
        إذا(n == 1 && b_ګام == حجم(fptype))
        {
            إذا(typeid(fptype) == typeid(عائم))
                sgesv_(&_m, &_n, (عائم*)a, &lda, piv, (عائم*)b, &_m, _info);
            وإلا إذا(typeid(fptype) == typeid(مزدوج))
                dgesv_(&_m, &_n, (مزدوج*)a, &lda, piv, (مزدوج*)b, &_m, _info);
        }
        وإلا
        {
            صحيح ldb = (صحيح)(b_ګام / حجم(fptype));
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
#د_که_پای
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

کينډۍ <ډول_نوم fptype> ساكن مضمن صحيح
lapack_Cholesky(fptype* a, size_t a_ګام, صحيح m, fptype* b, size_t b_ګام, صحيح n, منطقي* info)
{
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    اوږد _m = static_cast<اوږد>(m), _n = static_cast<اوږد>(n);
    اوږد lapack_حالت = 0;
    اوږد lda = (اوږد)(a_ګام / حجم(fptype));
#وإلا
    صحيح _m = m, _n = n;
    صحيح lapack_حالت = 0;
    صحيح lda = (صحيح)(a_ګام / حجم(fptype));
#د_که_پای
    حرف L[] = {'L', '\0'};

    إذا(b)
    {
        إذا(n == 1 && b_ګام == حجم(fptype))
        {
            إذا(typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (عائم*)a, &lda, (عائم*)b, &_m, &lapack_حالت);
            وإلا إذا(typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (مزدوج*)a, &lda, (مزدوج*)b, &_m, &lapack_حالت);
        }
        وإلا
        {
            صحيح ldb = (صحيح)(b_ګام / حجم(fptype));
            fptype* tmpB = جديد fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            إذا(typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (عائم*)a, &lda, (عائم*)tmpB, &_m, &lapack_حالت);
            وإلا إذا(typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (مزدوج*)a, &lda, (مزدوج*)tmpB, &_m, &lapack_حالت);

            transpose(tmpB, m, b, ldb, n, m);
            ړنګول[] tmpB;
        }
    }
    وإلا
    {
        إذا(typeid(fptype) == typeid(عائم))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (عائم*)a, &lda, &lapack_حالت);
        وإلا إذا(typeid(fptype) == typeid(مزدوج))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (مزدوج*)a, &lda, &lapack_حالت);
    }

    إذا(lapack_حالت == 0) *info = ریښتیا;
    وإلا *info = خطأ; //in opencv Cholesky function false means error

    أرجع lapack_حالت >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

کينډۍ <ډول_نوم fptype> ساكن مضمن صحيح
lapack_SVD(fptype* a, size_t a_ګام, fptype *w, fptype* u, size_t u_ګام, fptype* vt, size_t v_ګام, صحيح m, صحيح n, صحيح flags, صحيح* info)
{
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    اوږد _m = static_cast<اوږد>(m), _n = static_cast<اوږد>(n);
    اوږد _info[1];
    اوږد lda = (اوږد)(a_ګام / حجم(fptype));
    اوږد ldv = (اوږد)(v_ګام / حجم(fptype));
    اوږد ldu = (اوږد)(u_ګام / حجم(fptype));
    اوږد lwork = -1;
    cv::Auto_بفر<اوږد> iworkBuf_(8 * std::min(m, n));
#وإلا
    صحيح _m = m, _n = n;
    صحيح* _info = info;
    صحيح lda = (صحيح)(a_ګام / حجم(fptype));
    صحيح ldv = (صحيح)(v_ګام / حجم(fptype));
    صحيح ldu = (صحيح)(u_ګام / حجم(fptype));
    صحيح lwork = -1;
    cv::Auto_بفر<صحيح> iworkBuf_(8 * std::min(m, n));
#د_که_پای
    خپلکاری iworkBuf = iworkBuf_.معلومات();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    حرف ډېرپېښ[] = { ' ', '\0'};
    إذا(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        ډېرپېښ[0] = 'N';
    }
    وإلا إذا((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        ډېرپېښ[0] = 'O';
    وإلا إذا((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        ډېرپېښ[0] = 'S';
    وإلا إذا(flags & CV_HAL_SVD_ډک_UV) //full SVD, U stored in u or in a
        ډېرپېښ[0] = 'A';

    إذا((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_ډک_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    إذا(typeid(fptype) == typeid(عائم))
        OCV_LAPACK_FUNC(sgesdd)(ډېرپېښ, &_m, &_n, (عائم*)a, &lda, (عائم*)w, (عائم*)u, &ldu, (عائم*)vt, &ldv, (عائم*)&work1, &lwork, iworkBuf, _info);
    وإلا إذا(typeid(fptype) == typeid(مزدوج))
        OCV_LAPACK_FUNC(dgesdd)(ډېرپېښ, &_m, &_n, (مزدوج*)a, &lda, (مزدوج*)w, (مزدوج*)u, &ldu, (مزدوج*)vt, &ldv, (مزدوج*)&work1, &lwork, iworkBuf, _info);

    إذا(*info < 0)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (صحيح)round(work1); //optimal buffer size
    std::vector<fptype> بفر(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_حافظه_IS_INITIALIZED(بفر.معلومات(), حجم(fptype) * (lwork + 1));

    إذا(typeid(fptype) == typeid(عائم))
        OCV_LAPACK_FUNC(sgesdd)(ډېرپېښ, &_m, &_n, (عائم*)a, &lda, (عائم*)w, (عائم*)u, &ldu, (عائم*)vt, &ldv, (عائم*)&بفر[0], &lwork, iworkBuf, _info);
    وإلا إذا(typeid(fptype) == typeid(مزدوج))
        OCV_LAPACK_FUNC(dgesdd)(ډېرپېښ, &_m, &_n, (مزدوج*)a, &lda, (مزدوج*)w, (مزدوج*)u, &ldu, (مزدوج*)vt, &ldv, (مزدوج*)&بفر[0], &lwork, iworkBuf, _info);

#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحيح>(_info[0]);
#د_که_پای

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_حافظه_IS_INITIALIZED(a, a_ګام * n);
    إذا (u)
      CV_ANNOTATE_حافظه_IS_INITIALIZED(u, u_ګام * m);
    إذا (vt)
      CV_ANNOTATE_حافظه_IS_INITIALIZED(vt, v_ګام * n);
    إذا (w)
      CV_ANNOTATE_حافظه_IS_INITIALIZED(w, حجم(fptype) * std::min(m, n));

    إذا(!(flags & CV_HAL_SVD_NO_UV))
        transpose_مربع_inplace(vt, ldv, n);

    إذا((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_ډک_UV))
    {
        لكل(صحيح i = 0; i < m; i++)
            لكل(صحيح j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    إذا(*info < 0)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع CV_HAL_ERROR_OK;
}

کينډۍ <ډول_نوم fptype> ساكن مضمن صحيح
lapack_QR(fptype* a, size_t a_ګام, صحيح m, صحيح n, صحيح k, fptype* b, size_t b_ګام, fptype* dst, صحيح* info)
{
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    اوږد _m = static_cast<اوږد>(m), _n = static_cast<اوږد>(n), _k = static_cast<اوږد>(k);
    اوږد _info[1];
    اوږد lda = (اوږد)(a_ګام / حجم(fptype));
    اوږد lwork = -1;
    اوږد ldtmpA;
#وإلا
    صحيح _m = m, _n = n, _k = k;
    صحيح* _info = info;
    صحيح lda = (صحيح)(a_ګام / حجم(fptype));
    صحيح lwork = -1;
    صحيح ldtmpA;
#د_که_پای

    حرف ډېرپېښ[] = { 'N', '\0' };
    إذا(m < n)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    إذا (m == n)
    {
        transpose_مربع_inplace(a, lda, m);
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
        إذا (k == 1 && b_ګام == حجم(fptype))
        {
            إذا (typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sgels)(ډېرپېښ, &_m, &_n, &_k, (عائم*)tmpA, &ldtmpA, (عائم*)b, &_m, (عائم*)&work1, &lwork, _info);
            وإلا إذا (typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dgels)(ډېرپېښ, &_m, &_n, &_k, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)b, &_m, (مزدوج*)&work1, &lwork, _info);

            CV_ANNOTATE_حافظه_IS_INITIALIZED(info, حجم(صحيح));
            إذا (*info < 0)
                أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_پړاو(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* بفر = &workBufMemHolder.front();

            إذا (typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sgels)(ډېرپېښ, &_m, &_n, &_k, (عائم*)tmpA, &ldtmpA, (عائم*)b, &_m, (عائم*)بفر, &lwork, _info);
            وإلا إذا (typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dgels)(ډېرپېښ, &_m, &_n, &_k, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)b, &_m, (مزدوج*)بفر, &lwork, _info);
        }
        وإلا
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            صحيح ldb = (صحيح)(b_ګام / حجم(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            إذا (typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sgels)(ډېرپېښ, &_m, &_n, &_k, (عائم*)tmpA, &ldtmpA, (عائم*)tmpB, &_m, (عائم*)&work1, &lwork, _info);
            وإلا إذا (typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dgels)(ډېرپېښ, &_m, &_n, &_k, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)tmpB, &_m, (مزدوج*)&work1, &lwork, _info);

            CV_ANNOTATE_حافظه_IS_INITIALIZED(info, حجم(صحيح));
            إذا (*info < 0)
                أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_پړاو(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* بفر = &workBufMemHolder.front();

            إذا (typeid(fptype) == typeid(عائم))
                OCV_LAPACK_FUNC(sgels)(ډېرپېښ, &_m, &_n, &_k, (عائم*)tmpA, &ldtmpA, (عائم*)tmpB, &_m, (عائم*)بفر, &lwork, _info);
            وإلا إذا (typeid(fptype) == typeid(مزدوج))
                OCV_LAPACK_FUNC(dgels)(ډېرپېښ, &_m, &_n, &_k, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)tmpB, &_m, (مزدوج*)بفر, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    وإلا
    {
        إذا (typeid(fptype) == typeid(عائم))
            sgeqrf_(&_m, &_n, (عائم*)tmpA, &ldtmpA, (عائم*)dst, (عائم*)&work1, &lwork, _info);
        وإلا إذا (typeid(fptype) == typeid(مزدوج))
            dgeqrf_(&_m, &_n, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)dst, (مزدوج*)&work1, &lwork, _info);

        CV_ANNOTATE_حافظه_IS_INITIALIZED(info, حجم(صحيح));
        إذا (*info < 0)
            أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cv_پړاو(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* بفر = &workBufMemHolder.front();

        إذا (typeid(fptype) == typeid(عائم))
            sgeqrf_(&_m, &_n, (عائم*)tmpA, &ldtmpA, (عائم*)dst, (عائم*)بفر, &lwork, _info);
        وإلا إذا (typeid(fptype) == typeid(مزدوج))
            dgeqrf_(&_m, &_n, (مزدوج*)tmpA, &ldtmpA, (مزدوج*)dst, (مزدوج*)بفر, &lwork, _info);
    }

    CV_ANNOTATE_حافظه_IS_INITIALIZED(info, حجم(صحيح));
    إذا (m == n)
        transpose_مربع_inplace(a, lda, m);
    وإلا
        transpose(tmpA, m, a, lda, n, m);

#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<صحيح>(_info[0]);
#د_که_پای

    إذا (*info != 0)
        *info = 0;
    وإلا
        *info = 1;

    أرجع CV_HAL_ERROR_OK;
}

کينډۍ <ډول_نوم fptype> ساكن مضمن صحيح
lapack_gemm(ثابت fptype *src1, size_t src1_ګام, ثابت fptype *src2, size_t src2_ګام, fptype alpha,
            ثابت fptype *src3, size_t src3_ګام, fptype beta, fptype *dst, size_t dst_ګام, صحيح a_m, صحيح a_n, صحيح d_n, صحيح flags)
{
    صحيح ldsrc1 = (صحيح)(src1_ګام / حجم(fptype));
    صحيح ldsrc2 = (صحيح)(src2_ګام / حجم(fptype));
    صحيح ldsrc3 = (صحيح)(src3_ګام / حجم(fptype));
    صحيح lddst = (صحيح)(dst_ګام / حجم(fptype));
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

    إذا(src3 != dst && beta != 0.0 && src3_ګام != 0) {
        إذا(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        وإلا
            copy_ماتریکس(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    وإلا إذا (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    وإلا إذا(src3_ګام == 0 && beta != 0.0)
        سېټ_ارزښت_2(dst, lddst, (fptype)0.0, d_m, d_n);

    إذا(typeid(fptype) == typeid(عائم))
        cblas_sgemm(Cblas_لیکه_Major, transA, transB, a_m, d_n, a_n, (عائم)alpha, (عائم*)src1, ldsrc1, (عائم*)src2, ldsrc2, (عائم)beta, (عائم*)dst, lddst);
    وإلا إذا(typeid(fptype) == typeid(مزدوج))
        cblas_dgemm(Cblas_لیکه_Major, transA, transB, a_m, d_n, a_n, (مزدوج)alpha, (مزدوج*)src1, ldsrc1, (مزدوج*)src2, ldsrc2, (مزدوج)beta, (مزدوج*)dst, lddst);

    أرجع CV_HAL_ERROR_OK;
}

کينډۍ <ډول_نوم fptype> ساكن مضمن صحيح
lapack_gemm_c(ثابت fptype *src1, size_t src1_ګام, ثابت fptype *src2, size_t src2_ګام, fptype alpha,
            ثابت fptype *src3, size_t src3_ګام, fptype beta, fptype *dst, size_t dst_ګام, صحيح a_m, صحيح a_n, صحيح d_n, صحيح flags)
{
    صحيح ldsrc1 = (صحيح)(src1_ګام / حجم(std::complex<fptype>));
    صحيح ldsrc2 = (صحيح)(src2_ګام / حجم(std::complex<fptype>));
    صحيح ldsrc3 = (صحيح)(src3_ګام / حجم(std::complex<fptype>));
    صحيح lddst = (صحيح)(dst_ګام / حجم(std::complex<fptype>));
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

    إذا(src3 != dst && beta != 0.0 && src3_ګام != 0) {
        إذا(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        وإلا
            copy_ماتریکس((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    وإلا إذا (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    وإلا إذا(src3_ګام == 0 && beta != 0.0)
        سېټ_ارزښت_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#إذا defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    صحيح M = a_m, N = d_n, K = a_n;
    إذا(typeid(fptype) == typeid(عائم)) {
        خپلکاری src1_cast = (std::complex<عائم>*)(src1);
        خپلکاری src2_cast = (std::complex<عائم>*)(src2);
        خپلکاری dst_cast = (std::complex<عائم>*)(dst);
        اوږد lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(Cblas_لیکه_Major, transA, transB, M, N, K, (std::complex<عائم>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<عائم>*)&cBeta, dst_cast, ldc);
    }
    وإلا إذا(typeid(fptype) == typeid(مزدوج)) {
        خپلکاری src1_cast = (std::complex<مزدوج>*)(src1);
        خپلکاری src2_cast = (std::complex<مزدوج>*)(src2);
        خپلکاری dst_cast = (std::complex<مزدوج>*)(dst);
        اوږد lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(Cblas_لیکه_Major, transA, transB, M, N, K, (std::complex<مزدوج>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<مزدوج>*)&cBeta, dst_cast, ldc);
    }
#وإلا
    إذا(typeid(fptype) == typeid(عائم))
        cblas_cgemm(Cblas_لیکه_Major, transA, transB, a_m, d_n, a_n, (عائم*)reinterpret_cast<fptype(&)[2]>(cAlpha), (عائم*)src1, ldsrc1, (عائم*)src2, ldsrc2, (عائم*)reinterpret_cast<fptype(&)[2]>(cBeta), (عائم*)dst, lddst);
    وإلا إذا(typeid(fptype) == typeid(مزدوج))
        cblas_zgemm(Cblas_لیکه_Major, transA, transB, a_m, d_n, a_n, (مزدوج*)reinterpret_cast<fptype(&)[2]>(cAlpha), (مزدوج*)src1, ldsrc1, (مزدوج*)src2, ldsrc2, (مزدوج*)reinterpret_cast<fptype(&)[2]>(cBeta), (مزدوج*)dst, lddst);
#د_که_پای

    أرجع CV_HAL_ERROR_OK;
}
صحيح lapack_LU32f(عائم* a, size_t a_ګام, صحيح m, عائم* b, size_t b_ګام, صحيح n, صحيح* info)
{
    إذا(m < HAL_LU_SMALL_ماتریکس_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_LU(a, a_ګام, m, b, b_ګام, n, info);
}

صحيح lapack_LU64f(مزدوج* a, size_t a_ګام, صحيح m, مزدوج* b, size_t b_ګام, صحيح n, صحيح* info)
{
    إذا(m < HAL_LU_SMALL_ماتریکس_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_LU(a, a_ګام, m, b, b_ګام, n, info);
}

صحيح lapack_Cholesky32f(عائم* a, size_t a_ګام, صحيح m, عائم* b, size_t b_ګام, صحيح n, منطقي *info)
{
    إذا(m < HAL_CHOLESKY_SMALL_ماتریکس_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_Cholesky(a, a_ګام, m, b, b_ګام, n, info);
}

صحيح lapack_Cholesky64f(مزدوج* a, size_t a_ګام, صحيح m, مزدوج* b, size_t b_ګام, صحيح n, منطقي *info)
{
    إذا(m < HAL_CHOLESKY_SMALL_ماتریکس_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_Cholesky(a, a_ګام, m, b, b_ګام, n, info);
}

صحيح lapack_SVD32f(عائم* a, size_t a_ګام, عائم *w, عائم* u, size_t u_ګام, عائم* vt, size_t v_ګام, صحيح m, صحيح n, صحيح flags)
{
    إذا(m < HAL_SVD_SMALL_ماتریکس_THRESH || n <= 0)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    صحيح info = 0;
    أرجع lapack_SVD(a, a_ګام, w, u, u_ګام, vt, v_ګام, m, n, flags, &info);
}

صحيح lapack_SVD64f(مزدوج* a, size_t a_ګام, مزدوج *w, مزدوج* u, size_t u_ګام, مزدوج* vt, size_t v_ګام, صحيح m, صحيح n, صحيح flags)
{
    إذا(m < HAL_SVD_SMALL_ماتریکس_THRESH || n <= 0)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    صحيح info = 0;
    أرجع lapack_SVD(a, a_ګام, w, u, u_ګام, vt, v_ګام, m, n, flags, &info);
}

صحيح lapack_QR32f(عائم* src1, size_t src1_ګام, صحيح m, صحيح n, صحيح k, عائم* src2, size_t src2_ګام, عائم* dst, صحيح* info)
{
    إذا (m < HAL_QR_SMALL_ماتریکس_THRESH)
      أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_QR(src1, src1_ګام, m, n, k, src2, src2_ګام, dst, info);
}

صحيح lapack_QR64f(مزدوج* src1, size_t src1_ګام, صحيح m, صحيح n, صحيح k, مزدوج* src2, size_t src2_ګام, مزدوج* dst, صحيح* info)
{
    إذا (m < HAL_QR_SMALL_ماتریکس_THRESH)
      أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_QR(src1, src1_ګام, m, n, k, src2, src2_ګام, dst, info);
}

صحيح lapack_gemm32f(ثابت عائم *src1, size_t src1_ګام, ثابت عائم *src2, size_t src2_ګام, عائم alpha,
                   ثابت عائم *src3, size_t src3_ګام, عائم beta, عائم *dst, size_t dst_ګام, صحيح m, صحيح n, صحيح k, صحيح flags)
{
    إذا(m < HAL_GEMM_SMALL_ماتریکس_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_gemm(src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m, n, k, flags);
}

صحيح lapack_gemm64f(ثابت مزدوج *src1, size_t src1_ګام, ثابت مزدوج *src2, size_t src2_ګام, مزدوج alpha,
                   ثابت مزدوج *src3, size_t src3_ګام, مزدوج beta, مزدوج *dst, size_t dst_ګام, صحيح m, صحيح n, صحيح k, صحيح flags)
{
    إذا(m < HAL_GEMM_SMALL_ماتریکس_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_gemm(src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m, n, k, flags);
}

صحيح lapack_gemm32fc(ثابت عائم *src1, size_t src1_ګام, ثابت عائم *src2, size_t src2_ګام, عائم alpha,
                   ثابت عائم *src3, size_t src3_ګام, عائم beta, عائم *dst, size_t dst_ګام, صحيح m, صحيح n, صحيح k, صحيح flags)
{
    إذا(m < HAL_GEMM_SMALL_COMPLEX_ماتریکس_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_gemm_c(src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m, n, k, flags);
}
صحيح lapack_gemm64fc(ثابت مزدوج *src1, size_t src1_ګام, ثابت مزدوج *src2, size_t src2_ګام, مزدوج alpha,
                   ثابت مزدوج *src3, size_t src3_ګام, مزدوج beta, مزدوج *dst, size_t dst_ګام, صحيح m, صحيح n, صحيح k, صحيح flags)
{
    إذا(m < HAL_GEMM_SMALL_COMPLEX_ماتریکس_THRESH)
        أرجع CV_HAL_ERROR_NOT_IMPLEMENTED;
    أرجع lapack_gemm_c(src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m, n, k, flags);
}

#إذا defined(__APPLE__) && defined(__clang__)
#لارښوونه clang diagnostic pop
#د_که_پای

#د_که_پای //HAVE_LAPACK
