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

പൂർണ്ണസംഖ്യ lapack_LU32f(ദശാംശം* a, size_t a_step, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t b_step, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ* info);
പൂർണ്ണസംഖ്യ lapack_LU64f(ഇരട്ട* a, size_t a_step, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t b_step, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ* info);
പൂർണ്ണസംഖ്യ lapack_Cholesky32f(ദശാംശം* a, size_t a_step, പൂർണ്ണസംഖ്യ m, ദശാംശം* b, size_t b_step, പൂർണ്ണസംഖ്യ n, തർക്കം* info);
പൂർണ്ണസംഖ്യ lapack_Cholesky64f(ഇരട്ട* a, size_t a_step, പൂർണ്ണസംഖ്യ m, ഇരട്ട* b, size_t b_step, പൂർണ്ണസംഖ്യ n, തർക്കം* info);
പൂർണ്ണസംഖ്യ lapack_SVD32f(ദശാംശം* a, size_t a_step, ദശാംശം* w, ദശാംശം* u, size_t u_step, ദശാംശം* vt, size_t v_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ flags);
പൂർണ്ണസംഖ്യ lapack_SVD64f(ഇരട്ട* a, size_t a_step, ഇരട്ട* w, ഇരട്ട* u, size_t u_step, ഇരട്ട* vt, size_t v_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ flags);
പൂർണ്ണസംഖ്യ lapack_QR32f(ദശാംശം* src1, size_t src1_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, ദശാംശം* src2, size_t src2_step, ദശാംശം* dst, പൂർണ്ണസംഖ്യ* info);
പൂർണ്ണസംഖ്യ lapack_QR64f(ഇരട്ട* src1, size_t src1_step, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, ഇരട്ട* src2, size_t src2_step, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ* info);
പൂർണ്ണസംഖ്യ lapack_gemm32f(സ്ഥിരം ദശാംശം* src1, size_t src1_step, സ്ഥിരം ദശാംശം* src2, size_t src2_step,
                   ദശാംശം alpha, സ്ഥിരം ദശാംശം* src3, size_t src3_step, ദശാംശം beta, ദശാംശം* dst, size_t dst_step,
                   പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ flags);
പൂർണ്ണസംഖ്യ lapack_gemm64f(സ്ഥിരം ഇരട്ട* src1, size_t src1_step, സ്ഥിരം ഇരട്ട* src2, size_t src2_step,
                   ഇരട്ട alpha, സ്ഥിരം ഇരട്ട* src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട* dst, size_t dst_step,
                   പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ flags);
പൂർണ്ണസംഖ്യ lapack_gemm32fc(സ്ഥിരം ദശാംശം* src1, size_t src1_step, സ്ഥിരം ദശാംശം* src2, size_t src2_step,
                   ദശാംശം alpha, സ്ഥിരം ദശാംശം* src3, size_t src3_step, ദശാംശം beta, ദശാംശം* dst, size_t dst_step,
                   പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ flags);
പൂർണ്ണസംഖ്യ lapack_gemm64fc(സ്ഥിരം ഇരട്ട* src1, size_t src1_step, സ്ഥിരം ഇരട്ട* src2, size_t src2_step,
                   ഇരട്ട alpha, സ്ഥിരം ഇരട്ട* src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട* dst, size_t dst_step,
                   പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ flags);

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
