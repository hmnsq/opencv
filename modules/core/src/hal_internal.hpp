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

पूर्णांक lapack_LU32f(दशांश* a, माप_प्रकार a_step, पूर्णांक m, दशांश* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info);
पूर्णांक lapack_LU64f(द्विगुण* a, माप_प्रकार a_step, पूर्णांक m, द्विगुण* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info);
पूर्णांक lapack_Cholesky32f(दशांश* a, माप_प्रकार a_step, पूर्णांक m, दशांश* b, माप_प्रकार b_step, पूर्णांक n, तार्किक* info);
पूर्णांक lapack_Cholesky64f(द्विगुण* a, माप_प्रकार a_step, पूर्णांक m, द्विगुण* b, माप_प्रकार b_step, पूर्णांक n, तार्किक* info);
पूर्णांक lapack_SVD32f(दशांश* a, माप_प्रकार a_step, दशांश* w, दशांश* u, माप_प्रकार u_step, दशांश* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags);
पूर्णांक lapack_SVD64f(द्विगुण* a, माप_प्रकार a_step, द्विगुण* w, द्विगुण* u, माप_प्रकार u_step, द्विगुण* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags);
पूर्णांक lapack_QR32f(दशांश* src1, माप_प्रकार src1_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, दशांश* src2, माप_प्रकार src2_step, दशांश* dst, पूर्णांक* info);
पूर्णांक lapack_QR64f(द्विगुण* src1, माप_प्रकार src1_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, द्विगुण* src2, माप_प्रकार src2_step, द्विगुण* dst, पूर्णांक* info);
पूर्णांक lapack_gemm32f(स्थिरांक दशांश* src1, माप_प्रकार src1_step, स्थिरांक दशांश* src2, माप_प्रकार src2_step,
                   दशांश alpha, स्थिरांक दशांश* src3, माप_प्रकार src3_step, दशांश beta, दशांश* dst, माप_प्रकार dst_step,
                   पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags);
पूर्णांक lapack_gemm64f(स्थिरांक द्विगुण* src1, माप_प्रकार src1_step, स्थिरांक द्विगुण* src2, माप_प्रकार src2_step,
                   द्विगुण alpha, स्थिरांक द्विगुण* src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण* dst, माप_प्रकार dst_step,
                   पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags);
पूर्णांक lapack_gemm32fc(स्थिरांक दशांश* src1, माप_प्रकार src1_step, स्थिरांक दशांश* src2, माप_प्रकार src2_step,
                   दशांश alpha, स्थिरांक दशांश* src3, माप_प्रकार src3_step, दशांश beta, दशांश* dst, माप_प्रकार dst_step,
                   पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags);
पूर्णांक lapack_gemm64fc(स्थिरांक द्विगुण* src1, माप_प्रकार src1_step, स्थिरांक द्विगुण* src2, माप_प्रकार src2_step,
                   द्विगुण alpha, स्थिरांक द्विगुण* src3, माप_प्रकार src3_step, द्विगुण beta, द्विगुण* dst, माप_प्रकार dst_step,
                   पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags);

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
