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

#समावेश "precomp.hpp"
#समावेश "hal_internal.hpp"

#अगर_घोषित HAVE_LAPACK

#समावेश "opencv_lapack.h"

#समावेश <cmath>
#समावेश <algorithm>
#समावेश <typeinfo>
#समावेश <limits>
#समावेश <complex>
#समावेश <vector>

#घोषणा HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH 100
#घोषणा HAL_GEMM_SMALL_MATRIX_THRESH 100
#घोषणा HAL_SVD_SMALL_MATRIX_THRESH 25
#घोषणा HAL_QR_SMALL_MATRIX_THRESH 30
#घोषणा HAL_LU_SMALL_MATRIX_THRESH 100
#घोषणा HAL_CHOLESKY_SMALL_MATRIX_THRESH 100

#अगर defined(__clang__) && defined(__has_feature)
#अगर __has_feature(memory_sanitizer)
#समावेश <sanitizer/msan_interface.h>
#घोषणा CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#घोषणा CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#पूर्ण_अगर
#पूर्ण_अगर
#अगर_अघोषित CV_ANNOTATE_MEMORY_IS_INITIALIZED
#घोषणा CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) करो { } जबतक(0)
#घोषणा CV_ANNOTATE_NO_SANITIZE_MEMORY
#पूर्ण_अगर

#अगर defined(__APPLE__) && defined(__clang__)
#आशय clang diagnostic push
#आशय clang diagnostic ignored "-Wdeprecated-declarations"
#पूर्ण_अगर

//lapack stores matrices in column-major order so transposing is needed everywhere
ढाँचा <typename fptype> अटल अंतरभूत व्योम
transpose_square_inplace(fptype *src, माप_प्रकार src_ld, माप_प्रकार m)
{
    क्रम(माप_प्रकार i = 0; i < m - 1; i++)
        क्रम(माप_प्रकार j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

ढाँचा <typename fptype> अटल अंतरभूत व्योम
transpose(स्थिर fptype *src, माप_प्रकार src_ld, fptype* dst, माप_प्रकार dst_ld, माप_प्रकार m, माप_प्रकार n)
{
    क्रम(माप_प्रकार i = 0; i < m; i++)
        क्रम(माप_प्रकार j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

ढाँचा <typename fptype> अटल अंतरभूत व्योम
copy_matrix(स्थिर fptype *src, माप_प्रकार src_ld, fptype* dst, माप_प्रकार dst_ld, माप_प्रकार m, माप_प्रकार n)
{
    क्रम(माप_प्रकार i = 0; i < m; i++)
        क्रम(माप_प्रकार j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

ढाँचा <typename fptype> अटल अंतरभूत व्योम
set_मूल्य_2(fptype *dst, माप_प्रकार dst_ld, fptype value, माप_प्रकार m, माप_प्रकार n)
{
    क्रम(माप_प्रकार i = 0; i < m; i++)
        क्रम(माप_प्रकार j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
ढाँचा <typename fptype> अटल अंतरभूत पूर्णांक
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, माप_प्रकार a_step, पूर्णांक m, fptype* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info)
{
#अगर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::Auto_बफर<दीर्घ> piv_buff(m);
    दीर्घ lda = (दीर्घ)(a_step / माप(fptype));
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ _info[1];
#अन्यथा
    cv::Auto_बफर<पूर्णांक> piv_buff(m);
    पूर्णांक lda = (पूर्णांक)(a_step / माप(fptype));
    पूर्णांक _m = m, _n = n;
    पूर्णांक* _info = info;
#पूर्ण_अगर
    स्वतः piv = piv_buff.तथ्य();

    transpose_square_inplace(a, lda, m);

    अगर(b)
    {
        अगर(n == 1 && b_step == माप(fptype))
        {
            अगर(typeid(fptype) == typeid(भग्न))
                sgesv_(&_m, &_n, (भग्न*)a, &lda, piv, (भग्न*)b, &_m, _info);
            अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
                dgesv_(&_m, &_n, (द्विगुन*)a, &lda, piv, (द्विगुन*)b, &_m, _info);
        }
        अन्यथा
        {
            पूर्णांक ldb = (पूर्णांक)(b_step / माप(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            अगर(typeid(fptype) == typeid(भग्न))
                sgesv_(&_m, &_n, (भग्न*)a, &lda, piv, (भग्न*)&tmpB[0], &_m, _info);
            अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
                dgesv_(&_m, &_n, (द्विगुन*)a, &lda, piv, (द्विगुन*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    अन्यथा
    {
        अगर(typeid(fptype) == typeid(भग्न))
            sgetrf_(&_m, &_m, (भग्न*)a, &lda, piv, _info);
        अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
            dgetrf_(&_m, &_m, (द्विगुन*)a, &lda, piv, _info);
    }

#अगर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णांक>(_info[0]);
#पूर्ण_अगर
    पूर्णांक retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    पूर्णांक sign = 0;
    अगर(*info == 0)
    {
        क्रम(पूर्णांक i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    अन्यथा
        *info = 0; //in opencv LU function zero means error

    वापस retcode;
}

ढाँचा <typename fptype> अटल अंतरभूत पूर्णांक
lapack_Cholesky(fptype* a, माप_प्रकार a_step, पूर्णांक m, fptype* b, माप_प्रकार b_step, पूर्णांक n, bool* info)
{
#अगर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ lapackStatus = 0;
    दीर्घ lda = (दीर्घ)(a_step / माप(fptype));
#अन्यथा
    पूर्णांक _m = m, _n = n;
    पूर्णांक lapackStatus = 0;
    पूर्णांक lda = (पूर्णांक)(a_step / माप(fptype));
#पूर्ण_अगर
    अक्षर L[] = {'L', '\0'};

    अगर(b)
    {
        अगर(n == 1 && b_step == माप(fptype))
        {
            अगर(typeid(fptype) == typeid(भग्न))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (भग्न*)a, &lda, (भग्न*)b, &_m, &lapackStatus);
            अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (द्विगुन*)a, &lda, (द्विगुन*)b, &_m, &lapackStatus);
        }
        अन्यथा
        {
            पूर्णांक ldb = (पूर्णांक)(b_step / माप(fptype));
            fptype* tmpB = नया fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            अगर(typeid(fptype) == typeid(भग्न))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (भग्न*)a, &lda, (भग्न*)tmpB, &_m, &lapackStatus);
            अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (द्विगुन*)a, &lda, (द्विगुन*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            मिटाओ[] tmpB;
        }
    }
    अन्यथा
    {
        अगर(typeid(fptype) == typeid(भग्न))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (भग्न*)a, &lda, &lapackStatus);
        अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (द्विगुन*)a, &lda, &lapackStatus);
    }

    अगर(lapackStatus == 0) *info = true;
    अन्यथा *info = false; //in opencv Cholesky function false means error

    वापस lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

ढाँचा <typename fptype> अटल अंतरभूत पूर्णांक
lapack_SVD(fptype* a, माप_प्रकार a_step, fptype *w, fptype* u, माप_प्रकार u_step, fptype* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags, पूर्णांक* info)
{
#अगर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ _info[1];
    दीर्घ lda = (दीर्घ)(a_step / माप(fptype));
    दीर्घ ldv = (दीर्घ)(v_step / माप(fptype));
    दीर्घ ldu = (दीर्घ)(u_step / माप(fptype));
    दीर्घ lwork = -1;
    cv::Auto_बफर<दीर्घ> iworkBuf_(8 * std::min(m, n));
#अन्यथा
    पूर्णांक _m = m, _n = n;
    पूर्णांक* _info = info;
    पूर्णांक lda = (पूर्णांक)(a_step / माप(fptype));
    पूर्णांक ldv = (पूर्णांक)(v_step / माप(fptype));
    पूर्णांक ldu = (पूर्णांक)(u_step / माप(fptype));
    पूर्णांक lwork = -1;
    cv::Auto_बफर<पूर्णांक> iworkBuf_(8 * std::min(m, n));
#पूर्ण_अगर
    स्वतः iworkBuf = iworkBuf_.तथ्य();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    अक्षर mode[] = { ' ', '\0'};
    अगर(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    अन्यथा अगर((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    अन्यथा अगर((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    अन्यथा अगर(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    अगर((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    अगर(typeid(fptype) == typeid(भग्न))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (भग्न*)a, &lda, (भग्न*)w, (भग्न*)u, &ldu, (भग्न*)vt, &ldv, (भग्न*)&work1, &lwork, iworkBuf, _info);
    अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (द्विगुन*)a, &lda, (द्विगुन*)w, (द्विगुन*)u, &ldu, (द्विगुन*)vt, &ldv, (द्विगुन*)&work1, &lwork, iworkBuf, _info);

    अगर(*info < 0)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (पूर्णांक)round(work1); //optimal buffer size
    std::vector<fptype> बफर(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(बफर.तथ्य(), माप(fptype) * (lwork + 1));

    अगर(typeid(fptype) == typeid(भग्न))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (भग्न*)a, &lda, (भग्न*)w, (भग्न*)u, &ldu, (भग्न*)vt, &ldv, (भग्न*)&बफर[0], &lwork, iworkBuf, _info);
    अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (द्विगुन*)a, &lda, (द्विगुन*)w, (द्विगुन*)u, &ldu, (द्विगुन*)vt, &ldv, (द्विगुन*)&बफर[0], &lwork, iworkBuf, _info);

#अगर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णांक>(_info[0]);
#पूर्ण_अगर

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    अगर (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    अगर (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    अगर (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, माप(fptype) * std::min(m, n));

    अगर(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    अगर((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        क्रम(पूर्णांक i = 0; i < m; i++)
            क्रम(पूर्णांक j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    अगर(*info < 0)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस CV_HAL_ERROR_OK;
}

ढाँचा <typename fptype> अटल अंतरभूत पूर्णांक
lapack_QR(fptype* a, माप_प्रकार a_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, fptype* b, माप_प्रकार b_step, fptype* dst, पूर्णांक* info)
{
#अगर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n), _k = static_cast<दीर्घ>(k);
    दीर्घ _info[1];
    दीर्घ lda = (दीर्घ)(a_step / माप(fptype));
    दीर्घ lwork = -1;
    दीर्घ ldtmpA;
#अन्यथा
    पूर्णांक _m = m, _n = n, _k = k;
    पूर्णांक* _info = info;
    पूर्णांक lda = (पूर्णांक)(a_step / माप(fptype));
    पूर्णांक lwork = -1;
    पूर्णांक ldtmpA;
#पूर्ण_अगर

    अक्षर mode[] = { 'N', '\0' };
    अगर(m < n)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    अगर (m == n)
    {
        transpose_square_inplace(a, lda, m);
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

    अगर (b)
    {
        अगर (k == 1 && b_step == माप(fptype))
        {
            अगर (typeid(fptype) == typeid(भग्न))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (भग्न*)tmpA, &ldtmpA, (भग्न*)b, &_m, (भग्न*)&work1, &lwork, _info);
            अन्यथा अगर (typeid(fptype) == typeid(द्विगुन))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुन*)tmpA, &ldtmpA, (द्विगुन*)b, &_m, (द्विगुन*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, माप(पूर्णांक));
            अगर (*info < 0)
                वापस CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* बफर = &workBufMemHolder.front();

            अगर (typeid(fptype) == typeid(भग्न))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (भग्न*)tmpA, &ldtmpA, (भग्न*)b, &_m, (भग्न*)बफर, &lwork, _info);
            अन्यथा अगर (typeid(fptype) == typeid(द्विगुन))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुन*)tmpA, &ldtmpA, (द्विगुन*)b, &_m, (द्विगुन*)बफर, &lwork, _info);
        }
        अन्यथा
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            पूर्णांक ldb = (पूर्णांक)(b_step / माप(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            अगर (typeid(fptype) == typeid(भग्न))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (भग्न*)tmpA, &ldtmpA, (भग्न*)tmpB, &_m, (भग्न*)&work1, &lwork, _info);
            अन्यथा अगर (typeid(fptype) == typeid(द्विगुन))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुन*)tmpA, &ldtmpA, (द्विगुन*)tmpB, &_m, (द्विगुन*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, माप(पूर्णांक));
            अगर (*info < 0)
                वापस CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* बफर = &workBufMemHolder.front();

            अगर (typeid(fptype) == typeid(भग्न))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (भग्न*)tmpA, &ldtmpA, (भग्न*)tmpB, &_m, (भग्न*)बफर, &lwork, _info);
            अन्यथा अगर (typeid(fptype) == typeid(द्विगुन))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुन*)tmpA, &ldtmpA, (द्विगुन*)tmpB, &_m, (द्विगुन*)बफर, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    अन्यथा
    {
        अगर (typeid(fptype) == typeid(भग्न))
            sgeqrf_(&_m, &_n, (भग्न*)tmpA, &ldtmpA, (भग्न*)dst, (भग्न*)&work1, &lwork, _info);
        अन्यथा अगर (typeid(fptype) == typeid(द्विगुन))
            dgeqrf_(&_m, &_n, (द्विगुन*)tmpA, &ldtmpA, (द्विगुन*)dst, (द्विगुन*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, माप(पूर्णांक));
        अगर (*info < 0)
            वापस CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* बफर = &workBufMemHolder.front();

        अगर (typeid(fptype) == typeid(भग्न))
            sgeqrf_(&_m, &_n, (भग्न*)tmpA, &ldtmpA, (भग्न*)dst, (भग्न*)बफर, &lwork, _info);
        अन्यथा अगर (typeid(fptype) == typeid(द्विगुन))
            dgeqrf_(&_m, &_n, (द्विगुन*)tmpA, &ldtmpA, (द्विगुन*)dst, (द्विगुन*)बफर, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, माप(पूर्णांक));
    अगर (m == n)
        transpose_square_inplace(a, lda, m);
    अन्यथा
        transpose(tmpA, m, a, lda, n, m);

#अगर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णांक>(_info[0]);
#पूर्ण_अगर

    अगर (*info != 0)
        *info = 0;
    अन्यथा
        *info = 1;

    वापस CV_HAL_ERROR_OK;
}

ढाँचा <typename fptype> अटल अंतरभूत पूर्णांक
lapack_gemm(स्थिर fptype *src1, माप_प्रकार src1_step, स्थिर fptype *src2, माप_प्रकार src2_step, fptype alpha,
            स्थिर fptype *src3, माप_प्रकार src3_step, fptype beta, fptype *dst, माप_प्रकार dst_step, पूर्णांक a_m, पूर्णांक a_n, पूर्णांक d_n, पूर्णांक flags)
{
    पूर्णांक ldsrc1 = (पूर्णांक)(src1_step / माप(fptype));
    पूर्णांक ldsrc2 = (पूर्णांक)(src2_step / माप(fptype));
    पूर्णांक ldsrc3 = (पूर्णांक)(src3_step / माप(fptype));
    पूर्णांक lddst = (पूर्णांक)(dst_step / माप(fptype));
    पूर्णांक c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    अगर(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        अगर(flags & CV_HAL_GEMM_1_T )
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
        अगर(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अन्यथा
        {
            d_m = a_m;
        }
    }

    अगर(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    अन्यथा
    {
        c_m = d_m;
        c_n = d_n;
    }

    अगर(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    अन्यथा
    {
        transA = CblasNoTrans;
    }

    अगर(src3 != dst && beta != 0.0 && src3_step != 0) {
        अगर(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        अन्यथा
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    अन्यथा अगर (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    अन्यथा अगर(src3_step == 0 && beta != 0.0)
        set_मूल्य_2(dst, lddst, (fptype)0.0, d_m, d_n);

    अगर(typeid(fptype) == typeid(भग्न))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (भग्न)alpha, (भग्न*)src1, ldsrc1, (भग्न*)src2, ldsrc2, (भग्न)beta, (भग्न*)dst, lddst);
    अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (द्विगुन)alpha, (द्विगुन*)src1, ldsrc1, (द्विगुन*)src2, ldsrc2, (द्विगुन)beta, (द्विगुन*)dst, lddst);

    वापस CV_HAL_ERROR_OK;
}

ढाँचा <typename fptype> अटल अंतरभूत पूर्णांक
lapack_gemm_c(स्थिर fptype *src1, माप_प्रकार src1_step, स्थिर fptype *src2, माप_प्रकार src2_step, fptype alpha,
            स्थिर fptype *src3, माप_प्रकार src3_step, fptype beta, fptype *dst, माप_प्रकार dst_step, पूर्णांक a_m, पूर्णांक a_n, पूर्णांक d_n, पूर्णांक flags)
{
    पूर्णांक ldsrc1 = (पूर्णांक)(src1_step / माप(std::complex<fptype>));
    पूर्णांक ldsrc2 = (पूर्णांक)(src2_step / माप(std::complex<fptype>));
    पूर्णांक ldsrc3 = (पूर्णांक)(src3_step / माप(std::complex<fptype>));
    पूर्णांक lddst = (पूर्णांक)(dst_step / माप(std::complex<fptype>));
    पूर्णांक c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    अगर(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        अगर(flags & CV_HAL_GEMM_1_T )
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
        अगर(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अन्यथा
        {
            d_m = a_m;
        }
    }

    अगर(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    अन्यथा
    {
        c_m = d_m;
        c_n = d_n;
    }

    अगर(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    अन्यथा
    {
        transA = CblasNoTrans;
    }

    अगर(src3 != dst && beta != 0.0 && src3_step != 0) {
        अगर(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        अन्यथा
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    अन्यथा अगर (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    अन्यथा अगर(src3_step == 0 && beta != 0.0)
        set_मूल्य_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#अगर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    पूर्णांक M = a_m, N = d_n, K = a_n;
    अगर(typeid(fptype) == typeid(भग्न)) {
        स्वतः src1_cast = (std::complex<भग्न>*)(src1);
        स्वतः src2_cast = (std::complex<भग्न>*)(src2);
        स्वतः dst_cast = (std::complex<भग्न>*)(dst);
        दीर्घ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<भग्न>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<भग्न>*)&cBeta, dst_cast, ldc);
    }
    अन्यथा अगर(typeid(fptype) == typeid(द्विगुन)) {
        स्वतः src1_cast = (std::complex<द्विगुन>*)(src1);
        स्वतः src2_cast = (std::complex<द्विगुन>*)(src2);
        स्वतः dst_cast = (std::complex<द्विगुन>*)(dst);
        दीर्घ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<द्विगुन>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<द्विगुन>*)&cBeta, dst_cast, ldc);
    }
#अन्यथा
    अगर(typeid(fptype) == typeid(भग्न))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (भग्न*)reinterpret_cast<fptype(&)[2]>(cAlpha), (भग्न*)src1, ldsrc1, (भग्न*)src2, ldsrc2, (भग्न*)reinterpret_cast<fptype(&)[2]>(cBeta), (भग्न*)dst, lddst);
    अन्यथा अगर(typeid(fptype) == typeid(द्विगुन))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (द्विगुन*)reinterpret_cast<fptype(&)[2]>(cAlpha), (द्विगुन*)src1, ldsrc1, (द्विगुन*)src2, ldsrc2, (द्विगुन*)reinterpret_cast<fptype(&)[2]>(cBeta), (द्विगुन*)dst, lddst);
#पूर्ण_अगर

    वापस CV_HAL_ERROR_OK;
}
पूर्णांक lapack_LU32f(भग्न* a, माप_प्रकार a_step, पूर्णांक m, भग्न* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info)
{
    अगर(m < HAL_LU_SMALL_MATRIX_THRESH)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_LU(a, a_step, m, b, b_step, n, info);
}

पूर्णांक lapack_LU64f(द्विगुन* a, माप_प्रकार a_step, पूर्णांक m, द्विगुन* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info)
{
    अगर(m < HAL_LU_SMALL_MATRIX_THRESH)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_LU(a, a_step, m, b, b_step, n, info);
}

पूर्णांक lapack_Cholesky32f(भग्न* a, माप_प्रकार a_step, पूर्णांक m, भग्न* b, माप_प्रकार b_step, पूर्णांक n, bool *info)
{
    अगर(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

पूर्णांक lapack_Cholesky64f(द्विगुन* a, माप_प्रकार a_step, पूर्णांक m, द्विगुन* b, माप_प्रकार b_step, पूर्णांक n, bool *info)
{
    अगर(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

पूर्णांक lapack_SVD32f(भग्न* a, माप_प्रकार a_step, भग्न *w, भग्न* u, माप_प्रकार u_step, भग्न* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags)
{
    अगर(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    पूर्णांक info = 0;
    वापस lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

पूर्णांक lapack_SVD64f(द्विगुन* a, माप_प्रकार a_step, द्विगुन *w, द्विगुन* u, माप_प्रकार u_step, द्विगुन* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags)
{
    अगर(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    पूर्णांक info = 0;
    वापस lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

पूर्णांक lapack_QR32f(भग्न* src1, माप_प्रकार src1_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, भग्न* src2, माप_प्रकार src2_step, भग्न* dst, पूर्णांक* info)
{
    अगर (m < HAL_QR_SMALL_MATRIX_THRESH)
      वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

पूर्णांक lapack_QR64f(द्विगुन* src1, माप_प्रकार src1_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, द्विगुन* src2, माप_प्रकार src2_step, द्विगुन* dst, पूर्णांक* info)
{
    अगर (m < HAL_QR_SMALL_MATRIX_THRESH)
      वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

पूर्णांक lapack_gemm32f(स्थिर भग्न *src1, माप_प्रकार src1_step, स्थिर भग्न *src2, माप_प्रकार src2_step, भग्न alpha,
                   स्थिर भग्न *src3, माप_प्रकार src3_step, भग्न beta, भग्न *dst, माप_प्रकार dst_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags)
{
    अगर(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

पूर्णांक lapack_gemm64f(स्थिर द्विगुन *src1, माप_प्रकार src1_step, स्थिर द्विगुन *src2, माप_प्रकार src2_step, द्विगुन alpha,
                   स्थिर द्विगुन *src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन *dst, माप_प्रकार dst_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags)
{
    अगर(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

पूर्णांक lapack_gemm32fc(स्थिर भग्न *src1, माप_प्रकार src1_step, स्थिर भग्न *src2, माप_प्रकार src2_step, भग्न alpha,
                   स्थिर भग्न *src3, माप_प्रकार src3_step, भग्न beta, भग्न *dst, माप_प्रकार dst_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags)
{
    अगर(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
पूर्णांक lapack_gemm64fc(स्थिर द्विगुन *src1, माप_प्रकार src1_step, स्थिर द्विगुन *src2, माप_प्रकार src2_step, द्विगुन alpha,
                   स्थिर द्विगुन *src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन *dst, माप_प्रकार dst_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags)
{
    अगर(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        वापस CV_HAL_ERROR_NOT_IMPLEMENTED;
    वापस lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#अगर defined(__APPLE__) && defined(__clang__)
#आशय clang diagnostic pop
#पूर्ण_अगर

#पूर्ण_अगर //HAVE_LAPACK
