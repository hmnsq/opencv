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

முழு lapack_LU32f(தசம* a, size_t a_step, முழு m, தசம* b, size_t b_step, முழு n, முழு* info);
முழு lapack_LU64f(இரட்டை* a, size_t a_step, முழு m, இரட்டை* b, size_t b_step, முழு n, முழு* info);
முழு lapack_Cholesky32f(தசம* a, size_t a_step, முழு m, தசம* b, size_t b_step, முழு n, தருக்க* info);
முழு lapack_Cholesky64f(இரட்டை* a, size_t a_step, முழு m, இரட்டை* b, size_t b_step, முழு n, தருக்க* info);
முழு lapack_SVD32f(தசம* a, size_t a_step, தசம* w, தசம* u, size_t u_step, தசம* vt, size_t v_step, முழு m, முழு n, முழு flags);
முழு lapack_SVD64f(இரட்டை* a, size_t a_step, இரட்டை* w, இரட்டை* u, size_t u_step, இரட்டை* vt, size_t v_step, முழு m, முழு n, முழு flags);
முழு lapack_QR32f(தசம* src1, size_t src1_step, முழு m, முழு n, முழு k, தசம* src2, size_t src2_step, தசம* dst, முழு* info);
முழு lapack_QR64f(இரட்டை* src1, size_t src1_step, முழு m, முழு n, முழு k, இரட்டை* src2, size_t src2_step, இரட்டை* dst, முழு* info);
முழு lapack_gemm32f(மாறிலி தசம* src1, size_t src1_step, மாறிலி தசம* src2, size_t src2_step,
                   தசம alpha, மாறிலி தசம* src3, size_t src3_step, தசம beta, தசம* dst, size_t dst_step,
                   முழு m, முழு n, முழு k, முழு flags);
முழு lapack_gemm64f(மாறிலி இரட்டை* src1, size_t src1_step, மாறிலி இரட்டை* src2, size_t src2_step,
                   இரட்டை alpha, மாறிலி இரட்டை* src3, size_t src3_step, இரட்டை beta, இரட்டை* dst, size_t dst_step,
                   முழு m, முழு n, முழு k, முழு flags);
முழு lapack_gemm32fc(மாறிலி தசம* src1, size_t src1_step, மாறிலி தசம* src2, size_t src2_step,
                   தசம alpha, மாறிலி தசம* src3, size_t src3_step, தசம beta, தசம* dst, size_t dst_step,
                   முழு m, முழு n, முழு k, முழு flags);
முழு lapack_gemm64fc(மாறிலி இரட்டை* src1, size_t src1_step, மாறிலி இரட்டை* src2, size_t src2_step,
                   இரட்டை alpha, மாறிலி இரட்டை* src3, size_t src3_step, இரட்டை beta, இரட்டை* dst, size_t dst_step,
                   முழு m, முழு n, முழு k, முழு flags);

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
