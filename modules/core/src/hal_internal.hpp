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

#ifndef OPENCV_CORE_HAL_INTERNAL_HPP
#define OPENCV_CORE_HAL_INTERNAL_HPP

#ifdef HAVE_LAPACK

ಪೂರ್ಣಾಙ್ಕ lapack_LU32f(ಭಿನ್ನ* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ* info);
ಪೂರ್ಣಾಙ್ಕ lapack_LU64f(ದ್ವಿಗುಣ* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ* info);
ಪೂರ್ಣಾಙ್ಕ lapack_Cholesky32f(ಭಿನ್ನ* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ಭಿನ್ನ* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ತಾರ್ಕಿಕ* info);
ಪೂರ್ಣಾಙ್ಕ lapack_Cholesky64f(ದ್ವಿಗುಣ* a, size_t a_step, ಪೂರ್ಣಾಙ್ಕ m, ದ್ವಿಗುಣ* b, size_t b_step, ಪೂರ್ಣಾಙ್ಕ n, ತಾರ್ಕಿಕ* info);
ಪೂರ್ಣಾಙ್ಕ lapack_SVD32f(ಭಿನ್ನ* a, size_t a_step, ಭಿನ್ನ* w, ಭಿನ್ನ* u, size_t u_step, ಭಿನ್ನ* vt, size_t v_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ flags);
ಪೂರ್ಣಾಙ್ಕ lapack_SVD64f(ದ್ವಿಗುಣ* a, size_t a_step, ದ್ವಿಗುಣ* w, ದ್ವಿಗುಣ* u, size_t u_step, ದ್ವಿಗುಣ* vt, size_t v_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ flags);
ಪೂರ್ಣಾಙ್ಕ lapack_QR32f(ಭಿನ್ನ* src1, size_t src1_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಭಿನ್ನ* src2, size_t src2_step, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ* info);
ಪೂರ್ಣಾಙ್ಕ lapack_QR64f(ದ್ವಿಗುಣ* src1, size_t src1_step, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ದ್ವಿಗುಣ* src2, size_t src2_step, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ* info);
ಪೂರ್ಣಾಙ್ಕ lapack_gemm32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ* src2, size_t src2_step,
                   ಭಿನ್ನ alpha, ಧ್ರುವ ಭಿನ್ನ* src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ* dst, size_t dst_step,
                   ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ flags);
ಪೂರ್ಣಾಙ್ಕ lapack_gemm64f(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t src2_step,
                   ದ್ವಿಗುಣ alpha, ಧ್ರುವ ದ್ವಿಗುಣ* src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_step,
                   ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ flags);
ಪೂರ್ಣಾಙ್ಕ lapack_gemm32fc(ಧ್ರುವ ಭಿನ್ನ* src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ* src2, size_t src2_step,
                   ಭಿನ್ನ alpha, ಧ್ರುವ ಭಿನ್ನ* src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ* dst, size_t dst_step,
                   ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ flags);
ಪೂರ್ಣಾಙ್ಕ lapack_gemm64fc(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t src2_step,
                   ದ್ವಿಗುಣ alpha, ಧ್ರುವ ದ್ವಿಗುಣ* src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_step,
                   ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ flags);

#undef cv_hal_LU32f
#define cv_hal_LU32f lapack_LU32f
#undef cv_hal_LU64f
#define cv_hal_LU64f lapack_LU64f

#undef cv_hal_Cholesky32f
#define cv_hal_Cholesky32f lapack_Cholesky32f
#undef cv_hal_Cholesky64f
#define cv_hal_Cholesky64f lapack_Cholesky64f

#undef cv_hal_SVD32f
#define cv_hal_SVD32f lapack_SVD32f
#undef cv_hal_SVD64f
#define cv_hal_SVD64f lapack_SVD64f

#undef cv_hal_QR32f
#define cv_hal_QR32f lapack_QR32f
#undef cv_hal_QR64f
#define cv_hal_QR64f lapack_QR64f

#undef cv_hal_gemm32f
#define cv_hal_gemm32f lapack_gemm32f
#undef cv_hal_gemm64f
#define cv_hal_gemm64f lapack_gemm64f
#undef cv_hal_gemm32fc
#define cv_hal_gemm32fc lapack_gemm32fc
#undef cv_hal_gemm64fc
#define cv_hal_gemm64fc lapack_gemm64fc

#endif //HAVE_LAPACK
#endif //OPENCV_CORE_HAL_INTERNAL_HPP
