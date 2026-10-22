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

#اگر_غیر_تعریف_شدہ OPENCV_CORE_HAL_اندرونی_HPP
#تعریف_کرو OPENCV_CORE_HAL_اندرونی_HPP

#اگر_تعریف_شدہ HAVE_LAPACK

صحیح lapack_LU32f(اعشاری* a, size_t a_قدم, صحیح m, اعشاری* b, size_t b_قدم, صحیح n, صحیح* info);
صحیح lapack_LU64f(دوہرا* a, size_t a_قدم, صحیح m, دوہرا* b, size_t b_قدم, صحیح n, صحیح* info);
صحیح lapack_Cholesky32f(اعشاری* a, size_t a_قدم, صحیح m, اعشاری* b, size_t b_قدم, صحیح n, منطقی* info);
صحیح lapack_Cholesky64f(دوہرا* a, size_t a_قدم, صحیح m, دوہرا* b, size_t b_قدم, صحیح n, منطقی* info);
صحیح lapack_SVD32f(اعشاری* a, size_t a_قدم, اعشاری* w, اعشاری* u, size_t u_قدم, اعشاری* vt, size_t v_قدم, صحیح m, صحیح n, صحیح flags);
صحیح lapack_SVD64f(دوہرا* a, size_t a_قدم, دوہرا* w, دوہرا* u, size_t u_قدم, دوہرا* vt, size_t v_قدم, صحیح m, صحیح n, صحیح flags);
صحیح lapack_QR32f(اعشاری* src1, size_t src1_قدم, صحیح m, صحیح n, صحیح k, اعشاری* src2, size_t src2_قدم, اعشاری* dst, صحیح* info);
صحیح lapack_QR64f(دوہرا* src1, size_t src1_قدم, صحیح m, صحیح n, صحیح k, دوہرا* src2, size_t src2_قدم, دوہرا* dst, صحیح* info);
صحیح lapack_gemm32f(ثابت اعشاری* src1, size_t src1_قدم, ثابت اعشاری* src2, size_t src2_قدم,
                   اعشاری alpha, ثابت اعشاری* src3, size_t src3_قدم, اعشاری beta, اعشاری* dst, size_t dst_قدم,
                   صحیح m, صحیح n, صحیح k, صحیح flags);
صحیح lapack_gemm64f(ثابت دوہرا* src1, size_t src1_قدم, ثابت دوہرا* src2, size_t src2_قدم,
                   دوہرا alpha, ثابت دوہرا* src3, size_t src3_قدم, دوہرا beta, دوہرا* dst, size_t dst_قدم,
                   صحیح m, صحیح n, صحیح k, صحیح flags);
صحیح lapack_gemm32fc(ثابت اعشاری* src1, size_t src1_قدم, ثابت اعشاری* src2, size_t src2_قدم,
                   اعشاری alpha, ثابت اعشاری* src3, size_t src3_قدم, اعشاری beta, اعشاری* dst, size_t dst_قدم,
                   صحیح m, صحیح n, صحیح k, صحیح flags);
صحیح lapack_gemm64fc(ثابت دوہرا* src1, size_t src1_قدم, ثابت دوہرا* src2, size_t src2_قدم,
                   دوہرا alpha, ثابت دوہرا* src3, size_t src3_قدم, دوہرا beta, دوہرا* dst, size_t dst_قدم,
                   صحیح m, صحیح n, صحیح k, صحیح flags);

#تعریف_منسوخ cv_hal_LU32f
#تعریف_کرو cv_hal_LU32f lapack_LU32f
#تعریف_منسوخ cv_hal_LU64f
#تعریف_کرو cv_hal_LU64f lapack_LU64f

#تعریف_منسوخ cv_hal_Cholesky32f
#تعریف_کرو cv_hal_Cholesky32f lapack_Cholesky32f
#تعریف_منسوخ cv_hal_Cholesky64f
#تعریف_کرو cv_hal_Cholesky64f lapack_Cholesky64f

#تعریف_منسوخ cv_hal_SVD32f
#تعریف_کرو cv_hal_SVD32f lapack_SVD32f
#تعریف_منسوخ cv_hal_SVD64f
#تعریف_کرو cv_hal_SVD64f lapack_SVD64f

#تعریف_منسوخ cv_hal_QR32f
#تعریف_کرو cv_hal_QR32f lapack_QR32f
#تعریف_منسوخ cv_hal_QR64f
#تعریف_کرو cv_hal_QR64f lapack_QR64f

#تعریف_منسوخ cv_hal_gemm32f
#تعریف_کرو cv_hal_gemm32f lapack_gemm32f
#تعریف_منسوخ cv_hal_gemm64f
#تعریف_کرو cv_hal_gemm64f lapack_gemm64f
#تعریف_منسوخ cv_hal_gemm32fc
#تعریف_کرو cv_hal_gemm32fc lapack_gemm32fc
#تعریف_منسوخ cv_hal_gemm64fc
#تعریف_کرو cv_hal_gemm64fc lapack_gemm64fc

#اگر_ختم //HAVE_LAPACK
#اگر_ختم //OPENCV_CORE_HAL_INTERNAL_HPP
