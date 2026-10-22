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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_HAL_REPLACEMENT_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_HAL_REPLACEMENT_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/hal/interface.h"

#ਜੇ defined(__clang__)  // clang or MSVC clang
#ਹਦਾਇਤ clang diagnostic push
#ਹਦਾਇਤ clang diagnostic ignored "-Wunused-parameter"
#ਜਾਂਜੇ defined(_MSC_VER)
#ਹਦਾਇਤ warning(push)
#ਹਦਾਇਤ warning(disable : 4100)
#ਜਾਂਜੇ defined(__GNUC__)
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-parameter"
#ਜੇ_ਅੰਤ

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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_add16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub8u32f(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sub8s32f(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Scalar32f32f(ਅਚਲ ਦਸ਼ਮਲਵ*   src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਦਸ਼ਮਲਵ*   dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦਸ਼ਮਲਵ*   scalar_ਅੰਕੜੇ, ਪੂਰਨ nChannels) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Scalar16s16s(ਅਚਲ int16_t* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, int16_t* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ int16_t* scalar_ਅੰਕੜੇ, ਪੂਰਨ nChannels) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_max16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absdiff16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absDiffScalar32f32f(ਅਚਲ ਦਸ਼ਮਲਵ* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਦਸ਼ਮਲਵ*    dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦਸ਼ਮਲਵ* scalar_ਅੰਕੜੇ, ਪੂਰਨ nChannels) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absDiffScalar32s32u(ਅਚਲ ਪੂਰਨ*   src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uint32_t* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਪੂਰਨ*   scalar_ਅੰਕੜੇ, ਪੂਰਨ nChannels) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_absDiffScalar8u8u  (ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar*    dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ uchar* scalar_ਅੰਕੜੇ, ਪੂਰਨ nChannels) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_and8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_or8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_xor8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_not8u(ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add8u hal_ni_add8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add8s hal_ni_add8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add16u hal_ni_add16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add16s hal_ni_add16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add32u hal_ni_add32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add32s hal_ni_add32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add64u hal_ni_add64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add64s hal_ni_add64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add32f hal_ni_add32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add64f hal_ni_add64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add16f hal_ni_add16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_add16bf hal_ni_add16bf
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub8u hal_ni_sub8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub8s hal_ni_sub8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub16u hal_ni_sub16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub16s hal_ni_sub16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub32u hal_ni_sub32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub32s hal_ni_sub32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub64u hal_ni_sub64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub64s hal_ni_sub64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub32f hal_ni_sub32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub64f hal_ni_sub64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub8u32f hal_ni_sub8u32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub8s32f hal_ni_sub8s32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub16f hal_ni_sub16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sub16bf hal_ni_sub16bf
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Scalar32f32f hal_ni_ਜੋੜੋ_Scalar32f32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Scalar16s16s hal_ni_ਜੋੜੋ_Scalar16s16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max8u hal_ni_max8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max8s hal_ni_max8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max16u hal_ni_max16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max16s hal_ni_max16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max32u hal_ni_max32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max32s hal_ni_max32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max64u hal_ni_max64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max64s hal_ni_max64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max32f hal_ni_max32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max64f hal_ni_max64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max16f hal_ni_max16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_max16bf hal_ni_max16bf
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min8u hal_ni_min8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min8s hal_ni_min8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min16u hal_ni_min16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min16s hal_ni_min16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min32u hal_ni_min32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min32s hal_ni_min32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min64u hal_ni_min64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min64s hal_ni_min64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min32f hal_ni_min32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min64f hal_ni_min64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min16f hal_ni_min16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min16bf hal_ni_min16bf
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff8u hal_ni_absdiff8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff8s hal_ni_absdiff8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff16u hal_ni_absdiff16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff16s hal_ni_absdiff16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff32u hal_ni_absdiff32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff32s hal_ni_absdiff32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff64u hal_ni_absdiff64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff64s hal_ni_absdiff64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff32f hal_ni_absdiff32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff64f hal_ni_absdiff64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff16f hal_ni_absdiff16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absdiff16bf hal_ni_absdiff16bf
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absDiffScalar32f32f hal_ni_absDiffScalar32f32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absDiffScalar32s32u hal_ni_absDiffScalar32s32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_absDiffScalar8u8u   hal_ni_absDiffScalar8u8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_and8u hal_ni_and8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_or8u hal_ni_or8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_xor8u hal_ni_xor8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_not8u hal_ni_not8u
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_lut(ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, size_t src_type, ਅਚਲ uchar* lut_ਅੰਕੜੇ, size_t lut_channel_ਮਾਪ, size_t lut_channels, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_lut hal_ni_lut
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_lut16(ਅਚਲ ushort *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, size_t src_type, ਅਚਲ uchar* lut_ਅੰਕੜੇ, size_t lut_channel_ਮਾਪ, size_t lut_channels, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_lut16 hal_ni_lut16
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_normHamming8u(ਅਚਲ uchar* a, ਪੂਰਨ n, ਪੂਰਨ cell_ਮਾਪ, ਪੂਰਨ* ਨਤੀਜਾ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_normHammingDiff8u(ਅਚਲ uchar* a, ਅਚਲ uchar* b, ਪੂਰਨ n, ਪੂਰਨ cell_ਮਾਪ, ਪੂਰਨ* ਨਤੀਜਾ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_norm(ਅਚਲ uchar* src, size_t src_ਕਦਮ, ਅਚਲ uchar* ਨਕਾਬ, size_t ਨਕਾਬ_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ,
                       ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ type, ਪੂਰਨ norm_type, ਦੁੱਗਣਾ* ਨਤੀਜਾ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_normDiff(ਅਚਲ uchar* src1, size_t src1_ਕਦਮ, ਅਚਲ uchar* src2, size_t src2_ਕਦਮ, ਅਚਲ uchar* ਨਕਾਬ,
                           size_t ਨਕਾਬ_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ type, ਪੂਰਨ norm_type, ਦੁੱਗਣਾ* ਨਤੀਜਾ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_convertScale(ਅਚਲ uchar* src, size_t src_ਕਦਮ, uchar* dst, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                               ਪੂਰਨ sdepth, ਪੂਰਨ ddepth, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_normHamming8u hal_ni_normHamming8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_normHammingDiff8u hal_ni_normHammingDiff8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_norm hal_ni_norm
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_normDiff hal_ni_normDiff
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_convertScale hal_ni_convertScale
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cmp16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ operation) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp8u hal_ni_cmp8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp8s hal_ni_cmp8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp16u hal_ni_cmp16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp16s hal_ni_cmp16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp32u hal_ni_cmp32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp32s hal_ni_cmp32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp64u hal_ni_cmp64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp64s hal_ni_cmp64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp32f hal_ni_cmp32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp64f hal_ni_cmp64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp16f hal_ni_cmp16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cmp16bf hal_ni_cmp16bf
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul8u16u(ਅਚਲ uchar* src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar* src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul8s16s(ਅਚਲ schar* src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar* src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_mul16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_div16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip8u(ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip8s(ਅਚਲ schar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip16u(ਅਚਲ ushort *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip16s(ਅਚਲ ਛੋਟਾ *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip32s(ਅਚਲ ਪੂਰਨ *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip32f(ਅਚਲ ਦਸ਼ਮਲਵ *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip64f(ਅਚਲ ਦੁੱਗਣਾ *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip64u(ਅਚਲ uint64 *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip64s(ਅਚਲ int64 *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip16f(ਅਚਲ cv_hal_f16 *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_recip16bf(ਅਚਲ cv_hal_bf16 *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ scale) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul8u hal_ni_mul8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul8s hal_ni_mul8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul16u hal_ni_mul16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul16s hal_ni_mul16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul32u hal_ni_mul32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul32s hal_ni_mul32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul64u hal_ni_mul64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul64s hal_ni_mul64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul32f hal_ni_mul32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul64f hal_ni_mul64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul8u16u hal_ni_mul8u16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul8s16s hal_ni_mul8s16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul16f hal_ni_mul16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_mul16bf hal_ni_mul16bf
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div8u hal_ni_div8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div8s hal_ni_div8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div16u hal_ni_div16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div16s hal_ni_div16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div32u hal_ni_div32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div32s hal_ni_div32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div64u hal_ni_div64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div64s hal_ni_div64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div32f hal_ni_div32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div64f hal_ni_div64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div16f hal_ni_div16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_div16bf hal_ni_div16bf
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip8u hal_ni_recip8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip8s hal_ni_recip8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip16u hal_ni_recip16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip16s hal_ni_recip16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip32u hal_ni_recip32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip32s hal_ni_recip32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip64u hal_ni_recip64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip64s hal_ni_recip64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip32f hal_ni_recip32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip64f hal_ni_recip64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip16f hal_ni_recip16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_recip16bf hal_ni_recip16bf
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted8u(ਅਚਲ uchar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uchar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted8s(ਅਚਲ schar *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ schar *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, schar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted16u(ਅਚਲ ushort *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ushort *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ushort *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted16s(ਅਚਲ ਛੋਟਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਛੋਟਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਛੋਟਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted32u(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਬਿਨਾਨਿਸ਼ਾਨੀ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted32s(ਅਚਲ ਪੂਰਨ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਪੂਰਨ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਪੂਰਨ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted32f(ਅਚਲ ਦਸ਼ਮਲਵ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted64f(ਅਚਲ ਦੁੱਗਣਾ *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted64u(ਅਚਲ uint64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ uint64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, uint64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted64s(ਅਚਲ int64 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ int64 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, int64 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted16f(ਅਚਲ cv_hal_f16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_f16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_f16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜੋ_Weighted16bf(ਅਚਲ cv_hal_bf16 *src1_ਅੰਕੜੇ, size_t src1_ਕਦਮ, ਅਚਲ cv_hal_bf16 *src2_ਅੰਕੜੇ, size_t src2_ਕਦਮ, cv_hal_bf16 *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ scalars[3]) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted8u hal_ni_ਜੋੜੋ_Weighted8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted8s hal_ni_ਜੋੜੋ_Weighted8s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted16u hal_ni_ਜੋੜੋ_Weighted16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted16s hal_ni_ਜੋੜੋ_Weighted16s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted32u hal_ni_ਜੋੜੋ_Weighted32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted32s hal_ni_ਜੋੜੋ_Weighted32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted64u hal_ni_ਜੋੜੋ_Weighted64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted64s hal_ni_ਜੋੜੋ_Weighted64s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted32f hal_ni_ਜੋੜੋ_Weighted32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted64f hal_ni_ਜੋੜੋ_Weighted64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted16f hal_ni_ਜੋੜੋ_Weighted16f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜੋ_Weighted16bf hal_ni_ਜੋੜੋ_Weighted16bf
//! @endcond

/**
@param src_data array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param dst_data array of pointers to destination arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_split Channel split
//! @{
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_split8u(ਅਚਲ uchar *src_ਅੰਕੜੇ, uchar **dst_ਅੰਕੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_split16u(ਅਚਲ ushort *src_ਅੰਕੜੇ, ushort **dst_ਅੰਕੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_split32s(ਅਚਲ ਪੂਰਨ *src_ਅੰਕੜੇ, ਪੂਰਨ **dst_ਅੰਕੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_split64s(ਅਚਲ int64 *src_ਅੰਕੜੇ, int64 **dst_ਅੰਕੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_split8u hal_ni_split8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_split16u hal_ni_split16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_split32s hal_ni_split32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_split64s hal_ni_split64s
//! @endcond

/**
@param src_data array of pointers to source arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param dst_data destination array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_merge Channel merge
//! @{
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_merge8u(ਅਚਲ uchar **src_ਅੰਕੜੇ, uchar *dst_ਅੰਕੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_merge16u(ਅਚਲ ushort **src_ਅੰਕੜੇ, ushort *dst_ਅੰਕੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_merge32s(ਅਚਲ ਪੂਰਨ **src_ਅੰਕੜੇ, ਪੂਰਨ *dst_ਅੰਕੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_merge64s(ਅਚਲ int64 **src_ਅੰਕੜੇ, int64 *dst_ਅੰਕੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_merge8u hal_ni_merge8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_merge16u hal_ni_merge16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_merge32s hal_ni_merge32s
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_merge64s hal_ni_merge64s
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cart_ਮੰਜ਼ਿਲ_Polar32f(ਅਚਲ ਦਸ਼ਮਲਵ* x, ਅਚਲ ਦਸ਼ਮਲਵ* y, ਦਸ਼ਮਲਵ* mag, ਦਸ਼ਮਲਵ* angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_cart_ਮੰਜ਼ਿਲ_Polar64f(ਅਚਲ ਦੁੱਗਣਾ* x, ਅਚਲ ਦੁੱਗਣਾ* y, ਦੁੱਗਣਾ* mag, ਦੁੱਗਣਾ* angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cart_ਮੰਜ਼ਿਲ_Polar32f hal_ni_cart_ਮੰਜ਼ਿਲ_Polar32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_cart_ਮੰਜ਼ਿਲ_Polar64f hal_ni_cart_ਮੰਜ਼ਿਲ_Polar64f
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_fastAtan32f(ਅਚਲ ਦਸ਼ਮਲਵ* y, ਅਚਲ ਦਸ਼ਮਲਵ* x, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_fastAtan64f(ਅਚਲ ਦੁੱਗਣਾ* y, ਅਚਲ ਦੁੱਗਣਾ* x, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_fastAtan32f hal_ni_fastAtan32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_fastAtan64f hal_ni_fastAtan64f
//! @endcond


/**
@param x source X array
@param y source Y array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_magnitude Magnitude calculation
//! @{
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_magnitude32f(ਅਚਲ ਦਸ਼ਮਲਵ *x, ਅਚਲ ਦਸ਼ਮਲਵ *y, ਦਸ਼ਮਲਵ *dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_magnitude64f(ਅਚਲ ਦੁੱਗਣਾ *x, ਅਚਲ ਦੁੱਗਣਾ  *y, ਦੁੱਗਣਾ *dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_magnitude32f hal_ni_magnitude32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_magnitude64f hal_ni_magnitude64f
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_polar_ਮੰਜ਼ਿਲ_Cart32f(ਅਚਲ ਦਸ਼ਮਲਵ* mag, ਅਚਲ ਦਸ਼ਮਲਵ* angle, ਦਸ਼ਮਲਵ* x, ਦਸ਼ਮਲਵ* y, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_polar_ਮੰਜ਼ਿਲ_Cart64f(ਅਚਲ ਦੁੱਗਣਾ* mag, ਅਚਲ ਦੁੱਗਣਾ* angle, ਦੁੱਗਣਾ* x, ਦੁੱਗਣਾ* y, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_polar_ਮੰਜ਼ਿਲ_Cart32f hal_ni_polar_ਮੰਜ਼ਿਲ_Cart32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_polar_ਮੰਜ਼ਿਲ_Cart64f hal_ni_polar_ਮੰਜ਼ਿਲ_Cart64f
//! @endcond

/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_invSqrt Inverse square root calculation
//! @{
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_invSqrt32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_invSqrt64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_invSqrt32f hal_ni_invSqrt32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_invSqrt64f hal_ni_invSqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_sqrt Square root calculation
//! @{
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sqrt32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_sqrt64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sqrt32f hal_ni_sqrt32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_sqrt64f hal_ni_sqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_log Natural logarithm calculation
//! @{
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_log32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_log64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_log32f hal_ni_log32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_log64f hal_ni_log64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_exp Exponent calculation
//! @{
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_exp32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_exp64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_exp32f hal_ni_exp32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_exp64f hal_ni_exp64f
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
ਬਣਤਰ cvhalDFT {};

/**
@param context double pointer to context storing all necessary data
@param len transformed array length
@param count estimated transformation count
@param depth array type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, CV_HAL_DFT_SCALE, ...)
@param needBuffer pointer to boolean variable, if valid pointer provided, then variable value should be set to true to signal that additional memory buffer is needed for operations
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dftInit1D(cvhalDFT **context, ਪੂਰਨ len, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ flags, ਤਾਰਕਿਕ *need_ਬਫਰ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src source data
@param dst destination data
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dft1D(cvhalDFT *context, ਅਚਲ uchar *src, uchar *dst) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dftFree1D(cvhalDFT *context) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dft(ਅਚਲ uchar* src, uchar* dst, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ nf, ਪੂਰਨ *factors, ਦੁੱਗਣਾ scale, ਪੂਰਨ* itab, ਖਾਲੀ* wave,
                         ਪੂਰਨ tab_ਮਾਪ, ਪੂਰਨ n, ਤਾਰਕਿਕ isInverse, ਤਾਰਕਿਕ no_ਕ੍ਰਮ_ਬਦਲੋ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dftInit1D hal_ni_dftInit1D
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dft1D hal_ni_dft1D
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dftFree1D hal_ni_dftFree1D
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dft hal_ni_dft
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dftInit2D(cvhalDFT **context, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ src_channels, ਪੂਰਨ dst_channels, ਪੂਰਨ flags, ਪੂਰਨ nonzero_ਪੰਕਤੀਆਂ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dft2D(cvhalDFT *context, ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dftFree2D(cvhalDFT *context) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dftInit2D hal_ni_dftInit2D
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dft2D hal_ni_dft2D
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dftFree2D hal_ni_dftFree2D
//! @endcond

/**
@param context double pointer to context storing all necessary data
@param width image width
@param height image height
@param depth image type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, ...)
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dctInit2D(cvhalDFT **context, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ flags) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dct2D(cvhalDFT *context, ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dctFree2D(cvhalDFT *context) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dctInit2D hal_ni_dctInit2D
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dct2D hal_ni_dct2D
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dctFree2D hal_ni_dctFree2D
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_LU32f(ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ, ਪੂਰਨ n, ਪੂਰਨ* info) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_LU64f(ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ, ਪੂਰਨ n, ਪੂਰਨ* info) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_Cholesky32f(ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ, ਪੂਰਨ n, ਤਾਰਕਿਕ* info) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_Cholesky64f(ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ, ਪੂਰਨ n, ਤਾਰਕਿਕ* info) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_SVD32f(ਦਸ਼ਮਲਵ* src, size_t src_ਕਦਮ, ਦਸ਼ਮਲਵ* w, ਦਸ਼ਮਲਵ* u, size_t u_ਕਦਮ, ਦਸ਼ਮਲਵ* vt, size_t vt_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ flags) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_SVD64f(ਦੁੱਗਣਾ* src, size_t src_ਕਦਮ, ਦੁੱਗਣਾ* w, ਦੁੱਗਣਾ* u, size_t u_ਕਦਮ, ਦੁੱਗਣਾ* vt, size_t vt_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ flags) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_QR32f(ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ* info) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_QR64f(ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ, ਦੁੱਗਣਾ* dst, ਪੂਰਨ* info) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}



//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_LU32f hal_ni_LU32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_LU64f hal_ni_LU64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_Cholesky32f hal_ni_Cholesky32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_Cholesky64f hal_ni_Cholesky64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_SVD32f hal_ni_SVD32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_SVD64f hal_ni_SVD64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_QR32f hal_ni_QR32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_QR64f hal_ni_QR64f
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_gemm32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
                          ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
                          ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_gemm64f(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
                          ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
                          ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_gemm32fc(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
                          ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
                          ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_gemm64fc(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
                          ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
                          ਪੂਰਨ m, ਪੂਰਨ n, ਪੂਰਨ k, ਪੂਰਨ flags) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_gemm32f hal_ni_gemm32f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_gemm64f hal_ni_gemm64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_gemm32fc hal_ni_gemm32fc
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_gemm64fc hal_ni_gemm64fc
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_minMaxIdx(ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਡੂੰਘਾਈ, ਦੁੱਗਣਾ* minVal, ਦੁੱਗਣਾ* maxVal,
                            ਪੂਰਨ* minIdx, ਪੂਰਨ* maxIdx, uchar* ਨਕਾਬ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_min_Max_Idx_ਨਕਾਬ_ਕਦਮ(ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ ਡੂੰਘਾਈ, ਦੁੱਗਣਾ* minVal, ਦੁੱਗਣਾ* maxVal,
                                    ਪੂਰਨ* minIdx, ਪੂਰਨ* maxIdx, uchar* ਨਕਾਬ, size_t ਨਕਾਬ_ਕਦਮ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_minMaxIdx hal_ni_minMaxIdx
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_min_Max_Idx_ਨਕਾਬ_ਕਦਮ hal_ni_min_Max_Idx_ਨਕਾਬ_ਕਦਮ
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਔਸਤ_Std_Dev(ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                             ਪੂਰਨ src_type, ਦੁੱਗਣਾ* ਔਸਤ_val, ਦੁੱਗਣਾ* stddev_val, uchar* ਨਕਾਬ, size_t ਨਕਾਬ_ਕਦਮ)
{ ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਔਸਤ_Std_Dev hal_ni_ਔਸਤ_Std_Dev
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_dot_ਗੁਣਨਫਲ(ਅਚਲ uchar* a_ਅੰਕੜੇ, size_t a_ਕਦਮ, ਅਚਲ uchar* b_ਅੰਕੜੇ, size_t b_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                             ਪੂਰਨ type, ਦੁੱਗਣਾ *dot_val)
{ ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_dot_ਗੁਣਨਫਲ hal_ni_dot_ਗੁਣਨਫਲ
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_flip(ਪੂਰਨ src_type, ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਪੂਰਨ src_ਚੌੜਾਈ, ਪੂਰਨ src_ਉਚਾਈ,
                       uchar* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ flip_ਬਹੁਲਕ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_flip hal_ni_flip
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_rotate90(ਪੂਰਨ src_type, ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਪੂਰਨ src_ਚੌੜਾਈ, ਪੂਰਨ src_ਉਚਾਈ,
                           uchar* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ angle) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_rotate90 hal_ni_rotate90
//! @endcond

/**
   @brief Transpose2d
   @param src_data,src_step Source image
   @param dst_data,dst_step Destination image
   @param src_width,src_height Source image dimensions
   @param element_size Size of an element in bytes
*/
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_transpose2d(ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ src_ਚੌੜਾਈ,
                              ਪੂਰਨ src_ਉਚਾਈ, ਪੂਰਨ element_ਮਾਪ) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_transpose2d hal_ni_transpose2d
//! @endcond

/**
    @brief copyTo with mask
    @param src_data, src_step Source image
    @param dst_data, dst_step Destination image
    @param width, height Image dimensions of source, destination and mask
    @param type Type of source and destination images, for example CV_8UC1 or CV_32FC3
    @param mask_data, mask_step, mask_type Mask
*/
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_copy_ਮੰਜ਼ਿਲ_Masked(ਅਚਲ uchar* src_ਅੰਕੜੇ, size_t src_ਕਦਮ, uchar* dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                             ਪੂਰਨ type, ਅਚਲ uchar* ਨਕਾਬ_ਅੰਕੜੇ, size_t ਨਕਾਬ_ਕਦਮ, ਪੂਰਨ ਨਕਾਬ_type)
{ ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_copy_ਮੰਜ਼ਿਲ_Masked hal_ni_copy_ਮੰਜ਼ਿਲ_Masked
//! @endcond

/**
 @ brief sum
 @param src_data Source image data
 @param src_step Source image step
 @param src_type Source image type
 @param width, height Source image dimensions
 @param result Pointer to save the sum result to.
 */
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਜੋੜ(ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ, ਪੂਰਨ src_type, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਦੁੱਗਣਾ *ਨਤੀਜਾ)
{ ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਜੋੜ hal_ni_ਜੋੜ
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਵਿੱਚ_Range8u(ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ,
              uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ dst_ਡੂੰਘਾਈ_2, ਪੂਰਨ ਚੌੜਾਈ,
              ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ cn, uchar lower_bound, uchar upper_bound) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
ਸਤਰਅੰਦਰ ਪੂਰਨ hal_ni_ਵਿੱਚ_Range32f(ਅਚਲ uchar *src_ਅੰਕੜੇ, size_t src_ਕਦਮ,
              uchar *dst_ਅੰਕੜੇ, size_t dst_ਕਦਮ, ਪੂਰਨ dst_ਡੂੰਘਾਈ_2, ਪੂਰਨ ਚੌੜਾਈ,
              ਪੂਰਨ ਉਚਾਈ, ਪੂਰਨ cn, ਦੁੱਗਣਾ lower_bound, ਦੁੱਗਣਾ upper_bound) { ਵਾਪਸ CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਵਿੱਚ_Range8u hal_ni_ਵਿੱਚ_Range8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_hal_ਵਿੱਚ_Range32f hal_ni_ਵਿੱਚ_Range32f
//! @endcond

//! @}

#ਜੇ defined(__clang__)
#ਹਦਾਇਤ clang diagnostic pop
#ਜਾਂਜੇ defined(_MSC_VER)
#ਹਦਾਇਤ warning(pop)
#ਜਾਂਜੇ defined(__GNUC__)
#ਹਦਾਇਤ GCC diagnostic pop
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hal_internal.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "custom_hal.hpp"

//! @cond IGNORED

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CALL_HAL_RET2(name, fun, retval, ...) \
{ \
    ਪੂਰਨ res = __CV_EXPAND(fun(__VA_ARGS__)); \
    ਜੇ (res == CV_HAL_ERROR_OK) \
        ਵਾਪਸ retval; \
    ਨਹੀਂਤਾਂ ਜੇ (res != CV_HAL_ERROR_NOT_IMPLEMENTED) \
        CV_Error_(cv::Error::Sts_ਅੰਦਰੂਨੀ, \
        ("HAL implementation " CVAUX_STR(name) " ==> " CVAUX_STR(fun) " returned %d (0x%08x)", res, res)); \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CALL_HAL_RET(name, fun, retval, ...) \
CALL_HAL_RET2(name, fun, retval, __VA_ARGS__, &retval)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CALL_HAL(name, fun, ...) \
CALL_HAL_RET2(name, fun, ,__VA_ARGS__)

//! @endcond

#ਜੇ_ਅੰਤ
