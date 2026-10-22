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

#यदि_अपरिभाषितम् OPENCV_CORE_HAL_REPLACEMENT_HPP
#परिभाषय OPENCV_CORE_HAL_REPLACEMENT_HPP

#अन्तर्भाव "opencv2/core/hal/interface.h"

#यदि defined(__clang__)  // clang or MSVC clang
#निर्देशः clang diagnostic push
#निर्देशः clang diagnostic ignored "-Wunused-parameter"
#नोचेद्यदि defined(_MSC_VER)
#निर्देशः warning(push)
#निर्देशः warning(disable : 4100)
#नोचेद्यदि defined(__GNUC__)
#निर्देशः GCC diagnostic push
#निर्देशः GCC diagnostic ignored "-Wunused-parameter"
#यदि_अन्तः

//! @addtogroup core_hal_interface
//! @note Define your functions to override default implementations:
//! @code
//! #undef hal_add8u
//! #define hal_add8u my_add8u
//! @endcode
//! @{

/**
Add: _dst[i] = src1[i] + src2[i]_ @n
Sub: _dst[i] = src1[i] - src2[i]_
@param src1_data first source image data
@param src1_step first source image step
@param src2_data second source image data
@param src2_step second source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
*/
//! @addtogroup core_hal_interface_addsub Element-wise add and subtract
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_add8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_add16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

अन्तःस्थ पूर्णाङ्क hal_ni_sub8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

अन्तःस्थ पूर्णाङ्क hal_ni_sub8u32f(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sub8s32f(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

/**
Add scalar: _dst[i] = src[i] + scalar

@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
@param scalar_data pointer to scalar value
@param nChannels number of channels per element
*/
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Scalar32f32f(ध्रुव भिन्न*   src_दत्त, size_t src_चरणः, भिन्न*   dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव भिन्न*   scalar_दत्त, पूर्णाङ्क nChannels) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Scalar16s16s(ध्रुव int16_t* src_दत्त, size_t src_चरणः, int16_t* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव int16_t* scalar_दत्त, पूर्णाङ्क nChannels) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
Minimum: _dst[i] = min(src1[i], src2[i])_ @n
Maximum: _dst[i] = max(src1[i], src2[i])_
@param src1_data first source image data
@param src1_step first source image step
@param src2_data second source image data
@param src2_step second source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
*/
//! @addtogroup core_hal_interface_minmax Element-wise minimum or maximum
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_max8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_max16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

अन्तःस्थ पूर्णाङ्क hal_ni_min8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_min16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
Absolute difference: _dst[i] = | src1[i] - src2[i] |_
@param src1_data first source image data
@param src1_step first source image step
@param src2_data second source image data
@param src2_step second source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
*/
//! @addtogroup core_hal_interface_absdiff Element-wise absolute difference
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absdiff16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

/*
Absolute difference with scalar: _dst[i] = | src[i] - scalar |_

@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
@param scalar_data pointer to scalar value
@param nChannels number of channels per element
*/
अन्तःस्थ पूर्णाङ्क hal_ni_absDiffScalar32f32f(ध्रुव भिन्न* src_दत्त, size_t src_चरणः, भिन्न*    dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव भिन्न* scalar_दत्त, पूर्णाङ्क nChannels) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absDiffScalar32s32u(ध्रुव पूर्णाङ्क*   src_दत्त, size_t src_चरणः, uint32_t* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव पूर्णाङ्क*   scalar_दत्त, पूर्णाङ्क nChannels) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_absDiffScalar8u8u  (ध्रुव uchar* src_दत्त, size_t src_चरणः, uchar*    dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव uchar* scalar_दत्त, पूर्णाङ्क nChannels) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @}

/**
Bitwise AND: _dst[i] = src1[i] & src2[i]_ @n
Bitwise OR: _dst[i] = src1[i] | src2[i]_ @n
Bitwise XOR: _dst[i] = src1[i] ^ src2[i]_ @n
Bitwise NOT: _dst[i] = ~src[i]_
@param src1_data first source image data
@param src1_step first source image step
@param src2_data second source image data
@param src2_step second source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
 */
//! @addtogroup core_hal_interface_logical Bitwise logical operations
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_and8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_or8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_xor8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_not8u(ध्रुव uchar *src_दत्त, size_t src_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_add8u hal_ni_add8u
#परिभाषय cv_hal_add8s hal_ni_add8s
#परिभाषय cv_hal_add16u hal_ni_add16u
#परिभाषय cv_hal_add16s hal_ni_add16s
#परिभाषय cv_hal_add32u hal_ni_add32u
#परिभाषय cv_hal_add32s hal_ni_add32s
#परिभाषय cv_hal_add64u hal_ni_add64u
#परिभाषय cv_hal_add64s hal_ni_add64s
#परिभाषय cv_hal_add32f hal_ni_add32f
#परिभाषय cv_hal_add64f hal_ni_add64f
#परिभाषय cv_hal_add16f hal_ni_add16f
#परिभाषय cv_hal_add16bf hal_ni_add16bf
#परिभाषय cv_hal_sub8u hal_ni_sub8u
#परिभाषय cv_hal_sub8s hal_ni_sub8s
#परिभाषय cv_hal_sub16u hal_ni_sub16u
#परिभाषय cv_hal_sub16s hal_ni_sub16s
#परिभाषय cv_hal_sub32u hal_ni_sub32u
#परिभाषय cv_hal_sub32s hal_ni_sub32s
#परिभाषय cv_hal_sub64u hal_ni_sub64u
#परिभाषय cv_hal_sub64s hal_ni_sub64s
#परिभाषय cv_hal_sub32f hal_ni_sub32f
#परिभाषय cv_hal_sub64f hal_ni_sub64f
#परिभाषय cv_hal_sub8u32f hal_ni_sub8u32f
#परिभाषय cv_hal_sub8s32f hal_ni_sub8s32f
#परिभाषय cv_hal_sub16f hal_ni_sub16f
#परिभाषय cv_hal_sub16bf hal_ni_sub16bf
#परिभाषय cv_hal_सङ्कलय_Scalar32f32f hal_ni_सङ्कलय_Scalar32f32f
#परिभाषय cv_hal_सङ्कलय_Scalar16s16s hal_ni_सङ्कलय_Scalar16s16s
#परिभाषय cv_hal_max8u hal_ni_max8u
#परिभाषय cv_hal_max8s hal_ni_max8s
#परिभाषय cv_hal_max16u hal_ni_max16u
#परिभाषय cv_hal_max16s hal_ni_max16s
#परिभाषय cv_hal_max32u hal_ni_max32u
#परिभाषय cv_hal_max32s hal_ni_max32s
#परिभाषय cv_hal_max64u hal_ni_max64u
#परिभाषय cv_hal_max64s hal_ni_max64s
#परिभाषय cv_hal_max32f hal_ni_max32f
#परिभाषय cv_hal_max64f hal_ni_max64f
#परिभाषय cv_hal_max16f hal_ni_max16f
#परिभाषय cv_hal_max16bf hal_ni_max16bf
#परिभाषय cv_hal_min8u hal_ni_min8u
#परिभाषय cv_hal_min8s hal_ni_min8s
#परिभाषय cv_hal_min16u hal_ni_min16u
#परिभाषय cv_hal_min16s hal_ni_min16s
#परिभाषय cv_hal_min32u hal_ni_min32u
#परिभाषय cv_hal_min32s hal_ni_min32s
#परिभाषय cv_hal_min64u hal_ni_min64u
#परिभाषय cv_hal_min64s hal_ni_min64s
#परिभाषय cv_hal_min32f hal_ni_min32f
#परिभाषय cv_hal_min64f hal_ni_min64f
#परिभाषय cv_hal_min16f hal_ni_min16f
#परिभाषय cv_hal_min16bf hal_ni_min16bf
#परिभाषय cv_hal_absdiff8u hal_ni_absdiff8u
#परिभाषय cv_hal_absdiff8s hal_ni_absdiff8s
#परिभाषय cv_hal_absdiff16u hal_ni_absdiff16u
#परिभाषय cv_hal_absdiff16s hal_ni_absdiff16s
#परिभाषय cv_hal_absdiff32u hal_ni_absdiff32u
#परिभाषय cv_hal_absdiff32s hal_ni_absdiff32s
#परिभाषय cv_hal_absdiff64u hal_ni_absdiff64u
#परिभाषय cv_hal_absdiff64s hal_ni_absdiff64s
#परिभाषय cv_hal_absdiff32f hal_ni_absdiff32f
#परिभाषय cv_hal_absdiff64f hal_ni_absdiff64f
#परिभाषय cv_hal_absdiff16f hal_ni_absdiff16f
#परिभाषय cv_hal_absdiff16bf hal_ni_absdiff16bf
#परिभाषय cv_hal_absDiffScalar32f32f hal_ni_absDiffScalar32f32f
#परिभाषय cv_hal_absDiffScalar32s32u hal_ni_absDiffScalar32s32u
#परिभाषय cv_hal_absDiffScalar8u8u   hal_ni_absDiffScalar8u8u
#परिभाषय cv_hal_and8u hal_ni_and8u
#परिभाषय cv_hal_or8u hal_ni_or8u
#परिभाषय cv_hal_xor8u hal_ni_xor8u
#परिभाषय cv_hal_not8u hal_ni_not8u
//! @endcond

/**
Lookup table replacement
Table consists of 256 elements of a size from 1 to 8 bytes having 1 channel or src_channels
For 8s input type 128 is added to LUT index
Destination should have the same element type and number of channels as lookup table elements
@param src_data Source image data
@param src_step Source image step
@param src_type Source image type
@param lut_data Pointer to lookup table
@param lut_channel_size Size of each channel in bytes
@param lut_channels Number of channels in lookup table
@param dst_data Destination data
@param dst_step Destination step
@param width Width of images
@param height Height of images
@sa LUT
*/
//! @addtogroup core_hal_interface_lut Lookup table
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_lut(ध्रुव uchar *src_दत्त, size_t src_चरणः, size_t src_type, ध्रुव uchar* lut_दत्त, size_t lut_channel_आकार, size_t lut_channels, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_lut hal_ni_lut
//! @endcond

/**
Lookup table replacement
Table consists of 65536 elements of a size from 1 to 8 bytes having 1 channel or src_channels
For 16s input typea 32768 is added to LUT index
Destination should have the same element type and number of channels as lookup table elements
@param src_data Source image data
@param src_step Source image step
@param src_type Source image type
@param lut_data Pointer to lookup table
@param lut_channel_size Size of each channel in bytes
@param lut_channels Number of channels in lookup table
@param dst_data Destination data
@param dst_step Destination step
@param width Width of images
@param height Height of images
@sa LUT
*/
//! @addtogroup core_hal_interface_lut16 Lookup table for 16 bit index
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_lut16(ध्रुव ushort *src_दत्त, size_t src_चरणः, size_t src_type, ध्रुव uchar* lut_दत्त, size_t lut_channel_आकार, size_t lut_channels, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_lut16 hal_ni_lut16
//! @endcond

/**
Hamming norm of a vector
@param a pointer to vector data
@param n length of a vector
@param cellSize how many bits of the vector will be added and treated as a single bit, can be 1 (standard Hamming distance), 2 or 4
@param result pointer to result output
*/
//! @addtogroup core_hal_interface_hamming Hamming distance
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_normHamming8u(ध्रुव uchar* a, पूर्णाङ्क n, पूर्णाङ्क cell_आकार, पूर्णाङ्क* परिणाम) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
Hamming distance between two vectors
@param a pointer to first vector data
@param b pointer to second vector data
@param n length of vectors
@param cellSize how many bits of the vectors will be added and treated as a single bit, can be 1 (standard Hamming distance), 2 or 4
@param result pointer to result output
*/
//! @addtogroup core_hal_interface_hamming Hamming distance
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_normHammingDiff8u(ध्रुव uchar* a, ध्रुव uchar* b, पूर्णाङ्क n, पूर्णाङ्क cell_आकार, पूर्णाङ्क* परिणाम) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
@brief Generic norm of an array.
@param src Source image
@param src_step Source image
@param mask Specified array region.
@param mask_step Mask array step.
@param width Source image dimensions
@param height Source image dimensions
@param type Element type of source image
@param norm_type Type of the norm
@param result Pointer to result output
*/
//! @addtogroup core_hal_interface_norm Absolute norm
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_norm(ध्रुव uchar* src, size_t src_चरणः, ध्रुव uchar* आवरणम्, size_t आवरणम्_चरणः, पूर्णाङ्क विस्तारः,
                       पूर्णाङ्क height, पूर्णाङ्क type, पूर्णाङ्क norm_type, द्विगुण* परिणाम) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
@brief Generic norm between two arrays.
@param src1 First source image
@param src1_step First source image
@param src2 Second source image
@param src2_step Second source image
@param mask Specified array region.
@param mask_step Mask array step.
@param width Source image dimensions
@param height Source image dimensions
@param type Element type of source image
@param norm_type Type of the norm
@param result Pointer to result output
*/
//! @addtogroup core_hal_interface_norm Absolute norm
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_normDiff(ध्रुव uchar* src1, size_t src1_चरणः, ध्रुव uchar* src2, size_t src2_चरणः, ध्रुव uchar* आवरणम्,
                           size_t आवरणम्_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क type, पूर्णाङ्क norm_type, द्विगुण* परिणाम) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
@brief Convert array to another with specified type.
@param src Source image
@param src_step Source image
@param dst Destination image
@param dst_step Destination image
@param width Source image dimensions
@param height Source image dimensions
@param sdepth Depth of source image
@param ddepth Depth of destination image
@param alpha Scale value
@param beta Shift value
*/
//! @addtogroup core_hal_interface_convert Array convert
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_convertScale(ध्रुव uchar* src, size_t src_चरणः, uchar* dst, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height,
                               पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, द्विगुण alpha, द्विगुण beta) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_normHamming8u hal_ni_normHamming8u
#परिभाषय cv_hal_normHammingDiff8u hal_ni_normHammingDiff8u
#परिभाषय cv_hal_norm hal_ni_norm
#परिभाषय cv_hal_normDiff hal_ni_normDiff
#परिभाषय cv_hal_convertScale hal_ni_convertScale
//! @endcond

/**
Compare: _dst[i] = src1[i] op src2[i]_
@param src1_data first source image data
@param src1_step first source image step
@param src2_data second source image data
@param src2_step second source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
@param operation one of (CV_HAL_CMP_EQ, CV_HAL_CMP_GT, ...)
*/
//! @addtogroup core_hal_interface_compare Element-wise compare
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_cmp8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cmp16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क operation) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_cmp8u hal_ni_cmp8u
#परिभाषय cv_hal_cmp8s hal_ni_cmp8s
#परिभाषय cv_hal_cmp16u hal_ni_cmp16u
#परिभाषय cv_hal_cmp16s hal_ni_cmp16s
#परिभाषय cv_hal_cmp32u hal_ni_cmp32u
#परिभाषय cv_hal_cmp32s hal_ni_cmp32s
#परिभाषय cv_hal_cmp64u hal_ni_cmp64u
#परिभाषय cv_hal_cmp64s hal_ni_cmp64s
#परिभाषय cv_hal_cmp32f hal_ni_cmp32f
#परिभाषय cv_hal_cmp64f hal_ni_cmp64f
#परिभाषय cv_hal_cmp16f hal_ni_cmp16f
#परिभाषय cv_hal_cmp16bf hal_ni_cmp16bf
//! @endcond

/**
Multiply: _dst[i] = scale * src1[i] * src2[i]_
@param src1_data first source image data
@param src1_step first source image step
@param src2_data second source image data
@param src2_step second source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
@param scale additional multiplier
*/
//! @addtogroup core_hal_interface_multiply Element-wise multiply
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_mul8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul8u16u(ध्रुव uchar* src1_दत्त, size_t src1_चरणः, ध्रुव uchar* src2_दत्त, size_t src2_चरणः, ushort* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul8s16s(ध्रुव schar* src1_दत्त, size_t src1_चरणः, ध्रुव schar* src2_दत्त, size_t src2_चरणः, ह्रस्व* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_mul16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
Divide: _dst[i] = scale * src1[i] / src2[i]_
@param src1_data first source image data and step
@param src1_step first source image data and step
@param src2_data second source image data and step
@param src2_step second source image data and step
@param dst_data destination image data and step
@param dst_step destination image data and step
@param width dimensions of the images
@param height dimensions of the images
@param scale additional multiplier
*/
//! @addtogroup core_hal_interface_divide Element-wise divide
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_div8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_div16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
Computes reciprocal: _dst[i] = scale / src[i]_
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
@param scale additional multiplier
 */
//! @addtogroup core_hal_interface_reciprocal Element-wise reciprocal
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_recip8u(ध्रुव uchar *src_दत्त, size_t src_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip8s(ध्रुव schar *src_दत्त, size_t src_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip16u(ध्रुव ushort *src_दत्त, size_t src_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip16s(ध्रुव ह्रस्व *src_दत्त, size_t src_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip32u(ध्रुव अचिह्नित *src_दत्त, size_t src_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip32s(ध्रुव पूर्णाङ्क *src_दत्त, size_t src_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip32f(ध्रुव भिन्न *src_दत्त, size_t src_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip64f(ध्रुव द्विगुण *src_दत्त, size_t src_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip64u(ध्रुव uint64 *src_दत्त, size_t src_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip64s(ध्रुव int64 *src_दत्त, size_t src_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip16f(ध्रुव cv_hal_f16 *src_दत्त, size_t src_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_recip16bf(ध्रुव cv_hal_bf16 *src_दत्त, size_t src_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण scale) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_mul8u hal_ni_mul8u
#परिभाषय cv_hal_mul8s hal_ni_mul8s
#परिभाषय cv_hal_mul16u hal_ni_mul16u
#परिभाषय cv_hal_mul16s hal_ni_mul16s
#परिभाषय cv_hal_mul32u hal_ni_mul32u
#परिभाषय cv_hal_mul32s hal_ni_mul32s
#परिभाषय cv_hal_mul64u hal_ni_mul64u
#परिभाषय cv_hal_mul64s hal_ni_mul64s
#परिभाषय cv_hal_mul32f hal_ni_mul32f
#परिभाषय cv_hal_mul64f hal_ni_mul64f
#परिभाषय cv_hal_mul8u16u hal_ni_mul8u16u
#परिभाषय cv_hal_mul8s16s hal_ni_mul8s16s
#परिभाषय cv_hal_mul16f hal_ni_mul16f
#परिभाषय cv_hal_mul16bf hal_ni_mul16bf
#परिभाषय cv_hal_div8u hal_ni_div8u
#परिभाषय cv_hal_div8s hal_ni_div8s
#परिभाषय cv_hal_div16u hal_ni_div16u
#परिभाषय cv_hal_div16s hal_ni_div16s
#परिभाषय cv_hal_div32u hal_ni_div32u
#परिभाषय cv_hal_div32s hal_ni_div32s
#परिभाषय cv_hal_div64u hal_ni_div64u
#परिभाषय cv_hal_div64s hal_ni_div64s
#परिभाषय cv_hal_div32f hal_ni_div32f
#परिभाषय cv_hal_div64f hal_ni_div64f
#परिभाषय cv_hal_div16f hal_ni_div16f
#परिभाषय cv_hal_div16bf hal_ni_div16bf
#परिभाषय cv_hal_recip8u hal_ni_recip8u
#परिभाषय cv_hal_recip8s hal_ni_recip8s
#परिभाषय cv_hal_recip16u hal_ni_recip16u
#परिभाषय cv_hal_recip16s hal_ni_recip16s
#परिभाषय cv_hal_recip32u hal_ni_recip32u
#परिभाषय cv_hal_recip32s hal_ni_recip32s
#परिभाषय cv_hal_recip64u hal_ni_recip64u
#परिभाषय cv_hal_recip64s hal_ni_recip64s
#परिभाषय cv_hal_recip32f hal_ni_recip32f
#परिभाषय cv_hal_recip64f hal_ni_recip64f
#परिभाषय cv_hal_recip16f hal_ni_recip16f
#परिभाषय cv_hal_recip16bf hal_ni_recip16bf
//! @endcond

/**
Computes weighted sum of two arrays using formula: _dst[i] = a * src1[i] + b * src2[i] + c_
@param src1_data first source image data
@param src1_step first source image step
@param src2_data second source image data
@param src2_step second source image step
@param dst_data destination image data
@param dst_step destination image step
@param width width of the images
@param height height of the images
@param scalars numbers _a_, _b_, and _c_
 */
//! @addtogroup core_hal_interface_addWeighted Element-wise weighted sum
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted8u(ध्रुव uchar *src1_दत्त, size_t src1_चरणः, ध्रुव uchar *src2_दत्त, size_t src2_चरणः, uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted8s(ध्रुव schar *src1_दत्त, size_t src1_चरणः, ध्रुव schar *src2_दत्त, size_t src2_चरणः, schar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted16u(ध्रुव ushort *src1_दत्त, size_t src1_चरणः, ध्रुव ushort *src2_दत्त, size_t src2_चरणः, ushort *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted16s(ध्रुव ह्रस्व *src1_दत्त, size_t src1_चरणः, ध्रुव ह्रस्व *src2_दत्त, size_t src2_चरणः, ह्रस्व *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted32u(ध्रुव अचिह्नित *src1_दत्त, size_t src1_चरणः, ध्रुव अचिह्नित *src2_दत्त, size_t src2_चरणः, अचिह्नित *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted32s(ध्रुव पूर्णाङ्क *src1_दत्त, size_t src1_चरणः, ध्रुव पूर्णाङ्क *src2_दत्त, size_t src2_चरणः, पूर्णाङ्क *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted32f(ध्रुव भिन्न *src1_दत्त, size_t src1_चरणः, ध्रुव भिन्न *src2_दत्त, size_t src2_चरणः, भिन्न *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted64f(ध्रुव द्विगुण *src1_दत्त, size_t src1_चरणः, ध्रुव द्विगुण *src2_दत्त, size_t src2_चरणः, द्विगुण *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted64u(ध्रुव uint64 *src1_दत्त, size_t src1_चरणः, ध्रुव uint64 *src2_दत्त, size_t src2_चरणः, uint64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted64s(ध्रुव int64 *src1_दत्त, size_t src1_चरणः, ध्रुव int64 *src2_दत्त, size_t src2_चरणः, int64 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted16f(ध्रुव cv_hal_f16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_f16 *src2_दत्त, size_t src2_चरणः, cv_hal_f16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_सङ्कलय_Weighted16bf(ध्रुव cv_hal_bf16 *src1_दत्त, size_t src1_चरणः, ध्रुव cv_hal_bf16 *src2_दत्त, size_t src2_चरणः, cv_hal_bf16 *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, ध्रुव द्विगुण scalars[3]) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_सङ्कलय_Weighted8u hal_ni_सङ्कलय_Weighted8u
#परिभाषय cv_hal_सङ्कलय_Weighted8s hal_ni_सङ्कलय_Weighted8s
#परिभाषय cv_hal_सङ्कलय_Weighted16u hal_ni_सङ्कलय_Weighted16u
#परिभाषय cv_hal_सङ्कलय_Weighted16s hal_ni_सङ्कलय_Weighted16s
#परिभाषय cv_hal_सङ्कलय_Weighted32u hal_ni_सङ्कलय_Weighted32u
#परिभाषय cv_hal_सङ्कलय_Weighted32s hal_ni_सङ्कलय_Weighted32s
#परिभाषय cv_hal_सङ्कलय_Weighted64u hal_ni_सङ्कलय_Weighted64u
#परिभाषय cv_hal_सङ्कलय_Weighted64s hal_ni_सङ्कलय_Weighted64s
#परिभाषय cv_hal_सङ्कलय_Weighted32f hal_ni_सङ्कलय_Weighted32f
#परिभाषय cv_hal_सङ्कलय_Weighted64f hal_ni_सङ्कलय_Weighted64f
#परिभाषय cv_hal_सङ्कलय_Weighted16f hal_ni_सङ्कलय_Weighted16f
#परिभाषय cv_hal_सङ्कलय_Weighted16bf hal_ni_सङ्कलय_Weighted16bf
//! @endcond

/**
@param src_data array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param dst_data array of pointers to destination arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_split Channel split
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_split8u(ध्रुव uchar *src_दत्त, uchar **dst_दत्त, पूर्णाङ्क len, पूर्णाङ्क cn) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_split16u(ध्रुव ushort *src_दत्त, ushort **dst_दत्त, पूर्णाङ्क len, पूर्णाङ्क cn) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_split32s(ध्रुव पूर्णाङ्क *src_दत्त, पूर्णाङ्क **dst_दत्त, पूर्णाङ्क len, पूर्णाङ्क cn) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_split64s(ध्रुव int64 *src_दत्त, int64 **dst_दत्त, पूर्णाङ्क len, पूर्णाङ्क cn) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_split8u hal_ni_split8u
#परिभाषय cv_hal_split16u hal_ni_split16u
#परिभाषय cv_hal_split32s hal_ni_split32s
#परिभाषय cv_hal_split64s hal_ni_split64s
//! @endcond

/**
@param src_data array of pointers to source arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param dst_data destination array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_merge Channel merge
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_merge8u(ध्रुव uchar **src_दत्त, uchar *dst_दत्त, पूर्णाङ्क len, पूर्णाङ्क cn) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_merge16u(ध्रुव ushort **src_दत्त, ushort *dst_दत्त, पूर्णाङ्क len, पूर्णाङ्क cn) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_merge32s(ध्रुव पूर्णाङ्क **src_दत्त, पूर्णाङ्क *dst_दत्त, पूर्णाङ्क len, पूर्णाङ्क cn) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_merge64s(ध्रुव int64 **src_दत्त, int64 *dst_दत्त, पूर्णाङ्क len, पूर्णाङ्क cn) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_merge8u hal_ni_merge8u
#परिभाषय cv_hal_merge16u hal_ni_merge16u
#परिभाषय cv_hal_merge32s hal_ni_merge32s
#परिभाषय cv_hal_merge64s hal_ni_merge64s
//! @endcond

/**
@param x source X arrays
@param y source Y arrays
@param mag destination magnitude array
@param angle destination angle array
@param len length of arrays
@param angleInDegrees if set to true return angles in degrees, otherwise in radians
*/
//! @addtogroup core_hal_interface_fastAtan Atan calculation
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_cart_गन्तव्यम्_Polar32f(ध्रुव भिन्न* x, ध्रुव भिन्न* y, भिन्न* mag, भिन्न* angle, पूर्णाङ्क len, तार्किक angle_अन्तर्_Degrees) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_cart_गन्तव्यम्_Polar64f(ध्रुव द्विगुण* x, ध्रुव द्विगुण* y, द्विगुण* mag, द्विगुण* angle, पूर्णाङ्क len, तार्किक angle_अन्तर्_Degrees) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_cart_गन्तव्यम्_Polar32f hal_ni_cart_गन्तव्यम्_Polar32f
#परिभाषय cv_hal_cart_गन्तव्यम्_Polar64f hal_ni_cart_गन्तव्यम्_Polar64f
//! @endcond

/**
@param y source Y arrays
@param x source X arrays
@param dst destination array
@param len length of arrays
@param angleInDegrees if set to true return angles in degrees, otherwise in radians
 */
//! @addtogroup core_hal_interface_fastAtan Atan calculation
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_fastAtan32f(ध्रुव भिन्न* y, ध्रुव भिन्न* x, भिन्न* dst, पूर्णाङ्क len, तार्किक angle_अन्तर्_Degrees) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_fastAtan64f(ध्रुव द्विगुण* y, ध्रुव द्विगुण* x, द्विगुण* dst, पूर्णाङ्क len, तार्किक angle_अन्तर्_Degrees) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_fastAtan32f hal_ni_fastAtan32f
#परिभाषय cv_hal_fastAtan64f hal_ni_fastAtan64f
//! @endcond


/**
@param x source X array
@param y source Y array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_magnitude Magnitude calculation
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_magnitude32f(ध्रुव भिन्न *x, ध्रुव भिन्न *y, भिन्न *dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_magnitude64f(ध्रुव द्विगुण *x, ध्रुव द्विगुण  *y, द्विगुण *dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_magnitude32f hal_ni_magnitude32f
#परिभाषय cv_hal_magnitude64f hal_ni_magnitude64f
//! @endcond

/**
@param mag source magnitude arrays
@param mag source angle arrays
@param x destination X array
@param y destination Y array
@param len length of arrays
@param angleInDegrees if set to true interpret angles from degrees, otherwise from radians
*/
//! @addtogroup core_hal_interface_fastAtan Atan calculation
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_polar_गन्तव्यम्_Cart32f(ध्रुव भिन्न* mag, ध्रुव भिन्न* angle, भिन्न* x, भिन्न* y, पूर्णाङ्क len, तार्किक angle_अन्तर्_Degrees) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_polar_गन्तव्यम्_Cart64f(ध्रुव द्विगुण* mag, ध्रुव द्विगुण* angle, द्विगुण* x, द्विगुण* y, पूर्णाङ्क len, तार्किक angle_अन्तर्_Degrees) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_polar_गन्तव्यम्_Cart32f hal_ni_polar_गन्तव्यम्_Cart32f
#परिभाषय cv_hal_polar_गन्तव्यम्_Cart64f hal_ni_polar_गन्तव्यम्_Cart64f
//! @endcond

/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_invSqrt Inverse square root calculation
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_invSqrt32f(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_invSqrt64f(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_invSqrt32f hal_ni_invSqrt32f
#परिभाषय cv_hal_invSqrt64f hal_ni_invSqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_sqrt Square root calculation
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_sqrt32f(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_sqrt64f(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_sqrt32f hal_ni_sqrt32f
#परिभाषय cv_hal_sqrt64f hal_ni_sqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_log Natural logarithm calculation
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_log32f(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_log64f(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_log32f hal_ni_log32f
#परिभाषय cv_hal_log64f hal_ni_log64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_exp Exponent calculation
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_exp32f(ध्रुव भिन्न* src, भिन्न* dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_exp64f(ध्रुव द्विगुण* src, द्विगुण* dst, पूर्णाङ्क len) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_exp32f hal_ni_exp32f
#परिभाषय cv_hal_exp64f hal_ni_exp64f
//! @endcond


/**
@brief Dummy structure storing DFT/DCT context

Users can convert this pointer to any type they want. Initialisation and destruction should be made in Init and Free function implementations correspondingly.
Example:
@code{.cpp}
int my_hal_dftInit2D(cvhalDFT **context, ...) {
    *context = static_cast<cvhalDFT*>(new MyFilterData());
    //... init
}

int my_hal_dftFree2D(cvhalDFT *context) {
    MyFilterData *c = static_cast<MyFilterData*>(context);
    delete c;
}
@endcode
 */
संरचना cvhalDFT {};

/**
@param context double pointer to context storing all necessary data
@param len transformed array length
@param count estimated transformation count
@param depth array type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, CV_HAL_DFT_SCALE, ...)
@param needBuffer pointer to boolean variable, if valid pointer provided, then variable value should be set to true to signal that additional memory buffer is needed for operations
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dftInit1D(cvhalDFT **context, पूर्णाङ्क len, पूर्णाङ्क गणनम्_2, पूर्णाङ्क गभीरता, पूर्णाङ्क flags, तार्किक *need_बफर) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src source data
@param dst destination data
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dft1D(cvhalDFT *context, ध्रुव uchar *src, uchar *dst) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dftFree1D(cvhalDFT *context) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

/**
@param src source data
@param dst destination data
@param depth depth of source
@param nf OcvDftOptions data
@param factors OcvDftOptions data
@param scale OcvDftOptions data
@param itab OcvDftOptions data
@param wave OcvDftOptions data
@param tab_size OcvDftOptions data
@param n OcvDftOptions data
@param isInverse OcvDftOptions data
@param noPermute OcvDftOptions data
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dft(ध्रुव uchar* src, uchar* dst, पूर्णाङ्क गभीरता, पूर्णाङ्क nf, पूर्णाङ्क *factors, द्विगुण scale, पूर्णाङ्क* itab, शून्य* wave,
                         पूर्णाङ्क tab_आकार, पूर्णाङ्क n, तार्किक isInverse, तार्किक noPermute) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_dftInit1D hal_ni_dftInit1D
#परिभाषय cv_hal_dft1D hal_ni_dft1D
#परिभाषय cv_hal_dftFree1D hal_ni_dftFree1D
#परिभाषय cv_hal_dft hal_ni_dft
//! @endcond

/**
@param context double pointer to context storing all necessary data
@param width image width
@param height image height
@param depth image type (CV_32F or CV_64F)
@param src_channels number of channels in input image
@param dst_channels number of channels in output image
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, ...)
@param nonzero_rows number of nonzero rows in image, can be used for optimization
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dftInit2D(cvhalDFT **context, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क गभीरता, पूर्णाङ्क src_channels, पूर्णाङ्क dst_channels, पूर्णाङ्क flags, पूर्णाङ्क nonzero_पङ्क्तिकाः) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dft2D(cvhalDFT *context, ध्रुव uchar *src_दत्त, size_t src_चरणः, uchar *dst_दत्त, size_t dst_चरणः) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dftFree2D(cvhalDFT *context) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_dftInit2D hal_ni_dftInit2D
#परिभाषय cv_hal_dft2D hal_ni_dft2D
#परिभाषय cv_hal_dftFree2D hal_ni_dftFree2D
//! @endcond

/**
@param context double pointer to context storing all necessary data
@param width image width
@param height image height
@param depth image type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, ...)
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dctInit2D(cvhalDFT **context, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क गभीरता, पूर्णाङ्क flags) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dct2D(cvhalDFT *context, ध्रुव uchar *src_दत्त, size_t src_चरणः, uchar *dst_दत्त, size_t dst_चरणः) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dctFree2D(cvhalDFT *context) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_dctInit2D hal_ni_dctInit2D
#परिभाषय cv_hal_dct2D hal_ni_dct2D
#परिभाषय cv_hal_dctFree2D hal_ni_dctFree2D
//! @endcond


/**
Performs \f$LU\f$ decomposition of square matrix \f$A=P*L*U\f$ (where \f$P\f$ is permutation matrix) and solves matrix equation \f$A*X=B\f$.
Function returns the \f$sign\f$ of permutation \f$P\f$ via parameter info.
@param src1 pointer to input matrix \f$A\f$ stored in row major order. After finish of work src1 contains at least \f$U\f$ part of \f$LU\f$
decomposition which is appropriate for determinant calculation: \f$det(A)=sign*\prod_{j=1}^{M}a_{jj}\f$.
@param src1_step number of bytes between two consequent rows of matrix \f$A\f$.
@param m size of square matrix \f$A\f$.
@param src2 pointer to \f$M\times N\f$ matrix \f$B\f$ which is the right-hand side of system \f$A*X=B\f$. \f$B\f$ stored in row major order.
If src2 is null pointer only \f$LU\f$ decomposition will be performed. After finish of work src2 contains solution \f$X\f$ of system \f$A*X=B\f$.
@param src2_step number of bytes between two consequent rows of matrix \f$B\f$.
@param n number of right-hand vectors in \f$M\times N\f$ matrix \f$B\f$.
@param info indicates success of decomposition. If *info is equals to zero decomposition failed, otherwise *info is equals to \f$sign\f$.
 */
//! @addtogroup core_hal_interface_decomp_lu LU matrix decomposition
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_LU32f(भिन्न* src1, size_t src1_चरणः, पूर्णाङ्क m, भिन्न* src2, size_t src2_चरणः, पूर्णाङ्क n, पूर्णाङ्क* info) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_LU64f(द्विगुण* src1, size_t src1_चरणः, पूर्णाङ्क m, द्विगुण* src2, size_t src2_चरणः, पूर्णाङ्क n, पूर्णाङ्क* info) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
Performs Cholesky decomposition of matrix \f$A = L*L^T\f$ and solves matrix equation \f$A*X=B\f$.
@param src1 pointer to input matrix \f$A\f$ stored in row major order. After finish of work src1 contains lower triangular matrix \f$L\f$.
@param src1_step number of bytes between two consequent rows of matrix \f$A\f$.
@param m size of square matrix \f$A\f$.
@param src2 pointer to \f$M\times N\f$ matrix \f$B\f$ which is the right-hand side of system \f$A*X=B\f$. B stored in row major order.
If src2 is null pointer only Cholesky decomposition will be performed. After finish of work src2 contains solution \f$X\f$ of system \f$A*X=B\f$.
@param src2_step number of bytes between two consequent rows of matrix \f$B\f$.
@param n number of right-hand vectors in \f$M\times N\f$ matrix \f$B\f$.
@param info indicates success of decomposition. If *info is false decomposition failed.
 */

//! @addtogroup core_hal_interface_decomp_cholesky Cholesky matrix decomposition
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_Cholesky32f(भिन्न* src1, size_t src1_चरणः, पूर्णाङ्क m, भिन्न* src2, size_t src2_चरणः, पूर्णाङ्क n, तार्किक* info) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_Cholesky64f(द्विगुण* src1, size_t src1_चरणः, पूर्णाङ्क m, द्विगुण* src2, size_t src2_चरणः, पूर्णाङ्क n, तार्किक* info) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
Performs singular value decomposition of \f$M\times N\f$(\f$M>N\f$) matrix \f$A = U*\Sigma*V^T\f$.
@param src pointer to input \f$M\times N\f$ matrix \f$A\f$ stored in column major order.
After finish of work src will be filled with rows of \f$U\f$ or not modified (depends of flag CV_HAL_SVD_MODIFY_A).
@param src_step number of bytes between two consequent columns of matrix \f$A\f$.
@param w pointer to array for singular values of matrix \f$A\f$ (i. e. first \f$N\f$ diagonal elements of matrix \f$\Sigma\f$).
@param u pointer to output \f$M\times N\f$ or \f$M\times M\f$ matrix \f$U\f$ (size depends of flags). Pointer must be valid if flag CV_HAL_SVD_MODIFY_A not used.
@param u_step number of bytes between two consequent rows of matrix \f$U\f$.
@param vt pointer to array for \f$N\times N\f$ matrix \f$V^T\f$.
@param vt_step number of bytes between two consequent rows of matrix \f$V^T\f$.
@param m number fo rows in matrix \f$A\f$.
@param n number of columns in matrix \f$A\f$.
@param flags algorithm options (combination of CV_HAL_SVD_FULL_UV, ...).
 */
//! @addtogroup core_hal_interface_decomp_svd Singular value matrix decomposition
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_SVD32f(भिन्न* src, size_t src_चरणः, भिन्न* w, भिन्न* u, size_t u_चरणः, भिन्न* vt, size_t vt_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_SVD64f(द्विगुण* src, size_t src_चरणः, द्विगुण* w, द्विगुण* u, size_t u_चरणः, द्विगुण* vt, size_t vt_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क flags) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

/**
Performs QR decomposition of \f$M\times N\f$(\f$M>N\f$) matrix \f$A = Q*R\f$ and solves matrix equation \f$A*X=B\f$.
@param src1 pointer to input matrix \f$A\f$ stored in row major order. After finish of work src1 contains upper triangular \f$N\times N\f$ matrix \f$R\f$.
Lower triangle of src1 will be filled with vectors of elementary reflectors. See @cite VandLec and Lapack's DGEQRF documentation for details.
@param src1_step number of bytes between two consequent rows of matrix \f$A\f$.
@param m number fo rows in matrix \f$A\f$.
@param n number of columns in matrix \f$A\f$.
@param k number of right-hand vectors in \f$M\times K\f$ matrix \f$B\f$.
@param src2 pointer to \f$M\times K\f$ matrix \f$B\f$ which is the right-hand side of system \f$A*X=B\f$. \f$B\f$ stored in row major order.
If src2 is null pointer only QR decomposition will be performed. Otherwise system will be solved and src1 will be used as temporary buffer, so
after finish of work src2 contains solution \f$X\f$ of system \f$A*X=B\f$.
@param src2_step number of bytes between two consequent rows of matrix \f$B\f$.
@param dst pointer to continiuos \f$N\times 1\f$ array for scalar factors of elementary reflectors. See @cite VandLec for details.
@param info indicates success of decomposition. If *info is zero decomposition failed.
*/
//! @addtogroup core_hal_interface_decomp_qr QR matrix decomposition
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_QR32f(भिन्न* src1, size_t src1_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, भिन्न* src2, size_t src2_चरणः, भिन्न* dst, पूर्णाङ्क* info) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_QR64f(द्विगुण* src1, size_t src1_चरणः, पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, द्विगुण* src2, size_t src2_चरणः, द्विगुण* dst, पूर्णाङ्क* info) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}



//! @cond IGNORED
#परिभाषय cv_hal_LU32f hal_ni_LU32f
#परिभाषय cv_hal_LU64f hal_ni_LU64f
#परिभाषय cv_hal_Cholesky32f hal_ni_Cholesky32f
#परिभाषय cv_hal_Cholesky64f hal_ni_Cholesky64f
#परिभाषय cv_hal_SVD32f hal_ni_SVD32f
#परिभाषय cv_hal_SVD64f hal_ni_SVD64f
#परिभाषय cv_hal_QR32f hal_ni_QR32f
#परिभाषय cv_hal_QR64f hal_ni_QR64f
//! @endcond


/**
The function performs generalized matrix multiplication similar to the gemm functions in BLAS level 3:
\f$D = \alpha*AB+\beta*C\f$

@param src1 pointer to input \f$M\times N\f$ matrix \f$A\f$ or \f$A^T\f$ stored in row major order.
@param src1_step number of bytes between two consequent rows of matrix \f$A\f$ or \f$A^T\f$.
@param src2 pointer to input \f$N\times K\f$ matrix \f$B\f$ or \f$B^T\f$ stored in row major order.
@param src2_step number of bytes between two consequent rows of matrix \f$B\f$ or \f$B^T\f$.
@param alpha \f$\alpha\f$ multiplier before \f$AB\f$
@param src3 pointer to input \f$M\times K\f$ matrix \f$C\f$ or \f$C^T\f$ stored in row major order.
@param src3_step number of bytes between two consequent rows of matrix \f$C\f$ or \f$C^T\f$.
@param beta \f$\beta\f$ multiplier before \f$C\f$
@param dst pointer to input \f$M\times K\f$ matrix \f$D\f$ stored in row major order.
@param dst_step number of bytes between two consequent rows of matrix \f$D\f$.
@param m number of rows in matrix \f$A\f$ or \f$A^T\f$, equals to number of rows in matrix \f$D\f$
@param n number of columns in matrix \f$A\f$ or \f$A^T\f$
@param k number of columns in matrix \f$B\f$ or \f$B^T\f$, equals to number of columns in matrix \f$D\f$
@param flags algorithm options (combination of CV_HAL_GEMM_1_T, ...).
 */

//! @addtogroup core_hal_interface_matrix_multiplication Matrix multiplication
//! @{
अन्तःस्थ पूर्णाङ्क hal_ni_gemm32f(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
                          भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
                          पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_gemm64f(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
                          द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
                          पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_gemm32fc(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
                          भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
                          पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
अन्तःस्थ पूर्णाङ्क hal_ni_gemm64fc(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
                          द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
                          पूर्णाङ्क m, पूर्णाङ्क n, पूर्णाङ्क k, पूर्णाङ्क flags) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#परिभाषय cv_hal_gemm32f hal_ni_gemm32f
#परिभाषय cv_hal_gemm64f hal_ni_gemm64f
#परिभाषय cv_hal_gemm32fc hal_ni_gemm32fc
#परिभाषय cv_hal_gemm64fc hal_ni_gemm64fc
//! @endcond

/**
   @brief Finds the global minimum and maximum in an array.
   @param src_data Source image
   @param src_step Source image
   @param width Source image dimensions
   @param height Source image dimensions
   @param depth Depth of source image
   @param minVal Pointer to the returned global minimum and maximum in an array.
   @param maxVal Pointer to the returned global minimum and maximum in an array.
   @param minIdx Pointer to the returned minimum and maximum location.
   @param maxIdx Pointer to the returned minimum and maximum location.
   @param mask Specified array region.
*/
अन्तःस्थ पूर्णाङ्क hal_ni_minMaxIdx(ध्रुव uchar* src_दत्त, size_t src_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क गभीरता, द्विगुण* minVal, द्विगुण* maxVal,
                            पूर्णाङ्क* minIdx, पूर्णाङ्क* maxIdx, uchar* आवरणम्) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief Finds the global minimum and maximum in an array.
   @param src_data Source image
   @param src_step Source image
   @param width Source image dimensions
   @param height Source image dimensions
   @param depth Depth of source image
   @param minVal Pointer to the returned global minimum and maximum in an array.
   @param maxVal Pointer to the returned global minimum and maximum in an array.
   @param minIdx Pointer to the returned minimum and maximum location.
   @param maxIdx Pointer to the returned minimum and maximum location.
   @param mask Specified array region.
   @param mask_step Mask array step.
*/
अन्तःस्थ पूर्णाङ्क hal_ni_min_Max_Idx_आवरणम्_चरणः(ध्रुव uchar* src_दत्त, size_t src_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, पूर्णाङ्क गभीरता, द्विगुण* minVal, द्विगुण* maxVal,
                                    पूर्णाङ्क* minIdx, पूर्णाङ्क* maxIdx, uchar* आवरणम्, size_t आवरणम्_चरणः) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_minMaxIdx hal_ni_minMaxIdx
#परिभाषय cv_hal_min_Max_Idx_आवरणम्_चरणः hal_ni_min_Max_Idx_आवरणम्_चरणः
//! @endcond

/**
   @brief calculates the mean and the standard deviation of array elements independently for each channel
   @param src_data Source image
   @param src_step Source image
   @param width Source image dimensions
   @param height Source image dimensions
   @param src_type Type of source image
   @param mean_val Array of per-channel mean values. May be nullptr, if mean value is not required.
   @param stddev_val Array of per-channel standard deviation values. May be nullptr, if stddev value is not required.
   @param mask Specified array region.
   @param mask_step Mask array step.
   @sa meanStdDev
*/
अन्तःस्थ पूर्णाङ्क hal_ni_माध्यम्_Std_Dev(ध्रुव uchar* src_दत्त, size_t src_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height,
                             पूर्णाङ्क src_type, द्विगुण* माध्यम्_val, द्विगुण* stddev_val, uchar* आवरणम्, size_t आवरणम्_चरणः)
{ प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_माध्यम्_Std_Dev hal_ni_माध्यम्_Std_Dev
//! @endcond

/**
 * @brief calculates dot product of two vectors (represented as 2d images)
 *
 * @param a_data Pointer to 1st 2nd image data
 * @param a_step Stride of 1st 2nd image
 * @param b_data Pointer to 1st 2nd image data
 * @param b_step Stride of 1st 2nd image
 * @param width Width of both images
 * @param height Height of both images
 * @param type Data type of both images, for example CV_8U or CV_32F
 * @param dot_val Pointer to resulting dot product value
 * @return int
 */
अन्तःस्थ पूर्णाङ्क hal_ni_dot_गुणनफलम्(ध्रुव uchar* a_दत्त, size_t a_चरणः, ध्रुव uchar* b_दत्त, size_t b_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height,
                             पूर्णाङ्क type, द्विगुण *dot_val)
{ प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_dot_गुणनफलम् hal_ni_dot_गुणनफलम्
//! @endcond

/**
   @brief hal_flip
   @param src_type source and destination image type
   @param src_data source image data
   @param src_step source image step
   @param src_width source and destination image width
   @param src_height source and destination image height
   @param dst_data destination image data
   @param dst_step destination image step
   @param flip_mode 0 flips around x-axis, positive around y-axis, negative both
 */
अन्तःस्थ पूर्णाङ्क hal_ni_flip(पूर्णाङ्क src_type, ध्रुव uchar* src_दत्त, size_t src_चरणः, पूर्णाङ्क src_विस्तारः, पूर्णाङ्क src_height,
                       uchar* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क flip_बहुलकम्) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_flip hal_ni_flip
//! @endcond

/**
   @brief rotate90
   @param src_type source and destination image type
   @param src_data source image data
   @param src_step source image step
   @param src_width source image width
   If angle has value [180] it is also destination image width
   If angle has values [90, 270] it is also destination image height
   @param src_height source and destination image height (destination image width for angles [90, 270])
   If angle has value [180] it is also destination image height
   If angle has values [90, 270] it is also destination image width
   @param dst_data destination image data
   @param dst_step destination image step
   @param angle clockwise angle for rotation in degrees from set [90, 180, 270]
 */
अन्तःस्थ पूर्णाङ्क hal_ni_rotate90(पूर्णाङ्क src_type, ध्रुव uchar* src_दत्त, size_t src_चरणः, पूर्णाङ्क src_विस्तारः, पूर्णाङ्क src_height,
                           uchar* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क angle) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_rotate90 hal_ni_rotate90
//! @endcond

/**
   @brief Transpose2d
   @param src_data,src_step Source image
   @param dst_data,dst_step Destination image
   @param src_width,src_height Source image dimensions
   @param element_size Size of an element in bytes
*/
अन्तःस्थ पूर्णाङ्क hal_ni_transpose2d(ध्रुव uchar* src_दत्त, size_t src_चरणः, uchar* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क src_विस्तारः,
                              पूर्णाङ्क src_height, पूर्णाङ्क element_आकार) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_transpose2d hal_ni_transpose2d
//! @endcond

/**
    @brief copyTo with mask
    @param src_data, src_step Source image
    @param dst_data, dst_step Destination image
    @param width, height Image dimensions of source, destination and mask
    @param type Type of source and destination images, for example CV_8UC1 or CV_32FC3
    @param mask_data, mask_step, mask_type Mask
*/
अन्तःस्थ पूर्णाङ्क hal_ni_copy_गन्तव्यम्_Masked(ध्रुव uchar* src_दत्त, size_t src_चरणः, uchar* dst_दत्त, size_t dst_चरणः, पूर्णाङ्क विस्तारः, पूर्णाङ्क height,
                             पूर्णाङ्क type, ध्रुव uchar* आवरणम्_दत्त, size_t आवरणम्_चरणः, पूर्णाङ्क आवरणम्_type)
{ प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_copy_गन्तव्यम्_Masked hal_ni_copy_गन्तव्यम्_Masked
//! @endcond

/**
 @ brief sum
 @param src_data Source image data
 @param src_step Source image step
 @param src_type Source image type
 @param width, height Source image dimensions
 @param result Pointer to save the sum result to.
 */
अन्तःस्थ पूर्णाङ्क hal_ni_योग(ध्रुव uchar *src_दत्त, size_t src_चरणः, पूर्णाङ्क src_type, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, द्विगुण *परिणाम)
{ प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_योग hal_ni_योग
//! @endcond

/**
   @brief inRange (lower_bound <= src_value) && (src_value <= upper_bound) ? 255 : 0
   @param src_data Source image data
   @param src_step Source image step
   @param dst_data Destination image data
   @param dst_step Destination image step
   @param dst_depth Destination image depth
   @param width Image width
   @param height Image height
   @param cn number of channels
   @param lower_bound Range lower bound
   @param upper_bound Range upper bound
*/
अन्तःस्थ पूर्णाङ्क hal_ni_अन्तर्_Range8u(ध्रुव uchar *src_दत्त, size_t src_चरणः,
              uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क dst_गभीरता_2, पूर्णाङ्क विस्तारः,
              पूर्णाङ्क height, पूर्णाङ्क cn, uchar lower_bound, uchar upper_bound) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief inRange (lower_bound <= src_value) && (src_value <= upper_bound) ? 255 : 0
   @param src_data Source image data
   @param src_step Source image step
   @param dst_data Destination image data
   @param dst_step Destination image step
   @param dst_depth Destination image depth
   @param width Image width
   @param height Image height
   @param cn number of channels
   @param lower_bound Range lower bound
   @param upper_bound Range upper bound
*/
अन्तःस्थ पूर्णाङ्क hal_ni_अन्तर्_Range32f(ध्रुव uchar *src_दत्त, size_t src_चरणः,
              uchar *dst_दत्त, size_t dst_चरणः, पूर्णाङ्क dst_गभीरता_2, पूर्णाङ्क विस्तारः,
              पूर्णाङ्क height, पूर्णाङ्क cn, द्विगुण lower_bound, द्विगुण upper_bound) { प्रत्यागम CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#परिभाषय cv_hal_अन्तर्_Range8u hal_ni_अन्तर्_Range8u
#परिभाषय cv_hal_अन्तर्_Range32f hal_ni_अन्तर्_Range32f
//! @endcond

//! @}

#यदि defined(__clang__)
#निर्देशः clang diagnostic pop
#नोचेद्यदि defined(_MSC_VER)
#निर्देशः warning(pop)
#नोचेद्यदि defined(__GNUC__)
#निर्देशः GCC diagnostic pop
#यदि_अन्तः

#अन्तर्भाव "hal_internal.hpp"
#अन्तर्भाव "custom_hal.hpp"

//! @cond IGNORED

#परिभाषय CALL_HAL_RET2(name, fun, retval, ...) \
{ \
    पूर्णाङ्क res = __CV_EXPAND(fun(__VA_ARGS__)); \
    यदि (res == CV_HAL_ERROR_OK) \
        प्रत्यागम retval; \
    अन्यथा यदि (res != CV_HAL_ERROR_NOT_IMPLEMENTED) \
        CV_Error_(cv::Error::Sts_आन्तरिकम्, \
        ("HAL implementation " CVAUX_STR(name) " ==> " CVAUX_STR(fun) " returned %d (0x%08x)", res, res)); \
}

#परिभाषय CALL_HAL_RET(name, fun, retval, ...) \
CALL_HAL_RET2(name, fun, retval, __VA_ARGS__, &retval)

#परिभाषय CALL_HAL(name, fun, ...) \
CALL_HAL_RET2(name, fun, ,__VA_ARGS__)

//! @endcond

#यदि_अन्तः
