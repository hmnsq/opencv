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

#جے_بنا_تعریف OPENCV_CORE_HAL_اندرونی_HPP
#تعریف_کرو OPENCV_CORE_HAL_اندرونی_HPP

#جے_تعریف_شدہ HAVE_LAPACK

صحيح lapack_LU32f(عائم* a, size_t a_قدم, صحيح m, عائم* b, size_t b_قدم, صحيح n, صحيح* info);
صحيح lapack_LU64f(مزدوج* a, size_t a_قدم, صحيح m, مزدوج* b, size_t b_قدم, صحيح n, صحيح* info);
صحيح lapack_Cholesky32f(عائم* a, size_t a_قدم, صحيح m, عائم* b, size_t b_قدم, صحيح n, منطقي* info);
صحيح lapack_Cholesky64f(مزدوج* a, size_t a_قدم, صحيح m, مزدوج* b, size_t b_قدم, صحيح n, منطقي* info);
صحيح lapack_SVD32f(عائم* a, size_t a_قدم, عائم* w, عائم* u, size_t u_قدم, عائم* vt, size_t v_قدم, صحيح m, صحيح n, صحيح flags);
صحيح lapack_SVD64f(مزدوج* a, size_t a_قدم, مزدوج* w, مزدوج* u, size_t u_قدم, مزدوج* vt, size_t v_قدم, صحيح m, صحيح n, صحيح flags);
صحيح lapack_QR32f(عائم* src1, size_t src1_قدم, صحيح m, صحيح n, صحيح k, عائم* src2, size_t src2_قدم, عائم* dst, صحيح* info);
صحيح lapack_QR64f(مزدوج* src1, size_t src1_قدم, صحيح m, صحيح n, صحيح k, مزدوج* src2, size_t src2_قدم, مزدوج* dst, صحيح* info);
صحيح lapack_gemm32f(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
                   عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
                   صحيح m, صحيح n, صحيح k, صحيح flags);
صحيح lapack_gemm64f(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
                   مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
                   صحيح m, صحيح n, صحيح k, صحيح flags);
صحيح lapack_gemm32fc(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
                   عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
                   صحيح m, صحيح n, صحيح k, صحيح flags);
صحيح lapack_gemm64fc(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
                   مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
                   صحيح m, صحيح n, صحيح k, صحيح flags);

#تعریف_رد cv_hal_LU32f
#تعریف_کرو cv_hal_LU32f lapack_LU32f
#تعریف_رد cv_hal_LU64f
#تعریف_کرو cv_hal_LU64f lapack_LU64f

#تعریف_رد cv_hal_Cholesky32f
#تعریف_کرو cv_hal_Cholesky32f lapack_Cholesky32f
#تعریف_رد cv_hal_Cholesky64f
#تعریف_کرو cv_hal_Cholesky64f lapack_Cholesky64f

#تعریف_رد cv_hal_SVD32f
#تعریف_کرو cv_hal_SVD32f lapack_SVD32f
#تعریف_رد cv_hal_SVD64f
#تعریف_کرو cv_hal_SVD64f lapack_SVD64f

#تعریف_رد cv_hal_QR32f
#تعریف_کرو cv_hal_QR32f lapack_QR32f
#تعریف_رد cv_hal_QR64f
#تعریف_کرو cv_hal_QR64f lapack_QR64f

#تعریف_رد cv_hal_gemm32f
#تعریف_کرو cv_hal_gemm32f lapack_gemm32f
#تعریف_رد cv_hal_gemm64f
#تعریف_کرو cv_hal_gemm64f lapack_gemm64f
#تعریف_رد cv_hal_gemm32fc
#تعریف_کرو cv_hal_gemm32fc lapack_gemm32fc
#تعریف_رد cv_hal_gemm64fc
#تعریف_کرو cv_hal_gemm64fc lapack_gemm64fc

#جے_انت //HAVE_LAPACK
#جے_انت //OPENCV_CORE_HAL_INTERNAL_HPP
