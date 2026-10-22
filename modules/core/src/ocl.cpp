/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
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
// In no event shall the OpenCV Foundation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENCL
#ಅನ್ತರ್ಭಾವ "ocl_disabled.impl.hpp"
#ಇಲ್ಲದಿದ್ದರೆ // HAVE_OPENCL

#ಅನ್ತರ್ಭಾವ <list>
#ಅನ್ತರ್ಭಾವ <map>
#ಅನ್ತರ್ಭಾವ <deque>
#ಅನ್ತರ್ಭಾವ <set>
#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <fstream>
#ಆದರೆ !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_LOG_STRIP_LEVEL
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ಅನ್ತರ್ಭಾವ "opencv2/core/ocl_genbase.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/lock.private.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"

#ವ್ಯಾಖ್ಯಾನಿಸು CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ಖಾಲಿ, 0, "OpenCL allocator: " << __VA_ARGS__)
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/allocator_stats.impl.hpp"
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV__ALLOCATOR_STATS_LOG

#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SHOW_BUILD_OPTIONS             0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SHOW_BUILD_KERNELS             0

#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SHOW_ಸತತ_KERNELS               0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SYNC_ಸತತ_KERNELS               0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_TRACE_CHECK                    0

#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS       1

#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SHOW_SVM_LOG                   0

#ಅನ್ತರ್ಭಾವ "opencv2/core/bufferpool.hpp"
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LOG_ಬಫರ್_POOL
# ಆದರೆ 0
#   define LOG_ಬಫರ್_POOL ಬರೆ
# ಇಲ್ಲದಿದ್ದರೆ
#   define LOG_ಬಫರ್_POOL(...)
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SVM_TRACE_P ಬರೆ("line %d (ocl.cpp): ", __LINE__); ಬರೆ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SVM_TRACE_P(...)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SVM_TRACE_ERROR_P ಬರೆ("Error on line %d (ocl.cpp): ", __LINE__); ಬರೆ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_SVM_TRACE_ERROR_P(...)
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_core.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/opencl_svm.hpp"
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "umatrix.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ocl {

#ವ್ಯಾಖ್ಯಾನಿಸು IMPLEMENT_REFCOUNTABLE() \
    ಶೂನ್ಯ addref() { CV_XADD(&refcount, 1); } \
    ಶೂನ್ಯ release() { ಆದರೆ( CV_XADD(&refcount, -1) == 1 && !cv::__termination) ಅಳಿಸು ಇದು; } \
    ಪೂರ್ಣಾಂಕ refcount

ಸ್ಥಿರ cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    ಹಿಂತಿರುಗಿ opencl_allocator_stats;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _DEBUG
ಸ್ಥಿರ ತಾರ್ಕಿಕ isRaiseError()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ value = ಸುಳ್ಳು;
    ಆದರೆ (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", ಸುಳ್ಳು);
        initialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ value;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    ತಾರ್ಕಿಕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", ಸುಳ್ಳು);
    ಆದರೆ (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#ಆದರೆ CV_OPENCL_TRACE_CHECK
ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ traceOpenCLCheck(cl_int ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* message)
{
    std::ಹೊರಗೆ << "OpenCV(OpenCL:" << ಸ್ಥಿತಿ_2 << "): " << message << std::ಸಾಲು_ಅಂತ್ಯ << std::flush;
}
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_TRACE_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, message) traceOpenCLCheck(ಸ್ಥಿತಿ_2, message)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_TRACE_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, message) /* nothing */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_API_ERROR_ಸಂದೇಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_ಫಲಿತಾಂಶ), check_ಫಲಿತಾಂಶ, ಸಂದೇಶ)

#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_CHECK_ಫಲಿತಾಂಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ) \
    ಮಾಡು { \
        CV_OCL_TRACE_CHECK_ಫಲಿತಾಂಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ); \
        ಆದರೆ (check_ಫಲಿತಾಂಶ != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(ಸಂದೇಶ), ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_ಸಂದೇಶ_2 = CV_OCL_API_ERROR_ಸಂದೇಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ); \
            CV_Error(Error::OpenCLApiCallError, error_ಸಂದೇಶ_2); \
        } \
    } ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_CHECK_(ಅಭಿವ್ಯಕ್ತಿ, check_ಫಲಿತಾಂಶ) ಮಾಡು { ಅಭಿವ್ಯಕ್ತಿ; CV_OCL_CHECK_ಫಲಿತಾಂಶ(check_ಫಲಿತಾಂಶ, #expr); } ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_CHECK(ಅಭಿವ್ಯಕ್ತಿ) ಮಾಡು { cl_int __cl_result = (ಅಭಿವ್ಯಕ್ತಿ); CV_OCL_CHECK_ಫಲಿತಾಂಶ(__cl_result, #expr); } ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _DEBUG
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ) CV_OCL_CHECK_ಫಲಿತಾಂಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_DBG_CHECK(ಅಭಿವ್ಯಕ್ತಿ) CV_OCL_CHECK(ಅಭಿವ್ಯಕ್ತಿ)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_DBG_CHECK_(ಅಭಿವ್ಯಕ್ತಿ, check_ಫಲಿತಾಂಶ) CV_OCL_CHECK_(ಅಭಿವ್ಯಕ್ತಿ, check_ಫಲಿತಾಂಶ)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ) \
    ಮಾಡು { \
        CV_OCL_TRACE_CHECK_ಫಲಿತಾಂಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ); \
        ಆದರೆ (check_ಫಲಿತಾಂಶ != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(ಸಂದೇಶ), ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_ಸಂದೇಶ_2 = CV_OCL_API_ERROR_ಸಂದೇಶ(check_ಫಲಿತಾಂಶ, ಸಂದೇಶ); \
            CV_Error(Error::OpenCLApiCallError, error_ಸಂದೇಶ_2); \
        } \
    } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_DBG_CHECK_(ಅಭಿವ್ಯಕ್ತಿ, check_ಫಲಿತಾಂಶ) ಮಾಡು { ಅಭಿವ್ಯಕ್ತಿ; CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(check_ಫಲಿತಾಂಶ, #expr); } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_DBG_CHECK(ಅಭಿವ್ಯಕ್ತಿ) ಮಾಡು { cl_int __cl_result = (ಅಭಿವ್ಯಕ್ತಿ); CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(__cl_result, #expr); } ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ


ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", ನಿಜ);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", ನಿಜ);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ CV_OPENCL_CACHE_ಬೀಗ_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", ನಿಜ);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", ನಿಜ);

#ಆದರೆ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS_ಮೌಲ್ಯ = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", ಸುಳ್ಳು);
#ಆದರೆ_ಅಂತ್ಯ

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ CV_OPENCL_DISABLE_ಬಫರ್_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#ಆದರೆ 1 //def __APPLE__
        ನಿಜ
#ಇಲ್ಲದಿದ್ದರೆ
        ಸುಳ್ಳು
#ಆದರೆ_ಅಂತ್ಯ
);

ಸ್ಥಿರ String getBuildExtraOptions()
{
    ಸ್ಥಿರ String param_buildExtraOptions;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಆದರೆ (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = ನಿಜ;
        ಆದರೆ (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(ಖಾಲಿ, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    ಹಿಂತಿರುಗಿ param_buildExtraOptions;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", ನಿಜ);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


ರಚನೆ UMat2D
{
    UMat2D(ಸ್ಥಿರಾಂಕ UMat& m)
    {
        offset = (ಪೂರ್ಣಾಂಕ)m.offset;
        ಹೆಜ್ಜೆ = (ಪೂರ್ಣಾಂಕ)m.ಹೆಜ್ಜೆ;
        ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    }
    ಪೂರ್ಣಾಂಕ offset;
    ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ;
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು;
    ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
};

ರಚನೆ UMat3D
{
    UMat3D(ಸ್ಥಿರಾಂಕ UMat& m)
    {
        offset = (ಪೂರ್ಣಾಂಕ)m.offset;
        ಹೆಜ್ಜೆ = (ಪೂರ್ಣಾಂಕ)m.ಹೆಜ್ಜೆ.p[1];
        slicestep = (ಪೂರ್ಣಾಂಕ)m.ಹೆಜ್ಜೆ.p[0];
        slices = (ಪೂರ್ಣಾಂಕ)m.size.p[0];
        ಅಡ್ಡಸಾಲುಗಳು = m.size.p[1];
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.size.p[2];
    }
    ಪೂರ್ಣಾಂಕ offset;
    ಪೂರ್ಣಾಂಕ slicestep;
    ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ;
    ಪೂರ್ಣಾಂಕ slices;
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು;
    ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ಸ್ಥಿರ uint64 crc64( ಸ್ಥಿರಾಂಕ uchar* ದತ್ತಾಂಶ, size_t size, uint64 crc0=0 )
{
    ಸ್ಥಿರ uint64 table[256];
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;

    ಆದರೆ( !initialized )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < 256; i++ )
        {
            uint64 c = i;
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = ನಿಜ;
    }

    uint64 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ~crc0;
    ಗಾಗಿ( size_t idx = 0; idx < size; idx++ )
        ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = table[(uchar)ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ ^ ದತ್ತಾಂಶ[idx]] ^ (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 8);

    ಹಿಂತಿರುಗಿ ~ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
}

#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
ರಚನೆ OpenCLBinaryCacheConfigurator
{
    cv::String cache_ಮಾರ್ಗ;
    cv::String cache_ಬೀಗ_filename;
    cv::Ptr<utils::fs::File_ಬೀಗ> cache_ಬೀಗ;

    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(ಖಾಲಿ, "Initializing OpenCL cache configuration...");
        ಆದರೆ (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(ಖಾಲಿ, "OpenCL cache is disabled");
            ಹಿಂತಿರುಗಿ;
        }
        cache_ಮಾರ್ಗ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        ಆದರೆ (cache_ಮಾರ್ಗ.empty())
        {
            CV_LOG_INFO(ಖಾಲಿ, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        ಮಾಡು
        {
            ಪ್ರಯತ್ನಿಸು
            {
                ಆದರೆ (cache_ಮಾರ್ಗ.empty())
                    ನಿಲ್ಲಿಸು;
                ಆದರೆ (cache_ಮಾರ್ಗ == "disabled")
                    ನಿಲ್ಲಿಸು;
                ಆದರೆ (!utils::fs::createDirectories(cache_ಮಾರ್ಗ))
                {
                    CV_LOG_DEBUG(ಖಾಲಿ, "Can't use OpenCL cache directory: " << cache_ಮಾರ್ಗ);
                    clear();
                    ನಿಲ್ಲಿಸು;
                }

                ಆದರೆ (CV_OPENCL_CACHE_ಬೀಗ_ENABLE)
                {
                    cache_ಬೀಗ_filename = cache_ಮಾರ್ಗ + ".lock";
                    ಆದರೆ (!utils::fs::exists(cache_ಬೀಗ_filename))
                    {
                        CV_LOG_DEBUG(ಖಾಲಿ, "Creating lock file... (" << cache_ಬೀಗ_filename << ")");
                        std::ofstream ಬೀಗ_filename(cache_ಬೀಗ_filename.c_str(), std::ios::out);
                        ಆದರೆ (!ಬೀಗ_filename.is_open())
                        {
                            CV_LOG_WARNING(ಖಾಲಿ, "Can't create lock file for OpenCL program cache: " << cache_ಬೀಗ_filename);
                            ನಿಲ್ಲಿಸು;
                        }
                    }

                    ಪ್ರಯತ್ನಿಸು
                    {
                        cache_ಬೀಗ = makePtr<utils::fs::File_ಬೀಗ>(cache_ಬೀಗ_filename.c_str());
                        CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Checking cache lock... (" << cache_ಬೀಗ_filename << ")");
                        {
                            utils::shared_ಬೀಗ_guard<utils::fs::File_ಬೀಗ> lock(*cache_ಬೀಗ);
                        }
                        CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Checking cache lock... Done!");
                    }
                    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
                    {
                        CV_LOG_WARNING(ಖಾಲಿ, "Can't create OpenCL program cache lock: " << cache_ಬೀಗ_filename << std::ಸಾಲು_ಅಂತ್ಯ << e.what());
                    }
                    ಹಿಡಿ (...)
                    {
                        CV_LOG_WARNING(ಖಾಲಿ, "Can't create OpenCL program cache lock: " << cache_ಬೀಗ_filename);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಆದರೆ (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(ಖಾಲಿ, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        CV_LOG_INFO(ಖಾಲಿ, "OpenCL cache lock is disabled");
                    }
                }
            }
            ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
            {
                CV_LOG_WARNING(ಖಾಲಿ, "Can't prepare OpenCL program cache: " << cache_ಮಾರ್ಗ << std::ಸಾಲು_ಅಂತ್ಯ << e.what());
                clear();
            }
        } ಆಗ (0);
        ಆದರೆ (!cache_ಮಾರ್ಗ.empty())
        {
            ಆದರೆ (cache_ಬೀಗ.empty() && CV_OPENCL_CACHE_ಬೀಗ_ENABLE)
            {
                CV_LOG_WARNING(ಖಾಲಿ, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_LOG_INFO(ಖಾಲಿ, "Successfully initialized OpenCL cache directory: " << cache_ಮಾರ್ಗ);
            }
        }
    }

    ಶೂನ್ಯ clear()
    {
        cache_ಮಾರ್ಗ.clear();
        cache_ಬೀಗ_filename.clear();
        cache_ಬೀಗ.release();
    }

    std::string prepareCacheDirectoryForContext(ಸ್ಥಿರಾಂಕ std::string& ctx_ಪೂರ್ವಪ್ರತ್ಯಯ,
            ಸ್ಥಿರಾಂಕ std::string& cleanup_ಪೂರ್ವಪ್ರತ್ಯಯ)
    {
        ಆದರೆ (cache_ಮಾರ್ಗ.empty())
            ಹಿಂತಿರುಗಿ std::string();

        Auto_ಬೀಗ lock(mutex_prepared_contexts_);

        ContextCacheType::iterator ಸಿಕ್ಕಿತು_it = prepared_contexts_.find(ctx_ಪೂರ್ವಪ್ರತ್ಯಯ);
        ಆದರೆ (ಸಿಕ್ಕಿತು_it != prepared_contexts_.end())
            ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು_it->second;

        CV_LOG_INFO(ಖಾಲಿ, "Preparing OpenCL cache configuration for context: " << ctx_ಪೂರ್ವಪ್ರತ್ಯಯ);

        std::string ಗುರಿ_directory = cache_ಮಾರ್ಗ + ctx_ಪೂರ್ವಪ್ರತ್ಯಯ + "/";
        ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = utils::fs::isDirectory(ಗುರಿ_directory);
        ಆದರೆ (!ಫಲಿತಾಂಶ)
        {
            ಪ್ರಯತ್ನಿಸು
            {
                CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Creating directory: " << ಗುರಿ_directory);
                ಆದರೆ (utils::fs::createDirectories(ಗುರಿ_directory))
                {
                    ಫಲಿತಾಂಶ = ನಿಜ;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    CV_LOG_WARNING(ಖಾಲಿ, "Can't create directory: " << ಗುರಿ_directory);
                }
            }
            ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
            {
                CV_LOG_ERROR(ಖಾಲಿ, "Can't create OpenCL program cache directory for context: " << ಗುರಿ_directory << std::ಸಾಲು_ಅಂತ್ಯ << e.what());
            }
        }
        ಗುರಿ_directory = ಫಲಿತಾಂಶ ? ಗುರಿ_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_ಪೂರ್ವಪ್ರತ್ಯಯ, ಗುರಿ_directory));

        ಆದರೆ (ಫಲಿತಾಂಶ && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_ಪೂರ್ವಪ್ರತ್ಯಯ.empty())
        {
            ಪ್ರಯತ್ನಿಸು
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_ಮಾರ್ಗ, cleanup_ಪೂರ್ವಪ್ರತ್ಯಯ + "*", entries, ಸುಳ್ಳು, ನಿಜ);
                std::vector<String> remove_entries;
                ಗಾಗಿ (size_t i = 0; i < entries.size(); i++)
                {
                    ಸ್ಥಿರಾಂಕ String& name = entries[i];
                    ಆದರೆ (0 == name.find(cleanup_ಪೂರ್ವಪ್ರತ್ಯಯ))
                    {
                        ಆದರೆ (0 == name.find(ctx_ಪೂರ್ವಪ್ರತ್ಯಯ))
                            ಮುಂದುವರಿ; // skip current
                        remove_entries.push_back(name);
                    }
                }
                ಆದರೆ (!remove_entries.empty())
                {
                    CV_LOG_WARNING(ಖಾಲಿ, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(ಖಾಲಿ, "Trying to remove these directories...");
                    ಗಾಗಿ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(ಖಾಲಿ, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(ಖಾಲಿ, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    ಗಾಗಿ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ಸ್ಥಿರಾಂಕ String& name = remove_entries[i];
                        cv::String ಮಾರ್ಗ = utils::fs::join(cache_ಮಾರ್ಗ, name);
                        ಪ್ರಯತ್ನಿಸು
                        {
                            utils::fs::remove_all(ಮಾರ್ಗ);
                            CV_LOG_WARNING(ಖಾಲಿ, "Removed: " << ಮಾರ್ಗ);
                        }
                        ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
                        {
                            CV_LOG_ERROR(ಖಾಲಿ, "Exception during removal of obsolete OpenCL cache directory: " << ಮಾರ್ಗ << std::ಸಾಲು_ಅಂತ್ಯ << e.what());
                        }
                    }
                }
            }
            ಹಿಡಿ (...)
            {
                CV_LOG_WARNING(ಖಾಲಿ, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(ಖಾಲಿ, 1, "  Result: " << (ಗುರಿ_directory.empty() ? std::string("Failed") : ಗುರಿ_directory));
        ಹಿಂತಿರುಗಿ ಗುರಿ_directory;
    }

    ಸ್ಥಿರ OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, ಹೊಸ OpenCLBinaryCacheConfigurator());
    }
};
ವರ್ಗ ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ_File
{
    ಎಣಿಕೆ { MAX_ENTRIES = 64 };

    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ uint32_t;

    ರಚನೆ CV_DECL_ALIGNED(4) File_ಶೀರ್ಷಿಕೆ
    {
        uint32_t ಮೂಲ_Signature_ಗಾತ್ರ;
        //char sourceSignature[];
    };

    ರಚನೆ CV_DECL_ALIGNED(4) File_ಕೋಷ್ಟಕ
    {
        uint32_t ಸಂಖ್ಯೆ_ದ_Entries;
        //uint32_t firstEntryOffset[];
    };

    ರಚನೆ CV_DECL_ALIGNED(4) File_ನಮೂದು
    {
        uint32_t ಮುಂದಿನ_ನಮೂದು_File_ಸ್ಥಾನಾಂತರ; // 0 for the last entry in chain
        uint32_t ಕೀಲಿ_ಗಾತ್ರ;
        uint32_t ದತ್ತಾಂಶ_ಗಾತ್ರ;
        //char key[];
        //char data[];
    };

    ಸ್ಥಿರಾಂಕ std::string fileName_;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ ಮೂಲ_Signature;
    ಸ್ಥಿರಾಂಕ size_t ಮೂಲ_Signature_ಗಾತ್ರ_2;

    std::fstream f;

    uint32_t ನಮೂದು_Offsets[MAX_ENTRIES];

    uint32_t getHash(ಸ್ಥಿರಾಂಕ std::string& options)
    {
        uint64 hash = crc64((ಸ್ಥಿರಾಂಕ uchar*)options.c_str(), options.size(), 0);
        ಹಿಂತಿರುಗಿ hash & (MAX_ENTRIES - 1);
    }

    ಸಾಲೊಳಗೆ size_t get_File_ಗಾತ್ರ()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_ಗಾತ್ರ = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        ಹಿಂತಿರುಗಿ file_ಗಾತ್ರ;
    }
    ಸಾಲೊಳಗೆ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.ಪಠ((ಅಕ್ಷರ*)&res, ಗಾತ್ರ(uint32_t));
        CV_Assert(!f.fail());
        ಹಿಂತಿರುಗಿ res;
    }
    ಸಾಲೊಳಗೆ ಶೂನ್ಯ writeUInt32(ಸ್ಥಿರಾಂಕ uint32_t value)
    {
        uint32_t v = value;
        f.ಲಿಖ((ಅಕ್ಷರ*)&v, ಗಾತ್ರ(uint32_t));
        CV_Assert(!f.fail());
    }

    ಸಾಲೊಳಗೆ ಶೂನ್ಯ seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    ಸಾಲೊಳಗೆ ಶೂನ್ಯ seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    ಸಾಲೊಳಗೆ ಶೂನ್ಯ seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    ಶೂನ್ಯ clearFile()
    {
        f.close();
        ಆದರೆ (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(ಖಾಲಿ, "Can't remove: " << fileName_);
        ಹಿಂತಿರುಗಿ;
    }

ಸಾರ್ವಜನಿಕ:
    ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ_File(ಸ್ಥಿರಾಂಕ std::string& fileName, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಮೂಲ_Signature_2)
        : fileName_(fileName), ಮೂಲ_Signature(ಮೂಲ_Signature_2), ಮೂಲ_Signature_ಗಾತ್ರ_2(ಮೂಲ_Signature ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮೂಲ_Signature) : 0)
    {
        CV_StaticAssert(ಗಾತ್ರ(uint32_t) == 4, "");
        CV_Assert(ಮೂಲ_Signature != ಖಾಲಿ);
        CV_Assert(ಮೂಲ_Signature_ಗಾತ್ರ_2 > 0);
        memset(ನಮೂದು_Offsets, 0, ಗಾತ್ರ(ನಮೂದು_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        ಆದರೆ(f.is_open() && get_File_ಗಾತ್ರ() > 0)
        {
            ತಾರ್ಕಿಕ isValid = ಸುಳ್ಳು;
            ಪ್ರಯತ್ನಿಸು
            {
                uint32_t file_ಮೂಲ_Signature_ಗಾತ್ರ = readUInt32();
                ಆದರೆ (file_ಮೂಲ_Signature_ಗಾತ್ರ == ಮೂಲ_Signature_ಗಾತ್ರ_2)
                {
                    cv::Auto_ಬಫರ್<ಅಕ್ಷರ> file_ಮೂಲ_Signature(file_ಮೂಲ_Signature_ಗಾತ್ರ + 1);
                    f.ಪಠ(file_ಮೂಲ_Signature.ದತ್ತಾಂಶ(), file_ಮೂಲ_Signature_ಗಾತ್ರ);
                    ಆದರೆ (f.eof())
                    {
                        CV_LOG_ERROR(ಖಾಲಿ, "Unexpected EOF");
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (memcmp(ಮೂಲ_Signature_2, file_ಮೂಲ_Signature.ದತ್ತಾಂಶ(), file_ಮೂಲ_Signature_ಗಾತ್ರ) == 0)
                    {
                        isValid = ನಿಜ;
                    }
                }
                ಆದರೆ (!isValid)
                {
                    CV_LOG_ERROR(ಖಾಲಿ, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
            {
                CV_LOG_ERROR(ಖಾಲಿ, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            ಹಿಡಿ (...)
            {
                CV_LOG_ERROR(ಖಾಲಿ, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            ಆದರೆ (!isValid)
            {
                clearFile();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                seekReadAbsolute(0);
            }
        }
    }

    ತಾರ್ಕಿಕ ಪಠ(ಸ್ಥಿರಾಂಕ std::string& ಕೀಲಿ, std::vector<ಅಕ್ಷರ>& buf)
    {
        ಆದರೆ (!f.is_open())
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        size_t file_ಗಾತ್ರ = get_File_ಗಾತ್ರ();
        ಆದರೆ (file_ಗಾತ್ರ == 0)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "Invalid file (empty): " << fileName_);
            clearFile();
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ಮೂಲ_Signature_ಗಾತ್ರ = readUInt32();
        CV_Assert(file_ಮೂಲ_Signature_ಗಾತ್ರ > 0);
        seekReadRelative(file_ಮೂಲ_Signature_ಗಾತ್ರ);

        uint32_t ಸಂಖ್ಯೆ_ದ_Entries = readUInt32();
        CV_Assert(ಸಂಖ್ಯೆ_ದ_Entries > 0);
        ಆದರೆ (ಸಂಖ್ಯೆ_ದ_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "Invalid file: " << fileName_);
            clearFile();
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        f.ಪಠ((ಅಕ್ಷರ*)&ನಮೂದು_Offsets[0], ಗಾತ್ರ(ನಮೂದು_Offsets));
        CV_Assert(!f.fail());

        uint32_t ನಮೂದು_ಅಂಕೆ = getHash(ಕೀಲಿ);

        uint32_t ನಮೂದು_ಸ್ಥಾನಾಂತರ = ನಮೂದು_Offsets[ನಮೂದು_ಅಂಕೆ];
        File_ನಮೂದು ನಮೂದು;
        ಆಗ (ನಮೂದು_ಸ್ಥಾನಾಂತರ > 0)
        {
            seekReadAbsolute(ನಮೂದು_ಸ್ಥಾನಾಂತರ);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.ಪಠ((ಅಕ್ಷರ*)&ನಮೂದು, ಗಾತ್ರ(ನಮೂದು));
            CV_Assert(!f.fail());
            cv::Auto_ಬಫರ್<ಅಕ್ಷರ> file_ಕೀಲಿ(ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ + 1);
            ಆದರೆ (ಕೀಲಿ.size() == ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ)
            {
                ಆದರೆ (ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ > 0)
                {
                    f.ಪಠ(file_ಕೀಲಿ.ದತ್ತಾಂಶ(), ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ);
                    CV_Assert(!f.fail());
                }
                ಆದರೆ (memcmp(file_ಕೀಲಿ.ದತ್ತಾಂಶ(), ಕೀಲಿ.c_str(), ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ) == 0)
                {
                    buf.resize(ನಮೂದು.ದತ್ತಾಂಶ_ಗಾತ್ರ);
                    f.ಪಠ(&buf[0], ನಮೂದು.ದತ್ತಾಂಶ_ಗಾತ್ರ);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Read...");
                    ಹಿಂತಿರುಗಿ ನಿಜ;
                }
            }
            ಆದರೆ (ನಮೂದು.ಮುಂದಿನ_ನಮೂದು_File_ಸ್ಥಾನಾಂತರ == 0)
                ನಿಲ್ಲಿಸು;
            ನಮೂದು_ಸ್ಥಾನಾಂತರ = ನಮೂದು.ಮುಂದಿನ_ನಮೂದು_File_ಸ್ಥಾನಾಂತರ;
        }
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ತಾರ್ಕಿಕ ಲಿಖ(ಸ್ಥಿರಾಂಕ std::string& ಕೀಲಿ, std::vector<ಅಕ್ಷರ>& buf)
    {
        ಆದರೆ (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            ಆದರೆ (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                ಆದರೆ (!f.is_open())
                {
                    CV_LOG_ERROR(ಖಾಲಿ, "Can't create file: " << fileName_);
                    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
                }
            }
        }

        size_t file_ಗಾತ್ರ = get_File_ಗಾತ್ರ();
        ಆದರೆ (file_ಗಾತ್ರ == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)ಮೂಲ_Signature_ಗಾತ್ರ_2);
            f.ಲಿಖ(ಮೂಲ_Signature, ಮೂಲ_Signature_ಗಾತ್ರ_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(ನಮೂದು_Offsets, 0, ಗಾತ್ರ(ನಮೂದು_Offsets));
            f.ಲಿಖ((ಅಕ್ಷರ*)ನಮೂದು_Offsets, ಗಾತ್ರ(ನಮೂದು_Offsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            file_ಗಾತ್ರ = get_File_ಗಾತ್ರ();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ಮೂಲ_Signature_ಗಾತ್ರ = readUInt32();
        CV_Assert(file_ಮೂಲ_Signature_ಗಾತ್ರ == ಮೂಲ_Signature_ಗಾತ್ರ_2);
        seekReadRelative(file_ಮೂಲ_Signature_ಗಾತ್ರ);

        uint32_t ಸಂಖ್ಯೆ_ದ_Entries = readUInt32();
        CV_Assert(ಸಂಖ್ಯೆ_ದ_Entries > 0);
        ಆದರೆ (ಸಂಖ್ಯೆ_ದ_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "Invalid file: " << fileName_);
            clearFile();
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        size_t ಕೋಷ್ಟಕ_Entries_ಸ್ಥಾನಾಂತರ = (size_t)f.tellg();
        f.ಪಠ((ಅಕ್ಷರ*)&ನಮೂದು_Offsets[0], ಗಾತ್ರ(ನಮೂದು_Offsets));
        CV_Assert(!f.fail());

        uint32_t ನಮೂದು_ಅಂಕೆ = getHash(ಕೀಲಿ);

        uint32_t ನಮೂದು_ಸ್ಥಾನಾಂತರ = ನಮೂದು_Offsets[ನಮೂದು_ಅಂಕೆ];
        File_ನಮೂದು ನಮೂದು;
        ಆಗ (ನಮೂದು_ಸ್ಥಾನಾಂತರ > 0)
        {
            seekReadAbsolute(ನಮೂದು_ಸ್ಥಾನಾಂತರ);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.ಪಠ((ಅಕ್ಷರ*)&ನಮೂದು, ಗಾತ್ರ(ನಮೂದು));
            CV_Assert(!f.fail());
            cv::Auto_ಬಫರ್<ಅಕ್ಷರ> file_ಕೀಲಿ(ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ + 1);
            ಆದರೆ (ಕೀಲಿ.size() == ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ)
            {
                ಆದರೆ (ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ > 0)
                {
                    f.ಪಠ(file_ಕೀಲಿ.ದತ್ತಾಂಶ(), ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ);
                    CV_Assert(!f.fail());
                }
                ಆದರೆ (0 == memcmp(file_ಕೀಲಿ.ದತ್ತಾಂಶ(), ಕೀಲಿ.c_str(), ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ))
                {
                    // duplicate
                    CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Duplicate key ignored: " << fileName_);
                    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
                }
            }
            ಆದರೆ (ನಮೂದು.ಮುಂದಿನ_ನಮೂದು_File_ಸ್ಥಾನಾಂತರ == 0)
                ನಿಲ್ಲಿಸು;
            ನಮೂದು_ಸ್ಥಾನಾಂತರ = ನಮೂದು.ಮುಂದಿನ_ನಮೂದು_File_ಸ್ಥಾನಾಂತರ;
        }
        seekReadAbsolute(0);
        ಆದರೆ (ನಮೂದು_ಸ್ಥಾನಾಂತರ > 0)
        {
            seekWriteAbsolute(ನಮೂದು_ಸ್ಥಾನಾಂತರ);
            ನಮೂದು.ಮುಂದಿನ_ನಮೂದು_File_ಸ್ಥಾನಾಂತರ = (uint32_t)file_ಗಾತ್ರ;
            f.ಲಿಖ((ಅಕ್ಷರ*)&ನಮೂದು, ಗಾತ್ರ(ನಮೂದು));
            CV_Assert(!f.fail());
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ನಮೂದು_Offsets[ನಮೂದು_ಅಂಕೆ] = (uint32_t)file_ಗಾತ್ರ;
            seekWriteAbsolute(ಕೋಷ್ಟಕ_Entries_ಸ್ಥಾನಾಂತರ);
            f.ಲಿಖ((ಅಕ್ಷರ*)ನಮೂದು_Offsets, ಗಾತ್ರ(ನಮೂದು_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_ಗಾತ್ರ);
        ನಮೂದು.ಮುಂದಿನ_ನಮೂದು_File_ಸ್ಥಾನಾಂತರ = 0;
        ನಮೂದು.ದತ್ತಾಂಶ_ಗಾತ್ರ = (uint32_t)buf.size();
        ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ = (uint32_t)ಕೀಲಿ.size();
        f.ಲಿಖ((ಅಕ್ಷರ*)&ನಮೂದು, ಗಾತ್ರ(ನಮೂದು));
        CV_Assert(!f.fail());
        f.ಲಿಖ(ಕೀಲಿ.c_str(), ನಮೂದು.ಕೀಲಿ_ಗಾತ್ರ);
        CV_Assert(!f.fail());
        f.ಲಿಖ(&buf[0], ನಮೂದು.ದತ್ತಾಂಶ_ಗಾತ್ರ);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Write... (" << buf.size() << " bytes)");
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
};
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT



ರಚನೆ OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    ಪೂರ್ಣಾಂಕ device_;  // device index in context
    ocl::ಸರತಿ ಸರತಿ_3;
    ಪೂರ್ಣಾಂಕ useOpenCL_;

ಸಂರಕ್ಷಿತ:
    Impl() = ಅಳಿಸು;

    ಶೂನ್ಯ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        ಪೂರ್ಣಾಂಕ ndevices = (ಪೂರ್ಣಾಂಕ)context_.ndevices();
        CV_Assert(ndevices > 0);
        ತಾರ್ಕಿಕ ಸಿಕ್ಕಿತು = ಸುಳ್ಳು;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            ಆದರೆ (dhandle == deviceID)
            {
                device_ = i;
                ಸಿಕ್ಕಿತು = ನಿಜ;
                ನಿಲ್ಲಿಸು;
            }
        }
        CV_Assert(ಸಿಕ್ಕಿತು && "OpenCL device can't work with passed OpenCL context");
    }

    ಶೂನ್ಯ _init_device(ಸ್ಥಿರಾಂಕ ocl::Device& device)
    {
        CV_Assert(device.ptr());
        ಪೂರ್ಣಾಂಕ ndevices = (ಪೂರ್ಣಾಂಕ)context_.ndevices();
        CV_Assert(ndevices > 0);
        ತಾರ್ಕಿಕ ಸಿಕ್ಕಿತು = ಸುಳ್ಳು;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            ಆದರೆ (d.getImpl() == device.getImpl())
            {
                device_ = i;
                ಸಿಕ್ಕಿತು = ನಿಜ;
                ನಿಲ್ಲಿಸು;
            }
        }
        CV_Assert(ಸಿಕ್ಕಿತು && "OpenCL device can't work with passed OpenCL context");
    }

ಸಾರ್ವಜನಿಕ:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_ಬಳಸದ(platformID);
        CV_Assert(context);
        CV_Assert(deviceID);

        context_ = Context::fromHandle(context);
        _init_device(deviceID);
        ಸರತಿ_3 = ಸರತಿ(context_, context_.device(device_));
    }

    Impl(ಸ್ಥಿರಾಂಕ ocl::Context& context, ಸ್ಥಿರಾಂಕ ocl::Device& device, ಸ್ಥಿರಾಂಕ ocl::ಸರತಿ& ಸರತಿ_2)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        ಸರತಿ_3 = ಸರತಿ_2;
    }

    Impl(ಸ್ಥಿರಾಂಕ ocl::Context& context, ಸ್ಥಿರಾಂಕ ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        ಸರತಿ_3 = ಸರತಿ(context_, context_.device(device_));
    }

    Impl(ಸ್ಥಿರಾಂಕ ocl::Context& context, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ device, ಸ್ಥಿರಾಂಕ ocl::ಸರತಿ& ಸರತಿ_2)
        : context_(context)
        , device_(device)
        , ಸರತಿ_3(ಸರತಿ_2)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ಸ್ಥಿರಾಂಕ Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , ಸರತಿ_3(other.ಸರತಿ_3)
        , useOpenCL_(-1)
    {
        // nothing
    }

    ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ useOpenCL() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ const_cast<Impl*>(ಇದು)->useOpenCL(); }
    ತಾರ್ಕಿಕ useOpenCL()
    {
        ಆದರೆ (useOpenCL_ < 0)
        {
            ಪ್ರಯತ್ನಿಸು
            {
                useOpenCL_ = 0;
                ಆದರೆ (!context_.empty() && context_.ndevices() > 0)
                {
                    ಸ್ಥಿರಾಂಕ Device& d = context_.device(device_);
                    useOpenCL_ = d.ಲಭ್ಯ();
                }
            }
            ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception&)
            {
                // nothing
            }
            ಆದರೆ (!useOpenCL_)
                CV_LOG_INFO(ಖಾಲಿ, "OpenCL: can't use OpenCL execution context");
        }
        ಹಿಂತಿರುಗಿ useOpenCL_ > 0;
    }

    ಶೂನ್ಯ ಗಣ_Use_Open_CL(ತಾರ್ಕಿಕ flag)
    {
        ಆದರೆ (!flag)
            useOpenCL_ = 0;
        ಇಲ್ಲದಿದ್ದರೆ
            useOpenCL_ = -1;
    }

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(ಖಾಲಿ, "OpenCL: initializing thread execution context");

        ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
        ಸ್ಥಿರ std::shared_ptr<Impl> g_primaryExecutionContext;

        ಆದರೆ (!initialized)
        {
            cv::Auto_ಬೀಗ lock(getInitializationMutex());
            ಆದರೆ (!initialized)
            {
                CV_LOG_INFO(ಖಾಲಿ, "OpenCL: creating new execution context...");
                ಪ್ರಯತ್ನಿಸು
                {
                    Context c = ocl::Context::create(std::string());
                    ಆದರೆ (c.ndevices())
                    {
                        ಪೂರ್ಣಾಂಕ deviceId = 0;
                        ಸ್ವಯಂಚಾಲಿತ& d = c.device(deviceId);
                        ಆದರೆ (d.ಲಭ್ಯ())
                        {
                            ಸ್ವಯಂಚಾಲಿತ q = ocl::ಸರತಿ(c, d);
                            ಆದರೆ (!q.ptr())
                            {
                                CV_LOG_ERROR(ಖಾಲಿ, "OpenCL: Can't create default OpenCL queue");
                            }
                            ಇಲ್ಲದಿದ್ದರೆ
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(ಖಾಲಿ, "OpenCL: device=" << d.name());
                            }
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            CV_LOG_ERROR(ಖಾಲಿ, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        CV_LOG_INFO(ಖಾಲಿ, "OpenCL: context is not available/disabled");
                    }
                }
                ಹಿಡಿ (ಸ್ಥಿರಾಂಕ std::exception& e)
                {
                    CV_LOG_INFO(ಖಾಲಿ, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                ಹಿಡಿ (...)
                {
                    CV_LOG_WARNING(ಖಾಲಿ, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = ನಿಜ;
            }
        }
        ಹಿಂತಿರುಗಿ g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->context_;
}
Device& OpenCLExecutionContext::getDevice() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->context_.device(p->device_);
}
ಸರತಿ& OpenCLExecutionContext::get_ಸರತಿ() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->ಸರತಿ_3;
}

ತಾರ್ಕಿಕ OpenCLExecutionContext::useOpenCL() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (p)
        ಹಿಂತಿರುಗಿ p->useOpenCL();
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}
ಶೂನ್ಯ OpenCLExecutionContext::ಗಣ_Use_Open_CL(ತಾರ್ಕಿಕ flag)
{
    CV_Assert(p);
    p->ಗಣ_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
    OpenCLExecutionContext& c = ದತ್ತಾಂಶ.oclExecutionContext;
    ಆದರೆ (!ದತ್ತಾಂಶ.oclExecutionContextInitialized)
    {
        ದತ್ತಾಂಶ.oclExecutionContextInitialized = ನಿಜ;
        ಆದರೆ (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    ಹಿಂತಿರುಗಿ c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
    OpenCLExecutionContext& c = ದತ್ತಾಂಶ.oclExecutionContext;
    ಹಿಂತಿರುಗಿ c;
}

ಶೂನ್ಯ OpenCLExecutionContext::bind() ಸ್ಥಿರಾಂಕ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
    ದತ್ತಾಂಶ.oclExecutionContext = *ಇದು;
    ದತ್ತಾಂಶ.oclExecutionContextInitialized = ನಿಜ;
    ದತ್ತಾಂಶ.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_ಸರತಿ() ಸ್ಥಿರಾಂಕ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ಸ್ಥಿರಾಂಕ ಸರತಿ q(getContext(), getDevice());
    ಹಿಂತಿರುಗಿ clone_With_New_ಸರತಿ(q);
}

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_ಸರತಿ(ಸ್ಥಿರಾಂಕ ocl::ಸರತಿ& q) ಸ್ಥಿರಾಂಕ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != ಖಾಲಿ);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    ಹಿಂತಿರುಗಿ c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ಸ್ಥಿರಾಂಕ Context& context, ಸ್ಥಿರಾಂಕ Device& device, ಸ್ಥಿರಾಂಕ ocl::ಸರತಿ& ಸರತಿ_2)
{
    CV_TRACE_FUNCTION();
    ಆದರೆ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, ಸರತಿ_2);
    ಹಿಂತಿರುಗಿ ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ಸ್ಥಿರಾಂಕ Context& context, ಸ್ಥಿರಾಂಕ Device& device)
{
    CV_TRACE_FUNCTION();
    ಆದರೆ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    ಹಿಂತಿರುಗಿ ctx;

}

ಶೂನ್ಯ OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isOpenCLInitialized = ಸುಳ್ಳು;
ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isOpenCLAvailable = ಸುಳ್ಳು;

ತಾರ್ಕಿಕ haveOpenCL()
{
    CV_TRACE_FUNCTION();

    ಆದರೆ (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string env_ಮಾರ್ಗ = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        ಆದರೆ (!env_ಮಾರ್ಗ.empty())
        {
            ಆದರೆ (env_ಮಾರ್ಗ == "disabled")
            {
                g_isOpenCLAvailable = ಸುಳ್ಳು;
                g_isOpenCLInitialized = ನಿಜ;
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        }

        cv::Auto_ಬೀಗ lock(getInitializationMutex());
        CV_LOG_INFO(ಖಾಲಿ, "Initialize OpenCL runtime...");
        ಪ್ರಯತ್ನಿಸು
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, ಖಾಲಿ, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(ಖಾಲಿ, "OpenCL: found " << n << " platforms");
        }
        ಹಿಡಿ (...)
        {
            g_isOpenCLAvailable = ಸುಳ್ಳು;
        }
        g_isOpenCLInitialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ g_isOpenCLAvailable;
}

ತಾರ್ಕಿಕ useOpenCL()
{
    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
    ಆದರೆ (ದತ್ತಾಂಶ.useOpenCL < 0)
    {
        ಪ್ರಯತ್ನಿಸು
        {
            ದತ್ತಾಂಶ.useOpenCL = 0;
            ಆದರೆ (haveOpenCL())
            {
                ಸ್ವಯಂಚಾಲಿತ c = OpenCLExecutionContext::getCurrent();
                ದತ್ತಾಂಶ.useOpenCL = c.useOpenCL();
            }
        }
        ಹಿಡಿ (...)
        {
            CV_LOG_INFO(ಖಾಲಿ, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ.useOpenCL > 0;
}

ತಾರ್ಕಿಕ isOpenCLActivated()
{
    ಆದರೆ (!g_isOpenCLAvailable)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    ಹಿಂತಿರುಗಿ useOpenCL();
}

ಶೂನ್ಯ ಗಣ_Use_Open_CL(ತಾರ್ಕಿಕ flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
    ಸ್ವಯಂಚಾಲಿತ& c = OpenCLExecutionContext::getCurrentRef();
    ಆದರೆ (!c.empty())
    {
        c.ಗಣ_Use_Open_CL(flag);
        ದತ್ತಾಂಶ.useOpenCL = c.useOpenCL();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (!flag)
            ದತ್ತಾಂಶ.useOpenCL = 0;
        ಇಲ್ಲದಿದ್ದರೆ
            ದತ್ತಾಂಶ.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CLAMDBLAS

ವರ್ಗ AmdBlasHelper
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, ಹೊಸ AmdBlasHelper())
    }

    ತಾರ್ಕಿಕ is_ಲಭ್ಯ() ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ g_is_Amd_Blas_ಲಭ್ಯ;
    }

    ~AmdBlasHelper()
    {
        // Do not tear down clBLAS.
        // The user application may still use clBLAS even after OpenCV is unloaded.
        /*try
        {
            clblasTeardown();
        }
        catch (...) { }*/
    }

ಸಂರಕ್ಷಿತ:
    AmdBlasHelper()
    {
        ಆದರೆ (!g_isAmdBlasInitialized)
        {
            Auto_ಬೀಗ lock(getInitializationMutex());

            ಆದರೆ (!g_isAmdBlasInitialized)
            {
                ಆದರೆ (haveOpenCL())
                {
                    ಪ್ರಯತ್ನಿಸು
                    {
                        g_is_Amd_Blas_ಲಭ್ಯ = clblas_ಸಿದ್ಧತೆ() == clblasSuccess;
                    }
                    ಹಿಡಿ (...)
                    {
                        g_is_Amd_Blas_ಲಭ್ಯ = ಸುಳ್ಳು;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    g_is_Amd_Blas_ಲಭ್ಯ = ಸುಳ್ಳು;

                g_isAmdBlasInitialized = ನಿಜ;
            }
        }
    }

ಖಾಸಗಿ:
    ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isAmdBlasInitialized;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ g_is_Amd_Blas_ಲಭ್ಯ;
};

ತಾರ್ಕಿಕ AmdBlasHelper::g_is_Amd_Blas_ಲಭ್ಯ = ಸುಳ್ಳು;
ತಾರ್ಕಿಕ AmdBlasHelper::g_isAmdBlasInitialized = ಸುಳ್ಳು;

ತಾರ್ಕಿಕ haveAmdBlas()
{
    ಹಿಂತಿರುಗಿ AmdBlasHelper::getInstance().is_ಲಭ್ಯ();
}

#ಇಲ್ಲದಿದ್ದರೆ

ತಾರ್ಕಿಕ haveAmdBlas()
{
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CLAMDFFT

ವರ್ಗ AmdFftHelper
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, ಹೊಸ AmdFftHelper())
    }

    ತಾರ್ಕಿಕ is_ಲಭ್ಯ() ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ g_is_Amd_Fft_ಲಭ್ಯ;
    }

    ~AmdFftHelper()
    {
        // Do not tear down clFFT.
        // The user application may still use clFFT even after OpenCV is unloaded.
        /*try
        {
            clfftTeardown();
        }
        catch (...) { }*/
    }

ಸಂರಕ್ಷಿತ:
    AmdFftHelper()
    {
        ಆದರೆ (!g_isAmdFftInitialized)
        {
            Auto_ಬೀಗ lock(getInitializationMutex());

            ಆದರೆ (!g_isAmdFftInitialized)
            {
                ಆದರೆ (haveOpenCL())
                {
                    ಪ್ರಯತ್ನಿಸು
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_ಸಿದ್ಧತೆ_ದತ್ತಾಂಶ(&ಸಿದ್ಧತೆ_ದತ್ತಾಂಶ) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_ಆವೃತ್ತಿ(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_ಲಭ್ಯ = ನಿಜ;
                    }
                    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ Exception &)
                    {
                        g_is_Amd_Fft_ಲಭ್ಯ = ಸುಳ್ಳು;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    g_is_Amd_Fft_ಲಭ್ಯ = ಸುಳ್ಳು;

                g_isAmdFftInitialized = ನಿಜ;
            }
        }
    }

ಖಾಸಗಿ:
    ಸ್ಥಿರ clfft_ಸಿದ್ಧತೆ_ದತ್ತಾಂಶ ಸಿದ್ಧತೆ_ದತ್ತಾಂಶ;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isAmdFftInitialized;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ g_is_Amd_Fft_ಲಭ್ಯ;
};

clfft_ಸಿದ್ಧತೆ_ದತ್ತಾಂಶ AmdFftHelper::ಸಿದ್ಧತೆ_ದತ್ತಾಂಶ;
ತಾರ್ಕಿಕ AmdFftHelper::g_is_Amd_Fft_ಲಭ್ಯ = ಸುಳ್ಳು;
ತಾರ್ಕಿಕ AmdFftHelper::g_isAmdFftInitialized = ಸುಳ್ಳು;

ತಾರ್ಕಿಕ haveAmdFft()
{
    ಹಿಂತಿರುಗಿ AmdFftHelper::getInstance().is_ಲಭ್ಯ();
}

#ಇಲ್ಲದಿದ್ದರೆ

ತಾರ್ಕಿಕ haveAmdFft()
{
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

#ಆದರೆ_ಅಂತ್ಯ

ತಾರ್ಕಿಕ haveSVM()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
    ಹಿಂತಿರುಗಿ ನಿಜ;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ಮುಕ್ತಾಯ_2()
{
    ಸರತಿ::getDefault().ಮುಕ್ತಾಯ_2();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

ರಚನೆ Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = ಸುಳ್ಳು;
    }

    ~Impl() {}

    ಶೂನ್ಯ init()
    {
        ಆದರೆ( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            ಆದರೆ( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            ಆದರೆ( handle != 0 )
            {
                ಅಕ್ಷರ buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, ಗಾತ್ರ(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = ನಿಜ;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    ತಾರ್ಕಿಕ initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    ಆದರೆ(p)
        p->release();
}

Platform::Platform(ಸ್ಥಿರಾಂಕ Platform& pl)
{
    p = (Impl*)pl.p;
    ಆದರೆ(p)
        p->addref();
}

Platform& Platform::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    ಆದರೆ(newp)
        newp->addref();
    ಆದರೆ(p)
        p->release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = ಶೂನ್ಯಸೂಚಕ;
}

Platform& Platform::ಕ್ರಿಯಾಕಾರಕ = (Platform&& pl) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &pl) {
        ಆದರೆ(p)
            p->release();
        p = pl.p;
        pl.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಶೂನ್ಯ* Platform::ptr() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(ಖಾಲಿ, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    ಸ್ಥಿರ Platform p;
    ಆದರೆ( !p.p )
    {
        p.p = ಹೊಸ Impl;
        p.p->init();
    }
    ಹಿಂತಿರುಗಿ p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
ಸ್ಥಿರ ಶೂನ್ಯ parseOpenCLVersion(ಸ್ಥಿರಾಂಕ String &ಆವೃತ್ತಿ, ಪೂರ್ಣಾಂಕ &major, ಪೂರ್ಣಾಂಕ &minor)
{
    major = minor = 0;
    ಆದರೆ (10 >= ಆವೃತ್ತಿ.length())
        ಹಿಂತಿರುಗಿ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pstr = ಆವೃತ್ತಿ.c_str();
    ಆದರೆ (0 != ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(pstr, "OpenCL ", 7))
        ಹಿಂತಿರುಗಿ;
    size_t ppos = ಆವೃತ್ತಿ.find('.', 7);
    ಆದರೆ (String::npos == ppos)
        ಹಿಂತಿರುಗಿ;
    String ತಾತ್ಕಾಲಿಕ_2 = ಆವೃತ್ತಿ.substr(7, ppos - 7);
    major = atoi(ತಾತ್ಕಾಲಿಕ_2.c_str());
    ತಾತ್ಕಾಲಿಕ_2 = ಆವೃತ್ತಿ.substr(ppos + 1);
    minor = atoi(ತಾತ್ಕಾಲಿಕ_2.c_str());
}

ರಚನೆ Device::Impl
{
    Impl(ಶೂನ್ಯ* d)
        : refcount(1)
        , handle(0)
    {
        ಪ್ರಯತ್ನಿಸು
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        ಹಿಡಿ (...)
        {
            ಎಸೆ;
        }
    }

    ಶೂನ್ಯ _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        ಆವೃತ್ತಿ_3 = getStrProp(CL_DEVICE_ಆವೃತ್ತಿ);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_HALF_FP_CONFIG);
        host_Unified_ಸ್ಮರಣೆ = getBoolProp(CL_DEVICE_HOST_UNIFIED_ಸ್ಮರಣೆ);
        maxComputeUnits_ = getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_ಗುಂಪು_ಗಾತ್ರ_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_ಗುಂಪು_ಗಾತ್ರ);
        type_ = getProp<cl_device_type, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_TYPE);
        driver_ಆವೃತ್ತಿ_2 = getStrProp(CL_DRIVER_ಆವೃತ್ತಿ);
        ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು_2 = getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು);

        String device_ಆವೃತ್ತಿ = getStrProp(CL_DEVICE_ಆವೃತ್ತಿ);
        parseOpenCLVersion(device_ಆವೃತ್ತಿ, device_ಆವೃತ್ತಿ_Major_2, device_ಆವೃತ್ತಿ_Minor_2);

        size_t pos = 0;
        ಆಗ (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            ಆದರೆ (pos2 == String::npos)
                pos2 = extensions_.size();
            ಆದರೆ (pos2 > pos)
            {
                std::string extensionName = extensions_.substr(pos, pos2 - pos);
                extensions_ಗಣ.insert(extensionName);
            }
            pos = pos2 + 1;
        }

        khr_fp64_support_ = isExtensionSupported("cl_khr_fp64");
        khr_fp16_support_ = isExtensionSupported("cl_khr_fp16");

        intelSubgroupsSupport_ = isExtensionSupported("cl_intel_subgroups");

        vendorName_ = getStrProp(CL_DEVICE_VENDOR);
        ಆದರೆ (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        ಇಲ್ಲದಿದ್ದರೆ
            vendorID_ = UNKNOWN_VENDOR;

        ಸ್ಥಿರಾಂಕ size_t CV_OPENCL_DEVICE_MAX_WORK_ಗುಂಪು_ಗಾತ್ರ = utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        ಆದರೆ (CV_OPENCL_DEVICE_MAX_WORK_ಗುಂಪು_ಗಾತ್ರ > 0)
        {
            ಸ್ಥಿರಾಂಕ size_t new_max_Work_ಗುಂಪು_ಗಾತ್ರ = std::min(max_Work_ಗುಂಪು_ಗಾತ್ರ_2, CV_OPENCL_DEVICE_MAX_WORK_ಗುಂಪು_ಗಾತ್ರ);
            ಆದರೆ (new_max_Work_ಗುಂಪು_ಗಾತ್ರ != max_Work_ಗುಂಪು_ಗಾತ್ರ_2)
                CV_LOG_WARNING(ಖಾಲಿ, "OpenCL: using workgroup size: " << new_max_Work_ಗುಂಪು_ಗಾತ್ರ << " (was " << max_Work_ಗುಂಪು_ಗಾತ್ರ_2 << ")");
            max_Work_ಗುಂಪು_ಗಾತ್ರ_2 = new_max_Work_ಗುಂಪು_ಗಾತ್ರ;
        }
#ಆದರೆ 0
        ಆದರೆ (isExtensionSupported("cl_khr_spir"))
        {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CL_DEVICE_SPIR_VERSIONS
#ವ್ಯಾಖ್ಯಾನಿಸು CL_DEVICE_SPIR_VERSIONS                     0x40E0
#ಆದರೆ_ಅಂತ್ಯ
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::ಹೊರಗೆ << spir_versions << std::ಸಾಲು_ಅಂತ್ಯ;
        }
#ಆದರೆ_ಅಂತ್ಯ
    }

    ~Impl()
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
        ಆದರೆ (!cv::__termination)
#ಆದರೆ_ಅಂತ್ಯ
        {
            ಆದರೆ (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _TpCL, ಬಗೆ_ಹೆಸರು Tp_ಹೊರಹೋಗು>
    Tp_ಹೊರಹೋಗು getProp(cl_device_info prop) ಸ್ಥಿರಾಂಕ
    {
        _TpCL ತಾತ್ಕಾಲಿಕ_2=_TpCL();
        size_t sz = 0;

        ಹಿಂತಿರುಗಿ clGetDeviceInfo(handle, prop, ಗಾತ್ರ(ತಾತ್ಕಾಲಿಕ_2), &ತಾತ್ಕಾಲಿಕ_2, &sz) == CL_SUCCESS &&
            sz == ಗಾತ್ರ(ತಾತ್ಕಾಲಿಕ_2) ? Tp_ಹೊರಹೋಗು(ತಾತ್ಕಾಲಿಕ_2) : Tp_ಹೊರಹೋಗು();
    }

    ತಾರ್ಕಿಕ getBoolProp(cl_device_info prop) ಸ್ಥಿರಾಂಕ
    {
        cl_bool ತಾತ್ಕಾಲಿಕ_2 = CL_FALSE;
        size_t sz = 0;

        ಹಿಂತಿರುಗಿ clGetDeviceInfo(handle, prop, ಗಾತ್ರ(ತಾತ್ಕಾಲಿಕ_2), &ತಾತ್ಕಾಲಿಕ_2, &sz) == CL_SUCCESS &&
            sz == ಗಾತ್ರ(ತಾತ್ಕಾಲಿಕ_2) ? ತಾತ್ಕಾಲಿಕ_2 != 0 : ಸುಳ್ಳು;
    }

    String getStrProp(cl_device_info prop) ಸ್ಥಿರಾಂಕ
    {
        ಅಕ್ಷರ buf[4096];
        size_t sz=0;
        ಹಿಂತಿರುಗಿ clGetDeviceInfo(handle, prop, ಗಾತ್ರ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < ಗಾತ್ರ(buf) ? String(buf) : String();
    }

    ತಾರ್ಕಿಕ isExtensionSupported(ಸ್ಥಿರಾಂಕ std::string& extensionName) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ extensions_ಗಣ.ಎಣಿಕೆ_3(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String ಆವೃತ್ತಿ_3;
    std::string extensions_;
    ಪೂರ್ಣಾಂಕ doubleFPConfig_;
    ತಾರ್ಕಿಕ khr_fp64_support_;
    ಪೂರ್ಣಾಂಕ halfFPConfig_;
    ತಾರ್ಕಿಕ khr_fp16_support_;
    ತಾರ್ಕಿಕ host_Unified_ಸ್ಮರಣೆ;
    ಪೂರ್ಣಾಂಕ maxComputeUnits_;
    size_t max_Work_ಗುಂಪು_ಗಾತ್ರ_2;
    ಪೂರ್ಣಾಂಕ type_;
    ಪೂರ್ಣಾಂಕ ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು_2;
    ಪೂರ್ಣಾಂಕ device_ಆವೃತ್ತಿ_Major_2;
    ಪೂರ್ಣಾಂಕ device_ಆವೃತ್ತಿ_Minor_2;
    String driver_ಆವೃತ್ತಿ_2;
    String vendorName_;
    ಪೂರ್ಣಾಂಕ vendorID_;
    ತಾರ್ಕಿಕ intelSubgroupsSupport_;

    std::ಗಣ<std::string> extensions_ಗಣ;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(ಶೂನ್ಯ* d)
{
    p = 0;
    ಗಣ(d);
}

Device::Device(ಸ್ಥಿರಾಂಕ Device& d)
{
    p = d.p;
    ಆದರೆ(p)
        p->addref();
}

Device& Device::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Device& d)
{
    Impl* newp = (Impl*)d.p;
    ಆದರೆ(newp)
        newp->addref();
    ಆದರೆ(p)
        p->release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = ಶೂನ್ಯಸೂಚಕ;
}

Device& Device::ಕ್ರಿಯಾಕಾರಕ = (Device&& d) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &d) {
        ಆದರೆ(p)
            p->release();
        p = d.p;
        d.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Device::~Device()
{
    ಆದರೆ(p)
        p->release();
}

ಶೂನ್ಯ Device::ಗಣ(ಶೂನ್ಯ* d)
{
    ಆದರೆ(p)
        p->release();
    p = ಹೊಸ Impl(d);
    ಆದರೆ (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(ಶೂನ್ಯ* d)
{
    Device device(d);
    ಹಿಂತಿರುಗಿ device;
}

ಶೂನ್ಯ* Device::ptr() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->handle : 0;
}

String Device::name() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->name_ : String(); }

String Device::extensions() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? String(p->extensions_) : String(); }

ತಾರ್ಕಿಕ Device::isExtensionSupported(ಸ್ಥಿರಾಂಕ String& extensionName) ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->isExtensionSupported(extensionName) : ಸುಳ್ಳು; }

String Device::ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->ಆವೃತ್ತಿ_3 : String(); }

String Device::vendorName() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->vendorName_ : String(); }

ಪೂರ್ಣಾಂಕ Device::vendorID() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->vendorID_ : 0; }

String Device::Open_CL_C_ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getStrProp(CL_DEVICE_OPENCL_C_ಆವೃತ್ತಿ) : String(); }

String Device::OpenCLVersion() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getStrProp(CL_DEVICE_ಆವೃತ್ತಿ) : String(); }

ಪೂರ್ಣಾಂಕ Device::device_ಆವೃತ್ತಿ_Major() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->device_ಆವೃತ್ತಿ_Major_2 : 0; }

ಪೂರ್ಣಾಂಕ Device::device_ಆವೃತ್ತಿ_Minor() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->device_ಆವೃತ್ತಿ_Minor_2 : 0; }

String Device::driver_ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->driver_ಆವೃತ್ತಿ_2 : String(); }

ಪೂರ್ಣಾಂಕ Device::type() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->type_ : 0; }

ಪೂರ್ಣಾಂಕ Device::ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು_2 : 0; }

ತಾರ್ಕಿಕ Device::ಲಭ್ಯ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getBoolProp(CL_DEVICE_ಲಭ್ಯ) : ಸುಳ್ಳು; }

ತಾರ್ಕಿಕ Device::compiler_ಲಭ್ಯ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getBoolProp(CL_DEVICE_COMPILER_ಲಭ್ಯ) : ಸುಳ್ಳು; }

ತಾರ್ಕಿಕ Device::linker_ಲಭ್ಯ() ಸ್ಥಿರಾಂಕ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CL_ಆವೃತ್ತಿ_1_2
{ ಹಿಂತಿರುಗಿ p ? p->getBoolProp(CL_DEVICE_LINKER_ಲಭ್ಯ) : ಸುಳ್ಳು; }
#ಇಲ್ಲದಿದ್ದರೆ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ Device::doubleFPConfig() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->doubleFPConfig_ : 0; }

ಪೂರ್ಣಾಂಕ Device::singleFPConfig() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_device_fp_config, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

ಪೂರ್ಣಾಂಕ Device::halfFPConfig() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->halfFPConfig_ : 0; }

ತಾರ್ಕಿಕ Device::hasFP64() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->khr_fp64_support_ : ಸುಳ್ಳು; }
ತಾರ್ಕಿಕ Device::hasFP16() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->khr_fp16_support_ : ಸುಳ್ಳು; }

ತಾರ್ಕಿಕ Device::endianLittle() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : ಸುಳ್ಳು; }

ತಾರ್ಕಿಕ Device::errorCorrectionSupport() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : ಸುಳ್ಳು; }

ಪೂರ್ಣಾಂಕ Device::executionCapabilities() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_device_exec_capabilities, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::global_Mem_Cache_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_ಗಾತ್ರ) : 0; }

ಪೂರ್ಣಾಂಕ Device::globalMemCacheType() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_device_mem_cache_type, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

ಪೂರ್ಣಾಂಕ Device::global_Mem_Cache_ಸಾಲು_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_GLOBAL_MEM_CACHELINE_ಗಾತ್ರ) : 0; }

size_t Device::global_Mem_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_ಗಾತ್ರ) : 0; }

size_t Device::local_Mem_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_ಗಾತ್ರ) : 0; }

ಪೂರ್ಣಾಂಕ Device::localMemType() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_device_local_mem_type, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

ತಾರ್ಕಿಕ Device::host_Unified_ಸ್ಮರಣೆ_2() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->host_Unified_ಸ್ಮರಣೆ : ಸುಳ್ಳು; }

ತಾರ್ಕಿಕ Device::imageSupport() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : ಸುಳ್ಳು; }

ತಾರ್ಕಿಕ Device::image_From_ಬಫರ್_Support() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : ಸುಳ್ಳು;
}

uint Device::imagePitchAlignment() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

uint Device::image_ತಳ_ವಿಳಾಸ_Alignment() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CL_DEVICE_IMAGE_ತಳ_ವಿಳಾಸ_ALIGNMENT
    ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_ತಳ_ವಿಳಾಸ_ALIGNMENT) : 0;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

size_t Device::image2_DMax_ಅಗಲ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_ಅಗಲ) : 0; }

size_t Device::image2_DMax_ಎತ್ತರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_ಎತ್ತರ) : 0; }

size_t Device::image3_DMax_ಅಗಲ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ಅಗಲ) : 0; }

size_t Device::image3_DMax_ಎತ್ತರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ಎತ್ತರ) : 0; }

size_t Device::image3_DMax_ಆಳ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ಆಳ) : 0; }

size_t Device::image_Max_ಬಫರ್_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CL_ಆವೃತ್ತಿ_1_2
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ಬಫರ್_ಗಾತ್ರ) : 0; }
#ಇಲ್ಲದಿದ್ದರೆ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#ಆದರೆ_ಅಂತ್ಯ

size_t Device::image_Max_Array_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CL_ಆವೃತ್ತಿ_1_2
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_ಗಾತ್ರ) : 0; }
#ಇಲ್ಲದಿದ್ದರೆ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#ಆದರೆ_ಅಂತ್ಯ

ತಾರ್ಕಿಕ Device::intelSubgroupsSupport() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->intelSubgroupsSupport_ : ಸುಳ್ಳು; }

ಪೂರ್ಣಾಂಕ Device::maxClockFrequency() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

ಪೂರ್ಣಾಂಕ Device::maxComputeUnits() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->maxComputeUnits_ : 0; }

ಪೂರ್ಣಾಂಕ Device::max_Constant_ವಾದಗಳು() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_MAX_CONSTANT_ವಾದಗಳು) : 0; }

size_t Device::max_Constant_ಬಫರ್_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_ಬಫರ್_ಗಾತ್ರ) : 0; }

size_t Device::max_Mem_Alloc_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_ಗಾತ್ರ) : 0; }

size_t Device::max_Parameter_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_ಗಾತ್ರ) : 0; }

ಪೂರ್ಣಾಂಕ Device::max_Read_Image_ವಾದಗಳು() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_MAX_READ_IMAGE_ವಾದಗಳು) : 0; }

ಪೂರ್ಣಾಂಕ Device::max_Write_Image_ವಾದಗಳು() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_MAX_WRITE_IMAGE_ವಾದಗಳು) : 0; }

ಪೂರ್ಣಾಂಕ Device::maxSamplers() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_ಗುಂಪು_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->max_Work_ಗುಂಪು_ಗಾತ್ರ_2 : 0; }

ಪೂರ್ಣಾಂಕ Device::max_Work_ಅಂಶ_Dims() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_MAX_WORK_ಅಂಶ_DIMENSIONS) : 0; }

ಶೂನ್ಯ Device::max_Work_ಅಂಶ_Sizes(size_t* sizes) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(p)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ಅಂಶ_SIZES,
                MAX_DIMS*ಗಾತ್ರ(sizes[0]), &sizes[0], &retsz));
    }
}

ಪೂರ್ಣಾಂಕ Device::mem_ತಳ_Addr_Align() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_MEM_ತಳ_ADDR_ALIGN) : 0; }

ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Char() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_NATIVE_ಸದಿಶ_ಅಗಲ_CHAR) : 0; }

ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Short() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_NATIVE_ಸದಿಶ_ಅಗಲ_SHORT) : 0; }

ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Int() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_NATIVE_ಸದಿಶ_ಅಗಲ_INT) : 0; }

ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Long() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_NATIVE_ಸದಿಶ_ಅಗಲ_LONG) : 0; }

ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Float() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_NATIVE_ಸದಿಶ_ಅಗಲ_FLOAT) : 0; }

ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Double() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_NATIVE_ಸದಿಶ_ಅಗಲ_DOUBLE) : 0; }

ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Half() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_NATIVE_ಸದಿಶ_ಅಗಲ_HALF) : 0; }

ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Char() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_PREFERRED_ಸದಿಶ_ಅಗಲ_CHAR) : 0; }

ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Short() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_PREFERRED_ಸದಿಶ_ಅಗಲ_SHORT) : 0; }

ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Int() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_PREFERRED_ಸದಿಶ_ಅಗಲ_INT) : 0; }

ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Long() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_PREFERRED_ಸದಿಶ_ಅಗಲ_LONG) : 0; }

ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Float() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_PREFERRED_ಸದಿಶ_ಅಗಲ_FLOAT) : 0; }

ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Double() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_PREFERRED_ಸದಿಶ_ಅಗಲ_DOUBLE) : 0; }

ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Half() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಂಕ>(CL_DEVICE_PREFERRED_ಸದಿಶ_ಅಗಲ_HALF) : 0; }

size_t Device::printf_ಬಫರ್_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CL_ಆವೃತ್ತಿ_1_2
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_ಬಫರ್_ಗಾತ್ರ) : 0; }
#ಇಲ್ಲದಿದ್ದರೆ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#ಆದರೆ_ಅಂತ್ಯ


size_t Device::profilingTimerResolution() ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ಸ್ಥಿರಾಂಕ Device& Device::getDefault()
{
    ಸ್ವಯಂಚಾಲಿತ& c = OpenCLExecutionContext::getCurrent();
    ಆದರೆ (!c.empty())
    {
        ಹಿಂತಿರುಗಿ c.getDevice();
    }

    ಸ್ಥಿರ Device dummy;
    ಹಿಂತಿರುಗಿ dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು Functor, ಬಗೆ_ಹೆಸರು ObjectType>
ಸಾಲೊಳಗೆ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, ಖಾಲಿ, &required);
    ಆದರೆ (err != CL_SUCCESS)
        ಹಿಂತಿರುಗಿ err;

    param.clear();
    ಆದರೆ (required > 0)
    {
        Auto_ಬಫರ್<ಅಕ್ಷರ> buf(required + 1);
        ಅಕ್ಷರ* ptr = buf.ದತ್ತಾಂಶ(); // cleanup is not needed
        err = f(obj, name, required, ptr, ಖಾಲಿ);
        ಆದರೆ (err != CL_SUCCESS)
            ಹಿಂತಿರುಗಿ err;
        param = ptr;
    }

    ಹಿಂತಿರುಗಿ CL_SUCCESS;
}

ಸ್ಥಿರ ಶೂನ್ಯ ವಿಭಜಿಸು(ಸ್ಥಿರಾಂಕ std::string &s, ಅಕ್ಷರ delim, std::vector<std::string> &elems)
{
    elems.clear();
    ಆದರೆ (s.size() == 0)
        ಹಿಂತಿರುಗಿ;
    std::istringstream ss(s);
    std::string ಅಂಶ;
    ಆಗ (!ss.eof())
    {
        std::getline(ss, ಅಂಶ, delim);
        elems.push_back(ಅಂಶ);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
ಸ್ಥಿರ ತಾರ್ಕಿಕ parseOpenCLDeviceConfiguration(ಸ್ಥಿರಾಂಕ std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    ವಿಭಜಿಸು(configurationStr, ':', parts);
    ಆದರೆ (parts.size() > 3)
    {
        CV_LOG_ERROR(ಖಾಲಿ, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಆದರೆ (parts.size() > 2)
        deviceNameOrID = parts[2];
    ಆದರೆ (parts.size() > 1)
    {
        ವಿಭಜಿಸು(parts[1], '|', deviceTypes);
    }
    ಆದರೆ (parts.size() > 0)
    {
        platform = parts[0];
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಸ್ಥಿರ cl_device_id selectOpenCLDevice(ಸ್ಥಿರಾಂಕ std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    ಆದರೆ (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    ಆದರೆ (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ತಾರ್ಕಿಕ isID = ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ deviceID = -1;
    ಆದರೆ (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = ನಿಜ;
        ಗಾಗಿ (size_t i = 0; i < deviceName.length(); i++)
        {
            ಆದರೆ (!ಅಂಕಿಯೇ(deviceName[i]))
            {
                isID = ಸುಳ್ಳು;
                ನಿಲ್ಲಿಸು;
            }
        }
        ಆದರೆ (isID)
        {
            deviceID = atoi(deviceName.c_str());
            ಆದರೆ (deviceID < 0)
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint ಅಂಕೆ_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ಖಾಲಿ, &ಅಂಕೆ_Platforms));

        ಆದರೆ (ಅಂಕೆ_Platforms == 0)
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        platforms.resize((size_t)ಅಂಕೆ_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], &ಅಂಕೆ_Platforms));
        platforms.resize(ಅಂಕೆ_Platforms);
    }

    ಆದರೆ (platform.length() > 0)
    {
        ಗಾಗಿ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            ಆದರೆ (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        ಆದರೆ (platforms.size() == 0)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "OpenCL: Can't find OpenCL platform by name: " << platform);
            ಹೋಗು not_ಸಿಕ್ಕಿತು;
        }
    }
    ಆದರೆ (deviceTypes.size() == 0)
    {
        ಆದರೆ (!isID)
        {
            deviceTypes.push_back("GPU");
            ಆದರೆ (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        ಇಲ್ಲದಿದ್ದರೆ
            deviceTypes.push_back("ALL");
    }
    ಗಾಗಿ (size_t t = 0; t < deviceTypes.size(); t++)
    {
        ಪೂರ್ಣಾಂಕ deviceType = 0;
        std::string ತಾತ್ಕಾಲಿಕ_Str_Device_Type = deviceTypes[t];
        std::transform(ತಾತ್ಕಾಲಿಕ_Str_Device_Type.begin(), ತಾತ್ಕಾಲಿಕ_Str_Device_Type.end(), ತಾತ್ಕಾಲಿಕ_Str_Device_Type.begin(), details::char_tolower);

        ಆದರೆ (ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "gpu" || ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "dgpu" || ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_LOG_ERROR(ಖಾಲಿ, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            ಹೋಗು not_ಸಿಕ್ಕಿತು;
        }

        std::vector<cl_device_id> devices;
        ಗಾಗಿ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint ಎಣಿಕೆ_3 = 0;
            cl_int ಸ್ಥಿತಿ_2 = clGetDeviceIDs(*currentPlatform, deviceType, 0, ಖಾಲಿ, &ಎಣಿಕೆ_3);
            ಆದರೆ (!(ಸ್ಥಿತಿ_2 == CL_SUCCESS || ಸ್ಥಿತಿ_2 == CL_DEVICE_NOT_ಸಿಕ್ಕಿತು))
            {
                CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clGetDeviceIDs get count");
            }
            ಆದರೆ (ಎಣಿಕೆ_3 == 0)
                ಮುಂದುವರಿ;
            size_t ತಳ = devices.size();
            devices.resize(ತಳ + ಎಣಿಕೆ_3);
            ಸ್ಥಿತಿ_2 = clGetDeviceIDs(*currentPlatform, deviceType, ಎಣಿಕೆ_3, &devices[ತಳ], &ಎಣಿಕೆ_3);
            ಆದರೆ (!(ಸ್ಥಿತಿ_2 == CL_SUCCESS || ಸ್ಥಿತಿ_2 == CL_DEVICE_NOT_ಸಿಕ್ಕಿತು))
            {
                CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clGetDeviceIDs get IDs");
            }
        }

        ಗಾಗಿ (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : ನಿಜ) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = ನಿಜ;
            ಆದರೆ(ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "dgpu" || ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_ಸ್ಮರಣೆ, ಗಾತ್ರ(isIGPU), &isIGPU, ಖಾಲಿ));
                useGPU = ತಾತ್ಕಾಲಿಕ_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            ಆದರೆ ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                ಹಿಂತಿರುಗಿ devices[i];
            }
        }
    }

not_ಸಿಕ್ಕಿತು:
    ಆದರೆ (configuration.empty())
        ಹಿಂತಿರುಗಿ ಖಾಲಿ; // suppress messages on stderr

    std::ostringstream ಸಂದೇಶ;
    ಸಂದೇಶ << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::ಸಾಲು_ಅಂತ್ಯ
        << "    Device types:";
    ಗಾಗಿ (size_t t = 0; t < deviceTypes.size(); t++)
        ಸಂದೇಶ << ' ' << deviceTypes[t];

    ಸಂದೇಶ << std::ಸಾಲು_ಅಂತ್ಯ << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(ಖಾಲಿ, ಸಂದೇಶ.str());
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
ನಾಮಾಕಾಶ svm {

ಎಣಿಕೆ AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_ಬಫರ್ = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_ಬಫರ್ = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ = 3 << 16,
        OPENCL_SVM_ಬಫರ್_MAP = 4 << 16
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ checkForceSVMUmatUsage()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ force = ಸುಳ್ಳು;
    ಆದರೆ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", ಸುಳ್ಳು);
        initialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ force;
}
ಸ್ಥಿರ ತಾರ್ಕಿಕ checkDisableSVMUMatUsage()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ force = ಸುಳ್ಳು;
    ಆದರೆ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", ಸುಳ್ಳು);
        initialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ force;
}
ಸ್ಥಿರ ತಾರ್ಕಿಕ checkDisableSVM()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ force = ಸುಳ್ಳು;
    ಆದರೆ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", ಸುಳ್ಳು);
        initialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ force;
}
// see SVMCapabilities
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ get_SVMCapabilities_ಮುಖವಾಡ()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ = 0;
    ಆದರೆ (!initialized)
    {
        ಸ್ಥಿರಾಂಕ std::string env_ಮೌಲ್ಯ = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        ಆದರೆ (env_ಮೌಲ್ಯ.empty())
        {
            ಹಿಂತಿರುಗಿ ~0U; // all bits 1
        }
        ಮುಖವಾಡ = atoi(env_ಮೌಲ್ಯ.c_str());
        initialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}
} // namespace
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ size_t get_ಕಾರ್ಯಕ್ರಮ_ಎಣಿಕೆ_ಎಲ್ಲೆ()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ size_t ಎಣಿಕೆ_3 = 0;
    ಆದರೆ (!initialized)
    {
        ಎಣಿಕೆ_3 = utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_3;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ g_contextId = 0;

ವರ್ಗ OpenCLBufferPoolImpl;
ವರ್ಗ OpenCLSVMBufferPoolImpl;

ರಚನೆ Context::Impl
{
    ಸ್ಥಿರ Context::Impl* get(Context& context) { ಹಿಂತಿರುಗಿ context.getImpl(); }

    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ std::deque<Context::Impl*> container_t;
    ಸ್ಥಿರ container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        ಸ್ಥಿರ container_t* g_contexts = ಹೊಸ container_t();
        ಹಿಂತಿರುಗಿ *g_contexts;
    }

ಸಂರಕ್ಷಿತ:
    Impl(ಸ್ಥಿರಾಂಕ std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        , svmInitialized(ಸುಳ್ಳು)
#ಆದರೆ_ಅಂತ್ಯ
    {
        ಆದರೆ (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
        ಸ್ವಯಂಚಾಲಿತ& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = ಇದು;
    }

    ~Impl()
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
        ಆದರೆ (!cv::__termination)
#ಆದರೆ_ಅಂತ್ಯ
        {
            ಆದರೆ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = ಖಾಲಿ;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
            ಸ್ವಯಂಚಾಲಿತ& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = ಖಾಲಿ;
        }
    }

    ಶೂನ್ಯ init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_ಅಂಕೆ_DEVICES, ಗಾತ್ರ(ndevices), &ndevices, ಖಾಲಿ));
        CV_Assert(ndevices > 0);

        cv::Auto_ಬಫರ್<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_ಗಾತ್ರ = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * ಗಾತ್ರ(cl_device_id), &cl_devices[0], &devices_ret_ಗಾತ್ರ));
        CV_CheckEQ(devices_ret_ಗಾತ್ರ, cl_devices.size() * ಗಾತ್ರ(cl_device_id), "");

        devices.clear();
        ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    ಶೂನ್ಯ __init_buffer_pools();  // w/o synchronization
    ಶೂನ್ಯ init_ಬಫರ್_pools() ಸ್ಥಿರಾಂಕ
    {
        ಆದರೆ (!ಬಫರ್_Pool_2)
        {
            cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
            ಆದರೆ (!ಬಫರ್_Pool_2)
            {
                const_cast<Impl*>(ಇದು)->__init_buffer_pools();
            }
        }
    }
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ Impl* ಕಂಡುಹಿಡಿ_Context(ಸ್ಥಿರಾಂಕ std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
        ಸ್ವಯಂಚಾಲಿತ& container = getGlobalContainer();
        ಆದರೆ (configuration.empty() && !container.empty())
            ಹಿಂತಿರುಗಿ container[0];
        ಗಾಗಿ (ಸ್ವಯಂಚಾಲಿತ it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            ಆದರೆ (i && i->configuration == configuration)
            {
                ಹಿಂತಿರುಗಿ i;
            }
        }
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಸ್ಥಿರ Impl* ಕಂಡುಹಿಡಿ_Or_Create_Context(ಸ್ಥಿರಾಂಕ std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        ಆದರೆ (configuration_.empty())
        {
            ಸ್ಥಿರಾಂಕ std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            ಆದರೆ (!c.empty())
                configuration = c;
        }
        Impl* impl = ಕಂಡುಹಿಡಿ_Context(configuration);
        ಆದರೆ (impl)
        {
            CV_LOG_INFO(ಖಾಲಿ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ಹಿಂತಿರುಗಿ impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        ಆದರೆ (d == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;

        impl = ಹೊಸ Impl(configuration);
        ಪ್ರಯತ್ನಿಸು
        {
            impl->createFromDevice(d);
            ಆದರೆ (impl->handle)
                ಹಿಂತಿರುಗಿ impl;
            ಅಳಿಸು impl;
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
        ಹಿಡಿ (...)
        {
            ಅಳಿಸು impl;
            ಎಸೆ;
        }
    }

    ಸ್ಥಿರ Impl* ಕಂಡುಹಿಡಿ_Or_Create_Context(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (ಶೂನ್ಯ*)h);
        Impl* impl = ಕಂಡುಹಿಡಿ_Context(configuration);
        ಆದರೆ (impl)
        {
            CV_LOG_INFO(ಖಾಲಿ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ಹಿಂತಿರುಗಿ impl;
        }

        impl = ಹೊಸ Impl(configuration);
        ಪ್ರಯತ್ನಿಸು
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            ಹಿಂತಿರುಗಿ impl;
        }
        ಹಿಡಿ (...)
        {
            ಅಳಿಸು impl;
            ಎಸೆ;
        }
    }

    ಸ್ಥಿರ Impl* ಕಂಡುಹಿಡಿ_Or_Create_Context(ಸ್ಥಿರಾಂಕ ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (ಶೂನ್ಯ*)d);
        Impl* impl = ಕಂಡುಹಿಡಿ_Context(configuration);
        ಆದರೆ (impl)
        {
            CV_LOG_INFO(ಖಾಲಿ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ಹಿಂತಿರುಗಿ impl;
        }

        impl = ಹೊಸ Impl(configuration);
        ಪ್ರಯತ್ನಿಸು
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            ಹಿಂತಿರುಗಿ impl;
        }
        ಹಿಡಿ (...)
        {
            ಅಳಿಸು impl;
            ಎಸೆ;
        }
    }

    ಶೂನ್ಯ ಗಣ_Default()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        ಆದರೆ (d == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ;

        createFromDevice(d);
    }

    ಶೂನ್ಯ createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == ಖಾಲಿ);

        cl_platform_id pl = ಖಾಲಿ;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, ಗಾತ್ರ(cl_platform_id), &pl, ಖಾಲಿ));

        cl_context_properties prop[] =
        {
            CL_CONTEXT_PLATFORM, (cl_context_properties)pl,
            0
        };

        // !!! in the current implementation force the number of devices to 1 !!!
        cl_uint nd = 1;
        cl_int ಸ್ಥಿತಿ_2;

        handle = clCreateContext(prop, nd, &d, 0, 0, &ಸ್ಥಿತಿ_2);
        CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clCreateContext");

        ತಾರ್ಕಿಕ ok = handle != 0 && ಸ್ಥಿತಿ_2 == CL_SUCCESS;
        ಆದರೆ( ok )
        {
            devices.resize(nd);
            devices[0].ಗಣ(d);
        }
        ಇಲ್ಲದಿದ್ದರೆ
            handle = ಖಾಲಿ;
    }

    ಕಾರ್ಯಕ್ರಮ_2 getProg(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src, ಸ್ಥಿರಾಂಕ String& buildflags, String& errmsg);

    ಶೂನ್ಯ unloadProg(ಕಾರ್ಯಕ್ರಮ_2& prog)
    {
        cv::Auto_ಬೀಗ lock(ಕಾರ್ಯಕ್ರಮ_cache_mutex);
        ಗಾಗಿ (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              ಆದರೆ (it != phash.end())
              {
                  ಆದರೆ (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      ಹಿಂತಿರುಗಿ;
                  }
              }
        }
    }

    std::string& get_ಪೂರ್ವಪ್ರತ್ಯಯ_String()
    {
        ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ.empty())
        {
            cv::Auto_ಬೀಗ lock(ಕಾರ್ಯಕ್ರಮ_cache_mutex);
            ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ.empty())
            {
                CV_Assert(!devices.empty());
                ಸ್ಥಿರಾಂಕ Device& d = devices[0];
                ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು_2 = d.ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು();
                ಆದರೆ (ಬಿಟ್ಟುಗಳು_2 > 0 && ಬಿಟ್ಟುಗಳು_2 != 64)
                    ಪೂರ್ವಪ್ರತ್ಯಯ = cv::format("%d-bit--", ಬಿಟ್ಟುಗಳು_2);
                ಪೂರ್ವಪ್ರತ್ಯಯ += d.vendorName() + "--" + d.name() + "--" + d.driver_ಆವೃತ್ತಿ();
                // sanitize chars
                ಗಾಗಿ (size_t i = 0; i < ಪೂರ್ವಪ್ರತ್ಯಯ.size(); i++)
                {
                    ಅಕ್ಷರ c = ಪೂರ್ವಪ್ರತ್ಯಯ[i];
                    ಆದರೆ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        ಪೂರ್ವಪ್ರತ್ಯಯ[i] = '_';
                    }
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಪೂರ್ವಪ್ರತ್ಯಯ;
    }

    std::string& get_ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ()
    {
        ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ.empty())
        {
            cv::Auto_ಬೀಗ lock(ಕಾರ್ಯಕ್ರಮ_cache_mutex);
            ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ.empty())
            {
                ಸ್ಥಿರಾಂಕ Device& d = devices[0];
                ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು_2 = d.ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು();
                ಆದರೆ (ಬಿಟ್ಟುಗಳು_2 > 0 && ಬಿಟ್ಟುಗಳು_2 != 64)
                    ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ = cv::format("%d-bit--", ಬಿಟ್ಟುಗಳು_2);
                ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                ಗಾಗಿ (size_t i = 0; i < ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ.size(); i++)
                {
                    ಅಕ್ಷರ c = ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ[i];
                    ಆದರೆ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ[i] = '_';
                    }
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ;
    }

    IMPLEMENT_REFCOUNTABLE();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ contextId;  // global unique ID
    ಸ್ಥಿರಾಂಕ std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string ಪೂರ್ವಪ್ರತ್ಯಯ;
    std::string ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ;

    cv::Mutex ಕಾರ್ಯಕ್ರಮ_cache_mutex;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ std::map<std::string, ಕಾರ್ಯಕ್ರಮ_2> phash_t;
    phash_t phash;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> ಬಫರ್_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> ಬಫರ್_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_ಬಫರ್_Pool() ಸ್ಥಿರಾಂಕ
    {
        init_ಬಫರ್_pools();
        CV_DbgAssert(ಬಫರ್_Pool_2);
        ಹಿಂತಿರುಗಿ *ಬಫರ್_Pool_2.get();
    }
    OpenCLBufferPoolImpl& get_ಬಫರ್_Pool_Host_Ptr() ಸ್ಥಿರಾಂಕ
    {
        init_ಬಫರ್_pools();
        CV_DbgAssert(ಬಫರ್_Pool_Host_Ptr);
        ಹಿಂತಿರುಗಿ *ಬಫರ್_Pool_Host_Ptr.get();
    }

    std::map<std::type_ಸೂಚ್ಯಂಕ, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    ಶೂನ್ಯ ಗಣ_User_Context(std::type_ಸೂಚ್ಯಂಕ typeId, ಸ್ಥಿರಾಂಕ std::shared_ptr<UserContext>& userContext) {
        cv::Auto_ಬೀಗ lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_ಸೂಚ್ಯಂಕ typeId) {
        cv::Auto_ಬೀಗ lock(userContextMutex);
        ಸ್ವಯಂಚಾಲಿತ it = userContextStorage.find(typeId);
        ಆದರೆ (it != userContextStorage.end())
            ಹಿಂತಿರುಗಿ it->second;
        ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
    ತಾರ್ಕಿಕ svmInitialized;
    ತಾರ್ಕಿಕ svm_ಲಭ್ಯ;
    ತಾರ್ಕಿಕ svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    ಶೂನ್ಯ svmInit()
    {
        CV_Assert(handle != ಖಾಲಿ);
        ಸ್ಥಿರಾಂಕ Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((ಶೂನ್ಯ)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int ಸ್ಥಿತಿ_2 = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, ಗಾತ್ರ(deviceCaps), &deviceCaps, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", ಸ್ಥಿತಿ_2);
            ಹೋಗು noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (ಪೂರ್ಣಾಂಕ)deviceCaps);
        CV_Assert(((ಶೂನ್ಯ)0, CL_DEVICE_SVM_COARSE_GRAIN_ಬಫರ್ == CL_DEVICE_SVM_COARSE_GRAIN_ಬಫರ್_AMD)); // Check assumption
        svmCapabilities.ಮೌಲ್ಯ_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_ಬಫರ್) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_ಬಫರ್ : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_ಬಫರ್) ? svm::SVMCapabilities::SVM_FINE_GRAIN_ಬಫರ್ : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.ಮೌಲ್ಯ_2 &= svm::get_SVMCapabilities_ಮುಖವಾಡ();
        ಆದರೆ (svmCapabilities.ಮೌಲ್ಯ_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            ಹೋಗು noSVM;
        }
        ಪ್ರಯತ್ನಿಸು
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            ಶೂನ್ಯ* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            ಆದರೆ (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            ಪ್ರಯತ್ನಿಸು
            {
                ತಾರ್ಕಿಕ error = ಸುಳ್ಳು;
                cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
                ಆದರೆ (CL_SUCCESS != cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, ಖಾಲಿ, ಖಾಲಿ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                cl_ಮುಕ್ತಾಯ(q);
                ಪ್ರಯತ್ನಿಸು
                {
                    ((ಪೂರ್ಣಾಂಕ*)ptr)[0] = 100;
                }
                ಹಿಡಿ (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = ನಿಜ;
                }
                ಆದರೆ (CL_SUCCESS != cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap(q, ptr, 0, ಖಾಲಿ, ಖಾಲಿ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                cl_ಮುಕ್ತಾಯ(q);
                ಆದರೆ (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            ಹಿಡಿ (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                ಎಸೆ;
            }
            clSVMFree(handle, ptr);
            svmFunctions.fn_clSVMAlloc = clSVMAlloc;
            svmFunctions.fn_clSVMFree = clSVMFree;
            svmFunctions.fn_cl_ಗಣ_Kernel_Arg_SVMPointer = cl_ಗಣ_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMemcpy = cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMemcpy;
            svmFunctions.fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMem_Fill = cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMem_Fill;
            svmFunctions.fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap = cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap;
            svmFunctions.fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap = cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap;
        }
        ಹಿಡಿ (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            ಪ್ರಯತ್ನಿಸು
            {
                // Try HSA extension
                String extensions = device.extensions();
                ಆದರೆ (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    ಹೋಗು noSVM;
                }
                cl_platform_id p = ಖಾಲಿ;
                CV_OCL_CHECK(ಸ್ಥಿತಿ_2 = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, ಗಾತ್ರ(cl_platform_id), &p, ಖಾಲಿ));
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_ಗಣ_Kernel_Arg_SVMPointer = (cl_ಗಣ_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMemcpy = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMem_Fill = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap_AMD_fn)cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap_AMD_fn)cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            ಹಿಡಿ (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                ಹೋಗು noSVM;
            }
        }

        svm_ಲಭ್ಯ = ನಿಜ;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = ನಿಜ;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        ಹಿಂತಿರುಗಿ;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_ಲಭ್ಯ = ಸುಳ್ಳು;
        svmEnabled = ಸುಳ್ಳು;
        svmCapabilities.ಮೌಲ್ಯ_2 = 0;
        svmInitialized = ನಿಜ;
        svmFunctions.fn_clSVMAlloc = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> ಬಫರ್_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_ಬಫರ್_Pool_SVM() ಸ್ಥಿರಾಂಕ
    {
        init_ಬಫರ್_pools();
        CV_DbgAssert(ಬಫರ್_Pool_SVM);
        ಹಿಂತಿರುಗಿ *ಬಫರ್_Pool_SVM.get();
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಮಿತ್ರ ವರ್ಗ ಕಾರ್ಯಕ್ರಮ_2;
};


Context::Context() CV_NOEXCEPT
{
    p = 0;
}

Context::~Context()
{
    release();
}

// deprecated
Context::Context(ಪೂರ್ಣಾಂಕ dtype)
{
    p = 0;
    create(dtype);
}

ಶೂನ್ಯ Context::release()
{
    ಆದರೆ (p)
    {
        p->release();
        p = ಖಾಲಿ;
    }
}

ತಾರ್ಕಿಕ Context::create()
{
    release();
    ಆದರೆ (!haveOpenCL())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    p = Impl::ಕಂಡುಹಿಡಿ_Or_Create_Context(std::string());
    ಆದರೆ (p && p->handle)
        ಹಿಂತಿರುಗಿ ನಿಜ;
    release();
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

// deprecated
ತಾರ್ಕಿಕ Context::create(ಪೂರ್ಣಾಂಕ dtype)
{
    ಆದರೆ( !haveOpenCL() )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    release();
    ಆದರೆ (dtype == CL_DEVICE_TYPE_DEFAULT || (ಚಿಹ್ನೆರಹಿತ)dtype == (ಚಿಹ್ನೆರಹಿತ)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::ಕಂಡುಹಿಡಿ_Or_Create_Context("");
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::ಕಂಡುಹಿಡಿ_Or_Create_Context(":GPU:");
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::ಕಂಡುಹಿಡಿ_Or_Create_Context(":CPU:");
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_LOG_ERROR(ಖಾಲಿ, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    ಆದರೆ (p && !p->handle)
    {
        release();
    }
    ಹಿಂತಿರುಗಿ p != 0;
}

Context::Context(ಸ್ಥಿರಾಂಕ Context& c)
{
    p = (Impl*)c.p;
    ಆದರೆ(p)
        p->addref();
}

Context& Context::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Context& c)
{
    Impl* newp = (Impl*)c.p;
    ಆದರೆ(newp)
        newp->addref();
    ಆದರೆ(p)
        p->release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = ಶೂನ್ಯಸೂಚಕ;
}

Context& Context::ಕ್ರಿಯಾಕಾರಕ = (Context&& c) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &c) {
        ಆದರೆ(p)
            p->release();
        p = c.p;
        c.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಶೂನ್ಯ* Context::ptr() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p == ಖಾಲಿ ? ಖಾಲಿ : p->handle;
}

size_t Context::ndevices() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರ Device dummy;
    ಹಿಂತಿರುಗಿ !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    ಸ್ವಯಂಚಾಲಿತ& c = OpenCLExecutionContext::getCurrent();
    ಆದರೆ (!c.empty())
    {
        ಸ್ವಯಂಚಾಲಿತ& ctx = c.getContext();
        ಹಿಂತಿರುಗಿ ctx;
    }

    ಸ್ಥಿರ Context dummy;
    ಹಿಂತಿರುಗಿ dummy;
}

ಕಾರ್ಯಕ್ರಮ_2 Context::getProg(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& prog,
                         ಸ್ಥಿರಾಂಕ String& buildopts, String& errmsg)
{
    ಹಿಂತಿರುಗಿ p ? p->getProg(prog, buildopts, errmsg) : ಕಾರ್ಯಕ್ರಮ_2();
}

ಶೂನ್ಯ Context::unloadProg(ಕಾರ್ಯಕ್ರಮ_2& prog)
{
    ಆದರೆ (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(ಶೂನ್ಯ* context)
{
    Context ctx;
    ctx.p = Impl::ಕಂಡುಹಿಡಿ_Or_Create_Context((cl_context)context);
    ಹಿಂತಿರುಗಿ ctx;
}

/* static */
Context Context::fromDevice(ಸ್ಥಿರಾಂಕ ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::ಕಂಡುಹಿಡಿ_Or_Create_Context(device);
    ಹಿಂತಿರುಗಿ ctx;
}

/* static */
Context Context::create(ಸ್ಥಿರಾಂಕ std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::ಕಂಡುಹಿಡಿ_Or_Create_Context(configuration);
    ಹಿಂತಿರುಗಿ ctx;
}

ಶೂನ್ಯ* Context::getOpenCLContextProperty(ಪೂರ್ಣಾಂಕ propertyId) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (p == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, ಖಾಲಿ, &size));
    std::vector<cl_context_properties> prop(size / ಗಾತ್ರ(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.ದತ್ತಾಂಶ(), ಖಾಲಿ));
    ಗಾಗಿ (size_t i = 0; i < prop.size(); i += 2)
    {
        ಆದರೆ (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "OpenCL: found context property=" << propertyId << ") => " << (ಶೂನ್ಯ*)prop[i + 1]);
            ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ*)prop[i + 1];
        }
    }
    ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
ತಾರ್ಕಿಕ Context::useSVM() ಸ್ಥಿರಾಂಕ
{
    Context::Impl* i = p;
    CV_Assert(i);
    ಆದರೆ (!i->svmInitialized)
        i->svmInit();
    ಹಿಂತಿರುಗಿ i->svmEnabled;
}
ಶೂನ್ಯ Context::ಗಣ_Use_SVM(ತಾರ್ಕಿಕ enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    ಆದರೆ (!i->svmInitialized)
        i->svmInit();
    ಆದರೆ (enabled && !i->svm_ಲಭ್ಯ)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#ಇಲ್ಲದಿದ್ದರೆ
ತಾರ್ಕಿಕ Context::useSVM() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
ಶೂನ್ಯ Context::ಗಣ_Use_SVM(ತಾರ್ಕಿಕ enabled) { CV_Assert(!enabled); }
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
ನಾಮಾಕಾಶ svm {

ಸ್ಥಿರಾಂಕ SVMCapabilities getSVMCapabilitites(ಸ್ಥಿರಾಂಕ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    ಆದರೆ (!i->svmInitialized)
        i->svmInit();
    ಹಿಂತಿರುಗಿ i->svmCapabilities;
}

CV_EXPORTS ಸ್ಥಿರಾಂಕ SVMFunctions* getSVMFunctions(ಸ್ಥಿರಾಂಕ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != ಖಾಲಿ);
    ಹಿಂತಿರುಗಿ &i->svmFunctions;
}

CV_EXPORTS ತಾರ್ಕಿಕ useSVM(UMatUsageFlags usageFlags)
{
    ಆದರೆ (checkForceSVMUmatUsage())
        ಹಿಂತಿರುಗಿ ನಿಜ;
    ಆದರೆ (checkDisableSVMUMatUsage())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಆದರೆ ((usageFlags & USAGE_ALLOCATE_SHARED_ಸ್ಮರಣೆ) != 0)
        ಹಿಂತಿರುಗಿ ನಿಜ;
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; // don't use SVM by default
}

} // namespace cv::ocl::svm
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

ಶೂನ್ಯ Context::ಗಣ_User_Context(std::type_ಸೂಚ್ಯಂಕ typeId, ಸ್ಥಿರಾಂಕ std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->ಗಣ_User_Context(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_ಸೂಚ್ಯಂಕ typeId)
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->getUserContext(typeId);
}

ಸ್ಥಿರ ಶೂನ್ಯ get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    Auto_ಬಫರ್<ಅಕ್ಷರ> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.ದತ್ತಾಂಶ(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.ದತ್ತಾಂಶ();
}

/*
// Attaches OpenCL context to OpenCV
*/
ಶೂನ್ಯ attachContext(ಸ್ಥಿರಾಂಕ String& platformName, ಶೂನ್ಯ* platformID, ಶೂನ್ಯ* context, ಶೂನ್ಯ* deviceID)
{
    ಸ್ವಯಂಚಾಲಿತ ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ಸ್ಥಿರಾಂಕ std::string& platformName, ಶೂನ್ಯ* platformID, ಶೂನ್ಯ* context, ಶೂನ್ಯ* deviceID
)
{
    ಆದರೆ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    ಆದರೆ (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    ತಾರ್ಕಿಕ platform_ಲಭ್ಯ = ಸುಳ್ಳು;

    // check if external platformName contained in list of available platforms in OpenCV
    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; i < cnt; i++)
    {
        String ಲಭ್ಯ_Platform_Name;
        get_platform_name(platforms[i], ಲಭ್ಯ_Platform_Name);
        // external platform is found in the list of available platforms
        ಆದರೆ (platformName == ಲಭ್ಯ_Platform_Name)
        {
            platform_ಲಭ್ಯ = ನಿಜ;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ (!platform_ಲಭ್ಯ)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    ಆದರೆ (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    ಹಿಂತಿರುಗಿ ctx;
}

ಶೂನ್ಯ initializeContextFromHandle(Context& ctx, ಶೂನ್ಯ* _platform, ಶೂನ್ಯ* _context, ಶೂನ್ಯ* _device)
{
    // internal call, less checks
    cl_platform_id platformID = (cl_platform_id)_platform;
    cl_context context = (cl_context)_context;
    cl_device_id deviceID = (cl_device_id)_device;

    std::string platformName = PlatformInfo(&platformID).name();

    ಸ್ವಯಂಚಾಲಿತ clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    CV_Assert(!clExecCtx.empty());
    ctx = clExecCtx.getContext();
}

/////////////////////////////////////////// Queue /////////////////////////////////////////////

ರಚನೆ ಸರತಿ::Impl
{
    ಸಾಲೊಳಗೆ ಶೂನ್ಯ __init()
    {
        refcount = 1;
        handle = 0;
        is_Profiling_ಸರತಿ = ಸುಳ್ಳು;
    }

    Impl(cl_command_ಸರತಿ q)
    {
        __init();
        handle = q;

        cl_command_ಸರತಿ_properties props = 0;
        CV_OCL_CHECK(cl_Get_Command_ಸರತಿ_Info(handle, CL_ಸರತಿ_PROPERTIES, ಗಾತ್ರ(cl_command_ಸರತಿ_properties), &props, ಖಾಲಿ));
        is_Profiling_ಸರತಿ = !!(props & CL_ಸರತಿ_PROFILING_ENABLE);
    }

    Impl(cl_command_ಸರತಿ q, ತಾರ್ಕಿಕ is_Profiling_ಸರತಿ_2)
    {
        __init();
        handle = q;
        is_Profiling_ಸರತಿ = is_Profiling_ಸರತಿ_2;
    }

    Impl(ಸ್ಥಿರಾಂಕ Context& c, ಸ್ಥಿರಾಂಕ Device& d, ತಾರ್ಕಿಕ withProfiling = ಸುಳ್ಳು)
    {
        __init();

        ಸ್ಥಿರಾಂಕ Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        ಆದರೆ( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        ಆದರೆ( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_ಸರತಿ_properties props = withProfiling ? CL_ಸರತಿ_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = cl_Create_Command_ಸರತಿ(ch, dh, props, &retval), retval);
        is_Profiling_ಸರತಿ = withProfiling;
    }

    ~Impl()
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
        ಆದರೆ (!cv::__termination)
#ಆದರೆ_ಅಂತ್ಯ
        {
            ಆದರೆ(handle)
            {
                CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ(handle));
                CV_OCL_DBG_CHECK(cl_Release_Command_ಸರತಿ(handle));
                handle = ಖಾಲಿ;
            }
        }
    }

    ಸ್ಥಿರಾಂಕ cv::ocl::ಸರತಿ& get_Profiling_ಸರತಿ(ಸ್ಥಿರಾಂಕ cv::ocl::ಸರತಿ& self)
    {
        ಆದರೆ (is_Profiling_ಸರತಿ)
            ಹಿಂತಿರುಗಿ self;

        ಆದರೆ (profiling_ಸರತಿ.ptr())
            ಹಿಂತಿರುಗಿ profiling_ಸರತಿ;

        cl_context ctx = 0;
        CV_OCL_CHECK(cl_Get_Command_ಸರತಿ_Info(handle, CL_ಸರತಿ_CONTEXT, ಗಾತ್ರ(cl_context), &ctx, ಖಾಲಿ));

        cl_device_id device = 0;
        CV_OCL_CHECK(cl_Get_Command_ಸರತಿ_Info(handle, CL_ಸರತಿ_DEVICE, ಗಾತ್ರ(cl_device_id), &device, ಖಾಲಿ));

        cl_int ಫಲಿತಾಂಶ = CL_SUCCESS;
        cl_command_ಸರತಿ_properties props = CL_ಸರತಿ_PROFILING_ENABLE;
        cl_command_ಸರತಿ q = cl_Create_Command_ಸರತಿ(ctx, device, props, &ಫಲಿತಾಂಶ);
        CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಫಲಿತಾಂಶ, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        ಸರತಿ ಸರತಿ_2;
        ಸರತಿ_2.p = ಹೊಸ Impl(q, ನಿಜ);
        profiling_ಸರತಿ = ಸರತಿ_2;

        ಹಿಂತಿರುಗಿ profiling_ಸರತಿ;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_ಸರತಿ handle;
    ತಾರ್ಕಿಕ is_Profiling_ಸರತಿ;
    cv::ocl::ಸರತಿ profiling_ಸರತಿ;
};

ಸರತಿ::ಸರತಿ() CV_NOEXCEPT
{
    p = 0;
}

ಸರತಿ::ಸರತಿ(ಸ್ಥಿರಾಂಕ Context& c, ಸ್ಥಿರಾಂಕ Device& d)
{
    p = 0;
    create(c, d);
}

ಸರತಿ::ಸರತಿ(ಸ್ಥಿರಾಂಕ ಸರತಿ& q)
{
    p = q.p;
    ಆದರೆ(p)
        p->addref();
}

ಸರತಿ& ಸರತಿ::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ ಸರತಿ& q)
{
    Impl* newp = (Impl*)q.p;
    ಆದರೆ(newp)
        newp->addref();
    ಆದರೆ(p)
        p->release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಸರತಿ::ಸರತಿ(ಸರತಿ&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = ಶೂನ್ಯಸೂಚಕ;
}

ಸರತಿ& ಸರತಿ::ಕ್ರಿಯಾಕಾರಕ = (ಸರತಿ&& q) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &q) {
        ಆದರೆ(p)
            p->release();
        p = q.p;
        q.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಸರತಿ::~ಸರತಿ()
{
    ಆದರೆ(p)
        p->release();
}

ತಾರ್ಕಿಕ ಸರತಿ::create(ಸ್ಥಿರಾಂಕ Context& c, ಸ್ಥಿರಾಂಕ Device& d)
{
    ಆದರೆ(p)
        p->release();
    p = ಹೊಸ Impl(c, d);
    ಹಿಂತಿರುಗಿ p->handle != 0;
}

ಶೂನ್ಯ ಸರತಿ::ಮುಕ್ತಾಯ_2()
{
    ಆದರೆ(p && p->handle)
    {
        CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ(p->handle));
    }
}

ಸ್ಥಿರಾಂಕ ಸರತಿ& ಸರತಿ::get_Profiling_ಸರತಿ() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->get_Profiling_ಸರತಿ(*ಇದು);
}

ಶೂನ್ಯ* ಸರತಿ::ptr() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->handle : 0;
}

ಸರತಿ& ಸರತಿ::getDefault()
{
    ಸ್ವಯಂಚಾಲಿತ& c = OpenCLExecutionContext::getCurrent();
    ಆದರೆ (!c.empty())
    {
        ಸ್ವಯಂಚಾಲಿತ& q = c.get_ಸರತಿ();
        ಹಿಂತಿರುಗಿ q;
    }
    ಸ್ಥಿರ ಸರತಿ dummy;
    ಹಿಂತಿರುಗಿ dummy;
}

ಸ್ಥಿರ cl_command_ಸರತಿ get_ಸರತಿ(ಸ್ಥಿರಾಂಕ ಸರತಿ& q)
{
    cl_command_ಸರತಿ qq = (cl_command_ಸರತಿ)q.ptr();
    ಆದರೆ(!qq)
        qq = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಹಿಂತಿರುಗಿ qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ಪೂರ್ಣಾಂಕ _flags, UMat* _m, ಪೂರ್ಣಾಂಕ _wscale, ಪೂರ್ಣಾಂಕ _iwscale, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != ಖಾಲಿ);
}

KernelArg KernelArg::Constant(ಸ್ಥಿರಾಂಕ Mat& m)
{
    CV_Assert(m.isContinuous());
    ಹಿಂತಿರುಗಿ KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.ಒಟ್ಟು()*m.elem_ಗಾತ್ರ());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

ರಚನೆ Kernel::Impl
{
    Impl(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog) :
        refcount(1), handle(ಖಾಲಿ), is_ಒಳ_ಪ್ರಗತಿ(ಸುಳ್ಳು), is_Async_ಸತತ(ಸುಳ್ಳು), nu(0)
    {
        cl_ಕಾರ್ಯಕ್ರಮ ph = (cl_ಕಾರ್ಯಕ್ರಮ)prog.ptr();
        cl_int retval = 0;
        name = kname;
        ಆದರೆ (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_ತಾತ್ಕಾಲಿಕ_Dst_UMats = ಸುಳ್ಳು;
        have_ತಾತ್ಕಾಲಿಕ_Src_UMats = ಸುಳ್ಳು;
    }

    ಶೂನ್ಯ cleanupUMats()
    {
        ತಾರ್ಕಿಕ exceptionOccurred = ಸುಳ್ಳು;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < MAX_ARRS; i++ )
        {
            ಆದರೆ( u[i] )
            {
                ಆದರೆ( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMat_ದತ್ತಾಂಶ::ASYNC_CLEANUP;
                    ಪ್ರಯತ್ನಿಸು
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    ಹಿಡಿ(ಸ್ಥಿರಾಂಕ std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(ಖಾಲಿ, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = ನಿಜ;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_ತಾತ್ಕಾಲಿಕ_Dst_UMats = ಸುಳ್ಳು;
        have_ತಾತ್ಕಾಲಿಕ_Src_UMats = ಸುಳ್ಳು;
        CV_Assert(!exceptionOccurred);
    }

    ಶೂನ್ಯ ಕೂಡಿಸು_UMat(ಸ್ಥಿರಾಂಕ UMat& m, ತಾರ್ಕಿಕ dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        ಆದರೆ(dst && m.u->ತಾತ್ಕಾಲಿಕ_UMat())
            have_ತಾತ್ಕಾಲಿಕ_Dst_UMats = ನಿಜ;
        ಆದರೆ(m.u->original_UMat_ದತ್ತಾಂಶ == ಖಾಲಿ && m.u->ತಾತ್ಕಾಲಿಕ_UMat())
            have_ತಾತ್ಕಾಲಿಕ_Src_UMats = ನಿಜ;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    ಶೂನ್ಯ ರಿಜಿಸ್ಟರ್_Image_ವಾದ(ಪೂರ್ಣಾಂಕ arg, ಸ್ಥಿರಾಂಕ Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        ಆದರೆ (arg < (ಪೂರ್ಣಾಂಕ)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_ಒಳ_ಪ್ರಗತಿ, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    ಶೂನ್ಯ finit(cl_event e)
    {
        CV_ಬಳಸದ(e);
        is_ಒಳ_ಪ್ರಗತಿ = ಸುಳ್ಳು;
        ಪ್ರಯತ್ನಿಸು
        {
            cleanupUMats();
        }
        ಹಿಡಿ(...)
        {
            release();
            ಎಸೆ;
        }
        release();
    }

    ತಾರ್ಕಿಕ ಸತತ(ಪೂರ್ಣಾಂಕ dims, size_t _globalsize[], size_t _localsize[],
            ತಾರ್ಕಿಕ ಸಮಕಾಲೀಕರಣ, int64* ಸಮಯ_NS, ಸ್ಥಿರಾಂಕ ಸರತಿ& q);

    ~Impl()
    {
        ಆದರೆ(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    ಎಣಿಕೆ { MAX_ARRS = 16 };
    UMat_ದತ್ತಾಂಶ* u[MAX_ARRS];
    ತಾರ್ಕಿಕ is_ಒಳ_ಪ್ರಗತಿ;
    ತಾರ್ಕಿಕ is_Async_ಸತತ;  // true if kernel was scheduled in async mode
    ಪೂರ್ಣಾಂಕ nu;
    std::vector<Image2D> shadow_images;
    ತಾರ್ಕಿಕ have_ತಾತ್ಕಾಲಿಕ_Dst_UMats;
    ತಾರ್ಕಿಕ have_ತಾತ್ಕಾಲಿಕ_Src_UMats;
};

}} // namespace cv::ocl

ಬಾಹ್ಯ "C" {

ಸ್ಥಿರ ಶೂನ್ಯ CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, ಶೂನ್ಯ *p)
{
    ಪ್ರಯತ್ನಿಸು
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& exc)
    {
        CV_LOG_ERROR(ಖಾಲಿ, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ std::exception& exc)
    {
        CV_LOG_ERROR(ಖಾಲಿ, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    ಹಿಡಿ (...)
    {
        CV_LOG_ERROR(ಖಾಲಿ, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src,
               ಸ್ಥಿರಾಂಕ String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ಸ್ಥಿರಾಂಕ Kernel& k)
{
    p = k.p;
    ಆದರೆ(p)
        p->addref();
}

Kernel& Kernel::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    ಆದರೆ(newp)
        newp->addref();
    ಆದರೆ(p)
        p->release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = ಶೂನ್ಯಸೂಚಕ;
}

Kernel& Kernel::ಕ್ರಿಯಾಕಾರಕ = (Kernel&& k) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &k) {
        ಆದರೆ(p)
            p->release();
        p = k.p;
        k.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Kernel::~Kernel()
{
    ಆದರೆ(p)
        p->release();
}

ತಾರ್ಕಿಕ Kernel::create(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog)
{
    ಆದರೆ(p)
        p->release();
    p = ಹೊಸ Impl(kname, prog);
    ಆದರೆ(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_OPENCL_ಸತತ_ASSERT // check kernel compilation fails
    CV_Assert(p);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ p != 0;
}

ತಾರ್ಕಿಕ Kernel::create(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src,
                    ಸ್ಥಿರಾಂಕ String& buildopts, String* errmsg)
{
    ಆದರೆ(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    ಆದರೆ( !errmsg ) errmsg = &tempmsg;
    ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2 prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    ಹಿಂತಿರುಗಿ create(kname, prog);
}

ಶೂನ್ಯ* Kernel::ptr() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->handle : 0;
}

ತಾರ್ಕಿಕ Kernel::empty() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ptr() == 0;
}

ಸ್ಥಿರ cv::String dump_ಮೌಲ್ಯ(size_t sz, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* p)
{
    ಆದರೆ (!p)
        ಹಿಂತಿರುಗಿ "NULL";
    ಆದರೆ (sz == 2)
        ಹಿಂತಿರುಗಿ cv::format("%d / %uu / 0x%04x", *(ಗಿಡ್ಡ*)p, *(ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ*)p, *(ಗಿಡ್ಡ*)p);
    ಆದರೆ (sz == 4)
        ಹಿಂತಿರುಗಿ cv::format("%d / %uu / 0x%08x / %g", *(ಪೂರ್ಣಾಂಕ*)p, *(ಪೂರ್ಣಾಂಕ*)p, *(ಪೂರ್ಣಾಂಕ*)p, *(ದಶಮಾಂಶ*)p);
    ಆದರೆ (sz == 8)
        ಹಿಂತಿರುಗಿ cv::format("%lld / %lluu / 0x%16llx / %g", *(ದೀರ್ಘ ದೀರ್ಘ*)p, *(ದೀರ್ಘ ದೀರ್ಘ*)p, *(ದೀರ್ಘ ದೀರ್ಘ*)p, *(ದ್ವಿಗುಣ*)p);
    ಹಿಂತಿರುಗಿ cv::format("%p", p);
}

ಪೂರ್ಣಾಂಕ Kernel::ಗಣ(ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* value, size_t sz)
{
    ಆದರೆ (!p || !p->handle)
        ಹಿಂತಿರುಗಿ -1;
    ಆದರೆ (i < 0)
        ಹಿಂತಿರುಗಿ i;
    ಆದರೆ( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i, (ಪೂರ್ಣಾಂಕ)sz, dump_ಮೌಲ್ಯ(sz, value).c_str()).c_str());
    ಆದರೆ (retval != CL_SUCCESS)
        ಹಿಂತಿರುಗಿ -1;
    ಹಿಂತಿರುಗಿ i+1;
}

ಪೂರ್ಣಾಂಕ Kernel::ಗಣ(ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿರಾಂಕ Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    ಪೂರ್ಣಾಂಕ res = ಗಣ(i, &h, ಗಾತ್ರ(h));
    ಆದರೆ (res >= 0)
        p->ರಿಜಿಸ್ಟರ್_Image_ವಾದ(i, image2D);
    ಹಿಂತಿರುಗಿ res;
}

ಪೂರ್ಣಾಂಕ Kernel::ಗಣ(ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿರಾಂಕ UMat& m)
{
    ಹಿಂತಿರುಗಿ ಗಣ(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ಸ್ಥಿರ ತಾರ್ಕಿಕ isCudaBacked(ಸ್ಥಿರಾಂಕ UMat_ದತ್ತಾಂಶ* u)
{
    ಸ್ಥಿರಾಂಕ MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    ಹಿಂತಿರುಗಿ u && cudaAllocator && u->currAllocator == cudaAllocator;
}

ಪೂರ್ಣಾಂಕ Kernel::ಗಣ(ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿರಾಂಕ KernelArg& arg)
{
    ಆದರೆ( !p || !p->handle )
        ಹಿಂತಿರುಗಿ -1;
    ಆದರೆ (i < 0)
    {
        CV_LOG_ERROR(ಖಾಲಿ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i));
        ಹಿಂತಿರುಗಿ i;
    }
    ಆದರೆ( i == 0 )
        p->cleanupUMats();
    cl_int ಸ್ಥಿತಿ_2 = 0;
    ಆದರೆ( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        ತಾರ್ಕಿಕ ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        ಆದರೆ (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)ಖಾಲಿ;
            ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)i, ಗಾತ್ರ(h_null), &h_null);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i).c_str());
            ಹಿಂತಿರುಗಿ i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        ಆದರೆ (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(ಖಾಲಿ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            ಹಿಂತಿರುಗಿ -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        ಆದರೆ (!h)
        {
            CV_LOG_ERROR(ಖಾಲಿ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i, (ಪೂರ್ಣಾಂಕ)arg.flags, arg.m));
            p->release();
            p = 0;
            ಹಿಂತಿರುಗಿ -1;
        }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ಆದರೆ ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
        {
            ಸ್ಥಿರಾಂಕ Context& ctx = Context::getDefault();
            ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_ದತ್ತಾಂಶ_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_ದತ್ತಾಂಶ_Ptr);
#ಆದರೆ 1 // TODO
            ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಗಣ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_ದತ್ತಾಂಶ_Ptr);
#ಇಲ್ಲದಿದ್ದರೆ
            ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಗಣ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_ದತ್ತಾಂಶ_Ptr);
#ಆದರೆ_ಅಂತ್ಯ
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i, (ಶೂನ್ಯ*)svm_ದತ್ತಾಂಶ_Ptr).c_str());
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        {
            ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)i, ಗಾತ್ರ(h), &h);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i, (ಶೂನ್ಯ*)h).c_str());
        }

        ಆದರೆ (ptronly)
        {
            i++;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)(i+1), ಗಾತ್ರ(u2d.ಹೆಜ್ಜೆ), &u2d.ಹೆಜ್ಜೆ);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)(i+1), (ಪೂರ್ಣಾಂಕ)u2d.ಹೆಜ್ಜೆ).c_str());
            ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)(i+2), ಗಾತ್ರ(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)(i+2), (ಪೂರ್ಣಾಂಕ)u2d.offset).c_str());
            i += 3;

            ಆದರೆ( !(arg.flags & KernelArg::NO_ಗಾತ್ರ) )
            {
                ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = u2d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*arg.wscale/arg.iwscale;
                ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)i, ಗಾತ್ರ(u2d.ಅಡ್ಡಸಾಲುಗಳು), &u2d.ಅಡ್ಡಸಾಲುಗಳು);
                CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i, (ಪೂರ್ಣಾಂಕ)u2d.ಅಡ್ಡಸಾಲುಗಳು).c_str());
                ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)(i+1), ಗಾತ್ರ(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ), &ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
                CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)(i+1), (ಪೂರ್ಣಾಂಕ)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ).c_str());
                i += 2;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            UMat3D u3d(*arg.m);
            ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)(i+1), ಗಾತ್ರ(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)(i+1), (ಪೂರ್ಣಾಂಕ)u3d.slicestep).c_str());
            ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)(i+2), ಗಾತ್ರ(u3d.ಹೆಜ್ಜೆ), &u3d.ಹೆಜ್ಜೆ);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)(i+2), (ಪೂರ್ಣಾಂಕ)u3d.ಹೆಜ್ಜೆ).c_str());
            ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)(i+3), ಗಾತ್ರ(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)(i+3), (ಪೂರ್ಣಾಂಕ)u3d.offset).c_str());
            i += 4;
            ಆದರೆ( !(arg.flags & KernelArg::NO_ಗಾತ್ರ) )
            {
                ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = u3d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*arg.wscale/arg.iwscale;
                ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)i, ಗಾತ್ರ(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i, (ಪೂರ್ಣಾಂಕ)u3d.slices).c_str());
                ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)(i+1), ಗಾತ್ರ(u3d.ಅಡ್ಡಸಾಲುಗಳು), &u3d.ಅಡ್ಡಸಾಲುಗಳು);
                CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)(i+1), (ಪೂರ್ಣಾಂಕ)u3d.ಅಡ್ಡಸಾಲುಗಳು).c_str());
                ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)(i+2), ಗಾತ್ರ(u3d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ), &ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
                CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)(i+2), (ಪೂರ್ಣಾಂಕ)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ).c_str());
                i += 3;
            }
        }
        p->ಕೂಡಿಸು_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        ಹಿಂತಿರುಗಿ i;
    }
    ಸ್ಥಿತಿ_2 = cl_ಗಣ_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (ಪೂರ್ಣಾಂಕ)i, (ಪೂರ್ಣಾಂಕ)arg.sz, (ಶೂನ್ಯ*)arg.obj).c_str());
    ಹಿಂತಿರುಗಿ i+1;
}

ತಾರ್ಕಿಕ Kernel::ಸತತ(ಪೂರ್ಣಾಂಕ dims, size_t _globalsize[], size_t _localsize[],
                 ತಾರ್ಕಿಕ ಸಮಕಾಲೀಕರಣ, ಸ್ಥಿರಾಂಕ ಸರತಿ& q)
{
    ಆದರೆ (!p)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t ಒಟ್ಟು = 1;
    CV_Assert(_globalsize != ಖಾಲಿ);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(ಪೂರ್ಣಾಂಕ)(i>0)) : 1;
        CV_Assert( val > 0 );
        ಒಟ್ಟು *= _globalsize[i];
        ಆದರೆ (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_ಮೇಲೆ(_globalsize[i], (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)val) * val;
    }
    CV_Assert(ಒಟ್ಟು > 0);

    ಹಿಂತಿರುಗಿ p->ಸತತ(dims, globalsize, _localsize, ಸಮಕಾಲೀಕರಣ, ಖಾಲಿ, q);
}


ತಾರ್ಕಿಕ Kernel::ಸತತ_2(ಪೂರ್ಣಾಂಕ dims, size_t _globalsize[], size_t _localsize[],
                  ತಾರ್ಕಿಕ ಸಮಕಾಲೀಕರಣ, ಸ್ಥಿರಾಂಕ ಸರತಿ& q)
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->ಸತತ(dims, _globalsize, _localsize, ಸಮಕಾಲೀಕರಣ, ಖಾಲಿ, q);
}


ತಾರ್ಕಿಕ Kernel::Impl::ಸತತ(ಪೂರ್ಣಾಂಕ dims, size_t globalsize[], size_t localsize[],
        ತಾರ್ಕಿಕ ಸಮಕಾಲೀಕರಣ, int64* ಸಮಯ_NS, ಸ್ಥಿರಾಂಕ ಸರತಿ& q)
{
    CV_INSTRUMENT_REGION_OPENCL_ಸತತ(name.c_str());

    ಆದರೆ (!handle)
    {
        CV_LOG_ERROR(ಖಾಲಿ, "OpenCL kernel has zero handle: " << name);
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಆದರೆ (is_Async_ಸತತ)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_ಸತತ = !ಸಮಕಾಲೀಕರಣ;

    ಆದರೆ (is_ಒಳ_ಪ್ರಗತಿ)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#ಆದರೆ CV_OPENCL_SYNC_ಸತತ_KERNELS
    ಸಮಕಾಲೀಕರಣ = ನಿಜ;
#ಆದರೆ_ಅಂತ್ಯ

    cl_command_ಸರತಿ qq = get_ಸರತಿ(q);
    ಆದರೆ (have_ತಾತ್ಕಾಲಿಕ_Dst_UMats)
        ಸಮಕಾಲೀಕರಣ = ನಿಜ;
    ಆದರೆ (have_ತಾತ್ಕಾಲಿಕ_Src_UMats)
        ಸಮಕಾಲೀಕರಣ = ನಿಜ;
    ಆದರೆ (ಸಮಯ_NS)
        ಸಮಕಾಲೀಕರಣ = ನಿಜ;
    cl_event asyncEvent = 0;
    cl_int retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           ಖಾಲಿ, globalsize, localsize, 0, 0,
                                           (ಸಮಕಾಲೀಕರಣ && !ಸಮಯ_NS) ? 0 : &asyncEvent);
#ಆದರೆ !CV_OPENCL_SHOW_ಸತತ_KERNELS
    ಆದರೆ (retval != CL_SUCCESS)
#ಆದರೆ_ಅಂತ್ಯ
    {
        cv::String ಸಂದೇಶ = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (ಪೂರ್ಣಾಂಕ)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        ಸಮಕಾಲೀಕರಣ ? "true" : "false"
                        );
        ಆದರೆ (retval != CL_SUCCESS)
        {
            ಸಂದೇಶ = CV_OCL_API_ERROR_ಸಂದೇಶ(retval, ಸಂದೇಶ.c_str());
        }
#ಆದರೆ CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_ಫಲಿತಾಂಶ(retval, ಸಂದೇಶ.c_str());
#ಇಲ್ಲದಿದ್ದರೆ
        ಬರೆ("%s\n", ಸಂದೇಶ.c_str());
        fflush(stdout);
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಆದರೆ (ಸಮಕಾಲೀಕರಣ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ(qq));
        ಆದರೆ (ಸಮಯ_NS)
        {
            ಆದರೆ (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong ಆರಂಭ_ಸಮಯ, stop_ಸಮಯ;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_ಆರಂಭ, ಗಾತ್ರ(ಆರಂಭ_ಸಮಯ), &ಆರಂಭ_ಸಮಯ, ಖಾಲಿ));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_ಕೊನೆ, ಗಾತ್ರ(stop_ಸಮಯ), &stop_ಸಮಯ, ಖಾಲಿ));
                *ಸಮಯ_NS = (int64)(stop_ಸಮಯ - ಆರಂಭ_ಸಮಯ);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                *ಸಮಯ_NS = -1;
            }
        }
        cleanupUMats();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        addref();
        is_ಒಳ_ಪ್ರಗತಿ = ನಿಜ;
        CV_OCL_CHECK(cl_ಗಣ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, ಇದು));
    }
    ಆದರೆ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ಹಿಂತಿರುಗಿ retval == CL_SUCCESS;
}

ತಾರ್ಕಿಕ Kernel::ಸತತ_Task(ತಾರ್ಕಿಕ ಸಮಕಾಲೀಕರಣ, ಸ್ಥಿರಾಂಕ ಸರತಿ& q)
{
    ಆದರೆ(!p || !p->handle || p->is_ಒಳ_ಪ್ರಗತಿ)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    cl_command_ಸರತಿ qq = get_ಸರತಿ(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Task(qq, p->handle, 0, 0, ಸಮಕಾಲೀಕರಣ ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), ಸಮಕಾಲೀಕರಣ ? "true" : "false").c_str());
    ಆದರೆ (ಸಮಕಾಲೀಕರಣ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ(qq));
        p->cleanupUMats();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        p->addref();
        p->is_ಒಳ_ಪ್ರಗತಿ = ನಿಜ;
        CV_OCL_CHECK(cl_ಗಣ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    ಆದರೆ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ಹಿಂತಿರುಗಿ retval == CL_SUCCESS;
}

int64 Kernel::ಸತತ_Profiling(ಪೂರ್ಣಾಂಕ dims, size_t globalsize[], size_t localsize[], ಸ್ಥಿರಾಂಕ ಸರತಿ& q_)
{
    CV_Assert(p && p->handle && !p->is_ಒಳ_ಪ್ರಗತಿ);
    ಸರತಿ q = q_.ptr() ? q_ : ಸರತಿ::getDefault();
    CV_Assert(q.ptr());
    q.ಮುಕ್ತಾಯ_2(); // call clFinish() on base queue
    ಸರತಿ profiling_ಸರತಿ_2 = q.get_Profiling_ಸರತಿ();
    int64 ಸಮಯ_Ns = -1;
    ತಾರ್ಕಿಕ res = p->ಸತತ(dims, globalsize, localsize, ನಿಜ, &ಸಮಯ_Ns, profiling_ಸರತಿ_2);
    ಹಿಂತಿರುಗಿ res ? ಸಮಯ_Ns : -1;
}

size_t Kernel::work_ಗುಂಪು_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!p || !p->handle)
        ಹಿಂತಿರುಗಿ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int ಸ್ಥಿತಿ_2 = cl_Get_Kernel_Work_ಗುಂಪು_Info(p->handle, dev, CL_KERNEL_WORK_ಗುಂಪು_ಗಾತ್ರ, ಗಾತ್ರ(val), &val, &retsz);
    CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_ಗುಂಪು_ಗಾತ್ರ_ಅಪವರ್ತ್ಯ() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!p || !p->handle)
        ಹಿಂತಿರುಗಿ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int ಸ್ಥಿತಿ_2 = cl_Get_Kernel_Work_ಗುಂಪು_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_ಗುಂಪು_ಗಾತ್ರ_ಅಪವರ್ತ್ಯ, ಗಾತ್ರ(val), &val, &retsz);
    CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 == CL_SUCCESS ? val : 0;
}

ತಾರ್ಕಿಕ Kernel::compile_Work_ಗುಂಪು_ಗಾತ್ರ(size_t wsz[]) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!p || !p->handle || !wsz)
        ಹಿಂತಿರುಗಿ 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int ಸ್ಥಿತಿ_2 = cl_Get_Kernel_Work_ಗುಂಪು_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_ಗುಂಪು_ಗಾತ್ರ, ಗಾತ್ರ(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 == CL_SUCCESS;
}

size_t Kernel::local_Mem_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!p || !p->handle)
        ಹಿಂತಿರುಗಿ 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int ಸ್ಥಿತಿ_2 = cl_Get_Kernel_Work_ಗುಂಪು_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_ಗಾತ್ರ, ಗಾತ್ರ(val), &val, &retsz);
    CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

ರಚನೆ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    ಎಣಿಕೆ ಬಗೆ_4 {
        ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ = 0,
        ಕಾರ್ಯಕ್ರಮ_BINARIES,
        ಕಾರ್ಯಕ್ರಮ_SPIR,
        ಕಾರ್ಯಕ್ರಮ_SPIRV
    } ಬಗೆ_5;

    Impl(ಸ್ಥಿರಾಂಕ String& src)
    {
        init(ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ, cv::String(), cv::String());
        init_From_ಮೂಲ(src, cv::String());
    }
    Impl(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ String& ಸಂಕೇತ_Str, ಸ್ಥಿರಾಂಕ String& ಸಂಕೇತ_Hash)
    {
        init(ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ, module, name);
        init_From_ಮೂಲ(ಸಂಕೇತ_Str, ಸಂಕೇತ_Hash);
    }

    /// reset fields
    ಶೂನ್ಯ init(ಎಣಿಕೆ ಬಗೆ_4 ಬಗೆ, ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name)
    {
        refcount = 1;
        ಬಗೆ_5 = ಬಗೆ;
        module_ = module;
        name_ = name;

        ಮೂಲ_Addr = ಖಾಲಿ;
        ಮೂಲ_ಗಾತ್ರ = 0;
        isHashUpdated = ಸುಳ್ಳು;
    }

    ಶೂನ್ಯ init_From_ಮೂಲ(ಸ್ಥಿರಾಂಕ String& ಸಂಕೇತ_Str, ಸ್ಥಿರಾಂಕ String& ಸಂಕೇತ_Hash)
    {
        ಸಂಕೇತ_Str_2 = ಸಂಕೇತ_Str;
        ಮೂಲ_Hash = ಸಂಕೇತ_Hash;
        ಆದರೆ (ಮೂಲ_Hash.empty())
        {
            updateHash();
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            isHashUpdated = ನಿಜ;
        }
    }

    ಶೂನ್ಯ updateHash(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* hashStr = ಖಾಲಿ)
    {
        ಆದರೆ (hashStr)
        {
            ಮೂಲ_Hash = cv::String(hashStr);
            isHashUpdated = ನಿಜ;
            ಹಿಂತಿರುಗಿ;
        }
        uint64 hash = 0;
        ಆಯ್ಕೆ (ಬಗೆ_5)
        {
        ಸ್ಥಿತಿ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ:
            ಆದರೆ (ಮೂಲ_Addr)
            {
                CV_Assert(ಸಂಕೇತ_Str_2.empty());
                hash = crc64(ಮೂಲ_Addr, ಮೂಲ_ಗಾತ್ರ); // static storage
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_Assert(!ಸಂಕೇತ_Str_2.empty());
                hash = crc64((uchar*)ಸಂಕೇತ_Str_2.c_str(), ಸಂಕೇತ_Str_2.size());
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ಕಾರ್ಯಕ್ರಮ_BINARIES:
        ಸ್ಥಿತಿ ಕಾರ್ಯಕ್ರಮ_SPIR:
        ಸ್ಥಿತಿ ಕಾರ್ಯಕ್ರಮ_SPIRV:
            hash = crc64(ಮೂಲ_Addr, ಮೂಲ_ಗಾತ್ರ);
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            CV_Error(Error::Sts_ಆಂತರಿಕ, "Internal error");
        }
        ಮೂಲ_Hash = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = ನಿಜ;
    }

    Impl(ಎಣಿಕೆ ಬಗೆ_4 ಬಗೆ,
            ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name,
            ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* binary, ಸ್ಥಿರಾಂಕ size_t size,
            ಸ್ಥಿರಾಂಕ cv::String& buildOptions = cv::String())
    {
        init(ಬಗೆ, module, name);

        ಮೂಲ_Addr = binary;
        ಮೂಲ_ಗಾತ್ರ = size;

        buildOptions_ = buildOptions;
    }

    ಸ್ಥಿರ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ from_ಮೂಲ_With_Static_Lifetime(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name,
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಮೂಲ_ಸಂಕೇತ_Static_Str, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* hashStaticStr,
            ಸ್ಥಿರಾಂಕ cv::String& buildOptions)
    {
        ಕಾರ್ಯಕ್ರಮ_ಮೂಲ ಫಲಿತಾಂಶ;
        ಫಲಿತಾಂಶ.p = ಹೊಸ Impl(ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ, module, name,
                (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*)ಮೂಲ_ಸಂಕೇತ_Static_Str, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮೂಲ_ಸಂಕೇತ_Static_Str), buildOptions);
        ಫಲಿತಾಂಶ.p->updateHash(hashStaticStr);
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    ಸ್ಥಿರ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ from_ದ್ವಿಮಾನ(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name,
            ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* binary, ಸ್ಥಿರಾಂಕ size_t size,
            ಸ್ಥಿರಾಂಕ cv::String& buildOptions)
    {
        ಕಾರ್ಯಕ್ರಮ_ಮೂಲ ಫಲಿತಾಂಶ;
        ಫಲಿತಾಂಶ.p = ಹೊಸ Impl(ಕಾರ್ಯಕ್ರಮ_BINARIES, module, name, binary, size, buildOptions);
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    ಸ್ಥಿರ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ fromSPIR(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name,
            ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* binary, ಸ್ಥಿರಾಂಕ size_t size,
            ಸ್ಥಿರಾಂಕ cv::String& buildOptions)
    {
        ಕಾರ್ಯಕ್ರಮ_ಮೂಲ ಫಲಿತಾಂಶ;
        ಫಲಿತಾಂಶ.p = ಹೊಸ Impl(ಕಾರ್ಯಕ್ರಮ_SPIR, module, name, binary, size, buildOptions);
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String ಸಂಕೇತ_Str_2; // PROGRAM_SOURCE_CODE only

    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ಮೂಲ_Addr;
    size_t ಮೂಲ_ಗಾತ್ರ;

    cv::String buildOptions_;

    String ಮೂಲ_Hash;
    ತಾರ್ಕಿಕ isHashUpdated;

    ಮಿತ್ರ ರಚನೆ ಕಾರ್ಯಕ್ರಮ_2::Impl;
    ಮಿತ್ರ ರಚನೆ internal::ಕಾರ್ಯಕ್ರಮ_ನಮೂದು;
    ಮಿತ್ರ ರಚನೆ Context::Impl;
};


ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ() CV_NOEXCEPT
{
    p = 0;
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ String& ಸಂಕೇತ_Str, ಸ್ಥಿರಾಂಕ String& ಸಂಕೇತ_Hash)
{
    p = ಹೊಸ Impl(module, name, ಸಂಕೇತ_Str, ಸಂಕೇತ_Hash);
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* prog)
{
    p = ಹೊಸ Impl(prog);
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಸ್ಥಿರಾಂಕ String& prog)
{
    p = ಹೊಸ Impl(prog);
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::~ಕಾರ್ಯಕ್ರಮ_ಮೂಲ()
{
    ಆದರೆ(p)
        p->release();
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& prog)
{
    p = prog.p;
    ಆದರೆ(p)
        p->addref();
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& prog)
{
    Impl* newp = (Impl*)prog.p;
    ಆದರೆ(newp)
        newp->addref();
    ಆದರೆ(p)
        p->release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಕಾರ್ಯಕ್ರಮ_ಮೂಲ&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = ಶೂನ್ಯಸೂಚಕ;
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕ್ರಿಯಾಕಾರಕ = (ಕಾರ್ಯಕ್ರಮ_ಮೂಲ&& prog) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &prog) {
        ಆದರೆ(p)
            p->release();
        p = prog.p;
        prog.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಸ್ಥಿರಾಂಕ String& ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಮೂಲ() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    CV_Assert(p->ಬಗೆ_5 == Impl::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ);
    CV_Assert(p->ಮೂಲ_Addr == ಖಾಲಿ); // method returns reference - can't construct temporary object
    ಹಿಂತಿರುಗಿ p->ಸಂಕೇತ_Str_2;
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::hash_t ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::hash() ಸ್ಥಿರಾಂಕ
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::from_ದ್ವಿಮಾನ(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name,
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* binary, ಸ್ಥಿರಾಂಕ size_t size,
        ಸ್ಥಿರಾಂಕ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ಹಿಂತಿರುಗಿ Impl::from_ದ್ವಿಮಾನ(module, name, binary, size, buildOptions);
}

ಕಾರ್ಯಕ್ರಮ_ಮೂಲ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::fromSPIR(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name,
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* binary, ಸ್ಥಿರಾಂಕ size_t size,
        ಸ್ಥಿರಾಂಕ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ಹಿಂತಿರುಗಿ Impl::from_ದ್ವಿಮಾನ(module, name, binary, size, buildOptions);
}


internal::ಕಾರ್ಯಕ್ರಮ_ನಮೂದು::ಕ್ರಿಯಾಕಾರಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ&() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (ಇದು->p_ಕಾರ್ಯಕ್ರಮ_ಮೂಲ == ಖಾಲಿ)
    {
        cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
        ಆದರೆ (ಇದು->p_ಕಾರ್ಯಕ್ರಮ_ಮೂಲ == ಖಾಲಿ)
        {
            ಕಾರ್ಯಕ್ರಮ_ಮೂಲ ps = ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl::from_ಮೂಲ_With_Static_Lifetime(ಇದು->module, ಇದು->name, ಇದು->ಕಾರ್ಯಕ್ರಮ_ಸಂಕೇತ, ಇದು->ಕಾರ್ಯಕ್ರಮ_Hash, cv::String());
            ಕಾರ್ಯಕ್ರಮ_ಮೂಲ* ptr = ಹೊಸ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ps);
            const_cast<ಕಾರ್ಯಕ್ರಮ_ನಮೂದು*>(ಇದು)->p_ಕಾರ್ಯಕ್ರಮ_ಮೂಲ = ptr;
        }
    }
    ಹಿಂತಿರುಗಿ *ಇದು->p_ಕಾರ್ಯಕ್ರಮ_ಮೂಲ;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

ಸ್ಥಿರ
cv::String joinBuildOptions(ಸ್ಥಿರಾಂಕ cv::String& a, ಸ್ಥಿರಾಂಕ cv::String& b)
{
    ಆದರೆ (b.empty())
        ಹಿಂತಿರುಗಿ a;
    ಆದರೆ (a.empty())
        ಹಿಂತಿರುಗಿ b;
    ಆದರೆ (b[0] == ' ')
        ಹಿಂತಿರುಗಿ a + b;
    ಹಿಂತಿರುಗಿ a + (cv::String(" ") + b);
}

ರಚನೆ ಕಾರ್ಯಕ್ರಮ_2::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src,
         ಸ್ಥಿರಾಂಕ String& _buildflags, String& errmsg) :
         refcount(1),
         handle(ಖಾಲಿ),
         buildflags(_buildflags)
    {
        ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        ಮೂಲ_Module = src_->module_;
        ಮೂಲ_Name = src_->name_;
        ಸ್ಥಿರಾಂಕ Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        ಆದರೆ (ctx.ptr() == ಖಾಲಿ || device.ptr() == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        ಆದರೆ (src.getImpl()->ಬಗೆ_5 == ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ)
        {
            ಆದರೆ (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ಸ್ಥಿರಾಂಕ String param_buildExtraOptions = getBuildExtraOptions();
            ಆದರೆ (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#ಆದರೆ CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(ಖಾಲಿ, "OpenCL program '" << ಮೂಲ_Module << "/" << ಮೂಲ_Name << "' options:" << buildflags);
#ಆದರೆ_ಅಂತ್ಯ
        compile(ctx, src_, errmsg);
#ಆದರೆ CV_OPENCL_SHOW_BUILD_KERNELS
        ಆದರೆ (handle)
        {
            size_t retsz = 0;
            ಅಕ್ಷರ kernels_ಬಫರ್[4096] = {0};
            cl_int ಫಲಿತಾಂಶ = cl_Get_ಕಾರ್ಯಕ್ರಮ_Info(handle, CL_ಕಾರ್ಯಕ್ರಮ_KERNEL_ಹೆಸರುಗಳು, ಗಾತ್ರ(kernels_ಬಫರ್), &kernels_ಬಫರ್[0], &retsz);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಫಲಿತಾಂಶ, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", ಮೂಲ_Module.c_str(), ಮೂಲ_Name.c_str()).c_str());
            ಆದರೆ (ಫಲಿತಾಂಶ == CL_SUCCESS && retsz < ಗಾತ್ರ(kernels_ಬಫರ್))
            {
                kernels_ಬಫರ್[retsz] = 0;
                CV_LOG_INFO(ಖಾಲಿ, "OpenCL program '" << ಮೂಲ_Module << "/" << ಮೂಲ_Name << "' kernels: '" << kernels_ಬಫರ್ << "'");
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_LOG_ERROR(ಖಾಲಿ, "OpenCL program '" << ಮೂಲ_Module << "/" << ಮೂಲ_Name << "' can't retrieve kernel names!");
            }
        }
#ಆದರೆ_ಅಂತ್ಯ
    }

    ತಾರ್ಕಿಕ compile(ಸ್ಥಿರಾಂಕ Context& ctx, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        ಆದರೆ (src_->ಬಗೆ_5 == ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl::ಕಾರ್ಯಕ್ರಮ_BINARIES)
        {
            CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            ತಾರ್ಕಿಕ isLoaded = create_From_ದ್ವಿಮಾನ(ctx, src_->ಮೂಲ_Addr, src_->ಮೂಲ_ಗಾತ್ರ, errmsg);
            ಹಿಂತಿರುಗಿ isLoaded;
        }
        ಹಿಂತಿರುಗಿ compileWithCache(ctx, src_, errmsg);
    }

    ತಾರ್ಕಿಕ compileWithCache(ಸ್ಥಿರಾಂಕ Context& ctx, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->ಬಗೆ_5 != ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl::ಕಾರ್ಯಕ್ರಮ_BINARIES);

#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ಸ್ಥಿರಾಂಕ std::string ತಳ_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_ಪೂರ್ವಪ್ರತ್ಯಯ_String(),
                ctx.getImpl()->get_ಪೂರ್ವಪ್ರತ್ಯಯ_ತಳ()
        );
        ಸ್ಥಿರಾಂಕ String& hash_str = src_->ಮೂಲ_Hash;
        cv::String fname;
        ಆದರೆ (!ತಳ_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(ತಳ_dir, fname);
        }
        ಸ್ಥಿರಾಂಕ cv::Ptr<utils::fs::File_ಬೀಗ> file_ಬೀಗ = config.cache_ಬೀಗ; // can be empty
        ಆದರೆ (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            ಪ್ರಯತ್ನಿಸು
            {
                std::vector<ಅಕ್ಷರ> ದ್ವಿಮಾನ_Buf;
                ತಾರ್ಕಿಕ res = ಸುಳ್ಳು;
                {
                    cv::utils::optional_shared_ಬೀಗ_guard<cv::utils::fs::File_ಬೀಗ> ಬೀಗ_fs(file_ಬೀಗ.get());
                    ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ_File file(fname, hash_str.c_str());
                    res = file.ಪಠ(buildflags, ದ್ವಿಮಾನ_Buf);
                }
                ಆದರೆ (res)
                {
                    CV_Assert(!ದ್ವಿಮಾನ_Buf.empty());
                    CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    ತಾರ್ಕಿಕ isLoaded = create_From_ದ್ವಿಮಾನ(ctx, ದ್ವಿಮಾನ_Buf, errmsg);
                    ಆದರೆ (isLoaded)
                        ಹಿಂತಿರುಗಿ ನಿಜ;
                }
            }
            ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
            {
                CV_ಬಳಸದ(e);
                CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Can't load OpenCL binary: " + fname << std::ಸಾಲು_ಅಂತ್ಯ << e.what());
            }
            ಹಿಡಿ (...)
            {
                CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == ಖಾಲಿ);
        ಆದರೆ (src_->ಬಗೆ_5 == ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ)
        {
            ಆದರೆ (!buildFromSources(ctx, src_, errmsg))
            {
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (src_->ಬಗೆ_5 == ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl::ಕಾರ್ಯಕ್ರಮ_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            ಆದರೆ ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            ತಾರ್ಕಿಕ isLoaded = create_From_ದ್ವಿಮಾನ(ctx, src_->ಮೂಲ_Addr, src_->ಮೂಲ_ಗಾತ್ರ, errmsg);
            ಆದರೆ (!isLoaded)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (src_->ಬಗೆ_5 == ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl::ಕಾರ್ಯಕ್ರಮ_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_Error(Error::Sts_ಆಂತರಿಕ, "Internal error");
        }
        CV_Assert(handle != ಖಾಲಿ);
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
        ಆದರೆ (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            ಪ್ರಯತ್ನಿಸು
            {
                std::vector<ಅಕ್ಷರ> ದ್ವಿಮಾನ_Buf;
                get_ಕಾರ್ಯಕ್ರಮ_ದ್ವಿಮಾನ(ದ್ವಿಮಾನ_Buf);
                {
                    cv::utils::optional_ಬೀಗ_guard<cv::utils::fs::File_ಬೀಗ> ಬೀಗ_fs(file_ಬೀಗ.get());
                    ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ_File file(fname, hash_str.c_str());
                    file.ಲಿಖ(buildflags, ದ್ವಿಮಾನ_Buf);
                }
            }
            ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
            {
                CV_LOG_WARNING(ಖಾಲಿ, "Can't save OpenCL binary into cache: " + fname << std::ಸಾಲು_ಅಂತ್ಯ << e.what());
            }
            ಹಿಡಿ (...)
            {
                CV_LOG_WARNING(ಖಾಲಿ, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT
#ಆದರೆ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS
        ಆದರೆ (CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS_ಮೌಲ್ಯ)
        {
            std::vector<ಅಕ್ಷರ> ದ್ವಿಮಾನ_Buf;
            get_ಕಾರ್ಯಕ್ರಮ_ದ್ವಿಮಾನ(ದ್ವಿಮಾನ_Buf);
            ಆದರೆ (!ದ್ವಿಮಾನ_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_ಕಾರ್ಯಕ್ರಮ(handle));
                handle = ಖಾಲಿ;
                create_From_ದ್ವಿಮಾನ(ctx, ದ್ವಿಮಾನ_Buf, errmsg);
            }
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಹಿಂತಿರುಗಿ handle != ಖಾಲಿ;
    }

    ಶೂನ್ಯ dumpBuildLog_(cl_int ಫಲಿತಾಂಶ, ಸ್ಥಿರಾಂಕ cl_device_id* deviceList, String& errmsg)
    {
        Auto_ಬಫರ್<ಅಕ್ಷರ, 4096> ಬಫರ್; ಬಫರ್[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_ಕಾರ್ಯಕ್ರಮ_Build_Info(handle, deviceList[0],
                                                  CL_ಕಾರ್ಯಕ್ರಮ_BUILD_LOG, 0, 0, &retsz);
        ಆದರೆ (log_retval == CL_SUCCESS && retsz > 1)
        {
            ಬಫರ್.resize(retsz + 16);
            log_retval = cl_Get_ಕಾರ್ಯಕ್ರಮ_Build_Info(handle, deviceList[0],
                                               CL_ಕಾರ್ಯಕ್ರಮ_BUILD_LOG, retsz+1, ಬಫರ್.ದತ್ತಾಂಶ(), &retsz);
            ಆದರೆ (log_retval == CL_SUCCESS)
            {
                ಆದರೆ (retsz < ಬಫರ್.size())
                    ಬಫರ್[retsz] = 0;
                ಇಲ್ಲದಿದ್ದರೆ
                    ಬಫರ್[ಬಫರ್.size() - 1] = 0;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಬಫರ್[0] = 0;
            }
        }

        errmsg = String(ಬಫರ್.ದತ್ತಾಂಶ());
        ಬರೆ("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                ಮೂಲ_Module.c_str(), ಮೂಲ_Name.c_str(),
                ಫಲಿತಾಂಶ, getOpenCLErrorString(ಫಲಿತಾಂಶ),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    ತಾರ್ಕಿಕ buildFromSources(ಸ್ಥಿರಾಂಕ Context& ctx, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->ಬಗೆ_5 == ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ_ಸಂಕೇತ);
        CV_Assert(handle == ಖಾಲಿ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                ಮೂಲ_Module.c_str(), ಮೂಲ_Name.c_str(),
                src_->ಮೂಲ_Hash.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Compile... " << ಮೂಲ_Module.c_str() << "/" << ಮೂಲ_Name.c_str());

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* srcptr = src_->ಮೂಲ_Addr ? ((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)src_->ಮೂಲ_Addr) : src_->ಸಂಕೇತ_Str_2.c_str();
        size_t srclen = src_->ಮೂಲ_Addr ? src_->ಮೂಲ_ಗಾತ್ರ : src_->ಸಂಕೇತ_Str_2.size();
        CV_Assert(srcptr != ಖಾಲಿ);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_ಕಾರ್ಯಕ್ರಮ_With_ಮೂಲ((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        ಆದರೆ (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_ಬಫರ್<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.ದತ್ತಾಂಶ();
            ಗಾಗಿ (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = cl_Build_ಕಾರ್ಯಕ್ರಮ(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_ಫಲಿತಾಂಶ(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#ಆದರೆ !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            ಆದರೆ (retval != CL_SUCCESS)
#ಆದರೆ_ಅಂತ್ಯ
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                ಆದರೆ (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_ಕಾರ್ಯಕ್ರಮ(handle));
                    handle = ಖಾಲಿ;
                }
                ಆದರೆ (retval != CL_SUCCESS &&
                    ಮೂಲ_Name != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#ಆದರೆ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS
            ಆದರೆ (handle && CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS_ಮೌಲ್ಯ)
            {
                CV_LOG_INFO(ಖಾಲಿ, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                ಅಕ್ಷರ kernels_ಬಫರ್[4096] = {0};
                cl_int ಫಲಿತಾಂಶ = cl_Get_ಕಾರ್ಯಕ್ರಮ_Info(handle, CL_ಕಾರ್ಯಕ್ರಮ_KERNEL_ಹೆಸರುಗಳು, ಗಾತ್ರ(kernels_ಬಫರ್), &kernels_ಬಫರ್[0], &retsz);
                ಆದರೆ (retsz < ಗಾತ್ರ(kernels_ಬಫರ್))
                    kernels_ಬಫರ್[retsz] = 0;
                ಇಲ್ಲದಿದ್ದರೆ
                    kernels_ಬಫರ್[0] = 0;
                CV_LOG_INFO(ಖಾಲಿ, ಫಲಿತಾಂಶ << ": Kernels='" << kernels_ಬಫರ್ << "'");
            }
#ಆದರೆ_ಅಂತ್ಯ
        }
        ಹಿಂತಿರುಗಿ handle != ಖಾಲಿ;
    }

    ಶೂನ್ಯ get_ಕಾರ್ಯಕ್ರಮ_ದ್ವಿಮಾನ(std::vector<ಅಕ್ಷರ>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_ಕಾರ್ಯಕ್ರಮ_Info(handle, CL_ಕಾರ್ಯಕ್ರಮ_ದ್ವಿಮಾನ_SIZES, ಗಾತ್ರ(sz), &sz, ಖಾಲಿ));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_ಕಾರ್ಯಕ್ರಮ_Info(handle, CL_ಕಾರ್ಯಕ್ರಮ_BINARIES, ಗಾತ್ರ(ptr), &ptr, ಖಾಲಿ));
    }

    ತಾರ್ಕಿಕ create_From_ದ್ವಿಮಾನ(ಸ್ಥಿರಾಂಕ Context& ctx, ಸ್ಥಿರಾಂಕ std::vector<ಅಕ್ಷರ>& buf, String& errmsg)
    {
        ಹಿಂತಿರುಗಿ create_From_ದ್ವಿಮಾನ(ctx, (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*)&buf[0], buf.size(), errmsg);
    }

    ತಾರ್ಕಿಕ create_From_ದ್ವಿಮಾನ(ಸ್ಥಿರಾಂಕ Context& ctx, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ದ್ವಿಮಾನ_Addr, ಸ್ಥಿರಾಂಕ size_t ದ್ವಿಮಾನ_ಗಾತ್ರ, String& errmsg)
    {
        CV_Assert(handle == ಖಾಲಿ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(ಖಾಲಿ, 0, "Load from binary... (" << ದ್ವಿಮಾನ_ಗಾತ್ರ << " bytes)");

        CV_Assert(ದ್ವಿಮಾನ_ಗಾತ್ರ > 0);

        size_t ndevices = (ಪೂರ್ಣಾಂಕ)ctx.ndevices();
        Auto_ಬಫರ್<cl_device_id> devices_(ndevices);
        Auto_ಬಫರ್<ಸ್ಥಿರಾಂಕ uchar*> ದ್ವಿಮಾನ_Ptrs(ndevices);
        Auto_ಬಫರ್<size_t> ದ್ವಿಮಾನ_Sizes(ndevices);

        cl_device_id* devices = devices_.ದತ್ತಾಂಶ();
        ಸ್ಥಿರಾಂಕ uchar** ದ್ವಿಮಾನ_Ptrs_2 = ದ್ವಿಮಾನ_Ptrs.ದತ್ತಾಂಶ();
        size_t* ದ್ವಿಮಾನ_Sizes_2 = ದ್ವಿಮಾನ_Sizes.ದತ್ತಾಂಶ();
        ಗಾಗಿ (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ದ್ವಿಮಾನ_Ptrs_2[i] = ದ್ವಿಮಾನ_Addr;
            ದ್ವಿಮಾನ_Sizes_2[i] = ದ್ವಿಮಾನ_ಗಾತ್ರ;
        }

        cl_int ಫಲಿತಾಂಶ = 0;
        handle = cl_Create_ಕಾರ್ಯಕ್ರಮ_With_ದ್ವಿಮಾನ((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.ದತ್ತಾಂಶ(),
                                           ದ್ವಿಮಾನ_Sizes_2, ದ್ವಿಮಾನ_Ptrs_2, ಖಾಲಿ, &ಫಲಿತಾಂಶ);
        ಆದರೆ (ಫಲಿತಾಂಶ != CL_SUCCESS)
        {
            CV_LOG_ERROR(ಖಾಲಿ, CV_OCL_API_ERROR_ಸಂದೇಶ(ಫಲಿತಾಂಶ, "clCreateProgramWithBinary"));
            ಆದರೆ (handle)
            {
                CV_OCL_DBG_CHECK(cl_Release_ಕಾರ್ಯಕ್ರಮ(handle));
                handle = ಖಾಲಿ;
            }
        }
        ಆದರೆ (!handle)
        {
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        // call clBuildProgram()
        {
            ಫಲಿತಾಂಶ = cl_Build_ಕಾರ್ಯಕ್ರಮ(handle, (cl_uint)ndevices, devices_.ದತ್ತಾಂಶ(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಫಲಿತಾಂಶ, cv::format("clBuildProgram(binary: %s/%s)", ಮೂಲ_Module.c_str(), ಮೂಲ_Name.c_str()).c_str());
            ಆದರೆ (ಫಲಿತಾಂಶ != CL_SUCCESS)
            {
                dumpBuildLog_(ಫಲಿತಾಂಶ, devices, errmsg);
                ಆದರೆ (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_ಕಾರ್ಯಕ್ರಮ(handle));
                    handle = ಖಾಲಿ;
                }
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        }
        // check build status
        {
            cl_build_ಸ್ಥಿತಿ build_ಸ್ಥಿತಿ = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(ಫಲಿತಾಂಶ = cl_Get_ಕಾರ್ಯಕ್ರಮ_Build_Info(handle, devices[0], CL_ಕಾರ್ಯಕ್ರಮ_BUILD_ಸ್ಥಿತಿ,
                    ಗಾತ್ರ(build_ಸ್ಥಿತಿ), &build_ಸ್ಥಿತಿ, &retsz));
            ಆದರೆ (ಫಲಿತಾಂಶ == CL_SUCCESS)
            {
                ಆದರೆ (build_ಸ್ಥಿತಿ == CL_BUILD_SUCCESS)
                {
                    ಹಿಂತಿರುಗಿ ನಿಜ;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    CV_LOG_WARNING(ಖಾಲಿ, "clGetProgramBuildInfo() returns " << build_ಸ್ಥಿತಿ);
                    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_LOG_ERROR(ಖಾಲಿ, CV_OCL_API_ERROR_ಸಂದೇಶ(ಫಲಿತಾಂಶ, "clGetProgramBuildInfo()"));
                ಆದರೆ (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_ಕಾರ್ಯಕ್ರಮ(handle));
                    handle = ಖಾಲಿ;
                }
            }
        }
#ಆದರೆ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS
        ಆದರೆ (handle && CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS_ಮೌಲ್ಯ)
        {
            CV_LOG_INFO(ಖಾಲಿ, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            ಅಕ್ಷರ kernels_ಬಫರ್[4096] = {0};
            ಫಲಿತಾಂಶ = cl_Get_ಕಾರ್ಯಕ್ರಮ_Info(handle, CL_ಕಾರ್ಯಕ್ರಮ_KERNEL_ಹೆಸರುಗಳು, ಗಾತ್ರ(kernels_ಬಫರ್), &kernels_ಬಫರ್[0], &retsz);
            ಆದರೆ (retsz < ಗಾತ್ರ(kernels_ಬಫರ್))
                kernels_ಬಫರ್[retsz] = 0;
            ಇಲ್ಲದಿದ್ದರೆ
                kernels_ಬಫರ್[0] = 0;
            CV_LOG_INFO(ಖಾಲಿ, ಫಲಿತಾಂಶ << ": Kernels='" << kernels_ಬಫರ್ << "'");
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಹಿಂತಿರುಗಿ handle != ಖಾಲಿ;
    }

    ~Impl()
    {
        ಆದರೆ( handle )
        {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
            ಆದರೆ (!cv::__termination)
#ಆದರೆ_ಅಂತ್ಯ
            {
                cl_Release_ಕಾರ್ಯಕ್ರಮ(handle);
            }
            handle = ಖಾಲಿ;
        }
    }

    cl_ಕಾರ್ಯಕ್ರಮ handle;

    String buildflags;
    String ಮೂಲ_Module;
    String ಮೂಲ_Name;
};


ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2() CV_NOEXCEPT
{
    p = 0;
}

ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src,
        ಸ್ಥಿರಾಂಕ String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog)
{
    p = prog.p;
    ಆದರೆ(p)
        p->addref();
}

ಕಾರ್ಯಕ್ರಮ_2& ಕಾರ್ಯಕ್ರಮ_2::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog)
{
    Impl* newp = (Impl*)prog.p;
    ಆದರೆ(newp)
        newp->addref();
    ಆದರೆ(p)
        p->release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2(ಕಾರ್ಯಕ್ರಮ_2&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = ಶೂನ್ಯಸೂಚಕ;
}

ಕಾರ್ಯಕ್ರಮ_2& ಕಾರ್ಯಕ್ರಮ_2::ಕ್ರಿಯಾಕಾರಕ = (ಕಾರ್ಯಕ್ರಮ_2&& prog) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &prog) {
        ಆದರೆ(p)
            p->release();
        p = prog.p;
        prog.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಕಾರ್ಯಕ್ರಮ_2::~ಕಾರ್ಯಕ್ರಮ_2()
{
    ಆದರೆ(p)
        p->release();
}

ತಾರ್ಕಿಕ ಕಾರ್ಯಕ್ರಮ_2::create(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src,
            ಸ್ಥಿರಾಂಕ String& buildflags, String& errmsg)
{
    ಆದರೆ(p)
    {
        p->release();
        p = ಖಾಲಿ;
    }
    p = ಹೊಸ Impl(src, buildflags, errmsg);
    ಆದರೆ(!p->handle)
    {
        p->release();
        p = 0;
    }
    ಹಿಂತಿರುಗಿ p != 0;
}

ಶೂನ್ಯ* ಕಾರ್ಯಕ್ರಮ_2::ptr() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->handle : 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_REMOVE_DEPRECATED_API
ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& ಕಾರ್ಯಕ್ರಮ_2::ಮೂಲ() ಸ್ಥಿರಾಂಕ
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

ತಾರ್ಕಿಕ ಕಾರ್ಯಕ್ರಮ_2::ಪಠ(ಸ್ಥಿರಾಂಕ String& bin, ಸ್ಥಿರಾಂಕ String& buildflags)
{
    CV_ಬಳಸದ(bin); CV_ಬಳಸದ(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

ತಾರ್ಕಿಕ ಕಾರ್ಯಕ್ರಮ_2::ಲಿಖ(String& bin) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String ಕಾರ್ಯಕ್ರಮ_2::get_ಪೂರ್ವಪ್ರತ್ಯಯ() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!p)
        ಹಿಂತಿರುಗಿ String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    ಹಿಂತಿರುಗಿ cv::format("opencl=%s\nbuildflags=%s", ctx_->get_ಪೂರ್ವಪ್ರತ್ಯಯ_String().c_str(), p->buildflags.c_str());
}

String ಕಾರ್ಯಕ್ರಮ_2::get_ಪೂರ್ವಪ್ರತ್ಯಯ(ಸ್ಥಿರಾಂಕ String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        ಹಿಂತಿರುಗಿ cv::format("opencl=%s\nbuildflags=%s", ctx_->get_ಪೂರ್ವಪ್ರತ್ಯಯ_String().c_str(), buildflags.c_str());
}
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_REMOVE_DEPRECATED_API

ಶೂನ್ಯ ಕಾರ್ಯಕ್ರಮ_2::get_ದ್ವಿಮಾನ(std::vector<ಅಕ್ಷರ>& binary) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p && "Empty program");
    p->get_ಕಾರ್ಯಕ್ರಮ_ದ್ವಿಮಾನ(binary);
}

ಕಾರ್ಯಕ್ರಮ_2 Context::Impl::getProg(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src,
                               ಸ್ಥಿರಾಂಕ String& buildflags, String& errmsg)
{
    size_t ಎಲ್ಲೆ = get_ಕಾರ್ಯಕ್ರಮ_ಎಣಿಕೆ_ಎಲ್ಲೆ();
    ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String ಕೀಲಿ = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->ಮೂಲ_Hash.c_str(),
            get_ಪೂರ್ವಪ್ರತ್ಯಯ_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_ಬೀಗ lock(ಕಾರ್ಯಕ್ರಮ_cache_mutex);
        phash_t::iterator it = phash.find(ಕೀಲಿ);
        ಆದರೆ (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), ಕೀಲಿ);
            ಆದರೆ (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(ಕೀಲಿ);
            }
            ಹಿಂತಿರುಗಿ it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            ಆದರೆ (ಎಲ್ಲೆ > 0 && sz >= ಎಲ್ಲೆ)
            {
                ಸ್ಥಿರ ತಾರ್ಕಿಕ warningFlag = ಸುಳ್ಳು;
                ಆದರೆ (!warningFlag)
                {
                    ಬರೆ("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = ನಿಜ;
                }
                ಆಗ (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    ಆದರೆ (c != 0)
                        ನಿಲ್ಲಿಸು;
                }
            }
        }
    }
    ಕಾರ್ಯಕ್ರಮ_2 prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::Auto_ಬೀಗ lock(ಕಾರ್ಯಕ್ರಮ_cache_mutex);
        phash.insert(std::pair<std::string, ಕಾರ್ಯಕ್ರಮ_2>(ಕೀಲಿ, prog));
        cacheList.push_front(ಕೀಲಿ);
    }
    ಹಿಂತಿರುಗಿ prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ವರ್ಗ OpenCLBufferPool
{
ಸಂರಕ್ಷಿತ:
    ~OpenCLBufferPool() { }
ಸಾರ್ವಜನಿಕ:
    ವಾಸ್ತವಿಕ T allocate(size_t size) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ release(T ಬಫರ್) = 0;
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು Derived, ಬಗೆ_ಹೆಸರು ಬಫರ್_ನಮೂದು, ಬಗೆ_ಹೆಸರು T>
ವರ್ಗ Open_CLBuffer_Pool_ತಳ_Impl : ಸಾರ್ವಜನಿಕ ಬಫರ್_Pool_Controller, ಸಾರ್ವಜನಿಕ OpenCLBufferPool<T>
{
ಖಾಸಗಿ:
    ಸಾಲೊಳಗೆ Derived& derived() { ಹಿಂತಿರುಗಿ *static_cast<Derived*>(ಇದು); }
ಸಂರಕ್ಷಿತ:
    Mutex mutex_;

    size_t current_Reserved_ಗಾತ್ರ;
    size_t max_Reserved_ಗಾತ್ರ;

    std::list<ಬಫರ್_ನಮೂದು> allocatedEntries_; // Allocated and used entries
    std::list<ಬಫರ್_ನಮೂದು> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    ತಾರ್ಕಿಕ ಕಂಡುಹಿಡಿ_And_Remove_ನಮೂದು_From_Allocated_List(CV_ಹೊರಹೋಗು ಬಫರ್_ನಮೂದು& ನಮೂದು, T ಬಫರ್)
    {
        ಬಗೆ_ಹೆಸರು std::list<ಬಫರ್_ನಮೂದು>::iterator i = allocatedEntries_.begin();
        ಗಾಗಿ (; i != allocatedEntries_.end(); ++i)
        {
            ಬಫರ್_ನಮೂದು& e = *i;
            ಆದರೆ (e.cl_ಬಫರ್_2 == ಬಫರ್)
            {
                ನಮೂದು = e;
                allocatedEntries_.erase(i);
                ಹಿಂತಿರುಗಿ ನಿಜ;
            }
        }
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    // synchronized
    ತಾರ್ಕಿಕ ಕಂಡುಹಿಡಿ_And_Remove_ನಮೂದು_From_Reserved_List(CV_ಹೊರಹೋಗು ಬಫರ್_ನಮೂದು& ನಮೂದು, ಸ್ಥಿರಾಂಕ size_t size)
    {
        ಆದರೆ (reservedEntries_.empty())
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಬಗೆ_ಹೆಸರು std::list<ಬಫರ್_ನಮೂದು>::iterator i = reservedEntries_.begin();
        ಬಗೆ_ಹೆಸರು std::list<ಬಫರ್_ನಮೂದು>::iterator ಫಲಿತಾಂಶ_pos = reservedEntries_.end();
        ಬಫರ್_ನಮೂದು ಫಲಿತಾಂಶ;
        size_t minDiff = (size_t)(-1);
        ಗಾಗಿ (; i != reservedEntries_.end(); ++i)
        {
            ಬಫರ್_ನಮೂದು& e = *i;
            ಆದರೆ (e.ಸಾಮರ್ಥ್ಯ >= size)
            {
                size_t diff = e.ಸಾಮರ್ಥ್ಯ - size;
                ಆದರೆ (diff < std::max((size_t)4096, size / 8) && (ಫಲಿತಾಂಶ_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    ಫಲಿತಾಂಶ_pos = i;
                    ಫಲಿತಾಂಶ = e;
                    ಆದರೆ (diff == 0)
                        ನಿಲ್ಲಿಸು;
                }
            }
        }
        ಆದರೆ (ಫಲಿತಾಂಶ_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(ಫಲಿತಾಂಶ_pos);
            ನಮೂದು = ಫಲಿತಾಂಶ;
            current_Reserved_ಗಾತ್ರ -= ನಮೂದು.ಸಾಮರ್ಥ್ಯ;
            allocatedEntries_.push_back(ನಮೂದು);
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    // synchronized
    ಶೂನ್ಯ check_ಗಾತ್ರ_ದ_Reserved_Entries()
    {
        ಆಗ (current_Reserved_ಗಾತ್ರ > max_Reserved_ಗಾತ್ರ)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ಸ್ಥಿರಾಂಕ ಬಫರ್_ನಮೂದು& ನಮೂದು = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_ಗಾತ್ರ >= ನಮೂದು.ಸಾಮರ್ಥ್ಯ);
            current_Reserved_ಗಾತ್ರ -= ನಮೂದು.ಸಾಮರ್ಥ್ಯ;
            derived().release_ಬಫರ್_ನಮೂದು(ನಮೂದು);
            reservedEntries_.pop_back();
        }
    }

    ಸಾಲೊಳಗೆ size_t ಹಂಚಿಕೆ_Granularity(size_t size)
    {
        // heuristic values
        ಆದರೆ (size < 1024*1024)
            ಹಿಂತಿರುಗಿ 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (size < 16*1024*1024)
            ಹಿಂತಿರುಗಿ 64*1024;
        ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ 1024*1024;
    }

ಸಾರ್ವಜನಿಕ:
    Open_CLBuffer_Pool_ತಳ_Impl()
        : current_Reserved_ಗಾತ್ರ(0),
          max_Reserved_ಗಾತ್ರ(0)
    {
        // nothing
    }
    ವಾಸ್ತವಿಕ ~Open_CLBuffer_Pool_ತಳ_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
ಸಾರ್ವಜನಿಕ:
    ವಾಸ್ತವಿಕ T allocate(size_t size) CV_OVERRIDE
    {
        Auto_ಬೀಗ locker(mutex_);
        ಬಫರ್_ನಮೂದು ನಮೂದು;
        ಆದರೆ (max_Reserved_ಗಾತ್ರ > 0 && ಕಂಡುಹಿಡಿ_And_Remove_ನಮೂದು_From_Reserved_List(ನಮೂದು, size))
        {
            CV_DbgAssert(size <= ನಮೂದು.ಸಾಮರ್ಥ್ಯ);
            LOG_ಬಫರ್_POOL("Reuse reserved buffer: %p\n", ನಮೂದು.cl_ಬಫರ್_2);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            derived().allocate_ಬಫರ್_ನಮೂದು(ನಮೂದು, size);
        }
        ಹಿಂತಿರುಗಿ ನಮೂದು.cl_ಬಫರ್_2;
    }
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ release(T ಬಫರ್) CV_OVERRIDE
    {
        Auto_ಬೀಗ locker(mutex_);
        ಬಫರ್_ನಮೂದು ನಮೂದು;
        CV_Assert(ಕಂಡುಹಿಡಿ_And_Remove_ನಮೂದು_From_Allocated_List(ನಮೂದು, ಬಫರ್));
        ಆದರೆ (max_Reserved_ಗಾತ್ರ == 0 || ನಮೂದು.ಸಾಮರ್ಥ್ಯ > max_Reserved_ಗಾತ್ರ / 8)
        {
            derived().release_ಬಫರ್_ನಮೂದು(ನಮೂದು);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            reservedEntries_.push_front(ನಮೂದು);
            current_Reserved_ಗಾತ್ರ += ನಮೂದು.ಸಾಮರ್ಥ್ಯ;
            check_ಗಾತ್ರ_ದ_Reserved_Entries();
        }
    }

    ವಾಸ್ತವಿಕ size_t get_Reserved_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ current_Reserved_ಗಾತ್ರ; }
    ವಾಸ್ತವಿಕ size_t get_Max_Reserved_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ max_Reserved_ಗಾತ್ರ; }
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಗಣ_Max_Reserved_ಗಾತ್ರ(size_t size) CV_OVERRIDE
    {
        Auto_ಬೀಗ locker(mutex_);
        size_t old_Max_Reserved_ಗಾತ್ರ = max_Reserved_ಗಾತ್ರ;
        max_Reserved_ಗಾತ್ರ = size;
        ಆದರೆ (max_Reserved_ಗಾತ್ರ < old_Max_Reserved_ಗಾತ್ರ)
        {
            ಬಗೆ_ಹೆಸರು std::list<ಬಫರ್_ನಮೂದು>::iterator i = reservedEntries_.begin();
            ಗಾಗಿ (; i != reservedEntries_.end();)
            {
                ಸ್ಥಿರಾಂಕ ಬಫರ್_ನಮೂದು& ನಮೂದು = *i;
                ಆದರೆ (ನಮೂದು.ಸಾಮರ್ಥ್ಯ > max_Reserved_ಗಾತ್ರ / 8)
                {
                    CV_DbgAssert(current_Reserved_ಗಾತ್ರ >= ನಮೂದು.ಸಾಮರ್ಥ್ಯ);
                    current_Reserved_ಗಾತ್ರ -= ನಮೂದು.ಸಾಮರ್ಥ್ಯ;
                    derived().release_ಬಫರ್_ನಮೂದು(ನಮೂದು);
                    i = reservedEntries_.erase(i);
                    ಮುಂದುವರಿ;
                }
                ++i;
            }
            check_ಗಾತ್ರ_ದ_Reserved_Entries();
        }
    }
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_ಬೀಗ locker(mutex_);
        ಬಗೆ_ಹೆಸರು std::list<ಬಫರ್_ನಮೂದು>::const_iterator i = reservedEntries_.begin();
        ಗಾಗಿ (; i != reservedEntries_.end(); ++i)
        {
            ಸ್ಥಿರಾಂಕ ಬಫರ್_ನಮೂದು& ನಮೂದು = *i;
            derived().release_ಬಫರ್_ನಮೂದು(ನಮೂದು);
        }
        reservedEntries_.clear();
        current_Reserved_ಗಾತ್ರ = 0;
    }
};

ರಚನೆ CLBuffer_ನಮೂದು
{
    cl_mem cl_ಬಫರ್_2;
    size_t ಸಾಮರ್ಥ್ಯ;
    CLBuffer_ನಮೂದು() : cl_ಬಫರ್_2((cl_mem)ಖಾಲಿ), ಸಾಮರ್ಥ್ಯ(0) { }
};

ವರ್ಗ OpenCLBufferPoolImpl CV_FINAL : ಸಾರ್ವಜನಿಕ Open_CLBuffer_Pool_ತಳ_Impl<OpenCLBufferPoolImpl, CLBuffer_ನಮೂದು, cl_mem>
{
ಸಾರ್ವಜನಿಕ:
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ CLBuffer_ನಮೂದು ಬಫರ್_ನಮೂದು;
ಸಂರಕ್ಷಿತ:
    ಪೂರ್ಣಾಂಕ createFlags_;
ಸಾರ್ವಜನಿಕ:
    OpenCLBufferPoolImpl(ಪೂರ್ಣಾಂಕ createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    ಶೂನ್ಯ allocate_ಬಫರ್_ನಮೂದು(ಬಫರ್_ನಮೂದು& ನಮೂದು, size_t size)
    {
        CV_DbgAssert(ನಮೂದು.cl_ಬಫರ್_2 == ಖಾಲಿ);
        ನಮೂದು.ಸಾಮರ್ಥ್ಯ = align_ಗಾತ್ರ(size, (ಪೂರ್ಣಾಂಕ)ಹಂಚಿಕೆ_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        ನಮೂದು.cl_ಬಫರ್_2 = cl_Create_ಬಫರ್((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, ನಮೂದು.ಸಾಮರ್ಥ್ಯ, 0, &retval);
        CV_OCL_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ, (ಶೂನ್ಯ*)ನಮೂದು.cl_ಬಫರ್_2).c_str());
        CV_Assert(ನಮೂದು.cl_ಬಫರ್_2 != ಖಾಲಿ);
        ಆದರೆ(retval == CL_SUCCESS)
        {
            CV_IMPL_ಕೂಡಿಸು(CV_IMPL_OCL);
        }
        LOG_ಬಫರ್_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (ದೀರ್ಘ ದೀರ್ಘ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ, (ದೀರ್ಘ ದೀರ್ಘ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ, ನಮೂದು.cl_ಬಫರ್_2);
        allocatedEntries_.push_back(ನಮೂದು);
    }

    ಶೂನ್ಯ release_ಬಫರ್_ನಮೂದು(ಸ್ಥಿರಾಂಕ ಬಫರ್_ನಮೂದು& ನಮೂದು)
    {
        CV_Assert(ನಮೂದು.ಸಾಮರ್ಥ್ಯ != 0);
        CV_Assert(ನಮೂದು.cl_ಬಫರ್_2 != ಖಾಲಿ);
        LOG_ಬಫರ್_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                ನಮೂದು.cl_ಬಫರ್_2, (ದೀರ್ಘ ದೀರ್ಘ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ, (ದೀರ್ಘ ದೀರ್ಘ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ);
        CV_OCL_DBG_CHECK(clReleaseMemObject(ನಮೂದು.cl_ಬಫರ್_2));
    }
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
ರಚನೆ CLSVMBuffer_ನಮೂದು
{
    ಶೂನ್ಯ* cl_ಬಫರ್_2;
    size_t ಸಾಮರ್ಥ್ಯ;
    CLSVMBuffer_ನಮೂದು() : cl_ಬಫರ್_2(ಖಾಲಿ), ಸಾಮರ್ಥ್ಯ(0) { }
};
ವರ್ಗ OpenCLSVMBufferPoolImpl CV_FINAL : ಸಾರ್ವಜನಿಕ Open_CLBuffer_Pool_ತಳ_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_ನಮೂದು, ಶೂನ್ಯ*>
{
ಸಾರ್ವಜನಿಕ:
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ CLSVMBuffer_ನಮೂದು ಬಫರ್_ನಮೂದು;
ಸಾರ್ವಜನಿಕ:
    OpenCLSVMBufferPoolImpl()
    {
    }

    ಶೂನ್ಯ allocate_ಬಫರ್_ನಮೂದು(ಬಫರ್_ನಮೂದು& ನಮೂದು, size_t size)
    {
        CV_DbgAssert(ನಮೂದು.cl_ಬಫರ್_2 == ಖಾಲಿ);
        ನಮೂದು.ಸಾಮರ್ಥ್ಯ = align_ಗಾತ್ರ(size, (ಪೂರ್ಣಾಂಕ)ಹಂಚಿಕೆ_Granularity(size));

        Context& ctx = Context::getDefault();
        ಸ್ಥಿರಾಂಕ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ತಾರ್ಕಿಕ is_Fine_Grain_ಬಫರ್ = svmCaps.is_Support_Fine_Grain_ಬಫರ್();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_ಬಫರ್ ? CL_MEM_SVM_FINE_GRAIN_ಬಫರ್ : 0);

        ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (ಪೂರ್ಣಾಂಕ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ);
        ಶೂನ್ಯ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, ನಮೂದು.ಸಾಮರ್ಥ್ಯ, 0);
        CV_Assert(buf);

        ನಮೂದು.cl_ಬಫರ್_2 = buf;
        {
            CV_IMPL_ಕೂಡಿಸು(CV_IMPL_OCL);
        }
        LOG_ಬಫರ್_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (ದೀರ್ಘ ದೀರ್ಘ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ, (ದೀರ್ಘ ದೀರ್ಘ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ, ನಮೂದು.cl_ಬಫರ್_2);
        allocatedEntries_.push_back(ನಮೂದು);
    }

    ಶೂನ್ಯ release_ಬಫರ್_ನಮೂದು(ಸ್ಥಿರಾಂಕ ಬಫರ್_ನಮೂದು& ನಮೂದು)
    {
        CV_Assert(ನಮೂದು.ಸಾಮರ್ಥ್ಯ != 0);
        CV_Assert(ನಮೂದು.cl_ಬಫರ್_2 != ಖಾಲಿ);
        LOG_ಬಫರ್_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                ನಮೂದು.cl_ಬಫರ್_2, (ದೀರ್ಘ ದೀರ್ಘ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ, (ದೀರ್ಘ ದೀರ್ಘ)ನಮೂದು.ಸಾಮರ್ಥ್ಯ);
        Context& ctx = Context::getDefault();
        ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  ನಮೂದು.cl_ಬಫರ್_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), ನಮೂದು.cl_ಬಫರ್_2);
    }
};
#ಆದರೆ_ಅಂತ್ಯ



ಮಾದರಿ <ತಾರ್ಕಿಕ readAccess, ತಾರ್ಕಿಕ writeAccess>
ವರ್ಗ Aligned_ದತ್ತಾಂಶ_Ptr
{
ಸಂರಕ್ಷಿತ:
    ಸ್ಥಿರಾಂಕ size_t ಗಾತ್ರ_4;
    uchar* ಸ್ಥಿರಾಂಕ originPtr_;
    ಸ್ಥಿರಾಂಕ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

ಸಾರ್ವಜನಿಕ:
    Aligned_ದತ್ತಾಂಶ_Ptr(uchar* ptr, size_t size, size_t alignment)
        : ಗಾತ್ರ_4(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ಖಾಲಿ)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        ಆದರೆ (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = ಹೊಸ uchar[ಗಾತ್ರ_4 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ಆದರೆ (readAccess)
            {
                ಸ್ಮರಣೆ_ನಕಲು(ptr_, originPtr_, ಗಾತ್ರ_4);
            }
        }
    }

    uchar* getAlignedPtr() ಸ್ಥಿರಾಂಕ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ಹಿಂತಿರುಗಿ ptr_;
    }

    ~Aligned_ದತ್ತಾಂಶ_Ptr()
    {
        ಆದರೆ (allocatedPtr_)
        {
            ಆದರೆ (writeAccess)
            {
                ಸ್ಮರಣೆ_ನಕಲು(originPtr_, ptr_, ಗಾತ್ರ_4);
            }
            ಅಳಿಸು[] allocatedPtr_;
            allocatedPtr_ = ಖಾಲಿ;
        }
        ptr_ = ಖಾಲಿ;
    }
ಖಾಸಗಿ:
    Aligned_ದತ್ತಾಂಶ_Ptr(ಸ್ಥಿರಾಂಕ Aligned_ದತ್ತಾಂಶ_Ptr&); // disabled
    Aligned_ದತ್ತಾಂಶ_Ptr& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Aligned_ದತ್ತಾಂಶ_Ptr&); // disabled
};

ಮಾದರಿ <ತಾರ್ಕಿಕ readAccess, ತಾರ್ಕಿಕ writeAccess>
ವರ್ಗ Aligned_ದತ್ತಾಂಶ_Ptr2_D
{
ಸಂರಕ್ಷಿತ:
    ಸ್ಥಿರಾಂಕ size_t ಗಾತ್ರ_4;
    uchar* ಸ್ಥಿರಾಂಕ originPtr_;
    ಸ್ಥಿರಾಂಕ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t ಅಡ್ಡಸಾಲುಗಳು_2;
    size_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2;
    size_t ಹೆಜ್ಜೆ_2;

ಸಾರ್ವಜನಿಕ:
    Aligned_ದತ್ತಾಂಶ_Ptr2_D(uchar* ptr, size_t ಅಡ್ಡಸಾಲುಗಳು, size_t ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, size_t ಹೆಜ್ಜೆ, size_t alignment, size_t extrabytes=0)
        : ಗಾತ್ರ_4(ಅಡ್ಡಸಾಲುಗಳು*ಹೆಜ್ಜೆ), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ಖಾಲಿ), ಅಡ್ಡಸಾಲುಗಳು_2(ಅಡ್ಡಸಾಲುಗಳು), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ), ಹೆಜ್ಜೆ_2(ಹೆಜ್ಜೆ)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != ಖಾಲಿ);
        ಆದರೆ (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = ಹೊಸ uchar[ಗಾತ್ರ_4 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ಆದರೆ (readAccess)
            {
                ಗಾಗಿ (size_t i = 0; i < ಅಡ್ಡಸಾಲುಗಳು_2; i++)
                    ಸ್ಮರಣೆ_ನಕಲು(ptr_ + i*ಹೆಜ್ಜೆ_2, originPtr_ + i*ಹೆಜ್ಜೆ_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2);
            }
        }
    }

    uchar* getAlignedPtr() ಸ್ಥಿರಾಂಕ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ಹಿಂತಿರುಗಿ ptr_;
    }

    ~Aligned_ದತ್ತಾಂಶ_Ptr2_D()
    {
        ಆದರೆ (allocatedPtr_)
        {
            ಆದರೆ (writeAccess)
            {
                ಗಾಗಿ (size_t i = 0; i < ಅಡ್ಡಸಾಲುಗಳು_2; i++)
                    ಸ್ಮರಣೆ_ನಕಲು(originPtr_ + i*ಹೆಜ್ಜೆ_2, ptr_ + i*ಹೆಜ್ಜೆ_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2);
            }
            ಅಳಿಸು[] allocatedPtr_;
            allocatedPtr_ = ಖಾಲಿ;
        }
        ptr_ = ಖಾಲಿ;
    }
ಖಾಸಗಿ:
    Aligned_ದತ್ತಾಂಶ_Ptr2_D(ಸ್ಥಿರಾಂಕ Aligned_ದತ್ತಾಂಶ_Ptr2_D&); // disabled
    Aligned_ದತ್ತಾಂಶ_Ptr2_D& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Aligned_ದತ್ತಾಂಶ_Ptr2_D&); // disabled
};

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT 16
#ಆದರೆ_ಅಂತ್ಯ


ಶೂನ್ಯ Context::Impl::__init_buffer_pools()
{
    ಬಫರ್_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& ಬಫರ್_Pool_3 = *ಬಫರ್_Pool_2.get();
    ಬಫರ್_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& ಬಫರ್_Pool_Host_Ptr_2 = *ಬಫರ್_Pool_Host_Ptr.get();

    size_t default_Pool_ಗಾತ್ರ = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_ಗಾತ್ರ = utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_ಗಾತ್ರ);
    ಬಫರ್_Pool_3.ಗಣ_Max_Reserved_ಗಾತ್ರ(pool_ಗಾತ್ರ);
    size_t pool_ಗಾತ್ರ_Host_Ptr = utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_ಗಾತ್ರ);
    ಬಫರ್_Pool_Host_Ptr_2.ಗಣ_Max_Reserved_ಗಾತ್ರ(pool_ಗಾತ್ರ_Host_Ptr);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
    ಬಫರ್_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& ಬಫರ್_Pool_SVM_2 = *ಬಫರ್_Pool_SVM.get();
    size_t pool_ಗಾತ್ರ_SVM = utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_ಗಾತ್ರ);
    ಬಫರ್_Pool_SVM_2.ಗಣ_Max_Reserved_ಗಾತ್ರ(pool_ಗಾತ್ರ_SVM);
#ಆದರೆ_ಅಂತ್ಯ

    CV_LOG_INFO(ಖಾಲಿ, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_ಗಾತ್ರ << " poolSizeHostPtr=" << pool_ಗಾತ್ರ_Host_Ptr);
}

ವರ್ಗ OpenCLAllocator CV_FINAL : ಸಾರ್ವಜನಿಕ MatAllocator
{
ಸಾರ್ವಜನಿಕ:
    ಎಣಿಕೆ AllocatorFlags
    {
        ALLOCATOR_FLAGS_ಬಫರ್_POOL_ಬಳಸಿದ = 1 << 0,
        ALLOCATOR_FLAGS_ಬಫರ್_POOL_HOST_PTR_ಬಳಸಿದ = 1 << 1,
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_ಬಫರ್_POOL_SVM_ಬಳಸಿದ = 1 << 2,
#ಆದರೆ_ಅಂತ್ಯ
        ALLOCATOR_FLAGS_EXTERNAL_ಬಫರ್ = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flush_Cleanup_ಸರತಿ();
    }

    UMat_ದತ್ತಾಂಶ* defaultAllocate(ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type, ಶೂನ್ಯ* ದತ್ತಾಂಶ, size_t* ಹೆಜ್ಜೆ,
            AccessFlag flags, UMatUsageFlags usageFlags) ಸ್ಥಿರಾಂಕ
    {
        UMat_ದತ್ತಾಂಶ* u = matStdAllocator->allocate(dims, sizes, type, ದತ್ತಾಂಶ, ಹೆಜ್ಜೆ, flags, usageFlags);
        ಹಿಂತಿರುಗಿ u;
    }

    ಸ್ಥಿರ ತಾರ್ಕಿಕ isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        ಸ್ಥಿರ ತಾರ್ಕಿಕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", ಸುಳ್ಳು);
        ಹಿಂತಿರುಗಿ value;
    }
    ಸ್ಥಿರ ತಾರ್ಕಿಕ isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        ಸ್ಥಿರ ತಾರ್ಕಿಕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", ಸುಳ್ಳು);
        ಹಿಂತಿರುಗಿ value;
    }

    ಶೂನ್ಯ get_ಉತ್ತಮ_Flags(ಸ್ಥಿರಾಂಕ Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, ಪೂರ್ಣಾಂಕ& createFlags, UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag& flags0) ಸ್ಥಿರಾಂಕ
    {
        ಸ್ಥಿರಾಂಕ Device& dev = ctx.device(0);
        createFlags = 0;
        ಆದರೆ ((usageFlags & USAGE_ALLOCATE_HOST_ಸ್ಮರಣೆ) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        ಆದರೆ (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_ಸ್ಮರಣೆ_2()
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __APPLE__
                || dev.isIntel()
#ಆದರೆ_ಅಂತ್ಯ
                )
            )
        )
            flags0 = static_cast<UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag>(0);
        ಇಲ್ಲದಿದ್ದರೆ
            flags0 = UMat_ದತ್ತಾಂಶ::COPY_ON_MAP;
    }

    UMat_ದತ್ತಾಂಶ* allocate(ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type,
                       ಶೂನ್ಯ* ದತ್ತಾಂಶ, size_t* ಹೆಜ್ಜೆ, AccessFlag flags, UMatUsageFlags usageFlags) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!useOpenCL())
            ಹಿಂತಿರುಗಿ defaultAllocate(dims, sizes, type, ದತ್ತಾಂಶ, ಹೆಜ್ಜೆ, flags, usageFlags);

        flush_Cleanup_ಸರತಿ();

        CV_Assert(ದತ್ತಾಂಶ == 0);
        size_t ಒಟ್ಟು = CV_ELEM_ಗಾತ್ರ(type);
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = dims-1; i >= 0; i-- )
        {
            ಆದರೆ( ಹೆಜ್ಜೆ )
                ಹೆಜ್ಜೆ[i] = ಒಟ್ಟು;
            ಒಟ್ಟು *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        ಆದರೆ (!ctx.getImpl())
            ಹಿಂತಿರುಗಿ defaultAllocate(dims, sizes, type, ದತ್ತಾಂಶ, ಹೆಜ್ಜೆ, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        ಪೂರ್ಣಾಂಕ createFlags = 0;
        UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag flags0 = static_cast<UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag>(0);
        get_ಉತ್ತಮ_Flags(ctx, flags, usageFlags, createFlags, flags0);

        ಶೂನ್ಯ* handle = ಖಾಲಿ;
        ಪೂರ್ಣಾಂಕ allocatorFlags = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ಸ್ಥಿರಾಂಕ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ಆದರೆ (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_ಬಫರ್_POOL_SVM_ಬಳಸಿದ;
            handle = ctxImpl.get_ಬಫರ್_Pool_SVM().allocate(ಒಟ್ಟು);

            // this property is constant, so single buffer pool can be used here
            ತಾರ್ಕಿಕ is_Fine_Grain_ಬಫರ್ = svmCaps.is_Support_Fine_Grain_ಬಫರ್();
            allocatorFlags |= is_Fine_Grain_ಬಫರ್ ? svm::OPENCL_SVM_FINE_GRAIN_ಬಫರ್ : svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್;
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_ಬಫರ್_POOL_ಬಳಸಿದ;
            handle = ctxImpl.get_ಬಫರ್_Pool().allocate(ಒಟ್ಟು);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_ಬಫರ್_POOL_HOST_PTR_ಬಳಸಿದ;
            handle = ctxImpl.get_ಬಫರ್_Pool_Host_Ptr().allocate(ಒಟ್ಟು);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_Assert(handle != ಖಾಲಿ); // Unsupported, throw
        }

        ಆದರೆ (!handle)
            ಹಿಂತಿರುಗಿ defaultAllocate(dims, sizes, type, ದತ್ತಾಂಶ, ಹೆಜ್ಜೆ, flags, usageFlags);

        UMat_ದತ್ತಾಂಶ* u = ಹೊಸ UMat_ದತ್ತಾಂಶ(ಇದು);
        u->ದತ್ತಾಂಶ = 0;
        u->size = ಒಟ್ಟು;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<ಶೂನ್ಯ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->ತಾತ್ಕಾಲಿಕ_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(ನಿಜ);
        opencl_allocator_stats.onAllocate(u->size);
        ಹಿಂತಿರುಗಿ u;
    }

    ತಾರ್ಕಿಕ allocate(UMat_ದತ್ತಾಂಶ* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!u)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        flush_Cleanup_ಸರತಿ();

        UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ lock(u);

        ಆದರೆ(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            ಪೂರ್ಣಾಂಕ createFlags = 0;
            UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag flags0 = static_cast<UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag>(0);
            get_ಉತ್ತಮ_Flags(ctx, accessFlags, usageFlags, createFlags, flags0);

            ತಾರ್ಕಿಕ copyOnMap = (flags0 & UMat_ದತ್ತಾಂಶ::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            ಪೂರ್ಣಾಂಕ allocatorFlags = 0;
            UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag ತಾತ್ಕಾಲಿಕ_UMat_Flags = static_cast<UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag>(0);
            ಶೂನ್ಯ* handle = ಖಾಲಿ;
            cl_int retval = CL_SUCCESS;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            ತಾರ್ಕಿಕ useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            ಆದರೆ (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                ತಾತ್ಕಾಲಿಕ_UMat_Flags = UMat_ದತ್ತಾಂಶ::ತಾತ್ಕಾಲಿಕ_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (ಪೂರ್ಣಾಂಕ)u->size, handle);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (useSVM && (svmCaps.is_Support_Fine_Grain_ಬಫರ್() || svmCaps.is_Support_Coarse_Grain_ಬಫರ್()))
            {
                ಆದರೆ (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    ತಾರ್ಕಿಕ is_Fine_Grain_ಬಫರ್ = svmCaps.is_Support_Fine_Grain_ಬಫರ್();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_ಬಫರ್ ? CL_MEM_SVM_FINE_GRAIN_ಬಫರ್ : 0);

                    ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (ಪೂರ್ಣಾಂಕ)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_ಸರತಿ q = ಖಾಲಿ;
                    ಆದರೆ (!is_Fine_Grain_ಬಫರ್)
                    {
                        q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (ಪೂರ್ಣಾಂಕ)u->size);
                        cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, ಖಾಲಿ, ಖಾಲಿ);
                        CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMMap()");

                    }
                    ಸ್ಮರಣೆ_ನಕಲು(handle, u->origdata, u->size);
                    ಆದರೆ (!is_Fine_Grain_ಬಫರ್)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap(q, handle, 0, ಖಾಲಿ, ಖಾಲಿ);
                        CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMUnmap()");
                    }

                    ತಾತ್ಕಾಲಿಕ_UMat_Flags = UMat_ದತ್ತಾಂಶ::ತಾತ್ಕಾಲಿಕ_UMAT | UMat_ದತ್ತಾಂಶ::ತಾತ್ಕಾಲಿಕ_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_ಬಫರ್ ? svm::OPENCL_SVM_FINE_GRAIN_ಬಫರ್
                                                : svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
            {
                ಆದರೆ( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                ತಾತ್ಕಾಲಿಕ_UMat_Flags = UMat_ದತ್ತಾಂಶ::ತಾತ್ಕಾಲಿಕ_UMAT;
                ಆದರೆ (
                #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
                    !copyOnMap &&
                #ಆದರೆ_ಅಂತ್ಯ
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (ಪೂರ್ಣಾಂಕ)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_ದತ್ತಾಂಶ && u->original_UMat_ದತ್ತಾಂಶ->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_ಬಫರ್(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size, u->origdata, (ಶೂನ್ಯ*)handle).c_str());
                }
                ಆದರೆ((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_ಬಫರ್(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size, u->origdata, (ಶೂನ್ಯ*)handle).c_str());
                    ತಾತ್ಕಾಲಿಕ_UMat_Flags |= UMat_ದತ್ತಾಂಶ::ತಾತ್ಕಾಲಿಕ_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clCreateBuffer() => %p", (ಶೂನ್ಯ*)handle).c_str());
            ಆದರೆ(!handle || retval != CL_SUCCESS)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = ಇದು;
            u->flags |= ತಾತ್ಕಾಲಿಕ_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        ಆದರೆ (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(ನಿಜ);
        opencl_allocator_stats.onAllocate(u->size);
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    /*void sync(UMatData* u) const
    {
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        UMatDataAutoLock lock(u);

        if( u->hostCopyObsolete() && u->handle && u->refcount > 0 && u->origdata)
        {
            if( u->tempCopiedUMat() )
            {
                clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                    u->size, u->origdata, 0, 0, 0);
            }
            else
            {
                cl_int retval = 0;
                void* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                (CL_MAP_READ | CL_MAP_WRITE),
                                                0, u->size, 0, 0, 0, &retval);
                clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                clFinish(q);
            }
            u->markHostCopyObsolete(false);
        }
        else if( u->copyOnMap() && u->deviceCopyObsolete() && u->data )
        {
            clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                 u->size, u->data, 0, 0, 0);
        }
    }*/

    ಶೂನ್ಯ deallocate(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!u)
            ಹಿಂತಿರುಗಿ;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        ಆದರೆ (!!(u->flags & UMat_ದತ್ತಾಂಶ::ASYNC_CLEANUP))
            ಕೂಡಿಸು_ಗಮ್ಯ_Cleanup_ಸರತಿ(u);
        ಇಲ್ಲದಿದ್ದರೆ
            deallocate_(u);
    }

    ಶೂನ್ಯ deallocate_(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        ಆದರೆ ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_ಬಫರ್) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
        ಆದರೆ (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            ಹಿಂತಿರುಗಿ;             // avoid any OpenCL calls
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ(u->ತಾತ್ಕಾಲಿಕ_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            ಆದರೆ (u->hostCopyObsolete())
            {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
                ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    ಆದರೆ( u->ತಾತ್ಕಾಲಿಕ_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_ಬಫರ್ ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್);
                        ತಾರ್ಕಿಕ is_Fine_Grain_ಬಫರ್ = (u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_ಬಫರ್;
                        cl_command_ಸರತಿ q = ಖಾಲಿ;
                        ಆದರೆ (!is_Fine_Grain_ಬಫರ್)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_MAP) == 0));
                            q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ಪೂರ್ಣಾಂಕ)u->size);
                            cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, ಖಾಲಿ, ಖಾಲಿ);
                            CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMMap()");
                        }
                        cl_ಮುಕ್ತಾಯ(q);
                        ಸ್ಮರಣೆ_ನಕಲು(u->origdata, u->handle, u->size);
                        ಆದರೆ (!is_Fine_Grain_ಬಫರ್)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap(q, u->handle, 0, ಖಾಲಿ, ಖಾಲಿ);
                            CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMUnmap()");
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
                {
                    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
                    ಆದರೆ( u->ತಾತ್ಕಾಲಿಕ_Copied_UMat() )
                    {
                        Aligned_ದತ್ತಾಂಶ_Ptr<ಸುಳ್ಳು, ನಿಜ> alignedPtr(u->origdata, u->size, CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Read_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        cl_int retval = 0;
                        ಆದರೆ (u->ತಾತ್ಕಾಲಿಕ_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flush_Cleanup_ಸರತಿ(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            ಶೂನ್ಯ* ದತ್ತಾಂಶ = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Map_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ಶೂನ್ಯ*)u->handle, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size, ದತ್ತಾಂಶ).c_str());
                            CV_Assert(u->origdata == ದತ್ತಾಂಶ && "Details: https://github.com/opencv/opencv/issues/6293");
                            ಆದರೆ (u->original_UMat_ದತ್ತಾಂಶ)
                            {
                                CV_Assert(u->original_UMat_ದತ್ತಾಂಶ->ದತ್ತಾಂಶ == ದತ್ತಾಂಶ);
                            }
                            retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Unmap_Mem_Object(q, (cl_mem)u->handle, ದತ್ತಾಂಶ, 0, 0, 0);
                            CV_OCL_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ಶೂನ್ಯ*)u->handle, ದತ್ತಾಂಶ, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size).c_str());
                            CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ(q));
                        }
                    }
                }
                u->markHostCopyObsolete(ಸುಳ್ಳು);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // nothing
            }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
            ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
            {
                ಆದರೆ( u->ತಾತ್ಕಾಲಿಕ_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clReleaseMemObject(ptr=%p)", (ಶೂನ್ಯ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ನಿಜ);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = ಖಾಲಿ;
            ಆದರೆ(u->ದತ್ತಾಂಶ && u->copyOnMap() && u->ದತ್ತಾಂಶ != u->origdata)
                fastFree(u->ದತ್ತಾಂಶ);
            u->ದತ್ತಾಂಶ = u->origdata;
            u->currAllocator->deallocate(u);
            u = ಖಾಲಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_Assert(u->origdata == ಖಾಲಿ);
            ಆದರೆ(u->ದತ್ತಾಂಶ && u->copyOnMap() && u->ದತ್ತಾಂಶ != u->origdata)
            {
                fastFree(u->ದತ್ತಾಂಶ);
                u->ದತ್ತಾಂಶ = 0;
                u->markHostCopyObsolete(ನಿಜ);
            }
            ಆದರೆ (u->allocatorFlags_ & ALLOCATOR_FLAGS_ಬಫರ್_POOL_ಬಳಸಿದ)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_ಬಫರ್_Pool().release((cl_mem)u->handle);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (u->allocatorFlags_ & ALLOCATOR_FLAGS_ಬಫರ್_POOL_HOST_PTR_ಬಳಸಿದ)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_ಬಫರ್_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (u->allocatorFlags_ & ALLOCATOR_FLAGS_ಬಫರ್_POOL_SVM_ಬಳಸಿದ)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_ಬಫರ್ ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್)
                {
                    ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

                    ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap(q, u->handle, 0, ಖಾಲಿ, ಖಾಲಿ);
                        CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_ಬಫರ್_Pool_SVM().release((ಶೂನ್ಯ*)u->handle);
            }
#ಆದರೆ_ಅಂತ್ಯ
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ನಿಜ);
            ಅಳಿಸು u;
            u = ಖಾಲಿ;
        }
        CV_Assert(u == ಖಾಲಿ);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    ಶೂನ್ಯ map(UMat_ದತ್ತಾಂಶ* u, AccessFlag accessFlags) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        ಆದರೆ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ನಿಜ);

        cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

        {
            ಆದರೆ( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
                ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
                {
                    ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್)
                    {
                        Context& ctx = Context::getDefault();
                        ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ಪೂರ್ಣಾಂಕ)u->size);
                            cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, ಖಾಲಿ, ಖಾಲಿ);
                            CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_ಬಫರ್_MAP;
                        }
                    }
                    cl_ಮುಕ್ತಾಯ(q);
                    u->ದತ್ತಾಂಶ = (uchar*)u->handle;
                    u->markHostCopyObsolete(ಸುಳ್ಳು);
                    u->markDeviceMemMapped(ನಿಜ);
                    ಹಿಂತಿರುಗಿ;
                }
#ಆದರೆ_ಅಂತ್ಯ

                cl_int retval = CL_SUCCESS;
                ಆದರೆ (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->ದತ್ತಾಂಶ = (uchar*)cl_ಸರತಿಗೆ_ಸೇರಿಸು_Map_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ಶೂನ್ಯ*)u->handle, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size, u->ದತ್ತಾಂಶ).c_str());
                }
                ಆದರೆ (u->ದತ್ತಾಂಶ && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(ಸುಳ್ಳು);
                    u->markDeviceMemMapped(ನಿಜ);
                    ಹಿಂತಿರುಗಿ;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_ದತ್ತಾಂಶ::COPY_ON_MAP;
            }

            ಆದರೆ(!u->ದತ್ತಾಂಶ)
            {
                u->ದತ್ತಾಂಶ = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(ನಿಜ);
            }
        }

        ಆದರೆ (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_ದತ್ತಾಂಶ_Ptr<ಸುಳ್ಳು, ನಿಜ> alignedPtr(u->ದತ್ತಾಂಶ, u->size, CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == 0);
#ಆದರೆ_ಅಂತ್ಯ
            cl_int retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Read_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ಶೂನ್ಯ*)u->handle, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(ಸುಳ್ಳು);
        }
    }

    ಶೂನ್ಯ unmap(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!u)
            ಹಿಂತಿರುಗಿ;


        CV_Assert(u->handle != 0);

        UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ autolock(u);

        cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
        cl_int retval = 0;
        ಆದರೆ( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->ದತ್ತಾಂಶ != ಖಾಲಿ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
            ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
            {
                ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್)
                {
                    Context& ctx = Context::getDefault();
                    ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap(q, u->handle,
                                0, ಖಾಲಿ, ಖಾಲಿ);
                        CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMUnmap()");
                        cl_ಮುಕ್ತಾಯ(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_ಬಫರ್_MAP;
                    }
                }
                ಆದರೆ (u->refcount == 0)
                    u->ದತ್ತಾಂಶ = 0;
                u->markDeviceCopyObsolete(ಸುಳ್ಳು);
                u->markHostCopyObsolete(ನಿಜ);
                ಹಿಂತಿರುಗಿ;
            }
#ಆದರೆ_ಅಂತ್ಯ
            ಆದರೆ (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Unmap_Mem_Object(q, (cl_mem)u->handle, u->ದತ್ತಾಂಶ, 0, 0, 0);
                CV_OCL_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ಶೂನ್ಯ*)u->handle, u->ದತ್ತಾಂಶ, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size).c_str());
                ಆದರೆ (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ(q));
                }
                u->markDeviceMemMapped(ಸುಳ್ಳು);
                u->ದತ್ತಾಂಶ = 0;
                u->markDeviceCopyObsolete(ಸುಳ್ಳು);
                u->markHostCopyObsolete(ನಿಜ);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_ದತ್ತಾಂಶ_Ptr<ನಿಜ, ಸುಳ್ಳು> alignedPtr(u->ದತ್ತಾಂಶ, u->size, CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == 0);
#ಆದರೆ_ಅಂತ್ಯ
            retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Write_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ಶೂನ್ಯ*)u->handle, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(ಸುಳ್ಳು);
            u->markHostCopyObsolete(ನಿಜ);
        }
    }

    ತಾರ್ಕಿಕ checkContinuous(ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
                         ಸ್ಥಿರಾಂಕ size_t srcofs[], ಸ್ಥಿರಾಂಕ size_t srcstep[],
                         ಸ್ಥಿರಾಂಕ size_t dstofs[], ಸ್ಥಿರಾಂಕ size_t dststep[],
                         size_t& ಒಟ್ಟು, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ಸ್ಥಿರಾಂಕ
    {
        ತಾರ್ಕಿಕ iscontinuous = ನಿಜ;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        ಒಟ್ಟು = sz[dims-1];
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = dims-2; i >= 0; i-- )
        {
            ಆದರೆ( i >= 0 && (ಒಟ್ಟು != srcstep[i] || ಒಟ್ಟು != dststep[i]) )
                iscontinuous = ಸುಳ್ಳು;
            ಒಟ್ಟು *= sz[i];
            ಆದರೆ( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            ಆದರೆ( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        ಆದರೆ( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            ಆದರೆ( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                ಆದರೆ( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                ಆದರೆ( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                ಆದರೆ( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                ಆದರೆ( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        ಹಿಂತಿರುಗಿ iscontinuous;
    }

    ಶೂನ್ಯ download(UMat_ದತ್ತಾಂಶ* u, ಶೂನ್ಯ* dstptr, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
                  ಸ್ಥಿರಾಂಕ size_t srcofs[], ಸ್ಥಿರಾಂಕ size_t srcstep[],
                  ಸ್ಥಿರಾಂಕ size_t dststep[]) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!u)
            ಹಿಂತಿರುಗಿ;
        UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ autolock(u);

        ಆದರೆ( u->ದತ್ತಾಂಶ && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            ಹಿಂತಿರುಗಿ;
        }
        CV_Assert( u->handle != 0 );

        cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

        size_t ಒಟ್ಟು = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ತಾರ್ಕಿಕ iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            ಒಟ್ಟು, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
        {
            CV_DbgAssert(u->ದತ್ತಾಂಶ == ಖಾಲಿ || u->ದತ್ತಾಂಶ == u->handle);
            Context& ctx = Context::getDefault();
            ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_MAP) == 0);
            ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ಪೂರ್ಣಾಂಕ)u->size);
                cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, ಖಾಲಿ, ಖಾಲಿ);
                CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMMap()");
            }
            cl_ಮುಕ್ತಾಯ(q);
            ಆದರೆ( iscontinuous )
            {
                ಸ್ಮರಣೆ_ನಕಲು(dstptr, (uchar*)u->handle + srcrawofs, ಒಟ್ಟು);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // This code is from MatAllocator::download()
                ಪೂರ್ಣಾಂಕ isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                    ಆದರೆ( sz[i] == 0 )
                    ಹಿಂತಿರುಗಿ;
                    ಆದರೆ( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (ಪೂರ್ಣಾಂಕ)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ಸ್ಥಿರಾಂಕ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ಗಾಗಿ( j = 0; j < it.nplanes; j++, ++it )
                    ಸ್ಮರಣೆ_ನಕಲು(ptrs[1], ptrs[0], planesz);
            }
            ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap(q, u->handle,
                        0, ಖಾಲಿ, ಖಾಲಿ);
                CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMUnmap()");
                cl_ಮುಕ್ತಾಯ(q);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        {
            ಆದರೆ( iscontinuous )
            {
                Aligned_ದತ್ತಾಂಶ_Ptr<ಸುಳ್ಳು, ನಿಜ> alignedPtr((uchar*)dstptr, ಒಟ್ಟು, CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Read_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, ಒಟ್ಟು, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (CV_OPENCL_DISABLE_ಬಫರ್_RECT_OPERATIONS)
            {
                ಸ್ಥಿರಾಂಕ size_t padding = CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_ದತ್ತಾಂಶ_Ptr2_D<ಸುಳ್ಳು, ಸುಳ್ಳು> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                ಒಟ್ಟು = align_ಗಾತ್ರ(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                ಒಟ್ಟು = std::min(ಒಟ್ಟು, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Read_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, ಒಟ್ಟು, ptr, 0, 0, 0));
                ಗಾಗಿ( size_t i = 0; i < new_sz[1]; i++ )
                    ಸ್ಮರಣೆ_ನಕಲು( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                Aligned_ದತ್ತಾಂಶ_Ptr2_D<ಸುಳ್ಳು, ನಿಜ> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Read_ಬಫರ್_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    ಶೂನ್ಯ upload(UMat_ದತ್ತಾಂಶ* u, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* srcptr, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
                ಸ್ಥಿರಾಂಕ size_t dstofs[], ಸ್ಥಿರಾಂಕ size_t dststep[],
                ಸ್ಥಿರಾಂಕ size_t srcstep[]) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!u)
            ಹಿಂತಿರುಗಿ;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->ತಾತ್ಕಾಲಿಕ_UMat());

        size_t ಒಟ್ಟು = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ತಾರ್ಕಿಕ iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            ಒಟ್ಟು, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        ಆದರೆ( u->ದತ್ತಾಂಶ && (u->hostCopyObsolete() < u->deviceCopyObsolete() || ಒಟ್ಟು == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(ಸುಳ್ಳು);
            u->markDeviceCopyObsolete(ನಿಜ);
            ಹಿಂತಿರುಗಿ;
        }

        CV_Assert( u->handle != 0 );
        cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
        {
            CV_DbgAssert(u->ದತ್ತಾಂಶ == ಖಾಲಿ || u->ದತ್ತಾಂಶ == u->handle);
            Context& ctx = Context::getDefault();
            ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_MAP) == 0);
            ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ಪೂರ್ಣಾಂಕ)u->size);
                cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, ಖಾಲಿ, ಖಾಲಿ);
                CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMMap()");
            }
            cl_ಮುಕ್ತಾಯ(q);
            ಆದರೆ( iscontinuous )
            {
                ಸ್ಮರಣೆ_ನಕಲು((uchar*)u->handle + dstrawofs, srcptr, ಒಟ್ಟು);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // This code is from MatAllocator::upload()
                ಪೂರ್ಣಾಂಕ isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                    ಆದರೆ( sz[i] == 0 )
                    ಹಿಂತಿರುಗಿ;
                    ಆದರೆ( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (ಪೂರ್ಣಾಂಕ)sz[i];
                }

                Mat src(dims, isz, CV_8U, (ಶೂನ್ಯ*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ಸ್ಥಿರಾಂಕ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ಗಾಗಿ( j = 0; j < it.nplanes; j++, ++it )
                    ಸ್ಮರಣೆ_ನಕಲು(ptrs[1], ptrs[0], planesz);
            }
            ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_COARSE_GRAIN_ಬಫರ್)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMUnmap(q, u->handle,
                        0, ಖಾಲಿ, ಖಾಲಿ);
                CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMUnmap()");
                cl_ಮುಕ್ತಾಯ(q);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        {
            ಆದರೆ( iscontinuous )
            {
                Aligned_ದತ್ತಾಂಶ_Ptr<ನಿಜ, ಸುಳ್ಳು> alignedPtr((uchar*)srcptr, ಒಟ್ಟು, CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT);
                cl_int retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Write_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, ಒಟ್ಟು, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (ಶೂನ್ಯ*)u->handle, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)dstrawofs, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (CV_OPENCL_DISABLE_ಬಫರ್_RECT_OPERATIONS)
            {
                ಸ್ಥಿರಾಂಕ size_t padding = CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_ದತ್ತಾಂಶ_Ptr2_D<ಸುಳ್ಳು, ಸುಳ್ಳು> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                ಒಟ್ಟು = align_ಗಾತ್ರ(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                ಒಟ್ಟು = std::min(ಒಟ್ಟು, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Read_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, ಒಟ್ಟು, ptr, 0, 0, 0));
                ಗಾಗಿ( size_t i = 0; i < new_sz[1]; i++ )
                    ಸ್ಮರಣೆ_ನಕಲು( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Write_ಬಫರ್(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, ಒಟ್ಟು, ptr, 0, 0, 0));
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                Aligned_ದತ್ತಾಂಶ_Ptr2_D<ನಿಜ, ಸುಳ್ಳು> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Write_ಬಫರ್_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(ನಿಜ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ಆದರೆ ((u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_ಬಫರ್ ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        {
            u->markHostCopyObsolete(ನಿಜ);
        }
        u->markDeviceCopyObsolete(ಸುಳ್ಳು);
    }

    ಶೂನ್ಯ copy(UMat_ದತ್ತಾಂಶ* src, UMat_ದತ್ತಾಂಶ* dst, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
              ಸ್ಥಿರಾಂಕ size_t srcofs[], ಸ್ಥಿರಾಂಕ size_t srcstep[],
              ಸ್ಥಿರಾಂಕ size_t dstofs[], ಸ್ಥಿರಾಂಕ size_t dststep[], ತಾರ್ಕಿಕ _sync) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!src || !dst)
            ಹಿಂತಿರುಗಿ;

        size_t ಒಟ್ಟು = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ತಾರ್ಕಿಕ iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            ಒಟ್ಟು, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ src_autolock(src, dst);

        ಆದರೆ( !src->handle || (src->ದತ್ತಾಂಶ && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->ದತ್ತಾಂಶ + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            ಹಿಂತಿರುಗಿ;
        }
        ಆದರೆ( !dst->handle || (dst->ದತ್ತಾಂಶ && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->ದತ್ತಾಂಶ + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(ಸುಳ್ಳು);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
            ಆದರೆ ((dst->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_ಬಫರ್ ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
            {
                dst->markDeviceCopyObsolete(ನಿಜ);
            }
            ಹಿಂತಿರುಗಿ;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ಆದರೆ ((src->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
        {
            ಆದರೆ ((src->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
            {
                Context& ctx = Context::getDefault();
                ಸ್ಥಿರಾಂಕ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                ಆದರೆ( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು);
                    cl_int ಸ್ಥಿತಿ_2 = svmFns->fn_cl_ಸರತಿಗೆ_ಸೇರಿಸು_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            ಒಟ್ಟು, 0, ಖಾಲಿ, ಖಾಲಿ);
                    CV_OCL_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2, "clEnqueueSVMMemcpy()");
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    cl_ಮುಕ್ತಾಯ(q);
                    // This code is from MatAllocator::download()/upload()
                    ಪೂರ್ಣಾಂಕ isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
                        ಆದರೆ( sz[i] == 0 )
                        ಹಿಂತಿರುಗಿ;
                        ಆದರೆ( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (ಪೂರ್ಣಾಂಕ)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
                    {
                        ಆದರೆ( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ಸ್ಥಿರಾಂಕ Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    ಗಾಗಿ( j = 0; j < it.nplanes; j++, ++it )
                        ಸ್ಮರಣೆ_ನಕಲು(ptrs[1], ptrs[0], planesz);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ ((src->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->ದತ್ತಾಂಶ + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->ದತ್ತಾಂಶ + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        {
            ಆದರೆ( iscontinuous )
            {
                retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, ಒಟ್ಟು, 0, 0, 0);
                CV_OCL_CHECK_ಫಲಿತಾಂಶ(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (ಶೂನ್ಯ*)src->handle, (ಶೂನ್ಯ*)dst->handle, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)srcrawofs, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)dstrawofs, (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು).c_str());
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (CV_OPENCL_DISABLE_ಬಫರ್_RECT_OPERATIONS)
            {
                ಸ್ಥಿರಾಂಕ size_t padding = CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_ದತ್ತಾಂಶ_Ptr2_D<ಸುಳ್ಳು, ಸುಳ್ಳು> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT, padding*2);
                Aligned_ದತ್ತಾಂಶ_Ptr2_D<ಸುಳ್ಳು, ಸುಳ್ಳು> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_ದತ್ತಾಂಶ_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_ಒಟ್ಟು = align_ಗಾತ್ರ(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_ಒಟ್ಟು = std::min(src_ಒಟ್ಟು, src->size - new_srcrawofs);
                size_t dst_ಒಟ್ಟು = align_ಗಾತ್ರ(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_ಒಟ್ಟು = std::min(dst_ಒಟ್ಟು, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Read_ಬಫರ್(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_ಒಟ್ಟು, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Read_ಬಫರ್(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_ಒಟ್ಟು, dstptr, 0, 0, 0));

                ಗಾಗಿ( size_t i = 0; i < new_sz[1]; i++ )
                    ಸ್ಮರಣೆ_ನಕಲು( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Write_ಬಫರ್(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_ಒಟ್ಟು, dstptr, 0, 0, 0));
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_OCL_CHECK(retval = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        ಆದರೆ (retval == CL_SUCCESS)
        {
            CV_IMPL_ಕೂಡಿಸು(CV_IMPL_OCL)
        }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ಆದರೆ ((dst->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_ಬಫರ್ ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_ಬಫರ್_ಮುಖವಾಡ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        {
            dst->markHostCopyObsolete(ನಿಜ);
        }
        dst->markDeviceCopyObsolete(ಸುಳ್ಳು);

        ಆದರೆ( _sync )
        {
            CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ(q));
        }
    }

    ಬಫರ್_Pool_Controller* get_ಬಫರ್_Pool_Controller(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* id) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        ಆದರೆ (ctx.empty())
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_SVM
        ಆದರೆ ((svm::checkForceSVMUmatUsage() && (id == ಖಾಲಿ || strcmp(id, "OCL") == 0)) || (id != ಖಾಲಿ && strcmp(id, "SVM") == 0))
        {
            ಹಿಂತಿರುಗಿ &ctx.getImpl()->get_ಬಫರ್_Pool_SVM();
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ (id != ಖಾಲಿ && strcmp(id, "HOST_ALLOC") == 0)
        {
            ಹಿಂತಿರುಗಿ &ctx.getImpl()->get_ಬಫರ್_Pool_Host_Ptr();
        }
        ಆದರೆ (id != ಖಾಲಿ && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        ಹಿಂತಿರುಗಿ &ctx.getImpl()->get_ಬಫರ್_Pool();
    }

    MatAllocator* matStdAllocator;

    ಬದಲಾಯಿಸಬಹುದಾದ cv::Mutex cleanup_ಸರತಿ_Mutex;
    ಬದಲಾಯಿಸಬಹುದಾದ std::deque<UMat_ದತ್ತಾಂಶ*> cleanup_ಸರತಿ;

    ಶೂನ್ಯ flush_Cleanup_ಸರತಿ() ಸ್ಥಿರಾಂಕ
    {
        // TSAN false positive: see #27638
        ಆದರೆ (!cleanup_ಸರತಿ.empty())
        {
            std::deque<UMat_ದತ್ತಾಂಶ*> q;
            {
                cv::Auto_ಬೀಗ lock(cleanup_ಸರತಿ_Mutex);
                // TSAN false positive: see #27638
                q.swap(cleanup_ಸರತಿ);
            }
            ಗಾಗಿ (std::deque<UMat_ದತ್ತಾಂಶ*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    ಶೂನ್ಯ ಕೂಡಿಸು_ಗಮ್ಯ_Cleanup_ಸರತಿ(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_ಬೀಗ lock(cleanup_ಸರತಿ_Mutex);
            cleanup_ಸರತಿ.push_back(u);
        }
    }
};

ಸ್ಥಿರ OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    ಸ್ಥಿರ OpenCLAllocator* g_allocator = ಹೊಸ OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    ಹಿಂತಿರುಗಿ g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


ನಾಮಾಕಾಶ cv {

// three funcs below are implemented in umatrix.cpp
ಶೂನ್ಯ ಗಣ_ಗಾತ್ರ( UMat& m, ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2,
              ತಾರ್ಕಿಕ auto_ಹೆಜ್ಜೆಗಳು = ಸುಳ್ಳು );
ಶೂನ್ಯ finalizeHdr(UMat& m);

} // namespace cv


ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
ಶೂನ್ಯ convert_From_ಬಫರ್(ಶೂನ್ಯ* cl_mem_ಬಫರ್, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, UMat& dst)
{
    ಪೂರ್ಣಾಂಕ d = 2;
    ಪೂರ್ಣಾಂಕ sizes[] = { ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_ಮುಖವಾಡ) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    ಗಣ_ಗಾತ್ರ(dst, d, sizes, 0, ನಿಜ);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_ಬಫರ್;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, ಗಾತ್ರ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_ಬಫರ್ == mem_type);

    size_t ಒಟ್ಟು = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_ಗಾತ್ರ, ಗಾತ್ರ(size_t), &ಒಟ್ಟು, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((ಪೂರ್ಣಾಂಕ)ಹೆಜ್ಜೆ >= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * CV_ELEM_ಗಾತ್ರ(type));
    CV_Assert(ಒಟ್ಟು >= ಅಡ್ಡಸಾಲುಗಳು * ಹೆಜ್ಜೆ);

    // attach clBuffer to UMatData
    dst.u = ಹೊಸ UMat_ದತ್ತಾಂಶ(getOpenCLAllocator());
    dst.u->ದತ್ತಾಂಶ            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_ಬಫರ್;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag>(0);
    dst.u->handle          = cl_mem_ಬಫರ್;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = ಒಟ್ಟು;

    finalizeHdr(dst);
    dst.addref();

    ಹಿಂತಿರುಗಿ;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
ಶೂನ್ಯ convertFromImage(ಶೂನ್ಯ* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, ಗಾತ್ರ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, ಗಾತ್ರ(cl_image_format), &fmt, 0));

    ಪೂರ್ಣಾಂಕ ಆಳ = CV_8U;
    ಆಯ್ಕೆ (fmt.image_channel_ದತ್ತಾಂಶ_type)
    {
    ಸ್ಥಿತಿ CL_UNORM_INT8:
    ಸ್ಥಿತಿ CL_UNSIGNED_INT8:
        ಆಳ = CV_8U;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CL_SNORM_INT8:
    ಸ್ಥಿತಿ CL_SIGNED_INT8:
        ಆಳ = CV_8S;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CL_UNORM_INT16:
    ಸ್ಥಿತಿ CL_UNSIGNED_INT16:
        ಆಳ = CV_16U;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CL_SNORM_INT16:
    ಸ್ಥಿತಿ CL_SIGNED_INT16:
        ಆಳ = CV_16S;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CL_SIGNED_INT32:
        ಆಳ = CV_32S;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CL_FLOAT:
        ಆಳ = CV_32F;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CL_HALF_FLOAT:
        ಆಳ = CV_16F;
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    ಪೂರ್ಣಾಂಕ type = CV_8UC1;
    ಆಯ್ಕೆ (fmt.image_channel_ಕ್ರಮ)
    {
    ಸ್ಥಿತಿ CL_R:
    ಸ್ಥಿತಿ CL_A:
    ಸ್ಥಿತಿ CL_INTENSITY:
    ಸ್ಥಿತಿ CL_LUMINANCE:
        type = CV_MAKE_TYPE(ಆಳ, 1);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ CL_RG:
    ಸ್ಥಿತಿ CL_RA:
        type = CV_MAKE_TYPE(ಆಳ, 2);
        ನಿಲ್ಲಿಸು;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    ಸ್ಥಿತಿ CL_RGBA:
    ಸ್ಥಿತಿ CL_BGRA:
    ಸ್ಥಿತಿ CL_ARGB:
        type = CV_MAKE_TYPE(ಆಳ, 4);
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        ನಿಲ್ಲಿಸು;
    }

    size_t ಹೆಜ್ಜೆ = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ಅಡ್ಡಸಾಲು_PITCH, ಗಾತ್ರ(size_t), &ಹೆಜ್ಜೆ, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ಅಗಲ, ಗಾತ್ರ(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ಎತ್ತರ, ಗಾತ್ರ(size_t), &h, 0));

    dst.create((ಪೂರ್ಣಾಂಕ)h, (ಪೂರ್ಣಾಂಕ)w, type);

    cl_mem cl_ಬಫರ್ = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_Image_ಗಮ್ಯ_ಬಫರ್(q, clImage, cl_ಬಫರ್, src_origin, region, offset, 0, ಖಾಲಿ, ಖಾಲಿ));

    CV_OCL_CHECK(cl_ಮುಕ್ತಾಯ(q));

    ಹಿಂತಿರುಗಿ;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ಸ್ಥಿರ ಶೂನ್ಯ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint ಅಂಕೆ_Devices = 0;
    cl_int ಸ್ಥಿತಿ_2 = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, ಖಾಲಿ, &ಅಂಕೆ_Devices);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_DEVICE_NOT_ಸಿಕ್ಕಿತು) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(ಸ್ಥಿತಿ_2,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &ಅಂಕೆ_Devices).c_str());
    }

    ಆದರೆ (ಅಂಕೆ_Devices == 0)
    {
        devices.clear();
        ಹಿಂತಿರುಗಿ;
    }

    devices.resize((size_t)ಅಂಕೆ_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, ಅಂಕೆ_Devices, &devices[0], &ಅಂಕೆ_Devices));
}

ರಚನೆ PlatformInfo::Impl
{
    Impl(ಶೂನ್ಯ* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        ಆವೃತ್ತಿ_3 = getStrProp(CL_PLATFORM_ಆವೃತ್ತಿ);
        parseOpenCLVersion(ಆವೃತ್ತಿ_3, ಆವೃತ್ತಿ_Major, ಆವೃತ್ತಿ_Minor);
    }

    String getStrProp(cl_platform_info prop) ಸ್ಥಿರಾಂಕ
    {
        ಅಕ್ಷರ buf[1024];
        size_t sz=0;
        ಹಿಂತಿರುಗಿ clGetPlatformInfo(handle, prop, ಗಾತ್ರ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < ಗಾತ್ರ(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String ಆವೃತ್ತಿ_3;
    ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ_Major;
    ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ_Minor;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(ಶೂನ್ಯ* platform_id)
{
    p = ಹೊಸ Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    ಆದರೆ(p)
        p->release();
}

PlatformInfo::PlatformInfo(ಸ್ಥಿರಾಂಕ PlatformInfo& i)
{
    ಆದರೆ (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::ಕ್ರಿಯಾಕಾರಕ =(ಸ್ಥಿರಾಂಕ PlatformInfo& i)
{
    ಆದರೆ (i.p != p)
    {
        ಆದರೆ (i.p)
            i.p->addref();
        ಆದರೆ (p)
            p->release();
        p = i.p;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = ಶೂನ್ಯಸೂಚಕ;
}

PlatformInfo& PlatformInfo::ಕ್ರಿಯಾಕಾರಕ = (PlatformInfo&& i) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &i) {
        ಆದರೆ(p)
            p->release();
        p = i.p;
        i.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಪೂರ್ಣಾಂಕ PlatformInfo::device_ಸಂಖ್ಯೆ() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? (ಪೂರ್ಣಾಂಕ)p->devices.size() : 0;
}

ಶೂನ್ಯ PlatformInfo::getDevice(Device& device, ಪೂರ್ಣಾಂಕ d) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p && d < (ಪೂರ್ಣಾಂಕ)p->devices.size() );
    ಆದರೆ(p)
        device.ಗಣ(p->devices[d]);
}

String PlatformInfo::name() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->ಆವೃತ್ತಿ_3 : String();
}

ಪೂರ್ಣಾಂಕ PlatformInfo::ಆವೃತ್ತಿ_Major_2() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->ಆವೃತ್ತಿ_Major;
}

ಪೂರ್ಣಾಂಕ PlatformInfo::ಆವೃತ್ತಿ_Minor_2() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->ಆವೃತ್ತಿ_Minor;
}

ಸ್ಥಿರ ಶೂನ್ಯ getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint ಅಂಕೆ_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ಖಾಲಿ, &ಅಂಕೆ_Platforms));

    ಆದರೆ (ಅಂಕೆ_Platforms == 0)
    {
        platforms.clear();
        ಹಿಂತಿರುಗಿ;
    }

    platforms.resize((size_t)ಅಂಕೆ_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], &ಅಂಕೆ_Platforms));
}

ಶೂನ್ಯ getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    ಗಾಗಿ (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((ಶೂನ್ಯ*)&platforms[i]) );
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_ಗಮ್ಯ_Str(ಪೂರ್ಣಾಂಕ type)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* tab[CV_ಆಳ_MAX*16]=
    {
        "uchar", "uchar2", "uchar3", "uchar4", 0, 0, 0, "uchar8", 0, 0, 0, 0, 0, 0, 0, "uchar16",
        "char", "char2", "char3", "char4", 0, 0, 0, "char8", 0, 0, 0, 0, 0, 0, 0, "char16",
        "ushort", "ushort2", "ushort3", "ushort4", 0, 0, 0, "ushort8", 0, 0, 0, 0, 0, 0, 0, "ushort16",
        "short", "short2", "short3", "short4", 0, 0, 0, "short8", 0, 0, 0, 0, 0, 0, 0, "short16",
        "int", "int2", "int3", "int4", 0, 0, 0, "int8", 0, 0, 0, 0, 0, 0, 0, "int16",
        "float", "float2", "float3", "float4", 0, 0, 0, "float8", 0, 0, 0, 0, 0, 0, 0, "float16",
        "double", "double2", "double3", "double4", 0, 0, 0, "double8", 0, 0, 0, 0, 0, 0, 0, "double16",
        "half", "half2", "half3", "half4", 0, 0, 0, "half8", 0, 0, 0, 0, 0, 0, 0, "half16",
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // CV_16BF
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // CV_Bool
        "ulong", "ulong2", "ulong3", "ulong4", 0, 0, 0, "ulong8", 0, 0, 0, 0, 0, 0, 0, "ulong16",
        "long", "long2", "long3", "long4", 0, 0, 0, "long8", 0, 0, 0, 0, 0, 0, 0, "long16",
        "uint", "uint2", "uint3", "uint4", 0, 0, 0, "uint8", 0, 0, 0, 0, 0, 0, 0, "uint16"
    };
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(type), ಆಳ = CV_MAT_ಆಳ(type);
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಫಲಿತಾಂಶ = cn > 16 ? ಶೂನ್ಯಸೂಚಕ : tab[ಆಳ*16 + cn-1];
    CV_Assert(ಫಲಿತಾಂಶ);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* memop_Type_ಗಮ್ಯ_Str(ಪೂರ್ಣಾಂಕ type)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* tab[CV_ಆಳ_MAX*16] =
    {
        "uchar", "uchar2", "uchar3", "uchar4", 0, 0, 0, "uchar8", 0, 0, 0, 0, 0, 0, 0, "uchar16",
        "char", "char2", "char3", "char4", 0, 0, 0, "char8", 0, 0, 0, 0, 0, 0, 0, "char16",
        "ushort", "ushort2", "ushort3", "ushort4", 0, 0, 0, "ushort8", 0, 0, 0, 0, 0, 0, 0, "ushort16",
        "short", "short2", "short3", "short4", 0, 0, 0, "short8", 0, 0, 0, 0, 0, 0, 0, "short16",
        "int", "int2", "int3", "int4", 0, 0, 0, "int8", 0, 0, 0, 0, 0, 0, 0, "int16",
        "int", "int2", "int3", "int4", 0, 0, 0, "int8", 0, 0, 0, 0, 0, 0, 0, "int16",
        "ulong", "ulong2", "ulong3", "ulong4", 0, 0, 0, "ulong8", 0, 0, 0, 0, 0, 0, 0, "ulong16",
        "short", "short2", "short3", "short4", 0, 0, 0, "short8", 0, 0, 0, 0, 0, 0, 0, "short16",
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // CV_16BF
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // CV_Bool
        "ulong", "ulong2", "ulong3", "ulong4", 0, 0, 0, "ulong8", 0, 0, 0, 0, 0, 0, 0, "ulong16",
        "long", "long2", "long3", "long4", 0, 0, 0, "long8", 0, 0, 0, 0, 0, 0, 0, "long16",
        "uint", "uint2", "uint3", "uint4", 0, 0, 0, "uint8", 0, 0, 0, 0, 0, 0, 0, "uint16"
    };
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(type), ಆಳ = CV_MAT_ಆಳ(type);
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಫಲಿತಾಂಶ = cn > 16 ? ಶೂನ್ಯಸೂಚಕ : tab[ಆಳ*16 + cn-1];
    CV_Assert(ಫಲಿತಾಂಶ);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* vecop_Type_ಗಮ್ಯ_Str(ಪೂರ್ಣಾಂಕ type)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* tab[CV_ಆಳ_MAX*16] =
    {
        "uchar", "short", "uchar3", "int", 0, 0, 0, "int2", 0, 0, 0, 0, 0, 0, 0, "int4",
        "char", "short", "char3", "int", 0, 0, 0, "int2", 0, 0, 0, 0, 0, 0, 0, "int4",
        "ushort", "int", "ushort3", "int2",0, 0, 0, "int4", 0, 0, 0, 0, 0, 0, 0, "int8",
        "short", "int", "short3", "int2", 0, 0, 0, "int4", 0, 0, 0, 0, 0, 0, 0, "int8",
        "int", "int2", "int3", "int4", 0, 0, 0, "int8", 0, 0, 0, 0, 0, 0, 0, "int16",
        "int", "int2", "int3", "int4", 0, 0, 0, "int8", 0, 0, 0, 0, 0, 0, 0, "int16",
        "ulong", "ulong2", "ulong3", "ulong4", 0, 0, 0, "ulong8", 0, 0, 0, 0, 0, 0, 0, "ulong16",
        "short", "short2", "short3", "short4", 0, 0, 0, "short8", 0, 0, 0, 0, 0, 0, 0, "short16",
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // CV_16BF
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // CV_Bool
        "ulong", "ulong2", "ulong3", "ulong4", 0, 0, 0, "ulong8", 0, 0, 0, 0, 0, 0, 0, "ulong16",
        "long", "long2", "long3", "long4", 0, 0, 0, "long8", 0, 0, 0, 0, 0, 0, 0, "long16",
        "uint", "uint2", "uint3", "uint4", 0, 0, 0, "uint8", 0, 0, 0, 0, 0, 0, 0, "uint16"
    };
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(type), ಆಳ = CV_MAT_ಆಳ(type);
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಫಲಿತಾಂಶ = cn > 16 ? 0 : tab[ಆಳ*16 + cn-1];
    CV_Assert(ಫಲಿತಾಂಶ);
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

// Deprecated due to size of buf buffer being unknowable.
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* convertTypeStr(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth, ಪೂರ್ಣಾಂಕ cn, ಅಕ್ಷರ* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    ಹಿಂತಿರುಗಿ convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* convertTypeStr(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth, ಪೂರ್ಣಾಂಕ cn, ಅಕ್ಷರ* buf, size_t buf_ಗಾತ್ರ_2)
{
    ಆದರೆ( sdepth == ddepth )
        ಹಿಂತಿರುಗಿ "noconvert";
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *typestr = type_ಗಮ್ಯ_Str(CV_MAKETYPE(ddepth, cn));
    ಆದರೆ( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_ಗಾತ್ರ_2, "convert_%s", typestr);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_ಗಾತ್ರ_2, "convert_%s_sat_rte", typestr);
    ಇಲ್ಲದಿದ್ದರೆ
        snprintf(buf, buf_ಗಾತ್ರ_2, "convert_%s_sat", typestr);

    ಹಿಂತಿರುಗಿ buf;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* getOpenCLErrorString(ಪೂರ್ಣಾಂಕ error_ಸಂಕೇತ)
{
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_ಸಂಕೇತ(id) ಸ್ಥಿತಿ id: ಹಿಂತಿರುಗಿ #id
#ವ್ಯಾಖ್ಯಾನಿಸು CV_OCL_ಸಂಕೇತ_2(id, name) ಸ್ಥಿತಿ id: ಹಿಂತಿರುಗಿ #name
    ಆಯ್ಕೆ (error_ಸಂಕೇತ)
    {
    CV_OCL_ಸಂಕೇತ(CL_SUCCESS);
    CV_OCL_ಸಂಕೇತ(CL_DEVICE_NOT_ಸಿಕ್ಕಿತು);
    CV_OCL_ಸಂಕೇತ(CL_DEVICE_NOT_ಲಭ್ಯ);
    CV_OCL_ಸಂಕೇತ(CL_COMPILER_NOT_ಲಭ್ಯ);
    CV_OCL_ಸಂಕೇತ(CL_MEM_OBJECT_ಹಂಚಿಕೆ_FAILURE);
    CV_OCL_ಸಂಕೇತ(CL_ಹೊರಹೋಗು_ದ_RESOURCES);
    CV_OCL_ಸಂಕೇತ(CL_ಹೊರಹೋಗು_ದ_HOST_ಸ್ಮರಣೆ);
    CV_OCL_ಸಂಕೇತ(CL_PROFILING_INFO_NOT_ಲಭ್ಯ);
    CV_OCL_ಸಂಕೇತ(CL_MEM_COPY_OVERLAP);
    CV_OCL_ಸಂಕೇತ(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_ಸಂಕೇತ(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_ಸಂಕೇತ(CL_BUILD_ಕಾರ್ಯಕ್ರಮ_FAILURE);
    CV_OCL_ಸಂಕೇತ(CL_MAP_FAILURE);
    CV_OCL_ಸಂಕೇತ(CL_MISALIGNED_SUB_ಬಫರ್_ಸ್ಥಾನಾಂತರ);
    CV_OCL_ಸಂಕೇತ(CL_EXEC_ಸ್ಥಿತಿ_ERROR_FOR_EVENTS_ಒಳ_WAIT_LIST);
    CV_OCL_ಸಂಕೇತ(CL_COMPILE_ಕಾರ್ಯಕ್ರಮ_FAILURE);
    CV_OCL_ಸಂಕೇತ(CL_LINKER_NOT_ಲಭ್ಯ);
    CV_OCL_ಸಂಕೇತ(CL_LINK_ಕಾರ್ಯಕ್ರಮ_FAILURE);
    CV_OCL_ಸಂಕೇತ(CL_DEVICE_ವಿಭಜನೆ_FAILED);
    CV_OCL_ಸಂಕೇತ(CL_KERNEL_ARG_INFO_NOT_ಲಭ್ಯ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ಮೌಲ್ಯ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_DEVICE_TYPE);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_PLATFORM);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_DEVICE);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_CONTEXT);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ಸರತಿ_PROPERTIES);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_COMMAND_ಸರತಿ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_HOST_PTR);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_MEM_OBJECT);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_IMAGE_ಗಾತ್ರ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_SAMPLER);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ದ್ವಿಮಾನ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ಕಾರ್ಯಕ್ರಮ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ಕಾರ್ಯಕ್ರಮ_EXECUTABLE);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_KERNEL_NAME);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_KERNEL);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ARG_ಸೂಚ್ಯಂಕ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ARG_ಮೌಲ್ಯ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ARG_ಗಾತ್ರ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_KERNEL_ವಾದಗಳು);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_WORK_DIMENSION);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_WORK_ಗುಂಪು_ಗಾತ್ರ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_WORK_ಅಂಶ_ಗಾತ್ರ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_GLOBAL_ಸ್ಥಾನಾಂತರ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_EVENT);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_OPERATION);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_GL_OBJECT);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_ಬಫರ್_ಗಾತ್ರ);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_MIP_LEVEL);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_GLOBAL_WORK_ಗಾತ್ರ);
    // OpenCL 1.1
    CV_OCL_ಸಂಕೇತ(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_ಸಂಕೇತ(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_ಸಂಕೇತ(CL_INVALID_DEVICE_ವಿಭಜನೆ_ಎಣಿಕೆ);
    // OpenCL 2.0
    CV_OCL_ಸಂಕೇತ_2(-69, CL_INVALID_PIPE_ಗಾತ್ರ);
    CV_OCL_ಸಂಕೇತ_2(-70, CL_INVALID_DEVICE_ಸರತಿ);
    // Extensions
    CV_OCL_ಸಂಕೇತ_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_ಸಂಕೇತ_2(-1001, CL_PLATFORM_NOT_ಸಿಕ್ಕಿತು_KHR);
    CV_OCL_ಸಂಕೇತ_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_ಸಂಕೇತ_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_ಸಂಕೇತ_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_ಸಂಕೇತ_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    ಪೂರ್ವನಿಯೋಜಿತ: ಹಿಂತಿರುಗಿ "Unknown OpenCL error";
    }
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_OCL_ಸಂಕೇತ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_OCL_ಸಂಕೇತ_2
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ std::string ker_ಗಮ್ಯ_Str(ಸ್ಥಿರಾಂಕ Mat & k)
{
    ಪೂರ್ಣಾಂಕ ಅಗಲ = k.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1, ಆಳ = k.ಆಳ();
    ಸ್ಥಿರಾಂಕ T * ಸ್ಥಿರಾಂಕ ದತ್ತಾಂಶ = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    ಆದರೆ (ಆಳ <= CV_8S)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಗಲ; ++i)
            stream << "DIG(" << (ಪೂರ್ಣಾಂಕ)ದತ್ತಾಂಶ[i] << ")";
        stream << "DIG(" << (ಪೂರ್ಣಾಂಕ)ದತ್ತಾಂಶ[ಅಗಲ] << ")";
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಗಲ; ++i)
            stream << "DIG(" << ದತ್ತಾಂಶ[i] << "f)";
        stream << "DIG(" << ದತ್ತಾಂಶ[ಅಗಲ] << "f)";
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಗಲ; ++i)
            stream << "DIG(" << (ದಶಮಾಂಶ)ದತ್ತಾಂಶ[i] << "h)";
        stream << "DIG(" << (ದಶಮಾಂಶ)ದತ್ತಾಂಶ[ಅಗಲ] << "h)";
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಗಲ; ++i)
            stream << "DIG(" << ದತ್ತಾಂಶ[i] << ")";
        stream << "DIG(" << ದತ್ತಾಂಶ[ಅಗಲ] << ")";
    }

    ಹಿಂತಿರುಗಿ stream.str();
}

String kernel_ಗಮ್ಯ_Str(ಒಳಹರಿವು_Array _kernel, ಪೂರ್ಣಾಂಕ ddepth, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    ಪೂರ್ಣಾಂಕ ಆಳ = kernel.ಆಳ();
    ಆದರೆ (ddepth < 0)
        ddepth = ಆಳ;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    ಆದರೆ (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    ಆದರೆ (ddepth != ಆಳ)
        kernel.convert_ಗಮ್ಯ(kernel, ddepth);

    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ std::string (* func_t)(ಸ್ಥಿರಾಂಕ Mat &);
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ func_t funcs[CV_ಆಳ_MAX] = { ker_ಗಮ್ಯ_Str<uchar>, ker_ಗಮ್ಯ_Str<ಅಕ್ಷರ>, ker_ಗಮ್ಯ_Str<ushort>, ker_ಗಮ್ಯ_Str<ಗಿಡ್ಡ>,
                                                ker_ಗಮ್ಯ_Str<ಪೂರ್ಣಾಂಕ>, ker_ಗಮ್ಯ_Str<ದಶಮಾಂಶ>, ker_ಗಮ್ಯ_Str<ದ್ವಿಗುಣ>, ker_ಗಮ್ಯ_Str<hfloat>,
                                                0, ker_ಗಮ್ಯ_Str<ತಾರ್ಕಿಕ>, ker_ಗಮ್ಯ_Str<uint64_t>, ker_ಗಮ್ಯ_Str<int64_t>, ker_ಗಮ್ಯ_Str<ಚಿಹ್ನೆರಹಿತ> };
    ಸ್ಥಿರಾಂಕ func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    ಹಿಂತಿರುಗಿ cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#ವ್ಯಾಖ್ಯಾನಿಸು PROCESS_SRC(src) \
    ಮಾಡು \
    { \
        ಆದರೆ (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            ಗಾತ್ರ_2 csize = src.size(); \
            ಪೂರ್ಣಾಂಕ ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_ಆಳ(ctype), \
                ckercn = ಸದಿಶ_Widths[cdepth], cwidth = ccn * csize.ಅಗಲ; \
            ಆದರೆ (cwidth < ckercn || ckercn <= 0) \
                ಹಿಂತಿರುಗಿ 1; \
            ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ.push_back(cwidth); \
            ಆದರೆ (strat == OCL_ಸದಿಶ_OWN && ctype != ref_type) \
                ಹಿಂತಿರುಗಿ 1; \
            offsets.push_back(src.offset()); \
            ಹೆಜ್ಜೆಗಳು.push_back(src.ಹೆಜ್ಜೆ()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    ಆಗ ((ಶೂನ್ಯ)0, 0)

ಪೂರ್ಣಾಂಕ predict_Optimal_ಸದಿಶ_ಅಗಲ(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
                              ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
                              ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9,
                              Ocl_ಸದಿಶ_Strategy strat)
{
    ಸ್ಥಿರಾಂಕ ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    ಪೂರ್ಣಾಂಕ ಸದಿಶ_Widths[CV_ಆಳ_MAX] = {
        d.preferred_ಸದಿಶ_ಅಗಲ_Char(), d.preferred_ಸದಿಶ_ಅಗಲ_Char(),   // CV_8U,  CV_8S
        d.preferred_ಸದಿಶ_ಅಗಲ_Short(), d.preferred_ಸದಿಶ_ಅಗಲ_Short(), // CV_16U, CV_16S
        d.preferred_ಸದಿಶ_ಅಗಲ_Int(), d.preferred_ಸದಿಶ_ಅಗಲ_Float(),   // CV_32S, CV_32F
        d.preferred_ಸದಿಶ_ಅಗಲ_Double(), d.preferred_ಸದಿಶ_ಅಗಲ_Half(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferred_ಸದಿಶ_ಅಗಲ_Char(),                                 // CV_Bool
        d.preferred_ಸದಿಶ_ಅಗಲ_Long(), d.preferred_ಸದಿಶ_ಅಗಲ_Long(),   // CV_64U, CV_64S
        d.preferred_ಸದಿಶ_ಅಗಲ_Int()                                   // CV_32U
    };

    // if the device says don't use vectors
    ಆದರೆ (ಸದಿಶ_Widths[0] == 1)
    {
        // it's heuristic
        ಸದಿಶ_Widths[CV_8U] = ಸದಿಶ_Widths[CV_8S] = 4;
        ಸದಿಶ_Widths[CV_16U] = ಸದಿಶ_Widths[CV_16S] = ಸದಿಶ_Widths[CV_16F] = 2;
        ಸದಿಶ_Widths[CV_32S] = ಸದಿಶ_Widths[CV_32F] = ಸದಿಶ_Widths[CV_64F] = 1;
    }

    ಹಿಂತಿರುಗಿ check_Optimal_ಸದಿಶ_ಅಗಲ(ಸದಿಶ_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

ಪೂರ್ಣಾಂಕ check_Optimal_ಸದಿಶ_ಅಗಲ([[maybe_unused]] ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *ಸದಿಶ_Widths,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src1,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src2,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src3,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src4,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src5,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src6,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src7,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src8,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src9,
                            [[maybe_unused]] Ocl_ಸದಿಶ_Strategy strat)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
    ಹಿಂತಿರುಗಿ 1;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Assert(ಸದಿಶ_Widths);

    ಪೂರ್ಣಾಂಕ ref_type = src1.type();

    std::vector<size_t> offsets, ಹೆಜ್ಜೆಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    std::vector<ಪೂರ್ಣಾಂಕ> dividers, kercns;
    PROCESS_SRC(src1);
    PROCESS_SRC(src2);
    PROCESS_SRC(src3);
    PROCESS_SRC(src4);
    PROCESS_SRC(src5);
    PROCESS_SRC(src6);
    PROCESS_SRC(src7);
    PROCESS_SRC(src8);
    PROCESS_SRC(src9);

    size_t size = offsets.size();

    ಗಾಗಿ (size_t i = 0; i < size; ++i)
        ಆಗ (offsets[i] % dividers[i] != 0 || ಹೆಜ್ಜೆಗಳು[i] % dividers[i] != 0 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    ಪೂರ್ಣಾಂಕ kercn = *std::min_element(kercns.begin(), kercns.end());

    ಹಿಂತಿರುಗಿ kercn;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ predict_Optimal_ಸದಿಶ_ಅಗಲ_Max(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
                                 ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
                                 ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9)
{
    ಹಿಂತಿರುಗಿ predict_Optimal_ಸದಿಶ_ಅಗಲ(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_ಸದಿಶ_MAX);
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
ಶೂನ್ಯ build_Options_ಕೂಡಿಸು_ಮಾತೃಕೆ_Description(String& buildOptions, ಸ್ಥಿರಾಂಕ String& name, ಒಳಹರಿವು_Array _m)
{
    ಆದರೆ (!buildOptions.empty())
        buildOptions += " ";
    ಪೂರ್ಣಾಂಕ type = _m.type(), ಆಳ = CV_MAT_ಆಳ(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_ಗಮ್ಯ_Str(type),
            name.c_str(), ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, 1)),
            name.c_str(), (ಪೂರ್ಣಾಂಕ)CV_MAT_CN(type),
            name.c_str(), (ಪೂರ್ಣಾಂಕ)CV_ELEM_ಗಾತ್ರ(type),
            name.c_str(), (ಪೂರ್ಣಾಂಕ)CV_ELEM_SIZE1(type),
            name.c_str(), (ಪೂರ್ಣಾಂಕ)ಆಳ
            );
}


ರಚನೆ Image2D::Impl
{
    Impl(ಸ್ಥಿರಾಂಕ UMat &src, ತಾರ್ಕಿಕ norm, ತಾರ್ಕಿಕ alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        ಆದರೆ (handle)
            clReleaseMemObject(handle);
    }

    ಸ್ಥಿರ cl_image_format getImageFormat(ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ cn, ತಾರ್ಕಿಕ norm)
    {
        cl_image_format format;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        ಪೂರ್ಣಾಂಕ channelType = norm ? channelTypesNorm[ಆಳ] : channelTypes[ಆಳ];
        ಪೂರ್ಣಾಂಕ channel_ಕ್ರಮ = channelOrders[cn];
        format.image_channel_ದತ್ತಾಂಶ_type = (cl_channel_type)channelType;
        format.image_channel_ಕ್ರಮ = (cl_channel_ಕ್ರಮ)channel_ಕ್ರಮ;
        ಹಿಂತಿರುಗಿ format;
    }

    ಸ್ಥಿರ ತಾರ್ಕಿಕ isFormatSupported(cl_image_format format)
    {
        ಆದರೆ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        ಆದರೆ (!context)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        // Figure out how many formats are supported by this context.
        cl_uint ಅಂಕೆ_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, ಅಂಕೆ_Formats,
                                                ಖಾಲಿ, &ಅಂಕೆ_Formats);
        CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        ಆದರೆ (ಅಂಕೆ_Formats > 0)
        {
            Auto_ಬಫರ್<cl_image_format> formats(ಅಂಕೆ_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, ಅಂಕೆ_Formats,
                                             formats.ದತ್ತಾಂಶ(), ಖಾಲಿ);
            CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            ಗಾಗಿ (cl_uint i = 0; i < ಅಂಕೆ_Formats; ++i)
            {
                ಆದರೆ (!memcmp(&formats[i], &format, ಗಾತ್ರ(format)))
                {
                    ಹಿಂತಿರುಗಿ ನಿಜ;
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಶೂನ್ಯ init(ಸ್ಥಿರಾಂಕ UMat &src, ತಾರ್ಕಿಕ norm, ತಾರ್ಕಿಕ alias)
    {
        ಆದರೆ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        ಪೂರ್ಣಾಂಕ err, ಆಳ = src.ಆಳ(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(ಆಳ, cn, norm);

        ಆದರೆ (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        ಆದರೆ (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_ಸರತಿ ಸರತಿ_2 = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CL_ಆವೃತ್ತಿ_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ಸ್ಥಿರಾಂಕ Device & d = ocl::Device::getDefault();
        ಪೂರ್ಣಾಂಕ minor = d.device_ಆವೃತ್ತಿ_Minor(), major = d.device_ಆವೃತ್ತಿ_Major();
        CV_Assert(!alias || canCreateAlias(src));
        ಆದರೆ (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_ಅಗಲ      = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
            desc.image_ಎತ್ತರ     = src.ಅಡ್ಡಸಾಲುಗಳು;
            desc.image_ಆಳ      = 0;
            desc.image_array_ಗಾತ್ರ = 1;
            desc.image_ಅಡ್ಡಸಾಲು_pitch  = alias ? src.ಹೆಜ್ಜೆ[0] : 0;
            desc.image_ತುಂಡು_pitch = 0;
            desc.ಬಫರ್           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.ಅಂಕೆ_mip_ಮಟ್ಟಗಳು   = 0;
            desc.ಅಂಕೆ_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, ಖಾಲಿ, &err);
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        {
            CV_SUPPRESS_DEPRECATED_ಆರಂಭ
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, 0, ಖಾಲಿ, &err);
            CV_SUPPRESS_DEPRECATED_ಕೊನೆ
        }
        CV_OCL_DBG_CHECK_ಫಲಿತಾಂಶ(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ), static_cast<size_t>(src.ಅಡ್ಡಸಾಲುಗಳು), 1 };

        cl_mem dev_ದತ್ತಾಂಶ;
        ಆದರೆ (!alias && !src.isContinuous())
        {
            dev_ದತ್ತಾಂಶ = cl_Create_ಬಫರ್(context, CL_MEM_READ_ONLY, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * src.ಅಡ್ಡಸಾಲುಗಳು * src.elem_ಗಾತ್ರ(), ಖಾಲಿ, &err);
            CV_OCL_CHECK_ಫಲಿತಾಂಶ(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * src.ಅಡ್ಡಸಾಲುಗಳು * src.elem_ಗಾತ್ರ()), (ಶೂನ್ಯ*)dev_ದತ್ತಾಂಶ
                ).c_str());

            ಸ್ಥಿರಾಂಕ size_t roi[3] = {static_cast<size_t>(src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) * src.elem_ಗಾತ್ರ(), static_cast<size_t>(src.ಅಡ್ಡಸಾಲುಗಳು), 1};
            CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್_Rect(ಸರತಿ_2, (cl_mem)src.handle(ACCESS_READ), dev_ದತ್ತಾಂಶ, origin, origin,
                roi, src.ಹೆಜ್ಜೆ, 0, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * src.elem_ಗಾತ್ರ(), 0, 0, ಖಾಲಿ, ಖಾಲಿ));
            CV_OCL_DBG_CHECK(clFlush(ಸರತಿ_2));
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            dev_ದತ್ತಾಂಶ = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_ದತ್ತಾಂಶ != ಖಾಲಿ);

        ಆದರೆ (!alias)
        {
            CV_OCL_CHECK(cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್_ಗಮ್ಯ_Image(ಸರತಿ_2, dev_ದತ್ತಾಂಶ, handle, 0, origin, region, 0, ಖಾಲಿ, 0));
            ಆದರೆ (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(ಸರತಿ_2));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_ದತ್ತಾಂಶ));
            }
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_mem handle;
};

Image2D::Image2D() CV_NOEXCEPT
{
    p = ಖಾಲಿ;
}

Image2D::Image2D(ಸ್ಥಿರಾಂಕ UMat &src, ತಾರ್ಕಿಕ norm, ತಾರ್ಕಿಕ alias)
{
    p = ಹೊಸ Impl(src, norm, alias);
}

ತಾರ್ಕಿಕ Image2D::canCreateAlias(ಸ್ಥಿರಾಂಕ UMat &m)
{
    ತಾರ್ಕಿಕ ret = ಸುಳ್ಳು;
    ಸ್ಥಿರಾಂಕ Device & d = ocl::Device::getDefault();
    ಆದರೆ (d.image_From_ಬಫರ್_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        ಆದರೆ (pitchAlign && !(m.ಹೆಜ್ಜೆ % (pitchAlign * m.elem_ಗಾತ್ರ())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            ಆದರೆ (!m.u->ತಾತ್ಕಾಲಿಕ_UMat())
            {
                ret = ನಿಜ;
            }
        }
    }
    ಹಿಂತಿರುಗಿ ret;
}

ತಾರ್ಕಿಕ Image2D::isFormatSupported(ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ cn, ತಾರ್ಕಿಕ norm)
{
    cl_image_format format = Impl::getImageFormat(ಆಳ, cn, norm);

    ಹಿಂತಿರುಗಿ Impl::isFormatSupported(format);
}

Image2D::Image2D(ಸ್ಥಿರಾಂಕ Image2D & i)
{
    p = i.p;
    ಆದರೆ (p)
        p->addref();
}

Image2D & Image2D::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Image2D & i)
{
    ಆದರೆ (i.p != p)
    {
        ಆದರೆ (i.p)
            i.p->addref();
        ಆದರೆ (p)
            p->release();
        p = i.p;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = ಶೂನ್ಯಸೂಚಕ;
}

Image2D& Image2D::ಕ್ರಿಯಾಕಾರಕ = (Image2D&& i) CV_NOEXCEPT
{
    ಆದರೆ (ಇದು != &i) {
        ಆದರೆ (p)
            p->release();
        p = i.p;
        i.p = ಶೂನ್ಯಸೂಚಕ;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Image2D::~Image2D()
{
    ಆದರೆ (p)
        p->release();
}

ಶೂನ್ಯ* Image2D::ptr() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p ? p->handle : 0;
}

ತಾರ್ಕಿಕ internal::isOpenCLForced()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ value = ಸುಳ್ಳು;
    ಆದರೆ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", ಸುಳ್ಳು);
        initialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ value;
}

ತಾರ್ಕಿಕ internal::isPerformanceCheckBypassed()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ value = ಸುಳ್ಳು;
    ಆದರೆ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", ಸುಳ್ಳು);
        initialized = ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ value;
}

ತಾರ್ಕಿಕ internal::isCLBuffer(UMat& u)
{
    ಆದರೆ (isCudaBacked(u.u))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಶೂನ್ಯ* h = u.handle(ACCESS_RW);
    ಆದರೆ (!h)
        ಹಿಂತಿರುಗಿ ನಿಜ;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#ಆದರೆ 1
    ಆದರೆ ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಇಲ್ಲದಿದ್ದರೆ
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, ಗಾತ್ರ(type), &type, ಖಾಲಿ);
    ಆದರೆ (ret != CL_SUCCESS || type != CL_MEM_OBJECT_ಬಫರ್)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ರಚನೆ Timer::Impl
{
    ಸ್ಥಿರಾಂಕ ಸರತಿ ಸರತಿ_2;

    Impl(ಸ್ಥಿರಾಂಕ ಸರತಿ& q)
        : ಸರತಿ_2(q)
    {
    }

    ~Impl(){}

    ಶೂನ್ಯ ಆರಂಭ()
    {
        CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ((cl_command_ಸರತಿ)ಸರತಿ_2.ptr()));
        timer.ಆರಂಭ();
    }

    ಶೂನ್ಯ stop()
    {
        CV_OCL_DBG_CHECK(cl_ಮುಕ್ತಾಯ((cl_command_ಸರತಿ)ಸರತಿ_2.ptr()));
        timer.stop();
    }

    uint64 durationNS() ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ (uint64)(timer.get_ಸಮಯ_Sec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ಸ್ಥಿರಾಂಕ ಸರತಿ& q) : p(ಹೊಸ Impl(q)) { }
Timer::~Timer() { ಅಳಿಸು p; }

ಶೂನ್ಯ Timer::ಆರಂಭ()
{
    CV_Assert(p);
    p->ಆರಂಭ();
}

ಶೂನ್ಯ Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->durationNS();
}

}} // namespace

#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENCL
