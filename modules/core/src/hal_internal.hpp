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

#यदि_अपरिभाषितम् OPENCV_CORE_HAL_आन्तरिकम्_HPP
#परिभाषय OPENCV_CORE_HAL_आन्तरिकम्_HPP

#यदि_परिभाषितम् HAVE_LAPACK

पूर्णाङ्क lapack_LU32f(भिन्न* a, size_t a_चरणः, पूर्णाङ्क m, भिन्न* b, size_t b_चरणः, पूर्णाङ्क n, पूर्णाङ्क* info);
पूर्णाङ्क lapack_LU64f(द्विगुण* a, size_t a_चरणः, पूर्णाङ्क m, द्विगुण* b, size_t b_चरणः, पूर्णाङ्क n, पूर्णाङ्क* info);
पूर्णाङ्क lapack_Cholesky32f(भिन्न* a, size_t a_चरणः, पूर्णाङ्क m, भिन्न* b, size_t b_चरणः, पूर्णाङ्क n, तार्किक* info);
पूर्णाङ्क lapack_Cholesky64f(द्विगुण* a, size_t a_चरणः, पूर्णाङ्क m, द्विगुण* b, size_t b_चरणः, पूर्णाङ्क n, तार्किक* info);
पूर्णाङ्क lapack_SVD32f(भिन्न* a, size_t a_चरणः, भिन्न* w, भिन्न* u, size_t u_चरणः, भिन्न* vt, size_t v_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags);
पूर्णाङ्क lapack_SVD64f(द्विगुण* a, size_t a_चरणः, द्विगुण* w, द्विगुण* u, size_t u_चरणः, द्विगुण* vt, size_t v_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags);
पूर्णाङ्क lapack_QR32f(भिन्न* src1, size_t src1_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, भिन्न* src2, size_t src2_चरणः, भिन्न* dst, पूर्णाङ्क* info);
पूर्णाङ्क lapack_QR64f(द्विगुण* src1, size_t src1_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, द्विगुण* src2, size_t src2_चरणः, द्विगुण* dst, पूर्णाङ्क* info);
पूर्णाङ्क lapack_gemm32f(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
                   भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
                   पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags);
पूर्णाङ्क lapack_gemm64f(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
                   द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
                   पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags);
पूर्णाङ्क lapack_gemm32fc(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
                   भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
                   पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags);
पूर्णाङ्क lapack_gemm64fc(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
                   द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
                   पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags);

#अपरिभाषय cv_hal_LU32f
#परिभाषय cv_hal_LU32f lapack_LU32f
#अपरिभाषय cv_hal_LU64f
#परिभाषय cv_hal_LU64f lapack_LU64f

#अपरिभाषय cv_hal_Cholesky32f
#परिभाषय cv_hal_Cholesky32f lapack_Cholesky32f
#अपरिभाषय cv_hal_Cholesky64f
#परिभाषय cv_hal_Cholesky64f lapack_Cholesky64f

#अपरिभाषय cv_hal_SVD32f
#परिभाषय cv_hal_SVD32f lapack_SVD32f
#अपरिभाषय cv_hal_SVD64f
#परिभाषय cv_hal_SVD64f lapack_SVD64f

#अपरिभाषय cv_hal_QR32f
#परिभाषय cv_hal_QR32f lapack_QR32f
#अपरिभाषय cv_hal_QR64f
#परिभाषय cv_hal_QR64f lapack_QR64f

#अपरिभाषय cv_hal_gemm32f
#परिभाषय cv_hal_gemm32f lapack_gemm32f
#अपरिभाषय cv_hal_gemm64f
#परिभाषय cv_hal_gemm64f lapack_gemm64f
#अपरिभाषय cv_hal_gemm32fc
#परिभाषय cv_hal_gemm32fc lapack_gemm32fc
#अपरिभाषय cv_hal_gemm64fc
#परिभाषय cv_hal_gemm64fc lapack_gemm64fc

#यदि_अन्तः //HAVE_LAPACK
#यदि_अन्तः //OPENCV_CORE_HAL_INTERNAL_HPP
