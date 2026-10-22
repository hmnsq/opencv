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

#నిర్వచితం_కాకపోతే OPENCV_CORE_HAL_REPLACEMENT_HPP
#నిర్వచించు OPENCV_CORE_HAL_REPLACEMENT_HPP

#అన్తర్భావ "opencv2/core/hal/interface.h"

#ఒకవేళ defined(__clang__)  // clang or MSVC clang
#సూచన clang diagnostic push
#సూచన clang diagnostic ignored "-Wunused-parameter"
#లేదావేళ defined(_MSC_VER)
#సూచన warning(push)
#సూచన warning(disable : 4100)
#లేదావేళ defined(__GNUC__)
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wunused-parameter"
#అయితే_ముగింపు

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
అంతఃవరుస పూర్ణాంకం hal_ni_add8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_add16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

అంతఃవరుస పూర్ణాంకం hal_ni_sub8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

అంతఃవరుస పూర్ణాంకం hal_ni_sub8u32f(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sub8s32f(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Scalar32f32f(స్థిరరాశి దశాంశం*   src_దత్తాంశం, size_t src_అడుగు, దశాంశం*   dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి దశాంశం*   scalar_దత్తాంశం, పూర్ణాంకం nChannels) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Scalar16s16s(స్థిరరాశి int16_t* src_దత్తాంశం, size_t src_అడుగు, int16_t* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి int16_t* scalar_దత్తాంశం, పూర్ణాంకం nChannels) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_max8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_max16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

అంతఃవరుస పూర్ణాంకం hal_ni_min8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_min16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absdiff16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
అంతఃవరుస పూర్ణాంకం hal_ni_absDiffScalar32f32f(స్థిరరాశి దశాంశం* src_దత్తాంశం, size_t src_అడుగు, దశాంశం*    dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి దశాంశం* scalar_దత్తాంశం, పూర్ణాంకం nChannels) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absDiffScalar32s32u(స్థిరరాశి పూర్ణాంకం*   src_దత్తాంశం, size_t src_అడుగు, uint32_t* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి పూర్ణాంకం*   scalar_దత్తాంశం, పూర్ణాంకం nChannels) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_absDiffScalar8u8u  (స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, uchar*    dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి uchar* scalar_దత్తాంశం, పూర్ణాంకం nChannels) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
అంతఃవరుస పూర్ణాంకం hal_ni_and8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_or8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_xor8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_not8u(స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_add8u hal_ni_add8u
#నిర్వచించు cv_hal_add8s hal_ni_add8s
#నిర్వచించు cv_hal_add16u hal_ni_add16u
#నిర్వచించు cv_hal_add16s hal_ni_add16s
#నిర్వచించు cv_hal_add32u hal_ni_add32u
#నిర్వచించు cv_hal_add32s hal_ni_add32s
#నిర్వచించు cv_hal_add64u hal_ni_add64u
#నిర్వచించు cv_hal_add64s hal_ni_add64s
#నిర్వచించు cv_hal_add32f hal_ni_add32f
#నిర్వచించు cv_hal_add64f hal_ni_add64f
#నిర్వచించు cv_hal_add16f hal_ni_add16f
#నిర్వచించు cv_hal_add16bf hal_ni_add16bf
#నిర్వచించు cv_hal_sub8u hal_ni_sub8u
#నిర్వచించు cv_hal_sub8s hal_ni_sub8s
#నిర్వచించు cv_hal_sub16u hal_ni_sub16u
#నిర్వచించు cv_hal_sub16s hal_ni_sub16s
#నిర్వచించు cv_hal_sub32u hal_ni_sub32u
#నిర్వచించు cv_hal_sub32s hal_ni_sub32s
#నిర్వచించు cv_hal_sub64u hal_ni_sub64u
#నిర్వచించు cv_hal_sub64s hal_ni_sub64s
#నిర్వచించు cv_hal_sub32f hal_ni_sub32f
#నిర్వచించు cv_hal_sub64f hal_ni_sub64f
#నిర్వచించు cv_hal_sub8u32f hal_ni_sub8u32f
#నిర్వచించు cv_hal_sub8s32f hal_ni_sub8s32f
#నిర్వచించు cv_hal_sub16f hal_ni_sub16f
#నిర్వచించు cv_hal_sub16bf hal_ni_sub16bf
#నిర్వచించు cv_hal_కూడు_Scalar32f32f hal_ni_కూడు_Scalar32f32f
#నిర్వచించు cv_hal_కూడు_Scalar16s16s hal_ni_కూడు_Scalar16s16s
#నిర్వచించు cv_hal_max8u hal_ni_max8u
#నిర్వచించు cv_hal_max8s hal_ni_max8s
#నిర్వచించు cv_hal_max16u hal_ni_max16u
#నిర్వచించు cv_hal_max16s hal_ni_max16s
#నిర్వచించు cv_hal_max32u hal_ni_max32u
#నిర్వచించు cv_hal_max32s hal_ni_max32s
#నిర్వచించు cv_hal_max64u hal_ni_max64u
#నిర్వచించు cv_hal_max64s hal_ni_max64s
#నిర్వచించు cv_hal_max32f hal_ni_max32f
#నిర్వచించు cv_hal_max64f hal_ni_max64f
#నిర్వచించు cv_hal_max16f hal_ni_max16f
#నిర్వచించు cv_hal_max16bf hal_ni_max16bf
#నిర్వచించు cv_hal_min8u hal_ni_min8u
#నిర్వచించు cv_hal_min8s hal_ni_min8s
#నిర్వచించు cv_hal_min16u hal_ni_min16u
#నిర్వచించు cv_hal_min16s hal_ni_min16s
#నిర్వచించు cv_hal_min32u hal_ni_min32u
#నిర్వచించు cv_hal_min32s hal_ni_min32s
#నిర్వచించు cv_hal_min64u hal_ni_min64u
#నిర్వచించు cv_hal_min64s hal_ni_min64s
#నిర్వచించు cv_hal_min32f hal_ni_min32f
#నిర్వచించు cv_hal_min64f hal_ni_min64f
#నిర్వచించు cv_hal_min16f hal_ni_min16f
#నిర్వచించు cv_hal_min16bf hal_ni_min16bf
#నిర్వచించు cv_hal_absdiff8u hal_ni_absdiff8u
#నిర్వచించు cv_hal_absdiff8s hal_ni_absdiff8s
#నిర్వచించు cv_hal_absdiff16u hal_ni_absdiff16u
#నిర్వచించు cv_hal_absdiff16s hal_ni_absdiff16s
#నిర్వచించు cv_hal_absdiff32u hal_ni_absdiff32u
#నిర్వచించు cv_hal_absdiff32s hal_ni_absdiff32s
#నిర్వచించు cv_hal_absdiff64u hal_ni_absdiff64u
#నిర్వచించు cv_hal_absdiff64s hal_ni_absdiff64s
#నిర్వచించు cv_hal_absdiff32f hal_ni_absdiff32f
#నిర్వచించు cv_hal_absdiff64f hal_ni_absdiff64f
#నిర్వచించు cv_hal_absdiff16f hal_ni_absdiff16f
#నిర్వచించు cv_hal_absdiff16bf hal_ni_absdiff16bf
#నిర్వచించు cv_hal_absDiffScalar32f32f hal_ni_absDiffScalar32f32f
#నిర్వచించు cv_hal_absDiffScalar32s32u hal_ni_absDiffScalar32s32u
#నిర్వచించు cv_hal_absDiffScalar8u8u   hal_ni_absDiffScalar8u8u
#నిర్వచించు cv_hal_and8u hal_ni_and8u
#నిర్వచించు cv_hal_or8u hal_ni_or8u
#నిర్వచించు cv_hal_xor8u hal_ni_xor8u
#నిర్వచించు cv_hal_not8u hal_ni_not8u
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
అంతఃవరుస పూర్ణాంకం hal_ni_lut(స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు, size_t src_type, స్థిరరాశి uchar* lut_దత్తాంశం, size_t lut_channel_కొలత, size_t lut_channels, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_lut hal_ni_lut
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
అంతఃవరుస పూర్ణాంకం hal_ni_lut16(స్థిరరాశి ushort *src_దత్తాంశం, size_t src_అడుగు, size_t src_type, స్థిరరాశి uchar* lut_దత్తాంశం, size_t lut_channel_కొలత, size_t lut_channels, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_lut16 hal_ni_lut16
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
అంతఃవరుస పూర్ణాంకం hal_ni_normHamming8u(స్థిరరాశి uchar* a, పూర్ణాంకం n, పూర్ణాంకం cell_కొలత, పూర్ణాంకం* ఫలితం) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_normHammingDiff8u(స్థిరరాశి uchar* a, స్థిరరాశి uchar* b, పూర్ణాంకం n, పూర్ణాంకం cell_కొలత, పూర్ణాంకం* ఫలితం) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_norm(స్థిరరాశి uchar* src, size_t src_అడుగు, స్థిరరాశి uchar* ముసుగు, size_t ముసుగు_అడుగు, పూర్ణాంకం వెడల్పు,
                       పూర్ణాంకం ఎత్తు, పూర్ణాంకం type, పూర్ణాంకం norm_type, ద్విగుణ* ఫలితం) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_normDiff(స్థిరరాశి uchar* src1, size_t src1_అడుగు, స్థిరరాశి uchar* src2, size_t src2_అడుగు, స్థిరరాశి uchar* ముసుగు,
                           size_t ముసుగు_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం type, పూర్ణాంకం norm_type, ద్విగుణ* ఫలితం) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_convertScale(స్థిరరాశి uchar* src, size_t src_అడుగు, uchar* dst, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                               పూర్ణాంకం sdepth, పూర్ణాంకం ddepth, ద్విగుణ alpha, ద్విగుణ beta) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_normHamming8u hal_ni_normHamming8u
#నిర్వచించు cv_hal_normHammingDiff8u hal_ni_normHammingDiff8u
#నిర్వచించు cv_hal_norm hal_ni_norm
#నిర్వచించు cv_hal_normDiff hal_ni_normDiff
#నిర్వచించు cv_hal_convertScale hal_ni_convertScale
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
అంతఃవరుస పూర్ణాంకం hal_ni_cmp8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cmp16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం operation) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_cmp8u hal_ni_cmp8u
#నిర్వచించు cv_hal_cmp8s hal_ni_cmp8s
#నిర్వచించు cv_hal_cmp16u hal_ni_cmp16u
#నిర్వచించు cv_hal_cmp16s hal_ni_cmp16s
#నిర్వచించు cv_hal_cmp32u hal_ni_cmp32u
#నిర్వచించు cv_hal_cmp32s hal_ni_cmp32s
#నిర్వచించు cv_hal_cmp64u hal_ni_cmp64u
#నిర్వచించు cv_hal_cmp64s hal_ni_cmp64s
#నిర్వచించు cv_hal_cmp32f hal_ni_cmp32f
#నిర్వచించు cv_hal_cmp64f hal_ni_cmp64f
#నిర్వచించు cv_hal_cmp16f hal_ni_cmp16f
#నిర్వచించు cv_hal_cmp16bf hal_ni_cmp16bf
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
అంతఃవరుస పూర్ణాంకం hal_ni_mul8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul8u16u(స్థిరరాశి uchar* src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar* src2_దత్తాంశం, size_t src2_అడుగు, ushort* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul8s16s(స్థిరరాశి schar* src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar* src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_mul16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_div8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_div16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_recip8u(స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip8s(స్థిరరాశి schar *src_దత్తాంశం, size_t src_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip16u(స్థిరరాశి ushort *src_దత్తాంశం, size_t src_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip16s(స్థిరరాశి పొట్టి *src_దత్తాంశం, size_t src_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip32u(స్థిరరాశి గుర్తులేని *src_దత్తాంశం, size_t src_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip32s(స్థిరరాశి పూర్ణాంకం *src_దత్తాంశం, size_t src_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip32f(స్థిరరాశి దశాంశం *src_దత్తాంశం, size_t src_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip64f(స్థిరరాశి ద్విగుణ *src_దత్తాంశం, size_t src_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip64u(స్థిరరాశి uint64 *src_దత్తాంశం, size_t src_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip64s(స్థిరరాశి int64 *src_దత్తాంశం, size_t src_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip16f(స్థిరరాశి cv_hal_f16 *src_దత్తాంశం, size_t src_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_recip16bf(స్థిరరాశి cv_hal_bf16 *src_దత్తాంశం, size_t src_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ scale) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_mul8u hal_ni_mul8u
#నిర్వచించు cv_hal_mul8s hal_ni_mul8s
#నిర్వచించు cv_hal_mul16u hal_ni_mul16u
#నిర్వచించు cv_hal_mul16s hal_ni_mul16s
#నిర్వచించు cv_hal_mul32u hal_ni_mul32u
#నిర్వచించు cv_hal_mul32s hal_ni_mul32s
#నిర్వచించు cv_hal_mul64u hal_ni_mul64u
#నిర్వచించు cv_hal_mul64s hal_ni_mul64s
#నిర్వచించు cv_hal_mul32f hal_ni_mul32f
#నిర్వచించు cv_hal_mul64f hal_ni_mul64f
#నిర్వచించు cv_hal_mul8u16u hal_ni_mul8u16u
#నిర్వచించు cv_hal_mul8s16s hal_ni_mul8s16s
#నిర్వచించు cv_hal_mul16f hal_ni_mul16f
#నిర్వచించు cv_hal_mul16bf hal_ni_mul16bf
#నిర్వచించు cv_hal_div8u hal_ni_div8u
#నిర్వచించు cv_hal_div8s hal_ni_div8s
#నిర్వచించు cv_hal_div16u hal_ni_div16u
#నిర్వచించు cv_hal_div16s hal_ni_div16s
#నిర్వచించు cv_hal_div32u hal_ni_div32u
#నిర్వచించు cv_hal_div32s hal_ni_div32s
#నిర్వచించు cv_hal_div64u hal_ni_div64u
#నిర్వచించు cv_hal_div64s hal_ni_div64s
#నిర్వచించు cv_hal_div32f hal_ni_div32f
#నిర్వచించు cv_hal_div64f hal_ni_div64f
#నిర్వచించు cv_hal_div16f hal_ni_div16f
#నిర్వచించు cv_hal_div16bf hal_ni_div16bf
#నిర్వచించు cv_hal_recip8u hal_ni_recip8u
#నిర్వచించు cv_hal_recip8s hal_ni_recip8s
#నిర్వచించు cv_hal_recip16u hal_ni_recip16u
#నిర్వచించు cv_hal_recip16s hal_ni_recip16s
#నిర్వచించు cv_hal_recip32u hal_ni_recip32u
#నిర్వచించు cv_hal_recip32s hal_ni_recip32s
#నిర్వచించు cv_hal_recip64u hal_ni_recip64u
#నిర్వచించు cv_hal_recip64s hal_ni_recip64s
#నిర్వచించు cv_hal_recip32f hal_ni_recip32f
#నిర్వచించు cv_hal_recip64f hal_ni_recip64f
#నిర్వచించు cv_hal_recip16f hal_ni_recip16f
#నిర్వచించు cv_hal_recip16bf hal_ni_recip16bf
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
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted8u(స్థిరరాశి uchar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uchar *src2_దత్తాంశం, size_t src2_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted8s(స్థిరరాశి schar *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి schar *src2_దత్తాంశం, size_t src2_అడుగు, schar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted16u(స్థిరరాశి ushort *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ushort *src2_దత్తాంశం, size_t src2_అడుగు, ushort *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted16s(స్థిరరాశి పొట్టి *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పొట్టి *src2_దత్తాంశం, size_t src2_అడుగు, పొట్టి *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted32u(స్థిరరాశి గుర్తులేని *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి గుర్తులేని *src2_దత్తాంశం, size_t src2_అడుగు, గుర్తులేని *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted32s(స్థిరరాశి పూర్ణాంకం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి పూర్ణాంకం *src2_దత్తాంశం, size_t src2_అడుగు, పూర్ణాంకం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted32f(స్థిరరాశి దశాంశం *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి దశాంశం *src2_దత్తాంశం, size_t src2_అడుగు, దశాంశం *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted64f(స్థిరరాశి ద్విగుణ *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ *src2_దత్తాంశం, size_t src2_అడుగు, ద్విగుణ *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted64u(స్థిరరాశి uint64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి uint64 *src2_దత్తాంశం, size_t src2_అడుగు, uint64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted64s(స్థిరరాశి int64 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి int64 *src2_దత్తాంశం, size_t src2_అడుగు, int64 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted16f(స్థిరరాశి cv_hal_f16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_f16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_f16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_కూడు_Weighted16bf(స్థిరరాశి cv_hal_bf16 *src1_దత్తాంశం, size_t src1_అడుగు, స్థిరరాశి cv_hal_bf16 *src2_దత్తాంశం, size_t src2_అడుగు, cv_hal_bf16 *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, స్థిరరాశి ద్విగుణ scalars[3]) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_కూడు_Weighted8u hal_ni_కూడు_Weighted8u
#నిర్వచించు cv_hal_కూడు_Weighted8s hal_ni_కూడు_Weighted8s
#నిర్వచించు cv_hal_కూడు_Weighted16u hal_ni_కూడు_Weighted16u
#నిర్వచించు cv_hal_కూడు_Weighted16s hal_ni_కూడు_Weighted16s
#నిర్వచించు cv_hal_కూడు_Weighted32u hal_ni_కూడు_Weighted32u
#నిర్వచించు cv_hal_కూడు_Weighted32s hal_ni_కూడు_Weighted32s
#నిర్వచించు cv_hal_కూడు_Weighted64u hal_ni_కూడు_Weighted64u
#నిర్వచించు cv_hal_కూడు_Weighted64s hal_ni_కూడు_Weighted64s
#నిర్వచించు cv_hal_కూడు_Weighted32f hal_ni_కూడు_Weighted32f
#నిర్వచించు cv_hal_కూడు_Weighted64f hal_ni_కూడు_Weighted64f
#నిర్వచించు cv_hal_కూడు_Weighted16f hal_ni_కూడు_Weighted16f
#నిర్వచించు cv_hal_కూడు_Weighted16bf hal_ni_కూడు_Weighted16bf
//! @endcond

/**
@param src_data array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param dst_data array of pointers to destination arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_split Channel split
//! @{
అంతఃవరుస పూర్ణాంకం hal_ni_split8u(స్థిరరాశి uchar *src_దత్తాంశం, uchar **dst_దత్తాంశం, పూర్ణాంకం len, పూర్ణాంకం cn) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_split16u(స్థిరరాశి ushort *src_దత్తాంశం, ushort **dst_దత్తాంశం, పూర్ణాంకం len, పూర్ణాంకం cn) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_split32s(స్థిరరాశి పూర్ణాంకం *src_దత్తాంశం, పూర్ణాంకం **dst_దత్తాంశం, పూర్ణాంకం len, పూర్ణాంకం cn) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_split64s(స్థిరరాశి int64 *src_దత్తాంశం, int64 **dst_దత్తాంశం, పూర్ణాంకం len, పూర్ణాంకం cn) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_split8u hal_ni_split8u
#నిర్వచించు cv_hal_split16u hal_ni_split16u
#నిర్వచించు cv_hal_split32s hal_ni_split32s
#నిర్వచించు cv_hal_split64s hal_ni_split64s
//! @endcond

/**
@param src_data array of pointers to source arrays (__cn__ items x __len__ items) [ [B, B, ...], [G, G, ...], [R, R, ...] ]
@param dst_data destination array of interleaved values (__len__ x __cn__ items) [ B, G, R, B, G, R, ...]
@param len number of elements
@param cn number of channels
 */
//! @addtogroup core_hal_interface_merge Channel merge
//! @{
అంతఃవరుస పూర్ణాంకం hal_ni_merge8u(స్థిరరాశి uchar **src_దత్తాంశం, uchar *dst_దత్తాంశం, పూర్ణాంకం len, పూర్ణాంకం cn) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_merge16u(స్థిరరాశి ushort **src_దత్తాంశం, ushort *dst_దత్తాంశం, పూర్ణాంకం len, పూర్ణాంకం cn) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_merge32s(స్థిరరాశి పూర్ణాంకం **src_దత్తాంశం, పూర్ణాంకం *dst_దత్తాంశం, పూర్ణాంకం len, పూర్ణాంకం cn) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_merge64s(స్థిరరాశి int64 **src_దత్తాంశం, int64 *dst_దత్తాంశం, పూర్ణాంకం len, పూర్ణాంకం cn) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_merge8u hal_ni_merge8u
#నిర్వచించు cv_hal_merge16u hal_ni_merge16u
#నిర్వచించు cv_hal_merge32s hal_ni_merge32s
#నిర్వచించు cv_hal_merge64s hal_ni_merge64s
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
అంతఃవరుస పూర్ణాంకం hal_ni_cart_గమ్యం_Polar32f(స్థిరరాశి దశాంశం* x, స్థిరరాశి దశాంశం* y, దశాంశం* mag, దశాంశం* angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_cart_గమ్యం_Polar64f(స్థిరరాశి ద్విగుణ* x, స్థిరరాశి ద్విగుణ* y, ద్విగుణ* mag, ద్విగుణ* angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_cart_గమ్యం_Polar32f hal_ni_cart_గమ్యం_Polar32f
#నిర్వచించు cv_hal_cart_గమ్యం_Polar64f hal_ni_cart_గమ్యం_Polar64f
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
అంతఃవరుస పూర్ణాంకం hal_ni_fastAtan32f(స్థిరరాశి దశాంశం* y, స్థిరరాశి దశాంశం* x, దశాంశం* dst, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_fastAtan64f(స్థిరరాశి ద్విగుణ* y, స్థిరరాశి ద్విగుణ* x, ద్విగుణ* dst, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_fastAtan32f hal_ni_fastAtan32f
#నిర్వచించు cv_hal_fastAtan64f hal_ni_fastAtan64f
//! @endcond


/**
@param x source X array
@param y source Y array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_magnitude Magnitude calculation
//! @{
అంతఃవరుస పూర్ణాంకం hal_ni_magnitude32f(స్థిరరాశి దశాంశం *x, స్థిరరాశి దశాంశం *y, దశాంశం *dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_magnitude64f(స్థిరరాశి ద్విగుణ *x, స్థిరరాశి ద్విగుణ  *y, ద్విగుణ *dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_magnitude32f hal_ni_magnitude32f
#నిర్వచించు cv_hal_magnitude64f hal_ni_magnitude64f
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
అంతఃవరుస పూర్ణాంకం hal_ni_polar_గమ్యం_Cart32f(స్థిరరాశి దశాంశం* mag, స్థిరరాశి దశాంశం* angle, దశాంశం* x, దశాంశం* y, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_polar_గమ్యం_Cart64f(స్థిరరాశి ద్విగుణ* mag, స్థిరరాశి ద్విగుణ* angle, ద్విగుణ* x, ద్విగుణ* y, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_polar_గమ్యం_Cart32f hal_ni_polar_గమ్యం_Cart32f
#నిర్వచించు cv_hal_polar_గమ్యం_Cart64f hal_ni_polar_గమ్యం_Cart64f
//! @endcond

/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_invSqrt Inverse square root calculation
//! @{
అంతఃవరుస పూర్ణాంకం hal_ni_invSqrt32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_invSqrt64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_invSqrt32f hal_ni_invSqrt32f
#నిర్వచించు cv_hal_invSqrt64f hal_ni_invSqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_sqrt Square root calculation
//! @{
అంతఃవరుస పూర్ణాంకం hal_ni_sqrt32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_sqrt64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_sqrt32f hal_ni_sqrt32f
#నిర్వచించు cv_hal_sqrt64f hal_ni_sqrt64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_log Natural logarithm calculation
//! @{
అంతఃవరుస పూర్ణాంకం hal_ni_log32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_log64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_log32f hal_ni_log32f
#నిర్వచించు cv_hal_log64f hal_ni_log64f
//! @endcond


/**
@param src source array
@param dst destination array
@param len length of arrays
 */
//! @addtogroup core_hal_interface_exp Exponent calculation
//! @{
అంతఃవరుస పూర్ణాంకం hal_ni_exp32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_exp64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_exp32f hal_ni_exp32f
#నిర్వచించు cv_hal_exp64f hal_ni_exp64f
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
నిర్మాణం cvhalDFT {};

/**
@param context double pointer to context storing all necessary data
@param len transformed array length
@param count estimated transformation count
@param depth array type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, CV_HAL_DFT_SCALE, ...)
@param needBuffer pointer to boolean variable, if valid pointer provided, then variable value should be set to true to signal that additional memory buffer is needed for operations
 */
అంతఃవరుస పూర్ణాంకం hal_ni_dftInit1D(cvhalDFT **context, పూర్ణాంకం len, పూర్ణాంకం లెక్క_2, పూర్ణాంకం లోతు, పూర్ణాంకం flags, తార్కిక *need_బఫరు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src source data
@param dst destination data
 */
అంతఃవరుస పూర్ణాంకం hal_ni_dft1D(cvhalDFT *context, స్థిరరాశి uchar *src, uchar *dst) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
అంతఃవరుస పూర్ణాంకం hal_ni_dftFree1D(cvhalDFT *context) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
అంతఃవరుస పూర్ణాంకం hal_ni_dft(స్థిరరాశి uchar* src, uchar* dst, పూర్ణాంకం లోతు, పూర్ణాంకం nf, పూర్ణాంకం *factors, ద్విగుణ scale, పూర్ణాంకం* itab, శూన్య* wave,
                         పూర్ణాంకం tab_కొలత, పూర్ణాంకం n, తార్కిక isInverse, తార్కిక no_క్రమమార్పు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_dftInit1D hal_ni_dftInit1D
#నిర్వచించు cv_hal_dft1D hal_ni_dft1D
#నిర్వచించు cv_hal_dftFree1D hal_ni_dftFree1D
#నిర్వచించు cv_hal_dft hal_ni_dft
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
అంతఃవరుస పూర్ణాంకం hal_ni_dftInit2D(cvhalDFT **context, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం లోతు, పూర్ణాంకం src_channels, పూర్ణాంకం dst_channels, పూర్ణాంకం flags, పూర్ణాంకం nonzero_అడ్డువరుసలు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
అంతఃవరుస పూర్ణాంకం hal_ni_dft2D(cvhalDFT *context, స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
అంతఃవరుస పూర్ణాంకం hal_ni_dftFree2D(cvhalDFT *context) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_dftInit2D hal_ni_dftInit2D
#నిర్వచించు cv_hal_dft2D hal_ni_dft2D
#నిర్వచించు cv_hal_dftFree2D hal_ni_dftFree2D
//! @endcond

/**
@param context double pointer to context storing all necessary data
@param width image width
@param height image height
@param depth image type (CV_32F or CV_64F)
@param flags algorithm options (combination of CV_HAL_DFT_INVERSE, ...)
 */
అంతఃవరుస పూర్ణాంకం hal_ni_dctInit2D(cvhalDFT **context, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం లోతు, పూర్ణాంకం flags) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
@param src_data source image data
@param src_step source image step
@param dst_data destination image data
@param dst_step destination image step
 */
అంతఃవరుస పూర్ణాంకం hal_ni_dct2D(cvhalDFT *context, స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు, uchar *dst_దత్తాంశం, size_t dst_అడుగు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
/**
@param context pointer to context storing all necessary data
 */
అంతఃవరుస పూర్ణాంకం hal_ni_dctFree2D(cvhalDFT *context) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_dctInit2D hal_ni_dctInit2D
#నిర్వచించు cv_hal_dct2D hal_ni_dct2D
#నిర్వచించు cv_hal_dctFree2D hal_ni_dctFree2D
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
అంతఃవరుస పూర్ణాంకం hal_ni_LU32f(దశాంశం* src1, size_t src1_అడుగు, పూర్ణాంకం m, దశాంశం* src2, size_t src2_అడుగు, పూర్ణాంకం n, పూర్ణాంకం* info) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_LU64f(ద్విగుణ* src1, size_t src1_అడుగు, పూర్ణాంకం m, ద్విగుణ* src2, size_t src2_అడుగు, పూర్ణాంకం n, పూర్ణాంకం* info) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_Cholesky32f(దశాంశం* src1, size_t src1_అడుగు, పూర్ణాంకం m, దశాంశం* src2, size_t src2_అడుగు, పూర్ణాంకం n, తార్కిక* info) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_Cholesky64f(ద్విగుణ* src1, size_t src1_అడుగు, పూర్ణాంకం m, ద్విగుణ* src2, size_t src2_అడుగు, పూర్ణాంకం n, తార్కిక* info) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_SVD32f(దశాంశం* src, size_t src_అడుగు, దశాంశం* w, దశాంశం* u, size_t u_అడుగు, దశాంశం* vt, size_t vt_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం flags) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_SVD64f(ద్విగుణ* src, size_t src_అడుగు, ద్విగుణ* w, ద్విగుణ* u, size_t u_అడుగు, ద్విగుణ* vt, size_t vt_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం flags) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_QR32f(దశాంశం* src1, size_t src1_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, దశాంశం* src2, size_t src2_అడుగు, దశాంశం* dst, పూర్ణాంకం* info) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_QR64f(ద్విగుణ* src1, size_t src1_అడుగు, పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, ద్విగుణ* src2, size_t src2_అడుగు, ద్విగుణ* dst, పూర్ణాంకం* info) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}



//! @cond IGNORED
#నిర్వచించు cv_hal_LU32f hal_ni_LU32f
#నిర్వచించు cv_hal_LU64f hal_ni_LU64f
#నిర్వచించు cv_hal_Cholesky32f hal_ni_Cholesky32f
#నిర్వచించు cv_hal_Cholesky64f hal_ni_Cholesky64f
#నిర్వచించు cv_hal_SVD32f hal_ni_SVD32f
#నిర్వచించు cv_hal_SVD64f hal_ni_SVD64f
#నిర్వచించు cv_hal_QR32f hal_ni_QR32f
#నిర్వచించు cv_hal_QR64f hal_ni_QR64f
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
అంతఃవరుస పూర్ణాంకం hal_ni_gemm32f(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
                          దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
                          పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_gemm64f(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
                          ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
                          పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_gemm32fc(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
                          దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
                          పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
అంతఃవరుస పూర్ణాంకం hal_ni_gemm64fc(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
                          ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
                          పూర్ణాంకం m, పూర్ణాంకం n, పూర్ణాంకం k, పూర్ణాంకం flags) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
//! @}

//! @cond IGNORED
#నిర్వచించు cv_hal_gemm32f hal_ni_gemm32f
#నిర్వచించు cv_hal_gemm64f hal_ni_gemm64f
#నిర్వచించు cv_hal_gemm32fc hal_ni_gemm32fc
#నిర్వచించు cv_hal_gemm64fc hal_ni_gemm64fc
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
అంతఃవరుస పూర్ణాంకం hal_ni_minMaxIdx(స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం లోతు, ద్విగుణ* minVal, ద్విగుణ* maxVal,
                            పూర్ణాంకం* minIdx, పూర్ణాంకం* maxIdx, uchar* ముసుగు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

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
అంతఃవరుస పూర్ణాంకం hal_ni_min_Max_Idx_ముసుగు_అడుగు(స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, పూర్ణాంకం లోతు, ద్విగుణ* minVal, ద్విగుణ* maxVal,
                                    పూర్ణాంకం* minIdx, పూర్ణాంకం* maxIdx, uchar* ముసుగు, size_t ముసుగు_అడుగు) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_minMaxIdx hal_ni_minMaxIdx
#నిర్వచించు cv_hal_min_Max_Idx_ముసుగు_అడుగు hal_ni_min_Max_Idx_ముసుగు_అడుగు
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
అంతఃవరుస పూర్ణాంకం hal_ni_సగటు_Std_Dev(స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                             పూర్ణాంకం src_type, ద్విగుణ* సగటు_val, ద్విగుణ* stddev_val, uchar* ముసుగు, size_t ముసుగు_అడుగు)
{ తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_సగటు_Std_Dev hal_ni_సగటు_Std_Dev
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
అంతఃవరుస పూర్ణాంకం hal_ni_dot_లబ్ధం(స్థిరరాశి uchar* a_దత్తాంశం, size_t a_అడుగు, స్థిరరాశి uchar* b_దత్తాంశం, size_t b_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                             పూర్ణాంకం type, ద్విగుణ *dot_val)
{ తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_dot_లబ్ధం hal_ni_dot_లబ్ధం
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
అంతఃవరుస పూర్ణాంకం hal_ni_flip(పూర్ణాంకం src_type, స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, పూర్ణాంకం src_వెడల్పు, పూర్ణాంకం src_ఎత్తు,
                       uchar* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం flip_బహుళకం) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_flip hal_ni_flip
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
అంతఃవరుస పూర్ణాంకం hal_ni_rotate90(పూర్ణాంకం src_type, స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, పూర్ణాంకం src_వెడల్పు, పూర్ణాంకం src_ఎత్తు,
                           uchar* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం angle) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_rotate90 hal_ni_rotate90
//! @endcond

/**
   @brief Transpose2d
   @param src_data,src_step Source image
   @param dst_data,dst_step Destination image
   @param src_width,src_height Source image dimensions
   @param element_size Size of an element in bytes
*/
అంతఃవరుస పూర్ణాంకం hal_ni_transpose2d(స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, uchar* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం src_వెడల్పు,
                              పూర్ణాంకం src_ఎత్తు, పూర్ణాంకం element_కొలత) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_transpose2d hal_ni_transpose2d
//! @endcond

/**
    @brief copyTo with mask
    @param src_data, src_step Source image
    @param dst_data, dst_step Destination image
    @param width, height Image dimensions of source, destination and mask
    @param type Type of source and destination images, for example CV_8UC1 or CV_32FC3
    @param mask_data, mask_step, mask_type Mask
*/
అంతఃవరుస పూర్ణాంకం hal_ni_copy_గమ్యం_Masked(స్థిరరాశి uchar* src_దత్తాంశం, size_t src_అడుగు, uchar* dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                             పూర్ణాంకం type, స్థిరరాశి uchar* ముసుగు_దత్తాంశం, size_t ముసుగు_అడుగు, పూర్ణాంకం ముసుగు_type)
{ తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_copy_గమ్యం_Masked hal_ni_copy_గమ్యం_Masked
//! @endcond

/**
 @ brief sum
 @param src_data Source image data
 @param src_step Source image step
 @param src_type Source image type
 @param width, height Source image dimensions
 @param result Pointer to save the sum result to.
 */
అంతఃవరుస పూర్ణాంకం hal_ni_కూడిక(స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు, పూర్ణాంకం src_type, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, ద్విగుణ *ఫలితం)
{ తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_కూడిక hal_ni_కూడిక
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
అంతఃవరుస పూర్ణాంకం hal_ni_లోపల_Range8u(స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు,
              uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం dst_లోతు_2, పూర్ణాంకం వెడల్పు,
              పూర్ణాంకం ఎత్తు, పూర్ణాంకం cn, uchar lower_bound, uchar upper_bound) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }
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
అంతఃవరుస పూర్ణాంకం hal_ni_లోపల_Range32f(స్థిరరాశి uchar *src_దత్తాంశం, size_t src_అడుగు,
              uchar *dst_దత్తాంశం, size_t dst_అడుగు, పూర్ణాంకం dst_లోతు_2, పూర్ణాంకం వెడల్పు,
              పూర్ణాంకం ఎత్తు, పూర్ణాంకం cn, ద్విగుణ lower_bound, ద్విగుణ upper_bound) { తిరిగి CV_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#నిర్వచించు cv_hal_లోపల_Range8u hal_ni_లోపల_Range8u
#నిర్వచించు cv_hal_లోపల_Range32f hal_ni_లోపల_Range32f
//! @endcond

//! @}

#ఒకవేళ defined(__clang__)
#సూచన clang diagnostic pop
#లేదావేళ defined(_MSC_VER)
#సూచన warning(pop)
#లేదావేళ defined(__GNUC__)
#సూచన GCC diagnostic pop
#అయితే_ముగింపు

#అన్తర్భావ "hal_internal.hpp"
#అన్తర్భావ "custom_hal.hpp"

//! @cond IGNORED

#నిర్వచించు CALL_HAL_RET2(name, fun, retval, ...) \
{ \
    పూర్ణాంకం res = __CV_EXPAND(fun(__VA_ARGS__)); \
    ఒకవేళ (res == CV_HAL_ERROR_OK) \
        తిరిగి retval; \
    లేకపోతే ఒకవేళ (res != CV_HAL_ERROR_NOT_IMPLEMENTED) \
        CV_Error_(cv::Error::Sts_అంతర్గత, \
        ("HAL implementation " CVAUX_STR(name) " ==> " CVAUX_STR(fun) " returned %d (0x%08x)", res, res)); \
}

#నిర్వచించు CALL_HAL_RET(name, fun, retval, ...) \
CALL_HAL_RET2(name, fun, retval, __VA_ARGS__, &retval)

#నిర్వచించు CALL_HAL(name, fun, ...) \
CALL_HAL_RET2(name, fun, ,__VA_ARGS__)

//! @endcond

#అయితే_ముగింపు
