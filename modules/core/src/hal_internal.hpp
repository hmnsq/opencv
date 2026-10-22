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

#अगर_अघोषित OPENCV_CORE_HAL_INTERNAL_HPP
#घोषणा OPENCV_CORE_HAL_INTERNAL_HPP

#अगर_घोषित HAVE_LAPACK

पूर्णांक lapack_LU32f(भग्न* a, माप_प्रकार a_step, पूर्णांक m, भग्न* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info);
पूर्णांक lapack_LU64f(द्विगुन* a, माप_प्रकार a_step, पूर्णांक m, द्विगुन* b, माप_प्रकार b_step, पूर्णांक n, पूर्णांक* info);
पूर्णांक lapack_Cholesky32f(भग्न* a, माप_प्रकार a_step, पूर्णांक m, भग्न* b, माप_प्रकार b_step, पूर्णांक n, bool* info);
पूर्णांक lapack_Cholesky64f(द्विगुन* a, माप_प्रकार a_step, पूर्णांक m, द्विगुन* b, माप_प्रकार b_step, पूर्णांक n, bool* info);
पूर्णांक lapack_SVD32f(भग्न* a, माप_प्रकार a_step, भग्न* w, भग्न* u, माप_प्रकार u_step, भग्न* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags);
पूर्णांक lapack_SVD64f(द्विगुन* a, माप_प्रकार a_step, द्विगुन* w, द्विगुन* u, माप_प्रकार u_step, द्विगुन* vt, माप_प्रकार v_step, पूर्णांक m, पूर्णांक n, पूर्णांक flags);
पूर्णांक lapack_QR32f(भग्न* src1, माप_प्रकार src1_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, भग्न* src2, माप_प्रकार src2_step, भग्न* dst, पूर्णांक* info);
पूर्णांक lapack_QR64f(द्विगुन* src1, माप_प्रकार src1_step, पूर्णांक m, पूर्णांक n, पूर्णांक k, द्विगुन* src2, माप_प्रकार src2_step, द्विगुन* dst, पूर्णांक* info);
पूर्णांक lapack_gemm32f(स्थिर भग्न* src1, माप_प्रकार src1_step, स्थिर भग्न* src2, माप_प्रकार src2_step,
                   भग्न alpha, स्थिर भग्न* src3, माप_प्रकार src3_step, भग्न beta, भग्न* dst, माप_प्रकार dst_step,
                   पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags);
पूर्णांक lapack_gemm64f(स्थिर द्विगुन* src1, माप_प्रकार src1_step, स्थिर द्विगुन* src2, माप_प्रकार src2_step,
                   द्विगुन alpha, स्थिर द्विगुन* src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन* dst, माप_प्रकार dst_step,
                   पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags);
पूर्णांक lapack_gemm32fc(स्थिर भग्न* src1, माप_प्रकार src1_step, स्थिर भग्न* src2, माप_प्रकार src2_step,
                   भग्न alpha, स्थिर भग्न* src3, माप_प्रकार src3_step, भग्न beta, भग्न* dst, माप_प्रकार dst_step,
                   पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags);
पूर्णांक lapack_gemm64fc(स्थिर द्विगुन* src1, माप_प्रकार src1_step, स्थिर द्विगुन* src2, माप_प्रकार src2_step,
                   द्विगुन alpha, स्थिर द्विगुन* src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन* dst, माप_प्रकार dst_step,
                   पूर्णांक m, पूर्णांक n, पूर्णांक k, पूर्णांक flags);

#अघोषित cv_hal_LU32f
#घोषणा cv_hal_LU32f lapack_LU32f
#अघोषित cv_hal_LU64f
#घोषणा cv_hal_LU64f lapack_LU64f

#अघोषित cv_hal_Cholesky32f
#घोषणा cv_hal_Cholesky32f lapack_Cholesky32f
#अघोषित cv_hal_Cholesky64f
#घोषणा cv_hal_Cholesky64f lapack_Cholesky64f

#अघोषित cv_hal_SVD32f
#घोषणा cv_hal_SVD32f lapack_SVD32f
#अघोषित cv_hal_SVD64f
#घोषणा cv_hal_SVD64f lapack_SVD64f

#अघोषित cv_hal_QR32f
#घोषणा cv_hal_QR32f lapack_QR32f
#अघोषित cv_hal_QR64f
#घोषणा cv_hal_QR64f lapack_QR64f

#अघोषित cv_hal_gemm32f
#घोषणा cv_hal_gemm32f lapack_gemm32f
#अघोषित cv_hal_gemm64f
#घोषणा cv_hal_gemm64f lapack_gemm64f
#अघोषित cv_hal_gemm32fc
#घोषणा cv_hal_gemm32fc lapack_gemm32fc
#अघोषित cv_hal_gemm64fc
#घोषणा cv_hal_gemm64fc lapack_gemm64fc

#पूर्ण_अगर //HAVE_LAPACK
#पूर्ण_अगर //OPENCV_CORE_HAL_INTERNAL_HPP
