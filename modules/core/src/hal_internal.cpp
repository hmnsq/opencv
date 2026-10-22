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

#यदि_परिभाषितम् HAVE_LAPACK

#अन्तर्भाव "opencv_lapack.h"

#अन्तर्भाव <cmath>
#अन्तर्भाव <algorithm>
#अन्तर्भाव <typeinfo>
#अन्तर्भाव <limits>
#अन्तर्भाव <complex>
#अन्तर्भाव <vector>

#परिभाषय HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#परिभाषय HAL_GEMM_SMALL_MATRIX_THRESH 100
#परिभाषय HAL_SVD_SMALL_MATRIX_THRESH 25
#परिभाषय HAL_QR_SMALL_MATRIX_THRESH 30
#परिभाषय HAL_LU_SMALL_MATRIX_THRESH 100
#परिभाषय HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#यदि defined(__clang__) && defined(__has_feature)
#यदि __has_feature(स्मृतिः_sanitizer)
#अन्तर्भाव <sanitizer/msan_interface.h>
#परिभाषय CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#परिभाषय CV_ANNOTATE_NO_SANITIZE_स्मृतिः __attribute__((no_sanitize("memory")))
#यदि_अन्तः
#यदि_अन्तः
#यदि_अपरिभाषितम् CV_ANNOTATE_स्मृतिः_IS_INITIALIZED
#परिभाषय CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(address, size) कुरु { } यावत्(0)
#परिभाषय CV_ANNOTATE_NO_SANITIZE_स्मृतिः
#यदि_अन्तः

#यदि defined(__APPLE__) && defined(__clang__)
#निर्देशः clang diagnostic push
#निर्देशः clang diagnostic ignored "-Wdeprecated-declarations"
#यदि_अन्तः

//lapack stores matrices in column-major order so transposing is needed everywhere
प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ शून्य
transpose_वर्गफलम्_inplace(fptype *src, size_t src_ld, size_t m)
{
    प्रति(size_t i = 0; i < m - 1; i++)
        प्रति(size_t j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ शून्य
transpose(ध्रुव fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    प्रति(size_t i = 0; i < m; i++)
        प्रति(size_t j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ शून्य
copy_matrix(ध्रुव fptype *src, size_t src_ld, fptype* dst, size_t dst_ld, size_t m, size_t n)
{
    प्रति(size_t i = 0; i < m; i++)
        प्रति(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ शून्य
समुच्चयः_मानम्_2(fptype *dst, size_t dst_ld, fptype value, size_t m, size_t n)
{
    प्रति(size_t i = 0; i < m; i++)
        प्रति(size_t j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ पूर्णाङ्क
CV_ANNOTATE_NO_SANITIZE_स्मृतिः lapack_LU(fptype* a, size_t a_चरणः, पूर्णाङ्क m, fptype* b, size_t b_चरणः, पूर्णाङ्क n, पूर्णाङ्क* info)
{
#यदि defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::Auto_बफर<दीर्घ> piv_buff(m);
    दीर्घ lda = (दीर्घ)(a_चरणः / परिमाण(fptype));
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ _info[1];
#अन्यथा
    cv::Auto_बफर<पूर्णाङ्क> piv_buff(m);
    पूर्णाङ्क lda = (पूर्णाङ्क)(a_चरणः / परिमाण(fptype));
    पूर्णाङ्क _m = m, _n = n;
    पूर्णाङ्क* _info = info;
#यदि_अन्तः
    स्वचालित piv = piv_buff.दत्त();

    transpose_वर्गफलम्_inplace(a, lda, m);

    यदि(b)
    {
        यदि(n == 1 && b_चरणः == परिमाण(fptype))
        {
            यदि(typeid(fptype) == typeid(भिन्न))
                sgesv_(&_m, &_n, (भिन्न*)a, &lda, piv, (भिन्न*)b, &_m, _info);
            अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
                dgesv_(&_m, &_n, (द्विगुण*)a, &lda, piv, (द्विगुण*)b, &_m, _info);
        }
        अन्यथा
        {
            पूर्णाङ्क ldb = (पूर्णाङ्क)(b_चरणः / परिमाण(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            यदि(typeid(fptype) == typeid(भिन्न))
                sgesv_(&_m, &_n, (भिन्न*)a, &lda, piv, (भिन्न*)&tmpB[0], &_m, _info);
            अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
                dgesv_(&_m, &_n, (द्विगुण*)a, &lda, piv, (द्विगुण*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    अन्यथा
    {
        यदि(typeid(fptype) == typeid(भिन्न))
            sgetrf_(&_m, &_m, (भिन्न*)a, &lda, piv, _info);
        अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
            dgetrf_(&_m, &_m, (द्विगुण*)a, &lda, piv, _info);
    }

#यदि defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णाङ्क>(_info[0]);
#यदि_अन्तः
    पूर्णाङ्क retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    पूर्णाङ्क sign = 0;
    यदि(*info == 0)
    {
        प्रति(पूर्णाङ्क i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    अन्यथा
        *info = 0; //in opencv LU function zero means error

    प्रत्यागम retcode;
}

प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_Cholesky(fptype* a, size_t a_चरणः, पूर्णाङ्क m, fptype* b, size_t b_चरणः, पूर्णाङ्क n, तार्किक* info)
{
#यदि defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ lapack_स्थितिः = 0;
    दीर्घ lda = (दीर्घ)(a_चरणः / परिमाण(fptype));
#अन्यथा
    पूर्णाङ्क _m = m, _n = n;
    पूर्णाङ्क lapack_स्थितिः = 0;
    पूर्णाङ्क lda = (पूर्णाङ्क)(a_चरणः / परिमाण(fptype));
#यदि_अन्तः
    वर्ण L[] = {'L', '\0'};

    यदि(b)
    {
        यदि(n == 1 && b_चरणः == परिमाण(fptype))
        {
            यदि(typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (भिन्न*)a, &lda, (भिन्न*)b, &_m, &lapack_स्थितिः);
            अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)b, &_m, &lapack_स्थितिः);
        }
        अन्यथा
        {
            पूर्णाङ्क ldb = (पूर्णाङ्क)(b_चरणः / परिमाण(fptype));
            fptype* tmpB = नूतन fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            यदि(typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (भिन्न*)a, &lda, (भिन्न*)tmpB, &_m, &lapack_स्थितिः);
            अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)tmpB, &_m, &lapack_स्थितिः);

            transpose(tmpB, m, b, ldb, n, m);
            विलोपय[] tmpB;
        }
    }
    अन्यथा
    {
        यदि(typeid(fptype) == typeid(भिन्न))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (भिन्न*)a, &lda, &lapack_स्थितिः);
        अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (द्विगुण*)a, &lda, &lapack_स्थितिः);
    }

    यदि(lapack_स्थितिः == 0) *info = सत्य;
    अन्यथा *info = असत्य; //in opencv Cholesky function false means error

    प्रत्यागम lapack_स्थितिः >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_SVD(fptype* a, size_t a_चरणः, fptype *w, fptype* u, size_t u_चरणः, fptype* vt, size_t v_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags, पूर्णाङ्क* info)
{
#यदि defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ _info[1];
    दीर्घ lda = (दीर्घ)(a_चरणः / परिमाण(fptype));
    दीर्घ ldv = (दीर्घ)(v_चरणः / परिमाण(fptype));
    दीर्घ ldu = (दीर्घ)(u_चरणः / परिमाण(fptype));
    दीर्घ lwork = -1;
    cv::Auto_बफर<दीर्घ> iworkBuf_(8 * std::min(m, n));
#अन्यथा
    पूर्णाङ्क _m = m, _n = n;
    पूर्णाङ्क* _info = info;
    पूर्णाङ्क lda = (पूर्णाङ्क)(a_चरणः / परिमाण(fptype));
    पूर्णाङ्क ldv = (पूर्णाङ्क)(v_चरणः / परिमाण(fptype));
    पूर्णाङ्क ldu = (पूर्णाङ्क)(u_चरणः / परिमाण(fptype));
    पूर्णाङ्क lwork = -1;
    cv::Auto_बफर<पूर्णाङ्क> iworkBuf_(8 * std::min(m, n));
#यदि_अन्तः
    स्वचालित iworkBuf = iworkBuf_.दत्त();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    वर्ण बहुलकम्[] = { ' ', '\0'};
    यदि(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        बहुलकम्[0] = 'N';
    }
    अन्यथा यदि((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        बहुलकम्[0] = 'O';
    अन्यथा यदि((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        बहुलकम्[0] = 'S';
    अन्यथा यदि(flags & CV_HAL_SVD_पूर्णम्_UV) //full SVD, U stored in u or in a
        बहुलकम्[0] = 'A';

    यदि((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_पूर्णम्_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    यदि(typeid(fptype) == typeid(भिन्न))
        OCV_LAPACK_FUNC(sgesdd)(बहुलकम्, &_m, &_n, (भिन्न*)a, &lda, (भिन्न*)w, (भिन्न*)u, &ldu, (भिन्न*)vt, &ldv, (भिन्न*)&work1, &lwork, iworkBuf, _info);
    अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
        OCV_LAPACK_FUNC(dgesdd)(बहुलकम्, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)w, (द्विगुण*)u, &ldu, (द्विगुण*)vt, &ldv, (द्विगुण*)&work1, &lwork, iworkBuf, _info);

    यदि(*info < 0)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (पूर्णाङ्क)round(work1); //optimal buffer size
    std::vector<fptype> बफर(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(बफर.दत्त(), परिमाण(fptype) * (lwork + 1));

    यदि(typeid(fptype) == typeid(भिन्न))
        OCV_LAPACK_FUNC(sgesdd)(बहुलकम्, &_m, &_n, (भिन्न*)a, &lda, (भिन्न*)w, (भिन्न*)u, &ldu, (भिन्न*)vt, &ldv, (भिन्न*)&बफर[0], &lwork, iworkBuf, _info);
    अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
        OCV_LAPACK_FUNC(dgesdd)(बहुलकम्, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)w, (द्विगुण*)u, &ldu, (द्विगुण*)vt, &ldv, (द्विगुण*)&बफर[0], &lwork, iworkBuf, _info);

#यदि defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णाङ्क>(_info[0]);
#यदि_अन्तः

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(a, a_चरणः * n);
    यदि (u)
      CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(u, u_चरणः * m);
    यदि (vt)
      CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(vt, v_चरणः * n);
    यदि (w)
      CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(w, परिमाण(fptype) * std::min(m, n));

    यदि(!(flags & CV_HAL_SVD_NO_UV))
        transpose_वर्गफलम्_inplace(vt, ldv, n);

    यदि((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_पूर्णम्_UV))
    {
        प्रति(पूर्णाङ्क i = 0; i < m; i++)
            प्रति(पूर्णाङ्क j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    यदि(*info < 0)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम CV_HAL_ERROR_OK;
}

प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_QR(fptype* a, size_t a_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, fptype* b, size_t b_चरणः, fptype* dst, पूर्णाङ्क* info)
{
#यदि defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n), _k = static_cast<दीर्घ>(k);
    दीर्घ _info[1];
    दीर्घ lda = (दीर्घ)(a_चरणः / परिमाण(fptype));
    दीर्घ lwork = -1;
    दीर्घ ldtmpA;
#अन्यथा
    पूर्णाङ्क _m = m, _n = n, _k = k;
    पूर्णाङ्क* _info = info;
    पूर्णाङ्क lda = (पूर्णाङ्क)(a_चरणः / परिमाण(fptype));
    पूर्णाङ्क lwork = -1;
    पूर्णाङ्क ldtmpA;
#यदि_अन्तः

    वर्ण बहुलकम्[] = { 'N', '\0' };
    यदि(m < n)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    यदि (m == n)
    {
        transpose_वर्गफलम्_inplace(a, lda, m);
        tmpA = a;
        ldtmpA = lda;
    }
    अन्यथा
    {
        tmpAMemHolder.resize(m*n);
        tmpA = &tmpAMemHolder.front();
        ldtmpA = m;
        transpose(a, lda, tmpA, m, m, n);
    }

    fptype work1 = 0.;

    यदि (b)
    {
        यदि (k == 1 && b_चरणः == परिमाण(fptype))
        {
            यदि (typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sgels)(बहुलकम्, &_m, &_n, &_k, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)b, &_m, (भिन्न*)&work1, &lwork, _info);
            अन्यथा यदि (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(बहुलकम्, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)b, &_m, (द्विगुण*)&work1, &lwork, _info);

            CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(info, परिमाण(पूर्णाङ्क));
            यदि (*info < 0)
                प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_आवर्तनम्(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* बफर = &workBufMemHolder.front();

            यदि (typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sgels)(बहुलकम्, &_m, &_n, &_k, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)b, &_m, (भिन्न*)बफर, &lwork, _info);
            अन्यथा यदि (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(बहुलकम्, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)b, &_m, (द्विगुण*)बफर, &lwork, _info);
        }
        अन्यथा
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            पूर्णाङ्क ldb = (पूर्णाङ्क)(b_चरणः / परिमाण(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            यदि (typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sgels)(बहुलकम्, &_m, &_n, &_k, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)tmpB, &_m, (भिन्न*)&work1, &lwork, _info);
            अन्यथा यदि (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(बहुलकम्, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)tmpB, &_m, (द्विगुण*)&work1, &lwork, _info);

            CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(info, परिमाण(पूर्णाङ्क));
            यदि (*info < 0)
                प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cv_आवर्तनम्(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* बफर = &workBufMemHolder.front();

            यदि (typeid(fptype) == typeid(भिन्न))
                OCV_LAPACK_FUNC(sgels)(बहुलकम्, &_m, &_n, &_k, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)tmpB, &_m, (भिन्न*)बफर, &lwork, _info);
            अन्यथा यदि (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(बहुलकम्, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)tmpB, &_m, (द्विगुण*)बफर, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    अन्यथा
    {
        यदि (typeid(fptype) == typeid(भिन्न))
            sgeqrf_(&_m, &_n, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)dst, (भिन्न*)&work1, &lwork, _info);
        अन्यथा यदि (typeid(fptype) == typeid(द्विगुण))
            dgeqrf_(&_m, &_n, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)dst, (द्विगुण*)&work1, &lwork, _info);

        CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(info, परिमाण(पूर्णाङ्क));
        यदि (*info < 0)
            प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cv_आवर्तनम्(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* बफर = &workBufMemHolder.front();

        यदि (typeid(fptype) == typeid(भिन्न))
            sgeqrf_(&_m, &_n, (भिन्न*)tmpA, &ldtmpA, (भिन्न*)dst, (भिन्न*)बफर, &lwork, _info);
        अन्यथा यदि (typeid(fptype) == typeid(द्विगुण))
            dgeqrf_(&_m, &_n, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)dst, (द्विगुण*)बफर, &lwork, _info);
    }

    CV_ANNOTATE_स्मृतिः_IS_INITIALIZED(info, परिमाण(पूर्णाङ्क));
    यदि (m == n)
        transpose_वर्गफलम्_inplace(a, lda, m);
    अन्यथा
        transpose(tmpA, m, a, lda, n, m);

#यदि defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णाङ्क>(_info[0]);
#यदि_अन्तः

    यदि (*info != 0)
        *info = 0;
    अन्यथा
        *info = 1;

    प्रत्यागम CV_HAL_ERROR_OK;
}

प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_gemm(ध्रुव fptype *src1, size_t src1_चरणः, ध्रुव fptype *src2, size_t src2_चरणः, fptype alpha,
            ध्रुव fptype *src3, size_t src3_चरणः, fptype beta, fptype *dst, size_t dst_चरणः, पूर्णाङ्क a_m, पूर्णाङ्क a_n, पूर्णाङ्क d_n, पूर्णाङ्क flags)
{
    पूर्णाङ्क ldsrc1 = (पूर्णाङ्क)(src1_चरणः / परिमाण(fptype));
    पूर्णाङ्क ldsrc2 = (पूर्णाङ्क)(src2_चरणः / परिमाण(fptype));
    पूर्णाङ्क ldsrc3 = (पूर्णाङ्क)(src3_चरणः / परिमाण(fptype));
    पूर्णाङ्क lddst = (पूर्णाङ्क)(dst_चरणः / परिमाण(fptype));
    पूर्णाङ्क c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    यदि(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        यदि(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अन्यथा
        {
            d_m = a_m;
        }
    }
    अन्यथा
    {
        transB = CblasNoTrans;
        यदि(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अन्यथा
        {
            d_m = a_m;
        }
    }

    यदि(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    अन्यथा
    {
        c_m = d_m;
        c_n = d_n;
    }

    यदि(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    अन्यथा
    {
        transA = CblasNoTrans;
    }

    यदि(src3 != dst && beta != 0.0 && src3_चरणः != 0) {
        यदि(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        अन्यथा
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    अन्यथा यदि (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    अन्यथा यदि(src3_चरणः == 0 && beta != 0.0)
        समुच्चयः_मानम्_2(dst, lddst, (fptype)0.0, d_m, d_n);

    यदि(typeid(fptype) == typeid(भिन्न))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (भिन्न)alpha, (भिन्न*)src1, ldsrc1, (भिन्न*)src2, ldsrc2, (भिन्न)beta, (भिन्न*)dst, lddst);
    अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (द्विगुण)alpha, (द्विगुण*)src1, ldsrc1, (द्विगुण*)src2, ldsrc2, (द्विगुण)beta, (द्विगुण*)dst, lddst);

    प्रत्यागम CV_HAL_ERROR_OK;
}

प्रतिरूपम् <प्रकारनाम fptype> स्थिर अन्तःस्थ पूर्णाङ्क
lapack_gemm_c(ध्रुव fptype *src1, size_t src1_चरणः, ध्रुव fptype *src2, size_t src2_चरणः, fptype alpha,
            ध्रुव fptype *src3, size_t src3_चरणः, fptype beta, fptype *dst, size_t dst_चरणः, पूर्णाङ्क a_m, पूर्णाङ्क a_n, पूर्णाङ्क d_n, पूर्णाङ्क flags)
{
    पूर्णाङ्क ldsrc1 = (पूर्णाङ्क)(src1_चरणः / परिमाण(std::complex<fptype>));
    पूर्णाङ्क ldsrc2 = (पूर्णाङ्क)(src2_चरणः / परिमाण(std::complex<fptype>));
    पूर्णाङ्क ldsrc3 = (पूर्णाङ्क)(src3_चरणः / परिमाण(std::complex<fptype>));
    पूर्णाङ्क lddst = (पूर्णाङ्क)(dst_चरणः / परिमाण(std::complex<fptype>));
    पूर्णाङ्क c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    यदि(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        यदि(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अन्यथा
        {
            d_m = a_m;
        }
    }
    अन्यथा
    {
        transB = CblasNoTrans;
        यदि(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अन्यथा
        {
            d_m = a_m;
        }
    }

    यदि(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    अन्यथा
    {
        c_m = d_m;
        c_n = d_n;
    }

    यदि(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    अन्यथा
    {
        transA = CblasNoTrans;
    }

    यदि(src3 != dst && beta != 0.0 && src3_चरणः != 0) {
        यदि(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        अन्यथा
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    अन्यथा यदि (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    अन्यथा यदि(src3_चरणः == 0 && beta != 0.0)
        समुच्चयः_मानम्_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#यदि defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    पूर्णाङ्क M = a_m, N = d_n, K = a_n;
    यदि(typeid(fptype) == typeid(भिन्न)) {
        स्वचालित src1_cast = (std::complex<भिन्न>*)(src1);
        स्वचालित src2_cast = (std::complex<भिन्न>*)(src2);
        स्वचालित dst_cast = (std::complex<भिन्न>*)(dst);
        दीर्घ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<भिन्न>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<भिन्न>*)&cBeta, dst_cast, ldc);
    }
    अन्यथा यदि(typeid(fptype) == typeid(द्विगुण)) {
        स्वचालित src1_cast = (std::complex<द्विगुण>*)(src1);
        स्वचालित src2_cast = (std::complex<द्विगुण>*)(src2);
        स्वचालित dst_cast = (std::complex<द्विगुण>*)(dst);
        दीर्घ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<द्विगुण>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<द्विगुण>*)&cBeta, dst_cast, ldc);
    }
#अन्यथा
    यदि(typeid(fptype) == typeid(भिन्न))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (भिन्न*)reinterpret_cast<fptype(&)[2]>(cAlpha), (भिन्न*)src1, ldsrc1, (भिन्न*)src2, ldsrc2, (भिन्न*)reinterpret_cast<fptype(&)[2]>(cBeta), (भिन्न*)dst, lddst);
    अन्यथा यदि(typeid(fptype) == typeid(द्विगुण))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (द्विगुण*)reinterpret_cast<fptype(&)[2]>(cAlpha), (द्विगुण*)src1, ldsrc1, (द्विगुण*)src2, ldsrc2, (द्विगुण*)reinterpret_cast<fptype(&)[2]>(cBeta), (द्विगुण*)dst, lddst);
#यदि_अन्तः

    प्रत्यागम CV_HAL_ERROR_OK;
}
पूर्णाङ्क lapack_LU32f(भिन्न* a, size_t a_चरणः, पूर्णाङ्क m, भिन्न* b, size_t b_चरणः, पूर्णाङ्क n, पूर्णाङ्क* info)
{
    यदि(m < HAL_LU_SMALL_MATRIX_THRESH)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_LU(a, a_चरणः, m, b, b_चरणः, n, info);
}

पूर्णाङ्क lapack_LU64f(द्विगुण* a, size_t a_चरणः, पूर्णाङ्क m, द्विगुण* b, size_t b_चरणः, पूर्णाङ्क n, पूर्णाङ्क* info)
{
    यदि(m < HAL_LU_SMALL_MATRIX_THRESH)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_LU(a, a_चरणः, m, b, b_चरणः, n, info);
}

पूर्णाङ्क lapack_Cholesky32f(भिन्न* a, size_t a_चरणः, पूर्णाङ्क m, भिन्न* b, size_t b_चरणः, पूर्णाङ्क n, तार्किक *info)
{
    यदि(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_Cholesky(a, a_चरणः, m, b, b_चरणः, n, info);
}

पूर्णाङ्क lapack_Cholesky64f(द्विगुण* a, size_t a_चरणः, पूर्णाङ्क m, द्विगुण* b, size_t b_चरणः, पूर्णाङ्क n, तार्किक *info)
{
    यदि(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_Cholesky(a, a_चरणः, m, b, b_चरणः, n, info);
}

पूर्णाङ्क lapack_SVD32f(भिन्न* a, size_t a_चरणः, भिन्न *w, भिन्न* u, size_t u_चरणः, भिन्न* vt, size_t v_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags)
{
    यदि(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    पूर्णाङ्क info = 0;
    प्रत्यागम lapack_SVD(a, a_चरणः, w, u, u_चरणः, vt, v_चरणः, m, n, flags, &info);
}

पूर्णाङ्क lapack_SVD64f(द्विगुण* a, size_t a_चरणः, द्विगुण *w, द्विगुण* u, size_t u_चरणः, द्विगुण* vt, size_t v_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags)
{
    यदि(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    पूर्णाङ्क info = 0;
    प्रत्यागम lapack_SVD(a, a_चरणः, w, u, u_चरणः, vt, v_चरणः, m, n, flags, &info);
}

पूर्णाङ्क lapack_QR32f(भिन्न* src1, size_t src1_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, भिन्न* src2, size_t src2_चरणः, भिन्न* dst, पूर्णाङ्क* info)
{
    यदि (m < HAL_QR_SMALL_MATRIX_THRESH)
      प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_QR(src1, src1_चरणः, m, n, k, src2, src2_चरणः, dst, info);
}

पूर्णाङ्क lapack_QR64f(द्विगुण* src1, size_t src1_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, द्विगुण* src2, size_t src2_चरणः, द्विगुण* dst, पूर्णाङ्क* info)
{
    यदि (m < HAL_QR_SMALL_MATRIX_THRESH)
      प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_QR(src1, src1_चरणः, m, n, k, src2, src2_चरणः, dst, info);
}

पूर्णाङ्क lapack_gemm32f(ध्रुव भिन्न *src1, size_t src1_चरणः, ध्रुव भिन्न *src2, size_t src2_चरणः, भिन्न alpha,
                   ध्रुव भिन्न *src3, size_t src3_चरणः, भिन्न beta, भिन्न *dst, size_t dst_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags)
{
    यदि(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_gemm(src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m, n, k, flags);
}

पूर्णाङ्क lapack_gemm64f(ध्रुव द्विगुण *src1, size_t src1_चरणः, ध्रुव द्विगुण *src2, size_t src2_चरणः, द्विगुण alpha,
                   ध्रुव द्विगुण *src3, size_t src3_चरणः, द्विगुण beta, द्विगुण *dst, size_t dst_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags)
{
    यदि(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_gemm(src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m, n, k, flags);
}

पूर्णाङ्क lapack_gemm32fc(ध्रुव भिन्न *src1, size_t src1_चरणः, ध्रुव भिन्न *src2, size_t src2_चरणः, भिन्न alpha,
                   ध्रुव भिन्न *src3, size_t src3_चरणः, भिन्न beta, भिन्न *dst, size_t dst_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags)
{
    यदि(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_gemm_c(src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m, n, k, flags);
}
पूर्णाङ्क lapack_gemm64fc(ध्रुव द्विगुण *src1, size_t src1_चरणः, ध्रुव द्विगुण *src2, size_t src2_चरणः, द्विगुण alpha,
                   ध्रुव द्विगुण *src3, size_t src3_चरणः, द्विगुण beta, द्विगुण *dst, size_t dst_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags)
{
    यदि(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED;
    प्रत्यागम lapack_gemm_c(src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m, n, k, flags);
}

#यदि defined(__APPLE__) && defined(__clang__)
#निर्देशः clang diagnostic pop
#यदि_अन्तः

#यदि_अन्तः //HAVE_LAPACK
