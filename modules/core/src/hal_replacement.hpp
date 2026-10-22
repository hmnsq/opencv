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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_HAL_REPLACEMENT_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_HAL_REPLACEMENT_HPP

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/interface.h"

#ಆದರೆ defined(__clang__)  // clang or MSVC clang
#ಸೂಚನೆ clang diagnostic push
#ಸೂಚನೆ clang diagnostic ignored "-Wunused-parameter"
#ಅಥವಾಆದರೆ defined(_MSC_VER)
#ಸೂಚನೆ warning(push)
#ಸೂಚನೆ warning(disable : 4100)
#ಅಥವಾಆದರೆ defined(__GNUC__)
#ಸೂಚನೆ GCC diagnostic push
#ಸೂಚನೆ GCC diagnostic ignored "-Wunused-parameter"
#ಆದರೆ_ಅಂತ್ಯ

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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_add16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub8u32f(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sub8s32f(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Scalar32f32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*   src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ*   dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*   scalar_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ nChannels) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Scalar16s16s(ಸ್ಥಿರಾಂಕ int16_t* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, int16_t* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ int16_t* scalar_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ nChannels) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_max16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absdiff16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absDiffScalar32f32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ*    dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* scalar_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ nChannels) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absDiffScalar32s32u(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*   src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uint32_t* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*   scalar_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ nChannels) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_absDiffScalar8u8u  (ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar*    dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ uchar* scalar_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ nChannels) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_and8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_or8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_xor8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_not8u(ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add8u hal_ni_add8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add8s hal_ni_add8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add16u hal_ni_add16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add16s hal_ni_add16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add32u hal_ni_add32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add32s hal_ni_add32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add64u hal_ni_add64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add64s hal_ni_add64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add32f hal_ni_add32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add64f hal_ni_add64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add16f hal_ni_add16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_add16bf hal_ni_add16bf
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub8u hal_ni_sub8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub8s hal_ni_sub8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub16u hal_ni_sub16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub16s hal_ni_sub16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub32u hal_ni_sub32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub32s hal_ni_sub32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub64u hal_ni_sub64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub64s hal_ni_sub64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub32f hal_ni_sub32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub64f hal_ni_sub64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub8u32f hal_ni_sub8u32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub8s32f hal_ni_sub8s32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub16f hal_ni_sub16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sub16bf hal_ni_sub16bf
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Scalar32f32f hal_ni_ಕೂಡಿಸು_Scalar32f32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Scalar16s16s hal_ni_ಕೂಡಿಸು_Scalar16s16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max8u hal_ni_max8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max8s hal_ni_max8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max16u hal_ni_max16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max16s hal_ni_max16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max32u hal_ni_max32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max32s hal_ni_max32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max64u hal_ni_max64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max64s hal_ni_max64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max32f hal_ni_max32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max64f hal_ni_max64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max16f hal_ni_max16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_max16bf hal_ni_max16bf
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min8u hal_ni_min8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min8s hal_ni_min8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min16u hal_ni_min16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min16s hal_ni_min16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min32u hal_ni_min32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min32s hal_ni_min32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min64u hal_ni_min64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min64s hal_ni_min64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min32f hal_ni_min32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min64f hal_ni_min64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min16f hal_ni_min16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min16bf hal_ni_min16bf
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff8u hal_ni_absdiff8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff8s hal_ni_absdiff8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff16u hal_ni_absdiff16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff16s hal_ni_absdiff16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff32u hal_ni_absdiff32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff32s hal_ni_absdiff32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff64u hal_ni_absdiff64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff64s hal_ni_absdiff64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff32f hal_ni_absdiff32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff64f hal_ni_absdiff64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff16f hal_ni_absdiff16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absdiff16bf hal_ni_absdiff16bf
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absDiffScalar32f32f hal_ni_absDiffScalar32f32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absDiffScalar32s32u hal_ni_absDiffScalar32s32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_absDiffScalar8u8u   hal_ni_absDiffScalar8u8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_and8u hal_ni_and8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_or8u hal_ni_or8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_xor8u hal_ni_xor8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_not8u hal_ni_not8u
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_lut(ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, size_t src_type, ಸ್ಥಿರಾಂಕ uchar* lut_ದತ್ತಾಂಶ, size_t lut_channel_ಗಾತ್ರ, size_t lut_channels, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_lut hal_ni_lut
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_lut16(ಸ್ಥಿರಾಂಕ ushort *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, size_t src_type, ಸ್ಥಿರಾಂಕ uchar* lut_ದತ್ತಾಂಶ, size_t lut_channel_ಗಾತ್ರ, size_t lut_channels, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_lut16 hal_ni_lut16
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_normHamming8u(ಸ್ಥಿರಾಂಕ uchar* a, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ cell_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ* ಫಲಿತಾಂಶ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_normHammingDiff8u(ಸ್ಥಿರಾಂಕ uchar* a, ಸ್ಥಿರಾಂಕ uchar* b, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ cell_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ* ಫಲಿತಾಂಶ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_norm(ಸ್ಥಿರಾಂಕ uchar* src, size_t src_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, size_t ಮುಖವಾಡ_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ,
                       ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ norm_type, ದ್ವಿಗುಣ* ಫಲಿತಾಂಶ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_normDiff(ಸ್ಥಿರಾಂಕ uchar* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar* src2, size_t src2_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ,
                           size_t ಮುಖವಾಡ_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ norm_type, ದ್ವಿಗುಣ* ಫಲಿತಾಂಶ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_convertScale(ಸ್ಥಿರಾಂಕ uchar* src, size_t src_ಹೆಜ್ಜೆ, uchar* dst, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                               ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_normHamming8u hal_ni_normHamming8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_normHammingDiff8u hal_ni_normHammingDiff8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_norm hal_ni_norm
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_normDiff hal_ni_normDiff
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_convertScale hal_ni_convertScale
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cmp16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ operation) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp8u hal_ni_cmp8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp8s hal_ni_cmp8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp16u hal_ni_cmp16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp16s hal_ni_cmp16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp32u hal_ni_cmp32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp32s hal_ni_cmp32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp64u hal_ni_cmp64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp64s hal_ni_cmp64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp32f hal_ni_cmp32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp64f hal_ni_cmp64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp16f hal_ni_cmp16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cmp16bf hal_ni_cmp16bf
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul8u16u(ಸ್ಥಿರಾಂಕ uchar* src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar* src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul8s16s(ಸ್ಥಿರಾಂಕ schar* src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar* src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_mul16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_div16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip8u(ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip8s(ಸ್ಥಿರಾಂಕ schar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip16u(ಸ್ಥಿರಾಂಕ ushort *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip64u(ಸ್ಥಿರಾಂಕ uint64 *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip64s(ಸ್ಥಿರಾಂಕ int64 *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_recip16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ scale) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul8u hal_ni_mul8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul8s hal_ni_mul8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul16u hal_ni_mul16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul16s hal_ni_mul16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul32u hal_ni_mul32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul32s hal_ni_mul32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul64u hal_ni_mul64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul64s hal_ni_mul64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul32f hal_ni_mul32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul64f hal_ni_mul64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul8u16u hal_ni_mul8u16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul8s16s hal_ni_mul8s16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul16f hal_ni_mul16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_mul16bf hal_ni_mul16bf
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div8u hal_ni_div8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div8s hal_ni_div8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div16u hal_ni_div16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div16s hal_ni_div16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div32u hal_ni_div32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div32s hal_ni_div32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div64u hal_ni_div64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div64s hal_ni_div64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div32f hal_ni_div32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div64f hal_ni_div64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div16f hal_ni_div16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_div16bf hal_ni_div16bf
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip8u hal_ni_recip8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip8s hal_ni_recip8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip16u hal_ni_recip16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip16s hal_ni_recip16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip32u hal_ni_recip32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip32s hal_ni_recip32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip64u hal_ni_recip64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip64s hal_ni_recip64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip32f hal_ni_recip32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip64f hal_ni_recip64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip16f hal_ni_recip16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_recip16bf hal_ni_recip16bf
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted8u(ಸ್ಥಿರಾಂಕ uchar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted8s(ಸ್ಥಿರಾಂಕ schar *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ schar *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, schar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted16u(ಸ್ಥಿರಾಂಕ ushort *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ushort *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ushort *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted16s(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಗಿಡ್ಡ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted32u(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಚಿಹ್ನೆರಹಿತ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted64u(ಸ್ಥಿರಾಂಕ uint64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uint64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, uint64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted64s(ಸ್ಥಿರಾಂಕ int64 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ int64 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, int64 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted16f(ಸ್ಥಿರಾಂಕ cv_hal_f16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_f16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_f16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಕೂಡಿಸು_Weighted16bf(ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src1_ದತ್ತಾಂಶ, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ cv_hal_bf16 *src2_ದತ್ತಾಂಶ, size_t src2_ಹೆಜ್ಜೆ, cv_hal_bf16 *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ scalars[3]) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted8u hal_ni_ಕೂಡಿಸು_Weighted8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted8s hal_ni_ಕೂಡಿಸು_Weighted8s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted16u hal_ni_ಕೂಡಿಸು_Weighted16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted16s hal_ni_ಕೂಡಿಸು_Weighted16s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted32u hal_ni_ಕೂಡಿಸು_Weighted32u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted32s hal_ni_ಕೂಡಿಸು_Weighted32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted64u hal_ni_ಕೂಡಿಸು_Weighted64u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted64s hal_ni_ಕೂಡಿಸು_Weighted64s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted32f hal_ni_ಕೂಡಿಸು_Weighted32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted64f hal_ni_ಕೂಡಿಸು_Weighted64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted16f hal_ni_ಕೂಡಿಸು_Weighted16f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಕೂಡಿಸು_Weighted16bf hal_ni_ಕೂಡಿಸು_Weighted16bf
//! @endcond

/**
@param src_data array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param dst_data array of pointers to destination arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_split Channel split
//! @{
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_split8u(ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, uchar **dst_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_split16u(ಸ್ಥಿರಾಂಕ ushort *src_ದತ್ತಾಂಶ, ushort **dst_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_split32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *src_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ **dst_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_split64s(ಸ್ಥಿರಾಂಕ int64 *src_ದತ್ತಾಂಶ, int64 **dst_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_split8u hal_ni_split8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_split16u hal_ni_split16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_split32s hal_ni_split32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_split64s hal_ni_split64s
//! @endcond

/**
@param src_data array of pointers to source arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param dst_data destination array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_merge Channel merge
//! @{
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_merge8u(ಸ್ಥಿರಾಂಕ uchar **src_ದತ್ತಾಂಶ, uchar *dst_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_merge16u(ಸ್ಥಿರಾಂಕ ushort **src_ದತ್ತಾಂಶ, ushort *dst_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_merge32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ **src_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ *dst_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_merge64s(ಸ್ಥಿರಾಂಕ int64 **src_ದತ್ತಾಂಶ, int64 *dst_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_merge8u hal_ni_merge8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_merge16u hal_ni_merge16u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_merge32s hal_ni_merge32s
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_merge64s hal_ni_merge64s
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cart_ಗಮ್ಯ_Polar32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* x, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* y, ದಶಮಾಂಶ* mag, ದಶಮಾಂಶ* angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_cart_ಗಮ್ಯ_Polar64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* x, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ದ್ವಿಗುಣ* angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cart_ಗಮ್ಯ_Polar32f hal_ni_cart_ಗಮ್ಯ_Polar32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_cart_ಗಮ್ಯ_Polar64f hal_ni_cart_ಗಮ್ಯ_Polar64f
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_fastAtan32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* y, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* x, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_fastAtan64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* y, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* x, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_fastAtan32f hal_ni_fastAtan32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_fastAtan64f hal_ni_fastAtan64f
//! @endcond


/**
@param x source X array
@param y source Y array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_magnitude Magnitude calculation
//! @{
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_magnitude32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *x, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *y, ದಶಮಾಂಶ *dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_magnitude64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *x, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ  *y, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_magnitude32f hal_ni_magnitude32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_magnitude64f hal_ni_magnitude64f
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_polar_ಗಮ್ಯ_Cart32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* mag, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* angle, ದಶಮಾಂಶ* x, ದಶಮಾಂಶ* y, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_polar_ಗಮ್ಯ_Cart64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* mag, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* angle, ದ್ವಿಗುಣ* x, ದ್ವಿಗುಣ* y, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_polar_ಗಮ್ಯ_Cart32f hal_ni_polar_ಗಮ್ಯ_Cart32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_polar_ಗಮ್ಯ_Cart64f hal_ni_polar_ಗಮ್ಯ_Cart64f
//! @endcond

/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_invSqrt Inverse square root calculation
//! @{
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_invSqrt32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_invSqrt64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_invSqrt32f hal_ni_invSqrt32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_invSqrt64f hal_ni_invSqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_sqrt Square root calculation
//! @{
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sqrt32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_sqrt64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sqrt32f hal_ni_sqrt32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_sqrt64f hal_ni_sqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_log Natural logarithm calculation
//! @{
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_log32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_log64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_log32f hal_ni_log32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_log64f hal_ni_log64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_exp Exponent calculation
//! @{
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_exp32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_exp64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_exp32f hal_ni_exp32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_exp64f hal_ni_exp64f
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
ರಚನೆ cvhalDFT {};

/**
@param context double pointer to context storing all necessary data
@param len transformed array length
@param count estimated transformation count
@param depth array type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, CV_HAL_DFT_SCALE, ...)
@param needBuffer pointer to boolean variable, if valid pointer provided, then variable value should be set to true to signal that additional memory buffer is needed for operations
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dftInit1D(cvhalDFT **context, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ flags, ತಾರ್ಕಿಕ *need_ಬಫರ್) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src source data
@param dst destination data
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dft1D(cvhalDFT *context, ಸ್ಥಿರಾಂಕ uchar *src, uchar *dst) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dftFree1D(cvhalDFT *context) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dft(ಸ್ಥಿರಾಂಕ uchar* src, uchar* dst, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ nf, ಪೂರ್ಣಾಂಕ *factors, ದ್ವಿಗುಣ scale, ಪೂರ್ಣಾಂಕ* itab, ಶೂನ್ಯ* wave,
                         ಪೂರ್ಣಾಂಕ tab_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ n, ತಾರ್ಕಿಕ isInverse, ತಾರ್ಕಿಕ no_ಕ್ರಮಬದಲಾವಣೆ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dftInit1D hal_ni_dftInit1D
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dft1D hal_ni_dft1D
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dftFree1D hal_ni_dftFree1D
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dft hal_ni_dft
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dftInit2D(cvhalDFT **context, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ src_channels, ಪೂರ್ಣಾಂಕ dst_channels, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ nonzero_ಅಡ್ಡಸಾಲುಗಳು) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dft2D(cvhalDFT *context, ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dftFree2D(cvhalDFT *context) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dftInit2D hal_ni_dftInit2D
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dft2D hal_ni_dft2D
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dftFree2D hal_ni_dftFree2D
//! @endcond

/**
@param context double pointer to context storing all necessary data
@param width image width
@param height image height
@param depth image type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, ...)
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dctInit2D(cvhalDFT **context, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dct2D(cvhalDFT *context, ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dctFree2D(cvhalDFT *context) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dctInit2D hal_ni_dctInit2D
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dct2D hal_ni_dct2D
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dctFree2D hal_ni_dctFree2D
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_LU32f(ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ* info) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_LU64f(ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ* info) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_Cholesky32f(ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ತಾರ್ಕಿಕ* info) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_Cholesky64f(ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ n, ತಾರ್ಕಿಕ* info) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_SVD32f(ದಶಮಾಂಶ* src, size_t src_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ* w, ದಶಮಾಂಶ* u, size_t u_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ* vt, size_t vt_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_SVD64f(ದ್ವಿಗುಣ* src, size_t src_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ* w, ದ್ವಿಗುಣ* u, size_t u_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ* vt, size_t vt_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_QR32f(ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ* info) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_QR64f(ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ* info) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}



//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_LU32f hal_ni_LU32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_LU64f hal_ni_LU64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_Cholesky32f hal_ni_Cholesky32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_Cholesky64f hal_ni_Cholesky64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_SVD32f hal_ni_SVD32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_SVD64f hal_ni_SVD64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_QR32f hal_ni_QR32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_QR64f hal_ni_QR64f
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_gemm32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
                          ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
                          ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_gemm64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
                          ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
                          ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_gemm32fc(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
                          ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
                          ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_gemm64fc(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
                          ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
                          ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_gemm32f hal_ni_gemm32f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_gemm64f hal_ni_gemm64f
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_gemm32fc hal_ni_gemm32fc
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_gemm64fc hal_ni_gemm64fc
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_minMaxIdx(ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ ಆಳ, ದ್ವಿಗುಣ* minVal, ದ್ವಿಗುಣ* maxVal,
                            ಪೂರ್ಣಾಂಕ* minIdx, ಪೂರ್ಣಾಂಕ* maxIdx, uchar* ಮುಖವಾಡ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_min_Max_Idx_ಮುಖವಾಡ_ಹೆಜ್ಜೆ(ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ ಆಳ, ದ್ವಿಗುಣ* minVal, ದ್ವಿಗುಣ* maxVal,
                                    ಪೂರ್ಣಾಂಕ* minIdx, ಪೂರ್ಣಾಂಕ* maxIdx, uchar* ಮುಖವಾಡ, size_t ಮುಖವಾಡ_ಹೆಜ್ಜೆ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_minMaxIdx hal_ni_minMaxIdx
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_min_Max_Idx_ಮುಖವಾಡ_ಹೆಜ್ಜೆ hal_ni_min_Max_Idx_ಮುಖವಾಡ_ಹೆಜ್ಜೆ
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಸರಾಸರಿ_Std_Dev(ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                             ಪೂರ್ಣಾಂಕ src_type, ದ್ವಿಗುಣ* ಸರಾಸರಿ_val, ದ್ವಿಗುಣ* stddev_val, uchar* ಮುಖವಾಡ, size_t ಮುಖವಾಡ_ಹೆಜ್ಜೆ)
{ ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಸರಾಸರಿ_Std_Dev hal_ni_ಸರಾಸರಿ_Std_Dev
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_dot_ಗುಣಲಬ್ಧ(ಸ್ಥಿರಾಂಕ uchar* a_ದತ್ತಾಂಶ, size_t a_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ uchar* b_ದತ್ತಾಂಶ, size_t b_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                             ಪೂರ್ಣಾಂಕ type, ದ್ವಿಗುಣ *dot_val)
{ ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_dot_ಗುಣಲಬ್ಧ hal_ni_dot_ಗುಣಲಬ್ಧ
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_flip(ಪೂರ್ಣಾಂಕ src_type, ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ src_ಅಗಲ, ಪೂರ್ಣಾಂಕ src_ಎತ್ತರ,
                       uchar* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ flip_ಬಹುಲಕ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_flip hal_ni_flip
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_rotate90(ಪೂರ್ಣಾಂಕ src_type, ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ src_ಅಗಲ, ಪೂರ್ಣಾಂಕ src_ಎತ್ತರ,
                           uchar* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ angle) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_rotate90 hal_ni_rotate90
//! @endcond

/**
   @brief Transpose2d
   @param src_data,src_step Source image
   @param dst_data,dst_step Destination image
   @param src_width,src_height Source image dimensions
   @param element_size Size of an element in bytes
*/
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_transpose2d(ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ src_ಅಗಲ,
                              ಪೂರ್ಣಾಂಕ src_ಎತ್ತರ, ಪೂರ್ಣಾಂಕ element_ಗಾತ್ರ) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_transpose2d hal_ni_transpose2d
//! @endcond

/**
    @brief copyTo with mask
    @param src_data, src_step Source image
    @param dst_data, dst_step Destination image
    @param width, height Image dimensions of source, destination and mask
    @param type Type of source and destination images, for example CV_8UC1 or CV_32FC3
    @param mask_data, mask_step, mask_type Mask
*/
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_copy_ಗಮ್ಯ_Masked(ಸ್ಥಿರಾಂಕ uchar* src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, uchar* dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                             ಪೂರ್ಣಾಂಕ type, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ_ದತ್ತಾಂಶ, size_t ಮುಖವಾಡ_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ_type)
{ ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_copy_ಗಮ್ಯ_Masked hal_ni_copy_ಗಮ್ಯ_Masked
//! @endcond

/**
 @ brief sum
 @param src_data Source image data
 @param src_step Source image step
 @param src_type Source image type
 @param width, height Source image dimensions
 @param result Pointer to save the sum result to.
 */
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಮೊತ್ತ(ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ src_type, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ದ್ವಿಗುಣ *ಫಲಿತಾಂಶ)
{ ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಮೊತ್ತ hal_ni_ಮೊತ್ತ
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಒಳ_Range8u(ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ,
              uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ dst_ಆಳ_2, ಪೂರ್ಣಾಂಕ ಅಗಲ,
              ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ cn, uchar lower_bound, uchar upper_bound) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ hal_ni_ಒಳ_Range32f(ಸ್ಥಿರಾಂಕ uchar *src_ದತ್ತಾಂಶ, size_t src_ಹೆಜ್ಜೆ,
              uchar *dst_ದತ್ತಾಂಶ, size_t dst_ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ dst_ಆಳ_2, ಪೂರ್ಣಾಂಕ ಅಗಲ,
              ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಪೂರ್ಣಾಂಕ cn, ದ್ವಿಗುಣ lower_bound, ದ್ವಿಗುಣ upper_bound) { ಹಿಂತಿರುಗಿ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಒಳ_Range8u hal_ni_ಒಳ_Range8u
#ವ್ಯಾಖ್ಯಾನಿಸು cv_hal_ಒಳ_Range32f hal_ni_ಒಳ_Range32f
//! @endcond

//! @}

#ಆದರೆ defined(__clang__)
#ಸೂಚನೆ clang diagnostic pop
#ಅಥವಾಆದರೆ defined(_MSC_VER)
#ಸೂಚನೆ warning(pop)
#ಅಥವಾಆದರೆ defined(__GNUC__)
#ಸೂಚನೆ GCC diagnostic pop
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "hal_internal.hpp"
#ಅನ್ತರ್ಭಾವ "custom_hal.hpp"

//! @cond IGNORED

#ವ್ಯಾಖ್ಯಾನಿಸು CALL_HAL_RET2(name, fun, retval, ...) \
{ \
    ಪೂರ್ಣಾಂಕ res = __CV_EXPAND(fun(__VA_ARGS__)); \
    ಆದರೆ (res == CV_HAL_ERROR_OK) \
        ಹಿಂತಿರುಗಿ retval; \
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (res != CV_HAL_ERROR_NOT_IMPLEMENTED) \
        CV_Error_(cv::Error::Sts_ಆಂತರಿಕ, \
        ("HAL implementation " CVAUX_STR(name) " ==> " CVAUX_STR(fun) " returned %d (0x%08x)", res, res)); \
}

#ವ್ಯಾಖ್ಯಾನಿಸು CALL_HAL_RET(name, fun, retval, ...) \
CALL_HAL_RET2(name, fun, retval, __VA_ARGS__, &retval)

#ವ್ಯಾಖ್ಯಾನಿಸು CALL_HAL(name, fun, ...) \
CALL_HAL_RET2(name, fun, ,__VA_ARGS__)

//! @endcond

#ಆದರೆ_ಅಂತ್ಯ
