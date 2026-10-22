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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_HAL_ಆಂತರಿಕ_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_HAL_ಆಂತರಿಕ_HPP

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LAPACK

ಪೂರ್ಣಾಂಕ lapack_LU32f(ದಶಮಾಂಶ* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ* info);
ಪೂರ್ಣಾಂಕ lapack_LU64f(ದ್ವಿಗುಣ* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ* info);
ಪೂರ್ಣಾಂಕ lapack_Cholesky32f(ದಶಮಾಂಶ* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ತಾರ್ಕಿಕ* info);
ಪೂರ್ಣಾಂಕ lapack_Cholesky64f(ದ್ವಿಗುಣ* a, size_t a_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* b, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ತಾರ್ಕಿಕ* info);
ಪೂರ್ಣಾಂಕ lapack_SVD32f(ದಶಮಾಂಶ* a, size_t a_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ* w, ದಶಮಾಂಶ* u, size_t u_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ* vt, size_t v_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ flags);
ಪೂರ್ಣಾಂಕ lapack_SVD64f(ದ್ವಿಗುಣ* a, size_t a_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ* w, ದ್ವಿಗುಣ* u, size_t u_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ* vt, size_t v_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ flags);
ಪೂರ್ಣಾಂಕ lapack_QR32f(ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ* info);
ಪೂರ್ಣಾಂಕ lapack_QR64f(ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ* info);
ಪೂರ್ಣಾಂಕ lapack_gemm32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
                   ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
                   ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags);
ಪೂರ್ಣಾಂಕ lapack_gemm64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
                   ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
                   ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags);
ಪೂರ್ಣಾಂಕ lapack_gemm32fc(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
                   ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
                   ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags);
ಪೂರ್ಣಾಂಕ lapack_gemm64fc(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
                   ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
                   ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags);

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_LU32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_LU32f lapack_LU32f
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_LU64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_LU64f lapack_LU64f

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_Cholesky32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_Cholesky32f lapack_Cholesky32f
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_Cholesky64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_Cholesky64f lapack_Cholesky64f

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_SVD32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_SVD32f lapack_SVD32f
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_SVD64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_SVD64f lapack_SVD64f

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_QR32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_QR32f lapack_QR32f
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_QR64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_QR64f lapack_QR64f

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_gemm32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_gemm32f lapack_gemm32f
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_gemm64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_gemm64f lapack_gemm64f
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_gemm32fc
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_gemm32fc lapack_gemm32fc
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು cv_hal_gemm64fc
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_gemm64fc lapack_gemm64fc

#ಆದರೆ_ಅಂತ್ಯ //HAVE_LAPACK
#ಆದರೆ_ಅಂತ್ಯ //OPENCV_CORE_HAL_INTERNAL_HPP
