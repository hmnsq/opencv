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

पूर्णाङ्क lapack_LU32f(भिन्न* a, माप_प्रकार a_step, पूर्णाङ्क m, भिन्न* b, माप_प्रकार b_step, पूर्णाङ्क n, पूर्णाङ्क* info);
पूर्णाङ्क lapack_LU64f(द्विगुण* a, माप_प्रकार a_step, पूर्णाङ्क m, द्विगुण* b, माप_प्रकार b_step, पूर्णाङ्क n, पूर्णाङ्क* info);
पूर्णाङ्क lapack_Cholesky32f(भिन्न* a, माप_प्रकार a_step, पूर्णाङ्क m, भिन्न* b, माप_प्रकार b_step, पूर्णाङ्क n, तार्किक* info);
पूर्णाङ्क lapack_Cholesky64f(द्विगुण* a, माप_प्रकार a_step, पूर्णाङ्क m, द्विगुण* b, माप_प्रकार b_step, पूर्णाङ्क n, तार्किक* info);
पूर्णाङ्क lapack_SVD32f(भिन्न* a, माप_प्रकार a_step, भिन्न* w, भिन्न* u, माप_प्रकार u_step, भिन्न* vt, माप_प्रकार v_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags);
पूर्णाङ्क lapack_SVD64f(द्विगुण* a, माप_प्रकार a_step, द्विगुण* w, द्विगुण* u, माप_प्रकार u_step, द्विगुण* vt, माप_प्रकार v_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags);
पूर्णाङ्क lapack_QR32f(भिन्न* src1, माप_प्रकार src1_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, भिन्न* src2, माप_प्रकार src2_step, भिन्न* dst, पूर्णाङ्क* info);
पूर्णाङ्क lapack_QR64f(द्विगुण* src1, माप_प्रकार src1_step, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, द्विगुण* src2, माप_प्रकार src2_step, द्विगुण* dst, पूर्णाङ्क* info);
पूर्णाङ्क lapack_gemm32f(अचर भिन्न* src1, माप_प्रकार src1_step, अचर भिन्न* src2, माप_प्रकार src2_step,
                   भिन्न alpha, अचर भिन्न* src3, माप_प्रकार src3_step, भिन्न beta, भिन्न* dst, माप_प्रकार dst_step,
                   पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags);
पूर्णाङ्क lapack_gemm64f(अचर द्विगुण* src1, माप_प्रकार src1_step, अचर द्विगुण* src2, माप_प्रकार src2_step,
                   द्विगुण alpha, अचर द्विगुण* src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण* dst, माप_प्रकार dst_step,
                   पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags);
पूर्णाङ्क lapack_gemm32fc(अचर भिन्न* src1, माप_प्रकार src1_step, अचर भिन्न* src2, माप_प्रकार src2_step,
                   भिन्न alpha, अचर भिन्न* src3, माप_प्रकार src3_step, भिन्न beta, भिन्न* dst, माप_प्रकार dst_step,
                   पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags);
पूर्णाङ्क lapack_gemm64fc(अचर द्विगुण* src1, माप_प्रकार src1_step, अचर द्विगुण* src2, माप_प्रकार src2_step,
                   द्विगुण alpha, अचर द्विगुण* src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण* dst, माप_प्रकार dst_step,
                   पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags);

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
