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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_HAL_ਅੰਦਰੂਨੀ_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_HAL_ਅੰਦਰੂਨੀ_HPP

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LAPACK

ਪੂਰਨ lapack_LU32f(ਦਸ਼ਮਲਵ* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਪੂਰਨ* info);
ਪੂਰਨ lapack_LU64f(ਦੁੱਗਣਾ* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਪੂਰਨ* info);
ਪੂਰਨ lapack_Cholesky32f(ਦਸ਼ਮਲਵ* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਤਾਰਕਿਕ* info);
ਪੂਰਨ lapack_Cholesky64f(ਦੁੱਗਣਾ* a, size_t a_ਕਦਮ, ਪੂਰਨ m, ਦੁੱਗਣਾ* b, size_t b_ਕਦਮ, ਪੂਰਨ n, ਤਾਰਕਿਕ* info);
ਪੂਰਨ lapack_SVD32f(ਦਸ਼ਮਲਵ* a, size_t a_ਕਦਮ, ਦਸ਼ਮਲਵ* w, ਦਸ਼ਮਲਵ* u, size_t u_ਕਦਮ, ਦਸ਼ਮਲਵ* vt, size_t v_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ flags);
ਪੂਰਨ lapack_SVD64f(ਦੁੱਗਣਾ* a, size_t a_ਕਦਮ, ਦੁੱਗਣਾ* w, ਦੁੱਗਣਾ* u, size_t u_ਕਦਮ, ਦੁੱਗਣਾ* vt, size_t v_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ flags);
ਪੂਰਨ lapack_QR32f(ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ* info);
ਪੂਰਨ lapack_QR64f(ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ* info);
ਪੂਰਨ lapack_gemm32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
                   ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
                   ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags);
ਪੂਰਨ lapack_gemm64f(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
                   ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
                   ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags);
ਪੂਰਨ lapack_gemm32fc(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
                   ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
                   ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags);
ਪੂਰਨ lapack_gemm64fc(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
                   ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
                   ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_LU32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_LU32f lapack_LU32f
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_LU64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_LU64f lapack_LU64f

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_Cholesky32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_Cholesky32f lapack_Cholesky32f
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_Cholesky64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_Cholesky64f lapack_Cholesky64f

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_SVD32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_SVD32f lapack_SVD32f
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_SVD64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_SVD64f lapack_SVD64f

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_QR32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_QR32f lapack_QR32f
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_QR64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_QR64f lapack_QR64f

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_gemm32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_gemm32f lapack_gemm32f
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_gemm64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_gemm64f lapack_gemm64f
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_gemm32fc
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_gemm32fc lapack_gemm32fc
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ cv_hal_gemm64fc
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_gemm64fc lapack_gemm64fc

#ਜੇ_ਅੰਤ //HAVE_LAPACK
#ਜੇ_ਅੰਤ //OPENCV_CORE_HAL_INTERNAL_HPP
