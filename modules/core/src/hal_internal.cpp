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

#जर defined(__clang__) && defined(__has_feature)
#जर __has_feature(memory_sanitizer)
#अन्तर्भाव <sanitizer/msan_interface.h>
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) \
__msan_unpoison(address, size)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY __attribute__((no_sanitize("memory")))
#endif
#endif
#ifndef CV_ANNOTATE_MEMORY_IS_INITIALIZED
#define CV_ANNOTATE_MEMORY_IS_INITIALIZED(address, size) करा { } पर्यंत(0)
#define CV_ANNOTATE_NO_SANITIZE_MEMORY
#endif

#जर defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

//lapack stores matrices in column-major order so transposing is needed everywhere
ढाँचा <typename fptype> स्थिर ओळीत शून्य
transpose_square_inplace(fptype *src, माप_प्रकार src_ld, माप_प्रकार m)
{
    साठी(माप_प्रकार i = 0; i < m - 1; i++)
        साठी(माप_प्रकार j = i + 1; j < m; j++)
            std::swap(src[j*src_ld + i], src[i*src_ld + j]);
}

ढाँचा <typename fptype> स्थिर ओळीत शून्य
transpose(स्थिरांक fptype *src, माप_प्रकार src_ld, fptype* dst, माप_प्रकार dst_ld, माप_प्रकार m, माप_प्रकार n)
{
    साठी(माप_प्रकार i = 0; i < m; i++)
        साठी(माप_प्रकार j = 0; j < n; j++)
            dst[j*dst_ld + i] = src[i*src_ld + j];
}

ढाँचा <typename fptype> स्थिर ओळीत शून्य
copy_matrix(स्थिरांक fptype *src, माप_प्रकार src_ld, fptype* dst, माप_प्रकार dst_ld, माप_प्रकार m, माप_प्रकार n)
{
    साठी(माप_प्रकार i = 0; i < m; i++)
        साठी(माप_प्रकार j = 0; j < n; j++)
            dst[i*dst_ld + j] = src[i*src_ld + j];
}

ढाँचा <typename fptype> स्थिर ओळीत शून्य
set_मूल्य_2(fptype *dst, माप_प्रकार dst_ld, fptype value, माप_प्रकार m, माप_प्रकार n)
{
    साठी(माप_प्रकार i = 0; i < m; i++)
        साठी(माप_प्रकार j = 0; j < n; j++)
            dst[i*dst_ld + j] = value;
}

// MSAN can't see that the fortran LAPACK functions initialize `info`
ढाँचा <typename fptype> स्थिर ओळीत पूर्णांक
CV_ANNOTATE_NO_SANITIZE_MEMORY lapack_LU(fptype* a, माप_प्रकार a_step, पूर्णांक m, fptype* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info)
{
#जर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    cv::AutoBuffer<दीर्घ> piv_buff(m);
    दीर्घ lda = (दीर्घ)(a_step / आकार(fptype));
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ _info[1];
#अन्यथा
    cv::AutoBuffer<पूर्णांक> piv_buff(m);
    पूर्णांक lda = (पूर्णांक)(a_step / आकार(fptype));
    पूर्णांक _m = m, _n = n;
    पूर्णांक* _info = info;
#endif
    स्वतः piv = piv_buff.data();

    transpose_square_inplace(a, lda, m);

    जर(b)
    {
        जर(n == 1 && b_step == आकार(fptype))
        {
            जर(typeid(fptype) == typeid(दशांश))
                sgesv_(&_m, &_n, (दशांश*)a, &lda, piv, (दशांश*)b, &_m, _info);
            अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
                dgesv_(&_m, &_n, (द्विगुण*)a, &lda, piv, (द्विगुण*)b, &_m, _info);
        }
        अन्यथा
        {
            पूर्णांक ldb = (पूर्णांक)(b_step / आकार(fptype));
            std::vector<fptype> tmpB(m*n+1);

            transpose(b, ldb, &tmpB[0], m, m, n);

            जर(typeid(fptype) == typeid(दशांश))
                sgesv_(&_m, &_n, (दशांश*)a, &lda, piv, (दशांश*)&tmpB[0], &_m, _info);
            अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
                dgesv_(&_m, &_n, (द्विगुण*)a, &lda, piv, (द्विगुण*)&tmpB[0], &_m, _info);

            transpose(&tmpB[0], m, b, ldb, n, m);
        }
    }
    अन्यथा
    {
        जर(typeid(fptype) == typeid(दशांश))
            sgetrf_(&_m, &_m, (दशांश*)a, &lda, piv, _info);
        अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
            dgetrf_(&_m, &_m, (द्विगुण*)a, &lda, piv, _info);
    }

#जर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णांक>(_info[0]);
#endif
    पूर्णांक retcode = *info >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;

    पूर्णांक sign = 0;
    जर(*info == 0)
    {
        साठी(पूर्णांक i = 0; i < m; i++)
            sign ^= piv[i] != i + 1;
        *info = sign ? -1 : 1;
    }
    अन्यथा
        *info = 0; //in opencv LU function zero means error

    परत retcode;
}

ढाँचा <typename fptype> स्थिर ओळीत पूर्णांक
lapack_Cholesky(fptype* a, माप_प्रकार a_step, पूर्णांक m, fptype* b, माप_प्रकार b_step, पूर्णांक n, तार्किक* info)
{
#जर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ lapackStatus = 0;
    दीर्घ lda = (दीर्घ)(a_step / आकार(fptype));
#अन्यथा
    पूर्णांक _m = m, _n = n;
    पूर्णांक lapackStatus = 0;
    पूर्णांक lda = (पूर्णांक)(a_step / आकार(fptype));
#endif
    अक्षर L[] = {'L', '\0'};

    जर(b)
    {
        जर(n == 1 && b_step == आकार(fptype))
        {
            जर(typeid(fptype) == typeid(दशांश))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (दशांश*)a, &lda, (दशांश*)b, &_m, &lapackStatus);
            अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)b, &_m, &lapackStatus);
        }
        अन्यथा
        {
            पूर्णांक ldb = (पूर्णांक)(b_step / आकार(fptype));
            fptype* tmpB = नवीन fptype[m*n];
            transpose(b, ldb, tmpB, m, m, n);

            जर(typeid(fptype) == typeid(दशांश))
                OCV_LAPACK_FUNC(sposv)(L, &_m, &_n, (दशांश*)a, &lda, (दशांश*)tmpB, &_m, &lapackStatus);
            अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dposv)(L, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)tmpB, &_m, &lapackStatus);

            transpose(tmpB, m, b, ldb, n, m);
            delete[] tmpB;
        }
    }
    अन्यथा
    {
        जर(typeid(fptype) == typeid(दशांश))
            OCV_LAPACK_FUNC(spotrf)(L, &_m, (दशांश*)a, &lda, &lapackStatus);
        अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
            OCV_LAPACK_FUNC(dpotrf)(L, &_m, (द्विगुण*)a, &lda, &lapackStatus);
    }

    जर(lapackStatus == 0) *info = सत्य;
    अन्यथा *info = असत्य; //in opencv Cholesky function false means error

    परत lapackStatus >= 0 ? CV_HAL_ERROR_OK : CV_HAL_ERROR_NOT_IMPLEMENTED;
}

ढाँचा <typename fptype> स्थिर ओळीत पूर्णांक
lapack_SVD(fptype* a, माप_प्रकार a_step, fptype *w, fptype* u, माप_प्रकार u_step, fptype* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags, पूर्णांक* info)
{
#जर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n);
    दीर्घ _info[1];
    दीर्घ lda = (दीर्घ)(a_step / आकार(fptype));
    दीर्घ ldv = (दीर्घ)(v_step / आकार(fptype));
    दीर्घ ldu = (दीर्घ)(u_step / आकार(fptype));
    दीर्घ lwork = -1;
    cv::AutoBuffer<दीर्घ> iworkBuf_(8 * std::min(m, n));
#अन्यथा
    पूर्णांक _m = m, _n = n;
    पूर्णांक* _info = info;
    पूर्णांक lda = (पूर्णांक)(a_step / आकार(fptype));
    पूर्णांक ldv = (पूर्णांक)(v_step / आकार(fptype));
    पूर्णांक ldu = (पूर्णांक)(u_step / आकार(fptype));
    पूर्णांक lwork = -1;
    cv::AutoBuffer<पूर्णांक> iworkBuf_(8 * std::min(m, n));
#endif
    स्वतः iworkBuf = iworkBuf_.data();
    std::vector<fptype> ubuf;
    fptype work1 = 0;

    //A already transposed and m>=n
    अक्षर mode[] = { ' ', '\0'};
    जर(flags & CV_HAL_SVD_NO_UV)
    {
        ldv = 1;
        mode[0] = 'N';
    }
    अन्यथा जर((flags & CV_HAL_SVD_SHORT_UV) && (flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in a
        mode[0] = 'O';
    अन्यथा जर((flags & CV_HAL_SVD_SHORT_UV) && !(flags & CV_HAL_SVD_MODIFY_A)) //short SVD, U stored in u if m>=n
        mode[0] = 'S';
    अन्यथा जर(flags & CV_HAL_SVD_FULL_UV) //full SVD, U stored in u or in a
        mode[0] = 'A';

    जर((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV)) //U stored in a
    {
        ubuf.resize(m*m);
        u = &ubuf[0];
        ldu = m;
    }

    जर(typeid(fptype) == typeid(दशांश))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (दशांश*)a, &lda, (दशांश*)w, (दशांश*)u, &ldu, (दशांश*)vt, &ldv, (दशांश*)&work1, &lwork, iworkBuf, _info);
    अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)w, (द्विगुण*)u, &ldu, (द्विगुण*)vt, &ldv, (द्विगुण*)&work1, &lwork, iworkBuf, _info);

    जर(*info < 0)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;

    lwork = (पूर्णांक)round(work1); //optimal buffer size
    std::vector<fptype> buffer(lwork + 1);

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language is called.
    // Note: we do this here because if dgesdd is C++, MSAN errors can be reported within it.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(buffer.data(), आकार(fptype) * (lwork + 1));

    जर(typeid(fptype) == typeid(दशांश))
        OCV_LAPACK_FUNC(sgesdd)(mode, &_m, &_n, (दशांश*)a, &lda, (दशांश*)w, (दशांश*)u, &ldu, (दशांश*)vt, &ldv, (दशांश*)&buffer[0], &lwork, iworkBuf, _info);
    अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
        OCV_LAPACK_FUNC(dgesdd)(mode, &_m, &_n, (द्विगुण*)a, &lda, (द्विगुण*)w, (द्विगुण*)u, &ldu, (द्विगुण*)vt, &ldv, (द्विगुण*)&buffer[0], &lwork, iworkBuf, _info);

#जर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णांक>(_info[0]);
#endif

    // Make sure MSAN sees the memory as having been written.
    // MSAN does not think it has been written because a different language was called.
    CV_ANNOTATE_MEMORY_IS_INITIALIZED(a, a_step * n);
    जर (u)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(u, u_step * m);
    जर (vt)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(vt, v_step * n);
    जर (w)
      CV_ANNOTATE_MEMORY_IS_INITIALIZED(w, आकार(fptype) * std::min(m, n));

    जर(!(flags & CV_HAL_SVD_NO_UV))
        transpose_square_inplace(vt, ldv, n);

    जर((flags & CV_HAL_SVD_MODIFY_A) && (flags & CV_HAL_SVD_FULL_UV))
    {
        साठी(पूर्णांक i = 0; i < m; i++)
            साठी(पूर्णांक j = 0; j < m; j++)
                a[i*lda + j] = u[i*m + j];
    }

    जर(*info < 0)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत CV_HAL_ERROR_OK;
}

ढाँचा <typename fptype> स्थिर ओळीत पूर्णांक
lapack_QR(fptype* a, माप_प्रकार a_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, fptype* b, माप_प्रकार b_step, fptype* dst, पूर्णांक* info)
{
#जर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    दीर्घ _m = static_cast<दीर्घ>(m), _n = static_cast<दीर्घ>(n), _k = static_cast<दीर्घ>(k);
    दीर्घ _info[1];
    दीर्घ lda = (दीर्घ)(a_step / आकार(fptype));
    दीर्घ lwork = -1;
    दीर्घ ldtmpA;
#अन्यथा
    पूर्णांक _m = m, _n = n, _k = k;
    पूर्णांक* _info = info;
    पूर्णांक lda = (पूर्णांक)(a_step / आकार(fptype));
    पूर्णांक lwork = -1;
    पूर्णांक ldtmpA;
#endif

    अक्षर mode[] = { 'N', '\0' };
    जर(m < n)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;

    std::vector<fptype> tmpAMemHolder;
    fptype* tmpA;

    जर (m == n)
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

    जर (b)
    {
        जर (k == 1 && b_step == आकार(fptype))
        {
            जर (typeid(fptype) == typeid(दशांश))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (दशांश*)tmpA, &ldtmpA, (दशांश*)b, &_m, (दशांश*)&work1, &lwork, _info);
            अन्यथा जर (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)b, &_m, (द्विगुण*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, आकार(पूर्णांक));
            जर (*info < 0)
                परत CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            जर (typeid(fptype) == typeid(दशांश))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (दशांश*)tmpA, &ldtmpA, (दशांश*)b, &_m, (दशांश*)buffer, &lwork, _info);
            अन्यथा जर (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)b, &_m, (द्विगुण*)buffer, &lwork, _info);
        }
        अन्यथा
        {
            std::vector<fptype> tmpBMemHolder(m*k);
            fptype* tmpB = &tmpBMemHolder.front();
            पूर्णांक ldb = (पूर्णांक)(b_step / आकार(fptype));
            transpose(b, ldb, tmpB, m, m, k);

            जर (typeid(fptype) == typeid(दशांश))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (दशांश*)tmpA, &ldtmpA, (दशांश*)tmpB, &_m, (दशांश*)&work1, &lwork, _info);
            अन्यथा जर (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)tmpB, &_m, (द्विगुण*)&work1, &lwork, _info);

            CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, आकार(पूर्णांक));
            जर (*info < 0)
                परत CV_HAL_ERROR_NOT_IMPLEMENTED;

            lwork = cvRound(work1); //optimal buffer size
            std::vector<fptype> workBufMemHolder(lwork + 1);
            fptype* buffer = &workBufMemHolder.front();

            जर (typeid(fptype) == typeid(दशांश))
                OCV_LAPACK_FUNC(sgels)(mode, &_m, &_n, &_k, (दशांश*)tmpA, &ldtmpA, (दशांश*)tmpB, &_m, (दशांश*)buffer, &lwork, _info);
            अन्यथा जर (typeid(fptype) == typeid(द्विगुण))
                OCV_LAPACK_FUNC(dgels)(mode, &_m, &_n, &_k, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)tmpB, &_m, (द्विगुण*)buffer, &lwork, _info);

            transpose(tmpB, m, b, ldb, k, m);
        }
    }
    अन्यथा
    {
        जर (typeid(fptype) == typeid(दशांश))
            sgeqrf_(&_m, &_n, (दशांश*)tmpA, &ldtmpA, (दशांश*)dst, (दशांश*)&work1, &lwork, _info);
        अन्यथा जर (typeid(fptype) == typeid(द्विगुण))
            dgeqrf_(&_m, &_n, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)dst, (द्विगुण*)&work1, &lwork, _info);

        CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, आकार(पूर्णांक));
        जर (*info < 0)
            परत CV_HAL_ERROR_NOT_IMPLEMENTED;

        lwork = cvRound(work1); //optimal buffer size
        std::vector<fptype> workBufMemHolder(lwork + 1);
        fptype* buffer = &workBufMemHolder.front();

        जर (typeid(fptype) == typeid(दशांश))
            sgeqrf_(&_m, &_n, (दशांश*)tmpA, &ldtmpA, (दशांश*)dst, (दशांश*)buffer, &lwork, _info);
        अन्यथा जर (typeid(fptype) == typeid(द्विगुण))
            dgeqrf_(&_m, &_n, (द्विगुण*)tmpA, &ldtmpA, (द्विगुण*)dst, (द्विगुण*)buffer, &lwork, _info);
    }

    CV_ANNOTATE_MEMORY_IS_INITIALIZED(info, आकार(पूर्णांक));
    जर (m == n)
        transpose_square_inplace(a, lda, m);
    अन्यथा
        transpose(tmpA, m, a, lda, n, m);

#जर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    *info = static_cast<पूर्णांक>(_info[0]);
#endif

    जर (*info != 0)
        *info = 0;
    अन्यथा
        *info = 1;

    परत CV_HAL_ERROR_OK;
}

ढाँचा <typename fptype> स्थिर ओळीत पूर्णांक
lapack_gemm(स्थिरांक fptype *src1, माप_प्रकार src1_step, स्थिरांक fptype *src2, माप_प्रकार src2_step, fptype alpha,
            स्थिरांक fptype *src3, माप_प्रकार src3_step, fptype beta, fptype *dst, माप_प्रकार dst_step, पूर्णांक a_m, पूर्णांक a_n, पूर्णांक d_n, पूर्णांक flags)
{
    पूर्णांक ldsrc1 = (पूर्णांक)(src1_step / आकार(fptype));
    पूर्णांक ldsrc2 = (पूर्णांक)(src2_step / आकार(fptype));
    पूर्णांक ldsrc3 = (पूर्णांक)(src3_step / आकार(fptype));
    पूर्णांक lddst = (पूर्णांक)(dst_step / आकार(fptype));
    पूर्णांक c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;

    जर(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        जर(flags & CV_HAL_GEMM_1_T )
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
        जर(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अन्यथा
        {
            d_m = a_m;
        }
    }

    जर(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    अन्यथा
    {
        c_m = d_m;
        c_n = d_n;
    }

    जर(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    अन्यथा
    {
        transA = CblasNoTrans;
    }

    जर(src3 != dst && beta != 0.0 && src3_step != 0) {
        जर(flags & CV_HAL_GEMM_3_T)
            transpose(src3, ldsrc3, dst, lddst, c_m, c_n);
        अन्यथा
            copy_matrix(src3, ldsrc3, dst, lddst, c_m, c_n);
    }
    अन्यथा जर (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    अन्यथा जर(src3_step == 0 && beta != 0.0)
        set_मूल्य_2(dst, lddst, (fptype)0.0, d_m, d_n);

    जर(typeid(fptype) == typeid(दशांश))
        cblas_sgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (दशांश)alpha, (दशांश*)src1, ldsrc1, (दशांश*)src2, ldsrc2, (दशांश)beta, (दशांश*)dst, lddst);
    अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
        cblas_dgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (द्विगुण)alpha, (द्विगुण*)src1, ldsrc1, (द्विगुण*)src2, ldsrc2, (द्विगुण)beta, (द्विगुण*)dst, lddst);

    परत CV_HAL_ERROR_OK;
}

ढाँचा <typename fptype> स्थिर ओळीत पूर्णांक
lapack_gemm_c(स्थिरांक fptype *src1, माप_प्रकार src1_step, स्थिरांक fptype *src2, माप_प्रकार src2_step, fptype alpha,
            स्थिरांक fptype *src3, माप_प्रकार src3_step, fptype beta, fptype *dst, माप_प्रकार dst_step, पूर्णांक a_m, पूर्णांक a_n, पूर्णांक d_n, पूर्णांक flags)
{
    पूर्णांक ldsrc1 = (पूर्णांक)(src1_step / आकार(std::complex<fptype>));
    पूर्णांक ldsrc2 = (पूर्णांक)(src2_step / आकार(std::complex<fptype>));
    पूर्णांक ldsrc3 = (पूर्णांक)(src3_step / आकार(std::complex<fptype>));
    पूर्णांक lddst = (पूर्णांक)(dst_step / आकार(std::complex<fptype>));
    पूर्णांक c_m, c_n, d_m;
    CBLAS_TRANSPOSE transA, transB;
    std::complex<fptype> cAlpha(alpha, 0.0);
    std::complex<fptype> cBeta(beta, 0.0);

    जर(flags & CV_HAL_GEMM_2_T)
    {
        transB = CblasTrans;
        जर(flags & CV_HAL_GEMM_1_T )
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
        जर(flags & CV_HAL_GEMM_1_T )
        {
            d_m = a_n;
        }
        अन्यथा
        {
            d_m = a_m;
        }
    }

    जर(flags & CV_HAL_GEMM_3_T)
    {
        c_m = d_n;
        c_n = d_m;
    }
    अन्यथा
    {
        c_m = d_m;
        c_n = d_n;
    }

    जर(flags & CV_HAL_GEMM_1_T )
    {
        transA = CblasTrans;
        std::swap(a_n, a_m);
    }
    अन्यथा
    {
        transA = CblasNoTrans;
    }

    जर(src3 != dst && beta != 0.0 && src3_step != 0) {
        जर(flags & CV_HAL_GEMM_3_T)
            transpose((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
        अन्यथा
            copy_matrix((std::complex<fptype>*)src3, ldsrc3, (std::complex<fptype>*)dst, lddst, c_m, c_n);
    }
    अन्यथा जर (src3 == dst && (flags & CV_HAL_GEMM_3_T)) //actually transposing C in this case done by openCV
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    अन्यथा जर(src3_step == 0 && beta != 0.0)
        set_मूल्य_2((std::complex<fptype>*)dst, lddst, std::complex<fptype>(0.0, 0.0), d_m, d_n);

    // FIXME: this is a workaround. Support ILP64 in HAL API.
#जर defined (ACCELERATE_NEW_LAPACK) && defined (ACCELERATE_LAPACK_ILP64)
    पूर्णांक M = a_m, N = d_n, K = a_n;
    जर(typeid(fptype) == typeid(दशांश)) {
        स्वतः src1_cast = (std::complex<दशांश>*)(src1);
        स्वतः src2_cast = (std::complex<दशांश>*)(src2);
        स्वतः dst_cast = (std::complex<दशांश>*)(dst);
        दीर्घ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_cgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<दशांश>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<दशांश>*)&cBeta, dst_cast, ldc);
    }
    अन्यथा जर(typeid(fptype) == typeid(द्विगुण)) {
        स्वतः src1_cast = (std::complex<द्विगुण>*)(src1);
        स्वतः src2_cast = (std::complex<द्विगुण>*)(src2);
        स्वतः dst_cast = (std::complex<द्विगुण>*)(dst);
        दीर्घ lda = ldsrc1, ldb = ldsrc2, ldc = lddst;
        cblas_zgemm(CblasRowMajor, transA, transB, M, N, K, (std::complex<द्विगुण>*)&cAlpha, src1_cast, lda, src2_cast, ldb, (std::complex<द्विगुण>*)&cBeta, dst_cast, ldc);
    }
#अन्यथा
    जर(typeid(fptype) == typeid(दशांश))
        cblas_cgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (दशांश*)reinterpret_cast<fptype(&)[2]>(cAlpha), (दशांश*)src1, ldsrc1, (दशांश*)src2, ldsrc2, (दशांश*)reinterpret_cast<fptype(&)[2]>(cBeta), (दशांश*)dst, lddst);
    अन्यथा जर(typeid(fptype) == typeid(द्विगुण))
        cblas_zgemm(CblasRowMajor, transA, transB, a_m, d_n, a_n, (द्विगुण*)reinterpret_cast<fptype(&)[2]>(cAlpha), (द्विगुण*)src1, ldsrc1, (द्विगुण*)src2, ldsrc2, (द्विगुण*)reinterpret_cast<fptype(&)[2]>(cBeta), (द्विगुण*)dst, lddst);
#endif

    परत CV_HAL_ERROR_OK;
}
पूर्णांक lapack_LU32f(दशांश* a, माप_प्रकार a_step, पूर्णांक m, दशांश* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info)
{
    जर(m < HAL_LU_SMALL_MATRIX_THRESH)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_LU(a, a_step, m, b, b_step, n, info);
}

पूर्णांक lapack_LU64f(द्विगुण* a, माप_प्रकार a_step, पूर्णांक m, द्विगुण* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info)
{
    जर(m < HAL_LU_SMALL_MATRIX_THRESH)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_LU(a, a_step, m, b, b_step, n, info);
}

पूर्णांक lapack_Cholesky32f(दशांश* a, माप_प्रकार a_step, पूर्णांक m, दशांश* b, माप_प्रकार b_step, पूर्णांक n, तार्किक *info)
{
    जर(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

पूर्णांक lapack_Cholesky64f(द्विगुण* a, माप_प्रकार a_step, पूर्णांक m, द्विगुण* b, माप_प्रकार b_step, पूर्णांक n, तार्किक *info)
{
    जर(m < HAL_CHOLESKY_SMALL_MATRIX_THRESH)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_Cholesky(a, a_step, m, b, b_step, n, info);
}

पूर्णांक lapack_SVD32f(दशांश* a, माप_प्रकार a_step, दशांश *w, दशांश* u, माप_प्रकार u_step, दशांश* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags)
{
    जर(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    पूर्णांक info = 0;
    परत lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

पूर्णांक lapack_SVD64f(द्विगुण* a, माप_प्रकार a_step, द्विगुण *w, द्विगुण* u, माप_प्रकार u_step, द्विगुण* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags)
{
    जर(m < HAL_SVD_SMALL_MATRIX_THRESH || n <= 0)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    पूर्णांक info = 0;
    परत lapack_SVD(a, a_step, w, u, u_step, vt, v_step, m, n, flags, &info);
}

पूर्णांक lapack_QR32f(दशांश* src1, माप_प्रकार src1_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, दशांश* src2, माप_प्रकार src2_step, दशांश* dst, पूर्णांक* info)
{
    जर (m < HAL_QR_SMALL_MATRIX_THRESH)
      परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

पूर्णांक lapack_QR64f(द्विगुण* src1, माप_प्रकार src1_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, द्विगुण* src2, माप_प्रकार src2_step, द्विगुण* dst, पूर्णांक* info)
{
    जर (m < HAL_QR_SMALL_MATRIX_THRESH)
      परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_QR(src1, src1_step, m, n, k, src2, src2_step, dst, info);
}

पूर्णांक lapack_gemm32f(स्थिरांक दशांश *src1, माप_प्रकार src1_step, स्थिरांक दशांश *src2, माप_प्रकार src2_step, दशांश alpha,
                   स्थिरांक दशांश *src3, माप_प्रकार src3_step, दशांश beta, दशांश *dst, माप_प्रकार dst_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags)
{
    जर(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

पूर्णांक lapack_gemm64f(स्थिरांक द्विगुण *src1, माप_प्रकार src1_step, स्थिरांक द्विगुण *src2, माप_प्रकार src2_step, द्विगुण alpha,
                   स्थिरांक द्विगुण *src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण *dst, माप_प्रकार dst_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags)
{
    जर(m < HAL_GEMM_SMALL_MATRIX_THRESH)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_gemm(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

पूर्णांक lapack_gemm32fc(स्थिरांक दशांश *src1, माप_प्रकार src1_step, स्थिरांक दशांश *src2, माप_प्रकार src2_step, दशांश alpha,
                   स्थिरांक दशांश *src3, माप_प्रकार src3_step, दशांश beta, दशांश *dst, माप_प्रकार dst_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags)
{
    जर(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}
पूर्णांक lapack_gemm64fc(स्थिरांक द्विगुण *src1, माप_प्रकार src1_step, स्थिरांक द्विगुण *src2, माप_प्रकार src2_step, द्विगुण alpha,
                   स्थिरांक द्विगुण *src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण *dst, माप_प्रकार dst_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags)
{
    जर(m < HAL_GEMM_SMALL_COMPLEX_MATRIX_THRESH)
        परत CV_HAL_ERROR_NOT_IMPLEMENTED;
    परत lapack_gemm_c(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m, n, k, flags);
}

#जर defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif //HAVE_LAPACK
