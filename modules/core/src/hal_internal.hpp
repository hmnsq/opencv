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

#నిర్వచితం_కాకపోతే OPENCV_CORE_HAL_అంతర్గత_HPP
#నిర్వచించు OPENCV_CORE_HAL_అంతర్గత_HPP

#నిర్వచితమైతే HAVE_LAPACK

పూర్ణాంకం lapack_LU32f(దశాంశం* a, size_t a_అడుగు, పూర్ణాంకం m, దశాంశం* b, size_t b_అడుగు, పూర్ణాంకం n, పూర్ణాంకం* info);
పూర్ణాంకం lapack_LU64f(ద్విగుణ* a, size_t a_అడుగు, పూర్ణాంకం m, ద్విగుణ* b, size_t b_అడుగు, పూర్ణాంకం n, పూర్ణాంకం* info);
పూర్ణాంకం lapack_Cholesky32f(దశాంశం* a, size_t a_అడుగు, పూర్ణాంకం m, దశాంశం* b, size_t b_అడుగు, పూర్ణాంకం n, తార్కిక* info);
పూర్ణాంకం lapack_Cholesky64f(ద్విగుణ* a, size_t a_అడుగు, పూర్ణాంకం m, ద్విగుణ* b, size_t b_అడుగు, పూర్ణాంకం n, తార్కిక* info);
పూర్ణాంకం lapack_SVD32f(దశాంశం* a, size_t a_అడుగు, దశాంశం* w, దశాంశం* u, size_t u_అడుగు, దశాంశం* vt, size_t v_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం flags);
పూర్ణాంకం lapack_SVD64f(ద్విగుణ* a, size_t a_అడుగు, ద్విగుణ* w, ద్విగుణ* u, size_t u_అడుగు, ద్విగుణ* vt, size_t v_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం flags);
పూర్ణాంకం lapack_QR32f(దశాంశం* src1, size_t src1_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, దశాంశం* src2, size_t src2_అడుగు, దశాంశం* dst, పూర్ణాంకం* info);
పూర్ణాంకం lapack_QR64f(ద్విగుణ* src1, size_t src1_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, ద్విగుణ* src2, size_t src2_అడుగు, ద్విగుణ* dst, పూర్ణాంకం* info);
పూర్ణాంకం lapack_gemm32f(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
                   దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
                   పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags);
పూర్ణాంకం lapack_gemm64f(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
                   ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
                   పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags);
పూర్ణాంకం lapack_gemm32fc(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
                   దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
                   పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags);
పూర్ణాంకం lapack_gemm64fc(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
                   ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
                   పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags);

#నిర్వచనం_రద్దు cv_hal_LU32f
#నిర్వచించు cv_hal_LU32f lapack_LU32f
#నిర్వచనం_రద్దు cv_hal_LU64f
#నిర్వచించు cv_hal_LU64f lapack_LU64f

#నిర్వచనం_రద్దు cv_hal_Cholesky32f
#నిర్వచించు cv_hal_Cholesky32f lapack_Cholesky32f
#నిర్వచనం_రద్దు cv_hal_Cholesky64f
#నిర్వచించు cv_hal_Cholesky64f lapack_Cholesky64f

#నిర్వచనం_రద్దు cv_hal_SVD32f
#నిర్వచించు cv_hal_SVD32f lapack_SVD32f
#నిర్వచనం_రద్దు cv_hal_SVD64f
#నిర్వచించు cv_hal_SVD64f lapack_SVD64f

#నిర్వచనం_రద్దు cv_hal_QR32f
#నిర్వచించు cv_hal_QR32f lapack_QR32f
#నిర్వచనం_రద్దు cv_hal_QR64f
#నిర్వచించు cv_hal_QR64f lapack_QR64f

#నిర్వచనం_రద్దు cv_hal_gemm32f
#నిర్వచించు cv_hal_gemm32f lapack_gemm32f
#నిర్వచనం_రద్దు cv_hal_gemm64f
#నిర్వచించు cv_hal_gemm64f lapack_gemm64f
#నిర్వచనం_రద్దు cv_hal_gemm32fc
#నిర్వచించు cv_hal_gemm32fc lapack_gemm32fc
#నిర్వచనం_రద్దు cv_hal_gemm64fc
#నిర్వచించు cv_hal_gemm64fc lapack_gemm64fc

#అయితే_ముగింపు //HAVE_LAPACK
#అయితే_ముగింపు //OPENCV_CORE_HAL_INTERNAL_HPP
