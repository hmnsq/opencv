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

صحیح lapack_LU32f(اعشاری* a, size_t a_step, صحیح m, اعشاری* b, size_t b_step, صحیح n, صحیح* info);
صحیح lapack_LU64f(دوگانه* a, size_t a_step, صحیح m, دوگانه* b, size_t b_step, صحیح n, صحیح* info);
صحیح lapack_Cholesky32f(اعشاری* a, size_t a_step, صحیح m, اعشاری* b, size_t b_step, صحیح n, منطقی* info);
صحیح lapack_Cholesky64f(دوگانه* a, size_t a_step, صحیح m, دوگانه* b, size_t b_step, صحیح n, منطقی* info);
صحیح lapack_SVD32f(اعشاری* a, size_t a_step, اعشاری* w, اعشاری* u, size_t u_step, اعشاری* vt, size_t v_step, صحیح m, صحیح n, صحیح flags);
صحیح lapack_SVD64f(دوگانه* a, size_t a_step, دوگانه* w, دوگانه* u, size_t u_step, دوگانه* vt, size_t v_step, صحیح m, صحیح n, صحیح flags);
صحیح lapack_QR32f(اعشاری* src1, size_t src1_step, صحیح m, صحیح n, صحیح k, اعشاری* src2, size_t src2_step, اعشاری* dst, صحیح* info);
صحیح lapack_QR64f(دوگانه* src1, size_t src1_step, صحیح m, صحیح n, صحیح k, دوگانه* src2, size_t src2_step, دوگانه* dst, صحیح* info);
صحیح lapack_gemm32f(ثابت اعشاری* src1, size_t src1_step, ثابت اعشاری* src2, size_t src2_step,
                   اعشاری alpha, ثابت اعشاری* src3, size_t src3_step, اعشاری beta, اعشاری* dst, size_t dst_step,
                   صحیح m, صحیح n, صحیح k, صحیح flags);
صحیح lapack_gemm64f(ثابت دوگانه* src1, size_t src1_step, ثابت دوگانه* src2, size_t src2_step,
                   دوگانه alpha, ثابت دوگانه* src3, size_t src3_step, دوگانه beta, دوگانه* dst, size_t dst_step,
                   صحیح m, صحیح n, صحیح k, صحیح flags);
صحیح lapack_gemm32fc(ثابت اعشاری* src1, size_t src1_step, ثابت اعشاری* src2, size_t src2_step,
                   اعشاری alpha, ثابت اعشاری* src3, size_t src3_step, اعشاری beta, اعشاری* dst, size_t dst_step,
                   صحیح m, صحیح n, صحیح k, صحیح flags);
صحیح lapack_gemm64fc(ثابت دوگانه* src1, size_t src1_step, ثابت دوگانه* src2, size_t src2_step,
                   دوگانه alpha, ثابت دوگانه* src3, size_t src3_step, دوگانه beta, دوگانه* dst, size_t dst_step,
                   صحیح m, صحیح n, صحیح k, صحیح flags);

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
