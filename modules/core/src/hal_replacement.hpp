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

#که_نه_تعریف_شوی OPENCV_CORE_HAL_REPLACEMENT_HPP
#تعریف_کړه OPENCV_CORE_HAL_REPLACEMENT_HPP

#ضمن "opencv2/core/hal/interface.h"

#إذا defined(__clang__)  // clang or MSVC clang
#لارښوونه clang diagnostic push
#لارښوونه clang diagnostic ignored "-Wunused-parameter"
#وإلاإذا defined(_MSC_VER)
#لارښوونه warning(push)
#لارښوونه warning(disable : 4100)
#وإلاإذا defined(__GNUC__)
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wunused-parameter"
#د_که_پای

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
مضمن صحيح hal_ni_add8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_add16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

مضمن صحيح hal_ni_sub8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

مضمن صحيح hal_ni_sub8u32f(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sub8s32f(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
مضمن صحيح hal_ni_جمع_کړه_Scalar32f32f(ثابت عائم*   src_معلومات, size_t src_ګام, عائم*   dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت عائم*   scalar_معلومات, صحيح nChannels) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Scalar16s16s(ثابت int16_t* src_معلومات, size_t src_ګام, int16_t* dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت int16_t* scalar_معلومات, صحيح nChannels) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_max8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_max16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

مضمن صحيح hal_ni_min8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_min16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_absdiff8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absdiff16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
مضمن صحيح hal_ni_absDiffScalar32f32f(ثابت عائم* src_معلومات, size_t src_ګام, عائم*    dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت عائم* scalar_معلومات, صحيح nChannels) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absDiffScalar32s32u(ثابت صحيح*   src_معلومات, size_t src_ګام, uint32_t* dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت صحيح*   scalar_معلومات, صحيح nChannels) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_absDiffScalar8u8u  (ثابت uchar* src_معلومات, size_t src_ګام, uchar*    dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت uchar* scalar_معلومات, صحيح nChannels) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
مضمن صحيح hal_ni_and8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_or8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_xor8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_not8u(ثابت uchar *src_معلومات, size_t src_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_add8u hal_ni_add8u
#تعریف_کړه cv_hal_add8s hal_ni_add8s
#تعریف_کړه cv_hal_add16u hal_ni_add16u
#تعریف_کړه cv_hal_add16s hal_ni_add16s
#تعریف_کړه cv_hal_add32u hal_ni_add32u
#تعریف_کړه cv_hal_add32s hal_ni_add32s
#تعریف_کړه cv_hal_add64u hal_ni_add64u
#تعریف_کړه cv_hal_add64s hal_ni_add64s
#تعریف_کړه cv_hal_add32f hal_ni_add32f
#تعریف_کړه cv_hal_add64f hal_ni_add64f
#تعریف_کړه cv_hal_add16f hal_ni_add16f
#تعریف_کړه cv_hal_add16bf hal_ni_add16bf
#تعریف_کړه cv_hal_sub8u hal_ni_sub8u
#تعریف_کړه cv_hal_sub8s hal_ni_sub8s
#تعریف_کړه cv_hal_sub16u hal_ni_sub16u
#تعریف_کړه cv_hal_sub16s hal_ni_sub16s
#تعریف_کړه cv_hal_sub32u hal_ni_sub32u
#تعریف_کړه cv_hal_sub32s hal_ni_sub32s
#تعریف_کړه cv_hal_sub64u hal_ni_sub64u
#تعریف_کړه cv_hal_sub64s hal_ni_sub64s
#تعریف_کړه cv_hal_sub32f hal_ni_sub32f
#تعریف_کړه cv_hal_sub64f hal_ni_sub64f
#تعریف_کړه cv_hal_sub8u32f hal_ni_sub8u32f
#تعریف_کړه cv_hal_sub8s32f hal_ni_sub8s32f
#تعریف_کړه cv_hal_sub16f hal_ni_sub16f
#تعریف_کړه cv_hal_sub16bf hal_ni_sub16bf
#تعریف_کړه cv_hal_جمع_کړه_Scalar32f32f hal_ni_جمع_کړه_Scalar32f32f
#تعریف_کړه cv_hal_جمع_کړه_Scalar16s16s hal_ni_جمع_کړه_Scalar16s16s
#تعریف_کړه cv_hal_max8u hal_ni_max8u
#تعریف_کړه cv_hal_max8s hal_ni_max8s
#تعریف_کړه cv_hal_max16u hal_ni_max16u
#تعریف_کړه cv_hal_max16s hal_ni_max16s
#تعریف_کړه cv_hal_max32u hal_ni_max32u
#تعریف_کړه cv_hal_max32s hal_ni_max32s
#تعریف_کړه cv_hal_max64u hal_ni_max64u
#تعریف_کړه cv_hal_max64s hal_ni_max64s
#تعریف_کړه cv_hal_max32f hal_ni_max32f
#تعریف_کړه cv_hal_max64f hal_ni_max64f
#تعریف_کړه cv_hal_max16f hal_ni_max16f
#تعریف_کړه cv_hal_max16bf hal_ni_max16bf
#تعریف_کړه cv_hal_min8u hal_ni_min8u
#تعریف_کړه cv_hal_min8s hal_ni_min8s
#تعریف_کړه cv_hal_min16u hal_ni_min16u
#تعریف_کړه cv_hal_min16s hal_ni_min16s
#تعریف_کړه cv_hal_min32u hal_ni_min32u
#تعریف_کړه cv_hal_min32s hal_ni_min32s
#تعریف_کړه cv_hal_min64u hal_ni_min64u
#تعریف_کړه cv_hal_min64s hal_ni_min64s
#تعریف_کړه cv_hal_min32f hal_ni_min32f
#تعریف_کړه cv_hal_min64f hal_ni_min64f
#تعریف_کړه cv_hal_min16f hal_ni_min16f
#تعریف_کړه cv_hal_min16bf hal_ni_min16bf
#تعریف_کړه cv_hal_absdiff8u hal_ni_absdiff8u
#تعریف_کړه cv_hal_absdiff8s hal_ni_absdiff8s
#تعریف_کړه cv_hal_absdiff16u hal_ni_absdiff16u
#تعریف_کړه cv_hal_absdiff16s hal_ni_absdiff16s
#تعریف_کړه cv_hal_absdiff32u hal_ni_absdiff32u
#تعریف_کړه cv_hal_absdiff32s hal_ni_absdiff32s
#تعریف_کړه cv_hal_absdiff64u hal_ni_absdiff64u
#تعریف_کړه cv_hal_absdiff64s hal_ni_absdiff64s
#تعریف_کړه cv_hal_absdiff32f hal_ni_absdiff32f
#تعریف_کړه cv_hal_absdiff64f hal_ni_absdiff64f
#تعریف_کړه cv_hal_absdiff16f hal_ni_absdiff16f
#تعریف_کړه cv_hal_absdiff16bf hal_ni_absdiff16bf
#تعریف_کړه cv_hal_absDiffScalar32f32f hal_ni_absDiffScalar32f32f
#تعریف_کړه cv_hal_absDiffScalar32s32u hal_ni_absDiffScalar32s32u
#تعریف_کړه cv_hal_absDiffScalar8u8u   hal_ni_absDiffScalar8u8u
#تعریف_کړه cv_hal_and8u hal_ni_and8u
#تعریف_کړه cv_hal_or8u hal_ni_or8u
#تعریف_کړه cv_hal_xor8u hal_ni_xor8u
#تعریف_کړه cv_hal_not8u hal_ni_not8u
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
مضمن صحيح hal_ni_lut(ثابت uchar *src_معلومات, size_t src_ګام, size_t src_type, ثابت uchar* lut_معلومات, size_t lut_channel_اندازه, size_t lut_channels, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_lut hal_ni_lut
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
مضمن صحيح hal_ni_lut16(ثابت ushort *src_معلومات, size_t src_ګام, size_t src_type, ثابت uchar* lut_معلومات, size_t lut_channel_اندازه, size_t lut_channels, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_lut16 hal_ni_lut16
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
مضمن صحيح hal_ni_normHamming8u(ثابت uchar* a, صحيح n, صحيح cell_اندازه, صحيح* پایله) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_normHammingDiff8u(ثابت uchar* a, ثابت uchar* b, صحيح n, صحيح cell_اندازه, صحيح* پایله) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_norm(ثابت uchar* src, size_t src_ګام, ثابت uchar* ماسک, size_t ماسک_ګام, صحيح پلنوالی,
                       صحيح لوړوالی, صحيح type, صحيح norm_type, مزدوج* پایله) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_normDiff(ثابت uchar* src1, size_t src1_ګام, ثابت uchar* src2, size_t src2_ګام, ثابت uchar* ماسک,
                           size_t ماسک_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح type, صحيح norm_type, مزدوج* پایله) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_convertScale(ثابت uchar* src, size_t src_ګام, uchar* dst, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی,
                               صحيح sdepth, صحيح ddepth, مزدوج alpha, مزدوج beta) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_normHamming8u hal_ni_normHamming8u
#تعریف_کړه cv_hal_normHammingDiff8u hal_ni_normHammingDiff8u
#تعریف_کړه cv_hal_norm hal_ni_norm
#تعریف_کړه cv_hal_normDiff hal_ni_normDiff
#تعریف_کړه cv_hal_convertScale hal_ni_convertScale
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
مضمن صحيح hal_ni_cmp8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cmp16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح operation) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_cmp8u hal_ni_cmp8u
#تعریف_کړه cv_hal_cmp8s hal_ni_cmp8s
#تعریف_کړه cv_hal_cmp16u hal_ni_cmp16u
#تعریف_کړه cv_hal_cmp16s hal_ni_cmp16s
#تعریف_کړه cv_hal_cmp32u hal_ni_cmp32u
#تعریف_کړه cv_hal_cmp32s hal_ni_cmp32s
#تعریف_کړه cv_hal_cmp64u hal_ni_cmp64u
#تعریف_کړه cv_hal_cmp64s hal_ni_cmp64s
#تعریف_کړه cv_hal_cmp32f hal_ni_cmp32f
#تعریف_کړه cv_hal_cmp64f hal_ni_cmp64f
#تعریف_کړه cv_hal_cmp16f hal_ni_cmp16f
#تعریف_کړه cv_hal_cmp16bf hal_ni_cmp16bf
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
مضمن صحيح hal_ni_mul8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul8u16u(ثابت uchar* src1_معلومات, size_t src1_ګام, ثابت uchar* src2_معلومات, size_t src2_ګام, ushort* dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul8s16s(ثابت schar* src1_معلومات, size_t src1_ګام, ثابت schar* src2_معلومات, size_t src2_ګام, لنډ* dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_mul16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_div8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_div16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_recip8u(ثابت uchar *src_معلومات, size_t src_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip8s(ثابت schar *src_معلومات, size_t src_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip16u(ثابت ushort *src_معلومات, size_t src_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip16s(ثابت لنډ *src_معلومات, size_t src_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip32u(ثابت بینښه *src_معلومات, size_t src_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip32s(ثابت صحيح *src_معلومات, size_t src_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip32f(ثابت عائم *src_معلومات, size_t src_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip64f(ثابت مزدوج *src_معلومات, size_t src_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip64u(ثابت uint64 *src_معلومات, size_t src_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip64s(ثابت int64 *src_معلومات, size_t src_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip16f(ثابت cv_hal_f16 *src_معلومات, size_t src_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_recip16bf(ثابت cv_hal_bf16 *src_معلومات, size_t src_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, مزدوج scale) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_mul8u hal_ni_mul8u
#تعریف_کړه cv_hal_mul8s hal_ni_mul8s
#تعریف_کړه cv_hal_mul16u hal_ni_mul16u
#تعریف_کړه cv_hal_mul16s hal_ni_mul16s
#تعریف_کړه cv_hal_mul32u hal_ni_mul32u
#تعریف_کړه cv_hal_mul32s hal_ni_mul32s
#تعریف_کړه cv_hal_mul64u hal_ni_mul64u
#تعریف_کړه cv_hal_mul64s hal_ni_mul64s
#تعریف_کړه cv_hal_mul32f hal_ni_mul32f
#تعریف_کړه cv_hal_mul64f hal_ni_mul64f
#تعریف_کړه cv_hal_mul8u16u hal_ni_mul8u16u
#تعریف_کړه cv_hal_mul8s16s hal_ni_mul8s16s
#تعریف_کړه cv_hal_mul16f hal_ni_mul16f
#تعریف_کړه cv_hal_mul16bf hal_ni_mul16bf
#تعریف_کړه cv_hal_div8u hal_ni_div8u
#تعریف_کړه cv_hal_div8s hal_ni_div8s
#تعریف_کړه cv_hal_div16u hal_ni_div16u
#تعریف_کړه cv_hal_div16s hal_ni_div16s
#تعریف_کړه cv_hal_div32u hal_ni_div32u
#تعریف_کړه cv_hal_div32s hal_ni_div32s
#تعریف_کړه cv_hal_div64u hal_ni_div64u
#تعریف_کړه cv_hal_div64s hal_ni_div64s
#تعریف_کړه cv_hal_div32f hal_ni_div32f
#تعریف_کړه cv_hal_div64f hal_ni_div64f
#تعریف_کړه cv_hal_div16f hal_ni_div16f
#تعریف_کړه cv_hal_div16bf hal_ni_div16bf
#تعریف_کړه cv_hal_recip8u hal_ni_recip8u
#تعریف_کړه cv_hal_recip8s hal_ni_recip8s
#تعریف_کړه cv_hal_recip16u hal_ni_recip16u
#تعریف_کړه cv_hal_recip16s hal_ni_recip16s
#تعریف_کړه cv_hal_recip32u hal_ni_recip32u
#تعریف_کړه cv_hal_recip32s hal_ni_recip32s
#تعریف_کړه cv_hal_recip64u hal_ni_recip64u
#تعریف_کړه cv_hal_recip64s hal_ni_recip64s
#تعریف_کړه cv_hal_recip32f hal_ni_recip32f
#تعریف_کړه cv_hal_recip64f hal_ni_recip64f
#تعریف_کړه cv_hal_recip16f hal_ni_recip16f
#تعریف_کړه cv_hal_recip16bf hal_ni_recip16bf
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
مضمن صحيح hal_ni_جمع_کړه_Weighted8u(ثابت uchar *src1_معلومات, size_t src1_ګام, ثابت uchar *src2_معلومات, size_t src2_ګام, uchar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted8s(ثابت schar *src1_معلومات, size_t src1_ګام, ثابت schar *src2_معلومات, size_t src2_ګام, schar *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted16u(ثابت ushort *src1_معلومات, size_t src1_ګام, ثابت ushort *src2_معلومات, size_t src2_ګام, ushort *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted16s(ثابت لنډ *src1_معلومات, size_t src1_ګام, ثابت لنډ *src2_معلومات, size_t src2_ګام, لنډ *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted32u(ثابت بینښه *src1_معلومات, size_t src1_ګام, ثابت بینښه *src2_معلومات, size_t src2_ګام, بینښه *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted32s(ثابت صحيح *src1_معلومات, size_t src1_ګام, ثابت صحيح *src2_معلومات, size_t src2_ګام, صحيح *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted32f(ثابت عائم *src1_معلومات, size_t src1_ګام, ثابت عائم *src2_معلومات, size_t src2_ګام, عائم *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted64f(ثابت مزدوج *src1_معلومات, size_t src1_ګام, ثابت مزدوج *src2_معلومات, size_t src2_ګام, مزدوج *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted64u(ثابت uint64 *src1_معلومات, size_t src1_ګام, ثابت uint64 *src2_معلومات, size_t src2_ګام, uint64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted64s(ثابت int64 *src1_معلومات, size_t src1_ګام, ثابت int64 *src2_معلومات, size_t src2_ګام, int64 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted16f(ثابت cv_hal_f16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_f16 *src2_معلومات, size_t src2_ګام, cv_hal_f16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_جمع_کړه_Weighted16bf(ثابت cv_hal_bf16 *src1_معلومات, size_t src1_ګام, ثابت cv_hal_bf16 *src2_معلومات, size_t src2_ګام, cv_hal_bf16 *dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی, ثابت مزدوج scalars[3]) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_جمع_کړه_Weighted8u hal_ni_جمع_کړه_Weighted8u
#تعریف_کړه cv_hal_جمع_کړه_Weighted8s hal_ni_جمع_کړه_Weighted8s
#تعریف_کړه cv_hal_جمع_کړه_Weighted16u hal_ni_جمع_کړه_Weighted16u
#تعریف_کړه cv_hal_جمع_کړه_Weighted16s hal_ni_جمع_کړه_Weighted16s
#تعریف_کړه cv_hal_جمع_کړه_Weighted32u hal_ni_جمع_کړه_Weighted32u
#تعریف_کړه cv_hal_جمع_کړه_Weighted32s hal_ni_جمع_کړه_Weighted32s
#تعریف_کړه cv_hal_جمع_کړه_Weighted64u hal_ni_جمع_کړه_Weighted64u
#تعریف_کړه cv_hal_جمع_کړه_Weighted64s hal_ni_جمع_کړه_Weighted64s
#تعریف_کړه cv_hal_جمع_کړه_Weighted32f hal_ni_جمع_کړه_Weighted32f
#تعریف_کړه cv_hal_جمع_کړه_Weighted64f hal_ni_جمع_کړه_Weighted64f
#تعریف_کړه cv_hal_جمع_کړه_Weighted16f hal_ni_جمع_کړه_Weighted16f
#تعریف_کړه cv_hal_جمع_کړه_Weighted16bf hal_ni_جمع_کړه_Weighted16bf
//! @endcond

/**
@param src_data array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param dst_data array of pointers to destination arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_split Channel split
//! @{
مضمن صحيح hal_ni_split8u(ثابت uchar *src_معلومات, uchar **dst_معلومات, صحيح len, صحيح cn) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_split16u(ثابت ushort *src_معلومات, ushort **dst_معلومات, صحيح len, صحيح cn) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_split32s(ثابت صحيح *src_معلومات, صحيح **dst_معلومات, صحيح len, صحيح cn) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_split64s(ثابت int64 *src_معلومات, int64 **dst_معلومات, صحيح len, صحيح cn) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_split8u hal_ni_split8u
#تعریف_کړه cv_hal_split16u hal_ni_split16u
#تعریف_کړه cv_hal_split32s hal_ni_split32s
#تعریف_کړه cv_hal_split64s hal_ni_split64s
//! @endcond

/**
@param src_data array of pointers to source arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param dst_data destination array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_merge Channel merge
//! @{
مضمن صحيح hal_ni_merge8u(ثابت uchar **src_معلومات, uchar *dst_معلومات, صحيح len, صحيح cn) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_merge16u(ثابت ushort **src_معلومات, ushort *dst_معلومات, صحيح len, صحيح cn) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_merge32s(ثابت صحيح **src_معلومات, صحيح *dst_معلومات, صحيح len, صحيح cn) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_merge64s(ثابت int64 **src_معلومات, int64 *dst_معلومات, صحيح len, صحيح cn) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_merge8u hal_ni_merge8u
#تعریف_کړه cv_hal_merge16u hal_ni_merge16u
#تعریف_کړه cv_hal_merge32s hal_ni_merge32s
#تعریف_کړه cv_hal_merge64s hal_ni_merge64s
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
مضمن صحيح hal_ni_cart_منزل_Polar32f(ثابت عائم* x, ثابت عائم* y, عائم* mag, عائم* angle, صحيح len, منطقي angle_دننه_Degrees) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_cart_منزل_Polar64f(ثابت مزدوج* x, ثابت مزدوج* y, مزدوج* mag, مزدوج* angle, صحيح len, منطقي angle_دننه_Degrees) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_cart_منزل_Polar32f hal_ni_cart_منزل_Polar32f
#تعریف_کړه cv_hal_cart_منزل_Polar64f hal_ni_cart_منزل_Polar64f
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
مضمن صحيح hal_ni_fastAtan32f(ثابت عائم* y, ثابت عائم* x, عائم* dst, صحيح len, منطقي angle_دننه_Degrees) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_fastAtan64f(ثابت مزدوج* y, ثابت مزدوج* x, مزدوج* dst, صحيح len, منطقي angle_دننه_Degrees) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_fastAtan32f hal_ni_fastAtan32f
#تعریف_کړه cv_hal_fastAtan64f hal_ni_fastAtan64f
//! @endcond


/**
@param x source X array
@param y source Y array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_magnitude Magnitude calculation
//! @{
مضمن صحيح hal_ni_magnitude32f(ثابت عائم *x, ثابت عائم *y, عائم *dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_magnitude64f(ثابت مزدوج *x, ثابت مزدوج  *y, مزدوج *dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_magnitude32f hal_ni_magnitude32f
#تعریف_کړه cv_hal_magnitude64f hal_ni_magnitude64f
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
مضمن صحيح hal_ni_polar_منزل_Cart32f(ثابت عائم* mag, ثابت عائم* angle, عائم* x, عائم* y, صحيح len, منطقي angle_دننه_Degrees) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_polar_منزل_Cart64f(ثابت مزدوج* mag, ثابت مزدوج* angle, مزدوج* x, مزدوج* y, صحيح len, منطقي angle_دننه_Degrees) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_polar_منزل_Cart32f hal_ni_polar_منزل_Cart32f
#تعریف_کړه cv_hal_polar_منزل_Cart64f hal_ni_polar_منزل_Cart64f
//! @endcond

/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_invSqrt Inverse square root calculation
//! @{
مضمن صحيح hal_ni_invSqrt32f(ثابت عائم* src, عائم* dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_invSqrt64f(ثابت مزدوج* src, مزدوج* dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_invSqrt32f hal_ni_invSqrt32f
#تعریف_کړه cv_hal_invSqrt64f hal_ni_invSqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_sqrt Square root calculation
//! @{
مضمن صحيح hal_ni_sqrt32f(ثابت عائم* src, عائم* dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_sqrt64f(ثابت مزدوج* src, مزدوج* dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_sqrt32f hal_ni_sqrt32f
#تعریف_کړه cv_hal_sqrt64f hal_ni_sqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_log Natural logarithm calculation
//! @{
مضمن صحيح hal_ni_log32f(ثابت عائم* src, عائم* dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_log64f(ثابت مزدوج* src, مزدوج* dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_log32f hal_ni_log32f
#تعریف_کړه cv_hal_log64f hal_ni_log64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_exp Exponent calculation
//! @{
مضمن صحيح hal_ni_exp32f(ثابت عائم* src, عائم* dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_exp64f(ثابت مزدوج* src, مزدوج* dst, صحيح len) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_exp32f hal_ni_exp32f
#تعریف_کړه cv_hal_exp64f hal_ni_exp64f
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
بنية cvhalDFT {};

/**
@param context double pointer to context storing all necessary data
@param len transformed array length
@param count estimated transformation count
@param depth array type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, CV_HAL_DFT_SCALE, ...)
@param needBuffer pointer to boolean variable, if valid pointer provided, then variable value should be set to true to signal that additional memory buffer is needed for operations
 */
مضمن صحيح hal_ni_dftInit1D(cvhalDFT **context, صحيح len, صحيح شمېر_2, صحيح ژوروالی, صحيح flags, منطقي *need_بفر) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src source data
@param dst destination data
 */
مضمن صحيح hal_ni_dft1D(cvhalDFT *context, ثابت uchar *src, uchar *dst) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
مضمن صحيح hal_ni_dftFree1D(cvhalDFT *context) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
مضمن صحيح hal_ni_dft(ثابت uchar* src, uchar* dst, صحيح ژوروالی, صحيح nf, صحيح *factors, مزدوج scale, صحيح* itab, فراغ* wave,
                         صحيح tab_اندازه, صحيح n, منطقي isInverse, منطقي no_ترتیب_بدلول) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_dftInit1D hal_ni_dftInit1D
#تعریف_کړه cv_hal_dft1D hal_ni_dft1D
#تعریف_کړه cv_hal_dftFree1D hal_ni_dftFree1D
#تعریف_کړه cv_hal_dft hal_ni_dft
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
مضمن صحيح hal_ni_dftInit2D(cvhalDFT **context, صحيح پلنوالی, صحيح لوړوالی, صحيح ژوروالی, صحيح src_channels, صحيح dst_channels, صحيح flags, صحيح nonzero_لیکې) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
مضمن صحيح hal_ni_dft2D(cvhalDFT *context, ثابت uchar *src_معلومات, size_t src_ګام, uchar *dst_معلومات, size_t dst_ګام) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
مضمن صحيح hal_ni_dftFree2D(cvhalDFT *context) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_dftInit2D hal_ni_dftInit2D
#تعریف_کړه cv_hal_dft2D hal_ni_dft2D
#تعریف_کړه cv_hal_dftFree2D hal_ni_dftFree2D
//! @endcond

/**
@param context double pointer to context storing all necessary data
@param width image width
@param height image height
@param depth image type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, ...)
 */
مضمن صحيح hal_ni_dctInit2D(cvhalDFT **context, صحيح پلنوالی, صحيح لوړوالی, صحيح ژوروالی, صحيح flags) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
مضمن صحيح hal_ni_dct2D(cvhalDFT *context, ثابت uchar *src_معلومات, size_t src_ګام, uchar *dst_معلومات, size_t dst_ګام) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
مضمن صحيح hal_ni_dctFree2D(cvhalDFT *context) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_dctInit2D hal_ni_dctInit2D
#تعریف_کړه cv_hal_dct2D hal_ni_dct2D
#تعریف_کړه cv_hal_dctFree2D hal_ni_dctFree2D
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
مضمن صحيح hal_ni_LU32f(عائم* src1, size_t src1_ګام, صحيح m, عائم* src2, size_t src2_ګام, صحيح n, صحيح* info) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_LU64f(مزدوج* src1, size_t src1_ګام, صحيح m, مزدوج* src2, size_t src2_ګام, صحيح n, صحيح* info) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_Cholesky32f(عائم* src1, size_t src1_ګام, صحيح m, عائم* src2, size_t src2_ګام, صحيح n, منطقي* info) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_Cholesky64f(مزدوج* src1, size_t src1_ګام, صحيح m, مزدوج* src2, size_t src2_ګام, صحيح n, منطقي* info) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_SVD32f(عائم* src, size_t src_ګام, عائم* w, عائم* u, size_t u_ګام, عائم* vt, size_t vt_ګام, صحيح m, صحيح n, صحيح flags) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_SVD64f(مزدوج* src, size_t src_ګام, مزدوج* w, مزدوج* u, size_t u_ګام, مزدوج* vt, size_t vt_ګام, صحيح m, صحيح n, صحيح flags) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_QR32f(عائم* src1, size_t src1_ګام, صحيح m, صحيح n, صحيح k, عائم* src2, size_t src2_ګام, عائم* dst, صحيح* info) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_QR64f(مزدوج* src1, size_t src1_ګام, صحيح m, صحيح n, صحيح k, مزدوج* src2, size_t src2_ګام, مزدوج* dst, صحيح* info) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}



//! @cond IGNORED
#تعریف_کړه cv_hal_LU32f hal_ni_LU32f
#تعریف_کړه cv_hal_LU64f hal_ni_LU64f
#تعریف_کړه cv_hal_Cholesky32f hal_ni_Cholesky32f
#تعریف_کړه cv_hal_Cholesky64f hal_ni_Cholesky64f
#تعریف_کړه cv_hal_SVD32f hal_ni_SVD32f
#تعریف_کړه cv_hal_SVD64f hal_ni_SVD64f
#تعریف_کړه cv_hal_QR32f hal_ni_QR32f
#تعریف_کړه cv_hal_QR64f hal_ni_QR64f
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
مضمن صحيح hal_ni_gemm32f(ثابت عائم* src1, size_t src1_ګام, ثابت عائم* src2, size_t src2_ګام,
                          عائم alpha, ثابت عائم* src3, size_t src3_ګام, عائم beta, عائم* dst, size_t dst_ګام,
                          صحيح m, صحيح n, صحيح k, صحيح flags) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_gemm64f(ثابت مزدوج* src1, size_t src1_ګام, ثابت مزدوج* src2, size_t src2_ګام,
                          مزدوج alpha, ثابت مزدوج* src3, size_t src3_ګام, مزدوج beta, مزدوج* dst, size_t dst_ګام,
                          صحيح m, صحيح n, صحيح k, صحيح flags) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_gemm32fc(ثابت عائم* src1, size_t src1_ګام, ثابت عائم* src2, size_t src2_ګام,
                          عائم alpha, ثابت عائم* src3, size_t src3_ګام, عائم beta, عائم* dst, size_t dst_ګام,
                          صحيح m, صحيح n, صحيح k, صحيح flags) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
مضمن صحيح hal_ni_gemm64fc(ثابت مزدوج* src1, size_t src1_ګام, ثابت مزدوج* src2, size_t src2_ګام,
                          مزدوج alpha, ثابت مزدوج* src3, size_t src3_ګام, مزدوج beta, مزدوج* dst, size_t dst_ګام,
                          صحيح m, صحيح n, صحيح k, صحيح flags) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#تعریف_کړه cv_hal_gemm32f hal_ni_gemm32f
#تعریف_کړه cv_hal_gemm64f hal_ni_gemm64f
#تعریف_کړه cv_hal_gemm32fc hal_ni_gemm32fc
#تعریف_کړه cv_hal_gemm64fc hal_ni_gemm64fc
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
مضمن صحيح hal_ni_minMaxIdx(ثابت uchar* src_معلومات, size_t src_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح ژوروالی, مزدوج* minVal, مزدوج* maxVal,
                            صحيح* minIdx, صحيح* maxIdx, uchar* ماسک) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
مضمن صحيح hal_ni_min_Max_Idx_ماسک_ګام(ثابت uchar* src_معلومات, size_t src_ګام, صحيح پلنوالی, صحيح لوړوالی, صحيح ژوروالی, مزدوج* minVal, مزدوج* maxVal,
                                    صحيح* minIdx, صحيح* maxIdx, uchar* ماسک, size_t ماسک_ګام) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_minMaxIdx hal_ni_minMaxIdx
#تعریف_کړه cv_hal_min_Max_Idx_ماسک_ګام hal_ni_min_Max_Idx_ماسک_ګام
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
مضمن صحيح hal_ni_منځنۍ_Std_Dev(ثابت uchar* src_معلومات, size_t src_ګام, صحيح پلنوالی, صحيح لوړوالی,
                             صحيح src_type, مزدوج* منځنۍ_val, مزدوج* stddev_val, uchar* ماسک, size_t ماسک_ګام)
{ أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_منځنۍ_Std_Dev hal_ni_منځنۍ_Std_Dev
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
مضمن صحيح hal_ni_dot_حاصل_ضرب(ثابت uchar* a_معلومات, size_t a_ګام, ثابت uchar* b_معلومات, size_t b_ګام, صحيح پلنوالی, صحيح لوړوالی,
                             صحيح type, مزدوج *dot_val)
{ أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_dot_حاصل_ضرب hal_ni_dot_حاصل_ضرب
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
مضمن صحيح hal_ni_flip(صحيح src_type, ثابت uchar* src_معلومات, size_t src_ګام, صحيح src_پلنوالی, صحيح src_لوړوالی,
                       uchar* dst_معلومات, size_t dst_ګام, صحيح flip_ډېرپېښ) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_flip hal_ni_flip
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
مضمن صحيح hal_ni_rotate90(صحيح src_type, ثابت uchar* src_معلومات, size_t src_ګام, صحيح src_پلنوالی, صحيح src_لوړوالی,
                           uchar* dst_معلومات, size_t dst_ګام, صحيح angle) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_rotate90 hal_ni_rotate90
//! @endcond

/**
   @brief Transpose2d
   @param src_data,src_step Source image
   @param dst_data,dst_step Destination image
   @param src_width,src_height Source image dimensions
   @param element_size Size of an element in bytes
*/
مضمن صحيح hal_ni_transpose2d(ثابت uchar* src_معلومات, size_t src_ګام, uchar* dst_معلومات, size_t dst_ګام, صحيح src_پلنوالی,
                              صحيح src_لوړوالی, صحيح element_اندازه) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_transpose2d hal_ni_transpose2d
//! @endcond

/**
    @brief copyTo with mask
    @param src_data, src_step Source image
    @param dst_data, dst_step Destination image
    @param width, height Image dimensions of source, destination and mask
    @param type Type of source and destination images, for example CV_8UC1 or CV_32FC3
    @param mask_data, mask_step, mask_type Mask
*/
مضمن صحيح hal_ni_copy_منزل_Masked(ثابت uchar* src_معلومات, size_t src_ګام, uchar* dst_معلومات, size_t dst_ګام, صحيح پلنوالی, صحيح لوړوالی,
                             صحيح type, ثابت uchar* ماسک_معلومات, size_t ماسک_ګام, صحيح ماسک_type)
{ أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_copy_منزل_Masked hal_ni_copy_منزل_Masked
//! @endcond

/**
 @ brief sum
 @param src_data Source image data
 @param src_step Source image step
 @param src_type Source image type
 @param width, height Source image dimensions
 @param result Pointer to save the sum result to.
 */
مضمن صحيح hal_ni_مجموعه(ثابت uchar *src_معلومات, size_t src_ګام, صحيح src_type, صحيح پلنوالی, صحيح لوړوالی, مزدوج *پایله)
{ أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_مجموعه hal_ni_مجموعه
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
مضمن صحيح hal_ni_دننه_Range8u(ثابت uchar *src_معلومات, size_t src_ګام,
              uchar *dst_معلومات, size_t dst_ګام, صحيح dst_ژوروالی_2, صحيح پلنوالی,
              صحيح لوړوالی, صحيح cn, uchar lower_bound, uchar upper_bound) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
مضمن صحيح hal_ni_دننه_Range32f(ثابت uchar *src_معلومات, size_t src_ګام,
              uchar *dst_معلومات, size_t dst_ګام, صحيح dst_ژوروالی_2, صحيح پلنوالی,
              صحيح لوړوالی, صحيح cn, مزدوج lower_bound, مزدوج upper_bound) { أرجع CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#تعریف_کړه cv_hal_دننه_Range8u hal_ni_دننه_Range8u
#تعریف_کړه cv_hal_دننه_Range32f hal_ni_دننه_Range32f
//! @endcond

//! @}

#إذا defined(__clang__)
#لارښوونه clang diagnostic pop
#وإلاإذا defined(_MSC_VER)
#لارښوونه warning(pop)
#وإلاإذا defined(__GNUC__)
#لارښوونه GCC diagnostic pop
#د_که_پای

#ضمن "hal_internal.hpp"
#ضمن "custom_hal.hpp"

//! @cond IGNORED

#تعریف_کړه CALL_HAL_RET2(name, fun, retval, ...) \
{ \
    صحيح res = __CV_EXPAND(fun(__VA_ARGS__)); \
    إذا (res == CV_HAL_ERROR_OK) \
        أرجع retval; \
    وإلا إذا (res != CV_HAL_ERROR_NOT_IMPLEMENTED) \
        CV_Error_(cv::Error::Sts_دننی, \
        ("HAL implementation " CVAUX_STR(name) " ==> " CVAUX_STR(fun) " returned %d (0x%08x)", res, res)); \
}

#تعریف_کړه CALL_HAL_RET(name, fun, retval, ...) \
CALL_HAL_RET2(name, fun, retval, __VA_ARGS__, &retval)

#تعریف_کړه CALL_HAL(name, fun, ...) \
CALL_HAL_RET2(name, fun, ,__VA_ARGS__)

//! @endcond

#د_که_پای
